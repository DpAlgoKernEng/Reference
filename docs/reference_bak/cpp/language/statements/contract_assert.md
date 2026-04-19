# C++ `contract_assert` 声明详解

  [1. 基本语法与语义](#1-基本语法与语义)  
  
  [2. 契约机制详解](#2-契约机制详解)  
  
  [3. 编译时与运行时行为](#3-编译时与运行时行为)  
  
  [4. 常见应用场景](#4-常见应用场景)  
  
  [5. 与其他断言机制的比较](#5-与其他断言机制的比较)  
  
  [6. 性能影响与编译器行为](#6-性能影响与编译器行为)  
  
  [7. 与其他语言特性的交互](#7-与其他语言特性的交互)  
  
  [8. 最佳实践与代码规范](#8-最佳实践与代码规范)  
  
  [9. 标准参考](#9-标准参考)  
  
  [10. 总结](#10-总结)

## 1. 基本语法与语义

### 1.1 语法结构详解

C++ `contract_assert` 声明的基本语法如下：

```cpp
contract_assert attr(可选) ( predicate );
```

**属性说明符 (Attributes)**:

可以在 `contract_assert` 声明前添加属性说明符：

```cpp
#include <iostream>
#include <array>
#include <cmath>
#include <concepts>

template<std::floating_point T>
constexpr auto normalize_with_attributes(std::array<T, 3> vector) noexcept {
    auto& [x, y, z]{vector};
    const auto norm{std::hypot(x, y, z)};
    
    // 使用属性说明符
    [[likely]] contract_assert(std::isfinite(norm) && norm > T(0));
    
    x /= norm, y /= norm, z /= norm;
    return vector;
}

int main() {
    std::array<double, 3> vec{3.0, 4.0, 0.0};
    auto normalized = normalize_with_attributes(vec);
    std::cout << "Normalized vector: (" << normalized[0] << ", " 
              << normalized[1] << ", " << normalized[2] << ")\n";
    return 0;
}
```

### 1.2 语义分析

**契约语义**:

`contract_assert` 是一种契约断言，用于验证函数执行过程中的内部条件。它确保指定的条件在执行过程中保持不变。

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

class VectorProcessor {
public:
    // 确保向量不为空且所有元素都是正数
    static double calculate_average(const std::vector<double>& data) {
        contract_assert(!data.empty()); // 前置条件：向量不为空
        
        double sum = 0.0;
        for (const auto& value : data) {
            contract_assert(value > 0.0); // 循环不变量：所有元素为正
            sum += value;
        }
        
        double average = sum / data.size();
        contract_assert(std::isfinite(average)); // 后置条件：结果是有限值
        
        return average;
    }
};

int main() {
    try {
        std::vector<double> valid_data{1.0, 2.0, 3.0, 4.0, 5.0};
        double avg = VectorProcessor::calculate_average(valid_data);
        std::cout << "Average: " << avg << "\n";
        
        // 下面的调用会触发 contract_assert（在调试构建中）
        /*
        std::vector<double> empty_data{};
        double avg2 = VectorProcessor::calculate_average(empty_data);
        */
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
```

**与传统断言的对比**:

```cpp
#include <iostream>
#include <cassert>

void compare_assertions(int value) {
    std::cout << "=== Traditional assert ===\n";
    assert(value > 0); // 传统断言，发布版本中可能被禁用
    
    std::cout << "=== Contract assert ===\n";
    contract_assert(value > 0); // 契约断言，调试构建中触发冲突
    
    std::cout << "Value is positive: " << value << "\n";
}

int main() {
    compare_assertions(5); // 正常情况
    
    // 下面的调用会触发断言失败
    // compare_assertions(-1);
    
    return 0;
}
```

## 2. 契约机制详解

### 2.1 契约类型与作用

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
class SortedContainer {
private:
    std::vector<T> data_;
    
    // 内部不变量：数据始终保持排序状态
    void maintain_invariant() const {
        contract_assert(std::is_sorted(data_.begin(), data_.end()));
    }
    
public:
    void insert(const T& value) {
        data_.push_back(value);
        std::sort(data_.begin(), data_.end());
        maintain_invariant(); // 确保插入后仍然保持排序
    }
    
    bool contains(const T& value) const {
        maintain_invariant(); // 检查前置条件
        auto it = std::lower_bound(data_.begin(), data_.end(), value);
        bool found = (it != data_.end() && *it == value);
        contract_assert(found == std::binary_search(data_.begin(), data_.end(), value));
        return found;
    }
    
    const std::vector<T>& get_data() const {
        maintain_invariant(); // 确保返回的数据满足不变量
        return data_;
    }
};

int main() {
    SortedContainer<int> container;
    container.insert(3);
    container.insert(1);
    container.insert(4);
    container.insert(2);
    
    std::cout << "Container contains 3: " << std::boolalpha 
              << container.contains(3) << "\n";
    
    const auto& data = container.get_data();
    std::cout << "Sorted data: ";
    for (const auto& value : data) {
        std::cout << value << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 2.2 前置条件与后置条件

```cpp
#include <iostream>
#include <string>
#include <algorithm>

class StringProcessor {
public:
    // 前置条件：输入字符串不为空
    // 后置条件：返回字符串长度等于输入字符串长度
    static std::string reverse_string(std::string input) 
        pre(!input.empty()) 
        post(/* result.length() == input.length() */) {
        
        contract_assert(!input.empty()); // 前置条件检查
        
        std::reverse(input.begin(), input.end());
        
        contract_assert(input.length() > 0); // 后置条件检查
        return input;
    }
    
    // 前置条件：两个字符串都不为空
    // 后置条件：返回值不为空
    static std::string concatenate_strings(const std::string& str1, 
                                         const std::string& str2)
        pre(!str1.empty() && !str2.empty())
        post(/* !result.empty() */) {
        
        contract_assert(!str1.empty() && !str2.empty()); // 前置条件
        
        std::string result = str1 + str2;
        
        contract_assert(!result.empty()); // 后置条件
        return result;
    }
};

int main() {
    try {
        std::string reversed = StringProcessor::reverse_string("Hello");
        std::cout << "Reversed: " << reversed << "\n";
        
        std::string concatenated = StringProcessor::concatenate_strings("Hello", "World");
        std::cout << "Concatenated: " << concatenated << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
```

### 2.3 循环不变量

```cpp
#include <iostream>
#include <vector>
#include <numeric>

class MathProcessor {
public:
    // 计算向量元素的累积和，维护循环不变量
    static std::vector<int> cumulative_sum(const std::vector<int>& input) {
        contract_assert(!input.empty()); // 前置条件
        
        std::vector<int> result;
        result.reserve(input.size());
        
        int cumulative = 0;
        for (size_t i = 0; i < input.size(); ++i) {
            // 循环不变量：cumulative 等于前 i 个元素的和
            contract_assert(cumulative == std::accumulate(input.begin(), 
                                                        input.begin() + i, 0));
            
            cumulative += input[i];
            result.push_back(cumulative);
            
            // 循环后条件：result[i] 等于前 i+1 个元素的和
            contract_assert(result[i] == std::accumulate(input.begin(), 
                                                       input.begin() + i + 1, 0));
        }
        
        contract_assert(result.size() == input.size()); // 后置条件
        return result;
    }
};

int main() {
    std::vector<int> input{1, 2, 3, 4, 5};
    auto result = MathProcessor::cumulative_sum(input);
    
    std::cout << "Input: ";
    for (const auto& value : input) {
        std::cout << value << " ";
    }
    std::cout << "\n";
    
    std::cout << "Cumulative sum: ";
    for (const auto& value : result) {
        std::cout << value << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

## 3. 编译时与运行时行为

### 3.1 调试构建与发布构建

```cpp
#include <iostream>
#include <vector>

class DebugReleaseExample {
public:
    static void process_data(const std::vector<int>& data) {
        std::cout << "Processing " << data.size() << " elements\n";
        
        // 在调试构建中会检查，发布构建中可能被忽略
        contract_assert(!data.empty());
        
        for (size_t i = 0; i < data.size(); ++i) {
            contract_assert(i < data.size()); // 边界检查
            
            // 模拟处理
            int processed_value = data[i] * 2;
            contract_assert(processed_value >= 0 || data[i] < 0); // 逻辑检查
        }
        
        std::cout << "Processing completed\n";
    }
};

int main() {
    // 正常情况
    std::vector<int> valid_data{1, 2, 3, 4, 5};
    DebugReleaseExample::process_data(valid_data);
    
    // 边界情况（在调试构建中会触发 contract_assert）
    std::vector<int> empty_data{};
    DebugReleaseExample::process_data(empty_data);
    
    return 0;
}
```

### 3.2 异常处理与契约冲突

```cpp
#include <iostream>
#include <stdexcept>

class ExceptionHandlingExample {
public:
    static int divide_with_contracts(int numerator, int denominator) {
        // 契约断言：分母不能为零
        contract_assert(denominator != 0);
        
        // 契约断言：检查除法是否会溢出
        contract_assert(numerator != INT_MIN || denominator != -1);
        
        return numerator / denominator;
    }
    
    static double safe_sqrt(double value) {
        // 契约断言：输入必须非负
        contract_assert(value >= 0.0);
        
        // 契约断言：结果必须是有限值
        double result = std::sqrt(value);
        contract_assert(std::isfinite(result));
        
        return result;
    }
};

int main() {
    try {
        // 正常情况
        int result1 = ExceptionHandlingExample::divide_with_contracts(10, 2);
        std::cout << "10 / 2 = " << result1 << "\n";
        
        double result2 = ExceptionHandlingExample::safe_sqrt(16.0);
        std::cout << "sqrt(16) = " << result2 << "\n";
        
        // 下面的调用会触发 contract_assert（调试构建中）
        /*
        int result3 = ExceptionHandlingExample::divide_with_contracts(10, 0);
        double result4 = ExceptionHandlingExample::safe_sqrt(-1.0);
        */
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }
    
    return 0;
}
```

### 3.3 编译器指令控制

```cpp
#include <iostream>
#include <vector>

// 可以通过编译器指令控制契约行为
class CompilerControlExample {
public:
    static void process_with_level_control(const std::vector<int>& data) {
        // 不同级别的契约断言
        contract_assert(!data.empty()); // 基本级别
        
        for (const auto& value : data) {
            contract_assert(value >= -1000 && value <= 1000); // 详细级别
        }
        
        // 复杂的后置条件检查
        int sum = 0;
        for (const auto& value : data) {
            sum += value;
        }
        contract_assert(sum >= -10000 && sum <= 10000); // 结果验证
    }
};

int main() {
    std::vector<int> data{1, 2, 3, 4, 5};
    CompilerControlExample::process_with_level_control(data);
    
    return 0;
}
```

## 4. 常见应用场景

### 4.1 数学计算验证

```cpp
#include <iostream>
#include <array>
#include <cmath>
#include <concepts>

template<std::floating_point T>
class Vector3D {
private:
    std::array<T, 3> components_;
    
public:
    Vector3D(T x, T y, T z) : components_{x, y, z} {}
    
    // 向量归一化
    Vector3D<T> normalize() const {
        T norm = std::hypot(components_[0], components_[1], components_[2]);
        
        // 契约断言：确保向量不是零向量
        contract_assert(std::isfinite(norm) && norm > T(0));
        
        return Vector3D<T>(components_[0] / norm,
                          components_[1] / norm,
                          components_[2] / norm);
    }
    
    // 向量点积
    T dot(const Vector3D<T>& other) const {
        // 契约断言：确保两个向量都有效
        contract_assert(std::isfinite(components_[0]) && 
                       std::isfinite(components_[1]) && 
                       std::isfinite(components_[2]));
        contract_assert(std::isfinite(other.components_[0]) && 
                       std::isfinite(other.components_[1]) && 
                       std::isfinite(other.components_[2]));
        
        T result = components_[0] * other.components_[0] +
                   components_[1] * other.components_[1] +
                   components_[2] * other.components_[2];
        
        contract_assert(std::isfinite(result));
        return result;
    }
    
    void print() const {
        std::cout << "(" << components_[0] << ", " 
                  << components_[1] << ", " 
                  << components_[2] << ")";
    }
};

int main() {
    Vector3D<double> v1(3.0, 4.0, 0.0);
    Vector3D<double> v2(1.0, 0.0, 0.0);
    
    std::cout << "Vector 1: ";
    v1.print();
    std::cout << "\n";
    
    std::cout << "Vector 2: ";
    v2.print();
    std::cout << "\n";
    
    auto normalized = v1.normalize();
    std::cout << "Normalized v1: ";
    normalized.print();
    std::cout << "\n";
    
    double dot_product = v1.dot(v2);
    std::cout << "Dot product: " << dot_product << "\n";
    
    return 0;
}
```

### 4.2 数据结构完整性

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
class SortedVector {
private:
    std::vector<T> data_;
    
    // 维护排序不变量
    void check_invariant() const {
        contract_assert(std::is_sorted(data_.begin(), data_.end()));
    }
    
public:
    void insert(const T& value) {
        data_.push_back(value);
        std::sort(data_.begin(), data_.end());
        check_invariant(); // 确保插入后仍然有序
    }
    
    bool find(const T& value) const {
        check_invariant(); // 前置条件检查
        return std::binary_search(data_.begin(), data_.end(), value);
    }
    
    void remove(const T& value) {
        check_invariant(); // 前置条件检查
        
        auto it = std::lower_bound(data_.begin(), data_.end(), value);
        if (it != data_.end() && *it == value) {
            data_.erase(it);
        }
        
        check_invariant(); // 后置条件检查
    }
    
    size_t size() const {
        check_invariant(); // 确保容器状态有效
        return data_.size();
    }
    
    void print() const {
        check_invariant();
        std::cout << "[";
        for (size_t i = 0; i < data_.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data_[i];
        }
        std::cout << "]\n";
    }
};

int main() {
    SortedVector<int> sorted_vec;
    
    sorted_vec.insert(5);
    sorted_vec.insert(2);
    sorted_vec.insert(8);
    sorted_vec.insert(1);
    sorted_vec.insert(9);
    
    std::cout << "Sorted vector: ";
    sorted_vec.print();
    
    std::cout << "Contains 5: " << std::boolalpha << sorted_vec.find(5) << "\n";
    std::cout << "Contains 3: " << std::boolalpha << sorted_vec.find(3) << "\n";
    
    sorted_vec.remove(5);
    std::cout << "After removing 5: ";
    sorted_vec.print();
    
    return 0;
}
```

### 4.3 状态机验证

```cpp
#include <iostream>
#include <string>

class StateMachine {
private:
    enum class State { IDLE, RUNNING, PAUSED, ERROR };
    State current_state_ = State::IDLE;
    
    // 状态不变量检查
    void check_state_invariant() const {
        contract_assert(current_state_ != State::ERROR || 
                       current_state_ == State::IDLE ||
                       current_state_ == State::RUNNING ||
                       current_state_ == State::PAUSED);
    }
    
public:
    void start() {
        check_state_invariant();
        contract_assert(current_state_ == State::IDLE); // 前置条件
        
        current_state_ = State::RUNNING;
        
        check_state_invariant();
        contract_assert(current_state_ == State::RUNNING); // 后置条件
        std::cout << "State machine started\n";
    }
    
    void pause() {
        check_state_invariant();
        contract_assert(current_state_ == State::RUNNING); // 前置条件
        
        current_state_ = State::PAUSED;
        
        check_state_invariant();
        contract_assert(current_state_ == State::PAUSED); // 后置条件
        std::cout << "State machine paused\n";
    }
    
    void resume() {
        check_state_invariant();
        contract_assert(current_state_ == State::PAUSED); // 前置条件
        
        current_state_ = State::RUNNING;
        
        check_state_invariant();
        contract_assert(current_state_ == State::RUNNING); // 后置条件
        std::cout << "State machine resumed\n";
    }
    
    void stop() {
        check_state_invariant();
        contract_assert(current_state_ != State::IDLE); // 前置条件
        
        current_state_ = State::IDLE;
        
        check_state_invariant();
        contract_assert(current_state_ == State::IDLE); // 后置条件
        std::cout << "State machine stopped\n";
    }
    
    std::string get_state() const {
        check_state_invariant();
        switch (current_state_) {
            case State::IDLE: return "IDLE";
            case State::RUNNING: return "RUNNING";
            case State::PAUSED: return "PAUSED";
            case State::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }
};

int main() {
    StateMachine sm;
    
    std::cout << "Initial state: " << sm.get_state() << "\n";
    
    sm.start();
    std::cout << "Current state: " << sm.get_state() << "\n";
    
    sm.pause();
    std::cout << "Current state: " << sm.get_state() << "\n";
    
    sm.resume();
    std::cout << "Current state: " << sm.get_state() << "\n";
    
    sm.stop();
    std::cout << "Current state: " << sm.get_state() << "\n";
    
    return 0;
}
```

## 5. 与其他断言机制的比较

### 5.1 与 static_assert 的比较

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
class ComparisonExample {
public:
    void process_value(T value) {
        // static_assert: 编译时检查类型特性
        static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");
        
        // contract_assert: 运行时检查值的有效性
        contract_assert(value >= T(0)); // 运行时检查值非负
        
        std::cout << "Processing value: " << value << "\n";
    }
};

int main() {
    ComparisonExample<int> int_processor;
    int_processor.process_value(5); // 正常
    
    ComparisonExample<double> double_processor;
    double_processor.process_value(3.14); // 正常
    
    // 下面这行会在编译时失败
    // ComparisonExample<std::string> string_processor;
    
    return 0;
}
```

### 5.2 与 assert 的比较

```cpp
#include <iostream>
#include <cassert>

class AssertComparison {
public:
    static void compare_assertions(int value) {
        // 传统 assert：可能在发布版本中被禁用
        assert(value > 0);
        std::cout << "Traditional assert passed\n";
        
        // contract_assert：调试构建中触发冲突，发布构建中可能忽略
        contract_assert(value > 0);
        std::cout << "Contract assert passed\n";
        
        std::cout << "Value: " << value << "\n";
    }
};

int main() {
    AssertComparison::compare_assertions(5); // 正常情况
    
    // 下面的调用会触发断言失败（调试构建中）
    // AssertComparison::compare_assertions(-1);
    
    return 0;
}
```

### 5.3 与异常处理的比较

```cpp
#include <iostream>
#include <stdexcept>

class ErrorHandlingComparison {
public:
    // 使用 contract_assert 的方式
    static int divide_contract(int a, int b) {
        contract_assert(b != 0); // 契约断言
        return a / b;
    }
    
    // 使用异常的方式
    static int divide_exception(int a, int b) {
        if (b == 0) {
            throw std::invalid_argument("Division by zero");
        }
        return a / b;
    }
    
    // 使用返回值的方式
    static std::optional<int> divide_optional(int a, int b) {
        if (b == 0) {
            return std::nullopt;
        }
        return a / b;
    }
};

int main() {
    try {
        // 契约断言方式
        int result1 = ErrorHandlingComparison::divide_contract(10, 2);
        std::cout << "Contract result: " << result1 << "\n";
        
        // 异常方式
        int result2 = ErrorHandlingComparison::divide_exception(10, 2);
        std::cout << "Exception result: " << result2 << "\n";
        
        // 可选值方式
        auto result3 = ErrorHandlingComparison::divide_optional(10, 2);
        if (result3) {
            std::cout << "Optional result: " << *result3 << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }
    
    return 0;
}
```

## 6. 性能影响与编译器行为

### 6.1 性能基准测试

```cpp
#include <iostream>
#include <chrono>
#include <vector>

class PerformanceTest {
private:
    std::vector<int> data_;
    
public:
    PerformanceTest(size_t size) : data_(size, 42) {}
    
    // 带 contract_assert 的版本
    void process_with_contracts() {
        for (size_t i = 0; i < data_.size(); ++i) {
            contract_assert(i < data_.size()); // 边界检查
            contract_assert(data_[i] > 0);     // 值检查
            
            // 模拟处理
            data_[i] *= 2;
        }
    }
    
    // 不带 contract_assert 的版本
    void process_without_contracts() {
        for (size_t i = 0; i < data_.size(); ++i) {
            // 模拟处理
            data_[i] *= 2;
        }
    }
};

void performance_benchmark() {
    const size_t data_size = 1000000;
    const int iterations = 100;
    
    PerformanceTest test_with_contracts(data_size);
    PerformanceTest test_without_contracts(data_size);
    
    // 测试带契约的版本
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        test_with_contracts.process_with_contracts();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_with = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试不带契约的版本
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        test_without_contracts.process_without_contracts();
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_without = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "With contracts: " << duration_with.count() << " microseconds\n";
    std::cout << "Without contracts: " << duration_without.count() << " microseconds\n";
    std::cout << "Overhead: " << (duration_with.count() - duration_without.count()) * 100.0 / duration_without.count() 
              << "%\n";
}

int main() {
    performance_benchmark();
    return 0;
}
```

### 6.2 编译器优化

```cpp
#include <iostream>
#include <vector>

class OptimizationExample {
public:
    // 编译器可能优化掉的简单契约
    static int simple_contract(int value) {
        contract_assert(value > 0); // 简单检查
        return value * 2;
    }
    
    // 复杂契约可能影响优化
    static int complex_contract(const std::vector<int>& data, size_t index) {
        contract_assert(!data.empty());                    // 复杂检查1
        contract_assert(index < data.size());              // 复杂检查2
        contract_assert(data[index] >= -1000 && data[index] <= 1000); // 复杂检查3
        
        return data[index] * 2;
    }
};

int main() {
    int result1 = OptimizationExample::simple_contract(5);
    std::cout << "Simple contract result: " << result1 << "\n";
    
    std::vector<int> data{1, 2, 3, 4, 5};
    int result2 = OptimizationExample::complex_contract(data, 2);
    std::cout << "Complex contract result: " << result2 << "\n";
    
    return 0;
}
```

### 6.3 发布构建优化

```cpp
#include <iostream>
#include <vector>

// 在发布构建中可能被完全忽略的契约
class ReleaseOptimization {
public:
    static void process_data_release(const std::vector<int>& data) {
        // 在发布构建中这些契约可能被完全忽略
        contract_assert(!data.empty());
        
        for (size_t i = 0; i < data.size(); ++i) {
            contract_assert(i < data.size());
            // 处理数据...
        }
        
        std::cout << "Processing completed\n";
    }
    
    // 与发布构建优化对比的版本
    static void process_data_optimized(const std::vector<int>& data) {
        if (data.empty()) return; // 手动检查，可能被优化
        
        for (size_t i = 0; i < data.size(); ++i) {
            // 处理数据...
        }
        
        std::cout << "Processing completed\n";
    }
};

int main() {
    std::vector<int> data{1, 2, 3, 4, 5};
    
    ReleaseOptimization::process_data_release(data);
    ReleaseOptimization::process_data_optimized(data);
    
    return 0;
}
```

## 7. 与其他语言特性的交互

### 7.1 与模板的交互

```cpp
#include <iostream>
#include <type_traits>
#include <concepts>

// 使用概念和契约断言的模板
template<std::integral T>
class IntegralProcessor {
public:
    T process_value(T value) {
        // 契约断言：确保值在安全范围内
        contract_assert(value >= std::numeric_limits<T>::min() / 2);
        contract_assert(value <= std::numeric_limits<T>::max() / 2);
        
        T result = value * 2;
        
        // 契约断言：确保结果有效
        contract_assert(result >= std::numeric_limits<T>::min());
        contract_assert(result <= std::numeric_limits<T>::max());
        
        return result;
    }
};

// 使用概念约束的模板
template<std::floating_point T>
class FloatingProcessor {
public:
    T process_value(T value) {
        // 契约断言：确保输入有效
        contract_assert(std::isfinite(value));
        
        T result = std::sqrt(value * value); // 应该等于 abs(value)
        
        // 契约断言：确保结果有效
        contract_assert(std::isfinite(result));
        contract_assert(result >= 0);
        
        return result;
    }
};

int main() {
    IntegralProcessor<int> int_processor;
    int result1 = int_processor.process_value(100);
    std::cout << "Int result: " << result1 << "\n";
    
    FloatingProcessor<double> float_processor;
    double result2 = float_processor.process_value(25.0);
    std::cout << "Float result: " << result2 << "\n";
    
    return 0;
}
```

### 7.2 与 Lambda 表达式的交互

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class LambdaContractExample {
public:
    static void process_with_lambda_contracts() {
        std::vector<int> data{1, 2, 3, 4, 5};
        
        // 在 Lambda 中使用 contract_assert
        std::for_each(data.begin(), data.end(), [](int value) {
            contract_assert(value > 0); // 确保值为正
            
            int processed = value * 2;
            contract_assert(processed > 0); // 确保处理结果为正
            
            std::cout << "Processed: " << processed << "\n";
        });
    }
    
    // 返回带有契约检查的 Lambda
    static auto create_validator() {
        return [](int value) -> bool {
            contract_assert(value != 0); // 契约断言
            return value > 0;
        };
    }
};

int main() {
    LambdaContractExample::process_with_lambda_contracts();
    
    auto validator = LambdaContractExample::create_validator();
    std::cout << "Validator result for 5: " << std::boolalpha << validator(5) << "\n";
    std::cout << "Validator result for -3: " << std::boolalpha << validator(-3) << "\n";
    
    return 0;
}
```

### 7.3 与异常处理的交互

```cpp
#include <iostream>
#include <stdexcept>

class ExceptionContractInteraction {
public:
    static int risky_operation(int value) {
        try {
            // 契约断言：前置条件
            contract_assert(value != 0);
            
            if (value < 0) {
                throw std::invalid_argument("Negative value not allowed");
            }
            
            int result = 100 / value;
            
            // 契约断言：后置条件
            contract_assert(result > 0);
            
            return result;
        } catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << "\n";
            
            // 在异常处理中也可以使用契约断言
            contract_assert(std::string(e.what()).length() > 0);
            
            throw; // 重新抛出异常
        }
    }
};

int main() {
    try {
        int result1 = ExceptionContractInteraction::risky_operation(5);
        std::cout << "Result 1: " << result1 << "\n";
        
        // 下面的调用会抛出异常
        int result2 = ExceptionContractInteraction::risky_operation(-1);
    } catch (const std::exception& e) {
        std::cout << "Final exception: " << e.what() << "\n";
    }
    
    return 0;
}
```

### 7.4 与并发的交互

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

class ConcurrentContractExample {
private:
    std::atomic<int> counter_{0};
    
public:
    void increment_counter() {
        int current = counter_.load();
        
        // 契约断言：确保计数器值有效
        contract_assert(current >= 0);
        
        counter_.store(current + 1);
        
        int new_value = counter_.load();
        contract_assert(new_value > current); // 后置条件
    }
    
    int get_counter() const {
        int value = counter_.load();
        contract_assert(value >= 0); // 确保值非负
        return value;
    }
};

void worker_thread(ConcurrentContractExample& example, int thread_id) {
    for (int i = 0; i < 1000; ++i) {
        example.increment_counter();
        
        // 线程安全的契约检查
        contract_assert(example.get_counter() >= 0);
    }
    
    std::cout << "Thread " << thread_id << " completed\n";
}

int main() {
    ConcurrentContractExample example;
    std::vector<std::thread> threads;
    
    // 创建多个工作线程
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(worker_thread, std::ref(example), i);
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final counter value: " << example.get_counter() << "\n";
    
    return 0;
}
```

## 8. 最佳实践与代码规范

### 8.1 契约使用原则

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class ContractBestPractices {
public:
    // 好的实践：清晰的前置条件和后置条件
    static std::vector<int> filter_positive(const std::vector<int>& input)
        pre(!input.empty())  // 前置条件
        post(/* result.size() <= input.size() */) { // 后置条件
        
        contract_assert(!input.empty()); // 明确的前置条件检查
        
        std::vector<int> result;
        result.reserve(input.size());
        
        for (const auto& value : input) {
            contract_assert(value != INT_MIN); // 循环不变量
            
            if (value > 0) {
                result.push_back(value);
            }
        }
        
        contract_assert(result.size() <= input.size()); // 明确的后置条件检查
        return result;
    }
    
    // 避免的做法：过于复杂的契约
    /*
    static int complex_contract_example(int a, int b, int c, int d) {
        // 过于复杂的契约检查，影响可读性
        contract_assert(a > 0 && b > 0 && c > 0 && d > 0 && 
                       a + b < 1000 && b + c < 1000 && 
                       c + d < 1000 && d + a < 1000 &&
                       a * b < 1000000 && c * d < 1000000);
        return a + b + c + d;
    }
    */
};

int main() {
    std::vector<int> input{-2, -1, 0, 1, 2, 3, 4, 5};
    auto result = ContractBestPractices::filter_positive(input);
    
    std::cout << "Filtered positive values: ";
    for (const auto& value : result) {
        std::cout << value << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 8.2 错误处理策略

```cpp
#include <iostream>
#include <optional>
#include <stdexcept>

class ErrorHandlingStrategy {
public:
    // 策略1：使用契约断言进行调试检查
    static int divide_with_contracts(int numerator, int denominator) {
        contract_assert(denominator != 0); // 调试时检查
        
        // 在发布版本中可能需要其他错误处理
        if (denominator == 0) {
            throw std::invalid_argument("Division by zero");
        }
        
        return numerator / denominator;
    }
    
    // 策略2：结合可选值返回
    static std::optional<int> safe_divide(int numerator, int denominator) {
        contract_assert(denominator != 0); // 调试检查
        
        if (denominator == 0) {
            return std::nullopt; // 发布版本的安全返回
        }
        
        return numerator / denominator;
    }
    
    // 策略3：详细的错误信息
    static int checked_operation(int value) {
        contract_assert(value >= -1000 && value <= 1000); // 范围检查
        
        if (value < -1000 || value > 1000) {
            throw std::out_of_range("Value out of acceptable range");
        }
        
        return value * 2;
    }
};

int main() {
    try {
        // 使用契约断言的除法
        int result1 = ErrorHandlingStrategy::divide_with_contracts(10, 2);
        std::cout << "Division result: " << result1 << "\n";
        
        // 使用可选值的安全除法
        auto result2 = ErrorHandlingStrategy::safe_divide(10, 2);
        if (result2) {
            std::cout << "Safe division result: " << *result2 << "\n";
        }
        
        // 检查操作
        int result3 = ErrorHandlingStrategy::checked_operation(500);
        std::cout << "Checked operation result: " << result3 << "\n";
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
```

### 8.3 性能考虑

```cpp
#include <iostream>
#include <vector>

class PerformanceConsiderations {
public:
    // 好的做法：只在关键路径上使用必要的契约
    static void process_critical_path(std::vector<int>& data) {
        contract_assert(!data.empty()); // 必要的前置检查
        
        // 关键处理逻辑
        for (auto& value : data) {
            contract_assert(value != INT_MIN); // 必要的循环不变量
            value *= 2;
        }
        
        contract_assert(!data.empty()); // 必要的后置检查
    }
    
    // 避免的做法：过度使用契约影响性能
    /*
    static void process_with_excessive_contracts(std::vector<int>& data) {
        contract_assert(!data.empty());
        contract_assert(data.size() > 0);
        contract_assert(data.capacity() >= data.size());
        
        for (size_t i = 0; i < data.size(); ++i) {
            contract_assert(i < data.size());
            contract_assert(data[i] != INT_MAX);
            contract_assert(data[i] != INT_MIN);
            contract_assert(data[i] >= -1000000);
            contract_assert(data[i] <= 1000000);
            
            data[i] *= 2;
            
            contract_assert(data[i] >= -2000000);
            contract_assert(data[i] <= 2000000);
        }
        
        contract_assert(!data.empty());
        contract_assert(data.size() > 0);
    }
    */
};

int main() {
    std::vector<int> data{1, 2, 3, 4, 5};
    PerformanceConsiderations::process_critical_path(data);
    
    std::cout << "Processed data: ";
    for (const auto& value : data) {
        std::cout << value << " ";
    }
    std::cout << "\n";
    
    return 0;
}
```

### 8.4 现代 C++ 集成

```cpp
#include <iostream>
#include <expected>
#include <string>

// 使用 std::expected 结合契约断言（C++23）
/*
template<typename T, typename E>
using expected = std::expected<T, E>;

class ModernCppIntegration {
public:
    static expected<int, std::string> safe_operation(int value) {
        contract_assert(value != 0); // 调试时的契约检查
        
        if (value == 0) {
            return std::unexpected<std::string>("Zero value not allowed");
        }
        
        if (value < 0) {
            return std::unexpected<std::string>("Negative value not allowed");
        }
        
        return value * 2;
    }
};
*/

class AlternativeIntegration {
public:
    static std::optional<int> process_with_optional(int value) {
        contract_assert(value >= 0); // 调试检查
        
        if (value < 0) {
            return std::nullopt;
        }
        
        int result = value * 2;
        contract_assert(result >= 0); // 结果验证
        
        return result;
    }
};

int main() {
    /*
    auto result1 = ModernCppIntegration::safe_operation(5);
    if (result1) {
        std::cout << "Result: " << *result1 << "\n";
    } else {
        std::cout << "Error: " << result1.error() << "\n";
    }
    */
    
    auto result2 = AlternativeIntegration::process_with_optional(5);
    if (result2) {
        std::cout << "Optional result: " << *result2 << "\n";
    } else {
        std::cout << "Operation failed\n";
    }
    
    return 0;
}
```

## 9. 标准参考

### 9.1 ISO C++ 标准条款引用

**C++26 标准 (ISO/IEC 14882:2026)**：

- §8.(7+c): 断言语句 [stmt.contract.assert]
- §8.(7+c)/1: contract_assert 语句的语法和语义
- §8.(7+c)/2: 契约断言在调试构建中的行为
- §8.(7+c)/3: 契约断言在发布构建中的可选忽略

**相关标准文档**：

- P2900R14: Contracts for C++ (提案文档)
- ISO/IEC 14882:2026: Programming Languages - C++

## 10. 总结

### 10.1 C++ `contract_assert` 声明的特点

C++ `contract_assert` 声明是一个现代化的契约编程工具，用于在函数执行过程中验证内部条件。

**核心特性**:

- 运行时验证: 在调试构建中验证条件，发布构建中可能被忽略
- 契约语义: 支持前置条件、后置条件和不变量检查
- 性能友好: 在发布构建中可以被优化掉以提高性能
- 调试支持: 在调试构建中提供详细的错误信息和程序终止

### 10.2 与其他断言机制的比较

| 特性 | contract_assert | static_assert | assert | 异常处理 |
|------|----------------|---------------|--------|----------|
| 检查时机 | 运行时 | 编译时 | 运行时 | 运行时 |
| 发布构建 | 可能忽略 | 始终生效 | 可能禁用 | 始终生效 |
| 错误处理 | 程序终止 | 编译错误 | 程序终止 | 异常抛出 |
| 使用场景 | 调试验证 | 类型检查 | 调试验证 | 错误恢复 |

### 10.3 实际应用场景

**数学库验证**：

```cpp
#include <iostream>
#include <array>
#include <cmath>
#include <concepts>

template<std::floating_point T>
constexpr auto normalize_vector(std::array<T, 3> vector) noexcept
    pre(/* is_normalizable(vector) */)
    post(/* vector: is_normalized(vector) */) {
    auto& [x, y, z]{vector};
    const auto norm{std::hypot(x, y, z)};
    
    // debug check for normalization safety
    contract_assert(std::isfinite(norm) && norm > T(0));
    
    x /= norm, y /= norm, z /= norm;
    
    return vector;
}

int main() {
    std::array<double, 3> vec{3.0, 4.0, 0.0};
    auto normalized = normalize_vector(vec);
    std::cout << "Normalized vector: (" << normalized[0] << ", " 
              << normalized[1] << ", " << normalized[2] << ")\n";
    return 0;
}
```

**数据结构完整性**：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
class SafeContainer {
private:
    std::vector<T> data_;
    
    void maintain_sorted_invariant() const {
        contract_assert(std::is_sorted(data_.begin(), data_.end()));
    }
    
public:
    void insert_sorted(const T& value) {
        data_.push_back(value);
        std::sort(data_.begin(), data_.end());
        maintain_sorted_invariant();
    }
    
    bool contains(const T& value) const {
        maintain_sorted_invariant();
        return std::binary_search(data_.begin(), data_.end(), value);
    }
    
    size_t size() const {
        maintain_sorted_invariant();
        return data_.size();
    }
};

int main() {
    SafeContainer<int> container;
    container.insert_sorted(3);
    container.insert_sorted(1);
    container.insert_sorted(4);
    
    std::cout << "Container size: " << container.size() << "\n";
    std::cout << "Contains 3: " << std::boolalpha << container.contains(3) << "\n";
    
    return 0;
}
```

### 10.4 最佳实践总结

- 适度使用: 只在关键路径和重要条件上使用 contract_assert
- 清晰语义: 确保契约条件的语义清晰明确
- 性能考虑: 避免在性能敏感的代码中过度使用
- 调试价值: 利用 contract_assert 提供的调试信息
- 现代集成: 结合 std::optional、std::expected 等现代 C++ 特性

### 10.5 学习建议

- 理解概念: 深入理解契约编程的概念和价值
- 实践应用: 在实际项目中尝试使用 contract_assert
- 性能测试: 了解不同使用场景下的性能影响
- 工具支持: 关注编译器和调试工具对契约的支持
- 团队规范: 在团队中建立一致的契约使用规范

`contract_assert` 作为 C++26 引入的现代化契约编程工具，为开发者提供了强大的调试和验证能力。正确使用这一特性，能够显著提高代码的可靠性和可维护性，同时在发布版本中保持良好的性能表现。
