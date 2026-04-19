# 引用声明 (Reference Declaration)

## 1. 概述 (Overview)

引用（Reference）是 C++ 中的一种复合类型，用于声明一个命名变量作为已存在对象或函数的别名。引用本身不是对象，它只是现有对象或函数的另一个名称。

C++ 提供两种引用类型：

| 类型 | 语法 | 说明 |
|------|------|------|
| **左值引用 (Lvalue Reference)** | `&` | 绑定到左值，用于对象别名和传参 |
| **右值引用 (Rvalue Reference)** | `&&` | C++11 起，绑定到右值，支持移动语义 |

引用的核心特性：
- 必须在声明时初始化，绑定到有效的对象或函数
- 一旦初始化，就不能重新绑定到其他对象
- 不能形成对 void 的引用
- 引用类型在顶层不能有 cv 限定符（const/volatile）

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

引用是 Bjarne Stroustrup 在设计 C++ 时引入的特性，最初的主要目的是支持运算符重载。在 C 语言中，函数参数传递只有值传递，如果要修改实参，必须显式使用指针。引用提供了更优雅的解决方案，使代码更加直观和类型安全。

### 版本演变

| 版本 | 特性变更 |
|------|----------|
| **C++98** | 引入左值引用，支持引用折叠的基本规则 |
| **C++11** | 引入右值引用 `&&`，支持移动语义和完美转发；引入转发引用（Forwarding Reference）概念；属性支持 |
| **C++17** | 完善转发引用在类模板参数推导中的规则 |

### 功能测试宏

| 宏名称 | 值 | 标准 | 特性 |
|--------|-----|------|------|
| `__cpp_rvalue_references` | `200610L` | C++11 | 右值引用 |

### 缺陷报告

| 编号 | 适用版本 | 原有问题 | 修正方案 |
|------|----------|----------|----------|
| CWG 453 | C++98 | 引用不能绑定的对象或函数不明确 | 明确说明 |
| CWG 1510 | C++11 | decltype 的操作数中无法形成 cv 限定的引用 | 允许 |
| CWG 2550 | C++98 | 参数可以有"对 void 的引用"类型 | 禁止 |
| CWG 2933 | C++98 | 访问悬空引用的行为不明确 | 明确说明 |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

引用变量声明是声明符具有以下形式的简单声明：

| 形式 | 说明 | 版本 |
|------|------|------|
| `&` attr(可选) declarator | 左值引用声明符 | C++98 |
| `&&` attr(可选) declarator | 右值引用声明符 | C++11 起 |

**声明含义**：
- `S& D;` 声明 `D` 为对类型 `S` 的左值引用
- `S&& D;` 声明 `D` 为对类型 `S` 的右值引用

### 参数说明

| 参数 | 说明 |
|------|------|
| declarator | 任何声明符，但不能是另一个引用声明符（不存在引用的引用） |
| attr | 属性列表（C++11 起） |

### 引用的限制

由于引用不是对象，以下类型不能形成：

```cpp
int& a[3]; // 错误：引用的数组
int&* p;   // 错误：指向引用的指针
int& &r;   // 错误：引用的引用
void& r;   // 错误：对 void 的引用
```

**顶层 cv 限定符限制**：引用类型不能在顶层有 cv 限定符。如果通过 typedef 名称或 `decltype` 说明符添加限定符，它会被忽略。

### 引用折叠规则 (Reference Collapsing) (C++11 起)

通过模板或 typedef 的类型操作，可以形成"引用的引用"，此时应用引用折叠规则：

| 组合 | 结果 | 说明 |
|------|------|------|
| `& + &` | `&` | 左值引用 + 左值引用 = 左值引用 |
| `& + &&` | `&` | 左值引用 + 右值引用 = 左值引用 |
| `&& + &` | `&` | 右值引用 + 左值引用 = 左值引用 |
| `&& + &&` | `&&` | 右值引用 + 右值引用 = 右值引用 |

**核心规则**：只有右值引用到右值引用折叠为右值引用，其他所有组合都形成左值引用。

```cpp
typedef int&  lref;
typedef int&& rref;
int n;

lref&  r1 = n; // r1 的类型是 int&
lref&& r2 = n; // r2 的类型是 int&
rref&  r3 = n; // r3 的类型是 int&
rref&& r4 = 1; // r4 的类型是 int&&
```

这一规则与 `T&&` 在函数模板中的特殊模板参数推导规则结合，使 `std::forward` 成为可能。

## 4. 底层原理 (Underlying Principles)

### 存储模型

引用不是对象，它们不一定占用存储空间。但在某些情况下，编译器可能会分配存储空间以实现所需的语义：

