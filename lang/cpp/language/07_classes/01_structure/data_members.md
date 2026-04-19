# 非静态数据成员 (Non-static Data Members)

## 1. 概述

非静态数据成员 (non-static data members) 是在类的成员规范中声明的变量，它们构成了类对象的数据部分。每个类对象都拥有自己独立的非静态数据成员副本，这些成员的生命周期与包含它们的对象相同。

非静态数据成员是 C++ 面向对象编程的核心概念之一，用于封装对象的状态信息，是类实现数据抽象和封装特性的基础机制。

## 2. 来源与演变

### 首次引入

非静态数据成员的概念自 C++ 语言诞生之初就存在，最早可追溯到 C with Classes（1980 年代初），是 C++ 面向对象特性的基础组成部分。

### C++11 变化

- **默认成员初始化器 (default member initializer)**：允许在成员声明时直接指定初始值
- 禁止在非静态数据成员声明中使用占位符类型说明符（如 `auto`）

### C++14 变化

- 允许聚合类 (aggregate classes) 使用默认成员初始化器
- 新增 `decltype(auto)` 占位符（但仍不允许用于非静态数据成员）

### C++17 变化

- 类模板参数推导可用于某些上下文
- 增加了对引用成员从默认成员初始化器初始化的限制

### C++20 变化

- 允许位域成员使用默认成员初始化器
- `[[no_unique_address]]` 属性影响布局兼容性和公共初始序列

### C++23 变化

- 访问说明符不再影响非零大小成员的地址顺序（统一了布局规则）

### 关键缺陷报告

| 缺陷报告 | 版本 | 修正内容 |
|---------|------|---------|
| CWG 80 | C++98 | 允许非静态数据成员与类同名（若无用户声明构造函数） |
| CWG 613 | C++98 | 允许在未求值操作数中使用非静态数据成员 |
| CWG 1397 | C++11 | 默认成员初始化器不能触发默认构造函数的隐式定义 |
| CWG 1696 | C++98 | 禁止将引用成员绑定到临时对象 |

## 3. 语法与声明

### 基本声明语法

```cpp
class S
{
    int n;              // 非静态数据成员
    int& r;             // 引用类型的非静态数据成员
    int a[2] = {1, 2};  // 带默认成员初始化器的非静态数据成员 (C++11)
    std::string s, *ps; // 两个非静态数据成员

    struct NestedS
    {
        std::string s;
    } d5;               // 嵌套类型的非静态数据成员

    char bit : 2;       // 两位位域
};
```

### 声明限制

非静态数据成员的声明有以下限制：

| 限制 | 说明 |
|------|------|
| 存储类说明符 | 不允许 `extern`、`register`、`thread_local` |
| 类型限制 | 不允许不完整类型、抽象类类型及其数组 |
| 占位符类型 | 不允许 `auto`、`decltype(auto)` 等 (C++11 起) |
| 命名限制 | 若存在用户声明构造函数，成员名不能与类名相同 |
| 自引用 | 类 `C` 不能有类型为 `C` 的非静态成员，但可以有 `C&` 或 `C*` |

### 位域声明

```cpp
struct Flags
{
    unsigned int readable : 1;   // 1 位位域
    unsigned int writable : 1;   // 1 位位域
    unsigned int executable : 1; // 1 位位域
    unsigned int : 5;            // 无名位域，用于对齐
};
```

## 4. 底层原理

### 内存布局

当创建类 `C` 的对象时，每个非引用类型的非静态数据成员都分配在对象表示的某个部分中：

```
对象内存布局示意：
+------------------+
| 成员 1           | <- 低地址
+------------------+
| 填充 (padding)   |
+------------------+
| 成员 2           |
+------------------+
| 成员 3           | <- 高地址
+------------------+
```

#### 布局规则演变

| 版本 | 规则 |
|------|------|
| C++23 前 | 相同访问控制的成员按声明顺序分配地址，不同访问控制的成员顺序未指定 |
| C++23 起 | 所有非零大小成员按声明顺序分配地址，访问控制不再影响顺序 |

#### 引用成员

引用成员是否占用存储空间由实现定义，但其存储期与包含它们的对象相同。

### 标准布局类 (Standard-layout Class)

标准布局类具有特殊的内存布局保证，便于与 C 语言交互和进行低级内存操作。

#### 标准布局要求 (C++11 起)

- 所有非静态数据成员具有相同的访问控制
- 没有虚函数或虚基类
- 所有非静态数据成员和基类都是标准布局类型
- 其他特定条件

#### 公共初始序列 (Common Initial Sequence)

两个标准布局非联合类类型的公共初始序列是指从第一个实体开始，满足以下条件的最长成员序列：

1. 对应实体具有布局兼容类型
2. 对应实体具有相同的对齐要求
3. 两个实体要么都是相同宽度的位域，要么都不是位域
4. (C++20 起) 如果 `__has_cpp_attribute(no_unique_address) != 0`，则实体不能声明为 `[[no_unique_address]]`

