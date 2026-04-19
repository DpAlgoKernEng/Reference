# 模板参数与模板实参 (Template Parameters and Template Arguments)

## 1. 概述 (Overview)

模板参数 (template parameter) 是 C++ 模板机制的核心组成部分。每个模板都由一个或多个模板参数进行参数化，这些参数在模板声明的参数列表中指定。模板参数决定了模板的通用性和灵活性。

模板参数分为三种类型：
- **非类型模板参数 (non-type template parameter)**：以值作为参数，如整数、指针、引用等
- **类型模板参数 (type template parameter)**：以类型作为参数，使用 `typename` 或 `class` 关键字声明
- **模板模板参数 (template template parameter)**：以模板作为参数

模板实参 (template argument) 是在实例化模板时提供的具体值或类型，用于替换模板参数。对于类模板，实参可以显式提供、从初始化器推导 (C++17 起) 或使用默认值。对于函数模板，实参可以显式提供、从上下文推导或使用默认值。

## 2. 来源与演变 (Origin and Evolution)

### C++98 首次引入

模板参数机制在 **C++98** 标准中首次引入，作为泛型编程的核心基础：
- 支持类型模板参数和非类型模板参数
- 支持模板模板参数
- 确立了基本的模板实参匹配规则

### C++11 增强

- 引入**变参模板 (variadic template)**，支持模板参数包
- 新增 `std::nullptr_t` 作为非类型模板参数类型
- 允许在函数模板中使用默认模板参数
- 允许列表初始化作为非类型模板实参

### C++17 重大扩展

- 引入 `auto` 占位符类型用于非类型模板参数推导
- 放宽模板模板实参匹配规则 (P0522R0)
- 允许所有非类型模板实参进行常量求值
- 特性测试宏：`__cpp_nontype_template_parameter_auto`、`__cpp_template_template_args`、`__cpp_nontype_template_args`

### C++20 全面扩展

- 非类型模板参数支持**浮点类型**
- 非类型模板参数支持**字面类类型 (literal class type)**
- 引入**类型约束 (type constraint)**，使用概念 (concept) 约束类型参数
- 支持**推导类类型占位符**作为非类型模板参数
- 引入**模板参数对象 (template parameter object)** 概念
- 放宽对子对象地址的限制

### C++26 持续改进

- 对非类型模板参数初始化语义的进一步规范

## 3. 语法与参数 (Syntax and Parameters)

### 模板声明基本语法

```cpp
// 基本形式
template <parameter-list> declaration;

// C++20: 带约束的模板声明
template <parameter-list> requires constraint declaration;
```

### 非类型模板参数语法

```cpp
// 基本形式
type name;                          // 非类型模板参数
type name = default;                // 带默认值
type ... name;                      // 参数包 (C++11 起)
```

#### 允许的结构类型 (Structural Type)

非类型模板参数的类型必须是结构类型，包括：

| 类型分类 | 具体类型 |
|---------|---------|
| 基础类型 | 整型、枚举类型、浮点类型 (C++20 起) |
| 指针类型 | 对象指针、函数指针、成员指针 |
| 引用类型 | 左值引用 (对象或函数) |
| 特殊类型 | `std::nullptr_t` (C++11 起) |
| 字面类类型 | C++20 起，需满足特定条件 |

#### 字面类类型要求 (C++20 起)

```cpp
// 字面类类型必须满足：
// 1. 所有基类和非静态数据成员为 public 且非 mutable
// 2. 所有基类和非静态数据成员的类型也是结构类型
struct StructuralType {
    int x;
    double y;
    // 满足字面类类型要求
};
```

### 类型模板参数语法

```cpp
type-parameter-key name;                    // 无默认值
type-parameter-key name = default;          // 带默认值
type-parameter-key ... name;                // 参数包 (C++11 起)
type-constraint name;                       // 受约束的类型参数 (C++20 起)
type-constraint name = default;             // 受约束且带默认值 (C++20 起)
type-constraint ... name;                  // 受约束的参数包 (C++20 起)
```

其中 `type-parameter-key` 为 `typename` 或 `class`，两者在类型模板参数声明中没有区别。

```cpp
// 类型模板参数示例
template<class T>                        // 无默认值
class My_vector { /* ... */ };

template<class T = void>                  // 带默认值
struct My_op_functor { /* ... */ };

template<typename... Ts>                  // 参数包
class My_tuple { /* ... */ };

template<My_concept T>                    // 受约束的类型参数 (C++20)
class My_constrained_vector { /* ... */ };
```

### 模板模板参数语法

```cpp
template <parameter-list> type-parameter-key name;           // 无默认值
template <parameter-list> type-parameter-key name = default; // 带默认值
template <parameter-list> type-parameter-key ... name;       // 参数包 (C++11 起)
```

