# 复制构造函数 (Copy Constructor)

## 1. 概述

复制构造函数 (Copy Constructor) 是一种特殊的构造函数，它可以用相同类类型的参数调用，并在不修改参数的情况下复制参数的内容。它是 C++ 实现对象复制的核心机制之一。

复制构造函数用于创建一个对象的副本，确保新对象与原对象具有相同的状态，但两者相互独立。这是 C++ 值语义 (value semantics) 的重要组成部分。

## 2. 来源与演变

### C++98 首次引入

复制构造函数自 C++98 标准起就是语言的核心特性。在 C++98 中：

- 如果类没有显式定义复制构造函数，编译器会自动生成一个隐式复制构造函数
- 隐式复制构造函数执行成员逐一复制 (member-wise copy)
- 可以通过将复制构造函数声明为 private 来禁止对象复制

### C++11 变化

C++11 引入了多项重要变化：

- **`= default` 语法**：可以显式要求编译器生成默认复制构造函数
- **`= delete` 语法**：可以显式删除复制构造函数，替代传统的 private 声明方式
- **移动语义**：如果类声明了移动构造函数或移动赋值运算符，复制构造函数会被隐式删除
- **constexpr 支持**：生成的复制构造函数可以是 constexpr 的（如果满足条件）
- **右值引用成员**：如果类有右值引用类型的非静态数据成员，复制构造函数被定义为删除

### C++17 变化

- 异常规范改用 `noexcept` 说明符
- 复制消除 (copy elision) 在某些情况下成为强制要求

### C++20 变化

- 引入了"合格构造函数" (eligible constructor) 概念的精确定义
- 约束条件的满足情况会影响复制构造函数的合格性

### C++23 变化

- 隐式定义的复制构造函数如果是 constexpr 函数（而非仅仅是 constexpr 构造函数），则被认为是 constexpr 的

## 3. 语法与参数

### 声明语法

复制构造函数的声明形式如下：

| 形式 | 语法 | 说明 |
|------|------|------|
| 类内声明 | `class-name(parameter-list);` | 在类定义内声明 |
| 类内定义 | `class-name(parameter-list) function-body` | 在类定义内直接定义 |
| 显式默认 (C++11) | `class-name(single-parameter-list) = default;` | 显式要求编译器生成 |
| 显式删除 (C++11) | `class-name(parameter-list) = delete;` | 禁止复制 |
| 类外定义 | `class-name::class-name(parameter-list) function-body` | 在类外定义 |
| 类外默认 (C++11) | `class-name::class-name(single-parameter-list) = default;` | 类外显式默认 |

### 参数要求

复制构造函数的参数列表必须满足以下条件：

1. **第一个参数**必须是以下类型之一：
   - `T&`（非 const 引用）
   - `const T&`（const 引用，最常见）
   - `volatile T&`
   - `const volatile T&`

2. **其他参数**（如果有）必须都有默认参数

### 参数说明

| 参数 | 说明 |
|------|------|
| `class-name` | 正在声明复制构造函数的类名 |
| `parameter-list` | 非空参数列表，第一个参数必须是类类型的引用 |
| `single-parameter-list` | 仅包含一个参数的参数列表，类型为 `T&`、`const T&`、`volatile T&` 或 `const volatile T&`，且无默认参数 |
| `function-body` | 复制构造函数的函数体 |

### 声明示例

```cpp
struct X
{
    X(X& other);                   // 复制构造函数
    // X(X other);                 // 错误：参数类型不正确
};

union Y
{
    Y(Y& other, int num = 1);      // 带额外参数的复制构造函数
    // Y(Y& other, int num);       // 错误：num 没有默认参数
};
```

## 4. 底层原理

### 隐式声明复制构造函数

如果类类型没有提供用户定义的复制构造函数，编译器会始终声明一个复制构造函数作为类的非显式 (non-explicit)、内联 (inline)、公有 (public) 成员。

隐式声明的复制构造函数的形式取决于类的成员：

