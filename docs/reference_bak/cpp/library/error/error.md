# C++诊断库（Diagnostics Library）

来源：cppreference.com

## 诊断库概述

诊断库提供了C++程序中异常处理、错误报告和调试支持的相关类和函数。该库包含异常处理机制、系统错误报告、断言检查以及调试支持工具。

## 主要组件

### 一、异常处理（Exception Handling）

`<exception>`头文件提供了与C++程序异常处理相关的类和函数。

| 组件 | 说明 |
|------|------|
| `exception` | 标准库组件抛出的异常的基类（类） |
| `exception_ptr`(C++11) | 用于处理异常对象的共享指针类型（typedef） |
| `current_exception`(C++11) | 在`std::exception_ptr`中捕获当前异常（函数） |
| `rethrow_exception`(C++11) | 从`std::exception_ptr`抛出异常（函数） |
| `nested_exception`(C++11) | 用于捕获和存储当前异常的混合类型（类） |
| `terminate` | 异常处理失败时调用的函数（函数） |
| `bad_exception` | 当`std::current_exception`无法复制异常对象时抛出的异常（类） |

### 二、异常类别（Exception Categories）

`<stdexcept>`头文件预定义了几个便利类来报告特定的错误条件。

#### 逻辑错误（Logic Errors）
| 组件 | 说明 |
|------|------|
| `logic_error` | 指示违反逻辑前提条件或类不变量的异常类（类） |
| `invalid_argument` | 报告无效参数的异常类（类） |
| `domain_error` | 报告域错误的异常类（类） |
| `length_error` | 报告尝试超出最大允许大小的异常类（类） |
| `out_of_range` | 报告超出预期范围的参数的异常类（类） |

#### 运行时错误（Runtime Errors）
| 组件 | 说明 |
|------|------|
| `runtime_error` | 指示只能在运行时检测到的条件的异常类（类） |
| `range_error` | 报告内部计算中范围错误的异常类（类） |
| `overflow_error` | 报告算术溢出的异常类（类） |
| `underflow_error` | 报告算术下溢的异常类（类） |

### 三、错误编号（Error Numbers）

| 组件 | 说明 |
|------|------|
| `errno` | 展开为POSIX兼容的线程局部错误号变量的宏（宏变量） |
| `E2BIG`, `EACCES`, ..., `EXDEV` | 标准POSIX兼容错误条件的宏常量（宏常量） |

### 四、系统错误（System Error）（C++11起）

`<system_error>`头文件定义了用于报告源自操作系统的错误条件的类型和函数。

| 组件 | 说明 |
|------|------|
| `error_category`(C++11) | 错误类别的基类（类） |
| `generic_category`(C++11) | 标识通用错误类别（函数） |
| `system_category`(C++11) | 标识操作系统错误类别（函数） |
| `error_condition`(C++11) | 保存可移植错误代码（类） |
| `errc`(C++11) | 列出所有标准`<cerrno>`宏常量的`std::error_condition`枚举（类） |
| `error_code`(C++11) | 保存平台相关错误代码（类） |
| `system_error`(C++11) | 用于报告具有`error_code`的条件的异常类（类） |

### 五、断言（Assertions）

| 组件 | 说明 |
|------|------|
| `assert` | 如果用户指定的条件不为真则中止程序。可能在发布构建中被禁用。（函数宏） |

### 六、堆栈跟踪（Stacktrace）（C++23起）

| 组件 | 说明 |
|------|------|
| `stacktrace_entry`(C++23) | 堆栈跟踪中评估的表示（类） |
| `basic_stacktrace`(C++23) | 由堆栈跟踪条目组成的调用序列的近似表示（类模板） |

### 七、调试支持（Debugging Support）（C++26起）

| 组件 | 说明 |
|------|------|
| `breakpoint`(C++26) | 调用时暂停运行程序（函数） |
| `breakpoint_if_debugging`(C++26) | 如果`std::is_debugger_present`返回true则调用`std::breakpoint`（函数） |
| `is_debugger_present`(C++26) | 检查程序是否在调试器控制下运行（函数） |

---

## 扩展知识详解

### 一、现代异常处理机制

