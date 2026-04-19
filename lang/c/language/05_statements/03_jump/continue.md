# continue 语句 (continue statement)

## 1. 概述 (Overview)

**continue 语句**用于跳过当前迭代的剩余部分，直接进入下一次循环迭代。

### 核心概念

- **跳过剩余代码**：跳过循环体中 continue 之后的所有语句
- **继续迭代**：不退出循环，直接进入下一次迭代
- **三种循环通用**：适用于 for、while、do-while 循环

### 技术定位

continue 语句属于 C 语言的**跳转语句**（jump statements）类别，用于简化循环控制逻辑，避免深层嵌套的条件判断。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

continue 语句的设计动机是提供一种简洁的方式来跳过循环中的特定迭代，而无需使用复杂的条件嵌套或 goto 语句。

### 版本变更

| 标准 | 章节 | 主要变更 |
|------|------|----------|
| C89/C90 | 3.6.6.2 | 初始标准化 |
| C99 | 6.8.6.2 | 保持不变 |
| C11 | 6.8.6.2 | 保持不变 |
| C17 | 6.8.6.2 | 保持不变 |
| C23 | 6.8.6.2 | 新增属性说明符序列（`attr-spec-seq`）支持 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```c
attr-spec-seq(可选) continue ;
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr-spec-seq` | (C23 起) 可选的属性列表，应用于 continue 语句 |

### 使用限制

continue 语句只能出现在 for、while、do-while 循环体内。

---

## 4. 底层原理 (Underlying Principles)

### 在不同循环中的行为

**while 循环**：
```c
while (/* ... */) {
   // ...
   continue; // 等价于 goto contin;
   // ...
   contin:;
}
```

**do-while 循环**：
```c
do {
    // ...
    continue; // 等价于 goto contin;
    // ...
    contin:;
} while (/* ... */);
```

**for 循环**：
```c
for (/* ... */) {
   // ...
   continue; // 等价于 goto contin;
   // ...
   contin:;
}
```

### 关键区别

| 循环类型 | continue 后执行 |
|----------|-----------------|
| while | 条件表达式 |
| do-while | 条件表达式 |
| for | 迭代表达式，然后条件表达式 |

### 执行流程图

```
┌─────────────────────────────────────┐
│         执行到 continue 语句         │
└─────────────────┬───────────────────┘
                  │
        ┌─────────┴─────────┐
        │                   │
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│  for 循环     │   │ while/do-while│
└───────┬───────┘   └───────┬───────┘
        │                   │
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│ 执行迭代表达式 │   │ 执行条件表达式 │
└───────┬───────┘   └───────┬───────┘
        │                   │
        └─────────┬─────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│          检查条件，决定是否继续       │
└─────────────────────────────────────┘
```

---

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 过滤特定值 | 跳过不需要处理的迭代 |
| 简化嵌套 | 避免深层 if 嵌套 |
| 条件处理 | 满足条件时跳过后续代码 |

### 最佳实践

1. **替代深层嵌套**：用 continue 替代多层 if-else
2. **过滤条件置前**：将过滤条件放在循环开始处
3. **保持可读性**：避免过多 continue 导致控制流混乱

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 在 switch 中误用 | continue 对 switch 无效 | 确保在循环体内使用 |
| while 循环中跳过更新 | 可能导致无限循环 | 确保更新语句在 continue 之前 |

---

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

int main(void)
{
    for (int i = 0; i < 10; i++) {
        if (i != 5) continue;
        printf("%d ", i);  // 只有当 i == 5 时执行
    }

    printf("\n");

    for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 5; k++) {  // 只有这个循环受 continue 影响
            if (k == 3) continue;
            printf("%d%d ", j, k);    // k == 3 时跳过
        }
    }

    return 0;
}
```

**输出：**
```
5
00 01 02 04 10 11 12 14
```

### 过滤特定值

```c
#include <stdio.h>

int main(void)
{
    int arr[] = {1, -2, 3, -4, 5, -6, 7};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Positive numbers: ");
    for (int i = 0; i < size; i++)
    {
        if (arr[i] <= 0)
            continue;  // 跳过非正数
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
```

**输出：**
```
Positive numbers: 1 3 5 7
```

### 简化嵌套结构

```c
#include <stdio.h>

// 不使用 continue：深层嵌套
void process_without_continue(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] > 0)
        {
            if (arr[i] % 2 == 0)
            {
                printf("%d is positive even\n", arr[i]);
            }
        }
    }
}

// 使用 continue：扁平结构
void process_with_continue(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] <= 0)
            continue;
        if (arr[i] % 2 != 0)
            continue;
        printf("%d is positive even\n", arr[i]);
    }
}

int main(void)
{
    int arr[] = {2, -1, 4, 3, -5, 6};
    process_with_continue(arr, 6);
    return 0;
}
```

### while 循环中的注意事项

```c
#include <stdio.h>

int main(void)
{
    int i = 0;

    // 错误示例：无限循环
    // while (i < 10) {
    //     if (i == 5) continue;  // 跳过了 i++，导致无限循环
    //     printf("%d ", i);
    //     i++;
    // }

    // 正确写法：在 continue 之前更新
    while (i < 10) {
        i++;  // 放在 continue 之前
        if (i == 5) continue;
        printf("%d ", i);
    }
    printf("\n");

    // 或者使用 for 循环更安全
    for (int j = 0; j < 10; j++) {
        if (j == 5) continue;  // 迭代表达式 j++ 仍会执行
        printf("%d ", j);
    }
    printf("\n");

    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 作用范围 | 最内层的 for、while、do-while 循环 |
| 执行效果 | 跳过当前迭代剩余代码，进入下一次迭代 |
| 不退出循环 | 与 break 不同，continue 不终止循环 |

### continue 在不同循环中的行为

| 循环类型 | continue 后执行 |
|----------|-----------------|
| for | 迭代表达式 → 条件表达式 |
| while | 条件表达式 |
| do-while | 条件表达式 |

### continue vs break 对比

| 特性 | continue | break |
|------|----------|-------|
| 作用 | 跳过当前迭代 | 终止循环 |
| 循环继续 | 是 | 否 |
| 适用范围 | 仅循环 | 循环和 switch |

### 学习建议

1. **理解迭代控制**：continue 控制的是迭代，不是退出
2. **while 中谨慎使用**：确保更新语句在 continue 之前
3. **简化代码结构**：用 continue 替代深层 if 嵌套
4. **for 循环更安全**：迭代表达式总是执行

---

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.8.6.2 The continue statement
- C17 标准 (ISO/IEC 9899:2018): 6.8.6.2 The continue statement (p: 111)
- C11 标准 (ISO/IEC 9899:2011): 6.8.6.2 The continue statement (p: 153)
- C99 标准 (ISO/IEC 9899:1999): 6.8.6.2 The continue statement (p: 138)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.6.6.2 The continue statement