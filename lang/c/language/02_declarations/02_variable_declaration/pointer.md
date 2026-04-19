# 指针声明 (Pointer Declaration)

## 1. 概述

指针（Pointer）是 C 语言中最核心的概念之一。指针是一种对象类型，它指向另一个函数或对象，并可以添加类型限定符（qualifier）。指针还可以指向"空"，这种特殊值称为**空指针值**（null pointer value）。

指针的本质是存储内存地址的变量，通过指针可以实现间接访问（indirection），这是 C 语言强大和灵活的根源。

## 2. 来源与演变

### 历史背景

指针的概念最早出现在汇编语言中，用于直接操作内存地址。C 语言将这一概念抽象化，提供了类型安全的指针操作机制。

### 设计动机

指针的引入解决了以下核心问题：

1. **实现引用传递语义**：C 语言默认使用值传递，指针允许函数修改调用者的变量
2. **动态内存管理**：访问动态分配的内存对象
3. **表示"可选"类型**：使用空指针表示"无值"状态
4. **实现回调机制**：通过函数指针实现回调函数
5. **泛型接口**：使用 `void*` 实现泛型编程

### C 标准演进

| 标准版本 | 条款编号 | 变化 |
|---------|---------|------|
| C89/C90 | 3.5.4.1 | 首次标准化指针声明语法 |
| C99 | 6.7.5.1 | 新增 `restrict` 关键字用于指针别名优化 |
| C11 | 6.7.6.1 | 保持不变 |
| C17 | 6.7.6.1 | 保持不变 |
| C23 | 6.7.6.1 | 新增 `attr-spec-seq` 属性说明符序列 |

## 3. 语法与声明

### 基本语法

在指针声明的声明语法中，类型说明符序列（type-specifier sequence）指定了被指向的类型（可以是函数类型、对象类型或不完整类型），声明符（declarator）的形式如下：

```
* attr-spec-seq(可选) 限定符(可选) 声明符
```

### 语法元素说明

| 元素 | 说明 |
|------|------|
| `*` | 指针声明符，表示这是一个指针类型 |
| `attr-spec-seq` | (C23) 可选的属性说明符序列，应用于被声明的指针 |
| `限定符` | 可选的类型限定符（`const`、`volatile`、`restrict`），修饰指针本身 |
| `声明符` | 命名指针的标识符，或嵌套的指针声明符（表示指针的指针） |

### 声明示例

```c
float *p, **pp;     // p 是指向 float 的指针
                    // pp 是指向 "指向 float 的指针" 的指针

int (*fp)(int);     // fp 是指向函数 int(int) 的指针
```

### 限定符的位置与含义

限定符出现在 `*` 和标识符之间时，修饰的是指针本身的类型：

```c
int n;
const int *pc = &n;    // pc 是指向 const int 的非 const 指针
                       // *pc = 2; // 错误：不能通过 pc 修改 n
                       // pc = NULL; // 正确：pc 本身可以修改

int *const cp = &n;    // cp 是指向非 const int 的 const 指针
                       // *cp = 2; // 正确：可以通过 cp 修改 n
                       // cp = NULL; // 错误：cp 本身不能修改

int *const *pcp = &cp; // 指向 "指向非 const int 的 const 指针" 的非 const 指针
```

### 指针类型分类

| 类型 | 语法示例 | 说明 |
|------|---------|------|
| 对象指针 | `int *p;` | 指向数据对象 |
| 函数指针 | `int (*fp)(int);` | 指向函数 |
| void 指针 | `void *p;` | 指向未知类型 |
| 空指针 | `int *p = NULL;` | 不指向任何对象 |

## 4. 底层原理

### 内存模型

指针在内存中存储的是目标对象的内存地址：

```
内存地址:    0x1000        0x2000
变量:        n = 42        p = 0x1000
                          ┌─────────┐
                p ────────│ 0x1000  │
                          └─────────┘
                               │
                               ▼
                          ┌─────────┐
                n ────────│   42    │
                          └─────────┘
```

### 指针运算原理

对于指向数组元素的指针，C 语言定义了以下运算：

1. **指针加减整数**：移动到数组中的其他元素
2. **指针相减**：计算两个指针之间的元素个数
3. **指针比较**：比较同一数组内元素的位置关系

```c
int a[5] = {10, 20, 30, 40, 50};
int *p = a;        // p 指向 a[0]

p++;               // p 现在指向 a[1]
int diff = p - a;  // diff = 1
```

### 指针与数组的关系

数组名在大多数上下文中会隐式转换为指向首元素的指针：

```c
int a[2];
int *p = a;          // 等价于 int *p = &a[0]

int b[3][3];
int (*row)[3] = b;   // row 指向 b[0]（即一个包含3个int的数组）
```

