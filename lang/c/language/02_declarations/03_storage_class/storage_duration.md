# 存储类说明符 (Storage-Class Specifiers)

## 1. 概述 (Overview)

存储类说明符（storage-class specifiers）用于指定对象和函数的**存储期（storage duration）**与**链接（linkage）**属性。C 语言提供以下存储类说明符：

| 说明符 | 存储期 | 链接 | 说明 |
|--------|--------|------|------|
| `auto` | 自动存储期 | 无链接 | 块作用域对象的默认存储类 |
| `register` | 自动存储期 | 无链接 | 提示编译器将变量存储在寄存器中 |
| `static` | 静态存储期 | 内部链接（文件作用域） | 静态存储期，限制作用域 |
| `extern` | 静态存储期 | 外部链接 | 声明外部定义的变量或函数 |
| `_Thread_local` / `thread_local` (C11起) | 线程存储期 | 需配合 static/extern | 每个线程独立的存储 |

存储类说明符出现在声明中，最多只能使用一个说明符（`_Thread_local`/`thread_local` 可与 `static` 或 `extern` 组合使用以调整链接属性）。

## 2. 来源与演变 (Origin and Evolution)

### C89/C90 标准

存储类说明符是 C 语言最早的核心特性之一，在 C89/C90 标准中就已定义：
- `auto` - 用于声明自动变量
- `register` - 用于寄存器优化提示
- `static` - 用于静态存储和限制作用域
- `extern` - 用于外部链接声明

### C99 标准

C99 标准对变长数组（VLA）的存储期做了特殊规定：
- VLA 的存储在声明执行时分配，而非块入口
- VLA 的存储在声明作用域结束时释放，而非块出口

### C11 标准

C11 标准引入了线程存储期支持：
- 新增 `_Thread_local` 关键字，用于多线程编程
- 定义了线程存储期，每个线程拥有独立的对象实例
- 可通过 `<threads.h>` 中的 `thread_local` 宏使用

### C23 标准

C23 标准对存储类说明符进行了重要更新：
- `thread_local` 正式成为关键字（替代 `_Thread_local`）
- `auto` 关键字重新定义，用于类型推断（type inference）
- `constexpr` 说明符被正式列为存储类说明符（但不指定存储）

### C 与 C++ 的差异

文件作用域的 `const` 变量（非 `extern`）在 C 中具有外部链接，而在 C++ 中具有内部链接。这是 C 和 C++ 的重要不兼容点之一。

## 3. 语法与参数 (Syntax and Parameters)

### 存储类说明符语法

```c
// 基本语法
storage-class-specifier declaration;

// 组合语法（C11起）
_Thread_local static declaration;  // 线程存储期 + 内部链接
_Thread_local extern declaration;  // 线程存储期 + 外部链接
thread_local static declaration;   // C23起
```

### 各说明符详细说明

#### `auto` 说明符

```c
{
    auto int x = 10;      // 显式声明（C23之前）
    int y = 20;           // 隐式自动存储期
}
```

**限制条件：**
- 仅允许在块作用域中使用（函数参数列表除外）
- 不能与 `register` 或 `static` 同时使用
- C23 起用于类型推断，语义发生变化

#### `register` 说明符

```c
void func(register int param) {
    register int x = 10;
    // &x = ...;          // 错误：不能取 register 变量的地址
    // alignas(8) register int y;  // C11起错误：不能使用对齐说明符
}
```

**限制条件：**
- 仅允许在块作用域中使用（包括函数参数）
- 不能对该变量使用取地址运算符（`&`）
- C11 起不能使用 `_Alignas`/`alignas`
- register 数组不能转换为指针

#### `static` 说明符

```c
// 文件作用域：内部链接
static int global_var;        // 仅本翻译单元可见
static void helper_func(void); // 仅本翻译单元可见

// 块作用域：静态存储期
void counter(void) {
    static int count = 0;     // 值在函数调用间保持
    count++;
}
```

**限制条件：**
- 不能在函数参数列表中使用
- 可用于函数（仅文件作用域）和变量（文件或块作用域）

#### `extern` 说明符

```c
// 文件作用域声明
extern int external_var;       // 引用其他文件定义的变量
extern void external_func(void); // 引用其他文件定义的函数

// 块作用域声明
void func(void) {
    extern int shared_var;     // 引用全局变量
}
```

**特殊规则：**
- 若先前声明具有内部链接，则 `extern` 保持内部链接
- 否则（先前声明为外部、无链接或不可见），链接为外部

#### `_Thread_local` / `thread_local` 说明符（C11起）

