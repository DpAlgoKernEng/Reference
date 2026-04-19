# C 语言合规性（Conformance）

## 1. 概述（Overview）

**合规性（Conformance）** 是 C 语言标准中定义程序和实现之间关系的核心概念，它规定了什么样的程序是"符合标准的"以及什么样的编译器/实现是"符合标准的"。

合规性包含三个层次的定义：

| 类型 | 英文名称 | 定义 |
|------|----------|------|
| 严格合规程序 | Strictly Conforming Program | 仅使用明确定义的语言构造（即具有单一行为的构造），排除未指定、未定义或实现定义的行为，且不超过任何最小实现限制 |
| 合规程序 | Conforming Program | 被合规实现所接受的程序 |
| 合规实现 | Conforming Implementation | 符合标准要求的编译器/运行环境 |

### 核心概念

- **宿主环境（Hosted Environment）**：具有操作系统的环境，可使用完整的标准库功能
- **独立环境（Freestanding Environment）**：无操作系统的环境，仅可使用标准库的子集

## 2. 来源与演变（Origin and Evolution）

### 历史背景

C 语言的合规性标准从 C89/C90 开始定义，目的是确保：

1. **可移植性**：编写的程序可以在不同平台和编译器上运行
2. **实现灵活性**：允许编译器厂商添加扩展，同时保证核心行为一致
3. **嵌入式系统支持**：为无操作系统的嵌入式环境提供精简的标准库子集

### 标准版本演变

| 标准版本 | 年份 | 主要变更 |
|----------|------|----------|
| C89/C90 | 1990 | 首次定义合规性概念（1.7 Compliance） |
| C99 | 1999 | 新增 `<stdbool.h>`、`<stdint.h>` 等头文件 |
| C11 | 2011 | 新增 `<stdalign.h>`、`<stdnoreturn.h>` |
| C17 | 2018 | 修订版，无重大变更 |
| C23 | 2024 | 大幅扩展独立环境支持，新增 `<stdbit.h>`，明确独立环境头文件分类 |

### C23 重大变更

C23 标准对独立环境（Freestanding Environment）的支持进行了显著增强：

- 明确定义了**完全独立头文件（Fully Freestanding Headers）**
- 新增**条件完全独立头文件（Conditionally Fully Freestanding Headers）**
- 新增**部分独立头文件（Partially Freestanding Headers）**

## 3. 语法与参数（Syntax and Parameters）

### 合规实现的分类

```
合规实现（Conforming Implementation）
├── 宿主实现（Hosted Implementation）
│   └── 必须接受任何严格合规程序
│   └── 可使用完整标准库（第7条款）
│
└── 独立实现（Freestanding Implementation）
    └── 必须接受仅使用独立标准库头文件的严格合规程序
    └── 仅需提供第4条款要求的库功能子集
```

### 扩展规则

合规实现可以提供扩展（包括额外的库函数），前提是：

> 扩展不得改变任何严格合规程序的行为。

这意味着编译器添加的扩展功能不能破坏现有合规程序的正确执行。

## 4. 底层原理（Underlying Principles）

### 程序分类的设计动机

标准将程序分为"严格合规"和"合规"两类的原因：

1. **严格合规程序**：追求最大程度的可移植性，避免任何平台特定的行为
2. **合规程序**：允许利用特定实现的扩展功能，增强实用性

### 行为分类与合规性

| 行为类型 | 英文 | 对合规性的影响 |
|----------|------|----------------|
| 明确定义行为 | Defined Behavior | 严格合规程序可以使用 |
| 未定义行为 | Undefined Behavior | 严格合规程序禁止使用 |
| 未指定行为 | Unspecified Behavior | 严格合规程序禁止使用 |
| 实现定义行为 | Implementation-defined Behavior | 严格合规程序禁止使用 |

### 独立环境设计原理

独立环境的设计目标是为嵌入式系统和内核开发提供最小化的运行时支持：

- 不假设存在操作系统
- 不依赖动态内存分配（可选）
- 不依赖文件系统
- 仅提供类型定义、宏定义等基础功能

## 5. 使用场景（Use Cases）

### 宿主环境适用场景

- 桌面应用程序开发
- 服务器端应用程序
- 命令行工具
- 需要 I/O 操作的程序

### 独立环境适用场景

- 嵌入式系统固件
- 操作系统内核开发
- 设备驱动程序
- 引导加载程序（Bootloader）
- 实时操作系统（RTOS）

### 最佳实践

#### 编写可移植代码

```c
/* 推荐：使用标准定义的类型和宏 */
#include <stdint.h>
#include <stddef.h>

int32_t calculate(int32_t a, int32_t b) {
    return a + b;
}
```

#### 避免未定义行为

```c
/* 避免：未定义行为 */
int *ptr = NULL;
int value = *ptr;  // 解引用空指针 - 未定义行为

/* 正确做法 */
if (ptr != NULL) {
    int value = *ptr;
}
```

### 常见陷阱

1. **误以为所有标准库函数都可在独立环境使用**
   - 错误：在内核开发中使用 `printf()`
   - 修正：仅使用独立环境提供的头文件

2. **忽略实现定义行为的差异**
   - 错误：假设 `int` 总是 32 位
   - 修正：使用 `<stdint.h>` 中的固定宽度类型

## 6. 代码示例（Examples）

### 示例 1：严格合规程序

