# 作用域（Scope）

## 1. 概述（Overview）

**作用域（Scope）** 是 C++ 程序中声明可见的区域。每个出现在 C++ 程序中的声明只在某些可能不连续的作用域中可见。

在一个作用域内，可以使用**非限定名称查找（unqualified name lookup）** 将名称与其声明关联。

### 作用域类型

C++ 提供以下作用域类型：

| 作用域类型 | 说明 |
|------------|------|
| 块作用域（block scope） | 选择语句、迭代语句、处理器、复合语句引入 |
| 函数参数作用域（function parameter scope） | 函数参数声明引入 |
| Lambda 作用域（lambda scope） | Lambda 表达式引入（C++14 起） |
| 命名空间作用域（namespace scope） | 命名空间定义引入 |
| 类作用域（class scope） | 类或类模板声明引入 |
| 枚举作用域（enumeration scope） | 枚举声明引入 |
| 模板参数作用域（template parameter scope） | 模板声明引入 |

### 基本概念

- **全局作用域（global scope）**：包含整个程序的作用域
- **封闭作用域（enclosing scope）**：包含某个程序点的任何作用域
- **直接作用域（immediate scope）**：某个程序点的最小封闭作用域
- **父作用域（parent scope）**：包含作用域 S 的最小非模板参数作用域

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C++98 | 定义基础作用域规则（块、函数参数、命名空间、类） |
| C++11 | 引入 Lambda 作用域、范围 for 语句作用域 |
| C++14 | Lambda 捕获初始化器的作用域规则 |
| C++17 | 结构化绑定声明的作用域、推导指引的函数参数作用域 |
| C++20 | 概念定义的作用域、requires 表达式的函数参数作用域 |
| C++26 | 名称独立声明的作用域冲突规则 |

### 与 C 语言的区别

| 特性 | C++ | C |
|------|-----|---|
| 结构体作用域 | 有 | 无 |
| 命名空间作用域 | 有 | 无（类似文件作用域） |
| Lambda 作用域 | 有 | 无 |
| 模板参数作用域 | 有 | 无 |
| 循环体隐藏循环变量 | 不允许 | 允许 |

## 3. 语法与参数（Syntax and Parameters）

### 3.1 块作用域

以下结构引入块作用域：
- 选择语句（`if`、`switch`）
- 迭代语句（`for`、范围 `for`（C++11 起）、`while`、`do-while`）
- 处理器（handler）
- 复合语句（非处理器的复合语句）

```cpp
int i = 42;
int a[10];

for (int i = 0; i < 10; i++)  // 内层 "i" 属于 for 语句引入的块作用域
    a[i] = i;

int j = i;  // j = 42
```

**块作用域变量**：属于块作用域的变量称为块变量。

**块作用域 extern 声明**：目标为更大的封闭作用域，但在其直接作用域中绑定名称。

**冲突规则**：如果非名称独立声明（C++26 起）在块作用域 S 中绑定名称，且与父作用域中的声明潜在冲突，则程序非良构：

```cpp
if (int x = f())  // 声明 "x"
{ // if 块是 if 语句的子语句
    int x;        // 错误："x" 重声明
}
else
{ // else 块也是 if 语句的子语句
    int x;        // 错误："x" 重声明
}

void g(int i)
{
    extern int i;  // 错误："i" 重声明
}
```

### 3.2 函数参数作用域

每个参数声明 P 引入一个函数参数作用域，包含 P。

**作用域范围**：
- **函数定义**：扩展到函数定义结束
- **函数原型**：扩展到函数声明符结束
- **Lambda 表达式**（C++11 起）：扩展到 `{ body }` 结束
- **推导指引**（C++17 起）：扩展到推导指引结束
- **requires 表达式**（C++20 起）：扩展到 `{ requirement-seq }` 结束

```cpp
int f(int n)  // 参数 "n" 的声明引入函数参数作用域
{             //
    /* ... */
}             // 函数参数作用域在此结束
```

### 3.3 Lambda 作用域（C++14 起）

每个 Lambda 表达式引入一个 Lambda 作用域，从 `[captures]` 之后立即开始，延伸到 `{ body }` 结束。

带有初始化器的捕获属于 Lambda 表达式引入的 Lambda 作用域。

