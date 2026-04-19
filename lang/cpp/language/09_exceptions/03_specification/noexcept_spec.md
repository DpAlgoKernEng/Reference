# `noexcept` 说明符 (C++11 起)

## 1. 概述 (Overview)

`noexcept` 说明符（noexcept specifier）是 C++11 引入的一种异常规范机制，用于指定函数是否可能抛出异常。通过该说明符，程序员可以向编译器明确承诺函数是否会抛出异常，从而帮助编译器进行代码优化，并使 `noexcept` 运算符能够在编译期检查特定表达式是否声明为不抛出异常。

### 核心概念

在 C++ 中，每个函数都被归类为以下两种类型之一：

- **非抛出函数（non-throwing function）**：保证不会抛出任何异常
- **潜在抛出函数（potentially-throwing function）**：可能抛出异常

`noexcept` 说明符是函数类型的一部分（自 C++17 起），其正确使用对于实现高效的移动语义、容器优化以及异常安全保证至关重要。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++11 之前，C++ 支持**动态异常规范**（dynamic exception specification），即使用 `throw(type1, type2, ...)` 或 `throw()` 的形式来指定函数可能抛出的异常类型。然而，这种机制存在以下问题：

- 运行时检查开销大
- 编译时优化困难
- 实际使用中很少被正确应用

### C++11 的改进

C++11 引入 `noexcept` 说明符作为改进方案，取代了旧的 `throw()` 语法：

| 特性 | `throw()` (C++11前) | `noexcept` (C++11起) |
|------|---------------------|----------------------|
| 违规行为 | 调用 `std::unexpected` | 调用 `std::terminate` |
| 栈展开 | 总是展开 | 可能不展开 |
| 运行时开销 | 较高 | 较低或无 |

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| **C++11** | 引入 `noexcept` 说明符；`throw()` 废弃但保留 |
| **C++17** | `throw()` 重定义为 `noexcept(true)` 的等价形式；异常规范成为函数类型的一部分；`throw()` 标记为废弃 |
| **C++20** | 完全移除 `throw()` 动态异常规范 |

### 特性测试宏