- 类的非静态数据成员如果是引用类型，通常会增大类的大小（存储内存地址所需的空间）
- 在需要实现引用语义的场景下，编译器可能将引用实现为指针

### 引用的本质

从实现角度来看，引用通常被编译器实现为常量指针：

```cpp
int x = 10;
int& ref = x;  // 编译器可能在底层实现为: int* const ref_ptr = &x;
ref = 20;      // 编译器可能在底层实现为: *ref_ptr = 20;
```

编译器自动处理解引用操作，使引用使用起来更加直观和安全。

### 绑定机制

引用绑定的关键规则：

| 引用类型 | 可绑定对象 | 说明 |
|----------|-----------|------|
| `T&` | 非常量左值 | 只能绑定到相同类型的非常量左值 |
| `const T&` | 任何值 | 可绑定到左值、右值，并延长临时对象生命周期 |
| `T&&` | 右值 | 可绑定到纯右值和亡值，延长临时对象生命周期 |

### 生命周期延长

当引用绑定到临时对象时，临时对象的生命周期会被延长以匹配引用的生命周期：

```cpp
const std::string& r2 = s1 + s1; // const 左值引用延长临时对象生命周期
std::string&& r3 = s1 + s1;       // 右值引用延长临时对象生命周期，且可修改
```

### 悬空引用 (Dangling References)

虽然引用在初始化时总是指向有效的对象或函数，但可能创建程序使得被引用对象的生命周期结束但引用仍可访问（悬空引用）：

```cpp
std::string& f()
{
    std::string s = "Example";
    return s; // 危险：返回局部变量的引用
}

std::string& r = f(); // r 是悬空引用
std::cout << r;        // 未定义行为：读取悬空引用
```

**悬空引用的访问行为**：给定引用类型的表达式 expr，设 target 为引用所表示的对象或函数：
- 如果指向 target 的指针在 expr 的求值上下文中有效，则结果表示 target
- 否则，行为未定义

## 5. 使用场景 (Use Cases)

### 左值引用的用途

#### 用途一：对象别名

```cpp
std::string s = "Ex";
std::string& r1 = s;       // r1 是 s 的别名，可修改
const std::string& r2 = s; // 常量引用，不能通过 r2 修改 s
```

#### 用途二：函数参数传递（引用传递语义）

```cpp
void double_string(std::string& s)
{
    s += s;  // 直接修改原对象，无需拷贝
}

int main()
{
    std::string str = "Test";
    double_string(str);
    std::cout << str << '\n';  // 输出 "TestTest"
}
```

#### 用途三：返回引用以支持链式调用

```cpp
char& char_number(std::string& s, std::size_t n)
{
    return s.at(n);  // 返回引用，可以被赋值
}

int main()
{
    std::string str = "Test";
    char_number(str, 1) = 'a';  // 函数调用是左值，可被赋值
    std::cout << str << '\n';   // 输出 "Tast"
}
```

### 右值引用的用途

#### 用途一：延长临时对象生命周期

```cpp
std::string s1 = "Test";
// std::string&& r1 = s1;        // 错误：右值引用不能绑定到左值

const std::string& r2 = s1 + s1; // OK：const 左值引用延长生命周期
// r2 += "Test";                 // 错误：不能通过 const 引用修改

std::string&& r3 = s1 + s1;       // OK：右值引用延长生命周期
r3 += "Test";                     // OK：可以通过非 const 引用修改
```

#### 用途二：函数重载区分左右值

```cpp
void f(int& x);        // 绑定到非 const 左值
void f(const int& x);  // 绑定到 const 左值或右值
void f(int&& x);       // 绑定到右值（优先匹配）

int main()
{
    int i = 1;
    const int ci = 2;

    f(i);  // 调用 f(int&)
    f(ci); // 调用 f(const int&)
    f(3);  // 调用 f(int&&)
    f(std::move(i)); // 调用 f(int&&)

    // 重要：右值引用变量在表达式中是左值
    int&& x = 1;
    f(x);            // 调用 f(int& x) - x 是左值
    f(std::move(x)); // 调用 f(int&& x)
}
```

这使得移动构造函数、移动赋值运算符和其他移动感知函数（如 `std::vector::push_back()`）能够自动选择合适的重载。

#### 用途三：实现移动语义

```cpp
std::vector<int> v{1, 2, 3, 4, 5};
std::vector<int> v2(std::move(v)); // 移动构造，v 变为空
assert(v.empty());
```

### 转发引用 (Forwarding References)

转发引用是一种特殊的引用，用于在函数模板中保持参数的值类别，使 `std::forward` 完美转发成为可能。

#### 形式一：函数模板参数

