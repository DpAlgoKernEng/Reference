# 约束与概念（Constraints and Concepts）

## 1. 概述 (Overview)

**约束（Constraints）** 和 **概念（Concepts）** 是 C++20 引入的重要特性，用于对模板参数指定要求。类模板、函数模板（包括泛型 lambda）以及其他模板化函数可以与约束关联，约束指定了对模板参数的要求，可用于选择最合适的函数重载和模板特化。

**概念** 是命名的要求集合。每个概念都是一个在编译时求值的谓词，成为使用它作为约束的模板接口的一部分。

**核心价值**：
- 在编译时早期检测模板参数约束违反
- 提供清晰易懂的错误信息
- 模型语义类别（如 Number、Range、RegularFunction）而非语法限制（如 HasPlus、Array）

根据 ISO C++ 核心指南 T.20："指定有意义的语义是真正概念的定义特征，而非语法约束。"

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 模板编程长期以来面临错误信息难以理解的问题。当模板实例化失败时，编译器往往会产生数十行甚至数百行难以理解的错误输出。

### 设计动机

概念特性的设计目标是：
1. 提供一种在模板实例化早期检测约束违反的机制
2. 改善模板相关的编译错误信息质量
3. 使模板接口更具表达力和自文档化
4. 支持基于语义类别而非语法限制的约束

### 版本演进

| 特性测试宏 | 值 | 标准 | 特性 |
|-----------|-----|------|------|
| `__cpp_concepts` | `201907L` | C++20 | 约束特性 |
| `__cpp_concepts` | `202002L` | C++20 | 条件平凡特殊成员函数 |
| 折叠扩展约束 | - | C++26 | 折叠扩展约束支持 |

### 缺陷报告

| DR | 应用于 | 已发布行为 | 正确行为 |
|----|--------|-----------|---------|
| CWG 2428 | C++20 | 无法将属性应用于概念 | 已允许 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 概念定义语法

概念定义必须出现在命名空间作用域，语法形式如下：

```
template <模板参数列表> concept 概念名 属性(可选) = 约束表达式;
```

| 参数 | 说明 |
|------|------|
| 概念名 | 概念的标识符 |
| 属性 | 任意数量的属性序列 |
| 约束表达式 | 编译时求值为 bool 的常量表达式 |

### 概念定义示例

```cpp
// 定义概念 "Derived"，检查 T 是否派生自 U
template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

// 单参数概念
template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};
```

### 约束类型

约束是对模板参数指定要求的逻辑操作和操作数序列。共有四种类型（C++26 新增第四种）：

| 类型 | 说明 | 引入版本 |
|------|------|---------|
| 合取约束（Conjunctions） | 使用 `&&` 连接的约束 | C++20 |
| 析取约束（Disjunctions） | 使用 `\|\|` 连接的约束 | C++20 |
| 原子约束（Atomic constraints） | 不可分割的基础约束 | C++20 |
| 折叠扩展约束（Fold expanded constraints） | 从约束和折叠操作符形成 | C++26 |

### requires 子句

`requires` 关键字用于引入 **requires 子句**，指定对模板参数或函数声明的约束：

```cpp
// 出现在函数声明符的最后元素
template<typename T>
void f(T&&) requires Eq<T>;

// 出现在模板参数列表之后
template<typename T> requires Addable<T>
T add(T a, T b) { return a + b; }
```

requires 子句后的表达式必须是以下形式之一：
- 主表达式（如 `Swappable<T>`）
- 用 `&&` 连接的主表达式序列
- 用 `||` 连接的上述表达式序列

### 约束应用方式

约束模板的四种等价写法：

```cpp
template<Hashable T>           // 方式1：类型参数约束
void f(T) {}

template<typename T>           // 方式2：requires 子句在模板参数后
    requires Hashable<T>
void f(T) {}

template<typename T>           // 方式3：尾置 requires 子句
void f(T) requires Hashable<T> {}

void f(Hashable auto) {}      // 方式4：简写函数模板
```

---

## 4. 底层原理 (Underlying Principles)

### 约束规范化（Constraint Normalization）

约束规范化是将约束表达式转换为原子约束的合取和析取序列的过程。

**规范形式定义**：

| 表达式形式 | 规范形式 |
|-----------|---------|
| `(E)` | E 的规范形式 |
| `E1 && E2` | E1 和 E2 规范形式的合取 |
| `E1 \|\| E2` | E1 和 E2 规范形式的析取 |
| `C<A1, A2, ..., AN>`（C 是概念） | 概念约束表达式的规范形式，替换参数映射 |
| 其他表达式 E | 原子约束，表达式为 E，参数映射为恒等映射 |

