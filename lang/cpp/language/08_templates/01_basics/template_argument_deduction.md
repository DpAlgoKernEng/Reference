# 模板参数推导 (Template Argument Deduction)

## 1. 概述 (Overview)

模板参数推导 (Template Argument Deduction) 是 C++ 编译器的一项核心机制，用于自动确定函数模板实例化所需的模板参数。在实例化函数模板时，每个模板参数都必须已知，但并非每个模板参数都需要显式指定。当可能时，编译器会从函数参数中推导缺失的模板参数。

### 核心概念

- **推导时机**：模板参数推导发生在函数模板名称查找（可能涉及参数依赖查找 ADL）之后、模板参数替换（可能涉及 SFINAE）和重载解析之前
- **适用范围**：函数调用、取函数模板地址、运算符表达式等多种上下文
- **推导对象**：类型模板参数 `T`、模板模板参数 `TT`、非类型模板参数 `I`

### 技术定位

模板参数推导是泛型编程的基础设施，使得模板的使用更加简洁和自然。通过推导机制，开发者无需在每个调用点显式指定所有模板参数，大大提高了代码的可读性和易用性。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

模板参数推导自 C++98 标准引入，最初仅支持函数模板的基本推导规则。随着语言的发展，推导机制不断扩展和完善。

### 版本演进

| 版本 | 主要变更 |
|------|----------|
| C++98 | 引入基本的模板参数推导机制，支持从函数调用推导类型和非类型参数 |
| C++11 | 引入 `auto` 类型推导、初始化列表推导、转发引用（Forwarding Reference）推导、参数包展开推导 |
| C++14 | 支持函数返回类型的 `auto` 推导 |
| C++17 | 支持类模板参数推导（CTAD）、函数指针转换参与推导、`noexcept` 说明符参与推导 |
| C++20 | 别名模板在类模板参数推导中可被推导 |
| C++26 | 包索引说明符（Pack Indexing Specifier）作为非推导上下文 |

### 解决的问题

- **简化调用语法**：避免在调用点重复书写冗长的模板参数列表
- **支持运算符模板**：运算符无法像函数那样显式指定模板参数，推导机制使其成为可能
- **实现完美转发**：通过转发引用推导，实现参数的完美转发语义

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
template<typename To, typename From>
To convert(From f);

void g(double d) {
    int i = convert<int>(d);    // 调用 convert<int, double>(double)
    char c = convert<char>(d);  // 调用 convert<char, double>(double)
    int(*ptr)(float) = convert; // 实例化 convert<int, float>(float)
                                // 并将其地址存储在 ptr 中
}
```

### 推导前的类型调整

在推导开始前，对参数类型 `P` 和实参类型 `A` 进行以下调整：

1. **非引用参数 `P`**：
   - 若 `A` 是数组类型，则转换为指针类型（数组到指针转换）
   - 若 `A` 是函数类型，则转换为函数指针类型（函数到指针转换）
   - 若 `A` 有顶层 cv 限定符，则忽略这些限定符

```cpp
template<class T>
void f(T);

int a[3];
f(a); // P = T, A = int[3], 调整为 int*: 推导 T = int*

void b(int);
f(b); // P = T, A = void(int), 调整为 void(*)(int): 推导 T = void(*)(int)

const int c = 13;
f(c); // P = T, A = const int, 调整为 int: 推导 T = int
```

2. **cv 限定的 `P`**：忽略顶层 cv 限定符

3. **引用类型的 `P`**：使用被引用的类型进行推导

4. **转发引用**：若 `P` 是对无 cv 限定的模板参数的右值引用（转发引用），且对应的函数调用实参是左值，则使用对 `A` 的左值引用类型代替 `A` 进行推导

```cpp
template<class T>
int f(T&&);       // P 是对无 cv 限定的 T 的右值引用（转发引用）

template<class T>
int g(const T&&); // P 是对有 cv 限定的 T 的右值引用（非转发引用）