```c
// 文件作用域
thread_local int tls_var;           // 错误：必须指定链接

// 正确用法
static thread_local int tls_var;    // 内部链接 + 线程存储期
extern thread_local int tls_var;    // 外部链接 + 线程存储期

// 块作用域
void func(void) {
    static thread_local int tls_local; // 正确：线程存储期
}
```

**限制条件：**
- 不能用于函数声明
- 必须出现在对象的每个声明中
- 块作用域必须与 `static` 或 `extern` 组合使用

### 默认规则

当未指定存储类说明符时，适用以下默认规则：

| 声明位置 | 默认存储类 |
|----------|-----------|
| 所有函数 | `extern` |
| 文件作用域对象 | `extern` |
| 块作用域对象 | `auto` |

## 4. 底层原理 (Underlying Principles)

### 存储期类型

每个对象都有一个称为**存储期（storage duration）**的属性，它限制对象的生命周期。C 语言定义了四种存储期：

#### 自动存储期（Automatic Storage Duration）

```c
void func(void) {
    int x = 10;           // 块入口分配
    int arr[n];           // VLA：声明执行时分配（C99）
}                         // 块出口释放
```

**特点：**
- 存储在声明它的块被**进入时分配**，退出时**释放**（通过任何方式：goto、return、到达末尾）
- VLA 例外：存储在声明执行时分配，声明作用域结束时释放
- 如果递归进入块，每个递归层级执行新的分配
- 函数参数、非 static 块作用域对象、块作用域复合字面量（C23前）具有此存储期

#### 静态存储期（Static Storage Duration）

```c
static int global = 10;    // 程序启动前初始化
int external_var;          // 同上

int main(void) {
    // 整个程序执行期间存在
}
```

**特点：**
- 存储期为整个程序的执行期间
- 存储的值在 `main` 函数之前初始化一次
- 所有 `static` 声明的对象、具有内部或外部链接的非 `_Thread_local` 对象具有此存储期

#### 线程存储期（Thread Storage Duration）（C11起）

```c
static thread_local int tls_data = 0;

void* thread_func(void* arg) {
    tls_data = 42;         // 每个线程独立的值
    return NULL;
}
```

**特点：**
- 存储期为创建它的线程的整个执行期间
- 存储的值在线程启动时初始化
- 每个线程拥有独立、不同的对象实例
- 如果访问此对象的线程不是执行初始化的线程，行为由实现定义
- 所有 `_Thread_local`/`thread_local` 声明的对象具有此存储期

#### 分配存储期（Allocated Storage Duration）

```c
int* ptr = malloc(sizeof(int));  // 手动分配
free(ptr);                       // 手动释放
```

**特点：**
- 存储通过动态内存分配函数按请求分配和释放
- 生命周期完全由程序员控制
- 由 `malloc`、`calloc`、`realloc` 分配，`free` 释放

### 链接类型

链接（linkage）指标识符（变量或函数）在其他作用域中被引用的能力。

#### 无链接（No Linkage）

```c
void func(int param) {       // 函数参数：无链接
    int local = 10;          // 块作用域非 extern：无链接
}
```

**特点：**
- 变量或函数只能在其所在的作用域中被引用
- 块作用域非 `extern` 变量、函数参数、非函数非变量的标识符具有此链接

#### 内部链接（Internal Linkage）

```c
static int file_local = 10;     // 文件作用域 static
static void helper(void) {}      // 文件作用域 static 函数
constexpr int CONST = 5;         // C23：文件作用域 constexpr
```

**特点：**
- 变量或函数可从当前翻译单元的所有作用域中引用
- 文件作用域 `static` 变量、文件作用域 `static` 函数、文件作用域 `constexpr` 变量（C23起）具有此链接

#### 外部链接（External Linkage）

```c
int global_var;                // 文件作用域非 static
void public_func(void) {}      // 文件作用域非 static 函数

void func(void) {
    extern int ext_var;        // 块作用域 extern 声明
}
```

**特点：**
- 变量或函数可从整个程序的任何其他翻译单元引用
- 文件作用域非 `static`/`constexpr` 变量、文件作用域非 `static` 函数、块作用域函数声明、`extern` 声明（除非先前有内部链接声明可见）具有此链接

### 链接冲突

如果同一标识符在同一翻译单元中同时出现内部和外部链接，行为是未定义的。这可能在使用试探性定义（tentative definitions）时发生。

### 存储期与链接对照表

