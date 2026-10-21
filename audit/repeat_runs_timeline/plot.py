from pathlib import Path
import json,csv
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent
inventory=json.loads((ROOT/'audit/repeat_runs_review/bug_inventory.json').read_text())
common={'discarded_expression','incdec_expression','nonaddressable_expression','pointer_return','char_signedness'}
for run,extra in [(1,'unused_declaration'),(2,'long_width')]:
 out=OUT/f'run{run}';out.mkdir(exist_ok=True)
 selected=sorted([r for r in inventory if r['run']==run and r['tool']=='c2go' and r['root_family'] in common|{extra}],key=lambda r:r['elapsed_seconds'])
 assert len(selected)==6 and all(r['status']=='confirmed' for r in selected)
 curve=[]
 for tool in ['c2rust','c2go','translate-c']:
  curve.append(dict(experiment=tool,time_seconds=0,bug_count=0))
  if tool=='c2go':
   for n,r in enumerate(selected,1):curve.append(dict(experiment=tool,time_seconds=r['elapsed_seconds'],bug_count=n))
  curve.append(dict(experiment=tool,time_seconds=86400,bug_count=6 if tool=='c2go' else 0))
 with (out/'curve.csv').open('w',encoding='utf-8-sig',newline='') as f:
  w=csv.DictWriter(f,fieldnames=['experiment','time_seconds','bug_count']);w.writeheader();w.writerows(curve)
 with (out/'bugs.csv').open('w',encoding='utf-8-sig',newline='') as f:
  w=csv.DictWriter(f,fieldnames=list(selected[0]));w.writeheader();w.writerows(selected)
 method=dict(run=run,budget_seconds=86400,time_basis='oracle completion elapsed_seconds from fuzzer launch',included_c2go_families=[r['root_family'] for r in selected],c2rust='No accepted bug; NaN formatting and source-semantics candidates excluded',translate_c='Known char signedness issue #159 excluded by user policy',scope='User-selected confirmed root families, not a count of previously unreported upstream issues',comparison_with_output_20='Different selected families: these runs include pointer_return and char_signedness; historical output_20 chart excluded pointer_return and included switch-no-case. Do not interpret equal final totals as equal discovery sets.')
 (out/'method.json').write_text(json.dumps(method,indent=2))
 fig,ax=plt.subplots(figsize=(8,4.5),layout='constrained')
 for tool,color,style,width in [('c2go','#2457a6','-',2),('c2rust','#d97319','-',3),('translate-c','#188b68','--',1.8)]:
  pts=[r for r in curve if r['experiment']==tool]
  ax.step([r['time_seconds']/3600 for r in pts],[r['bug_count'] for r in pts],where='post',label=tool,color=color,linestyle=style,linewidth=width)
 ax.set(xlim=(0,24),ylim=(-.15,6.4),xlabel='Time (hours)',ylabel='Cumulative bugs',title=f'TeTRIS: 24-hour run {run}')
 ax.set_xticks(range(0,25,3));ax.set_yticks(range(7));ax.grid(alpha=.2);ax.legend(loc='lower right')
 for ext in ['png','pdf','svg']:fig.savefig(out/f'bug_growth.{ext}',dpi=200)
 plt.close(fig)
 print(run,selected[-1]['first_time'],out)