### 约束关联顺序

声明关联的约束由逻辑 AND 表达式确定，操作数顺序如下：
1. 每个受约束的类型模板参数或非类型模板参数引入的约束表达式（按出现顺序）
2. 模板参数列表后的 requires 子句中的约束表达式
3. 简写函数模板声明中每个受约束占位符类型参数引入的约束表达式
4. 尾置 requires 子句中的约束表达式

此顺序决定了检查满足性时约束实例化的顺序。

### 约束子sumption（包含关系）

约束 `P` **包含**（subsume）约束 `Q`，当可以证明 `P` 蕴含 `Q`（基于原子约束的同一性）。

**判定算法**：
1. 将 `P` 转换为析取范式（DNF）
2. 将 `Q` 转换为合取范式（CNF）
3. `P` 包含 `Q` 当且仅当：DNF 中每个析取子句包含 CNF 中每个合取子句

**原子约束同一性**：两个原子约束被认为是相同的，如果它们在源码级别由相同表达式形成，且参数映射等价。

### 合取和析取的短路求值

合取和析取约束按从左到右顺序求值，并支持短路：
- 合取：如果左约束不满足，不尝试右约束的模板参数替换
- 析取：如果左约束满足，不尝试右约束的模板参数替换

```cpp
template<typename T>
    requires (sizeof(T) > 1 && get_value<T>())
void f(T);   // #1

// 当 sizeof(char) > 1 不满足时，get_value<T>() 不会被检查
// 这防止了立即上下文之外的替换失败
```

### 折叠扩展约束（C++26）

折叠扩展约束由约束 `C` 和折叠操作符（`&&` 或 `||`）形成，是包展开。

设 N 为包展开参数中的元素数量：
- 如果包展开无效（如展开不同大小的包），约束不满足
- 如果 N = 0：`&&` 折叠满足，`||` 折叠不满足
- 对于正 N：按顺序替换并检查每个元素

---

## 5. 使用场景 (Use Cases)

### 适用场景

1. **模板参数约束**：限制模板参数满足特定语义要求
2. **函数重载选择**：基于约束选择最佳重载
3. **模板特化选择**：确定最匹配的特化版本
4. **接口文档化**：使模板接口意图清晰明确

### 最佳实践

1. **模型语义类别而非语法限制**：
```cpp
// 推荐：语义概念
template<typename T>
concept Number = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    // ...更多语义操作
};

// 不推荐：仅语法检查
template<typename T>
concept HasPlus = requires(T a, T b) {
    { a + b };
};
```

2. **使用概念约束而非 SFINAE**：
```cpp
// C++20 之前
template<typename T,
         typename = std::enable_if_t<std::is_integral_v<T>>>
void foo(T t);

// C++20 推荐
template<std::integral T>
void foo(T t);
```

3. **利用约束子关系进行重载排序**：
```cpp
template<Decrementable T>
void f(T);  // #1 较少约束

template<RevIterator T>  // RevIterator 包含 Decrementable
void f(T);  // #2 更多约束，优先选择
```

### 常见陷阱

1. **重新声明必须使用相同语法形式**：
```cpp
// 这两个声明是正确的
template<Incrementable T>
void f(T) requires Decrementable<T>;

template<Incrementable T>
void f(T) requires Decrementable<T>;  // OK：重新声明

// 这第三个声明形式不当，NDR
template<typename T>
    requires Incrementable<T> && Decrementable<T>
void f(T);  // 错误：语法形式不同
```

2. **概念不能递归引用自身**：
```cpp
template<typename T>
concept V = V<T*>;  // 错误：递归概念
```

3. **概念不能被约束**：
```cpp
template<class T>
concept C1 = true;

template<C1 T>
concept Error1 = true;  // 错误：尝试约束概念定义

template<class T> requires C1<T>
concept Error2 = true;  // 错误：requires 子句尝试约束概念
```

4. **原子约束类型必须为 bool**：
```cpp
template<typename T>
struct S {
    constexpr operator bool() const { return true; }
};

template<typename T>
    requires (S<T>{})  // S<T>{} 类型不为 bool
void f(T);  // 即使可转换为 bool，也会出错
```

---

## 6. 代码示例 (Examples)

### 基础用法：定义和使用概念

