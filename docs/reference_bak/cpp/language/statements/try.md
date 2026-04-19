# C++ 尝试块详解

  [1. 基本语法与语义](#1-基本语法与语义)  

  [2. try 块结构详解](#2-try-块结构详解)  

  [3. 异常处理机制](#3-异常处理机制)  

  [4. 属性与 try 块](#4-属性与-try-块)  

  [5. 常见应用场景](#5-常见应用场景)  

  [6. 与其他语言特性的交互](#6-与其他语言特性的交互)  

  [7. 最佳实践与代码规范](#7-最佳实践与代码规范)  

  [8. 标准参考](#8-标准参考)  

  [9. 总结](#9-总结)

## 1. 基本语法与语义

### 1.1 try 块语法结构

C++ try 块的基本语法如下：

```cpp
attr(optional) try compound-statement handler-seq
```

**语法元素说明**：

- `attr(optional)`: （从 C++11 开始）任意数量属性的可选序列
- `try`: 关键字，标识 try 块的开始
- `compound-statement`: 复合语句（块），包含可能抛出异常的代码
- `handler-seq`: 异常处理程序序列（一个或多个 catch 子句）

```cpp
#include <iostream>
#include <stdexcept>

void basic_try_block_examples() {
    std::cout << "=== Basic Try Block Examples ===\n";
    
    // 最简单的 try-catch 结构
    try {
        std::cout << "Inside try block\n";
        throw std::runtime_error("Simple exception");
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
    
    // 多个 catch 子句
    try {
        std::cout << "Inside multi-catch try block\n";
        int choice = 2;
        switch (choice) {
            case 1:
                throw std::runtime_error("Runtime error");
            case 2:
                throw std::logic_error("Logic error");
            case 3:
                throw 42; // 抛出整数
        }
    } catch (const std::runtime_error& e) {
        std::cout << "Caught runtime error: " << e.what() << "\n";
    } catch (const std::logic_error& e) {
        std::cout << "Caught logic error: " << e.what() << "\n";
    } catch (...) {
        std::cout << "Caught unknown exception\n";
    }
    
    // 嵌套 try 块
    try {
        std::cout << "Outer try block\n";
        try {
            std::cout << "Inner try block\n";
            throw std::domain_error("Domain error");
        } catch (const std::domain_error& e) {
            std::cout << "Inner catch: " << e.what() << "\n";
            throw; // 重新抛出异常
        }
    } catch (const std::exception& e) {
        std::cout << "Outer catch: " << e.what() << "\n";
    }
}

int main() {
    basic_try_block_examples();
    return 0;
}
```

### 1.2 try 块的本质

try 块提供了一种结构化的异常处理机制，允许程序在遇到错误时优雅地恢复：

```cpp
#include <iostream>
#include <stdexcept>
#include <memory>

class ExceptionDemo {
private:
    std::string name_;
    static int counter_;
    
public:
    ExceptionDemo(const std::string& name) : name_(name) {
        std::cout << "Constructor: " << name_ << " (instance " << ++counter_ << ")\n";
    }
    
    ~ExceptionDemo() {
        std::cout << "Destructor: " << name_ << " (instance " << counter_-- << ")\n";
    }
    
    void risky_operation(bool should_throw) {
        if (should_throw) {
            throw std::runtime_error("Risky operation failed");
        }
        std::cout << "Risky operation succeeded\n";
    }
};

int ExceptionDemo::counter_ = 0;

void try_block_nature() {
    std::cout << "=== Try Block Nature ===\n";
    
    try {
        std::cout << "Entering try block\n";
        ExceptionDemo obj1("Try Block Object 1");
        ExceptionDemo obj2("Try Block Object 2");
        
        obj1.risky_operation(false); // 不抛出异常
        obj2.risky_operation(true);  // 抛出异常
        
        std::cout << "This won't be executed\n";
        
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
        // obj2 和 obj1 会按相反顺序自动销毁
    }
    
    std::cout << "After try-catch block\n";
}

int main() {
    try_block_nature();
    return 0;
}
```

### 1.3 异常安全保证

try 块提供了不同级别的异常安全保证：

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

void exception_safety_guarantees() {
    std::cout << "=== Exception Safety Guarantees ===\n";
    
    // 1. 基本异常安全保证（Basic Guarantee）
    try {
        std::vector<int> vec(1000);
        for (int i = 0; i < 1000; ++i) {
            vec[i] = i;
        }
        
        // 可能抛出异常的操作
        vec.at(10000); // 抛出 std::out_of_range
        
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
        // 程序状态可能已改变，但不会泄漏资源
    }
    
    // 2. 强异常安全保证（Strong Guarantee）
    try {
        std::vector<int> vec1 = {1, 2, 3, 4, 5};
        std::vector<int> vec2 = vec1; // 强异常安全：要么成功，要么无变化
        
        std::cout << "Vector copy succeeded\n";
        std::cout << "Vec1 size: " << vec1.size() << "\n";
        std::cout << "Vec2 size: " << vec2.size() << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "Caught exception during copy: " << e.what() << "\n";
        // 如果抛出异常，原对象保持不变
    }
    
    // 3. 无异常抛出保证（No-throw Guarantee）
    try {
        int x = 42;
        int y = 24;
        std::swap(x, y); // 无异常抛出保证
        
        std::cout << "Swap succeeded: x = " << x << ", y = " << y << "\n";
        
    } catch (...) {
        std::cout << "This should never be executed\n";
    }
}

int main() {
    exception_safety_guarantees();
    return 0;
}
```

## 2. try 块结构详解

### 2.1 基本 try-catch 结构

```cpp
#include <iostream>
#include <stdexcept>
#include <typeinfo>

void basic_try_catch_structure() {
    std::cout << "=== Basic Try-Catch Structure ===\n";
    
    // 1. 单个 catch 子句
    try {
        throw std::runtime_error("Single catch example");
    } catch (const std::runtime_error& e) {
        std::cout << "Caught runtime_error: " << e.what() << "\n";
    }
    
    // 2. 多个 catch 子句
    try {
        int error_type = 2;
        switch (error_type) {
            case 1:
                throw std::invalid_argument("Invalid argument");
            case 2:
                throw std::out_of_range("Out of range");
            case 3:
                throw std::length_error("Length error");
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught invalid_argument: " << e.what() << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Caught out_of_range: " << e.what() << "\n";
    } catch (const std::length_error& e) {
        std::cout << "Caught length_error: " << e.what() << "\n";
    }
    
    // 3. 捕获所有异常
    try {
        throw 42; // 抛出整数
    } catch (const std::exception& e) {
        std::cout << "Caught std::exception: " << e.what() << "\n";
    } catch (...) {
        std::cout << "Caught unknown exception (not derived from std::exception)\n";
        std::cout << "Exception type: " << typeid(std::current_exception()).name() << "\n";
    }
    
    // 4. 重新抛出异常
    try {
        try {
            throw std::domain_error("Domain error");
        } catch (const std::domain_error& e) {
            std::cout << "Inner catch: " << e.what() << "\n";
            throw; // 重新抛出相同的异常
        }
    } catch (const std::exception& e) {
        std::cout << "Outer catch: " << e.what() << "\n";
    }
}

int main() {
    basic_try_catch_structure();
    return 0;
}
```

### 2.2 异常处理程序序列

```cpp
#include <iostream>
#include <stdexcept>
#include <typeinfo>

void exception_handler_sequence() {
    std::cout << "=== Exception Handler Sequence ===\n";
    
    // 1. 精确匹配优先
    try {
        throw std::runtime_error("Exact match test");
    } catch (const std::runtime_error& e) {
        std::cout << "Caught runtime_error (exact match): " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Caught std::exception (base class): " << e.what() << "\n";
    }
    
    // 2. 基类匹配
    try {
        throw std::overflow_error("Base class match test");
    } catch (const std::logic_error& e) {
        std::cout << "Caught logic_error: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Caught runtime_error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Caught std::exception (base class): " << e.what() << "\n";
    }
    
    // 3. 异常对象切片问题
    try {
        std::runtime_error base_exception("Base exception");
        throw base_exception;
    } catch (const std::exception& e) {
        std::cout << "Caught by base class: " << e.what() << "\n";
    }
    
    // 4. 引用 vs 值捕获
    try {
        throw std::runtime_error("Reference vs value test");
    } catch (std::runtime_error e) { // 值捕获（不推荐）
        std::cout << "Caught by value: " << e.what() << "\n";
    } catch (const std::runtime_error& e) { // 引用捕获（推荐）
        std::cout << "Caught by const reference: " << e.what() << "\n";
    }
    
    // 5. 多重继承异常处理
    class CustomException : public std::exception, public std::bad_cast {
    public:
        const char* what() const noexcept override {
            return "Custom exception";
        }
    };
    
    try {
        throw CustomException();
    } catch (const std::bad_cast& e) {
        std::cout << "Caught bad_cast: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Caught std::exception: " << e.what() << "\n";
    }
}

int main() {
    exception_handler_sequence();
    return 0;
}
```

### 2.3 函数级 try 块

```cpp
#include <iostream>
#include <stdexcept>

class FunctionTryBlockDemo {
private:
    int value_;
    
public:
    // 构造函数的函数级 try 块
    FunctionTryBlockDemo(int value) try : value_(value) {
        std::cout << "Constructor body: value = " << value_ << "\n";
        if (value < 0) {
            throw std::invalid_argument("Negative value not allowed");
        }
    } catch (const std::exception& e) {
        std::cout << "Constructor catch: " << e.what() << "\n";
        throw; // 必须重新抛出或抛出新异常
    }
    
    // 析构函数的函数级 try 块
    ~FunctionTryBlockDemo() try {
        std::cout << "Destructor body: value = " << value_ << "\n";
        // 析构函数中的异常会被忽略
    } catch (const std::exception& e) {
        std::cout << "Destructor catch: " << e.what() << "\n";
        // 析构函数中的异常不应该被抛出
    }
    
    // 成员函数的函数级 try 块
    void risky_operation() try {
        std::cout << "Member function body\n";
        if (value_ == 0) {
            throw std::runtime_error("Zero value operation failed");
        }
        std::cout << "Operation succeeded with value: " << value_ << "\n";
    } catch (const std::exception& e) {
        std::cout << "Member function catch: " << e.what() << "\n";
        throw;
    }
};

// 普通函数的函数级 try 块
int divide_function(int a, int b) try {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
} catch (const std::exception& e) {
    std::cout << "Function catch: " << e.what() << "\n";
    throw;
}

void function_try_block_examples() {
    std::cout << "=== Function Try Block Examples ===\n";
    
    try {
        FunctionTryBlockDemo obj1(42);
        obj1.risky_operation();
        
        FunctionTryBlockDemo obj2(-1); // 构造函数会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Main catch: " << e.what() << "\n";
    }
    
    try {
        int result = divide_function(10, 2);
        std::cout << "Division result: " << result << "\n";
        
        result = divide_function(10, 0); // 会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Main catch: " << e.what() << "\n";
    }
}

int main() {
    function_try_block_examples();
    return 0;
}
```

## 3. 异常处理机制

### 3.1 异常传播机制

```cpp
#include <iostream>
#include <stdexcept>

void level3_function() {
    std::cout << "Level 3 function\n";
    throw std::runtime_error("Exception from level 3");
}

void level2_function() {
    std::cout << "Level 2 function\n";
    try {
        level3_function();
    } catch (const std::exception& e) {
        std::cout << "Level 2 catch: " << e.what() << "\n";
        throw; // 重新抛出
    }
}

void level1_function() {
    std::cout << "Level 1 function\n";
    level2_function();
}

void exception_propagation_mechanism() {
    std::cout << "=== Exception Propagation Mechanism ===\n";
    
    try {
        level1_function();
    } catch (const std::exception& e) {
        std::cout << "Top level catch: " << e.what() << "\n";
        
        // 获取异常信息
        std::cout << "Exception type: " << typeid(e).name() << "\n";
    }
    
    // 异常栈展开过程
    try {
        std::cout << "Creating objects for stack unwinding demo\n";
        
        class StackObject {
        private:
            std::string name_;
        public:
            StackObject(const std::string& name) : name_(name) {
                std::cout << "StackObject " << name_ << " constructed\n";
            }
            ~StackObject() {
                std::cout << "StackObject " << name_ << " destructed\n";
            }
        };
        
        StackObject obj1("Obj1");
        {
            StackObject obj2("Obj2");
            StackObject obj3("Obj3");
            throw std::runtime_error("Stack unwinding test");
        }
        
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
}

int main() {
    exception_propagation_mechanism();
    return 0;
}
```

### 3.2 异常对象生命周期

```cpp
#include <iostream>
#include <stdexcept>
#include <memory>

class ExceptionLifecycleDemo {
private:
    std::string message_;
    static int counter_;
    
public:
    ExceptionLifecycleDemo(const std::string& message) : message_(message) {
        std::cout << "Exception object " << ++counter_ << " constructed: " << message_ << "\n";
    }
    
    ExceptionLifecycleDemo(const ExceptionLifecycleDemo& other) : message_(other.message_) {
        std::cout << "Exception object " << ++counter_ << " copy constructed: " << message_ << "\n";
    }
    
    ExceptionLifecycleDemo& operator=(const ExceptionLifecycleDemo& other) {
        message_ = other.message_;
        std::cout << "Exception object assigned: " << message_ << "\n";
        return *this;
    }
    
    ~ExceptionLifecycleDemo() {
        std::cout << "Exception object " << counter_-- << " destructed: " << message_ << "\n";
    }
    
    const std::string& getMessage() const { return message_; }
};

int ExceptionLifecycleDemo::counter_ = 0;

void exception_object_lifecycle() {
    std::cout << "=== Exception Object Lifecycle ===\n";
    
    // 1. 异常对象的创建和销毁
    try {
        ExceptionLifecycleDemo exception_obj("Test exception");
        throw exception_obj; // 抛出异常对象的副本
    } catch (const ExceptionLifecycleDemo& e) {
        std::cout << "Caught exception: " << e.getMessage() << "\n";
        // 异常对象副本在此处销毁
    }
    
    // 2. 动态分配的异常对象
    try {
        auto dynamic_exception = std::make_unique<ExceptionLifecycleDemo>("Dynamic exception");
        throw *dynamic_exception; // 抛出解引用的对象
    } catch (const ExceptionLifecycleDemo& e) {
        std::cout << "Caught dynamic exception: " << e.getMessage() << "\n";
    }
    
    // 3. 异常对象的移动语义
    try {
        ExceptionLifecycleDemo move_exception("Move exception");
        throw std::move(move_exception); // 移动语义
    } catch (const ExceptionLifecycleDemo& e) {
        std::cout << "Caught moved exception: " << e.getMessage() << "\n";
    }
    
    std::cout << "End of function\n";
}

int main() {
    exception_object_lifecycle();
    return 0;
}
```

### 3.3 异常规范和 noexcept

```cpp
#include <iostream>
#include <stdexcept>

// 1. noexcept 函数
void noexcept_function() noexcept {
    std::cout << "Noexcept function called\n";
    // 即使抛出异常也会调用 std::terminate
    // throw std::runtime_error("This will terminate the program");
}

// 2. 带条件的 noexcept
void conditional_noexcept_function(bool condition) noexcept(condition) {
    std::cout << "Conditional noexcept function\n";
    if (!condition) {
        throw std::runtime_error("Condition not met");
    }
}

// 3. 异常规范检查
void exception_specification_check() {
    std::cout << "=== Exception Specification Check ===\n";
    
    // 检查函数是否为 noexcept
    std::cout << "noexcept_function is noexcept: " 
              << noexcept(noexcept_function()) << "\n";
    
    std::cout << "conditional_noexcept_function(true) is noexcept: " 
              << noexcept(conditional_noexcept_function(true)) << "\n";
    
    std::cout << "conditional_noexcept_function(false) is noexcept: " 
              << noexcept(conditional_noexcept_function(false)) << "\n";
    
    // 调用 noexcept 函数
    noexcept_function();
    
    // 调用条件 noexcept 函数
    try {
        conditional_noexcept_function(true);
        conditional_noexcept_function(false); // 会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
}

// 4. 虚函数的异常规范
class BaseExceptionClass {
public:
    virtual void virtual_function() noexcept(false) {
        std::cout << "Base virtual function\n";
    }
    
    virtual ~BaseExceptionClass() = default;
};

class DerivedExceptionClass : public BaseExceptionClass {
public:
    // 派生类可以收紧异常规范
    void virtual_function() noexcept(true) override {
        std::cout << "Derived virtual function (noexcept)\n";
    }
};

void virtual_function_exception_specification() {
    std::cout << "=== Virtual Function Exception Specification ===\n";
    
    BaseExceptionClass* base_ptr = new DerivedExceptionClass();
    base_ptr->virtual_function();
    delete base_ptr;
}

int main() {
    exception_specification_check();
    virtual_function_exception_specification();
    return 0;
}
```

## 4. 属性与 try 块

### 4.1 C++11 属性支持

从 C++11 开始，可以在 try 块前使用属性说明符：

```cpp
#include <iostream>
#include <stdexcept>

void attribute_try_blocks() {
    std::cout << "=== Attribute Try Blocks ===\n";
    
    // [[noreturn]] 属性通常不用于 try 块
    
    // [[carries_dependency]] 属性
    // [[carries_dependency]] try { /* statements */ } catch (...) { }
    
    // [[deprecated]] 属性
    [[deprecated("This try block is deprecated")]]
    try {
        throw std::runtime_error("Deprecated try block");
    } catch (const std::exception& e) {
        std::cout << "Caught in deprecated try block: " << e.what() << "\n";
    }
    
    // [[fallthrough]] 属性通常用于 switch 语句
    
    // [[maybe_unused]] 属性通常不直接用于 try 块
    
    // [[likely]] 和 [[unlikely]] 属性（C++20）
    #if __cplusplus >= 202002L
    if (true) [[likely]] {
        try {
            [[likely]] throw std::runtime_error("Likely exception");
        } catch (const std::exception& e) {
            [[likely]] std::cout << "Caught likely exception: " << e.what() << "\n";
        }
    } else [[unlikely]] {
        try {
            [[unlikely]] throw std::logic_error("Unlikely exception");
        } catch (const std::exception& e) {
            [[unlikely]] std::cout << "Caught unlikely exception: " << e.what() << "\n";
        }
    }
    #endif
}

int main() {
    attribute_try_blocks();
    return 0;
}
```

### 4.2 自定义属性

```cpp
#include <iostream>
#include <stdexcept>

// 定义自定义属性（需要编译器支持）
#if defined(__GNUC__) || defined(__clang__)
#define CUSTOM_ATTR [[gnu::always_inline]]
#define HOT_ATTR [[gnu::hot]]
#define COLD_ATTR [[gnu::cold]]
#else
#define CUSTOM_ATTR
#define HOT_ATTR
#define COLD_ATTR
#endif

CUSTOM_ATTR void custom_attribute_try_block() {
    std::cout << "=== Custom Attribute Try Block ===\n";
    
    HOT_ATTR try {
        // 热路径异常处理
        throw std::runtime_error("Hot path exception");
    } catch (const std::exception& e) {
        HOT_ATTR std::cout << "Caught hot path exception: " << e.what() << "\n";
    }
    
    COLD_ATTR try {
        // 冷路径异常处理
        throw std::logic_error("Cold path exception");
    } catch (const std::exception& e) {
        COLD_ATTR std::cout << "Caught cold path exception: " << e.what() << "\n";
    }
}

int main() {
    custom_attribute_try_block();
    return 0;
}
```

## 5. 常见应用场景

### 5.1 资源管理与异常安全

```cpp
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>

void resource_management_with_exceptions() {
    std::cout << "=== Resource Management with Exceptions ===\n";
    
    // 1. 文件操作异常安全
    try {
        std::ofstream output_file("test.txt");
        if (!output_file.is_open()) {
            throw std::runtime_error("Failed to open output file");
        }
        
        output_file << "Hello, World!\n";
        
        // 模拟可能的异常
        if (false) {
            throw std::runtime_error("Write operation failed");
        }
        
        std::cout << "File written successfully\n";
        // output_file 在作用域结束时自动关闭
        
    } catch (const std::exception& e) {
        std::cout << "File operation failed: " << e.what() << "\n";
        // 资源自动清理
    }
    
    // 2. 内存管理异常安全
    try {
        auto ptr = std::make_unique<int[]>(1000);
        
        // 初始化数组
        for (int i = 0; i < 1000; ++i) {
            ptr[i] = i;
        }
        
        // 模拟可能的异常
        if (false) {
            throw std::bad_alloc();
        }
        
        std::cout << "Array initialized successfully\n";
        // ptr 在作用域结束时自动释放内存
        
    } catch (const std::exception& e) {
        std::cout << "Memory operation failed: " << e.what() << "\n";
        // 资源自动清理
    }
    
    // 3. 多资源管理异常安全
    try {
        std::unique_ptr<std::ifstream> input_file = std::make_unique<std::ifstream>("input.txt");
        std::unique_ptr<std::ofstream> output_file = std::make_unique<std::ofstream>("output.txt");
        
        if (!input_file->is_open()) {
            throw std::runtime_error("Failed to open input file");
        }
        
        if (!output_file->is_open()) {
            throw std::runtime_error("Failed to open output file");
        }
        
        std::string line;
        while (std::getline(*input_file, line)) {
            *output_file << line << "\n";
        }
        
        std::cout << "File copy completed successfully\n";
        // 所有资源在作用域结束时自动清理
        
    } catch (const std::exception& e) {
        std::cout << "Multi-resource operation failed: " << e.what() << "\n";
        // 所有已分配的资源自动清理
    }
}

int main() {
    resource_management_with_exceptions();
    return 0;
}
```

### 5.2 网络编程异常处理

```cpp
#include <iostream>
#include <stdexcept>
#include <memory>

// 模拟网络连接类
class NetworkConnection {
private:
    std::string host_;
    int port_;
    bool connected_;
    
public:
    NetworkConnection(const std::string& host, int port) 
        : host_(host), port_(port), connected_(false) {
        std::cout << "Creating network connection to " << host_ << ":" << port_ << "\n";
    }
    
    void connect() {
        // 模拟连接过程
        if (host_ == "invalid.host") {
            throw std::runtime_error("Invalid hostname");
        }
        
        if (port_ < 0 || port_ > 65535) {
            throw std::out_of_range("Port number out of range");
        }
        
        connected_ = true;
        std::cout << "Connected to " << host_ << ":" << port_ << "\n";
    }
    
    void send_data(const std::string& data) {
        if (!connected_) {
            throw std::runtime_error("Not connected");
        }
        
        if (data.empty()) {
            throw std::invalid_argument("Empty data");
        }
        
        std::cout << "Sending data: " << data << "\n";
        
        // 模拟网络错误
        if (data == "error") {
            throw std::runtime_error("Network error during send");
        }
    }
    
    void disconnect() {
        if (connected_) {
            connected_ = false;
            std::cout << "Disconnected from " << host_ << ":" << port_ << "\n";
        }
    }
    
    ~NetworkConnection() {
        if (connected_) {
            disconnect();
        }
        std::cout << "Network connection destroyed\n";
    }
};

void network_programming_exception_handling() {
    std::cout << "=== Network Programming Exception Handling ===\n";
    
    // 1. 基本网络连接异常处理
    try {
        NetworkConnection conn("example.com", 80);
        conn.connect();
        conn.send_data("Hello, Server!");
        
    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument: " << e.what() << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "General exception: " << e.what() << "\n";
    }
    
    // 2. 多步骤网络操作异常处理
    try {
        auto connection = std::make_unique<NetworkConnection>("api.example.com", 443);
        connection->connect();
        
        // 多次发送数据
        std::vector<std::string> messages = {"Message 1", "Message 2", "error", "Message 4"};
        
        for (const auto& message : messages) {
            connection->send_data(message);
        }
        
    } catch (const std::exception& e) {
        std::cout << "Network operation failed: " << e.what() << "\n";
        // 连接对象自动清理
    }
    
    std::cout << "Network operations completed\n";
}

int main() {
    network_programming_exception_handling();
    return 0;
}
```

### 5.3 数据库操作异常处理

```cpp
#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>

// 模拟数据库连接类
class DatabaseConnection {
private:
    std::string connection_string_;
    bool connected_;
    
public:
    DatabaseConnection(const std::string& conn_str) 
        : connection_string_(conn_str), connected_(false) {
        std::cout << "Creating database connection: " << connection_string_ << "\n";
    }
    
    void connect() {
        if (connection_string_.empty()) {
            throw std::invalid_argument("Empty connection string");
        }
        
        // 模拟连接失败
        if (connection_string_ == "invalid://connection") {
            throw std::runtime_error("Failed to connect to database");
        }
        
        connected_ = true;
        std::cout << "Database connected successfully\n";
    }
    
    void execute_query(const std::string& query) {
        if (!connected_) {
            throw std::runtime_error("Not connected to database");
        }
        
        if (query.empty()) {
            throw std::invalid_argument("Empty query");
        }
        
        std::cout << "Executing query: " << query << "\n";
        
        // 模拟查询错误
        if (query.find("INVALID") != std::string::npos) {
            throw std::runtime_error("Invalid SQL syntax");
        }
        
        if (query.find("TIMEOUT") != std::string::npos) {
            throw std::runtime_error("Query timeout");
        }
    }
    
    std::vector<std::string> fetch_results() {
        if (!connected_) {
            throw std::runtime_error("Not connected to database");
        }
        
        // 模拟获取结果
        std::vector<std::string> results = {"Row 1", "Row 2", "Row 3"};
        std::cout << "Fetched " << results.size() << " rows\n";
        return results;
    }
    
    void disconnect() {
        if (connected_) {
            connected_ = false;
            std::cout << "Database disconnected\n";
        }
    }
    
    ~DatabaseConnection() {
        if (connected_) {
            disconnect();
        }
        std::cout << "Database connection destroyed\n";
    }
};

void database_exception_handling() {
    std::cout << "=== Database Exception Handling ===\n";
    
    // 1. 基本数据库操作异常处理
    try {
        DatabaseConnection db("postgresql://localhost:5432/mydb");
        db.connect();
        db.execute_query("SELECT * FROM users");
        auto results = db.fetch_results();
        
        for (const auto& row : results) {
            std::cout << "Result: " << row << "\n";
        }
        
    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "General exception: " << e.what() << "\n";
    }
    
    // 2. 事务处理异常处理
    try {
        auto db_connection = std::make_unique<DatabaseConnection>("mysql://localhost:3306/mydb");
        db_connection->connect();
        
        // 开始事务
        db_connection->execute_query("BEGIN TRANSACTION");
        
        try {
            // 执行多个操作
            db_connection->execute_query("INSERT INTO users (name) VALUES ('Alice')");
            db_connection->execute_query("UPDATE users SET age = 25 WHERE name = 'Alice'");
            db_connection->execute_query("INVALID SQL STATEMENT"); // 这会抛出异常
            
            // 提交事务
            db_connection->execute_query("COMMIT");
            
        } catch (const std::exception& e) {
            std::cout << "Transaction failed: " << e.what() << "\n";
            // 回滚事务
            db_connection->execute_query("ROLLBACK");
            throw; // 重新抛出异常
        }
        
    } catch (const std::exception& e) {
        std::cout << "Database transaction error: " << e.what() << "\n";
        // 数据库连接自动清理
    }
    
    std::cout << "Database operations completed\n";
}

int main() {
    database_exception_handling();
    return 0;
}
```

## 6. 与其他语言特性的交互

### 6.1 与模板的交互

```cpp
#include <iostream>
#include <stdexcept>
#include <type_traits>

template<typename T>
class TemplateExceptionDemo {
private:
    T value_;
    
public:
    TemplateExceptionDemo(T value) : value_(value) {
        std::cout << "TemplateExceptionDemo constructed with value: " << value_ << "\n";
    }
    
    void validate_and_process() {
        try {
            if constexpr (std::is_arithmetic_v<T>) {
                if (value_ < 0) {
                    throw std::invalid_argument("Negative arithmetic value not allowed");
                }
                std::cout << "Processed arithmetic value: " << value_ << "\n";
            } else if constexpr (std::is_same_v<T, std::string>) {
                if (value_.empty()) {
                    throw std::invalid_argument("Empty string not allowed");
                }
                std::cout << "Processed string value: " << value_ << "\n";
            } else {
                throw std::runtime_error("Unsupported type");
            }
        } catch (const std::exception& e) {
            std::cout << "Template catch: " << e.what() << "\n";
            throw;
        }
    }
};

// 模板函数异常处理
template<typename T>
T safe_divide(T a, T b) {
    try {
        if (b == T{0}) {
            throw std::domain_error("Division by zero");
        }
        return a / b;
    } catch (const std::exception& e) {
        std::cout << "Template function catch: " << e.what() << "\n";
        throw;
    }
}

void template_exception_interaction() {
    std::cout << "=== Template Exception Interaction ===\n";
    
    try {
        // 整数模板实例化
        TemplateExceptionDemo<int> int_demo(42);
        int_demo.validate_and_process();
        
        TemplateExceptionDemo<int> negative_int_demo(-1);
        negative_int_demo.validate_and_process(); // 会抛出异常
        
    } catch (const std::exception& e) {
        std::cout << "Main catch for int template: " << e.what() << "\n";
    }
    
    try {
        // 字符串模板实例化
        TemplateExceptionDemo<std::string> string_demo("Hello");
        string_demo.validate_and_process();
        
        TemplateExceptionDemo<std::string> empty_string_demo("");
        empty_string_demo.validate_and_process(); // 会抛出异常
        
    } catch (const std::exception& e) {
        std::cout << "Main catch for string template: " << e.what() << "\n";
    }
    
    try {
        // 模板函数调用
        double result = safe_divide(10.0, 2.0);
        std::cout << "Division result: " << result << "\n";
        
        result = safe_divide(10.0, 0.0); // 会抛出异常
        
    } catch (const std::exception& e) {
        std::cout << "Main catch for template function: " << e.what() << "\n";
    }
}

int main() {
    template_exception_interaction();
    return 0;
}
```

### 6.2 与 Lambda 表达式的交互

```cpp
#include <iostream>
#include <functional>
#include <stdexcept>

void lambda_exception_interaction() {
    std::cout << "=== Lambda Exception Interaction ===\n";
    
    // 1. Lambda 内部的异常处理
    auto risky_lambda = [](int x) -> int {
        try {
            if (x < 0) {
                throw std::invalid_argument("Negative value not allowed");
            }
            if (x == 0) {
                throw std::runtime_error("Zero value not allowed");
            }
            return 100 / x;
        } catch (const std::exception& e) {
            std::cout << "Lambda internal catch: " << e.what() << "\n";
            throw;
        }
    };
    
    try {
        int result = risky_lambda(5);
        std::cout << "Lambda result: " << result << "\n";
        
        result = risky_lambda(-1); // 会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Main catch for lambda: " << e.what() << "\n";
    }
    
    // 2. 带捕获的 Lambda 异常处理
    int captured_value = 42;
    auto capturing_lambda = [captured_value](int x) {
        try {
            if (x > captured_value) {
                throw std::out_of_range("Value exceeds captured limit");
            }
            std::cout << "Captured value: " << captured_value << ", input: " << x << "\n";
        } catch (const std::exception& e) {
            std::cout << "Capturing lambda catch: " << e.what() << "\n";
            throw;
        }
    };
    
    try {
        capturing_lambda(20);
        capturing_lambda(50); // 会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Main catch for capturing lambda: " << e.what() << "\n";
    }
    
    // 3. 异常安全的 Lambda 使用
    try {
        std::vector<std::function<void()>> operations;
        
        // 添加可能抛出异常的操作
        operations.push_back([]() {
            std::cout << "Operation 1\n";
        });
        
        operations.push_back([]() {
            throw std::runtime_error("Operation 2 failed");
        });
        
        operations.push_back([]() {
            std::cout << "Operation 3\n"; // 这不会被执行
        });
        
        // 执行所有操作
        for (const auto& op : operations) {
            op();
        }
        
    } catch (const std::exception& e) {
        std::cout << "Vector operations catch: " << e.what() << "\n";
    }
}

int main() {
    lambda_exception_interaction();
    return 0;
}
```

### 6.3 与并发的交互

```cpp
#include <iostream>
#include <thread>
#include <future>
#include <stdexcept>
#include <vector>

void concurrent_exception_interaction() {
    std::cout << "=== Concurrent Exception Interaction ===\n";
    
    // 1. 线程中的异常处理
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([i]() {
            try {
                std::cout << "Thread " << i << " started\n";
                
                if (i == 1) {
                    throw std::runtime_error("Exception in thread " + std::to_string(i));
                }
                
                std::cout << "Thread " << i << " completed successfully\n";
                
            } catch (const std::exception& e) {
                std::cout << "Thread " << i << " caught exception: " << e.what() << "\n";
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // 2. 异步操作中的异常处理
    try {
        auto future1 = std::async(std::launch::async, []() {
            std::cout << "Async task 1 started\n";
            return 42;
        });
        
        auto future2 = std::async(std::launch::async, []() {
            std::cout << "Async task 2 started\n";
            throw std::runtime_error("Exception in async task 2");
            return 24;
        });
        
        // 获取结果
        int result1 = future1.get();
        std::cout << "Async task 1 result: " << result1 << "\n";
        
        int result2 = future2.get(); // 这会重新抛出异常
        std::cout << "Async task 2 result: " << result2 << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "Async operations catch: " << e.what() << "\n";
    }
    
    // 3. 多线程异常聚合处理
    try {
        std::vector<std::future<int>> futures;
        
        for (int i = 0; i < 5; ++i) {
            futures.push_back(std::async(std::launch::async, [i]() {
                if (i % 2 == 0) {
                    return i * i;
                } else {
                    throw std::runtime_error("Odd number error: " + std::to_string(i));
                }
            }));
        }
        
        std::vector<int> results;
        std::vector<std::exception_ptr> exceptions;
        
        for (auto& future : futures) {
            try {
                results.push_back(future.get());
            } catch (...) {
                exceptions.push_back(std::current_exception());
            }
        }
        
        std::cout << "Successful results: ";
        for (int result : results) {
            std::cout << result << " ";
        }
        std::cout << "\n";
        
        std::cout << "Number of exceptions: " << exceptions.size() << "\n";
        
        // 重新抛出第一个异常（如果有的话）
        if (!exceptions.empty()) {
            std::rethrow_exception(exceptions.front());
        }
        
    } catch (const std::exception& e) {
        std::cout << "Multi-thread catch: " << e.what() << "\n";
    }
}

int main() {
    concurrent_exception_interaction();
    return 0;
}
```

### 6.4 与智能指针的交互

```cpp
#include <iostream>
#include <memory>
#include <stdexcept>

class SmartPointerExceptionDemo {
private:
    std::string name_;
    static int counter_;
    
public:
    SmartPointerExceptionDemo(const std::string& name) : name_(name) {
        std::cout << "SmartPointerExceptionDemo " << ++counter_ << " constructed: " << name_ << "\n";
    }
    
    ~SmartPointerExceptionDemo() {
        std::cout << "SmartPointerExceptionDemo " << counter_-- << " destructed: " << name_ << "\n";
    }
    
    void risky_operation(bool should_throw) {
        if (should_throw) {
            throw std::runtime_error("Risky operation failed in " + name_);
        }
        std::cout << "Risky operation succeeded in " << name_ << "\n";
    }
};

int SmartPointerExceptionDemo::counter_ = 0;

void smart_pointer_exception_interaction() {
    std::cout << "=== Smart Pointer Exception Interaction ===\n";
    
    // 1. unique_ptr 与异常处理
    try {
        auto unique_obj = std::make_unique<SmartPointerExceptionDemo>("Unique Object");
        unique_obj->risky_operation(false);
        unique_obj->risky_operation(true); // 会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Unique ptr catch: " << e.what() << "\n";
        // unique_obj 自动销毁
    }
    
    // 2. shared_ptr 与异常处理
    try {
        auto shared_obj = std::make_shared<SmartPointerExceptionDemo>("Shared Object");
        std::cout << "Shared ptr use count: " << shared_obj.use_count() << "\n";
        
        auto another_shared = shared_obj;
        std::cout << "Shared ptr use count after copy: " << shared_obj.use_count() << "\n";
        
        shared_obj->risky_operation(true); // 会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Shared ptr catch: " << e.what() << "\n";
        // shared_obj 和 another_shared 自动销毁
    }
    
    // 3. 异常安全的智能指针操作
    try {
        std::vector<std::unique_ptr<SmartPointerExceptionDemo>> objects;
        
        // 创建多个对象
        for (int i = 0; i < 5; ++i) {
            objects.push_back(std::make_unique<SmartPointerExceptionDemo>("Object " + std::to_string(i)));
        }
        
        std::cout << "Created " << objects.size() << " objects\n";
        
        // 执行可能抛出异常的操作
        for (size_t i = 0; i < objects.size(); ++i) {
            objects[i]->risky_operation(i == 2); // 第3个对象会抛出异常
        }
        
    } catch (const std::exception& e) {
        std::cout << "Vector smart pointers catch: " << e.what() << "\n";
        // 所有 unique_ptr 自动销毁
    }
    
    std::cout << "Smart pointer operations completed\n";
}

int main() {
    smart_pointer_exception_interaction();
    return 0;
}
```

## 7. 最佳实践与代码规范

### 7.1 合理使用 try 块

```cpp
#include <iostream>
#include <stdexcept>
#include <fstream>

// 好的做法：合理使用 try 块
void good_try_block_usage() {
    std::cout << "=== Good Try Block Usage ===\n";
    
    // 1. 明确的异常处理边界
    try {
        // 资源获取和使用
        std::ifstream file("data.txt");
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
        
        std::string line;
        while (std::getline(file, line)) {
            // 处理每一行
            if (line.empty()) {
                continue;
            }
            
            // 可能抛出异常的处理
            if (line == "ERROR") {
                throw std::runtime_error("Error line encountered");
            }
            
            std::cout << "Processed line: " << line << "\n";
        }
        
    } catch (const std::ifstream::failure& e) {
        std::cout << "File I/O error: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "General error: " << e.what() << "\n";
    }
    
    // 2. 分层异常处理
    try {
        // 高层业务逻辑
        try {
            // 底层操作
            int result = 42 / 0; // 这会导致未定义行为，应该避免
        } catch (const std::exception& e) {
            std::cout << "Low-level catch: " << e.what() << "\n";
            throw std::runtime_error("Business logic failed: " + std::string(e.what()));
        }
    } catch (const std::exception& e) {
        std::cout << "High-level catch: " << e.what() << "\n";
    }
}

int main() {
    good_try_block_usage();
    return 0;
}
```

### 7.2 避免的做法

```cpp
#include <iostream>
#include <stdexcept>

// 避免的做法：不合理的 try 块使用
void avoid_unnecessary_try_blocks() {
    std::cout << "=== Avoid Unnecessary Try Blocks ===\n";
    
    // 避免：过度使用 try 块
    /*
    try {
        int x = 42; // 简单赋值不需要异常处理
        std::cout << "x = " << x << "\n";
    } catch (...) {
        // 不必要的异常处理
    }
    */
    
    // 更好的做法：只在必要时使用异常处理
    int x = 42;
    std::cout << "x = " << x << "\n";
    
    // 避免：忽略异常
    /*
    try {
        throw std::runtime_error("Important error");
    } catch (...) {
        // 忽略所有异常 - 危险做法
    }
    */
    
    // 更好的做法：适当的异常处理
    try {
        throw std::runtime_error("Important error");
    } catch (const std::exception& e) {
        std::cout << "Handled important error: " << e.what() << "\n";
        // 记录日志或采取适当措施
    }
    
    // 避免：过于宽泛的异常捕获
    /*
    try {
        // 一些操作
    } catch (...) {
        // 捕获所有异常但不处理 - 不明确
    }
    */
    
    // 更好的做法：具体的异常处理
    try {
        // 一些操作
        throw std::invalid_argument("Invalid argument");
    } catch (const std::invalid_argument& e) {
        std::cout << "Handled invalid argument: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Handled runtime error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Handled general exception: " << e.what() << "\n";
    }
}

int main() {
    avoid_unnecessary_try_blocks();
    return 0;
}
```

### 7.3 代码风格建议

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>

class ExceptionHandlingStyle {
public:
    void proper_style_examples() {
        std::cout << "=== Proper Style Examples ===\n";
        
        // 1. 一致的异常处理风格
        try {
            // 业务逻辑
            perform_business_operation();
        } catch (const BusinessLogicError& e) {
            // 业务逻辑异常处理
            handle_business_error(e);
        } catch (const std::system_error& e) {
            // 系统级异常处理
            handle_system_error(e);
        } catch (const std::exception& e) {
            // 通用异常处理
            handle_general_error(e);
        }
        
        // 2. 清晰的异常处理注释
        try {
            // 数据库操作
            execute_database_query();
        } catch (const DatabaseConnectionError& e) {
            // 数据库连接失败 - 记录错误并重试
            log_error("Database connection failed", e);
            retry_database_operation();
        } catch (const DatabaseQueryError& e) {
            // 查询执行失败 - 回滚事务并通知用户
            rollback_transaction();
            notify_user("Query execution failed");
        }
        
        // 3. 异常安全的资源管理
        try {
            // 使用 RAII 管理资源
            auto resource_manager = std::make_unique<ResourceManager>();
            resource_manager->acquire_resources();
            
            // 执行操作
            resource_manager->perform_operation();
            
        } catch (const ResourceAcquisitionError& e) {
            // 资源获取失败的特殊处理
            handle_resource_error(e);
        } catch (const std::exception& e) {
            // 通用异常处理
            handle_operation_error(e);
        }
        // 资源自动释放
    }
    
private:
    // 自定义异常类
    class BusinessLogicError : public std::runtime_error {
    public:
        BusinessLogicError(const std::string& msg) : std::runtime_error(msg) {}
    };
    
    class DatabaseConnectionError : public std::runtime_error {
    public:
        DatabaseConnectionError(const std::string& msg) : std::runtime_error(msg) {}
    };
    
    class DatabaseQueryError : public std::runtime_error {
    public:
        DatabaseQueryError(const std::string& msg) : std::runtime_error(msg) {}
    };
    
    class ResourceAcquisitionError : public std::runtime_error {
    public:
        ResourceAcquisitionError(const std::string& msg) : std::runtime_error(msg) {}
    };
    
    void perform_business_operation() {
        // 模拟业务操作
        throw BusinessLogicError("Business logic failed");
    }
    
    void handle_business_error(const BusinessLogicError& e) {
        std::cout << "Business error handled: " << e.what() << "\n";
    }
    
    void handle_system_error(const std::system_error& e) {
        std::cout << "System error handled: " << e.what() << "\n";
    }
    
    void handle_general_error(const std::exception& e) {
        std::cout << "General error handled: " << e.what() << "\n";
    }
    
    void execute_database_query() {
        // 模拟数据库操作
        throw DatabaseConnectionError("Database connection failed");
    }
    
    void log_error(const std::string& msg, const std::exception& e) {
        std::cout << "Error logged: " << msg << " - " << e.what() << "\n";
    }
    
    void retry_database_operation() {
        std::cout << "Retrying database operation\n";
    }
    
    void rollback_transaction() {
        std::cout << "Transaction rolled back\n";
    }
    
    void notify_user(const std::string& msg) {
        std::cout << "User notified: " << msg << "\n";
    }
    
    class ResourceManager {
    public:
        void acquire_resources() {
            std::cout << "Resources acquired\n";
        }
        
        void perform_operation() {
            std::cout << "Operation performed\n";
        }
    };
    
    void handle_resource_error(const ResourceAcquisitionError& e) {
        std::cout << "Resource error handled: " << e.what() << "\n";
    }
    
    void handle_operation_error(const std::exception& e) {
        std::cout << "Operation error handled: " << e.what() << "\n";
    }
};

int main() {
    ExceptionHandlingStyle style;
    style.proper_style_examples();
    return 0;
}
```

### 7.4 现代 C++ 替代方案

```cpp
#include <iostream>
#include <optional>
#include <variant>
#include <expected> // C++23 (需要支持)

// 使用现代 C++ 特性替代传统的异常处理模式
void modern_alternatives() {
    std::cout << "=== Modern C++ Alternatives ===\n";
    
    // 1. 使用 std::optional 替代可能失败的操作
    auto safe_divide = [](int a, int b) -> std::optional<int> {
        if (b == 0) {
            return std::nullopt;
        }
        return a / b;
    };
    
    auto result = safe_divide(10, 2);
    if (result) {
        std::cout << "Division result: " << *result << "\n";
    } else {
        std::cout << "Division by zero\n";
    }
    
    result = safe_divide(10, 0);
    if (result) {
        std::cout << "Division result: " << *result << "\n";
    } else {
        std::cout << "Division by zero\n";
    }
    
    // 2. 使用 std::variant 替代异常处理
    enum class ParseResult { SUCCESS, INVALID_FORMAT, OUT_OF_RANGE };
    
    auto parse_integer = [](const std::string& str) -> std::variant<int, ParseResult> {
        if (str.empty()) {
            return ParseResult::INVALID_FORMAT;
        }
        
        try {
            int value = std::stoi(str);
            if (value < 0 || value > 1000) {
                return ParseResult::OUT_OF_RANGE;
            }
            return value;
        } catch (const std::exception&) {
            return ParseResult::INVALID_FORMAT;
        }
    };
    
    auto parse_result = parse_integer("42");
    if (std::holds_alternative<int>(parse_result)) {
        std::cout << "Parsed integer: " << std::get<int>(parse_result) << "\n";
    } else {
        auto error = std::get<ParseResult>(parse_result);
        switch (error) {
            case ParseResult::INVALID_FORMAT:
                std::cout << "Invalid format\n";
                break;
            case ParseResult::OUT_OF_RANGE:
                std::cout << "Out of range\n";
                break;
            case ParseResult::SUCCESS:
                break;
        }
    }
    
    // 3. 使用预期值模式（C++23 std::expected 的简化实现）
    #if __cplusplus >= 202302L
    // 当 std::expected 可用时的示例
    /*
    auto file_operation = [](const std::string& filename) -> std::expected<std::string, std::string> {
        if (filename.empty()) {
            return std::unexpected<std::string>("Empty filename");
        }
        
        // 模拟文件读取
        return std::string("File content");
    };
    
    auto file_result = file_operation("test.txt");
    if (file_result) {
        std::cout << "File content: " << *file_result << "\n";
    } else {
        std::cout << "File operation failed: " << file_result.error() << "\n";
    }
    */
    #endif
    
    // 4. 组合使用现代特性
    class ModernResourceManagement {
    public:
        static std::optional<std::unique_ptr<int>> acquire_resource(bool success) {
            if (!success) {
                return std::nullopt;
            }
            return std::make_unique<int>(42);
        }
        
        static std::variant<int, std::string> process_resource(const std::unique_ptr<int>& resource) {
            if (!resource) {
                return std::string("Invalid resource");
            }
            
            if (*resource < 0) {
                return std::string("Negative resource value");
            }
            
            return *resource * 2;
        }
    };
    
    auto resource = ModernResourceManagement::acquire_resource(true);
    if (resource) {
        auto process_result = ModernResourceManagement::process_resource(*resource);
        if (std::holds_alternative<int>(process_result)) {
            std::cout << "Processed result: " << std::get<int>(process_result) << "\n";
        } else {
            std::cout << "Processing failed: " << std::get<std::string>(process_result) << "\n";
        }
    } else {
        std::cout << "Resource acquisition failed\n";
    }
}

int main() {
    modern_alternatives();
    return 0;
}
```

## 8. 标准参考

### 8.1 ISO C++ 标准条款引用

**C++98/C++03 标准 (ISO/IEC 14882:1998/2003)**：

- §15: 异常处理
- §15.1: 抛出异常
- §15.2: 捕获异常处理程序
- §15.3: 异常处理
- §15.4: 异常规范

**C++11 标准 (ISO/IEC 14882:2011)**：

- §15: 异常处理
- §15.1: 抛出异常
- §15.2: 捕获异常处理程序
- §15.3: 异常处理
- §15.4: 动态异常规范
- §15.5: noexcept 规范

**C++14 标准 (ISO/IEC 14882:2014)**：

- §15: 异常处理语义的澄清
- §15.1: 抛出异常的优化
- §15.2: 异常处理程序的改进

**C++17 标准 (ISO/IEC 14882:2017)**：

- §18: 异常处理
- §18.1: 抛出异常
- §18.2: 捕获异常处理程序
- §18.3: 异常处理

**C++20 标准 (ISO/IEC 14882:2020)**：

- §14: 异常处理的语义
- §14.1: 抛出异常
- §14.2: 异常处理程序
- §14.3: 异常处理

**C++23 标准 (ISO/IEC 14882:2023)**：

- §13: 异常处理的更新
- §13.1: 抛出异常
- §13.2: 异常处理程序
- §13.3: 异常处理

### 8.2 相关标准文档

- ISO/IEC 14882: Programming Languages - C++
- N3337: C++11 Standard Draft
- N4140: C++14 Standard Draft
- N4659: C++17 Standard Draft
- N4860: C++20 Standard Draft
- N4950: C++23 Standard Draft

## 9. 总结

### 9.1 C++ try 块的特点

C++ try 块提供了一种结构化的异常处理机制，是 C++ 错误处理的核心特性。

**核心特性**：

- 结构化处理: 提供清晰的异常处理结构
- 资源安全: 保证异常发生时资源的正确清理
- 类型安全: 支持基于类型的异常处理
- 性能优化: 现代编译器对异常处理进行了优化
- 标准兼容: 遵循 ISO C++ 标准规范

### 9.2 异常处理机制比较

| 机制 | 语法示例 | 特点 | 适用场景 |
|------|----------|------|----------|
| try-catch | `try { } catch(...) { }` | 结构化处理 | 复杂错误处理 |
| noexcept | `void func() noexcept` | 性能优化 | 不抛异常函数 |
| std::optional | `std::optional<T>` | 值语义 | 可能失败操作 |
| std::variant | `std::variant<T1, T2>` | 类型安全 | 多种返回类型 |

### 9.3 实际应用场景

**企业级应用异常处理框架**：

```cpp
#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>

// 企业级异常处理框架
class EnterpriseExceptionHandler {
public:
    // 业务逻辑异常基类
    class BusinessLogicException : public std::runtime_error {
    public:
        BusinessLogicException(const std::string& msg) : std::runtime_error(msg) {}
    };
    
    // 数据访问异常
    class DataAccessException : public BusinessLogicException {
    public:
        DataAccessException(const std::string& msg) : BusinessLogicException(msg) {}
    };
    
    // 网络异常
    class NetworkException : public BusinessLogicException {
    public:
        NetworkException(const std::string& msg) : BusinessLogicException(msg) {}
    };
    
    // 安全异常
    class SecurityException : public BusinessLogicException {
    public:
        SecurityException(const std::string& msg) : BusinessLogicException(msg) {}
    };
    
    static void enterprise_application_example() {
        std::cout << "=== Enterprise Application Example ===\n";
        
        try {
            // 模拟企业级应用流程
            authenticate_user();
            validate_permissions();
            process_business_logic();
            update_database();
            send_notifications();
            
        } catch (const SecurityException& e) {
            std::cout << "Security error: " << e.what() << "\n";
            log_security_event(e.what());
            notify_security_team();
        } catch (const DataAccessException& e) {
            std::cout << "Data access error: " << e.what() << "\n";
            log_database_error(e.what());
            rollback_transaction();
        } catch (const NetworkException& e) {
            std::cout << "Network error: " << e.what() << "\n";
            log_network_error(e.what());
            retry_operation();
        } catch (const BusinessLogicException& e) {
            std::cout << "Business logic error: " << e.what() << "\n";
            log_business_error(e.what());
            notify_business_team();
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << "\n";
            log_system_error(e.what());
            notify_system_admin();
        }
    }
    
private:
    static void authenticate_user() {
        std::cout << "Authenticating user...\n";
        // 模拟认证失败
        throw SecurityException("Invalid credentials");
    }
    
    static void validate_permissions() {
        std::cout << "Validating permissions...\n";
    }
    
    static void process_business_logic() {
        std::cout << "Processing business logic...\n";
    }
    
    static void update_database() {
        std::cout << "Updating database...\n";
        // 模拟数据库错误
        throw DataAccessException("Database connection failed");
    }
    
    static void send_notifications() {
        std::cout << "Sending notifications...\n";
        // 模拟网络错误
        throw NetworkException("Notification service unavailable");
    }
    
    static void log_security_event(const std::string& msg) {
        std::cout << "Security event logged: " << msg << "\n";
    }
    
    static void notify_security_team() {
        std::cout << "Security team notified\n";
    }
    
    static void log_database_error(const std::string& msg) {
        std::cout << "Database error logged: " << msg << "\n";
    }
    
    static void rollback_transaction() {
        std::cout << "Database transaction rolled back\n";
    }
    
    static void log_network_error(const std::string& msg) {
        std::cout << "Network error logged: " << msg << "\n";
    }
    
    static void retry_operation() {
        std::cout << "Operation will be retried\n";
    }
    
    static void log_business_error(const std::string& msg) {
        std::cout << "Business error logged: " << msg << "\n";
    }
    
    static void notify_business_team() {
        std::cout << "Business team notified\n";
    }
    
    static void log_system_error(const std::string& msg) {
        std::cout << "System error logged: " << msg << "\n";
    }
    
    static void notify_system_admin() {
        std::cout << "System administrator notified\n";
    }
};

void enterprise_application_framework() {
    EnterpriseExceptionHandler::enterprise_application_example();
}

int main() {
    enterprise_application_framework();
    return 0;
}
```

**现代异常安全系统**：

```cpp
#include <iostream>
#include <stdexcept>
#include <memory>
#include <optional>
#include <variant>

class ModernExceptionSafetySystem {
public:
    static void modern_exception_safety() {
        std::cout << "=== Modern Exception Safety System ===\n";
        
        // 1. RAII 资源管理
        try {
            auto resource_manager = std::make_unique<ResourceManager>();
            resource_manager->acquire_resources();
            
            // 执行可能抛出异常的操作
            resource_manager->perform_risky_operation();
            
        } catch (const ResourceException& e) {
            std::cout << "Resource exception handled: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "General exception handled: " << e.what() << "\n";
        }
        // 资源自动清理
        
        // 2. 异常安全的容器操作
        try {
            std::vector<std::unique_ptr<DataProcessor>> processors;
            
            // 创建处理器
            for (int i = 0; i < 5; ++i) {
                processors.push_back(std::make_unique<DataProcessor>(i));
            }
            
            std::cout << "Created " << processors.size() << " processors\n";
            
            // 执行处理（可能抛出异常）
            for (auto& processor : processors) {
                processor->process_data();
            }
            
        } catch (const DataProcessingException& e) {
            std::cout << "Data processing exception: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "General processing exception: " << e.what() << "\n";
        }
        // 所有处理器自动清理
        
        // 3. 现代错误处理模式
        auto result = perform_operation_with_modern_error_handling();
        handle_modern_result(result);
    }
    
private:
    // 自定义异常类
    class ResourceException : public std::runtime_error {
    public:
        ResourceException(const std::string& msg) : std::runtime_error(msg) {}
    };
    
    class DataProcessingException : public std::runtime_error {
    public:
        DataProcessingException(const std::string& msg) : std::runtime_error(msg) {}
    };
    
    // 资源管理器
    class ResourceManager {
    private:
        bool resources_acquired_ = false;
        
    public:
        void acquire_resources() {
            std::cout << "Acquiring resources...\n";
            // 模拟资源获取
            resources_acquired_ = true;
        }
        
        void perform_risky_operation() {
            std::cout << "Performing risky operation...\n";
            if (true) { // 模拟条件
                throw ResourceException("Resource operation failed");
            }
        }
        
        ~ResourceManager() {
            if (resources_acquired_) {
                std::cout << "Releasing resources...\n";
            }
        }
    };
    
    // 数据处理器
    class DataProcessor {
    private:
        int id_;
        
    public:
        DataProcessor(int id) : id_(id) {
            std::cout << "DataProcessor " << id_ << " created\n";
        }
        
        void process_data() {
            std::cout << "Processing data in processor " << id_ << "\n";
            if (id_ == 2) { // 模拟第三个处理器失败
                throw DataProcessingException("Processor " + std::to_string(id_) + " failed");
            }
        }
        
        ~DataProcessor() {
            std::cout << "DataProcessor " << id_ << " destroyed\n";
        }
    };
    
    // 现代错误处理结果类型
    using OperationResult = std::variant<int, std::string>;
    
    static OperationResult perform_operation_with_modern_error_handling() {
        try {
            // 模拟可能失败的操作
            int value = 42;
            if (value < 0) {
                return std::string("Negative value not allowed");
            }
            return value * 2;
        } catch (const std::exception& e) {
            return std::string("Exception occurred: ") + e.what();
        }
    }
    
    static void handle_modern_result(const OperationResult& result) {
        if (std::holds_alternative<int>(result)) {
            std::cout << "Operation succeeded with result: " << std::get<int>(result) << "\n";
        } else {
            std::cout << "Operation failed with error: " << std::get<std::string>(result) << "\n";
        }
    }
};

void modern_exception_safety_application() {
    ModernExceptionSafetySystem::modern_exception_safety();
}

int main() {
    modern_exception_safety_application();
    return 0;
}
```

### 9.4 最佳实践总结

- 明确边界: 在适当的层次使用 try 块
- 具体处理: 捕获具体的异常类型而非通用类型
- 资源安全: 利用 RAII 确保资源正确清理
- 性能考虑: 合理使用 noexcept 和现代替代方案
- 文档注释: 为异常处理逻辑添加清晰注释

### 9.5 学习建议

- 理解基础: 深入掌握异常处理的基本概念和机制
- 实践应用: 在实际项目中练习异常处理技术
- 关注标准: 跟踪 C++ 标准对异常处理的新特性
- 代码审查: 建立异常处理的代码审查机制
- 性能意识: 理解异常处理对性能的影响

try 块作为 C++ 异常处理的核心机制，虽然提供了强大的错误处理能力，但也需要谨慎使用。掌握其特性和最佳实践，能够帮助开发者编写更健壮、更可靠的 C++ 程序。
