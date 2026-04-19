# C++ 属性：likely, unlikely (C++20 起)

## 1. 概述 (Overview)

`[[likely]]` 和 `[[unlikely]]` 是 C++20 引入的两个属性（attribute），用于向编译器提供分支预测提示。它们允许程序员指示某条执行路径比其他路径更可能或更不可能被执行，从而帮助编译器进行更优化的代码生成。

这两个属性定义在 C++ 标准库中，无需包含任何头文件，属于语言内置属性。可以应用于标签（label）和语句（statement），但不能同时应用于同一个标签或语句。

### 核心作用

| 属性 | 作用 |
|------|------|
| `[[likely]]` | 指示包含该语句的执行路径比不包含该语句的其他执行路径**更可能**被执行 |
| `[[unlikely]]` | 指示包含该语句的执行路径比不包含该语句的其他执行路径**更不可能**被执行 |

## 2. 来源与演变 (Origin and Evolution)

### 首次引入

`[[likely]]` 和 `[[unlikely]]` 属性首次在 **C++20** 标准中引入。

### 历史背景

在 C++20 之前，程序员需要依赖以下非标准方式进行分支预测优化：

| 方式 | 说明 | 问题 |
|------|------|------|
| `__builtin_expect` (GCC/Clang) | 编译器内建函数 | 可移植性差，需要包装宏 |
| `__assume` (MSVC) | 微软编译器的假设指令 | 编译器特定 |
| `if (likely(cond))` 宏定义 | 社区常用模式 | 非标准，维护成本高 |
| 手动代码重排 | 调整 if-else 分支顺序 | 影响可读性 |
| PGO | Profile-Guided Optimization | 需要额外构建流程 |

C++20 标准化这两个属性解决了这些问题：
- 提供标准化的分支预测提示机制
- 消除对编译器特定扩展的依赖
- 提高代码可读性和可移植性

### 设计动机

1. **标准化分支预测提示**：提供跨编译器的统一语法
2. **提高代码可读性**：属性语法比宏定义更直观
3. **改善性能优化**：在热点代码中提供更好的分支预测

### 标准文档位置

| 标准 | 章节 |
|------|------|
| C++23 (ISO/IEC 14882:2024) | 9.12.7 Likelihood attributes [dcl.attr.likelihood] |
| C++20 (ISO/IEC 14882:2020) | 9.12.6 Likelihood attributes [dcl.attr.likelihood] |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
[[likely]]      // (1) 指示更可能的执行路径
[[unlikely]]    // (2) 指示更不可能的执行路径
```

### 适用位置

| 位置 | 说明 | 示例 |
|------|------|------|
| 语句（statement） | if 分支、循环体等 | `if (cond) [[likely]] { ... }` |
| 标签（label） | switch case 标签 | `[[likely]] case 1: ...` |

### 使用限制

1. **不能应用于声明语句（declaration-statement）**
2. **`[[likely]]` 和 `[[unlikely]]` 不能同时应用于同一个标签或语句**

### 语义说明

1. **`[[likely]]`**：应用于语句时，表示包含该语句的执行路径比不包含该语句的任何其他执行路径更可能被执行。

2. **`[[unlikely]]`**：应用于语句时，表示包含该语句的执行路径比不包含该语句的任何其他执行路径更不可能被执行。

### 标签的特殊语义

执行路径被视为包含某个标签，**当且仅当**它包含跳转到该标签的语句：

```cpp
int f(int i) {
    switch (i) {
        case 1: [[fallthrough]];
        [[likely]] case 2: return 1;  // 只有直接跳转到 case 2 才受影响
    }
    return 2;
}
```

在上述示例中，`i == 2` 被认为比 `i` 的其他值更可能发生，但 `[[likely]]` 对 `i == 1` 的情况没有影响，即使它落空（fallthrough）到了 `case 2:` 标签。

### 语法示例

```cpp
// 应用于 if 语句
if (condition) [[likely]] {
    // 更可能执行的代码
}

// 应用于 else 分支
if (condition) {
    // ...
} else [[unlikely]] {
    // 更不可能执行的代码
}

