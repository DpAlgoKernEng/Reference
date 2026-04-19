# 整数常量 (Integer Constant)

## 1. 概述 (Overview)

整数常量（Integer Constant）是 C 语言中允许在表达式中直接使用整数值的字面量形式。它是程序中最基本、最常用的常量类型之一，用于表示各种进制的整数值。

### 核心概念

- **非左值表达式**：整数常量是非左值（non-lvalue）表达式，不能被赋值
- **直接嵌入**：允许在代码中直接书写数值，无需定义变量
- **多进制支持**：支持十进制、八进制、十六进制，C23 起支持二进制
- **类型自动推导**：编译器根据数值大小和后缀自动确定常量类型

### 主要用途

- 初始化变量
- 数组大小定义
- 循环控制
- 位运算和掩码
- 枚举值定义
- 条件判断

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

整数常量是 C 语言诞生之初就存在的基础特性，其设计动机源于系统编程的需求：

- **直接硬件映射**：八进制和十六进制常量可以直接对应机器底层表示
- **可移植性**：提供统一的方式在不同平台上表示整数值
- **类型安全**：通过后缀机制帮助编译器进行类型检查

### 版本演进

| C 标准版本 | 新增特性 | 说明 |
|-----------|---------|------|
| C89/C90 | 基础特性 | 十进制、八进制、十六进制常量，`u/U`、`l/L` 后缀 |
| C99 | 长长整型 | 新增 `ll/LL` 后缀支持 64 位整数 |
| C99 | 宏扩展 | 在 `#if`/`#elif` 中整数常量按 `intmax_t`/`uintmax_t` 处理 |
| C23 | 二进制常量 | 新增 `0b`/`0B` 前缀的二进制常量 |
| C23 | 数字分隔符 | 允许在数字间使用单引号 `'` 作为分隔符提高可读性 |
| C23 | 位精确整数 | 新增 `wb`/`WB` 后缀支持 `_BitInt` 类型 |

### 设计动机

1. **可读性**：十六进制常量便于表示位模式和内存地址
2. **精度控制**：后缀机制允许程序员显式指定所需类型
3. **向后兼容**：类型推导规则确保代码在新标准下仍能正常工作

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

整数常量由两部分组成：数值部分和可选的后缀部分。

```
整数常量 = 数值前缀 + 数字序列 + [整数后缀]
```

### 四种进制形式

#### 1. 十进制常量 (Decimal Constant)

```
decimal-constant integer-suffix(可选)
```

- **规则**：非零数字（1-9）开头，后接零或多个十进制数字（0-9）
- **示例**：`42`, `123`, `999`

#### 2. 八进制常量 (Octal Constant)

```
octal-constant integer-suffix(可选)
```

- **规则**：数字 0 开头，后接零或多个八进制数字（0-7）
- **示例**：`052`, `0123`, `0777`

#### 3. 十六进制常量 (Hexadecimal Constant)

```
hex-constant integer-suffix(可选)
```

- **规则**：`0x` 或 `0X` 开头，后接一个或多个十六进制数字（0-9, a-f, A-F）
- **示例**：`0x2a`, `0X2A`, `0xFF`, `0xDEADBEEF`

#### 4. 二进制常量 (Binary Constant) [C23 起]

```
binary-constant integer-suffix(可选)
```

- **规则**：`0b` 或 `0B` 开头，后接一个或多个二进制数字（0-1）
- **示例**：`0b101010`, `0B11111111`

### 整数后缀 (Integer Suffix)

后缀用于指定整数常量的类型，可组合使用。

| 后缀 | 含义 | 示例 |
|-----|------|------|
| `u` 或 `U` | 无符号类型 | `42u`, `0xFFU` |
| `l` 或 `L` | long 类型 | `42L`, `0xFFl` |
| `ll` 或 `LL` | long long 类型 [C99] | `42LL`, `0xFFll` |
| `wb` 或 `WB` | `_BitInt` 类型 [C23] | `42wb` |
| 组合后缀 | 类型组合 | `42ull`, `0xFFLLU` |

