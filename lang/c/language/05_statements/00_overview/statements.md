# C 语句 (Statements)

## 1. 概述 (Overview)

语句（Statements）是 C 程序按顺序执行的代码片段。任何函数体都是一个复合语句，复合语句又是由一系列语句和声明组成的。

### 五种语句类型

| 类型 | 中文名称 | 说明 |
|------|----------|------|
| Compound statements | 复合语句 | 用花括号包围的语句和声明序列 |
| Expression statements | 表达式语句 | 后跟分号的表达式 |
| Selection statements | 选择语句 | if、switch 等条件分支语句 |
| Iteration statements | 迭代语句 | while、do-while、for 循环语句 |
| Jump statements | 跳转语句 | break、continue、return、goto |

### C23 新特性

C23 标准允许将属性说明符序列（attr-spec-seq）应用于无标签语句，属性将应用于相应的语句。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C 语言的语句设计继承了 B 语言和 BCPL 的风格，强调简洁性和直接的流程控制。语句是 C 程序执行流的基本单位。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C89/C90 | 标准化五种基本语句类型 |
| C99 | 允许在代码块任意位置声明变量；引入变长数组（VLA） |
| C11 | 无重大变更 |
| C17 | 无重大变更 |
| C23 | 引入属性说明符序列；标签后可不跟语句 |

### 设计动机

语句机制解决了以下核心问题：

1. **顺序执行**：明确程序执行流程
2. **作用域控制**：通过复合语句引入块作用域
3. **流程控制**：通过选择、迭代、跳转语句控制执行路径
4. **代码组织**：将复杂的程序逻辑分解为可管理的单元

## 3. 语法与参数 (Syntax and Parameters)

### 标签语法

| 语法 | 说明 |
|------|------|
| `attr-spec-seq(可选) identifier :` | (1) goto 目标标签 |
| `attr-spec-seq(可选) case 常量表达式 :` | (2) switch case 标签 |
| `attr-spec-seq(可选) default :` | (3) switch default 标签 |

### 复合语句语法

```c
// C23 前
{ 语句或声明... }

// C23 起
attr-spec-seq(可选) { 无标签语句 | 标签 | 声明... }
```

### 表达式语句语法

```c
表达式(可选) ;           // 基本形式
attr-spec-seq 表达式 ;   // C23 起
```

### 选择语句语法

```c
// if 语句
attr-spec-seq(可选) if (表达式) 语句
attr-spec-seq(可选) if (表达式) 语句 else 语句

// switch 语句
attr-spec-seq(可选) switch (表达式) 语句
```

### 迭代语句语法

```c
// while 循环
attr-spec-seq(可选) while (表达式) 语句

// do-while 循环
attr-spec-seq(可选) do 语句 while (表达式) ;

// for 循环
attr-spec-seq(可选) for (初始化子句; 表达式(可选); 表达式(可选)) 语句
```

### 跳转语句语法

```c
attr-spec-seq(可选) break ;
attr-spec-seq(可选) continue ;
attr-spec-seq(可选) return 表达式(可选) ;
attr-spec-seq(可选) goto 标识符 ;
```

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
程序开始
    ↓
进入 main 函数的复合语句
    ↓
顺序执行语句和声明
    ↓
┌─────────────────────────────────┐
│ 复合语句   │ 选择语句  │ 迭代语句 │ 跳转语句 │
└─────────────────────────────────┘
    ↓              ↓           ↓         ↓
顺序执行    条件分支执行   循环执行   无条件跳转
    ↓              ↓           ↓         ↓
┌─────────────────────────────────┐
│            顺序执行              │
└─────────────────────────────────┘
    ↓
函数返回
```

### 块作用域

每个复合语句引入自己的块作用域：

```c
{
    int x = 10;        // 外层块作用域
    {
        int x = 20;    // 内层块作用域，隐藏外层 x
        // x = 20
    }
    // x = 10
}
```

### 声明执行

块内自动存储期变量的初始化器和 VLA 声明符在控制流经过时执行：

```c
{
    puts("hello");           // 表达式语句
    int n = printf("abc\n"); // 声明，打印 "abc"，存储 4 到 n
    int a[n*printf("1\n")];  // 声明，打印 "1"，分配 8*sizeof(int)
    printf("%zu\n", sizeof(a)); // 表达式语句
}
```

### 标签行为

#### C23 前

- 标签后必须跟语句
- 标签本身不影响控制流

#### C23 起

- 标签可以单独出现在块中
- 单独标签的行为如同后跟空语句
- 属性可应用于标签

## 5. 使用场景 (Use Cases)

### 复合语句

#### 1. 函数体

```c
int main(void)
{ // 复合语句开始
    int n = 1;           // 声明
    n = n + 1;           // 表达式语句
    printf("n = %d\n", n); // 表达式语句
    return 0;            // return 语句
} // 复合语句结束
```

#### 2. 条件和循环体

```c
if (expr)
{ // 块开始
    int n = 1;           // 声明
    printf("%d\n", n);   // 表达式语句
} // 块结束
```

### 表达式语句

#### 1. 赋值语句

```c
x = 10;
array[i] = value;
```

#### 2. 函数调用

```c
printf("Hello, World!\n");
process_data(data, size);
```

#### 3. 空语句

```c
while (*s++ != '\0')
    ; // 空语句
