# Using-declaration - using 声明

## 1. 概述

`using-declaration`（using 声明）是 C++ 中的一种声明机制，用于将其他作用域中定义的名称引入到当前声明区域。它允许开发者将命名空间成员、基类成员或枚举器（C++20 起）引入到当前作用域中，从而简化代码书写并增强代码可读性。

using 声明主要有以下用途：
- **命名空间成员引入**：将命名空间中的特定成员引入到当前作用域
- **基类成员引入**：在派生类中改变基类成员的访问权限或引入基类成员
- **继承构造函数**（C++11 起）：自动继承基类的构造函数
- **枚举器引入**（C++20 起）：将作用域枚举的枚举器引入到命名空间、块或类作用域

## 2. 来源与演变

### 首次引入

using 声明最早出现在 **C++98** 标准中，主要用于命名空间成员的引入和基类成员的访问控制调整。

### 历史背景

在 using 声明引入之前，开发者面临以下问题：
1. 使用命名空间成员时需要完整限定名，代码冗长
2. 派生类中无法直接提升基类保护成员的访问权限
3. 无法方便地重用基类的重载函数集

### 版本演进

| 版本 | 变化 |
|------|------|
| C++98 | 基本的 using 声明功能：命名空间成员引入、基类成员引入 |
| C++11 | 新增继承构造函数（inheriting constructors）功能，`using Base::Base;` |
| C++17 | 支持多个声明符（comma-separated list）、包展开（pack expansion） |
| C++20 | 支持引入作用域枚举的枚举器 |

### 缺陷报告修正

| 缺陷报告 | 修正内容 |
|----------|----------|
| CWG 258 | 派生类成员函数覆盖/隐藏基类成员函数时，要求 cv 限定符必须相同 |
| CWG 1738 | 禁止显式实例化或显式特化继承构造函数模板 |
| CWG 2504 | 明确了从虚基类继承构造函数的行为 |
| P0136R1 | 改变继承构造函数语义，不再注入额外构造函数，而是使基类构造函数可通过名称查找发现 |

## 3. 语法与参数

### 语法形式

**C++17 之前：**

```cpp
using typename(可选) 嵌套名称说明符 非限定标识符 ;
```

**C++17 及之后：**

```cpp
using 声明符列表 ;
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `typename` | 可选关键字，用于在类模板中从基类引入依赖类型成员时消除歧义 |
| 嵌套名称说明符 | 名称和作用域解析运算符 `::` 的序列，以作用域解析运算符结尾。单个 `::` 表示全局命名空间 |
| 非限定标识符 | 标识表达式，表示要引入的名称 |
| 声明符列表 | 逗号分隔的一个或多个声明符，每个声明符形式为 `typename(可选) 嵌套名称说明符 非限定标识符`。部分声明符可后跟省略号 `...` 表示包展开 |

### 类型别名语法（using 别名声明）

注意：using 声明与 using 别名声明（type alias）是不同的语法：

```cpp
// using 声明：引入已有名称
using std::vector;  // 引入 std::vector 到当前作用域

// using 别名声明：定义类型别名
using IntVector = std::vector<int>;  // 定义 IntVector 为 std::vector<int> 的别名
```

## 4. 底层原理

### 名称查找机制

using 声明通过修改名称查找规则来实现其功能：

1. **命名空间作用域**：将指定名称添加到当前作用域的声明集合中
2. **类作用域**：在派生类中创建指向基类成员的"链接"，使基类成员在派生类中可见

### 访问控制

当 using 声明引入基类成员时：
- 引入后的访问权限由 using 声明所在位置的访问说明符决定
- 可以将基类的 `protected` 成员提升为 `public`
- 不能降低基类 `private` 成员的访问权限（因为派生类无法访问）

### 继承构造函数原理

C++11 最初实现：在派生类中注入一系列合成的构造函数声明。

C++17（P0136R1）之后：继承构造函数声明仅使基类构造函数可通过名称查找发现，调用时直接使用基类构造函数初始化基类子对象。

### 包展开机制（C++17）

```cpp
template<typename... Ts>
struct Overloader : Ts... {
    using Ts::operator()...;  // 展开所有基类的 operator()
};
```

## 5. 使用场景

### 在命名空间和块作用域中使用

用于引入命名空间的特定成员，避免重复书写完整限定名：

```cpp
#include <iostream>
#include <string>

