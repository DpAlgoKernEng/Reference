# C 条件包含 (Conditional Inclusion)

## 1. 概述 (Overview)

条件包含（Conditional Inclusion）是 C 预处理器的核心功能之一，允许根据条件选择性地编译源文件的特定部分。这种机制通过预处理指令控制代码块的编译与否，是实现跨平台开发、编译时配置和调试控制的基石。

条件包含通过以下指令实现：

- **条件测试指令**：`#if`、`#ifdef`、`#ifndef`
- **分支指令**：`#elif`、`#elifdef`、`#elifndef`（C23）、`#else`
- **结束指令**：`#endif`

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

条件编译机制源于 C 语言早期设计，目的是解决跨平台移植性问题。通过条件编译，开发者可以为不同平台、不同配置编写统一的源代码，由预处理器选择合适的代码段进行编译。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C89/C90 | 标准化基础条件编译指令：`#if`、`#ifdef`、`#ifndef`、`#elif`、`#else`、`#endif` |
| C99 | 无重大变更 |
| C11 | 无重大变更 |
| C23 | 新增 `#elifdef` 和 `#elifndef` 指令；`true` 在条件表达式中求值为 1；`__has_include`、`__has_embed`、`__has_c_attribute` 可用于条件检测 |

### 设计动机

条件编译解决了以下核心问题：

1. **平台适配**：不同操作系统、硬件平台需要不同的代码实现
2. **编译配置**：调试版本与发布版本的不同编译选项
3. **功能开关**：根据是否定义某些宏来启用或禁用特定功能
4. **向后兼容**：支持不同版本的语言特性

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```
#if 表达式
#ifdef 标识符
#ifndef 标识符
#elif 表达式
#elifdef 标识符        (C23 起)
#elifndef 标识符      (C23 起)
#else
#endif
```

### 指令详解

#### 条件测试指令

| 指令 | 说明 | 版本 |
|------|------|------|
| `#if 表达式` | 如果表达式非零，编译后续代码块 | C89 |
| `#ifdef 标识符` | 如果标识符已定义为宏，编译后续代码块 | C89 |
| `#ifndef 标识符` | 如果标识符未定义为宏，编译后续代码块 | C89 |

#### 分支指令

| 指令 | 说明 | 版本 |
|------|------|------|
| `#elif 表达式` | 否则如果表达式非零 | C89 |
| `#elifdef 标识符` | 否则如果标识符已定义 | C23 |
| `#elifndef 标识符` | 否则如果标识符未定义 | C23 |
| `#else` | 否则（无条件编译） | C89 |

#### 结束指令

| 指令 | 说明 |
|------|------|
| `#endif` | 结束条件编译块 |

### 条件表达式规则

#### `#if` 和 `#elif` 表达式

表达式必须是**常量表达式**，遵循以下规则：

1. **标识符替换**：未使用 `#define` 定义的非字面量标识符替换为 `0`（`true` 例外，C23 起求值为 `1`）
2. **`defined` 运算符**：
   - `defined 标识符` 或 `defined(标识符)`
   - 如果标识符已定义为宏，返回 `1`；否则返回 `0`
3. **特性检测运算符**（C23 起）：`__has_include`、`__has_embed`、`__has_c_attribute` 被视为已定义的宏名

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

```c
#if OUTER_COND
    #if INNER_COND
        // 内部条件块独立处理
    #endif
#elif ANOTHER_COND
    // ...
#endif
```

### 表达式求值

预处理条件表达式的求值规则：

1. 所有宏展开在表达式求值之前完成
2. 未定义的标识符（非 `defined` 操作数）替换为 `0`
3. 表达式在预处理阶段求值，只能使用常量和已定义的宏

### DR 412 变更

| 版本 | 行为 |
|------|------|
| DR 412 前 | `#elif` 的表达式即使在被跳过的分支中也必须是有效表达式 |
| DR 412 后 | 被跳过的 `#elif` 分支的表达式不会被求值，不必有效 |

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 头文件保护

```c
#ifndef MY_HEADER_H
#define MY_HEADER_H

// 头文件内容

#endif // MY_HEADER_H
```

#### 2. 平台适配

```c
#if defined(_WIN32)
    // Windows 平台代码
#elif defined(__linux__)
    // Linux 平台代码
#elif defined(__APPLE__)
    // macOS 平台代码
#else
    #error "Unsupported platform"
#endif
```

#### 3. 编译配置

```c
#ifdef DEBUG
    #define LOG(msg) printf("[DEBUG] %s\n", msg)
#else
    #define LOG(msg) // 发布版本移除日志
#endif
```

#### 4. 版本检测（C23 起）

```c
#if __has_include(<stdatomic.h>)
    #include <stdatomic.h>
    #define HAS_ATOMICS 1
#else
    // 提供替代实现
#endif
```

#### 5. 特性检测（C23 起）

```c
#if __has_c_attribute(fallthrough)
    #define FALLTHROUGH [[fallthrough]]
#else
    #define FALLTHROUGH
#endif
```

### 最佳实践

1. **使用 `#if defined()` 替代 `#ifdef`**：在复杂条件中更清晰
2. **优先使用 `#elif` 而非嵌套 `#else #if`**：减少嵌套层级
3. **为 `#endif` 添加注释**：标明对应的条件开始
4. **使用 `#error` 处理不支持的情况**：提供明确的编译错误信息

### 常见陷阱

#### 陷阱 1：忘记 `#endif`

```c
#ifdef FEATURE_A
    // 代码
// 错误：缺少 #endif，导致后续代码被意外跳过
```

#### 陷阱 2：条件表达式中的未定义标识符

```c
#if VERSION == 2  // 如果 VERSION 未定义，替换为 0，条件为假
    // 可能不符合预期
#endif
```

