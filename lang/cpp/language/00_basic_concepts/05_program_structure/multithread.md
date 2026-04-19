# 多线程执行与数据竞争 (Multi-threaded Executions and Data Races)

> C++11 起

## 1. 概述 (Overview)

**执行线程 (Thread of Execution)** 是程序内的控制流，始于特定顶层函数的调用（通过 `std::thread`、`std::async`、`std::jthread`（C++20 起）或其他方式），并递归地包含该线程随后执行的每个函数调用。

### 核心概念

- **线程创建**：当一个线程创建另一个线程时，新线程顶层函数的初始调用由新线程执行，而非创建线程
- **对象访问**：任何线程都可能访问程序中的任何对象和函数
  - 自动存储期和线程局部存储期的对象仍可通过指针或引用被其他线程访问
- **并发执行**：在宿主实现下，C++ 程序可以有多个线程并发运行

### 实现环境差异

| 实现类型 | 多线程支持 |
|---------|-----------|
| 宿主实现 (Hosted) | 支持多个线程并发运行 |
| 独立实现 (Freestanding) | 是否支持多线程由实现定义 |

**信号处理器说明**：对于非通过 `std::raise` 调用执行的信号处理器，其所在的执行线程是未指定的。

## 2. 来源与演变 (Origin and Evolution)

### 设计动机

C++11 引入多线程支持的设计目标：

1. **标准化并发编程** - 提供跨平台的多线程编程模型
2. **内存模型定义** - 明确定义多线程环境下的内存访问语义
3. **数据竞争检测** - 为程序员提供检测和避免数据竞争的规则
4. **原子操作支持** - 提供无锁编程的基础设施

### 标准演进

| C++ 版本 | 变更内容 |
|---------|---------|
| C++11 | 首次引入线程支持库、原子操作、内存模型 |
| C++17 | 新增并行算法和前向进度保证委托机制 |
| C++20 | 引入 `std::jthread`（自动 join 的线程） |

### 缺陷报告详解

**CWG 1953**：C++11 标准最初规定，开始/结束存储重叠对象生命周期的两个表达式求值不冲突。修正后规定这种情况也构成冲突。

**LWG 2200**：C++11 标准中不清楚容器数据竞争要求是否仅适用于序列容器。修正后明确适用于所有容器。

**P2809R3**：C++11 标准中执行"平凡"无限循环的行为是未定义的。修正后正确定义了"平凡无限循环"并使其行为良好定义。

## 3. 语法与参数 (Syntax and Parameters)

### 数据竞争判定规则

两个表达式求值**冲突 (Conflict)**，如果：
- 其中一个修改内存位置或开始/结束该内存位置中对象的生命周期
- 另一个读取或修改同一内存位置，或开始/结束占用存储与该内存位置重叠的对象的生命周期

程序存在两个冲突的求值即产生**数据竞争 (Data Race)**，除非满足以下任一条件：

| 免除条件 | 说明 |
|---------|------|
| 同一线程 | 两个求值在同一线程或同一信号处理器中执行 |
| 原子操作 | 两个冲突的求值都是原子操作（参见 `std::atomic`） |
| 先序关系 | 一个冲突的求值 happens-before 另一个（参见 `std::memory_order`） |

**重要后果**：如果发生数据竞争，程序行为是**未定义的**。

### 平凡无限循环语法

平凡空迭代语句匹配以下形式之一：

```cpp
while (condition) ;                    // (1) 空 simple statement
while (condition) { }                  // (2) 空 compound statement
do ; while (condition) ;               // (3) 空 simple statement
do { } while (condition) ;             // (4) 空 compound statement
for (init-statement condition(opt) ; ) ;   // (5) 空 simple statement
for (init-statement condition(opt) ; ) { } // (6) 空 compound statement
```

**控制表达式**：
- 形式 1-4：condition 本身
- 形式 5-6：如果存在 condition 则为 condition，否则为 true

**平凡无限循环**：平凡空迭代语句，其转换后的控制表达式是常量表达式（显式常量求值时），且求值为 true。

