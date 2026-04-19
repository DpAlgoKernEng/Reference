# C++ 表达式求值顺序 (Evaluation Order)

## 1. 概述 (Overview)

**求值顺序 (Order of Evaluation)** 指的是程序在运行时对表达式各部分进行计算的先后次序。在 C++ 中，表达式的求值顺序（包括函数参数的求值顺序）在大多数情况下是**未指定的 (unspecified)**，这意味着编译器可以以任意顺序对操作数和其他子表达式进行求值，甚至在同一表达式被多次求值时可能选择不同的顺序。

需要注意的是，求值顺序与运算符的结合性 (associativity) 是两个完全不同的概念：
- **结合性**：决定表达式如何解析（语法层面）
- **求值顺序**：决定运行时各部分实际执行的先后次序（运行时层面）

例如，表达式 `a() + b() + c()` 由于 `operator+` 的左结合性被解析为 `(a() + b()) + c()`，但在运行时，`c()` 可能最先被求值，也可能最后被求值，或在 `a()` 和 `b()` 之间被求值。

## 2. 来源与演变 (Origin and Evolution)

### C++11 之前的序列点模型

在 C++11 之前，使用 **序列点 (Sequence Point)** 的概念来描述求值顺序。序列点是执行序列中的一个点，在该点之前所有求值的副作用都已完成，而后续求值的副作用尚未开始。

序列点的典型位置：
- 每个完整表达式的末尾（通常是分号）
- 函数调用时，所有参数求值完成后、函数体执行前
- `&&`、`||`、`?:`、`,` 运算符的第一个操作数求值后

### C++11 起：先序于关系模型

从 C++11 开始，C++ 标准引入了更精确的 **"先序于" (Sequenced Before)** 模型来替代序列点概念。这是一个非对称、可传递的二元关系，用于描述同一线程内求值之间的先后顺序。

该模型提供了更细粒度的控制，明确区分了：
- 值计算 (Value Computation)
- 副作用 (Side Effect)

### C++17 的改进

C++17 对求值顺序规则进行了重要的细化和加强：
- 明确了函数调用中函数表达式的求值顺序
- 明确了下标表达式、成员指针访问表达式等的新规则
- 强化了赋值表达式的求值顺序保证
- 减少了大量历史遗留的未定义行为

## 3. 语法与参数 (Syntax and Parameters)

本节内容描述的是语言规则而非特定语法构造。

### 求值的两个组成部分

每个表达式的求值包括两个独立的部分：

| 组成部分 | 英文 | 说明 |
|---------|------|------|
| **值计算** | Value Computation | 计算表达式返回的值。可能涉及确定对象身份（glvalue 求值，如表达式返回某对象的引用）或读取对象先前赋予的值（prvalue 求值，如表达式返回数值） |
| **副作用** | Side Effect | 访问 volatile glvalue 指定的对象、修改对象、调用库 I/O 函数，或调用执行这些操作的函数 |

### 先序于关系定义

对于同一线程内的求值 A 和 B，它们之间可能存在以下关系：

| 关系类型 | 英文 | 含义 |
|---------|------|------|
| **A 先序于 B** | A is sequenced before B | A 的求值在 B 开始之前完成 |
| **A 后序于 B** | A is sequenced after B | B 的求值在 A 开始之前完成 |
| **无序** | Unsequenced | A 和 B 可以任意顺序执行且可能重叠（编译器可交错执行指令） |
| **不确定序** | Indeterminately Sequenced | A 和 B 可以任意顺序执行但不能重叠；本次是一个顺序，下次可能相反 |

### 关键运算符的求值顺序保证

以下运算符明确保证了求值顺序：

| 运算符 | 求值顺序 |
|--------|---------|
| `&&` (逻辑与) | 左操作数先求值，仅在必要时求值右操作数 |
| `\|\|` (逻辑或) | 左操作数先求值，仅在必要时求值右操作数 |
| `,` (逗号) | 左操作数先求值，然后求值右操作数 |
| `?:` (条件) | 第一个表达式先求值，然后求值第二或第三个表达式 |

