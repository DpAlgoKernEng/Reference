# 注入类名 (Injected-class-name)

## 1. 概述

**注入类名 (injected-class-name)** 是 C++ 中一个重要的名称查找机制，指的是在类的作用域内，当前类的类名或类模板的模板名被视为一个公共成员名。这个名称被称为"注入"到类作用域中，因此可以在类的内部直接使用类名来引用该类本身。

注入类名的声明点紧随类（模板）定义的左花括号之后。

在类模板中，注入类名可以有两种用途：
- 作为**模板名 (template-name)**，引用当前模板本身
- 作为**类型名 (type-name)**，引用当前实例化类型

## 2. 来源与演变

### 首次引入

注入类名机制在 **C++98** 标准中首次引入，是 C++ 类作用域名称查找规则的核心组成部分。

### 设计动机

注入类名的设计解决了以下问题：

1. **简化类内自引用**：允许在类的成员函数中方便地使用类名声明指针或引用
2. **模板中的统一表示**：在类模板中，无需重复书写模板参数即可引用当前实例化
3. **CRTP 模式支持**：为奇异递归模板模式 (Curiously Recurring Template Pattern) 提供基础支持

### 缺陷报告修正

| DR | 应用版本 | 原始行为 | 修正行为 |
|----|----------|----------|----------|
| CWG 1004 | C++98 | 注入类名不能作为模板模板参数 | 允许使用，此时引用类模板本身 |
| CWG 2637 | C++98 | 整个 template-id 可以是注入类名 | 仅模板名可以是注入类名 |

## 3. 语法与声明

### 基本语法规则

在类作用域内，注入类名的行为如下：

```cpp
struct ClassName {
    // ClassName 在此处被"注入"
    // 可以在类内部直接使用 ClassName 来引用此类
};
```

### 类模板中的双重语义

在类模板中，注入类名有两种使用方式：

**作为模板名使用的情况：**

| 使用场景 | 说明 |
|----------|------|
| 后跟 `<` | `X<T1, T2>` 中的 X 被视为模板名 |
| 作为模板模板参数 | `A<X>` 中的 X 被视为模板名 |
| 友元类模板声明的最终标识符 | `friend class X;` 中的 X 被视为模板名 |

**作为类型名使用的情况：**

除此之外的所有其他情况，注入类名被视为类型名，等价于 `TemplateName<TemplateParameters>`。

### 声明点规则

注入类名的声明点位于类定义的左花括号之后：

```cpp
struct X {
    // X 在此处已被注入，可以在后续代码中使用
    void f();
};
```

## 4. 底层原理

### 名称查找机制

注入类名的查找遵循标准的名称查找规则：

1. **类作用域查找**：在类成员函数中查找非限定名称时，首先查找局部作用域，然后查找类作用域
2. **继承关系查找**：注入类名作为成员被继承，因此也可以在派生类中访问基类的注入类名

### 访问控制

注入类名的访问权限遵循继承的访问控制：

```cpp
struct A {};
struct B : private A {};
struct C : public B {
    A* p;   // 错误：注入类名 A 不可访问（私有继承）
    ::A* q; // 正确：使用全局作用域的 A，不使用注入类名
};
```

### 歧义解析

当从多个基类继承相同的注入类名时，可能产生歧义：

```cpp
template<class T>
struct Base {};

template<class T>
struct Derived: Base<int>, Base<char> {
    typename Derived::Base b;         // 错误：歧义
    typename Derived::Base<double> d; // 正确：明确指定模板参数
};
```

如果所有找到的注入类名都引用同一个类模板的特化，且作为模板名使用，则引用类模板本身，不产生歧义。

### 与构造函数的关系

构造函数本身没有名称，但在构造函数声明和定义中，外围类的注入类名被视为命名构造函数：

```cpp
struct A {
    A();           // 构造函数声明
};
A::A() {}          // 构造函数定义，A::A 被视为命名构造函数
```

## 5. 使用场景

### 场景一：类内自引用

```cpp
struct Node {
    Node* next;     // 使用注入类名声明指针
    Node& self();   // 使用注入类名声明引用
};
```

### 场景二：类模板内部简化类型表示

```cpp
template<typename T, typename Allocator>
class vector {
    vector* clone();  // 无需写 vector<T, Allocator>
};
```

### 场景三：继承中的注入类名使用

```cpp
template<class T>
struct Base {
    Base* p;  // Base 意味着 Base<T>
};

template<class T>
struct Derived : public Base<T*> {
    typename Derived::Base* p;  // Derived::Base 意味着 Base<T*>
};
```

### 场景四：友元声明

```cpp
template<class T1, class T2>
struct X {
    template<class U1, class U2>
    friend class X;  // X 被视为模板名
};
```

### 最佳实践

1. **在类模板中使用注入类名简化代码**：避免重复书写完整的模板参数列表
2. **注意私有继承的影响**：私有继承会使基类的注入类名在派生类中不可访问
3. **多重继承时注意歧义**：从多个基类继承相同名称时需要显式限定

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 全局名称遮蔽 | 如果全局作用域有同名的变量或函数，会遮蔽类名 |
| 私有继承访问限制 | 私有继承使基类注入类名不可访问 |
| 多重继承歧义 | 从多个基类继承同名注入类名时产生歧义 |
| 构造函数误用 | `A::A a;` 是错误的，A::A 被视为构造函数名 |

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

int X;  // 全局变量 X

struct X {
    void f() {
        X* p;    // 正确：X 是注入类名，指代 struct X
        // ::X* q; // 错误：::X 查找到的是全局变量，而非 struct
    }
};

int main() {
    X obj;
    obj.f();
    return 0;
}
```

### 类模板中的注入类名

```cpp
#include <iostream>

