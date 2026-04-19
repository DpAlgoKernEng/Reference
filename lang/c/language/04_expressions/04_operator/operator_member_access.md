# 成员访问运算符 (Member Access Operators)

成员访问运算符允许访问其操作数的成员。

| 运算符 | 运算符名称 | 示例 | 描述 |
| --- | --- | --- | --- |
| [] | 数组下标 (array subscript) | a[b] | 访问数组 **a** 的第 **b** 个元素 |
| * | 解引用 (pointer dereference) | *a | 解引用指针 **a** 以访问其指向的对象或函数 |
| & | 取地址 (address of) | &a | 创建指向对象或函数 **a** 的指针 |
| . | 成员访问 (member access) | a.b | 访问结构体或联合体 **a** 的成员 **b** |
| -> | 指针成员访问 (member access through pointer) | a->b | 访问 **a** 所指向的结构体或联合体的成员 **b** |

---

## 1. 概述 (Overview)

成员访问运算符是 C 语言中一组用于访问数据成员的核心运算符。它们提供了对数组元素、指针指向对象、结构体/联合体成员的直接访问能力，是 C 语言底层内存操作的基础工具。

### 核心功能

- **数组下标运算符 `[]`**：通过索引访问数组元素
- **解引用运算符 `*`**：获取指针所指向的对象或函数
- **取地址运算符 `&`**：获取对象或函数的内存地址
- **成员访问运算符 `.`**：访问结构体或联合体的成员
- **指针成员访问运算符 `->`**：通过指针访问结构体或联合体的成员

### 技术定位

这些运算符是 C 语言指针和结构体操作的基石，理解它们的工作原理对于掌握 C 语言的内存模型至关重要。它们在底层系统编程、数据结构实现以及高性能计算中有着广泛的应用。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

成员访问运算符自 C 语言的诞生之初就存在，其设计源于 B 语言和 BCPL 语言。Dennis Ritchie 在设计 C 语言时，为了支持结构化数据和指针操作，引入了这套运算符体系。

### 设计动机

1. **数组下标运算符**：提供直观的数组元素访问方式，`a[i]` 比 `*(a+i)` 更具可读性
2. **解引用和取地址运算符**：实现指针与对象之间的双向转换，支持底层内存操作
3. **成员访问运算符**：支持结构化数据类型的成员访问
4. **指针成员访问运算符**：简化通过指针访问成员的语法，`p->member` 比 `(*p).member` 更简洁

### 版本变更

#### C89/C90 标准
- 定义了所有五种成员访问运算符的基本语义
- DR 076：允许 `&*` 运算符相互抵消，即使对于空指针也是合法的

#### C99 标准
- 完善了数组下标运算符对多维数组的支持
- 明确了指针运算的边界条件

#### C11 标准
- 增加了对原子类型 (atomic) 的限制：对原子类型使用成员访问运算符会导致未定义行为
- 引入了 `_Alignof` 运算符（现已废弃）

#### C17 和 C23 标准
- 继续保持对现有语义的兼容性
- C23 将 `alignof` 作为关键字引入

---

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 数组下标运算符 (Array Subscript Operator)

#### 语法形式

```
pointer-expression [ integer-expression ]    // 形式 1
integer-expression [ pointer-expression ]    // 形式 2
```

#### 参数说明

| 参数 | 类型 | 说明 |
| --- | --- | --- |
| pointer-expression | 指向完整对象的指针 | 可以是指针变量或数组名（会退化为指针） |
| integer-expression | 整数类型 | 作为数组索引，可以是正数或负数 |

#### 返回值

- 左值 (lvalue) 表达式
- 类型为指针所指向对象的类型

### 3.2 解引用运算符 (Dereference Operator)

#### 语法形式

```
* pointer-expression
```

#### 参数说明

| 参数 | 类型 | 说明 |
| --- | --- | --- |
| pointer-expression | 任意指针类型 | 可以是对象指针或函数指针 |

#### 返回值

- 如果是函数指针：返回函数指示符 (function designator)
- 如果是对象指针：返回指定对象的左值表达式

