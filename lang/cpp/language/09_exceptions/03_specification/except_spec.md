# 动态异常规范 (Dynamic Exception Specification)

## 1. 概述 (Overview)

**动态异常规范**（Dynamic Exception Specification）是 C++ 早期版本中用于声明函数可能抛出异常类型的机制。它允许程序员在函数声明中显式列出函数可能抛出的异常类型，帮助编译器和程序员理解函数的异常行为。

动态异常规范使用 `throw()` 语法，在 C++11 中被弃用，并在 C++17 中被完全移除。现代 C++ 应使用 `noexcept` 说明符替代。

### 核心概念

- **异常规范**：声明函数可以抛出哪些类型的异常
- **潜在异常集合**（Set of Potential Exceptions）：函数可能抛出的所有异常类型的集合
- **意外异常处理**：当函数抛出未列出的异常时，调用 `std::unexpected()`

## 2. 来源与演变 (Origin and Evolution)

### 首次引入

动态异常规范首次在 **C++98** 标准中引入，设计初衷是提供一种类似 Java 的 checked exception 机制，让编译器能够验证异常处理的完整性。

### 历史背景

早期 C++ 设计者希望异常规范能够：
- 提供文档化功能，明确函数可能抛出的异常
- 允许编译器优化代码（假设不抛出异常）
- 提供运行时检查，确保只抛出声明的异常类型

### 废弃原因

动态异常规范在实践中暴露出多个严重问题：

| 问题 | 说明 |
|------|------|
| **运行时检查** | 异常检查发生在运行时，而非编译时，增加了运行开销 |
| **模板兼容性差** | 模板代码难以确定所有可能的异常类型 |
| **代码膨胀** | 需要生成额外的异常处理代码 |
| **优化障碍** | 实际上反而阻碍了编译器优化 |
| **API 脆弱性** | 修改异常规范会破坏现有代码 |

### 版本演变

| 版本 | 状态 |
|------|------|
| **C++98** | 正式引入动态异常规范 `throw(type-list)` |
| **C++11** | 弃用动态异常规范，引入 `noexcept` 说明符 |
| **C++17** | 完全移除动态异常规范，`throw()` 等价于 `noexcept(true)` |
| **C++20 及以后** | `throw()` 作为 `noexcept` 的别名被保留 |

### 替代方案

C++11 引入的 `noexcept` 说明符解决了动态异常规范的问题：

```cpp
// C++98 风格（已移除）
void f() throw(int, std::runtime_error);

// C++11 及以后推荐
void f() noexcept(false);           // 可能抛出异常
void g() noexcept;                  // 不抛出异常
void h() noexcept(true);            // 不抛出异常（同上）
```

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```
throw( type-id-list(optional) )
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `type-id-list` | 逗号分隔的类型标识符列表，表示函数可能抛出的异常类型 |
| （空列表）`throw()` | 表示函数不抛出任何异常 |
| （无参数）`throw` | 不允许，必须包含括号 |

### 类型限制

异常规范中不允许以下类型：
- 不完整类型（Incomplete types）
- 指向不完整类型的指针或引用（`cv void*` 除外）
- 右值引用类型（C++11 起）
- 数组类型会被调整为对应的指针类型
- 顶层 cv 限定符会被丢弃

### 使用位置限制

动态异常规范只能出现在以下位置：

1. 函数声明的顶层类型
2. 函数指针类型声明
3. 函数引用类型声明
4. 成员函数指针类型声明
5. 函数声明中的参数或返回类型

### 正确与错误示例

```cpp
// 正确用法
void f() throw(int);                    // 函数声明
void (*pf)() throw(int);                // 函数指针声明
void g(void pfa() throw(int));          // 函数参数中的函数指针
int (S::*pmf)() throw(int);             // 成员函数指针

// 错误用法
typedef int (*pf)() throw(int);         // 错误：typedef 声明中不允许
```

### 潜在异常集合规则

每个函数都有对应的**潜在异常集合**，定义如下：

| 函数声明 | 潜在异常集合 |
|---------|-------------|
| 使用动态异常规范 `throw(T1, T2, ...)` | `{T1, T2, ...}` |
| 使用 `noexcept(true)`（C++11 起） | 空集合 `{}` |
| 无异常规范 | 所有可能的类型（全集） |

## 4. 底层原理 (Underlying Principles)

### 运行时检查机制

当函数抛出异常时：

1. 运行时系统检查异常类型是否在异常规范列表中
2. 如果匹配成功，正常进行栈展开（stack unwinding）
3. 如果匹配失败，调用 `std::unexpected()`

### std::unexpected() 处理流程

```
函数抛出异常
    │
    ▼
异常类型在规范中？
    │
    ├─ 是 ──► 正常栈展开
    │
    └─ 否 ──► 调用 std::unexpected()
                    │
                    ▼
              用户是否设置了自定义处理函数？
                    │
                    ├─ 否 ──► std::terminate()
                    │
                    └─ 是 ──► 执行用户处理函数
                                │
                                ▼
                         抛出的异常是否被接受？
                                │
                                ├─ 是 ──► 正常栈展开
                                │
                                └─ 否 ──► 异常规范是否允许 std::bad_exception？
                                            │
                                            ├─ 是 ──► 抛出 std::bad_exception
                                            │
                                            └─ 否 ──► std::terminate()
