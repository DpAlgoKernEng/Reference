# 模板偏特化 (Partial Template Specialization)

## 1. 概述

模板偏特化（Partial Template Specialization）是 C++ 模板系统的重要特性，允许为类模板和变量模板（C++14 起）的特定参数类别提供定制化实现。偏特化介于主模板（primary template）和完全特化（full specialization）之间，提供了一种更灵活的模板定制方式。

与完全特化（为所有模板参数提供具体类型）不同，偏特化只需要部分参数固定或对参数施加特定约束，从而为某一类模板参数提供优化的实现。标准库中 `std::unique_ptr` 就是一个典型例子，它对数组类型有专门的偏特化版本。

## 2. 来源与演变

### 首次引入

模板偏特化首次在 **C++98** 标准中引入，是 C++ 模板元编程的基础设施之一。

### 设计动机

在偏特化出现之前，开发者面临以下问题：

1. **完全特化过于严格**：完全特化要求为所有模板参数指定具体类型，无法处理一类相关类型
2. **代码重复**：相似但不同的类型需要各自写完全特化版本
3. **无法针对特征优化**：无法针对指针类型、引用类型等类别提供优化实现

偏特化的引入解决了这些问题，提供了：
- 针对类型类别的优化能力
- 减少代码重复
- 支持更灵活的模板元编程技术

### C++11 变化

- 明确偏特化必须比主模板更特化（more specialized）
- 修复了参数包相关的规范模糊问题

### C++14 变化

- 新增对变量模板（Variable Template）偏特化的支持

### 缺陷报告修正

| DR | 适用版本 | 原行为 | 修正后行为 |
|----|---------|--------|-----------|
| CWG 727 | C++98 | 偏特化和完全特化不允许在类作用域内声明 | 允许在任意作用域声明 |
| CWG 1315 | C++98 | 非类型模板参数只能是标识表达式 | 只要可推导，表达式也可 |
| CWG 1495 | C++11 | 参数包相关规范不清晰 | 偏特化必须更特化 |
| CWG 1711 | C++14 | 缺少变量模板偏特化规范 | 添加变量模板支持 |
| CWG 1819 | C++98 | 偏特化定义的作用域规范不清 | 可与主模板在同一作用域声明 |
| CWG 2330 | C++14 | 缺少变量模板引用 | 添加变量模板支持 |

## 3. 语法与参数

### 基本语法

**类模板偏特化**

```cpp
template<parameter-list>
class-key class-head-name<argument-list> declaration;
```

**变量模板偏特化（C++14 起）**

```cpp
template<parameter-list>
decl-specifier-seq declarator<argument-list> initializer(optional);
```

其中：
- `class-head-name` 标识已声明的类模板名称
- `declarator` 标识已声明的变量模板名称（C++14 起）

### 参数说明

| 参数类型 | 说明 |
|---------|------|
| `parameter-list` | 模板参数列表，可以与主模板不同 |
| `argument-list` | 模板实参列表，指定偏特化的约束条件 |
| `class-key` | `class`、`struct` 或 `union` |

### 偏特化声明示例

```cpp
// 主模板
template<class T1, class T2, int I>
class A {};

// 偏特化 #1：T2 是 T1 的指针
template<class T, int I>
class A<T, T*, I> {};

// 偏特化 #2：T1 是指针
template<class T, class T2, int I>
class A<T*, T2, I> {};

// 偏特化 #3：T1 是 int，I 是 5，T2 是指针
template<class T>
class A<int, T*, 5> {};

// 偏特化 #4：T2 是指针
template<class X, class T, int I>
class A<X, T*, I> {};
```

### 实参列表限制

偏特化的实参列表必须遵守以下规则：

1. **不能与主模板相同**：实参列表必须有所特化

```cpp
template<class T1, class T2, int I> class B {};        // 主模板
template<class X, class Y, int N> class B<X, Y, N> {}; // 错误：未特化任何内容
```

2. **必须比主模板更特化**（C++11 起）

