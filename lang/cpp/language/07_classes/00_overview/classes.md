# 类 (Classes)

## 1. 概述 (Overview)

类 (Class) 是 C++ 中的用户定义类型 (user-defined type)，是面向对象编程的核心概念。类类型通过类说明符 (class-specifier) 定义，出现在声明语法的声明说明符序列中。

**核心特点**：
- 封装数据成员和成员函数
- 支持继承和多态
- 提供访问控制机制
- 是 C++ 面向对象编程的基础构建块

**技术定位**：
- 属于类型系统中的复合类型
- 是结构和联合的扩展
- 提供数据抽象和封装能力

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

类概念源自 Simula 语言，C++ 的类设计受到 Simula 67 的深刻影响。Bjarne Stroustrup 在设计 C++ 时，将 Simula 的类概念引入 C 语言，形成了带类的 C (C with Classes)，最终演化为 C++。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 定义基本的类概念，包括成员、访问控制、继承 |
| C++11 | 引入字面类型 (LiteralType)、标准布局类定义、POD 类重定义 |
| C++14 | 成员模板可包含变量模板 |
| C++20 | using-enum-declarations 可引入枚举器；POD 类被废弃 |
| C++26 | 平凡类 (Trivial class) 被废弃 |

### 设计动机

- 提供数据抽象和封装机制
- 支持面向对象编程范式
- 实现零开销抽象原则
- 保持与 C 结构体的兼容性

### 缺陷报告

| DR | 应用版本 | 原有行为 | 修正行为 |
|------|----------|----------|----------|
| CWG 148 | C++98 | POD 类不能包含成员指针 | 移除限制 |
| CWG 383 | C++98 | POD 类可声明但未定义拷贝赋值运算符或析构函数 | 不允许 |
| CWG 1363 | C++11 | 同时有平凡和非平凡默认构造函数的类可能是平凡的 | 是非平凡的 |
| CWG 1496 | C++11 | 只有删除构造函数的类可能是平凡的 | 是非平凡的 |
| CWG 1672 | C++11 | 有多个空基类的类可能是标准布局类 | 不是标准布局类 |
| CWG 1734 | C++11 | 平凡可复制类不能有删除的非平凡拷贝/移动操作 | 删除的操作不影响 |
| CWG 1813 | C++11 | 有基类继承非静态数据成员的类不是标准布局类 | 可以是标准布局类 |
| CWG 1881 | C++11 | 标准布局类的数据成员可能在不同类中声明 | 必须在同一类中声明 |
| CWG 1909 | C++98 | 成员模板可以与类同名 | 禁止 |
| CWG 2120 | C++11 | M(X) 定义未考虑首成员为数组的情况 | 在定义中处理 |
| CWG 2605 | C++98 | 隐式生命周期类可以有用户提供的析构函数 | 禁止 |

## 3. 语法与参数 (Syntax and Parameters)

### 类定义语法

```cpp
class 类名 { 成员说明 };      // class 关键字
struct 类名 { 成员说明 };     // struct 关键字（默认公有）
union 类名 { 成员说明 };      // union 关键字（默认公有，所有成员共享存储）
```

### 成员类型

类可以包含以下类型的成员：

#### 1. 数据成员

| 类型 | 说明 |
|------|------|
| 非静态数据成员 | 每个类实例包含独立副本，包括位字段 |
| 静态数据成员 | 所有实例共享，独立于对象存储 |

#### 2. 成员函数

| 类型 | 说明 |
|------|------|
| 非静态成员函数 | 操作对象实例，可访问 this 指针 |
| 静态成员函数 | 不关联特定对象实例，无 this 指针 |

#### 3. 嵌套类型

- 嵌套类和枚举定义
- 类型别名（typedef 或 using 声明）
- 注入类名 (injected-class-name)：类名在其自身定义中作为成员类型别名

#### 4. 枚举器

来自类中定义的无作用域枚举，或通过 using 声明引入的枚举器。

#### 5. 成员模板

| 类型 | 版本 |
|------|------|
| 函数模板 | C++98+ |
| 类模板 | C++98+ |
| 变量模板 | C++14+ |

### 成员命名限制

类 `T` 的以下成员不能使用 `T` 作为名称：

- 静态数据成员
- 成员函数
- 成员类型
- 成员模板
- 枚举器（除非是有作用域枚举）
- 匿名联合成员

**例外**：非静态数据成员可以使用类名 `T`，前提是没有用户声明的构造函数。

```cpp
struct T {
    int T;        // 正确：非静态数据成员可以与类同名
    static int T; // 错误：静态数据成员不能与类同名
};

struct U {
    U(int);       // 有用户声明的构造函数
    int U;        // 错误：有构造函数时，非静态成员也不能同名
};
```

## 4. 底层原理 (Underlying Principles)

### 多态类 (Polymorphic Class)

