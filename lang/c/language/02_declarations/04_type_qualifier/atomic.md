# Atomic Types - 原子类型

## 1. 概述

`_Atomic` 是 C11 标准引入的关键字，用于声明原子类型（atomic types）。原子类型是 C 语言中唯一可以避免数据竞争（data races）的对象类型，允许多个线程安全地并发修改或读取同一对象。

原子类型定义在 `<stdatomic.h>` 头文件中，该头文件还提供了从 `atomic_bool` 到 `atomic_uintmax_t` 等便利类型别名，简化内置类型和库类型的使用。

原子类型的核心理念是通过原子操作保证对共享变量的访问不会产生数据竞争，这是多线程编程的基础构建块。

## 2. 来源与演变

### 首次引入

`_Atomic` 关键字和原子类型支持首次在 **C11** 标准（ISO/IEC 9899:2011）中引入，作为 C 语言并发支持的重要组成部分。

### 历史背景

在 C11 之前，C 语言的多线程编程面临以下挑战：

1. **缺乏标准化的原子操作**：开发者需要依赖编译器扩展或平台特定的内联汇编
2. **数据竞争问题**：多线程访问共享变量时，普通类型无法保证操作的原子性
3. **内存可见性问题**：不同线程可能看到不同的变量值

C11 引入原子类型解决了这些问题，提供了：
- 标准化的原子操作接口
- 定义明确的内存序（memory ordering）语义
- 跨平台的并发编程支持

### C11 标准

- 引入 `_Atomic` 关键字和 `<stdatomic.h>` 头文件
- 定义了原子类型说明符（type specifier）和类型限定符（type qualifier）两种用法
- 提供了完整的原子操作库函数

### C17 标准（ISO/IEC 9899:2018）

- 保留了 C11 的原子类型支持，无重大变更
- 标准文档位置：6.7.2.4 Atomic type specifiers

### C23 标准（ISO/IEC 9899:2024）

- 继续支持原子类型
- 建议确保 C 的 `_Atomic(T)` 与 C++ 的 `std::atomic<T>` 表示形式兼容

### 兼容性检查

如果编译器定义了宏常量 `__STDC_NO_ATOMICS__`，则表示该编译器不提供 `_Atomic` 关键字支持。

## 3. 语法与参数

### 基本语法

`_Atomic` 关键字有两种使用形式：

| 形式 | 语法 | 说明 |
|------|------|------|
| 类型说明符 | `_Atomic ( type-name )` | 创建新的原子类型 |
| 类型限定符 | `_Atomic type-name` | 创建 type-name 的原子版本 |

**形式 1**：作为类型说明符使用
```c
_Atomic(int) x;        // x 是原子 int 类型
_Atomic(const int)* p; // p 是指向原子 const int 的指针
```

