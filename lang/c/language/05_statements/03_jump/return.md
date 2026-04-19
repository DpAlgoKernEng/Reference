# return 语句 (return statement)

## 1. 概述 (Overview)

**return 语句**用于终止当前函数的执行，并将控制权（可能还有返回值）返回给调用者。

### 核心概念

- **函数终止**：立即结束当前函数的执行
- **值返回**：将表达式的值返回给调用者
- **控制转移**：将控制权返回给调用函数

### 技术定位

return 语句属于 C 语言的**跳转语句**（jump statements）类别，是函数返回的标准机制。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

return 语句源自函数式编程的概念，是函数返回值的标准机制。它提供了从函数任意位置退出的能力，而不仅仅是函数末尾。

### 版本变更

| 标准 | 章节 | 主要变更 |
|------|------|----------|
| C89/C90 | 3.6.6.4 | 初始标准化 |
| C99 | 6.8.6.4 | 保持不变 |
| C11 | 6.8.6.4 | 新增 _Noreturn 函数限制 |
| C17 | 6.8.6.4 | 保持不变 |
| C23 | 6.8.6.4 | 新增属性说明符序列支持 |

### C11 新增

- 在 `_Noreturn` 函数中执行 return 语句是未定义行为

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```c
attr-spec-seq(可选) return expression ;    (1)
attr-spec-seq(可选) return ;               (2)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr-spec-seq` | (C23 起) 可选的属性列表，应用于 return 语句 |
| `expression` | 用于初始化函数返回值的表达式 |

### 使用规则

| 形式 | 适用函数类型 | 说明 |
|------|--------------|------|
| `return expression;` | 非 void 返回类型 | 返回表达式的值 |
| `return;` | void 返回类型 | 仅返回控制权 |

### 类型转换

如果表达式类型与函数返回类型不同：
- 值被转换，如同赋值给返回类型的对象
- 允许对象表示之间的重叠

---

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
┌─────────────────────────────────────┐
│         执行 return 语句             │
└─────────────────┬───────────────────┘
                  │
        ┌─────────┴─────────┐
        │                   │
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│return expr;   │   │return;        │
└───────┬───────┘   └───────┬───────┘
        │                   │
        ▼                   │
┌───────────────┐           │
│求值表达式     │           │
└───────┬───────┘           │
        │                   │
        ▼                   │
┌───────────────┐           │
│类型转换       │           │
└───────┬───────┘           │
        │                   │
        └─────────┬─────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│  返回控制权给调用函数                 │
│  （返回值成为函数调用表达式的值）      │
└─────────────────────────────────────┘
```

### 返回值传递

- 表达式的值被复制到调用者可以访问的位置
- 浮点类型可能有更大的范围和精度

### 隐式 return

**void 函数**：
```c
void func(void)
{
    // 到达函数末尾等价于 return;
}
```

**非 void 函数**：
```c
int func(void)
{
    // 到达函数末尾：如果使用了返回值，则行为未定义
    // 允许丢弃返回值（不使用函数结果）
}
```

### _Noreturn 函数（C11 起）

在声明为 `_Noreturn` 的函数中执行 return 语句是未定义行为：

```c
_Noreturn void fatal_error(void)
{
    // 不应该执行 return
    // 执行 return 是未定义行为
}
```

---

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 函数返回值 | 返回计算结果 |
| 提前退出 | 满足条件时提前返回 |
| 错误处理 | 返回错误码或特殊值 |
| void 函数 | 提前退出，不返回值 |

### 最佳实践

1. **每个路径都有返回值**：非 void 函数确保所有执行路径都有 return
2. **提前返回**：简化代码，减少嵌套
3. **一致性**：保持返回值类型与声明一致
4. **检查返回值**：调用者应检查返回值

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 缺少返回值 | 非 void 函数末尾无 return | 确保所有路径都有 return |
| 类型不匹配 | 返回类型与声明不一致 | 保持类型一致或显式转换 |
| 返回局部变量地址 | 返回局部变量的指针 | 返回值而非地址 |
| void 函数返回值 | void 函数使用 return expr | 使用 return; |

---

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

void fa(int i)
{
    if (i == 2)
       return;
    printf("fa():   %d\n", i);
} // 隐式 return;

int fb(int i)
{
    if (i > 4)
       return 4;
    printf("fb():   %d\n", i);
    return 2;
}

int main(void)
{
    fa(2);
    fa(1);
    int i = fb(5);   // 返回值 4 用于初始化 i
    i = fb(i);       // 返回值 2 作为赋值右侧
    printf("main(): %d\n", i);

    return 0;
}
```

