# 成员模板 (Member Template)

## 1. 概述

**成员模板** (Member Template) 是在类、结构体或联合体（非局部类）的成员声明中出现的模板声明。成员模板可以是：

- **成员函数模板** (Member Function Template)：最常见的成员模板形式
- **成员类模板** (Member Class Template)：嵌套在类中的类模板
- **成员变量模板** (Member Variable Template)（C++14 起）：静态数据成员模板

成员模板允许类成员具有泛化能力，使得一个类可以针对多种类型提供相同的操作，增强了类的灵活性和可复用性。

## 2. 来源与演变

### 首次引入

成员模板作为 C++ 模板机制的一部分，最早在 **C++98** 标准中引入。随着模板元编程的发展，成员模板成为实现泛型编程的重要工具。

### 历史背景

在成员模板引入之前：
- 类成员函数只能针对固定类型工作
- 若要支持多种类型，需要为每种类型编写单独的函数或使用继承
- 代码重复严重，维护困难

成员模板的引入解决了这些问题，提供了：
- 类型安全的泛化操作
- 与类模板配合实现更灵活的设计
- 减少代码重复

### C++14 变化

- 新增**成员变量模板**支持，允许在类作用域内声明静态数据成员模板
- 禁止转换函数模板使用推导返回类型（`operator auto`）

### C++17 及以后

- CWG 727 缺陷报告修正：允许在类内部定义成员模板的完整特化

### 重要缺陷报告

| DR | 应用版本 | 原始行为 | 修正行为 |
|-----|---------|---------|---------|
| CWG 727 | C++98 | 类内部完整特化不被允许 | 允许类内部完整特化 |
| CWG 1878 | C++14 | `operator auto` 技术上被允许 | 禁止 `operator auto` 作为模板 |

## 3. 语法与声明

### 基本语法

成员模板声明在类的成员声明区域：

```cpp
class ClassName {
    // 成员函数模板
    template<typename T>
    ReturnType functionName(T parameter);

    // 成员类模板
    template<typename T>
    class NestedClass;

    // 成员变量模板 (C++14)
    template<typename T>
    static const T memberVariable;
};
```

### 类外定义语法

当成员模板在类外定义时，需要提供两层模板参数列表（对于类模板的成员模板）：

```cpp
// 类模板
template<typename T1>
class OuterClass {
    // 成员模板
    template<typename T2>
    void memberFunction(T2 param);
};

// 类外定义：需要两层 template 参数
template<typename T1>    // 外层类模板参数
template<typename T2>    // 成员模板参数
void OuterClass<T1>::memberFunction(T2 param) {
    // 函数体
}
```

### 部分特化与完整特化

```cpp
struct A {
    template<class T> struct B;        // 主模板
    template<class T> struct B<T*> {}; // 部分特化（类内部）
};

template<> struct A::B<int*> {};       // 完整特化（类外部）
template<class T> struct A::B<T&> {};  // 部分特化（类外部）
```

### 声明限制

| 成员类型 | 是否可为模板 | 说明 |
|---------|------------|------|
| 析构函数 | 否 | 不能是模板 |
| 复制构造函数 | 否 | 不能是模板（使用隐式声明的版本） |
| 虚函数 | 否 | 成员函数模板不能是 virtual |
| 转换函数 | 是 | 可以是模板 |
| 静态数据成员 | 是 | C++14 起支持变量模板 |

## 4. 底层原理

### 模板实例化机制

成员模板的实例化遵循以下规则：

1. **隐式实例化**：当使用特定模板参数调用成员模板时，编译器自动生成对应版本
2. **显式实例化**：可以显式要求编译器生成特定版本
3. **延迟实例化**：成员模板只有在被使用时才会实例化

```cpp
template<typename T>
class Container {
    template<typename U>
    void process(U value);  // 只有被调用时才实例化
};
```

### 重载决议规则

当存在同名模板和非模板成员函数时：

1. 如果模板特化与非模板函数签名完全匹配
2. 优先选择**非模板函数**
3. 除非显式提供模板参数列表

```cpp
template<typename T>
struct A {
    void f(int);              // 非模板成员
    template<typename T2>
    void f(T2);              // 成员模板
};

A<char> ac;
ac.f(1);      // 调用非模板 A<char>::f(int)
ac.f<>(1);    // 调用模板 A<char>::f<int>(int)
```

### 转换函数模板的特殊规则

转换函数模板在重载决议时有特殊规则：

1. **名称查找**：转换函数模板的特化不参与名称查找
2. **推导规则**：所有可见的转换函数模板都被考虑
3. **模板参数推导**：使用特殊规则进行推导