### 3.3 取地址运算符 (Address-of Operator)

#### 语法形式

```
& function                    // 形式 1：取函数地址
& lvalue-expression           // 形式 2：取对象地址
& * expression                // 形式 3：特殊情况，与 * 相互抵消
& expression [ expression ]   // 形式 4：特殊情况，与 [] 中的 * 抵消
```

#### 参数说明

| 参数 | 类型 | 说明 |
| --- | --- | --- |
| function | 函数指示符 | 函数名或函数引用 |
| lvalue-expression | 左值表达式 | 不能是位域 (bit-field)，不能有 register 存储类 |

#### 返回值

- 函数指针（对于函数）或对象指针（对于对象）
- 不是左值

### 3.4 成员访问运算符 (Member Access Operator)

#### 语法形式

```
expression . member-name
```

#### 参数说明

| 参数 | 类型 | 说明 |
| --- | --- | --- |
| expression | 结构体或联合体类型 | 可以是左值或右值 |
| member-name | 标识符 | 必须是指定结构体或联合体的成员名 |

#### 返回值

- 值类别与左操作数相同
- 如果左操作数有 const/volatile 限定，结果也有相应限定

### 3.5 指针成员访问运算符 (Member Access Through Pointer)

#### 语法形式

```
expression -> member-name
```

#### 参数说明

| 参数 | 类型 | 说明 |
| --- | --- | --- |
| expression | 指向结构体或联合体的指针 | 任何指向完整结构体/联合体类型的指针 |
| member-name | 标识符 | 必须是指定结构体或联合体的成员名 |

#### 返回值

- 始终是左值 (lvalue)
- 如果指向的类型有 const/volatile 限定，结果也有相应限定

---

## 4. 底层原理 (Underlying Principles)

### 4.1 数组下标运算符的本质

#### 核心定义

根据 C 标准，`E1[E2]` 完全等同于 `*((E1)+(E2))`。这意味着：

```c
a[i]  ≡  *(a + i)
i[a]  ≡  *(i + a)  // 交换律成立
```

#### 指针运算机制

当指针与整数相加时：
1. 整数首先乘以指针所指类型的大小
2. 结果是相对于原指针的偏移地址
3. 如果原指针指向数组的第 i 个元素，则新指针指向第 (i+n) 个元素

#### 多维数组

对于二维数组 `a[m][n]`：
- `a[i]` 等价于 `*(a + i)`，类型为 `type[n]`（第 i 行）
- `a[i][j]` 等价于 `*(*(a + i) + j)`

### 4.2 解引用运算符的内存访问

#### 工作原理

解引用运算符将指针值转换为对目标对象的引用：
1. 读取指针变量中存储的地址值
2. 根据指针类型确定要访问的内存大小
3. 将该地址开始的内存解释为指定类型的对象

#### 未定义行为情况

以下情况解引用会导致未定义行为：
- 空指针 (null pointer)
- 悬空指针 (dangling pointer) - 指向已超出生命周期的对象
- 未对齐的指针 (misaligned pointer)
- 值不确定的指针 (indeterminate value)

#### 特殊情况

`&*E` 表达式中，`&` 和 `*` 会相互抵消，不会实际执行解引用操作，因此对空指针使用 `&*p` 是合法的。

### 4.3 取地址运算符的实现

#### 地址计算

取地址运算符返回对象的内存地址：
- 对于函数：返回函数的入口地址
- 对于对象：返回对象在内存中的起始地址

#### 特殊优化

编译器对 `&*E` 和 `&E[N]` 有特殊处理：
- `&*p` 不实际解引用，直接返回 `p`（但结果不是左值）
- `&a[N]` 等价于 `a + N`，即使 `N` 等于数组大小也是合法的（获取尾后指针）

### 4.4 成员访问运算符的内存布局

#### 结构体成员访问

`.` 运算符通过以下步骤访问成员：
1. 确定结构体对象的起始地址
2. 加上成员在结构体中的偏移量
3. 访问该地址处的内存

#### 指针成员访问

