# 变量模板 (Variable Template)

## 1. 概述

**变量模板 (Variable Template)** 是 C++14 引入的一种模板特性，用于定义一组变量或静态数据成员的模板。通过变量模板，可以根据模板参数生成不同的变量实例，实现参数化的变量定义。

变量模板的核心价值在于：
- **简化常量定义**：避免为不同类型重复定义相同语义的常量
- **类型安全的参数化**：为不同类型提供类型精确的变量值
- **代码复用**：消除冗余的函数模板或类模板静态成员

变量模板定义在命名空间作用域时声明一个变量，在类作用域时声明一个静态数据成员模板。

## 2. 来源与演变

### 首次引入

变量模板在 **C++14** 标准中首次引入，是 C++ 模板体系的重要扩展之一。

### 历史背景

在变量模板出现之前，开发者通常采用以下两种方式实现参数化变量：

| 传统方法 | 示例 | 缺点 |
|---------|------|------|
| 类模板静态数据成员 | `struct Math { static constexpr T pi; };` | 需要类包装，语法冗余 |
| constexpr 函数模板 | `template<T> constexpr T pi() { return T(3.14...); }` | 调用时需要括号，语义不够直观 |

变量模板的引入解决了这些问题，提供了更直观、更简洁的语法：

```cpp
// C++14 之前：使用函数模板
template<typename T>
constexpr T pi() { return T(3.1415926535897932385L); }
// 使用：pi<double>()

// C++14：使用变量模板
template<typename T>
constexpr T pi = T(3.1415926535897932385L);
// 使用：pi<double>
```

### 版本演进

| 版本 | 变化 |
|------|------|
| C++14 | 首次引入变量模板 |
| C++20 | 支持 `requires` 约束子句，可对模板参数施加限制 |

### Feature-test 宏

| 宏 | 值 | 标准 | 特性 |
|----|-----|------|------|
| `__cpp_variable_templates` | `201304L` | C++14 | 变量模板 |

## 3. 语法与参数

### 基本语法

```cpp
// (1) 基本形式 (C++14 起)
template<parameter-list> variable-declaration

// (2) 带约束的形式 (C++20 起)
template<parameter-list> requires constraint variable-declaration
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `variable-declaration` | 变量声明。声明的变量名成为模板名。 |
| `parameter-list` | 非空的逗号分隔的模板参数列表。每个参数可以是：非类型参数、类型参数、模板参数，或上述任意类型的参数包。 |
| `constraint` | 约束表达式，用于限制此变量模板接受的模板参数。C++20 引入。 |

### 命名空间作用域

变量模板可以在命名空间作用域中声明，此时 `variable-declaration` 声明一个变量：

```cpp
template<class T>
constexpr T pi = T(3.1415926535897932385L);  // 变量模板
```

### 类作用域（静态数据成员模板）

在类作用域中，变量模板声明一个静态数据成员模板：

```cpp
struct limits {
    template<typename T>
    static const T min;  // 静态数据成员模板声明
};

template<typename T>
const T limits::min = {};  // 静态数据成员模板定义
```

### 实例化变量术语

从变量模板实例化的变量称为**实例化变量 (instantiated variable)**。从静态数据成员模板实例化的静态数据成员称为**实例化静态数据成员 (instantiated static data member)**。

## 4. 底层原理

### 隐式实例化机制

除非变量模板被显式特化或显式实例化，否则在以下情况下会发生隐式实例化：

1. **需要定义存在时**：当变量模板的特化在需要变量定义存在的上下文中被引用
2. **影响程序语义时**：如果定义的存在会影响程序的语义

```cpp
template<typename T>
constexpr T value = T(42);

int main() {
    // 隐式实例化 value<int>
    std::cout << value<int> << std::endl;  // 触发实例化
}
```

### 常量求值的影响

如果变量被表达式需要用于常量求值，则认为该变量的定义存在会影响程序语义，即使在以下情况下也会触发实例化：

- 不要求对该表达式进行常量求值
- 常量表达式求值不使用该定义

这确保了常量表达式的语义一致性。

### 与其他模板类型的区别

| 特性 | 变量模板 | 函数模板 | 类模板 |
|------|---------|---------|--------|
| 实例化结果 | 变量 | 函数 | 类型 |
| 可作为模板模板参数 | 否 | 是 | 是 |
| 作用域 | 命名空间/类 | 命名空间/类 | 命名空间 |
| 支持约束 (C++20) | 是 | 是 | 是 |

### 静态数据成员模板的链接

静态数据成员模板的定义可能需要在类定义之外提供：

```cpp
template<class T>
class X {
    static T s;  // 非模板静态数据成员的声明
};

template<class T>
T X<T>::s = 0;  // 类模板的非模板数据成员的定义
```

注意区分：
- **静态数据成员模板**：带有模板参数的静态成员
- **类模板的非模板静态数据成员**：类模板中的普通静态成员

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 数学常量定义 | 为不同数值类型（float、double、long double）定义精确的常量 |
| 类型特征值 | 定义与类型相关的常量值，如类型大小、对齐要求等 |
| 配置参数 | 根据类型参数化的配置值 |
| 静态数据成员模板 | 类中需要根据类型参数化的静态成员 |

### 最佳实践

1. **使用 constexpr 配合变量模板**：确保编译期求值
2. **为浮点类型提供精确常量**：利用不同类型的精度特性
3. **配合类型萃取使用**：提供类型相关的元信息

### 注意事项

1. **变量模板不能作为模板模板参数**：这是 C++ 标准的限制
2. **静态数据成员模板定义**：可能需要在类外提供定义
3. **实例化时机**：注意隐式实例化可能影响编译时间

### 与传统方案的对比

```cpp
// 方案 1：类模板静态成员（C++14 之前）
template<typename T>
struct MathConstants {
    static constexpr T PI = T(3.1415926535897932385L);
};
// 使用：MathConstants<double>::PI

