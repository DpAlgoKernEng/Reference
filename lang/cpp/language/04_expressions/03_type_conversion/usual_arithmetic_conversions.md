# 常规算术转换 (Usual Arithmetic Conversions)

## 1. 概述 (Overview)

**常规算术转换 (Usual Arithmetic Conversions)** 是 C++ 中许多二元运算符处理算术或枚举类型操作数时遵循的一套类型转换规则。这些转换的目的是产生一个**公共类型 (Common Type)**，该类型同时也是运算结果的类型。

### 核心概念

- **适用运算符**：期望算术或枚举类型操作数的二元运算符
- **转换目标**：产生公共类型作为结果类型
- **处理流程**：分阶段执行类型提升和转换

### 技术定位

常规算术转换是 C++ 类型系统的核心机制之一，确保二元运算在类型不一致时能够：
1. 避免精度损失
2. 保证运算安全
3. 提供一致的行为

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

常规算术转换源于 C 语言，在 C++ 中得到继承和发展。其设计动机是解决混合类型运算时的类型兼容性问题。

### 版本演进

| 版本 | 变更内容 |
|------|---------|
| **C++98** | 初始定义，但可能涉及左值，导致行为不一致 |
| **C++11** | 增加作用域枚举类型处理规则（Stage 2）；调整整数转换等级规则 |
| **C++20** | 枚举类型与浮点类型混合运算标记为弃用 |
| **C++23** | 引入浮点转换子等级 (Floating-point Conversion Subrank)；完善浮点转换规则 |
| **C++26** | 禁止不同枚举类型或枚举与浮点类型的混合运算 |

### 缺陷报告修复

| 缺陷编号 | 问题 | 修正 |
|---------|------|------|
| **CWG 1642** | 常规算术转换可能涉及左值 | 首先应用左值到右值转换 |
| **CWG 2528** | unsigned char 与 unsigned int 的三路比较因中间整数提升而不合法 | 基于提升后的类型确定公共类型，无需实际提升操作数 |
| **CWG 2892** | 两个操作数类型相同时，"无需进一步转换"含义不明确 | 改为"将不执行进一步转换" |

---

## 3. 语法与参数 (Syntax and Parameters)

### 转换阶段定义

常规算术转换按以下 5 个阶段执行：

#### 阶段 1：左值到右值转换

对两个操作数应用**左值到右值转换 (Lvalue-to-rvalue Conversion)**，生成的纯右值 (prvalue) 将用于后续处理。

#### 阶段 2：作用域枚举检查 (C++11 起)

```
如果任一操作数是作用域枚举类型：
    - 不执行任何转换
    - 如果另一个操作数类型不同，表达式非良构
否则：
    - 进入下一阶段
```

#### 阶段 3：枚举类型混合检查 (C++26 起)

```
如果任一操作数是枚举类型，且另一操作数是：
    - 不同的枚举类型，或
    - 浮点类型
    则表达式非良构
否则：
    进入下一阶段
```

#### 阶段 4：浮点类型处理

如果任一操作数是浮点类型，应用以下规则：

| 条件 | 转换规则 |
|------|---------|
| 两个操作数类型相同 | 不执行进一步转换 |
| 一个操作数是非浮点类型 | 将该操作数转换为另一操作数的类型 |
| 浮点转换等级有序但不相等 (C++23) | 将较低等级的操作数转换为较高等级类型 |
| 浮点转换等级相等但子等级不同 (C++23) | 将较低子等级的操作数转换为较高子等级类型 |
| 其他情况 | 表达式非良构 |

如果两个操作数都是整数类型，进入阶段 5。

#### 阶段 5：整数类型公共类型推导

设 `T1` 和 `T2` 为操作数经过**整数提升 (Integral Promotion)** 后的类型，公共类型 `C` 的确定规则：

```
规则 1: 如果 T1 和 T2 是相同类型，则 C 为该类型

规则 2: 如果 T1 和 T2 都是有符号整数类型或都是无符号整数类型，
        则 C 为整数转换等级较大的类型

规则 3: 如果一个是有符号类型 S，另一个是无符号类型 U：
        - 如果 U 的等级 >= S 的等级，则 C 为 U
        - 否则如果 S 能表示 U 的所有值，则 C 为 S
        - 否则 C 为 S 对应的无符号类型
```

### 整数转换等级 (Integer Conversion Rank)

