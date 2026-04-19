# 浮点字面量 (Floating-point Literal)

## 1. 概述 (Overview)

浮点字面量（Floating-point Literal）是 C++ 中用于在源代码中直接表示浮点数常量的语法结构。它定义了一个编译时常量（compile-time constant），其值在源文件中直接指定。

浮点字面量支持两种表示形式：
- **十进制浮点字面量**：使用十进制科学计数法
- **十六进制浮点字面量**（C++17 起）：使用十六进制表示

浮点字面量的类型由后缀决定，默认为 `double` 类型。它广泛用于需要精确浮点数值的场景，如数学计算、物理模拟、科学计算等领域。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

浮点字面量是 C++ 继承自 C 语言的基础特性。在早期的编程语言设计中，浮点数的字面表示是一个重要问题，因为浮点数无法用二进制精确表示所有十进制小数。

### 版本变更

| 版本 | 特性变更 |
|------|----------|
| C++98 | 基础十进制浮点字面量，支持 `f`、`l`、`F`、`L` 后缀 |
| C++11 | I/O 函数开始支持解析和打印十六进制浮点数（`std::hexfloat`） |
| C++14 | 引入数字分隔符（单引号 `'`），提高可读性 |
| C++17 | 正式引入十六进制浮点字面量（`0x` 或 `0X` 前缀） |
| C++23 | 新增 `f16`、`f32`、`f64`、`f128`、`bf16` 等后缀，支持标准浮点类型 |

### 设计动机

- **十六进制浮点字面量**：解决十进制浮点数在二进制表示中的精度问题，使程序员能够精确指定浮点数的二进制表示
- **数字分隔符**：提高长数字的可读性，如 `3.14'15'92`
- **类型后缀扩展**：支持 C++23 标准库中的扩展浮点类型（`std::float16_t` 等）

## 3. 语法与参数 (Syntax and Parameters)

### 十进制浮点字面量语法

十进制浮点字面量有以下三种基本形式：

| 形式 | 语法 | 说明 |
|------|------|------|
| 形式 1 | `digit-sequence decimal-exponent suffix(可选)` | 整数部分，必须包含指数 |
| 形式 2 | `digit-sequence . decimal-exponent(可选) suffix(可选)` | 带小数点的整数部分 |
| 形式 3 | `digit-sequence(可选) . digit-sequence decimal-exponent(可选) suffix(可选)` | 完整的小数表示 |

其中：
- **digit-sequence**：十进制数字序列（0-9）
- **decimal-exponent**：十进制指数，形式为 `e` 或 `E` 后跟可选符号和数字序列
- **suffix**：类型后缀（见下文）

**示例：**
```cpp
1e10        // 形式 1：整数部分 + 指数
1e-5L       // 形式 1：带后缀和负指数
1.          // 形式 2：整数部分 + 小数点
1.e-2       // 形式 2：带指数
3.14        // 形式 3：完整小数
.1f         // 形式 3：无整数部分
0.1e-1L     // 形式 3：完整形式
```

### 十六进制浮点字面量语法（C++17 起）

十六进制浮点字面量以 `0x` 或 `0X` 开头：

| 形式 | 语法 | 说明 |
|------|------|------|
| 形式 4 | `0x\|0X hex-digit-sequence hex-exponent suffix(可选)` | 十六进制整数部分 |
| 形式 5 | `0x\|0X hex-digit-sequence . hex-exponent suffix(可选)` | 带小数点 |
| 形式 6 | `0x\|0X hex-digit-sequence(可选) . hex-digit-sequence hex-exponent suffix(可选)` | 完整形式 |

其中：
- **hex-digit-sequence**：十六进制数字序列（0-9, a-f, A-F）
- **hex-exponent**：十六进制指数，形式为 `p` 或 `P` 后跟可选符号和十进制数字序列，表示 2 的幂次

**注意**：十六进制浮点字面量的指数部分**不可省略**。

**示例：**
```cpp
0x1ffp10    // 十六进制整数部分 + 指数
0X0p-1      // 零值
0x1.p0      // 带小数点
0xf.p-1     // 带负指数
0x0.123p-1  // 完整小数形式
0xa.bp10l   // 带后缀
```

### 类型后缀

后缀决定浮点字面量的类型：

| 后缀 | 类型 | 版本 |
|------|------|------|
| 无后缀 | `double` | 所有版本 |
| `f` 或 `F` | `float` | 所有版本 |
| `l` 或 `L` | `long double` | 所有版本 |
| `f16` 或 `F16` | `std::float16_t` | C++23 |
| `f32` 或 `F32` | `std::float32_t` | C++23 |
| `f64` 或 `F64` | `std::float64_t` | C++23 |
| `f128` 或 `F128` | `std::float128_t` | C++23 |
| `bf16` 或 `BF16` | `std::bfloat16_t` | C++23 |

### 数字分隔符（C++14 起）

单引号 `'` 可插入数字之间作为分隔符，编译时会被忽略：

```cpp
3.14'15'92     // 等同于 3.141592
1'000.5        // 等同于 1000.5
```

## 4. 底层原理 (Underlying Principles)