```cpp
struct A { int a; char b; };
struct B { const int b1; volatile char b2; };
// A 和 B 的公共初始序列是 A.a、A.b 和 B.b1、B.b2

struct C { int c; unsigned : 0; char b; };
// A 和 C 的公共初始序列是 A.a 和 C.c

struct D { int d; char b : 4; };
// A 和 D 的公共初始序列是 A.a 和 D.d

struct E { unsigned int e; char b; };
// A 和 E 的公共初始序列为空
```

#### 布局兼容 (Layout-compatible)

两个标准布局类型满足以下条件之一即为布局兼容：
- 它们是相同类型（忽略 cv 限定符）
- 它们是布局兼容的枚举（具有相同底层类型）
- 它们的公共初始序列包含所有非静态数据成员和位域

### 标准布局类型的特殊属性

1. **联合体公共初始序列访问**：在标准布局联合体中，可以通过另一个联合体成员的非静态数据成员读取活动成员的公共初始序列部分（读取 volatile 成员例外）

2. **指针转换**：指向标准布局类对象的指针可以 `reinterpret_cast` 为指向其第一个非静态非位域数据成员（如有）或基类子对象的指针，反之亦然

3. **offsetof 宏**：可以使用 `offsetof` 宏确定成员距标准布局类开头的偏移量

## 5. 使用场景

### 成员初始化方式

非静态数据成员可以通过两种方式初始化：

#### 1. 构造函数成员初始化列表

```cpp
struct S
{
    int n;
    std::string s;
    S() : n(7) {} // 直接初始化 n，默认初始化 s
};
```

#### 2. 默认成员初始化器 (Default Member Initializer, C++11 起)

```cpp
struct S
{
    int n = 7;
    std::string s{'a', 'b', 'c'};
    S() {} // 默认成员初始化器复制初始化 n，列表初始化 s
};
```

**注意**：如果构造函数的成员初始化列表中显式初始化了某个成员，则该成员的默认成员初始化器被忽略。

```cpp
#include <iostream>

int x = 0;

struct S
{
    int n = ++x;
    S() {}                 // 使用默认成员初始化器
    S(int arg) : n(arg) {} // 使用成员初始化列表
};

int main()
{
    std::cout << x << '\n'; // 输出 0
    S s1;                   // 默认初始化器执行
    std::cout << x << '\n'; // 输出 1
    S s2(7);                // 默认初始化器未执行
    std::cout << x << '\n'; // 输出 1
}
```

### 默认成员初始化器的限制

| 限制 | 版本 |
|------|------|
| 位域成员不允许使用默认成员初始化器 | C++20 前 |
| 数组成员不能从初始化器推导大小 | 所有版本 |
| 不能触发外围类默认构造函数的隐式定义 | 所有版本 |
| 引用成员不能绑定到临时对象 | 所有版本 |

```cpp
struct X
{
    int a[] = {1, 2, 3};  // 错误：不能推导数组大小
    int b[3] = {1, 2, 3}; // 正确
};

struct node
{
    node* p = new node; // 错误：会导致隐式定义 node::node()
};

struct A
{
    A() = default;
    A(int v) : v(v) {}
    const int& v = 42; // 注意：默认构造时绑定临时对象是错误形式
};

A a1;    // 错误：临时对象绑定到引用成员
A a2(1); // 正确（v 在构造函数中初始化，默认成员初始化器被忽略）
         // 但 a2.v 是悬空引用
```

### 非静态数据成员名称的使用场景

非静态数据成员的名称只能在以下三种情况下使用：

#### 1. 类成员访问表达式

```cpp
struct S
{
    int m;
    int n;
    int x = m;            // 正确：默认初始化器中允许隐式 this->

    S(int i) : m(i), n(m) // 正确：成员初始化列表中允许隐式 this->
    {
        this->f();        // 显式成员访问
        f();              // 成员函数体中允许隐式 this->
    }

    void f();
};
```

#### 2. 形成指向非静态成员的指针

```cpp
struct S
{
    int m;
    void f();
};

int S::*p = &S::m;       // 正确：使用 m 形成成员指针
void (S::*fp)() = &S::f; // 正确：使用 f 形成成员函数指针
```

#### 3. 在未求值操作数中使用 (仅限数据成员)

```cpp
struct S
{
    int m;
    static const std::size_t sz = sizeof m; // 正确：m 在未求值操作数中
};

std::size_t j = sizeof(S::m + 42); // 正确：即使没有 this 对象
```

**注意**：此用法通过 CWG issue 613 (N2253) 解决，部分编译器将其视为 C++11 的变更。

## 6. 代码示例

### 基础用法

