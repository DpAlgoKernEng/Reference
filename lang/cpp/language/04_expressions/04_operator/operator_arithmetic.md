# 算术运算符 (Arithmetic Operators)

## 1. 概述 (Overview)

算术运算符是 C++ 中最基本的运算符类别，用于执行各种算术运算并返回运算结果。这些运算符涵盖了从基础的四则运算到位运算的完整计算能力。

### 概念定义

算术运算符包括以下几类：

| 运算符名称 | 语法 | 类内定义原型 | 类外定义原型 |
|-----------|------|-------------|-------------|
| 一元正号 | `+a` | `T T::operator+() const;` | `T operator+(const T& a);` |
| 一元负号 | `-a` | `T T::operator-() const;` | `T operator-(const T& a);` |
| 加法 | `a + b` | `T T::operator+(const T2& b) const;` | `T operator+(const T& a, const T2& b);` |
| 减法 | `a - b` | `T T::operator-(const T2& b) const;` | `T operator-(const T& a, const T2& b);` |
| 乘法 | `a * b` | `T T::operator*(const T2& b) const;` | `T operator*(const T& a, const T2& b);` |
| 除法 | `a / b` | `T T::operator/(const T2& b) const;` | `T operator/(const T& a, const T2& b);` |
| 取余 | `a % b` | `T T::operator%(const T2& b) const;` | `T operator%(const T& a, const T2& b);` |
| 位反 | `~a` | `T T::operator~() const;` | `T operator~(const T& a);` |
| 位与 | `a & b` | `T T::operator&(const T2& b) const;` | `T operator&(const T& a, const T2& b);` |
| 位或 | `a | b` | `T T::operator|(const T2& b) const;` | `T operator|(const T& a, const T2& b);` |
| 位异或 | `a ^ b` | `T T::operator^(const T2& b) const;` | `T operator^(const T& a, const T2& b);` |
| 左移 | `a << b` | `T T::operator<<(const T2& b) const;` | `T operator<<(const T& a, const T2& b);` |
| 右移 | `a >> b` | `T T::operator>>(const T2& b) const;` | `T operator>>(const T& a, const T2& b);` |

### 主要用途

- 执行数值计算（加、减、乘、除、取余）
- 执行位级操作（位与、位或、位异或、位反、移位）
- 支持指针算术运算
- 支持用户自定义类型的运算符重载

### 技术定位

算术运算符是 C++ 表达式系统的基础组成部分，属于内置运算符的核心类别。所有内置算术运算符都返回值（而非引用），这使得它们可以链式调用。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

算术运算符继承自 C 语言，在 C++ 中得到了扩展：

1. **C 语言继承**：基础算术运算符（+、-、*、/、%）和位运算符都来自 C 语言
2. **运算符重载**：C++ 增加了运算符重载机制，允许用户为自定义类型定义运算符行为
3. **类型安全增强**：C++ 加强了类型检查，引入了更严格的类型转换规则

### 版本变更

| 版本 | 变更内容 |
|------|---------|
| C++98 | 基础算术运算符定义，整数除法向零取整的规则在 CWG 614 后明确 |
| C++11 | 一元正号可用于非泛型无捕获 lambda 表达式到函数指针的转换 |
| C++20 | 有符号整数保证使用二进制补码表示；右移运算符对于负数明确定义为算术右移 |

### 重要缺陷报告

- **CWG 614**：明确了整数除法的代数商向零取整（截断小数部分）
- **CWG 1450**：如果 `a / b` 的结果无法在结果类型中表示，则 `a / b` 和 `a % b` 的行为都是未定义的
- **CWG 1457**：将正数的最高位 `1` 移入符号位的行为从未定义改为明确定义
- **CWG 1515**：所有无符号整数都遵循模 2^n 运算规则

## 3. 语法与参数 (Syntax and Parameters)

### 一元算术运算符

```
+ expression    // 一元正号（提升）
- expression    // 一元负号（取负）
```

**优先级**：一元 `+` 和 `-` 的优先级高于所有二元算术运算符，从右向左结合。

