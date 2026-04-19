# 查找与命名空间（Lookup and Name Spaces）

## 1. 概述（Overview）

当 C 程序中遇到标识符时，会执行**查找（lookup）** 操作以定位引入该标识符且当前处于作用域内的声明。C 语言允许同一标识符的多个声明同时处于作用域内，前提是这些标识符属于不同的类别，称为**命名空间（name spaces）**。

### 命名空间的作用

命名空间解决了标识符命名冲突问题，允许同一名称在不同上下文中表示不同的实体。

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C89/C90 | 定义四种命名空间：标签、标签名、成员名、普通标识符 |
| C99 | 沿用既有命名空间规则 |
| C11 | 沿用既有命名空间规则 |
| C17 | 沿用既有命名空间规则 |
| C23 | 新增属性相关命名空间：全局属性命名空间、非标准属性命名空间 |

### 与 C++ 的区别

| 特性 | C | C++ |
|------|---|-----|
| 结构体作用域 | 无 | 有 |
| 枚举常量命名空间 | 普通标识符命名空间 | 结构体作用域内 |
| 成员命名空间 | 每个结构体/联合体独立 | 类作用域 |
| 属性命名空间 | C23 起支持 | C++11 起支持 |

## 3. 语法与参数（Syntax and Parameters）

### 3.1 命名空间类型

C 语言定义以下命名空间：

| 编号 | 命名空间 | 包含的标识符 |
|------|----------|--------------|
| 1 | 标签命名空间 | 所有声明为标签的标识符 |
| 2 | 标签名命名空间 | 所有声明为结构体、联合体、枚举类型名称的标识符（三类共享一个命名空间） |
| 3 | 成员命名空间 | 所有声明为某个结构体或联合体成员的标识符（每个结构体/联合体有独立命名空间） |
| 4 | 全局属性命名空间（C23 起） | 标准定义的属性标记或实现定义的属性前缀 |
| 5 | 非标准属性命名空间（C23 起） | 属性前缀后的属性名（每个属性前缀有独立命名空间） |
| 6 | 普通标识符命名空间 | 所有其他标识符（函数名、对象名、typedef 名、枚举常量） |

### 3.2 命名空间的确定规则

在查找时，标识符的命名空间由其使用方式决定：

| 使用方式 | 查找的命名空间 |
|----------|----------------|
| `goto` 语句的操作数 | 标签命名空间 |
| `struct`、`union`、`enum` 关键字后的标识符 | 标签名命名空间 |
| 成员访问运算符（`.` 或 `->`）后的标识符 | 左操作数类型的成员命名空间 |
| 属性说明符 `[[...]]` 中直接出现的标识符（C23 起） | 全局属性命名空间 |
| 属性前缀后的 `::` 后的标识符（C23 起） | 该属性前缀的命名空间 |
| 其他所有情况 | 普通标识符命名空间 |

### 3.3 命名空间与作用域的关系

命名空间和作用域是正交概念：
- **命名空间**：决定哪些标识符可以同名
- **作用域**：决定标识符在哪里可见

同一命名空间内，作用域嵌套规则适用（内层隐藏外层）。

## 4. 底层原理（Underlying Principles）

### 4.1 查找过程

当编译器遇到标识符时：

```
1. 确定使用上下文 → 确定命名空间
2. 在该命名空间中查找声明
3. 遵循作用域规则（从内到外）
4. 找到匹配声明或报错
```

### 4.2 宏名称的特殊处理

宏名称不属于任何命名空间，因为在语义分析之前已被预处理器替换：

```c
#define foo 100

void foo(void);  // 错误：foo 已被替换为 100
```

### 4.3 typedef 注入技术

常用技术是使用 `typedef` 将结构体/联合体/枚举名注入到普通标识符命名空间：

```c
struct A { };         // 在标签命名空间引入名称 A
typedef struct A A;   // 首先，"struct" 后的 A 在标签命名空间查找
                      // 然后，在普通标识符命名空间引入名称 A

struct A* p;          // OK，此 A 在标签命名空间查找
A* q;                 // OK，此 A 在普通标识符命名空间查找
```

