# 重载决议 (Overload Resolution)

## 1. 概述 (Overview)

重载决议 (Overload Resolution) 是 C++ 编译器在编译函数调用时确定具体调用哪个重载函数的过程。当函数名通过名字查找后指向多个实体时，该函数名被称为**重载的 (overloaded)**，编译器必须决定调用哪个重载版本。简而言之，参数与实参匹配程度最高的重载版本将被调用。

重载决议是 C++ 支持函数重载的核心机制，它使得程序员可以使用相同的函数名定义多个函数，只要它们的参数类型不同。这是 C++ 实现多态性的重要手段之一。

### 核心流程

重载决议依次执行以下三个步骤：

1. **构建候选函数集合 (Candidate Functions)** - 收集所有可能被调用的函数
2. **筛选可行函数集合 (Viable Functions)** - 排除参数数量或类型不匹配的函数
3. **确定最佳可行函数 (Best Viable Function)** - 通过比较隐式转换序列的优劣，选出唯一的最佳匹配

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

函数重载是 C++ 相对于 C 语言的重要增强之一。在 C 语言中，同一作用域内不能存在同名函数，这导致程序员不得不为相似操作创建不同的函数名（如 `print_int`、`print_float`、`print_string` 等）。C++ 通过引入函数重载机制解决了这个问题，允许使用统一的函数名来处理不同类型的参数。

### 版本演进

| C++ 版本 | 主要变更 |
|---------|---------|
| C++98 | 确立了基本的重载决议规则 |
| C++11 | 引入列表初始化、移动语义相关的重载决议规则；弃用的字符串字面量到 `char*` 的转换被移除 |
| C++17 | 增加函数指针转换的重载决议支持；引入类模板参数推导 (CTAD) 相关的重载规则 |
| C++20 | 引入重写候选 (rewritten candidates) 机制，支持 `<=>` 和 `==` 运算符的自动合成；增加约束 (constraints) 检查 |
| C++23 | 静态成员函数的隐式对象参数不再参与重载决议排名；引入显式对象参数 (deducing this) |

### 设计动机

重载决议的设计目标包括：

- **类型安全性**：确保选择最匹配的函数，减少隐式转换带来的潜在错误
- **直觉性**：使函数调用的选择结果符合程序员的直觉预期
- **可扩展性**：支持用户定义类型的隐式转换参与重载决议
- **模板兼容性**：支持函数模板与普通函数之间的重载

## 3. 语法与参数 (Syntax and Parameters)

重载决议本身不涉及特定语法，但它决定了函数调用表达式的绑定结果。以下是与重载决议相关的关键概念：

### 3.1 候选函数 (Candidate Functions)

候选函数是名字查找和模板参数推导后得到的函数集合。根据调用上下文的不同，候选函数的来源也不同：

#### 命名函数调用

```cpp
void f(long);
void f(float);

f(0L);  // 调用 f(long)
f(0);   // 错误：二义性重载
```

#### 类对象调用 (函数调用运算符)

```cpp
struct A {
    using fp1 = int(*)(int);
    operator fp1() { return f1; }  // 转换函数
} a;

int i = a(1);  // 通过转换函数返回的函数指针调用
```

#### 运算符重载

运算符重载的候选函数包括：
- **成员候选**：类成员运算符函数
- **非成员候选**：通过名字查找找到的非成员运算符函数
- **内置候选**：内置运算符
- **重写候选** (C++20 起)：通过 `<=>` 和 `==` 合成的候选

### 3.2 可行函数 (Viable Functions)

可行函数必须满足以下条件：

1. 参数数量匹配（或有默认参数、省略号参数）
2. 每个实参都存在到对应形参类型的隐式转换序列
3. 引用参数能够正确绑定
4. 约束条件满足 (C++20 起)

### 3.3 最佳可行函数判定规则

按优先级从高到低：

