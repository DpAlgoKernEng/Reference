# ASCII 字符表（ASCII Chart）

## 1. 概述（Overview）

**ASCII（American Standard Code for Information Interchange，美国信息交换标准代码）** 是一种基于拉丁字母的字符编码系统，最初用于电信通信，后成为计算机领域最基础的字符编码标准。

ASCII 包含 **128 个字符**（编码范围 0-127），涵盖：
- 33 个控制字符（0-31 及 127）
- 95 个可打印字符（32-126）

在 Unicode 中，ASCII 字符块被称为 **U+0000..U+007F Basic Latin（基本拉丁）**。

## 2. 来源与演变（Origin and Evolution）

### 历史背景

| 年份 | 事件 |
|------|------|
| 1963 | ASA（现 ANSI）发布 ASCII 标准（X3.4-1963） |
| 1967 | 发布修订版，定义了小写字母 |
| 1986 | 最终修订版（ANSI X3.4-1986） |
| 现代 | 被 Unicode 向下兼容，成为 UTF-8 的子集 |

### 与 C++ 的关系

C++ 继承了 C 语言的字符处理传统：
- `char` 类型通常为 8 位，可存储 ASCII 字符
- 字符常量如 `'A'` 对应 ASCII 值 65
- 转义字符如 `'\n'` 对应 ASCII 控制字符
- C++11 引入 `char16_t`、`char32_t`，C++20 引入 `char8_t` 用于 Unicode

### C++ 字符类型演进

| 类型 | C++ 标准 | 用途 |
|------|----------|------|
| `char` | C++98 | ASCII/单字节字符 |
| `wchar_t` | C++98 | 宽字符（平台相关） |
| `char16_t` | C++11 | UTF-16 字符 |
| `char32_t` | C++11 | UTF-32 字符 |
| `char8_t` | C++20 | UTF-8 字符 |

## 3. 语法与参数（Syntax and Parameters）

### 完整 ASCII 字符表

#### 控制字符（0-31, 127）

| Dec | Oct | Hex | 字符 | 说明 |
|-----|-----|-----|------|------|
| 0 | 0 | 00 | NUL | 空字符（Null） |
| 1 | 1 | 01 | SOH | 标题开始（Start of Header） |
| 2 | 2 | 02 | STX | 正文开始（Start of Text） |
| 3 | 3 | 03 | ETX | 正文结束（End of Text） |
| 4 | 4 | 04 | EOT | 传输结束（End of Transmission） |
| 5 | 5 | 05 | ENQ | 询问（Enquiry） |
| 6 | 6 | 06 | ACK | 确认（Acknowledge） |
| 7 | 7 | 07 | BEL | 响铃（Bell） |
| 8 | 10 | 08 | BS | 退格（Backspace） |
| 9 | 11 | 09 | HT | 水平制表符（Horizontal Tab） |
| 10 | 12 | 0A | LF | 换行（Line Feed） |
| 11 | 13 | 0B | VT | 垂直制表符（Vertical Tab） |
| 12 | 14 | 0C | FF | 换页（Form Feed） |
| 13 | 15 | 0D | CR | 回车（Carriage Return） |
| 14 | 16 | 0E | SO | 移出（Shift Out） |
| 15 | 17 | 0F | SI | 移入（Shift In） |
| 16 | 20 | 10 | DLE | 数据链路转义（Data Link Escape） |
| 17 | 21 | 11 | DC1 | 设备控制 1 |
| 18 | 22 | 12 | DC2 | 设备控制 2 |
| 19 | 23 | 13 | DC3 | 设备控制 3 |
| 20 | 24 | 14 | DC4 | 设备控制 4 |
| 21 | 25 | 15 | NAK | 否定确认（Negative Acknowledge） |
| 22 | 26 | 16 | SYN | 同步空闲（Synchronous Idle） |
| 23 | 27 | 17 | ETB | 传输块结束（End of Trans. Block） |
| 24 | 30 | 18 | CAN | 取消（Cancel） |
| 25 | 31 | 19 | EM | 介质结束（End of Medium） |
| 26 | 32 | 1A | SUB | 替换（Substitute） |
| 27 | 33 | 1B | ESC | 转义（Escape） |
| 28 | 34 | 1C | FS | 文件分隔符（File Separator） |
| 29 | 35 | 1D | GS | 组分隔符（Group Separator） |
| 30 | 36 | 1E | RS | 记录分隔符（Record Separator） |
| 31 | 37 | 1F | US | 单元分隔符（Unit Separator） |
| 127 | 177 | 7F | DEL | 删除（Delete） |

