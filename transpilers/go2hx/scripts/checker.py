#!/usr/bin/env python3
"""This script automates the process of checking if the Go binary and transpiled
Haxe binary do the same thing, i.e., Differential Testing.

The script will:
* compile the Go source
* execute the Go executable
* compile the Haxe source
* execute the Haxe executable
* check that the Go and Haxe executables produced the same output

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
flags.DEFINE_string('output_diff_go', '../output_diff_go', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('output_diff_hx', '../output_diff_hx', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('faulty', '../faulty', 'Copy faulty codes to a directory AFL doesnt touch')
flags.DEFINE_boolean('verify_output', True, 'Run both versions to see if outputs match')




################## [BEGIN] KEEP IN SYNC WITH AFL SCRIPT #################
#Script assumes these paths exist!!
Go_SRC = "new_runner.go"
Go_EXE = "./new_runner"
TRANSPILED_SOURCE_CODE = "golibs/_internal/Runner.hx"

TRANSPILED_EXE = ["haxe", "--main", "_internal.Runner", "-lib", "go2hx", "--interp"]

TRANSPILED_PACKAGE = "golibs"
KEEP_DIVERGENCE="2"
OK_TO_KEEP="1"
BUILD_FAILURE="3"
IGNORE_TEST_CASE="0"
Go_MUTATED = "mutated.go"
################## [END] KEEP IN SYNC WITH AFL SCRIPT ###################

EXE_TIMEOUT = 30 # seconds to wait for C compiled executable to run



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
    subprocess.run(["rm", Go_EXE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["rm", "-rf", TRANSPILED_PACKAGE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

def update_runner_hx(file_path):
    # Define the new code to be added
    new_code = '''    var formattedFloat = Math.round(_global_float * 100) / 100;
                        var floatStr = Std.string(formattedFloat);
                        if (floatStr.indexOf(".") == -1) {
                            floatStr += ".00";
                        } else if (floatStr.split(".")[1].length < 2) {
                            floatStr += "0";
                        }
                        var output = Std.string(_global_int) + ":" + floatStr;
                        Sys.println(output);
               '''

    # Read the original file content
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
    except FileNotFoundError:
        #print(f"Error: The file '{file_path}' was not found.")
        return TRANSPILED_FILE_MISSING


    # Find the location to insert the new code
    insertion_point = None
    for i, line in enumerate(lines):
        if '_func1();' in line:
            insertion_point = i + 1
            break

    if insertion_point is not None:
        # Insert the new code after the _func1() call
        lines.insert(insertion_point, new_code + '\n')

        # Write the updated content back to the file
        with open(file_path, 'w') as file:
            file.writelines(lines)

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
            return {"status": TRANSPILED_BUILD_FAILURE }

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
    
    #Get the Go exe output
    expected_output = execute_driver(Go_EXE)
    if expected_output["status"] != "OK":
        return expected_output["status"]
    
    if not FLAGS.verify_output:
        return TRANSPILED_VERIFIED
    

    # Get the Haxe exe output
    os.chdir("golibs/")
    actual_output = execute_driver(TRANSPILED_EXE)
    os.chdir("..")

    if actual_output["status"] != "OK":
        return actual_output["status"]
    
    logging.info("Expected from C: "+str(expected_output))
    logging.info("Actual from Go: "+str(actual_output))

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
    
    modify_result = update_runner_hx(TRANSPILED_SOURCE_CODE)
    if modify_result == TRANSPILED_FILE_MISSING:
        with open(FLAGS.status_file, 'a') as f:
            f.write("%s %s\n" % (argv[1], modify_result))
            f.write("\n==========================================****=========================================\n")
        copy_c_lib(Go_SRC, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_MISSING")    
        print(IGNORE_TEST_CASE)
        return
    
    transpile_status = verify_transpilation()
    # Will overwrite status.txt with latest run.
    with open(FLAGS.status_file, 'a') as f:
        f.write("%s %s\n" % (argv[1], transpile_status))
        f.write("\n==========================================****=========================================\n")

    #TODO: Move to a function.
    if transpile_status.startswith(TIMED_OUT):
        copy_c_lib(Go_SRC, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_TIMED_OUT")
        #print("timed out")
        print(IGNORE_TEST_CASE)

    if transpile_status == TRANSPILED_VERIFIED:
        print(OK_TO_KEEP) #1
    if transpile_status == TRANSPILED_BUILD_FAILURE:
        copy_c_lib(Go_SRC, os.path.join(FLAGS.build_failure, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_BUILD_FAILURE")
        print(BUILD_FAILURE) #3
    if transpile_status.startswith(TRANSPILED_OUTPUT_DIFF):
        copy_c_lib(Go_SRC, os.path.join(FLAGS.output_diff_go, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_Go")
        copy_c_lib(TRANSPILED_SOURCE_CODE, os.path.join(FLAGS.output_diff_hx, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_H" )
        print(KEEP_DIVERGENCE) #2

    
    clean_artifacts()
    

if __name__ == "__main__":
    app.run(main)
