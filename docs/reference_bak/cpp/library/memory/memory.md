# C++内存管理库（Memory Management Library）

来源：cppreference.com

## 内存管理库概述

内存管理库提供了一系列工具和类来管理程序中的内存分配、释放和生命周期管理。它包括智能指针、分配器、内存资源管理、未初始化存储操作等功能。

## 核心组件

### 一、智能指针（Smart Pointers）（C++11起）

智能指针提供自动的、异常安全的对象生命周期管理。

| 智能指针类型 | 说明 |
|-------------|------|
| `unique_ptr`(C++11) | 具有唯一对象所有权语义的智能指针（类模板） |
| `shared_ptr`(C++11) | 具有共享对象所有权语义的智能指针（类模板） |
| `weak_ptr`(C++11) | 对`std::shared_ptr`管理的对象的弱引用（类模板） |
| `auto_ptr`(C++17前) | 具有严格对象所有权语义的智能指针（类模板，已弃用） |

#### 辅助类
| 类 | 说明 |
|----|------|
| `owner_less`(C++11) | 提供基于所有者的共享指针和弱指针混合类型排序（类模板） |
| `enable_shared_from_this`(C++11) | 允许对象创建指向自身的`shared_ptr`（类模板） |
| `bad_weak_ptr`(C++11) | 访问已销毁对象的`weak_ptr`时抛出的异常（类） |
| `default_delete`(C++11) | `unique_ptr`的默认删除器（类模板） |

#### 智能指针适配器（C++23起）
| 适配器 | 说明 |
|--------|------|
| `out_ptr_t`(C++23) | 与外部指针设置器交互，在销毁时重置智能指针（类模板） |
| `inout_ptr_t`(C++23) | 与外部指针设置器交互，从智能指针获取初始指针值，并在销毁时重置（类模板） |

### 二、分配器（Allocators）

分配器是封装内存分配策略的类模板，使通用容器能够将内存管理与数据本身分离。

| 分配器组件 | 说明 |
|------------|------|
| `allocator` | 默认分配器（类模板） |
| `allocator_traits`(C++11) | 提供分配器类型信息（类模板） |
| `scoped_allocator_adaptor`(C++11) | 为多级容器实现多级分配器（类模板） |
| `polymorphic_allocator`(C++17) | 基于构造时的`std::pmr::memory_resource`支持运行时多态的分配器（类模板） |

### 三、内存资源（Memory Resources）（C++17起）

内存资源实现可由`std::pmr::polymorphic_allocator`使用的内存分配策略。

| 内存资源 | 说明 |
|----------|------|
| `memory_resource`(C++17) | 封装内存资源的类的抽象接口（类） |
| `new_delete_resource`(C++17) | 返回使用全局`operator new`和`operator delete`分配和释放内存的静态程序级资源（函数） |
| `null_memory_resource`(C++17) | 返回不执行分配的静态`std::pmr::memory_resource`（函数） |
| `synchronized_pool_resource`(C++17) | 管理不同块大小池分配的线程安全`std::pmr::memory_resource`（类） |
| `unsynchronized_pool_resource`(C++17) | 管理不同块大小池分配的非线程安全`std::pmr::memory_resource`（类） |
| `monotonic_buffer_resource`(C++17) | 在资源销毁时才释放分配内存的专用`std::pmr::memory_resource`（类） |

### 四、未初始化内存算法

用于在未初始化内存区域操作对象的算法。

| 算法 | 说明 |
|------|------|
| `uninitialized_copy` | 将对象范围复制到未初始化内存区域（函数模板） |
| `uninitialized_fill` | 将对象复制到由范围定义的未初始化内存区域（函数模板） |
| `uninitialized_move`(C++17) | 将对象范围移动到未初始化内存区域（函数模板） |
| `uninitialized_default_construct`(C++17) | 在未初始化内存区域按默认初始化构造对象（函数模板） |
| `uninitialized_value_construct`(C++17) | 在未初始化内存区域按值初始化构造对象（函数模板） |
| `destroy`(C++17) | 销毁对象范围（函数模板） |
| `construct_at`(C++20) | 在给定地址创建对象（函数模板） |

