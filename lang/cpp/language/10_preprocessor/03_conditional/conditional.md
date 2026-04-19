# C++ 条件包含 (Conditional Inclusion)

## 1. 概述 (Overview)

条件包含（Conditional Inclusion）是 C++ 预处理器的核心功能，允许根据条件选择性地编译源文件的特定部分。这种机制通过预处理指令控制代码块的编译与否，是实现跨平台开发、编译时配置、特性检测和调试控制的关键技术。

条件包含通过以下指令实现：

- **条件测试指令**：`#if`、`#ifdef`、`#ifndef`
- **分支指令**：`#elif`、`#elifdef`、`#elifndef`（C++23）、`#else`
- **结束指令**：`#endif`

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

条件编译机制继承自 C 语言，在 C++ 早期版本中就已存在。它解决了跨平台移植性和编译时配置的核心问题，让开发者能够为不同环境编写统一的源代码。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 标准化基础条件编译指令：`#if`、`#ifdef`、`#ifndef`、`#elif`、`#else`、`#endif` |
| C++11 | 无重大变更 |
| C++17 | `__has_include` 可用于条件检测；`defined` 运算符支持 `__has_include` |
| C++20 | `__has_cpp_attribute` 可用于条件检测；`defined` 运算符支持 `__has_cpp_attribute` |
| C++23 | 新增 `#elifdef` 和 `#elifndef` 指令 |

### 设计动机

条件编译解决了以下核心问题：

1. **平台适配**：不同操作系统、硬件架构需要不同的实现
2. **编译配置**：调试版本与发布版本的不同行为
3. **特性检测**：检测编译器是否支持特定语言特性
4. **头文件保护**：防止头文件重复包含
5. **版本兼容**：支持多个语言版本的代码

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```
#if 表达式
#ifdef 标识符
#ifndef 标识符
#elif 表达式
#elifdef 标识符        (C++23 起)
#elifndef 标识符      (C++23 起)
#else
#endif
```

### 指令详解

#### 条件测试指令

| 指令 | 说明 | 版本 |
|------|------|------|
| `#if 表达式` | 如果表达式非零，编译后续代码块 | C++98 |
| `#ifdef 标识符` | 如果标识符已定义为宏，编译后续代码块 | C++98 |
| `#ifndef 标识符` | 如果标识符未定义为宏，编译后续代码块 | C++98 |

#### 分支指令

| 指令 | 说明 | 版本 |
|------|------|------|
| `#elif 表达式` | 否则如果表达式非零 | C++98 |
| `#elifdef 标识符` | 否则如果标识符已定义 | C++23 |
| `#elifndef 标识符` | 否则如果标识符未定义 | C++23 |
| `#else` | 否则（无条件编译） | C++98 |

#### 结束指令

| 指令 | 说明 |
|------|------|
| `#endif` | 结束条件编译块 |

### 条件表达式规则

#### `#if` 和 `#elif` 表达式

表达式可以包含以下内容：

##### `defined` 运算符

```cpp
defined 标识符
defined (标识符)
```

- 如果标识符已定义为宏名，返回 `1`
- 否则返回 `0`
- C++17 起：`__has_include` 和 `__has_cpp_attribute`（C++20）在此上下文中被视为已定义的宏名

##### `__has_include` 表达式（C++17）

```cpp
__has_include("文件名")
__has_include(<文件名>)
```

检测头文件或源文件是否存在。

##### `__has_cpp_attribute` 表达式（C++20）

```cpp
__has_cpp_attribute(属性标记)
```

检测给定属性标记是否受支持及其支持的版本。

##### 标识符替换规则

在宏展开和 `defined`、`__has_include`、`__has_cpp_attribute` 表达式求值后：

- 非布尔字面量的标识符替换为 `0`
- 包括词法上为关键字的标识符
- 不包括替代标记（如 `and`、`or` 等）

##### 表达式求值

最终表达式作为整型常量表达式求值：
- 非零值：编译代码块
- 零值：跳过代码块