### 等价性要求

类外定义必须与类内声明**等价**，否则被视为重载：

```cpp
struct X {
    template<class T> T good(T n);
    template<class T> T bad(T n);
};

template<class V>
V X::good(V n) { return n; }  // OK: 等价声明

// 错误：不等价于类内声明
template<class T>
T X::bad(typename identity<T>::type n) { return n; }
```

## 5. 使用场景

### 适用场景

| 场景 | 示例 |
|------|------|
| 泛型操作符重载 | 实现 `operator()` 可接受多种类型参数 |
| 类型转换函数 | 转换函数模板允许转换到任意类型 |
| 泛型构造函数 | 从任意兼容类型构造对象 |
| 静态成员泛化 | C++14 起支持静态数据成员模板 |

### 实际应用案例

#### 1. 泛型仿函数

成员函数模板最常见的用途是实现泛型仿函数：

```cpp
struct Printer {
    std::ostream& os;
    Printer(std::ostream& os) : os(os) {}

    template<typename T>
    void operator()(const T& obj) {
        os << obj << ' ';
    }
};

// 可以处理任何可输出类型
std::vector<int> v{1, 2, 3};
std::for_each(v.begin(), v.end(), Printer(std::cout));

std::string s{"abc"};
std::ranges::for_each(s, Printer(std::cout));
```

#### 2. 泛型构造函数

允许从多种类型构造对象：

```cpp
template<typename T1>
class MyString {
public:
    // 泛型构造函数
    template<typename T2>
    MyString(const std::basic_string<T2>& s) {
        // 从不同字符类型的字符串构造
    }
};
```

#### 3. 转换函数模板

```cpp
struct A {
    template<typename T>
    operator T*() { return nullptr; }  // 可转换到任意指针类型
};

A a;
int* ip = a;        // 调用 operator int*()
char* cp = a;       // 调用 operator char*()
```

### 最佳实践

1. **避免与虚函数混淆**：成员函数模板不能是虚函数，不能覆盖基类的虚函数
2. **使用显式模板参数**：当需要调用模板版本而非非模板版本时，使用 `<>` 语法
3. **确保声明等价**：类外定义必须与类内声明签名完全一致
4. **注意复制构造函数**：如果模板构造函数可能匹配复制构造函数签名，编译器会优先使用隐式生成的复制构造函数

### 常见陷阱

1. **虚函数覆盖误解**：成员函数模板不能覆盖基类虚函数

```cpp
class Base {
    virtual void f(int);
};

struct Derived : Base {
    template<class T> void f(T);  // 不覆盖 Base::f(int)！

    void f(int i) override {     // 需要非模板版本
        f<>(i);                   // 调用模板版本
    }
};
```

2. **转换函数模板与推导返回类型**：C++14 起禁止使用

```cpp
struct S {
    operator auto() const { return 10; }  // OK
    template<class T>
    operator auto() const { return 42; }  // 错误！
};
```

## 6. 代码示例

### 基础用法：成员函数模板

```cpp
#include <iostream>
#include <string>

class Calculator {
public:
    // 成员函数模板
    template<typename T>
    T add(T a, T b) {
        return a + b;
    }

    // 类外定义的成员模板
    template<typename T>
    T multiply(T a, T b);
};

// 类外定义
template<typename T>
T Calculator::multiply(T a, T b) {
    return a * b;
}

int main() {
    Calculator calc;

    std::cout << calc.add(1, 2) << std::endl;        // 3 (int)
    std::cout << calc.add(1.5, 2.5) << std::endl;    // 4.0 (double)
    std::cout << calc.multiply(3, 4) << std::endl;  // 12

    return 0;
}
```

### 高级用法：类模板中的成员模板

```cpp
#include <iostream>
#include <vector>

template<typename T>
class Container {
private:
    std::vector<T> data;

public:
    Container() = default;

    // 泛型构造函数：从迭代器范围构造
    template<typename Iterator>
    Container(Iterator first, Iterator last) : data(first, last) {}

    // 泛型添加函数
    template<typename U>
    void add(U&& value) {
        data.push_back(static_cast<T>(std::forward<U>(value)));
    }

    // 泛型转换函数
    template<typename U>
    operator Container<U>() const {
        Container<U> result;
        for (const auto& item : data) {
            result.add(static_cast<U>(item));
        }
        return result;
    }

    void print() const {
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    // 从初始化列表构造
    Container<int> c1 = {1, 2, 3, 4, 5};
    c1.print();  // 1 2 3 4 5

    // 泛型转换
    Container<double> c2 = c1;
    c2.print();  // 1 2 3 4 5 (作为 double)

    return 0;
}
```

