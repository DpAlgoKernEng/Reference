# C 语句详解

  [1. 语句基本概念](#1-语句基本概念)  
  
  [2. 语句类型详解](#2-语句类型详解)  
  
  [3. 标签语句深入分析](#3-标签语句深入分析)  
  
  [4. 复合语句与作用域](#4-复合语句与作用域)  
  
  [5. 表达式语句详解](#5-表达式语句详解)  
  
  [6. 选择语句深入探讨](#6-选择语句深入探讨)  
  
  [7. 迭代语句全面解析](#7-迭代语句全面解析)  
  
  [8. 跳转语句深度剖析](#8-跳转语句深度剖析)  
  
  [9. 属性说明符（C23）](#9-属性说明符c23)  
  
  [10. 语句执行顺序与控制流](#10-语句执行顺序与控制流)  
  
  [11. 常见陷阱与最佳实践](#11-常见陷阱与最佳实践)  
  
  [12. 标准参考](#12-标准参考)  
  
  [13. 总结](#13-总结)

## 1. 语句基本概念

### 1.1 语句的本质

在 C 语言中，语句（Statement）是程序执行的基本单位，它指示编译器执行特定的操作。语句与表达式有着本质的区别：

- 表达式（Expression）：计算并产生一个值，如 `a + b`、`func()`、`x = 5`
- 语句（Statement）：执行一个操作，如 `a = b + c;`、`if (x > 0) {...}`

语句是构成程序控制流的基本元素，它们按照顺序执行，形成程序的逻辑结构。

### 1.2 语句与函数的关系

任何函数的主体都是一个复合语句（Compound Statement），也称为块（Block）。复合语句由一系列语句和声明组成，用大括号 `{}` 包围。

```c
#include <stdio.h>

// 函数主体是一个复合语句
int calculate_sum(int a, int b) {
    // 声明语句（C99+ 允许在块中声明）
    int result;
    
    // 表达式语句
    result = a + b;
    
    // 返回语句
    return result;
}

int main(void) {
    // 复合语句示例
    {
        int local_var = 10;  // 局部变量声明
        printf("Local variable: %d\n", local_var);
        
        // 嵌套复合语句
        {
            int nested_var = 20;
            printf("Nested variable: %d\n", nested_var);
            printf("Outer variable: %d\n", local_var);  // 可访问外层变量
        }
        
        // printf("Nested variable: %d\n", nested_var);  // 错误：超出作用域
    }
    
    return 0;
}
```

### 1.3 语句的分类依据

C 语言根据语句的功能将其分为五类：

- 复合语句：组织其他语句的容器
- 表达式语句：执行表达式操作
- 选择语句：根据条件选择执行路径
- 迭代语句：重复执行代码块
- 跳转语句：改变程序执行顺序

这种分类反映了结构化程序设计的核心思想：顺序、选择、循环。

## 2. 语句类型详解

### 2.1 五种语句类型的特征对比

| 类型 | 特征 | 用途 | 控制流影响 |
|------|------|------|------------|
| 复合语句 | `{}`包围的语句序列 | 组织代码块 | 引入新作用域 |
| 表达式语句 | 表达式+分号 | 执行操作 | 顺序执行 |
| 选择语句 | 条件分支 | 决策控制 | 改变执行路径 |
| 迭代语句 | 循环执行 | 重复操作 | 循环控制 |
| 跳转语句 | 无条件转移 | 流程控制 | 跳转执行 |

### 2.2 语句的语法结构

每种语句都有其特定的语法结构：

```c
// 复合语句
{ statement-list }

// 表达式语句
expression;

// 选择语句
if (expression) statement
if (expression) statement else statement
switch (expression) statement

// 迭代语句
while (expression) statement
do statement while (expression);
for (expression; expression; expression) statement

// 跳转语句
break;
continue;
return expression;
goto identifier;
```

### 2.3 语句的嵌套特性

C 语言允许语句的任意嵌套，这为复杂的程序逻辑提供了支持：

```c
#include <stdio.h>

int main() {
    int i, j, k;
    
    // 三层嵌套循环
    for (i = 0; i < 3; i++) {
        printf("Outer loop: %d\n", i);
        
        for (j = 0; j < 2; j++) {
            printf("  Middle loop: %d\n", j);
            
            for (k = 0; k < 2; k++) {
                printf("    Inner loop: %d\n", k);
                
                // 条件语句嵌套
                if (i == j && j == k) {
                    printf("    *** All equal: %d ***\n", i);
                }
            }
        }
    }
    
    return 0;
}
```

## 3. 标签语句深入分析

### 3.1 标签的语法与语义

标签（Label）是用于标记语句的标识符，允许跳转语句跳转到该位置。标签具有函数作用域，必须在同一个函数内唯一。

```c
#include <stdio.h>

int main() {
    int x = 10;
    
    // 普通标签
    start:
        printf("Start of program\n");
        
        if (x > 5) {
            goto middle;
        }
        
        printf("This won't be printed\n");
        
    middle:
        printf("Middle section\n");
        
        if (x < 20) {
            goto end;
        }
        
        printf("This also won't be printed\n");
        
    end:
        printf("End of program\n");
        
    return 0;
}
```

### 3.2 三种标签类型详解

**普通标签**:

普通标签用于 `goto` 语句的跳转目标：

```c
#include <stdio.h>

void demonstrate_goto() {
    int i = 0;
    
    loop_start:
        if (i >= 5) {
            goto loop_end;
        }
        
        printf("Iteration %d\n", i);
        i++;
        goto loop_start;
        
    loop_end:
        printf("Loop finished\n");
}
```

**Case 标签**:

Case 标签用于 `switch` 语句中的分支：

```c
#include <stdio.h>

void demonstrate_switch() {
    int choice = 2;
    
    switch (choice) {
        case 1:
            printf("Choice 1 selected\n");
            break;
            
        case 2:
            printf("Choice 2 selected\n");
            // fall-through to case 3
            
        case 3:
            printf("Choice 3 selected (fall-through)\n");
            break;
            
        case 4:
        case 5:
            printf("Choice 4 or 5 selected\n");
            break;
            
        default:
            printf("Invalid choice\n");
    }
}
```

**Default 标签**:

Default 标签用于 `switch` 语句中的默认分支：

```c
#include <stdio.h>

void demonstrate_default() {
    char grade = 'X';
    
    switch (grade) {
        case 'A':
            printf("Excellent\n");
            break;
            
        case 'B':
            printf("Good\n");
            break;
            
        case 'C':
            printf("Average\n");
            break;
            
        case 'D':
            printf("Below Average\n");
            break;
            
        case 'F':
            printf("Fail\n");
            break;
            
        default:
            printf("Invalid grade: %c\n", grade);
    }
}
```

### 3.3 标签的作用域与生命周期

标签具有函数作用域，这意味着：

- 标签在声明它的函数内可见
- 同一函数内的标签名称必须唯一
- 标签不能跨越函数边界

```c
#include <stdio.h>

void function1() {
    label1:
        printf("In function1\n");
        goto label2;  // 错误：无法跳转到其他函数的标签
}

void function2() {
    label2:
        printf("In function2\n");
}

int main() {
    function1();
    return 0;
}
```

### 3.4 标签的高级用法

**多重标签**：

一个语句可以有多个标签：

```c
#include <stdio.h>

int main() {
    int x = 1;
    
    switch (x) {
        case 1:
        case 2:
        case 3:
            printf("Small number: %d\n", x);
            break;
            
        case 4:
        case 5:
            printf("Medium number: %d\n", x);
            break;
            
        default:
            printf("Large number: %d\n", x);
    }
    
    return 0;
}
```

**标签与复合语句**：

标签可以标记复合语句：

```c
#include <stdio.h>

int main() {
    int i = 0;
    
    outer_loop:
    {
        int j = 0;
        
        inner_loop:
        {
            if (i >= 3) {
                goto end_program;
            }
            
            if (j >= 2) {
                i++;
                goto outer_loop;
            }
            
            printf("i=%d, j=%d\n", i, j);
            j++;
            goto inner_loop;
        }
    }
    
    end_program:
        printf("Program ended\n");
        
    return 0;
}
```

## 4. 复合语句与作用域

### 4.1 复合语句的本质

复合语句（Compound Statement）是由大括号 `{}` 包围的语句和声明序列。它不仅是代码组织的工具，更是作用域管理的核心机制。

```c
#include <stdio.h>

int global_var = 100;  // 全局作用域

int main() {
    int outer_var = 10;  // main函数作用域
    
    {
        int inner_var = 20;  // 块作用域
        printf("Global: %d, Outer: %d, Inner: %d\n", 
               global_var, outer_var, inner_var);
        
        {
            int nested_var = 30;  // 嵌套块作用域
            printf("All vars: %d, %d, %d, %d\n", 
                   global_var, outer_var, inner_var, nested_var);
        }
        
        // printf("%d\n", nested_var);  // 错误：超出作用域
    }
    
    // printf("%d\n", inner_var);  // 错误：超出作用域
    
    return 0;
}
```

### 4.2 块作用域的规则

**作用域嵌套规则**：

```c
#include <stdio.h>

int x = 100;  // 全局变量

int main() {
    int x = 10;  // 局部变量，遮蔽全局变量
    
    {
        int x = 20;  // 内层局部变量，遮蔽外层变量
        printf("Inner x: %d\n", x);  // 输出 20
        
        {
            printf("Still inner x: %d\n", x);  // 输出 20
        }
    }
    
    printf("Outer x: %d\n", x);  // 输出 10
    printf("Global x: %d\n", ::x);  // C++语法，C中需用不同名称
    
    return 0;
}
```

**变量生命周期**：

```c
#include <stdio.h>

void demonstrate_lifetime() {
    printf("=== Function start ===\n");
    
    {
        int block_var = 42;
        printf("Block variable created: %d\n", block_var);
        
        {
            int nested_var = 84;
            printf("Nested variable created: %d\n", nested_var);
            printf("Block variable still accessible: %d\n", block_var);
        }
        
        printf("Nested variable destroyed\n");
        // printf("%d\n", nested_var);  // 错误：变量已销毁
        
        printf("Block variable still accessible: %d\n", block_var);
    }
    
    printf("Block variable destroyed\n");
    // printf("%d\n", block_var);  // 错误：变量已销毁
    
    printf("=== Function end ===\n");
}
```

### 4.3 复合语句的高级应用

**作用域隔离**：

```c
#include <stdio.h>

#define SWAP(a, b, type) do { \
    type temp_swap_var = (a); \
    (a) = (b); \
    (b) = temp_swap_var; \
} while(0)

int main() {
    int x = 10, y = 20;
    printf("Before swap: x=%d, y=%d\n", x, y);
    
    SWAP(x, y, int);
    
    printf("After swap: x=%d, y=%d\n", x, y);
    
    // temp_swap_var 在宏中定义，不会污染外部作用域
    return 0;
}
```

**条件编译与复合语句**：

```c
#include <stdio.h>

#define DEBUG_MODE 1

int main() {
    int value = 42;
    
    #ifdef DEBUG_MODE
    {
        int debug_temp = value * 2;
        printf("[DEBUG] Processing value: %d, temp: %d\n", value, debug_temp);
        
        {
            char debug_buffer[256];
            sprintf(debug_buffer, "Value squared: %d", value * value);
            printf("[DEBUG] %s\n", debug_buffer);
        }
    }
    #endif
    
    printf("Final value: %d\n", value);
    return 0;
}
```

## 5. 表达式语句详解

### 5.1 表达式语句的本质

表达式语句是以分号 `;` 结尾的表达式。它是 C 程序中最常见的语句类型，用于执行各种操作。

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int a = 5, b = 10;
    
    // 赋值表达式语句
    a = b + 3;
    
    // 函数调用表达式语句
    printf("a = %d\n", a);
    
    // 自增/自减表达式语句
    a++;
    --b;
    
    // 逗号表达式语句
    int result = (a++, b--, a + b);
    
    // 条件表达式语句
    int max = (a > b) ? a : b;
    
    printf("Result: %d, Max: %d\n", result, max);
    
    return 0;
}
```

### 5.2 空语句的用途

空语句是没有表达式的表达式语句，通常用于循环体或携带标签。

```c
#include <stdio.h>
#include <string.h>

void demonstrate_null_statements() {
    // 1. 用于循环体
    char str[] = "Hello, World!";
    char *ptr = str;
    
    // 跳过所有非字母字符
    while (*ptr && !((*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z')))
        ptr++;  // 空语句
    
    printf("First letter: %c\n", *ptr);
    
    // 2. 用于携带标签
    int i = 0;
    
    loop_start:
        if (i >= 5) {
            goto loop_end;
        }
        
        printf("Count: %d\n", i);
        i++;
        ;
        goto loop_start;
        
    loop_end:
        printf("Loop finished\n");
}
```

### 5.3 表达式语句的副作用

许多表达式语句具有副作用，即除了计算值外还会改变程序状态：

```c
#include <stdio.h>

int global_counter = 0;

int increment_counter() {
    return ++global_counter;
}

int main() {
    int x = 5;
    
    // 1. 赋值操作的副作用
    int y = (x = 10);  // x 被修改，y 获得新值
    printf("x = %d, y = %d\n", x, y);
    
    // 2. 自增/自减的副作用
    int arr[] = {1, 2, 3, 4, 5};
    int *ptr = arr;
    
    printf("Array elements: ");
    int i;
    for (i = 0; i < 5; i++) {
        printf("%d ", *ptr++);  // ptr 被修改
    }
    printf("\n");
    
    // 3. 函数调用的副作用
    printf("Counter before: %d\n", global_counter);
    int new_value = increment_counter();
    printf("Counter after: %d, returned: %d\n", global_counter, new_value);
    
    // 4. 逗号表达式的副作用
    int a = 1, b = 2;
    int result = (a++, b++, a + b);  // a 和 b 都被修改
    printf("a = %d, b = %d, result = %d\n", a, b, result);
    
    return 0;
}
```

### 5.4 表达式语句的最佳实践

**避免复杂的副作用**：

```c
#include <stdio.h>

int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// 危险的用法：在表达式中多次修改同一变量
void dangerous_usage() {
    int i = 0;
    // arr[i] = i++;  // 未定义行为：i 被多次修改
    // arr[i++] = i;  // 未定义行为：i 被多次修改
    
    // 安全的用法
    arr[i] = i;
    i++;
}

// 安全的用法示例
void safe_usage() {
    int i = 0;
    
    // 清晰的顺序
    arr[i] = 42;
    i++;
    
    // 或者使用逗号表达式确保顺序
    int j = 0;
    (arr[j] = 99, j++);
    
    printf("Safe assignments completed\n");
}
```

**函数调用的副作用管理**：

```c
#include <stdio.h>
#include <time.h>

// 具有副作用的函数
time_t get_current_time() {
    static int call_count = 0;
    call_count++;
    printf("Function called %d times\n", call_count);
    return time(NULL);
}

int main() {
    // 每次调用都有副作用
    time_t t1 = get_current_time();
    time_t t2 = get_current_time();
    time_t t3 = get_current_time();
    
    printf("Times: %ld, %ld, %ld\n", t1, t2, t3);
    
    return 0;
}
```

## 6. 选择语句深入探讨

### 6.1 `if` 语句的详细分析

**基本语法与执行流程**：

```c
#include <stdio.h>

void basic_if_examples() {
    int x = 10;
    
    // 简单 if 语句
    if (x > 5) {
        printf("x is greater than 5\n");
    }
    
    // if-else 语句
    if (x > 15) {
        printf("x is greater than 15\n");
    } else {
        printf("x is not greater than 15\n");
    }
    
    // 嵌套 if 语句
    if (x > 0) {
        if (x < 20) {
            printf("x is between 0 and 20\n");
        } else {
            printf("x is 20 or greater\n");
        }
    } else {
        printf("x is zero or negative\n");
    }
}
```

**悬空 else 问题**：

```c
#include <stdio.h>

void dangling_else_problem() {
    int a = 5, b = 10, c = 15;
    
    // 可能引起混淆的代码
    if (a > 0)
        if (b > 0)
            printf("Both a and b are positive\n");
        else
            printf("This else belongs to the inner if!\n");
    
    // 使用大括号明确意图
    if (a > 0) {
        if (b > 0) {
            printf("Both a and b are positive\n");
        }
    } else {
        printf("This else belongs to the outer if\n");
    }
}
```

**条件表达式的类型转换**：

```c
#include <stdio.h>

void condition_evaluation() {
    // 整数条件
    int x = 0;
    if (x) {
        printf("x is non-zero\n");
    } else {
        printf("x is zero\n");
    }
    
    // 指针条件
    char *ptr = NULL;
    if (ptr) {
        printf("Pointer is not NULL\n");
    } else {
        printf("Pointer is NULL\n");
    }
    
    ptr = "Hello";
    if (ptr) {
        printf("Pointer points to: %s\n", ptr);
    }
    
    // 浮点数条件（注意精度问题）
    double d = 0.1 + 0.2 - 0.3;
    if (d == 0.0) {
        printf("d equals 0.0\n");
    } else {
        printf("d = %.20f (not exactly 0 due to floating point precision)\n", d);
    }
    
    // 使用 epsilon 比较浮点数
    const double epsilon = 1e-10;
    if (fabs(d) < epsilon) {
        printf("d is effectively zero\n");
    }
}
```

### 6.2 `switch` 语句深入分析

**基本语法与执行机制**：

```c
#include <stdio.h>

void basic_switch_examples() {
    int choice = 2;
    
    // 基本 switch 语句
    switch (choice) {
        case 1:
            printf("Choice 1\n");
            break;
            
        case 2:
            printf("Choice 2\n");
            break;
            
        case 3:
            printf("Choice 3\n");
            break;
            
        default:
            printf("Invalid choice\n");
    }
    
    // 没有 break 的 switch（fall-through）
    printf("\nFall-through example:\n");
    switch (choice) {
        case 1:
            printf("Case 1\n");
            // fall-through
            
        case 2:
            printf("Case 2\n");
            // fall-through
            
        case 3:
            printf("Case 3\n");
            break;
            
        default:
            printf("Default case\n");
    }
}
```

**switch 语句的限制与特性**：

```c
#include <stdio.h>

void switch_limitations() {
    int value = 2;
    char ch = 'B';
    
    // 1. switch 只能用于整型表达式
    switch (value) {
        case 1:
            printf("Integer case 1\n");
            break;
            
        case 2:
            printf("Integer case 2\n");
            break;
    }
    
    // 2. 字符在 switch 中被视为整数
    switch (ch) {
        case 'A':
            printf("Character A\n");
            break;
            
        case 'B':
            printf("Character B\n");
            break;
            
        case 'C':
            printf("Character C\n");
            break;
    }
    
    // 3. case 标签必须是常量表达式
    const int CONSTANT = 3;
    switch (value) {
        case CONSTANT:  // 合法：const 变量在 C 中可以作为 case 标签
            printf("Constant case\n");
            break;
            
        // case value:  // 非法：变量不能作为 case 标签
        //     printf("Variable case\n");
        //     break;
    }
}
```

**switch 语句的最佳实践**：

```c
#include <stdio.h>

// 使用枚举提高 switch 的可读性
typedef enum {
    RED = 1,
    GREEN = 2,
    BLUE = 3
} Color;

void color_handler(Color color) {
    switch (color) {
        case RED:
            printf("Red color selected\n");
            break;
            
        case GREEN:
            printf("Green color selected\n");
            break;
            
        case BLUE:
            printf("Blue color selected\n");
            break;
            
        default:
            printf("Unknown color\n");
            break;
    }
}

// 处理多个相同情况
void grade_handler(char grade) {
    switch (grade) {
        case 'A':
        case 'B':
        case 'C':
            printf("Passing grade: %c\n", grade);
            break;
            
        case 'D':
        case 'F':
            printf("Failing grade: %c\n", grade);
            break;
            
        default:
            printf("Invalid grade: %c\n", grade);
    }
}

int main() {
    color_handler(RED);
    color_handler(GREEN);
    
    grade_handler('A');
    grade_handler('D');
    grade_handler('X');
    
    return 0;
}
```

### 6.3 选择语句的性能考虑

```c
#include <stdio.h>
#include <time.h>

// if-else 链的性能
void if_else_performance(int value) {
    clock_t start = clock();
    
    for (int i = 0; i < 1000000; i++) {
        if (value == 1) {
            // 处理情况1
        } else if (value == 2) {
            // 处理情况2
        } else if (value == 3) {
            // 处理情况3
        } else if (value == 4) {
            // 处理情况4
        } else {
            // 默认处理
        }
    }
    
    clock_t end = clock();
    printf("if-else chain time: %f seconds\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC);
}

// switch 语句的性能
void switch_performance(int value) {
    clock_t start = clock();
    
    for (int i = 0; i < 1000000; i++) {
        switch (value) {
            case 1:
                // 处理情况1
                break;
                
            case 2:
                // 处理情况2
                break;
                
            case 3:
                // 处理情况3
                break;
                
            case 4:
                // 处理情况4
                break;
                
            default:
                // 默认处理
                break;
        }
    }
    
    clock_t end = clock();
    printf("switch statement time: %f seconds\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC);
}
```

## 7. 迭代语句全面解析

### 7.1 `while` 循环详解

**基本语法与执行流程**：

```c
#include <stdio.h>

void while_loop_examples() {
    // 基本 while 循环
    int i = 0;
    while (i < 5) {
        printf("While loop iteration: %d\n", i);
        i++;
    }
    
    // 条件在循环体末尾检查的变体
    printf("\nModified while loop:\n");
    i = 0;
    while (1) {  // 无限循环
        printf("Modified while iteration: %d\n", i);
        i++;
        if (i >= 5) {
            break;
        }
    }
    
    // 空循环体的 while 循环
    printf("\nEmpty body while loop:\n");
    char str[] = "Hello";
    char *ptr = str;
    while (*ptr++ != '\0')
        ;  // 空语句，用于移动指针到字符串末尾
    
    printf("String length: %ld\n", ptr - str - 1);
}
```

**while 循环的常见应用场景**：

```c
#include <stdio.h>
#include <stdlib.h>

void while_loop_applications() {
    // 1. 用户输入验证
    int number;
    printf("Enter a positive number: ");
    while (scanf("%d", &number) != 1 || number <= 0) {
        printf("Invalid input. Enter a positive number: ");
        while (getchar() != '\n');  // 清空输入缓冲区
    }
    printf("Valid number entered: %d\n", number);
    
    // 2. 文件读取
    FILE *file = fopen("example.txt", "r");
    if (file) {
        int ch;
        printf("File contents:\n");
        while ((ch = fgetc(file)) != EOF) {
            putchar(ch);
        }
        fclose(file);
    }
    
    // 3. 链表遍历
    struct Node {
        int data;
        struct Node *next;
    };
    
    struct Node *head = NULL;
    // 假设链表已构建...
    
    struct Node *current = head;
    while (current != NULL) {
        printf("Node data: %d\n", current->data);
        current = current->next;
    }
}
```

### 7.2 `do-while` 循环详解

**基本语法与执行流程**：

```c
#include <stdio.h>

void do_while_examples() {
    // 基本 do-while 循环
    int i = 0;
    do {
        printf("Do-while iteration: %d\n", i);
        i++;
    } while (i < 5);
    
    // 至少执行一次的特性
    printf("\nAt least once execution:\n");
    i = 10;  // 初始条件不满足 while 条件
    do {
        printf("This will execute at least once: %d\n", i);
        i++;
    } while (i < 5);
    
    // 菜单驱动程序示例
    printf("\nMenu example:\n");
    int choice;
    do {
        printf("1. Option 1\n");
        printf("2. Option 2\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // 清空输入缓冲区
            choice = 0;  // 重置选择
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Option 1 selected\n");
                break;
                
            case 2:
                printf("Option 2 selected\n");
                break;
                
            case 3:
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 3);
}
```

**do-while 与 while 的比较**：

```c
#include <stdio.h>

void compare_while_do_while() {
    int condition = 0;
    
    printf("While loop with false condition:\n");
    while (condition) {
        printf("This will not execute\n");
    }
    
    printf("Do-while loop with false condition:\n");
    do {
        printf("This will execute once\n");
    } while (condition);
    
    // 实际应用场景对比
    printf("\nUser input validation:\n");
    
    // 使用 while（可能一次都不执行）
    printf("While version:\n");
    int number1;
    while (printf("Enter a number > 0: ") && 
           scanf("%d", &number1) == 1 && 
           number1 <= 0) {
        printf("Invalid number\n");
    }
    
    // 使用 do-while（至少执行一次提示）
    printf("Do-while version:\n");
    int number2;
    do {
        printf("Enter a number > 0: ");
        if (scanf("%d", &number2) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            number2 = 0;
        } else if (number2 <= 0) {
            printf("Number must be positive\n");
        }
    } while (number2 <= 0);
}
```

### 7.3 `for` 循环详解

**基本语法与执行流程**：

```c
#include <stdio.h>

void for_loop_examples() {
    // 基本 for 循环
    printf("Basic for loop:\n");
    for (int i = 0; i < 5; i++) {
        printf("Iteration %d\n", i);
    }
    
    // 多个初始化和更新表达式
    printf("\nMultiple expressions:\n");
    for (int i = 0, j = 10; i < 5 && j > 5; i++, j--) {
        printf("i = %d, j = %d\n", i, j);
    }
    
    // 省略部分表达式
    printf("\nOmitted expressions:\n");
    int k = 0;
    for (; k < 3; ) {
        printf("k = %d\n", k);
        k++;
    }
    
    // 无限循环
    printf("\nInfinite for loop (first 5 iterations):\n");
    int count = 0;
    for (;;) {
        printf("Infinite loop iteration %d\n", count);
        count++;
        if (count >= 5) {
            break;
        }
    }
}
```

**for 循环的高级用法**：

```c
#include <stdio.h>

void advanced_for_loops() {
    // 1. 数组遍历
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    printf("Array traversal:\n");
    for (int i = 0; i < size; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    
    // 2. 逆序遍历
    printf("\nReverse traversal:\n");
    for (int i = size - 1; i >= 0; i--) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    
    // 3. 步长不为1的遍历
    printf("\nStep size traversal:\n");
    for (int i = 0; i < size; i += 2) {
        printf("Even indices: arr[%d] = %d\n", i, arr[i]);
    }
    
    // 4. 嵌套循环
    printf("\nNested loops:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("(%d, %d) ", i, j);
        }
        printf("\n");
    }
    
    // 5. 循环变量的作用域
    printf("\nScope demonstration:\n");
    for (int i = 0; i < 2; i++) {
        printf("Outer loop i = %d\n", i);
        
        for (int i = 0; i < 2; i++) {  // 内层 i 遮蔽外层 i
            printf("  Inner loop i = %d\n", i);
        }
        
        // printf("Outer i after inner loop: %d\n", i);  // 外层 i 仍然可用
    }
}
```

**C99 之后的 for 循环特性**：

```c
#include <stdio.h>

void c99_for_features() {
    // 1. 循环变量的块作用域（C99+）
    printf("Block scope variables:\n");
    for (int i = 0; i < 3; i++) {
        printf("Loop 1: i = %d\n", i);
    }
    
    // for (int i = 0; i < 3; i++) {  // 可以重新声明同名变量
    //     printf("Loop 2: i = %d\n", i);
    // }
    
    // 2. 复杂的初始化表达式
    printf("\nComplex initialization:\n");
    for (int i = 0, j = 10, k = i + j; i < 5; i++, k = i + j) {
        printf("i = %d, j = %d, k = %d\n", i, j, k);
    }
    
    // 3. 指针遍历
    printf("\nPointer traversal:\n");
    char str[] = "Hello";
    for (char *ptr = str; *ptr != '\0'; ptr++) {
        printf("Character: %c\n", *ptr);
    }
}
```

### 7.4 迭代语句的性能与优化

```c
#include <stdio.h>
#include <time.h>

#define SIZE 1000000

void performance_comparison() {
    int arr[SIZE];
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }
    
    clock_t start, end;
    
    // 1. 基本 for 循环
    start = clock();
    long long sum1 = 0;
    for (int i = 0; i < SIZE; i++) {
        sum1 += arr[i];
    }
    end = clock();
    printf("Basic for loop time: %f seconds, sum = %lld\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC, sum1);
    
    // 2. 指针遍历
    start = clock();
    long long sum2 = 0;
    int *ptr = arr;
    int *end_ptr = arr + SIZE;
    while (ptr < end_ptr) {
        sum2 += *ptr++;
    }
    end = clock();
    printf("Pointer traversal time: %f seconds, sum = %lld\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC, sum2);
    
    // 3. 递减循环（某些架构优化）
    start = clock();
    long long sum3 = 0;
    for (int i = SIZE - 1; i >= 0; i--) {
        sum3 += arr[i];
    }
    end = clock();
    printf("Decrementing loop time: %f seconds, sum = %lld\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC, sum3);
}
```

## 8. 跳转语句深度剖析

### 8.1 `break` 语句详解

**基本用法**：

```c
#include <stdio.h>

void break_statement_examples() {
    // 1. 在循环中使用 break
    printf("Break in for loop:\n");
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            printf("Breaking at i = %d\n", i);
            break;
        }
        printf("i = %d\n", i);
    }
    
    // 2. 在 switch 中使用 break
    printf("\nBreak in switch:\n");
    int choice = 2;
    switch (choice) {
        case 1:
            printf("Case 1\n");
            break;  // 防止 fall-through
            
        case 2:
            printf("Case 2\n");
            break;
            
        case 3:
            printf("Case 3\n");
            break;
            
        default:
            printf("Default case\n");
    }
    
    // 3. 嵌套循环中的 break
    printf("\nBreak in nested loops:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) {
                printf("Breaking inner loop at (%d, %d)\n", i, j);
                break;
            }
            printf("  (%d, %d)\n", i, j);
        }
        printf("Outer loop i = %d\n", i);
    }
}
```

**break 的限制与注意事项**：

```c
#include <stdio.h>

void break_limitations() {
    // 1. break 只能跳出最近的一层循环或 switch
    printf("Break limitation demonstration:\n");
    for (int i = 0; i < 2; i++) {
        printf("Outer loop: %d\n", i);
        
        for (int j = 0; j < 3; j++) {
            printf("  Inner loop: %d\n", j);
            
            if (j == 1) {
                printf("    Breaking inner loop\n");
                break;  // 只跳出内层循环
            }
        }
        printf("Back to outer loop\n");
    }
    
    // 2. 在非循环/switch 语句中使用 break 是错误的
    // if (1) {
    //     break;  // 编译错误
    // }
}
```

### 8.2 `continue` 语句详解

**基本用法**：

```c
#include <stdio.h>

void continue_statement_examples() {
    // 1. 在 for 循环中使用 continue
    printf("Continue in for loop:\n");
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            printf("Skipping even number: %d\n", i);
            continue;
        }
        printf("Processing odd number: %d\n", i);
    }
    
    // 2. 在 while 循环中使用 continue
    printf("\nContinue in while loop:\n");
    int j = 0;
    while (j < 5) {
        j++;
        if (j == 3) {
            printf("Skipping iteration 3\n");
            continue;
        }
        printf("Processing iteration: %d\n", j);
    }
    
    // 3. 在 do-while 循环中使用 continue
    printf("\nContinue in do-while loop:\n");
    int k = 0;
    do {
        k++;
        if (k % 3 == 0) {
            printf("Skipping multiple of 3: %d\n", k);
            continue;
        }
        printf("Processing: %d\n", k);
    } while (k < 10);
}
```

**continue 与 break 的对比**：

```c
#include <stdio.h>

void continue_vs_break() {
    printf("Using break:\n");
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            printf("Breaking at %d\n", i);
            break;  // 完全退出循环
        }
        printf("Processing %d\n", i);
    }
    
    printf("\nUsing continue:\n");
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            printf("Continuing at %d\n", i);
            continue;  // 跳过本次迭代，继续下一次
        }
        printf("Processing %d\n", i);
    }
}
```

### 8.3 `return` 语句详解

**基本用法**：

```c
#include <stdio.h>
#include <stdlib.h>

// 无返回值函数
void print_message(const char *msg) {
    if (msg == NULL) {
        printf("Error: NULL message\n");
        return;  // 提前返回
    }
    printf("Message: %s\n", msg);
}

// 有返回值函数
int calculate_square(int x) {
    if (x < 0) {
        printf("Warning: Negative input\n");
        return 0;  // 返回默认值
    }
    return x * x;
}

// 多个返回点
int find_max(int a, int b, int c) {
    if (a >= b && a >= c) {
        return a;
    }
    if (b >= c) {
        return b;
    }
    return c;
}

void return_examples() {
    print_message("Hello, World!");
    print_message(NULL);
    
    printf("Square of 5: %d\n", calculate_square(5));
    printf("Square of -3: %d\n", calculate_square(-3));
    
    printf("Max of 3, 7, 2: %d\n", find_max(3, 7, 2));
}
```

**return 语句的高级用法**：

```c
#include <stdio.h>
#include <stdlib.h>

// 早期返回优化
int *find_element(int *arr, int size, int target) {
    if (arr == NULL || size <= 0) {
        return NULL;  // 早期返回，避免深层嵌套
    }
    
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return &arr[i];
        }
    }
    
    return NULL;  // 未找到
}

// 错误处理中的 return
int safe_divide(int a, int b, int *result) {
    if (b == 0) {
        return -1;  // 错误代码
    }
    
    *result = a / b;
    return 0;  // 成功
}

void advanced_return_examples() {
    int arr[] = {1, 2, 3, 4, 5};
    int *found = find_element(arr, 5, 3);
    if (found) {
        printf("Found element: %d\n", *found);
    } else {
        printf("Element not found\n");
    }
    
    int result;
    if (safe_divide(10, 2, &result) == 0) {
        printf("Division result: %d\n", result);
    } else {
        printf("Division by zero error\n");
    }
}
```

### 8.4 `goto` 语句详解

**基本用法与争议**：

```c
#include <stdio.h>

void goto_basic_examples() {
    int i = 0;
    
    // 基本的 goto 使用
    if (i == 0) {
        goto skip_initialization;
    }
    
    printf("This won't be printed\n");
    
skip_initialization:
    printf("Skipped initialization\n");
    
    // 循环模拟
    printf("\nGoto-based loop:\n");
    int j = 0;
    
loop_start:
    if (j >= 5) {
        goto loop_end;
    }
    
    printf("Iteration %d\n", j);
    j++;
    goto loop_start;
    
loop_end:
    printf("Loop finished\n");
}

// 错误处理中的 goto（推荐用法）
int complex_function() {
    int *ptr1 = NULL;
    int *ptr2 = NULL;
    FILE *file = NULL;
    int result = -1;
    
    ptr1 = malloc(sizeof(int));
    if (ptr1 == NULL) {
        goto cleanup;
    }
    
    ptr2 = malloc(sizeof(int));
    if (ptr2 == NULL) {
        goto cleanup;
    }
    
    file = fopen("data.txt", "r");
    if (file == NULL) {
        goto cleanup;
    }
    
    // 正常处理逻辑
    *ptr1 = 42;
    *ptr2 = 84;
    result = 0;  // 成功
    
cleanup:
    if (file) {
        fclose(file);
    }
    if (ptr2) {
        free(ptr2);
    }
    if (ptr1) {
        free(ptr1);
    }
    
    return result;
}
```

**goto 的合理使用场景**：

```c
#include <stdio.h>
#include <stdlib.h>

// 状态机实现
typedef enum {
    STATE_START,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

void state_machine_example() {
    State current_state = STATE_START;
    
state_start:
    printf("Entering START state\n");
    current_state = STATE_PROCESSING;
    goto state_processing;
    
state_processing:
    printf("Entering PROCESSING state\n");
    // 模拟处理逻辑
    int success = 1;  // 假设处理成功
    if (success) {
        current_state = STATE_FINISHED;
        goto state_finished;
    } else {
        current_state = STATE_ERROR;
        goto state_error;
    }
    
state_finished:
    printf("Entering FINISHED state\n");
    goto state_end;
    
state_error:
    printf("Entering ERROR state\n");
    goto state_end;
    
state_end:
    printf("State machine ended\n");
}

// 多层嵌套循环的跳出
void nested_loop_break_example() {
    int found = 0;
    
    for (int i = 0; i < 10 && !found; i++) {
        for (int j = 0; j < 10 && !found; j++) {
            for (int k = 0; k < 10 && !found; k++) {
                if (i + j + k == 15) {
                    printf("Found combination: %d + %d + %d = 15\n", i, j, k);
                    found = 1;
                    goto search_complete;  // 一次性跳出所有循环
                }
            }
        }
    }
    
search_complete:
    if (found) {
        printf("Search completed successfully\n");
    } else {
        printf("No combination found\n");
    }
}
```

## 9. 属性说明符（C23）

### 9.1 属性说明符的引入背景

C23 标准引入了属性说明符（Attribute Specifier），允许为语句、声明等附加元信息。这一特性增强了代码的可读性、可维护性和编译器优化能力。

```c
#include <stdio.h>

// C23 属性说明符示例
[[deprecated("Use new_function instead")]]
int old_function(int x) {
    return x * 2;
}

[[nodiscard]]
int important_calculation(int a, int b) {
    return a + b;
}

void attribute_examples() {
    int result1 = old_function(5);  // 编译器可能发出警告
    
    int result2 = important_calculation(3, 4);  // 编译器可能警告未使用返回值
    // [[maybe_unused]] int unused_result = important_calculation(1, 2);  // 抑制未使用警告
    
    printf("Results: %d, %d\n", result1, result2);
}
```

### 9.2 常用属性说明符

**`[[deprecated]]` 属性**：

```c
#include <stdio.h>

[[deprecated("This function is deprecated, use calculate_new instead")]]
int calculate_old(int x, int y) {
    return x + y;
}

int calculate_new(int x, int y) {
    return x * y;
}

void deprecated_example() {
    int old_result = calculate_old(3, 4);    // 可能产生警告
    int new_result = calculate_new(3, 4);
    
    printf("Old result: %d, New result: %d\n", old_result, new_result);
}
```

**`[[nodiscard]]` 属性**：

```c
#include <stdio.h>

[[nodiscard]]
int get_error_code() {
    return -1;  // 模拟错误码
}

[[nodiscard]]
void* allocate_memory(size_t size) {
    return malloc(size);
}

void nodiscard_example() {
    get_error_code();  // 编译器可能警告未使用返回值
    
    void *ptr = allocate_memory(100);
    if (ptr) {
        // 使用分配的内存
        free(ptr);
    }
}
```

**`[[maybe_unused]]` 属性**：

```c
#include <stdio.h>

void maybe_unused_example() {
    [[maybe_unused]] int debug_variable = 42;
    
#ifdef DEBUG
    printf("Debug variable: %d\n", debug_variable);
#endif
    
    // 即使在非调试模式下，也不会产生未使用变量警告
}
```

### 9.3 属性说明符在语句中的应用

```c
#include <stdio.h>

void statement_attributes_example() {
    int x = 10;
    
    // 在 if 语句中使用属性
    [[likely]] if (x > 0) {
        printf("Positive number\n");
    } else {
        printf("Non-positive number\n");
    }
    
    // 在循环中使用属性
    for (int i = 0; i < 100; i++) {
        [[unlikely]] if (i == 99) {
            printf("Last iteration\n");
            break;
        }
        // 处理其他情况
    }
    
    // 在 switch 语句中使用属性
    switch (x) {
        case 10:
            [[fallthrough]];  // 明确表示贯穿执行
        case 20:
            printf("x is 10 or 20\n");
            break;
            
        default:
            printf("Other value\n");
    }
}
```

---

## 10. 语句执行顺序与控制流

### 10.1 顺序执行

C 语言中的语句默认按照顺序执行（Sequential Execution），即按照代码书写的顺序依次执行。

```c
#include <stdio.h>

void sequential_execution() {
    printf("Statement 1\n");
    printf("Statement 2\n");
    printf("Statement 3\n");
    int x = 5;
    printf("Statement 4: x = %d\n", x);
    x = x * 2;
    printf("Statement 5: x = %d\n", x);
}
```

### 10.2 选择执行

选择语句根据条件决定执行哪条路径：

```c
#include <stdio.h>

void conditional_execution(int value) {
    printf("Input value: %d\n", value);
    
    if (value > 0) {
        printf("Positive number\n");
        if (value > 100) {
            printf("Large positive number\n");
        } else {
            printf("Small positive number\n");
        }
    } else if (value < 0) {
        printf("Negative number\n");
    } else {
        printf("Zero\n");
    }
}
```

### 10.3 循环执行

循环语句重复执行代码块：

```c
#include <stdio.h>

void loop_execution() {
    printf("For loop:\n");
    for (int i = 0; i < 3; i++) {
        printf("  Iteration %d\n", i);
    }
    
    printf("While loop:\n");
    int j = 0;
    while (j < 3) {
        printf("  Iteration %d\n", j);
        j++;
    }
    
    printf("Do-while loop:\n");
    int k = 0;
    do {
        printf("  Iteration %d\n", k);
        k++;
    } while (k < 3);
}
```

### 10.4 跳转执行

跳转语句改变正常的执行顺序：

```c
#include <stdio.h>

void jump_execution() {
    printf("Start\n");
    
    goto middle;
    
    printf("This won't be printed\n");
    
middle:
    printf("Middle section\n");
    
    for (int i = 0; i < 5; i++) {
        if (i == 2) {
            printf("Breaking at i = %d\n", i);
            break;
        }
        if (i == 1) {
            printf("Continuing at i = %d\n", i);
            continue;
        }
        printf("Processing i = %d\n", i);
    }
    
    printf("End\n");
}
```

### 10.5 复杂控制流示例

```c
#include <stdio.h>

void complex_control_flow() {
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    printf("Matrix traversal with early exit:\n");
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix[i][j] == 5) {
                printf("Found target value 5 at position (%d, %d)\n", i, j);
                goto found_target;
            }
            
            if (matrix[i][j] % 2 == 0) {
                printf("Skipping even number %d at (%d, %d)\n", matrix[i][j], i, j);
                continue;
            }
            
            printf("Processing odd number %d at (%d, %d)\n", matrix[i][j], i, j);
        }
    }
    
found_target:
    printf("Search completed\n");
}
```

## 11. 常见陷阱与最佳实践

### 11.1 常见陷阱

**悬空 else 问题**：

```c
#include <stdio.h>

void dangling_else_trap() {
    int a = 5, b = 10;
    
    // 错误的理解：else 属于外层 if
    if (a > 0)
        if (b > 0)
            printf("Both positive\n");
        else
            printf("This else belongs to inner if!\n");  // 实际上是这样
    
    // 正确的做法：使用大括号明确意图
    if (a > 0) {
        if (b > 0) {
            printf("Both positive\n");
        }
    } else {
        printf("a is not positive\n");
    }
}
```

**switch 中的 fall-through**：

```c
#include <stdio.h>

void fallthrough_trap() {
    int grade = 85;
    char letter_grade;
    
    // 危险的 fall-through
    switch (grade / 10) {
        case 10:
        case 9:
            letter_grade = 'A';
            // break;  // 忘记 break，导致 fall-through
        case 8:
            letter_grade = 'B';
            // break;  // 忘记 break
        case 7:
            letter_grade = 'C';
            break;
        default:
            letter_grade = 'F';
    }
    
    printf("Grade %d -> Letter grade: %c\n", grade, letter_grade);
    
    // 安全的做法：明确使用 fallthrough 属性（C23）
    switch (grade / 10) {
        case 10:
        case 9:
            letter_grade = 'A';
            [[fallthrough]];
        case 8:
            letter_grade = 'B';
            [[fallthrough]];
        case 7:
            letter_grade = 'C';
            break;
        default:
            letter_grade = 'F';
    }
    
    printf("Corrected grade %d -> Letter grade: %c\n", grade, letter_grade);
}
```

**循环变量的作用域问题**：

```c
#include <stdio.h>

void loop_scope_trap() {
    // C89 风格：循环变量在外部声明
    int i;
    for (i = 0; i < 3; i++) {
        printf("C89 style: %d\n", i);
    }
    printf("Loop variable still accessible: %d\n", i);  // 可能不是期望的行为
    
    // C99+ 风格：循环变量在循环内声明
    for (int j = 0; j < 3; j++) {
        printf("C99 style: %d\n", j);
    }
    // printf("j = %d\n", j);  // 编译错误：j 超出作用域
}
```

### 11.2 最佳实践

**使用大括号提高可读性**：

```c
#include <stdio.h>

void brace_best_practices() {
    int x = 5;
    
    // 始终使用大括号，即使只有一条语句
    if (x > 0) {
        printf("Positive\n");
    }
    
    // 嵌套结构使用适当的缩进和空行
    if (x > 0) {
        printf("x is positive\n");
        
        if (x > 10) {
            printf("x is greater than 10\n");
        } else {
            printf("x is between 0 and 10\n");
        }
    }
    
    // 循环也使用大括号
    for (int i = 0; i < 3; i++) {
        printf("Iteration %d\n", i);
    }
}
```

**合理使用 continue 和 break**：

```c
#include <stdio.h>

void loop_control_best_practices() {
    int arr[] = {1, -2, 3, -4, 5, 0, 7};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    // 使用 continue 跳过无效数据
    printf("Processing positive numbers only:\n");
    for (int i = 0; i < size; i++) {
        if (arr[i] <= 0) {
            continue;  // 跳过非正数
        }
        printf("Processing: %d\n", arr[i]);
    }
    
    // 使用 break 提前退出
    printf("\nFinding first negative number:\n");
    for (int i = 0; i < size; i++) {
        if (arr[i] < 0) {
            printf("First negative number found: %d at index %d\n", arr[i], i);
            break;
        }
    }
}
```

**错误处理中的 goto 使用**：

```c
#include <stdio.h>
#include <stdlib.h>

int resource_management_example() {
    FILE *file = NULL;
    int *buffer = NULL;
    int result = -1;
    
    file = fopen("data.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        goto cleanup;
    }
    
    buffer = (int*)malloc(100 * sizeof(int));
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        goto cleanup;
    }
    
    // 正常处理逻辑
    // ...
    result = 0;  // 成功
    
cleanup:
    if (buffer) {
        free(buffer);
    }
    if (file) {
        fclose(file);
    }
    
    return result;
}
```

**函数设计中的早期返回**：

```c
#include <stdio.h>
#include <string.h>

int validate_and_process(const char *input) {
    // 早期返回处理错误情况
    if (input == NULL) {
        fprintf(stderr, "Error: NULL input\n");
        return -1;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return -2;
    }
    
    if (strlen(input) > 100) {
        fprintf(stderr, "Error: Input too long\n");
        return -3;
    }
    
    // 正常处理逻辑
    printf("Processing input: %s\n", input);
    return 0;  // 成功
}

void early_return_example() {
    const char *test_inputs[] = {
        NULL,
        "",
        "This is a very long string that exceeds the maximum allowed length",
        "Valid input"
    };
    
    for (int i = 0; i < 4; i++) {
        int result = validate_and_process(test_inputs[i]);
        printf("Result for input %d: %d\n", i, result);
    }
}
```

## 12. 标准参考

### 12.1 C 标准版本演进

语句相关标准条款的演进：

- C23 标准（ISO/IEC 9899:2024）：6.8 语句和块，引入属性说明符（[[attribute]]），增强标签语句支持
- C17 标准（ISO/IEC 9899:2018）：6.8 语句和块（第106-112页），保持了 C11 的语句特性
- C11 标准（ISO/IEC 9899:2011）：6.8 语句和块（第146-154页），引入 `_Static_assert`
- C99 标准（ISO/IEC 9899:1999）：6.8 语句和块（第131-139页），允许在 for 循环中声明变量
- C89/C90 标准（ISO/IEC 9899:1990）：3.6 声明，建立了基本的语句框架

### 12.2 相关标准条款详解

**6.8 语句和块**：

该条款详细定义了五种语句类型：

- 标记语句（Labeled Statements）：`identifier: statement`
- 复合语句（Compound Statements）：`{ declaration-list statement-list }`
- 表达式和空语句（Expression and Null Statements）：`expression;`
- 选择语句（Selection Statements）：`if` 和 `switch`
- 迭代语句（Iteration Statements）：`while`、`do` 和 `for`
- 跳转语句（Jump Statements）：`goto`、`continue`、`break` 和 `return`

**语义规则**：

标准中关于语句的重要语义规则：

- 标签具有函数作用域
- 复合语句引入新的块作用域
- `break` 语句只能出现在循环或 `switch` 语句中
- `continue` 语句只能出现在循环语句中
- `return` 语句可以在任何函数中使用
- `goto` 语句只能跳转到同一函数内的标签

### 12.3 编译器实现差异

不同编译器在语句处理上的差异：

```c
#include <stdio.h>

void compiler_differences() {
    // 1. 变量声明位置（C89 vs C99+）
    // C89 风格（所有变量在块开始声明）
    {
        int a, b, c;  // 声明在前
        a = 1;
        b = 2;
        c = a + b;
        printf("C89 style: %d\n", c);
    }
    
    // C99+ 风格（变量可以在使用时声明）
    {
        int a = 1;
        int b = 2;
        int c = a + b;  // 声明时初始化
        printf("C99+ style: %d\n", c);
    }
    
    // 2. 混合声明和代码（C99+）
    {
        int x = 10;
        printf("x = %d\n", x);
        int y = 20;  // C99+ 允许，C89 不允许
        printf("y = %d\n", y);
    }
}
```

## 13. 总结

### 13.1 核心要点回顾

C 语言的语句系统是程序执行的基础，包含五个核心类型：

- 复合语句：提供代码组织和作用域管理
- 表达式语句：执行各种操作和计算
- 选择语句：实现条件分支逻辑
- 迭代语句：支持循环执行模式
- 跳转语句：提供灵活的控制流转移

### 13.2 设计原则与最佳实践

**结构化编程原则**：

- 单一入口单一出口：每个函数应该有一个入口和一个出口
- 避免复杂的跳转：尽量减少 `goto` 的使用
- 清晰的控制流：使用适当的缩进和注释

**代码可读性**：

```c
#include <stdio.h>

// 好的代码风格示例
void good_coding_style() {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);
    int sum = 0;
    
    // 清晰的循环结构
    for (int i = 0; i < size; i++) {
        // 明确的条件判断
        if (numbers[i] > 0) {
            sum += numbers[i];
        }
    }
    
    printf("Sum of positive numbers: %d\n", sum);
}
```

**错误处理策略**：

```c
#include <stdio.h>
#include <stdlib.h>

// 统一的错误处理模式
typedef enum {
    SUCCESS = 0,
    ERROR_NULL_POINTER = -1,
    ERROR_INVALID_INPUT = -2,
    ERROR_MEMORY_ALLOCATION = -3
} ErrorCode;

ErrorCode process_data(int *data, int size) {
    // 参数验证
    if (data == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (size <= 0) {
        return ERROR_INVALID_INPUT;
    }
    
    // 内存分配
    int *temp = (int*)malloc(size * sizeof(int));
    if (temp == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // 数据处理
    for (int i = 0; i < size; i++) {
        temp[i] = data[i] * 2;
    }
    
    // 清理资源
    free(temp);
    return SUCCESS;
}
```

### 13.3 未来发展趋势

**C23 新特性的影响**：

C23 标准引入的属性说明符等特性将进一步增强语句的功能：

- 更好的编译时检查：通过属性提供更多的编译时信息
- 增强的代码文档：属性可以作为代码文档的一部分
- 改进的优化支持：编译器可以基于属性进行更好的优化

**现代 C 编程实践**：

现代 C 编程越来越注重：

- 安全性：避免未定义行为和缓冲区溢出
- 可维护性：清晰的代码结构和良好的注释
- 性能：高效的算法和数据结构选择
- 可移植性：遵循标准，避免编译器特定的扩展

**实践项目建议**：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 综合练习项目：简单的文本处理工具
typedef struct {
    char **lines;
    int count;
    int capacity;
} TextBuffer;

TextBuffer* create_text_buffer() {
    TextBuffer *buffer = (TextBuffer*)malloc(sizeof(TextBuffer));
    if (buffer == NULL) {
        return NULL;
    }
    
    buffer->lines = (char**)malloc(10 * sizeof(char*));
    if (buffer->lines == NULL) {
        free(buffer);
        return NULL;
    }
    
    buffer->count = 0;
    buffer->capacity = 10;
    return buffer;
}

void destroy_text_buffer(TextBuffer *buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (int i = 0; i < buffer->count; i++) {
        free(buffer->lines[i]);
    }
    free(buffer->lines);
    free(buffer);
}

int add_line(TextBuffer *buffer, const char *line) {
    if (buffer == NULL || line == NULL) {
        return -1;
    }
    
    // 扩容检查
    if (buffer->count >= buffer->capacity) {
        int new_capacity = buffer->capacity * 2;
        char **new_lines = (char**)realloc(buffer->lines, 
                                         new_capacity * sizeof(char*));
        if (new_lines == NULL) {
            return -1;
        }
        buffer->lines = new_lines;
        buffer->capacity = new_capacity;
    }
    
    // 添加新行
    buffer->lines[buffer->count] = strdup(line);
    if (buffer->lines[buffer->count] == NULL) {
        return -1;
    }
    
    buffer->count++;
    return 0;
}

void print_text_buffer(const TextBuffer *buffer) {
    if (buffer == NULL) {
        return;
    }
    
    for (int i = 0; i < buffer->count; i++) {
        printf("%d: %s\n", i + 1, buffer->lines[i]);
    }
}

int main() {
    TextBuffer *buffer = create_text_buffer();
    if (buffer == NULL) {
        fprintf(stderr, "Failed to create text buffer\n");
        return 1;
    }
    
    // 添加测试数据
    const char *test_lines[] = {
        "First line",
        "Second line",
        "Third line",
        "Fourth line",
        "Fifth line"
    };
    
    for (int i = 0; i < 5; i++) {
        if (add_line(buffer, test_lines[i]) != 0) {
            fprintf(stderr, "Failed to add line %d\n", i);
            destroy_text_buffer(buffer);
            return 1;
        }
    }
    
    printf("Text buffer contents:\n");
    print_text_buffer(buffer);
    
    destroy_text_buffer(buffer);
    return 0;
}
```

通过深入理解和熟练掌握 C 语言的语句系统，开发者可以编写出更加高效、安全和可维护的代码。语句不仅是程序执行的基本单位，更是实现复杂逻辑和算法的基础构建块。