// 应用于 switch case
switch (value) {
    [[likely]] case 1:  // case 1 更可能发生
        // ...
        break;
    [[unlikely]] case 0:  // case 0 不太可能发生
        // ...
        break;
}

// 应用于循环
for (int i = 0; i < n; ++i) [[likely]] {
    // 循环体更可能执行
}
```

## 4. 底层原理 (Underlying Principles)

### 分支预测优化

现代 CPU 使用**流水线技术（Pipeline）**执行指令，分支预测（Branch Prediction）是提高性能的关键技术：

```
CPU 执行流水线:
┌─────────────────────────────────────────────────────┐
│  取指 → 解码 → 执行 → 访存 → 写回                      │
└─────────────────────────────────────────────────────┘
         ↑
    分支预测器猜测分支方向
```

当遇到条件分支时，CPU 需要预测执行哪条路径：

| 预测结果 | 性能影响 |
|----------|---------|
| 预测正确 | 流水线继续执行，无性能损失 |
| 预测错误 | 流水线清空，重新取指，造成 10-20 个时钟周期损失 |

### 编译器优化策略

当编译器收到 `[[likely]]` 或 `[[unlikely]]` 提示后，可以进行以下优化：

| 优化策略 | 说明 |
|----------|------|
| **指令布局优化** | 将更可能执行的代码放在连续的内存位置，提高指令缓存命中率 |
| **分支指令生成** | 生成更有利于 CPU 预测的分支指令（如 x86 的分支提示前缀） |
| **函数内联决策** | 影响内联决策，优先内联热路径上的函数 |
| **基本块重排** | 减少跳转指令，使热路径代码更紧凑 |

### 性能特征

| 特征 | 说明 |
|------|------|
| **编译时开销** | 无额外编译开销 |
| **运行时开销** | 零运行时开销（纯编译时提示） |
| **预期效果** | 分支预测命中率提高，流水线效率提升 |

### 与传统方法对比

| 方法 | 可移植性 | 语法简洁性 | 标准化 |
|------|----------|------------|--------|
| `[[likely]]`/`[[unlikely]]` | 高（标准 C++20） | 简洁 | 是 |
| `__builtin_expect` | 低（GCC/Clang 特有） | 复杂 | 否 |
| PGO | 高 | 需额外流程 | 是 |

## 5. 使用场景 (Use Cases)

### 适合使用的场景

| 场景 | 推荐属性 | 说明 |
|------|---------|------|
| 错误处理分支 | `[[unlikely]]` | 错误通常较少发生 |
| 边界条件处理 | `[[unlikely]]` | 边界条件较少触发 |
| 主逻辑流程 | `[[likely]]` | 正常执行路径更常见 |
| 热点代码优化 | 视情况 | 循环内的条件判断 |

### 最佳实践

#### 1. 用于错误处理代码

```cpp
// 错误处理通常是不常见的路径
if (ptr == nullptr) [[unlikely]] {
    handle_error();
    return;
}
// 主逻辑
process(ptr);
```

#### 2. 基于实际数据使用

```cpp
// ✅ 正确使用：基于性能分析数据
if (data.is_valid()) [[likely]] {  // 根据分析，95% 的数据是有效的
    process(data);
}
```

#### 3. 配合 switch-case 使用

```cpp
switch (status) {
    [[likely]] case OK:           // 正常状态更常见
        handle_ok();
        break;
    case WARNING:
        handle_warning();
        break;
    [[unlikely]] case ERROR:      // 错误状态较少见
        handle_error();
        break;
}
```

### 注意事项

1. **不要过度使用**：现代编译器和 CPU 已有复杂的预测机制，大多数情况下自动预测效果很好
2. **避免猜测**：只在确定分支概率时使用，错误的提示可能适得其反
3. **配合性能分析**：使用性能分析工具确定热点代码后再优化
4. **代码可读性**：优先保证代码清晰，性能优化放在确定瓶颈后
5. **编译器可能忽略提示**：编译器有权忽略这些属性
6. **属性不改变程序语义**：只是优化提示，不影响程序正确性

### 常见陷阱

| 陷阱 | 说明 | 修正 |
|------|------|------|
| 错误的分支概率估计 | 错误的提示比没有提示更糟 | 使用性能分析工具验证 |
| 在冷代码中使用 | 微优化效果微乎其微 | 只在热点代码中使用 |
| 与编译器优化冲突 | 可能干扰编译器的自动优化 | 谨慎使用，测试效果 |
| 应用于声明语句 | 编译错误 | 应用于语句或标签 |

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>
#include <vector>

// 使用 [[likely]] 和 [[unlikely]] 进行分支预测提示
void process_data(const std::vector<int>& data) {
    // 空数据是罕见情况
    if (data.empty()) [[unlikely]] {
        std::cout << "Warning: empty data\n";
        return;
    }

    // 有效数据处理是常见情况
    if (data.size() > 0) [[likely]] {
        for (int value : data) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2;  // 空

    process_data(v1);  // 触发 [[likely]] 路径
    process_data(v2);  // 触发 [[unlikely]] 路径

    return 0;
}
```

