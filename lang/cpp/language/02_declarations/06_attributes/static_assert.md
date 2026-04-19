# `static_assert` 声明 (C++11 起)

## 1. 概述

`static_assert` 是 C++11 引入的**编译期断言声明**（compile-time assertion declaration）。它用于在编译期间检查常量表达式的真假，如果条件为假，则编译失败并输出指定的错误信息。

与运行时断言（如 `assert` 宏）不同，`static_assert` 在编译阶段就能发现错误，能够提前捕获类型不匹配、模板参数约束违反等问题，是模板元编程和类型约束的重要工具。`static_assert` 是零运行时开销的编译期检查机制。

`static_assert` 声明可以出现在：
- 命名空间作用域（namespace scope）
- 块作用域（block scope），作为块声明
- 类体内（class body），作为成员声明

## 2. 来源与演变

### 首次引入（C++11）

`static_assert` 在 **C++11** 标准中首次引入，作为对编译期编程能力的重要补充。在此之前，C++ 开发者需要使用各种技巧来实现类似的编译期检查：

```cpp
// C++11 之前的技巧：利用数组大小必须为正的特性
#define STATIC_ASSERT(cond) typedef char static_assert_##__LINE__[(cond) ? 1 : -1]
```

这种旧方式存在问题：
- 错误信息不直观，难以定位问题
- 不同编译器的行为不一致
- 无法自定义错误消息
- 在模板代码中使用受限

`static_assert` 的引入解决了这些问题，提供了标准化的编译期断言机制。

### C++17 改进

C++17 简化了语法，允许省略错误信息字符串：

```cpp
// C++11: 必须提供消息
static_assert(sizeof(int) == 4, "int must be 4 bytes");

// C++17: 可省略消息
static_assert(sizeof(int) == 4);
```

### C++26 扩展

C++26 进一步扩展了能力，支持用户生成的错误消息：

```cpp
// C++26: 支持运行时生成消息（需要 constexpr 的 format）
static_assert(sizeof(int) == 4, std::format("Expected 4, got {}", sizeof(int)));
```

这解决了之前错误消息不能包含动态信息（如模板参数名称）的限制。

### 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|-----------|-----|------|------|
| `__cpp_static_assert` | `200410L` | C++11 | static_assert 基本语法（语法 1） |
| `__cpp_static_assert` | `201411L` | C++17 | 单参数 static_assert（语法 2） |
| `__cpp_static_assert` | `202306L` | C++26 | 用户生成的错误消息（语法 3） |

### 缺陷报告

| 缺陷编号 | 适用版本 | 原行为 | 修正行为 |
|---------|---------|--------|---------|
| CWG 2039 | C++11 | 仅要求转换前的表达式是常量 | 转换本身也必须是有效的常量表达式 |
| CWG 2518 (P2593R1) | C++11 | 未实例化的 `static_assert(false, "");` 是非良构的 | 改为良构（well-formed），延迟到实例化时检查 |

## 3. 语法与参数

### 语法形式

