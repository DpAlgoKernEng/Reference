# 标点符号（Punctuators）

## 1. 概述（Overview）

**标点符号（Punctuators）** 是 C++ 语言中具有独立语法意义的符号集合。它们是编译器识别的独立词法单元（tokens），用于构建表达式、声明、语句、模板和控制结构等程序元素。

C++ 的标点符号在 C 语言基础上进行了显著扩展，增加了模板参数列表、命名空间、类定义、Lambda 表达式、概念约束等现代特性所需的语法支持。

### 分类概览

| 类别 | 符号示例 | 主要用途 |
|------|----------|----------|
| 预处理符号 | `#` `##` | 预处理器指令 |
| 分隔符 | `{ }` `[ ]` `( )` `;` `,` | 语法结构界定 |
| 作用域与成员访问 | `::` `.` `->` `.*` `->*` | 作用域解析、成员访问 |
| 运算符 | `+` `-` `*` `/` `%` 等 | 表达式计算 |
| 复合赋值符 | `+=` `-=` `*=` 等 | 简化赋值操作 |
| 比较运算符 | `<=>` (C++20) | 三路比较 |

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C++98 | 继承 C 语言标点符号，新增 `::` 作用域运算符、模板相关符号 |
| C++11 | 新增 Lambda 捕获 `[ ]`、尾置返回类型 `->`、右值引用 `&&`、`alignas`/`alignof`/`noexcept` 相关符号 |
| C++14 | 新增 `decltype(auto)` 中的 `( )` |
| C++17 | 新增结构化绑定 `[ ]`、嵌套命名空间 `::`、折叠表达式 `( )` |
| C++20 | 新增概念定义 `=`、三路比较 `<=>`、模块 `:` 和 `.`、`requires` 表达式 `{ }` |
| C++23 | 新增多参数下标 `[ ]`、`consteval if` 中的 `!` |
| C++26 | 新增包索引 `[ ]` 中的 `...` |

### 设计动机

C++ 标点符号的设计遵循以下原则：

1. **兼容性**：与 C 语言保持高度兼容
2. **扩展性**：支持面向对象、泛型编程、函数式编程等范式
3. **一致性**：同一符号在不同上下文中具有相似的语义
4. **复合性**：单字符符号可组合成多字符符号

### 替代表示法

C++ 提供了部分运算符的替代表示法（alternative representations），用于不支持某些字符的环境：

| 符号 | 替代表示 |
|------|----------|
| `{` | `<%` |
| `}` | `%>` |
| `[` | `<:` |
| `]` | `:>` |
| `#` | `%:` |
| `##` | `%:%:` |
| `&&` | `and` |
| `||` | `or` |
| `!` | `not` |
| `&` | `bitand` |
| `|` | `bitor` |
| `^` | `xor` |
| `~` | `compl` |
| `&=` | `and_eq` |
| `|=` | `or_eq` |
| `^=` | `xor_eq` |
| `!=` | `not_eq` |

## 3. 语法与参数（Syntax and Parameters）

### 3.1 预处理运算符

| 符号 | 名称 | 用途 |
|------|------|------|
| `#` | 字符串化运算符 | 将宏参数转换为字符串字面量 |
| `##` | 标记粘贴运算符 | 连接两个标记 |

```cpp
#define STRINGIFY(x) #x        // STRINGIFY(hello) → "hello"
#define CONCAT(a, b) a ## b     // CONCAT(var, 1) → var1
```

### 3.2 分隔类标点符号

#### 花括号 `{ }`

```cpp
class MyClass { /* members */ };     // 类定义
enum Color { Red, Green, Blue };     // 枚举定义
void func() { /* body */ }           // 函数体
int arr[] = {1, 2, 3};               // 列表初始化
namespace NS { /* ... */ }           // 命名空间体
extern "C" { /* ... */ }             // 语言链接规范
```

**用途**：