## 4. 底层原理 (Underlying Principles)

### 4.1 内存位置与并发访问

不同执行线程始终被允许并发访问（读取和修改）不同的内存位置，无需同步，不会产生干扰。

```cpp
struct Data {
    int a;
    int b;
};

Data data;
// 线程 1 修改 data.a
// 线程 2 修改 data.b
// OK：不同内存位置，无数据竞争
```

### 4.2 happens-before 关系

**happens-before** 是一个关键同步概念：

- 如果操作 A happens-before 操作 B，则 A 的效果对 B 可见
- `std::mutex` 的释放操作 synchronized-with（因而 happens-before）同一互斥量的获取操作

```cpp
std::mutex mtx;
int data = 0;

// 线程 1
{
    std::lock_guard<std::mutex> lock(mtx);
    data = 42;  // 写入数据
}  // 释放 mutex

// 线程 2
{
    std::lock_guard<std::mutex> lock(mtx);  // 获取 mutex
    // 此时 data == 42 必定可见
}
```

### 4.3 内存顺序 (Memory Order)

当线程从内存位置读取值时，可能看到：
- 初始值
- 同一线程写入的值
- 其他线程写入的值

写入何时对其他线程可见由内存顺序规则控制，详见 `std::memory_order`。

### 4.4 前向进度保证 (Forward Progress Guarantees)

#### 阻塞自由 (Obstruction Freedom)

当只有一个未在标准库函数中阻塞的线程执行无锁原子函数时，该执行保证完成。所有标准库无锁操作都是阻塞自由的。

#### 锁自由 (Lock Freedom)

当一个或多个无锁原子函数并发运行时，至少有一个保证完成。所有标准库无锁操作都是锁自由的——实现的职责是确保它们不会被其他线程无限期活锁（例如通过持续窃取缓存行）。

#### 进度保证规则

在有效的 C++ 程序中，每个线程最终会执行以下操作之一：

1. 终止
2. 调用 `std::this_thread::yield`
3. 调用库 I/O 函数
4. 通过 volatile 泛左值执行访问
5. 执行原子操作或同步操作
6. 继续执行平凡无限循环

**进度定义**：如果线程执行上述执行步骤之一、在标准库函数中阻塞，或调用因非阻塞并发线程而未完成的无锁原子函数，则称该线程取得进度。

### 4.5 编译器优化与无限循环

编译器可以移除、合并和重排所有没有可观察行为的循环，而无需证明它们最终会终止，因为编译器可以假设没有线程可以在不执行任何可观察行为的情况下永远执行。

**例外**：平凡无限循环不能被移除或重排。

```cpp
for (;;);             // 平凡无限循环，行为良好定义（P2809 起）
for (;;) { int x; }   // 非平凡无限循环，未定义行为
```

平凡无限循环的循环体被替换为调用 `std::this_thread::yield`。在独立实现上是否进行此替换由实现定义。

## 5. 使用场景 (Use Cases)

### 5.1 避免数据竞争

**使用互斥量**：

```cpp
#include <mutex>

int counter = 0;
std::mutex counter_mtx;

void increment() {
    std::lock_guard<std::mutex> lock(counter_mtx);
    ++counter;  // 安全：互斥量保护
}
```

**使用原子操作**：

```cpp
#include <atomic>

std::atomic<int> counter{0};

void increment() {
    ++counter;  // 安全：原子操作
}
```

### 5.2 容器的线程安全

标准库容器（除 `std::vector<bool>` 外）保证对同一容器中不同元素的并发修改不会导致数据竞争：

```cpp
std::vector<int> vec = {1, 2, 3, 4};

// OK：修改不同元素
auto f = [&](int index) { vec[index] = 5; };
std::thread t1{f, 0}, t2{f, 1};  // OK

// 未定义行为：修改同一元素
std::thread t3{f, 2}, t4{f, 2};  // UB！
```

**`std::vector<bool>` 特殊情况**：

