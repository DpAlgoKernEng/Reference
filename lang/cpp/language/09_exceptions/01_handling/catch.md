# catch - 异常处理器

## 1. 概述

`catch` 是 C++ 异常处理机制的核心组件，用于捕获并处理 `try` 块中抛出的异常（exception）。catch 子句定义了异常处理器（handler），指定了可以捕获的异常类型以及相应的处理逻辑。

catch 与 `try` 块配合使用，构成了 C++ 异常处理的基本框架。当异常被抛出时，程序控制流会转移到最近的匹配处理器，执行相应的异常处理代码。

catch 定义在 C++ 语言核心中，是异常处理（exception handling）机制的关键部分。

## 2. 来源与演变

### 首次引入

异常处理机制首次在 **C++98** 标准中引入，`catch` 作为异常处理器的语法关键字被定义。

### 历史背景

在异常处理机制出现之前，C++ 开发者主要使用以下方式处理错误：

1. **返回值检查**：函数返回错误码，调用者需要检查返回值
2. **全局变量**：使用 `errno` 等全局变量存储错误状态
3. **回调函数**：注册错误处理回调

异常处理机制的出现解决了这些问题，提供了：
- 自动栈展开（stack unwinding）
- 错误信息与控制流的分离
- 类型安全的错误传递

### C++11 变化

- 新增 `attr`（属性）支持，可以为 catch 参数添加属性
- 新增 `std::nullptr_t` 类型匹配支持
- `throw nullptr` 可以匹配指针类型的处理器

### C++17 变化

- 新增函数指针转换（function pointer conversion）支持

### 缺陷报告修正

| DR | 版本 | 问题 | 修正 |
|----|------|------|------|
| CWG 98 | C++98 | switch 语句可以跳转到处理器中 | 禁止此行为 |
| CWG 210 | C++98 | throw 表达式与处理器匹配 | 改为异常对象与处理器匹配 |
| CWG 388 | C++98 | 指针类型异常无法被 const 引用匹配 | 允许转换匹配 |
| CWG 1166 | C++98 | 抽象类类型的引用处理器行为未定义 | 禁止抽象类类型处理器 |
| CWG 1769 | C++98 | 基类类型处理器可能使用转换构造函数 | 从基类子对象复制初始化 |
| CWG 2093 | C++98 | 指针类型异常无法通过限定转换匹配 | 允许匹配 |

## 3. 语法与参数

### 基本语法

```cpp
// (1) 命名参数处理器
catch (attr(可选) 类型说明符序列 声明符) 复合语句

// (2) 匿名参数处理器
catch (attr(可选) 类型说明符序列 抽象声明符(可选)) 复合语句

// (3) 捕获所有异常
catch (...) 复合语句
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr` | (C++11 起) 任意数量的属性，应用于参数 |
| `类型说明符序列` | 形式参数声明的一部分，与函数参数列表相同 |
| `声明符` | 参数声明的一部分，指定参数名称 |
| `抽象声明符` | 匿名参数声明的一部分，省略参数名称 |
| `复合语句` | 处理器执行体，包含异常处理代码 |

### 处理器类型限制

如果参数声明为以下类型，程序是**非良构**（ill-formed）的：

- 不完整类型（incomplete type）
- 抽象类类型（abstract class type）
- 右值引用类型（rvalue reference type）（C++11 起）
- 指向不完整类型的指针（除了 `void*`）
- 指向不完整类型的左值引用

### 类型调整

如果参数声明为"数组类型 `T`"或函数类型 `T`，类型会被调整为"指向 `T` 的指针"。

## 4. 底层原理

### 异常匹配机制

当异常从 try 块中抛出时，处理器按出现顺序依次尝试匹配异常。

处理器与类型 `E` 的异常对象匹配的条件：

**类型匹配规则**：

1. 处理器类型为"可能带 cv 限定的 `T`"或"对可能带 cv 限定的 `T` 的左值引用"，且满足以下条件之一：
   - `E` 和 `T` 是相同类型（忽略顶层 cv 限定符）
   - `T` 是 `E` 的无歧义公有基类

