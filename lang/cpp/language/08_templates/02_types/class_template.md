# 类模板 (Class Template)

## 1. 概述 (Overview)

类模板（Class Template）是 C++ 模板机制的核心组成部分，它定义了一个类家族。通过类模板，程序员可以创建与类型无关的通用类结构，在实际使用时再指定具体的类型参数，从而实现代码复用和类型安全。

### 核心概念

- **类模板本身不是类型**：类模板只是一个蓝图，不是类型、对象或任何其他实体。
- **必须实例化才能生成代码**：仅包含模板定义的源文件不会生成任何代码。模板必须被实例化——提供模板参数——编译器才能生成实际的类。
- **类型参数化**：类模板允许将类型作为参数，使得同一个类定义可以适用于多种数据类型。

### 主要用途

1. 实现泛型容器类（如 `std::vector`、`std::map`）
2. 创建类型安全的通用算法和数据结构
3. 提供代码复用机制，减少重复代码
4. 实现编译期多态（静态多态）

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

类模板是 C++ 模板机制的一部分，模板最初由 Bjarne Stroustrup 设计，旨在支持泛型编程。模板的设计动机源于以下需求：

1. **类型安全的容器**：C 语言中的泛型容器通常依赖 `void*`，缺乏类型安全性
2. **代码复用**：避免为每种数据类型编写重复的类定义
3. **编译期效率**：模板实例化在编译期完成，没有运行时开销

### 版本演变

| 版本 | 变更内容 |
|------|----------|
| C++98 | 引入类模板基础语法，支持 `export` 关键字 |
| C++11 | 移除 `export` 关键字；引入 `extern template` 显式实例化声明 |
| C++14 | 允许显式实例化变量模板 |
| C++17 | 局部类及其成员中使用的模板作为实体实例化的一部分进行实例化 |
| C++20 | 引入 `requires` 约束子句，支持概念（Concepts）约束模板参数 |

### export 关键字的兴衰

`export` 是一个可选的修饰符，用于声明模板为**导出模板**。使用 `export` 时，实例化导出模板的文件无需包含模板定义，声明即可。然而：

