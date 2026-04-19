# 常量表达式 (Constant Expression)

## 1. 概述 (Overview)

**常量表达式 (Constant Expression)** 是指在编译期间就能确定其值的表达式。C 语言中定义了多种类型的常量表达式，它们在不同的上下文中有不同的用途和限制。

### 主要类型

常量表达式主要分为以下几类：

1. **预处理器常量表达式** - 用于 `#if` 和 `#elif` 指令
2. **整型常量表达式** - 用于数组大小、位域宽度、case 标签等
3. **静态初始化器** - 用于静态存储期对象的初始化
4. **浮点常量表达式** - 浮点类型的算术常量表达式

### 技术定位

常量表达式是 C 语言编译时计算的核心机制，它允许编译器在编译阶段确定表达式的值，从而实现更高效的代码生成和更强的类型安全保证。常量表达式广泛应用于编译时决策、静态初始化、数组边界定义等场景。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

常量表达式的概念从 C 语言的早期版本就存在，主要用于支持编译时的常量计算，使编译器能够在编译阶段进行优化和错误检查。

### 版本演变

| C 标准版本 | 主要变更 |
|-----------|---------|
| C89/C90 | 定义了基本的常量表达式规则 |
| C99 | 引入 VLA（变长数组），允许在 sizeof 运算符中使用非 VLA 操作数；数组指示符中的索引可以是常量表达式 |
| C11 | 新增 `_Alignof`、`_Static_assert`，要求整型常量表达式 |
| C23 | 用 `alignof` 替代 `_Alignof`；引入 `constexpr` 存储类说明符；支持 `typeof`/`typeof_unqual` 运算符；引入命名常量和复合字面量常量 |

### 解决的问题

- **编译时计算**：允许编译器在编译阶段确定值，减少运行时开销
- **类型安全**：在编译期检查数组边界、位域宽度等是否合法
- **预处理器条件**：支持条件编译，根据常量表达式的结果选择编译路径

## 3. 语法与参数 (Syntax and Parameters)

### 预处理器常量表达式

用于 `#if` 或 `#elif` 指令后的表达式，可包含：

- **运算符**：除赋值、自增、自减、函数调用、逗号之外的运算符
- **整型常量**：如 `42`、`0xFF`
- **字符常量**：如 `'a'`、`'\n'`
- **特殊预处理器运算符**：`defined`

```c
#if defined(DEBUG) && DEBUG > 0
    // 调试代码
#elif VERSION >= 2
    // 版本 2+ 代码
#endif
```

**注意事项**：

- 在 `#if` 表达式中，字符常量可能在源字符集、执行字符集或其他实现定义的字符集中解释
- C99 起，整型运算使用 `intmax_t`（有符号类型）和 `uintmax_t`（无符号类型）的语义

### 整型常量表达式

整型常量表达式是只包含以下内容的表达式：

| 允许的元素 | 说明 |
|-----------|------|
| 运算符 | 除赋值、自增、自减、函数调用、逗号之外的运算符；强制转换运算符只能将算术类型转换为整型，除非作为 `sizeof`、`_Alignof`(C11 起，C23 前)、`alignas`(C23 起)、`typeof`/`typeof_unqual`(C23 起) 的操作数 |
| 整型常量 | 如 `100`、`0x10` |
| 枚举常量 | 枚举类型的命名常量 |
| 字符常量 | 如 `'A'`、`L'B'` |
| 浮点常量 | 仅当立即用作到整型的强制转换的操作数时 |
| sizeof 运算符 | 操作数不能是 VLA（变长数组，C99 起） |
| _Alignof / alignas | C11 起 |
| 命名常量和复合字面量常量 | C23 起，整型或算术类型 |

**使用场景**（需要整型常量表达式的上下文）：

```c
// 位域宽度
struct flags {
    unsigned int ready : 1;    // 1 是整型常量表达式
    unsigned int mode : 3;     // 3 是整型常量表达式
};

// 枚举常量值
enum { MAX_SIZE = 100 };       // 100 是整型常量表达式

// switch 的 case 标签
switch (ch) {
    case 'A':                  // 'A' 是整型常量表达式
        break;
}

// 非VLA数组大小
int array[10];                 // 10 是整型常量表达式
```

**C99 新增场景**：

```c
// 数组指示符中的索引
int arr[10] = { [5] = 100 };   // 5 必须是整型常量表达式
```

**C11 新增场景**：

```c
// _Static_assert 的第一个参数
_Static_assert(sizeof(int) == 4, "int must be 4 bytes");

// _Alignof 的整型参数（C23 前为 _Alignof，C23 起为 alignof）
size_t alignment = alignof(int);
```

**C23 新增场景**：

```c
// 位精确整型的位数 N
_BitInt(32) x;                 // 32 必须是整型常量表达式
```

