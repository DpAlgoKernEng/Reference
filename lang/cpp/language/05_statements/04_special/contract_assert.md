# contract_assert 语句 (C++26)

## 1. 概述

`contract_assert` 是 C++26 标准引入的契约断言语句（contract assertion），用于在函数或 lambda 表达式体内验证内部条件。它确保在程序执行期间某个条件始终为真，如果条件评估为假或评估过程抛出异常，则触发违约处理（如调试构建中的程序终止）。在发布构建中，`contract_assert` 可被忽略以提高性能。

`contract_assert` 是 C++ 契约编程（Contract Programming）特性的重要组成部分，定义在语言核心层面，无需包含特定头文件。

## 2. 来源与演变

### 首次引入

`contract_assert` 首次在 **C++26** 标准中引入，是契约编程特性的核心组件之一。该特性通过特性测试宏 `__cpp_contracts`（值为 `202502L`）进行检测。

### 历史背景

契约编程的概念最早可追溯到 Bertrand Meyer 在 Eiffel 语言中的设计契约（Design by Contract）理念。C++ 社区经过长期讨论，最终在 C++26 中引入了完整的契约支持，包括：

- **前置条件**（precondition）：使用 `pre` 指定
- **后置条件**（postcondition）：使用 `post` 指定
- **断言语句**（assertion）：使用 `contract_assert` 指定

### 设计动机

在 `contract_assert` 出现之前，C++ 开发者主要使用以下方式进行断言检查：

| 方式 | 局限性 |
|------|--------|
| `assert()` 宏 | 依赖于 `NDEBUG` 宏，无法精细控制；无法用于 constexpr 上下文 |
| `if` 检查 + 异常 | 增加运行时开销，无法在发布构建中完全移除 |
| `[[assume(expression)]]` (C++23) | 仅用于优化提示，不做检查，违反假设会导致未定义行为 |

`contract_assert` 的出现提供了：

- **统一的标准语法**：语言级别支持，无需宏
- **可配置的违约处理**：调试时检查，发布时移除
- **与前置/后置条件的一致性**：统一的契约语义

### 相关提案

主要参考提案：**P2900R14** - Contracts for C++

## 3. 语法与参数

### 基本语法

```cpp
contract_assert attr(optional) ( predicate ) ;
```

### 参数说明

| 组成部分 | 说明 |
|---------|------|
| `contract_assert` | 关键字，标识契约断言语句 |
| `attr` (可选) | 任意数量的属性，用于提供额外信息 |
| `predicate` | 布尔表达式，应评估为 `true`；表示必须成立的条件 |

### 关键字

`contract_assert` 是 C++26 引入的新关键字。

### 特性测试宏

```cpp
#if defined(__cpp_contracts) && __cpp_contracts >= 202502L
    // contract_assert 可用
#endif
```

| 宏名称 | 值 | 标准 | 特性 |
|--------|-----|------|------|
| `__cpp_contracts` | `202502L` | C++26 | 契约编程完整支持 |

## 4. 底层原理

### 执行语义

`contract_assert` 的执行流程：

```
1. 评估 predicate 表达式
   ├─ 若评估为 true → 正常继续执行
   ├─ 若评估为 false → 触发违约处理
   └─ 若评估抛出异常 → 触发违约处理
```

### 构建模式差异

| 构建模式 | 行为 |
|---------|------|
| **调试构建**（Debug Build） | 完整评估条件，条件为假或异常时触发违约处理 |
| **发布构建**（Release Build） | 可完全忽略断言检查，消除性能开销 |

### 与其他断言机制的对比

| 特性 | `contract_assert` | `assert()` | `[[assume]]` |
|------|-------------------|------------|--------------|
| 标准版本 | C++26 | C 标准库 | C++23 |
| 关键字/宏 | 关键字 | 宏 | 属性 |
| 调试时检查 | 是 | 是 | 否 |
| 发布时可移除 | 是 | 是 | N/A |
| 违约处理 | 可配置 | `abort()` | 未定义行为 |
| constexpr 支持 | 是 | 否 | 是 |
| 异常处理 | 违约 | 未定义行为 | 未定义行为 |

### 性能特征

- **零开销原则**：在发布构建中，`contract_assert` 不产生任何运行时开销
- **调试开销**：仅评估谓词表达式的成本
- **编译时影响**：可能增加编译时间（语义分析）

## 5. 使用场景

### 适合使用 contract_assert 的场景

| 场景 | 说明 |
|------|------|
| 函数内部不变量检查 | 验证算法执行过程中的内部状态 |
| 循环不变量 | 确保循环每次迭代的条件成立 |
| 中间结果验证 | 验证计算中间结果的合理性 |
| 调试辅助 | 开发阶段捕获逻辑错误 |
| 文档化假设 | 将假设条件显式记录在代码中 |

### 与前置/后置条件的区别

| 契约类型 | 位置 | 用途 |
|---------|------|------|
| `pre` | 函数声明 | 验证调用者传入参数的有效性 |
| `post` | 函数声明 | 验证函数返回结果的有效性 |
| `contract_assert` | 函数体内 | 验证内部执行过程中的条件 |

### 最佳实践

1. **验证内部不变量**：用于检查函数执行过程中的关键假设
2. **避免副作用**：谓词表达式应无副作用
3. **清晰的错误信息**：虽然 `contract_assert` 不直接支持消息参数，但可通过属性或注释补充说明
4. **与前置/后置条件配合**：构建完整的契约体系

### 注意事项

- **不要用于参数验证**：参数验证应使用 `pre` 前置条件
- **避免改变程序状态**：谓词不应修改任何状态
- **考虑性能影响**：调试构建中的频繁检查可能影响性能
- **编译器支持**：截至 C++26 标准，主流编译器支持仍在完善中

