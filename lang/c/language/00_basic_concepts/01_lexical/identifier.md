# 标识符（Identifier）

## 1. 概述（Overview）

**标识符（Identifier）** 是 C 语言中用于命名程序实体的字符序列。标识符可以表示以下类型的实体：

- 对象（objects）
- 函数（functions）
- 标签（tags）：结构体、联合体或枚举
- 结构体或联合体成员（structure or union members）
- 枚举常量（enumeration constants）
- typedef 名称（typedef names）
- 标签名（label names）
- 宏名称（macro names）
- 宏参数名称（macro parameter names）

每个标识符（宏名称和宏参数名称除外）都具有**作用域（scope）**，属于某个**命名空间（name space）**，并可能具有**链接（linkage）**。同一个标识符可以在程序的不同位置表示不同的实体，或者如果实体位于不同的命名空间中，则可以在同一位置表示不同的实体。

### 标识符组成规则

标识符是由以下字符组成的任意长序列：
- 数字（0-9）
- 下划线（_）
- 小写和大写拉丁字母（a-z, A-Z）
- Unicode 字符（通过 `\u` 和 `\U` 转义表示法，C99 起）
- XID_Continue 类 Unicode 字符（C23 起）

### 有效标识符要求

| 标准 | 首字符要求 | 后续字符要求 |
|------|------------|--------------|
| C89/C90 | 拉丁字母或下划线 | 拉丁字母、数字或下划线 |
| C99-C17 | 拉丁字母、下划线或 Unicode 非数字字符 | 拉丁字母、数字、下划线或 Unicode 字符 |
| C23 起 | XID_Start 类 Unicode 字符 | XID_Continue 类 Unicode 字符 |

**重要特性**：
- 标识符区分大小写（小写和大写字母是不同的）
- C23 起，每个标识符必须符合 Unicode 规范化形式 C（Normalization Form C）

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C89/C90 | 基础标识符规则，仅支持 ASCII 字符 |
| C95 | 引入宽字符和扩展字符集支持 |
| C99 | 支持 Unicode 转义序列 `\u` 和 `\U`；放宽翻译限制 |
| C11 | 沿用 C99 规则 |
| C17 | 沿用既有规则 |
| C23 | 引入 XID_Start/XID_Continue Unicode 属性；要求规范化形式 C；引入"潜在保留标识符"概念 |

### 设计动机

1. **可移植性**：通过定义保留标识符规则，确保用户代码与标准库和编译器实现不冲突
2. **国际化**：C99 引入 Unicode 支持，C23 进一步规范为 XID 属性，支持多语言编程
3. **扩展性**：为标准库的未来扩展预留命名空间

### 翻译限制的演变

| 限制项 | C89/C90 | C99 起 |
|--------|---------|--------|
| 内部标识符/宏名有效字符数 | 31 | 63 |
| 外部标识符有效字符数 | 6 | 31 |
| 翻译单元中外标识符数 | 511 | 4095 |
| 块内声明的块作用域标识符数 | 127 | 511 |
| 同时定义的宏标识符数 | 1024 | 4095 |

### 与 C++ 的区别

- **C++**：任何位置包含双下划线的标识符都是保留的
- **C**：仅以双下划线开头的标识符是保留的

## 3. 语法与参数（Syntax and Parameters）

### 3.1 标识符语法

```
identifier:
    identifier-nondigit
    identifier identifier-nondigit
    identifier digit

identifier-nondigit:
    nondigit
    universal-character-name (C99 起)

nondigit: one of
    _ a b c d e f g h i j k l m n o p q r s t u v w x y z
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z

digit: one of
    0 1 2 3 4 5 6 7 8 9
```

### 3.2 Unicode 转义序列（C99 起）

```c
char *\U0001f431 = "cat";  // 支持：使用转义序列
char *🐱 = "cat";          // 由实现定义：使用原始 Unicode 字符
                           // 例如：Clang 支持，GCC 10 之前不支持
                           // C23 起：emoji 不是 XID_Start 字符，两者都是非良构
```

### 3.3 命名空间

C 语言有四种独立的命名空间：

| 命名空间 | 示例 |
|----------|------|
| 标签（struct/union/enum） | `struct foo { };` |
| 成员（结构体/联合体成员） | `obj.member` |
| 标签名（goto 目标） | `label:` |
| 普通标识符（其他所有） | 变量、函数、typedef 等 |

### 3.4 作用域

| 作用域类型 | 说明 |
|------------|------|
| 文件作用域 | 在所有块和参数列表之外声明 |
| 块作用域 | 在块或参数列表内声明 |
| 函数原型作用域 | 在函数原型参数列表中声明（非定义） |
| 函数作用域 | 仅用于标签名 |

### 3.5 链接