| 场景 | 说明 |
|------|------|
| 类定义 | 界定成员规范 |
| 枚举定义 | 界定枚举器列表 |
| 复合语句 | 函数体、try 块、Lambda 表达式（C++11 起） |
| 初始化 | 聚合初始化 / 列表初始化 |
| 命名空间定义 | 界定命名空间体 |
| 语言链接规范 | 界定声明 |
| requires 表达式 | 界定约束要求（C++20 起） |
| 导出声明 | 界定声明（C++20 起） |

#### 方括号 `[ ]`

```cpp
arr[0];                              // 下标运算符
int arr[10];                         // 数组声明
auto lambda = [x, &y]() { };         // Lambda 捕获（C++11 起）
auto [a, b] = getTuple();            // 结构化绑定（C++17 起）
[[nodiscard]] int func();            // 属性（C++11 起）
```

**用途**：

| 场景 | 说明 |
|------|------|
| 下标运算符 | 数组/容器元素访问 |
| 数组声明符 | 声明或类型标识中的数组维度 |
| new[] 运算符 | 数组分配函数重载 |
| delete[] 运算符 | 数组释放函数重载 |
| Lambda 捕获 | 界定捕获列表（C++11 起） |
| 属性界定 | 界定属性说明符（C++11 起） |
| 结构化绑定 | 界定标识符列表（C++17 起） |
| 包索引 | 界定索引常量表达式（C++26 起） |

#### 圆括号 `( )`

```cpp
(a + b) * c;                         // 表达式分组
func(arg1, arg2);                    // 函数调用
int* p = new int(42);                 // 直接初始化
static_cast<int>(x);                  // 类型转换
decltype(auto) x = func();           // decltype 说明符（C++14 起）
```

**用途**：

| 场景 | 说明 |
|------|------|
| 表达式分组 | 改变运算优先级 |
| 函数调用 | 函数调用运算符 |
| 函数式类型转换 | 界定表达式/初始化器 |
| 类型转换 | `static_cast`、`const_cast` 等 |
| 运算符操作数 | `typeid`、`sizeof`、`sizeof...`、`alignof`、`noexcept`（C++11 起） |
| 函数声明符 | 界定参数列表 |
| Lambda 表达式 | 界定参数列表（C++11 起） |
| 推导指引 | 用户定义推导指引（C++17 起） |
| 控制语句 | `if`、`switch`、`while`、`for`、范围 `for` |
| 异常处理 | 界定捕获参数声明 |
| 静态断言 | 界定操作数（C++11 起） |
| 折叠表达式 | 界定折叠表达式（C++17 起） |

#### 分号 `;`

```cpp
int x = 10;              // 声明结束
x = x + 1;               // 语句结束
for (int i = 0; i < 10; ++i)  // for 语句分隔子句
```

**用途**：

| 场景 | 说明 |
|------|------|
| 语句结束 | 标记语句结束 |
| 声明结束 | 标记声明结束 |
| for 语句 | 分隔条件子句和迭代子句 |
| 模块声明 | 模块声明、导入声明等结束（C++20 起） |

#### 冒号 `:`

```cpp
cond ? a : b;            // 条件运算符
label:                   // 标签
class Derived : public Base { };  // 基类子句
public:                  // 访问说明符
struct { int x : 4; };   // 位域
: member(value) { }     // 成员初始化列表
for (auto& x : container) { }  // 范围 for（C++11 起）
enum class Color : int { };  // 枚举基类型（C++11 起）
```

**用途**：

| 场景 | 说明 |
|------|------|
| 条件运算符 | 三元运算符的一部分 |
| 标签声明 | `case`、`default`、普通标签 |
| 基类子句 | 引入基类 |
| 访问说明符 | `public`、`protected`、`private` |
| 位域声明 | 指定位域宽度 |
| 成员初始化列表 | 构造函数中引入成员初始化 |
| 范围 for | 分隔项声明和范围初始化器（C++11 起） |
| 枚举基类型 | 引入底层类型（C++11 起） |
| 属性说明符 | 分隔属性命名空间和属性列表（C++17 起） |
| 模块分区 | 引入模块分区名（C++20 起） |

