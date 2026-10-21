import os
import subprocess
from datetime import datetime
import sys
import json
import shutil
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from oracle_events import run_campaign

def run_command(command):
    process = subprocess.run(command, shell=True)
    if process.returncode != 0:
        print(f"Error running command: {command}")
        exit(1)

def main(trial_number):
    original_dir = str(Path(__file__).resolve().parent)
    os.chdir(original_dir)
    root_dir = os.path.abspath(os.path.join(original_dir, "..", ".."))
    semantic_json_path = f"{root_dir}/generator/grammars/c/semantic.json"
    INPUT_SEED_FOLDER = f"{original_dir}/seeds/"
    os.environ["TETRIS_INIT_FILE_DIR"] = INPUT_SEED_FOLDER
    skip_tetris_build = os.environ.get("SKIP_TETRIS_BUILD") == "1"
    clang_bin = os.environ.get("CLANG_BIN") or shutil.which("clang")
    if clang_bin is None:
        print("Error: clang is not available from PATH")
        exit(1)
    os.environ["CLANG_BIN"] = clang_bin

    c2go_bin = shutil.which("c2go")
    if c2go_bin is None:
        print("Error: c2go is not available from PATH")
        exit(1)

    go_bin = shutil.which("go")
    if go_bin is None:
        print("Error: go is not available from PATH")
        exit(1)

    afl_qemu_trace = shutil.which("afl-qemu-trace")
    if afl_qemu_trace is None:
        default_afl_qemu_trace = "/home/zzj/opt/aflplusplus/bin/afl-qemu-trace"
        if os.path.exists(default_afl_qemu_trace):
            afl_qemu_trace = default_afl_qemu_trace
    if afl_qemu_trace is None:
        print("Error: afl-qemu-trace is not available from PATH")
        print("Set AFL_PATH to the directory containing afl-qemu-trace.")
        exit(1)
    os.environ["AFL_PATH"] = os.path.dirname(afl_qemu_trace)
    capstone_home = os.environ.get("CAPSTONE_HOME", "/home/zzj/opt/src/AFLplusplus/nyx_mode/QEMU-Nyx/capstone_v4")
    if os.path.exists(capstone_home):
        os.environ["LD_LIBRARY_PATH"] = capstone_home + ":" + os.environ.get("LD_LIBRARY_PATH", "")

    if not skip_tetris_build:
        # Update InitFileDir in semantic.json before regenerating TeTRIS.
        with open(semantic_json_path, "r+") as file:
            data = json.load(file)
            data["InitFileDir"] = INPUT_SEED_FOLDER

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

    if skip_tetris_build:
        print("Skipping TeTRIS build because SKIP_TETRIS_BUILD=1")
        if not os.path.exists(f"{root_dir}/core/afl-fuzz-tetris"):
            print("Error: core/afl-fuzz-tetris does not exist; unset SKIP_TETRIS_BUILD to build it.")
            exit(1)
    else:
        print("Building TeTRIS...")
        os.chdir(f"{root_dir}/generator")
        run_command("make clean")
        run_command("make LAN=c")
        os.chdir(original_dir)

    print("Building pre-transpilation script...")
    run_command("g++ -std=c++17 -O3 -funroll-loops -fpermissive scripts/instrument_testcase.cc -o scripts/instrument_testcase")

    # Set up the campaign output directory
    CAMPAIGN_OUT_DIR = campaign_out_dir
    os.environ['CAMPAIGN_OUT_DIR'] = CAMPAIGN_OUT_DIR
    os.makedirs(CAMPAIGN_OUT_DIR)
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "LOGS"))
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "build_failure"))
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "output_diff_c"))
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "output_diff_go"))
    os.makedirs(os.path.join(CAMPAIGN_OUT_DIR, "faulty"))
    go_cache_dir = os.path.join(CAMPAIGN_OUT_DIR, "go-cache")
    os.makedirs(go_cache_dir)
    os.environ["GOCACHE"] = go_cache_dir

    # Copy and prepare the fuzz environment
    print("Setting up the fuzz environment...")
    run_command(f"cp -r fuzz_env_template/ {CAMPAIGN_OUT_DIR}/fuzz_env")
    run_command(f"envsubst < fuzz_env_template/compile_commands.json > {CAMPAIGN_OUT_DIR}/fuzz_env/compile_commands.json")

    # Change directory to the fuzz environment
    os.chdir(os.path.join(CAMPAIGN_OUT_DIR, "fuzz_env"))

    # Define paths and scripts
    TeTRIS_FUZZER = f"{root_dir}/core/afl-fuzz-tetris"

    AFL_FLAGS = "-Q -t 10000000+ -m none -f mutated_code.c"
    INSTRUMENT_SCRIPT = f"{original_dir}/scripts/instrument_testcase mutated_code.c runner.c new_runner.c"
    VERIFIER_SCRIPT = f"{original_dir}/scripts/checker.py --check_compiler_warnings --status_file {CAMPAIGN_OUT_DIR}/verification_results --log_dir={CAMPAIGN_OUT_DIR}/LOGS"
    C2GO_BIN = f"c2go"

    # Run the fuzzer with the verifier and instrumenter scripts
    print("Running the fuzzer...")
    AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES = "1"
    AFL_SKIP_CPUFREQ = "1"
    AFL_NO_FORKSRV = "1"
    AFL_NO_UI = "1"
    os.environ['AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES'] = AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES
    os.environ['AFL_SKIP_CPUFREQ'] = AFL_SKIP_CPUFREQ
    os.environ['AFL_NO_FORKSRV'] = AFL_NO_FORKSRV
    os.environ['AFL_NO_UI'] = AFL_NO_UI

    fuzz_command = f"{TeTRIS_FUZZER} -i {INPUT_SEED_FOLDER} -o {CAMPAIGN_OUT_DIR} {AFL_FLAGS} -V \"{VERIFIER_SCRIPT}\" -I \"{INSTRUMENT_SCRIPT}\" -- {C2GO_BIN} transpile runner.c"
    run_campaign(CAMPAIGN_OUT_DIR, "c2go", fuzz_command)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 fuzz.py <trial_number>")
        exit(1)

    trial_number = sys.argv[1]
    main(trial_number)
