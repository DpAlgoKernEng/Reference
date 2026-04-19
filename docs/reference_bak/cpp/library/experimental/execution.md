# C++执行库（Execution Library）（C++26起）

来源：cppreference.com

## 执行库概述

执行库提供了一个框架，用于在通用执行资源上管理异步执行。该库旨在为异步操作提供词汇类型，并允许以简单、可组合的方式构建任务执行图。

## 核心概念

### 一、发送者（Sender）
描述要发送执行的异步工作。产生操作状态（如下所述）。

- **发送者异步"发送"结果**给称为"接收者"的监听器
- 发送者可以使用通用算法组成**任务图**
- **发送者工厂和适配器**是捕获常见异步模式的对象，满足发送者概念

### 二、接收者（Receiver）
消费或"接收"发送者产生的异步结果的广义回调。

- 接收者有三个不同的"通道"，发送者可以通过它们传播结果：
  - 成功、失败和取消，分别命名为"值"、"错误"和"停止"
- 接收者提供可扩展的执行环境：消费者可以用来参数化异步操作的键/值对集合

### 三、操作状态（Operation State）
包含异步操作所需状态的对象。

- 当发送者和接收者传递给`std::execution::connect`函数时，它们被连接
- 连接发送者和接收者的结果是操作状态
- 只有在操作状态上调用"`start`"后，工作才会排队执行
- 一旦启动，操作状态的生命周期在异步操作完成之前不能结束，其地址必须稳定

### 四、调度器（Scheduler）
执行上下文的轻量级句柄。

- 执行上下文是异步执行的源，如线程池或GPU流
- 调度器是工厂，为发送者创建完成其接收者的发送者，该发送者从执行上下文拥有的执行线程完成

## 库组件详解

### 一、概念（Concepts）

#### 调度器概念
| 组件 | 说明 |
|------|------|
| `execution::scheduler`(C++26) | 指定类型为调度器（概念） |

#### 发送者概念
| 组件 | 说明 |
|------|------|
| `execution::sender`(C++26) | 指定类型为发送者（概念） |
| `execution::sender_in`(C++26) | 指定可以为给定关联环境类型创建异步操作的发送者（概念） |
| `execution::sender_to`(C++26) | 指定可以与特定接收者类型连接的发送者（概念） |

#### 接收者概念
| 组件 | 说明 |
|------|------|
| `execution::receiver`(C++26) | 指定类型为接收者（概念） |
| `execution::receiver_of`(C++26) | 指定为给定完成签名的接收者（概念） |

#### 操作状态概念
| 组件 | 说明 |
|------|------|
| `execution::operation_state`(C++26) | 指定类型为操作状态（概念） |

### 二、实用组件（Utility Components）

#### 执行上下文
| 组件 | 说明 |
|------|------|
| `execution::run_loop`(C++26) | 执行资源，包含线程安全的MPSC任务队列和手动驱动的事件循环（类） |

#### 执行域
| 组件 | 说明 |
|------|------|
| `execution::default_domain`(C++26) | 默认执行域标签类型，从发送者标签分发转换（类） |
| `execution::transform_sender`(C++26) | 在给定执行域标签下转换为新发送者（函数模板） |

#### 前进进度保证
| 组件 | 说明 |
|------|------|
| `execution::forward_progress_guarantee`(C++26) | 指定调度器关联执行资源创建的执行代理的前进进度保证（枚举） |

#### 环境
| 组件 | 说明 |
|------|------|
| `execution::prop`(C++26) | 从查询对象和值构建可查询对象（类模板） |
| `execution::env`(C++26) | 将多个可查询对象聚合为一个可查询对象（类模板） |
| `execution::get_env`(C++26) | 返回其给定参数的关联可查询对象（定制点对象） |

#### 查询
| 组件 | 说明 |
|------|------|
| `forwarding_query`(C++26) | 询问查询对象是否应通过可查询适配器转发（定制点对象） |
| `get_allocator`(C++26) | 询问可查询对象的关联分配器（定制点对象） |
| `get_stop_token`(C++26) | 询问可查询对象的关联停止令牌（定制点对象） |
| `execution::get_domain`(C++26) | 询问可查询对象的关联执行域标签（定制点对象） |
| `execution::get_scheduler`(C++26) | 询问可查询对象的关联调度器（定制点对象） |

#### 完成签名
| 组件 | 说明 |
|------|------|
| `execution::completion_signatures`(C++26) | 编码完成签名集的类型（类模板） |
| `execution::get_completion_signatures`(C++26) | 获取发送者的完成签名（定制点对象） |
| `execution::sends_stopped`(C++26) | 确定发送者是否支持停止完成（变量模板） |

