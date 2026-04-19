# 内存模型 (Memory Model)

## 1. 概述 (Overview)

C++ 内存模型定义了 C++ 抽象机视角下计算机内存存储的语义。

### 核心概念

C++ 程序可用的内存由一个或多个**连续字节序列**组成。内存中的每个字节都有唯一的地址。

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

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 内存模型的设计源于以下需求：

1. **抽象性**：提供跨平台的内存抽象
2. **与 C 兼容**：继承 C 语言的内存模型
3. **并发支持**：C++11 引入完整的并发内存模型

### 版本演进

| C++ 标准版本 | 发布年份 | 内存模型相关改进 |
|-------------|---------|-----------------|
| C++98 | 1998 | 基本字节和内存概念 |
| C++11 | 2011 | 引入完整并发内存模型（原子操作、内存顺序） |
| C++23 | 2023 | 更新字节定义以支持 UTF-8 |

### 缺陷报告

| 缺陷报告 | 问题 | 解决方案 |
|---------|------|---------|
| CWG 1953 | 占用相同存储的对象被视为不同内存位置 | 内存位置现在指向存储 |

### C++11 重大变革

C++11 引入了完整的并发内存模型，定义在 `<atomic>` 头文件中，包括：

- 原子类型 (`std::atomic`)
- 内存顺序 (`std::memory_order`)
- 数据竞争定义

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 字节相关

```cpp
#include <climits>
#include <limits>

// 获取字节的位数
int bits_per_byte = CHAR_BIT;  // 通常为 8

// 或使用 C++ 方式
int bits = std::numeric_limits<unsigned char>::digits;

// 字符类型占用一个字节
static_assert(sizeof(char) == 1);
static_assert(sizeof(signed char) == 1);
static_assert(sizeof(unsigned char) == 1);
```

### 3.2 内存位置相关

```cpp
struct Example {
    int a;           // 内存位置 #1
    int b : 5;       // 内存位置 #2
    int c : 11;      // 内存位置 #2（继续）
    int  : 0;        // 零长度位域，分隔
    int d : 8;       // 内存位置 #3
};
```

### 3.3 原子操作相关（C++11）

```cpp
#include <atomic>

// 原子类型
std::atomic<int> counter{0};

// 内存顺序
namespace std {
    enum memory_order {
        memory_order_relaxed,
        memory_order_consume,
        memory_order_acquire,
        memory_order_release,
        memory_order_acq_rel,
        memory_order_seq_cst
    };
}
```

## 4. 底层原理 (Underlying Principles)

### 4.1 字节 (Byte)

**字节是内存的最小可寻址单元**。

#### 定义

字节是一个连续的位序列，大小足以容纳：

| C++ 版本 | 定义 |
|---------|------|
| C++23 之前 | 任何 UTF-8 码元的值（256 个不同值）和基本执行字符集的任何成员 |
| C++23 起 | 基本字面字符集的任何元素的普通字面编码 |

C++ 支持 8 位及更大的字节。

#### 字符类型与字节

| 类型 | 存储大小 | 说明 |
|-----|---------|------|
| `char` | 1 字节 | 用于存储和值表示 |
| `signed char` | 1 字节 | 有符号字符 |
| `unsigned char` | 1 字节 | 无符号字符 |

```cpp
#include <climits>
#include <limits>
#include <iostream>

int main() {
    std::cout << "每字节位数: " << CHAR_BIT << '\n';
    std::cout << "每字节位数: "
              << std::numeric_limits<unsigned char>::digits << '\n';
    // 通常输出 8
}
```

### 4.2 内存位置 (Memory Location)

**内存位置**是以下之一：

1. **非位域标量类型对象的对象表示所占用的存储**：
   - 算术类型（整数、浮点）
   - 指针类型
   - 枚举类型

2. **最大连续非零长度位域序列**：
   - 相邻的位域可能共享同一内存位置
   - 零长度位域会分隔内存位置

#### 实现管理的额外内存位置

语言的各种特性（如引用和虚函数）可能涉及额外的内存位置，这些位置程序无法访问但由实现管理。

| 特性 | 额外存储 |
|-----|---------|
| 虚函数 | 虚表指针 (vptr) |
| 虚继承 | 虚基类指针 |
| 引用 | 可能需要存储 |

#### 内存位置示例