**注意事项**：
- 后缀字母不区分大小写（除 `lL` 或 `Ll` 这种混合形式不允许）
- 组合后缀顺序可以任意（`ul` 和 `lu` 等价）
- `ll/LL` 必须两个字符相同

### 数字分隔符 [C23 起]

可在数字之间插入单引号 `'` 作为分隔符，编译器会忽略它们：

```c
int million = 1'000'000;
unsigned long long big = 18'446'744'073'709'550'592llu;
int binary = 0b1010'1100'0011'1111;
```

## 4. 底层原理 (Underlying Principles)

### 类型推导机制

整数常量的类型由编译器根据**数值大小**、**进制**和**后缀**自动确定。类型选择遵循"最小适配"原则：选择能容纳该值的第一个类型。

#### 类型推导表

| 后缀 | 十进制 | 八进制/十六进制 |
|-----|--------|----------------|
| 无后缀 | int → long int → long long int [C99] | int → unsigned int → long int → unsigned long int → long long int [C99] → unsigned long long int [C99] |
| `u/U` | unsigned int → unsigned long int → unsigned long long int [C99] | unsigned int → unsigned long int → unsigned long long int [C99] |
| `l/L` | long int → long long int [C99] | long int → unsigned long int → long long int [C99] → unsigned long long int [C99] |
| `l/L` + `u/U` | unsigned long int → unsigned long long int [C99] | unsigned long int → unsigned long long int [C99] |
| `ll/LL` | long long int [C99] | long long int [C99] → unsigned long long int [C99] |
| `ll/LL` + `u/U` | unsigned long long int [C99] | unsigned long long int [C99] |
| `wb/WB` | `_BitInt(N)` [C23] | `_BitInt(N)` [C23] |
| `wb/WB` + `u/U` | `unsigned _BitInt(N)` [C23] | `unsigned _BitInt(N)` [C23] |

### 关键差异

**十进制 vs 其他进制**：
- 十进制常量优先选择有符号类型
- 八进制/十六进制常量可能选择无符号类型（如果值超出有符号范围）

**示例**：

```c
// 假设 int 为 16 位，long 为 32 位
int a = 32767;      // 类型为 int
int b = 32768;      // 类型为 long（超出 int 范围）

// 十六进制会优先尝试 unsigned int
int c = 0x7FFF;     // 类型为 int
int d = 0x8000;     // 类型可能为 unsigned int（取决于平台）
```

### 溢出处理

如果整数常量的值无法放入任何允许的类型中：
- **有扩展整数类型支持**（如 `__int128`）：可能使用扩展类型
- **无扩展类型支持**：程序非良构（编译错误）

### 最大匹配原则 (Maximal Munch)

编译器在词法分析时采用"最长匹配"原则，这可能导致意外行为：

```c
int x = 0xE+2;   // 错误！被解析为预处理数字 "0xE+2"
int y = 0xa+2;   // 正确，解析为 0xa + 2
int z = 0xE +2;  // 正确，空格分隔
int q = (0xE)+2; // 正确，括号分隔
```

**原因**：十六进制数字包含 `e`/`E`，编译器会尝试将 `0xE+2` 解析为预处理器数字标记，导致错误。

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 变量初始化

```c
int count = 100;              // 十进制
int mask = 0xFF;              // 十六进制掩码
int permissions = 0755;       // 八进制权限
int flags = 0b10101010;       // 二进制标志位 [C23]
```

#### 2. 数组大小定义

```c
int buffer[1024];             // 缓冲区大小
int matrix[3][3];             // 矩阵维度
int lookup_table[0x100];      // 查找表（256个元素）
```

#### 3. 位运算

```c
unsigned int mask = 0xFFFF;   // 16位掩码
int flag_a = 0b00000001;      // 标志位A [C23]
int flag_b = 0b00000010;      // 标志位B [C23]
int combined = flag_a | flag_b;
```

#### 4. 类型显式指定

```c
long big_number = 123456789L;          // 显式指定 long
unsigned int positive = 42U;           // 显式指定 unsigned
long long very_big = 9223372036854775807LL;  // 显式指定 long long
```

### 最佳实践

#### 1. 选择合适的进制

