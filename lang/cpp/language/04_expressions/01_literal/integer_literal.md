# 整数字面量 (Integer Literal)

## 1. 概述 (Overview)

**整数字面量 (Integer Literal)** 是 C++ 中用于在程序源代码中直接表示整数值的词法元素。它允许程序员在表达式中直接使用整数类型的值，而无需通过变量或其他计算方式获得。

整数字面量是一种**基本表达式 (Primary Expression)**，它在编译时就被解析为特定类型的整数值，是 C++ 词法分析的重要组成部分。

### 主要用途

- 在代码中直接表示常量值
- 初始化整数类型变量
- 作为数组大小、循环计数器等场景的常量值
- 在预处理指令（如 `#if`、`#elif`）中使用

### 技术定位

整数字面量属于 C++ 词法层的基础概念，是编译器词法分析阶段处理的词法单元。它与浮点字面量、字符字面量、字符串字面量等共同构成了 C++ 的字面量体系。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

整数字面量是 C/C++ 语言最基础的语言特性之一，从 C 语言的早期版本就存在。它允许程序员直接在源代码中书写整数值，并根据数值范围和可选的类型后缀自动推导出合适的类型。

### C++ 标准演进

| 版本 | 特性 | 说明 |
|------|------|------|
| C++98 | 基础整数字面量 | 支持十进制、八进制、十六进制，支持 `u/U`、`l/L` 后缀 |
| C++11 | `long long` 后缀 | 引入 `ll`/`LL` 后缀，支持 64 位整数字面量 |
| C++14 | 二进制字面量 | 引入 `0b`/`0B` 前缀表示二进制数 |
| C++14 | 数字分隔符 | 允许在数字中使用单引号 `'` 作为分隔符，提高可读性 |
| C++23 | `size_t` 后缀 | 引入 `z`/`Z` 后缀，直接表示 `std::size_t` 类型 |

### 设计动机

1. **提高可读性**：二进制字面量和数字分隔符的引入，使大数值和位模式更易读
2. **类型安全**：`size_t` 后缀避免了 `auto` 类型推导时的意外类型转换
3. **向后兼容**：所有扩展都保持了与旧代码的兼容性

### 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|-----------|-----|------|------|
| `__cpp_binary_literals` | `201304L` | C++14 | 二进制字面量 |
| `__cpp_size_t_suffix` | `202011L` | C++23 | `std::size_t` 及其有符号版本的字面量后缀 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

整数字面量的通用形式为：

```
前缀 数字序列 后缀(可选)
```

### 四种进制表示

#### 1. 十进制字面量 (Decimal Literal)

```
非零数字 [数字序列]
```

- 以非零数字（1-9）开头
- 后续可跟任意数量的十进制数字（0-9）
- 示例：`42`, `123`, `999999`

#### 2. 八进制字面量 (Octal Literal)

```
0 [八进制数字序列]
```

- 以数字 `0` 开头
- 后续可跟任意数量的八进制数字（0-7）
- 示例：`052`, `0777`, `0644`

#### 3. 十六进制字面量 (Hexadecimal Literal)

```
0x|0X 十六进制数字序列
```

- 以 `0x` 或 `0X` 开头
- 后续可跟任意数量的十六进制数字（0-9, a-f, A-F）
- 示例：`0x2a`, `0XFF`, `0xDEADBEEF`

#### 4. 二进制字面量 (Binary Literal, C++14 起)

```
0b|0B 二进制数字序列
```

- 以 `0b` 或 `0B` 开头
- 后续可跟任意数量的二进制数字（0-1）
- 示例：`0b101010`, `0B11111111`

### 类型后缀

类型后缀用于显式指定整数字面量的类型。

