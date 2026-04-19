# 标识符（Identifiers）

## 1. 概述（Overview）

**标识符（Identifier）** 是 C++ 语言中用于命名程序实体的字符序列。标识符由数字、下划线、大小写拉丁字母和大多数 Unicode 字符组成的任意长序列。

标识符可以用于命名以下实体：
- 对象（objects）
- 引用（references）
- 函数（functions）
- 枚举器（enumerators）
- 类型（types）
- 类成员（class members）
- 命名空间（namespaces）
- 模板（templates）
- 模板特化（template specializations）
- 参数包（parameter packs）（C++11 起）
- goto 标签（goto labels）
- 其他实体

### 标识符特性

| 特性 | 说明 |
|------|------|
| 大小写敏感 | 小写和大写字母是不同的 |
| 字符有效性 | 每个字符都有意义 |
| Unicode 支持 | 支持 XID_Start 和 XID_Continue 属性的字符 |
| 规范化要求 | 必须符合 Unicode 规范化形式 C |

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C++98 | 基础标识符规则，继承自 C 语言 |
| C++11 | 引入用户定义字面量标识符、参数包标识符、特殊含义标识符（final、override） |
| C++14 | 移除部分标准库标识符（僵尸名称） |
| C++20 | 引入模块相关特殊标识符（import、module）、概念特化标识符、模板参数对象 |
| C++26 | 引入包索引说明符 |

### 设计动机

1. **兼容性**：与 C 语言保持兼容的基础标识符规则
2. **扩展性**：支持现代编程范式（泛型、函数式等）
3. **国际化**：通过 Unicode 支持多语言编程
4. **安全性**：通过保留标识符规则保护实现和标准库

### 与 C 语言的区别

| 特性 | C++ | C |
|------|-----|---|
| 双下划线保留 | 任何位置 | 仅开头 |
| 特殊含义标识符 | final、override、import、module | 无 |
| 僵尸名称 | C++14 起移除部分标识符 | 无 |
| 隐式成员访问转换 | 支持 | 不支持 |

### 僵尸标识符（C++14 起）

从 C++14 开始，某些标识符从 C++ 标准库中移除，但仍为之前的标准化保留：

- 移除的成员函数名不能用作类函数宏名
- 其他移除的成员名不能用作对象宏名

## 3. 语法与参数（Syntax and Parameters）

### 3.1 标识符语法规则

**首字符要求**：
- 大写拉丁字母 A-Z
- 小写拉丁字母 a-z
- 下划线 `_`
- 任何具有 XID_Start Unicode 属性的字符

**后续字符要求**：
- 数字 0-9
- 大写拉丁字母 A-Z
- 小写拉丁字母 a-z
- 下划线 `_`
- 任何具有 XID_Continue Unicode 属性的字符

```
identifier:
    nondigit
    identifier nondigit
    identifier digit

nondigit:
    _ a b c d e f g h i j k l m n o p q r s t u v w x y z
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
    universal-character-name

digit:
    0 1 2 3 4 5 6 7 8 9
```

### 3.2 非限定标识符

除了适当声明的标识符外，以下内容也可以在表达式中使用：

| 类型 | 示例 |
|------|------|
| 重载运算符名（函数表示法） | `operator+`、`operator new` |
| 用户定义转换函数名 | `operator bool` |
| 用户定义字面量运算符名（C++11 起） | `operator "" _km` |
| 模板名及其参数列表 | `MyTemplate<int>` |
| `~` 后跟类名 | `~MyClass` |
| `~` 后跟 decltype 说明符（C++11 起） | `~decltype(str)` |
| `~` 后跟包索引说明符（C++26 起） | `~pack...[0]` |

### 3.3 限定标识符

限定标识符表达式是非限定标识符表达式前加上作用域解析运算符 `::`，以及可选的一系列以下内容（用 `::` 分隔）：

- 命名空间名
- 类名
- 枚举名
- decltype 说明符（表示类或枚举类型）（C++11 起）
- 包索引说明符（表示类或枚举类型）（C++26 起）

```cpp
std::string::npos     // std 命名空间中 string 类的静态成员 npos
::tolower             // 全局命名空间中的 tolower 函数
::std::cout           // 顶层命名空间 std 中的全局变量 cout
boost::signals2::connection  // boost 命名空间中 signals2 命名空间的类型 connection
```

