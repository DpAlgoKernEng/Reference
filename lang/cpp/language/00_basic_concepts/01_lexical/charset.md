# 字符集与编码（Character Sets and Encodings）

## 1. 概述（Overview）

C++ 标准定义了多层字符集概念，用于规范源代码表示和程序运行时的字符处理：

- **翻译字符集（Translation Character Set）**：C++23 起，包含所有 Unicode 码点
- **基本字符集（Basic Character Set）**：96 个字符（C++26 起为 99 个），用于源代码编写
- **基本字面量字符集（Basic Literal Character Set）**：在基本字符集基础上增加控制字符
- **执行字符集（Execution Character Set）**：运行时字符表示，与区域设置相关

## 2. 来源与演变（Origin and Evolution）

### 标准演进

| 标准 | 说明 |
|------|------|
| C++98 | 定义基本源字符集、基本执行字符集 |
| C++11 | 支持 Unicode 字面量（`u`、`U`、`u8`） |
| C++20 | `char8_t` 类型，UTF-8 字面量类型变更 |
| C++23 | 引入翻译字符集概念，术语重命名 |
| C++26 | 基本字符集增加 `$`、`@`、`` ` `` |

### 术语变更（C++23，P2314R4）

| 新名称 | 旧名称 |
|--------|--------|
| 基本字符集 | 基本源字符集（Basic Source Character Set） |
| 基本字面量字符集 | 基本执行字符集（Basic Execution Character Set）、基本执行宽字符集（Basic Execution Wide-Character Set） |

### 与 C 语言的主要差异

| 特性 | C++ | C |
|------|-----|---|
| 基本字符集数量 | 96（C++26 起 99） | 95 |
| LF (U+000A) | 包含在基本字符集中 | 不包含，用行结束指示器处理 |
| `$`、`@`、`` ` `` | C++26 起包含 | C23 起保证单字节编码 |
| 翻译字符集 | C++23 起定义 | 无 |

## 3. 语法与参数（Syntax and Parameters）

### 翻译字符集（C++23 起）

**翻译字符集（Translation Character Set）** 包含：
- Unicode 码空间中分配给抽象字符的每个码点
- 每个未分配给抽象字符的 Unicode 标量值对应的独特字符

翻译字符集是基本字符集和基本字面量字符集的超集。

### 基本字符集（Basic Character Set）

基本字符集包含 **96 个字符**（C++26 起为 **99 个**）。

#### 控制字符

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0009 | 水平制表符（Character Tabulation） | `\t` |
| U+000A | 换行符（Line Feed） | `\n` |
| U+000B | 垂直制表符（Line Tabulation） | `\v` |
| U+000C | 换页符（Form Feed） | `\f` |
| U+0020 | 空格（Space） | ` ` |

#### 标点符号

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0021 | 感叹号（Exclamation Mark） | `!` |
| U+0022 | 双引号（Quotation Mark） | `"` |
| U+0023 | 井号（Number Sign） | `#` |
| U+0025 | 百分号（Percent Sign） | `%` |
| U+0026 | 和号（Ampersand） | `&` |
| U+0027 | 单引号（Apostrophe） | `'` |
| U+0028 | 左圆括号（Left Parenthesis） | `(` |
| U+0029 | 右圆括号（Right Parenthesis） | `)` |
| U+002A | 星号（Asterisk） | `*` |
| U+002B | 加号（Plus Sign） | `+` |
| U+002C | 逗号（Comma） | `,` |
| U+002D | 连字符（Hyphen-Minus） | `-` |
| U+002E | 句点（Full Stop） | `.` |
| U+002F | 斜杠（Solidus） | `/` |
| U+003A | 冒号（Colon） | `:` |
| U+003B | 分号（Semicolon） | `;` |
| U+003C | 小于号（Less-Than Sign） | `<` |
| U+003D | 等于号（Equals Sign） | `=` |
| U+003E | 大于号（Greater-Than Sign） | `>` |
| U+003F | 问号（Question Mark） | `?` |
| U+005B | 左方括号（Left Square Bracket） | `[` |
| U+005C | 反斜杠（Reverse Solidus） | `\` |
| U+005D | 右方括号（Right Square Bracket） | `]` |
| U+005E | 脱字符（Circumflex Accent） | `^` |
| U+005F | 下划线（Low Line） | `_` |
| U+007B | 左花括号（Left Curly Bracket） | `{` |
| U+007C | 竖线（Vertical Line） | `\|` |
| U+007D | 右花括号（Right Curly Bracket） | `}` |
| U+007E | 波浪号（Tilde） | `~` |

#### 数字和字母

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0030..U+0039 | 数字 0-9 | `0 1 2 3 4 5 6 7 8 9` |
| U+0041..U+005A | 拉丁大写字母 A-Z | `A B C D E F G H I J K L M N O P Q R S T U V W X Y Z` |
| U+0061..U+007A | 拉丁小写字母 a-z | `a b c d e f g h i j k l m n o p q r s t u v w x y z` |

#### C++26 新增字符

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0024 | 美元符号（Dollar Sign） | `$` |
| U+0040 | 商业 at（Commercial At） | `@` |
| U+0060 | 重音符（Grave Accent） | `` ` `` |

