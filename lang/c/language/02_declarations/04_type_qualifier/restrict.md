# restrict 类型限定符（C99 起）

## 1. 概述

`restrict` 是 C99 标准引入的类型限定符（type qualifier），用于告知编译器指针是访问特定对象的唯一方式。这是一种优化提示，承诺通过该限定指针访问的对象不会被其他指针别名访问。

`restrict` 关键字属于 C 语言类型限定符家族，与 `const` 和 `volatile` 并列。它仅适用于指向对象类型的指针或其（多维）数组（C23 起）。`restrict` 的核心价值在于帮助编译器进行更激进的优化，包括向量化、循环展开和指令重排等。

**核心概念**：
- **别名（Aliasing）**：多个指针指向同一内存位置的现象
- **restrict 语义**：承诺被修改的对象只能通过 restrict 限定指针访问

## 2. 来源与演变

### 历史背景

在 C99 之前，编译器在进行指针别名分析时面临巨大困难。由于两个不同类型的指针理论上可能指向同一内存位置，编译器必须保守处理，导致许多优化机会被放弃。

### 设计动机

`restrict` 的设计灵感来源于 Fortran 语言的别名规则。Fortran 假设子程序的参数之间不会相互别名，这使得编译器能够生成更高效的代码。C 语言引入 `restrict` 的目的是让 C 程序也能获得类似的优化机会。

### C99 引入

`restrict` 关键字在 **C99 标准**（ISO/IEC 9899:1999）中首次引入，正式名称为"restrict type qualifier"。相关规范位于标准的 6.7.3.1 节。

### C23 变化

从 **C23 标准**起，restrict 限定符的适用范围扩展到数组类型：

| 版本 | 行为 |
|------|------|
| C99 - C17 | 数组类型声明 restrict 时，限定的是元素类型，而非数组本身 |
| C23 起 | 数组类型及其元素类型被视为具有相同的 restrict 限定 |

### 标准演进

| 标准 | 章节 |
|------|------|
| C99 | 6.7.3.1 Formal definition of restrict |
| C11 | 6.7.3.1 Formal definition of restrict |
| C17 | 6.7.3.1 Formal definition of restrict (p: 89-90) |
| C23 | 6.7.3.1 Formal definition of restrict |

## 3. 语法与参数

### 基本语法

```c
// restrict 限定指针的声明
类型 * restrict 指针名;

// 函数参数中使用
返回类型 函数名(类型 * restrict 参数1, 类型 * restrict 参数2);
```

### 正确用法

```c
// 正确：指向对象类型的指针
int * restrict p;
float * restrict q;

// 正确：指向数组的指针
int (* restrict arr_ptr)[10];

// 正确：函数参数
void f(int n, int * restrict p, int * restrict q);

// 正确：结构体成员
struct buffer {
    int n;
    float * restrict data;
};
```

### 错误用法

```c
// 错误：不能用于非指针类型
int restrict *p;           // 错误：int 不是指针

// 错误：不能用于函数指针
float (* restrict f9)(void); // 错误：函数指针不能 restrict 限定
```

### restrict 的作用域规则

restrict 限定指针的别名断言仅在声明它的块作用域内有效：

| 声明位置 | 作用域 |
|----------|--------|
| 函数参数 | 整个函数体 |
| 块作用域 | 该块的执行期间 |
| 文件作用域 | 整个程序执行期间 |
| 结构体成员 | 访问该结构体的标识符作用域 |

### 数组参数中的 restrict

```c
// 在函数参数的数组声明中使用 restrict
void f(int m, int n, float a[restrict m][n], float b[restrict m][n]);
// 等价于：
void f(int m, int n, float (* restrict a)[n], float (* restrict b)[n]);
```

## 4. 底层原理

### 别名问题与优化障碍

当编译器无法确定两个指针是否指向同一内存位置时，必须保守处理：

```c
void add(int *a, int *b, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = b[i] + 1;
    }
}
```

在没有 `restrict` 的情况下，编译器必须假设 `a` 和 `b` 可能重叠，因此：
- 不能对 `a[i] = b[i] + 1` 进行向量化（SIMD）
- 每次写入 `a[i]` 后，后续读取 `b[i]` 可能看到不同的值
- 必须按顺序执行，无法并行优化

### restrict 的优化机制

```c
void add_restrict(int * restrict a, int * restrict b, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = b[i] + 1;
    }
}
```

使用 `restrict` 后，编译器获知 `a` 和 `b` 指向不重叠的内存区域，可以进行：
- **向量化**：使用 SIMD 指令一次处理多个元素
- **循环展开**：减少循环开销
- **指令重排**：提高流水线效率
- **寄存器分配**：将数据保持在寄存器中更长时间

### 编译器生成的代码对比

```c
// 无 restrict 版本
int foo(int *a, int *b) {
    *a = 5;
    *b = 6;
    return *a + *b;
}

// 有 restrict 版本
int rfoo(int * restrict a, int * restrict b) {
    *a = 5;
    *b = 6;
    return *a + *b;
}
```

