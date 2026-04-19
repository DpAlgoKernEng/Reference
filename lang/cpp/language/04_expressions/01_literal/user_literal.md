# 用户定义字面量 (User-defined Literals)

## 1. 概述 (Overview)

用户定义字面量（User-defined Literals）是 C++11 引入的一项特性，允许程序员为整数、浮点数、字符和字符串字面量定义自定义后缀，从而产生用户定义类型的对象。

### 概念定义

用户定义字面量通过定义用户定义后缀（ud-suffix），将内置类型的字面量转换为用户定义类型的对象。这种机制使得代码更加直观和类型安全。

### 主要用途

- **类型安全的单位表示**：如 `10_km`、`3.14_rad` 等带有单位的字面量
- **自定义类型的字面量构造**：直接通过字面量语法创建自定义类型对象
- **代码可读性提升**：使代码更加直观、自文档化
- **编译期计算**：配合 `constexpr` 实现编译期计算

### 技术定位

用户定义字面量属于 C++ 语言核心特性，位于表达式层级的扩展，是对字面量语法的增强。它通过字面量操作符（Literal Operator）实现类型转换，是现代 C++ 类型安全和表达力的重要组成部分。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++11 之前，C++ 只支持内置类型的字面量（如 `42`、`3.14`、`"hello"` 等）。对于用户定义类型，必须通过构造函数或工厂函数来创建对象，语法相对冗长。例如，要表示一个距离值，可能需要写成：

```cpp
Distance d(10, DistanceUnit::Kilometers);  // 传统方式
```

### 设计动机

用户定义字面量的设计动机包括：

1. **简化用户类型的对象创建**：使自定义类型能像内置类型一样通过字面量创建
2. **增强类型安全**：通过单位后缀区分不同量纲，避免单位混淆
3. **提升代码可读性**：使代码更加直观，接近自然语言表达
4. **支持编译期计算**：配合 `constexpr` 实现零开销抽象

### 版本变更历史

| 版本 | 特性 | 说明 |
|------|------|------|
| C++11 | 基本用户定义字面量 | 引入字面量操作符，支持整数、浮点数、字符、字符串字面量 |
| C++14 | 数字分隔符 | 允许在数字字面量中使用单引号作为分隔符，如 `1'000'000_km` |
| C++17 | 十六进制浮点字面量 | 支持以 `p`/`P` 结尾的十六进制浮点字面量 |
| C++20 | 字符串字面量操作符模板 | 支持类类型的非类型模板参数，允许字符串作为模板参数 |
| C++20 | `char8_t` 支持 | 增加对 `char8_t` 类型的字面量操作符支持 |

### 特性测试宏

```cpp
#if __cpp_user_defined_literals >= 200809L
    // 用户定义字面量可用
#endif
```

---

## 3. 语法与参数 (Syntax and Parameters)

### 用户定义字面量语法

用户定义字面量表达式的形式如下：

| 形式 | 类型 | 示例 |
|------|------|------|
| decimal-literal ud-suffix | 整数字面量 | `12_km` |
| octal-literal ud-suffix | 八进整数字面量 | `0755_mode` |
| hex-literal ud-suffix | 十六进整数字面量 | `0xFF_color` |
| binary-literal ud-suffix | 二进整数字面量 | `0b1010_flags` |
| fractional-constant exponent-part(opt) ud-suffix | 浮点字面量 | `0.5_Pa` |
| digit-sequence exponent-part ud-suffix | 浮点字面量 | `1e10_energy` |
| character-literal ud-suffix | 字符字面量 | `'c'_X` |
| string-literal ud-suffix | 字符串字面量 | `"hello"_s` 或 `u"text"_u` |

### 术语说明

| 术语 | 说明 |
|------|------|
| decimal-literal | 非零十进制数字后跟零个或多个十进制数字 |
| octal-literal | 数字 0 后跟零个或多个八进制数字 |
| hex-literal | `0x` 或 `0X` 后跟一个或多个十六进制数字 |
| binary-literal | `0b` 或 `0B` 后跟一个或多个二进制数字 |
| digit-sequence | 十进制数字序列 |
| fractional-constant | 数字序列加点（`123.`）或可选数字序列加点加数字序列（`1.0` 或 `.12`） |
| exponent-part | 字母 `e` 或 `E` 后跟可选符号和数字序列 |
| character-literal | 字符字面量 |
| string-literal | 字符串字面量，包括原始字符串字面量 |
| ud-suffix | 标识符，由字面量操作符或字面量操作符模板引入 |

