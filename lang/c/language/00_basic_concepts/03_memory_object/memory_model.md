# 内存模型 (Memory Model)

## 1. 概述 (Overview)

C 语言内存模型定义了 C 抽象机视角下计算机内存存储的语义。

### 核心概念

C 程序可用的数据存储（内存）由一个或多个**连续字节序列**组成。内存中的每个字节都有唯一的地址。

```
内存布局示意：
+------+------+------+------+------+------+
| 字节0 | 字节1 | 字节2 | 字节3 | ...  | 字节N |
+------+------+------+------+------+------+
   地址0   地址1   地址2   地址3          地址N
```

### 内存模型的组成

| 概念 | 说明 |
|-----|------|
| **字节 (Byte)** | 最小可寻址内存单元 |
| **内存位置 (Memory Location)** | 可独立访问的存储单元 |
| **线程与数据竞争** | 并发访问的规则 |
| **内存顺序** | 多线程可见性保证 |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C 语言内存模型的设计源于以下需求：

1. **抽象性**：提供跨平台的内存抽象
2. **效率**：允许直接内存操作
3. **并发支持**：C11 引入多线程内存模型

### 版本演进

| C 标准版本 | 发布年份 | 内存模型相关改进 |
|-----------|---------|-----------------|
| C89/C90 | 1989/1990 | 基本字节和内存概念 |
| C99 | 1999 | 完善字节定义 |
| C11 | 2011 | 引入多线程内存模型、数据竞争、内存顺序 |
| C23 | 2024 | 沿用 C11 模型 |

### C11 重大变革

C11 引入了完整的并发内存模型，包括：

- 线程概念
- 数据竞争定义
- 原子操作
- 内存顺序语义

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 字节相关

```c
#include <limits.h>

// 获取字节的位数
int bits_per_byte = CHAR_BIT;  // 通常为 8

// 字符类型占用一个字节
sizeof(char) == 1;         // 始终为真
sizeof(signed char) == 1;  // 始终为真
sizeof(unsigned char) == 1;  // 始终为真
```

### 3.2 内存位置相关

```c
struct Example {
    int a;           // 内存位置 #1
    int b : 5;       // 内存位置 #2
    int c : 11;      // 内存位置 #2（继续）
    int  : 0;        // 零长度位域，分隔
    int d : 8;       // 内存位置 #3
};
```

### 3.3 线程相关（C11）

```c
#include <threads.h>
#include <stdatomic.h>

// 原子类型
atomic_int counter;

// 内存顺序
enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
};
```

## 4. 底层原理 (Underlying Principles)

### 4.1 字节 (Byte)

**字节是内存的最小可寻址单元**。

#### 定义

- 字节是一个连续的位序列
- 大小足以容纳基本执行字符集的任何成员（96 个必需的单字节字符）
- C 支持大小为 8 位及更大的字节

#### 字符类型与字节

| 类型 | 存储大小 | 说明 |
|-----|---------|------|
| `char` | 1 字节 | 用于存储和值表示 |
| `signed char` | 1 字节 | 有符号字符 |
| `unsigned char` | 1 字节 | 无符号字符 |

```c
// CHAR_BIT 定义字节的位数
#include <limits.h>

printf("每字节位数: %d\n", CHAR_BIT);  // 通常输出 8
```

### 4.2 内存位置 (Memory Location)

**内存位置**是以下之一：

1. **标量类型对象**：
   - 算术类型（整数、浮点）
   - 指针类型
   - 枚举类型

2. **最大连续非零长度位域序列**：
   - 相邻的位域可能共享同一内存位置
   - 零长度位域会分隔内存位置

#### 内存位置示例

```c
struct S {
    char a;         // 内存位置 #1
    int b : 5;      // 内存位置 #2
    int c : 11,     // 内存位置 #2（继续）
          : 0,      // 零长度位域，分隔符
        d : 8;      // 内存位置 #3
    struct {
        int ee : 8; // 内存位置 #4
    } e;
} obj;  // 对象 'obj' 由 4 个独立内存位置组成
```

#### 内存位置图解

```
struct S 内存布局：

内存位置 #1: [ a (char) ]
内存位置 #2: [ b:5 | c:11 ]        ← 共享同一位置
             [---分隔符---:0]
内存位置 #3: [ d:8 ]
内存位置 #4: [ ee:8 ]
```