```c
// 推荐做法
int mask = 0xFF;              // 位掩码用十六进制
int permissions = 0755;       // Unix 权限用八进制
int count = 100;              // 普通计数用十进制

// 避免
int confusing = 010;          // 易混淆：这是 8，不是 10
```

#### 2. 使用后缀避免类型歧义

```c
// 推荐：显式指定类型
long x = 100L;
unsigned int y = 42U;

// 避免：让编译器猜测
long x = 100;     // 可能产生不必要的类型转换
```

#### 3. 利用数字分隔符提高可读性 [C23]

```c
// 推荐
long population = 7'800'000'000;      // 78亿
int rgb = 0xFF'00'FF;                  // 紫色
int binary_value = 0b1100'1010'1111'0000;

// 不推荐
long population = 7800000000;         // 难以识别数量级
```

#### 4. 大整数使用 ULL 后缀

```c
// 推荐
unsigned long long big = 18446744073709551615ULL;

// 可能问题
unsigned long long big = 18446744073709551615;  // 类型可能不正确
```

### 常见陷阱

#### 陷阱 1：八进制前导零

```c
// 错误示例
int month = 09;    // 编译错误！9不是八进制数字
int day = 012;     // 这是 10，不是 12！

// 正确做法
int month = 9;     // 十进制
int day = 12;      // 十进制
int octal_value = 012;  // 明确知道这是八进制
```

#### 陷阱 2：负数不是常量

```c
// 理解本质
int x = -42;       // 这不是"负数常量"
                    // 而是对常量 42 应用一元负号运算符

// 这在大整数时很重要
long long min_ll = -9223372036854775808LL;  // 可能错误！
// 因为 9223372036854775808 可能超出 long long 范围

// 正确做法
long long min_ll = -9223372036854775807LL - 1;
```

#### 陷阱 3：十六进制的 e/E

```c
// 错误
int x = 0xE+2;     // 解析为预处理标记，编译错误

// 正确
int x = 0xE + 2;   // 加空格
int x = (0xE)+2;   // 加括号
```

#### 陷阱 4：类型推导意外

```c
// 假设 int 为 16 位
int a = 32768;     // 类型可能是 long，不是 int

// 显式指定类型
long a = 32768L;   // 明确意图
```

### 注意事项

1. **预处理阶段**：在 `#if` 和 `#elif` 指令中，所有有符号整数常量按 `intmax_t` 处理，无符号按 `uintmax_t` 处理

2. **字母大小写**：十六进制字母和后缀字母不区分大小写（`0xFF` = `0xff`，`42UL` = `42ul`）

3. **无负数常量**：C 语言没有负数常量，`-42` 是对常量 `42` 应用负号运算符

4. **扩展整数类型**：如果编译器支持扩展整数类型（如 `__int128`），超出标准类型范围的常量可能使用扩展类型

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：不同进制表示相同值

```c
#include <stdio.h>

int main(void) {
    // 以下变量都初始化为值 42
    int d = 42;          // 十进制
    int o = 052;         // 八进制
    int x = 0x2a;        // 十六进制（小写）
    int X = 0X2A;        // 十六进制（大写）
    int b = 0b101010;    // 二进制 [C23]

    printf("十进制 42: %d\n", d);
    printf("八进制 052: %d\n", o);
    printf("十六进制 0x2a: %d\n", x);
    printf("十六进制 0X2A: %d\n", X);
    printf("二进制 0b101010: %d\n", b);

    return 0;
}
```

**输出**：
```
十进制 42: 42
八进制 052: 42
十六进制 0x2a: 42
十六进制 0X2A: 42
二进制 0b101010: 42
```

#### 示例 2：后缀指定类型

```c
#include <stdio.h>
#include <inttypes.h>

int main(void) {
    unsigned int u = 42U;
    long l = 42L;
    long long ll = 42LL;
    unsigned long long ull = 42ULL;

    printf("unsigned int: %u\n", u);
    printf("long: %ld\n", l);
    printf("long long: %lld\n", ll);
    printf("unsigned long long: %llu\n", ull);

    return 0;
}
```

### 进阶用法

#### 示例 3：数字分隔符 [C23]

