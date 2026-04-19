# C++ 协程支持库（Coroutine support）（C++20起）

来源：cppreference.com

## 协程支持库概述

协程支持库定义了多个类型，为协程提供编译时和运行时支持，使开发者能够编写高效的异步和生成器代码。

## 核心组件

### 一、协程特征

| 组件 | 说明 |
|------|------|
| `<coroutine>`头文件 | |
| `coroutine_traits`(C++20) | 用于发现协程承诺类型的特征类型（类模板） |

### 二、协程句柄

| 组件 | 说明 |
|------|------|
| `<coroutine>`头文件 | |
| `coroutine_handle`(C++20) | 用于引用挂起或执行中的协程（类模板） |

### 三、无操作协程

| 组件 | 说明 |
|------|------|
| `<coroutine>`头文件 | |
| `noop_coroutine`(C++20) | 创建恢复或销毁时无可观测效果的协程句柄（函数） |
| `noop_coroutine_promise`(C++20) | 用于无可观测效果的协程（类） |
| `noop_coroutine_handle`(C++20) | `std::coroutine_handle<std::noop_coroutine_promise>`，用于引用无操作协程（typedef） |

### 四、简单等待对象

| 组件 | 说明 |
|------|------|
| `<coroutine>`头文件 | |
| `suspend_never`(C++20) | 指示await表达式永不挂起（类） |
| `suspend_always`(C++20) | 指示await表达式总是挂起（类） |

### 五、范围生成器

| 组件 | 说明 |
|------|------|
| `<generator>`头文件(C++23) | |
| `generator`(C++23) | 表示同步协程生成器的视图（类模板） |

---

## 扩展知识详解

### 一、协程基础概念

#### 1. 协程的核心组件
```cpp
#include <coroutine>
#include <iostream>
#include <chrono>
#include <thread>

// 简单的协程承诺类型
struct SimplePromise {
    // 协程初始挂起点
    auto initial_suspend() { return std::suspend_never{}; }
    
    // 协程最终挂起点
    auto final_suspend() noexcept { return std::suspend_always{}; }
    
    // 协程返回值处理
    void return_void() {}
    
    // 协程异常处理
    void unhandled_exception() { std::terminate(); }
    
    // 获取协程句柄
    auto get_return_object() {
        return std::coroutine_handle<SimplePromise>::from_promise(*this);
    }
};

// 使用简单承诺类型的协程
std::coroutine_handle<SimplePromise> simple_coroutine() {
    std::cout << "Coroutine started" << std::endl;
    co_await std::suspend_always{};  // 挂起协程
    std::cout << "Coroutine resumed" << std::endl;
    co_return;  // 协程结束
}

void basic_coroutine_example() {
    auto handle = simple_coroutine();
    std::cout << "Coroutine created" << std::endl;
    
    if (!handle.done()) {
        std::cout << "Resuming coroutine" << std::endl;
        handle.resume();  // 恢复协程
    }
    
    if (handle.done()) {
        std::cout << "Coroutine finished" << std::endl;
        handle.destroy();  // 销毁协程
    }
}
```

#### 2. 协程特征详解
```cpp
#include <coroutine>
#include <iostream>

// 自定义协程类型
template<typename T>
struct Task {
    struct promise_type {
        T value;
        
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        
        void return_value(T v) { value = v; }
        void unhandled_exception() { std::terminate(); }
        
        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
    };
    
    std::coroutine_handle<promise_type> handle;
    
    Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    
    ~Task() {
        if (handle) handle.destroy();
    }
    
    T get() {
        return handle.promise().value;
    }
};

// 使用自定义协程类型
Task<int> compute_value() {
    std::cout << "Computing value..." << std::endl;
    co_return 42;
}

void coroutine_traits_example() {
    auto task = compute_value();
    std::cout << "Result: " << task.get() << std::endl;
}
```

### 二、协程句柄详解

#### 1. 协程句柄操作
```cpp
#include <coroutine>
#include <iostream>

struct CounterPromise {
    int count = 0;
    
    auto initial_suspend() { return std::suspend_never{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    
    auto get_return_object() {
        return std::coroutine_handle<CounterPromise>::from_promise(*this);
    }
    
    auto yield_value(int value) {
        count = value;
        return std::suspend_always{};
    }
};

// 生成器协程
std::coroutine_handle<CounterPromise> counter(int max_count) {
    for (int i = 0; i < max_count; ++i) {
        co_yield i;  // 暂停并返回值
        std::cout << "Resumed with value: " << i << std::endl;
    }
    co_return;
}

void coroutine_handle_examples() {
    auto handle = counter(3);
    
    while (!handle.done()) {
        std::cout << "Current count: " << handle.promise().count << std::endl;
        handle.resume();  // 恢复协程
    }
    
    std::cout << "Coroutine completed" << std::endl;
    handle.destroy();
}
```