### 十进制科学计数法

十进制浮点字面量的值计算公式为：

```
值 = 有效数字 × 10^指数
```

例如：`123e4` 的数学含义是 **123×10^4 = 1,230,000**

### 十六进制浮点表示（C++17）

十六进制浮点字面量的值计算公式为：

```
值 = 十六进制有效数字 × 2^指数
```

例如：`0x1.4p3` 的计算过程：
1. 十六进制有效数字 `1.4` 转换为十进制为 `1.25`
2. 指数 `p3` 表示 2^3 = 8
3. 最终值：`1.25 × 8 = 10.0`

### 类型转换与精度

浮点字面量在编译时被转换为相应类型的内部表示：

```cpp
// float 的精度限制
123.456e-67f   // 转换为 float 时可能截断为零

// 类型比较陷阱
3.4028234e38f  // float
3.4028234e38   // double（不同的内部表示）
static_assert(3.4028234e38f != 3.4028234e38);  // 不同类型，值不同
```

### 编译时常量

浮点字面量是编译时常量，在编译阶段完成解析和转换：

```cpp
// 编译时计算
constexpr double pi = 3.14159265358979;
static_assert(pi > 3.14);  // 编译时断言
```

## 5. 使用场景 (Use Cases)

### 适用场景

1. **数学常数定义**
   ```cpp
   constexpr double pi = 3.14159265358979;
   constexpr double e = 2.71828182845904;
   ```

2. **科学计算**
   ```cpp
   double avogadro = 6.02214076e23;  // 阿伏伽德罗常数
   double planck = 6.62607015e-34;    // 普朗克常数
   ```

3. **精确二进制浮点表示（C++17）**
   ```cpp
   // 需要精确控制浮点数的二进制表示时使用十六进制字面量
   double exact = 0x1.0p0;  // 精确表示 1.0
   ```

4. **类型敏感的数值计算**
   ```cpp
   float a = 1.0f;      // 明确使用 float
   long double b = 1.0L; // 明确使用 long double
   ```

### 最佳实践

1. **明确指定类型后缀**：避免隐式类型转换带来的精度问题
   ```cpp
   float f = 3.14f;    // 推荐：明确指定 float
   float g = 3.14;      // 不推荐：double 到 float 的隐式转换
   ```

2. **使用数字分隔符提高可读性**（C++14）
   ```cpp
   double large = 1'000'000.5;     // 推荐
   double small = 0.000'001;       // 推荐
   ```

3. **使用科学计数法表示极大或极小的数**
   ```cpp
   double big = 6.022e23;   // 推荐
   double big2 = 602200000000000000000000.0;  // 不推荐：难以阅读
   ```

### 常见陷阱

1. **类型不匹配导致精度问题**
   ```cpp
   float f = 3.4028234e38f;   // float 最大值
   double d = 3.4028234e38;   // double，不同的内部表示
   // 即使数值相同，类型不同可能导致比较失败
   static_assert(f != d);     // 通过：float 提升为 double 后值不同
   ```

2. **十六进制浮点字面量必须包含指数**
   ```cpp
   // 错误：缺少指数
   // double x = 0x1.5;  // 编译错误

   // 正确：包含指数
   double x = 0x1.5p0;  // 正确
   ```

3. **混淆十六进制整数和十六进制浮点数**
   ```cpp
   int i = 0x1e5;        // 十六进制整数，值为 485
   double f = 0x1e5;     // 仍是整数 485（隐式转换）
   double g = 0x1.5p0;   // 十六进制浮点数，值为 1.3125
   ```

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
    // 不同类型的浮点字面量
    double d1 = 58.;           // double，带小数点
    double d2 = 4e2;            // double，科学计数法
    double d3 = 123.456e-67;    // double，负指数

    float f1 = 123.456e-67f;    // float，可能截断为零
    float f2 = .1E4f;           // float，.1E4 = 1000.0

    long double ld = 1.18e-4932l;  // long double

    std::cout << "double d1 = " << d1 << '\n'
              << "double d2 = " << d2 << '\n'
              << "double d3 = " << d3 << '\n'
              << "float f1  = " << f1 << '\n'  // 输出: 0
              << "float f2  = " << f2 << '\n'
              << "long double ld = " << ld << '\n';

    return 0;
}
```

### 十六进制浮点字面量（C++17）

```cpp
#include <iostream>
#include <iomanip>

int main() {
    // 十六进制浮点字面量
    double h1 = 0x10.1p0;   // 16 + 1/16 = 16.0625
    double h2 = 0x1p5;      // 1 * 32 = 32
    double h3 = 0x1.4p3;     // 1.25 * 8 = 10.0

    std::cout << "0x10.1p0 = " << h1 << '\n'  // 16.0625
              << "0x1p5    = " << h2 << '\n'  // 32
              << "0x1.4p3  = " << h3 << '\n'; // 10

    // 精确表示浮点数
    std::cout << std::hexfloat;
    std::cout << "10.0 in hexfloat: " << 10.0 << '\n';

    return 0;
}
```

### 数字分隔符（C++14）

```cpp
#include <iostream>

