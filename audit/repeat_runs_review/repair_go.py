import sys,json,concurrent.futures
from pathlib import Path
sys.path.insert(0,'transpilers/c2go/scripts');from checker import process_go_file
sys.path.insert(0,'audit/repeat_runs_review');from replay import run,ROOT,OUT
rs=json.loads((OUT/'replay_results.json').read_text())
def fix(r):
 if r['tool']!='c2go':return r
 d=ROOT/r['dir'];p=d/'runner.go'
 if not p.exists():return r
 g=p.read_text();g=g.replace('import "fmt"\n','');g=g.replace(' fmt.Printf("%d:%.2f:%d\\n",global_int,global_float,global_char)\n','');p.write_text(g)
 process_go_file(p)
 r['wide_build']=run(['/home/zzj/opt/go/bin/go','build','-o','go_wide','runner.go'],d)
 if r['wide_build']['code']==0:r['go_wide']=run(['./go_wide'],d,4)
 (d/'result.json').write_text(json.dumps(r,indent=2));return r
with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:rs=list(pool.map(fix,rs))
(OUT/'replay_results.json').write_text(json.dumps(rs,indent=2))