### 五、杂项工具

| 工具 | 说明 |
|------|------|
| `pointer_traits`(C++11) | 提供指针类类型信息（类模板） |
| `to_address`(C++20) | 从指针类类型获取原始指针（函数模板） |
| `addressof`(C++11) | 获取对象的实际地址，即使`&`运算符被重载（函数模板） |
| `align`(C++11) | 在缓冲区中对齐指针（函数） |

---

## 扩展知识详解

### 一、智能指针深度解析

#### 1. unique_ptr使用示例
```cpp
#include <memory>
#include <iostream>
#include <vector>

class MyClass {
public:
    MyClass(int value) : value_(value) {
        std::cout << "MyClass constructed with value: " << value_ << std::endl;
    }
    
    ~MyClass() {
        std::cout << "MyClass destroyed with value: " << value_ << std::endl;
    }
    
    int getValue() const { return value_; }
    
private:
    int value_;
};

void unique_ptr_examples() {
    // 基本使用
    {
        auto ptr = std::make_unique<MyClass>(42);
        std::cout << "Value: " << ptr->getValue() << std::endl;
        
        // 移动语义
        auto ptr2 = std::move(ptr);
        if (!ptr) {
            std::cout << "ptr is now null" << std::endl;
        }
        std::cout << "Value from ptr2: " << ptr2->getValue() << std::endl;
    }  // 自动销毁
    
    // 自定义删除器
    auto deleter = [](int* p) {
        std::cout << "Custom deleter called for: " << *p << std::endl;
        delete p;
    };
    std::unique_ptr<int, decltype(deleter)> ptr3(new int(100), deleter);
    
    // 容器中的使用
    std::vector<std::unique_ptr<MyClass>> vec;
    vec.push_back(std::make_unique<MyClass>(1));
    vec.push_back(std::make_unique<MyClass>(2));
    vec.push_back(std::make_unique<MyClass>(3));
    
    for (const auto& item : vec) {
        std::cout << "Vector item: " << item->getValue() << std::endl;
    }
}
```

#### 2. shared_ptr和weak_ptr使用示例
```cpp
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

class Node {
public:
    Node(int id) : id_(id) {
        std::cout << "Node " << id_ << " created" << std::endl;
    }
    
    ~Node() {
        std::cout << "Node " << id_ << " destroyed" << std::endl;
    }
    
    void setNext(std::shared_ptr<Node> next) {
        next_ = next;
    }
    
    std::shared_ptr<Node> getNext() const {
        return next_;
    }
    
    int getId() const { return id_; }
    
private:
    int id_;
    std::shared_ptr<Node> next_;
};

void shared_ptr_examples() {
    // 基本使用
    {
        auto node1 = std::make_shared<Node>(1);
        auto node2 = std::make_shared<Node>(2);
        
        node1->setNext(node2);
        std::cout << "node1 use count: " << node1.use_count() << std::endl;
        std::cout << "node2 use count: " << node2.use_count() << std::endl;
        
        // 循环引用问题演示
        node2->setNext(node1);
        std::cout << "After circular reference:" << std::endl;
        std::cout << "node1 use count: " << node1.use_count() << std::endl;
        std::cout << "node2 use count: " << node2.use_count() << std::endl;
    }
    
    // 使用weak_ptr解决循环引用
    {
        struct BetterNode {
            BetterNode(int id) : id_(id) {}
            int id_;
            std::weak_ptr<BetterNode> next_;
        };
        
        auto node1 = std::make_shared<BetterNode>(1);
        auto node2 = std::make_shared<BetterNode>(2);
        
        node1->next_ = node2;
        node2->next_ = node1;  // weak_ptr不会增加引用计数
        
        std::cout << "BetterNode use counts: " << node1.use_count() 
                  << ", " << node2.use_count() << std::endl;
                  
        // 安全访问weak_ptr
        if (auto locked = node1->next_.lock()) {
            std::cout << "Locked node id: " << locked->id_ << std::endl;
        } else {
            std::cout << "Node no longer exists" << std::endl;
        }
    }
}
```

