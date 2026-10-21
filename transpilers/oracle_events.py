"""Append-only oracle events. Candidate identity is independent of AFL queue IDs."""
import hashlib
import json
import os
from pathlib import Path
import time
import uuid
import subprocess
import signal
from datetime import datetime, timezone


def record_event(status_file, tool, input_label, source, status):
    now = time.time()
    mono = time.monotonic()
    root = Path(status_file).resolve().parent
    data = Path(source).read_bytes() if Path(source).is_file() else None
    digest = hashlib.sha256(data).hexdigest() if data is not None else None
    kind = status.split(' ', 1)[0]
    if status.startswith('TIDY_WARNINGS'):
        kind = 'TIDY_WARNINGS'
    candidate = kind in {'OUTPUT_DIFF', 'TRANSPILE_BUILD_FAILURE', 'NZEC', 'TIMED_OUT', 'MISSING'}
    artifact = None
    if candidate and data is not None:
        folder = root / 'oracle_candidates'
        folder.mkdir(exist_ok=True)
        artifact = folder / (digest + '.c')
        # Repeated events reference the same immutable source snapshot.
        try:
            with artifact.open('xb') as f:
                f.write(data)
        except FileExistsError:
            pass
    start = os.environ.get('TETRIS_START_MONOTONIC')
    event = {
        'schema_version': 1, 'event_id': uuid.uuid4().hex,
        'tool': tool, 'timestamp_utc': datetime.fromtimestamp(now, timezone.utc).isoformat(),
        'unix_time': now, 'elapsed_seconds': mono - float(start) if start else None,
        'clock_basis': 'fuzzer_launch' if start else 'unknown',
        'input_label': input_label, 'source_sha256': digest,
        'status': kind, 'detail': status, 'candidate': candidate,
        'source_artifact': str(artifact.relative_to(root)) if artifact else None,
        'oracle_return': {'VERIFIED': 1, 'OUTPUT_DIFF': 2, 'TRANSPILE_BUILD_FAILURE': 3}.get(kind, 0),
    }
    with (root / 'oracle_events.jsonl').open('a') as f:
        f.write(json.dumps(event, ensure_ascii=False) + '\n')
        f.flush()
        os.fsync(f.fileno())


def start_campaign(output_dir, tool, command):
    now, mono = time.time(), time.monotonic()
    os.environ['TETRIS_START_MONOTONIC'] = str(mono)
    metadata = {'tool': tool, 'start_unix_time': now, 'start_monotonic': mono,
                'start_utc': datetime.fromtimestamp(now, timezone.utc).isoformat(),
                'command': command, 'clock_basis': 'fuzzer_launch'}
    metadata['budget_seconds'] = float(os.environ.get('TETRIS_DURATION_SECONDS', '86400'))
    seed_dir = Path(os.environ['TETRIS_INIT_FILE_DIR']) if os.environ.get('TETRIS_INIT_FILE_DIR') else None
    metadata['seeds_sha256'] = ({p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                                for p in sorted(seed_dir.iterdir()) if p.is_file()}
                               if seed_dir else {})
    repo = Path(__file__).resolve().parents[1]
    metadata['git_revision'] = subprocess.check_output(['git', '-C', str(repo), 'rev-parse', 'HEAD'], text=True).strip()
    (Path(output_dir) / 'tracked_changes.patch').write_bytes(subprocess.check_output(['git', '-C', str(repo), 'diff', 'HEAD']))
    binary = repo / 'core/afl-fuzz-tetris'
    metadata['fuzzer_sha256'] = hashlib.sha256(binary.read_bytes()).hexdigest()
    (Path(output_dir) / 'campaign_timing.json').write_text(json.dumps(metadata, indent=2))


def _descendants(pid):
    parents = {}
    for item in Path('/proc').glob('[0-9]*/stat'):
        try:
            fields = item.read_text().rsplit(')', 1)[1].split()
            parents[int(item.parent.name)] = int(fields[1])
        except (OSError, ValueError, IndexError):
            continue
    found = {pid}
    while True:
        expanded = found | {child for child, parent in parents.items() if parent in found}
        if expanded == found:
            return found - {pid}
        found = expanded


def _signal_pids(pids, sig):
    for pid in pids:
        try:
            os.kill(pid, sig)
        except ProcessLookupError:
            pass


def run_campaign(output_dir, tool, command):
    """Budget includes calibration; stop descendants even if AFL calls setsid."""
    duration = float(os.environ.get('TETRIS_DURATION_SECONDS', '86400'))
    if duration <= 0:
        raise ValueError('TETRIS_DURATION_SECONDS must be positive')
    start_campaign(output_dir, tool, command)
    # Adopt orphaned targets, so cleanup can still find children after AFL exits.
    import ctypes
    if ctypes.CDLL(None, use_errno=True).prctl(36, 1, 0, 0, 0) != 0:
        raise OSError(ctypes.get_errno(), 'cannot enable child subreaper')
    proc = subprocess.Popen(command, shell=True, start_new_session=True)
    timed_out = False
    code = None
    try:
        code = proc.wait(timeout=duration)
    except subprocess.TimeoutExpired:
        timed_out = True
    finally:
        descendants = _descendants(os.getpid())
        _signal_pids(descendants, signal.SIGINT)
        try:
            code = proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            _signal_pids(_descendants(os.getpid()), signal.SIGKILL)
            code = proc.wait()
        # Targets can outlive the shell and the fuzzer's process group.
        _signal_pids(_descendants(os.getpid()), signal.SIGKILL)
        (Path(output_dir) / 'campaign_exit.json').write_text(json.dumps({
            'budget_seconds': duration, 'timed_out': timed_out, 'returncode': code,
            'end_unix_time': time.time()}, indent=2))
    if code != 0 and not timed_out:
        raise RuntimeError(f'fuzzer exited with status {code}')
