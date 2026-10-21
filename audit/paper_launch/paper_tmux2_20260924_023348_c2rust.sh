#!/usr/bin/env bash
set -o pipefail
export SKIP_TETRIS_BUILD=1
bash /home/zzj/TeTRIS/transpilers/c2rust/run_calibration.sh paper_tmux2_20260924_023348 24h 2>&1 | tee /home/zzj/TeTRIS/audit/paper_launch/paper_tmux2_20260924_023348_c2rust.log
status=${PIPESTATUS[0]}
echo "Campaign exit status: $status"
exit "$status"