#### 可打印字符（32-126）

**空格和标点符号**

| Dec | Oct | Hex | 字符 | Dec | Oct | Hex | 字符 |
|-----|-----|-----|------|-----|-----|-----|------|
| 32 | 40 | 20 | (空格) | 60 | 74 | 3C | < |
| 33 | 41 | 21 | ! | 61 | 75 | 3D | = |
| 34 | 42 | 22 | " | 62 | 76 | 3E | > |
| 35 | 43 | 23 | # | 63 | 77 | 3F | ? |
| 36 | 44 | 24 | $ | 91 | 133 | 5B | [ |
| 37 | 45 | 25 | % | 92 | 134 | 5C | \ |
| 38 | 46 | 26 | & | 93 | 135 | 5D | ] |
| 39 | 47 | 27 | ' | 94 | 136 | 5E | ^ |
| 40 | 50 | 28 | ( | 95 | 137 | 5F | _ |
| 41 | 51 | 29 | ) | 96 | 140 | 60 | ` |
| 42 | 52 | 2A | * | 123 | 173 | 7B | { |
| 43 | 53 | 2B | + | 124 | 174 | 7C | \| |
| 44 | 54 | 2C | , | 125 | 175 | 7D | } |
| 45 | 55 | 2D | - | 126 | 176 | 7E | ~ |
| 46 | 56 | 2E | . | | | | |
| 47 | 57 | 2F | / | | | | |
| 58 | 72 | 3A | : | | | | |
| 59 | 73 | 3B | ; | | | | |

**数字（48-57）**

| Dec | Oct | Hex | 字符 |
|-----|-----|-----|------|
| 48 | 60 | 30 | 0 |
| 49 | 61 | 31 | 1 |
| 50 | 62 | 32 | 2 |
| 51 | 63 | 33 | 3 |
| 52 | 64 | 34 | 4 |
| 53 | 65 | 35 | 5 |
| 54 | 66 | 36 | 6 |
| 55 | 67 | 37 | 7 |
| 56 | 70 | 38 | 8 |
| 57 | 71 | 39 | 9 |

**大写字母（65-90）**

| Dec | Oct | Hex | 字符 | Dec | Oct | Hex | 字符 |
|-----|-----|-----|------|-----|-----|-----|------|
| 65 | 101 | 41 | A | 78 | 116 | 4E | N |
| 66 | 102 | 42 | B | 79 | 117 | 4F | O |
| 67 | 103 | 43 | C | 80 | 120 | 50 | P |
| 68 | 104 | 44 | D | 81 | 121 | 51 | Q |
| 69 | 105 | 45 | E | 82 | 122 | 52 | R |
| 70 | 106 | 46 | F | 83 | 123 | 53 | S |
| 71 | 107 | 47 | G | 84 | 124 | 54 | T |
| 72 | 110 | 48 | H | 85 | 125 | 55 | U |
| 73 | 111 | 49 | I | 86 | 126 | 56 | V |
| 74 | 112 | 4A | J | 87 | 127 | 57 | W |
| 75 | 113 | 4B | K | 88 | 130 | 58 | X |
| 76 | 114 | 4C | L | 89 | 131 | 59 | Y |
| 77 | 115 | 4D | M | 90 | 132 | 5A | Z |

**小写字母（97-122）**

| Dec | Oct | Hex | 字符 | Dec | Oct | Hex | 字符 |
|-----|-----|-----|------|-----|-----|-----|------|
| 97 | 141 | 61 | a | 110 | 156 | 6E | n |
| 98 | 142 | 62 | b | 111 | 157 | 6F | o |
| 99 | 143 | 63 | c | 112 | 160 | 70 | p |
| 100 | 144 | 64 | d | 113 | 161 | 71 | q |
| 101 | 145 | 65 | e | 114 | 162 | 72 | r |
| 102 | 146 | 66 | f | 115 | 163 | 73 | s |
| 103 | 147 | 67 | g | 116 | 164 | 74 | t |
| 104 | 150 | 68 | h | 117 | 165 | 75 | u |
| 105 | 151 | 69 | i | 118 | 166 | 76 | v |
| 106 | 152 | 6A | j | 119 | 167 | 77 | w |
| 107 | 153 | 6B | k | 120 | 170 | 78 | x |
| 108 | 154 | 6C | l | 121 | 171 | 79 | y |
| 109 | 155 | 6D | m | 122 | 172 | 7A | z |

### 进制转换

| 进制 | 前缀 | 示例 |
|------|------|------|
| 十进制（Decimal） | 无 | `65` |
| 八进制（Octal） | `0` | `0101` |
| 十六进制（Hexadecimal） | `0x` | `0x41` |

## 4. 底层原理（Underlying Principles）

### 编码结构

ASCII 采用 **7 位二进制编码**：
- 可表示 2^7 = 128 个字符
- 在 8 位字节中，最高位通常为 0
- UTF-8 对 ASCII 字符使用单字节编码，与 ASCII 完全兼容

### 大小写转换原理

大小写字母在 ASCII 中相差固定的 32（0x20）：
- 大写 `A` = 65，小写 `a` = 97
- 转换公式：`小写 = 大写 + 32` 或 `小写 = 大写 | 0x20`

```cpp
// 大写转小写（位运算）
char tolower_bit(char c) {
    return c | 0x20;
}