`->` 运算符的工作流程：
1. 解引用指针获取结构体地址
2. 加上成员偏移量
3. 访问目标成员

实际上，`p->member` 完全等同于 `(*p).member`。

#### 偏移量计算

编译器在编译时已知每个成员的偏移量：
```c
struct Example {
    char a;    // 偏移量 0
    int b;     // 偏移量 4（考虑对齐）
    double c;  // 偏移量 8
};
```

### 4.5 性能特征

| 运算符 | 时间复杂度 | 空间开销 | 备注 |
| --- | --- | --- | --- |
| `[]` | O(1) | 无 | 仅涉及指针运算 |
| `*` | O(1) | 无 | 直接内存访问 |
| `&` | O(1) | 无 | 编译时确定 |
| `.` | O(1) | 无 | 编译时计算偏移 |
| `->` | O(1) | 无 | 编译时计算偏移 |

所有成员访问运算符都是常数时间操作，没有任何运行时开销。

---

## 5. 使用场景 (Use Cases)

### 5.1 数组下标运算符 `[]`

#### 适用场景

- **数组元素访问**：最直观的数组操作方式
- **指针算术替代**：比指针运算更易读
- **多维数组处理**：支持级联使用 `a[i][j]`
- **字符串字面量访问**：字符串字面量本质上是字符数组

#### 最佳实践

```c
// 推荐：使用数组下标
for (int i = 0; i < n; i++) {
    array[i] = i;
}

// 等效但可读性较差的指针方式
for (int i = 0; i < n; i++) {
    *(array + i) = i;
}
```

#### 常见陷阱

1. **数组越界**：访问负索引或超出数组大小的索引会导致未定义行为
2. **忘记交换律**：`i[a]` 和 `a[i]` 等效，可能导致代码审查时困惑

### 5.2 解引用运算符 `*`

#### 适用场景

- **指针访问**：获取指针指向的对象
- **动态内存访问**：访问 malloc 分配的内存
- **函数指针调用**：通过函数指针调用函数
- **指针链**：多级指针的逐级访问

#### 最佳实践

```c
int value = 42;
int *ptr = &value;
int **pptr = &ptr;

// 多级指针解引用
int result = **pptr;  // 等价于 *ptr，也等价于 value
```

#### 常见陷阱

1. **空指针解引用**：最常见的程序崩溃原因之一
2. **悬空指针**：指向已释放或超出作用域的内存
3. **类型不匹配**：错误类型的指针解引用会导致数据损坏

### 5.3 取地址运算符 `&`

#### 适用场景

- **创建指针**：获取对象的地址
- **函数指针**：获取函数的地址
- **输出参数**：将变量地址传递给函数以修改其值
- **指针运算起点**：作为指针算术的基础

#### 最佳实践

```c
int x = 10;
int *p = &x;           // 获取对象地址

void (*fp)(int) = &func;  // 获取函数地址

scanf("%d", &x);       // 传递地址给 scanf
```

#### 常见陷阱

1. **对寄存器变量取地址**：`register` 变量不能取地址
2. **对位域取地址**：位域成员不能取地址
3. **对字面量取地址**：`&5` 或 `&"hello"` 等操作是非法的

### 5.4 成员访问运算符 `.`

#### 适用场景

- **结构体/联合体成员访问**：直接访问成员
- **函数返回值访问**：访问函数返回的结构体成员
- **临时对象访问**：访问复合字面量的成员

#### 最佳实践

```c
struct Point { int x, y; };

struct Point p = {10, 20};
p.x = 15;  // 修改成员

struct Point get_origin(void);
int x = get_origin().x;  // 访问返回值的成员
```

#### 常见陷阱

1. **const 限定传播**：const 结构体的成员也是 const
2. **右值不能赋值**：`get_point().x = 1` 是错误的
3. **原子类型未定义**：对原子结构体使用 `.` 是未定义行为

### 5.5 指针成员访问运算符 `->`

#### 适用场景

