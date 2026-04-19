# `virtual` 函数说明符

## 1. 概述

`virtual` 说明符（specifier）指定一个非静态成员函数为**虚函数**（virtual function），并支持动态分派（dynamic dispatch）。它只能出现在非静态成员函数初始声明的 decl-specifier-seq 中（即在类定义中声明时）。

虚函数是可以在派生类中被重写（override）的成员函数。与非虚函数不同，即使编译时不知道类的实际类型，重写行为也会被保留。这意味着，如果通过指向基类的指针或引用来处理派生类对象，调用被重写的虚函数时将执行派生类中定义的行为。这种函数调用被称为**虚函数调用**（virtual function call）或**虚调用**（virtual call）。

`virtual` 关键字定义在 C++ 语言核心中，是实现运行时多态的核心机制。

## 2. 来源与演变

### 首次引入

`virtual` 关键字在 **C++98** 标准中首次引入，是 C++ 实现面向对象编程三大特性之一——多态（polymorphism）的核心机制。

### 历史背景

在虚函数出现之前，面向对象编程主要依赖：

1. **静态绑定**：函数调用在编译时确定，无法实现运行时多态
2. **函数指针**：需要手动管理，使用复杂且容易出错
3. **C 风格多态**：通过 union 或 void* 实现，类型不安全

虚函数的出现解决了这些问题，提供了：
- 自动化的动态分派机制
- 类型安全的运行时多态
- 简洁直观的语法形式

### C++11 变化

- 新增 `override` 说明符：显式声明函数重写基类虚函数
- 新增 `final` 说明符：禁止函数被进一步重写或类被继承
- 明确虚函数调用的定义（CWG 1516）

### C++20 变化

- 虚函数不能有关联约束（requires 子句）
- `consteval` 虚函数有特殊限制：不能与非 `consteval` 虚函数相互重写

### 缺陷报告

| DR | 应用版本 | 原行为 | 修正行为 |
|----|----------|--------|----------|
| CWG 258 | C++98 | 派生类的非 const 成员函数可能因基类 const 虚函数变为虚函数 | 虚函数特性要求 cv 限定符相同 |
| CWG 477 | C++98 | 友元声明可以包含 virtual 说明符 | 不允许 |
| CWG 1516 | C++98 | 未提供"虚函数调用"和"虚调用"的定义 | 已提供定义 |

## 3. 语法与参数

### 基本语法

```cpp
class Base {
public:
    virtual return_type function_name(parameter_list);  // 声明虚函数
};
```

### 虚函数声明规则

| 规则 | 说明 |
|------|------|
| 只能用于非静态成员函数 | 非成员函数和静态成员函数不能是虚函数 |
| 只能在初始声明中使用 | 只能在类定义中声明，不能在类外定义时添加 |
| 函数模板不能是虚函数 | 模板函数本身不能是虚函数，但类模板的普通成员函数可以 |

### 重写条件

派生类函数要重写基类虚函数，必须满足以下条件：

1. **函数名相同**
2. **参数类型列表相同**（返回类型可以不同，见协变返回类型）
3. **cv 限定符相同**（const/volatile）
4. **引用限定符相同**（&/&&）

满足以上条件后，派生类函数自动成为虚函数（无论是否使用 `virtual` 关键字），并重写基类函数。

### `override` 说明符（C++11）

```cpp
struct Derived : Base {
    void f() override;  // 显式声明重写基类虚函数
};
```

- 如果函数声明了 `override` 但没有重写任何虚函数，程序非法
- 推荐使用 `override` 以提高代码可读性和编译期检查

### `final` 说明符（C++11）

```cpp
struct Base {
    virtual void f() final;  // 禁止进一步重写
};

struct Derived final : Base {  // 禁止类被继承
};
```

- 如果函数声明为 `final`，其他函数尝试重写它时程序非法
- `final` 也可以用于类，表示该类不能被继承

## 4. 底层原理

### 动态分派机制

虚函数通过**虚函数表**（virtual function table，简称 vtable）实现动态分派：

1. **虚函数表**：每个包含虚函数的类都有一个虚函数表，存储虚函数的指针
2. **虚表指针**：每个对象包含一个指向其类虚函数表的指针（vptr）
3. **运行时查找**：调用虚函数时，通过 vptr 查找 vtable，获取正确的函数地址

### 最终覆盖者（Final Overrider）

对于每个虚函数，存在一个**最终覆盖者**，在虚函数调用时执行：

- 如果派生类没有声明或继承重写该虚函数的函数，则基类的虚函数是最终覆盖者
- 如果派生类声明或继承了重写函数，则该函数成为最终覆盖者
- 如果一个虚函数有多个最终覆盖者，程序非法（歧义）

### 虚函数调用抑制

如果使用**限定名称查找**调用函数（即函数名出现在作用域解析运算符 `::` 右侧），虚函数调用被抑制：

```cpp
Base* ptr = &derived;
ptr->Base::f();  // 非虚调用，直接调用 Base::f()
```

