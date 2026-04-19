# struct 声明 - 结构体

## 1. 概述

`struct`（结构体）是 C 语言中用于定义自定义数据类型的关键字。结构体由一系列成员组成，这些成员的存储空间按定义顺序依次分配（与联合体 union 不同，后者的成员存储空间是重叠的）。

结构体是 C 语言中组织复杂数据的核心机制，广泛用于：
- 将相关数据组合成逻辑单元
- 实现数据抽象和封装
- 构建链表、树等数据结构
- 与硬件寄存器映射

## 2. 来源与演变

### 历史背景

结构体概念源于 C 语言的早期设计。在 C 语言诞生之前，程序员需要使用多个独立变量来描述复杂实体，这种方式：
- 代码可读性差
- 数据管理困难
- 函数参数传递繁琐

结构体的引入解决了这些问题，提供了将相关数据组织为单一实体的能力。

### 版本演进

| 标准 | 新增特性 |
|------|----------|
| C89/C90 | 基本结构体定义、前向声明 |
| C99 | 灵活数组成员（Flexible Array Member） |
| C11 | 匿名结构体（Anonymous Structure） |
| C23 | 结构体类型属性说明符（attr-spec-seq） |

### 缺陷报告

DR 499（C11）：匿名结构体/联合体的成员应保留其内存布局，被视为外层结构体/联合体的成员。

## 3. 语法与声明

### 基本语法

```
struct attr-spec-seq(可选) name(可选) { struct-declaration-list }    (1)
struct attr-spec-seq(可选) name                                        (2)
```

| 形式 | 说明 |
|------|------|
| (1) | 结构体定义：引入新类型 `struct name` 并定义其含义 |
| (2) | 前向声明：声明但不定义结构体 `name`（单独使用时，如 `struct name;`） |

### 参数说明

| 参数 | 说明 |
|------|------|
| `name` | 被定义的结构体名称 |
| `struct-declaration-list` | 成员声明列表，可包含变量声明、位域声明、静态断言声明。不允许不完整类型的成员和函数类型的成员（灵活数组成员除外） |
| `attr-spec-seq` | (C23) 应用于结构体类型的可选属性列表 |

### 前向声明语法

```c
struct attr-spec-seq(可选) name ;
```

前向声明会隐藏标签命名空间中该名称之前的任何含义，并在当前作用域中声明 `name` 为新的结构体名称。直到定义出现前，该结构体名称具有不完整类型。

## 4. 底层原理

### 内存布局

结构体对象中，其元素（以及位域分配单元）的地址按成员定义顺序递增。

**内存布局示例：**

```
struct A {
    char a;      // 偏移量 0
    double b;    // 偏移量 8（对齐要求）
    char c;      // 偏移量 16
};               // 总大小 24（末尾填充）
```

### 内存对齐与填充

结构体可能在任意两个成员之间或最后一个成员之后存在未命名的填充字节，但在第一个成员之前不能有填充。

| 对齐规则 | 说明 |
|----------|------|
| 成员对齐 | 每个成员通常按其类型大小对齐 |
| 结构体对齐 | 结构体总大小是其最严格对齐成员大小的倍数 |
| 填充位置 | 成员之间和结构体末尾可能有填充 |

### 指针转换

结构体指针可以转换为其第一个成员的指针（反之亦然）：

```c
struct s { int x; float y; };
struct s obj;
int* p = (int*)&obj;  // 合法：指向第一个成员
```

### 灵活数组成员（C99 起）

如果结构体定义了至少一个命名成员，则允许其最后一个成员声明为不完整数组类型。

```c
struct s {
    int n;
    double d[];  // 灵活数组成员
};
```

当访问灵活数组成员元素时，结构体表现得如同该数组成员具有适合所分配内存的最大长度。

**灵活数组成员的限制：**
- 初始化和赋值运算符忽略灵活数组成员
- `sizeof` 运算符忽略灵活数组成员
- 含有灵活数组成员的结构体不能作为数组元素或其他结构体的成员

### 匿名结构体（C11 起）

无名的结构体成员称为匿名结构体，其成员被视为外层结构体或联合体的成员。

```c
struct v {
    union {              // 匿名联合体
        struct { int i, j; };  // 匿名结构体
        struct { long k, l; } w;
    };
    int m;
} v1;

v1.i = 2;      // 合法：访问匿名结构体成员
v1.w.k = 5;    // 合法：访问命名结构体成员
```

### 时间复杂度

