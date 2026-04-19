# 类型（Type）

## 1. 概述（Overview）

对象、函数和表达式都有一个称为**类型（type）** 的属性，它决定了存储在对象中或由表达式求值的二进制值的解释方式。

### 类型的作用

- **存储解释**：确定二进制数据如何被解释
- **操作约束**：定义可执行的操作
- **大小确定**：决定存储空间大小

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C89/C90 | 定义基础类型系统：基本类型、派生类型、枚举类型 |
| C99 | 新增 `_Bool`、`long long`、`_Complex`、`_Imaginary`、VLA |
| C11 | 新增原子类型（`_Atomic`）、`_Alignas`/`_Alignof` |
| C17 | 沿用既有类型系统 |
| C23 | 新增 `_BitInt(N)` 位精确整数、十进制浮点类型、`nullptr_t`、`alignof`/`alignas` |

### 新类型一览

| 类型 | 引入版本 | 说明 |
|------|----------|------|
| `_Bool` | C99 | 布尔类型 |
| `long long` | C99 | 扩展整数类型 |
| `_Complex` | C99 | 复数类型 |
| `_Imaginary` | C99 | 虚数类型 |
| `_Atomic` | C11 | 原子类型 |
| `_BitInt(N)` | C23 | 位精确整数 |
| `_Decimal32/64/128` | C23 | 十进制浮点类型 |
| `nullptr_t` | C23 | 空指针类型 |

## 3. 语法与参数（Syntax and Parameters）

### 3.1 类型分类

C 类型系统由以下类型组成：

```
类型
├── void 类型
├── 基本类型
│   ├── char 类型
│   ├── 有符号整数类型
│   │   ├── 标准：signed char, short, int, long, long long (C99 起)
│   │   ├── 位精确：_BitInt(N) (C23 起)
│   │   └── 扩展：__int128 等 (C99 起)
│   ├── 无符号整数类型
│   │   ├── 标准：_Bool (C99 起), unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long (C99 起)
│   │   ├── 位精确：unsigned _BitInt(N) (C23 起)
│   │   └── 扩展：__uint128 等 (C99 起)
│   └── 浮点类型
│       ├── 实浮点：float, double, long double
│       ├── 十进制浮点：_Decimal32, _Decimal64, _Decimal128 (C23 起)
│       ├── 复数：float _Complex, double _Complex, long double _Complex (C99 起)
│       └── 虚数：float _Imaginary, double _Imaginary, long double _Imaginary (C99 起)
├── 枚举类型
├── 派生类型
│   ├── 数组类型
│   ├── 结构体类型
│   ├── 联合体类型
│   ├── 函数类型
│   ├── 指针类型
│   └── 原子类型 (C11 起)
```

### 3.2 类型限定符

对于上述每种类型，可能存在其类型的多个限定版本，对应于 `const`、`volatile` 和 `restrict` 限定符的一个、两个或全部三个的组合（在限定符语义允许的情况下）。

### 3.3 类型分组

| 类型组 | 包含的类型 |
|--------|------------|
| 对象类型 | 所有非函数类型 |
| 字符类型 | `char`、`signed char`、`unsigned char` |
| 整数类型 | `char`、有符号整数类型、无符号整数类型、枚举类型 |
| 实类型 | 整数类型和实浮点类型 |
| 算术类型 | 整数类型和浮点类型 |
| 标量类型 | 算术类型、指针类型、`nullptr_t`（C23 起） |
| 聚合类型 | 数组类型和结构体类型 |
| 派生声明符类型 | 数组类型、函数类型、指针类型 |

### 3.4 类型名称

类型名称用于声明以外的上下文中，语法上与类型说明符和类型限定符列表相同，后跟声明符（省略标识符）：

```c
int n;                    // int 类型变量的声明
sizeof(int);              // 类型名称的使用

int *a[3];                // 3 个指向 int 的指针数组的声明
sizeof(int *[3]);         // 类型名称的使用

int (*p)[3];              // 指向 3 个 int 数组的指针的声明
sizeof(int (*)[3]);       // 类型名称的使用

int (*a)[*];              // 指向 VLA 的指针声明（函数参数中）
sizeof(int (*)[*]);       // 类型名称的使用（函数参数中）
```

