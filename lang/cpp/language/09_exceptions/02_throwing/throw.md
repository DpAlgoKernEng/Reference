# throw 表达式 - 抛出异常

## 1. 概述

`throw` 表达式是 C++ 异常处理机制的核心组件，用于抛出异常并将控制权转移给异常处理器（handler）。当程序遇到无法就地处理的错误情况时，可以通过 throw 表达式将异常抛出，由调用链上层的适当处理器捕获并处理。

异常可以从以下来源抛出：
- **throw 表达式**：显式抛出异常
- **分配函数**：内存分配失败时
- **`dynamic_cast`**：引用类型的转换失败时
- **`typeid`**：解引用空指针时
- **new 表达式**：内存分配失败时
- **标准库函数**：各种标准库操作失败时

throw 表达式定义在 C++ 语言核心中，无需包含特定头文件，但通常与 `<stdexcept>` 标准异常类一起使用。

## 2. 来源与演变

### 首次引入

异常处理机制首次在 **C++98** 标准中引入，作为 C++ 语言的一项核心特性。该机制借鉴了其他语言（如 Ada、ML）的异常处理设计，旨在提供一种结构化的错误处理方式。

### 历史背景

在异常处理机制出现之前，C++ 开发者主要使用以下方式处理错误：

1. **返回错误码**：函数返回特殊值表示错误，容易被忽略
2. **全局错误变量**：类似 C 的 `errno`，存在线程安全问题
3. **断言**：仅适用于调试阶段，不能用于运行时错误
4. **`setjmp`/`longjmp`**：非结构化跳转，不调用析构函数

异常处理机制的引入解决了这些问题：
- 强制调用者处理错误（未捕获异常会终止程序）
- 自动清理栈上的对象（栈展开）
- 将正常流程与错误处理分离

### C++11 变化

- 引入 `std::exception_ptr`，支持跨线程传递异常
- 引入 `std::nested_exception`，支持嵌套异常
- 异常对象销毁时机更加明确
- 允许在 throw 表达式中对参数进行隐式移动

### C++17 变化

- 移除动态异常规范（dynamic exception specifications）
- `noexcept` 成为异常规范的唯一形式

### 标准文档位置

| 标准 | 章节 |
|------|------|
| C++23 | 7.6.18 [expr.throw], 14.2 [except.throw] |
| C++20 | 7.6.18 [expr.throw], 14.2 [except.throw] |
| C++17 | 8.17 [expr.throw], 18.1 [except.throw] |
| C++14 | 15.1 [except.throw] |
| C++11 | 15.1 [except.throw] |
| C++03 | 15.1 [except.throw] |
| C++98 | 15.1 [except.throw] |

## 3. 语法与参数

### 基本语法

```cpp
throw expression;    // (1) 抛出新异常
throw;               // (2) 重新抛出当前异常
```

### 语法形式说明

| 形式 | 语法 | 说明 |
|------|------|------|
| (1) | `throw expression` | 抛出一个新异常，expression 用于构造异常对象 |
| (2) | `throw` | 重新抛出当前正在处理的异常（空 throw） |

### 参数说明

- **expression**：用于构造异常对象的表达式。将执行以下转换：
  1. 数组到指针转换（array-to-pointer conversion）
  2. 函数到指针转换（function-to-pointer conversion）
  3. 移除顶层 cv 限定符

### 异常对象类型限制

以下类型不能作为异常对象类型（程序为非良构）：

| 类型 | 说明 |
|------|------|
| 不完整类型（incomplete type） | 如未完整定义的类 |
| 抽象类类型（abstract class type） | 包含纯虚函数的类 |
| 指向不完整类型的指针 | 除 `void*` 外 |

### 异常对象的构造与析构

给定异常对象类型 `T`：
- 必须能够从 `const T` 类型的左值复制初始化 `T` 类型的对象
- 如果 `T` 是类类型：
  - 选定的构造函数会被 ODR 使用（odr-used）
  - `T` 的析构函数可能被调用

