# C++ 诊断指令 (Diagnostic Directives)

## 1. 概述 (Overview)

诊断指令（Diagnostic Directives）是 C++ 预处理器的功能，允许开发者在预处理阶段生成编译时诊断信息。C++ 语言提供两种诊断指令：

- **`#error`**：显示错误消息并使程序非法，终止编译
- **`#warning`**（C++23）：显示警告消息但不影响程序有效性，编译继续

这些指令通常用于在编译时检测不满足条件的环境或配置，提供清晰的错误提示。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`#error` 指令从 C++ 早期就存在，继承自 C 语言。它解决了编译时配置验证的需求，让开发者能够在不满足前提条件时提供清晰的错误信息。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 继承 C 语言的 `#error` 指令 |
| C++03 | 无重大变更 |
| C++11 | 无重大变更 |
| C++14 | 无重大变更 |
| C++17 | 无重大变更 |
| C++20 | 无重大变更 |
| C++23 | 标准化 `#warning` 指令 |

### 设计动机

诊断指令解决了以下核心问题：

1. **编译时验证**：检测编译环境是否满足要求
2. **配置检查**：验证必要的宏定义是否存在
3. **平台适配**：确保目标平台被正确识别
4. **版本兼容**：检测语言版本是否满足要求
5. **错误提示**：提供清晰的错误信息，帮助开发者快速定位问题

### `#warning` 的历史

在 C++23 标准化之前，`#warning` 已被许多编译器作为一致扩展（conforming extension）提供，在所有编译模式下都可用。C++23 将其正式标准化。

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

| 语法 | 说明 | 版本 |
|------|------|------|
| `#error diagnostic-message` | (1) 错误指令，终止编译 | C++98 |
| `#warning diagnostic-message` | (2) 警告指令，继续编译 | C++23 |

### 参数说明

#### diagnostic-message（诊断消息）

- 可以由多个单词组成
- 不需要用引号包围
- 可以包含任何文本内容
- 在 `#error` 或 `#warning` 后到行末的所有内容都是消息的一部分

### 行为说明

#### `#error` 指令

1. 预处理器遇到 `#error` 指令后显示诊断消息
2. 程序被标记为非法（ill-formed）
3. 编译停止

#### `#warning` 指令（C++23）

1. 预处理器显示警告消息
2. 程序的有效性不受影响
3. 编译继续进行

## 4. 底层原理 (Underlying Principles)

### 处理流程

```
预处理阶段
    ↓
遇到 #error 或 #warning 指令
    ↓
┌─────────────────────────────────┐
│   #error          │   #warning  │
└─────────────────────────────────┘
    ↓                    ↓
显示错误消息         显示警告消息
    ↓                    ↓
程序标记为非法       程序继续编译
    ↓                    ↓
编译终止             编译继续
```

### 执行时机

诊断指令在**预处理阶段**执行，这是 C++ 语言翻译过程的第 4 阶段：

1. **阶段 1-3**：字符处理、行拼接、标记化
2. **阶段 4**：预处理指令执行（包括 `#error`/`#warning`）
3. **阶段 5-8**：编译、模板实例化、链接

### 消息处理

诊断消息的处理方式：
- 消息内容直接传递给编译器的诊断系统
- 实现决定消息的具体显示格式
- 通常包含源文件名和行号信息

### 与条件编译配合

诊断指令通常与条件编译配合使用：

```cpp
#if CONDITION
    // 满足条件的代码
#else
    #error "CONDITION not met"
#endif
```

### 与模块系统交互（C++20）

在 C++20 模块系统中，诊断指令的行为与头文件相同：
- 可以在模块接口中使用
- 可以在模块实现单元中使用
- 在全局模块片段中有效

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 实现环境检查

```cpp
#if __STDC_HOSTED__ != 1
    #error "Not a hosted implementation"
#endif
```

#### 2. 平台检测

```cpp
#if !defined(_WIN32) && !defined(__linux__) && !defined(__APPLE__)
    #error "Unsupported platform. Supported: Windows, Linux, macOS"
#endif
```

#### 3. 版本检查

```cpp
#if __cplusplus < 201103L
    #error "This code requires C++11 or later"
#endif
```

#### 4. 配置验证

```cpp
#ifndef BUFFER_SIZE
    #error "BUFFER_SIZE must be defined"
#endif

#if BUFFER_SIZE < 64 || BUFFER_SIZE > 4096
    #error "BUFFER_SIZE must be between 64 and 4096"
#endif
```

