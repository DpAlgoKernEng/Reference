# 移动赋值运算符 (Move Assignment Operator)

## 1. 概述

移动赋值运算符（move assignment operator）是一种特殊的非模板非静态成员函数，其名称为 `operator=`，可以使用相同类类型的参数调用，并复制参数的内容，可能会修改参数的状态。

移动赋值运算符是 C++11 引入的核心特性之一，与移动构造函数一起构成了移动语义（move semantics）的基础。它的主要目的是实现资源的高效转移，而非传统的深拷贝，从而显著提升程序性能。

### 核心概念

- **移动语义**：转移资源所有权而非复制资源
- **右值引用**：使用 `T&&` 语法表示右值引用参数
- **资源转移**：指针、文件描述符、线程句柄等资源的所有权转移
- **有效但未指定状态**：移动后的源对象处于可析构的有效状态

## 2. 来源与演变

### 历史背景

在 C++11 之前，C++ 仅支持复制语义。当对象被赋值时，必须进行深拷贝，这对于管理动态内存或外部资源的对象来说效率低下：

```cpp
// C++11 之前：必须复制所有资源
std::vector<int> v1 = {1, 2, 3, 4, 5};
std::vector<int> v2;
v2 = v1;  // 必须复制所有元素，即使 v1 不再需要
```

### C++11 引入移动赋值运算符

C++11 标准引入了移动语义，解决了以下问题：

1. **性能优化**：避免不必要的资源复制
2. **资源转移**：支持唯一所有权语义（如 `std::unique_ptr`）
3. **完美转发**：配合 `std::forward` 实现高效参数传递

### 标准演进

| 版本 | 变化 |
|------|------|
| C++11 | 首次引入移动赋值运算符和移动语义 |
| C++14 | 隐式定义的移动赋值运算符可以是 `constexpr` |
| C++20 | 引入合格（eligible）移动赋值运算符概念，用于约束条件 |
| C++23 | 隐式定义的移动赋值运算符始终为 `constexpr` |

### 缺陷报告

| DR | 版本 | 问题 | 修正 |
|-----|------|------|------|
| CWG 1353 | C++11 | 默认删除条件未考虑多维数组类型 | 考虑这些类型 |
| CWG 1402 | C++11 | 调用非平凡复制赋值的默认移动赋值被删除 | 允许调用；删除的在重载决议中忽略 |
| CWG 1806 | C++11 | 虚基类规范缺失 | 已添加 |
| CWG 2094 | C++11 | volatile 子对象影响平凡性 | 不影响平凡性 |
| CWG 2180 | C++11 | 抽象类与非可移动赋值的虚基类情况 | 定义为删除 |
| CWG 2595 | C++20 | 约束更严格但不满足条件的运算符 | 可以合格 |
| CWG 2690 | C++11 | 联合体的隐式定义未复制对象表示 | 复制对象表示 |

## 3. 语法与参数

### 语法形式

移动赋值运算符的语法形式如下：

| 序号 | 语法 | 说明 |
|------|------|------|
| (1) | `return-type operator=(parameter-list);` | 类内声明 |
| (2) | `return-type operator=(parameter-list) function-body` | 类内定义 |
| (3) | `return-type operator=(parameter-list-no-default) = default;` | 显式默认 |
| (4) | `return-type operator=(parameter-list) = delete;` | 显式删除 |
| (5) | `return-type class-name::operator=(parameter-list) function-body` | 类外定义 |
| (6) | `return-type class-name::operator=(parameter-list-no-default) = default;` | 类外默认 |

### 参数说明

| 参数 | 说明 |
|------|------|
| `class-name` | 声明移动赋值运算符的类名，下文中类型记为 `T` |
| `parameter-list` | 仅包含一个参数的参数列表，类型为 `T&&`、`const T&&`、`volatile T&&` 或 `const volatile T&&` |
| `parameter-list-no-default` | 同上，但不允许默认参数 |
| `function-body` | 函数体 |
| `return-type` | 任意类型，推荐 `T&` 以与标量类型保持一致 |

### 基本声明示例

```cpp
struct X
{
    X& operator=(X&& other);    // 移动赋值运算符
//  X operator=(const X other); // 错误：参数类型不正确
};

union Y
{
    // 其他合法语法形式
    auto operator=(Y&& other) -> Y&;       // OK: 尾置返回类型
    Y& operator=(this Y&& self, Y& other); // OK: 显式对象参数 (C++23)
//  Y& operator=(Y&&, int num = 1);        // 错误：有其他非对象参数
};
```

