# C 语言关键字 (C Keywords)

## 1. 概述 (Overview)

**关键字 (Keywords)** 是 C 语言中具有特殊含义的保留标识符。由于它们被语言本身使用，这些关键字不能被重新定义。作为一个例外，在 C23 标准的属性标记 (attribute-tokens) 中，它们不被视为保留字。

### 关键字特点

- **保留性**：关键字不能用作变量名、函数名或其他标识符
- **版本演进**：不同 C 标准版本引入了不同的关键字
- **替代拼写**：部分关键字有下划线前缀的替代形式

## 2. 来源与演变 (Origin and Evolution)

### 标准演进历程

| C 标准版本 | 新增关键字数量 | 主要新增关键字 |
|-----------|---------------|---------------|
| C89/C90 | 32 个基础关键字 | `int`, `char`, `if`, `else`, `for`, `while` 等 |
| C99 | 5 个 | `inline`, `restrict`, `_Bool`, `_Complex`, `_Imaginary` |
| C11 | 7 个 | `_Alignas`, `_Alignof`, `_Atomic`, `_Generic`, `_Noreturn`, `_Static_assert`, `_Thread_local` |
| C23 | 14 个 | `alignas`, `alignof`, `bool`, `true`, `false`, `nullptr`, `constexpr`, `static_assert`, `thread_local`, `typeof`, `typeof_unqual`, `_BitInt`, `_Decimal32`, `_Decimal64`, `_Decimal128` |

### 设计动机

1. **类型安全增强** - 引入 `_Bool`、`_Complex` 等类型关键字
2. **并发支持** - C11 引入 `_Atomic`、`_Thread_local` 支持多线程编程
3. **泛型编程** - C11 的 `_Generic` 实现类型泛型选择
4. **现代特性** - C23 引入 `constexpr`、`nullptr` 等现代编程特性
5. **可读性改进** - C23 将下划线前缀关键字标准化为更易读的形式

### 标准参考

| 标准 | 文档章节 |
|-----|---------|
| C23 | 6.4.1 Keywords |
| C17 | 6.4.1 Keywords (p: 42-43) |
| C11 | 6.4.1 Keywords (p: 58-59) |
| C99 | 6.4.1 Keywords (p: 50) |
| C89/C90 | 3.1.1 Keywords |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 核心关键字列表

#### 数据类型关键字

| 关键字 | 引入版本 | 说明 |
|-------|---------|------|
| `void` | C89 | 空类型 |
| `char` | C89 | 字符类型 |
| `short` | C89 | 短整型 |
| `int` | C89 | 整型 |
| `long` | C89 | 长整型 |
| `float` | C89 | 单精度浮点型 |
| `double` | C89 | 双精度浮点型 |
| `signed` | C89 | 有符号类型修饰符 |
| `unsigned` | C89 | 无符号类型修饰符 |
| `_Bool` | C99 | 布尔类型（C23 中弃用） |
| `bool` | C23 | 布尔类型 |
| `_Complex` | C99 | 复数类型 |
| `_Decimal32` | C23 | 32位十进制浮点 |
| `_Decimal64` | C23 | 64位十进制浮点 |
| `_Decimal128` | C23 | 128位十进制浮点 |
| `_BitInt(N)` | C23 | 指定位宽的整数类型 |

#### 存储类关键字

| 关键字 | 引入版本 | 说明 |
|-------|---------|------|
| `auto` | C89 | 自动存储期（C23 中语义变更） |
| `register` | C89 | 寄存器存储类（现代 C 中忽略） |
| `static` | C89 | 静态存储期/内部链接 |
| `extern` | C89 | 外部链接声明 |
| `typedef` | C89 | 类型别名定义 |
| `thread_local` | C23 | 线程局部存储 |
| `_Thread_local` | C11 | 线程局部存储（C23 中弃用） |

#### 类型限定符

| 关键字 | 引入版本 | 说明 |
|-------|---------|------|
| `const` | C89 | 常量限定符 |
| `volatile` | C89 | 易变限定符 |
| `restrict` | C99 | 限制指针别名 |
| `_Atomic` | C11 | 原子类型限定符 |

#### 控制流关键字

| 关键字 | 引入版本 | 说明 |
|-------|---------|------|
| `if` | C89 | 条件语句 |
| `else` | C89 | 条件分支 |
| `switch` | C89 | 多分支选择 |
| `case` | C89 | switch 分支标签 |
| `default` | C89 | switch 默认分支 |
| `for` | C89 | for 循环 |
| `while` | C89 | while 循环 |
| `do` | C89 | do-while 循环 |
| `break` | C89 | 跳出循环/switch |
| `continue` | C89 | 继续下一次迭代 |
| `goto` | C89 | 无条件跳转 |
| `return` | C89 | 函数返回 |