### 3.4 名称（Names）

**名称**是使用以下方式之一引用实体：

| 类型 | 示例 |
|------|------|
| 标识符 | `myVariable` |
| 重载运算符名 | `operator+`、`operator new` |
| 用户定义转换函数名 | `operator bool` |
| 用户定义字面量运算符名（C++11 起） | `operator ""_km` |
| 模板名及其参数列表 | `MyTemplate<int>` |

每个名称都通过**声明**引入程序。在多个翻译单元中使用的名称可能引用相同或不同的实体，取决于链接。

## 4. 底层原理（Underlying Principles）

### 4.1 名称查找

当编译器遇到未知的名称时，通过**名称查找**将其与引入该名称的声明关联：

```cpp
int x = 10;           // 声明引入名称 x

void func() {
    std::cout << x;   // 名称查找找到全局 x
}
```

### 4.2 隐式成员访问转换

如果标识符表达式 E 表示某个类 C 的非静态非类型成员，且满足以下条件，E 被转换为类成员访问表达式 `this->E`：

1. E 不是成员访问运算符的右操作数
2. 如果 E 是限定标识符表达式，E 不是取地址运算符的非括号化操作数
3. 满足以下条件之一：
   - E 是可能被求值的
   - C 是 E 处最内层包围类
   - C 是 E 处最内层包围类的基类

```cpp
struct X {
    int x;
};

struct B {
    int b;
};

struct D : B {
    X d;

    void func()
    {
        d;   // OK，转换为 this->d
        b;   // OK，转换为 this->b
        x;   // 错误：this->x 是非良构的（X 不是基类）

        d.x; // OK，转换为 this->d.x
    }
};
```

### 4.3 类型推导规则

标识符表达式的类型与其命名的实体类型相同，但有例外：

**Lambda 捕获的特殊情况（C++11 起）**：

```cpp
void f()
{
    float x, &r = x;

    [=]
    {
        decltype(x) y1;        // y1 类型为 float
        decltype((x)) y2 = y1; // y2 类型为 float const&
                               // 因为该 lambda 非 mutable 且 x 是左值
        decltype(r) r1 = y1;   // r1 类型为 float&
        decltype((r)) r2 = y2; // r2 类型为 float const&
    };
}
```

**模板参数对象（C++20 起）**：

如果命名的实体是类型 T 的模板参数的模板参数对象，表达式的类型为 `const T`。

### 4.4 值类别

| 实体类型 | 值类别 |
|----------|--------|
| 函数 | 左值（lvalue） |
| 变量 | 左值（lvalue） |
| 模板参数对象（C++20 起） | 左值（lvalue） |
| 数据成员 | 左值（lvalue） |
| 枚举器 | 右值（C++11 前）/ 纯右值（C++11 起） |
| 概念特化（C++20 起） | bool 纯右值 |

## 5. 使用场景（Use Cases）

### 5.1 命名约定

| 类型 | 约定 | 示例 |
|------|------|------|
| 宏常量 | 全大写 + 下划线 | `MAX_SIZE` |
| 常量 | 全大写 + 下划线 或 k 前缀 | `MAX_SIZE` / `kMaxSize` |
| 变量 | 小写 + 下划线 或 小驼峰 | `user_name` / `userName` |
| 函数 | 小写 + 下划线 或 小驼峰 | `get_value()` / `getValue()` |
| 类/结构体 | 大驼峰 | `MyClass` / `Rectangle` |
| 模板参数 | 单个大写字母 或 大驼峰 | `T` / `typename Allocator` |
| 命名空间 | 小写 | `std` / `boost` |

### 5.2 避免保留标识符

**禁止使用的标识符**：

```cpp
// 1. 关键字
int int;              // 错误：int 是关键字
int class;            // 错误：class 是关键字

// 2. 全局命名空间中以 _ 开头
int _global;          // 危险：全局命名空间保留

// 3. 包含双下划线
int my__var;          // 危险：包含 __，始终保留
int __reserved;       // 危险：以 __ 开头

// 4. 以 _ + 大写字母开头
int _Reserved;        // 危险：保留给实现
```

### 5.3 特殊含义标识符（C++11 起）

