# Boolean Literals - 布尔字面量

## 1. 概述

Boolean literals（布尔字面量）是 C++ 中的基本字面量类型，用于表示布尔值 `true`（真）和 `false`（假）。它们是关键字 `true` 和 `false`，属于 `bool` 类型的纯右值（prvalue，pure rvalue）。

布尔字面量是 C++ 语言中最基础的字面量之一，广泛用于条件判断、逻辑运算和标志变量等场景。作为语言内置的关键字，它们提供了直接、清晰的方式来表示布尔状态。

## 2. 来源与演变

### 首次引入

Boolean literals 在 **C++98** 标准中首次引入，同时引入的还有 `bool` 类型和 `true`、`false` 关键字。在此之前，C++ 继承了 C 语言的做法，使用整数 `0` 和 `1` 来表示假和真。

### 历史背景

在 C++98 之前，C++ 程序员面临以下问题：

1. **缺乏原生布尔类型**：使用 `int`、`char` 或枚举来表示布尔值
2. **类型安全性差**：布尔值可以与整数混用，容易引发错误
3. **代码可读性低**：`if (x != 0)` 不如 `if (x)` 直观

引入 `bool` 类型和 Boolean literals 解决了这些问题：
- 提供了类型安全的布尔类型
- 增强了代码可读性
- 统一了布尔值的表示方式

### 版本演进

| 标准 | 章节编号 | 说明 |
|------|----------|------|
| C++98 | 2.13.5 | 首次引入布尔字面量 |
| C++11 | 2.13.6 | 保持不变，章节号调整 |
| C++14 | 2.13.6 | 保持不变 |
| C++17 | 5.13.6 | 章节号调整 |
| C++20 | 5.13.6 | 保持不变 |
| C++23 | 5.13.6 | 保持不变 |

从 C++98 到 C++23，Boolean literals 的定义和语义保持高度稳定，体现了这一特性的成熟性和基础性。

## 3. 语法与参数

### 基本语法

Boolean literals 的语法极其简单：

```cpp
true    // (1) 表示真值
false   // (2) 表示假值
```

### 语法说明

| 字面量 | 类型 | 值类别 | 含义 |
|--------|------|--------|------|
| `true` | `bool` | prvalue | 表示逻辑真，对应整数值 1 |
| `false` | `bool` | prvalue | 表示逻辑假，对应整数值 0 |

### 类型特性

Boolean literals 具有以下类型特性：

```cpp
// 类型
static_assert(std::is_same_v<decltype(true), bool>);
static_assert(std::is_same_v<decltype(false), bool>);

// 值类别
static_assert(std::is_prvalue_v<decltype(true)>);   // 纯右值
static_assert(std::is_prvalue_v<decltype(false)>);  // 纯右值

// 数值转换
static_assert(true == 1);
static_assert(false == 0);
```

### 无参数

Boolean literals 不接受任何参数，它们是语言级别的关键字字面量。

## 4. 底层原理

### 存储表示

虽然 C++ 标准没有规定 `bool` 类型的具体大小，但常见实现中：

```cpp
// 典型实现
sizeof(bool) == 1    // 大多数平台
sizeof(true) == 1    // true 是 bool 类型的 prvalue
sizeof(false) == 1   // false 是 bool 类型的 prvalue

// 内部表示（实现定义）
true  -> 0x01 或非零值
false -> 0x00
```

### 值类别（Value Category）

Boolean literals 是**纯右值（prvalue）**，具有以下特性：

1. **无地址**：不能对 `true` 或 `false` 取地址
2. **可移动**：可以作为右值参与移动语义
3. **可转换**：可以隐式转换为其他类型

```cpp
// 错误：不能对布尔字面量取地址
// bool* p = &true;  // 编译错误

// 正确：可以绑定到 const 引用
const bool& ref = true;  // 创建临时对象并绑定

// 正确：可以绑定到右值引用
bool&& rref = false;  // 直接绑定到右值
```