int main() {
    int i;
    int n1 = f(i); // 实参是左值：调用 f<int&>(int&)（特殊规则）
    int n2 = f(0); // 实参非左值：调用 f<int>(int&&)
    // int n3 = g(i); // 错误：推导为 g<int>(const int&&)
                     // 无法将右值引用绑定到左值
}
```

### 从类型推导

给定依赖于模板参数的函数参数 `P` 和对应的实参 `A`，若 `P` 具有以下形式之一，则进行推导：

| 形式 | 说明 |
|------|------|
| `cv`(可选) `T` | 带可选 cv 限定的类型 |
| `T*` | 指向 T 的指针 |
| `T&` | 对 T 的左值引用 |
| `T&&` | 对 T 的右值引用（C++11 起） |
| `T`(可选) `[I`(可选)`]` | 数组类型 |
| `T`(可选) `(U`(可选)`)` | 函数类型（C++17 前） |
| `T`(可选) `(U`(可选)`) noexcept(I`(可选)`)` | 带 noexcept 的函数类型（C++17 起） |
| `T`(可选) `U`(可选)`::*` | 成员指针 |
| `TT`(可选)`<T>` | 模板特化 |
| `TT`(可选)`<I>` | 非类型模板参数的模板特化 |
| `TT`(可选)`<TU>` | 模板模板参数 |

### 非推导上下文 (Non-deduced Contexts)

在以下情况下，类型不参与模板参数推导：

1. **限定标识符的嵌套名称说明符**：使用限定标识符指定的类型中，`::` 左边的部分

```cpp
template<typename T>
struct identity { typedef T type; };

template<typename T>
void good(std::vector<T> x, typename identity<T>::type value = 1);
// T 在 identity<T>::type 中是非推导上下文，使用其他地方推导的 T
```

2. **包索引说明符或包索引表达式**（C++26 起）

3. **decltype 说明符中的表达式**

4. **非类型模板参数或数组边界中的子表达式**：引用模板参数的子表达式

```cpp
template<std::size_t N>
void f(std::array<int, 2 * N> a); // 2 * N 是非推导上下文
```

5. **使用默认参数的函数参数**：模板参数用于具有默认参数且该默认参数被使用的函数参数类型

6. **重载函数集**：`A` 是函数或重载集，且有多个匹配或不匹配的情况

7. **花括号初始化列表**：`P` 不是 `std::initializer_list`、其引用或数组引用

8. **非末尾的参数包**：参数包未出现在参数列表末尾

9. **模板参数列表中的包展开**：未出现在模板参数列表末尾

10. **数组类型的主要边界**：对于数组类型（非数组引用或数组指针）

### 其他上下文中的推导

| 上下文 | 说明 |
|--------|------|
| `auto` 类型推导 | 变量声明时从初始化器推导 `auto` 的含义 |
| `auto` 返回类型 | 函数声明时从 return 语句推导返回类型 |
| 重载解析 | 从候选模板函数生成特化时进行推导 |
| 重载集地址 | 取包含函数模板的重载集地址时进行推导 |
| 部分排序 | 对重载函数模板进行部分排序时进行推导 |
| 转换函数模板 | 选择用户定义转换函数模板参数时进行推导 |
| 显式实例化 | 确定特化引用的模板时进行推导 |
| 释放函数模板 | 确定是否匹配放置 new 时进行推导 |

## 4. 底层原理 (Underlying Principles)

### 推导机制

模板参数推导的核心是参数-实参匹配：编译器尝试找到模板参数，使得将推导出的模板参数代入参数类型 `P` 后得到的"推导型"与转换后的实参类型 `A` 相同。

### 推导过程

1. **类型调整**：对 `P` 和 `A` 应用前述调整规则
2. **模式匹配**：递归地将 `P` 的结构分解，与 `A` 进行匹配
3. **参数收集**：收集每个模板参数的推导结果
4. **一致性检查**：确保同一模板参数在不同位置的推导结果一致
5. **失败处理**：若推导失败或结果不一致，编译失败

### 替代推导规则

若常规推导失败，还考虑以下替代方案：

1. **cv 限定放宽**：若 `P` 是引用类型，推导型可以比转换后的 `A` 有更多的 cv 限定

```cpp
template<typename T>
void f(const T& t);

