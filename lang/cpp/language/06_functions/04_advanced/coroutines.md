# C++ 协程 (Coroutines)

## 1. 概述

**协程（Coroutine）** 是 C++20 引入的一种可以暂停执行并在稍后恢复的函数。与传统的函数不同，协程可以在执行过程中挂起，将控制权返回给调用者，并在稍后从暂停点继续执行。

协程是**无栈（stackless）** 的：它们通过返回给调用者来暂停执行，恢复执行所需的数据与调用栈分开存储。这种设计使得协程能够：

- 以同步代码风格编写异步逻辑（如处理非阻塞 I/O 而无需显式回调）
- 支持惰性计算的无限序列算法
- 实现协作式多任务

### 协程的判定

一个函数如果其定义中包含以下任何关键字，则该函数为协程：

- **`co_await` 表达式** — 暂停执行直到被恢复
- **`co_yield` 表达式** — 暂停执行并返回一个值
- **`co_return` 语句** — 完成执行并返回一个值

### 协程核心组件

每个协程都与以下三个核心组件相关联：

| 组件 | 说明 |
|------|------|
| **Promise 对象** | 从协程内部操作，协程通过此对象提交结果或异常 |
| **协程句柄** | 从协程外部操作，非拥有句柄，用于恢复执行或销毁协程帧 |
| **协程状态** | 内部动态分配的存储，包含 Promise 对象、参数副本、当前暂停点信息、跨越暂停点的局部变量和临时对象 |

## 2. 来源与演变

### 首次引入

协程在 **C++20** 标准中正式引入，是 C++ 语言特性的重大扩展。

### 历史背景

在协程引入之前，C++ 开发者处理异步操作主要依赖：

1. **回调函数**：代码结构复杂，易产生"回调地狱"
2. **线程**：开销大，同步困难
3. **状态机**：手动管理状态，维护成本高

协程的设计目标：
- 提供一种编写异步代码的同步风格
- 零开销抽象，不引入额外运行时成本
- 与现有 C++ 代码无缝集成

### 版本演进

| 标准 | 特性测试宏 | 说明 |
|------|-----------|------|
| C++20 | `__cpp_impl_coroutine` (201902L) | 编译器支持 |
| C++20 | `__cpp_lib_coroutine` (201902L) | 库支持 |
| C++23 | `__cpp_lib_generator` (202207L) | `std::generator` 同步协程生成器 |

### 缺陷报告

| 编号 | 问题 | 修正 |
|------|------|------|
| CWG 2556 | 无效的 `return_void` 导致协程结束行为未定义 | 程序在此情况下格式错误 |
| CWG 2668 | `co_await` 不能出现在 lambda 表达式中 | 已允许 |
| CWG 2754 | 显式对象成员函数构造 Promise 对象时错误地复制 `*this` | 不在此情况下复制 |

## 3. 语法与参数

### 协程关键字

#### co_await 表达式

`co_await` 是一元操作符，用于暂停协程并将控制权返回给调用者。

```cpp
co_await expr
```

**出现位置限制**：
- 只能出现在潜在求值表达式中
- 不能出现在异常处理器中
- 不能出现在声明语句中（除非在初始化器中）
- 不能出现在默认参数中
- 不能出现在静态或线程存储期变量的初始化器中

#### co_yield 表达式

`co_yield` 用于返回值给调用者并暂停当前协程，是可恢复生成器函数的基础构建块。

```cpp
co_yield expr
co_yield braced-init-list
```

等价于：

```cpp
co_await promise.yield_value(expr)
```

#### co_return 语句

`co_return` 用于完成协程执行并返回值。

```cpp
co_return;           // 调用 promise.return_void()
co_return expr;      // 若 expr 类型为 void，调用 promise.return_void()
                     // 否则调用 promise.return_value(expr)
```

### 协程限制

协程**不能**使用：

- 可变参数（variadic arguments）
- 普通的 `return` 语句
- 占位符返回类型（`auto` 或 Concept）

以下函数**不能**是协程：

- `consteval` 函数
- `constexpr` 函数
- 构造函数
- 析构函数
- `main` 函数

### Promise 类型确定

Promise 类型由编译器通过 `std::coroutine_traits` 从协程返回类型推导：

| 协程定义 | Promise 类型 |
|----------|-------------|
| `task<void> foo(int x);` | `std::coroutine_traits<task<void>, int>::promise_type` |
| `task<void> Bar::foo(int x) const;` | `std::coroutine_traits<task<void>, const Bar&, int>::promise_type` |
| `task<void> Bar::foo(int x) &&;` | `std::coroutine_traits<task<void>, Bar&&, int>::promise_type` |

### Promise 类型必需接口

一个完整的 Promise 类型应实现以下方法：

