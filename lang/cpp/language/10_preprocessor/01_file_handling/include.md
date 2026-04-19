# C++ 源文件包含 (Source File Inclusion)

## 1. 概述 (Overview)

源文件包含（Source File Inclusion）是 C++ 预处理器的重要功能，通过 `#include` 指令将另一个源文件的内容插入到当前源文件中。插入位置在指令所在行的下一行开始。

C++ 语言提供两种主要的包含语法：
- **尖括号形式** `#include <header>`：用于包含系统头文件
- **双引号形式** `#include "file"`：用于包含用户头文件

C++17 标准引入了 `__has_include` 运算符，用于在预处理阶段检测头文件或源文件是否存在。C++20 模块系统中，`#include` 可能被转换为 `import` 指令。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`#include` 指令继承自 C 语言，是模块化编程的基础。它允许将代码分散在多个文件中，通过头文件声明接口，实现代码的组织和复用。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 继承 C 语言的 `#include` 指令，支持 `<...>` 和 `"..."` 两种形式 |
| C++11 | 无重大变更 |
| C++14 | 无重大变更 |
| C++17 | 引入 `__has_include` 运算符，用于检测文件是否存在 |
| C++20 | 模块系统引入，`#include` 可能被转换为 `import` 指令 |
| C++23 | 字符集术语从"源字符集"变为"翻译字符集"；`__has_include` 被 `#elifdef`/`#elifndef` 视为已定义宏 |

### 设计动机

文件包含机制解决了以下核心问题：

1. **模块化开发**：将代码分离到多个文件，提高组织性
2. **接口声明**：通过头文件共享类型定义和函数声明
3. **代码复用**：标准库和其他库的头文件可被多个程序使用
4. **条件包含**：结合条件编译实现跨平台支持和特性检测
5. **模块过渡**：C++20 模块系统提供更现代化的代码组织方式

## 3. 语法与参数 (Syntax and Parameters)

### `#include` 指令语法

| 语法形式 | 说明 | 版本 |
|----------|------|------|
| `#include <h-char-sequence>` | (1) 尖括号形式，搜索系统头文件 | C++98 |
| `#include "q-char-sequence"` | (2) 双引号形式，搜索用户文件 | C++98 |
| `#include pp-tokens` | (3) 宏替换形式 | C++98 |

### `__has_include` 运算符语法（C++17）

| 语法形式 | 说明 | 版本 |
|----------|------|------|
| `__has_include("q-char-sequence")` | (4) 检测用户文件是否存在 | C++17 |
| `__has_include(<h-char-sequence>)` | (4) 检测系统头文件是否存在 | C++17 |
| `__has_include(string-literal)` | (5) 字符串字面量形式 | C++17 |
| `__has_include(<h-pp-tokens>)` | (5) 预处理标记形式 | C++17 |

### 参数详解

#### h-char-sequence（h 字符序列）

一个或多个 h-char 的序列。包含以下字符是**条件支持**的，语义由实现定义：
- 单引号 `'`
- 双引号 `"`
- 反斜杠 `\`
- 注释序列 `//` 或 `/*`

**h-char**：源字符集（C++23 起：翻译字符集）中除换行符和 `>` 外的任何字符。

#### q-char-sequence（q 字符序列）

一个或多个 q-char 的序列。包含以下字符是**条件支持**的，语义由实现定义：
- 单引号 `'`
- 反斜杠 `\`
- 注释序列 `//` 或 `/*`

**q-char**：源字符集（C++23 起：翻译字符集）中除换行符和 `"` 外的任何字符。

#### pp-tokens

一个或多个预处理标记（preprocessing tokens）。

#### h-pp-tokens

一个或多个预处理标记，但不包含 `>`。

### 搜索规则

#### 尖括号形式 `#include <header>`

1. 在实现定义的位置序列中搜索由 h-char-sequence 唯一标识的头文件
2. 搜索方式和位置指定是实现定义的
3. 设计意图：搜索实现控制的文件（系统头文件）
4. 典型实现：仅搜索标准包含目录
5. 标准 C++ 库和标准 C 库隐式包含在这些目录中
6. 用户可通过编译器选项控制标准包含目录

#### 双引号形式 `#include "file"`

1. 以实现定义的方式搜索由 q-char-sequence 标识的源文件
2. 如果搜索不支持或搜索失败，回退到尖括号形式重新处理
3. 设计意图：搜索非实现控制的文件（用户头文件）
4. 典型实现：首先搜索当前文件所在目录，再搜索标准包含目录