using std::string;  // 在命名空间级别引入

int main() {
    string str = "Example";  // 无需 std:: 前缀
    using std::cout;         // 在块级别引入
    cout << str;
}
```

### 在类定义中使用

#### 场景一：调整基类成员访问权限

```cpp
class Base {
protected:
    int value;  // Base::value 是 protected
};

class Derived : public Base {
public:
    using Base::value;  // Derived::value 变为 public
};
```

#### 场景二：引入基类重载函数集

当派生类重写基类函数时，其他重载版本会被隐藏。使用 using 声明可以引入所有重载版本：

```cpp
struct Base {
    void f(int);
    void f(double);
};

struct Derived : Base {
    void f(int);  // 隐藏 Base::f(int) 和 Base::f(double)
    using Base::f;  // 引入所有 Base::f 重载版本
};
```

#### 场景三：继承构造函数

```cpp
struct Base {
    Base(int, double);
    Base(std::string);
};

struct Derived : Base {
    using Base::Base;  // 继承 Base 的所有构造函数
};
```

### 最佳实践

1. **选择性引入**：优先使用 `using std::cout;` 而非 `using namespace std;`，避免命名污染
2. **最小作用域原则**：在尽可能小的作用域内使用 using 声明
3. **构造函数继承**：当派生类无需额外初始化逻辑时，使用继承构造函数减少代码重复

### 注意事项

| 注意点 | 说明 |
|--------|------|
| 不继承默认参数 | using 声明引入构造函数时不继承默认参数 |
| 重名处理 | 派生类同名成员会隐藏 using 声明引入的成员 |
| 不能引入模板特化 | `using B::f<int>;` 是错误的 |
| 不能引入命名空间 | 只能引入命名空间成员，不能引入整个命名空间 |
| 析构函数不能引入 | using 声明不能用于基类析构函数 |
| 赋值运算符特殊处理 | 基类的赋值运算符若与派生类的复制/移动赋值运算符签名相同，会被派生类的隐式声明隐藏 |

## 6. 代码示例

### 基础用法：命名空间成员引入

```cpp
#include <iostream>
#include <string>

// 在命名空间级别引入
using std::string;

int main() {
    string name = "Alice";

    // 在块级别引入
    using std::cout;
    using std::endl;

    cout << "Hello, " << name << endl;
    return 0;
}
```

### 基类成员访问权限调整

```cpp
#include <iostream>

struct Base {
protected:
    int m;
    using value_type = int;

    void print() { std::cout << "Base::print" << std::endl; }
};

struct Derived : Base {
public:
    using Base::m;           // m 变为 public
    using Base::value_type;  // value_type 变为 public
    using Base::print;       // print 变为 public
};

int main() {
    Derived d;
    d.m = 42;           // OK: m 是 public
    d.print();          // OK: print 是 public

    // Base b;
    // b.m = 42;        // Error: Base::m 是 protected

    return 0;
}
```

### 引入重载函数集

```cpp
#include <iostream>

struct B {
    virtual void f(int) { std::cout << "B::f(int)" << std::endl; }
    void g(char)        { std::cout << "B::g(char)" << std::endl; }
    void h(int)         { std::cout << "B::h(int)" << std::endl; }
};

struct D : B {
    using B::f;
    void f(int) override { std::cout << "D::f(int)" << std::endl; }  // override B::f(int)

    using B::g;
    void g(int) { std::cout << "D::g(int)" << std::endl; }  // g(int) 和 g(char) 都可见

    using B::h;
    void h(int) { std::cout << "D::h(int)" << std::endl; }  // D::h(int) 隐藏 B::h(int)
};

int main() {
    D d;
    B& b = d;

    b.f(1);   // 调用 D::f(int) - 虚函数
    d.f(1);   // 调用 D::f(int)

    d.g(1);   // 调用 D::g(int)
    d.g('a'); // 调用 B::g(char) - 通过 using 引入

    b.h(1);   // 调用 B::h(int) - 非虚函数
    d.h(1);   // 调用 D::h(int)

    return 0;
}
```

### 继承构造函数（C++11）

```cpp
#include <iostream>
#include <string>

struct Base {
    int x;
    double y;

    Base(int i, double d) : x(i), y(d) {
        std::cout << "Base(int, double)" << std::endl;
    }

    Base(std::string s) : x(0), y(0.0) {
        std::cout << "Base(string)" << std::endl;
    }
};

