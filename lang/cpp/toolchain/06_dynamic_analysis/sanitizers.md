# Sanitizers - 编译时动态分析工具

## 1. 概述与背景

### 1.1 工具定位

Sanitizers 是 Clang/GCC 编译器提供的运行时动态分析工具集，通过编译时插桩技术在程序运行时检测各类内存错误、线程问题和未定义行为。与 Valgrind 等外部工具不同，Sanitizers 直接集成在编译器中，具有性能开销低、检测精度高的特点。

### 1.2 发展历史

| 年份 | 版本/事件 | 说明 |
|------|----------|------|
| 2011 | AddressSanitizer | Google 开源，首次发布 |
| 2012 | ThreadSanitizer v2 | 数据竞争检测器发布 |
| 2013 | MemorySanitizer | 未初始化内存检测器发布 |
| 2014 | UndefinedBehaviorSanitizer | 未定义行为检测器发布 |
| 2015 | LeakSanitizer | 内存泄漏检测器发布 |
| 2017 | GCC 7+ | 全面支持所有 Sanitizers |
| 2020 | C++20 | 标准库增强 Sanitizer 支持 |

### 1.3 核心特性

- **低性能开销**：ASan 约 2x，TSan 约 5-15x，远低于 Valgrind 的 10-50x
- **即时检测**：错误发生时立即报告，精确定位源码位置
- **零误报设计**：报告的错误均为真实问题
- **编译器集成**：无需额外工具链，只需添加编译选项
- **CI/CD 友好**：易于集成到持续集成流程

### 1.4 适用场景

| 场景 | 推荐 Sanitizer | 说明 |
|------|----------------|------|
| 日常开发 | ASan + UBSan | 覆盖最常见问题 |
| 多线程程序 | TSan | 检测数据竞争和死锁 |
| 安全敏感代码 | MSan | 检测未初始化内存 |
| 内存泄漏检测 | LSan | 自动集成于 ASan |
| CI/CD 流程 | ASan + UBSan | 平衡检测覆盖与性能 |

### 1.5 对比分析

| 特性 | Sanitizers | Valgrind | 静态分析 |
|------|------------|----------|----------|
| 性能开销 | 1-15x | 10-50x | 无运行开销 |
| 检测时机 | 运行时即时 | 运行时汇总 | 编译时 |
| 重编译要求 | 需要 | 不需要 | 不需要 |
| 精确度 | 高（零误报） | 中 | 低（高误报） |
| 集成 CI | 简单 | 较慢 | 简单 |
| 线程检测 | TSan | Helgrind/DRD | 有限 |
| 适用场景 | 开发/测试 | 生产环境分析 | 代码审查 |

## 2. 安装与配置

### 2.1 多平台安装

Sanitizers 随编译器一起安装，无需额外配置：

```bash
# Linux - GCC
sudo apt install gcc g++  # Debian/Ubuntu
sudo yum install gcc gcc-c++  # CentOS/RHEL

# Linux - Clang
sudo apt install clang

# macOS - Xcode 自带 Clang
xcode-select --install

# Windows - MSVC (部分支持)
# Visual Studio 2019+ 内置 ASan 支持
```

### 2.2 版本管理

| 编译器 | 最低版本 | 完整支持版本 |
|--------|----------|--------------|
| GCC | 4.8+ | 7.0+ |
| Clang | 3.1+ | 6.0+ |
| MSVC | VS2019 | VS2022 |

### 2.3 环境配置

```bash
# 检查编译器支持
gcc --version
clang --version

# 验证 Sanitizer 支持
echo "int main() { return 0; }" > test.c
gcc -fsanitize=address test.c -o test && ./test && echo "ASan OK"
gcc -fsanitize=thread test.c -o test && ./test && echo "TSan OK"
gcc -fsanitize=memory test.c -o test && ./test && echo "MSan OK"
```

### 2.4 验证安装

```bash
# 查看编译器支持的 Sanitizer 选项
gcc --help=warnings | grep sanitize

# 输出示例
# -fsanitize=address
# -fsanitize=thread
# -fsanitize=memory
# -fsanitize=undefined
# -fsanitize=leak
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 最简单的使用方式
gcc -fsanitize=address -g program.c -o program
./program

# 组合多个 Sanitizer
gcc -fsanitize=address,undefined -g program.c -o program
./program
```

### 3.2 Sanitizer 类型概览