// 小写转大写（位运算）
char toupper_bit(char c) {
    return c & ~0x20;
}
```

### 数字字符与数值的关系

数字字符 `'0'` 到 `'9'` 的 ASCII 值连续排列（48-57）：
- 字符转数值：`数值 = 字符 - '0'`
- 数值转字符：`字符 = 数值 + '0'`

### C++ 字面量表示

| 类型 | 示例 | 说明 |
|------|------|------|
| 字符字面量 | `'A'` | 类型为 `char` |
| 整数转义 | `'\101'` | 八进制表示（65） |
| 十六进制转义 | `'\x41'` | 十六进制表示（65） |
| Unicode 字面量 | `u8'A'` | C++20 起，类型为 `char8_t` |

## 5. 使用场景（Use Cases）

### 常用控制字符的转义表示

| ASCII | 十进制 | C++ 转义符 | 说明 |
|-------|--------|------------|------|
| NUL | 0 | `\0` | 字符串结束符 |
| BEL | 7 | `\a` | 响铃 |
| BS | 8 | `\b` | 退格 |
| HT | 9 | `\t` | 水平制表符 |
| LF | 10 | `\n` | 换行 |
| VT | 11 | `\v` | 垂直制表符 |
| FF | 12 | `\f` | 换页 |
| CR | 13 | `\r` | 回车 |

### C++ 字符分类函数

C++ 标准库 `<cctype>` 提供基于 ASCII 的字符分类函数：

| 函数 | 判断条件 | ASCII 范围 |
|------|----------|-------------|
| `std::isalnum()` | 字母或数字 | A-Z, a-z, 0-9 |
| `std::isalpha()` | 字母 | A-Z, a-z |
| `std::isdigit()` | 数字 | 0-9 |
| `std::islower()` | 小写字母 | a-z |
| `std::isupper()` | 大写字母 | A-Z |
| `std::isspace()` | 空白字符 | 空格, \t, \n, \v, \f, \r |
| `std::ispunct()` | 标点符号 | 非字母数字且非空格的可打印字符 |
| `std::iscntrl()` | 控制字符 | 0-31, 127 |
| `std::isprint()` | 可打印字符 | 32-126 |
| `std::isgraph()` | 图形字符 | 33-126（不含空格） |

### C++ 标准库字符转换

```cpp
#include <cctype>

char c = 'G';
char lower = std::tolower(c);  // 'g'
char upper = std::toupper('k'); // 'K'
```

### 常见陷阱

1. **有符号字符问题**：

```cpp
char c = 128;  // 若 char 为有符号，c 可能为负值
// 建议使用 unsigned char 处理扩展 ASCII
```

2. **平台相关的 `char` 符号性**：
- ARM、PowerPC：默认 `unsigned char`
- x86、x64：默认 `signed char`

3. **平台相关的换行符**：
- Unix/Linux：`\n`（LF，10）
- Windows：`\r\n`（CR+LF，13+10）

## 6. 代码示例（Examples）

### 基础用法：打印可打印 ASCII 字符

```cpp
#include <iostream>