#### 2. 协程状态检查
```cpp
#include <coroutine>
#include <iostream>

struct StateCheckPromise {
    auto initial_suspend() { return std::suspend_never{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
    
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    
    auto get_return_object() {
        return std::coroutine_handle<StateCheckPromise>::from_promise(*this);
    }
};

std::coroutine_handle<StateCheckPromise> state_checker() {
    std::cout << "Before first suspend" << std::endl;
    co_await std::suspend_always{};
    std::cout << "After first suspend" << std::endl;
    co_await std::suspend_always{};
    std::cout << "After second suspend" << std::endl;
    co_return;
}

void state_check_example() {
    auto handle = state_checker();
    
    std::cout << "Initial done(): " << handle.done() << std::endl;
    
    handle.resume();
    std::cout << "After first resume, done(): " << handle.done() << std::endl;
    
    handle.resume();
    std::cout << "After second resume, done(): " << handle.done() << std::endl;
    
    handle.resume();
    std::cout << "After third resume, done(): " << handle.done() << std::endl;
    
    handle.destroy();
}
```

### 三、简单等待对象

#### 1. suspend_never和suspend_always
```cpp
#include <coroutine>
#include <iostream>

struct NeverSuspendPromise {
    auto initial_suspend() { return std::suspend_never{}; }  // 不挂起
    auto final_suspend() noexcept { return std::suspend_never{}; }
    
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    
    auto get_return_object() {
        return std::coroutine_handle<NeverSuspendPromise>::from_promise(*this);
    }
};

struct AlwaysSuspendPromise {
    auto initial_suspend() { return std::suspend_always{}; }  // 总是挂起
    auto final_suspend() noexcept { return std::suspend_always{}; }
    
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    
    auto get_return_object() {
        return std::coroutine_handle<AlwaysSuspendPromise>::from_promise(*this);
    }
};

void suspend_examples() {
    // suspend_never示例
    std::cout << "=== suspend_never example ===" << std::endl;
    auto never_handle = []() -> std::coroutine_handle<NeverSuspendPromise> {
        std::cout << "Coroutine started and runs to completion" << std::endl;
        co_return;
    }();
    
    std::cout << "Coroutine done immediately: " << never_handle.done() << std::endl;
    never_handle.destroy();
    
    // suspend_always示例
    std::cout << "\n=== suspend_always example ===" << std::endl;
    auto always_handle = []() -> std::coroutine_handle<AlwaysSuspendPromise> {
        std::cout << "Coroutine started but suspended" << std::endl;
        co_return;
    }();
    
    std::cout << "Coroutine suspended initially: " << always_handle.done() << std::endl;
    always_handle.resume();
    std::cout << "Coroutine done after resume: " << always_handle.done() << std::endl;
    always_handle.destroy();
}
```

### 四、无操作协程

#### 1. noop_coroutine使用
```cpp
#include <coroutine>
#include <iostream>

void noop_coroutine_examples() {
    // 创建无操作协程
    auto noop_handle = std::noop_coroutine();
    
    std::cout << "No-op coroutine type: " 
              << typeid(noop_handle.promise()).name() << std::endl;
    
    // 无操作协程的操作
    noop_handle.resume();  // 无效果
    noop_handle.destroy(); // 无效果
    
    std::cout << "No-op coroutine done: " << noop_handle.done() << std::endl;
    
    // 检查是否为无操作协程
    if (noop_handle == std::noop_coroutine()) {
        std::cout << "Handle is a no-op coroutine" << std::endl;
    }
}
```

### 五、生成器协程（C++23）

#### 1. std::generator基础使用
```cpp
#include <generator>
#include <iostream>

// 简单的整数序列生成器
std::generator<int> simple_sequence(int start, int count) {
    for (int i = 0; i < count; ++i) {
        co_yield start + i;
    }
}

// 斐波那契数列生成器
std::generator<long long> fibonacci() {
    long long a = 0, b = 1;
    while (true) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

void generator_examples() {
    // 使用简单序列生成器
    std::cout << "Simple sequence: ";
    for (int value : simple_sequence(10, 5)) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // 使用斐波那契生成器
    std::cout << "Fibonacci sequence: ";
    int count = 0;
    for (auto value : fibonacci()) {
        if (count++ >= 10) break;
        std::cout << value << " ";
    }
    std::cout << std::endl;
}
```

#### 2. 复杂生成器示例
```cpp
#include <generator>
#include <iostream>
#include <vector>
#include <string>

// 字符串分割生成器
std::generator<std::string_view> split_string(std::string_view text, char delimiter) {
    size_t start = 0;
    size_t pos = 0;
    
    while ((pos = text.find(delimiter, start)) != std::string_view::npos) {
        if (pos > start) {
            co_yield text.substr(start, pos - start);
        }
        start = pos + 1;
    }
    
    if (start < text.length()) {
        co_yield text.substr(start);
    }
}

// 树遍历生成器
struct TreeNode {
    int value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    
    TreeNode(int v) : value(v) {}
};

std::generator<int> inorder_traversal(TreeNode* root) {
    if (!root) co_return;
    
    if (root->left) {
        for (int value : inorder_traversal(root->left)) {
            co_yield value;
        }
    }
    
    co_yield root->value;
    
    if (root->right) {
        for (int value : inorder_traversal(root->right)) {
            co_yield value;
        }
    }
}

void advanced_generator_examples() {
    // 字符串分割
    std::string text = "hello,world,cpp,coroutines";
    std::cout << "Split string: ";
    for (auto part : split_string(text, ',')) {
        std::cout << "[" << part << "] ";
    }
    std::cout << std::endl;
    
    // 树遍历
    TreeNode root(1);
    root.left = new TreeNode(2);
    root.right = new TreeNode(3);
    root.left->left = new TreeNode(4);
    root.left->right = new TreeNode(5);
    
    std::cout << "Inorder traversal: ";
    for (int value : inorder_traversal(&root)) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // 清理内存
    delete root.left->left;
    delete root.left->right;
    delete root.left;
    delete root.right;
}
```

