# extern 关键字与外部定义

## 1. 概述 (Overview)

在 C 语言中，**外部定义 (External Definition)** 是指出现在任何函数之外的声明，用于声明具有外部链接 (external linkage) 或内部链接 (internal linkage) 的函数和对象。这些声明被称为"外部声明"，因为它们出现在函数体之外。

### 核心概念

| 概念 | 英文术语 | 说明 |
|------|----------|------|
| 外部声明 | External Declaration | 出现在函数外部的声明 |
| 外部定义 | External Definition | 分配存储空间的声明 |
| 尝试性定义 | Tentative Definition | 可能成为定义的声明 |
| 单一定义规则 | One Definition Rule (ODR) | 每个标识符只能有一个外部定义 |

通过 `extern` 关键字声明的对象具有**静态存储期 (static storage duration)**，其标识符具有**文件作用域 (file scope)**。这意味着这些对象在程序启动时创建，程序结束时销毁，且在整个文件内可见。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

**尝试性定义 (Tentative Definition)** 的概念是为了标准化 C89 之前各种前向声明内部链接标识符的方法而引入的。在早期的 C 语言实现中，不同编译器对于多次声明同一标识符的处理方式不一致，尝试性定义的引入解决了这一问题。

### 标准演进

| 标准 | 版本 | 年份 | 主要变更 |
|------|------|------|----------|
| C89/C90 | ISO/IEC 9899:1990 | 1990 | 首次标准化外部定义和尝试性定义 |
| C99 | ISO/IEC 9899:1999 | 1999 | 引入 inline 函数定义的 ODR 豁免 |
| C11 | ISO/IEC 9899:2011 | 2011 | 新增 `_Alignof` 运算符豁免 |
| C17 | ISO/IEC 9899:2018 | 2018 | 无重大变更 |
| C23 | ISO/IEC 9899:2024 | 2024 | `auto` 可用于类型推断；`alignof` 和 `typeof` 运算符豁免 |

### 标准引用

- **C23**: 6.9 External definitions
- **C17**: 6.9 External definitions (p. 113-116)
- **C11**: 6.9 External definitions (p. 155-159)
- **C99**: 6.9 External definitions (p. 140-144)
- **C89/C90**: 3.7 EXTERNAL DEFINITIONS

## 3. 语法与参数 (Syntax and Parameters)

### 外部声明语法

```c
// 外部声明（声明但不定义）
extern int n;                    // 外部链接声明

// 外部定义（声明并定义）
int b = 1;                      // 外部链接定义
static const char *c = "abc";   // 内部链接定义

// 函数定义
int f(void)                     // 外部链接函数定义
{
    int a = 1;                  // 非外部声明（局部变量）
    return b;
}

static void x(void)             // 内部链接函数定义
{
}
```

### 尝试性定义语法

尝试性定义是一种特殊的外部声明，具有以下特征：

1. 没有初始化器 (initializer)
2. 没有存储类说明符，或使用 `static` 说明符

```c
int i;           // 尝试性定义（外部链接）
static int j;    // 尝试性定义（内部链接）
```

### 存储类说明符对链接的影响

| 说明符 | 链接类型 | 说明 |
|--------|----------|------|
| 无说明符（文件作用域） | 外部链接 | 默认行为 |
| `extern` | 保持已有链接或外部链接 | 不改变已建立的链接 |
| `static` | 内部链接 | 限制在本翻译单元内 |

## 4. 底层原理 (Underlying Principles)

### 链接 (Linkage)

链接决定了标识符在不同作用域和翻译单元中的可见性：

| 链接类型 | 英文 | 可见范围 |
|----------|------|----------|
| 外部链接 | External Linkage | 整个程序的所有翻译单元 |
| 内部链接 | Internal Linkage | 仅在当前翻译单元内 |
| 无链接 | No Linkage | 仅在当前块作用域内 |

**重要规则**：`extern` 声明不会改变已建立的链接，而尝试性定义可能与同标识符的其他声明产生链接冲突，导致未定义行为。

### 存储期 (Storage Duration)

外部声明的对象具有静态存储期：
- 程序启动时分配内存
- 程序结束时释放内存
- 未显式初始化时自动零初始化

### 声明与定义的区别

| 类型 | 分配存储 | 初始化 | 可重复性 |
|------|----------|--------|----------|
| 声明 (Declaration) | 否 | 否 | 可多次声明 |
| 定义 (Definition) | 是 | 可以 | 唯一（ODR 约束） |
| 尝试性定义 | 可能 | 零初始化 | 可多次出现 |

### 尝试性定义的行为

尝试性定义的解析规则：

1. **存在实际定义时**：尝试性定义仅作为声明
2. **不存在实际定义时**：尝试性定义成为实际定义，对象被零初始化

```c
int i3;        // 尝试性定义
int i3;        // 尝试性定义（可重复）
extern int i3; // 声明
// 如果没有其他定义，等价于 "int i3 = 0;"
```

## 5. 使用场景 (Use Cases)

### 跨文件共享全局变量

使用 `extern` 在多个源文件间共享全局变量：

```c
// file1.c
int global_counter = 0;  // 定义

// file2.c
extern int global_counter;  // 声明（使用 file1.c 中的定义）
```

### 防止重复定义

在头文件中使用 `extern` 声明，在单一源文件中定义：

```c
// header.h
#ifndef HEADER_H
#define HEADER_H
extern int shared_data;  // 仅声明
#endif

// source.c
#include "header.h"
int shared_data = 42;    // 唯一定义
```

### 尝试性定义的用途

尝试性定义允许在不确定是否有其他定义的情况下安全声明：