2. 处理器类型为"可能带 cv 限定的 `T`"或 `const T&`，其中 `T` 是指针或成员指针类型，且满足以下条件之一：
   - `E` 是可以通过标准指针转换转换为 `T` 的指针或成员指针类型
   - `E` 是 `std::nullptr_t`（C++11 起）

### 处理器参数初始化

处理器参数从异常对象初始化：

- 如果 `T` 是 `E` 的基类，参数从异常对象的基类子对象复制初始化
- 否则，参数从异常对象复制初始化

**重要区别**：

| 参数类型 | 行为 |
|---------|------|
| 对象类型 | 修改参数不影响异常对象 |
| 引用类型 | 修改引用对象会影响异常对象，重新抛出时生效 |

### 处理器激活

处理器在参数初始化完成后被认为是**活动的**（active）。

**当前处理的异常**（currently handled exception）是指最近激活且仍处于活动状态的处理器所关联的异常。

### 控制流限制

处理器的复合语句是控制流受限语句：

```cpp
void f()
{
    goto label;     // 错误：不能跳入处理器
    try
    {
        goto label; // 错误：不能从 try 块跳出到处理器内的标签
    }
    catch (...)
    {
        goto label; // 正确：处理器内部可以跳转
        label: ;
    }
}
```

### 栈展开

栈展开（stack unwinding）在控制权转移到处理器时发生。当处理器变为活动状态时，栈展开已经完成。

## 5. 使用场景

### 适合使用 catch 的场景

| 场景 | 说明 |
|------|------|
| 错误恢复 | 捕获可恢复的异常，执行恢复逻辑 |
| 资源清理 | 捕获异常后释放资源 |
| 异常转换 | 捕获底层异常，抛出高层异常 |
| 日志记录 | 记录异常信息用于调试 |
| 兜底处理 | 使用 `catch(...)` 防止未捕获异常 |

### 最佳实践

1. **按引用捕获**：避免对象切片，保留完整异常信息
2. **从具体到一般**：先捕获派生类异常，再捕获基类异常
3. **使用 const 引用**：防止意外修改异常对象
4. **合理使用 `catch(...)`**：仅用于确保 nothrow 保证或清理资源

### 常见陷阱

#### 陷阱 1：处理器顺序错误

```cpp
// 错误：派生类处理器永远不会执行
try {
    f();
}
catch (const std::exception& e) {}  // 先捕获基类
catch (const std::runtime_error& e) {}  // 死代码！
```

#### 陷阱 2：值捕获导致对象切片

```cpp
// 错误：值捕获导致对象切片
catch (std::exception e) {}  // 派生类信息丢失

// 正确：引用捕获保留完整对象
catch (const std::exception& e) {}  // 保留派生类信息
```

#### 陷阱 3：指针类型匹配问题

```cpp
// throw 0 不会匹配指针类型处理器
try {
    throw 0;  // int 类型
}
catch (void* p) {}  // 不匹配！

// C++11 起：使用 throw nullptr
try {
    throw nullptr;  // std::nullptr_t
}
catch (void* p) {}  // 匹配！
```

### 异常安全保证

- 如果没有匹配的处理器，调用 `std::terminate`
- `catch(...)` 可以捕获任何类型异常
- `catch(...)` 必须是处理器序列中的最后一个

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

int main()
{
    // 捕获整数异常
    try
    {
        std::cout << "Throwing an integer exception...\n";
        throw 42;
    }
    catch (int i)
    {
        std::cout << "The integer exception was caught, with value: " << i << '\n';
    }

    // 捕获标准异常
    try
    {
        std::cout << "Creating a vector of size 5...\n";
        std::vector<int> v(5);
        std::cout << "Accessing the 11th element of the vector...\n";
        std::cout << v.at(10); // vector::at() 抛出 std::out_of_range
    }
    catch (const std::exception& e) // 通过基类引用捕获
    {
        std::cout << "A standard exception was caught, with message: '"
                  << e.what() << "'\n";
    }

    return 0;
}
```

输出：
```
Throwing an integer exception...
The integer exception was caught, with value: 42
Creating a vector of size 5...
Accessing the 11th element of the vector...
A standard exception was caught, with message: 'out_of_range'
```

### 高级用法：多级异常处理

```cpp
#include <iostream>
#include <stdexcept>