| 后缀 | 含义 | 示例 |
|------|------|------|
| `u` 或 `U` | 无符号类型 (unsigned) | `42u`, `0xFFU` |
| `l` 或 `L` | 长整型 (long) | `42L`, `0xFFl` |
| `ll` 或 `LL` | 长长整型 (long long, C++11 起) | `42LL`, `0xFFLL` |
| `z` 或 `Z` | size_t 类型 (C++23 起) | `42z`, `0xFFZ` |

后缀可以组合使用：
- `ul`/`UL`/`uL`/`Ul`：无符号长整型
- `ull`/`ULL`：无符号长长整型
- `uz`/`UZ`：无符号 size_t 类型（等同于 `size_t`）

### 数字分隔符 (C++14 起)

可以在数字之间插入单引号 `'` 作为分隔符，提高大数值的可读性：

```cpp
int million = 1'000'000;
int binary = 0b1010'1010;
int hex = 0xDEAD'BEEF;
```

### 语法规则表

| 进制 | 前缀 | 允许的数字 | 示例 |
|------|------|-----------|------|
| 十进制 | 无 | 0-9 | `42`, `1'000'000` |
| 八进制 | `0` | 0-7 | `0755`, `0644` |
| 十六进制 | `0x` 或 `0X` | 0-9, a-f, A-F | `0xFF`, `0Xdeadbeef` |
| 二进制 | `0b` 或 `0B` | 0-1 | `0b1010`, `0B1111'1111` |

---

## 4. 底层原理 (Underlying Principles)

### 类型推导机制

整数字面量的类型由编译器根据**数值大小**、**进制**和**后缀**共同决定。编译器会选择能够容纳该值的第一个类型。

### 类型推导规则表

| 后缀 | 十进制进制 | 二进制、八进制、十六进制 |
|------|-----------|----------------------|
| 无后缀 | `int` → `long int` → `long long int` (C++11 起) | `int` → `unsigned int` → `long int` → `unsigned long int` → `long long int` (C++11 起) → `unsigned long long int` (C++11 起) |
| `u` 或 `U` | `unsigned int` → `unsigned long int` → `unsigned long long int` (C++11 起) | `unsigned int` → `unsigned long int` → `unsigned long long int` (C++11 起) |
| `l` 或 `L` | `long int` → `unsigned long int` (C++11 前为 `long long int`) → `long long int` (C++11 起) | `long int` → `unsigned long int` → `long long int` (C++11 起) → `unsigned long long int` (C++11 起) |
| `l`/`L` + `u`/`U` | `unsigned long int` → `unsigned long long int` (C++11 起) | `unsigned long int` → `unsigned long long int` (C++11 起) |
| `ll` 或 `LL` (C++11 起) | `long long int` | `long long int` → `unsigned long long int` |
| `ll`/`LL` + `u`/`U` (C++11 起) | `unsigned long long int` | `unsigned long long int` |
| `z` 或 `Z` (C++23 起) | `std::size_t` 的有符号版本 | `std::size_t` 的有符号版本 → `std::size_t` |
| `z`/`Z` + `u`/`U` (C++23 起) | `std::size_t` | `std::size_t` |

**关键规则**：
- 对于十进制，类型列表中**不包含**无符号类型（除非显式使用 `u` 后缀）
- 对于二进制、八进制、十六进制，类型列表中**包含**无符号类型
- 编译器从左到右选择第一个能容纳该值的类型

### 最大匹配原则 (Maximal Munch)

编译器在词法分析时采用最大匹配原则，即尽可能长地匹配词法单元。这导致十六进制字面量后跟 `e` 或 `E` 再接运算符时可能产生歧义：

```cpp
auto x = 0xE+2.0;   // 错误：被解析为预处理数字 token "0xE+2.0"
auto y = 0xa+2.0;   // 正确：被解析为 0xa + 2.0
auto z = 0xE +2.0;  // 正确：空格分隔
auto q = (0xE)+2.0; // 正确：括号分隔
```

### 预处理中的类型

