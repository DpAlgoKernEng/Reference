# 浮点常量（Floating Constant）

## 1. 概述（Overview）

**浮点常量**（Floating Constant）是 C 语言中用于在源代码中直接表示浮点数值的字面量。它允许程序员在表达式中直接使用浮点类型的值，而无需通过变量或计算。

浮点常量是非左值表达式，其值在编译期确定，用于初始化浮点变量或在表达式中提供浮点操作数。C 语言支持两种浮点常量形式：
- **十进制浮点常量**（Decimal Floating Constant）：使用十进制数字表示
- **十六进制浮点常量**（Hexadecimal Floating Constant）：使用十六进制数字表示（C99 起）

## 2. 来源与演变（Origin and Evolution）

### C89/C90 标准
最初的 C 语言标准定义了基本的十进制浮点常量语法，支持 `float`、`double` 和 `long double` 三种类型。

### C99 标准
C99 标准引入了重要扩展：
- 新增十六进制浮点常量，使用 `0x` 或 `0X` 前缀
- 使用 `p` 或 `P` 作为指数分隔符，指数表示 2 的幂次
- 提供了更精确的浮点数表示方式，避免十进制转换误差

### C23 标准
C23 标准进一步扩展：
- 支持十进制浮点类型（`_Decimal32`、`_Decimal64`、`_Decimal128`）
- 引入数字分隔符（单引号 `'`），提高可读性
- 新增后缀 `df`/`DF`、`dd`/`DD`、`dl`/`DL`

### 设计动机
浮点常量的设计旨在：
1. 提供直观的浮点数值表示方式
2. 支持不同精度需求（单精度、双精度、扩展精度）
3. 确保编译期常量计算的精确性
4. 十六进制形式避免十进制到二进制的转换舍入误差

## 3. 语法与参数（Syntax and Parameters）

### 基本语法结构

浮点常量的完整语法形式为：

```
significand exponent(optional) suffix(optional)
```

#### 有效数字（Significand）

有效数字部分的形式：

```
whole-number(optional) .(optional) fraction(optional)
```

#### 指数（Exponent）

**十进制浮点常量的指数：**
```
e | E exponent-sign(optional) digit-sequence
```

**十六进制浮点常量的指数：**（C99 起）
```
p | P exponent-sign(optional) digit-sequence
```

### 后缀说明

| 后缀 | 类型 | 说明 |
|------|------|------|
| 无后缀 | `double` | 默认类型 |
| `f` 或 `F` | `float` | 单精度浮点 |
| `l` 或 `L` | `long double` | 扩展精度浮点 |
| `df` 或 `DF` | `_Decimal32` | 十进制浮点（C23 起）|
| `dd` 或 `DD` | `_Decimal64` | 十进制浮点（C23 起）|
| `dl` 或 `DL` | `_Decimal128` | 十进制浮点（C23 起）|

### 数字分隔符（C23 起）

可选的单引号 `'` 可以插入在数字之间作为分隔符，编译时会被忽略：

```c
double value = 1'000'000.5;  // 等价于 1000000.5
```

### 语法规则

#### 十进制浮点常量
- 指数部分可选
- 如果省略指数，小数点必须存在
- 整数部分和小数部分至少有一个存在

```c
double a = 1.0;    // 完整形式
double b = 1.;     // 省略小数部分
double c = .1;     // 省略整数部分
double d = 1e3;    // 省略小数点（有指数时）
```

#### 十六进制浮点常量（C99 起）
- 以 `0x` 或 `0X` 开头
- **指数部分必须存在**，以避免 `f` 后缀被误认为十六进制数字
- 指数表示 2 的幂次

```c
double hex1 = 0x1.2p3;   // 十六进制 1.2 × 2³ = 9.0
double hex2 = 0x1.Fp10;  // 十六进制 1.F × 2¹⁰
```

## 4. 底层原理（Underlying Principles）

### 编译期转换

浮点常量在编译期转换为内部表示：
1. **词法分析**：识别浮点常量的语法结构
2. **语义分析**：确定类型和精度
3. **值转换**：将文本转换为二进制浮点表示
4. **舍入处理**：根据目标类型进行舍入

### 舍入规则

浮点常量的求值结果为：
- 最近的可表示值，或
- 大于或小于最近可表示值的相邻可表示值