## 4. 底层原理

### 隐式声明条件

如果没有为类类型提供用户定义的移动赋值运算符，且满足以下所有条件，编译器将声明一个移动赋值运算符：

- 没有用户声明的复制构造函数
- 没有用户声明的移动构造函数
- 没有用户声明的复制赋值运算符
- 没有用户声明的析构函数

隐式声明的签名为：`T& T::operator=(T&&)`

### 隐式定义行为

当隐式声明的移动赋值运算符既未被删除也非平凡时，编译器会自动定义其函数体：

**对于联合体类型**：隐式定义的移动赋值运算符复制对象表示（如同 `std::memmove`）。

**对于非联合体类类型**：执行完整的逐成员移动赋值：
- 按声明顺序对直接基类执行移动赋值
- 对非静态成员执行移动赋值
- 对标量类型使用内置赋值
- 对数组执行逐元素移动赋值
- 对类类型调用其移动赋值运算符（非虚调用）

### 虚基类行为

对于通过继承格中多条路径可访问的虚基类子对象，隐式定义的移动赋值运算符可能会多次赋值：

```cpp
struct V
{
    V& operator=(V&& other)
    {
        // 可能被调用一次或两次
        // 如果调用两次，'other' 是刚被移动的 V 子对象
        return *this;
    }
};

struct A : virtual V {}; // operator= 调用 V::operator=
struct B : virtual V {}; // operator= 调用 V::operator=
struct C : B, A {};      // operator= 调用 B::operator=，然后 A::operator=
                         // 但 V::operator= 可能只被调用一次

int main()
{
    C c1, c2;
    c2 = std::move(c1);
}
```

### 删除条件

隐式声明或默认的移动赋值运算符在以下情况被定义为删除：

1. 类 `T` 有 `const` 限定的非类类型非静态数据成员（或多维数组）
2. 类 `T` 有引用类型的非静态数据成员
3. 类 `T` 有潜在构造子对象 `M`（或多维数组），且：
   - 无法找到 `M` 的移动赋值运算符的可用候选，或
   - 对于变体成员，选择了非平凡函数

被删除的隐式声明移动赋值运算符会被重载决议忽略。

### 平凡性条件

类 `T` 的移动赋值运算符是平凡的，当且仅当：

- 非用户提供（隐式定义或默认）
- `T` 没有虚成员函数
- `T` 没有虚基类
- 每个直接基类的移动赋值运算符都是平凡的
- 每个非静态类类型成员的移动赋值运算符都是平凡的

**注意**：平凡的移动赋值运算符执行与平凡复制赋值运算符相同的操作，即通过 `std::memmove` 复制对象表示。所有与 C 语言兼容的数据类型都是平凡可移动赋值的。

### 合格性条件 (C++20)

| 版本 | 条件 |
|------|------|
| C++20 之前 | 移动赋值运算符合格当且仅当未被删除 |
| C++20 起 | 移动赋值运算符合格当且仅当满足：<br>1. 未被删除<br>2. 关联约束（如有）满足<br>3. 没有更受约束的移动赋值运算符 |

合格移动赋值运算符的平凡性决定类是否为平凡可复制类型。

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 管理动态内存的类 | 转移指针所有权，避免深拷贝 |
| 管理系统资源的类 | 文件描述符、线程句柄、TCP 套接字等 |
| 容器类 | 高效转移元素所有权 |
| 唯一所有权语义 | 如 `std::unique_ptr`，禁止复制只允许移动 |

### 重载决议行为

当同时提供复制赋值和移动赋值运算符时：

- **右值参数**：选择移动赋值（prvalue 临时量或 xvalue 如 `std::move` 结果）
- **左值参数**：选择复制赋值（命名对象或返回左值引用的函数/运算符）
- **仅复制赋值**：所有参数类别都选择它（右值可绑定到 const 引用）

```cpp
// 右值 -> 移动赋值
obj = SomeClass();          // prvalue 临时量
obj = std::move(other_obj); // xvalue

// 左值 -> 复制赋值
obj = named_object;         // 左值
```

### 最佳实践