| 操作 | 时间复杂度 |
|------|-----------|
| 成员访问 | O(1) |
| 结构体复制 | O(sizeof(struct)) |
| 指针转换 | O(1) |

## 5. 使用场景

### 适合使用结构体的场景

| 场景 | 说明 |
|------|------|
| 数据封装 | 将相关数据组合成单一实体 |
| 函数参数传递 | 将多个相关参数打包传递 |
| 数据结构实现 | 链表节点、树节点等 |
| 硬件映射 | 映射硬件寄存器布局 |
| 协议定义 | 网络协议头、文件格式 |

### 最佳实践

1. **成员顺序优化**：按对齐要求排列成员可减少填充
   ```c
   // 不推荐：可能产生更多填充
   struct Bad { char a; double b; char c; };  // 大小可能为 24

   // 推荐：减少填充
   struct Good { double b; char a; char c; };   // 大小可能为 16
   ```

2. **使用前向声明减少依赖**：
   ```c
   struct node;  // 前向声明
   struct edge { struct node* target; };
   struct node { struct edge* edges; };
   ```

3. **自引用结构体使用指针**：
   ```c
   struct Node {
       int data;
       struct Node* next;  // 指针类型，合法
   };
   ```

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 成员顺序影响大小 | 不同成员顺序可能导致不同结构体大小 |
| 前向声明限制 | 不完整类型不能用于定义对象，只能声明指针 |
| 灵活数组初始化 | 不能直接初始化灵活数组成员 |
| 匿名结构体可见性 | 嵌套类型在结构体定义后仍在外层作用域可见 |

## 6. 代码示例

### 基础用法

```c
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    // 声明结构体类型
    struct car
    {
        char* make;
        int year;
    };

    // 声明并初始化结构体对象
    struct car c = {.year = 1923, .make = "Nash"};
    printf("Car: %d %s\n", c.year, c.make);

    // 同时声明结构体类型、对象和指针
    struct spaceship
    {
        char* model;
        int max_speed;
    } ship = {"T-65 X-wing starfighter", 1050},
      *pship = &ship;

    printf("Spaceship: %s. Max speed: %d km/h\n",
           ship.model, ship.max_speed);

    return 0;
}
```

### 内存布局与偏移量

```c
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    // 成员地址按定义顺序递增，可能有填充
    struct A { char a; double b; char c; };
    printf(
        "Offset of char a = %zu\n"
        "Offset of double b = %zu\n"
        "Offset of char c = %zu\n"
        "Size of struct A = %zu\n",
        offsetof(struct A, a),
        offsetof(struct A, b),
        offsetof(struct A, c),
        sizeof(struct A)
    );

    // 优化成员顺序减少填充
    struct B { char a; char b; double c; };
    printf(
        "Offset of char a = %zu\n"
        "Offset of char b = %zu\n"
        "Offset of double c = %zu\n"
        "Size of struct B = %zu\n",
        offsetof(struct B, a),
        offsetof(struct B, b),
        offsetof(struct B, c),
        sizeof(struct B)
    );

    return 0;
}
```

**可能的输出：**
```
Offset of char a = 0
Offset of double b = 8
Offset of char c = 16
Size of struct A = 24

Offset of char a = 0
Offset of char b = 1
Offset of double c = 8
Size of struct B = 16
```

### 指针与首成员转换

```c
#include <stdio.h>

struct spaceship {
    char* model;
    int max_speed;
};

int main(void)
{
    struct spaceship ship = {"T-65 X-wing", 1050};

    // 结构体指针可转换为首成员指针
    char** pmodel = (char**)&ship;
    printf("%s\n", *pmodel);  // 输出: T-65 X-wing

    // 首成员指针可转换回结构体指针
    struct spaceship* pship = (struct spaceship*)pmodel;
    printf("Speed: %d\n", pship->max_speed);

    return 0;
}
```

### 前向声明与自引用

```c
// 前向声明允许互相引用的结构体
struct y;
struct x { struct y *p; };
struct y { struct x *q; };

// 自引用结构体实现链表
struct Node {
    int data;
    struct Node* next;  // 指针类型，合法
    // struct Node node; // 错误：不能包含自身类型的完整对象
};

int main(void)
{
    // 前向声明隐藏外层作用域的同名结构体
    struct s* p = NULL;     // 声明未知结构体
    struct s { int a; };     // 定义该结构体

    {
        struct s;           // 前向声明新的局部 struct s
        struct s* local_p;   // 指向局部 struct s
        struct s { char* p; }; // 定义局部 struct s
    }

    return 0;
}
```

### 灵活数组成员（C99）

