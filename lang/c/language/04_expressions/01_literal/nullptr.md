# nullptr - 预定义空指针常量（C23 起）

## 1. 概述 (Overview)

`nullptr` 是 C23 标准引入的关键字，表示一个预定义的空指针常量（null pointer constant）。它是 `nullptr_t` 类型的非左值（non-lvalue），可以隐式转换为任意指针类型或 `bool` 类型，转换结果分别是对应类型的空指针值或 `false`。

### 核心特性

- **类型安全**：`nullptr` 具有独立的类型 `nullptr_t`，区别于整数类型
- **统一表示**：提供统一的空指针表示方式，消除了 `NULL` 和 `0` 的歧义性
- **隐式转换**：可自动转换为任意指针类型，转换结果为该类型的空指针值

### 技术定位

`nullptr` 是 C 语言现代化的标志性特性之一，旨在解决传统空指针常量（`NULL`、`0`）存在的类型安全问题，与 C++11 引入的 `nullptr` 保持一致。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C23 之前，C 语言中空指针常量的表示方式存在以下问题：

**传统表示方式的缺陷**：

| 表示方式 | 问题 |
|---------|------|
| `0` | 整数常量零，在某些上下文中会产生歧义（如重载决议） |
| `NULL` | 实现定义的宏，可能是 `0`、`((void*)0)` 或其他形式，行为不一致 |

**典型问题示例**：

```c
// 问题 1：类型推断歧义
auto p = NULL;  // NULL 的类型是什么？取决于实现

// 问题 2：泛型选择困难
_Generic(x,
    int: ...,
    void*: ...,
    default: ...
)
// 如果 NULL 是整数 0，会匹配 int；如果是 (void*)0，会匹配 void*
```

### 设计动机

C23 引入 `nullptr` 的主要目的：

1. **类型安全**：提供具有独立类型的空指针常量，避免整数与指针混淆
2. **与 C++ 兼容**：与 C++11 的 `nullptr` 保持一致，便于代码迁移
3. **消除歧义**：在泛型选择（Generic Selection）等场景中提供明确的类型匹配

### 版本变更

| 标准 | 空指针常量表示 |
|------|--------------|
| C89/C99/C11 | `0`、`NULL`（实现定义） |
| C23 | 新增 `nullptr` 关键字，`NULL` 仍然保留 |

### 标准参考

C23 标准（ISO/IEC 9899:2024）第 6.4.4.6 节定义了预定义常量 `nullptr`。

## 3. 语法与参数 (Syntax and Parameters)

### 语法

```
nullptr
```

`nullptr` 是一个关键字，无需包含任何头文件即可使用。

### 类型说明

| 属性 | 说明 |
|------|------|
| 类型 | `nullptr_t`（定义于 `<stddef.h>`） |
| 值类别 | 非左值（non-lvalue） |
| 可转换类型 | 任意指针类型、`bool` 类型 |

### 隐式转换规则

```c
nullptr  → 任意指针类型 T*   结果：空指针值（null pointer value）
nullptr  → bool             结果：false
```

### 相关类型与宏

| 名称 | 说明 |
|------|------|
| `nullptr_t` | `nullptr` 的类型，定义于 `<stddef.h>`（C23 起） |
| `NULL` | 实现定义的空指针常量宏（传统兼容） |

## 4. 底层原理 (Underlying Principles)

### 类型系统设计

`nullptr` 的核心创新在于引入了独立类型 `nullptr_t`：

```
nullptr_t 类型特性：
├── 可转换为任意指针类型
├── 可转换为 bool 类型
├── 不可转换为整数类型（与 0/NULL 的关键区别）
└── 不是左值，不能取地址
```

### 与传统空指针常量的对比

| 特性 | `nullptr` | `NULL` | `0` |
|------|-----------|--------|-----|
| 类型 | `nullptr_t` | 实现定义 | `int` |
| 类型安全 | 是 | 部分 | 否 |
| 泛型选择行为 | 明确匹配 `nullptr_t` | 实现定义 | 匹配 `int` |
| 可移植性 | 完全一致 | 依赖实现 | 完全一致 |
| auto 类型推导 | `nullptr_t` | 实现定义 | `int` |