```cpp
#include <string>
#include <iostream>

class Person
{
public:
    // 非静态数据成员声明
    std::string name;
    int age;
    double height;

    // 默认成员初始化器 (C++11)
    int id = 0;

    // 构造函数使用成员初始化列表
    Person(const std::string& n, int a, double h)
        : name(n), age(a), height(h)
    {
        // 构造函数体
    }

    // 默认构造函数使用默认成员初始化器
    Person() = default;
};

int main()
{
    Person p1("Alice", 30, 165.5);
    Person p2;  // name 为空, age 未定义, height 未定义, id = 0

    std::cout << p1.name << ", " << p1.age << std::endl;
    return 0;
}
```

### 高级用法：标准布局类

```cpp
#include <cstddef>
#include <iostream>

// 标准布局结构体
struct Point
{
    int x;
    int y;
};

// 布局兼容结构体
struct Point3D
{
    int x;
    int y;
    int z;
};

int main()
{
    // 标准布局类的特殊属性演示

    Point p{10, 20};

    // 1. 指针转换：可以转换为第一个成员的指针
    int* first_member = reinterpret_cast<int*>(&p);
    std::cout << "First member: " << *first_member << std::endl; // 10

    // 2. 使用 offsetof 获取成员偏移
    std::cout << "Offset of y: " << offsetof(Point, y) << std::endl;

    return 0;
}
```

### 联合体公共初始序列

```cpp
#include <iostream>

struct A
{
    int x;
    double y;
};

struct B
{
    int x;
    float z;
};

union U
{
    A a;
    B b;
};

int main()
{
    U u;
    u.a.x = 42;  // 激活 a 成员

    // 标准：可以通过 b.x 读取，因为它是公共初始序列的一部分
    std::cout << u.b.x << std::endl;  // 输出 42（公共初始序列访问）

    return 0;
}
```

### 常见错误及修正

#### 错误 1：类中包含自身类型的成员

```cpp
// 错误：类不能包含自身类型的非静态成员
struct Node
{
    Node next;  // 错误：不完整类型
};

// 正确：使用指针或引用
struct Node
{
    Node* next;   // 正确：指针允许
    Node& ref;    // 正确：引用允许
};
```

#### 错误 2：成员名与类名冲突

```cpp
// 若有用户声明构造函数，成员不能与类同名
struct S
{
    S(int);  // 用户声明构造函数
    int S;   // 错误：成员名与类名相同
};

// 正确：无用户声明构造函数时允许
struct T
{
    int T;   // 正确：无用户声明构造函数
};
```

#### 错误 3：引用成员绑定临时对象

```cpp
// 错误：引用成员不能绑定临时对象
struct Bad
{
    const int& ref = 42;  // 危险！
};

Bad b;  // 错误：尝试绑定临时对象

// 正确：通过构造函数初始化
struct Good
{
    const int& ref;

    Good(const int& r) : ref(r) {}
    Good(int&&) = delete;  // 防止绑定临时对象
};

int value = 42;
Good g(value);  // 正确
```

#### 错误 4：默认成员初始化器导致无限递归

```cpp
// 错误：默认成员初始化器需要默认构造函数
struct Node
{
    Node* next = new Node;  // 错误：无限递归定义
};

// 正确：延迟初始化
struct Node
{
    Node* next = nullptr;

    void createNext() { next = new Node; }
};
```

## 7. 总结

非静态数据成员是 C++ 类封装数据的核心机制，每个对象拥有独立的成员副本。

### 核心要点

| 特性 | 说明 |
|------|------|
| 声明限制 | 不允许 `extern`、`register`、`thread_local`；不允许不完整类型 |
| 内存布局 | 成员按声明顺序分配（C++23 统一规则），访问控制曾影响布局（C++23 前） |
| 初始化方式 | 构造函数初始化列表 或 默认成员初始化器 (C++11 起) |
| 标准布局 | 相同访问控制、无虚函数、布局兼容类型，支持 `offsetof` 和指针转换 |

### 版本特性对比

| 特性 | C++98 | C++11 | C++20 | C++23 |
|------|-------|-------|-------|-------|
| 默认成员初始化器 | 不支持 | 支持 | 支持位域 | 支持 |
| 占位符类型 | 不适用 | 禁止 | 禁止 | 禁止 |
| 访问控制影响布局 | 是 | 是 | 是 | 否 |
| `[[no_unique_address]]` | 不适用 | 不适用 | 支持 | 支持 |

### 最佳实践

1. **优先使用默认成员初始化器**：减少构造函数重复，提高可维护性
2. **注意引用成员**：确保生命周期安全，避免悬空引用
3. **使用标准布局**：需要与 C 交互或使用 `offsetof` 时确保类为标准布局
4. **避免自引用值类型**：使用指针或引用代替值类型的自引用

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/data_members
- C++ Standard: [class.mem] 非静态数据成员
- C++ Standard: [class.mem.general] 成员规范
- C++ Standard: [class.standard.layout] 标准布局类