| 存储类说明符 | 作用域 | 存储期 | 链接 |
|-------------|--------|--------|------|
| `auto` | 块 | 自动 | 无 |
| `register` | 块/参数 | 自动 | 无 |
| `static` | 文件 | 静态 | 内部 |
| `static` | 块 | 静态 | 无 |
| `extern` | 文件 | 静态 | 外部 |
| `extern` | 块 | 静态 | 外部 |
| `thread_local static` | 文件/块 | 线程 | 内部 |
| `thread_local extern` | 文件/块 | 线程 | 外部 |
| 无（文件作用域变量） | 文件 | 静态 | 外部 |
| 无（块作用域变量） | 块 | 自动 | 无 |
| 无（函数） | 任意 | 静态 | 外部 |

## 5. 使用场景 (Use Cases)

### 适合使用各说明符的场景

| 说明符 | 适用场景 |
|--------|---------|
| `auto` | C23前几乎不显式使用；C23起用于类型推断 |
| `register` | 性能关键代码中提示编译器优化（现代编译器通常忽略） |
| `static` | 限制全局变量/函数的作用域；保持函数内状态 |
| `extern` | 跨文件共享变量/函数；访问库中的全局变量 |
| `thread_local` | 多线程编程中需要线程独立状态的场景 |

### 库接口设计示例

**库接口头文件 "flib.h"：**

```c
#ifndef FLIB_H
#define FLIB_H

void f(void);                  // 函数声明：外部链接
extern int state;              // 变量声明：外部链接
static const int size = 5;     // 只读变量定义：内部链接
enum { MAX = 10 };             // 常量定义

inline int sum(int a, int b) { // 内联函数定义
    return a + b;
}

#endif // FLIB_H
```

**库实现文件 "flib.c"：**

```c
#include "flib.h"

static void local_f(int s) {}  // 内部链接（仅本文件使用）
static int local_state;        // 内部链接（仅本文件使用）

int state;                     // 外部链接定义（供 main.c 使用）
void f(void) { local_f(state); } // 外部链接定义
```

**应用代码 "main.c"：**

```c
#include "flib.h"

int main(void) {
    int x[MAX] = {size};       // 使用常量和只读变量
    state = 7;                 // 修改 flib.c 中的 state
    f();                       // 调用 flib.c 中的 f()
}
```

### 最佳实践

1. **限制全局变量作用域**：使用 `static` 将全局变量限制在单个翻译单元内，避免命名冲突

2. **避免过度使用 `register`**：现代编译器的优化能力强，通常会自动选择最佳存储位置

3. **正确使用 `extern`**：在头文件中声明，在源文件中定义，避免头文件中的定义导致重复符号

4. **线程安全**：使用 `thread_local` 替代全局变量实现线程安全

5. **初始化静态变量**：静态变量只初始化一次，适合用于计数器、缓存等

### 常见陷阱

#### 陷阱 1：头文件中的静态变量

```c
// 错误示例：头文件中定义 static 变量
// header.h
static int counter = 0;  // 每个包含此头文件的 .c 文件都有独立的副本！

// 正确做法：在源文件中定义，头文件中声明
// header.h
extern int counter;

// source.c
#include "header.h"
int counter = 0;
```

#### 陷阱 2：忘记初始化静态变量

```c
void func(void) {
    static int count;  // 自动初始化为 0（静态存储期）
    // 但显式初始化更清晰
}

// 推荐
void func(void) {
    static int count = 0;  // 显式初始化，意图明确
}
```

#### 陷阱 3：返回局部变量指针

```c
// 错误示例
int* bad_func(void) {
    int local = 10;
    return &local;  // 返回自动存储期变量的地址，未定义行为！
}

// 正确做法 1：返回静态变量
int* good_func1(void) {
    static int result = 10;
    return &result;  // 静态存储期，有效
}

// 正确做法 2：动态分配
int* good_func2(void) {
    int* result = malloc(sizeof(int));
    *result = 10;
    return result;  // 调用者负责释放
}
```

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>
#include <stdlib.h>

// 静态存储期（文件作用域）
int A;

int main(void)
{
    printf("&A = %p\n", (void*)&A);

    // 自动存储期（块作用域）
    int A = 1;   // 隐藏全局 A
    printf("&A = %p\n", (void*)&A);

    // 分配存储期
    int* ptr_1 = malloc(sizeof(int));   // 开始分配存储期
    printf("address of int in allocated memory = %p\n", (void*)ptr_1);
    free(ptr_1);                        // 结束分配存储期
}
```

**可能的输出：**

```
&A = 0x600ae4
&A = 0x7ffefb064f5c
address of int in allocated memory = 0x1f28c30
```

### 静态变量计数器

```c
#include <stdio.h>

