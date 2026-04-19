# Argument-Dependent Lookup (ADL) - 参数依赖查找

## 1. 概述

**参数依赖查找**（Argument-Dependent Lookup，简称 ADL），又称为 **Koenig 查找**（Koenig Lookup），是 C++ 中用于查找非限定函数名称的一套规则。在函数调用表达式中，包括对重载运算符的隐式调用，这些函数名除了在常规非限定名称查找所考虑的作用域和命名空间中进行查找外，还会在其参数所在的命名空间中进行查找。

ADL 的核心作用是使得在不同命名空间中定义的运算符能够被正确使用，是实现运算符重载和泛型编程的关键机制之一。

## 2. 来源与演变

### 历史背景

ADL 由 **Andrew Koenig** 提出，他是 AT&T 贝尔实验室的研究员，也是 C++ 标准委员会的长期成员。这一机制的设计动机源于以下问题：

1. **运算符重载的可见性问题**：当用户定义类型位于特定命名空间时，如何让该命名空间中的运算符函数能够被发现？
2. **泛型代码的灵活性**：标准库算法如何能够调用用户定义命名空间中的函数？

### 设计动机

考虑以下场景：

```cpp
namespace MyLib {
    struct Point { int x, y; };
    Point operator+(const Point& a, const Point& b);
}

void foo() {
    MyLib::Point p1, p2;
    p1 + p2;           // 如何找到 MyLib::operator+ ?
    // 如果没有 ADL，需要写成 MyLib::operator+(p1, p2)
}
```

ADL 的出现解决了这个问题：当编译器看到 `p1 + p2` 时，会自动在参数 `p1` 和 `p2` 的命名空间（`MyLib`）中查找 `operator+`。

### 版本变更

| 版本 | 变更内容 |
|------|---------|
| C++98 | ADL 首次纳入标准 |
| C++11 | 内联命名空间（inline namespace）纳入 ADL 查找范围 |
| C++11 | range-for 循环对 `begin` 和 `end` 的 ADL 查找 |
| C++17 | 结构化绑定对 `get` 的 ADL 查找 |
| C++20 | 消除函数模板显式模板参数需要普通查找先找到声明的限制 |

## 3. 语法与规则

### 基本规则

当进行函数调用时，ADL 遵循以下查找规则：

**第一步：确定是否触发 ADL**

如果常规非限定查找的结果包含以下任一情况，则**不进行 ADL**：
1. 类成员声明
2. 块作用域中的函数声明（非 using 声明）
3. 非函数或函数模板的声明（如函数对象或变量）

**第二步：确定关联命名空间和类集合**

对于每个函数参数，根据其类型确定**关联命名空间和类集合**：

| 参数类型 | 关联集合 |
|---------|---------|
| 基本类型（int, double 等） | 空集 |
| 类类型（含联合体） | 类本身；所有直接和间接基类；如果是嵌套类则包含其所属类；这些类所在的最内层命名空间 |
| 类模板特化 | 上述类类型的规则 + 所有模板类型参数的关联集合 + 模板模板参数所在的命名空间和类 |
| 枚举类型 | 枚举声明所在的最内层命名空间；如果是类成员则包含该类 |
| 指针类型 `T*` 或 `T(*)[N]` | 类型 `T` 的关联集合 |
| 函数类型 | 参数类型和返回类型的关联集合 |
| 成员函数指针 `R (C::*)(Args...)` | 类 `C`、参数类型、返回类型的关联集合 |
| 数据成员指针 `T C::*` | 类 `C` 和类型 `T` 的关联集合 |
| 重载函数集名称/取地址表达式 | 集合中每个函数的关联集合 |

**第三步：内联命名空间处理（C++11 起）**

- 如果关联集合中的命名空间是内联命名空间，则其外围命名空间也加入集合
- 如果关联集合中的命名空间直接包含内联命名空间，则该内联命名空间也加入集合

**第四步：合并查找结果**

将常规非限定查找的结果与 ADL 查找的结果合并，遵循以下规则：
1. 关联命名空间中的 using 指令被忽略
2. 在关联类中声明的命名空间级友元函数可通过 ADL 找到（即使普通查找看不到）
3. 忽略所有非函数和非函数模板的名称

### ADL 仅查找的上下文

某些上下文中，仅进行 ADL 查找（不进行常规非限定查找）：

| 上下文 | 引入版本 |
|-------|---------|
| range-for 循环查找 `begin` 和 `end`（成员查找失败后） | C++11 |
| 模板实例化点的依赖名称查找 | C++98 |
| 结构化绑定声明对元组类型查找 `get` | C++17 |

## 4. 底层原理

### 查找机制

ADL 的实现机制涉及编译器的名称解析阶段：

```
函数调用 f(args...)
    |
    v
常规非限定查找 --> 找到声明?
    |                    |
    | 否                  | 是
    v                    v
检查是否排除 ADL <--- 检查声明类型
    |                    |
    | 不排除              | 排除（类成员/块作用域函数/非函数）
    v                    |
对每个参数计算关联集合      |
    |                    |
    v                    |
在关联集合中查找 f         |
    |                    |
    v                    |
合并两个查找结果 <--------+
    |
    v
重载决议
```

