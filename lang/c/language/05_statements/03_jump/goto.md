# goto 语句 (goto statement)

## 1. 概述 (Overview)

**goto 语句**实现无条件跳转，将控制权转移到同一函数内指定标签处的语句。

### 核心概念

- **无条件跳转**：立即跳转到指定标签
- **函数内跳转**：只能在同一函数内跳转
- **标签标识**：使用标签名（后跟冒号）标记跳转目标

### 技术定位

goto 语句属于 C 语言的**跳转语句**（jump statements）类别，是最原始的控制流转移机制。虽然被认为会降低代码可读性，但在某些场景下（如错误处理、退出多层循环）仍有其价值。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

goto 语句是最古老的控制流机制之一，源自汇编语言的跳转指令。1968 年 Edsger Dijkstra 发表著名论文《Go To Statement Considered Harmful》，引发结构化编程运动，倡导限制 goto 的使用。

### 版本变更

| 标准 | 章节 | 主要变更 |
|------|------|----------|
| C89/C90 | 3.6.6.1 | 初始标准化 |
| C99 | 6.8.6.1 | 新增 VLA 跳转限制 |
| C11 | 6.8.6.1 | 保持不变 |
| C17 | 6.8.6.1 | 保持不变 |
| C23 | 6.8.6.1 | 标签后语句可选 |

### C99 新增限制

- 不允许跳转到 VLA（变长数组）或其他可变修改类型的作用域内

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```c
attr-spec-seq(可选) goto label ;
```

**标签语法**：
```c
label : statement      // C23 之前
label :                // C23 起（语句可选）
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr-spec-seq` | (C23 起) 可选的属性列表，应用于 goto 语句 |
| `label` | 目标标签名，必须在同一函数内 |

### 标签特性

- **函数作用域**：标签是唯一具有函数作用域的标识符
- **可见性**：可在函数内任意位置通过 goto 引用
- **多重标签**：一个语句前可以有多个标签

---

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
┌─────────────────────────────────────┐
│         执行 goto label;            │
└─────────────────┬───────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│    查找函数内的 label 标签           │
└─────────────────┬───────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│    跳转到 label 处继续执行           │
└─────────────────────────────────────┘
```

### 跳转限制

**允许的跳转**：
```c
goto lab1;  // OK: 进入普通变量的作用域
    int n = 5;
lab1:;      // 注意：n 未初始化，相当于 int n;
```

**禁止的跳转**（C99 起）：
```c
//  goto lab2;   // 错误：进入 VLA 或可变修改类型的作用域
     double a[n]; // 变长数组
     int (*p)[n]; // 可变修改指针
lab2:
```

### VLA 的释放与重新分配

如果 goto 离开 VLA 的作用域，VLA 会被释放：

```c
{
   int n = 1;
label:;
   int a[n]; // 重新分配 10 次，每次大小不同
   if (n++ < 10) goto label; // 离开 VM 作用域
}
```

### 标签的作用域

标签具有**函数作用域**（function scope），可在函数内任意位置通过 goto 引用：

```c
void func(int x) {
    if (x == 0)
        goto end;  // 向前跳转

    // ... 正常处理 ...

end:  // 标签在函数末尾
    return;
}
```

---

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 退出多层循环 | break 只能退出一层 |
| 集中错误处理 | 跳转到统一的清理代码 |
| 资源清理 | 函数退出前统一释放资源 |
| 状态机实现 | 跳转到不同状态处理 |

### 最佳实践

1. **限制使用**：仅在没有更好的替代方案时使用
2. **向前跳转**：优先使用向前跳转（向下）而非向后跳转
3. **清晰命名**：使用有意义的标签名
4. **集中处理**：用于集中的错误处理或资源清理

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 跳过初始化 | 跳过变量初始化代码 | 确保变量初始化或使用默认值 |
| 进入 VLA 作用域 | C99 禁止跳入 VLA | 避免在 VLA 作用域前使用 goto |
| 代码可读性 | 过多 goto 导致难以理解 | 限制使用，添加注释 |

### C 与 C++ 的区别

| 特性 | C | C++ |
|------|---|-----|
| 标签后声明 | 需要空语句 | 允许声明作为语句 |
| 跳过初始化 | 允许但危险 | 禁止跳过有初始化的声明 |

---

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

int main(void)
{
    // goto 用于退出多层循环
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            printf("(%d;%d)\n", x, y);
            if (x + y >= 3) goto endloop;
        }
    }
endloop:;
    return 0;
}
```