#### 逗号 `,`

```cpp
int a = 1, b = 2;        // 声明列表
func(1, 2, 3);           // 参数列表
arr[1, 2] = 0;           // 多参数下标（C++23 起）
```

**用途**：

| 场景 | 说明 |
|------|------|
| 逗号运算符 | 序列点，从左到右求值 |
| 声明列表 | 分隔声明符 |
| 初始化列表 | 分隔初始化器 |
| 函数调用 | 分隔参数 |
| 枚举列表 | 分隔枚举常量 |
| 基类列表 | 分隔基类 |
| 成员初始化列表 | 分隔成员初始化器 |
| 模板参数列表 | 分隔模板参数 |
| Lambda 捕获列表 | 分隔捕获项（C++11 起） |
| 结构化绑定 | 分隔标识符（C++17 起） |
| 多参数下标 | 分隔参数（C++23 起） |

### 3.3 作用域与成员访问

#### 作用域解析运算符 `::`

```cpp
std::cout;               // 命名空间限定
::globalVar;             // 全局作用域
Class::staticMethod();   // 类静态成员
Class::NestedClass;      // 嵌套类型
namespace A::B { }       // 嵌套命名空间定义（C++17 起）
```

**用途**：

| 场景 | 说明 |
|------|------|
| 限定名 | 作用域解析 |
| 指向成员指针声明 | 成员指针类型 |
| new/delete 表达式 | 指示仅查找全局分配/释放函数 |
| 属性作用域 | 指示属性命名空间（C++11 起） |
| 嵌套命名空间 | 嵌套命名空间定义（C++17 起） |

#### 成员访问运算符 `.` 和 `->`

```cpp
obj.member;              // 成员访问
ptr->member;             // 指针成员访问
ptr->*pmember;           // 指向成员指针访问
obj.*pmember;            // 指向成员指针访问
```

| 符号 | 名称 | 用途 |
|------|------|------|
| `.` | 成员访问 | 通过对象访问成员、指定初始化器（C++20 起）、模块名/分区名 |
| `->` | 成员访问 | 通过指针访问成员、尾置返回类型（C++11 起）、推导指引结果类型（C++17 起） |
| `.*` | 指向成员指针访问 | 通过对象访问指向成员指针 |
| `->*` | 指向成员指针访问 | 通过指针访问指向成员指针 |

### 3.4 运算符类标点符号

#### 算术运算符

| 符号 | 名称 | 用途 |
|------|------|------|
| `+` | 加法/正号 | 一元正号、二元加法 |
| `-` | 减法/负号 | 一元负号、二元减法 |
| `*` | 乘法/解引用/指针 | 一元解引用、二元乘法、指针声明 |
| `/` | 除法 | 除法运算 |
| `%` | 取模 | 取模运算 |

#### 位运算符

| 符号 | 名称 | 用途 |
|------|------|------|
| `~` | 按位取反 | 一元补码运算、析构函数名 |
| `&` | 按位与/取地址/左值引用 | 一元取地址、二元按位与、左值引用声明、Lambda 引用捕获、ref 限定符（C++11 起） |
| `|` | 按位或 | 二元按位或 |
| `^` | 按位异或 | 二元按位异或 |
| `<<` | 左移 | 位左移、流插入 |
| `>>` | 右移 | 位右移、流提取 |

#### 逻辑运算符

| 符号 | 名称 | 用途 |
|------|------|------|
| `!` | 逻辑非 | 一元逻辑取反、consteval if（C++23 起） |
| `&&` | 逻辑与 | 短路逻辑与、右值引用声明、ref 限定符（C++11 起） |
| `||` | 逻辑或 | 短路逻辑或 |

#### 关系运算符