生成的汇编代码对比（x86-64 平台）：

```asm
; 无 restrict - 必须重新读取 *a
foo:
    movl    $5, (%rdi)    ; 存储 5 到 *a
    movl    $6, (%rsi)    ; 存储 6 到 *b
    movl    (%rdi), %eax  ; 重新读取 *a（可能被 *b 修改）
    addl    $6, %eax      ; 加 6
    ret

; 有 restrict - 编译器知道值是常量
rfoo:
    movl    $11, %eax     ; 直接返回 11（编译期常量）
    movl    $5, (%rdi)
    movl    $6, (%rsi)
    ret
```

### 赋值规则

restrict 指针之间的赋值有严格限制：

```c
// 未定义行为：restrict 指针之间的赋值
int * restrict p1 = &a;
int * restrict p2 = &b;
p1 = p2;  // 未定义行为！

// 合法：从外层块向内层块赋值
void outer(void) {
    int * restrict p = &a;
    {
        int * restrict q = p;  // OK：从外层向内层赋值
    }
}

// 合法：restrict 指针赋值给非 restrict 指针
void f(int n, float * restrict r, float * restrict s) {
    float *p = r, *q = s;  // OK
    while (n-- > 0)
        *p++ = *q++;  // 编译器仍可优化
}
```

## 5. 使用场景

### 函数参数（最常用）

这是 `restrict` 最常见的使用场景，用于函数参数声明：

```c
// 内存复制函数
void my_memcpy(void * restrict dest,
               const void * restrict src,
               size_t n);

// 向量加法
void vec_add(float * restrict result,
             const float * restrict a,
             const float * restrict b,
             size_t n);

// 矩阵乘法
void mat_mul(double * restrict c,
             const double * restrict a,
             const double * restrict b,
             size_t n);
```

### 文件作用域

用于全局指针变量，指向动态分配的全局数组：

```c
// 文件作用域的 restrict 指针
float * restrict buffer_a;
float * restrict buffer_b;

int init_buffers(int n) {
    float *t = malloc(2 * n * sizeof(float));
    buffer_a = t;          // 指向前半部分
    buffer_b = t + n;      // 指向后半部分
    return 0;
}
// 编译器可推断 buffer_a、buffer_b 不存在别名
```

### 块作用域

用于局部变量，在关键代码段（如紧凑循环）中提供别名断言：

```c
void process(float *a, float *b, int n) {
    {
        float * restrict ra = a;
        float * restrict rb = b;
        for (int i = 0; i < n; i++) {
            ra[i] = rb[i] * 2.0f;  // 可向量化
        }
    }
    // 之后 a 和 b 可能再次重叠使用
}
```

### 结构体成员

用于结构体中的指针成员，断言这些指针指向不重叠的存储：

```c
struct buffers {
    int n;
    float * restrict input;
    float * restrict output;
};

void process(struct buffers buf) {
    // buf.input 和 buf.output 应指向不重叠的存储
    for (int i = 0; i < buf.n; i++) {
        buf.output[i] = buf.input[i] * 2.0f;
    }
}
```

### 最佳实践

1. **仅在有性能需求时使用**：`restrict` 是优化提示，不是功能需求
2. **确保断言成立**：违反 `restrict` 约束会导致未定义行为
3. **在函数原型中明确标注**：让调用者清楚别名要求
4. **优先用于函数参数**：这是最安全、最有效的使用方式

### 常见陷阱

| 陷阱 | 后果 | 正确做法 |
|------|------|----------|
| 传递重叠指针 | 未定义行为 | 确保参数指向不重叠的内存区域 |
| 块作用域外使用 | 断言无效 | 仅在声明块内信赖 restrict 语义 |
| 忘记 const 修饰 | 只读参数可能被修改 | 对只读参数同时使用 `const` 和 `restrict` |
| 循环内重新赋值 | 可能破坏 restrict 语义 | 避免在作用域内修改 restrict 指针 |

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>
#include <string.h>

// 简单的内存复制函数
void my_memcpy(void * restrict dest,
               const void * restrict src,
               size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    while (n--) {
        *d++ = *s++;
    }
}

int main(void) {
    char src[] = "Hello, World!";
    char dest[20];

    my_memcpy(dest, src, sizeof(src));
    printf("%s\n", dest);  // 输出: Hello, World!

    return 0;
}
```

### 向量运算示例

```c
#include <stdio.h>
#include <stdlib.h>

// 向量加法：result = a + b
void vec_add(double * restrict result,
             const double * restrict a,
             const double * restrict b,
             size_t n) {
    for (size_t i = 0; i < n; i++) {
        result[i] = a[i] + b[i];
    }
}

// 向量缩放：v = v * scale
void vec_scale(double * restrict v, double scale, size_t n) {
    for (size_t i = 0; i < n; i++) {
        v[i] *= scale;
    }
}

