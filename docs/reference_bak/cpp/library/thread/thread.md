# C++ 并发支持库（Concurrency support library）（C++11起）

来源：cppreference.com

## 并发支持库概述

C++内置支持线程、原子操作、互斥锁、条件变量和期程（futures），为现代多线程编程提供了全面的基础设施。

## 核心组件

### 一、线程管理

| 组件 | 说明 |
|------|------|
| `<thread>`头文件 | |
| `thread`(C++11) | 管理独立线程（类） |
| `jthread`(C++20) | 支持自动连接和取消的std::thread（类） |
| **当前线程函数** |
| `yield`(C++11) | 建议实现重新调度线程执行（函数） |
| `get_id`(C++11) | 返回当前线程的线程ID（函数） |
| `sleep_for`(C++11) | 停止当前线程指定的时间间隔（函数） |
| `sleep_until`(C++11) | 停止当前线程直到指定的时间点（函数） |

### 二、协作取消（C++20起）

| 组件 | 说明 |
|------|------|
| `<stop_token>`头文件 | |
| **停止令牌类型** |
| `stop_token`(C++20) | 查询std::jthread取消请求的接口（类） |
| `never_stop_token`(C++26) | 提供永不被停止的令牌接口（类） |
| `inplace_stop_token`(C++26) | 引用关联的`std::inplace_stop_source`对象停止状态的令牌（类） |
| **停止源类型** |
| `stop_source`(C++20) | 表示停止一个或多个std::jthreads请求的类（类） |
| `inplace_stop_source`(C++26) | 单独拥有停止状态的停止源（类） |
| **停止回调类型** |
| `stop_callback`(C++20) | 注册std::jthread取消回调的接口（类模板） |
| `inplace_stop_callback`(C++26) | `std::inplace_stop_token`的停止回调（类模板） |

### 三、缓存大小访问（C++17起）

| 组件 | 说明 |
|------|------|
| `<new>`头文件 | |
| `hardware_destructive_interference_size`(C++17) | 避免伪共享的最小偏移量 |
| `hardware_constructive_interference_size`(C++17) | 促进真共享的最大偏移量 |

### 四、原子操作

| 组件 | 说明 |
|------|------|
| `<atomic>`头文件 | |
| **原子类型** |
| `atomic`(C++11) | 原子类模板及bool、整型、浮点型和指针类型的特化（类模板） |
| `atomic_ref`(C++20) | 对非原子对象提供原子操作（类模板） |
| **原子类型操作** |
| `atomic_is_lock_free`(C++11) | 检查原子类型的锁无关操作（函数模板） |
| `atomic_store`等(C++11) | 原子地替换原子对象的值（函数模板） |
| `atomic_load`等(C++11) | 原子地获取原子对象存储的值（函数模板） |
| `atomic_exchange`等(C++11) | 原子交换并返回旧值（函数模板） |
| `atomic_compare_exchange`等(C++11) | 原子比较交换操作（函数模板） |
| `atomic_fetch_add`等(C++11) | 原子加法并返回旧值（函数模板） |
| `atomic_wait`等(C++20) | 等待原子值变化（函数模板） |
| **标志类型和操作** |
| `atomic_flag`(C++11) | 无锁布尔原子类型（类） |
| `atomic_flag_test_and_set`等(C++11) | 原子地设置和清除标志（函数） |
| **初始化** |
| `ATOMIC_FLAG_INIT`(C++11) | 初始化std::atomic_flag为false（宏常量） |
| **内存同步顺序** |
| `memory_order`(C++11) | 定义给定原子操作的内存排序约束（枚举） |
| `atomic_thread_fence`(C++11) | 通用内存顺序相关的同步原语（函数） |

### 五、互斥锁

| 组件 | 说明 |
|------|------|
| `<mutex>`头文件 | |
| `mutex`(C++11) | 提供基本互斥设施（类） |
| `timed_mutex`(C++11) | 提供带超时锁定的互斥设施（类） |
| `recursive_mutex`(C++11) | 可被同一线程递归锁定的互斥设施（类） |
| `recursive_timed_mutex`(C++11) | 可递归锁定且带超时的互斥设施（类） |
| `<shared_mutex>`头文件 | |
| `shared_mutex`(C++17) | 提供共享互斥设施（类） |
| `shared_timed_mutex`(C++14) | 提供共享互斥设施且带超时（类） |
| **通用互斥管理** |
| `lock_guard`(C++11) | 作用域基互斥所有权包装器（类模板） |
| `unique_lock`(C++11) | 可移动互斥所有权包装器（类模板） |
| `shared_lock`(C++14) | 可移动共享互斥所有权包装器（类模板） |

