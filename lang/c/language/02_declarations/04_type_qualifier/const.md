# const 类型限定符 (const type qualifier)

## 1. 概述

在 C 语言类型系统中，每个基本类型都有若干**限定版本**（qualified versions），对应 `const`、`volatile` 和 `restrict`（仅限对象指针类型）三种类型限定符中的一至三种组合。本文档详细描述 `const` 限定符的作用与用法。

`const` 限定符的核心作用是声明**只读**对象。使用 `const` 修饰的对象：

- 可能被编译器放置在只读内存区域
- 如果程序中从未取其地址，编译器可能完全不为其分配存储空间
- 任何试图修改 `const` 限定类型对象的操作都会导致**未定义行为**（undefined behavior）

## 2. 来源与演变

### 首次引入

`const` 关键字首次在 **C89/C90** 标准中引入，作为类型限定符的一部分。该特性从 C++ 语言借鉴而来。

### 历史背景

在 `const` 引入之前，C 语言程序员主要依靠以下方式实现类似功能：

1. **宏定义**：使用 `#define` 定义常量，但缺乏类型检查
2. **枚举**：只能定义整数常量
3. **约定俗成**：依靠编程规范约束，无编译器保护

`const` 的出现解决了这些问题，提供了：
- 编译期类型检查
- 明确的只读语义
- 更好的代码可读性和安全性

### C 语言与 C++ 的关键差异

这是 C 语言中 `const` 与 C++ 中最重要的区别：

| 特性 | C 语言 | C++ 语言 |
|------|--------|----------|
| `const` 对象作为常量表达式 | 不可以 | 可以 |
| 用于 `case` 标签 | 不可以 | 可以 |
| 初始化静态存储期对象 | 不可以 | 可以 |
| 初始化枚举器 | 不可以 | 可以 |
| 用于位域大小 | 不可以 | 可以 |
| 用于数组大小时 | 创建 VLA（变长数组） | 创建固定大小数组 |

### C99 变化

- 允许在函数参数的数组声明中使用 `const`，用于限定转换后的指针类型
- `const` 限定的复合字面量不保证是独立对象，可能与其他复合字面量或字符串字面量共享存储

### C23 变化

- 数组类型与其元素类型总是被认为具有相同的 `const` 限定属性
- 在 C23 之前，通过 `typedef` 定义的数组类型被 `const` 修饰时，数组类型本身不是 `const` 限定的，只有其元素类型是

## 3. 语法与参数

### 基本语法

```c
const type name = value;           // const 修饰基本类型
type const *ptr;                   // 指向 const 对象的指针
type *const ptr;                   // const 指针（指向非 const 对象）
const type *const ptr;             // const 指针指向 const 对象
```

### 指针与 const 的组合

| 声明 | 含义 |
|------|------|
| `int *p` | 指向 int 的指针（可修改指针、可修改值） |
| `const int *p` | 指向 const int 的指针（可修改指针、不可修改值） |
| `int *const p` | const 指针指向 int（不可修改指针、可修改值） |
| `const int *const p` | const 指针指向 const int（两者都不可修改） |

### const 语义适用范围

`const` 语义仅适用于**左值表达式**（lvalue expression）。当一个 `const` 限定的左值表达式用于不需要左值的上下文时，其 `const` 限定符会丢失（注意：如果存在 `volatile` 限定符，则不会丢失）。

### 不可修改左值

以下左值表达式是**不可修改左值**（non-modifiable lvalues），不能被赋值：

1. 指定 `const` 限定类型对象的表达式
2. 指定结构体或联合体类型对象的表达式，且该结构体/联合体至少有一个成员是 `const` 限定类型（包括递归包含的聚合体或联合体成员）

### 结构体/联合体成员的 const 传播

结构体或联合体类型的 `const` 限定成员会获得其所属类型的限定属性：

```c
struct s { int i; const int ci; } s;
// s.i 的类型是 int
// s.ci 的类型是 const int

const struct s cs;
// cs.i 和 cs.ci 的类型都是 const int
```

### 函数参数中的 const（C99 起）

在函数声明中，`const` 可以出现在用于声明函数参数数组类型的方括号内。它限定数组类型转换后的指针类型：

```c
void f(double x[const], const double y[const]);
// 等价于：
void f(double * const x, const double * const y);
```

## 4. 底层原理

### 编译器优化

`const` 限定符为编译器提供了重要的优化信息：

1. **常量折叠**：编译器可以在编译期计算 `const` 变量的值
2. **只读内存放置**：`const` 对象可能被放置在只读内存段（如 `.rodata`）
3. **存储优化**：如果从未取 `const` 对象的地址，编译器可能完全不为其分配存储