#### 1. 基础异常处理
```cpp
#include <exception>
#include <stdexcept>
#include <iostream>

void basic_exception_handling() {
    try {
        // 可能抛出异常的代码
        throw std::runtime_error("Something went wrong!");
    } catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Standard exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception caught!" << std::endl;
    }
}
```

#### 2. 自定义异常类
```cpp
#include <stdexcept>
#include <string>

class CustomException : public std::exception {
private:
    std::string message_;
    
public:
    explicit CustomException(const std::string& msg) : message_(msg) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class DatabaseException : public std::runtime_error {
private:
    int error_code_;
    
public:
    DatabaseException(const std::string& msg, int code) 
        : std::runtime_error(msg), error_code_(code) {}
    
    int code() const { return error_code_; }
};

void custom_exceptions() {
    try {
        throw DatabaseException("Database connection failed", 1001);
    } catch (const DatabaseException& e) {
        std::cout << "Database error " << e.code() 
                  << ": " << e.what() << std::endl;
    }
}
```

#### 3. 异常指针（C++11）
```cpp
#include <exception>
#include <future>
#include <iostream>

void exception_ptr_example() {
    std::exception_ptr eptr;
    
    try {
        throw std::runtime_error("Error in thread");
    } catch (...) {
        eptr = std::current_exception();  // 捕获当前异常
    }
    
    // 在其他地方重新抛出
    if (eptr) {
        try {
            std::rethrow_exception(eptr);
        } catch (const std::exception& e) {
            std::cout << "Caught: " << e.what() << std::endl;
        }
    }
}
```

#### 4. 嵌套异常（C++11）
```cpp
#include <exception>
#include <stdexcept>
#include <iostream>

void nested_exception_example() {
    try {
        try {
            throw std::runtime_error("Inner exception");
        } catch (...) {
            std::throw_with_nested(std::logic_error("Outer exception"));
        }
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
        try {
            std::rethrow_if_nested(e);
        } catch (const std::exception& nested) {
            std::cout << "Nested: " << nested.what() << std::endl;
        }
    }
}
```

### 二、系统错误处理（C++11）

#### 1. error_code和error_condition
```cpp
#include <system_error>
#include <iostream>
#include <cerrno>

void system_error_example() {
    // 使用系统错误代码
    std::error_code ec(errno, std::system_category());
    if (ec) {
        std::cout << "System error: " << ec.message() << std::endl;
        std::cout << "Error code: " << ec.value() << std::endl;
        std::cout << "Category: " << ec.category().name() << std::endl;
    }
    
    // 使用标准错误条件
    std::error_condition cond(std::errc::no_such_file_or_directory);
    std::cout << "Error condition: " << cond.message() << std::endl;
    
    // 创建system_error异常
    try {
        throw std::system_error(ec, "File operation failed");
    } catch (const std::system_error& e) {
        std::cout << "System error: " << e.what() << std::endl;
        std::cout << "Code: " << e.code().value() << std::endl;
    }
}
```

#### 2. 自定义错误类别
```cpp
#include <system_error>
#include <string>

class CustomErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override {
        return "custom";
    }
    
    std::string message(int ev) const override {
        switch (ev) {
            case 1: return "Custom error 1";
            case 2: return "Custom error 2";
            default: return "Unknown custom error";
        }
    }
};

const CustomErrorCategory& custom_category() {
    static CustomErrorCategory instance;
    return instance;
}

std::error_code make_custom_error(int code) {
    return std::error_code(code, custom_category());
}

void custom_error_category_example() {
    auto ec = make_custom_error(1);
    std::cout << "Custom error: " << ec.message() << std::endl;
}
```

### 三、断言和调试工具

#### 1. 断言使用
```cpp
#include <cassert>
#include <vector>

void assertion_example() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 预条件检查
    assert(!vec.empty());
    
    // 范围检查
    size_t index = 2;
    assert(index < vec.size());
    
    std::cout << "Element at index " << index 
              << ": " << vec[index] << std::endl;
}

// 在发布版本中禁用断言
#ifdef NDEBUG
    // 断言被禁用
#else
    // 断言启用
#endif
```