```cpp
std::vector<bool> vec = {false, false};

// 即使修改不同元素，也是未定义行为！
auto f = [&](int index) { vec[index] = true; };
std::thread t1{f, 0}, t2{f, 1};  // UB！
```

`std::vector<bool>` 的特殊性在于其元素不是独立的内存位置（它是位打包存储）。

### 5.3 前向进度保证类型

| 保证类型 | 说明 | 适用场景 |
|---------|------|---------|
| 并发前向进度 | 在有限时间内取得进度，不受其他线程影响 | 主线程、`std::thread` 创建的线程（鼓励但不强制） |
| 并行前向进度 | 未执行步骤前不保证，执行步骤后提供并发前向进度保证 | 线程池中的工作线程 |
| 弱并行前向进度 | 不保证最终取得进度 | 需要通过前向进度委托获得保证 |

### 5.4 前向进度委托

弱并行前向进度保证的线程可以通过**前向进度保证委托**获得进度保证：

- 如果线程 P 在一组线程 S 的完成上阻塞
- 则 S 中至少有一个线程提供与 P 相同或更强的前向进度保证
- 该线程完成后，S 中另一个线程获得类似的增强
- S 为空后，P 解除阻塞

C++17 标准库的并行算法在库管理线程的完成上使用前向进度委托阻塞。

## 6. 代码示例 (Examples)

### 6.1 基础用法：正确使用原子操作避免数据竞争

```cpp
#include <atomic>
#include <thread>
#include <iostream>

int main() {
    // 错误示例：非原子操作导致数据竞争
    // int cnt = 0;
    // auto f = [&] { cnt++; };
    // std::thread t1{f}, t2{f}, t3{f}; // 未定义行为！
    // t1.join(); t2.join(); t3.join();

    // 正确做法：使用原子操作
    std::atomic<int> cnt{0};
    auto f = [&] { cnt++; };
    std::thread t1{f}, t2{f}, t3{f};  // OK
    t1.join(); t2.join(); t3.join();

    std::cout << "Count: " << cnt << std::endl;  // 输出: Count: 3
    return 0;
}
```

### 6.2 基础用法：使用互斥量保护共享数据

```cpp
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>

class Counter {
    int value = 0;
    std::mutex mtx;

public:
    void increment() {
        std::lock_guard<std::mutex> lock(mtx);
        ++value;
    }

    int get() const {
        std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mtx));
        return value;
    }
};

int main() {
    Counter counter;

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter] {
            for (int j = 0; j < 1000; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final value: " << counter.get() << std::endl;  // 输出: 10000
    return 0;
}
```

### 6.3 高级用法：容器的并发安全操作

```cpp
#include <vector>
#include <thread>
#include <iostream>

int main() {
    // 安全：修改不同元素
    std::vector<int> vec = {1, 2, 3, 4, 5};

    auto writer = [&](int index, int value) {
        vec[index] = value;  // 不同索引安全
    };

    std::thread t1(writer, 0, 10);
    std::thread t2(writer, 1, 20);
    std::thread t3(writer, 2, 30);

    t1.join(); t2.join(); t3.join();

    for (int v : vec) {
        std::cout << v << " ";  // 输出: 10 20 30 4 5
    }
    std::cout << std::endl;

    return 0;
}
```

### 6.4 高级用法：平凡无限循环

```cpp
#include <thread>

void worker() {
    // 平凡无限循环 - 行为良好定义（C++11 起，P2809 明确）
    // 循环体会被替换为 std::this_thread::yield() 调用
    for (;;) {
        // 空循环体
    }
}

int main() {
    // 这会导致线程永远运行（除非外部终止）
    // 但行为是定义良好的，不是 UB
    // std::thread t(worker);
    // t.detach();

    // 非平凡无限循环 - 未定义行为
    // for (;;) {
    //     int x;  // 有声明，非平凡
    // }

    return 0;
}
```

### 6.5 高级用法：并行算法的前向进度保证

