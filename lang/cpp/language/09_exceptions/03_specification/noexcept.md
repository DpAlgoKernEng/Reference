# `noexcept` 运算符 (since C++11)

## 1. 概述

`noexcept` 运算符是 C++11 引入的编译时检查工具，用于判断一个表达式是否声明为不抛出异常。它返回一个 `bool` 类型的纯右值（prvalue）：如果表达式被声明为不抛出异常，则返回 `true`；否则返回 `false`。

`noexcept` 运算符的核心用途是在函数模板的 `noexcept` 说明符中声明：函数对某些类型抛出异常，而对其他类型不抛出异常。这使得模板代码能够根据类型特征自动推导异常规范。

需要注意区分两个相关概念：
- **`noexcept` 运算符（operator）**：本文档的主题，用于查询表达式是否不抛出异常
- **`noexcept` 说明符（specifier）**：用于声明函数是否抛出异常

## 2. 来源与演变

### 首次引入

`noexcept` 运算符首次在 **C++11** 标准中引入，与 `noexcept` 说明符同时出现。

### 历史背景

在 `noexcept` 出现之前，C++ 使用 **动态异常规范（dynamic exception specification）**：

```cpp
// C++98 风格（已废弃）
void func() throw();        // 不抛出异常
void func() throw(int);     // 只抛出 int 类型异常
void func() throw(...);     // 可能抛出任何异常
```

动态异常规范存在以下问题：
1. 运行时检查，性能开销大
2. 编译器无法优化
3. 语法复杂，使用不便
4. 大多数编译器实现不完整

`noexcept` 的出现解决了这些问题：
- **编译时检查**：`noexcept` 运算符在编译期确定表达式的异常特性
- **零运行时开销**：编译器可以进行更激进的优化
- **简洁语法**：二元语义（抛出或不抛出），更易于理解和使用

### C++17 变化

| 变化 | 说明 |
|------|------|
| 语义调整 | 从检查"潜在异常集合是否为空"改为检查"表达式是否被指定为非抛出" |
| 临时物化 | 如果表达式是纯右值，会应用临时物化转换 |
| 析构函数可访问性 | 对于类类型的纯右值，临时物化要求析构函数非删除且可访问 |

### 缺陷报告修复

| 缺陷报告 | 版本 | 问题 | 修复 |
|----------|------|------|------|
| CWG 2722 | C++17 | 不明确纯右值表达式是否应用临时物化 | 明确在此情况下应用临时物化 |
| CWG 2792 | C++11 | `noexcept` 运算符需要判断未定义行为情况下的异常抛出 | 不再要求 |

## 3. 语法与参数

### 基本语法

```cpp
noexcept(expression)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `expression` | 要检查的表达式，作为不求值操作数（unevaluated operand）处理 |

### 返回值

- 返回 `bool` 类型的纯右值
- `true`：表达式被声明为不抛出异常
- `false`：表达式可能抛出异常

### C++17 后的语义规则

```cpp
// 返回 true 的条件（C++17 起）
noexcept(expr) == true  // 当且仅当 expr 被指定为非抛出（non-throwing）