在 `#if` 和 `#elif` 控制表达式中（C++11 起）：
- 所有有符号整型常量表现为 `std::intmax_t` 类型
- 所有无符号整型常量表现为 `std::uintmax_t` 类型

### 扩展整数类型

如果整数字面量的值太大，无法放入任何标准类型，且编译器支持扩展整数类型（如 `__int128`），则该字面量可以被赋予扩展整数类型。否则，程序是非良构的 (ill-formed)。

**注意**：C++23 起，带 `z`/`Z` 后缀的字面量如果值太大，程序直接非良构，不会尝试使用扩展整数类型。

---

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 常量初始化

```cpp
const int MAX_SIZE = 100;
const unsigned int MASK = 0xFF00FF00;
constexpr long long BIG_NUMBER = 1'000'000'000'000LL;
```

#### 2. 位掩码与标志位

```cpp
// 权限标志（八进制）
int permissions = 0755;  // rwxr-xr-x

// 位掩码（十六进制）
unsigned int red   = 0xFF0000;
unsigned int green = 0x00FF00;
unsigned int blue  = 0x0000FF;

// 位标志（二进制，C++14 起）
unsigned char flags = 0b00001111;
```

#### 3. 大数值表示（使用数字分隔符）

```cpp
// 金融应用
long long revenue = 1'234'567'890LL;

// 科学计算
long long avogadro = 6'022'140'760'000'000'000LL;
```

#### 4. 平台无关类型（C++23 起）

```cpp
// 自动匹配 size_t 类型
auto size = 0uz;  // std::size_t
auto diff = 0z;   // std::make_signed_t<std::size_t>
```

### 最佳实践

#### 1. 使用正确的大小写后缀

```cpp
// 推荐：使用大写后缀，避免与数字混淆
long val = 42L;      // 好
long bad = 42l;      // 差：容易与数字 1 混淆

long long ll_val = 42LL;  // 好
```

#### 2. 十六进制字面量的注意事项

```cpp
// 避免在十六进制字面量后直接使用 + 或 - 运算符
auto a = 0xE + 2.0;   // 好：有空格分隔
auto b = (0xE)+2.0;   // 好：用括号分隔
// auto c = 0xE+2.0;  // 错误：解析为预处理数字
```

#### 3. 大数值使用分隔符

```cpp
// 好：可读性强
int million = 1'000'000;
int credit_card = 1234'5678'9012'3456;

// 差：可读性差
int million_bad = 1000000;
```

#### 4. 位操作使用二进制或十六进制

```cpp
// 好：意图清晰
uint8_t mask = 0b11110000;  // C++14 起
uint8_t mask2 = 0xF0;       // 等价的十六进制

// 差：需要计算
uint8_t mask_bad = 240;
```

### 常见陷阱

#### 1. 八进制陷阱

```cpp
int a = 010;  // 这是 8，不是 10！
int b = 08;   // 错误：8 不是有效的八进制数字
int c = 0;    // 这是 0
int d = 00;   // 这也是 0（八进制）
```

**建议**：如果意图是十进制，不要以 `0` 开头（除非就是 `0` 本身）。

#### 2. 负数字面量不存在

```cpp
// 不存在负整数字面量
auto x = -42;  // 这是一元负号运算符应用于正字面量 42

// 这会影响类型推导
auto a = -2147483648;     // 2147483648 是 int（假设 int 是 32 位）
                          // 对 int 应用一元负号，结果仍是 int

auto b = -2147483649;     // 2147483649 可能是 long long
                          // 结果是 long long
```

对于 `LLONG_MIN`：

```cpp
// 错误方式
auto wrong = -9223372036854775808;  // 9223372036854775808 无法放入 long long

// 正确方式
auto right = -9223372036854775807 - 1;  // 先计算再减 1
auto also_right = LLONG_MIN;            // 使用宏
```

#### 3. 类型溢出

