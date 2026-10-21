import subprocess,os,pathlib,json
p=pathlib.Path(__file__).resolve().parent
root=p.parent.parent
(p/'simple.c').write_text('int probe(int x) { return x+1; }\n')
(p/'complex.c').write_text('struct S { int x; double y; };\nint probe(int x) { struct S a={x,2.5}; switch(x) {case 1: return a.x; case 2: return (int)a.y; default: while(x>0) x--; return x;} }\n')
(p/'invalid.c').write_text('int probe( { NOT_VALID }\n')
e=os.environ.copy();e['AFL_PATH']='/home/zzj/opt/aflplusplus/bin';e['AFL_NO_FORKSRV']='1';e['AFL_SKIP_CPUFREQ']='1';e['AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES']='1'
subprocess.run(['gcc',str(root/'transpilers/zig-tc/fuzz_env_template/c2zig_fuzz.c'),'-o',str(p/'wrapper')],check=True)
rows=[]
for mode in ['wrapper','direct']:
 for src in ['simple.c','complex.c','invalid.c','simple.c']:
  n=len(rows); out=p/f'{n}_{mode}.map'
  target=[str(p/'wrapper'),src] if mode=='wrapper' else ['/home/zzj/opt/zig/zig','translate-c',src]
  cmd=['/home/zzj/opt/aflplusplus/bin/afl-showmap','-Q','-m','none','-t','20000','-o',str(out),'--']+target
  r=subprocess.run(cmd,cwd=p,env=e,text=True,capture_output=True,timeout=45)
  (p/f'{n}.stdout').write_text(r.stdout);(p/f'{n}.stderr').write_text(r.stderr)
  lines=out.read_text().splitlines() if out.exists() else []
  rows.append(dict(mode=mode,source=src,returncode=r.returncode,edges=len(lines),map=str(out),cmd=cmd))
  print(rows[-1],flush=True)
(p/'results.json').write_text(json.dumps(rows,indent=2))
