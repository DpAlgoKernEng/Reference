# 静态存储期 (Static Storage Duration)

## 1. 概述 (Overview)

**静态存储期 (Static Storage Duration)** 是 C 语言中对象的一种存储期类型。如果一个对象具有静态存储期，则其生命周期覆盖整个程序的执行过程，且其存储值只在程序启动前初始化一次。

### 定义

满足以下任一条件的对象具有静态存储期：

1. 标识符声明时**没有**使用 `_Thread_local` 存储类说明符，且具有：
   - 外部链接 (external linkage)，或
   - 内部链接 (internal linkage)

2. 标识符声明时使用了 `static` 存储类说明符

### 核心特征

| 特性 | 描述 |
|------|------|
| **生命周期** | 整个程序执行期间 |
| **初始化时机** | 程序启动前（仅一次） |
| **存储位置** | 静态存储区（数据段或 BSS 段） |
| **默认初始化** | 自动初始化为零（零初始化） |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

静态存储期概念最早出现在 C 语言的前身 B 语言中，并在 1972 年 Dennis Ritchie 设计 C 语言时被正式引入。

### 设计动机

静态存储期的设计主要解决以下问题：

1. **持久状态保持**：需要在函数调用之间保持状态，但又不想使用全局变量
2. **性能优化**：避免每次函数调用时的重复初始化开销
3. **内存管理**：为程序提供一种在编译时就能确定内存分配的机制

### 版本变更

| C 标准版本 | 变更内容 |
|-----------|---------|
| **C89/C90** | 正式定义静态存储期概念 |
| **C99** | 引入变长数组 (VLA)，明确静态存储期对象不能为变长 |
| **C11** | 引入 `_Thread_local`，区分线程存储期和静态存储期 |

### 关键演进

- **早期 C 语言**：`static` 关键字同时用于定义链接属性和存储期
- **C11 标准**：引入 `_Thread_local`，使静态存储期定义更加清晰，允许同一标识符在不同线程中拥有独立实例

## 3. 语法与参数 (Syntax and Parameters)

### 声明语法

#### 文件作用域（全局变量）

```c
// 外部链接 + 静态存储期（默认）
int global_var;           // 定义，外部链接

// 内部链接 + 静态存储期
static int file_var;      // 定义，内部链接

// 外部声明（引用其他文件的定义）
extern int external_var;  // 声明，外部链接
```

#### 块作用域（局部变量）

```c
void function(void) {
    static int count = 0;  // 静态存储期 + 块作用域
    int auto_var = 0;      // 自动存储期（对比）
}
```

### 存储类说明符组合

| 存储类说明符 | 链接属性 | 存储期 | 作用域 |
|-------------|---------|--------|--------|
| 无（文件作用域） | 外部 | 静态 | 文件 |
| `static`（文件作用域） | 内部 | 静态 | 文件 |
| `static`（块作用域） | 无 | 静态 | 块 |
| `extern` | 外部 | 静态 | 块/文件 |
| 无（块作用域） | 无 | 自动 | 块 |
| `auto` | 无 | 自动 | 块 |
| `register` | 无 | 自动 | 块 |
| `_Thread_local` | 可变 | 线程 | 块/文件 |

### 初始化规则

```c
// 显式初始化
static int a = 10;          // 初始化为 10

// 隐式初始化（零初始化）
static int b;                // 自动初始化为 0
static int arr[5];           // 所有元素初始化为 0
static char str[10];         // 所有字符初始化为 '\0'
static int *ptr;             // 初始化为 NULL

// 非法初始化（编译错误）
void func(int n) {
    static int arr[n];       // 错误：静态存储期对象不能为变长
}
```

## 4. 底层原理 (Underlying Principles)

### 内存布局

静态存储期对象在程序内存中的位置：