#### 协程工具
| 组件 | 说明 |
|------|------|
| `execution::as_awaitable`(C++26) | 在特定协程内将表达式转换为可等待对象（定制点对象） |
| `execution::with_awaitable_senders`(C++26) | 用作协程承诺类型的基类时，使发送者在该协程类型内可等待（类模板） |

### 三、核心操作（Core Operations）

#### 操作状态
| 组件 | 说明 |
|------|------|
| `execution::connect`(C++26) | 连接`sender`与`receiver`（定制点对象） |
| `execution::start`(C++26) | 启动与`operation_state`对象关联的异步操作（定制点对象） |

#### 完成函数
这些函数由发送者调用，向其接收者宣布工作的完成。

| 组件 | 说明 |
|------|------|
| `execution::set_value`(C++26) | 值完成函数，指示成功完成（定制点对象） |
| `execution::set_error`(C++26) | 错误完成函数，指示计算或调度期间发生错误（定制点对象） |
| `execution::set_stopped`(C++26) | 停止完成函数，指示操作在成功或失败之前结束（定制点对象） |

### 四、发送者算法（Sender Algorithms）

#### 发送者工厂
发送者工厂是返回发送者的函数，其参数类型不满足`sender`概念。

| 组件 | 说明 |
|------|------|
| `execution::just`(C++26) | 接受可变数量的参数，返回发送者，连接和启动时通过将参数传递给接收者的值完成函数同步完成 |
| `execution::just_error`(C++26) | 接受单个参数，返回发送者，连接和启动时通过将参数传递给接收者的错误完成函数同步完成 |
| `execution::just_stopped`(C++26) | 创建立即通过调用其接收者的`set_stopped`完成的发送者 |
| `execution::read_env`(C++26) | 创建查询其接收者关联环境的发送者 |
| `execution::schedule`(C++26) | 准备在给定调度器上执行的任务图 |

#### 发送者适配器
发送者适配器是返回发送者的函数，其参数至少包含一个满足`sender`概念的类型，返回的发送者是适配器函数发送者参数的父发送者。

| 组件 | 说明 |
|------|------|
| `execution::starts_on`(C++26) | 适配提供的发送者，使其在提供的调度器执行资源上开始执行 |
| `execution::continues_on`(C++26) | 适配提供的发送者，使其在提供的调度器执行资源上完成 |
| `execution::then`(C++26) | 通过输入发送者与表示使用输入发送者发送的值作为参数调用提供的函数的节点链接任务图 |
| `execution::when_all`(C++26) | 适配多个输入发送者为一个发送者，该发送者在所有输入发送者完成后完成 |

#### 发送者消费者
发送者消费者是接受一个或多个发送者作为参数且不返回发送者的算法。

| 组件 | 说明 |
|------|------|
| `this_thread::sync_wait`(C++26) | 阻塞当前线程直到指定发送者完成并返回其异步结果 |

---

## 扩展知识详解

### 一、核心概念深入理解

#### 1. 发送者-接收者模型
```cpp
#include <execution>
#include <iostream>
#include <string>

// 简单的发送者示例
template<typename T>
class simple_sender {
    T value_;
    
public:
    explicit simple_sender(T value) : value_(std::move(value)) {}
    
    // 连接函数
    template<typename Receiver>
    auto connect(Receiver&& receiver) {
        struct operation_state {
            T value;
            Receiver recv;
            
            void start() noexcept {
                try {
                    std::execution::set_value(std::move(recv), std::move(value));
                } catch (...) {
                    std::execution::set_error(std::move(recv), std::current_exception());
                }
            }
        };
        
        return operation_state{std::move(value_), std::forward<Receiver>(receiver)};
    }
};

// 简单的接收者示例
struct simple_receiver {
    void set_value(const std::string& value) {
        std::cout << "Received value: " << value << std::endl;
    }
    
    void set_error(std::exception_ptr e) {
        std::cout << "Error occurred" << std::endl;
    }
    
    void set_stopped() {
        std::cout << "Operation stopped" << std::endl;
    }
};

void sender_receiver_example() {
    auto sender = simple_sender<std::string>("Hello, World!");
    auto receiver = simple_receiver{};
    auto op_state = std::execution::connect(std::move(sender), std::move(receiver));
    std::execution::start(op_state);
}
```