| 优先级 | 规则 |
|-------|------|
| 1 | 各实参的隐式转换序列都不比其他候选差，且至少有一个更好 |
| 2 | 非类类型初始化时，结果的标准转换序列更好 |
| 3 | 引用绑定时，结果引用类型匹配更好 (C++11 起) |
| 4 | 非模板函数优于模板特化 |
| 5 | 更特化的模板优于更通用的模板 |
| 6 | 约束更严格的函数优于约束更宽松的函数 (C++20 起) |
| 7 | 派生类构造函数优于基类构造函数 (C++11 起) |
| 8 | 非重写候选优于重写候选 (C++20 起) |
| 9 | 非合成重写候选优于合成重写候选 (C++20 起) |
| 10 | 用户定义推导指南生成的函数优于其他 (C++17 起) |
| 11 | 复制推导候选优于其他 (C++17 起) |

## 4. 底层原理 (Underlying Principles)

### 4.1 隐式转换序列 (Implicit Conversion Sequence)

隐式转换序列用于比较实参与形参的匹配程度。每个隐式转换序列最多包含三个部分：

```
标准转换序列 -> 用户定义转换 -> 标准转换序列
```

#### 标准转换序列的等级

| 等级 | 包含的转换类型 |
|-----|---------------|
| **精确匹配 (Exact Match)** | 无转换、左值到右值转换、限定转换、函数指针转换 (C++17 起)、同类转换 |
| **提升 (Promotion)** | 整型提升、浮点提升 |
| **转换 (Conversion)** | 整型转换、浮点转换、浮点-整型转换、指针转换、成员指针转换、布尔转换、派生类到基类转换 |

等级越低越好：精确匹配 > 提升 > 转换。

### 4.2 转换序列排名比较规则

#### 标准转换序列比较

```cpp
int f(const int*);  // #1
int f(int*);        // #2

int i;
int j = f(&i);  // #2 更好：int* -> int* 是精确匹配
                //       int* -> const int* 需要限定转换
```

#### 引用绑定比较

```cpp
int g(const int&);   // #1
int g(const int&&);  // #2

int i;
int k = g(i);     // #1：左值绑定到 const int&
int m = g(42);    // #2：右值绑定到 const int&& 更好
```

#### 用户定义转换序列比较

当使用相同的用户定义转换函数或构造函数时，比较第二标准转换序列：

```cpp
struct A {
    operator short();  // 用户定义转换函数
} a;

int f(int);    // #1
int f(float);  // #2

int i = f(a);  // #1 更好：A -> short -> int (提升)
               //       A -> short -> float (转换)
```

### 4.3 列表初始化的隐式转换序列

列表初始化 (braced-init-list) 有特殊的转换序列规则：

```cpp
void f1(int);                                // #1
void f1(std::initializer_list<long>);        // #2

void g1() { f1({42}); }  // 选择 #2，initializer_list 更优
```

### 4.4 重写候选机制 (C++20 起)

C++20 引入了重写候选机制，允许通过 `operator<=>` 和 `operator==` 自动生成其他比较运算符：

```cpp
// 只需定义 operator<=> 和 operator==
// 编译器会自动生成 <, <=, >, >=, != 的重写候选

struct Value {
    int v;
    auto operator<=>(const Value&) const = default;
};

bool result = Value{1} < Value{2};   // 使用 operator<=>
bool eq = Value{1} == Value{2};      // 使用 operator==
bool ne = Value{1} != Value{2};      // 自动重写为 !(Value{1} == Value{2})
```

## 5. 使用场景 (Use Cases)

### 5.1 适用场景

1. **函数重载**：为不同类型的参数提供统一的接口
2. **运算符重载**：为自定义类型定义运算符行为
3. **模板特化选择**：在多个模板特化中选择最匹配的版本
4. **构造函数选择**：对象初始化时选择合适的构造函数
5. **转换函数选择**：类型转换时选择合适的转换路径

### 5.2 最佳实践

#### 提供精确匹配的重载

```cpp
// 推荐：为常用类型提供精确匹配
void process(int x);
void process(double x);
void process(const std::string& x);

// 避免：可能导致二义性
void process(long x);
void process(float x);

process(0);  // 二义性：int 可提升为 long 或 float
```

#### 使用引用限定符区分左值和右值

```cpp
class Buffer {
public:
    // 左值对象返回引用
    const char& operator[](size_t index) const& { return data[index]; }
    char& operator[](size_t index) & { return data[index]; }

    // 右值对象返回值
    char operator[](size_t index) && { return data[index]; }
};
```

