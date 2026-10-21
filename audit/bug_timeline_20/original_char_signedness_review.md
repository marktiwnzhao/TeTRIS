# 原始轮 C2Go char 符号性回溯（2026-10-22）

**结论：`output_20` 已发现此根因。现有保存差分候选中最早为 00:00:32（32 秒，AFL 文件名候选时间）。它原先未进入选定六类的图表，属于审计遗漏，不能再描述为“该轮未确认”。**

## 最早候选与对照

候选：`id:000007,src:000000,time:00-00-32,op:(null),pos:0,+cov1780935999_OUTPUT_DIFF_C`。

原 C 的关键路径先执行 `global_char = ~global_char`，随后用该值计算 `local_int`。`global_char` 初始为 0，本平台普通 char 为有符号类型，C 中得到 -1；生成 Go 将它映射为 `byte`，得到 255。

此时相关整数表达式可化简为 `3 * (global_char + 1) + 56`，分别得到 56 和 824。oracle 前的 `set_var` 又将 `global_char` 设置为 `local_char`（1），所以最终 char 字段相同，差异只出现在整数结果；只搜索最终负 char 输出会漏掉这个更早样本。

| 对照 | 完整输出 | 结果 |
|---|---|---|
| 普通 char，Clang 17 + UBSan/float-cast-overflow | `56:335.22:1` | 无 sanitizer 报错、无 -Wunsequenced 警告 |
| 显式 signed char，同样检查 | `56:335.22:1` | 保持原 C 结果 |
| 普通 char，增加 -funsigned-char，同样检查 | `824:335.22:1` | 与保存的 Go 输出一致 |
| 原始日志保存的 Go 输出 | `824:335.22:1` | 错误无符号表示 |
| 已有 long→long-long 诊断重放的 Go | `824:335.22:1` | 加宽 long 没有修复差异，因此不能归为 #928 |

本次 C 对照在匿名内存文件中编译执行，没有修改原始实验文件。对照复现了原有 checker 在 func1 第一个 return 前插入 set_var、main 调用 func1 并打印全局变量的行为。

## 直接 char 输出的更晚样本

00:03:59 的 `id:000054` 也确认了同根因：普通 char 与显式 signed char 的 C 输出 `57:2.00:-1`，-funsigned-char 的 C 与保存的 Go 输出 `57:2.00:255`。该样本不是最早发现时间。

## 全部保存差分样本的扫描

对 output_diff_c 的 51 个文件分别重建 checker 观测程序，用 Clang 17 的普通 char 和 -funsigned-char 对照；使用 `-O0 -Wunsequenced -fsanitize=undefined,float-cast-overflow -fno-sanitize-recover=all`。比较时只归一化 NaN/Inf 文本拼写。

| 对照筛选结果 | 样本数 |
|---|---:|
| 无 -Wunsequenced 警告且 sanitizer 干净，unsigned-char 控制的归一化输出匹配 Go、普通 char 不匹配 | 17 |
| 归一化后普通 C 与 Go 无差异（格式类） | 6 |
| 普通 C 出现 -Wunsequenced 警告 | 15 |
| unsigned-char 控制未单独解释差异 | 13 |
| 合计 | 51 |

这是对照筛选，不是 17 个独立 bug，也不表示已完整证明所有其余样本的 C 语义。根因确认与最早时间以第一个样本的具体语义和原始日志为依据。00:00:32 本身就是最早保存的差分候选，因此在这些保存差分文件中没有更早时间；不声称覆盖了未保存或被过滤的所有生成程序。

普通 char 的符号性是实现定义的。此结论针对本次源 C 的有符号 char 环境，不能表述为所有平台上 char 必须有符号，也不能称为显式 signed char 本身的支持错误。与 #590 的具体同根因对应仍未严格确认，本次不据此宣称新上游 issue。

## 对当前图的影响

- 原图保留的六类之外，还应补 char-signedness：继续按原要求排除 pointer-return，则原始轮为 **7 类**。
- 如采用新两轮已经纳入 pointer-return 的统一口径，原始轮已知合计为 **8 类**，新两轮各 6 类。
- 现有曲线/bugs.csv/统计图仍是旧的六类快照，**尚未重画，不能再视为完整发现数量**。本次只更新回溯记录及说明，不替用户决定最终统一排除口径。

## 证据路径

原始源文件和生成 Go 位于 `/home/zzj/TeTRIS/transpilers/c2go/fuzzing/output_20/output_diff_c/` 与 `output_diff_go/` 的对应 `id:000007,...time:00-00-32,...` 文件。

日志：`/home/zzj/TeTRIS/transpilers/c2go/fuzzing/output_20/LOGS/py_checker.607-4090.zzj.log.INFO.20260609-002638.1901793`（以随附 JSON 的实际 log 字段为准）。原始日志依次记录 C 的 `56:335.22:1`、Go 的 `824:335.22:1` 及两份文件的保存命令。

已有 width-control 重放位于 `audit/bug_timeline_20/long_probe/`（case 信息见 results.json）；C 对照输出由本次匿名内存编译执行取得。