### 字面量操作符声明语法

字面量操作符（Literal Operator）是用户定义字面量调用的函数，声明形式有两种：

```cpp
// 形式 1：已弃用的语法（C++11/C++14 风格）
operator "" identifier        // 如 operator "" _km（注意空格）

// 形式 2：推荐语法（C++11 起）
operator user-defined-string-literal  // 如 operator""_km（无空格）
```

**重要规则**：
- ud-suffix 必须以下划线 `_` 开头（非下划线开头的后缀保留给标准库使用）
- ud-suffix 不能包含双下划线 `__`（双下划线保留）

### 允许的参数列表

字面量操作符只能使用以下参数列表：

| 参数列表 | 用途 | 版本 |
|----------|------|------|
| `(const char*)` | 原始字面量操作符，整数/浮点后备 | C++11 |
| `(unsigned long long int)` | 整数字面量首选 | C++11 |
| `(long double)` | 浮点字面量首选 | C++11 |
| `(char)` | char 字符字面量 | C++11 |
| `(wchar_t)` | wchar_t 字符字面量 | C++11 |
| `(char8_t)` | char8_t 字符字面量 | C++20 |
| `(char16_t)` | char16_t 字符字面量 | C++11 |
| `(char32_t)` | char32_t 字符字面量 | C++11 |
| `(const char*, std::size_t)` | char 字符串字面量 | C++11 |
| `(const wchar_t*, std::size_t)` | wchar_t 字符串字面量 | C++11 |
| `(const char8_t*, std::size_t)` | char8_t 字符串字面量 | C++20 |
| `(const char16_t*, std::size_t)` | char16_t 字符串字面量 | C++11 |
| `(const char32_t*, std::size_t)` | char32_t 字符串字面量 | C++11 |

### 字面量操作符模板

字面量操作符可以是模板，有两种形式：

**数值字面量操作符模板（Numeric Literal Operator Template）**：
```cpp
template<char...>
double operator""_x();  // 接收字符序列作为模板参数
```

**字符串字面量操作符模板（String Literal Operator Template，C++20 起）**：
```cpp
struct A { constexpr A(const char*); };
template<A a>
A operator""_a();  // 接收字符串作为类类型非类型模板参数
```

### 限制条件

- 不允许默认参数
- 不允许 C 语言链接（C linkage）
- 必须在命名空间作用域声明（可以是友元函数、显式实例化或特化、using 声明）

---

## 4. 底层原理 (Underlying Principles)

### 编译器处理流程

当编译器遇到用户定义字面量时，执行以下步骤：

#### 1. 词法分析阶段

编译器首先进行词法分析，确定字面量的类型和后缀：

```cpp
auto x = 123_km;    // 识别为整数字面量 + 后缀 _km
auto y = 3.14_rad;  // 识别为浮点字面量 + 后缀 _rad
auto z = "hello"_s;  // 识别为字符串字面量 + 后缀 _s
```

#### 2. 名称查找

编译器对后缀 `X` 进行无限定名称查找，查找名为 `operator""X` 的函数：

```cpp
// 查找 operator""_km
auto x = 123_km;  // 查找 operator""_km
```

如果查找失败，程序非法。

#### 3. 重载解析

根据字面量类型，编译器选择合适的重载：

**整数字面量的解析顺序**：
1. 如果存在参数为 `unsigned long long` 的字面量操作符，调用 `operator""X(nULL)`
2. 否则，如果存在原始字面量操作符，调用 `operator""X("n")`
3. 否则，如果存在数值字面量操作符模板，调用 `operator""X<'1','2','3'>()`

**浮点字面量的解析顺序**：
1. 如果存在参数为 `long double` 的字面量操作符，调用 `operator""X(fL)`
2. 否则，如果存在原始字面量操作符，调用 `operator""X("f")`
3. 否则，如果存在数值字面量操作符模板，调用 `operator""X<'3','.','1','4'>()`

