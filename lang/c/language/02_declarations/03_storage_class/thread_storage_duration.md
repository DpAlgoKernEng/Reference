# 线程存储期 (Thread Storage Duration)

## 1. 概述 (Overview)

**线程存储期 (Thread Storage Duration)** 是 C 语言中四种存储期之一（另外三种为自动存储期、静态存储期和动态分配存储期）。使用 `_Thread_local` 存储类说明符（storage-class specifier）声明的对象具有线程存储期。

### 核心特性

- **独立实例**：每个线程拥有该对象的独立实例
- **生命周期**：对象的生存期（lifetime）贯穿创建它的线程的整个执行过程
- **初始化时机**：在线程启动时初始化其存储值
- **访问规则**：在表达式中使用声明的名称时，引用的是与当前执行线程相关联的对象实例

### 技术定位

线程存储期主要用于多线程编程场景，提供线程安全的全局或静态变量访问方式，避免线程间的数据竞争（data race）。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C11 标准之前，C 语言对多线程的支持非常有限。程序员通常依赖以下方式实现线程特定数据：

- **POSIX 线程特定数据**：使用 `pthread_key_create`、`pthread_setspecific`、`pthread_getspecific` 等函数
- **Windows TLS**：使用 `TlsAlloc`、`TlsSetValue`、`TlsGetValue` 等 API
- **编译器扩展**：如 GCC 的 `__thread` 关键字

### C11 标准化

**C11 标准**（ISO/IEC 9899:2011）正式引入了 `_Thread_local` 关键字，作为标准化的线程局部存储解决方案：

- **设计动机**：提供可移植的、语言级别的线程局部存储机制
- **解决的问题**：统一不同平台上的线程特定数据实现方式
- **标准化时间**：2011 年 12 月正式发布

### C23 更新

C23 标准中 `_Thread_local` 保持不变，同时可以通过 `<threads.h>` 头文件使用 `thread_local` 便利宏（convenience macro）。

---

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```c
_Thread_local type identifier;
_Thread_local type identifier = initializer;
```

### 存储类说明符组合规则

`_Thread_local` 可以与 `static` 或 `extern` 组合使用：

| 组合方式 | 语义 | 链接属性 |
|---------|------|---------|
| `_Thread_local` | 线程局部存储，外部链接（默认） | 外部链接 |
| `static _Thread_local` | 线程局部存储，内部链接 | 内部链接 |
| `extern _Thread_local` | 线程局部存储，声明引用 | 外部链接 |

### 参数说明

- **type**：任意完整对象类型（complete object type）
- **identifier**：对象标识符
- **initializer**：可选的初始化表达式

### 使用限制

- `_Thread_local` 不能用于函数参数
- `_Thread_local` 不能用于块作用域的非静态变量
- 具有线程存储期的对象不能在表达式中用作非常量表达式

---

## 4. 底层原理 (Underlying Principles)

### 实现机制

编译器和操作系统协作实现线程局部存储：

**编译器层面**
- 为每个线程局部变量生成线程局部存储（TLS）索引
- 将变量访问转换为 TLS 查找操作
- 生成的代码通过线程特定数据段访问变量

**操作系统层面**
- 线程创建时分配 TLS 存储空间
- 每个线程维护独立的 TLS 表
- 提供快速的线程特定数据访问机制

### 内存布局

```
+------------------+
|    主线程 TLS    |
+------------------+
| 线程局部变量 A   | <--- 主线程访问 A
| 线程局部变量 B   |
+------------------+

+------------------+
|   线程1的 TLS    |
+------------------+
| 线程局部变量 A   | <--- 线程1访问 A
| 线程局部变量 B   |
+------------------+

+------------------+
|   线程2的 TLS    |
+------------------+
| 线程局部变量 A   | <--- 线程2访问 A
| 线程局部变量 B   |
+------------------+
```

### 性能特征

