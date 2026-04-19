# C++ `do-while` 循环详解

  [1. 基本语法与语义](#1-基本语法与语义)  
  
  [2. 条件表达式的深入分析](#2-条件表达式的深入分析)  
  
  [3. 循环控制机制详解](#3-循环控制机制详解)  
  
  [4. 作用域与变量声明](#4-作用域与变量声明)  
  
  [5. 常见陷阱与高级技巧](#5-常见陷阱与高级技巧)  
  
  [6. 性能优化与编译器行为](#6-性能优化与编译器行为)  
  
  [7. 与其他语言特性的交互](#7-与其他语言特性的交互)  
  
  [8. 标准演进与未来特性](#8-标准演进与未来特性)  
  
  [9. 最佳实践与代码规范](#9-最佳实践与代码规范)  
  
  [10. 标准参考](#10-标准参考)  
  
  [11. 总结](#11-总结)

## 1. 基本语法与语义

### 1.1 语法结构详解

C++ `do-while` 循环的基本语法如下：

```cpp
attr(可选) do statement while (expression);
```

**属性说明符 (Attributes)**:

从 C++11 开始，可以在 `do-while` 循环前添加属性说明符：

```cpp
#include <iostream>

int main() {
    int x = 0;
    
    // [[likely]] 属性提示编译器循环很可能执行多次
    [[likely]] do {
        std::cout << x << " ";
        x++;
    } while (x < 5);
    std::cout << "\n";
    
    return 0;
}
```

### 1.2 语义分析

**控制流语义**:

`do-while` 循环至少执行一次循环体，然后根据条件表达式的布尔值决定是否继续执行。当条件为真时，继续执行循环体；当条件为假时，跳出循环。

```cpp
#include <iostream>

int main() {
    int counter = 0;
    
    std::cout << "Before do-while loop\n";
    
    do {
        std::cout << "Inside loop, counter = " << counter << "\n";
        counter++;
    } while (counter < 3);
    
    std::cout << "After do-while loop, counter = " << counter << "\n";
    
    return 0;
}
```

**与 while 循环的区别**:

```cpp
#include <iostream>

int main() {
    int x = 5;
    
    // while 循环：条件为假时不执行
    std::cout << "While loop:\n";
    while (x < 0) {
        std::cout << "This will not be printed\n";
        x++;
    }
    
    // do-while 循环：至少执行一次
    std::cout << "Do-while loop:\n";
    do {
        std::cout << "This will be printed once\n";
        x++;
    } while (x < 0);
    
    return 0;
}
```

**作用域语义**:

`do-while` 循环中的循环体有自己的作用域规则：

```cpp
#include <iostream>

int main() {
    int global_var = 100;
    
    do {
        int local_var = 42;
        std::cout << "local_var = " << local_var << "\n";
        std::cout << "global_var = " << global_var << "\n";
        local_var--; // 修改局部变量
    } while (false);
    // local_var 在这里不可见
    
    return 0;
}
```

## 2. 条件表达式的深入分析

### 2.1 表达式作为条件

`do-while` 循环的条件必须是一个表达式，它的值会被上下文转换为 `bool` 类型。

**基本类型转换**:

```cpp
#include <iostream>

int main() {
    // 整数到布尔的转换
    int x = 3;
    do {
        std::cout << "x = " << x << "\n";
        x--;
    } while (x);
    
    // 指针到布尔的转换
    char *ptr = "Hello";
    do {
        std::cout << *ptr;
        ptr++;
    } while (ptr && *ptr);
    std::cout << "\n";
    
    // 浮点数到布尔的转换
    double d = 1.0;
    do {
        std::cout << "d = " << d << "\n";
        d -= 0.3;
    } while (d > 0.1);
    
    return 0;
}
```

**用户定义类型转换**:

```cpp
#include <iostream>

class BooleanLike {
private:
    bool value_;
    
public:
    BooleanLike(bool value) : value_(value) {}
    
    // 用户定义的布尔转换
    explicit operator bool() const {
        return value_;
    }
};

int main() {
    BooleanLike condition(true);
    int counter = 0;
    
    do {
        std::cout << "Loop iteration " << counter << "\n";
        counter++;
        if (counter >= 3) {
            condition = BooleanLike(false);
        }
    } while (condition && counter < 5);
    
    return 0;
}
```

### 2.2 复杂表达式条件

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();
    
    do {
        std::cout << "Element: " << *it << "\n";
        ++it;
    } while (it != vec.end());
    
    return 0;
}
```

### 2.3 函数调用作为条件

```cpp
#include <iostream>
#include <random>

bool should_continue() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 1);
    return dis(gen) == 1;
}

int main() {
    int iteration = 0;
    
    do {
        std::cout << "Iteration " << iteration << "\n";
        iteration++;
    } while (should_continue() && iteration < 10);
    
    std::cout << "Final iteration count: " << iteration << "\n";
    
    return 0;
}
```

## 3. 循环控制机制详解

### 3.1 循环执行机制

`do-while` 循环至少执行一次循环体。

```cpp
#include <iostream>

int main() {
    int x = 0;
    
    do {
        std::cout << "x = " << x << "\n";
        x++;
    } while (x < 3);
    
    std::cout << "Final x = " << x << "\n";
    
    return 0;
}
```

### 3.2 循环终止机制

当条件为假时，循环终止。

```cpp
#include <iostream>

int main() {
    int x = 5;
    
    do {
        std::cout << "x = " << x << "\n";
        x--;
    } while (x > 0);
    
    std::cout << "Loop terminated, x = " << x << "\n";
    
    return 0;
}
```

### 3.3 break 语句

`break` 语句用于提前终止循环。

```cpp
#include <iostream>

int main() {
    int x = 0;
    
    do {
        if (x == 3) {
            std::cout << "Breaking at x = " << x << "\n";
            break;
        }
        std::cout << "x = " << x << "\n";
        x++;
    } while (x < 10);
    
    std::cout << "After loop, x = " << x << "\n";
    
    return 0;
}
```

### 3.4 continue 语句

`continue` 语句用于跳过当前迭代的剩余部分，直接进入下一次迭代的条件检查。

```cpp
#include <iostream>

int main() {
    int x = 0;
    
    do {
        x++;
        if (x % 2 == 0) {
            std::cout << "Skipping even number: " << x << "\n";
            continue;
        }
        std::cout << "Odd number: " << x << "\n";
    } while (x < 10);
    
    return 0;
}
```

### 3.5 嵌套循环

```cpp
#include <iostream>

int main() {
    int i = 0;
    
    do {
        std::cout << "Outer loop i = " << i << "\n";
        int j = 0;
        do {
            std::cout << "  Inner loop j = " << j << "\n";
            j++;
        } while (j < 2);
        i++;
    } while (i < 3);
    
    return 0;
}
```

## 4. 作用域与变量声明

### 4.1 变量作用域限制

循环体总是引入块作用域。其中声明的变量仅在循环体内可见。

```cpp
#include <iostream>

int main() {
    int x = 3;
    
    do {
        int local_var = x * 2;
        std::cout << "local_var = " << local_var << "\n";
        x--;
    } while (x > 0);
    // local_var 在这里不可见
    // std::cout << local_var; // 编译错误
    
    return 0;
}
```

### 4.2 复合语句中的变量声明

```cpp
#include <iostream>

int main() {
    int x = 3;
    
    do {
        int y = x * 2;
        std::cout << "y = " << y << "\n";
        
        double z = 3.14;
        std::cout << "z = " << z << "\n";
        
        x--;
    } while (x > 0);
    
    return 0;
}
```

### 4.3 外部变量的作用域

```cpp
#include <iostream>

int main() {
    int global_counter = 0;
    int loop_var = 3;
    
    do {
        std::cout << "Global counter: " << global_counter << "\n";
        std::cout << "Loop variable: " << loop_var << "\n";
        global_counter++;
        loop_var--;
    } while (loop_var > 0);
    
    std::cout << "Final global counter: " << global_counter << "\n";
    
    return 0;
}
```

## 5. 常见陷阱与高级技巧

### 5.1 忘记分号

`do-while` 循环末尾必须有分号，忘记会导致编译错误：

```cpp
#include <iostream>

int main() {
    int x = 0;
    
    do {
        std::cout << "x = " << x << "\n";
        x++;
    } while (x < 3) // 错误：忘记分号
    // 编译错误：expected ';' after do-while loop
    
    return 0;
}
```

### 5.2 无限循环

忘记更新循环条件会导致无限循环：

```cpp
#include <iostream>

int main() {
    int x = 0;
    
    // 错误：忘记更新 x
    do {
        std::cout << "x = " << x << "\n";
        // x++; // 忘记这一行会导致无限循环
    } while (x < 5);
    
    return 0;
}
```

### 5.3 浮点数比较陷阱

使用浮点数作为循环条件时要注意精度问题：

```cpp
#include <iostream>
#include <cmath>
#include <limits>

int main() {
    double d = 1.0;
    
    // 错误的做法
    do {
        std::cout << "d = " << d << "\n";
        d -= 0.1;
    } while (d > 0.0);
    
    // 正确的做法
    d = 1.0;
    const double epsilon = std::numeric_limits<double>::epsilon() * 100;
    do {
        std::cout << "d = " << d << "\n";
        d -= 0.1;
    } while (d > epsilon);
    
    return 0;
}
```

### 5.4 高级技巧：菜单系统实现

```cpp
#include <iostream>
#include <string>

int main() {
    int choice;
    
    do {
        std::cout << "\n=== Menu ===\n";
        std::cout << "1. Option 1\n";
        std::cout << "2. Option 2\n";
        std::cout << "3. Option 3\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                std::cout << "You selected Option 1\n";
                break;
            case 2:
                std::cout << "You selected Option 2\n";
                break;
            case 3:
                std::cout << "You selected Option 3\n";
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice, please try again\n";
                break;
        }
    } while (choice != 0);
    
    return 0;
}
```

### 5.5 输入验证

```cpp
#include <iostream>
#include <limits>

int main() {
    int number;
    
    do {
        std::cout << "Enter a positive number: ";
        std::cin >> number;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        if (number <= 0) {
            std::cout << "Number must be positive. Please try again.\n";
        }
    } while (number <= 0);
    
    std::cout << "You entered: " << number << "\n";
    
    return 0;
}
```

## 6. 性能优化与编译器行为

### 6.1 循环展开优化

编译器可能会对简单的 `do-while` 循环进行展开优化：

```cpp
#include <iostream>
#include <chrono>
#include <vector>

// 简单循环，可能被编译器展开
void simple_do_while_loop() {
    int sum = 0;
    int i = 0;
    do {
        sum += i;
        i++;
    } while (i < 10);
    std::cout << "Sum: " << sum << "\n";
}

// 复杂循环，不太可能被展开
void complex_do_while_loop() {
    std::vector<int> data(1000000);
    
    // 初始化数据
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = i;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    long long sum = 0;
    size_t i = 0;
    do {
        sum += data[i];
        i++;
    } while (i < data.size());
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Sum: " << sum << ", Time: " << duration.count() << " microseconds\n";
}

int main() {
    simple_do_while_loop();
    complex_do_while_loop();
    
    return 0;
}
```

### 6.2 编译器优化

```cpp
#include <iostream>

// 编译器可以优化的简单循环
int simple_do_while_loop_func(int n) {
    int sum = 0;
    int i = 0;
    do {
        sum += i;
        i++;
    } while (i < n);
    return sum;
}

int main() {
    // 测试简单循环
    std::cout << "simple_do_while_loop_func(10): " << simple_do_while_loop_func(10) << "\n";
    
    return 0;
}
```

### 6.3 常量折叠优化

```cpp
#include <iostream>

constexpr int compile_time_sum(int n) {
    int sum = 0;
    int i = 0;
    do {
        sum += i;
        i++;
    } while (i < n);
    return sum;
}

int main() {
    // 这些值在编译时就被计算出来了
    constexpr int result1 = compile_time_sum(5);
    constexpr int result2 = compile_time_sum(10);
    
    std::cout << "result1: " << result1 << "\n";
    std::cout << "result2: " << result2 << "\n";
    
    // 静态断言确保编译时计算正确
    static_assert(result1 == 10);
    static_assert(result2 == 45);
    
    return 0;
}
```

## 7. 与其他语言特性的交互

### 7.1 与异常处理的交互

```cpp
#include <iostream>
#include <stdexcept>

void risky_function(bool should_throw) {
    if (should_throw) {
        throw std::runtime_error("Something went wrong!");
    }
    std::cout << "Function executed successfully\n";
}

int main() {
    int counter = 0;
    
    try {
        do {
            try {
                if (counter == 1) {
                    risky_function(true);
                } else {
                    risky_function(false);
                }
            } catch (const std::exception& e) {
                std::cout << "Caught exception: " << e.what() << "\n";
            }
            counter++;
        } while (counter < 3);
    } catch (...) {
        std::cout << "Outer exception caught\n";
    }
    
    return 0;
}
```

### 7.2 与模板的交互

```cpp
#include <iostream>
#include <type_traits>
#include <vector>

template<typename T>
class DoWhileProcessor {
public:
    void process(const std::vector<T>& container) {
        auto it = container.begin();
        if (it != container.end()) {
            do {
                if constexpr (std::is_arithmetic_v<T>) {
                    std::cout << "Processing arithmetic value: " << (*it) * 2 << "\n";
                } else if constexpr (std::is_pointer_v<T>) {
                    std::cout << "Processing pointer value: " << **it << "\n";
                } else {
                    std::cout << "Processing other type\n";
                }
                ++it;
            } while (it != container.end());
        }
    }
};

int main() {
    DoWhileProcessor<int> int_processor;
    DoWhileProcessor<int*> ptr_processor;
    
    std::vector<int> int_vec = {1, 2, 3};
    std::vector<int*> ptr_vec = {new int(1), new int(2), new int(3)};
    
    int_processor.process(int_vec);
    ptr_processor.process(ptr_vec);
    
    // 清理内存
    for (auto ptr : ptr_vec) {
        delete ptr;
    }
    
    return 0;
}
```

### 7.3 与 Lambda 表达式的交互

```cpp
#include <iostream>
#include <functional>
#include <vector>

int main() {
    // Lambda 表达式中的 do-while 循环
    auto processor = [](std::vector<int>& vec) {
        auto it = vec.begin();
        if (it != vec.end()) {
            do {
                *it *= 2;
                ++it;
            } while (it != vec.end());
        }
    };
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    std::cout << "Before processing: ";
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    processor(vec);
    
    std::cout << "After processing: ";
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 7.4 与并发的交互

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> ready{false};
std::atomic<int> counter{0};

void worker_thread() {
    do {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, std::chrono::milliseconds(100), []{ return ready.load(); });
        
        if (ready) {
            std::cout << "Worker thread is processing, counter = " << counter << "\n";
            counter++;
            
            // 模拟一些工作
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    } while (counter < 5);
    
    std::cout << "Worker thread finished\n";
}

int main() {
    std::thread worker(worker_thread);
    
    // 主线程做一些准备工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    
    cv.notify_all();
    
    worker.join();
    
    return 0;
}
```

## 8. 标准演进与未来特性

### 8.1 C++26 预期特性

目前 `do-while` 循环的标准演进相对较少，但未来可能会有改进：

```cpp
#include <iostream>
#include <tuple>
#include <optional>

// 这是可能的未来特性（目前不支持）
std::optional<std::tuple<int, double>> get_data(bool success) {
    if (success) {
        return std::make_tuple(42, 3.14);
    }
    return std::nullopt;
}

int main() {
    bool success = true;
    
    // 目前不支持的语法（未来可能支持）
    /*
    do {
        auto [i, d] = get_data(success).value();
        std::cout << "i = " << i << ", d = " << d << "\n";
        success = false;
    } while (success);
    */
    
    std::cout << "Current do-while features are stable\n";
    
    return 0;
}
```

### 8.2 编译器支持现状

```cpp
#include <iostream>

// 检查编译器对不同特性的支持
#if __cplusplus >= 201103L
    #define HAS_CPP11
#endif

#if __cplusplus >= 201703L
    #define HAS_CPP17
#endif

#if __cplusplus >= 202002L
    #define HAS_CPP20
#endif

int main() {
#ifdef HAS_CPP11
    std::cout << "C++11 features are available\n";
    
    // 测试属性说明符
    [[likely]] do {
        std::cout << "do-while with attributes\n";
        break;
    } while (false);
#endif

#ifdef HAS_CPP17
    std::cout << "C++17 features are available\n";
#endif

#ifdef HAS_CPP20
    std::cout << "C++20 features are available\n";
#endif

    return 0;
}
```

## 9. 最佳实践与代码规范

### 9.1 代码风格建议

```cpp
#include <iostream>

// 好的实践：总是使用大括号
void good_practice() {
    int x = 0;
    
    do {
        std::cout << "x = " << x << "\n";
        x++;
    } while (x < 3);
}

// 避免的做法：省略大括号（对于多行循环体）
void avoid_practice() {
    int x = 0;
    
    do
        std::cout << "x = " << x << "\n";
        x++; // 这行在循环体内
    while (x < 3);
}

int main() {
    good_practice();
    avoid_practice();
    
    return 0;
}
```

### 9.2 复杂条件的处理

```cpp
#include <iostream>

// 好的实践：将复杂条件分解为命名变量
void good_complex_condition() {
    int age = 25;
    bool has_license = true;
    bool is_insured = true;
    int years_driving = 5;
    
    bool is_adult = age >= 18;
    bool can_drive_legally = has_license && is_insured;
    bool is_experienced_driver = years_driving >= 3;
    bool should_continue = is_adult && can_drive_legally && is_experienced_driver;
    
    do {
        std::cout << "Can drive legally\n";
        break; // 避免无限循环
    } while (should_continue);
}

// 避免的做法：复杂的嵌套条件
void avoid_complex_condition() {
    int age = 25;
    bool has_license = true;
    bool is_insured = true;
    int years_driving = 5;
    
    do {
        std::cout << "Can drive legally\n";
        break; // 避免无限循环
    } while (age >= 18 && has_license && is_insured && years_driving >= 3);
}

int main() {
    good_complex_condition();
    avoid_complex_condition();
    
    return 0;
}
```

### 9.3 错误处理最佳实践

```cpp
#include <iostream>
#include <optional>
#include <stdexcept>

// 好的实践：使用 optional 进行错误处理
std::optional<int> safe_divide(int a, int b) {
    if (b == 0) {
        return std::nullopt;
    }
    return a / b;
}

void good_error_handling() {
    int a = 10, b = 2;
    bool has_result = true;
    
    do {
        auto result = safe_divide(a, b);
        if (result.has_value()) {
            std::cout << "Result: " << *result << "\n";
            b--; // 修改 b
            if (b <= 0) has_result = false;
        } else {
            std::cout << "Division by zero error\n";
            has_result = false;
        }
    } while (has_result);
}

// 避免的做法：使用异常进行控制流
void avoid_exception_for_control_flow() {
    int a = 10, b = 2;
    bool continue_loop = true;
    
    do {
        try {
            if (b == 0) {
                throw std::runtime_error("Division by zero");
            }
            int result = a / b;
            std::cout << "Result: " << result << "\n";
            b--; // 修改 b
            if (b <= 0) continue_loop = false;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            continue_loop = false;
        }
    } while (continue_loop);
}

int main() {
    good_error_handling();
    avoid_exception_for_control_flow();
    
    return 0;
}
```

### 9.4 性能优化建议

```cpp
#include <iostream>
#include <chrono>
#include <vector>

// 好的实践：减少循环内的计算
void optimized_loop() {
    std::vector<int> data(1000000);
    
    // 初始化数据
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = i;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    long long sum = 0;
    size_t size = data.size(); // 提前计算大小
    size_t i = 0;
    do {
        sum += data[i];
        i++;
    } while (i < size);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Sum: " << sum << ", Time: " << duration.count() << " microseconds\n";
}

// 避免的做法：在循环内重复计算
void unoptimized_loop() {
    std::vector<int> data(1000000);
    
    // 初始化数据
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = i;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    long long sum = 0;
    size_t i = 0;
    do {
        sum += data[i];
        i++;
    } while (i < data.size()); // 每次都调用 size()
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Sum: " << sum << ", Time: " << duration.count() << " microseconds\n";
}

int main() {
    optimized_loop();
    unoptimized_loop();
    
    return 0;
}
```

## 10. 标准参考

### 10.1 ISO C++ 标准条款引用

**C++98/C++03 标准 (ISO/IEC 14882:1998/2003)**：

- §6.5.3/1: do-while 语句至少执行一次循环体
- §6.5.3/2: 表达式条件通过上下文转换为 bool 类型
- §6.5.3/3: 循环体具有块作用域

**C++11 标准 (ISO/IEC 14882:2011)**：

- §7.6.1: 属性说明符语法扩展
- §6.5.3: 带属性的 do-while 语句

**C++14 标准 (ISO/IEC 14882:2014)**：

- §5.18: 条件表达式求值规则的澄清

**C++17 标准 (ISO/IEC 14882:2017)**：

- §9.5.3/1: do-while 语句语法
- §9.5.3/2: 表达式条件的语义
- §9.5.3/3: 循环控制语句的语义

**C++20 标准 (ISO/IEC 14882:2020)**：

- §8.5.3: do-while 语句的语义

**C++23 标准 (ISO/IEC 14882:2023)**：

- §8.5.3: do-while 语句的语义

## 11. 总结

### 11.1 C++ `do-while` 循环的特点

C++ `do-while` 循环是三种循环结构中独特的一种，它保证循环体至少执行一次。这种特性使其在某些特定场景下非常有用。

**核心特性**:

- 至少执行一次: 无论条件如何，循环体都会至少执行一次
- 后置条件检查: 条件在循环体执行后检查
- 语法简洁: 相比其他循环结构，语法相对简单

### 11.2 与 `while` 和 `for` 循环的比较

| 特性 | do-while | while | for |
|------|----------|-------|-----|
| 至少执行次数 | 1次 | 0次 | 0次 |
| 条件检查时机 | 循环体后 | 循环体前 | 循环体前 |
| 适用场景 | 需要至少执行一次的场景 | 一般条件循环 | 已知迭代次数的场景 |
| 语法复杂度 | 简单 | 简单 | 复杂 |

### 11.3 实际应用场景

**输入验证**：

```cpp
#include <iostream>
#include <limits>

int main() {
    int number;
    
    do {
        std::cout << "Enter a positive number: ";
        std::cin >> number;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        if (number <= 0) {
            std::cout << "Number must be positive. Please try again.\n";
        }
    } while (number <= 0);
    
    std::cout << "You entered: " << number << "\n";
    
    return 0;
}
```

**菜单系统**：

```cpp
#include <iostream>

int main() {
    int choice;
    
    do {
        std::cout << "\n=== Menu ===\n";
        std::cout << "1. Option 1\n";
        std::cout << "2. Option 2\n";
        std::cout << "3. Option 3\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                std::cout << "You selected Option 1\n";
                break;
            case 2:
                std::cout << "You selected Option 2\n";
                break;
            case 3:
                std::cout << "You selected Option 3\n";
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice, please try again\n";
                break;
        }
    } while (choice != 0);
    
    return 0;
}
```

**游戏循环**：

```cpp
#include <iostream>
#include <chrono>
#include <thread>

enum class GameState {
    RUNNING,
    PAUSED,
    STOPPED
};

int main() {
    GameState state = GameState::RUNNING;
    int frame_count = 0;
    
    do {
        switch (state) {
            case GameState::RUNNING:
                std::cout << "Frame " << frame_count << ": Game is running\n";
                frame_count++;
                if (frame_count >= 5) {
                    state = GameState::STOPPED;
                }
                break;
            case GameState::PAUSED:
                std::cout << "Game is paused\n";
                state = GameState::STOPPED;
                break;
            case GameState::STOPPED:
                std::cout << "Game stopped\n";
                break;
        }
        
        // 模拟帧率控制
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (state != GameState::STOPPED);
    
    return 0;
}
```

### 11.4 最佳实践总结

- 明确使用场景: 只在确实需要至少执行一次循环体时使用 `do-while`
- 注意语法细节: 不要忘记末尾的分号
- 合理设计条件: 确保循环条件最终会变为假，避免无限循环
- 保持代码清晰: 使用大括号包围循环体，提高代码可读性
- 性能考虑: 在循环体内避免不必要的计算和函数调用

### 11.5 学习建议

- 理解语义差异: 深刻理解 `do-while` 与其他循环的区别
- 实践应用场景: 通过实际项目练习掌握其使用场景
- 关注性能: 了解编译器对 `do-while` 循环的优化策略
- 代码审查: 在团队中推广一致的编码规范

`do-while` 循环虽然使用频率不如 `while` 和 `for` 循环高，但在特定场景下具有不可替代的价值。掌握其特性和最佳实践，能够帮助开发者写出更加优雅和高效的代码。
