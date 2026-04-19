# 字符字面量 (Character Literal)

## 1. 概述 (Overview)

字符字面量是 C++ 中表示单个字符值的字面量。它用单引号括起一个或多个字符，在编译时转换为对应类型的字符值。字符字面量是 C++ 基础文本处理的核心构造。

**核心特点**：
- 用单引号 `'` 包围字符内容
- 编译后转换为特定类型的字符值
- 支持多种字符编码（普通字符、UTF-8、UTF-16、UTF-32、宽字符）
- 支持转义序列表示特殊字符

**技术定位**：
- 属于词法元素中的字面量类型
- 与字符串字面量（双引号）不同，字符字面量表示单个字符值
- 在表达式中作为字符值参与运算

**与 C 语言的区别**：
- C 语言中普通字符常量 `'a'` 的类型为 `int`
- C++ 中普通字符字面量 `'a'` 的类型为 `char`

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

字符字面量最早出现在 B 语言中，C 语言继承了这一设计。C++ 又从 C 语言继承了字符字面量，但对类型系统进行了调整。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 定义基本的普通字符字面量和宽字符字面量 `L'...'` |
| C++11 | 新增 `u'...'` (char16_t) 和 `U'...'` (char32_t) 类型 |
| C++17 | 新增 `u8'...'` (char8_t) UTF-8 字符字面量 |
| C++20 | `u8'...'` 的类型从 `char` 改为 `char8_t` |
| C++23 | 移除宽多字符字面量 `L'...'`；改进编码错误处理 |

### 设计动机

- 提供与实现无关的字符表示方式
- 支持国际化字符集（Unicode）
- 解决不同平台字符编码差异问题
- 提供类型安全的字符处理

### 缺陷报告

| DR | 应用版本 | 原有行为 | 修正行为 |
|------|----------|----------|----------|
| CWG 912 | C++98 | 不可编码的普通字符字面量未指定 | 指定为条件支持 |
| CWG 1024 | C++98 | 多字符字面量被要求支持 | 改为条件支持 |
| CWG 1656 | C++98 | 字符字面量中数值转义序列的含义不清晰 | 明确规定 |
| P1854R4 | C++98 | 不可编码的字符字面量是条件支持的 | 程序为 ill-formed |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
'c-char'           // (1) 普通字符字面量，类型为 char
u8'c-char'         // (2) UTF-8 字符字面量 (C++17)，类型为 char8_t (C++20)
u'c-char'          // (3) UTF-16 字符字面量 (C++11)，类型为 char16_t
U'c-char'          // (4) UTF-32 字符字面量 (C++11)，类型为 char32_t
L'c-char'          // (5) 宽字符字面量，类型为 wchar_t

'c-char-sequence'  // (6) 多字符字面量，类型为 int（条件支持）
L'c-char-sequence' // (7) 宽多字符字面量（C++23 前），类型为 wchar_t（条件支持）
```

### c-char 定义

c-char 可以是以下之一：

1. **basic-c-char**：来自基本源字符集（C++23 前）或翻译字符集（C++23 起）的字符，除单引号 `'`、反斜杠 `\` 和换行符外

2. **转义序列**：
   - 特殊字符转义：`\'` `\"` `\?` `\\` `\a` `\b` `\f` `\n` `\r` `\t` `\v`
   - 十六进制转义：`\x...`
   - 八进制转义：`\...`

3. **通用字符名**：`\u....` 或 `\U........`

### 类型与值

| 前缀 | 类型 | 说明 | 版本 |
|------|------|------|------|
| 无 | `char` | 执行字符集中的单字节值 | C++98+ |
| `u8` | `char8_t` | UTF-8 单码单元值 (0x00-0x7F) | C++17+ |
| `u` | `char16_t` | UTF-16 单码单元值 (0x0-0xFFFF) | C++11+ |
| `U` | `char32_t` | UTF-32 单码单元值 (0x0-0x10FFFF) | C++11+ |
| `L` | `wchar_t` | 执行宽字符集中的值 | C++98+ |

### 数值转义序列

数值转义序列（八进制和十六进制）可用于指定字符的值：

**C++23 起**：
- 如果字符字面量只包含一个数值转义序列，且该转义序列指定的值可由其类型的无符号版本表示，则字符字面量具有与指定值相同的值
- UTF-N 字符字面量可以具有其类型可表示的任何值，即使该值不对应有效的 Unicode 码点

**C++23 前**：
- 如果普通或宽字符字面量中数值转义序列指定的值不能由 `char` 或 `wchar_t` 表示，则该值是实现定义的

## 4. 底层原理 (Underlying Principles)

### 编译期转换机制

字符字面量在编译时被转换为对应类型的值：