每个整数类型都有一个整数转换等级，定义如下：

#### 等级规则

1. **唯一性**：除 `char` 和 `signed char`（当 char 为有符号时）外，没有两个有符号整数类型具有相同等级
2. **宽度排序**：有符号整数类型的等级大于任何宽度较小的有符号整数类型
3. **类型等级降序**：
   - `long long` (C++11 起)
   - `long`
   - `int`
   - `short`
   - `signed char`
4. **无符号类型**：无符号整数类型的等级等于对应有符号整数类型的等级
5. **标准与扩展**：标准整数类型的等级大于相同宽度的扩展整数类型 (C++11 起)
6. **bool 类型**：`bool` 的等级小于所有标准整数类型
7. **字符类型**：编码字符类型的等级等于其底层类型的等级
   - `char` = `signed char` = `unsigned char`
   - `char8_t` = `unsigned char` (C++20 起)
   - `char16_t` = `std::uint_least16_t` (C++11 起)
   - `char32_t` = `std::uint_least32_t` (C++11 起)
   - `wchar_t` = 其实现定义的底层类型
8. **扩展类型**：相同宽度的扩展有符号整数类型之间的相对等级由实现定义 (C++11 起)
9. **传递性**：如果 `T1` > `T2` > `T3`，则 `T1` > `T3`

### 浮点转换等级和子等级 (C++23 起)

#### 浮点转换等级 (Floating-point Conversion Rank)

标准浮点类型的等级降序排列：
- `long double`
- `double`
- `float`

扩展浮点类型的等级规则：
- 类型 `T` 的等级大于其值集是 `T` 值集真子集的任何浮点类型
- 具有相同值集的两个扩展浮点类型等级相等
- 与恰好一个标准浮点类型具有相同值集的扩展浮点类型，等级等于该标准类型
- 与多个标准浮点类型具有相同值集的扩展浮点类型，等级等于 `double`

#### 浮点转换子等级 (Floating-point Conversion Subrank)

具有相同浮点转换等级的类型通过子等级排序：
- `std::float16_t`、`std::float32_t`、`std::float64_t`、`std::float128_t` 的子等级高于相同等级的标准浮点类型
- 其他子等级顺序由实现定义

---

## 4. 底层原理 (Underlying Principles)

### 类型转换的核心机制

#### 整数提升 (Integral Promotion)

在阶段 5 之前，操作数首先经过整数提升：
- 小整数类型（如 `char`、`short`）提升为 `int` 或 `unsigned int`
- 提升规则基于类型的整数转换等级

#### 公共类型计算算法

```
输入：类型 T1, T2（经过整数提升后）

算法：
1. 如果 T1 == T2：
   返回 T1

2. 如果 T1 和 T2 同为有符号或同为无符号：
   返回等级较高的类型

3. 否则（一个有符号 S，一个无符号 U）：
   a. 如果 rank(U) >= rank(S)：
      返回 U
   b. 否则如果 S 能表示 U 的所有值：
      返回 S
   c. 否则：
      返回 S 对应的无符号类型
```

### 性能特征

| 操作 | 时间复杂度 | 说明 |
|------|-----------|------|
| 类型判断 | O(1) | 编译期完成 |
| 转换执行 | 依赖硬件 | 可能有隐式转换开销 |

### 实现机制

1. **编译期决策**：所有类型判断和转换决策在编译期完成
2. **隐式转换代码**：编译器生成必要的转换指令
3. **优化机会**：现代编译器可优化部分转换序列

---

## 5. 使用场景 (Use Cases)

### 适用场景

#### 场景 1：混合整数类型运算

```cpp
int a = 10;
long b = 20L;
auto result = a + b;  // int 提升为 long，结果类型为 long
```

#### 场景 2：整数与浮点混合运算

```cpp
int count = 5;
double total = 12.5;
auto average = total / count;  // int 转换为 double，结果类型为 double
```

#### 场景 3：有符号与无符号混合运算

```cpp
int signed_val = -10;
unsigned int unsigned_val = 20U;
auto result = signed_val + unsigned_val;  // 结果类型为 unsigned int
// 注意：可能导致意外结果！
```

### 最佳实践

#### 实践 1：避免有符号与无符号混合运算