bool a = false;
f(a); // P = const T&, 推导 T = bool, 推导型 = const bool
      // 推导型比 A 有更多 cv 限定
```

2. **限定转换**：转换后的 `A` 可以通过限定转换或函数指针转换（C++17 起）转换为推导型

```cpp
template<typename T>
void f(const T*);

int* p;
f(p); // P = const T*, 推导 T = int, 推导型 = const int*
      // 应用限定转换（从 int* 到 const int*）
```

3. **派生类匹配**：若 `P` 是类且具有简单模板 ID 形式，转换后的 `A` 可以是推导型的派生类

```cpp
template<class T>
struct B {};

template<class T>
struct D : public B<T> {};

template<class T>
void f(B<T>&) {}

void f() {
    D<int> d;
    f(d); // P = B<T>&, 推导 T = int, 推导型 = B<int>
          // A 是 D<int>，派生自 B<int>
}
```

### 初始化列表推导（C++11 起）

若移除引用和 cv 限定后的 `P` 为 `std::initializer_list<P'>` 且 `A` 是花括号初始化列表，则对初始化列表的每个元素进行推导：

```cpp
template<class T>
void f(std::initializer_list<T>);

f({1, 2, 3});  // 推导 T = int
f({1, "abc"}); // 错误：推导失败，T 歧义（int vs const char*）
```

### 性能特征

- **编译时计算**：推导完全在编译时完成，无运行时开销
- **复杂度**：推导复杂度与类型结构的复杂度相关，通常为线性或多项式复杂度
- **错误诊断**：推导失败通常产生详细的编译错误信息

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 简化函数调用

无需显式指定所有模板参数，使代码更简洁：

```cpp
template<typename T>
void print(const T& value);

print(42);        // 推导 T = int
print(3.14);      // 推导 T = double
print("hello");   // 推导 T = const char[6]
```

#### 2. 运算符模板

运算符无法显式指定模板参数，必须依赖推导：

```cpp
std::cout << "Hello, world" << std::endl;
// operator<< 通过 ADL 查找为 std::operator<<
// 推导为 operator<<<char, std::char_traits<char>>
// std::endl 推导为 &std::endl<char, std::char_traits<char>>
```

#### 3. 完美转发

通过转发引用实现参数的完美转发：

```cpp
template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));
}

int x = 42;
wrapper(x);       // T 推导为 int&，arg 类型为 int&
wrapper(42);      // T 推导为 int，arg 类型为 int&&
```

#### 4. 类模板参数推导（C++17 起）

构造对象时自动推导类模板参数：

```cpp
std::pair p(2, 4.5);     // 推导 std::pair<int, double>
std::tuple t(4, 3, 2.5); // 推导 std::tuple<int, int, double>
std::lock_guard lck(mtx); // 推导 std::lock_guard<std::mutex>
```

### 最佳实践

#### 1. 避免歧义推导

当多个参数推导同一模板参数时，确保类型一致：

```cpp
template<typename T>
void bad(std::vector<T> x, T value = 1);

std::vector<std::complex<double>> x;
bad(x, 1.2);  // 错误：T 同时推导为 std::complex<double> 和 double
```

使用非推导上下文避免歧义：

```cpp
template<typename T>
void good(std::vector<T> x, typename identity<T>::type value = 1);
// 第二个参数中的 T 是非推导上下文
```

#### 2. 正确使用转发引用

区分转发引用和普通右值引用：

```cpp
template<class T>
int f(T&&);       // 转发引用：T&& 其中 T 是模板参数

template<class T>
int g(const T&&); // 非转发引用：有 cv 限定
```

#### 3. 理解非推导上下文

合理利用非推导上下文控制推导行为：

```cpp
// 使用 type_identity 阻止推导
template<typename T>
void func(std::type_identity_t<T> arg);

func(42);       // 错误：T 在非推导上下文中
func<int>(42);  // 正确：显式指定
```

### 常见陷阱

#### 1. 数组类型退化