### 隐式转换规则

Boolean literals 支持以下隐式转换：

#### 1. 布尔提升（Boolean Promotion）

```cpp
bool b = true;
int i = true;   // i = 1
double d = false;  // d = 0.0
```

#### 2. 布尔转换（Boolean Conversion）

从其他类型转换为 `bool`：

```cpp
bool b1 = 42;      // true（非零值）
bool b2 = 0;       // false
bool b3 = 3.14;    // true（非零浮点数）
bool b4 = nullptr; // false（空指针）
bool b5 = &b1;     // true（非空指针）
```

### 编译器优化

现代编译器会对 Boolean literals 进行优化：

```cpp
// 优化：直接生成机器指令
if (true) { /* ... */ }  // 编译器通常移除条件判断

// 编译时常量折叠
constexpr bool flag = true;
static_assert(flag, "Always true");
```

## 5. 使用场景

### 适用场景

#### 1. 条件判断

```cpp
if (true) {
    // 始终执行的代码块（通常用于调试或测试）
}

while (true) {
    // 无限循环
    if (should_exit) break;
}
```

#### 2. 标志变量初始化

```cpp
bool is_valid = true;
bool has_error = false;
bool is_running = true;
```

#### 3. 函数返回值

```cpp
bool is_empty() const {
    return size == 0 ? true : false;  // 可简化为 return size == 0;
}

bool check_permission(User& user) {
    return user.has_permission("admin");
}
```

#### 4. 逻辑运算

```cpp
bool a = true;
bool b = false;

bool result1 = a && b;  // 逻辑与：false
bool result2 = a || b;  // 逻辑或：true
bool result3 = !a;     // 逻辑非：false
```

### 最佳实践

#### 1. 避免冗余比较

```cpp
// 不推荐
if (is_valid == true) { }
if (is_empty != false) { }

// 推荐
if (is_valid) { }
if (!is_empty) { }
```

#### 2. 使用字面量初始化

```cpp
// 推荐：直接使用字面量
bool flag = true;

// 不推荐：使用整数字面量
bool flag = 1;  // 可行但不推荐
```

#### 3. 条件表达式返回

```cpp
// 不推荐：冗余的条件表达式
bool is_positive(int x) {
    return x > 0 ? true : false;
}

// 推荐：直接返回条件结果
bool is_positive(int x) {
    return x > 0;
}
```

### 常见陷阱

#### 1. 与整数的隐式转换

```cpp
int x = true + false;  // x = 1 + 0 = 1
int y = true * 5;      // y = 1 * 5 = 5

// 注意：虽然合法，但降低代码可读性
```

#### 2. 指针与布尔值混淆

```cpp
int* p = nullptr;
bool b = p;  // b = false，隐式转换

// 注意：这可能是无意的，建议显式转换
bool b2 = (p != nullptr);  // 更清晰
```

#### 3. 布尔运算符优先级

```cpp
bool a = true, b = false;
int x = 1, y = 2;

// 注意运算符优先级
int result = a + b * x;  // 1 + (0 * 1) = 1，而非 (1 + 0) * 1

// 建议：使用括号明确意图
int result2 = (a + b) * x;  // (1 + 0) * 1 = 1
```

## 6. 代码示例

### 基础用法

#### 示例 1：基本声明与输出

```cpp
#include <iostream>

int main() {
    // 声明布尔变量
    bool flag1 = true;
    bool flag2 = false;

    // 默认输出（0/1）
    std::cout << "flag1 (default): " << flag1 << '\n';
    std::cout << "flag2 (default): " << flag2 << '\n';

    // 使用 boolalpha 输出（true/false）
    std::cout << std::boolalpha;
    std::cout << "flag1 (boolalpha): " << flag1 << '\n';
    std::cout << "flag2 (boolalpha): " << flag2 << '\n';

    // 恢复默认输出
    std::cout << std::noboolalpha;
    std::cout << "flag1 (noboolalpha): " << flag1 << '\n';

    return 0;
}
```

