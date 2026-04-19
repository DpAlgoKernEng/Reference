# 非限定名查找 (Unqualified Name Lookup)

## 1. 概述 (Overview)

**非限定名 (Unqualified Name)** 是指出现在作用域解析运算符 `::` 右侧以外的名称。非限定名查找是一种从当前作用域开始，逐层向外查找名称声明的机制。

### 查找原则

当编译器遇到非限定名时，它会按照特定顺序检查各个作用域，直到找到至少一个任意类型的声明，此时查找停止，不再检查后续作用域。

### 重要说明

- **查找跳过规则**：某些上下文会跳过特定声明。例如，`::` 左侧的名称查找会忽略函数、变量和枚举器声明；基类说明符中的名称查找会忽略所有非类型声明。
- **using-directive 效果**：对于非限定名查找，using-directive 引入的命名空间中的所有声明，被视为声明在包含 using-directive 和被引入命名空间的最接近的外围命名空间中。
- **参数依赖查找 (ADL)**：函数调用运算符左侧名称的非限定名查找由参数依赖查找规则描述。

## 2. 来源与演变 (Origin and Evolution)

### 设计动机

非限定名查找机制的设计旨在解决以下问题：

1. **名称可见性** - 确定在特定位置哪些名称是可见的
2. **作用域层次** - 建立从内到外的查找顺序
3. **名称冲突解决** - 当多个作用域存在同名实体时的优先级规则
4. **继承与多态** - 处理派生类中成员名称的查找

### 标准演进

| C++ 版本 | 变更内容 |
|---------|---------|
| C++98 | 初始定义，建立基本的查找规则体系 |
| C++11 | 重构成员名称查找规则，引入查找集 (lookup set) 概念 |
| CWG 490 | 修正友元成员函数声明中模板参数名称的查找范围 |
| CWG 514 | 修正命名空间外定义命名空间成员时的查找顺序 |

### 缺陷报告详解

**CWG 490**：C++98 标准规定友元成员函数声明中，模板参数中的任何名称都不会在成员函数所在类的作用域中查找。修正后规定只有声明标识符中的模板参数才被排除在该查找之外。

**CWG 514**：C++98 标准规定命名空间作用域中的任何非限定名首先在该命名空间中查找。修正后规定在命名空间外定义该命名空间的变量成员时，非限定名首先在该命名空间中查找。

## 3. 语法与参数 (Syntax and Parameters)

### 基本查找规则

非限定名查找遵循"由内向外"的原则，依次检查以下作用域：

```
当前作用域 → 外围作用域 → ... → 全局作用域
```

一旦找到声明，查找立即终止。

### 各上下文的查找范围

| 上下文 | 查找范围顺序 |
|-------|-------------|
| 文件作用域 | 使用点之前的全局作用域 |
| 命名空间作用域 | 当前命名空间 → 外围命名空间 → 全局作用域 |
| 函数体 | 块作用域 → 外围块 → 函数所在命名空间 → 外围命名空间 |
| 类定义 | 类体 → 基类体 → 外围类 → 命名空间 |
| 成员函数体 | 块作用域 → 整个类体 → 基类 → 命名空间 |
| 默认参数 | 参数名 → 外围作用域 |
| 枚举器初始化 | 同枚举中之前的枚举器 → 外围作用域 |

## 4. 底层原理 (Underlying Principles)

### 4.1 文件作用域查找

对于全局（顶层命名空间）作用域中的名称，在函数、类或用户声明的命名空间之外使用时，检查使用点之前的全局作用域：

```cpp
int n = 1;     // n 的声明
int x = n + 1; // OK：查找找到 ::n

int z = y - 1; // 错误：查找失败
int y = 2;     // y 的声明
```

### 4.2 命名空间作用域查找

在用户声明的命名空间中，在函数或类之外使用名称时：

1. 检查使用点之前的当前命名空间
2. 检查声明该命名空间之前的外围命名空间
3. 递归检查直到全局命名空间

```cpp
int n = 1;

namespace N
{
    int m = 2;

    namespace Y
    {
        int x = n; // OK，查找找到 ::n
        int y = m; // OK，查找找到 ::N::m
        int z = k; // 错误：查找失败
    }

    int k = 3;
}
```

### 4.3 命名空间外定义成员

在命名空间外定义命名空间成员变量时，查找方式与在命名空间内部相同：

```cpp
namespace X
{
    extern int x; // 声明，非定义
    int n = 1;    // 第 1 个找到
}

int n = 2;        // 第 2 个找到
int X::x = n;     // 找到 X::n，将 X::x 设置为 1
```

### 4.4 非成员函数定义中的查找

