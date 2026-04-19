# if 语句 (if statement)

## 1. 概述 (Overview)

**if 语句**是 C 语言中最基本的条件控制语句，用于根据条件的真假决定是否执行特定的代码块。

### 核心概念

- **条件执行**：仅当指定条件为真时才执行相应代码
- **分支选择**：支持单分支（仅 if）和双分支（if-else）两种形式
- **标量类型判断**：条件表达式可以是任意标量类型（整数、浮点、指针等）

### 技术定位

if 语句属于 C 语言的**选择语句**（selection statements）类别，是实现程序分支逻辑的基础构建块。它与 switch 语句共同构成了 C 语言的条件控制结构。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

if 语句源自 ALGOL 语言家族，是结构化编程的核心概念之一。其设计动机是为了替代早期汇编语言中的条件跳转指令，提供更清晰、更易读的条件控制结构。

### 版本变更

| 标准 | 章节 | 主要变更 |
|------|------|----------|
| C89/C90 | 3.6.4.1 | 初始标准化 |
| C99 | 6.8.4.1 | 引入块级作用域规则 |
| C11 | 6.8.4.1 | 保持不变 |
| C17 | 6.8.4.1 | 保持不变 |
| C23 | 6.8.5.2 | 新增 `attr-spec-seq`（属性说明符序列）支持 |

### C23 新特性

从 C23 标准开始，if 语句支持可选的属性说明符序列（`attr-spec-seq`），允许在 if 语句上应用编译器属性。

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```
attr-spec-seq(可选) if ( expression ) statement-true                    (1)
attr-spec-seq(可选) if ( expression ) statement-true else statement-false  (2)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr-spec-seq` | (C23 起) 可选的属性列表，应用于 if 语句 |
| `expression` | 任意标量类型的表达式 |
| `statement-true` | 当表达式不等于 0 时执行的语句（通常是复合语句） |
| `statement-false` | 当表达式等于 0 时执行的语句（通常是复合语句） |

### 表达式求值规则

- **标量类型**：表达式可以是整数、浮点数、指针等任意标量类型
- **真值判断**：表达式值不等于整数零时为真
- **假值判断**：表达式值等于整数零时为假

---

## 4. 底层原理 (Underlying Principles)

### 实现机制

if 语句在编译后通常转换为条件跳转指令：

1. **求值表达式**：计算条件表达式的值
2. **零值比较**：将结果与零进行比较
3. **条件跳转**：根据比较结果跳转到相应的代码块

### 作用域规则（C99 起）

整个 if 语句具有独立的块作用域：

```c
enum {a, b};

int different(void)
{
    if (sizeof(enum {b, a}) != sizeof(int))
        return a; // a == 1
    return b; // b == 0 in C89, b == 1 in C99
}
```

在上述示例中，if 语句内部的 `enum {b, a}` 定义了一个新的枚举类型，其作用域仅限于该 if 语句内部。

### else 匹配规则

**else 总是与最近的未匹配的 if 相关联**（即"就近匹配"原则）：

```c
int j = 1;
if (i > 1)
    if(j > 2)
        printf("%d > 1 and %d > 2\n", i, j);
    else // 此 else 属于 if (j > 2)，而非 if (i > 1)
        printf("%d > 1 and %d <= 2\n", i, j);
```

### goto 行为

如果通过 `goto` 跳转到 `statement-true`，则 `statement-false` 不会被执行。

---

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 描述 |
|------|------|
| 条件验证 | 检查输入参数、边界条件、返回值等 |
| 错误处理 | 根据错误码执行不同的处理逻辑 |
| 状态判断 | 根据程序状态执行相应操作 |
| 功能开关 | 控制特定功能的启用/禁用 |

### 最佳实践

1. **使用花括号**：始终使用花括号 `{}` 包裹语句体，即使只有一条语句
2. **避免深层嵌套**：超过 3 层嵌套时应考虑重构
3. **优先处理错误/边界情况**：使用"提前返回"模式减少嵌套

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 悬空 else | else 与错误的 if 匹配 | 使用花括号明确作用域 |
| 赋值 vs 比较 | `if (x = 5)` 写成赋值 | 将常量放左边 `if (5 == x)` |
| 浮点比较 | 直接比较浮点数 | 使用容差值比较 |
| 指针作为条件 | NULL 指针判断 | 显式写 `if (ptr != NULL)` |