// C++17 之前
noexcept(expr) == true  // 当且仅当 expr 的潜在异常集合为空
```

### 重要说明

1. **不求值操作数**：`expression` 不会被实际求值，仅进行类型检查
2. **未定义行为**：即使 `noexcept(expr)` 返回 `true`，如果表达式求值时遇到未定义行为，仍可能抛出异常
3. **临时物化（C++17 起）**：如果表达式是纯右值，会应用临时物化转换

## 4. 底层原理

### 编译时求值机制

`noexcept` 运算符完全在编译时执行：

1. **类型推导**：编译器分析表达式的类型
2. **异常规范检查**：检查所调用函数的异常规范
3. **隐式 noexcept 规则**：应用语言的隐式 noexcept 规则

### 隐式 noexcept 规则

某些操作默认被声明为 `noexcept`：

| 操作 | 默认 noexcept |
|------|---------------|
| 析构函数 | 是（除非基类或成员的析构函数抛出异常） |
| 默认构造函数 | 取决于成员 |
| 复制构造函数 | 取决于成员的复制构造函数 |
| 移动构造函数 | 取决于成员的移动构造函数 |
| 删除操作 | 是 |

### 表达式求值规则

`noexcept` 运算符检查表达式的"潜在异常集合"：

```cpp
// 表达式的 noexcept 属性传递规则
noexcept(func())           // 取决于 func 的 noexcept 说明
noexcept(a + b)            // 取决于 operator+ 的 noexcept 属性
noexcept(a = b)            // 取决于赋值运算符的 noexcept 属性
noexcept(T())              // 取决于 T 的构造函数和析构函数
noexcept(delete p)         // 始终为 true（析构函数检查）
```

### 性能影响

`noexcept` 运算符本身是编译时操作，没有运行时开销：

| 特性 | 说明 |
|------|------|
| 编译时求值 | 结果在编译期确定 |
| 零运行时开销 | 不生成额外代码 |
| 优化机会 | 允许编译器进行更激进的优化 |

## 5. 使用场景

### 适合使用 noexcept 运算符的场景

| 场景 | 说明 |
|------|------|
| 条件 noexcept 说明符 | 在模板中根据类型特征决定异常规范 |
| 静态断言 | 编译期验证函数的异常安全保证 |
| 类型特征实现 | 实现自定义的类型检查特征 |
| 泛型编程 | 根据类型特性选择不同的代码路径 |

### 模板中的条件 noexcept

```cpp
// 根据元素类型的移动操作决定容器的 noexcept
template<typename T>
class Container {
public:
    void push_back(T&& value)
        noexcept(std::is_nothrow_move_constructible_v<T>) {
        // 实现
    }
};
```

### 最佳实践

1. **配合类型特征使用**：使用 `<type_traits>` 中的特征进行组合判断
2. **避免过度使用**：仅在真正需要时声明 noexcept
3. **正确传递 noexcept 属性**：在包装函数中使用 `noexcept(noexcept(expr))` 模式
4. **注意析构函数**：析构函数默认是 noexcept，除非显式声明为 `noexcept(false)`

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 未定义行为 | `noexcept(expr)` 为 `true` 不保证 `expr` 求值时不抛异常 |
| 成员函数覆盖 | 派生类虚函数不能比基类版本更宽松 |
| 析构函数可访问性 | C++17 起，检查类类型纯右值要求析构函数可访问 |

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <utility>
#include <vector>

// 可能抛出异常的函数
void may_throw();

// 不抛出异常的函数
void no_throw() noexcept;

int main() {
    std::cout << std::boolalpha;

    // 检查函数的 noexcept 属性
    std::cout << "may_throw() is noexcept: "
              << noexcept(may_throw()) << '\n';     // false
    std::cout << "no_throw() is noexcept: "
              << noexcept(no_throw()) << '\n';     // true

    // 检查 lambda 的 noexcept 属性
    auto lmay_throw = []{};
    auto lno_throw = []() noexcept {};

    std::cout << "lmay_throw() is noexcept: "
              << noexcept(lmay_throw()) << '\n';   // false
    std::cout << "lno_throw() is noexcept: "
              << noexcept(lno_throw()) << '\n';    // true

    return 0;
}
```

### 高级用法：条件 noexcept 说明符

```cpp
#include <iostream>
#include <type_traits>
#include <utility>

// 条件 noexcept：根据类型特征决定是否抛出异常
template<typename T>
void swap_wrapper(T& a, T& b)
    noexcept(noexcept(std::swap(a, b))) {
    std::swap(a, b);
}

// 模板类中的条件 noexcept
template<typename T>
class Container {
public:
    // 仅当 T 的移动构造函数是 noexcept 时，此函数才是 noexcept
    void relocate(T&& value)
        noexcept(std::is_nothrow_move_constructible_v<T>) {
        // 实现
    }
};

// 使用 noexcept 运算符实现类型特征
template<typename T>
struct is_nothrow_swappable {
    static constexpr bool value =
        noexcept(std::declval<T&>() = std::declval<T&&>());
};

int main() {
    std::cout << std::boolalpha;

    // 检查标准类型的 noexcept 属性
    std::cout << "int move ctor is noexcept: "
              << std::is_nothrow_move_constructible_v<int> << '\n';

    // 检查 vector 的移动操作
    std::cout << "vector<int> move ctor is noexcept: "
              << noexcept(std::vector<int>(std::declval<std::vector<int>>())) << '\n';

    return 0;
}
```

### 常见错误及修正

#### 错误 1：误解 noexcept 运算符的含义

```cpp
// 错误理解：认为 noexcept(expr) 为 true 时，expr 绝不会抛出异常
void risky_function() noexcept {
    // 即使声明 noexcept，未定义行为仍可能"抛出"
    int* p = nullptr;
    *p = 42;  // 未定义行为！程序可能崩溃，而非抛出异常
}

// 正确理解：noexcept 仅检查声明的异常规范
void correct_usage() {
    // noexcept 运算符检查的是声明，而非运行时行为
    bool is_noexcept = noexcept(risky_function());  // true
    // 但调用 risky_function() 仍可能导致程序崩溃
}
```