```

### 模板实例化规则

函数模板的动态异常规范**不会**随函数声明一起实例化，只有在**需要时**才实例化：

```cpp
template<class T>
T f() throw(std::array<char, sizeof(T)>);

int main() {
    decltype(f<void>()) *p;  // 错误！
    // 即使 f<void>() 未被求值，异常规范也需要实例化
    // 实例化时计算 sizeof(void) 是非法的
}
```

**需要实例化异常规范的场景**：
- 函数被重载决议选中
- 函数被 ODR 使用
- 函数出现在未求值操作数中但仍需要异常规范
- 与另一个函数声明进行比较（如虚函数重写）
- 函数定义中
- 默认特殊成员函数需要检查基类的异常规范

### 隐式声明成员函数的异常规范

编译器为隐式声明的特殊成员函数自动推导异常规范：

```cpp
struct A {
    A(int = (A(5), 0)) noexcept;
    A(const A&) throw();
    A(A&&) throw();
    ~A() throw(X);
};

struct B {
    B() throw();
    B(const B&) = default;              // 异常规范为 noexcept(true)
    B(B&&, int = (throw Y(), 0)) noexcept;
    ~B() throw(Y);
};

struct D : public A, public B {
    // 编译器隐式生成的成员函数：
    // D::D() throw(X, std::bad_array_new_length);
    // D::D(const D&) noexcept(true);
    // D::D(D&&) throw(Y);
    // D::~D() throw(X, Y);
};
```

推导规则：
- 如果潜在异常集合是全集，则异常规范允许所有异常
- 如果潜在异常集合非空，则列出所有类型
- 如果潜在异常集合为空，则异常规范为 `throw()`（C++11 前）或 `noexcept(true)`（C++11 起）

### 表达式的潜在异常集合

表达式 `e` 的潜在异常集合计算规则：

| 表达式类型 | 潜在异常集合 |
|-----------|-------------|
| 核心常量表达式 | 空集合 |
| 函数调用表达式 | 被调用函数的潜在异常集合 |
| `throw` 表达式 | 被抛出的异常类型 |
| `dynamic_cast` 到多态类型引用 | `{std::bad_cast}` |
| `typeid` 解引用多态类型指针 | `{std::bad_typeid}` |
| `new[]` 非常量大小的数组（C++11） | `{std::bad_array_new_length}` |

## 5. 使用场景 (Use Cases)

### 适用场景（历史背景）

在 C++17 之前，动态异常规范用于：

| 场景 | 说明 |
|------|------|
| 文档化异常行为 | 明确函数可能抛出的异常类型 |
| 不抛出保证 | 使用 `throw()` 声明函数不抛出异常 |
| 接口契约 | 定义函数的异常契约 |

### 现代替代方案

| 旧语法 | 现代替代 |
|--------|---------|
| `void f() throw()` | `void f() noexcept` |
| `void f() throw(int, double)` | `void f()` + 文档注释 |
| `void f() throw(...)` | 不允许，用 `void f() noexcept(false)` |

### 最佳实践（现代 C++）

```cpp
// 不推荐：已移除的语法
// void oldFunction() throw(int, std::runtime_error);

// 推荐：使用 noexcept
void newFunction() noexcept;              // 不抛出异常
void mayThrow() noexcept(false);          // 可能抛出异常

// 推荐：使用文档注释说明可能抛出的异常
/// @throws std::invalid_argument 如果参数无效
/// @throws std::runtime_error 如果运行时出错
void documentedFunction(int value);
```

### 注意事项

1. **不要在 C++17 及以后使用动态异常规范**：会导致编译错误
2. **C++11~C++14 过渡期**：可以使用但会产生警告
3. **空异常规范 `throw()`**：在 C++17 中等价于 `noexcept(true)`
4. **异常安全保证**：应使用 RAII 和 `noexcept` 实现异常安全

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 运行时开销 | 每次抛出异常都要检查类型匹配 |
| 隐式转换 | 派生类异常可以匹配基类类型 |
| 虚函数重写 | 派生类的异常规范必须至少与基类一样严格 |

## 6. 代码示例 (Examples)

### 基础用法（C++98 风格）

以下代码需要在 C++98 或 C++11 模式下编译，C++17 及以后将无法编译。

```cpp
#include <cstdlib>
#include <exception>
#include <iostream>

class X {};
class Y {};
class Z : public X {};  // Z 是 X 的派生类
class W {};

// 函数声明：只允许抛出 X 或 Y 类型
void f() throw(X, Y)
{
    bool n = false;

    if (n)
        throw X();  // OK: X 在规范中
    if (n)
        throw Z();   // OK: Z 派生自 X，匹配 X 处理器

    throw W();       // 错误：W 不在规范中，调用 std::unexpected()
}

