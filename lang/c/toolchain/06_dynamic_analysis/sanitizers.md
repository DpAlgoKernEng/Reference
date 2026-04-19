# Sanitizers - 编译时动态分析工具完整指南

## 1. 概述与背景

### 1.1 工具定位

Sanitizers 是 Clang 和 GCC 编译器提供的编译时动态分析工具集，通过在编译阶段注入检测代码，在运行时实时发现各类内存错误和并发问题。相比 Valgrind 等动态插桩工具，Sanitizers 具有更低性能开销（1-2x vs 10-50x）和更精准的错误定位能力。

### 1.2 发展历史

| 年份 | 版本/事件 | 特性 |
|------|-----------|------|
| 2011 | ASan 首次发布 | AddressSanitizer 在 LLVM 中引入 |
| 2012 | TSan v2 | ThreadSanitizer 新版本，性能大幅提升 |
| 2013 | MSan 发布 | MemorySanitizer 检测未初始化内存 |
| 2014 | UBSan 完善 | UndefinedBehaviorSanitizer 增加更多检查项 |
| 2015 | GCC 集成 | GCC 5.1 完整支持所有 Sanitizers |
| 2016 | LSan 集成 | LeakSanitizer 成为 ASan 默认组件 |
| 2018 | CMake 支持 | CMake 3.13 原生支持 Sanitizers 选项 |
| 2020 | 性能优化 | ASan 性能开销降至 1.5x |
| 2022 | 标准化 | C++ 标准委员会讨论 sanitizer 接口标准化 |

### 1.3 核心特性

| 特性 | 说明 | 优势 |
|------|------|------|
| **低开销** | 性能损失 1-2x | 可用于日常开发和测试 |
| **即时检测** | 运行时实时报告错误 | 无需等待程序结束 |
| **精准定位** | 提供完整调用栈 | 快速定位问题根源 |
| **编译集成** | 无需额外工具 | 与构建系统无缝集成 |
| **CI 友好** | 快速执行速度 | 适合持续集成流程 |

### 1.4 工具分类

| Sanitizer | 检测重点 | 性能开销 | 适用场景 |
|-----------|----------|----------|----------|
| **AddressSanitizer (ASan)** | 地址错误 | 1.5-2x | 日常开发、内存安全检查 |
| **MemorySanitizer (MSan)** | 未初始化内存 | 2-3x | 安全关键代码、嵌入式系统 |
| **UndefinedBehaviorSanitizer (UBSan)** | 未定义行为 | 1.1-1.5x | 可移植性测试、边界条件 |
| **ThreadSanitizer (TSan)** | 数据竞争 | 5-15x | 多线程程序、并发调试 |
| **LeakSanitizer (LSan)** | 内存泄漏 | 集成于 ASan | 资源管理验证 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 | 适用场景 |
|------|------|------|----------|
| **Sanitizers** | 低开销、实时检测、精准定位 | 需要重编译、单次检测单一类型 | 开发阶段、CI/CD |
| **Valgrind** | 无需重编译、工具丰富 | 高开销、执行慢 | 生产环境、第三方代码 |
| **静态分析** | 编译期发现、零运行时开销 | 误报率高、检测范围有限 | 代码审查、早期预防 |
| **自定义断言** | 精准控制、零外部依赖 | 覆盖有限、需要手动编写 | 特定业务逻辑验证 |

## 2. 安装与配置

### 2.1 多平台安装

**Linux (GCC/Clang)**

```bash
# Debian/Ubuntu - GCC 工具链
sudo apt-get install gcc g++ libasan6 libubsan1 libtsan0

# RHEL/CentOS/Fedora
sudo dnf install gcc gcc-c++ libasan libubsan libtsan

# Arch Linux
sudo pacman -S gcc

# 验证安装
gcc --version  # GCC 5.1+ 支持所有 Sanitizers
clang --version  # Clang 3.1+ 支持 ASan
```

**macOS (Clang)**

```bash
# macOS 自带 Clang，无需额外安装
xcode-select --install

# 验证 Clang 版本
clang --version  # Apple Clang 7.0+ 完整支持

# Homebrew 安装最新 LLVM
brew install llvm
export CC=/usr/local/opt/llvm/bin/clang
```