#### 避免危险的隐式转换

```cpp
class String {
public:
    // 使用 explicit 阻止隐式转换
    explicit String(int size);  // 避免 String s = 10; 这样的隐式转换
    String(const char* str);     // 允许从 C 字符串隐式转换
};
```

### 5.3 常见陷阱

#### 二义性重载

```cpp
void f(long);
void f(float);

f(0);   // 错误：int -> long 和 int -> float 都是提升，二义性
f(0L);  // OK：精确匹配 f(long)
```

#### 用户定义转换导致的二义性

```cpp
class B;

class A {
public:
    A(B&);  // 转换构造函数
};
class B {
public:
    operator A();  // 转换函数
};

void f(A);

B b;
f(b);  // 错误：可通过 A::A(B&) 或 B::operator A() 转换，二义性
```

#### 默认参数与重载的交互

```cpp
void f(int x);
void f(int x, int y = 0);

f(10);  // 错误：二义性，两个函数都匹配
```

#### const 与非 const 重载

```cpp
void f(int& x);       // #1
void f(const int& x); // #2

int i = 10;
f(i);   // #1 更好：左值 int 绑定到 int& 是精确匹配

const int ci = 10;
f(ci);  // #2：const int& 是唯一可行选项

f(10);  // #2：右值只能绑定到 const int&
```

## 6. 代码示例 (Examples)

### 6.1 基础示例：函数重载选择

```cpp
#include <iostream>

void print(int x) {
    std::cout << "int: " << x << std::endl;
}

void print(double x) {
    std::cout << "double: " << x << std::endl;
}

void print(const char* x) {
    std::cout << "const char*: " << x << std::endl;
}

int main() {
    print(42);        // 调用 print(int)：精确匹配
    print(3.14);      // 调用 print(double)：精确匹配
    print("hello");   // 调用 print(const char*)：精确匹配
    print(42L);       // 调用 print(int)：long -> int 是转换
                      //             long -> double 也是转换，但 int 是整型

    return 0;
}
```

### 6.2 成员函数重载决议

```cpp
#include <iostream>

struct Base {
    void f(int) { std::cout << "Base::f(int)\n"; }
};

struct Derived : Base {
    void f(double) { std::cout << "Derived::f(double)\n"; }
    using Base::f;  // 引入基类的 f 到派生类作用域
};

int main() {
    Derived d;
    d.f(42);    // 调用 Derived::f(double)：int -> double
    d.f(3.14);  // 调用 Derived::f(double)：精确匹配

    Base& b = d;
    b.f(42);    // 调用 Base::f(int)：通过 Base 引用调用

    return 0;
}
```

### 6.3 运算符重载决议

```cpp
#include <iostream>

struct Complex {
    double real, imag;

    Complex(double r, double i) : real(r), imag(i) {}

    // 成员运算符
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
};

// 非成员运算符
Complex operator+(const Complex& c, double d) {
    return Complex(c.real + d, c.imag);
}

Complex operator+(double d, const Complex& c) {
    return Complex(c.real + d, c.imag);
}

int main() {
    Complex c1(1, 2);
    Complex c2(3, 4);

    auto r1 = c1 + c2;   // 成员 operator+
    auto r2 = c1 + 5.0;  // 非成员 operator+(Complex, double)
    auto r3 = 5.0 + c1;  // 非成员 operator+(double, Complex)

    return 0;
}
```

### 6.4 模板与非模板重载

```cpp
#include <iostream>
#include <vector>

// 通用模板
template<typename T>
void process(T x) {
    std::cout << "Template: " << x << std::endl;
}

// 非模板重载（优先选择）
void process(int x) {
    std::cout << "Non-template int: " << x << std::endl;
}

// 更特化的模板
template<typename T>
void process(std::vector<T> v) {
    std::cout << "Vector template, size: " << v.size() << std::endl;
}

int main() {
    process(42);           // 调用非模板版本：非模板优于模板
    process(3.14);         // 调用模板版本：唯一匹配
    process(std::vector<int>{1, 2, 3});  // 调用 vector 特化版本

    return 0;
}
```

### 6.5 列表初始化重载决议

