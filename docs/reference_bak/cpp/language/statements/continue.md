# C++ `continue` 语句详解

  [1. 基本语法与语义](#1-基本语法与语义)  
  
  [2. 在循环中的应用](#2-在循环中的应用)  
  
  [3. 控制流机制详解](#3-控制流机制详解)  
  
  [4. 作用域与变量销毁](#4-作用域与变量销毁)  
  
  [5. 常见陷阱与高级技巧](#5-常见陷阱与高级技巧)  
  
  [6. 性能影响与编译器行为](#6-性能影响与编译器行为)  
  
  [7. 与其他语言特性的交互](#7-与其他语言特性的交互)  
  
  [8. 最佳实践与代码规范](#8-最佳实践与代码规范)  
  
  [9. 标准参考](#9-标准参考)  
  
  [10. 总结](#10-总结)

## 1. 基本语法与语义

### 1.1 语法结构详解

C++ `continue` 语句的基本语法如下：

```cpp
attr(可选) continue;
```

**属性说明符 (Attributes)**:

从 C++11 开始，可以在 `continue` 语句前添加属性说明符：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    for (const auto& element : vec) {
        if (element == 3) {
            // [[unlikely]] 属性提示编译器这个分支不太可能执行
            [[unlikely]] continue;
        }
        std::cout << "Element: " << element << "\n";
    }
    
    return 0;
}
```

### 1.2 语义分析

**控制流语义**:

`continue` 语句用于跳过当前循环迭代的剩余部分，并立即开始下一次迭代（如果循环条件仍然满足）。它只能出现在 `for`、`while`、`do-while` 和基于范围的 `for` 循环的循环体中。

```cpp
#include <iostream>

int main() {
    std::cout << "Before loop\n";
    
    for (int i = 0; i < 10; ++i) {
        std::cout << "Start of iteration " << i << "\n";
        
        if (i % 2 == 0) {
            std::cout << "Skipping even number: " << i << "\n";
            continue; // 跳过当前迭代的剩余部分
        }
        
        std::cout << "Processing odd number: " << i << "\n";
        std::cout << "End of iteration " << i << "\n";
    }
    
    std::cout << "After loop\n";
    
    return 0;
}
```

**与 break 的区别**:

```cpp
#include <iostream>

int main() {
    std::cout << "Using continue:\n";
    for (int i = 0; i < 5; ++i) {
        if (i == 2) {
            continue; // 跳过当前迭代，继续下一次迭代
        }
        std::cout << "i = " << i << "\n";
    }
    
    std::cout << "\nUsing break:\n";
    for (int i = 0; i < 5; ++i) {
        if (i == 2) {
            break; // 完全退出循环
        }
        std::cout << "i = " << i << "\n";
    }
    
    return 0;
}
```

## 2. 在循环中的应用

### 2.1 在 for 循环中的应用

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 在传统 for 循环中使用 continue
    std::cout << "Traditional for loop:\n";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] % 2 == 0) {
            std::cout << "Skipping even number: " << vec[i] << "\n";
            continue;
        }
        std::cout << "Processing odd number: " << vec[i] << "\n";
    }
    
    // 在基于范围的 for 循环中使用 continue
    std::cout << "\nRange-based for loop:\n";
    for (const auto& element : vec) {
        if (element % 2 == 0) {
            std::cout << "Skipping even number: " << element << "\n";
            continue;
        }
        std::cout << "Processing odd number: " << element << "\n";
    }
    
    return 0;
}
```

### 2.2 在 while 循环中的应用

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto it = vec.begin();
    
    // 在 while 循环中使用 continue
    std::cout << "While loop:\n";
    while (it != vec.end()) {
        if (*it % 2 == 0) {
            std::cout << "Skipping even number: " << *it << "\n";
            ++it;
            continue;
        }
        std::cout << "Processing odd number: " << *it << "\n";
        ++it;
    }
    
    return 0;
}
```

### 2.3 在 do-while 循环中的应用

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto it = vec.begin();
    
    // 在 do-while 循环中使用 continue
    std::cout << "Do-while loop:\n";
    if (it != vec.end()) {
        do {
            if (*it % 2 == 0) {
                std::cout << "Skipping even number: " << *it << "\n";
                ++it;
                continue;
            }
            std::cout << "Processing odd number: " << *it << "\n";
            ++it;
        } while (it != vec.end());
    }
    
    return 0;
}
```

### 2.4 嵌套循环中的 continue

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    // continue 只影响最内层循环
    std::cout << "Nested loops with continue:\n";
    for (size_t i = 0; i < matrix.size(); ++i) {
        std::cout << "Row " << i << ":\n";
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] % 2 == 0) {
                std::cout << "  Skipping even number at (" << i << ", " << j << "): " << matrix[i][j] << "\n";
                continue; // 只跳过内层循环的当前迭代
            }
            std::cout << "  Processing odd number at (" << i << ", " << j << "): " << matrix[i][j] << "\n";
        }
    }
    
    return 0;
}
```

## 3. 控制流机制详解

### 3.1 continue 的执行机制

`continue` 语句的执行遵循以下步骤：

- 跳过当前循环迭代的剩余部分
- 执行循环的迭代表达式（对于 `for` 循环）
- 评估循环条件
- 如果条件为真，开始下一次迭代；否则退出循环

```cpp
#include <iostream>

int main() {
    std::cout << "For loop with continue:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "Start of iteration " << i << "\n";
        
        if (i == 2) {
            std::cout << "Continuing at i = " << i << "\n";
            continue; // 跳过剩余部分，执行 ++i，然后检查 i < 5
        }
        
        std::cout << "End of iteration " << i << "\n";
    }
    
    std::cout << "\nWhile loop with continue:\n";
    int j = 0;
    while (j < 5) {
        std::cout << "Start of iteration " << j << "\n";
        
        if (j == 2) {
            std::cout << "Continuing at j = " << j << "\n";
            ++j; // 在 continue 之前手动递增
            continue; // 跳过剩余部分，然后检查 j < 5
        }
        
        std::cout << "End of iteration " << j << "\n";
        ++j;
    }
    
    return 0;
}
```

### 3.2 不同循环类型的 continue 行为

```cpp
#include <iostream>

int main() {
    std::cout << "For loop behavior:\n";
    for (int i = 0; i < 5; ++i) {
        if (i == 2) {
            std::cout << "Continuing at i = " << i << "\n";
            continue; // 自动执行 ++i
        }
        std::cout << "i = " << i << "\n";
    }
    
    std::cout << "\nWhile loop behavior:\n";
    int j = 0;
    while (j < 5) {
        if (j == 2) {
            std::cout << "Continuing at j = " << j << "\n";
            ++j; // 必须手动递增，否则会无限循环
            continue;
        }
        std::cout << "j = " << j << "\n";
        ++j;
    }
    
    std::cout << "\nDo-while loop behavior:\n";
    int k = 0;
    do {
        if (k == 2) {
            std::cout << "Continuing at k = " << k << "\n";
            ++k; // 必须手动递增
            continue;
        }
        std::cout << "k = " << k << "\n";
        ++k;
    } while (k < 5);
    
    return 0;
}
```

### 3.3 多层嵌套中的控制流

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    std::cout << "Continue in nested loops:\n";
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 5) {
                std::cout << "Skipping element 5 at (" << i << ", " << j << ")\n";
                continue; // 只影响内层循环
            }
            std::cout << "Element (" << i << ", " << j << "): " << matrix[i][j] << "\n";
        }
        std::cout << "Finished row " << i << "\n";
    }
    
    return 0;
}
```

## 4. 作用域与变量销毁

### 4.1 循环变量的销毁

```cpp
#include <iostream>
#include <vector>

class Resource {
private:
    int id_;
    
public:
    Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " constructed\n";
    }
    
    ~Resource() {
        std::cout << "Resource " << id_ << " destroyed\n";
    }
};

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    std::cout << "Before loop\n";
    
    for (const auto& element : vec) {
        Resource res(element); // 在每次迭代中创建资源
        
        if (element == 3) {
            std::cout << "Continuing at element " << element << "\n";
            continue; // 资源会被销毁，然后开始下一次迭代
        }
        
        std::cout << "Processing element " << element << "\n";
    }
    
    std::cout << "After loop\n";
    
    return 0;
}
```

### 4.2 条件变量的销毁

```cpp
#include <iostream>

int main() {
    std::cout << "Before loop\n";
    
    for (int i = 0; i < 5; ++i) {
        int loop_var = i * 2; // 在每次迭代中创建
        std::cout << "loop_var = " << loop_var << "\n";
        
        if (i == 2) {
            std::cout << "Continuing at i = " << i << "\n";
            continue; // loop_var 会被销毁，然后开始下一次迭代
        }
        
        double temp_var = 3.14; // 只在不 continue 的情况下创建
        std::cout << "temp_var = " << temp_var << "\n";
    }
    
    std::cout << "After loop\n";
    
    return 0;
}
```

## 5. 常见陷阱与高级技巧

### 5.1 在 while 循环中忘记递增变量

```cpp
#include <iostream>

int main() {
    // 错误：在 while 循环中使用 continue 时忘记递增变量
    /*
    int i = 0;
    while (i < 5) {
        if (i == 2) {
            continue; // 这会导致无限循环，因为 i 永远不会递增
        }
        std::cout << "i = " << i << "\n";
        ++i;
    }
    */
    
    // 正确：在 continue 之前递增变量
    int j = 0;
    while (j < 5) {
        if (j == 2) {
            std::cout << "Continuing at j = " << j << "\n";
            ++j; // 在 continue 之前递增
            continue;
        }
        std::cout << "j = " << j << "\n";
        ++j;
    }
    
    return 0;
}
```

### 5.2 复杂条件逻辑中的 continue

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 好的做法：清晰的条件检查
    std::cout << "Clear condition checking:\n";
    for (const auto& element : vec) {
        // 检查多个条件
        if (element < 3 || element > 7) {
            std::cout << "Skipping element " << element << " (out of range 3-7)\n";
            continue;
        }
        
        if (element % 2 == 0) {
            std::cout << "Skipping even element " << element << "\n";
            continue;
        }
        
        std::cout << "Processing valid odd element: " << element << "\n";
    }
    
    return 0;
}
```

### 5.3 使用布尔标志避免复杂的 continue 逻辑

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用布尔标志控制处理逻辑
    for (const auto& element : vec) {
        bool should_process = true;
        
        // 检查条件
        if (element < 3 || element > 7) {
            std::cout << "Element " << element << " out of range\n";
            should_process = false;
        }
        
        if (element % 2 == 0) {
            std::cout << "Element " << element << " is even\n";
            should_process = false;
        }
        
        if (should_process) {
            std::cout << "Processing element: " << element << "\n";
        } else {
            std::cout << "Skipping element: " << element << "\n";
        }
    }
    
    return 0;
}
```

### 5.4 高级技巧：使用函数封装复杂逻辑

```cpp
#include <iostream>
#include <vector>

bool should_process_element(int element) {
    // 复杂的条件检查逻辑
    if (element < 3 || element > 7) {
        return false;
    }
    
    if (element % 2 == 0) {
        return false;
    }
    
    return true;
}

void process_element(int element) {
    std::cout << "Processing element: " << element << "\n";
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    for (const auto& element : vec) {
        if (!should_process_element(element)) {
            std::cout << "Skipping element: " << element << "\n";
            continue;
        }
        
        process_element(element);
    }
    
    return 0;
}
```

## 6. 性能影响与编译器行为

### 6.1 编译器优化

```cpp
#include <iostream>
#include <vector>
#include <chrono>

// 编译器可以优化的简单循环
void optimized_continue_loop(const std::vector<int>& vec) {
    for (const auto& element : vec) {
        if (element % 2 == 0) {
            continue; // 编译器可以优化这个 continue
        }
        // 模拟一些处理
        volatile int temp = element * 2;
    }
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    optimized_continue_loop(vec);
    
    return 0;
}
```

### 6.2 性能对比：带 continue vs 不带 continue

```cpp
#include <iostream>
#include <vector>
#include <chrono>

void loop_with_continue(const std::vector<int>& vec) {
    for (const auto& element : vec) {
        if (element % 2 == 0) {
            continue;
        }
        // 模拟一些处理
        volatile int temp = element * 2;
    }
}

void loop_without_continue(const std::vector<int>& vec) {
    for (const auto& element : vec) {
        if (element % 2 != 0) {
            // 模拟一些处理
            volatile int temp = element * 2;
        }
    }
}

int main() {
    std::vector<int> vec(1000000);
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = i % 100;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    loop_with_continue(vec);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    start = std::chrono::high_resolution_clock::now();
    loop_without_continue(vec);
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "With continue: " << duration1.count() << " microseconds\n";
    std::cout << "Without continue: " << duration2.count() << " microseconds\n";
    
    return 0;
}
```

## 7. 与其他语言特性的交互

### 7.1 与异常处理的交互

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>

void risky_operation(int value) {
    if (value == 5) {
        throw std::runtime_error("Value is 5!");
    }
    std::cout << "Processing value: " << value << "\n";
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7};
    
    for (const auto& element : vec) {
        try {
            risky_operation(element);
        } catch (const std::exception& e) {
            std::cout << "Caught exception for element " << element << ": " << e.what() << "\n";
            continue; // 继续处理下一个元素
        }
        
        if (element % 2 == 0) {
            std::cout << "Skipping even number: " << element << "\n";
            continue;
        }
        
        std::cout << "Successfully processed odd number: " << element << "\n";
    }
    
    return 0;
}
```

### 7.2 与模板的交互

```cpp
#include <iostream>
#include <vector>
#include <type_traits>

template<typename Container>
void process_container(const Container& container) {
    for (const auto& element : container) {
        if constexpr (std::is_arithmetic_v<typename Container::value_type>) {
            if (element % 2 == 0) {
                std::cout << "Skipping even number: " << element << "\n";
                continue;
            }
            std::cout << "Processing odd number: " << element << "\n";
        } else {
            std::cout << "Processing element: " << element << "\n";
        }
    }
}

int main() {
    std::vector<int> int_vec = {1, 2, 3, 4, 5};
    std::vector<std::string> string_vec = {"Hello", "World", "C++"};
    
    process_container(int_vec);
    process_container(string_vec);
    
    return 0;
}
```

### 7.3 与 Lambda 表达式的交互

```cpp
#include <iostream>
#include <vector>
#include <functional>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Lambda 表达式中的 continue
    auto processor = [&vec]() {
        for (const auto& element : vec) {
            if (element % 2 == 0) {
                std::cout << "Skipping even number: " << element << "\n";
                continue;
            }
            std::cout << "Processing odd number: " << element << "\n";
        }
    };
    
    processor();
    
    return 0;
}
```

### 7.4 与并发的交互

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx;
std::vector<int> shared_data(1000000);

void worker_thread(const std::vector<int>& source_data, size_t start, size_t end, int thread_id) {
    for (size_t i = start; i < end && i < source_data.size(); ++i) {
        if (source_data[i] % 2 == 0) {
            // 跳过偶数
            continue;
        }
        
        std::lock_guard<std::mutex> lock(mtx);
        shared_data[i] = source_data[i] * thread_id;
    }
}

int main() {
    std::vector<int> source_data(1000000);
    for (size_t i = 0; i < source_data.size(); ++i) {
        source_data[i] = i % 100;
    }
    
    const int num_threads = 4;
    const size_t chunk_size = source_data.size() / num_threads;
    
    std::vector<std::thread> threads;
    
    // 创建多个工作线程
    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? source_data.size() : (i + 1) * chunk_size;
        threads.emplace_back(worker_thread, std::cref(source_data), start, end, i + 1);
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    // 验证结果
    std::cout << "First 20 elements: ";
    for (size_t i = 0; i < 20; ++i) {
        std::cout << shared_data[i] << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

## 8. 最佳实践与代码规范

### 8.1 代码风格建议

```cpp
#include <iostream>
#include <vector>

