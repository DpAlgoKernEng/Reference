# 字符集与编码（Character Sets and Encodings）

## 1. 概述（Overview）

C 语言定义了两层字符集概念：

- **基本字符集（Basic Character Set）**：也称基本源字符集，包含 95 个字符，用于源代码编写
- **基本执行字符集（Basic Execution Character Set）**：在基本字符集基础上增加控制字符，用于程序运行时

C 语言还定义了多种**字面量编码（Literal Encodings）**，用于将字符映射到字符常量和字符串字面量中的实际编码值。

## 2. 来源与演变（Origin and Evolution）

### 标准演进

| 标准 | 说明 |
|------|------|
| C89/C90 | 定义基本字符集和基本执行字符集 |
| C99 | 沿用既有定义 |
| C11 | 引入 Unicode 字面量编码（`u`、`U`、`u8` 前缀） |
| C23 | 明确 UTF 编码规范，`$`、`@`、`` ` `` 必须单字节编码 |

### 与 C++ 的差异

| 特性 | C | C++ |
|------|---|-----|
| 换行符处理 | 行结束指示器处理，不包含在基本字符集 | U+000A 包含在基本字符集中 |
| 执行字符集别名 | 基本执行字符集 | 基本字面量字符集、基本执行宽字符集 |

## 3. 语法与参数（Syntax and Parameters）

### 基本字符集（Basic Character Set）

基本字符集包含 **95 个字符**，也称**基本源字符集（Basic Source Character Set）**。

#### 控制字符

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0009 | 水平制表符（Character Tabulation） | `\t` |
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

#### 数字

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0030..U+0039 | 数字 0-9 | `0 1 2 3 4 5 6 7 8 9` |

#### 大写字母

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0041..U+005A | 拉丁大写字母 A-Z | `A B C D E F G H I J K L M N O P Q R S T U V W X Y Z` |

#### 小写字母

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0061..U+007A | 拉丁小写字母 a-z | `a b c d e f g h i j k l m n o p q r s t u v w x y z` |

### 基本执行字符集（Basic Execution Character Set）

基本执行字符集包含基本字符集的所有成员，并额外包含以下控制字符：

| Unicode | 字符名称 | 转义符 |
|---------|----------|--------|
| U+0000 | 空字符（Null） | `\0` |
| U+0007 | 响铃（Bell） | `\a` |
| U+0008 | 退格（Backspace） | `\b` |
| U+000A | 换行（Line Feed） | `\n` |
| U+000D | 回车（Carriage Return） | `\r` |

### 额外单字节字符（C23 起）

以下字符不在基本执行字符集中，但要求在普通字符常量或字符串字面量中编码为单字节：

| Unicode | 字符名称 | 符号 |
|---------|----------|------|
| U+0024 | 美元符号（Dollar Sign） | `$` |
| U+0040 | 商业 at（Commercial At） | `@` |
| U+0060 | 重音符（Grave Accent） | `` ` `` |

## 4. 底层原理（Underlying Principles）

### 字符值约束

基本执行字符集的成员值必须满足：
1. 每个成员值为**非负数**
2. 每个成员值**互不相同**
3. 数字 `0` 到 `9` 的值**连续递增**
4. `U+0000 NULL` 字符值为 **0**

### 字节表示

基本执行字符集的每个成员都可以用**单个字节**表示。

### 换行符处理

C 语言处理换行符的特殊方式：
- U+000A LINE FEED (LF) **不包含**在基本字符集中
- 源文件通过**行结束指示器**标记每行结束
- 编译器将行结束指示器视为单个换行字符

### 字面量编码映射

| 编码类型 | 前缀 | 编码方式 |
|----------|------|----------|
| 普通字面量编码 | 无 | 实现定义 |
| 宽字面量编码 | `L` | 实现定义 |
| UTF-8 编码 | `u8` | UTF-8（C23 起明确） |
| UTF-16 编码 | `u` | UTF-16（C23 起明确） |
| UTF-32 编码 | `U` | UTF-32（C23 起明确） |

## 5. 使用场景（Use Cases）

### 源代码字符要求