```cpp
struct S {
    char a;         // 内存位置 #1
    int b : 5;      // 内存位置 #2
    int c : 11,     // 内存位置 #2（继续）
          : 0,      // 零长度位域，分隔符
        d : 8;      // 内存位置 #3
    struct {
        int ee : 8; // 内存位置 #4
    } e;
} obj;  // 对象 "obj" 由 4 个独立内存位置组成
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

### 4.3 并发内存模型（C++11）

C++11 引入了完整的并发内存模型，主要概念包括：

#### 数据竞争

当满足以下条件时，两个表达式求值**冲突**：

1. 一个求值写入内存位置
2. 另一个求值读取或修改同一内存位置

**数据竞争**：程序中存在两个冲突求值，除非：

1. 两个冲突求值都是**原子操作**
2. 一个求值 **happens-before** 另一个

**如果发生数据竞争，程序行为未定义！**

#### 内存顺序

C++ 定义了六种内存顺序：

| 内存顺序 | 说明 | 典型用途 |
|---------|------|---------|
| `memory_order_relaxed` | 无同步，仅保证原子性 | 计数器 |
| `memory_order_consume` | 消费语义（已弃用） | 数据依赖 |
| `memory_order_acquire` | 获取语义，防止后续重排 | 锁获取 |
| `memory_order_release` | 释放语义，防止前面重排 | 锁释放 |
| `memory_order_acq_rel` | 获取+释放 | 读写锁 |
| `memory_order_seq_cst` | 顺序一致性（默认） | 最强保证 |

## 5. 使用场景 (Use Cases)

### 5.1 内存位置的应用

| 场景 | 考虑事项 |
|-----|---------|
| 位域设计 | 注意共享内存位置的并发问题 |
| 结构体布局 | 理解内存位置边界 |
| 原子操作 | 需要操作整个内存位置 |
| 并发编程 | 不同内存位置可独立访问 |

### 5.2 避免数据竞争的策略

| 策略 | C++ 工具 |
|-----|---------|
| 互斥量 | `std::mutex`, `std::lock_guard` |
| 原子操作 | `std::atomic<T>` |
| 线程局部存储 | `thread_local` |
| 消息传递 | `std::promise`, `std::future` |

### 5.3 注意事项

1. **位域并发问题**：
   ```cpp
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

```cpp
#include <iostream>
#include <climits>
#include <limits>

int main() {
    std::cout << "=== 字节信息 ===\n";
    std::cout << "每字节位数: " << CHAR_BIT << '\n';
    std::cout << "numeric_limits 方式: "
              << std::numeric_limits<unsigned char>::digits << '\n';

    std::cout << "\n=== 类型大小 ===\n";
    std::cout << "sizeof(char) = " << sizeof(char) << '\n';
    std::cout << "sizeof(int) = " << sizeof(int) << '\n';
    std::cout << "sizeof(double) = " << sizeof(double) << '\n';

    // 字节序检测
    int value = 1;
    auto bytes = reinterpret_cast<unsigned char*>(&value);
    std::cout << "\n=== 字节序 ===\n";
    if (bytes[0] == 1) {
        std::cout << "小端序 (Little-endian)\n";
    } else {
        std::cout << "大端序 (Big-endian)\n";
    }

    return 0;
}
```

### 6.2 内存位置分析

```cpp
#include <iostream>
#include <cstddef>

struct MemoryLayout {
    char a;           // 内存位置 #1
    int b : 5;        // 内存位置 #2
    int c : 11;       // 内存位置 #2（继续）
    int  : 0;         // 分隔符
    int d : 8;        // 内存位置 #3
    double e;         // 内存位置 #4
};

int main() {
    std::cout << "=== 内存位置分析 ===\n";
    std::cout << "sizeof(MemoryLayout) = " << sizeof(MemoryLayout) << '\n';
    std::cout << "offsetof(a) = " << offsetof(MemoryLayout, a) << '\n';
    std::cout << "offsetof(e) = " << offsetof(MemoryLayout, e) << '\n';

    // 统计内存位置数量
    std::cout << "\n内存位置数量: 4\n";
    std::cout << "  #1: char a\n";
    std::cout << "  #2: bit-fields b, c\n";
    std::cout << "  #3: bit-field d\n";
    std::cout << "  #4: double e\n";

    return 0;
}
```

### 6.3 多线程安全访问（C++11）

```cpp
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> counter{0};

void increment() {
    for (int i = 0; i < 10000; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter.load() << '\n';  // 必然是 20000

    return 0;
}
```

### 6.4 使用互斥量保护共享数据（C++11）

```cpp
#include <iostream>
#include <thread>
#include <mutex>

class SharedCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }

private:
    int value_ = 0;
    mutable std::mutex mutex_;
};

int main() {
    SharedCounter counter;

    auto work = [&counter]() {
        for (int i = 0; i < 10000; ++i) {
            counter.increment();
        }
    };

    std::thread t1(work);
    std::thread t2(work);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter.get() << '\n';  // 必然是 20000

    return 0;
}
```

### 6.5 内存顺序示例（C++11）

```cpp
#include <iostream>
#include <thread>
#include <atomic>

// 生产者-消费者模式
int data = 0;
std::atomic<bool> ready{false};

void producer() {
    data = 42;
    // 释放语义：确保上面的写入对其他线程可见
    ready.store(true, std::memory_order_release);
}

void consumer() {
    // 获取语义：确保看到 ready 后，也能看到 data
    while (!ready.load(std::memory_order_acquire)) {
        // 等待
    }
    std::cout << "Received data: " << data << '\n';  // 必然看到 42
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
```

### 6.6 使用 std::atomic_ref（C++20）

