# try 块 - 异常处理块

## 1. 概述

`try` 块（try block）是 C++ 异常处理机制的核心组成部分。try 块中抛出的异常可以被关联的 handler（异常处理器）捕获和处理。C++ 提供了两种类型的 try 块：

1. **普通 try 块（Ordinary try block）**：作为语句使用，用于包围可能抛出异常的代码段
2. **函数 try 块（Function try block）**：作为函数体使用，可以捕获函数体内以及构造函数初始化列表中抛出的异常

try 块与 `catch` 子句配合使用，构成完整的异常处理机制。

## 2. 来源与演变

### 首次引入

`try` 块首次在 **C++98** 标准中引入，是 C++ 异常处理机制的基础组成部分。异常处理机制的设计参考了其他编程语言（如 Ada、ML）的经验，为 C++ 提供了结构化的错误处理方式。

### 设计动机

在 try 块引入之前，C++ 开发者主要使用以下方式处理错误：

1. **返回错误码**：函数返回特殊值表示错误，容易忽略
2. **全局错误变量**：类似 C 的 `errno`，不够优雅
3. **setjmp/longjmp**：非结构化跳转，不调用析构函数

try 块的出现解决了这些问题：
- 自动调用析构函数进行资源清理（栈展开）
- 将错误处理代码与正常逻辑分离
- 支持异常类型的层次化匹配

### 标准演进

| 版本 | 变化 |
|------|------|
| C++98 | 首次引入 try 块和异常处理机制 |
| C++11 | 明确线程存储期对象析构函数抛出的异常不被线程函数的 function try block 捕获 |

### 缺陷报告

| DR | 应用版本 | 原始行为 | 修正行为 |
|------|---------|---------|---------|
| CWG 98 | C++98 | switch 语句可以将控制流转移到 try 块的复合语句内 | 禁止此行为 |
| CWG 1167 | C++98 | 析构函数的 function try block 是否捕获基类或成员析构函数的异常未明确 | 明确此类异常会被捕获 |

## 3. 语法与参数

### 基本语法

```cpp
// 1) 普通 try 块
try compound-statement handler-seq

// 2) 函数 try 块
try ctor-initializer(optional) compound-statement handler-seq
```

### 参数说明

| 参数 | 说明 |
|------|------|
| compound-statement | 复合语句（花括号包围的语句块） |
| handler-seq | 非空的 catch 处理器序列 |
| ctor-initializer | 成员初始化列表（仅用于构造函数） |

### 语法形式详解

#### 普通 try 块

普通 try 块是一个语句，用于包围可能抛出异常的代码：

```cpp
try {
    // 可能抛出异常的代码
} catch (const std::exception& e) {
    // 异常处理代码
} catch (...) {
    // 捕获所有其他异常
}
```

#### 函数 try 块

函数 try 块是一种特殊的函数体形式，可以捕获函数体内以及成员初始化列表中抛出的异常：

```cpp
// 构造函数的函数 try 块
ClassName() try : member1(init1), member2(init2) {
    // 构造函数体
} catch (...) {
    // 异常处理代码
}

// 普通函数的函数 try 块
ReturnType functionName() try {
    // 函数体
} catch (...) {
    // 异常处理代码
}
```

## 4. 底层原理

### 异常匹配机制

当 try 块的 compound-statement 中抛出异常时，异常将按顺序与 handler-seq 中的处理器进行匹配：

1. 按照声明顺序依次检查每个 catch 子句
2. 如果异常类型与 catch 子句的参数类型匹配，则进入该处理器
3. 如果没有匹配的处理器，异常继续向外传播

### 栈展开（Stack Unwinding）

当异常被抛出时，会触发栈展开过程：

1. 销毁 try 块内已构造的局部对象（调用析构函数）
2. 如果在析构过程中又抛出异常，调用 `std::terminate`
3. 异常继续向上传播直到找到匹配的处理器

### 控制流限制

try 块的复合语句是控制流受限语句（control-flow-limited statement）：

```cpp
void f()
{
    goto label;     // 错误：不能跳入 try 块
    try
    {
        goto label; // 正确：在 try 块内部跳转
        label: ;
    }
    catch (...)
    {
        goto label; // 错误：不能从 catch 块跳入 try 块
    }
}
```

