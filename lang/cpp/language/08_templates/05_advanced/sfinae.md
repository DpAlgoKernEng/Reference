# SFINAE

"Substitution Failure Is Not An Error"（替换失败不是错误）

## 1. 概述 (Overview)

SFINAE 是 C++ 模板元编程中的一个核心规则，全称为 **Substitution Failure Is Not An Error**（替换失败不是错误）。该规则适用于函数模板的重载解析过程：当用显式指定或推导的类型替换模板参数失败时，该特化版本会被从重载集中移除，而不是导致编译错误。

这一特性使得模板元编程成为可能，允许程序员根据类型特征在编译期选择不同的函数重载或类特化版本。SFINAE 是 C++ 类型萃取（type traits）、概念约束等高级模板技术的基础。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

SFINAE 规则最早出现在 C++98 标准中，是模板实例化机制的自然产物。最初的设计动机是为了解决模板重载解析时的健壮性问题——当某个模板特化对特定类型无效时，编译器应该继续尝试其他重载，而不是直接报错。

### 版本演变

| 版本 | 变更内容 |
|------|----------|
| C++98 | 基础 SFINAE 规则确立；仅用于类型中的常量表达式（如数组边界）需要作为 SFINAE 处理 |
| C++11 | 扩展为表达式 SFINAE；引入 `std::enable_if` 库支持；增加包展开长度不匹配的 SFINAE 错误类型 |
| C++14 | 支持变量模板偏特化中的 SFINAE |
| C++17 | 引入 `std::void_t` 简化偏特化 SFINAE 应用；引入 `if constexpr` 作为替代方案 |
| C++20 | explicit specifier 中的表达式也参与 SFINAE；Lambda 表达式不被视为立即上下文的一部分；引入概念（Concepts）作为更优雅的替代方案 |

### 缺陷报告

| DR | 适用版本 | 原行为 | 修正行为 |
|----|----------|--------|----------|
| CWG 295 | C++98 | 创建 cv 限定的函数类型可能导致替换失败 | 不再是失败，丢弃 cv 限定符 |
| CWG 1227 | C++98 | 替换顺序未指定 | 与词法顺序一致 |
| CWG 2054 | C++98 | 偏特化中的替换未正确指定 | 已正确指定 |
| CWG 2322 | C++11 | 不同词法顺序的声明可能导致模板实例化以不同顺序发生或不发生 | 此类情况为病态，无需诊断 |

## 3. 语法与参数 (Syntax and Parameters)

### 替换发生的位置

函数模板参数的替换分两个阶段进行：
1. **显式指定的模板参数**：在模板参数推导之前替换
2. **推导的参数和默认参数**：在模板参数推导之后替换

替换发生在以下位置：
- 函数类型中使用的所有类型（包括返回类型和所有参数的类型）
- 模板参数声明中使用的所有类型
- 偏特化的模板参数列表中使用的所有类型
- （C++11 起）函数类型中使用的所有表达式
- （C++11 起）模板参数声明中使用的所有表达式
- （C++11 起）偏特化模板参数列表中使用的所有表达式
- （C++20 起）explicit specifier 中使用的所有表达式

### 替换失败的定义

**替换失败**是指在使用替换后的参数编写上述类型或表达式时，会导致程序病态（需要诊断）的任何情况。

### 立即上下文 (Immediate Context)

只有发生在函数类型或其模板参数类型或 explicit specifier（C++20 起）的**立即上下文**中的失败才是 SFINAE 错误。

如果替换的类型/表达式的求值导致副作用（如某个模板特化的实例化、隐式定义成员函数的生成等），这些副作用中的错误被视为**硬错误**（hard error），会导致编译失败。

**注意**：Lambda 表达式不被视为立即上下文的一部分（C++20 起）。

### 类型 SFINAE 错误

以下类型错误是 SFINAE 错误：