| 链接类型 | 说明 |
|----------|------|
| 外部链接 | 可被其他翻译单元访问 |
| 内部链接 | 仅在当前翻译单元内可见 |
| 无链接 | 块作用域变量、参数等 |

## 4. 底层原理（Underlying Principles）

### 4.1 词法分析

标识符在编译的**翻译阶段 3（translation phase 3）** 被识别：

```
源代码字符序列 → [词法分析] → 标识符、关键字、常量、字符串字面量、标点符号
```

### 4.2 最长匹配原则

编译器采用**最长匹配原则**识别标识符：

```c
int abc123def;    // 一个标识符
int abc 123 def;  // 语法错误
```

### 4.3 名称修饰（Name Mangling）

外部标识符的名称修饰规则因实现而异：
- 某些链接器区分大小写，某些不区分
- 某些链接器对名称长度有限制
- C 标准要求支持的最小有效字符数（C89: 6 字符，C99: 31 字符）

### 4.4 Unicode 规范化（C23 起）

C23 要求标识符符合**规范化形式 C（NFC）**：

```c
// C23 起：以下必须保持一致性
char *café = "coffee";      // NFC 形式
char *café = "coffee";      // 如果使用了不同的 Unicode 序列表示相同字符，需要规范化
```

### 4.5 XID 属性（C23 起）

C23 使用 Unicode 标准定义的字符属性：
- **XID_Start**：可以作为标识符首字符
- **XID_Continue**：可以作为标识符后续字符

这确保了标识符的语法与 Unicode 标准一致，并支持多语言编程。

## 5. 使用场景（Use Cases）

### 5.1 命名约定

| 类型 | 约定 | 示例 |
|------|------|------|
| 宏常量 | 全大写 + 下划线 | `MAX_SIZE` |
| 宏函数 | 全大写 + 下划线 | `MAX(a, b)` |
| 变量 | 小写 + 下划线 或 小驼峰 | `user_name` / `userName` |
| 函数 | 小写 + 下划线 或 小驼峰 | `get_value()` / `getValue()` |
| 类型（typedef） | 首字母大写 或 小写 + _t | `Size` / `size_t` |
| 结构体标签 | 首字母大写 或 小写 + 下划线 | `struct Node` / `struct node` |

### 5.2 避免保留标识符

**禁止使用的标识符**：

```c
// 1. 关键字
int int;          // 错误：int 是关键字
int return;       // 错误：return 是关键字

// 2. 以下划线开头的外部标识符
int _global;      // 危险：外部链接，以下划线开头是保留的

// 3. 以双下划线开头或下划线+大写字母开头
int __reserved;   // 危险：保留给实现
int _Reserved;    // 危险：保留给实现

// 4. 标准库函数名（外部链接）
int printf = 0;   // 危险：与标准库冲突
```

### 5.3 利用命名空间

```c
// 同一标识符可用于不同命名空间
struct Node { int value; };   // 标签命名空间
int Node = 0;                 // 普通标识符命名空间

void func(void) {
Node:                         // 标签命名空间
    ;
}

struct Node node;
node.value = Node;            // 成员命名空间
```

### 5.4 常见陷阱

1. **与保留标识符冲突**：

```c
// 错误：使用保留标识符
int _count = 0;       // 以 _ 开头，外部链接时保留
int __myvar = 0;      // 以 __ 开头，始终保留
int _MyVar = 0;       // 以 _ + 大写字母开头，始终保留
```

2. **与标准库未来扩展冲突**：

```c
#include <ctype.h>
int isnumber(int c) { return c >= '0' && c <= '9'; }  // 危险：is 后跟小写字母保留

#include <math.h>
double cr_sin(double x) { ... }  // 危险：cr_ 前缀在 C23 中保留
```

3. **名称长度限制**：

```c
// 假设链接器只支持 31 个有效字符
int this_is_a_very_long_identifier_name_that_might_be_truncated = 0;
int this_is_a_very_long_identifier_name_that_might_be_truncat = 0;
// 某些链接器可能将两者视为相同
```

4. **大小写敏感性**：

```c
int value = 1;
int Value = 2;    // 不同的标识符
int VALUE = 3;    // 又一个不同的标识符
```

## 6. 代码示例（Examples）

### 基础用法

```c
#include <stdio.h>

// 文件作用域标识符
int global_counter = 0;       // 外部链接
static int file_local = 10;   // 内部链接

// typedef 名称
typedef unsigned int uint;

// 结构体标签
struct Point {
    int x;
    int y;
};

// 枚举
enum Color { RED, GREEN, BLUE };

// 函数声明
int add(int a, int b);

int main(void)
{
    // 块作用域标识符
    int local_var = 100;

    // 使用标识符
    struct Point p = {10, 20};
    uint value = 42;

    printf("Point: (%d, %d)\n", p.x, p.y);
    printf("Sum: %d\n", add(1, 2));
    printf("Color: %d\n", RED);

    // 标签
    goto skip;
    local_var = 200;
skip:
    printf("Local: %d\n", local_var);  // 100

    return 0;
}

int add(int a, int b) {
    return a + b;
}
```