- 实现复杂且罕见
- 不同实现之间存在细节分歧
- C++11 中被移除

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```
template < parameter-list > class-declaration                    (1)
template < parameter-list > requires constraint class-declaration (2) (since C++20)
export template < parameter-list > class-declaration             (3) (removed in C++11)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `class-declaration` | 类声明。声明的类名成为模板名。 |
| `parameter-list` | 非空的逗号分隔模板参数列表。每个参数可以是非类型参数、类型参数、模板参数，或上述任一的参数包。 |
| `constraint` | 约束表达式，限制此类模板接受的模板参数（C++20 起）。 |
| `class-key` | `class`、`struct` 或 `union` 之一。 |

### 显式实例化语法

```
template class-key template-name < argument-list > ;            (1) 显式实例化定义
extern template class-key template-name < argument-list > ;    (2) 显式实例化声明 (C++11 起)
```

### 模板参数类型

模板参数可以是以下类型：

1. **类型参数**（type parameter）：使用 `typename` 或 `class` 关键字声明
2. **非类型参数**（non-type parameter）：如整数、指针、引用等
3. **模板参数**（template parameter）：将模板作为参数传递
4. **参数包**（parameter pack）：使用 `...` 表示可变参数（C++11 起）

## 4. 底层原理 (Underlying Principles)

### 实例化机制

#### 显式实例化 (Explicit Instantiation)

显式实例化定义强制实例化所引用的类、结构体或联合体：

- 可出现在模板定义之后的任何位置
- 对于给定的参数列表，整个程序中只允许出现一次（无需诊断）
- 显式实例化定义会实例化所有非继承的非模板成员

显式实例化声明（`extern template`）跳过隐式实例化步骤：

- 代码使用其他地方提供的显式实例化定义
- 如果不存在实例化，会导致链接错误
- 可用于减少编译时间

#### 隐式实例化 (Implicit Instantiation)

当代码在需要完整定义类型的上下文中引用模板，且该类型尚未被显式实例化时，发生隐式实例化：

- 构造该类型的对象时会触发隐式实例化
- 构造指向该类型的指针**不会**触发实例化
- 成员按需实例化：如果成员未被使用，则不会被实例化

### 编译期代码生成

类模板采用**两阶段编译**机制：

1. **定义阶段**：检查模板定义中的非依赖名称和语法错误
2. **实例化阶段**：当模板被实例化时，检查依赖名称并生成具体代码

### 作用域规则

显式实例化只能出现在模板所在的封闭命名空间中，除非使用限定标识符（qualified-id）。

### 与显式特化的交互

如果相同模板参数的显式特化出现在显式实例化之前，显式实例化没有效果。

## 5. 使用场景 (Use Cases)

### 适用场景

1. **通用容器实现**：如 `std::vector<T>`、`std::list<T>`
2. **智能指针**：如 `std::unique_ptr<T>`、`std::shared_ptr<T>`
3. **策略模式实现**：通过模板参数传递策略类型
4. **数值计算**：支持多种数值类型的数学运算
5. **资源管理**：泛型的资源句柄类

### 最佳实践

1. **头文件中定义**：模板定义通常放在头文件中，以便编译器在各翻译单元实例化
2. **使用显式实例化减少编译时间**：在多个源文件使用同一模板实例时，显式实例化可减少重复编译
3. **合理使用 `extern template`**：避免在多个编译单元中重复实例化相同模板

### 注意事项

1. **定义可见性要求**：
   - 显式实例化函数模板、变量模板、成员函数或静态数据成员时，只需声明可见
   - 显式实例化类模板、成员类时，必须提供完整定义

2. **访问说明符被忽略**：显式实例化定义忽略成员访问说明符，参数类型和返回类型可以是私有的

3. **成员按需实例化**：未使用的成员不会被实例化，定义可以省略

### 常见陷阱

1. **定义不可见**：模板定义不可见时会导致链接错误
2. **重复显式实例化**：同一参数列表的显式实例化定义出现多次是未定义行为
3. **不完整类型**：仅声明未定义的模板被实例化时产生不完整类型错误

## 6. 代码示例 (Examples)

### 基础用法：简单的类模板

```cpp
#include <iostream>

// 定义一个简单的类模板
template<typename T>
class Container {
private:
    T value;
public:
    Container(const T& val) : value(val) {}
    T getValue() const { return value; }
    void setValue(const T& val) { value = val; }
};

int main() {
    Container<int> intContainer(42);
    Container<double> doubleContainer(3.14);

    std::cout << "Int: " << intContainer.getValue() << std::endl;
    std::cout << "Double: " << doubleContainer.getValue() << std::endl;

    return 0;
}
```

### 高级用法：显式实例化

```cpp
// 模板定义文件：my_vector.hpp
#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

