# C++ 语句 (Statements)

## 1. 概述 (Overview)

语句（Statements）是 C++ 程序按顺序执行的代码片段。任何函数体都是一系列语句。语句是 C++ 程序执行流的基本单位。

### 九种语句类型

| 类型 | 中文名称 | 说明 |
|------|----------|------|
| Labeled statements | 标签语句 | 带有标签的语句 |
| Expression statements | 表达式语句 | 后跟分号的表达式 |
| Compound statements | 复合语句 | 用花括号包围的语句序列 |
| Selection statements | 选择语句 | if、switch 等条件分支语句 |
| Iteration statements | 迭代语句 | while、do-while、for 循环语句 |
| Jump statements | 跳转语句 | break、continue、return、goto |
| Declaration statements | 声明语句 | 引入标识符的语句 |
| try blocks | try 块 | 异常处理块 |
| Atomic and synchronized blocks | 原子和同步块 | 事务内存（TM TS） |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 的语句设计继承自 C 语言，强调简洁性和直接的流程控制。随着语言演进，C++ 增加了许多现代特性。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 继承 C 语言的基本语句类型，增加声明语句和 try 块 |
| C++11 | 引入属性语法、范围 for 循环、返回语句的列表初始化 |
| C++17 | 引入 `constexpr if` 语句、控制流受限语句概念 |
| C++23 | 引入 `consteval if` 语句；标签可独立出现在复合语句中 |

### 设计动机

语句机制解决了以下核心问题：

1. **顺序执行**：明确程序执行流程
2. **作用域控制**：通过复合语句引入块作用域
3. **流程控制**：通过选择、迭代、跳转语句控制执行路径
4. **异常处理**：通过 try 块处理运行时错误
5. **编译时分支**：通过 constexpr if 实现编译时条件编译

## 3. 语法与参数 (Syntax and Parameters)

### 标签语句语法

```cpp
label statement
```

### 标签语法

| 语法 | 说明 |
|------|------|
| `attr(可选) identifier:` | (1) goto 目标标签 |
| `attr(可选) case 常量表达式:` | (2) switch case 标签 |
| `attr(可选) default:` | (3) switch default 标签 |

### 表达式语句语法

```cpp
attr(可选) expression(可选) ;
```

### 复合语句语法

```cpp
attr(可选) { statement...(可选) label...(可选)(C++23) }
```

### 选择语句语法

```cpp
// if 语句
attr(可选) if constexpr(可选) ( init-statement(可选) condition ) statement
attr(可选) if constexpr(可选) ( init-statement(可选) condition ) statement else statement

// switch 语句
attr(可选) switch ( init-statement(可选) condition ) statement

// consteval if 语句（C++23）
attr(可选) if !(可选) consteval 复合语句
attr(可选) if !(可选) consteval 复合语句 else statement
```

### 迭代语句语法

```cpp
// while 循环
attr(可选) while ( condition ) statement

// do-while 循环
attr(可选) do statement while ( expression ) ;

// for 循环
attr(可选) for ( init-statement condition(可选) ; expression(可选) ) statement

// 范围 for 循环（C++11）
attr(可选) for ( init-statement(可选)(C++20) for-range-decl : for-range-init ) statement
```

### 跳转语句语法

```cpp
attr(可选) break ;
attr(可选) continue ;
attr(可选) return expression(可选) ;
attr(可选) return braced-init-list ;  // C++11
attr(可选) goto identifier ;
```

### 声明语句语法

```cpp
block-declaration
```

### try 块语法

```cpp
attr(可选) try 复合语句 handler序列
```

### 原子和同步块语法（TM TS）

```cpp
synchronized 复合语句
atomic_noexcept 复合语句
atomic_cancel 复合语句
atomic_commit 复合语句
```

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
程序开始
    ↓
进入 main 函数
    ↓
顺序执行语句
    ↓
┌─────────────────────────────────────────┐
│ 复合语句 │ 选择语句 │ 迭代语句 │ 跳转语句 │
└─────────────────────────────────────────┘
    ↓           ↓           ↓         ↓
顺序执行   条件分支执行   循环执行   无条件跳转
    ↓           ↓           ↓         ↓
┌─────────────────────────────────────────┐
│              对象析构检查               │
└─────────────────────────────────────────┘
    ↓