| 标识符 | 用途 | 说明 |
|--------|------|------|
| `final` | 类/虚函数 | 禁止继承/重写 |
| `override` | 虚函数 | 显式重写声明 |
| `import`（C++20） | 模块 | 导入模块 |
| `module`（C++20） | 模块 | 模块声明 |

```cpp
class Base {
public:
    virtual void foo() final;  // final：禁止重写
};

class Derived final : Base {   // final：禁止继承
public:
    void foo() override;       // override：显式重写
};
```

### 5.4 属性中的标识符（C++11 起）

关键字在属性标记中可作为普通标识符使用：

```cpp
[[private]]  // private 在属性中不是关键字
[[nodiscard]]
[[maybe_unused]]
```

### 5.5 常见陷阱

1. **与保留标识符冲突**：

```cpp
// 错误：使用保留标识符
int _count = 0;        // 危险：全局命名空间以 _ 开头
int __myvar = 0;       // 危险：包含双下划线
int _MyVar = 0;        // 危险：以 _ + 大写字母开头
```

2. **大小写敏感性**：

```cpp
int value = 1;
int Value = 2;    // 不同的标识符
int VALUE = 3;    // 又一个不同的标识符
```

3. **头文件保护**：

```cpp
// 错误：以 _ + 大写字母开头
#ifndef _MYHEADER_H
#define _MYHEADER_H
// ...
#endif

// 正确：使用非保留标识符
#ifndef MYHEADER_H
#define MYHEADER_H
// ...
#endif
```

## 6. 代码示例（Examples）

### 基础用法

```cpp
#include <iostream>
#include <string>

// 命名空间
namespace mylib {

// 类定义
class Calculator {
public:
    // 构造函数
    Calculator() = default;

    // 成员函数
    int add(int a, int b) {
        return a + b;
    }

private:
    int result_;  // 成员变量：_ 后缀（非保留）
};

// 函数
void print_message(const std::string& message) {
    std::cout << message << std::endl;
}

}  // namespace mylib

int main()
{
    mylib::Calculator calc;
    int result = calc.add(1, 2);
    std::cout << "Result: " << result << std::endl;

    mylib::print_message("Hello, C++!");
    return 0;
}
```

### 模板与泛型

```cpp
#include <iostream>
#include <concepts>  // C++20

// 模板参数命名约定
template<typename T, typename Allocator = std::allocator<T>>
class Container {
public:
    using value_type = T;
    using size_type = std::size_t;

    void push_back(const T& value) {
        // ...
    }

private:
    T* data_;
    size_type size_;
};

// C++20 概念
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T>
T add(T a, T b) {
    return a + b;
}

int main()
{
    Container<int> container;
    container.push_back(42);

    std::cout << add(1, 2) << std::endl;
    std::cout << add(1.5, 2.5) << std::endl;

    return 0;
}
```

### 用户定义字面量（C++11 起）

```cpp
#include <iostream>

// 用户定义字面量运算符
constexpr long double operator"" _km(long double x) {
    return x * 1000.0;  // 公里转米
}

constexpr long double operator"" _mi(long double x) {
    return x * 1609.344;  // 英里转米
}

// 用户定义转换函数
class Boolean {
public:
    explicit operator bool() const {
        return value_;
    }

private:
    bool value_ = true;
};

int main()
{
    auto distance_km = 5.0_km;
    auto distance_mi = 3.0_mi;

    std::cout << "5 km = " << distance_km << " m" << std::endl;
    std::cout << "3 mi = " << distance_mi << " m" << std::endl;

    Boolean b;
    if (b) {
        std::cout << "Boolean is true" << std::endl;
    }

    return 0;
}
```

### 限定标识符与作用域

```cpp
#include <iostream>

namespace outer {
namespace inner {
    int value = 42;

    void print() {
        std::cout << "inner::value = " << value << std::endl;
    }
}

int value = 100;
}

// 全局变量
int value = 0;

int main()
{
    // 使用限定标识符
    std::cout << "Global value: " << ::value << std::endl;
    std::cout << "outer::value: " << outer::value << std::endl;
    std::cout << "outer::inner::value: " << outer::inner::value << std::endl;

    outer::inner::print();

    return 0;
}
```

### Lambda 与 decltype（C++11 起）

