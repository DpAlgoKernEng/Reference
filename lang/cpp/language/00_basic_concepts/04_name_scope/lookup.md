# 名称查找（Name Lookup）

## 1. 概述（Overview）

**名称查找（Name Lookup）** 是将程序中遇到的名称与引入该名称的声明关联的过程。

当编译器遇到一个名称时，需要确定该名称引用的是哪个实体（变量、函数、类型、命名空间等），这个过程就是名称查找。

### 示例分析

对于表达式 `std::cout << std::endl;`，编译器执行以下查找：

| 名称 | 查找类型 | 查找结果 |
|------|----------|----------|
| `std` | 非限定名称查找 | 在 `<iostream>` 中找到命名空间 std 的声明 |
| `cout` | 限定名称查找 | 在命名空间 std 中找到变量声明 |
| `endl` | 限定名称查找 | 在命名空间 std 中找到函数模板声明 |
| `operator<<` | 参数依赖查找（ADL）+ 限定名称查找 | 在 std 中找到多个函数模板，在 std::ostream 中找到多个成员函数 |

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C++98 | 定义基础名称查找规则，包括限定/非限定查找、ADL |
| C++11 | 引入 `decltype` 相关查找规则 |
| C++17 | 沿用既有规则 |
| C++20 | 概念约束相关的查找规则 |
| C++23 | 沿用既有规则 |

### 重要缺陷报告

| 缺陷报告 | 变更 |
|----------|------|
| CWG 2063 | "struct hack" 现在适用于类作用域（修复 C 兼容性） |
| CWG 2218 | 非函数（模板）名称可以关联多个声明（如果声明同一实体） |

## 3. 语法与参数（Syntax and Parameters）

### 3.1 查找类型

C++ 名称查找分为两大类：

| 查找类型 | 触发条件 | 说明 |
|----------|----------|------|
| **限定名称查找** | 名称出现在 `::` 右侧 | 在指定作用域内查找 |
| **非限定名称查找** | 其他所有情况 | 按作用域层级从内到外查找 |

### 3.2 限定名称查找

当名称出现在作用域解析运算符 `::` 之后（可能在 `template` 关键字之后）：

```cpp
std::cout              // 在命名空间 std 中查找 cout
::global_var            // 在全局命名空间中查找 global_var
Class<T>::value         // 在类 Class<T> 中查找 value
std::template Foo<int>  // 使用 template 消歧
```

### 3.3 非限定名称查找

对于不在 `::` 之后的名称：

```cpp
int x = 10;           // 查找 x
func(args);           // 查找 func
cout << "hello";      // 查找 cout（可能通过 using 声明）
```

对于函数名称，非限定名称查找还包括**参数依赖查找（ADL）**。

### 3.4 参数依赖查找（ADL）

参数依赖查找（Argument-Dependent Lookup，又称 Koenig Lookup）根据函数参数的类型在关联的命名空间中查找函数：

```cpp
namespace MyLib {
    struct Data { int value; };
    void process(Data d) { /* ... */ }
}

int main() {
    MyLib::Data d{42};
    process(d);  // OK：通过 ADL 找到 MyLib::process
}
```

## 4. 底层原理（Underlying Principles）

### 4.1 函数名称的特殊处理

对于函数和函数模板名称：

1. 名称查找可关联多个同名声明
2. 参数依赖查找可能获得额外声明
3. 模板参数推导可能应用
4. 声明集合传递给**重载决议**选择最终使用的声明

**重要**：成员访问规则（如 `private`、`protected`）仅在名称查找和重载决议之后才考虑。

### 4.2 非函数名称的处理

对于变量、命名空间、类等非函数名称：

- 如果多个声明声明的是**同一实体**，则可以关联多个声明
- 否则，必须产生**单一声明**才能编译通过

### 4.3 "Struct Hack"（类型/非类型隐藏）

在同一作用域内，名称的某些出现可以引用类/结构体/联合体/枚举的声明（非 typedef），而其他所有出现引用同一变量、非静态数据成员、枚举器，或都引用可能重载的函数/函数模板名称。

**规则**：此时不产生错误，但类型名称被隐藏，必须使用详述类型说明符访问：

```cpp
int x = 0;           // 变量 x
struct x { int a; }; // 类型 x，隐藏变量 x？

// 实际上在 C++ 中这是错误的，"struct hack" 有特定规则
```

正确的 "struct hack" 示例：

```cpp
struct Node { int value; };  // 类型 Node
int Node = 0;                // 变量 Node（隐藏类型 Node）

Node n;                      // 错误：Node 是变量，不是类型
struct Node n2;              // OK：使用详述类型说明符
```

