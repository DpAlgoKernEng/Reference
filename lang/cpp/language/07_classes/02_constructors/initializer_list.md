# 构造函数与成员初始化列表 (Constructors and Member Initializer Lists)

## 1. 概述 (Overview)

**构造函数 (Constructor)** 是一种特殊的非静态成员函数，使用特殊的声明符语法声明，用于初始化其类类型的对象。构造函数没有名称，不能被直接调用，而是在对象初始化时根据初始化规则被隐式调用。

**成员初始化列表 (Member Initializer List)** 是构造函数定义中函数体开括号之前的部分，用于指定基类和非静态数据成员的初始化方式。它以冒号 `:` 开始，后跟逗号分隔的一个或多个成员初始化器。

构造函数定义在 `<构造函数所属类>` 中（作为类的成员函数），是 C++ 面向对象编程的核心机制之一。

### 核心特性

| 特性 | 说明 |
|------|------|
| 无返回类型 | 构造函数声明不允许指定返回类型 |
| 自动调用 | 对象创建时自动调用，不能直接调用 |
| 初始化顺序 | 按声明顺序初始化，与初始化列表顺序无关 |
| 可重载 | 支持多个构造函数，通过参数区分 |

## 2. 来源与演变 (Origin and Evolution)

### 首次引入

构造函数的概念在 **C++98** 标准中首次引入，是 C++ 面向对象特性的基础组成部分。

### 历史背景

在构造函数出现之前，对象初始化面临以下问题：

1. **初始化时机不确定**：对象创建后处于未初始化状态
2. **手动初始化繁琐**：需要显式调用初始化函数
3. **资源管理困难**：无法保证资源正确获取

构造函数的出现解决了这些问题，提供了：
- 自动化的对象初始化机制
- 与对象生命周期绑定的资源管理
- 一致的初始化接口

### C++11 新增特性

- **委托构造函数 (Delegating Constructor)**：允许构造函数调用同类的其他构造函数
- **成员初始化器 (Default Member Initializer)**：类内直接为非静态成员指定默认值
- **继承构造函数 (Inheriting Constructor)**：使用 `using` 声明继承基类构造函数
- `constexpr` 构造函数：支持编译期构造

### C++20 变化

- 构造函数不能是协程 (coroutine)

### C++23 变化

- 构造函数不能有显式对象参数 (explicit object parameter)

### 缺陷报告修正

| DR | 版本 | 原行为 | 修正后行为 |
|----|------|--------|-----------|
| CWG 194 | C++98 | 声明语法只允许一个函数说明符 | 允许多个函数说明符（如 `inline explicit`） |
| CWG 257 | C++98 | 抽象类是否需为虚基类提供成员初始化器未指定 | 不需要，执行时忽略此类初始化器 |
| CWG 263 | C++98 | 构造函数语法禁止构造函数作为友元 | 允许构造函数作为友元 |
| CWG 1345 | C++98 | 无默认成员初始化器的匿名联合成员被默认初始化 | 它们不被初始化 |
| CWG 1435 | C++98 | 构造函数声明中"类名"含义不清 | 改为专用函数声明符语法 |
| CWG 1696 | C++98 | 引用成员可初始化为临时对象 | 此类初始化为非法 |

## 3. 语法与参数 (Syntax and Parameters)

### 构造函数声明语法

```
class-name ( parameter-list(optional) ) except(optional) attr(optional)
```

| 组成部分 | 说明 |
|----------|------|
| class-name | 标识符表达式，可能后跟属性列表，可能用括号包围（C++11 起） |
| parameter-list | 参数列表 |
| except | 异常规范（动态异常规范或 noexcept 规范） |
| attr | 属性列表（C++11 起） |

### 允许的声明说明符

构造函数声明只允许以下说明符：
- `friend`
- `inline`
- `constexpr` (C++11 起)
- `consteval` (C++20 起)
- `explicit`

