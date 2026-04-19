# Templates - 模板

## 1. 概述

模板（template）是 C++ 的核心特性之一，是一种支持参数化多态（parametric polymorphism）的机制。模板定义了一族（family）实体，通过模板参数实现代码的泛型化。

模板可以定义以下类型的实体：

| 实体类型 | 说明 | 引入版本 |
|---------|------|---------|
| 类模板（class template） | 定义一族类，包括嵌套类 | C++98 |
| 函数模板（function template） | 定义一族函数，包括成员函数 | C++98 |
| 别名模板（alias template） | 定义一族类型别名 | C++11 |
| 变量模板（variable template） | 定义一族变量 | C++14 |
| 概念（concept） | 定义约束条件 | C++20 |

模板通过一个或多个模板参数（template parameters）进行参数化，参数分为三种类型：
- **类型模板参数**（type template parameter）
- **非类型模板参数**（non-type template parameter）
- **模板模板参数**（template template parameter）

当提供模板实参后，或通过推导（函数模板及 C++17 的类模板）获得实参后，这些实参会替换模板参数，从而获得模板的**特化**（specialization），即一个具体的类型或函数左值。

## 2. 来源与演变

### 历史背景

模板机制最初由 Bjarne Stroustrup 设计，旨在支持泛型编程（generic programming）。C++ 模板的设计受到以下因素驱动：

1. **代码复用**：避免为不同类型编写重复代码
2. **类型安全**：在编译期进行类型检查
3. **性能**：编译期实例化，无运行时开销

### 版本演变

#### C++98

- 引入类模板和函数模板
- 支持显式特化（explicit specialization）和偏特化（partial specialization）
- 引入 `export` 关键字（用于分离模板声明和定义）

#### C++11

- **移除 `export` 关键字**：由于实现复杂且互不兼容，`export` 被移除
- **引入别名模板**（alias template）：使用 `using` 定义类型别名模板
- **引入变参模板**（variadic template）：支持任意数量的模板参数
- **引入模板参数包**（template parameter pack）

#### C++14

- **引入变量模板**（variable template）：允许定义参数化的变量
- 放宽函数模板返回类型推导

#### C++17

- **类模板参数推导**（class template argument deduction, CTAD）：编译器可从构造函数推导模板参数
- `if constexpr`：编译期条件编译

#### C++20

- **引入概念**（concept）：为模板参数提供约束机制
- **约束子句**（requires-clause）：在模板声明中指定约束
- 更强大的模板参数推导

### 缺陷报告

| DR | 应用版本 | 原发布行为 | 修正行为 |
|----|---------|-----------|---------|
| CWG 2293 | C++98 | 未提供判断模板标识符有效性的规则 | 添加规则 |
| CWG 2682 | C++98/C++14 | 缺少模板化函数/模板化变量定义 | 添加定义 |
| P2308R1 | C++98 | 非类型模板参数不等价时模板标识符不同 | 改为参数值不等价时不同 |

## 3. 语法与参数

### 基本语法

```cpp
template <parameter-list> requires-clause(可选) declaration    // (1) 基本形式
export template <parameter-list> declaration                    // (2) C++11 前的导出形式（已移除）
template <parameter-list> concept concept-name = constraint-expression;  // (3) C++20 概念定义
```

#### 参数说明

| 参数 | 说明 |
|------|------|
| `parameter-list` | 非空的逗号分隔模板参数列表，每个参数可以是：非类型参数、类型参数、模板参数，或（C++11 起）参数包 |
| `requires-clause` | （C++20 起）指定模板实参约束的 requires 子句 |
| `declaration` | 声明：类（包括 struct 和 union）、成员类或成员枚举、函数或成员函数、命名空间作用域的静态数据成员、变量或类作用域的静态数据成员（C++14 起）、别名模板（C++11 起） |

### 模板参数类型

#### 类型模板参数

```cpp
template<typename T>          // 类型参数
template<class T>            // 等价于 typename
template<typename... Ts>     // 类型参数包（C++11）
```

#### 非类型模板参数

```cpp
template<int N>              // 整型参数
template<const char* Str>    // 指针参数
template<auto X>             // 通用非类型参数（C++17）
```

#### 模板模板参数

```cpp
template<template<typename> class Container>
class Wrapper;               // Container 是一个模板
```

### 模板标识符（Template Identifier）

模板标识符具有以下语法形式：

```cpp
template-name <template-argument-list(可选)>           // (1) 简单模板标识符
operator op <template-argument-list(可选)>            // (2) 运算符函数模板标识符
operator "" identifier <template-argument-list(可选)>  // (3) 字面量运算符（C++11，已弃用）
operator user-defined-string-literal <template-argument-list(可选)>  // (4) 用户定义字面量（C++11）
```

