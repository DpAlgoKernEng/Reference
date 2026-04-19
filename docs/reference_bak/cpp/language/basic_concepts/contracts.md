# C++ 合约契约详解

  [1. 基础概念](#1-基础概念)
  
  [2. 合约断言](#2-合约断言)
  
  [3. 函数合约规范](#3-函数合约规范)
  
  [4. 评估语义](#4-评估语义)
  
  [5. 契约违规处理](#5-契约违规处理)
  
  [6. 契约违规处理器](#6-契约违规处理器)
  
  [7. 按顺序评估](#7-按顺序评估)
  
  [8. 实践示例](#8-实践示例)
  
  [9. 标准参考](#9-标准参考)
  
  [10. 总结](#10-总结)

## 1. 基础概念

### 1.1 合约契约定义

合约契约是C++26引入的重要特性，允许程序员指定程序在执行过程中某些时刻应保持的状态属性。合约提供了一种声明性的方式来表达程序的预期行为和约束条件。

```cpp
#include <iostream>
#include <vector>

void contracts_basics() {
    std::cout << "Contracts Basics:" << std::endl;
    
    // 传统断言方式
    std::cout << "  === Traditional Assertions ===" << std::endl;
    std::cout << "    - 使用 assert() 宏" << std::endl;
    std::cout << "    - 运行时检查" << std::endl;
    std::cout << "    - 可以被禁用" << std::endl;
    std::cout << "    - 缺乏语义表达能力" << std::endl;
    
    // C++合约契约
    std::cout << "  === C++ Contracts ===" << std::endl;
    std::cout << "    - 语言级别的契约支持" << std::endl;
    std::cout << "    - 前置条件、后置条件、断言" << std::endl;
    std::cout << "    - 编译时和运行时检查" << std::endl;
    std::cout << "    - 更丰富的语义表达" << std::endl;
    std::cout << "    - 可配置的评估策略" << std::endl;
    
    // 合约的优势
    std::cout << "  === Contracts Advantages ===" << std::endl;
    std::cout << "    - 提高代码可读性和文档化" << std::endl;
    std::cout << "    - 更好的错误检测和调试" << std::endl;
    std::cout << "    - 编译时优化机会" << std::endl;
    std::cout << "    - 形式化验证支持" << std::endl;
    std::cout << "    - 统一的契约表达方式" << std::endl;
}
```

### 1.2 合约类型

C++合约主要分为三种类型：

```cpp
#include <iostream>

void contract_types() {
    std::cout << "Contract Types:" << std::endl;
    
    // 1. 前置条件 (Preconditions)
    std::cout << "  === Preconditions (pre) ===" << std::endl;
    std::cout << "    - 函数调用前必须满足的条件" << std::endl;
    std::cout << "    - 调用者的责任" << std::endl;
    std::cout << "    - 例如：参数有效性检查" << std::endl;
    
    // 2. 后置条件 (Postconditions)
    std::cout << "  === Postconditions (post) ===" << std::endl;
    std::cout << "    - 函数调用后必须满足的条件" << std::endl;
    std::cout << "    - 被调用函数的责任" << std::endl;
    std::cout << "    - 例如：返回值范围保证" << std::endl;
    
    // 3. 断言 (Assertions)
    std::cout << "  === Assertions (contract_assert) ===" << std::endl;
    std::cout << "    - 程序执行过程中的不变量" << std::endl;
    std::cout << "    - 程序内部状态检查" << std::endl;
    std::cout << "    - 例如：循环不变量、状态一致性" << std::endl;
}
```

## 2. 合约断言

### 2.1 合约断言语法

合约断言是程序执行过程中需要保持的条件：

```cpp
#include <iostream>

void contract_assert_syntax() {
    std::cout << "Contract Assert Syntax:" << std::endl;
    
    // 基本语法
    std::cout << "  === Basic Syntax ===" << std::endl;
    std::cout << "    contract_assert(predicate);" << std::endl;
    std::cout << "    - predicate: 布尔表达式" << std::endl;
    std::cout << "    - 在运行时评估" << std::endl;
    
    // 示例
    std::cout << "  === Examples ===" << std::endl;
    std::cout << "    contract_assert(x > 0);" << std::endl;
    std::cout << "    contract_assert(ptr != nullptr);" << std::endl;
    std::cout << "    contract_assert(index < size);" << std::endl;
    std::cout << "    contract_assert(!container.empty());" << std::endl;
}
```

### 2.2 合约断言示例

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// 数组处理函数示例
void process_array(int* arr, size_t size) {
    // 确保数组指针不为空
    contract_assert(arr != nullptr);
    
    // 确保数组大小合理
    contract_assert(size > 0);
    contract_assert(size <= 1000000);  // 防止过大数组
    
    // 处理数组元素
    for (size_t i = 0; i < size; ++i) {
        // 确保数组元素在有效范围内
        contract_assert(arr[i] >= -1000 && arr[i] <= 1000);
        
        // 对元素进行处理
        arr[i] = arr[i] * 2;
        
        // 确保处理后结果仍在有效范围内
        contract_assert(arr[i] >= -2000 && arr[i] <= 2000);
    }
}

// 排序函数示例
void bubble_sort(std::vector<int>& vec) {
    // 确保向量不为空
    contract_assert(!vec.empty());
    
    size_t n = vec.size();
    
    // 排序过程中的不变量检查
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            // 确保索引有效
            contract_assert(j < n - 1);
            
            if (vec[j] > vec[j + 1]) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
        
        // 检查部分排序的不变量
        contract_assert(std::is_sorted(vec.begin(), vec.end() - (n - i - 1)));
    }
    
    // 确保最终结果正确排序
    contract_assert(std::is_sorted(vec.begin(), vec.end()));
}

// 状态机示例
class StateMachine {
private:
    enum class State { INIT, RUNNING, STOPPED };
    State current_state = State::INIT;
    
public:
    void start() {
        contract_assert(current_state == State::INIT);
        current_state = State::RUNNING;
        contract_assert(current_state == State::RUNNING);
    }
    
    void stop() {
        contract_assert(current_state == State::RUNNING);
        current_state = State::STOPPED;
        contract_assert(current_state == State::STOPPED);
    }
    
    bool is_running() const {
        contract_assert(current_state != State::INIT);
        return current_state == State::RUNNING;
    }
};

// 使用示例
int main() {
    // 测试数组处理
    int arr[] = {1, 2, 3, 4, 5};
    process_array(arr, 5);
    
    // 测试排序
    std::vector<int> vec = {5, 2, 8, 1, 9};
    bubble_sort(vec);
    
    // 测试状态机
    StateMachine sm;
    sm.start();
    std::cout << "Is running: " << sm.is_running() << std::endl;
    sm.stop();
    
    return 0;
}
```

## 3. 函数合约规范

### 3.1 函数合约语法

函数合约规范允许为函数指定前置条件和后置条件：

```cpp
#include <iostream>

void function_contract_syntax() {
    std::cout << "Function Contract Syntax:" << std::endl;
    
    // 前置条件语法
    std::cout << "  === Preconditions (pre) ===" << std::endl;
    std::cout << "    void function(parameters) pre(condition);" << std::endl;
    std::cout << "    - condition: 调用前必须满足的布尔表达式" << std::endl;
    
    // 后置条件语法
    std::cout << "  === Postconditions (post) ===" << std::endl;
    std::cout << "    void function(parameters) post(condition);" << std::endl;
    std::cout << "    - condition: 调用后必须满足的布尔表达式" << std::endl;
    
    // 组合使用
    std::cout << "  === Combined Usage ===" << std::endl;
    std::cout << "    void function(parameters) pre(pre_condition) post(post_condition);" << std::endl;
    
    // 复杂条件
    std::cout << "  === Complex Conditions ===" << std::endl;
    std::cout << "    可以使用逻辑运算符组合多个条件" << std::endl;
    std::cout << "    void func(int x) pre(x > 0 && x < 100);" << std::endl;
}
```

### 3.2 函数合约示例

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

// 数学函数示例
double divide(double a, double b) 
    pre(b != 0.0) 
    post(result >= 0.0 || result <= 0.0)  // 确保返回有效数值
{
    contract_assert(std::isfinite(a));
    contract_assert(std::isfinite(b));
    
    double result = a / b;
    
    // 确保结果是有效数值
    contract_assert(std::isfinite(result));
    
    return result;
}

// 容器操作示例
void push_element(std::vector<int>& vec, int element) 
    pre(!vec.empty()) 
    post(vec.size() == old(vec.size()) + 1)
{
    size_t old_size = vec.size();
    contract_assert(old_size > 0);
    
    vec.push_back(element);
    
    // 确保大小正确增加
    contract_assert(vec.size() == old_size + 1);
    contract_assert(vec.back() == element);
}

// 字符串处理示例
std::string trim_whitespace(const std::string& str) 
    post(result.empty() || (result.front() != ' ' && result.back() != ' '))
{
    contract_assert(!str.empty());
    
    std::string result = str;
    
    // 移除前导空格
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), 
                [](unsigned char ch) { return !std::isspace(ch); }));
    
    // 移除尾随空格
    result.erase(std::find_if(result.rbegin(), result.rend(),
                [](unsigned char ch) { return !std::isspace(ch); }).base(), result.end());
    
    // 确保结果正确
    contract_assert(result.empty() || (result.front() != ' ' && result.back() != ' '));
    
    return result;
}

// 内存管理示例
class Buffer {
private:
    char* data;
    size_t capacity;
    size_t size;
    
public:
    Buffer(size_t cap) 
        pre(cap > 0) 
        post(data != nullptr && capacity == cap && size == 0)
        : capacity(cap), size(0)
    {
        contract_assert(cap > 0);
        
        data = new char[cap];
        
        contract_assert(data != nullptr);
        contract_assert(capacity == cap);
        contract_assert(size == 0);
    }
    
    ~Buffer() 
        pre(data != nullptr)
    {
        contract_assert(data != nullptr);
        delete[] data;
    }
    
    void write(const char* src, size_t count) 
        pre(src != nullptr && count > 0 && size + count <= capacity)
        post(size == old(size) + count)
    {
        contract_assert(src != nullptr);
        contract_assert(count > 0);
        contract_assert(size + count <= capacity);
        
        size_t old_size = size;
        
        std::memcpy(data + size, src, count);
        size += count;
        
        contract_assert(size == old_size + count);
    }
    
    size_t get_size() const 
        post(result <= capacity)
    {
        contract_assert(size <= capacity);
        return size;
    }
};

// 使用示例
int main() {
    try {
        // 测试除法函数
        double result = divide(10.0, 2.0);
        std::cout << "10.0 / 2.0 = " << result << std::endl;
        
        // 测试容器操作
        std::vector<int> vec = {1, 2, 3};
        push_element(vec, 4);
        std::cout << "Vector size after push: " << vec.size() << std::endl;
        
        // 测试字符串处理
        std::string str = "  Hello World  ";
        std::string trimmed = trim_whitespace(str);
        std::cout << "Original: '" << str << "'" << std::endl;
        std::cout << "Trimmed: '" << trimmed << "'" << std::endl;
        
        // 测试缓冲区
        Buffer buffer(100);
        const char* data = "Hello, Contracts!";
        buffer.write(data, std::strlen(data));
        std::cout << "Buffer size: " << buffer.get_size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
```

## 4. 评估语义

### 4.1 评估语义类型

合约断言的评估使用不同的评估语义：

```cpp
#include <iostream>

void evaluation_semantics() {
    std::cout << "Evaluation Semantics:" << std::endl;
    
    // 1. 忽略语义 (Ignore)
    std::cout << "  === Ignore Semantic ===" << std::endl;
    std::cout << "    - 合约断言评估没有效果" << std::endl;
    std::cout << "    - 用于生产环境性能优化" << std::endl;
    std::cout << "    - 不进行任何检查" << std::endl;
    
    // 2. 观察语义 (Observe)
    std::cout << "  === Observe Semantic ===" << std::endl;
    std::cout << "    - 进行检查但不终止程序" << std::endl;
    std::cout << "    - 用于调试和测试" << std::endl;
    std::cout << "    - 可能产生诊断信息" << std::endl;
    
    // 3. 强制语义 (Enforce)
    std::cout << "  === Enforce Semantic ===" << std::endl;
    std::cout << "    - 进行检查并可能终止程序" << std::endl;
    std::cout << "    - 用于开发和关键系统" << std::endl;
    std::cout << "    - 调用契约违规处理器" << std::endl;
    
    // 4. 快速强制语义 (Quick-enforce)
    std::cout << "  === Quick-enforce Semantic ===" << std::endl;
    std::cout << "    - 立即终止程序" << std::endl;
    std::cout << "    - 用于性能关键场景" << std::endl;
    std::cout << "    - 不调用处理器，直接终止" << std::endl;
}
```

### 4.2 评估语义示例

```cpp
#include <iostream>
#include <stdexcept>

// 不同评估语义的行为示例
void demonstrate_evaluation_semantics() {
    std::cout << "Demonstrating Evaluation Semantics:" << std::endl;
    
    // 带副作用的合约断言
    int counter = 0;
    
    auto test_function = [&counter](bool condition) {
        contract_assert((counter++, condition));  // 副作用：递增counter
        return counter;
    };
    
    std::cout << "Initial counter: " << counter << std::endl;
    
    // 根据不同的评估语义，counter的行为可能不同
    try {
        int result1 = test_function(true);
        std::cout << "After true condition: " << result1 << std::endl;
        
        int result2 = test_function(false);  // 可能触发契约违规
        std::cout << "After false condition: " << result2 << std::endl;
    } catch (...) {
        std::cout << "Exception caught" << std::endl;
    }
    
    std::cout << "Final counter: " << counter << std::endl;
}

// 常量表达式中的合约断言
constexpr int factorial(int n) 
    pre(n >= 0) 
    post(result > 0)
{
    contract_assert(n >= 0);
    
    if (n <= 1) {
        contract_assert(n == 0 || n == 1);
        return 1;
    }
    
    int result = n * factorial(n - 1);
    contract_assert(result > 0);
    return result;
}

// 编译时常量表达式评估
void compile_time_contracts() {
    std::cout << "Compile-time Contract Evaluation:" << std::endl;
    
    // 这些在编译时评估
    constexpr int fact5 = factorial(5);
    std::cout << "5! = " << fact5 << std::endl;
    
    // 这个可能在编译时违反契约
    // constexpr int fact_neg = factorial(-1);  // 编译错误
}
```

## 5. 契约违规处理

### 5.1 契约违规检测

契约违规在以下情况下发生：

```cpp
#include <iostream>
#include <stdexcept>

void contract_violation_detection() {
    std::cout << "Contract Violation Detection:" << std::endl;
    
    // 1. 谓词值为false
    std::cout << "  === Predicate is False ===" << std::endl;
    auto check_positive = [](int x) {
        contract_assert(x > 0);  // 如果x <= 0则违规
        return x * 2;
    };
    
    // 2. 谓词评估抛出异常
    std::cout << "  === Predicate Throws Exception ===" << std::endl;
    auto check_with_exception = [](int x) {
        contract_assert([&]() {
            if (x < 0) throw std::invalid_argument("Negative value");
            return x > 0;
        }());
        return x;
    };
    
    // 3. 常量表达式中非核心常量表达式
    std::cout << "  === Non-Core Constant Expression ===" << std::endl;
    // constexpr auto result = []() {
    //     int x = 5;
    //     contract_assert(x > 10);  // 违规：谓词为false
    //     return x;
    // }();
}
```

### 5.2 契约违规处理示例

```cpp
#include <iostream>
#include <stdexcept>

// 模拟契约违规处理
void handle_contract_violations() {
    std::cout << "Handling Contract Violations:" << std::endl;
    
    // 常量表达式上下文中的违规
    std::cout << "  === Constant Expression Context ===" << std::endl;
    // 在constexpr函数中违反契约会导致编译错误
    
    // 非常量表达式上下文中的违规
    std::cout << "  === Non-Constant Expression Context ===" << std::endl;
    
    auto risky_function = [](int value) {
        // 前置条件检查
        contract_assert(value > 0);
        
        // 执行操作
        int result = 100 / value;
        
        // 后置条件检查
        contract_assert(result > 0);
        
        return result;
    };
    
    try {
        // 正常调用
        int result1 = risky_function(10);
        std::cout << "Result for 10: " << result1 << std::endl;
        
        // 违规调用
        int result2 = risky_function(0);  // 前置条件违规
        std::cout << "Result for 0: " << result2 << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
}
```

## 6. 契约违规处理器

### 6.1 违规处理器定义

契约违规处理器是处理契约违规的自定义函数：

```cpp
#include <iostream>
#include <contracts>

// 自定义契约违规处理器 (C++26)
void handle_contract_violation(const std::contracts::contract_violation& violation) {
    std::cout << "=== Contract Violation Detected ===" << std::endl;
    std::cout << "File: " << violation.file() << std::endl;
    std::cout << "Line: " << violation.line() << std::endl;
    std::cout << "Function: " << violation.function() << std::endl;
    std::cout << "Condition: " << violation.condition() << std::endl;
    std::cout << "Comment: " << violation.comment() << std::endl;
    
    // 可以选择如何处理违规
    // 1. 记录日志并继续
    std::cerr << "Contract violation: " << violation.condition() << std::endl;
    
    // 2. 抛出异常
    // throw std::runtime_error("Contract violation occurred");
    
    // 3. 终止程序
    // std::terminate();
    
    // 4. 正常返回（如果评估语义允许）
    // 控制流将继续执行
}

void custom_handler_example() {
    std::cout << "Custom Contract Violation Handler:" << std::endl;
    
    auto test_function = [](int x) pre(x > 0) {
        contract_assert(x != 42);  // 特殊测试值
        return x * 2;
    };
    
    try {
        int result1 = test_function(5);   // 正常
        std::cout << "Result for 5: " << result1 << std::endl;
        
        int result2 = test_function(42);  // 违规
        std::cout << "Result for 42: " << result2 << std::endl;
    } catch (...) {
        std::cout << "Exception caught" << std::endl;
    }
}
```

### 6.2 默认违规处理器

```cpp
#include <iostream>

void default_handler_behavior() {
    std::cout << "Default Contract Violation Handler:" << std::endl;
    
    // C++标准建议的默认行为
    std::cout << "  === Standard Recommendation ===" << std::endl;
    std::cout << "    - 产生格式化的诊断输出" << std::endl;
    std::cout << "    - 对重复违规进行速率限制" << std::endl;
    std::cout << "    - 正常返回（根据评估语义）" << std::endl;
    
    // 实现示例（伪代码）
    std::cout << "  === Implementation Example ===" << std::endl;
    std::cout << "    void default_handler(const contract_violation& v) {" << std::endl;
    std::cout << "        static std::set<std::string> reported_violations;" << std::endl;
    std::cout << "        std::string key = v.file() + std::to_string(v.line());" << std::endl;
    std::cout << "        if (reported_violations.find(key) == reported_violations.end()) {" << std::endl;
    std::cout << "            std::cerr << \"Contract violation at \" << v.file() << \":\" << v.line() << std::endl;" << std::endl;
    std::cout << "            reported_violations.insert(key);" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "    }" << std::endl;
}
```

## 7. 按顺序评估

### 7.1 顺序评估规则

合约断言列表按特定顺序评估：

```cpp
#include <iostream>

void sequential_evaluation() {
    std::cout << "Sequential Evaluation:" << std::endl;
    
    // 评估规则
    std::cout << "  === Evaluation Rules ===" << std::endl;
    std::cout << "    1. 保持原始顺序" << std::endl;
    std::cout << "    2. 允许重复评估" << std::endl;
    std::cout << "    3. 实现定义的重复次数" << std::endl;
    std::cout << "    4. 保持相对顺序" << std::endl;
    
    // 示例说明
    std::cout << "  === Example ===" << std::endl;
    std::cout << "    void f(int i)" << std::endl;
    std::cout << "    {" << std::endl;
    std::cout << "        contract_assert(i > 0);  // #1" << std::endl;
    std::cout << "        contract_assert(i < 10); // #2" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    Valid sequences:" << std::endl;
    std::cout << "    - #1 #2 (no repeat)" << std::endl;
    std::cout << "    - #1 #1 #2 #2 (repeat in sequence)" << std::endl;
    std::cout << "    - #1 #2 #1 #2 (repeat alternatively)" << std::endl;
    std::cout << "    - #1 #2 #2 #1 (second occurrences can switch order)" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    Invalid sequences:" << std::endl;
    std::cout << "    - #2 #1 (first occurrences cannot switch)" << std::endl;
}
```

### 7.2 顺序评估示例

```cpp
#include <iostream>

// 全局计数器用于演示评估顺序
static int evaluation_counter = 0;

void demonstrate_sequential_evaluation() {
    std::cout << "Demonstrating Sequential Evaluation:" << std::endl;
    
    auto reset_counter = []() {
        evaluation_counter = 0;
    };
    
    auto increment_and_check = [](int value, int expected_order) {
        int current = ++evaluation_counter;
        std::cout << "  Evaluation #" << current << ": checking " << value << std::endl;
        contract_assert(current == expected_order);
        return value > 0;
    };
    
    auto test_function = [](int a, int b, int c) {
        reset_counter();
        
        contract_assert(increment_and_check(a, 1));
        contract_assert(increment_and_check(b, 2));
        contract_assert(increment_and_check(c, 3));
        
        return a + b + c;
    };
    
    try {
        int result = test_function(1, 2, 3);
        std::cout << "Result: " << result << std::endl;
    } catch (...) {
        std::cout << "Contract violation occurred" << std::endl;
    }
}
```

## 8. 实践示例

### 8.1 完整的合约契约系统示例

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

// 安全字符串类
class SafeString {
private:
    std::string data;
    
public:
    // 构造函数
    explicit SafeString(const std::string& str = "") 
        pre(str.length() <= 1000)  // 限制最大长度
        post(data.length() == str.length())
        : data(str)
    {
        contract_assert(str.length() <= 1000);
        contract_assert(data.length() == str.length());
    }
    
    // 拷贝构造函数
    SafeString(const SafeString& other) 
        post(data == other.data)
        : data(other.data)
    {
        contract_assert(data == other.data);
    }
    
    // 赋值操作符
    SafeString& operator=(const SafeString& other) 
        post(data == other.data)
    {
        contract_assert(this != &other);  // 自赋值检查
        
        data = other.data;
        
        contract_assert(data == other.data);
        return *this;
    }
    
    // 获取字符
    char& at(size_t index) 
        pre(index < data.length()) 
        post(result == old(data[index]))
    {
        contract_assert(index < data.length());
        
        char old_char = data[index];
        char& result = data.at(index);
        
        contract_assert(result == old_char);
        return result;
    }
    
    const char& at(size_t index) const 
        pre(index < data.length())
    {
        contract_assert(index < data.length());
        return data.at(index);
    }
    
    // 追加字符串
    SafeString& append(const SafeString& other) 
        pre(data.length() + other.data.length() <= 1000)
        post(data.length() == old(data.length()) + other.data.length())
    {
        contract_assert(data.length() + other.data.length() <= 1000);
        
        size_t old_length = data.length();
        
        data.append(other.data);
        
        contract_assert(data.length() == old_length + other.data.length());
        return *this;
    }
    
    // 获取长度
    size_t length() const 
        post(result <= 1000)
    {
        contract_assert(data.length() <= 1000);
        return data.length();
    }
    
    // 检查是否为空
    bool empty() const {
        bool result = data.empty();
        contract_assert(result == (data.length() == 0));
        return result;
    }
    
    // 转换为std::string
    const std::string& str() const {
        return data;
    }
};

// 安全向量类
template<typename T>
class SafeVector {
private:
    std::vector<T> data;
    
public:
    // 构造函数
    explicit SafeVector(size_t size = 0) 
        pre(size <= 10000)
        post(data.size() == size)
        : data(size)
    {
        contract_assert(size <= 10000);
        contract_assert(data.size() == size);
    }
    
    // 析构函数
    ~SafeVector() 
        pre(data.size() <= 10000)
    {
        contract_assert(data.size() <= 10000);
    }
    
    // 访问元素
    T& at(size_t index) 
        pre(index < data.size())
        post(result == old(data[index]))
    {
        contract_assert(index < data.size());
        
        T old_value = data[index];
        T& result = data.at(index);
        
        contract_assert(result == old_value);
        return result;
    }
    
    const T& at(size_t index) const 
        pre(index < data.size())
    {
        contract_assert(index < data.size());
        return data.at(index);
    }
    
    // 添加元素
    void push_back(const T& value) 
        pre(data.size() < 10000)
        post(data.size() == old(data.size()) + 1)
    {
        contract_assert(data.size() < 10000);
        
        size_t old_size = data.size();
        
        data.push_back(value);
        
        contract_assert(data.size() == old_size + 1);
        contract_assert(data.back() == value);
    }
    
    // 获取大小
    size_t size() const 
        post(result <= 10000)
    {
        contract_assert(data.size() <= 10000);
        return data.size();
    }
    
    // 检查是否为空
    bool empty() const {
        bool result = data.empty();
        contract_assert(result == (data.size() == 0));
        return result;
    }
    
    // 清空向量
    void clear() 
        post(data.empty())
    {
        data.clear();
        contract_assert(data.empty());
    }
};

// 使用示例
int main() {
    std::cout << "=== Safe String Example ===" << std::endl;
    
    try {
        // 创建安全字符串
        SafeString str1("Hello");
        SafeString str2(" World");
        
        std::cout << "str1: " << str1.str() << std::endl;
        std::cout << "str2: " << str2.str() << std::endl;
        
        // 连接字符串
        str1.append(str2);
        std::cout << "After append: " << str1.str() << std::endl;
        
        // 访问字符
        char ch = str1.at(0);
        std::cout << "First character: " << ch << std::endl;
        
        // 修改字符
        str1.at(0) = 'h';
        std::cout << "After modification: " << str1.str() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== Safe Vector Example ===" << std::endl;
    
    try {
        // 创建安全向量
        SafeVector<int> vec(5);
        
        std::cout << "Initial size: " << vec.size() << std::endl;
        
        // 添加元素
        for (int i = 0; i < 3; ++i) {
            vec.push_back(i * 10);
        }
        
        std::cout << "After adding elements, size: " << vec.size() << std::endl;
        
        // 访问元素
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << "Element " << i << ": " << vec.at(i) << std::endl;
        }
        
        // 清空向量
        vec.clear();
        std::cout << "After clear, size: " << vec.size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
```

### 8.2 数学库合约示例

```cpp
#include <iostream>
#include <cmath>
#include <limits>

// 数学函数库示例
namespace math_contracts {
    
    // 平方根函数
    double sqrt(double x) 
        pre(x >= 0.0) 
        post(result >= 0.0 && std::abs(result * result - x) < 1e-10)
    {
        contract_assert(x >= 0.0);
        contract_assert(std::isfinite(x));
        
        if (x == 0.0) {
            contract_assert(result == 0.0);
            return 0.0;
        }
        
        double result = std::sqrt(x);
        
        contract_assert(result >= 0.0);
        contract_assert(std::isfinite(result));
        contract_assert(std::abs(result * result - x) < 1e-10);
        
        return result;
    }
    
    // 幂函数
    double power(double base, int exponent) 
        pre(exponent >= 0 || base != 0.0) 
        post(exponent >= 0 || result == 1.0 / old(power(base, -exponent)))
    {
        contract_assert(exponent >= 0 || base != 0.0);
        contract_assert(std::isfinite(base));
        
        if (exponent == 0) {
            contract_assert(result == 1.0);
            return 1.0;
        }
        
        if (exponent < 0) {
            double positive_power = power(base, -exponent);
            double result = 1.0 / positive_power;
            
            contract_assert(std::isfinite(result));
            return result;
        }
        
        double result = 1.0;
        double base_power = base;
        
        while (exponent > 0) {
            if (exponent & 1) {
                result *= base_power;
            }
            base_power *= base_power;
            exponent >>= 1;
        }
        
        contract_assert(std::isfinite(result));
        return result;
    }
    
    // 对数函数
    double ln(double x) 
        pre(x > 0.0) 
        post(std::isfinite(result))
    {
        contract_assert(x > 0.0);
        contract_assert(std::isfinite(x));
        
        double result = std::log(x);
        
        contract_assert(std::isfinite(result));
        
        // 验证结果
        contract_assert(std::abs(std::exp(result) - x) < 1e-10);
        
        return result;
    }
    
    // 三角函数
    double sin(double x) 
        post(result >= -1.0 && result <= 1.0)
    {
        contract_assert(std::isfinite(x));
        
        double result = std::sin(x);
        
        contract_assert(result >= -1.0 && result <= 1.0);
        contract_assert(std::isfinite(result));
        
        return result;
    }
    
    double cos(double x) 
        post(result >= -1.0 && result <= 1.0)
    {
        contract_assert(std::isfinite(x));
        
        double result = std::cos(x);
        
        contract_assert(result >= -1.0 && result <= 1.0);
        contract_assert(std::isfinite(result));
        
        return result;
    }
}

// 使用示例
int main() {
    using namespace math_contracts;
    
    std::cout << "=== Mathematical Functions with Contracts ===" << std::endl;
    
    try {
        // 测试平方根
        std::cout << "sqrt(16) = " << sqrt(16.0) << std::endl;
        std::cout << "sqrt(2) = " << sqrt(2.0) << std::endl;
        
        // 测试幂函数
        std::cout << "power(2, 3) = " << power(2.0, 3) << std::endl;
        std::cout << "power(2, 0) = " << power(2.0, 0) << std::endl;
        std::cout << "power(2, -2) = " << power(2.0, -2) << std::endl;
        
        // 测试对数
        std::cout << "ln(1) = " << ln(1.0) << std::endl;
        std::cout << "ln(2.71828) = " << ln(2.71828) << std::endl;
        
        // 测试三角函数
        std::cout << "sin(0) = " << sin(0.0) << std::endl;
        std::cout << "cos(0) = " << cos(0.0) << std::endl;
        std::cout << "sin(3.14159/2) = " << sin(3.14159/2) << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
```

## 9. 标准参考

C++ 合约契约的相关标准条款：

- C++26 标准（ISO/IEC 14882:2026）：
  - 9.11 合约 [contracts]
  - 9.11.1 合约断言 [contract.assert]
  - 9.11.2 函数合约规范 [contract.fcn]
  - 9.11.3 合约违规 [contract.violation]
  - 9.11.4 合约评估 [contract.eval]
  - 9.11.5 契约违规处理器 [contract.handler]

特性测试宏：

- `__cpp_contracts`: 202502L (C++26) - 契约支持

相关的标准库支持：

- `std::contracts` 命名空间
- `std::contracts::contract_violation` 类
- `::handle_contract_violation` 函数

相关的提案：

- P2900R14: Contracts for C++ (最终技术规范)

## 10. 总结

C++ 合约契约是现代C++的重要特性，为程序正确性提供了强大的语言支持。

### 10.1 核心要点

- 合约类型：前置条件(pre)、后置条件(post)、断言(contract_assert)
- 评估语义：忽略、观察、强制、快速强制四种语义
- 违规处理：可自定义的违规处理器机制
- 顺序评估：保证合约断言的评估顺序
- 编译时支持：常量表达式中的合约评估

### 10.2 最佳实践

- 合理使用：在关键路径和接口处使用合约
- 性能考虑：根据环境选择合适的评估语义
- 文档化：合约作为代码文档的重要部分
- 测试覆盖：确保合约条件得到充分测试
- 渐进采用：从新代码开始逐步引入合约

### 10.3 未来发展趋势

- 工具支持：IDE和静态分析工具对合约的支持
- 编译器优化：基于合约的编译时优化
- 形式化验证：与形式化验证工具的集成
- 标准完善：合约机制的进一步标准化
- 生态系统：第三方库广泛采用合约

通过深入理解和正确应用C++合约契约，开发者可以构建更可靠、更易维护的高质量C++应用程序。