**形式 2**：作为类型限定符使用
```c
_Atomic int x;         // x 是原子 int 类型
_Atomic const int* p;  // p 是指向原子 const int 的指针
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `type-name` | 任何非数组、非函数的类型。对于形式 (1)，type-name 也不能是原子类型或 cvr 限定类型 |

### 类型限定符混合使用

`_Atomic` 作为类型限定符时，可以与 `const`、`volatile`、`restrict` 混合使用。但需要注意的是，原子版本的类型可能具有不同的大小、对齐方式和对象表示。

```c
_Atomic const int* p1;  // p1 是指向原子 const int 的指针
const atomic_int* p2;   // 等价写法（使用类型别名）
const _Atomic(int)* p3; // 等价写法（使用类型说明符形式）
```

### 便利类型别名

`<stdatomic.h>` 头文件定义了以下便利类型别名：

| 类型别名 | 对应类型 |
|---------|---------|
| `atomic_bool` | `_Atomic _Bool` |
| `atomic_char` | `_Atomic char` |
| `atomic_schar` | `_Atomic signed char` |
| `atomic_uchar` | `_Atomic unsigned char` |
| `atomic_short` | `_Atomic short` |
| `atomic_ushort` | `_Atomic unsigned short` |
| `atomic_int` | `_Atomic int` |
| `atomic_uint` | `_Atomic unsigned int` |
| `atomic_long` | `_Atomic long` |
| `atomic_ulong` | `_Atomic unsigned long` |
| `atomic_llong` | `_Atomic long long` |
| `atomic_ullong` | `_Atomic unsigned long long` |
| `atomic_intptr_t` | `_Atomic intptr_t` |
| `atomic_uintptr_t` | `_Atomic uintptr_t` |
| `atomic_size_t` | `_Atomic size_t` |
| `atomic_intmax_t` | `_Atomic intmax_t` |
| `atomic_uintmax_t` | `_Atomic uintmax_t` |

### 关键字

`_Atomic` 是 C11 标准的关键字。

## 4. 底层原理

### 修改顺序（Modification Order）

每个原子对象都有其关联的**修改顺序**（modification order），这是一个对该对象所有修改的全序（total order）。

关键特性：
- 如果从某个线程的角度看，原子对象 `M` 的修改 `A` happens-before 同一对象的修改 `B`，则在 `M` 的修改顺序中，`A` 出现在 `B` 之前
- 不同线程可能以不同顺序观察不同原子对象的修改
- 不存在单一的全局总序

### 四种一致性保证

原子操作保证以下四种一致性（coherence）：

| 一致性类型 | 保证内容 |
|-----------|---------|
| **写-写一致性** (write-write coherence) | 如果修改原子对象 `M` 的操作 `A` happens-before 修改 `M` 的操作 `B`，则在 `M` 的修改顺序中 `A` 出现在 `B` 之前 |
| **读-读一致性** (read-read coherence) | 如果原子对象 `M` 的值计算 `A` happens-before `M` 的值计算 `B`，且 `A` 从 `M` 上的副作用 `X` 获取值，则 `B` 计算的值要么是 `X` 存储的值，要么是修改顺序中 `X` 之后的副作用 `Y` 存储的值 |
| **读-写一致性** (read-write coherence) | 如果原子对象 `M` 的值计算 `A` happens-before 对 `M` 的操作 `B`，则 `A` 从修改顺序中 `B` 之前的副作用获取值 |
| **写-读一致性** (write-read coherence) | 如果原子对象 `M` 上的副作用 `X` happens-before 值计算 `B`，则 `B` 从 `X` 或修改顺序中 `X` 之后的副作用获取值 |

### 内存序语义

某些原子操作也是同步操作，可能具有额外的语义：

| 内存序 | 说明 |
|-------|------|
| `memory_order_relaxed` | 仅保证原子性，无同步语义 |
| `memory_order_acquire` | 获取语义，阻止后续读写重排到此操作之前 |
| `memory_order_release` | 释放语义，阻止之前读写重排到此操作之后 |
| `memory_order_acq_rel` | 同时具有获取和释放语义 |
| `memory_order_seq_cst` | 顺序一致性，默认语义 |

### 内置操作的原子性

内置的递增（`++`）和递减（`--`）运算符以及复合赋值运算符是**读-修改-写**（read-modify-write）原子操作，具有完全的顺序一致性（等价于 `memory_order_seq_cst`）。

### 左值特性

原子属性仅对左值表达式有意义。左值到右值的转换（模拟从原子位置读取到 CPU 寄存器）会剥离原子属性和其他限定符。

## 5. 使用场景

### 适合使用原子类型的场景

| 场景 | 说明 |
|------|------|
| 多线程共享计数器 | 如引用计数、事件计数器 |
| 线程间状态标志 | 如停止标志、就绪标志 |
| 无锁数据结构 | 如无锁队列、无锁栈的基础组件 |
| 简单的线程同步 | 替代重量级锁机制 |

### 不适合使用原子类型的场景

| 场景 | 推荐替代 | 原因 |
|------|---------|------|
| 复杂数据结构的并发访问 | 互斥锁（mutex） | 需要保护多个变量的原子性 |
| 需要事务性操作 | 事务内存或锁 | 原子操作无法组合 |
| 大对象的并发修改 | 锁 + 普通类型 | 原子操作可能效率低下 |

### 注意事项

1. **访问原子结构体/联合体成员是未定义行为**
   ```c
   _Atomic struct { int x; int y; } s;
   // s.x = 10;  // 未定义行为！
   ```

2. **`sig_atomic_t` 不提供线程同步**：它只保证信号处理程序中的原子性，不提供线程间同步或内存排序

3. **`volatile` 不提供原子性**：`volatile` 类型不提供线程间同步、内存排序或原子性保证

4. **与 C++ 的兼容性**：实现应确保 C 的 `_Atomic(T)` 与 C++ 的 `std::atomic<T>` 具有相同的表示形式

### 与 volatile 的区别

| 特性 | `_Atomic` | `volatile` |
|-----|-----------|------------|
| 原子性 | 提供 | 不提供 |
| 内存序 | 提供 | 不提供 |
| 线程同步 | 提供 | 不提供 |
| 防止优化 | 是 | 仅防止缓存优化 |
| 适用场景 | 多线程 | 硬件寄存器、信号处理 |

## 6. 代码示例

### 基础用法：原子计数器

```c
#include <stdatomic.h>
#include <stdio.h>
#include <threads.h>

atomic_int acnt;  // 原子计数器
int cnt;          // 非原子计数器（用于对比）

int f(void* thr_data)
{
    for (int n = 0; n < 1000; ++n) {
        ++cnt;    // 非原子操作，可能产生数据竞争
        ++acnt;   // 原子操作，保证正确性
    }
    return 0;
}