#### 3. enable_shared_from_this使用示例
```cpp
#include <memory>
#include <iostream>

class MyClass : public std::enable_shared_from_this<MyClass> {
public:
    MyClass(int value) : value_(value) {}
    
    std::shared_ptr<MyClass> getSharedPtr() {
        // 安全地从this创建shared_ptr
        return shared_from_this();
    }
    
    void doSomething() {
        std::cout << "Doing something with value: " << value_ << std::endl;
    }
    
private:
    int value_;
};

void enable_shared_from_this_example() {
    auto obj = std::make_shared<MyClass>(42);
    auto shared_copy = obj->getSharedPtr();
    
    std::cout << "Original use count: " << obj.use_count() << std::endl;
    obj->doSomething();
    shared_copy->doSomething();
}
```

### 二、分配器详解

#### 1. 自定义分配器实现
```cpp
#include <memory>
#include <iostream>
#include <vector>

template<typename T>
class TrackingAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    
    template<typename U>
    struct rebind {
        using other = TrackingAllocator<U>;
    };
    
    TrackingAllocator() = default;
    
    template<typename U>
    TrackingAllocator(const TrackingAllocator<U>&) {}
    
    pointer allocate(size_type n) {
        std::cout << "Allocating " << n << " elements of size " 
                  << sizeof(T) << std::endl;
        return static_cast<pointer>(std::malloc(n * sizeof(T)));
    }
    
    void deallocate(pointer p, size_type n) {
        std::cout << "Deallocating " << n << " elements" << std::endl;
        std::free(p);
    }
    
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new(p) U(std::forward<Args>(args)...);
    }
    
    template<typename U>
    void destroy(U* p) {
        p->~U();
    }
    
    template<typename U>
    bool operator==(const TrackingAllocator<U>&) const { return true; }
    
    template<typename U>
    bool operator!=(const TrackingAllocator<U>&) const { return false; }
};

void custom_allocator_example() {
    std::vector<int, TrackingAllocator<int>> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}
```

#### 2. 多态分配器使用（C++17）
```cpp
#include <memory_resource>
#include <vector>
#include <iostream>

void polymorphic_allocator_example() {
    // 使用单调缓冲区资源
    char buffer[1024];
    std::pmr::monotonic_buffer_resource pool{buffer, sizeof(buffer)};
    
    // 使用多态分配器的vector
    std::pmr::vector<int> vec1(&pool);
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);
    
    std::cout << "Vector elements: ";
    for (const auto& item : vec1) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    // 使用池资源
    std::pmr::synchronized_pool_resource pool_resource;
    std::pmr::vector<int> vec2(&pool_resource);
    vec2.push_back(4);
    vec2.push_back(5);
    vec2.push_back(6);
    
    std::cout << "Pool vector elements: ";
    for (const auto& item : vec2) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}
```

### 三、内存资源管理

#### 1. 内存池使用示例
```cpp
#include <memory_resource>
#include <vector>
#include <string>
#include <chrono>
#include <iostream>

void memory_pool_performance_example() {
    const size_t iterations = 10000;
    
    // 标准分配器性能测试
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::string> vec;
        for (size_t i = 0; i < iterations; ++i) {
            vec.push_back("String " + std::to_string(i));
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto std_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 内存池性能测试
    std::pmr::synchronized_pool_resource pool;
    start = std::chrono::high_resolution_clock::now();
    {
        std::pmr::vector<std::pmr::string> vec(&pool);
        for (size_t i = 0; i < iterations; ++i) {
            vec.push_back(std::pmr::string("String " + std::to_string(i), &pool));
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto pool_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Standard allocator time: " << std_duration.count() << "ms" << std::endl;
    std::cout << "Pool allocator time: " << pool_duration.count() << "ms" << std::endl;
}
```