### 类型检测机制

通过 `_Generic` 可以区分不同的空指针常量：

```c
#define DETECT_NULL_POINTER_CONSTANT(e) \
    _Generic(e,                         \
        void*: "void*",                 \
        nullptr_t: "nullptr_t",         \
        default: "integer"              \
    )

// nullptr      → "nullptr_t"
// ((void*)0)   → "void*"
// 0            → "integer"
// NULL         → 实现定义（可能是以上任一种）
```

### auto 类型推导行为

```c
auto p1 = nullptr;  // 类型：nullptr_t
auto p2 = NULL;     // 类型：实现定义（可能是 int 或 void*）
auto p3 = 0;        // 类型：int

// p1 可以安全地传递给任何指针参数
// p2、p3 的行为取决于实现，可能不可移植
```

## 5. 使用场景 (Use Cases)

### 适用场景

**场景一：函数参数传递**

```c
void process(int* ptr);

// 推荐用法
process(nullptr);  // 明确传递空指针

// 传统方式（仍然有效但不够明确）
process(NULL);
process(0);
```

**场景二：泛型编程**

```c
#define handle_pointer(e) \
    _Generic(e,           \
        nullptr_t: handle_null(), \
        int*: handle_int(e),       \
        char*: handle_char(e),     \
        default: handle_other(e)   \
    )

// nullptr 会明确匹配 nullptr_t 分支
handle_pointer(nullptr);
```

**场景三：auto 类型推导**

```c
auto ptr = nullptr;  // ptr 类型为 nullptr_t，可安全用于任何指针上下文

// 与 NULL 对比
auto old_ptr = NULL;  // 类型不确定，移植性差
```

### 最佳实践

1. **优先使用 nullptr**：在 C23 及以后版本中，优先使用 `nullptr` 表示空指针
2. **保持一致性**：同一项目中统一使用 `nullptr`
3. **结合类型检查**：利用 `nullptr_t` 进行编译期类型检查

### 注意事项

1. **版本兼容性**：`nullptr` 仅在 C23 及以后版本可用，旧编译器不支持
2. **与 NULL 共存**：`NULL` 宏仍然保留用于向后兼容，但新代码应使用 `nullptr`
3. **auto 复制行为**：`nullptr` 的副本仍可作为空指针常量使用

### 常见陷阱

**陷阱一：混用 NULL 和 nullptr 导致类型不一致**

```c
// 错误示范
auto p1 = nullptr;
auto p2 = NULL;

// p1 和 p2 的类型可能不同，在泛型代码中行为不一致
_Generic(p1, nullptr_t: A, default: B)  // 匹配 A
_Generic(p2, nullptr_t: A, default: B)  // 可能匹配 B（取决于实现）
```

**陷阱二：期望 nullptr 转换为整数**

```c
// 错误：nullptr 不能隐式转换为整数
int x = nullptr;  // 编译错误

// 正确：先转换为指针，再判断
void* p = nullptr;
if (!p) { /* ... */ }  // 正确

// 或者直接判断
if (nullptr == (void*)nullptr) { /* ... */ }  // 总是 true
```

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stddef.h>
#include <stdio.h>

void g(int* ptr)
{
    if (ptr == nullptr) {
        puts("Received null pointer");
    } else {
        puts("Received valid pointer");
    }
}

int main(void)
{
    g(nullptr);  // 明确传递空指针
    g(NULL);     // 传统方式（兼容）
    g(0);        // 传统方式（兼容）

    return 0;
}
```

### 进阶示例：类型检测

```c
#include <stddef.h>
#include <stdio.h>

#define DETECT_NULL_POINTER_CONSTANT(e) \
    _Generic(e,                         \
        void*: puts("void*"),           \
        nullptr_t: puts("nullptr_t"),   \
        default: puts("integer")         \
    )

