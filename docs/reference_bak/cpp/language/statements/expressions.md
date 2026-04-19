# C++ 表达式语句详解

  [1. 基本语法与语义](#1-基本语法与语义)  
  
  [2. 表达式语句类型详解](#2-表达式语句类型详解)  
  
  [3. 属性与表达式语句](#3-属性与表达式语句)  
  
  [4. 空语句详解](#4-空语句详解)  
  
  [5. 常见应用场景](#5-常见应用场景)  
  
  [6. 与其他语言特性的交互](#6-与其他语言特性的交互)  
  
  [7. 最佳实践与代码规范](#7-最佳实践与代码规范)  
  
  [8. 标准参考](#8-标准参考)  
  
  [9. 总结](#9-总结)

## 1. 基本语法与语义

### 1.1 表达式语句语法结构

C++ 表达式语句的基本语法如下：

```cpp
attr(optional) expression(optional) ;
```

**语法元素说明**：

- `attr(optional)`: （从 C++11 开始）任意数量属性的可选序列
- `expression(optional)`: 可选的表达式
- `;`: 必需的分号结束符

```cpp
#include <iostream>

void basic_expression_statement_examples() {
    // 最简单的表达式语句：空语句
    ;
    
    // 赋值表达式语句
    int x = 5;
    x = 10;
    
    // 函数调用表达式语句
    std::cout << "Hello, World!\n";
    
    // 自增/自减表达式语句
    x++;
    --x;
    
    // 复合赋值表达式语句
    x += 5;
    x *= 2;
    
    // 逗号表达式语句
    int y = 0;
    x = 1, y = 2, std::cout << "Comma expression\n";
    
    std::cout << "x = " << x << ", y = " << y << "\n";
}

int main() {
    basic_expression_statement_examples();
    return 0;
}
```

### 1.2 表达式语句的本质

表达式语句是将表达式的求值结果丢弃，仅执行其副作用的语句：

```cpp
#include <iostream>

class SideEffectDemo {
private:
    static int counter_;
    
public:
    // 构造函数有副作用（输出）
    SideEffectDemo() {
        std::cout << "Constructor called, counter = " << ++counter_ << "\n";
    }
    
    // 析构函数有副作用（输出）
    ~SideEffectDemo() {
        std::cout << "Destructor called, counter = " << --counter_ << "\n";
    }
    
    // 成员函数有副作用（修改状态和输出）
    void do_something() {
        std::cout << "Doing something\n";
    }
};

int SideEffectDemo::counter_ = 0;

void expression_statement_side_effects() {
    std::cout << "=== Expression Statement Side Effects ===\n";
    
    // 构造表达式语句（有构造副作用）
    SideEffectDemo();
    
    // 函数调用表达式语句（有函数副作用）
    SideEffectDemo().do_something();
    
    // 赋值表达式语句（有赋值副作用）
    int value = 42;
    value = 24;
    
    std::cout << "Final value: " << value << "\n";
}

int main() {
    expression_statement_side_effects();
    return 0;
}
```

### 1.3 表达式语句与表达式的区别

表达式语句是表达式的语句形式，主要区别在于：

```cpp
#include <iostream>

void expression_vs_statement() {
    // 表达式（可以作为其他表达式的一部分）
    int x = (5 + 3) * 2;  // (5 + 3) * 2 是表达式
    
    // 表达式语句（独立的语句）
    (5 + 3) * 2;  // 这是表达式语句，结果被丢弃
    
    // 函数调用表达式
    int result = std::max(5, 3);  // std::max(5, 3) 是表达式
    
    // 函数调用表达式语句
    std::max(5, 3);  // 这是表达式语句，返回值被丢弃
    
    std::cout << "x = " << x << ", result = " << result << "\n";
}

int main() {
    expression_vs_statement();
    return 0;
}
```

## 2. 表达式语句类型详解

### 2.1 赋值表达式语句

```cpp
#include <iostream>
#include <vector>

void assignment_expression_statements() {
    std::cout << "=== Assignment Expression Statements ===\n";
    
    // 基本赋值
    int x = 10;
    x = 20;
    std::cout << "x = " << x << "\n";
    
    // 复合赋值
    x += 5;   // x = x + 5
    std::cout << "x += 5: " << x << "\n";
    
    x -= 3;   // x = x - 3
    std::cout << "x -= 3: " << x << "\n";
    
    x *= 2;   // x = x * 2
    std::cout << "x *= 2: " << x << "\n";
    
    x /= 4;   // x = x / 4
    std::cout << "x /= 4: " << x << "\n";
    
    // 链式赋值
    int a, b, c;
    a = b = c = 42;
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << "\n";
    
    // 容器赋值
    std::vector<int> vec1 = {1, 2, 3};
    std::vector<int> vec2;
    vec2 = vec1;  // 调用赋值运算符
    std::cout << "vec2 size: " << vec2.size() << "\n";
}

int main() {
    assignment_expression_statements();
    return 0;
}
```

### 2.2 函数调用表达式语句

```cpp
#include <iostream>
#include <algorithm>
#include <vector>

void function_call_expression_statements() {
    std::cout << "=== Function Call Expression Statements ===\n";
    
    // 标准库函数调用
    std::cout << "Hello from function call!\n";
    
    // 自定义函数调用
    auto print_message = [](const std::string& msg) {
        std::cout << "Message: " << msg << "\n";
    };
    
    print_message("Direct call");
    
    // 成员函数调用
    std::vector<int> vec = {3, 1, 4, 1, 5};
    vec.push_back(9);  // 调用成员函数，有副作用
    std::cout << "Vector size after push_back: " << vec.size() << "\n";
    
    // 算法函数调用
    std::sort(vec.begin(), vec.end());  // 排序，有副作用
    std::cout << "Sorted vector: ";
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    
    // I/O 操作函数调用
    std::cin.ignore();  // 忽略输入缓冲区字符
}

int main() {
    function_call_expression_statements();
    return 0;
}
```

### 2.3 自增/自减表达式语句

```cpp
#include <iostream>
#include <iterator>

void increment_decrement_statements() {
    std::cout << "=== Increment/Decrement Expression Statements ===\n";
    
    // 前缀形式
    int x = 5;
    ++x;  // 前缀自增
    std::cout << "After ++x: " << x << "\n";
    
    --x;  // 前缀自减
    std::cout << "After --x: " << x << "\n";
    
    // 后缀形式
    x++;  // 后缀自增
    std::cout << "After x++: " << x << "\n";
    
    x--;  // 后缀自减
    std::cout << "After x--: " << x << "\n";
    
    // 在循环中的应用
    for (int i = 0; i < 3; ++i) {  // 前缀更高效
        std::cout << "Loop iteration " << i << "\n";
    }
    
    // 迭代器操作
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();
    ++it;  // 移动迭代器
    std::cout << "Second element: " << *it << "\n";
}

int main() {
    increment_decrement_statements();
    return 0;
}
```

### 2.4 逗号表达式语句

```cpp
#include <iostream>

void comma_expression_statements() {
    std::cout << "=== Comma Expression Statements ===\n";
    
    // 基本逗号表达式
    int x = 1, y = 2, z = 3;
    (x = 10, y = 20, z = 30);  // 逗号表达式语句
    std::cout << "x = " << x << ", y = " << y << ", z = " << z << "\n";
    
    // 逗号表达式的求值顺序
    int a = 0, b = 0;
    (a = 5, b = a * 2, std::cout << "a = " << a << ", b = " << b << "\n");
    
    // 在循环中的应用
    for (int i = 0, j = 10; i < 5; ++i, --j) {
        std::cout << "i = " << i << ", j = " << j << "\n";
    }
    
    // 复杂逗号表达式
    int result = (std::cout << "Calculating...", x = 42, y = 24, x + y);
    std::cout << "Result: " << result << "\n";
}

int main() {
    comma_expression_statements();
    return 0;
}
```

## 3. 属性与表达式语句

### 3.1 C++11 属性支持

从 C++11 开始，可以在表达式语句前添加属性说明符：

```cpp
#include <iostream>

void attribute_expression_statements() {
    std::cout << "=== Attribute Expression Statements ===\n";
    
    // [[noreturn]] 属性（通常用于函数，但可以用于表达式语句）
    // 注意：[[noreturn]] 通常不直接用于表达式语句
    
    // [[carries_dependency]] 属性
    // [[carries_dependency]] some_expression; // 在特定上下文中有用
    
    // [[deprecated]] 属性
    [[deprecated("This expression is deprecated")]]
    int deprecated_var = 42;
    deprecated_var = 24;  // 可能产生警告
    
    // [[fallthrough]] 属性（主要用于 switch 语句）
    int value = 2;
    switch (value) {
        case 1:
            std::cout << "Case 1\n";
            [[fallthrough]];
        case 2:
            std::cout << "Case 2\n";
            // [[fallthrough]]; // 最后一个 case 不需要
        default:
            std::cout << "Default case\n";
    }
    
    // [[maybe_unused]] 属性
    [[maybe_unused]] int unused_var = 100;
    unused_var = 200;  // 抑制未使用警告
    
    // [[likely]] 和 [[unlikely]] 属性（C++20）
    #if __cplusplus >= 202002L
    if (true) [[likely]] {
        [[likely]] std::cout << "Likely path\n";
    } else [[unlikely]] {
        [[unlikely]] std::cout << "Unlikely path\n";
    }
    #endif
}

int main() {
    attribute_expression_statements();
    return 0;
}
```

### 3.2 自定义属性

```cpp
#include <iostream>

// 定义自定义属性（需要编译器支持）
#if defined(__GNUC__) || defined(__clang__)
#define CUSTOM_ATTR [[gnu::always_inline]]
#else
#define CUSTOM_ATTR
#endif

void custom_attribute_example() {
    std::cout << "=== Custom Attribute Example ===\n";
    
    CUSTOM_ATTR int x = 42;
    CUSTOM_ATTR x = 24;
    
    std::cout << "x = " << x << "\n";
}

int main() {
    custom_attribute_example();
    return 0;
}
```

## 4. 空语句详解

### 4.1 空语句的定义与用途

空语句是没有表达式的表达式语句，通常用于：

```cpp
#include <iostream>

void empty_statement_examples() {
    std::cout << "=== Empty Statement Examples ===\n";
    
    // 1. 循环的空体
    int counter = 0;
    while (counter < 3) {
        std::cout << "Counter: " << counter << "\n";
        counter++;
        ; // 空语句作为循环体
    }
    
    // 2. for 循环的空体
    for (int i = 0; i < 3; std::cout << "Iteration " << i << "\n", i++) {
        ; // 空语句作为循环体
    }
    
    // 3. 在复合语句末尾携带标签（C++23 前）
    goto end_label;
    
    std::cout << "This won't be executed\n";
    
    end_label:
    ; // 空语句可以携带标签
    
    std::cout << "After end_label\n";
    
    // 4. 占位符语句
    if (true) {
        ; // 占位符，稍后添加实际代码
    } else {
        ; // 占位符
    }
}

int main() {
    empty_statement_examples();
    return 0;
}
```

### 4.2 空语句与标签的结合

```cpp
#include <iostream>

void empty_statement_with_labels() {
    std::cout << "=== Empty Statement with Labels ===\n";
    
    // 在复合语句末尾携带标签
    {
        std::cout << "Inside block\n";
        goto block_end;
        
        std::cout << "This won't be executed\n";
        
    block_end:
        ; // 空语句携带标签
    }
    
    std::cout << "After block\n";
    
    // 多个标签与空语句
    goto first_label;
    
first_label:
second_label:
third_label:
    ; // 单个空语句可以有多个标签
    
    std::cout << "After multiple labels\n";
}

int main() {
    empty_statement_with_labels();
    return 0;
}
```

### 4.3 空语句的语义

```cpp
#include <iostream>

void empty_statement_semantics() {
    std::cout << "=== Empty Statement Semantics ===\n";
    
    // 空语句不执行任何操作，不产生任何副作用
    ;
    
    // 空语句可以被优化器完全消除
    for (int i = 0; i < 1000000; ++i) {
        ; // 这个循环很可能被优化器消除
    }
    
    std::cout << "Empty statement semantics demonstrated\n";
    
    // 空语句在语法上是有效的语句
    if (true) {
        ; // 语法正确
    }
    
    // 空语句可以出现在任何需要语句的地方
    while (false) {
        ; // 语法正确
    }
}

int main() {
    empty_statement_semantics();
    return 0;
}
```

## 5. 常见应用场景

### 5.1 循环控制

```cpp
#include <iostream>
#include <vector>

void loop_control_examples() {
    std::cout << "=== Loop Control Examples ===\n";
    
    // 1. 空循环体用于延迟
    void* ptr = nullptr;
    while (ptr == nullptr) {
        ptr = malloc(1024);  // 模拟等待资源
        if (ptr) {
            free(ptr);
            break;
        }
        ; // 空语句作为循环体
    }
    
    // 2. 复杂的 for 循环条件
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();
    for (; it != vec.end() && *it < 4; ++it) {
        ; // 空语句，所有逻辑在 for 语句中
    }
    std::cout << "Iterator stopped at: " << *it << "\n";
    
    // 3. 带副作用的空循环
    int counter = 0;
    while (++counter < 5) {
        ; // 空语句，但 ++counter 有副作用
    }
    std::cout << "Counter final value: " << counter << "\n";
}

int main() {
    loop_control_examples();
    return 0;
}
```

### 5.2 条件编译占位

```cpp
#include <iostream>

void conditional_compilation_placeholder() {
    std::cout << "=== Conditional Compilation Placeholder ===\n";
    
    // 在条件编译中作为占位符
#ifdef DEBUG
    std::cout << "Debug mode\n";
#else
    ; // 生产模式下的空语句占位符
#endif
    
    // 根据配置选择不同的实现
#ifdef ENABLE_FEATURE_X
    // 复杂的特性 X 实现
    std::cout << "Feature X enabled\n";
#else
    ; // 空语句占位符
#endif
    
    std::cout << "Conditional compilation demonstrated\n";
}

int main() {
    conditional_compilation_placeholder();
    return 0;
}
```

### 5.3 错误处理框架

```cpp
#include <iostream>
#include <memory>

class ErrorHandler {
private:
    bool initialized_ = false;
    
public:
    void initialize() {
        initialized_ = true;
        std::cout << "Initialized\n";
    }
    
    bool is_initialized() const { return initialized_; }
};

void error_handling_with_empty_statements() {
    std::cout << "=== Error Handling with Empty Statements ===\n";
    
    ErrorHandler handler;
    
    // 使用空语句作为错误处理的占位符
    if (!handler.is_initialized()) {
        handler.initialize();
        ; // 初始化后的占位符
    }
    
    // 复杂的错误检查链
    if (true) {
        if (true) {
            if (false) {
                ; // 错误路径的占位符
            } else {
                std::cout << "Success path\n";
                ; // 成功路径的占位符
            }
        }
    }
    
    // 使用 goto 和空语句的错误处理
    goto normal_path;
    
error_path:
    ; // 错误处理的空语句占位符
    std::cout << "Error handling\n";
    return;
    
normal_path:
    ; // 正常路径的空语句占位符
    std::cout << "Normal execution\n";
}

int main() {
    error_handling_with_empty_statements();
    return 0;
}
```

## 6. 与其他语言特性的交互

### 6.1 与异常处理的交互

```cpp
#include <iostream>
#include <stdexcept>

void expression_statement_with_exceptions() {
    std::cout << "=== Expression Statement with Exceptions ===\n";
    
    try {
        // 函数调用表达式语句可能抛出异常
        throw std::runtime_error("Test exception");
        ; // 这个空语句不会被执行
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
    
    // 在 catch 块中使用表达式语句
    try {
        std::cout << "No exception here\n";
    } catch (...) {
        ; // 空语句作为异常处理的占位符
        std::cout << "Catch all handler\n";
    }
    
    // 表达式语句中的异常安全
    try {
        auto ptr = std::make_unique<int>(42);
        *ptr = 24;  // 赋值表达式语句
        ; // 空语句
    } catch (const std::bad_alloc& e) {
        std::cout << "Memory allocation failed\n";
    }
}

int main() {
    expression_statement_with_exceptions();
    return 0;
}
```

### 6.2 与模板的交互

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
void template_expression_statements(T value) {
    std::cout << "=== Template Expression Statements ===\n";
    
    if constexpr (std::is_integral_v<T>) {
        // 整数类型的表达式语句
        T result = value + 1;
        result = result * 2;
        std::cout << "Integral result: " << result << "\n";
        ; // 空语句占位符
    } else if constexpr (std::is_floating_point_v<T>) {
        // 浮点数类型的表达式语句
        T result = value * 1.5;
        result += 0.5;
        std::cout << "Floating point result: " << result << "\n";
        ; // 空语句占位符
    } else {
        // 其他类型的表达式语句
        std::cout << "Other type value: " << value << "\n";
        ; // 空语句占位符
    }
}

int main() {
    template_expression_statements(42);      // int
    template_expression_statements(3.14);    // double
    template_expression_statements("Hello"); // const char*
    return 0;
}
```

### 6.3 与 Lambda 表达式的交互

```cpp
#include <iostream>
#include <functional>

void lambda_expression_statements() {
    std::cout << "=== Lambda Expression Statements ===\n";
    
    // Lambda 作为表达式语句
    []() { std::cout << "Lambda expression statement\n"; }();
    
    // 带捕获的 Lambda 表达式语句
    int x = 42;
    [x]() { std::cout << "Captured value: " << x << "\n"; }();
    
    // Lambda 返回值被丢弃的表达式语句
    auto lambda = [](int a, int b) { return a + b; };
    lambda(5, 3);  // 返回值被丢弃
    
    // 在循环中使用 Lambda 表达式语句
    for (int i = 0; i < 3; ++i) {
        [i]() { std::cout << "Lambda iteration: " << i << "\n"; }();
        ; // 空语句占位符
    }
}

int main() {
    lambda_expression_statements();
    return 0;
}
```

### 6.4 与并发的交互

```cpp
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> global_counter{0};

void concurrent_expression_statements() {
    std::cout << "=== Concurrent Expression Statements ===\n";
    
    // 并发环境中的表达式语句
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            // 原子操作表达式语句
            global_counter.fetch_add(1);
            
            // I/O 表达式语句
            std::cout << "Thread " << i << " incremented counter\n";
            
            // 空语句占位符
            ;
        });
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final counter value: " << global_counter.load() << "\n";
}

int main() {
    concurrent_expression_statements();
    return 0;
}
```

## 7. 最佳实践与代码规范

### 7.1 合理使用空语句

```cpp
#include <iostream>

// 好的做法：合理使用空语句
void good_empty_statement_usage() {
    std::cout << "=== Good Empty Statement Usage ===\n";
    
    // 1. 循环中的占位符
    for (int i = 0; i < 1000000; ++i) {
        ; // 明确的占位符意图
    }
    
    // 2. 条件编译占位符
#ifdef FEATURE_ENABLED
    std::cout << "Feature enabled\n";
#else
    ; // 明确的占位符意图
#endif
    
    // 3. 标签携带（C++23 前）
    goto cleanup_label;
    
cleanup_label:
    ; // 清晰的标签携带意图
    
    std::cout << "Good usage demonstrated\n";
}

int main() {
    good_empty_statement_usage();
    return 0;
}
```

### 7.2 避免的做法

```cpp
#include <iostream>

// 避免的做法：不合理的空语句使用
void avoid_unnecessary_empty_statements() {
    std::cout << "=== Avoid Unnecessary Empty Statements ===\n";
    
    // 避免：无意义的空语句
    ;
    ;
    ; // 这些空语句没有实际用途
    
    // 避免：混淆的空语句
    if (true)
        ; // 容易被忽略的空语句
    
    // 更好的做法：使用明确的注释
    if (true) {
        // TODO: 实现此分支的逻辑
        ; // 占位符
    }
    
    std::cout << "Avoid unnecessary empty statements\n";
}

int main() {
    avoid_unnecessary_empty_statements();
    return 0;
}
```

### 7.3 代码风格建议

```cpp
#include <iostream>

class ExpressionStatementStyle {
public:
    void proper_style_examples() {
        std::cout << "=== Proper Style Examples ===\n";
        
        // 1. 清晰的表达式语句
        int x = 42;
        x = 24;  // 明确的赋值
        
        // 2. 合理的空语句使用
        for (int i = 0; i < 10; ++i) {
            ; // 循环体占位符，意图明确
        }
        
        // 3. 带注释的占位符
        #ifdef DEBUG_MODE
            std::cout << "Debug information\n";
        #else
            ; // 生产模式下无操作
        #endif
        
        // 4. 一致的格式
        std::cout << "First operation\n";
        ; // 占位符
        std::cout << "Second operation\n";
    }
};

int main() {
    ExpressionStatementStyle style;
    style.proper_style_examples();
    return 0;
}
```

### 7.4 现代 C++ 替代方案

```cpp
#include <iostream>
#include <optional>

// 使用现代 C++ 特性替代传统的表达式语句模式
void modern_alternatives() {
    std::cout << "=== Modern C++ Alternatives ===\n";
    
    // 1. 使用 std::optional 替代可能失败的操作
    auto safe_divide = [](int a, int b) -> std::optional<int> {
        if (b == 0) {
            return std::nullopt;  // 替代错误状态
        }
        return a / b;
    };
    
    auto result = safe_divide(10, 2);
    if (result) {
        std::cout << "Result: " << *result << "\n";
    }
    
    // 2. 使用 RAII 替代手动资源管理
    class ResourceGuard {
    public:
        ResourceGuard() { std::cout << "Resource acquired\n"; }
        ~ResourceGuard() { std::cout << "Resource released\n"; }
    };
    
    {
        ResourceGuard guard;  // 自动资源管理
        ; // 不需要手动清理
    }
    
    // 3. 使用智能指针替代原始指针
    auto ptr = std::make_unique<int>(42);
    *ptr = 24;  // 安全的表达式语句
    // 自动清理，不需要 delete
}

int main() {
    modern_alternatives();
    return 0;
}
```

## 8. 标准参考

### 8.1 ISO C++ 标准条款引用

**C++98/C++03 标准 (ISO/IEC 14882:1998/2003)**：

- §6.2: 表达式语句的语法和语义
- §6.2/1: 表达式语句的定义

**C++11 标准 (ISO/IEC 14882:2011)**：

- §6.2: 表达式语句
- §6.2/1: 属性说明符序列的支持

**C++14 标准 (ISO/IEC 14882:2014)**：

- §6.2: 表达式语句语义的澄清

**C++17 标准 (ISO/IEC 14882:2017)**：

- §9.2/1: 表达式语句语法

**C++20 标准 (ISO/IEC 14882:2020)**：

- §8.2: 表达式语句的语义

**C++23 标准 (ISO/IEC 14882:2023)**：

- §8.2: 空语句可以在复合语句中的任何位置携带标签

### 8.2 相关标准文档

- ISO/IEC 14882: Programming Languages - C++
- N3337: C++11 Standard Draft
- N4140: C++14 Standard Draft
- N4659: C++17 Standard Draft
- N4860: C++20 Standard Draft
- N4950: C++23 Standard Draft

## 9. 总结

### 9.1 C++ 表达式语句的特点

C++ 表达式语句是后跟分号的表达式，是 C++ 程序中最常见的语句类型。

**核心特性**：

- 语法简单: `expression;` 形式
- 副作用执行: 执行表达式的副作用，丢弃结果
- 类型多样: 支持各种表达式类型
- 属性支持: 从 C++11 开始支持属性说明符

### 9.2 表达式语句类型比较

| 类型 | 语法示例 | 特点 | 常见用途 |
|------|----------|------|----------|
| 赋值语句 | `x = 5;` | 修改变量值 | 变量初始化和更新 |
| 函数调用 | `func();` | 执行函数 | I/O 操作、算法调用 |
| 自增/自减 | `x++;` | 修改数值 | 循环控制、计数器 |
| 逗号表达式 | `a=1, b=2;` | 顺序执行 | 复杂表达式构造 |
| 空语句 | `;` | 无操作 | 占位符、循环体 |

### 9.3 实际应用场景

**循环控制**：

```cpp
#include <iostream>
#include <chrono>
#include <thread>

void loop_control_application() {
    std::cout << "=== Loop Control Application ===\n";
    
    // 延迟循环
    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - start).count() < 100) {
        ; // 空循环体，等待时间
    }
    
    std::cout << "Delay completed\n";
    
    // 复杂条件循环
    int counter = 0;
    for (; counter < 10 && counter % 2 == 0; counter += 2) {
        ; // 空循环体，所有逻辑在 for 语句中
    }
    
    std::cout << "Counter final value: " << counter << "\n";
}

int main() {
    loop_control_application();
    return 0;
}
```

**资源管理**：

```cpp
#include <iostream>
#include <memory>

class ResourceManager {
private:
    std::unique_ptr<int[]> data_;
    size_t size_;
    
public:
    ResourceManager(size_t size) : size_(size) {
        data_ = std::make_unique<int[]>(size);
        std::cout << "Resource allocated: " << size << " elements\n";
    }
    
    ~ResourceManager() {
        std::cout << "Resource deallocated\n";
    }
    
    void process() {
        // 处理资源的表达式语句
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = static_cast<int>(i);  // 赋值表达式语句
            ; // 处理占位符
        }
        
        std::cout << "Processing completed\n";
    }
};

void resource_management_example() {
    {
        ResourceManager manager(5);
        manager.process();
        ; // 清理前的占位符
    } // 自动清理资源
    
    std::cout << "Scope exited, resources cleaned up\n";
}

int main() {
    resource_management_example();
    return 0;
}
```

### 9.4 最佳实践总结

- 明确意图: 使用空语句时要明确其意图和用途
- 避免滥用: 不要无意义地使用空语句
- 注释说明: 为占位符空语句添加适当注释
- 现代替代: 优先使用智能指针、RAII 等现代 C++ 特性
- 一致风格: 在项目中保持一致的表达式语句使用风格

### 9.5 学习建议

- 理解基础: 深入理解表达式语句的基本概念和语义
- 实践应用: 在实际编程中练习各种表达式语句的使用
- 关注标准: 跟踪 C++ 标准对表达式语句的更新
- 代码审查: 在团队中建立表达式语句使用的代码审查机制
- 性能意识: 理解表达式语句的性能特征和优化可能性

表达式语句作为 C++ 程序的基础构建块，虽然语法简单，但在实际编程中扮演着重要角色。掌握其特性和最佳实践，能够帮助开发者编写更清晰、更高效的 C++ 代码。
