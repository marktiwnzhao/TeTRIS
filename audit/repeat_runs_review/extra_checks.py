import sys,json,concurrent.futures,re
from pathlib import Path
sys.path.insert(0,'audit/repeat_runs_review');from replay import *
rs=json.loads((OUT/'replay_results.json').read_text())
def work(r):
 d=ROOT/r['dir']
 if r['tool']=='c2rust':
  results={}
  for comp in ['/home/zzj/opt/llvm17/bin/clang','gcc']:
   for opt in ['-O0','-O2']:
    b=run([comp,opt,'source.c','-o','control'],d)
    results[comp+' '+opt]=run(['./control'],d,4) if b['code']==0 else b
  (d/'compiler_controls.json').write_text(json.dumps(results,indent=2))
 if r['tool']=='c2go':
  if '07-07-37' in r['case']:
   s=(d/'source.c').read_text().replace('set_var(local_int,local_float,local_char);\n','');(d/'source.c').write_text(s)
   b=run(['/home/zzj/opt/llvm17/bin/clang','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','source.c','-o','source'],d)
   if b['code']==0:r['strict']=run(['./source'],d,4)
  s=(d/'source.c').read_text();(d/'unsigned.c').write_text(s)
  b=run(['/home/zzj/opt/llvm17/bin/clang','-funsigned-char','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','unsigned.c','-o','unsigned'],d)
  if b['code']==0:r['unsigned_char']=run(['./unsigned'],d,4)
 return r
with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:rs=list(pool.map(work,rs))
(OUT/'replay_results.json').write_text(json.dumps(rs,indent=2))