```cpp
template<int N, typename T1, typename... Ts> struct B;
template<typename... Ts> struct B<0, Ts...> {}; // 错误：不比主模板更特化
```

3. **默认参数不能出现在实参列表中**

4. **包扩展必须是最后一个参数**

5. **非类型实参表达式可以使用模板参数**，前提是参数至少在非推导上下文之外出现一次

```cpp
template<int I, int J> struct A {};
template<int I> struct A<I + 5, I * 2> {}; // 错误：I 不可推导

template<int I, int J, int K> struct B {};
template<int I> struct B<I, I * 2, 2> {};   // 正确：第一个参数可推导
```

6. **非类型模板实参不能特化依赖特化参数的模板参数**

```cpp
template<class T, T t> struct C {}; // 主模板
template<class T> struct C<T, 1>;   // 错误：实参 1 的类型是 T，依赖于参数 T

template<int X, int (*array_ptr)[X]> class B {}; // 主模板
int array[5];
template<int X> class B<X, &array> {}; // 错误：&array 的类型是 int(*)[X]，依赖于参数 X
```

## 4. 底层原理

### 名称查找机制

偏特化不直接参与名称查找。只有当主模板通过名称查找被发现后，才会考虑其偏特化版本：

```cpp
namespace N {
    template<class T1, class T2> class Z {}; // 主模板
}
using N::Z; // 引入主模板

namespace N {
    template<class T> class Z<T, T*> {}; // 偏特化
}
Z<int, int*> z; // 名称查找找到 N::Z（主模板），然后使用偏特化 T = int
```

### 偏序规则（Partial Ordering）

当实例化类模板或变量模板时，如果存在多个偏特化匹配，编译器使用偏序规则确定哪个更特化：

1. 如果只有一个特化匹配，使用该特化
2. 如果多个特化匹配，使用偏序规则确定最特化的那个
3. 如果没有特化匹配，使用主模板

**偏序判断方法**：将每个偏特化转换为虚构函数模板，然后按函数模板重载规则排序：

```cpp
template<int I, int J, class T> struct X {}; // 主模板

template<int I, int J>
struct X<I, J, int> { static const int s = 1; }; // 偏特化 #1
// 虚构函数模板：template<int I, int J> void f(X<I, J, int>);

template<int I>
struct X<I, I, int> { static const int s = 2; }; // 偏特化 #2
// 虚构函数模板：template<int I> void f(X<I, I, int>);

int main() {
    X<2, 2, int> x; // #1 和 #2 都匹配
    // 偏序判断：
    // #A 从 #B：void(X<I, J, int>) 从 void(X<U1, U1, int>)：推导成功
    // #B 从 #A：void(X<I, I, int>) 从 void(X<U1, U2, int>)：推导失败
    // #B 更特化，使用偏特化 #2
    std::cout << x.s << '\n'; // 输出 2
}
```

### 成员处理规则

偏特化的成员与主模板的成员**完全独立**：

1. 偏特化的成员必须与偏特化的参数列表匹配
2. 偏特化成员只需在使用时定义
3. 主模板的成员不会自动继承到偏特化中

```cpp
template<class T, int I>
struct A {
    void f(); // 主模板成员声明
};

template<class T, int I>
void A<T, I>::f() {} // 主模板成员定义

template<class T>
struct A<T, 2> {
    void f();
    void g();
    void h();
};

template<class T>
void A<T, 2>::g() {} // 偏特化成员定义

template<>
void A<char, 2>::h() {} // 偏特化成员的完全特化

int main() {
    A<char, 0> a0;
    A<char, 2> a2;
    a0.f(); // 正确：使用主模板成员定义
    a2.g(); // 正确：使用偏特化成员定义
    a2.h(); // 正确：使用偏特化成员的完全特化
    a2.f(); // 错误：偏特化 A<T,2> 没有 f() 的定义（不使用主模板）
}
```

## 5. 使用场景

### 适合使用偏特化的场景