- **动态分配的结构体**：访问 malloc 创建的结构体成员
- **链表/树节点访问**：遍历链表或树结构
- **结构体指针参数**：函数参数为结构体指针时访问成员
- **嵌入式系统**：通过指针访问硬件寄存器映射的结构体

#### 最佳实践

```c
struct Node {
    int data;
    struct Node *next;
};

// 链表遍历
void traverse(struct Node *head) {
    while (head != NULL) {
        printf("%d\n", head->data);
        head = head->next;
    }
}

// 动态分配
struct Point *p = malloc(sizeof(struct Point));
p->x = 10;
p->y = 20;
```

#### 常见陷阱

1. **NULL 指针解引用**：对 NULL 指针使用 `->` 会崩溃
2. **野指针**：使用未初始化的指针
3. **类型不匹配**：指针类型与结构体类型不一致

---

## 6. 代码示例 (Examples)

### 6.1 数组下标运算符示例

#### 基础用法

```c
#include <stdio.h>

int main(void)
{
    int a[3] = {1, 2, 3};

    // 正常数组访问
    printf("%d %d\n", a[2],     // 输出: 3
                       2[a]);   // 输出: 3（交换律）

    // 数组下标是左值，可以赋值
    a[2] = 7;
    printf("a[2] = %d\n", a[2]); // 输出: 7

    // 多维数组
    int n[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int (*p)[3] = &n[1];  // 指向第二行
    printf("%d %d %d\n", (*p)[0], p[0][1], p[0][2]); // 输出: 4 5 6

    int x = n[1][2];  // 访问第二行第三列
    printf("x = %d\n", x);  // 输出: 6

    // 字符串字面量也是数组
    printf("%c %c\n", "abc"[2], 2["abc"]);  // 输出: c c

    return 0;
}
```

#### 输出

```
3 3
a[2] = 7
4 5 6
x = 6
c c
```

### 6.2 解引用运算符示例

#### 基础用法

```c
#include <stdio.h>

int main(void)
{
    int n = 1;
    int *p = &n;

    // 解引用获取值
    printf("*p = %d\n", *p);  // 输出: 1

    // 解引用是左值，可以赋值
    *p = 7;
    printf("*p = %d\n", *p);  // 输出: 7

    // 多级指针解引用
    int **pp = &p;
    printf("**pp = %d\n", **pp);  // 输出: 7

    return 0;
}
```

#### 常见错误示例

```c
#include <stdio.h>

int main(void)
{
    int *p = NULL;

    // 错误：空指针解引用（会导致程序崩溃）
    // printf("%d\n", *p);

    // 正确：先检查指针是否为空
    if (p != NULL) {
        printf("%d\n", *p);
    } else {
        printf("Pointer is NULL\n");
    }

    // 悬空指针示例
    int *dangling;
    {
        int local = 10;
        dangling = &local;
    }  // local 在此超出作用域
    // dangling 现在是悬空指针

    // 错误：解引用悬空指针（未定义行为）
    // printf("%d\n", *dangling);

    return 0;
}
```

### 6.3 取地址运算符示例

#### 基础用法

```c
#include <stdio.h>

int f(char c) {
    return c;
}

int main(void)
{
    int n = 1;

    // 取对象地址
    int *p = &n;
    printf("n = %d, *p = %d\n", n, *p);  // 输出: 1 1

    // 取函数地址
    int (*fp)(char) = &f;
    printf("f('A') = %d\n", fp('A'));  // 输出: 65

    // 数组地址运算
    int a[3] = {1, 2, 3};
    int *beg = a;
    int *end = &a[3];  // 尾后指针（合法）
    // &a[3] 等价于 a + 3

    printf("Array elements: ");
    for (int *it = beg; it != end; ++it) {
        printf("%d ", *it);
    }
    printf("\n");  // 输出: 1 2 3

    // 特殊情况：&* 相互抵消
    int *ptr = NULL;
    int **pp = &*ptr;  // 合法！&* 抵消，不会解引用空指针
    (void)pp;  // 避免未使用变量警告

    return 0;
}
```

### 6.4 成员访问运算符示例

#### 基础用法

