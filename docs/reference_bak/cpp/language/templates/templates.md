# C++ 模板（Templates）

来源：cppreference.com

## 模板概览

| 模板相关主题 | 说明 |
|--------------|------|
| **基础概念** | 模板参数与实参、类模板、函数模板 |
| **现代特性** | 变量模板（C++14）、概念约束（C++20） |
| **模板机制** | 参数推导、特化、依赖名称 |
| **高级特性** | 参数包（C++11）、折叠表达式（C++17）、SFINAE |
| **C++20新特性** | 概念与约束、requires表达式 |

**模板**是C++实体，定义以下之一：

- **类族**（类模板），可包括嵌套类
- **函数族**（函数模板），可包括成员函数
- **类型别名族**（别名模板）（C++11）
- **变量族**（变量模板）（C++14）
- **概念**（约束与概念）（C++20）

模板由一个或多个模板参数参数化：**类型模板参数**、**非类型模板参数**、**模板模板参数**。

---

## 模板工作原理

### 一、模板特化

当提供模板实参，或仅对函数和类模板（C++17）进行推导时，它们会替换模板参数以获得模板的**特化**：
- **完全特化**：适用于类、变量（C++14）、函数模板
- **部分特化**：仅适用于类模板和变量模板（C++14）

### 二、模板实例化

当类模板特化在需要完整对象类型的上下文中被引用，或函数模板特化在需要函数定义存在的上下文中被引用时，模板被**实例化**（实际编译代码），除非模板已被显式特化或显式实例化。

类模板实例化不会实例化任何成员函数，除非它们也被使用。链接时，不同翻译单元生成的相同实例化会被合并。

### 三、模板定义可见性

类模板定义必须在隐式实例化点可见，这就是为什么模板库通常在头文件中提供所有模板定义（例如，大多数boost库是仅头文件）。

---

## 模板语法

### 一、基本语法

| 语法 | 说明 |
|------|------|
| `template <parameter-list> requires-clause(optional) declaration` | (1) 基本模板声明 |
| `export template <parameter-list> declaration` | (2) 导出模板（C++11前） |
| `template <parameter-list> concept concept-name = constraint-expression;` | (3) 概念定义（C++20） |

### 二、参数列表组成

- **非空逗号分隔的模板参数列表**
- 每个参数可以是：非类型参数、类型参数、模板参数，或这些的参数包（C++11）

### 三、可选子句

- **requires子句**（C++20）：指定模板实参的约束
- **声明**：类、函数、变量、别名模板的声明，也可定义模板特化

> ⚠️ **注意**：`export`是可选修饰符，声明模板为**导出**（C++11前）。实现稀少且存在分歧，现已弃用。

---

## 模板标识符

### 一、语法形式

| 语法 | 说明 |
|------|------|
| `template-name<template-argument-list(optional)>` | (1) 简单模板标识符 |
| `operator op<template-argument-list(optional)>` | (2) 运算符函数模板标识符 |
| `operator "" identifier <template-argument-list(optional)>` | (3) 字面量运算符模板标识符（C++11，已弃用） |
| `operator user-defined-string-literal <template-argument-list(optional)>` | (4) 用户定义字符串字面量模板标识符（C++11） |

### 二、命名规则

- 简单模板标识符命名类模板特化时命名一个类
- 命名别名模板特化时命名一个类型
- 命名函数模板特化时命名一个函数

### 三、有效性检查

模板标识符在以下条件满足时**有效**：

1. 实参数量不超过形参数量，或存在模板参数包（C++11）
2. 每个无默认值的不可推导非包参数都有对应实参
3. 每个模板实参匹配对应模板参数
4. 每个模板实参替换到后续模板参数成功
5. 若模板标识符非依赖，相关约束得到满足（C++20）

无效简单模板id是编译时错误，除非它命名函数模板特化（此时可能应用SFINAE）。

### 四、示例

```cpp
template<class T, T::type n = 0>
class X;

struct S {
    using type = int;
};

using T1 = X<S, int, int>; // 错误：实参过多
using T2 = X<>;            // 错误：第一个模板参数无默认值
using T3 = X<1>;           // 错误：值1不匹配类型参数
using T4 = X<int>;         // 错误：第二个模板参数替换失败
using T5 = X<S>;           // 正确
```

---

## 模板实体概念

### 一、模板化实体（Templated Entity）

在模板定义内定义（或对lambda表达式，创建）的任何实体。包括：

- 类/函数/变量模板
- 模板化实体的成员
- 模板化实体内定义的任何实体（局部类、局部变量、友元函数等）
- 出现在模板化实体声明中的lambda表达式的闭包类型（C++11）

```cpp
template<typename T>
struct A {
    void f() {}  // A::f是模板化函数，但不是函数模板
};
```

### 二、分类

- **模板化函数**：函数模板或模板化函数
- **模板化类**：类模板或模板化类
- **模板化变量**：变量模板或模板化变量（C++14）

---

## 扩展知识详解

### 一、模板参数类型

#### 1. 类型模板参数
```cpp
template<typename T>
class Container {
    T value;
};
```

#### 2. 非类型模板参数
```cpp
template<int N>
class Array {
    int data[N];
};
```

#### 3. 模板模板参数
```cpp
template<template<typename> class Container>
class Wrapper {
    Container<int> data;
};
```

