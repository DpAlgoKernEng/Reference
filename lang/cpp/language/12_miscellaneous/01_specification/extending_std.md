# 扩展 std 命名空间

## 概述 (Overview)

在 C++ 中，`std` 命名空间是标准库的核心命名空间，包含了所有标准库类型、函数和模板。对于开发者而言，了解如何正确地与 `std` 命名空间交互至关重要。

根据 C++ 标准，向 `std` 命名空间或其嵌套命名空间中添加声明或定义是**未定义行为 (Undefined Behavior)**，只有少数例外情况允许这样做。这一规则保护了标准库的完整性和可移植性，确保程序在不同编译器和平台上的行为一致。

允许的例外情况主要包括：
- 为标准库类模板添加特化（需满足特定条件）
- 为标准库函数模板添加特化（C++20 之前允许，C++20 起禁止）
- 显式实例化标准库模板（需满足特定条件）

## 来源与演变 (Origin and Evolution)

### 历史背景

C++ 标准委员会在设计标准库时，需要平衡两个需求：一方面要保护标准库的稳定性，另一方面要允许用户扩展标准库以支持自定义类型。因此制定了一系列关于 `std` 命名空间扩展的规则。

### 版本演变

| C++ 版本 | 主要变化 |
|----------|----------|
| C++98 | 确立了基本的扩展规则，允许用户定义类型的模板特化 |
| C++11 | 引入了 `std::hash`、`std::atomic` 等需要特化的模板；增加了 `<type_traits>` 中模板的特化限制 |
| C++14 | 引入变量模板，增加了对变量模板特化的规定 |
| C++17 | 废弃 `std::unary_function` 和 `std::binary_function` |
| C++20 | 禁止函数模板的完全特化；引入地址限制规则；新增 `std::ranges` 相关特化点 |

### 缺陷报告

| 缺陷报告 | 应用版本 | 原有行为 | 修正行为 |
|----------|----------|----------|----------|
| LWG 120 | C++98 | 用户可以为非用户定义类型显式实例化标准库模板 | 禁止此类实例化 |
| LWG 232 | C++98 | 用户可以基于具有外部链接的用户定义名称进行特化（可能引用非用户定义类型） | 仅允许针对用户定义类型 |
| LWG 422 | C++98 | 用户可以单独特化成员或成员模板，而无需特化整个类 | 此类行为为未定义行为 |

## 语法与参数 (Syntax and Parameters)

### 向 std 添加声明

基本原则：向 `std` 命名空间或其嵌套命名空间添加声明或定义是未定义行为。

### 类模板特化

**允许条件**：
1. 特化必须依赖于至少一个程序定义类型 (program-defined type)
2. 特化必须满足原始模板的所有要求
3. 特化未被明确禁止

**语法形式**：
```cpp
template<>
struct std::hash<MyType>
{
    std::size_t operator()(const MyType& t) const { return t.hash(); }
};
```

### 函数模板特化

| C++ 版本 | 规则 |
|----------|------|
| C++20 之前 | 允许特化，条件同类模板特化 |
| C++20 起 | 禁止对标准库函数模板进行完全特化 |

### 变量模板特化 (C++14 起)

基本原则：声明标准库变量模板的完全或部分特化是未定义行为，除非明确允许。

**允许特化的变量模板**：
- `std::disable_sized_sentinel_for`
- `std::ranges::disable_sized_range`
- `std::ranges::enable_view`
- `std::ranges::enable_borrowed_range`
- 数学常量变量模板

### 显式实例化

允许显式实例化标准库模板，条件：
1. 声明依赖于至少一个程序定义类型
2. 实例化满足原始模板的标准库要求

## 底层原理 (Underlying Principles)

### 为什么需要这些限制？

1. **实现自由度**：标准库实现者需要保留修改内部实现的自由，用户随意添加代码会破坏这种自由度。

2. **名称冲突**：标准库可能在内部使用某些名称，用户添加的代码可能与未来版本产生冲突。

3. **优化机会**：实现者可能针对特定情况做特殊优化，用户特化可能破坏这些优化。

4. **一致性保证**：确保标准库类型在不同平台上行为一致。

### 特化机制的设计考量

允许用户为自定义类型特化标准库模板是一种精心设计的扩展机制：
- 用户不能修改标准库代码
- 但需要让标准库能够处理自定义类型
- 特化提供了一种安全的"注入点"，在不破坏标准库的前提下实现扩展

### C++20 函数模板特化禁止的原因

C++20 禁止函数模板特化的原因是：
- 函数模板特化与 ADL (Argument Dependent Lookup) 交互复杂
- 容易产生歧义和意外行为
- 标准库函数通常有复杂的重载集，特化容易破坏预期行为

## 使用场景 (Use Cases)

### 常见扩展场景

#### 1. 为自定义类型特化 std::hash

这是最常见的扩展场景，用于支持自定义类型作为无序容器的键。

```cpp
#include <typeindex>

struct MyType {
    int value;
    std::size_t hash() const { return std::hash<int>{}(value); }
};

// 特化 std::hash，不打开 std 命名空间
template<>
struct std::hash<MyType>
{
    std::size_t operator()(const MyType& t) const { return t.hash(); }
};

// 现在可以使用 MyType 作为 unordered_map/unordered_set 的键
std::unordered_set<MyType> my_set;
```