```

### 选择语句

#### 1. if 语句

```c
if (x > 0) {
    printf("Positive\n");
} else if (x < 0) {
    printf("Negative\n");
} else {
    printf("Zero\n");
}
```

#### 2. switch 语句

```c
switch (ch) {
    case 'a':
    case 'A':
        printf("A\n");
        break;
    case 'b':
    case 'B':
        printf("B\n");
        break;
    default:
        printf("Other\n");
}
```

### 迭代语句

#### 1. while 循环

```c
int i = 0;
while (i < 10) {
    printf("%d\n", i);
    i++;
}
```

#### 2. do-while 循环

```c
int choice;
do {
    printf("Enter choice (1-5): ");
    scanf("%d", &choice);
} while (choice < 1 || choice > 5);
```

#### 3. for 循环

```c
for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
}
```

### 跳转语句

#### 1. break

```c
for (int i = 0; i < 100; i++) {
    if (found) {
        break; // 退出循环
    }
}
```

#### 2. continue

```c
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
        continue; // 跳过偶数
    }
    printf("%d\n", i);
}
```

#### 3. return

```c
int add(int a, int b) {
    return a + b;
}
```

#### 4. goto

```c
void cleanup_example(void) {
    char *buffer = NULL;
    FILE *file = NULL;

    buffer = malloc(1024);
    if (!buffer) goto cleanup;

    file = fopen("data.txt", "r");
    if (!file) goto cleanup;

    // 处理代码

cleanup:
    if (file) fclose(file);
    if (buffer) free(buffer);
}
```

### 最佳实践

1. **使用花括号**：即使单条语句也使用花括号
2. **避免 goto**：尽量使用结构化控制流
3. **合理使用空语句**：确保空语句有明确用途
4. **标签命名清晰**：使用有意义的标签名
5. **限制块嵌套深度**：避免过深的嵌套

### 常见陷阱

#### 陷阱 1：悬空 else

```c
// 危险：else 关联到最近的 if
if (a > 0)
    if (b > 0)
        printf("Both positive\n");
else
    printf("a <= 0\n"); // 实际上关联到内层 if

// 修正：使用花括号
if (a > 0) {
    if (b > 0)
        printf("Both positive\n");
} else {
    printf("a <= 0\n");
}
```

#### 陷阱 2：switch 缺少 break

```c
// 危险：fall-through 行为
switch (x) {
    case 1:
        printf("One\n");
        // 缺少 break，继续执行 case 2
    case 2:
        printf("Two\n");
        break;
}

// 修正：添加 break 或注释说明 fall-through
switch (x) {
    case 1:
        printf("One\n");
        break;
    case 2:
        printf("Two\n");
        break;
}
```

#### 陷阱 3：分号错误

```c
// 错误：if 后多余的分号
if (x > 0);
{
    printf("Positive\n"); // 总是执行
}

// 修正：删除多余分号
if (x > 0)
{
    printf("Positive\n");
}
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：函数体结构

```c
#include <stdio.h>

int main(void)
{ // 复合语句开始
    int n = 1;               // 声明（不是语句）
    n = n + 1;               // 表达式语句
    printf("n = %d\n", n);   // 表达式语句
    return 0;                // return 语句
} // 复合语句结束，函数体结束
```

#### 示例 2：空语句用法

```c
#include <stdio.h>

int main(void)
{
    char str[] = "hello";
    char *s = str;

    // 空语句用于循环体
    while (*s++ != '\0')
        ; // 找到字符串末尾

    printf("String length: %td\n", s - str - 1);
    return 0;
}
```

### 高级用法

#### 示例 3：变长数组与声明执行

```c
#include <stdio.h>

int main(void)
{ // 块开始
    { // 内层块开始
        puts("hello");                     // 表达式语句
        int n = printf("abc\n");           // 声明，打印 "abc"，存储 4 到 n
        int a[n * printf("1\n")];          // 声明，打印 "1"，分配 8*sizeof(int)
        printf("%zu\n", sizeof(a));        // 表达式语句
    } // 内层块结束，n 和 a 的作用域结束

    int n = 7; // n 可以重用
    printf("n = %d\n", n);
    return 0;
}
```

