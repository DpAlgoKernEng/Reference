# C++ 属性: noreturn (C++11 起)

## 1. 概述

`[[noreturn]]` 是 C++11 引入的函数属性（attribute），用于指示函数**不会将控制流返回给调用者**。该属性告知编译器：被标注的函数在执行完毕后不会返回到调用点，这对于编译器优化和静态分析具有重要价值。

典型的 `[[noreturn]]` 函数包括：
- 终止程序的函数（如 `std::exit()`, `std::abort()`）
- 抛出异常的函数
- 无限循环的函数

该属性定义在 C++ 标准库中，无需包含特定头文件，属于语言内置属性。

## 2. 来源与演变

### 首次引入

`[[noreturn]]` 属性在 **C++11** 标准中首次引入，作为新的属性语法的一部分。它取代了早期编译器厂商提供的非标准扩展，如 `__attribute__((noreturn))` (GCC/Clang) 或 `__declspec(noreturn)` (MSVC)。

### 历史背景

在 C++11 之前，开发者需要依赖编译器特定的扩展来标记不返回的函数：

```cpp
// GCC/Clang 扩展
__attribute__((noreturn)) void my_exit();

// MSVC 扩展
__declspec(noreturn) void my_exit();
```

这些非标准扩展导致代码可移植性问题。C++11 引入统一的 `[[noreturn]]` 属性语法，解决了跨平台兼容性问题。

### C++23 更新

C++23 标准引入了 `std::unreachable()` 函数，该函数同样声明为 `[[noreturn]]`，用于标记程序中不可到达的执行点。

### 缺陷报告

| 缺陷编号 | 适用标准 | 原发布行为 | 修正后行为 |
|---------|---------|-----------|-----------|
| CWG 2924 | C++11 | 从 `[[noreturn]]` 函数返回导致未定义行为 | 明确为**运行时**未定义行为 |

## 3. 语法与参数

### 基本语法

```cpp
[[noreturn]]
```

### 声明位置

`[[noreturn]]` 属性应用于**函数声明中的函数名**，而非函数类型。

```cpp
// ✅ 正确：属性应用于函数名
[[noreturn]] void f1();
void f2 [[noreturn]] ();

// ❌ 错误：属性应用于函数类型
void f3() [[noreturn]];  // 编译错误
```

### 声明规则

1. **首次声明必须指定**：如果任何声明指定了 `[[noreturn]]`，则该函数的**第一个声明**必须指定此属性
2. **翻译单元一致性**：如果在一个翻译单元中声明为 `[[noreturn]]`，在另一个翻译单元中声明时没有此属性，程序是病态的（ill-formed），且无需诊断

### 语法表格

| 声明形式 | 示例 | 说明 |
|---------|------|------|
| 函数声明前 | `[[noreturn]] void f();` | 推荐写法 |
| 函数名后 | `void f [[noreturn]] ();` | 合法但较少使用 |
| 函数类型后 | `void f() [[noreturn]];` | **错误**：属性应用于类型 |

## 4. 底层原理

### 编译器优化

`[[noreturn]]` 属性允许编译器进行重要的优化：

**1. 死代码消除**

编译器可以安全地删除函数调用后的不可达代码：

```cpp
[[noreturn]] void fatal_error();

void process(int x) {
    if (x < 0) {
        fatal_error();
        // 编译器知道此代码永远不会执行
        // 可以安全删除后续代码
    }
    // 继续处理...
}
```

**2. 控制流优化**

编译器无需为 `[[noreturn]]` 函数调用准备返回地址处理：

```cpp
[[noreturn]] void exit_program(int code);

int main() {
    exit_program(0);
    // 编译器无需生成 return 0 的代码
}
```

**3. 警告抑制**

编译器不会对 `[[noreturn]]` 函数缺少 `return` 语句发出警告。

### 运行时行为

如果被声明为 `[[noreturn]]` 的函数实际上返回了控制流，行为是**运行时未定义**（runtime-undefined behavior）。这意味着：

- 编译器不需要在运行时检查
- 程序可能崩溃、产生不可预测结果，或"正常"执行
- 开发者有责任确保函数确实不返回

### 与异常的关系

抛出异常的函数适合标记为 `[[noreturn]]`，因为异常会改变控制流，函数本身不会正常返回：

```cpp
[[noreturn]] void throw_error(const char* msg) {
    throw std::runtime_error(msg);
}
```

