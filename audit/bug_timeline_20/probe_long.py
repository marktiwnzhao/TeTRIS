"""Diagnostic only: replace LP64 long casts with equally wide long long casts."""
from pathlib import Path
import os,re,subprocess,json,concurrent.futures,hashlib,sys
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/"transpilers/c2go/scripts"))
from checker import process_go_file
ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent/'long_probe';OUT.mkdir(exist_ok=True)
BASE=ROOT/'transpilers/c2go/fuzzing/output_20'
env={**os.environ,'PATH':'/home/zzj/opt/clang-6.0/bin:/home/zzj/opt/go/bin:'+os.environ['PATH'],'GOPATH':'/home/zzj/opt/go-work','GO111MODULE':'off'}
def run(cmd,cwd):
 try:
  p=subprocess.run(cmd,cwd=cwd,env=env,capture_output=True,text=True,timeout=20);return {'returncode':p.returncode,'stdout':p.stdout,'stderr':p.stderr}
 except subprocess.TimeoutExpired:return {'returncode':124,'stdout':'','stderr':'timeout'}
def probe(p):
 d=OUT/hashlib.sha256(p.name.encode()).hexdigest()[:16];d.mkdir(exist_ok=True)
 s=p.read_text();s=re.sub(r'(?<!long )\blong\b(?!\s+long)', 'long long',s) # normalized first below
 # Apply replacement to whole type spellings, never to half of a long long.
 s=re.sub(r'\blong(?:\s+long)?(?:\s+int)?\b',lambda m:m[0] if m[0].count('long')==2 else m[0].replace('long','long long'),p.read_text())
 f=s.index('int func1');s=s[:f]+'void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}\n'+s[f:]
 i=s.find('return',s.index('int func1'));s=s[:i]+'set_var(local_int,local_float,local_char);\n'+s[i:]
 (d/'runner.c').write_text(s+'\nint main(){func1();return 0;}\n')
 result={'case':p.name};result['transpile']=run(['/home/zzj/opt/go-work/bin/c2go','transpile','runner.c'],d)
 if (d/'runner.go').exists():
  process_go_file(d/'runner.go')
  result['build']=run(['/home/zzj/opt/go/bin/go','build','runner.go'],d)
  if result['build']['returncode']==0:result['run']=run(['./runner'],d)
 (d/'result.json').write_text(json.dumps(result,indent=2));return result
if __name__=='__main__':
 with concurrent.futures.ThreadPoolExecutor(max_workers=2) as ex:
  results=list(ex.map(probe,sorted((BASE/'output_diff_c').iterdir())))
 (OUT/'results.json').write_text(json.dumps(results,indent=2));print('replayed',len(results))
