#!/usr/bin/env python3
"""This script automates the process of checking if the Haxe binary and transpiled
Python binary do the same thing, i.e., Differential Testing.

The script will:
* compile and execute the Haxe source
* compile and execute the Python source
* check that the Haxe and Python executables produced the same output

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
flags.DEFINE_string('output_diff_py', '../output_diff_py', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('faulty', '../faulty', 'Copy faulty codes to a directory AFL doesnt touch')
flags.DEFINE_boolean('verify_output', True, 'Run both versions to see if outputs match')



################## [BEGIN] KEEP IN SYNC WITH AFL SCRIPT #################
#Script assumes these paths exist!!
Haxe_SRC = "Runner.hx"


TRANSPILED_SOURCE_CODE = "runner_output.py"


Haxe_EXE = ["haxe", "--main", "Runner", "--interp"]


TRANSPILED_EXE = [ "python3" ,"runner_output.py"]

KEEP_DIVERGENCE="2"
OK_TO_KEEP="1"
BUILD_FAILURE="3"
IGNORE_TEST_CASE="0"
Haxe_MUTATED = "mutated_code.hx"
################## [END] KEEP IN SYNC WITH AFL SCRIPT ###################

EXE_TIMEOUT = 30 
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
    subprocess.run(["rm", TRANSPILED_SOURCE_CODE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


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

def modify_python_file(file_path):
    try:
        # Read the contents of the file
        with open(file_path, 'r') as file:
            lines = file.readlines()
    except FileNotFoundError:
        print(f"Error: The file '{file_path}' was not found.")
        return TRANSPILED_FILE_MISSING

# Find the line containing the 'main' function definition and 'Runner.func1()' call
    main_function_index = -1
    func1_call_index = -1
    for i, line in enumerate(lines):
        if 'def main():' in line:
            main_function_index = i
        if main_function_index != -1 and 'Runner.func1()' in line:
            func1_call_index = i
            break

    # If the function definition and call are found, insert the required lines
    if func1_call_index != -1:
        # Lines to insert after `Runner.func1()` call
        lines.insert(func1_call_index + 1, '        formatted_float = round(_Runner_Runner_Fields_.global_float * 100) / 100\n')
        lines.insert(func1_call_index + 2, '        print(f"{_Runner_Runner_Fields_.global_int}:{formatted_float}")\n')

        # Write the modified lines back to the file
        with open(file_path, 'w') as file:
            file.writelines(lines)
        #print(f"File '{file_path}' modified successfully.")
    else:
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
    

def verify_transpilation():
    
    #Get the source exe's output
    expected_output = execute_driver(Haxe_EXE)
    if expected_output["status"] != "OK":
        return expected_output["status"]
    
    
    if not FLAGS.verify_output:
        return TRANSPILED_VERIFIED
    
    logging.info("Building is successful!")
    # Get the transpiled exe's output
    actual_output = execute_driver(TRANSPILED_EXE)
   

    if actual_output["status"] != "OK":
        return actual_output["status"]
    
    logging.info("Expected from Haxe: "+str(expected_output))
    logging.info("Actual from Python: "+str(actual_output))

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
    modify_result = modify_python_file(TRANSPILED_SOURCE_CODE)
    if modify_result == TRANSPILED_FILE_MISSING:
        with open(FLAGS.status_file, 'a') as f:
            f.write("%s %s\n" % (argv[1], modify_result))
            f.write("\n==========================================****=========================================\n")
        copy_c_lib(Haxe_SRC, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_MISSING")    
        print(IGNORE_TEST_CASE)
        return
    
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
        copy_c_lib(TRANSPILED_SOURCE_CODE, os.path.join(FLAGS.output_diff_py, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_PY" )
        print(KEEP_DIVERGENCE) #2

    
    clean_artifacts()
    

if __name__ == "__main__":
    app.run(main)
