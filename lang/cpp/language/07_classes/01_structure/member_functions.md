# 非静态成员函数 (Non-static Member Functions)

## 1. 概述

非静态成员函数（non-static member function）是在类的成员说明中声明、但不带 `static` 或 `friend` 说明符的函数。这是 C++ 面向对象编程的核心概念之一，定义了对象的行为。

每个非静态成员函数都与类的实例（对象）关联，通过隐式或显式的 `this` 指针访问对象的数据成员。与静态成员函数不同，非静态成员函数必须在对象上调用。

**核心特点**：
- 必须通过对象实例调用
- 拥有隐式 `this` 指针参数
- 可声明为 const、volatile 或引用限定
- 支持虚函数和多态
- 包含构造函数、析构函数和转换函数等特殊成员函数

**相关概念**：
- **显式对象成员函数（Explicit Object Member Function）**：C++23 引入，带有显式对象参数的非静态成员函数
- **隐式对象成员函数（Implicit Object Member Function）**：不带显式对象参数的传统非静态成员函数

## 2. 来源与演变

### C++98 首次引入

非静态成员函数是 C++ 从 Simula 继承的核心面向对象特性。C++98 标准确立了以下特性：

- cv 限定符（const/volatile）
- 虚函数和纯虚函数
- 特殊成员函数（构造、析构、拷贝）

### C++11 新增特性

| 特性 | 说明 |
|------|------|
| 引用限定符（ref-qualifier） | 允许 `&` 和 `&&` 限定成员函数，区分左值/右值对象调用 |
| `override` 和 `final` 说明符 | 增强虚函数覆盖的安全性 |
| 移动构造/赋值 | 新增特殊成员函数 |

**Feature-test macro**: `__cpp_ref_qualifiers` = `200710L`

### C++23 新增特性

| 特性 | 说明 |
|------|------|
| 显式对象参数（Deducing this） | 允许将 `this` 作为显式参数传递，简化 CRTP 模式 |

**Feature-test macro**: `__cpp_explicit_this_parameter` = `202110L`

### 缺陷报告

| 缺陷报告 | 应用版本 | 原始行为 | 修正行为 |
|----------|----------|----------|----------|
| CWG 194 | C++98 | 非静态成员函数能否与类同名存在歧义 | 明确禁止同名 |

## 3. 语法与参数

### 基本语法

```cpp
class 类名 {
    // 声明
    返回类型 函数名(参数列表) cv限定符(可选) ref限定符(可选);

    // 定义（可内联）
    返回类型 函数名(参数列表) cv限定符(可选) ref限定符(可选) { 函数体 }
};

// 外部定义
返回类型 类名::函数名(参数列表) cv限定符(可选) ref限定符(可选) { 函数体 }
```

### 声明元素

| 元素 | 语法 | 说明 |
|------|------|------|
| cv 限定符 | `const`、`volatile`、`const volatile` | 修饰 `this` 指针类型 |
| ref 限定符 | `&`、`&&` | 限制左值或右值对象调用（C++11） |
| virtual | `virtual` | 声明虚函数 |
| pure-specifier | `= 0` | 声明纯虚函数 |
| override | `override` | 确保覆盖基类虚函数（C++11） |
| final | `final` | 阻止进一步派生或覆盖（C++11） |

### cv 限定符

cv 限定符（const/volatile）出现在参数列表之后，修饰 `this` 指针类型：

```cpp
struct S {
    void f();              // this 类型: S*
    void f() const;        // this 类型: const S*
    void f() volatile;     // this 类型: volatile S*
    void f() const volatile; // this 类型: const volatile S*
};
```

**重要特性**：
- 不同 cv 限定符的函数构成重载
- const 成员函数内只能调用 const 成员函数
- const 成员函数内修改成员需要 `const_cast` 或 `mutable`

### 引用限定符（C++11）

引用限定符用于区分对象是左值还是右值：