舍入方向由实现定义（编译器的默认舍入模式）。

**重要特性：**
- 相同源码形式的浮点常量转换为相同的内部格式和值
- 不同源码形式（如 `1.23` 和 `1.230`）可能转换为不同的内部表示

### 十六进制浮点常量的精确性（C99 起）

当 `FLT_RADIX` 为 2 时，十六进制浮点常量的求值结果为：
- 浮点常量表示的**精确值**
- 正确舍入到目标类型

这避免了十进制浮点常量常见的转换误差：

```c
// 十进制：可能有小误差
double dec = 0.1;  // 0.1 无法精确表示为二进制浮点

// 十六进制：精确表示
double hex = 0x1.999999999999ap-4;  // 精确的 0.1
```

### 精度扩展

根据 `FLT_EVAL_METHOD` 的值，浮点常量可能以更高的精度和范围求值：

```c
#if FLT_EVAL_METHOD == 2
// 常量以 long double 精度求值
float f = 0.1f;  // 可能以 long double 精度存储
#endif
```

### 十进制浮点类型的量子指数（C23 起）

十进制浮点类型具有量子指数概念：
- 相同数值但不同量子指数的常量有不同的内部表示
- 例如：`1230.dd`、`1230.0dd`、`1.23e3dd` 数值相同但表示不同

量子指数由最后一位数字的位置决定。

## 5. 使用场景（Use Cases）

### 基本场景

#### 初始化浮点变量

```c
double pi = 3.14159265358979;
float radius = 5.0f;
long double precise = 1.0L;
```

#### 科学计算常量

```c
double avogadro = 6.022e23;      // 阿伏伽德罗常数
double planck = 6.626e-34;       // 普朗克常数
double golden_ratio = 1.618033988749;
```

#### 精确位模式表示

使用十六进制浮点常量精确控制位模式：

```c
double exact = 0x1.8p+0;  // 精确的 1.5
```

### 最佳实践

#### 1. 选择合适的类型后缀

```c
// 推荐：显式指定类型
float f = 1.0f;           // 单精度
double d = 1.0;           // 双精度（默认）
long double ld = 1.0L;    // 扩展精度

// 避免：隐式类型转换
float bad = 1.0;          // double 转 float，可能丢失精度
```

#### 2. 使用十六进制常量避免精度损失

```c
// 十进制常量可能有舍入误差
double val1 = 0.15625;

// 十六进制常量精确表示
double val2 = 0x1.4p-3;   // 精确等于 0.15625
```

#### 3. 使用科学计数法表示极大或极小值

```c
double tiny = 1e-300;     // 极小值
double huge = 1e300;      // 极大值
```

#### 4. 使用数字分隔符提高可读性（C23 起）

```c
double million = 1'000'000.0;
double billion = 1'000'000'000.0;
```

### 常见陷阱

#### 陷阱 1：十六进制浮点常量必须包含指数

```c
// 错误：缺少指数
// double bad = 0x1.5;    // 编译错误

// 正确：包含指数
double good = 0x1.5p0;   // 正确
```

#### 陷阱 2：小数点的 locale 依赖

```c
// 浮点常量始终使用点号作为小数点
// setlocale 不影响常量语法
double val = 3.14;       // 始终使用点号
```

#### 陷阱 3：不存在负的浮点常量

```c
// -1.2 不是浮点常量
// 它是单目负号运算符应用于浮点常量 1.2
double neg = -1.2;       // - 是运算符，1.2 是常量
```

#### 陷阱 4：溢出和下溢

```c
// 超出范围会导致溢出
double overflow = 1e400;   // 可能变为无穷大

// 过小会导致下溢
double underflow = 1e-400;  // 可能变为零
```

#### 陷阱 5：NaN 和无穷大不能直接表示

```c
// 错误：无法直接表示 NaN 和无穷大
// double nan = 0x1.FFFFFEp128f;  // 这会溢出为无穷大

// 正确：使用宏或函数
#include <math.h>
double inf_val = INFINITY;
double nan_val = NAN;
```

## 6. 代码示例（Examples）

### 示例 1：基本用法