// 好的实践：清晰的 continue 使用
void good_continue_usage() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    for (const auto& element : vec) {
        // 明确的条件检查
        if (element % 2 == 0) {
            std::cout << "Skipping even number: " << element << "\n";
            continue;
        }
        
        std::cout << "Processing odd number: " << element << "\n";
    }
}

// 避免的做法：复杂的嵌套条件
void avoid_complex_conditions() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    for (const auto& element : vec) {
        // 避免深层嵌套
        if (element >= 3) {
            if (element <= 7) {
                if (element % 2 != 0) {
                    std::cout << "Processing element: " << element << "\n";
                } else {
                    std::cout << "Skipping even number: " << element << "\n";
                    continue;
                }
            } else {
                std::cout << "Skipping element out of upper range: " << element << "\n";
                continue;
            }
        } else {
            std::cout << "Skipping element out of lower range: " << element << "\n";
            continue;
        }
    }
}

int main() {
    good_continue_usage();
    avoid_complex_conditions();
    
    return 0;
}
```

### 8.2 条件检查的最佳实践

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 好的做法：早期返回/继续
    for (const auto& element : vec) {
        // 检查无效条件并尽早 continue
        if (element < 3) {
            std::cout << "Element " << element << " too small, skipping\n";
            continue;
        }
        
        if (element > 7) {
            std::cout << "Element " << element << " too large, skipping\n";
            continue;
        }
        
        if (element % 2 == 0) {
            std::cout << "Element " << element << " is even, skipping\n";
            continue;
        }
        
        // 处理有效的元素
        std::cout << "Processing valid odd element in range: " << element << "\n";
    }
    
    return 0;
}
```

