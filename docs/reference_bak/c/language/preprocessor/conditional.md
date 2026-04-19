# C 有条件包含详解

  [1. 有条件包含基本概念](#1-有条件包含基本概念)
  
  [2. 条件包含指令详解](#2-条件包含指令详解)
  
  [3. 条件表达式详解](#3-条件表达式详解)
  
  [4. 条件包含块结构](#4-条件包含块结构)
  
  [5. 条件包含的应用场景](#5-条件包含的应用场景)
  
  [6. C23 新特性详解](#6-c23-新特性详解)
  
  [7. 高级用法与技巧](#7-高级用法与技巧)
  
  [8. 最佳实践](#8-最佳实践)
  
  [9. 标准参考](#9-标准参考)
  
  [10. 总结](#10-总结)

## 1. 有条件包含基本概念

### 1.1 什么是有条件包含

有条件包含（Conditional Inclusion）是 C 预处理器提供的一个重要功能，允许根据编译时的条件来决定是否编译源代码的特定部分。这个机制使得同一份源代码可以在不同的环境、平台或配置下编译出不同的结果。

有条件包含通过一组预处理指令来实现，这些指令在预处理阶段被评估，根据条件的真假来决定是否将相应的代码传递给编译器。

```c
#include <stdio.h>

// 根据是否定义了 DEBUG 宏来决定是否包含调试代码
#ifdef DEBUG
#define LOG_DEBUG(msg) printf("[DEBUG] %s\n", msg)
#else
#define LOG_DEBUG(msg) do {} while(0)  // 空操作
#endif

int main() {
    printf("Application started\n");
    LOG_DEBUG("Debug information");
    return 0;
}
```

### 1.2 有条件包含的作用

有条件包含的主要作用包括：

- 平台适配：根据不同操作系统或硬件平台编译不同的代码
- 版本控制：根据编译器版本或语言标准启用不同的特性
- 调试支持：在调试版本中包含额外的调试信息和检查
- 功能开关：根据配置启用或禁用特定功能
- 编译器兼容：处理不同编译器之间的差异
- 代码优化：根据编译选项启用不同的优化策略

### 1.3 条件包含指令概述

C 语言提供以下条件包含指令：

| 指令 | 功能 | 标准版本 |
|------|------|----------|
| `#if` | 根据常量表达式条件编译 | C89 |
| `#ifdef` | 如果定义了宏则编译 | C89 |
| `#ifndef` | 如果未定义宏则编译 | C89 |
| `#elif` | 条件编译的附加条件 | C89 |
| `#elifdef` | 如果定义了宏的附加条件 | C23 |
| `#elifndef` | 如果未定义宏的附加条件 | C23 |
| `#else` | 条件编译的替代分支 | C89 |
| `#endif` | 结束条件编译块 | C89 |

## 2. 条件包含指令详解

### 2.1 #if 指令

`#if` 指令根据常量表达式的值来决定是否编译代码块。表达式在预处理阶段被求值，如果结果非零则编译代码块。

```c
#include <stdio.h>

#define VERSION 2
#define DEBUG_LEVEL 3

int main() {
    printf("Application started\n");
    
    // 基本数值比较
    #if VERSION >= 2
    printf("Using version 2 features\n");
    #endif
    
    // 复杂表达式
    #if VERSION > 1 && DEBUG_LEVEL > 2
    printf("High debug level enabled\n");
    #endif
    
    // 逻辑运算
    #if !(VERSION < 1)
    printf("Version check passed\n");
    #endif
    
    return 0;
}
```

### 2.2 #ifdef 指令

`#ifdef` 指令检查指定的宏是否已定义，如果已定义则编译代码块。

```c
#include <stdio.h>

#define ENABLE_LOGGING
// #define ENABLE_DEBUG  // 未定义

int main() {
    printf("Application started\n");
    
    // 检查是否定义了日志功能
    #ifdef ENABLE_LOGGING
    printf("Logging is enabled\n");
    #endif
    
    // 检查是否定义了调试功能
    #ifdef ENABLE_DEBUG
    printf("Debug mode is enabled\n");
    #endif
    
    // 可以检查系统预定义宏
    #ifdef __GNUC__
    printf("Compiled with GCC\n");
    #endif
    
    #ifdef _MSC_VER
    printf("Compiled with MSVC\n");
    #endif
    
    return 0;
}
```

### 2.3 #ifndef 指令

`#ifndef` 指令检查指定的宏是否未定义，如果未定义则编译代码块。常用于头文件保护。

```c
#include <stdio.h>

// 模拟未定义的宏
// #define CONFIG_LOADED  // 如果定义了这个宏

int main() {
    printf("Application started\n");
    
    // 检查配置是否未加载
    #ifndef CONFIG_LOADED
    printf("Loading default configuration\n");
    #define CONFIG_LOADED
    #endif
    
    // 头文件保护的经典用法
    #ifndef MY_HEADER_H
    #define MY_HEADER_H
    printf("Header guard activated\n");
    #endif
    
    return 0;
}
```

### 2.4 #elif 指令

`#elif` 指令提供附加的条件分支，类似于 if-else if 结构。

```c
#include <stdio.h>

#define PLATFORM 2
#define COMPILER 1

int main() {
    printf("Application started\n");
    
    // 多条件分支
    #if PLATFORM == 1
    printf("Running on Windows\n");
    #elif PLATFORM == 2
    printf("Running on Linux\n");
    #elif PLATFORM == 3
    printf("Running on macOS\n");
    #else
    printf("Running on unknown platform\n");
    #endif
    
    // 编译器特定代码
    #if COMPILER == 1
    printf("Using GCC-specific features\n");
    #elif COMPILER == 2
    printf("Using Clang-specific features\n");
    #elif COMPILER == 3
    printf("Using MSVC-specific features\n");
    #else
    printf("Using generic compiler\n");
    #endif
    
    return 0;
}
```

### 2.5 #elifdef 和 #elifndef 指令 (C23)

C23 标准新增了 `#elifdef` 和 `#elifndef` 指令，分别等价于 `#elif defined` 和 `#elif !defined`。

```c
#include <stdio.h>

#define FEATURE_A
// #define FEATURE_B

int main() {
    printf("Application started\n");
    
    // C23 新增指令
    #ifdef FEATURE_A
    printf("Feature A is enabled\n");
    #elifdef FEATURE_B  // 等价于 #elif defined(FEATURE_B)
    printf("Feature B is enabled\n");
    #elifndef FEATURE_C  // 等价于 #elif !defined(FEATURE_C)
    printf("Feature C is not enabled\n");
    #else
    printf("No features enabled\n");
    #endif
    
    return 0;
}
```

### 2.6 #else 指令

`#else` 指令提供条件编译的默认分支。

```c
#include <stdio.h>

#define ENABLE_ADVANCED_FEATURES

int main() {
    printf("Application started\n");
    
    // 基本的 if-else 结构
    #ifdef ENABLE_ADVANCED_FEATURES
    printf("Advanced features enabled\n");
    printf("Using enhanced algorithms\n");
    printf("Additional memory required\n");
    #else
    printf("Basic features only\n");
    printf("Using standard algorithms\n");
    printf("Minimal memory usage\n");
    #endif
    
    // 多重条件的 else 分支
    #if defined(__x86_64__)
    printf("Optimized for x86-64 architecture\n");
    #elif defined(__arm__)
    printf("Optimized for ARM architecture\n");
    #else
    printf("Using generic optimization\n");
    #endif
    
    return 0;
}
```

### 2.7 #endif 指令

`#endif` 指令结束条件编译块，是每个条件包含结构的必需部分。

```c
#include <stdio.h>

#define DEBUG 1
#define VERSION 2

int main() {
    printf("Application started\n");
    
    // 简单条件包含
    #ifdef DEBUG
    printf("Debug mode: detailed logging enabled\n");
    #endif  // DEBUG
    
    // 复杂条件包含
    #if VERSION >= 2
    printf("Version 2+ features available\n");
    #ifdef DEBUG
    printf("Debug: Version 2+ specific debug info\n");
    #endif  // DEBUG
    #else
    printf("Legacy version features\n");
    #endif  // VERSION >= 2
    
    // 多层嵌套
    #ifndef SKIP_FEATURES
    printf("Main features enabled\n");
    
    #ifndef SKIP_EXTRA_FEATURES
    printf("Extra features enabled\n");
    #endif  // SKIP_EXTRA_FEATURES
    
    #endif  // SKIP_FEATURES
    
    return 0;
}
```

## 3. 条件表达式详解

### 3.1 常量表达式

`#if` 和 `#elif` 指令使用常量表达式进行条件判断。这些表达式只能包含：

- 整数字面量
- 使用 `#define` 定义的宏
- 算术运算符 (+, -, *, /, %, <<, >>, &, |, ^, ~)
- 关系运算符 (==, !=, <, >, <=, >=)
- 逻辑运算符 (!, &&, ||)
- 三元运算符 (? :)
- `defined` 运算符

```c
#include <stdio.h>

#define MAX_BUFFER_SIZE 1024
#define MIN_BUFFER_SIZE 256
#define ENABLE_COMPRESSION 1
#define COMPRESSION_LEVEL 5

int main() {
    printf("Application started\n");
    
    // 基本数值比较
    #if MAX_BUFFER_SIZE > 512
    printf("Large buffer size: %d\n", MAX_BUFFER_SIZE);
    #endif
    
    // 算术表达式
    #if (MAX_BUFFER_SIZE / MIN_BUFFER_SIZE) == 4
    printf("Buffer size ratio is 4:1\n");
    #endif
    
    // 逻辑运算
    #if ENABLE_COMPRESSION && COMPRESSION_LEVEL >= 3
    printf("High compression enabled (level %d)\n", COMPRESSION_LEVEL);
    #endif
    
    // 复杂表达式
    #if (MAX_BUFFER_SIZE > 1000) || (ENABLE_COMPRESSION && COMPRESSION_LEVEL > 7)
    printf("Advanced configuration detected\n");
    #else
    printf("Standard configuration\n");
    #endif
    
    // 三元运算符
    #if MAX_BUFFER_SIZE > 2048
    #define BUFFER_TYPE "Huge"
    #elif MAX_BUFFER_SIZE > 512
    #define BUFFER_TYPE "Large"
    #else
    #define BUFFER_TYPE "Small"
    #endif
    
    printf("Buffer type: %s\n", BUFFER_TYPE);
    
    return 0;
}
```

### 3.2 defined 运算符

`defined` 运算符用于检查宏是否已定义，有两种形式：

- `defined identifier`
- `defined (identifier)`

```c
#include <stdio.h>

#define DEBUG 1
#define MAX_CONNECTIONS 100
// #define ENABLE_SSL  // 未定义

int main() {
    printf("Application started\n");
    
    // 检查宏是否定义
    #if defined(DEBUG)
    printf("Debug mode is enabled\n");
    #endif
    
    // 检查宏是否未定义
    #if !defined(ENABLE_SSL)
    printf("SSL is not enabled\n");
    #endif
    
    // 复杂的 defined 表达式
    #if defined(DEBUG) && defined(MAX_CONNECTIONS)
    printf("Debug mode with connection limit: %d\n", MAX_CONNECTIONS);
    #endif
    
    // 使用括号形式
    #if defined (ENABLE_SSL) || defined (DEBUG)
    printf("Either SSL or debug mode is enabled\n");
    #else
    printf("Neither SSL nor debug mode is enabled\n");
    #endif
    
    // 多重 defined 检查
    #if defined(DEBUG) && defined(MAX_CONNECTIONS) && !defined(ENABLE_SSL)
    printf("Debug configuration without SSL\n");
    #endif
    
    return 0;
}
```

### 3.3 宏展开与条件表达式

在条件表达式中，宏会被展开后再进行求值。

```c
#include <stdio.h>

#define VERSION_MAJOR 1
#define VERSION_MINOR 2
#define VERSION_PATCH 3

#define VERSION_STRING "1.2.3"
#define VERSION_NUMBER (VERSION_MAJOR * 10000 + VERSION_MINOR * 100 + VERSION_PATCH)

#define IS_STABLE_VERSION (VERSION_MAJOR >= 1 && VERSION_MINOR >= 0)

int main() {
    printf("Application started\n");
    
    // 宏展开示例
    #if VERSION_MAJOR >= 1
    printf("Major version check passed\n");
    #endif
    
    // 复杂宏展开
    #if VERSION_NUMBER >= 10203
    printf("Version number check passed: %d\n", VERSION_NUMBER);
    #endif
    
    // 布尔宏展开
    #if IS_STABLE_VERSION
    printf("Stable version detected\n");
    #endif
    
    // 字符串宏在条件表达式中的处理
    #if 0  // 字符串宏不能直接用于数值比较
    // #if VERSION_STRING == "1.2.3"  // 这会导致错误
    printf("This will never be compiled\n");
    #endif
    
    // 正确的字符串版本检查方式
    #if VERSION_MAJOR == 1 && VERSION_MINOR == 2 && VERSION_PATCH == 3
    printf("Version 1.2.3 confirmed\n");
    #endif
    
    return 0;
}
```

### 3.4 C23 新增运算符

C23 标准增加了对 `__has_include`、`__has_embed` 和 `__has_c_attribute` 的支持。

```c
#include <stdio.h>

int main() {
    printf("Application started\n");
    
    // C23: __has_include 检查头文件是否存在
    #if __has_include(<threads.h>)
    #include <threads.h>
    printf("C11 threads support available\n");
    #else
    printf("C11 threads support not available\n");
    #endif
    
    // 检查多个头文件
    #if __has_include(<stdatomic.h>)
    #include <stdatomic.h>
    printf("Atomic operations support available\n");
    #endif
    
    // 检查用户头文件
    #if __has_include("config.h")
    #include "config.h"
    printf("Custom configuration file found\n");
    #else
    printf("Using default configuration\n");
    #endif
    
    // C23: __has_c_attribute 检查属性支持
    #if __has_c_attribute(fallthrough)
    printf("Fallthrough attribute supported\n");
    #endif
    
    return 0;
}
```

## 4. 条件包含块结构

### 4.1 基本结构

条件包含块的基本结构遵循严格的语法要求：

```c
#include <stdio.h>

#define FEATURE_A
#define FEATURE_B
#define DEBUG 1

int main() {
    printf("Application started\n");
    
    // 最简单的结构
    #ifdef FEATURE_A
    printf("Feature A enabled\n");
    #endif
    
    // if-else 结构
    #ifdef DEBUG
    printf("Debug mode\n");
    #else
    printf("Release mode\n");
    #endif
    
    // if-elif-else 结构
    #ifdef FEATURE_A
    printf("Using Feature A\n");
    #elif defined(FEATURE_B)
    printf("Using Feature B\n");
    #else
    printf("Using default feature\n");
    #endif
    
    // 复杂的多重条件
    #if defined(FEATURE_A) && defined(FEATURE_B)
    printf("Both features A and B enabled\n");
    #elif defined(FEATURE_A)
    printf("Only feature A enabled\n");
    #elif defined(FEATURE_B)
    printf("Only feature B enabled\n");
    #else
    printf("No special features enabled\n");
    #endif
    
    return 0;
}
```

### 4.2 嵌套条件包含

条件包含块可以嵌套使用，形成复杂的条件逻辑：

```c
#include <stdio.h>

#define PLATFORM_WINDOWS
#define DEBUG 1
#define ENABLE_LOGGING

int main() {
    printf("Application started\n");
    
    // 第一层条件包含
    #ifdef PLATFORM_WINDOWS
    printf("Windows platform detected\n");
    
    // 嵌套的第二层条件包含
    #ifdef DEBUG
    printf("Windows debug build\n");
    
    // 嵌套的第三层条件包含
    #ifdef ENABLE_LOGGING
    printf("Windows debug build with logging\n");
    #else
    printf("Windows debug build without logging\n");
    #endif  // ENABLE_LOGGING
    
    #else  // not DEBUG
    printf("Windows release build\n");
    #endif  // DEBUG
    
    #else  // not PLATFORM_WINDOWS
    printf("Non-Windows platform\n");
    
    #ifdef DEBUG
    printf("Non-Windows debug build\n");
    #else
    printf("Non-Windows release build\n");
    #endif  // DEBUG
    
    #endif  // PLATFORM_WINDOWS
    
    return 0;
}
```

### 4.3 条件包含的处理流程

预处理器按顺序处理条件包含指令：

```c
#include <stdio.h>

#define VERSION 2
#define DEBUG 1
#define EXPERIMENTAL_FEATURE

int main() {
    printf("Application started\n");
    
    // 处理流程演示
    #if VERSION >= 3
    printf("Version 3+ code path\n");
    // 这段代码被跳过，后续的 elif 也会被处理
    #elif VERSION >= 2
    printf("Version 2 code path\n");
    // 这段代码被包含，后续的 elif 和 else 被跳过
    #elif VERSION >= 1
    printf("Version 1 code path\n");
    // 这段代码被跳过
    #else
    printf("Unknown version code path\n");
    // 这段代码被跳过
    #endif
    
    // 另一个处理流程
    #ifdef DEBUG
    printf("Debug information\n");
    
    #ifdef EXPERIMENTAL_FEATURE
    printf("Experimental feature debug info\n");
    #endif
    
    #else  // not DEBUG
    printf("Release mode\n");
    #endif
    
    return 0;
}
```

## 5. 条件包含的应用场景

### 5.1 平台相关代码

根据不同操作系统编译不同的代码：

```c
#include <stdio.h>

int main() {
    printf("Application started\n");
    
    // Windows 平台特定代码
    #ifdef _WIN32
    #include <windows.h>
    printf("Running on Windows\n");
    Sleep(1000);  // Windows 特定函数
    
    // Linux 平台特定代码
    #elif defined(__linux__)
    #include <unistd.h>
    printf("Running on Linux\n");
    sleep(1);     // Linux 特定函数
    
    // macOS 平台特定代码
    #elif defined(__APPLE__)
    #include <unistd.h>
    printf("Running on macOS\n");
    sleep(1);     // macOS 特定函数
    
    // 其他平台
    #else
    printf("Running on unknown platform\n");
    #endif
    
    return 0;
}
```

### 5.2 版本控制

根据编译器版本或 C 标准版本启用不同特性：

```c
#include <stdio.h>

int main() {
    printf("Application started\n");
    
    // C 标准版本检查
    #if __STDC_VERSION__ >= 202311L
    printf("C23 or later\n");
    #elif __STDC_VERSION__ >= 201112L
    printf("C11 or later\n");
    #elif __STDC_VERSION__ >= 199901L
    printf("C99 or later\n");
    #else
    printf("C89/C90\n");
    #endif
    
    // 编译器版本检查
    #ifdef __GNUC__
    #if __GNUC__ >= 10
    printf("GCC 10 or later\n");
    #elif __GNUC__ >= 7
    printf("GCC 7-9\n");
    #else
    printf("Old GCC version\n");
    #endif
    #endif
    
    // 编译器特定特性
    #ifdef __clang__
    printf("Using Clang compiler\n");
    #elif defined(_MSC_VER)
    printf("Using MSVC compiler\n");
    #elif defined(__GNUC__)
    printf("Using GCC compiler\n");
    #endif
    
    return 0;
}
```

### 5.3 调试与发布版本

根据构建配置包含不同的代码：

```c
#include <stdio.h>
#include <assert.h>

// 通常在构建系统中定义 DEBUG
// #define DEBUG 1

#ifdef DEBUG
#define LOG_DEBUG(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define ASSERT(condition) assert(condition)
#else
#define LOG_DEBUG(fmt, ...) do {} while(0)
#define ASSERT(condition) do {} while(0)
#endif

int main() {
    printf("Application started\n");
    
    int x = 5;
    LOG_DEBUG("Variable x initialized to %d", x);
    
    ASSERT(x > 0);
    
    #ifdef DEBUG
    printf("Debug build: additional checks enabled\n");
    printf("Debug build: verbose logging enabled\n");
    printf("Debug build: assertions enabled\n");
    #else
    printf("Release build: optimized for performance\n");
    printf("Release build: minimal logging\n");
    printf("Release build: assertions disabled\n");
    #endif
    
    return 0;
}
```

### 5.4 功能开关

通过配置宏启用或禁用特定功能：

```c
#include <stdio.h>

// 功能开关配置
#define ENABLE_NETWORKING 1
#define ENABLE_DATABASE 0
#define ENABLE_ENCRYPTION 1
#define ENABLE_LOGGING 1

int main() {
    printf("Application started\n");
    
    // 网络功能
    #if ENABLE_NETWORKING
    printf("Networking enabled\n");
    // 网络相关代码
    #include <sys/socket.h>
    // 网络初始化代码
    #else
    printf("Networking disabled\n");
    #endif
    
    // 数据库功能
    #if ENABLE_DATABASE
    printf("Database support enabled\n");
    // 数据库相关代码
    #include <sqlite3.h>
    // 数据库初始化代码
    #else
    printf("Database support disabled\n");
    #endif
    
    // 加密功能
    #if ENABLE_ENCRYPTION
    printf("Encryption enabled\n");
    // 加密相关代码
    #else
    printf("Encryption disabled\n");
    #endif
    
    // 日志功能
    #if ENABLE_LOGGING
    printf("Logging enabled\n");
    // 日志相关代码
    #else
    printf("Logging disabled\n");
    #endif
    
    return 0;
}
```

### 5.5 编译器特定代码

处理不同编译器之间的差异：

```c
#include <stdio.h>

int main() {
    printf("Application started\n");
    
    // GCC 特定代码
    #ifdef __GNUC__
    printf("Compiled with GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    
    #if __GNUC__ >= 7
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #endif
    
    #if __GNUC__ >= 7
    #pragma GCC diagnostic pop
    #endif
    #endif
    
    // Clang 特定代码
    #ifdef __clang__
    printf("Compiled with Clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
    
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-variable"
    #pragma clang diagnostic pop
    #endif
    
    // MSVC 特定代码
    #ifdef _MSC_VER
    printf("Compiled with MSVC %d\n", _MSC_VER);
    
    #pragma warning(push)
    #pragma warning(disable: 4996)
    #pragma warning(pop)
    #endif
    
    // 通用代码
    printf("Common code executed on all compilers\n");
    
    return 0;
}
```

## 6. C23 新特性详解

### 6.1 #elifdef 指令

`#elifdef` 指令等价于 `#elif defined(identifier)`，提供更简洁的语法：

```c
#include <stdio.h>

#define FEATURE_A
// #define FEATURE_B
// #define FEATURE_C

int main() {
    printf("Application started\n");
    
    // 传统写法
    #ifdef FEATURE_A
    printf("Feature A enabled (traditional)\n");
    #elif defined(FEATURE_B)
    printf("Feature B enabled (traditional)\n");
    #elif defined(FEATURE_C)
    printf("Feature C enabled (traditional)\n");
    #else
    printf("No features enabled (traditional)\n");
    #endif
    
    // C23 新写法
    #ifdef FEATURE_A
    printf("Feature A enabled (C23)\n");
    #elifdef FEATURE_B
    printf("Feature B enabled (C23)\n");
    #elifdef FEATURE_C
    printf("Feature C enabled (C23)\n");
    #else
    printf("No features enabled (C23)\n");
    #endif
    
    return 0;
}
```

### 6.2 #elifndef 指令

`#elifndef` 指令等价于 `#elif !defined(identifier)`，用于检查宏未定义：

```c
#include <stdio.h>

#define CONFIG_FILE_LOADED
// #define DATABASE_CONFIGURED
// #define NETWORK_INITIALIZED

int main() {
    printf("Application started\n");
    
    // 传统写法
    #ifdef CONFIG_FILE_LOADED
    printf("Config file loaded (traditional)\n");
    #elif !defined(DATABASE_CONFIGURED)
    printf("Database not configured (traditional)\n");
    #elif !defined(NETWORK_INITIALIZED)
    printf("Network not initialized (traditional)\n");
    #else
    printf("All systems go (traditional)\n");
    #endif
    
    // C23 新写法
    #ifdef CONFIG_FILE_LOADED
    printf("Config file loaded (C23)\n");
    #elifndef DATABASE_CONFIGURED
    printf("Database not configured (C23)\n");
    #elifndef NETWORK_INITIALIZED
    printf("Network not initialized (C23)\n");
    #else
    printf("All systems go (C23)\n");
    #endif
    
    return 0;
}
```

### 6.3 新增运算符支持

C23 增强了对 `__has_include`、`__has_embed` 和 `__has_c_attribute` 的支持：

```c
#include <stdio.h>

int main() {
    printf("Application started\n");
    
    // __has_include 支持
    #if __has_include(<stdatomic.h>)
    #include <stdatomic.h>
    printf("Atomic operations available\n");
    #else
    printf("Atomic operations not available\n");
    #endif
    
    // __has_c_attribute 支持
    #if __has_c_attribute(noreturn)
    [[noreturn]] void fatal_error(void) {
        printf("Fatal error occurred\n");
        exit(1);
    }
    printf("Noreturn attribute supported\n");
    #endif
    
    // __has_embed 支持 (示例)
    #if __has_embed("resource.txt")
    // 嵌入资源文件
    printf("Resource embedding supported\n");
    #endif
    
    return 0;
}
```

## 7. 高级用法与技巧

### 7.1 复杂条件组合

组合多个条件创建复杂的编译逻辑：

```c
#include <stdio.h>

#define PLATFORM_WINDOWS
#define COMPILER_GCC
#define VERSION_MAJOR 2
#define VERSION_MINOR 1
#define ENABLE_DEBUG 1
#define ENABLE_PROFILING 0

int main() {
    printf("Application started\n");
    
    // 复杂的平台和编译器组合
    #if (defined(PLATFORM_WINDOWS) && defined(COMPILER_GCC)) || \
        (defined(PLATFORM_LINUX) && defined(COMPILER_CLANG))
    printf("Supported platform-compiler combination\n");
    #else
    printf("Unsupported platform-compiler combination\n");
    #endif
    
    // 版本和功能组合
    #if VERSION_MAJOR >= 2 && (VERSION_MINOR >= 1 || defined(ENABLE_DEBUG))
    printf("Advanced features available\n");
    
    #if ENABLE_DEBUG && !ENABLE_PROFILING
    printf("Debug mode without profiling\n");
    #elif ENABLE_PROFILING && !ENABLE_DEBUG
    printf("Profiling mode without debug\n");
    #elif ENABLE_DEBUG && ENABLE_PROFILING
    printf("Debug and profiling mode\n");
    #else
    printf("Release mode\n");
    #endif
    
    #else
    printf("Basic features only\n");
    #endif
    
    return 0;
}
```

### 7.2 条件包含与宏定义

动态定义宏以控制后续代码：

```c
#include <stdio.h>

#define TARGET_PLATFORM 2  // 1=Windows, 2=Linux, 3=macOS

int main() {
    printf("Application started\n");
    
    // 根据平台定义不同的宏
    #if TARGET_PLATFORM == 1
    #define PLATFORM_NAME "Windows"
    #define PATH_SEPARATOR '\\'
    #define HAS_WIDE_CHARACTERS 1
    #elif TARGET_PLATFORM == 2
    #define PLATFORM_NAME "Linux"
    #define PATH_SEPARATOR '/'
    #define HAS_WIDE_CHARACTERS 0
    #elif TARGET_PLATFORM == 3
    #define PLATFORM_NAME "macOS"
    #define PATH_SEPARATOR '/'
    #define HAS_WIDE_CHARACTERS 1
    #else
    #define PLATFORM_NAME "Unknown"
    #define PATH_SEPARATOR '/'
    #define HAS_WIDE_CHARACTERS 0
    #endif
    
    printf("Platform: %s\n", PLATFORM_NAME);
    printf("Path separator: %c\n", PATH_SEPARATOR);
    
    #if HAS_WIDE_CHARACTERS
    printf("Wide character support enabled\n");
    #else
    printf("Wide character support disabled\n");
    #endif
    
    return 0;
}
```

### 7.3 条件包含性能优化

使用条件包含优化编译性能：

```c
#include <stdio.h>

// 配置选项
#define OPTIMIZATION_LEVEL 3
#define ENABLE_ASSERTIONS 0
#define INLINE_FUNCTIONS 1

// 性能相关的宏定义
#if OPTIMIZATION_LEVEL >= 3
#define HOT_FUNCTION __attribute__((hot))
#define COLD_FUNCTION __attribute__((cold))
#elif OPTIMIZATION_LEVEL >= 2
#define HOT_FUNCTION
#define COLD_FUNCTION __attribute__((cold))
#else
#define HOT_FUNCTION
#define COLD_FUNCTION
#endif

#if ENABLE_ASSERTIONS
#define DEBUG_ASSERT(condition) assert(condition)
#else
#define DEBUG_ASSERT(condition) do {} while(0)
#endif

#if INLINE_FUNCTIONS
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE
#endif

HOT_FUNCTION
int frequently_called_function(int x) {
    DEBUG_ASSERT(x > 0);
    return x * 2;
}

COLD_FUNCTION
void rarely_called_function(void) {
    printf("Rarely called function\n");
}

FORCE_INLINE
int inline_function(int x) {
    return x + 1;
}

int main() {
    printf("Application started\n");
    printf("Result: %d\n", frequently_called_function(5));
    rarely_called_function();
    printf("Inline result: %d\n", inline_function(10));
    return 0;
}
```

### 7.4 条件包含调试技巧

使用条件包含进行调试和诊断：

```c
#include <stdio.h>

#define DEBUG_LEVEL 2
#define TRACE_FUNCTIONS 1
#define DUMP_VARIABLES 0

// 调试宏
#if DEBUG_LEVEL >= 3
#define DEBUG_TRACE(fmt, ...) fprintf(stderr, "[TRACE] " fmt "\n", ##__VA_ARGS__)
#elif DEBUG_LEVEL >= 2
#define DEBUG_TRACE(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#elif DEBUG_LEVEL >= 1
#define DEBUG_TRACE(fmt, ...) fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG_TRACE(fmt, ...) do {} while(0)
#endif

// 函数跟踪宏
#if TRACE_FUNCTIONS
#define FUNCTION_ENTER(name) do { \
    DEBUG_TRACE("Entering function: %s", name); \
} while(0)
#define FUNCTION_EXIT(name) do { \
    DEBUG_TRACE("Exiting function: %s", name); \
} while(0)
#else
#define FUNCTION_ENTER(name) do {} while(0)
#define FUNCTION_EXIT(name) do {} while(0)
#endif

// 变量转储宏
#if DUMP_VARIABLES
#define DUMP_VAR(name, value) do { \
    DEBUG_TRACE("Variable %s = %d", #name, (int)(value)); \
} while(0)
#else
#define DUMP_VAR(name, value) do {} while(0)
#endif

int calculate_result(int a, int b) {
    FUNCTION_ENTER("calculate_result");
    DUMP_VAR(a, a);
    DUMP_VAR(b, b);
    
    int result = a + b;
    DUMP_VAR(result, result);
    
    FUNCTION_EXIT("calculate_result");
    return result;
}

int main() {
    DEBUG_TRACE("Application started");
    
    int x = 10, y = 20;
    int result = calculate_result(x, y);
    
    DEBUG_TRACE("Final result: %d", result);
    DEBUG_TRACE("Application finished");
    
    return 0;
}
```

## 8. 最佳实践

### 8.1 条件包含编写规范

遵循规范的条件包含编写方式：

```c
#include <stdio.h>

// 配置宏定义（通常在头文件或构建系统中）
#define ENABLE_FEATURE_X 1
#define ENABLE_FEATURE_Y 0
#define MAX_BUFFER_SIZE 4096
#define DEBUG_BUILD 1

// 宏定义的组织方式
// 1. 系统检测宏
#if defined(_WIN32)
    #define PLATFORM_WINDOWS 1
#elif defined(__linux__)
    #define PLATFORM_LINUX 1
#elif defined(__APPLE__)
    #define PLATFORM_MACOS 1
#endif

// 2. 编译器检测宏
#if defined(__GNUC__)
    #define COMPILER_GCC 1
#elif defined(__clang__)
    #define COMPILER_CLANG 1
#elif defined(_MSC_VER)
    #define COMPILER_MSVC 1
#endif

// 3. 功能检测宏
#if __STDC_VERSION__ >= 201112L
    #define HAS_C11_FEATURES 1
#endif

int main() {
    printf("Application started\n");
    
    // 条件包含的规范写法
    #if ENABLE_FEATURE_X
    printf("Feature X is enabled\n");
    #endif
    
    // 多条件的规范写法
    #if defined(PLATFORM_WINDOWS) && defined(COMPILER_GCC)
    printf("Windows GCC build\n");
    #elif defined(PLATFORM_LINUX) && defined(COMPILER_GCC)
    printf("Linux GCC build\n");
    #elif defined(PLATFORM_MACOS) && defined(COMPILER_CLANG)
    printf("macOS Clang build\n");
    #else
    printf("Other platform/compiler\n");
    #endif
    
    // 版本检查的规范写法
    #if MAX_BUFFER_SIZE >= 4096
    printf("Large buffer size: %d\n", MAX_BUFFER_SIZE);
    #endif
    
    // 调试代码的规范写法
    #if DEBUG_BUILD
    printf("Debug build information\n");
    #endif
    
    return 0;
}
```

### 8.2 条件包含设计模式

使用设计模式优化条件包含的使用：

```c
#include <stdio.h>

// 1. 特性检测模式
#define HAS_THREADS_FEATURE __STDC_VERSION__ >= 201112L && defined(__STDC_NO_THREADS__) == 0

// 2. 平台抽象模式
#if defined(_WIN32)
    #define PLATFORM_API "Windows"
    #define SLEEP_FUNC(ms) Sleep(ms)
#elif defined(__unix__)
    #define PLATFORM_API "Unix"
    #define SLEEP_FUNC(ms) usleep((ms) * 1000)
#else
    #define PLATFORM_API "Generic"
    #define SLEEP_FUNC(ms) (void)(ms)
#endif

// 3. 配置模式
typedef enum {
    CONFIG_MODE_DEBUG,
    CONFIG_MODE_RELEASE,
    CONFIG_MODE_PROFILE
} config_mode_t;

#if defined(DEBUG)
    #define CURRENT_CONFIG_MODE CONFIG_MODE_DEBUG
#elif defined(PROFILE)
    #define CURRENT_CONFIG_MODE CONFIG_MODE_PROFILE
#else
    #define CURRENT_CONFIG_MODE CONFIG_MODE_RELEASE
#endif

// 4. 功能开关模式
#define FEATURE_NETWORK (1)
#define FEATURE_DATABASE (0)
#define FEATURE_ENCRYPTION (1)

#define REQUIRED_FEATURES (FEATURE_NETWORK && FEATURE_ENCRYPTION)
#define OPTIONAL_FEATURES (FEATURE_DATABASE)

int main() {
    printf("Application started\n");
    printf("Platform API: %s\n", PLATFORM_API);
    printf("Config mode: %d\n", CURRENT_CONFIG_MODE);
    printf("Required features: %s\n", REQUIRED_FEATURES ? "Available" : "Missing");
    printf("Optional features: %s\n", OPTIONAL_FEATURES ? "Available" : "Missing");
    
    return 0;
}
```

### 8.3 条件包含维护策略

制定有效的条件包含维护策略：

```c
#include <stdio.h>

// 1. 版本兼容性维护
#if __STDC_VERSION__ >= 202311L
    // C23 特性
    #define MODERN_SYNTAX_AVAILABLE 1
#elif __STDC_VERSION__ >= 201112L
    // C11 特性
    #define MODERN_SYNTAX_AVAILABLE 0
#else
    // C99/C89 特性
    #define MODERN_SYNTAX_AVAILABLE 0
#endif

// 2. 向后兼容性维护
#if defined(__GNUC__) && (__GNUC__ >= 4)
    #define GCC_MODERN_FEATURES 1
#else
    #define GCC_MODERN_FEATURES 0
#endif

// 3. 弃用警告维护
#if defined(DEPRECATED_FEATURE) && !defined(SUPPRESS_DEPRECATED_WARNINGS)
    #warning "DEPRECATED_FEATURE is deprecated and will be removed in future versions"
#endif

// 4. 配置验证维护
#if defined(ENABLE_FEATURE_A) && defined(ENABLE_FEATURE_B)
    #if ENABLE_FEATURE_A && !ENABLE_FEATURE_B
    #error "Feature A requires Feature B to be enabled"
    #endif
#endif

int main() {
    printf("Application started\n");
    
    #if MODERN_SYNTAX_AVAILABLE
    printf("Using modern C syntax\n");
    #else
    printf("Using legacy C syntax\n");
    #endif
    
    #if GCC_MODERN_FEATURES
    printf("GCC modern features available\n");
    #endif
    
    return 0;
}
```

### 8.4 常见错误与避免

识别和避免条件包含的常见错误：

```c
#include <stdio.h>

// 错误1: 宏定义中的语法错误
// #define MAX(a,b) (a > b) ? a : b  // 错误：缺少括号
#define MAX(a,b) ((a) > (b) ? (a) : (b))  // 正确

// 错误2: 未匹配的条件包含指令
/*
#if defined(DEBUG)
    printf("Debug mode\n");
// 缺少 #endif
*/

// 错误3: 嵌套条件包含的逻辑错误
/*
#ifdef FEATURE_A
    #ifdef FEATURE_B
        printf("Both features\n");
    #else
        printf("Only feature A\n");
    #endif
#else
    #ifdef FEATURE_B
        printf("Only feature B\n");
    #else
        printf("No features\n");
    #endif
#endif
*/

// 错误4: 宏展开导致的意外结果
#define MULTIPLY(a,b) a * b
// printf("%d\n", MULTIPLY(2 + 3, 4 + 5));  // 结果是 2+3*4+5 = 19，不是期望的 5*9 = 45

int main() {
    printf("Application started\n");
    
    // 正确的条件包含使用
    #ifdef DEBUG
    printf("Debug mode enabled\n");
    #endif
    
    // 正确的嵌套条件包含
    #if defined(FEATURE_A) || defined(FEATURE_B)
    printf("At least one feature enabled\n");
    
    #if defined(FEATURE_A) && defined(FEATURE_B)
    printf("Both features enabled\n");
    #elif defined(FEATURE_A)
    printf("Only feature A enabled\n");
    #else
    printf("Only feature B enabled\n");
    #endif
    
    #else
    printf("No features enabled\n");
    #endif
    
    // 正确的宏使用
    printf("Max of 5 and 3: %d\n", MAX(5, 3));
    printf("Multiply (2+3)*(4+5): %d\n", MULTIPLY((2 + 3), (4 + 5)));
    
    return 0;
}
```

## 9. 标准参考

### 9.1 C 标准版本

有条件包含相关标准条款：

- C23 标准 (ISO/IEC 9899:2024): 6.10.1 有条件包含
- C17 标准 (ISO/IEC 9899:2018): 6.10.1 有条件包含 (第118-119页)
- C11 标准 (ISO/IEC 9899:2011): 6.10.1 有条件包含 (第162-164页)
- C99 标准 (ISO/IEC 9899:1999): 6.10.1 有条件包含 (第147-149页)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.8.1 有条件包含

### 9.2 相关标准条款

- 5.1.1.2 翻译阶段
- 6.10 预处理指令
- 6.10.1 有条件包含
- 6.10.2 源文件包含
- 6.10.3 宏替换
- 6.10.8 预定义宏名

### 9.3 缺陷报告

相关缺陷报告：

**DR 412 (C89)**：

- 已发布的行为：失败的 `#elif` 表达式必须有效
- 正确的行为：失败的 `#elif` 被跳过

## 10. 总结

### 10.1 核心要点

有条件包含是 C 预处理器的核心功能之一，提供了以下关键能力：

- 条件编译：根据编译时条件决定代码是否被编译
- 平台适配：支持不同操作系统和硬件平台的代码编译
- 版本控制：根据编译器版本和 C 标准版本启用不同特性
- 调试支持：在调试和发布版本中包含不同的代码
- 功能开关：动态启用或禁用特定功能
- 编译器兼容：处理不同编译器之间的差异

### 10.2 使用建议

- 合理使用条件包含：避免过度复杂的条件逻辑
- 保持代码可读性：使用清晰的缩进和注释
- 测试所有路径：确保所有条件分支都能正确编译
- 文档化配置选项：清楚地记录所有配置宏的含义
- 避免副作用：条件包含不应影响程序的逻辑结构
- 考虑维护成本：平衡灵活性和代码复杂度

### 10.3 发展趋势

有条件包含在 C 语言的发展中持续演进：

- C89/C90：建立了基本的条件包含框架
- C99：增强了宏定义和条件包含的能力
- C11：增加了对新特性的支持
- C17：保持了 C11 的条件包含特性
- C23：新增了 `#elifdef` 和 `#elifndef` 指令，增强了 `__has_include` 等运算符

随着 C 语言的不断发展，有条件包含功能变得更加强大和易用，为开发者提供了更好的条件编译支持。