// 自定义意外异常处理函数
void my_unexpected_handler()
{
    std::cerr << "That was unexpected!" << std::endl;
    std::abort();
}

int main()
{
    // 设置自定义处理函数
    std::set_unexpected(my_unexpected_handler);

    f();  // 将触发 my_unexpected_handler()

    return 0;
}
```

输出：
```
That was unexpected!
```

### 现代 C++ 替代方案

```cpp
#include <iostream>
#include <stdexcept>

class X {};
class Y {};
class Z : public X {};
class W : public std::exception {};

// 不抛出异常的函数
void noThrowFunc() noexcept
{
    // 如果抛出异常，会直接调用 std::terminate()
}

// 可能抛出异常的函数（默认行为）
void mayThrowFunc()
{
    throw std::runtime_error("Error");
}

// 使用 noexcept 条件表达式
template<typename T>
void conditionalFunc() noexcept(std::is_nothrow_destructible<T>::value)
{
    // 根据 T 的析构函数是否 noexcept 决定
}

int main()
{
    try {
        noThrowFunc();        // 安全
        mayThrowFunc();       // 可能抛出
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    // 检查函数是否 noexcept
    static_assert(noexcept(noThrowFunc()), "noThrowFunc should be noexcept");
    static_assert(!noexcept(mayThrowFunc()), "mayThrowFunc may throw");

    return 0;
}
```

### 常见错误及修正

#### 错误 1：在 C++17 中使用动态异常规范

```cpp
// 错误：C++17 中动态异常规范已被移除
void f() throw(int);  // 编译错误！

// 修正：使用 noexcept
void f() noexcept(false);  // 可能抛出异常
```

#### 错误 2：异常规范违反导致程序终止

```cpp
#include <exception>
#include <iostream>

void declaredNoThrow() noexcept
{
    throw std::runtime_error("Oops!");  // 运行时调用 std::terminate()
}

int main()
{
    declaredNoThrow();  // 程序终止
    return 0;
}
```

#### 错误 3：虚函数异常规范不兼容

```cpp
class Base {
public:
    virtual void f() noexcept {}
    virtual void g() {}  // 可能抛出
};

class Derived : public Base {
public:
    // 错误：派生类不能放宽异常规范
    // virtual void f() {}  // 编译错误

    // 正确：派生类可以收紧异常规范
    virtual void f() noexcept override {}

    // 正确：派生类可以添加 noexcept
    virtual void g() noexcept override {}
};
```

### 异常规范与模板

```cpp
#include <type_traits>
#include <vector>

// 条件 noexcept：根据模板参数决定
template<typename T>
void process(T& value) noexcept(std::is_nothrow_move_constructible<T>::value)
{
    T temp = std::move(value);  // 如果 T 的移动构造不是 noexcept，可能抛出
    value = std::move(temp);
}

// 使用 noexcept 操作符检查表达式
template<typename T>
void safeSwap(T& a, T& b) noexcept(noexcept(a.swap(b)))
{
    a.swap(b);
}

int main()
{
    std::vector<int> v1{1, 2, 3};
    std::vector<int> v2{4, 5, 6};

    // 检查是否 noexcept
    static_assert(noexcept(safeSwap(v1, v2)), "swap should be noexcept");

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 方面 | 说明 |
|------|------|
| **定义** | 动态异常规范使用 `throw(type-list)` 声明函数可能抛出的异常类型 |
| **状态** | C++98 引入，C++11 弃用，C++17 移除 |
| **运行时行为** | 抛出未列出异常时调用 `std::unexpected()` |
| **替代方案** | 使用 `noexcept` 说明符 |

### 动态异常规范 vs noexcept

| 特性 | 动态异常规范 | noexcept |
|------|-------------|----------|
| 检查时机 | 运行时 | 编译时（部分） |
| 类型列表 | 支持多种类型 | 仅区分抛出/不抛出 |
| 性能开销 | 有（运行时检查） | 极小或无 |
| 编译器优化 | 阻碍优化 | 促进优化 |
| 状态 | 已移除 | 当前标准 |

### 学习建议

1. **不要使用动态异常规范**：该特性已被移除
2. **理解 noexcept**：学习现代 C++ 的 `noexcept` 说明符
3. **异常安全**：了解基本保证、强保证和不抛出保证
4. **运行时检查**：理解 `std::unexpected()` 和 `std::terminate()` 的区别

### 相关概念

| 概念 | 关系 |
|------|------|
| `noexcept` 说明符 | 动态异常规范的现代替代 |
| `noexcept` 操作符 | 编译时检查表达式是否可能抛出异常 |
| `std::unexpected()` | 异常规范违反时的处理函数 |
| `std::terminate()` | 异常处理失败时调用的终止函数 |
| 异常安全 | 程序在异常发生时保持一致性的能力 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/except_spec
- C++98 Standard: [except.spec]
- C++11 Standard: [except.spec] (deprecated)
- C++17 Standard: removed
- CWG Defect Reports: CWG 25, CWG 973, CWG 1330, CWG 1267, CWG 1351, CWG 1777, CWG 2191