#### 复合类型关键字

| 关键字 | 引入版本 | 说明 |
|-------|---------|------|
| `struct` | C89 | 结构体 |
| `union` | C89 | 联合体 |
| `enum` | C89 | 枚举 |

#### 函数相关关键字

| 关键字 | 引入版本 | 说明 |
|-------|---------|------|
| `inline` | C99 | 内联函数建议 |
| `_Noreturn` | C11 | 不返回函数（C23 中弃用） |

#### 其他关键字

| 关键字 | 引入版本 | 说明 |
|-------|---------|------|
| `sizeof` | C89 | 获取大小运算符 |
| `_Generic` | C11 | 泛型选择表达式 |
| `alignas` | C23 | 对齐指定符 |
| `alignof` | C23 | 对齐查询运算符 |
| `constexpr` | C23 | 常量表达式 |
| `static_assert` | C23 | 编译时断言 |
| `typeof` | C23 | 类型查询 |
| `typeof_unqual` | C23 | 无限定符类型查询 |
| `true` | C23 | 布尔真值 |
| `false` | C23 | 布尔假值 |
| `nullptr` | C23 | 空指针常量 |

### 3.2 便捷宏与头文件

以下下划线前缀关键字通常通过便捷宏使用：

| 关键字 | 便捷宏 | 定义头文件 | 版本信息 |
|-------|-------|-----------|---------|
| `_Alignas` | `alignas` | `<stdalign.h>` | C11 引入，C23 中宏被移除 |
| `_Alignof` | `alignof` | `<stdalign.h>` | C11 引入，C23 中宏被移除 |
| `_Bool` | `bool` | `<stdbool.h>` | C99 引入，C23 中宏被移除 |
| `_Complex` | `complex` | `<complex.h>` | C99 引入 |
| `_Imaginary` | `imaginary` | `<complex.h>` | C99 引入 |
| `_Noreturn` | `noreturn` | `<stdnoreturn.h>` | C11 引入，C23 中弃用 |
| `_Static_assert` | `static_assert` | `<assert.h>` | C11 引入，C23 中宏被移除 |
| `_Thread_local` | `thread_local` | `<threads.h>` | C11 引入，C23 中宏被移除 |

### 3.3 预处理器指令关键字

在预处理指令上下文中识别的关键字：

| 关键字 | 引入版本 | 说明 |
|-------|---------|------|
| `if` | C89 | 条件编译 |
| `elif` | C89 | 否则如果 |
| `else` | C89 | 否则 |
| `endif` | C89 | 结束条件 |
| `ifdef` | C89 | 如果定义 |
| `ifndef` | C89 | 如果未定义 |
| `elifdef` | C23 | 否则如果定义 |
| `elifndef` | C23 | 否则如果未定义 |
| `define` | C89 | 宏定义 |
| `undef` | C89 | 取消宏定义 |
| `include` | C89 | 文件包含 |
| `embed` | C23 | 资源嵌入 |
| `line` | C89 | 行号控制 |
| `error` | C89 | 生成错误 |
| `warning` | C23 | 生成警告 |
| `pragma` | C89 | 编译器指令 |
| `defined` | C89 | 检查宏定义 |
| `__has_include` | C23 | 检查头文件存在 |
| `__has_embed` | C23 | 检查资源嵌入能力 |
| `__has_c_attribute` | C23 | 检查属性支持 |

## 4. 底层原理 (Underlying Principles)

### 4.1 关键字的保留规则

**标识符保留规则**：
- 双下划线 `__` 开头的名称始终保留
- 单下划线 `_` 后跟大写字母开头的名称保留
- 单下划线 `_` 开头的名称在全局作用域保留

**属性标记例外**（C23 起）：
- 在属性标记中，关键字不被视为保留字
- 允许在属性中使用与关键字同名的属性

### 4.2 替代拼写的实现

```c
// C23 之前的写法
#include <stdbool.h>
bool b = true;  // bool 是宏，展开为 _Bool

// C23 的写法
bool b = true;  // bool 是关键字，不需要头文件
```

C23 将部分便捷宏标准化为关键字，但保留了下划线前缀形式以实现向后兼容。

### 4.3 二字符组和三字符组

以下二字符组提供标准标记的替代表示：

| 二字符组 | 等价于 |
|---------|-------|
| `<%` | `{` |
| `%>` | `}` |
| `<:` | `[` |
| `:>` | `]` |
| `%:` | `#` |
| `%:%:` | `##` |

## 5. 使用场景 (Use Cases)

### 5.1 数据类型定义

