#!/usr/bin/env python3
"""Replay zig-tc build_failure cases and recover missing error messages."""

from __future__ import annotations

import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import csv
import os
import shutil
import subprocess
import tempfile
from pathlib import Path


MAIN_TAIL = """
pub fn main() void {
    _ = func1();
    const int_global_float: i32 = @intFromFloat(global_float);
    std.debug.print("{}:{}:{}", .{ global_int, int_global_float, global_char });
}
"""


def env() -> dict[str, str]:
    e = os.environ.copy()
    home = str(Path.home())
    llvm_home = e.get("LLVM_HOME", "/home/zzj/opt/llvm17")
    zig_home = e.get("ZIG_HOME", "/home/zzj/opt/zig")
    venv_dir = e.get("VENV_DIR", "/home/zzj/opt/venvs/tetris")
    afl_prefix = e.get("AFL_PREFIX", "/home/zzj/opt/aflplusplus")
    e["PATH"] = (
        f"{venv_dir}/bin:{llvm_home}/bin:{afl_prefix}/bin:"
        f"{zig_home}:{home}/.cargo/bin:{e.get('PATH', '')}"
    )
    return e


def run(cmd: list[str], cwd: Path, e: dict[str, str], timeout: int = 30) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        cmd,
        cwd=cwd,
        env=e,
        text=True,
        capture_output=True,
        timeout=timeout,
    )


def run_shell(command: str, cwd: Path, e: dict[str, str], timeout: int = 30) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        cwd=cwd,
        env=e,
        shell=True,
        text=True,
        capture_output=True,
        timeout=timeout,
        executable="/bin/bash",
    )


def append_zig_main(path: Path) -> None:
    text = path.read_text(errors="replace")
    if not text.startswith('const std = @import("std");'):
        text = 'const std = @import("std");\n' + text
    text += "\n" + MAIN_TAIL
    path.write_text(text)


def clean_error(text: str) -> str:
    lines = [line.rstrip() for line in text.splitlines()]
    lines = [line for line in lines if line.strip()]
    return "\n".join(lines)


def replay_case(case_file: Path, script_dir: Path, e: dict[str, str]) -> tuple[str, str]:
    with tempfile.TemporaryDirectory(prefix="zig_tc_replay_") as tmp:
        work = Path(tmp)
        case_env = e.copy()
        zig_global_cache = work / "zig-global-cache"
        zig_local_cache = work / "zig-local-cache"
        zig_global_cache.mkdir()
        zig_local_cache.mkdir()
        case_env["ZIG_GLOBAL_CACHE_DIR"] = str(zig_global_cache)
        case_env["ZIG_LOCAL_CACHE_DIR"] = str(zig_local_cache)
        shutil.copy2(case_file, work / "mutated_code.c")

        instrument = script_dir / "scripts" / "instrument_testcase"
        proc = run(
            [str(instrument), "mutated_code.c", "runner.c", "new_runner.c"],
            work,
            case_env,
            timeout=40,
        )
        if proc.returncode != 0:
            return "instrument", clean_error(proc.stderr or proc.stdout or f"exit {proc.returncode}")

        proc = run_shell(
            "zig translate-c runner.c -I /usr/include -I/usr/include/x86_64-linux-gnu > runner.zig",
            work,
            case_env,
            timeout=180,
        )
        if proc.returncode != 0:
            return "translate-c", clean_error(proc.stderr or proc.stdout or f"exit {proc.returncode}")

        append_zig_main(work / "runner.zig")

        proc = run(
            ["zig", "build-exe", "runner.zig", "-lc", "-femit-bin=runner"],
            work,
            case_env,
            timeout=60,
        )
        if proc.returncode != 0:
            return "zig-build", clean_error(proc.stderr or proc.stdout or f"exit {proc.returncode}")

        proc = run(["./runner"], work, case_env, timeout=30)
        if proc.returncode != 0:
            return "zig-run", clean_error(proc.stderr or proc.stdout or f"exit {proc.returncode}")

        return "not-reproduced", clean_error(
            f"runner exited 0\nstdout: {proc.stdout.strip()}\nstderr: {proc.stderr.strip()}"
        )


def default_paths(csv_path: Path) -> tuple[Path, Path]:
    output_dir = csv_path.parent
    category_dir = output_dir / "build_failure"
    return output_dir, category_dir


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "csv",
        nargs="?",
        default="fuzzing/output_20/build_failure_errors.csv",
        help="Existing build_failure_errors.csv",
    )
    parser.add_argument("-o", "--output", help="Output CSV path")
    parser.add_argument("--limit", type=int, default=0)
    parser.add_argument("--jobs", type=int, default=1, help="Number of cases to replay in parallel")
    args = parser.parse_args()

    script_dir = Path(__file__).resolve().parent
    csv_path = Path(args.csv)
    if not csv_path.is_absolute():
        csv_path = script_dir / csv_path
    output_dir, category_dir = default_paths(csv_path)
    out_path = Path(args.output) if args.output else output_dir / "build_failure_errors_replayed.csv"
    if not out_path.is_absolute():
        out_path = script_dir / out_path

    e = env()
    rows = list(csv.DictReader(csv_path.open(newline="")))
    if args.limit:
        rows = rows[: args.limit]

    def handle(row: dict[str, str]) -> dict[str, str]:
        case_name = row["case"]
        case_file = category_dir / case_name
        if not case_file.exists():
            stage, error = "missing-case", f"missing file: {case_file}"
        else:
            try:
                stage, error = replay_case(case_file, script_dir, e)
            except subprocess.TimeoutExpired as exc:
                stage, error = "timeout", f"timeout while running: {exc.cmd}"
        return {
            "category": row.get("category", "build_failure"),
            "case": case_name,
            "log": row.get("log", ""),
            "stage": stage,
            "error": error,
        }

    with out_path.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=["category", "case", "log", "stage", "error"])
        writer.writeheader()
        if args.jobs <= 1:
            for idx, row in enumerate(rows, 1):
                out_row = handle(row)
                writer.writerow(out_row)
                f.flush()
                print(f"[{idx}/{len(rows)}] {out_row['case']}: {out_row['stage']}")
        else:
            with ThreadPoolExecutor(max_workers=args.jobs) as executor:
                futures = [executor.submit(handle, row) for row in rows]
                for idx, future in enumerate(as_completed(futures), 1):
                    out_row = future.result()
                    writer.writerow(out_row)
                    f.flush()
                    print(f"[{idx}/{len(rows)}] {out_row['case']}: {out_row['stage']}")

    print(f"Wrote {out_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