### 8.3 错误处理最佳实践

```cpp
#include <iostream>
#include <vector>
#include <optional>

// 好的实践：使用现代 C++ 特性进行错误处理
std::optional<int> safe_process(int value) {
    if (value < 0) {
        return std::nullopt; // 无效值
    }
    
    if (value % 2 == 0) {
        return std::nullopt; // 偶数不处理
    }
    
    return value * 2; // 处理奇数
}

void good_error_handling() {
    std::vector<int> vec = {-1, 0, 1, 2, 3, 4, 5};
    
    for (const auto& element : vec) {
        auto result = safe_process(element);
        if (!result.has_value()) {
            std::cout << "Skipping element " << element << "\n";
            continue;
        }
        
        std::cout << "Processed element " << element << " -> " << result.value() << "\n";
    }
}

int main() {
    good_error_handling();
    
    return 0;
}
```

### 8.4 性能优化建议

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// 好的实践：使用标准库算法而不是手动循环
void optimized_filter() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用标准库算法和 lambda
    std::for_each(vec.begin(), vec.end(), [](int element) {
        if (element % 2 == 0) {
            std::cout << "Skipping even number: " << element << "\n";
            return; // 在 lambda 中使用 return 而不是 continue
        }
        std::cout << "Processing odd number: " << element << "\n";
    });
}

