# C++ 函数 (Functions)

## 1. 概述

函数（Function）是 C++ 中的核心实体，它将一组语句序列（称为**函数体**，function body）与一个**名称**（name）以及零或多个**函数参数**（function parameters）列表相关联。

函数是 C++ 程序的基本构建块，用于：
- **代码组织**：将复杂任务分解为可管理的单元
- **代码复用**：避免重复代码，提高可维护性
- **抽象封装**：隐藏实现细节，提供清晰接口
- **模块化设计**：支持自顶向下的程序设计方法

每个函数都有其特定的类型，由返回类型、参数类型列表、`noexcept` 说明符（C++17 起）、以及对于非静态成员函数的 cv 限定和引用限定（C++11 起）共同决定。函数不是对象，不能被复制或作为值传递，但可以通过函数指针和函数引用进行间接操作。

## 2. 来源与演变

### 历史背景

函数的概念源于数学中的函数定义，在编程语言中演变为可调用的代码单元。C++ 从 C 语言继承了函数的基本概念，并在此基础上进行了大量扩展。

### C++98/03 基础特性

- 函数声明与定义
- 函数重载（Function Overloading）
- 默认参数
- 内联函数
- 函数指针
- 成员函数

### C++11 新增特性

| 特性 | 说明 |
|------|------|
| Lambda 表达式 | 实现匿名函数，支持闭包语义 |
| 右值引用参数 | 支持移动语义 |
| 可变参数模板 | 类型安全的可变参数函数 |
| 引用限定成员函数 | 区分左值/右值对象的成员函数调用 |
| `constexpr` 函数 | 编译期可求值的函数 |
| 尾置返回类型 | `auto f() -> int` 语法 |

### C++17 新增特性

| 特性 | 说明 |
|------|------|
| `noexcept` 作为函数类型一部分 | 函数类型包含异常说明 |
| `if constexpr` | 编译期条件分支 |
| 结构化绑定 | 函数可返回多值 |
| `std::invoke` | 统一的调用语法 |

### C++20 新增特性

| 特性 | 说明 |
|------|------|
| 协程（Coroutines） | 函数可暂停执行并在后续恢复 |
| `consteval` | 强制编译期求值 |
| `std::bind_front` / `std::bind_back` | 简化的函数绑定 |

### C++23 新增特性

| 特性 | 说明 |
|------|------|
| `std::move_only_function` | 仅移动的函数包装器 |
| `std::copyable_function` | 可复制的函数包装器 |
| Deducing this | 显式对象参数 |

### C++26 新增特性

| 特性 | 说明 |
|------|------|
| `std::function_ref` | 非拥有函数引用包装器 |

## 3. 语法与参数

### 基本语法结构

```cpp
// 函数声明（函数原型）
返回类型 函数名(参数列表);

// 函数定义
返回类型 函数名(参数列表)
{
    // 函数体
    语句序列;
}
```

### 函数声明示例

```cpp
// 基本函数声明
bool isodd(int n);           // 接受 int 参数，返回 bool

// 带默认参数的声明
int divide(int a, int b = 1);

// 重载函数声明
void print(int value);
void print(double value);
void print(const std::string& value);

// 带 noexcept 说明符的声明（C++17 起属于类型的一部分）
void safe_function() noexcept;

// 带引用限定的成员函数声明（C++11）
class Widget {
    void process() &;        // 左值对象调用
    void process() &&;       // 右值对象调用
    void process() const &;  // const 左值对象调用
};
```

### 参数列表

参数列表由零个或多个参数声明组成，以逗号分隔：

```cpp
// 无参数函数
void hello();

// 单参数函数
void print(int value);

// 多参数函数
int add(int a, int b);

// 带默认参数
int power(int base, int exp = 2);

// 可变参数函数（C 风格）
void printf_style(const char* format, ...);

// 可变参数模板（C++11，类型安全）
template<typename... Args>
void variadic_func(Args... args);
```

### 参数类型转换

在参数传递时会发生以下标准转换：

| 转换类型 | 说明 |
|---------|------|
| 数组到指针转换 | `int[]` → `int*` |
| 函数到指针转换 | `void f()` → `void (*)()` |
| 左值到右值转换 | 用于值传递 |

