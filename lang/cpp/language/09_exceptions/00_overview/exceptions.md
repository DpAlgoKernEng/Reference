# C++ 异常处理 (Exceptions)

## 1. 概述

异常处理（Exception Handling）提供了一种将控制权和信息从程序执行的某一点转移到与之前执行经过的某一点相关联的处理程序的机制。换言之，异常处理将控制权沿调用栈向上传递。

当计算 `throw` 表达式时会抛出异常。异常也可以在其他上下文中抛出。

为了捕获异常，`throw` 表达式必须位于 `try` 块内，且该 `try` 块必须包含与异常对象类型匹配的处理程序（handler）。

异常处理是 C++ 中错误处理的核心机制，定义在 `<exception>` 头文件中。

## 2. 来源与演变

### 首次引入

异常处理机制在 **C++98** 标准中首次引入，成为 C++ 语言的核心特性之一。

### 历史背景

在异常处理出现之前，C++ 开发者主要使用以下方式进行错误处理：

1. **返回值检查**：函数通过返回特殊值（如 `-1`、`nullptr`）表示错误
2. **全局错误变量**：使用类似 `errno` 的全局变量
3. **回调函数**：注册错误处理回调

这些方法存在以下问题：
- 错误处理代码与正常逻辑混杂
- 容易忽略错误检查
- 无法自动传递错误信息

异常机制的引入解决了这些问题：
- 错误处理代码集中
- 自动栈展开（Stack Unwinding）
- 类型安全的错误传递

### C++11 变化

- 引入 `noexcept` 说明符，替代动态异常说明
- 引入 `noexcept` 运算符，用于检查函数是否抛出异常
- 析构函数默认标记为 `noexcept`

### C++17 变化

- 移除动态异常说明（Dynamic Exception Specifications）
- 保留 `throw()` 作为 `noexcept(true)` 的别名（已弃用）

### C++26 变化

- 新增 `__cpp_constexpr_exceptions` 特性测试宏
- 支持 `constexpr` 上下文中的异常处理

## 3. 语法与参数

### 基本语法

#### throw 表达式

```cpp
throw expression;  // 抛出异常
throw;             // 重新抛出当前异常
```

#### try-catch 块

```cpp
try {
    // 可能抛出异常的代码
} catch (const Type1& e) {
    // 处理 Type1 类型的异常
} catch (const Type2& e) {
    // 处理 Type2 类型的异常
} catch (...) {
    // 捕获所有其他类型的异常
}
```

#### 函数异常说明

```cpp
// C++11 及以后
void func() noexcept;           // 不抛出异常
void func() noexcept(true);      // 同上
void func() noexcept(false);     // 可能抛出异常
void func();                     // 可能抛出异常（默认）

// C++17 已移除
void func() throw(Type1, Type2); // 动态异常说明（已弃用）
void func() throw();             // 不抛出异常（已弃用，等同于 noexcept）
```

### 异常说明类型

| 说明符 | 版本 | 含义 |
|--------|------|------|
| `throw(type-list)` | C++98 (C++17 移除) | 动态异常说明，列出可能抛出的异常类型 |
| `throw()` | C++98 (C++17 弃用) | 函数不抛出异常 |
| `noexcept` | C++11 起 | 函数不抛出异常 |
| `noexcept(expression)` | C++11 起 | 根据表达式决定是否抛出异常 |
| `noexcept` (运算符) | C++11 起 | 检查表达式是否可能抛出异常 |

### 异常对象类型要求

- 可以抛出任何完整类型的对象
- 可以抛出指向 `void` 的 cv 限定指针
- 标准库函数抛出的异常对象均派生自 `std::exception`
- 最佳实践：捕获异常时使用引用，避免对象切片

## 4. 底层原理

### 异常处理流程

当异常被抛出时，运行时系统执行以下步骤：

1. **异常对象构造**：在特殊内存区域（异常安全存储区）构造异常对象
2. **栈展开（Stack Unwinding）**：沿调用栈向上搜索匹配的 catch 块
3. **析构局部对象**：在栈展开过程中，自动析构所有局部对象
4. **匹配处理程序**：找到类型匹配的 catch 块后，将控制权转移到该处理程序
5. **异常对象销毁**：异常处理完成后，销毁异常对象

