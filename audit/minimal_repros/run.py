#!/usr/bin/env python3
"""Run isolated root-cause probes; retain commands, generated code and outputs."""
import json, subprocess, pathlib, shutil
P=pathlib.Path(__file__).resolve().parent
rows=[]
def run(cmd,cwd):
 p=subprocess.run([str(x) for x in cmd],cwd=cwd,text=True,capture_output=True,timeout=120)
 rows.append(dict(case=cwd.name,cmd=[str(x) for x in cmd],code=p.returncode,stdout=p.stdout,stderr=p.stderr))
 return p
zig_cases={
 'zig_char_promotion':'int probe(void) { char x = -1; return x + 1; }',
 'zig_signed_char_control':'int probe(void) { signed char x = -1; return x + 1; }',
 'zig_char_float':'int probe(void) { float f = -1.0f; char x = (char)f; return x; }',
 'zig_signed_char_float_control':'int probe(void) { float f = -1.0f; signed char x = (signed char)f; return x; }',
}
rust_cases={
 'rust_order':'int g; int f(void) { g = 60; return 0; }\nint main(void) { g += 56 - g + f(); printf("%d\\n",g); return 0; }',
 'rust_sequenced':'int g; int f(void) { g = 60; return 0; }\nint main(void) { int r=f(); g += 56 - g + r; printf("%d\\n",g); return 0; }',
 'rust_order_left':'int g; int f(void) { g = 60; return 0; }\nint main(void) { int old=g; int rhs=56-g; int r=f(); g=old+rhs+r; printf("%d\\n",g); return 0; }',
 'rust_order_right':'int g; int f(void) { g = 60; return 0; }\nint main(void) { int rhs=56-g; int r=f(); g=g+rhs+r; printf("%d\\n",g); return 0; }',
}
for name,src in zig_cases.items():
 d=P/name; d.mkdir(exist_ok=True); (d/'probe.c').write_text(src+'\n')
 (d/'driver.c').write_text('#include <stdio.h>\n'+src+'\nint main(void) { printf("%d\\n",probe()); }\n')
 p=run(['clang','-fsigned-char','-fsanitize=undefined','-fno-sanitize-recover=all','driver.c','-o','c_runner'],d)
 if p.returncode==0: run([d/'c_runner'],d)
 p=run(['zig','translate-c','probe.c'],d)
 if p.returncode: continue
 (d/'probe.zig').write_text(p.stdout+'\nconst std = @import("std");\npub fn main() void { std.debug.print("{}\\n", .{probe()}); }\n')
 p=run(['zig','build-exe','probe.zig','-femit-bin=zig_runner'],d)
 if p.returncode==0: run([d/'zig_runner'],d)
for name,src in rust_cases.items():
 d=P/name; d.mkdir(exist_ok=True); (d/'runner.c').write_text('#include <stdio.h>\n'+src+'\n')
 for compiler in ['clang','gcc']:
  for opt in ['-O0','-O2']:
   p=run([compiler,opt,'-fsanitize=undefined','-fno-sanitize-recover=all','runner.c','-o','c_runner'],d)
   if p.returncode==0: run([d/'c_runner'],d)
 (d/'compile_commands.json').write_text(json.dumps([{'directory':str(d),'file':str(d/'runner.c'),'arguments':[shutil.which('clang'),'-O0',str(d/'runner.c')]}]))
 p=run(['c2rust-transpile','compile_commands.json','-e','-o','rust','--overwrite-existing','--binary','runner'],d)
 if p.returncode: continue
 p=run(['cargo','build','--release','--manifest-path','rust/Cargo.toml'],d)
 if p.returncode==0: run([d/'rust/target/release/runner'],d)
(P/'results.json').write_text(json.dumps(rows,indent=2))
for r in rows:
 if len(r['cmd'])==1: print(r['case'],r['cmd'][0].split('/')[-1],r['code'],repr(r['stdout']),repr(r['stderr'][:180]))