```c
/* strictly_conforming.c - 严格合规程序示例 */
#include <stdio.h>
#include <stdint.h>

int main(void) {
    int32_t x = 42;
    int32_t y = 10;
    int32_t sum = x + y;

    printf("Sum: %" PRId32 "\n", sum);
    return 0;
}
```

> 说明：此程序仅使用明确定义的行为，使用固定宽度类型确保可移植性，不依赖任何实现定义的行为。

### 示例 2：独立环境程序

```c
/* freestanding.c - 独立环境程序示例 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 独立环境下可用，无需操作系统支持 */
void kernel_entry(void) {
    volatile uint32_t *uart = (volatile uint32_t *)0x40000000;

    /* 简单的 UART 输出 */
    const char *msg = "Hello from freestanding C!\n";
    while (*msg) {
        *uart = (uint32_t)*msg++;
    }

    /* 无限循环（内核主循环） */
    while (true) {
        /* 等待中断 */
    }
}
```

> 说明：此程序适用于独立环境，仅使用了独立环境支持的 `<stdint.h>`、`<stddef.h>` 和 `<stdbool.h>` 头文件。

### 示例 3：常见错误及修正

```c
/* 错误示例：在独立环境使用不可用的库函数 */

/* freestanding_broken.c - 错误：使用了独立环境不支持的函数 */
#include <stdio.h>   /* 独立环境可能不提供 */
#include <stdlib.h>  /* 独立环境可能不提供完整功能 */

void entry_point(void) {
    printf("Starting...\n");  /* 错误：printf 可能不可用 */
    void *mem = malloc(1024);  /* 错误：malloc 可能不可用 */
}
```

```c
/* 正确示例：仅在独立环境使用支持的功能 */

/* freestanding_correct.c - 正确：使用独立环境支持的头文件 */
#include <stdint.h>
#include <stddef.h>
#include <stdbit.h>  /* C23 新增 */

/* 自定义简单的内存管理或使用静态分配 */
static uint8_t memory_pool[4096];
static size_t pool_offset = 0;

void *simple_alloc(size_t size) {
    if (pool_offset + size > sizeof(memory_pool)) {
        return NULL;
    }
    void *ptr = &memory_pool[pool_offset];
    pool_offset += size;
    return ptr;
}

void entry_point(void) {
    void *mem = simple_alloc(1024);  /* 正确：使用自定义分配 */
    /* ... */
}
```

### 示例 4：条件独立环境功能（C23）

```c
/* conditional_freestanding.c - C23 条件独立环境示例 */
#if defined(__STDC_IEC_60559_BFP__) || defined(__STDC_IEC_60559_DFP__)
#include <math.h>
#include <fenv.h>
#define HAS_FP_SUPPORT 1
#else
#define HAS_FP_SUPPORT 0
#endif

#include <stdint.h>

float compute_value(float x) {
#if HAS_FP_SUPPORT
    /* 当浮点环境支持可用时 */
    return sqrtf(x);
#else
    /* 回退方案 */
    return x * 0.5f;  /* 简化近似 */
#endif
}
```

## 7. 总结（Summary）

### 核心要点

| 概念 | 关键点 |
|------|--------|
| 严格合规程序 | 最大可移植性，避免所有未定义/未指定/实现定义行为 |
| 合规程序 | 可利用实现扩展，但需符合实现要求 |
| 合规实现 | 必须支持严格合规程序，可提供不改变行为的扩展 |
| 宿主环境 | 有操作系统，支持完整标准库 |
| 独立环境 | 无操作系统，仅支持精简标准库子集 |

### 独立环境头文件分类（C23）

```
独立环境标准库头文件
├── 完全独立（Fully Freestanding）
│   ├── <float.h>      - 浮点类型限制
│   ├── <iso646.h>     - 替代运算符拼写（C95起）
│   ├── <limits.h>     - 整数类型范围
│   ├── <stdalign.h>   - 对齐宏（C11起）
│   ├── <stdarg.h>     - 可变参数
│   ├── <stdbool.h>    - 布尔类型宏（C99起）
│   ├── <stddef.h>     - 通用宏定义
│   ├── <stdint.h>     - 固定宽度整数（C99起）
│   ├── <stdnoreturn.h> - noreturn宏（C11起）
│   └── <stdbit.h>     - 位操作宏（C23起）
│
├── 条件完全独立（Conditionally Fully Freestanding）
│   ├── <fenv.h>       - 浮点环境（需 IEC 60559 支持）
│   └── <math.h>       - 数学函数（需 IEC 60559 支持）
│
└── 部分独立（Partially Freestanding）
    ├── <stdlib.h>     - 仅 memalignment 和部分数值转换
    └── <string.h>     - 部分函数不可用（如 strdup、strtok）
```

### 学习建议

1. **理解标准条款**：仔细阅读 C 标准第 4 条款（Conformance）
2. **区分环境类型**：根据目标平台选择宿主或独立环境
3. **查阅编译器文档**：了解具体实现的扩展和限制
4. **使用静态分析工具**：检测潜在的未定义行为
5. **关注 C23 更新**：C23 大幅扩展了独立环境支持

### 参考资源

- C23 标准（ISO/IEC 9899:2024）第 4 条款
- C17 标准（ISO/IEC 9899:2018）第 4 条款
- C11 标准（ISO/IEC 9899:2011）第 4 条款
- C99 标准（ISO/IEC 9899:1999）第 4 条款
- C89/C90 标准（ISO/IEC 9899:1990）第 1.7 条款