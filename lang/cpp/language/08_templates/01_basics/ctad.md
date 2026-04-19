# 类模板参数推导 (Class Template Argument Deduction, CTAD) (C++17 起)

## 1. 概述 (Overview)

类模板参数推导（Class Template Argument Deduction，简称 CTAD）是 C++17 引入的一项重要特性。它允许编译器从初始化表达式的类型自动推导类模板的模板参数，从而在实例化类模板时无需显式指定所有模板参数。

在 C++17 之前，实例化类模板必须显式指定所有模板参数：

```cpp
std::pair<int, double> p(1, 2.5);  // 必须显式指定 int 和 double
```

使用 CTAD 后，可以简化为：

```cpp
std::pair p(1, 2.5);  // 自动推导为 std::pair<int, double>
```

CTAD 的核心价值：
- **简化代码**：减少冗余的类型声明
- **提高可读性**：代码更简洁清晰
- **减少错误**：避免手动指定类型时的拼写错误

### 适用场景

CTAD 在以下上下文中生效：

| 场景 | 示例 |
|------|------|
| 变量声明（类型为类模板） | `std::pair p(2, 4.5);` |
| new 表达式 | `auto y = new A{1, 2};` |
| 函数式转型表达式 | `auto lck = std::lock_guard(mtx);` |
| 非类型模板参数 (C++20) | `Y<0> y;` |

## 2. 来源与演变 (Origin and Evolution)

### 首次引入

CTAD 首次在 **C++17** 标准中引入，由提案 N45XX 系列发展而来。其设计动机是消除类模板与函数模板在参数推导方面的不对称性。

### 历史背景

在 CTAD 出现之前，C++ 存在一个不一致的设计：
- **函数模板**：参数可以从函数实参自动推导
- **类模板**：必须显式指定所有模板参数

这导致了"工厂函数"模式的流行：

```cpp
// C++17 之前：必须使用 make_* 工厂函数
auto p1 = std::make_pair(1, 2.5);     // 推导为 pair<int, double>
auto t1 = std::make_tuple(1, 2, 3.0); // 推导为 tuple<int, int, double>

// C++17 起：直接使用类模板
std::pair p2(1, 2.5);     // 等价
std::tuple t2(1, 2, 3.0); // 等价
```

### C++17 变化

- 引入类模板参数推导基础机制
- 自动生成隐式推导指引（从构造函数生成）
- 支持用户自定义推导指引

### C++20 变化

- **聚合类型的 CTAD**：为聚合类型自动生成推导指引
- **别名模板的 CTAD**：支持从别名模板推导类模板参数
- **约束传播**：隐式推导指引正确传播 requires 约束
- **推导指引语法增强**：支持 trailing requires-clause

### 功能测试宏

| 宏 | 值 | 标准 | 功能 |
|----|-----|------|------|
| `__cpp_deduction_guides` | `201703L` | C++17 | 类模板参数推导 |
| `__cpp_deduction_guides` | `201907L` | C++20 | 聚合类型和别名的 CTAD |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

当类型说明符仅为类模板名（不带模板参数列表）时，编译器会尝试推导模板参数：

```cpp
std::pair p(2, 4.5);     // 推导为 std::pair<int, double>
std::tuple t(4, 3, 2.5); // 推导为 std::tuple<int, int, double>
std::less l;             // 推导为 std::less<void>
```

### 隐式推导指引生成

对于类模板 `C`，编译器会自动生成以下推导指引：

1. **构造函数指引**：从每个构造函数生成对应的虚构函数模板
2. **默认构造指引**：如果无构造函数，从假设的 `C()` 生成
3. **复制推导候选**：从假设的 `C(C)` 生成，用于复制语义

生成的虚构函数模板遵循以下规则：

```
模板参数 = 类模板参数 + 构造函数模板参数
返回类型 = C<模板参数>
参数列表 = 构造函数参数列表
```

### 用户自定义推导指引

用户可以显式定义推导指引来控制推导行为：

```cpp
// 语法格式 (1)：非模板推导指引
explicit(可选) 模板名 ( 参数列表 ) -> 简单模板标识符 requires-clause(可选) ;

// 语法格式 (2)：模板推导指引
template <模板参数列表> requires-clause(可选)
explicit(可选) 模板名 ( 参数列表 ) -> 简单模板标识符 requires-clause(可选) ;
```

参数说明：

| 参数 | 说明 |
|------|------|
| 模板名 | 要推导参数的类模板名称 |
| 参数列表 | 函数式参数列表（不能使用占位类型） |
| 简单模板标识符 | 指定推导结果的模板特化 |