```cpp
template<class T>
int f(T&& x)  // x 是转发引用
{
    return g(std::forward<T>(x));  // 完美转发
}

int main()
{
    int i;
    f(i); // 参数是左值，调用 f<int&>(int&)，std::forward<int&>(x) 是左值
    f(0); // 参数是右值，调用 f<int>(int&&)，std::forward<int>(x) 是右值
}
```

**注意**：以下情况 `T&&` 不是转发引用：

```cpp
template<class T>
int g(const T&& x);  // 不是转发引用：const T 不是 cv 无限定的

template<class T>
struct A {
    template<class U>
    A(T&& x, U&& y, int* p);  // x 不是转发引用（T 不是该构造函数的类型模板参数）
                              // 但 y 是转发引用
};
```

#### 形式二：auto&&

```cpp
auto&& vec = foo();  // foo() 可能是左值或右值，vec 是转发引用
auto i = std::begin(vec);  // 无论哪种情况都有效
(*i)++;

g(std::forward<decltype(vec)>(vec));  // 完美转发

// 范围 for 循环中的常见用法
for (auto&& x : f()) {
    // x 是转发引用，泛型代码中的常见模式
}

auto&& z = {1, 2, 3};  // 不是转发引用（初始化列表的特殊情况）
```

### 最佳实践

| 场景 | 推荐做法 | 原因 |
|------|---------|------|
| 函数参数（只读） | `const T&` | 避免拷贝，支持任何值类型 |
| 函数参数（需修改） | `T&` | 直接修改原对象 |
| 函数参数（转移所有权） | `T&&` | 支持移动语义 |
| 泛型代码转发 | `T&&` + `std::forward` | 保持值类别 |

### 常见陷阱

#### 陷阱一：悬空引用

```cpp
// 错误：返回局部变量的引用
std::string& f()
{
    std::string s = "Example";
    return s;  // 局部变量销毁后引用失效
}

std::string& r = f();  // r 是悬空引用
std::cout << r;        // 未定义行为
```

#### 陷阱二：类型不可访问的引用绑定

```cpp
char x alignas(int);

int& ir = *reinterpret_cast<int*>(&x); // 未定义行为：
                                        // 初始化器引用 char 对象
```

#### 陷阱三：调用不兼容的函数引用

```cpp
void f(int);

using F = void(float);
F& ir = *reinterpret_cast<F*>(&f); // 未定义行为：
                                    // 初始化器引用 void(int) 函数
```

#### 陷阱四：右值引用变量是左值

```cpp
int&& x = 1;   // x 是右值引用类型
f(x);          // 但在表达式中，x 是左值！调用 f(int&)

// 必须使用 std::move 转换为右值
f(std::move(x)); // 调用 f(int&&)
```

## 6. 代码示例 (Examples)

### 示例一：左值引用基础用法

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string s = "Ex";
    std::string& r1 = s;
    const std::string& r2 = s;

    r1 += "ample";           // 修改 s
//  r2 += "!";               // 错误：不能通过 const 引用修改
    std::cout << r2 << '\n'; // 输出 "Example"
}
```

### 示例二：通过引用传递参数

```cpp
#include <iostream>
#include <string>

void double_string(std::string& s)
{
    s += s;  // 's' 与 main() 中的 'str' 是同一对象
}

int main()
{
    std::string str = "Test";
    double_string(str);
    std::cout << str << '\n';  // 输出 "TestTest"
}
```

### 示例三：返回引用实现可修改的函数调用

```cpp
#include <iostream>
#include <string>

char& char_number(std::string& s, std::size_t n)
{
    return s.at(n);  // string::at() 返回字符引用
}

int main()
{
    std::string str = "Test";
    char_number(str, 1) = 'a';  // 函数调用是左值，可被赋值
    std::cout << str << '\n';   // 输出 "Tast"
}
```

### 示例四：右值引用与函数重载

```cpp
#include <iostream>
#include <utility>

void f(int& x)
{
    std::cout << "lvalue reference overload f(" << x << ")\n";
}

void f(const int& x)
{
    std::cout << "lvalue reference to const overload f(" << x << ")\n";
}

void f(int&& x)
{
    std::cout << "rvalue reference overload f(" << x << ")\n";
}

int main()
{
    int i = 1;
    const int ci = 2;

    f(i);  // 调用 f(int&)
    f(ci); // 调用 f(const int&)
    f(3);  // 调用 f(int&&)
    f(std::move(i)); // 调用 f(int&&)

    // 右值引用变量在表达式中是左值
    int&& x = 1;
    f(x);            // 调用 f(int& x)
    f(std::move(x)); // 调用 f(int&& x)
}
```

### 示例五：转发引用与完美转发

```cpp
#include <utility>
#include <iostream>

void g(int& x) { std::cout << "lvalue: " << x << '\n'; }
void g(int&& x) { std::cout << "rvalue: " << x << '\n'; }

