#!/usr/bin/env python3
"""Extract checker error messages for files saved in a fuzz result category.

Example:
  python3 transpilers/extract_fuzz_errors.py \
    transpilers/c2go/fuzzing/output_20/build_failure
"""

from __future__ import annotations

import argparse
import csv
import re
import sys
from pathlib import Path


GLOG_PREFIX = re.compile(
    r"^[IWEF]\d{4}\s+\d\d:\d\d:\d\d\.\d+\s+\d+\s+[^:]+:\d+\]\s?"
)
IMPORTANT_MARKERS = (
    "STDERR:",
    "STDOUT:",
    "ERROR",
    "EXECUTION FAILURE",
    "Exception",
    "Traceback",
    "BUILD",
    "DIFF",
    "CRASH",
    "TIMEOUT",
    "Expected from",
    "Actual from",
    "LLVM",
    "clang",
    "cargo",
    "go build",
    "zig",
)


def strip_glog(line: str) -> str:
    return GLOG_PREFIX.sub("", line.rstrip("\n"))


def id_key(path: Path) -> tuple[int, str]:
    match = re.search(r"id:(\d+)", path.name)
    if match:
        return (int(match.group(1)), path.name)
    return (10**12, path.name)


def saved_to_queue_name(name: str) -> str:
    """Convert a saved category file name back to the queue file name if possible."""
    patterns = (
        r"\d+_BUILD_FAILURE$",
        r"\d+_OUTPUT_DIFF_[A-Za-z]+$",
        r"\d+_OUTPUT_DIFF_[A-Za-z_]+$",
        r"\d+_CRASH$",
        r"\d+_HANG$",
        r"\d+_FAULTY$",
    )
    for pattern in patterns:
        stripped = re.sub(pattern, "", name)
        if stripped != name:
            return stripped
    return name


def load_logs(logs_dir: Path) -> list[tuple[Path, str]]:
    logs = []
    for path in sorted(logs_dir.iterdir()):
        if path.is_file():
            logs.append((path, path.read_text(errors="replace")))
    return logs


def find_log(
    basename: str, queue_name: str, logs: list[tuple[Path, str]]
) -> tuple[Path | None, str | None, str]:
    for path, text in logs:
        if basename in text:
            return path, text, "saved-name"
    for path, text in logs:
        if queue_name in text:
            return path, text, "queue-name"
    return None, None, "not-found"


def extract_error_lines(text: str, basename: str, category: str) -> list[str]:
    raw_lines = text.splitlines()
    out: list[str] = []

    for line in raw_lines:
        clean = strip_glog(line)
        if "VERIFYING:" in clean:
            out.append(clean)
            break

    collecting = False
    for line in raw_lines:
        clean = strip_glog(line)
        is_new_log_line = bool(GLOG_PREFIX.match(line))

        if any(marker in clean for marker in IMPORTANT_MARKERS):
            if clean not in out:
                out.append(clean)
            collecting = "STDERR:" in clean or "Traceback" in clean
            continue

        if collecting:
            if is_new_log_line:
                collecting = False
            elif clean.strip():
                out.append(clean)

        if basename in clean or f"../{category}/" in clean:
            if clean not in out:
                out.append(clean)

    if not out:
        out = [strip_glog(line) for line in raw_lines[-20:]]

    return out


def first_value(lines: list[str], marker: str) -> str:
    for line in lines:
        clean = strip_glog(line)
        if marker in clean:
            return clean.split(marker, 1)[1].strip()
    return ""


def all_values(lines: list[str], marker: str) -> list[str]:
    values = []
    for line in lines:
        clean = strip_glog(line)
        if marker in clean:
            values.append(clean.split(marker, 1)[1].strip())
    return values


def collect_multiline_after(lines: list[str], marker: str) -> str:
    collected: list[str] = []
    active = False
    for line in lines:
        clean = strip_glog(line)
        if marker in clean:
            collected.append(clean.split(marker, 1)[1].strip())
            active = True
            continue
        if active:
            if GLOG_PREFIX.match(line):
                break
            collected.append(clean)
    return "\n".join(line for line in collected if line.strip())


def verifying_path(lines: list[str]) -> str:
    value = first_value(lines, "VERIFYING:")
    return value


def related_category_files(category_dir: Path, queue_name: str) -> dict[str, str]:
    related = {}
    parent = category_dir.parent
    for sibling in sorted(parent.iterdir()):
        if not sibling.is_dir() or sibling.name in {"LOGS", "queuegg", "fuzz_env"}:
            continue
        matches = [p for p in sibling.iterdir() if p.is_file() and p.name.startswith(queue_name)]
        if matches:
            related[sibling.name] = ";".join(str(p) for p in sorted(matches))
    return related