对于非成员函数定义中的名称（函数体或默认参数），查找顺序：

1. 使用点所在的块（使用点之前）
2. 外围块（该块开始之前）
3. ...直到函数体块
4. 函数所在命名空间（直到函数定义点）
5. 外围命名空间...

```cpp
namespace A
{
    namespace N
    {
        void f();
        int i = 3; // 第 3 找到（如果第 2 不存在）
    }

    int i = 4;     // 第 4 找到（如果第 3 不存在）
}

int i = 5;         // 第 5 找到（如果第 4 不存在）

void A::N::f()
{
    int i = 2;     // 第 2 找到（如果第 1 不存在）

    while (true)
    {
       int i = 1;  // 第 1 找到：查找完成
       std::cout << i;
    }
}
```

### 4.5 类定义中的查找

对于类定义中使用的名称（不包括成员函数体、默认参数、异常说明、默认成员初始化器），查找范围：

1. 使用点之前的类体
2. 基类的整个体（递归进入其基类）
3. 如果是嵌套类：外围类体（直到该类定义点）及其基类
4. 如果是局部类或嵌套在局部类中：定义点之前的块作用域
5. 如果类是命名空间成员：命名空间作用域（直到类定义点）→ 外围命名空间 → 全局作用域

**友元声明例外**：查找确定是否引用先前声明的实体时，在内层外围命名空间后停止。

```cpp
namespace M
{
    class B
    {
        // static const int i = 3; // 第 3 找到（但不能通过访问检查）
    };
}

// const int i = 5;              // 第 5 找到

namespace N
{
    // const int i = 4;          // 第 4 找到

    class Y : public M::B
    {
        // static const int i = 2; // 第 2 找到

        class X
        {
            // static const int i = 1; // 第 1 找到
            int a[i]; // i 的使用
        };
    };
}
```

### 4.6 注入类名 (Injected Class Name)

在类或类模板的定义中，或在其派生类中使用的类名或类模板名，非限定名查找找到正在定义的类，就像该名称是通过成员声明引入的（具有 public 成员访问权限）。

### 4.7 成员函数定义中的查找

对于成员函数体、默认参数、异常说明或默认成员初始化器中使用的名称：

- 与类定义中相同，但搜索类的**整个作用域**（不只是使用点之前的部分）
- 对于嵌套类，搜索外围类的整个体

```cpp
class B
{
    // int i;         // 第 3 找到
};

namespace M
{
    // int i;         // 第 5 找到

    namespace N
    {
        // int i;     // 第 4 找到

        class X : public B
        {
            // int i; // 第 2 找到
            void f();
            // int i; // 同样第 2 找到
        };
    }
}

// int i;             // 第 6 找到

void M::N::X::f()
{
    // int i;         // 第 1 找到
    i = 16;
}
```

### 4.8 虚继承中的支配规则（C++11 起）

C++11 采用查找集 (lookup set) 机制处理成员名称查找：

**查找集构建规则**：

1. 为类 C 构建查找集（声明集合 + 声明所在的子对象）
2. 如果 C 中声明列表为空，为每个直接基类 Bi 构建查找集
3. 将基类的查找集合并到 C 的查找集

**合并规则**：

| 条件 | 操作 |
|-----|------|
| Bi 中声明集为空 | 丢弃 Bi 的查找集 |
| C 当前查找集为空 | 用 Bi 的查找集替换 |
| Bi 中每个子对象都是 C 已添加子对象的基类 | 丢弃 Bi 的查找集 |
| C 中已添加的每个子对象都是 Bi 中某子对象的基类 | 用 Bi 的查找集替换 C 的查找集 |
| Bi 和 C 的声明集不同 | 歧义合并（可能有无效声明） |
| 其他情况 | 共享声明集，子对象取并集 |

```cpp
struct X { void f(); };

struct B1: virtual X { void f(); };

struct B2: virtual X {};

struct D : B1, B2
{
    void foo()
    {
        X::f(); // OK，调用 X::f（限定查找）
        f();    // OK，调用 B1::f（非限定查找）
    }
};

// C++11 查找过程：
// 1. D 中查找 f 无结果
// 2. B1 中查找找到 B1::f，查找集完成
// 3. 合并到 D，D 的查找集包含 B1::f（在 B1 中）
// 4. B2 中查找 f 无结果，进入基类 X 找到 X::f
// 5. 合并到 D 时，B2 查找集中的 X 是 B1 的基类，丢弃 B2 的查找集
// 最终 D 的查找集只包含 B1::f
```

### 4.9 非虚基类中静态成员的特殊规则