### 六、实际应用场景

#### 1. 异步任务处理
```cpp
#include <coroutine>
#include <iostream>
#include <chrono>
#include <thread>
#include <future>

template<typename T>
struct AsyncTask {
    struct promise_type {
        std::promise<T> promise_;
        
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        
        void return_value(const T& value) {
            promise_.set_value(value);
        }
        
        void unhandled_exception() {
            promise_.set_exception(std::current_exception());
        }
        
        AsyncTask get_return_object() {
            return AsyncTask{promise_.get_future(), 
                           std::coroutine_handle<promise_type>::from_promise(*this)};
        }
    };
    
    std::future<T> future_;
    std::coroutine_handle<promise_type> handle_;
    
    AsyncTask(std::future<T> f, std::coroutine_handle<promise_type> h) 
        : future_(std::move(f)), handle_(h) {}
    
    ~AsyncTask() {
        if (handle_) handle_.destroy();
    }
    
    T get() {
        return future_.get();
    }
};

// 模拟异步计算
AsyncTask<int> async_computation(int input) {
    std::cout << "Starting async computation with input: " << input << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    co_return input * input;
}

void async_task_example() {
    auto task1 = async_computation(5);
    auto task2 = async_computation(10);
    
    std::cout << "Results: " << task1.get() << ", " << task2.get() << std::endl;
}
```

#### 2. 数据流处理
```cpp
#include <generator>
#include <iostream>
#include <vector>
#include <algorithm>

// 数据处理管道
template<typename T>
std::generator<T> filter(std::generator<T> source, auto predicate) {
    for (T value : source) {
        if (predicate(value)) {
            co_yield value;
        }
    }
}

template<typename T, typename U>
std::generator<U> transform(std::generator<T> source, auto transformer) {
    for (T value : source) {
        co_yield transformer(value);
    }
}

std::generator<int> numbers(int start, int count) {
    for (int i = 0; i < count; ++i) {
        co_yield start + i;
    }
}

void data_pipeline_example() {
    // 创建数据处理管道
    auto pipeline = transform(
        filter(
            numbers(1, 20),
            [](int x) { return x % 2 == 0; }  // 过滤偶数
        ),
        [](int x) { return x * x; }  // 平方
    );
    
    std::cout << "Processed numbers: ";
    for (int value : pipeline) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}
```

## 最佳实践建议

### 一、协程设计准则

1. **明确生命周期**：确保协程句柄的正确管理和销毁
2. **异常安全**：正确处理协程中的异常情况
3. **资源管理**：使用RAII原则管理协程资源
4. **性能考虑**：避免不必要的协程挂起和恢复

### 二、使用建议

1. **选择合适的等待对象**：根据需求选择`suspend_never`或`suspend_always`
2. **正确处理返回值**：定义合适的承诺类型处理返回值
3. **状态管理**：合理管理协程的执行状态
4. **调试支持**：为协程提供良好的调试信息

### 三、性能优化

1. **减少分配**：尽量减少协程状态对象的内存分配
2. **避免深度递归**：协程递归可能导致栈溢出
3. **批量处理**：考虑批量处理减少协程切换开销
4. **缓存友好**：设计协程数据结构时考虑缓存局部性

## 特性测试宏

| 宏 | 值 | 标准 | 功能 |
|-----|-----|------|------|
| `__cpp_impl_coroutine` | `201902L` | C++20 | 协程（编译器支持） |
| `__cpp_lib_coroutine` | `201902L` | C++20 | 协程（库支持） |
| `__cpp_lib_generator` | `202207L` | C++23 | `std::generator`：范围同步协程生成器 |

## 相关页面

| 页面 | 说明 |
|------|------|
| 生成器(C++23) | 表示同步协程生成器的视图 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/coroutine&oldid=162075>
- 最后修改时间：2023年11月5日 11:39
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++协程支持库为现代C++提供了强大的异步编程和生成器功能。从基础的协程句柄管理到高级的生成器模式，协程技术让复杂的异步操作变得直观和高效。正确理解和使用协程特性对于开发现代高性能C++应用程序至关重要。通过合理的协程设计和优化，可以构建出更加优雅和高效的代码结构。持续学习和实践协程编程技术，结合具体的应用场景，能够充分发挥协程的强大能力。记住协程的复杂性，始终进行充分的测试和性能分析。