## 4. 底层原理 (Underlying Principles)

### 先序于规则（C++11 起）

以下规则定义了求值之间的先序于关系：

#### 基本规则

| 规则编号 | 描述 |
|----------|------|
| **规则 1** | 每个完整表达式 (full-expression) 的值计算和副作用先序于下一个完整表达式的每个值计算和副作用 |
| **规则 2** | 任何运算符的操作数的值计算（非副作用）先序于该运算符结果的值计算 |
| **规则 3** | 调用函数时，每个参数表达式和指明被调用函数的后缀表达式的值计算和副作用，先序于被调用函数体内的每个表达式或语句的执行 |

#### 运算符相关规则

| 规则编号 | 运算符 | 描述 |
|----------|--------|------|
| **规则 4** | 后置 `++`/`--` | 值计算先序于其副作用 |
| **规则 5** | 前置 `++`/`--` | 副作用先序于其值计算 |
| **规则 6** | `&&`、`\|\|`、`,` | 第一个（左）参数的所有求值先序于第二个（右）参数的所有求值 |
| **规则 7** | `?:` | 第一个表达式的所有求值先序于第二或第三个表达式的所有求值 |
| **规则 8** | `=`、`@=` | 左右参数的值计算先序于赋值的副作用，赋值的副作用先序于赋值表达式的值计算 |
| **规则 9** | 列表初始化 | 每个初始化子句的求值先序于花括号列表中其后任何初始化子句的求值 |

#### 函数调用规则

| 规则编号 | 描述 |
|----------|------|
| **规则 10** | 一个函数调用若未被先序于或后序于另一个表达式求值，则该调用与该表达式求值呈不确定序（程序必须表现得如同函数调用的指令不与其他表达式的指令交错） |
| **规则 11** | new 表达式中，对分配函数 (operator new) 的调用与构造函数参数的求值关系：C++17 前为不确定序，C++17 起为先序于 |
| **规则 12** | 函数返回时，函数调用结果临时对象的复制初始化先序于 return 语句操作数末尾所有临时对象的析构，后者先序于包围 return 语句的块的局部变量析构 |

#### C++17 新增规则

| 规则编号 | 表达式类型 | 描述 |
|----------|-----------|------|
| **规则 13** | 函数调用 | 命名函数的表达式先序于每个参数表达式和每个默认参数 |
| **规则 14** | 函数调用 | 每个参数初始化的值计算和副作用与其他参数呈不确定序 |
| **规则 15** | 重载运算符 | 使用运算符记法调用时遵循内置运算符的排序规则 |
| **规则 16** | `E1[E2]` | E1 的所有求值先序于 E2 的所有求值 |
| **规则 17** | `E1.*E2`、`E1->*E2` | E1 的所有求值先序于 E2 的所有求值（除非 E1 的动态类型不包含 E2 所指的成员） |
| **规则 18** | `E1 << E2`、`E1 >> E2` | E1 的所有求值先序于 E2 的所有求值 |
| **规则 19** | `E1 = E2`、`E1 @= E2` | E2 的所有求值先序于 E1 的所有求值 |
| **规则 20** | 括号初始化列表 | 逗号分隔的表达式按函数调用方式求值（不确定序） |

### C++17 的特殊情况

标准库算法在 `std::execution::par_unseq` 执行策略下进行的函数调用是无序的 (unsequenced)，可以任意交错。

### 未定义行为的情况

以下情况会导致未定义行为：

| 情况 | 描述 |
|------|------|
| **情况 1** | 同一内存位置的多个副作用无序 |
| **情况 2** | 同一内存位置的副作用与值计算无序 |
| **情况 3** | 对象生命周期的开始或结束与同一内存位置的副作用或值计算无序 |

## 5. 使用场景 (Use Cases)

### 需要特别注意求值顺序的场景

#### 1. 函数调用中的多个参数修改同一变量

参数求值顺序未指定，可能导致不确定结果：

