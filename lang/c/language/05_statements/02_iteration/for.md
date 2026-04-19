# for 循环 (for loop)

## 1. 概述 (Overview)

**for 循环**是一种结构化的循环语句，将初始化、条件检查和迭代更新集中在一行中，是 while 循环的简洁等价形式。

### 核心概念

- **三段式结构**：初始化子句、条件表达式、迭代表达式
- **集中控制**：循环控制要素集中在一行，提高可读性
- **灵活可选**：三个部分均可省略，支持无限循环

### 技术定位

for 循环属于 C 语言的**迭代语句**（iteration statements）类别，是最常用的循环结构，特别适合已知迭代次数或需要索引的场景。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

for 循环源自 ALGOL 语言，设计动机是提供一种简洁的方式来表达计数器驱动的循环，将循环控制逻辑集中在一处。

### 版本变更

| 标准 | 章节 | 主要变更 |
|------|------|----------|
| C89/C90 | 3.6.5.3 | 初始标准化 |
| C99 | 6.8.5.3 | 初始化子句支持声明，引入块作用域 |
| C11 | 6.8.5.3 | 保持不变 |
| C17 | 6.8.5.3 | 保持不变 |
| C23 | 6.8.5.3 | 新增属性说明符序列（`attr-spec-seq`）支持 |

### C99 重要变更

- 初始化子句可以是声明
- 声明的变量作用域为整个循环
- 只允许 `auto` 和 `register` 存储类说明符

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```c
attr-spec-seq(C23 起)(可选) for ( init-clause ; cond-expression ; iteration-expression ) loop-statement
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr-spec-seq` | (C23 起) 可选的属性列表，应用于 for 语句 |
| `init-clause` | 初始化子句：表达式或声明（C99 起） |
| `cond-expression` | 条件表达式：每次迭代前求值，为零则退出 |
| `iteration-expression` | 迭代表达式：每次迭代后求值，结果被丢弃 |
| `loop-statement` | 循环体语句（可以是空语句） |

### 初始化子句（init-clause）

**表达式形式**：
- 求值一次，结果被丢弃
- 在第一次条件检查前执行

**声明形式**（C99 起）：
- 作用域覆盖整个循环（包括条件、迭代表达式和循环体）
- 只允许 `auto` 和 `register` 存储类说明符

### 可选性

| 部分 | 可省略 | 省略时的行为 |
|------|--------|--------------|
| `init-clause` | 是 | 不执行初始化 |
| `cond-expression` | 是 | 替换为非零常量（无限循环） |
| `iteration-expression` | 是 | 不执行迭代更新 |
| `loop-statement` | 否 | 但可以是空语句 `;` |

---

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
┌─────────────────────────────────────┐
│         执行 init-clause             │
└─────────────────┬───────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│    求值 cond-expression              │
└─────────────────┬───────────────────┘
                  │
        ┌─────────┴─────────┐
        │                   │
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│ 非零（执行）   │   │ 零或省略       │
└───────┬───────┘   └───────┬───────┘
        │                   │
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│执行循环体      │   │ 退出循环       │
└───────┬───────┘   └───────────────┘
        │
        ▼
┌─────────────────────────────────────┐
│   执行 iteration-expression          │
└─────────────────┬───────────────────┘
                  │
                  └──────► 返回条件检查
```

### 与 while 循环的等价性

```c
// for 循环
for (init; condition; iteration) {
    body;
}

// 等价的 while 循环
{
    init;
    while (condition) {
        body;
        iteration;
    }
}
```

### continue 的行为

`continue` 语句会跳转到 `iteration-expression` 执行，然后返回条件检查。

### 无限循环

```c
for(;;) {
    printf("endless loop!");
}
```

省略 `cond-expression` 时，替换为非零常量，形成无限循环。`for(;;)` 总是无限循环。

### 无限循环的未定义行为

无限循环如果没有任何可观察行为（I/O、volatile 访问、原子或同步操作），则具有未定义行为。例外：`for(;;)` 总是无限循环。

### 块作用域（C99 起）

for 语句建立独立的块作用域：`init-clause`、`cond-expression`、`iteration-expression` 中引入的标识符在 `loop-statement` 结束后超出作用域。

### C 与 C++ 的区别

```c
for (int i = 0; ; ) {
    long i = 1;   // C 中合法，C++ 中非法
    // ...
}
```

在 C 中，循环体是一个块，可以重新声明 `i`。在 C++ 中，整个循环是一个作用域，不允许重声明。

---

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 计数循环 | 已知迭代次数 |
| 数组遍历 | 使用索引访问数组元素 |
| 初始化序列 | 初始化数组或数据结构 |
| 查找操作 | 带索引的查找 |

### 最佳实践

1. **使用 C99 声明语法**：在初始化子句中声明变量
2. **使用 size_t 作为索引**：避免负数问题
3. **避免复杂表达式**：保持三个部分简洁
4. **注意迭代顺序**：迭代表达式在循环体后执行

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 分号后多余分号 | `for(...);` 循环体是空语句 | 检查循环体位置 |
| 修改索引变量 | 在循环体内修改索引 | 避免或小心使用 |
| 浮点计数器 | 浮点精度问题 | 使用整数计数器 |
| 无限循环 | 条件永远为真 | 确保终止条件可达 |

---

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>
#include <stdlib.h>

enum { SIZE = 8 };

int main(void)
{
    int array[SIZE];

    // 填充数组
    for(size_t i = 0 ; i < SIZE; ++i)
        array[i] = rand() % 2;

    printf("Array filled!\n");

    // 打印数组
    for (size_t i = 0; i < SIZE; ++i)
        printf("%d ", array[i]);

    putchar('\n');

    return 0;
}
```