#### 2. 调度器实现
```cpp
#include <execution>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class thread_pool_scheduler {
private:
    std::vector<std::jthread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_{false};
    
public:
    thread_pool_scheduler(size_t num_threads = std::thread::hardware_concurrency()) {
        threads_.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            threads_.emplace_back([this]() {
                worker_thread();
            });
        }
    }
    
    ~thread_pool_scheduler() {
        stop_ = true;
        condition_.notify_all();
        for (auto& thread : threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
    
    // 调度器必须提供schedule函数
    auto schedule() const {
        struct sender {
            const thread_pool_scheduler* sched;
            
            template<typename Receiver>
            auto connect(Receiver&& receiver) const {
                struct operation_state {
                    const thread_pool_scheduler* sched;
                    Receiver recv;
                    
                    void start() noexcept {
                        sched->submit([recv = std::move(recv)]() mutable {
                            std::execution::set_value(std::move(recv));
                        });
                    }
                };
                
                return operation_state{sched, std::forward<Receiver>(receiver)};
            }
        };
        
        return sender{this};
    }
    
private:
    void worker_thread() {
        while (!stop_) {
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
    }
    
    template<typename F>
    void submit(F&& f) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (stop_) {
                throw std::runtime_error("Scheduler is stopped");
            }
            tasks_.emplace(std::forward<F>(f));
        }
        condition_.notify_one();
    }
};
```

### 二、现代异步编程模式

#### 1. 管道操作模式
```cpp
#include <execution>
#include <iostream>
#include <vector>
#include <string>

void pipeline_example() {
    namespace exec = std::execution;
    
    // 创建简单的管道
    auto pipeline = exec::just(42)
        | exec::then([](int x) { return x * 2; })
        | exec::then([](int x) { return std::to_string(x); })
        | exec::then([](const std::string& s) { 
            std::cout << "Result: " << s << std::endl; 
            return s.length();
        });
    
    // 同步等待结果
    auto result = std::this_thread::sync_wait(std::move(pipeline));
    if (result) {
        std::cout << "Final result: " << std::get<0>(*result) << std::endl;
    }
}
```

#### 2. 并行处理模式
```cpp
#include <execution>
#include <vector>
#include <iostream>

void parallel_processing_example() {
    namespace exec = std::execution;
    
    std::vector<int> data(1000);
    std::iota(data.begin(), data.end(), 1);
    
    // 创建并行处理管道
    auto sender = exec::just(std::move(data))
        | exec::bulk(4, [](int index, std::vector<int>& vec) {
            // 并行处理每个元素
            vec[index] = vec[index] * vec[index];
        })
        | exec::then([](std::vector<int> vec) {
            // 计算总和
            return std::reduce(vec.begin(), vec.end(), 0LL);
        });
    
    auto result = std::this_thread::sync_wait(std::move(sender));
    if (result) {
        std::cout << "Sum of squares: " << std::get<0>(*result) << std::endl;
    }
}
```

#### 3. 错误处理模式
```cpp
#include <execution>
#include <stdexcept>
#include <iostream>

void error_handling_example() {
    namespace exec = std::execution;
    
    auto sender = exec::just(10)
        | exec::then([](int x) -> int {
            if (x < 0) {
                throw std::runtime_error("Negative value not allowed");
            }
            return x * 2;
        })
        | exec::upon_error([](std::exception_ptr e) {
            try {
                std::rethrow_exception(e);
            } catch (const std::runtime_error& ex) {
                std::cout << "Caught error: " << ex.what() << std::endl;
                return -1;  // 返回默认值
            }
        });
    
    auto result = std::this_thread::sync_wait(std::move(sender));
    if (result) {
        std::cout << "Result: " << std::get<0>(*result) << std::endl;
    }
}
```

### 三、协程集成

#### 1. 协程与发送者集成
```cpp
#include <execution>
#include <coroutine>
#include <iostream>

// 使发送者可在协程中等待
template<typename Promise = void>
struct awaitable_sender {
    template<typename Sender>
    static auto as_awaitable(Sender&& sender) {
        struct awaiter {
            using result_type = std::decay_t<decltype(
                std::this_thread::sync_wait(std::forward<Sender>(sender))
            )>;
            
            Sender s;
            result_type result{};
            
            bool await_ready() const noexcept { return false; }
            
            void await_suspend(std::coroutine_handle<> h) {
                // 在后台执行发送者
                std::thread([s = std::move(s), h]() mutable {
                    result = std::this_thread::sync_wait(std::move(s));
                    h.resume();
                }).detach();
            }
            
            auto await_resume() {
                return std::move(result);
            }
        };
        
        return awaiter{std::forward<Sender>(sender)};
    }
};

// 协程示例
std::coroutine_handle<> async_coroutine_example() {
    co_await awaitable_sender<>::as_awaitable(std::execution::just(42));
    std::cout << "Coroutine resumed" << std::endl;
}
```