```cpp
auto lambda = [x = 1, y]()  // Lambda 表达式引入 Lambda 作用域
{                           // 捕获 "x" 的目标作用域
    /* ... */
};                          // Lambda 作用域在分号前结束
```

### 3.4 命名空间作用域

每个命名空间 N 的命名空间定义引入一个命名空间作用域 S，包含 N 的每个命名空间定义的声明。

对于目标作用域为 S 或被 S 包含的非友元重声明或特化，以下部分也属于作用域 S：
- 类（模板）重声明或类模板特化：类头名之后的部分
- 枚举重声明：枚举头名之后的部分
- 其他重声明或特化：声明符的非限定标识符或限定标识符之后的部分

```cpp
namespace V   // "V" 的命名空间定义
{             // 引入命名空间作用域 "S"
    void f();
}

void V::f()   // "f" 之后的部分也属于作用域 "S"
{
    void h(); // 声明 V::h
}
```

**全局作用域**：全局命名空间的命名空间作用域。

### 3.5 类作用域

每个类或类模板 C 的声明引入一个类作用域 S，包含 C 的类定义的成员规范。

对于目标作用域为 S 或被 S 包含的非友元重声明或特化，以下部分也属于作用域 S：
- 类（模板）重声明或类模板特化：类头名之后的部分
- 枚举重声明：枚举头名之后的部分
- 其他重声明或特化：声明符的非限定标识符或限定标识符之后的部分

```cpp
class C       // "C" 的类定义
{             // 引入类作用域 "S"
    void f();
};

void C::f()   // "f" 之后的部分也属于作用域 "S"
{
    /* ... */
}
```

### 3.6 枚举作用域

每个枚举 E 的声明引入一个枚举作用域，包含 E 的非不透明枚举声明（C++11 起）的枚举器列表（如果存在）。

```cpp
enum class E  // "E" 的枚举声明
{             // 引入枚举作用域 "S"
    e1, e2, e3
}
```

### 3.7 模板参数作用域

每个模板模板参数引入一个模板参数作用域，包含整个模板参数列表和该模板模板参数的 require 子句（C++20 起）。

每个模板声明 D 引入一个模板参数作用域 S，从 D 的模板参数列表开始延伸到 D 结束。模板参数列表之外的任何声明如果本应属于 S，则属于与 D 相同的作用域。

```cpp
// "X" 的类模板声明
// 引入模板参数作用域 "S1"
template
<
    // 作用域 "S1" 从此开始
    template // 模板模板参数 "T"
             // 引入另一个模板参数作用域 "S2"
    <
        typename T1,
        typename T2
    > requires std::convertible_from<T1, T2>  // 作用域 "S2" 在此结束
    class T,
    typename U
>
class X;  // 作用域 "S1" 在分号前结束
```

## 4. 底层原理（Underlying Principles）

### 4.1 声明点（Point of Declaration）

名称通常在其首次声明的位置点之后可见。

**简单声明**：声明点在声明符之后、初始化器之前。

```cpp
int x = 32;  // 外层 x 在作用域内

{
    int x = x;  // 内层 x 在初始化器 (= x) 之前进入作用域
                // 不会用外层 x 的值 (32) 初始化内层 x
                // 而是用其自身的不确定值初始化
}

std::function<int(int)> f = [&](int n){ return n > 1 ? n * f(n - 1) : n; };
// 函数名 f 在 Lambda 中在作用域内，可以被引用捕获，实现递归函数
```

### 4.2 数组声明符中的声明点

```cpp
const int x = 2;  // 外层 x 在作用域内

{
    int x[x] = {};  // 内层 x 在初始化器 (= {}) 之前但在声明符 (x[x]) 之后进入作用域
                    // 在声明符中，外层 x 仍在作用域内
                    // 这声明了一个 2 个 int 的数组
}
```

### 4.3 类声明点

类或类模板声明的声明点在类头中命名类的标识符之后。类名在基类列表中已在作用域内。

```cpp
struct S: std::enable_shared_from_this<S> {};  // S 在冒号处已在作用域内
```

### 4.4 枚举声明点

枚举说明符或不透明枚举声明（C++11 起）的声明点在命名枚举的标识符之后。