| Sanitizer | 编译选项 | 检测内容 | 性能开销 |
|-----------|----------|----------|----------|
| **ASan** | `-fsanitize=address` | 缓冲区溢出、非法访问 | ~2x |
| **MSan** | `-fsanitize=memory` | 未初始化内存使用 | ~3x |
| **UBSan** | `-fsanitize=undefined` | 未定义行为 | ~1.1x |
| **TSan** | `-fsanitize=thread` | 数据竞争 | ~5-15x |
| **LSan** | `-fsanitize=leak` | 内存泄漏 | ~1.1x |

### 3.3 基本命令

```bash
# AddressSanitizer
gcc -fsanitize=address -fno-omit-frame-pointer -g program.c -o program

# MemorySanitizer (仅限 Clang)
clang -fsanitize=memory -fno-omit-frame-pointer -g program.c -o program

# ThreadSanitizer
gcc -fsanitize=thread -fno-omit-frame-pointer -g program.c -o program -lpthread

# UndefinedBehaviorSanitizer
gcc -fsanitize=undefined -fno-omit-frame-pointer -g program.c -o program

# LeakSanitizer
gcc -fsanitize=leak -g program.c -o program
```

### 3.4 常用编译选项

| 选项 | 说明 |
|------|------|
| `-fsanitize=<type>` | 启用指定 Sanitizer |
| `-fno-omit-frame-pointer` | 保留帧指针，改善堆栈追踪 |
| `-fno-sanitize-recover` | 检测到错误时终止程序 |
| `-fsanitize-recover` | 检测到错误后继续执行 |
| `-fno-sanitize-address-use-after-scope` | 禁用作用域后使用检测 |

## 4. 核心检测器详解

### 4.1 AddressSanitizer (ASan)

AddressSanitizer 是最常用的检测器，能检测堆、栈、全局变量的内存访问错误。

**编译启用：**

```bash
# 基础启用
gcc -fsanitize=address -g program.c -o program

# 详细选项
gcc -fsanitize=address \
    -fno-omit-frame-pointer \
    -fsanitize-recover=address \
    -g program.c -o program

# CMake 配置
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
```

**检测的问题类型：**

```c
// 1. 堆缓冲区溢出
int* arr = malloc(10 * sizeof(int));
arr[10] = 0;  // ASan: heap-buffer-overflow

// 2. 栈缓冲区溢出
int stack_arr[5];
stack_arr[10] = 0;  // ASan: stack-buffer-overflow

// 3. 全局缓冲区溢出
int global_arr[10];
int main() {
    global_arr[11] = 0;  // ASan: global-buffer-overflow
}

// 4. 释放后使用 (Use-After-Free)
int* p = malloc(sizeof(int));
free(p);
*p = 10;  // ASan: heap-use-after-free

// 5. 双重释放 (Double-Free)
int* p = malloc(sizeof(int));
free(p);
free(p);  // ASan: double-free

// 6. 作用域后使用 (Use-After-Scope)
int* func() {
    int x = 10;
    return &x;  // ASan: stack-use-after-scope
}

// 7. 返回后使用 (Use-After-Return)
int* dangling_pointer() {
    int local = 42;
    return &local;  // ASan: stack-use-after-return
}
```

**输出示例：**

```
==12345==ERROR: AddressSanitizer: heap-buffer-overflow
==12345==READ of size 4 at 0x602000000010 thread T0
    #0 0x401234 in main program.c:10
    #1 0x7f... in __libc_start_main

0x602000000010 is located 4 bytes after 100-byte region
allocated by:
    #0 0x401000 in malloc
    #1 0x401200 in main program.c:8
```

### 4.2 MemorySanitizer (MSan)

MemorySanitizer 检测未初始化内存的读取，仅 Clang 支持完整实现。

**编译启用：**

```bash
# 仅 Clang 支持
clang -fsanitize=memory -fno-omit-frame-pointer -g program.c -o program

# 追踪未初始化来源
clang -fsanitize=memory -fsanitize-memory-track-origins=2 -g program.c -o program
```

**检测的问题：**

```c
// 1. 未初始化变量读取
int x;
printf("%d", x);  // MSan: use of uninitialized value

// 2. 未初始化堆内存
int* p = malloc(sizeof(int));
printf("%d", *p);  // MSan: use of uninitialized value
*p = 10;  // 初始化后 OK

// 3. 未初始化结构体成员
struct { int a; int b; } s;
s.a = 1;
printf("%d", s.b);  // MSan: use of uninitialized value
```

### 4.3 UndefinedBehaviorSanitizer (UBSan)

UBSan 检测 C/C++ 中的未定义行为，可在编译时或运行时检查。

