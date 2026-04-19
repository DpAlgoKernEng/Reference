# 字符串字面量 (String Literal)

## 1. 概述 (Overview)

字符串字面量 (String Literal) 是 C++ 中用于表示字符串常量的语法结构。它是由双引号括起的字符序列，在编译时被转换为静态存储期的字符数组。字符串字面量是 C++ 中表示文本数据的基本方式，广泛用于初始化字符数组、传递字符串参数、输出文本等场景。

字符串字面量具有以下核心特性：
- **静态存储期**：字符串字面量对象具有静态存储持续时间
- **不可修改性**：尝试修改字符串字面量对象会导致未定义行为
- **自动拼接**：相邻的字符串字面量在编译阶段会自动连接
- **多种编码支持**：支持普通字符、宽字符、UTF-8、UTF-16、UTF-32 等多种编码

字符串字面量在程序中表示一个以空字符结尾的字符数组，其类型取决于使用的编码前缀。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

字符串字面量从 C 语言继承而来，最早出现在 C 语言的早期版本中。在 C 语言中，字符串字面量的类型为 `char[N]` 或 `wchar_t[N]`，并且可以隐式转换为非 const 指针以兼容早期 C 代码。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 基础字符串字面量和宽字符串字面量 |
| C++11 | 引入原始字符串字面量 (Raw String Literal)、UTF-8/UTF-16/UTF-32 字符串字面量 |
| C++11 | 废弃字符串字面量到非 const 指针的隐式转换 |
| C++20 | UTF-8 字符串字面量的类型从 `const char[N]` 改为 `const char8_t[N]` |
| C++23 | 强化字符串字面量拼接规则，明确禁止不兼容类型的拼接 |
| C++26 | 未求值字符串 (Unevaluated Strings) 必须使用普通字符串字面量 |

### 设计动机

字符串字面量的设计解决了以下问题：
1. 提供一种简洁的语法表示文本常量
2. 支持多种字符编码以适应国际化需求
3. 原始字符串字面量简化了包含特殊字符（如反斜杠、引号）的字符串编写
4. 静态存储期确保字符串在整个程序运行期间有效

## 3. 语法与参数 (Syntax and Parameters)

### 基础语法

#### 普通字符串字面量
```
"s-char-seq(可选)"
```

#### 原始字符串字面量（C++11 起）
```
R"d-char-seq(可选)(r-char-seq(可选))d-char-seq(可选)"
```

#### 宽字符串字面量
```
L"s-char-seq(可选)"
```

#### UTF-8 字符串字面量（C++11 起）
```
u8"s-char-seq(可选)"
```

#### UTF-16 字符串字面量（C++11 起）
```
u"s-char-seq(可选)"
```

#### UTF-32 字符串字面量（C++11 起）
```
U"s-char-seq(可选)"
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `s-char-seq` | 一个或多个 `s-char` 的序列 |
| `s-char` | 可以是 `basic-s-char`、转义序列或通用字符名 |
| `basic-s-char` | 翻译字符集中的字符，除双引号 `"`、反斜杠 `\` 和换行符外 |
| `d-char-seq` | 一个或多个 `d-char` 的序列，最多 16 个字符 |
| `d-char` | 基本字符集中的字符，除括号、反斜杠和空格外 |
| `r-char-seq` | 一个或多个 `r-char` 的序列，但不能包含闭括号序列 `)d-char-seq"` |
| `r-char` | 翻译字符集中的任意字符 |

### 类型与编码对照表

| 语法 | 种类 | 类型 | 编码 |
|------|------|------|------|
| (1,2) | 普通字符串字面量 | `const char[N]` | 普通字面量编码 |
| (3,4) | 宽字符串字面量 | `const wchar_t[N]` | 宽字面量编码 |
| (5,6) | UTF-8 字符串字面量 | `const char[N]` (C++20 前) / `const char8_t[N]` (C++20 起) | UTF-8 |
| (7,8) | UTF-16 字符串字面量 | `const char16_t[N]` | UTF-16 |
| (9,10) | UTF-32 字符串字面量 | `const char32_t[N]` | UTF-32 |

其中，`N` 是编码码元的数量，由初始化规则确定。

### 原始字符串字面量语法详解

原始字符串字面量 (Raw String Literal) 使用前缀 `R`，其特点是不对任何字符进行转义处理。分隔符 `d-char-seq` 用于标识字符串的边界。

**语法结构**：
```
R"delimiter(content)delimiter"
```

其中：
- `delimiter` 是可选的分隔符序列（最多 16 个字符）
- `content` 是任意原始内容
- 开头和结尾的分隔符必须完全相同

## 4. 底层原理 (Underlying Principles)