### 模板标识符有效性条件

模板标识符在以下所有条件满足时有效：

1. 参数数量不超过模板参数数量，或存在模板参数包（C++11 起）
2. 每个不可推导的非包参数都有对应实参或默认参数
3. 每个模板实参与对应模板参数匹配
4. 每个模板实参到后续模板参数的代入成功
5. （C++20 起）如果模板标识符非依赖，相关约束必须满足

无效的简单模板标识符会导致编译错误，除非它命名一个函数模板特化（此时可能适用 SFINAE）。

## 4. 底层原理

### 模板实例化（Instantiation）

当类模板特化在需要完整对象类型的上下文中被引用，或函数模板特化在需要函数定义存在的上下文中被引用时，模板会被**实例化**（instantiate），即编译器生成实际代码。

实例化规则：

1. **隐式实例化**：编译器根据使用自动实例化
2. **显式实例化**：程序员显式请求实例化
3. **显式特化**：为特定参数提供专门实现

```cpp
// 显式实例化
template class std::vector<int>;

// 显式特化
template<>
class std::vector<bool> { /* 特殊实现 */ };
```

### 实例化时机

- 类模板实例化不会自动实例化其成员函数，除非成员函数也被使用
- 链接时，不同翻译单元生成的相同实例化会被合并
- 模板定义必须在隐式实例化点可见（因此模板库通常只有头文件）

### SFINAE 原则

**SFINAE**（Substitution Failure Is Not An Error）是模板元编程的核心机制：模板参数替换失败不会导致编译错误，而是简单地排除该模板重载。

```cpp
template<typename T>
auto f(T t) -> decltype(t.size(), void()) {
    // T 有 size() 方法时匹配
}

template<typename T>
void f(...) {
    // 兜底版本
}
```

### 模板化实体（Templated Entity）

**模板化实体**（或称 "temploid"）是在模板定义内定义或创建的任何实体。包括：

- 类/函数/变量模板
- 概念（C++20 起）
- 模板化实体的成员（如类模板的非模板成员函数）
- 枚举的枚举项
- 在模板化实体内定义的任何实体：局部类、局部变量、友元函数等
- （C++11 起）lambda 表达式的闭包类型

```cpp
template<typename T>
struct A {
    void f() {}  // f 不是函数模板，但仍是模板化实体
};
```

### 特化（Specialization）

模板特化分为两种：

| 特化类型 | 适用范围 | 说明 |
|---------|---------|------|
| 全特化（full specialization） | 类、变量、函数模板 | 为特定参数提供完全定制的实现 |
| 偏特化（partial specialization） | 类、变量模板（C++14 起） | 为部分参数定制实现 |

```cpp
// 全特化
template<>
struct S<int> { /* ... */ };

// 偏特化
template<typename T>
struct S<T*> { /* ... */ };
```

## 5. 使用场景

### 适合使用模板的场景

| 场景 | 说明 |
|------|------|
| 泛型容器 | 如 `std::vector<T>`、`std::map<K, V>` |
| 泛型算法 | 如 `std::sort`、`std::find` |
| 类型安全接口 | 避免使用 `void*` 和强制转换 |
| 编译期计算 | 模板元编程、常量表达式 |
| 代码复用 | 避免为不同类型重复编写相似代码 |

### 最佳实践

1. **头文件中提供定义**：模板定义必须在每个使用点可见，通常放在头文件中
2. **使用概念约束参数**（C++20）：提供更清晰的错误信息
3. **合理使用特化**：为特定类型提供优化实现
4. **避免代码膨胀**：将不依赖模板参数的代码提取到基类或非模板函数中

### 常见陷阱

1. **定义位置不当**：模板定义放在 .cpp 文件中导致链接错误
2. **过度复杂化**：模板错误信息难以理解
3. **编译时间增加**：大量模板实例化会增加编译时间
4. **代码膨胀**：每种实例化都会生成独立代码

## 6. 代码示例

### 基础用法：函数模板

```cpp
#include <iostream>
#include <string>

// 函数模板：比较两个值
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    std::cout << max(3, 5) << std::endl;           // int -> 5
    std::cout << max(3.14, 2.71) << std::endl;     // double -> 3.14
    std::cout << max(std::string("abc"), std::string("xyz")) << std::endl;  // xyz

    return 0;
}
```

### 基础用法：类模板