### 关联集合计算示例

```cpp
namespace N {
    struct A {};
    struct B : A {};

    template<typename T>
    struct C {
        T value;
    };

    inline namespace Inner {
        struct D {};
    }
}

// 参数类型         关联集合
// N::A            {N::A, N}
// N::B            {N::B, N::A, N}
// N::C<N::A>      {N::C<N::A>, N::A, N}
// N::D            {N::D, N} (内联命名空间)
// N::A*           {N::A, N}
// void(*)(N::A)   {N::A, N}
```

### 与模板实例化的交互

ADL 在模板实例化时特别重要：

```cpp
namespace MyNS {
    struct Data {};
    void process(const Data&);  // 自定义处理函数
}

template<typename T>
void genericProcess(const T& value) {
    process(value);  // 依赖名称查找：实例化时进行 ADL
}

// 实例化时，ADL 找到 MyNS::process
MyNS::Data d;
genericProcess(d);  // 调用 MyNS::process
```

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 运算符重载 | 使得定义在类命名空间中的运算符能被找到 |
| 泛型编程 | 标准库算法能找到用户定义命名空间中的函数 |
| swap 惯用法 | `using std::swap; swap(a, b);` 依赖 ADL |
| 友元函数定义 | 类内定义的友元函数可通过 ADL 找到 |

### swap 惯用法

这是 ADL 最经典的应用：

```cpp
// ❌ 错误方式：直接调用 std::swap
std::swap(obj1, obj2);  // 无法找到用户定义的 swap

// ❌ 危险方式：仅使用非限定调用
swap(obj1, obj2);  // 如果没有用户 swap，编译失败

// ✅ 正确方式：引入 std::swap 作为后备
using std::swap;
swap(obj1, obj2);
// ADL 优先查找用户命名空间中的 swap
// 如果找不到，则使用 std::swap
```

标准库中的 `std::iter_swap` 和其他算法都使用这种方式。

### 注意事项

#### 1. 不要为 std 类型添加运算符

```cpp
// ❌ 错误：为 std 类型定义运算符
namespace std {
    template<typename T>
    ostream& operator<<(ostream& os, const vector<T>& v);  // 未定义行为！
}

// 原因：ADL 在模板实例化时可能找不到这些运算符
// 因为模板实例化点可能看不到这个命名空间扩展
```

#### 2. 类内友元函数定义

```cpp
template<typename T>
struct number {
    number(int);
    friend number gcd(number x, number y) { return 0; }
    // 友元函数在类内定义，没有外部声明
};

void g() {
    number<double> a(3), b(4);
    a = gcd(a, b);     // OK：ADL 找到 gcd
    // b = gcd(3, 4);  // 错误：gcd 不可见（参数类型是 int，无关联命名空间）
}
```

#### 3. 显式模板参数与 ADL（C++20 前）

```cpp
namespace N1 {
    struct S {};
    template<int X>
    void f(S);
}

namespace N2 {
    template<class T>
    void f(T t);
}

void g(N1::S s) {
    f<3>(s);        // C++20 前：语法错误（普通查找找不到 f）
                    // C++20 起：OK
    N1::f<3>(s);    // OK：限定查找找到模板
    N2::f<3>(s);    // 错误：N2::f 不接受非类型参数

    using N2::f;
    f<3>(s);        // OK：普通查找找到 N2::f，然后 ADL 找到 N1::f
}
```

### 常见陷阱

#### 歧义问题

```cpp
namespace A {
    struct X;
    void g(X);
}

namespace B {
    void g(A::X x) {
        g(x);  // 错误：歧义！
               // B::g（普通查找）vs A::g（ADL）
    }
}
```

#### 无限递归

```cpp
namespace B {
    void f(int i) {
        f(i);  // 无限递归：调用 B::f，不是 ADL
    }
}
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

int main() {
    // ADL 示例：std::ostream 的 operator<<
    std::cout << "Test\n";
    // 全局命名空间没有 operator<<
    // 但 ADL 检查 std 命名空间（左参数 std::cout 所在的命名空间）
    // 找到 std::operator<<(std::ostream&, const char*)

    // 函数调用形式
    operator<<(std::cout, "Test\n");  // 同上，ADL 生效

    // ADL 不适用的例子
    // std::cout << endl;  // 错误：endl 不是函数调用，ADL 不适用

    endl(std::cout);  // OK：函数调用，ADL 找到 std::endl

    // (endl)(std::cout);  // 错误：(endl) 不是非限定 id，ADL 不适用
}
```

### 自定义命名空间中的运算符

```cpp
#include <iostream>

namespace Geometry {
    struct Point {
        double x, y;
    };

    // 定义在 Point 相同的命名空间中
    Point operator+(const Point& a, const Point& b) {
        return {a.x + b.x, a.y + b.y};
    }

    std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
}

int main() {
    Geometry::Point p1{1.0, 2.0};
    Geometry::Point p2{3.0, 4.0};

    // ADL 找到 Geometry::operator+
    auto p3 = p1 + p2;

    // ADL 找到 Geometry::operator<<
    std::cout << p3 << std::endl;  // 输出: (4, 6)
}
```