#### 指令等价关系

| 指令 | 等价形式 |
|------|----------|
| `#ifdef 标识符` | `#if defined 标识符` |
| `#ifndef 标识符` | `#if !defined 标识符` |
| `#elifdef 标识符` | `#elif defined 标识符` |
| `#elifndef 标识符` | `#elif !defined 标识符` |

## 4. 底层原理 (Underlying Principles)

### 处理流程

条件预处理块的执行遵循以下流程：

```
开始 (#if/#ifdef/#ifndef)
    ↓
计算条件表达式
    ↓
条件为真？ ──是──→ 编译代码块，跳过后续分支
    │
    否
    ↓
处理下一个分支 (#elif/#elifdef/#elifndef)
    ↓
计算条件表达式
    ↓
条件为真？ ──是──→ 编译代码块，跳过后续分支
    │
    否
    ↓
有 #else？ ──是──→ 编译 #else 代码块
    │
    否
    ↓
结束 (#endif)
```

### 嵌套处理

条件编译块支持嵌套，每个内部条件块独立处理：

```cpp
#if OUTER_COND
    #if INNER_COND
        // 内部条件块独立处理
    #endif
#elif ANOTHER_COND
    // ...
#endif
```

### CWG 1955 变更

| 版本 | 行为 |
|------|------|
| CWG 1955 前 | `#elif` 的表达式即使在被跳过的分支中也必须是有效表达式 |
| CWG 1955 后 | 被跳过的 `#elif` 分支的表达式不会被求值，不必有效 |

**示例**：

```cpp
#if COND1
    // ...
#elif UNDEFINED_MACRO  // CWG 1955 前：如果 COND1 为真，此表达式仍需有效
    // ...
#endif
```

### 表达式求值顺序

1. 宏展开
2. `defined` 表达式求值
3. `__has_include` 表达式求值（C++17）
4. `__has_cpp_attribute` 表达式求值（C++20）
5. 未定义标识符替换为 `0`
6. 整型常量表达式求值

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 头文件保护

```cpp
#ifndef MY_HEADER_HPP
#define MY_HEADER_HPP

// 头文件内容

#endif // MY_HEADER_HPP
```

#### 2. 平台适配

```cpp
#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#elif defined(__APPLE__)
    #include <mach/mach.h>
#endif
```

#### 3. 编译配置

```cpp
#ifdef NDEBUG
    #define LOG(msg)  // 发布版本移除日志
#else
    #include <iostream>
    #define LOG(msg) std::cout << msg << std::endl
#endif
```

#### 4. 特性检测（C++17）

```cpp
#if __has_include(<optional>)
    #include <optional>
    #define HAS_OPTIONAL 1
#else
    // 提供替代实现
#endif
```

#### 5. 属性检测（C++20）

```cpp
#if __has_cpp_attribute([[likely]])
    #define LIKELY [[likely]]
#else
    #define LIKELY
#endif
```

#### 6. C++23 指令兼容性处理

```cpp
// 检测编译器是否支持 #elifdef/#elifndef
#if 0
#elifndef UNDEFINED_MACRO
    #define ELIFDEF_SUPPORTED
#else
#endif

#ifdef ELIFDEF_SUPPORTED
    // 使用 C++23 新语法
    #ifdef CPU
        // ...
    #elifdef GPU
        // ...
    #endif
#else
    // 使用传统语法
    #ifdef CPU
        // ...
    #elif defined(GPU)
        // ...
    #endif
#endif
```

### 最佳实践

1. **优先使用 `#if defined()` 替代 `#ifdef`**：在复杂条件中更清晰
2. **使用 `#elif` 减少嵌套**：避免过深的嵌套层级
3. **为 `#endif` 添加注释**：标明对应的条件开始
4. **特性检测优于版本检测**：检测具体特性而非编译器版本

### 常见陷阱

#### 陷阱 1：C++23 前版本使用 `#elifdef`

