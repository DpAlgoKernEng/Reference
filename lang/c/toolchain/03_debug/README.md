# C 语言调试器

本目录收录 C 语言调试工具文档。

## 调试器概览

| 调试器 | 平台 | 说明 |
|--------|------|------|
| [GDB](gdb.md) | 跨平台 | GNU 调试器，最广泛使用 |
| [LLDB](lldb.md) | 跨平台 | LLVM 调试器，macOS 默认 |
| [WinDbg](windbg.md) | Windows | Windows 内核/用户态调试 |
| [rr](rr.md) | Linux | Mozilla 时间旅行调试器 |

## 调试器对比

| 特性 | GDB | LLDB | WinDbg | rr |
|------|-----|------|--------|-----|
| 跨平台 | ✅ | ✅ | ❌ | ❌ Linux |
| 时间旅行 | ❌ | 部分 | ❌ | ✅ |
| 内核调试 | ❌ | ❌ | ✅ | ❌ |
| 脚本支持 | Python | Python | JS | Python |
| IDE 集成 | 广泛 | Xcode/VSCode | VS | VSCode |

## 基本调试流程

```
1. 编译时添加调试信息 (-g)
2. 启动调试器加载程序
3. 设置断点
4. 单步执行/继续执行
5. 查看变量/内存/调用栈
6. 分析问题原因
```

## 常用调试命令对照

| 操作 | GDB | LLDB |
|------|-----|------|
| 启动程序 | `run` | `run` |
| 设置断点 | `break main` | `b main` |
| 单步执行 | `next` / `step` | `n` / `s` |
| 继续执行 | `continue` | `c` |
| 查看变量 | `print var` | `p var` |
| 查看调用栈 | `backtrace` | `bt` |
| 查看内存 | `x/10x &var` | `mem read &var` |

## 调试技巧

### 1. 条件断点
```bash
(gdb) break foo.c:42 if x > 10
(lldb) b foo.c:42 -c 'x > 10'
```

### 2. 监视点
```bash
(gdb) watch variable
(lldb) watch set variable variable
```

### 3. 反向调试 (rr)
```bash
rr record ./program
rr replay
(gdb) reverse-continue
```

## 相关文档

- [编译器](../01_compiler/) - GCC、Clang 等
- [动态分析](../06_dynamic_analysis/) - Valgrind、Sanitizers 等