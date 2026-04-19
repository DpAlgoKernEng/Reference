# 并发支持库（Concurrency support library）

来源：[cppreference.com](https://en.cppreference.com)

---

## 简介

C 语言自 C11 标准起正式引入了**并发支持库（Concurrency Support Library）**，使得开发者能够编写跨平台的多线程程序。该库包括以下核心功能：

- **线程管理**
- **原子操作**
- **互斥量**
- **条件变量**
- **线程局部存储**

> ⚠️ **注意**：
> - 并发支持库是**可选实现**，并非所有编译器都完整支持；
> - 若编译器定义了宏 `__STDC_NO_THREADS__`，则不提供 `<threads.h>`；
> - 若编译器定义了宏 `__STDC_NO_ATOMICS__`，则不提供 `<stdatomic.h>`；
> - 有关 `_Atomic` 类型修饰符，请参考 [Atomic Types](https://zh.cppreference.com/w/c/language/atomic)。

---

## 库功能概览

| 模块 | 功能 |
|------|------|
| [线程管理](#线程-thread) | 线程创建、等待、退出等 |
| [原子操作](#原子操作-atomic-operations) | 无锁同步的底层机制 |
| [互斥量](#互斥量-mutual-exclusion) | 数据访问保护 |
| [条件变量](#条件变量-condition-variables) | 线程间通信机制 |
| [线程局部存储](#线程局部存储-thread-local-storage) | 每线程私有数据 |

---

## 线程（Thread）

### 头文件：`<threads.h>`（C11）

#### 核心类型和常量

| 类型/常量 | 说明 |
|-----------|------|
| `thrd_t` | 线程标识符类型 |
| `thrd_start_t` | 线程函数指针类型：`int (*)(void*)` |
| `thrd_success` | 操作成功 |
| `thrd_error` | 操作失败 |
| `thrd_timedout` | 超时 |
| `thrd_busy` | 忙碌 |
| `thrd_nomem` | 内存不足 |

#### 函数列表

| 函数 | 功能 |
|------|------|
| `thrd_create()` | 创建线程 |
| `thrd_equal()` | 比较两个线程标识符 |
| `thrd_current()` | 获取当前线程 ID |
| `thrd_sleep()` | 睡眠指定时间 |
| `thrd_yield()` | 礼让 CPU 控制权 |
| `thrd_exit()` | 终止当前线程 |
| `thrd_detach()` | 脱离线程（不再 joinable） |
| `thrd_join()` | 等待线程结束 |

#### 示例代码

```c
#include <stdio.h>
#include <threads.h>

int thread_func(void *arg) {
    printf("Hello from thread %d\n", *(int*)arg);
    return 0;
}

int main() {
    thrd_t t1;
    int id = 1;

    if (thrd_create(&t1, thread_func, &id) == thrd_success) {
        thrd_join(t1, NULL);
    }

    return 0;
}
```

---

## 原子操作（Atomic Operations）

### 头文件：`<stdatomic.h>`（C11）

原子操作提供无锁编程能力，保证多个线程对共享变量的操作是互斥且可见的。

#### 原子类型别名（部分）

| 类型名 | 对应类型 |
|--------|----------|
| `atomic_bool` | `_Atomic _Bool` |
| `atomic_int` | `_Atomic int` |
| `atomic_char` | `_Atomic char` |
| `atomic_ptrdiff_t` | `_Atomic ptrdiff_t` |
| ...（见完整列表） | ... |

#### 原子操作函数

| 函数 | 功能 |
|------|------|
| `atomic_init()` | 初始化原子对象 |
| `atomic_is_lock_free()` | 判断是否为无锁实现 |
| `atomic_store()` / `atomic_load()` | 原子写入/读取 |
| `atomic_exchange()` | 原子交换 |
| `atomic_compare_exchange_strong()` | 原子比较并交换 |
| `atomic_fetch_add()` / `atomic_fetch_sub()` | 原子增减 |
| `atomic_fetch_or()` / `atomic_fetch_and()` / `atomic_fetch_xor()` | 原子位操作 |

#### 原子标志（atomic_flag）

| 类型/函数 | 说明 |
|-----------|------|
| `atomic_flag` | 无锁布尔标志 |
| `ATOMIC_FLAG_INIT` | 初始化宏 |
| `atomic_flag_test_and_set()` | 设置标志并返回旧值 |
| `atomic_flag_clear()` | 清除标志 |

#### 示例代码

```c
#include <stdio.h>
#include <stdatomic.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

int main() {
    atomic_fetch_add(&counter, 1);
    printf("Counter: %d\n", atomic_load(&counter));
    return 0;
}
```

#### 内存序（Memory Ordering）

使用 `memory_order` 枚举精确控制内存一致性。

| 枚举值 | 说明 |
|--------|------|
| `memory_order_relaxed` | 仅保证原子性 |
| `memory_order_acquire` | 获取语义（后续读取不能重排到此之前） |
| `memory_order_release` | 释放语义（之前的写入不能重排到此之后） |
| `memory_order_acq_rel` | acquire + release |
| `memory_order_seq_cst` | 顺序一致（默认模型） |

#### 屏障函数

| 函数 | 作用 |
|------|------|
| `atomic_thread_fence()` | 线程间内存屏障 |
| `atomic_signal_fence()` | 信号处理函数间屏障 |

---

## 互斥量（Mutual Exclusion）

### 头文件：`<threads.h>`

互斥量防止多个线程同时访问共享资源。

#### 类型和常量

| 类型/常量 | 说明 |
|-----------|------|
| `mtx_t` | 互斥量类型 |
| `mtx_plain` | 普通互斥量 |
| `mtx_recursive` | 可递归互斥量 |
| `mtx_timed` | 支持超时的互斥量 |

#### 函数列表

| 函数 | 功能 |
|------|------|
| `mtx_init()` | 初始化互斥量 |
| `mtx_lock()` | 上锁（阻塞） |
| `mtx_trylock()` | 尝试上锁（不阻塞） |
| `mtx_timedlock()` | 带超时上锁 |
| `mtx_unlock()` | 解锁 |
| `mtx_destroy()` | 销毁互斥量 |

#### 示例代码

```c
#include <stdio.h>
#include <threads.h>

mtx_t mutex;

int main() {
    mtx_init(&mutex, mtx_plain);

    mtx_lock(&mutex);
    printf("进入临界区\n");
    mtx_unlock(&mutex);

    mtx_destroy(&mutex);
    return 0;
}
```

#### 一次性调用（call_once）

| 函数 | 说明 |
|------|------|
| `call_once()` | 确保某函数只被调用一次 |

---

## 条件变量（Condition Variables）

### 头文件：`<threads.h>`

用于线程间的等待与通知机制。

#### 类型

| 类型 | 说明 |
|------|------|
| `cnd_t` | 条件变量类型 |

#### 函数列表

| 函数 | 功能 |
|------|------|
| `cnd_init()` | 初始化条件变量 |
| `cnd_signal()` | 通知一个等待线程 |
| `cnd_broadcast()` | 通知所有等待线程 |
| `cnd_wait()` | 等待条件满足 |
| `cnd_timedwait()` | 带超时等待 |
| `cnd_destroy()` | 销毁条件变量 |

---

## 线程局部存储（Thread-Local Storage）

### 头文件：`<threads.h>`

允许每个线程拥有其专属的数据副本。

#### 类型和宏

| 名称 | 说明 |
|------|------|
| `thread_local` | 存储类修饰符（C11） |
| `tss_t` | 线程局部存储键 |
| `TSS_DTOR_ITERATIONS` | 析构函数最大调用次数 |
| `tss_dtor_t` | 析构函数指针类型：`void (*)(void*)` |

#### 函数列表

| 函数 | 功能 |
|------|------|
| `tss_create()` | 创建 TLS 键和析构函数 |
| `tss_get()` | 获取线程局部值 |
| `tss_set()` | 设置线程局部值 |
| `tss_delete()` | 删除 TLS 键 |

#### 示例代码

```c
#include <stdio.h>
#include <threads.h>

tss_t key;

void destructor(void *data) {
    free(data);
}

int main() {
    tss_create(&key, destructor);
    int *value = malloc(sizeof(int));
    *value = 42;
    tss_set(key, value);
    printf("TLS value: %d\n", *(int *)tss_get(key));
    tss_delete(key);
    return 0;
}
```

---

## 标准未来预留标识符

以下命名规则的标识符为标准保留：

| 规则 | 描述 |
|------|------|
| `cnd_`, `mtx_`, `thrd_`, `tss_` + 小写字母 | 供 `<threads.h>` 使用 |
| `ATOMIC_` + 大写字母 | 供 `<stdatomic.h>` 使用 |
| `atomic_`, `memory_` + 小写字母 | 类型标识符 |
| `memory_order_` + 小写字母 | `memory_order` 常量 |

> ❗ 未定义行为警告：不要自定义上述命名空间内的标识符。

---

## 参考标准文档

- **C23 标准 (ISO/IEC 9899:2024)**：
  - §7.17 Atomics `<stdatomic.h>`
  - §7.26 Threads `<threads.h>`
  - §7.31.8 Atomics `<stdatomic.h>`
  - §7.31.15 Threads `<threads.h>`

- **C17 标准 (ISO/IEC 9899:2018)**：
  - 同上章节（页码略）

- **C11 标准 (ISO/IEC 9899:2011)**：
  - §7.17 Atomics `<stdatomic.h>`（第 273–286 页）
  - §7.26 Threads `<threads.h>`（第 376–387 页）

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 并发库 | [C++ Concurrency Support Library](https://zh.cppreference.com/w/cpp/thread) |
| GNU Libc 手册 - ISO C Mutexes | [GNU Libc Manual: ISO C Mutexes](https://www.gnu.org/software/libc/manual/html_node/ISO-C-Mutexes.html) |

---

> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=c/thread&oldid=180203>  
> 最后修改时间：2025年2月6日 15:27 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

✅ **小结提示**：
- C 的并发支持是现代多核系统开发的重要基石；
- 多线程编程需特别注意数据竞争和死锁问题；
- 原子操作适合低延迟场景，互斥量适合复杂数据结构同步；
- 条件变量搭配互斥量构建生产者-消费者模型；
- 线程局部存储避免全局变量冲突，提高线程安全性。

如需更高层次的抽象和更多功能（如线程池、future/promise），可考虑使用 C++ 或第三方库（如 Pthreads、OpenMP）。