**注意**：不允许返回类型，也不允许 cv 限定符和引用限定符。

### 成员初始化列表语法

```
(1) class-or-identifier ( expression-list(optional) )
(2) class-or-identifier braced-init-list                    // C++11 起
(3) parameter-pack ...                                       // C++11 起
```

| 语法 | 说明 |
|------|------|
| (1) | 使用直接初始化（或值为初始化，若表达式列表为空） |
| (2) | 使用列表初始化（空列表时为值初始化，聚合时为聚合初始化） |
| (3) | 包展开，初始化多个基类 |

### 参数说明

| 参数 | 说明 |
|------|------|
| class-or-identifier | 命名非静态数据成员的标识符，或命名类本身（委托构造）或直接/虚基类的类型名 |
| expression-list | 传递给基类或成员构造函数的参数列表（可能为空） |
| braced-init-list | 花括号包围的初始化列表 |
| parameter-pack | 可变模板参数包的名称 |

### 构造函数类型

| 类型 | 说明 |
|------|------|
| 默认构造函数 | 可无参调用的构造函数 |
| 复制构造函数 | 参数为同类对象的构造函数 |
| 移动构造函数 | 参数为同类右值引用的构造函数 |
| 转换构造函数 | 无 `explicit` 说明符的构造函数 |
| 委托构造函数 | 成员初始化列表中调用自身类其他构造函数（C++11 起） |

## 4. 底层原理 (Underlying Principles)

### 初始化顺序

成员初始化列表中的初始化顺序与列表中的书写顺序**无关**。实际初始化顺序如下：

1. **虚基类**：按深度优先、从左到右遍历基类声明的顺序初始化（仅对最派生类的构造函数）
2. **直接基类**：按基类说明符列表中从左到右的顺序初始化
3. **非静态数据成员**：按类定义中的声明顺序初始化
4. **构造函数体**：最后执行

**重要**：这种设计确保了析构函数可以按构造的逆序销毁对象。

### 成员初始化机制

```cpp
struct S {
    int n;
    S(int);       // 构造函数声明
    S() : n(7) {} // 构造函数定义
                  // ": n(7)" 是成员初始化列表
};
```

在构造函数体开始执行之前：
1. 所有直接基类已完成初始化
2. 所有虚基类已完成初始化
3. 所有非静态数据成员已完成初始化

### 虚基类初始化

- 只有最派生类的构造函数会初始化虚基类
- 其他类对虚基类的成员初始化器会被忽略
- 这保证了虚基类只被初始化一次

### 虚函数调用规则

从成员初始化列表调用成员函数（包括虚函数）时：
- 如果直接基类已初始化，虚函数调用按正常规则进行
- 动态类型被视为正在构造的类的静态类型（动态分派不会向下传播）
- 调用纯虚函数是未定义行为

### 异常处理

成员初始化器中抛出的异常可以被函数 try 块捕获：

```cpp
Class()
try   // 函数 try 块在函数体之前开始，包括初始化列表
    : Class(0.0) // 委托构造
{
    // ...
}
catch (...)
{
    // 初始化时发生异常
}
```

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 初始化 const 成员 | const 成员必须在成员初始化列表中初始化 |
| 初始化引用成员 | 引用成员必须在成员初始化列表中初始化 |
| 初始化无默认构造函数的成员 | 没有默认构造函数的类类型成员必须显式初始化 |
| 初始化基类 | 需要传递参数给基类构造函数时 |
| 性能优化 | 避免先默认构造再赋值的开销 |
| 委托构造 | 减少重复代码，统一初始化逻辑 |

### 最佳实践

1. **优先使用成员初始化列表**：相比在构造函数体内赋值，效率更高
2. **按声明顺序书写**：虽然顺序不影响实际执行，但按声明顺序书写可提高可读性
3. **使用默认成员初始化器**（C++11）：为简单成员提供默认值
4. **谨慎调用虚函数**：构造期间虚函数行为可能与预期不同