### 类型系统层面

在 C 语言类型系统中，`const` 是类型限定符，修饰的是类型本身：

```
int          -> 基本类型
const int    -> const 限定的 int 类型
int *        -> 指向 int 的指针
const int *  -> 指向 const int 的指针
int *const   -> const 限定的指针（指向 int）
```

### 指针类型兼容性规则

对于两个类型要兼容，它们的限定属性必须相同：

```c
char *p = 0;
const char **cpp = &p;    // 错误：char* 和 const char* 不是兼容类型
char * const *pcp = &p;    // 正确：添加限定（char* 到 char*const）
```

### 数组类型的 const 限定

| C 标准版本 | 规则 |
|-----------|------|
| C23 之前 | 通过 `typedef` 定义的数组类型被 `const` 修饰时，数组类型本身不是 `const` 限定的，只有其元素类型是 |
| C23 及之后 | 数组类型与其元素类型总是被认为具有相同的 `const` 限定属性 |

```c
typedef int A[2][3];
const A a = {{4, 5, 6}, {7, 8, 9}};  // const int 的数组的数组

int* pi = a[0];           // 错误：a[0] 的类型是 const int*
void* unqual_ptr = a;     // C23 之前：正确；C23 之后：错误
```

## 5. 使用场景

### 适合使用 const 的场景

| 场景 | 说明 |
|------|------|
| 函数参数 | 防止函数内部意外修改参数值 |
| 全局常量 | 定义程序范围内的常量值 |
| 指针参数 | 表明函数不会通过指针修改指向的数据 |
| 配置值 | 定义编译时可确定的配置值 |
| 接口契约 | 明确表达"不修改"的意图 |

### 最佳实践

1. **函数参数使用 const 指针**：如果函数不应该修改指针指向的内容
2. **返回 const 指针**：如果调用者不应该修改返回的数据
3. **const 正确性**：保持整个调用链的 const 一致性
4. **头文件中声明常量**：使用 `extern const` 在头文件中声明，在源文件中定义

### 常见陷阱

1. **const 对象的地址转换**
   - 将 `const` 对象的地址转换为非 const 指针是危险的
   - 通过非 const 指针修改 const 对象是未定义行为

2. **指针到指针的转换**
   - `T**` 不能隐式转换为 `const T**`
   - 这可能导致无意中修改 const 对象

3. **C 与 C++ 的差异**
   - 在 C 中，`const` 变量不是常量表达式
   - 移植代码时需要特别注意这一点

4. **数组 typedef 的 const 行为变化**
   - C23 标准改变了通过 typedef 定义的数组类型的 const 行为

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>

int main(void) {
    /* const 限定变量 */
    const int n = 1;        /* const 限定的 int 类型对象 */
    /* n = 2; */            /* 错误：n 是 const 限定的，不能赋值 */

    printf("n = %d\n", n);
    return 0;
}
```

### 指针与 const

```c
#include <stdio.h>

int main(void) {
    int x = 10;
    int y = 20;

    /* 指向 const int 的指针 */
    const int *p1 = &x;
    /* *p1 = 15; */         /* 错误：不能修改 const 指向的值 */
    p1 = &y;                /* 正确：可以修改指针本身 */

    /* const 指针（指向 int） */
    int *const p2 = &x;
    *p2 = 15;               /* 正确：可以修改指向的值 */
    /* p2 = &y; */          /* 错误：不能修改 const 指针 */

    /* const 指针指向 const int */
    const int *const p3 = &x;
    /* *p3 = 25; */         /* 错误：不能修改值 */
    /* p3 = &y; */          /* 错误：不能修改指针 */

    printf("x = %d, y = %d\n", x, y);
    return 0;
}
```

### 函数参数中的 const

```c
#include <stdio.h>
#include <string.h>

/* 参数声明：p 是 const 指针，指向 const char */
/* 函数内部不能修改 p 本身，也不能修改 p 指向的内容 */
size_t safe_strlen(const char *const p) {
    /* p = "other"; */      /* 错误：不能修改 const 指针 */
    /* p[0] = 'x'; */       /* 错误：不能修改 const 数据 */
    size_t len = 0;
    const char *tmp = p;
    while (*tmp++) len++;
    return len;
}

/* C99 数组参数语法 */
void process(double x[const], const double y[const]) {
    /* x 和 y 本身是 const 指针 */
    /* x = y; */            /* 错误：不能修改 const 指针 */
    x[0] = 1.0;             /* 正确：x 指向的数据不是 const */
    /* y[0] = 2.0; */       /* 错误：y 指向 const double */
}