```cpp
template<class T>
void f(T);

int a[3];
f(a); // T 推导为 int*，而非 int[3]
```

#### 2. 顶层 cv 限定符被忽略

```cpp
template<class T>
void f(T);

const int c = 13;
f(c); // T 推导为 int，而非 const int
```

#### 3. 初始化列表不匹配

```cpp
template<class T>
void g1(std::vector<T>);

g1({1, 2, 3}); // 错误：无法从花括号初始化列表推导 std::vector<T>
```

#### 4. 重载函数歧义

```cpp
template<typename T>
int f(T(*p)(T));

int g(int);
int g(char);

f(g); // 正确：只有一个重载匹配
```

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>
#include <vector>

// 基本模板参数推导
template<typename T>
void print_type(T value) {
    std::cout << "Type: " << typeid(T).name() << ", Value: " << value << std::endl;
}

// 多参数推导
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

int main() {
    // 单参数推导
    print_type(42);       // T = int
    print_type(3.14);     // T = double
    print_type("hello");  // T = const char*

    // 多参数推导
    auto result1 = add(1, 2.5);     // T = int, U = double
    auto result2 = add(1.0f, 2);    // T = float, U = int

    std::cout << "Result 1: " << result1 << std::endl;
    std::cout << "Result 2: " << result2 << std::endl;

    return 0;
}
```

### 转发引用与完美转发

```cpp
#include <iostream>
#include <utility>
#include <string>

void process(int& x) {
    std::cout << "Lvalue reference: " << x << std::endl;
}

void process(int&& x) {
    std::cout << "Rvalue reference: " << x << std::endl;
}

void process(const std::string& s) {
    std::cout << "Lvalue string: " << s << std::endl;
}

void process(std::string&& s) {
    std::cout << "Rvalue string: " << s << std::endl;
}

// 完美转发模板
template<typename T>
void wrapper(T&& arg) {
    // 转发引用：根据实参推导 T
    // 左值实参 -> T = Type&，arg 类型 = Type&
    // 右值实参 -> T = Type，arg 类型 = Type&&
    process(std::forward<T>(arg));
}

int main() {
    int x = 42;
    std::string s = "hello";

    wrapper(x);              // 左值 int：T = int&
    wrapper(100);           // 右值 int：T = int
    wrapper(s);             // 左值 string：T = std::string&
    wrapper(std::string("world")); // 右值 string：T = std::string

    return 0;
}
```

### 类模板参数推导（C++17）

```cpp
#include <iostream>
#include <utility>
#include <tuple>
#include <memory>

// 自定义类模板
template<typename T>
struct Container {
    T value;

    Container(T v) : value(std::move(v)) {}

    void print() const {
        std::cout << "Container value: " << value << std::endl;
    }
};

// 推导指引（Deduction Guide）
template<typename T>
Container(T) -> Container<T>;

int main() {
    // 标准库示例
    std::pair p(1, 2.5);           // std::pair<int, double>
    std::tuple t(1, 2.0, 'a');     // std::tuple<int, double, char>

    std::cout << "Pair: (" << p.first << ", " << p.second << ")" << std::endl;

    // 自定义类模板推导
    Container c1(42);              // Container<int>
    Container c2(3.14);            // Container<double>
    Container c3(std::string("hello")); // Container<std::string>

    c1.print();
    c2.print();
    c3.print();

    return 0;
}
```

### 非推导上下文示例

```cpp
#include <iostream>
#include <vector>
#include <type_traits>

// 问题：歧义推导
template<typename T>
void bad_insert(std::vector<T>& vec, T value) {
    vec.push_back(value);
}

// 解决方案 1：使用 type_identity（C++20 起标准库提供）
template<typename T>
struct type_identity {
    using type = T;
};

template<typename T>
using type_identity_t = typename type_identity<T>::type;

template<typename T>
void good_insert1(std::vector<T>& vec, type_identity_t<T> value) {
    vec.push_back(value);
}

// 解决方案 2：分开指定
template<typename T, typename U>
void good_insert2(std::vector<T>& vec, U value) {
    vec.push_back(value);
}