具有至少一个声明或继承的虚成员函数的类是多态类。

**特性**：
- 对象表示中包含运行时类型信息 (RTTI)
- 支持 `dynamic_cast` 和 `typeid` 查询
- 虚成员函数参与动态绑定
- 通常包含虚函数表指针 (vptr)

```cpp
struct Polymorphic {
    virtual void foo();  // 使类成为多态类
    virtual ~Polymorphic() = default;
};

// 内存布局（典型实现）：
// [vptr] -> 虚函数表
// [其他数据成员...]
```

### 抽象类 (Abstract Class)

具有至少一个纯虚函数的类是抽象类。

**特性**：
- 不能实例化对象
- 可以有指针和引用
- 派生类必须实现所有纯虚函数才能实例化

### 平方可复制类 (Trivially Copyable Class)

平凡可复制类满足以下条件：

1. 至少有一个合格的拷贝构造函数、移动构造函数、拷贝赋值运算符或移动赋值运算符
2. 每个合格的拷贝/移动构造函数是平凡的
3. 每个合格的拷贝/移动赋值运算符是平凡的
4. 有非删除的平凡析构函数

**意义**：
- 可以用 `memcpy` 安全地复制
- 可以安全地序列化/反序列化
- 兼容 C 语言的内存操作

### 平凡类 (Trivial Class) [C++26 中废弃]

平凡类满足以下条件：
- 是平凡可复制的
- 有一个或多个合格的平凡默认构造函数

### 标准布局类 (Standard-Layout Class)

标准布局类满足以下条件：

1. 没有非标准布局类（或其数组）或引用类型的非静态数据成员
2. 没有虚函数和虚基类
3. 所有非静态数据成员具有相同的访问控制
4. 没有非标准布局的基类
5. 只有最多一个类在继承层次中有非静态数据成员
6. 没有基类与第一个非静态数据成员类型相同（M(S) 规则）

**M(X) 定义**：

| X 的类型 | M(X) 的值 |
|---------|----------|
| 无非静态数据成员的非联合类 | 空集 |
| 第一个非静态数据成员为 X0 的非联合类 | {X0} ∪ M(X0) |
| 联合类型 | ⋃(M(Ui) ∪ {Ui}) |
| 元素类型为 Xe 的数组 | {Xe} ∪ M(Xe) |
| 非类非数组类型 | 空集 |

### 隐式生命周期类 (Implicit-Lifetime Class)

隐式生命周期类满足以下条件之一：

1. 是聚合且析构函数非用户声明（C++11 前）/ 非用户提供（C++11 起），或
2. 至少有一个平凡的合格构造函数和平凡的非删除析构函数

**意义**：此类对象可通过 `malloc` 等方式分配内存后直接使用。

### POD 类 (Plain Old Data Class) [C++20 废弃]

| 版本 | 定义 |
|------|------|
| C++98 前 | 聚合、无用户声明拷贝赋值、无用户声明析构函数、无非 POD 类型成员 |
| C++11 起 | 平凡类 + 标准布局类 + 无非 POD 类型成员 |

## 5. 使用场景 (Use Cases)

### 类属性选择指南

| 属性 | 使用场景 |
|------|----------|
| 多态类 | 需要运行时多态、类型安全向下转型 |
| 抽象类 | 定义接口规范、实现依赖反转 |
| 平方可复制类 | 需要与 C 兼容、序列化、共享内存 |
| 标准布局类 | 与 C 结构体交互、固定内存布局 |
| POD 类 | C 兼容、跨语言接口（已废弃，使用平凡可复制+标准布局） |

### 最佳实践

#### 1. 选择合适的类关键字

```cpp
// 使用 struct：默认公有访问，适合 POD 类型
struct Point { int x, y; };

// 使用 class：默认私有访问，适合封装类型
class Account {
    double balance_;  // 私有成员
public:
    void deposit(double amount);
    double get_balance() const;
};
```

#### 2. 遵循"零之法则"或"五之法则"

```cpp
// 平方可复制类：依赖编译器生成的特殊成员函数
struct SimpleData {
    int x, y, z;
};  // 自动平凡可复制

// 多态类：声明虚析构函数
class Polymorphic {
public:
    virtual ~Polymorphic() = default;
    virtual void process() = 0;
};
```

#### 3. 使用注入类名

```cpp
template<typename T>
struct Container {
    // 注入类名 Container 在此可见
    Container* next;  // 不需要 Container<T>* next
    void link(Container& other);  // 不需要 Container<T>&
};
```

### 常见陷阱

| 陷阱 | 问题 | 解决方案 |
|------|------|----------|
| 忘记虚析构函数 | 通过基类指针删除派生类对象时资源泄漏 | 多态基类必须声明虚析构函数 |
| 非标准布局的数据成员顺序 | 不同编译器可能有不同布局 | 标准布局类保证固定布局 |
| 成员与类同名 | 可能导致歧义 | 了解命名规则，避免误用 |
| 拷贝非平凡可复制类 | memcpy 可能破坏类不变量 | 使用拷贝构造函数或赋值运算符 |