int main(void) {
    const char *str = "Hello, World!";
    printf("Length: %zu\n", safe_strlen(str));
    return 0;
}
```

### 结构体中的 const 成员

```c
#include <stdio.h>

struct point {
    int x;
    const int y;            /* const 成员 */
};

int main(void) {
    struct point p1 = {10, 20};
    struct point p2 = {30, 40};

    p1.x = 15;              /* 正确：x 不是 const */
    /* p1.y = 25; */        /* 错误：y 是 const 成员 */

    /* p1 = p2; */          /* 错误：结构体包含 const 成员，整体赋值非法 */

    printf("p1: (%d, %d)\n", p1.x, p1.y);
    return 0;
}
```

### 常见错误及修正

#### 错误 1：修改 const 对象（未定义行为）

```c
/* 错误示例 */
const int n = 1;            /* const 限定的对象 */
int *p = (int*)&n;          /* 强制转换去除 const */
*p = 2;                     /* 未定义行为！可能崩溃或产生意外结果 */

/* 正确做法：如果需要修改值，不要声明为 const */
int m = 1;
int *q = &m;
*q = 2;                     /* 正确：m 不是 const */
```

#### 错误 2：指针类型不兼容

```c
/* 错误示例 */
char *p = 0;
const char **cpp = &p;      /* 错误：char* 和 const char* 不是兼容类型 */
                            /* 这会导致类型系统漏洞 */

/* 正确做法 */
char *p = 0;
const char *const *pcp = &p; /* 正确：添加限定 */
```

#### 错误 3：在 C 中使用 const 变量定义数组大小

```c
/* C 语言中的问题 */
const int size = 10;
int arr[size];              /* C89-C17: 创建 VLA（变长数组），非标准数组 */
                            /* C23: 可能是标准数组 */

/* 正确做法 1：使用宏定义 */
#define SIZE 10
int arr1[SIZE];             /* 正确：编译时常量 */

/* 正确做法 2：使用枚举 */
enum { SIZE_ENUM = 10 };
int arr2[SIZE_ENUM];        /* 正确：枚举常量是常量表达式 */
```

#### 错误 4：函数类型使用 const

```c
/* 错误示例：通过 typedef 给函数类型加 const */
typedef void func_t(int);
const func_t my_func;       /* 未定义行为！ */

/* 正确做法：const 放在参数上 */
void my_func(const int x);  /* 正确：限定参数类型 */
```

## 注意事项

1. **未定义行为**：修改 `const` 对象会导致未定义行为，编译器可能优化掉相关代码
2. **C 与 C++ 差异**：C 中 `const` 变量不是常量表达式，不能用于定义数组大小等场景
3. **指针转换风险**：将 `const` 指针转换为非 `const` 指针（通过强制转换）虽然编译可能通过，但使用结果指针修改数据是未定义行为
4. **复合字面量共享存储**（C99）：`const` 限定的复合字面量可能与其他字面量共享存储
5. **结构体赋值限制**：包含 `const` 成员的结构体不能整体赋值

## 相关概念

| 概念 | 关系 |
|------|------|
| `volatile` 限定符 | 另一种类型限定符，防止编译器优化 |
| `restrict` 限定符 | 仅用于指针类型，表明指针是访问对象的唯一方式 |
| `#define` 宏 | 传统常量定义方式，无类型检查 |
| `enum` 枚举 | 定义整数常量，是常量表达式 |

## 7. 总结

`const` 类型限定符是 C 语言中实现类型安全和编译器优化的重要工具。它提供了：

- **只读语义**：明确表达"不修改"的编程意图
- **编译期检查**：编译器帮助防止意外修改
- **优化机会**：编译器可以基于 const 信息进行优化

核心使用建议：

1. **函数参数**：如果函数不应修改参数，使用 `const` 限定
2. **指针参数**：使用 `const type *ptr` 声明"只读"指针参数
3. **避免强制转换**：不要通过强制转换去除 const 属性
4. **注意 C/C++ 差异**：移植代码时注意 `const` 在两种语言中的不同语义

## 参考资料

- C17 standard (ISO/IEC 9899:2018): 6.7.3 Type qualifiers (p: 87-90)
- C11 standard (ISO/IEC 9899:2011): 6.7.3 Type qualifiers (p: 121-123)
- C99 standard (ISO/IEC 9899:1999): 6.7.3 Type qualifiers (p: 108-110)
- C89/C90 standard (ISO/IEC 9899:1990): 6.5.3 Type qualifiers
- cppreference: https://en.cppreference.com/w/c/language/const