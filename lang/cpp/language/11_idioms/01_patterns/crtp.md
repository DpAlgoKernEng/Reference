# CRTP - 奇异递归模板模式 (Curiously Recurring Template Pattern)

## 1. 概述 (Overview)

CRTP（Curiously Recurring Template Pattern，奇异递归模板模式）是 C++ 中一种独特的模板元编程技术。其核心特点是：类 `X` 继承自模板类 `Y`，而 `Y` 以 `X` 作为模板参数实例化。即 `class X : public Y<X>`。

CRTP 的主要用途包括：
- **编译期多态**（compile-time polymorphism）：避免虚函数的运行时开销
- **静态接口设计**：在基类中定义接口，派生类实现具体行为
- **代码复用**：在基类中实现通用逻辑，通过类型参数调用派生类方法

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

CRTP 的名称源于 James Coplien 在 1995 年出版的《Advanced C++ Programming Styles and Idioms》一书中的描述。这种模式"奇异"之处在于：派生类在定义时就将自己作为基类的模板参数，形成了一种看似递归的结构。

### 设计动机

传统运行时多态（虚函数）存在以下问题：
- **运行时开销**：虚函数调用需要通过虚表（vtable）进行间接寻址
- **内存开销**：每个对象需要存储虚表指针（vptr）
- **内联限制**：虚函数调用难以被编译器内联优化

CRTP 通过模板在编译期解析函数调用，解决了这些问题：
- 零运行时开销
- 无额外内存占用
- 支持编译期内联

### C++23 演进

C++23 引入了显式对象成员函数（Explicit Object Member Functions，又称 deducing `this`），提供了更简洁的 CRTP 替代语法：

```cpp
// C++23 新语法
struct Base {
    void name(this auto&& self) { self.impl(); }
};
```

这种新语法消除了传统 CRTP 的模板样板代码，同时保留了编译期多态的优势。

## 3. 语法与参数 (Syntax and Parameters)

### 基本模板定义

```cpp
template<class Z>
class Y {};

class X : public Y<X> {};
```

### 传统 CRTP 模式

```cpp
template <class Derived>
struct Base {
    void name() {
        static_cast<Derived*>(this)->impl();
    }
protected:
    Base() = default;  // 禁止直接创建 Base 对象
};

struct D1 : public Base<D1> {
    void impl() { /* 实现 */ }
};
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `Derived` | 派生类类型，作为基类模板参数传递 |
| `Base<Derived>` | 以派生类实例化的基类模板 |

### 关键语法元素

| 语法 | 说明 |
|------|------|
| `static_cast<Derived*>(this)` | 将基类指针向下转型为派生类指针 |
| `protected` 构造函数 | 防止直接实例化基类 |
| `Base() = default` | 显式声明默认构造函数为保护成员 |

### C++23 新语法

```cpp
struct Base {
    void name(this auto&& self) {
        self.impl();  // 直接调用，无需转型
    }
};
```

| 语法 | 说明 |
|------|------|
| `this auto&& self` | 显式对象参数，编译器自动推导类型 |
| `self.impl()` | 直接调用派生类方法 |

## 4. 底层原理 (Underlying Principles)

### 编译期多态机制

CRTP 通过模板实例化和静态类型转换实现编译期多态：

```
编译期阶段：
1. 模板实例化：Base<D1> 和 Base<D2> 生成不同的类定义
2. 函数调用解析：编译器知道 static_cast<Derived*>(this) 的具体类型
3. 内联优化：由于类型确定，编译器可以内联调用 impl()
```

### 与虚函数对比

| 特性 | 虚函数 | CRTP |
|------|--------|------|
| 调用时机 | 运行时 | 编译期 |
| 间接寻址 | 需要（虚表） | 不需要 |
| 内存开销 | vptr（通常 8 字节） | 无 |
| 内联可能性 | 低 | 高 |
| 代码体积 | 小 | 可能增大（模板膨胀） |
| 灵活性 | 运行时多态 | 编译期多态 |

### 静态转型安全性

```cpp
void name() {
    static_cast<Derived*>(this)->impl();
}
```

这种向下转型（downcast）在 CRTP 中是安全的，原因：
- `Derived` 继承自 `Base<Derived>`
- `this` 指针始终指向 `Derived` 对象
- 编译期类型检查确保正确性

**注意**：如果错误继承（如 `class Wrong : public Base<OtherClass>`），会导致未定义行为。

### 保护构造函数的作用

```cpp
protected:
    Base() = default;