def csv_row(path: Path, logs: list[tuple[Path, str]], category: str) -> dict[str, str]:
    basename = path.name
    queue_name = saved_to_queue_name(basename)
    log_path, log_text, matched_by = find_log(basename, queue_name, logs)

    row = {
        "category": category,
        "case": basename,
        "file": str(path),
        "queue_name": queue_name,
        "log": str(log_path) if log_path else "",
        "matched_by": matched_by,
        "verify_path": "",
        "build_stdout": "",
        "build_stderr": "",
        "clang_tidy": "",
        "expected_from_c": "",
        "actual_from_transpiled": "",
        "execution_results": "",
        "related_files": "",
        "error_summary": "",
    }

    if log_text is None:
        row["error_summary"] = "No matching checker log was found under LOGS/."
        return row

    lines = log_text.splitlines()
    row["verify_path"] = verifying_path(lines)
    row["clang_tidy"] = collect_multiline_after(lines, "CLANG-TIDY STDOUT:")
    row["build_stdout"] = (
        collect_multiline_after(lines, "GO BUILD STDOUT:")
        or collect_multiline_after(lines, "ZIG BUILD STDOUT:")
        or collect_multiline_after(lines, "CLANG STDOUT:")
        or collect_multiline_after(lines, "CARGO STDOUT:")
    )
    row["build_stderr"] = (
        collect_multiline_after(lines, "GO BUILD STDERR:")
        or collect_multiline_after(lines, "ZIG BUILD STDERR:")
        or collect_multiline_after(lines, "CLANG STDERR:")
        or collect_multiline_after(lines, "CARGO STDERR:")
    )
    execution_failure_stdout = collect_multiline_after(lines, "EXECUTION FAILURE STDOUT:")
    execution_failure_stderr = collect_multiline_after(lines, "EXECUTION FAILURE STDERR:")
    if execution_failure_stderr or execution_failure_stdout:
        row["build_stderr"] = execution_failure_stderr or execution_failure_stdout
    elif (
        category == "build_failure"
        and any("Executing: ./runner" in strip_glog(line) for line in lines)
        and not row["build_stderr"]
        and not row["build_stdout"]
    ):
        row["build_stderr"] = (
            "./runner exited non-zero, but this checker log did not record "
            "the process stdout/stderr. Replay this case with the updated "
            "zig-tc checker to recover the Zig runtime error."
        )
    row["expected_from_c"] = first_value(lines, "Expected from C:")
    row["actual_from_transpiled"] = (
        first_value(lines, "Actual from Go:")
        or first_value(lines, "Actual from Rust:")
        or first_value(lines, "Actual from Zig:")
        or first_value(lines, "Actual from")
    )
    row["execution_results"] = "; ".join(all_values(lines, "Execution finished:"))

    if category.startswith("output_diff"):
        related = related_category_files(path.parent, queue_name)
        row["related_files"] = "; ".join(
            f"{name}={paths}" for name, paths in sorted(related.items())
        )

    row["error_summary"] = "\n".join(extract_error_lines(log_text, basename, category))
    return row


def csv_columns(category: str) -> list[str]:
    return ["category", "case", "log", "error"]


def full_csv_columns(category: str) -> list[str]:
    base = ["category", "case", "file", "queue_name", "log", "matched_by", "verify_path"]
    if category == "build_failure":
        return base + ["build_stdout", "build_stderr", "clang_tidy", "error_summary"]
    if category.startswith("output_diff"):
        return base + [
            "expected_from_c",
            "actual_from_transpiled",
            "execution_results",
            "related_files",
            "error_summary",
        ]
    return base + [
        "build_stdout",
        "build_stderr",
        "expected_from_c",
        "actual_from_transpiled",
        "execution_results",
        "related_files",
        "error_summary",
    ]


def clean_error(error: str) -> str:
    ignored_exact = {
        "# command-line-arguments",
    }
    ignored_prefixes = (
        "VERIFYING:",
        "Checking C source file with clang-tidy",
        "GO BUILD STDOUT:",
        "GO BUILD STDERR:",
        "ZIG BUILD STDOUT:",
        "ZIG BUILD STDERR:",
        "EXECUTION FAILURE:",
        "EXECUTION FAILURE STDOUT:",
        "EXECUTION FAILURE STDERR:",
        "CLANG STDERR:",
        "CARGO STDERR:",
    )
    lines = []
    for line in error.splitlines():
        stripped = line.strip()
        if not stripped:
            continue
        if stripped in ignored_exact:
            continue
        if any(stripped.startswith(prefix) for prefix in ignored_prefixes):
            continue
        if stripped.startswith("['cp', "):
            continue
        lines.append(stripped)
    return "\n".join(lines)


