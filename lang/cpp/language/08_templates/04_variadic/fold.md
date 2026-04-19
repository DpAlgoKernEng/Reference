# Fold expressions - 折叠表达式 (C++17)

## 1. 概述

**折叠表达式**（Fold expression）是 C++17 引入的一种语言特性，用于简化可变参数模板（variadic templates）中的参数包（parameter pack）展开操作。它允许通过二元运算符将参数包中的所有元素"折叠"成一个单一值，类似于函数式编程中的折叠（fold）或归约（reduce）操作。

折叠表达式定义在 C++ 语言核心中，无需包含任何头文件。

## 2. 来源与演变

### 首次引入

折叠表达式首次在 **C++17** 标准中引入，提案编号为 N4295 和 P0036R0。

### 历史背景

在 C++17 之前，处理可变参数模板需要进行复杂的递归展开：

```cpp
// C++14 及之前：需要递归模板
template<typename T>
T sum(T value) {
    return value;
}

template<typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);  // 递归调用
}
```

折叠表达式的出现解决了这些问题：
- 消除了递归模板的复杂性
- 提供了更直观、更简洁的语法
- 减少了编译时间（无需实例化多层递归）

### 相关特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|-----------|-----|------|------|
| `__cpp_fold_expressions` | `201603L` | C++17 | 折叠表达式 |

### 缺陷报告

| DR | 应用版本 | 已发布行为 | 修正行为 |
|----|---------|-----------|---------|
| CWG 2611 | C++17 | 折叠表达式的展开结果未用括号包围 | 用括号包围 |

## 3. 语法与参数

### 基本语法形式

折叠表达式共有四种形式：

| 形式 | 语法 | 名称 |
|------|------|------|
| (1) | `( pack op ... )` | 一元右折叠（Unary right fold） |
| (2) | `( ... op pack )` | 一元左折叠（Unary left fold） |
| (3) | `( pack op ... op init )` | 二元右折叠（Binary right fold） |
| (4) | `( init op ... op pack )` | 二元左折叠（Binary left fold） |

### 参数说明

| 参数 | 说明 |
|------|------|
| `op` | 以下 32 种二元运算符之一：`+` `-` `*` `/` `%` `^` `&` `|` `=` `<` `>` `<<` `>>` `+=` `-=` `*=` `/=` `%=` `^=` `&=` `|=` `<<=` `>>=` `==` `!=` `<=` `>=` `&&` `||` `,` `.*` `->*`。在二元折叠中，两个运算符必须相同。 |
| `pack` | 包含未展开参数包的表达式，顶层不能包含优先级低于转换运算符的运算符（形式上是一个 cast-expression） |
| `init` | 不包含未展开参数包的表达式，顶层不能包含优先级低于转换运算符的运算符（形式上是一个 cast-expression） |

**重要**：折叠表达式两端的括号是必需的，不可省略。

### 展开规则

设参数包 `E` 包含 `N` 个元素 `E1, E2, ..., EN`，折叠表达式展开如下：

| 形式 | 展开结果 |
|------|---------|
| 一元右折叠 `(E op ...)` | `(E1 op (E2 op ... (EN-1 op EN)))` |
| 一元左折叠 `(... op E)` | `(((E1 op E2) op ...) op EN)` |
| 二元右折叠 `(E op ... op I)` | `(E1 op (E2 op ... (EN op I)))` |
| 二元左折叠 `(I op ... op E)` | `(((I op E1) op E2) op ...) op EN)` |

### 空包特殊规则

当一元折叠用于长度为零的参数包时，仅以下运算符允许使用：

| 运算符 | 空包值 |
|--------|--------|
| 逻辑与 `&&` | `true` |
| 逻辑或 `||` | `false` |
| 逗号 `,` | `void()` |

## 4. 底层原理

### 编译时展开

折叠表达式在编译时展开，而非运行时。编译器将折叠表达式转换为嵌套的表达式树，每个参数包元素作为一个节点参与运算。

### 与递归模板的对比

| 特性 | 折叠表达式 | 递归模板 |
|------|-----------|---------|
| 编译复杂度 | O(n) 实例化 | O(n) 层实例化 |
| 代码可读性 | 高 | 低 |
| 编译时间 | 较短 | 较长 |
| 调试难度 | 简单 | 复杂（深层调用栈） |
| 表达式形式 | 嵌套 | 递归调用 |