```

这确保：
- 派生类可以正常构造
- 用户无法直接创建 `Base<SomeType>` 实例
- 防止错误的向上转型

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 性能关键代码 | 消除虚函数开销 |
| 静态接口定义 | 定义派生类必须实现的方法 |
| 代码复用 | 基类提供通用实现，调用派生类特定行为 |
| 表达式模板 | 实现高效的数学运算库 |
| Mixin 模式 | 将功能注入到派生类中 |

### 经典应用案例

#### 1. 计数器 Mixin

```cpp
template <class Derived>
struct Counter {
    static inline int count = 0;
    Counter() { ++count; }
    Counter(const Counter&) { ++count; }
    ~Counter() { --count; }
};

class Widget : public Counter<Widget> {};
class Gadget : public Counter<Gadget> {};

// Widget::count 和 Gadget::count 是独立的计数器
```

#### 2. 可比较对象

```cpp
template <class Derived>
struct Comparable {
    bool operator!=(const Derived& other) const {
        return !static_cast<const Derived*>(this)->operator==(other);
    }
};

class Point : public Comparable<Point> {
public:
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    int x, y;
};
```

### 不适用场景

| 场景 | 原因 | 替代方案 |
|------|------|---------|
| 运行时多态需求 | CRTP 是编译期的 | 虚函数或 `std::variant` |
| 类型擦除需求 | 需要在容器中存储异构对象 | `std::function` 或虚函数 |
| 运行时动态绑定 | 类型必须在编译期确定 | 虚函数 |

### 注意事项

1. **继承正确性**：确保 `class D : public Base<D>` 而非 `class D : public Base<OtherClass>`
2. **构造函数保护**：基类构造函数应为 `protected` 以防止误用
3. **代码膨胀**：大量 CRTP 类可能导致模板实例化膨胀
4. **调试困难**：模板错误信息可能难以理解

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <cstdio>

// 传统 CRTP 语法
template <class Derived>
struct Base {
    void name() {
        static_cast<Derived*>(this)->impl();
    }
protected:
    Base() = default;  // 防止直接创建 Base 对象
};

struct D1 : public Base<D1> {
    void impl() { std::puts("D1::impl()"); }
};

struct D2 : public Base<D2> {
    void impl() { std::puts("D2::impl()"); }
};

int main() {
    D1 d1; d1.name();  // 输出: D1::impl()
    D2 d2; d2.name();  // 输出: D2::impl()
    return 0;
}
```

### C++23 新语法

```cpp
#include <cstdio>

// C++23 deducing-this 语法
struct Base {
    void name(this auto&& self) {
        self.impl();
    }
};

struct D1 : public Base {
    void impl() { std::puts("D1::impl()"); }
};

struct D2 : public Base {
    void impl() { std::puts("D2::impl()"); }
};

int main() {
    D1 d1; d1.name();  // 输出: D1::impl()
    D2 d2; d2.name();  // 输出: D2::impl()
    return 0;
}
```

### 高级用法：表达式模板