### 函数类型组成

函数类型由以下要素组成：

1. **返回类型**：函数返回值的类型
2. **参数类型列表**：所有参数的类型（不含名称）
3. **noexcept 说明符**（C++17 起）：是否承诺不抛出异常
4. **cv 限定**（非静态成员函数）：`const`/`volatile` 限定
5. **引用限定**（C++11 起，非静态成员函数）：`&` 或 `&&`

```cpp
// 示例：不同函数类型的函数
void f1();                    // 类型：void()
void f2() noexcept;           // 类型：void() noexcept（C++17 起）
void f3() const;              // 成员函数：const 限定
void f4() &;                   // 成员函数：左值引用限定
void f5() &&;                  // 成员函数：右值引用限定
```

## 4. 底层原理

### 函数调用机制

当函数被调用时，执行以下步骤：

1. **参数初始化**：每个参数从对应的实参初始化（复制或移动）
2. **控制转移**：程序计数器跳转到函数入口点
3. **栈帧创建**：为局部变量分配栈空间
4. **函数体执行**：执行函数体内的语句
5. **返回值处理**：将返回值复制到调用者可访问的位置
6. **栈帧销毁**：释放局部变量，恢复调用者栈帧

### 参数传递方式

| 方式 | 语法 | 特点 |
|------|------|------|
| 值传递 | `void f(T param)` | 复制参数，适合小型类型 |
| 引用传递 | `void f(T& param)` | 避免复制，可修改原对象 |
| 常量引用 | `void f(const T& param)` | 避免复制，只读访问 |
| 右值引用 | `void f(T&& param)` | 支持移动语义 |
| 指针传递 | `void f(T* param)` | 可选参数，需检查空指针 |

### 参数依赖查找（ADL）

在函数调用表达式中，非限定函数名会进行特殊的查找规则，称为**参数依赖查找**（Argument-Dependent Lookup）或 **Koenig 查找**：

```cpp
namespace MyLib {
    struct Data { int value; };

    void process(const Data& d) {
        std::cout << d.value << std::endl;
    }
}

int main() {
    MyLib::Data d{42};
    process(d);  // ADL 找到 MyLib::process，无需命名空间前缀
}
```

### 函数重载解析

当存在多个同名函数时，编译器根据以下规则选择最佳匹配：

1. 确定候选函数集
2. 筛选可行函数
3. 按隐式转换序列排序
4. 选择最佳可行函数

```cpp
void f(int);      // #1
void f(double);   // #2
void f(long);      // #3

f(10);      // 调用 #1：精确匹配 int
f(10.0);    // 调用 #2：精确匹配 double
f(10L);     // 调用 #3：精确匹配 long
f('a');     // 调用 #1：char 提升到 int
f(10u);     // 歧义：unsigned int 可转换为 int、double 或 long
```

### 函数指针与引用

函数不是对象，但可以通过指针和引用间接操作：

```cpp
// 函数指针
int (*func_ptr)(int, int) = &add;  // 或简写为 int (*func_ptr)(int, int) = add;

// 函数引用
int (&func_ref)(int, int) = add;

// 使用
int result = func_ptr(1, 2);  // 通过指针调用
int result2 = func_ref(1, 2); // 通过引用调用
```

## 5. 使用场景

### 函数声明的位置

| 位置 | 说明 |
|------|------|
| 命名空间作用域 | 普通函数，最常见 |
| 类作用域 | 成员函数或友元函数 |
| 块作用域 | 局部函数声明（不推荐，易混淆） |

### 函数重载的适用场景

| 场景 | 示例 |
|------|------|
| 相同操作，不同参数类型 | `std::abs(int)`, `std::abs(double)` |
| 操作语义相同 | `print(int)`, `print(string)` |
| 构造函数重载 | 多种初始化方式 |

### 不能仅靠返回类型区分重载

```cpp
// 错误：仅返回类型不同
int f();
double f();  // 编译错误！

// 错误：仅 noexcept 不同（C++17 起）
void g();
void g() noexcept;  // 编译错误！（C++17 起 noexcept 是类型一部分）
```

### 最佳实践