template<class T>
int f(T&& x)  // x 是转发引用
{
    return g(std::forward<T>(x));  // 完美转发
}

int main()
{
    int i = 42;
    f(i); // 参数是左值，调用 f<int&>(int&)，std::forward<int&>(x) 是左值
    f(0); // 参数是右值，调用 f<int>(int&&)，std::forward<int>(x) 是右值
}
```

### 示例六：auto&& 转发引用

```cpp
#include <vector>
#include <utility>

// auto&& 作为转发引用的常见用法
void process(auto&& vec)
{
    auto i = std::begin(vec);
    (*i)++;
    // 可以根据 vec 是左值还是右值选择不同处理
}

int main()
{
    std::vector<int> v{1, 2, 3};
    process(v);                   // 传递左值
    process(std::vector<int>{4}); // 传递右值

    // 范围 for 循环中的常见模式
    for (auto&& x : v) {
        // x 是转发引用
    }
}
```

### 示例七：常见错误 - 返回局部变量引用

```cpp
#include <string>
#include <iostream>

// 错误示例：返回局部变量的引用
std::string& f()
{
    std::string s = "Example";
    return s;  // 错误：返回局部变量的引用
}

// 正确做法：返回值或传递引用参数
std::string f_correct()
{
    return "Example";  // 返回值，使用返回值优化
}

int main()
{
    // std::string& r = f();  // 悬空引用，未定义行为
    std::string s = f_correct();  // 正确
}
```

### 示例八：延长临时对象生命周期

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string s1 = "Test";

    // const 左值引用延长临时对象生命周期
    const std::string& r2 = s1 + s1;
    std::cout << r2 << '\n';  // 输出 "TestTest"
    // r2 += "!";  // 错误：不能通过 const 引用修改

    // 右值引用延长临时对象生命周期，且可修改
    std::string&& r3 = s1 + s1;
    r3 += "!";
    std::cout << r3 << '\n';  // 输出 "TestTest!"
}
```

## 7. 总结 (Summary)

### 核心要点对比

| 特性 | 左值引用 (`&`) | 右值引用 (`&&`) | 转发引用 |
|------|---------------|-----------------|----------|
| 引入版本 | C++98 | C++11 | C++11 |
| 声明形式 | `T&` | `T&&` | `T&&`（模板）或 `auto&&` |
| 绑定对象 | 左值 | 右值 | 左值或右值 |
| 主要用途 | 别名、传参、返回值 | 移动语义、完美转发 | 完美转发 |

### 引用与指针对比

| 对比项 | 引用 | 指针 |
|--------|------|------|
| 初始化 | 必须初始化 | 可以为空（nullptr） |
| 重新绑定 | 不可以 | 可以 |
| 占用空间 | 不一定（可能不占存储） | 固定大小 |
| 语法使用 | 直接使用，无需解引用 | 需要解引用 `*ptr` |
| 数组支持 | 不支持引用数组 | 支持指针数组 |
| 空值 | 不存在空引用 | 存在空指针 |
| 运算 | 不支持算术运算 | 支持指针运算 |

### 学习建议

1. **理解引用的本质**：引用是别名而非对象，理解这一点对掌握移动语义至关重要。

2. **掌握值类别**：理解左值（lvalue）、纯右值（prvalue）、亡值（xvalue）的概念，是正确使用引用的前提。

3. **区分转发引用与右值引用**：
   - `T&&` 在类型推导上下文（模板参数、auto）中是转发引用
   - `int&&` 或 `std::string&&` 是右值引用
   - `const T&&` 不是转发引用

4. **避免悬空引用**：永远不要返回局部变量或临时对象的引用。

5. **善用 const 引用**：const 左值引用可以绑定到任何值类型，是函数参数的首选方式之一。

6. **理解右值引用变量是左值**：`int&& x = 1;` 中 x 的类型是右值引用，但在表达式中 x 是左值。

### 关键概念速查

| 概念 | 英文 | 说明 |
|------|------|------|
| 左值引用 | Lvalue Reference | 使用 `&` 声明，绑定到左值 |
| 右值引用 | Rvalue Reference | 使用 `&&` 声明，绑定到右值，支持移动语义 |
| 转发引用 | Forwarding Reference | 在模板参数推导上下文中的 `T&&`，保持值类别 |
| 引用折叠 | Reference Collapsing | 多重引用组合的简化规则 |
| 悬空引用 | Dangling Reference | 引用的对象已销毁，访问会导致未定义行为 |
| 生命周期延长 | Lifetime Extension | const 引用或右值引用绑定临时对象时延长其生命周期 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/reference
- C++ Standard: [dcl.ref]
- Thomas Becker, 2013 - C++ Rvalue References Explained