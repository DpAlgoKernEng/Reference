# C++ 有条件包含详解

  [1. 有条件包含基本概念](#1-有条件包含基本概念)
  
  [2. 条件包含指令详解](#2-条件包含指令详解)
  
  [3. 条件表达式详解](#3-条件表达式详解)
  
  [4. 条件包含块结构](#4-条件包含块结构)
  
  [5. 条件包含的应用场景](#5-条件包含的应用场景)
  
  [6. C++23 新特性详解](#6-c23-新特性详解)
  
  [7. 高级用法与技巧](#7-高级用法与技巧)
  
  [8. 最佳实践](#8-最佳实践)
  
  [9. 标准参考](#9-标准参考)
  
  [10. 总结](#10-总结)

## 1. 有条件包含基本概念

### 1.1 什么是有条件包含

有条件包含（Conditional Inclusion）是 C++ 预处理器提供的一个重要功能，允许根据编译时的条件来决定是否编译源代码的特定部分。这个机制使得同一份源代码可以在不同的环境、平台或配置下编译出不同的结果。

有条件包含通过一组预处理指令来实现，这些指令在预处理阶段被评估，根据条件的真假来决定是否将相应的代码传递给编译器。

```cpp
#include <iostream>

// 根据是否定义了 DEBUG 宏来决定是否包含调试代码
#ifdef DEBUG
#define LOG_DEBUG(msg) std::cerr << "[DEBUG] " << msg << std::endl
#else
#define LOG_DEBUG(msg) do {} while(0)  // 空操作
#endif

int main() {
    std::cout << "Application started\n";
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
- C++ 标准特性检测：检测和使用特定 C++ 标准的新特性

### 1.3 条件包含指令概述

C++ 语言提供以下条件包含指令：

| 指令 | 功能 | 标准版本 |
|------|------|----------|
| `#if` | 根据常量表达式条件编译 | C++98 |
| `#ifdef` | 如果定义了宏则编译 | C++98 |
| `#ifndef` | 如果未定义宏则编译 | C++98 |
| `#elif` | 条件编译的附加条件 | C++98 |
| `#elifdef` | 如果定义了宏的附加条件 | C++23 |
| `#elifndef` | 如果未定义宏的附加条件 | C++23 |
| `#else` | 条件编译的替代分支 | C++98 |
| `#endif` | 结束条件编译块 | C++98 |

## 2. 条件包含指令详解

### 2.1 #if 指令

`#if` 指令根据常量表达式的值来决定是否编译代码块。表达式在预处理阶段被求值，如果结果非零则编译代码块。

```cpp
#include <iostream>

#define VERSION 2
#define DEBUG_LEVEL 3

int main() {
    std::cout << "Application started\n";
    
    // 基本数值比较
    #if VERSION >= 2
    std::cout << "Using version 2 features\n";
    #endif
    
    // 复杂表达式
    #if VERSION > 1 && DEBUG_LEVEL > 2
    std::cout << "High debug level enabled\n";
    #endif
    
    // 逻辑运算
    #if !(VERSION < 1)
    std::cout << "Version check passed\n";
    #endif
    
    return 0;
}
```

### 2.2 #ifdef 指令

`#ifdef` 指令检查指定的宏是否已定义，如果已定义则编译代码块。

```cpp
#include <iostream>

#define ENABLE_LOGGING
// #define ENABLE_DEBUG  // 未定义

int main() {
    std::cout << "Application started\n";
    
    // 检查是否定义了日志功能
    #ifdef ENABLE_LOGGING
    std::cout << "Logging is enabled\n";
    #endif
    
    // 检查是否定义了调试功能
    #ifdef ENABLE_DEBUG
    std::cout << "Debug mode is enabled\n";
    #endif
    
    // 可以检查系统预定义宏
    #ifdef __GNUC__
    std::cout << "Compiled with GCC\n";
    #endif
    
    #ifdef _MSC_VER
    std::cout << "Compiled with MSVC\n";
    #endif
    
    return 0;
}
```

### 2.3 #ifndef 指令

`#ifndef` 指令检查指定的宏是否未定义，如果未定义则编译代码块。常用于头文件保护。

```cpp
#include <iostream>

// 模拟未定义的宏
// #define CONFIG_LOADED  // 如果定义了这个宏

int main() {
    std::cout << "Application started\n";
    
    // 检查配置是否未加载
    #ifndef CONFIG_LOADED
    std::cout << "Loading default configuration\n";
    #define CONFIG_LOADED
    #endif
    
    // 头文件保护的经典用法
    #ifndef MY_HEADER_H
    #define MY_HEADER_H
    std::cout << "Header guard activated\n";
    #endif
    
    return 0;
}
```

### 2.4 #elif 指令

`#elif` 指令提供附加的条件分支，类似于 if-else if 结构。

```cpp
#include <iostream>

#define PLATFORM 2
#define COMPILER 1

int main() {
    std::cout << "Application started\n";
    
    // 多条件分支
    #if PLATFORM == 1
    std::cout << "Running on Windows\n";
    #elif PLATFORM == 2
    std::cout << "Running on Linux\n";
    #elif PLATFORM == 3
    std::cout << "Running on macOS\n";
    #else
    std::cout << "Running on unknown platform\n";
    #endif
    
    // 编译器特定代码
    #if COMPILER == 1
    std::cout << "Using GCC-specific features\n";
    #elif COMPILER == 2
    std::cout << "Using Clang-specific features\n";
    #elif COMPILER == 3
    std::cout << "Using MSVC-specific features\n";
    #else
    std::cout << "Using generic compiler\n";
    #endif
    
    return 0;
}
```

### 2.5 #elifdef 和 #elifndef 指令 (C++23)

C++23 标准新增了 `#elifdef` 和 `#elifndef` 指令，分别等价于 `#elif defined` 和 `#elif !defined`。

```cpp
#include <iostream>

#define FEATURE_A
// #define FEATURE_B

int main() {
    std::cout << "Application started\n";
    
    // C++23 新增指令
    #ifdef FEATURE_A
    std::cout << "Feature A is enabled\n";
    #elifdef FEATURE_B  // 等价于 #elif defined(FEATURE_B)
    std::cout << "Feature B is enabled\n";
    #elifndef FEATURE_C  // 等价于 #elif !defined(FEATURE_C)
    std::cout << "Feature C is not enabled\n";
    #else
    std::cout << "No features enabled\n";
    #endif
    
    return 0;
}
```

### 2.6 #else 指令

`#else` 指令提供条件编译的默认分支。

```cpp
#include <iostream>

#define ENABLE_ADVANCED_FEATURES

int main() {
    std::cout << "Application started\n";
    
    // 基本的 if-else 结构
    #ifdef ENABLE_ADVANCED_FEATURES
    std::cout << "Advanced features enabled\n";
    std::cout << "Using enhanced algorithms\n";
    std::cout << "Additional memory required\n";
    #else
    std::cout << "Basic features only\n";
    std::cout << "Using standard algorithms\n";
    std::cout << "Minimal memory usage\n";
    #endif
    
    // 多重条件的 else 分支
    #if defined(__x86_64__)
    std::cout << "Optimized for x86-64 architecture\n";
    #elif defined(__arm__)
    std::cout << "Optimized for ARM architecture\n";
    #else
    std::cout << "Using generic optimization\n";
    #endif
    
    return 0;
}
```

### 2.7 #endif 指令

`#endif` 指令结束条件编译块，是每个条件包含结构的必需部分。

```cpp
#include <iostream>

#define DEBUG 1
#define VERSION 2

int main() {
    std::cout << "Application started\n";
    
    // 简单条件包含
    #ifdef DEBUG
    std::cout << "Debug mode: detailed logging enabled\n";
    #endif  // DEBUG
    
    // 复杂条件包含
    #if VERSION >= 2
    std::cout << "Version 2+ features available\n";
    #ifdef DEBUG
    std::cout << "Debug: Version 2+ specific debug info\n";
    #endif  // DEBUG
    #else
    std::cout << "Legacy version features\n";
    #endif  // VERSION >= 2
    
    // 多层嵌套
    #ifndef SKIP_FEATURES
    std::cout << "Main features enabled\n";
    
    #ifndef SKIP_EXTRA_FEATURES
    std::cout << "Extra features enabled\n";
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
- C++20 起的 `__has_include`、`__has_cpp_attribute`
- C++26 起的 `__has_embed`

```cpp
#include <iostream>

#define MAX_BUFFER_SIZE 1024
#define MIN_BUFFER_SIZE 256
#define ENABLE_COMPRESSION 1
#define COMPRESSION_LEVEL 5

int main() {
    std::cout << "Application started\n";
    
    // 基本数值比较
    #if MAX_BUFFER_SIZE > 512
    std::cout << "Large buffer size: " << MAX_BUFFER_SIZE << "\n";
    #endif
    
    // 算术表达式
    #if (MAX_BUFFER_SIZE / MIN_BUFFER_SIZE) == 4
    std::cout << "Buffer size ratio is 4:1\n";
    #endif
    
    // 逻辑运算
    #if ENABLE_COMPRESSION && COMPRESSION_LEVEL >= 3
    std::cout << "High compression enabled (level " << COMPRESSION_LEVEL << ")\n";
    #endif
    
    // 复杂表达式
    #if (MAX_BUFFER_SIZE > 1000) || (ENABLE_COMPRESSION && COMPRESSION_LEVEL > 7)
    std::cout << "Advanced configuration detected\n";
    #else
    std::cout << "Standard configuration\n";
    #endif
    
    // 三元运算符
    #if MAX_BUFFER_SIZE > 2048
    #define BUFFER_TYPE "Huge"
    #elif MAX_BUFFER_SIZE > 512
    #define BUFFER_TYPE "Large"
    #else
    #define BUFFER_TYPE "Small"
    #endif
    
    std::cout << "Buffer type: " << BUFFER_TYPE << "\n";
    
    return 0;
}
```

### 3.2 defined 运算符

`defined` 运算符用于检查宏是否已定义，有两种形式：

- `defined identifier`
- `defined (identifier)`

```cpp
#include <iostream>

#define DEBUG 1
#define MAX_CONNECTIONS 100
// #define ENABLE_SSL  // 未定义

int main() {
    std::cout << "Application started\n";
    
    // 检查宏是否定义
    #if defined(DEBUG)
    std::cout << "Debug mode is enabled\n";
    #endif
    
    // 检查宏是否未定义
    #if !defined(ENABLE_SSL)
    std::cout << "SSL is not enabled\n";
    #endif
    
    // 复杂的 defined 表达式
    #if defined(DEBUG) && defined(MAX_CONNECTIONS)
    std::cout << "Debug mode with connection limit: " << MAX_CONNECTIONS << "\n";
    #endif
    
    // 使用括号形式
    #if defined (ENABLE_SSL) || defined (DEBUG)
    std::cout << "Either SSL or debug mode is enabled\n";
    #else
    std::cout << "Neither SSL nor debug mode is enabled\n";
    #endif
    
    // 多重 defined 检查
    #if defined(DEBUG) && defined(MAX_CONNECTIONS) && !defined(ENABLE_SSL)
    std::cout << "Debug configuration without SSL\n";
    #endif
    
    return 0;
}
```

### 3.3 宏展开与条件表达式

在条件表达式中，宏会被展开后再进行求值。

```cpp
#include <iostream>

#define VERSION_MAJOR 1
#define VERSION_MINOR 2
#define VERSION_PATCH 3

#define VERSION_STRING "1.2.3"
#define VERSION_NUMBER (VERSION_MAJOR * 10000 + VERSION_MINOR * 100 + VERSION_PATCH)

#define IS_STABLE_VERSION (VERSION_MAJOR >= 1 && VERSION_MINOR >= 0)

int main() {
    std::cout << "Application started\n";
    
    // 宏展开示例
    #if VERSION_MAJOR >= 1
    std::cout << "Major version check passed\n";
    #endif
    
    // 复杂宏展开
    #if VERSION_NUMBER >= 10203
    std::cout << "Version number check passed: " << VERSION_NUMBER << "\n";
    #endif
    
    // 布尔宏展开
    #if IS_STABLE_VERSION
    std::cout << "Stable version detected\n";
    #endif
    
    // 字符串宏在条件表达式中的处理
    #if 0  // 字符串宏不能直接用于数值比较
    // #if VERSION_STRING == "1.2.3"  // 这会导致错误
    std::cout << "This will never be compiled\n";
    #endif
    
    // 正确的字符串版本检查方式
    #if VERSION_MAJOR == 1 && VERSION_MINOR == 2 && VERSION_PATCH == 3
    std::cout << "Version 1.2.3 confirmed\n";
    #endif
    
    return 0;
}
```

### 3.4 C++20 新增运算符

C++20 标准增加了对 `__has_include` 和 `__has_cpp_attribute` 的支持。

```cpp
#include <iostream>

int main() {
    std::cout << "Application started\n";
    
    // C++20: __has_include 检查头文件是否存在
    #if __has_include(<thread>)
    #include <thread>
    std::cout << "C++11 thread support available\n";
    #else
    std::cout << "C++11 thread support not available\n";
    #endif
    
    // 检查多个头文件
    #if __has_include(<atomic>)
    #include <atomic>
    std::cout << "Atomic operations support available\n";
    #endif
    
    // 检查用户头文件
    #if __has_include("config.h")
    #include "config.h"
    std::cout << "Custom configuration file found\n";
    #else
    std::cout << "Using default configuration\n";
    #endif
    
    // C++20: __has_cpp_attribute 检查属性支持
    #if __has_cpp_attribute(nodiscard)
    std::cout << "Nodiscard attribute supported\n";
    #endif
    
    #if __has_cpp_attribute(fallthrough)
    std::cout << "Fallthrough attribute supported\n";
    #endif
    
    return 0;
}
```

### 3.5 C++26 新增运算符

C++26 标准将增加对 `__has_embed` 的支持，用于检测资源是否可供嵌入。

```cpp
#include <iostream>

int main() {
    std::cout << "Application started\n";
    
    // C++26: __has_embed 检查资源文件
    #if __has_embed("resource.txt")
    // 嵌入资源文件
    std::cout << "Resource embedding supported\n";
    #endif
    
    return 0;
}
```

## 4. 条件包含块结构

### 4.1 基本结构

条件包含块的基本结构遵循严格的语法要求：

```cpp
#include <iostream>

#define FEATURE_A
#define FEATURE_B
#define DEBUG 1

int main() {
    std::cout << "Application started\n";
    
    // 最简单的结构
    #ifdef FEATURE_A
    std::cout << "Feature A enabled\n";
    #endif
    
    // if-else 结构
    #ifdef DEBUG
    std::cout << "Debug mode\n";
    #else
    std::cout << "Release mode\n";
    #endif
    
    // if-elif-else 结构
    #ifdef FEATURE_A
    std::cout << "Using Feature A\n";
    #elif defined(FEATURE_B)
    std::cout << "Using Feature B\n";
    #else
    std::cout << "Using default feature\n";
    #endif
    
    // 复杂的多重条件
    #if defined(FEATURE_A) && defined(FEATURE_B)
    std::cout << "Both features A and B enabled\n";
    #elif defined(FEATURE_A)
    std::cout << "Only feature A enabled\n";
    #elif defined(FEATURE_B)
    std::cout << "Only feature B enabled\n";
    #else
    std::cout << "No special features enabled\n";
    #endif
    
    return 0;
}
```

### 4.2 嵌套条件包含

条件包含块可以嵌套使用，形成复杂的条件逻辑：

```cpp
#include <iostream>

#define PLATFORM_WINDOWS
#define DEBUG 1
#define ENABLE_LOGGING

int main() {
    std::cout << "Application started\n";
    
    // 第一层条件包含
    #ifdef PLATFORM_WINDOWS
    std::cout << "Windows platform detected\n";
    
    // 嵌套的第二层条件包含
    #ifdef DEBUG
    std::cout << "Windows debug build\n";
    
    // 嵌套的第三层条件包含
    #ifdef ENABLE_LOGGING
    std::cout << "Windows debug build with logging\n";
    #else
    std::cout << "Windows debug build without logging\n";
    #endif  // ENABLE_LOGGING
    
    #else  // not DEBUG
    std::cout << "Windows release build\n";
    #endif  // DEBUG
    
    #else  // not PLATFORM_WINDOWS
    std::cout << "Non-Windows platform\n";
    
    #ifdef DEBUG
    std::cout << "Non-Windows debug build\n";
    #else
    std::cout << "Non-Windows release build\n";
    #endif  // DEBUG
    
    #endif  // PLATFORM_WINDOWS
    
    return 0;
}
```

### 4.3 条件包含的处理流程

预处理器按顺序处理条件包含指令：

```cpp
#include <iostream>

#define VERSION 2
#define DEBUG 1
#define EXPERIMENTAL_FEATURE

int main() {
    std::cout << "Application started\n";
    
    // 处理流程演示
    #if VERSION >= 3
    std::cout << "Version 3+ code path\n";
    // 这段代码被跳过，后续的 elif 也会被处理
    #elif VERSION >= 2
    std::cout << "Version 2 code path\n";
    // 这段代码被包含，后续的 elif 和 else 被跳过
    #elif VERSION >= 1
    std::cout << "Version 1 code path\n";
    // 这段代码被跳过
    #else
    std::cout << "Unknown version code path\n";
    // 这段代码被跳过
    #endif
    
    // 另一个处理流程
    #ifdef DEBUG
    std::cout << "Debug information\n";
    
    #ifdef EXPERIMENTAL_FEATURE
    std::cout << "Experimental feature debug info\n";
    #endif
    
    #else  // not DEBUG
    std::cout << "Release mode\n";
    #endif
    
    return 0;
}
```

## 5. 条件包含的应用场景

### 5.1 平台相关代码

根据不同操作系统编译不同的代码：

```cpp
#include <iostream>

int main() {
    std::cout << "Application started\n";
    
    // Windows 平台特定代码
    #ifdef _WIN32
    #include <windows.h>
    std::cout << "Running on Windows\n";
    Sleep(1000);  // Windows 特定函数
    
    // Linux 平台特定代码
    #elif defined(__linux__)
    #include <unistd.h>
    std::cout << "Running on Linux\n";
    sleep(1);     // Linux 特定函数
    
    // macOS 平台特定代码
    #elif defined(__APPLE__)
    #include <unistd.h>
    std::cout << "Running on macOS\n";
    sleep(1);     // macOS 特定函数
    
    // 其他平台
    #else
    std::cout << "Running on unknown platform\n";
    #endif
    
    return 0;
}
```

### 5.2 版本控制

根据编译器版本或 C++ 标准版本启用不同特性：

```cpp
#include <iostream>

int main() {
    std::cout << "Application started\n";
    
    // C++ 标准版本检查
    #if __cplusplus >= 202302L
    std::cout << "C++23 or later\n";
    #elif __cplusplus >= 202002L
    std::cout << "C++20 or later\n";
    #elif __cplusplus >= 201703L
    std::cout << "C++17 or later\n";
    #elif __cplusplus >= 201402L
    std::cout << "C++14 or later\n";
    #elif __cplusplus >= 201103L
    std::cout << "C++11 or later\n";
    #else
    std::cout << "C++98/C++03\n";
    #endif
    
    // 编译器版本检查
    #ifdef __GNUC__
    #if __GNUC__ >= 10
    std::cout << "GCC 10 or later\n";
    #elif __GNUC__ >= 7
    std::cout << "GCC 7-9\n";
    #else
    std::cout << "Old GCC version\n";
    #endif
    #endif
    
    // 编译器特定特性
    #ifdef __clang__
    std::cout << "Using Clang compiler\n";
    #elif defined(_MSC_VER)
    std::cout << "Using MSVC compiler\n";
    #elif defined(__GNUC__)
    std::cout << "Using GCC compiler\n";
    #endif
    
    return 0;
}
```

### 5.3 调试与发布版本

根据构建配置包含不同的代码：

```cpp
#include <iostream>
#include <cassert>

// 通常在构建系统中定义 DEBUG
// #define DEBUG 1

#ifdef DEBUG
#define LOG_DEBUG(fmt) std::cerr << "[DEBUG] " << fmt << std::endl
#define ASSERT(condition) assert(condition)
#else
#define LOG_DEBUG(fmt) do {} while(0)
#define ASSERT(condition) do {} while(0)
#endif

int main() {
    std::cout << "Application started\n";
    
    int x = 5;
    LOG_DEBUG("Variable x initialized to " << x);
    
    ASSERT(x > 0);
    
    #ifdef DEBUG
    std::cout << "Debug build: additional checks enabled\n";
    std::cout << "Debug build: verbose logging enabled\n";
    std::cout << "Debug build: assertions enabled\n";
    #else
    std::cout << "Release build: optimized for performance\n";
    std::cout << "Release build: minimal logging\n";
    std::cout << "Release build: assertions disabled\n";
    #endif
    
    return 0;
}
```

### 5.4 功能开关

通过配置宏启用或禁用特定功能：

```cpp
#include <iostream>

// 功能开关配置
#define ENABLE_NETWORKING 1
#define ENABLE_DATABASE 0
#define ENABLE_ENCRYPTION 1
#define ENABLE_LOGGING 1

int main() {
    std::cout << "Application started\n";
    
    // 网络功能
    #if ENABLE_NETWORKING
    std::cout << "Networking enabled\n";
    // 网络相关代码
    #include <sys/socket.h>
    // 网络初始化代码
    #else
    std::cout << "Networking disabled\n";
    #endif
    
    // 数据库功能
    #if ENABLE_DATABASE
    std::cout << "Database support enabled\n";
    // 数据库相关代码
    #include <sqlite3.h>
    // 数据库初始化代码
    #else
    std::cout << "Database support disabled\n";
    #endif
    
    // 加密功能
    #if ENABLE_ENCRYPTION
    std::cout << "Encryption enabled\n";
    // 加密相关代码
    #else
    std::cout << "Encryption disabled\n";
    #endif
    
    // 日志功能
    #if ENABLE_LOGGING
    std::cout << "Logging enabled\n";
    // 日志相关代码
    #else
    std::cout << "Logging disabled\n";
    #endif
    
    return 0;
}
```

### 5.5 编译器特定代码

处理不同编译器之间的差异：

```cpp
#include <iostream>

int main() {
    std::cout << "Application started\n";
    
    // GCC 特定代码
    #ifdef __GNUC__
    std::cout << "Compiled with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << "\n";
    
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
    std::cout << "Compiled with Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << "\n";
    
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-variable"
    #pragma clang diagnostic pop
    #endif
    
    // MSVC 特定代码
    #ifdef _MSC_VER
    std::cout << "Compiled with MSVC " << _MSC_VER << "\n";
    
    #pragma warning(push)
    #pragma warning(disable: 4996)
    #pragma warning(pop)
    #endif
    
    // 通用代码
    std::cout << "Common code executed on all compilers\n";
    
    return 0;
}
```

### 5.6 C++ 标准特性检测

检测和使用特定 C++ 标准的新特性：

```cpp
#include <iostream>

int main() {
    std::cout << "Application started\n";
    
    // C++11 特性检测
    #if __cplusplus >= 201103L
    std::cout << "C++11 features available\n";
    
    // auto 关键字
    auto value = 42;
    std::cout << "Auto value: " << value << "\n";
    
    // nullptr
    void* ptr = nullptr;
    std::cout << "nullptr supported\n";
    #endif
    
    // C++14 特性检测
    #if __cplusplus >= 201402L
    std::cout << "C++14 features available\n";
    
    // 二进制字面量
    auto binary_value = 0b1010;
    std::cout << "Binary literal: " << binary_value << "\n";
    #endif
    
    // C++17 特性检测
    #if __cplusplus >= 201703L
    std::cout << "C++17 features available\n";
    
    // 结构化绑定
    auto [x, y] = std::make_pair(1, 2);
    std::cout << "Structured binding: " << x << ", " << y << "\n";
    #endif
    
    // C++20 特性检测
    #if __cplusplus >= 202002L
    std::cout << "C++20 features available\n";
    
    // 概念 (Concepts)
    #if __has_cpp_attribute(nodiscard)
    [[nodiscard]] int get_value() { return 42; }
    std::cout << "nodiscard attribute supported\n";
    #endif
    #endif
    
    return 0;
}
```

## 6. C++23 新特性详解

### 6.1 #elifdef 指令

`#elifdef` 指令等价于 `#elif defined(identifier)`，提供更简洁的语法：

```cpp
#include <iostream>

#define FEATURE_A
// #define FEATURE_B
// #define FEATURE_C

int main() {
    std::cout << "Application started\n";
    
    // 传统写法
    #ifdef FEATURE_A
    std::cout << "Feature A enabled (traditional)\n";
    #elif defined(FEATURE_B)
    std::cout << "Feature B enabled (traditional)\n";
    #elif defined(FEATURE_C)
    std::cout << "Feature C enabled (traditional)\n";
    #else
    std::cout << "No features enabled (traditional)\n";
    #endif
    
    // C++23 新写法
    #ifdef FEATURE_A
    std::cout << "Feature A enabled (C++23)\n";
    #elifdef FEATURE_B
    std::cout << "Feature B enabled (C++23)\n";
    #elifdef FEATURE_C
    std::cout << "Feature C enabled (C++23)\n";
    #else
    std::cout << "No features enabled (C++23)\n";
    #endif
    
    return 0;
}
```

### 6.2 #elifndef 指令

`#elifndef` 指令等价于 `#elif !defined(identifier)`，用于检查宏未定义：

```cpp
#include <iostream>

#define CONFIG_FILE_LOADED
// #define DATABASE_CONFIGURED
// #define NETWORK_INITIALIZED

int main() {
    std::cout << "Application started\n";
    
    // 传统写法
    #ifdef CONFIG_FILE_LOADED
    std::cout << "Config file loaded (traditional)\n";
    #elif !defined(DATABASE_CONFIGURED)
    std::cout << "Database not configured (traditional)\n";
    #elif !defined(NETWORK_INITIALIZED)
    std::cout << "Network not initialized (traditional)\n";
    #else
    std::cout << "All systems go (traditional)\n";
    #endif
    
    // C++23 新写法
    #ifdef CONFIG_FILE_LOADED
    std::cout << "Config file loaded (C++23)\n";
    #elifndef DATABASE_CONFIGURED
    std::cout << "Database not configured (C++23)\n";
    #elifndef NETWORK_INITIALIZED
    std::cout << "Network not initialized (C++23)\n";
    #else
    std::cout << "All systems go (C++23)\n";
    #endif
    
    return 0;
}
```

### 6.3 新增运算符支持

C++23 增强了对 `__has_include`、`__has_cpp_attribute` 和 `__has_embed` 的支持：

```cpp
#include <iostream>

int main() {
    std::cout << "Application started\n";
    
    // __has_include 支持
    #if __has_include(<atomic>)
    #include <atomic>
    std::cout << "Atomic operations available\n";
    #else
    std::cout << "Atomic operations not available\n";
    #endif
    
    // __has_cpp_attribute 支持
    #if __has_cpp_attribute(noreturn)
    [[noreturn]] void fatal_error() {
        std::cout << "Fatal error occurred\n";
        std::exit(1);
    }
    std::cout << "Noreturn attribute supported\n";
    #endif
    
    // __has_embed 支持 (示例)
    #if __has_embed("resource.txt")
    // 嵌入资源文件
    std::cout << "Resource embedding supported\n";
    #endif
    
    return 0;
}
```

## 7. 高级用法与技巧

### 7.1 复杂条件组合

组合多个条件创建复杂的编译逻辑：

```cpp
#include <iostream>

#define PLATFORM_WINDOWS
#define COMPILER_GCC
#define VERSION_MAJOR 2
#define VERSION_MINOR 1
#define ENABLE_DEBUG 1
#define ENABLE_PROFILING 0

int main() {
    std::cout << "Application started\n";
    
    // 复杂的平台和编译器组合
    #if (defined(PLATFORM_WINDOWS) && defined(COMPILER_GCC)) || \
        (defined(PLATFORM_LINUX) && defined(COMPILER_CLANG))
    std::cout << "Supported platform-compiler combination\n";
    #else
    std::cout << "Unsupported platform-compiler combination\n";
    #endif
    
    // 版本和功能组合
    #if VERSION_MAJOR >= 2 && (VERSION_MINOR >= 1 || defined(ENABLE_DEBUG))
    std::cout << "Advanced features available\n";
    
    #if ENABLE_DEBUG && !ENABLE_PROFILING
    std::cout << "Debug mode without profiling\n";
    #elif ENABLE_PROFILING && !ENABLE_DEBUG
    std::cout << "Profiling mode without debug\n";
    #elif ENABLE_DEBUG && ENABLE_PROFILING
    std::cout << "Debug and profiling mode\n";
    #else
    std::cout << "Release mode\n";
    #endif
    
    #else
    std::cout << "Basic features only\n";
    #endif
    
    return 0;
}
```

### 7.2 条件包含与宏定义

动态定义宏以控制后续代码：

```cpp
#include <iostream>

#define TARGET_PLATFORM 2  // 1=Windows, 2=Linux, 3=macOS

int main() {
    std::cout << "Application started\n";
    
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
    
    std::cout << "Platform: " << PLATFORM_NAME << "\n";
    std::cout << "Path separator: " << PATH_SEPARATOR << "\n";
    
    #if HAS_WIDE_CHARACTERS
    std::cout << "Wide character support enabled\n";
    #else
    std::cout << "Wide character support disabled\n";
    #endif
    
    return 0;
}
```

### 7.3 条件包含性能优化

使用条件包含优化编译性能：

```cpp
#include <iostream>

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
void rarely_called_function() {
    std::cout << "Rarely called function\n";
}

FORCE_INLINE
int inline_function(int x) {
    return x + 1;
}

int main() {
    std::cout << "Application started\n";
    std::cout << "Result: " << frequently_called_function(5) << "\n";
    rarely_called_function();
    std::cout << "Inline result: " << inline_function(10) << "\n";
    return 0;
}
```

### 7.4 条件包含调试技巧

使用条件包含进行调试和诊断：

```cpp
#include <iostream>

#define DEBUG_LEVEL 2
#define TRACE_FUNCTIONS 1
#define DUMP_VARIABLES 0

// 调试宏
#if DEBUG_LEVEL >= 3
#define DEBUG_TRACE(msg) std::cerr << "[TRACE] " << msg << std::endl
#elif DEBUG_LEVEL >= 2
#define DEBUG_TRACE(msg) std::cerr << "[DEBUG] " << msg << std::endl
#elif DEBUG_LEVEL >= 1
#define DEBUG_TRACE(msg) std::cerr << "[INFO] " << msg << std::endl
#else
#define DEBUG_TRACE(msg) do {} while(0)
#endif

// 函数跟踪宏
#if TRACE_FUNCTIONS
#define FUNCTION_ENTER(name) do { \
    DEBUG_TRACE("Entering function: " #name); \
} while(0)
#define FUNCTION_EXIT(name) do { \
    DEBUG_TRACE("Exiting function: " #name); \
} while(0)
#else
#define FUNCTION_ENTER(name) do {} while(0)
#define FUNCTION_EXIT(name) do {} while(0)
#endif

// 变量转储宏
#if DUMP_VARIABLES
#define DUMP_VAR(name, value) do { \
    DEBUG_TRACE("Variable " #name " = " + std::to_string(static_cast<int>(value))); \
} while(0)
#else
#define DUMP_VAR(name, value) do {} while(0)
#endif

int calculate_result(int a, int b) {
    FUNCTION_ENTER(calculate_result);
    DUMP_VAR(a, a);
    DUMP_VAR(b, b);
    
    int result = a + b;
    DUMP_VAR(result, result);
    
    FUNCTION_EXIT(calculate_result);
    return result;
}

int main() {
    DEBUG_TRACE("Application started");
    
    int x = 10, y = 20;
    int result = calculate_result(x, y);
    
    DEBUG_TRACE("Final result: " + std::to_string(result));
    DEBUG_TRACE("Application finished");
    
    return 0;
}
```

## 8. 最佳实践

### 8.1 条件包含编写规范

遵循规范的条件包含编写方式：

```cpp
#include <iostream>

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
#if __cplusplus >= 201103L
    #define HAS_C11_FEATURES 1
#endif

int main() {
    std::cout << "Application started\n";
    
    // 条件包含的规范写法
    #if ENABLE_FEATURE_X
    std::cout << "Feature X is enabled\n";
    #endif
    
    // 多条件的规范写法
    #if defined(PLATFORM_WINDOWS) && defined(COMPILER_GCC)
    std::cout << "Windows GCC build\n";
    #elif defined(PLATFORM_LINUX) && defined(COMPILER_GCC)
    std::cout << "Linux GCC build\n";
    #elif defined(PLATFORM_MACOS) && defined(COMPILER_CLANG)
    std::cout << "macOS Clang build\n";
    #else
    std::cout << "Other platform/compiler\n";
    #endif
    
    // 版本检查的规范写法
    #if MAX_BUFFER_SIZE >= 4096
    std::cout << "Large buffer size: " << MAX_BUFFER_SIZE << "\n";
    #endif
    
    // 调试代码的规范写法
    #if DEBUG_BUILD
    std::cout << "Debug build information\n";
    #endif
    
    return 0;
}
```

### 8.2 条件包含设计模式

使用设计模式优化条件包含的使用：

```cpp
#include <iostream>

// 1. 特性检测模式
#define HAS_THREADS_FEATURE __cplusplus >= 201103L

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
enum ConfigMode {
    CONFIG_MODE_DEBUG,
    CONFIG_MODE_RELEASE,
    CONFIG_MODE_PROFILE
};

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
    std::cout << "Application started\n";
    std::cout << "Platform API: " << PLATFORM_API << "\n";
    std::cout << "Config mode: " << CURRENT_CONFIG_MODE << "\n";
    std::cout << "Required features: " << (REQUIRED_FEATURES ? "Available" : "Missing") << "\n";
    std::cout << "Optional features: " << (OPTIONAL_FEATURES ? "Available" : "Missing") << "\n";
    
    return 0;
}
```

### 8.3 条件包含维护策略

制定有效的条件包含维护策略：

```cpp
#include <iostream>

// 1. 版本兼容性维护
#if __cplusplus >= 202302L
    // C++23 特性
    #define MODERN_SYNTAX_AVAILABLE 1
#elif __cplusplus >= 201103L
    // C++11 特性
    #define MODERN_SYNTAX_AVAILABLE 0
#else
    // C++98/C++03 特性
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
    std::cout << "Application started\n";
    
    #if MODERN_SYNTAX_AVAILABLE
    std::cout << "Using modern C++ syntax\n";
    #else
    std::cout << "Using legacy C++ syntax\n";
    #endif
    
    #if GCC_MODERN_FEATURES
    std::cout << "GCC modern features available\n";
    #endif
    
    return 0;
}
```

### 8.4 常见错误与避免

识别和避免条件包含的常见错误：

```cpp
#include <iostream>

// 错误1: 宏定义中的语法错误
// #define MAX(a,b) (a > b) ? a : b  // 错误：缺少括号
#define MAX(a,b) ((a) > (b) ? (a) : (b))  // 正确

// 错误2: 未匹配的条件包含指令
/*
#if defined(DEBUG)
    std::cout << "Debug mode\n";
// 缺少 #endif
*/

// 错误3: 嵌套条件包含的逻辑错误
/*
#ifdef FEATURE_A
    #ifdef FEATURE_B
        std::cout << "Both features\n";
    #else
        std::cout << "Only feature A\n";
    #endif
#else
    #ifdef FEATURE_B
        std::cout << "Only feature B\n";
    #else
        std::cout << "No features\n";
    #endif
#endif
*/

// 错误4: 宏展开导致的意外结果
#define MULTIPLY(a,b) a * b
// std::cout << MULTIPLY(2 + 3, 4 + 5) << "\n";  // 结果是 2+3*4+5 = 19，不是期望的 5*9 = 45

int main() {
    std::cout << "Application started\n";
    
    // 正确的条件包含使用
    #ifdef DEBUG
    std::cout << "Debug mode enabled\n";
    #endif
    
    // 正确的嵌套条件包含
    #if defined(FEATURE_A) || defined(FEATURE_B)
    std::cout << "At least one feature enabled\n";
    
    #if defined(FEATURE_A) && defined(FEATURE_B)
    std::cout << "Both features enabled\n";
    #elif defined(FEATURE_A)
    std::cout << "Only feature A enabled\n";
    #else
    std::cout << "Only feature B enabled\n";
    #endif
    
    #else
    std::cout << "No features enabled\n";
    #endif
    
    // 正确的宏使用
    std::cout << "Max of 5 and 3: " << MAX(5, 3) << "\n";
    std::cout << "Multiply (2+3)*(4+5): " << MULTIPLY((2 + 3), (4 + 5)) << "\n";
    
    return 0;
}
```

## 9. 标准参考

### 9.1 C++ 标准版本

有条件包含相关标准条款：

- C++23 标准 (ISO/IEC 14882:2024): 15.1 条件包含
- C++20 标准 (ISO/IEC 14882:2020): 15.1 条件包含
- C++17 标准 (ISO/IEC 14882:2017): 16.1 条件包含
- C++14 标准 (ISO/IEC 14882:2014): 16.1 条件包含
- C++11 标准 (ISO/IEC 14882:2011): 16.1 条件包含
- C++98/C++03 标准 (ISO/IEC 14882:1998/2003): 16.1 条件包含

### 9.2 相关标准条款

- 2.1 翻译阶段
- 15.1/16.1 条件包含
- 15.2/16.2 源文件包含
- 15.3/16.3 宏替换
- 15.8/16.8 预定义宏名

### 9.3 缺陷报告

相关缺陷报告：

**CWG 1955 (C++98)**：

- 已发布的行为：失败的 `#elif` 表达式必须有效
- 正确的行为：失败的 `#elif` 被跳过

## 10. 总结

### 10.1 核心要点

有条件包含是 C++ 预处理器的核心功能之一，提供了以下关键能力：

- 条件编译：根据编译时条件决定代码是否被编译
- 平台适配：支持不同操作系统和硬件平台的代码编译
- 版本控制：根据编译器版本和 C++ 标准版本启用不同特性
- 调试支持：在调试和发布版本中包含不同的代码
- 功能开关：动态启用或禁用特定功能
- 编译器兼容：处理不同编译器之间的差异
- C++ 标准特性检测：检测和使用特定 C++ 标准的新特性

### 10.2 使用建议

- 合理使用条件包含：避免过度复杂的条件逻辑
- 保持代码可读性：使用清晰的缩进和注释
- 测试所有路径：确保所有条件分支都能正确编译
- 文档化配置选项：清楚地记录所有配置宏的含义
- 避免副作用：条件包含不应影响程序的逻辑结构
- 考虑维护成本：平衡灵活性和代码复杂度
- 利用 C++ 特性：充分利用 `__has_include`、`__has_cpp_attribute` 等现代特性

### 10.3 发展趋势

有条件包含在 C++ 语言的发展中持续演进：

- C++98/C++03：建立了基本的条件包含框架
- C++11：增强了宏定义和条件包含的能力
- C++14：保持了 C++11 的条件包含特性
- C++17：增加了对 `__has_include` 的标准支持
- C++20：增加了对 `__has_cpp_attribute` 的支持
- C++23：新增了 `#elifdef` 和 `#elifndef` 指令
- C++26：计划增加对 `__has_embed` 的支持

随着 C++ 语言的不断发展，有条件包含功能变得更加强大和易用，为开发者提供了更好的条件编译支持。C++23 引入的 `#elifdef` 和 `#elifndef` 指令进一步简化了常见的条件包含模式，使得代码更加清晰和易读。