### 六、条件变量

| 组件 | 说明 |
|------|------|
| `<condition_variable>`头文件 | |
| `condition_variable`(C++11) | 与std::unique_lock关联的条件变量（类） |
| `condition_variable_any`(C++11) | 与任何锁类型关联的条件变量（类） |
| `cv_status`(C++11) | 条件变量定时等待的可能结果（枚举） |

### 七、信号量（C++20起）

| 组件 | 说明 |
|------|------|
| `<semaphore>`头文件 | |
| `counting_semaphore`(C++20) | 模拟非负资源计数的信号量（类模板） |
| `binary_semaphore`(C++20) | 只有两种状态的信号量（typedef） |

### 八、门闩和屏障（C++20起）

| 组件 | 说明 |
|------|------|
| `<latch>`头文件 | |
| `latch`(C++20) | 单次使用的线程屏障（类） |
| `<barrier>`头文件 | |
| `barrier`(C++20) | 可重复使用的线程屏障（类模板） |

### 九、期程（Futures）

| 组件 | 说明 |
|------|------|
| `<future>`头文件 | |
| `promise`(C++11) | 存储值以进行异步检索（类模板） |
| `future`(C++11) | 等待异步设置的值（类模板） |
| `shared_future`(C++11) | 等待异步设置的值（可能被多个期程引用） |
| `async`(C++11) | 异步运行函数并返回保存结果的std::future（函数模板） |
| `launch`(C++11) | 指定std::async的启动策略（枚举） |

---

## 扩展知识详解

### 一、线程基础

#### 1. thread类详解
```cpp
#include <thread>
#include <iostream>
#include <chrono>
#include <vector>

void thread_function(int id, int delay) {
    std::cout << "Thread " << id << " starting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    std::cout << "Thread " << id << " finished!" << std::endl;
}

void basic_thread_examples() {
    // 创建和启动线程
    std::thread t1(thread_function, 1, 2);
    std::thread t2(thread_function, 2, 1);
    
    // 获取线程ID
    std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;
    std::cout << "Thread 1 ID: " << t1.get_id() << std::endl;
    std::cout << "Thread 2 ID: " << t2.get_id() << std::endl;
    
    // 等待线程完成
    t1.join();
    t2.join();
    
    // 线程移动语义
    std::thread t3(thread_function, 3, 1);
    std::thread t4 = std::move(t3);  // 移动线程所有权
    if (t3.joinable()) {
        t3.join();
    }
    t4.join();
    
    // 获取硬件并发线程数
    unsigned int hw_threads = std::thread::hardware_concurrency();
    std::cout << "Hardware concurrency: " << hw_threads << std::endl;
}

void thread_pool_example() {
    const int num_threads = 4;
    std::vector<std::thread> threads;
    
    // 创建线程池
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([i]() {
            std::cout << "Worker thread " << i << " processing..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i + 1)));
            std::cout << "Worker thread " << i << " done!" << std::endl;
        });
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
```

#### 2. jthread类（C++20）
```cpp
#include <thread>
#include <iostream>
#include <chrono>

void jthread_examples() {
    // jthread自动连接
    {
        std::jthread jt([]() {
            for (int i = 0; i < 5; ++i) {
                std::cout << "jthread working... " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
        
        // jthread自动管理生命周期
        std::cout << "jthread started, will auto-join at end of scope" << std::endl;
    }  // 自动调用join()
    
    // jthread取消支持
    std::jthread cancellable_thread([](std::stop_token st) {
        int count = 0;
        while (!st.stop_requested() && count < 10) {
            std::cout << "Working... " << count++ << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        if (st.stop_requested()) {
            std::cout << "Thread was cancelled!" << std::endl;
        }
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    cancellable_thread.request_stop();  // 请求取消
    // jthread会自动处理取消请求
}
```

### 二、原子操作详解