```cpp
#include <cstddef>
#include <concepts>
#include <functional>
#include <string>

// 声明概念 "Hashable"：任何类型 T 满足此概念
// 当 std::hash<T>{}(a) 可编译且结果可转换为 std::size_t
template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

struct meow {};

// 受约束的 C++20 函数模板
template<Hashable T>
void f(T) {}

int main() {
    using std::operator""s;

    f("abc"s);    // OK，std::string 满足 Hashable
    // f(meow{}); // 错误：meow 不满足 Hashable
}
```

### 错误信息对比

```cpp
#include <list>
#include <algorithm>

std::list<int> l = {3, -1, 10};
std::sort(l.begin(), l.end());

// 典型的无概念编译错误（难以理解）：
// invalid operands to binary expression ('std::_List_iterator<int>' and
// 'std::_List_iterator<int>')
//                           std::__lg(__last - __first) * 2);
//                                     ~~~~~~ ^ ~~~~~~~
// ... 50 行输出 ...

// 典型的有概念编译错误（清晰明了）：
// error: cannot call std::sort with std::_List_iterator<int>
// note:  concept RandomAccessIterator<std::_List_iterator<int>> was not satisfied
```

### 概念继承和子关系

```cpp
template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

template<typename T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;

// SignedIntegral 包含 Integral
// UnsignedIntegral 包含 Integral 和 !SignedIntegral
```

### 使用 requires 子句

```cpp
template<typename T>
concept Eq = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

// 尾置 requires 子句
template<typename T>
void f(T&&) requires Eq<T>;

// 模板参数后 requires 子句
template<typename T> requires Addable<T>
T add(T a, T b) { return a + b; }
```

### 约束子关系与重载解析

```cpp
template<typename T>
concept Decrementable = requires(T t) { --t; };

template<typename T>
concept RevIterator = Decrementable<T> && requires(T t) { *t; };

// RevIterator 包含 Decrementable

template<Decrementable T>
void f(T);  // #1

template<RevIterator T>
void f(T);  // #2，比 #1 更多约束

int main() {
    f(0);       // int 只满足 Decrementable，选择 #1
    f((int*)0); // int* 满足两个约束，选择 #2（更多约束）
}
```

### 常见错误：约束顺序导致歧义

```cpp
template<typename T>
concept RevIterator2 = requires(T t) { --t; *t; };

template<Decrementable T>
void h(T);  // #5

template<RevIterator2 T>
void h(T);  // #6

// RevIterator2 不包含 Decrementable（原子约束不同）
// 它们是独立的约束，不构成子关系
h((int*)0); // 歧义：两个重载都不是更受约束
```

### C++26 折叠扩展约束

```cpp
template <class T>
concept A = std::is_move_constructible_v<T>;

template <class T>
concept B = std::is_copy_constructible_v<T>;

template <class T>
concept C = A<T> && B<T>;

// C++26 中，折叠被展开，#2 的约束包含 #1 的约束
template <class... T> requires (A<T> && ...)
void g(T...); // #1

template <class... T> requires (C<T> && ...)
void g(T...); // #2，更多约束，优先选择
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| **概念（Concepts）** | 命名的约束集合，编译时求值的谓词 |
| **约束（Constraints）** | 对模板参数要求的逻辑表达式 |
| **requires 子句** | 引入约束的语法结构 |
| **子关系（Subsumption）** | 约束之间的包含关系，用于重载选择 |

### 约束类型对比

| 约束类型 | 操作符 | 求值特性 |
|---------|--------|---------|
| 合取约束 | `&&` | 短路求值，左假则右不计算 |
| 析取约束 | `\|\|` | 短路求值，左真则右不计算 |
| 原子约束 | - | 基础约束单元 |
| 折叠扩展约束 | `&&` 或 `\|\|` | C++26，包展开支持 |

### 技术对比

| 方面 | C++17 (SFINAE) | C++20 (Concepts) |
|------|----------------|------------------|
| 语法复杂度 | 高（enable_if、类型特征） | 低（直接声明式语法） |
| 错误信息 | 难以理解 | 清晰明确 |
| 表达能力 | 有限 | 强大（支持语义建模） |
| 编译时开销 | 较高 | 较低（早期检测） |

### 学习建议

1. 从标准库概念开始学习（`<concepts>` 头文件）
2. 理解约束子关系对重载解析的影响
3. 优先使用语义概念而非语法约束
4. 掌握 requires 表达式和 requires 子句的区别
5. 了解 C++26 折叠扩展约束的新特性

### 相关关键字

- `concept`
- `requires`
- `typename`

### 参见

- Requires Expression（requires 表达式）
- Named Requirements（命名要求）
- Concepts TS