**编译启用：**

```bash
# 启用所有检查
gcc -fsanitize=undefined -g program.c -o program

# 启用特定检查
gcc -fsanitize=signed-integer-overflow,float-divide-by-zero -g program.c -o program

# 运行时检查 + 打印堆栈
gcc -fsanitize=undefined -fno-sanitize-recover=undefined -g program.c -o program
```

**检测的问题类型：**

```c
#include <limits.h>

// 1. 有符号整数溢出
int x = INT_MAX;
x = x + 1;  // UBSan: signed integer overflow

// 2. 整数除零
int y = 10 / 0;  // UBSan: division by zero

// 3. 浮点除零
float f = 1.0f / 0.0f;  // UBSan: float-divide-by-zero

// 4. 空指针解引用
int* p = NULL;
int x = *p;  // UBSan: null pointer dereference

// 5. 移位溢出
int x = 1 << 32;  // UBSan: shift exponent overflow

// 6. 数组越界（动态检查有限）
int arr[10];
int idx = 20;
arr[idx] = 0;  // UBSan: index out of bounds (仅部分场景)

// 7. 不兼容类型转换
float f = 0.7;
int* p = (int*)&f;  // UBSan: misaligned address

// 8. 无效枚举值
enum Color { RED, GREEN, BLUE };
enum Color c = (enum Color)10;  // UBSan: invalid enum value
```

**可选检查选项：**

| 选项 | 说明 |
|------|------|
| `signed-integer-overflow` | 有符号整数溢出 |
| `unsigned-integer-overflow` | 无符号整数溢出 |
| `float-divide-by-zero` | 浮点除零 |
| `integer-divide-by-zero` | 整数除零 |
| `shift` | 移位溢出 |
| `bool` | 布尔值非法转换 |
| `bounds` | 数组越界 |
| `alignment` | 内存对齐错误 |
| `vptr` | C++ 虚函数指针错误 |
| `null` | 空指针解引用 |
| `return` | 非空函数未返回值 |
| `vla-bound` | 变长数组边界错误 |

### 4.4 ThreadSanitizer (TSan)

ThreadSanitizer 检测多线程程序中的数据竞争和死锁。

**编译启用：**

```bash
gcc -fsanitize=thread -fno-omit-frame-pointer -g program.c -o program -lpthread
```

**检测数据竞争：**

```c
#include <pthread.h>

int shared_data = 0;  // 共享变量，无同步保护

void* thread_func(void* arg) {
    for (int i = 0; i < 1000; i++) {
        shared_data++;  // TSan: data race
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Result: %d\n", shared_data);
    return 0;
}
```

**输出示例：**

```
==================
WARNING: ThreadSanitizer: data race
  Write of size 4 at 0x7b... by thread T2:
    #0 thread_func program.c:6 (program+0x...)

  Previous write of size 4 at 0x7b... by thread T1:
    #0 thread_func program.c:6 (program+0x...)

  Location is global 'shared_data' of size 4 at 0x... (program.c:3)
==================
```

**修复方案：**

```c
#include <pthread.h>
#include <stdatomic.h>

// 方案 1: 使用原子操作
atomic_int shared_data = 0;

// 方案 2: 使用互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_data = 0;

void* thread_func(void* arg) {
    for (int i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex);
        shared_data++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
```

### 4.5 LeakSanitizer (LSan)

LeakSanitizer 检测内存泄漏，通常与 ASan 一起使用。

**编译启用：**

```bash
# ASan 自动包含 LSan（Linux）
gcc -fsanitize=address -g program.c -o program

# 单独启用 LSan
gcc -fsanitize=leak -g program.c -o program
```

**检测内存泄漏：**

```c
#include <stdlib.h>

void leak_example() {
    int* p = malloc(100 * sizeof(int));
    // 忘记 free(p) - 内存泄漏
}

void leak_example2() {
    int* p = malloc(200);
    p = NULL;  // 丢失指针 - 内存泄漏
}

int main() {
    leak_example();
    leak_example2();
    return 0;
}

// 运行输出:
// =================================================================
// ==12345==ERROR: LeakSanitizer: detected memory leaks
// Direct leak of 400 byte(s) in 1 object(s) allocated from:
//     #0 malloc program.c:4
//     #1 leak_example program.c:5
// SUMMARY: AddressSanitizer: 600 byte(s) leaked in 2 allocation(s).
```

## 5. 进阶特性

### 5.1 组合使用

**兼容性矩阵：**

