# TeTRIS output_20: selected six root-cause families

All times come from this project's original c2go output_20. No LLM4Transpiler discovery times are used. c2rust and translate-c remain zero under the agreed counting policy.

| Root-cause family | Earliest matched saved candidate (seconds) | Cumulative count |
|---|---:|---:|
| #911: discarded expression emitted as an illegal Go statement | 55 | 1 |
| address-of-rvalue (#909/#715 family, counted once) | 109 | 2 |
| #918: increment/decrement expression emitted as assignment | 442 | 3 |
| #928: LP64 long width, including unsigned long | 9328 | 4 |
| switch body without a case label executed outside switch | 29647 | 5 |
| #912: unused local variable | 66323 | 6 |

The pointer compound-assignment return-&p cluster is excluded at the user's request. #921 has no matching evidence in the saved output_20 and is not included. These six families are not six newly reported issue numbers: #909/#715 are grouped, and the switch bug uses a descriptive ID.

Times are retrospectively reconstructed from candidate filenames (time:HH-MM-SS), at second resolution, before checker completion. They are the earliest currently matched saved candidates, not proof that no earlier input triggered the same bug before filtering or diagnostic masking. #911 uses the discarded-expression family, not an exact comma-operator template; the 24-second unary-plus candidate is not automatically assigned to it.

- curve.csv: experiment,time_seconds,bug_count; all three tools, 0 through 86400 seconds.
- bugs.csv: one row per counted root cause and its archived source path.
- evidence.csv: original diagnostics and attribution notes.
- bug_growth.png/pdf/svg: cumulative step plots; the two zero curves overlap.
- rebuild.py: reconstruct CSVs and plots from the archived output_20.

Original experiment files are unmodified. Paired archived C/Go filenames are not assumed to guarantee matching bodies; the 00:48:04 mismatch was not used as switch evidence.

## 原始轮 char 回溯修正（2026-10-22）

已确认原始轮 char 符号性根因，最早保存候选为 **00:00:32**。原始轮旧图的 6 类遗漏了该类：继续排除指针返回类时应为 **7 类**；如与新两轮统一纳入指针返回类，则为 **8 类**。现有图表仍是旧六类快照，尚未重画。详见 [回溯证据](original_char_signedness_review.md)。
