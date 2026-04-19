# 三法则/五法则/零法则 (The Rule of Three/Five/Zero)

## 1. 概述 (Overview)

**三法则 (Rule of Three)** 是 C++ 中关于资源管理类设计的重要准则：如果一个类需要用户定义的析构函数、拷贝构造函数或拷贝赋值运算符中的任何一个，那么它几乎肯定需要全部三个。

这一规则源于 C++ 对象复制和赋值的机制。当类管理着某种资源（如动态内存、文件句柄、网络连接等），而该资源的句柄本身不会自动销毁资源（如裸指针 raw pointer、POSIX 文件描述符等）时，编译器隐式生成的特殊成员函数无法正确处理资源的生命周期。

随着 C++11 引入移动语义，该准则扩展为**五法则 (Rule of Five)**。进一步地，现代 C++ 提倡**零法则 (Rule of Zero)**，即通过使用标准库资源管理类（如 `std::string`、`std::vector`、智能指针等）来避免手动管理资源。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++ 早期，类的设计者需要手动管理资源。当一个类持有裸指针或其他需要显式释放的资源句柄时：

1. **析构函数**：负责释放资源
2. **拷贝构造函数**：负责正确复制资源（而非仅复制句柄）
3. **拷贝赋值运算符**：负责正确赋值资源

如果只定义了其中一部分，编译器隐式生成的其他函数会导致**浅拷贝 (shallow copy)** 问题，造成资源泄漏或双重释放 (double free)。

### C++11 变化

C++11 引入了**移动语义 (move semantics)**，新增两个特殊成员函数：

4. **移动构造函数 (move constructor)**
5. **移动赋值运算符 (move assignment operator)**

这扩展了三法则为**五法则**。同时，如果用户定义了析构函数、拷贝构造函数或拷贝赋值运算符，编译器将不再隐式生成移动操作。

### 零法则的提出

**零法则 (Rule of Zero)** 由 R. Martinho Fernandes 于 2012 年提出，并被纳入 C++ Core Guidelines（准则 C.20：如果能避免定义默认操作，就避免定义）。

核心思想是：资源管理应该委托给专门的所有权类，业务类通过组合这些类来实现自动的资源管理，从而无需自定义任何特殊成员函数。

### C++ Core Guidelines 相关准则

| 准则编号 | 内容 |
|---------|------|
| C.20 | 如果能避免定义默认操作，就避免定义 |
| C.21 | 如果定义或删除了任何拷贝、移动或析构函数，就要定义或删除它们全部 |
| C.67 | 多态类应该禁止公开的拷贝/移动操作 |

## 3. 语法与参数 (Syntax and Parameters)

### 特殊成员函数

C++ 中有六个特殊成员函数（special member functions）：

| 函数 | 签名 | 说明 |
|------|------|------|
| 默认构造函数 | `T()` | 无参构造 |
| 析构函数 | `~T()` | 对象销毁时调用 |
| 拷贝构造函数 | `T(const T&)` | 拷贝初始化 |
| 拷贝赋值运算符 | `T& operator=(const T&)` | 拷贝赋值 |
| 移动构造函数 | `T(T&&)` (C++11) | 移动初始化 |
| 移动赋值运算符 | `T& operator=(T&&)` (C++11) | 移动赋值 |

### = default 和 = delete 语法

```cpp
class Example {
public:
    // 显式默认
    Example() = default;
    Example(const Example&) = default;
    Example(Example&&) = default;
    Example& operator=(const Example&) = default;
    Example& operator=(Example&&) = default;
    ~Example() = default;

    // 显式删除
    Example(const Example&) = delete;  // 禁止拷贝
    Example& operator=(const Example&) = delete;  // 禁止赋值
};
```

### 参数说明

- **const T&**：左值引用，用于拷贝操作
- **T&&**：右值引用，用于移动操作
- **noexcept**：移动操作通常声明为不抛出异常

## 4. 底层原理 (Underlying Principles)

### 隐式生成规则

编译器在以下条件下隐式生成特殊成员函数：

