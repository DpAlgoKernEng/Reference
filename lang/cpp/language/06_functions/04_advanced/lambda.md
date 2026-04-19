# Lambda 表达式（Lambda Expressions）

## 1. 概述

Lambda 表达式（Lambda Expression）是 C++11 引入的一种匿名函数机制，用于构造闭包（Closure）—— 一种能够捕获作用域内变量的无名函数对象。Lambda 表达式提供了一种简洁的方式来定义内联函数对象，特别适合用于算法回调、事件处理和函数式编程风格。

Lambda 表达式的核心特性：
- **匿名性**：无需显式命名函数
- **捕获能力**：可以捕获外部作用域的变量
- **灵活性**：支持泛型编程（C++14 起）
- **就地定义**：在需要的地方直接编写逻辑

## 2. 来源与演变

### 首次引入

Lambda 表达式首次在 **C++11** 标准中引入，旨在简化函数对象的创建过程，减少编写繁琐的仿函数类（Functor Class）。

### 历史背景

在 Lambda 出现之前，C++ 开发者需要：
1. **定义独立的函数**：无法携带上下文状态
2. **编写仿函数类**：代码冗长，定义与使用分离
3. **使用函数指针**：类型不安全，难以内联优化

Lambda 表达式的出现解决了这些问题：
- 在调用点就地定义函数逻辑
- 自动捕获上下文变量
- 编译器生成高效的函数对象

### 版本演进

| 版本 | 特性变更 |
|------|---------|
| **C++11** | Lambda 表达式首次引入，支持基本捕获和函数体 |
| **C++14** | 泛型 Lambda（参数类型使用 `auto`）、初始化捕获（init-capture）、支持默认参数 |
| **C++17** | `constexpr` Lambda、捕获 `*this` 值语义、`__cpp_capture_star_this` 特性宏 |
| **C++20** | 模板参数列表、`consteval` Lambda、`requires` 约束、初始化捕获包展开 |
| **C++23** | 无参数列表语法简化、`static` 修饰符、显式对象参数、属性支持增强 |

### 特性测试宏

| 宏 | 值 | 标准 | 特性 |
|----|-----|------|------|
| `__cpp_lambdas` | `200907L` | C++11 | Lambda 表达式 |
| `__cpp_generic_lambdas` | `201304L` | C++14 | 泛型 Lambda |
| `__cpp_generic_lambdas` | `201707L` | C++20 | 泛型 Lambda 模板参数列表 |
| `__cpp_init_captures` | `201304L` | C++14 | Lambda 初始化捕获 |
| `__cpp_init_captures` | `201803L` | C++20 | 初始化捕获包展开 |
| `__cpp_capture_star_this` | `201603L` | C++17 | 捕获 `*this` 值语义 |
| `__cpp_static_call_operator` | `202207L` | C++23 | 无捕获 Lambda 的 static operator() |

## 3. 语法与参数

### 基本语法（无显式模板参数列表）

```
[captures] front-attr(optional) (params) specs(optional) exception(optional) back-attr(optional) trailing-type(optional) requires(optional) { body }
```

简化形式（C++23 起）：

```
[captures] { body }                                        // 最简形式
[captures] front-attr(optional) trailing-type(optional) { body }
[captures] front-attr(optional) exception back-attr(optional) trailing-type(optional) { body }
[captures] front-attr(optional) specs exception(optional) back-attr(optional) trailing-type(optional) { body }
```

### 泛型 Lambda 语法（C++20 起）

```
[captures] <tparams> t-requires(optional) front-attr(optional) (params) specs(optional) exception(optional) back-attr(optional) trailing-type(optional) requires(optional) { body }
```

### 语法元素详解

#### captures（捕获列表）

捕获列表定义了 Lambda 可以访问的外部变量：

| 语法 | 说明 |
|------|------|
| `[=]` | 按值捕获所有使用的自动存储期变量 |
| `[&]` | 按引用捕获所有使用的自动存储期变量 |
| `[x]` | 按值捕获变量 x |
| `[&x]` | 按引用捕获变量 x |
| `[=, &x]` | 按值捕获所有，但 x 按引用捕获 |
| `[&, x]` | 按引用捕获所有，但 x 按值捕获 |
| `[this]` | 捕获当前对象的 this 指针（按引用） |
| `[*this]` | 按值捕获当前对象（C++17 起） |
| `[x = expr]` | 初始化捕获（C++14 起） |
| `[&x = expr]` | 引用初始化捕获（C++14 起） |

#### params（参数列表）

Lambda 的参数列表与普通函数类似：
- C++11 起：可以有默认参数（CWG 974）
- C++14 起：参数类型可以使用 `auto`（泛型 Lambda）
- C++23 起：可以有显式对象参数