### 异常安全保证级别

| 保证级别 | 说明 | 示例 |
|----------|------|------|
| **无抛出保证 (Nothrow/Nofail)** | 函数永不抛出异常 | 析构函数、`swap`、移动构造函数 |
| **强异常保证 (Strong)** | 异常抛出后，程序状态回滚到函数调用前 | `std::vector::push_back` |
| **基本异常保证 (Basic)** | 异常抛出后，程序处于有效状态，无资源泄漏 | 大多数标准库操作 |
| **无异常保证 (None)** | 异常抛出后，程序可能处于无效状态 | 部分底层操作 |

### 异常中性格子保证

泛型组件可能提供**异常中性格子保证**：如果模板参数抛出异常（如 `std::sort` 的比较函数或 `std::make_shared` 中 `T` 的构造函数），异常将原封不动地传播给调用者。

### 时间与空间开销

| 方面 | 说明 |
|------|------|
| 正常执行路径开销 | 零成本或接近零成本（"零成本异常"模型） |
| 异常抛出开销 | 较高，需要栈展开和类型匹配 |
| 代码体积开销 | 需要额外的异常处理表 |

## 5. 使用场景

### 错误处理场景

异常应用于以下错误情况：

| 场景 | 说明 |
|------|------|
| 后置条件失败 | 函数无法产生有效的返回值对象 |
| 前置条件失败 | 无法满足被调用函数的前置条件 |
| 类不变量破坏 | 非私有成员函数无法（重新）建立类不变量 |

特别注意：
- 构造函数失败应通过抛出异常报告（参见 RAII）
- 大多数运算符失败应通过抛出异常报告
- 宽契约函数（Wide Contract Functions）使用异常表示不可接受的输入

### 宽契约函数示例

`std::basic_string::at()` 没有前置条件，但抛出异常表示索引越界：

```cpp
// at() 是宽契约函数：任何索引都可以传入
// 但越界时会抛出 std::out_of_range
char c = str.at(10);  // 如果索引越界，抛出异常
```

### 最佳实践

1. **抛出异常**：使用按值抛出（Throw by Value）
2. **捕获异常**：使用引用捕获（Catch by Reference），避免对象切片
3. **异常类型**：使用派生自 `std::exception` 的自定义类型
4. **析构函数**：永不抛出异常（C++11 起默认为 `noexcept`）

### 不应使用异常的场景

| 场景 | 原因 |
|------|------|
| 控制流 | 异常应仅用于错误处理，不应替代正常的控制流 |
| 性能关键代码 | 异常抛出有较高开销 |
| 嵌入式系统 | 可能禁用异常以减小代码体积 |

### 线程安全性

异常处理本身是线程安全的，每个线程有独立的异常栈。但需要注意：

- 一个线程抛出的异常无法被另一个线程捕获
- 使用 `std::exception_ptr` 可在线程间传递异常

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

// 自定义异常类
class MyException : public std::runtime_error {
public:
    explicit MyException(const std::string& msg)
        : std::runtime_error(msg) {}
};

// 可能抛出异常的函数
double divide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

int main() {
    try {
        double result = divide(10.0, 0.0);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown exception caught" << std::endl;
    }

    return 0;
}
// 输出: Invalid argument: Division by zero
```

### 高级用法：RAII 与异常安全

```cpp
#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>

// RAII 资源管理类
class FileHandle {
    FILE* file_;
public:
    explicit FileHandle(const char* filename)
        : file_(std::fopen(filename, "r")) {
        if (!file_) {
            throw std::runtime_error("Cannot open file");
        }
    }

    ~FileHandle() noexcept {
        if (file_) {
            std::fclose(file_);
        }
    }

    // 禁止拷贝
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FILE* get() const noexcept { return file_; }
};

// 强异常安全保证的函数
void processData(const std::vector<int>& data) {
    std::vector<int> temp = data;  // 先复制

    // 处理 temp...
    for (auto& x : temp) {
        x *= 2;
    }

    // 如果上面都成功，才交换
    // swap 是 noexcept 操作
    const_cast<std::vector<int>&>(data) = std::move(temp);
}

