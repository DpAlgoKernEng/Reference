# 函数定义 (Function Definition)

## 1. 概述

**函数定义 (Function Definition)** 是 C 语言中将函数体（一系列声明和语句）与函数名及参数列表关联起来的语法结构。与函数声明不同，函数定义只能出现在文件作用域（file scope），C 语言不支持嵌套函数。

函数定义不仅引入了函数本身，还可能作为函数原型（function prototype）为后续的函数调用表达式提供类型检查和参数转换信息。

## 2. 来源与演变

### K&R C 时期

在 C 语言的最初版本（K&R C）中，函数定义采用"旧式"（old-style）语法：

```c
int max(a, b)
int a, b;
{
    return a > b ? a : b;
}
```

这种定义方式**不提供函数原型**，调用时会对参数执行默认参数提升（default argument promotions），缺乏类型安全性。

### C89 标准化

C89 引入了"新式"函数定义语法：

```c
int max(int a, int b)
{
    return a > b ? a : b;
}
```

这种定义同时充当函数原型，强制对参数进行类型转换。

### C99 变化

- 移除了隐式 `int` 返回类型（若省略类型说明符，不再默认为 `int`）
- 要求旧式定义中每个参数都必须有声明
- 引入 `__func__` 预定义标识符

### C23 变化

- 移除了旧式（K&R）函数定义
- 允许函数定义中使用未命名参数
- 新增属性说明符序列（attr-spec-seq）支持

### 版本对比表

| 特性 | C89 | C99 | C23 |
|------|-----|-----|-----|
| 新式函数定义 | 支持 | 支持 | 支持 |
| 旧式函数定义 | 支持 | 支持（废弃） | 移除 |
| 隐式 int 返回类型 | 支持 | 移除 | 移除 |
| 未命名参数 | 不允许 | 不允许 | 允许 |
| 函数属性 | 不支持 | 不支持 | 支持 |

## 3. 语法与参数

### 语法形式

C 语言支持两种函数定义形式：

**形式 1：新式函数定义（C89 起）**

```
attr-spec-seq(可选) 说明符与限定符 参数列表声明符 函数体
```

**形式 2：旧式函数定义（C23 前）**

```
说明符与限定符 标识符列表声明符 声明列表 函数体
```

### 语法元素说明

| 元素 | 说明 |
|------|------|
| `attr-spec-seq` | (C23) 可选的属性列表，应用于函数 |
| `说明符与限定符` | 类型说明符（形成返回类型）、存储类说明符（`static`、`extern` 或无）、函数说明符（`inline`、`_Noreturn` 或无）的组合 |
| `参数列表声明符` | 使用参数列表指定函数参数的声明符 |
| `标识符列表声明符` | 使用标识符列表指定函数参数的声明符（旧式） |
| `声明列表` | 声明标识符列表中每个标识符的声明序列，不能使用初始化器，唯一允许的存储类说明符是 `register` |
| `函数体` | 复合语句（花括号括起的声明和语句序列），函数被调用时执行 |

### 新式函数定义示例

```c
// 基本形式
int max(int a, int b)
{
    return a > b ? a : b;
}

// 无参数
double g(void)
{
    return 0.1;
}

// 返回指针
int* find(int arr[], int size, int value)
{
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return &arr[i];
        }
    }
    return NULL;
}
```

### 旧式函数定义示例（C23 前）

```c
// 旧式定义
int max(a, b)
int a, b;
{
    return a > b ? a : b;
}

// 无参数声明（默认 int 类型，C99 前有效）
double g()
{
    return 0.1;
}
```

### 参数规则

**返回类型规则**：
- 必须是完整的非数组对象类型或 `void` 类型
- 如果返回类型有 cvr 限定符，会被调整为无限定版本
- C99 前省略类型说明符默认为 `int`

**参数类型调整**：
- 函数类型参数调整为指向函数的指针
- 数组类型参数调整为指向首元素的指针
- 参数的顶层 cvr 限定符在确定兼容函数类型时被忽略

**未命名参数规则**：

| 版本 | 规则 |
|------|------|
| C23 前 | 参数必须有名字（`void` 除外），否则与旧式定义冲突 |
| C23 起 | 参数可以未命名，但在函数体内无法通过名称访问 |

```c
int f(int, int);                    // 声明：允许未命名参数
// int f(int, int) { return 7; }    // C23 前：错误；C23 起：允许
int f(int a, int b) { return 7; }   // 定义：命名参数
int g(void) { return 8; }           // 特殊情况：(void) 不声明参数
```

## 4. 底层原理

### 函数类型构建

函数定义过程中，编译器构建函数类型：

