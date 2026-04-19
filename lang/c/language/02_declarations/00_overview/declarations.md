# C 语言声明（Declarations）

## 1. 概述

**声明（declaration）** 是 C 语言的核心构造，用于向程序引入一个或多个标识符，并指定它们的含义和属性。声明可以出现在任何作用域中，每个声明都以分号结尾。

声明的核心功能：
- 引入标识符（变量、函数、类型名称等）
- 指定标识符的类型信息
- 分配存储空间（对于定义而言）
- 建立链接属性

C 语言声明由三个部分组成（C23 标准起）：
1. **说明符和限定符（specifiers-and-qualifiers）**：类型说明符、存储类说明符、类型限定符等
2. **声明符和初始化器（declarators-and-initializers）**：标识符名称及附加类型信息
3. **属性说明序列（attr-spec-seq）**：C23 新增，可选的属性列表

## 2. 来源与演变

### 历史背景

C 语言的声明语法设计遵循"声明模仿使用"（declaration mimics use）原则：当声明的标识符在表达式中以声明符相同的形式出现时，其类型就是类型说明符指定的类型。这一设计理念源自 BCPL 语言，由 Dennis Ritchie 在设计 C 语言时继承并发展。

### 各版本变化

| 版本 | 变化内容 |
|------|---------|
| **C89/C90** | 声明必须出现在块的开头，在任何语句之前；函数返回 int 可隐式声明 |
| **C99** | 允许声明与语句混合出现；取消隐式函数声明；引入变长数组（VLA）相关声明规则 |
| **C11** | 新增 `_Alignas` 对齐说明符；`_Static_assert` 作为特殊声明；新增 `_Thread_local` 存储类说明符 |
| **C23** | 引入属性说明序列；新增空属性声明；支持 `auto` 类型推断；`alignas` 替代 `_Alignas`；`constexpr` 存储类说明符 |

### 设计动机

C 语言声明语法的复杂性源于其声明符的递归组合能力：
- 单一语法支持变量、指针、数组、函数的声明
- 声明符可以嵌套组合，表达复杂类型
- 保持与表达式使用形式的一致性

## 3. 语法与参数

### 声明语法结构

#### 基本形式

```
// C23 之前
specifiers-and-qualifiers declarators-and-initializers(可选) ;

// C23 起
attr-spec-seq(可选) specifiers-and-qualifiers declarators-and-initializers ;

// C23 空属性声明
attr-spec-seq ;
```

#### 说明符与限定符

说明符和限定符由以下元素组成（顺序不限，以空白分隔）：

| 类别 | 说明符/限定符 | 说明 |
|------|-------------|------|
| **类型说明符** | `void` | 空类型 |
| | 算术类型名 | `int`, `char`, `float`, `double` 等 |
| | 原子类型名 | `_Atomic` 类型 |
| | typedef 名称 | 之前定义的类型别名 |
| | 结构/联合/枚举说明符 | `struct`, `union`, `enum` |
| | typeof 说明符 | C23 新增 |
| **存储类说明符**（最多一个） | `typedef` | 类型别名定义 |
| | `constexpr` | 常量表达式（C23） |
| | `auto` | 自动存储期（C23 起用于类型推断） |
| | `register` | 寄存器存储 |
| | `static` | 静态存储期/内部链接 |
| | `extern` | 外部链接 |
| | `_Thread_local` | 线程存储期（C11） |
| **类型限定符**（可多个） | `const` | 只读 |
| | `volatile` | 易变 |
| | `restrict` | 指针别名优化 |
| | `_Atomic` | 原子访问（C11） |
| **函数说明符**（仅函数声明） | `inline` | 内联建议 |
| | `_Noreturn` | 不返回（C11） |
| **对齐说明符** | `_Alignas` / `alignas` | 对齐要求（C11/C23） |

### 声明符语法

声明符定义标识符名称及其附加类型信息：

| 形式 | 语法 | 说明 |
|------|------|------|
| **标识符声明符** | `identifier attr-spec-seq(可选)` | 直接声明标识符 |
| **括号声明符** | `( declarator )` | 用于改变结合顺序，声明指向数组或函数的指针 |
| **指针声明符** | `* attr-spec-seq(可选) qualifiers(可选) declarator` | 声明指针类型 |
| **数组声明符** | `noptr-declarator [ static(可选) qualifiers(可选) expression ]` | 声明数组类型 |
| | `noptr-declarator [ qualifiers(可选) * ]` | 变长数组（VLA） |
| **函数声明符** | `noptr-declarator ( parameters-or-identifiers )` | 声明函数类型 |

### 定义与声明

**定义（definition）** 是提供完整信息的声明：

| 实体 | 定义条件 |
|------|---------|
| **枚举/typedef** | 每个声明都是定义 |
| **函数** | 包含函数体的声明 |
| **对象** | 分配存储的声明（非 `extern` 声明） |
| **结构/联合** | 指定成员列表的声明 |