| 错误类型 | 版本 |
|----------|------|
| 尝试实例化包含多个不同长度包的包展开 | C++11 起 |
| 创建 void 数组、引用数组、函数数组、负大小数组、非整数大小数组或零大小数组 | 所有版本 |
| 在作用域解析运算符 `::` 左侧使用非类或枚举类型 | 所有版本 |
| 尝试使用类型的成员，但类型不包含该成员，或成员类型不匹配 | 所有版本 |
| 创建指向引用的指针 | 所有版本 |
| 创建 void 的引用 | 所有版本 |
| 创建成员指针，但 T 不是类类型 | 所有版本 |
| 为非类型模板参数提供无效类型 | 所有版本 |
| 在模板参数表达式或函数声明表达式中执行无效转换 | 所有版本 |
| 创建带有 void 参数的函数类型 | 所有版本 |
| 创建返回数组类型或函数类型的函数类型 | 所有版本 |

### 表达式 SFINAE 错误（C++11 起）

以下表达式错误是 SFINAE 错误：
- 模板参数类型中使用的病态表达式
- 函数类型中使用的病态表达式

## 4. 底层原理 (Underlying Principles)

### 替换机制

替换按**词法顺序**进行，遇到失败时停止。

```cpp
template<typename A>
struct B { using type = typename A::type; };

template<
    class T,
    class U = typename T::type,    // 如果 T 没有成员 type，SFINAE 失败
    class V = typename B<T>::type> // 如果 B 没有成员 type，硬错误
                                   // （CWG 1227 保证不会发生，因为 U 的替换先失败）
void foo(int);
```

### 词法顺序与重声明

如果存在具有不同词法顺序的多个声明（例如，一个函数模板声明带有后置返回类型——在参数之后替换——并重新声明为普通返回类型——在参数之前替换），并且这会导致模板实例化以不同顺序发生或不发生，则程序是病态的，无需诊断。

```cpp
template<class T>
typename T::type h(typename B<T>::type);

template<class T>
auto h(typename B<T>::type) -> typename T::type; // 重声明

template<class T>
void h(...) {}

using R = decltype(h<int>(0));     // 病态，无需诊断
```

### 偏特化中的 SFINAE

在确定类模板或变量模板（C++14 起）的特化是否由某个偏特化或主模板生成时，也会进行推导和替换。在这种确定过程中，替换失败不会被当作硬错误，而是使相应的偏特化声明被忽略，就像函数模板重载解析中一样。

```cpp
// 主模板处理不可引用的类型
template<class T, class = void>
struct reference_traits
{
    using add_lref = T;
    using add_rref = T;
};

// 偏特化识别可引用的类型
template<class T>
struct reference_traits<T, std::void_t<T&>>
{
    using add_lref = T&;
    using add_rref = T&&;
};
```

## 5. 使用场景 (Use Cases)

### 适用场景

1. **类型萃取（Type Traits）实现**：基于类型特征在编译期选择不同实现
2. **条件编译**：根据编译期条件启用或禁用特定函数重载
3. **模板偏特化选择**：根据类型特征选择合适的类模板偏特化
4. **函数重载解析控制**：精细控制哪些重载版本参与候选集

### 标准库支持

| 组件 | 版本 | 用途 |
|------|------|------|
| `std::enable_if` | C++11 起 | 根据编译期条件创建替换失败，启用或禁用特定重载 |
| `std::void_t` | C++17 起 | 简化偏特化 SFINAE 应用的工具元函数 |

### 替代方案

| 方案 | 版本 | 说明 |
|------|------|------|
| 标签分发（Tag Dispatch） | 所有版本 | 使用标签类型选择重载 |
| `if constexpr` | C++17 起 | 编译期条件分支，更清晰 |
| 概念（Concepts） | C++20 起 | 更优雅、更易读的约束语法 |
| `static_assert` | C++11 起 | 仅需要条件编译错误时使用 |

### 最佳实践