**形式为 `T::T(const T&)` 的条件**（所有条件必须同时满足）：
- 每个直接基类和虚基类 `B` 都有参数类型为 `const B&` 或 `const volatile B&` 的复制构造函数
- 每个类类型或类类型数组的非静态数据成员 `M` 都有参数类型为 `const M&` 或 `const volatile M&` 的复制构造函数

**否则**，隐式声明的复制构造函数形式为 `T::T(T&)`。

由于这些规则，隐式声明的复制构造函数无法绑定到 volatile 左值参数。

### 类可以有多个复制构造函数

```cpp
struct A {
    A(const A&);    // 复制构造函数
    A(A&);          // 另一个复制构造函数
};
```

### 隐式定义复制构造函数

如果隐式声明的复制构造函数未被删除，则在 ODR 使用 (odr-used) 或需要常量求值时，编译器会定义（生成函数体并编译）它。

**对于联合体类型**：
- 隐式定义的复制构造函数复制对象表示（如同通过 `std::memmove`）

**对于非联合体类类型**：
- 按照初始化顺序，对对象的直接基类子对象和成员子对象执行完整的成员逐一复制
- 对于引用类型的非静态数据成员，复制构造函数将引用绑定到源引用所绑定的同一对象或函数

### 平凡复制构造函数 (Trivial Copy Constructor)

类的复制构造函数在满足以下所有条件时是平凡的 (trivial)：

1. 不是用户提供的（即隐式定义或默认的）
2. 类没有虚成员函数
3. 类没有虚基类
4. 每个直接基类的复制构造函数都是平凡的
5. 每个非静态类类型（或类类型数组）成员的复制构造函数都是平凡的

**平凡复制构造函数的行为**：
- 对于非联合体类，有效地复制参数的每个标量子对象（递归包括子对象的子对象等），不执行其他操作
- 填充字节 (padding bytes) 不需要复制
- 只要值相同，复制后的子对象的对象表示可以与源不同

**平凡复制的优势**：
- 平凡可复制 (TriviallyCopyable) 对象可以通过手动复制对象表示来复制，例如使用 `std::memmove`
- 与 C 语言兼容的所有数据类型（POD 类型）都是平凡可复制的

### 删除的复制构造函数

隐式声明或显式默认 (C++11 起) 的复制构造函数在满足以下任一条件时被定义为删除：

| 条件 | 版本 |
|------|------|
| 类有右值引用类型的非静态数据成员 | C++11 起 |
| 类有潜在的构造子对象 `M`，其析构函数被删除或不可访问 | - |
| 类有潜在的构造子对象 `M`，重载决议找不到可用的复制构造函数 | - |
| 类有潜在的构造子对象 `M` 是变体成员，且选择了非平凡函数 | - |
| 类声明了移动构造函数或移动赋值运算符 | C++11 起 |

### 合格复制构造函数 (Eligible Copy Constructor)

合格复制构造函数的定义随 C++ 版本演进：

| 版本 | 定义 |
|------|------|
| C++11 之前 | 用户声明，或者隐式声明且可定义 |
| C++11 - C++17 | 未被删除 |
| C++20 起 | 满足所有条件：未被删除；关联约束（如有）被满足；没有更受约束的复制构造函数 |

合格复制构造函数的平凡性决定了类是否是隐式生命周期类型 (implicit-lifetime type) 和是否是平凡可复制类型 (trivially copyable type)。

## 5. 使用场景

### 调用时机

复制构造函数在以下情况被调用：

| 场景 | 示例 | 说明 |
|------|------|------|
| 初始化 | `T a = b;` 或 `T a(b);` | 从同类型对象初始化 |
| 函数参数传递 | `f(a);` | 其中 `a` 是 `T` 类型，`f` 是 `void f(T t)` |
| 函数返回 | `return a;` | 在函数 `T f()` 内部，`a` 是 `T` 类型且无移动构造函数 |

### 何时需要自定义复制构造函数