### 四、实际应用示例

#### 1. 异步HTTP客户端框架
```cpp
#include <execution>
#include <string>
#include <chrono>
#include <iostream>

// 模拟异步HTTP请求
class AsyncHttpClient {
public:
    auto get(const std::string& url) {
        struct sender {
            std::string url_;
            
            template<typename Receiver>
            auto connect(Receiver&& receiver) {
                struct operation_state {
                    std::string url;
                    Receiver recv;
                    
                    void start() noexcept {
                        // 模拟异步操作
                        std::thread([url = std::move(url), recv = std::move(recv)]() {
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                            std::string response = "Response from " + url;
                            std::execution::set_value(std::move(recv), std::move(response));
                        }).detach();
                    }
                };
                
                return operation_state{std::move(url_), std::forward<Receiver>(receiver)};
            }
        };
        
        return sender{url};
    }
};

void http_client_example() {
    namespace exec = std::execution;
    
    AsyncHttpClient client;
    
    auto requests = exec::when_all(
        client.get("http://example.com/api/1"),
        client.get("http://example.com/api/2"),
        client.get("http://example.com/api/3")
    );
    
    auto result = std::this_thread::sync_wait(std::move(requests));
    if (result) {
        const auto& [resp1, resp2, resp3] = *result;
        std::cout << "Response 1: " << std::get<0>(resp1) << std::endl;
        std::cout << "Response 2: " << std::get<0>(resp2) << std::endl;
        std::cout << "Response 3: " << std::get<0>(resp3) << std::endl;
    }
}
```

#### 2. 异步事件处理系统
```cpp
#include <execution>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class EventProcessor {
private:
    std::queue<std::function<void()>> event_queue_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> running_{true};
    std::jthread worker_;
    
public:
    EventProcessor() : worker_([this]() { event_loop(); }) {}
    
    ~EventProcessor() {
        running_ = false;
        condition_.notify_all();
    }
    
    template<typename Sender>
    void post(Sender&& sender) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            event_queue_.emplace([s = std::forward<Sender>(sender)]() mutable {
                std::this_thread::sync_wait(std::move(s));
            });
        }
        condition_.notify_one();
    }
    
    auto schedule() {
        struct scheduler {
            EventProcessor* processor;
            
            auto schedule() const {
                struct sender {
                    EventProcessor* processor;
                    
                    template<typename Receiver>
                    auto connect(Receiver&& receiver) {
                        struct operation_state {
                            EventProcessor* processor;
                            Receiver recv;
                            
                            void start() noexcept {
                                processor->post(std::execution::just() 
                                    | std::execution::then([recv = std::move(recv)]() mutable {
                                        std::execution::set_value(std::move(recv));
                                    })
                                );
                            }
                        };
                        
                        return operation_state{processor, std::forward<Receiver>(receiver)};
                    }
                };
                
                return sender{processor};
            }
        };
        
        return scheduler{this};
    }
    
private:
    void event_loop() {
        while (running_) {
            std::function<void()> event;
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                condition_.wait(lock, [this] { return !running_ || !event_queue_.empty(); });
                
                if (!running_ && event_queue_.empty()) {
                    break;
                }
                
                event = std::move(event_queue_.front());
                event_queue_.pop();
            }
            
            if (event) {
                event();
            }
        }
    }
};
```

