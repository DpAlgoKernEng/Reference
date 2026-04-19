# C++ 文件名和行号信息 (Filename and Line Information)

## 1. 概述 (Overview)

`#line` 指令是 C++ 预处理器的控制指令，用于修改预处理器中的当前行号和文件名。这会影响 `__LINE__` 和 `__FILE__` 预定义宏的展开结果。

### 核心功能

- **修改行号**：更改当前的预处理器行号
- **修改文件名**：更改当前的预处理器文件名
- **影响宏展开**：改变 `__LINE__` 和 `__FILE__` 宏的值

### 主要用途

`#line` 指令主要用于自动代码生成工具，这些工具从其他语言或格式生成 C++ 源代码。通过 `#line` 指令，生成的 C++ 文件可以引用原始源文件的行号和文件名，便于调试和错误定位。

### C++20 替代方案

C++20 引入了 `std::source_location` 类，提供了更现代的方式来获取源代码位置信息，减少了对 `#line` 指令的依赖。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`#line` 指令继承自 C 语言，从 C++ 早期就存在。最初是为了支持代码生成工具。当从其他语言（如 Yacc/Bison 语法文件、Lex 词法文件等）生成 C++ 代码时，`#line` 指令可以将错误信息映射回原始源文件。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 继承 C 语言的 `#line` 指令，行号上限为 32767 |
| C++11 | 行号上限提升至 2147483647 |
| C++14 | 无重大变更 |
| C++17 | 无重大变更 |
| C++20 | 引入 `std::source_location` 作为现代替代方案 |
| C++23 | 无重大变更 |

### 设计动机

`#line` 指令解决了以下核心问题：

1. **错误定位**：将错误信息映射到原始源文件
2. **调试支持**：在调试器中显示正确的源代码位置
3. **代码生成工具**：支持 Yacc、Bison、Lex 等工具
4. **日志记录**：在日志中显示原始文件的行号

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

| 语法 | 说明 |
|------|------|
| `#line lineno` | (1) 仅更改行号 |
| `#line lineno "filename"` | (2) 更改行号和文件名 |

### 参数说明

#### lineno（行号）

- 必须是至少一个十进制数字的序列
- 始终解释为十进制数（即使以 0 开头）
- 允许使用预处理标记（宏常量或表达式），只要它们展开为有效的十进制整数

**行号范围**：

| 版本 | 有效范围 | 超出范围的行为 |
|------|----------|----------------|
| C++98/C++03 | 1 - 32767 | 未定义行为 |
| C++11 起 | 1 - 2147483647 | 未定义行为 |

如果 `lineno` 为 0 或超过上限，行为未定义。

#### filename（文件名）

- 必须是有效的字符串
- 可以是字符串字面量或展开为字符串的宏
- 后续的 `__FILE__` 宏将展开为此文件名

### 行为说明

#### 形式 (1)

- 将当前预处理器行号更改为 `lineno`
- 后续的 `__LINE__` 宏将展开为 `lineno` 加上此后遇到的实际源代码行数

#### 形式 (2)

- 同时更改当前预处理器文件名
- 后续的 `__FILE__` 宏将产生 `filename`

## 4. 底层原理 (Underlying Principles)

### 处理流程

```
预处理阶段
    ↓
遇到 #line 指令
    ↓
解析参数
    ↓
┌─────────────────────────────────┐
│ 仅行号        │ 行号 + 文件名   │
└─────────────────────────────────┘
    ↓                ↓
更新内部行号    更新内部行号和文件名
    ↓                ↓
后续 __LINE__   后续 __FILE__ 和 __LINE__
宏受影响         宏受影响
```

### 行号计算

```cpp
#line 100
// __LINE__ 在此行为 100

int a;      // __LINE__ = 101
int b;      // __LINE__ = 102
// ...
```

### 与 `std::source_location` 的关系（C++20）

C++20 引入的 `std::source_location` 提供了一种更现代的方式获取源代码位置：

```cpp
#include <source_location>

void log(const std::source_location& loc = std::source_location::current())
{
    std::cout << loc.file_name() << ":" << loc.line() << "\n";
}
```

`std::source_location` 的优势：
- 类型安全
- 可在函数参数中使用
- 不需要预处理器
- 编译时计算

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 代码生成工具

Yacc/Bison 生成的解析器使用 `#line` 指令：

```cpp
/* 由 yacc 生成的代码 */
#line 42 "parser.y"
/* 错误将指向 parser.y 的第 42 行 */
```