| 条件 | 析构函数 | 拷贝构造 | 拷贝赋值 | 移动构造 | 移动赋值 |
|------|---------|---------|---------|---------|---------|
| 用户未定义该函数 | 自动生成 | 自动生成 | 自动生成 | 自动生成* | 自动生成* |
| 用户定义了析构函数 | - | 自动生成 | 自动生成 | 不生成 | 不生成 |
| 用户定义了拷贝操作 | 自动生成 | - | 自动生成 | 不生成 | 不生成 |
| 用户定义了移动操作 | 自动生成 | 不生成 | 不生成 | - | - |

*注：移动操作仅在未定义任何析构、拷贝操作时隐式生成

### 浅拷贝问题

当类持有裸指针时，默认的拷贝操作仅复制指针值：

```
原始对象:    拷贝后:
┌─────────┐    ┌─────────┐
│ ptr ────┼───►│ ptr ────┼───► 同一内存块
└─────────┘    └─────────┘

结果：两个指针指向同一资源
析构时：双重释放 (double free)
```

### 深拷贝实现

正确的拷贝操作需要执行**深拷贝 (deep copy)**：

```
原始对象:    拷贝后:
┌─────────┐    ┌─────────┐
│ ptr ────┼───►│ ptr ────┼───► 新内存块（复制的内容）
└─────────┘    └─────────┘
     │               │
     ▼               ▼
  原内存块        独立副本
```

### 移动语义原理

移动操作通过**资源转移 (resource transfer)** 实现：

```cpp
// 移动构造函数：窃取资源
T(T&& other) noexcept : ptr(std::exchange(other.ptr, nullptr)) {}

// other 的资源被转移到新对象，other 变为空状态
```

### 异常安全

拷贝赋值运算符通常使用**拷贝交换惯用法 (copy-and-swap idiom)** 实现强异常安全保证：

```cpp
T& operator=(const T& other) {
    T temp(other);        // 拷贝构造，可能抛出异常
    swap(ptr, temp.ptr);  // 不抛出异常的交换
    return *this;
    // temp 析构时释放旧资源
}
```

## 5. 使用场景 (Use Cases)

### 三法则适用场景

| 场景 | 说明 |
|------|------|
| 管理动态内存 | 类持有 `new[]` 分配的数组 |
| 管理 C 风格资源 | 持有 `FILE*`、文件描述符、socket 等 |
| 封装第三方库句柄 | 需要调用特定释放函数的资源 |

### 五法则适用场景

在 C++11 及更高版本中，当需要三法则且希望优化性能时，应实现五法则：

| 场景 | 收益 |
|------|------|
| 容器元素移动 | 避免不必要的深拷贝 |
| 函数返回值 | 启用移动语义 |
| 临时对象处理 | 提高效率 |

### 零法则适用场景

| 场景 | 说明 |
|------|------|
| 业务逻辑类 | 不直接管理资源 |
| 使用标准容器 | `std::vector`、`std::string` 等已正确实现特殊成员函数 |
| 使用智能指针 | `std::unique_ptr`、`std::shared_ptr` 自动管理生命周期 |

### 最佳实践

1. **优先遵循零法则**：使用标准库资源管理类型
2. **必要时遵循五法则**：自定义资源管理类
3. **多态基类**：声明虚析构函数，显式默认或删除拷贝/移动操作
4. **不可复制的资源**：使用 `= delete` 禁止拷贝操作

### 常见陷阱

| 陷阱 | 后果 | 解决方案 |
|------|------|---------|
| 只定义析构函数 | 拷贝操作为浅拷贝 | 实现完整的五法则 |
| 忘记自赋值检查 | 自赋值导致资源泄漏 | 在赋值运算符中检查 `this == &other` |
| 移动后使用源对象 | 访问空指针 | 移动后源对象处于有效但未指定状态 |
| 虚析构函数阻止移动 | 移动操作被删除 | 显式 `= default` 移动操作 |

## 6. 代码示例 (Examples)

### 三法则示例