### 存储模型

字符串字面量对象具有**静态存储期** (Static Storage Duration)，这意味着：
1. 对象在程序启动时分配（或首次使用时）
2. 对象在程序结束时销毁
3. 对象存储在可执行文件的数据段中

**对象唯一性**：
- 编译器是否将所有字符串字面量存储在非重叠对象中是未指定的
- 连续求值同一字符串字面量是否产生相同或不同对象也是未指定的

```cpp
bool b = "bar" == 3 + "foobar"; // 可能为 true 或 false，未指定
```

### 初始化过程

字符串字面量对象通过以下方式初始化：

1. **基本字符和转义序列处理**：每个 `basic-s-char`、`r-char`、简单转义序列和通用字符名都按照字符串字面量的关联字符编码进行编码。

2. **数值转义序列处理**：对于八进制或十六进制转义序列：
   - 如果值 `v` 在类型 `T` 的可表示范围内，则贡献一个码元
   - 如果是普通或宽字符串字面量，且 `v` 在对应无符号类型范围内，则贡献一个值
   - 其他情况程序非良构

3. **条件转义序列**：贡献实现定义的码元序列

### 拼接机制

相邻的字符串字面量在翻译阶段 6（预处理之后）进行拼接：

**同类型拼接**：结果保持原类型

**不同类型拼接规则**（C++11 起）：
- 普通字符串字面量 + 非普通字符串字面量 → 结果为后者类型
- UTF-8 字符串字面量 + 宽字符串字面量 → 程序非良构
- 其他组合（C++23 前）：有条件支持，实现定义语义
- 其他组合（C++23 起）：非良构

```cpp
"Hello, " " world!"              // 拼接为 "Hello, world!"
L"Δx = %" PRId16                 // PRId16 展开为 "d"，拼接为 L"Δx = %d"
```

### 编码转换

字符串字面量涉及的编码转换：
- 源字符集 → 翻译字符集 → 执行字符集
- 多字节字符按编码规则转换为码元序列
- 不可编码字符导致程序非良构

## 5. 使用场景 (Use Cases)

### 适用场景

1. **字符串常量定义**
   ```cpp
   const char* greeting = "Hello, World!";
   const wchar_t* wide_text = L"宽字符文本";
   const char8_t* utf8_text = u8"UTF-8 编码";  // C++20 起
   const char16_t* utf16_text = u"UTF-16 编码";
   const char32_t* utf32_text = U"UTF-32 编码";
   ```

2. **字符数组初始化**
   ```cpp
   char str[] = "Hello";  // str 包含 {'H', 'e', 'l', 'l', 'o', '\0'}
   ```

3. **包含特殊字符的字符串（原始字符串字面量）**
   ```cpp
   // 包含反斜杠和换行符
   const char* path = R"(C:\Users\name\file.txt)";

   // 包含正则表达式
   const char* regex = R"(\d+\.\d+)";

   // 多行文本
   const char* multiline = R"(
   Line 1
   Line 2
   Line 3
   )";
   ```

4. **跨平台编码处理**
   ```cpp
   const char8_t* filename = u8"文件名.txt";  // 保证 UTF-8 编码
   ```

### 最佳实践

1. **始终使用 const 指针接收字符串字面量**
   ```cpp
   const char* correct = "text";    // 正确
   char* wrong = "text";             // 错误：C++11 起已废弃
   ```

2. **长字符串使用原始字符串字面量提高可读性**
   ```cpp
   // 推荐：清晰易读
   const char* json = R"({"name": "value"})";

   // 不推荐：转义混乱
   const char* json = "{\"name\": \"value\"}";
   ```

3. **使用原始字符串字面量避免转义冲突**
   ```cpp
   // Windows 路径
   const char* path = R"(C:\Program Files\MyApp)";

   // 正则表达式
   const char* pattern = R"(\b\w+\b)";
   ```

### 常见陷阱

1. **尝试修改字符串字面量**
   ```cpp
   const char* pc = "Hello";
   char* p = const_cast<char*>(pc);
   p[0] = 'M';  // 未定义行为！
   ```

2. **字符串字面量比较**
   ```cpp
   // 危险：结果未指定
   if ("text" == "text") { /* 可能不会执行 */ }

   // 正确：使用字符串比较
   if (std::strcmp("text", "text") == 0) { /* 保证执行 */ }
   ```

3. **内嵌空字符**
   ```cpp
   const char* p = "abc\0def";
   std::strlen(p);  // 返回 3，但数组大小为 8
   ```

4. **十六进制转义序列后跟有效十六进制数字**
   ```cpp
   // 错误：转义序列超出范围
   // const char* p = "\xfff";

   // 正确：使用字符串拼接
   const char* p = "\xff" "f";  // 保存 {'\xff', 'f', '\0'}
   ```