```cpp
// 不支持 C++23 的编译器可能跳过未知指令
#ifdef CPU
    std::cout << "4: no1\n";
#elifdef GPU  // 未知指令，被跳过
    std::cout << "4: no2\n";
#elifndef RAM
    std::cout << "4: yes\n";
#else
    std::cout << "4: no!\n";  // 可能意外选中此分支
#endif
```

#### 陷阱 2：忘记 `#endif`

```cpp
#ifdef FEATURE_A
    // 代码
// 错误：缺少 #endif
```

#### 陷阱 3：条件表达式中的未定义标识符

```cpp
#if VERSION == 2  // 如果 VERSION 未定义，替换为 0，条件为假
    // 可能不符合预期
#endif

// 修正：使用 defined() 保护
#if defined(VERSION) && VERSION == 2
    // ...
#endif
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：基本条件编译

```cpp
#define ABCD 2
#include <iostream>

int main()
{
#ifdef ABCD
    std::cout << "1: yes\n";
#else
    std::cout << "1: no\n";
#endif

#ifndef ABCD
    std::cout << "2: no1\n";
#elif ABCD == 2
    std::cout << "2: yes\n";
#else
    std::cout << "2: no2\n";
#endif

#if !defined(DCBA) && (ABCD < 2*4-3)
    std::cout << "3: yes\n";
#endif

    return 0;
}
```

**输出**：
```
1: yes
2: yes
3: yes
```

#### 示例 2：`defined` 运算符使用

```cpp
#include <iostream>

#define DEBUG 1
#define VERSION 3

int main()
{
#if defined(DEBUG) && DEBUG > 0
    std::cout << "Debug mode enabled\n";
#endif

#if defined(VERSION) && VERSION >= 2
    std::cout << "Version 2 or later\n";
#else
    std::cout << "Version 1 or undefined\n";
#endif

#if !defined(UNDEFINED_MACRO)
    std::cout << "UNDEFINED_MACRO is not defined\n";
#endif

    return 0;
}
```

### 高级用法

#### 示例 3：C++23 新指令兼容性处理

```cpp
#include <iostream>

// 检测编译器是否支持 #elifdef/#elifndef
#if 0
#elifndef UNDEFINED_MACRO
    #define ELIFDEF_SUPPORTED
#else
#endif

int main()
{
#ifdef ELIFDEF_SUPPORTED
    // 使用 C++23 新语法
    #ifdef CPU
        std::cout << "4: no1\n";
    #elifdef GPU
        std::cout << "4: no2\n";
    #elifndef RAM
        std::cout << "4: yes\n";
    #else
        std::cout << "4: no3\n";
    #endif
#else
    // 使用传统语法
    #ifdef CPU
        std::cout << "4: no1\n";
    #elif defined(GPU)
        std::cout << "4: no2\n";
    #elif !defined(RAM)
        std::cout << "4: yes\n";
    #else
        std::cout << "4: no3\n";
    #endif
#endif

    return 0;
}
```

**输出**：
```
4: yes
```

#### 示例 4：特性检测（C++17/20）

```cpp
#include <iostream>

// 检测头文件是否存在（C++17）
#if __has_include(<optional>)
    #include <optional>
    #define HAS_OPTIONAL 1
#endif

// 检测属性支持（C++20）
#if __has_cpp_attribute([[nodiscard]])
    #define NODISCARD [[nodiscard]]
#else
    #define NODISCARD
#endif

// 检测另一个属性
#if __has_cpp_attribute([[likely]]) >= 201803L
    #define LIKELY [[likely]]
    #define UNLIKELY [[unlikely]]
#else
    #define LIKELY
    #define UNLIKELY
#endif

int main()
{
#ifdef HAS_OPTIONAL
    std::cout << "std::optional is available\n";
#else
    std::cout << "std::optional is not available\n";
#endif

    return 0;
}
```

#### 示例 5：复杂条件组合

```cpp
#include <iostream>

