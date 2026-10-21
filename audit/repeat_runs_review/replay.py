from pathlib import Path
import json,os,re,subprocess,concurrent.futures,hashlib
ROOT=Path('/home/zzj/TeTRIS'); OUT=ROOT/'audit/repeat_runs_review'
ENV={**os.environ,'PATH':'/home/zzj/opt/clang-6.0/bin:/home/zzj/opt/go/bin:'+os.environ['PATH'],'GOPATH':'/home/zzj/opt/go-work','GO111MODULE':'off'}
def run(cmd,d,timeout=25):
 try:
  p=subprocess.run(cmd,cwd=d,env=ENV,capture_output=True,text=True,timeout=timeout);return dict(code=p.returncode,out=p.stdout,err=p.stderr)
 except subprocess.TimeoutExpired:return dict(code=124,out='',err='TIMEOUT')
def instrument(s):
 f=s.find('int func1');s=s[:f]+'void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}\n'+s[f:]
 i=s.find('return',f);s=s[:i]+'set_var(local_int,local_float,local_char);\n'+s[i:]
 return '#include <stdio.h>\n'+s+'\nint main(void){func1();printf("%d:%.2f:%d\\n",global_int,global_float,(int)global_char);return 0;}\n'
def check(item):
 i,t,r=item;d=OUT/'replays'/f'run{i}_{t}'/hashlib.sha256(r['case'].encode()).hexdigest()[:16];d.mkdir(parents=True,exist_ok=True)
 s=(ROOT/r['file']).read_text();(d/'source.c').write_text(instrument(s));result=dict(run=i,tool=t,case=r['case'],dir=str(d.relative_to(ROOT)))
 for label,extra in [('strict',[]),('unsigned_char',['-funsigned-char'])] if t=='zig-tc' else [('strict',[])]:
  res=run(['/home/zzj/opt/llvm17/bin/clang','-O0','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all',*extra,'source.c','-o','source'],d)
  result[label+'_build']=res
  if res['code']==0:result[label]=run(['./source'],d,4)
 # run archived Go, and diagnostic width-repair transpilation for c2go diffs
 if t=='c2go':
  go=ROOT / next(x.split('=',1)[1] for x in r['related_files'].split('; ') if x.startswith('output_diff_go='))
  (d/'archived.go').write_bytes(go.read_bytes());result['go_build']=run(['/home/zzj/opt/go/bin/go','build','-o','go_original','archived.go'],d)
  if result['go_build']['code']==0:result['go_original']=run(['./go_original'],d,4)
  # long -> long long maintains LP64 source width while bypassing c2go long mapping
  fixed=re.sub(r'\blong(?:\s+long)?(?:\s+int)?\b',lambda m:m[0] if m[0].count('long')==2 else m[0].replace('long','long long'),s)
  ss=instrument(fixed);ss=re.sub(r'#include <stdio.h>\n','',ss);ss=re.sub(r'printf\("%d:%.2f:%d\\n",global_int,global_float,\(int\)global_char\);','',ss)
  (d/'runner.c').write_text(ss);result['wide_translate']=run(['/home/zzj/opt/go-work/bin/c2go','transpile','runner.c'],d)
  if (d/'runner.go').exists():
   g=(d/'runner.go').read_text();g=g.replace('package main','package main\nimport "fmt"',1);g=g.replace('func1()\n','func1()\n fmt.Printf("%d:%.2f:%d\\n",global_int,global_float,global_char)\n')
   (d/'runner.go').write_text(g);result['wide_build']=run(['/home/zzj/opt/go/bin/go','build','-o','go_wide','runner.go'],d)
   if result['wide_build']['code']==0:result['go_wide']=run(['./go_wide'],d,4)
 (d/'result.json').write_text(json.dumps(result,indent=2));return result
if __name__=='__main__':
 items=[]
 for i in [1,2]:
  for t in ['c2rust','c2go','zig-tc']:
   for r in json.loads((OUT/f'run{i}_{t}_archive.json').read_text()):
    if r['category']=='output_diff_c' or t=='zig-tc':items.append((i,t,r))
 with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
  rs=[]
  for r in pool.map(check,items):
   rs.append(r);print(r['run'],r['tool'],r['case'][:16],r.get('strict',{}).get('code'),r.get('strict',{}).get('out','').strip(),r.get('go_wide',{}).get('out','').strip(),flush=True)
 (OUT/'replay_results.json').write_text(json.dumps(rs,indent=2))