5. **不兼容类型的拼接（C++23 起）**
   ```cpp
   // C++23 前可能有条件支持
   // C++23 起为非良构
   const auto* s = u"UTF-16" U"UTF-32";  // 错误
   ```

### 未求值字符串

某些上下文期望字符串字面量但不对其求值：
- 语言链接规范
- `static_assert`（C++11 起）
- 字面量运算符名（C++11 起）
- `[[deprecated]]` 属性（C++14 起）
- `[[nodiscard]]` 属性（C++20 起）
- 删除函数体（C++26 起）

C++26 起，这些上下文只能使用普通字符串字面量。

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>
#include <cstring>

int main() {
    // 基本字符串字面量
    const char* simple = "Hello, World!";
    std::cout << simple << std::endl;

    // 宽字符串字面量
    const wchar_t* wide = L"宽字符";
    std::wcout << wide << std::endl;

    // 字符串拼接
    const char* combined = "Hello, " "World!";  // 等价于 "Hello, World!"
    std::cout << combined << std::endl;

    return 0;
}
```

### 原始字符串字面量示例

```cpp
#include <iostream>

int main() {
    // 包含反斜杠（等价于 "\\"）
    const char* backslash = R"(\)";
    std::cout << "Backslash: " << backslash << std::endl;

    // 包含换行符序列（等价于 "\\n\\n\\n\\n"）
    const char* newlines = R"(\n\n\n\n)";
    std::cout << "Newlines literal: " << newlines << std::endl;

    // 使用自定义分隔符
    const char* with_delimiter = R"-delimiter(content)delimiter-";
    std::cout << "With delimiter: " << with_delimiter << std::endl;

    // 多行文本
    const char* multiline = R"(
Line 1
Line 2
Line 3
    )";
    std::cout << "Multiline:" << multiline << std::endl;

    return 0;
}
```

### Unicode 字符串字面量

```cpp
#include <iostream>
#include <string>

int main() {
    // UTF-8 字符串（C++11 起）
    const char* utf8_old = u8"UTF-8 字符串";  // C++17
    // C++20 起
    #if __cplusplus >= 202002L
    const char8_t* utf8_new = u8"UTF-8 字符串";
    #endif

    // UTF-16 字符串
    const char16_t* utf16 = u"UTF-16 字符串";

    // UTF-32 字符串
    const char32_t* utf32 = U"UTF-32 字符串";

    // 原始 UTF-8 字符串
    const char* raw_utf8 = u8R"(原始 UTF-8: 中文)";

    return 0;
}
```

### 字符数组初始化

```cpp
#include <iostream>
#include <cstring>

int main() {
    // 使用字符串字面量初始化字符数组
    char array1[] = "Foobar";
    char array2[] = {'F', 'o', 'o', 'b', 'a', 'r', '\0'};

    std::cout << "array1: " << array1 << std::endl;
    std::cout << "array2: " << array2 << std::endl;
    std::cout << "Equal: " << (std::strcmp(array1, array2) == 0) << std::endl;

    return 0;
}
```

### 字符串拼接规则

```cpp
#include <iostream>

int main() {
    // 同类型拼接
    const wchar_t* s1 = L"ABC" L"DEF";   // 等价于 L"ABCDEF"
    std::wcout << s1 << std::endl;

    // 普通 + UTF-32 = UTF-32
    const char32_t* s2 = U"GHI" "JKL";   // 等价于 U"GHIJKL"

    // 普通 + UTF-16 = UTF-16
    const char16_t* s3 = "MN" u"OP" "QR"; // 等价于 u"MNOPQR"

    // 原始字符串字面量
    const wchar_t* s4 = LR"--(STUV)--";   // 等价于 L"STUV"
    std::wcout << s4 << std::endl;

    // C++23 前可能支持，C++23 起非良构
    // const auto* s5 = u"UTF-16" U"UTF-32";

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>
#include <cstring>

int main() {
    // 错误 1：尝试修改字符串字面量
    // const char* p1 = "Hello";
    // char* p2 = const_cast<char*>(p1);
    // p2[0] = 'h';  // 未定义行为！

    // 正确做法：使用字符数组或 std::string
    char p1[] = "Hello";  // 创建副本
    p1[0] = 'h';          // 正确：修改副本

    // 错误 2：使用 == 比较字符串字面量
    // if ("text" == "text") { }  // 结果未指定

    // 正确做法：使用 strcmp 或 std::string_view
    if (std::strcmp("text", "text") == 0) {
        std::cout << "Strings are equal" << std::endl;
    }

    // 错误 3：十六进制转义后跟十六进制数字
    // const char* p3 = "\xfff";  // 错误：转义序列超出范围

    // 正确做法：字符串拼接
    const char* p3 = "\xff" "f";  // 正确

    // 错误 4：忽略内嵌空字符
    const char* p4 = "abc\0def";
    std::cout << "strlen: " << std::strlen(p4) << std::endl;  // 输出 3
    // 注意：数组大小为 8，包含 'a','b','c','\0','d','e','f','\0'

    return 0;
}
```

### 综合示例

```cpp
#include <iostream>

// 字符串字面量拼接
char array1[] = "Foo" "bar";
char array2[] = {'F', 'o', 'o', 'b', 'a', 'r', '\0'};

// 原始字符串字面量
const char* s1 = R"foo(
Hello
  World
)foo";
const char* s2 = "\nHello\n  World\n";
const char* s3 = "\n"
                 "Hello\n"
                 "  World\n";

