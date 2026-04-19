# static_assert - 静态断言

## 1. 概述

`static_assert` 是 C 语言中的**静态断言**（static assertion）机制，用于在**编译时**检查常量表达式的真假。当条件为假时，编译器会报错并显示指定的错误消息；当条件为真时，不产生任何代码，对程序运行无影响。

静态断言是一种**编译时诊断工具**，与运行时断言 `assert()` 不同，它在编译阶段就能发现错误，帮助开发者在代码运行之前就捕获潜在问题。

静态断言定义在 `<assert.h>` 头文件中（C23 之前），或作为关键字直接使用（C23 起）。

## 2. 来源与演变

### 首次引入

静态断言在 **C11** 标准中首次引入，关键字为 `_Static_assert`。在此之前，C 语言没有原生的编译时断言机制，开发者需要使用各种技巧来实现类似功能。

### 历史背景

在 C11 之前，开发者通常使用以下技巧实现编译时断言：

```c
// 旧式技巧：利用数组大小不能为负的特性
#define STATIC_ASSERT(cond) \
    typedef char static_assert_##__LINE__[(cond) ? 1 : -1]
```

这种方法存在局限性：
- 语法不够直观
- 错误消息不清晰
- 在某些作用域无法使用

C11 引入 `_Static_assert` 解决了这些问题，提供了：
- 直观的语法形式
- 可自定义的错误消息
- 可在任意作用域使用

### C11 版本

| 语法 | 说明 |
|------|------|
| `_Static_assert(expression, message)` | 必须提供消息参数 |
| `static_assert` 宏 | 在 `<assert.h>` 中定义为 `_Static_assert` 的别名 |

### C23 版本变化

C23 标准对静态断言进行了重要改进：

| 变化 | 说明 |
|------|------|
| `static_assert` 成为关键字 | 与 C++ 保持一致，不再需要头文件 |
| 消息参数可选 | 可以省略错误消息 |
| `_Static_assert` 被弃用 | 保留仅为向后兼容 |
| 头文件不再提供 `static_assert` 宏 | 可作为预定义宏存在 |

### 与 C++ 的关系

`static_assert` 最早在 **C++11** 中引入。C11 将其加入 C 标准（使用 `_Static_assert` 形式），C23 最终统一了 C 和 C++ 的语法，使 `static_assert` 成为首选关键字。

## 3. 语法与参数

### 基本语法

**C11 语法（需要消息参数）：**
```c
_Static_assert(expression, message)
```

**C23 语法：**
```c
static_assert(expression, message)  // 带消息
static_assert(expression)           // 不带消息（C23 新增）
_Static_assert(expression, message) // 已弃用，保留兼容
_Static_assert(expression)          // 已弃用，保留兼容
```

### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| `expression` | 整数常量表达式（integer constant expression） | 在编译时求值的整数常量表达式，结果与零比较 |
| `message` | 字符串字面量（string literal） | 断言失败时显示的错误消息，C23 起可选 |

### 关键字

| 关键字 | 版本 | 状态 |
|--------|------|------|
| `_Static_assert` | C11 起 | C23 起弃用 |
| `static_assert` | C23 起 | 当前推荐 |

### 头文件依赖

| 版本 | 依赖 |
|------|------|
| C11 - C17 | 需要 `#include <assert.h>` 使用 `static_assert` 宏 |
| C23 | 无需头文件，`static_assert` 为关键字 |

### 使用位置

静态断言可以在以下位置使用：

| 位置 | 说明 |
|------|------|
| 文件作用域 | 全局声明区 |
| 函数内部 | 语句块中 |
| 结构体/联合体定义中 | 成员声明之间 |

## 4. 底层原理

### 编译时求值

静态断言的核心机制是**编译时常量表达式求值**：

1. **表达式求值**：编译器在编译阶段计算 `expression` 的值
2. **与零比较**：将结果与零比较
3. **条件判断**：
   - 结果等于零：触发编译错误，显示消息
   - 结果不等于零：断言通过，不产生任何代码

### 无运行时开销

静态断言在编译时处理，**完全不产生运行时代码**：

```
源代码阶段：static_assert(...)
    ↓
编译阶段：求值、检查
    ↓
编译结果：成功时无代码生成 / 失败时编译错误
```

### 整数常量表达式要求

`expression` 必须是**整数常量表达式**（integer constant expression），这意味着：