#### 2. 静态断言（C++11）
```cpp
#include <type_traits>

template<typename T>
void static_assertion_example() {
    // 编译时断言
    static_assert(std::is_integral<T>::value, "T must be integral type");
    static_assert(sizeof(T) >= 4, "T must be at least 4 bytes");
    
    // C++17简化的静态断言
    static_assert(std::is_arithmetic_v<T>);
}

// 使用示例
void static_assert_usage() {
    static_assertion_example<int>();    // OK
    // static_assertion_example<double>(); // 编译错误
}
```

### 四、堆栈跟踪（C++23）

#### 1. 基础堆栈跟踪
```cpp
#include <stacktrace>
#include <iostream>

void stacktrace_example() {
    // 获取当前堆栈跟踪
    auto trace = std::stacktrace::current();
    
    std::cout << "Stack trace:\n" << trace << std::endl;
    
    // 遍历堆栈条目
    for (size_t i = 0; i < trace.size(); ++i) {
        const auto& entry = trace[i];
        std::cout << "#" << i << " " 
                  << entry.description() << " at "
                  << entry.source_file() << ":" 
                  << entry.source_line() << std::endl;
    }
}

void function_a() { stacktrace_example(); }
void function_b() { function_a(); }
void function_c() { function_b(); }
```

### 五、实际应用示例

#### 1. 异常安全的资源管理
```cpp
#include <memory>
#include <fstream>
#include <stdexcept>

class ResourceManager {
private:
    std::unique_ptr<int[]> data_;
    size_t size_;
    
public:
    explicit ResourceManager(size_t size) 
        : size_(size) {
        if (size == 0) {
            throw std::invalid_argument("Size cannot be zero");
        }
        
        data_ = std::make_unique<int[]>(size);
        if (!data_) {
            throw std::bad_alloc();
        }
    }
    
    int& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    size_t size() const { return size_; }
};

void exception_safe_example() {
    try {
        ResourceManager rm(10);
        rm[5] = 42;
        std::cout << "Value at index 5: " << rm[5] << std::endl;
        
        // 这会抛出异常
        rm[15] = 100;  // 超出范围
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}
```

#### 2. 错误处理策略
```cpp
#include <system_error>
#include <optional>
#include <variant>
#include <iostream>

// 错误码返回方式
enum class FileError {
    Success,
    FileNotFound,
    PermissionDenied,
    IOError
};

FileError read_file_traditional(const std::string& filename) {
    // 模拟文件读取
    if (filename.empty()) {
        return FileError::FileNotFound;
    }
    return FileError::Success;
}

// 使用optional表示可能失败的操作
std::optional<std::string> read_file_optional(const std::string& filename) {
    if (filename.empty()) {
        return std::nullopt;
    }
    return std::make_optional<std::string>("File content");
}

// 使用variant表示成功或错误
using FileResult = std::variant<std::string, FileError>;

FileResult read_file_variant(const std::string& filename) {
    if (filename.empty()) {
        return FileError::FileNotFound;
    }
    return std::string("File content");
}

// 使用异常处理
std::string read_file_exception(const std::string& filename) {
    if (filename.empty()) {
        throw std::system_error(
            std::make_error_code(std::errc::no_such_file_or_directory),
            "File not found"
        );
    }
    return "File content";
}

void error_handling_strategies() {
    // 传统错误码
    auto result = read_file_traditional("test.txt");
    if (result != FileError::Success) {
        std::cout << "File operation failed\n";
    }
    
    // Optional方式
    auto content = read_file_optional("test.txt");
    if (content) {
        std::cout << "Content: " << *content << std::endl;
    } else {
        std::cout << "File not found\n";
    }
    
    // Variant方式
    auto variant_result = read_file_variant("test.txt");
    if (std::holds_alternative<std::string>(variant_result)) {
        std::cout << "Content: " << std::get<std::string>(variant_result) << std::endl;
    } else {
        std::cout << "Error occurred\n";
    }
    
    // 异常方式
    try {
        auto content = read_file_exception("test.txt");
        std::cout << "Content: " << content << std::endl;
    } catch (const std::system_error& e) {
        std::cout << "System error: " << e.what() << std::endl;
    }
}
```

