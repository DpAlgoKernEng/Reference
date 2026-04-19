# 限定名查找 (Qualified Name Lookup)

## 1. 概述 (Overview)

**限定名 (Qualified Name)** 是指出现在作用域解析运算符 `::` 右侧的名称。限定名查找是一种在特定作用域中查找名称的机制，它明确指定了查找的目标范围。

限定名可以引用以下实体：
- **类成员** - 包括静态和非静态函数、类型、模板等
- **命名空间成员** - 包括嵌套的命名空间
- **枚举器 (Enumerator)** - 枚举类型的成员

与非限定名查找 (Unqualified Name Lookup) 不同，限定名查找的范围是明确指定的，不会受到当前作用域中同名实体的干扰，这使得它成为访问被隐藏名称的重要手段。

## 2. 来源与演变 (Origin and Evolution)

### 设计动机

限定名查找机制的设计旨在解决以下问题：

1. **名称隐藏问题** - 当局部变量或嵌套作用域中的实体隐藏了外部作用域的名称时，需要一种机制来访问被隐藏的名称
2. **明确的作用域指定** - 在大型程序中，明确指定名称所属的作用域可以提高代码的可读性和可维护性
3. **多继承和命名空间冲突** - 当多个基类或命名空间包含同名成员时，限定名提供了明确的访问方式

### 标准演进

| C++ 版本 | 变更内容 |
|---------|---------|
| C++98 | 初始定义，要求 `::` 左侧的名称必须是类名或命名空间名 |
| CWG 215 | 修正：允许模板参数出现在 `::` 左侧（用于依赖类型） |
| CWG 318 | 修正：`A::A` 形式的限定名只在特定上下文中表示构造函数 |

### 缺陷报告详解

**CWG 215**：C++98 标准最初规定 `::` 左侧的名称必须是类名或命名空间名，这导致模板参数不能出现在 `::` 左侧。修正后允许指定类、命名空间或依赖类型的名称。

**CWG 318**：C++98 标准规定当 `::` 右侧的名称与左侧相同时，该限定名总是被视为构造函数。修正后规定只在可接受的上下文（如构造函数声明）中才表示构造函数，而在详述类型说明符等上下文中则表示注入类名 (injected-class-name)。

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
// 全局命名空间访问
::name                           // 在全局命名空间中查找 name

// 类成员访问
class-name::member-name          // 在指定类中查找成员
nested-class::member-name        // 在嵌套类中查找成员

// 命名空间成员访问
namespace-name::member-name      // 在指定命名空间中查找成员
::namespace-name::member-name    // 从全局命名空间开始查找
```

### 查找规则概要

#### 3.1 全局命名空间查找

当 `::` 左侧没有内容时，查找只在全局命名空间中进行：

```cpp
::variable_name    // 明确访问全局变量
::function_name()  // 明确调用全局函数
::Classname        // 明确引用全局类
```

#### 3.2 左侧名称的查找

在查找 `::` 右侧的名称之前，必须先完成左侧名称的查找（除非左侧是 `decltype` 表达式或为空）。左侧查找考虑的实体类型：
- 命名空间
- 类类型
- 枚举类型
- 特化为类型的模板

#### 3.3 析构函数的特殊规则

当 `::` 后紧跟 `~` 和标识符（指定析构函数或伪析构函数）时，该标识符在与 `::` 左侧名称相同的作用域中查找。

## 4. 底层原理 (Underlying Principles)

### 4.1 类成员查找机制

当左侧名称指定一个类/结构体/联合体时，右侧名称在该类的作用域中查找：

```cpp
struct Base {
    int value;
    void foo();
};

struct Derived : Base {
    int value;  // 隐藏 Base::value
};

Derived d;
d.value;        // 访问 Derived::value
d.Base::value;  // 通过限定名访问 Base::value
```

**查找顺序**：
1. 在类自身的成员中查找
2. 在基类成员中查找（按继承顺序）
3. 考虑虚基类

### 4.2 虚函数的特殊行为

**重要特性**：限定名的成员函数调用永远不会触发虚函数分发：

```cpp
struct B { virtual void foo(); };
struct D : B { void foo() override; };

D x;
B& b = x;

b.foo();     // 虚函数调用，实际调用 D::foo
b.B::foo();  // 静态绑定，直接调用 B::foo
```

这一特性允许在需要时明确调用基类版本的虚函数。

### 4.3 命名空间查找机制

限定名在命名空间中的查找遵循以下规则：

**第一阶段**：在目标命名空间 N 及其所有内联命名空间成员中查找。

**第二阶段**：如果第一阶段未找到声明，则在 N 中的 using-directive 所引用的命名空间中查找。

```cpp
namespace A { int a; }
namespace B { using namespace A; }