```cpp
struct S {
    void f() &;   // 仅限左值对象调用
    void f() &&;  // 仅限右值对象调用
};

S s;
s.f();            // 调用 f() &
std::move(s).f(); // 调用 f() &&
S().f();          // 调用 f() &&
```

**注意**：与 cv 限定符不同，引用限定符不改变 `this` 指针的性质 —— 在 `&&` 限定的函数内，`*this` 仍然是左值表达式。

### 显式对象参数（C++23）

C++23 允许将 `this` 作为显式参数传递：

```cpp
struct X {
    void foo(this X const& self, int i); // 等价于 void foo(int i) const &
    void bar(this X self, int i);        // 按值传递对象
};
```

**优势**：
1. 消除 const/non-const 成员函数的代码重复
2. 简化 CRTP（Curiously Recurring Template Pattern）模式
3. 派生类类型推导

### 隐式 this 指针

在非静态成员函数体内：
- 非类型成员访问自动转换为 `(*this).member`
- 静态成员/枚举/嵌套类型自动转换为 `ClassName::member`

```cpp
struct S {
    int n;
    static int s;
    void f();
};

void S::f() {
    n = 1;     // 转换为 (*this).n = 1;
    s = 2;     // 转换为 S::s = 2;
}
```

### 特殊成员函数

| 函数 | 默认定义条件 |
|------|--------------|
| 默认构造函数 | 无其他构造函数时 |
| 拷贝构造函数 | 无移动操作和析构函数时 |
| 移动构造函数 | 无拷贝操作、移动赋值和析构函数时（C++11） |
| 拷贝赋值运算符 | 无移动操作和析构函数时 |
| 移动赋值运算符 | 无拷贝操作、移动构造和析构函数时（C++11） |
| 析构函数 | 始终 |

这些函数可以使用 `= default` 显式要求编译器生成默认实现。

## 4. 底层原理

### 调用机制

非静态成员函数的调用涉及隐式对象参数传递：

```cpp
// 源代码
object.method(arg1, arg2);

// 编译器转换（概念上）
ClassName::method(&object, arg1, arg2);
```

### this 指针

`this` 指针是一个隐式传递给非静态成员函数的指针参数：

| 函数类型 | this 指针类型 |
|----------|---------------|
| 普通成员函数 | `X*` |
| const 成员函数 | `const X*` |
| volatile 成员函数 | `volatile X*` |
| const volatile 成员函数 | `const volatile X*` |

### 虚函数机制

虚函数通过虚函数表（vtable）实现多态：

1. 每个含有虚函数的类有一个虚函数表
2. 每个对象包含指向虚函数表的指针（vptr）
3. 虚函数调用通过虚函数表间接寻址

```cpp
struct Base {
    virtual void foo();  // 虚函数
};

struct Derived : Base {
    void foo() override; // 覆盖虚函数
};

Base* b = new Derived;
b->foo(); // 调用 Derived::foo()（动态绑定）
```

### 函数指针差异

普通成员函数指针与显式对象成员函数指针有本质区别：

```cpp
struct Y {
    int f(int, int) const&;      // 传统成员函数
    int g(this Y const&, int, int); // 显式对象参数（C++23）
};

auto pf = &Y::f;  // 成员函数指针
auto pg = &Y::g;  // 普通函数指针

// 调用方式不同
(y.*pf)(1, 2);  // 成员函数指针调用
pg(y, 1, 2);    // 普通函数调用
```

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 封装对象行为 | 定义对象可执行的操作 |
| 访问控制 | 通过 public/private 控制接口可见性 |
| 多态设计 | 使用虚函数实现运行时多态 |
| const 安全 | 通过 const 成员函数保证只读语义 |
| 资源管理 | 构造/析构函数管理对象生命周期 |

### 最佳实践

**1. const 正确性**

```cpp
class Array {
    std::vector<int> data;
public:
    // const 版本：只读访问
    int operator[](int idx) const { return data[idx]; }

    // 非 const 版本：可读写访问
    int& operator[](int idx) { return data[idx]; }
};
```