void counter(void) {
    static int count = 0;  // 静态存储期，值在调用间保持
    count++;
    printf("Called %d time(s)\n", count);
}

int main(void) {
    counter();  // Called 1 time(s)
    counter();  // Called 2 time(s)
    counter();  // Called 3 time(s)
    return 0;
}
```

### 线程局部存储（C11起）

```c
#include <stdio.h>
#include <threads.h>

static thread_local int tls_counter = 0;  // 每个线程独立

int thread_func(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 3; i++) {
        tls_counter++;
        printf("Thread %d: counter = %d\n", id, tls_counter);
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

    return 0;
}
```

**可能的输出：**

```
Thread 1: counter = 1
Thread 1: counter = 2
Thread 1: counter = 3
Thread 2: counter = 1
Thread 2: counter = 2
Thread 2: counter = 3
```

### 链接示例

```c
// file1.c
#include <stdio.h>

static int internal_var = 100;  // 内部链接，仅本文件可见
int external_var = 200;          // 外部链接，其他文件可访问

static void internal_func(void) {  // 内部链接函数
    printf("Internal: %d\n", internal_var);
}

void external_func(void) {  // 外部链接函数
    internal_func();
    printf("External: %d\n", external_var);
}

// file2.c
#include <stdio.h>

extern int external_var;       // 引用外部变量
extern void external_func(void); // 引用外部函数

// static int internal_var;    // 错误：无法访问 file1.c 的 internal_var

int main(void) {
    external_var = 300;
    external_func();
    return 0;
}
```

### 常见错误及修正

#### 错误 1：register 变量取地址

```c
// 错误
void bad_register(void) {
    register int x = 10;
    int* p = &x;  // 错误：不能取 register 变量的地址
}

// 修正：不使用 register 或不取地址
void good_register(void) {
    int x = 10;
    int* p = &x;  // 正确
}
```

#### 错误 2：块作用域 thread_local 缺少 static/extern

```c
// 错误（C11）
void bad_thread_local(void) {
    thread_local int x = 10;  // 错误：需要 static 或 extern
}

// 修正
void good_thread_local(void) {
    static thread_local int x = 10;  // 正确
}
```

#### 错误 3：静态变量与多线程

```c
// 危险：多线程环境下静态变量不安全
int unsafe_counter(void) {
    static int count = 0;
    return ++count;  // 数据竞争！
}

// 修正：使用线程局部存储
int safe_counter(void) {
    static thread_local int count = 0;
    return ++count;  // 每个线程独立计数
}
```

## 7. 总结 (Summary)

### 核心要点

存储类说明符是 C 语言中控制对象生命周期和可见性的核心机制：

| 概念 | 说明 |
|------|------|
| **存储期** | 决定对象的生命周期：自动、静态、线程、分配 |
| **链接** | 决定标识符的可见范围：无链接、内部链接、外部链接 |
| **作用域** | 决定标识符的有效区域：块、函数、文件 |

### 存储期快速参考

| 存储期 | 分配时机 | 释放时机 | 关键字 |
|--------|---------|---------|--------|
| 自动 | 块入口/声明执行 | 块出口/作用域结束 | `auto`, `register` |
| 静态 | 程序启动前 | 程序结束 | `static`, `extern` |
| 线程 | 线程启动时 | 线程结束 | `thread_local` (C11) |
| 分配 | 调用分配函数 | 调用释放函数 | `malloc`/`free` |

### 使用建议

1. **默认选择**：块作用域变量使用自动存储期，文件作用域变量谨慎使用
2. **限制可见性**：使用 `static` 限制全局变量和函数的作用域
3. **线程安全**：多线程环境下优先使用 `thread_local` 而非全局变量
4. **避免 register**：现代编译器能更好地进行寄存器分配优化
5. **正确使用 extern**：头文件声明，源文件定义

### 标准参考

| 标准 | 条款 |
|------|------|
| C23 | 6.2.2 链接, 6.2.4 存储期, 6.7.1 存储类说明符 |
| C17 | 6.2.2 链接, 6.2.4 存储期, 6.7.1 存储类说明符 |
| C11 | 6.2.2 链接, 6.2.4 存储期, 6.7.1 存储类说明符 |
| C99 | 6.2.2 链接, 6.2.4 存储期, 6.7.1 存储类说明符 |
| C89 | 3.1.2.2 链接, 3.1.2.4 存储期, 3.5.1 存储类说明符 |

### 参考资料

- cppreference: https://en.cppreference.com/w/c/language/storage_duration
- ISO/IEC 9899:2024 (C23 Standard)
- ISO/IEC 9899:2018 (C17 Standard)