### 4.4 查找顺序

非限定名称查找的顺序：

```
1. 块作用域（从内到外）
2. 命名空间作用域（从内到外）
3. 全局作用域
4. 参数依赖查找（仅函数调用）
```

## 5. 使用场景（Use Cases）

### 5.1 命名空间中的查找

```cpp
#include <iostream>

namespace Outer {
    int value = 10;

    namespace Inner {
        int value = 20;

        void print() {
            int value = 30;
            std::cout << value << std::endl;    // 30（局部）
            std::cout << Inner::value << std::endl;  // 20
            std::cout << Outer::value << std::endl;  // 10
        }
    }
}
```

### 5.2 类成员查找

```cpp
class Base {
public:
    int value;
    void func() {}
};

class Derived : public Base {
public:
    int value;  // 隐藏 Base::value

    void test() {
        value = 10;        // Derived::value
        Base::value = 20;  // 显式访问基类成员
    }
};
```

### 5.3 ADL 应用

```cpp
#include <iostream>
#include <algorithm>

namespace MyNS {
    struct Point {
        int x, y;
    };

    void swap(Point& a, Point& b) {
        std::swap(a.x, b.x);
        std::swap(a.y, b.y);
    }
}

int main() {
    MyNS::Point p1{1, 2}, p2{3, 4};
    swap(p1, p2);  // ADL 找到 MyNS::swap
    return 0;
}
```

### 5.4 常见陷阱

1. **名称隐藏**：

```cpp
int x = 10;  // 全局 x

void func() {
    int x = 20;  // 隐藏全局 x
    std::cout << x;  // 20
    std::cout << ::x;  // 10（使用 :: 访问全局）
}
```

2. **类成员隐藏基类**：

```cpp
class Base {
public:
    void func(int) {}
    void func(double) {}
};

class Derived : public Base {
public:
    void func(int) {}  // 隐藏所有 Base::func 重载
};

Derived d;
d.func(3.14);  // 调用 Derived::func(int)，不调用 Base::func(double)
d.Base::func(3.14);  // 显式调用基类版本
```

3. **ADL 意外行为**：

```cpp
namespace A {
    struct X {};
    void f(X) { std::cout << "A::f\n"; }
}

namespace B {
    void f(A::X) { std::cout << "B::f\n"; }
}

int main() {
    A::X x;
    f(x);  // 歧义：找到 A::f 和 B::f（通过 ADL）
}
```

## 6. 代码示例（Examples）

### 基础用法

```cpp
#include <iostream>

// 全局变量
int global_value = 100;

namespace MyApp {
    // 命名空间变量
    int value = 200;

    // 类定义
    class Widget {
    public:
        int value;  // 成员变量

        Widget(int v) : value(v) {}

        void print() {
            int value = 300;  // 局部变量
            std::cout << "Local: " << value << std::endl;
            std::cout << "Member: " << this->value << std::endl;
            std::cout << "Namespace: " << MyApp::value << std::endl;
            std::cout << "Global: " << ::global_value << std::endl;
        }
    };
}

int main()
{
    MyApp::Widget w(42);
    w.print();
    return 0;
}
```

### 限定与非限定查找

```cpp
#include <iostream>
#include <vector>

namespace Math {
    const double PI = 3.14159;

    double square(double x) {
        return x * x;
    }

    namespace Constants {
        const double E = 2.71828;
    }
}

int main()
{
    // 非限定查找
    using Math::PI;
    std::cout << PI << std::endl;  // 使用 using 声明后

    // 限定查找
    std::cout << Math::square(2.0) << std::endl;
    std::cout << Math::Constants::E << std::endl;

    // 嵌套限定
    namespace MC = Math::Constants;  // 命名空间别名
    std::cout << MC::E << std::endl;

    return 0;
}
```

### 参数依赖查找（ADL）

```cpp
#include <iostream>

namespace Geometry {
    struct Point {
        double x, y;
    };

    // ADL 会找到这个函数
    Point operator+(const Point& a, const Point& b) {
        return {a.x + b.x, a.y + b.y};
    }

    void print(const Point& p) {
        std::cout << "(" << p.x << ", " << p.y << ")";
    }
}

int main()
{
    Geometry::Point p1{1.0, 2.0};
    Geometry::Point p2{3.0, 4.0};

    // 不需要 Geometry::operator+，ADL 会找到它
    auto p3 = p1 + p2;

    // ADL 也找到 print
    print(p3);  // 输出: (4, 6)

    return 0;
}
```