### 4.4 标签命名空间共享

结构体、联合体、枚举的标签共享同一个命名空间：

```c
struct foo { int x; };
union foo { int a; float b; };  // 错误：foo 在标签命名空间已存在
enum foo { A, B };              // 错误：foo 在标签命名空间已存在
```

### 4.5 成员命名空间独立性

每个结构体和联合体有自己独立的成员命名空间：

```c
struct A { int x; };
struct B { int x; };  // OK：B 有独立的成员命名空间

struct A a;
struct B b;
a.x = 1;  // 访问 A 的成员 x
b.x = 2;  // 访问 B 的成员 x
```

## 5. 使用场景（Use Cases）

### 5.1 标签与普通标识符共存

```c
// POSIX sys/stat.h 中的经典例子
int stat(const char *path, struct stat *buf);
//      ↑ 普通标识符命名空间（函数名）
//                           ↑ 标签命名空间（结构体名）
```

### 5.2 成员名与普通标识符共存

```c
struct Point {
    int x;  // 成员命名空间
    int y;  // 成员命名空间
};

int x = 10;  // 普通标识符命名空间，与成员 x 不冲突

struct Point p = {1, 2};
printf("%d, %d\n", p.x, x);  // 1, 10
```

### 5.3 标签与函数名共存

```c
void foo(void) { return; }  // 普通标识符命名空间（函数）
struct foo { int x; };       // 标签命名空间（结构体）

foo:                         // 标签命名空间（标签）
    goto foo;                // 跳转到标签
```

### 5.4 枚举常量在普通命名空间

```c
struct tagged_union {
   enum {INT, FLOAT, STRING} type;  // INT, FLOAT, STRING 在普通命名空间
   union {
      int integer;
      float floating_point;
      char *string;
   };
} tu;

tu.type = INT;  // C: OK，INT 在普通命名空间
                // C++: 错误，INT 在结构体作用域内
```

### 5.5 常见陷阱

1. **标签命名空间冲突**：

```c
struct foo { int x; };
enum foo { A, B };  // 错误：foo 在标签命名空间已存在
```

2. **普通标识符命名空间冲突**：

```c
int foo = 10;
void foo(void);  // 错误：foo 在普通命名空间已存在
```

3. **枚举常量与变量冲突**：

```c
enum Color { RED, GREEN, BLUE };
int RED = 0;  // 错误：RED 在普通命名空间已存在
```

## 6. 代码示例（Examples）

### 基础用法

```c
#include <stdio.h>

// 同一标识符在不同命名空间
void foo(void) { puts("function"); }  // 普通命名空间

struct foo {                           // 标签命名空间
    int foo;                           // 成员命名空间（属于 struct foo）
};

int main(void)
{
    // 使用不同命名空间的 foo
    foo();                             // 调用函数
    struct foo s = {42};               // 声明结构体变量
    printf("member: %d\n", s.foo);     // 访问成员

    // 标签
    goto foo;
foo:
    puts("label");

    return 0;
}
```

### typedef 注入模式

```c
#include <stdio.h>

// 方式一：分离声明
struct Node {
    int data;
    struct Node* next;
};
typedef struct Node Node;  // 注入到普通命名空间

// 方式二：合并声明
typedef struct List {
    int data;
    struct List* next;
} List;

int main(void)
{
    Node n1 = {1, NULL};
    struct Node n2 = {2, NULL};  // 两种方式都可以

    List l1 = {10, NULL};
    struct List l2 = {20, NULL};

    return 0;
}
```

### 成员命名空间独立性

```c
#include <stdio.h>

struct A {
    int value;
    char name[20];
};

struct B {
    int value;    // 与 A::value 不冲突
    double data;
};

union C {
    int value;    // 与 A::value 和 B::value 都不冲突
    float f;
};

int main(void)
{
    struct A a = {1, "A"};
    struct B b = {2, 3.14};
    union C c = {100};

    printf("A.value = %d\n", a.value);  // 1
    printf("B.value = %d\n", b.value);  // 2
    printf("C.value = %d\n", c.value);  // 100

    return 0;
}
```