```c
#include <stdio.h>

int main(void) {
    // 使用数字分隔符提高可读性
    unsigned long long l1 = 18446744073709550592ull;
    unsigned long long l2 = 18'446'744'073'709'550'592llu;
    unsigned long long l3 = 1844'6744'0737'0955'0592uLL;
    unsigned long long l4 = 184467'440737'0'95505'92LLU;

    printf("l1 = %llu\n", l1);
    printf("l2 = %llu\n", l2);
    printf("l3 = %llu\n", l3);
    printf("l4 = %llu\n", l4);

    return 0;
}
```

#### 示例 4：类型推导演示

```c
#include <stdio.h>
#include <inttypes.h>

int main(void) {
    printf("123 = %d\n", 123);
    printf("0123 = %d\n", 0123);          // 八进制：83
    printf("0x123 = %d\n", 0x123);        // 十六进制：291

    // 大整数自动选择 64 位类型
    printf("12345678901234567890ull = %llu\n",
           12345678901234567890ull);

    // 即使无后缀，也会选择足够大的类型
    printf("12345678901234567890u = %" PRIu64 "\n",
           12345678901234567890u);

    // LLONG_MIN 的正确表示方式
    printf("%lld\n", -9223372036854775807ll - 1);

    return 0;
}
```

**输出**：
```
123 = 123
0123 = 83
0x123 = 291
12345678901234567890ull = 12345678901234567890
12345678901234567890u = 12345678901234567890
-9223372036854775808
```

#### 示例 5：位运算掩码

```c
#include <stdio.h>

// 使用十六进制和二进制定义标志位
#define FLAG_READ    0b00000001  // [C23]
#define FLAG_WRITE   0b00000010
#define FLAG_EXECUTE 0b00000100
#define MASK_ALL     0xFF
#define MASK_UPPER   0xF0
#define MASK_LOWER   0x0F

int main(void) {
    unsigned char permissions = FLAG_READ | FLAG_WRITE;

    printf("Permissions: 0x%02X\n", permissions);

    if (permissions & FLAG_READ) {
        printf("Read permission granted\n");
    }

    if (permissions & FLAG_EXECUTE) {
        printf("Execute permission granted\n");
    } else {
        printf("Execute permission denied\n");
    }

    // 使用掩码提取低 4 位
    unsigned char value = 0xAB;
    printf("Lower nibble of 0x%02X: 0x%02X\n",
           value, value & MASK_LOWER);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：八进制前导零误解

```c
// 错误示例
int month = 09;    // 编译错误：9 不是合法八进制数字
int day = 012;     // 逻辑错误：这是 10，不是 12

// 正确做法
int month = 9;     // 十进制
int day = 12;      // 十进制

// 如果确实需要八进制
int octal_value = 012;  // 值为 10，明确意图
```

#### 错误 2：十六进制与运算符混淆

```c
// 错误示例
int x = 0xE+2;     // 编译错误：被解析为预处理标记

// 正确做法
int x = 0xE + 2;   // 加空格分隔
int y = 0xa+2;     // 正确：a 不是 E，不会混淆
int z = (0xE)+2;   // 使用括号
```

#### 错误 3：大整数负值表示

```c
// 错误示例：9223372036854775808 超出 long long 范围
// printf("%lld\n", -9223372036854775808);  // 编译错误

// 正确方法 1：分开计算
printf("%lld\n", -9223372036854775807ll - 1);

// 正确方法 2：使用 ULL 然后取负
printf("%llu\n", -9223372036854775808ull);
// 注意：这会对无符号值应用负号，结果为 9223372036854775808
```

#### 错误 4：类型推导意外

```c
// 潜在问题
#include <stdio.h>

int main(void) {
    // 在 16 位 int 系统上，32768 可能是 long 类型
    // 这可能导致格式字符串不匹配
    // printf("%d\n", 32768);  // 可能警告或错误

    // 正确做法：显式指定类型
    long value = 32768L;
    printf("%ld\n", value);

    return 0;
}
```

### 综合示例

```c
#include <stdio.h>
#include <inttypes.h>
#include <limits.h>