编写 C 源代码时：
- 所有源代码字符必须来自基本字符集
- 注释和字符串可包含扩展字符
- `$`、`@`、`` ` `` 可用（C23 起保证单字节）

### 字面量编码选择

| 场景 | 推荐编码 | 示例 |
|------|----------|------|
| ASCII 文本 | 普通编码 | `"Hello"` |
| 宽字符处理 | `L` 前缀 | `L"你好"` |
| UTF-8 文本 | `u8` 前缀 | `u8"世界"` |
| UTF-16 处理 | `u` 前缀 | `u"文本"` |
| UTF-32 处理 | `U` 前缀 | `U"字符"` |

### 宽字符编码一致性

如果实现未定义 `__STDC_MB_MIGHT_NEQ_WC__` 宏，则宽字面量编码与普通字面量编码对基本执行字符集产生相同的值。

### 常见陷阱

1. **字符集依赖实现**：普通字面量编码是实现定义的，不同平台可能不同

2. **多字节字符**：普通编码可能包含多字节字符序列

3. **宽字符不一致**：宽字符编码可能与普通编码值不同（除非定义了 `__STDC_MB_MIGHT_NEQ_WC__`）

## 6. 代码示例（Examples）

### 基本字符集使用

```c
#include <stdio.h>

int main(void)
{
    // 使用基本字符集中的字符
    char letter = 'A';      // 大写字母
    char digit = '5';       // 数字
    char punct = ';';       // 标点符号

    printf("Letter: %c, Digit: %c, Punctuation: %c\n", letter, digit, punct);
    return 0;
}
```

### 执行字符集控制字符

```c
#include <stdio.h>

int main(void)
{
    // 使用执行字符集中的控制字符
    printf("Hello\n");     // LF 换行
    printf("World\r");     // CR 回车
    printf("\a");          // BEL 响铃
    printf("Tab:\tHere\n"); // HT 水平制表
    return 0;
}
```

### 字面量编码对比

```c
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main(void)
{
    setlocale(LC_ALL, "");

    // 普通编码
    char* s1 = "ASCII";
    printf("普通编码: %s\n", s1);

    // UTF-8 编码 (C11 起)
    char* s2 = u8"UTF-8文本";
    printf("UTF-8 编码: %s\n", s2);

    // 宽字符编码
    wchar_t* ws = L"宽字符";
    wprintf(L"宽字符编码: %ls\n", ws);

    return 0;
}
```

### 字符值验证

```c
#include <stdio.h>

int main(void)
{
    // 验证数字字符连续性
    for (char c = '0'; c <= '9'; ++c) {
        printf("'%c' = %d, ", c, c);
    }
    printf("\n");

    // 验证 NULL 字符值为 0
    char null_char = '\0';
    printf("NULL character value: %d\n", null_char);

    return 0;
}
```

### 常见错误及修正

```c
#include <stdio.h>

int main(void)
{
    /* 错误：假设字符编码为 ASCII */
    char c = 'A';
    // 以下假设仅对 ASCII 兼容编码成立
    int lower = c + 32;  // 假设大小写差 32

    /* 正确：使用标准库函数 */
    #include <ctype.h>
    int lower_correct = tolower(c);

    /* 错误：假设 char 为有符号 */
    char ch = 128;  // 可能溢出，取决于实现

    /* 正确：使用 unsigned char 或 int */
    unsigned char uch = 128;

    printf("Original: %c, Lower: %c\n", c, lower_correct);
    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **两层字符集**：基本字符集（95 字符）→ 基本执行字符集（+5 控制字符）
2. **编码实现定义**：普通和宽字面量编码由实现定义
3. **Unicode 支持**：C11 起 UTF-8/16/32 编码，C23 明确规范
4. **值约束**：字符值非负、互异、数字连续、NULL 为 0

### 字符集层次结构

```
基本字符集 (95 字符)
    ├── 空白字符: 空格、HT、VT、FF
    ├── 标点符号: 29 个
    ├── 数字: 0-9
    ├── 大写字母: A-Z
    └── 小写字母: a-z

基本执行字符集 (基本字符集 + 5 个控制字符)
    └── NULL、BEL、BS、LF、CR
```

### 字面量编码选择指南

| 需求 | C11 前 | C11 起 | C23 起 |
|------|--------|--------|--------|
| ASCII 文本 | `"..."` | `"..."` | `"..."` |
| UTF-8 文本 | 无标准 | `u8"..."` | `u8"..."` |
| UTF-16 文本 | 无标准 | `u"..."` | `u"..."` |
| UTF-32 文本 | 无标准 | `U"..."` | `U"..."` |
| 宽字符 | `L"..."` | `L"..."` | `L"..."` |

### 与 C++ 的主要差异

| 特性 | C | C++ |
|------|---|-----|
| LF 在基本字符集 | 不包含 | 包含 |
| 执行字符集名称 | 基本执行字符集 | 基本字面量字符集 |

### 学习建议

- 理解字符集与编码的区别
- 使用标准库函数处理字符转换
- 优先使用 UTF 编码处理国际化文本
- 注意实现定义行为的移植性影响

### 参考资料

- C17 标准 (ISO/IEC 9899:2018): 5.2.1 Character sets
- C11 标准 (ISO/IEC 9899:2011): 5.2.1 Character sets
- C23 标准: 字符集和编码