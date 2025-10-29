#!/usr/bin/env python3
"""This script automates the process of checking if the Haxe binary and transpiled
CPP binary do the same thing, i.e., Differential Testing.

The script will:
* compile and execute the Haxe source
* compile and execute the CPP source
* check that the Haxe and CPP executables produced the same output

A result outcome is written out to a std output, which is used by AFL to determine if this input is
interesting enough to be saved.
"""


from subprocess import PIPE
import subprocess
from pathlib import Path
from absl import app
from absl import flags
from absl import logging
import os
import math
import time


FLAGS = flags.FLAGS
flags.DEFINE_boolean('replay', False, 'Instruments and Transpiles library before checking.')
flags.DEFINE_string('replay_from_status', '', 'Replay files recorded in a status file.')
flags.DEFINE_string('status_file', 'status.txt', 'Full path of file to dump run results')
flags.DEFINE_string('build_failure', '../build_failure', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('output_diff_hx', '../output_diff_hx', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('output_diff_cpp', '../output_diff_cpp', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('faulty', '../faulty', 'Copy faulty codes to a directory AFL doesnt touch')
flags.DEFINE_boolean('verify_output', True, 'Run both versions to see if outputs match')



################## [BEGIN] KEEP IN SYNC WITH AFL SCRIPT #################
#Script assumes these paths exist!!
Haxe_SRC = "Runner.hx"

TRANSPILED_PACKAGE = "cpp_output/"
TRANSPILED_SOURCE_CODE = "cpp_output/src/Runner.cpp"


Haxe_EXE = ["haxe", "--main", "Runner", "--interp"]

CPP_BUILD = ["haxelib", "run", "hxcpp", "Build.xml"]
TRANSPILED_EXE = "cpp_output/Runner"

KEEP_DIVERGENCE="2"
OK_TO_KEEP="1"
BUILD_FAILURE="3"
IGNORE_TEST_CASE="0"
Haxe_MUTATED = "mutated_code.hx"
################## [END] KEEP IN SYNC WITH AFL SCRIPT ###################

EXE_TIMEOUT = 20 # seconds to wait for C compiled executable to run
BUILD_TIMEOUT = 30
################## [BEGIN] KEEP IN SYNC WITH ERROR CODES IN AFL-FUZZ.C #################
TRANSPILED_BUILD_FAILURE = "TRANSPILE_BUILD_FAILURE"
TRANSPILED_OUTPUT_DIFF = "OUTPUT_DIFF"
TRANSPILED_VERIFIED = "VERIFIED"
TRANSPILE_BUILD_OK = "TRANSPILE_BUILD_OK"
TIMED_OUT = "TIMED_OUT"
TRANSPILED_FILE_MISSING = "MISSING"
################## [END] KEEP IN SYNC WITH ERROR CODES IN AFL-FUZZ.C #################


def setup_logging():
    logging.get_absl_handler().use_absl_log_file()

def clean_artifacts():
    subprocess.run(["rm", Haxe_SRC], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["rm", "-rf", TRANSPILED_PACKAGE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def update_src_haxe(file_path):
    
    with open(file_path, 'r') as file:
        lines = file.readlines()
    insert_lines = [
        "        var formattedFloat = Math.round(global_float * 100) / 100;\n",
        '        Sys.println(global_int + ":" + Std.string(formattedFloat));\n'
    ]

    inside_main_function = False
    func1_call_found = False
    modified_lines = []

    for line in lines:
        if "function main" in line:
            inside_main_function = True
        if inside_main_function and "func1" in line:
            func1_call_found = True
        if func1_call_found:
            modified_lines.append(line)
            modified_lines.extend(insert_lines)
            func1_call_found = False  
        else:
            modified_lines.append(line)
        if inside_main_function and "}" in line:
            inside_main_function = False
    with open(file_path, 'w') as file:
        file.writelines(modified_lines)

def modify_cpp_file(file_path):
    try:
        # Read the contents of the file
        with open(file_path, 'r') as file:
            lines = file.readlines()
    except Exception as e:
        #print(f"Error: The file '{file_path}' was not found.")
        return TRANSPILED_FILE_MISSING

    # Flags to check if headers are added
    header_iostream_added = False
    header_cmath_added = False
    modified_lines = []
    inside_main_function = False
    func1_call_found = False

    # The lines to insert after the func1() call
    trace_line = (
        '    std::cout << _Runner::Runner_Fields__obj::global_int << '
        '":" << std::round(_Runner::Runner_Fields__obj::global_float * 100) / 100 '
        '<< std::endl;\n'
    )

    # Loop through the lines and modify the file content
    for line in lines:
        # Add #include<iostream> and #include<cmath> if not added yet
        if not header_iostream_added and '#include' in line:
            modified_lines.append('#include<iostream>\n')
            header_iostream_added = True
        if not header_cmath_added and '#include' in line:
            modified_lines.append('#include<cmath>\n')
            header_cmath_added = True

        # Detect the start of the main function
        if "void Runner_obj::main()" in line:
            inside_main_function = True
        
        # Look for the func1 call inside the main function
        if inside_main_function and "::Runner_obj::func1();" in line:
            func1_call_found = True
        
        # If func1 is found and we're still inside the main function, add the trace line after func1 call
        if func1_call_found:
            modified_lines.append(line)
            modified_lines.append(trace_line)
            func1_call_found = False  # Reset to avoid adding the trace multiple times
            continue
        
        # Check if we've exited the main function
        if inside_main_function and "}" in line:
            inside_main_function = False
        
        # Append the current line to the modified lines
        modified_lines.append(line)

    # Write the modified content back to the file
    try:
        with open(file_path, 'w') as file:
            file.writelines(modified_lines)
        #print(f"File '{file_path}' updated successfully.")
        return True
    except Exception as e:
       # print(f"Error writing to the file '{file_path}': {e}")
        return TRANSPILED_FILE_MISSING


def execute_driver(exe_name):
    """Execute the given executable and return its stdout output."""

    logging.info("Executing: %s", exe_name)
    try:
        
        exec_result = subprocess.run(
         exe_name,
         check=True,
         stdout=PIPE,stderr=PIPE,
         timeout=EXE_TIMEOUT)
        
        if exec_result.stderr:
            return {"status": TRANSPILED_BUILD_FAILURE}

        output = {}
        output["stdout"] = exec_result.stdout
        output["status"] = "OK"
        logging.info("Execution finished: %s", output)
        return output

    except subprocess.CalledProcessError as e:
        return {
            "status": TRANSPILED_BUILD_FAILURE
        }
    except subprocess.TimeoutExpired:
        return {
            "status": 'TIMED_OUT'
        }
    

def build_transpiled():
  try:
    os.chdir("cpp_output/")
    logging.info(f"Running command: {' '.join(CPP_BUILD)}")
    p = subprocess.run(CPP_BUILD, capture_output=True, text=True, timeout=BUILD_TIMEOUT)
    #logging.info(f"Command stdout:\n{p.stdout}")
    #logging.info(f"Command stderr:\n{p.stderr}")
    if p.returncode != 0:
        return TRANSPILED_BUILD_FAILURE
    return TRANSPILE_BUILD_OK
  except subprocess.TimeoutExpired:
      #logging.info("CPP build timeout!")
      return TRANSPILED_BUILD_FAILURE
  finally:
    os.chdir("..")


def verify_transpilation():
    
    #Get the Haxe exe output
    expected_output = execute_driver(Haxe_EXE)
    if expected_output["status"] != "OK":
        return expected_output["status"]
    
    
    transpile_build_status = build_transpiled()

    if transpile_build_status != TRANSPILE_BUILD_OK:
        return transpile_build_status
    
    if not FLAGS.verify_output:
        return TRANSPILED_VERIFIED
    
    logging.info("Building is successful!")
    # Get the Haxe cpp output
    actual_output = execute_driver(TRANSPILED_EXE)
   

    if actual_output["status"] != "OK":
        return actual_output["status"]
    
    logging.info("Expected from Haxe: "+str(expected_output))
    logging.info("Actual from CPP: "+str(actual_output))

    expected_array = convert_and_split(expected_output['stdout'])
    actual_array = convert_and_split(actual_output['stdout'])

    if expected_array == actual_array:
        return TRANSPILED_VERIFIED
    else:
        return TRANSPILED_OUTPUT_DIFF + " expected: " + str(expected_array) + " actual: " + str(actual_array)


def copy_c_lib(c_file, dst):
    copy_cmd = [
            "cp",
            c_file,
            dst
    ]
    logging.info(copy_cmd)
    p = subprocess.run(copy_cmd, capture_output=True, text=True)
    logging.info(p.returncode)
    logging.info(p.stdout)
    logging.info(p.stderr)
    return p.returncode == 0



def convert_and_split(stdout_bytes):
    stdout_str = stdout_bytes.decode('utf-8').strip()
    return stdout_str.split(":")


def main(argv):
    setup_logging()
    logging.info("VERIFYING: " + argv[1])
    update_src_haxe(Haxe_SRC)
    modify_result = modify_cpp_file(TRANSPILED_SOURCE_CODE)
    if modify_result == TRANSPILED_FILE_MISSING:
        with open(FLAGS.status_file, 'a') as f:
            f.write("%s %s\n" % (argv[1], modify_result))
            f.write("\n==========================================****=========================================\n")
        copy_c_lib(Haxe_SRC, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_MISSING")    
        print(IGNORE_TEST_CASE)
        return
    subprocess.run(["rm", TRANSPILED_EXE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    transpile_status = verify_transpilation()
    if transpile_status.startswith(TIMED_OUT):
        copy_c_lib(Haxe_SRC, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_TIMED_OUT")
        print(IGNORE_TEST_CASE)
    
    if transpile_status == TRANSPILED_VERIFIED:
        print(OK_TO_KEEP) #1
    if transpile_status == TRANSPILED_BUILD_FAILURE:
        copy_c_lib(Haxe_SRC, os.path.join(FLAGS.build_failure, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_BUILD_FAILURE")
        print(BUILD_FAILURE) #3
    if transpile_status.startswith(TRANSPILED_OUTPUT_DIFF):
        copy_c_lib(Haxe_SRC, os.path.join(FLAGS.output_diff_hx, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_H")
        copy_c_lib(TRANSPILED_SOURCE_CODE, os.path.join(FLAGS.output_diff_cpp, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_CPP" )
        print(KEEP_DIVERGENCE) #2

    
    clean_artifacts()
    

if __name__ == "__main__":
    app.run(main)