**Windows (MSVC/Clang)**

```bash
# MSVC 2019+ 内置支持
# Visual Studio Installer 勾选 "C++ Clang tools"

# 命令行验证
cl /?  # 检查 MSVC 版本
clang-cl /?  # 检查 Clang-cl 版本
```

### 2.2 版本要求

| Sanitizer | GCC 最低版本 | Clang 最低版本 | MSVC 支持 |
|-----------|--------------|----------------|-----------|
| ASan | 4.8 | 3.1 | VS2019 16.9+ |
| MSan | 9.0 | 3.7 | 不支持 |
| UBSan | 4.9 | 3.3 | VS2019 16.10+ |
| TSan | 4.8 | 3.2 | VS2019 16.10+ |
| LSan | 6.0 | 3.8 | VS2019 16.10+ |

### 2.3 编译配置

**基础编译选项**

```bash
# 最小配置
gcc -fsanitize=address -g program.c -o program

# 推荐配置（包含符号信息和帧指针）
gcc -fsanitize=address \
    -fno-omit-frame-pointer \
    -fno-optimize-sibling-calls \
    -g \
    -O1 \
    program.c -o program

# 选项说明
# -fsanitize=address    : 启用 ASan
# -fno-omit-frame-pointer : 保留帧指针，改善调用栈
# -g                     : 生成调试信息
# -O1                    : 适度优化，避免过度优化隐藏问题
```

**链接器选项**

```bash
# 链接时也需要启用 Sanitizer
gcc -fsanitize=address program.c -o program

# 或者分开设置
gcc -fsanitize=address -c program.c -o program.o
gcc -fsanitize=address program.o -o program

# 动态库链接
gcc -fsanitize=address -shared lib.c -o libsan.so
```

### 2.4 验证安装

**测试程序**

```c
// test_asan.c - ASan 验证
#include <stdlib.h>
#include <string.h>

int main() {
    char *buffer = malloc(10);
    strcpy(buffer, "This is a test");  // 故意溢出
    free(buffer);
    return 0;
}
```

```bash
# 编译并运行
gcc -fsanitize=address -g test_asan.c -o test_asan
./test_asan

# 预期输出包含 heap-buffer-overflow 错误
```

## 3. 基础使用

### 3.1 AddressSanitizer (ASan)

**核心检测能力**

ASan 通过影子内存（Shadow Memory）和红区（Redzone）技术，检测以下内存错误：

| 错误类型 | 英文名称 | 示例场景 |
|----------|----------|----------|
| 堆缓冲区溢出 | heap-buffer-overflow | `malloc(10); buf[15] = 0;` |
| 栈缓冲区溢出 | stack-buffer-overflow | `int arr[5]; arr[10] = 0;` |
| 全局缓冲区溢出 | global-buffer-overflow | 全局数组越界访问 |
| 释放后使用 | heap-use-after-free | `free(p); *p = 10;` |
| 双重释放 | double-free | `free(p); free(p);` |
| 作用域后使用 | stack-use-after-scope | 局部变量地址返回 |
| 返回后使用 | stack-use-after-return | 返回局部变量地址 |

**检测示例**

```c
// 1. 堆缓冲区溢出
void heap_overflow() {
    int *arr = malloc(10 * sizeof(int));
    arr[10] = 0;  // 越界访问
    free(arr);
}

// 2. 栈缓冲区溢出
void stack_overflow() {
    int arr[5];
    arr[10] = 0;  // 栈溢出
}

// 3. 使用已释放内存
void use_after_free() {
    int *p = malloc(sizeof(int));
    free(p);
    *p = 10;  // 使用已释放内存
}

// 4. 返回局部变量地址
int* return_local() {
    int x = 10;
    return &x;  // 危险：返回栈地址
}

// 5. 双重释放
void double_free() {
    int *p = malloc(sizeof(int));
    free(p);
    free(p);  // 双重释放
}
```

**错误报告解析**