```cpp
struct promise_type {
    // 获取返回对象
    auto get_return_object();

    // 初始暂停点
    auto initial_suspend();  // 通常返回 std::suspend_always 或 std::suspend_never

    // 最终暂停点
    auto final_suspend() noexcept;  // 通常返回 std::suspend_always

    // 返回值处理
    void return_void();       // 对应 co_return;
    void return_value(T v);   // 对应 co_return expr;

    // 异常处理
    void unhandled_exception();

    // 可选：co_yield 支持
    auto yield_value(T v);
};
```

## 4. 底层原理

### 协程执行流程

当协程开始执行时，它会执行以下步骤：

1. **分配协程状态**：使用 `operator new` 分配协程状态对象
2. **复制参数**：将所有函数参数复制到协程状态中
3. **构造 Promise 对象**：调用 Promise 构造函数
4. **获取返回对象**：调用 `promise.get_return_object()` 并保存结果
5. **初始暂停**：调用 `promise.initial_suspend()` 并 `co_await` 其结果
6. **执行协程体**：当 `co_await promise.initial_suspend()` 恢复后，开始执行协程体

### co_await 执行机制

`co_await expr` 的执行过程：

1. **转换表达式为可等待对象**：
   - 如果 `Promise` 类型有 `await_transform` 成员函数，调用 `promise.await_transform(expr)`
   - 否则直接使用 `expr`

2. **获取等待器（awaiter）对象**：
   - 如果 `operator co_await` 重载解析成功，调用该操作符
   - 否则直接使用可等待对象

3. **查询就绪状态**：
   - 调用 `awaiter.await_ready()`
   - 如果返回 `true`，跳过暂停，直接调用 `awaiter.await_resume()`

4. **暂停协程**（如果 `await_ready()` 返回 `false`）：
   - 填充协程状态
   - 调用 `awaiter.await_suspend(handle)`
   - 根据返回值：
     - `void`：控制权返回调用者
     - `bool`：`true` 返回调用者，`false` 恢复当前协程
     - `coroutine_handle`：恢复指定协程

5. **恢复执行**：
   - 调用 `awaiter.await_resume()` 获取结果

### 动态内存分配

协程状态通过 `operator new` 动态分配：

**分配优化**：如果满足以下条件，编译器可以优化掉动态分配：

- 协程状态的生命周期严格嵌套在调用者生命周期内
- 协程帧大小在调用点已知

此时协程状态可以嵌入调用者的栈帧（普通函数）或协程状态（协程调用协程）。

**分配失败处理**：

- 默认抛出 `std::bad_alloc`
- 如果 Promise 类型定义了 `get_return_object_on_allocation_failure()` 静态方法：
  - 使用 `nothrow` 形式的 `operator new`
  - 分配失败时返回该方法获取的对象

### 协程销毁流程

协程状态销毁时执行：

1. 调用 Promise 对象的析构函数
2. 调用函数参数副本的析构函数
3. 调用 `operator delete` 释放协程状态内存
4. 将执行权返回给调用者/恢复者

### 线程安全注意事项

协程句柄可以在不同线程间共享：

- 在 `await_suspend()` 返回前，协程句柄可能已被其他线程恢复
- `await_suspend()` 应将 `*this` 视为已销毁状态
- 需要适当的同步机制（至少使用 release/acquire 语义）

## 5. 使用场景

### 适合使用协程的场景

| 场景 | 优势 |
|------|------|
| 异步 I/O 操作 | 以同步风格编写异步代码，避免回调地狱 |
| 生成器模式 | 惰性生成无限序列 |
| 状态机实现 | 将状态转换编码为控制流 |
| 协作式多任务 | 用户态调度，低开销切换 |
| 异步网络编程 | 清晰的请求/响应处理流程 |

### 最佳实践

1. **参数传递**：按值传递参数以避免悬垂引用

```cpp
// 正确：按值传递，参数被复制到协程帧
coroutine good(int i) {
    std::cout << i;
    co_return;
}

// 错误：按引用传递可能导致悬垂引用
coroutine bad(int& i) {
    // i 可能已失效
    co_return;
}
```

2. **生命周期管理**：确保协程帧的生命周期正确管理

```cpp
// 正确：使用智能指针或 RAII 包装器
auto coro = [](int i) -> coroutine {
    std::cout << i;
    co_return;
}(0);  // i 被复制到协程帧
```

3. **异常处理**：在 Promise 类型中正确处理异常

```cpp
void unhandled_exception() {
    exception_ = std::current_exception();
}
```

4. **内存分配**：对于性能敏感场景，自定义分配器

### 常见陷阱

#### 陷阱 1：悬垂引用

协程按引用传递参数可能导致悬垂引用：

