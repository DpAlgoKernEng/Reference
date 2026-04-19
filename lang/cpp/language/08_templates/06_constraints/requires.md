# `requires` 表达式 (since C++20)

## 1. 概述

`requires` 表达式是 C++20 引入的核心特性之一，用于在编译期检测和表达模板参数的约束条件。它返回一个 `bool` 类型的纯右值（prvalue），用于描述一组约束是否被满足。

`requires` 表达式是 C++ 概念（Concepts）机制的重要组成部分，使得模板元编程中的约束检查更加直观和类型安全。

定义在 `<concepts>` 头文件相关命名空间中，作为概念定义的核心语法元素。

## 2. 来源与演变

### 首次引入

`requires` 表达式在 **C++20** 标准中正式引入，是 Concepts TS（Technical Specification）最终纳入标准的成果之一。

### 历史背景

在 C++20 之前，模板约束的表达方式非常有限：

1. **SFINAE 技巧**：通过 `std::enable_if` 和类型特性进行约束检查
2. **静态断言**：使用 `static_assert` 在实例化时检查条件
3. **文档约束**：仅通过文档说明模板参数的要求

这些方法的问题：
- 代码冗长、难以阅读和维护
- 错误信息晦涩难懂
- 约束表达不够直观

`requires` 表达式的出现解决了这些问题：
- 提供了声明式的约束语法
- 改善了编译器的错误诊断
- 与 `concept` 关键字配合实现类型安全的泛型编程

### C++20 核心特性

`requires` 有两种用法：
1. **`requires` 表达式**：产生布尔值的约束表达式
2. **`requires` 子句**：用于函数模板声明中的约束子句

### 缺陷报告

| DR | 版本 | 问题描述 | 修正 |
|-----|------|---------|------|
| CWG 2560 | C++20 | 参数类型在 requires 表达式中是否调整不明确 | 明确也需要调整 |
| CWG 2911 | C++20 | 原规定所有 requires 表达式内的表达式都是不求值操作数 | 修正为只有部分是 |

## 3. 语法与参数

### 基本语法

```cpp
// 形式 (1): 无参数形式
requires { requirement-seq }

// 形式 (2): 带参数形式
requires ( parameter-list ) { requirement-seq }
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `parameter-list` | 可选的参数列表，用于在约束中引入局部参数 |
| `requirement-seq` | 要求序列，包含一个或多个要求（requirement） |

### 四种要求类型

`requirement-seq` 可以包含以下四种类型的要求：

| 类型 | 语法 | 说明 |
|------|------|------|
| 简单要求 (Simple requirement) | `expression ;` | 断言表达式有效 |
| 类型要求 (Type requirement) | `typename identifier ;` | 断言类型有效 |
| 复合要求 (Compound requirement) | `{ expression } ...` | 断言表达式的多个属性 |
| 嵌套要求 (Nested requirement) | `requires constraint-expression ;` | 嵌套约束表达式 |

### 简单要求语法

```cpp
expression ;
```

- `expression`：不以 `requires` 开头的任意表达式
- 表达式是不求值操作数（unevaluated operand）

### 类型要求语法

```cpp
typename identifier ;
```

- `identifier`：可能是限定的标识符（包括简单模板标识符）

### 复合要求语法

```cpp
{ expression } ;                                        // (1) 基本形式
{ expression } noexcept ;                                // (2) 带 noexcept
{ expression } -> type-constraint ;                      // (3) 带类型约束
{ expression } noexcept -> type-constraint ;             // (4) 完整形式
```

| 部分 | 说明 |
|------|------|
| `expression` | 要检查的表达式 |
| `type-constraint` | 类型约束，用于限制表达式结果的类型 |

### 嵌套要求语法

```cpp
requires constraint-expression ;
```

- `constraint-expression`：表示约束的表达式

## 4. 底层原理

### 求值机制

`requires` 表达式的求值遵循以下规则：

1. **词法顺序检查**：替换和语义约束检查按词法顺序进行
2. **短路求值**：遇到决定结果的条件时立即停止
3. **SFINAE 友好**：约束失败不会导致程序错误，而是返回 `false`

### 替换失败处理

```cpp
template<class T>
concept C = requires
{
    new int[-(int)sizeof(T)]; // 对所有 T 都无效：非良构，无需诊断
};
```

当模板参数替换到 `requires` 表达式中时：
- 如果形成无效类型或表达式，表达式求值为 `false`
- 程序本身不会因为约束失败而变得非良构

### 局部参数特性

`requires` 表达式可以引入局部参数：

```cpp
template<typename T>
concept C = requires(T p[2])
{
    (decltype(p))nullptr; // OK，p 的类型是 T*
};
```

局部参数的特点：
- 没有链接性（linkage）
- 没有存储
- 没有生命周期
- 仅作为定义要求的记号使用

### 类型推导规则

局部参数的类型推导与函数参数相同：

```cpp
// 数组类型 T[N] 退化为 T*
requires(T arr[10]) { /* arr 的类型是 T* */ }
```

### 禁止的形式

以下形式会导致程序非良构：

```cpp
// 错误：局部参数有默认参数
template<typename T>
concept C1 = requires(T t = 0) { t; };