```cpp
// 十进制 vs 其他进制的类型推导差异
auto dec = 4294967295;    // int → long → long long（有符号类型优先）
auto hex = 0xFFFFFFFF;    // int → unsigned int（匹配！）

// 结果类型不同
static_assert(std::is_same_v<decltype(dec), long long>);
static_assert(std::is_same_v<decltype(hex), unsigned int>);
```

---

## 6. 代码示例 (Examples)

### 基础用法

#### 不同进制的整数字面量

```cpp
#include <iostream>

int main() {
    // 十进制
    int decimal = 42;

    // 八进制（以 0 开头）
    int octal = 052;  // 等于十进制的 42

    // 十六进制（以 0x 或 0X 开头）
    int hex_lower = 0x2a;  // 等于十进制的 42
    int hex_upper = 0X2A;  // 同上

    // 二进制（C++14 起，以 0b 或 0B 开头）
    int binary = 0b101010;  // 等于十进制的 42

    std::cout << "decimal: " << decimal << '\n';
    std::cout << "octal: " << octal << '\n';
    std::cout << "hex_lower: " << hex_lower << '\n';
    std::cout << "hex_upper: " << hex_upper << '\n';
    std::cout << "binary: " << binary << '\n';

    return 0;
}
```

输出：
```
decimal: 42
octal: 42
hex_lower: 42
hex_upper: 42
binary: 42
```

#### 类型后缀的使用

```cpp
#include <iostream>
#include <type_traits>

int main() {
    // 无符号后缀
    auto u_val = 42u;
    std::cout << "u_val type is unsigned: "
              << std::is_unsigned_v<decltype(u_val)> << '\n';

    // 长整型后缀
    auto l_val = 42L;
    std::cout << "l_val type is long: "
              << std::is_same_v<decltype(l_val), long> << '\n';

    // 长长整型后缀（C++11 起）
    auto ll_val = 42LL;
    std::cout << "ll_val type is long long: "
              << std::is_same_v<decltype(ll_val), long long> << '\n';

    // 组合后缀
    auto ull_val = 42ULL;  // unsigned long long
    std::cout << "ull_val type is unsigned long long: "
              << std::is_same_v<decltype(ull_val), unsigned long long> << '\n';

    return 0;
}
```

输出：
```
u_val type is unsigned: 1
l_val type is long: 1
ll_val type is long long: 1
ull_val type is unsigned long long: 1
```

#### 数字分隔符（C++14 起）

```cpp
#include <iostream>

int main() {
    // 使用单引号作为数字分隔符，提高可读性
    int million = 1'000'000;
    int billion = 1'000'000'000;

    // 可以在任意位置使用
    int flexible = 1'2'3'4'5;

    // 二进制中的使用
    int binary = 0b1111'0000'1111'0000;

    // 十六进制中的使用
    int hex = 0xDEAD'BEEF;

    // 验证分隔符不影响数值
    std::cout << "million: " << million << '\n';
    std::cout << "billion: " << billion << '\n';
    std::cout << "flexible: " << flexible << '\n';
    std::cout << "binary: " << binary << '\n';
    std::cout << "hex: " << hex << '\n';

    return 0;
}
```

输出：
```
million: 1000000
billion: 1000000000
flexible: 12345
binary: 61680
hex: 3735928559
```

### 高级用法

#### Size_t 后缀（C++23 起）

```cpp
#include <cstddef>
#include <iostream>
#include <type_traits>

int main() {
#if __cpp_size_t_suffix >= 202011L
    // 使用 z 后缀
    auto size_val = 0uz;  // unsigned size_t
    auto signed_size = 0z; // signed version of size_t

    static_assert(std::is_same_v<decltype(size_val), std::size_t>);
    static_assert(std::is_same_v<decltype(signed_size),
                                 std::make_signed_t<std::size_t>>);

    std::cout << "size_val type is size_t\n";
    std::cout << "signed_size type is signed size_t\n";
#else
    std::cout << "C++23 size_t suffix not supported\n";
#endif

    return 0;
}
```