### 高级用法：性能对比

以下示例展示了 `[[likely]]`/`[[unlikely]]` 对性能的影响：

```cpp
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

namespace with_attributes {
    constexpr double pow(double x, long long n) noexcept {
        if (n > 0) [[likely]]
            return x * pow(x, n - 1);
        else [[unlikely]]
            return 1;
    }

    constexpr long long fact(long long n) noexcept {
        if (n > 1) [[likely]]
            return n * fact(n - 1);
        else [[unlikely]]
            return 1;
    }

    constexpr double cos(double x) noexcept {
        constexpr long long precision{16LL};
        double y{};
        for (auto n{0LL}; n < precision; n += 2LL) [[likely]]
            y += pow(x, n) / (n & 2LL ? -fact(n) : fact(n));
        return y;
    }
} // namespace with_attributes

namespace no_attributes {
    constexpr double pow(double x, long long n) noexcept {
        if (n > 0)
            return x * pow(x, n - 1);
        else
            return 1;
    }

    constexpr long long fact(long long n) noexcept {
        if (n > 1)
            return n * fact(n - 1);
        else
            return 1;
    }

    constexpr double cos(double x) noexcept {
        constexpr long long precision{16LL};
        double y{};
        for (auto n{0LL}; n < precision; n += 2LL)
            y += pow(x, n) / (n & 2LL ? -fact(n) : fact(n));
        return y;
    }
} // namespace no_attributes

double gen_random() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(-1.0, 1.0);
    return dis(gen);
}

volatile double sink{}; // 确保有副作用，防止优化消除

int main() {
    for (const auto x : {0.125, 0.25, 0.5, 1. / (1 << 26)})
        std::cout << std::setprecision(53)
                  << "x = " << x << '\n'
                  << std::cos(x) << '\n'
                  << with_attributes::cos(x) << '\n'
                  << (std::cos(x) == with_attributes::cos(x) ? "equal" : "differ")
                  << '\n';

    auto benchmark = [](auto fun, auto rem) {
        const auto start = std::chrono::high_resolution_clock::now();
        for (auto size{1ULL}; size != 10'000'000ULL; ++size)
            sink = fun(gen_random());
        const std::chrono::duration<double> diff =
            std::chrono::high_resolution_clock::now() - start;
        std::cout << "Time: " << std::fixed << std::setprecision(6)
                  << diff.count() << " sec " << rem << std::endl;
    };

    benchmark(with_attributes::cos, "(with attributes)");
    benchmark(no_attributes::cos, "(without attributes)");
    benchmark([](double t) { return std::cos(t); }, "(std::cos)");
}
```

可能的输出：
```
x = 0.125
0.99219766722932900560039115589461289346218109130859375
0.99219766722932900560039115589461289346218109130859375
equal
x = 0.25
0.96891242171064473343022882545483298599720001220703125
0.96891242171064473343022882545483298599720001220703125
equal
x = 0.5
0.8775825618903727587394314468838274478912353515625
0.8775825618903727587394314468838274478912353515625
equal
x = 1.490116119384765625e-08
0.99999999999999988897769753748434595763683319091796875
0.99999999999999988897769753748434595763683319091796875
equal
Time: 0.579122 sec (with attributes)
Time: 0.722553 sec (without attributes)
Time: 0.425963 sec (std::cos)
```

### switch 语句中的应用