```cpp
#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>

class rule_of_three {
    char* cstring;  // 裸指针：动态分配内存的句柄

public:
    rule_of_three(const char* s, std::size_t n)
        : cstring(new char[n + 1])  // 分配资源
    {
        std::memcpy(cstring, s, n);
        cstring[n] = '\0';
    }

    explicit rule_of_three(const char* s = "")
        : rule_of_three(s, std::strlen(s)) {}

    // I. 析构函数：释放资源
    ~rule_of_three() {
        delete[] cstring;
    }

    // II. 拷贝构造函数：深拷贝
    rule_of_three(const rule_of_three& other)
        : rule_of_three(other.cstring) {}

    // III. 拷贝赋值运算符：拷贝交换惯用法
    rule_of_three& operator=(const rule_of_three& other) {
        if (this == &other)
            return *this;  // 自赋值检查

        rule_of_three temp(other);           // 使用拷贝构造函数
        std::swap(cstring, temp.cstring);    // 交换资源

        return *this;
        // temp 析构时自动释放旧资源
    }

    const char* c_str() const { return cstring; }
};

int main() {
    rule_of_three o1{"abc"};
    std::cout << o1.c_str() << ' ';

    auto o2{o1};  // 使用拷贝构造函数
    std::cout << o2.c_str() << ' ';

    rule_of_three o3{"def"};
    std::cout << o3.c_str() << ' ';

    o3 = o2;  // 使用拷贝赋值运算符
    std::cout << o3.c_str() << '\n';
}
// 输出: abc abc def abc
// 所有析构函数在此自动调用
```

### 五法则示例

```cpp
#include <cstddef>
#include <cstring>
#include <utility>

class rule_of_five {
    char* cstring;

public:
    rule_of_five(const char* s, std::size_t n)
        : cstring(new char[n + 1]) {
        std::memcpy(cstring, s, n);
        cstring[n] = '\0';
    }

    explicit rule_of_five(const char* s = "")
        : rule_of_five(s, std::strlen(s)) {}

    // I. 析构函数
    ~rule_of_five() {
        delete[] cstring;
    }

    // II. 拷贝构造函数
    rule_of_five(const rule_of_five& other)
        : rule_of_five(other.cstring) {}

    // III. 拷贝赋值运算符
    rule_of_five& operator=(const rule_of_five& other) {
        if (this == &other)
            return *this;

        rule_of_five temp(other);
        std::swap(cstring, temp.cstring);
        return *this;
    }

    // IV. 移动构造函数
    rule_of_five(rule_of_five&& other) noexcept
        : cstring(std::exchange(other.cstring, nullptr)) {}

    // V. 移动赋值运算符
    rule_of_five& operator=(rule_of_five&& other) noexcept {
        rule_of_five temp(std::move(other));
        std::swap(cstring, temp.cstring);
        return *this;
    }
};
```

### 零法则示例

```cpp
#include <string>
#include <vector>

// 零法则：无需定义任何特殊成员函数
class rule_of_zero {
    std::string cppstring;  // 标准库已正确实现所有特殊成员函数

public:
    // 无需自定义析构函数、拷贝/移动操作
    // 编译器生成的默认实现完全正确
};

// 组合使用智能指针
class resource_owner {
    std::unique_ptr<int[]> data;  // 独占所有权
    std::shared_ptr<std::string> name;  // 共享所有权

public:
    // 无需自定义任何特殊成员函数
    // unique_ptr 自动处理移动语义
    // shared_ptr 自动处理引用计数
};
```

### 多态基类示例

```cpp
// 多态基类：需要虚析构函数
class base_of_five_defaults {
public:
    base_of_five_defaults(const base_of_five_defaults&) = default;
    base_of_five_defaults(base_of_five_defaults&&) = default;
    base_of_five_defaults& operator=(const base_of_five_defaults&) = default;
    base_of_five_defaults& operator=(base_of_five_defaults&&) = default;

    virtual ~base_of_five_defaults() = default;  // 虚析构函数阻止隐式移动
};

// 或者禁止拷贝和移动（推荐用于多态类）
class polymorphic_base {
public:
    polymorphic_base() = default;
    virtual ~polymorphic_base() = default;

    // 禁止拷贝和移动，防止切片问题
    polymorphic_base(const polymorphic_base&) = delete;
    polymorphic_base& operator=(const polymorphic_base&) = delete;
    polymorphic_base(polymorphic_base&&) = delete;
    polymorphic_base& operator=(polymorphic_base&&) = delete;
};
```