```cpp
// (1) 带固定错误消息（C++11 起）
static_assert(bool-constexpr, unevaluated-string)

// (2) 无错误消息（C++17 起）
static_assert(bool-constexpr)

// (3) 带用户生成错误消息（C++26 起）
static_assert(bool-constexpr, constant-expression)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `bool-constexpr` | 布尔常量表达式。C++23 前：需要是上下文转换为 `bool` 的常量表达式，不允许内置转换（除了到 `bool` 的非窄化整型转换）；C++23 起：转换为 `bool` 的表达式，该转换为常量表达式 |
| `unevaluated-string` | 未求值的字符串字面量，将作为错误消息显示 |
| `constant-expression` | 满足以下条件的常量表达式 `msg`：`msg.size()` 可隐式转换为 `std::size_t`；`msg.data()` 可隐式转换为 `const char*` |

### 行为说明

如果 `bool-constexpr` 满足以下条件之一，则此声明无效果：
- 表达式是良构的且求值为 `true`
- 表达式在模板定义上下文中求值，且模板未被实例化

否则，编译器产生编译错误，并包含用户提供的消息（如果有）。

### 错误消息确定规则

消息文本按以下规则确定：

1. 如果消息符合 `unevaluated-string` 的语法要求，消息文本即为该字符串字面量的内容

2. （C++26 起）否则，给定：
   - `msg` 表示 `constant-expression` 的值
   - `len` 表示 `msg.size()` 的值（必须可转换为 `std::size_t`）
   - `ptr` 表示隐式转换为 `const char*` 的 `msg.data()`（必须是核心常量表达式）
   - 消息文本为由 `ptr` 开始、长度为 `len` 的普通字面量编码字符序列

### C++26 前的限制

C++26 之前，错误消息必须是字符串字面量，不能包含：
- 动态信息
- 非字面量的常量表达式
- 模板类型参数名称

## 4. 底层原理

### 编译期检查机制

`static_assert` 的检查完全在编译期进行：

```
编译阶段流程：
源代码 → 预处理 → 词法分析 → 语法分析 → 语义分析 → [static_assert 检查] → 代码生成
```

当编译器遇到 `static_assert` 声明时：
1. 解析并求值 `bool-constexpr` 表达式
2. 如果表达式不是常量表达式，产生编译错误
3. 如果表达式求值为 `false`，产生编译错误并显示消息
4. 如果表达式求值为 `true`，继续编译，无额外开销

### 模板实例化延迟

对于模板中的 `static_assert`，检查发生在模板实例化时：

```cpp
template<class T>
struct container {
    static_assert(std::is_default_constructible_v<T>,
                  "T must be default constructible");
};

// 模板定义时 static_assert 不被检查
// 只有在 container<int> 等实例化时才检查
```

这是 CWG 2518 确认的行为：在模板定义上下文中，如果模板未被实例化，`static_assert` 无效果。

### 零运行时开销

`static_assert` 是纯编译期构造：
- 不生成任何机器代码
- 不占用运行时内存
- 不影响程序性能
- 失败时阻止编译成功

### 与其他断言机制的对比

| 特性 | `static_assert` | `assert` (C 宏) | `#error` (预处理指令) |
|------|-----------------|-----------------|----------------------|
| 检查时机 | 编译期 | 运行时 | 预处理期 |
| 条件类型 | 常量表达式 | 任意表达式 | 无条件 |
| 运行时开销 | 无 | 有（调试构建） | 无 |
| 可用信息 | 编译期信息 | 运行期信息 | 无 |
| 可禁用 | 否 | 可通过 NDEBUG | 否 |
| 模板支持 | 支持 | 不适用 | 不适用 |

## 5. 使用场景

### 适用场景

| 场景 | 示例 |
|------|------|
| 模板参数约束 | `static_assert(std::is_integral_v<T>, "T must be integral")` |
| 类型特征检查 | `static_assert(std::is_copy_constructible_v<T>)` |
| 平台相关常量验证 | `static_assert(sizeof(void*) == 8, "64-bit only")` |
| 编译器特性检查 | `static_assert(__cplusplus >= 201703L, "C++17 required")` |
| 结构体布局验证 | `static_assert(offsetof(S, member) == 8)` |
| 对齐要求验证 | `static_assert(alignof(T) >= 16)` |

### 最佳实践

1. **提供有意义的错误消息**：错误消息应清晰说明问题所在

```cpp
// 差：消息不明确
static_assert(sizeof(int) == 4);

// 好：消息清晰
static_assert(sizeof(int) == 4, "This library requires 32-bit int");
```

2. **在模板代码中验证约束**：提供更好的错误诊断

```cpp
template<typename T>
class Container {
    static_assert(std::is_default_constructible_v<T>,
                  "Container requires T to be default constructible");
    // ...
};
```

3. **结合类型特征使用**：确保类型满足要求

```cpp
template<typename T>
void process(T value) {
    static_assert(std::is_arithmetic_v<T>,
                  "process() only works with arithmetic types");
    // ...
}
```

### 常见陷阱

#### 陷阱 1：在未实例化模板中使用 `false`

```cpp
// C++11-C++20 的问题行为
template<class T>
struct bad_type {
    static_assert(false, "Always fails");  // 即使未实例化也失败！
};

// 修正方案（C++11-C++20）
template<class>
constexpr bool dependent_false = false;  // 始终为 false，但依赖模板参数

template<class T>
struct bad_type {
    static_assert(dependent_false<T>, "Error on instantiation");
};

// C++26 直接支持
template<class T>
struct bad_type {
    static_assert(false, "Error on instantiation");  // CWG 2518 后合法
};
```