**类型名称的使用场景**：
- 强制类型转换（cast）
- `sizeof` 运算符
- 复合字面量（C99 起）
- 泛型选择
- `_Alignof`/`alignof`（C11 起，C23 语法更新）
- `_Alignas`/`alignas`（C11 起，C23 语法更新）
- `_Atomic`（作为类型说明符时）（C11 起）

## 4. 底层原理（Underlying Principles）

### 4.1 兼容类型（Compatible Types）

在 C 程序中，引用同一对象或函数的不同翻译单元中的声明不必使用相同的类型，只需使用**足够相似**的类型，形式上称为兼容类型。

**类型 T 和 U 兼容的条件**：

| 条件 | 说明 |
|------|------|
| 相同类型 | 相同名称或 typedef 引入的别名 |
| 相同 cvr 限定 | 相同 cvr 限定版本的兼容非限定类型 |
| 指针类型 | 指向兼容类型 |
| 数组类型 | 元素类型兼容；若都有常量大小，则大小相同 |
| 结构体/联合体/枚举 | 标签相同；若为完整类型，成员完全对应 |
| 枚举与底层类型 | 一个是枚举类型，另一个是其底层类型 |
| 函数类型 | 返回类型兼容；参数列表兼容 |

**重要规则**：
- `char` 与 `signed char` 和 `unsigned char` 都不兼容
- 未知边界数组与任何兼容元素类型的数组兼容
- VLA 与任何兼容元素类型的数组兼容（C99 起）

```c
// 不同翻译单元中的兼容类型示例
// TU1
struct S { int a; };
extern struct S *x;

// TU2
struct S;
extern struct S *x;  // 与 TU1 兼容

// TU3
struct S { float a; };
extern struct S *x;  // 与 TU1 不兼容！
```

### 4.2 复合类型（Composite Types）

复合类型可从两个兼容类型构造，它是与两个类型都兼容的类型。

**数组类型的复合规则**：
1. 如果一个类型是已知常量大小的数组，复合类型是该大小的数组
2. 如果一个类型是指定大小的 VLA，复合类型是该大小的 VLA
3. 如果一个类型是未指定大小的 VLA，复合类型是未指定大小的 VLA
4. 否则，两个类型都是未知大小的数组，复合类型是未知大小的数组

```c
// 给定两个文件作用域声明：
int f(int (*)(), double (*)[3]);
int f(int (*)(char *), double (*)[]);
// 复合类型为：
int f(int (*)(char *), double (*)[3]);
```

### 4.3 不完整类型（Incomplete Types）

不完整类型是缺乏足够信息确定该类型对象大小的对象类型。

**不完整类型**：

| 类型 | 能否完成 | 完成方式 |
|------|----------|----------|
| `void` | 不能 | — |
| 未知大小数组 | 能 | 指定大小的声明 |
| 内容未知的结构体/联合体 | 能 | 定义内容的声明 |

```c
extern char a[];     // 不完整类型（通常在头文件中）
char a[10];          // 完整类型（通常在源文件中）

struct node {
    struct node* next;  // 此处 struct node 不完整
};                      // 此处 struct node 完整
```

### 4.4 对象表示大小限制

构造一个完整对象类型，使其对象表示的字节数无法用 `size_t` 类型表示（即 `sizeof` 运算符的结果类型），包括在运行时形成此类 VLA 类型（C99 起），是未定义行为。

## 5. 使用场景（Use Cases）

### 5.1 基本类型选择

| 需求 | 推荐类型 |
|------|----------|
| 布尔值 | `_Bool`（C99 起）或 `int` |
| 小整数 | `int` 或 `short` |
| 大整数 | `long long`（C99 起） |
| 字符 | `char`、`signed char` 或 `unsigned char` |
| 单精度浮点 | `float` |
| 双精度浮点 | `double` |
| 扩展精度 | `long double` |

### 5.2 派生类型使用

```c
// 数组类型
int arr[10];

// 结构体类型
struct Point {
    int x;
    int y;
};

// 联合体类型
union Data {
    int i;
    float f;
};

// 函数类型
int add(int a, int b);

// 指针类型
int *ptr;
```

### 5.3 类型限定符