1. **优先使用 Concepts**：C++20 起应优先使用概念而非 SFINAE
2. **使用标准库工具**：使用 `std::enable_if`、`std::void_t` 等标准工具而非手写实现
3. **注意立即上下文**：确保 SFINAE 错误发生在立即上下文中，避免硬错误
4. **保持替换顺序一致**：避免重声明导致词法顺序不一致

### 常见陷阱

1. **硬错误**：副作用（如模板实例化）中的错误会导致编译失败，不是 SFINAE
2. **词法顺序不一致**：重声明可能导致病态程序（无需诊断）
3. **Lambda 不在立即上下文**：C++20 起 Lambda 表达式中的错误是硬错误

## 6. 代码示例 (Examples)

### 基础用法：数组大小判断

使用数组大小作为 SFINAE 条件选择不同重载：

```cpp
#include <iostream>

// 当 I 为偶数时选择此重载
template<int I>
void div(char(*)[I % 2 == 0] = nullptr)
{
    std::cout << I << " is even\n";
}

// 当 I 为奇数时选择此重载
template<int I>
void div(char(*)[I % 2 == 1] = nullptr)
{
    std::cout << I << " is odd\n";
}

int main()
{
    div<4>();  // 输出: 4 is even
    div<7>();  // 输出: 7 is odd
}
```

### 类型成员检测

检测类型是否包含特定成员：

```cpp
#include <iostream>

template<class T>
int f(typename T::B*) { return 1; }  // #1: 需要 T 有成员 B

template<class T>
int f(T) { return 2; }              // #2: 兜底版本

struct A {};          // 没有成员 B
struct B { int B; };  // 有成员 B，但不是类型

int main()
{
    std::cout << f<int>(0) << "\n";  // 输出: 2（#1 替换失败，选择 #2）
    std::cout << f<A>(0) << "\n";    // 输出: 2（#1 替换失败，选择 #2）
}
```

### 类类型检测

使用成员指针检测类型是否为类：

```cpp
#include <iostream>

template<typename T>
class is_class
{
    typedef char yes[1];
    typedef char no[2];

    template<typename C>
    static yes& test(int C::*);  // C 是类类型时选择

    template<typename C>
    static no& test(...);        // 其他情况选择

public:
    static bool const value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

struct MyClass {};
enum MyEnum {};

int main()
{
    std::cout << std::boolalpha;
    std::cout << "is_class<MyClass>::value = " << is_class<MyClass>::value << "\n";  // true
    std::cout << "is_class<int>::value = " << is_class<int>::value << "\n";          // false
    std::cout << "is_class<MyEnum>::value = " << is_class<MyEnum>::value << "\n";    // false
}
```

### 表达式 SFINAE（C++11 起）

使用后置返回类型和 `decltype` 检测表达式有效性：

```cpp
#include <iostream>

struct X {};
struct Y { Y(X){} };  // X 可转换为 Y

// #1: 需要 T 支持加法
template<class T>
auto f(T t1, T t2) -> decltype(t1 + t2);

// #2: 接受 Y 参数的重载
X f(Y, Y);

int main()
{
    X x1, x2;
    X x3 = f(x1, x2);  // #1 推导失败（x1 + x2 病态）
                        // 只有 #2 在重载集中，被调用
    std::cout << "f(x1, x2) called\n";
}
```

### 成员函数指针检测

使用逗号运算符检测成员函数是否存在并调用：

```cpp
#include <iostream>

// #1: C 是类或类引用，F 是 C 的成员函数指针
template<class C, class F>
auto test(C c, F f) -> decltype((void)(c.*f)(), void())
{
    std::cout << "(1) Class/class reference overload called\n";
}

// #2: C 是类指针，F 是 C 的成员函数指针
template<class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void())
{
    std::cout << "(2) Pointer overload called\n";
}

// #3: 兜底版本（省略号参数排名最低）
void test(...)
{
    std::cout << "(3) Catch-all overload called\n";
}

int main()
{
    struct X { void f() {} };
    X x;
    X& rx = x;
    test(x, &X::f);   // (1)
    test(rx, &X::f);  // (1)，创建 x 的副本
    test(&x, &X::f);  // (2)
    test(42, 1337);   // (3)
}
```