#### 2. 模板系统

从模板生成代码时保持原始行号：

```cpp
/* 模板引擎生成 */
#line 15 "template.html"
/* HTML 模板中的代码 */
```

#### 3. 测试框架

在测试代码中控制错误消息的显示：

```cpp
#line 1 "test_case"
assert(condition);  // 错误消息显示 test_case:1
```

#### 4. 调试特定代码段

标记调试区域：

```cpp
#line 1000 "debug_section"
// 错误信息将显示 debug_section:1000
```

### 最佳实践

1. **用于代码生成工具**：这是 `#line` 的主要用途
2. **保持一致性**：如果修改行号，也要修改文件名
3. **记录原始位置**：在注释中说明为什么使用 `#line`
4. **避免滥用**：不要在手动编写的代码中随意使用
5. **考虑 `source_location`**：C++20 起优先使用 `std::source_location`

### 常见陷阱

#### 陷阱 1：行号超出范围

```cpp
#line 99999999999999999 "file.cpp"  // 未定义行为：超出范围
```

#### 陷阱 2：行号为 0

```cpp
#line 0 "file.cpp"  // 未定义行为：行号不能为 0
```

#### 陷阱 3：与 `source_location` 混淆

```cpp
// C++20: source_location 不受 #line 影响
#line 100 "fake.cpp"
auto loc = std::source_location::current();
// loc.line() 可能仍然是实际行号，而非 100
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：更改行号和文件名

```cpp
#include <cassert>
#define FNAME "test.cc"

int main()
{
#line 777 FNAME
        assert(2+2 == 5);
}
```

**输出**：
```
test: test.cc:777: int main(): Assertion `2+2 == 5' failed.
```

#### 示例 2：仅更改行号

```cpp
#include <iostream>

int main()
{
    std::cout << "Current line: " << __LINE__ << "\n";  // 输出: 6

#line 100
    std::cout << "After #line: " << __LINE__ << "\n";   // 输出: 100
    std::cout << "Next line: " << __LINE__ << "\n";      // 输出: 101

    return 0;
}
```

### 高级用法

#### 示例 3：代码生成工具模拟

```cpp
#include <iostream>
#include <cassert>

/* 模拟 Yacc/Bison 生成的代码 */

/* 原始文件: calc.y */
/* 以下是生成的 C++ 代码 */

#line 42 "calc.y"

void yyerror(const char* msg)
{
    /* 错误消息将指向 calc.y:42 */
    std::cerr << "Error at line " << __LINE__ << " in " << __FILE__
              << ": " << msg << "\n";
}

#line 100 "calc.y"

int yylex()
{
    /* 词法分析代码 */
    /* 错误消息将指向 calc.y:100 */
    std::cout << "Lexing at " << __FILE__ << ":" << __LINE__ << "\n";
    return 0;
}

int main()
{
    std::cout << "File: " << __FILE__ << ", Line: " << __LINE__ << "\n";
    yylex();
    return 0;
}
```

**输出**：
```
File: calc.y, Line: 114
Lexing at calc.y:100
```

#### 示例 4：使用宏作为参数

```cpp
#include <iostream>

#define NEW_LINE 500
#define NEW_FILE "generated.cpp"

int main()
{
    std::cout << "Before: " << __FILE__ << ":" << __LINE__ << "\n";

#line NEW_LINE NEW_FILE
    std::cout << "After: " << __FILE__ << ":" << __LINE__ << "\n";

    return 0;
}
```

**输出**：
```
Before: example.cpp:8
After: generated.cpp:500
```

#### 示例 5：`std::source_location` 对比（C++20）

```cpp
#include <iostream>
#include <source_location>

// 使用 #line 的传统方式
void log_with_line(int line = __LINE__, const char* file = __FILE__)
{
    std::cout << "[Traditional] " << file << ":" << line << "\n";
}

// 使用 source_location 的现代方式（C++20）
void log_with_source_location(
    const std::source_location& loc = std::source_location::current())
{
    std::cout << "[Modern] " << loc.file_name() << ":" << loc.line() << "\n";
}

int main()
{
#line 100 "fake.cpp"
    log_with_line();          // 使用 #line 设置的值
    log_with_source_location(); // 可能使用实际位置

    return 0;
}
```

#### 示例 6：模板引擎示例