```
源代码 'A' → 编译器 → 字符值 'A' (char 类型)
```

### 字符集映射

1. **普通字符字面量**：从源字符集映射到执行字符集（C++23 前）或普通字面量编码（C++23 起）
2. **宽字符字面量**：映射到执行宽字符集（C++23 前）或宽字面量编码（C++23 起）
3. **UTF 字符字面量**：直接使用 ISO/IEC 10646 码点值

### 不可编码字符处理

如果 c-char 不是数值转义序列，且：
- c-char 在字面量的关联字符编码中不可表示，或
- 无法在该编码中编码为单个码单元（例如在 Windows 上 wchar_t 为 16 位时的非 BMP 值）

则程序为 ill-formed。

### 多字符字面量的存储

多字符字面量（如 `'AB'`）是条件支持的，大多数编译器（MSVC 是显著例外）按照 B 语言规范实现：

- 按**大端序**、**零填充**、**右对齐**方式存储
- 例如：`'\1'` 的值为 `0x00000001`，`'\1\2\3\4'` 的值为 `0x01020304`

```
'\1'        → 0x00000001
'\1\2\3\4'  → 0x01020304
'AB'        → 0x00004142  // 'A'=0x41, 'B'=0x42
```

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 推荐用法 | 示例 |
|------|----------|------|
| ASCII 字符处理 | 普通字符字面量 | `'a'`, `'\n'` |
| Unicode 字符 | UTF-32 字面量 | `U'猫'`, `U'🍌'` |
| 宽字符环境 | 宽字符字面量 | `L'字'` |
| BMP 字符 | UTF-16 字面量 | `u'猫'` |
| ASCII/UTF-8 字符 | UTF-8 字面量 | `u8'a'` |

### 最佳实践

1. **优先使用单字符字面量**：多字符字面量的行为是实现定义的，不可移植
2. **Unicode 字符使用 `U` 前缀**：保证跨平台一致性，支持所有 Unicode 码点
3. **避免依赖实现定义行为**：如非 BMP 字符在 16 位 wchar_t 环境下的处理
4. **使用 `const` 或 `constexpr`**：提高代码安全性

### 常见陷阱

| 陷阱 | 问题 | 解决方案 |
|------|------|----------|
| `u'🍌'` | Emoji 需要 UTF-16 代理对 | 使用 `U'🍌'` 或 `u'\U0001f34c'` |
| `'AB'` 的值 | 实现定义，不可移植 | 避免使用多字符字面量 |
| `u8'猫'` | 非 ASCII 字符需要多个 UTF-8 码单元 | 使用 `U'猫'` 或字符串 |
| Windows 上 `L'🍌'` (C++23) | wchar_t 为 16 位，无法编码 | 使用 `U'🍌'` |

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

int main() {
    // 普通字符字面量
    char newline = '\n';
    char tab = '\t';
    char quote = '\'';

    std::cout << "换行符值: " << static_cast<int>(newline) << '\n';   // 输出: 10
    std::cout << "制表符值: " << static_cast<int>(tab) << '\n';       // 输出: 9
    std::cout << "单引号值: " << static_cast<int>(quote) << '\n';     // 输出: 39

    return 0;
}
```

### Unicode 字符字面量

```cpp
#include <iostream>
#include <cstdint>

int main() {
    // UTF-8 字符字面量 (C++17)
    char8_t c8 = u8'A';
    std::cout << "UTF-8 'A': 0x" << std::hex << static_cast<int>(c8) << '\n';

    // UTF-16 字符字面量
    char16_t c16 = u'猫';
    std::cout << "UTF-16 '猫': 0x" << static_cast<int>(c16) << '\n';  // 输出: 0x732b

    // UTF-32 字符字面量 - 支持 Emoji
    char32_t c32 = U'🍌';
    std::cout << "UTF-32 '🍌': 0x" << static_cast<int>(c32) << '\n';  // 输出: 0x1f34c

    return 0;
}
```

### 宽字符字面量

```cpp
#include <iostream>
#include <cwchar>
#include <locale>

int main() {
    std::setlocale(LC_ALL, "en_US.utf8");

    wchar_t wc = L'中';
    std::wcout << L"宽字符: " << wc << L'\n';

    return 0;
}
```

### 转义序列示例

```cpp
#include <iostream>

int main() {
    std::cout << "响铃字符: " << static_cast<int>('\a') << '\n';        // 7
    std::cout << "十六进制 \\x41: " << '\x41' << '\n';                  // A
    std::cout << "八进制 \\101: " << '\101' << '\n';                    // A
    std::cout << "通用字符 \\u4e2d: " << L'\u4e2d' << '\n';             // 中 (宽字符)

    return 0;
}
```

### 常见错误示例

```cpp
#include <iostream>