int main() {
    // 使用单引号分隔数字，提高可读性
    double pi = 3.14'15'92'65'35'89'79;
    double large = 1'000'000.5;
    double tiny = 0.000'000'001;

    std::cout << "pi    = " << pi << '\n'    // 3.14159
              << "large = " << large << '\n'  // 1000000.5
              << "tiny  = " << tiny << '\n';  // 1e-09

    return 0;
}
```

### 类型比较陷阱

```cpp
#include <iostream>
#include <limits>
#include <iomanip>

int main() {
    // 注意：float 和 double 的精度差异
    float f = 3.4028234e38f;
    double d = 3.4028234e38;

    std::cout << std::setprecision(39);
    std::cout << "float  = " << f << '\n';
    std::cout << "double = " << d << '\n';

    // float 转换为 double 后值不同
    static_assert(f != d, "float and double have different values!");

    // float 类型的最大值
    static_assert(f == std::numeric_limits<float>::max());

    // 不同后缀的 float 值可能相等（精度限制）
    static_assert(3.4028234e38f == 3.4028235e38f);  // 都舍入到相同值

    // 但 double 类型可以区分
    static_assert(3.4028234e38 != 3.4028235e38);  // double 精度更高

    return 0;
}
```

### 完整示例

```cpp
#include <iomanip>
#include <iostream>
#include <limits>
#include <typeinfo>

#define OUT(x) '\n' << std::setw(16) << #x << x

int main() {
    std::cout
        << "Literal" "\t" "Printed value" << std::left
        << OUT( 58.            ) // double
        << OUT( 4e2            ) // double
        << OUT( 123.456e-67    ) // double
        << OUT( 123.456e-67f   ) // float, truncated to zero
        << OUT( .1E4f          ) // float
        << OUT( 0x10.1p0       ) // double
        << OUT( 0x1p5          ) // double
        << OUT( 0x1e5          ) // integer literal, not floating-point
        << OUT( 3.14'15'92     ) // double, single quotes ignored (C++14)
        << OUT( 1.18e-4932l    ) // long double
        << std::setprecision(39)
        << OUT( 3.4028234e38f  ) // float
        << OUT( 3.4028234e38   ) // double
        << OUT( 3.4028234e38l  ) // long double
        << '\n';

    static_assert(3.4028234e38f == std::numeric_limits<float>::max());

    static_assert(3.4028234e38f ==  // ends with 4
                  3.4028235e38f);   // ends with 5

    static_assert(3.4028234e38 !=   // ends with 4
                  3.4028235e38);    // ends with 5

    // Both floating-point constants below are 3.4028234e38
    static_assert(3.4028234e38f !=  // a float (then promoted to double)
                  3.4028234e38);    // a double

    return 0;
}
```

**可能的输出：**
```
Literal         Printed value
58.             58
4e2             400
123.456e-67     1.23456e-65
123.456e-67f    0
.1E4f           1000
0x10.1p0        16.0625
0x1p5           32
0x1e5           485
3.14'15'92      3.14159
1.18e-4932l     1.18e-4932
3.4028234e38f   340282346638528859811704183484516925440
3.4028234e38    340282339999999992395853996843190976512
3.4028234e38l   340282339999999999995912555211526242304
```

## 7. 总结 (Summary)

### 核心要点

1. **浮点字面量**是 C++ 中表示编译时浮点数常量的语法结构
2. 支持两种形式：
   - **十进制浮点字面量**：使用十进制科学计数法
   - **十六进制浮点字面量**（C++17）：使用十六进制表示，指数为 2 的幂
3. **类型由后缀决定**：`f/F` 为 `float`，`l/L` 为 `long double`，无后缀为 `double`
4. **数字分隔符**（C++14）：单引号 `'` 可用于提高可读性

### 类型对比

| 特性 | 十进制字面量 | 十六进制字面量 (C++17) |
|------|-------------|----------------------|
| 前缀 | 无 | `0x` 或 `0X` |
| 指数符号 | `e` 或 `E` | `p` 或 `P` |
| 指数基数 | 10 | 2 |
| 指数可省略 | 视情况而定 | **不可省略** |
| 精度控制 | 近似十进制表示 | 精确二进制表示 |

### 学习建议

1. **掌握基本语法**：熟悉三种十进制形式和三种十六进制形式的语法规则
2. **理解类型后缀**：根据精度需求选择合适的浮点类型
3. **注意精度问题**：理解 `float`、`double`、`long double` 的精度差异
4. **使用十六进制字面量**：当需要精确控制浮点数的二进制表示时使用
5. **善用数字分隔符**：提高长数字的可读性

### 相关特性测试宏

| 特性测试宏 | 值 | 版本 | 特性 |
|-----------|-----|------|------|
| `__cpp_hex_float` | `201603L` | C++17 | 十六进制浮点字面量 |

### 参见

- **用户定义字面量**（User-defined literals, C++11）：允许自定义字面量后缀
- **std::hexfloat**（C++11）：I/O 操纵符，用于十六进制浮点数的输入输出
- **std::strtof**：C 标准库函数，支持解析十六进制浮点字符串