### 4.3 线程与数据竞争 (Threads and Data Races)

#### 线程定义

**执行线程 (Thread of execution)** 是程序内的控制流，始于通过 `thrd_create` 或其他方式调用顶层函数。

#### 并发访问规则

| 规则 | 说明 |
|-----|------|
| 独立内存位置 | 不同线程可以并发访问不同内存位置，无需同步 |
| 同一内存位置 | 并发访问同一内存位置需要同步或原子操作 |
| 位域警告 | 同一结构体中的非原子位域即使不重叠也可能竞争 |

#### 数据竞争定义

当满足以下条件时，两个表达式求值**冲突 (conflict)**：

1. 一个求值写入内存位置
2. 另一个求值读取或修改同一内存位置

**数据竞争 (Data race)**：程序中存在两个冲突求值，除非：

1. 两个冲突求值都是**原子操作**
2. 一个求值 **happens-before** 另一个

**如果发生数据竞争，程序行为未定义！**

#### happens-before 关系

```c
// mutex 操作示例
mtx_lock(&mutex);    // 获取锁
// 临界区代码
mtx_unlock(&mutex);  // 释放锁

// mtx_unlock 与同一互斥量的 mtx_unlock（另一线程）
// 存在 synchronizes-with 关系
// 因此 happens-before 关系成立
```

### 4.4 内存顺序 (Memory Order)

当一个线程从内存位置读取值时，可能看到：

1. **初始值**
2. **同一线程写入的值**
3. **其他线程写入的值**

内存顺序定义了线程写入何时对其他线程可见。

| 内存顺序 | 说明 |
|---------|------|
| `memory_order_relaxed` | 无同步，仅保证原子性 |
| `memory_order_acquire` | 获取语义，防止后续操作重排到前面 |
| `memory_order_release` | 释放语义，防止前面操作重排到后面 |
| `memory_order_acq_rel` | 获取+释放语义 |
| `memory_order_seq_cst` | 顺序一致性（默认，最强保证） |

## 5. 使用场景 (Use Cases)

### 5.1 内存位置的应用

| 场景 | 考虑事项 |
|-----|---------|
| 位域设计 | 注意共享内存位置的并发问题 |
| 结构体布局 | 理解内存位置边界 |
| 原子操作 | 需要操作整个内存位置 |
| 并发编程 | 不同内存位置可独立访问 |

### 5.2 避免数据竞争的策略

| 策略 | 说明 |
|-----|------|
| 互斥量 (Mutex) | 使用 `mtx_lock`/`mtx_unlock` 保护共享数据 |
| 原子操作 | 使用 `stdatomic.h` 中的原子类型 |
| 线程局部存储 | 使用 `_Thread_local` 避免共享 |
| 消息传递 | 线程间通信而非共享内存 |

### 5.3 注意事项

1. **位域并发问题**：
   ```c
   struct {
       int a : 1;
       int b : 1;  // 与 a 共享内存位置！
   } flags;

   // 不同线程同时修改 a 和 b 是数据竞争！
   ```

2. **原子操作大小**：
   - 原子操作必须作用于完整内存位置
   - 不能对部分位域进行原子操作

3. **内存顺序选择**：
   - 默认使用 `memory_order_seq_cst`
   - 性能关键代码可考虑更宽松的顺序

## 6. 代码示例 (Examples)

### 6.1 字节与内存基础

```c
#include <stdio.h>
#include <limits.h>

int main(void) {
    printf("=== 字节信息 ===\n");
    printf("每字节位数: %d\n", CHAR_BIT);
    printf("sizeof(char) = %zu\n", sizeof(char));
    printf("sizeof(int) = %zu\n", sizeof(int));
    printf("sizeof(double) = %zu\n", sizeof(double));

    // 字节序检测
    int value = 1;
    unsigned char* bytes = (unsigned char*)&value;
    printf("\n=== 字节序 ===\n");
    if (bytes[0] == 1) {
        printf("小端序 (Little-endian)\n");
    } else {
        printf("大端序 (Big-endian)\n");
    }

    return 0;
}
```

### 6.2 内存位置分析