### 加法运算符

```
lhs + rhs    // 二元加法
lhs - rhs    // 二元减法
```

**优先级**：二元 `+` 和 `-` 的优先级低于 `*`、`/`、`%`，高于移位和位逻辑运算符，从左向右结合。

### 乘法运算符

```
lhs * rhs    // 乘法
lhs / rhs    // 除法
lhs % rhs    // 取余
```

**优先级**：乘法运算符优先级最高（在算术运算符中），从左向右结合。

### 位逻辑运算符

```
~ rhs           // 位反（一元）
lhs & rhs       // 位与
lhs ^ rhs       // 位异或
lhs | rhs       // 位或
```

**优先级**：
- 位反 `~` 优先级高于所有二元算术运算符，从右向左结合
- 位与 `&` > 位异或 `^` > 位或 `|`（优先级递减），均从左向右结合

### 位移运算符

```
lhs << rhs    // 左移
lhs >> rhs    // 右移
```

**优先级**：位移运算符优先级高于位逻辑运算符，低于加法和乘法运算符，从左向右结合。

### 参数说明

| 参数 | 说明 |
|-----|------|
| `lhs` | 左操作数，对于二元运算符 |
| `rhs` | 右操作数 |
| `expression` | 表达式，对于一元运算符 |

**类型要求**：
- 一元运算符：操作数必须是算术类型、无作用域枚举类型或指针类型（仅限一元正号）
- 二元运算符：操作数必须满足常规算术转换的要求
- 位运算符：操作数必须是整数或无作用域枚举类型

## 4. 底层原理 (Underlying Principles)

### 类型转换机制

#### 整型提升 (Integral Promotion)

如果传递给内置算术运算符的操作数是整型或无作用域枚举类型，则在任何其他操作之前（但在左值到右值转换之后），操作数会进行整型提升。

#### 常规算术转换 (Usual Arithmetic Conversions)

对于二元运算符（移位除外），如果提升后的操作数具有不同类型，则应用常规算术转换。

### 溢出处理

#### 无符号整数

无符号整数算术运算总是执行模 2^n 运算，其中 n 是该整数的位数。例如，对于 `unsigned int`，对 `UINT_MAX` 加 1 得到 0，从 0 减 1 得到 `UINT_MAX`。

#### 有符号整数

当有符号整数算术运算溢出（结果不适合结果类型）时，行为是**未定义的**。可能的表现包括：

- 根据表示规则（通常是二进制补码）回绕
- 陷阱（trap）——在某些平台或由于编译器选项（如 GCC 和 Clang 的 `-ftrapv`）
- 饱和到最小或最大值（在许多 DSP 上）
- 被编译器完全优化掉

### 浮点环境

如果支持 `#pragma STDC FENV_ACCESS` 并设置为 `ON`，所有浮点算术运算符都遵守当前的浮点舍入方向，并按照 `math_errhandling` 的规定报告浮点算术错误。

### 浮点收缩

除非 `#pragma STDC FP_CONTRACT` 设置为 `OFF`，否则所有浮点算术运算都可以执行，就像中间结果具有无限范围和精度一样。这允许省略舍入误差和浮点异常的优化。

### 指针算术

当整型表达式 J 加到或从指针表达式 P 中减去时：

- 如果 P 指向数组对象的第 i 个元素，`P + J` 指向第 `i+j` 个元素
- 如果 P 指向完整对象、基类子对象或成员子对象 y，则可以加 0 或 1（指向 y 或 y 之后）
- 从指针减去指针的结果类型是 `std::ptrdiff_t`

### 移位运算行为

| 条件 | C++20 之前 | C++20 起 |
|-----|-----------|---------|
| 无符号 a 的左移 `a << b` | `a * 2^b` 模 2^N | 与 `a * 2^b` 模 2^N 同余的唯一值 |
| 非负有符号 a 的左移 | 如果 `a * 2^b` 可表示则合法 | 与 `a * 2^b` 模 2^N 同余的唯一值 |
| 负数 a 的左移 | **未定义行为** | 与 `a * 2^b` 模 2^N 同余的唯一值 |
| 无符号/非负 a 的右移 | `a / 2^b` 的整数部分 | `a / 2^b` 的整数部分 |
| 负数 a 的右移 | 实现定义 | 向负无穷舍入（算术右移） |