### 运算顺序

折叠表达式的展开结果保证用括号包围，这确保了正确的运算顺序和优先级：

```cpp
// 一元右折叠
(args + ...) // 展开为 (a1 + (a2 + (a3 + a4)))

// 一元左折叠
(... + args) // 展开为 (((a1 + a2) + a3) + a4)
```

### 求值保证

对于大多数运算符（如 `+`, `*`, `&&`, `||`），折叠表达式严格按照从左到右或从右到左的顺序求值：

- **左折叠**：从左向右依次求值
- **右折叠**：从右向左依次求值

## 5. 使用场景

### 适合使用折叠表达式的场景

| 场景 | 示例 |
|------|------|
| 可变参数求和 | `(args + ...)` |
| 可变参数逻辑判断 | `(... && args)` |
| 可变参数打印 | `(std::cout << ... << args)` |
| 编译时断言检查 | `static_assert((condition && ...))` |
| 顺序执行多个操作 | `(func(args), ...)` |

### 不适合使用折叠表达式的场景

| 场景 | 推荐替代 | 原因 |
|------|---------|------|
| 需要对每个参数执行不同操作 | `if constexpr` + 递归 | 折叠表达式对每个参数执行相同操作 |
| 需要访问参数索引 | 索引序列 + 折叠 | 需要额外工具 |
| 需要提前终止 | 递归模板 | 折叠表达式无法提前终止 |

### 最佳实践

1. **优先使用左折叠**：大多数情况下更符合直觉的求值顺序
2. **合理选择运算符**：根据语义选择合适的运算符
3. **注意空包情况**：使用 `&&` 或 `||` 时考虑空包的默认值
4. **使用初始值**：需要默认值或累积起点时使用二元折叠

### 运算符优先级注意事项

如果作为 `init` 或 `pack` 的表达式顶层包含优先级低于转换运算符的运算符，必须用括号包围：

```cpp
// 错误：运算符优先级问题
return (args + ... + 1 * 2);   // 错误：* 优先级低于 +，解析为 (args + ... + 1) * 2

// 正确：用括号包围
return (args + ... + (1 * 2)); // OK
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <vector>

// 基础示例：折叠可变参数
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // 一元右折叠
}

// 逻辑判断：检查所有参数是否为真
template<typename... Args>
bool all(Args... args) {
    return (... && args);  // 一元左折叠
}

// 逻辑判断：检查是否存在真值
template<typename... Args>
bool any(Args... args) {
    return (... || args);  // 一元左折叠
}

int main() {
    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << std::endl;  // 输出: 15
    std::cout << "All true: " << all(true, true, true) << std::endl;  // 输出: 1
    std::cout << "Any true: " << any(false, false, true) << std::endl;  // 输出: 1
    std::cout << "Empty all: " << all() << std::endl;  // 输出: 1 (空包默认 true)
    std::cout << "Empty any: " << any() << std::endl;  // 输出: 0 (空包默认 false)
    return 0;
}
```

### 高级用法