#### 3. 调试辅助工具
```cpp
#include <debugging>  // C++26
#include <iostream>

void debugging_example() {
    // 检查是否在调试器中运行
    if (std::is_debugger_present()) {
        std::cout << "Running under debugger\n";
        // 条件断点
        std::breakpoint_if_debugging();
    } else {
        std::cout << "Running normally\n";
    }
    
    // 强制断点
    // std::breakpoint(); // 取消注释会在调试器中暂停
}
```

#### 4. 综合错误处理框架
```cpp
#include <system_error>
#include <stdexcept>
#include <string>
#include <iostream>

// 统一的错误处理类
class ErrorHandler {
public:
    // 处理系统错误
    static void handle_system_error(const std::system_error& e) {
        std::cerr << "System error [" << e.code() << "]: " 
                  << e.what() << std::endl;
    }
    
    // 处理运行时错误
    static void handle_runtime_error(const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }
    
    // 处理逻辑错误
    static void handle_logic_error(const std::logic_error& e) {
        std::cerr << "Logic error: " << e.what() << std::endl;
    }
    
    // 处理未知异常
    static void handle_unknown_exception() {
        std::cerr << "Unknown exception caught!" << std::endl;
    }
    
    // 统一的异常处理包装器
    template<typename Func>
    static auto safe_execute(Func&& func) -> decltype(func()) {
        try {
            return std::forward<Func>(func)();
        } catch (const std::system_error& e) {
            handle_system_error(e);
        } catch (const std::runtime_error& e) {
            handle_runtime_error(e);
        } catch (const std::logic_error& e) {
            handle_logic_error(e);
        } catch (const std::exception& e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
        } catch (...) {
            handle_unknown_exception();
        }
        
        // 根据返回类型返回默认值
        if constexpr (std::is_same_v<decltype(func()), void>) {
            return;
        } else {
            return decltype(func()){};
        }
    }
};

// 使用示例
void comprehensive_error_handling() {
    // 安全执行可能抛出异常的函数
    ErrorHandler::safe_execute([]() {
        throw std::runtime_error("Test error");
        return 42;
    });
    
    // 安全执行不返回值的函数
    ErrorHandler::safe_execute([]() {
        std::cout << "Executing safe function\n";
    });
}
```

## 最佳实践和注意事项

### 一、异常安全保证

1. **基本保证**：如果异常被抛出，程序状态仍然有效
2. **强保证**：如果异常被抛出，程序状态保持不变
3. **不抛出保证**：操作不会抛出异常

### 二、异常处理原则

1. **不要忽略异常**：总是处理或重新抛出捕获的异常
2. **按值捕获，按引用抛出**：避免对象切片
3. **异常应该用于异常情况**：不要用异常控制正常流程
4. **提供有意义的错误信息**：在`what()`中提供有用的诊断信息

### 三、错误处理策略选择

| 策略 | 适用场景 | 优点 | 缺点 |
|------|----------|------|------|
| 异常 | 异常情况，错误处理复杂 | 清晰的错误分离，自动栈展开 | 性能开销，复杂性 |
| 错误码 | 性能关键，简单错误 | 高性能，简单 | 代码冗长，易忽略 |
| Optional | 可能失败但不异常的操作 | 类型安全，清晰表达 | 仅适用于单一错误 |
| Variant | 多种可能的错误类型 | 类型安全，多种错误 | 复杂性较高 |

## 相关页面

| 页面 | 说明 |
|------|------|
| `static_assert`声明(C++11) | 执行编译时断言检查 |
| C文档 - 错误处理 | C语言错误处理机制 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/error&oldid=179907>
- 最后修改时间：2025年1月28日 11:35
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++诊断库为现代C++程序提供了全面的错误处理和调试支持。从基础的异常处理机制到现代的系统错误报告，从编译时断言到运行时堆栈跟踪，诊断库不断演进以满足复杂软件开发的需求。理解异常安全保证、错误处理策略和调试工具的使用，是编写健壮、可维护C++代码的关键。正确使用异常处理可以提高代码的可读性和可靠性，而系统错误处理机制则提供了与操作系统底层交互的标准方式。持续学习和实践诊断库的新特性，是成为优秀C++开发者的重要途径。无论是传统的异常处理还是现代的堆栈跟踪和调试支持，这些工具都为开发者提供了强大的诊断和调试能力。