当查找找到静态成员、嵌套类型或枚举器时，即使继承树中存在多个非虚基类子对象，查找也是无歧义的：

```cpp
struct V { int v; };

struct B
{
    int a;
    static int s;
    enum { e };
};

struct B1 : B, virtual V {};
struct B2 : B, virtual V {};
struct D : B1, B2 {};

void f(D& pd)
{
    ++pd.v;       // OK：只有一个 v（虚基类只有一个子对象）
    ++pd.s;       // OK：只有一个静态 B::s
    int i = pd.e; // OK：只有一个枚举器 B::e
    ++pd.a;       // 错误，歧义：B1 中的 B::a 和 B2 中的 B::a
}
```

### 4.10 模板定义中的查找

#### 非依赖名称

非限定名查找在模板定义检查时进行。此时绑定的声明不受实例化点可见声明的影响。

#### 依赖名称

查找推迟到模板参数已知时：

- **ADL 查找**：检查模板定义上下文和模板实例化上下文中可见的函数声明
- **非 ADL 查找**：只检查模板定义上下文中可见的函数声明

**重要规则**：如果基类依赖于模板参数，其作用域不会被非限定名查找检查。

```cpp
void f(char); // f 的第一个声明

template<class T>
void g(T t)
{
    f(1);    // 非依赖名称：现在绑定到 ::f(char)
    f(T(1)); // 依赖名称：查找推迟
    f(t);    // 依赖名称：查找推迟
}

enum E { e };
void f(E);   // f 的第二个声明
void f(int); // f 的第三个声明

void h()
{
    g(e);  // 实例化 g<E>
           // 依赖名称 f 查找找到 ::f(char)（普通查找）和 ::f(E)（ADL）
           // 重载决议选择 ::f(E)

    g(32); // 实例化 g<int>
           // 依赖名称 f 查找只找到 ::f(char)
           // 重载决议选择 ::f(char)
}

typedef double A;

template<class T>
class B
{
    typedef int A;
};

template<class T>
struct X : B<T>
{
    A a; // 查找 A 找到 ::A (double)，而不是 B<T>::A
};
```

## 5. 使用场景 (Use Cases)

### 5.1 友元函数定义中的查找

**在类体内定义的友元函数**：查找方式与成员函数相同。

**在类体外定义的友元函数**：查找方式与命名空间中的函数相同。

```cpp
int i = 3;                     // 对 f1 第 3 找到，对 f2 第 2 找到

struct X
{
    static const int i = 2;    // 对 f1 第 2 找到，对 f2 永远找不到

    friend void f1(int x)
    {
        // int i;              // 第 1 找到
        i = x;                 // 找到并修改 X::i
    }

    friend int f2();
};

void f2(int x)
{
    // int i;                  // 第 1 找到
    i = x;                     // 找到并修改 ::i
}
```

### 5.2 友元函数声明中的查找

当友元声明引用另一个类的成员函数时：

1. 首先检查成员函数所在类的整个作用域
2. 如果未找到（或名称是声明标识符中模板参数的一部分），继续在授权友元的类中查找

```cpp
template<class T>
struct S;

struct A
{
    typedef int AT;

    void f1(AT);
    void f2(float);

    template<class T>
    void f3();

    void f4(S<AT>);
};

struct B
{
    typedef char AT;
    typedef float BT;

    friend void A::f1(AT);    // AT 找到 A::AT（在 A 中找到）
    friend void A::f2(BT);    // BT 找到 B::BT（在 A 中未找到）
    friend void A::f3<AT>();  // AT 找到 B::AT（AT 在声明标识符中，不查 A）
};

template<class AT>
struct C
{
    friend void A::f4(S<AT>); // AT 找到 A::AT（AT 不在声明标识符中）
};
```

### 5.3 默认参数中的查找

函数声明中默认参数的名称查找：

1. 首先查找函数参数名
2. 然后查找外围块、类或命名空间作用域

```cpp
class X
{
    int a, b, i, j;
public:
    const int& r;

    X(int i): r(a),      // 初始化 X::r 引用 X::a
              b(i),      // 初始化 X::b 为参数 i 的值
              i(i),      // 初始化 X::i 为参数 i 的值
              j(this->i) // 初始化 X::j 为 X::i 的值
    {}
};

int a;
int f(int a, int b = a); // 错误：查找 a 找到参数 a，不是 ::a
                         // 参数不能用作默认参数
```

### 5.4 静态数据成员定义中的查找

与成员函数定义中的查找方式相同：

```cpp
struct X
{
    static int x;
    static const int n = 1; // 第 1 找到
};

int n = 2;                  // 第 2 找到
int X::x = n;               // 找到 X::n，设置 X::x 为 1，不是 2
```