```cpp
#include <iostream>

// 标签上的属性使用示例
int process_input(int i) {
    switch (i) {
        case 1: [[fallthrough]];
        [[likely]] case 2: return 1;  // case 2 更可能被直接跳转
        [[unlikely]] case 0: return 0; // case 0 不太可能发生
        default: return -1;
    }
}

int main() {
    std::cout << process_input(2) << "\n";  // 输出: 1
    std::cout << process_input(1) << "\n";  // 输出: 1 (fallthrough)
    std::cout << process_input(0) << "\n"; // 输出: 0
    return 0;
}
```

**注意**：在上面的例子中，`[[likely]]` 只影响直接跳转到 `case 2` 的情况，不影响从 `case 1` fallthrough 过来的情况。

### 常见错误及修正

#### 错误 1：应用于声明语句

```cpp
// ❌ 错误：不能应用于声明语句
[[likely]] int x = 10;  // 编译错误

// ✅ 修正：应用于包含声明的复合语句
if (condition) [[likely]] {
    int x = 10;
}
```

#### 错误 2：同时使用两个属性

```cpp
// ❌ 错误：不能同时使用两个属性
if (condition) [[likely]] [[unlikely]] {  // 编译错误
    // ...
}

// ✅ 修正：只使用一个属性
if (condition) [[likely]] {
    // ...
}
```

#### 错误 3：错误的分支概率估计

```cpp
// ❌ 错误：与实际执行概率相反
bool is_error = check_error();
if (is_error) [[likely]] {  // 错误！通常不是错误
    handle_error();
}

// ✅ 修正：根据实际概率使用
if (is_error) [[unlikely]] {  // 正确：错误是罕见的
    handle_error();
}
```

#### 错误 4：在冷代码中使用

```cpp
// ⚠️ 问题：在只执行一次的初始化代码中使用
void init() {
    if (needs_init) [[likely]] {  // 无意义：只执行一次
        do_init();
    }
}

// ✅ 修正：在热点代码中使用
void process_hot_path(int value) {
    for (int i = 0; i < 1000000; ++i) {
        if (value > 0) [[likely]] {  // 有意义：执行一百万次
            process(value);
        }
    }
}
```

## 7. 总结 (Summary)

### 核心要点

`[[likely]]` 和 `[[unlikely]]` 属性是 C++20 提供的标准化分支预测提示工具：

| 特性 | 说明 |
|------|------|
| **引入版本** | C++20 |
| **作用** | 向编译器提供分支概率提示 |
| **语法** | `[[likely]]` / `[[unlikely]]` |
| **应用位置** | 语句和标签 |
| **运行时开销** | 零开销（纯编译时提示） |
| **可移植性** | 标准 C++，跨平台 |

### 使用建议

1. **优先使用标准属性**：C++20 及以上版本优先使用 `[[likely]]` 和 `[[unlikely]]`
2. **结合性能分析**：先确定热点代码，再应用属性优化
3. **谨慎估计概率**：只在确定分支概率时使用
4. **保持代码清晰**：不要为了微优化牺牲代码可读性
5. **仅在性能关键路径使用**：普通代码不需要分支预测提示

### 技术对比

| 特性 | `[[likely]]`/`[[unlikely]]` | `__builtin_expect` | PGO |
|------|---------------------------|-------------------|-----|
| 标准 | C++20 | GCC/Clang 扩展 | 工具链支持 |
| 可移植性 | 高 | 低 | 中 |
| 使用复杂度 | 低 | 中 | 高 |
| 精确度 | 手动标注 | 手动标注 | 自动分析 |

### 相关概念

| 概念 | 关系 |
|------|------|
| 分支预测（Branch Prediction） | 属性优化的底层机制 |
| 指令缓存（Instruction Cache） | 属性影响代码布局 |
| `[[fallthrough]]` | 另一个控制流相关属性 |
| Profile-Guided Optimization (PGO) | 更高级的分支优化技术 |

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.12.7 Likelihood attributes [dcl.attr.likelihood]
- C++20 标准 (ISO/IEC 14882:2020): 9.12.6 Likelihood attributes [dcl.attr.likelihood]
- cppreference: https://en.cppreference.com/w/cpp/language/attributes/likely