```cpp
#include <iostream>
#include <initializer_list>
#include <vector>

class Container {
public:
    Container(int x) {
        std::cout << "Container(int): " << x << std::endl;
    }

    Container(std::initializer_list<int> list) {
        std::cout << "Container(initializer_list): ";
        for (int x : list) std::cout << x << " ";
        std::cout << std::endl;
    }
};

int main() {
    Container c1(42);           // 调用 Container(int)
    Container c2{42};           // 调用 Container(initializer_list)
    Container c3{1, 2, 3};       // 调用 Container(initializer_list)
    Container c4 = {42};        // 调用 Container(initializer_list)

    return 0;
}
```

### 6.6 C++20 重写候选示例

```cpp
#include <compare>
#include <iostream>

struct Point {
    int x, y;

    // 定义三向比较运算符
    auto operator<=>(const Point&) const = default;
    bool operator==(const Point&) const = default;
};

int main() {
    Point p1{1, 2};
    Point p2{3, 4};

    // 以下都通过重写候选机制工作
    bool b1 = p1 < p2;   // 使用 operator<=>
    bool b2 = p1 <= p2;  // 使用 operator<=>
    bool b3 = p1 > p2;   // 使用 operator<=>（参数反转）
    bool b4 = p1 >= p2;  // 使用 operator<=>（参数反转）
    bool b5 = p1 != p2;  // 使用 operator==（逻辑非）

    std::cout << std::boolalpha;
    std::cout << "p1 < p2: " << b1 << std::endl;

    return 0;
}
```

### 6.7 常见错误示例

#### 错误：二义性重载

```cpp
void f(long x);
void f(float x);

f(0);   // 错误：二义性
        // 修正：显式指定类型 f(0L) 或 f(0.0f)
```

#### 错误：转换序列导致二义性

```cpp
struct A {
    A(int);          // 从 int 转换
    A(long, int);   // 从 long 和 int 构造
};

A a = {42};  // 可能二义性，取决于具体实现
             // 修正：显式调用构造函数 A a(42);
```

#### 错误：SFINAE 导致无匹配

```cpp
#include <type_traits>

template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
f(T x) { return x * 2; }

f(3.14);  // 错误：double 不是整型，没有匹配的函数
          // 修正：为浮点类型添加重载或移除约束
```

## 7. 总结 (Summary)

### 核心要点

1. **三步流程**：重载决议依次执行候选函数收集、可行函数筛选、最佳函数确定三个步骤。

2. **转换序列等级**：标准转换序列分为精确匹配、提升、转换三个等级，等级越低越优先。

3. **模板与非模板**：非模板函数优先于模板特化，更特化的模板优先于更通用的模板。

4. **C++20 新特性**：重写候选机制简化了比较运算符的定义，只需定义 `operator<=>` 和 `operator==` 即可自动支持所有比较运算符。

5. **列表初始化**：优先匹配 `initializer_list` 构造函数，这是 C++11 以后的重要规则。

### 技术对比

| 特性 | C 风格解决方案 | C++ 重载决议 |
|-----|---------------|-------------|
| 多类型处理 | 不同函数名 (`print_int`, `print_float`) | 统一函数名 (`print`) |
| 类型安全 | 运行时检查或无检查 | 编译时决议 |
| 代码可读性 | 函数名冗长 | 接口简洁统一 |
| 扩展性 | 需要新函数名 | 直接添加新重载 |

### 学习建议

1. **理解转换等级**：掌握精确匹配、提升、转换的区别，这是理解重载决议的基础。

2. **避免二义性**：设计重载函数时注意避免二义性，特别是涉及用户定义转换时。

3. **善用 `explicit`**：防止单参数构造函数参与重载决议导致的意外转换。

4. **理解 C++20 新规则**：重写候选机制是现代 C++ 的重要特性，值得深入学习。

5. **阅读编译错误**：重载决议失败时，编译器会列出所有候选和可行函数，仔细阅读有助于定位问题。

### 参考资源

- C++23 标准 (ISO/IEC 14882:2024): 12.2 Overload resolution [over.match]
- C++20 标准 (ISO/IEC 14882:2020): 12.4 Overload resolution [over.match]
- cppreference: https://en.cppreference.com/w/cpp/language/overload_resolution