```c
const int ci = 10;        // 只读
volatile int vi;          // 易变，禁止优化
int *restrict rp;         // 限制指针别名
const volatile int cvi;   // 组合限定
```

### 5.4 常见陷阱

1. **`char` 类型的符号性**：

```c
char c = 200;  // 实现定义：可能为负数
unsigned char uc = 200;  // 明确为 200
signed char sc = 200;    // 可能溢出
```

2. **类型兼容性错误**：

```c
// TU1
struct S { int a; };
extern struct S x;

// TU2
struct S { float a; };  // 不兼容！
extern struct S x;      // UB
```

3. **不完整类型使用错误**：

```c
struct Node;  // 不完整类型
struct Node* p = malloc(sizeof(struct Node));  // 错误：大小未知

struct Node {  // 完整类型
    int data;
    struct Node* next;
};
struct Node* p2 = malloc(sizeof(struct Node));  // OK
```

## 6. 代码示例（Examples）

### 基础类型使用

```c
#include <stdio.h>
#include <stdbool.h>  // C99 起

int main(void)
{
    // 整数类型
    char c = 'A';
    short s = 100;
    int i = 1000;
    long l = 100000L;
    long long ll = 10000000000LL;  // C99 起

    // 无符号整数类型
    unsigned int ui = 4000000000U;
    unsigned long long ull = 18000000000000000000ULL;

    // 浮点类型
    float f = 3.14f;
    double d = 3.14159265358979;
    long double ld = 3.141592653589793238L;

    // 布尔类型（C99 起）
    _Bool b1 = 1;
    bool b2 = true;  // 通过 stdbool.h

    printf("int: %d, long long: %lld\n", i, ll);
    printf("double: %.15f\n", d);
    printf("bool: %d\n", b2);

    return 0;
}
```

### 复数类型（C99 起）

```c
#include <stdio.h>
#include <complex.h>

int main(void)
{
    // 复数类型
    double complex z1 = 1.0 + 2.0 * I;
    double complex z2 = 3.0 - 4.0 * I;

    // 复数运算
    double complex sum = z1 + z2;
    double complex prod = z1 * z2;
    double complex conj_z1 = conj(z1);

    printf("z1 = %.2f + %.2fi\n", creal(z1), cimag(z1));
    printf("sum = %.2f + %.2fi\n", creal(sum), cimag(sum));
    printf("conj(z1) = %.2f + %.2fi\n", creal(conj_z1), cimag(conj_z1));

    return 0;
}
```

### 位精确整数（C23 起）

```c
#include <stdio.h>

int main(void)
{
    // 8 位精确整数
    _BitInt(8) small = 127;
    unsigned _BitInt(8) usmall = 255;

    // 128 位精确整数
    _BitInt(128) huge = 0;

    printf("8-bit: %d\n", (int)small);
    printf("unsigned 8-bit: %u\n", (unsigned)usmall);

    // 每个不同的 N 指定不同的类型
    _BitInt(16) a = 1000;
    // _BitInt(16) b = a;  // OK：相同类型
    // _BitInt(32) c = a;  // 需要转换

    return 0;
}
```

### 原子类型（C11 起）

```c
#include <stdio.h>
#include <stdatomic.h>

int main(void)
{
    // 原子整数
    atomic_int counter = 0;

    // 原子操作
    atomic_fetch_add(&counter, 1);
    int value = atomic_load(&counter);

    printf("Counter: %d\n", value);

    // 使用 _Atomic 类型说明符
    _Atomic float atomic_float = 3.14f;

    return 0;
}
```

### 兼容类型示例

```c
#include <stdio.h>

// 兼容的类型声明
extern int arr[];       // 不完整类型
int arr[10];            // 完整类型，兼容

// 兼容的函数声明
int func(int x, double y);       // 原型
int func(int x, double y) {      // 定义，兼容
    return x + (int)y;
}

// 不兼容的类型会导致 UB
// TU1:
// struct S { int a; };
// extern struct S x;
//
// TU2:
// struct S { float a; };  // 成员类型不同，不兼容
// extern struct S x;      // UB

int main(void)
{
    printf("arr[0] = %d\n", arr[0]);
    printf("func(1, 2.5) = %d\n", func(1, 2.5));
    return 0;
}
```