## 5. 使用场景

### 适合使用 `[[noreturn]]` 的场景

| 场景 | 示例函数 | 说明 |
|------|---------|------|
| 程序终止 | `std::exit()`, `std::abort()`, `std::_Exit()` | 终止整个程序执行 |
| 异常处理 | `std::terminate()`, `std::rethrow_exception()` | 异常相关的不返回函数 |
| 错误处理 | 自定义错误报告函数 | 总是抛出异常或终止 |
| 无限循环 | 事件循环、服务器主循环 | 永不退出的循环 |

### 标准库中的 `[[noreturn]]` 函数

#### 终止函数

| 函数 | 头文件 | 说明 |
|------|-------|------|
| `std::_Exit()` (C++11) | `<cstdlib>` | 正常终止，不执行清理 |
| `std::abort()` | `<cstdlib>` | 异常终止，不执行清理 |
| `std::exit()` | `<cstdlib>` | 正常终止，执行清理 |
| `std::quick_exit()` (C++11) | `<cstdlib>` | 快速退出，不完全清理 |
| `std::terminate()` | `<exception>` | 异常处理失败时调用 |
| `std::unexpected()` (已废弃) | `<exception>` | 违反动态异常规范时调用 |

#### 编译器提示

| 函数 | 头文件 | 说明 |
|------|-------|------|
| `std::unreachable()` (C++23) | `<utility>` | 标记不可到达的执行点 |

#### 总是抛出异常的函数

| 函数 | 头文件 | 说明 |
|------|-------|------|
| `std::rethrow_exception()` (C++11) | `<exception>` | 重新抛出异常 |
| `std::rethrow_nested()` | `<exception>` | 抛出嵌套异常 |
| `std::throw_with_nested()` (C++11) | `<exception>` | 抛出带嵌套的异常 |

#### 非局部跳转

| 函数 | 头文件 | 说明 |
|------|-------|------|
| `std::longjmp()` (C++17) | `<csetjmp>` | 跳转到指定位置 |

### 最佳实践

1. **仅在确实不返回时使用**：错误使用会导致未定义行为
2. **首次声明必须包含属性**：确保所有翻译单元中声明一致
3. **用于异常抛出函数**：提高代码可读性和编译器优化
4. **配合静态分析工具**：帮助检测逻辑错误

### 常见陷阱

| 陷阱 | 问题 | 解决方案 |
|------|------|---------|
| 条件返回 | 某些路径可能返回 | 确保所有路径都终止或抛出 |
| 跨翻译单元不一致 | 导致 ODR 违规 | 保持所有声明一致 |
| 应用于错误位置 | 编译错误 | 属性应用于函数名，非类型 |

## 6. 代码示例

### 基础用法

```cpp
#include <stdexcept>
#include <cstdlib>

// 函数总是抛出异常
[[noreturn]] void report_error(const char* message) {
    throw std::runtime_error(message);
}

// 函数总是终止程序
[[noreturn]] void fatal_exit(int code) {
    std::exit(code);
}

// 无限循环函数
[[noreturn]] void event_loop() {
    while (true) {
        // 处理事件...
    }
}

int main() {
    try {
        report_error("Something went wrong");
    } catch (const std::exception& e) {
        // 捕获异常
    }
    return 0;
}
```

### 高级用法

```cpp
#include <iostream>
#include <exception>
#include <string>

// 自定义异常类
class ApplicationError : public std::exception {
public:
    explicit ApplicationError(const std::string& msg)
        : message_(msg) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

// 总是失败的函数
[[noreturn]] void fail(const std::string& reason) {
    throw ApplicationError(reason);
}

// 断言失败处理
[[noreturn]] void assertion_failed(const char* expr, const char* file, int line) {
    std::cerr << "Assertion failed: " << expr
              << " at " << file << ":" << line << std::endl;
    std::abort();
}

// 自定义断言宏
#define MY_ASSERT(expr) \
    ((expr) ? (void)0 : assertion_failed(#expr, __FILE__, __LINE__))

// 使用示例
int divide(int a, int b) {
    if (b == 0) {
        fail("Division by zero");
    }
    return a / b;
}

int main() {
    try {
        divide(10, 0);  // 将抛出异常
    } catch (const ApplicationError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

### 常见错误及修正

#### 错误 1：错误地应用于函数类型

```cpp
// ❌ 错误：属性应用于函数类型，而非函数名
void f() [[noreturn]];  // 编译错误

