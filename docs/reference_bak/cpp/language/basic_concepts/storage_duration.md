# C++ 存储持续时间（Storage Duration）完全详解

  [1. 存储持续时间基本概念](#1-存储持续时间基本概念)

  [2. 存储持续时间类型](#2-存储持续时间类型)

  [3. 静态存储持续时间](#3-静态存储持续时间)

  [4. 线程存储持续时间](#4-线程存储持续时间)

  [5. 自动存储持续时间](#5-自动存储持续时间)

  [6. 动态存储持续时间](#6-动态存储持续时间)

  [7. 链接性（Linkage）](#7-链接性linkage)

  [8. 存储类说明符](#8-存储类说明符)

  [9. 静态块变量](#9-静态块变量)

  [10. 翻译单元局部实体](#10-翻译单元局部实体)

  [11. 最佳实践](#11-最佳实践)

  [12. 标准参考](#12-标准参考)

  [13. 总结](#13-总结)

C++ 存储持续时间是程序中对象生命周期的重要属性，它定义了包含对象的存储的最小潜在生命周期。正确理解和使用存储持续时间对于编写高效、安全的 C++ 程序至关重要。

## 1. 存储持续时间基本概念

### 1.1 存储持续时间定义

存储持续时间是对象的属性，它定义了包含对象的存储的最小潜在生命周期。存储持续时间与对象的作用域一起，控制着对象的两个独立属性：存储持续时间和链接性。

```cpp
#include <iostream>

// 全局对象 - 静态存储持续时间
int global_var = 100;

void storage_duration_basics() {
    // 局部对象 - 自动存储持续时间
    int local_var = 200;
    
    std::cout << "Global var: " << global_var << std::endl;
    std::cout << "Local var: " << local_var << std::endl;
    
    // global_var 的存储持续时间：程序执行期间
    // local_var 的存储持续时间：函数调用期间
}

int main() {
    storage_duration_basics();
    return 0;
}
```

### 1.2 存储持续时间与生命周期的关系

存储持续时间定义了对象存储的最小潜在生命周期，但实际生命周期可能更短：

```cpp
#include <iostream>
#include <memory>

class StorageDurationExample {
private:
    int value;
    
public:
    StorageDurationExample(int v) : value(v) {
        std::cout << "StorageDurationExample(" << value << ") constructed\n";
    }
    
    ~StorageDurationExample() {
        std::cout << "StorageDurationExample(" << value << ") destructed\n";
    }
    
    int getValue() const { return value; }
};

void storage_duration_vs_lifecycle() {
    std::cout << "=== Static storage duration ===\n";
    {
        static StorageDurationExample static_obj(42);  // 静态存储持续时间
        std::cout << "Static object value: " << static_obj.getValue() << "\n";
        // 存储持续时间：程序执行期间
        // 生命周期：程序执行期间（但可能被显式销毁）
    }  // static_obj 仍然存在
    
    std::cout << "\n=== Automatic storage duration ===\n";
    {
        StorageDurationExample auto_obj(100);  // 自动存储持续时间
        std::cout << "Auto object value: " << auto_obj.getValue() << "\n";
        // 存储持续时间：作用域期间
        // 生命周期：作用域期间
    }  // auto_obj 被销毁
    
    std::cout << "\n=== Dynamic storage duration ===\n";
    {
        StorageDurationExample* dynamic_obj = new StorageDurationExample(200);  // 动态存储持续时间
        std::cout << "Dynamic object value: " << dynamic_obj->getValue() << "\n";
        // 存储持续时间：程序执行期间（直到显式释放）
        // 生命周期：直到 delete 调用
        
        delete dynamic_obj;  // 显式销毁
        // 存储被释放，生命周期结束
    }
}
```

## 2. 存储持续时间类型

### 2.1 四种存储持续时间类型

C++ 定义了四种存储持续时间类型：

```cpp
#include <iostream>
#include <thread>
#include <chrono>

// 1. 静态存储持续时间
static int static_var = 100;  // 文件作用域静态变量
int global_var = 200;         // 全局变量

void demonstrate_storage_durations() {
    // 2. 自动存储持续时间
    int auto_var = 300;  // 函数内局部变量
    
    // 3. 线程存储持续时间（C++11）
    thread_local int thread_var = 400;  // 线程局部变量
    
    std::cout << "Static var: " << static_var << std::endl;
    std::cout << "Global var: " << global_var << std::endl;
    std::cout << "Auto var: " << auto_var << std::endl;
    std::cout << "Thread var: " << thread_var << std::endl;
    
    // 4. 动态存储持续时间
    int* dynamic_var = new int(500);  // 动态分配
    std::cout << "Dynamic var: " << *dynamic_var << std::endl;
    delete dynamic_var;  // 释放内存
}

class StorageDurationTypes {
private:
    // 类成员的存储持续时间取决于对象的存储持续时间
    int member_var;
    static int static_member;  // 静态成员变量
    
public:
    StorageDurationTypes(int value) : member_var(value) {
        std::cout << "Member var (" << member_var << ") created\n";
    }
    
    ~StorageDurationTypes() {
        std::cout << "Member var (" << member_var << ") destroyed\n";
    }
    
    void printStorageInfo() {
        std::cout << "Member var: " << member_var << std::endl;
        std::cout << "Static member: " << static_member << std::endl;
    }
};

int StorageDurationTypes::static_member = 600;

void storage_duration_types_example() {
    std::cout << "=== Storage duration types ===\n";
    demonstrate_storage_durations();
    
    std::cout << "\n=== Class member storage ===\n";
    {
        StorageDurationTypes obj(700);  // obj 具有自动存储持续时间
        obj.printStorageInfo();
        // obj 的成员变量具有自动存储持续时间
        // obj 的静态成员具有静态存储持续时间
    }  // obj 及其成员被销毁
}
```

### 2.2 存储持续时间与引用

引用的存储持续时间与其引用的对象相同：

```cpp
#include <iostream>
#include <memory>

class ReferenceStorage {
private:
    int value;
    
public:
    ReferenceStorage(int v) : value(v) {
        std::cout << "ReferenceStorage(" << value << ") constructed\n";
    }
    
    ~ReferenceStorage() {
        std::cout << "ReferenceStorage(" << value << ") destructed\n";
    }
    
    int getValue() const { return value; }
    void setValue(int v) { value = v; }
};

void reference_storage_duration() {
    std::cout << "=== Reference storage duration ===\n";
    {
        ReferenceStorage obj(42);  // 自动存储持续时间
        ReferenceStorage& ref = obj;  // 引用具有相同的存储持续时间
        
        std::cout << "Object value: " << obj.getValue() << "\n";
        std::cout << "Reference value: " << ref.getValue() << "\n";
        
        // ref 和 obj 具有相同的存储持续时间
    }  // obj 和 ref 都被销毁
    
    std::cout << "\n=== Static reference ===\n";
    {
        static ReferenceStorage static_obj(100);  // 静态存储持续时间
        ReferenceStorage& static_ref = static_obj;  // 静态存储持续时间
        
        std::cout << "Static object value: " << static_obj.getValue() << "\n";
        std::cout << "Static reference value: " << static_ref.getValue() << "\n";
    }  // static_obj 和 static_ref 仍然存在
    
    std::cout << "\n=== Dynamic reference ===\n";
    {
        ReferenceStorage* dynamic_obj = new ReferenceStorage(200);  // 动态存储持续时间
        ReferenceStorage& dynamic_ref = *dynamic_obj;  // 动态存储持续时间
        
        std::cout << "Dynamic object value: " << dynamic_obj->getValue() << "\n";
        std::cout << "Dynamic reference value: " << dynamic_ref.getValue() << "\n";
        
        delete dynamic_obj;  // 释放内存，对象和引用都失效
    }
}
```

## 3. 静态存储持续时间

### 3.1 静态存储持续时间的特征

具有静态存储持续时间的对象的存储持续时间为程序持续时间：

```cpp
#include <iostream>

// 全局变量 - 静态存储持续时间
int global_counter = 0;

// 文件作用域静态变量 - 静态存储持续时间
static int file_static_counter = 0;

class StaticStorageExample {
private:
    int value;
    
public:
    StaticStorageExample(int v) : value(v) {
        std::cout << "StaticStorageExample(" << value << ") constructed\n";
    }
    
    ~StaticStorageExample() {
        std::cout << "StaticStorageExample(" << value << ") destructed\n";
    }
    
    static void incrementCounters() {
        global_counter++;
        file_static_counter++;
    }
    
    static void printCounters() {
        std::cout << "Global counter: " << global_counter << "\n";
        std::cout << "File static counter: " << file_static_counter << "\n";
    }
};

// 类静态成员变量 - 静态存储持续时间
int StaticStorageExample::static_member = 0;

void static_storage_demonstration() {
    std::cout << "=== Static storage duration ===\n";
    
    // 第一次调用
    StaticStorageExample::incrementCounters();
    StaticStorageExample::printCounters();
    
    // 第二次调用
    StaticStorageExample::incrementCounters();
    StaticStorageExample::printCounters();
    
    // 创建静态对象
    static StaticStorageExample static_obj(42);  // 静态存储持续时间
    std::cout << "Static object created\n";
    
    // 静态对象在整个程序执行期间都存在
}

// 静态对象在程序结束时被销毁
// 析构函数调用顺序：后创建的先销毁
```

### 3.2 静态对象初始化

静态对象的初始化时机和顺序：

```cpp
#include <iostream>

// 零初始化
int zero_initialized = 0;  // 零初始化
static int static_zero = 0;  // 零初始化

// 常量初始化
const int const_initialized = 42;  // 常量初始化
constexpr int constexpr_initialized = 100;  // 常量初始化

// 动态初始化
int dynamic_initialized = []() {
    std::cout << "Dynamic initialization function called\n";
    return 42;
}();  // 动态初始化

class InitializationExample {
private:
    int value;
    static int instance_count;
    
public:
    InitializationExample(int v) : value(v) {
        instance_count++;
        std::cout << "InitializationExample(" << value << ") #" << instance_count << " constructed\n";
    }
    
    ~InitializationExample() {
        std::cout << "InitializationExample(" << value << ") destructed\n";
    }
    
    int getValue() const { return value; }
};

int InitializationExample::instance_count = 0;

// 静态对象初始化顺序
static InitializationExample first_static(100);
static InitializationExample second_static(200);

void static_initialization_order() {
    std::cout << "=== Static initialization order ===\n";
    std::cout << "First static value: " << first_static.getValue() << "\n";
    std::cout << "Second static value: " << second_static.getValue() << "\n";
    
    // 同一翻译单元内的静态对象按声明顺序初始化
    // 不同翻译单元间的初始化顺序未定义
}

// 函数内静态变量
void function_static_example() {
    static int call_count = 0;  // 只初始化一次
    call_count++;
    
    std::cout << "Function called " << call_count << " times\n";
    
    // 函数内静态变量在第一次执行到声明时初始化
    static InitializationExample function_static(300);
    std::cout << "Function static value: " << function_static.getValue() << "\n";
}
```

## 4. 线程存储持续时间

### 4.1 线程局部存储（C++11）

线程存储持续时间的对象为每个线程维护一个独立的实例：

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// 线程局部变量
thread_local int thread_counter = 0;
thread_local std::string thread_name = "unnamed";

class ThreadLocalExample {
private:
    int value;
    static thread_local int instance_count;
    
public:
    ThreadLocalExample(int v) : value(v) {
        instance_count++;
        std::cout << "ThreadLocalExample(" << value << ") #" << instance_count 
                  << " constructed in thread " << std::this_thread::get_id() << "\n";
    }
    
    ~ThreadLocalExample() {
        std::cout << "ThreadLocalExample(" << value << ") destructed in thread " 
                  << std::this_thread::get_id() << "\n";
    }
    
    void increment() {
        value++;
        std::cout << "Thread " << std::this_thread::get_id() 
                  << ": value = " << value << "\n";
    }
};

thread_local int ThreadLocalExample::instance_count = 0;

// 线程局部静态对象
thread_local ThreadLocalExample thread_local_obj(42);

void thread_function(const std::string& name) {
    thread_name = name;  // 每个线程有自己的 thread_name 副本
    thread_counter = 0;  // 每个线程有自己的 thread_counter 副本
    
    std::cout << "Thread " << name << " started\n";
    
    for (int i = 0; i < 3; i++) {
        thread_counter++;
        std::cout << "Thread " << name << ": counter = " << thread_counter << "\n";
        
        thread_local_obj.increment();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Thread " << name << " finished\n";
}

void thread_local_storage_example() {
    std::cout << "=== Thread local storage ===\n";
    
    std::vector<std::thread> threads;
    
    // 创建多个线程
    for (int i = 0; i < 3; i++) {
        threads.emplace_back(thread_function, "Thread-" + std::to_string(i + 1));
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    // 主线程的线程局部变量
    std::cout << "Main thread counter: " << thread_counter << "\n";
    std::cout << "Main thread name: " << thread_name << "\n";
}
```

### 4.2 线程局部存储的陷阱

```cpp
#include <iostream>
#include <thread>
#include <memory>

class ThreadLocalPitfalls {
private:
    int value;
    
public:
    ThreadLocalPitfalls(int v) : value(v) {
        std::cout << "ThreadLocalPitfalls(" << value << ") constructed in thread " 
                  << std::this_thread::get_id() << "\n";
    }
    
    ~ThreadLocalPitfalls() {
        std::cout << "ThreadLocalPitfalls(" << value << ") destructed in thread " 
                  << std::this_thread::get_id() << "\n";
    }
    
    int getValue() const { return value; }
};

// 线程局部指针
thread_local ThreadLocalPitfalls* thread_local_ptr = nullptr;

void thread_local_pitfalls() {
    std::cout << "=== Thread local pitfalls ===\n";
    
    // 每个线程都有自己的指针副本
    thread_local_ptr = new ThreadLocalPitfalls(100);
    std::cout << "Thread " << std::this_thread::get_id() 
              << " pointer value: " << thread_local_ptr->getValue() << "\n";
    
    // 注意：线程结束时需要手动清理线程局部指针指向的对象
    // 否则会造成内存泄漏
    delete thread_local_ptr;
    thread_local_ptr = nullptr;
}

void demonstrate_thread_local_cleanup() {
    std::thread t1([]() {
        thread_local_pitfalls();
    });
    
    std::thread t2([]() {
        thread_local_pitfalls();
    });
    
    t1.join();
    t2.join();
}
```

## 5. 自动存储持续时间

### 5.1 自动存储持续时间的特征

自动存储持续时间的对象存储持续到它们被创建的块退出时：

```cpp
#include <iostream>

class AutoStorageExample {
private:
    int value;
    
public:
    AutoStorageExample(int v) : value(v) {
        std::cout << "AutoStorageExample(" << value << ") constructed\n";
    }
    
    ~AutoStorageExample() {
        std::cout << "AutoStorageExample(" << value << ") destructed\n";
    }
    
    int getValue() const { return value; }
};

void automatic_storage_example() {
    std::cout << "=== Automatic storage duration ===\n";
    
    {
        AutoStorageExample obj1(100);  // 自动存储持续时间
        std::cout << "obj1 value: " << obj1.getValue() << "\n";
        
        {
            AutoStorageExample obj2(200);  // 嵌套块中的自动存储
            std::cout << "obj2 value: " << obj2.getValue() << "\n";
            
            // obj2 在此处被销毁
        }
        
        std::cout << "obj1 still exists\n";
        // obj1 在此处被销毁
    }
    
    std::cout << "All automatic objects destroyed\n";
}

// 函数参数 - 自动存储持续时间
void function_parameter_example(int param) {  // param 具有自动存储持续时间
    AutoStorageExample local_obj(300);  // local_obj 具有自动存储持续时间
    
    std::cout << "Parameter value: " << param << "\n";
    std::cout << "Local object value: " << local_obj.getValue() << "\n";
    
    // param 和 local_obj 在函数结束时被销毁
}
```

### 5.2 自动存储与 RAII

自动存储持续时间与 RAII（Resource Acquisition Is Initialization）模式：

```cpp
#include <iostream>
#include <fstream>
#include <memory>

class RAIIExample {
private:
    std::string resource_name;
    static int instance_count;
    
public:
    RAIIExample(const std::string& name) : resource_name(name) {
        instance_count++;
        std::cout << "RAIIExample(" << resource_name << ") #" << instance_count << " acquired\n";
    }
    
    ~RAIIExample() {
        std::cout << "RAIIExample(" << resource_name << ") released\n";
        instance_count--;
    }
    
    void useResource() {
        std::cout << "Using resource: " << resource_name << "\n";
    }
};

int RAIIExample::instance_count = 0;

void raii_example() {
    std::cout << "=== RAII with automatic storage ===\n";
    
    {
        RAIIExample resource("database_connection");
        resource.useResource();
        
        // 资源在作用域结束时自动释放
    }
    
    std::cout << "Resource automatically released\n";
}

// 使用智能指针进行 RAII
void smart_pointer_raii() {
    std::cout << "\n=== Smart pointer RAII ===\n";
    
    {
        auto file_ptr = std::make_unique<std::ofstream>("example.txt");
        if (file_ptr->is_open()) {
            *file_ptr << "Hello, RAII!\n";
            std::cout << "File written successfully\n";
        }
        
        // 文件在 unique_ptr 销毁时自动关闭
    }
    
    std::cout << "File automatically closed\n";
}
```

## 6. 动态存储持续时间

### 6.1 动态存储持续时间的特征

动态存储持续时间的对象由 new 表达式创建，由 delete 表达式销毁：

```cpp
#include <iostream>
#include <memory>

class DynamicStorageExample {
private:
    int value;
    static int instance_count;
    
public:
    DynamicStorageExample(int v) : value(v) {
        instance_count++;
        std::cout << "DynamicStorageExample(" << value << ") #" << instance_count << " constructed\n";
    }
    
    ~DynamicStorageExample() {
        std::cout << "DynamicStorageExample(" << value << ") destructed\n";
        instance_count--;
    }
    
    int getValue() const { return value; }
    void setValue(int v) { value = v; }
};

int DynamicStorageExample::instance_count = 0;

void dynamic_storage_example() {
    std::cout << "=== Dynamic storage duration ===\n";
    
    // 使用 new 创建对象
    DynamicStorageExample* ptr1 = new DynamicStorageExample(100);
    std::cout << "ptr1 value: " << ptr1->getValue() << "\n";
    
    // 使用 delete 销毁对象
    delete ptr1;
    ptr1 = nullptr;
    
    // 数组形式
    DynamicStorageExample* array_ptr = new DynamicStorageExample[3]{
        DynamicStorageExample(200),
        DynamicStorageExample(300),
        DynamicStorageExample(400)
    };
    
    std::cout << "Array created\n";
    
    // 销毁数组
    delete[] array_ptr;
    array_ptr = nullptr;
    
    std::cout << "Array destroyed\n";
}
```

### 6.2 智能指针与动态存储

现代 C++ 推荐使用智能指针管理动态存储：

```cpp
#include <iostream>
#include <memory>
#include <vector>

class SmartPointerExample {
private:
    int value;
    
public:
    SmartPointerExample(int v) : value(v) {
        std::cout << "SmartPointerExample(" << value << ") constructed\n";
    }
    
    ~SmartPointerExample() {
        std::cout << "SmartPointerExample(" << value << ") destructed\n";
    }
    
    int getValue() const { return value; }
    void setValue(int v) { value = v; }
};

void smart_pointer_examples() {
    std::cout << "=== Smart pointer management ===\n";
    
    // unique_ptr - 独占所有权
    {
        auto unique_ptr = std::make_unique<SmartPointerExample>(100);
        std::cout << "Unique ptr value: " << unique_ptr->getValue() << "\n";
        
        // 自动销毁
    }
    
    // shared_ptr - 共享所有权
    {
        auto shared_ptr1 = std::make_shared<SmartPointerExample>(200);
        std::cout << "Shared ptr1 value: " << shared_ptr1->getValue() << "\n";
        
        {
            auto shared_ptr2 = shared_ptr1;  // 共享所有权
            std::cout << "Shared ptr2 value: " << shared_ptr2->getValue() << "\n";
            std::cout << "Reference count: " << shared_ptr1.use_count() << "\n";
            
            // shared_ptr2 在此处销毁，但对象仍然存在
        }
        
        std::cout << "Reference count after shared_ptr2 destruction: " 
                  << shared_ptr1.use_count() << "\n";
        
        // shared_ptr1 在此处销毁，对象被释放
    }
    
    // weak_ptr - 弱引用
    {
        auto shared_ptr = std::make_shared<SmartPointerExample>(300);
        std::weak_ptr<SmartPointerExample> weak_ptr = shared_ptr;
        
        std::cout << "Weak ptr expired: " << weak_ptr.expired() << "\n";
        
        if (auto locked_ptr = weak_ptr.lock()) {
            std::cout << "Locked ptr value: " << locked_ptr->getValue() << "\n";
        }
        
        shared_ptr.reset();  // 释放共享指针
        
        std::cout << "Weak ptr expired after shared_ptr reset: " << weak_ptr.expired() << "\n";
    }
}
```

### 6.3 动态存储的陷阱

```cpp
#include <iostream>
#include <memory>

class MemoryManagementPitfalls {
private:
    int value;
    
public:
    MemoryManagementPitfalls(int v) : value(v) {
        std::cout << "MemoryManagementPitfalls(" << value << ") constructed\n";
    }
    
    ~MemoryManagementPitfalls() {
        std::cout << "MemoryManagementPitfalls(" << value << ") destructed\n";
    }
    
    int getValue() const { return value; }
};

void memory_management_pitfalls() {
    std::cout << "=== Memory management pitfalls ===\n";
    
    // 1. 内存泄漏 - 忘记 delete
    {
        MemoryManagementPitfalls* ptr = new MemoryManagementPitfalls(100);
        std::cout << "Object created, value: " << ptr->getValue() << "\n";
        
        // delete ptr;  // 忘记删除 - 内存泄漏
        // 正确做法：使用智能指针或确保 delete 被调用
    }
    
    // 2. 悬空指针 - 访问已删除的对象
    {
        MemoryManagementPitfalls* ptr = new MemoryManagementPitfalls(200);
        delete ptr;
        ptr = nullptr;
        
        // if (ptr) {  // 检查指针
        //     std::cout << ptr->getValue() << "\n";  // 危险！访问已删除的对象
        // }
    }
    
    // 3. 双重删除
    {
        MemoryManagementPitfalls* ptr = new MemoryManagementPitfalls(300);
        delete ptr;
        // delete ptr;  // 双重删除 - 未定义行为
        ptr = nullptr;
    }
    
    // 4. 数组删除错误
    {
        MemoryManagementPitfalls* array_ptr = new MemoryManagementPitfalls[3]{
            MemoryManagementPitfalls(400),
            MemoryManagementPitfalls(500),
            MemoryManagementPitfalls(600)
        };
        
        // delete array_ptr;  // 错误！应该使用 delete[]
        delete[] array_ptr;  // 正确
        array_ptr = nullptr;
    }
}
```

## 7. 链接性（Linkage）

### 7.1 链接性类型

C++ 中的名称可以具有不同的链接性：

```cpp
#include <iostream>

// 1. 外部链接 - 可以在其他翻译单元中访问
int external_var = 100;
void external_function() {
    std::cout << "External function called\n";
}

// 2. 内部链接 - 仅在同一翻译单元中可访问
static int internal_var = 200;
static void internal_function() {
    std::cout << "Internal function called\n";
}

// 3. 无链接 - 仅在声明的作用域中可访问
void no_linkage_example() {
    int local_var = 300;  // 无链接
    static int static_local = 400;  // 无链接（内部链接）
    
    class LocalClass {  // 无链接
    public:
        static void local_function() {
            std::cout << "Local class function called\n";
        }
    };
    
    std::cout << "Local var: " << local_var << "\n";
    std::cout << "Static local: " << static_local << "\n";
    LocalClass::local_function();
}

// 4. const 变量的链接性（C++ 特性）
const int const_external = 500;  // 默认内部链接（C++）
// extern const int const_external = 500;  // 显式外部链接

void linkage_examples() {
    std::cout << "=== Linkage examples ===\n";
    
    std::cout << "External var: " << external_var << "\n";
    external_function();
    
    std::cout << "Internal var: " << internal_var << "\n";
    internal_function();
    
    no_linkage_example();
    
    std::cout << "Const external: " << const_external << "\n";
}
```

### 7.2 无名命名空间

无名命名空间中的名称具有内部链接：

```cpp
#include <iostream>

// 无名命名空间 - 其中所有名称都具有内部链接
namespace {
    int anonymous_var = 100;
    
    class AnonymousClass {
    public:
        static void anonymous_function() {
            std::cout << "Anonymous function called\n";
        }
    };
    
    // 即使显式声明为 extern，也具有内部链接
    extern int explicit_extern = 200;
}

void anonymous_namespace_example() {
    std::cout << "=== Anonymous namespace ===\n";
    
    std::cout << "Anonymous var: " << anonymous_var << "\n";
    AnonymousClass::anonymous_function();
    std::cout << "Explicit extern: " << explicit_extern << "\n";
    
    // 这些名称不能在其他翻译单元中访问
}
```

### 7.3 模块链接性（C++20）

C++20 引入了模块链接性：

```cpp
// module_example.cpp
// export module my_module;
// 
// export int module_var = 300;
// 
// export void module_function() {
//     std::cout << "Module function called\n";
// }

// main.cpp
// import my_module;
// 
// void module_linkage_example() {
//     std::cout << "Module var: " << module_var << "\n";
//     module_function();
// }
```

## 8. 存储类说明符

### 8.1 存储类说明符详解

C++ 提供了多个存储类说明符：

```cpp
#include <iostream>

// auto - 类型推导（C++11 后不再是存储类说明符）
void auto_example() {
    auto value = 42;  // int
    auto text = "Hello";  // const char*
    
    std::cout << "Auto value: " << value << "\n";
    std::cout << "Auto text: " << text << "\n";
}

// register - 提示存储在寄存器中（C++17 已弃用）
void register_example() {
    // register int reg_var = 100;  // C++17 前可用，现在已弃用
    int reg_var = 100;  // 现代做法
    
    std::cout << "Register var: " << reg_var << "\n";
}

// static - 静态存储持续时间
static int static_var = 200;  // 文件作用域静态变量

void static_example() {
    static int function_static = 300;  // 函数内静态变量
    function_static++;
    
    std::cout << "Function static: " << function_static << "\n";
    std::cout << "File static: " << static_var << "\n";
}

// thread_local - 线程存储持续时间（C++11）
thread_local int thread_local_var = 400;

void thread_local_example() {
    thread_local int function_thread_local = 500;
    function_thread_local++;
    
    std::cout << "Thread local: " << thread_local_var << "\n";
    std::cout << "Function thread local: " << function_thread_local << "\n";
}

// extern - 外部链接
extern int external_var;  // 声明在其他地方定义

void extern_example() {
    // std::cout << "External var: " << external_var << "\n";  // 需要定义
}

// mutable - 允许修改 const 对象的成员
class MutableExample {
private:
    mutable int cache_value;
    int real_value;
    
public:
    MutableExample(int value) : cache_value(0), real_value(value) {}
    
    int getValue() const {
        if (cache_value == 0) {
            cache_value = real_value * 2;  // mutable 允许在 const 函数中修改
        }
        return cache_value;
    }
    
    void resetCache() const {
        cache_value = 0;  // mutable 成员可以在 const 函数中修改
    }
};

void mutable_example() {
    std::cout << "=== Mutable example ===\n";
    
    const MutableExample obj(42);
    std::cout << "First call: " << obj.getValue() << "\n";
    std::cout << "Second call: " << obj.getValue() << "\n";
    
    obj.resetCache();
    std::cout << "After reset: " << obj.getValue() << "\n";
}

void storage_class_specifiers() {
    std::cout << "=== Storage class specifiers ===\n";
    
    auto_example();
    register_example();
    static_example();
    thread_local_example();
    // extern_example();  // 需要外部定义
    mutable_example();
}
```

### 8.2 存储类说明符组合

```cpp
#include <iostream>

// static 和 thread_local 可以组合使用
static thread_local int static_thread_local_var = 100;

// extern 和 thread_local 可以组合使用
extern thread_local int extern_thread_local_var;

void storage_class_combinations() {
    std::cout << "=== Storage class combinations ===\n";
    
    // static thread_local - 每个线程一个静态变量
    static_thread_local_var++;
    std::cout << "Static thread local: " << static_thread_local_var << "\n";
    
    // extern thread_local - 声明外部线程局部变量
    // std::cout << "Extern thread local: " << extern_thread_local_var << "\n";
}
```

## 9. 静态块变量

### 9.1 静态块变量初始化

具有静态存储持续时间的块变量在控制第一次通过其声明时被初始化：

```cpp
#include <iostream>
#include <thread>
#include <chrono>

class StaticBlockVariable {
private:
    int value;
    static int instance_count;
    
public:
    StaticBlockVariable(int v) : value(v) {
        instance_count++;
        std::cout << "StaticBlockVariable(" << value << ") #" << instance_count << " constructed\n";
    }
    
    ~StaticBlockVariable() {
        std::cout << "StaticBlockVariable(" << value << ") destructed\n";
    }
    
    int getValue() const { return value; }
};

int StaticBlockVariable::instance_count = 0;

void static_block_variable_example() {
    std::cout << "=== Static block variable ===\n";
    
    for (int i = 0; i < 3; i++) {
        // 静态块变量只初始化一次
        static StaticBlockVariable static_obj(i * 100);
        std::cout << "Iteration " << i << ", static object value: " << static_obj.getValue() << "\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Static object still exists after loop\n";
}

// 零初始化和常量初始化的静态变量
void zero_and_constant_initialization() {
    std::cout << "\n=== Zero and constant initialization ===\n";
    
    // 零初始化 - 可能在程序启动前完成
    static int zero_init = 0;
    
    // 常量初始化 - 可能在程序启动前完成
    static const int const_init = 42;
    static constexpr int constexpr_init = 100;
    
    std::cout << "Zero init: " << zero_init << "\n";
    std::cout << "Const init: " << const_init << "\n";
    std::cout << "Constexpr init: " << constexpr_init << "\n";
    
    zero_init++;  // 可以修改
    std::cout << "Modified zero init: " << zero_init << "\n";
}
```

### 9.2 线程安全的静态初始化

C++11 保证静态局部变量的线程安全初始化：

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class ThreadSafeStatic {
private:
    int thread_id;
    static std::mutex creation_mutex;
    static int creation_count;
    
public:
    ThreadSafeStatic(int id) : thread_id(id) {
        std::lock_guard<std::mutex> lock(creation_mutex);
        creation_count++;
        std::cout << "ThreadSafeStatic created by thread " << thread_id 
                  << " (creation #" << creation_count << ")\n";
    }
    
    ~ThreadSafeStatic() {
        std::cout << "ThreadSafeStatic destroyed by thread " << thread_id << "\n";
    }
    
    void useObject() {
        std::cout << "Using ThreadSafeStatic in thread " << thread_id << "\n";
    }
};

std::mutex ThreadSafeStatic::creation_mutex;
int ThreadSafeStatic::creation_count = 0;

void thread_safe_static_function(int thread_id) {
    // 静态局部变量的线程安全初始化
    static ThreadSafeStatic static_obj(thread_id);
    static_obj.useObject();
}

void thread_safe_static_initialization() {
    std::cout << "=== Thread-safe static initialization ===\n";
    
    std::vector<std::thread> threads;
    
    // 创建多个线程同时访问静态局部变量
    for (int i = 0; i < 5; i++) {
        threads.emplace_back(thread_safe_static_function, i);
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "All threads completed\n";
}
```

## 10. 翻译单元局部实体

### 10.1 翻译单元局部实体概念

翻译单元局部实体是仅在单个翻译单元中可见的实体：

```cpp
#include <iostream>

// 翻译单元局部实体示例

// 1. 具有内部链接的名称
static int tu_local_var = 100;  // 翻译单元局部

namespace {
    int anonymous_namespace_var = 200;  // 翻译单元局部
}

// 2. 无链接的名称
void tu_local_function() {
    int local_var = 300;  // 翻译单元局部
    
    class LocalClass {  // 翻译单元局部
    public:
        static void local_method() {
            std::cout << "Local class method\n";
        }
    };
    
    LocalClass::local_method();
}

// 3. 模板中的翻译单元局部实体
template<typename T>
class TemplateWithLocal {
private:
    static int template_static;  // 每个翻译单元一个实例
    
public:
    static void printStatic() {
        std::cout << "Template static: " << template_static << "\n";
        template_static++;
    }
};

template<typename T>
int TemplateWithLocal<T>::template_static = 0;

void translation_unit_local_example() {
    std::cout << "=== Translation unit local entities ===\n";
    
    std::cout << "TU local var: " << tu_local_var << "\n";
    std::cout << "Anonymous namespace var: " << anonymous_namespace_var << "\n";
    
    tu_local_function();
    
    TemplateWithLocal<int>::printStatic();
    TemplateWithLocal<double>::printStatic();
}
```

### 10.2 翻译单元局部实体的限制

```cpp
#include <iostream>

// 翻译单元局部实体的使用限制

// 具有内部链接的类型
namespace {
    struct LocalType {
        int value;
        LocalType(int v) : value(v) {}
    };
}

// 这些类型不能在其他翻译单元中使用作为外部链接函数的参数类型
// void external_function(LocalType param);  // 可能违反 ODR

void tu_local_restrictions() {
    std::cout << "=== Translation unit local restrictions ===\n";
    
    LocalType local_obj(42);
    std::cout << "Local type value: " << local_obj.value << "\n";
    
    // LocalType 不能安全地传递给其他翻译单元的函数
}
```

## 11. 最佳实践

### 11.1 存储持续时间管理最佳实践

```cpp
#include <iostream>
#include <memory>
#include <vector>

class BestPracticeExample {
private:
    int value;
    
public:
    BestPracticeExample(int v) : value(v) {
        std::cout << "BestPracticeExample(" << value << ") constructed\n";
    }
    
    ~BestPracticeExample() {
        std::cout << "BestPracticeExample(" << value << ") destructed\n";
    }
    
    int getValue() const { return value; }
};

// 1. 优先使用自动存储和 RAII
void automatic_storage_best_practice() {
    std::cout << "=== Automatic storage best practice ===\n";
    
    // 直接在栈上创建对象
    BestPracticeExample obj(100);
    std::cout << "Object value: " << obj.getValue() << "\n";
    
    // 资源自动管理
}

// 2. 使用智能指针管理动态存储
void smart_pointer_best_practice() {
    std::cout << "\n=== Smart pointer best practice ===\n";
    
    // 使用 make_unique 创建对象
    auto unique_obj = std::make_unique<BestPracticeExample>(200);
    std::cout << "Unique object value: " << unique_obj->getValue() << "\n";
    
    // 使用 make_shared 创建共享对象
    auto shared_obj = std::make_shared<BestPracticeExample>(300);
    std::cout << "Shared object value: " << shared_obj->getValue() << "\n";
    
    // 自动资源管理
}

// 3. 合理使用静态存储
class StaticBestPractice {
private:
    static int instance_count;
    
public:
    StaticBestPractice() {
        instance_count++;
        std::cout << "StaticBestPractice instance #" << instance_count << " created\n";
    }
    
    ~StaticBestPractice() {
        std::cout << "StaticBestPractice instance destroyed\n";
    }
    
    static int getInstanceCount() {
        return instance_count;
    }
};

int StaticBestPractice::instance_count = 0;

void static_storage_best_practice() {
    std::cout << "\n=== Static storage best practice ===\n";
    
    // 函数内静态变量 - 只初始化一次
    static StaticBestPractice static_obj;
    std::cout << "Instance count: " << StaticBestPractice::getInstanceCount() << "\n";
    
    // 避免全局变量，优先使用静态成员或函数内静态变量
}

// 4. 线程局部存储的正确使用
thread_local int thread_local_counter = 0;

void thread_local_best_practice() {
    std::cout << "\n=== Thread local best practice ===\n";
    
    thread_local_counter++;
    std::cout << "Thread local counter: " << thread_local_counter << "\n";
    
    // 确保线程结束时清理资源
    // 可以使用线程局部析构函数或 RAII
}
```

### 11.2 避免常见错误

```cpp
#include <iostream>
#include <memory>

class CommonMistakes {
private:
    int* data;
    size_t size;
    
public:
    // 正确的构造函数
    CommonMistakes(size_t s) : size(s) {
        data = new int[size];
        std::cout << "CommonMistakes constructed with size " << size << "\n";
    }
    
    // 正确的析构函数
    ~CommonMistakes() {
        delete[] data;
        std::cout << "CommonMistakes destructed\n";
    }
    
    // 正确的拷贝构造函数（深拷贝）
    CommonMistakes(const CommonMistakes& other) : size(other.size) {
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        std::cout << "CommonMistakes copy constructed\n";
    }
    
    // 正确的移动构造函数
    CommonMistakes(CommonMistakes&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        std::cout << "CommonMistakes move constructed\n";
    }
    
    // 正确的拷贝赋值运算符
    CommonMistakes& operator=(const CommonMistakes& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
            std::cout << "CommonMistakes copy assigned\n";
        }
        return *this;
    }
    
    // 正确的移动赋值运算符
    CommonMistakes& operator=(CommonMistakes&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            std::cout << "CommonMistakes move assigned\n";
        }
        return *this;
    }
};

void avoid_common_mistakes() {
    std::cout << "=== Avoiding common mistakes ===\n";
    
    // 1. 使用智能指针避免手动内存管理
    {
        auto smart_ptr = std::make_unique<CommonMistakes>(10);
        // 自动清理，无需担心内存泄漏
    }
    
    // 2. 避免悬空指针
    {
        auto ptr = std::make_unique<int>(42);
        // int* dangling = ptr.get();  // 避免保存裸指针
        // ptr.reset();
        // *dangling;  // 危险！
    }
    
    // 3. 遵循 Rule of Five
    CommonMistakes obj1(5);
    CommonMistakes obj2 = obj1;  // 拷贝构造
    CommonMistakes obj3(10);
    obj3 = obj1;  // 拷贝赋值
    CommonMistakes obj4 = std::move(obj1);  // 移动构造
    CommonMistakes obj5(15);
    obj5 = std::move(obj2);  // 移动赋值
}
```

## 12. 标准参考

C++ 存储持续时间的相关标准条款：

- C++23 标准（ISO/IEC 14882:2024）：6.7.5 存储持续时间 [basic.stc]
- C++20 标准（ISO/IEC 14882:2020）：6.7.5 存储持续时间 [basic.stc]
- C++17 标准（ISO/IEC 14882:2017）：6.7 存储持续时间 [basic.stc]
- C++14 标准（ISO/IEC 14882:2014）：3.7 存储持续时间 [basic.stc]
- C++11 标准（ISO/IEC 14882:2011）：3.7 存储持续时间 [basic.stc]
- C++03 标准（ISO/IEC 14882:2003）： 3.7 存储持续时间 [basic.stc]
- C++98 标准（ISO/IEC 14882:1998）：3.7 存储持续时间 [basic.stc]

相关的标准参考还包括：

- 6.6 名称解析 [basic.lookup]
- 11 类 [class]
- 12 特殊成员函数 [special]
- 15 异常处理 [except]
- 20 动态内存管理 [support.dynamic]

## 13. 总结

C++ 存储持续时间是程序设计中的核心概念，它通过控制对象存储的生命周期来帮助管理内存和资源。深入理解存储持续时间机制对于编写安全、高效的 C++ 程序至关重要。

### 13.1 核心要点回顾

**四种存储持续时间类型**：

- 静态存储持续时间：程序执行期间
- 线程存储持续时间：线程执行期间
- 自动存储持续时间：作用域期间
- 动态存储持续时间：显式分配和释放期间

**三种链接性**：

- 外部链接：可在其他翻译单元访问
- 内部链接：仅在同一翻译单元访问
- 无链接：仅在声明作用域访问

**存储类说明符**：

- `static`：静态存储持续时间
- `thread_local`：线程存储持续时间
- `extern`：外部链接
- `mutable`：允许修改 const 对象成员

**现代 C++ 特性**：

- 智能指针自动管理动态存储
- 线程安全的静态局部变量初始化
- RAII 模式确保资源正确释放

### 13.2 实践建议

- 优先使用自动存储：栈上分配更安全、更高效
- 使用智能指针：避免手动内存管理的错误
- 遵循 RAII 原则：资源获取即初始化
- 合理使用静态存储：避免全局变量，优先使用静态成员
- 正确使用线程局部存储：注意线程安全和资源清理

### 13.3 历史演进

C++ 存储持续时间机制在不同标准版本中持续演进：

- C++98/03：建立了基本的存储持续时间规则
- C++11：引入线程局部存储、移动语义、智能指针
- C++14：完善模板和变量链接性规则
- C++17：弃用 register，完善存储类说明符
- C++20：引入模块，标准化翻译单元局部实体
- C++23：进一步完善存储持续时间相关规则

通过深入理解和正确应用存储持续时间概念，开发者可以编写出更加安全、可靠和高效的 C++ 程序，避免常见的内存管理和资源控制错误，提高代码的质量和可维护性。