### 推导指引示例

```cpp
template<class T>
struct container
{
    container(T t) {}
    template<class Iter>
    container(Iter beg, Iter end);
};

// 用户自定义推导指引
template<class Iter>
container(Iter b, Iter e)
    -> container<typename std::iterator_traits<Iter>::value_type>;

// 使用
container c(7);                              // OK: 推导 T=int
std::vector<double> v = {/* ... */};
auto d = container(v.begin(), v.end());      // OK: 推导 T=double
container e{5, 6};                           // 错误：int 没有 iterator_traits
```

## 4. 底层原理 (Underlying Principles)

### 推导机制详解

CTAD 的推导过程可分为以下步骤：

#### 1. 候选生成

编译器首先生成候选的推导指引集合：

```
对于类模板 C，候选包括：
├── 从每个构造函数生成的隐式指引
├── 默认构造指引（如适用）
├── 复制推导候选
└── 用户自定义推导指引
```

#### 2. 虚构类型构造

编译器构造一个虚构的类类型，其构造函数签名与推导指引匹配：

```cpp
template<class T>
struct UniquePtr
{
    UniquePtr(T* t);
};

// 隐式生成的推导指引：
// F1: template<class T> UniquePtr<T> F(T* p);
// F2: template<class T> UniquePtr<T> F(UniquePtr<T>); // 复制候选

// 等价的虚构类类型：
// struct X {
//     template<class T> X(T* p);         // 来自 F1
//     template<class T> X(UniquePtr<T>); // 来自 F2
// };
```

#### 3. 重载决议

使用初始化表达式对虚构类类型进行重载决议，选择的构造函数对应的推导指引的返回类型即为推导结果。

### Explicit 规则

推导指引的 explicit 规则：

```cpp
template<class T>
struct A
{
    explicit A(const T&, ...); // #1 显式构造函数
    A(T&&, ...);               // #2 非显式构造函数
};

int i;
A a1 = {i, i}; // 错误：#1 显式且不能用于复制初始化
A a2{i, i};    // OK：#1 推导为 A<int>
A a3{0, i};    // OK：#2 推导为 A<int>
A a4 = {0, i}; // OK：#2 推导为 A<int>
```

### 转发引用与类模板参数

重要区别：类模板参数的右值引用**不是**转发引用：

```cpp
template<class T>
struct A
{
    template<class U>
    A(T&&, U&&, int*); // T&& 不是转发引用，U&& 是转发引用

    A(T&&, int*);      // T&& 不是转发引用
};

template<class T>
A(T&&, int*) -> A<T>;  // T&& 是转发引用！

int i, *ip;
A a{i, 0, ip};  // 错误：无法从 #1 推导
A a0{0, 0, ip}; // OK：#1 推导为 A<int>
A a2{i, ip};    // OK：用户指引 #3 推导为 A<int&>
```

### 复制 vs 包装

当从单一参数初始化且参数类型是类模板的特化时，复制语义优先：

```cpp
std::tuple t1{1};  // std::tuple<int>
std::tuple t2{t1}; // std::tuple<int>，不是 std::tuple<std::tuple<int>>

std::vector v1{1, 2};   // std::vector<int>
std::vector v2{v1};     // std::vector<int>，不是 std::vector<std::vector<int>>
std::vector v3{v1, v2}; // std::vector<std::vector<int>>
```

## 5. 使用场景 (Use Cases)

### 适合使用 CTAD 的场景

| 场景 | 示例 |
|------|------|
| 标准库容器初始化 | `std::vector v{1, 2, 3};` |
| 智能指针创建 | `std::unique_ptr p{new int(5)};` |
| 元组和配对 | `std::pair p{1, 2.0};` |
| 锁守卫 | `std::lock_guard lg(mutex);` |

### 需要用户自定义推导指引的场景

1. **迭代器构造容器**：

```cpp
template<class Iter>
container(Iter b, Iter e)
    -> container<typename std::iterator_traits<Iter>::value_type>;
```

2. **类型转换需求**：

```cpp
template<class T>
struct S
{
    S(T);
};
S(const char*) -> S<std::string>; // C 字符串转为 std::string

S s{"hello"}; // 推导为 S<std::string>
```

3. **聚合类型 (C++20 前)**：

```cpp
template<class A, class B>
struct Agg
{
    A a;
    B b;
};

// C++17 需要手动添加推导指引
template<class A, class B>
Agg(A a, B b) -> Agg<A, B>;

Agg agg{1, 2.0}; // 推导为 Agg<int, double>
```