### swap 惯用法实现

```cpp
#include <utility>
#include <iostream>
#include <algorithm>

namespace MyContainer {
    class BigData {
    public:
        int* data;
        size_t size;

        BigData(size_t n) : data(new int[n]()), size(n) {}
        ~BigData() { delete[] data; }

        // 自定义 swap 提供高效交换
        friend void swap(BigData& a, BigData& b) noexcept {
            using std::swap;
            swap(a.data, b.data);
            swap(a.size, b.size);
        }
    };
}

template<typename T>
void efficientSwap(T& a, T& b) {
    using std::swap;
    swap(a, b);  // ADL 可能找到用户定义的 swap
}

int main() {
    MyContainer::BigData d1(100), d2(200);

    // 正确的 swap 调用方式
    using std::swap;
    swap(d1, d2);  // ADL 找到 MyContainer::swap

    // 或使用标准库函数
    std::swap(d1, d2);  // C++11 起，内部也会使用 ADL
}
```

### 常见错误及修正

#### 错误 1：忽略 ADL 导致找不到函数

```cpp
namespace Lib {
    struct Data {};
    void process(const Data&);  // 用户扩展
}

// ❌ 错误：显式指定命名空间
void foo() {
    Lib::Data d;
    std::process(d);  // 错误：std 中没有 process
}

// ✅ 修正：让 ADL 工作
void foo() {
    Lib::Data d;
    process(d);  // ADL 找到 Lib::process
}
```

#### 错误 2：命名空间污染导致的歧义

```cpp
namespace A {
    struct X {};
    void foo(X);
}

namespace B {
    void foo(A::X);
}

using namespace A;
using namespace B;

// ❌ 错误：歧义
void bar() {
    X x;
    foo(x);  // 歧义：A::foo vs B::foo
}

// ✅ 修正：显式限定
void bar() {
    X x;
    A::foo(x);  // 明确指定
}
```

#### 错误 3：类成员与 ADL 冲突

```cpp
namespace N {
    struct X {};
    void f(X);
}

struct MyClass {
    void f(N::X);  // 成员函数

    void test() {
        N::X x;
        f(x);  // 调用成员函数 MyClass::f，ADL 不触发
        // ::f(x);  // 错误：全局没有 f
    }
};

// ✅ 修正：使用自由函数或显式调用
void test2() {
    N::X x;
    f(x);  // ADL 找到 N::f
}
```

## 7. 总结

### 核心要点

1. **ADL 是 C++ 名称查找的关键机制**：它使得在参数命名空间中定义的函数能够被自动发现
2. **主要应用于运算符重载和泛型编程**：是标准库和用户代码协同工作的基础
3. **swap 惯用法是 ADL 的经典应用**：`using std::swap; swap(a, b);`
4. **理解 ADL 触发条件**：只在非限定函数调用时触发

### ADL 适用性速查

| 场景 | ADL 是否生效 |
|------|------------|
| `f(x)` | 是（非限定调用） |
| `N::f(x)` | 否（限定调用） |
| `(f)(x)` | 否（括号包围） |
| `obj.f(x)` | 否（成员调用） |
| `x + y` | 是（运算符调用） |
| `std::cout << x` | 是（运算符调用） |

### 学习建议

1. 理解 ADL 对于泛型编程至关重要，特别是在实现可自定义行为的模板库时
2. 在自定义命名空间中定义类型时，考虑将与类型相关的自由函数（特别是 `swap` 和运算符）放在同一命名空间
3. 避免为 `std` 命名空间中的类型添加运算符重载
4. 在模板代码中使用 `using std::swap; swap(a, b);` 模式以支持用户自定义类型

### 缺陷报告历史

| DR | 适用版本 | 原行为 | 修正行为 |
|----|---------|-------|---------|
| CWG 33 | C++98 | 重载函数集地址的关联命名空间/类未指定 | 已指定 |
| CWG 90 | C++98 | 嵌套非联合类的关联类不包含其外围类 | 包含外围类 |
| CWG 239 | C++98 | 块作用域函数声明不阻止 ADL | ADL 不适用（using 声明除外） |
| CWG 997 | C++98 | 函数模板的依赖参数类型和返回类型被排除 | 包含在内 |
| CWG 1690 | C++98/11 | ADL 找不到 lambda 和局部类类型的返回对象 | 可找到 |
| CWG 1691 | C++11 | 不透明枚举声明的 ADL 行为异常 | 已修正 |
| CWG 1692 | C++98 | 双重嵌套类无关联命名空间 | 扩展到最内层外围命名空间 |
| CWG 2857 | C++98 | 不完整类类型的关联类包含基类 | 不包含 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/adl
- Andrew Koenig: "A Personal Note About Argument-Dependent Lookup"
- Herb Sutter (1998): "What's In a Class? - The Interface Principle", C++ Report, 10(3)
- GotW #30: http://www.gotw.ca/gotw/030.htm