#### 类型推导示例

```cpp
#include <iostream>
#include <climits>
#include <type_traits>

int main() {
    // 十进制：倾向于有符号类型
    auto small = 100;          // int
    auto big = 2147483648;      // long 或 long long（取决于平台）

    // 十六进制：可以是无符号类型
    auto hex_max = 0xFFFFFFFF;  // unsigned int（在 32 位 int 平台）

    std::cout << "small is int: "
              << std::is_same_v<decltype(small), int> << '\n';
    std::cout << "hex_max is unsigned: "
              << std::is_unsigned_v<decltype(hex_max)> << '\n';

    // 显式指定类型
    auto explicit_ll = 100LL;           // long long
    auto explicit_ull = 100ULL;         // unsigned long long

    std::cout << "explicit_ll is long long: "
              << std::is_same_v<decltype(explicit_ll), long long> << '\n';
    std::cout << "explicit_ull is unsigned long long: "
              << std::is_same_v<decltype(explicit_ull), unsigned long long>
              << '\n';

    return 0;
}
```

#### 预处理指令中的使用

```cpp
#include <iostream>

#define VERSION 0x0102  // 版本 1.2

int main() {
#if VERSION >= 0x0200
    std::cout << "Version 2.0 or later\n";
#elif VERSION >= 0x0100
    std::cout << "Version 1.x\n";
#else
    std::cout << "Legacy version\n";
#endif

    // 位操作标志
    const int READ    = 0b001;
    const int WRITE   = 0b010;
    const int EXECUTE = 0b100;

    int permissions = READ | WRITE;

    if (permissions & READ) {
        std::cout << "Has read permission\n";
    }
    if (permissions & EXECUTE) {
        std::cout << "Has execute permission\n";
    }

    return 0;
}
```

### 常见错误及修正

#### 错误 1：八进制误解

```cpp
#include <iostream>

int main() {
    // 错误：意图是十进制 10，但写成了八进制
    int months = 010;  // 实际值是 8

    std::cout << "months = " << months << '\n';  // 输出 8，不是 10

    // 修正：去掉前导零
    int months_correct = 10;
    std::cout << "months_correct = " << months_correct << '\n';

    return 0;
}
```

输出：
```
months = 8
months_correct = 10
```

#### 错误 2：LLONG_MIN 的错误表示

```cpp
#include <iostream>
#include <climits>

int main() {
    // 错误：没有负数字面量，9223372036854775808 超出 long long 范围
    // long long min_wrong = -9223372036854775808;  // 编译错误

    // 修正方法 1：分开计算
    long long min_correct1 = -9223372036854775807 - 1;

    // 修正方法 2：使用宏
    long long min_correct2 = LLONG_MIN;

    std::cout << "LLONG_MIN = " << min_correct1 << '\n';
    std::cout << "LLONG_MIN = " << min_correct2 << '\n';

    // 正确处理无符号的大值
    unsigned long long big = 9223372036854775808ULL;
    std::cout << "big unsigned = " << big << '\n';

    return 0;
}
```

#### 错误 3：十六进制后跟运算符

```cpp
#include <iostream>

int main() {
    // 错误：最大匹配原则导致解析错误
    // auto bad = 0xE+2.0;  // 编译错误：无效的预处理数字

    // 修正方法 1：添加空格
    auto good1 = 0xE + 2.0;

    // 修正方法 2：使用括号
    auto good2 = (0xE) + 2.0;

    // 修正方法 3：避免 e/E 结尾的十六进制
    auto good3 = 0xF + 2.0;  // 使用 F 而不是 E

    std::cout << "good1 = " << good1 << '\n';
    std::cout << "good2 = " << good2 << '\n';
    std::cout << "good3 = " << good3 << '\n';

    return 0;
}
```

输出：
```
good1 = 16
good2 = 16
good3 = 17
```