int main(void)
{
    DETECT_NULL_POINTER_CONSTANT(((void*)0));  // 输出: void*
    DETECT_NULL_POINTER_CONSTANT(0);            // 输出: integer
    DETECT_NULL_POINTER_CONSTANT(nullptr);      // 输出: nullptr_t
    // NULL 的输出取决于实现
    DETECT_NULL_POINTER_CONSTANT(NULL);

    return 0;
}
```

### 完整示例：auto 与 nullptr 的交互

```c
#include <stddef.h>
#include <stdio.h>

void g(int* ptr)
{
    puts("Function g called");
}

int main(void)
{
    // 方式 1：直接使用 nullptr
    g(nullptr);  // OK

    // 方式 2：使用 NULL（传统方式）
    g(NULL);     // OK

    // 方式 3：使用 0（传统方式）
    g(0);        // OK

    // 方式 4：复制 nullptr
    auto cloned_nullptr = nullptr;
    g(cloned_nullptr);  // OK，类型为 nullptr_t，可转换为 int*

    // 方式 5：复制 NULL（行为实现定义）
    [[maybe_unused]] auto cloned_NULL = NULL;
    // g(cloned_NULL);  // 实现定义：可能 OK，取决于 NULL 的定义

    // 方式 6：复制 0（错误用法）
    [[maybe_unused]] auto cloned_zero = 0;
    // g(cloned_zero);  // 错误：int 不能隐式转换为 int*

    return 0;
}
```

**输出示例**：

```
Function g called
Function g called
Function g called
Function g called
```

### 常见错误及修正

**错误示例 1：将 nullptr 赋值给整数类型**

```c
// 错误
int x = nullptr;  // 编译错误：不能将 nullptr_t 转换为 int

// 正确
int* ptr = nullptr;  // OK
if (ptr == nullptr) { /* ... */ }  // OK
```

**错误示例 2：在 C23 之前的版本使用**

```c
// C23 之前版本
int* p = nullptr;  // 编译错误：未识别的标识符

// C23 之前的替代方案
int* p = NULL;     // 正确
int* p = 0;        // 正确
```

**错误示例 3：对 nullptr 取地址**

```c
// 错误
void* addr = &nullptr;  // 编译错误：nullptr 不是左值

// 正确（如果需要指针变量）
nullptr_t np = nullptr;
void* addr = &np;  // OK
```

## 7. 总结 (Summary)

### 核心要点

| 要点 | 说明 |
|------|------|
| 类型安全 | `nullptr` 具有 `nullptr_t` 类型，区别于整数类型 |
| 统一表示 | 提供跨平台一致的空指针常量表示 |
| 向后兼容 | `NULL` 宏仍然保留，可与传统代码共存 |
| C++ 兼容 | 与 C++11 的 `nullptr` 语义一致 |

### 技术对比

| 特性 | `nullptr` (C23) | `NULL` (传统) | `0` (传统) |
|------|----------------|---------------|-----------|
| 类型 | `nullptr_t` | 实现定义 | `int` |
| 类型安全 | 高 | 中 | 低 |
| 泛型选择 | 明确匹配 `nullptr_t` | 实现定义 | 匹配 `int` |
| auto 推导 | `nullptr_t` | 实现定义 | `int` |
| 可移植性 | 高 | 中 | 高 |
| 版本要求 | C23+ | 所有版本 | 所有版本 |

### 学习建议

1. **理解类型系统**：掌握 `nullptr_t` 类型与指针类型的转换规则
2. **熟悉演进历史**：了解 `0` → `NULL` → `nullptr` 的演进动机
3. **实践迁移**：在新项目中优先使用 `nullptr`，逐步替换旧代码中的 `NULL`
4. **注意兼容性**：针对需要支持 C23 之前标准的代码，使用条件编译或继续使用 `NULL`

### 相关主题

- `NULL` - 实现定义的空指针常量宏
- `nullptr_t` - `nullptr` 的类型定义（C23 起）
- 指针类型 - C 语言指针系统
- `_Generic` - 泛型选择（C11 起）

---

**参考资料**：
- C23 标准（ISO/IEC 9899:2024）第 6.4.4.6 节
- cppreference: [nullptr](https://en.cppreference.com/w/c/language/nullptr)