| 允许的表达式 | 不允许的表达式 |
|--------------|----------------|
| 字面量（如 `1`, `42`） | 变量（即使是 `const` 变量，C23 之前） |
| 枚举常量 | 函数调用 |
| `sizeof` 表达式 | 运行时计算的值 |
| `alignof` 表达式 | 非 `constexpr` 变量（C23 前） |
| `constexpr` 变量（C23） | |

### 错误消息显示

当断言失败时：
- 编译器**必须**显示错误消息（C23 之前：仅限基本字符集）
- 编译器**应该**显示错误消息（C23 起，如果提供了消息）
- 错误消息帮助开发者快速定位问题

## 5. 使用场景

### 适用场景

| 场景 | 示例 |
|------|------|
| 类型大小验证 | 检查 `int` 是否为 4 字节 |
| 结构体布局检查 | 验证结构体大小是否符合预期 |
| 平台特性验证 | 检查指针大小是否为预期值 |
| 编译时条件检查 | 验证编译时常量是否满足条件 |
| 接口契约验证 | 确保编译时参数满足要求 |

### 典型应用

#### 1. 验证类型大小

```c
static_assert(sizeof(int) == 4, "int must be 4 bytes");
static_assert(sizeof(void*) == 8, "64-bit platform required");
```

#### 2. 结构体布局验证

```c
struct packet {
    uint32_t header;
    uint32_t length;
    uint8_t data[56];
};

static_assert(sizeof(struct packet) == 64, "packet size must be 64 bytes");
```

#### 3. 枚举值验证

```c
enum status {
    STATUS_OK = 0,
    STATUS_ERROR = 1,
    STATUS_PENDING = 2
};

static_assert(STATUS_ERROR > STATUS_OK, "STATUS_ERROR must be greater than STATUS_OK");
```

#### 4. 版本检查

```c
#define VERSION_MAJOR 2
#define VERSION_MINOR 5

static_assert(VERSION_MAJOR >= 2, "Version 2.0 or higher required");
```

### 最佳实践

1. **放在头文件中**：在头文件中使用静态断言检查编译时条件
2. **提供有意义的消息**：消息应清楚说明检查的内容和失败原因
3. **避免复杂表达式**：保持断言条件简单明了
4. **配合 `sizeof` 使用**：常用于检查类型大小
5. **尽早放置**：在文件顶部或结构体定义后立即放置断言

### 注意事项

| 事项 | 说明 |
|------|------|
| 表达式必须是编译时常量 | 运行时变量不能使用 |
| C23 之前必须提供消息 | C11/C17 省略消息会导致编译错误 |
| 断言失败终止编译 | 无法跳过或忽略 |
| 无运行时影响 | 通过的断言不产生任何代码 |

### 常见陷阱

#### 陷阱 1：使用 const 变量（C23 之前）

```c
const int size = 10;
// static_assert(size == 10, "...");  // 错误：size 不是常量表达式（C23 前）
```

#### 陷阱 2：消息参数的编码

```c
// C23 之前：非基本字符集的字符可能无法正确显示
_Static_assert(1, "中文消息");  // 可能显示不正确（C23 前）
```

## 6. 代码示例

### 基础用法

```c
#include <assert.h>  // C23 之前需要此头文件使用 static_assert 宏

int main(void)
{
    // C23 语法：使用 static_assert 关键字
    static_assert((2 + 2) % 3 == 1, "Math check: (2+2)%3 should equal 1");

    // C11/C17 语法：使用 _Static_assert 关键字
    _Static_assert(2 + 2 * 2 == 6, "Basic arithmetic check");

    // C23 起：消息可以省略
    static_assert(sizeof(int) >= 4);  // 无消息形式

    return 0;
}
```

### 在结构体定义中使用

```c
#include <stdint.h>

// 网络数据包结构
struct network_packet {
    uint32_t magic;      // 4 bytes
    uint32_t sequence;   // 4 bytes
    uint32_t length;     // 4 bytes
    uint8_t  data[52];   // 52 bytes
};  // 总计 64 bytes

// 验证结构体大小
static_assert(sizeof(struct network_packet) == 64,
    "network_packet must be exactly 64 bytes for protocol compatibility");

// 验证成员偏移
static_assert(offsetof(struct network_packet, data) == 12,
    "data field must start at offset 12");
```

### 平台适配验证

```c
#include <stdint.h>

// 检查平台特性
static_assert(sizeof(void*) == 8, "This code requires a 64-bit platform");
static_assert(CHAR_BIT == 8, "This code assumes 8-bit bytes");

// 检查整数类型大小
static_assert(sizeof(int32_t) == 4, "int32_t must be 4 bytes");
static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
```