1. **返回类型确定**：由说明符与限定符中的类型说明符确定，可能被声明符修改
2. **参数类型确定**：
   - 新式定义：直接从参数列表获取
   - 旧式定义：从声明列表获取，或默认参数提升
3. **类型调整**：应用函数到指针、数组到指针的调整

```c
void f(char *s) { puts(s); }                     // 返回类型 void
int sum(int a, int b) { return a + b; }          // 返回类型 int
int (*foo(const void *p))[3] {                   // 返回类型：指向 int[3] 的指针
    return malloc(sizeof(int[3]));
}
```

### 参数存储与传递

在函数体内，每个命名参数：

- 是一个**左值表达式**（lvalue expression）
- 具有**自动存储期**（automatic storage duration）
- 具有**块作用域**（block scope）

**内存布局**：
- 参数在内存中的布局（或是否存储在内存中）是未指定的
- 这是**调用约定**（calling convention）的一部分
- 不同平台和编译器可能采用不同的传递方式（寄存器传递、栈传递等）

```c
int main(int ac, char **av)
{
    ac = 2;                                // 参数是左值，可以修改
    av = (char *[]){"abc", "def", NULL};   // 参数可重新赋值
    f(ac, av);
    return 0;
}
```

### __func__ 预定义标识符

C99 起，每个函数体内都有一个特殊的预定义变量 `__func__`：

```c
// 相当于在左花括号后立即定义
static const char __func__[] = "function name";
```

**用途**：
- 调试和日志记录
- 与 `__FILE__`、`__LINE__` 宏配合使用
- `assert` 宏内部使用此标识符

```c
#include <stdio.h>

void my_function(void)
{
    printf("Function: %s\n", __func__);    // 输出: Function: my_function
    printf("File: %s, Line: %d\n", __FILE__, __LINE__);
}
```

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 程序模块化 | 将功能分解为独立的函数单元 |
| 代码复用 | 避免重复代码，提高可维护性 |
| 类型安全 | 新式定义提供函数原型，确保调用类型正确 |
| 接口抽象 | 通过函数声明隐藏实现细节 |

### 最佳实践

**1. 始终使用新式函数定义**

```c
// 推荐：新式定义
int add(int a, int b)
{
    return a + b;
}

// 避免：旧式定义（C23 已移除）
int add(a, b)
int a, b;
{
    return a + b;
}
```

**2. 为无参数函数使用 `void`**

```c
// 推荐：明确表示无参数
int get_value(void)
{
    return 42;
}

// 避免：空参数列表（意义不同）
int get_value()           // 旧式声明，不提供原型信息
{
    return 42;
}
```

**3. 使用 `static` 限制作用域**

```c
// 文件内部函数，限制外部链接
static int internal_helper(int x)
{
    return x * 2;
}
```

**4. 合理使用 `inline` 提示**

```c
// 建议编译器内联展开
inline int square(int x)
{
    return x * x;
}
```

### 常见陷阱

**1. 返回类型不可为数组**

```c
// 错误：不能返回数组类型
int[10] get_array(void) { /* ... */ }

// 正确：返回指向数组的指针
int (*get_array(void))[10] { /* ... */ }
```

**2. 参数列表不能继承自 typedef**

```c
typedef int p(int q, int r);    // p 是函数类型 int(int, int)
p f { return q + r; }           // 错误：参数列表不能继承
int f(int q, int r) { return q + r; }  // 正确：显式写出参数
```

**3. 旧式定义的类型提升问题**

```c
// 旧式定义
void old_func(x)
double x;           // 期望 double
{
    printf("%f\n", x);
}

old_func(42);       // 问题：旧式定义不提供原型，int 被提升但不会转换为 double
```

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>

// 基本函数定义
int max(int a, int b)
{
    return a > b ? a : b;
}

// 无返回值函数
void print_message(const char *msg)
{
    printf("%s\n", msg);
}

// 无参数函数
int get_version(void)
{
    return 1;
}