### 静态初始化器

用于静态存储期对象或线程存储期对象（以及 C23 起 `constexpr` 声明的对象）的初始化器表达式，必须是字符串字面量或以下形式之一：

#### 1. 算术常量表达式

算术类型的表达式，包含：

- 运算符（除赋值、自增、自减、函数调用、逗号外），强制转换只能在算术类型之间，除非作为 `sizeof`、`alignof` 等的操作数
- 整型常量、浮点常量、枚举常量、字符常量
- `sizeof` 运算符（操作数不能是 VLA）
- C11 起：`alignof` 运算符
- C23 起：命名常量或复合字面量常量（算术类型）

```c
static int x = 10 + 20;           // 算术常量表达式
static double pi = 3.14159;       // 算术常量表达式
static float val = sizeof(int);   // sizeof 结果是整型常量
```

#### 2. 空指针常量

```c
static int *ptr = NULL;            // 空指针常量
static char *str = 0;              // 整数常量 0 转换为空指针
```

#### 3. 地址常量表达式

- 空指针
- 指向静态存储期对象的左值或函数指示符，通过以下方式转换：
  - 一元取地址运算符 `&`
  - 将整数常量转换为指针
  - 数组到指针或函数到指针的隐式转换

```c
static int global_var;
static int *p1 = &global_var;      // 取地址运算符
static int *p2 = (int *)0x1000;    // 整数常量转换为指针

void func(void);
static void (*func_ptr)(void) = func; // 函数到指针隐式转换
```

#### 4. 地址常量表达式加减整型常量表达式

```c
static int arr[10];
static int *p = arr + 2;           // 地址常量 + 整型常量表达式
static int *q = &arr[5] - 1;       // 地址常量 - 整型常量表达式
```

#### 5. 命名常量 (C23 起)

```c
constexpr int max_val = 100;
static int arr[max_val];           // 命名常量作为数组大小

constexpr struct Point { int x, y; } origin = {0, 0};
static int px = origin.x;          // 成员访问运算符应用于命名常量
```

**结构或联合常量**：命名常量或复合字面量常量如果是结构或联合类型，访问成员时必须访问初始化时指定的成员。

#### 6. 复合字面量常量 (C23 起)

```c
static int *p = (constexpr int[]){1, 2, 3};
```

#### 7. 实现定义的其他形式常量表达式

### 浮点常量表达式

不在静态初始化器中使用的浮点类型算术常量表达式，总是像运行时一样求值：

- 受当前舍入模式影响（如果 `FENV_ACCESS` 开启）
- 按 `math_errhandling` 规定报告错误

## 4. 底层原理 (Underlying Principles)

### 编译时求值

整型常量表达式在**编译时**求值，这有以下含义：

1. **性能优化**：编译器可以预先计算值，避免运行时计算开销
2. **编译时检查**：编译器可以在编译阶段发现错误（如数组大小为负数或零）
3. **条件编译**：预处理器可以根据常量表达式的结果选择编译路径

### 静态初始化器的特殊处理

与整型常量表达式不同，静态初始化器表达式**不要求**在编译时求值。编译器可以选择：

- 在编译时计算初始值
- 生成在程序启动前执行的初始化代码

```c
static int i = 2 || 1 / 0;   // 初始化 i 为 1，不产生运行时错误
                             // 因为短路求值在编译时也适用
```

### 浮点常量表达式的精度保证

静态初始化器中的浮点表达式精度保证：

- 不低于运行时计算的精度
- 可能比运行时计算更精确（编译器可能使用更高精度）

```c
static double x = 0.1;   // 编译时可能使用更高精度
double y = 0.1;          // 运行时精度
```

### 求值时机对比

| 表达式类型 | 求值时机 | 编译器行为 |
|-----------|---------|-----------|
| 预处理器常量表达式 | 编译时（预处理阶段） | 必须在编译时求值 |
| 整型常量表达式 | 编译时 | 必须在编译时求值 |
| 静态初始化器 | 编译时或启动时 | 可选择 |
| 浮点常量表达式（非静态初始化器） | 运行时 | 作为运行时代码 |

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 条件编译

```c
#define VERSION 3

#if VERSION >= 3
    // 版本 3+ 的代码
#elif VERSION >= 2
    // 版本 2 的代码
#else
    // 旧版本代码
#endif
```

#### 2. 数组大小定义

```c
#define MAX_ITEMS 100
int buffer[MAX_ITEMS];  // 整型常量表达式确定数组大小
```

#### 3. 位域定义

```c
struct Status {
    unsigned int flags : 4;    // 4 是整型常量表达式
    unsigned int mode : 2;     // 2 是整型常量表达式
};
```

#### 4. 枚举值定义

```c
enum {
    BUFFER_SIZE = 1024,    // 整型常量表达式
    MAX_CONNECTIONS = 10   // 整型常量表达式
};
```