```cpp
// 错误示例
for (int i = 0; i < container.size(); i++) {  // 警告：比较有符号与无符号
    // ...
}

// 正确示例
for (size_t i = 0; i < container.size(); i++) {  // 使用 size_t
    // ...
}

// 或使用 auto
for (auto i = 0u; i < container.size(); i++) {
    // ...
}
```

#### 实践 2：显式类型转换提高代码可读性

```cpp
int numerator = 10;
int denominator = 4;

// 错误示例：整数除法丢失精度
double result1 = numerator / denominator;  // 结果为 2.0

// 正确示例：显式转换
double result2 = static_cast<double>(numerator) / denominator;  // 结果为 2.5
```

#### 实践 3：使用 auto 避免类型推断错误

```cpp
// 明确期望的类型
double expected = 3.14;
int count = 10;

// 使用 auto 确保类型正确
auto result = expected * count;  // result 类型为 double
```

### 常见陷阱

#### 陷阱 1：无符号整数下溢

```cpp
unsigned int a = 5;
unsigned int b = 10;
auto diff = a - b;  // 结果是很大的无符号数，而非 -5

// 正确做法：使用有符号类型或显式检查
if (a >= b) {
    unsigned int diff = a - b;
} else {
    // 处理负数情况
}
```

#### 陷阱 2：枚举类型陷阱 (C++26 前)

```cpp
enum Color { Red, Green, Blue };
enum Size { Small, Medium, Large };

// C++20 弃用，C++26 错误
auto bad = Red + Small;  // 不同枚举类型混合运算
```

#### 陷阱 3：三路比较中的转换 (CWG 2528)

```cpp
unsigned char uc = 10;
unsigned int ui = 10;

// C++20 前：可能因中间转换导致三路比较非良构
auto cmp = uc <=> ui;  // C++20 后正确：直接转换，不经过中间提升
```

---

## 6. 代码示例 (Examples)

### 基础示例：整数类型提升

```cpp
#include <iostream>
#include <type_traits>

int main() {
    short s = 100;
    int i = 200;
    long l = 300L;

    // short + int -> int
    auto result1 = s + i;
    std::cout << "short + int: " << typeid(result1).name() << std::endl;
    std::cout << "Is int: " << std::is_same_v<decltype(result1), int> << std::endl;

    // int + long -> long
    auto result2 = i + l;
    std::cout << "int + long: " << typeid(result2).name() << std::endl;
    std::cout << "Is long: " << std::is_same_v<decltype(result2), long> << std::endl;

    return 0;
}
```

**输出**：
```
short + int: i
Is int: 1
int + long: l
Is long: 1
```

### 进阶示例：有符号与无符号转换

```cpp
#include <iostream>
#include <type_traits>

int main() {
    // 示例 1：int 与 unsigned int
    int i = -10;
    unsigned int ui = 20U;

    auto result1 = i + ui;
    std::cout << "Type is unsigned int: "
              << std::is_same_v<decltype(result1), unsigned int> << std::endl;
    std::cout << "Result: " << result1 << std::endl;  // 注意：可能产生意外值

    // 示例 2：long 与 unsigned int
    long l = -10L;
    unsigned int ui2 = 20U;

    // long 的等级 >= unsigned int 的等级，且能表示所有 unsigned int 值
    // 结果类型可能是 long（取决于平台）
    auto result2 = l + ui2;
    std::cout << "Result type depends on platform" << std::endl;

    return 0;
}
```

### 高级示例：浮点类型转换 (C++23)

```cpp
#include <iostream>
#include <type_traits>
#include <cmath>

int main() {
    float f = 3.14f;
    double d = 2.71828;
    long double ld = 1.41421356L;

    // float + double -> double
    auto result1 = f + d;
    std::cout << "float + double -> double: "
              << std::is_same_v<decltype(result1), double> << std::endl;

    // double + long double -> long double
    auto result2 = d + ld;
    std::cout << "double + long double -> long double: "
              << std::is_same_v<decltype(result2), long double> << std::endl;

    // int + double -> double
    int i = 10;
    auto result3 = i + d;
    std::cout << "int + double -> double: "
              << std::is_same_v<decltype(result3), double> << std::endl;

    return 0;
}
```

### 错误示例及修正

#### 错误 1：无符号整数负值陷阱

```cpp
#include <iostream>

int main() {
    unsigned int a = 5;
    unsigned int b = 10;

    // 错误：期望得到负数，实际得到很大的无符号数
    std::cout << "a - b = " << a - b << std::endl;  // 输出：4294967291

    return 0;
}
```