```cpp
int i = 0;
void f(int a, int b) { /* ... */ }
f(++i, ++i);  // 危险！参数求值顺序未指定
```

#### 2. 链式操作中使用自增/自减运算符

同一表达式中多次修改同一变量可能引发未定义行为：

```cpp
int i = 0;
std::cout << i << i++;  // 未定义行为（C++17 前）
```

#### 3. 复杂表达式中的副作用

需要确保副作用的发生顺序符合预期：

```cpp
int arr[10];
int i = 0;
arr[i] = i++;  // 未定义行为（C++17 前）
```

### 最佳实践

| 实践 | 说明 |
|------|------|
| **分离修改和读取** | 避免在同一表达式中既修改又读取同一变量 |
| **使用序列点** | 通过完整表达式（分号）分隔有副作用的操作 |
| **显式求值顺序** | 使用临时变量显式控制求值顺序 |
| **避免过度依赖** | 不依赖编译器特定的求值顺序 |
| **单一修改原则** | 每个完整表达式中对同一变量最多修改一次 |

### 常见陷阱

| 陷阱 | 示例 | 风险 |
|------|------|------|
| 函数参数顺序依赖 | `f(a(), b(), c())` | 参数求值顺序未指定 |
| 链式输出中的自增 | `cout << i << i++` | 未定义行为（C++17 前） |
| 数组下标与自增混合 | `a[i] = i++` | 未定义行为（C++17 前） |
| 同一表达式中多次自增 | `i = ++i + i++` | 未定义行为 |
| 联合体成员并发修改 | `(u.x = 1, 0) + (u.y = 2, 0)` | 未定义行为 |

## 6. 代码示例 (Examples)

### 基础示例：函数参数求值顺序未指定

```cpp
#include <cstdio>

int a() { return std::puts("a"); }
int b() { return std::puts("b"); }
int c() { return std::puts("c"); }

void z(int, int, int) {}

int main()
{
    z(a(), b(), c());       // 允许 6 种排列中的任意一种
    return a() + b() + c(); // 允许 6 种排列中的任意一种
}
```

**可能的输出**（注意输出顺序可能不同）：

```
b
c
a
c
a
b
```

**说明**：函数参数 `a()`、`b()`、`c()` 的求值顺序是未指定的，编译器可以任意顺序调用这三个函数。

### 未定义行为示例

#### 示例 1：同一内存位置的多个副作用

```cpp
int i = 0;

// 良定义
i = ++i + 2;       // i 最终为 2，副作用和值计算有序

// C++17 前未定义行为，C++17 起良定义但结果未指定
i = i++ + 2;       // 危险！

// C++17 前未定义行为：两个参数都修改 i
f(i = -2, i = -2);

// C++17 前未定义行为，C++17 起顺序未指定但良定义
f(++i, ++i);

// 未定义行为：多次修改
i = ++i + i++;     // 未定义
```

#### 示例 2：副作用与值计算无序

```cpp
int i = 0;

// C++17 前未定义行为：读取和修改无序
std::cout << i << i++;

// C++17 前未定义行为：读取下标和修改无序
a[i] = i++;

// 未定义行为：修改和读取无序
n = ++i + i;
```

#### 示例 3：对象生命周期与副作用冲突

```cpp
union U { int x, y; } u;

// 未定义行为：联合体成员修改与生命周期冲突
// 同一位置的两个修改无序
(u.x = 1, 0) + (u.y = 2, 0);
```

### 正确用法示例

#### 示例 4：显式控制求值顺序

```cpp
// 错误：依赖未指定的求值顺序
int result = compute(a(), b(), c());

// 正确：使用临时变量显式控制顺序
auto av = a();
auto bv = b();
auto cv = c();
int result = compute(av, bv, cv);
```

#### 示例 5：安全的自增操作