| 场景 | 原因 |
|------|------|
| 类包含指针成员 | 需要深复制而非浅复制 |
| 类管理资源（文件、网络连接等） | 需要正确复制或共享资源 |
| 类有引用成员 | 需要决定引用绑定的行为 |
| 类有不平凡复制的成员 | 需要自定义复制逻辑 |

### 何时禁止复制

| 场景 | 方法 |
|------|------|
| 禁止复制（C++11 起） | `T(const T&) = delete;` |
| 禁止复制（C++98） | 将复制构造函数声明为 private 且不定义 |

### 最佳实践

1. **Rule of Three/Five/Zero**：如果需要自定义复制构造函数，通常也需要自定义复制赋值运算符和析构函数

2. **优先使用 `= default`**：如果默认行为满足需求，显式使用 `= default` 表明意图

3. **优先使用 `= delete`**：明确禁止复制而非依赖隐式删除

4. **考虑移动语义**：如果定义了复制构造函数，通常也需要定义移动构造函数

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 自复制 | 复制构造函数应处理 `this == &other` 的情况 |
| 移动语义冲突 | 声明移动构造函数会导致复制构造函数被删除 |
| volatile 不支持 | 隐式复制构造函数无法绑定 volatile 左值 |
| 浅复制问题 | 包含指针成员时默认复制可能导致双重释放 |

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

struct A
{
    int n;
    A(int n = 1) : n(n) {}
    A(const A& a) : n(a.n) {} // 用户定义的复制构造函数
};

struct B : A
{
    // 隐式默认构造函数 B::B()
    // 隐式复制构造函数 B::B(const B&)
};

struct C : B
{
    C() : B() {}
private:
    C(const C&); // 不可复制，C++98 风格
};

int main()
{
    A a1(7);
    A a2(a1);      // 调用复制构造函数

    B b;
    B b2 = b;      // 调用隐式复制构造函数
    A a3 = b;      // 转换为 A& 并调用复制构造函数

    volatile A va(10);
    // A a4 = va;  // 编译错误：volatile 不支持

    C c;
    // C c2 = c;   // 编译错误：复制构造函数是私有的

    return 0;
}
```

### 深复制示例

```cpp
#include <cstring>
#include <iostream>

class String {
private:
    char* data;
    size_t size;

public:
    // 构造函数
    String(const char* str = "") {
        size = std::strlen(str);
        data = new char[size + 1];
        std::strcpy(data, str);
    }

    // 析构函数
    ~String() {
        delete[] data;
    }

    // 复制构造函数（深复制）
    String(const String& other) : size(other.size) {
        data = new char[size + 1];
        std::strcpy(data, other.data);
        std::cout << "Copy constructor called\n";
    }

    // 复制赋值运算符
    String& operator=(const String& other) {
        if (this != &other) {  // 防止自赋值
            delete[] data;
            size = other.size;
            data = new char[size + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }

    const char* c_str() const { return data; }
};

int main() {
    String s1("Hello");
    String s2 = s1;  // 调用复制构造函数
    std::cout << s2.c_str() << std::endl;  // 输出: Hello
    return 0;
}
```

### 现代用法（C++11 起）

```cpp
#include <iostream>
#include <utility>

class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;             // 禁止复制
    NonCopyable& operator=(const NonCopyable&) = delete;   // 禁止复制赋值

    // 允许移动
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};

class UseDefaultCopy {
public:
    int value;
    std::string name;

    UseDefaultCopy(int v, const std::string& n) : value(v), name(n) {}