```c
// 基本类型
int count = 10;
char letter = 'A';
float price = 19.99;
double precision = 3.14159265358979;

// 类型修饰
unsigned long big_number = 4294967295UL;
const char* message = "Hello";

// C23 新类型
bool flag = true;
_BitInt(128) huge_number = 0;
```

### 5.2 存储类管理

```c
// 静态变量
static int counter = 0;  // 文件内可见，持久存储

// 外部链接
extern int global_var;   // 声明外部变量

// 线程局部存储（C11/C23）
thread_local int tls_var = 0;

// 类型定义
typedef unsigned int uint;
```

### 5.3 控制流结构

```c
// 条件语句
if (condition) {
    // ...
} else {
    // ...
}

// 循环
for (int i = 0; i < 10; i++) {
    if (i == 5) continue;
    if (i == 8) break;
}

// 多分支
switch (value) {
    case 1: handle_one(); break;
    case 2: handle_two(); break;
    default: handle_default();
}
```

### 5.4 复合类型定义

```c
// 结构体
struct Point {
    int x;
    int y;
};

// 联合体
union Data {
    int i;
    float f;
    char str[4];
};

// 枚举
enum Color {
    RED,
    GREEN,
    BLUE
};
```

### 5.5 C11/C23 新特性

```c
// 泛型选择（C11）
#define print(x) _Generic((x), \
    int: print_int, \
    float: print_float, \
    default: print_default \
)(x)

// 静态断言（C23 关键字版本）
static_assert(sizeof(int) == 4, "int must be 4 bytes");

// 对齐指定（C23）
alignas(16) int aligned_var;

// 常量表达式（C23）
constexpr int MAX_SIZE = 100;

// 类型查询（C23）
typeof(x) y = x;  // y 与 x 同类型
```

## 6. 代码示例 (Examples)

### 6.1 基础用法：变量声明与类型

```c
#include <stdio.h>

// C23 可直接使用 bool，C99 需要 <stdbool.h>
#ifdef __STDC_VERSION__
#if __STDC_VERSION__ >= 202311L
    // C23: bool 是关键字
#else
    #include <stdbool.h>
#endif
#endif

int main(void) {
    // 基本类型
    int integer = 42;
    float decimal = 3.14f;
    char character = 'X';

    // 类型修饰
    const int immutable = 100;
    volatile int hardware_reg = 0;
    unsigned long big = 999999999UL;

    // 布尔类型
    bool is_valid = true;
    bool is_ready = false;

    printf("int: %d, float: %f, char: %c\n", integer, decimal, character);
    printf("bool values: %d, %d\n", is_valid, is_ready);

    return 0;
}
```

### 6.2 基础用法：控制流

```c
#include <stdio.h>

int main(void) {
    int values[] = {10, 20, 30, 40, 50};
    int n = sizeof(values) / sizeof(values[0]);

    // for 循环
    for (int i = 0; i < n; i++) {
        printf("values[%d] = %d\n", i, values[i]);
    }

    // while 循环
    int j = 0;
    while (j < n) {
        if (values[j] == 30) {
            printf("Found 30 at index %d\n", j);
            break;
        }
        j++;
    }

    // switch 语句
    int choice = 2;
    switch (choice) {
        case 1:
            printf("Option 1 selected\n");
            break;
        case 2:
            printf("Option 2 selected\n");
            break;
        default:
            printf("Unknown option\n");
    }

    return 0;
}
```

### 6.3 高级用法：泛型编程（C11）

```c
#include <stdio.h>

// 泛型打印函数
void print_int(int x) { printf("int: %d\n", x); }
void print_float(float x) { printf("float: %f\n", x); }
void print_double(double x) { printf("double: %lf\n", x); }
void print_default(void* x) { printf("pointer: %p\n", x); }

#define print(x) _Generic((x), \
    int: print_int, \
    float: print_float, \
    double: print_double, \
    default: print_default \
)(x)

int main(void) {
    int i = 42;
    float f = 3.14f;
    double d = 2.71828;
    int* p = &i;

    print(i);  // 输出: int: 42
    print(f);  // 输出: float: 3.140000
    print(d);  // 输出: double: 2.718280
    print(p);  // 输出: pointer: 0x...

    return 0;
}
```

### 6.4 高级用法：原子操作与线程（C11）

```c
#include <stdio.h>
#include <stdatomic.h>
#include <threads.h>

atomic_int counter = 0;

int thread_func(void* arg) {
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&counter, 1);
    }
    return 0;
}

int main(void) {
    thrd_t t1, t2;

    thrd_create(&t1, thread_func, NULL);
    thrd_create(&t2, thread_func, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    printf("Counter: %d\n", counter);  // 输出: Counter: 2000

    return 0;
}
```

### 6.5 高级用法：C23 新特性