### 注意事项

#### 引用成员不能绑定到临时对象

```cpp
struct A {
    A() : v(42) {} // 错误！不能将引用成员绑定到临时对象
    const int& v;
};
```

#### 默认成员初始化器与成员初始化列表

如果非静态数据成员同时有默认成员初始化器和成员初始化列表初始化，成员初始化列表优先：

```cpp
struct S {
    int n = 42;   // 默认成员初始化器
    S() : n(7) {} // n 会被初始化为 7，而非 42
};
```

### 常见陷阱

1. **初始化顺序陷阱**：成员初始化列表中的书写顺序不影响实际初始化顺序
2. **未初始化成员**：某些成员（如 const、引用）必须初始化
3. **迭代器失效**：在成员初始化列表中调用成员函数时，对象尚未完全构造
4. **循环委托**：委托构造函数不能形成循环

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>
#include <string>

struct Base {
    int n;
    Base(int x) : n(x) { std::cout << "Base constructed with " << n << "\n"; }
};

struct Derived : public Base {
    int value;
    const int constant;
    int& reference;

    // 成员初始化列表示例
    Derived(int v, int& ref)
        : Base(v * 2)      // 初始化基类
        , value(v)         // 初始化普通成员
        , constant(42)      // 初始化 const 成员
        , reference(ref)   // 初始化引用成员
    {
        std::cout << "Derived constructed\n";
    }
};

int main() {
    int x = 10;
    Derived d(5, x);
    std::cout << "Base::n = " << d.n << "\n";
    std::cout << "value = " << d.value << "\n";
    std::cout << "constant = " << d.constant << "\n";
    std::cout << "reference = " << d.reference << "\n";
    return 0;
}
```

### 高级用法：委托构造函数

```cpp
#include <iostream>
#include <string>

class Person {
public:
    // 主构造函数
    Person(std::string name, int age, std::string address)
        : name_(std::move(name)), age_(age), address_(std::move(address))
    {
        std::cout << "Main constructor called\n";
    }

    // 委托构造函数：年龄默认 0
    Person(std::string name, std::string address)
        : Person(std::move(name), 0, std::move(address))
    {
        std::cout << "Delegating constructor (age=0) called\n";
    }

    // 委托构造函数：使用默认地址
    Person(std::string name, int age)
        : Person(std::move(name), age, "Unknown")
    {
        std::cout << "Delegating constructor (default address) called\n";
    }

private:
    std::string name_;
    int age_;
    std::string address_;
};

int main() {
    Person p1("Alice", 25, "123 Main St");
    Person p2("Bob", "456 Oak Ave");
    Person p3("Charlie", 30);
    return 0;
}
```

### 高级用法：初始化顺序演示

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

struct Base {
    int n;
    Base() : n(0) { std::cout << "Base initialized\n"; }
};

struct Class : public Base {
    unsigned char x;
    unsigned char y;
    std::mutex m;
    std::lock_guard<std::mutex> lg;
    std::fstream f;
    std::string s;

    Class(int x)
        : Base{123}      // 1. 首先初始化基类
        , x(x)           // 3. 按声明顺序初始化成员
        , y{0}
        , f{"test.txt", std::ios::app}
        , s(__func__)
        , lg(m)          // m 在 lg 之前声明，所以 m 先初始化
        , m{}            // 虽然写在最后，但按声明顺序先于 lg 初始化
    {
        std::cout << "Constructor body executed\n";
    }

    // 危险示例：依赖未初始化的成员
    Class(double a)
        : y(a + 1)
        , x(y)    // 警告！x 在 y 之前声明，此时 y 未初始化
        , lg(m)
    {}
};

int main() {
    Class c(1);
    return 0;
}
```

### 常见错误及修正

#### 错误 1：初始化顺序误解