#### 2. 单调缓冲区资源使用
```cpp
#include <memory_resource>
#include <vector>
#include <iostream>

void monotonic_buffer_example() {
    // 预分配缓冲区
    char buffer[1024];
    std::pmr::monotonic_buffer_resource monotonic_pool{buffer, sizeof(buffer)};
    
    // 所有分配都在预分配的缓冲区中进行
    std::pmr::vector<int> vec1(&monotonic_pool);
    std::pmr::vector<int> vec2(&monotonic_pool);
    
    for (int i = 0; i < 100; ++i) {
        vec1.push_back(i);
        vec2.push_back(i * 2);
    }
    
    std::cout << "vec1 size: " << vec1.size() << std::endl;
    std::cout << "vec2 size: " << vec2.size() << std::endl;
    
    // 当作用域结束时，所有分配一次性释放
}  // 缓冲区在此处被释放
```

### 四、未初始化内存操作

#### 1. 未初始化构造和销毁
```cpp
#include <memory>
#include <iostream>

void uninitialized_operations_example() {
    // 分配原始内存
    constexpr size_t count = 5;
    void* raw_memory = std::aligned_alloc(alignof(int), sizeof(int) * count);
    
    if (raw_memory) {
        int* int_array = static_cast<int*>(raw_memory);
        
        // 在未初始化内存中构造对象
        std::uninitialized_value_construct(int_array, int_array + count);
        
        // 使用构造的对象
        for (size_t i = 0; i < count; ++i) {
            int_array[i] = static_cast<int>(i * i);
            std::cout << "Element " << i << ": " << int_array[i] << std::endl;
        }
        
        // 销毁对象
        std::destroy(int_array, int_array + count);
        
        // 释放内存
        std::free(raw_memory);
    }
}

// C++20方式
void construct_at_example() {
    alignas(int) char buffer[sizeof(int)];
    int* int_ptr = reinterpret_cast<int*>(buffer);
    
    // 使用construct_at在指定位置构造对象
    std::construct_at(int_ptr, 42);
    std::cout << "Constructed value: " << *int_ptr << std::endl;
    
    // 销毁对象
    std::destroy_at(int_ptr);
}
```

### 五、现代C++23特性

#### 1. 显式生命周期管理（C++23）
```cpp
#include <memory>
#include <iostream>

// C++23特性示例（需要支持C++23的编译器）
void explicit_lifetime_example() {
    // 以下代码需要C++23支持
    /*
    alignas(int) char buffer[sizeof(int)];
    
    // 隐式创建对象
    auto* int_ptr = std::start_lifetime_as<int>(buffer);
    *int_ptr = 42;
    std::cout << "Value: " << *int_ptr << std::endl;
    
    // 数组版本
    alignas(int) char array_buffer[sizeof(int) * 5];
    auto* array_ptr = std::start_lifetime_as_array<int, 5>(array_buffer);
    for (int i = 0; i < 5; ++i) {
        array_ptr[i] = i * 10;
        std::cout << "Array[" << i << "]: " << array_ptr[i] << std::endl;
    }
    */
    std::cout << "C++23 explicit lifetime management example (requires C++23 compiler support)" << std::endl;
}
```

#### 2. 智能指针适配器（C++23）
```cpp
#include <memory>
#include <iostream>

// C++23 out_ptr和inout_ptr示例
void smart_pointer_adapters_example() {
    /*
    // 示例：与C API交互
    void* c_api_allocate(size_t size) {
        return std::malloc(size);
    }
    
    void c_api_deallocate(void* ptr) {
        std::free(ptr);
    }
    
    // 使用out_ptr与C API交互
    std::unique_ptr<int, decltype([](int* p) { c_api_deallocate(p); })> ptr;
    
    // 假设有一个接受指针指针的C函数
    void c_function_that_allocates(int** out_ptr) {
        *out_ptr = static_cast<int*>(c_api_allocate(sizeof(int)));
        **out_ptr = 42;
    }
    
    c_function_that_allocates(std::out_ptr(ptr));
    std::cout << "Value from C API: " << *ptr << std::endl;
    */
    std::cout << "C++23 smart pointer adapters example (requires C++23 compiler support)" << std::endl;
}
```

### 六、性能优化技巧