```
==12345==ERROR: AddressSanitizer: heap-buffer-overflow
==12345==READ of size 4 at 0x602000000010 thread T0
    #0 0x401234 in main /path/to/program.c:10:5
    #1 0x7f1234567890 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b90)

0x602000000010 is located 4 bytes after 100-byte region
allocated by thread T0 here:
    #0 0x401000 in malloc (/usr/lib/x86_64-linux-gnu/libasan.so.5+0x10d800)
    #1 0x401200 in main /path/to/program.c:8:15
    #2 0x7f1234567890 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b90)

SUMMARY: AddressSanitizer: heap-buffer-overflow /path/to/program.c:10:5 in main
```

**报告关键信息**

| 字段 | 说明 |
|------|------|
| `ERROR类型` | heap-buffer-overflow 等 |
| `READ/WRITE` | 读操作或写操作 |
| `size` | 访问字节数 |
| `thread T0` | 线程编号 |
| `调用栈` | 错误发生位置和分配位置 |
| `SUMMARY` | 总结信息 |

### 3.2 MemorySanitizer (MSan)

**检测未初始化内存**

MSan 追踪内存的初始化状态，检测使用未初始化内存的情况。

```c
#include <stdio.h>
#include <stdlib.h>

void uninitialized_stack() {
    int x;  // 未初始化
    if (x > 0) {  // MSan: use of uninitialized value
        printf("Positive\n");
    }
}

void uninitialized_heap() {
    int *p = malloc(sizeof(int));
    printf("%d\n", *p);  // MSan: use of uninitialized value
    *p = 10;  // 初始化后使用正常
    printf("%d\n", *p);
    free(p);
}

void conditional_uninitialized() {
    int x;
    int y = x;  // MSan: use of uninitialized value
    printf("%d\n", y);
}
```

**编译与运行**

```bash
# 编译（注意：MSan 不与 ASan 兼容）
clang -fsanitize=memory -fPIE -pie -g program.c -o program

# 运行并查看报告
./program

# MSan 输出示例
# ==12345==WARNING: MemorySanitizer: use-of-uninitialized-value
#     #0 0x401234 in main program.c:5:9
```

### 3.3 UndefinedBehaviorSanitizer (UBSan)

**检测未定义行为**

UBSan 检测 C/C++ 标准中未定义的行为，这些行为在不同平台可能产生不同结果。

```c
#include <stdio.h>
#include <limits.h>

void signed_overflow() {
    int x = INT_MAX;
    x = x + 1;  // UBSan: signed integer overflow
    printf("%d\n", x);
}

void division_by_zero() {
    int x = 10;
    int y = 0;
    int z = x / y;  // UBSan: division by zero
}

void null_dereference() {
    int *p = NULL;
    int x = *p;  // UBSan: null pointer dereference
}

void misaligned_access() {
    int arr[2] = {1, 2};
    short *p = (short*)arr;
    // 可能的对齐错误（取决于平台）
}

void shift_overflow() {
    int x = 1 << 32;  // UBSan: shift exponent 32 is too large
}
```

**可选检查项**

| 检查项 | 编译选项 | 说明 |
|--------|----------|------|
| 有符号整数溢出 | `-fsanitize=signed-integer-overflow` | `INT_MAX + 1` |
| 无符号整数溢出 | `-fsanitize=unsigned-integer-overflow` | 某些场景有用 |
| 整数除零 | `-fsanitize=integer-divide-by-zero` | `x / 0` |
| 浮点除零 | `-fsanitize=float-divide-by-zero` | `x / 0.0` |
| 移位溢出 | `-fsanitize=shift` | `1 << 32` |
| 布尔转换 | `-fsanitize=bool` | 非法布尔值 |
| 数组越界 | `-fsanitize=bounds` | `arr[100]` |
| 对齐错误 | `-fsanitize=alignment` | 错误内存对齐 |
| 空指针 | `-fsanitize=null` | 解引用 NULL |
| 类型不匹配 | `-fsanitize=vptr` | C++ 虚函数错误 |