### 编译时数学验证

```c
#define ARRAY_SIZE 100
#define BUFFER_SIZE (ARRAY_SIZE * sizeof(int))

static_assert(ARRAY_SIZE > 0, "ARRAY_SIZE must be positive");
static_assert(BUFFER_SIZE <= 4096, "Buffer size exceeds maximum allowed");
static_assert((ARRAY_SIZE & (ARRAY_SIZE - 1)) == 0,
    "ARRAY_SIZE must be a power of 2 for optimal performance");
```

### C23 新特性示例

```c
// C23: 使用 constexpr 变量
constexpr int max_items = 100;
static_assert(max_items <= 1000);  // constexpr 变量可用于静态断言

// C23: 消息参数可选
static_assert(sizeof(long) >= 4);  // 无消息，编译器生成默认消息

// C23: static_assert 作为关键字，无需头文件
// 文件顶部无需 #include <assert.h>
```

### 常见错误及修正

#### 错误 1：断言条件为假

```c
// 错误：条件为假，编译失败
// static_assert(sizeof(int) < sizeof(char), "This will always fail!");
```

**编译错误示例：**
```
error: static assertion failed: "This will always fail!"
```

#### 错误 2：使用非常量表达式（C23 之前）

```c
int calculate_size(void) { return 10; }

// 错误：函数调用不是常量表达式
// static_assert(calculate_size() == 10, "Not a constant expression");
```

**修正方式：**
```c
#define SIZE 10
static_assert(SIZE == 10, "SIZE must be 10");
```

#### 错误 3：C11/C17 省略消息参数

```c
// C11/C17: 错误，消息参数必需
// static_assert(sizeof(int) == 4);  // 编译错误！

// 正确做法
static_assert(sizeof(int) == 4, "int must be 4 bytes");
```

**C23 修正：**
```c
// C23: 正确，消息可选
static_assert(sizeof(int) == 4);  // 有效
```

### 与运行时断言对比

```c
#include <assert.h>  // 同时提供 static_assert 和 assert

int process(int value)
{
    // 静态断言：编译时检查
    static_assert(sizeof(int) == 4, "int must be 4 bytes");

    // 运行时断言：运行时检查
    assert(value > 0);  // 如果 value <= 0，程序终止

    return value * 2;
}
```

| 特性 | static_assert | assert |
|------|---------------|--------|
| 检查时机 | 编译时 | 运行时 |
| 条件类型 | 整数常量表达式 | 任意表达式 |
| 运行时开销 | 无 | 有（检查） |
| 可禁用 | 否 | 可通过 NDEBUG 禁用 |

## 7. 总结

### 核心要点

`static_assert` 是 C 语言中强大的编译时诊断工具：

| 特性 | 说明 |
|------|------|
| **编译时检查** | 在编译阶段发现错误，零运行时开销 |
| **类型大小验证** | 常用于检查类型大小、结构体布局 |
| **平台适配** | 确保代码在特定平台上正确编译 |
| **契约验证** | 编译时验证常量条件 |

### 版本演进

| 版本 | 关键字 | 消息参数 | 头文件 |
|------|--------|----------|--------|
| C11 | `_Static_assert` | 必需 | `static_assert` 宏需 `<assert.h>` |
| C17 | `_Static_assert` | 必需 | `static_assert` 宏需 `<assert.h>` |
| C23 | `static_assert`（推荐）| 可选 | 无需头文件 |

### 使用建议

1. **优先使用 `static_assert`**：C23 起首选此形式，与 C++ 一致
2. **提供清晰的消息**：即使 C23 允许省略，也建议提供描述性消息
3. **检查关键假设**：类型大小、常量值、平台特性等
4. **放在合适位置**：头文件中、结构体定义后、函数开头
5. **区分静态与运行时**：编译时可确定用 `static_assert`，否则用 `assert`

### 相关概念

| 概念 | 关系 |
|------|------|
| `assert()` | 运行时断言，检查运行时条件 |
| `#error` | 预处理器指令，无条件产生编译错误 |
| `constexpr`（C23） | 定义编译时常量，可用于静态断言 |
| `sizeof` | 常与静态断言配合检查类型大小 |

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.7.11 Static assertions
- C17 标准 (ISO/IEC 9899:2018): 6.7.10 Static assertions, 7.2 Diagnostics
- C11 标准 (ISO/IEC 9899:2011): 6.7.10 Static assertions, 7.2 Diagnostics
- cppreference: https://en.cppreference.com/w/c/language/_Static_assert