int main(void)
{
    int result = max(10, 20);
    printf("Max: %d\n", result);           // 输出: Max: 20

    print_message("Hello, World!");         // 输出: Hello, World!

    printf("Version: %d\n", get_version()); // 输出: Version: 1

    return 0;
}
```

### 高级用法

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 返回指针的函数
int* create_array(int size, int init_value)
{
    int *arr = malloc(size * sizeof(int));
    if (arr != NULL) {
        for (int i = 0; i < size; i++) {
            arr[i] = init_value;
        }
    }
    return arr;
}

// 使用 static 的文件作用域函数
static int internal_counter = 0;

static void increment_counter(void)
{
    internal_counter++;
}

int get_counter(void)
{
    increment_counter();
    return internal_counter;
}

// 使用 inline 的函数
inline double celsius_to_fahrenheit(double celsius)
{
    return celsius * 9.0 / 5.0 + 32.0;
}

// 使用 _Noreturn 的函数（C11）
#include <stdlib.h>
_Noreturn void fatal_error(const char *msg)
{
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(1);
}

int main(void)
{
    // 使用返回指针的函数
    int *arr = create_array(5, 10);
    if (arr != NULL) {
        for (int i = 0; i < 5; i++) {
            printf("%d ", arr[i]);  // 输出: 10 10 10 10 10
        }
        printf("\n");
        free(arr);
    }

    // 使用计数器
    printf("Counter: %d\n", get_counter());  // 输出: Counter: 1
    printf("Counter: %d\n", get_counter());  // 输出: Counter: 2

    // 使用 inline 函数
    printf("100C = %.1fF\n", celsius_to_fahrenheit(100.0));  // 输出: 100C = 212.0F

    return 0;
}
```

### 常见错误及修正

#### 错误 1：旧式定义导致类型不匹配

```c
/* 错误示例：旧式定义不提供原型 */
#include <stdio.h>

double square(x)     /* 旧式定义，无原型信息 */
double x;
{
    return x * x;
}

int main(void)
{
    /* 问题：编译器不知道 square 期望 double */
    printf("%f\n", square(3));     /* 未定义行为：int 被传递但期望 double */
    return 0;
}

/* 正确做法：使用新式定义 */
double square(double x)    /* 新式定义，提供原型 */
{
    return x * x;
}

int main(void)
{
    printf("%f\n", square(3));     /* 正确：int 自动转换为 double */
    return 0;
}
```

#### 错误 2：未命名参数无法访问

```c
/* C23 起：未命名参数示例 */
int add(int a, int, int c)    /* 第二个参数未命名 */
{
    /* return a + b + c; */   /* 错误：b 未声明，无法访问 */
    return a + c;             /* 只能访问命名参数 */
}

/* 如果需要使用参数，必须命名 */
int add_correct(int a, int b, int c)
{
    return a + b + c;
}
```

#### 错误 3：返回局部变量地址

```c
/* 错误示例：返回局部变量地址 */
int* get_local_pointer(void)
{
    int local = 42;
    return &local;    /* 错误：返回局部变量地址 */
}                      /* local 在此处销毁 */

/* 正确做法：返回静态变量或动态分配 */
int* get_valid_pointer(void)
{
    static int local = 42;    /* 静态存储期 */
    return &local;
}

int* get_dynamic_pointer(void)
{
    int *ptr = malloc(sizeof(int));
    if (ptr != NULL) {
        *ptr = 42;
    }
    return ptr;    /* 调用者负责释放 */
}
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| 定义位置 | 仅限文件作用域，不支持嵌套函数 |
| 语法形式 | 新式（推荐）和旧式（C23 已移除） |
| 原型作用 | 新式定义充当函数原型，提供类型安全 |
| 参数特性 | 自动存储期、块作用域、左值表达式 |
| 版本演进 | C23 移除旧式定义，允许未命名参数 |

### 技术对比

| 特性 | 新式定义 | 旧式定义 |
|------|---------|---------|
| 函数原型 | 提供 | 不提供 |
| 类型检查 | 编译时检查 | 无检查 |
| 参数转换 | 自动转换 | 默认提升 |
| 参数列表 | 类型+名称 | 仅名称，类型后声明 |
| C23 状态 | 支持 | 已移除 |

### 学习建议

1. **始终使用新式函数定义**：获得类型安全和参数转换的好处
2. **理解参数传递机制**：了解值传递、指针传递的区别
3. **注意存储类说明符**：`static` 限制链接、`extern` 声明外部定义
4. **善用 `__func__`**：调试时输出函数名
5. **避免旧式语法**：虽然 C23 前支持，但不推荐使用

### 相关概念

| 概念 | 关系 |
|------|------|
| 函数声明 | 声明函数签名，不提供函数体 |
| 函数原型 | 提供类型信息的函数声明 |
| 函数调用 | 通过函数表达式执行函数体 |
| 内联函数 | 使用 `inline` 建议编译器内联展开 |

## 参考资料

- C17 标准 (ISO/IEC 9899:2018): 6.9.1 Function definitions
- C11 标准 (ISO/IEC 9899:2011): 6.9.1 Function definitions
- C99 标准 (ISO/IEC 9899:1999): 6.9.1 Function definitions
- C89/C90 标准 (ISO/IEC 9899:1990): 3.7.1 Function definitions