    // 显式要求编译器生成默认复制构造函数
    UseDefaultCopy(const UseDefaultCopy&) = default;
    UseDefaultCopy& operator=(const UseDefaultCopy&) = default;
};

int main() {
    NonCopyable n1;
    // NonCopyable n2 = n1;  // 编译错误：复制构造函数已删除
    NonCopyable n3 = std::move(n1);  // OK：移动构造

    UseDefaultCopy u1(42, "test");
    UseDefaultCopy u2 = u1;  // OK：使用默认复制构造函数

    return 0;
}
```

### 常见错误及修正

#### 错误 1：忘记处理自赋值

```cpp
// 错误：没有处理自赋值
class BadString {
public:
    BadString& operator=(const BadString& other) {
        delete[] data;              // 如果 this == &other，other.data 已被删除！
        data = new char[other.size + 1];
        std::strcpy(data, other.data);  // 未定义行为
        return *this;
    }
private:
    char* data;
    size_t size;
};

// 正确：处理自赋值
class GoodString {
public:
    GoodString& operator=(const GoodString& other) {
        if (this != &other) {       // 检查自赋值
            delete[] data;
            size = other.size;
            data = new char[size + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }
private:
    char* data;
    size_t size;
};

// 更好：使用 copy-and-swap 惯用法
#include <utility>
class BetterString {
public:
    BetterString(const BetterString& other) : size(other.size), data(new char[other.size + 1]) {
        std::strcpy(data, other.data);
    }

    BetterString& operator=(BetterString other) {  // 注意：按值传递
        swap(*this, other);
        return *this;
    }

    friend void swap(BetterString& first, BetterString& second) {
        using std::swap;
        swap(first.size, second.size);
        swap(first.data, second.data);
    }
private:
    char* data;
    size_t size;
};
```

#### 错误 2：移动语义与复制语义冲突

```cpp
#include <iostream>

class MyClass {
public:
    MyClass() = default;

    // 移动构造函数
    MyClass(MyClass&& other) noexcept {
        std::cout << "Move constructor\n";
    }

    // 错误：没有显式定义复制构造函数，它被隐式删除了！
};

int main() {
    MyClass a;
    // MyClass b = a;  // 编译错误：复制构造函数已删除
    MyClass c = std::move(a);  // OK
}

// 修正：显式定义或默认复制构造函数
class MyClassFixed {
public:
    MyClassFixed() = default;

    MyClassFixed(const MyClassFixed&) = default;  // 显式默认
    MyClassFixed(MyClassFixed&&) noexcept = default;
};
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| **定义** | 第一个参数为类类型引用的构造函数 |
| **自动生成** | 未定义时编译器自动声明并生成（除非有删除条件） |
| **调用时机** | 对象初始化、参数传递、函数返回 |
| **平凡性** | 满足特定条件时为平凡复制，可使用 `std::memmove` |
| **删除条件** | 移动构造函数、右值引用成员、不可复制成员等 |

### 版本特性对比

| 特性 | C++98 | C++11 起 |
|------|-------|----------|
| 禁止复制 | private 声明 | `= delete` |
| 显式默认 | 不支持 | `= default` |
| 移动语义影响 | 无影响 | 声明移动构造函数会删除复制构造函数 |
| constexpr | 不支持 | 支持（条件满足时） |

### 学习建议

1. **理解默认行为**：掌握编译器何时自动生成复制构造函数及其行为
2. **掌握 Rule of Five**：复制构造、复制赋值、移动构造、移动赋值、析构函数需一起考虑
3. **优先使用默认**：默认行为通常正确，仅在需要深复制时自定义
4. **注意移动语义**：C++11 起需考虑移动操作对复制操作的影响

### 相关概念

| 概念 | 关系 |
|------|------|
| 移动构造函数 (Move Constructor) | 替代复制构造函数用于右值，提高性能 |
| 复制赋值运算符 (Copy Assignment Operator) | 与复制构造函数同属复制语义 |
| 复制消除 (Copy Elision) | 编译器优化，可能省略复制构造函数调用 |
| 转换构造函数 (Converting Constructor) | 非显式构造函数，可从其他类型隐式转换 |
| 默认构造函数 (Default Constructor) | 无参数构造函数 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/copy_constructor
- C++ Standard: [class.copy.ctor]
- Effective C++, Scott Meyers, Item 11: Handle assignment to self in operator=
- Effective Modern C++, Scott Meyers, Item 17: Understand special member function generation