#### 陷阱 2：消息不能包含动态信息（C++26 前）

```cpp
// C++26 前：错误
template<int N>
void foo() {
    static_assert(N > 0, "N must be positive, got: " + std::to_string(N));  // 错误
}

// C++26 起：可以
template<int N>
void foo() {
    static_assert(N > 0, std::format("N must be positive, got: {}", N));
}
```

#### 陷阱 3：误用运行时值

```cpp
void foo(int x) {
    static_assert(x > 0, "x must be positive");  // 错误：x 不是常量表达式
}

// 正确做法：使用运行时检查
void foo(int x) {
    assert(x > 0);  // 运行时检查
    // 或
    if (x <= 0) throw std::invalid_argument("x must be positive");
}
```

## 6. 代码示例

### 基础用法

```cpp
#include <type_traits>

// 验证基本常量
static_assert(03301 == 1729);  // C++17 起，消息可选

// 验证类型大小
static_assert(sizeof(int) == 4, "int must be 4 bytes on this platform");
static_assert(sizeof(void*) == 8, "This code requires 64-bit platform");

// 验证对齐要求
struct alignas(16) AlignedStruct {
    int x;
};
static_assert(alignof(AlignedStruct) == 16, "Alignment requirement not met");

int main() {
    // 函数体内的 static_assert
    static_assert(sizeof(long) >= sizeof(int),
                  "long must be at least as large as int");
    return 0;
}
```

### 模板约束验证

```cpp
#include <type_traits>

template<class T>
void swap(T& a, T& b) noexcept {
    static_assert(std::is_copy_constructible_v<T>,
                  "Swap requires copying");
    static_assert(std::is_nothrow_copy_constructible_v<T> &&
                  std::is_nothrow_copy_assignable_v<T>,
                  "Swap requires nothrow copy/assign");
    auto c = b;
    b = a;
    a = c;
}

template<class T>
struct data_structure {
    static_assert(std::is_default_constructible_v<T>,
                  "Data structure requires default-constructible elements");
};

struct no_copy {
    no_copy(const no_copy&) = delete;
    no_copy() = default;
};

struct no_default {
    no_default() = delete;
};

int main() {
    int a = 1, b = 2;
    swap(a, b);  // OK

    no_copy nc_a, nc_b;
    // swap(nc_a, nc_b);  // 编译错误：Swap requires copying

    data_structure<int> ds_ok;  // OK
    // data_structure<no_default> ds_error;  // 编译错误
}
```

### 模板中的依赖型断言

```cpp
#include <type_traits>

// 解决 CWG 2518 之前的问题
template<class>
constexpr bool dependent_false = false;  // 始终为 false，但依赖模板参数

template<class T>
struct always_fail {
    static_assert(dependent_false<T>, "This template should never be instantiated");
    // C++26 / CWG 2518 后也可以直接写：
    // static_assert(false, "This template should never be instantiated");
};

// 使用 if constexpr 实现条件编译
template<typename T>
void process(T value) {
    if constexpr (std::is_integral_v<T>) {
        // 整数类型的实现
    } else if constexpr (std::is_floating_point_v<T>) {
        // 浮点类型的实现
    } else {
        static_assert(dependent_false<T>, "Unsupported type");
    }
}
```

### 平台与架构检测

```cpp
#include <cstddef>
#include <cstdint>

// 操作系统检查
#if defined(_WIN32) || defined(_WIN64)
    static_assert(sizeof(void*) == 4 || sizeof(void*) == 8,
                  "Windows must be 32-bit or 64-bit");
#endif

// 字节序检查（C++20 起）
#if __cplusplus >= 202002L
    #include <bit>
    static_assert(std::endian::native == std::endian::little ||
                  std::endian::native == std::endian::big,
                  "Mixed endian not supported");
#endif

// 整数类型大小验证
static_assert(sizeof(int32_t) == 4, "int32_t must be exactly 4 bytes");
static_assert(sizeof(int64_t) == 8, "int64_t must be exactly 8 bytes");

// 结构体布局验证
struct PacketHeader {
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    uint32_t length;
};

static_assert(sizeof(PacketHeader) == 12,
              "PacketHeader must be 12 bytes for wire compatibility");
static_assert(offsetof(PacketHeader, length) == 8,
              "length field must be at offset 8");
```