#### 宏替换形式 `#include pp-tokens`

1. 如果不匹配 (1) 或 (2)，pp-tokens 进行宏替换
2. 替换后的指令重新尝试匹配 (1) 或 (2)
3. 如果替换后仍不匹配，行为未定义
4. 将 `<...>` 或 `"..."` 内的标记组合为头文件名的方式是实现定义的

### `__has_include` 运算符（C++17）

#### 功能

检测头文件或源文件是否可用于包含。

#### 返回值

- 文件存在：返回 `1`
- 文件不存在：返回 `0`

#### 使用限制

- 可用于 `#if` 和 `#elif` 的表达式中
- 被 `#ifdef`、`#ifndef`、`#elifdef`（C++23）、`#elifndef`（C++23）和 `defined` 视为已定义的宏
- 不能在其他地方使用

#### 注意事项

`__has_include` 返回 `1` 仅表示指定名称的头文件或源文件存在，不保证：
- 包含该文件不会导致错误
- 该文件包含任何有用内容

**示例**：支持 C++14 和 C++17 模式的实现可能在 C++14 模式下 `__has_include(<optional>)` 返回 `1`，但实际 `#include <optional>` 可能导致错误。

### 模块系统交互（C++20）

如果头文件名标识的是一个**可导入头文件**（importable header），实现可以决定将 `#include` 指令替换为以下形式的 `import` 指令：

```cpp
import header-name ;
```

这是实现定义的行为。

## 4. 底层原理 (Underlying Principles)

### 处理流程

```
遇到 #include 指令
    ↓
解析指令形式
    ↓
┌─────────────────────────────────────┐
│ 尖括号形式 │ 双引号形式 │ 宏替换形式 (3) │
└─────────────────────────────────────┘
    ↓           ↓              ↓
搜索系统    搜索当前目录    宏替换
包含目录    再搜索系统      ↓
    ↓           ↓         重新匹配
找到文件？   找到文件？       ↓
    ↓           ↓           搜索文件
读取文件内容 读取文件内容    找到文件？
    ↓           ↓              ↓
翻译阶段 1-4  翻译阶段 1-4   读取文件内容
    ↓           ↓              ↓
递归处理     递归处理       翻译阶段 1-4
嵌套 #include              ↓
    ↓                     递归处理
替换完成                    ↓
                          替换完成
```

### 翻译阶段处理

被包含的文件经过翻译阶段 1-4 的处理：

1. **阶段 1**：物理源文件字符映射到源字符集（C++23 起：翻译字符集）
2. **阶段 2**：反斜杠换行符拼接
3. **阶段 3**：分解为预处理标记和空白字符
4. **阶段 4**：执行预处理指令，展开宏

### 递归包含

被包含文件中的 `#include` 指令会被递归处理，直到实现定义的嵌套限制。

### 头文件保护机制

#### 传统头文件保护

```cpp
#ifndef MYHEADER_H
#define MYHEADER_H

// 头文件内容

#endif /* MYHEADER_H */
```

#### `#pragma once`（非标准扩展）

许多编译器支持非标准扩展 `#pragma once`：
- 如果同一文件（以操作系统特定方式判断）已被包含，则跳过处理
- 相比传统头文件保护更简单，但不是标准

### 转义序列处理

q-char-sequence 或 h-char-sequence 中类似转义序列的字符序列可能导致：
- 错误
- 被解释为转义序列对应的字符
- 具有完全不同的含义

具体行为取决于实现。

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 包含标准库头文件

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
```

#### 2. 包含用户自定义头文件

```cpp
#include "myclass.h"
#include "utils/helper.h"
```

#### 3. 条件包含头文件（C++17）

```cpp
#if __has_include(<optional>)
    #include <optional>
    #define HAS_OPTIONAL 1
#else
    #define HAS_OPTIONAL 0
#endif
```

#### 4. 使用宏构建包含路径

```cpp
#define HEADER_PATH "config.h"
#include HEADER_PATH

// 或更复杂的形式
#define MAKE_PATH(name) <lib/name.hpp>
#include MAKE_PATH(utils)
```

#### 5. 跨平台头文件选择

```cpp
#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif
```

#### 6. 特性检测与回退

```cpp
#if __has_include(<optional>)
    #include <optional>
    template<class T>
    using Optional = std::optional<T>;