int main() {
    // 错误：多字符字面量值是实现定义的
    int mc = 'AB';
    std::cout << "'AB' = 0x" << std::hex << mc << " (实现定义)\n";

    // 错误：u'🍌' 需要代理对，编译错误
    // char16_t emoji = u'🍌';  // 编译错误！

    // 正确：使用 UTF-32
    char32_t emoji = U'🍌';  // 正确

    // 错误：u8'猫' 需要多个 UTF-8 码单元，编译错误
    // char8_t cat = u8'猫';  // 编译错误！

    // 正确：使用 UTF-32 或字符串
    char32_t cat = U'猫';  // 正确

    std::cout << "正确: U'🍌' = 0x" << static_cast<int>(emoji) << '\n';
    std::cout << "正确: U'猫' = 0x" << static_cast<int>(cat) << '\n';

    return 0;
}
```

### 完整示例：字符字面量类型对比

```cpp
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string_view>

template<typename CharT>
void dump(std::string_view s, const CharT c) {
    const uint8_t* data{reinterpret_cast<const uint8_t*>(&c)};

    std::cout << s << " \t" << std::hex
              << std::uppercase << std::setfill('0');

    for (auto i{0U}; i != sizeof(CharT); ++i)
        std::cout << std::setw(2) << static_cast<unsigned>(data[i]) << ' ';

    std::cout << '\n';
}

void print(std::string_view str = "") { std::cout << str << '\n'; }

int main() {
    print("字符字面量类型对比");
    print("==================\n");

    print("普通字符字面量:");
    char c1 = 'a'; dump("'a'", c1);
    char c2 = '\x2a'; dump("'*'", c2);

    print("\n多字符字面量:");
    int mc1 = 'ab'; dump("'ab'", mc1);       // 实现定义
    int mc2 = 'abc'; dump("'abc'", mc2);     // 实现定义

    print("\nUTF-8 字符字面量:");
    char8_t C1 = u8'a'; dump("u8'a'", C1);
    // char8_t C2 = u8'猫';  // 错误：猫需要 3 个 UTF-8 码单元

    print("\nUTF-16 字符字面量:");
    char16_t uc1 = u'a'; dump("u'a'", uc1);
    char16_t uc2 = u'¢'; dump("u'¢'", uc2);
    char16_t uc3 = u'猫'; dump("u'猫'", uc3);
    // char16_t uc4 = u'🍌';  // 错误：🍌需要 2 个 UTF-16 码单元

    print("\nUTF-32 字符字面量:");
    char32_t Uc1 = U'a'; dump("U'a'", Uc1);
    char32_t Uc2 = U'¢'; dump("U'¢'", Uc2);
    char32_t Uc3 = U'猫'; dump("U'猫'", Uc3);
    char32_t Uc4 = U'🍌'; dump("U'🍌'", Uc4);  // 正确

    print("\n宽字符字面量:");
    wchar_t wc1 = L'a'; dump("L'a'", wc1);
    wchar_t wc2 = L'猫'; dump("L'猫'", wc2);

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 基本语法 | `'字符'` 或 `前缀'字符'` |
| 默认类型 | `char`（非 C 的 `int`） |
| UTF-8 前缀 | `u8'...'` (C++17)，类型 `char8_t` (C++20) |
| UTF-16 前缀 | `u'...'` (C++11)，类型 `char16_t` |
| UTF-32 前缀 | `U'...'` (C++11)，类型 `char32_t` |
| 宽字符前缀 | `L'...'`，类型 `wchar_t` |

### 类型对比

| 前缀 | 类型 | 大小 | 适用场景 |
|------|------|------|----------|
| 无 | `char` | 通常 1 字节 | ASCII 字符 |
| `u8` | `char8_t` | 1 字节 | ASCII/UTF-8 字符 (0x00-0x7F) |
| `u` | `char16_t` | 2 字节 | BMP Unicode 字符 |
| `U` | `char32_t` | 4 字节 | 所有 Unicode 字符 |
| `L` | `wchar_t` | 平台相关 | 平台本地化字符 |

### 版本兼容性提示

- **C++23**：移除宽多字符字面量；改进编码错误处理
- **C++20**：`u8'...'` 类型改为 `char8_t`
- **C++17**：新增 `u8'...'`
- **C++11**：新增 `u'...'` 和 `U'...'`

### 学习建议

1. 理解字符字面量与字符串字面量的区别
2. 掌握不同前缀对应的编码类型和适用范围
3. 避免使用多字符字面量以确保可移植性
4. Unicode 字符优先使用 `U'...'` 前缀
5. 注意 C++ 与 C 语言中普通字符常量类型的差异