函数返回
```

### 块作用域与对象生命周期

每个复合语句引入自己的块作用域。块内声明的变量在关闭花括号处按逆序销毁：

```cpp
int main()
{ // 外层块开始
    { // 内层块开始
        std::ofstream f("test.txt"); // 声明语句
        f << "abc\n";                // 表达式语句
    } // 内层块结束，f 被刷新并关闭
    std::ifstream f("test.txt"); // 声明语句
    std::string str;             // 声明语句
    f >> str;                    // 表达式语句
} // 外层块结束，str 被销毁，f 被关闭
```

### 标签作用域

- 标签具有**函数作用域**：可在函数内任何位置通过 goto 访问
- 标签**不参与**非限定查找：可与程序中任何其他实体同名
- C++23 起：标签可独立出现在复合语句末尾

### 控制流受限语句（C++17）

以下语句是**控制流受限语句**：

- try 块的复合语句
- 处理程序的复合语句
- `constexpr if` 语句的所有子语句（C++17）
- `consteval if` 语句的所有子语句（C++23）

**限制**：
- 控制流受限语句内的 goto 标签只能被同一语句内的代码引用
- 其中的 case/default 标签只能与同一语句内的 switch 关联

### 子语句概念

语句 S1 的子语句包括：

1. 标签语句的语句部分
2. 复合语句中的任意语句
3. 选择语句的语句部分
4. 迭代语句的语句部分

**包围关系**：语句 S1 包围语句 S2，如果 S2 是 S1 的子语句，或通过传递关系。

## 5. 使用场景 (Use Cases)

### 标签语句

#### 1. goto 目标

```cpp
void process()
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (error_condition) {
                goto cleanup;  // 跳出嵌套循环
            }
        }
    }
    return;

cleanup:
    printf("Error occurred\n");
}
```

#### 2. switch case 标签

```cpp
switch (ch) {
    case 'a':
    case 'A':
        std::cout << "A\n";
        break;
    default:
        std::cout << "Other\n";
}
```

### 表达式语句

#### 1. 赋值语句

```cpp
x = 10;
array[i] = value;
```

#### 2. 函数调用

```cpp
std::cout << "Hello, World!\n";
process_data(data, size);
```

#### 3. 空语句

```cpp
while (*s++ != '\0')
    ; // 空语句
```

### 选择语句

#### 1. if 语句

```cpp
if (x > 0) {
    std::cout << "Positive\n";
} else if (x < 0) {
    std::cout << "Negative\n";
} else {
    std::cout << "Zero\n";
}
```

#### 2. if with init-statement（C++17）

```cpp
if (auto it = map.find(key); it != map.end()) {
    std::cout << "Found: " << it->second << "\n";
}
```

#### 3. constexpr if（C++17）

```cpp
if constexpr (std::is_integral_v<T>) {
    return x + 1;
} else {
    return x + 0.5;
}
```

#### 4. consteval if（C++23）

```cpp
if consteval {
    // 编译时执行
} else {
    // 运行时执行
}
```

### 迭代语句

#### 1. 范围 for 循环（C++11）

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
for (int& x : v) {
    x *= 2;
}
```

#### 2. 范围 for with init-statement（C++20）

```cpp
for (auto& [key, value] : map) {
    std::cout << key << ": " << value << "\n";
}
```

### 跳转语句

#### 1. return with braced-init-list（C++11）

```cpp
std::pair<int, int> get_point() {
    return {1, 2};  // 列表初始化
}
```

### 最佳实践

1. **使用花括号**：即使单条语句也使用花括号
2. **避免 goto**：尽量使用结构化控制流
3. **优先使用范围 for**：更安全、更简洁
4. **使用 constexpr if**：编译时分支，零运行时开销
5. **初始化语句**：限制变量作用域

### 常见陷阱

#### 陷阱 1：悬空 else

```cpp
// 危险：else 关联到最近的 if
if (a > 0)
    if (b > 0)
        std::cout << "Both\n";
else
    std::cout << "a <= 0\n"; // 实际关联到内层 if

// 修正：使用花括号
if (a > 0) {
    if (b > 0)
        std::cout << "Both\n";
} else {
    std::cout << "a <= 0\n";
}
```

#### 陷阱 2：switch 缺少 break

```cpp
// 危险：fall-through 行为
switch (x) {
    case 1:
        std::cout << "One\n";
        // 缺少 break，继续执行
    case 2:
        std::cout << "Two\n";
        break;
}

// C++17: 使用 [[fallthrough]] 属性明确意图
switch (x) {
    case 1:
        std::cout << "One\n";
        [[fallthrough]];
    case 2:
        std::cout << "Two\n";
        break;
}
```

#### 陷阱 3：跳转导致对象未析构

```cpp
// 危险：跳过对象初始化
goto skip;
std::string s = "hello";  // 跳过构造
skip:
// s 未被正确构造

// 修正：确保对象正确初始化
{
    std::string s = "hello";
    // ...
}
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：函数体结构

```cpp
#include <iostream>

int main()
{
    int n = 1;                        // 声明语句
    n = n + 1;                        // 表达式语句
    std::cout << "n = " << n << '\n'; // 表达式语句
    return 0;                         // return 语句
}
```

#### 示例 2：复合语句与作用域

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main()
{ // 外层块开始
    { // 内层块开始
        std::ofstream f("test.txt"); // 声明语句
        f << "abc\n";                // 表达式语句
    } // 内层块结束，f 被刷新并关闭
    std::ifstream f("test.txt"); // 声明语句，f 可重用
    std::string str;             // 声明语句
    f >> str;                    // 表达式语句
    std::cout << str << "\n";    // 输出: abc
} // 外层块结束，str 被销毁，f 被关闭
```