| 符号 | 名称 | 用途 |
|------|------|------|
| `<` | 小于 | 比较运算、模板参数列表引入、头文件名引入 |
| `>` | 大于 | 比较运算、模板参数列表结束 |
| `<=` | 小于等于 | 比较运算 |
| `>=` | 大于等于 | 比较运算 |
| `==` | 等于 | 相等比较 |
| `!=` | 不等于 | 不等比较 |
| `<=>` | 三路比较 | 宇宙飞船运算符（C++20 起） |

#### 赋值运算符

| 符号 | 名称 | 用途 |
|------|------|------|
| `=` | 简单赋值 | 赋值、初始化、默认参数、默认模板参数、枚举值、纯虚函数、Lambda 按值捕获、`=default`/`=delete`、类型别名（C++11 起）、概念定义（C++20 起） |
| `+=` | 复合赋值 | 加法赋值 |
| `-=` | 复合赋值 | 减法赋值 |
| `*=` | 复合赋值 | 乘法赋值 |
| `/=` | 复合赋值 | 除法赋值 |
| `%=` | 复合赋值 | 取模赋值 |
| `&=` | 复合赋值 | 按位与赋值 |
| `|=` | 复合赋值 | 按位或赋值 |
| `^=` | 复合赋值 | 按位异或赋值 |
| `<<=` | 复合赋值 | 左移赋值 |
| `>>=` | 复合赋值 | 右移赋值 |

#### 自增/自减运算符

| 符号 | 名称 | 用途 |
|------|------|------|
| `++` | 自增 | 前置/后置自增 |
| `--` | 自减 | 前置/后置自减 |

#### 条件运算符

```cpp
condition ? expression1 : expression2
```

#### 省略号 `...`

```cpp
void func(int count, ...);        // 变参函数
template<typename... Args>        // 参数包声明
void print(Args... args) { }      // 参数包展开
catch (...) { }                    // 捕获所有异常
```

**用途**：

| 场景 | 说明 |
|------|------|
| 变参函数 | 函数声明符、Lambda 表达式（C++11 起）、推导指引（C++17 起） |
| 捕获所有异常 | 异常处理器 |
| 变参宏 | 宏定义 |
| 参数包 | 参数包声明和展开（C++11 起） |
| 包索引 | 包索引表达式和说明符（C++26 起） |

### 3.5 三路比较运算符 `<=>`（C++20）

```cpp
auto result = a <=> b;   // 返回 std::strong_ordering、std::weak_ordering 或 std::partial_ordering
```

三路比较运算符（spaceship operator）返回一个比较类别类型，可一次性确定 `<`、`>`、`==`、`!=`、`<=`、`>=` 六种关系。

## 4. 底层原理（Underlying Principles）

### 词法分析

标点符号在编译的**翻译阶段 3（translation phase 3）** 被识别为独立词法单元：

```
源代码字符序列 → [词法分析] → 标点符号、标识符、字面量、运算符
```

### 最长匹配原则

当多个字符可能组成合法标点符号时，编译器采用**最长匹配原则**（maximal munch）：

```cpp
a+++b    // 解析为 a ++ + b，而非 a + ++ b
a->b     // 解析为 a -> b，而非 a - > b
a<=>b    // 解析为 a <=> b（C++20 起），而非 a < = > b
template<template<typename>> class T>  // 错误：>> 被解析为右移
// C++11 前：template <typename T> class C > 需要空格
// C++11 起：允许 >> 作为两个 > 处理
```

### 模板尖括号的特殊处理

C++11 起，模板声明中的 `>>` 被特殊处理：

```cpp
// C++11 前：需要空格
std::vector<std::vector<int> > matrix;  // 正确
std::vector<std::vector<int>> matrix;    // 错误：>> 被解析为右移运算符

// C++11 起：自动处理
std::vector<std::vector<int>> matrix;   // 正确：>> 被识别为两个 >
```

### 右尖括号歧义问题

C++11 引入的 `>>` 特殊处理规则不适用于所有情况：

```cpp
template<int N>
struct A { };

A<1 >> 2> a;    // 仍然被解析为 A<(1 >> 2)>，而非 A<1> > 2>
A<(1 >> 2)> b;  // 正确：显式括号
```