### 注意事项

#### 1. 模板参数列表存在时不进行推导

```cpp
std::tuple t1(1, 2, 3);                // OK：进行推导
std::tuple<int, int, int> t2(1, 2, 3); // OK：显式指定

std::tuple<> t3(1, 2, 3);    // 错误：tuple<> 无匹配构造函数
std::tuple<int> t4(1, 2, 3);  // 错误：参数过多
```

#### 2. 注入类名不触发推导

在类模板内部，模板名（不带参数列表）是注入类名，不触发推导：

```cpp
template<class T>
struct X
{
    X(T) {}

    template<class Iter>
    auto foo(Iter b, Iter e)
    {
        return X(b, e); // 不进行推导，X 是当前 X<T>
    }

    auto baz()
    {
        return ::X(0); // 进行推导，结果为 X<int>
    }
};
```

#### 3. 重载决议顺序

当多个候选匹配时，优先级为：

1. 用户自定义推导指引 > 隐式生成的指引
2. 复制推导候选 > 其他隐式指引
3. 非模板构造函数生成的指引 > 模板构造函数生成的指引

```cpp
template<class T>
struct A
{
    using value_type = T;
    A(value_type);     // #1 非模板构造函数
    A(const A&);       // #2 复制构造
    A(T, T, int);      // #3 非模板构造函数
    template<class U>
    A(int, T, U);      // #4 模板构造函数
};

A x(1, 2, 3); // 使用 #3

template<class T>
A(T) -> A<T>; // #6 用户指引

A a(42); // 使用 #6 推导为 A<int>，#1 初始化
A b = a; // 使用复制候选 #5 推导为 A<int>，#2 初始化
```

### 常见陷阱

#### 陷阱 1：initializer_list 构造函数

```cpp
std::vector v1{1, 2};            // std::vector<int>
std::vector v2(v1.begin(), v1.end());  // std::vector<int>
std::vector v3{v1.begin(), v1.end()};  // std::vector<std::vector<int>::iterator>
                                        // 注意：使用 initializer_list 构造函数！
```

#### 陷阱 2：C++20 前的聚合类型

```cpp
// C++17：聚合类型没有隐式推导指引
template<class T>
struct Point { T x, y; };
Point p{1, 2}; // 错误！C++17 无法推导

// 需要手动添加推导指引
template<class T>
Point(T, T) -> Point<T>;

// C++20：自动支持聚合类型推导
```

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <utility>
#include <tuple>
#include <memory>
#include <mutex>

int main()
{
    // pair 推导
    std::pair p(2, 4.5);     // std::pair<int, double>
    std::pair p2{1, 2};      // std::pair<int, int>

    // tuple 推导
    std::tuple t(4, 3, 2.5); // std::tuple<int, int, double>

    // 智能指针
    std::unique_ptr up(new int(42));    // std::unique_ptr<int>
    std::shared_ptr sp = std::make_shared<int>(10);

    // 锁守卫
    std::mutex mtx;
    std::lock_guard lg(mtx);  // std::lock_guard<std::mutex>

    return 0;
}
```

### 用户自定义推导指引

```cpp
#include <iterator>
#include <vector>
#include <string>

// 自定义容器模板
template<class T>
struct Container
{
    Container(T t) {}
    template<class Iter>
    Container(Iter beg, Iter end) {}
};

// 从迭代器推导元素类型的推导指引
template<class Iter>
Container(Iter b, Iter e)
    -> Container<typename std::iterator_traits<Iter>::value_type>;

// 字符串特化推导指引
Container(const char*) -> Container<std::string>;

int main()
{
    Container c1(42);  // Container<int>

    std::vector<double> v = {1.0, 2.0, 3.0};
    Container c2(v.begin(), v.end());  // Container<double>

    Container c3("hello");  // Container<std::string>，而非 Container<const char*>

    return 0;
}
```

### 复杂示例：嵌套模板推导

```cpp
#include <iostream>

template<class T>
struct Outer
{
    template<class U>
    struct Inner
    {
        Inner(T);
        Inner(T, U);

        template<class V>
        Inner(V, U);
    };
};