### 协变返回类型（Covariant Return Types）

如果派生类函数重写基类函数，返回类型可以不同，但必须是**协变**的：

满足协变的条件：
1. 两个类型都是指向类的指针或引用（支持多级指针或引用）
2. 派生类返回类型所指向的类，必须是基类返回类型所指向的类的直接或间接基类
3. 派生类返回类型的 cv 限定不能比基类返回类型更严格

```cpp
class B {};
struct Base {
    virtual B* f();
};
struct Derived : Base {
    B* f();     // OK: 返回类型相同
    // D* f();  // OK: 如果 D 继承自 B，则返回类型协变
};
```

### 时间复杂度

| 操作 | 时间复杂度 | 说明 |
|------|-----------|------|
| 虚函数调用 | O(1) | 通过 vtable 索引查找 |
| 非虚函数调用 | O(1) | 直接调用，编译时确定 |
| 对象大小 | +1 指针 | 每个多态对象包含 vptr |

### 性能影响

- **内存开销**：每个多态对象包含一个 vptr（通常一个指针大小）
- **调用开销**：虚函数调用比普通函数调用多一次指针解引用
- **内联限制**：虚函数通常无法内联（除非编译器能确定具体类型）

## 5. 使用场景

### 适合使用虚函数的场景

| 场景 | 说明 |
|------|------|
| 运行时多态 | 需要根据实际对象类型选择行为 |
| 接口设计 | 定义抽象接口供派生类实现 |
| 插件架构 | 允许运行时加载不同实现 |
| 策略模式 | 通过基类指针调用不同策略 |

### 虚析构函数

当类可能被继承，且需要通过基类指针删除派生类对象时，基类析构函数必须声明为虚函数：

```cpp
class Base {
public:
    virtual ~Base() { /* 释放 Base 资源 */ }
};

class Derived : public Base {
    ~Derived() { /* 释放 Derived 资源 */ }
};

Base* ptr = new Derived;
delete ptr;  // 调用 Derived::~Derived()，然后 Base::~Base()
```

**重要规则**：如果基类析构函数不是虚函数，通过基类指针删除派生类对象是**未定义行为**。

### 构造和析构期间的行为

在构造或析构期间调用虚函数时：
- 调用的是当前构造函数或析构函数所在类的最终覆盖者
- 派生类尚未构造或已经析构，因此不会调用派生类的重写版本

```cpp
struct Base {
    Base() { f(); }  // 调用 Base::f()，不是 Derived::f()
    virtual void f() { std::cout << "Base\n"; }
};

struct Derived : Base {
    void f() override { std::cout << "Derived\n"; }
};

Derived d;  // 输出 "Base"，而非 "Derived"
```

### 私有虚函数

基类虚函数不需要可访问或可见就能被重写：

```cpp
class B {
    virtual void do_f();  // 私有虚函数
public:
    void f() { do_f(); }  // 公共接口
};

struct D : public B {
    void do_f() override;  // 重写 B::do_f
};
```

### 最佳实践

1. **使用 `override` 说明符**：确保确实重写了基类函数，避免拼写错误
2. **基类析构函数声明为虚函数**：如果类可能被继承
3. **避免在构造/析构函数中调用虚函数**：此时多态行为受限
4. **优先使用接口继承**：基类可以只声明纯虚函数
5. **谨慎使用多重继承**：可能导致多个最终覆盖者的歧义

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 参数签名不匹配 | 派生类函数隐藏而非重写基类函数 |
| 忘记 `virtual` | 派生类函数重写但不声明，可读性差 |
| 构造函数中调用虚函数 | 不会调用派生类版本 |
| 通过值调用虚函数 | 对象切片，失去多态性 |

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

struct Base {
    virtual void f() {
        std::cout << "base\n";
    }
};

struct Derived : Base {
    void f() override {  // 'override' 是可选的
        std::cout << "derived\n";
    }
};

int main() {
    Base b;
    Derived d;

    // 通过引用调用虚函数
    Base& br = b;  // br 的类型是 Base&
    Base& dr = d;  // dr 的类型也是 Base&
    br.f();         // 输出 "base"
    dr.f();         // 输出 "derived"

    // 通过指针调用虚函数
    Base* bp = &b;  // bp 的类型是 Base*
    Base* dp = &d;  // dp 的类型也是 Base*
    bp->f();        // 输出 "base"
    dp->f();        // 输出 "derived"

    // 非虚函数调用（限定名称查找）
    br.Base::f();   // 输出 "base"
    dr.Base::f();   // 输出 "base"

    return 0;
}
```

### 最终覆盖者

```cpp
#include <iostream>

struct A { virtual void f(); };     // A::f 是虚函数
struct B : A { void f(); };         // B::f 在 B 中重写 A::f
struct C : virtual B { void f(); }; // C::f 在 C 中重写 A::f

struct D : virtual B {};  // D 不引入重写者，B::f 在 D 中是最终覆盖者