### 常见错误及修正

#### 错误 1：只定义析构函数

```cpp
// 错误：只定义了析构函数
class bad_example {
    char* data;
public:
    bad_example(const char* s) : data(new char[std::strlen(s) + 1]) {
        std::strcpy(data, s);
    }
    ~bad_example() { delete[] data; }  // 只定义析构函数
    // 拷贝操作使用默认的浅拷贝！
};

// 修正：实现完整的五法则
class good_example {
    char* data;
public:
    good_example(const char* s);
    ~good_example();
    good_example(const good_example& other);
    good_example& operator=(const good_example& other);
    good_example(good_example&& other) noexcept;
    good_example& operator=(good_example&& other) noexcept;
};
```

#### 错误 2：忘记自赋值检查

```cpp
// 错误：自赋值会导致释放后使用
class bad_assignment {
    char* data;
public:
    bad_assignment& operator=(const bad_assignment& other) {
        delete[] data;  // 如果 this == &other，释放了自己的数据
        data = new char[std::strlen(other.data) + 1];
        std::strcpy(data, other.data);  // other.data 已被释放！
        return *this;
    }
};

// 修正：添加自赋值检查
class good_assignment {
    char* data;
public:
    good_assignment& operator=(const good_assignment& other) {
        if (this == &other)
            return *this;  // 自赋值检查
        delete[] data;
        data = new char[std::strlen(other.data) + 1];
        std::strcpy(data, other.data);
        return *this;
    }
};

// 更好：使用拷贝交换惯用法
class better_assignment {
    char* data;
public:
    better_assignment& operator=(const better_assignment& other) {
        better_assignment temp(other);  // 拷贝构造
        std::swap(data, temp.data);     // 交换
        return *this;  // temp 析构时释放旧资源
    }
};
```

## 7. 总结 (Summary)

### 核心要点

| 法则 | 适用版本 | 内容 |
|------|---------|------|
| 三法则 | C++98 | 需要析构函数、拷贝构造函数、拷贝赋值运算符之一时，需要全部三个 |
| 五法则 | C++11+ | 三法则 + 移动构造函数 + 移动赋值运算符 |
| 零法则 | C++11+ | 通过组合资源管理类，无需自定义任何特殊成员函数 |

### 决策指南

```
是否需要管理资源？
    ├── 否 → 使用零法则（默认实现即可）
    └── 是 → 资源是否可以用智能指针/标准容器管理？
                ├── 是 → 使用零法则（组合资源管理类）
                └── 否 → 使用五法则（自定义资源管理类）
```

### 技术对比

| 特性 | 零法则 | 三法则 | 五法则 |
|------|--------|--------|--------|
| 代码量 | 最少 | 中等 | 较多 |
| 性能 | 良好 | 基本拷贝 | 支持移动优化 |
| 安全性 | 高（使用标准库） | 需要仔细实现 | 需要仔细实现 |
| 适用场景 | 大多数业务类 | C++98 资源管理类 | C++11+ 资源管理类 |

### 学习建议

1. **优先使用零法则**：现代 C++ 中，大多数类不需要自定义特殊成员函数
2. **理解资源管理**：学习 `std::unique_ptr`、`std::shared_ptr` 的使用
3. **掌握拷贝交换惯用法**：实现强异常安全的赋值运算符
4. **注意多态类**：虚析构函数会阻止隐式移动操作

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/rule_of_three
- C++ Core Guidelines: C.20, C.21, C.67
- "Rule of Zero", R. Martinho Fernandes, 2012
- "A Concern about the Rule of Zero", Scott Meyers, 2014