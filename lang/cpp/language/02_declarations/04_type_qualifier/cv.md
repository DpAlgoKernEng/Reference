# cv 类型限定符（const 与 volatile）

## 1. 概述

**cv 限定符**（cv-qualifiers）是 C++ 中用于修饰类型的说明符，包括 `const` 和 `volatile` 两个关键字。它们可以出现在任何类型说明符中，包括声明的 decl-specifier-seq 语法中，用于指定所声明对象的常量性（constness）或易变性（volatility）。

- **const** - 定义类型为**常量**，表示对象的值不可修改
- **volatile** - 定义类型为**易变**，表示对象的值可能被程序外部因素改变

任何类型（除函数类型和引用类型外，可能是不完整类型）都属于以下四种相互关联但不同的类型组之一：

| 类型版本 | 说明 |
|---------|------|
| cv-unqualified | 无 cv 限定的版本 |
| const-qualified | const 限定的版本 |
| volatile-qualified | volatile 限定的版本 |
| const-volatile-qualified | 同时具有 const 和 volatile 限定的版本 |

同一组中的四种类型具有相同的内存表示和对齐要求。数组类型的 cv 限定被认为与其元素类型的 cv 限定相同。

## 2. 来源与演变

### 历史背景

cv 限定符的概念继承自 C 语言：

- **const** 关键字最初在 C 语言中引入，用于定义常量，避免使用预处理宏 `#define`
- **volatile** 关键字用于处理硬件寄存器、信号处理和多线程共享内存等场景

### C 与 C++ 的差异

| 特性 | C 语言 | C++ |
|------|--------|-----|
| const 文件作用域变量 | 外部链接 | 内部链接（C++ 特有） |
| const 表达式 | 不完全支持 | 完全支持，可用于编译期常量 |

### C++ 版本变化

**C++14**：
- const 限定符用于非模板变量时，若非 volatile、非 inline、非 extern，则具有内部链接

**C++17**：
- 明确了 inline 变量与 const 的交互规则

**C++20**：
- 以下 volatile 使用方式被**弃用**：
  - volatile 类型作为内置自增/自减操作符的操作数
  - volatile 类型作为内置直接赋值的左操作数（除非在未求值上下文或弃值表达式中）
  - volatile 对象类型作为函数参数或返回类型
  - 结构化绑定声明中的 volatile 限定符

### 缺陷报告

| DR 编号 | 应用版本 | 原始行为 | 修正行为 |
|---------|---------|----------|----------|
| CWG 1428 | C++98 | const 对象的定义基于声明 | 基于对象类型 |
| CWG 1528 | C++98 | 无 cv 限定符出现次数要求 | 每个 cv 限定符最多出现一次 |
| CWG 1799 | C++98 | mutable 可应用于非 const 声明的数据成员，但类型可能是 const 限定 | 不能应用于 const 限定类型的数据成员 |

## 3. 语法与声明

### cv 限定符语法

```cpp
const T          // const 限定的类型 T
volatile T       // volatile 限定的类型 T
const volatile T // 同时具有两种限定的类型 T
```

**重要规则**：每个 cv 限定符在同一个 cv 限定符序列中最多出现一次。例如：
- `const const int` - 非法（重复）
- `volatile const volatile` - 非法（重复）

### const 对象定义

**const 对象**是指：
- 类型为 const 限定的对象，或
- const 对象的非 mutable 子对象

```cpp
const int n = 10;        // const 对象
int const m = 20;        // 等价写法
const int* p = &n;       // 指向 const 对象的指针
int* const q = nullptr; // const 指针（指向非 const 对象）
```

### volatile 对象定义

**volatile 对象**是指：
- 类型为 volatile 限定的对象
- volatile 对象的子对象
- const-volatile 对象的 mutable 子对象

```cpp
volatile int v1;              // volatile 对象
int volatile v2;              // 等价写法
volatile int* p;              // 指向 volatile 对象的指针
```

### mutable 说明符

**mutable** 说明符允许修改类的 mutable 成员，即使包含该成员的对象被声明为 const。

```cpp
class X {
    mutable const int* p;    // 合法
    mutable int* const q;    // 非法：const 指针不能是 mutable
    mutable int& r;          // 非法：引用不能是 mutable
};
```

**mutable 只能用于**：
- 非静态类成员
- 非引用类型
- 非 const 限定类型

### cv 限定符的偏序关系

cv 限定符存在偏序关系，按限制程度递增排列：

```
unqualified < const
unqualified < volatile
unqualified < const volatile
const < const volatile
volatile < const volatile
```

指向 cv 限定类型的引用和指针可以**隐式转换**为指向更严格 cv 限定类型的引用和指针。要转换为更宽松的 cv 限定类型，必须使用 `const_cast`。

## 4. 底层原理

### const 的实现机制

编译器对 const 对象的处理：

1. **编译期检查**：直接修改 const 对象会产生编译错误
2. **运行期行为**：通过指针或引用间接修改 const 对象会导致**未定义行为**

```cpp
const int x = 10;
// x = 20;              // 编译错误
const_cast<int&>(x) = 20; // 未定义行为（若 x 是真正的 const 对象）
```