### 跳出 try 块

跳转语句（`goto`、`break`、`return`、`continue`）可以用于将控制流移出 try 块（包括其处理器）。此时，try 块内声明的每个变量都会在其声明所在的直接上下文中被销毁：

```cpp
try
{
    T1 t1;
    try
    {
        T2 t2;
        goto label; // 先销毁 t2，再销毁 t1
    }
    catch(...)
    {
        // 如果销毁 t2 时抛出异常，在此执行
    }
}
catch(...)
{
    // 如果销毁 t1 时抛出异常，在此执行
}
label: ;
```

## 5. 使用场景

### 普通 try 块适用场景

| 场景 | 说明 |
|------|------|
| 局部异常处理 | 在函数内部捕获和处理特定代码段的异常 |
| 资源管理 | 配合 RAII 确保资源正确释放 |
| 错误恢复 | 捕获异常后进行恢复或替代操作 |

### 函数 try 块适用场景

| 场景 | 说明 |
|------|------|
| 构造函数初始化失败 | 捕获成员初始化列表中抛出的异常 |
| 析构函数异常处理 | 捕获基类或成员析构时抛出的异常 |
| 统一异常日志 | 在函数入口统一记录所有异常 |

### 不适用场景

| 场景 | 原因 |
|------|------|
| 性能关键代码 | 异常处理有一定的性能开销 |
| 构造函数中访问成员 | 在构造函数的 function try block 的 handler 中访问非静态成员是未定义行为 |

### 最佳实践

1. **优先使用 RAII**：让资源的生命周期由对象管理，减少显式 try-catch 的需求
2. **按引用捕获异常**：避免对象切片，使用 `catch (const std::exception& e)`
3. **从具体到一般**：先捕获具体异常类型，最后使用 `catch(...)` 作为兜底
4. **避免在析构函数中抛出异常**：可能导致 `std::terminate` 被调用

### 注意事项

1. **构造函数 function try block 特殊规则**：
   - 在 handler 中引用对象的任何非静态成员或基类会导致未定义行为
   - handler 中出现 `return` 语句会导致程序错误
   - 控制流到达 handler 的末尾时，当前处理的异常会被重新抛出

2. **析构函数 function try block 特殊规则**：
   - 控制流到达 handler 的末尾时，当前处理的异常会被重新抛出

3. **静态存储期对象**：
   - 静态存储期对象的析构函数中抛出的异常，不会被 main 函数的 function try block 捕获
   - 线程存储期对象的析构函数中抛出的异常（C++11），不会被线程入口函数的 function try block 捕获

## 6. 代码示例

### 基础用法：普通 try 块

```cpp
#include <iostream>
#include <stdexcept>

void ordinaryTryBlock() {
    // 普通 try 块示例
    try {
        throw 1;     // 不被下面的 handler 处理
    }
    catch (int e) {
        std::cout << "Caught int: " << e << std::endl;
    }

    try {
        throw 2;     // 被关联的 handler 处理
    }
    catch (...) {
        std::cout << "Caught unknown exception" << std::endl;
    }

    throw 3;         // 不被上面的 handler 处理
}

int main() {
    try {
        ordinaryTryBlock();
    }
    catch (int e) {
        std::cout << "Caught in main: " << e << std::endl;
    }
    return 0;
}
```

### 高级用法：函数 try 块

```cpp
#include <iostream>
#include <stdexcept>

// 辅助函数，可能抛出异常
int f(bool cond) {
    if (cond)
        throw 1;
    return 0;
}

// 类定义，展示构造函数和析构函数的 function try block
struct X {
    int mem;

    // 构造函数的 function try block，捕获初始化列表中的异常
    X() try : mem(f(true)) {
        std::cout << "X constructor body" << std::endl;
    }
    catch (...) {
        std::cout << "Caught exception in X() initializer" << std::endl;
        // 注意：异常会被重新抛出
    }

    // 另一个构造函数的 function try block，捕获函数体内的异常
    X(int) try {
        throw 2;
    }
    catch (...) {
        std::cout << "Caught exception in X(int) body" << std::endl;
        // 注意：异常会被重新抛出
    }
};

int main() {
    try {
        X x1;     // 初始化列表抛出异常
    }
    catch (int e) {
        std::cout << "Exception propagated: " << e << std::endl;
    }

    try {
        X x2(42); // 函数体抛出异常
    }
    catch (int e) {
        std::cout << "Exception propagated: " << e << std::endl;
    }

    return 0;
}
```