## 5. 使用场景 (Use Cases)

### 基础数值计算

算术运算符最常用于数值计算场景：

```cpp
int sum = a + b;          // 加法
int diff = a - b;         // 减法
int product = a * b;      // 乘法
int quotient = a / b;     // 除法（整数除法向零取整）
int remainder = a % b;    // 取余
```

### 指针运算

指针算术在数组和内存操作中广泛使用：

```cpp
int arr[10];
int* p = arr + 5;         // 指向第 5 个元素
ptrdiff_t diff = p - arr; // 结果为 5
```

### 位操作

位运算符用于底层系统编程、标志位操作、加密算法等：

```cpp
unsigned int flags = 0x0F;
flags |= 0x10;            // 设置位
flags &= ~0x08;           // 清除位
flags ^= 0xFF;            // 翻转位
bool isSet = flags & 0x02; // 测试位
```

### 最佳实践

#### 避免有符号整数溢出

```cpp
// 错误：可能导致未定义行为
int x = INT_MAX + 1;  // 未定义行为

// 正确：使用无符号类型或检查边界
unsigned int y = UINT_MAX + 1U;  // 定义良好的回绕
```

#### 注意整数除法

```cpp
// 整数除法向零取整
int a = -7 / 2;   // 结果为 -3（不是 -4）
int b = 7 / -2;   // 结果为 -3
int c = -7 / -2;  // 结果为 3
```

#### 移位操作注意边界

```cpp
// 错误：移位量超过类型宽度
int x = 1 << 32;  // 未定义行为（假设 int 为 32 位）

// 正确：检查移位量
if (shift >= 0 && shift < 32) {
    int result = 1 << shift;
}
```

### 常见陷阱

1. **无符号与有符号混用**

```cpp
unsigned int a = 1;
int b = -10;
auto result = a + b;  // b 被转换为无符号，结果为很大的正数
```

2. **除以零**

```cpp
int x = 10 / 0;   // 未定义行为
```

3. **指针运算越界**

```cpp
int arr[5];
int* p = arr + 10;  // 未定义行为（越界）
```

4. **位运算符优先级**

```cpp
// 错误：位运算符优先级低于比较运算符
if (flags & 0x01 == 0) { /* ... */ }  // 实际解析为 flags & (0x01 == 0)

// 正确：使用括号
if ((flags & 0x01) == 0) { /* ... */ }
```

## 6. 代码示例 (Examples)

### 基础用法：一元运算符

```cpp
#include <iostream>

int main() {
    char c = 0x6a;
    int n1 = 1;
    unsigned char n2 = 1;
    unsigned int n3 = 1;

    std::cout << "char: " << c << " int: " << +c << "\n"
                 "-1, where 1 is signed: " << -n1 << "\n"
                 "-1, where 1 is unsigned char: " << -n2 << "\n"
                 "-1, where 1 is unsigned int: " << -n3 << '\n';

    char a[3];
    std::cout << "size of array: " << sizeof a << "\n"
                 "size of pointer: " << sizeof +a << '\n';

    return 0;
}

// 输出：
// char: j int: 106
// -1, where 1 is signed: -1
// -1, where 1 is unsigned char: -1
// -1, where 1 is unsigned int: 4294967295
// size of array: 3
// size of pointer: 8
```

### 基础用法：加法与指针运算