| 组合 | 支持 | 说明 |
|------|------|------|
| ASan + LSan | Yes | 自动包含 |
| ASan + UBSan | Yes | 常用组合 |
| ASan + TSan | No | 不兼容 |
| MSan + 其他 | No | 必须单独使用 |
| TSan + 其他 | No | 必须单独使用 |

**推荐组合配置：**

```bash
# 开发阶段：ASan + UBSan（推荐）
gcc -fsanitize=address,undefined -fno-omit-frame-pointer -g program.c -o program

# 多线程程序：单独使用 TSan
gcc -fsanitize=thread -fno-omit-frame-pointer -g program.c -o program -lpthread

# 未初始化检查：单独使用 MSan
clang -fsanitize=memory -fno-omit-frame-pointer -g program.c -o program

# 完整检查（CI 流水线）
# 步骤 1: ASan + UBSan 测试
# 步骤 2: TSan 测试（多线程部分）
# 步骤 3: MSan 测试（Clang 环境）
```

### 5.2 环境变量控制

| 变量 | 说明 | 示例 |
|------|------|------|
| `ASAN_OPTIONS` | ASan 配置 | `detect_leaks=1:halt_on_error=0` |
| `MSAN_OPTIONS` | MSan 配置 | `track_origins=2` |
| `UBSAN_OPTIONS` | UBSan 配置 | `print_stacktrace=1` |
| `TSAN_OPTIONS` | TSan 配置 | `detect_deadlocks=1` |
| `LSAN_OPTIONS` | LSan 配置 | `leak_check_at_exit=1` |

**常用配置示例：**

```bash
# ASan 配置
export ASAN_OPTIONS=detect_leaks=1:halt_on_error=0:abort_on_error=1:detect_stack_use_after_return=1

# UBSan 配置
export UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1

# TSan 配置
export TSAN_OPTIONS=detect_deadlocks=1:second_deadlock_crash=1:history_size=7

# MSan 配置
export MSAN_OPTIONS=track_origins=2:warn Origins=1
```

### 5.3 抑制警告

对于已知且暂时不修复的问题，可使用抑制文件。

**ASan 抑制文件 (asan.supp)：**

```
# 抑制特定函数中的泄漏
leak:known_leak_function

# 抑制特定库中的错误
interceptor_via_lib:libname.so

# 抑制特定类型的错误
heap-buffer-overflow:known_overflow_function
```

**TSan 抑制文件 (tsan.supp)：**

```
# 抑制已知数据竞争
race:known_race_pattern

# 抑制特定函数中的竞争
race:RaceConditionClass::knownRaceMethod

# 抑制死锁检测
deadlock:known_deadlock_pattern
```

**使用方法：**

```bash
# 指定抑制文件
export ASAN_OPTIONS=suppressions=asan.supp
export TSAN_OPTIONS=suppressions=tsan.supp

./program
```

## 6. 性能优化与最佳实践

### 6.1 调优策略

| 策略 | 说明 | 配置方法 |
|------|------|----------|
| 减少检测范围 | 仅对关键模块启用 | 编译脚本控制 |
| 抑制已知问题 | 使用抑制文件 | `suppressions=file.supp` |
| 降低开销 | 使用 `halt_on_error=0` | 环境变量配置 |
| 分层测试 | 不同 CI 阶段用不同 Sanitizer | CI/CD 配置 |

### 6.2 最佳实践

1. **开发阶段**
   - 默认启用 ASan + UBSan
   - 保持调试符号 (`-g`)
   - 保留帧指针 (`-fno-omit-frame-pointer`)

2. **CI/CD 集成**
   - 分阶段运行不同 Sanitizer
   - 设置超时（TSan 较慢）
   - 保存错误报告

3. **生产环境**
   - 不建议在生产环境使用 Sanitizers
   - 使用 Valgrind 进行离线分析
   - 核心转储 + 事后分析

4. **性能考量**
   - ASan 内存开销约 3-5 倍
   - TSan 内存开销约 5-10 倍
   - MSan 内存开销约 2-3 倍

## 7. 集成实践

### 7.1 CMake 集成

