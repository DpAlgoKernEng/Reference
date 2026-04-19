# C++ 基于范围的 `for` 循环详解

  [1. 基本语法与语义](#1-基本语法与语义)  
  
  [2. 初始化语句的深入分析](#2-初始化语句的深入分析)  
  
  [3. 项目声明的深入分析](#3-项目声明的深入分析)  
  
  [4. 范围初始值设定项的深入分析](#4-范围初始值设定项的深入分析)  
  
  [5. 循环控制机制详解](#5-循环控制机制详解)  
  
  [6. 作用域与变量声明](#6-作用域与变量声明)  
  
  [7. 常见陷阱与高级技巧](#7-常见陷阱与高级技巧)  
  
  [8. 性能优化与编译器行为](#8-性能优化与编译器行为)  
  
  [9. 与其他语言特性的交互](#9-与其他语言特性的交互)  
  
  [10. 标准演进与未来特性](#10-标准演进与未来特性)  
  
  [11. 最佳实践与代码规范](#11-最佳实践与代码规范)  
  
  [12. 标准参考](#12-标准参考)  
  
  [13. 总结](#13-总结)

## 1. 基本语法与语义

### 1.1 语法结构详解

C++ 基于范围的 `for` 循环（Range-based for loop）的基本语法如下：

```cpp
attr(可选) for (init-statement(可选) item-declaration : range-initializer) statement
```

**属性说明符 (Attributes)**:

可以在基于范围的 `for` 循环前添加属性说明符：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // [[likely]] 属性提示编译器循环很可能执行多次
    [[likely]] for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 1.2 语义分析

**控制流语义**:

基于范围的 `for` 循环是传统 `for` 循环的简化版本，专门用于遍历容器或数组中的所有元素。其等价于以下代码：

```cpp
{
    auto&& range = range-initializer;
    auto begin = begin-expr;
    auto end = end-expr;
    for (; begin != end; ++begin) {
        item-declaration = *begin;
        statement
    }
}
```

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    std::cout << "Range-based for loop:\n";
    // 基于范围的 for 循环
    for (const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
    }
    
    std::cout << "\nEquivalent traditional for loop:\n";
    // 等价的传统 for 循环
    {
        auto&& range = vec;
        auto begin = range.begin();
        auto end = range.end();
        for (; begin != end; ++begin) {
            const auto& element = *begin;
            std::cout << "Element: " << element << "\n";
        }
    }
    
    return 0;
}
```

**作用域语义**:

基于范围的 `for` 循环中的项目声明有自己的作用域规则：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int global_var = 100;
    
    for (const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
        std::cout << "Global var: " << global_var << "\n";
        // element 在这里可见
    }
    // element 在这里不可见
    
    return 0;
}
```

## 2. 初始化语句的深入分析

### 2.1 表达式语句作为初始化 (C++20)

从 C++20 开始，可以在基于范围的 `for` 循环中使用初始化语句：

```cpp
#include <iostream>
#include <vector>

int main() {
    // 使用表达式语句作为初始化
    for (std::vector<int> vec = {1, 2, 3, 4, 5}; const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
    }
    
    // 使用空表达式语句
    for (; const auto& element : std::vector<int>{6, 7, 8, 9, 10}) {
        std::cout << "Element: " << element << "\n";
        break; // 避免无限循环
    }
    
    return 0;
}
```

### 2.2 变量声明作为初始化 (C++20)

```cpp
#include <iostream>
#include <vector>

int main() {
    // 声明单个变量
    for (int multiplier = 2; auto& element : std::vector<int>{1, 2, 3, 4, 5}) {
        element *= multiplier;
        std::cout << "Modified element: " << element << "\n";
    }
    
    // 声明多个变量
    for (int i = 0, size = 5; auto& element : std::vector<int>(size, 1)) {
        element += i;
        std::cout << "Element: " << element << "\n";
        i++;
    }
    
    return 0;
}
```

### 2.3 别名声明作为初始化 (C++23)

```cpp
#include <iostream>
#include <vector>

template<typename T>
void process_container(const T& container) {
    // 使用别名声明作为初始化
    for (using value_type = typename T::value_type; const auto& element : container) {
        std::cout << "Element: " << element << "\n";
    }
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    process_container(vec);
    
    return 0;
}
```

## 3. 项目声明的深入分析

### 3.1 值传递

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> vec = {"Hello", "World", "C++"};
    
    // 值传递 - 创建元素的拷贝
    for (auto element : vec) {
        std::cout << "Element: " << element << "\n";
        element += " Modified"; // 不影响原容器
    }
    
    std::cout << "Original container:\n";
    for (const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
    }
    
    return 0;
}
```

### 3.2 引用传递

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> vec = {"Hello", "World", "C++"};
    
    // 引用传递 - 直接操作元素
    for (auto& element : vec) {
        std::cout << "Element: " << element << "\n";
        element += " Modified"; // 修改原容器
    }
    
    std::cout << "Modified container:\n";
    for (const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
    }
    
    return 0;
}
```

### 3.3 常量引用传递

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> vec = {"Hello", "World", "C++"};
    
    // 常量引用传递 - 只读访问
    for (const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
        // element += " Modified"; // 编译错误：不能修改常量引用
    }
    
    return 0;
}
```

### 3.4 转发引用 (Universal References)

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> vec = {"Hello", "World", "C++"};
    
    // 转发引用 - 根据容器的引用类型自动推导
    for (auto&& element : vec) {
        std::cout << "Element: " << element << "\n";
        // 对于非常量容器，element 是非const引用
        // 对于常量容器，element 是const引用
    }
    
    const auto& const_vec = vec;
    for (auto&& element : const_vec) {
        std::cout << "Const element: " << element << "\n";
        // element 是const引用
    }
    
    return 0;
}
```

### 3.5 结构化绑定 (C++17)

```cpp
#include <iostream>
#include <map>
#include <tuple>

int main() {
    std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
    
    // 使用结构化绑定遍历 map
    for (const auto& [key, value] : m) {
        std::cout << key << " => " << value << "\n";
    }
    
    // 使用结构化绑定遍历 tuple 容器
    std::vector<std::tuple<int, double, std::string>> vec = {
        {1, 3.14, "pi"},
        {2, 2.71, "e"},
        {3, 1.41, "sqrt2"}
    };
    
    for (const auto& [i, d, s] : vec) {
        std::cout << "i = " << i << ", d = " << d << ", s = " << s << "\n";
    }
    
    return 0;
}
```

## 4. 范围初始值设定项的深入分析

### 4.1 容器作为范围初始值设定项

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <set>

int main() {
    // vector
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (const auto& element : vec) {
        std::cout << "Vector element: " << element << " ";
    }
    std::cout << "\n";
    
    // list
    std::list<int> lst = {6, 7, 8, 9, 10};
    for (const auto& element : lst) {
        std::cout << "List element: " << element << " ";
    }
    std::cout << "\n";
    
    // set
    std::set<int> st = {11, 12, 13, 14, 15};
    for (const auto& element : st) {
        std::cout << "Set element: " << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 4.2 数组作为范围初始值设定项

```cpp
#include <iostream>

int main() {
    // 静态数组
    int arr[] = {1, 2, 3, 4, 5};
    for (const auto& element : arr) {
        std::cout << "Array element: " << element << " ";
    }
    std::cout << "\n";
    
    // 栈上数组
    int stack_arr[5] = {6, 7, 8, 9, 10};
    for (const auto& element : stack_arr) {
        std::cout << "Stack array element: " << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 4.3 初始化列表作为范围初始值设定项

```cpp
#include <iostream>
#include <initializer_list>

int main() {
    // 直接使用初始化列表
    for (const auto& element : {1, 2, 3, 4, 5}) {
        std::cout << "Initializer list element: " << element << " ";
    }
    std::cout << "\n";
    
    // 使用 std::initializer_list
    std::initializer_list<int> il = {6, 7, 8, 9, 10};
    for (const auto& element : il) {
        std::cout << "Initializer list element: " << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 4.4 函数返回值作为范围初始值设定项

```cpp
#include <iostream>
#include <vector>

std::vector<int> get_numbers() {
    return {1, 2, 3, 4, 5};
}

int main() {
    // 函数返回值作为范围初始值设定项
    for (const auto& element : get_numbers()) {
        std::cout << "Function return element: " << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 4.5 临时对象的生命周期扩展

```cpp
#include <iostream>
#include <vector>

std::vector<int> create_vector() {
    return {1, 2, 3, 4, 5};
}

int main() {
    // 临时对象的生命周期扩展到循环结束
    for (const auto& element : create_vector()) {
        std::cout << "Temporary vector element: " << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

## 5. 循环控制机制详解

### 5.1 break 语句

`break` 语句用于提前终止循环：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用 break 提前退出循环
    for (const auto& element : vec) {
        if (element == 5) {
            std::cout << "Breaking at element = " << element << "\n";
            break;
        }
        std::cout << "Element: " << element << "\n";
    }
    
    return 0;
}
```

### 5.2 continue 语句

`continue` 语句用于跳过当前迭代的剩余部分：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用 continue 跳过某些迭代
    for (const auto& element : vec) {
        if (element % 2 == 0) {
            std::cout << "Skipping even number: " << element << "\n";
            continue;
        }
        std::cout << "Odd number: " << element << "\n";
    }
    
    return 0;
}
```

### 5.3 嵌套循环

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    // 嵌套基于范围的 for 循环
    for (const auto& row : matrix) {
        std::cout << "Row: ";
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }
    
    return 0;
}
```

## 6. 作用域与变量声明

### 6.1 项目声明变量的作用域

项目声明中声明的变量只在基于范围的 `for` 循环的作用域内可见：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int outer_var = 100;
    
    for (const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
        std::cout << "Outer var: " << outer_var << "\n";
        // element 在这里可见
    }
    
    // element 在这里不可见
    // std::cout << element; // 编译错误
    
    return 0;
}
```

### 6.2 初始化语句变量的作用域 (C++20)

```cpp
#include <iostream>
#include <vector>

int main() {
    // 初始化语句中声明的变量在整个循环中可见
    for (int multiplier = 2; auto& element : std::vector<int>{1, 2, 3, 4, 5}) {
        element *= multiplier;
        std::cout << "Multiplied element: " << element << " (multiplier = " << multiplier << ")\n";
    }
    
    // multiplier 在这里不可见
    // std::cout << multiplier; // 编译错误
    
    return 0;
}
```

### 6.3 循环体内声明的变量

循环体内声明的变量只在当前迭代中可见：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    for (const auto& element : vec) {
        int iteration_var = element * 2;
        std::cout << "Element: " << element << ", Iteration var: " << iteration_var << "\n";
        
        double temp_var = 3.14;
        std::cout << "Temp var: " << temp_var << "\n";
    }
    
    // iteration_var 和 temp_var 在这里不可见
    
    return 0;
}
```

## 7. 常见陷阱与高级技巧

### 7.1 重新声明陷阱

在循环体内重新声明初始化语句中引入的名称是错误的：

```cpp
#include <iostream>
#include <vector>

int main() {
    // 错误的做法：重新声明
    /*
    for (int i : {1, 2, 3}) {
        int i = 10; // 编译错误：重定义
        std::cout << i << "\n";
    }
    */
    
    // 正确的做法
    for (int i : {1, 2, 3}) {
        int j = 10;
        std::cout << "i = " << i << ", j = " << j << "\n";
    }
    
    return 0;
}
```

### 7.2 临时对象生命周期陷阱

```cpp
#include <iostream>
#include <vector>

class TemporaryContainer {
private:
    std::vector<int> data;
    
public:
    TemporaryContainer(std::initializer_list<int> il) : data(il) {
        std::cout << "TemporaryContainer constructed\n";
    }
    
    ~TemporaryContainer() {
        std::cout << "TemporaryContainer destroyed\n";
    }
    
    const std::vector<int>& get_data() const {
        return data;
    }
};

TemporaryContainer create_container() {
    return {1, 2, 3, 4, 5};
}

int main() {
    // 正确的做法：临时对象生命周期扩展
    for (const auto& element : create_container().get_data()) {
        std::cout << "Element: " << element << "\n";
    }
    
    return 0;
}
```

### 7.3 写时复制陷阱

```cpp
#include <iostream>
#include <vector>

// 模拟写时复制容器
class CopyOnWriteVector {
private:
    std::vector<int> data;
    mutable bool copied = false;
    
public:
    CopyOnWriteVector(std::initializer_list<int> il) : data(il) {}
    
    // 非const begin 会触发拷贝
    auto begin() {
        if (!copied) {
            std::cout << "Triggering copy on write\n";
            copied = true;
        }
        return data.begin();
    }
    
    auto end() {
        return data.end();
    }
    
    // const begin 不会触发拷贝
    auto begin() const {
        return data.begin();
    }
    
    auto end() const {
        return data.end();
    }
};

int main() {
    CopyOnWriteVector vec = {1, 2, 3, 4, 5};
    
    // 可能触发写时复制
    std::cout << "Non-const iteration:\n";
    for (auto& element : vec) {
        std::cout << "Element: " << element << "\n";
    }
    
    // 不会触发写时复制
    std::cout << "\nConst iteration:\n";
    for (const auto& element : std::as_const(vec)) {
        std::cout << "Element: " << element << "\n";
    }
    
    return 0;
}
```

### 7.4 高级技巧：自定义容器支持

```cpp
#include <iostream>

// 自定义容器类
class CustomContainer {
private:
    int* data;
    size_t size_;
    
public:
    CustomContainer(std::initializer_list<int> il) : size_(il.size()) {
        data = new int[size_];
        size_t i = 0;
        for (const auto& element : il) {
            data[i++] = element;
        }
    }
    
    ~CustomContainer() {
        delete[] data;
    }
    
    // 提供 begin 和 end 方法
    int* begin() { return data; }
    int* end() { return data + size_; }
    const int* begin() const { return data; }
    const int* end() const { return data + size_; }
};

int main() {
    CustomContainer container = {1, 2, 3, 4, 5};
    
    // 使用基于范围的 for 循环遍历自定义容器
    for (const auto& element : container) {
        std::cout << "Element: " << element << "\n";
    }
    
    return 0;
}
```

### 7.5 复杂数据结构遍历

```cpp
#include <iostream>
#include <map>
#include <vector>
#include <tuple>

int main() {
    // 复杂嵌套数据结构
    std::map<std::string, std::vector<std::tuple<int, double, std::string>>> complex_data = {
        {"group1", {{1, 3.14, "pi"}, {2, 2.71, "e"}}},
        {"group2", {{3, 1.41, "sqrt2"}, {4, 1.73, "sqrt3"}}}
    };
    
    // 遍历复杂嵌套数据结构
    for (const auto& [group_name, tuples] : complex_data) {
        std::cout << "Group: " << group_name << "\n";
        for (const auto& [i, d, s] : tuples) {
            std::cout << "  i = " << i << ", d = " << d << ", s = " << s << "\n";
        }
    }
    
    return 0;
}
```

## 8. 性能优化与编译器行为

### 8.1 引用 vs 值传递性能

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

class HeavyObject {
private:
    std::string data;
    
public:
    HeavyObject(const std::string& str) : data(str) {}
    
    HeavyObject(const HeavyObject& other) : data(other.data) {
        std::cout << "HeavyObject copy constructor called\n";
    }
    
    const std::string& get_data() const { return data; }
};

int main() {
    std::vector<HeavyObject> vec;
    for (int i = 0; i < 5; ++i) {
        vec.emplace_back("Heavy object " + std::to_string(i));
    }
    
    std::cout << "Value passing (copy):\n";
    auto start = std::chrono::high_resolution_clock::now();
    for (auto element : vec) { // 拷贝每个元素
        std::cout << "Element: " << element.get_data() << "\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "\nReference passing (no copy):\n";
    start = std::chrono::high_resolution_clock::now();
    for (const auto& element : vec) { // 引用传递
        std::cout << "Element: " << element.get_data() << "\n";
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Copy time: " << duration1.count() << " microseconds\n";
    std::cout << "Reference time: " << duration2.count() << " microseconds\n";
    
    return 0;
}
```

### 8.2 编译器优化

```cpp
#include <iostream>
#include <vector>

// 编译器可以优化的简单循环
void optimized_range_loop(const std::vector<int>& vec) {
    for (const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
    }
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    optimized_range_loop(vec);
    
    return 0;
}
```

### 8.3 向量化优化

```cpp
#include <iostream>
#include <vector>

void vectorized_loop() {
    std::vector<int> a(1000000, 1);
    std::vector<int> b(1000000, 2);
    std::vector<int> c(1000000);
    
    // 这种简单的循环可能被编译器向量化
    for (size_t i = 0; i < a.size(); ++i) {
        c[i] = a[i] + b[i];
    }
    
    std::cout << "First few results: ";
    for (size_t i = 0; i < 5; ++i) {
        std::cout << c[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    vectorized_loop();
    
    return 0;
}
```

## 9. 与其他语言特性的交互

### 9.1 与异常处理的交互

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>

void risky_function(int value) {
    if (value == 5) {
        throw std::runtime_error("Value is 5!");
    }
    std::cout << "Processing value: " << value << "\n";
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7};
    
    try {
        for (const auto& element : vec) {
            try {
                risky_function(element);
            } catch (const std::exception& e) {
                std::cout << "Caught exception: " << e.what() << "\n";
                continue; // 继续处理下一个元素
            }
        }
    } catch (...) {
        std::cout << "Outer exception caught\n";
    }
    
    return 0;
}
```

### 9.2 与模板的交互

```cpp
#include <iostream>
#include <type_traits>
#include <vector>

template<typename T>
class RangeProcessor {
public:
    template<typename Container>
    void process(const Container& container) {
        if constexpr (std::is_arithmetic_v<typename Container::value_type>) {
            // 对于算术类型，计算总和
            typename Container::value_type sum = typename Container::value_type{};
            for (const auto& element : container) {
                sum += element;
            }
            std::cout << "Sum: " << sum << "\n";
        } else {
            // 对于其他类型，简单遍历
            for (const auto& element : container) {
                std::cout << "Element: " << element << "\n";
            }
        }
    }
};

int main() {
    RangeProcessor<int> processor;
    
    std::vector<int> int_vec = {1, 2, 3, 4, 5};
    std::vector<std::string> string_vec = {"Hello", "World", "C++"};
    
    processor.process(int_vec);
    processor.process(string_vec);
    
    return 0;
}
```

### 9.3 与 Lambda 表达式的交互

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // Lambda 表达式中的基于范围的 for 循环
    auto processor = [](std::vector<int>& container) {
        for (auto& element : container) {
            element *= 2;
        }
    };
    
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

### 9.4 与并发的交互

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx;
std::vector<int> shared_data(1000000);

void worker_thread(const std::vector<int>& source_data, size_t start, size_t end, int thread_id) {
    for (size_t i = start; i < end && i < source_data.size(); ++i) {
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
    
    // 创建多个线程并行处理数据
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
    std::cout << "First 10 elements: ";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << shared_data[i] << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

## 10. 标准演进与未来特性

### 10.1 C++23 生命周期扩展改进

```cpp
#include <iostream>
#include <vector>

class ContainerWrapper {
private:
    std::vector<int> data;
    
public:
    ContainerWrapper(std::initializer_list<int> il) : data(il) {}
    
    const std::vector<int>& get_items() const & {
        return data;
    }
    
    std::vector<int> get_items() && {
        return std::move(data);
    }
};

ContainerWrapper create_wrapper() {
    return {1, 2, 3, 4, 5};
}

int main() {
    // C++23 改进的生命周期扩展
    for (const auto& element : create_wrapper().get_items()) {
        std::cout << "Element: " << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 10.2 编译器支持现状

```cpp
#include <iostream>
#include <vector>

// 检查编译器对不同特性的支持
#if __cplusplus >= 201103L
    #define HAS_CPP11
#endif

#if __cplusplus >= 201402L
    #define HAS_CPP14
#endif

#if __cplusplus >= 201703L
    #define HAS_CPP17
#endif

#if __cplusplus >= 202002L
    #define HAS_CPP20
#endif

#if __cplusplus >= 202302L
    #define HAS_CPP23
#endif

int main() {
#ifdef HAS_CPP11
    std::cout << "C++11 features are available\n";
    
    // 基本的基于范围的 for 循环
    std::vector<int> vec = {1, 2, 3};
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";
#endif

#ifdef HAS_CPP17
    std::cout << "C++17 features are available\n";
    
    // 结构化绑定
    std::vector<std::pair<int, std::string>> pairs = {{1, "one"}, {2, "two"}};
    for (const auto& [key, value] : pairs) {
        std::cout << key << " => " << value << "\n";
    }
#endif

#ifdef HAS_CPP20
    std::cout << "C++20 features are available\n";
    
    // 初始化语句
    for (int multiplier = 2; auto& element : std::vector<int>{1, 2, 3}) {
        element *= multiplier;
        std::cout << element << " ";
    }
    std::cout << "\n";
#endif

#ifdef HAS_CPP23
    std::cout << "C++23 features are available\n";
    
    // 别名声明作为初始化
    for (using Int = int; const auto& element : std::vector<Int>{4, 5, 6}) {
        std::cout << element << " ";
    }
    std::cout << "\n";
#endif

    return 0;
}
```

## 11. 最佳实践与代码规范

### 11.1 代码风格建议

```cpp
#include <iostream>
#include <vector>
#include <string>

// 好的实践：使用一致的命名和格式
void good_practice() {
    std::vector<std::string> strings = {"Hello", "World", "C++"};
    
    // 使用 const 引用避免不必要的拷贝
    for (const auto& str : strings) {
        std::cout << "String: " << str << "\n";
    }
    
    // 当需要修改元素时使用非const引用
    for (auto& str : strings) {
        str += " Modified";
    }
    
    // 遍历修改后的容器
    for (const auto& str : strings) {
        std::cout << "Modified string: " << str << "\n";
    }
}

// 避免的做法：不一致的风格
void avoid_practice() {
    std::vector<std::string> strings = {"Hello", "World", "C++"};
    
    // 不必要的拷贝
    for (auto str : strings) { // 拷贝每个字符串
        std::cout << "String: " << str << "\n";
    }
    
    // 混合使用不同的引用类型
    for (const auto& str : strings) {
        std::cout << "String: " << str << "\n";
    }
}

int main() {
    good_practice();
    avoid_practice();
    
    return 0;
}
```

### 11.2 引用类型选择最佳实践

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> vec = {"Hello", "World", "C++"};
    
    // 当只需要读取元素时使用 const 引用
    std::cout << "Read-only access:\n";
    for (const auto& element : vec) {
        std::cout << "Element: " << element << "\n";
    }
    
    // 当需要修改元素时使用非const引用
    std::cout << "\nModify elements:\n";
    for (auto& element : vec) {
        element += " Modified";
    }
    
    // 当元素类型很小且不需要修改时可以考虑值传递
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "\nValue passing for small types:\n";
    for (auto element : numbers) { // 对于int类型，值传递可能更高效
        std::cout << "Element: " << element << "\n";
    }
    
    return 0;
}
```

### 11.3 性能优化建议

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <string>

// 好的实践：避免不必要的拷贝
void optimized_loop() {
    std::vector<std::string> data(10000);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = "String " + std::to_string(i);
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 使用 const 引用避免拷贝
    size_t count = 0;
    for (const auto& str : data) {
        if (str.find("5") != std::string::npos) {
            count++;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Count: " << count << ", Time: " << duration.count() << " microseconds\n";
}

// 避免的做法：不必要的拷贝
void unoptimized_loop() {
    std::vector<std::string> data(10000);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = "String " + std::to_string(i);
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 值传递导致不必要的拷贝
    size_t count = 0;
    for (auto str : data) { // 每次迭代都拷贝字符串
        if (str.find("5") != std::string::npos) {
            count++;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Count: " << count << ", Time: " << duration.count() << " microseconds\n";
}

int main() {
    optimized_loop();
    unoptimized_loop();
    
    return 0;
}
```

### 11.4 错误处理最佳实践

```cpp
#include <iostream>
#include <vector>
#include <optional>
#include <stdexcept>

// 好的实践：使用现代 C++ 特性进行错误处理
std::optional<int> safe_divide(int a, int b) {
    if (b == 0) {
        return std::nullopt;
    }
    return a / b;
}

void good_error_handling() {
    std::vector<std::pair<int, int>> divisions = {{10, 2}, {15, 3}, {20, 0}, {25, 5}};
    
    for (const auto& [dividend, divisor] : divisions) {
        auto result = safe_divide(dividend, divisor);
        if (result.has_value()) {
            std::cout << dividend << " / " << divisor << " = " << *result << "\n";
        } else {
            std::cout << "Cannot divide " << dividend << " by " << divisor << "\n";
        }
    }
}

int main() {
    good_error_handling();
    
    return 0;
}
```

## 12. 标准参考

### 12.1 ISO C++ 标准条款引用

**C++11 标准 (ISO/IEC 14882:2011)**：

- §6.5.4/1: 基于范围的 for 语句的语法和语义
- §6.5.4/2: 范围初始值设定项可以是表达式或大括号括起来的初始值设定项列表
- §6.5.4/3: 项目声明的限制
- §6.5.4/4: begin 和 end 表达式的定义

**C++14 标准 (ISO/IEC 14882:2014)**：

- §5.19: 范围初始值设定项求值规则的澄清

**C++17 标准 (ISO/IEC 14882:2017)**：

- §9.5.4/1: 基于范围的 for 语句语法
- §9.5.4/2: 项目声明可以是结构化绑定声明
- §9.5.4/3: begin 和 end 表达式的语义
- §9.5.4/4: 循环控制语句的语义

**C++20 标准 (ISO/IEC 14882:2020)**：

- §8.5.4: 基于范围的 for 语句的语义
- §8.5.4/2: 初始化语句的支持

**C++23 标准 (ISO/IEC 14882:2023)**：

- §8.5.4/3: 别名声明作为初始化的支持
- §8.5.4/4: 临时对象生命周期扩展的改进

## 13. 总结

### 13.1 C++ 基于范围的 `for` 循环的特点

C++ 基于范围的 `for` 循环是 C++11 引入的重要特性，它简化了容器和数组的遍历操作，提供了更简洁、更安全的语法。

**核心特性**:

- 简洁性: 语法简单直观，减少了样板代码
- 安全性: 自动处理迭代器和边界检查
- 灵活性: 支持多种容器类型和自定义类型
- 性能: 编译器可以进行优化，避免不必要的拷贝

### 13.2 与其他循环的比较

| 特性 | 基于范围的 for | 传统 for | while | do-while |
|------|----------------|----------|-------|----------|
| 语法复杂度 | 简单 | 复杂 | 中等 | 中等 |
| 适用场景 | 容器遍历 | 复杂控制 | 一般条件循环 | 至少执行一次 |
| 安全性 | 高 | 中等 | 中等 | 中等 |
| 性能 | 优秀 | 优秀 | 优秀 | 优秀 |

### 13.3 实际应用场景

**容器遍历**：

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <set>

int main() {
    // vector 遍历
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (const auto& element : vec) {
        std::cout << "Vector element: " << element << " ";
    }
    std::cout << "\n";
    
    // map 遍历
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}, {"three", 3}};
    for (const auto& [key, value] : m) {
        std::cout << key << " => " << value << "\n";
    }
    
    // set 遍历
    std::set<int> s = {10, 20, 30, 40, 50};
    for (const auto& element : s) {
        std::cout << "Set element: " << element << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

**算法实现**：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// 查找最大元素
int find_max(const std::vector<int>& vec) {
    if (vec.empty()) return 0;
    
    int max_val = vec[0];
    for (const auto& element : vec) {
        if (element > max_val) {
            max_val = element;
        }
    }
    return max_val;
}

// 计算平均值
double calculate_average(const std::vector<int>& vec) {
    if (vec.empty()) return 0.0;
    
    long long sum = 0;
    for (const auto& element : vec) {
        sum += element;
    }
    return static_cast<double>(sum) / vec.size();
}

int main() {
    std::vector<int> numbers = {10, 5, 8, 20, 3, 15, 7};
    
    std::cout << "Numbers: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n";
    
    std::cout << "Maximum: " << find_max(numbers) << "\n";
    std::cout << "Average: " << calculate_average(numbers) << "\n";
    
    return 0;
}
```

**数据处理**：

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Student {
    std::string name;
    int score;
};

// 过滤及格学生
std::vector<Student> filter_passed_students(const std::vector<Student>& students) {
    std::vector<Student> passed;
    for (const auto& student : students) {
        if (student.score >= 60) {
            passed.push_back(student);
        }
    }
    return passed;
}

// 计算平均分
double calculate_average_score(const std::vector<Student>& students) {
    if (students.empty()) return 0.0;
    
    int total_score = 0;
    for (const auto& student : students) {
        total_score += student.score;
    }
    return static_cast<double>(total_score) / students.size();
}

int main() {
    std::vector<Student> students = {
        {"Alice", 85},
        {"Bob", 92},
        {"Charlie", 45},
        {"David", 78},
        {"Eve", 55}
    };
    
    std::cout << "All students:\n";
    for (const auto& student : students) {
        std::cout << student.name << ": " << student.score << "\n";
    }
    
    auto passed_students = filter_passed_students(students);
    std::cout << "\nPassed students:\n";
    for (const auto& student : passed_students) {
        std::cout << student.name << ": " << student.score << "\n";
    }
    
    std::cout << "\nAverage score: " << calculate_average_score(students) << "\n";
    
    return 0;
}
```

### 13.4 最佳实践总结

- 优先使用基于范围的 for 循环: 当需要遍历容器或数组时，优先考虑使用基于范围的 for 循环
- 正确选择引用类型: 根据是否需要修改元素选择合适的引用类型（const 引用、非const引用或值传递）
- 注意性能优化: 避免不必要的拷贝，使用 const 引用传递大型对象
- 保持代码清晰: 使用有意义的变量名，保持一致的代码风格
- 理解生命周期: 注意临时对象的生命周期扩展规则

### 13.5 学习建议

- 掌握基础语法: 熟练掌握基于范围的 for 循环的基本语法和语义
- 理解引用语义: 深入理解不同引用类型对性能和功能的影响
- 实践不同场景: 通过实际项目练习掌握各种使用场景
- 关注性能优化: 了解编译器优化机制，编写高效的循环代码
- 跟进标准演进: 持续关注 C++ 标准的新特性，及时学习和应用

基于范围的 `for` 循环作为 C++11 的重要特性，极大地简化了容器遍历操作，提高了代码的可读性和安全性。掌握其特性和最佳实践，能够帮助开发者写出更加优雅、高效和可维护的代码。