```cpp
#include <iostream>
#include <stdexcept>

/* 模拟模板引擎生成的代码 */
/* 原始模板文件: page.template */

#line 1 "page.template"

void render_header()
{
    std::cout << "<html><head><title>Page</title></head><body>\n";
}

#line 15 "page.template"

void render_content()
{
    std::cout << "<div class=\"content\">\n";
    std::cout << "    <h1>Welcome</h1>\n";
    std::cout << "</div>\n";
}

#line 30 "page.template"

void render_footer()
{
    std::cout << "</body></html>\n";
}

// 错误处理示例
void check_condition(bool cond)
{
#line 50 "page.template"
    if (!cond) {
        throw std::runtime_error("Condition failed at " + std::string(__FILE__)
                                  + ":" + std::to_string(__LINE__));
    }
}

int main()
{
    render_header();
    render_content();
    render_footer();

    std::cout << "Template executed at " << __FILE__ << ":" << __LINE__ << "\n";

    try {
        check_condition(false);  // 将抛出指向 page.template:50 的错误
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：行号为 0

```cpp
// 错误：行号为 0 导致未定义行为
#line 0 "file.cpp"

// 修正：使用有效的行号
#line 1 "file.cpp"
```

#### 错误示例 2：行号超出范围

```cpp
// 错误：行号超过 2147483647
#line 9999999999999999999 "file.cpp"

// 修正：使用有效范围内的行号
#line 1000000 "file.cpp"
```

#### 错误示例 3：误解 `source_location` 行为

```cpp
// 错误假设：#line 会影响 source_location
#line 1000 "fake.cpp"
auto loc = std::source_location::current();
// loc.line() 可能不是 1000！

// 修正：理解 source_location 是独立的机制
// #line 主要影响 __LINE__ 和 __FILE__ 宏
```

## 7. 总结 (Summary)

### 核心要点

1. **两种语法**：`#line lineno` 和 `#line lineno "filename"`
2. **主要用途**：代码生成工具映射错误到原始文件
3. **影响宏**：修改 `__LINE__` 和 `__FILE__` 的值
4. **行号范围**：C++11 起为 1 至 2147483647
5. **现代替代**：C++20 提供了 `std::source_location`

### 参数约束

| 参数 | 约束 | 违反约束的后果 |
|------|------|----------------|
| lineno | ≥ 1 且在有效范围内 | 未定义行为 |
| lineno | 必须是十进制数字序列 | 程序非法 |
| filename | 必须是有效字符串 | 程序非法 |

### `#line` vs `std::source_location`

| 特性 | `#line` + `__LINE__`/`__FILE__` | `std::source_location` |
|------|--------------------------------|------------------------|
| 引入版本 | C++98 | C++20 |
| 类型安全 | ❌ 宏展开 | ✅ 类型化 |
| 可定制位置 | ✅ 可修改 | ❌ 编译器决定 |
| 函数参数使用 | ❌ 不便 | ✅ 默认参数 |
| 适用场景 | 代码生成工具 | 通用日志/调试 |

### 使用场景对比

| 场景 | 适用性 | 说明 |
|------|--------|------|
| 代码生成工具 | ✅ 推荐 | 主要设计用途 |
| 手写代码调试 | ⚠️ 谨慎 | 可能造成混淆 |
| 测试框架 | ✅ 可用 | 控制错误消息显示 |
| 生产代码 | ❌ 不推荐 | 应避免使用 |
| 日志记录 | ⚠️ 考虑替代 | C++20 起优先用 `source_location` |

### 学习建议

1. **理解设计目的**：`#line` 主要用于代码生成工具
2. **避免在手写代码中使用**：除非有特殊需求
3. **注意行号范围**：确保行号在有效范围内
4. **保持一致性**：修改行号时同时修改文件名
5. **学习现代替代方案**：C++20 起了解 `std::source_location`

---

**标准参考**：
- C++23 (ISO/IEC 14882:2024): 15.7 Line control [cpp.line]
- C++20 (ISO/IEC 14882:2020): 15.7 Line control [cpp.line]
- C++17 (ISO/IEC 14882:2017): 19.4 Line control [cpp.line]
- C++14 (ISO/IEC 14882:2014): 16.4 Line control [cpp.line]
- C++11 (ISO/IEC 14882:2011): 16.4 Line control [cpp.line]
- C++98 (ISO/IEC 14882:1998): 16.4 Line control [cpp.line]

**相关参考**：
- `std::source_location` - C++20 源代码位置信息类