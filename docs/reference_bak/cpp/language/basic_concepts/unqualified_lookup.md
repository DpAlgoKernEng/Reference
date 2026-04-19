# C++ 未限定名称查找完全详解

  [1. 未限定名称查找基本概念](#1-未限定名称查找基本概念)
  
  [2. 查找规则与作用域](#2-查找规则与作用域)
  
  [3. 各种上下文中的未限定名称查找](#3-各种上下文中的未限定名称查找)
  
  [4. 类成员查找与支配规则](#4-类成员查找与支配规则)
  
  [5. 模板中的未限定名称查找](#5-模板中的未限定名称查找)
  
  [6. 特殊查找场景](#6-特殊查找场景)
  
  [7. 最佳实践](#7-最佳实践)
  
  [8. 标准参考](#8-标准参考)
  
  [9. 总结](#9-总结)

## 1. 未限定名称查找基本概念

### 1.1 什么是未限定名称查找

未限定名称查找（Unqualified Name Lookup）是指对于一个不出现作用域解析运算符 `::` 右侧的名称进行的查找过程。查找会按照特定的顺序检查作用域，直到找到至少一个任意类型的声明，此时查找停止，不再检查其他作用域。

```cpp
#include <iostream>

// 全局作用域
int global_var = 100;

namespace example {
    // 命名空间作用域
    int ns_var = 200;
    
    void function() {
        // 局部作用域
        int local_var = 300;
        
        // 未限定名称查找示例
        std::cout << global_var << std::endl;  // 查找全局作用域
        std::cout << ns_var << std::endl;      // 查找当前命名空间作用域
        std::cout << local_var << std::endl;   // 查找局部作用域
    }
}
```

### 1.2 查找的基本原则

未限定名称查找遵循以下基本原则：

- 从内到外查找：从当前作用域开始，逐步向外层作用域查找
- 找到即停止：一旦找到匹配的声明，立即停止查找
- using 指令扩展：通过 `using` 指令引入的命名空间声明被视为在包含该指令的命名空间中声明

```cpp
#include <iostream>

int x = 100;  // 全局变量

namespace outer {
    int x = 200;  // outer 命名空间中的变量
    
    namespace inner {
        int x = 300;  // inner 命名空间中的变量
        
        void demonstrate_lookup() {
            // 未限定名称查找顺序：
            // 1. 当前作用域 (函数内部)
            // 2. 包含作用域 (inner 命名空间)
            // 3. 外层作用域 (outer 命名空间)
            // 4. 全局作用域
            
            int x = 400;  // 局部变量，遮蔽所有外层变量
            std::cout << "Local x: " << x << std::endl;  // 输出 400
            
            {
                int x = 500;  // 更内层的局部变量
                std::cout << "Inner local x: " << x << std::endl;  // 输出 500
            }
            
            std::cout << "Local x: " << x << std::endl;  // 输出 400
        }
    }
    
    void outer_function() {
        // 在 outer 命名空间中查找
        std::cout << "outer::x: " << x << std::endl;  // 输出 200
    }
}

void global_function() {
    // 在全局作用域中查找
    std::cout << "Global x: " << x << std::endl;  // 输出 100
}
```

## 2. 查找规则与作用域

### 2.1 文件作用域查找

对于在全局（顶级命名空间）作用域中使用的名称，在函数、类或用户声明的命名空间之外，会检查该名称使用之前的全局作用域：

```cpp
#include <iostream>

int n = 1;     // declaration of n
int x = n + 1; // OK: lookup finds ::n

// int z = y - 1; // Error: lookup fails - y 尚未声明
int y = 2;     // declaration of y

void file_scope_example() {
    std::cout << "n = " << n << std::endl;  // 1
    std::cout << "x = " << x << std::endl;  // 2
    std::cout << "y = " << y << std::endl;  // 2
}
```

### 2.2 命名空间作用域查找

对于在用户声明的命名空间中使用的名称，在函数或类之外，会先搜索该名称使用之前的命名空间，然后搜索包含此命名空间的命名空间（在声明此命名空间之前），以此类推，直到达到全局命名空间。

```cpp
#include <iostream>

int n = 1; // 全局声明

namespace N {
    int m = 2;  // N 命名空间中的声明
    
    namespace Y {
        int x = n; // OK, lookup finds ::n
        int y = m; // OK, lookup finds ::N::m
        // int z = k; // Error: lookup fails - k 尚未声明
    }
    
    int k = 3;  // N 命名空间中的声明
}

void namespace_scope_example() {
    std::cout << "Global n: " << n << std::endl;           // 1
    std::cout << "N::m: " << N::m << std::endl;            // 2
    std::cout << "N::Y::x: " << N::Y::x << std::endl;      // 1
    std::cout << "N::Y::y: " << N::Y::y << std::endl;      // 2
    std::cout << "N::k: " << N::k << std::endl;            // 3
}
```

### 2.3 using 指令的影响

通过 `using` 指令指定的命名空间的全部声明，就好像是在包含 `using` 指令和指定命名空间（直接或间接包含）的最近外围命名空间中声明的。

```cpp
#include <iostream>

namespace A {
    int a = 10;
    void func_a() { std::cout << "A::func_a\n"; }
}

namespace B {
    int b = 20;
    void func_b() { std::cout << "B::func_b\n"; }
}

namespace C {
    using namespace A;  // 将 A 中的声明引入 C
    using namespace B;  // 将 B 中的声明引入 C
    
    void demonstrate_using_directive() {
        // 现在可以在 C 中直接访问 A 和 B 中的声明
        std::cout << "a = " << a << std::endl;  // 访问 A::a
        std::cout << "b = " << b << std::endl;  // 访问 B::b
        func_a();  // 调用 A::func_a
        func_b();  // 调用 B::func_b
    }
}

// using 指令的传递性
namespace D {
    using namespace C;  // 间接引入 A 和 B 的声明
    
    void demonstrate_transitive_using() {
        std::cout << "Transitive access: a = " << a << ", b = " << b << std::endl;
    }
}
```

## 3. 各种上下文中的未限定名称查找

### 3.1 非成员函数定义中的查找

对于在函数定义中使用（包括在函数体中或作为默认参数的一部分），且该函数是用户声明或全局命名空间成员的名称，在使用名称之前会先搜索名称所在的块，然后在该块开始之前搜索外围块，以此类推，直到函数体所在的块。接着搜索函数声明的命名空间，直到使用该名称的函数定义（不一定是声明），然后搜索外围命名空间等。

```cpp
#include <iostream>

namespace A {
    namespace N {
        void f();
        int i = 3; // found 3rd (if 2nd is not present)
    }
    
    int i = 4;     // found 4th (if 3rd is not present)
}

int i = 5;         // found 5th (if 4th is not present)

void A::N::f() {
    int i = 2;     // found 2nd (if 1st is not present)
    
    while (true) {
        int i = 1;  // found 1st: lookup is done
        std::cout << "Innermost i: " << i << std::endl;  // 输出 1
        break;
    }
    
    std::cout << "Local i: " << i << std::endl;  // 输出 2
}

// int i;          // not found - 在函数定义之后声明

namespace A {
    namespace N {
        // int i;  // not found - 在函数定义之后声明
    }
}

void function_lookup_example() {
    A::N::f();
    std::cout << "Global i: " << i << std::endl;  // 输出 5
}
```

### 3.2 类定义中的查找

对于在类定义中任何位置使用的名称（包括基类指定符和嵌套类定义），除了在成员函数体内部、成员函数的默认参数、成员函数的异常规范或默认成员初始化器内部，其中成员可能属于定义在包含类体内的嵌套类，将搜索以下作用域：

```cpp
#include <iostream>

namespace M {
    // const int i = 1; // never found
    
    class B {
        // static const int i = 3;     // found 3rd (but will not pass access check)
    };
}

// const int i = 5;                    // found 5th

namespace N {
    // const int i = 4;                // found 4th
    
    class Y : public M::B {
        // static const int i = 2;     // found 2nd
        
        class X {
            // static const int i = 1; // found 1st
            // int a[i]; // use of i
            // static const int i = 1; // never found
        };
        
        // static const int i = 2;     // never found
    };
    
    // const int i = 4;                // never found
}

// const int i = 5;                    // never found

void class_definition_lookup() {
    // 示例说明查找顺序，实际代码需要根据具体需求调整
    std::cout << "Class definition lookup example\n";
}
```

### 3.3 成员函数定义中的查找

对于在成员函数体内使用的名称、成员函数的默认参数、成员函数的异常规范或默认成员初始化器，搜索的范围与类定义中的范围相同，只是将整个类的范围都考虑在内，而不仅仅是使用该名称声明之前的部分。对于嵌套类，将搜索包含类的整个主体。

```cpp
#include <iostream>

class B {
    // int i;         // found 3rd
};

namespace M {
    // int i;         // found 5th
    
    namespace N {
        // int i;     // found 4th
        
        class X : public B {
            // int i; // found 2nd
            void f();
            // int i; // found 2nd as well
        };
        
        // int i;     // found 4th
    }
}

// int i;             // found 6th

void M::N::X::f() {
    // int i;         // found 1st
    // i = 16;
    // int i;         // never found
    
    std::cout << "Member function lookup example\n";
}

namespace M {
    namespace N {
        // int i;     // never found
    }
}

void member_function_lookup_example() {
    M::N::X obj;
    obj.f();
}
```

## 4. 类成员查找与支配规则

### 4.1 支配规则详解

在检查类派生的基类时，将遵循以下规则，有时被称为虚继承中的支配：

```cpp
#include <iostream>

struct X { 
    void f() { 
        std::cout << "X::f()\n"; 
    } 
};

struct B1: virtual X { 
    void f() { 
        std::cout << "B1::f()\n"; 
    } 
};

struct B2: virtual X {};

struct D : B1, B2 {
    void foo() {
        X::f(); // OK, calls X::f (qualified lookup)
        f();    // OK, calls B1::f (unqualified lookup)
    }
};

// C++98 rules: B1::f hides X::f, so even though X::f can be reached from D
// through B2, it is not found by name lookup from D.

// C++11 rules: lookup set for f in D finds nothing, proceeds to bases
//  lookup set for f in B1 finds B1::f, and is completed
// merge replaces the empty set, now lookup set for f in C has B1::f in B1
//  lookup set for f in B2 finds nothing, proceeds to bases
//    lookup for f in X finds X::f
//  merge replaces the empty set, now lookup set for f in B2 has X::f in X
// merge into C finds that every subobject (X) in the lookup set in B2 is a base
// of every subobject (B1) already merged, so the B2 set is discarded
// C is left with just B1::f found in B1

void dominance_rule_example() {
    D obj;
    obj.foo();
}
```

### 4.2 静态成员与枚举器的查找

未限定名称查找能找到 B 的静态成员、B 的嵌套类型以及 B 中声明的枚举器，即使在被检查类的继承树中有多个类型为 B 的非虚基子对象，这种未限定名称查找也是明确的：

```cpp
#include <iostream>

struct V { 
    int v; 
};

struct B {
    int a;
    static int s;
    enum { e };
};

struct B1 : B, virtual V {};
struct B2 : B, virtual V {};
struct D : B1, B2 {};

// 静态成员定义
int B::s = 100;

void static_member_lookup_example(D& pd) {
    ++pd.v;       // OK: only one v because only one virtual base subobject
    ++pd.s;       // OK: only one static B::s, even though found in both B1 and B2
    int i = pd.e; // OK: only one enumerator B::e, even though found in both B1 and B2
    // ++pd.a;    // error, ambiguous: B::a in B1 and B::a in B2 
    
    std::cout << "v = " << pd.v << std::endl;
    std::cout << "s = " << pd.s << std::endl;
    std::cout << "e = " << i << std::endl;
}

void test_static_lookup() {
    D obj;
    obj.v = 42;
    static_member_lookup_example(obj);
}
```

## 5. 模板中的未限定名称查找

### 5.1 非依赖名称与依赖名称

对于在模板定义中使用的非依赖名称，当模板定义被检查时会发生未限定名称查找。对于在模板定义中使用的依赖名称，查找被推迟到模板参数已知时。

```cpp
#include <iostream>

void f(char) { 
    std::cout << "f(char)\n"; 
} // first declaration of f

template<class T> 
void g(T t) {
    f(1);    // non-dependent name: lookup finds ::f(char) and binds it now
    f(T(1)); // dependent name: lookup postponed
    f(t);    // dependent name: lookup postponed
    // dd++; // non-dependent name: lookup finds no declaration
}

enum E { e };
void f(E) { 
    std::cout << "f(E)\n"; 
}   // second declaration of f
void f(int) { 
    std::cout << "f(int)\n"; 
}  // third declaration of f
double dd;

void template_lookup_example() {
    g(e);  // instantiates g<E>, at which point
           // the second and the third uses of the name 'f'
           // are looked up and find ::f(char) (by lookup) and ::f(E) (by ADL)
           // then overload resolution chooses ::f(E).
           // This calls f(char), then f(E) twice
    
    g(32); // instantiates g<int>, at which point
           // the second and the third uses of the name 'f'
           // are looked up and find ::f(char) only
           // then overload resolution chooses ::f(char)
           // This calls f(char) three times
}

void test_template_lookup() {
    template_lookup_example();
}
```

### 5.2 模板中的作用域查找

```cpp
#include <iostream>

typedef double A;

template<class T>
class B {
    typedef int A;
};

template<class T>
struct X : B<T> {
    A a; // lookup for A finds ::A (double), not B<T>::A
};

void template_scope_example() {
    X<int> obj;
    // obj.a 的类型是 double，不是 int
    std::cout << "Template scope lookup example\n";
}
```

## 6. 特殊查找场景

### 6.1 友元函数定义中的查找

对于一个在授予友元的类的体内使用的友元函数定义中的名称，未限定名称查找与成员函数的查找方式相同。对于一个在类体外定义的友元函数中的名称，未限定名称查找与命名空间中函数的查找方式相同。

```cpp
#include <iostream>

int i = 3;                     // found 3rd for f1, found 2nd for f2

struct X {
    static const int i = 2;    // found 2nd for f1, never found for f2

    friend void f1(int x) {
        // int i;              // found 1st
        i = x;                 // finds and modifies X::i
        std::cout << "X::i = " << X::i << std::endl;
    }

    friend int f2();

    // static const int i = 2; // found 2nd for f1 anywhere in class scope
};

void f2(int x) {
    // int i;                  // found 1st
    i = x;                     // finds and modifies ::i
    std::cout << "::i = " << ::i << std::endl;
}

void friend_function_lookup_example() {
    f1(10);  // 修改 X::i
    f2(20);  // 修改 ::i
}
```

### 6.2 默认参数中的查找

在一个函数声明中用作默认参数的名称，或在一个构造函数的成员初始化器的表达式部分使用的名称，首先查找函数参数名称，然后再检查包含的代码块、类或命名空间作用域：

```cpp
#include <iostream>
#include <cassert>

class X {
    int a, b, i, j;
public:
    const int& r;

    X(int i): r(a),      // initializes X::r to refer to X::a
              b(i),      // initializes X::b to the value of the parameter i
              i(i),      // initializes X::i to the value of the parameter i
              j(this->i) // initializes X::j to the value of X::i
    {
        std::cout << "X::a = " << a << ", X::b = " << b 
                  << ", X::i = " << i << ", X::j = " << j << std::endl;
    }
};

int a = 100;

// int f(int a, int b = a); // error: lookup for a finds the parameter a, not ::a
                         // and parameters are not allowed as default arguments

void default_parameter_lookup_example() {
    X obj(42);
}
```

### 6.3 枚举器声明中的查找

在枚举的初始化部分使用的名称，首先会查找同一枚举中先前声明的枚举，然后未限定名称查找才会检查包含的块、类或命名空间作用域：

```cpp
#include <iostream>

const int RED = 7;

enum class color {
    RED,
    GREEN = RED + 2, // RED finds color::RED, not ::RED, so GREEN = 2
    BLUE = ::RED + 4 // qualified lookup finds ::RED, BLUE = 11
};

void enumerator_lookup_example() {
    std::cout << "color::RED = " << static_cast<int>(color::RED) << std::endl;
    std::cout << "color::GREEN = " << static_cast<int>(color::GREEN) << std::endl;
    std::cout << "color::BLUE = " << static_cast<int>(color::BLUE) << std::endl;
}
```

## 7. 最佳实践

### 7.1 避免名称冲突

```cpp
#include <iostream>

// 1. 使用命名空间组织代码
namespace project {
    namespace utility {
        void helper_function() {
            std::cout << "Utility helper function\n";
        }
    }
    
    namespace core {
        void core_function() {
            utility::helper_function();  // 明确指定命名空间
        }
    }
}

// 2. 避免过度使用 using 声明
void best_practices_example() {
    // 好的做法：明确限定
    std::cout << "Explicit qualification\n";
    
    // 避免在头文件中使用 using namespace
    // using namespace std;  // 不推荐在头文件中使用
    
    // 在实现文件中可以有限制地使用
    {
        using std::cout;
        using std::endl;
        cout << "Limited using declarations" << endl;
    }
}

// 3. 合理使用友元函数
class MathVector {
private:
    double x, y;
    
public:
    MathVector(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
    
    // 友元函数可以访问私有成员
    friend MathVector operator+(const MathVector& a, const MathVector& b) {
        return MathVector(a.x + b.x, a.y + b.y);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const MathVector& v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }
};

void friend_best_practices() {
    MathVector a(1, 2);
    MathVector b(3, 4);
    MathVector c = a + b;
    
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
}
```

### 7.2 模板中的最佳实践

```cpp
#include <iostream>
#include <type_traits>

// 1. 使用 typename 明确依赖类型
template<typename Container>
void process_container(const Container& c) {
    // 明确指出 value_type 是类型而非变量
    typename Container::value_type first_element = c.front();
    std::cout << "First element: " << first_element << std::endl;
}

// 2. 使用 std::enable_if 进行 SFINAE
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_if_integral(T value) {
    std::cout << "Integral value: " << value << std::endl;
}

template<typename T>
typename std::enable_if<!std::is_integral<T>::value, void>::type
print_if_integral(T value) {
    std::cout << "Non-integral value: " << value << std::endl;
}

void template_best_practices() {
    // std::vector<int> vec = {1, 2, 3, 4, 5};
    // process_container(vec);
    
    print_if_integral(42);
    print_if_integral(3.14);
}
```

## 8. 标准参考

C++ 未限定名称查找的相关标准条款：

- C++23 标准（ISO/IEC 14882:2024）：
  - 6.5 名称查找 [basic.lookup] (第 44-45 页)
  - 6.5.2 成员名称查找 [class.member.lookup] (第 45-47 页)
  - 13.8 名称解析 [temp.res] (第 399-403 页)
- C++20 标准 (ISO/IEC 14882:2020):
  - 6.5 命名查找 [basic.lookup] (p: 38-50)
  - 11.8 成员命名查找 [class.member.lookup] (p: 283-285)
  - 13.8 命名解析 [temp.res] (p: 385-400)
- C++17 标准 (ISO/IEC 14882:2017):
  - 6.4 命名查找 [basic.lookup] (p: 50-63)
  - 13.2 成员名查找 [class.member.lookup] (p: 259-262)
  - 17.6 名字解析 [temp.res] (p: 375-378)
- C++14 标准 (ISO/IEC 14882:2014):
  - 3.4 名字查找 [basic.lookup] (p: 42-56)
  - 10.2 成员名查找 [class.member.lookup] (p: 233-236)
  - 14.6 命名解析 [temp.res] (p: 346-359)
- C++11 标准（ISO/IEC 14882:2011）：
  - 3.4 命名查找 [basic.lookup]
  - 10.2 成员名查找 [class.member.lookup]
  - 14.6 命名解析 [temp.res]
- C++98 标准（ISO/IEC 14882:1998）：
  - 3.4 命名查找 [basic.lookup]
  - 10.2 成员命名查找 [class.member.lookup]
  - 14.6 命名解析 [temp.res]

相关的标准参考还包括：

- 3.3 作用域 [basic.scope]
- 6.6 名称解析 [basic.link]
- 13 重载 [over]
- 14 模板 [temp]
- 15 异常处理 [except]

## 9. 总结

C++ 未限定名称查找是编译器理解程序语义的核心机制之一，它决定了程序中每个未限定标识符的含义。深入理解未限定名称查找机制对于编写正确、高效的 C++ 程序至关重要。

### 9.1 核心要点回顾

**查找顺序**：

- 当前作用域（块、类、命名空间等）
- 外层作用域（逐层向外）
- 全局作用域

**关键概念**：

- using 指令：将命名空间声明视为在包含命名空间中声明
- 依赖名称与非依赖名称：模板中不同处理时机
- 支配规则：虚继承中名称解析的特殊规则
- 友元函数查找：特殊上下文中的查找规则

### 9.2 实践建议

- 明确限定：在可能的情况下使用明确的限定名称，提高代码可读性
- 合理使用命名空间：使用命名空间组织代码，避免全局名称污染
- 谨慎使用 using：避免在头文件中使用 `using namespace`
- 模板中的 typename：在模板中访问依赖类型时使用 `typename`
- 避免名称冲突：通过良好的命名约定和作用域管理避免冲突

### 9.3 历史演进

C++ 未限定名称查找机制在不同标准版本中持续演进：

- C++98/03：建立了基本的未限定名称查找框架
- C++11：完善了模板和虚继承相关的查找规则
- C++14：进一步澄清了复杂的查找场景
- C++17：改进了模板中的名称查找
- C++20：引入概念，提供了更强的模板约束机制
- C++23：继续完善边界情况的处理

通过深入理解和正确应用未限定名称查找规则，开发者可以编写出更加清晰、健壮和可维护的 C++ 程序，避免因名称解析错误导致的编译问题和运行时错误。
