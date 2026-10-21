"""Rebuild the audit exclusively from archived evidence and exact oracle events."""
from pathlib import Path
import json,re,csv,hashlib,collections,ast
ROOT=Path('/home/zzj/TeTRIS'); OUT=ROOT/'audit/repeat_runs_review'
RUNS={1:'output_paper_tmux_20260924_021229',2:'output_paper_tmux2_20260924_023348'}
replays={(r['run'],r['tool'],r['case']):r for r in json.loads((OUT/'replay_results.json').read_text())}
patterns=[('discarded_expression','confirmed',r'\bis not used'),('incdec_expression','confirmed',r'unexpected [+-]='),('nonaddressable_expression','confirmed',r'cannot take address of'),('pointer_return','confirmed',r'cannot use &'),('unused_declaration','confirmed',r'declared and not used'),('undefined_pp','harness',r'undefined: pp'),('missing_noarch','environment',r'(?:cannot find|no required module).*|package .*noarch')]
def norm(s):return re.sub(r'-?nan','nan',s.lower().strip())
def output(x):
 try:
  v=ast.literal_eval(x);v=v.get('stdout') or v.get('stderr','');return (v.decode() if isinstance(v,bytes) else v).strip()
 except:return ''
def zig_norm(s):
 try:
  a=s.strip().split(':');return a[0],int(float(a[1])),a[2]
 except:return s.strip()
rows=[];stats={}
for run,name in RUNS.items():
 for tool in ['c2go','c2rust','zig-tc']:
  base=ROOT/'transpilers'/tool/'fuzzing'/name
  events=[json.loads(l) for l in (base/'oracle_events.jsonl').read_text().splitlines()]
  stats[f'run{run}_{tool}']=dict(collections.Counter(e['status'] for e in events))
  index=collections.defaultdict(list)
  for e in events:index[(e.get('source_sha256'),e.get('input_label'))].append(e)
  for a in json.loads((OUT/f'run{run}_{tool}_archive.json').read_text()):
   sha=hashlib.sha256((ROOT/a['file']).read_bytes()).hexdigest()
   matches=index[(sha,a['verify_path'])]
   assert len(matches)==1,(a['case'],len(matches))
   e=matches[0];r=replays.get((run,tool,a['case']),{});tags=[]
   if tool=='c2go' and a['category']=='build_failure':
    for fam,status,pat in patterns:
     if re.search(pat,a['build_stderr']):tags.append((fam,status,'saved Go build diagnostic'))
    if not tags:tags=[('unclassified_build','unresolved','inspect saved build diagnostic')]
   elif tool=='c2go':
    warn='unsequenced' in r.get('strict_build',{}).get('err','')
    c=r.get('strict',{}).get('out','');g=r.get('go_original',{}).get('out','')
    if warn:tags.append(('unsequenced_source','invalid_source_candidate','Clang -Wunsequenced; sanitizer success does not rule this out'))
    elif r.get('unsigned_char',{}).get('code')==0 and norm(r['unsigned_char']['out'])==norm(g) and norm(c)!=norm(g):tags.append(('char_signedness','confirmed','unsigned-char C control exactly matches Go; signed-char C differs'))
    elif c and g and c.split(':')[:2]==g.split(':')[:2] and c.split(':')[-1]!=g.split(':')[-1]:tags.append(('char_signedness','confirmed','only final char differs (-90 versus 166); unsigned-char sanitized control traps on intermediate float-to-char conversion'))
    elif r.get('go_wide',{}).get('code')==0 and norm(r['go_wide']['out'])==norm(c) and norm(c)!=norm(g):tags.append(('long_width','confirmed','LP64 long-to-long-long control restores C result; earliest case also restored by changing only two uint32 casts to uint64 in archived Go'))
    else:tags.append(('unresolved_output','unresolved','no validated root assignment'))
   elif tool=='c2rust':
    if norm(output(a['expected_from_c']))==norm(output(a['actual_from_transpiled'])):tags=[('nan_spelling','oracle_format','outputs identical after NaN normalization')]
    elif '12-15-21' in a['case']:tags=[('state_evaluation_order','allowed_evaluation_order','2026-10-22: fifth reply changes global_int from -264 to -334; C loads outer lhs after call, Rust before call; GDB old-value control reproduces Rust exactly. C11 6.5.2.2p10. See c2rust_run2_semantics_review.md.')]
    else:tags=[('aliased_compound_assignment','source_undefined_behavior','2026-10-22: fixed alias ft_ptr=&why; executed line 25 has unsequenced read/write; all six alias-expanded sources produce Clang -Wunsequenced. See c2rust_run2_semantics_review.md.')]
   else:
    if a['category']=='build_failure':tags=[('char_signedness','strong_root_match','runtime float-to-integer panic; signed-char C clean, unsigned-char C traps; merged with char family')]
    else:
     assert zig_norm(r['unsigned_char']['out'])==zig_norm(output(a['actual_from_transpiled'])),a['case']
     tags=[('char_signedness','confirmed','unsigned-char C matches archived Zig output under integerized-float oracle; panic cases merged into this family')]
   for fam,status,reason in tags:
    rows.append(dict(run=run,tool=tool,root_family=fam,status=status,elapsed_seconds=e['elapsed_seconds'],timestamp_utc=e['timestamp_utc'],clock_basis='oracle_completion',case=a['case'],source_sha256=sha,source=a['file'],log=a['log'],replay=r.get('dir',''),reason=reason))
# AFL hangs have a different clock basis and no oracle event.
for r in json.loads((OUT/'hang_replays/results.json').read_text()):
 t=list(map(int,re.search(r'time:(\d+)-(\d+)-(\d+)',r['file']).groups()));sec=t[0]*3600+t[1]*60+t[2]
 rows.append(dict(run=r['run'],tool='zig-tc',root_family='afl_hang',status='unreproduced_timeout',elapsed_seconds=sec,timestamp_utc='',clock_basis='afl_filename',case=Path(r['file']).name,source_sha256=hashlib.sha256((ROOT/r['file']).read_bytes()).hexdigest(),source=r['file'],log='',replay='audit/repeat_runs_review/hang_replays/results.json',reason=f"direct translate-c exit={r['code']}, elapsed={r['seconds']:.3f}s; AFL/QEMU cause not established"))
def write(name,data):
 (OUT/(name+'.json')).write_text(json.dumps(data,indent=2))
 with (OUT/(name+'.csv')).open('w') as f:
  w=csv.DictWriter(f,fieldnames=list(data[0]));w.writeheader();w.writerows(data)
write('candidate_inventory',rows)
summary=[]
for key,group in __import__('itertools').groupby(sorted(rows,key=lambda x:(x['run'],x['tool'],x['root_family'],x['elapsed_seconds'])),lambda x:(x['run'],x['tool'],x['root_family'])):
 group=list(group);first=dict(group[0]);first['candidate_count']=len(group);first['statuses']=';'.join(sorted({x['status'] for x in group}));sec=first['elapsed_seconds'];first['first_time']=f'{int(sec//3600):02d}:{int(sec%3600//60):02d}:{sec%60:06.3f}';summary.append(first)
write('bug_inventory',summary)
(OUT/'oracle_status_counts.json').write_text(json.dumps(stats,indent=2))
for x in summary:print(x['run'],x['tool'],x['root_family'],x['candidate_count'],x['first_time'],x['statuses'])