```cpp
#include <iostream>
#include <type_traits>

int main()
{
    float x = 3.14f;
    float& r = x;

    // Lambda 按值捕获
    auto lambda = [=]() {
        // decltype(x) 是 float（捕获的副本）
        decltype(x) y1 = 1.0f;

        // decltype((x)) 是 float const&（因为 lambda 非 mutable）
        decltype((x)) y2 = y1;

        std::cout << "y1 = " << y1 << std::endl;
        std::cout << "y2 = " << y2 << std::endl;

        // 验证类型
        static_assert(std::is_same_v<decltype(y1), float>);
        static_assert(std::is_same_v<decltype(y2), const float&>);
    };

    lambda();

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>

/* 错误：使用保留标识符 */
int _global = 0;           // 危险：全局命名空间以 _ 开头
int __reserved = 0;        // 危险：以 __ 开头
int _Uppercase = 0;        // 危险：以 _ + 大写字母开头

/* 正确：使用非保留标识符 */
int my_global = 0;         // 安全
int reserved_value = 0;    // 安全

/* 错误：宏重定义关键字 */
// #define int long          // 未定义行为：重定义关键字

/* 正确：使用不同的名称 */
#define my_int long         // 安全（但不推荐用宏定义类型）

/* 错误：头文件保护使用保留标识符 */
// #ifndef _MYHEADER_H      // 危险
// #define _MYHEADER_H

/* 正确：使用非保留标识符 */
#ifndef MYHEADER_H
#define MYHEADER_H
// 头文件内容
#endif

class Example {
public:
    void func()
    {
        /* 隐式成员访问转换示例 */
        // member;  // 转换为 this->member
    }

private:
    int member;
};

/* 模板中的保留名称 */
template<typename T>
class Container {
    // 错误：以 _ 开头后跟大写字母
    // int _Size;

    // 正确：使用非保留名称
    int size_;
    int m_size;  // 或使用 m_ 前缀约定
};

int main()
{
    std::cout << my_global << std::endl;
    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **命名规则**：首字符必须是 XID_Start 类字符，后续字符必须是 XID_Continue 类字符
2. **区分大小写**：大小写字母是不同的字符
3. **规范化要求**：必须符合 Unicode 规范化形式 C（NFC）
4. **保留标识符**：关键字、全局 `_` 前缀、`__` 包含、`_` + 大写字母开头

### 保留标识符规则

| 规则 | 示例 | 说明 |
|------|------|------|
| 关键字 | `int`、`class`、`return` | 不能用于其他目的（属性中除外） |
| 全局命名空间 `_` 前缀 | `_global` | 全局作用域中保留 |
| 包含 `__` | `my__var`、`__reserved` | 任何位置保留 |
| `_` + 大写字母开头 | `_Reserved` | 始终保留 |
| 替代表示 | `and`、`or`、`not` | 运算符替代表示 |

### 特殊含义标识符（C++11 起）

| 标识符 | 上下文 | 行为 |
|--------|--------|------|
| `final` | 类声明 | 禁止继承 |
| `final` | 虚函数 | 禁止重写 |
| `override` | 虚函数 | 显式重写 |
| `import`（C++20） | 模块 | 导入模块 |
| `module`（C++20） | 模块 | 模块声明 |

### 与 C 语言的区别

| 特性 | C++ | C |
|------|-----|---|
| 双下划线保留 | 任何位置 | 仅开头 |
| 特殊含义标识符 | C++11 起 | 无 |
| 僵尸标识符 | C++14 起 | 无 |
| 隐式成员访问 | 支持 | 不支持 |

### 最佳实践

- 使用有意义的描述性名称
- 避免使用保留标识符
- 遵循一致的命名约定
- 头文件保护不要使用保留标识符
- 注意 Unicode 支持的编译器差异

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 5.13 Identifiers
- C++20 标准 (ISO/IEC 14882:2020): 5.13 Identifiers
- C++17 标准 (ISO/IEC 14882:2017): 5.10 Identifiers
- C++14 标准 (ISO/IEC 14882:2014): 2.14 Identifiers
- C++11 标准 (ISO/IEC 14882:2011): 2.14 Identifiers
- C++03 标准 (ISO/IEC 14882:2003): 2.10 Identifiers
- C++98 标准 (ISO/IEC 14882:1998): 2.10 Identifiers