```
+------------------+
|    代码段        |  (.text) - 可执行代码
+------------------+
|    只读数据段    |  (.rodata) - 字符串字面量、const 全局变量
+------------------+
|    数据段        |  (.data) - 已初始化的静态/全局变量
+------------------+
|    BSS 段        |  (.bss) - 未初始化的静态/全局变量（零初始化）
+------------------+
|    堆            |  ↑ 动态分配，向下增长
|                  |
+------------------+
|    栈            |  ↓ 函数调用栈，向上增长
+------------------+
```

### 初始化机制

#### 静态初始化 (Static Initialization)

发生在程序启动前：

1. **零初始化 (Zero Initialization)**
   - 未显式初始化的静态对象
   - 执行时机：编译时/加载时
   - 所有位清零

2. **常量初始化 (Constant Initialization)**
   - 使用常量表达式初始化
   - 执行时机：编译时
   - 值嵌入可执行文件

#### 动态初始化 (Dynamic Initialization)

如果初始化表达式不是常量表达式：

```c
int get_value(void) { return 42; }
static int global = get_value();  // 动态初始化
```

- 执行时机：`main()` 执行前
- 执行顺序：同一编译单元内按定义顺序
- **注意**：跨编译单元的初始化顺序未定义

### 生命周期管理

```
程序加载
    │
    ▼
静态初始化（零初始化 + 常量初始化）
    │
    ▼
动态初始化（如果需要）
    │
    ▼
main() 执行
    │
    ├── 函数调用时访问静态局部变量
    │
    ▼
main() 返回
    │
    ▼
程序终止（静态对象销毁）
```

### 性能特征

| 操作 | 时间复杂度 | 空间开销 | 备注 |
|------|-----------|---------|------|
| 访问 | O(1) | 无额外开销 | 直接地址访问 |
| 初始化 | O(1) | 编译时完成 | 常量初始化 |
| 动态初始化 | O(n) | 启动时开销 | n = 需动态初始化的对象数 |

### 线程安全性

- **C11 之前**：静态局部变量初始化不是线程安全的
- **C11 之后**：静态局部变量初始化保证原子性（编译器支持）

```c
// C11 线程安全的初始化
int* get_instance(void) {
    static int instance = compute();  // 首次调用时原子初始化
    return &instance;
}
```

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 函数调用计数器

```c
void track_calls(void) {
    static int call_count = 0;
    call_count++;
    printf("Called %d times\n", call_count);
}
```

#### 2. 单例模式实现

```c
typedef struct {
    int data;
} Singleton;

Singleton* get_singleton(void) {
    static Singleton instance = {0};
    return &instance;
}
```

#### 3. 缓存计算结果

```c
int expensive_computation(int n) {
    static int cache[100] = {0};
    static int computed[100] = {0};

    if (!computed[n]) {
        cache[n] = /* 复杂计算 */;
        computed[n] = 1;
    }
    return cache[n];
}
```

#### 4. 模块私有状态

```c
// file: counter.c
static int counter = 0;  // 文件作用域，内部链接

void increment(void) { counter++; }
int get_count(void) { return counter; }
```

### 最佳实践

#### 推荐做法

1. **合理使用作用域限制**
   ```c
   // 好的做法：限制作用域
   static int module_state = 0;  // 文件作用域，避免命名冲突

   // 不推荐：过度使用全局变量
   int g_global;  // 外部链接，可能被其他文件意外修改
   ```

2. **明确初始化**
   ```c
   // 好的做法：显式初始化
   static int count = 0;

   // 也可以：依赖零初始化
   static int count;  // 自动为 0
   ```

3. **使用 const 保护只读数据**
   ```c
   static const int lookup_table[] = {1, 2, 3, 4, 5};
   ```

### 常见陷阱

#### 1. 初始化顺序问题

```c
// file1.c
int get_value(void) { return 42; }
int global1 = get_value();  // 动态初始化

// file2.c
extern int global1;
int global2 = global1;  // 危险！global1 可能未初始化
```

**修正方案**：使用函数封装

```c
int get_global1(void) {
    static int value = 0;
    static int initialized = 0;
    if (!initialized) {
        value = get_value();
        initialized = 1;
    }
    return value;
}
```

#### 2. 线程安全问题