```c
#include <stdio.h>

struct Point {
    int x;
    int y;
};

struct Point get_point(void) {
    return (struct Point){1, 2};
}

int main(void)
{
    // 结构体成员访问
    struct Point p;
    p.x = 10;   // 修改成员
    p.y = 20;
    printf("p = (%d, %d)\n", p.x, p.y);  // 输出: (10, 20)

    // 函数返回值的成员访问
    int x = get_point().x;
    printf("x = %d\n", x);  // 输出: 1

    // 错误示例：返回值是右值，不能赋值
    // get_point().x = 100;  // 编译错误

    // const 限定传播
    const struct Point cp = {5, 10};
    // cp.x = 1;  // 编译错误：const 成员不能修改
    printf("cp = (%d, %d)\n", cp.x, cp.y);  // 输出: (5, 10)

    // 联合体成员访问
    union Data {
        int i;
        double d;
    };

    union Data u = {.i = 1};
    printf("u.i = %d\n", u.i);  // 输出: 1

    u.d = 0.5;  // 改变联合体的活跃成员
    printf("u.d = %f\n", u.d);  // 输出: 0.500000

    return 0;
}
```

### 6.5 指针成员访问运算符示例

#### 基础用法

```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

int main(void)
{
    // 基本指针成员访问
    struct Node n = {1, NULL};
    struct Node *p = &n;

    p->data = 10;
    printf("p->data = %d\n", p->data);  // 输出: 10

    // 动态分配的结构体
    struct Node *head = malloc(sizeof(struct Node));
    head->data = 1;
    head->next = malloc(sizeof(struct Node));
    head->next->data = 2;
    head->next->next = NULL;

    // 链表遍历
    printf("Linked list: ");
    for (struct Node *it = head; it != NULL; it = it->next) {
        printf("%d ", it->data);
    }
    printf("\n");  // 输出: 1 2

    // 释放内存
    free(head->next);
    free(head);

    return 0;
}
```

#### 链表实现示例

```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

// 创建新节点
struct Node* create_node(int data) {
    struct Node *node = malloc(sizeof(struct Node));
    if (node != NULL) {
        node->data = data;
        node->next = NULL;
    }
    return node;
}

// 在链表末尾添加节点
void append(struct Node **head, int data) {
    struct Node *new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    struct Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

// 打印链表
void print_list(struct Node *head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

// 释放链表
void free_list(struct Node *head) {
    while (head != NULL) {
        struct Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void)
{
    struct Node *head = NULL;

    append(&head, 1);
    append(&head, 2);
    append(&head, 3);

    printf("Linked list: ");
    print_list(head);  // 输出: 1 -> 2 -> 3 -> NULL

    free_list(head);
    return 0;
}
```

### 6.6 综合示例

#### 指针与数组的各种访问方式