int main() {
    std::vector<double> vec;

    // bad_insert(vec, 42); // 错误：T 歧义（double vs int）

    good_insert1(vec, 42);    // 正确：T 从 vec 推导为 double
    good_insert2(vec, 42);    // 正确：T = double, U = int

    std::cout << "Vector size: " << vec.size() << std::endl;
    std::cout << "Elements: " << vec[0] << ", " << vec[1] << std::endl;

    return 0;
}
```

### auto 类型推导

```cpp
#include <iostream>
#include <initializer_list>

int main() {
    // 变量 auto 推导
    auto x = 42;              // int
    auto y = 3.14;            // double
    const auto& ref = x;      // const int&

    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "ref = " << ref << std::endl;

    // 花括号初始化列表
    auto list1 = {1, 2, 3};   // std::initializer_list<int>
    auto list2 = {3.14};      // std::initializer_list<double>

    // C++17 直接列表初始化（单元素）
    // auto a{1, 2};          // 错误：多于一个元素
    // auto b{3};             // C++17 起：int（非 initializer_list）

    // 函数返回类型推导（C++14）
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << "add(1, 2) = " << add(1, 2) << std::endl;
    std::cout << "add(1.5, 2.5) = " << add(1.5, 2.5) << std::endl;

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>
#include <vector>
#include <array>

// 错误 1：数组边界推导问题
template<int N>
void f2(int a[N][20]);    // P = int[N][20]，N 是非推导上下文

template<int N>
void f3(int (&a)[N][20]); // P = int(&)[N][20]，N 可推导

// 错误 2：非类型参数类型不匹配
template<short s>
void process_array(int arr[s]) {} // 注意：这实际上是指针，不是数组

template<int i>
class A {};

template<short s>
void func(A<s>);

// 错误 3：顶层 const 被忽略
template<typename T>
void modify(T& value) {
    // T 推导时忽略顶层 const
}

int main() {
    int arr[10][20];

    // f2(arr);    // 错误：N 无法推导
    f2<10>(arr);   // 正确：显式指定
    f3(arr);        // 正确：N 推导为 10

    // A<1> a;     // 类型为 A<1>，模板参数类型为 int
    // func(a);    // 错误：类型不匹配（int vs short）
    func<1>(A<1>{}); // 正确：显式指定

    const int x = 10;
    int y = 20;
    modify(y);      // T = int，value 类型为 int&

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 主题 | 要点 |
|------|------|
| **推导时机** | 函数调用、取函数地址、重载解析、显式实例化等多种上下文 |
| **类型调整** | 数组到指针转换、函数到指针转换、忽略顶层 cv 限定符 |
| **转发引用** | `T&&` 形式，左值推导为 `T&`，右值推导为 `T` |
| **非推导上下文** | 嵌套名称说明符、decltype 表达式、非类型参数表达式等 |
| **替代规则** | cv 限定放宽、限定转换、派生类匹配 |

### 技术对比

| 特性 | C++98 | C++11 | C++14 | C++17 | C++20 |
|------|-------|-------|-------|-------|-------|
| 基本推导 | 支持 | 支持 | 支持 | 支持 | 支持 |
| `auto` 推导 | - | 支持 | 支持 | 支持 | 支持 |
| 转发引用 | - | 支持 | 支持 | 支持 | 支持 |
| 返回类型推导 | - | - | 支持 | 支持 | 支持 |
| 类模板推导 | - | - | - | 支持 | 支持 |
| 别名模板推导 | - | - | - | - | 有限支持 |

### 学习建议

1. **理解核心机制**：掌握 P/A 匹配模型，这是所有推导规则的基础
2. **熟悉类型调整规则**：特别是数组退化、顶层 cv 限定符忽略
3. **掌握转发引用**：理解左值/右值的推导差异，是完美转发的关键
4. **了解非推导上下文**：合理利用非推导上下文解决歧义问题
5. **实践验证**：使用编译器验证推导结果，加深理解

### 参考资源

- C++ 标准文档：[temp.deduct] 章节
- cppreference: Template argument deduction
- 《C++ Templates: The Complete Guide》