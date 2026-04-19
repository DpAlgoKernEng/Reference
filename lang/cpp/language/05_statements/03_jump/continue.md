# continue 语句 (continue statement)

## 1. 概述 (Overview)

`continue` 语句是 C++ 中的流程控制语句（flow control statement），用于跳过当前循环迭代的剩余部分，直接进入下一次迭代。它可以用于 for、range-for、while 和 do-while 循环中。

### 核心概念

- **跳过剩余代码**：跳过循环体中 continue 之后的所有语句
- **继续迭代**：不退出循环，直接进入下一次迭代
- **四种循环通用**：适用于 for、range-for、while、do-while 循环

### 技术定位

continue 语句属于 C++ 的**跳转语句**（jump statements）类别，用于简化循环控制逻辑，避免深层嵌套的条件判断。它提供了一种比使用条件语句来忽略循环剩余部分更优雅的方式。

### 主要用途

当在循环中遇到某些条件，需要跳过当前迭代的剩余代码但继续后续迭代时，continue 提供了一个简洁的解决方案，无需将剩余代码包裹在 else 块中。

## 2. 来源与演变 (Origin and Evolution)

### 首次引入

`continue` 语句从 **C 语言** 继承而来，在 C 语言时期就已存在。它作为结构化程序设计（structured programming）的一部分，提供了比 goto 语句更安全、更可控的跳转机制。

### 历史背景

在 continue 出现之前，跳过循环迭代的方式包括：

1. **使用 goto 语句**：需要手动定义标签，容易出错且难以维护
2. **深层嵌套条件**：导致代码缩进层次过多，可读性差

continue 语句的引入解决了这些问题：
- 提供语义明确的跳转
- 限制在循环体内部使用，避免滥用
- 提高代码可读性和维护性

### 版本变更

| 标准 | 主要变更 |
|------|----------|
| C++98 | 继承 C 语言 continue 语义 |
| C++11 | 新增属性（`attr`）支持，支持 range-for 循环 |
| C++17 | 允许 continue 语句携带属性，如 `[[likely]]`、`[[unlikely]]` |

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```cpp
attr(可选) continue ;
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr` | (C++11 起) 任意数量的属性，如 `[[likely]]`、`[[unlikely]]` |

### 使用限制

- **只能出现在循环体内**：for、range-for、while、do-while 循环
- **不能出现在循环体外**：会导致编译错误
- **与 switch 语句无关**：在 switch 中直接使用 continue 是错误的（除非 switch 嵌套在循环中）

### 语法示例

```cpp
// 基本形式
for (int i = 0; i < 10; ++i) {
    if (i == 5) continue;  // 跳过 i == 5 的情况
    std::cout << i << " ";
}

// C++17: 带属性的形式
for (int i = 0; i < 100; ++i) {
    if (rare_condition(i)) [[unlikely]] continue;
    // 处理正常情况
}
```

## 4. 底层原理 (Underlying Principles)

### 编译器实现机制

`continue` 语句在编译后会被转换为跳转指令，类似于 `goto` 的实现。编译器会：

1. 识别 continue 所在的循环类型
2. 根据循环类型确定跳转目标
3. 生成相应的跳转指令（如汇编中的 `jmp`）

### 在不同循环中的行为详解

**while 循环中的 continue**：

```cpp
while (/* ... */)
{
   // ...
   continue; // acts as goto contin;
   // ...
   contin:;
}
// 之后会重新判断 while 条件
```

**do-while 循环中的 continue**：

```cpp
do
{
    // ...
    continue; // acts as goto contin;
    // ...
    contin:;
} while (/* ... */);
// 之后会判断 while 条件
```

**for 和 range-for 循环中的 continue**：

```cpp
for (/* ... */)
{
   // ...
   continue; // acts as goto contin;
   // ...
   contin:;
}
// 之后会执行迭代表达式，再判断条件
```

### 关键区别

| 循环类型 | continue 后执行 | 说明 |
|----------|-----------------|------|
| while | 条件表达式 | 直接跳到循环顶部判断条件 |
| do-while | 条件表达式 | 跳到循环底部判断条件 |
| for | 迭代表达式 → 条件表达式 | 先执行迭代表达式，再判断条件 |
| range-for | 下一个元素的迭代 | 继续遍历下一个元素 |

**重要**：在 for 循环中，continue 会跳转到迭代表达式**之前**，这意味着迭代表达式仍会被执行。

### 性能特征

| 方面 | 说明 |
|------|------|
| 时间复杂度 | O(1) - 单次跳转指令 |
| 空间开销 | 无额外内存分配 |
| 编译优化 | 通常被编译为无条件跳转，效率极高 |
| 分支预测 | 可能影响 CPU 分支预测，但影响极小 |

### 汇编层面示例

```cpp
// C++ 代码
for (int i = 0; i < 10; ++i) {
    if (i == 5) continue;
    // do something
}

// 可能的汇编伪代码
// loop_start:
//     cmp i, 5
//     je skip_body      ; i == 5 时跳过
//     ; do something
// skip_body:
//     inc i
//     cmp i, 10
//     jl loop_start
```

