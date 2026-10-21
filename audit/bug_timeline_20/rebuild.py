"""Reconstruct TeTRIS output_20 only; example-project data is never input."""
import csv
import json
import re
import hashlib
from pathlib import Path

OUT = Path(__file__).resolve().parent
ROOT = OUT.parents[1]
DATA = ROOT / 'transpilers/c2go/fuzzing/output_20'

def save(name, rows, fields):
    with (OUT / name).open('w', encoding='utf-8-sig', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)

build = list(csv.DictReader((DATA / 'build_failure_errors.csv').open()))
selected = []
for issue, pattern in [(918, r'unexpected [+-]='), (912, r'declared and not used')]:
    matches = [r for r in build if re.search(pattern, r['error'])]
    matches.sort(key=lambda r: re.search(r'time:(\d+-\d+-\d+)', r['case'])[1])
    row = matches[0]
    selected.append((issue, row, 'build_failure', 'historical compiler diagnostic and C source'))
row = next(r for r in build if 'int32(0)' in r['error'] and 'is not used' in r['error'])
selected.append((911, row, 'build_failure', 'Same discarded-expression root cause as #911; this trigger is a standalone expression rather than the issue reproducer comma operand. Confirmed by expression_review.'))
row = next(r for r in build if 'cannot take address of' in r['error'])
selected.append(('address-of-rvalue (#909/#715)', row, 'build_failure', 'Known c2go address-of-rvalue family; current output_20 candidate is a root-cause match.'))
diff = list(csv.DictReader((DATA / 'diff_errors.csv').open()))
row = next(r for r in diff if 'time:02-35-28,' in r['case'])
selected.append((928, row, 'output_diff_c', 'LP64 long-width root-cause family; widening long casts to long long restores recorded C output; see long_probe'))
row = next(r for r in diff if 'time:08-14-07,' in r['case'])
selected.append(('switch-no-case', row, 'output_diff_c', 'Unlabelled switch body statements are emitted before the Go switch; archived Go has c += 1 before an empty switch. User minimal reproduction rechecked: C exit 0, Go exit 7; see switch_review/results.json. Earliest matched saved differential candidate, not proof of earliest trigger across all inputs.'))
evidence = []
for issue, row, folder, note in selected:
    h, m, s = map(int, re.search(r'time:(\d+)-(\d+)-(\d+)', row['case']).groups())
    path = DATA / folder / row['case']
    assert path.exists()
    evidence.append(dict(experiment='c2go', bug_id=issue, time_seconds=h*3600+m*60+s,
                         source='TeTRIS output_20', program_path=str(path.relative_to(ROOT)),
                         sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
                         log=row['log'], diagnostic=row['error'], note=note))
evidence.sort(key=lambda r: r['time_seconds'])
save('evidence.csv', evidence, list(evidence[0]))
fields = ['experiment', 'bug_id', 'time_seconds', 'source', 'program_path']
save('bugs.csv', [{k: r[k] for k in fields} for r in evidence], fields)
curve = []
for tool in ['c2rust', 'c2go', 'translate-c']:
    curve.append(dict(experiment=tool, time_seconds=0, bug_count=0))
    count = 0
    if tool == 'c2go':
        for r in evidence:
            count += 1
            curve.append(dict(experiment=tool, time_seconds=r['time_seconds'], bug_count=count))
    curve.append(dict(experiment=tool, time_seconds=86400, bug_count=count))
save('curve.csv', curve, ['experiment', 'time_seconds', 'bug_count'])
(OUT / 'method.json').write_text(json.dumps({
    'campaign': 'output_20', 'budget_seconds': 86400,
    'time_basis': 'AFL candidate filename time:HH-MM-SS, checked against historical verification_results; second precision; before checker completion',
    'included': [911, 912, 918, 928, 'address-of-rvalue (#909/#715)', 'switch-no-case'],
    'review_status': 'selected six-family timeline; retrospective earliest matched saved candidates, not an exhaustive discovery census',
    'not_attributed': {'921': 'no union in archived build_failure or output_diff_c candidates; no unsafe.Pointer struct-literal diagnostic'},
    'zero_by_user_counting_policy': ['c2rust', 'translate-c'],
    'scope': 'six confirmed root-cause families requested for the curve; pointer-return compound-assignment family intentionally excluded',
}, indent=2))

if __name__ == '__main__':
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    fig, ax = plt.subplots(figsize=(8, 4.5), layout='constrained')
    for tool, color, style in [('c2go', '#2457a6', '-'), ('c2rust', '#d97319', '-'), ('translate-c', '#188b68', '--')]:
        points = [r for r in curve if r['experiment'] == tool]
        ax.step([r['time_seconds']/3600 for r in points], [r['bug_count'] for r in points], where='post', label=tool, color=color, linestyle=style, linewidth=2)
    ax.set(xlim=(0, 24), ylim=(-.12, 6.35), xlabel='Time (hours)', ylabel='Cumulative bugs', title='TeTRIS output_20: six confirmed root-cause families')
    ax.set_yticks(range(7))
    ax.grid(alpha=.2)
    ax.legend()
    for ext in ['png', 'pdf', 'svg']:
        fig.savefig(OUT / ('bug_growth.' + ext), dpi=180)