## 6. 代码示例 (Examples)

### 基础类定义

```cpp
#include <iostream>
#include <string>

// 使用 struct 定义简单数据类
struct Point {
    int x;
    int y;

    void print() const {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

// 使用 class 定义封装类
class BankAccount {
private:
    std::string owner_;
    double balance_;

public:
    BankAccount(const std::string& owner, double initial_balance)
        : owner_(owner), balance_(initial_balance) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance_ += amount;
        }
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance_) {
            balance_ -= amount;
            return true;
        }
        return false;
    }

    double get_balance() const { return balance_; }
    const std::string& get_owner() const { return owner_; }
};

int main() {
    Point p{10, 20};
    p.print();

    BankAccount account("Alice", 1000.0);
    account.deposit(500.0);
    account.withdraw(200.0);
    std::cout << account.get_owner() << "'s balance: "
              << account.get_balance() << "\n";

    return 0;
}
```

### 多态类示例

```cpp
#include <iostream>
#include <memory>
#include <vector>

// 抽象基类（多态类）
class Shape {
public:
    virtual ~Shape() = default;  // 虚析构函数至关重要
    virtual double area() const = 0;
    virtual void draw() const = 0;
};

// 具体派生类
class Circle : public Shape {
    double radius_;

public:
    explicit Circle(double r) : radius_(r) {}

    double area() const override {
        return 3.14159 * radius_ * radius_;
    }

    void draw() const override {
        std::cout << "Circle with radius " << radius_ << "\n";
    }
};

class Rectangle : public Shape {
    double width_, height_;

public:
    Rectangle(double w, double h) : width_(w), height_(h) {}

    double area() const override {
        return width_ * height_;
    }

    void draw() const override {
        std::cout << "Rectangle " << width_ << "x" << height_ << "\n";
    }
};

void process_shapes(const std::vector<std::unique_ptr<Shape>>& shapes) {
    double total_area = 0;
    for (const auto& shape : shapes) {
        shape->draw();
        total_area += shape->area();
    }
    std::cout << "Total area: " << total_area << "\n";
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 3.0));

    process_shapes(shapes);

    // 运行时类型识别
    for (const auto& shape : shapes) {
        if (auto* circle = dynamic_cast<const Circle*>(shape.get())) {
            std::cout << "Found a circle\n";
        }
    }

    return 0;
}
```

### 平方可复制类与标准布局类

```cpp
#include <iostream>
#include <cstring>
#include <type_traits>

// 平方可复制类
struct TriviallyCopyableData {
    int id;
    double value;
    char code;

    // 无用户定义的特殊成员函数
};

// 非平凡可复制类
struct NonTriviallyCopyable {
    int* data;
    int size;

    NonTriviallyCopyable() : data(nullptr), size(0) {}
    ~NonTriviallyCopyable() { delete[] data; }  // 非平凡析构函数

    // 需要自定义拷贝语义
    NonTriviallyCopyable(const NonTriviallyCopyable& other)
        : size(other.size), data(new int[other.size]) {
        std::memcpy(data, other.data, size * sizeof(int));
    }
};

// 标准布局类
struct StandardLayoutPoint {
    int x, y;  // 所有成员相同访问权限
    // 无虚函数、无基类
};

// 非标准布局类
struct NonStandardLayout {
private:
    int x;
public:
    int y;  // 混合访问权限 -> 非标准布局
};

int main() {
    // 类型特征检查
    std::cout << "TriviallyCopyableData:\n";
    std::cout << "  is_trivially_copyable: "
              << std::is_trivially_copyable_v<TriviallyCopyableData> << "\n";
    std::cout << "  is_standard_layout: "
              << std::is_standard_layout_v<TriviallyCopyableData> << "\n";

    std::cout << "\nNonTriviallyCopyable:\n";
    std::cout << "  is_trivially_copyable: "
              << std::is_trivially_copyable_v<NonTriviallyCopyable> << "\n";

    std::cout << "\nStandardLayoutPoint:\n";
    std::cout << "  is_standard_layout: "
              << std::is_standard_layout_v<StandardLayoutPoint> << "\n";

    std::cout << "\nNonStandardLayout:\n";
    std::cout << "  is_standard_layout: "
              << std::is_standard_layout_v<NonStandardLayout> << "\n";

    // 安全使用 memcpy
    TriviallyCopyableData src{1, 3.14, 'A'};
    TriviallyCopyableData dst;
    std::memcpy(&dst, &src, sizeof(TriviallyCopyableData));
    std::cout << "\nCopied: id=" << dst.id << ", value=" << dst.value
              << ", code=" << dst.code << "\n";

    return 0;
}
```

