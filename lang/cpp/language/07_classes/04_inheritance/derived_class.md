# 派生类 (Derived Classes)

## 1. 概述 (Overview)

派生类（Derived Class）是 C++ 面向对象编程的核心机制之一，允许一个类类型（使用 `class` 或 `struct` 关键字声明）从一个或多个**基类（Base Class）**派生而来。基类本身也可以从其自身的基类派生，从而形成**继承层次结构（Inheritance Hierarchy）**。

派生类的主要特性包括：

- **代码复用**：派生类自动获得基类的成员
- **多态性支持**：通过虚函数实现运行时多态
- **类型扩展**：在基类基础上添加新功能
- **关系建模**：表达 IS-A（是一种）或 HAS-A（有一个）关系

每个直接和间接基类都作为**基类子对象（Base Class Subobject）**存在于派生类的对象表示中，其偏移量由 ABI 决定。空基类通常因**空基类优化（Empty Base Optimization, EBO）**而不增加派生类对象的大小。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

继承机制是面向对象编程（OOP）的三大支柱之一（封装、继承、多态），C++ 从设计之初就支持继承，使其成为支持面向对象编程的系统级语言。

### 版本演进

| 版本 | 特性变更 |
|------|----------|
| C++98 | 基础继承机制：单继承、多继承、虚继承 |
| C++11 | 引入 `final` 关键字禁止类被继承；支持属性（attributes）；支持从 `decltype` 表达式派生 |
| C++26 | 支持包索引说明符（pack-index-specifier）作为基类 |

### 缺陷报告

| DR | 应用版本 | 原始行为 | 修正行为 |
|-----|----------|----------|----------|
| CWG 1710 | C++98 | `class-or-decltype` 语法使得从需要 template 消歧符的依赖类型派生变得不可能 | 允许使用 template |

## 3. 语法与参数 (Syntax and Parameters)

### 基类说明语法

基类列表在类声明的基类子句（base-clause）中提供，由冒号 `:` 后跟一个或多个基类说明符（base-specifier）组成，多个说明符之间用逗号分隔。

#### 基类说明符形式

| 形式 | 语法 | 说明 |
|------|------|------|
| (1) | `attr(可选) class-or-computed` | 非虚继承，默认成员访问权限 |
| (2) | `attr(可选) virtual class-or-computed` | 虚继承，默认成员访问权限 |
| (3) | `attr(可选) access-specifier class-or-computed` | 非虚继承，指定成员访问权限 |
| (4) | `attr(可选) virtual access-specifier class-or-computed` | 虚继承，指定成员访问权限 |
| (5) | `attr(可选) access-specifier virtual class-or-computed` | 同 (4)，virtual 和访问说明符顺序可互换 |

#### 参数说明

| 参数 | 说明 |
|------|------|
| `attr` | (C++11 起) 任意数量的属性序列 |
| `access-specifier` | 访问说明符：`private`、`public` 或 `protected` |
| `class-or-computed` | 基类类型，可以是：类型名、模板 ID、`decltype` 表达式（C++11 起）、包索引说明符（C++26 起） |

#### 访问说明符默认值

- 使用 `struct` 声明的派生类：默认为 `public` 继承
- 使用 `class` 声明的派生类：默认为 `private` 继承

```cpp
struct Base { int a, b, c; };

// struct 默认 public 继承
struct Derived : Base { int b; };

// class 默认 private 继承
class Derived2 : Base { int c; };
```

### 关键限制

- 同一个类不能被多次指定为直接基类
- 同一个类可以同时是直接基类和间接基类
- 使用 `final` 声明的类不能作为基类
- 详细类型说明符（elaborated type specifier）不能直接作为 `class-or-computed`
- C++11 起，基类说明符可以是包展开（pack expansion）

## 4. 底层原理 (Underlying Principles)

### 对象布局

每个派生类对象包含其所有基类子对象，这些子对象在内存中的布局由具体 ABI 决定。关键机制：

**空基类优化（Empty Base Optimization, EBO）**