```cpp
#include <iostream>

// 类模板：简单的容器
template<typename T>
class Container {
private:
    T value;
public:
    Container(T v) : value(v) {}
    T get() const { return value; }
    void set(T v) { value = v; }
};

int main() {
    Container<int> c1(42);
    Container<double> c2(3.14);

    std::cout << c1.get() << std::endl;  // 42
    std::cout << c2.get() << std::endl;  // 3.14

    return 0;
}
```

### 高级用法：变量模板（C++14）

```cpp
#include <iostream>

// 变量模板：数学常量
template<typename T>
constexpr T pi = T(3.14159265358979323846);

// 变量模板特化
template<>
constexpr int pi<int> = 3;

int main() {
    std::cout << pi<float> << std::endl;    // float 精度
    std::cout << pi<double> << std::endl;   // double 精度
    std::cout << pi<int> << std::endl;       // 3（特化版本）

    return 0;
}
```

### 高级用法：别名模板（C++11）

```cpp
#include <vector>
#include <memory>

// 别名模板
template<typename T>
using Vec = std::vector<T>;

template<typename T>
using Ptr = std::unique_ptr<T>;

int main() {
    Vec<int> v = {1, 2, 3};       // 等价于 std::vector<int>
    Ptr<int> p = std::make_unique<int>(42);

    return 0;
}
```

### 高级用法：概念约束（C++20）

```cpp
#include <iostream>
#include <concepts>

// 定义概念
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

// 使用概念约束模板参数
template<Numeric T>
T add(T a, T b) {
    return a + b;
}

// 或使用 requires 子句
template<typename T>
    requires std::is_arithmetic_v<T>
T multiply(T a, T b) {
    return a * b;
}

int main() {
    std::cout << add(1, 2) << std::endl;        // 3
    std::cout << multiply(2.5, 4.0) << std::endl;  // 10

    // add(std::string("a"), std::string("b"));  // 编译错误：不满足 Numeric 概念

    return 0;
}
```

### 常见错误及修正

#### 错误 1：模板定义与声明分离

```cpp
// ❌ 错误：模板定义放在 .cpp 文件中
// header.h
template<typename T>
void foo(T t);

// source.cpp
template<typename T>
void foo(T t) { /* 实现 */ }

// main.cpp
foo(42);  // 链接错误：未定义的引用

// ✅ 修正：将模板定义放在头文件中
// header.h
template<typename T>
void foo(T t) { /* 实现 */ }
```

#### 错误 2：模板标识符参数不匹配

```cpp
template<class T, typename T::type n = 0>
class X;

struct S {
    using type = int;
};

using T1 = X<S, int, int>;  // ❌ 错误：参数过多
using T2 = X<>;             // ❌ 错误：第一个参数无默认值
using T3 = X<1>;            // ❌ 错误：值 1 不匹配类型参数
using T4 = X<int>;          // ❌ 错误：int 没有 type 成员，代入失败
using T5 = X<S>;            // ✅ 正确
```

#### 错误 3：概念约束不满足（C++20）

```cpp
template<typename T>
concept C1 = sizeof(T) != sizeof(int);

template<C1 T>
struct S1 {};

S1<int>* p;   // ❌ 错误：约束不满足（sizeof(int) == sizeof(int)）
S1<char> s;   // ✅ 正确：sizeof(char) != sizeof(int)
```

## 7. 总结

模板是 C++ 泛型编程的基石，提供以下核心能力：

| 特性 | 说明 |
|------|------|
| 代码复用 | 一次编写，适用于多种类型 |
| 类型安全 | 编译期类型检查 |
| 零开销抽象 | 编译期实例化，无运行时开销 |
| 编译期计算 | 支持模板元编程 |

### 关键要点

1. **模板定义必须可见**：隐式实例化需要模板定义在编译点可见
2. **三种模板参数**：类型参数、非类型参数、模板模板参数
3. **两种特化**：全特化（类、变量、函数模板）、偏特化（类、变量模板）
4. **SFINAE 原则**：参数替换失败不是错误
5. **概念约束**（C++20）：提供更清晰的模板参数约束

### 版本特性速查

| C++ 版本 | 新增特性 |
|---------|---------|
| C++98 | 类模板、函数模板、显式特化、偏特化 |
| C++11 | 别名模板、变参模板、参数包（移除 export） |
| C++14 | 变量模板 |
| C++17 | 类模板参数推导（CTAD）、if constexpr |
| C++20 | 概念、requires 子句、约束模板 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/templates
- C++ Standard: [temp] 模板章节
- "C++ Templates: The Complete Guide", David Vandevoorde, Nicolai M. Josuttis