1. **始终实现移动赋值时同时实现移动构造**：保持语义一致性
2. **移动后源对象应处于有效状态**：可析构、可赋值
3. **使用 `noexcept` 说明符**：移动操作通常不应抛出异常
4. **返回 `*this`**：支持链式赋值
5. **自赋值检查**：处理 `a = std::move(a)` 情况

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 忘记实现移动构造函数 | 只实现移动赋值会导致不一致行为 |
| 析构函数阻止隐式生成 | 用户定义析构函数会阻止隐式移动赋值 |
| const 成员或引用成员 | 导致移动赋值被隐式删除 |
| 自移动 | 需要正确处理 `a = std::move(a)` |

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <string>
#include <utility>

struct A
{
    std::string s;

    A() : s("test") {}

    A(const A& o) : s(o.s) { std::cout << "copy constructor\n"; }

    A(A&& o) : s(std::move(o.s)) { std::cout << "move constructor\n"; }

    A& operator=(const A& other)
    {
        s = other.s;
        std::cout << "copy assigned\n";
        return *this;
    }

    A& operator=(A&& other) noexcept
    {
        s = std::move(other.s);
        std::cout << "move assigned\n";
        return *this;
    }
};

int main()
{
    A a1, a2;

    // 从右值临时量移动赋值
    std::cout << "Move-assign from rvalue temporary:\n";
    a1 = A();  // 调用移动赋值

    // 从 xvalue 移动赋值
    std::cout << "\nMove-assign from xvalue:\n";
    a2 = std::move(a1);  // 调用移动赋值

    return 0;
}
```

输出：
```
Move-assign from rvalue temporary:
move assigned

Move-assign from xvalue:
move assigned
```

### 完整示例：隐式与显式行为

```cpp
#include <iostream>
#include <string>
#include <utility>

struct A
{
    std::string s;

    A() : s("test") {}

    A(const A& o) : s(o.s) { std::cout << "move failed!\n"; }

    A(A&& o) : s(std::move(o.s)) {}

    A& operator=(const A& other)
    {
         s = other.s;
         std::cout << "copy assigned\n";
         return *this;
    }

    A& operator=(A&& other)
    {
         s = std::move(other.s);
         std::cout << "move assigned\n";
         return *this;
    }
};

A f(A a) { return a; }

struct B : A
{
    std::string s2;
    int n;
    // 隐式移动赋值运算符 B& B::operator=(B&&)
    // 调用 A 的移动赋值运算符
    // 调用 s2 的移动赋值运算符
    // 对 n 进行位复制
};

struct C : B
{
    ~C() {} // 析构函数阻止隐式移动赋值
};

struct D : B
{
    D() {}
    ~D() {} // 析构函数会阻止隐式移动赋值
    D& operator=(D&&) = default; // 强制生成移动赋值
};

int main()
{
    A a1, a2;
    std::cout << "Trying to move-assign A from rvalue temporary\n";
    a1 = f(A()); // 从右值临时量移动赋值
    std::cout << "Trying to move-assign A from xvalue\n";
    a2 = std::move(a1); // 从 xvalue 移动赋值

    std::cout << "\nTrying to move-assign B\n";
    B b1, b2;
    std::cout << "Before move, b1.s = \"" << b1.s << "\"\n";
    b2 = std::move(b1); // 调用隐式移动赋值
    std::cout << "After move, b1.s = \"" << b1.s << "\"\n";

    std::cout << "\nTrying to move-assign C\n";
    C c1, c2;
    c2 = std::move(c1); // 调用复制赋值运算符！

    std::cout << "\nTrying to move-assign D\n";
    D d1, d2;
    d2 = std::move(d1);
}
```

输出：
```
Trying to move-assign A from rvalue temporary
move assigned
Trying to move-assign A from xvalue
move assigned

Trying to move-assign B
Before move, b1.s = "test"
move assigned
After move, b1.s = ""

Trying to move-assign C
copy assigned

Trying to move-assign D
move assigned
```

### 管理资源的完整示例

```cpp
#include <iostream>
#include <utility>

class Buffer
{
private:
    int* data_;
    size_t size_;

public:
    // 默认构造函数
    Buffer() : data_(nullptr), size_(0) {}

    // 参数构造函数
    explicit Buffer(size_t size) : data_(new int[size]{}), size_(size) {}

    // 析构函数
    ~Buffer()
    {
        delete[] data_;
    }