1. **单一职责**：每个函数只做一件事
2. **参数数量控制**：建议不超过 4-5 个参数，过多考虑使用结构体
3. **合理使用默认参数**：将最常用的参数放在末尾
4. **优先使用 `constexpr`**：编译期求值可提高性能
5. **使用 `noexcept` 标记不抛异常的函数**：帮助编译器优化

### 函数对象（Function Objects）

除了普通函数，以下类型也可用于函数调用：

| 类型 | 说明 |
|------|------|
| 函数指针 | `int (*fp)(int)` |
| 重载 `operator()` 的类 | 函数对象（Functor） |
| Lambda 表达式 | 匿名函数对象 |
| `std::function` | 多态函数包装器 |
| `std::bind` 结果 | 绑定表达式 |

```cpp
#include <functional>
#include <algorithm>
#include <vector>

// 函数对象示例
struct Multiplier {
    int factor;
    int operator()(int x) const { return x * factor; }
};

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Lambda 表达式
    std::transform(v.begin(), v.end(), v.begin(),
                   [](int x) { return x * 2; });

    // 函数对象
    Multiplier mul{3};
    std::transform(v.begin(), v.end(), v.begin(), mul);

    // std::function
    std::function<int(int)> func = [](int x) { return x + 1; };
}
```

### 注意事项

1. **函数不是对象**：不能创建函数数组，不能按值传递函数
2. **可寻址函数**：除 `main` 和部分标准库函数外，大多数函数可取地址
3. **无 cv 限定的函数类型**：函数类型的 cv 限定符会被忽略
4. **协程特性**（C++20）：函数可暂停执行并后续恢复

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <string>

// 基本函数定义
bool isodd(int n)
{                 // 函数体开始
    return n % 2;
}                 // 函数体结束

// 带默认参数的函数
int power(int base, int exp = 2) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

// 函数重载
void print(int value) {
    std::cout << "int: " << value << std::endl;
}

void print(double value) {
    std::cout << "double: " << value << std::endl;
}

void print(const std::string& value) {
    std::cout << "string: " << value << std::endl;
}

int main() {
    // 基本调用
    for (int arg : {-3, -2, -1, 0, 1, 2, 3})
        std::cout << isodd(arg) << ' ';  // 输出: 1 0 1 0 1 0 1

    std::cout << std::endl;

    // 默认参数
    std::cout << power(3) << std::endl;      // 输出: 9 (3^2)
    std::cout << power(3, 4) << std::endl;   // 输出: 81 (3^4)

    // 重载解析
    print(42);           // 调用 print(int)
    print(3.14);         // 调用 print(double)
    print("hello");      // 调用 print(string)

    return 0;
}
```

### 高级用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

// constexpr 函数（C++11）
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// noexcept 函数（C++17 起属于类型一部分）
void safe_operation() noexcept {
    // 保证不抛出异常
}

// 可变参数模板（C++11）
template<typename... Args>
void print_all(Args... args) {
    (std::cout << ... << args) << std::endl;  // C++17 折叠表达式
}

// 函数返回类型推导（C++14）
auto add(int a, int b) {
    return a + b;
}

// 尾置返回类型（C++11）
auto make_vector() -> std::vector<int> {
    return {1, 2, 3};
}

// Lambda 表达式（C++11）
int main() {
    // 编译期计算
    constexpr int fact5 = factorial(5);  // 编译期求值: 120
    std::cout << "factorial(5) = " << fact5 << std::endl;

    // 可变参数模板
    print_all(1, 2, 3, "hello", 3.14);

    // Lambda 表达式
    std::vector<int> v = {5, 2, 8, 1, 9};

    auto is_even = [](int n) { return n % 2 == 0; };
    int even_count = std::count_if(v.begin(), v.end(), is_even);
    std::cout << "Even numbers: " << even_count << std::endl;

    // 带捕获的 Lambda
    int threshold = 5;
    auto above = [threshold](int n) { return n > threshold; };
    int above_count = std::count_if(v.begin(), v.end(), above);
    std::cout << "Numbers above " << threshold << ": " << above_count << std::endl;

    // 泛型 Lambda（C++14）
    auto generic_add = [](auto a, auto b) { return a + b; };
    std::cout << generic_add(1, 2) << std::endl;      // int
    std::cout << generic_add(1.5, 2.5) << std::endl;  // double

    // std::function 多态函数包装
    std::function<int(int, int)> operation;

    operation = [](int a, int b) { return a + b; };
    std::cout << "Add: " << operation(3, 4) << std::endl;

    operation = [](int a, int b) { return a * b; };
    std::cout << "Multiply: " << operation(3, 4) << std::endl;

    return 0;
}
```