### 高级用法

#### 示例 3：constexpr if（C++17）

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
T process(T x)
{
    if constexpr (std::is_integral_v<T>) {
        return x * 2;  // 整数：乘法
    } else if constexpr (std::is_floating_point_v<T>) {
        return x * 1.5;  // 浮点：使用不同系数
    } else {
        return x;  // 其他：原样返回
    }
}

int main()
{
    std::cout << process(5) << "\n";     // 输出: 10
    std::cout << process(2.0) << "\n";   // 输出: 3
    return 0;
}
```

#### 示例 4：if with init-statement（C++17）

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };

    // 限制锁的作用域
    if (auto it = scores.find("Alice"); it != scores.end()) {
        std::cout << "Alice's score: " << it->second << "\n";
    } else {
        std::cout << "Alice not found\n";
    }

    // it 在此处已超出作用域
    return 0;
}
```

#### 示例 5：consteval if（C++23）

```cpp
#include <iostream>
#include <cmath>

constexpr double compute(double x)
{
    if consteval {
        // 编译时：使用精确计算
        return x * x + x;
    } else {
        // 运行时：可以使用优化
        return std::fma(x, x, x);  // fused multiply-add
    }
}

int main()
{
    constexpr double a = compute(2.0);  // 编译时计算
    double b = compute(3.0);            // 运行时计算

    std::cout << a << "\n";  // 输出: 6
    std::cout << b << "\n";  // 输出: 12
    return 0;
}
```

#### 示例 6：范围 for 循环

```cpp
#include <iostream>
#include <vector>
#include <map>

int main()
{
    // 基本 range for
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (const auto& x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // 结构化绑定（C++17）
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87}
    };
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << "\n";
    }

    // with init-statement（C++20）
    for (auto& v = getVector(); const auto& x : v) {
        std::cout << x << " ";
    }

    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：标签作用域误解

```cpp
void f()
{
    {
        goto label; // OK：标签在作用域内（即使声明在后面）
        label:      // C++23: 标签可独立出现在块末尾
    }
    goto label; // OK：标签忽略块作用域
}

void g()
{
    goto label; // 错误：label 不在 g() 的作用域内
}
```

#### 错误示例 2：控制流受限语句违规

```cpp
// C++17: constexpr if 内的标签不能被外部引用
void bad()
{
    if constexpr (true) {
        inner_label:  // 控制流受限
        ;
    }
    goto inner_label;  // 错误：不能跳入控制流受限语句
}

// 修正：将标签移到外部
void good()
{
    outer_label:
    if constexpr (true) {
        goto outer_label;  // OK：可以跳出
    }
}
```

#### 错误示例 3：跳过对象初始化

```cpp
// 危险：跳过对象初始化
void bad()
{
    goto skip;
    std::string s = "hello";  // 跳过
skip:
    // s 处于未定义状态
}

// 修正：正确管理对象生命周期
void good()
{
    {
        std::string s = "hello";
        // 使用 s
    }
    // 此处跳转是安全的
}
```

## 7. 总结 (Summary)

### 核心要点

1. **九种类型**：标签、表达式、复合、选择、迭代、跳转、声明、try 块、原子块
2. **顺序执行**：语句按顺序执行，除非遇到跳转语句
3. **块作用域**：复合语句引入新作用域，变量按逆序销毁
4. **标签函数作用域**：标签可在函数内任意位置访问
5. **控制流受限**：constexpr if、try 块等限制 goto 和 switch 标签

### C++ 与 C 的差异

| 特性 | C | C++ |
|------|---|-----|
| 声明语句 | 无（声明不是语句） | 有 |
| try 块 | 无 | 有 |
| 属性语法 | C23 引入 | C++11 引入 |
| constexpr if | 无 | C++17 引入 |
| consteval if | 无 | C++23 引入 |
| 范围 for | 无 | C++11 引入 |
| 标签独立出现 | C23 支持 | C++23 支持 |

### C++ 版本特性摘要

| 版本 | 新增语句特性 |
|------|-------------|
| C++11 | 属性、范围 for、return 列表初始化 |
| C++17 | constexpr if、if with init-statement、控制流受限语句 |
| C++23 | consteval if、标签可独立出现 |

### 学习建议

1. **理解对象生命周期**：跳转语句可能影响对象析构
2. **使用现代 C++ 特性**：范围 for、constexpr if 等
3. **善用初始化语句**：限制变量作用域
4. **避免 goto**：优先使用结构化控制流
5. **注意控制流受限语句**：理解其限制

---

**相关参考**：
- C++ 声明和初始化文档
- C++ try 块文档
- C++ 属性文档