```cpp
#include <iostream>
#include <thread>
#include <atomic>

int main() {
    int value = 0;

    auto increment = [&value]() {
        for (int i = 0; i < 10000; ++i) {
            // C++20: 对现有变量进行原子操作
            std::atomic_ref<int> ref(value);
            ref.fetch_add(1, std::memory_order_relaxed);
        }
    };

    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Value: " << value << '\n';  // 必然是 20000

    return 0;
}
```

### 6.7 常见错误及修正

#### 错误示例 1：数据竞争

```cpp
// 错误：无同步的共享变量访问
#include <iostream>
#include <thread>

int counter = 0;  // 非原子变量

void increment() {
    for (int i = 0; i < 10000; ++i) {
        counter++;  // 数据竞争！
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter << '\n';  // 可能不是 20000
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> counter{0};  // 原子变量

void increment() {
    for (int i = 0; i < 10000; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);  // 安全
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter.load() << '\n';  // 必然是 20000
    return 0;
}
```

#### 错误示例 2：位域并发修改

```cpp
// 错误：并发修改同一位域组的成员
#include <thread>

struct Flags {
    unsigned int a : 1;
    unsigned int b : 1;  // 与 a 共享内存位置！
};

Flags flags = {0, 0};

void set_a() {
    for (int i = 0; i < 10000; ++i) {
        flags.a = 1;  // 数据竞争！
        flags.a = 0;
    }
}

void set_b() {
    for (int i = 0; i < 10000; ++i) {
        flags.b = 1;  // 数据竞争！
        flags.b = 0;
    }
}
```

**正确做法**：

```cpp
#include <thread>
#include <atomic>

// 方案1：使用独立变量
std::atomic<bool> a{false};  // 独立内存位置
std::atomic<bool> b{false};  // 独立内存位置

// 方案2：使用整个原子变量
struct Flags {
    unsigned int a : 1;
    unsigned int b : 1;
};

// 使用互斥量或整体原子操作访问
std::atomic<unsigned int> flags{0};

void set_a() {
    flags.fetch_or(0x01, std::memory_order_relaxed);
}

void set_b() {
    flags.fetch_or(0x02, std::memory_order_relaxed);
}
```

#### 错误示例 3：错误的内存顺序

```cpp
// 错误：使用过于宽松的内存顺序
#include <iostream>
#include <thread>
#include <atomic>

int data = 0;
std::atomic<bool> ready{false};

void producer() {
    data = 42;
    ready.store(true, std::memory_order_relaxed);  // 可能不安全！
}

void consumer() {
    while (!ready.load(std::memory_order_relaxed)) {
        // 等待
    }
    std::cout << "Data: " << data << '\n';  // 可能看不到 42！
}
```

**正确做法**：

```cpp
#include <iostream>
#include <thread>
#include <atomic>

int data = 0;
std::atomic<bool> ready{false};

void producer() {
    data = 42;
    // 释放语义确保 data 写入可见
    ready.store(true, std::memory_order_release);
}

void consumer() {
    // 获取语义确保看到 ready 后也能看到 data
    while (!ready.load(std::memory_order_acquire)) {
        // 等待
    }
    std::cout << "Data: " << data << '\n';  // 必然看到 42
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

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
   - 非位域标量类型对象的存储
   - 最大连续非零长度位域序列
   - 零长度位域作为分隔符

3. **数据竞争（C++11）**：
   - 两个冲突求值访问同一内存位置
   - 必须使用原子操作或同步
   - 否则行为未定义

4. **内存顺序（C++11）**：
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

### 7.3 与 C 的对比

| 特性 | C++ | C |
|-----|-----|---|
| 基本概念 | 相同 | 相同 |
| 原子操作 | `std::atomic<T>` | `_Atomic T` |
| 线程支持 | `<thread>` | `<threads.h>` |
| 内存顺序 | `std::memory_order` | `memory_order` |
| `atomic_ref` | C++20 | 无 |

### 7.4 C++ 特有特性

| 特性 | 版本 | 说明 |
|-----|------|------|
| `std::atomic<T>` | C++11 | 原子类型模板 |
| `std::atomic_ref<T>` | C++20 | 对现有变量的原子引用 |
| `std::memory_order` | C++11 | 内存顺序枚举 |
| `std::atomic_flag` | C++11 | 最小原子布尔类型 |
| `std::mutex` | C++11 | 互斥量 |
| `std::shared_mutex` | C++14 | 读写互斥量 |

### 7.5 学习建议

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

4. **利用现代 C++ 工具**：
   - `std::atomic<T>` 进行原子操作
   - `std::mutex` 保护临界区
   - `std::atomic_ref<T>` (C++20) 对现有变量原子操作

5. **避免数据竞争**：
   - 使用 ThreadSanitizer 检测
   - 注意位域共享内存位置问题

---

**参考资源**：
- ISO/IEC 14882:2024 (C++23 标准)
- `<atomic>` - 原子操作库
- `<thread>` - 线程支持库
- `<mutex>` - 互斥量
- `std::memory_order` - 内存顺序