```c
#include <stdio.h>

int main(void)
{
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;

    // 四种等价的访问方式
    printf("arr[2]     = %d\n", arr[2]);    // 数组下标
    printf("*(arr + 2) = %d\n", *(arr + 2)); // 指针运算
    printf("p[2]       = %d\n", p[2]);      // 指针下标
    printf("*(p + 2)   = %d\n", *(p + 2));  // 指针运算

    // 两种等价的取地址方式
    printf("&arr[3]    = %p\n", (void*)&arr[3]);
    printf("arr + 3    = %p\n", (void*)(arr + 3));

    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

1. **数组下标运算符 `[]`**
   - 本质是指针运算：`a[i]` 等价于 `*(a + i)`
   - 支持交换律：`a[i]` 和 `i[a]` 等效
   - 结果是左值，可以赋值
   - 时间复杂度 O(1)

2. **解引用运算符 `*`**
   - 将指针转换为对象引用
   - 对于函数指针返回函数指示符
   - 解引用空指针、悬空指针是未定义行为
   - `&*p` 会相互抵消，不会实际解引用

3. **取地址运算符 `&`**
   - 返回指针，不是左值
   - 不能对位域和 register 变量使用
   - `&a[N]` 可以获取尾后指针（合法）

4. **成员访问运算符 `.`**
   - 用于结构体/联合体类型的对象
   - 值类别与左操作数相同
   - const/volatile 限定会传播

5. **指针成员访问运算符 `->`**
   - 用于结构体/联合体类型的指针
   - 等价于 `(*p).member`
   - 结果始终是左值
   - 需要检查指针是否为 NULL

### 技术对比

| 特性 | `.` | `->` |
| --- | --- | --- |
| 左操作数类型 | 结构体/联合体对象 | 结构体/联合体指针 |
| 值类别 | 继承左操作数 | 始终是左值 |
| 典型用途 | 栈对象、函数返回值 | 动态分配、链表节点 |
| NULL 检查 | 不需要 | 需要 |

### 运算符优先级

在 C 语言运算符优先级表中，成员访问运算符 `.` 和 `->` 具有最高优先级（与函数调用 `()` 和数组下标 `[]` 同级），从左到右结合。这意味着：

```c
*p->member   // 等价于 *(p->member)，而不是 (*p)->member
&a->member   // 等价于 &(a->member)
```

### 学习建议

1. **理解指针的本质**：指针是地址的抽象，所有成员访问运算符最终都涉及地址计算

2. **掌握底层内存模型**：
   - 数组名会退化为指向首元素的指针
   - 结构体成员在内存中有固定的偏移量
   - 指针运算考虑类型大小

3. **实践安全编程**：
   - 使用指针前检查是否为 NULL
   - 避免悬空指针（释放后置 NULL）
   - 使用 const 限定不可修改的数据

4. **调试技巧**：
   - 使用调试器查看指针值和内存内容
   - 使用 Valgrind 等工具检测内存错误
   - 理解编译器警告信息

### 常见问题

**Q: 为什么 `a[i]` 和 `i[a]` 等效？**

A: 因为数组下标定义为 `E1[E2]` 等价于 `*((E1)+(E2))`，加法满足交换律，所以 `a[i]` 和 `i[a]` 都等价于 `*(a+i)` 和 `*(i+a)`。

**Q: `->` 和 `.` 有什么区别？**

A: `.` 用于对象本身，`->` 用于指向对象的指针。`p->member` 等价于 `(*p).member`。

**Q: `&*p` 会解引用空指针吗？**

A: 不会。C 标准规定 `&` 和 `*` 会相互抵消，不会实际执行解引用操作，因此 `&*p` 即使在 `p` 是空指针时也是合法的。

**Q: 如何选择使用 `.` 还是 `->`？**

A: 如果你有对象本身（结构体变量），使用 `.`；如果你有指向对象的指针，使用 `->`。你也可以通过 `(*p).member` 使用 `.`，但 `p->member` 更简洁。

---

## 参考资料

- C17 标准 (ISO/IEC 9899:2018):
  - 6.5.2.1 数组下标 (p: 57-58)
  - 6.5.2.3 结构体和联合体成员 (p: 58-59)
  - 6.5.3.2 地址和间接运算符 (p: 59-61)

- C11 标准 (ISO/IEC 9899:2011):
  - 6.5.2.1 数组下标 (p: 80)
  - 6.5.2.3 结构体和联合体成员 (p: 82-84)
  - 6.5.3.2 地址和间接运算符 (p: 88-89)

- C99 标准 (ISO/IEC 9899:1999):
  - 6.5.2.1 数组下标 (p: 70)
  - 6.5.2.3 结构体和联合体成员 (p: 72-74)
  - 6.5.3.2 地址和间接运算符 (p: 78-79)

- C89/C90 标准 (ISO/IEC 9899:1990):
  - 3.3.2.1 数组下标
  - 3.3.2.3 结构体和联合体成员
  - 3.3.3.2 地址和间接运算符

### 缺陷报告

| DR | 应用于 | 原发布行为 | 修正行为 |
| --- | --- | --- | --- |
| DR 076 | C89 | 多余的间接运算不能被 `&` 抵消 | 允许抵消 |

### 相关主题

- 运算符优先级
- 指针算术
- 结构体与联合体
- 数组与指针的关系