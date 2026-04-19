# if 语句

## 1. 概述 (Overview)

if 语句是 C++ 中的条件控制语句，用于根据条件表达式的结果有条件地执行另一个语句。它是程序流程控制的基础构件之一，允许程序根据运行时或编译时的条件选择性地执行代码路径。

if 语句支持多种形式：
- 基本条件判断（无 else 分支）
- 带 else 分支的条件判断
- constexpr if（编译时条件判断，C++17 起）
- consteval if（常量求值上下文检测，C++23 起）
- 带初始化语句的 if（C++17 起）

主要用途包括：
- 运行时条件分支选择
- 编译时条件编译（通过 constexpr if）
- 检测是否在常量求值上下文中执行（通过 consteval if）
- 在条件判断中引入临时变量（带初始化语句）

## 2. 来源与演变 (Origin and Evolution)

### C++98/03 标准时期

if 语句从 C 语言继承而来，提供基本的条件分支功能：
- 支持表达式条件
- 支持声明条件（在条件中声明变量）
- 支持 else 分支

### C++11 标准引入

- 允许在 if 语句上使用属性（attribute）

### C++17 标准重要更新

引入了两个重要特性：

**constexpr if 语句**
- 允许在编译时进行条件判断
- 未被选中的分支在模板外会进行语法检查，在模板内可能不被实例化
- 提供了更强大的编译时编程能力

**带初始化语句的 if**
- 允许在条件判断前执行初始化语句
- 简化了需要临时变量的条件判断场景
- 提高了代码的可读性和作用域控制

### C++23 标准扩展

**consteval if 语句**
- 用于检测当前是否在显式常量求值上下文中执行
- 支持正向和反向形式（`if consteval` 和 `if !consteval`）
- 区分编译时和运行时执行路径

**init-statement 支持别名声明**
- 初始化语句可以包含别名声明

### C++26 标准预览

- 条件可以是结构化绑定声明

### 特性测试宏

| 宏 | 值 | 标准 | 特性 |
|---|---|---|---|
| `__cpp_if_constexpr` | 201606L | C++17 | constexpr if |
| `__cpp_if_consteval` | 202106L | C++23 | consteval if |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

**形式 1：无 else 分支**
```cpp
attr(optional) if constexpr(optional) ( init-statement(optional) condition ) statement-true
```

**形式 2：带 else 分支**
```cpp
attr(optional) if constexpr(optional) ( init-statement(optional) condition ) statement-true else statement-false
```

**形式 3：consteval if 无 else 分支**（C++23 起）
```cpp
attr(optional) if !(optional) consteval compound-statement
```

**形式 4：consteval if 带 else 分支**（C++23 起）
```cpp
attr(optional) if !(optional) consteval compound-statement else statement
```

### 参数说明

| 参数 | 说明 | 引入版本 |
|---|---|---|
| `attr` | 任意数量的属性 | C++11 |
| `constexpr` | 如果存在，语句成为 constexpr if 语句 | C++17 |
| `init-statement` | 初始化语句，可以是表达式语句、简单声明或别名声明（C++23 起） | C++17 |
| `condition` | 条件，可以是表达式或简单声明 | C++98 |
| `statement-true` | 条件为真时执行的语句 | C++98 |
| `statement-false` | 条件为假时执行的语句 | C++98 |
| `compound-statement` | 复合语句（语句块） | C++23 |

### init-statement 详细说明

初始化语句可以是：
1. **表达式语句**（包括空语句 `;`）
2. **简单声明**：通常是带初始化器的变量声明，可以声明多个变量或结构化绑定
3. **别名声明**（C++23 起）

注意：任何初始化语句必须以分号结束。

### condition 详细说明

条件可以是表达式或简单声明。

**表达式条件**
- 值为表达式上下文转换为 bool 的结果
- 如果转换不合法，程序不合法

**声明条件（非结构化绑定）**

语法限制：
- C++11 前：`type-specifier-seq declarator = assignment-expression`
- C++11 起：`attribute-specifier-seq(optional) decl-specifier-seq declarator brace-or-equal-initializer`

声明限制：
- 声明符不能指定函数或数组
- 类型说明符序列（C++11 前）/声明说明符序列（C++11 起）只能包含类型说明符和 `constexpr`
- 不能定义类或枚举

**声明条件（结构化绑定）**（C++26 起）
- 初始化器中的表达式不能是数组类型
- 声明说明符序列只能包含类型说明符和 `constexpr`
- 决策变量是声明引入的虚构变量 `e`

### 分支选择规则