---

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

int main(void)
{
    int i = 2;

    // 完整的 if-else 结构
    if (i > 2)
    {
        printf("i > 2 is true\n");
    }
    else
    {
        printf("i > 2 is false\n");
    }

    // 单独的 if 语句
    i = 3;
    if (i == 3)
        printf("i == 3\n");

    // 带有 else 的条件判断
    if (i != 3)
        printf("i != 3 is true\n");
    else
        printf("i != 3 is false\n");

    return 0;
}
```

**输出结果：**
```
i > 2 is false
i == 3
i != 3 is false
```

### 最佳实践示例

```c
#include <stdio.h>
#include <stdlib.h>

// 推荐：使用花括号和提前返回
int process_data(int *data, int size)
{
    // 参数验证
    if (data == NULL)
    {
        fprintf(stderr, "Error: NULL pointer\n");
        return -1;
    }

    if (size <= 0)
    {
        fprintf(stderr, "Error: invalid size %d\n", size);
        return -1;
    }

    // 处理逻辑
    for (int i = 0; i < size; i++)
    {
        if (data[i] < 0)
        {
            printf("Warning: negative value at index %d\n", i);
        }
    }

    return 0;
}

// 推荐：使用常量在左边的比较
void check_value(int x)
{
    if (5 == x)  // 如果误写为 if (5 = x) 会编译报错
    {
        printf("x is 5\n");
    }
}
```

### 常见错误及修正

```c
#include <stdio.h>

int main(void)
{
    int x = 5;

    // 错误 1：误用赋值运算符
    // if (x = 10)  // 危险！总是为真，且改变了 x 的值

    // 正确写法
    if (x == 10)
    {
        printf("x is 10\n");
    }

    // 错误 2：悬空 else
    int a = 1, b = 2;
    // 下面的 else 与哪个 if 配对？
    if (a > 0)
        if (b > 10)
            printf("both conditions true\n");
    // else  // 意图是匹配 if (a > 0)，但实际匹配 if (b > 10)
    //     printf("a <= 0\n");

    // 正确写法：使用花括号
    if (a > 0)
    {
        if (b > 10)
        {
            printf("both conditions true\n");
        }
    }
    else
    {
        printf("a <= 0\n");
    }

    return 0;
}
```

### 高级用法：多条件判断

```c
#include <stdio.h>

const char* get_grade(int score)
{
    if (score >= 90)
        return "A";
    else if (score >= 80)
        return "B";
    else if (score >= 70)
        return "C";
    else if (score >= 60)
        return "D";
    else
        return "F";
}

int main(void)
{
    int scores[] = {95, 82, 71, 65, 45};

    for (int i = 0; i < 5; i++)
    {
        printf("Score %d: Grade %s\n", scores[i], get_grade(scores[i]));
    }

    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 基本形式 | 单分支（if）和双分支（if-else） |
| 条件类型 | 任意标量类型（整数、浮点、指针） |
| 真值判断 | 非零为真，零为假 |
| 作用域 | C99 起整个 if 语句具有独立块作用域 |
| else 匹配 | 就近匹配原则 |

### 技术对比

| 特性 | if 语句 | switch 语句 |
|------|---------|-------------|
| 条件类型 | 任意标量表达式 | 整数类型（含枚举） |
| 分支数量 | 有限分支时简洁 | 多分支时更清晰 |
| 值匹配 | 范围判断灵活 | 精确值匹配 |
| 性能 | 一般 | 编译器可能优化为跳转表 |

### 学习建议

1. **掌握基础**：理解条件表达式的求值规则和真值判断
2. **避免陷阱**：特别注意赋值与比较运算符的区别
3. **规范编码**：始终使用花括号，遵循代码风格指南
4. **合理嵌套**：控制嵌套层次，必要时使用提前返回

---

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.8.5.2 The if statement (p: 154)
- C17 标准 (ISO/IEC 9899:2018): 6.8.4.1 The if statement (p: 108-109)
- C11 标准 (ISO/IEC 9899:2011): 6.8.4.1 The if statement (p: 148-149)
- C99 标准 (ISO/IEC 9899:1999): 6.8.4.1 The if statement (p: 133-134)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.6.4.1 The if statement