| 场景 | 说明 |
|------|------|
| 指针类型优化 | 为指针类型提供特殊实现，如 `std::unique_ptr<T[]>` |
| 类型特征实现 | 实现类型萃取（Type Traits），如 `is_pointer` |
| 减少代码重复 | 为一类相关类型提供统一实现 |
| 性能优化 | 为特定类型类别提供优化版本 |

### 偏特化 vs 完全特化

| 特性 | 偏特化 | 完全特化 |
|------|--------|---------|
| 参数约束 | 部分参数或类型类别 | 所有参数具体化 |
| 灵活性 | 更灵活，可处理类型类别 | 只能处理具体类型 |
| 代码复用 | 更好，一类类型共享实现 | 每个具体类型需要单独实现 |

### 最佳实践

1. **优先使用偏特化**：当需要为某一类类型提供定制实现时，偏特化比多个完全特化更简洁
2. **注意偏序规则**：确保偏特化之间有明确的偏序关系，避免歧义
3. **成员独立性**：偏特化的成员与主模板无关，需要重新定义所需成员
4. **作用域规则**：偏特化可以在不同于主模板的作用域中声明

### 常见陷阱

1. **歧义偏特化**：两个偏特化匹配但无法确定哪个更特化

```cpp
template<class T, class T2, int I>
class A<T*, T2, I> {}; // 偏特化 #2

template<class X, class T, int I>
class A<X, T*, I> {};   // 偏特化 #4

A<int*, int*, 2> a; // 错误：#2 和 #4 都匹配，但都无法确定更特化
```

2. **忘记偏特化成员定义**：偏特化不会继承主模板的成员

3. **无效的偏特化参数**：实参列表未实际特化任何内容

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

// 主模板：通用实现
template<typename T>
class Container {
public:
    void print() { std::cout << "Generic container\n"; }
};

// 偏特化：指针类型
template<typename T>
class Container<T*> {
public:
    void print() { std::cout << "Pointer container\n"; }
};

// 偏特化：数组类型
template<typename T, std::size_t N>
class Container<T[N]> {
public:
    void print() { std::cout << "Array container of size " << N << "\n"; }
};

int main() {
    Container<int> c1;
    c1.print();  // 输出：Generic container

    Container<int*> c2;
    c2.print();  // 输出：Pointer container

    Container<int[10]> c3;
    c3.print();  // 输出：Array container of size 10

    return 0;
}
```

### 高级用法：类型特征

```cpp
#include <iostream>
#include <type_traits>

// 移除指针的主模板
template<typename T>
struct RemovePointer {
    using type = T;
};

// 偏特化：处理指针类型
template<typename T>
struct RemovePointer<T*> {
    using type = T;
};

// 偏特化：处理 const 指针类型
template<typename T>
struct RemovePointer<T* const> {
    using type = T;
};

// 偏特化：处理 volatile 指针类型
template<typename T>
struct RemovePointer<T* volatile> {
    using type = T;
};

// 偏特化：处理 const volatile 指针类型
template<typename T>
struct RemovePointer<T* const volatile> {
    using type = T;
};

int main() {
    static_assert(std::is_same_v<RemovePointer<int*>::type, int>);
    static_assert(std::is_same_v<RemovePointer<int* const>::type, int>);
    static_assert(std::is_same_v<RemovePointer<int>::type, int>);

    std::cout << "All type traits tests passed!\n";
    return 0;
}
```

### 高级用法：智能指针数组特化

```cpp
#include <iostream>
#include <memory>

// 简化版 unique_ptr 演示
template<typename T>
class UniquePtr {
public:
    UniquePtr(T* p = nullptr) : ptr(p) {}
    ~UniquePtr() { delete ptr; }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

private:
    T* ptr;
};

// 偏特化：数组类型
template<typename T>
class UniquePtr<T[]> {
public:
    UniquePtr(T* p = nullptr) : ptr(p) {}
    ~UniquePtr() { delete[] ptr; }  // 使用 delete[]

    T& operator[](std::size_t i) const { return ptr[i]; }
    T* get() const { return ptr; }

private:
    T* ptr;
};