**输出：**
```
fa():   1
fb():   4
main(): 2
```

### 提前返回模式

```c
#include <stdio.h>

// 不推荐：深层嵌套
int check_nested(int x)
{
    int result = 0;
    if (x > 0) {
        if (x < 100) {
            if (x % 2 == 0) {
                result = 1;
            }
        }
    }
    return result;
}

// 推荐：提前返回
int check_early_return(int x)
{
    if (x <= 0)
        return 0;
    if (x >= 100)
        return 0;
    if (x % 2 != 0)
        return 0;
    return 1;
}

int main(void)
{
    printf("%d\n", check_early_return(50));  // 1
    printf("%d\n", check_early_return(-1));  // 0
    printf("%d\n", check_early_return(150)); // 0
    printf("%d\n", check_early_return(51));  // 0
    return 0;
}
```

### 错误处理

```c
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR_NULL -1
#define ERROR_SIZE -2

int process_array(int* arr, int size)
{
    if (arr == NULL)
        return ERROR_NULL;
    if (size <= 0)
        return ERROR_SIZE;

    // 处理数组
    for (int i = 0; i < size; i++)
        arr[i] *= 2;

    return SUCCESS;
}

int main(void)
{
    int arr[] = {1, 2, 3};
    int result = process_array(arr, 3);

    if (result == SUCCESS)
        printf("Success: %d %d %d\n", arr[0], arr[1], arr[2]);
    else
        printf("Error: %d\n", result);

    return 0;
}
```

### 返回结构体

```c
#include <stdio.h>

struct Point {
    int x;
    int y;
};

struct Point create_point(int x, int y)
{
    struct Point p = {x, y};
    return p;  // 返回结构体值
}

// 对象表示重叠示例
struct s { double i; };

union {
    struct { int f1; struct s f2; } u1;
    struct { struct s f3; int f4; } u2;
} g;

struct s f(void)
{
    return g.u1.f2;  // 允许重叠
}

int main(void)
{
    struct Point p = create_point(10, 20);
    printf("Point: (%d, %d)\n", p.x, p.y);
    return 0;
}
```

### 常见错误

```c
#include <stdio.h>
#include <stdlib.h>

// 错误 1：返回局部变量地址
int* bad_return_local(void)
{
    int local = 10;
    return &local;  // 错误：返回局部变量地址
}

// 正确：返回指针参数或动态分配
int* good_return_dynamic(void)
{
    int* p = malloc(sizeof(int));
    if (p) *p = 10;
    return p;
}

// 错误 2：void 函数返回值
void bad_void_return(void)
{
    // return 10;  // 错误：void 函数不能返回值
    return;        // 正确
}

// 错误 3：缺少返回值
int bad_missing_return(int x)
{
    if (x > 0)
        return 1;
    // 错误：x <= 0 时没有返回值
    // 如果使用返回值，行为未定义
}

// 正确：所有路径都有返回值
int good_all_paths(int x)
{
    if (x > 0)
        return 1;
    return 0;
}

int main(void)
{
    int* p = good_return_dynamic();
    if (p) {
        printf("Value: %d\n", *p);
        free(p);
    }
    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 函数终止 | 立即结束当前函数 |
| 值返回 | 表达式值返回给调用者 |
| 类型转换 | 自动转换为返回类型 |
| 隐式 return | void 函数末尾隐式 return; |

### return 语句形式

| 形式 | 适用函数 | 行为 |
|------|----------|------|
| `return expr;` | 非 void | 返回表达式值 |
| `return;` | void | 仅返回控制权 |
| （到达末尾） | void | 等价于 return; |
| （到达末尾） | 非 void | 使用返回值时未定义 |

### 返回值注意事项

| 注意点 | 说明 |
|--------|------|
| 类型匹配 | 保持与声明一致 |
| 所有路径返回 | 非 void 函数确保每个路径都有 return |
| 避免返回局部地址 | 不要返回局部变量的指针 |
| _Noreturn 限制 | C11 起，不返回的函数不应执行 return |

### 学习建议

1. **理解返回机制**：值如何传递给调用者
2. **提前返回**：简化代码结构
3. **检查返回值**：调用者应处理返回值
4. **避免常见错误**：缺少返回值、返回局部地址

---

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.8.6.4 The return statement
- C17 标准 (ISO/IEC 9899:2018): 6.8.6.4 The return statement (p: 111-112)
- C11 标准 (ISO/IEC 9899:2011): 6.8.6.4 The return statement (p: 154)
- C99 标准 (ISO/IEC 9899:1999): 6.8.6.4 The return statement (p: 139)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.6.6.4 The return statement