#### 错误 4：类型推导的意外结果

```cpp
#include <iostream>
#include <type_traits>

int main() {
    // 十进制：不使用无符号类型
    auto dec = 4294967295;  // 在 32 位 int 平台上，这会是 long long

    // 十六进制：可以使用无符号类型
    auto hex = 0xFFFFFFFF;  // unsigned int

    std::cout << "dec is signed: " << std::is_signed_v<decltype(dec)> << '\n';
    std::cout << "hex is unsigned: " << std::is_unsigned_v<decltype(hex)> << '\n';

    // 如果意图是无符号，显式使用后缀
    auto dec_unsigned = 4294967295u;
    std::cout << "dec_unsigned is unsigned: "
              << std::is_unsigned_v<decltype(dec_unsigned)> << '\n';

    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

1. **四种进制表示**：十进制、八进制（前缀 `0`）、十六进制（前缀 `0x`/`0X`）、二进制（C++14 起，前缀 `0b`/`0B`）

2. **类型后缀系统**：
   - `u`/`U`：无符号类型
   - `l`/`L`：长整型
   - `ll`/`LL`：长长整型（C++11 起）
   - `z`/`Z`：size_t 类型（C++23 起）

3. **类型推导规则**：
   - 十进制倾向于有符号类型
   - 二进制/八进制/十六进制可以使用无符号类型
   - 编译器选择能容纳该值的第一个类型

4. **数字分隔符**（C++14 起）：使用单引号 `'` 提高大数值可读性

### C++ 版本特性对比

| 特性 | C++98 | C++11 | C++14 | C++23 |
|------|-------|-------|-------|-------|
| 十进制字面量 | ✓ | ✓ | ✓ | ✓ |
| 八进制字面量 | ✓ | ✓ | ✓ | ✓ |
| 十六进制字面量 | ✓ | ✓ | ✓ | ✓ |
| 二进制字面量 | ✗ | ✗ | ✓ | ✓ |
| `ll`/`LL` 后缀 | ✗ | ✓ | ✓ | ✓ |
| 数字分隔符 | ✗ | ✗ | ✓ | ✓ |
| `z`/`Z` 后缀 | ✗ | ✗ | ✗ | ✓ |

### 最佳实践建议

1. **使用大写后缀**：`L` 比 `l` 更清晰，避免与数字 `1` 混淆
2. **避免八进制陷阱**：除非性能明确需要，否则不要使用前导零
3. **使用数字分隔符**：提高大数值的可读性（C++14 起）
4. **显式指定类型**：当类型重要时，使用后缀明确类型
5. **注意十六进制的 `E`**：避免与运算符产生歧义

### 学习建议

1. **理解类型推导规则**：掌握不同进制和后缀对类型的影响
2. **掌握最大匹配原则**：避免词法分析的歧义
3. **熟悉 C++ 标准演进**：了解新特性的引入时间和用途
4. **实践编码规范**：建立清晰的编码习惯，减少错误

### 相关主题

- 浮点字面量 (Floating Point Literal)
- 字符字面量 (Character Literal)
- 字符串字面量 (String Literal)
- 用户定义字面量 (User-defined Literal, C++11 起)
- 常量表达式 (Constant Expression)

---

## 参考

- C++23 标准 (ISO/IEC 14882:2024)：5.13.2 Integer literals [lex.icon]
- C++20 标准 (ISO/IEC 14882:2020)：5.13.2 Integer literals [lex.icon]
- C++17 标准 (ISO/IEC 14882:2017)：5.13.2 Integer literals [lex.icon]
- C++14 标准 (ISO/IEC 14882:2014)：2.14.2 Integer literals [lex.icon]
- C++11 标准 (ISO/IEC 14882:2011)：2.14.2 Integer literals [lex.icon]
- C++98 标准 (ISO/IEC 14882:1998)：2.13.1 Integer literals [lex.icon]