### 函数指针的实现

函数指针存储的是函数代码的入口地址。与函数不同，函数指针是对象，可以存储在数组中、作为参数传递、作为返回值等。

```c
void f(int);
void (*pf1)(int) = &f;   // 显式取地址
void (*pf2)(int) = f;    // 隐式转换，与 &f 等价
```

### void 指针的通用性

`void*` 可以接受任何对象指针类型的值，反之亦然：

```c
int n = 1, *p = &n;
void *pv = p;    // int* 到 void*（隐式转换）
int *p2 = pv;    // void* 到 int*（隐式转换）
```

## 5. 使用场景

### 对象指针

对象指针用于访问和操作数据对象：

```c
int n;
int *np = &n;                     // 指向 int 的指针
int *const *npp = &np;            // 指向 const 指针的非 const 指针

int a[2];
int (*ap)[2] = &a;                // 指向数组的指针

struct S { int n; } s = {1};
int *sp = &s.n;                   // 指向结构体成员的指针
```

通过解引用操作符（`*`）访问指针指向的对象：

```c
int n;
int *p = &n;    // 指针 p 指向 n
*p = 7;         // 将 7 存入 n
printf("%d\n", *p);  // 从 n 读取值
```

对于结构体和联合体指针，可使用 `->` 操作符访问成员：

```c
struct Person { char name[50]; int age; };
struct Person person = {"Alice", 30};
struct Person *pp = &person;
printf("Name: %s\n", pp->name);   // 等价于 (*pp).name
```

### 函数指针

函数指针用于实现回调和动态分发：

```c
#include <stdio.h>

int f(int n) {
    printf("%d\n", n);
    return n * n;
}

int main(void) {
    int (*p)(int) = f;    // 函数指针指向 f
    int x = p(7);         // 通过指针调用函数
    return 0;
}
```

函数指针解引用会得到函数指示符，可以直接调用：

```c
int f(void);
int (*p)(void) = f;    // p 指向 f
(*p)();                // 通过函数指示符调用
p();                    // 直接通过指针调用（更常见）
```

函数参数的顶层限定符不影响函数类型的兼容性：

```c
int f(int), fc(const int);
int (*pc)(const int) = f;    // 正确
int (*p)(int) = fc;          // 正确
pc = p;                       // 正确
```

### void 指针

`void*` 用于泛型接口，常见于标准库函数：

| 函数 | 用途 |
|------|------|
| `malloc()` | 返回 `void*`，通用内存分配 |
| `qsort()` | 接受 `const void*` 参数的比较函数 |
| `pthread_create()` | 接受和返回 `void*` 的线程函数 |

```c
int n = 1, *p = &n;
void *pv = p;             // int* 到 void*
int *p2 = pv;             // void* 到 int*
printf("%d\n", *p2);      // 输出 1
```

### 空指针

空指针表示"不指向任何对象或函数"。使用空指针的场景：

1. 初始化指针（表示尚未指向有效对象）
2. 表示"无值"或"可选"语义
3. 标记结束（如字符串的 `'\0'`、链表的 `NULL` 终止符）

```c
int *p = NULL;    // 初始化为空指针
if (p != NULL) {
    *p = 10;      // 安全使用
}

// 函数参数检查
void process(int *data) {
    if (data == NULL) {
        // 处理空指针情况
        return;
    }
    // 正常处理
}
```

### 最佳实践

1. **始终初始化指针**：未初始化的指针包含随机值，使用它是未定义行为
2. **检查空指针**：接收指针参数的函数应检查 NULL
3. **释放后置 NULL**：`free(p); p = NULL;` 防止悬空指针
4. **使用 const 保护数据**：`const int *p` 防止意外修改数据

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 悬空指针 | 指向已释放内存的指针 |
| 野指针 | 未初始化的指针 |
| 内存泄漏 | 丢失动态分配内存的指针 |
| 类型不匹配 | 指针类型与实际对象类型不一致 |

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>

int main(void) {
    int n = 42;
    int *p = &n;              // p 指向 n

    printf("n = %d\n", n);     // 直接访问
    printf("*p = %d\n", *p);   // 通过指针访问

    *p = 100;                  // 通过指针修改 n
    printf("n = %d\n", n);     // 输出 100

    return 0;
}
```

### 指针与数组

```c
#include <stdio.h>

int main(void) {
    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;              // 指向数组首元素

    // 使用指针遍历数组
    for (int i = 0; i < 5; i++) {
        printf("arr[%d] = %d, *(p+%d) = %d\n",
               i, arr[i], i, *(p + i));
    }

    // 指针运算
    p++;                       // p 现在指向 arr[1]
    printf("*p = %d\n", *p);   // 输出 20

    return 0;
}
```

### 函数指针

```c
#include <stdio.h>
#include <stdlib.h>