### 5.5 枚举器声明中的查找

枚举器初始化部分使用的名称：

1. 首先查找同一枚举中之前声明的枚举器
2. 然后进行常规的非限定名查找

```cpp
const int RED = 7;

enum class color
{
    RED,
    GREEN = RED + 2, // RED 找到 color::RED，不是 ::RED，所以 GREEN = 2
    BLUE = ::RED + 4 // 限定查找找到 ::RED，BLUE = 11
};
```

### 5.6 函数 try 块处理器中的查找

处理器中的名称查找如同在函数体最外层块的最开始进行：

- 函数参数可见
- 最外层块中声明的名称不可见

```cpp
int n = 3;          // 第 3 找到
int f(int n = 2)    // 第 2 找到
try
{
    int n = -1;     // 永远找不到
}
catch(...)
{
    // int n = 1;   // 第 1 找到
    assert(n == 2); // 查找 n 找到函数参数
    throw;
}
```

### 5.7 重载运算符的查找

表达式中的运算符查找与显式函数调用不同：

**表达式形式** (`a + b`)：执行两次独立查找
- 非成员运算符重载
- 成员运算符重载
- 与内置运算符一起参与重载决议

**显式调用形式** (`operator+(a, b)`)：执行常规非限定名查找

```cpp
struct A {};
void operator+(A, A);  // 用户定义的非成员 operator+

struct B
{
    void operator+(B); // 用户定义的成员 operator+
    void f();
};

A a;

void B::f()
{
    operator+(a, a); // 错误：从成员函数进行常规名称查找
                     // 找到 B 作用域中的 operator+ 声明
                     // 停止查找，永远无法到达全局作用域

    a + a; // OK：成员查找找到 B::operator+，非成员查找找到 ::operator+(A, A)
           // 重载决议选择 ::operator+(A, A)
}
```

## 6. 代码示例 (Examples)

### 6.1 基础用法：命名空间中的查找

```cpp
#include <iostream>

int value = 100;

namespace Outer {
    int value = 200;

    namespace Inner {
        void printValues() {
            int value = 300;
            std::cout << "Local value: " << value << std::endl;     // 300
            std::cout << "Outer value: " << Outer::value << std::endl; // 200
            std::cout << "Global value: " << ::value << std::endl;    // 100
        }
    }
}

int main() {
    Outer::Inner::printValues();
    return 0;
}
```

### 6.2 基础用法：类定义中的查找

```cpp
#include <iostream>

int i = 5;

struct Base {
    int i = 3;
};

struct Derived : Base {
    int i = 2;

    struct Nested {
        int arr[i];  // i 找到哪个？
        // 在类定义中，查找顺序：
        // 1. Nested 类体（使用点之前）- 无
        // 2. 基类体 - 无
        // 3. 外围类 Derived（直到 Nested 定义点）- 找到 Derived::i = 2
        // 所以 arr 的大小为 2
    };
};

int main() {
    std::cout << "Array size: " << sizeof(Derived::Nested().arr) / sizeof(int) << std::endl;
    // 输出: Array size: 2
    return 0;
}
```

### 6.3 高级用法：成员函数中的查找范围

```cpp
#include <iostream>

int x = 100;

class Base {
public:
    int x = 10;
    void show() { std::cout << "Base::x = " << x << std::endl; }
};

class Derived : public Base {
public:
    int x = 20;
    int y;  // 声明在后面

    void test() {
        std::cout << "Derived::x = " << x << std::endl;   // 20 - 找到 Derived::x
        std::cout << "Base::x = " << Base::x << std::endl; // 10 - 限定查找
        std::cout << "y = " << y << std::endl;            // 找到 Derived::y
        // 成员函数中查找整个类体，不限于使用点之前
    }

    int y = x + 5;  // x 找到 Derived::x = 20，所以 y = 25
};

int main() {
    Derived d;
    d.test();
    std::cout << "y value: " << d.y << std::endl;
    return 0;
}
```

### 6.4 高级用法：虚继承中的支配规则

```cpp
#include <iostream>

struct A { void f() { std::cout << "A::f()\n"; } };

struct B1 : virtual A { void f() { std::cout << "B1::f()\n"; } };

struct B2 : virtual A { };

struct C : B1, B2 {
    void test() {
        f();  // 调用 B1::f()，不是 A::f()
    }
};

int main() {
    C c;
    c.test();  // 输出: B1::f()

    // 理解：B1::f 隐藏了虚基类 A 中的 f
    return 0;
}
```

### 6.5 高级用法：模板中的查找时机