### 表达式类型

`throw` 表达式的类型为 `void`，是一个纯右值（prvalue）。这使得它可以用在其他表达式中：

```cpp
// throw 表达式可以用在条件运算符中
double f(double d) {
    return d > 1e7 ? throw std::overflow_error("too big") : d;
}
```

## 4. 底层原理

### 异常对象的生命周期

抛出异常时，会在动态存储期（dynamic storage duration）创建一个**异常对象**：

1. **内存分配**：异常对象的内存以实现定义的方式分配，保证不使用全局分配函数（`operator new`）
2. **对象构造**：从 throw 表达式的参数复制初始化异常对象
3. **栈展开**：控制流回溯调用栈，析构所有自动存储期的对象
4. **异常处理**：找到匹配的 catch 块并执行
5. **对象销毁**：异常处理完成后销毁异常对象

### 异常对象销毁时机

| 版本 | 销毁时机 |
|------|----------|
| C++11 之前 | 当最后一个活跃处理器退出时（非重新抛出），立即销毁异常对象 |
| C++11 及之后 | 有多个潜在销毁点，由实现选择最后一个销毁点：<br>1. 处理器退出时（非重新抛出）<br>2. 引用该异常的 `std::exception_ptr` 被销毁时 |

### 栈展开（Stack Unwinding）

当异常被抛出后，控制流会沿调用栈向上回溯直到找到匹配的处理器：

```
调用栈:
func_c()  -- throw 发生
func_b()  -- 栈展开，析构局部对象
func_a()  -- 栈展开，析构局部对象
main()    -- 找到 catch 块
```

栈展开过程中：

1. **析构顺序**：按构造完成顺序的逆序析构自动存储期的对象
2. **构造函数异常**：如果异常从构造函数抛出，析构所有已构造的基类和成员
3. **委托构造**：如果委托构造函数在非委托构造完成后抛出异常，对象析构函数被调用
4. **new 表达式**：如果 new 表达式中的构造函数抛出异常，调用匹配的 deallocation 函数

### std::terminate 触发条件

以下情况会调用 `std::terminate`：

| 情况 | 说明 |
|------|------|
| 重新抛出时无异常 | `throw;` 但没有正在处理的异常 |
| 栈展开期间抛出异常 | 异常对象构造后、处理器开始前，任何函数抛出异常 |
| 异常未被捕获 | 异常逃出 `main()`、线程函数、静态对象构造/析构函数 |
| 析构函数抛出异常 | 在栈展开期间（C++98 明确，C++11 进一步澄清） |

### 异常处理性能考虑

| 方面 | 说明 |
|------|------|
| 无异常时开销 | 现代 ABI 使用"零成本"异常，无异常时几乎无开销 |
| 抛出异常开销 | 抛出和捕获异常代价昂贵，涉及栈展开和类型匹配 |
| 代码大小 | 异常处理表会增加可执行文件大小 |

## 5. 使用场景

### 适合使用异常的场景

| 场景 | 原因 |
|------|------|
| 构造函数失败 | 构造函数无法返回错误码 |
| 无法恢复的错误 | 如内存耗尽、资源不可用 |
| 深层调用链错误 | 异常自动传播到合适的处理层 |
| 错误需要强制处理 | 未捕获异常终止程序 |

### 不适合使用异常的场景

| 场景 | 替代方案 | 原因 |
|------|----------|------|
| 频繁发生的"错误" | 返回值/错误码 | 异常开销大 |
| 实时系统 | 错误码 | 确定性执行时间 |
| 性能关键代码 | 错误码 | 避免异常开销 |
| 边界检查 | 断言/返回值 | 预期内的条件 |

### 最佳实践

1. **按值抛出，按引用捕获**
   ```cpp
   throw std::runtime_error("error");  // 按值抛出
   catch (const std::exception& e)      // 按引用捕获
   ```