1. 如果条件产生 `true`，执行 `statement-true`
2. 如果存在 else 部分且条件产生 `false`，执行 `statement-false`
3. 嵌套 if 语句中，else 与最近的尚未关联 else 的 if 配对

## 4. 底层原理 (Underlying Principles)

### 条件求值机制

**运行时条件判断**
- 条件表达式在运行时求值
- 结果上下文转换为 bool 类型
- 根据结果跳转到相应分支执行

**编译时条件判断（constexpr if）**

constexpr if 的核心机制：

1. **条件求值**
   - C++23 前：条件必须是上下文转换为 bool 的常量表达式
   - C++23 起：条件是上下文转换为 bool 的表达式，且转换本身是常量表达式

2. **分支丢弃**
   - 如果条件为真，`statement-false` 被丢弃
   - 如果条件为假，`statement-true` 被丢弃

3. **模板实例化**
   - 在模板实体中，如果条件实例化后不依赖值，丢弃的语句不被实例化
   - 如果条件实例化后仍依赖值（如嵌套模板），需要进一步实例化才能决定

4. **丢弃语句的限制**
   - 被丢弃的语句不能对所有可能的特化都不合法
   - 在模板外，被丢弃的语句仍需进行完整的语法检查

### consteval if 机制

consteval if 用于检测执行上下文：

1. **显式常量求值上下文**
   - 在常量表达式要求的上下文中
   - 在 `if consteval` 的复合语句中
   - 在立即函数调用中

2. **执行规则**
   - 如果在显式常量求值上下文中求值，执行 `compound-statement`
   - 否则执行 `statement`（如果存在）

3. **反向形式**
   - `if !consteval { stmt }` 等价于 `if consteval {} else { stmt }`
   - `if !consteval { stmt-1 } else { stmt-2 }` 等价于 `if consteval { stmt-2 } else { stmt-1 }`

4. **立即函数上下文**
   - consteval if 中的复合语句处于立即函数上下文中
   - 在此上下文中，调用立即函数不需要是常量表达式

### 作用域规则

**初始化语句的作用域**
- 初始化语句声明的名称与条件声明的名称处于同一作用域
- 该作用域也是两个分支语句的作用域

**条件声明的作用域**
- 条件中声明的变量在整个 if 语句体内可见
- 即使分支语句不是复合语句，也隐含作用域

```cpp
if (int x = f()) {
    int x; // 错误：重定义
} else {
    int x; // 错误：重定义
}
```

## 5. 使用场景 (Use Cases)

### 基本条件判断

**适用场景**
- 根据运行时条件选择执行路径
- 验证输入参数
- 错误处理和边界检查

**最佳实践**
- 保持条件简单清晰
- 避免深层嵌套
- 合理使用 else 分支处理所有情况

### 带初始化语句的 if（C++17）

**适用场景**
- 在条件中使用临时变量
- 锁保护的条件判断
- 迭代器查找

```cpp
// 在 map 中查找元素
if (auto it = m.find(key); it != m.end()) {
    return it->second;
}

// 带锁的条件判断
if (std::lock_guard lock(mx); shared_flag) {
    unsafe_ping();
    shared_flag = false;
}
```

### constexpr if（C++17）

**适用场景**
- 模板元编程
- 类型特征分支
- 编译时优化

```cpp
template<typename T>
auto get_value(T t) {
    if constexpr (std::is_pointer_v<T>)
        return *t;
    else
        return t;
}
```

**注意事项**
- 被丢弃的分支仍需语法正确
- 在模板外不替代预处理指令
- 避免对所有特化都不合法的代码

**常见陷阱**

错误示例：
```cpp
template<typename T>
void f() {
    if constexpr (std::is_arithmetic_v<T>)
        // ...
    else {
        using invalid_array = int[-1]; // 错误：对所有 T 都不合法
        static_assert(false, "Must be arithmetic"); // CWG2518 前错误
    }
}
```

正确做法（CWG2518 前）：
```cpp
template<typename>
constexpr bool dependent_false_v = false;

template<typename T>
void f() {
    if constexpr (std::is_arithmetic_v<T>)
        // ...
    else {
        static_assert(dependent_false_v<T>, "Must be arithmetic");
    }
}
```

### consteval if（C++23）

**适用场景**
- 区分编译时和运行时执行路径
- 选择不同的算法实现
- 检测常量求值上下文

```cpp
constexpr std::uint64_t ipow(std::uint64_t base, std::uint8_t exp) {
    if consteval {
        return ipow_ct(base, exp); // 编译时优化算法
    } else {
        return std::pow(base, exp); // 运行时使用库函数
    }
}
```