```bash
# 启用所有检查
gcc -fsanitize=undefined -g program.c -o program

# 启用特定检查
gcc -fsanitize=signed-integer-overflow,float-divide-by-zero,null -g program.c

# 运行时选项
export UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=0
./program
```

### 3.4 ThreadSanitizer (TSan)

**检测数据竞争**

TSan 检测多线程程序中的数据竞争问题。

```c
#include <pthread.h>
#include <stdio.h>

int shared_counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        shared_counter++;  // 数据竞争！
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter: %d\n", shared_counter);
    // 可能不是 200000，因为竞争条件
    return 0;
}
```

**编译与运行**

```bash
# 编译
gcc -fsanitize=thread -g program.c -o program -lpthread

# 运行
./program

# TSan 输出示例
# ==================
# WARNING: ThreadSanitizer: data race
#   Write of size 4 at 0x7ffe... by thread T2:
#     #0 increment /path/to/program.c:7
#     #1 <null> <null>
#
#   Previous write of size 4 at 0x7ffe... by thread T1:
#     #0 increment /path/to/program.c:7
#     #1 <null> <null>
# ==================
```

**修复数据竞争**

```c
#include <pthread.h>

int shared_counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment_safe(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        shared_counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
```

### 3.5 LeakSanitizer (LSan)

**检测内存泄漏**

LSan 自动集成在 ASan 中，检测程序退出时的内存泄漏。

```c
#include <stdlib.h>

void memory_leak() {
    int *p = malloc(100);
    // 忘记 free(p)
}

void another_leak() {
    char *str = strdup("Hello");
    // 忘记 free(str)
}

int main() {
    memory_leak();
    another_leak();
    return 0;
    // LSan: 报告泄漏
}
```

**编译与报告**

```bash
# 编译（ASan 自动包含 LSan）
gcc -fsanitize=address -g leak.c -o leak

# 运行
./leak

# LSan 输出
# =================================================================
# ==12345==ERROR: LeakSanitizer: detected memory leaks
#
# Direct leak of 100 byte(s) in 1 object(s) allocated from:
#     #0 0x401000 in malloc (/usr/lib/libasan.so+0x10d800)
#     #1 0x401100 in memory_leak /path/to/leak.c:4:15
#
# Direct leak of 6 byte(s) in 1 object(s) allocated from:
#     #0 0x401000 in malloc (/usr/lib/libasan.so+0x10d800)
#     #1 0x402000 in strdup (/usr/lib/libc.so+0x123456)
#     #2 0x401200 in another_leak /path/to/leak.c:9:16
#
# SUMMARY: AddressSanitizer: 106 byte(s) leaked in 2 allocation(s).
# =================================================================
```

**独立使用 LSan**

```bash
# 在不支持 ASan 的环境单独启用
gcc -fsanitize=leak -g program.c -o program
```

## 4. 进阶特性

### 4.1 组合使用策略

**兼容性矩阵**

| 组合 | 支持 | 说明 |
|------|------|------|
| ASan + LSan | ✅ | 默认启用，无需额外配置 |
| ASan + UBSan | ✅ | 推荐组合，覆盖面广 |
| TSan + 其他 | ❌ | 不兼容其他 Sanitizer |
| MSan + 其他 | ❌ | 不兼容其他 Sanitizer |
| UBSan + 其他 | ✅ | 可与 ASan/TSan 组合 |

**推荐配置**

```bash
# 开发阶段：ASan + UBSan（最常用）
gcc -fsanitize=address,undefined -g program.c -o program

# 多线程程序：单独 TSan
gcc -fsanitize=thread -g program.c -o program -lpthread

# 嵌入式/安全关键：MSan
clang -fsanitize=memory -g program.c -o program

# 生产环境：LSan 轻量检查
gcc -fsanitize=leak -g program.c -o program
```

### 4.2 环境变量配置

**ASan 环境变量**