### 类型名称使用

```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // sizeof 中的类型名称
    printf("sizeof(int) = %zu\n", sizeof(int));
    printf("sizeof(int*[10]) = %zu\n", sizeof(int*[10]));

    // 强制类型转换中的类型名称
    double d = 3.14;
    int i = (int)d;

    // 复合字面量中的类型名称（C99 起）
    int *p = (int[]){1, 2, 3, 4, 5};
    printf("p[0] = %d\n", p[0]);

    // 泛型选择中的类型名称（C11 起）
    #define type_name(x) _Generic((x), \
        int: "int", \
        double: "double", \
        float: "float", \
        default: "unknown")

    int n = 10;
    printf("Type of n: %s\n", type_name(n));

    return 0;
}
```

### 常见错误及修正

```c
#include <stdio.h>
#include <stdlib.h>

/* 错误：char 类型的符号性不明确 */
void error_char_sign(void)
{
    char c = 200;  // 实现定义行为
    printf("char 200 = %d\n", c);  // 可能输出 -56
}

/* 正确：使用明确的类型 */
void correct_char_sign(void)
{
    unsigned char uc = 200;  // 明确为 200
    printf("unsigned char 200 = %d\n", uc);  // 输出 200
}

/* 错误：不完整类型使用 */
void error_incomplete_type(void)
{
    struct Node;  // 不完整类型声明
    // struct Node* p = malloc(sizeof(struct Node));  // 错误：大小未知
}

/* 正确：先完成类型 */
struct Node {
    int data;
    struct Node* next;
};

void correct_complete_type(void)
{
    struct Node* p = malloc(sizeof(struct Node));
    if (p) {
        p->data = 10;
        p->next = NULL;
        free(p);
    }
}

/* 错误：类型不兼容 */
// TU1: struct S { int a; }; extern struct S x;
// TU2: struct S { float a; }; extern struct S x;  // UB

/* 正确：使用相同的结构体定义 */
// 在共享头文件中：
// struct S { int a; };
// extern struct S x;

int main(void)
{
    error_char_sign();
    correct_char_sign();
    correct_complete_type();
    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **类型决定解释**：类型决定二进制值的解释方式
2. **类型分类**：void、基本类型、枚举类型、派生类型
3. **类型限定符**：`const`、`volatile`、`restrict`
4. **兼容类型**：不同翻译单元中声明同一对象/函数时需要兼容
5. **不完整类型**：`void`、未知大小数组、内容未知的结构体/联合体

### 类型系统概览

| 类别 | 类型 |
|------|------|
| 整数 | `char`、`short`、`int`、`long`、`long long`、`_Bool`、`_BitInt(N)` |
| 浮点 | `float`、`double`、`long double`、`_Decimal32/64/128` |
| 复数 | `float _Complex`、`double _Complex`、`long double _Complex` |
| 派生 | 数组、结构体、联合体、函数、指针、原子 |

### 与 C++ 的区别

| 特性 | C | C++ |
|------|---|-----|
| 兼容类型 | 有概念 | 无概念 |
| `_Bool` | C99 起支持 | 使用 `bool` |
| 复数类型 | 内置支持 | 通过 `std::complex` |
| VLA | C99 起支持 | 不支持（部分编译器扩展） |
| `_Generic` | C11 起支持 | 不支持（使用重载/模板） |

### 最佳实践

- 使用 `typedef` 提高类型可读性和可移植性
- 注意 `char` 类型的符号性，需要明确时使用 `signed char` 或 `unsigned char`
- 在头文件中共享类型定义，确保跨翻译单元类型兼容
- 使用 `<stdint.h>` 中的固定宽度整数类型提高可移植性
- 避免依赖实现定义的类型行为

### 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.2.5 Types, 6.2.6 Representations of types, 6.2.7 Compatible type and composite type
- C17 标准 (ISO/IEC 9899:2018): 6.2.5 Types (p: 31-33)
- C11 标准 (ISO/IEC 9899:2011): 6.2.5 Types (p: 39-43)
- C99 标准 (ISO/IEC 9899:1999): 6.2.5 Types (p: 33-37)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.1.2.5 Types