// 比较函数，用于 qsort
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// 回调函数示例
void process(int *arr, int size, void (*callback)(int)) {
    for (int i = 0; i < size; i++) {
        callback(arr[i]);
    }
}

void print_value(int n) {
    printf("%d ", n);
}

int main(void) {
    // 使用 qsort 排序
    int arr[] = {5, 2, 8, 1, 9};
    qsort(arr, 5, sizeof(int), compare);

    // 使用回调函数处理数组
    process(arr, 5, print_value);

    return 0;
}
```

### void 指针实现泛型

```c
#include <stdio.h>
#include <string.h>

// 通用的交换函数
void swap(void *a, void *b, size_t size) {
    char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

int main(void) {
    int x = 10, y = 20;
    printf("Before: x=%d, y=%d\n", x, y);
    swap(&x, &y, sizeof(int));
    printf("After: x=%d, y=%d\n", x, y);

    double a = 1.5, b = 2.5;
    swap(&a, &b, sizeof(double));
    printf("After: a=%.1f, b=%.1f\n", a, b);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：使用未初始化的指针

```c
// 错误：未初始化的指针
int *p;
*p = 10;              // 未定义行为：p 指向随机地址

// 修正：始终初始化指针
int n = 0;
int *p = &n;
*p = 10;              // 正确

// 或初始化为 NULL
int *p = NULL;
if (p != NULL) {
    *p = 10;
}
```

#### 错误 2：悬空指针

```c
// 错误：返回指向局部变量的指针
int *get_value(void) {
    int value = 42;
    return &value;     // 错误：value 在函数返回后失效
}

// 修正：使用静态变量或动态分配
int *get_value(void) {
    static int value = 42;
    return &value;     // 正确：静态变量生命周期持续整个程序
}

// 或
int *get_value(void) {
    int *p = malloc(sizeof(int));
    if (p != NULL) {
        *p = 42;
    }
    return p;          // 调用者负责释放
}
```

#### 错误 3：误解 const 限定符的位置

```c
int n = 10;

// 指向 const int 的指针
const int *p1 = &n;
// *p1 = 20;          // 错误：不能通过 p1 修改值
p1 = NULL;            // 正确：可以修改 p1 本身

// const 指针（指向非 const int）
int *const p2 = &n;
*p2 = 20;             // 正确：可以修改指向的值
// p2 = NULL;         // 错误：不能修改 p2 本身

// const 指针指向 const int
const int *const p3 = &n;
// *p3 = 30;          // 错误
// p3 = NULL;         // 错误
```

#### 错误 4：free 后继续使用指针

```c
// 错误：使用已释放的指针
int *p = malloc(sizeof(int));
*p = 42;
free(p);
*p = 100;             // 未定义行为：使用已释放的内存

// 修正：释放后置 NULL
int *p = malloc(sizeof(int));
*p = 42;
free(p);
p = NULL;             // 防止误用

if (p != NULL) {
    *p = 100;         // 不会执行
}
```

## 7. 总结

指针是 C 语言的核心特性，提供了强大的内存操作能力：

| 特性 | 说明 |
|------|------|
| 间接访问 | 通过内存地址访问数据 |
| 引用传递 | 实现函数修改调用者数据 |
| 动态内存 | 管理堆分配的对象 |
| 函数指针 | 实现回调和多态 |
| 泛型编程 | 通过 `void*` 处理未知类型 |

### 核心要点

1. **指针声明**：`*` 表示指针，限定符位置决定修饰的是指针本身还是指向的对象
2. **指针运算**：仅对数组指针有效，移动单位为元素大小
3. **空指针**：使用 `NULL` 初始化，表示"无有效指向"
4. **const 指针**：`const int *p` 与 `int *const p` 含义不同
5. **类型安全**：解引用时类型必须匹配，否则为未定义行为

### 类型对照表

| 声明 | 含义 |
|------|------|
| `int *p` | 指向 int 的指针 |
| `int **p` | 指向 "指向 int 的指针" 的指针 |
| `int *p[10]` | 包含 10 个 int 指针的数组 |
| `int (*p)[10]` | 指向包含 10 个 int 的数组的指针 |
| `int *p(void)` | 返回 int 指针的函数 |
| `int (*p)(void)` | 指向返回 int 的函数的指针 |

### 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.7.6.1 指针声明器
- C17 标准 (ISO/IEC 9899:2018): 6.7.6.1 指针声明器 (p. 93-94)
- C11 标准 (ISO/IEC 9899:2011): 6.7.6.1 指针声明器 (p. 130)
- C99 标准 (ISO/IEC 9899:1999): 6.7.5.1 指针声明器 (p. 115-116)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.5.4.1 指针声明器