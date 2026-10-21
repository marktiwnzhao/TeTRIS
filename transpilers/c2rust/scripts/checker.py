#!/usr/bin/env python3
"""This script automates the process of checking if the C code and transpiled
rust binary do the same thing.

The script will:
* compile the C source
* execute the C executable
* compile the Rust source
* execute the Rust executable
* check that the C and Rust executables produced the same output

A result outcome is written out to a file, which is used by AFL to determine if this input is
interesting enough to be saved.
"""

from subprocess import PIPE
import subprocess
import sys
from pathlib import Path
import os
import shutil
import math
import time
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from oracle_events import record_event

from absl import app
from absl import flags
from absl import logging

FLAGS = flags.FLAGS
flags.DEFINE_boolean('replay', False, 'Instruments and Transpiles library before checking.')
flags.DEFINE_string('replay_from_status', '', 'Replay files recorded in a status file.')
flags.DEFINE_string('c_warnings', '-Wall -Wno-unused -Wno-implicit-function-declaration', '-W flags to pass onto the C compiler')
flags.DEFINE_string('clang_tidy_checks', 'cppcoreguidelines-init-variables,-clang-diagnostic-implicit-function-declaration,-clang-analyzer-deadcode.*', 'Checks to enable in clang-tidy')
flags.DEFINE_string('status_file', 'status.txt', 'Full path of file to dump run results')
flags.DEFINE_string('build_failure', '../build_failure', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('output_diff_c', '../output_diff_c', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('output_diff_rust', '../output_diff_rust', 'Copy interesting results to a directory AFL doesnt touch')
flags.DEFINE_string('faulty', '../faulty', 'Copy faulty codes to a directory AFL doesnt touch')
flags.DEFINE_boolean('check_compiler_warnings', True, 'if true, ignore testcase')
flags.DEFINE_boolean('verify_output', True, 'Run both versions to see if outputs match')


RUST_WARNINGS_TO_IGNORE = [
    "#[deny(arithmetic_overflow)], #[warn(path_statements)]"
]

################## [BEGIN] KEEP IN SYNC WITH AFL SCRIPT #################
#Script assumes these paths exist!!
#TRANSPILER = "../transpiler_latest/c2rust_/target/release/c2rust" 
C_SRC = "c_src/new_runner.c"
C_EXE = "c_src/new_runner.out"
TRANSPILED_PACKAGE = "transpiled_code"
TRANSPILED_PACKAGE_MANIFEST = "transpiled_code/Cargo.toml"
TRANSPILED_SOURCE_CODE = "transpiled_code/src/runner.rs"
tail_rust = """unsafe fn main_0() -> ::core::ffi::c_int {
    func1();
    println!("{}:{:.2}:{:}", global_int, global_float as ::core::ffi::c_double, global_char as ::core::ffi::c_int);
    return 0;
}
pub fn main() {
    unsafe { ::std::process::exit(main_0() as i32) }
}"""

TRANSPILED_EXE = "transpiled_code/target/release/runner"
KEEP_DIVERGENCE="2"
OK_TO_KEEP="1"
BUILD_FAILURE="3"
IGNORE_TEST_CASE="0"
C_MUTATED = "mutated_code.c"
################## [END] KEEP IN SYNC WITH AFL SCRIPT ###################

EXE_TIMEOUT = 30 # seconds to wait for C compiled executable to run
BUILD_TIMEOUT = 30
C_COMPILER = "clang"
CLANG_TIDY = "clang-tidy"
C_COMPILER_OPTIONS = "-fsanitize=undefined"
CARGO = "cargo"
CARGO_CMD = [
    CARGO,
    "build",
    "--release",
    "--manifest-path=Cargo.toml",
]

################## [BEGIN] KEEP IN SYNC WITH ERROR CODES IN AFL-FUZZ.C #################
IS_TIDY = "IS_TIDY"
COMPILE_FAILURE = "COMPILE_FAILURE"
COMPILE_WARNINGS = "COMPILE_WARNINGS"
TIDY_WARNINGS = "TIDY_WARNINGS"
COMPILE_SUCCESS = "COMPILE_SUCCESS"
TRANSPILED_BUILD_FAILURE = "TRANSPILE_BUILD_FAILURE"
TRANSPILED_OUTPUT_DIFF = "OUTPUT_DIFF"
TRANSPILED_VERIFIED = "VERIFIED"
UNSAFE_C_CODE = "UNSAFE_C_CODE"
TRANSPILE_BUILD_OK = "TRANSPILE_BUILD_OK"
TIMED_OUT = "TIMED_OUT"
TRANSPILED_FILE_MISSING = "MISSING"
################## [END] KEEP IN SYNC WITH ERROR CODES IN AFL-FUZZ.C #################

def setup_logging():
    logging.get_absl_handler().use_absl_log_file()

def clean_artifacts():
    subprocess.run(["rm", C_EXE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(["rm", "-rf", TRANSPILED_PACKAGE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

def execute_driver(exe_name, run_twice=False):
    """Execute the given executable and return its stdout output."""

    logging.info("Executing: %s", exe_name)
    try:
        exec_result = subprocess.run(
         exe_name,
         check=True,
         stdout=PIPE,stderr=PIPE,
         timeout=EXE_TIMEOUT)
        output = {}

        if run_twice:
            second_exec_result = subprocess.run(
                exe_name,
                check=True,
                stdout=PIPE,stderr=PIPE,
                timeout=EXE_TIMEOUT
            )

            if exec_result.stdout != second_exec_result.stdout:
                return {
                    "status": "UNDETERMINISTIC"
                }

        output["stdout"] = exec_result.stdout
        output["status"] = "OK"
        logging.info("Execution finished: %s", output)
        return output

    except subprocess.CalledProcessError as e:
        return {
            "status": 'NZEC'
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


def compile_c_file(c_file, output_c_exe_name, check_warnings):
    """Compile the given C source file to produce the given executable."""
    logging.info("Compiling C source file with clang")
    warnings = FLAGS.c_warnings.split(" ")
    compile_cmd = [
        C_COMPILER,
        C_COMPILER_OPTIONS,
    ]
    compile_cmd.extend(warnings)
    compile_cmd.extend([
        "-o", 
        output_c_exe_name,
        c_file
    ])

    p = subprocess.run(compile_cmd, capture_output=True, text=True)
    
    if check_warnings:
        if p.stderr != "": 
            # The 1:-1 is to remove opening and closing brackets
            warnings = [token[1:-1] for token in p.stderr.split() if token.startswith("[-W")]
            logging.info("CLANG STDERR:" + p.stderr)
            logging.info("CLANG WARNINGS:" + str(set(warnings)))
            return COMPILE_WARNINGS + str(set(warnings))

    if p.returncode != 0:
        return COMPILE_FAILURE
    return COMPILE_SUCCESS




def build_transpiled():
    try:
        p = subprocess.run(CARGO_CMD, cwd=TRANSPILED_PACKAGE, capture_output=True, text=True, timeout=BUILD_TIMEOUT)
        if p.returncode != 0:
            logging.info("Cargo build stderr: %s", p.stderr)
            for ignore_rust_check in RUST_WARNINGS_TO_IGNORE:
                if ignore_rust_check in p.stderr:
                    return UNSAFE_C_CODE
            return TRANSPILED_BUILD_FAILURE
        return TRANSPILE_BUILD_OK
    except subprocess.TimeoutExpired:
        logging.info("Cargo build timeout!")
        return TRANSPILED_BUILD_FAILURE
    
def verify_transpilation():
    # First verify that uninstrumented c library has no warnings.
    tidy_status = check_clang_tidy(C_SRC)
    if tidy_status != IS_TIDY and FLAGS.check_compiler_warnings:
        return tidy_status

  
    # Get the C exe output
    expected_output = execute_driver(C_EXE)
    if expected_output["status"] != "OK":
        return expected_output["status"]

    transpile_build_status = build_transpiled()
    if transpile_build_status != TRANSPILE_BUILD_OK:
        return transpile_build_status

    if not FLAGS.verify_output:
        return TRANSPILED_VERIFIED

    
    # Get the Rust exe output
    actual_output = execute_driver(TRANSPILED_EXE)
    logging.info("Expected from C: "+str(expected_output))
    logging.info("Actual from Rust: "+str(actual_output))

    if actual_output['status'] != 'OK':
        return actual_output['status']

    expected_array = convert_and_split(expected_output['stdout'])
    actual_array = convert_and_split(actual_output['stdout'])

    if expected_array == actual_array:
        return TRANSPILED_VERIFIED
    else:
        return TRANSPILED_OUTPUT_DIFF + " expected: " + str(expected_array) + " actual: " + str(actual_array)
    
    #if actual_output["stdout"] != expected_output["stdout"]:
    #    return TRANSPILED_OUTPUT_DIFF + " expected: " + str(expected_output) + " actual: " + str(actual_output)
    
    #return TRANSPILED_VERIFIED


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

def append_tail_to_file(file_path, tail_string):
    try:
        with open(file_path, 'a') as file:  # Open the file in append mode
            file.write("\n" + tail_string)  # Add a newline before the tail string
        #print(f"Successfully appended to {file_path}")
    except Exception as e:
        #print(f"An error occurred: {e}")
        return TRANSPILED_FILE_MISSING

def convert_and_split(stdout_bytes):
    stdout_str = stdout_bytes.decode('utf-8').strip()
    return stdout_str.split(":")


def check_case(argv):
    setup_logging()
    logging.info("VERIFYING: " + argv[1])
    modify_result = append_tail_to_file(TRANSPILED_SOURCE_CODE, tail_rust)

    if modify_result == TRANSPILED_FILE_MISSING:
        record_event(FLAGS.status_file, 'c2rust', argv[1], C_MUTATED, modify_result)
        with open(FLAGS.status_file, 'a') as f:
            f.write("%s %s\n" % (argv[1], modify_result))
            f.write("\n==========================================****=========================================\n")
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_MISSING")    
        print(IGNORE_TEST_CASE)
        return
        

    transpile_status = verify_transpilation()
    record_event(FLAGS.status_file, 'c2rust', argv[1], C_MUTATED, transpile_status)
    # Will overwrite status.txt with latest run.
    with open(FLAGS.status_file, 'a') as f:
        f.write("%s %s\n" % (argv[1], transpile_status))
        f.write("\n==========================================****=========================================\n")

    #TODO: Move to a function.
    if transpile_status.startswith(COMPILE_WARNINGS) or transpile_status.startswith(COMPILE_FAILURE):
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_COMPILE_FAIL")
        print(IGNORE_TEST_CASE) #0
    if transpile_status in ("NZEC", "UNDETERMINISTIC"):
        print(IGNORE_TEST_CASE)
    if transpile_status.startswith(TIMED_OUT):
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) +"_TIMED_OUT")
        print(IGNORE_TEST_CASE)
    if transpile_status.startswith(TIDY_WARNINGS):
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_TIDY_WARNINGS")
        print(IGNORE_TEST_CASE)
    if transpile_status == UNSAFE_C_CODE:
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.faulty, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_UNSAFE_C_CODE")
        print(IGNORE_TEST_CASE)
    

    if transpile_status == TRANSPILED_VERIFIED:
        print(OK_TO_KEEP) #1
    if transpile_status == TRANSPILED_BUILD_FAILURE:
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.build_failure, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_BUILD_FAILURE")
        print(BUILD_FAILURE) #3
    if transpile_status.startswith(TRANSPILED_OUTPUT_DIFF):
        copy_c_lib(C_MUTATED, os.path.join(FLAGS.output_diff_c, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_C")
        copy_c_lib(TRANSPILED_SOURCE_CODE, os.path.join(FLAGS.output_diff_rust, os.path.basename(argv[1])) + str(math.floor(time.time())) + "_OUTPUT_DIFF_R" )
        print(KEEP_DIVERGENCE) #2

    
    clean_artifacts()
    

def main(argv):
    try:
        return check_case(argv)
    except Exception as exc:
        record_event(FLAGS.status_file, 'c2rust', argv[1] if len(argv) > 1 else '',
                     C_MUTATED, 'CHECKER_ERROR ' + repr(exc))
        raise


if __name__ == "__main__":
    app.run(main)