**2. 引用限定符防止误用**

```cpp
class String {
    std::string data;
public:
    // 仅限左值对象赋值
    String& operator=(const String& other) & {
        data = other.data;
        return *this;
    }
};

String getStr();
String s;
s = getStr();     // OK
// getStr() = s;  // 编译错误：不能对右值赋值
```

**3. 显式对象参数简化代码（C++23）**

```cpp
// 传统写法：需要两个函数
class Container {
    int& at(size_t i) { return data[i]; }
    const int& at(size_t i) const { return data[i]; }
    std::vector<int> data;
};

// C++23 写法：一个函数
class Container {
    template<typename Self>
    auto& at(this Self&& self, size_t i) {
        return self.data[i];
    }
    std::vector<int> data;
};
```

### 常见陷阱

**1. const 成员函数中修改成员**

```cpp
struct S {
    int value;
    void bad() const {
        // value = 1;  // 错误：const 函数不能修改成员
    }

    void good() const {
        const_cast<S*>(this)->value = 1;  // 不推荐
    }

    void better() const {
        // 使用 mutable 关键字
    }
};

struct T {
    mutable int counter;  // mutable 成员
    void increment() const {
        counter++;  // OK
    }
};
```

**2. 迭代器失效**

```cpp
struct Container {
    std::vector<int> data;

    // 错误：返回内部迭代器可能失效
    std::vector<int>::iterator getEnd() {
        return data.end();
    }

    // 更安全：返回索引或 const 引用
    size_t size() const { return data.size(); }
};
```

**3. 虚函数在构造/析构中的调用**

```cpp
struct Base {
    Base() {
        foo();  // 调用 Base::foo()，而非派生类版本
    }
    virtual void foo() { std::cout << "Base\n"; }
};

struct Derived : Base {
    void foo() override { std::cout << "Derived\n"; }
};

Derived d;  // 输出 "Base"，而非 "Derived"
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int age;

public:
    // 构造函数（特殊成员函数）
    Person(const std::string& n, int a) : name(n), age(a) {}

    // const 成员函数
    const std::string& getName() const { return name; }
    int getAge() const { return age; }

    // 非 const 成员函数
    void setAge(int a) { age = a; }

    // 虚函数
    virtual void introduce() const {
        std::cout << "I'm " << name << ", " << age << " years old.\n";
    }

    // 析构函数
    virtual ~Person() = default;
};

int main() {
    Person p("Alice", 30);
    p.introduce();      // 调用虚函数
    std::cout << p.getName() << "\n";  // 调用 const 函数
    p.setAge(31);       // 调用非 const 函数
    return 0;
}
```

### 引用限定符示例

```cpp
#include <iostream>
#include <utility>
#include <string>

class Resource {
    std::string data;
public:
    Resource(const std::string& s) : data(s) {}

    // 左值引用限定：拷贝数据
    std::string getData() const & {
        std::cout << "Copying data\n";
        return data;
    }

    // 右值引用限定：移动数据
    std::string getData() && {
        std::cout << "Moving data\n";
        return std::move(data);
    }
};

int main() {
    Resource r("hello");
    std::string s1 = r.getData();           // 输出: Copying data
    std::string s2 = std::move(r).getData(); // 输出: Moving data
    std::string s3 = Resource("temp").getData(); // 输出: Moving data
    return 0;
}
```

### 显式对象参数示例（C++23）

```cpp
#include <iostream>

// CRTP 简化示例
template<typename Derived>
struct Addable {
    template<typename Self>
    Derived operator+(this Self&& self, const Derived& other) {
        Derived result = static_cast<const Derived&>(self);
        result.add(other);
        return result;
    }
};

class Point : public Addable<Point> {
public:
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    void add(const Point& other) {
        x += other.x;
        y += other.y;
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main() {
    Point p1(1, 2);
    Point p2(3, 4);
    Point p3 = p1 + p2;
    p3.print();  // 输出: (4, 6)
    return 0;
}
```