#### 5. 必需宏定义

```cpp
#ifndef PROJECT_NAME
    #error "PROJECT_NAME must be defined via -D option"
#endif
```

#### 6. 编译时警告（C++23）

```cpp
#if DEBUG_LEVEL > 2
    #warning "High debug level may affect performance"
#endif
```

#### 7. C++ 特性检查

```cpp
#if __cpp_exceptions < 199711L
    #error "Exception support required"
#endif

#if __cpp_rtti < 199711L
    #error "RTTI support required"
#endif
```

### 最佳实践

1. **提供清晰的消息**：说明问题所在和解决方法
2. **使用有意义的条件**：确保检查的条件确实重要
3. **结合条件编译**：只在特定条件下触发
4. **避免过度使用**：仅在必要时使用，不要滥用
5. **文档说明**：在代码注释中解释触发条件

### 常见陷阱

#### 陷阱 1：条件始终为真

```cpp
// 错误：条件永远不会触发
#if 1
    #error "This will always fail"  // 始终会报错
#endif
```

#### 陷阱 2：依赖未定义的宏

```cpp
// 警告：MACRO_NAME 未定义时，条件可能不按预期工作
#if MACRO_NAME != 1
    #error "MACRO_NAME must be defined as 1"
#endif

// 修正：先检查是否定义
#if !defined(MACRO_NAME) || MACRO_NAME != 1
    #error "MACRO_NAME must be defined as 1"
#endif
```

#### 陷阱 3：消息格式问题

```cpp
// 某些编译器可能对特殊字符处理不同
#error This is an error message with "quotes"  // 可能有问题

// 建议：使用一致的消息格式
#error "This is an error message with quotes"
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：宿主实现检查

```cpp
#if __STDC_HOSTED__ != 1
#   error "Not a hosted implementation"
#endif

#if __cplusplus >= 202302L
#   warning "Using #warning as a standard feature"
#endif

#include <iostream>

int main()
{
    std::cout << "The implementation used is hosted\n";
}
```

#### 示例 2：版本检查

```cpp
#if __cplusplus < 201703L
    #error "This library requires C++17 or later"
#endif

#include <iostream>
#include <optional>

int main()
{
    std::optional<int> opt = 42;
    std::cout << "Value: " << opt.value() << std::endl;
    return 0;
}
```

### 高级用法

#### 示例 3：C++23 `#warning` 指令

```cpp
// 版本特性检测
#if __cplusplus >= 202302L
    #define CPP23_OR_LATER 1
#else
    #define CPP23_OR_LATER 0
#endif

#if CPP23_OR_LATER
    #warning "Using C++23 features - ensure compatibility"
#endif

#include <iostream>

int main()
{
    std::cout << "C++23 or later: " << CPP23_OR_LATER << std::endl;
    return 0;
}
```

#### 示例 4：平台适配检查

```cpp
/* 平台检测 */
#if defined(_WIN32)
    #define PLATFORM_NAME "Windows"
#elif defined(__linux__)
    #define PLATFORM_NAME "Linux"
#elif defined(__APPLE__)
    #define PLATFORM_NAME "macOS"
#else
    #error "Unsupported platform. Supported: Windows, Linux, macOS"
#endif

/* 架构检测 */
#if !defined(__x86_64__) && !defined(__i386__) && !defined(__arm__) && !defined(__aarch64__)
    #warning "Untested architecture, compilation may fail"
#endif

#include <iostream>

int main()
{
    std::cout << "Running on " << PLATFORM_NAME << " platform" << std::endl;
    return 0;
}
```

#### 示例 5：特性检测与配置验证

```cpp
/* C++ 特性检测 */
#if !defined(__cpp_exceptions) || __cpp_exceptions < 199711L
    #error "Exception support is required for this library"
#endif

#if !defined(__cpp_rtti) || __cpp_rtti < 199711L
    #error "RTTI support is required for this library"
#endif

/* 配置验证 */
#ifndef THREAD_COUNT
    #define THREAD_COUNT 4
#endif

#if THREAD_COUNT < 1
    #error "THREAD_COUNT must be at least 1"
#elif THREAD_COUNT > 64
    #error "THREAD_COUNT cannot exceed 64"
#elif THREAD_COUNT > 16
    #warning "Large thread count may cause performance issues"
#endif

/* 调试模式警告 */
#ifdef NDEBUG
    #warning "Debug assertions are disabled"
#endif

#include <iostream>
#include <stdexcept>

int main()
{
    try {
        std::cout << "Thread count: " << THREAD_COUNT << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

#### 示例 6：模块兼容性检查（C++20）

```cpp
// 全局模块片段中的检查
module;