其中 `type-parameter-key` 为 `class` 或 `typename` (C++17 起)。

```cpp
template<typename T>
class my_array {};

// 两个类型模板参数和一个模板模板参数
template<typename K, typename V, template<typename> typename C = my_array>
class Map {
    C<K> key;
    C<V> value;
};
```

### 参数命名规则

模板参数名在其作用域内不能重复声明，也不能与模板名相同：

```cpp
template<class T, int N>
class Y {
    int T;      // 错误: 模板参数重复声明
    void f() {
        char T; // 错误: 模板参数重复声明
    }
};

template<class X>
class X; // 错误: 模板参数与模板名重复
```

### 默认模板参数

默认模板参数在参数列表中用 `=` 指定：

```cpp
// 默认参数合并规则
template<typename T1, typename T2 = int> class A;
template<typename T1 = int, typename T2> class A;

// 等价于
template<typename T1 = int, typename T2 = int> class A;
```

默认参数的限制：
- 类模板、变量模板 (C++14 起)、别名模板的主模板声明中，如果有默认参数，后续参数也必须有默认值（最后一个可以是参数包）
- 不能在同一作用域内为同一参数指定两次默认值
- 不能在类模板成员的外部定义中指定默认参数
- 不能在友元类模板声明中指定默认参数

## 4. 底层原理 (Underlying Principles)

### 模板实例化机制

模板实例化是编译器根据模板实参生成具体代码的过程：

1. **类型检查**：验证实参与参数声明是否兼容
2. **参数替换**：将实参替换模板参数
3. **代码生成**：生成具体化的类或函数

### 模板实参确定规则

#### 非类型模板实参

非类型模板实参的值根据参数类型 `T` 和实参 `A` 确定：

| 版本 | 规则 |
|------|------|
| C++98 | `A` 必须是 `T` 类型的转换常量表达式 |
| C++11 起 | `A` 可以是表达式或花括号初始化列表 |
| C++20 起 | 类类型参数引入模板参数对象概念 |

```cpp
template<int i>
struct C { /* ... */ };

C<{42}> c1; // C++11 起: 使用花括号初始化列表
```

#### 类型模板实参

类型模板实参必须是类型标识符 (type-id)，可以命名不完整类型：

```cpp
template<typename T>
class X {};

struct A;            // 不完整类型
X<A> x1;             // OK: 'A' 命名一个类型
X<A*> x2;            // OK: 'A*' 命名一个类型
```

#### 模板模板实参匹配

模板模板实参 `A` 匹配模板模板参数 `P` 的条件：`P` 必须至少与 `A` 一样特化。

```cpp
template<class T> class A { /* ... */ };
template<class T, class U = T> class B { /* ... */ };
template<class... Types> class C { /* ... */ };

template<template<class> class P> class X { /* ... */ };
X<A> xa; // OK
X<B> xb; // C++17 起 OK (P0522R0)
X<C> xc; // C++17 起 OK (P0522R0)
```

### 模板实参等价性

两个值**模板实参等价 (template-argument-equivalent)** 当它们：

| 条件 | 说明 |
|------|------|
| 整型/枚举类型 | 值相同 |
| 指针类型 | 指针值相同 |
| 成员指针类型 | 指向同一成员或都为空 |
| 左值引用类型 | 引用同一对象或函数 |
| `std::nullptr_t` | C++11 起 |
| 浮点类型 | C++20 起，值相同 |
| 数组类型 | C++20 起，对应元素等价 |
| 联合体类型 | C++20 起，活动成员相同且等价 |
| 字面类类型 | C++20 起，对应子对象和引用成员等价 |

### 约束表达式生成 (C++20 起)

受约束的类型参数 `P` 的约束为 `Q`（命名概念 `C`）时，生成约束表达式 `E` 的规则：

- 若 `Q` 为 `C`（无参数列表）：
  - 非参数包：`E` 为 `C<P>`
  - 参数包：`E` 为 `(C<P> && ...)`
- 若 `Q` 为 `C<A1,A2...,AN>`：
  - 非参数包：`E` 为 `C<P,A1,A2,...AN>`
  - 参数包：`E` 为 `(C<P,A1,A2,...AN> && ...)`

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 推荐参数类型 | 说明 |
|------|-------------|------|
| 容器类元素类型 | 类型模板参数 | `std::vector<T>` |
| 编译期常量 | 非类型模板参数 | `std::array<T, N>` |
| 策略类选择 | 模板模板参数 | 自定义容器分配器 |
| 泛型算法 | 类型模板参数 + 约束 | C++20 概念约束 |
| 元编程 | 非类型模板参数 | 编译期计算 |

### 最佳实践

#### 1. 使用 auto 推导非类型参数 (C++17 起)