#### 示例 4：goto 进行错误处理

```c
#include <stdio.h>
#include <stdlib.h>

int process_file(const char *filename)
{
    FILE *file = NULL;
    char *buffer = NULL;
    int result = -1;

    file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        goto cleanup;
    }

    buffer = malloc(1024);
    if (!buffer) {
        perror("Failed to allocate buffer");
        goto cleanup;
    }

    // 处理文件
    while (fgets(buffer, 1024, file)) {
        printf("%s", buffer);
    }

    result = 0; // 成功

cleanup:
    if (buffer) free(buffer);
    if (file) fclose(file);
    return result;
}

int main(void)
{
    return process_file("data.txt");
}
```

#### 示例 5：嵌套控制结构

```c
#include <stdio.h>

int main(void)
{
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int target = 5;
    int found = 0;

    for (int i = 0; i < 3 && !found; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix[i][j] == target) {
                printf("Found %d at [%d][%d]\n", target, i, j);
                found = 1;
                break; // 退出内层循环
            }
        }
        // break 使控制流到达这里
    }

    if (!found) {
        printf("%d not found\n", target);
    }

    return 0;
}
```

#### 示例 6：C23 属性语法

```c
#include <stdio.h>

// C23: 属性应用于语句
int main(void)
{
    [[maybe_unused]] int unused_var = 42;

    [[likely]] if (1) {
        printf("Likely branch\n");
    }

    for (int i = 0; i < 10; i++) {
        [[unlikely]] if (i == 100) {
            printf("Unlikely\n");
        }
    }

    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：悬空 else

```c
// 错误：else 关联错误的 if
int a = 1, b = 0;
if (a > 0)
    if (b > 0)
        printf("Both\n");
else
    printf("a <= 0\n"); // 实际关联到内层 if

// 修正：使用花括号明确作用域
if (a > 0) {
    if (b > 0)
        printf("Both\n");
} else {
    printf("a <= 0\n");
}
```

#### 错误示例 2：switch fall-through

```c
// 错误：意外的 fall-through
int x = 1;
switch (x) {
    case 1:
        printf("One\n");
    case 2:
        printf("Two\n");
}
// 输出: One\nTwo\n

// 修正：添加 break
switch (x) {
    case 1:
        printf("One\n");
        break;
    case 2:
        printf("Two\n");
        break;
}
```

#### 错误示例 3：赋值与比较混淆

```c
// 错误：赋值而非比较
if (x = 5) {  // x 被赋值为 5，条件始终为真
    printf("x is 5\n");
}

// 修正：使用比较运算符
if (x == 5) {
    printf("x is 5\n");
}

// 或者：将常量放在左边（编译器会警告赋值）
if (5 == x) {
    printf("x is 5\n");
}
```

## 7. 总结 (Summary)

### 核心要点

1. **五种类型**：复合语句、表达式语句、选择语句、迭代语句、跳转语句
2. **顺序执行**：语句按顺序执行，除非遇到跳转语句
3. **块作用域**：复合语句引入新的作用域
4. **标签功能**：任何语句可以被标签标记
5. **C23 属性**：属性可应用于语句和标签

### 语句类型对比

| 类型 | 关键字/语法 | 主要用途 |
|------|------------|----------|
| 复合语句 | `{ ... }` | 定义作用域、组织代码 |
| 表达式语句 | `expr;` | 执行操作、调用函数 |
| 选择语句 | `if`、`switch` | 条件分支 |
| 迭代语句 | `while`、`do`、`for` | 循环执行 |
| 跳转语句 | `break`、`continue`、`return`、`goto` | 控制流跳转 |

### C23 变更摘要

| 特性 | C23 前 | C23 起 |
|------|--------|--------|
| 属性应用于语句 | 不支持 | 支持 |
| 标签后必须有语句 | 是 | 否（可单独出现） |
| 属性应用于标签 | 不支持 | 支持 |

### 学习建议

1. **理解执行顺序**：掌握语句的顺序执行模型
2. **善用花括号**：提高代码可读性和安全性
3. **避免 goto**：优先使用结构化控制流
4. **注意 break**：switch 语句中记得使用 break
5. **学习 C23 特性**：了解属性的新用法

---

**标准参考**：
- C23 (ISO/IEC 9899:2024): 6.8 Statements and blocks
- C17 (ISO/IEC 9899:2018): 6.8 Statements and blocks (p: 106-112)
- C11 (ISO/IEC 9899:2011): 6.8 Statements and blocks (p: 146-154)
- C99 (ISO/IEC 9899:1999): 6.8 Statements and blocks (p: 131-139)
- C89/C90 (ISO/IEC 9899:1990): 3.6 STATEMENTS