### volatile 的实现机制

volatile 关键字影响编译器的**优化行为**：

1. **可见性保证**：每次通过 volatile 限定类型的泛左值（glvalue）进行的访问（读/写操作、成员函数调用等）都被视为可见的副作用
2. **禁止优化**：
   - volatile 访问不能被优化掉
   - volatile 访问不能与其他可见副作用重排序
3. **内存语义**：volatile 不提供线程同步保证，不适合用于多线程通信

```cpp
volatile int flag = 0;

// 编译器必须生成实际的内存访问指令
// 不能将其优化到寄存器中
while (flag == 0) {
    // 等待外部修改 flag
}
```

### 内存表示

cv 限定符**不影响**对象的内存表示：

```cpp
static_assert(sizeof(int) == sizeof(const int));
static_assert(sizeof(int) == sizeof(volatile int));
static_assert(sizeof(int) == sizeof(const volatile int));
static_assert(alignof(int) == alignof(const volatile int));
```

### volatile 与信号处理

volatile 对象适合用于与信号处理程序（signal handler）通信，因为：
- 信号处理程序可能在程序主执行流之外修改变量
- volatile 防止编译器缓存变量值

### volatile 与多线程

**重要提示**：volatile **不适用于**多线程同步：
- 不保证原子性
- 不提供内存屏障
- 不保证可见性顺序

正确的多线程同步应使用：
- `std::atomic<T>` - 原子操作
- `std::memory_order` - 内存顺序
- 互斥锁（`std::mutex`）

## 5. 使用场景

### const 的使用场景

| 场景 | 说明 |
|------|------|
| 编译期常量 | 定义编译期可确定的常量值 |
| 函数参数保护 | 防止函数内部修改参数指向的数据 |
| 成员函数限定 | const 成员函数承诺不修改对象状态 |
| 接口契约 | 向调用者承诺不修改数据 |
| 内部链接 | 文件作用域的 const 变量默认具有内部链接 |

### volatile 的使用场景

| 场景 | 说明 |
|------|------|
| 硬件寄存器映射 | 访问内存映射的 I/O 寄存器 |
| 信号处理 | 与信号处理程序共享的变量 |
| setjmp/longjmp | 用于 setjmp/longjmp 跳转后保持值的局部变量 |
| 外部修改 | 可能被外部代理（如调试器）修改的变量 |

### mutable 的使用场景

mutable 用于表示不影响类外部可见状态的成员：

| 场景 | 示例 |
|------|------|
| 互斥锁 | `mutable std::mutex` - 保护内部状态的锁 |
| 缓存 | `mutable Cache cache` - 惰性计算的缓存 |
| 计数器 | `mutable size_t access_count` - 访问计数 |
| 惰性求值 | `mutable std::optional<Result>` - 延迟计算的结果 |

### 最佳实践

1. **const 正确性**：尽可能使用 const，形成完整的 const 接口
2. **M&M 规则**：mutable 与 mutex 总是成对出现
3. **避免 const_cast**：修改 const 对象是未定义行为
4. **谨慎使用 volatile**：现代 C++ 中，volatile 主要用于硬件交互

### 常见陷阱

1. **const 指针 vs 指向 const 的指针**
```cpp
const int* p;      // 指向 const int 的指针（可改指针，不可改值）
int* const p;      // const 指针，指向 int（不可改指针，可改值）
const int* const p; // const 指针，指向 const int
```

2. **volatile 不保证线程安全**
```cpp
volatile int counter = 0;  // 错误：不保证原子性
std::atomic<int> counter{0};  // 正确：原子操作
```

3. **const 成员函数中的 mutable 成员**
```cpp
class Example {
    mutable int cache;
public:
    int get() const {
        cache = compute();  // 合法：mutable 成员可以在 const 函数中修改
        return cache;
    }
};
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

int main() {
    // const 对象
    int n1 = 0;              // 非 const 对象
    const int n2 = 0;        // const 对象
    int const n3 = 0;        // const 对象（等价写法）
    volatile int n4 = 0;     // volatile 对象

    // const 对象与 mutable 成员
    const struct {
        int n1;
        mutable int n2;
    } x = {0, 0};

    n1 = 1;           // OK：可修改对象
    // n2 = 2;        // 错误：const 对象不可修改
    n4 = 3;           // OK：volatile 写入是可见的副作用
    // x.n1 = 4;      // 错误：const 对象的成员是 const
    x.n2 = 4;         // OK：mutable 成员可以修改

    // const 引用
    const int& r1 = n1;   // const 引用绑定到非 const 对象
    // r1 = 2;            // 错误：不能通过 const 引用修改
    const_cast<int&>(r1) = 2;  // OK：修改非 const 对象 n1

    const int& r2 = n2;   // const 引用绑定到 const 对象
    // const_cast<int&>(r2) = 2;  // 未定义行为：修改 const 对象 n2

    std::cout << "n1 = " << n1 << std::endl;  // 输出: n1 = 2
    std::cout << "x.n2 = " << x.n2 << std::endl;  // 输出: x.n2 = 4

    return 0;
}
```