    // 复制构造函数
    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_)
    {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Copy constructor\n";
    }

    // 移动构造函数
    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "Move constructor\n";
    }

    // 复制赋值运算符
    Buffer& operator=(const Buffer& other)
    {
        if (this != &other)
        {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        std::cout << "Copy assignment\n";
        return *this;
    }

    // 移动赋值运算符
    Buffer& operator=(Buffer&& other) noexcept
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        std::cout << "Move assignment\n";
        return *this;
    }

    size_t size() const { return size_; }
    bool valid() const { return data_ != nullptr; }
};

int main()
{
    Buffer b1(100);
    std::cout << "b1 valid: " << b1.valid() << ", size: " << b1.size() << "\n";

    Buffer b2;
    b2 = std::move(b1);  // 移动赋值

    std::cout << "After move:\n";
    std::cout << "b1 valid: " << b1.valid() << ", size: " << b1.size() << "\n";
    std::cout << "b2 valid: " << b2.valid() << ", size: " << b2.size() << "\n";

    return 0;
}
```

### 常见错误及修正

#### 错误 1：析构函数阻止隐式生成

```cpp
// 错误：析构函数阻止隐式移动赋值
struct C : B
{
    ~C() {} // 用户定义析构函数
};
// 移动赋值会回退到复制赋值，效率低下

// 修正：显式默认移动赋值
struct D : B
{
    ~D() {}
    D& operator=(D&&) = default; // 强制生成
};
```

#### 错误 2：忘记自赋值检查

```cpp
// 错误：未处理自移动赋值
Buffer& operator=(Buffer&& other) noexcept
{
    delete[] data_;        // 如果 this == &other，数据丢失！
    data_ = other.data_;
    size_ = other.size_;
    other.data_ = nullptr;
    other.size_ = 0;
    return *this;
}

// 修正：添加自赋值检查
Buffer& operator=(Buffer&& other) noexcept
{
    if (this != &other)    // 检查自赋值
    {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}
```

#### 错误 3：const 成员导致删除

```cpp
// 错误：const 成员阻止移动赋值
struct Bad
{
    const int id;         // const 成员
    std::string name;

    Bad(int i, std::string n) : id(i), name(std::move(n)) {}
    // 移动赋值运算符被隐式删除！
};

// 修正方案 1：移除 const
struct Good1
{
    int id;               // 非 const
    std::string name;
};

// 修正方案 2：提供自定义移动赋值
struct Good2
{
    const int id;
    std::string name;

    Good2(int i, std::string n) : id(i), name(std::move(n)) {}

    Good2& operator=(Good2&& other) noexcept
    {
        if (this != &other)
        {
            // id 不能修改，只能移动 name
            name = std::move(other.name);
        }
        return *this;
    }
};
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| 目的 | 高效转移资源所有权，避免深拷贝 |
| 签名 | `T& operator=(T&&)` 或类似形式 |
| 隐式生成 | 需满足"三五法则"（无用户定义的特殊成员函数） |
| 移动后状态 | 源对象处于有效但未指定状态 |
| 返回值 | 推荐 `T&`，支持链式赋值 |

### 移动赋值 vs 复制赋值

| 特性 | 复制赋值 | 移动赋值 |
|------|---------|---------|
| 参数类型 | `const T&` | `T&&` |
| 资源处理 | 深拷贝 | 所有权转移 |
| 源对象状态 | 保持不变 | 有效但未指定 |
| 性能 | 较低（需要分配内存） | 较高（指针交换） |
| 异常 | 可能抛出 | 通常 `noexcept` |

### 隐式生成条件

满足以下全部条件时，编译器隐式生成移动赋值运算符：

1. 无用户声明的复制构造函数
2. 无用户声明的移动构造函数
3. 无用户声明的复制赋值运算符
4. 无用户声明的析构函数

### 学习建议

1. **理解"三五法则"**：如果需要定义任何一个特殊成员函数，通常需要定义全部五个
2. **优先使用 `= default`**：让编译器生成高效正确的实现
3. **使用 `std::move` 显式移动**：明确表达意图
4. **注意移动后状态**：不要依赖移动后对象的值
5. **测试移动操作**：验证移动语义正确实现

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/move_assignment
- C++ Standard: [class.copy.assign]
- Effective Modern C++, Scott Meyers, Item 17-18
- The C++ Programming Language, Bjarne Stroustrup