int main(void)
{
    thrd_t thr[10];
    for (int n = 0; n < 10; ++n)
        thrd_create(&thr[n], f, NULL);
    for (int n = 0; n < 10; ++n)
        thrd_join(thr[n], NULL);

    printf("The atomic counter is %u\n", acnt);     // 输出: 10000
    printf("The non-atomic counter is %u\n", cnt);  // 输出: < 10000（不确定值）
    return 0;
}
```

**可能的输出**：
```
The atomic counter is 10000
The non-atomic counter is 8644
```

### 高级用法：使用显式内存序

```c
#include <stdatomic.h>
#include <stdio.h>

// 使用 relaxed 内存序进行简单的计数
// 适用于不需要同步语义的场景
void relaxed_counter_example(void) {
    atomic_int counter = ATOMIC_VAR_INIT(0);

    // 使用 relaxed 内存序，仅保证原子性
    atomic_fetch_add_explicit(&counter, 1, memory_order_relaxed);

    printf("Counter: %d\n", atomic_load(&counter));
}

// 使用 acquire-release 进行线程同步
atomic_int flag = ATOMIC_VAR_INIT(0);
int data = 0;

void producer(void) {
    data = 42;
    // release 语义：确保 data 的写入在 flag 设置之前完成
    atomic_store_explicit(&flag, 1, memory_order_release);
}

void consumer(void) {
    // acquire 语义：确保读取 flag 后才能访问 data
    while (atomic_load_explicit(&flag, memory_order_acquire) == 0) {
        // 等待 flag 被设置
    }
    printf("Data: %d\n", data);  // 保证看到 data = 42
}
```

### 常见错误及修正

#### 错误 1：访问原子结构体成员

```c
// 错误：访问原子结构体成员是未定义行为
_Atomic struct Point { int x; int y; } point;
point.x = 10;  // 未定义行为！

// 修正：使用整体操作
struct Point temp = {10, 20};
atomic_store(&point, temp);
```

#### 错误 2：误用 volatile 实现线程同步

```c
// 错误：volatile 不提供线程同步
volatile int ready = 0;
int data = 0;

void thread1(void) {
    data = 42;
    ready = 1;  // 可能被重排序！
}

void thread2(void) {
    while (!ready);  // 可能永远看不到 ready 的变化
    printf("%d\n", data);  // 可能看到错误的 data 值
}

// 修正：使用原子类型
atomic_int ready = ATOMIC_VAR_INIT(0);
int data = 0;

void thread1_fixed(void) {
    data = 42;
    atomic_store_explicit(&ready, 1, memory_order_release);
}

void thread2_fixed(void) {
    while (atomic_load_explicit(&ready, memory_order_acquire) == 0);
    printf("%d\n", data);  // 保证看到 data = 42
}
```

#### 错误 3：对非原子变量使用原子操作

```c
// 错误：对非原子变量使用原子操作是未定义行为
int x = 0;
atomic_fetch_add(&x, 1);  // 未定义行为！

// 修正：声明为原子类型
atomic_int x = ATOMIC_VAR_INIT(0);
atomic_fetch_add(&x, 1);  // 正确
```

## 7. 总结

### 核心要点

1. **原子类型是 C 语言多线程编程的基础**：原子类型是唯一可以避免数据竞争的对象类型
2. **两种语法形式**：类型说明符 `_Atomic(T)` 和类型限定符 `_Atomic T`
3. **修改顺序保证**：每个原子对象都有明确定义的修改顺序
4. **四种一致性保证**：写-写、读-读、读-写、写-读一致性
5. **内存序控制**：可以通过显式内存序函数精细控制同步语义

### 技术对比

| 特性 | `_Atomic` | `volatile` | 普通类型 + 锁 |
|-----|-----------|------------|--------------|
| 原子性 | 自动保证 | 不保证 | 锁保护时保证 |
| 内存序 | 可控制 | 不保证 | 隐式保证 |
| 性能开销 | 低 | 最低 | 较高（锁竞争） |
| 适用复杂度 | 简单变量 | 硬件访问 | 复杂数据结构 |

### 学习建议

1. **理解内存模型**：深入理解 C11 内存模型是正确使用原子类型的前提
2. **优先使用默认内存序**：除非有明确的性能需求，否则使用默认的顺序一致性语义
3. **使用类型别名**：使用 `atomic_int` 等类型别名提高代码可读性
4. **避免原子结构体成员访问**：原子结构体/联合体的成员访问是未定义行为
5. **与 C++ 兼容**：设计跨语言接口时注意 C 和 C++ 原子类型的兼容性

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.7.2.4 Atomic type specifiers, 7.17 Atomics
- C17 标准 (ISO/IEC 9899:2018): 6.7.2.4 Atomic type specifiers, 7.17 Atomics
- C11 标准 (ISO/IEC 9899:2011): 6.7.2.4 Atomic type specifiers, 7.17 Atomics
- cppreference: https://en.cppreference.com/w/c/language/atomic