```bash
# 启用泄漏检测
export ASAN_OPTIONS=detect_leaks=1

# 错误时继续执行（用于检测多个错误）
export ASAN_OPTIONS=detect_leaks=1:halt_on_error=0

# 错误时中止（默认行为）
export ASAN_OPTIONS=abort_on_error=1

# 启用详细日志
export ASAN_OPTIONS=verbosity=1

# 设置内存限制（MB）
export ASAN_OPTIONS=malloc_context_size=30:quarantine_size_mb=64

# 符号化调用栈
export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer
export ASAN_OPTIONS=symbolize=1:external_symbolizer_path=/usr/bin/llvm-symbolizer
```

**TSan 环境变量**

```bash
# 检测死锁
export TSAN_OPTIONS=detect_deadlocks=1

# 第二次死锁时崩溃
export TSAN_OPTIONS=second_deadlock_crash=1

# 历史记录大小
export TSAN_OPTIONS=history_size=7

# 详细输出
export TSAN_OPTIONS=verbosity=1
```

**UBSan 环境变量**

```bash
# 打印调用栈
export UBSAN_OPTIONS=print_stacktrace=1

# 错误时继续
export UBSAN_OPTIONS=halt_on_error=0

# 输出到文件
export UBSAN_OPTIONS=log_path=/var/log/ubsan.log
```

**LSan 环境变量**

```bash
# 启用泄漏检测
export LSAN_OPTIONS=detect_leaks=1

# 快速泄漏检查（牺牲准确性）
export LSAN_OPTIONS=fast_unwind_on_malloc=1

# 泄漏时中止
export LSAN_OPTIONS=exitcode=1
```

### 4.3 抑制已知问题

**ASan 抑制文件**

```
# asan.supp - ASan 抑制文件

# 抑制特定函数中的泄漏
leak:known_leak_function

# 抑制特定库中的问题
interceptor_via_lib:liblegacy.so

# 抑制特定类型的错误
heap-buffer-overflow:legacy_code.c
```

```bash
export ASAN_OPTIONS=suppressions=asan.supp
./program
```

**TSan 抑制文件**

```
# tsan.supp - TSan 抑制文件

# 抑制已知的数据竞争
race:legacy_data
race:known_race_pattern

# 抑制特定函数中的竞争
race:unsafe_global_function

# 抑制特定文件中的问题
race:legacy_module.c
```

```bash
export TSAN_OPTIONS=suppressions=tsan.supp
./program
```

### 4.4 符号化调用栈

```bash
# 安装符号化工具
# Linux
sudo apt-get install llvm-symbolizer

# macOS (已随 Xcode 安装)
# 位于 /Applications/Xcode.app/Contents/Developer/Toolchains/

# 配置符号化
export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer
export ASAN_OPTIONS=symbolize=1

# 或在运行时指定
ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer ./program
```

## 5. 性能优化

### 5.1 性能开销分析

| Sanitizer | 典型开销 | 内存开销 | 最优化场景 |
|-----------|----------|----------|------------|
| ASan | 1.5-2x | 3-5x | -O1 优化级别 |
| MSan | 2-3x | 2-3x | 单独编译运行 |
| UBSan | 1.1-1.5x | 1x | 与 ASan 组合 |
| TSan | 5-15x | 5-10x | 减少线程数测试 |
| LSan | 集成于 ASan | 最小 | 程序退出时检查 |

### 5.2 调优策略

**优化编译选项**

```bash
# 适度优化，避免隐藏问题
gcc -fsanitize=address -O1 -g program.c -o program

# 避免过度优化
# -O0 : 调试友好，但性能差
# -O1 : 推荐，平衡性能和检测能力
# -O2/-O3 : 可能优化掉某些问题

# 减少误报
gcc -fsanitize=address -fno-optimize-sibling-calls program.c

# 控制红区大小
gcc -fsanitize=address -fsanitize-address-poison-custom-array-cookie program.c
```

**运行时优化**

```bash
# 减少内存使用
export ASAN_OPTIONS=quarantine_size_mb=32

# 加快执行速度
export ASAN_OPTIONS=fast_unwind_on_malloc=1

# 限制栈深度
export ASAN_OPTIONS=malloc_context_size=10

# 并行测试
export ASAN_OPTIONS=detect_leaks=0  # 测试时禁用泄漏检查
```

### 5.3 最佳实践

**1. 开发阶段**