| 操作 | 时间复杂度 | 说明 |
|------|-----------|------|
| 访问线程局部变量 | O(1) | 通常只需一次 TLS 索引查找 |
| 线程创建 | O(n) | n 为线程局部变量数量 |
| 内存开销 | 每线程每变量一份 | 线程数 × 变量大小的内存占用 |

### 跨线程访问行为

从与对象不关联的线程间接访问具有线程存储期的对象，其结果是**实现定义的（implementation-defined）**。这通常发生在：

- 将线程局部变量的指针传递给其他线程
- 通过全局指针访问其他线程的局部存储

---

## 5. 使用场景 (Use Cases)

### 适用场景

**1. 线程安全的随机数生成器**

```c
_Thread_local unsigned int seed;  // 每个线程独立种子

int thread_safe_rand(void) {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}
```

**2. 线程特定的错误状态**

```c
_Thread_local int thread_errno;  // 类似 errno 的线程安全版本
```

**3. 线程本地日志缓冲区**

```c
_Thread_local char log_buffer[1024];  // 避免锁竞争
```

**4. 线程私有的状态机**

```c
_Thread_local ParserState parser_state;  // 每个线程独立解析状态
```

### 最佳实践

- **优先使用线程局部存储**：替代全局变量 + 互斥锁的方案
- **初始化简单**：利用线程启动时的自动初始化
- **避免指针传递**：不要将线程局部变量的地址传递给其他线程

### 注意事项

**生命周期管理**
- 线程退出时，线程局部变量自动销毁
- 主线程的线程局部变量在程序结束时销毁

**初始化顺序**
- 线程局部变量在控制流首次经过声明点时初始化
- 动态初始化可能发生在线程启动后的任何时刻

### 常见陷阱

**陷阱 1：误认为所有线程共享同一实例**

```c
// 错误理解：期望所有线程共享 counter
_Thread_local int counter = 0;

void* thread_func(void* arg) {
    counter++;  // 每个线程有独立的 counter
    printf("Counter: %d\n", counter);  // 总是输出 1
    return NULL;
}
```

**陷阱 2：跨线程传递指针**

```c
_Thread_local int* ptr;

void* thread_func(void* arg) {
    int value = 42;
    ptr = &value;  // 危险：指向线程局部栈
    // 线程结束后 ptr 变为悬垂指针
    return NULL;
}
```

---

## 6. 代码示例 (Examples)

### 示例 1：基础用法

```c
#include <stdio.h>
#include <threads.h>

const double PI = 3.14159;         /* const 变量对所有线程全局共享 */
_Thread_local unsigned int seed;    /* seed 是线程特定变量 */

int thread_func(void* arg) {
    int id = *(int*)arg;
    seed = id * 1000;  /* 每个线程初始化自己的 seed */

    for (int i = 0; i < 3; i++) {
        seed = seed * 1103515245 + 12345;
        printf("Thread %d, seed = %u\n", id, seed);
    }

    return 0;
}

int main(void) {
    thrd_t t1, t2;
    int id1 = 1, id2 = 2;

    thrd_create(&t1, thread_func, &id1);
    thrd_create(&t2, thread_func, &id2);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    printf("PI = %f (shared by all threads)\n", PI);

    return 0;
}
```

**可能的输出：**

```
Thread 1, seed = 1103527545
Thread 1, seed = 377401575
Thread 1, seed = 1610794849
Thread 2, seed = 2207055090
Thread 2, seed = 754803150
Thread 2, seed = 3221589698
PI = 3.141590 (shared by all threads)
```

### 示例 2：与 static 组合使用

```c
#include <stdio.h>

// 内部链接的线程局部变量
static _Thread_local int call_count = 0;

void increment_count(void) {
    call_count++;
    printf("Call count in this thread: %d\n", call_count);
}
```

### 示例 3：常见错误 - 忘记线程独立性