**修正**：

```cpp
#include <iostream>
#include <optional>

std::optional<int> safe_subtract(unsigned int a, unsigned int b) {
    if (a >= b) {
        return static_cast<int>(a - b);
    }
    return std::nullopt;  // 或抛出异常
}

int main() {
    unsigned int a = 5;
    unsigned int b = 10;

    // 正确：显式处理无符号减法
    auto result = safe_subtract(a, b);
    if (result) {
        std::cout << "a - b = " << *result << std::endl;
    } else {
        std::cout << "Cannot subtract: a < b" << std::endl;
    }

    return 0;
}
```

#### 错误 2：整数除法精度丢失

```cpp
#include <iostream>

int main() {
    int a = 5;
    int b = 2;

    // 错误：整数除法丢失小数部分
    double result = a / b;  // result = 2.0，而非 2.5
    std::cout << "a / b = " << result << std::endl;

    return 0;
}
```

**修正**：

```cpp
#include <iostream>

int main() {
    int a = 5;
    int b = 2;

    // 正确：至少一个操作数转换为浮点类型
    double result = static_cast<double>(a) / b;  // result = 2.5
    std::cout << "a / b = " << result << std::endl;

    return 0;
}
```

#### 错误 3：作用域枚举类型混合 (C++11)

```cpp
#include <iostream>

enum class Color { Red, Green, Blue };
enum class Size { Small, Medium, Large };

int main() {
    Color c = Color::Red;
    Size s = Size::Small;

    // 错误：作用域枚举不能混合运算
    // auto result = c + s;  // 编译错误

    // 如果需要数值运算，显式转换为整数
    int value = static_cast<int>(c) + static_cast<int>(s);
    std::cout << "Combined value: " << value << std::endl;

    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 要点 | 说明 |
|------|------|
| **目的** | 为二元运算产生公共类型 |
| **阶段** | 5 个阶段，按顺序执行 |
| **整数提升** | 小整数类型首先提升 |
| **等级系统** | 整数和浮点各有转换等级体系 |
| **版本演进** | C++11、C++20、C++23、C++26 持续完善 |

### 关键规则速查

1. **相同类型**：无需转换
2. **浮点优先**：整数转换为浮点
3. **有符号与无符号**：
   - 无符号等级 >= 有符号等级 → 结果为无符号
   - 有符号能表示无符号所有值 → 结果为有符号
   - 否则 → 结果为有符号对应的无符号类型

### 技术对比

| 特性 | C++98 | C++11 | C++20 | C++23 | C++26 |
|------|-------|-------|-------|-------|-------|
| 作用域枚举处理 | - | 支持 | - | - | - |
| 枚举混合弃用 | - | - | 弃用 | - | 错误 |
| 浮点子等级 | - | - | - | 支持 | - |
| 三路比较修复 | - | - | CWG 2528 | - | - |

### 学习建议

1. **理解等级系统**：掌握整数和浮点转换等级是理解常规算术转换的关键
2. **避免混合类型**：显式类型转换比隐式转换更安全、更清晰
3. **警惕无符号**：有符号与无符号混合运算可能导致意外结果
4. **使用现代工具**：利用编译器警告（如 `-Wsign-conversion`）发现潜在问题
5. **实践验证**：使用 `std::is_same_v` 和 `typeid` 验证类型推断

### 相关概念

- **整数提升 (Integral Promotion)**：小整数类型的自动提升
- **左值到右值转换 (Lvalue-to-rvalue Conversion)**：提取存储的值
- **窄化转换 (Narrowing Conversion)**：可能丢失信息的转换
- **公共类型 (Common Type)**：`std::common_type` 类型特征

---

## 参考资料

- [C++ Reference: Usual Arithmetic Conversions](https://en.cppreference.com/w/cpp/language/usual_arithmetic_conversions)
- [CWG Issue 1642: Lvalues in usual arithmetic conversions](https://cplusplus.github.io/CWG/issues/1642.html)
- [CWG Issue 2528: Three-way comparison and integral promotion](https://cplusplus.github.io/CWG/issues/2528.html)
- [CWG Issue 2892: Clarification of floating-point conversion wording](https://cplusplus.github.io/CWG/issues/2892.html)