#### 1. 内存对齐优化
```cpp
#include <memory>
#include <iostream>

void alignment_examples() {
    // 检查对齐
    int value = 42;
    void* ptr = &value;
    
    if (std::is_aligned(ptr, alignof(int))) {
        std::cout << "Pointer is properly aligned" << std::endl;
    }
    
    // 手动对齐
    alignas(32) char aligned_buffer[64];
    std::cout << "Buffer address: " << reinterpret_cast<uintptr_t>(aligned_buffer) << std::endl;
    std::cout << "32-byte aligned: " << (reinterpret_cast<uintptr_t>(aligned_buffer) % 32 == 0) << std::endl;
    
    // C++20对齐假设
    /*
    int* assumed_aligned_ptr = std::assume_aligned<32>(reinterpret_cast<int*>(aligned_buffer));
    */
}
```

#### 2. 内存泄漏检测
```cpp
#include <memory>
#include <iostream>

// 简单的内存泄漏检测器
class LeakDetector {
private:
    static size_t allocation_count_;
    static size_t deallocation_count_;
    
public:
    static void record_allocation() {
        ++allocation_count_;
    }
    
    static void record_deallocation() {
        ++deallocation_count_;
    }
    
    static void report() {
        std::cout << "Allocations: " << allocation_count_ << std::endl;
        std::cout << "Deallocations: " << deallocation_count_ << std::endl;
        if (allocation_count_ > deallocation_count_) {
            std::cout << "Potential memory leak: " 
                      << (allocation_count_ - deallocation_count_) 
                      << " allocations not freed" << std::endl;
        }
    }
};

size_t LeakDetector::allocation_count_ = 0;
size_t LeakDetector::deallocation_count_ = 0;

// 自定义分配器集成泄漏检测
template<typename T>
class LeakDetectingAllocator {
public:
    using value_type = T;
    using pointer = T*;
    
    pointer allocate(size_t n) {
        LeakDetector::record_allocation();
        return static_cast<pointer>(std::malloc(n * sizeof(T)));
    }
    
    void deallocate(pointer p, size_t n) {
        LeakDetector::record_deallocation();
        std::free(p);
    }
    
    template<typename U>
    bool operator==(const LeakDetectingAllocator<U>&) const { return true; }
    
    template<typename U>
    bool operator!=(const LeakDetectingAllocator<U>&) const { return false; }
};

void leak_detection_example() {
    {
        std::vector<int, LeakDetectingAllocator<int>> vec;
        for (int i = 0; i < 10; ++i) {
            vec.push_back(i);
        }
    }
    
    LeakDetector::report();
}
```

## 最佳实践建议

### 一、智能指针使用准则

1. **优先使用unique_ptr**：对于独占所有权的场景
2. **谨慎使用shared_ptr**：避免循环引用问题
3. **合理使用weak_ptr**：解决循环引用和观察场景
4. **避免裸指针**：除非与C API交互

### 二、分配器使用建议

1. **默认分配器通常足够**：除非有特殊性能需求
2. **内存池适用于频繁分配**：提高性能和减少碎片
3. **多态分配器提供灵活性**：运行时选择内存策略
4. **自定义分配器需谨慎**：确保正确实现所有要求

### 三、内存管理优化

1. **避免频繁小对象分配**：使用对象池或内存池
2. **注意内存对齐**：提升访问性能
3. **及时释放资源**：使用RAII原则
4. **监控内存使用**：检测泄漏和过度分配

## 相关页面

| 页面 | 说明 |
|------|------|
| 新表达式 | 对象创建和内存分配 |
| 删除表达式 | 对象销毁和内存释放 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/memory&oldid=179892>
- 最后修改时间：2025年1月28日 10:10
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++内存管理库为现代C++开发提供了强大的内存管理能力。从智能指针到分配器，从内存资源到未初始化操作，这些工具帮助开发者编写更安全、更高效的代码。正确理解和使用这些特性是掌握现代C++的关键。智能指针消除了大多数内存泄漏问题，分配器提供了灵活的内存管理策略，内存资源优化了性能敏感场景，而未初始化操作为底层编程提供了必要的支持。持续学习和实践这些内存管理技术，结合性能分析和最佳实践，能够构建出高质量的C++应用程序。