### 常见陷阱

1. **误用为前置条件**：应在函数声明中使用 `pre`
2. **谓词含副作用**：可能导致发布构建行为不一致
3. **依赖外部状态**：可能导致不可重复的违约

## 6. 代码示例

### 基础用法

```cpp
#include <cmath>
#include <array>
#include <concepts>

template<std::floating_point T>
constexpr auto normalize(std::array<T, 3> vector) noexcept
    pre(/* is_normalizable(vector) */)  // 前置条件
    post(/* vector: is_normalized(vector) */)  // 后置条件
{
    auto& [x, y, z] = vector;
    const auto norm = std::hypot(x, y, z);

    // 验证内部条件：范数必须是有限的正数
    contract_assert(std::isfinite(norm) && norm > T(0));

    x /= norm;
    y /= norm;
    z /= norm;

    return vector;
}
```

### 循环不变量检查

```cpp
#include <vector>
#include <algorithm>

// 在排序算法中验证循环不变量
void insertion_sort(std::vector<int>& data) {
    for (size_t i = 1; i < data.size(); ++i) {
        int key = data[i];
        size_t j = i;

        while (j > 0 && data[j - 1] > key) {
            data[j] = data[j - 1];
            --j;
        }
        data[j] = key;

        // 循环不变量：前 i+1 个元素已排序
        contract_assert(std::is_sorted(data.begin(), data.begin() + i + 1));
    }
}
```

### 中间结果验证

```cpp
#include <cmath>
#include <algorithm>

// 计算三角形的面积（海伦公式）
double triangle_area(double a, double b, double c)
    pre(a > 0 && b > 0 && c > 0)  // 前置条件：边长必须为正
    post(/* area > 0 */)  // 后置条件：面积必须为正
{
    double s = (a + b + c) / 2.0;

    // 中间条件：半周长必须大于最长边
    // 否则三角形不等式不成立
    contract_assert(s > std::max({a, b, c}));

    double area_sq = s * (s - a) * (s - b) * (s - c);

    // 中间条件：面积平方必须非负
    contract_assert(area_sq >= 0);

    return std::sqrt(area_sq);
}
```

### 常见错误及修正

#### 错误 1：误用为参数验证

```cpp
// ❌ 错误：参数验证不应使用 contract_assert
void process(int* ptr) {
    contract_assert(ptr != nullptr);  // 应使用前置条件
    // ...
}

// ✅ 修正：使用 pre 前置条件
void process(int* ptr)
    pre(ptr != nullptr)  // 正确的前置条件声明
{
    // ...
}
```

#### 错误 2：谓词含有副作用

```cpp
// ❌ 错误：谓词修改了状态
int counter = 0;
void process() {
    contract_assert(++counter > 0);  // 发布构建可能不执行
    // 调试和发布构建行为不一致
}

// ✅ 修正：谓词无副作用
int counter = 0;
void process() {
    contract_assert(counter >= 0);  // 仅检查，不修改
    ++counter;
}
```

#### 错误 3：忽略性能影响

```cpp
// ⚠️ 注意：调试构建中的复杂检查可能影响性能
void process_large_data(const std::vector<int>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        // 每次 O(n) 检查，总共 O(n²)
        contract_assert(std::find(data.begin(), data.end(), data[i]) != data.end());
    }
}

// ✅ 改进：仅在关键点检查
void process_large_data(const std::vector<int>& data) {
    // 循环前置检查一次
    contract_assert(!data.empty());

    for (size_t i = 0; i < data.size(); ++i) {
        // 简单的 O(1) 检查
        contract_assert(data[i] >= 0);
    }
}
```

## 7. 总结

`contract_assert` 是 C++26 契约编程特性的核心组件，提供了：

- **标准化断言语法**：语言级别支持，无需依赖宏
- **调试/发布分离**：调试时检查，发布时零开销
- **异常安全**：谓词异常触发违约处理而非未定义行为
- **与前置/后置条件的一致性**：统一的契约语义

### 核心要点

| 特性 | 说明 |
|------|------|
| 引入版本 | C++26 |
| 关键字 | `contract_assert` |
| 位置 | 函数体或 lambda 体内 |
| 用途 | 验证内部条件 |
| 发布时行为 | 可被忽略（零开销） |
| 调试时行为 | 条件为假或异常时触发违约处理 |

### 技术对比

| 特性 | `contract_assert` | `assert()` | `static_assert` | `[[assume]]` |
|------|-------------------|------------|-----------------|--------------|
| 检查时机 | 运行时 | 运行时 | 编译时 | 不检查 |
| 标准版本 | C++26 | C 标准库 | C++11 | C++23 |
| 可配置 | 是 | 否 | N/A | 否 |
| constexpr | 支持 | 不支持 | 支持 | 支持 |

### 使用建议

1. **用于内部不变量**：验证函数执行过程中的关键假设
2. **配合前置/后置条件**：构建完整的契约体系
3. **避免副作用**：谓词应保持纯函数特性
4. **关注性能**：调试构建中避免过于复杂的检查

### 参考资料

- C++26 标准 (ISO/IEC 14882:2026): 8.(7+c) Assertion statement [stmt.contract.assert]
- 提案 P2900R14: Contracts for C++
- cppreference: https://en.cppreference.com/w/cpp/language/contract_assert

## 参见

- `assert` - 如果条件不真则终止程序（函数宏）
- `static_assert` - 编译时断言检查（C++11）
- `pre`/`post` - 函数契约说明符（C++26）
- `[[assume]]` - 编译器优化假设（C++23）