| 宏名称 | 值 | 标准 | 特性描述 |
|--------|-----|------|----------|
| `__cpp_noexcept_function_type` | `201510L` | C++17 | 将异常规范纳入类型系统 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```
noexcept                                          // (1) 等价于 noexcept(true)
noexcept( expression )                            // (2) 条件性 noexcept
throw()                                            // (3) C++17前等价于 noexcept(true)
                                                   //     C++17起废弃，C++20起移除
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `expression` | 上下文转换为 `bool` 类型的常量表达式。如果求值为 `true`，则函数声明为不抛出异常 |

### 语法详解

**形式 (1)：无条件 noexcept**
- 等价于 `noexcept(true)`
- 声明函数保证不抛出任何异常

**形式 (2)：条件性 noexcept**
- 根据表达式的值决定函数是否为非抛出函数
- `noexcept` 后紧跟的 `(` 总是作为语法的一部分，而非初始化列表

**形式 (3)：废弃的 throw()**
- 在 C++17 中等价于 `noexcept(true)`
- 已废弃，C++20 中完全移除

### 声明位置规则

**C++17 之前：**
- 异常规范不是函数类型的一部分
- 只能出现在函数声明符、函数指针、函数引用、成员函数指针等声明中
- 不能出现在 typedef 或类型别名声明中

**C++17 起：**
- 异常规范是函数类型的一部分
- 可以出现在任何函数声明符中

```cpp
// C++17 前后的差异示例
void f() noexcept;              // 函数 f() 不抛出异常
void (*fp)() noexcept(false);   // fp 指向可能抛出异常的函数
void g(void pfa() noexcept);     // g 接受一个不抛出异常的函数指针

// typedef int (*pf)() noexcept; // 错误：C++17 前不能在 typedef 中使用
```

---

## 4. 底层原理 (Underlying Principles)

### 非抛出函数的判定

函数被判定为**非抛出函数**的条件：

1. 使用 `noexcept` 或 `noexcept(true)` 声明的函数
2. 隐式声明的析构函数（除非任何基类或成员的析构函数是潜在抛出的）
3. 在首次声明时默认定义（defaulted）的特殊成员函数：
   - 默认构造函数
   - 复制构造函数
   - 移动构造函数
   - 复制赋值运算符
   - 移动赋值运算符
   - 比较运算符（C++20 起）
4. 释放函数（deallocation functions）

### 潜在抛出函数的判定

函数被判定为**潜在抛出函数**的条件：

1. 使用 `noexcept(false)` 声明的函数
2. 未使用 `noexcept` 说明符的函数（上述非抛出函数除外）
3. 使用非空动态异常规范声明的函数（C++17 前）

### 潜在抛出表达式（Potentially-throwing Expression）

表达式 `e` 被判定为**潜在抛出表达式**的条件：

| 条件 | 说明 |
|------|------|
| 函数调用 | 调用潜在抛出的函数、函数指针或成员函数指针 |
| 隐式调用 | 隐式调用潜在抛出函数（如重载运算符、new 表达式中的分配函数、构造函数、析构函数等） |
| throw 表达式 | 显式的 `throw` 语句 |
| dynamic_cast | 对多态引用类型进行 `dynamic_cast` |
| typeid | 对解引用的多态类型指针应用 `typeid` |
| 子表达式 | 包含潜在抛出的子表达式 |

### 异常规范的延迟实例化

函数模板特化的异常规范不会随函数声明一起实例化，仅在**需要时**（needed）才实例化：

**需要实例化异常规范的上下文：**

1. 表达式中函数被重载决议选中
2. 函数被 ODR 使用（odr-used）
3. 函数本会被 ODR 使用但出现在未求值操作数中
4. 需要与另一个函数声明比较（如虚函数重写或模板显式特化）
5. 在函数定义中
6. 默认特殊成员函数需要检查以决定自身的异常规范

```cpp
template<class T>
T f() noexcept(sizeof(T) < 4);

int main()
{
    decltype(f<void>()) *p; // f 未求值，但 noexcept 规范被需要
                            // 错误：实例化 noexcept 规范时计算 sizeof(void)
}
```

### 违规处理

如果异常被抛出，并且在寻找处理程序时遇到非抛出函数的最外层代码块，将调用 `std::terminate`：

```cpp
extern void f(); // 潜在抛出

void g() noexcept
{
    f();      // 合法，即使 f 抛出异常
    throw 42; // 合法，但效果是调用 std::terminate
}
```

---

## 5. 使用场景 (Use Cases)

### 编译器优化

`noexcept` 说明符使编译器能够对非抛出函数进行优化：

- 减少异常处理相关的代码生成
- 优化栈展开逻辑
- 更好的内联决策

### 容器优化

标准容器（如 `std::vector`）会根据元素的移动构造函数是否为 `noexcept` 来决定使用移动还是复制操作：

```cpp
// 如果元素的移动构造函数是 noexcept，vector 在扩容时会移动元素
// 否则会复制元素（除非复制构造函数不可访问）

template<typename T>
void relocate_elements(T* new_location, T* old_location, size_t n)
{
    if constexpr (std::is_nothrow_move_constructible_v<T>) {
        // 使用移动构造
        for (size_t i = 0; i < n; ++i)
            new (new_location + i) T(std::move(old_location[i]));
    } else {
        // 使用复制构造
        for (size_t i = 0; i < n; ++i)
            new (new_location + i) T(old_location[i]);
    }
}
```

### 宽松异常保证

使用 `std::move_if_noexcept` 可以在移动构造函数为非抛出时选择移动，否则选择复制：

```cpp
template<typename T>
void process(T&& arg)
{
    auto obj = std::move_if_noexcept(arg);
    // 如果移动构造是 noexcept，则移动；否则复制
}
```

### 虚函数重写

如果虚函数是非抛出的，所有重写函数（overrider）也必须是非抛出的，除非定义为删除：

```cpp
struct Base
{
    virtual void f() noexcept;
    virtual void g();
    virtual void h() noexcept = delete;
};

struct Derived : Base
{
    void f();          // 错误：潜在抛出，基类是非抛出
    void g() noexcept; // 正确
    void h() = delete; // 正确
};
```

### 函数指针转换

指向非抛出函数的指针可以转换为指向潜在抛出函数的指针，但反之不可：

```cpp
void ft();                     // 潜在抛出
void (*fn)() noexcept = ft;    // 错误：不能转换
```

### 最佳实践

1. **移动操作应为 noexcept**：移动构造函数和移动赋值运算符通常应标记为 `noexcept`，以启用容器优化
2. **析构函数隐式 noexcept**：除非基类或成员的析构函数抛出异常
3. **交换操作应为 noexcept**：`swap` 函数应保证不抛出异常
4. **谨慎使用条件 noexcept**：在模板中使用 `noexcept(noexcept(...))` 模式

### 常见陷阱

1. **noexcept 不是编译时检查**：标记 `noexcept` 的函数仍可能抛出异常，只是会调用 `std::terminate`
2. **函数重载冲突**：C++17 起，仅异常规范不同的函数不能重载
3. **虚函数约束**：重写虚函数时必须保持相同的异常规范或更严格的 `noexcept`

---

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>
#include <stdexcept>

// 无条件 noexcept
void safe_function() noexcept
{
    std::cout << "This function never throws.\n";
}

// 条件 noexcept
void unsafe_function() noexcept(false)
{
    std::cout << "This function may throw.\n";
    throw std::runtime_error("Error!");
}

// 默认：潜在抛出
void normal_function()
{
    std::cout << "This function may also throw.\n";
}

int main()
{
    safe_function();    // 安全调用

    try {
        unsafe_function();
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    return 0;
}
```

### 模板中的条件 noexcept

```cpp
#include <iostream>
#include <type_traits>

// 使用 noexcept 运算符检测类型 T 是否有不抛出的默认构造函数
template<typename T>
void process() noexcept(noexcept(T()))
{
    T obj{};
    std::cout << "Processing...\n";
}

// 检测成员函数是否抛出异常
template<typename T>
void call_method(T& obj) noexcept(noexcept(obj.method()))
{
    obj.method();
}

struct SafeType
{
    SafeType() noexcept = default;
    void method() noexcept { std::cout << "Safe method\n"; }
};

struct UnsafeType
{
    UnsafeType() { std::cout << "Unsafe constructor\n"; }
    void method() { std::cout << "Unsafe method\n"; }
};

int main()
{
    process<int>();       // noexcept(true)
    process<SafeType>();  // noexcept(true)
    process<UnsafeType>(); // noexcept(false)

    SafeType s;
    call_method(s);       // noexcept(true)

    return 0;
}
```

### 移动语义与 noexcept

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <utility>

class Resource
{
public:
    Resource() : data_(nullptr), size_(0) {}

    // 复制构造函数：可能抛出（分配内存）
    Resource(const Resource& other)
        : data_(new int[other.size_]), size_(other.size_)
    {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Copy constructor\n";
    }

    // 移动构造函数：不抛出异常
    Resource(Resource&& other) noexcept
        : data_(other.data_), size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "Move constructor (noexcept)\n";
    }

    ~Resource()
    {
        delete[] data_;
    }

private:
    int* data_;
    size_t size_;
};