def compact_csv_row(row: dict[str, str], category: str) -> dict[str, str]:
    if category == "build_failure":
        error = row["build_stderr"] or row["build_stdout"] or row["error_summary"]
    elif category.startswith("output_diff"):
        parts = []
        if row["expected_from_c"]:
            parts.append(f"expected: {row['expected_from_c']}")
        if row["actual_from_transpiled"]:
            parts.append(f"actual: {row['actual_from_transpiled']}")
        error = "\n".join(parts) or row["error_summary"]
    else:
        error = row["build_stderr"] or row["error_summary"]

    return {
        "category": row["category"],
        "case": row["case"],
        "log": row["log"],
        "error": clean_error(error),
    }


def summarize_one(path: Path, logs: list[tuple[Path, str]], category: str) -> str:
    basename = path.name
    queue_name = saved_to_queue_name(basename)
    log_path, log_text, matched_by = find_log(basename, queue_name, logs)

    lines = [
        "=" * 100,
        f"case: {basename}",
        f"file: {path}",
        f"queue_name: {queue_name}",
    ]

    if log_path is None or log_text is None:
        lines.extend(
            [
                "log: <not found>",
                "matched_by: not-found",
                "error:",
                "  No matching checker log was found under LOGS/.",
            ]
        )
        return "\n".join(lines)

    lines.extend([f"log: {log_path}", f"matched_by: {matched_by}", "error:"])
    error_lines = extract_error_lines(log_text, basename, category)
    lines.extend(f"  {line}" for line in error_lines)
    return "\n".join(lines)


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Given a fuzzing category directory, find matching checker logs and "
            "extract the corresponding error messages."
        )
    )
    parser.add_argument(
        "category_dir",
        help="Category directory, e.g. transpilers/c2go/fuzzing/output_20/build_failure",
    )
    parser.add_argument(
        "-o",
        "--output",
        help="Write the report to this file instead of stdout.",
    )
    parser.add_argument(
        "--format",
        choices=("text", "csv"),
        default=None,
        help="Output format. Default: infer csv from --output .csv, otherwise text.",
    )
    parser.add_argument(
        "--full-csv",
        action="store_true",
        help="Keep the old detailed CSV columns instead of the compact default.",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=0,
        help="Only summarize the first N files after sorting by id. Default: all.",
    )
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)
    category_dir = Path(args.category_dir).resolve()
    if not category_dir.is_dir():
        print(f"Not a directory: {category_dir}", file=sys.stderr)
        return 2

    output_dir = category_dir.parent
    logs_dir = output_dir / "LOGS"
    if not logs_dir.is_dir():
        print(f"Missing LOGS directory: {logs_dir}", file=sys.stderr)
        return 2

    category = category_dir.name
    files = sorted((p for p in category_dir.iterdir() if p.is_file()), key=id_key)
    if args.limit > 0:
        files = files[: args.limit]

    logs = load_logs(logs_dir)
    output_format = args.format
    if output_format is None:
        output_format = "csv" if args.output and args.output.endswith(".csv") else "text"

    if output_format == "csv":
        rows = [csv_row(path, logs, category) for path in files]
        if args.full_csv:
            columns = full_csv_columns(category)
        else:
            columns = csv_columns(category)
            rows = [compact_csv_row(row, category) for row in rows]
        if args.output:
            with Path(args.output).open("w", newline="") as f:
                writer = csv.DictWriter(f, fieldnames=columns, extrasaction="ignore")
                writer.writeheader()
                writer.writerows(rows)
        else:
            writer = csv.DictWriter(sys.stdout, fieldnames=columns, extrasaction="ignore")
            writer.writeheader()
            writer.writerows(rows)
        return 0

    reports = [
        f"category: {category_dir}",
        f"logs: {logs_dir}",
        f"files: {len(files)}",
        "",
    ]
    reports.extend(summarize_one(path, logs, category) for path in files)
    report = "\n\n".join(reports) + "\n"

    if args.output:
        Path(args.output).write_text(report)
    else:
        print(report, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