struct Derived : Base {
    using Base::Base;  // 继承所有 Base 构造函数

    std::string name = "unnamed";  // 默认成员初始化
};

int main() {
    Derived d1(42, 3.14);      // 调用 Base(int, double)
    Derived d2("hello");       // 调用 Base(string)

    std::cout << d1.x << ", " << d1.y << std::endl;
    std::cout << d2.name << std::endl;

    return 0;
}
```

### 包展开用法（C++17）

```cpp
#include <iostream>
#include <iomanip>

// 组合多个函数对象的调用运算符
template<typename... Ts>
struct Overloader : Ts... {
    using Ts::operator()...;  // 引入所有基类的 operator()
};

// C++17 推导指引
template<typename... T>
Overloader(T...) -> Overloader<T...>;

int main() {
    auto o = Overloader{
        [](auto const& a) { std::cout << a; },
        [](float f) { std::cout << std::setprecision(3) << f; }
    };

    o(42);      // 调用第一个 lambda
    o(3.14159f); // 调用第二个 lambda

    return 0;
}
```

### 枚举器引入（C++20）

```cpp
// C++20 起
enum class Button { Up, Down };

struct Widget {
    using Button::Up;
    Button state = Up;  // 无需 Button::Up
};

using Button::Down;
constexpr Button non_up = Down;  // 无需 Button::Down

constexpr auto get_button(bool is_up) {
    using Button::Up, Button::Down;  // 多个声明符
    return is_up ? Up : Down;
}
```

### 常见错误及修正

#### 错误 1：using 声明不能引入模板特化

```cpp
// 错误示例
struct B {
    template<class T>
    void f() {}
};

struct D : B {
    using B::f<int>;  // 错误：不能引入模板特化
};

// 修正：引入模板本身
struct D_fixed : B {
    using B::f;       // OK：引入模板名称
    void g() { f<int>(); }  // OK
};
```

#### 错误 2：依赖名称的模板消歧器问题

```cpp
template<class X>
struct Base {
    template<class T>
    void f(T) {}
};

template<class Y>
struct Derived : Base<Y> {
    // using Base<Y>::template f;  // 错误：不允许使用 template 消歧器
    using Base<Y>::f;              // OK，但 f 不被视为模板名

    void g() {
        // f<int>(0);              // 错误：f 不是模板名
        f(0);                      // OK
    }
};
```

#### 错误 3：虚基类继承构造函数的初始化问题

```cpp
struct V {
    V() = default;
    V(int);
};

struct A : virtual V {
    using V::V;
    A() = delete;  // 删除默认构造
};

int bar() { return 42; }

struct B : A {
    B() : A(bar()) {}  // OK：bar() 会被调用吗？
};

struct C : B {};

void foo() {
    C c;  // bar() 不会被调用！
    // 当 V 是虚基类，且当前对象不是最派生对象时，
    // 继承的构造函数调用（包括参数求值）会被省略
}
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 基本功能 | 将其他作用域的名称引入当前作用域 |
| 访问控制 | 可调整基类成员在派生类中的访问权限 |
| 重载引入 | 可引入基类的所有重载版本 |
| 构造函数继承 | C++11 起，可继承基类构造函数 |
| 包展开 | C++17 起，支持可变参数包展开 |
| 枚举器引入 | C++20 起，可引入作用域枚举的枚举器 |

### 相关语法对比

| 语法 | 用途 |
|------|------|
| `using std::cout;` | 声明：引入特定名称 |
| `using namespace std;` | 指令：引入整个命名空间 |
| `using Int = int;` | 别名：定义类型别名 |

### 学习建议

1. **区分 using 声明与 using 指令**：声明引入特定名称，指令引入整个命名空间
2. **理解访问权限调整的限制**：只能提升权限，不能降低
3. **注意继承构造函数的语义变化**：C++17 后的语义更简洁高效
4. **合理使用包展开**：C++17 的包展开可以简化变参模板代码

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/using_declaration
- C++23 标准 (ISO/IEC 14882:2024): 9.9 The using declaration [namespace.udecl]
- C++20 标准 (ISO/IEC 14882:2020): 9.9 The using declaration [namespace.udecl]
- C++17 标准 (ISO/IEC 14882:2017): 10.3.3 The using declaration [namespace.udecl]
- P0136R1: Rewording inheriting constructors