### 常见错误及修正

#### 错误 1：成员函数模板声明为虚函数

```cpp
// 错误：成员函数模板不能是虚函数
class Base {
    template<typename T>
    virtual void process(T value);  // 编译错误！
};

// 正确做法：使用非模板虚函数
class Base {
public:
    virtual void process(int value) { }
    virtual void process(double value) { }
};

// 或者使用模板方法模式
class Base {
public:
    virtual void doProcess() = 0;

    template<typename T>
    void process(T value) {
        // 非虚模板函数调用虚函数
        internalProcess(value);
        doProcess();
    }

private:
    template<typename T>
    void internalProcess(T value) { /* ... */ }
};
```

#### 错误 2：模板复制构造函数陷阱

```cpp
class MyClass {
public:
    // 这不是复制构造函数！
    template<typename T>
    MyClass(const T& other) { /* ... */ }
};

// 编译器仍会生成默认复制构造函数
MyClass a;
MyClass b(a);  // 调用隐式生成的复制构造函数，而非模板版本

// 正确做法：显式定义复制构造函数
class MyClass {
public:
    MyClass(const MyClass&) = default;  // 显式声明

    template<typename T>
    MyClass(const T& other) { /* ... */ }
};
```

#### 错误 3：类外定义声明不等价

```cpp
struct X {
    template<class T> T func(T n);
};

// 错误：不等价的声明被视为新函数（重载），但类外定义不允许重载
template<class T>
T X::func(typename std::add_const<T>::type n) {  // 编译错误！
    return n;
}

// 正确做法：确保签名完全一致
template<class T>
T X::func(T n) {
    return n;
}
```

### 完整示例：泛型打印器

```cpp
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <list>

struct Printer {
    std::ostream& os;

    Printer(std::ostream& os) : os(os) {}

    // 成员函数模板：可处理任何可输出类型
    template<typename T>
    void operator()(const T& obj) {
        os << obj << ' ';
    }
};

int main() {
    // 处理 int vector
    std::vector<int> v{1, 2, 3};
    std::for_each(v.begin(), v.end(), Printer(std::cout));
    std::cout << std::endl;  // 输出: 1 2 3

    // 处理 string
    std::string s{"abc"};
    std::ranges::for_each(s, Printer(std::cout));
    std::cout << std::endl;  // 输出: a b c

    // 处理 double list
    std::list<double> l{1.1, 2.2, 3.3};
    std::for_each(l.begin(), l.end(), Printer(std::cout));
    std::cout << std::endl;  // 输出: 1.1 2.2 3.3

    return 0;
}
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 声明位置 | 类、结构体或联合体的成员声明区域（非局部类） |
| 类型支持 | 函数模板、类模板、变量模板（C++14） |
| 虚函数限制 | 成员函数模板不能是 virtual |
| 特殊成员限制 | 析构函数和复制构造函数不能是模板 |
| 类外定义 | 需要两层 template 参数（类模板成员模板） |

### 成员模板 vs 非成员模板

| 特性 | 成员模板 | 非成员模板 |
|------|---------|-----------|
| 声明位置 | 类内部 | 命名空间作用域 |
| 访问权限 | 可访问私有成员 | 无法访问私有成员（除非为友元） |
| 实例化时机 | 使用时 | 使用时 |
| 特化方式 | 可在类内或类外特化 | 在命名空间作用域特化 |

### 使用建议

1. **优先使用成员模板**：当函数需要访问私有成员且需要泛化时
2. **避免与特殊成员函数冲突**：注意模板构造函数与复制/移动构造函数的交互
3. **谨慎使用转换函数模板**：可能导致意外的隐式转换
4. **保持声明一致性**：确保类外定义与类内声明签名完全等价
5. **善用泛型构造函数**：实现类型间的灵活转换

### 相关概念

| 概念 | 关系 |
|------|------|
| 类模板 (Class Template) | 成员模板可定义在类模板中，形成嵌套模板结构 |
| 函数模板 (Function Template) | 成员函数模板是函数模板的成员版本 |
| 模板特化 (Template Specialization) | 成员模板支持部分特化和完整特化 |
| CRTP (Curiously Recurring Template Pattern) | 成员模板常用于 CRTP 模式中 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/member_template
- C++ Standard: [class.mem], [temp.mem]
- Effective C++, Scott Meyers, Item 45