```cpp
#include <iostream>

int main() {
    char c = 2;
    unsigned int un = 2;
    int n = -10;

    std::cout << " 2 + (-10), where 2 is a char    = " << c + n << "\n"
                 " 2 + (-10), where 2 is unsigned  = " << un + n << "\n"
                 " -10 - 2.12  = " << n - 2.12 << '\n';

    char a[4] = {'a', 'b', 'c', 'd'};
    char* p = &a[1];
    std::cout << "Pointer addition examples: " << *p << *(p + 2)
              << *(2 + p) << *(p - 1) << '\n';
    char* p2 = &a[4];
    std::cout << "Pointer difference: " << p2 - p << '\n';

    return 0;
}

// 输出：
//  2 + (-10), where 2 is a char    = -8
//  2 + (-10), where 2 is unsigned  = 4294967288
//  -10 - 2.12  = -12.12
// Pointer addition examples: bdda
// Pointer difference: 3
```

### 基础用法：乘法运算符

```cpp
#include <iostream>

int main() {
    char c = 2;
    unsigned int un = 2;
    int n = -10;

    std::cout << "2 * (-10), where 2 is a char    = " << c * n << "\n"
                 "2 * (-10), where 2 is unsigned  = " << un * n << "\n"
                 "-10 / 2.12  = " << n / 2.12 << "\n"
                 "-10 / 21  = " << n / 21 << "\n"
                 "-10 % 21  = " << n % 21 << '\n';

    return 0;
}

// 输出：
// 2 * (-10), where 2 is a char    = -20
// 2 * (-10), where 2 is unsigned  = 4294967276
// -10 / 2.12  = -4.71698
// -10 / 21  = 0
// -10 % 21  = -10
```

### 基础用法：位运算

```cpp
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <iostream>

int main() {
    std::uint16_t mask = 0x00f0;
    std::uint32_t x0 = 0x12345678;
    std::uint32_t x1 = x0 | mask;
    std::uint32_t x2 = x0 & ~mask;
    std::uint32_t x3 = x0 & mask;
    std::uint32_t x4 = x0 ^ mask;
    std::uint32_t x5 = ~x0;

    using bin16 = std::bitset<16>;
    using bin32 = std::bitset<32>;

    std::cout << std::hex << std::showbase
              << "Mask: " << mask << std::setw(49) << bin16(mask) << "\n"
                 "Value: " << x0 << std::setw(42) << bin32(x0) << "\n"
                 "Setting bits: " << x1 << std::setw(35) << bin32(x1) << "\n"
                 "Clearing bits: " << x2 << std::setw(34) << bin32(x2) << "\n"
                 "Selecting bits: " << x3 << std::setw(39) << bin32(x3) << "\n"
                 "XOR-ing bits: " << x4 << std::setw(35) << bin32(x4) << "\n"
                 "Inverting bits: " << x5 << std::setw(33) << bin32(x5) << '\n';

    return 0;
}

// 输出：
// Mask: 0xf0                                 0000000011110000
// Value: 0x12345678          00010010001101000101011001111000
// Setting bits: 0x123456f8   00010010001101000101011011111000
// Clearing bits: 0x12345608  00010010001101000101011000001000
// Selecting bits: 0x70       00000000000000000000000001110000
// XOR-ing bits: 0x12345688   00010010001101000101011010001000
// Inverting bits: 0xedcba987 11101101110010111010100110000111
```

### 基础用法：位移运算

```cpp
#include <iostream>

enum { ONE = 1, TWO = 2 };

int main() {
    std::cout << std::hex << std::showbase;

    char c = 0x10;
    unsigned long long ull = 0x123;

    std::cout << "0x123 << 1 = " << (ull << 1) << "\n"
                 "0x123 << 63 = " << (ull << 63) << "\n"   // 无符号溢出
                 "0x10 << 10 = " << (c << 10) << '\n';      // char 提升为 int

    long long ll = -1000;
    std::cout << std::dec << "-1000 >> 1 = " << (ll >> ONE) << '\n';

    return 0;
}

// 输出：
// 0x123 << 1 = 0x246
// 0x123 << 63 = 0x8000000000000000
// 0x10 << 10 = 0x4000
// -1000 >> 1 = -500
```

### 高级用法：运算符重载