#### specs（说明符）

| 说明符 | 说明 |
|--------|------|
| `mutable` | 允许修改按值捕获的对象，调用其非 const 成员函数 |
| `constexpr`（C++17） | 显式指定 operator() 为 constexpr 函数 |
| `consteval`（C++20） | 指定 operator() 为立即函数 |
| `static`（C++23） | 指定 operator() 为静态成员函数（仅限无捕获 Lambda） |

#### exception（异常说明）

为 operator() 提供动态异常说明或 `noexcept` 说明符。

#### trailing-type（尾置返回类型）

```
-> ret
```

指定返回类型。若省略，则自动推导返回类型。

#### requires（约束）（C++20 起）

为 operator() 添加约束条件。

#### front-attr / back-attr（属性）（C++23 起）

- `front-attr`：应用于 operator()
- `back-attr`：应用于 operator() 的类型

### 捕获规则

1. **不能重复捕获**：同一变量只能出现一次
2. **参数名不能与捕获同名**：捕获名必须与参数名不同
3. **捕获默认不能嵌套**：`[&, &x]` 是错误的
4. **成员变量不能直接捕获**：只能捕获 `this` 指针

## 4. 底层原理

### 闭包类型（Closure Type）

Lambda 表达式的求值结果是一个纯右值（prvalue），其类型为唯一的、无名的非联合非聚合类类型，称为**闭包类型**（Closure Type）。

闭包类型在包含 Lambda 表达式的最小块作用域、类作用域或命名空间作用域中声明（用于 ADL）。

### 闭包类型的成员

#### operator()

```cpp
ret operator()(params) { body }                                    // 普通 Lambda
template<template-params> ret operator()(params) { body }         // 泛型 Lambda (C++14)
```

- 若未指定 `mutable` 且无显式对象参数，则 operator() 是 const 成员函数
- 若指定 `static`（C++23），则是静态成员函数
- 若有显式对象参数（C++23），则是显式对象成员函数

#### 转换为函数指针（仅无捕获 Lambda）

```cpp
using F = ret(*)(params);
operator F() const noexcept;                // C++17 前
constexpr operator F() const noexcept;      // C++17 起
```

无捕获的 Lambda 可以隐式转换为函数指针。

#### 构造函数与赋值运算符

| 操作 | C++20 前 | C++20 起（无捕获） | C++20 起（有捕获） |
|------|----------|-------------------|-------------------|
| 默认构造 | 删除 | 默认 | 删除 |
| 复制构造 | 默认 | 默认 | 默认 |
| 移动构造 | 默认 | 默认 | 默认 |
| 复制赋值 | 删除 | 默认 | 删除 |
| 移动赋值 | 删除 | 默认 | 删除 |

#### 析构函数

```cpp
~ClosureType() = default;
```

### 捕获变量的存储

- **按值捕获**：闭包类型包含未命名的非静态数据成员，存储捕获变量的副本
- **按引用捕获**：实现可能不存储额外数据成员，直接引用原始对象

### 悬垂引用问题

如果 Lambda 捕获的引用对象在其生命周期结束之后仍被调用，会导致未定义行为：

```cpp
auto make_function(int& x) {
    return [&] { return x; };  // 危险：x 可能在 Lambda 调用时已销毁
}
```

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 算法回调 | 作为 `std::sort`、`std::for_each` 等算法的谓词 |
| 事件处理 | GUI 编程中的事件响应函数 |
| 并发编程 | 作为线程函数或任务函数 |
| 函数式编程 | 高阶函数、柯里化等 |
| 延迟计算 | 延迟到特定时机执行的代码块 |
| 回调注册 | 异步操作的完成回调 |

### 捕获策略选择

| 捕获方式 | 适用情况 | 注意事项 |
|----------|---------|---------|
| `[=]` | 需要只读访问多个外部变量 | 注意 `this` 的隐式引用语义 |
| `[&]` | 需要修改外部变量 | 小心悬垂引用 |
| `[x]` | 只需要一个变量的副本 | 变量需可复制 |
| `[&x]` | 只需要修改一个变量 | 确保变量生命周期 |
| `[*this]` | 异步场景需要对象副本 | C++17 起 |
| `[x = std::move(x)]` | 捕获仅移动类型 | C++14 起 |

### 最佳实践

1. **优先使用初始化捕获**：C++14 起可用，语义更清晰
2. **避免捕获 `this` 在异步代码中**：使用 `[*this]` 捕获对象副本
3. **按引用捕获时确保生命周期**：Lambda 调用时被引用对象必须有效
4. **泛型 Lambda 配合 `auto` 参数**：提高代码通用性
5. **使用 `constexpr` Lambda**：支持编译期计算（C++17 起）