```cpp
enum E : int  // E 在冒号处已在作用域内
{
    A = sizeof(E)
};
```

### 4.5 类型别名声明点

类型别名或别名模板声明的声明点在别名引用的类型标识符之后。

```cpp
using T = int;  // 外层 T 在分号处进入作用域

{
    using T = T*;  // 内层 T 在分号处进入作用域
                   // 外层 T 在分号前仍在作用域内
                   // 等同于 T = int*
}
```

### 4.6 枚举器声明点

枚举器的声明点在其定义之后（与变量不同，不在初始化器之前）。

```cpp
const int x = 12;

{
    enum
    {
        x = x + 1,  // 枚举器 x 在逗号处进入作用域
                    // 外层 x 在逗号前在作用域内
                    // 枚举器 x 初始化为 13
        y = x + 1   // y 初始化为 14
    };
}
```

### 4.7 注入类名声明点

注入类名的声明点紧跟其类（或类模板）定义的左花括号之后。

```cpp
template<typename T>
struct Array
//  : std::enable_shared_from_this<Array>  // 错误：注入类名不在作用域内
    : std::enable_shared_from_this< Array<T> >  // 正确：模板名 Array 在作用域内
{ // 注入类名 Array 现在在作用域内，如同公有成员名
    Array* p;  // 指向 Array<T> 的指针
};
```

### 4.8 范围 for 循环声明点

范围 for 循环的范围声明中声明的变量或结构化绑定的声明点在范围表达式之后。

```cpp
std::vector<int> x;

for (auto x : x)  // vector x 在右括号前在作用域内
                  // auto x 在右括号处进入作用域
{
    // auto x 在作用域内
}
```

### 4.9 模板参数声明点

模板参数的声明点在其完整模板参数之后（包括可选的默认参数）。

```cpp
typedef unsigned char T;

template<
    class T = T,  // 模板参数 T 在逗号处进入作用域
                  // typedef 名 unsigned char 在逗号前在作用域内
    T             // 模板参数 T 在作用域内
    N = 0
>
struct A
{
};
```

## 5. 使用场景（Use Cases）

### 5.1 块作用域变量

```cpp
void process(int value) {
    int local = value * 2;  // 块作用域变量

    if (local > 10) {
        int temp = local;  // 内层块作用域
        // ...
    }
    // temp 不在作用域内
}
```

### 5.2 命名空间作用域

```cpp
namespace mylib {
    int value = 100;  // 命名空间作用域

    namespace detail {
        int helper = 200;  // 嵌套命名空间作用域
    }
}
```

### 5.3 类作用域

```cpp
class Widget {
    int value_;  // 类作用域

public:
    int getValue() const { return value_; }
    void setValue(int v) { value_ = v; }
};

// 类外定义
int Widget::getValue() const {
    return value_;  // value_ 在类作用域内
}
```

### 5.4 Lambda 捕获作用域（C++14 起）

```cpp
void demo() {
    int x = 10;

    auto f = [y = x + 1]() {  // y 属于 Lambda 作用域
        return y * 2;
    };

    std::cout << f() << std::endl;  // 22
}
```

### 5.5 常见陷阱

1. **变量自初始化**：

```cpp
int x = 32;
{
    int x = x;  // 危险：用不确定值初始化
}
```

2. **循环变量冲突**：

```cpp
// C++ 不允许循环体隐藏循环变量
for (int i = 0; i < 10; i++) {
    int i = 100;  // 错误：重声明
}

// C 允许这种写法
```

3. **if/else 子语句作用域**：

```cpp
if (int x = f()) {
    int x;  // 错误：重声明
} else {
    int x;  // 错误：重声明
}
```

4. **extern 声明冲突**：

```cpp
void g(int i) {
    extern int i;  // 错误：重声明
}
```

## 6. 代码示例（Examples）

### 基础用法

```cpp
#include <iostream>

int global = 100;  // 命名空间作用域

namespace mylib {
    int value = 200;  // 命名空间作用域

    void demo() {
        int local = 300;  // 块作用域
        std::cout << "global: " << global << std::endl;
        std::cout << "value: " << value << std::endl;
        std::cout << "local: " << local << std::endl;
    }
}

int main()
{
    mylib::demo();
    return 0;
}
```