void f()
{
    try
    {
        throw std::overflow_error("overflow in f()");
    }
    catch (const std::overflow_error& e)
    {
        // 同类型匹配
        std::cout << "Caught overflow_error: " << e.what() << '\n';
    }
    catch (const std::runtime_error& e)
    {
        // 基类匹配：捕获 std::underflow_error 等
        std::cout << "Caught runtime_error: " << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        // 更高层基类匹配
        std::cout << "Caught exception: " << e.what() << '\n';
    }
    catch (...)
    {
        // 捕获所有其他类型
        std::cout << "Caught unknown exception\n";
    }
}
```

### 高级用法：引用捕获与重新抛出

```cpp
#include <iostream>
#include <stdexcept>

void g()
{
    try
    {
        throw std::runtime_error("original error");
    }
    catch (std::runtime_error& e)  // 非常量引用
    {
        e.what();  // 可以修改异常对象
        throw;     // 重新抛出修改后的异常
    }
}
```

### 常见错误及修正

#### 错误 1：处理器顺序导致死代码

```cpp
// 错误：派生类处理器永远不会执行
try {
    f();
}
catch (const std::exception& e) {}  // 先捕获基类
catch (const std::runtime_error& e) {}  // 死代码！

// 修正：从具体到一般排列
try {
    f();
}
catch (const std::runtime_error& e) {}  // 先捕获派生类
catch (const std::exception& e) {}  // 再捕获基类
```

#### 错误 2：catch(...) 位置错误

```cpp
// 错误：catch(...) 后面还有处理器
try {
    f();
}
catch (...) {}  // 捕获所有
catch (int i) {}  // 错误！永远不会执行

// 修正：catch(...) 必须是最后一个
try {
    f();
}
catch (int i) {}
catch (...) {}  // 正确位置
```

#### 错误 3：使用不完整类型

```cpp
// 错误：不完整类型
class Incomplete;  // 前向声明

try {
    f();
}
catch (Incomplete& i) {}  // 错误：不完整类型

// 修正：使用完整类型
#include "Incomplete.h"  // 包含完整定义

try {
    f();
}
catch (Incomplete& i) {}  // 正确
```

## 7. 总结

`catch` 是 C++ 异常处理机制的核心组件，提供了类型安全的异常捕获能力。

**核心特性**：

| 特性 | 说明 |
|------|------|
| 类型匹配 | 支持精确匹配和基类匹配 |
| 引用捕获 | 避免对象切片，保留完整信息 |
| 通配符 | `catch(...)` 捕获所有异常 |
| 重新抛出 | `throw;` 重新抛出当前异常 |

**使用建议**：

1. 优先使用 `const` 引用捕获异常
2. 处理器按从具体到一般的顺序排列
3. `catch(...)` 放在最后，用于兜底处理
4. 避免使用不完整类型、抽象类类型作为处理器参数

**相关概念**：

| 概念 | 关系 |
|------|------|
| `try` 块 | 定义需要监控异常的代码区域 |
| `throw` 表达式 | 抛出异常 |
| `std::exception` | 标准异常基类 |
| `std::terminate` | 未捕获异常时调用 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/catch
- C++23 标准 (ISO/IEC 14882:2024): 14.4 Handling an exception [except.handle]
- C++20 标准 (ISO/IEC 14882:2020): 14.4 Handling an exception [except.handle]
- C++17 标准 (ISO/IEC 14882:2017): 18.3 Handling an exception [except.handle]