// 点积计算
double dot_product(const double * restrict a,
                   const double * restrict b,
                   size_t n) {
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

int main(void) {
    size_t n = 1000;
    double *a = malloc(n * sizeof(double));
    double *b = malloc(n * sizeof(double));
    double *result = malloc(n * sizeof(double));

    // 初始化向量
    for (size_t i = 0; i < n; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    vec_add(result, a, b, n);
    printf("result[0] = %f\n", result[0]);  // 0 + 0 = 0
    printf("result[1] = %f\n", result[1]);  // 1 + 2 = 3

    free(a);
    free(b);
    free(result);
    return 0;
}
```

### 矩阵运算示例

```c
#include <stdio.h>
#include <stdlib.h>

// 矩阵乘法：C = A * B
// 所有矩阵都是 n x n 的方阵
void matrix_mult(int n,
                 double * restrict C,
                 const double * restrict A,
                 const double * restrict B) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

int main(void) {
    int n = 3;
    double A[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double B[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    double C[9];

    matrix_mult(n, C, A, B);

    // 打印结果矩阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%6.1f ", C[i * n + j]);
        }
        printf("\n");
    }

    return 0;
}
```

### 常见错误及修正

#### 错误 1：传递重叠指针

```c
// 错误示例：违反 restrict 约束
void bad_example(void) {
    int arr[10];
    int *p = arr;
    int *q = arr + 5;

    // OK：p 和 q 不重叠
    // f(5, p, q);

    // 未定义行为：p 和 q 重叠
    // f(10, p, arr);  // p[5] 与 arr[5] 重叠
}

// 修正：确保参数不重叠
void good_example(void) {
    int arr1[10];
    int arr2[10];

    // 正确：两个独立的数组
    // process(10, arr1, arr2);
}
```

#### 错误 2：restrict 指针之间赋值

```c
// 错误示例
void bad_assign(void) {
    int a = 1, b = 2;
    int * restrict p = &a;
    int * restrict q = &b;

    p = q;  // 未定义行为！
}

// 修正：使用非 restrict 指针
void good_assign(void) {
    int a = 1, b = 2;
    int *p = &a;
    int *q = &b;

    p = q;  // 合法
}
```

#### 错误 3：忽略 const 修饰

```c
// 不推荐：只读参数可能被意外修改
void process_bad(int * restrict output, int * restrict input, int n);

// 推荐：对只读参数使用 const
void process_good(int * restrict output,
                  const int * restrict input,
                  int n);
```

### 使用宏定义实现可移植性

```c
// 提供 restrict 的可移植定义
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define RESTRICT restrict
#elif defined(__GNUC__) || defined(__clang__)
    #define RESTRICT __restrict
#elif defined(_MSC_VER)
    #define RESTRICT __restrict
#else
    #define RESTRICT
#endif

// 使用宏定义
void vec_add(double * RESTRICT result,
             const double * RESTRICT a,
             const double * RESTRICT b,
             size_t n);
```

## 7. 总结

### 核心要点

`restrict` 类型限定符是 C99 引入的重要优化工具：

| 特性 | 说明 |
|------|------|
| **用途** | 告知编译器指针是访问对象的唯一方式 |
| **效果** | 允许编译器进行更激进的优化 |
| **约束** | 违反 restrict 约束导致未定义行为 |
| **适用类型** | 指向对象类型的指针或其数组 |
| **作用域** | 仅在声明它的块作用域内有效 |

### 关键规则

1. **唯一访问承诺**：如果对象通过 restrict 指针被修改，所有访问必须通过该指针
2. **只读例外**：未被修改的对象可以通过多个 restrict 指针访问
3. **赋值限制**：restrict 指针之间不能互相赋值（除特定情况外）
4. **编译器可选**：编译器可以选择忽略 restrict 的优化提示

### 与其他限定符对比

| 限定符 | 作用 | 适用类型 |
|--------|------|----------|
| `const` | 表示只读 | 任何类型 |
| `volatile` | 防止优化，每次访问都读内存 | 任何类型 |
| `restrict` | 别名断言，优化提示 | 仅指针类型 |

### 学习建议

1. **理解别名问题**：先理解为什么编译器需要别名分析
2. **从小处着手**：先在简单函数参数中使用
3. **使用静态分析工具**：帮助检测潜在的 restrict 违规
4. **阅读标准库源码**：学习标准函数如何使用 restrict
5. **关注汇编输出**：对比有无 restrict 的汇编代码差异

### 注意事项

- `restrict` 只是承诺，编译器不检查违规
- 违反 `restrict` 约束是未定义行为，可能产生难以调试的错误
- 现代 C++ 没有直接的 `restrict` 等价物（C++ 中某些编译器支持 `__restrict__` 扩展）
- 删除所有 `restrict` 限定符不应改变程序的可观察行为

## 参考资料

- ISO/IEC 9899:2024 (C23) - 6.7.3.1 Formal definition of restrict
- ISO/IEC 9899:2018 (C17) - 6.7.3.1 Formal definition of restrict
- ISO/IEC 9899:2011 (C11) - 6.7.3.1 Formal definition of restrict
- ISO/IEC 9899:1999 (C99) - 6.7.3.1 Formal definition of restrict
- cppreference: https://en.cppreference.com/w/c/language/restrict