**字符串字面量的解析**：
1. C++20 起：如果存在字符串字面量操作符模板且字符串是合法的模板参数，调用 `operator""X<str>()`
2. 否则调用 `operator""X(str, len)`，其中 `len` 是字符串长度（不含终止空字符）

**字符字面量的解析**：
- 直接调用 `operator""X(ch)`，`ch` 是字符值

### 最大 munch 原则

词法分析遵循"最长匹配"原则（Maximal Munch），可能导致意外的解析：

```cpp
long double operator""_E(long double);
int operator""_p(unsigned long long);

auto x = 1.0_E+2.0;   // 错误：被解析为预处理标记 1.0_E+2.0
auto y = 1.0_E +2.0;  // 正确：空格分隔
auto q = (1.0_E)+2.0; // 正确：括号分隔

auto w = 1_p+2;       // 错误：被解析为预处理标记 1_p+2
auto u = 1_p +2;      // 正确：空格分隔
```

以 `p`、`P`（C++17 起）、`e`、`E` 结尾的用户定义整数字面量或浮点字面量，后跟 `+` 或 `-` 运算符时，必须用空格或括号分隔。

### 字符串拼接规则

翻译阶段 6 进行字符串字面量拼接时，用户定义字符串字面量也会被拼接：

```cpp
L"A" "B" "C"_x;   // 正确：等同于 L"ABC"_x
"P"_x "Q" "R"_y;  // 错误：两个不同的后缀 _x 和 _y
```

所有拼接的字面量只能有一个后缀。

### 编译期计算

字面量操作符可以声明为 `constexpr`，实现编译期计算：

```cpp
constexpr long double operator""_deg_to_rad(long double deg) {
    return deg * 3.14159265358979323846L / 180.0L;
}

constexpr auto rad = 90.0_deg_to_rad;  // 编译期计算
```

---

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 物理单位与类型安全

用户定义字面量最适合表示带有单位的物理量：

```cpp
// 定义距离单位
constexpr long double operator""_km(long double x) { return x * 1000; }
constexpr long double operator""_m(long double x)  { return x; }
constexpr long double operator""_cm(long double x) { return x / 100; }

auto distance = 5.0_km + 300.0_m;  // 清晰的单位表示
```

#### 2. 自定义类型的字面量构造

为自定义类型提供简洁的构造语法：

```cpp
struct BigDecimal {
    std::string value;
    BigDecimal(const char* str, size_t len) : value(str, len) {}
};

BigDecimal operator""_bd(const char* str, size_t len) {
    return BigDecimal(str, len);
}

auto pi = "3.141592653589793"_bd;
```

#### 3. 编译期字符串处理

利用字符串字面量操作符模板实现编译期字符串操作：

```cpp
// C++20 字符串字面量操作符模板
template<std::size_t N>
struct DoubleString {
    char p[N + N - 1]{};
    constexpr DoubleString(char const(&pp)[N]) {
        std::ranges::copy(pp, p);
        std::ranges::copy(pp, p + N - 1);
    }
};

template<DoubleString A>
constexpr auto operator""_x2() {
    return A.p;
}

constexpr auto doubled = "abc"_x2;  // 编译期得到 "abcabc"
```

#### 4. 日志与调试

用于输出或记录信息：

```cpp
void operator""_print(const char* str) {
    std::cout << "[LOG] " << str << '\n';
}

"Debug message"_print;  // 输出: [LOG] Debug message
```

### 最佳实践

#### 1. 后缀命名规范

```cpp
// 推荐：使用下划线开头，小写字母，有意义的后缀
constexpr auto operator""_km(long double);   // 公里
constexpr auto operator""_ms(unsigned long long); // 毫秒

// 避免：使用保留标识符
// float operator""_Reserved(const char*);    // 错误：_后跟大写字母是保留的
// float operator""__test(const char*);       // 错误：包含双下划线是保留的
```

#### 2. 使用 constexpr 实现编译期计算

```cpp
constexpr long double operator""_deg_to_rad(long double deg) {
    return deg * std::numbers::pi_v<long double> / 180;
}

constexpr auto right_angle = 90.0_deg_to_rad;  // 编译期计算
```

#### 3. 配合命名空间组织