**输出**：
```
(1) Class/class reference overload called
(1) Class/class reference overload called
(2) Pointer overload called
(3) Catch-all overload called
```

### 使用 std::enable_if（C++11 起）

条件启用函数重载：

```cpp
#include <iostream>
#include <type_traits>

// 仅当 T 是整数类型时启用
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
process(T value)
{
    std::cout << "Processing integer: " << value << "\n";
    return value * 2;
}

// 仅当 T 是浮点类型时启用
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
process(T value)
{
    std::cout << "Processing floating point: " << value << "\n";
    return value * 2.0;
}

int main()
{
    process(42);       // 调用整数版本
    process(3.14);    // 调用浮点版本
    // process("hello"); // 编译错误：无匹配重载
}
```

### 使用 std::void_t 实现类型萃取（C++17 起）

```cpp
#include <iostream>
#include <type_traits>

// 检测类型是否有 size() 成员函数
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

// 检测类型是否有 value_type 成员类型
template<typename T, typename = void>
struct has_value_type : std::false_type {};

template<typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

#include <vector>
#include <string>

int main()
{
    std::cout << std::boolalpha;
    std::cout << "has_size<std::vector<int>>: " << has_size<std::vector<int>>::value << "\n";  // true
    std::cout << "has_size<int>: " << has_size<int>::value << "\n";                            // false
    std::cout << "has_value_type<std::string>: " << has_value_type<std::string>::value << "\n"; // true
    std::cout << "has_value_type<double>: " << has_value_type<double>::value << "\n";          // false
}
```

### 常见错误：硬错误

以下代码演示 SFINAE 失败与硬错误的区别：

```cpp
// 错误示例：硬错误
template<typename T>
struct Bad { using type = typename T::nonexistent; };

template<typename T>
void func(typename Bad<T>::type) {}  // 实例化 Bad<int> 是副作用

int main()
{
    // func<int>(0);  // 硬错误！Bad<int> 实例化失败
}
```

**修正方法**：将检查放在立即上下文中：

```cpp
// 正确做法：SFINAE 在立即上下文中
template<typename T>
void func(typename T::nonexistent*) {}  // T::nonexistent 在立即上下文中

int main()
{
    // func<int>(nullptr);  // SFINAE 失败，编译通过（如果有其他重载）
}
```

## 7. 总结 (Summary)

### 核心要点

1. **SFINAE 定义**：模板参数替换失败不是编译错误，而是将该特化从候选集中移除
2. **适用范围**：仅限函数类型、模板参数类型、explicit specifier 的**立即上下文**中的失败
3. **词法顺序**：替换按词法顺序进行，遇到失败即停止
4. **库支持**：`std::enable_if`（C++11）、`std::void_t`（C++17）简化 SFINAE 使用

### 技术对比

| 特性 | SFINAE | if constexpr | Concepts |
|------|--------|--------------|----------|
| 版本要求 | C++98 | C++17 | C++20 |
| 可读性 | 较差 | 较好 | 优秀 |
| 编译错误信息 | 晦涩 | 中等 | 清晰 |
| 适用场景 | 所有版本模板 | 函数体内分支 | 函数签名约束 |
| 学习曲线 | 陡峭 | 平缓 | 平缓 |

### 学习建议

1. **理解立即上下文**：这是区分 SFINAE 错误和硬错误的关键
2. **掌握标准库工具**：优先使用 `std::enable_if`、`std::void_t` 和类型萃取
3. **升级到 Concepts**：C++20 起应优先使用概念，代码更清晰、错误信息更友好
4. **阅读标准库实现**：`<type_traits>` 头文件是学习 SFINAE 的最佳范例