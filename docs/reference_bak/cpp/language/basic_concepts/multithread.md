# C++ 多线程执行和数据竞争详解

  [1. 基础概念](#1-基础概念)
  
  [2. 执行线程](#2-执行线程)
  
  [3. 数据竞争](#3-数据竞争)
  
  [4. 内存顺序](#4-内存顺序)
  
  [5. 向前推进](#5-向前推进)
  
  [6. 进度保证](#6-进度保证)
  
  [7. 平凡的无限循环](#7-平凡的无限循环)
  
  [8. 并发前进保证](#8-并发前进保证)
  
  [9. 实践示例](#9-实践示例)
  
  [10. 标准参考](#10-标准参考)
  
  [11. 总结](#11-总结)

## 1. 基础概念

### 1.1 并发与并行

在深入理解C++多线程之前，需要先理解并发和并行的基本概念：

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void concurrency_vs_parallelism() {
    std::cout << "Concurrency vs Parallelism:" << std::endl;
    
    // 并发 (Concurrency) - 多个任务交替执行
    std::cout << "  === Concurrency ===" << std::endl;
    std::cout << "    多个任务在同一个处理器核心上交替执行" << std::endl;
    std::cout << "    通过时间片轮转实现任务切换" << std::endl;
    std::cout << "    适用于I/O密集型任务" << std::endl;
    
    auto concurrent_task = [](int id) {
        for (int i = 0; i < 3; ++i) {
            std::cout << "    Concurrent task " << id << " step " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };
    
    std::thread t1(concurrent_task, 1);
    std::thread t2(concurrent_task, 2);
    
    t1.join();
    t2.join();
    
    // 并行 (Parallelism) - 多个任务同时执行
    std::cout << "  === Parallelism ===" << std::endl;
    std::cout << "    多个任务在多个处理器核心上同时执行" << std::endl;
    std::cout << "    需要多核处理器支持" << std::endl;
    std::cout << "    适用于CPU密集型任务" << std::endl;
    
    auto parallel_task = [](int id) {
        // CPU密集型计算
        volatile long sum = 0;
        for (long i = 0; i < 1000000; ++i) {
            sum += i;
        }
        std::cout << "    Parallel task " << id << " completed with sum: " << sum << std::endl;
    };
    
    auto start = std::chrono::high_resolution_clock::now();
    std::thread p1(parallel_task, 1);
    std::thread p2(parallel_task, 2);
    
    p1.join();
    p2.join();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "    Parallel tasks completed in: " << duration.count() << " ms" << std::endl;
}
```

### 1.2 线程与进程

线程和进程是操作系统中的基本概念，理解它们的区别对多线程编程很重要：

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void threads_vs_processes() {
    std::cout << "Threads vs Processes:" << std::endl;
    
    // 进程 (Process)
    std::cout << "  === Process ===" << std::endl;
    std::cout << "    独立的执行环境" << std::endl;
    std::cout << "    拥有独立的内存空间" << std::endl;
    std::cout << "    进程间通信需要特殊机制" << std::endl;
    std::cout << "    创建和切换开销较大" << std::endl;
    
    // 线程 (Thread)
    std::cout << "  === Thread ===" << std::endl;
    std::cout << "    进程内的执行单元" << std::endl;
    std::cout << "    共享进程的内存空间" << std::endl;
    std::cout << "    线程间可以直接访问共享数据" << std::endl;
    std::cout << "    创建和切换开销较小" << std::endl;
    
    // C++线程示例
    auto thread_function = [](int id) {
        std::cout << "    Thread " << id << " started in process " << getpid() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "    Thread " << id << " finished" << std::endl;
    };
    
    std::cout << "    Main thread in process " << getpid() << std::endl;
    
    std::thread t1(thread_function, 1);
    std::thread t2(thread_function, 2);
    
    t1.join();
    t2.join();
}
```

### 1.3 存储持续时间

理解不同存储持续时间对多线程编程的影响：

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void storage_duration() {
    std::cout << "Storage Duration:" << std::endl;
    
    // 静态存储持续时间
    std::cout << "  === Static Storage Duration ===" << std::endl;
    static int static_var = 0;
    std::cout << "    Static variable address: " << &static_var << std::endl;
    
    // 线程局部存储持续时间
    std::cout << "  === Thread Local Storage Duration ===" << std::endl;
    thread_local int thread_local_var = 0;
    std::cout << "    Thread local variable address: " << &thread_local_var << std::endl;
    
    auto thread_function = []() {
        thread_local int local_thread_var = 0;
        std::cout << "    Thread local in thread: " << &local_thread_var << std::endl;
    };
    
    std::thread t1(thread_function);
    std::thread t2(thread_function);
    
    t1.join();
    t2.join();
    
    // 自动存储持续时间
    std::cout << "  === Automatic Storage Duration ===" << std::endl;
    int automatic_var = 0;
    std::cout << "    Automatic variable address: " << &automatic_var << std::endl;
    
    // 动态存储持续时间
    std::cout << "  === Dynamic Storage Duration ===" << std::endl;
    int* dynamic_var = new int(42);
    std::cout << "    Dynamic variable address: " << dynamic_var << std::endl;
    delete dynamic_var;
}
```

### 1.4 原子操作基础

原子操作是避免数据竞争的重要工具：

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

void atomic_operations_basics() {
    std::cout << "Atomic Operations Basics:" << std::endl;
    
    // 原子类型
    std::cout << "  === Atomic Types ===" << std::endl;
    std::atomic<int> atomic_int{0};
    std::atomic<bool> atomic_bool{false};
    std::atomic<void*> atomic_ptr{nullptr};
    
    std::cout << "    Atomic int is lock-free: " << atomic_int.is_lock_free() << std::endl;
    std::cout << "    Atomic bool is lock-free: " << atomic_bool.is_lock_free() << std::endl;
    
    // 基本原子操作
    std::cout << "  === Basic Atomic Operations ===" << std::endl;
    
    // store/load操作
    atomic_int.store(42);
    int value = atomic_int.load();
    std::cout << "    Stored 42, loaded: " << value << std::endl;
    
    // exchange操作
    int old_value = atomic_int.exchange(100);
    std::cout << "    Exchanged: old=" << old_value << ", new=" << atomic_int.load() << std::endl;
    
    // compare_exchange操作
    int expected = 100;
    bool success = atomic_int.compare_exchange_strong(expected, 200);
    std::cout << "    Compare-exchange: success=" << success << ", value=" << atomic_int.load() 
              << ", expected=" << expected << std::endl;
    
    // 原子运算操作
    std::cout << "  === Atomic Arithmetic Operations ===" << std::endl;
    std::atomic<int> counter{0};
    
    auto incrementer = [&counter]() {
        for (int i = 0; i < 10000; ++i) {
            counter.fetch_add(1);  // 原子增加
        }
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(incrementer);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "    Final counter value: " << counter.load() << std::endl;
}
```

### 1.5 内存模型基础

内存模型定义了程序如何访问和操作内存：

```cpp
#include <iostream>
#include <thread>
#include <atomic>

void memory_model_basics() {
    std::cout << "Memory Model Basics:" << std::endl;
    
    // 内存位置概念
    std::cout << "  === Memory Locations ===" << std::endl;
    int scalar_var = 0;  // 一个内存位置
    struct BitField {
        int a : 4;       // 同一内存位置的一部分
        int b : 4;       // 同一内存位置的另一部分
        int c : 8;       // 另一个内存位置
    } bitfield_var;      // 包含两个内存位置
    
    std::cout << "    Scalar variable address: " << &scalar_var << std::endl;
    std::cout << "    Bitfield variable address: " << &bitfield_var << std::endl;
    
    // Happens-before关系
    std::cout << "  === Happens-Before Relationship ===" << std::endl;
    std::atomic<bool> flag{false};
    int data = 0;
    
    auto writer = [&flag, &data]() {
        data = 42;  // 写入数据
        flag.store(true, std::memory_order_release);  // 发布操作
        std::cout << "    Writer finished" << std::endl;
    };
    
    auto reader = [&flag, &data]() {
        while (!flag.load(std::memory_order_acquire)) {  // 获取操作
            // 等待
        }
        std::cout << "    Reader got data: " << data << std::endl;
    };
    
    std::thread t1(writer);
    std::thread t2(reader);
    
    t1.join();
    t2.join();
}
```

### 1.6 同步原语

同步原语是实现线程间协调的基础：

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

void synchronization_primitives() {
    std::cout << "Synchronization Primitives:" << std::endl;
    
    // 互斥锁 (Mutex)
    std::cout << "  === Mutex ===" << std::endl;
    std::mutex mtx;
    int shared_data = 0;
    
    auto mutex_protected = [&mtx, &shared_data](int id) {
        std::lock_guard<std::mutex> lock(mtx);
        ++shared_data;
        std::cout << "    Thread " << id << " incremented shared_data to " << shared_data << std::endl;
    };
    
    std::thread t1(mutex_protected, 1);
    std::thread t2(mutex_protected, 2);
    
    t1.join();
    t2.join();
    
    // 条件变量 (Condition Variable)
    std::cout << "  === Condition Variable ===" << std::endl;
    std::mutex cv_mutex;
    std::condition_variable cv;
    bool ready = false;
    
    auto waiter = [&cv_mutex, &cv, &ready]() {
        std::unique_lock<std::mutex> lock(cv_mutex);
        cv.wait(lock, [&ready] { return ready; });
        std::cout << "    Waiter thread notified" << std::endl;
    };
    
    auto notifier = [&cv_mutex, &cv, &ready]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        {
            std::lock_guard<std::mutex> lock(cv_mutex);
            ready = true;
        }
        cv.notify_one();
        std::cout << "    Notifier thread sent notification" << std::endl;
    };
    
    std::thread t3(waiter);
    std::thread t4(notifier);
    
    t3.join();
    t4.join();
}
```

## 2. 执行线程

### 2.1 线程创建和管理

执行线程是程序中控制流的一个独立流程：

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void thread_creation_and_management() {
    std::cout << "Thread Creation and Management:" << std::endl;
    
    // 基本线程创建
    std::cout << "  === Basic Thread Creation ===" << std::endl;
    
    auto simple_function = [](int id) {
        std::cout << "    Thread " << id << " started" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "    Thread " << id << " finished" << std::endl;
    };
    
    std::thread t1(simple_function, 1);
    std::thread t2(simple_function, 2);
    
    t1.join();
    t2.join();
    
    // 线程标识符
    std::cout << "  === Thread Identifiers ===" << std::endl;
    
    auto print_id = []() {
        std::cout << "    Current thread ID: " << std::this_thread::get_id() << std::endl;
    };
    
    std::thread t3(print_id);
    print_id();  // 主线程ID
    t3.join();
    
    // 线程可连接性
    std::cout << "  === Thread Joinability ===" << std::endl;
    
    std::thread t4([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    });
    
    std::cout << "    Thread t4 joinable: " << std::boolalpha << t4.joinable() << std::endl;
    t4.join();
    std::cout << "    Thread t4 joinable after join: " << std::boolalpha << t4.joinable() << std::endl;
    
    // 线程分离
    std::cout << "  === Thread Detachment ===" << std::endl;
    
    std::thread t5([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "    Detached thread completed" << std::endl;
    });
    
    std::cout << "    Thread t5 joinable before detach: " << std::boolalpha << t5.joinable() << std::endl;
    t5.detach();
    std::cout << "    Thread t5 joinable after detach: " << std::boolalpha << t5.joinable() << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));  // 等待分离线程完成
}
```

### 2.2 线程间对象访问

任何线程都可能访问程序中的任何对象和函数：

```cpp
#include <iostream>
#include <thread>
#include <memory>

void inter_thread_object_access() {
    std::cout << "Inter-Thread Object Access:" << std::endl;
    
    // 全局对象访问
    std::cout << "  === Global Object Access ===" << std::endl;
    static int global_counter = 0;
    
    auto access_global = []() {
        global_counter += 10;
        std::cout << "    Thread accessed global counter: " << global_counter << std::endl;
    };
    
    std::thread t1(access_global);
    std::thread t2(access_global);
    
    t1.join();
    t2.join();
    
    // 通过指针或引用访问自动存储对象
    std::cout << "  === Pointer/Reference Access ===" << std::endl;
    
    struct SharedData {
        int value = 0;
        std::string message = "Hello";
    };
    
    SharedData shared_obj;
    
    auto access_via_pointer = [](SharedData* data) {
        data->value += 100;
        data->message += " World";
        std::cout << "    Accessed via pointer: " << data->value << ", " << data->message << std::endl;
    };
    
    auto access_via_reference = [](SharedData& data) {
        data.value += 50;
        data.message += " from Thread";
        std::cout << "    Accessed via reference: " << data.value << ", " << data.message << std::endl;
    };
    
    std::thread t3(access_via_pointer, &shared_obj);
    std::thread t4(access_via_reference, std::ref(shared_obj));
    
    t3.join();
    t4.join();
    
    std::cout << "    Final shared object: " << shared_obj.value << ", " << shared_obj.message << std::endl;
    
    // 线程局部存储访问
    std::cout << "  === Thread Local Storage ===" << std::endl;
    
    thread_local int tls_counter = 0;
    
    auto use_tls = [](int thread_id) {
        for (int i = 0; i < 3; ++i) {
            ++tls_counter;
            std::cout << "    Thread " << thread_id << " TLS counter: " << tls_counter << std::endl;
        }
    };
    
    std::thread t5(use_tls, 1);
    std::thread t6(use_tls, 2);
    
    t5.join();
    t6.join();
    
    // 主线程的TLS值
    std::cout << "    Main thread TLS counter: " << tls_counter << std::endl;
}
```

### 2.3 线程创建方式

C++提供了多种创建线程的方式：

```cpp
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

void thread_creation_methods() {
    std::cout << "Thread Creation Methods:" << std::endl;
    
    // std::thread
    std::cout << "  === std::thread ===" << std::endl;
    
    auto thread_func = [](const std::string& name) {
        std::cout << "    " << name << " thread started" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "    " << name << " thread finished" << std::endl;
    };
    
    std::thread t1(thread_func, "BasicThread");
    t1.join();
    
    // std::async
    std::cout << "  === std::async ===" << std::endl;
    
    auto async_func = [](int x, int y) -> int {
        std::cout << "    Async task with " << x << " and " << y << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return x + y;
    };
    
    auto future = std::async(std::launch::async, async_func, 5, 3);
    int result = future.get();
    std::cout << "    Async result: " << result << std::endl;
    
    // std::jthread (C++20)
    std::cout << "  === std::jthread (C++20) ===" << std::endl;
    
    auto jthread_func = [](std::stop_token st) {
        for (int i = 0; i < 10 && !st.stop_requested(); ++i) {
            std::cout << "    JThread iteration " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "    JThread finished" << std::endl;
    };
    
    std::jthread jt(jthread_func);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // jthread自动管理，无需显式join
}
```

## 3. 数据竞争

### 3.1 数据竞争定义和示例

数据竞争发生在不同执行线程并发访问同一内存位置且至少有一个是写操作：

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

void data_race_examples() {
    std::cout << "Data Race Examples:" << std::endl;
    
    // 未定义行为的数据竞争示例
    std::cout << "  === Undefined Behavior Example ===" << std::endl;
    
    int shared_counter = 0;  // 非原子共享变量
    
    auto increment_counter = [&shared_counter]() {
        for (int i = 0; i < 100000; ++i) {
            ++shared_counter;  // 数据竞争！
        }
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(increment_counter);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "    Shared counter (racing): " << shared_counter << " (expected: 400000)" << std::endl;
    
    // 使用原子操作避免数据竞争
    std::cout << "  === Atomic Operation Solution ===" << std::endl;
    
    std::atomic<int> atomic_counter{0};
    
    auto atomic_increment = [&atomic_counter]() {
        for (int i = 0; i < 100000; ++i) {
            atomic_counter.fetch_add(1);  // 原子操作，无数据竞争
        }
    };
    
    std::vector<std::thread> atomic_threads;
    for (int i = 0; i < 4; ++i) {
        atomic_threads.emplace_back(atomic_increment);
    }
    
    for (auto& t : atomic_threads) {
        t.join();
    }
    
    std::cout << "    Atomic counter: " << atomic_counter.load() << " (expected: 400000)" << std::endl;
    
    // 使用互斥锁避免数据竞争
    std::cout << "  === Mutex Solution ===" << std::endl;
    
    int protected_counter = 0;
    std::mutex counter_mutex;
    
    auto mutex_protected_increment = [&protected_counter, &counter_mutex]() {
        for (int i = 0; i < 100000; ++i) {
            std::lock_guard<std::mutex> lock(counter_mutex);
            ++protected_counter;  // 互斥保护，无数据竞争
        }
    };
    
    std::vector<std::thread> mutex_threads;
    for (int i = 0; i < 4; ++i) {
        mutex_threads.emplace_back(mutex_protected_increment);
    }
    
    for (auto& t : mutex_threads) {
        t.join();
    }
    
    std::cout << "    Protected counter: " << protected_counter << " (expected: 400000)" << std::endl;
}
```

### 3.2 冲突求值

两个表达式求值冲突的条件和示例：

```cpp
#include <iostream>
#include <thread>
#include <memory>

void conflicting_evaluations() {
    std::cout << "Conflicting Evaluations:" << std::endl;
    
    // 写-写冲突
    std::cout << "  === Write-Write Conflict ===" << std::endl;
    
    int data = 0;
    
    auto writer1 = [&data]() {
        for (int i = 0; i < 10000; ++i) {
            data = i;  // 写操作
        }
    };
    
    auto writer2 = [&data]() {
        for (int i = 0; i < 10000; ++i) {
            data = i * 2;  // 写操作（冲突）
        }
    };
    
    std::thread t1(writer1);
    std::thread t2(writer2);
    
    t1.join();
    t2.join();
    
    std::cout << "    Final data value (write-write conflict): " << data << std::endl;
    
    // 读-写冲突
    std::cout << "  === Read-Write Conflict ===" << std::endl;
    
    int shared_data = 0;
    
    auto reader = [&shared_data]() {
        for (int i = 0; i < 10000; ++i) {
            int local_copy = shared_data;  // 读操作
            // 使用local_copy...
        }
    };
    
    auto writer = [&shared_data]() {
        for (int i = 0; i < 10000; ++i) {
            shared_data = i;  // 写操作（与读操作冲突）
        }
    };
    
    std::thread t3(reader);
    std::thread t4(writer);
    
    t3.join();
    t4.join();
    
    std::cout << "    Final shared_data value (read-write conflict): " << shared_data << std::endl;
    
    // 生存期管理冲突
    std::cout << "  === Lifetime Management Conflict ===" << std::endl;
    
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    
    auto destroyer = [&ptr]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ptr.reset();  // 结束对象生存期
        std::cout << "    Object destroyed" << std::endl;
    };
    
    auto accessor = [&ptr]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (ptr) {
            int value = *ptr;  // 读取对象
            std::cout << "    Accessed value: " << value << std::endl;
        }
    };
    
    std::thread t5(destroyer);
    std::thread t6(accessor);
    
    t5.join();
    t6.join();
}
```

### 3.3 Happens-Before关系

Happens-Before关系是避免数据竞争的关键：

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

void happens_before_examples() {
    std::cout << "Happens-Before Examples:" << std::endl;
    
    // 同一线程内的顺序
    std::cout << "  === Sequential Order in Same Thread ===" << std::endl;
    
    int a = 0, b = 0;
    
    auto sequential_ops = [&a, &b]() {
        a = 1;  // 操作1
        b = 2;  // 操作2 (happens-after 操作1)
        std::cout << "    Sequential: a=" << a << ", b=" << b << std::endl;
    };
    
    sequential_ops();
    
    // 使用互斥锁建立Happens-Before关系
    std::cout << "  === Mutex-Based Happens-Before ===" << std::endl;
    
    int shared_data = 0;
    std::mutex data_mutex;
    
    auto producer = [&shared_data, &data_mutex]() {
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            shared_data = 42;  // 写操作
        }  // 释放互斥锁 (synchronizes-with 下一个获取)
        std::cout << "    Producer finished" << std::endl;
    };
    
    auto consumer = [&shared_data, &data_mutex]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            // 获取互斥锁 (synchronized-with 上一个释放)
            std::cout << "    Consumer read: " << shared_data << std::endl;
        }
    };
    
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
    
    // 原子操作的内存顺序
    std::cout << "  === Atomic Memory Ordering ===" << std::endl;
    
    std::atomic<bool> flag{false};
    std::atomic<int> data{0};
    
    auto atomic_producer = [&flag, &data]() {
        data.store(100, std::memory_order_relaxed);  // 数据写入
        flag.store(true, std::memory_order_release);  // 发布操作
        std::cout << "    Atomic producer finished" << std::endl;
    };
    
    auto atomic_consumer = [&flag, &data]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while (!flag.load(std::memory_order_acquire)) {  // 获取操作
            std::this_thread::yield();
        }
        int value = data.load(std::memory_order_relaxed);  // 数据读取
        std::cout << "    Atomic consumer read: " << value << std::endl;
    };
    
    std::thread t3(atomic_producer);
    std::thread t4(atomic_consumer);
    
    t3.join();
    t4.join();
}
```

## 4. 内存顺序

### 4.1 内存顺序类型详解

C++提供了多种内存顺序来控制原子操作：

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

void memory_order_types() {
    std::cout << "Memory Order Types:" << std::endl;
    
    // memory_order_relaxed - 最宽松的内存顺序
    std::cout << "  === memory_order_relaxed ===" << std::endl;
    std::cout << "    不提供同步或顺序保证，仅保证原子性" << std::endl;
    
    std::atomic<int> relaxed_counter{0};
    
    auto relaxed_worker = [&relaxed_counter]() {
        for (int i = 0; i < 10000; ++i) {
            relaxed_counter.fetch_add(1, std::memory_order_relaxed);
        }
    };
    
    std::vector<std::thread> relaxed_threads;
    for (int i = 0; i < 4; ++i) {
        relaxed_threads.emplace_back(relaxed_worker);
    }
    
    for (auto& t : relaxed_threads) {
        t.join();
    }
    
    std::cout << "    Relaxed counter final value: " << relaxed_counter.load() << std::endl;
    
    // memory_order_acquire - 获取操作
    std::cout << "  === memory_order_acquire ===" << std::endl;
    std::cout << "    用于读操作，与release操作形成同步关系" << std::endl;
    
    std::atomic<bool> data_ready{false};
    std::atomic<int> shared_data{0};
    
    auto acquire_consumer = [&data_ready, &shared_data]() {
        while (!data_ready.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        // 获取操作确保能看到发布操作之前的所有修改
        int value = shared_data.load(std::memory_order_relaxed);
        std::cout << "    Acquire consumer read: " << value << std::endl;
    };
    
    auto release_producer = [&data_ready, &shared_data]() {
        shared_data.store(42, std::memory_order_relaxed);
        data_ready.store(true, std::memory_order_release);  // 发布操作
        std::cout << "    Release producer finished" << std::endl;
    };
    
    std::thread t1(release_producer);
    std::thread t2(acquire_consumer);
    
    t1.join();
    t2.join();
    
    // memory_order_release - 发布操作
    std::cout << "  === memory_order_release ===" << std::endl;
    std::cout << "    用于写操作，与acquire操作形成同步关系" << std::endl;
    
    std::atomic<int> release_data{0};
    std::atomic<bool> release_flag{false};
    
    auto release_writer = [&release_data, &release_flag]() {
        release_data.store(100, std::memory_order_relaxed);
        release_flag.store(true, std::memory_order_release);  // 发布
        std::cout << "    Release writer finished" << std::endl;
    };
    
    auto acquire_reader = [&release_data, &release_flag]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while (!release_flag.load(std::memory_order_acquire)) {  // 获取
            std::this_thread::yield();
        }
        int value = release_data.load(std::memory_order_relaxed);
        std::cout << "    Acquire reader got: " << value << std::endl;
    };
    
    std::thread t3(release_writer);
    std::thread t4(acquire_reader);
    
    t3.join();
    t4.join();
    
    // memory_order_acq_rel - 获取-发布操作
    std::cout << "  === memory_order_acq_rel ===" << std::endl;
    std::cout << "    既是获取操作也是发布操作" << std::endl;
    
    std::atomic<int> acq_rel_data{0};
    
    auto acq_rel_worker = [&acq_rel_data](int id) {
        int prev = acq_rel_data.fetch_add(id, std::memory_order_acq_rel);
        std::cout << "    Worker " << id << " previous value: " << prev << std::endl;
    };
    
    std::thread t5(acq_rel_worker, 10);
    std::thread t6(acq_rel_worker, 20);
    
    t5.join();
    t6.join();
    
    std::cout << "    Final acq_rel_data value: " << acq_rel_data.load() << std::endl;
    
    // memory_order_seq_cst - 顺序一致性（默认）
    std::cout << "  === memory_order_seq_cst ===" << std::endl;
    std::cout << "    提供最强的顺序保证，所有线程看到相同的操作顺序" << std::endl;
    
    std::atomic<int> seq_cst_data1{0};
    std::atomic<int> seq_cst_data2{0};
    
    auto seq_cst_writer1 = [&seq_cst_data1, &seq_cst_data2]() {
        seq_cst_data1.store(1);  // 默认seq_cst
        int value = seq_cst_data2.load();  // 默认seq_cst
        std::cout << "    Writer1 read from data2: " << value << std::endl;
    };
    
    auto seq_cst_writer2 = [&seq_cst_data1, &seq_cst_data2]() {
        seq_cst_data2.store(1);  // 默认seq_cst
        int value = seq_cst_data1.load();  // 默认seq_cst
        std::cout << "    Writer2 read from data1: " << value << std::endl;
    };
    
    std::thread t7(seq_cst_writer1);
    std::thread t8(seq_cst_writer2);
    
    t7.join();
    t8.join();
}
```

### 4.2 内存可见性

内存可见性描述了一个线程对内存的修改何时对其他线程可见：

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

void memory_visibility() {
    std::cout << "Memory Visibility:" << std::endl;
    
    // 无同步的内存可见性问题
    std::cout << "  === Memory Visibility Without Synchronization ===" << std::endl;
    
    bool flag = false;
    int data = 0;
    
    auto writer_thread = [&flag, &data]() {
        data = 42;
        flag = true;  // 可能被重排序！
        std::cout << "    Writer finished" << std::endl;
    };
    
    auto reader_thread = [&flag, &data]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while (!flag) {
            std::this_thread::yield();
        }
        // 读取data时可能看不到42！
        std::cout << "    Reader got data: " << data << std::endl;
    };
    
    std::thread t1(writer_thread);
    std::thread t2(reader_thread);
    
    t1.join();
    t2.join();
    
    // 使用原子操作确保可见性
    std::cout << "  === Memory Visibility With Atomic Operations ===" << std::endl;
    
    std::atomic<bool> atomic_flag{false};
    std::atomic<int> atomic_data{0};
    
    auto atomic_writer = [&atomic_flag, &atomic_data]() {
        atomic_data.store(100, std::memory_order_relaxed);
        atomic_flag.store(true, std::memory_order_release);  // 确保可见性
        std::cout << "    Atomic writer finished" << std::endl;
    };
    
    auto atomic_reader = [&atomic_flag, &atomic_data]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while (!atomic_flag.load(std::memory_order_acquire)) {  // 确保可见性
            std::this_thread::yield();
        }
        int value = atomic_data.load(std::memory_order_relaxed);
        std::cout << "    Atomic reader got: " << value << std::endl;
    };
    
    std::thread t3(atomic_writer);
    std::thread t4(atomic_reader);
    
    t3.join();
    t4.join();
}
```

## 5. 向前推进

### 5.1 无阻塞自由和无锁自由

无阻塞和无锁属性是并发性能的重要指标：

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

void lock_free_properties() {
    std::cout << "Lock-Free Properties:" << std::endl;
    
    // 无锁自由 (Lock-Free)
    std::cout << "  === Lock-Free Freedom ===" << std::endl;
    std::cout << "    当一个或多个无锁原子函数并发运行时，至少有一个将保证完成" << std::endl;
    
    std::atomic<int> lock_free_counter{0};
    
    // 检查是否为无锁实现
    if (lock_free_counter.is_lock_free()) {
        std::cout << "    Atomic<int> is lock-free" << std::endl;
    } else {
        std::cout << "    Atomic<int> is not lock-free" << std::endl;
    }
    
    auto lock_free_worker = [&lock_free_counter](int id) {
        for (int i = 0; i < 10000; ++i) {
            lock_free_counter.fetch_add(1, std::memory_order_relaxed);
        }
        std::cout << "    Lock-free worker " << id << " finished" << std::endl;
    };
    
    std::vector<std::thread> lock_free_threads;
    for (int i = 0; i < 4; ++i) {
        lock_free_threads.emplace_back(lock_free_worker, i);
    }
    
    for (auto& t : lock_free_threads) {
        t.join();
    }
    
    std::cout << "    Lock-free counter final value: " << lock_free_counter.load() << std::endl;
    
    // 无阻塞自由 (Wait-Free)
    std::cout << "  === Wait-Free Freedom ===" << std::endl;
    std::cout << "    当且仅当只有一个线程在标准库函数中未阻塞时执行无锁原子函数，该执行将保证完成" << std::endl;
    
    // 等待自由操作示例
    std::atomic<int> wait_free_data{0};
    
    auto wait_free_worker = [&wait_free_data](int id) {
        // 等待自由操作示例
        int expected = 0;
        int desired = id + 1;
        if (wait_free_data.compare_exchange_strong(expected, desired)) {
            std::cout << "    Wait-free worker " << id << " succeeded" << std::endl;
        } else {
            std::cout << "    Wait-free worker " << id << " failed (expected: " 
                      << expected << ", got: " << wait_free_data.load() << ")" << std::endl;
        }
    };
    
    std::thread t1(wait_free_worker, 1);
    std::thread t2(wait_free_worker, 2);
    
    t1.join();
    t2.join();
    
    std::cout << "    Wait-free data final value: " << wait_free_data.load() << std::endl;
}
```

## 6. 进度保证

### 6.1 进度保证类型

C++标准规定了线程必须最终执行的操作：

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

void progress_guarantees() {
    std::cout << "Progress Guarantees:" << std::endl;
    
    // 1. 终止操作
    std::cout << "  === Termination ===" << std::endl;
    
    auto terminating_thread = []() {
        std::cout << "    Terminating thread started" << std::endl;
        // 执行一些工作后自然终止
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "    Terminating thread finished" << std::endl;
    };
    
    std::thread t1(terminating_thread);
    t1.join();
    
    // 2. 调用 std::this_thread::yield
    std::cout << "  === Yielding ===" << std::endl;
    
    auto yielding_thread = []() {
        std::cout << "    Yielding thread started" << std::endl;
        for (int i = 0; i < 5; ++i) {
            std::cout << "    Yielding iteration " << i << std::endl;
            std::this_thread::yield();  // 让出执行权
        }
        std::cout << "    Yielding thread finished" << std::endl;
    };
    
    std::thread t2(yielding_thread);
    t2.join();
    
    // 3. I/O 操作
    std::cout << "  === I/O Operations ===" << std::endl;
    
    auto io_thread = []() {
        std::cout << "    I/O thread started" << std::endl;
        std::cout << "    Performing I/O operation..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "    I/O thread finished" << std::endl;
    };
    
    std::thread t3(io_thread);
    t3.join();
    
    // 4. volatile 访问
    std::cout << "  === Volatile Access ===" << std::endl;
    
    volatile int volatile_var = 0;
    
    auto volatile_thread = [&volatile_var]() {
        std::cout << "    Volatile thread started" << std::endl;
        for (int i = 0; i < 3; ++i) {
            volatile_var = i;  // volatile 访问
            std::cout << "    Volatile access: " << volatile_var << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "    Volatile thread finished" << std::endl;
    };
    
    std::thread t4(volatile_thread);
    t4.join();
    
    // 5. 原子操作
    std::cout << "  === Atomic Operations ===" << std::endl;
    
    std::atomic<int> atomic_var{0};
    
    auto atomic_thread = [&atomic_var]() {
        std::cout << "    Atomic thread started" << std::endl;
        for (int i = 0; i < 3; ++i) {
            atomic_var.store(i);  // 原子操作
            int value = atomic_var.load();
            std::cout << "    Atomic operation: " << value << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "    Atomic thread finished" << std::endl;
    };
    
    std::thread t5(atomic_thread);
    t5.join();
}
```

## 7. 平凡的无限循环

### 7.1 平凡无限循环定义

平凡无限循环是指那些不执行任何可观察行为的无限循环：

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void trivial_infinite_loops() {
    std::cout << "Trivial Infinite Loops:" << std::endl;
    
    // 合法的平凡无限循环 (C++11及以后)
    std::cout << "  === Legal Trivial Infinite Loops ===" << std::endl;
    
    // 这些是合法的平凡无限循环
    std::cout << "    Examples of legal trivial infinite loops:" << std::endl;
    std::cout << "    for(;;);                    // (1)" << std::endl;
    std::cout << "    while(true);                // (2)" << std::endl;
    std::cout << "    do ; while(true);           // (3)" << std::endl;
    
    // 非平凡无限循环（未定义行为）
    std::cout << "  === Non-Trivial Infinite Loops (Undefined Behavior) ===" << std::endl;
    
    // 这些是非平凡无限循环，可能导致未定义行为
    std::cout << "    Examples that may cause undefined behavior:" << std::endl;
    std::cout << "    for(;;) { int x; }          // (1) - 声明变量" << std::endl;
    std::cout << "    while(true) { func(); }     // (2) - 调用函数" << std::endl;
    
    // 实际示例
    std::cout << "  === Practical Examples ===" << std::endl;
    
    // 合法的服务器循环
    auto server_loop = []() {
        std::cout << "    Server loop started (will run for 200ms)" << std::endl;
        auto start = std::chrono::steady_clock::now();
        while (true) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > 200) {
                break;
            }
            // 空循环体，但有退出条件
        }
        std::cout << "    Server loop finished" << std::endl;
    };
    
    std::thread t1(server_loop);
    t1.join();
}
```

## 8. 并发前进保证

### 8.1 并发前进保证类型

不同的线程类型提供不同级别的前进保证：

```cpp
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

void concurrent_progress_guarantees() {
    std::cout << "Concurrent Progress Guarantees:" << std::endl;
    
    // 并发前进保证 (Concurrent Forward Progress)
    std::cout << "  === Concurrent Forward Progress ===" << std::endl;
    
    std::cout << "    Threads providing concurrent forward progress:" << std::endl;
    std::cout << "    - Main thread" << std::endl;
    std::cout << "    - Threads created by std::thread" << std::endl;
    std::cout << "    - Threads created by std::jthread (C++20)" << std::endl;
    
    auto concurrent_thread = [](int id) {
        std::cout << "    Concurrent thread " << id << " started" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "    Concurrent thread " << id << " finished" << std::endl;
    };
    
    std::thread t1(concurrent_thread, 1);
    std::thread t2(concurrent_thread, 2);
    
    t1.join();
    t2.join();
    
    // 并行前进保证 (Parallel Forward Progress)
    std::cout << "  === Parallel Forward Progress ===" << std::endl;
    
    std::cout << "    Threads providing parallel forward progress:" << std::endl;
    std::cout << "    - Standard library parallel algorithms (C++17)" << std::endl;
    std::cout << "    - Thread pool workers" << std::endl;
    
    // 模拟并行算法执行
    auto parallel_task = [](int start, int end) {
        std::cout << "    Parallel task processing [" << start << ", " << end << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "    Parallel task [" << start << ", " << end << ") finished" << std::endl;
    };
    
    std::thread t3(parallel_task, 0, 100);
    std::thread t4(parallel_task, 100, 200);
    
    t3.join();
    t4.join();
}
```

## 9. 实践示例

### 9.1 生产者-消费者模式

使用内存模型概念实现线程安全的生产者-消费者模式：

```cpp
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

void producer_consumer_pattern() {
    std::cout << "Producer-Consumer Pattern:" << std::endl;
    
    // 线程安全的队列实现
    template<typename T>
    class ThreadSafeQueue {
    private:
        mutable std::mutex mut;
        std::queue<T> data_queue;
        std::condition_variable data_cond;
        
    public:
        void push(T new_value) {
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(new_value);
            data_cond.notify_one();
        }
        
        void wait_and_pop(T& value) {
            std::unique_lock<std::mutex> lk(mut);
            data_cond.wait(lk, [this] { return !data_queue.empty(); });
            value = data_queue.front();
            data_queue.pop();
        }
        
        bool try_pop(T& value) {
            std::lock_guard<std::mutex> lk(mut);
            if (data_queue.empty()) {
                return false;
            }
            value = data_queue.front();
            data_queue.pop();
            return true;
        }
        
        bool empty() const {
            std::lock_guard<std::mutex> lk(mut);
            return data_queue.empty();
        }
    };
    
    ThreadSafeQueue<int> queue;
    std::atomic<bool> done{false};
    
    // 生产者线程
    auto producer = [&queue, &done]() {
        for (int i = 0; i < 10; ++i) {
            queue.push(i);
            std::cout << "    Produced: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        done.store(true);
        std::cout << "    Producer finished" << std::endl;
    };
    
    // 消费者线程
    auto consumer = [&queue, &done]() {
        while (!done.load() || !queue.empty()) {
            int value;
            if (queue.try_pop(value)) {
                std::cout << "    Consumed: " << value << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "    Consumer finished" << std::endl;
    };
    
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
}
```

## 10. 标准参考

C++ 多线程和内存模型的相关标准条款：

- C++23 标准（ISO/IEC 14882:2024）：
  - 6.9.1 执行线程 [basic.stc]
  - 6.9.2 数据竞争 [basic.exec]
  - 6.9.3 内存模型 [basic.memo]
  - 32.2 线程 [thread.threads]
  - 32.3 互斥锁 [thread.mutex]
  - 32.4 条件变量 [thread.condition]
  - 32.5 原子类型 [atomics]
- C++20 标准（ISO/IEC 14882:2020）：
  - 6.9.1 执行线程 [basic.stc]
  - 6.9.2 数据竞争 [basic.exec]
  - 6.9.3 内存模型 [basic.memo]
  - 31.2 线程 [thread.threads]
  - 31.3 互斥锁 [thread.mutex]
  - 31.4 条件变量 [thread.condition]
  - 31.5 原子类型 [atomics]
- C++17 标准（ISO/IEC 14882:2017）：
  - 4.6 执行线程 [basic.stc]
  - 4.7 数据竞争 [basic.exec]
  - 4.8 内存模型 [basic.memo]
  - 33.2 线程 [thread.threads]
  - 33.3 互斥锁 [thread.mutex]
  - 33.4 条件变量 [thread.condition]
  - 33.5 原子类型 [atomics]

相关的缺陷报告：

- CWG 1953: 内存位置定义的澄清
- LWG 2200: 容器数据竞争的澄清
- P2809R3: 平凡无限循环行为的定义

## 11. 总结

C++ 多线程执行和数据竞争是现代C++并发编程的核心概念，理解这些概念对于编写正确和高效的多线程程序至关重要。

### 11.1 核心要点

- 基础概念：理解并发与并行、线程与进程、存储持续时间等基本概念
- 执行线程：每个线程是独立的执行流，可以并发访问程序中的任何对象
- 数据竞争：并发访问同一内存位置且至少有一个写操作时发生，导致未定义行为
- 内存顺序：控制原子操作的可见性和顺序，包括relaxed、acquire、release、acq_rel和seq_cst
- 前进保证：确保线程最终会执行某些操作，防止程序无限期挂起
- Happens-Before关系：建立操作间的顺序关系，避免数据竞争

### 11.2 最佳实践

- 避免数据竞争：使用原子操作、互斥锁或其他同步机制
- 选择合适的内存顺序：根据性能和正确性需求选择适当的内存顺序
- 理解前进保证：确保线程能够取得进展
- 使用标准库工具：优先使用标准库提供的线程和同步原语
- 测试并发代码：并发程序的行为可能因执行顺序而异，需要充分测试

### 11.3 未来发展趋势

- 更强的内存模型保证：标准将继续完善内存模型的定义和保证
- 更好的工具支持：编译器和调试工具将提供更好的并发程序分析能力
- 性能优化：硬件和编译器将继续优化多线程程序性能
- 安全增强：内存安全和数据竞争检测将成为更重要的关注点
- 并发模式标准化：更多并发编程模式将被标准化和优化

通过深入理解和正确应用C++多线程执行和数据竞争的概念，开发者可以编写出既高效又安全的并发C++程序，充分发挥现代多核处理器的性能优势。