struct E : C, D           // E 不引入重写者，C::f 在 E 中是最终覆盖者
{
    using A::f;  // 不是函数声明，只是让 A::f 对查找可见
};

int main() {
    E e;
    e.f();      // 虚调用，调用 C::f（E 中的最终覆盖者）
    e.E::f();   // 非虚调用，调用 A::f（在 E 中可见）

    return 0;
}
```

### 协变返回类型

```cpp
#include <iostream>

class B {};

struct Base {
    virtual void vf1();
    virtual void vf2();
    virtual void vf3();
    virtual B* vf4();
    virtual B* vf5();
};

class D : private B {
    friend struct Derived;  // 在 Derived 中，B 是 D 的可访问基类
};

struct Derived : public Base {
    void vf1();       // 虚函数，重写 Base::vf1()
    void vf2(int);    // 非虚函数，隐藏 Base::vf2()
    D* vf4();         // 重写 Base::vf4()，返回类型协变
};

int main() {
    Derived d;
    Base& br = d;

    br.vf1();  // 调用 Derived::vf1()
    br.vf2();  // 调用 Base::vf2()

    B* p = br.vf4();  // 调用 Derived::vf4()，结果转换为 B*

    return 0;
}
```

### 虚析构函数

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() {
        std::cout << "Base destructor\n";
    }
};

class Derived : public Base {
public:
    ~Derived() {
        std::cout << "Derived destructor\n";
    }
};

int main() {
    Base* b = new Derived;
    delete b;  // 虚函数调用，调用 Derived::~Derived()，然后 Base::~Base()
    // 输出:
    // Derived destructor
    // Base destructor

    return 0;
}
```

### 常见错误及修正

#### 错误 1：参数签名不匹配导致隐藏而非重写

```cpp
// 错误：参数列表不同，D::f(int) 隐藏 B::f()
struct B {
    virtual void f();
};

struct D : B {
    void f(int);  // D::f(int) 隐藏 B::f()（错误的参数列表）
};

struct D2 : D {
    void f();  // D2::f() 重写 B::f()（即使它不可见）
};

int main() {
    D d;
    B& d_as_b = d;
    d_as_b.f();  // 调用 B::f()（不是 D 的版本）

    // d.f();     // 错误：在 D 中查找只找到 f(int)

    return 0;
}

// 修正：使用相同的参数签名
struct D_correct : B {
    void f() override;  // 正确重写 B::f()
};
```

#### 错误 2：忘记虚析构函数

```cpp
// 错误：基类析构函数不是虚函数
class Base {
public:
    ~Base() { std::cout << "Base destructor\n"; }
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "Derived destructor\n"; }
};

int main() {
    Base* b = new Derived;
    delete b;  // 未定义行为！可能只调用 Base::~Base()
    return 0;
}

// 修正：基类析构函数声明为虚函数
class Base_fixed {
public:
    virtual ~Base_fixed() { std::cout << "Base destructor\n"; }
};
```

#### 错误 3：多重继承导致歧义

```cpp
// 错误：多个最终覆盖者导致程序非法
struct A {
    virtual void f();
};

struct VB1 : virtual A {
    void f();  // 重写 A::f
};

struct VB2 : virtual A {
    void f();  // 重写 A::f
};

// struct Error : VB1, VB2 {};  // 错误：A::f 在 Error 中有两个最终覆盖者

// 修正：在派生类中提供唯一重写
struct Okay : VB1, VB2 {
    void f();  // OK：这是 A::f 的最终覆盖者
};
```

#### 错误 4：`override` 说明符误用

```cpp
struct B {
    virtual void f(int);
};

struct D : B {
    virtual void f(int) override;   // OK：D::f(int) 重写 B::f(int)
    // virtual void f(long) override;  // 错误：f(long) 不重写 B::f(int)
};
```

#### 错误 5：`final` 说明符被重写

```cpp
struct B {
    virtual void f() const final;
};

struct D : B {
    // void f() const;  // 错误：D::f 试图重写 final B::f
};
```

## 7. 总结

`virtual` 说明符是 C++ 实现运行时多态的核心机制。它提供了：

- **动态分派**：根据对象实际类型选择正确的函数实现
- **接口抽象**：支持抽象基类和纯虚函数
- **类型安全**：编译时检查重写正确性（配合 `override`）
- **灵活设计**：支持协变返回类型

核心使用建议：

1. **基类析构函数声明为虚函数**：如果类可能被继承且需要通过基类指针删除对象
2. **使用 `override` 说明符**：确保正确重写基类函数，提高代码可读性
3. **避免在构造/析构函数中调用虚函数**：此时多态行为受限
4. **谨慎使用多重继承**：避免多个最终覆盖者的歧义
5. **理解性能影响**：虚函数调用有轻微开销，对象大小增加一个指针

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/virtual
- C++ Standard: [class.virtual]
- Effective C++, Scott Meyers, Item 7: Declare destructors virtual in polymorphic base classes