// 错误：参数列表以省略号结尾
template<typename T>
concept C2 = requires(T t, ...) { t; };
```

## 5. 使用场景

### 适合使用 `requires` 表达式的场景

| 场景 | 说明 |
|------|------|
| 定义概念 (Concept) | 检查类型是否满足特定约束 |
| 模板约束 | 在函数模板或类模板中约束模板参数 |
| SFINAE 替代 | 替代复杂的 `std::enable_if` 技巧 |
| 类型特征检测 | 检测类型是否具有特定成员或操作 |

### 简单要求：验证表达式有效性

用于检查某个表达式是否可以编译通过：

```cpp
template<typename T>
concept Addable = requires (T a, T b)
{
    a + b;  // 检查 "a + b" 是否是有效的表达式
};

template<class T, class U = T>
concept Swappable = requires(T&& t, U&& u)
{
    swap(std::forward<T>(t), std::forward<U>(u));
    swap(std::forward<U>(u), std::forward<T>(t));
};
```

### 类型要求：验证类型存在性

用于检查某个类型是否存在：

```cpp
template<typename T>
using Ref = T&;

template<typename T>
concept C = requires
{
    typename T::inner;  // 检查嵌套类型成员是否存在
    typename S<T>;      // 检查类模板特化是否有效
    typename Ref<T>;    // 检查别名模板替换是否有效
};
```

### 复合要求：验证表达式属性

复合要求可以同时检查多个属性：

```cpp
template<typename T>
concept C2 = requires(T x)
{
    // 表达式 *x 必须有效
    // 且结果必须可转换为 T::inner
    { *x } -> std::convertible_to<typename T::inner>;

    // 表达式 x + 1 必须有效
    // 且结果类型必须精确为 int
    { x + 1 } -> std::same_as<int>;

    // 表达式 x * 1 必须有效
    // 且结果必须可转换为 T
    { x * 1 } -> std::convertible_to<T>;
};
```

复合要求的检查顺序：

1. 替换模板参数到表达式
2. 如果有 `noexcept`，检查表达式是否不抛异常
3. 如果有类型约束，检查 `decltype((expression))` 是否满足约束

### 嵌套要求：组合约束

用于在局部参数基础上添加额外约束：

```cpp
template<class T>
concept Semiregular = DefaultConstructible<T> &&
    CopyConstructible<T> && CopyAssignable<T> && Destructible<T> &&
    requires(T a, std::size_t n)
{
    requires Same<T*, decltype(&a)>;           // 嵌套要求
    { a.~T() } noexcept;                        // 复合要求
    requires Same<T*, decltype(new T)>;         // 嵌套要求
    requires Same<T*, decltype(new T[n])>;      // 嵌套要求
    { delete new T };                           // 复合要求
    { delete new T[n] };                        // 复合要求
};
```

### `requires` 表达式 vs `requires` 子句

注意区分两种用法：

```cpp
// requires 表达式：产生布尔值
template<typename T>
concept Addable = requires (T x) { x + x; };

// requires 子句：约束模板参数
template<typename T> requires Addable<T>
T add(T a, T b) { return a + b; }

// 注意：requires 关键字使用两次
template<typename T>
    requires requires (T x) { x + x; }  // 子句 + 表达式
T add(T a, T b) { return a + b; }
```

### 最佳实践

1. **优先使用命名概念**：而非直接在 `requires` 子句中写复杂约束
2. **语义化命名**：概念名称应清楚表达其含义
3. **组合概念**：使用逻辑运算符组合多个简单概念
4. **避免过度约束**：只约束真正需要的操作

## 6. 代码示例

### 基础用法：定义概念

```cpp
#include <concepts>
#include <type_traits>

// 定义基本概念
template<typename T>
concept Numeric = requires(T a, T b)
{
    a + b;  // 支持加法
    a - b;  // 支持减法
    a * b;  // 支持乘法
    a / b;  // 支持除法
};

// 使用概念约束模板
template<Numeric T>
T calculate(T a, T b) {
    return a + b * a - b;
}
```

### 类型要求示例

```cpp
#include <concepts>
#include <type_traits>

// 检查类型是否有 value_type 成员
template<typename T>
concept HasValueType = requires
{
    typename T::value_type;
};

// 检查迭代器类型
template<typename T>
concept Iterator = requires
{
    typename T::iterator_category;
    typename T::value_type;
    typename T::difference_type;
    typename T::pointer;
    typename T::reference;
};
```

### 复合要求示例

```cpp
#include <concepts>
#include <utility>

// 可解引用且不抛异常
template<typename T>
concept Dereferenceable = requires(T t)
{
    { *t } -> std::same_as<typename T::reference>;
};

// 可递增
template<typename T>
concept Incrementable = requires(T t)
{
    { ++t } -> std::same_as<T&>;
    { t++ } -> std::same_as<T>;
};