```cpp
namespace units {
    namespace distance {
        constexpr long double operator""_km(long double x) { return x * 1000; }
        constexpr long double operator""_m(long double x) { return x; }
    }
    namespace time {
        constexpr auto operator""_h(unsigned long long x) { return std::chrono::hours(x); }
        constexpr auto operator""_min(unsigned long long x) { return std::chrono::minutes(x); }
    }
}

using namespace units::distance;
auto d = 5.0_km;  // 使用 distance 命名空间中的字面量
```

### 常见陷阱

#### 1. 最大 munch 导致的解析错误

```cpp
// 错误示例
long double operator""_E(long double);
auto x = 1.0_E+2.0;   // 错误：被解析为预处理标记 1.0_E+2.0

// 正确做法
auto x = 1.0_E +2.0;  // 添加空格
auto x = (1.0_E)+2.0; // 使用括号
```

#### 2. 成员访问符的点号冲突

```cpp
#include <chrono>
using namespace std::literals;

// 错误示例
auto a = 4s.count();   // 错误：4s.count 被解析为单个预处理标记

// 正确做法
auto b = 4s .count();  // 添加空格
auto c = (4s).count(); // 使用括号
```

#### 3. 格式宏与用户定义字面量的冲突

```cpp
// C++11 前：这是合法的
std::printf("%"PRId64"\n", INT64_MIN);

// C++11 起：需要添加空格
std::printf("%" PRId64"\n", INT64_MIN);
```

#### 4. 后缀命名冲突

```cpp
// 错误：不以 _ 开头的后缀保留给标准库
float operator""Z(const char*);  // 错误

// 正确：以 _ 开头
float operator""_Z(const char*);  // 正确
```

---

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：角度转换

```cpp
#include <iostream>
#include <numbers>

// 将角度转换为弧度
constexpr long double operator""_deg_to_rad(long double deg) {
    return deg * std::numbers::pi_v<long double> / 180.0L;
}

int main() {
    constexpr auto radians = 90.0_deg_to_rad;
    std::cout << "90 degrees = " << radians << " radians\n";
    // 输出: 90 degrees = 1.570796 radians
    return 0;
}
```

#### 示例 2：自定义类型

```cpp
#include <iostream>

struct Distance {
    unsigned long long meters;
};

constexpr Distance operator""_m(unsigned long long n) {
    return Distance{n};
}

constexpr Distance operator""_km(unsigned long long n) {
    return Distance{n * 1000};
}

int main() {
    auto d1 = 500_m;
    auto d2 = 2_km;

    std::cout << "d1 = " << d1.meters << " meters\n";
    std::cout << "d2 = " << d2.meters << " meters\n";
    return 0;
}
```

#### 示例 3：字符串处理

```cpp
#include <string>
#include <iostream>

std::string operator""_upper(const char* str, std::size_t len) {
    std::string result(str, len);
    for (char& c : result) {
        c = std::toupper(c);
    }
    return result;
}

int main() {
    auto text = "hello"_upper;
    std::cout << text << '\n';  // 输出: HELLO
    return 0;
}
```

### 高级用法

#### 示例 4：数值字面量操作符模板

```cpp
#include <iostream>

// 将数字字符序列转换为值
template<char... Digits>
constexpr unsigned long long operator""_binary() {
    unsigned long long result = 0;
    ((result = result * 2 + (Digits - '0')), ...);  // C++17 折叠表达式
    return result;
}

int main() {
    constexpr auto value = 1010_binary;  // 二进制 1010 = 十进制 10
    std::cout << "1010_binary = " << value << '\n';
    return 0;
}
```

#### 示例 5：C++20 字符串字面量操作符模板

```cpp
#include <algorithm>
#include <cstddef>
#include <iostream>

template<std::size_t N>
struct DoubleString {
    char p[N + N - 1]{};

    constexpr DoubleString(char const(&pp)[N]) {
        std::ranges::copy(pp, p);
        std::ranges::copy(pp, p + N - 1);
    }
};

template<DoubleString A>
constexpr auto operator""_x2() {
    return A.p;
}

int main() {
    constexpr auto doubled = "abc"_x2;
    std::cout << doubled << '\n';  // 输出: abcabc
    return 0;
}
```

#### 示例 6：标准库字面量使用