### mutable 与互斥锁（M&M 规则）

```cpp
#include <mutex>

class ThreadsafeCounter {
    mutable std::mutex m;  // M&M 规则：mutable 与 mutex 一起使用
    int data = 0;

public:
    int get() const {
        std::lock_guard<std::mutex> lk(m);
        return data;
    }

    void inc() {
        std::lock_guard<std::mutex> lk(m);
        ++data;
    }

    void dec() {
        std::lock_guard<std::mutex> lk(m);
        --data;
    }
};

// 即使是 const 对象也可以安全地调用 get()
void printCounter(const ThreadsafeCounter& counter) {
    std::cout << counter.get() << std::endl;  // 合法
}
```

### volatile 与硬件寄存器

```cpp
// 内存映射 I/O 寄存器
volatile uint32_t* const UART_DR = reinterpret_cast<volatile uint32_t*>(0x10000000);
volatile uint32_t* const UART_SR = reinterpret_cast<volatile uint32_t*>(0x10000004);

void sendChar(char c) {
    // 等待发送缓冲区为空
    while ((*UART_SR & 0x20) == 0) {
        // volatile 确保每次都读取实际寄存器值
    }
    *UART_DR = c;  // 写入数据寄存器
}

char receiveChar() {
    // 等待接收缓冲区有数据
    while ((*UART_SR & 0x01) == 0) {
        // 自旋等待
    }
    return static_cast<char>(*UART_DR);
}
```

### cv 限定符转换

```cpp
#include <iostream>

void demonstrateCvConversions() {
    int n = 10;

    // 增加限定：隐式转换
    int* p1 = &n;
    const int* p2 = p1;        // OK：int* -> const int*
    volatile int* p3 = p1;     // OK：int* -> volatile int*
    const volatile int* p4 = p1; // OK：int* -> const volatile int*

    // 减少限定：需要 const_cast
    const int* cp = &n;
    // int* p = cp;            // 错误：不能隐式去除 const
    int* p = const_cast<int*>(cp);  // OK：显式转换

    std::cout << *p << std::endl;
}
```

### 常见错误及修正

#### 错误 1：修改真正的 const 对象

```cpp
// 错误：通过 const_cast 修改 const 对象
const int global = 10;

void badCode() {
    const_cast<int&>(global) = 20;  // 未定义行为！
}

// 正确：只对非 const 对象使用 const_cast
void goodCode() {
    int value = 10;
    const int& cref = value;
    const_cast<int&>(cref) = 20;  // OK：value 是非 const 对象
}
```

#### 错误 2：volatile 用于多线程同步

```cpp
// 错误：volatile 不能保证线程安全
volatile bool ready = false;

void thread1() {
    // ... 准备数据
    ready = true;  // 不保证其他线程立即看到！
}

void thread2() {
    while (!ready) {}  // 可能无限循环
    // ... 使用数据
}

// 正确：使用 atomic
#include <atomic>
std::atomic<bool> ready{false};

void thread1_correct() {
    // ... 准备数据
    ready.store(true, std::memory_order_release);
}

void thread2_correct() {
    while (!ready.load(std::memory_order_acquire)) {}
    // ... 使用数据
}
```

#### 错误 3：const 成员函数中修改非 mutable 成员

```cpp
class Counter {
    int count = 0;
    // mutable int count = 0;  // 修正：添加 mutable

public:
    void increment() const {
        // count++;  // 错误：const 函数不能修改非 mutable 成员
    }
};

// 修正版本
class CounterFixed {
    mutable int count = 0;

public:
    void increment() const {
        count++;  // OK：mutable 成员可以在 const 函数中修改
    }
};
```

## 7. 总结

### 核心要点

| 限定符 | 核心作用 | 使用场景 |
|--------|----------|----------|
| const | 编译期禁止直接修改 | 接口契约、编译期常量、优化提示 |
| volatile | 禁止编译器优化访问 | 硬件寄存器、信号处理、外部修改 |
| mutable | 允许在 const 对象中修改 | 互斥锁、缓存、计数器 |

### 技术对比

| 特性 | const | volatile |
|------|-------|----------|
| 影响内存表示 | 否 | 否 |
| 影响对齐要求 | 否 | 否 |
| 编译期检查 | 是 | 否 |
| 运行期效果 | 无（除优化外） | 限制优化 |
| 线程安全 | 不保证 | 不保证 |

### 学习建议

1. **const**：优先使用，形成 const 正确的代码风格
2. **volatile**：谨慎使用，主要用于底层硬件交互
3. **mutable**：理解"M&M 规则"，合理用于实现细节
4. **const_cast**：避免使用，除非明确知道对象不是 const

### 注意事项

1. cv 限定符不影响内存表示和对齐
2. 间接修改 const 对象是未定义行为
3. volatile 不提供线程同步保证
4. 每个 cv 限定符最多出现一次
5. 数组的 cv 限定与元素类型相同

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/cv
- C++ Standard: [basic.type.qualifier]
- Effective C++, Scott Meyers, Item 3: Use const whenever possible