// 宽字符串拼接
const wchar_t* s4 = L"ABC" L"DEF";
const wchar_t* s5 = L"ABCDEF";

// UTF-32 与普通字符串拼接
const char32_t* s6 = U"GHI" "JKL";
const char32_t* s7 = U"GHIJKL";

// UTF-16 与普通字符串拼接
const char16_t* s9 = "MN" u"OP" "QR";
const char16_t* sA = u"MNOPQR";

// 原始宽字符串
const wchar_t* sC = LR"--(STUV)--";

int main() {
    std::cout << array1 << ' ' << array2 << '\n'
              << s1 << s2 << s3 << std::endl;
    std::wcout << s4 << ' ' << s5 << ' ' << sC
               << std::endl;

    return 0;
}
```

**输出**：
```
Foobar Foobar

Hello
  World

Hello
  World

Hello
  World

ABCDEF ABCDEF STUV
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 静态存储期 | 字符串字面量对象在程序整个运行期间存在 |
| 不可修改 | 尝试修改会导致未定义行为 |
| 自动拼接 | 相邻字符串字面量在编译阶段自动连接 |
| 多种编码 | 支持普通、宽字符、UTF-8/16/32 编码 |
| 原始字符串 | C++11 起支持，避免转义字符困扰 |

### 类型对比

| 前缀 | 类型 | 用途 |
|------|------|------|
| 无前缀 | `const char[N]` | ASCII/本地编码字符串 |
| `L` | `const wchar_t[N]` | 宽字符字符串 |
| `u8` | `const char[N]` (C++17) / `const char8_t[N]` (C++20) | UTF-8 编码 |
| `u` | `const char16_t[N]` | UTF-16 编码 |
| `U` | `const char32_t[N]` | UTF-32 编码 |
| `R` 前缀 | 配合以上前缀 | 原始字符串，不处理转义 |

### 版本特性总结

| 版本 | 新增特性 |
|------|----------|
| C++11 | 原始字符串字面量、UTF-8/16/32 字符串字面量 |
| C++11 | 废弃到非 const 指针的隐式转换 |
| C++20 | UTF-8 字符串类型改为 `char8_t` |
| C++23 | 禁止不兼容类型字符串字面量拼接 |
| C++26 | 未求值字符串必须使用普通字符串字面量 |

### 学习建议

1. **掌握基础**：理解字符串字面量的静态存储期和不可修改性
2. **善用原始字符串**：处理包含特殊字符（路径、正则表达式、JSON）时使用 `R"(...)"` 提高可读性
3. **选择合适编码**：国际化应用使用 UTF-8/UTF-16/UTF-32 字符串字面量
4. **避免常见错误**：不要尝试修改字符串字面量、不要用 `==` 比较、注意内嵌空字符
5. **关注版本差异**：C++20 起 UTF-8 字符串类型变化，C++23 拼接规则更严格

### 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|-----------|-----|------|------|
| `__cpp_char8_t` | `202207L` | C++23 (DR20) | `char8_t` 兼容性修复 |
| `__cpp_raw_strings` | `200710L` | C++11 | 原始字符串字面量 |
| `__cpp_unicode_literals` | `200710L` | C++11 | Unicode 字符串字面量 |

### 参考链接

- C++23 标准 (ISO/IEC 14882:2024): 5.13.5 String literals [lex.string]
- C++20 标准 (ISO/IEC 14882:2020): 5.13.5 String literals [lex.string]
- C++17 标准 (ISO/IEC 14882:2017): 5.13.5 String literals [lex.string]
- C++14 标准 (ISO/IEC 14882:2014): 2.14.5 String literals [lex.string]
- C++11 标准 (ISO/IEC 14882:2011): 2.14.5 String literals [lex.string]