```cpp
#include <iostream>

class Complex {
public:
    double real, imag;

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // 一元负号
    Complex operator-() const {
        return Complex(-real, -imag);
    }

    // 二元加法
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // 二元乘法
    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    void print() const {
        std::cout << real << " + " << imag << "i" << std::endl;
    }
};

int main() {
    Complex a(1, 2);  // 1 + 2i
    Complex b(3, 4);  // 3 + 4i

    Complex c = a + b;
    std::cout << "a + b = ";
    c.print();  // 4 + 6i

    Complex d = a * b;
    std::cout << "a * b = ";
    d.print();  // -5 + 10i

    Complex e = -a;
    std::cout << "-a = ";
    e.print();  // -1 + -2i

    return 0;
}
```

### 常见错误：有符号整数溢出

```cpp
#include <iostream>
#include <climits>

int main() {
    // 错误示例：有符号整数溢出
    int a = INT_MAX;
    int b = a + 1;  // 未定义行为！可能产生意想不到的结果

    std::cout << "INT_MAX = " << a << std::endl;
    std::cout << "INT_MAX + 1 = " << b << std::endl;  // 结果不可预测

    // 正确做法：使用无符号类型或检查边界
    unsigned int c = UINT_MAX;
    unsigned int d = c + 1U;  // 定义良好，结果为 0
    std::cout << "UINT_MAX + 1 = " << d << std::endl;

    // 或者检查边界
    int e = INT_MAX - 1;
    if (e <= INT_MAX - 1) {
        int f = e + 1;  // 安全
        std::cout << "Safe addition: " << f << std::endl;
    }

    return 0;
}
```

### 常见错误：移位量无效

```cpp
#include <iostream>
#include <climits>

int main() {
    int value = 1;

    // 错误示例：移位量超过类型宽度
    int shift = 32;  // 假设 int 为 32 位
    int result1 = value << shift;  // 未定义行为

    // 错误示例：负移位量
    int negShift = -1;
    int result2 = value << negShift;  // 未定义行为

    // 正确做法：检查移位量
    int safeShift = 5;
    if (safeShift >= 0 && safeShift < 32) {  // 假设 int 为 32 位
        int result3 = value << safeShift;  // 安全
        std::cout << "1 << 5 = " << result3 << std::endl;
    }

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 类别 | 运算符 | 关键要点 |
|-----|-------|---------|
| 一元运算符 | `+`, `-` | 一元正号执行整型提升；一元负号返回操作数的负值 |
| 算术运算符 | `+`, `-`, `*`, `/`, `%` | 整数除法向零取整；有符号溢出是 UB；无符号运算模 2^n |
| 位运算符 | `&`, `\|`, `^`, `~` | 操作数必须是整数类型；注意优先级低于比较运算符 |
| 移位运算符 | `<<`, `>>` | 移位量必须非负且小于类型宽度；C++20 起负数右移定义为算术右移 |

### 技术对比

| 特性 | C++20 之前 | C++20 起 |
|-----|-----------|---------|
| 有符号整数的负数左移 | 未定义行为 | 定义明确 |
| 有符号整数的负数右移 | 实现定义 | 算术右移（向负无穷舍入） |
| 整数表示 | 允许多种表示 | 强制二进制补码 |

### 学习建议

1. **优先级记忆**：记住位运算符优先级较低，经常需要括号
2. **无符号整数**：理解无符号整数的模运算特性，避免与有符号数混用
3. **溢出意识**：时刻警惕有符号整数溢出，使用静态分析工具检测
4. **指针算术**：理解指针运算的限制，避免越界访问
5. **运算符重载**：为自定义类型重载运算符时，保持与内置运算符一致的行为

### 标准库支持

算术运算符在标准库中被广泛重载：
- `std::chrono::duration` 和 `std::chrono::time_point` 的时间运算
- `std::complex` 的复数运算
- `std::valarray` 的元素级运算
- `std::bitset` 的位运算
- I/O 流的插入/提取运算符（`<<`, `>>`）

### 参见

- 运算符优先级 (Operator Precedence)
- 运算符重载 (Operator Overloading)
- 常规算术转换 (Usual Arithmetic Conversions)
- 整型提升 (Integral Promotion)