```c
extern int n;        // 声明（非定义）
int n = 10;          // 定义

int foo(double);     // 声明（函数原型）
int foo(double x) {  // 定义（函数体）
    return x;
}

struct X;            // 声明（不完整类型）
struct X { int n; }; // 定义（完整类型）
```

## 4. 底层原理

### 声明符解析机制

C 语言声明遵循"由内向外"的解析规则：

1. 从标识符开始
2. 向右处理数组 `[]` 和函数 `()` 说明符
3. 向左处理指针 `*` 说明符
4. 遇到括号时，先处理括号内的声明符

```
int (*(*foo)(double))[3] = NULL;
```

解析过程：
1. `foo` - 标识符
2. `(*foo)` - 指针，指向...
3. `(*foo)(double)` - 函数，参数 double，返回...
4. `*(*foo)(double)` - 指针，指向...
5. `(*(*foo)(double))[3]` - 数组，3 个元素，类型...
6. `int` - 整型

**结论**：`foo` 是指向"接受 double 参数并返回指向 3 元素 int 数组指针的函数"的指针。

### 类型组合

最终类型 = 类型说明符指定的类型 + 声明符修改的类型

```c
int *p[3];     // p 是"3 个元素的数组"，元素类型是"指向 int 的指针"
int (*p)[3];   // p 是"指向数组的指针"，数组是"3 个 int 元素的数组"
```

### 变长修改类型

C99 起引入变长修改类型（variably-modified type, VM）：
- 包含变长数组（VLA）的声明符
- 从 VM 类型派生的类型也是 VM 类型

VM 类型的限制：
- 只能出现在块作用域或函数原型作用域
- 不能作为结构体或联合体的成员
- 不能具有静态存储期（但指向 VLA 的指针可以是静态的）

## 5. 使用场景

### 常见声明类型

| 声明类型 | 示例 | 使用场景 |
|---------|------|---------|
| 简单变量 | `int a;` | 基本类型变量 |
| 指针变量 | `int *p;` | 动态内存、间接访问 |
| 数组 | `int arr[10];` | 固定大小数据集合 |
| 函数原型 | `int f(int, double);` | 函数接口声明 |
| 函数指针 | `int (*pf)(int);` | 回调函数、策略模式 |
| 指针数组 | `int *arr[10];` | 字符串数组、对象表 |
| 数组指针 | `int (*p)[10];` | 二维数组操作 |
| typedef | `typedef int INT32;` | 类型别名、跨平台 |
| 结构体 | `struct Point { int x; int y; };` | 复合数据类型 |

### 重声明规则

允许的重声明：
1. **有链接的对象**：`extern` 和 `static` 声明可重复
2. **typedef**：只要命名相同类型，可重复
3. **结构体/联合体**：可在定义前声明不完整类型

```c
extern int x;
int x = 10;        // OK，定义
extern int x;      // OK，重声明

typedef int int_t;
typedef int int_t; // OK，相同类型

struct X;          // 声明不完整类型
struct X { int n; };// 定义
struct X;          // OK，重声明
```

### 注意事项

#### C89 限制

在 C89 中，块作用域内的声明必须出现在所有语句之前：

```c
/* C89 */
int main(void) {
    int a = 1;      /* OK */
    a = a + 1;      /* 语句 */
    int b = 2;      /* C89: 错误！声明必须在块开头 */
    return 0;
}
```

#### 空声明符禁止

```c
int ;              // 错误：缺少声明符
struct { int n; }; // OK：定义结构体标签
```

#### static_assert 特殊性

`_Static_assert` 在语法上被视为声明，但不遵循普通声明语法：

```c
_Static_assert(sizeof(int) == 4, "int must be 4 bytes");
```

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>

/* 变量声明 */
int global_var;           /* 外部链接，静态存储期 */
static int file_var;      /* 内部链接，静态存储期 */

/* 函数声明 */
int add(int a, int b);    /* 函数原型 */

int main(void) {
    /* 块作用域声明 */
    int a = 10;           /* 自动存储期 */
    const int b = 20;     /* 只读变量 */
    static int count = 0; /* 静态存储期，局部可见 */

    /* 指针声明 */
    int *p = &a;          /* 指向 int 的指针 */
    const int *cp = &b;   /* 指向 const int 的指针 */

    /* 数组声明 */
    int arr[5] = {1, 2, 3, 4, 5};

    printf("a = %d, *p = %d, arr[0] = %d\n", a, *p, arr[0]);

    return 0;
}

/* 函数定义 */
int add(int a, int b) {
    return a + b;
}
```

### 高级用法

```c
#include <stdio.h>
#include <stdlib.h>