#### 5. switch-case 语句

```c
switch (op) {
    case '+':   // 整型常量表达式
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
}
```

#### 6. 静态变量初始化

```c
static int counter = 0;
static double scale = 1.0 / 3.0;  // 算术常量表达式
```

### 最佳实践

1. **使用宏或枚举代替魔法数字**

```c
// 推荐
#define MAX_BUFFER 1024
int buffer[MAX_BUFFER];

// 不推荐
int buffer[1024];
```

2. **利用 constexpr (C23) 增强类型安全**

```c
// C23 起
constexpr int max_size = 100;
static int data[max_size];
```

3. **注意短路求值**

```c
static int safe = 0 || 1 / 0;  // 安全：短路求值避免了除零
```

### 常见陷阱

#### 1. VLA 不是常量表达式

```c
int n = 10;
int arr[n];           // VLA，不是常量表达式数组
sizeof(arr);          // 运行时求值，不是整型常量表达式

// 正确做法
#define N 10
int arr[N];           // 整型常量表达式数组
sizeof(arr);          // 编译时求值
```

#### 2. 函数调用不是常量表达式

```c
// 错误：即使函数返回常量值，也不是常量表达式
int get_size(void) { return 100; }
int arr[get_size()];  // 错误：函数调用不是常量表达式

// 正确做法
enum { SIZE = 100 };
int arr[SIZE];
```

#### 3. 静态初始化器中的除零陷阱

```c
// 运行时会崩溃，但静态初始化器可能不会
void func(void) {
    float x = 0.0 / 0.0;  // 运行时产生异常
}
```

#### 4. 实现定义行为的陷阱

```c
// 字符常量的解释可能是实现定义的
#if 'A' == 65
    // 不一定在所有平台上成立
#endif
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：预处理器常量表达式

```c
#include <stdio.h>

#define DEBUG 1
#define VERSION 3

int main(void) {
#if defined(DEBUG) && DEBUG > 0
    printf("Debug mode enabled\n");
#endif

#if VERSION >= 3
    printf("Using version 3+ features\n");
#elif VERSION >= 2
    printf("Using version 2 features\n");
#else
    printf("Using legacy features\n");
#endif

    return 0;
}
```

#### 示例 2：整型常量表达式

```c
#include <stdio.h>

#define MAX_SIZE 100

// 整型常量表达式用于数组大小
static int buffer[MAX_SIZE];

// 整型常量表达式用于位域
struct Flags {
    unsigned int readable : 1;
    unsigned int writable : 1;
    unsigned int executable : 1;
};

// 整型常量表达式用于枚举
enum {
    MIN_SIZE = 1,
    DEFAULT_SIZE = 50,
    MAX_ITEMS = MAX_SIZE
};

int main(void) {
    int choice = 2;

    // 整型常量表达式用于 case 标签
    switch (choice) {
        case 1:
            printf("Option 1\n");
            break;
        case 2:
            printf("Option 2\n");
            break;
        default:
            printf("Unknown option\n");
    }

    printf("Array size: %zu\n", sizeof(buffer) / sizeof(buffer[0]));
    printf("Max items: %d\n", MAX_ITEMS);

    return 0;
}
```

#### 示例 3：静态初始化器

```c
#include <stdio.h>
#include <stddef.h>

static int global_var = 100;

// 算术常量表达式
static double pi = 3.14159;
static int sum = 10 + 20;

// 空指针常量
static int *null_ptr = NULL;

// 地址常量表达式
static int *ptr_to_global = &global_var;

// 地址常量 + 整型常量表达式
static int arr[10];
static int *ptr_in_arr = arr + 5;

int main(void) {
    printf("pi = %f\n", pi);
    printf("sum = %d\n", sum);
    printf("global_var = %d\n", *ptr_to_global);

    return 0;
}
```

### 高级用法

#### 示例 4：C23 constexpr 用法

```c
// C23 标准
#include <stdio.h>

// 命名常量
constexpr int MAX_BUFFER = 1024;
constexpr double SCALE_FACTOR = 1.5;

// 结构类型的常量
struct Point {
    int x, y;
};

constexpr struct Point origin = {0, 0};

int main(void) {
    int buffer[MAX_BUFFER];  // 使用命名常量作为数组大小

    printf("Buffer size: %d\n", MAX_BUFFER);
    printf("Origin: (%d, %d)\n", origin.x, origin.y);

    return 0;
}
```

#### 示例 5：静态初始化器的短路求值

```c
#include <stdio.h>

// 安全：短路求值避免了除零错误
static int safe_value = 0 || 1 / 0;  // 结果为 1，不执行 1/0

int main(void) {
    printf("safe_value = %d\n", safe_value);
    return 0;
}
```

#### 示例 6：浮点常量表达式的行为

```c
#include <stdio.h>
#include <fenv.h>