## 5. 使用场景 (Use Cases)

### 适合使用 continue 的场景

| 场景 | 说明 |
|------|------|
| 过滤特定条件 | 当某些数据不满足处理条件时跳过 |
| 输入验证 | 跳过无效输入，继续处理后续数据 |
| 错误恢复 | 遇到非致命错误时跳过当前项 |
| 简化嵌套条件 | 避免深层 if-else 嵌套 |

### 最佳实践

#### 实践 1：替代深层嵌套

```cpp
// ❌ 不推荐：深层嵌套
for (int i = 0; i < n; ++i) {
    if (condition1(i)) {
        if (condition2(i)) {
            if (condition3(i)) {
                // 实际处理逻辑
            }
        }
    }
}

// ✅ 推荐：使用 continue 早返回
for (int i = 0; i < n; ++i) {
    if (!condition1(i)) continue;
    if (!condition2(i)) continue;
    if (!condition3(i)) continue;
    // 实际处理逻辑
}
```

#### 实践 2：快速过滤

```cpp
// ✅ 推荐：快速过滤无效数据
for (const auto& item : items) {
    if (!item.is_valid()) continue;
    if (item.is_deleted()) continue;
    // 只处理有效且未删除的数据
    process(item);
}
```

### 常见陷阱

#### 陷阱 1：在嵌套循环中的误用

```cpp
// ⚠️ 注意：continue 只影响最内层循环
for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 5; ++j) {
        if (j == 2) continue;  // 只跳过 j 的当前迭代
        std::cout << "(" << i << "," << j << ") ";
    }
}
// 输出：(0,0) (0,1) (0,3) (0,4) (1,0) (1,1) (1,3) (1,4) ...
// 注意：i 不受影响，每个 i 都完整执行了内层循环
```

#### 陷阱 2：误解 for 循环中的迭代执行

```cpp
// ✅ 正确理解：continue 后仍会执行迭代表达式
for (int i = 0; i < 10; ++i) {
    if (i == 5) continue;  // i == 5 后仍会执行 ++i
    std::cout << i << " ";
}
// 输出：0 1 2 3 4 6 7 8 9
// 注意：5 被跳过，但 i 仍递增到 6
```

#### 陷阱 3：while 循环中跳过更新语句

```cpp
// ❌ 错误：可能导致无限循环
int i = 0;
while (i < 10) {
    if (i == 5) continue;  // 跳过了 ++i！
    std::cout << i << " ";
    ++i;
}
// 无限循环：当 i == 5 时，continue 跳过了 ++i

// ✅ 修正：将更新放在 continue 之前
int i = 0;
while (i < 10) {
    ++i;  // 更新放在前面
    if (i == 5) continue;
    std::cout << i << " ";
}
```

#### 陷阱 4：在 switch 中误用

```cpp
// ❌ 错误：switch 不是循环，不能使用 continue
int x = 2;
switch (x) {
    case 1:
        // do something
        break;
    case 2:
        // continue;  // 编译错误！
        break;
}

// ✅ 正确：switch 嵌套在循环中时可使用
for (int i = 0; i < 10; ++i) {
    switch (i) {
        case 5:
            continue;  // 正确：跳过 i == 5 的循环迭代
        default:
            break;
    }
}
```

### 线程安全性

`continue` 语句本身是线程安全的，不涉及共享数据访问。但在多线程循环中使用时，需注意：

- continue 不影响循环本身的线程安全性
- 如果循环体内访问共享数据，仍需适当的同步机制

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

int main()
{
    // 示例 1：跳过特定值
    for (int i = 0; i < 10; ++i)
    {
        if (i != 5)
            continue;
        std::cout << i << ' ';      // this statement is skipped each time i != 5
    }
    std::cout << '\n';

    // 示例 2：嵌套循环中的 continue
    for (int j = 0; 2 != j; ++j)
        for (int k = 0; k < 5; ++k) // only this loop is affected by continue
        {
            if (k == 3)
                continue;
            // this statement is skipped each time k == 3:
            std::cout << '(' << j << ',' << k << ") ";
        }
    std::cout << '\n';

    return 0;
}
```

**输出：**
```
5
(0,0) (0,1) (0,2) (0,4) (1,0) (1,1) (1,2) (1,4)
```

### 简化嵌套结构

```cpp
#include <iostream>
#include <vector>

// 不使用 continue：深层嵌套
void process_without_continue(const std::vector<int>& v)
{
    for (int n : v)
    {
        if (n > 0)
        {
            if (n % 2 == 0)
            {
                std::cout << n << " is positive even\n";
            }
        }
    }
}

// 使用 continue：扁平结构
void process_with_continue(const std::vector<int>& v)
{
    for (int n : v)
    {
        if (n <= 0)
            continue;
        if (n % 2 != 0)
            continue;
        std::cout << n << " is positive even\n";
    }
}