void f() {
    B::a;  // OK：通过 B 中的 using-directive 找到 A::a
}
```

### 4.4 注入类名 (Injected-Class-Name)

在类的作用域内，类名被视为注入的成员，称为注入类名：

```cpp
struct A {
    A* p;        // OK：A 指向当前类类型
    struct A { };  // 错误：不能重新定义 A
};

struct B {
    void f() {
        B* p1;   // OK
        ::B* p2; // OK
    }
};
```

## 5. 使用场景 (Use Cases)

### 5.1 访问被隐藏的全局名称

当局部变量隐藏了全局名称时，使用 `::` 访问全局实体：

```cpp
#include <iostream>

namespace M {
    const char* fail = "fail\n";
}

using M::fail;

namespace N {
    const char* ok = "ok\n";
}

using namespace N;

int main() {
    struct std {};  // 局部结构体隐藏了命名空间 std

    // std::cout << fail;  // 错误：非限定查找找到局部结构体 std
    ::std::cout << ::ok;   // OK：::std 找到命名空间 std
}
```

### 5.2 访问被隐藏的类成员

在派生类中访问被隐藏的基类成员：

```cpp
struct Base {
    int value = 10;
    void display() { /* ... */ }
};

struct Derived : Base {
    int value = 20;  // 隐藏 Base::value

    void show() {
        std::cout << value;         // Derived::value = 20
        std::cout << Base::value;   // Base::value = 10
    }
};
```

### 5.3 调用特定版本的虚函数

强制调用基类版本的虚函数：

```cpp
class Window {
public:
    virtual void draw() { /* 默认绘制 */ }
};

class Button : public Window {
public:
    void draw() override {
        Window::draw();  // 先执行基类绘制
        // 然后执行派生类特定绘制
    }
};
```

### 5.4 在声明中使用限定名

在类外定义成员时使用限定名：

```cpp
class X {};

constexpr int number = 100;

struct C {
    class X {};
    static const int number = 50;
    static X arr[number];
};

// 错误示例
// X C::arr[number], brr[number];
// X 查找到的是 ::X，不是 C::X

// 正确写法
C::X C::arr[number], brr[number];
// arr 的大小是 50 (C::number)
// brr 的大小是 100 (::number)
```

### 5.5 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|-----|------|---------|
| 模板参数查找位置 | 模板参数在当前作用域查找，不在模板名的作用域查找 | 使用限定名或 using 声明 |
| 析构函数查找 | 析构函数名称在与左侧相同的作用域查找 | 理解查找规则 |
| 命名空间歧义 | 多个 using-directive 引入同名实体导致歧义 | 明确限定名或调整命名空间结构 |

## 6. 代码示例 (Examples)

### 6.1 基础用法：全局命名空间访问

```cpp
#include <iostream>

int value = 100;

int main() {
    int value = 200;

    std::cout << "Local value: " << value << std::endl;      // 200
    std::cout << "Global value: " << ::value << std::endl;   // 100

    return 0;
}
```

### 6.2 基础用法：类成员访问

```cpp
#include <iostream>

struct A {
    static int n;
};

int A::n = 42;

int main() {
    int A = 0;  // 局部变量隐藏了类名 A

    // A::n = 100;  // OK：:: 左侧的非限定查找忽略变量 A
    // A b;         // 错误：非限定查找找到变量 A

    struct A a_obj;     // 使用详述类型说明符
    a_obj.n = 100;      // 通过对象访问静态成员

    return 0;
}
```

### 6.3 高级用法：命名空间中的限定查找

```cpp
#include <iostream>

int x;

namespace Y {
    void f(float) { std::cout << "Y::f(float)\n"; }
    void h(int) { std::cout << "Y::h(int)\n"; }
}

namespace Z {
    void h(double) { std::cout << "Z::h(double)\n"; }
}

namespace A {
    using namespace Y;
    void f(int) { std::cout << "A::f(int)\n"; }
    void g(int) { std::cout << "A::g(int)\n"; }
    int i;
}

namespace B {
    using namespace Z;
    void f(char) { std::cout << "B::f(char)\n"; }
    int i;
}

namespace AB {
    using namespace A;
    using namespace B;
    void g() { std::cout << "AB::g()\n"; }
}

void h() {
    AB::g();     // 调用 AB::g()

    AB::f(1);    // 查找顺序：AB -> A, B -> 找到 A::f 和 B::f
                 // 重载决议选择 A::f(int)

    AB::i++;     // 错误：A::i 和 B::i 歧义

    AB::h(16.8); // 查找顺序：AB -> A, B -> Y, Z
                 // 找到 Y::h 和 Z::h，重载决议选择 Z::h(double)
}

int main() {
    h();
    return 0;
}
```

### 6.4 高级用法：析构函数查找

```cpp
#include <iostream>

struct C { typedef int I; };

typedef int I1, I2;