#### 1. 基本原子操作
```cpp
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>

void atomic_examples() {
    // 基本原子类型
    std::atomic<int> atomic_counter{0};
    std::atomic<bool> atomic_flag{false};
    
    // 原子操作
    atomic_counter.store(10);  // 原子存储
    int value = atomic_counter.load();  // 原子加载
    std::cout << "Atomic value: " << value << std::endl;
    
    // 原子交换
    int old_value = atomic_counter.exchange(20);
    std::cout << "Old value: " << old_value << ", New value: " << atomic_counter.load() << std::endl;
    
    // 原子比较交换
    int expected = 20;
    int desired = 30;
    bool success = atomic_counter.compare_exchange_weak(expected, desired);
    std::cout << "CAS success: " << success << ", Value: " << atomic_counter.load() << std::endl;
    
    // 原子算术操作
    atomic_counter.fetch_add(5);  // 原子加法
    atomic_counter.fetch_sub(2);  // 原子减法
    std::cout << "After arithmetic operations: " << atomic_counter.load() << std::endl;
    
    // 原子标志操作
    bool was_set = atomic_flag.test_and_set();  // 原子设置并返回旧值
    std::cout << "Flag was set: " << was_set << std::endl;
    atomic_flag.clear();  // 原子清除
}

void concurrent_counter_example() {
    std::atomic<int> counter{0};
    const int num_threads = 10;
    const int increments_per_thread = 1000;
    
    std::vector<std::thread> threads;
    
    // 创建多个线程并发增加计数器
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, increments_per_thread]() {
            for (int j = 0; j < increments_per_thread; ++j) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Expected: " << num_threads * increments_per_thread << std::endl;
    std::cout << "Actual: " << counter.load() << std::endl;
}
```

#### 2. 内存顺序
```cpp
#include <atomic>
#include <thread>
#include <iostream>

void memory_order_examples() {
    std::atomic<int> data{0};
    std::atomic<bool> ready{false};
    
    // 生产者线程
    std::thread producer([&data, &ready]() {
        data.store(42, std::memory_order_relaxed);  // 存储数据
        ready.store(true, std::memory_order_release);  // 发布数据
    });
    
    // 消费者线程
    std::thread consumer([&data, &ready]() {
        while (!ready.load(std::memory_order_acquire)) {  // 获取数据
            std::this_thread::yield();
        }
        std::cout << "Data: " << data.load(std::memory_order_relaxed) << std::endl;
    });
    
    producer.join();
    consumer.join();
    
    // 内存屏障示例
    std::atomic_thread_fence(std::memory_order_seq_cst);  // 顺序一致性屏障
    std::atomic_thread_fence(std::memory_order_acquire);  // 获取屏障
    std::atomic_thread_fence(std::memory_order_release);  // 释放屏障
}
```

### 三、互斥锁详解

#### 1. 基本互斥锁
```cpp
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>

class ThreadSafeCounter {
private:
    mutable std::mutex mtx_;
    int count_ = 0;

public:
    void increment() {
        std::lock_guard<std::mutex> lock(mtx_);  // RAII锁保护
        ++count_;
    }
    
    void increment_many(int times) {
        for (int i = 0; i < times; ++i) {
            std::lock_guard<std::mutex> lock(mtx_);
            ++count_;
        }
    }
    
    int get_count() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return count_;
    }
};

void mutex_examples() {
    ThreadSafeCounter counter;
    std::vector<std::thread> threads;
    
    // 创建多个线程并发操作计数器
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&counter]() {
            counter.increment_many(100);
        });
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final count: " << counter.get_count() << std::endl;
}

void unique_lock_examples() {
    std::mutex mtx;
    
    // unique_lock提供更灵活的锁定机制
    std::unique_lock<std::mutex> lock(mtx);
    
    // 可以手动解锁和重新锁定
    lock.unlock();
    // 执行一些不需要锁定的操作
    lock.lock();
    
    // 可以移动所有权
    std::unique_lock<std::mutex> moved_lock = std::move(lock);
    
    // 带超时的锁定
    std::timed_mutex timed_mtx;
    std::unique_lock<std::timed_mutex> timed_lock(timed_mtx, std::defer_lock);
    if (timed_lock.try_lock_for(std::chrono::milliseconds(100))) {
        std::cout << "Locked successfully!" << std::endl;
    } else {
        std::cout << "Failed to lock within timeout!" << std::endl;
    }
}
```