```cpp
#include <iostream>
#include <chrono>
#include <string>
#include <string_view>
#include <complex>

int main() {
    using namespace std::literals;

    // 时间字面量
    auto duration = 1h + 30min + 45s;
    std::cout << "Total seconds: "
              << std::chrono::duration_cast<std::chrono::seconds>(duration).count()
              << '\n';

    // 字符串字面量
    std::string s = "hello"s;           // std::string
    std::string_view sv = "world"sv;     // std::string_view

    // 复数字面量
    std::complex<double> c = 1.0 + 2.0i; // 复数

    return 0;
}
```

### 常见错误及修正

#### 错误 1：后缀不以 _ 开头

```cpp
// 错误：后缀必须以下划线开头
float operator""km(unsigned long long);  // 错误

// 修正
float operator""_km(unsigned long long);  // 正确
```

#### 错误 2：使用保留标识符

```cpp
// 错误：_后跟大写字母是保留的（注意空格）
double operator"" _Z(long double);  // 错误（弃用）

// 正确：无空格，但避免使用 _X 形式（虽然语法正确）
double operator""_Z(long double);   // 语法正确，但 _Z 仍可能有冲突风险

// 最佳实践：使用 _小写 形式
double operator""_z(long double);   // 推荐
```

#### 错误 3：参数列表不正确

```cpp
// 错误：不允许默认参数
void operator""_test(const char* str, size_t len = 0);  // 错误

// 正确：无默认参数
void operator""_test(const char* str, size_t len);  // 正确
```

#### 错误 4：最大 munch 问题

```cpp
long double operator""_E(long double);

// 错误：1.0_E+2.0 被解析为预处理标记
auto x = 1.0_E+2.0;   // 错误

// 修正方案
auto x = 1.0_E +2.0;  // 添加空格
auto x = (1.0_E)+2.0; // 使用括号
```

#### 错误 5：字符串拼接后缀不一致

```cpp
// 错误：拼接的字面量有不同后缀
auto s = "hello"_x "world"_y;  // 错误

// 正确：只有一个后缀
auto s = "hello" "world"_x;    // 正确：等同于 "helloworld"_x
```

---

## 7. 总结 (Summary)

### 核心要点

1. **定义机制**：用户定义字面量通过字面量操作符（`operator""`）定义，后缀必须以下划线 `_` 开头且不能包含双下划线

2. **支持类型**：支持整数、浮点数、字符、字符串四种字面量类型，每种类型有对应的参数列表

3. **重载优先级**：编译器按照特定优先级选择重载（首选数值类型参数，其次是原始字符串参数，最后是模板）

4. **编译期能力**：配合 `constexpr` 可实现编译期计算，C++20 的字符串字面量操作符模板进一步增强了编译期处理能力

5. **最大 munch**：需注意词法分析的"最长匹配"原则，可能需要空格或括号分隔运算符

### 技术对比

| 特性 | 用户定义字面量 | 构造函数 | 工厂函数 |
|------|----------------|----------|----------|
| 语法简洁性 | 高 | 中 | 低 |
| 类型安全 | 高 | 高 | 中 |
| 编译期计算 | 支持 | 支持 | 有限支持 |
| 单位表达 | 直观 | 冗长 | 中等 |
| 可读性 | 高 | 中 | 低 |

### 学习建议

1. **从简单单位开始**：先实现简单的单位转换字面量（如 `_km`、`_m`）来理解基本概念

2. **理解重载解析**：掌握不同参数列表的优先级顺序，避免意外的函数调用

3. **注意命名规范**：严格遵循后缀命名规则，避免与保留标识符冲突

4. **善用 constexpr**：将字面量操作符声明为 `constexpr` 以获得编译期计算优势

5. **研究标准库实现**：查看 `<chrono>`、`<string>` 等标准库头文件中的字面量操作符实现

6. **关注 C++20 新特性**：字符串字面量操作符模板开启了新的元编程可能性

### 相关主题

- `constexpr` 和编译期计算
- 类型安全和单位制
- 运算符重载
- 模板元编程
- C++20 非类型模板参数的扩展

---

## 参考资料

- C++ 标准文档 [over.literal]
- cppreference: User-defined literals
- C++20 标准：字符串字面量操作符模板