### 常见错误及修正

#### 错误 1：仅通过返回类型区分重载

```cpp
// 错误：仅返回类型不同，无法重载
int getValue();
double getValue();  // 编译错误！

// 修正：使用不同的函数名或参数
int getValueAsInt();
double getValueAsDouble();

// 或使用模板
template<typename T>
T getValue();
```

#### 错误 2：默认参数重复定义

```cpp
// 声明
void func(int a, int b = 10);

// 定义
void func(int a, int b = 10) {  // 错误：默认参数重复定义！
    // ...
}

// 修正：定义中不写默认参数
void func(int a, int b) {  // 正确
    // ...
}
```

#### 错误 3：成员函数 cv 限定符位置错误

```cpp
class Widget {
    // 错误：cv 限定符应放在参数列表之后
    void process() const int;  // 错误！

    // 正确写法
    void process() const;      // const 成员函数
    int getValue() const;      // const 成员函数，返回 int
};
```

#### 错误 4：局部函数声明误解

```cpp
void outer() {
    void inner(int);  // 声明，不是定义

    inner(3.14);  // 可能调用全局函数，而非期望的重载
}

// 正确做法：使用 lambda 或局部类
void outer_correct() {
    auto inner = [](int x) { return x * 2; };
    inner(3);
}
```

#### 错误 5：忽略 ADL 导致的意外行为

```cpp
namespace A {
    struct X {};
    void f(X) { std::cout << "A::f" << std::endl; }
}

namespace B {
    void f(int) { std::cout << "B::f" << std::endl; }

    void test() {
        A::X x;
        f(x);  // 调用 A::f(X)，ADL 找到了 A 命名空间的函数
    }
}

// 如果只想调用当前命名空间的函数，使用完全限定名
// B::f(static_cast<int>(x));  // 显式调用
```

## 7. 总结

### 核心要点

1. **函数是 C++ 的基本构建块**：将代码组织成可重用的单元
2. **函数类型由多部分组成**：返回类型、参数列表、noexcept 说明符、cv/ref 限定符
3. **函数不是对象**：不能按值传递，但可通过指针和引用操作
4. **重载依赖于参数差异**：返回类型和 noexcept（C++17 起）不能用于区分重载
5. **Lambda 提供了匿名函数能力**：C++11 起支持闭包语义
6. **函数对象扩展了函数概念**：支持更灵活的调用语义

### 函数 vs 函数对象对比

| 特性 | 普通函数 | 函数对象 | Lambda |
|------|---------|---------|--------|
| 状态保持 | 否 | 是（成员变量） | 是（捕获） |
| 内联优化 | 是 | 是 | 是 |
| 灵活性 | 中等 | 高 | 高 |
| 可读性 | 高 | 中等 | 高（简短时） |
| 类型安全 | 是 | 是 | 是 |

### 学习建议

1. **掌握函数重载解析规则**：理解隐式转换序列和最佳匹配
2. **理解 ADL**：参数依赖查找是 C++ 特有的重要特性
3. **学习 Lambda 表达式**：现代 C++ 函数式编程的基础
4. **了解函数指针和 `std::function`**：用于回调和多态调用
5. **熟悉 constexpr 函数**：编译期计算是性能优化的重要手段

### 相关概念

| 概念 | 关系 |
|------|------|
| 成员函数 | 类作用域内的函数，有隐式 `this` 指针 |
| 友元函数 | 可访问类私有成员的非成员函数 |
| 虚函数 | 支持运行时多态的成员函数 |
| 模板函数 | 参数化类型的函数 |
| 协程（C++20） | 可暂停和恢复执行的函数 |
| Lambda 表达式 | 匿名函数对象 |
| 函数指针 | 指向函数的指针 |
| `std::function` | 多态函数包装器 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/functions
- C++ Standard: [dcl.fct] Function definitions
- The C++ Programming Language, Bjarne Stroustrup