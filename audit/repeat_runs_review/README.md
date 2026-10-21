# Completed TeTRIS repeat-run audit

This inventory covers the two completed 24-hour runs (`paper_tmux_20260924_021229` and `paper_tmux2_20260924_023348`). Times are exact `elapsed_seconds` values from `oracle_events.jsonl`; they are oracle completion times, not AFL filename times. The complete per-candidate file is [candidate_inventory.csv](candidate_inventory.csv); [bug_inventory.csv](bug_inventory.csv) collapses repeated candidates by root family.

The status labels are deliberately separate: `confirmed`, `candidate`/`strong_root_match`, `source_semantics_candidate`/`invalid_source_candidate`, `harness`, `environment`, `oracle_format`, and `unresolved`. No final exclusion policy or cumulative curve has been applied.

## c2go

Both runs reproduce four confirmed families: discarded expression statements (#911 family), `++/--` expression lowering (#918), taking the address of a non-addressable expression (#909/#715 family), and pointer-return type lowering (`&p`/`**T`). Run 1 also reproduces unused declarations (#912). Run 2 has four long-width cases; an LP64 width control (`long` to `long long`) restores the C result, and the earliest case is also repaired by changing only the two corresponding `uint32` casts in generated Go to `uint64`.

Signed-char cases are kept as a separate confirmed root family. Many other output differences contain unsequenced source expressions and are retained as `invalid_source_candidate` until C definedness is established. The `undefined: pp` family is a checker artifact: `process_go_file` inserts `_ = pp` inside a closure where `pp` is not in scope; the minimal proof is under [pp_probe/](pp_probe/). Missing `c2go/noarch` is an environment failure.

## c2rust

The NaN spelling differences are formatting artifacts. The 2026-10-22 review resolves the six run-2 floating-point candidates as source-C unsequenced read/write undefined behavior, and the integer/state difference as an allowed evaluation-order difference. See [detailed evidence](c2rust_run2_semantics_review.md). None is counted as a C2Rust bug.

## translate-c

All 26 oracle candidates in each run collapse to one signed-char/integer-promotion family: negative C `char` values become Zig unsigned-byte values. The runtime panic (`integer part of floating point value out of bounds`) occurs on the same converted values and is recorded as a symptom of that family, not a second bug. Direct isolated `zig translate-c` completed for all 29 saved AFL hangs (13 in run 1 and 16 in run 2), so those hangs currently indicate an AFL/QEMU or verifier/runtime condition and do not establish a translate-c defect.

The corrected translate-c harness was used in both runs. Replay materials, compiler controls, the hang replay, and the `pp` minimization are under this directory.

## Confirmed root families and first observed oracle events

| Tool | Root family | Run 1 | Run 2 |
|---|---|---:|---:|
| c2go | Discarded expression statement | 00:00:23.330 | 00:00:29.738 |
| c2go | Increment/decrement used as expression | 00:01:15.249 | 00:00:22.696 |
| c2go | Address of non-addressable expression | 00:02:21.618 | 00:01:17.589 |
| c2go | Pointer return gains extra indirection | 00:08:44.621 | 00:07:01.028 |
| c2go | Unused local declaration | 03:19:25.360 | not confirmed in this run |
| c2go | Plain char represented as unsigned byte | 00:30:56.800 | 01:27:38.474 |
| c2go | LP64 long lowered to 32 bits | not confirmed in this run | 03:21:18.511 |
| zig-tc | Plain char represented as unsigned byte | 03:03:49.363 | 03:52:59.376 |

C2Go: six confirmed families per run, seven distinct families across both runs. Translate-c: one family per run, with run 2 first observed as a runtime panic (strong root match), followed by a direct output difference at 04:12:44.155. C2Rust: zero confirmed families; the two non-formatting run-2 families have been resolved as source UB and allowed evaluation-order differences (2026-10-22). These are root-family counts, not claims about previously unreported upstream issues.

Candidate counts in the CSV overlap when a build log contains multiple diagnostics. The earliest timestamp is the earliest observed evidence that passed this review, not a claim that no earlier UB-bearing or masked candidate could contain the same defect. Historical issue numbers above are root-family cross-references from the prior audit, not newly verified one-to-one issue equivalence.

## 三轮 C2Go 根因对照

| 根因 / 对应 issue | Original run (`output_20`) | Additional run 1 | Additional run 2 |
|---|---|---|---|
| 丢弃表达式语句，#911 | ✓ | ✓ | ✓ |
| 自增/自减表达式，#918 | ✓ | ✓ | ✓ |
| 对不可取地址表达式取地址，#909/#715 类 | ✓ | ✓ | ✓ |
| 未使用局部变量，#912 | ✓ | ✓ | — |
| LP64 `long` 宽度错误，#928 | ✓ | — | ✓ |
| `switch` 中无 case 的语句处理错误 | ✓ | — | — |
| 指针复合赋值错误返回 `&p` / `**T` | 发现但按当时要求排除 | ✓ | ✓ |
| 普通 `char` 符号性错误 | ✓，最早 00:00:32；旧图未纳入 | ✓ | ✓ |
| **当前图中计数** | **6** | **6** | **6** |

`—` 表示该轮审计未确认，不能理解为绝对不可能触发。C2Go char 类与旧 issue #590 相关，但同根因对应尚未严格确认；指针返回类此前检索未找到同根因 issue，不等于已证明从未报告。

2026-10-22 回溯已确认：原始轮存在 char-signedness，最早保存候选为 00:00:32。它是旧审计遗漏，不是因 #590 主动排除。继续排除 pointer-return 时应为 7 类；按新两轮纳入 pointer-return 的口径则为 8 类。上表“当前图中计数”仍表示尚未更新的旧图快照。

## 原始轮 char 回溯修正（2026-10-22）

已确认原始轮 char 符号性根因，最早保存候选为 **00:00:32**。原始轮旧图的 6 类遗漏了该类：继续排除指针返回类时应为 **7 类**；如与新两轮统一纳入指针返回类，则为 **8 类**。现有图表仍是旧六类快照，尚未重画。详见 [回溯证据](../bug_timeline_20/original_char_signedness_review.md)。