```cpp
#include <climits>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

// 打印可变参数（使用二元左折叠）
template<typename... Args>
void printer(Args&&... args) {
    (std::cout << ... << args) << '\n';
}

// 打印类型限制（使用一元右折叠）
template<typename... Ts>
void print_limits() {
    ((std::cout << +std::numeric_limits<Ts>::max() << ' '), ...) << '\n';
}

// 带编译时检查的容器填充
template<typename T, typename... Args>
void push_back_vec(std::vector<T>& v, Args&&... args) {
    // 编译时检查所有类型都可构造
    static_assert((std::is_constructible_v<T, Args&&> && ...));
    // 顺序执行 push_back
    (v.push_back(std::forward<Args>(args)), ...);
}

// 使用索引序列执行表达式 N 次
template<class T, std::size_t... dummy_pack>
constexpr T bswap_impl(T i, std::index_sequence<dummy_pack...>) {
    T low_byte_mask = static_cast<unsigned char>(-1);
    T ret{};
    ([&] {
        (void)dummy_pack;
        ret <<= CHAR_BIT;
        ret |= i & low_byte_mask;
        i >>= CHAR_BIT;
    }(), ...);  // Lambda 折叠
    return ret;
}

constexpr auto bswap(std::unsigned_integral auto i) {
    return bswap_impl(i, std::make_index_sequence<sizeof(i)>{});
}

int main() {
    // 打印示例
    printer(1, 2, 3, "abc");  // 输出: 123abc

    // 类型限制示例
    print_limits<uint8_t, uint16_t, uint32_t>();

    // 带检查的容器填充
    std::vector<int> v;
    push_back_vec(v, 6, 2, 45, 12);
    push_back_vec(v, 1, 2, 9);
    for (int i : v) std::cout << i << ' ';
    std::cout << '\n';

    // 字节交换
    static_assert(bswap<std::uint16_t>(0x1234u) == 0x3412u);
    static_assert(bswap<std::uint64_t>(0x0123456789abcdefull) == 0xefcdab8967452301ULL);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：忘记括号

```cpp
// 错误：折叠表达式必须用括号包围
template<typename... Args>
auto sum(Args... args) {
    return args + ...;  // 编译错误
}

// 正确：使用括号
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // OK
}
```

#### 错误 2：空包使用非法运算符

```cpp
// 错误：空包使用 + 运算符无定义
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // 空包调用时编译错误
}

sum();  // 错误：空包无默认值

// 正确：使用二元折叠提供初始值
template<typename... Args>
auto sum(Args... args) {
    return (args + ... + 0);  // 空包返回 0
}

sum();  // OK，返回 0
```

#### 错误 3：运算符优先级问题

```cpp
// 错误：表达式顶层包含低优先级运算符
template<typename... Args>
int sum(Args&&... args) {
    return (args + ... + 1 * 2);  // 错误：解析为 ((args + ...) + 1) * 2
}

// 正确：用括号包围表达式
template<typename... Args>
int sum(Args&&... args) {
    return (args + ... + (1 * 2));  // OK
}
```

#### 错误 4：二元折叠运算符不一致

```cpp
// 错误：二元折叠的两个运算符必须相同
template<typename... Args>
auto bad_fold(Args... args) {
    return (args + ... - 0);  // 编译错误
}

// 正确：使用相同运算符
template<typename... Args>
auto good_fold(Args... args) {
    return (args + ... + 0);  // OK
}
```

## 注意事项

1. **必须使用括号**：折叠表达式的开闭括号是语法的一部分
2. **空包限制**：只有 `&&`、`||`、`,` 运算符支持一元空折叠
3. **运算符一致性**：二元折叠的两个运算符必须相同
4. **优先级问题**：低优先级运算符需要额外括号
5. **求值顺序**：左折叠从左到右，右折叠从右到左

## 相关概念

| 概念 | 关系 |
|------|------|
| 可变参数模板（Variadic templates） | 折叠表达式是处理参数包的工具 |
| 参数包展开（Pack expansion） | 折叠表达式是一种特殊的展开方式 |
| `std::index_sequence` | 常与折叠表达式配合使用 |
| `std::apply` | 另一种处理参数包的方式 |
| `if constexpr` | 可与折叠表达式结合进行编译时分支 |

## 7. 总结

折叠表达式是 C++17 中处理可变参数模板的重要工具，它提供了：

- **简洁的语法**：一行代码替代多层递归模板
- **四种折叠形式**：一元/二元、左/右折叠满足不同需求
- **编译时展开**：零运行时开销
- **类型安全**：编译时检查所有操作

核心使用建议：
1. 需要累积值时优先使用二元折叠（提供初始值）
2. 逻辑运算使用 `&&` 和 `||`（空包有明确定义）
3. 左折叠更符合直觉求值顺序，推荐优先使用
4. 复杂表达式用括号明确优先级

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/fold
- C++23 标准 (ISO/IEC 14882:2024): 7.5.6 Fold expressions [expr.prim.fold]
- C++20 标准 (ISO/IEC 14882:2020): 7.5.6 Fold expressions [expr.prim.fold]
- C++17 标准 (ISO/IEC 14882:2017): 8.1.6 Fold expressions [expr.prim.fold]
- 提案 N4295: Folding expressions