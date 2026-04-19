# 作用域（Scope）

## 1. 概述（Overview）

**作用域（Scope）** 是 C 程序中标识符可见（即可被使用）的源代码区域。每个出现在 C 程序中的标识符都只在某个可能不连续的源代码部分中可见，这部分代码称为该标识符的作用域。

在一个作用域内，仅当实体属于不同的命名空间时，同一标识符才能指定多个实体。

### 四种作用域类型

| 作用域类型 | 说明 |
|------------|------|
| 块作用域（block scope） | 复合语句、函数体、选择/迭代语句内的标识符 |
| 文件作用域（file scope） | 任何块或参数列表之外声明的标识符 |
| 函数作用域（function scope） | 仅适用于标签（label） |
| 函数原型作用域（function prototype scope） | 函数声明（非定义）的参数列表中的标识符 |

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C89/C90 | 定义四种作用域；外部链接标识符即使引入块内也有文件作用域 |
| C99 | 选择语句和迭代语句建立自己的块作用域 |
| C11 | 沿用既有规则 |
| C17 | 沿用既有规则 |
| C23 | 沿用既有规则 |

### 与 C++ 的区别

| 特性 | C | C++ |
|------|---|-----|
| 结构体作用域 | 无 | 有 |
| 循环体变量隐藏循环初始化变量 | 可以 | 不可以 |
| 选择/迭代语句的块作用域 | C99 起 | 始终有 |

### C99 的重要变更

在 C99 之前，选择语句（`if`、`switch`）和迭代语句（`for`、`while`、`do-while`）不建立自己的块作用域：

```c
enum {a, b};
int different(void)
{
    if (sizeof(enum {b, a}) != sizeof(int))
        return a;  // a == 1
    return b;      // C89: b == 0, C99: b == 1
}
```

## 3. 语法与参数（Syntax and Parameters）

### 3.1 块作用域

块作用域适用于以下位置声明的标识符：
- 复合语句（包括函数体）
- 选择语句（`if`、`switch`）内的表达式、声明或语句（C99 起）
- 迭代语句（`for`、`while`、`do-while`）内的表达式、声明或语句（C99 起）
- 函数定义的参数列表

**作用域范围**：从声明点开始，到声明它的块或语句结束为止。

```c
void f(int n)  // 函数参数 'n' 的作用域开始
{              // 函数体开始
   ++n;        // 'n' 在作用域内
// int n = 2;  // 错误：不能在同一作用域内重声明标识符
   for(int n = 0; n<10; ++n) {  // 循环局部 'n' 的作用域开始
       printf("%d\n", n);       // 打印 0 1 2 3 4 5 6 7 8 9
   }            // 循环局部 'n' 的作用域结束
                // 函数参数 'n' 回到作用域
   printf("%d\n", n);  // 打印参数值
}              // 函数参数 'n' 的作用域结束
```

### 3.2 文件作用域

文件作用域适用于在任何块或参数列表之外声明的标识符。

**作用域范围**：从声明点开始，到翻译单元结束为止。

```c
int i;  // i 的作用域开始
static int g(int a) { return a; }  // g 的作用域开始（注意 "a" 有块作用域）
int main(void)
{
    i = g(2);  // i 和 g 都在作用域内
}
```

**默认属性**：
- 外部链接（external linkage）
- 静态存储期（static storage duration）

### 3.3 函数作用域

函数作用域**仅适用于标签（label）**。

**特点**：
- 标签在函数内的任何位置都可见
- 包括所有嵌套块
- 在其声明之前和之后都可见

```c
void f()
{
   {
       goto label;  // 标签在作用域内，即使声明在后面
label:;
   }
   goto label;      // 标签忽略块作用域
}

void g()
{
    goto label;     // 错误：label 在 g() 中不在作用域内
}
```

### 3.4 函数原型作用域

函数原型作用域适用于函数声明（非定义）的参数列表中的名称。

**作用域范围**：从声明点开始，到函数声明符结束为止。

```c
int f(int n,
      int a[n]);  // n 在作用域内，引用第一个参数
```

如果有多个或嵌套的声明符，作用域在最近的封闭函数声明符结束处终止：

```c
void f (                        // 函数名 'f' 在文件作用域
 long double f,                 // 标识符 'f' 现在在作用域内，隐藏文件作用域的 'f'
 char (**a)[10 * sizeof f]      // 'f' 引用第一个参数
);

enum{ n = 3 };
int (*(*g)(int n))[n];  // 函数参数 'n' 的作用域在函数声明符结束处终止
                        // 在数组声明符中，全局 n 在作用域内
```

### 3.5 嵌套作用域

当两个同名标识符指定的不同实体同时处于作用域内，且属于同一命名空间时，作用域必须是嵌套的（不允许其他形式的作用域重叠），内层作用域的声明隐藏外层作用域的声明。

```c
int a;   // 名称 a 的文件作用域开始

void f(void)
{
    int a = 1;  // 名称 a 的块作用域开始；隐藏文件作用域的 a
    {
      int a = 2;         // 内层 a 的作用域开始，外层 a 被隐藏
      printf("%d\n", a); // 内层 a 在作用域内，打印 2
    }                    // 内层 a 的块作用域结束
    printf("%d\n", a);   // 外层 a 在作用域内，打印 1
}                        // 外层 a 的作用域结束

void g(int a);   // 名称 a 有函数原型作用域；隐藏文件作用域的 a
```