```cpp
int i = 0;

// 错误：同一表达式中的多次修改
i = i++ + 1;  // 未定义行为（C++17 前）

// 正确：分离操作
i++;
i = i + 1;   // 或 i += 1

// 错误：链式输出中使用自增
std::cout << i << i++;  // 未定义行为（C++17 前）

// 正确：分开输出
std::cout << i;
i++;
std::cout << i;
```

#### 示例 6：理解运算符结合性与求值顺序的区别

```cpp
#include <iostream>

int a() { std::cout << "a() "; return 1; }
int b() { std::cout << "b() "; return 2; }
int c() { std::cout << "c() "; return 3; }

int main()
{
    // 结合性：左到右，解析为 (a() + b()) + c()
    // 求值顺序：a(), b(), c() 可以任意顺序
    int result = a() + b() + c();
    std::cout << "= " << result << std::endl;

    return 0;
}
```

**可能的输出顺序**：
- `a() b() c() = 6`
- `b() a() c() = 6`
- `c() a() b() = 6`
- 等等（共 6 种可能）

**说明**：虽然运算符结合性决定了表达式解析为 `(a() + b()) + c()`，但运行时三个函数的调用顺序是未指定的。

#### 示例 7：利用短路求值的运算符

```cpp
#include <iostream>

bool check_a() {
    std::cout << "check_a() called" << std::endl;
    return false;
}

bool check_b() {
    std::cout << "check_b() called" << std::endl;
    return true;
}

int main() {
    // && 运算符保证左操作数先求值
    // 由于 check_a() 返回 false，check_b() 不会被调用
    if (check_a() && check_b()) {
        std::cout << "Both true" << std::endl;
    }

    // || 运算符保证左操作数先求值
    // 由于 check_a() 返回 false，check_b() 会被调用
    if (check_a() || check_b()) {
        std::cout << "At least one true" << std::endl;
    }

    return 0;
}
```

**输出**：

```
check_a() called
check_a() called
check_b() called
At least one true
```

### C++11 之前：序列点示例

```cpp
int i = 0;

// 未定义行为：两个序列点之间多次修改
i = ++i + i++;    // 未定义行为
i = i++ + 1;      // 未定义行为
i = ++i + 1;      // 未定义行为
++ ++i;           // 未定义行为
f(++i, ++i);      // 未定义行为
f(i = -1, i = -1);// 未定义行为

// 未定义行为：修改与读取无序
std::cout << i << i++;  // 未定义行为
a[i] = i++;             // 未定义行为
```

## 7. 总结 (Summary)

### 核心要点

1. **求值顺序大多未指定**：C++ 中大多数表达式的求值顺序是未指定的，编译器可自由选择

2. **结合性 ≠ 求值顺序**：运算符结合性决定语法解析，求值顺序决定运行时执行次序

3. **先序于关系（C++11 起）**：现代 C++ 使用"先序于 (Sequenced Before)"模型精确定义求值顺序关系

4. **未定义行为的根源**：
   - 同一内存位置的多个副作用无序
   - 副作用与值计算无序
   - 对象生命周期操作与副作用无序

5. **C++17 改进**：加强了多项求值顺序保证，减少了未定义行为

### 编程建议

| 建议 | 说明 |
|------|------|
| 避免复杂表达式 | 将复杂表达式拆分为简单语句 |
| 分离副作用 | 每个完整表达式最多包含一个对同一变量的修改 |
| 使用临时变量 | 显式控制求值顺序，提高代码可读性 |
| 了解序列点 | 理解哪些运算符保证求值顺序（`&&`、`\|\|`、`?:`、`,`） |
| 查阅标准文档 | 对于关键代码，查阅 C++ 标准确认行为 |

### 版本差异对比

| 特性 | C++11 前 | C++11 | C++17 |
|------|----------|-------|-------|
| 模型 | 序列点 (Sequence Point) | 先序于关系 | 先序于关系（细化） |
| `i = i++ + 1` | 未定义行为 | 未定义行为 | 良定义（但顺序未指定） |
| `f(++i, ++i)` | 未定义行为 | 未定义行为 | 良定义（顺序未指定） |
| 赋值运算符求值顺序 | 未完全指定 | 未完全指定 | 明确规定 |
| 函数表达式求值顺序 | 未指定 | 未指定 | 先序于参数求值 |
| 下标表达式求值顺序 | 未指定 | 未指定 | E1 先序于 E2 |