## 5. 使用场景（Use Cases）

### 5.1 类与对象

```cpp
class Base {
public:
    virtual void foo() = 0;  // 纯虚函数
    virtual ~Base() = default;  // 默认析构函数
};

class Derived : public Base {  // 基类子句
public:
    Derived(int val) : Base(), value(val) { }  // 成员初始化列表
private:
    int value : 4;  // 位域
};
```

### 5.2 模板与泛型

```cpp
// 函数模板
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// 类模板
template<typename T, std::size_t N>
class Array {
    T data[N];
};

// C++20 概念
template<typename T>
concept Numeric = requires(T t) {
    { t + t } -> std::convertible_to<T>;
};

// C++17 结构化绑定
auto [x, y, z] = std::tuple{1, 2, 3};
```

### 5.3 Lambda 表达式（C++11 起）

```cpp
// 基本语法
auto lambda = [captures](params) -> return_type { body };

// 示例
auto add = [](int a, int b) { return a + b; };

auto capture = [x, &y, this, ...args = args] { /* ... */ };

// 泛型 Lambda（C++14）
auto generic = [](auto a, auto b) { return a + b; };

// 模板 Lambda（C++20）
auto templ = []<typename T>(T a, T b) { return a + b; };
```

### 5.4 现代初始化语法

```cpp
// C++11 列表初始化
std::vector<int> v = {1, 2, 3, 4, 5};
std::map<std::string, int> m = {{"one", 1}, {"two", 2}};

// C++17 结构化绑定
auto [first, second] = std::make_pair(1, 2.0);

// C++20 指定初始化器
struct Config {
    int width = 800;
    int height = 600;
    std::string name;
};
Config cfg = {.width = 1024, .name = "window"};
```

### 5.5 模块（C++20 起）

```cpp
// 模块声明
export module my_module;

// 模块分区
export module my_module:part;

// 导入
import <iostream>;
import "my_header.h";
import my_module;

// 私有模块片段
module :private;
```

### 5.6 常见陷阱

1. **运算符优先级混淆**：

```cpp
// 陷阱：位运算符优先级低于比较运算符
if (flags & MASK == 0)       // 错误：解析为 flags & (MASK == 0)
if ((flags & MASK) == 0)     // 正确

// 陷阱：逗号运算符
int x = (1, 2, 3);           // x = 3，前两个表达式被求值但丢弃
```

2. **模板尖括号**：

```cpp
// C++11 前：需要空格
std::vector<std::vector<int> > v;  // 正确
std::vector<std::vector<int>> v;    // 错误

// C++11 起：自动处理
std::vector<std::vector<int>> v;    // 正确
```

3. **右移运算符与嵌套模板**：

```cpp
// C++11 前
template<int N> struct A { };
A<1>>2> a;  // 解析为 A<(1 >> 2)>，即 A<0>

// 需要避免歧义
A<(1>>2)> b;  // 正确：A<0>
```

4. **Lambda 捕获陷阱**：

```cpp
int x = 10;
auto f = [x]() mutable { x++; };  // 按值捕获，修改的是副本
f();
std::cout << x;  // 输出 10，原值未变

auto g = [&x]() { x++; };  // 按引用捕获
g();
std::cout << x;  // 输出 11
```

## 6. 代码示例（Examples）

### 基础用法

```cpp
#include <iostream>
#include <vector>
#include <memory>

class Point {
public:
    Point(double x = 0, double y = 0) : x_(x), y_(y) {}  // 成员初始化列表

    double x() const { return x_; }
    double y() const { return y_; }

private:
    double x_;
    double y_;
};

int main()
{
    // 花括号初始化
    Point p{3.0, 4.0};

    // 范围 for
    int arr[] = {1, 2, 3, 4, 5};
    for (int x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // 作用域解析
    std::cout << "Point: (" << p.x() << ", " << p.y() << ")\n";

    // 条件运算符
    double distance = (p.x() > p.y()) ? p.x() : p.y();
    std::cout << "Max coordinate: " << distance << "\n";

    return 0;
}
```