```c
int buffer_size;  // 尝试性定义
// 如果其他地方没有定义，此处将成为定义并初始化为 0
```

### 常见陷阱

#### 陷阱 1：链接冲突

```c
static int i4 = 2;  // 内部链接定义
int i4;              // 未定义行为：链接冲突！
```

#### 陷阱 2：不完整类型

```c
static int i[];  // 错误：静态尝试性定义必须具有完整类型
int i[];         // 正确：等价于 int i[1] = {0};
```

#### 陷阱 3：多重定义

```c
// file1.c
int x = 1;  // 定义

// file2.c
int x = 2;  // 链接错误：多重定义！
```

### ODR 使用豁免场景

以下表达式使用标识符时，不要求存在定义：

| 运算符 | 版本要求 |
|--------|----------|
| `sizeof` | 所有版本 |
| `_Alignof` | C11 起，C23 止 |
| `alignof` | C23 起 |
| `typeof` | C23 起 |
| 非 VLA 表达式 | C99 起 |

## 6. 代码示例 (Examples)

### 基础用法：外部声明与定义

```c
// extern_basic.c
#include <stdio.h>

// 外部声明（仅声明，不分配存储）
extern int external_var;

// 外部定义（声明并分配存储）
int defined_var = 100;

// 内部链接定义
static int internal_var = 50;

int main(void)
{
    // 使用外部声明的变量（需要在其他地方定义）
    printf("external_var = %d\n", external_var);

    // 使用本文件定义的变量
    printf("defined_var = %d\n", defined_var);
    printf("internal_var = %d\n", internal_var);

    return 0;
}

// 在另一个文件中定义
// other.c: int external_var = 200;
```

### 尝试性定义行为

```c
// tentative.c
#include <stdio.h>

int i1 = 1;     // 定义，外部链接
int i1;         // 尝试性定义，作为声明（因为 i1 已定义）
extern int i1;  // 声明，引用之前的定义

extern int i2 = 3; // 定义，外部链接（extern 可与定义共存）
int i2;            // 尝试性定义，作为声明
extern int i2;     // 声明

int i3;        // 尝试性定义，外部链接
int i3;        // 尝试性定义，外部链接
extern int i3; // 声明
               // 无其他定义时，i3 被零初始化：i3 = 0

int main(void)
{
    printf("i1 = %d\n", i1);  // 输出: i1 = 1
    printf("i2 = %d\n", i2);  // 输出: i2 = 3
    printf("i3 = %d\n", i3);  // 输出: i3 = 0
    return 0;
}
```

### 常见错误：链接冲突

```c
// error_linkage.c
// 编译此文件会导致未定义行为

static int i4 = 2;   // 定义，内部链接
int i4;               // 错误：链接冲突！未定义行为
extern int i4;        // 声明，引用内部链接定义

static int i5;        // 尝试性定义，内部链接
int i5;               // 错误：链接冲突！未定义行为
extern int i5;        // 引用之前的定义（内部链接）

// 正确做法：保持链接一致性
// 方案 1：全部使用 static
// 方案 2：全部不使用 static（外部链接）
```

### 正确用法：多文件项目

```c
// config.h - 头文件
#ifndef CONFIG_H
#define CONFIG_H

// 仅声明，不定义
extern int config_value;
extern const char *config_name;

#endif // CONFIG_H
```

```c
// config.c - 定义文件
#include "config.h"

int config_value = 42;
const char *config_name = "default";
```

```c
// main.c - 使用文件
#include <stdio.h>
#include "config.h"

int main(void)
{
    printf("config_value = %d\n", config_value);
    printf("config_name = %s\n", config_name);
    return 0;
}
```

### 不完整类型处理

```c
// incomplete_type.c

// 错误：静态尝试性定义必须具有完整类型
// static int arr[];  // 编译错误

// 正确：非静态尝试性定义允许不完整类型
int arr[];  // 等价于 int arr[1] = {0};

// 或稍后提供完整定义
int flexible[];
int flexible[10];  // 后续提供完整定义
```

## 7. 总结 (Summary)

### 核心要点

| 概念 | 要点 |
|------|------|
| extern 关键字 | 声明外部链接标识符，不改变已建立的链接 |
| 外部定义 | 函数外部的声明，分配静态存储期 |
| 尝试性定义 | 无初始化器的外部声明，可成为零初始化的定义 |
| 单一定义规则 | 每个外部链接标识符在整个程序中只能有一个定义 |
| 链接冲突 | static 与非 static 声明冲突导致未定义行为 |

### 最佳实践

1. **头文件中声明，源文件中定义**：在 `.h` 文件中使用 `extern` 声明，在单一 `.c` 文件中定义
2. **避免尝试性定义**：明确区分声明和定义，减少潜在歧义
3. **保持链接一致性**：同一标识符的所有声明必须具有相同的链接属性
4. **使用 static 保护内部标识符**：限制作用域，避免命名冲突
5. **静态尝试性定义需完整类型**：避免使用不完整类型的静态尝试性定义

### 技术对比

| 特性 | extern 声明 | 尝试性定义 | 实际定义 |
|------|------------|------------|----------|
| 分配存储 | 否 | 可能 | 是 |
| 可重复性 | 是 | 是 | 否（ODR） |
| 初始化器 | 不允许 | 不允许 | 允许 |
| 默认值 | N/A | 零初始化 | 指定值/零初始化 |

### 学习建议

1. 理解声明与定义的区别是掌握 C 语言模块化编程的基础
2. 尝试性定义是 C 语言特有的概念，C++ 不支持此特性
3. 在大型项目中，遵循"头文件声明、源文件定义"的原则
4. 使用 `static` 限制全局变量的作用域，减少命名冲突