```bash
# 日常开发：ASan + UBSan
make CFLAGS="-fsanitize=address,undefined -g -O1"

# 定期运行：MSan 检查
make CFLAGS="-fsanitize=memory -g -O1"

# 代码提交前：完整测试
make CFLAGS="-fsanitize=address,undefined -g" test
```

**2. CI/CD 集成**

```bash
# CI 脚本示例
#!/bin/bash
set -e

# ASan 测试
make clean && make CFLAGS="-fsanitize=address,undefined -g -O1"
./run_tests

# TSan 测试（多线程）
make clean && make CFLAGS="-fsanitize=thread -g -O1"
./run_threaded_tests
```

**3. 选择性启用**

```c
// 代码中控制检测范围
#ifdef __SANITIZE_ADDRESS__
    // ASan 特定代码
    __asan_poison_memory_region(ptr, size);
#endif
```

## 6. 问题排查

### 6.1 常见问题

**问题 1: 内存不足**

```
ERROR: AddressSanitizer failed to allocate 0x... bytes
```

**解决方案**

```bash
# 减少影子内存使用
export ASAN_OPTIONS=quarantine_size_mb=16

# 或减少分配限制
ulimit -v unlimited
```

**问题 2: 符号缺失**

```
#0 0x401234 in ?? ??:0
```

**解决方案**

```bash
# 确保包含调试信息
gcc -fsanitize=address -g program.c

# 配置符号化工具
export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer
```

**问题 3: 兼容性错误**

```
error: unsupported option '-fsanitize=thread' for target
```

**解决方案**

```bash
# 检查编译器版本
gcc --version

# macOS 使用 Clang
clang -fsanitize=thread program.c

# Windows 使用 MSVC 2019+
cl /fsanitize=address program.c
```

**问题 4: 检测不到问题**

```c
// 问题：优化导致检测失效
int *p = malloc(10);
free(p);
// 编译器优化可能移除这段代码
```

**解决方案**

```bash
# 使用 -O0 或 -O1
gcc -fsanitize=address -O0 program.c

# 防止优化移除代码
volatile int *vp = p;
```

### 6.2 调试技巧

**获取详细信息**

```bash
# 启用详细日志
export ASAN_OPTIONS=verbosity=1:debug=1

# 输出到文件
export ASAN_OPTIONS=log_path=/tmp/asan.log

# 保留核心转储
ulimit -c unlimited
export ASAN_OPTIONS=abort_on_error=1
```

**调试段错误**

```bash
# 生成核心转储
ulimit -c unlimited
./program

# 用 GDB 分析
gdb ./program core
(gdb) bt full
```

**内存映射分析**

```bash
# 查看内存映射
cat /proc/PID/maps

# ASan 报告中的地址解析
# 0x602000000010 -> 计算 shadow memory 地址
```

**性能分析**

```bash
# 时间测量
time ./program_with_asan
time ./program_without_asan

# 内存使用
/usr/bin/time -v ./program
```

## 7. 集成实践

### 7.1 CMake 集成

**基础配置**

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.13)
project(MyProject C)

# Sanitizer 选项
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_MSAN "Enable MemorySanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

# ASan 配置
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address)
    add_compile_options(-fno-omit-frame-pointer)
    add_compile_options(-fno-optimize-sibling-calls)
    add_compile_options(-g)
    add_link_options(-fsanitize=address)
    message(STATUS "AddressSanitizer enabled")
endif()

# TSan 配置
if(ENABLE_TSAN)
    add_compile_options(-fsanitize=thread)
    add_compile_options(-fno-omit-frame-pointer)
    add_compile_options(-g)
    add_link_options(-fsanitize=thread)
    message(STATUS "ThreadSanitizer enabled")
endif()

# UBSan 配置
if(ENABLE_UBSAN)
    add_compile_options(-fsanitize=undefined)
    add_compile_options(-fno-omit-frame-pointer)
    add_compile_options(-g)
    add_link_options(-fsanitize=undefined)
    message(STATUS "UndefinedBehaviorSanitizer enabled")
endif()