int main() {
    // 单个对象
    UniquePtr<int> p1(new int(42));
    std::cout << *p1 << "\n";  // 输出：42

    // 数组
    UniquePtr<int[]> p2(new int[3]{1, 2, 3});
    std::cout << p2[0] << ", " << p2[1] << ", " << p2[2] << "\n";  // 输出：1, 2, 3

    return 0;
}
```

### 常见错误及修正

#### 错误 1：歧义偏特化

```cpp
// ❌ 错误：两个偏特化都无法确定更特化
template<class T1, class T2>
class Pair {};

template<class T>
class Pair<T, T> {};   // 偏特化 #1

template<class T>
class Pair<T*, T*> {}; // 偏特化 #2

// Pair<int*, int*> p;  // 错误：#1 和 #2 都匹配（当 T = int*），无法确定

// ✅ 修正：设计时确保偏序关系明确
template<class T1, class T2>
class Pair {};

template<class T>
class Pair<T, T> {};      // 两个相同类型

template<class T>
class Pair<T*, T*> {};    // 两个相同类型的指针

template<class T1, class T2>
class Pair<T1*, T2*> {};   // 两个不同类型的指针
```

#### 错误 2：忘记定义偏特化成员

```cpp
// ❌ 错误：偏特化没有定义所有需要的成员
template<typename T>
class Processor {
public:
    void process() { std::cout << "Processing generic\n"; }
    void cleanup() { std::cout << "Generic cleanup\n"; }
};

template<typename T>
class Processor<T*> {
public:
    void process() { std::cout << "Processing pointer\n"; }
    // 忘记定义 cleanup()
};

// Processor<int*> p;
// p.cleanup();  // 链接错误：未定义

// ✅ 修正：偏特化定义所有需要的成员
template<typename T>
class Processor<T*> {
public:
    void process() { std::cout << "Processing pointer\n"; }
    void cleanup() { std::cout << "Pointer cleanup\n"; }
};
```

#### 错误 3：无效的偏特化

```cpp
// ❌ 错误：偏特化实参列表与主模板相同
template<typename T, int N>
class Array {};

template<typename U, int M>
class Array<U, M> {};  // 错误：未特化任何内容

// ✅ 修正：实际特化某些参数
template<typename T>
class Array<T, 0> {};  // 特化 N = 0 的情况

template<int N>
class Array<int, N> {}; // 特化 T = int 的情况
```

## 7. 总结

### 核心要点

模板偏特化是 C++ 模板系统的重要组成部分，主要特点包括：

1. **灵活性**：介于主模板和完全特化之间，支持为类型类别提供定制实现
2. **偏序规则**：编译器自动选择最特化的版本
3. **成员独立性**：偏特化的成员与主模板无关，需要单独定义

### 偏特化 vs 完全特化 vs 主模板

| 特性 | 主模板 | 偏特化 | 完全特化 |
|------|--------|--------|---------|
| 模板参数 | 全部泛型 | 部分泛型 | 全部具体 |
| 适用范围 | 最广 | 特定类别 | 最窄 |
| 实现数量 | 一个 | 可多个 | 每种类型一个 |
| 优先级 | 最低 | 中等 | 最高 |

### 使用建议

1. 当需要为指针、引用、数组等类型类别提供优化实现时，使用偏特化
2. 确保多个偏特化之间有明确的偏序关系
3. 偏特化的成员需要独立定义，不继承自主模板
4. 偏特化可以在任意允许主模板定义的作用域中声明

### 相关概念

| 概念 | 关系 |
|------|------|
| 类模板（Class Template） | 偏特化的主体 |
| 完全特化（Full Specialization） | 更严格的特化形式 |
| 函数模板（Function Template） | 不支持偏特化，但支持重载 |
| 类型特征（Type Traits） | 偏特化的典型应用场景 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/partial_specialization
- C++ Standard: [temp.class.spec]
- C++ Templates: The Complete Guide, David Vandevoorde et al.