```cpp
#include <iostream>
#include <vector>

// CRTP 表达式模板实现向量运算优化
template <class E>
struct VecExpr {
    double operator[](size_t i) const {
        return static_cast<const E&>(*this)[i];
    }

    size_t size() const {
        return static_cast<const E&>(*this).size();
    }
};

class Vec : public VecExpr<Vec> {
    std::vector<double> data;
public:
    Vec(std::initializer_list<double> l) : data(l) {}
    Vec(size_t n) : data(n) {}

    double operator[](size_t i) const { return data[i]; }
    double& operator[](size_t i) { return data[i]; }
    size_t size() const { return data.size(); }

    template <class E>
    Vec& operator=(const VecExpr<E>& expr) {
        for (size_t i = 0; i < data.size(); ++i)
            data[i] = expr[i];
        return *this;
    }
};

template <class E1, class E2>
struct VecSum : public VecExpr<VecSum<E1, E2>> {
    const E1& v1;
    const E2& v2;

    VecSum(const E1& a, const E2& b) : v1(a), v2(b) {}

    double operator[](size_t i) const { return v1[i] + v2[i]; }
    size_t size() const { return v1.size(); }
};

template <class E1, class E2>
VecSum<E1, E2> operator+(const VecExpr<E1>& a, const VecExpr<E2>& b) {
    return VecSum<E1, E2>(static_cast<const E1&>(a), static_cast<const E2&>(b));
}

int main() {
    Vec a{1.0, 2.0, 3.0};
    Vec b{4.0, 5.0, 6.0};
    Vec c(3);

    c = a + b;  // 无临时对象，惰性求值
    // 等价于: c[i] = a[i] + b[i] for each i

    std::cout << c[0] << ", " << c[1] << ", " << c[2] << std::endl;
    // 输出: 5, 7, 9
    return 0;
}
```

### 常见错误及修正

#### 错误 1：错误的模板参数

```cpp
// 错误：派生类使用了错误的模板参数
class D1 : public Base<D1> { /* 正确 */ };
class D2 : public Base<D1> { /* 错误！D2 继承了 Base<D1> */ };

// 修正：每个派生类使用自己的类型
class D2 : public Base<D2> { /* 正确 */ };
```

#### 错误 2：忘记实现派生类方法

```cpp
template <class Derived>
struct Base {
    void name() {
        static_cast<Derived*>(this)->impl();  // 调用派生类的 impl()
    }
};

struct D : public Base<D> {
    // 忘记实现 impl()，编译通过但运行时可能崩溃或行为异常
};

// 修正：使用 static_assert 或概念约束
template <class Derived>
struct Base {
    void name() {
        static_cast<Derived*>(this)->impl();
    }
protected:
    Base() {
        static_assert(requires(Derived d) { d.impl(); },
                     "Derived must implement impl()");
    }
};
```

#### 错误 3：直接实例化基类

```cpp
template <class Derived>
struct Base {
    void name() { static_cast<Derived*>(this)->impl(); }
    // 错误：没有保护构造函数
};

Base<int> b;  // 可以创建，但调用 name() 会导致未定义行为

// 修正：保护构造函数
template <class Derived>
struct Base {
    void name() { static_cast<Derived*>(this)->impl(); }
protected:
    Base() = default;  // 只允许派生类调用
};
```

## 7. 总结 (Summary)

### 核心要点

CRTP 是 C++ 模板元编程的重要技巧，其核心价值在于：

| 特性 | 说明 |
|------|------|
| 编译期多态 | 消除虚函数运行时开销 |
| 零成本抽象 | 符合 C++ "零开销原则" |
| 类型安全 | 编译期检查确保正确性 |
| 灵活复用 | 基类可调用派生类方法 |

### 技术对比

| 特性 | CRTP | 虚函数 | C++23 deducing-this |
|------|------|--------|---------------------|
| 编译期绑定 | 是 | 否 | 是 |
| 运行时开销 | 无 | 有 | 无 |
| 代码复杂度 | 较高 | 低 | 低 |
| 最低版本要求 | C++98 | C++98 | C++23 |
| 运行时多态 | 不支持 | 支持 | 不支持 |

### 学习建议

1. **理解模板实例化**：CRTP 依赖模板为每个派生类生成独立的基类代码
2. **掌握静态转型**：`static_cast<Derived*>(this)` 是 CRTP 的核心技巧
3. **注意安全性**：使用 `protected` 构造函数防止误用
4. **关注 C++23**：学习显式对象参数语法，简化 CRTP 实现

### 相关概念

| 概念 | 关系 |
|------|------|
| `std::enable_shared_from_this` | 使用 CRTP 实现 `shared_from_this()` |
| `std::ranges::view_interface` | C++20 使用 CRTP 简化视图实现 |
| 表达式模板 | CRTP 的高级应用，实现惰性求值 |
| Mixin 模式 | CRTP 实现代码注入的常见方式 |