**输出：**
```
flag1 (default): 1
flag2 (default): 0
flag1 (boolalpha): true
flag2 (boolalpha): false
flag1 (noboolalpha): 1
```

#### 示例 2：逻辑运算

```cpp
#include <iostream>
#include <iomanip>

int main() {
    bool a = true;
    bool b = false;

    std::cout << std::boolalpha;
    std::cout << "a = " << a << ", b = " << b << '\n';
    std::cout << "a && b = " << (a && b) << '\n';  // 逻辑与
    std::cout << "a || b = " << (a || b) << '\n';  // 逻辑或
    std::cout << "!a = " << !a << '\n';            // 逻辑非
    std::cout << "!b = " << !b << '\n';

    return 0;
}
```

**输出：**
```
a = true, b = false
a && b = false
a || b = true
!a = false
!b = true
```

### 高级用法

#### 示例 3：条件表达式与三目运算符

```cpp
#include <iostream>
#include <string>

int main() {
    // 在三目运算符中使用布尔字面量
    int value = 42;
    std::string result = (value > 0) ? "positive" : "non-positive";

    // 使用布尔字面量作为函数参数
    auto check = [](bool flag) -> std::string {
        return flag ? "YES" : "NO";
    };

    std::cout << "Value is " << result << '\n';
    std::cout << "Check true: " << check(true) << '\n';
    std::cout << "Check false: " << check(false) << '\n';

    // 编译时常量
    constexpr bool is_debug = true;
    if constexpr (is_debug) {
        std::cout << "Debug mode enabled\n";
    }

    return 0;
}
```

#### 示例 4：容器与算法中的布尔值

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // 初始化布尔向量
    std::vector<bool> flags(5, true);
    flags[2] = false;
    flags[4] = false;

    // 统计 true 的数量
    int true_count = std::count(flags.begin(), flags.end(), true);
    int false_count = std::count(flags.begin(), flags.end(), false);

    std::cout << "True count: " << true_count << '\n';
    std::cout << "False count: " << false_count << '\n';

    // 查找第一个 false
    auto it = std::find(flags.begin(), flags.end(), false);
    if (it != flags.end()) {
        std::cout << "First false at index: "
                  << std::distance(flags.begin(), it) << '\n';
    }

    return 0;
}
```

**输出：**
```
True count: 3
False count: 2
First false at index: 2
```

#### 示例 5：类型转换与数值运算

```cpp
#include <iostream>

int main() {
    // 布尔值参与算术运算
    int sum = true + true + false;  // 1 + 1 + 0 = 2
    int product = true * 5 + false * 3;  // 1*5 + 0*3 = 5

    std::cout << "Sum: " << sum << '\n';
    std::cout << "Product: " << product << '\n';

    // 布尔值转换为浮点数
    double d_true = true;   // 1.0
    double d_false = false; // 0.0

    std::cout << "Double true: " << d_true << '\n';
    std::cout << "Double false: " << d_false << '\n';

    // 从其他类型转换为布尔值
    bool from_int = 42;       // true
    bool from_zero = 0;       // false
    bool from_ptr = &sum;     // true（非空指针）
    bool from_null = nullptr; // false

    std::cout << std::boolalpha;
    std::cout << "From int: " << from_int << '\n';
    std::cout << "From zero: " << from_zero << '\n';
    std::cout << "From ptr: " << from_ptr << '\n';
    std::cout << "From null: " << from_null << '\n';

    return 0;
}
```

**输出：**
```
Sum: 2
Product: 5
Double true: 1
Double false: 0
From int: true
From zero: false
From ptr: true
From null: false
```

### 常见错误及修正

#### 错误示例 1：误用布尔字面量与整数比较

```cpp
#include <iostream>