#### 2. 共享互斥锁（C++14/17）
```cpp
#include <shared_mutex>
#include <thread>
#include <iostream>
#include <vector>

class ThreadSafeData {
private:
    mutable std::shared_mutex rw_mtx_;
    int data_ = 0;

public:
    void write_data(int value) {
        std::unique_lock<std::shared_mutex> lock(rw_mtx_);  // 独占写锁
        data_ = value;
        std::cout << "Wrote data: " << data_ << std::endl;
    }
    
    int read_data() const {
        std::shared_lock<std::shared_mutex> lock(rw_mtx_);  // 共享读锁
        std::cout << "Reading data: " << data_ << std::endl;
        return data_;
    }
};

void shared_mutex_examples() {
    ThreadSafeData data;
    
    // 写线程
    std::thread writer([&data]() {
        for (int i = 0; i < 3; ++i) {
            data.write_data(i * 10);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    
    // 读线程
    std::vector<std::thread> readers;
    for (int i = 0; i < 3; ++i) {
        readers.emplace_back([&data]() {
            for (int j = 0; j < 5; ++j) {
                data.read_data();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });
    }
    
    writer.join();
    for (auto& reader : readers) {
        reader.join();
    }
}
```

### 四、条件变量详解

#### 1. 基本条件变量使用
```cpp
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>

class ThreadSafeQueue {
private:
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<int> queue_;

public:
    void push(int value) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(value);
        cv_.notify_one();  // 通知等待的线程
    }
    
    int pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        // 等待直到队列不为空
        cv_.wait(lock, [this]() { return !queue_.empty(); });
        
        int value = queue_.front();
        queue_.pop();
        return value;
    }
    
    int pop_with_timeout() {
        std::unique_lock<std::mutex> lock(mtx_);
        // 带超时的等待
        if (cv_.wait_for(lock, std::chrono::seconds(1), [this]() { return !queue_.empty(); })) {
            int value = queue_.front();
            queue_.pop();
            return value;
        } else {
            throw std::runtime_error("Timeout");
        }
    }
};

void condition_variable_examples() {
    ThreadSafeQueue queue;
    
    // 生产者线程
    std::thread producer([&queue]() {
        for (int i = 0; i < 5; ++i) {
            queue.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });
    
    // 消费者线程
    std::thread consumer([&queue]() {
        try {
            for (int i = 0; i < 5; ++i) {
                int value = queue.pop();
                std::cout << "Consumed: " << value << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Consumer error: " << e.what() << std::endl;
        }
    });
    
    producer.join();
    consumer.join();
}
```

### 五、信号量详解（C++20）

#### 1. 信号量使用
```cpp
#include <semaphore>
#include <thread>
#include <iostream>
#include <vector>

void semaphore_examples() {
    // 二进制信号量（类似互斥锁）
    std::binary_semaphore binary_sem(1);  // 初始计数为1
    
    std::thread t1([&binary_sem]() {
        binary_sem.acquire();  // 获取信号量
        std::cout << "Thread 1 acquired semaphore" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Thread 1 releasing semaphore" << std::endl;
        binary_sem.release();  // 释放信号量
    });
    
    std::thread t2([&binary_sem]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        binary_sem.acquire();
        std::cout << "Thread 2 acquired semaphore" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << "Thread 2 releasing semaphore" << std::endl;
        binary_sem.release();
    });
    
    t1.join();
    t2.join();
    
    // 计数信号量
    std::counting_semaphore<5> counting_sem(3);  // 最大计数5，初始3
    
    std::vector<std::thread> workers;
    for (int i = 0; i < 5; ++i) {
        workers.emplace_back([i, &counting_sem]() {
            counting_sem.acquire();
            std::cout << "Worker " << i << " started" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "Worker " << i << " finished" << std::endl;
            counting_sem.release();
        });
    }
    
    for (auto& worker : workers) {
        worker.join();
    }
}
```

### 六、门闩和屏障（C++20）