**注意事项**
- `statement` 必须是复合语句
- else 分支同样需要是复合语句
- 与 `!consteval` 形式的等价关系

### 常见陷阱

**1. else 悬挂问题**

```cpp
if (i > 1)
    if (j > 2)
        std::cout << "nested";
    else // 与内层 if 关联
        std::cout << "inner else";
```

建议使用大括号明确范围：
```cpp
if (i > 1) {
    if (j > 2) {
        std::cout << "nested";
    } else {
        std::cout << "inner else";
    }
}
```

**2. goto 跳入 if 语句**

如果通过 `goto` 或 `longjmp` 进入 `statement-true`：
- 条件不被求值
- `statement-false` 不执行

## 6. 代码示例 (Examples)

### 示例 1：基本条件判断

```cpp
#include <iostream>

int main() {
    // 简单 if-else 语句
    int i = 2;
    if (i > 2)
        std::cout << i << " is greater than 2\n";
    else
        std::cout << i << " is not greater than 2\n";

    return 0;
}
```

输出：
```
2 is not greater than 2
```

### 示例 2：嵌套 if 和 else 悬挂

```cpp
#include <iostream>

int main() {
    // 嵌套 if 语句
    int i = 2;
    int j = 1;
    if (i > 1)
        if (j > 2)
            std::cout << i << " > 1 and " << j << " > 2\n";
        else // 此 else 属于 if (j > 2)，而非 if (i > 1)
            std::cout << i << " > 1 and " << j << " <= 2\n";

    return 0;
}
```

输出：
```
2 > 1 and 1 <= 2
```

### 示例 3：条件中的声明

```cpp
#include <iostream>

struct Base {
    virtual ~Base() {}
};

struct Derived : Base {
    void df() { std::cout << "df()\n"; }
};

int main() {
    Base* bp1 = new Base;
    Base* bp2 = new Derived;

    // 在条件中声明变量，配合 dynamic_cast 使用
    if (Derived* p = dynamic_cast<Derived*>(bp1)) // 转换失败，返回 nullptr
        p->df(); // 不执行

    if (auto p = dynamic_cast<Derived*>(bp2)) // 转换成功
        p->df(); // 执行

    delete bp1;
    delete bp2;
    return 0;
}
```

输出：
```
df()
```

### 示例 4：带初始化语句的 if（C++17）

```cpp
#include <iostream>
#include <map>
#include <mutex>
#include <string>

std::map<int, std::string> m;
std::mutex mx;
bool shared_flag = false;

void unsafe_ping() {
    std::cout << "ping\n";
}

int demo() {
    // 查找元素
    if (auto it = m.find(10); it != m.end())
        return static_cast<int>(it->second.size());

    // 带锁的条件判断
    if (std::lock_guard lock(mx); shared_flag) {
        unsafe_ping();
        shared_flag = false;
    }

    return 0;
}

int main() {
    m[10] = "hello";
    int result = demo();
    std::cout << "Result: " << result << "\n";
    return 0;
}
```

输出：
```
Result: 5
```

### 示例 5：constexpr if（C++17）

```cpp
#include <iostream>
#include <type_traits>

// 根据类型选择不同的返回值
template<typename T>
auto get_value(T t) {
    if constexpr (std::is_pointer_v<T>)
        return *t; // 对于 T = int*，推导返回类型为 int
    else
        return t;  // 对于 T = int，推导返回类型为 int
}

// 递归可变参数模板处理
template<typename T, typename... Rest>
void process(T&& first, Rest&&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
        process(rest...);
    }
}

int main() {
    int x = 42;
    int* px = &x;

    std::cout << "get_value(x) = " << get_value(x) << "\n";
    std::cout << "get_value(px) = " << get_value(px) << "\n";

    std::cout << "Processing: ";
    process(1, 2.0, "three", '4');
    std::cout << "\n";

    return 0;
}
```

输出：
```
get_value(x) = 42
get_value(px) = 42
Processing: 1, 2, three, 4
```

### 示例 6：consteval if（C++23）