/* typedef 声明 */
typedef int INT32;
typedef int (*CompareFunc)(const void*, const void*);

/* 结构体声明与定义 */
struct Point {
    int x;
    int y;
};

typedef struct Point Point;  /* 类型别名 */

/* 函数指针的高级应用 */
int ascending(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

/* 复杂声明：指向函数的指针数组 */
int add_op(int a, int b) { return a + b; }
int sub_op(int a, int b) { return a - b; }
int mul_op(int a, int b) { return a * b; }

int main(void) {
    /* 复杂声明解析 */
    int (*operations[3])(int, int) = {add_op, sub_op, mul_op};

    /* 指向数组的指针 */
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int (*row_ptr)[3] = matrix;

    /* 函数指针 */
    CompareFunc cmp = ascending;
    int arr[] = {3, 1, 4, 1, 5, 9};
    qsort(arr, 6, sizeof(int), cmp);

    /* 使用函数指针数组 */
    printf("add: %d\n", operations[0](5, 3));   /* 8 */
    printf("sub: %d\n", operations[1](5, 3));   /* 2 */
    printf("mul: %d\n", operations[2](5, 3));  /* 15 */

    return 0;
}
```

### 常见错误及修正

#### 错误 1：声明符优先级混淆

```c
/* ❌ 错误：误认为 p 是指向数组的指针 */
int *p[10];    /* 实际：p 是指针数组，10 个 int* 元素 */

/* ✅ 修正：正确声明指向数组的指针 */
int (*p)[10];  /* p 是指向 10 元素 int 数组的指针 */
```

#### 错误 2：const 限定符位置

```c
/* ❌ 常见混淆 */
const int *p;      /* 指向 const int 的指针，可改指针，不可改内容 */
int *const p;      /* const 指针，指向 int，不可改指针，可改内容 */
const int *const p; /* const 指针，指向 const int，都不可改 */

/* ✅ 推荐：使用 typedef 简化复杂声明 */
typedef const int *ConstIntPtr;
ConstIntPtr p;     /* 等价于 const int *p */
```

#### 错误 3：extern 声明不分配存储

```c
/* ❌ 错误：头文件中定义 */
/* header.h */
int global_var = 0;  /* 定义，多次包含导致重复定义 */

/* ✅ 修正：头文件中声明，源文件中定义 */
/* header.h */
extern int global_var;  /* 仅声明 */

/* source.c */
#include "header.h"
int global_var = 0;      /* 定义 */
```

#### 错误 4：C89 声明位置

```c
/* ❌ C89 中错误：声明在语句之后 */
void func(void) {
    int a = 1;
    printf("%d\n", a);
    int b = 2;      /* C89: 错误！ */
}

/* ✅ 修正：声明放在块开头（兼容 C89） */
void func(void) {
    int a = 1;
    int b = 2;      /* OK */
    printf("%d %d\n", a, b);
}
```

#### 错误 5：函数指针声明遗漏括号

```c
/* ❌ 错误：误认为返回指针的函数 */
int *f(int);       /* 函数，参数 int，返回 int* */

/* ✅ 正确：函数指针 */
int (*f)(int);     /* 指向函数的指针，参数 int，返回 int */

/* 函数指针数组 */
int (*f[5])(int);  /* 5 个函数指针的数组 */
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| **声明组成** | 说明符/限定符 + 声明符 + 初始化器（可选） |
| **解析原则** | 从标识符开始，先右后左，括号优先 |
| **定义 vs 声明** | 定义分配存储，声明仅引入标识符 |
| **声明符类型** | 标识符、指针、数组、函数四种基本形式 |
| **重声明** | 有链接对象、typedef、结构体标签可重声明 |

### 声明符优先级规则

1. 括号 `()` 内的声明符优先级最高
2. 数组 `[]` 和函数 `()` 结合律从左到右
3. 指针 `*` 结合律从右到左

### 学习建议

1. **从简单到复杂**：先掌握基本变量、指针、数组声明
2. **使用 typedef**：简化复杂声明，提高可读性
3. **cdecl 工具**：使用 `cdecl` 命令行工具辅助理解复杂声明
4. **声明模仿使用**：理解"声明形式模仿使用形式"的设计原则

### 相关概念

| 概念 | 关系 |
|------|------|
| **作用域** | 声明可见性的范围 |
| **链接** | 标识符在不同翻译单元中的可见性 |
| **存储期** | 对象的生命周期 |
| **类型限定符** | 修改类型的属性 |
| **初始化** | 声明时提供初始值 |

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.7 Declarations
- C17 标准 (ISO/IEC 9899:2018): 6.7 Declarations (p. 78-105)
- C11 标准 (ISO/IEC 9899:2011): 6.7 Declarations (p. 108-145)
- C99 标准 (ISO/IEC 9899:1999): 6.7 Declarations (p. 97-130)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.5 Declarations