// ✅ 修正：正确位置
[[noreturn]] void f();  // 正确
void g [[noreturn]] (); // 也正确
```

#### 错误 2：函数实际返回

```cpp
// ❌ 错误：标记为 noreturn 但可能返回
[[noreturn]] void process(int x) {
    if (x > 0) {
        throw std::runtime_error("positive");
    }
    // 如果 x <= 0，函数返回（未定义行为！）
}

// ✅ 修正：确保所有路径都不返回
[[noreturn]] void process(int x) {
    if (x > 0) {
        throw std::runtime_error("positive");
    } else {
        throw std::runtime_error("non-positive");
    }
}

// ✅ 或使用 std::unreachable (C++23)
[[noreturn]] void process(int x) {
    if (x > 0) {
        throw std::runtime_error("positive");
    }
    std::unreachable();  // 明确标记为不可达
}
```

#### 错误 3：跨翻译单元声明不一致

```cpp
// 文件 A.cpp
[[noreturn]] void fatal_error();  // 声明带 noreturn

// 文件 B.cpp
void fatal_error();  // ❌ 错误：声明不带 noreturn
                      // 程序病态，无需诊断

// ✅ 修正：使用头文件统一声明
// header.h
[[noreturn]] void fatal_error();
```

#### 错误 4：条件性 noreturn

```cpp
// ❌ 错误：有时返回，有时不返回
[[noreturn]] void conditional_exit(int code) {
    if (code != 0) {
        std::exit(code);
    }
    // code == 0 时返回，未定义行为！
}

// ✅ 修正：确保总是不返回
[[noreturn]] void conditional_exit(int code) {
    std::exit(code);  // exit 总是终止程序
}
```

## 注意事项

1. **运行时未定义行为**：从 `[[noreturn]]` 函数返回会导致运行时未定义行为，编译器不负责检测
2. **声明一致性**：所有翻译单元中的声明必须一致，否则违反 ODR（One Definition Rule）
3. **首次声明规则**：第一个声明必须包含该属性
4. **非终止性函数**：无限循环的函数也可以使用此属性
5. **C 兼容性**：C11 也引入了 `_Noreturn` 和 `[[noreturn]]`，C++ 和 C 的语法兼容

## 相关概念

| 概念 | 关系 |
|------|------|
| `std::terminate()` | 标准库中声明的 `[[noreturn]]` 函数 |
| `std::exit()` | 正常终止程序的 `[[noreturn]]` 函数 |
| `std::unreachable()` (C++23) | 标记不可达点的 `[[noreturn]]` 函数 |
| `throw` 表达式 | 改变控制流，常与 `[[noreturn]]` 函数配合 |
| `[[nodiscard]]` | 另一个 C++ 属性，表示返回值不应被忽略 |
| `[[maybe_unused]]` | 另一个 C++ 属性，抑制未使用警告 |

## 7. 总结

`[[noreturn]]` 属性是 C++11 引入的重要语言特性，用于标记不返回控制流的函数。它的核心价值包括：

**编译器优化**
- 死代码消除
- 控制流简化
- 警告抑制

**代码可读性**
- 明确表达函数意图
- 辅助静态分析工具

**使用建议**
1. 仅用于确实不返回的函数（终止、抛异常、无限循环）
2. 确保首次声明包含属性
3. 保持跨翻译单元声明一致
4. 结合 C++23 的 `std::unreachable()` 处理不可达代码

**关键限制**
- 误用会导致未定义行为
- 编译器不进行运行时检查
- 开发者负责正确性保证

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.12.10 Noreturn attribute [dcl.attr.noreturn]
- C++20 标准 (ISO/IEC 14882:2020): 9.12.9 Noreturn attribute [dcl.attr.noreturn]
- C++17 标准 (ISO/IEC 14882:2017): 10.6.8 Noreturn attribute [dcl.attr.noreturn]
- C++14 标准 (ISO/IEC 14882:2014): 7.6.3 Noreturn attribute [dcl.attr.noreturn]
- C++11 标准 (ISO/IEC 14882:2011): 7.6.3 Noreturn attribute [dcl.attr.noreturn]
- cppreference: https://en.cppreference.com/w/cpp/language/attributes/noreturn