#!/usr/bin/env bash
set -o pipefail
export SKIP_TETRIS_BUILD=1
bash /home/zzj/TeTRIS/transpilers/c2go/run_calibration.sh paper_tmux_20260924_021229 24h 2>&1 | tee /home/zzj/TeTRIS/audit/paper_launch/paper_tmux_20260924_021229_c2go.log
status=${PIPESTATUS[0]}
echo "Campaign exit status: $status"
exit "$status"