## 4. 底层原理（Underlying Principles）

### 4.1 声明点（Point of Declaration）

不同类型标识符的声明点规则：

| 标识符类型 | 声明点位置 |
|------------|------------|
| 结构体/联合体/枚举标签 | 标签在类型说明符中出现后立即开始 |
| 枚举常量 | 枚举器在枚举列表中出现后立即开始 |
| 其他标识符 | 声明符结束后、初始化器之前 |

### 4.2 结构体/联合体/枚举标签

```c
struct Node {
   struct Node* next;  // Node 在作用域内，引用此结构体
};
```

### 4.3 枚举常量

```c
enum { x = 12 };
{
    enum { x = x + 1,  // 新 x 直到逗号才进入作用域，x 初始化为 13
           y = x + 1   // 新枚举器 x 现在在作用域内，y 初始化为 14
         };
}
```

### 4.4 普通标识符

声明符结束后的声明点规则：

```c
int x = 2;  // 第一个 'x' 的作用域开始
{
    int x[x];  // 新声明的 x 的作用域在声明符 (x[x]) 之后开始
               // 在声明符内，外层 'x' 仍在作用域内
               // 这声明了一个 2 个 int 的 VLA 数组
}

unsigned char x = 32;  // 外层 'x' 的作用域开始
{
    unsigned char x = x;
        // 内层 'x' 的作用域在初始化器 (= x) 之前开始
        // 这不会用值 32 初始化内层 'x'
        // 而是用其自身的不确定值初始化内层 'x'
}

unsigned long factorial(unsigned long n)
// 声明符结束，'factorial' 从此点开始作用域
{
   return n<2 ? 1 : n*factorial(n-1);  // 递归调用
}
```

### 4.5 类型名称的特殊情况

如果类型名称不是标识符的声明，其作用域被认为从类型名称中省略标识符的位置之后开始。

### 4.6 存储期与作用域的关系

| 作用域类型 | 默认链接 | 默认存储期 |
|------------|----------|------------|
| 块作用域 | 无链接 | 自动存储期 |
| 文件作用域 | 外部链接 | 静态存储期 |

**注意**：非 VLA 局部变量的存储期在进入块时开始，但在看到声明之前，变量不在作用域内，无法访问。

## 5. 使用场景（Use Cases）

### 5.1 变量隐藏

```c
int value = 100;  // 全局变量

void func(void) {
    int value = 200;  // 隐藏全局变量
    {
        int value = 300;  // 隐藏外层局部变量
        printf("%d\n", value);  // 300
    }
    printf("%d\n", value);  // 200
}
```

### 5.2 循环变量作用域（C99 起）

```c
void loop_example(void) {
    for (int i = 0; i < 10; i++) {
        int i = 100;  // C: 允许，隐藏循环变量
        printf("%d\n", i);  // 始终打印 100
    }
    // i 在此处不可见
}
```

### 5.3 标签的函数作用域

```c
int search(int* arr, int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            goto found;
        }
    }
    return -1;  // 未找到

found:          // 标签在整个函数内可见
    return 1;   // 找到
}
```

### 5.4 函数原型参数

```c
// 原型作用域：参数名只在声明符内有效
int compute(int size, int array[size]);

// 定义时可以使用不同的参数名
int compute(int n, int array[n]) {
    return array[0];
}
```

### 5.5 常见陷阱

1. **使用未初始化的自身值**：

```c
unsigned char x = 32;
{
    unsigned char x = x;  // 危险：用不确定值初始化
}
```

2. **C 与 C++ 的循环变量差异**：

```c
// C: 允许循环体变量隐藏循环变量
for (int i = 0; i < 10; i++) {
    int i = 100;  // C: 允许
}

// C++: 不允许
for (int i = 0; i < 10; i++) {
    int i = 100;  // C++: 错误
}
```

3. **结构体成员与外部作用域**：

```c
// C: 结构体内声明在外部作用域可见
struct foo {
    struct baz { int x; };  // baz 在文件作用域可见
    enum color {RED, BLUE}; // color 和 RED 在文件作用域可见
};
struct baz b;           // baz 在作用域内
enum color x = RED;     // color 和 RED 在作用域内
```

## 6. 代码示例（Examples）

### 基础用法

```c
#include <stdio.h>

// 文件作用域
int global_var = 100;
static int file_local = 200;

void demonstrate_scope(void);

int main(void)
{
    printf("Global: %d\n", global_var);
    printf("File local: %d\n", file_local);

    demonstrate_scope();

    return 0;
}

void demonstrate_scope(void)
{
    // 块作用域
    int local_var = 300;
    printf("Local: %d\n", local_var);

    // 隐藏外层变量
    int global_var = 999;
    printf("Hidden global: %d\n", global_var);

    // 嵌套块作用域
    {
        int nested_var = 400;
        printf("Nested: %d\n", nested_var);
        printf("Still can access local: %d\n", local_var);
    }

    // 嵌套块已结束
    // printf("%d\n", nested_var);  // 错误：nested_var 不在作用域内
}
```