### 基本字面量字符集（Basic Literal Character Set）

基本字面量字符集包含基本字符集的所有成员，并额外包含以下控制字符：

| Unicode | 字符名称 | 转义符 |
|---------|----------|--------|
| U+0000 | 空字符（Null） | `\0` |
| U+0007 | 响铃（Bell） | `\a` |
| U+0008 | 退格（Backspace） | `\b` |
| U+000D | 回车（Carriage Return） | `\r` |

### 执行字符集（Execution Character Set）

执行字符集和执行宽字符集是基本字面量字符集的超集：
- 编码方式与**区域设置（Locale）**相关
- 执行宽字符集的每个元素必须能用独立的 `wchar_t` 码元表示

## 4. 底层原理（Underlying Principles）

### 码元与字面量编码

**码元（Code Unit）** 是字符类型的整数值。字符字面量和字符串字面量中的字符根据编码前缀编码为一个或多个码元的序列。

### 编码规则

字面量编码必须满足：
1. 基本字面量字符集的每个元素编码为**单个码元**
2. 码元值为**非负数**
3. 每个元素的码元值**互不相同**
4. `U+0000 NULL` 字符编码为值 **0**
5. 数字 `0` 到 `9` 的码元值**连续递增**

### 字面量编码类型

| 编码类型 | 前缀 | 说明 |
|----------|------|------|
| 普通字面量编码 | 无 | 普通字符/字符串字面量 |
| 宽字面量编码 | `L` | 宽字符/字符串字面量 |
| UTF-8 编码 | `u8` | UTF-8 字面量（C++20 起类型为 `char8_t[]`） |
| UTF-16 编码 | `u` | UTF-16 字面量 |
| UTF-32 编码 | `U` | UTF-32 字面量 |

### UTF 字面量编码

对于 UTF-8、UTF-16 或 UTF-32 字面量，翻译字符集中每个字符的 UCS 标量值按照 ISO/IEC 10646 规范编码。

### 源文件字符映射

翻译阶段 1 中，源文件字符到字符集的映射是**实现定义**的：
- C++23 前：映射到基本字符集
- C++23 起：映射到翻译字符集
- UTF-8 源文件有特殊处理规则

## 5. 使用场景（Use Cases）

### 源代码字符选择

| 字符类型 | C++98 | C++23 | C++26 |
|----------|-------|-------|-------|
| 基本字符集 | ✅ | ✅ | ✅ |
| `$`、`@`、`` ` `` | 实现定义 | 实现定义 | ✅ 基本字符集 |
| Unicode 字符 | 实现定义 | ✅ 翻译字符集 | ✅ 翻译字符集 |

### 字面量编码选择

```cpp
// 普通字面量（实现定义编码）
char s1[] = "Hello";

// 宽字面量
wchar_t s2[] = L"Wide";

// UTF-8 字面量（C++20 起类型为 char8_t[]）
char8_t s3[] = u8"UTF-8";

// UTF-16 字面量
char16_t s4[] = u"UTF-16";

// UTF-32 字面量
char32_t s5[] = U"UTF-32";
```

### 跨平台注意事项

1. **普通字面量编码**是实现定义的，不同平台可能不同
2. **执行字符集**与区域设置相关，需要正确设置 locale
3. **宽字符大小**因平台而异（Windows: 2 字节，Linux: 4 字节）

### 常见陷阱

1. **假设编码为 ASCII**：

```cpp
// 错误：假设 'a' 到 'z' 连续
char c = 'a';
int index = c - 'a';  // 仅对 ASCII 兼容编码正确

// 正确：使用标准库
int index = std::tolower(c) - 'a';
```

2. **宽字符大小假设**：

```cpp
// 错误：假设 wchar_t 为 2 字节
static_assert(sizeof(wchar_t) == 2);  // Linux 上失败

// 正确：使用 char16_t 或 char32_t
static_assert(sizeof(char16_t) == 2);
static_assert(sizeof(char32_t) == 4);
```

## 6. 代码示例（Examples）

### 基本字符集使用

```cpp
#include <iostream>