#### 3. 异步文件I/O操作
```cpp
#include <execution>
#include <fstream>
#include <string>
#include <filesystem>

class AsyncFileIO {
public:
    auto read_file(const std::string& filename) {
        struct sender {
            std::string filename_;
            
            template<typename Receiver>
            auto connect(Receiver&& receiver) {
                struct operation_state {
                    std::string filename;
                    Receiver recv;
                    
                    void start() noexcept {
                        std::thread([filename = std::move(filename), 
                                   recv = std::move(recv)]() {
                            try {
                                std::ifstream file(filename);
                                if (!file.is_open()) {
                                    std::execution::set_error(
                                        std::move(recv),
                                        std::make_exception_ptr(
                                            std::runtime_error("Failed to open file")
                                        )
                                    );
                                    return;
                                }
                                
                                std::string content(
                                    (std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>()
                                );
                                
                                std::execution::set_value(std::move(recv), std::move(content));
                            } catch (...) {
                                std::execution::set_error(
                                    std::move(recv),
                                    std::current_exception()
                                );
                            }
                        }).detach();
                    }
                };
                
                return operation_state{std::move(filename_), std::forward<Receiver>(receiver)};
            }
        };
        
        return sender{filename};
    }
    
    auto write_file(const std::string& filename, const std::string& content) {
        struct sender {
            std::string filename_;
            std::string content_;
            
            template<typename Receiver>
            auto connect(Receiver&& receiver) {
                struct operation_state {
                    std::string filename;
                    std::string content;
                    Receiver recv;
                    
                    void start() noexcept {
                        std::thread([filename = std::move(filename),
                                   content = std::move(content),
                                   recv = std::move(recv)]() {
                            try {
                                std::ofstream file(filename);
                                if (!file.is_open()) {
                                    std::execution::set_error(
                                        std::move(recv),
                                        std::make_exception_ptr(
                                            std::runtime_error("Failed to create file")
                                        )
                                    );
                                    return;
                                }
                                
                                file << content;
                                std::execution::set_value(std::move(recv));
                            } catch (...) {
                                std::execution::set_error(
                                    std::move(recv),
                                    std::current_exception()
                                );
                            }
                        }).detach();
                    }
                };
                
                return operation_state{
                    std::move(filename_), 
                    std::move(content_), 
                    std::forward<Receiver>(receiver)
                };
            }
        };
        
        return sender{filename, content};
    }
};

void file_io_example() {
    namespace exec = std::execution;
    
    AsyncFileIO file_io;
    
    auto pipeline = file_io.read_file("input.txt")
        | exec::then([](std::string content) {
            // 处理文件内容
            std::transform(content.begin(), content.end(), content.begin(), ::toupper);
            return content;
        })
        | exec::then([](std::string content) {
            return std::make_pair("output.txt", std::move(content));
        })
        | exec::let_value([](std::pair<std::string, std::string> file_data) {
            AsyncFileIO io;
            return io.write_file(file_data.first, file_data.second);
        });
    
    auto result = std::this_thread::sync_wait(std::move(pipeline));
    if (result) {
        std::cout << "File processing completed successfully" << std::endl;
    }
}
```

## 示例代码详解

### 原始示例分析
```cpp
#include <cstdio>
#include <execution>
#include <string>
#include <thread>
#include <utility>
using namespace std::literals;

int main()
{
    std::execution::run_loop loop;

    std::jthread worker(&{
        std::stop_callback cb{st, [&]{ loop.finish(); }};
        loop.run();
    });

    std::execution::sender auto hello = std::execution::just("hello world"s);
    std::execution::sender auto print
        = std::move(hello)
        | std::execution::then([](std::string msg)
        {
            return std::puts(msg.c_str());
        });

    std::execution::scheduler auto io_thread = loop.get_scheduler();
    std::execution::sender auto work = std::execution::on(io_thread, std::move(print));

    auto [result] = std::this_thread::sync_wait(std::move(work)).value();

    return result;
}
```

### 输出结果
```
hello world
```

### 代码要点解析

1. **run_loop**: 创建事件循环执行上下文
2. **后台工作线程**: 启动工作线程运行事件循环
3. **发送者创建**: 使用`just`创建发送值的发送者
4. **管道操作**: 使用`|`操作符链接发送者适配器
5. **调度器使用**: 获取调度器并在其上执行工作
6. **同步等待**: 阻塞等待异步操作完成

## 相关页面

| 页面 | 说明 |
|------|------|
| `async`(C++11) | 异步运行函数（可能在新线程中）并返回保存结果的std::future（函数模板） |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/execution&oldid=179211>
- 最后修改时间：2025年1月3日 12:59
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++执行库代表了现代C++异步编程的重大进步，提供了基于发送者-接收者模型的统一异步编程框架。理解核心概念如发送者、接收者、调度器和操作状态，是掌握现代异步编程的基础。通过管道操作符和组合算法，开发者可以以声明式的方式构建复杂的异步任务图。执行库的设计遵循零开销抽象原则，确保高性能的同时提供了强大的表达能力。正确使用调度器和执行上下文，可以让异步操作在合适的执行资源上运行。协程与发送者的集成，为现代C++提供了统一的异步编程体验。持续学习和实践执行库的新特性，是跟上C++现代化发展步伐的必要途径。执行库的成功应用将显著提高C++程序的并发性和响应性，是现代高性能C++应用开发的关键技术。