### 常见陷阱

1. **按值捕获的 `this` 实际是引用**：`[=]` 捕获 `this` 是按引用捕获
2. **修改按值捕获的变量需要 `mutable`**：否则编译错误
3. **成员变量不能直接捕获**：需要通过捕获 `this` 或使用初始化捕获
4. **嵌套 Lambda 的捕获**：需要正确理解捕获链

## 6. 代码示例

### 基础用法

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

    // 基本排序：按升序
    std::sort(v.begin(), v.end());
    for (int i : v) std::cout << i << ' ';
    std::cout << '\n';

    // 使用 Lambda 自定义排序：按降序
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    for (int i : v) std::cout << i << ' ';
    std::cout << '\n';

    // 按值捕获
    int threshold = 5;
    auto count_above = [threshold](int x) { return x > threshold; };
    int count = std::count_if(v.begin(), v.end(), count_above);
    std::cout << "Elements > " << threshold << ": " << count << '\n';

    return 0;
}
```

### 捕获方式示例

```cpp
#include <iostream>
#include <memory>

void capture_examples() {
    int x = 10;
    int y = 20;

    // 按值捕获
    auto by_value = [x]() { return x; };
    std::cout << "by_value: " << by_value() << '\n';

    // 按引用捕获
    auto by_ref = [&x]() { x *= 2; };
    by_ref();
    std::cout << "after by_ref, x = " << x << '\n';  // x = 20

    // 混合捕获
    auto mixed = [=, &y]() { y = x + y; return x; };  // x 按值，y 按引用
    mixed();
    std::cout << "after mixed, y = " << y << '\n';    // y = 40

    // 初始化捕获 (C++14)
    auto init_capture = z = x + y]() { return z; };
    std::cout << "init_capture: " << init_capture() << '\n';

    // 移动捕获 (C++14)
    auto ptr = std::make_unique<int>(42);
    auto move_capture = p = std::move(ptr)]() { return *p; };
    std::cout << "move_capture: " << move_capture() << '\n';
    // ptr 现在为 nullptr
}
```

### 泛型 Lambda（C++14）

```cpp
#include <iostream>
#include <string>

void generic_lambda_examples() {
    // 泛型 Lambda：参数类型使用 auto
    auto print = [](const auto& value) {
        std::cout << value << '\n';
    };

    print(42);           // int
    print(3.14);         // double
    print(std::string("hello"));  // std::string

    // 多参数泛型 Lambda
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << add(1, 2) << '\n';           // 3
    std::cout << add(1.5, 2.5) << '\n';       // 4.0
    std::cout << add(std::string("a"), std::string("b")) << '\n'; // "ab"
}
```

### 模板 Lambda（C++20）

```cpp
#include <iostream>
#include <vector>

void template_lambda_examples() {
    // C++20: 显式模板参数列表
    auto print_array = []<typename T, std::size_t N>(const T(&arr)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            std::cout << arr[i] << ' ';
        }
        std::cout << '\n';
    };

    int arr[] = {1, 2, 3, 4, 5};
    print_array(arr);

    // 带约束的 Lambda (C++20)
    auto safe_divide = []<typename T>(T a, T b) requires requires {
        requires std::is_arithmetic_v<T>;
        { a / b } -> std::same_as<T>;
    } {
        return b != 0 ? a / b : T{};
    };

    std::cout << safe_divide(10.0, 2.0) << '\n';  // 5.0
}
```

### mutable Lambda

```cpp
#include <iostream>

void mutable_example() {
    int counter = 0;

    // 不使用 mutable：无法修改按值捕获的变量
    // auto bad = [counter]() { counter++; };  // 编译错误

    // 使用 mutable：可以修改捕获的副本
    auto increment = [counter]() mutable {
        return ++counter;
    };

    std::cout << increment() << '\n';  // 1
    std::cout << increment() << '\n';  // 2
    std::cout << increment() << '\n';  // 3

    // 原始变量不受影响
    std::cout << "original counter: " << counter << '\n';  // 0
}
```

### 类成员中的 Lambda

```cpp
#include <iostream>

class Widget {
    int value = 0;

public:
    void demonstrate_capture() {
        // 捕获 this 指针（按引用捕获对象）
        auto get_value = [this]() { return value; };

        // 捕获 this 指针
        auto set_value = [this](int v) { value = v; };

        // C++17: 按值捕获 *this
        auto get_copy = [*this]() { return value; };

        // 使用初始化捕获捕获成员
        auto capture_member = [v = value]() { return v; };

        set_value(42);
        std::cout << "value: " << get_value() << '\n';    // 42
        std::cout << "copy: " << get_copy() << '\n';       // 42
    }
};
```

### 递归 Lambda

```cpp
#include <functional>
#include <iostream>