```c
#include <stdlib.h>
#include <stdio.h>

struct s {
    int n;
    double d[];  // 灵活数组成员
};

int main(void)
{
    // 分配额外空间用于数组
    struct s* s1 = malloc(sizeof(struct s) + 8 * sizeof(double));
    s1->n = 8;
    for (int i = 0; i < 8; i++) {
        s1->d[i] = i * 1.0;
    }

    // sizeof 忽略灵活数组成员
    printf("sizeof(struct s) = %zu\n", sizeof(struct s));

    free(s1);
    return 0;
}
```

### 匿名结构体（C11）

```c
#include <stdio.h>

struct v {
    union {                  // 匿名联合体
        struct { int i, j; };  // 匿名结构体
        struct { long k, l; } w;  // 命名结构体
    };
    int m;
};

int main(void)
{
    struct v v1;
    v1.i = 2;        // 合法：访问匿名结构体成员
    v1.j = 3;        // 合法
    // v1.k = 3;     // 非法：内部结构体不是匿名的
    v1.w.k = 5;      // 合法：通过命名成员访问
    v1.m = 10;       // 合法

    printf("i=%d, j=%d, w.k=%ld, m=%d\n",
           v1.i, v1.j, v1.w.k, v1.m);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：成员顺序不当导致内存浪费

```c
// 错误：未优化的成员顺序
struct Bad {
    char a;     // 1 字节 + 7 字节填充
    double b;   // 8 字节
    char c;     // 1 字节 + 7 字节填充
};              // 总大小：24 字节

// 修正：优化成员顺序
struct Good {
    double b;   // 8 字节
    char a;      // 1 字节
    char c;      // 1 字节 + 6 字节填充
};              // 总大小：16 字节
```

#### 错误 2：灵活数组成员错误使用

```c
struct s { int n; double d[]; };

// 错误：不能直接初始化灵活数组成员
// struct s t = {1, {1.0, 2.0}};  // 编译错误

// 错误：结构体赋值不会复制灵活数组成员
struct s* src = malloc(sizeof(struct s) + sizeof(double) * 4);
struct s* dst = malloc(sizeof(struct s) + sizeof(double) * 4);
// *dst = *src;  // 只复制 n，不复制 d 数组！

// 正确：手动复制
dst->n = src->n;
for (int i = 0; i < src->n; i++) {
    dst->d[i] = src->d[i];
}
```

#### 错误 3：前向声明使用不当

```c
struct s;  // 前向声明

// 错误：不能定义不完整类型的对象
// struct s obj;  // 编译错误

// 正确：可以声明指向不完整类型的指针
struct s* ptr;  // 合法
```

## 7. 总结

结构体是 C 语言中最基本的数据组织机制，提供了以下核心能力：

- **数据封装**：将相关数据组织为单一实体
- **类型抽象**：创建自定义复合数据类型
- **内存映射**：精确控制数据在内存中的布局
- **数据结构构建**：通过指针实现链表、树等复杂数据结构

### 核心要点

| 特性 | 说明 |
|------|------|
| 内存顺序 | 成员按定义顺序存储，地址递增 |
| 内存填充 | 可能存在成员间和末尾填充 |
| 指针转换 | 结构体指针与首成员指针可互换 |
| 前向声明 | 允许声明不完整类型，用于互引用结构体 |

### 技术对比

| 特性 | struct | union | array |
|------|--------|-------|-------|
| 成员存储 | 顺序存储 | 重叠存储 | 连续同类型 |
| 成员类型 | 可不同 | 可不同 | 必须相同 |
| 成员数量 | 任意 | 任意 | 固定 |
| 大小 | 成员之和+填充 | 最大成员 | 元素大小×数量 |

### 学习建议

1. 理解内存对齐和填充机制，优化结构体布局
2. 掌握前向声明的使用场景和限制
3. 熟悉 C99 灵活数组成员的用法和注意事项
4. 了解 C11 匿名结构体的便利性和适用场景

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.7.2.1 Structure and union specifiers
- C17 标准 (ISO/IEC 9899:2018): 6.7.2.1 Structure and union specifiers (p: 81-84)
- C11 标准 (ISO/IEC 9899:2011): 6.7.2.1 Structure and union specifiers (p: 112-117)
- C99 标准 (ISO/IEC 9899:1999): 6.7.2.1 Structure and union specifiers (p: 101-104)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.5.2.1 Structure and union specifiers

## 相关主题

- 结构体与联合体成员访问
- 位域（bit-field）
- 结构体初始化