### 缺陷报告

| DR 编号 | 应用版本 | 原行为 | 修正行为 |
|---------|---------|--------|---------|
| CWG 1885 | C++11 | 函数返回时自动变量析构的顺序不明确 | 添加排序规则 |
| CWG 1949 | C++11 | "先序于"的反向关系未定义 | 定义为"后序于" |
| CWG 1953 | C++11 | 副作用和值计算可能与对象生命周期操作无序 | 此情况定义为未定义行为 |
| CWG 2146 | C++98 | 未定义行为情况未考虑位域 | 已考虑 |

### 参考资料

- **C++23 标准 (ISO/IEC 14882:2024)**:
  - 6.9.1 Program execution [intro.execution]
  - 7.6.1.6 Increment and decrement [expr.post.incr]
  - 7.6.2.8 New [expr.new]
  - 7.6.14 Logical AND operator [expr.log.and]
  - 7.6.15 Logical OR operator [expr.log.or]
  - 7.6.16 Conditional operator [expr.cond]
  - 7.6.19 Assignment and compound assignment operators [expr.ass]
  - 7.6.20 Comma operator [expr.comma]
  - 9.4.5 List-initialization [dcl.init.list]

- **C++20 标准 (ISO/IEC 14882:2020)**:
  - 6.9.1 Program execution [intro.execution]
  - 7.6.1.5 Increment and decrement [expr.post.incr]
  - 7.6.2.7 New [expr.new]
  - 7.6.14 Logical AND operator [expr.log.and]
  - 7.6.15 Logical OR operator [expr.log.or]
  - 7.6.16 Conditional operator [expr.cond]
  - 7.6.19 Assignment and compound assignment operators [expr.ass]
  - 7.6.20 Comma operator [expr.comma]
  - 9.4.4 List-initialization [dcl.init.list]

- **C++17 标准 (ISO/IEC 14882:2017)**:
  - 4.6 Program execution [intro.execution]
  - 8.2.6 Increment and decrement [expr.post.incr]
  - 8.3.4 New [expr.new]
  - 8.14 Logical AND operator [expr.log.and]
  - 8.15 Logical OR operator [expr.log.or]
  - 8.16 Conditional operator [expr.cond]
  - 8.18 Assignment and compound assignment operators [expr.ass]
  - 8.19 Comma operator [expr.comma]
  - 11.6.4 List-initialization [dcl.init.list]

- **C++14 标准 (ISO/IEC 14882:2014)**:
  - 1.9 Program execution [intro.execution]
  - 5.2.6 Increment and decrement [expr.post.incr]
  - 5.3.4 New [expr.new]
  - 5.14 Logical AND operator [expr.log.and]
  - 5.15 Logical OR operator [expr.log.or]
  - 5.16 Conditional operator [expr.cond]
  - 5.17 Assignment and compound assignment operators [expr.ass]
  - 5.18 Comma operator [expr.comma]
  - 8.5.4 List-initialization [dcl.init.list]

- **C++11 标准 (ISO/IEC 14882:2011)**:
  - 1.9 Program execution [intro.execution]
  - 5.2.6 Increment and decrement [expr.post.incr]
  - 5.3.4 New [expr.new]
  - 5.14 Logical AND operator [expr.log.and]
  - 5.15 Logical OR operator [expr.log.or]
  - 5.16 Conditional operator [expr.cond]
  - 5.17 Assignment and compound assignment operators [expr.ass]
  - 5.18 Comma operator [expr.comma]
  - 8.5.4 List-initialization [dcl.init.list]

### 另见

- [运算符优先级 (Operator Precedence)](operator_precedence.md) - 定义表达式如何从源代码表示构建
- [表达式 (Expressions)](../expressions.md) - C++ 表达式概述