int main(void) {
    printf("=== 整数常量综合示例 ===\n\n");

    // 1. 进制演示
    printf("1. 不同进制表示值 255：\n");
    printf("   十进制: %d\n", 255);
    printf("   八进制: %d (0o377)\n", 0377);
    printf("   十六进制: %d (0xFF)\n", 0xFF);
    printf("   二进制: %d (0b11111111)\n\n", 0b11111111);  // C23

    // 2. 类型演示
    printf("2. 类型后缀演示：\n");
    printf("   int: %d (size: %zu)\n", 42, sizeof(42));
    printf("   long: %ld (size: %zu)\n", 42L, sizeof(42L));
    printf("   long long: %lld (size: %zu)\n", 42LL, sizeof(42LL));
    printf("   unsigned: %u (size: %zu)\n\n", 42U, sizeof(42U));

    // 3. 位掩码应用
    printf("3. 位掩码应用：\n");
    unsigned int flags = 0;
    flags |= 0b0001;  // 设置位 0
    flags |= 0b0100;  // 设置位 2
    printf("   Flags: 0x%04X\n", flags);
    printf("   Bit 0 set: %s\n", (flags & 0b0001) ? "Yes" : "No");
    printf("   Bit 1 set: %s\n", (flags & 0b0010) ? "Yes" : "No");
    printf("   Bit 2 set: %s\n\n", (flags & 0b0100) ? "Yes" : "No");

    // 4. 数字分隔符 [C23]
    printf("4. 数字分隔符 [C23]：\n");
    long population = 7'800'000'000;
    printf("   世界人口约: %ld\n\n", population);

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **进制多样性**：支持十进制、八进制、十六进制，C23 新增二进制常量支持

2. **类型自动推导**：编译器根据数值、进制和后缀自动选择合适的类型，遵循最小适配原则

3. **后缀机制**：通过 `u/U`、`l/L`、`ll/LL` 等后缀显式指定类型，提高代码可读性和可移植性

4. **无负数常量**：C 语言中不存在负数常量，负号是对正数常量的一元运算符

5. **最大匹配原则**：编译器采用最长匹配策略，需注意十六进制 `e/E` 与运算符的冲突

6. **C23 新特性**：二进制常量（`0b` 前缀）和数字分隔符（`'`）显著提高代码可读性

### 进制选择对比

| 进制 | 前缀 | 适用场景 | 示例 |
|-----|------|---------|------|
| 十进制 | 无 | 普通计数、计算 | `42`, `100` |
| 八进制 | `0` | Unix 权限、遗留代码 | `0755`, `0644` |
| 十六进制 | `0x/0X` | 位掩码、内存地址、颜色值 | `0xFF`, `0xDEADBEEF` |
| 二进制 | `0b/0B` [C23] | 位标志、硬件寄存器 | `0b10101010` |

### 类型推导对比

| 特性 | 十进制 | 八进制/十六进制 |
|-----|--------|----------------|
| 优先类型 | 有符号类型 | 混合类型（可能无符号） |
| 最小类型 | `int` | `int` |
| 溢出处理 | 尝试更大有符号类型 | 先尝试无符号类型 |

### 学习建议

1. **优先使用十进制**：除非有特殊需求，否则使用十进制最直观

2. **位操作用十六进制**：位掩码、颜色值等场景使用十六进制更清晰

3. **权限用八进制**：Unix 文件权限传统上使用八进制表示

4. **显式指定类型**：涉及大整数或特定类型要求时，使用后缀明确类型

5. **避免八进制陷阱**：不要在普通十进制数前加前导零

6. **善用 C23 特性**：二进制常量和数字分隔符显著提高代码可读性

### 相关参考

- C23 标准 (ISO/IEC 9899:2024): 6.4.4.1 Integer constants
- C17 标准 (ISO/IEC 9899:2018): 6.4.4.1 Integer constants
- C11 标准 (ISO/IEC 9899:2011): 6.4.4.1 Integer constants
- C99 标准 (ISO/IEC 9899:1999): 6.4.4.1 Integer constants
- C89/C90 标准 (ISO/IEC 9899:1990): 3.1.3.2 Integer constants