```cpp
#include <iostream>

void g(int) { std::cout << "g(int)\n"; }

template<typename T>
void call_g(T t) {
    g(t);  // 依赖名称，查找推迟到实例化时
}

void g(double) { std::cout << "g(double)\n"; }

struct MyType {};
void g(MyType) { std::cout << "g(MyType) - ADL\n"; }

int main() {
    call_g(42);    // 实例化 call_g<int>
                   // 只找到 g(int)，输出: g(int)

    call_g(3.14);  // 实例化 call_g<double>
                   // 只找到 g(int)（定义时可见），输出: g(int)

    call_g(MyType{});  // 实例化 call_g<MyType>
                       // 找到 g(int)（定义时）和 g(MyType)（ADL）
                       // 重载决议选择 g(MyType)，输出: g(MyType) - ADL
    return 0;
}
```

### 6.6 常见错误及修正

**错误示例 1：依赖基类的成员不可见**

```cpp
// 错误代码
template<typename T>
struct Base {
    int value;
};

template<typename T>
struct Derived : Base<T> {
    void foo() {
        value = 10;  // 错误：value 未声明
    }
};
```

```cpp
// 修正方法 1：使用 this-> 指针
template<typename T>
struct Base {
    int value;
};

template<typename T>
struct Derived : Base<T> {
    void foo() {
        this->value = 10;  // OK：通过 this-> 访问
    }
};
```

```cpp
// 修正方法 2：使用限定名
template<typename T>
struct Base {
    int value;
};

template<typename T>
struct Derived : Base<T> {
    void foo() {
        Base<T>::value = 10;  // OK：使用限定名
    }
};
```

**错误示例 2：默认参数中误用参数名**

```cpp
// 错误代码
int value = 100;

void func(int value, int x = value) {  // 错误：参数不能作为默认参数
    // ...
}
```

```cpp
// 修正方法：使用全局变量或其他表达式
int value = 100;

void func(int v, int x = value) {  // OK：使用全局变量
    // ...
}

// 或者使用常量
void func2(int v, int x = 0) {  // OK：使用常量
    // ...
}
```

**错误示例 3：运算符表达式与显式调用的区别**

```cpp
// 这不是错误，但需要理解行为差异
#include <iostream>

struct A {};

void operator+(A, A) {
    std::cout << "Global operator+\n";
}

struct B {
    void operator+(B) {
        std::cout << "Member operator+\n";
    }

    void test() {
        A a;
        // a + a;           // OK：表达式形式，两次独立查找
        // operator+(a, a); // 错误：显式调用，只查找到 B::operator+
    }
};

int main() {
    B b1, b2;
    b1 + b2;  // 表达式形式：找到成员和非成员版本
              // 但参数类型不匹配，调用成员版本

    A a1, a2;
    a1 + a2;  // 表达式形式：只找到全局 operator+
              // 输出: Global operator+
    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **查找原则**：非限定名查找从当前作用域开始，由内向外逐层查找，找到第一个声明即停止

2. **上下文相关**：不同上下文（类定义、成员函数、模板等）有不同的查找规则

3. **关键特性**：
   - 成员函数中查找整个类体，不限于使用点之前
   - 模板中非依赖名称在定义时查找，依赖名称在实例化时查找
   - 依赖模板参数的基类作用域不被查找

4. **特殊规则**：
   - 枚举器初始化优先查找同枚举中之前的枚举器
   - 函数 try 块处理器中函数参数可见
   - 重载运算符表达式执行两次独立查找

### 与限定名查找的对比

| 特性 | 非限定名查找 | 限定名查找 |
|-----|------------|-----------|
| 查找范围 | 从当前作用域向外逐层查找 | 明确指定的作用域 |
| 查找终止条件 | 找到任意声明 | 在指定作用域查找 |
| 受局部名称影响 | 是 | 否 |
| ADL 适用性 | 适用（函数调用） | 不适用 |
| 虚函数分发 | 触发（成员函数） | 不触发 |

### 相关概念

- **限定名查找 (Qualified Name Lookup)** - 使用 `::` 明确指定作用域
- **参数依赖查找 (Argument-Dependent Lookup, ADL)** - 基于函数参数类型的查找
- **注入类名 (Injected-Class-Name)** - 类名在自身作用域内可见
- **依赖名称 (Dependent Name)** - 依赖于模板参数的名称

### 学习建议

1. 理解各种上下文中查找规则的差异是掌握 C++ 名称查找的关键
2. 特别注意模板中依赖基类成员的访问问题
3. 了解运算符表达式与显式函数调用的查找差异
4. 在复杂继承结构中，理解支配规则对成员查找的影响