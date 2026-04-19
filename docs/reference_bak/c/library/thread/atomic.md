# 并发支持库（Concurrency support library）

来源：[cppreference.com](https://en.cppreference.com)

---

## 简介

自 C++11 起，标准库正式引入了**并发支持库（Concurrency Support Library）**，使得开发者能够方便地编写跨平台、高性能的多线程程序。该库涵盖从底层原子操作到高层线程、互斥、条件变量、异步任务和未来对象等一整套并发编程工具。

---

## 库功能概览

| 模块 | 描述 |
|------|------|
| [**线程管理**](#线程-thread) | 线程对象、控制接口、调度 |
| [**原子操作**](#原子操作-atomic-operations) | 无锁并发编程基础 |
| [**互斥量与锁**](#互斥量-mutual-exclusion) | 同步机制、避免竞争 |
| [**条件变量**](#条件变量-condition-variables) | 线程间通信方式 |
| [**信号量**](#信号量-semaphores)（C++20） | 轻量级同步原语 |
| [**屏障与门闩**](#门闩与屏障-latches-and-barriers)（C++20） | 线程协调机制 |
| [**异步任务与 Future**](#异步任务与-future) | 获取异步结果 |
| [**协作取消机制**](#协作取消机制-cooperative-cancellation)（C++20） | 请求中断机制 |
| [**安全回收技术**](#安全回收-safe-reclamation)（C++26） | 解决指针悬空问题 |

---

## 线程（Thread）

### 头文件：`<thread>`

#### 核心类和命名空间

| 类/命名空间 | 描述 |
|-------------|------|
| `std::thread`（C++11） | 代表一个执行线程的对象 |
| `std::jthread`（C++20） | 自动 join / 可取消版本的 thread |
| `this_thread` | 管理当前线程的函数集合 |

#### 公共函数

| 函数 | 描述 |
|------|------|
| `this_thread::get_id()` | 获取当前线程 ID |
| `this_thread::yield()` | 让出线程执行权 |
| `this_thread::sleep_for()` | 当前线程休眠一段时间 |
| `this_thread::sleep_until()` | 当前线程休眠到指定时间点 |

#### 示例代码

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void task(int id) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task " << id << " done.\n";
}

int main() {
    std::thread t1(task, 1);
    std::thread t2(task, 2);

    t1.join();  // 等待完成
    t2.join();

    return 0;
}
```

---

## 协作取消机制（Cooperative Cancellation）（C++20）

### 头文件：`<stop_token>`

允许安全请求任务停止，通常用于 `std::jthread` 和异步操作中。

#### 核心组件

| 类型 | 描述 |
|------|------|
| `std::stop_source` | 停止请求源 |
| `std::stop_token` | 停止令牌，用于查询是否被请求停止 |
| `std::stop_callback` | 收到停止请求时触发的回调 |
| `std::never_stop_token`（C++26） | 表示不可能被停止 |
| `std::inplace_stop_source`（C++26） | 本地存储的停止源 |

#### 示例代码

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <stop_token>

void cancellable_task(std::stop_token st) {
    while (!st.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Working...\n";
    }
    std::cout << "Cancelled.\n";
}

int main() {
    std::jthread t(cancellable_task);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    t.request_stop();  // 发出停止请求
    return 0;
}
```

---

## 缓存大小访问（Cache Size Access）（C++17）

### 头文件：`<new>`

| 常量 | 描述 |
|------|------|
| `hardware_destructive_interference_size` | 避免假共享所需的最小偏移量 |
| `hardware_constructive_interference_size` | 提升真共享的最大偏移量 |

#### 示例代码

```cpp
struct alignas(std::hardware_destructive_interference_size) PaddedCounter {
    std::atomic<int> a;
    std::atomic<int> b;
};
```

---

## 原子操作（Atomic Operations）

### 头文件：`<atomic>`

提供基础原子操作能力，支持无锁并发编程。

#### 原子类型

| 类型 | 说明 |
|------|------|
| `std::atomic<T>` | 泛型原子类型 |
| `std::atomic_flag` | 最简布尔原子标记 |
| `std::atomic_ref`（C++20） | 对非原子对象进行原子操作的引用 |

#### 原子操作函数（部分）

| 函数 | 说明 |
|------|------|
| `atomic_load()` / `atomic_store()` | 原子加载/存储 |
| `atomic_exchange()` | 原子交换 |
| `atomic_compare_exchange_weak()` | 弱原子比较交换 |
| `atomic_fetch_add()` / `atomic_fetch_sub()` | 原子增减 |
| `atomic_fetch_or()` / `atomic_fetch_and()` | 原子按位运算 |
| `atomic_wait()` / `atomic_notify_one()`（C++20） | 阻塞等待/唤醒机制 |

#### 示例代码

```cpp
#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> counter{0};

void increment() {
    for (int i = 0; i < 1000; ++i) {
        counter.fetch_add(1);
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();

    std::cout << "Final value: " << counter.load() << "\n";
    return 0;
}
```

#### 内存顺序枚举

| 枚举值 | 描述 |
|--------|------|
| `memory_order_relaxed` | 仅保证操作原子性 |
| `memory_order_acquire` | 获取语义 |
| `memory_order_release` | 释放语义 |
| `memory_order_acq_rel` | 获取+释放 |
| `memory_order_seq_cst` | 顺序一致性（默认） |

---

## 互斥量（Mutual Exclusion）

### 头文件：`<mutex>` / `<shared_mutex>`

用于保护共享资源，防止多线程竞争。

#### 互斥量类型

| 类型 | 特点 |
|------|------|
| `std::mutex` | 基础互斥量 |
| `std::timed_mutex` | 支持超时锁定 |
| `std::recursive_mutex` | 允许递归锁定 |
| `std::shared_mutex`（C++17） | 支持读者写者模型 |
| `std::shared_timed_mutex`（C++14） | 支持共享和超时 |

#### 锁管理器

| 类型 | 描述 |
|------|------|
| `std::lock_guard` | RAII 简单锁 |
| `std::unique_lock` | 可移动锁，支持手动解锁 |
| `std::shared_lock`（C++14） | 共享读锁 |
| `std::scoped_lock`（C++17） | 多个互斥量的死锁规避锁 |

#### 示例代码

```cpp
#include <mutex>
#include <thread>
#include <iostream>

std::mutex mtx;

void print_block(int n, char c) {
    std::lock_guard<std::mutex> lck(mtx);
    for (int i = 0; i < n; ++i) {
        std::cout << c;
    }
    std::cout << '\n';
}

int main() {
    std::thread t1(print_block, 50, '*');
    std::thread t2(print_block, 50, '$');

    t1.join();
    t2.join();

    return 0;
}
```

---

## 条件变量（Condition Variables）

### 头文件：`<condition_variable>`

实现线程间的通知与等待。

#### 类型

| 类型 | 说明 |
|------|------|
| `std::condition_variable` | 与 `unique_lock` 配合使用 |
| `std::condition_variable_any` | 可配合任意锁类型 |

#### 示例代码

```cpp
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker_thread() {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, []{ return ready; });  // 等待条件满足
    std::cout << "Worker thread is processing data.\n";
}

void main_thread() {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard<std::mutex> lck(mtx);
        ready = true;
    }
    cv.notify_one();  // 通知等待线程
}

int main() {
    std::thread t1(worker_thread);
    std::thread t2(main_thread);

    t1.join();
    t2.join();

    return 0;
}
```

---

## 信号量（Semaphores）（C++20）

### 头文件：`<semaphore>`

轻量级同步原语，适用于资源计数场景。

#### 类型

| 类型 | 说明 |
|------|------|
| `std::counting_semaphore<N>` | 计数型信号量 |
| `std::binary_semaphore` | 二进制信号量（N=1） |

#### 示例代码

```cpp
#include <semaphore>
#include <thread>
#include <iostream>

std::binary_semaphore smph(0);  // 初始为 0

void test() {
    smph.acquire();  // 等待许可
    std::cout << "Thread executed.\n";
}

int main() {
    std::thread t(test);
    smph.release();  // 发出许可
    t.join();
    return 0;
}
```

---

## 门闩与屏障（Latches and Barriers）（C++20）

### 头文件：`<latch>` / `<barrier>`

用于线程同步点。

#### 类型

| 类型 | 说明 |
|------|------|
| `std::latch` | 单次使用的屏障 |
| `std::barrier` | 可重复使用的屏障 |

#### 示例代码

```cpp
#include <latch>
#include <thread>
#include <iostream>

std::latch work_done(2);

void worker() {
    std::cout << "Worker starting...\n";
    work_done.count_down();  // 工作完成
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);

    work_done.wait();  // 等待所有工作完成
    std::cout << "All work done.\n";

    t1.join();
    t2.join();
    return 0;
}
```

---

## 异步任务与 Future

### 头文件：`<future>`

用于获取异步任务的返回值。

#### 核心类

| 类型 | 说明 |
|------|------|
| `std::future<T>` | 一次性获取异步结果 |
| `std::shared_future<T>` | 可复制共享的 future |
| `std::promise<T>` | 手动设置异步结果 |
| `std::packaged_task<T>` | 封装函数并返回 future |
| `std::async()` | 启动异步任务 |

#### 示例代码

```cpp
#include <future>
#include <thread>
#include <iostream>

int compute() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 123;
}

int main() {
    std::future<int> result = std::async(std::launch::async, compute);
    std::cout << "Result: " << result.get() << "\n";  // 阻塞获取结果
    return 0;
}
```

---

## 安全回收（Safe Reclamation）（C++26）

### 头文件：`<rcu>` / `<hazard_pointer>`

解决并发环境中指针悬空的问题。

#### 核心组件

| 类型 | 描述 |
|------|------|
| `std::rcu_obj_base` | RCU 保护对象基类 |
| `std::rcu_domain` | RCU 域对象 |
| `std::hazard_pointer` | 危险指针保护机制 |

---

## 标准未来预留标识符

以下命名规则的标识符为标准保留：

| 规则 | 描述 |
|------|------|
| `stoppable-source` | 停止源概念（C++26） |
| `stoppable_token` | 停止令牌概念（C++26） |
| `unstoppable_token` | 不可停止令牌概念（C++26） |
| `stoppable-callback-for` | 停止回调概念（C++26） |

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C 并发支持 | [C Concurrency Support Library](https://zh.cppreference.com/w/c/thread) |

---

> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=cpp/thread&oldid=179906>  
> 最后修改时间：2025年1月28日 11:27 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

✅ **小结提示**：
- C++ 的并发支持库提供了从底层到高层的完整并发编程生态；
- 多线程编程需注意数据竞争、死锁和性能问题；
- 利用 RAII 模式和智能指针提升代码安全；
- 原子操作适合简单数据同步，互斥量适合复杂结构；
- 使用 `std::async` 和 `std::future` 实现异步任务解耦；
- C++20 的协作取消机制是现代中断控制的重要工具；
- C++26 的 RCU 与危险指针技术适用于高性能并发场景。

如需更复杂的调度模型（如协程、线程池），可考虑 C++20 协程库或第三方库（Boost.Asio、Intel TBB 等）。