```c
#include <stdio.h>
#include <threads.h>

_Thread_local int shared_counter = 0;  // 错误命名：实际上不共享

void* bad_thread_func(void* arg) {
    for (int i = 0; i < 1000; i++) {
        shared_counter++;  // 每个线程独立计数，不会累加
    }
    return NULL;
}

int main(void) {
    thrd_t threads[4];

    // 创建 4 个线程
    for (int i = 0; i < 4; i++) {
        thrd_create(&threads[i], bad_thread_func, NULL);
    }

    for (int i = 0; i < 4; i++) {
        thrd_join(threads[i], NULL);
    }

    // 输出 0，因为 main 线程的 shared_counter 未被修改
    printf("Final counter: %d\n", shared_counter);

    return 0;
}
```

**正确做法（如果需要跨线程共享）：**

```c
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int shared_counter = 0;  // 使用原子变量实现真正的共享

int good_thread_func(void* arg) {
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&shared_counter, 1);
    }
    return 0;
}

int main(void) {
    thrd_t threads[4];

    for (int i = 0; i < 4; i++) {
        thrd_create(&threads[i], good_thread_func, NULL);
    }

    for (int i = 0; i < 4; i++) {
        thrd_join(threads[i], NULL);
    }

    printf("Final counter: %d\n", atomic_load(&shared_counter));  // 输出 4000

    return 0;
}
```

### 示例 4：线程安全的单例模式

```c
#include <stdio.h>
#include <threads.h>

// 每个线程拥有独立的资源实例
_Thread_local struct {
    int initialized;
    char buffer[256];
    int ref_count;
} thread_resource;

void init_resource(void) {
    if (!thread_resource.initialized) {
        thread_resource.initialized = 1;
        thread_resource.ref_count = 0;
        // 初始化其他字段
    }
}

void use_resource(void) {
    init_resource();
    thread_resource.ref_count++;
    printf("Thread resource used %d times\n", thread_resource.ref_count);
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| **引入版本** | C11 标准 |
| **关键字** | `_Thread_local` |
| **存储期类型** | 线程存储期 |
| **生命周期** | 线程创建到线程结束 |
| **实例数量** | 每线程一个独立实例 |
| **初始化时机** | 线程启动时 |

### 与其他存储期对比

| 存储期 | 关键字 | 生命周期 | 可见性 |
|--------|--------|----------|--------|
| 自动存储期 | `auto`（默认） | 块作用域 | 块内 |
| 静态存储期 | `static` | 程序全程 | 全局/文件作用域 |
| 线程存储期 | `_Thread_local` | 线程全程 | 线程内 |
| 动态存储期 | `malloc`/`free` | 手动管理 | 自定义 |

### 技术对比

| 方案 | 优点 | 缺点 |
|------|------|------|
| `_Thread_local` | 标准化、无锁、高效 | 仅 C11 及以上 |
| `pthread_key_*` | 广泛支持、POSIX 标准 | API 复杂、性能较低 |
| 编译器扩展 (`__thread`) | 高效、历史悠久 | 不可移植 |
| 全局变量 + 互斥锁 | 简单直观 | 性能开销、锁竞争 |

### 学习建议

1. **理解存储期概念**：掌握 C 语言的四种存储期及其区别
2. **掌握多线程编程基础**：了解线程创建、同步、通信机制
3. **实践代码示例**：编写简单的多线程程序验证线程局部存储的行为
4. **阅读标准文档**：参考 ISO/IEC 9899:2011 第 6.2.4 节
5. **平台差异**：了解不同操作系统对 TLS 的实现差异

### 相关概念

- **存储类说明符 (Storage Class Specifier)**：`auto`、`register`、`static`、`extern`、`_Thread_local`
- **链接属性 (Linkage)**：外部链接、内部链接、无链接
- **线程安全 (Thread Safety)**：多线程环境下的数据访问安全性
- **TLS (Thread Local Storage)**：线程局部存储的实现机制

---

**参考资料**
- ISO/IEC 9899:2011 第 6.2.4 节 "Storage durations of objects"
- cppreference: [Thread storage duration](https://en.cppreference.com/w/c/language/storage_duration)