### 利用命名空间

```c
#include <stdio.h>

// 同一名称在不同命名空间
struct info {
    int info;  // 成员命名空间
};

int info = 0;  // 普通标识符命名空间

void test(void)
{
info:          // 标签命名空间
    puts("label");
}

int main(void)
{
    struct info data;
    data.info = 100;

    printf("Member: %d\n", data.info);  // 成员
    printf("Global: %d\n", info);       // 全局变量

    goto info;  // 标签

    return 0;
}
```

### Unicode 标识符（C99 起）

```c
#include <stdio.h>

// 使用 Unicode 转义序列（推荐方式）
char *\U0001f431 = "cat";  // 有效的标识符

int main(void)
{
    // 普通标识符
    int 变量 = 10;  // 实现定义是否支持原始 Unicode

    printf("Value: %d\n", 变量);
    printf("Cat: %s\n", \U0001f431);

    return 0;
}

/* 注意：
 * C23 起，emoji 不是 XID_Start 字符，
 * 使用 emoji 作为标识符首字符是非良构的
 */
```

### 常见错误及修正

```c
#include <stdio.h>

/* 错误：使用保留标识符 */
int _count = 0;           // 危险：以 _ 开头

/* 正确：使用非保留标识符 */
int my_count = 0;         // 安全

/* 错误：重新定义标准库标识符 */
#include <string.h>
size_t strlen = 10;       // 危险：与标准库函数冲突

/* 正确：使用不同的名称 */
size_t my_strlen = 10;    // 安全

/* 错误：使用保留前缀 */
#include <ctype.h>
int is_custom(int c) { return c; }  // 危险：is + 小写字母保留

/* 正确：使用非保留前缀 */
int check_custom(int c) { return c; }  // 安全

/* 错误：宏定义与关键字相同 */
#define int long          // 未定义行为：重定义关键字

/* 正确：使用不同的宏名称 */
#define my_int long       // 安全

int main(void)
{
    /* 错误：混淆大小写 */
    int value = 1;
    int Value = 2;  // 不同的变量！
    printf("%d\n", value);  // 1
    printf("%d\n", Value);  // 2

    return 0;
}
```

### 正确的头文件保护

```c
/* myheader.h */

#ifndef MYHEADER_H
#define MYHEADER_H

/* 头文件内容 */
int my_function(int x);

#endif /* MYHEADER_H */

/* 注意：使用 MYHEADER_H 而非 _MYHEADER_H
 * 因为以 _ 开头后跟大写字母的标识符是保留的
 */
```

## 7. 总结（Summary）

### 核心要点

1. **命名规则**：以非数字字符开头，可包含字母、数字、下划线和 Unicode 字符（C99 起）
2. **区分大小写**：`value`、`Value`、`VALUE` 是三个不同的标识符
3. **保留标识符**：关键字、以 `_` 开头的外部标识符、以 `__` 或 `_` + 大写字母开头的标识符
4. **命名空间**：四种独立命名空间允许同名标识符共存
5. **翻译限制**：C99 起内部标识符有效字符数 63，外部标识符 31

### 保留标识符规则

| 规则 | 示例 |
|------|------|
| 关键字 | `int`、`return`、`if` 等 |
| 以 `_` 开头的外部标识符 | `_global`（外部链接时） |
| 以 `__` 开头 | `__reserved` |
| 以 `_` + 大写字母开头 | `_Reserved` |
| 标准库函数名（外部链接） | `printf`、`malloc` 等 |
| 标准库保留前缀 | `is`、`str`、`wcs`、`atomic_`、`E`、`FE_` 等 |

### 最佳实践

- 使用有意义的描述性名称
- 避免使用保留标识符和保留前缀
- 头文件保护符不要以 `_` + 大写字母开头
- 保持一致的命名风格
- 注意外部标识符的名称长度限制

### 与 C++ 的区别

| 特性 | C | C++ |
|------|---|-----|
| 双下划线保留 | 仅开头 | 任何位置 |
| 有效字符数（外部） | 31 (C99 起) | 实现定义 |
| Unicode 支持 | C99 起 | C++11 起 |

### 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.4.2 Identifiers
- C17 标准 (ISO/IEC 9899:2018): 6.4.2 Identifiers (p: 43)
- C11 标准 (ISO/IEC 9899:2011): 6.4.2 Identifiers (p: 59-60)
- C99 标准 (ISO/IEC 9899:1999): 6.4.2 Identifiers (p: 51-52)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.1.2 Identifiers