import os
import subprocess
from datetime import datetime
import sys
import json

def run_command(command):
    process = subprocess.run(command, shell=True)
    if process.returncode != 0:
        print(f"Error running command: {command}")
        exit(1)

def main(trial_number):
    original_dir = os.getcwd()
    root_dir = os.path.abspath(os.path.join(original_dir, "..", ".."))
    semantic_json_path = f"{root_dir}/generator/grammars/haxe/semantic.json"
    INPUT_SEED_FOLDER = f"{original_dir}/seeds/"
    
    # Update InitFileDir in semantic.json
    with open(semantic_json_path, "r+") as file:
        data = json.load(file)
        data["InitFileDir"] = INPUT_SEED_FOLDER  # Set full path to InitFileDir
        
        # Write changes back to the file
        file.seek(0)
        json.dump(data, file, indent=4)
        file.truncate()

    # Check if the fuzzing directory exists, if not, create it
    parent_fuzz_dir = os.path.join(original_dir, "fuzzing")
    if not os.path.exists(parent_fuzz_dir):
        print("Creating fuzzing directory...")
        os.makedirs(parent_fuzz_dir)

    # Check if the campaign output directory already exists
    campaign_out_dir = os.path.join(parent_fuzz_dir, f"output_{trial_number}")
    if os.path.exists(campaign_out_dir):
        print(f"Directory {campaign_out_dir} already exists. Please provide a new trial number.")
        exit(1)

    #Build TeTRIS
    print("Building TeTRIS...")
    os.chdir(f"{root_dir}/generator")
    run_command("make clean")
    run_command("make LAN=haxe")
    os.chdir(original_dir)

    print("Building pre-transpilation script...")
    run_command("g++ -std=c++17 -O3 -funroll-loops -fpermissive scripts/instrument_testcase.cc -o scripts/instrument_testcase")

    # Set up the campaign output directory
    CAMPAIGN_OUT_DIR = campaign_out_dir
    os.environ['CAMPAIGN_OUT_DIR'] = CAMPAIGN_OUT_DIR
    os.makedirs(CAMPAIGN_OUT_DIR)
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "LOGS"))
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "build_failure"))
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "outputdiff_hx"))
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "outputdiff_py"))
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "faulty"))

    # Copy and prepare the fuzz environment
    print("Setting up the fuzz environment...")
    run_command(f"cp -r fuzz_env_template/ {CAMPAIGN_OUT_DIR}/fuzz_env")
    
    # Change directory to the fuzz environment
    os.chdir(os.path.join(CAMPAIGN_OUT_DIR, "fuzz_env"))

    # Define paths and scripts
    TeTRIS_FUZZER = f"{root_dir}/core/afl-fuzz-tetris"
    
    AFL_FLAGS = "-Q -t 10000000+ -m none -f mutated_code.hx"
    INSTRUMENT_SCRIPT = f"{original_dir}/scripts/instrument_testcase mutated_code.hx Runner.hx"
    VERIFIER_SCRIPT = f"{original_dir}/scripts/checker.py --status_file {CAMPAIGN_OUT_DIR}/verification_results --log_dir={CAMPAIGN_OUT_DIR}/LOGS"
    HXPY_BIN = f"haxe"

    # Run the fuzzer with the verifier and instrumenter scripts
    print("Running the fuzzer...")
    AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES = "1"
    AFL_SKIP_CPUFREQ = "1"
    os.environ['AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES'] = AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES
    os.environ['AFL_SKIP_CPUFREQ'] = AFL_SKIP_CPUFREQ
 
    fuzz_command = f"{TeTRIS_FUZZER} -i {INPUT_SEED_FOLDER} -o {CAMPAIGN_OUT_DIR} {AFL_FLAGS} -V \"{VERIFIER_SCRIPT}\" -I \"{INSTRUMENT_SCRIPT}\" -- {HXPY_BIN} -python runner_output.py -main Runner"
    run_command(fuzz_command)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 fuzz.py <trial_number>")
        exit(1)

    trial_number = sys.argv[1]
    main(trial_number)