int main()
{
    // 基本字符集中的字符
    char letter = 'A';
    char digit = '5';
    char punct = ';';

    // 换行符（LF 在 C++ 基本字符集中）
    char newline = '\n';

    std::cout << "Letter: " << letter << newline;
    std::cout << "Digit: " << digit << newline;
    std::cout << "Punctuation: " << punct << newline;

    return 0;
}
```

### 字面量编码对比

```cpp
#include <iostream>
#include <iomanip>

int main()
{
    // 普通字面量
    char s1[] = "Hello";
    std::cout << "普通: " << s1 << "\n";

    // UTF-8 字面量
    char8_t s2[] = u8"世界";
    std::cout << "UTF-8: ";
    for (char8_t c : s2) {
        if (c == 0) break;
        std::cout << std::hex << std::setw(2) << static_cast<int>(c) << " ";
    }
    std::cout << "\n";

    // UTF-32 字面量
    char32_t s3[] = U"你好";
    for (char32_t c : s3) {
        if (c == 0) break;
        std::cout << "U+" << std::hex << static_cast<int>(c) << " ";
    }
    std::cout << "\n";

    return 0;
}
```

### 字符值验证

```cpp
#include <iostream>
#include <cctype>

int main()
{
    // 验证 NULL 字符值为 0
    char null_char = '\0';
    std::cout << "NULL character value: " << static_cast<int>(null_char) << "\n";

    // 验证数字字符连续性（标准保证）
    std::cout << "Digit values:\n";
    for (char c = '0'; c <= '9'; ++c) {
        std::cout << "'" << c << "' = " << static_cast<int>(c) << "  ";
    }
    std::cout << "\n";

    // 验证 LF 在基本字符集中
    char newline = '\n';
    std::cout << "LF value: " << static_cast<int>(newline) << "\n";

    return 0;
}
```

### C++26 新增字符示例

```cpp
#include <iostream>