```c
#include <stdio.h>
#include <stddef.h>

struct MemoryLayout {
    char a;           // 内存位置 #1
    int b : 5;        // 内存位置 #2
    int c : 11;       // 内存位置 #2（继续）
    int  : 0;         // 分隔符
    int d : 8;        // 内存位置 #3
    double e;         // 内存位置 #4
};

int main(void) {
    printf("=== 内存位置分析 ===\n");
    printf("sizeof(MemoryLayout) = %zu\n", sizeof(struct MemoryLayout));
    printf("offsetof(a) = %zu\n", offsetof(struct MemoryLayout, a));
    printf("offsetof(e) = %zu\n", offsetof(struct MemoryLayout, e));

    // 统计内存位置数量
    printf("\n内存位置数量: 4\n");
    printf("  #1: char a\n");
    printf("  #2: bit-fields b, c\n");
    printf("  #3: bit-field d\n");
    printf("  #4: double e\n");

    return 0;
}
```

### 6.3 多线程安全访问（C11）

```c
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

// 安全：使用原子变量
atomic_int counter = 0;

int increment(void* arg) {
    for (int i = 0; i < 10000; ++i) {
        atomic_fetch_add(&counter, 1);
    }
    return 0;
}

int main(void) {
    thrd_t t1, t2;

    thrd_create(&t1, increment, NULL);
    thrd_create(&t2, increment, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    printf("Counter: %d\n", atomic_load(&counter));  // 必然是 20000

    return 0;
}
```

### 6.4 使用互斥量保护共享数据（C11）

```c
#include <stdio.h>
#include <threads.h>
#include <stdlib.h>

typedef struct {
    int value;
    mtx_t mutex;
} SharedCounter;

int increment(void* arg) {
    SharedCounter* sc = (SharedCounter*)arg;

    for (int i = 0; i < 10000; ++i) {
        mtx_lock(&sc->mutex);
        sc->value++;
        mtx_unlock(&sc->mutex);
    }

    return 0;
}

int main(void) {
    SharedCounter sc = {0,};
    mtx_init(&sc.mutex, mtx_plain);

    thrd_t t1, t2;
    thrd_create(&t1, increment, &sc);
    thrd_create(&t2, increment, &sc);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    printf("Counter: %d\n", sc.value);  // 必然是 20000

    mtx_destroy(&sc.mutex);
    return 0;
}
```

### 6.5 内存顺序示例（C11）

```c
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

// 简单的生产者-消费者模式
atomic_int data = 0;
atomic_int ready = 0;

int producer(void* arg) {
    data = 42;
    // 释放语义：确保上面的写入对其他线程可见
    atomic_store_explicit(&ready, 1, memory_order_release);
    return 0;
}

int consumer(void* arg) {
    // 获取语义：确保看到 ready 后，也能看到 data
    while (atomic_load_explicit(&ready, memory_order_acquire) == 0) {
        // 等待
    }
    printf("Received data: %d\n", data);  // 必然看到 42
    return 0;
}

int main(void) {
    thrd_t t1, t2;

    thrd_create(&t1, producer, NULL);
    thrd_create(&t2, consumer, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    return 0;
}
```

### 6.6 常见错误及修正

#### 错误示例 1：数据竞争

```c
// 错误：无同步的共享变量访问
#include <stdio.h>
#include <threads.h>

int counter = 0;  // 非原子变量

int increment(void* arg) {
    for (int i = 0; i < 10000; ++i) {
        counter++;  // 数据竞争！
    }
    return 0;
}

int main(void) {
    thrd_t t1, t2;
    thrd_create(&t1, increment, NULL);
    thrd_create(&t2, increment, NULL);
    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    printf("Counter: %d\n", counter);  // 可能不是 20000
    return 0;
}
```

**正确做法**：

```c
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int counter = 0;  // 原子变量

int increment(void* arg) {
    for (int i = 0; i < 10000; ++i) {
        atomic_fetch_add(&counter, 1);  // 安全
    }
    return 0;
}

int main(void) {
    thrd_t t1, t2;
    thrd_create(&t1, increment, NULL);
    thrd_create(&t2, increment, NULL);
    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    printf("Counter: %d\n", atomic_load(&counter));  // 必然是 20000
    return 0;
}
```