```cpp
template<auto n>
struct B { /* ... */ };

B<5> b1;     // OK: 类型推导为 int
B<'a'> b2;   // OK: 类型推导为 char
B<2.5> b3;   // C++20 起 OK: 类型推导为 double
```

#### 2. 使用概念约束类型参数 (C++20 起)

```cpp
template<std::integral T>   // 使用概念约束
T add(T a, T b) {
    return a + b;
}
```

#### 3. 参数包与折叠表达式配合

```cpp
template<typename... Ts>
auto sum(Ts... args) {
    return (args + ...);  // 折叠表达式
}
```

### 注意事项

#### 1. 字符串字面量不能作为模板实参

```cpp
template<const char* p>
class X {};

X<"fail"> x1;  // 错误: 字符串字面量不能用作模板实参

char okay[] = "okay";  // 具有链接的静态对象
X<okay> x2;     // OK
```

#### 2. 临时对象、数组元素地址等限制

```cpp
template<int* p>
class Y {};

int a[10];
struct S { int m; static int s; } s;

Y<&a[2]> y1;   // C++20 前错误: 数组元素地址
Y<&s.m> y2;    // C++20 前错误: 非静态成员地址
Y<&s.s> y3;    // OK: 静态成员地址
Y<&S::s> y4;   // OK: 静态成员地址
```

#### 3. 类型歧义解析

当实参既能解释为类型标识符又能解释为表达式时，总是解释为类型：

```cpp
template<class T> void f(); // #1
template<int I> void f();   // #2

void g() {
    f<int()>(); // "int()" 既是类型也是表达式
                // 调用 #1，因为被解释为类型
}
```

### 常见陷阱

#### 陷阱 1：模板参数隐藏

```cpp
struct A { struct B {}; int C; int Y; };

template<class B, class C>
struct X : A {
    B b; // A::B，而非模板参数 B
    C c; // 错误: A::C 不是类型名
};
```

#### 陷阱 2：成员定义中的名称查找

```cpp
namespace N {
    class C {};
    template<class T> class B { void f(T); };
}

template<class C>
void N::B<C>::f(C) {
    C b; // C 是模板参数，而非 N::C
}
```

#### 陷阱 3：模板模板参数默认参数作用域

```cpp
template<typename T = float>
struct B {};

template<template<typename = float> typename T>
struct A {
    void f();
};

template<template<typename TT> class T>
void A<T>::f() {
    T<> t; // 错误: TT 在此作用域无默认值
}
```

## 6. 代码示例 (Examples)

### 基础用法

#### 非类型模板参数

```cpp
#include <array>
#include <iostream>
#include <numeric>

// 简单的非类型模板参数
template<int N>
struct S { int a[N]; };

template<const char*>
struct S2 {};

int main() {
    S<10> s;        // s.a 是一个包含 10 个 int 的数组
    s.a[9] = 4;

    char okay[] = "okay";  // 具有链接的静态对象
    S2<okay> s4;           // OK

    return 0;
}
```

#### 类型模板参数

```cpp
#include <vector>
#include <string>

// 带默认值的类型模板参数
template<typename T = int, typename Allocator = std::allocator<T>>
class Container {
    std::vector<T, Allocator> data;
public:
    void add(const T& value) { data.push_back(value); }
};

int main() {
    Container<> c1;           // Container<int, std::allocator<int>>
    Container<std::string> c2; // Container<std::string, std::allocator<std::string>>
    return 0;
}
```

#### 模板模板参数

```cpp
#include <vector>
#include <list>

template<typename T, template<typename> class Container>
class Stack {
    Container<T> data;
public:
    void push(const T& value) { data.push_back(value); }
    void pop() { data.pop_back(); }
};

int main() {
    Stack<int, std::vector> s1;  // 使用 vector 作为底层容器
    Stack<int, std::list> s2;    // 使用 list 作为底层容器
    return 0;
}
```

### 高级用法

#### C++20 字面类类型非类型参数

```cpp
#include <array>
#include <numeric>

// C++20: 字面类类型作为非类型模板参数
template<std::array arr>
constexpr auto sum() {
    return std::accumulate(arr.cbegin(), arr.cend(), 0);
}

int main() {
    // C++20: 类模板实参推导
    static_assert(sum<std::array<double, 8>{3, 1, 4, 1, 5, 9, 2, 6}>() == 31.0);

    // C++20: NTTP 实参推导和 CTAD
    static_assert(sum<std::array{2, 7, 1, 8, 2, 8}>() == 28);

    return 0;
}
```

#### C++20 受约束的类型参数