空类（没有非静态数据成员的类）作为基类时，编译器可以优化其占用空间为零，避免增加派生类的大小。

```cpp
struct Empty {};
struct Derived : Empty { int x; }; // sizeof(Derived) == sizeof(int)
```

### 虚继承机制

对于每个被指定为 `virtual` 的基类，**最派生对象（Most Derived Object）** 只包含一个该类型的基类子对象，即使该类在继承层次结构中出现多次。

#### 虚继承的初始化规则

- 所有虚基类子对象在任何非虚基类子对象之前初始化
- 只有最派生类在其成员初始化列表中调用虚基类的构造函数
- 中间派生类对虚基类构造函数的调用会被忽略

```cpp
struct B { int n; B(int x) : n(x) {} };

struct X : virtual B { X() : B(1) {} };  // 作为中间类时，B(1) 被忽略
struct Y : virtual B { Y() : B(2) {} };  // 作为中间类时，B(2) 被忽略
struct AA : X, Y     { AA() : B(3), X(), Y() {} };  // AA 负责初始化 B

AA a;  // a.n == 3（AA 的初始化生效）
X x;   // x.n == 1（X 作为最派生类时自己的初始化生效）
```

### 成员访问权限映射

| 继承类型 | 基类 public 成员 | 基类 protected 成员 | 基类 private 成员 |
|----------|-----------------|---------------------|-------------------|
| public 继承 | public | protected | 不可访问 |
| protected 继承 | protected | protected | 不可访问 |
| private 继承 | private | private | 不可访问 |

### 名称查找规则

虚继承涉及特殊的非限定名称查找规则，有时称为**支配规则（Rules of Dominance）**，用于处理虚基类中成员名称的歧义问题。

## 5. 使用场景 (Use Cases)

### Public 继承：IS-A 关系

公有继承建立子类型关系，派生类对象"是一种"基类对象。适用于：

- 多态设计：基类指针/引用可指向派生类对象
- 接口继承：派生类实现基类定义的接口
- 代码复用：派生类复用基类实现

**设计原则（LSP/契约设计）**：
- 派生类应保持其公有基类的类不变量
- 不应强化任何前置条件
- 不应削弱任何后置条件

### Protected 继承：受控多态

受保护继承适用于"受控多态"场景：
- 在派生类及其进一步派生的类的成员中，派生类 IS-A 基类
- 派生类及其子类的成员可以使用基类指针/引用

### Private 继承：实现细节

私有继承适用于：

1. **策略模式设计**：策略通常是空类，使用私有继承可获得：
   - 静态多态性
   - 空基类优化

2. **组合关系的实现**：基类子对象作为派生类的实现细节

**私有继承优于成员的情况**：
- 需要访问基类的 protected 成员（包括构造函数）
- 需要覆盖基类的虚函数
- 需要基类在某些其他基类子对象之前构造、之后析构
- 需要共享虚基类
- 需要控制虚基类的构造

**成员优于私有继承的情况**：
- 更好的封装性
- 不涉及上述特殊情况时首选

### 标准库实例

标准库 iostream 层次结构是虚继承的典型应用：
- `std::istream` 和 `std::ostream` 虚继承自 `std::ios`
- `std::iostream` 多重继承自 `std::istream` 和 `std::ostream`
- 每个 `std::iostream` 对象只包含一个 `std::ios` 子对象

## 6. 代码示例 (Examples)

### 基础示例：继承层次结构

```cpp
#include <iostream>

struct Base
{
    int a, b, c;
};

// Derived 包含 Base 作为子对象
struct Derived : Base
{
    int b;  // 隐藏 Base::b
};

// Derived2 包含 Derived 和 Base 作为子对象
struct Derived2 : Derived
{
    int c;  // 隐藏 Derived::Base::c
};

int main()
{
    Derived d;
    d.a = 1;       // 访问 Base::a
    d.Base::b = 2; // 访问 Base::b
    d.b = 3;       // 访问 Derived::b

    Derived2 d2;
    d2.a = 10;          // Base::a
    d2.Derived::b = 20; // Base::b（通过 Derived）
    d2.b = 30;          // Derived::b
    d2.c = 40;          // Derived2::c
}
```