**输出：**
```
Array filled!
1 0 1 1 1 1 0 0
```

### 空语句循环体

```c
#include <stdio.h>

int main(void)
{
    // 循环体是空语句，所有工作在迭代表达式中完成
    for(int n = 0; n < 10; ++n, printf("%d\n", n))
        ; // 空语句

    return 0;
}
```

### 使用 break 和 continue

```c
#include <stdio.h>

int main(void)
{
    // 使用 break 提前退出
    for (int i = 0; i < 10; i++)
    {
        if (i == 5)
            break;
        printf("%d ", i);
    }
    printf("\n");

    // 使用 continue 跳过特定值
    for (int i = 0; i < 10; i++)
    {
        if (i % 2 == 0)
            continue;
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}
```

**输出：**
```
0 1 2 3 4
1 3 5 7 9
```

### 多变量初始化

```c
#include <stdio.h>

int main(void)
{
    // 使用逗号运算符处理多个变量
    for (int i = 0, j = 10; i < j; i++, j--)
    {
        printf("i=%d, j=%d\n", i, j);
    }

    return 0;
}
```

### 无限循环

```c
#include <stdio.h>

int main(void)
{
    int count = 0;

    for (;;)  // 无限循环
    {
        printf("Iteration %d\n", ++count);
        if (count >= 5)
            break;
    }

    // 等价形式
    count = 0;
    for (; 1; )  // 条件为非零常量
    {
        printf("Iteration %d\n", ++count);
        if (count >= 5)
            break;
    }

    return 0;
}
```

### goto 进入循环

```c
#include <stdio.h>

int main(void)
{
    // 通过 goto 进入循环体
    // init-clause 和 cond-expression 不会执行
    int i = 5;

    goto inside_loop;

    for (int j = 0; j < 10; j++)
    {
    inside_loop:
        printf("i = %d\n", i);
        if (i > 7) break;
        i++;
    }

    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 三段结构 | 初始化、条件、迭代 |
| 执行顺序 | 初始化 → 条件 → 循环体 → 迭代 → 条件 |
| continue 行为 | 跳转到迭代表达式 |
| break 行为 | 直接退出循环 |

### 三种循环对比

| 特性 | while | do-while | for |
|------|-------|----------|-----|
| 条件检查 | 前 | 后 | 前 |
| 最少执行 | 0 次 | 1 次 | 0 次 |
| 初始化 | 手动 | 手动 | 内置 |
| 迭代更新 | 手动 | 手动 | 内置 |
| 典型场景 | 条件驱动 | 至少执行一次 | 计数驱动 |

### for 循环各部分行为

| 部分 | 执行时机 | 可省略 |
|------|----------|--------|
| init-clause | 循环开始前一次 | 是 |
| cond-expression | 每次迭代前 | 是（替换为非零） |
| iteration-expression | 每次迭代后 | 是 |
| loop-statement | 条件非零时 | 否（可为空语句） |

### 学习建议

1. **优先使用 for**：适合已知迭代次数的场景
2. **C99 语法**：在初始化子句中声明变量
3. **理解执行顺序**：初始化→条件→循环体→迭代→条件
4. **注意作用域**：变量作用域限于循环

---

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.8.5.3 The for statement
- C17 标准 (ISO/IEC 9899:2018): 6.8.5.3 The for statement (p: 110)
- C11 标准 (ISO/IEC 9899:2011): 6.8.5.3 The for statement (p: 151)
- C99 标准 (ISO/IEC 9899:1999): 6.8.5.3 The for statement (p: 136)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.6.5.3 The for statement