#### 陷阱 3：C23 前版本使用 `#elifdef`

```c
// C23 前版本不支持 #elifdef
#ifdef A
    // ...
#elifdef B  // 可能不被支持或作为扩展支持
    // ...
#endif

// 兼容写法
#ifdef A
    // ...
#elif defined(B)
    // ...
#endif
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：基本条件编译

```c
#include <stdio.h>

#define ABCD 2

int main(void)
{
#ifdef ABCD
    printf("1: yes\n");
#else
    printf("1: no\n");
#endif

#ifndef ABCD
    printf("2: no1\n");
#elif ABCD == 2
    printf("2: yes\n");
#else
    printf("2: no2\n");
#endif

#if !defined(DCBA) && (ABCD < 2 * 4 - 3)
    printf("3: yes\n");
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

```c
#include <stdio.h>

#define DEBUG 1
#define VERSION 3

int main(void)
{
#if defined(DEBUG) && DEBUG > 0
    printf("Debug mode enabled\n");
#endif

#if defined(VERSION) && VERSION >= 2
    printf("Version 2 or later\n");
#else
    printf("Version 1 or undefined\n");
#endif

#if !defined(UNDEFINED_MACRO)
    printf("UNDEFINED_MACRO is not defined\n");
#endif

    return 0;
}
```

### 高级用法

#### 示例 3：C23 新指令 `#elifdef`/`#elifndef`

```c
#include <stdio.h>

int main(void)
{
// C23 指令 #elifdef/#elifndef
#ifdef CPU
    printf("4: no1\n");
#elifdef GPU
    printf("4: no2\n");
#elifndef RAM
    printf("4: yes\n");  // C23 模式下选择此分支
#else
    printf("4: no3\n");  // C23 前模式可能选择此分支
#endif

    return 0;
}
```

**输出**：
```
4: yes
```

#### 示例 4：特性检测（C23）

```c
#include <stdio.h>

int main(void)
{
#if __has_include(<stdatomic.h>)
    #include <stdatomic.h>
    printf("Atomic operations supported\n");
#else
    printf("Atomic operations not available\n");
#endif

#if __has_c_attribute(noreturn)
    #define NORETURN [[noreturn]]
#else
    #define NORETURN
#endif

    return 0;
}
```

#### 示例 5：复杂条件组合

```c
#include <stdio.h>

#define PLATFORM_WINDOWS 1
#define FEATURE_X 1
#define VERSION 2

int main(void)
{
#if PLATFORM_WINDOWS && VERSION >= 2
    printf("Windows platform with version 2+\n");
#elif !PLATFORM_WINDOWS && defined(FEATURE_X)
    printf("Non-Windows with FEATURE_X\n");
#elif PLATFORM_WINDOWS && !defined(FEATURE_X)
    printf("Windows without FEATURE_X\n");
#else
    printf("Default configuration\n");
#endif

    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：条件表达式语法错误

```c
// 错误：#ifdef 不支持表达式
#ifdef VERSION > 1
    // ...
#endif

// 修正：使用 #if
#if VERSION > 1
    // ...
#endif
```

#### 错误示例 2：嵌套条件未正确关闭

```c
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

#### 错误示例 3：DR 412 前的表达式问题

```c
// DR 412 前版本：如果 VERSION 未定义，#elif 表达式无效
#ifdef VERSION
    // ...
#elif VERSION > 1  // 错误：VERSION 未定义时可能报错
    // ...
#endif

// 修正：使用 defined() 保护
#ifdef VERSION
    // ...
#elif defined(VERSION) && VERSION > 1
    // ...
#endif
```

## 7. 总结 (Summary)

### 核心要点

1. **指令体系**：`#if`/`#ifdef`/`#ifndef` 开始条件块，`#elif`/`#else` 提供分支，`#endif` 结束
2. **表达式求值**：未定义标识符替换为 `0`，`defined` 运算符检测宏定义
3. **C23 新特性**：`#elifdef`、`#elifndef` 简化了条件语法
4. **嵌套支持**：条件块可嵌套，每层独立处理

### 指令对比

| 指令 | 用途 | C23 新增 |
|------|------|----------|
| `#if` | 条件表达式测试 | 否 |
| `#ifdef` | 宏定义检测 | 否 |
| `#ifndef` | 宏未定义检测 | 否 |
| `#elif` | 否则如果（表达式） | 否 |
| `#elifdef` | 否则如果已定义 | 是 |
| `#elifndef` | 否则如果未定义 | 是 |
| `#else` | 否则 | 否 |
| `#endif` | 结束条件块 | 否 |

### 学习建议

1. **理解执行时机**：条件编译在预处理阶段执行，早于编译
2. **掌握 `defined` 运算符**：是复杂条件表达式的关键
3. **注意 C23 兼容性**：新指令可能不被旧编译器支持
4. **遵循代码规范**：正确缩进、添加注释、避免过深嵌套

### 缺陷报告

| 编号 | 版本 | 问题 | 修正 |
|------|------|------|------|
| DR 412 | C89 | 失败的 `#elif` 表达式仍需有效 | 失败的 `#elif` 被跳过 |

---

**标准参考**：
- C23 (ISO/IEC 9899:2024): 6.10.1 Conditional inclusion
- C17 (ISO/IEC 9899:2018): 6.10.1 Conditional inclusion (p: 118-119)
- C11 (ISO/IEC 9899:2011): 6.10.1 Conditional inclusion (p: 162-164)
- C99 (ISO/IEC 9899:1999): 6.10.1 Conditional inclusion (p: 147-149)
- C89/C90 (ISO/IEC 9899:1990): 3.8.1 Conditional inclusion