int main()
{
    // T 已知为 int（来自 Outer<int>）
    // 推导 U
    Outer<int>::Inner x{2.0, 1};

    // 推导过程：
    // F1: template<class U> Outer<int>::Inner<U> F(int);
    // F2: template<class U> Outer<int>::Inner<U> F(int, U);
    // F3: template<class U, class V> Outer<int>::Inner<U> F(V, U);
    // F4: template<class U> Outer<int>::Inner<U> F(Outer<int>::Inner<U>); // 复制候选

    // 重载决议选择 F3，U=int, V=double
    // 结果：Outer<int>::Inner<int>

    return 0;
}
```

### C++20 聚合类型推导

```cpp
// C++20 起自动支持
template<class A, class B>
struct Agg
{
    A a;
    B b;
};

Agg agg{1, 2.0};  // C++20: 自动推导为 Agg<int, double>

// 复杂聚合类型
template<class T>
struct ComplexAgg
{
    T t;
    struct
    {
        long a, b;
    } u;
};

ComplexAgg ca{1, 2, 3};
// 推导为 ComplexAgg<int>
// 等价的推导候选：
// template<class T>
// ComplexAgg<T> F(T, long, long);
```

### 常见错误及修正

#### 错误 1：期望推导但不提供足够信息

```cpp
// 错误：无法推导 T
template<class T>
struct Wrapper
{
    Wrapper() {} // 无参数，无法推导
};

Wrapper w; // 错误！无默认推导

// 修正：提供默认推导指引
template<class T = int>
Wrapper() -> Wrapper<T>;

Wrapper w; // OK：推导为 Wrapper<int>
```

#### 错误 2：错误理解复制语义

```cpp
// 错误预期：认为会"包装"现有对象
std::tuple t1{1};
std::tuple t2{t1};  // 实际是 std::tuple<int>，不是 std::tuple<std::tuple<int>>

// 修正：显式指定类型或使用额外参数
std::tuple<std::tuple<int>> t3{t1};  // 显式类型
std::tuple t4{t1, std::tuple<int>{}}; // 多参数避免复制语义
```

#### 错误 3：忽略 explicit 限制

```cpp
template<class T>
struct Strict
{
    explicit Strict(T) {}
};

Strict s1 = 42;  // 错误：explicit 构造函数不能用于复制初始化
Strict s2{42};   // OK：直接初始化
```

#### 错误 4：依赖不可推导的上下文

```cpp
template<class T>
struct A
{
    using Ptr = T*;
    A(Ptr); // Ptr 是别名，但这不影响推导
};

// 隐式推导指引是：
// template<class T> A<T> F(T*); 而非 A(typename A<T>::Ptr)
// 所以可以正常推导

A a{(int*)nullptr}; // OK：推导为 A<int>

// 但如果使用嵌套类型：
template<class T>
struct B
{
    struct Nested { T value; };
    B(Nested); // 无法推导 T
};

B b{B<int>::Nested{42}}; // 错误：无法从 Nested 推导 T
```

## 7. 总结 (Summary)

类模板参数推导 (CTAD) 是 C++ 现代化的重要特性，它解决了类模板与函数模板参数推导的不对称性问题。

### 核心要点

| 要点 | 说明 |
|------|------|
| **自动推导** | 从构造函数参数自动推导模板参数 |
| **隐式指引** | 编译器自动从构造函数生成推导指引 |
| **用户指引** | 可自定义推导规则处理特殊场景 |
| **复制优先** | 从同类型对象初始化时，复制语义优先 |
| **C++20 增强** | 支持聚合类型和别名模板推导 |

### 与传统方式对比

```cpp
// 传统方式 (C++14)
auto p1 = std::make_pair(1, 2.0);
auto t1 = std::make_tuple(1, 2, 3.0);
std::lock_guard<std::mutex> lg1(mtx);

// CTAD 方式 (C++17)
std::pair p2(1, 2.0);
std::tuple t2(1, 2, 3.0);
std::lock_guard lg2(mtx);
```

### 最佳实践

1. **优先使用 CTAD**：简化代码，提高可读性
2. **需要特殊推导逻辑时使用用户指引**：如迭代器到容器元素类型
3. **注意 explicit 语义**：复制初始化时 explicit 构造函数不参与
4. **注意 initializer_list 构造函数**：可能影响推导结果
5. **C++20 可直接对聚合类型使用 CTAD**

### 相关概念

| 概念 | 关系 |
|------|------|
| 模板参数推导 | 函数模板参数推导是 CTAD 的基础 |
| 工厂函数模式 | `make_*` 函数是 CTAD 出现前的替代方案 |
| 聚合类型 | C++20 支持聚合类型的 CTAD |
| 别名模板 | C++20 支持从别名模板推导 |

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
- C++17 标准: [class.ctad]
- P0091R3: Template argument deduction for class templates
- P0702R1: Making CTAD work for aggregates