int main()
{
    // C++26 起，这些字符在基本字符集中
    // C++26 之前可能不被支持或需要实现定义支持

#if __cplusplus >= 202600L
    char dollar = '$';
    char at = '@';
    char grave = '`';

    std::cout << "Dollar: " << dollar << "\n";
    std::cout << "At sign: " << at << "\n";
    std::cout << "Grave accent: " << grave << "\n";
#else
    std::cout << "C++26 features require C++26 or later\n";
#endif

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>
#include <cctype>
#include <locale>

int main()
{
    /* 错误：假设执行字符集为 ASCII */
    char c = 128;  // 可能超出基本字面量字符集范围

    /* 正确：使用 unsigned char 或明确编码 */
    unsigned char uc = 128;

    /* 错误：假设字符集大小写转换差 32 */
    char upper = 'Z';
    // int lower_wrong = upper + 32;  // 仅对 ASCII 正确

    /* 正确：使用标准库函数 */
    int lower_correct = std::tolower(upper);

    /* 错误：假设普通字面量编码可处理任意 Unicode */
    // char* s = "你好";  // 编译器可能警告或错误

    /* 正确：使用 UTF-8 字面量 */
    char8_t* s = u8"你好";

    std::cout << "Lowercase: " << static_cast<char>(lower_correct) << "\n";
    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **三层字符集结构**：翻译字符集 → 基本字符集 → 基本字面量字符集
2. **编码实现定义**：普通和宽字面量编码由实现定义
3. **Unicode 支持**：UTF-8/16/32 字面量有明确定义
4. **C++26 扩展**：`$`、`@`、`` ` `` 加入基本字符集

### 字符集层次结构

```
翻译字符集 (C++23)
    └── 所有 Unicode 码点

基本字符集 (96 → 99 字符)
    ├── 控制字符: HT, LF, VT, FF, Space
    ├── 标点符号: 29 个
    ├── 数字: 0-9
    ├── 大写字母: A-Z
    ├── 小写字母: a-z
    └── C++26 新增: $, @, `

基本字面量字符集 (基本字符集 + 4 个控制字符)
    └── NULL, BEL, BS, CR

核心概念图解
┌─────────────────────────────────────────────────────────────┐
│                    源代码阶段                                │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  基本字符集: 你能写在代码里的字符              │   │
│  │  → A-Z, a-z, 0-9, + - * / = { } [ ] 等共96个        │   │
│  └─────────────────────────────────────────────────────┘   │
│                           ↓ 编译                            │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                    运行时阶段                                │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  执行字符集: 程序运行时能处理的字符          │   │
│  │  → 基本字符集 + 控制字符(\0, \n, \t等) + 本地字符   │   │
│  └─────────────────────────────────────────────────────┘   │
│                           ↓                                 │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  字面量编码: 字符在内存中怎么存              │   │
│  │  → "Hello" 用什么编码？ASCII? UTF-8? GBK?           │   │
│  └─────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘

逐个解释

1. 基本字符集
含义：你能写在源代码里的字符

int main() {
  int x = 10;     // ✅ 这些都是基本字符集
  char c = 'A';   // ✅
  // char 中 = '中';  // ❌ '中' 不在基本字符集
}

2. 执行字符集
含义：程序运行时能处理的字符（比基本字符集大）

int main() {
  char s[] = "你好世界";  // ✅ 运行时可以处理
  // "你好世界" 属于执行字符集（扩展部分）
  printf("%s\n", s);      // 可以正常打印
}

3. 翻译字符集 (Translation Character Set, C++23)
含义：所有 Unicode 字符的超集，C++23 新概念

// C++23 起，编译器必须理解所有 Unicode 字符
char8_t s[] = u8"🎉🌍💻";  // 翻译字符集包含这些 emoji

4. 字面量编码
含义：字符串在内存里实际怎么存

// 同样的 "A"，不同编码存的值可能不同
char a = 'A';        // 普通编码：可能是 65 (ASCII)
wchar_t b = L'A';    // 宽字符编码：可能是 65 或其他
char8_t c = u8'A';   // UTF-8：一定是 65

实际例子对比

#include <stdio.h>

int main() {

    /* 基本字符集：写代码用的字符 */
    int count = 100;   // 这些英文字母、数字、符号都在基本字符集

    /* 执行字符集：运行时能处理什么 */
    char* chinese = "中文测试";    // 运行时能处理中文
    char* emoji = "🎉";           // 运行时能处理 emoji
    
    /* 字面量编码：内存中怎么存 */
    char     s1[] = "A";    // 普通编码，可能是 1 字节
    wchar_t  s2[] = L"A";   // 宽字符编码，可能是 2 或 4 字节
    char     s3[] = u8"A";  // UTF-8 编码，一定是 1 字节
    
    printf("普通: %s\n", s1);
    wprintf(L"宽字符: %ls\n", s2);
    printf("UTF-8: %s\n", s3);
    
    return 0;
}

一句话总结
┌────────────┬──────────────────────────────────────┐
│    概念    │              一句话说明              │
├────────────┼──────────────────────────────────────┤
│ 基本字符集 │ 写代码能用什么字（编译器认识什么）   │
├────────────┼──────────────────────────────────────┤
│ 执行字符集 │ 程序运行能处理什么字（能打印什么）   │
├────────────┼──────────────────────────────────────┤
│ 翻译字符集 │ 所有 Unicode（C++23 新标准）         │
├────────────┼──────────────────────────────────────┤
│ 字面量编码 │ 字符串在内存里怎么存（UTF-8？GBK？） │
└────────────┴──────────────────────────────────────┘

为什么需要区分？

// 问题场景
char* s = "你好";

// 基本字符集：编译器能识别源代码
// 执行字符集：运行时能显示"你好"
// 字面量编码：但"你好"在内存中怎么存？
//   - Windows 可能用 GBK（4字节）
//   - Linux 可能用 UTF-8（6字节）
//   → 这就是为什么需要编码概念！
```

### 字面量编码选择指南

| 需求 | 推荐 | 示例 |
|------|------|------|
| ASCII 文本 | 普通字面量 | `"Hello"` |
| 国际化文本 | UTF-8 | `u8"世界"` |
| 固定宽度字符 | UTF-32 | `U"字符"` |
| 平台宽字符 | 宽字面量 | `L"Wide"` |

### 标准版本特性对比

| 特性 | C++98 | C++11 | C++20 | C++23 | C++26 |
|------|-------|-------|-------|-------|-------|
| UTF 字面量 | ❌ | ✅ | ✅ | ✅ | ✅ |
| `char8_t` | ❌ | ❌ | ✅ | ✅ | ✅ |
| 翻译字符集 | ❌ | ❌ | ❌ | ✅ | ✅ |
| `$@`` 在基本字符集 | ❌ | ❌ | ❌ | ❌ | ✅ |

### 学习建议

- 使用 UTF-8 字面量处理国际化文本
- 避免假设字符编码为 ASCII
- 使用标准库函数进行字符分类和转换
- 注意 `wchar_t` 大小的平台差异

### 参考资料

- C++23 标准: 5.3 Character sets
- C++26 标准: 字符集扩展
- P2314R4: Character set naming changes
- ISO/IEC 10646: UCS 编码规范