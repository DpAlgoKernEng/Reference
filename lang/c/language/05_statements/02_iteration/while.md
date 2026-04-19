# while 循环 (while loop)

## 1. 概述 (Overview)

**while 循环**是一种先判断后执行的循环结构，在每次迭代前检查条件，条件为真时执行循环体。

### 核心概念

- **前置条件检查**：条件在每次迭代前求值
- **条件驱动**：循环由条件表达式控制
- **可能零次执行**：条件初始为假时，循环体不执行

### 技术定位

while 循环属于 C 语言的**迭代语句**（iteration statements）类别，是最基本的循环结构，适合条件驱动的循环场景。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

while 循环源自结构化编程理念，提供一种语义清晰的迭代机制，避免了使用 goto 和标签实现循环的复杂性。

### 版本变更

| 标准 | 章节 | 主要变更 |
|------|------|----------|
| C89/C90 | 3.6.5.1 | 初始标准化 |
| C99 | 6.8.5.1 | 引入块作用域规则 |
| C11 | 6.8.5.1 | 保持不变 |
| C17 | 6.8.5.1 | 保持不变 |
| C23 | 6.8.5.1 | 新增属性说明符序列（`attr-spec-seq`）支持 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```c
attr-spec-seq(可选) while ( expression ) statement
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr-spec-seq` | (C23 起) 可选的属性列表，应用于循环语句 |
| `expression` | 任意标量类型表达式，每次迭代前求值，等于零时退出 |
| `statement` | 任意语句（通常是复合语句），作为循环体 |

### 表达式求值规则

- **类型**：任意标量类型（整数、浮点、指针等）
- **求值时机**：每次循环体执行前
- **退出条件**：表达式等于零时退出循环

---

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
┌─────────────────────────────────────┐
│         进入 while 循环              │
└─────────────────┬───────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│         求值 expression              │
└─────────────────┬───────────────────┘
                  │
        ┌─────────┴─────────┐
        │                   │
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│ 非零（执行）   │   │ 零（退出）     │
└───────┬───────┘   └───────────────┘
        │
        ▼
┌─────────────────────────────────────┐
│         执行 statement（循环体）      │
└─────────────────┬───────────────────┘
                  │
                  └──────► 返回条件检查
```

### goto 进入循环体的行为

如果通过 goto 进入循环体中间：
- 循环体正常执行
- 条件在下次循环开始前求值
- 循环可能正常继续

### 无限循环行为

无限循环如果没有任何可观察行为（I/O、volatile 访问、原子或同步操作），则具有未定义行为。例外：条件为常量表达式的循环 `while(true)` 总是无限循环。

### 块作用域（C99 起）

while 语句建立独立的块作用域：表达式中引入的标识符在语句结束后超出作用域。

### 布尔和指针表达式

布尔值 `false` 和任何指针类型的空指针值都与零比较相等：

```c
while (ptr)        // ptr 非空时继续
while (*ptr)       // *ptr 非零时继续
while (condition)  // condition 为真时继续
```

---

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 条件驱动循环 | 循环次数不确定，由条件决定 |
| 读取数据 | 读取直到特定条件 |
| 查找操作 | 找到目标时退出 |
| 等待条件 | 等待某个条件成立 |

### 最佳实践

1. **确保终止条件**：条件最终能变为假
2. **更新条件变量**：循环体内更新影响条件的变量
3. **使用复合语句**：循环体使用花括号
4. **避免无限循环**：除非有意为之

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 无限循环 | 条件永远为真 | 确保条件能变为假 |
| 漏掉分号 | 循环体后错误分号 | 检查循环体位置 |
| 更新遗漏 | 条件变量未更新 | 确保循环体内更新 |
| 赋值 vs 比较 | `=` 写成 `==` | 使用 `==` 或"尤达条件式" |

---

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { SIZE = 8 };

int main(void)
{
    // 简单示例
    int array[SIZE], n = 0;
    while(n < SIZE) array[n++] = rand() % 2;
    puts("Array filled!");

    n = 0;
    while(n < SIZE) printf("%d ", array[n++]);
    printf("\n");

    return 0;
}
```

**输出：**
```
Array filled!
1 0 1 1 1 1 0 0
```

### 经典 strcpy 实现