#define PLATFORM_WINDOWS 1
#define FEATURE_X 1
#define VERSION 2

int main()
{
#if PLATFORM_WINDOWS && VERSION >= 2
    std::cout << "Windows platform with version 2+\n";
#elif !PLATFORM_WINDOWS && defined(FEATURE_X)
    std::cout << "Non-Windows with FEATURE_X\n";
#elif PLATFORM_WINDOWS && !defined(FEATURE_X)
    std::cout << "Windows without FEATURE_X\n";
#else
    std::cout << "Default configuration\n";
#endif

    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：条件表达式语法错误

```cpp
// 错误：#ifdef 不支持表达式
#ifdef VERSION > 1
    // ...
#endif

// 修正：使用 #if
#if VERSION > 1
    // ...
#endif

// 更安全：使用 defined() 保护
#if defined(VERSION) && VERSION > 1
    // ...
#endif
```

#### 错误示例 2：嵌套条件未正确关闭

```cpp
// 错误：嵌套条件未正确关闭
#ifdef A
    #ifdef B
        // 代码
#endif  // 只关闭了外层条件？

// 修正：正确匹配并添加注释
#ifdef A
    #ifdef B
        // 代码
    #endif // B
#endif // A
```

#### 错误示例 3：CWG 1955 前的表达式问题

```cpp
// CWG 1955 前：如果 SOME_UNDEFINED_MACRO 未定义，可能报错
#ifdef FEATURE_A
    // ...
#elif SOME_UNDEFINED_MACRO  // 可能报错
    // ...
#endif

// 修正：使用 defined() 保护
#ifdef FEATURE_A
    // ...
#elif defined(SOME_UNDEFINED_MACRO)
    // ...
#endif
```

## 7. 总结 (Summary)

### 核心要点

1. **指令体系**：`#if`/`#ifdef`/`#ifndef` 开始条件块，`#elif`/`#else` 提供分支，`#endif` 结束
2. **表达式求值**：未定义标识符替换为 `0`，`defined` 运算符检测宏定义
3. **C++17 特性**：`__has_include` 检测文件存在
4. **C++20 特性**：`__has_cpp_attribute` 检测属性支持
5. **C++23 特性**：`#elifdef`、`#elifndef` 简化条件语法
6. **CWG 1955**：被跳过的 `#elif` 表达式不再需要有效

### 指令对比

| 指令 | 用途 | 版本 |
|------|------|------|
| `#if` | 条件表达式测试 | C++98 |
| `#ifdef` | 宏定义检测 | C++98 |
| `#ifndef` | 宏未定义检测 | C++98 |
| `#elif` | 否则如果（表达式） | C++98 |
| `#elifdef` | 否则如果已定义 | C++23 |
| `#elifndef` | 否则如果未定义 | C++23 |
| `#else` | 否则 | C++98 |
| `#endif` | 结束条件块 | C++98 |

### 运算符对比

| 运算符 | 用途 | 版本 |
|--------|------|------|
| `defined` | 检测宏定义 | C++98 |
| `__has_include` | 检测文件存在 | C++17 |
| `__has_cpp_attribute` | 检测属性支持 | C++20 |

### 学习建议

1. **理解执行时机**：条件编译在预处理阶段执行，早于编译
2. **掌握 `defined` 运算符**：是复杂条件表达式的关键
3. **使用特性检测**：优先使用 `__has_include`、`__has_cpp_attribute` 而非版本检测
4. **注意兼容性**：C++23 新指令可能不被旧编译器支持
5. **遵循代码规范**：正确缩进、添加注释、避免过深嵌套

### 缺陷报告

| 编号 | 版本 | 问题 | 修正 |
|------|------|------|------|
| CWG 1955 | C++98 | 失败的 `#elif` 表达式仍需有效 | 失败的 `#elif` 被跳过 |

---

**相关参考**：
- C++ 预定义宏符号文档
- C++ 宏符号索引文档