#### 2. 为自定义类型特化 std::numeric_limits

用于定义自定义数值类型的特性。

#### 3. 特化 traits 类型模板

如 `std::iterator_traits`，用于支持自定义迭代器类型。

### 最佳实践

1. **不要打开 std 命名空间**：使用显式特化语法而非打开命名空间进行特化，避免意外添加其他声明。

2. **确保满足所有要求**：特化必须满足原始模板的所有要求，否则可能导致未定义行为。

3. **注意版本差异**：C++20 前后对函数模板特化的规则有重大变化。

### 常见陷阱

1. **忘记依赖用户定义类型**：特化必须依赖至少一个用户定义类型，对纯标准类型的特化是未定义行为。

2. **特化成员函数模板**：为标准库类的成员函数或成员函数模板提供特化是未定义行为。

3. **地址限制违规 (C++20)**：C++20 起尝试获取标准库函数的地址可能导致未指定行为。

## 代码示例 (Examples)

### 正确用法示例

#### 示例 1：特化 std::hash（推荐方式）

```cpp
#include <typeindex>
#include <unordered_set>
#include <string>

struct Person {
    std::string name;
    int age;
};

// 特化 std::hash - 不打开 std 命名空间
template<>
struct std::hash<Person>
{
    std::size_t operator()(const Person& p) const
    {
        return std::hash<std::string>{}(p.name) ^
               std::hash<int>{}(p.age);
    }
};

int main()
{
    std::unordered_set<Person> people;
    people.insert({"Alice", 30});
    return 0;
}
```

#### 示例 2：特化 std::numeric_limits

```cpp
#include <limits>
#include <iostream>

class FixedPoint {
    int value;
public:
    static constexpr int scale = 1000;
    FixedPoint(int v) : value(v) {}
    int raw_value() const { return value; }
};

namespace std {
    template<>
    class numeric_limits<FixedPoint>
    {
    public:
        static constexpr bool is_specialized = true;
        static constexpr FixedPoint min() noexcept { return FixedPoint(INT_MIN); }
        static constexpr FixedPoint max() noexcept { return FixedPoint(INT_MAX); }
        static constexpr int digits = 31;
        static constexpr bool is_integer = false;
        static constexpr bool is_signed = true;
    };
}

int main()
{
    std::cout << "FixedPoint max: " << numeric_limits<FixedPoint>::max().raw_value()
              << std::endl;
    return 0;
}
```

### 错误用法示例

#### 示例 1：向 std 命名空间添加新函数（未定义行为）

```cpp
#include <utility>

// 错误：向 std 添加新的函数定义 - 未定义行为
namespace std
{
    pair<int, int> operator+(pair<int, int> a, pair<int, int> b)
    {
        return {a.first + b.first, a.second + b.second};
    }
}
// 正确做法：在 std 外定义，或创建命名空间别名
```

#### 示例 2：为纯标准类型特化（未定义行为）

```cpp
#include <functional>

// 错误：特化 std::hash<int> 不依赖于用户定义类型
template<>
struct std::hash<int>
{
    std::size_t operator()(int x) const { return x * 2; }
};
// 这是未定义行为，因为 int 不是用户定义类型
```

#### 示例 3：C++20 中获取标准库函数地址（未指定行为）

```cpp
#include <cmath>
#include <memory>

int main()
{
    // C++17 合法，C++20 起为未指定行为
    auto fptr = &static_cast<float(&)(float, float)>(std::betaf);

    // 使用 std::addressof 也有问题
    auto fptr2 = std::addressof(static_cast<float(&)(float, float)>(std::betaf));

    return 0;
}
```

## 总结 (Summary)

### 核心要点

1. **基本原则**：不要向 `std` 命名空间添加声明或定义，除非标准明确允许。

2. **允许的扩展**：
   - 类模板特化（需依赖用户定义类型且满足原始模板要求）
   - 显式实例化（需依赖用户定义类型）
   - 少量变量模板特化（C++14 起）

3. **禁止的操作**：
   - 为纯标准类型特化模板
   - 特化成员函数或成员函数模板
   - C++20 起：函数模板完全特化
   - 获取非可寻址函数的地址

### 版本差异速查

| 操作 | C++98 | C++11 | C++14 | C++17 | C++20 |
|------|-------|-------|-------|-------|-------|
| 向 std 添加声明 | 禁止 | 禁止 | 禁止 | 禁止 | 禁止 |
| 类模板特化 | 允许 | 允许 | 允许 | 允许 | 允许 |
| 函数模板特化 | 允许 | 允许 | 允许 | 允许 | 禁止 |
| 变量模板特化 | N/A | N/A | 受限 | 受限 | 受限 |
| 获取函数地址 | 允许 | 允许 | 允许 | 允许 | 受限 |

### 学习建议

1. 在需要扩展 `std` 时，优先考虑特化而非添加新声明。

2. 使用 `template<>` 语法进行显式特化，避免打开 `std` 命名空间。

3. 关注 C++20 的变化，特别是函数模板特化被禁止这一重大变更。

4. 参考 C++ 标准文档了解最新的特化限制和要求。