// 方案 2：constexpr 函数（C++11）
template<typename T>
constexpr T PI() { return T(3.1415926535897932385L); }
// 使用：PI<double>()

// 方案 3：变量模板（C++14 推荐）
template<typename T>
constexpr T pi = T(3.1415926535897932385L);
// 使用：pi<double>
```

## 6. 代码示例

### 基础用法：数学常量

```cpp
#include <iostream>
#include <iomanip>

// 定义圆周率变量模板
template<typename T>
constexpr T pi = T(3.1415926535897932385L);

// 使用变量模板的函数模板
template<typename T>
T circular_area(T r) {
    return pi<T> * r * r;  // pi<T> 是变量模板实例化
}

int main() {
    std::cout << std::setprecision(20);

    // 不同精度的圆周率
    std::cout << "pi<float>:       " << pi<float> << std::endl;
    std::cout << "pi<double>:      " << pi<double> << std::endl;
    std::cout << "pi<long double>: " << pi<long double> << std::endl;

    // 计算圆面积
    std::cout << "Area (r=2.0): " << circular_area(2.0) << std::endl;

    return 0;
}
```

### 高级用法：静态数据成员模板

```cpp
#include <complex>
#include <array>

using namespace std::literals;

// 模拟矩阵类型
template<typename T, std::size_t N>
using hermitian_matrix = std::array<std::array<std::complex<T>, N>, N>;

struct matrix_constants {
    // 别名模板
    template<class T>
    using pauli = hermitian_matrix<T, 2>;

    // 静态数据成员模板：Pauli 矩阵
    template<class T>
    static constexpr pauli<T> sigmaX = {{{{0, 1}}, {{1, 0}}}};

    template<class T>
    static constexpr pauli<T> sigmaY = {{{{0, -1i}, {1i, 0}}}};

    template<class T>
    static constexpr pauli<T> sigmaZ = {{{{1, 0}, {0, -1}}}};
};

int main() {
    // 使用静态数据成员模板
    auto sigma_x = matrix_constants::sigmaX<double>;
    auto sigma_y = matrix_constants::sigmaY<float>;

    return 0;
}
```

### C++20 约束用法

```cpp
#include <concepts>
#include <iostream>

// 仅限数值类型的常量
template<std::floating_point T>
constexpr T e = T(2.71828182845904523536L);

template<std::integral T>
constexpr T max_value = std::numeric_limits<T>::max();

int main() {
    std::cout << "e<double>: " << e<double> << std::endl;  // OK
    // e<int>;  // 编译错误：int 不是浮点类型

    std::cout << "max_value<int>: " << max_value<int> << std::endl;  // OK
    // max_value<double>;  // 编译错误：double 不是整数类型

    return 0;
}
```

### 常见错误及修正

#### 错误 1：遗漏 constexpr 导致运行时初始化

```cpp
// 错误：非 constexpr 可能导致运行时初始化
template<typename T>
T value = T(42);  // 每次访问可能触发运行时初始化

// 正确：使用 constexpr 确保编译期常量
template<typename T>
constexpr T value = T(42);  // 编译期初始化
```

#### 错误 2：静态数据成员模板缺少定义

```cpp
// 声明
struct config {
    template<typename T>
    static T default_value;  // 声明
};

// 错误：使用时未定义会导致链接错误
// int x = config::default_value<int>;

// 正确：提供类外定义
template<typename T>
T config::default_value = T{};

int main() {
    int x = config::default_value<int>;  // OK
}
```

#### 错误 3：将变量模板用作模板模板参数

```cpp
template<template<typename> class V>
struct wrapper {};

template<typename T>
constexpr T my_value = T(10);

// 错误：变量模板不能作为模板模板参数
// wrapper<my_value> w;  // 编译错误

// 正确：使用类模板
template<typename T>
struct my_value_struct {
    static constexpr T value = T(10);
};

wrapper<my_value_struct> w;  // OK
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| 引入版本 | C++14 |
| 主要用途 | 定义参数化的变量或静态数据成员 |
| 实例化时机 | 隐式实例化（引用时）或显式实例化 |
| C++20 增强 | 支持 `requires` 约束 |

### 技术对比

| 方案 | 语法 | 使用方式 | 推荐度 |
|------|------|---------|--------|
| 变量模板 (C++14) | `template<T> T var = ...;` | `var<Type>` | 推荐 |
| 类模板静态成员 | `struct S { static T var; };` | `S<Type>::var` | 较繁琐 |
| constexpr 函数 | `template<T> constexpr T f() {...}` | `f<Type>()` | 语义不够直观 |

### 学习建议

1. **优先使用变量模板**：在需要参数化变量的场景，C++14 起应首选变量模板
2. **配合 constexpr**：大多数变量模板应声明为 constexpr 以获得编译期求值
3. **理解实例化规则**：掌握隐式实例化的触发条件有助于优化编译时间
4. **C++20 约束**：利用约束提高模板的类型安全性

### 缺陷报告

| 编号 | 应用版本 | 原行为 | 修正后行为 |
|------|---------|--------|-----------|
| CWG 2255 | C++14 | 静态数据成员模板的特化是否为静态数据成员不明确 | 明确为静态数据成员 |

### 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/variable_template
- C++14 Standard: [temp.variadic]
- C++20 Standard: [temp.constr] (Constraints)