### 循环和选择语句作用域（C99）

```c
#include <stdio.h>

void c99_scope_example(void)
{
    // for 循环变量的作用域
    for (int i = 0; i < 3; i++) {
        printf("i = %d\n", i);
    }
    // printf("%d\n", i);  // 错误：i 不在作用域内

    // if 语句内的作用域
    if (int x = 5) {  // C99: 条件中的声明
        printf("x = %d\n", x);
    }
    // printf("%d\n", x);  // 错误：x 不在作用域内

    // switch 语句内的作用域
    switch (int y = 1) {
        case 1:
            printf("y = %d\n", y);
            break;
    }
}

int main(void)
{
    c99_scope_example();
    return 0;
}
```

### 标签作用域

```c
#include <stdio.h>

void jump_example(int n)
{
    // 标签在整个函数内可见（包括声明之前）
    if (n < 0) {
        goto error;  // 向前跳转
    }

    printf("Processing: %d\n", n);
    goto done;

error:
    printf("Error: negative value\n");
    return;

done:
    printf("Done\n");
}

int main(void)
{
    jump_example(10);
    jump_example(-1);
    return 0;
}
```

### 函数原型作用域

```c
#include <stdio.h>

// 函数原型作用域：参数名只在声明符内有效
int process(int size, int data[size]);

// 另一个声明，可以使用不同的参数名
int process(int n, int arr[n]);

// 定义
int process(int count, int values[])
{
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += values[i];
    }
    return sum;
}

int main(void)
{
    int data[] = {1, 2, 3, 4, 5};
    printf("Sum: %d\n", process(5, data));
    return 0;
}
```

### 常见错误及修正

```c
#include <stdio.h>

int global = 10;

/* 错误：用不确定值初始化 */
void error_self_init(void)
{
    unsigned char x = 32;
    {
        unsigned char x = x;  // 用自身的不确定值初始化
        printf("x = %d\n", x);  // 未定义行为
    }
}

/* 正确：使用不同的名称或外层变量 */
void correct_init(void)
{
    unsigned char x = 32;
    {
        unsigned char y = x;  // 使用外层变量初始化
        printf("y = %d\n", y);  // 32
    }
}

/* 错误：在作用域外访问变量 */
void error_out_of_scope(void)
{
    for (int i = 0; i < 10; i++) {
        int temp = i * 2;
    }
    // printf("%d\n", i);    // 错误：i 不在作用域内
    // printf("%d\n", temp); // 错误：temp 不在作用域内
}

/* 正确：在需要的地方声明变量 */
void correct_scope(void)
{
    int i;
    for (i = 0; i < 10; i++) {
        int temp = i * 2;
        printf("%d ", temp);
    }
    printf("\ni after loop: %d\n", i);  // 10
}

/* 结构体成员作用域（与 C++ 不同） */
struct example {
    struct inner { int x; };  // inner 在文件作用域可见
    int value;
};

int main(void)
{
    struct inner obj = {42};  // C: 正确，inner 在文件作用域
    printf("inner.x = %d\n", obj.x);

    correct_init();
    correct_scope();

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **四种作用域**：块作用域、文件作用域、函数作用域、函数原型作用域
2. **嵌套隐藏**：内层作用域的声明隐藏外层同名声明
3. **声明点规则**：不同类型标识符有不同的声明点规则
4. **标签特殊性**：标签具有函数作用域，在整个函数内可见

### 作用域特性对比

| 作用域类型 | 适用标识符 | 范围 | 默认链接 | 默认存储期 |
|------------|------------|------|----------|------------|
| 块作用域 | 局部变量、参数 | 声明点到块结束 | 无链接 | 自动 |
| 文件作用域 | 全局变量、函数 | 声明点到翻译单元结束 | 外部 | 静态 |
| 函数作用域 | 标签 | 整个函数 | - | - |
| 函数原型作用域 | 原型参数 | 函数声明符内 | - | - |

### 与 C++ 的区别

| 特性 | C | C++ |
|------|---|-----|
| 结构体作用域 | 无 | 有 |
| 循环体隐藏循环变量 | 允许 | 不允许 |
| 选择/迭代语句块作用域 | C99 起 | 始终有 |

### 最佳实践

- 避免不必要的变量隐藏，使用有意义的名称
- 注意声明点规则，避免使用不确定值初始化
- 利用块作用域限制变量的生命周期
- 标签名应具有描述性且避免与变量名冲突
- 注意 C 与 C++ 在循环变量作用域上的差异

### 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.2.1 Scopes of identifiers
- C17 标准 (ISO/IEC 9899:2018): 6.2.1 Scopes of identifiers (p: 28-29)
- C11 标准 (ISO/IEC 9899:2011): 6.2.1 Scopes of identifiers (p: 35-36)
- C99 标准 (ISO/IEC 9899:1999): 6.2.1 Scopes of identifiers (p: 29-30)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.1.2.1 Scopes of identifiers