#### 1. latch和barrier使用
```cpp
#include <latch>
#include <barrier>
#include <thread>
#include <iostream>
#include <vector>

void latch_examples() {
    const int num_workers = 3;
    std::latch work_done(num_workers);  // 创建门闩，计数为3
    
    std::vector<std::thread> workers;
    for (int i = 0; i < num_workers; ++i) {
        workers.emplace_back([i, &work_done]() {
            std::cout << "Worker " << i << " starting work" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i + 1)));
            std::cout << "Worker " << i << " finished work" << std::endl;
            work_done.count_down();  // 减少计数
        });
    }
    
    std::cout << "Main thread waiting for workers..." << std::endl;
    work_done.wait();  // 等待所有工作完成
    std::cout << "All workers finished!" << std::endl;
    
    for (auto& worker : workers) {
        worker.join();
    }
}

void barrier_examples() {
    const int num_threads = 4;
    const int num_rounds = 3;
    
    // 创建可重复使用的屏障
    std::barrier<> sync_point(num_threads, []() {
        std::cout << "All threads reached barrier, next round!" << std::endl;
    });
    
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([i, &sync_point, num_rounds]() {
            for (int round = 0; round < num_rounds; ++round) {
                std::cout << "Thread " << i << " working in round " << round << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i + 1)));
                std::cout << "Thread " << i << " waiting at barrier (round " << round << ")" << std::endl;
                sync_point.arrive_and_wait();  // 到达并等待所有线程
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
}
```

### 七、期程（Futures）详解

#### 1. async和future使用
```cpp
#include <future>
#include <thread>
#include <iostream>
#include <chrono>

int compute_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return x * x;
}

void future_examples() {
    // 异步执行函数
    std::future<int> future_result = std::async(std::launch::async, compute_square, 5);
    
    // 等待结果
    int result = future_result.get();
    std::cout << "Result: " << result << std::endl;
    
    // 带超时的等待
    std::future<int> future_with_timeout = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 42;
    });
    
    std::future_status status = future_with_timeout.wait_for(std::chrono::milliseconds(1000));
    if (status == std::future_status::ready) {
        std::cout << "Result ready: " << future_with_timeout.get() << std::endl;
    } else if (status == std::future_status::timeout) {
        std::cout << "Timeout waiting for result" << std::endl;
    } else if (status == std::future_status::deferred) {
        std::cout << "Task is deferred" << std::endl;
    }
    
    // shared_future允许多个线程等待同一结果
    std::future<int> shared_future_source = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 100;
    });
    
    std::shared_future<int> shared_future = shared_future_source.share();
    
    std::thread t1([shared_future]() {
        std::cout << "Thread 1 result: " << shared_future.get() << std::endl;
    });
    
    std::thread t2([shared_future]() {
        std::cout << "Thread 2 result: " << shared_future.get() << std::endl;
    });
    
    t1.join();
    t2.join();
}

void promise_examples() {
    // 使用promise设置future的值
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    
    std::thread worker([promise = std::move(promise)]() mutable {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        promise.set_value(42);  // 设置promise的值
    });
    
    int result = future.get();  // 等待并获取结果
    std::cout << "Promise result: " << result << std::endl;
    worker.join();
    
    // promise设置异常
    std::promise<int> exception_promise;
    std::future<int> exception_future = exception_promise.get_future();
    
    std::thread exception_worker([exception_promise = std::move(exception_promise)]() mutable {
        try {
            throw std::runtime_error("Something went wrong!");
        } catch (...) {
            exception_promise.set_exception(std::current_exception());  // 设置异常
        }
    });
    
    try {
        int result = exception_future.get();  // 这会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    exception_worker.join();
}
```

### 八、协作取消（C++20）

#### 1. 停止令牌使用
```cpp
#include <stop_token>
#include <thread>
#include <iostream>
#include <chrono>

void stop_token_examples() {
    // 使用stop_source和stop_token
    std::stop_source source;
    std::stop_token token = source.get_token();
    
    std::thread cancellable_thread([token]() {
        int count = 0;
        while (count < 10 && !token.stop_requested()) {
            std::cout << "Working... " << count++ << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        
        if (token.stop_requested()) {
            std::cout << "Work was cancelled!" << std::endl;
        } else {
            std::cout << "Work completed normally!" << std::endl;
        }
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    source.request_stop();  // 请求停止
    
    cancellable_thread.join();
    
    // 使用停止回调
    std::stop_source callback_source;
    std::stop_token callback_token = callback_source.get_token();
    
    // 注册停止回调
    std::stop_callback callback(callback_token, []() {
        std::cout << "Stop callback executed!" << std::endl;
    });
    
    std::thread callback_thread([callback_token]() {
        for (int i = 0; i < 5; ++i) {
            if (callback_token.stop_requested()) {
                break;
            }
            std::cout << "Callback thread working... " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    callback_source.request_stop();
    callback_thread.join();
}
```

### 九、实际应用场景