```cpp
struct S {
    int i;
    coroutine f() {
        std::cout << i;  // 危险：this 可能已失效
        co_return;
    }
};

void bad() {
    coroutine h = S{0}.f();  // S{0} 已销毁
    h.resume();  // 未定义行为
}
```

#### 陷阱 2：从协程末尾掉落

如果协程体没有 `co_return` 且 Promise 没有定义 `return_void()`，行为未定义：

```cpp
task<void> f() {
    // 不是协程，未定义行为
}

task<void> g() {
    co_return;  // 正确
}
```

#### 陷阱 3：在最终暂停点后恢复协程

在 `final_suspend()` 后恢复协程是未定义行为。

## 6. 代码示例

### 基础用法：切换线程执行

```cpp
#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <thread>

auto switch_to_new_thread(std::jthread& out)
{
    struct awaitable
    {
        std::jthread* p_out;
        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<> h)
        {
            std::jthread& out = *p_out;
            if (out.joinable())
                throw std::runtime_error("Output jthread parameter not empty");
            out = std::jthread([h] { h.resume(); });
            std::cout << "New thread ID: " << out.get_id() << '\n';
        }
        void await_resume() {}
    };
    return awaitable{&out};
}

struct task
{
    struct promise_type
    {
        task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

task resuming_on_new_thread(std::jthread& out)
{
    std::cout << "Coroutine started on thread: " << std::this_thread::get_id() << '\n';
    co_await switch_to_new_thread(out);
    std::cout << "Coroutine resumed on thread: " << std::this_thread::get_id() << '\n';
}

int main()
{
    std::jthread out;
    resuming_on_new_thread(out);
}
```

**可能的输出**：
```
Coroutine started on thread: 139972277602112
New thread ID: 139972267284224
Coroutine resumed on thread: 139972267284224
```

### 高级用法：生成器实现

以下示例展示一个完整的斐波那契数列生成器：

```cpp
#include <coroutine>
#include <cstdint>
#include <exception>
#include <iostream>

template<typename T>
struct Generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type
    {
        T value_;
        std::exception_ptr exception_;

        Generator get_return_object()
        {
            return Generator(handle_type::from_promise(*this));
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { exception_ = std::current_exception(); }

        template<std::convertible_to<T> From>
        std::suspend_always yield_value(From&& from)
        {
            value_ = std::forward<From>(from);
            return {};
        }
        void return_void() {}
    };

    handle_type h_;

    Generator(handle_type h) : h_(h) {}
    ~Generator() { h_.destroy(); }

    explicit operator bool()
    {
        fill();
        return !h_.done();
    }

    T operator()()
    {
        fill();
        full_ = false;
        return std::move(h_.promise().value_);
    }

private:
    bool full_ = false;

    void fill()
    {
        if (!full_)
        {
            h_();
            if (h_.promise().exception_)
                std::rethrow_exception(h_.promise().exception_);
            full_ = true;
        }
    }
};

Generator<std::uint64_t>
fibonacci_sequence(unsigned n)
{
    if (n == 0)
        co_return;

    if (n > 94)
        throw std::runtime_error("Too big Fibonacci sequence. Elements would overflow.");

    co_yield 0;

    if (n == 1)
        co_return;

    co_yield 1;

    if (n == 2)
        co_return;

    std::uint64_t a = 0;
    std::uint64_t b = 1;

    for (unsigned i = 2; i < n; ++i)
    {
        std::uint64_t s = a + b;
        co_yield s;
        a = b;
        b = s;
    }
}

int main()
{
    try
    {
        auto gen = fibonacci_sequence(10);

        for (int j = 0; gen; ++j)
            std::cout << "fib(" << j << ")=" << gen() << '\n';
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << '\n';
    }
}
```

**输出**：
```
fib(0)=0
fib(1)=1
fib(2)=1
fib(3)=2
fib(4)=3
fib(5)=5
fib(6)=8
fib(7)=13
fib(8)=21
fib(9)=34
```

### 高级用法：可配置暂停行为