# MSan 配置（注意：不与其他 Sanitizer 兼容）
if(ENABLE_MSAN)
    if(ENABLE_ASAN OR ENABLE_TSAN OR ENABLE_UBSAN)
        message(FATAL_ERROR "MSan is incompatible with other sanitizers")
    endif()
    add_compile_options(-fsanitize=memory)
    add_compile_options(-fno-omit-frame-pointer)
    add_compile_options(-g)
    add_link_options(-fsanitize=memory)
    message(STATUS "MemorySanitizer enabled")
endif()

add_executable(myapp main.c)
```

**构建命令**

```bash
# 启用 ASan
cmake -B build -DENABLE_ASAN=ON
cmake --build build

# 启用 TSan
cmake -B build -DENABLE_TSAN=ON
cmake --build build

# 组合 ASan + UBSan
cmake -B build -DENABLE_ASAN=ON -DENABLE_UBSAN=ON
cmake --build build
```

### 7.2 Makefile 集成

```makefile
# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -g

# Sanitizer 目标
ifdef ASAN
    CFLAGS += -fsanitize=address -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=address
endif

ifdef MSAN
    CFLAGS += -fsanitize=memory -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=memory
endif

ifdef TSAN
    CFLAGS += -fsanitize=thread -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=thread
endif

ifdef UBSAN
    CFLAGS += -fsanitize=undefined -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=undefined
endif

# 默认组合
ifdef SANITIZE
    CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=address,undefined
endif

SRCS = main.c utils.c
OBJS = $(SRCS:.c=.o)
TARGET = myapp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test
```

**使用方式**

```bash
# 启用 ASan
make ASAN=1

# 启用常用组合
make SANITIZE=1

# 运行测试
make SANITIZE=1 test
```

### 7.3 CI/CD 配置

**GitHub Actions**

```yaml
# .github/workflows/sanitizer.yml
name: Sanitizer Tests

on: [push, pull_request]

jobs:
  asan-test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

      - name: Install dependencies
        run: sudo apt-get install -y llvm-symbolizer

      - name: Configure with ASan
        run: cmake -B build -DENABLE_ASAN=ON -DENABLE_UBSAN=ON

      - name: Build
        run: cmake --build build

      - name: Run tests
        run: |
          cd build
          ctest --output-on-failure
        env:
          ASAN_OPTIONS: detect_leaks=1:detect_stack_use_after_return=1
          UBSAN_OPTIONS: print_stacktrace=1

  tsan-test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

      - name: Configure with TSan
        run: cmake -B build -DENABLE_TSAN=ON

      - name: Build
        run: cmake --build build

      - name: Run thread tests
        run: cd build && ctest -R thread_test --output-on-failure
```

**GitLab CI**

```yaml
# .gitlab-ci.yml
stages:
  - test

sanitizer:asan:
  stage: test
  script:
    - cmake -B build -DENABLE_ASAN=ON -DENABLE_UBSAN=ON
    - cmake --build build
    - cd build && ctest --output-on-failure
  variables:
    ASAN_OPTIONS: detect_leaks=1:abort_on_error=1
    UBSAN_OPTIONS: print_stacktrace=1:halt_on_error=1

sanitizer:tsan:
  stage: test
  script:
    - cmake -B build -DENABLE_TSAN=ON
    - cmake --build build
    - cd build && ctest -R thread --output-on-failure
  variables:
    TSAN_OPTIONS: detect_deadlocks=1:second_deadlock_crash=1
```

### 7.4 实战案例

**案例 1: 内存泄漏检测**

```c
// leak_example.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int age;
} Person;

Person* create_person(const char *name, int age) {
    Person *p = malloc(sizeof(Person));
    p->name = strdup(name);  // 内存泄漏：析构时未释放
    p->age = age;
    return p;
}

void process_persons() {
    Person *people[3];

    for (int i = 0; i < 3; i++) {
        char name[20];
        sprintf(name, "Person%d", i);
        people[i] = create_person(name, 25 + i);
    }

    for (int i = 0; i < 3; i++) {
        printf("Name: %s, Age: %d\n", people[i]->name, people[i]->age);
        free(people[i]);  // 泄漏：未释放 people[i]->name
    }
}