// 避免的做法：手动实现过滤逻辑
void manual_filter() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    for (const auto& element : vec) {
        if (element % 2 == 0) {
            std::cout << "Skipping even number: " << element << "\n";
            continue;
        }
        std::cout << "Processing odd number: " << element << "\n";
    }
}

int main() {
    optimized_filter();
    manual_filter();
    
    return 0;
}
```

## 9. 标准参考

### 9.1 ISO C++ 标准条款引用

**C++98/C++03 标准 (ISO/IEC 14882:1998/2003)**：

- §6.6.2: continue 语句的语法和语义
- §6.6.2/1: continue 语句只能出现在迭代语句的循环体中
- §6.6.2/2: continue 语句导致跳转到循环体的末尾

**C++11 标准 (ISO/IEC 14882:2011)**：

- §6.6.2: continue 语句的语法和语义
- §6.6.2/1: 属性说明符序列的支持

**C++14 标准 (ISO/IEC 14882:2014)**：

- §6.6.2: continue 语句语义的澄清

**C++17 标准 (ISO/IEC 14882:2017)**：

- §9.6.2/1: continue 语句语法

**C++20 标准 (ISO/IEC 14882:2020)**：

- §8.6.2: continue 语句的语义

## 10. 总结

### 10.1 C++ `continue` 语句的特点

C++ `continue` 语句是一个重要的控制流语句，用于跳过当前循环迭代的剩余部分并开始下一次迭代。

**核心特性**:

- 跳过迭代: 跳过当前循环迭代的剩余部分
- 继续执行: 立即开始下一次迭代（如果条件满足）
- 作用域控制: 正确处理变量的销毁和作用域
- 灵活性: 可以与属性说明符结合使用

### 10.2 与其他控制流语句的比较

| 特性 | continue | break | return | goto |
|------|----------|-------|--------|------|
| 作用范围 | 当前迭代 | 最内层循环/switch | 函数 | 任意标签 |
| 执行效果 | 跳过当前迭代 | 完全退出 | 退出函数 | 跳转到标签 |
| 使用场景 | 跳过某些迭代 | 提前退出循环 | 函数返回 | 复杂控制流 |
| 安全性 | 高 | 高 | 高 | 低 |

### 10.3 实际应用场景

**数据过滤**：

```cpp
#include <iostream>
#include <vector>