#elif __has_include(<experimental/optional>)
    #include <experimental/optional>
    template<class T>
    using Optional = std::experimental::optional<T>;
#else
    // 提供自定义实现
#endif
```

### 最佳实践

1. **系统头文件用尖括号**：`#include <vector>`
2. **用户头文件用双引号**：`#include "myclass.hpp"`
3. **使用头文件保护**：防止重复包含
4. **头文件放在源文件顶部**：提高可读性
5. **避免循环包含**：A 包含 B，B 包含 A
6. **优先使用模块（C++20）**：模块提供更好的封装和编译性能

### 常见陷阱

#### 陷阱 1：重复包含

```cpp
// file1.cpp
#include "header.h"
#include "header.h"  // 重复包含，可能导致编译错误

// 解决方案：使用头文件保护
#ifndef HEADER_H
#define HEADER_H
// 内容
#endif
```

#### 陷阱 2：循环包含

```cpp
// a.h
#include "b.h"

// b.h
#include "a.h"  // 循环包含，编译失败

// 解决方案：前置声明
// a.h
class B;  // 前置声明
class A { B* b; };

// b.h
class A;  // 前置声明
class B { A* a; };
```

#### 陷阱 3：包含路径错误

```cpp
// 错误：使用尖括号包含用户头文件
#include "myheader.h"  // 正确：双引号先搜索当前目录
#include <myheader.h>  // 可能失败：仅搜索系统目录
```

#### 陷阱 4：`__has_include` 误用

```cpp
// 错误：在条件编译外使用
int result = __has_include(<iostream>);  // 错误

// 修正：在 #if 或 #elif 中使用
#if __has_include(<iostream>)
    #include <iostream>
#endif
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：包含标准库头文件

```cpp
#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<std::string> words = {"Hello", "World"};
    for (const auto& word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
    return 0;
}
```

#### 示例 2：包含用户头文件

```cpp
// myclass.h
#ifndef MYCLASS_H
#define MYCLASS_H

class MyClass {
public:
    void doSomething();
};

#endif /* MYCLASS_H */

// main.cpp
#include <iostream>
#include "myclass.h"

void MyClass::doSomething() {
    std::cout << "Doing something..." << std::endl;
}

int main()
{
    MyClass obj;
    obj.doSomething();
    return 0;
}
```

### 高级用法

#### 示例 3：`__has_include` 特性检测（C++17）

```cpp
#if __has_include(<optional>)
    #include <optional>
    #define has_optional 1
    template<class T>
    using optional_t = std::optional<T>;
#elif __has_include(<experimental/optional>)
    #include <experimental/optional>
    #define has_optional -1
    template<class T>
    using optional_t = std::experimental::optional<T>;
#else
    #define has_optional 0
    template<class V>
    class optional_t
    {
        V v{};
        bool has{};

    public:
        optional_t() = default;
        optional_t(V&& v) : v(v), has{true} {}
        V value_or(V&& alt) const&
        {
            return has ? v : alt;
        }
    };
#endif

#include <iostream>

int main()
{
    if (has_optional > 0)
        std::cout << "<optional> is present\n";
    else if (has_optional < 0)
        std::cout << "<experimental/optional> is present\n";
    else
        std::cout << "<optional> is not present\n";

    optional_t<int> op;
    std::cout << "op = " << op.value_or(-1) << '\n';
    op = 42;
    std::cout << "op = " << op.value_or(-1) << '\n';
}
```

**输出**：
```
<optional> is present
op = -1
op = 42
```

#### 示例 4：宏替换形式

```cpp
#include <iostream>

// 使用宏定义头文件路径
#define CONFIG_HEADER "config.h"
#include CONFIG_HEADER

// 更复杂的形式：根据平台选择
#if defined(_WIN32)
    #define PLATFORM_HEADER <windows.h>
#else
    #define PLATFORM_HEADER <unistd.h>
#endif
#include PLATFORM_HEADER

int main()
{
    std::cout << "Platform header included successfully\n";
    return 0;
}
```

#### 示例 5：条件包含跨平台头文件

```cpp
#include <iostream>

// 跨平台头文件选择
#if defined(_WIN32)
    #include <windows.h>
    #define SLEEP_MS(ms) Sleep(ms)
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
    #define SLEEP_MS(ms) usleep((ms) * 1000)
#else
    #define SLEEP_MS(ms)
#endif

// 可选特性检测
#if __has_include(<thread>)
    #include <thread>
    #define HAS_THREAD 1