extern int *p, *q;

struct A { ~A() { std::cout << "A::~A()\n"; } };

typedef A AB;

int main() {
    p->C::I::~I(); // I 在 C 的作用域中查找，找到 C::I

    q->I1::~I2();  // I2 在 I1 的作用域（当前作用域）中查找

    AB x;
    x.AB::~AB();   // AB 在当前作用域查找
                   // 调用 A 的析构函数

    return 0;
}
```

### 6.5 高级用法：构造函数与注入类名

```cpp
#include <iostream>

struct A { A() { std::cout << "A::A()\n"; } };

struct B : A { B() { std::cout << "B::B()\n"; } };

A::A() {}  // 定义 A 的构造函数
B::B() {}  // 定义 B 的构造函数

int main() {
    B::A ba;          // OK：B::A 表示类型 A（在 B 的作用域中查找）

    // A::A a;        // 错误：A::A 不表示类型

    struct A::A a2;   // OK：详述类型说明符中忽略函数名
                      // A::A 表示类 A 的注入类名

    return 0;
}
```

### 6.6 常见错误及修正

**错误示例 1：模板参数查找位置错误**

```cpp
// 错误代码
namespace N {
    template<typename T>
    struct foo {};

    struct X {};
}

N::foo<X> x;  // 错误：X 在当前作用域查找，不是在 N 中查找
```

```cpp
// 修正方法 1：使用完全限定名
namespace N {
    template<typename T>
    struct foo {};

    struct X {};
}

N::foo<N::X> x;  // OK：明确指定 N::X
```

```cpp
// 修正方法 2：使用 using 声明
namespace N {
    template<typename T>
    struct foo {};

    struct X {};
}

using N::X;
N::foo<X> x;  // OK：X 现在在当前作用域可用
```

**错误示例 2：命名空间成员歧义**

```cpp
// 错误代码
namespace A { int i; }
namespace B { int i; }

namespace AB {
    using namespace A;
    using namespace B;
}

void f() {
    AB::i++;  // 错误：A::i 和 B::i 歧义
}
```

```cpp
// 修正方法：使用完全限定名
namespace A { int i; }
namespace B { int i; }

namespace AB {
    using namespace A;
    using namespace B;
}

void f() {
    A::i++;  // OK：明确指定 A::i
    B::i++;  // OK：明确指定 B::i
}
```

**错误示例 3：误用限定名调用虚函数**

```cpp
// 这不是错误，但需要理解行为
#include <iostream>

struct Base {
    virtual void foo() { std::cout << "Base::foo()\n"; }
};

struct Derived : Base {
    void foo() override { std::cout << "Derived::foo()\n"; }
};

int main() {
    Derived d;
    Base& b = d;

    b.foo();     // 输出 "Derived::foo()"（虚函数调用）
    b.Base::foo();  // 输出 "Base::foo()"（静态绑定）

    return 0;
}
// 注意：这可能在需要基类行为时是有用的，
// 但如果不理解这一特性，可能导致意外行为
```

## 7. 总结 (Summary)

### 核心要点

1. **定义与作用**：限定名是出现在 `::` 右侧的名称，用于在明确指定的作用域中查找实体

2. **查找范围**：
   - `::name` - 全局命名空间
   - `Class::name` - 类作用域
   - `Namespace::name` - 命名空间作用域

3. **关键特性**：
   - 限定名的成员函数调用不触发虚函数分发
   - 模板参数在当前作用域查找，不在模板名的作用域查找
   - 同一个声明可以通过不同路径找到多次

4. **特殊规则**：
   - 析构函数名称在与左侧相同的作用域查找
   - `Class::Class` 形式在特定上下文中表示构造函数
   - 注入类名允许在类内部引用自身类型

### 与非限定名查找的对比

| 特性 | 限定名查找 | 非限定名查找 |
|-----|----------|-------------|
| 查找范围 | 明确指定 | 从当前作用域向外逐层查找 |
| 受局部名称影响 | 否 | 是 |
| ADL（参数依赖查找） | 不适用 | 适用 |
| 虚函数分发 | 不触发（成员函数） | 触发 |

### 相关概念

- **非限定名查找 (Unqualified Name Lookup)** - 从当前作用域开始向外查找
- **参数依赖查找 (Argument-Dependent Lookup, ADL)** - 基于函数参数类型的查找
- **作用域 (Scope)** - 名称可见的区域
- **注入类名 (Injected-Class-Name)** - 类名在自身作用域内作为成员可见

### 学习建议

1. 理解限定名查找与非限定名查找的区别是掌握 C++ 名称查找机制的基础
2. 注意模板参数查找位置的特殊规则
3. 利用限定名访问被隐藏的成员是一种常见的编程模式
4. 在多重继承和命名空间组合使用时，注意可能出现的歧义问题