int main()
{
    std::cout << "Printable ASCII [32..126]:\n";
    for (char c{' '}; c <= '~'; ++c)
        std::cout << c << ((c + 1) % 32 ? ' ' : '\n');
    std::cout << '\n';
    return 0;
}
```

**输出**：
```
Printable ASCII [32..126]:
  ! " # $ % & ' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ?
@ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ] ^ _
` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~
```

### 字符与数值转换

```cpp
#include <iostream>
#include <cctype>

int main()
{
    // 字符转数值
    char digit = '7';
    int value = digit - '0';
    std::cout << "字符 '" << digit << "' 的数值是 " << value << '\n';

    // 数值转字符
    int num = 5;
    char ch = num + '0';
    std::cout << "数值 " << num << " 的字符是 '" << ch << "'\n";

    // 大小写转换
    char upper = 'G';
    char lower = std::tolower(upper);
    std::cout << "大写 '" << upper << "' 转小写 '" << lower << "'\n";

    return 0;
}
```

**输出**：
```
字符 '7' 的数值是 7
数值 5 的字符是 '5'
大写 'G' 转小写 'g'
```

### 使用 `<cctype>` 进行字符判断

```cpp
#include <iostream>
#include <cctype>

int main()
{
    const char* test = "Hello123!";

    for (const char* p = test; *p; ++p) {
        char c = *p;
        std::cout << "字符 '" << c << "': ";

        if (std::isalpha(c)) std::cout << "字母 ";
        if (std::isdigit(c)) std::cout << "数字 ";
        if (std::islower(c)) std::cout << "小写 ";
        if (std::isupper(c)) std::cout << "大写 ";
        if (std::ispunct(c)) std::cout << "标点 ";

        std::cout << '\n';
    }

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>
#include <cctype>

int main()
{
    /* 错误：直接使用字符值作为数字 */
    char c = '5';
    int wrong = c;  // wrong = 53，而非 5

    /* 正确：减去 '0' 得到数值 */
    int correct = c - '0';  // correct = 5

    std::cout << "错误方式: " << wrong << '\n';
    std::cout << "正确方式: " << correct << '\n';

    /* 错误：对非字母字符使用大小写转换 */
    char symbol = '$';
    char transformed = std::tolower(symbol);  // symbol 不变

    /* 正确：先判断是否为字母 */
    if (std::isalpha(symbol)) {
        symbol = std::tolower(symbol);
    }

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **编码范围**：0-127 共 128 个字符
2. **字符分类**：33 个控制字符 + 95 个可打印字符
3. **Unicode 兼容**：ASCII 是 Unicode 的子集（Basic Latin）
4. **C++ 字符类型**：`char` 是基础，现代 C++ 提供 Unicode 支持

### 关键数值记忆

| 字符 | ASCII 值 | 说明 |
|------|----------|------|
| `'0'` | 48 | 数字起始 |
| `'A'` | 65 | 大写字母起始 |
| `'a'` | 97 | 小写字母起始 |
| `' '` | 32 | 空格 |
| `'\n'` | 10 | 换行 |
| `'\t'` | 9 | 制表符 |

### C++ 与 C 的差异

| 特性 | C | C++ |
|------|---|-----|
| 字符字面量类型 | `int` | `char` |
| 宽字符 | `wchar_t` | `wchar_t` |
| Unicode 支持 | 无内置 | `char16_t`, `char32_t`, `char8_t` |
| 标准库头文件 | `<ctype.h>` | `<cctype>` |

### 最佳实践

- 使用 `<cctype>` 函数进行字符分类和转换
- 记住大小写相差 32 的规律
- 注意 `char` 的符号性因平台而异
- 处理 Unicode 时使用现代 C++ 字符类型

### 参考资料

- ANSI X3.4-1986: ASCII 标准
- ISO/IEC 646: 国际版本
- Unicode Standard: U+0000..U+007F Basic Latin
- C++20 标准: 字符类型和字面量