### C++26 用户生成消息

```cpp
#include <format>
#include <type_traits>

#if __cpp_static_assert >= 202306L
// C++26: 用户生成的错误消息（需要 constexpr format）
template<int Size>
void fixed_size_buffer() {
    static_assert(Size > 0 && Size <= 1024,
                  std::format("Size must be in range [1, 1024], got {}", Size));
}

template<typename T>
void process_numeric() {
    static_assert(std::is_integral_v<T>,
                  std::format("Expected integral type, got something else"));
}
#endif
```

### 常见错误及修正

#### 错误 1：在静态断言中使用运行时值

```cpp
// 错误：运行时值不能用于 static_assert
void process(int size) {
    static_assert(size > 0, "Size must be positive");  // 编译错误！
}

// 修正：使用运行时检查
#include <cassert>
void process(int size) {
    assert(size > 0);  // 或使用 if 判断抛出异常
}
```

#### 错误 2：模板中的过早失败

```cpp
// 错误：模板未实例化时就失败（C++11-C++20）
template<typename T>
void foo() {
    static_assert(false, "Not implemented");  // 总是失败！
}

// 修正方案 1：使用依赖类型的条件
template<typename T>
constexpr bool always_false = false;

template<typename T>
void foo() {
    static_assert(always_false<T>, "Not implemented for this type");
}

// 修正方案 2：使用 if constexpr (C++17)
template<typename T>
void foo() {
    if constexpr (std::is_integral_v<T>) {
        // 整数类型的实现
    } else {
        static_assert(always_false<T>, "Not implemented for non-integral types");
    }
}
```

#### 错误 3：忘记包含必要的头文件

```cpp
// 错误：未包含 <type_traits>
template<typename T>
void foo() {
    static_assert(std::is_integral_v<T>, "T must be integral");  // 可能找不到定义
}

// 修正：包含必要头文件
#include <type_traits>

template<typename T>
void foo() {
    static_assert(std::is_integral_v<T>, "T must be integral");
}
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 检查时机 | 编译期 |
| 运行时开销 | 无 |
| 可用位置 | 命名空间、类体、函数体 |
| 消息要求 | C++11 必须提供，C++17 可选，C++26 支持动态生成 |
| 失败后果 | 编译错误 |

### 版本差异速查

| C++ 版本 | 语法 | 特性 |
|---------|------|------|
| C++11 | `static_assert(cond, "msg")` | 基本语法，必须提供错误消息 |
| C++17 | `static_assert(cond)` | 错误消息可选 |
| C++26 | `static_assert(cond, expr)` | 支持用户生成消息 |

### 使用建议

1. **优先使用 `static_assert` 进行编译期可验证的约束**：零运行时开销
2. **提供清晰的错误消息**：帮助开发者快速定位问题
3. **在模板代码中验证约束**：提供更好的错误诊断
4. **注意 C++ 版本差异**：不同版本支持的语法不同
5. **避免在模板中使用非依赖的 `false`**（C++26 前的问题）

### 相关概念对比

| 机制 | 检查时机 | 用途 |
|------|---------|------|
| `static_assert` | 编译期 | 常量表达式验证 |
| `assert` | 运行时 | 调试期条件检查 |
| `#error` | 预处理期 | 无条件显示错误 |
| `#if ... #error` | 预处理期 | 预处理条件检查 |
| `std::enable_if` (C++11) | 编译期 | SFINAE 和函数重载约束 |
| Concepts (C++20) | 编译期 | 更强大的模板约束机制 |

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.1 Preamble [dcl.pre]
- C++20 标准 (ISO/IEC 14882:2020): 9.1 Preamble [dcl.pre]
- C++17 标准 (ISO/IEC 14882:2017): 10 Declarations [dcl.dcl]
- C++14 标准 (ISO/IEC 14882:2014): 7 Declarations [dcl.dcl]
- C++11 标准 (ISO/IEC 14882:2011): 7 Declarations [dcl.dcl]
- cppreference: https://en.cppreference.com/w/cpp/language/static_assert