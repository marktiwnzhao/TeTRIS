#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TRIAL="${1:-}"
DURATION="${2:-30m}"

if [ -z "$TRIAL" ]; then
    TRIAL=20
    while [ -e "$SCRIPT_DIR/fuzzing/output_$TRIAL" ]; do
        TRIAL=$((TRIAL + 1))
    done
fi

export LLVM_HOME="${LLVM_HOME:-/home/zzj/opt/llvm17}"
export C2GO_CLANG_HOME="${C2GO_CLANG_HOME:-/home/zzj/opt/clang-6.0}"
export AFL_PREFIX="${AFL_PREFIX:-/home/zzj/opt/aflplusplus}"
export VENV_DIR="${VENV_DIR:-/home/zzj/opt/venvs/tetris}"
export GO_HOME="${GO_HOME:-/home/zzj/opt/go}"
export C2GO_HOME="${C2GO_HOME:-/home/zzj/opt/go-work}"

export PATH="$VENV_DIR/bin:$C2GO_CLANG_HOME/bin:$LLVM_HOME/bin:$AFL_PREFIX/bin:$C2GO_HOME/bin:$GO_HOME/bin:$HOME/.cargo/bin:$PATH"
export CLANG_BIN="${CLANG_BIN:-$LLVM_HOME/bin/clang}"
export AFL_PATH="$AFL_PREFIX/bin"
export CAPSTONE_HOME="${CAPSTONE_HOME:-/home/zzj/opt/src/AFLplusplus/nyx_mode/QEMU-Nyx/capstone_v4}"
export LD_LIBRARY_PATH="$CAPSTONE_HOME:${LD_LIBRARY_PATH:-}"
export AFL_NO_FORKSRV=1
export AFL_NO_UI=1

OUT_DIR="$SCRIPT_DIR/fuzzing/output_$TRIAL"

echo "Running c2go calibration"
echo "  trial:    $TRIAL"
echo "  duration: $DURATION"
echo "  output:   $OUT_DIR"
echo

cd "$SCRIPT_DIR"

set +e
TETRIS_DURATION_SECONDS=$(python3 -c 'import re,sys; m=re.fullmatch(r"([0-9]+(?:\.[0-9]+)?)([smhd]?)",sys.argv[1]); assert m, "invalid duration"; print(float(m[1])*{"":1,"s":1,"m":60,"h":3600,"d":86400}[m[2]])' "$DURATION")
export TETRIS_DURATION_SECONDS
python3 fuzz.py "$TRIAL"
STATUS=$?
set -e

if [ "$STATUS" -ne 0 ] && [ "$STATUS" -ne 124 ]; then
    echo
    echo "Calibration failed with exit status $STATUS"
    exit "$STATUS"
fi

echo
echo "Campaign completed; see $OUT_DIR/campaign_exit.json for stop reason."

STATS="$OUT_DIR/fuzzer_stats"

if [ ! -f "$STATS" ]; then
    echo "Missing fuzzer stats: $STATS"
    exit 1
fi

echo
echo "Key fuzzer stats:"
awk -F: '
  BEGIN {
    keys["cycles_done"] = 1
    keys["execs_done"] = 1
    keys["execs_per_sec"] = 1
    keys["slowest_exec_ms"] = 1
    keys["verified_cases"] = 1
    keys["diverge_crashes"] = 1
    keys["unique_crashes"] = 1
    keys["targ_biuld_fail"] = 1
    keys["src_compile_fail"] = 1
    keys["target_mode"] = 1
    keys["paths_total"] = 1
    keys["paths_found"] = 1
  }
  {
    key = $1
    gsub(/[ \t]+$/, "", key)
    if (key in keys) print $0
  }
' "$STATS"

echo
echo "Full stats:"
echo "$STATS"