```c
#include <stdio.h>
#include <string.h>

int main(void)
{
    // 经典 strcpy() 实现
    // 从 src 复制空终止字符串到 dst
    char src[] = "Hello, world", dst[sizeof src], *p = dst, *q = src;

    while((*p++ = *q++))  // 双括号（非必须）用于抑制警告
                          // 确保这是赋值（而非比较）
                          // 其结果用作真值
        ; // 空语句

    puts(dst);
    return 0;
}
```

**输出：**
```
Hello, world
```

### 读取输入直到条件

```c
#include <stdio.h>

int main(void)
{
    int sum = 0;
    int num;

    printf("Enter numbers (0 to stop):\n");

    while (scanf("%d", &num) == 1 && num != 0)
    {
        sum += num;
    }

    printf("Sum: %d\n", sum);

    return 0;
}
```

### 使用 break 和 continue

```c
#include <stdio.h>

int main(void)
{
    int i = 0;

    // 使用 continue 跳过特定值
    printf("Using continue: ");
    while (i < 10)
    {
        i++;
        if (i == 5)
            continue;  // 跳过 5
        printf("%d ", i);
    }
    printf("\n");

    // 使用 break 提前退出
    i = 0;
    printf("Using break: ");
    while (i < 10)
    {
        i++;
        if (i == 5)
            break;  // 在 5 处退出
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}
```

**输出：**
```
Using continue: 1 2 3 4 6 7 8 9 10
Using break: 1 2 3 4
```

### 指针遍历

```c
#include <stdio.h>

int main(void)
{
    char str[] = "Hello";
    char *p = str;

    // 使用指针遍历字符串
    while (*p)
    {
        printf("%c ", *p);
        p++;
    }
    printf("\n");

    // 等价的数组索引方式
    int i = 0;
    while (str[i])
    {
        printf("%c ", str[i]);
        i++;
    }
    printf("\n");

    return 0;
}
```

**输出：**
```
H e l l o
H e l l o
```

### 常见错误

```c
#include <stdio.h>

int main(void)
{
    // 错误 1：无限循环（条件永远为真）
    // int x = 5;
    // while (x > 0) {
    //     printf("%d\n", x);
    //     // 忘记更新 x
    // }

    // 正确写法
    int x = 5;
    while (x > 0) {
        printf("%d ", x);
        x--;
    }
    printf("\n");

    // 错误 2：赋值 vs 比较
    int y = 5;
    // while (y = 1) {  // 危险！总是为真
    //     printf("y = %d\n", y);
    //     y++;
    // }

    // 正确写法
    while (y == 1) {  // 比较
        printf("y = %d\n", y);
    }

    // 错误 3：多余的分号
    int z = 3;
    while (z > 0)
    {
        printf("%d ", z);
        z--;
    };  // 多余分号，无害但不需要

    printf("\n");
    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 条件检查 | 每次迭代前 |
| 最少执行 | 可能 0 次 |
| 条件类型 | 任意标量类型 |
| 无限循环 | `while(1)` 或 `while(true)` |

### 三种循环对比

| 特性 | while | do-while | for |
|------|-------|----------|-----|
| 条件检查 | 前 | 后 | 前 |
| 最少执行 | 0 次 | 1 次 | 0 次 |
| 初始化 | 手动 | 手动 | 内置 |
| 迭代更新 | 手动 | 手动 | 内置 |
| 典型场景 | 条件驱动 | 至少执行一次 | 计数驱动 |

### while 循环特点

| 特点 | 说明 |
|------|------|
| 简洁性 | 最简单的循环结构 |
| 灵活性 | 条件可以是任意表达式 |
| 通用性 | 可实现任何循环逻辑 |
| 可读性 | 条件清晰，易于理解 |

### 学习建议

1. **理解前置检查**：条件在循环体前检查
2. **确保终止**：条件最终能变为假
3. **手动更新**：循环体内更新条件变量
4. **选择合适循环**：条件驱动用 while，计数用 for

---

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.8.5.1 The while statement
- C17 标准 (ISO/IEC 9899:2018): 6.8.5.1 The while statement (p: 109)
- C11 标准 (ISO/IEC 9899:2011): 6.8.5.1 The while statement (p: 151)
- C99 标准 (ISO/IEC 9899:1999): 6.8.5.1 The while statement (p: 136)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.6.5.1 The while statement