### 模板与概念（C++20）

```cpp
#include <concepts>
#include <iostream>

// 概念定义
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// 约束模板
template<Numeric T>
T add(T a, T b) {
    return a + b;
}

// requires 子句
template<typename T>
    requires requires(T a, T b) { a + b; }
T multiply(T a, T b) {
    return a * b;
}

// 三路比较（C++20）
struct Value {
    int v;
    auto operator<=>(const Value&) const = default;
};

int main()
{
    std::cout << add(1, 2) << "\n";       // 3
    std::cout << add(1.5, 2.5) << "\n";   // 4.0

    Value a{1}, b{2};
    std::cout << (a < b) << "\n";         // 1 (true)
    std::cout << (a <=> b < 0) << "\n";   // 1 (true)

    return 0;
}
```

### Lambda 表达式

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

int main()
{
    // 基本 Lambda
    auto greet = []() { std::cout << "Hello\n"; };
    greet();

    // 带参数和返回类型
    auto add = [](int a, int b) -> int { return a + b; };
    std::cout << add(1, 2) << "\n";

    // 捕获
    int x = 10;
    int y = 20;

    auto by_value = [x, y]() { return x + y; };     // 按值捕获
    auto by_ref = [&x, &y]() { x++; y++; };         // 按引用捕获
    auto capture_all = [=]() { return x + y; };    // 捕获所有（按值）
    auto capture_all_ref = [&]() { x++; y++; };    // 捕获所有（按引用）

    // 泛型 Lambda（C++14）
    auto generic_add = [](auto a, auto b) { return a + b; };
    std::cout << generic_add(1, 2.5) << "\n";       // 3.5

    // 模板 Lambda（C++20）
    auto templ_lambda = []<typename T>(T a, T b) { return a + b; };
    std::cout << templ_lambda(1, 2) << "\n";       // 3

    // 实际应用：算法
    std::vector<int> v = {3, 1, 4, 1, 5, 9};
    int threshold = 3;
    auto count = std::count_if(v.begin(), v.end(),
        [threshold](int n) { return n > threshold; });
    std::cout << "Count > " << threshold << ": " << count << "\n";

    return 0;
}
```

### 结构化绑定与折叠表达式（C++17）

```cpp
#include <iostream>
#include <tuple>
#include <array>
#include <utility>

// 折叠表达式
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // 一元右折叠
}

template<typename... Args>
auto product(Args... args) {
    return (... * args);  // 一元左折叠
}