#### 1. 生产者-消费者模式
```cpp
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <iostream>
#include <chrono>

template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<T> queue_;
    bool shutdown_ = false;

public:
    void push(T item) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(item);
        cv_.notify_one();
    }
    
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() { return !queue_.empty() || shutdown_; });
        
        if (queue_.empty() && shutdown_) {
            return std::nullopt;
        }
        
        T item = queue_.front();
        queue_.pop();
        return item;
    }
    
    void shutdown() {
        std::lock_guard<std::mutex> lock(mtx_);
        shutdown_ = true;
        cv_.notify_all();
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.empty();
    }
};

void producer_consumer_example() {
    ThreadSafeQueue<int> queue;
    
    // 生产者线程
    std::thread producer([&queue]() {
        for (int i = 0; i < 10; ++i) {
            queue.push(i);
            std::cout << "Produced: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        queue.shutdown();  // 通知消费者停止
    });
    
    // 消费者线程
    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; ++i) {
        consumers.emplace_back([i, &queue]() {
            while (true) {
                auto item = queue.pop();
                if (!item) {
                    break;  // 队列已关闭
                }
                std::cout << "Consumer " << i << " consumed: " << *item << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
            std::cout << "Consumer " << i << " finished" << std::endl;
        });
    }
    
    producer.join();
    for (auto& consumer : consumers) {
        consumer.join();
    }
}
```

#### 2. 线程池实现
```cpp
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <future>
#include <iostream>

class ThreadPool {
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;

public:
    ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex_);
                        condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        
                        if (stop_ && tasks_.empty()) {
                            return;
                        }
                        
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    
                    task();
                }
            });
        }
    }
    
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using return_type = std::invoke_result_t<F, Args...>;
        
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> result = task->get_future();
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            
            if (stop_) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            
            tasks_.emplace([task]() { (*task)(); });
        }
        
        condition_.notify_one();
        return result;
    }
    
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }
        
        condition_.notify_all();
        
        for (std::thread& worker : workers_) {
            worker.join();
        }
    }
};

void thread_pool_example() {
    ThreadPool pool(4);
    std::vector<std::future<int>> results;
    
    // 提交任务到线程池
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i + 1)));
                std::cout << "Task " << i << " completed by thread " 
                          << std::this_thread::get_id() << std::endl;
                return i * i;
            })
        );
    }
    
    // 获取结果
    for (auto& result : results) {
        std::cout << "Result: " << result.get() << std::endl;
    }
}
```

## 最佳实践建议

### 一、线程管理准则

1. **RAII原则**：使用`std::lock_guard`、`std::unique_lock`等RAII类型管理资源
2. **避免死锁**：按固定顺序获取多个锁，使用`std::scoped_lock`
3. **异常安全**：确保异常情况下锁能正确释放
4. **线程数量**：合理控制线程数量，避免过度创建

### 二、原子操作建议

1. **最小化原子操作范围**：只在必要时使用原子操作
2. **选择合适的内存顺序**：根据需求选择`memory_order`
3. **避免ABA问题**：在指针操作中注意ABA问题
4. **性能考虑**：原子操作不一定比互斥锁快

### 三、同步原语选择

1. **互斥锁**：适用于复杂临界区保护
2. **原子操作**：适用于简单的计数器等操作
3. **条件变量**：适用于线程间通信和等待
4. **信号量**：适用于资源计数和控制

### 四、性能优化建议

1. **减少锁竞争**：使用细粒度锁或无锁数据结构
2. **避免虚假唤醒**：在条件变量中使用谓词检查
3. **合理使用异步**：`std::async`不总是创建新线程
4. **缓存友好的数据结构**：注意false sharing问题

## 相关页面

| 页面 | 说明 |
|------|------|
| 并行算法库 | 并行STL算法 |
| 执行库 | C++20执行器支持 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/thread&oldid=179906>
- 最后修改时间：2025年1月28日 11:27
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++并发支持库为现代多线程编程提供了强大的基础设施。从基础的线程管理到高级的原子操作、从简单的互斥锁到复杂的线程池，这些组件帮助开发者构建高效且正确的并发应用程序。正确理解和使用这些并发原语是掌握现代C++多线程编程的关键。通过合理选择同步机制、正确处理异常和优化性能，可以构建出高质量的并发系统。持续学习和实践并发编程技术，结合实际应用场景，能够构建出高性能的多线程应用程序。记住并发编程的复杂性，始终进行充分的测试和验证。