```c
#include <stdio.h>

int main(void) {
    // 不同类型的浮点常量
    float f = 3.14f;              // 单精度
    double d = 3.14159265358979;  // 双精度
    long double ld = 3.14159265358979L;  // 扩展精度

    printf("float: %f\n", f);
    printf("double: %lf\n", d);
    printf("long double: %Lf\n", ld);

    return 0;
}
```

### 示例 2：科学计数法

```c
#include <stdio.h>

int main(void) {
    // 科学计数法表示
    double small = 1.23e-10;    // 1.23 × 10⁻¹⁰
    double large = 4.56e20;      // 4.56 × 10²⁰

    printf("small = %e\n", small);
    printf("large = %e\n", large);

    // 不同指数大小写
    double a = 1e5;    // 小写 e
    double b = 1E5;    // 大写 E

    printf("a = %f, b = %f\n", a, b);

    return 0;
}
```

### 示例 3：十六进制浮点常量

```c
#include <stdio.h>

int main(void) {
    // 十六进制浮点常量（C99 起）
    double hex1 = 0x1.2p3;     // 十六进制 1.2 × 2³ = 9.0
    double hex2 = 0x1p0;       // 十六进制 1 × 2⁰ = 1.0
    double hex3 = 0x1.8p1;     // 十六进制 1.8 × 2¹ = 3.0

    printf("hex1 = %f\n", hex1);   // 输出: 9.0
    printf("hex2 = %f\n", hex2);   // 输出: 1.0
    printf("hex3 = %f\n", hex3);   // 输出: 3.0

    // 指数可正可负
    double hex4 = 0x1p-1;      // 0.5
    double hex5 = 0x1p+3;      // 8.0

    printf("hex4 = %f\n", hex4);   // 输出: 0.5
    printf("hex5 = %f\n", hex5);   // 输出: 8.0

    return 0;
}
```

### 示例 4：各种可选部分组合

```c
#include <stdio.h>

int main(void) {
    // 十进制浮点常量的各种形式

    // 有小数点，无指数
    double a = 3.14;      // 完整形式
    double b = 3.;       // 省略小数部分
    double c = .14;      // 省略整数部分

    // 有指数，可省略小数点
    double d = 1e10;     // 无小数点
    double e = 1.e10;    // 有小数点
    double f = .1e5;     // 有小数点，无整数部分

    // 负指数
    double g = 1e-5;
    double h = 3.14e-10;

    printf("a=%f, b=%f, c=%f\n", a, b, c);
    printf("d=%f, e=%f, f=%f\n", d, e, f);
    printf("g=%f, h=%e\n", g, h);

    return 0;
}
```

### 示例 5：溢出和下溢

```c
#include <stdio.h>
#include <float.h>
#include <math.h>

int main(void) {
    printf("DBL_MAX = %g\n", DBL_MAX);
    printf("DBL_MIN = %g\n", DBL_MIN);

    // 溢出为无穷大
    double overflow = 2.0e+308;
    printf("overflow = %g\n", overflow);

    if (isinf(overflow)) {
        printf("值已溢出为无穷大\n");
    }

    // 下溢为零
    double underflow = 1.0e-324;
    printf("underflow = %g\n", underflow);

    // 负无穷大
    double neg_inf = -2.0e+308;
    printf("neg_inf = %g\n", neg_inf);

    // 负零
    double neg_zero = -1.0e-324;
    printf("neg_zero = %g\n", neg_zero);

    return 0;
}
```

**输出示例：**
```
DBL_MAX = 1.79769e+308
DBL_MIN = 2.22507e-308
overflow = inf
值已溢出为无穷大
underflow = 0
neg_inf = -inf
neg_zero = -0
```

### 示例 6：类型后缀和精度

```c
#include <stdio.h>

int main(void) {
    // 无后缀 - double
    double d = 3.14159265358979323846;

    // f/F 后缀 - float
    float f = 3.14159265358979323846f;

    // l/L 后缀 - long double
    long double ld = 3.14159265358979323846L;

    printf("double:       %.15f\n", d);
    printf("float:        %.15f\n", f);
    printf("long double:  %.15Lf\n", ld);

    // 后缀大小写不敏感
    double d1 = 1.5e10;
    double d2 = 1.5E10;
    double d3 = 1.5e+10;

    printf("d1=%g, d2=%g, d3=%g\n", d1, d2, d3);

    return 0;
}
```