template<template<class, class> class>
struct A {};

template<class T1, class T2>
struct X {
    X<T1, T2>* p;   // 正确：X 被视为模板名

    using a = A<X>; // 正确：X 被视为模板名

    template<class U1, class U2>
    friend class X; // 正确：X 被视为模板名

    X* q;           // 正确：X 被视为类型名，等价于 X<T1, T2>
};

int main() {
    X<int, double> obj;
    return 0;
}
```

### 特化中的注入类名

```cpp
#include <iostream>

template<class T1, class T2>
struct X;

// 完全特化
template<>
struct X<void, void> {
    X* p;  // 正确：X 被视为类型名，等价于 X<void, void>

    template<class, class>
    friend class X;  // 正确：X 被视为模板名

    X<void, void>* q; // 正确：X 被视为模板名
};

// 偏特化
template<class T>
struct X<char, T> {
    X* p, q;  // 正确：X 被视为类型名，等价于 X<char, T>

    using r = X<int, int>; // 正确：可用于命名其他特化
};

int main() {
    X<void, void> a;
    X<char, int> b;
    return 0;
}
```

### 继承中的注入类名

```cpp
#include <iostream>

struct A {};
struct B : private A {};

struct C : public B {
    // A* p;    // 错误：注入类名 A 不可访问（私有继承）
    ::A* q;      // 正确：使用全局作用域解析，不使用注入类名
};

// 正确用法示例
template<class T>
struct Base {
    Base* p;  // Base 意味着 Base<T>
};

template<class T>
struct Derived : public Base<T*> {
    typename Derived::Base* p;  // Derived::Base 意味着 Base<T*>
};

int main() {
    C obj;
    ::A* q = nullptr;
    return 0;
}
```

### 构造函数与注入类名

```cpp
#include <iostream>

struct A {
    A();
    A(int);

    template<class T>
    A(T) {}
};

using A_alias = A;

A::A() {}              // 正确：A::A 命名构造函数
A_alias::A(int) {}     // 正确：A_alias::A 命名构造函数
template A::A(double); // 正确：显式实例化

struct B : A {
    using A_alias::A;  // 正确：继承构造函数
};

int main() {
    // A::A a;         // 错误：A::A 被视为命名构造函数，而非类型
    struct A::A a2;    // 正确：等价于 'A a2;'
    B::A b;            // 正确：等价于 'A b;'
    return 0;
}
```

### 默认模板参数中的注入类名

```cpp
#include <iostream>

template<class T>
struct Base {};

template<class T, template<class> class U = T::template Base>
struct Third {};

template<class T>
struct Derived : public Base<T*> {
    // Derived::Base 意味着 Base<T*>
};

int main() {
    Third<Derived<int>> t;  // 正确：默认参数使用注入类名作为模板
    return 0;
}
```

### 常见错误及修正

#### 错误 1：全局名称遮蔽类名

```cpp
// 错误示例
int X;  // 全局变量

struct X {
    void f() {
        ::X* p;  // 错误：::X 查找到全局变量 int X
    }
};

// 修正方法
struct Y {
    void f() {
        Y* p;  // 正确：使用注入类名
    }
};
```

#### 错误 2：私有继承导致注入类名不可访问

```cpp
// 错误示例
struct A {};
struct B : private A {};
struct C : public B {
    A* p;  // 错误：注入类名 A 不可访问
};

// 修正方法
struct C_correct : public B {
    ::A* p;  // 正确：使用全局作用域的 A
};
```

#### 错误 3：多重继承歧义

```cpp
// 错误示例
template<class T>
struct Base {};

template<class T>
struct Derived : Base<int>, Base<char> {
    typename Derived::Base b;  // 错误：歧义
};

// 修正方法
template<class T>
struct Derived_correct : Base<int>, Base<char> {
    typename Derived::Base<double> d;  // 正确：明确指定模板参数
};
```

#### 错误 4：误用构造函数名称

```cpp
// 错误示例
struct A {
    A() {}
};

A::A a;  // 错误：A::A 被视为命名构造函数

// 修正方法
struct A::A a2;  // 正确：在详细类型说明符中，A::A 是类型名
A a3;            // 正确：直接使用类名
```

## 7. 总结

### 核心要点

注入类名是 C++ 类作用域中的一个重要概念，其核心特性包括：

1. **自动注入**：类名在类定义开始处自动注入到类作用域中
2. **双重语义**：在类模板中可同时作为模板名和类型名使用
3. **继承传递**：作为成员被派生类继承
4. **构造函数关联**：在特定上下文中用于命名构造函数

### 使用规则速查表

| 场景 | 注入类名的含义 |
|------|----------------|
| `X* p;` 在类 X 内 | 类型名，指向当前类 |
| `X<T>` 在类模板 X 内 | 模板名 |
| `A<X>` 在类模板 X 内 | 模板名（作为模板模板参数） |
| `friend class X;` | 模板名 |
| `X::X` | 构造函数名 |
| `struct X::A a;` | 类型名（详细类型说明符中） |

### 设计意义

注入类名机制简化了 C++ 类的自引用语法，特别是在模板编程中，避免了重复书写冗长的模板参数列表。这一设计体现了 C++ 追求表达力和简洁性的理念。

### 注意事项

1. 当全局作用域存在同名实体时，使用 `::` 限定符会查找全局名称而非注入类名
2. 私有继承会使基类的注入类名在派生类中不可访问
3. 在多重继承中，从不同基类继承相同注入类名可能产生歧义
4. `Class::Class` 形式通常表示构造函数而非类型

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/injected-class-name
- C++ Standard: [class.qual], [class.member.scope]
- C++ Templates: The Complete Guide, David Vandevoorde et al.