```cpp
#include <cassert>
#include <coroutine>
#include <iostream>

struct tunable_coro
{
    class tunable_awaiter
    {
        bool ready_;
    public:
        explicit(false) tunable_awaiter(bool ready) : ready_{ready} {}
        bool await_ready() const noexcept { return ready_; }
        static void await_suspend(std::coroutine_handle<>) noexcept {}
        static void await_resume() noexcept {}
    };

    struct promise_type
    {
        using coro_handle = std::coroutine_handle<promise_type>;
        auto get_return_object() { return coro_handle::from_promise(*this); }
        static auto initial_suspend() { return std::suspend_always(); }
        static auto final_suspend() noexcept { return std::suspend_always(); }
        static void return_void() {}
        static void unhandled_exception() { std::terminate(); }

        // 自定义 await_transform
        auto await_transform(std::suspend_always) {
            return tunable_awaiter(!ready_);
        }
        void disable_suspension() { ready_ = false; }
    private:
        bool ready_{true};
    };

    tunable_coro(promise_type::coro_handle h) : handle_(h) { assert(h); }

    tunable_coro(tunable_coro const&) = delete;
    tunable_coro(tunable_coro&&) = delete;
    tunable_coro& operator=(tunable_coro const&) = delete;
    tunable_coro& operator=(tunable_coro&&) = delete;

    ~tunable_coro()
    {
        if (handle_)
            handle_.destroy();
    }

    void disable_suspension() const
    {
        if (handle_.done())
            return;
        handle_.promise().disable_suspension();
        handle_();
    }

    bool operator()()
    {
        if (!handle_.done())
            handle_();
        return !handle_.done();
    }
private:
    promise_type::coro_handle handle_;
};

tunable_coro generate(int n)
{
    for (int i{}; i != n; ++i)
    {
        std::cout << i << ' ';
        co_await std::suspend_always{};
    }
}

int main()
{
    auto coro = generate(8);
    coro();  // 输出 0
    for (int k{}; k < 4; ++k)
    {
        coro();  // 输出 1 2 3 4
        std::cout << ": ";
    }
    coro.disable_suspension();
    coro();  // 输出剩余的 5 6 7
}
```

**输出**：
```
0 1 : 2 : 3 : 4 : 5 6 7
```

### 常见错误及修正

#### 错误 1：使用普通 return 语句

```cpp
// 错误：协程不能使用普通 return
task<int> bad() {
    return 42;  // 编译错误
}

// 正确：使用 co_return
task<int> good() {
    co_return 42;
}
```

#### 错误 2：悬垂引用

```cpp
// 错误：lambda 销毁后访问捕获的变量
void bad() {
    coroutine h = [i = 0]() -> coroutine {
        std::cout << i;  // i 已失效
        co_return;
    }();
    h.resume();
    h.destroy();
}

// 正确：按值传递参数
void good() {
    coroutine h = [](int i) -> coroutine {
        std::cout << i;  // i 被复制到协程帧
        co_return;
    }(0);
    h.resume();
    h.destroy();
}
```

#### 错误 3：忘记定义 return_void

```cpp
// 错误：没有定义 return_void，从协程末尾掉落是未定义行为
struct bad_promise {
    // ... 缺少 return_void
};

// 正确：定义 return_void
struct good_promise {
    void return_void() {}  // 处理 co_return; 或隐式返回
};
```

## 7. 总结

C++20 协程是一种强大的语言特性，为异步编程和惰性计算提供了优雅的解决方案。

### 核心要点

| 特性 | 说明 |
|------|------|
| **无栈设计** | 协程状态与调用栈分离，开销小 |
| **三关键字** | `co_await`、`co_yield`、`co_return` 控制协程行为 |
| **Promise 模式** | 通过 Promise 类型定制协程行为 |
| **编译器支持** | 协程帧管理由编译器自动处理 |

### 技术对比

| 特性 | 协程 | 线程 | 回调 |
|------|------|------|------|
| 上下文切换开销 | 极低（用户态） | 高（内核态） | 无 |
| 代码可读性 | 高（同步风格） | 中 | 低 |
| 调度方式 | 协作式 | 抢占式 | 事件驱动 |
| 适用场景 | I/O 密集型 | CPU 密集型 | 事件处理 |

### 学习建议

1. **从生成器开始**：先理解 `co_yield` 的使用，这是协程最直观的应用
2. **理解 Promise 契约**：Promise 类型定义了协程的行为
3. **注意生命周期**：协程帧的生命周期管理是正确使用协程的关键
4. **使用现有库**：如 `cppcoro`、`folly::coro` 等成熟库，避免重复造轮子
5. **阅读标准库源码**：C++23 的 `std::generator` 是学习协程实现的优秀参考

### 相关头文件

```cpp
#include <coroutine>  // C++20 协程支持库
```

### 标准库支持类型

| 类型 | 说明 |
|------|------|
| `std::coroutine_handle` | 非拥有协程句柄 |
| `std::coroutine_traits` | 协程特征萃取 |
| `std::suspend_always` | 总是暂停的等待器 |
| `std::suspend_never` | 从不暂停的等待器 |
| `std::generator` (C++23) | 同步协程生成器 |

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/coroutines
- C++20 标准: [dcl.fct.def.coroutine]
- Lewis Baker, Asymmetric Transfer 博客系列
- David Mazières, C++20 Coroutines Tutorial