```cmake
# Sanitizer 选项
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_MSAN "Enable MemorySanitizer" OFF)
option(ENABLE_LSAN "Enable LeakSanitizer" OFF)

# ASan 配置
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
    # LSan 自动包含于 ASan
endif()

# TSan 配置
if(ENABLE_TSAN)
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=thread)
    # TSan 与其他 Sanitizer 不兼容
    if(ENABLE_ASAN OR ENABLE_UBSAN)
        message(FATAL_ERROR "TSan is incompatible with other sanitizers")
    endif()
endif()

# UBSan 配置
if(ENABLE_UBSAN)
    add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=undefined)
endif()

# MSan 配置（仅 Clang）
if(ENABLE_MSAN)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "MSan requires Clang compiler")
    endif()
    add_compile_options(-fsanitize=memory -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=memory)
endif()

# LSan 单独配置
if(ENABLE_LSAN AND NOT ENABLE_ASAN)
    add_compile_options(-fsanitize=leak -g)
    add_link_options(-fsanitize=leak)
endif()
```

### 7.2 CI/CD 配置

**GitHub Actions 示例：**

```yaml
name: Sanitizer Tests

on: [push, pull_request]

jobs:
  asan-ubsan:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Configure
        run: cmake -B build -DENABLE_ASAN=ON -DENABLE_UBSAN=ON
      - name: Build
        run: cmake --build build
      - name: Test
        run: ctest --test-dir build --output-on-failure
        env:
          ASAN_OPTIONS: detect_leaks=1:halt_on_error=1
          UBSAN_OPTIONS: print_stacktrace=1:halt_on_error=1

  tsan:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Configure
        run: cmake -B build -DENABLE_TSAN=ON
      - name: Build
        run: cmake --build build
      - name: Test
        run: ctest --test-dir build --output-on-failure
        env:
          TSAN_OPTIONS: detect_deadlocks=1
```

### 7.3 实战案例

**案例 1：定位缓冲区溢出**

```c
// buggy.c - 存在越界写入
#include <stdlib.h>
#include <string.h>

void process_data(char* input) {
    char buffer[10];
    strcpy(buffer, input);  // 潜在越界
    printf("Processed: %s\n", buffer);
}

int main(int argc, char** argv) {
    if (argc > 1) {
        process_data(argv[1]);
    }
    return 0;
}

// 编译和运行:
// $ gcc -fsanitize=address -g buggy.c -o buggy
// $ ./buggy "This is a very long input string"
// ==12345==ERROR: AddressSanitizer: stack-buffer-overflow
// WRITE of size 32 at 0x7ff... thread T0
//     #0 strcpy buggy.c:6
//     #1 process_data buggy.c:6
//     #2 main buggy.c:12
```

**案例 2：检测数据竞争**

```c
// race.c - 存在数据竞争
#include <pthread.h>

int counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 10000; i++) {
        counter++;  // 数据竞争
    }
    return NULL;
}

int main() {
    pthread_t threads[4];
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Counter: %d\n", counter);  // 预期 40000，实际可能小于
    return 0;
}

// 编译和运行:
// $ gcc -fsanitize=thread -g race.c -o race -lpthread
// $ ./race
// WARNING: ThreadSanitizer: data race
//   Write of size 4 at 0x... by thread T1
```

## 8. 问题排查与参考资源

### 8.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| "ASan runtime not found" | 运行时库未链接 | 确保 `-fsanitize` 同时用于编译和链接 |
| "Too many memory mappings" | ASan 内存映射限制 | 减小 `mmap_limit_mb` 或增大系统限制 |
| TSan 误报 | 预期竞争 | 使用抑制文件或添加同步机制 |
| MSan 不工作 | GCC 限制 | 使用 Clang 编译器 |
| 性能过慢 | 多 Sanitizer 冲突 | 检查兼容性，分开测试 |

### 8.2 调试技巧

```bash
# 获取详细堆栈追踪
export ASAN_OPTIONS=symbolize=1:abort_on_error=1:detect_leaks=1

# 生成核心转储
ulimit -c unlimited
export ASAN_OPTIONS=disable_coredump=0:unmap_shadow_on_exit=1

# 使用 GDB 调试 Sanitizer 错误
gdb ./program
(gdb) run
# 程序崩溃后
(gdb) bt
```

### 8.3 参考资源

**官方文档：**

| 资源 | 链接 |
|------|------|
| AddressSanitizer | https://clang.llvm.org/docs/AddressSanitizer.html |
| MemorySanitizer | https://clang.llvm.org/docs/MemorySanitizer.html |
| ThreadSanitizer | https://clang.llvm.org/docs/ThreadSanitizer.html |
| UBSan | https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html |
| GCC Sanitizers | https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html |

**学习路径：**

1. 入门：ASan 基础使用
2. 进阶：UBSan 组合使用
3. 多线程：TSan 数据竞争检测
4. 高级：MSan 未初始化检测
5. 集成：CI/CD 流水线配置