```cpp
// 错误：依赖未初始化的成员
struct Bad {
    int a;
    int b;
    Bad(int val) : b(val), a(b) {}  // a 先初始化，此时 b 未初始化
};

// 修正：按声明顺序书写，避免混淆
struct Good {
    int a;
    int b;
    Good(int val) : a(val), b(a) {}  // b 初始化时 a 已初始化
};
```

#### 错误 2：引用成员绑定临时对象

```cpp
// 错误：引用成员绑定到临时对象
struct Bad {
    const int& ref;
    Bad() : ref(42) {}  // 错误！临时对象 42 在构造函数结束后被销毁
};

// 修正：使用指针或确保引用绑定的对象生命周期足够长
struct Good {
    const int* ptr;
    Good() : ptr(nullptr) {}
    void bind(const int& val) { ptr = &val; }
};
```

#### 错误 3：忘记初始化 const 或引用成员

```cpp
// 错误：const 成员未初始化
struct Bad {
    const int value;  // 必须初始化
    Bad() {}  // 编译错误
};

// 修正：在成员初始化列表中初始化
struct Good {
    const int value;
    Good() : value(42) {}  // 正确
};
```

#### 错误 4：循环委托

```cpp
// 错误：循环委托
class Bad {
public:
    Bad(int) : Bad() {}    // 委托给 Bad()
    Bad() : Bad(0) {}      // 委托给 Bad(int) -- 循环！
};

// 修正：确保委托链有终点
class Good {
public:
    Good(int x) : value(x) {}  // 主构造函数
    Good() : Good(0) {}        // 委托构造函数
private:
    int value;
};
```

## 7. 总结 (Summary)

### 核心要点

1. **构造函数是特殊的成员函数**：没有返回类型，在对象创建时自动调用
2. **成员初始化列表是初始化的关键**：用于初始化基类、const 成员和引用成员
3. **初始化顺序是固定的**：按声明顺序初始化，与书写顺序无关
4. **委托构造减少代码重复**：C++11 引入，允许构造函数调用同类其他构造函数

### 构造函数类型对比

| 类型 | 特点 | 用途 |
|------|------|------|
| 默认构造函数 | 无参数或所有参数有默认值 | 默认初始化 |
| 复制构造函数 | 参数为 `const T&` | 复制对象 |
| 移动构造函数 | 参数为 `T&&` | 移动语义 |
| 转换构造函数 | 无 `explicit` | 隐式类型转换 |
| 委托构造函数 | 初始化列表调用自身其他构造函数 | 代码复用 |

### 初始化方式对比

| 方式 | 适用场景 | 性能 |
|------|----------|------|
| 成员初始化列表 | 必须用于 const/引用成员，推荐用于所有成员 | 最高 |
| 默认成员初始化器 | 简化多个构造函数的重复初始化 | 高 |
| 构造函数体内赋值 | 需要复杂逻辑时使用 | 较低（先默认构造再赋值） |

### 学习建议

1. **始终使用成员初始化列表**：这是 C++ 的最佳实践
2. **理解初始化顺序**：避免依赖未初始化成员的错误
3. **善用委托构造**：减少构造函数间的代码重复
4. **注意资源管理**：构造函数中获取的资源应在析构函数中释放

### 相关概念

- 复制消除 (copy elision)
- 转换构造函数 (converting constructor)
- 复制赋值运算符 (copy assignment)
- 复制构造函数 (copy constructor)
- 默认构造函数 (default constructor)
- 析构函数 (destructor)
- `explicit` 说明符
- 初始化 (initialization)
- 移动赋值运算符 (move assignment)
- 移动构造函数 (move constructor)

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024):
  - 11.4.5 Constructors [class.ctor]
  - 11.9.3 Initializing bases and members [class.base.init]
- C++20 标准 (ISO/IEC 14882:2020):
  - 11.4.4 Constructors [class.ctor]
  - 11.10.2 Initializing bases and members [class.base.init]
- cppreference: https://en.cppreference.com/w/cpp/language/constructor