// 完整的迭代器概念
template<typename T>
concept ForwardIterator = Dereferenceable<T> &&
    Incrementable<T> &&
    requires(T a, T b)
{
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};
```

### 嵌套要求示例

```cpp
#include <concepts>
#include <cstddef>

template<typename T>
concept Container = requires(T c)
{
    typename T::value_type;
    typename T::size_type;
    typename T::iterator;

    requires std::same_as<typename T::size_type, std::size_t>;
    requires requires(typename T::iterator it)
    {
        { *it } -> std::same_as<typename T::value_type&>;
    };
};
```

### 高级用法：SFINAE 替代

```cpp
#include <concepts>
#include <iostream>

// C++17 风格（使用 SFINAE）
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void old_style_print(T value) {
    std::cout << "Integer: " << value << std::endl;
}

// C++20 风格（使用 requires）
template<typename T>
    requires std::integral<T>
void new_style_print(T value) {
    std::cout << "Integer: " << value << std::endl;
}

// 或者直接在概念中检查
template<typename T>
concept Printable = requires(T t, std::ostream& os)
{
    { os << t } -> std::same_as<std::ostream&>;
};

template<Printable T>
void print(T value) {
    std::cout << value << std::endl;
}
```

### 常见错误及修正

#### 错误 1：简单要求误用以 requires 开头

```cpp
// 错误：简单要求不能以 requires 开头
template<typename T>
concept BadConcept = requires(T t)
{
    requires (t > 0);  // 这会被解析为嵌套要求，而非简单要求
};

// 正确：明确使用嵌套要求
template<typename T>
concept GoodConcept = requires(T t)
{
    requires std::integral<T>;  // 嵌套要求
};
```

#### 错误 2：局部参数使用不当

```cpp
// 错误：局部参数有默认值
template<typename T>
concept BadConcept = requires(T t = T{})  // 错误！
{
    t;
};

// 正确：不使用默认值
template<typename T>
concept GoodConcept = requires(T t)
{
    t;
};
```

#### 错误 3：混淆 requires 表达式与 requires 子句

```cpp
// 这里的 requires 是子句，不是表达式
template<typename T>
requires std::integral<T>  // requires 子句
void func(T t) {}

// 这里的 requires 是表达式
template<typename T>
concept IsIntegral = requires {  // requires 表达式
    requires std::integral<T>;
};

// 组合使用：子句中包含表达式
template<typename T>
requires requires(T t) { t + t; }  // 子句 + 表达式
T add(T a, T b) { return a + b; }
```

#### 错误 4：在非模板上下文中使用

```cpp
// 错误：在非模板声明中使用 requires 表达式
// 如果要求无效，程序非良构
void bad_func()
{
    // 错误：如果表达式无效，程序直接错误
    constexpr bool check = requires { typename int::foo; };
}

// 正确：在模板上下文中使用
template<typename T>
void good_func()
{
    // 正确：替换失败时返回 false，不会导致程序错误
    constexpr bool check = requires { typename T::foo; };
    static_assert(check, "T must have foo member");
}
```

## 注意事项

1. **求值顺序**：要求按词法顺序检查，利用短路求值特性优化性能
2. **不求值上下文**：简单要求和复合要求中的表达式是不求值的
3. **类型调整**：参数类型会像函数参数一样调整（数组退化为指针）
4. **命名冲突**：简单要求不能以未括号化的 `requires` 表达式开头
5. **编译器支持**：需要 C++20 或更高版本

## 相关概念

| 概念 | 关系 |
|------|------|
| `concept` | 用于定义命名约束的关键字 |
| `constraints` | 约束表达式，限制模板参数 |
| `std::same_as` | 标准库概念，检查类型相同 |
| `std::convertible_to` | 标准库概念，检查可转换性 |
| `std::derived_from` | 标准库概念，检查派生关系 |
| `std::integral` | 标准库概念，检查整数类型 |

## 7. 总结

`requires` 表达式是 C++20 概念系统的核心组成部分，提供了声明式的模板约束语法。

**核心优势：**
- **声明式语法**：约束表达直观清晰
- **编译期检查**：错误在模板定义时发现
- **改进错误诊断**：编译器可提供友好的错误信息
- **SFINAE 替代**：简化复杂的类型特征检测

**四种要求类型总结：**

| 类型 | 用途 | 示例 |
|------|------|------|
| 简单要求 | 验证表达式有效 | `a + b;` |
| 类型要求 | 验证类型存在 | `typename T::value_type;` |
| 复合要求 | 验证表达式属性 | `{ *p } -> std::same_as<int>;` |
| 嵌套要求 | 组合约束 | `requires std::integral<T>;` |

**使用建议：**
1. 优先使用标准库提供的概念
2. 为自定义概念选择语义化的名称
3. 避免在非模板上下文中使用
4. 注意区分 `requires` 表达式和 `requires` 子句

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 7.5.7 Requires expressions [expr.prim.req]
- C++20 标准 (ISO/IEC 14882:2020): 7.5.7 Requires expressions [expr.prim.req]
- cppreference: https://en.cppreference.com/w/cpp/language/requires