void recursive_lambda_examples() {
    // 方法 1：使用 std::function（有开销）
    std::function<int(int)> fib1 = [&fib1](int n) {
        return n <= 1 ? n : fib1(n - 1) + fib1(n - 2);
    };
    std::cout << "fib1(10) = " << fib1(10) << '\n';

    // 方法 2：将自身作为参数传递（无额外开销）
    auto fib2 = [](auto self, int n) -> int {
        return n <= 1 ? n : self(self, n - 1) + self(self, n - 2);
    };
    std::cout << "fib2(10) = " << fib2(fib2, 10) << '\n';

    // C++23: 显式对象参数（最优解）
#if __cpp_explicit_this_parameter
    auto fib3 = [](this auto self, int n) -> int {
        return n <= 1 ? n : self(n - 1) + self(n - 2);
    };
    std::cout << "fib3(10) = " << fib3(10) << '\n';
#endif
}
```

### 常见错误及修正

#### 错误 1：悬垂引用

```cpp
// 错误：返回捕获局部变量引用的 Lambda
auto make_bad_closure() {
    int x = 42;
    return [&x] { return x; };  // x 在函数返回后销毁
}  // 未定义行为！

// 修正 1：按值捕获
auto make_good_closure_v1() {
    int x = 42;
    return [x] { return x; };  // 捕获副本
}

// 修正 2：移动捕获（适用于仅移动类型）
#include <memory>
auto make_good_closure_v2() {
    auto ptr = std::make_unique<int>(42);
    return [p = std::move(ptr)] { return *p; };
}
```

#### 错误 2：误解 `this` 捕获

```cpp
class BadExample {
    int value = 0;
public:
    auto get_closure() {
        // 错误理解：认为 [=] 按值捕获了所有东西
        // 实际上：this 指针被按值捕获，成员访问仍是按引用！
        return [=]() { return value; };  // 危险！
    }
};

// 正确做法：按值捕获对象
class GoodExample {
    int value = 0;
public:
    auto get_closure() {
        return [*this]() { return value; };  // C++17 安全
    }
};
```

#### 错误 3：忘记 `mutable`

```cpp
void forgot_mutable() {
    int x = 10;
    auto lambda = [x]() {
        // return x++;  // 编译错误：不能修改 const 成员
        return x;
    };

    // 修正：如果需要修改副本
    auto mutable_lambda = [x]() mutable {
        return x++;
    };
}
```

#### 错误 4：捕获 `this` 后对象销毁

```cpp
#include <functional>

class AsyncWorker {
    int data = 42;
public:
    std::function<int()> get_callback() {
        // 危险：捕获 this，但对象可能在回调执行前销毁
        return [this]() { return data; };
    }
};

// 修正：按值捕获对象
class SafeWorker {
    int data = 42;
public:
    std::function<int()> get_callback() {
        return [*this]() { return data; };  // C++17
    }
};
```

## 7. 总结

Lambda 表达式是现代 C++ 最重要的特性之一，它提供了简洁、强大的函数对象定义方式。

### 核心要点

| 特性 | 说明 |
|------|------|
| **捕获机制** | 按值 `[=]`、按引用 `[&]`、初始化捕获 `[x=expr]` |
| **泛型支持** | C++14 起 `auto` 参数，C++20 起模板参数列表 |
| **constexpr 支持** | C++17 起 Lambda 可用于编译期计算 |
| **函数指针转换** | 无捕获 Lambda 可转换为函数指针 |
| **递归支持** | C++23 显式对象参数实现优雅递归 |

### 版本选择建议

- **C++11**：基本 Lambda 功能
- **C++14**：泛型 Lambda、初始化捕获（强烈推荐）
- **C++17**：`constexpr` Lambda、`[*this]` 捕获
- **C++20**：模板 Lambda、`consteval` Lambda
- **C++23**：`static` Lambda、显式对象参数、简化语法

### 最佳实践总结

1. 默认使用 `[=]` 或 `[&]`，根据需要显式捕获
2. 异步场景优先使用 `[*this]` 捕获对象副本
3. 仅移动类型使用初始化捕获 `x = std::move(x)`
4. 泛型编程使用 `auto` 参数
5. 递归 Lambda 使用 C++23 显式对象参数或传递自身

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/lambda
- C++ Standard: [expr.prim.lambda]
- Effective Modern C++, Scott Meyers, Item 31-34