#if __cplusplus < 202002L
    #error "Modules require C++20 or later"
#endif

#if defined(_WIN32) && !defined(_WIN64)
    #warning "32-bit Windows support is deprecated"
#endif

export module mymodule;

import <iostream>;

export void hello() {
    std::cout << "Hello from module!\n";
}
```

### 常见错误及修正

#### 错误示例 1：错误的条件表达式

```cpp
// 错误：FEATURE_FLAG 未定义时，!= 比较可能不按预期工作
#if FEATURE_FLAG != 1
    #error "FEATURE_FLAG must be 1"
#endif

// 修正：使用 defined() 检查
#if !defined(FEATURE_FLAG)
    #error "FEATURE_FLAG must be defined"
#elif FEATURE_FLAG != 1
    #error "FEATURE_FLAG must be 1"
#endif
```

#### 错误示例 2：过度使用 `#error`

```cpp
// 不推荐：过度检查
#ifndef FEATURE_A
    #error "FEATURE_A not defined"
#endif
#ifndef FEATURE_B
    #error "FEATURE_B not defined"
#endif
// ... 更多检查

// 推荐：合并检查并提供清晰说明
#if !defined(FEATURE_A) || !defined(FEATURE_B)
    #error "Required features missing. Define FEATURE_A and FEATURE_B"
#endif
```

#### 错误示例 3：在非条件上下文中使用

```cpp
// 错误：无条件错误，代码永远无法编译
#error "This code is incomplete"

// 修正：只在特定条件下触发
#ifndef ENABLE_EXPERIMENTAL
    #error "This feature is experimental. Define ENABLE_EXPERIMENTAL to use"
#endif
```

## 7. 总结 (Summary)

### 核心要点

1. **两种指令**：`#error` 终止编译，`#warning`（C++23）仅显示警告
2. **执行时机**：预处理阶段（翻译阶段 4）
3. **消息格式**：不需要引号，可包含多个单词
4. **典型用途**：编译时验证、配置检查、平台适配
5. **C++23 新特性**：`#warning` 正式标准化

### 指令对比

| 特性 | `#error` | `#warning` |
|------|----------|------------|
| 引入版本 | C++98 | C++23 |
| 编译影响 | 终止编译 | 继续编译 |
| 程序有效性 | 标记为非法 | 不影响 |
| 典型用途 | 必须满足的条件 | 提醒和提示 |

### 使用场景对比

| 场景 | 推荐指令 | 说明 |
|------|----------|------|
| 必需配置缺失 | `#error` | 必须终止编译 |
| 版本不兼容 | `#error` | 程序无法正确运行 |
| 平台不支持 | `#error` | 编译无意义 |
| 性能警告 | `#warning` | 仅提醒，不影响功能 |
| 实验功能 | `#warning` | 提醒用户注意 |
| 弃用提醒 | `#warning` | 未来版本可能移除 |

### C++ 与 C 的差异

| 特性 | C | C++ |
|------|---|-----|
| `#warning` 标准化版本 | C23 | C++23 |
| 特性检测宏 | 有限 | `__cpp_*` 宏更丰富 |
| 模块系统 | 无 | C++20 起支持 |

### 学习建议

1. **理解时机**：诊断指令在预处理阶段执行，早于编译
2. **清晰消息**：提供明确的错误原因和解决建议
3. **合理使用**：`#error` 用于必须终止的情况，`#warning` 用于提示
4. **版本兼容**：C++23 前版本中 `#warning` 可能不被支持
5. **结合文档**：在代码注释中说明触发条件
6. **利用特性宏**：使用 `__cpp_*` 宏进行更精确的特性检测

---

**标准参考**：
- C++23 (ISO/IEC 14882:2024): 15.8 Error directive [cpp.error]
- C++20 (ISO/IEC 14882:2020): 15.8 Error directive [cpp.error]
- C++17 (ISO/IEC 14882:2017): 19.5 Error directive [cpp.error]
- C++14 (ISO/IEC 14882:2014): 16.5 Error directive [cpp.error]
- C++11 (ISO/IEC 14882:2011): 16.5 Error directive [cpp.error]
- C++03 (ISO/IEC 14882:2003): 16.5 Error directive [cpp.error]
- C++98 (ISO/IEC 14882:1998): 16.5 Error directive [cpp.error]