template<typename T>
class MyVector {
public:
    void push_back(const T& value);
    T& operator[](size_t index);
    size_t size() const;
private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

// 声明常用的显式实例化
extern template class MyVector<int>;
extern template class MyVector<double>;

#endif // MY_VECTOR_HPP
```

```cpp
// 显式实例化定义文件：my_vector.cpp
#include "my_vector.hpp"

// 实现成员函数
template<typename T>
void MyVector<T>::push_back(const T& value) {
    // 实现代码...
}

template<typename T>
T& MyVector<T>::operator[](size_t index) {
    return data_[index];
}

template<typename T>
size_t MyVector<T>::size() const {
    return size_;
}

// 显式实例化定义
template class MyVector<int>;
template class MyVector<double>;
```

### 高级用法：约束模板（C++20）

```cpp
#include <concepts>
#include <iostream>

// 使用 concepts 约束模板参数
template<typename T>
requires std::integral<T>
class NumberCalculator {
public:
    T add(T a, T b) { return a + b; }
    T subtract(T a, T b) { return a - b; }
    T multiply(T a, T b) { return a * b; }
};

// 或使用简写形式
template<std::integral T>
class NumberCalculatorShort {
public:
    T add(T a, T b) { return a + b; }
};

int main() {
    NumberCalculator<int> calc;

    std::cout << "5 + 3 = " << calc.add(5, 3) << std::endl;
    std::cout << "5 - 3 = " << calc.subtract(5, 3) << std::endl;
    std::cout << "5 * 3 = " << calc.multiply(5, 3) << std::endl;

    // NumberCalculator<double> dcalc; // 编译错误：double 不满足 integral 约束

    return 0;
}
```

### 正确用法：命名空间中的显式实例化

```cpp
namespace N {
    template<class T>
    class Y {  // 模板定义
    public:
        void mf() {}
        void mg();
    };
}

// 正确用法：使用限定标识符进行显式实例化
template class N::Y<char*>;           // OK: 显式实例化
template void N::Y<double>::mf();     // OK: 成员函数显式实例化

// 错误示例：
// template class Y<int>;             // 错误：模板 Y 在全局命名空间不可见
// template class N::Y<int>;          // 错误：显式实例化必须在模板命名空间内，
                                     //      或使用 using 声明后
```

### 正确用法：隐式实例化

```cpp
template<class T>
struct Z {           // 模板定义
    void f() {}      // 已定义
    void g();        // 未定义
};

template struct Z<double>;  // 显式实例化 Z<double>

Z<int> a;           // 隐式实例化 Z<int>
Z<char>* p;        // 此处不实例化任何内容（仅指针）

int main() {
    p->f();         // 此处隐式实例化 Z<char> 和 Z<char>::f()
                    // Z<char>::g() 从未被需要，不会被实例化
    return 0;
}
```

### 常见错误及修正

#### 错误 1：不完整类型实例化

```cpp
// 错误示例
template<class T>
class X;    // 仅声明，未定义

X<char> ch; // 错误：不完整类型 X<char>

// 修正：提供完整定义
template<class T>
class X {   // 完整定义
    T data;
};

X<char> ch; // OK
```

#### 错误 2：定义不可见

```cpp
// 错误示例：模板定义不可见
// header.h
template<typename T>
class BadExample;  // 仅声明

// main.cpp
#include "header.h"
int main() {
    BadExample<int> obj;  // 错误：定义不可见
    return 0;
}

// 修正：在头文件中提供完整定义
// header.h
template<typename T>
class GoodExample {
    T value;
};
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 定义方式 | `template<参数列表> class 类声明` |
| 实例化时机 | 显式实例化或隐式实例化 |
| 成员实例化 | 按需实例化，未使用成员不实例化 |
| 代码生成 | 编译期完成，无运行时开销 |
| C++11 新特性 | `extern template` 显式实例化声明 |
| C++20 新特性 | `requires` 约束子句 |

### 显式实例化 vs 隐式实例化

| 特性 | 显式实例化 | 隐式实例化 |
|------|-----------|-----------|
| 控制权 | 程序员控制 | 编译器自动 |
| 编译时间 | 可减少（配合 extern） | 可能增加 |
| 代码膨胀 | 可预测 | 不可预测 |
| 使用场景 | 库开发、优化编译 | 一般应用开发 |

### 学习建议

1. **从简单开始**：先掌握基本的类模板定义和实例化
2. **理解实例化时机**：明确何时需要完整类型，何时指针即可
3. **掌握显式实例化**：理解其在大型项目中的编译优化作用
4. **学习 C++20 约束**：掌握 `requires` 和概念的使用
5. **实践最佳实践**：遵循模板定义放置头文件的惯例

### 相关主题

- 模板参数与实参：允许模板参数化
- 函数模板声明：声明函数模板
- 模板特化：为特定类型定义现有模板
- 参数包：允许在模板中使用类型列表（C++11 起）