int main()
{
    std::vector<Resource> vec;
    Resource r;

    // 由于移动构造函数是 noexcept，vector 扩容时会使用移动
    vec.push_back(Resource{});  // 移动构造

    return 0;
}
```

### 特殊成员函数的隐式 noexcept

```cpp
#include <iostream>

struct A
{
    A(int = (A(5), 0)) noexcept;
    A(const A&) noexcept;
    A(A&&) noexcept;
    ~A();
};

struct B
{
    B() throw();  // C++17 前等价于 noexcept
    B(const B&) = default;  // 隐式 noexcept(true)
    B(B&&, int = (throw 42, 0)) noexcept;  // noexcept 但有抛出异常的默认参数
    ~B() noexcept(false);  // 显式潜在抛出
};

int n = 7;

struct D : public A, public B
{
    int* p = new int[n];
    // D::D() 潜在抛出（因为 new 运算符）
    // D::D(const D&) 非抛出
    // D::D(D&&) 潜在抛出（B 的构造函数默认参数可能抛出）
    // D::~D() 潜在抛出（B::~B() 潜在抛出）
};

int main()
{
    static_assert(std::is_nothrow_move_constructible_v<A>, "A is nothrow move constructible");
    static_assert(!std::is_nothrow_move_constructible_v<D>, "D is not nothrow move constructible");

    return 0;
}
```

### 常见错误与修正

```cpp
// === 错误示例 1：重写虚函数时异常规范不匹配 ===
struct Base1
{
    virtual void foo() noexcept;
};