```cpp
#include <algorithm>
#include <vector>
#include <execution>

int main() {
    std::vector<int> data(1000000, 1);

    // C++17 并行算法
    // 执行策略使用库管理线程池
    // 通过前向进度委托保证进度
    std::for_each(std::execution::par, data.begin(), data.end(),
        [](int& x) {
            x *= 2;
        });

    return 0;
}
```

### 6.6 常见错误及修正

**错误示例 1：非原子操作导致数据竞争**

```cpp
// 错误代码
int counter = 0;

void increment() {
    counter++;  // 非原子操作
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();
    // counter 可能不是 2，行为未定义！
    return 0;
}
```

```cpp
// 修正方法 1：使用原子操作
#include <atomic>

std::atomic<int> counter{0};

void increment() {
    counter++;  // 原子操作
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();
    // counter 保证为 2
    return 0;
}
```

```cpp
// 修正方法 2：使用互斥量
#include <mutex>

int counter = 0;
std::mutex mtx;

void increment() {
    std::lock_guard<std::mutex> lock(mtx);
    counter++;
}
```

**错误示例 2：`std::vector<bool>` 的并发访问**

```cpp
// 错误代码
#include <vector>

std::vector<bool> flags(10, false);

void set_flag(int index) {
    flags[index] = true;  // 即使索引不同，也是 UB！
}

int main() {
    std::thread t1(set_flag, 0);
    std::thread t2(set_flag, 1);  // UB！
    t1.join();
    t2.join();
    return 0;
}
```

```cpp
// 修正方法：使用其他容器或同步
#include <vector>
#include <mutex>

std::vector<char> flags(10, 0);  // 使用 char 代替 bool
std::mutex mtx;

void set_flag(int index) {
    std::lock_guard<std::mutex> lock(mtx);
    flags[index] = 1;
}
```

**错误示例 3：非平凡无限循环**

```cpp
// 错误代码
void bad_loop() {
    for (;;) {
        int x = 0;  // 有变量声明
        x++;
    }
    // 行为未定义！
}
```

```cpp
// 修正方法：使用原子操作或 yield
#include <atomic>
#include <thread>

std::atomic<bool> running{true};

void good_loop() {
    while (running) {
        // 执行工作或使用 yield
        std::this_thread::yield();
    }
}
```

## 7. 总结 (Summary)

### 核心要点

1. **数据竞争定义**：两个冲突的表达式求值，如果不同时在线程执行、不是原子操作、也没有 happens-before 关系，则产生数据竞争

2. **避免数据竞争的方法**：
   - 使用互斥量（`std::mutex`）
   - 使用原子操作（`std::atomic`）
   - 确保正确的同步关系

3. **容器线程安全**：
   - 标准容器保证不同元素的并发修改安全
   - `std::vector<bool>` 是例外，其元素共享内存位置

4. **前向进度保证**：
   - 阻塞自由：单个线程的无锁操作保证完成
   - 锁自由：并发无锁操作至少一个完成
   - 平凡无限循环行为良好定义

### 前向进度保证对比

| 类型 | 独立进度保证 | 受其他线程影响 | 典型场景 |
|-----|-------------|---------------|---------|
| 并发前向进度 | 是 | 否 | 主线程、独立线程 |
| 并行前向进度 | 执行步骤后 | 否 | 线程池 |
| 弱并行前向进度 | 否 | 需委托 | 协作式任务 |

### 相关概念

- **std::thread** - 线程创建与管理
- **std::atomic** - 原子操作
- **std::mutex** - 互斥量同步
- **std::memory_order** - 内存顺序
- **std::async** - 异步任务
- **std::jthread** - 自动 join 线程（C++20）

### 学习建议

1. 理解数据竞争的本质是并发访问共享可变状态而不加同步
2. 优先使用高层同步原语（互斥量、条件变量），必要时才使用低层原子操作
3. 注意 `std::vector<bool>` 的特殊性，避免在多线程环境中使用
4. 了解不同前向进度保证的含义，正确选择并发策略
5. 平凡无限循环的使用场景有限，通常应该使用条件变量或其他同步机制来控制线程生命周期