#### 错误 2：在 noexcept 说明符中错误使用

```cpp
// 错误：noexcept 说明符中直接使用表达式
template<typename T>
void bad_swap(T& a, T& b) noexcept(std::swap(a, b)) {  // 编译错误！
    std::swap(a, b);
}

// 修正：使用 noexcept 运算符包装表达式
template<typename T>
void good_swap(T& a, T& b) noexcept(noexcept(std::swap(a, b))) {  // 正确
    std::swap(a, b);
}
```

#### 错误 3：忽略析构函数的影响（C++17 起）

```cpp
class NonTrivial {
public:
    ~NonTrivial() { /* 可能抛出异常的析构函数 */ }
};

void example() {
    // C++17 起，检查纯右值表达式时需要析构函数可访问且非删除
    bool result = noexcept(NonTrivial());  // 需要检查析构函数
}
```

### 完整示例：分析不同类型的 noexcept 属性

```cpp
#include <iostream>
#include <utility>
#include <vector>

void may_throw();
void no_throw() noexcept;
auto lmay_throw = []{};
auto lno_throw = []() noexcept {};

class T {
public:
    ~T() {}  // 析构函数阻止移动构造函数
             // 复制构造函数是 noexcept
};

class U {
public:
    ~U() {}  // 析构函数阻止移动构造函数
             // 复制构造函数是 noexcept(false)
    std::vector<int> v;
};

class V {
public:
    std::vector<int> v;
};

int main() {
    T t;
    U u;
    V v;

    std::cout << std::boolalpha <<
        "may_throw() is noexcept(" << noexcept(may_throw()) << ")\n"
        "no_throw() is noexcept(" << noexcept(no_throw()) << ")\n"
        "lmay_throw() is noexcept(" << noexcept(lmay_throw()) << ")\n"
        "lno_throw() is noexcept(" << noexcept(lno_throw()) << ")\n"
        "~T() is noexcept(" << noexcept(std::declval<T>().~T()) << ")\n"
        "T(rvalue T) is noexcept(" << noexcept(T(std::declval<T>())) << ")\n"
        "T(lvalue T) is noexcept(" << noexcept(T(t)) << ")\n"
        "U(rvalue U) is noexcept(" << noexcept(U(std::declval<U>())) << ")\n"
        "U(lvalue U) is noexcept(" << noexcept(U(u)) << ")\n"
        "V(rvalue V) is noexcept(" << noexcept(V(std::declval<V>())) << ")\n"
        "V(lvalue V) is noexcept(" << noexcept(V(v)) << ")\n";
}
```

输出结果：

```
may_throw() is noexcept(false)
no_throw() is noexcept(true)
lmay_throw() is noexcept(false)
lno_throw() is noexcept(true)
~T() is noexcept(true)
T(rvalue T) is noexcept(true)
T(lvalue T) is noexcept(true)
U(rvalue U) is noexcept(false)
U(lvalue U) is noexcept(false)
V(rvalue V) is noexcept(true)
V(lvalue V) is noexcept(false)
```

## 7. 总结

`noexcept` 运算符是 C++11 引入的编译时工具，用于查询表达式的异常安全保证。

### 核心要点

| 特性 | 说明 |
|------|------|
| 编译时求值 | 结果在编译期确定，零运行时开销 |
| 返回类型 | `bool` 类型纯右值 |
| 操作数 | 不求值操作数 |
| 主要用途 | 条件 noexcept 说明符、类型特征 |

### noexcept 运算符 vs noexcept 说明符

| 特性 | noexcept 运算符 | noexcept 说明符 |
|------|-----------------|-----------------|
| 作用 | 查询表达式是否 noexcept | 声明函数是否 noexcept |
| 语法 | `noexcept(expression)` | `noexcept` 或 `noexcept(expression)` |
| 结果 | `bool` 值 | 函数声明的一部分 |

### 使用建议

1. **模板编程**：使用 `noexcept(noexcept(expr))` 模式正确传递异常规范
2. **类型特征**：结合 `<type_traits>` 进行复杂的异常安全检查
3. **静态断言**：使用 `static_assert(noexcept(expr), ...)` 验证异常安全保证
4. **注意语义变化**：C++17 对纯右值表达式的处理有所变化

### 相关概念

| 概念 | 关系 |
|------|------|
| `noexcept` 说明符 | 用于声明函数的异常规范 |
| `std::is_nothrow_*` | 类型特征，检查类型的 noexcept 属性 |
| 动态异常规范 | C++17 前已废弃的异常规范语法 |

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/noexcept
- C++ Standard: [expr.unary.noexcept]
- Effective Modern C++, Scott Meyers, Item 14