**输出：**
```
(0;0)
(0;1)
(0;2)
(1;0)
(1;1)
(1;2)
```

### 错误处理模式

```c
#include <stdio.h>
#include <stdlib.h>

int process_file(const char* filename)
{
    FILE* f = NULL;
    char* buffer = NULL;
    int result = 0;

    f = fopen(filename, "r");
    if (!f) {
        result = -1;
        goto cleanup;
    }

    buffer = malloc(1024);
    if (!buffer) {
        result = -2;
        goto cleanup;
    }

    // ... 处理文件 ...

cleanup:
    if (buffer) free(buffer);
    if (f) fclose(f);
    return result;
}
```

### 替代方案对比

```c
#include <stdio.h>

// 使用 goto
int find_with_goto(int arr[][3], int rows, int target)
{
    int found = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 3; j++) {
            if (arr[i][j] == target) {
                found = 1;
                goto found_it;
            }
        }
    }
    printf("Not found\n");
    return 0;
found_it:
    printf("Found %d\n", target);
    return 1;
}

// 使用标志变量
int find_with_flag(int arr[][3], int rows, int target)
{
    int found = 0;
    for (int i = 0; i < rows && !found; i++) {
        for (int j = 0; j < 3 && !found; j++) {
            if (arr[i][j] == target) {
                found = 1;
            }
        }
    }
    if (found)
        printf("Found %d\n", target);
    else
        printf("Not found\n");
    return found;
}

// 使用函数返回
int find_with_return(int arr[][3], int rows, int target)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 3; j++) {
            if (arr[i][j] == target) {
                printf("Found %d\n", target);
                return 1;
            }
        }
    }
    printf("Not found\n");
    return 0;
}
```

### VLA 与 goto

```c
#include <stdio.h>

int main(void)
{
    // 正确：离开 VLA 作用域
    {
        int n = 1;
    label:;
        int a[n];  // 每次分配不同大小
        printf("a[%d] created\n", n);
        if (n++ < 3) goto label;
    }

    // 错误：跳入 VLA 作用域
    // int m = 5;
    // goto inside;  // 错误！
    // {
    //     int arr[m];
    // inside:
    //     printf("Inside\n");  // arr 未正确分配
    // }

    return 0;
}
```

### 标签在声明前

```c
#include <stdio.h>

int main(void)
{
    // C23 之前：标签后必须是语句
    // 使用空语句
    goto skip;
    int x = 10;  // 如果跳过，x 未初始化
skip:;          // 空语句

    // C23 起：标签后可以没有语句
    // goto end;
    // int y = 20;
    // end:  // C23 允许

    printf("x = %d (undefined)\n", x);

    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 跳转范围 | 同一函数内 |
| 标签作用域 | 函数作用域 |
| 跳入 VLA | 禁止（C99 起） |
| 跳过初始化 | 允许但危险 |

### goto 使用原则

| 原则 | 说明 |
|------|------|
| 限制使用 | 仅在必要时使用 |
| 向前跳转 | 优先向下跳转 |
| 清晰命名 | 有意义的标签名 |
| 集中清理 | 统一的清理代码 |

### goto vs 其他跳转方式

| 方式 | 作用范围 | 适用场景 |
|------|----------|----------|
| goto | 函数内任意位置 | 多层退出、错误处理 |
| break | 最内层循环/switch | 单层退出 |
| continue | 最内层循环 | 跳过迭代 |
| return | 函数退出 | 返回值 |

### 学习建议

1. **理解限制**：函数内跳转、VLA 限制
2. **合理使用**：退出多层循环、集中错误处理
3. **替代方案**：优先考虑 break、return、标志变量
4. **代码质量**：避免过度使用降低可读性

---

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.8.6.1 The goto statement
- C17 标准 (ISO/IEC 9899:2018): 6.8.6.1 The goto statement (p: 110-111)
- C11 标准 (ISO/IEC 9899:2011): 6.8.6.1 The goto statement (p: 152-153)
- C99 标准 (ISO/IEC 9899:1999): 6.8.6.1 The goto statement (p: 137-138)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.6.6.1 The goto statement
- Dijkstra, E. W. (1968). "Go To Statement Considered Harmful"