```c
// C23 示例（需要支持 C23 的编译器）
#include <stdio.h>

// 常量表达式
constexpr int MAX_ITEMS = 100;

// 静态断言
static_assert(MAX_ITEMS > 0, "MAX_ITEMS must be positive");

// 对齐指定
struct Aligned {
    alignas(16) char data[64];
};

// typeof 示例
#define SWAP(a, b) do { \
    typeof(a) _tmp = (a); \
    (a) = (b); \
    (b) = _tmp; \
} while(0)

int main(void) {
    // nullptr
    int* ptr = nullptr;

    // bool 关键字（无需头文件）
    bool ready = true;

    // typeof 自动推导
    int x = 10;
    typeof(x) y = x * 2;  // y 是 int 类型

    // SWAP 宏使用
    int a = 1, b = 2;
    SWAP(a, b);
    printf("a=%d, b=%d\n", a, b);  // 输出: a=2, b=1

    return 0;
}
```

### 6.6 常见错误及修正

**错误示例 1：使用关键字作为标识符**

```c
// 错误代码
int int = 10;        // 错误：int 是关键字
float float = 3.14;  // 错误：float 是关键字
char class = 'A';    // 错误：class 在某些编译器中是保留字
```

```c
// 修正方法：使用不同的标识符名
int count = 10;
float value = 3.14f;
char category = 'A';
```

**错误示例 2：忽略关键字版本差异**

```c
// 错误代码（在 C89 中）
bool flag = true;  // 错误：C89 没有 bool 关键字

// 在 C99 中需要包含头文件
#include <stdbool.h>
bool flag = true;  // OK
```

```c
// 跨版本兼容写法
#if __STDC_VERSION__ >= 202311L
    // C23: bool 是关键字，无需头文件
#else
    #include <stdbool.h>
#endif

bool flag = true;
```

**错误示例 3：误用 register 关键字**

```c
// 过时用法
register int x = 10;
int* p = &x;  // C11 前：错误，不能取 register 变量的地址
              // C11 起：合法，register 被忽略
```

```c
// 现代做法：不使用 register
int x = 10;  // 让编译器自动优化
int* p = &x;
```

**错误示例 4：混淆 inline 语义**

```c
// 可能的问题
// file1.c
inline int square(int x) { return x * x; }

// file2.c
inline int square(int x) { return x * x; }
// 可能导致链接错误或未定义行为
```

```c
// 修正方法：使用 static inline 或 extern inline
// header.h
static inline int square(int x) { return x * x; }

// 或在 C23 中使用更好的模块化方式
```

## 7. 总结 (Summary)

### 核心要点

1. **关键字总数**：C23 标准包含约 50+ 个关键字，涵盖数据类型、存储类、控制流等

2. **版本演进**：
   - C89/C90 奠定了基础关键字集
   - C99 引入布尔、复数和 `restrict`/`inline`
   - C11 加入并发支持和泛型
   - C23 大幅现代化，引入 `constexpr`、`nullptr`、`typeof` 等

3. **命名约定**：
   - 普通关键字：小写字母
   - 技术性关键字：下划线前缀（如 `_Atomic`）
   - 弃用模式：下划线版本被替代

4. **预处理器关键字**：独立于语言关键字，仅在预处理指令中有效

### 关键字分类概览

| 类别 | 关键字示例 |
|-----|-----------|
| 基本类型 | `int`, `char`, `float`, `double`, `void` |
| 类型修饰 | `signed`, `unsigned`, `short`, `long` |
| 存储类 | `auto`, `static`, `extern`, `register`, `typedef` |
| 类型限定 | `const`, `volatile`, `restrict`, `_Atomic` |
| 控制流 | `if`, `else`, `for`, `while`, `switch`, `break`, `continue`, `return`, `goto` |
| 复合类型 | `struct`, `union`, `enum` |
| 函数 | `inline`, `_Noreturn` |
| C11 新增 | `_Generic`, `_Static_assert`, `_Thread_local` |
| C23 新增 | `bool`, `true`, `false`, `nullptr`, `constexpr`, `typeof`, `alignas`, `alignof` |

### 相关概念

- **标识符 (Identifier)** - 变量、函数等的名称
- **保留标识符 (Reserved Identifier)** - 不能由程序员定义的标识符
- **预处理器指令 (Preprocessor Directive)** - 编译前处理的指令
- **二字符组 (Digraph)** - 关键字和符号的替代表示

### 学习建议

1. 熟悉各 C 标准版本引入的新关键字，了解代码的可移植性
2. 使用条件编译处理不同标准版本的兼容性问题
3. 理解关键字的语义，避免误用（如 `register` 现已无用）
4. 关注 C23 新特性，如 `constexpr`、`nullptr`、`typeof` 等现代化特性
5. 注意下划线前缀的保留标识符规则，避免与标准库冲突