int main() {
    process_persons();
    return 0;
}
```

```bash
# 编译运行
gcc -fsanitize=address -g leak_example.c -o leak_example
./leak_example

# 输出
# =================================================================
# ==12345==ERROR: LeakSanitizer: detected memory leaks
#
# Direct leak of 60 byte(s) in 3 object(s) allocated from:
#     #0 malloc
#     #1 strdup /path/to/leak_example.c:11
#     #2 create_person /path/to/leak_example.c:11
#
# SUMMARY: AddressSanitizer: 60 byte(s) leaked in 3 allocation(s).
```

**修复方案**

```c
void free_person(Person *p) {
    free(p->name);  // 释放内部字符串
    free(p);        // 释放结构体
}

int main() {
    process_persons();
    // 修改 process_persons 使用 free_person
    return 0;
}
```

**案例 2: 数据竞争修复**

```c
// race_example.c
#include <pthread.h>
#include <stdio.h>

int counter = 0;

void* increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        counter++;  // 数据竞争
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter: %d (expected: 200000)\n", counter);
    return 0;
}
```

```bash
# 编译运行
gcc -fsanitize=thread -g race_example.c -o race_example -lpthread
./race_example

# 输出
# ==================
# WARNING: ThreadSanitizer: data race
#   Write of size 4 at 0x... by thread T2:
#     #0 increment /path/to/race_example.c:7
# ...
```

**修复方案**

```c
#include <stdatomic.h>

atomic_int counter = 0;  // 使用原子操作

void* increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        atomic_fetch_add(&counter, 1);
    }
    return NULL;
}

// 或者使用互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment_mutex(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| **AddressSanitizer 官方文档** | https://clang.llvm.org/docs/AddressSanitizer.html |
| **MemorySanitizer 官方文档** | https://clang.llvm.org/docs/MemorySanitizer.html |
| **ThreadSanitizer 官方文档** | https://clang.llvm.org/docs/ThreadSanitizer.html |
| **UndefinedBehaviorSanitizer 文档** | https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html |
| **GCC Sanitizer 选项** | https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html |
| **Google Sanitizers Wiki** | https://github.com/google/sanitizers |

### 8.2 学习路径

**入门阶段**

1. 理解 Sanitizers 基本概念和原理
2. 掌握 ASan 基础用法（缓冲区溢出、UAF）
3. 学会解读错误报告和调用栈
4. 在小型项目中集成 ASan

**进阶阶段**

1. 掌握 TSan 检测数据竞争
2. 学习 UBSan 检测未定义行为
3. 理解 MSan 的初始化检查
4. 配置 CI/CD 自动化测试

**高级阶段**

1. 自定义抑制规则处理误报
2. 优化性能开销策略
3. 深入理解影子内存机制
4. 与静态分析工具组合使用

### 8.3 相关工具

| 工具 | 说明 | 对比 |
|------|------|------|
| **Valgrind** | 动态分析工具套件 | 无需重编译，但开销大 |
| **Static Analyzer** | Clang 静态分析 | 编译期检测，零运行时开销 |
| **Coverity** | 商业静态分析 | 企业级，功能全面 |
| **CPPCheck** | 开源静态分析 | 轻量级，适合 CI |
| **PVS-Studio** | 商业静态分析 | 误报率低，规则丰富 |

### 8.4 最佳实践总结

| 阶段 | 推荐配置 | 目的 |
|------|----------|------|
| **开发阶段** | `-fsanitize=address,undefined` | 快速发现常见错误 |
| **代码审查** | `-fsanitize=address,undefined -Werror` | 确保代码质量 |
| **CI/CD** | ASan + UBSan + TSan 组合测试 | 全面覆盖 |
| **发布前** | 完整 Sanitizer 测试套件 | 最后把关 |
| **生产环境** | Valgrind 或 LSan 轻量检查 | 持续监控 |

---

*本指南涵盖 Sanitizers 的核心概念、使用方法和最佳实践，适用于 C/C++ 开发全流程的内存安全检测。*