```c
// 非线程安全（C11 之前）
int* get_buffer(void) {
    static int buffer[100];
    return buffer;
}

// 线程安全版本
int* get_thread_buffer(void) {
    _Thread_local static int buffer[100];  // C11
    return buffer;
}
```

#### 3. 递归函数中的状态

```c
// 危险：递归调用会影响静态变量
int fibonacci(int n) {
    static int call_count = 0;  // 递归会多次修改
    call_count++;
    if (n <= 1) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

// 修正：传递状态参数
int fibonacci(int n, int* count) {
    (*count)++;
    if (n <= 1) return n;
    return fibonacci(n-1, count) + fibonacci(n-2, count);
}
```

#### 4. 可重入性问题

```c
// 不可重入函数
char* int_to_str(int num) {
    static char buffer[20];  // 信号处理程序中可能出问题
    sprintf(buffer, "%d", num);
    return buffer;
}

// 可重入版本
char* int_to_str_r(int num, char* buffer, size_t size) {
    snprintf(buffer, size, "%d", num);
    return buffer;
}
```

## 6. 代码示例 (Examples)

### 基础用法：静态局部变量

```c
#include <stdio.h>

void f(void)
{
    static int count = 0;   // 静态存储期，块作用域
    int i = 0;              // 自动存储期，块作用域
    printf("%d %d\n", i++, count++);
}

int main(void)
{
    for (int ndx = 0; ndx < 10; ++ndx)
        f();
    return 0;
}
```

**输出**：
```
0 0
0 1
0 2
0 3
0 4
0 5
0 6
0 7
0 8
0 9
```

**解析**：
- `i` 是自动变量，每次调用 `f()` 都重新初始化为 0
- `count` 是静态变量，只在程序启动时初始化一次，之后保持状态

### 高级用法：延迟初始化模式

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* data;
    size_t size;
    int initialized;
} LazyBuffer;

int* get_large_buffer(size_t size) {
    static LazyBuffer buf = {NULL, 0, 0};

    if (!buf.initialized) {
        buf.data = (int*)malloc(size * sizeof(int));
        if (buf.data) {
            buf.size = size;
            buf.initialized = 1;
            printf("Buffer allocated: %zu bytes\n", size * sizeof(int));
        }
    }
    return buf.data;
}

int main(void) {
    // 第一次调用时分配
    int* buf1 = get_large_buffer(1000);

    // 后续调用直接返回已分配的缓冲区
    int* buf2 = get_large_buffer(1000);

    printf("buf1 == buf2: %s\n", buf1 == buf2 ? "true" : "false");

    free(get_large_buffer(1));  // 释放内存
    return 0;
}
```

**输出**：
```
Buffer allocated: 4000 bytes
buf1 == buf2: true
```

### 高级用法：状态机实现

```c
#include <stdio.h>

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED
} State;

State get_next_state(State current, int input) {
    static const State transition[4][2] = {
        /* input: 0    1  */
        /* IDLE   */ {STATE_IDLE, STATE_RUNNING},
        /* RUNNING*/ {STATE_PAUSED, STATE_STOPPED},
        /* PAUSED */ {STATE_RUNNING, STATE_STOPPED},
        /* STOPPED*/ {STATE_IDLE, STATE_IDLE}
    };
    return transition[current][input];
}

void run_state_machine(void) {
    static State current = STATE_IDLE;
    static const char* state_names[] = {
        "IDLE", "RUNNING", "PAUSED", "STOPPED"
    };

    printf("Current: %s\n", state_names[current]);
}

int main(void) {
    int inputs[] = {1, 0, 1, 1, 0};
    State state = STATE_IDLE;

    for (int i = 0; i < 5; i++) {
        state = get_next_state(state, inputs[i]);
        const char* names[] = {"IDLE", "RUNNING", "PAUSED", "STOPPED"};
        printf("Input %d -> State: %s\n", inputs[i], names[state]);
    }
    return 0;
}
```

### 常见错误：静态变量与递归

```c
// 错误示例：递归中使用静态变量
int factorial_bad(int n) {
    static int depth = 0;
    depth++;  // 每次递归都会修改

    if (n <= 1) {
        printf("Max depth: %d\n", depth);
        return 1;
    }
    return n * factorial_bad(n - 1);
}