### 类作用域与成员访问

```cpp
#include <iostream>

class Base {
protected:
    int value_ = 10;
};

class Derived : public Base {
public:
    int getValue() const {
        return value_;  // 访问基类成员
    }

    void setValue(int v) {
        value_ = v;
    }
};

// 类外定义成员函数
void Derived::setValue(int v) {
    value_ = v;  // value_ 在类作用域内
}

int main()
{
    Derived d;
    d.setValue(42);
    std::cout << d.getValue() << std::endl;
    return 0;
}
```

### Lambda 与作用域（C++14）

```cpp
#include <iostream>
#include <functional>

int main()
{
    int x = 10;
    int y = 20;

    // Lambda 作用域示例
    auto f = [z = x + y]() {  // z 属于 Lambda 作用域
        return z * 2;
    };

    std::cout << "f() = " << f() << std::endl;  // 60

    // 递归 Lambda
    std::function<int(int)> factorial = [&](int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    };
    // factorial 在 Lambda 内在作用域内

    std::cout << "5! = " << factorial(5) << std::endl;  // 120

    return 0;
}
```

### 模板参数作用域

```cpp
#include <iostream>
#include <concepts>

// 使用已声明的模板参数
template<typename T, T DefaultValue = T{}>
struct Container {
    T value = DefaultValue;
};

// 嵌套模板参数作用域
template<
    template<typename> class Container,
    typename T
>
class Wrapper {
    Container<T> data_;
};

int main()
{
    Container<int, 42> c;
    std::cout << c.value << std::endl;  // 42

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>

/* 错误：变量自初始化 */
void error_self_init()
{
    int x = 32;
    {
        int x = x;  // 危险：用不确定值初始化
        std::cout << x << std::endl;  // 未定义行为
    }
}

/* 正确：使用不同的名称 */
void correct_init()
{
    int x = 32;
    {
        int y = x;  // 正确
        std::cout << y << std::endl;  // 32
    }
}

/* 错误：if/else 子语句中重声明 */
void error_if_scope()
{
    if (int x = 5) {
        // int x = 10;  // 错误：重声明
    } else {
        // int x = 20;  // 错误：重声明
    }
}

/* 正确：使用不同作用域 */
void correct_if_scope()
{
    if (int x = 5) {
        {
            int y = x;  // 正确：不同变量
        }
    }
}

/* 错误：循环体隐藏循环变量（C++ 不允许） */
void error_loop_hide()
{
    // for (int i = 0; i < 10; i++) {
    //     int i = 100;  // 错误：重声明
    // }
}

/* 正确：理解 for 循环的作用域 */
void correct_loop_scope()
{
    for (int i = 0; i < 3; i++) {
        std::cout << i << " ";
    }
    // std::cout << i;  // 错误：i 不在作用域内
    std::cout << std::endl;
}

/* 正确：范围 for 循环的声明点 */
void correct_range_for()
{
    std::vector<int> data = {1, 2, 3};

    // data 在右括号前在作用域内，auto x 在右括号处进入作用域
    for (auto x : data) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

/* 枚举器的声明点 */
void enum_declaration_point()
{
    const int x = 12;

    enum {
        x = x + 1,  // 枚举器 x = 13
        y = x + 1   // y = 14
    };

    std::cout << "enum x = " << x << std::endl;  // 13
    std::cout << "enum y = " << y << std::endl;  // 14
}

int main()
{
    correct_init();
    correct_if_scope();
    correct_loop_scope();
    enum_declaration_point();

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **七种作用域**：块、函数参数、Lambda、命名空间、类、枚举、模板参数
2. **声明点规则**：不同类型声明的声明点位置不同
3. **嵌套与隐藏**：内层作用域可以隐藏外层同名声明
4. **冲突检测**：块作用域中特定声明与父作用域冲突会导致编译错误

### 作用域类型对比

| 作用域类型 | 引入方式 | 范围 |
|------------|----------|------|
| 块作用域 | 选择/迭代语句、处理器、复合语句 | 语句或处理器内 |
| 函数参数作用域 | 参数声明 | 函数定义/声明符/lambda 体结束 |
| Lambda 作用域 | Lambda 表达式（C++14 起） | 从捕获列表后到 lambda 体结束 |
| 命名空间作用域 | 命名空间定义 | 命名空间体及扩展部分 |
| 类作用域 | 类/类模板声明 | 类定义及成员函数定义 |
| 枚举作用域 | 枚举声明 | 枚举器列表 |
| 模板参数作用域 | 模板声明 | 模板参数列表到模板结束 |

### 与 C 语言的区别

| 特性 | C++ | C |
|------|-----|---|
| 结构体作用域 | 有 | 无 |
| 命名空间作用域 | 有 | 无 |
| Lambda 作用域 | 有 | 无 |
| 循环体隐藏循环变量 | 不允许 | 允许 |
| 模板参数作用域 | 有 | 无 |

```
C++ vs C 作用域类型对比
┌────────────────┬────────────────────────────────┬─────────────────────────────────────────────────────────────────────────────────────┐                       
│  C 语言作用域  │          C++ 对应概念          │                                        说明                                      │ 
├────────────────┼────────────────────────────────┼─────────────────────────────────────────────────────────────────────────────────────┤                        
│ 文件作用域     │ 命名空间作用域（全局命名空间） │ C++ 用命名空间替代文件作用域的概念。全局命名空间作用域相当于 C 的文件作用域         │  
├────────────────┼────────────────────────────────┼─────────────────────────────────────────────────────────────────────────────────────┤
│ 函数作用域     │ 标签隐式具有函数作用域         │ C++ 中标签也具有函数作用域，但没有单独归类为一个作用域类型。标签在整个函数内可见    │
├────────────────┼────────────────────────────────┼─────────────────────────────────────────────────────────────────────────────────────┤
│ 函数原型作用域 │ 函数参数作用域                 │ 功能类似，但 C++ 的函数参数作用域有更多扩展（如 lambda、推导指引、requires 表达式） │
├────────────────┼────────────────────────────────┼─────────────────────────────────────────────────────────────────────────────────────┤
│ 块作用域       │ 块作用域                       │ 基本相同，但 C++ 增加了更多引入块作用域的结构                                       │
└────────────────┴────────────────────────────────┴─────────────────────────────────────────────────────────────────────────────────────┘