### 属性命名空间（C23 起）

```c
#include <stdio.h>

// 全局属性
[[nodiscard]] int get_value(void) { return 42; }

// 属性前缀命名空间
// [[prefix::attr]]  // prefix 有独立命名空间

int main(void)
{
    get_value();  // 编译器可能警告：忽略了 nodiscard 返回值
    return 0;
}

/* 注意：
 * 如果标准属性、属性前缀或非标准属性名不被支持，
 * 无效属性本身会被忽略，不会导致错误。
 */
```

### 常见错误及修正

```c
#include <stdio.h>

/* 错误：标签命名空间冲突 */
struct foo { int x; };
// union foo { int a; };  // 错误：foo 在标签命名空间已存在

/* 正确：使用不同名称 */
struct foo_struct { int x; };
union foo_union { int a; };

/* 错误：普通命名空间冲突 */
int bar = 10;
// void bar(void);  // 错误：bar 在普通命名空间已存在

/* 正确：使用不同名称 */
int bar_value = 10;
void bar_func(void) {}

/* 错误：枚举常量与变量冲突 */
enum Color { RED, GREEN, BLUE };
// int RED = 0;  // 错误：RED 在普通命名空间已存在

/* 正确：枚举常量与变量使用不同名称 */
enum Status { STATUS_OK, STATUS_ERROR };
int ok_flag = 0;

/* C 与 C++ 的差异示例 */
struct tagged_union {
   enum {INT, FLOAT, STRING} type;
   union {
      int integer;
      float floating_point;
      char *string;
   };
} tu;

int main(void)
{
    tu.type = INT;  // C: OK，INT 在普通命名空间
                    // C++: 错误，INT 在结构体作用域内

    /* 正确做法：避免歧义 */
    enum TagType { TAG_INT, TAG_FLOAT, TAG_STRING };
    struct tagged_union2 {
       enum TagType type;
       union {
          int integer;
          float floating_point;
          char *string;
       };
    } tu2;

    tu2.type = TAG_INT;  // C 和 C++ 都 OK

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **命名空间解决冲突**：允许同一名称在不同上下文中表示不同实体
2. **六种命名空间**：标签、标签名、成员、全局属性、非标准属性、普通标识符
3. **使用方式决定命名空间**：标识符的使用上下文决定在哪个命名空间查找
4. **与作用域正交**：命名空间和作用域是独立的维度

### 命名空间速查表

| 使用方式 | 命名空间 | 示例 |
|----------|----------|------|
| `goto label` | 标签 | `goto foo;` |
| `struct/union/enum name` | 标签名 | `struct foo;` |
| `obj.member` 或 `ptr->member` | 成员 | `p.x` |
| `[[attr]]` | 全局属性（C23） | `[[nodiscard]]` |
| `[[prefix::attr]]` | 属性前缀（C23） | `[[gnu::unused]]` |
| 其他 | 普通标识符 | `int x;` |

### 与 C++ 的关键差异

| 特性 | C | C++ |
|------|---|-----|
| 结构体作用域 | 无 | 有 |
| 枚举常量位置 | 普通命名空间 | 结构体作用域 |
| `tu.type = INT` | OK | 错误 |

### 最佳实践

- 使用 `typedef` 将标签名注入普通命名空间，提高可读性
- 避免在同一作用域使用相同名称的不同类型实体（虽然语法允许）
- 注意枚举常量在普通命名空间，可能与变量冲突
- 理解 C 与 C++ 在结构体作用域上的差异

### 参考资料

- C17 标准 (ISO/IEC 9899:2018): 6.2.3 Name spaces of identifiers (p: 29-30)
- C11 标准 (ISO/IEC 9899:2011): 6.2.3 Name spaces of identifiers (p: 37)
- C99 标准 (ISO/IEC 9899:1999): 6.2.3 Name spaces of identifiers (p: 31)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.1.2.3 Name spaces of identifiers