```cpp
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>

// 检测是否在常量求值上下文
constexpr bool is_constant_evaluated() noexcept {
    if consteval {
        return true;
    } else {
        return false;
    }
}

// 编译时优化的整数幂运算
consteval std::uint64_t ipow_ct(std::uint64_t base, std::uint8_t exp) {
    if (!base) return base;
    std::uint64_t res{1};
    while (exp) {
        if (exp & 1) res *= base;
        exp /= 2;
        base *= base;
    }
    return res;
}

// 根据上下文选择不同算法
constexpr std::uint64_t ipow(std::uint64_t base, std::uint8_t exp) {
    if consteval {
        // 编译时使用优化的整数算法
        return ipow_ct(base, exp);
    } else {
        // 运行时使用标准库函数
        return static_cast<std::uint64_t>(std::pow(base, exp));
    }
}

int main(int argc, const char* argv[]) {
    // 编译时计算
    static_assert(ipow(0, 10) == 0);
    static_assert(ipow(2, 10) == 1024);

    // 运行时计算
    std::cout << "ipow(2, 10) = " << ipow(2, 10) << "\n";
    if (argc > 0) {
        std::cout << "ipow(strlen(argv[0]), 3) = "
                  << ipow(std::strlen(argv[0]), 3) << "\n";
    }

    // 检测求值上下文
    constexpr bool ct = is_constant_evaluated();
    bool rt = is_constant_evaluated();
    std::cout << "Constant context: " << ct << "\n";
    std::cout << "Runtime context: " << rt << "\n";

    return 0;
}
```

输出（示例）：
```
ipow(2, 10) = 1024
ipow(strlen(argv[0]), 3) = 27
Constant context: 1
Runtime context: 0
```

### 示例 7：常见错误及修正

**错误 1：丢弃分支中的语法错误**

```cpp
// 错误：在模板外，丢弃的分支仍需语法正确
void f() {
    if constexpr(false) {
        int i = 0;
        int* p = i; // 错误：即使分支被丢弃，语法仍需正确
    }
}
```

**错误 2：条件声明中的作用域问题**

```cpp
// 错误：在条件声明的范围内重复定义
if (int x = f()) {
    int x; // 错误：重定义 x
}
else {
    int x; // 错误：重定义 x
}
```

正确做法：
```cpp
if (int x = f()) {
    int y = x; // 使用不同的名称
}
else {
    int z = 0; // 使用不同的名称
}
```

**错误 3：consteval if 语句格式**

```cpp
constexpr void f(bool b) {
    if (true)
        if consteval {}
        else ; // 错误：必须是复合语句
               // else 与外层 if 不关联
}
```

正确做法：
```cpp
constexpr void f(bool b) {
    if (true) {
        if consteval {
            // ...
        } else {
            // ...
        }
    }
}
```

## 7. 总结 (Summary)

### 核心要点

if 语句是 C++ 中最基础也是最重要的控制流语句之一，其发展历程体现了 C++ 语言的演进方向：

1. **运行时条件判断**：if 语句最基本的功能，根据布尔条件选择执行路径

2. **编译时条件判断**（constexpr if，C++17）：提供了编译时分支选择能力，是模板元编程的重要工具。注意被丢弃分支的语法要求和模板实例化规则。

3. **常量求值上下文检测**（consteval if，C++23）：区分编译时和运行时执行环境，支持针对不同上下文选择最优算法。

4. **初始化语句**（C++17）：增强了作用域控制，简化了需要临时变量的条件判断代码。

### 技术对比

| 特性 | C++98 | C++11 | C++17 | C++23 |
|---|---|---|---|---|
| 基本条件判断 | ✓ | ✓ | ✓ | ✓ |
| 条件中声明变量 | ✓ | ✓ | ✓ | ✓ |
| 属性支持 | - | ✓ | ✓ | ✓ |
| constexpr if | - | - | ✓ | ✓ |
| 初始化语句 | - | - | ✓ | ✓ |
| consteval if | - | - | - | ✓ |
| 结构化绑定条件 | - | - | - | C++26 |

### 最佳实践建议

1. **使用初始化语句**：当条件判断需要临时变量时，优先使用初始化语句限制作用域

2. **避免深层嵌套**：通过提前返回或重构减少 if 嵌套层次

3. **constexpr if 模板编程**：使用 constexpr if 简化模板特化和类型分支

4. **consteval if 性能优化**：在需要编译时和运行时不同实现的场景使用 consteval if

5. **明确的分支作用域**：始终使用大括号包裹分支语句，避免 else 悬挂问题

6. **静态断言替代**：在 constexpr if 中使用依赖类型的 false 值而非直接 `static_assert(false)`

### 相关特性

- `std::is_constant_evaluated()`（C++20）：检测是否在常量求值上下文中
- `consteval` 函数（C++23）：立即函数
- `constexpr` 函数：常量表达式函数
- 三元运算符 `?:`：另一种条件选择方式

### 缺陷报告

**CWG 631**：C++98 中，如果通过标签进入第一个子语句，控制流未定义。修正为：条件不被求值，第二个子语句不执行（与 C 一致）。