### 虚继承示例：菱形继承问题

```cpp
#include <iostream>

struct B { int n; };

class X : public virtual B {};
class Y : virtual public B {};
class Z : public B {};  // 非虚继承

// AA 包含：一个 X、一个 Y、一个 Z、两个 B
// 一个是 Z 的基类（非虚），一个是 X 和 Y 共享的（虚）
struct AA : X, Y, Z
{
    AA()
    {
        X::n = 1; // 修改虚基类 B 的成员
        Y::n = 2; // 修改同一个虚基类 B 的成员
        Z::n = 3; // 修改非虚基类 B 的成员

        std::cout << X::n << Y::n << Z::n << '\n'; // 输出: 223
    }
};

int main()
{
    AA a;  // X::n == Y::n == 2, Z::n == 3
    std::cout << "sizeof(AA) = " << sizeof(AA) << '\n';
}
```

### 虚继承构造函数初始化

```cpp
#include <iostream>

struct B
{
    int n;
    B(int x) : n(x) { std::cout << "B(" << x << ")\n"; }
};

struct X : virtual B { X() : B(1) { std::cout << "X()\n"; } };
struct Y : virtual B { Y() : B(2) { std::cout << "Y()\n"; } };

struct AA : X, Y
{
    AA() : B(3), X(), Y() { std::cout << "AA()\n"; }
    // 只有 AA 的 B(3) 生效，X() 和 Y() 中的 B(1)/B(2) 被忽略
};

int main()
{
    std::cout << "创建 AA 对象:\n";
    AA a;  // 输出: B(3), X(), Y(), AA()
    std::cout << "a.n = " << a.n << '\n';  // a.n = 3

    std::cout << "\n创建 X 对象:\n";
    X x;  // 输出: B(1), X()
    std::cout << "x.n = " << x.n << '\n';  // x.n = 1
}
```

### 私有继承：策略模式

```cpp
#include <iostream>

// TCP 传输策略
class TcpTransport
{
public:
    void send(const char* data) {
        std::cout << "TCP 发送: " << data << '\n';
    }
};

// UDP 传输策略
class UdpTransport
{
public:
    void send(const char* data) {
        std::cout << "UDP 发送: " << data << '\n';
    }
};

// 服务类：私有继承传输策略
template<typename Transport>
class Service : private Transport
{
public:
    void transmit(const char* data)
    {
        this->send(data);  // 使用策略的 send 方法
    }
};

int main()
{
    Service<TcpTransport> tcpService;
    tcpService.transmit("Hello via TCP");

    Service<UdpTransport> udpService;
    udpService.transmit("Hello via UDP");

    // TcpTransport* p = &tcpService;  // 错误：私有继承不可转换为基类指针
}
```

### 常见错误：违反 LSP 原则

```cpp
#include <iostream>
#include <string>
#include <vector>

struct MenuOption { std::string title; };

// Menu 是一个 MenuOption 的 vector
class Menu : public std::vector<MenuOption>
{
public:
    std::string title;

    void print() const
    {
        std::cout << title << ":\n";
        for (std::size_t i = 0, s = size(); i < s; ++i)
            std::cout << "  " << (i + 1) << ". " << at(i).title << '\n';
    }
};

enum class Color { WHITE, RED, BLUE, GREEN };

// 错误设计！ColorMenu 违反了 LSP 原则
class ColorMenu : public Menu
{
public:
    std::vector<Color> colors;

    void print() const
    {
        std::cout << title << ":\n";
        for (std::size_t i = 0, s = size(); i < s; ++i)
        {
            std::cout << "  " << (i + 1) << ". ";
            // apply_terminal_color(colors[i]);  // 假设有这个函数
            std::cout << at(i).title << '\n';
        }
    }
};

// 问题分析：
// 1. ColorMenu 需要保持 colors 和 Menu 元素数量同步
// 2. 但用户可以直接调用 push_back/erase 等方法修改 Menu
// 3. 这会破坏 ColorMenu 的不变量

int main()
{
    ColorMenu colorMenu;
    colorMenu.title = "主菜单";

    // 问题：直接调用基类的 push_back
    colorMenu.push_back(MenuOption{"选项1"});

    // colorMenu.print();  // 错误！colors 为空，访问越界

    colorMenu.colors.push_back(Color::RED);
    colorMenu.print();  // 现在可以工作

    // 但这不安全！用户可能再次调用 push_back 而忘记更新 colors
}
```