### 构造函数和析构函数的 function try block

```cpp
#include <iostream>

int f(bool cond = true) {
    if (cond)
        throw 1;
    return 0;
}

struct X {
    int mem = f();  // 默认成员初始化器

    ~X() {
        throw 2;    // 析构函数抛出异常
    }
};

struct Y {
    X mem;

    // 构造函数 function try block
    // 捕获成员初始化时抛出的异常
    Y() try {}
    catch (...) {
        std::cout << "Caught exception during Y member initialization" << std::endl;
    }

    // 析构函数 function try block
    // 捕获成员析构时抛出的异常
    ~Y() try {}
    catch (...) {
        std::cout << "Caught exception during Y member destruction" << std::endl;
    }
};

int main() {
    try {
        Y y;  // 初始化 Y::mem (类型 X) 时抛出异常
    }
    catch (int e) {
        std::cout << "Final catch: " << e << std::endl;
    }
    return 0;
}
```

### 常见错误及修正

#### 错误 1：在构造函数 function try block handler 中访问成员

```cpp
// 错误：在 handler 中访问非静态成员
struct Bad {
    int member;

    Bad() try : member(0) {
        throw std::runtime_error("Constructor failed");
    }
    catch (...) {
        // 未定义行为！不能访问非静态成员
        // std::cout << member;  // 错误！
    }
};

// 正确：仅进行日志记录或清理，不访问成员
struct Good {
    int member;

    Good() try : member(0) {
        throw std::runtime_error("Constructor failed");
    }
    catch (const std::exception& e) {
        std::cerr << "Construction failed: " << e.what() << std::endl;
        // 异常会自动重新抛出
    }
};
```

#### 错误 2：在构造函数 function try block handler 中使用 return

```cpp
// 错误：在构造函数的 handler 中使用 return
struct Bad {
    Bad() try {
        throw std::runtime_error("Error");
    }
    catch (...) {
        return;  // 编译错误！
    }
};

// 正确：让异常自然重新抛出
struct Good {
    Good() try {
        throw std::runtime_error("Error");
    }
    catch (...) {
        // 什么都不做，异常会自动重新抛出
    }
};
```

#### 错误 3：试图捕获静态对象的析构异常

```cpp
// 静态对象
static X staticObj;

int main() try {
    // ...
    return 0;
}
catch (...) {
    // 注意：main 函数的 function try block
    // 不会捕获 staticObj 析构时抛出的异常
}
```

## 7. 总结

`try` 块是 C++ 异常处理机制的基础，提供了两种形式：

### 核心要点

| 特性 | 普通 try 块 | 函数 try 块 |
|------|------------|------------|
| 用途 | 包围代码块 | 作为函数体 |
| 捕获范围 | 仅 try 块内的代码 | 函数体 + 成员初始化列表 |
| 适用函数 | 不适用 | 所有函数，特别适用于构造/析构函数 |
| handler 后行为 | 可继续执行 | 构造/析构函数会重新抛出异常 |

### 关键注意事项

1. **构造函数 function try block**：handler 结束后异常自动重新抛出，不能使用 return
2. **控制流限制**：不能使用 goto 跳入 try 块或 catch 块
3. **异常重新抛出**：构造函数和析构函数的 handler 结束时会自动重新抛出异常
4. **成员访问**：构造/析构函数 handler 中访问非静态成员是未定义行为

### 使用建议

1. 优先使用 RAII 管理资源，减少显式 try-catch
2. 构造函数使用 function try block 捕获初始化失败
3. 按引用捕获异常，从具体到一般排列 catch 子句
4. 避免在析构函数中抛出异常

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/try
- C++ Standard: [except.handle]
- Effective C++, Scott Meyers, Item 29: Strive for exception-safe code