#pragma STDC FENV_ACCESS ON

void test_floating_point(void) {
    // 静态初始化器：不产生异常
    static float static_x = 0.0 / 0.0;

    // 非静态初始化器：产生异常
    float arr[] = { 0.0 / 0.0 };    // 产生异常
    float y = 0.0 / 0.0;           // 产生异常
    double z = 0.0 / 0.0;          // 产生异常
}
```

### 常见错误及修正

#### 错误 1：使用非常量表达式作为数组大小

```c
// 错误：n 不是常量表达式
int n = 10;
int arr[n];  // VLA，可能不被某些编译器支持

// 修正方案 1：使用宏
#define N 10
int arr[N];

// 修正方案 2：使用枚举
enum { N = 10 };
int arr[N];

// 修正方案 3 (C23)：使用 constexpr
constexpr int n = 10;
int arr[n];
```

#### 错误 2：函数调用在常量表达式中

```c
// 错误：函数调用不是常量表达式
int get_size(void) { return 100; }
static int arr[get_size()];  // 编译错误

// 修正：使用宏或枚举
#define GET_SIZE 100
static int arr[GET_SIZE];
```

#### 错误 3：自增/自减运算符在常量表达式中

```c
// 错误：自增运算符不允许在常量表达式中
int x = 5;
static int y = ++x;  // 错误

// 修正：使用算术运算
#define X 5
static int y = X + 1;
```

#### 错误 4：逗号运算符在常量表达式中

```c
// 错误：逗号运算符不允许在常量表达式中
static int x = (1, 2);  // 错误

// 修正：直接使用值
static int x = 2;
```

#### 错误 5：联合常量的成员访问

```c
// C23 错误示例
union Data {
    int i;
    float f;
};

constexpr union Data u = { .i = 10 };
// 错误：访问的成员不是初始化的成员
int val = u.f;  // 未定义行为

// 修正：访问正确的成员
int val = u.i;  // 正确
```

## 7. 总结 (Summary)

### 核心要点

| 类型 | 求值时机 | 主要用途 | 关键限制 |
|------|---------|---------|---------|
| 预处理器常量表达式 | 编译时（预处理阶段） | 条件编译 | 不能使用赋值、自增、自减、函数调用、逗号运算符 |
| 整型常量表达式 | 编译时 | 数组大小、位域宽度、case 标签 | 强制转换受限，不能是 VLA 的 sizeof |
| 静态初始化器 | 编译时或启动时 | 静态存储期对象初始化 | 必须是算术常量、空指针或地址常量 |
| 浮点常量表达式 | 运行时 | 浮点运算 | 受舍入模式和错误处理影响 |

### 版本演进关键点

- **C99**：引入 VLA，`sizeof` 操作数不能是 VLA；数组指示符索引需为整型常量表达式
- **C11**：新增 `_Static_assert`、`_Alignof`，需要整型常量表达式
- **C23**：引入 `constexpr` 存储类说明符、`alignof`、`typeof`/`typeof_unqual`；支持命名常量和复合字面量常量

### 技术对比

| 特性 | C89/C90 | C99 | C11 | C23 |
|------|---------|-----|-----|-----|
| 基本常量表达式 | ✓ | ✓ | ✓ | ✓ |
| VLA | - | ✓ | ✓ | ✓ |
| `_Static_assert` | - | - | ✓ | ✓ |
| `_Alignof`/`alignof` | - | - | ✓ | ✓ |
| `constexpr` | - | - | - | ✓ |
| `typeof`/`typeof_unqual` | - | - | - | ✓ |
| 命名常量 | - | - | - | ✓ |

### 学习建议

1. **理解编译时与运行时的区别**：常量表达式的核心价值在于编译时求值，理解这一点对于编写高效代码至关重要。

2. **注意版本差异**：不同 C 标准对常量表达式的规定有所不同，特别是 C23 引入了重要的新特性。

3. **掌握允许的运算符和操作数**：记住哪些运算符（赋值、自增、自减、函数调用、逗号）不允许在常量表达式中使用。

4. **实践最佳实践**：使用宏、枚举或 `constexpr`（C23）代替魔法数字，提高代码可读性和可维护性。

5. **了解实现定义行为**：某些情况下（如 `#if` 中的字符常量），编译器可能有不同的解释方式。

### 参考

- C23 标准 (ISO/IEC 9899:2024): 6.6 Constant expressions
- C17 标准 (ISO/IEC 9899:2018): 6.6 Constant expressions (p: 76-77)
- C11 标准 (ISO/IEC 9899:2011): 6.6 Constant expressions (p: 106-107)
- C99 标准 (ISO/IEC 9899:1999): 6.6 Constant expressions (p: 95-96)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.4 CONSTANT EXPRESSIONS