template<typename T>
void filter_and_process(const std::vector<T>& vec) {
    for (const auto& element : vec) {
        // 过滤无效数据
        if (element <= 0) {
            std::cout << "Skipping invalid element: " << element << "\n";
            continue;
        }
        
        // 过滤不需要处理的数据
        if (element % 2 == 0) {
            std::cout << "Skipping even element: " << element << "\n";
            continue;
        }
        
        // 处理有效的奇数元素
        std::cout << "Processing valid odd element: " << element << "\n";
    }
}

int main() {
    std::vector<int> numbers = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    filter_and_process(numbers);
    
    return 0;
}
```

**输入验证**：

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <limits>

bool validate_input(const std::string& input) {
    if (input.empty()) {
        return false;
    }
    
    if (input.length() > 10) {
        return false;
    }
    
    return true;
}

void process_inputs() {
    std::vector<std::string> inputs = {"", "hello", "very_long_string", "world", "C++"};
    
    for (const auto& input : inputs) {
        if (!validate_input(input)) {
            std::cout << "Invalid input: '" << input << "', skipping\n";
            continue;
        }
        
        std::cout << "Processing valid input: '" << input << "'\n";
    }
}

int main() {
    process_inputs();
    
    return 0;
}
```

**批处理操作**：

```cpp
#include <iostream>
#include <vector>
#include <fstream>

struct Task {
    std::string filename;
    bool important;
};

void process_tasks(const std::vector<Task>& tasks) {
    for (const auto& task : tasks) {
        // 跳过不重要的任务
        if (!task.important) {
            std::cout << "Skipping non-important task: " << task.filename << "\n";
            continue;
        }
        
        // 尝试打开文件
        std::ifstream file(task.filename);
        if (!file.is_open()) {
            std::cout << "Cannot open file: " << task.filename << ", skipping\n";
            continue;
        }
        
        std::cout << "Processing important task: " << task.filename << "\n";
        // 实际的文件处理逻辑...
    }
}

int main() {
    std::vector<Task> tasks = {
        {"file1.txt", true},
        {"nonexistent.txt", true},
        {"file2.txt", false},
        {"file3.txt", true}
    };
    
    process_tasks(tasks);
    
    return 0;
}
```

### 10.4 最佳实践总结

- 正确使用 continue: 在需要跳过当前迭代时使用 continue
- 注意 while 循环: 在 while 循环中使用 continue 时要确保变量正确递增
- 保持条件清晰: 使用清晰的条件检查，避免复杂的嵌套逻辑
- 早期 continue: 尽早使用 continue 跳过不需要处理的情况
- 保持代码可读性: 使用有意义的变量名和注释

### 10.5 学习建议

- 掌握基础语法: 熟练掌握 continue 语句的基本语法和语义
- 理解不同循环类型: 深入理解 continue 在不同循环类型中的行为差异
- 实践不同场景: 通过实际项目练习掌握各种使用场景
- 关注性能影响: 了解 continue 语句对程序性能的影响
- 学习现代替代方案: 了解标准库算法等现代 C++ 特性

`continue` 语句作为 C++ 中重要的控制流语句，在循环控制中发挥着关键作用。掌握其特性和最佳实践，能够帮助开发者写出更加优雅、高效和可维护的代码。