struct Derived1 : Base1
{
    void foo();  // 错误：基类是 noexcept，派生类不能是潜在抛出
};

// 修正：保持一致的异常规范
struct Derived1_Fixed : Base1
{
    void foo() noexcept override;  // 正确
};

// === 错误示例 2：函数指针转换 ===
void may_throw();
void (*p_noexcept)() noexcept = may_throw;  // 错误：不能转换

// 修正：使用正确的函数指针类型
void (*p_may_throw)() = may_throw;  // 正确

// === 错误示例 3：noexcept 函数抛出异常 ===
void bad_noexcept() noexcept
{
    throw std::runtime_error("Oops!");  // 程序终止！
}

// 正确做法：要么移除 noexcept，要么确保不抛出
void good_noexcept() noexcept
{
    try {
        // 可能抛出的代码放在 try 块中
    } catch (...) {
        // 在 noexcept 函数中处理所有异常
    }
}

// === 错误示例 4：C++17 中仅异常规范不同的重载 ===
void func() noexcept;
void func();  // C++17 起：错误，仅异常规范不同

// 修正：删除其中一个声明
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| **用途** | 指定函数是否可能抛出异常 |
| **语法** | `noexcept` 或 `noexcept(expression)` |
| **类型地位** | C++17 起成为函数类型的一部分 |
| **违规处理** | 调用 `std::terminate` |
| **主要优势** | 编译器优化、容器移动语义优化、编译期检查 |

### 与 `throw()` 的对比

| 特性 | `noexcept` | `throw()` (已废弃) |
|------|------------|-------------------|
| 引入版本 | C++11 | C++98 |
| 状态 | 当前标准 | C++17 废弃，C++20 移除 |
| 违规处理 | `std::terminate` | `std::unexpected` |
| 栈展开 | 可能不展开 | 总是展开 |
| 运行时开销 | 低 | 高 |

### 学习建议

1. **优先使用 noexcept**：为移动构造函数、移动赋值运算符和 swap 函数添加 `noexcept` 说明符
2. **理解条件 noexcept**：掌握 `noexcept(noexcept(...))` 模式在模板中的应用
3. **注意类型系统**：C++17 起异常规范影响函数类型，重载和虚函数重写需特别留意
4. **避免误用**：`noexcept` 不是编译时异常检查，违规会导致程序终止

### 相关主题

| 主题 | 说明 |
|------|------|
| `noexcept` 运算符 | 编译期检测表达式是否声明为不抛出 |
| 动态异常规范 | C++17 前的 `throw(type)` 语法（已废弃） |
| `throw` 表达式 | 抛出异常 |
| `std::move_if_noexcept` | 条件性移动语义 |

---

## 参考链接

- [cppreference: noexcept specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec)
- [C++17: Exception specifications as part of type system](https://wg21.link/P0012R1)