2. **重新抛出使用空 throw**
   ```cpp
   catch (const std::exception& e) {
       // 做一些处理
       throw;  // 正确：重新抛出原始异常
       // throw e;  // 错误：对象切片，丢失派生类信息
   }
   ```

3. **异常类继承自 std::exception**
   ```cpp
   class MyException : public std::runtime_error {
   public:
       using std::runtime_error::runtime_error;
   };
   ```

4. **避免从析构函数抛出异常**
   ```cpp
   ~MyClass() noexcept {
       // 不要抛出异常
   }
   ```

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 对象切片 | `throw e` 捕获基类引用时 | 使用 `throw;` 重新抛出 |
| 异常安全 | 资源泄露 | 使用 RAII 智能指针 |
| 析构函数异常 | 栈展开期间调用 terminate | 析构函数声明 `noexcept` |
| 未初始化异常 | `throw;` 无活跃异常 | 确保有异常时才重新抛出 |

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

// 自定义异常类
class DivisionByZero : public std::runtime_error {
public:
    DivisionByZero() : std::runtime_error("Division by zero") {}
};

double divide(double a, double b) {
    if (b == 0) {
        throw DivisionByZero();  // 抛出异常
    }
    return a / b;
}

int main() {
    try {
        double result = divide(10, 0);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const DivisionByZero& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown exception" << std::endl;
    }

    return 0;
}
```

### 重新抛出异常

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

void intermediateFunction() {
    try {
        throw std::runtime_error("Original error");
    }
    catch (const std::exception& e) {
        std::cout << "Logging: " << e.what() << std::endl;
        throw;  // 重新抛出原始异常
    }
}

int main() {
    try {
        intermediateFunction();
    }
    catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << std::endl;
        // 输出: Caught: Original error（类型仍为 runtime_error）
    }

    return 0;
}
```

### 栈展开演示

```cpp
#include <iostream>
#include <stdexcept>

struct Resource {
    std::string name;
    Resource(std::string n) : name(std::move(n)) {
        std::cout << name << " acquired\n";
    }
    ~Resource() {
        std::cout << name << " released\n";
    }
};

void funcC() {
    Resource r("Resource C");
    throw std::runtime_error("Error in funcC");
    // Resource C 会在这里被析构
}

void funcB() {
    Resource r("Resource B");
    funcC();
    // Resource B 会在这里被析构
}

void funcA() {
    Resource r("Resource A");
    funcB();
    // Resource A 会在这里被析构
}

int main() {
    try {
        funcA();
    }
    catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    return 0;
}

// 输出:
// Resource A acquired
// Resource B acquired
// Resource C acquired
// Resource C released
// Resource B released
// Resource A released
// Caught: Error in funcC
```

### 构造函数异常处理

```cpp
#include <iostream>
#include <stdexcept>

struct A {
    int n;
    A(int n = 0) : n(n) {
        std::cout << "A(" << n << ") constructed successfully\n";
    }
    ~A() {
        std::cout << "A(" << n << ") destroyed\n";
    }
};

int foo() {
    throw std::runtime_error("error");
}

struct B {
    A a1, a2, a3;

    B() try : a1(1), a2(foo()), a3(3)  // a2 构造失败
    {
        std::cout << "B constructed successfully\n";
    }
    catch(...) {
        std::cout << "B::B() exiting with exception\n";
    }

    ~B() { std::cout << "B destroyed\n"; }
};

struct C : A, B {
    C() try {
        std::cout << "C::C() completed successfully\n";
    }
    catch(...) {
        std::cout << "C::C() exiting with exception\n";
    }

    ~C() { std::cout << "C destroyed\n"; }
};

int main() try {
    C c;  // 构造失败，栈展开
}
catch (const std::exception& e) {
    std::cout << "main() failed to create C with: " << e.what() << "\n";
}

// 输出:
// A(0) constructed successfully
// A(1) constructed successfully
// A(1) destroyed
// B::B() exiting with exception
// A(0) destroyed
// C::C() exiting with exception
// main() failed to create C with: error
```