### 注入类名示例

```cpp
#include <iostream>

template<typename T>
class Node {
public:
    T data;
    Node* next;  // 注入类名，等价于 Node<T>*

    Node(const T& val) : data(val), next(nullptr) {}

    // 在类定义内，注入类名作为类型别名
    void link_to(Node& other) {  // 等价于 Node<T>&
        next = &other;
    }
};

// 嵌套类中的注入类名
class Outer {
public:
    class Inner {
    public:
        Outer* parent;  // 可以引用外层类
        Inner(Outer* p) : parent(p) {}
    };

    Inner create_inner() {
        return Inner(this);
    }
};

int main() {
    Node<int> n1{10};
    Node<int> n2{20};
    n1.link_to(n2);

    std::cout << "n1.data = " << n1.data << "\n";
    std::cout << "n1.next->data = " << n1.next->data << "\n";

    return 0;
}
```

### 常见错误示例

```cpp
#include <iostream>

// 错误 1：多态类缺少虚析构函数
class BadBase {
public:
    // 缺少 virtual ~BadBase() = default;
    virtual void foo() = 0;
};

class Derived : public BadBase {
    int* data_;
public:
    Derived() : data_(new int[100]) {}
    ~Derived() { delete[] data_; }  // 通过基类指针删除时不会被调用
    void foo() override {}
};

// 正确做法
class GoodBase {
public:
    virtual ~GoodBase() = default;  // 虚析构函数
    virtual void foo() = 0;
};

// 错误 2：成员命名冲突
struct T {
    // static int T;  // 错误：静态成员不能与类同名
    int T;           // 正确：非静态数据成员可以
};

// 错误 3：期望标准布局但使用了混合访问权限
struct NotStdLayout {
private:
    int x;
public:
    int y;  // 混合访问 -> 非标准布局
};

// 正确做法
struct StdLayout {
    int x, y;  // 相同访问权限
};

int main() {
    // 演示内存泄漏（BadBase）
    BadBase* p = new Derived();
    delete p;  // Derived 的析构函数不被调用 -> 内存泄漏！

    GoodBase* q = new Derived();  // 需要修改 Derived 继承自 GoodBase
    delete q;  // 正确：Derived 的析构函数被调用

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 概念 | 说明 |
|------|------|
| 类 | 用户定义类型，封装数据和行为 |
| 多态类 | 有虚函数的类，支持 RTTI 和动态绑定 |
| 抽象类 | 有纯虚函数的类，不能实例化 |
| 平方可复制类 | 可用 memcpy 安全复制的类 |
| 标准布局类 | 与 C 结构体布局兼容的类 |
| POD 类 | 平凡可复制 + 标准布局（C++20 废弃） |

### 类属性判定流程

```
                    ┌─────────────────┐
                    │     类 T        │
                    └────────┬────────┘
                             │
              ┌──────────────┼──────────────┐
              ▼              ▼              ▼
        有虚函数?       有纯虚函数?     有虚析构函数?
              │              │              │
         多态类          抽象类        支持多态删除
              │              │              │
              └──────────────┴──────────────┘
                             │
              ┌──────────────┼──────────────┐
              ▼              ▼              ▼
        平凡特殊成员?    固定内存布局?   C 兼容?
              │              │              │
         平方可复制      标准布局        POD 类
```

### 类型特征对照

| 特征 | 多态类 | 抽象类 | 平方可复制 | 标准布局 | POD |
|------|--------|--------|------------|----------|-----|
| 虚函数 | ✓ | ✓ (纯虚) | ✗ | ✗ | ✗ |
| 实例化 | ✓ | ✗ | ✓ | ✓ | ✓ |
| RTTI | ✓ | ✓ | ✗ | ✗ | ✗ |
| memcpy 安全 | ✗ | ✗ | ✓ | - | ✓ |
| C 兼容布局 | ✗ | ✗ | - | ✓ | ✓ |

### 学习建议

1. **理解类属性的层次关系**：POD ⊂ (平凡可复制 ∩ 标准布局)
2. **多态类必须声明虚析构函数**：避免通过基类指针删除时的资源泄漏
3. **使用类型特征验证假设**：`std::is_trivially_copyable_v<T>` 等
4. **优先使用 struct 表示数据类**：语义更清晰，默认公有访问
5. **了解废弃特性**：POD 类在 C++20 废弃，使用具体属性替代

### 相关特性测试宏

| 宏 | 值 | 标准 | 特性 |
|----|-----|------|------|
| `__cpp_literal_type` | - | C++17 移除 | 字面类型 |
| `__cpp_lib_is_aggregate` | 201703L | C++17 | `std::is_aggregate` |

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/classes
- C++ Standard: [class]
- Effective C++, Scott Meyers, Item 7 (多态基类声明虚析构函数)