int main()
{
    // 结构化绑定 - 元组
    auto [x, y, z] = std::make_tuple(1, 2.0, "three");
    std::cout << x << ", " << y << ", " << z << "\n";

    // 结构化绑定 - 数组
    std::array<int, 3> arr = {10, 20, 30};
    auto [a, b, c] = arr;
    std::cout << a << ", " << b << ", " << c << "\n";

    // 结构化绑定 - 结构体
    struct Point { int x, y; };
    Point p{100, 200};
    auto [px, py] = p;
    std::cout << "Point: (" << px << ", " << py << ")\n";

    // 折叠表达式
    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << "\n";      // 15
    std::cout << "Product: " << product(1, 2, 3, 4) << "\n";  // 24

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>
#include <vector>

int main()
{
    // 错误：运算符优先级
    int a = 1, b = 2, c = 3;
    bool wrong = a < b < c;           // 解析为 (a < b) < c，即 1 < 3 = true
    bool right = (a < b) && (b < c);  // 正确的链式比较
    std::cout << "wrong: " << wrong << ", right: " << right << "\n";

    // 错误：逗号运算符陷阱
    int x;
    x = 1, 2, 3;          // x = 1，然后求值 2 和 3（无效果）
    std::cout << "x = " << x << "\n";  // 输出 1

    x = (1, 2, 3);        // x = 3，逗号运算符返回最后一个值
    std::cout << "x = " << x << "\n";  // 输出 3

    // 错误：位运算优先级
    int flags = 0x0F;
    if (flags & 0x01 == 1) {        // 错误：(flags) & (0x01 == 1)
        std::cout << "Bit 0 is set (wrong)\n";
    }

    if ((flags & 0x01) == 1) {      // 正确
        std::cout << "Bit 0 is set (correct)\n";
    }

    // 错误：Lambda 捕获生命周期
    std::function<int()> make_counter() {
        int count = 0;
        return [&count]() { return ++count; };  // 危险：引用局部变量
    }
    // auto counter = make_counter();  // 未定义行为
    // counter();

    // 正确：按值捕获或使用智能指针
    auto make_counter_safe = []() {
        auto count = std::make_shared<int>(0);
        return [count]() { return ++(*count); };
    };
    auto counter = make_counter_safe();
    std::cout << counter() << "\n";  // 1
    std::cout << counter() << "\n";  // 2

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **词法单元**：标点符号是 C++ 的基本词法单元，在翻译阶段 3 被识别
2. **多重语义**：同一符号在不同上下文中可能有不同含义
3. **最长匹配**：编译器采用最长匹配原则解析标点符号
4. **版本演进**：C++11、C++17、C++20、C++23 不断扩展标点符号的用途

### 与 C 语言标点符号对比

| 特性 | C++ | C |
|------|-----|---|
| 作用域解析 `::` | ✓ | ✗ |
| 模板 `< >` | ✓ | ✗ |
| Lambda `[ ]` `->` | C++11 起 | ✗ |
| 三路比较 `<=>` | C++20 起 | ✗ |
| 结构化绑定 `[ ]` | C++17 起 | ✗ |
| 模块 `.` `:` | C++20 起 | ✗ |
| 属性 `[[ ]]` | C++11 起 | C23 起 |

### 符号分类总览

| 类别 | 符号 | 主要用途 |
|------|------|----------|
| 预处理 | `#` `##` | 预处理器 |
| 分隔符 | `{ }` `[ ]` `( )` `;` `,` | 语法结构界定 |
| 作用域与成员 | `::` `.` `->` `.*` `->*` | 作用域解析、成员访问 |
| 算术运算 | `+` `-` `*` `/` `%` | 数值计算 |
| 关系运算 | `<` `>` `<=` `>=` `==` `!=` `<=>` | 比较运算 |
| 逻辑运算 | `!` `&&` `||` | 逻辑运算 |
| 位运算 | `~` `&` `|` `^` `<<` `>>` | 位操作 |
| 赋值运算 | `=` `+=` `-=` 等 | 赋值操作 |
| 自增自减 | `++` `--` | 计数操作 |
| 条件运算 | `?` `:` | 三元条件 |
| 其他 | `...` | 变参、参数包 |

### 最佳实践

- 使用括号明确运算优先级，避免依赖默认优先级
- 区分 `=`（赋值）和 `==`（比较）
- Lambda 捕获时注意生命周期问题
- 使用 C++11 及以上版本时，无需在嵌套模板间添加空格
- 关注新标准（C++11、C++17、C++20、C++23）引入的新特性

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 5.12 Operators and punctuators [lex.operators]
- C++20 标准 (ISO/IEC 14882:2020): 5.12 Operators and punctuators [lex.operators]
- C++17 标准 (ISO/IEC 14882:2017): 5.12 Operators and punctuators [lex.operators]
- C++14 标准 (ISO/IEC 14882:2014): 2.13 Operators and punctuators [lex.operators]
- C++11 标准 (ISO/IEC 14882:2011): 2.13 Operators and punctuators [lex.operators]
- C++03 标准 (ISO/IEC 14882:2003): 2.12 Operators and punctuators [lex.operators]
- C++98 标准 (ISO/IEC 14882:1998): 2.12 Operators and punctuators [lex.operators]