### 常见错误及修正

**错误 1：const 对象调用非 const 函数**

```cpp
class Counter {
    int count;
public:
    Counter() : count(0) {}
    int increment() { return ++count; }  // 非 const
    int getCount() { return count; }      // 非 const
};

int main() {
    const Counter c;
    // c.increment();  // 错误：const 对象不能调用非 const 函数
    // c.getCount();    // 错误：const 对象不能调用非 const 函数
}

// 修正：为需要的功能添加 const 版本
class Counter {
    int count;
public:
    Counter() : count(0) {}
    int increment() { return ++count; }
    int getCount() const { return count; }  // const 版本
};

int main() {
    const Counter c;
    c.getCount();  // OK
}
```

**错误 2：成员函数返回悬垂引用**

```cpp
class Container {
    int* data;
public:
    Container(size_t n) : data(new int[n]()) {}

    // 危险：返回内部指针的引用
    int*& getDataRef() { return data; }

    ~Container() { delete[] data; }
};

int main() {
    Container c(10);
    int*& ref = c.getDataRef();
    // 如果 c 被销毁，ref 成为悬垂引用
}

// 更好的设计：返回值或 const 引用
class Container {
    int* data;
public:
    int* getData() { return data; }           // 返回值拷贝
    const int* getData() const { return data; } // const 版本
};
```

**错误 3：在构造函数中调用虚函数**

```cpp
class Base {
public:
    Base() { init(); }  // 调用虚函数
    virtual void init() { std::cout << "Base init\n"; }
};

class Derived : public Base {
public:
    void init() override { std::cout << "Derived init\n"; }
};

int main() {
    Derived d;  // 输出 "Base init"，而非 "Derived init"
}

// 修正方案：使用两阶段初始化
class Base {
public:
    Base() = default;
    virtual void init() { std::cout << "Base init\n"; }
};

class Derived : public Base {
public:
    void init() override { std::cout << "Derived init\n"; }
};

int main() {
    Derived d;
    d.init();  // 显式调用，输出 "Derived init"
}
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| 隐式 this | 非静态成员函数通过 `this` 指针访问对象 |
| cv 限定符 | const/volatile 修饰 `this` 指针类型，支持重载 |
| ref 限定符 | `&`/`&&` 区分左值/右值对象调用（C++11） |
| 显式对象参数 | C++23 新特性，简化代码并支持类型推导 |
| 特殊成员函数 | 编译器可自动生成的构造/析构/赋值函数 |

### 技术对比

| 特性 | 传统成员函数 | 显式对象参数（C++23） |
|------|--------------|----------------------|
| this 访问 | 隐式 | 通过显式参数 |
| 函数指针类型 | 成员函数指针 | 普通函数指针 |
| const/non-const 重载 | 需要两份代码 | 可合并为一个模板 |
| CRTP 支持 | 复杂 | 简化 |

### 学习建议

1. **掌握 const 正确性**：始终为不修改对象状态的函数添加 const 限定符
2. **理解 this 指针**：理解隐式传递机制是理解成员函数的关键
3. **善用引用限定符**：防止对临时对象的不当操作
4. **学习 C++23 新特性**：显式对象参数大幅简化模板代码
5. **注意特殊成员函数规则**：理解何时编译器会自动生成这些函数

### 相关概念

| 概念 | 关系 |
|------|------|
| 静态成员函数 | 无 `this` 指针，与类关联而非对象 |
| 友元函数 | 可访问私有成员，但非类成员 |
| 虚函数 | 实现运行时多态 |
| 纯虚函数 | 使类成为抽象类 |
| 构造函数/析构函数 | 特殊成员函数，控制对象生命周期 |

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/member_functions
- C++ Standard: [class.mfct], [class.this]
- "Effective C++", Scott Meyers, Item 7-12
- "The C++ Programming Language", Bjarne Stroustrup