#### 错误示例 2：位域并发修改

```c
// 错误：并发修改同一位域组的成员
#include <stdio.h>
#include <threads.h>

struct Flags {
    unsigned int a : 1;
    unsigned int b : 1;  // 与 a 共享内存位置！
};

struct Flags flags = {0, 0};

int set_a(void* arg) {
    for (int i = 0; i < 10000; ++i) {
        flags.a = 1;  // 数据竞争！
        flags.a = 0;
    }
    return 0;
}

int set_b(void* arg) {
    for (int i = 0; i < 10000; ++i) {
        flags.b = 1;  // 数据竞争！
        flags.b = 0;
    }
    return 0;
}
```

**正确做法**：

```c
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

// 方案1：使用独立变量
unsigned char a = 0;  // 独立内存位置
unsigned char b = 0;  // 独立内存位置

// 方案2：使用整个原子变量
struct Flags {
    unsigned int a : 1;
    unsigned int b : 1;
};

// 使用互斥量或原子操作访问整个结构体
```

#### 错误示例 3：错误的内存顺序

```c
// 错误：使用过于宽松的内存顺序
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

int data = 0;
atomic_int ready = 0;

int producer(void* arg) {
    data = 42;
    atomic_store_explicit(&ready, 1, memory_order_relaxed);  // 可能不安全！
    return 0;
}

int consumer(void* arg) {
    while (atomic_load_explicit(&ready, memory_order_relaxed) == 0) {
        // 等待
    }
    printf("Data: %d\n", data);  // 可能看不到 42！
    return 0;
}
```

**正确做法**：

```c
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

int data = 0;
atomic_int ready = 0;

int producer(void* arg) {
    data = 42;
    // 释放语义确保 data 写入可见
    atomic_store_explicit(&ready, 1, memory_order_release);
    return 0;
}

int consumer(void* arg) {
    // 获取语义确保看到 ready 后也能看到 data
    while (atomic_load_explicit(&ready, memory_order_acquire) == 0) {
        // 等待
    }
    printf("Data: %d\n", data);  // 必然看到 42
    return 0;
}
```

## 7. 总结 (Summary)

### 7.1 核心要点

1. **字节**：
   - 最小可寻址内存单元
   - 大小至少 8 位，由 `CHAR_BIT` 指定
   - `char` 类型占用一个字节

2. **内存位置**：
   - 标量类型对象
   - 最大连续非零长度位域序列
   - 零长度位域分隔内存位置

3. **数据竞争**：
   - 两个冲突求值访问同一内存位置
   - 必须使用原子操作或同步
   - 否则行为未定义

4. **内存顺序**：
   - 定义跨线程写入可见性
   - 默认顺序一致性
   - 性能优化可使用更宽松顺序

### 7.2 内存位置规则

| 类型 | 内存位置 |
|-----|---------|
| 独立标量变量 | 各自独立 |
| 位域 | 连续非零位域共享 |
| 零长度位域 | 分隔符 |
| 结构体成员 | 通常各自独立 |

### 7.3 与 C++ 的对比

| 特性 | C | C++ |
|-----|---|-----|
| 内存模型基础 | 相同 | 相同 |
| 原子操作 | C11 `<stdatomic.h>` | C++11 `<atomic>` |
| 线程支持 | C11 `<threads.h>` | C++11 `<thread>` |
| 内存顺序 | 相同 | 相同 |

### 7.4 学习建议

1. **理解基本概念**：
   - 字节是内存的基本单位
   - 内存位置决定并发访问边界

2. **掌握并发规则**：
   - 不同内存位置可安全并发访问
   - 同一内存位置需要同步

3. **正确使用原子操作**：
   - 理解内存顺序语义
   - 默认使用 `memory_order_seq_cst`
   - 性能关键时考虑更宽松顺序

4. **避免数据竞争**：
   - 使用互斥量或原子操作
   - 注意位域共享内存位置问题
   - 使用 ThreadSanitizer 检测

---

**参考资源**：
- ISO/IEC 9899:2024 (C23 标准)
- `<stdatomic.h>` - 原子操作
- `<threads.h>` - 线程支持
- `memory_order` - 内存顺序
- 对象表示 (Object Representation)