// noexcept 函数示例
void safeFunction() noexcept {
    // 此函数保证不抛出异常
    // 如果内部抛出异常，程序将调用 std::terminate
}

int main() {
    try {
        FileHandle file("example.txt");
        // 使用文件...
    }
    catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    safeFunction();

    return 0;
}
```

### 常见错误及修正

#### 错误 1：析构函数抛出异常

```cpp
// 错误：析构函数抛出异常
class BadClass {
public:
    ~BadClass() {
        throw std::runtime_error("Error in destructor");  // 危险！
    }
};

// 修正：析构函数不应抛出异常
class GoodClass {
public:
    ~GoodClass() noexcept {
        try {
            // 可能抛出异常的操作
        }
        catch (...) {
            // 捕获并处理，不要让异常逃逸
            std::cerr << "Error in destructor" << std::endl;
        }
    }
};
```

#### 错误 2：按值捕获异常导致对象切片

```cpp
#include <stdexcept>

class BaseException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Base exception";
    }
};

class DerivedException : public BaseException {
public:
    const char* what() const noexcept override {
        return "Derived exception";
    }
};

int main() {
    try {
        throw DerivedException();
    }
    catch (BaseException e) {  // 错误：按值捕获，发生对象切片
        // e.what() 返回 "BaseException"，丢失了 DerivedException 的信息
    }
}

// 修正：使用引用捕获
int main() {
    try {
        throw DerivedException();
    }
    catch (const BaseException& e) {  // 正确：引用捕获，多态生效
        // e.what() 返回 "Derived exception"
    }
}
```

#### 错误 3：空 throw 语句使用不当

```cpp
void dangerousFunction() {
    try {
        throw std::runtime_error("Error");
    }
    catch (...) {
        // 处理异常...
        throw;  // 重新抛出当前异常
    }
}

void unsafeFunction() {
    throw;  // 错误！没有活动异常可重新抛出，调用 std::terminate
}
```

### 标准异常层次结构

```cpp
#include <iostream>
#include <stdexcept>
#include <typeinfo>

int main() {
    try {
        // 标准库异常示例
        throw std::out_of_range("Index out of range");
    }
    catch (const std::logic_error& e) {
        std::cout << "Logic error: " << e.what() << std::endl;
    }

    try {
        throw std::runtime_error("Runtime error occurred");
    }
    catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    }

    return 0;
}
```

## 7. 总结

C++ 异常处理是一个强大而灵活的错误处理机制，提供了以下核心能力：

### 核心要点

| 特性 | 说明 |
|------|------|
| **控制流转移** | 异常将控制权沿调用栈向上传递 |
| **自动清理** | 栈展开过程自动析构局部对象 |
| **类型安全** | 异常对象有明确的类型，支持多态 |
| **异常说明** | `noexcept` 指定函数是否抛出异常 |

### 异常安全保证对比

| 保证级别 | 使用场景 |
|----------|----------|
| 无抛出保证 | 析构函数、`swap`、移动构造函数 |
| 强异常保证 | 需要事务语义的操作 |
| 基本异常保证 | 一般函数的最低要求 |
| 无异常保证 | 应避免 |

### 使用建议

1. **抛出异常**：按值抛出，使用派生自 `std::exception` 的类型
2. **捕获异常**：使用 const 引用捕获，避免对象切片
3. **析构函数**：永不抛出异常，使用 `noexcept`
4. **异常安全**：为函数提供至少基本异常安全保证
5. **RAII 模式**：使用 RAII 管理资源，确保异常安全

### 相关概念

| 概念 | 关系 |
|------|------|
| `std::exception` | 所有标准异常的基类 |
| `std::terminate` | 未捕获异常的处理函数 |
| `std::unexpected` | (C++17 前) 违反异常说明的处理函数 |
| `std::exception_ptr` | 异常指针，用于跨线程传递异常 |
| RAII | 资源获取即初始化，确保异常安全的关键模式 |
| `noexcept` | 指定函数是否抛出异常 |

### 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/exceptions
- C++ Core Guidelines: I.10, E.14, E.15
- "The C++ Programming Language" - Bjarne Stroustrup, Appendix E
- "Exceptional C++" - Herb Sutter
- "C++ Coding Standards" - Herb Sutter, Andrei Alexandrescu