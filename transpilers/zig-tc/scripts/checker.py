#!/usr/bin/env python3
"""This script automates the process of checking if the C code and transpiled
Zig binary do the same thing, i.e., Differential Testing.

The script will:
* compile the C source
* execute the C executable
* compile the Zig source
* execute the Zig executable
* check that the C and Zig executables produced the same output

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
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from oracle_events import record_event

FLAGS = flags.FLAGS
flags.DEFINE_boolean('replay', False, 'Instruments and Transpiles library before checking.')
flags.DEFINE_string('replay_from_status', '', 'Replay files recorded in a status file.')
flags.DEFINE_string('c_warnings', '-Wall -Wno-unused -Wno-implicit-function-declaration', '-W flags to pass onto the C compiler')
flags.DEFINE_string('clang_tidy_checks', 'cppcoreguidelines-init-variables,-clang-diagnostic-implicit-function-declaration,-clang-diagnostic-unused-value,-clang-analyzer-deadcode.*', 'Checks to enable in clang-tidy')
flags.DEFINE_string('status_file', 'status.txt', 'Full path of file to dump run results')
flags.DEFINE_string('build_failure', '../build_failure', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('output_diff_c', '../output_diff_c', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('output_diff_zig', '../output_diff_zig', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('faulty', '../faulty', 'Copy faulty codes to a directory AFL doesnt touch')
flags.DEFINE_boolean('check_compiler_warnings', True, 'if true, ignore testcase')
flags.DEFINE_boolean('verify_output', True, 'Run both versions to see if outputs match')




################## [BEGIN] KEEP IN SYNC WITH AFL SCRIPT #################
#Script assumes these paths exist!!
C_SRC = "new_runner.c"
C_EXE = "./new_runner.out"
TRANSPILED_SOURCE_CODE = "runner.zig"

TRANSPILED_EXE = "./runner"
KEEP_DIVERGENCE="2"
OK_TO_KEEP="1"
BUILD_FAILURE="3"
IGNORE_TEST_CASE="0"
C_MUTATED = "mutated_code.c"
################## [END] KEEP IN SYNC WITH AFL SCRIPT ###################

EXE_TIMEOUT = 20 # seconds to wait for C compiled executable to run
BUILD_TIMEOUT = 30

C_COMPILER = "clang"
CLANG_TIDY = "clang-tidy"

ZIG = "zig"
ZIG_BUILD = [
    ZIG,
    "build-exe",
    TRANSPILED_SOURCE_CODE,
    "-lc",
    "-femit-bin=runner",
]

################## [BEGIN] KEEP IN SYNC WITH ERROR CODES IN AFL-FUZZ.C #################
IS_TIDY = "IS_TIDY"
TIDY_WARNINGS = "TIDY_WARNINGS"
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
    subprocess.run(["rm", C_EXE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["rm", TRANSPILED_EXE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["rm", TRANSPILED_SOURCE_CODE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)



def modify_zig_file(file_path):
    # Read the contents of the file
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
    except Exception as e:
        #print(f"An error occurred: {e}")
        return TRANSPILED_FILE_MISSING

    # translate-c can fail after truncating its stdout file. Treat an empty
    # translation like a missing translation instead of indexing lines[0].
    if not lines:
        return TRANSPILED_FILE_MISSING
 
    # Define the line to be added at the beginning
    import_line = 'const std = @import("std");\n'

    # Define the code to be added at the end
    main_function_code = '''
    pub fn main() void {
        _ = func1();
        const int_global_float: i32 = @intFromFloat(global_float);
        std.debug.print("{}:{}:{}", .{ global_int, int_global_float, global_char });
    }
    '''

    # Check if the import line is already present
    if lines[0].strip() != import_line.strip():
        # Add the import line at the beginning
        lines.insert(0, import_line)

    # Add the main function code at the end
    lines.append(main_function_code)

    # Write the modified contents back to the file
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
        output = {}

        output["stdout"] = exec_result.stdout.decode('utf-8').strip()
        output["stderr"] = exec_result.stderr.decode('utf-8').strip()
        output["status"] = "OK"
        logging.info("Execution finished: %s", output)
        return output

    except subprocess.CalledProcessError as e:
        stdout = e.stdout.decode('utf-8', errors='replace').strip() if e.stdout else ""
        stderr = e.stderr.decode('utf-8', errors='replace').strip() if e.stderr else ""
        logging.info("EXECUTION FAILURE: %s exited with %s", exe_name, e.returncode)
        logging.info("EXECUTION FAILURE STDOUT: %s", stdout)
        logging.info("EXECUTION FAILURE STDERR: %s", stderr)
        return {
            "stdout": stdout,
            "stderr": stderr,
            "status": TRANSPILED_BUILD_FAILURE
        }
    except subprocess.TimeoutExpired:
        return {
            "status": 'TIMED_OUT'
        }

def check_clang_tidy(c_file):
    logging.info("Checking C source file with clang-tidy")
    clang_tidy_cmd = [
        CLANG_TIDY,
        "-checks="+FLAGS.clang_tidy_checks,
        "--quiet",
        c_file,
    ]
    p = subprocess.run(clang_tidy_cmd, capture_output=True, text=True)
    if p.stdout != "": 
        # The 1:-1 is to remove opening and closing brackets
        checks = [token for token in p.stdout.split() if token.startswith("[")]
        logging.info("CLANG-TIDY STDOUT:" + p.stderr)
        logging.info("CLANG-TIDY CHECKS:" + str(set(checks)))
        return TIDY_WARNINGS + str(set(checks))
    return IS_TIDY





def build_transpiled():
    try:
        p = subprocess.run(ZIG_BUILD, capture_output=True, text=True, timeout=BUILD_TIMEOUT)
        if p.returncode != 0:
            logging.info("ZIG BUILD STDOUT: %s", p.stdout)
            logging.info("ZIG BUILD STDERR: %s", p.stderr)
            return TRANSPILED_BUILD_FAILURE
        return TRANSPILE_BUILD_OK
    except subprocess.TimeoutExpired:
        logging.info("Zig build timeout!")
        return TRANSPILED_BUILD_FAILURE
    
    
def verify_transpilation():
    # First verify that uninstrumented c library has no warnings.
    tidy_status = check_clang_tidy(C_SRC)
    if tidy_status != IS_TIDY and FLAGS.check_compiler_warnings:
        return tidy_status


    expected_output = execute_driver(C_EXE)
    if expected_output["status"] != "OK":
        return expected_output["status"]
    
    transpile_build_status = build_transpiled()
    if transpile_build_status != TRANSPILE_BUILD_OK:
        return transpile_build_status

    if not FLAGS.verify_output:
        return TRANSPILED_VERIFIED

    # Get the C exe output
    

    # Get the Zig exe output
    actual_output = execute_driver(TRANSPILED_EXE)
    
    if actual_output["status"] != "OK":
        return actual_output["status"]
    
    logging.info("Expected from C: "+str(expected_output))
    logging.info("Actual from Zig: "+str(actual_output))

    expected_array = convert_and_split(expected_output['stdout'])
    actual_array = convert_and_split(actual_output['stderr'])

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
    #stdout_str = stdout_bytes.decode('utf-8').strip()
    return stdout_bytes.split(":")


def check_case(argv):
    setup_logging()
    logging.info("VERIFYING: " + argv[1])
    modify_result = modify_zig_file(TRANSPILED_SOURCE_CODE)
    
    if modify_result == TRANSPILED_FILE_MISSING:
        record_event(FLAGS.status_file, 'zig-tc', argv[1], C_MUTATED, modify_result)
        with open(FLAGS.status_file, 'a') as f:
            f.write("%s %s\n" % (argv[1], modify_result))
            f.write("\n==========================================****=========================================\n")
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_MISSING")    
        print(IGNORE_TEST_CASE)
        return
    
    transpile_status = verify_transpilation()
    record_event(FLAGS.status_file, 'zig-tc', argv[1], C_MUTATED, transpile_status)
    # Will overwrite status.txt with latest run.
    with open(FLAGS.status_file, 'a') as f:
        f.write("%s %s\n" % (argv[1], transpile_status))
        f.write("\n==========================================****=========================================\n")

    #TODO: Move to a function.
    if transpile_status.startswith(TIMED_OUT):
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_TIMED_OUT")
        #print("timed out")
        print(IGNORE_TEST_CASE)
    if transpile_status.startswith(TIDY_WARNINGS):
        #print("tidy warnings")
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_TIDY_WARNINGS")
        print(IGNORE_TEST_CASE)

    if transpile_status == TRANSPILED_VERIFIED:
        print(OK_TO_KEEP) #1
    if transpile_status == TRANSPILED_BUILD_FAILURE:
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.build_failure, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_BUILD_FAILURE")
        print(BUILD_FAILURE) #3
    if transpile_status.startswith(TRANSPILED_OUTPUT_DIFF):
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.output_diff_c, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_C")
        copy_c_lib(TRANSPILED_SOURCE_CODE, os.path.join(FLAGS.output_diff_zig, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_Z" )
        print(KEEP_DIVERGENCE) #2

    
    clean_artifacts()
    

def main(argv):
    try:
        return check_case(argv)
    except Exception as exc:
        record_event(FLAGS.status_file, 'zig-tc', argv[1] if len(argv) > 1 else '',
                     C_MUTATED, 'CHECKER_ERROR ' + repr(exc))
        raise


if __name__ == "__main__":
    app.run(main)