int main()
{
    std::vector<int> v = {2, -1, 4, 3, -5, 6};
    process_with_continue(v);
    return 0;
}
```

**输出：**
```
2 is positive even
4 is positive even
6 is positive even
```

### 高级用法

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    // 示例：处理复杂数据结构
    struct Person {
        std::string name;
        int age;
        bool is_active;
    };

    std::vector<Person> people = {
        {"Alice", 25, true},
        {"Bob", 30, false},
        {"Charlie", 35, true},
        {"Diana", 28, false},
        {"Eve", 22, true}
    };

    std::cout << "活跃用户:\n";
    for (const auto& p : people) {
        if (!p.is_active) continue;  // 跳过非活跃用户
        if (p.age < 18) continue;     // 跳过未成年
        std::cout << "  " << p.name << " (" << p.age << "岁)\n";
    }

    return 0;
}
```

**输出：**
```
活跃用户:
  Alice (25岁)
  Charlie (35岁)
  Eve (22岁)
```

### 常见错误及修正

#### 错误 1：while 循环中的无限循环

```cpp
// ❌ 错误：continue 导致无限循环
int i = 0;
while (i < 10) {
    if (i == 5) continue;  // 跳过了 ++i！
    std::cout << i << " ";
    ++i;
}

// ✅ 修正：将更新放在 continue 之前
int i = 0;
while (i < 10) {
    ++i;
    if (i == 5) continue;
    std::cout << i << " ";
}
```

#### 错误 2：混淆 continue 和 break

```cpp
#include <iostream>

int main() {
    std::cout << "使用 continue:\n";
    for (int i = 0; i < 5; ++i) {
        if (i == 3) continue;  // 跳过当前迭代，继续后续迭代
        std::cout << i << " ";
    }
    std::cout << "\n\n";

    std::cout << "使用 break:\n";
    for (int i = 0; i < 5; ++i) {
        if (i == 3) break;  // 完全退出循环
        std::cout << i << " ";
    }
    std::cout << "\n";

    return 0;
}
```

**输出：**
```
使用 continue:
0 1 2 4

使用 break:
0 1 2
```

#### 错误 3：误解 for 循环中的行为

```cpp
#include <iostream>

int main() {
    // ❌ 错误理解：以为 continue 会跳过迭代表达式
    for (int i = 0; i < 10; ++i) {
        if (i == 5) continue;  // i 仍会递增到 6
        std::cout << i << " ";
    }
    // 输出：0 1 2 3 4 6 7 8 9
    // 注意：5 被跳过，但循环正常继续

    return 0;
}
```

### 实际应用示例

```cpp
#include <iostream>
#include <vector>

int main() {
    // 示例：处理用户输入，过滤无效数据
    std::vector<int> inputs = {10, -1, 20, 0, 30, -5, 40, 100, 50};

    std::cout << "有效输入 (> 0 且 < 100):\n";
    int count = 0;
    int sum = 0;

    for (int x : inputs) {
        // 快速过滤无效数据
        if (x <= 0) continue;      // 跳过非正数
        if (x >= 100) continue;    // 跳过超大数据

        // 处理有效数据
        ++count;
        sum += x;
        std::cout << "  处理: " << x << "\n";
    }

    std::cout << "\n统计:\n";
    std::cout << "  有效数量: " << count << "\n";
    std::cout << "  总和: " << sum << "\n";
    std::cout << "  平均值: " << (count > 0 ? (double)sum / count : 0.0) << "\n";

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| **作用** | 跳过当前循环迭代的剩余部分 |
| **适用范围** | 仅限 for、range-for、while、do-while 循环 |
| **跳转行为** | 在 for 循环中跳转到迭代表达式之前 |
| **嵌套循环** | 只影响最内层循环 |
| **性能** | 零开销抽象，编译为跳转指令 |

### continue vs break 对比

| 特性 | continue | break |
|------|----------|-------|
| 影响范围 | 仅当前迭代 | 整个循环 |
| 后续迭代 | 继续执行 | 完全退出 |
| for 循环行为 | 执行迭代表达式 | 不执行迭代表达式 |
| 使用场景 | 过滤特定条件 | 提前终止循环 |

### continue 在不同循环中的行为

| 循环类型 | continue 后执行 |
|----------|-----------------|
| for / range-for | 迭代表达式 → 条件表达式 |
| while | 条件表达式 |
| do-while | 条件表达式 |

### 学习建议

1. **理解迭代控制**：continue 控制的是迭代，不是退出
2. **while 中谨慎使用**：确保更新语句在 continue 之前
3. **for 循环更安全**：迭代表达式总是执行
4. **优先使用 continue 简化条件逻辑**：避免深层嵌套

### 相关概念

| 概念 | 关系 |
|------|------|
| `break` 语句 | 退出整个循环 |
| `return` 语句 | 退出整个函数 |
| `goto` 语句 | 无条件跳转（不推荐） |
| `throw` 语句 | 异常跳出循环 |

---

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/continue
- C++26 标准 (ISO/IEC 14882:2026)
- C++23 标准 (ISO/IEC 14882:2023)
- C++17 标准 (ISO/IEC 14882:2017)
- C++11 标准 (ISO/IEC 14882:2011)
- C++98 标准 (ISO/IEC 14882:1998)
- The C++ Programming Language, Bjarne Stroustrup