#### 4. 参数包（C++11）
```cpp
template<typename... Args>
void print(Args... args) {
    ((std::cout << args << " "), ...);  // C++17折叠表达式
}
```

### 二、现代模板特性

#### 1. 变量模板（C++14）
```cpp
template<typename T>
constexpr T pi = T(3.1415926535897932385);
```

#### 2. 概念与约束（C++20）
```cpp
template<typename T>
concept Integral = std::is_integral_v<T>;

template<Integral T>
T add(T a, T b) {
    return a + b;
}
```

#### 3. 类模板参数推导（C++17）
```cpp
std::vector v{1, 2, 3};  // 自动推导为std::vector<int>
```

#### 4. 折叠表达式（C++17）
```cpp
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // 左折叠
}
```

### 三、模板特化示例

#### 1. 函数模板特化
```cpp
template<typename T>
void func(T t) {
    std::cout << "Generic: " << t << std::endl;
}

template<>
void func<int>(int t) {  // 完全特化
    std::cout << "Int specialization: " << t << std::endl;
}
```

#### 2. 类模板特化
```cpp
template<typename T>
class MyClass {
public:
    void print() { std::cout << "Generic" << std::endl; }
};

template<>
class MyClass<int> {  // 完全特化
public:
    void print() { std::cout << "Int specialization" << std::endl; }
};

template<typename T>
class MyClass<T*> {  // 部分特化
public:
    void print() { std::cout << "Pointer specialization" << std::endl; }
};
```

#### 3. 变量模板特化（C++14）
```cpp
template<typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;

template<>
constexpr bool is_integral_v<char> = true;  // 特化
```

### 四、SFINAE原则

```cpp
template<typename T>
void func(T t, decltype(t.some_method())* = nullptr) {
    // 仅当T有some_method时有效
}

template<typename T>
void func(T t, ...) {
    // 备用重载
}
```

---

## 实际应用示例

### 一、通用容器模板

```cpp
#include <iostream>
#include <type_traits>

// 类模板
template<typename T, size_t N>
class FixedArray {
private:
    T data[N];
    
public:
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }
    size_t size() const { return N; }
};

// 函数模板
template<typename T>
void swap(T& a, T& b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

// 变量模板（C++14）
template<typename T>
constexpr bool is_numeric_v = std::is_arithmetic<T>::value;

// 概念约束（C++20）
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>
T average(T a, T b) {
    return (a + b) / 2;
}

int main() {
    FixedArray<int, 5> arr;
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = static_cast<int>(i * i);
    }
    
    std::cout << "is_numeric_v<int>: " << is_numeric_v<int> << std::endl;
    
    // C++20概念使用
    std::cout << "Average: " << average(3.5, 4.5) << std::endl;
    
    return 0;
}
```

### 二、现代模板工具类

```cpp
#include <type_traits>
#include <utility>

// 完美转发模板
template<typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));  // 完美转发
}

// 可变参数模板递归展开
template<typename T>
void print(T&& t) {
    std::cout << t << std::endl;
}

template<typename T, typename... Args>
void print(T&& t, Args&&... args) {
    std::cout << t << " ";
    print(args...);
}

// 折叠表达式（C++17）
template<typename... Args>
auto sum(Args... args) {
    return (args + ... + 0);  // 右折叠
}

template<typename... Args>
void print_all(Args&&... args) {
    ((std::cout << args << " "), ...);  // 折叠表达式
    std::cout << std::endl;
}
```

---

## 缺陷报告（Defect Reports）

| 缺陷编号 | 应用于 | 发布行为 | 修正行为 |
|----------|--------|----------|----------|
| CWG 2293 | C++98 | 未提供确定模板标识符是否有效的规则 | 提供规则 |
| CWG 2682 | C++98/C++14 | 缺少模板化函数/模板类/模板化变量的定义 | 添加定义 |
| P2308R1 | C++98 | 若对应非类型模板实参不模板参数等价则模板标识符不同 | 若对应非类型模板参数值不模板参数等价则不同 |

---

## 标准参考文献

### 相关C++标准章节：

- C++23标准
  - 13 Templates
- C++20标准
  - 13 Templates
- C++17标准
  - 17 Templates
- C++14标准
  - 14 Templates
- C++11标准
  - 14 Templates
- C++98/C++03标准
  - 14 Templates

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C泛型选择文档] | C语言泛型对照学习 |
| [类模板] | 类模板详细说明 |
| [函数模板] | 函数模板详细说明 |
| [概念与约束] | C++20概念机制 |
| [SFINAE] | 替换失败不是错误原则 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/language/templates&oldid=175216>
- 最后修改时间：2024年8月14日 17:13
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++模板系统是语言最强大的特性之一，从基本的类型泛型到现代的概念约束，提供了无与伦比的代码复用和类型安全机制。理解模板参数推导、特化、实例化等核心概念，是掌握模板编程的基础。现代C++引入的变量模板、概念约束、折叠表达式等特性，使模板编程更加直观和安全。模板特化和SFINAE机制为编写通用且高效的代码提供了强大工具。参数包和可变参数模板为处理不定数量参数提供了优雅解决方案。正确使用模板别名和using声明，能够简化复杂的模板类型。掌握模板元编程技术，能够实现编译时计算和类型操作，在性能关键场景中发挥重要作用。