**修正方案**：使用组合而非继承

```cpp
#include <iostream>
#include <string>
#include <vector>

struct MenuOption { std::string title; };

// 使用组合而非继承
class ColorMenu
{
private:
    std::vector<MenuOption> options;
    std::vector<int> colorCodes;  // 使用 int 代替 Color 简化示例

public:
    std::string title;

    void addOption(const std::string& optionTitle, int color)
    {
        options.push_back({optionTitle});
        colorCodes.push_back(color);
    }

    void removeOption(std::size_t index)
    {
        if (index < options.size())
        {
            options.erase(options.begin() + index);
            colorCodes.erase(colorCodes.begin() + index);
        }
    }

    void print() const
    {
        std::cout << title << ":\n";
        for (std::size_t i = 0, s = options.size(); i < s; ++i)
            std::cout << "  " << (i + 1) << ". " << options[i].title
                      << " (color: " << colorCodes[i] << ")\n";
    }
};

int main()
{
    ColorMenu menu;
    menu.title = "主菜单";
    menu.addOption("新建", 1);
    menu.addOption("打开", 2);
    menu.addOption("保存", 3);
    menu.print();

    menu.removeOption(1);  // 同步删除
    menu.print();
}
```

### 使用 final 禁止继承

```cpp
class Base final {};  // 不能被继承

// class Derived : public Base {};  // 错误：Base 被声明为 final

class NormalBase
{
public:
    virtual void foo() {}
};

class Derived : public NormalBase
{
public:
    void foo() override final {}  // 该虚函数不能被进一步覆盖
};

class Further : public Derived
{
public:
    // void foo() override {}  // 错误：foo 被声明为 final
};
```

## 7. 总结 (Summary)

### 核心要点

1. **继承类型**：C++ 支持三种继承方式（public、protected、private），影响基类成员在派生类中的访问权限

2. **虚继承**：解决菱形继承问题，确保虚基类在最派生对象中只有一份实例

3. **访问控制**：
   - Public 继承：建立 IS-A 关系，支持多态
   - Protected 继承：受限的多态，仅限派生类内部使用
   - Private 继承：实现细节封装，通常用于策略模式或组合实现

4. **构造与析构**：虚基类由最派生类初始化，且先于非虚基类构造

### 技术对比

| 特性 | Public 继承 | Protected 继承 | Private 继承 | 组合 |
|------|-------------|-----------------|--------------|------|
| 语义 | IS-A | 受限 IS-A | 实现细节 | HAS-A |
| 基类指针转换 | 允许 | 仅派生类内部 | 仅当前类内部 | 不适用 |
| 访问基类成员 | 保持原权限 | 变为 protected | 变为 private | 需通过成员访问 |
| 封装性 | 低 | 中 | 高 | 最高 |
| 代码复用 | 高 | 高 | 高 | 中 |

### 学习建议

1. **优先使用组合**：除非需要多态或访问 protected 成员，否则优先使用组合而非继承

2. **理解 LSP 原则**：公有继承应保持里氏替换原则，派生类应能完全替代基类

3. **谨慎使用多继承**：多继承增加复杂性，虚继承有性能开销

4. **使用 override 和 final**：现代 C++ 中使用这些关键字可以避免继承相关的常见错误

5. **理解对象布局**：了解虚继承对对象布局的影响有助于写出更高效的代码

### 相关主题

- 虚函数（Virtual Functions）
- 抽象类（Abstract Classes）
- 名称查找（Name Lookup）
- 空基类优化（Empty Base Optimization）