#else
    #define HAS_THREAD 0
#endif

int main()
{
    std::cout << "Waiting 1 second...\n";
    SLEEP_MS(1000);
    std::cout << "Done!\n";
    std::cout << "std::thread support: " << HAS_THREAD << "\n";
    return 0;
}
```

#### 示例 6：头文件保护模式

```cpp
// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace config {
    constexpr const char* APP_NAME = "MyApp";
    constexpr const char* APP_VERSION = "1.0.0";

    // 平台检测
    #if defined(_WIN32)
        constexpr const char* PLATFORM = "Windows";
    #elif defined(__linux__)
        constexpr const char* PLATFORM = "Linux";
    #elif defined(__APPLE__)
        constexpr const char* PLATFORM = "macOS";
    #else
        constexpr const char* PLATFORM = "Unknown";
    #endif
}

#endif /* CONFIG_H */

// main.cpp
#include <iostream>
#include "config.h"

int main()
{
    std::cout << config::APP_NAME << " v" << config::APP_VERSION << "\n";
    std::cout << "Platform: " << config::PLATFORM << "\n";
    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：重复包含导致重定义

```cpp
// header.h（无保护）
struct Point { int x, y; };

// main.cpp
#include "header.h"
#include "header.h"  // 错误：结构体重定义

// 修正：添加头文件保护
#ifndef HEADER_H
#define HEADER_H
struct Point { int x, y; };
#endif
```

#### 错误示例 2：循环包含

```cpp
// a.h
#include "b.h"
class A { B* b; };

// b.h
#include "a.h"  // 循环包含
class B { A* a; };

// 修正：使用前置声明
// a.h
class B;  // 前置声明
class A { B* b; };

// b.h
class A;  // 前置声明
class B { A* a; };
```

#### 错误示例 3：宏替换后不匹配

```cpp
// 错误：宏替换后不匹配任何形式
#define INVALID_HEADER something_weird
#include INVALID_HEADER  // 未定义行为

// 修正：确保宏替换后匹配有效形式
#define VALID_HEADER <iostream>
#include VALID_HEADER
```

## 7. 总结 (Summary)

### 核心要点

1. **两种形式**：尖括号搜索系统目录，双引号先搜索当前目录
2. **宏替换**：`#include` 后的标记可进行宏替换
3. **递归处理**：被包含文件经过翻译阶段 1-4，支持嵌套包含
4. **头文件保护**：使用 `#ifndef`/`#define`/`#endif` 或 `#pragma once` 防止重复包含
5. **C++17 特性**：`__has_include` 运算符检测文件是否存在
6. **C++20 模块**：`#include` 可能被转换为 `import` 指令

### 语法对比

| 语法 | 搜索顺序 | 适用场景 |
|------|----------|----------|
| `#include <header>` | 仅系统目录 | 标准库、系统头文件 |
| `#include "file"` | 当前目录 → 系统目录 | 用户头文件 |

### `__has_include` 使用限制

| 位置 | 可用性 |
|------|--------|
| `#if` 表达式 | ✅ 可用 |
| `#elif` 表达式 | ✅ 可用 |
| `defined` 运算符 | ✅ 视为已定义宏 |
| `#ifdef`/`#ifndef` | ✅ 视为已定义宏 |
| 其他位置 | ❌ 不可用 |

### C++ 与 C 的差异

| 特性 | C | C++ |
|------|---|-----|
| `__has_include` | C23 引入 | C++17 引入 |
| 模块系统 | 无 | C++20 引入 |
| 字符集术语 | 源字符集 | C++23 起改为翻译字符集 |
| 特殊字符处理 | 未定义行为 | 条件支持（CWG 787） |

### 学习建议

1. **理解搜索路径**：掌握编译器如何查找头文件
2. **正确选择语法**：系统头文件用 `<>`，用户头文件用 `""`
3. **使用头文件保护**：防止重复包含和编译错误
4. **避免循环包含**：使用前置声明或重新设计
5. **利用 C++17 特性**：`__has_include` 可实现灵活的条件包含
6. **考虑模块系统**：C++20 模块提供更好的封装和性能

### 缺陷报告

| 编号 | 版本 | 问题 | 修正 |
|------|------|------|------|
| CWG 787 | C++98 | q-char-sequence 或 h-char-sequence 中出现转义序列的行为未定义 | 改为条件支持 |

---

**相关参考**：
- C++ 标准库头文件列表
- C++ 模块系统文档