### 常见错误及修正

#### 错误 1：重新抛出时对象切片

```cpp
#include <stdexcept>

// ❌ 错误：对象切片
void badRethrow() {
    try {
        throw std::out_of_range("index out of range");
    }
    catch (const std::exception& e) {
        throw e;  // 切片！抛出的是 std::exception，不是 std::out_of_range
    }
}

// ✅ 修正：使用空 throw
void goodRethrow() {
    try {
        throw std::out_of_range("index out of range");
    }
    catch (const std::exception& e) {
        throw;  // 正确：重新抛出原始异常对象
    }
}
```

#### 错误 2：在析构函数中抛出异常

```cpp
#include <stdexcept>

// ❌ 错误：析构函数抛出异常
class BadClass {
public:
    ~BadClass() {
        throw std::runtime_error("Error in destructor");
        // 如果在栈展开期间调用，会触发 std::terminate
    }
};

// ✅ 修正：析构函数声明 noexcept
class GoodClass {
public:
    ~GoodClass() noexcept {
        // 处理错误，但不抛出异常
        try {
            // 可能失败的操作
        }
        catch (...) {
            // 捕获并记录，不重新抛出
        }
    }
};
```

#### 错误 3：无异常时重新抛出

```cpp
#include <iostream>
#include <stdexcept>

// ❌ 错误：无活跃异常时调用 throw;
void badCode() {
    throw;  // std::terminate() 被调用！
}

// ✅ 修正：确保只在 catch 块中重新抛出
void goodCode() {
    try {
        throw std::runtime_error("error");
    }
    catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
        throw;  // 正确：有活跃异常
    }
}
```

#### 错误 4：在条件表达式中使用 throw

```cpp
#include <stdexcept>

// ✅ 正确用法：throw 表达式返回 void，可用于条件运算符
double safeDivide(double a, double b) {
    return b != 0 ? a / b : throw std::runtime_error("Division by zero");
}

// 等价于：
double safeDivideVerbose(double a, double b) {
    if (b != 0) {
        return a / b;
    }
    throw std::runtime_error("Division by zero");
}
```

## 注意事项

1. **异常对象生命周期**：异常对象在找到匹配处理器后销毁（或被重新抛出）
2. **按引用捕获**：避免对象切片，保留完整的派生类类型信息
3. **析构函数安全**：析构函数应声明 `noexcept`，不应抛出异常
4. **异常规范**：C++17 起仅使用 `noexcept`，移除了动态异常规范
5. **性能考虑**：异常应用于真正的"异常"情况，不应用于正常控制流

## 相关概念

| 概念 | 关系 |
|------|------|
| `try-catch` | 异常处理块，捕获 throw 抛出的异常 |
| `noexcept` | 函数异常规范，声明函数不抛出异常 |
| `std::exception` | 标准异常基类 |
| `std::terminate` | 未捕获异常时调用的函数 |
| `std::exception_ptr` | C++11 起，支持跨线程传递异常 |
| 栈展开（stack unwinding） | 异常抛出后自动析构局部对象的机制 |

## 7. 总结

`throw` 表达式是 C++ 异常处理的核心机制：

- **两种语法**：`throw expr` 抛出新异常，`throw;` 重新抛出当前异常
- **自动清理**：栈展开确保自动存储期对象正确析构
- **类型安全**：异常对象必须为完整类型，按值抛出按引用捕获
- **强制处理**：未捕获异常导致程序终止

核心使用建议：
1. 按值抛出异常，按引用捕获
2. 重新抛出使用空 `throw;`，避免对象切片
3. 自定义异常继承自 `std::exception`
4. 析构函数声明 `noexcept`，不抛出异常
5. 异常用于真正的"异常"情况，非正常控制流

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/throw
- C++ Standard: [expr.throw], [except.throw]
- Effective C++, Scott Meyers, Item 29: Strive for exception-safe code
- More Effective C++, Scott Meyers, Item 10-12: Exceptions