C++ 独有的作用域类型
┌────────────────┬──────────┬──────────────────────────────────────────────────────────┐
│   作用域类型   │ 引入版本 │                           说明                           │
├────────────────┼──────────┼──────────────────────────────────────────────────────────┤
│ 类作用域       │ C++ 始终 │ 结构体/类的成员在类作用域内，而非文件作用域（与 C 不同） │
├────────────────┼──────────┼──────────────────────────────────────────────────────────┤
│ 命名空间作用域 │ C++ 始终 │ 替代 C 的文件作用域，支持命名空间嵌套                    │
├────────────────┼──────────┼──────────────────────────────────────────────────────────┤
│ 模板参数作用域 │ C++ 始终 │ 模板参数的作用域                                         │
├────────────────┼──────────┼──────────────────────────────────────────────────────────┤
│ 枚举作用域     │ C++ 始终 │ 枚举器的作用域                                           │
├────────────────┼──────────┼──────────────────────────────────────────────────────────┤
│ Lambda 作用域  │ C++14    │ Lambda 捕获初始化器的作用域                              │
└────────────────┴──────────┴──────────────────────────────────────────────────────────┘
```

### 最佳实践

- 避免变量自初始化陷阱
- 理解声明点的精确位置
- 利用命名空间组织代码
- 注意 C++ 与 C 在循环变量作用域上的差异
- 使用有意义的变量名避免意外隐藏

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 6.4 Scope [basic.scope]
- C++20 标准 (ISO/IEC 14882:2020): 6.4 Scope [basic.scope]
- C++17 标准 (ISO/IEC 14882:2017): 6.3 Scope [basic.scope]
- C++14 标准 (ISO/IEC 14882:2014): 3.3 Scope [basic.scope]
- C++11 标准 (ISO/IEC 14882:2011): 3.3 Scope [basic.scope]
- C++98 标准 (ISO/IEC 14882:1998): 3.3 Declarative regions and scopes [basic.scope]