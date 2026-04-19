# C++ 动态分析工具

本目录收录 C++ 运行时分析工具文档。

## 动态分析工具概览

| 工具 | 类型 | 说明 |
|------|------|------|
| [Valgrind](valgrind.md) | 内存分析 | 内存检测神器，检测泄漏/越界 |
| [Sanitizers](sanitizers.md) | 运行时检测 | Clang/GCC 内置，快速检测 |
| [gprof](gprof.md) | 性能分析 | GNU 性能分析器 |
| [perf](perf.md) | 性能分析 | Linux 内核性能分析工具 |
| [gperftools](gperftools.md) | 性能分析 | Google 高性能工具集 |

## 工具对比

| 特性 | Valgrind | ASan | gprof | perf |
|------|----------|------|-------|------|
| 性能开销 | 10-50x | 2-3x | 中等 | 低 |
| 内存泄漏 | ✅ | ✅ | ❌ | ❌ |
| 越界检测 | ✅ | ✅ | ❌ | ❌ |
| 竞态检测 | ✅(Helgrind) | ✅(TSan) | ❌ | ❌ |
| 性能分析 | ✅(Callgrind) | ❌ | ✅ | ✅ |
| 多线程支持 | 部分 | ✅ | ❌ | ✅ |

## Sanitizers 家族

| Sanitizer | 用途 | 编译选项 |
|-----------|------|----------|
| AddressSanitizer (ASan) | 内存错误检测 | `-fsanitize=address` |
| MemorySanitizer (MSan) | 未初始化内存 | `-fsanitize=memory` |
| ThreadSanitizer (TSan) | 数据竞态检测 | `-fsanitize=thread` |
| UndefinedBehaviorSanitizer (UBSan) | 未定义行为 | `-fsanitize=undefined` |
| LeakSanitizer (LSan) | 内存泄漏 | `-fsanitize=leak` |

## 使用场景

### 1. 内存问题排查
```bash
# Valgrind 内存检测
valgrind --leak-check=full ./program

# AddressSanitizer
gcc -fsanitize=address -g program.c -o program
./program
```

### 2. 性能分析
```bash
# gprof
gcc -pg program.c -o program
./program
gprof program gmon.out > analysis.txt

# perf
perf record ./program
perf report
```

### 3. 多线程竞态
```bash
# ThreadSanitizer
gcc -fsanitize=thread -g program.c -o program
./program

# Valgrind Helgrind
valgrind --tool=helgrind ./program
```

## 推荐工作流

```
开发阶段：Sanitizers（快速反馈）
     ↓
测试阶段：Valgrind（全面检测）
     ↓
性能优化：perf/gprof（性能瓶颈定位）
```

## 相关文档

- [调试器](../03_debug/) - GDB、LLDB 等
- [静态分析](../05_static_analysis/) - clang-tidy、cppcheck 等