// 正确做法：使用参数传递
int factorial_good(int n, int depth) {
    if (n <= 1) {
        printf("Max depth: %d\n", depth);
        return 1;
    }
    return n * factorial_good(n - 1, depth + 1);
}

int main(void) {
    printf("Factorial 5: %d\n", factorial_bad(5));
    // 第二次调用时 depth 不会重置
    printf("Factorial 3: %d\n", factorial_bad(3));

    printf("Factorial 3 (correct): %d\n", factorial_good(3, 1));
    printf("Factorial 5 (correct): %d\n", factorial_good(5, 1));
    return 0;
}
```

### 常见错误：返回静态缓冲区指针

```c
#include <stdio.h>
#include <string.h>

// 危险：返回静态缓冲区指针
char* format_data(int year, int month, int day) {
    static char buffer[20];
    sprintf(buffer, "%04d-%02d-%02d", year, month, day);
    return buffer;  // 多次调用会覆盖之前的结果
}

int main(void) {
    char* date1 = format_data(2024, 3, 15);
    char* date2 = format_data(2024, 12, 25);

    printf("Date1: %s\n", date1);  // 输出被覆盖
    printf("Date2: %s\n", date2);

    // 正确做法：调用者提供缓冲区
    char buf1[20], buf2[20];
    sprintf(buf1, "%04d-%02d-%02d", 2024, 3, 15);
    sprintf(buf2, "%04d-%02d-%02d", 2024, 12, 25);
    printf("Buf1: %s\n", buf1);
    printf("Buf2: %s\n", buf2);

    return 0;
}
```

**输出**：
```
Date1: 2024-12-25
Date2: 2024-12-25
Buf1: 2024-03-15
Buf2: 2024-12-25
```

## 7. 总结 (Summary)

### 核心要点

1. **定义识别**：没有 `_Thread_local` 且有外部/内部链接，或使用 `static` 说明符的对象具有静态存储期

2. **生命周期**：从程序启动到程序终止，跨越整个程序执行期

3. **初始化特点**：
   - 只初始化一次（程序启动前或首次使用前）
   - 未显式初始化自动零初始化
   - 支持常量初始化和动态初始化

4. **内存位置**：
   - 已初始化：`.data` 段
   - 未初始化：`.bss` 段

### 技术对比

| 特性 | 静态存储期 | 自动存储期 | 动态存储期 | 线程存储期 |
|------|-----------|-----------|-----------|-----------|
| **生命周期** | 程序全程 | 块作用域内 | 手动管理 | 线程全程 |
| **初始化** | 启动前一次 | 每次进入块 | 手动 | 线程启动时 |
| **默认值** | 零 | 未定义 | 未定义 | 零 |
| **存储位置** | 静态区 | 栈 | 堆 | 线程本地存储 |
| **关键字** | `static`/`extern` | `auto`/无 | `malloc`/`free` | `_Thread_local` |

### 学习建议

#### 初学者

1. 理解静态存储期与自动存储期的本质区别
2. 掌握 `static` 关键字的两种用法（存储期和链接属性）
3. 练习使用静态局部变量实现函数调用计数

#### 进阶学习

1. 深入理解初始化顺序问题（静态初始化 vs 动态初始化）
2. 学习内存布局（.data、.bss 段的作用）
3. 理解跨编译单元初始化顺序的不确定性

#### 高级应用

1. 掌握线程安全初始化（C11）
2. 学习可重入函数设计
3. 理解延迟初始化和单例模式在 C 语言中的实现

### 参考资料

- ISO/IEC 9899:2018 (C17) - 6.2.4 Storage durations of objects
- ISO/IEC 9899:2011 (C11) - 6.2.4 Storage durations of objects
- cppreference.com - Static storage duration