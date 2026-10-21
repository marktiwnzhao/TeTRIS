from pathlib import Path
import subprocess,time,json,hashlib,concurrent.futures,shutil
ROOT=Path('/home/zzj/TeTRIS'); OUT=ROOT/'audit/repeat_runs_review/hang_replays';OUT.mkdir(exist_ok=True)
items=[]
for i,name in [(1,'output_paper_tmux_20260924_021229'),(2,'output_paper_tmux2_20260924_023348')]:
 for p in (ROOT/'transpilers/zig-tc/fuzzing'/name/'hangs').iterdir():
  if p.is_file() and p.name.startswith('id:'):items.append((i,p))
def check(x):
 i,p=x;d=OUT/f'run{i}_{hashlib.sha256(p.name.encode()).hexdigest()[:12]}';d.mkdir(exist_ok=True);shutil.copyfile(p,d/'runner.c');t=time.monotonic()
 cmd=['/home/zzj/opt/zig/zig','translate-c','-j1','runner.c','-I','/usr/include','-I','/usr/include/x86_64-linux-gnu','--cache-dir',str(d/'cache'),'--global-cache-dir',str(OUT/'global-cache')]
 try:
  r=subprocess.run(cmd,cwd=d,capture_output=True,timeout=20);code=r.returncode;(d/'runner.zig').write_bytes(r.stdout);err=r.stderr.decode(errors='replace')
 except subprocess.TimeoutExpired:code=124;err='timeout after 20s'
 result=dict(run=i,file=str(p.relative_to(ROOT)),code=code,seconds=time.monotonic()-t,stderr=err);(d/'result.json').write_text(json.dumps(result,indent=2));return result
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:rs=list(pool.map(check,items))
(OUT/'results.json').write_text(json.dumps(rs,indent=2));print([(r['run'],r['code'],round(r['seconds'],2)) for r in rs])