### 示例 7：常见错误及修正

```c
#include <stdio.h>
#include <math.h>

int main(void) {
    // ===== 错误 1：十六进制常量缺少指数 =====
    // 错误：double bad = 0x1.5;  // 编译错误

    // 修正：必须包含指数
    double good = 0x1.5p0;
    printf("十六进制常量: %f\n", good);

    // ===== 错误 2：误以为存在负的浮点常量 =====
    // -1.2 不是浮点常量，而是运算符应用于常量
    double neg = -1.2;  // 正确，但 - 是运算符

    // ===== 错误 3：使用错误的小数点 =====
    // 在某些国家使用逗号作为小数点
    // double bad = 3,14;  // 错误：这不是浮点常量
    double correct = 3.14;  // 正确：始终使用点号

    // ===== 错误 4：假设所有值都可直接表示 =====
    // double bad_nan = 0x1.FFFFFEp128f;  // 溢出，不是 NaN

    // 正确：使用宏或函数获取特殊值
    double inf_val = INFINITY;
    double nan_val = NAN;

    printf("infinity: %f\n", inf_val);
    printf("nan: %f\n", nan_val);

    return 0;
}
```

### 示例 8：实际应用 - 物理常数

```c
#include <stdio.h>

int main(void) {
    // 物理常量（使用科学计数法）
    double c = 2.99792458e8;        // 光速 (m/s)
    double h = 6.62607015e-34;      // 普朗克常数 (J·s)
    double e = 1.602176634e-19;     // 元电荷 (C)
    double Na = 6.02214076e23;      // 阿伏伽德罗常数

    printf("光速 c = %.8e m/s\n", c);
    printf("普朗克常数 h = %.8e J·s\n", h);
    printf("元电荷 e = %.8e C\n", e);
    printf("阿伏伽德罗常数 Na = %.8e\n", Na);

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

| 特性 | 说明 |
|------|------|
| **基本形式** | 有效数字 + 指数（可选）+ 后缀（可选） |
| **默认类型** | 无后缀时为 `double` |
| **类型后缀** | `f`/`F` → `float`，`l`/`L` → `long double` |
| **十六进制形式** | C99 起，以 `0x`/`0X` 开头，指数必须存在 |
| **编译期求值** | 在编译时转换为内部表示 |
| **舍入规则** | 实现定义的默认舍入方向 |

### 十进制 vs 十六进制浮点常量

| 特性 | 十进制 | 十六进制（C99 起） |
|------|--------|-------------------|
| 前缀 | 无 | `0x` 或 `0X` |
| 指数分隔符 | `e` 或 `E` | `p` 或 `P` |
| 指数基数 | 10 | 2 |
| 指数可选 | 是（需小数点）| 否（必须存在）|
| 精度保证 | 可能有舍入误差 | 精确表示（FLT_RADIX=2 时）|

### C23 新特性

1. **十进制浮点类型后缀**：`df`/`dd`/`dl` 用于 `_Decimal32`/`_Decimal64`/`_Decimal128`
2. **数字分隔符**：可用单引号 `'` 分隔数字，提高可读性
3. **量子指数**：十进制浮点类型支持相同值的不同表示

### 学习建议

1. **理解类型系统**：掌握不同后缀对应的类型，避免隐式转换
2. **使用十六进制形式**：需要精确控制位模式时，优先使用十六进制浮点常量
3. **注意边界情况**：了解溢出、下溢、NaN 和无穷大的处理方式
4. **版本兼容性**：十六进制浮点常量需要 C99 或更高版本；十进制浮点类型需要 C23

### 相关内容

- 整数常量（Integer Constant）
- 字符常量（Character Constant）
- 字符串字面量（String Literal）
- 浮点类型（`float`、`double`、`long double`）
- 数值极限（`<float.h>`）

---

**参考标准：**
- ISO/IEC 9899:2024 (C23) - 6.4.4.2 Floating constants
- ISO/IEC 9899:2018 (C17) - 6.4.4.2 Floating constants
- ISO/IEC 9899:2011 (C11) - 6.4.4.2 Floating constants
- ISO/IEC 9899:1999 (C99) - 6.4.4.2 Floating constants
- ISO/IEC 9899:1990 (C89/C90) - 3.1.3.1 Floating constants