### 类型/非类型隐藏

```cpp
#include <iostream>

void demonstrate_struct_hack()
{
    struct Node { int value; };  // 类型 Node

    // 在同一作用域中
    // int Node = 0;  // 这会隐藏类型 Node

    // 使用详述类型说明符
    struct Node n{42};
    std::cout << n.value << std::endl;
}

// 更复杂的例子
struct S { int x; };  // 类型 S
int S = 10;           // 变量 S，隐藏类型 S

int main()
{
    // S s;  // 错误：S 是变量
    struct S s{20};   // OK：使用详述类型说明符

    std::cout << "Variable S: " << S << std::endl;      // 10
    std::cout << "Member s.x: " << s.x << std::endl;    // 20

    demonstrate_struct_hack();
    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>
#include <string>

/* 错误：名称隐藏导致的意外行为 */
namespace A {
    void f(int) { std::cout << "A::f(int)\n"; }
}

namespace B {
    void f(double) { std::cout << "B::f(double)\n"; }
}

using namespace A;
using namespace B;

void error_adl_ambiguity()
{
    // f(42);  // 歧义：A::f(int) 和 B::f(double) 都可行
}

/* 正确：显式指定命名空间 */
void correct_qualified()
{
    A::f(42);      // A::f(int)
    B::f(3.14);    // B::f(double)
}

/* 错误：基类函数被隐藏 */
class Base {
public:
    void func(int) { std::cout << "Base::func(int)\n"; }
    void func(double) { std::cout << "Base::func(double)\n"; }
};

class Derived : public Base {
public:
    void func(int) { std::cout << "Derived::func(int)\n"; }
    // 隐藏了 Base::func 的所有重载
};

void test_derived()
{
    Derived d;
    d.func(42);      // Derived::func(int)
    // d.func(3.14); // 错误：找不到 func(double)
    d.Base::func(3.14);  // OK
}

/* 正确：使用 using 声明引入基类重载 */
class Fixed : public Base {
public:
    using Base::func;  // 引入所有 Base::func 重载
    void func(int) { std::cout << "Fixed::func(int)\n"; }
};

void test_fixed()
{
    Fixed f;
    f.func(42);      // Fixed::func(int)
    f.func(3.14);    // Base::func(double) - OK
}

/* 错误：ADL 与全局函数冲突 */
void process(int x) { std::cout << "global process: " << x << "\n"; }

namespace MyNS {
    struct Data { int value; };
    void process(Data d) { std::cout << "MyNS::process: " << d.value << "\n"; }
}

void error_adl_conflict()
{
    process(42);  // OK：全局 process

    MyNS::Data d{100};
    process(d);   // OK：ADL 找到 MyNS::process
}
```

## 7. 总结（Summary）

### 核心要点

1. **名称查找目的**：将名称与声明关联
2. **两种主要类型**：限定名称查找和非限定名称查找
3. **ADL**：参数依赖查找根据参数类型在关联命名空间中查找
4. **重载决议**：函数名称查找后进行重载决议选择最终声明

### 查找类型对比

| 类型 | 触发条件 | 查找范围 |
|------|----------|----------|
| 限定名称查找 | `::` 之后 | 指定的作用域 |
| 非限定名称查找 | 其他情况 | 从内到外的作用域链 |
| 参数依赖查找 | 函数调用 | 参数类型的关联命名空间 |

### 与 C 语言的区别

| 特性 | C++ | C |
|------|-----|---|
| 命名空间查找 | 有 | 无 |
| ADL | 有 | 无 |
| 类作用域 | 有 | 无 |
| 重载决议 | 有 | 无 |
| "Struct Hack" | 有特定规则 | 名称在不同命名空间 |

### 最佳实践

- 使用限定名称明确意图，避免歧义
- 理解 ADL 行为，避免意外冲突
- 派生类中使用 `using` 声明引入基类重载
- 注意名称隐藏，必要时使用详述类型说明符
- 使用命名空间组织代码，避免全局命名污染

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 6.5 Name lookup [basic.lookup]
- C++20 标准 (ISO/IEC 14882:2020): 6.5 Name lookup [basic.lookup]
- C++17 标准 (ISO/IEC 14882:2017): 6.4 Name lookup [basic.lookup]
- C++14 标准 (ISO/IEC 14882:2014): 3.4 Name lookup [basic.lookup]
- C++11 标准 (ISO/IEC 14882:2011): 3.4 Name lookup [basic.lookup]
- C++98 标准 (ISO/IEC 14882:1998): 3.4 Name lookup [basic.lookup]