int main() {
    bool flag = true;

    // 错误：与整数字面量比较（虽然合法，但不推荐）
    if (flag == 1) {  // 可行，但降低可读性
        std::cout << "Flag is true\n";
    }

    // 修正：直接使用布尔值
    if (flag) {
        std::cout << "Flag is true (better)\n";
    }

    return 0;
}
```

#### 错误示例 2：布尔值的自增自减（已弃用）

```cpp
#include <iostream>

int main() {
    bool b = false;

    // C++17 前可行，C++17 起弃用，C++20 起移除
    // b++;  // 错误：C++20 中已移除布尔值的 ++ 运算符

    // 修正：显式赋值
    b = true;  // 正确做法

    std::cout << std::boolalpha << b << '\n';

    return 0;
}
```

#### 错误示例 3：混淆逻辑运算符与位运算符

```cpp
#include <iostream>

int main() {
    bool a = true;
    bool b = false;

    // 错误：使用位运算符而非逻辑运算符
    // bool result = a & b;   // 位与：0，但类型为 int
    // bool result = a | b;   // 位或：1，但类型为 int

    // 修正：使用逻辑运算符
    bool result_and = a && b;  // 逻辑与：false
    bool result_or = a || b;   // 逻辑或：true

    std::cout << std::boolalpha;
    std::cout << "a && b = " << result_and << '\n';
    std::cout << "a || b = " << result_or << '\n';

    return 0;
}
```

## 7. 总结

### 核心要点

Boolean literals 是 C++ 语言的基础构建块：

1. **简单性**：仅有 `true` 和 `false` 两个关键字
2. **类型安全**：专用的 `bool` 类型，避免与整数混淆
3. **高效性**：作为纯右值，编译器可以高度优化
4. **可读性**：提供清晰的布尔值表示，提升代码可读性

### 技术对比

| 特性 | Boolean literals | 整数模拟 | 宏定义 |
|------|------------------|----------|--------|
| 类型安全 | ✓ 类型明确 | ✗ 类型为 int | ✗ 无类型检查 |
| 可读性 | ✓ 清晰直观 | △ 需要约定 | △ 不够直观 |
| 作用域 | ✓ 关键字 | ✓ 值 | △ 全局宏 |
| 编译器支持 | ✓ 语言内置 | ✓ 基本类型 | ✓ 预处理器 |
| 推荐度 | ★★★★★ | ★★☆☆☆ | ★☆☆☆☆ |

### 学习建议

1. **掌握基础**：理解 Boolean literals 的类型（`bool`）和值类别（prvalue）
2. **熟悉转换**：掌握布尔值与其他类型的隐式转换规则
3. **规范使用**：
   - 优先使用 `if (flag)` 而非 `if (flag == true)`
   - 避免布尔值与整数的混用
   - 使用 `std::boolalpha` 提升输出可读性
4. **注意版本**：C++20 移除了布尔值的自增/自减运算符
5. **实践练习**：通过编写条件逻辑、标志管理等代码加深理解

### 相关主题

- **Integral conversions**：整数转换，包括 `bool` 与整数类型之间的转换
- **Boolean conversions**：布尔转换，其他类型到 `bool` 的转换规则
- **Value categories**：值类别，理解 lvalue、rvalue、prvalue 等
- **`std::vector<bool>`**：特殊的布尔向量容器，空间优化的特化版本

### 参考标准

- ISO/IEC 14882:2024 (C++23) §5.13.6 Boolean literals
- ISO/IEC 14882:2020 (C++20) §5.13.6 Boolean literals
- ISO/IEC 14882:2017 (C++17) §5.13.6 Boolean literals
- ISO/IEC 14882:2014 (C++14) §2.13.6 Boolean literals
- ISO/IEC 14882:2011 (C++11) §2.13.6 Boolean literals
- ISO/IEC 14882:1998 (C++98) §2.13.5 Boolean literals