```cpp
#include <concepts>
#include <iostream>

// 定义概念
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

// 使用概念约束
template<Numeric T>
T compute(T a, T b) {
    return a + b;
}

// 受约束的参数包
template<Addable... Ts>
auto sum_all(Ts... args) {
    return (args + ...);
}

int main() {
    std::cout << compute(1, 2) << std::endl;      // OK: int
    std::cout << compute(1.5, 2.5) << std::endl;  // OK: double
    // compute("a", "b");  // 错误: 不满足 Numeric 概念

    std::cout << sum_all(1, 2, 3, 4, 5) << std::endl;  // 输出: 15
    return 0;
}
```

#### 复杂的非类型模板参数

```cpp
// 复杂的非类型模板参数示例
template
<
    char c,             // 整型
    int (&ra)[5],       // 对象的左值引用（数组类型）
    int (*pf)(int),     // 函数指针
    int (S<10>::*a)[10] // 成员对象指针
>
struct Complicated {
    void foo(char base) {
        ra[4] = pf(c - base);
    }
};

int a[5];
int f(int n) { return n; }

int main() {
    S<10> s;
    Complicated<'2', a, f, &S<10>::a> c;
    c.foo('0');
    return 0;
}
```

### 常见错误及修正

#### 错误 1：参数类型不匹配

```cpp
// 错误: 浮点类型作为非类型参数 (C++20 前)
template<double d>  // C++20 前错误
struct FloatParam {};

// 修正: C++20 起支持浮点类型
template<double d>  // C++20 起 OK
struct FloatParamFixed {};

// 或使用 constexpr 变量作为替代方案
struct FloatWrapper {
    static constexpr double value = 3.14;
};
```

#### 错误 2：模板模板参数匹配失败

```cpp
template<typename T1> struct A;
template<typename T1, typename T2> struct B;

template<template<typename> class P>
struct X {};

X<A> xa;  // OK
X<B> xb;  // C++17 前错误: 参数列表不完全匹配
          // C++17 起 OK: P 比 B 更特化

// 修正: 使用变参模板模板参数
template<template<typename...> class P>
struct Y {};

Y<A> ya;  // OK
Y<B> yb;  // OK
```

#### 错误 3：模板参数对象初始化失败 (C++20)

```cpp
template<auto n>
struct B { /* ... */ };

struct J1 {
    J1* self = this;  // 非常量初始化
};

B<J1{}> j1;  // 错误: 模板参数对象初始化不是常量表达式

// 修正: 提供常量初始化
struct J2 {
    J2* self = this;
    constexpr J2() {}
    constexpr J2(const J2&) {}
};

B<J2{}> j2;  // 仍可能错误: 模板参数对象与临时对象不等价
```

## 7. 总结 (Summary)

### 核心要点

| 模板参数类型 | 用途 | 关键特性 |
|-------------|------|---------|
| 非类型模板参数 | 编译期常量 | C++20 起支持浮点类型和字面类类型 |
| 类型模板参数 | 泛型类型 | 使用 `typename` 或 `class` 声明 |
| 模板模板参数 | 模板作为参数 | 匹配规则：参数至少与实参一样特化 |

### 版本特性对照

| 特性 | 引入版本 |
|------|---------|
| 变参模板 (参数包) | C++11 |
| `auto` 非类型参数推导 | C++17 |
| 浮点类型非类型参数 | C++20 |
| 字面类类型非类型参数 | C++20 |
| 概念约束的类型参数 | C++20 |
| 模板参数对象 | C++20 |

### 学习建议

1. **从类型模板参数开始**：这是最常用的模板参数类型，理解其工作原理是掌握模板的基础
2. **逐步学习非类型参数**：从整型参数开始，再学习 C++20 的扩展特性
3. **理解模板模板参数匹配规则**：特别是 C++17 的 P0522R0 放宽规则
4. **掌握 C++20 概念约束**：概念为模板参数提供了清晰的约束机制

### 相关概念

| 概念 | 关系 |
|------|------|
| 模板实例化 | 使用模板实参替换模板参数生成具体代码 |
| 模板实参推导 | 编译器自动确定模板实参的过程 |
| SFINAE | 替换失败不是错误，与模板参数密切相关 |
| 概念 (Concept) | C++20 起对类型模板参数的约束机制 |

### 特性测试宏

| 宏名 | 值 | 标准 | 特性 |
|------|-----|------|------|
| `__cpp_nontype_template_parameter_auto` | 201606L | C++17 | 使用 `auto` 声明非类型模板参数 |
| `__cpp_template_template_args` | 201611L | C++17 (DR) | 模板模板实参匹配 |
| `__cpp_nontype_template_args` | 201411L | C++17 | 允许所有非类型模板实参加常量求值 |
| `__cpp_nontype_template_args` | 201911L | C++20 | 类类型和浮点类型作为非类型模板参数 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/template_parameters
- C++ Standard: [temp.param]
- P0522R0: DR 150 - Matching of template template-arguments
- C++20 Standard: Concepts and constraints