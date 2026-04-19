# C++ 关键字 (C++ Keywords)

## 1. 概述 (Overview)

**关键字 (Keywords)** 是 C++ 语言中具有特殊含义的保留标识符。由于它们被语言本身使用，这些关键字不能被重新定义或重载。作为一个例外，在属性（不包括属性参数列表）中它们不被视为保留字。（C++11 起）

### 关键字特点

- **保留性**：关键字不能用作变量名、函数名或其他标识符
- **不可重载**：不能重定义关键字的含义
- **版本演进**：不同 C++ 标准版本引入了不同的关键字
- **替代表示**：部分关键字有替代拼写形式

### 特殊说明

在属性（不包括属性参数列表）中，关键字不被视为保留字。这意味着可以在属性中使用与关键字同名的属性标识符。

## 2. 来源与演变 (Origin and Evolution)

### 标准演进历程

| C++ 标准版本 | 关键字变化 |
|-------------|-----------|
| C++98 | 63 个基础关键字（含 C 兼容关键字） |
| C++11 | 新增 `alignas`、`alignof`、`char16_t`、`char32_t`、`constexpr`、`decltype`、`noexcept`、`nullptr`、`static_assert`、`thread_local` |
| C++14 | 为 `decltype` 添加新语义 |
| C++17 | 修改/新增 `auto`、`constexpr`、`if`、`register`、`static_assert`、`typename` 等的语义 |
| C++20 | 新增 `char8_t`、`concept`、`consteval`、`constinit`、`co_await`、`co_return`、`co_yield`、`requires` |
| C++23 | 为 `auto`、`consteval`、`if`、`this` 添加新语义 |

### 语义变更标记说明

| 标记 | 含义 |
|-----|------|
| (1) | C++11 中含义变更或添加新含义 |
| (2) | C++14 中添加新含义 |
| (3) | C++17 中含义变更或添加新含义 |
| (4) | C++20 中含义变更或添加新含义 |
| (5) | C++23 中添加新含义 |

### 技术规范（TS）中的关键字

以下关键字来自技术规范（Technical Specifications），尚未正式纳入标准：

| 关键字 | 来源 |
|-------|------|
| `atomic_cancel` | 事务内存 TS (TM TS) |
| `atomic_commit` | 事务内存 TS (TM TS) |
| `atomic_noexcept` | 事务内存 TS (TM TS) |
| `synchronized` | 事务内存 TS (TM TS) |
| `reflexpr` | 反射 TS (Reflection TS) |
| `transaction_safe` | 事务内存 TS |
| `transaction_safe_dynamic` | 事务内存 TS |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 完整关键字列表

#### A – C

| 关键字 | 版本信息 | 说明 |
|-------|---------|------|
| `alignas` | C++11 | 对齐指定符 |
| `alignof` | C++11 | 对齐查询运算符 |
| `and` | - | `&&` 的替代表示 |
| `and_eq` | - | `&=` 的替代表示 |
| `asm` | - | 内联汇编（条件支持） |
| `auto` | (1)(3)(4)(5) | 自动类型推导 / 存储类说明符 |
| `bitand` | - | `&` 的替代表示 |
| `bitor` | - | `\|` 的替代表示 |
| `bool` | - | 布尔类型 |
| `break` | - | 跳出循环/switch |
| `case` | - | switch 分支标签 |
| `catch` | - | 异常捕获 |
| `char` | - | 字符类型 |
| `char8_t` | C++20 | UTF-8 字符类型 |
| `char16_t` | C++11 | UTF-16 字符类型 |
| `char32_t` | C++11 | UTF-32 字符类型 |
| `class` | (1) | 类定义 |
| `compl` | - | `~` 的替代表示 |
| `concept` | C++20 | 概念定义 |
| `const` | - | 常量限定符 |
| `consteval` | C++20(5) | 立即函数 |
| `constexpr` | C++11(3) | 常量表达式 |
| `constinit` | C++20 | 常量初始化 |
| `const_cast` | - | 常量转换 |
| `continue` | - | 继续下一次迭代 |
| `co_await` | C++20 | 协程等待 |
| `co_return` | C++20 | 协程返回 |
| `co_yield` | C++20 | 协程产出 |

#### D – P

| 关键字 | 版本信息 | 说明 |
|-------|---------|------|
| `decltype` | C++11(2) | 类型推导 |
| `default` | (1) | 默认分支/默认函数 |
| `delete` | (1) | 删除函数/内存释放 |
| `do` | - | do-while 循环 |
| `double` | - | 双精度浮点型 |
| `dynamic_cast` | - | 动态类型转换 |
| `else` | - | 条件分支 |
| `enum` | (1) | 枚举类型 |
| `explicit` | - | 显式构造函数/转换 |
| `export` | (1)(4) | 模板导出（C++20 模块） |
| `extern` | (1) | 外部链接声明 |
| `false` | - | 布尔假值 |
| `float` | - | 单精度浮点型 |
| `for` | (1) | for 循环 |
| `friend` | - | 友元声明 |
| `goto` | - | 无条件跳转 |
| `if` | (3)(5) | 条件语句 / constexpr if |
| `inline` | (1)(3) | 内联说明符 |
| `int` | (1) | 整型 |
| `long` | - | 长整型 |
| `mutable` | (1) | 可变成员 |
| `namespace` | - | 命名空间 |
| `new` | - | 内存分配 |
| `noexcept` | C++11 | 异常说明符 |
| `not` | - | `!` 的替代表示 |
| `not_eq` | - | `!=` 的替代表示 |
| `nullptr` | C++11 | 空指针常量 |
| `operator` | (1) | 运算符重载 |
| `or` | - | `\|\|` 的替代表示 |
| `or_eq` | - | `\|=` 的替代表示 |
| `private` | (4) | 私有访问 |
| `protected` | - | 保护访问 |
| `public` | - | 公有访问 |

#### R – Z

| 关键字 | 版本信息 | 说明 |
|-------|---------|------|
| `register` | (3) | 寄存器存储类（已弃用语义） |
| `reinterpret_cast` | - | 重新解释转换 |
| `requires` | C++20 | 约束要求 |
| `return` | - | 函数返回 |
| `short` | - | 短整型 |
| `signed` | - | 有符号类型 |
| `sizeof` | (1) | 获取大小运算符 |
| `static` | - | 静态说明符 |
| `static_assert` | C++11 | 编译时断言 |
| `static_cast` | - | 静态类型转换 |
| `struct` | (1) | 结构体 |
| `switch` | - | 多分支选择 |
| `template` | - | 模板定义 |
| `this` | (5) | 当前对象指针 |
| `thread_local` | C++11 | 线程局部存储 |
| `throw` | (3)(4) | 抛出异常 |
| `true` | - | 布尔真值 |
| `try` | - | 异常处理块 |
| `typedef` | - | 类型别名 |
| `typeid` | - | 类型信息 |
| `typename` | (3)(4) | 类型名说明符 |
| `union` | - | 联合体 |
| `unsigned` | - | 无符号类型 |
| `using` | (1)(4) | 使用声明/类型别名 |
| `virtual` | - | 虚函数 |
| `void` | - | 空类型 |
| `volatile` | - | 易变限定符 |
| `wchar_t` | - | 宽字符类型 |
| `while` | - | while 循环 |
| `xor` | - | `^` 的替代表示 |
| `xor_eq` | - | `^=` 的替代表示 |

### 3.2 替代表示 (Alternative Representations)

以下关键字提供标准运算符的替代表示：

| 替代表示 | 等价运算符 |
|---------|-----------|
| `and` | `&&` |
| `and_eq` | `&=` |
| `bitand` | `&` |
| `bitor` | `\|` |
| `compl` | `~` |
| `not` | `!` |
| `not_eq` | `!=` |
| `or` | `\|\|` |
| `or_eq` | `\|=` |
| `xor` | `^` |
| `xor_eq` | `^=` |

**注意**：这些替代表示在属性中仍被视为保留字。

### 3.3 二字符组和三字符组

**二字符组 (Digraphs)**：

| 二字符组 | 等价于 |
|---------|-------|
| `<%` | `{` |
| `%>` | `}` |
| `<:` | `[` |
| `:>` | `]` |
| `%:` | `#` |
| `%:%:` | `##` |

**三字符组 (Trigraphs)**（C++17 前有效，C++17 起移除）：

| 三字符组 | 等价于 |
|---------|-------|
| `??<` | `{` |
| `??>` | `}` |
| `??(` | `[` |
| `??)` | `]` |
| `??=` | `#` |
| `??/` | `\` |
| `??'` | `^` |
| `??!` | `\|` |
| `??-` | `~` |

### 3.4 具有特殊含义的标识符

以下标识符可用作对象或函数名，但在特定上下文中具有特殊含义：

| 标识符 | 版本 | 特殊含义 |
|-------|------|---------|
| `final` | C++11 | 禁止类被继承或虚函数被重写 |
| `override` | C++11 | 显式声明重写虚函数 |
| `import` | C++20 | 模块导入 |
| `module` | C++20 | 模块声明 |

### 3.5 预处理器指令关键字

在预处理指令上下文中识别的关键字：

| 关键字 | 版本 | 说明 |
|-------|------|------|
| `if` | - | 条件编译 |
| `elif` | - | 否则如果 |
| `else` | - | 否则 |
| `endif` | - | 结束条件 |
| `ifdef` | - | 如果定义 |
| `ifndef` | - | 如果未定义 |
| `elifdef` | C++23 | 否则如果定义 |
| `elifndef` | C++23 | 否则如果未定义 |
| `define` | - | 宏定义 |
| `undef` | - | 取消宏定义 |
| `include` | - | 文件包含 |
| `line` | - | 行号控制 |
| `error` | - | 生成错误 |
| `warning` | C++23 | 生成警告 |
| `pragma` | - | 编译器指令 |
| `defined` | - | 检查宏定义 |
| `__has_include` | C++17 | 检查头文件存在 |
| `__has_cpp_attribute` | C++20 | 检查属性支持 |
| `export` | C++20 | 模块导出 |
| `import` | C++20 | 模块导入 |
| `module` | C++20 | 模块声明 |

在预处理指令上下文外识别的关键字：
- `_Pragma` (C++11) — `#pragma` 的运算符形式

## 4. 底层原理 (Underlying Principles)

### 4.1 标识符保留规则

**始终保留的标识符**：
- 包含双下划线 `__` 的标识符（任何位置）
- 以单下划线 `_` 开头后跟大写字母的标识符
- 以单下划线 `_` 开头的标识符（保留用于全局命名空间）

### 4.2 命名空间保留

**`std` 命名空间**：用于放置标准 C++ 库的名称。向其添加名称需遵循特定规则（参见扩展 std 命名空间）。

**`posix` 命名空间**：保留用于未来的顶层命名空间。如果程序在该命名空间中声明或定义任何内容，行为未定义。

### 4.3 关键字在属性中的处理

C++11 起，在属性中关键字不被视为保留字（属性参数列表除外）：

```cpp
// 合法：在属性中使用与关键字同名的标识符
[[noreturn]] void f();  // noreturn 不是关键字
[[deprecated]] int x;   // deprecated 不是关键字

// 但某些实现可能将替代表示关键字也视为保留
[[and]] int y;  // 可能有问题
```

## 5. 使用场景 (Use Cases)

### 5.1 数据类型定义

```cpp
// 基本类型
int count = 10;
char letter = 'A';
double value = 3.14159;

// C++11 新类型
char16_t utf16 = u'字';
char32_t utf32 = U'字';
char8_t utf8 = u8'字';  // C++20

// 布尔类型
bool flag = true;
bool done = false;
```

### 5.2 类型推导（C++11）

```cpp
// auto 类型推导
auto x = 42;           // int
auto ptr = &x;         // int*
auto lambda = [](int n) { return n * 2; };  // lambda 类型

// decltype 类型推导
decltype(x) y = x;     // int
decltype((x)) ref = x; // int&
```

### 5.3 常量表达式（C++11）

```cpp
// constexpr 变量
constexpr int MAX_SIZE = 100;
constexpr double PI = 3.14159265358979;

// constexpr 函数
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// consteval 立即函数（C++20）
consteval int square(int n) {
    return n * n;
}
```

### 5.4 概念与约束（C++20）

```cpp
// 概念定义
template<typename T>
concept Numeric = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

// 使用概念
template<Numeric T>
T add(T a, T b) {
    return a + b;
}

// requires 子句
template<typename T>
    requires std::integral<T>
T modulo(T a, T b) {
    return a % b;
}
```

### 5.5 协程（C++20）

```cpp
#include <coroutine>

// 协程示例
Task<std::string> async_read() {
    std::string data = co_await read_from_network();
    co_return data;
}

Task<void> process() {
    auto data = co_await async_read();
    co_yield process_data(data);
}
```

### 5.6 编译时断言

```cpp
// static_assert
static_assert(sizeof(int) == 4, "int must be 4 bytes");
static_assert(sizeof(void*) == 8, "64-bit platform required");

// 带模板参数的静态断言
template<typename T>
void process(T value) {
    static_assert(std::is_integral_v<T>, "T must be integral");
}
```

## 6. 代码示例 (Examples)

### 6.1 基础用法：类型定义与控制流

```cpp
#include <iostream>

int main() {
    // 基本类型
    int integer = 42;
    double decimal = 3.14;
    bool flag = true;

    // 控制流
    if (flag) {
        std::cout << "Flag is true\n";
    }

    for (int i = 0; i < 5; ++i) {
        switch (i) {
            case 0:
                std::cout << "Zero\n";
                break;
            case 1:
                std::cout << "One\n";
                break;
            default:
                std::cout << i << "\n";
        }
    }

    return 0;
}
```

### 6.2 基础用法：类定义

```cpp
#include <string>

class Person {
public:
    Person(std::string name, int age)
        : name_(std::move(name)), age_(age) {}

    // virtual 函数
    virtual void greet() const {
        std::cout << "Hello, I'm " << name_ << std::endl;
    }

    // override 示例
    void setAge(int age) { age_ = age; }

protected:
    std::string name_;
    int age_;
};

class Student : public Person {
public:
    using Person::Person;  // 继承构造函数

    void greet() const override {  // override 关键字
        std::cout << "Hi, I'm student " << name_ << std::endl;
    }
};

// final 示例
class FinalClass final {  // 不能被继承
    void doSomething() final {}  // 不能被重写
};
```

### 6.3 高级用法：现代 C++ 特性

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <concepts>

// C++20 概念
template<typename T>
concept Printable = requires(std::ostream& os, T value) {
    { os << value };
};

// constexpr if (C++17)
template<typename T>
void process(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integer: " << value << std::endl;
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Float: " << value << std::endl;
    } else {
        std::cout << "Other type" << std::endl;
    }
}

// 立即函数 (C++20)
consteval int compile_time_square(int n) {
    return n * n;
}

// 常量初始化 (C++20)
constinit int global_val = compile_time_square(5);

int main() {
    // auto 类型推导
    auto value = 42;
    auto text = "Hello";

    // 结构化绑定 (C++17)
    std::pair<int, std::string> p{1, "one"};
    auto [num, str] = p;

    // constexpr
    constexpr int size = compile_time_square(3);

    // 编译时 if
    process(42);
    process(3.14);

    return 0;
}
```

### 6.4 高级用法：模板与概念

```cpp
#include <concepts>
#include <vector>
#include <iostream>

// 定义概念
template<typename T>
concept Container = requires(T c) {
    typename T::value_type;
    typename T::iterator;
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
};

// 使用概念约束模板
template<Container C>
void print_container(const C& container) {
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

// requires 表达式
template<typename T>
    requires requires(T a, T b) { a + b; }
auto add(T a, T b) {
    return a + b;
}

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    print_container(v);

    auto result = add(1, 2);
    return 0;
}
```

### 6.5 高级用法：协程

```cpp
#include <coroutine>
#include <iostream>
#include <generator>  // C++23

// 简单的生成器协程
std::generator<int> fibonacci(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        auto temp = a;
        a = b;
        b = temp + b;
    }
}

int main() {
    // 使用协程生成斐波那契数列
    for (int value : fibonacci(10)) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    // 输出: 0 1 1 2 3 5 8 13 21 34

    return 0;
}
```

### 6.6 常见错误及修正

**错误示例 1：使用关键字作为标识符**

```cpp
// 错误代码
int class = 10;        // 错误：class 是关键字
float new = 3.14f;     // 错误：new 是关键字
bool delete = true;    // 错误：delete 是关键字
```

```cpp
// 修正方法：使用不同的标识符名
int category = 10;
float value = 3.14f;
bool removed = true;
```

**错误示例 2：忽略 auto 语义变更**

```cpp
// C++98 中的 auto（存储类说明符）
auto int x = 10;  // C++11 前合法，C++11 起错误

// C++11 中 auto 用于类型推导
auto y = 10;      // C++11: y 是 int
```

```cpp
// 正确理解：C++11 后 auto 用于类型推导
auto x = 10;           // int
auto ptr = &x;         // int*
auto& ref = x;         // int&
const auto& cref = x;  // const int&
```

**错误示例 3：混淆 override 和 final 的用法**

```cpp
// 错误代码
class Base {
    virtual void foo() final;  // OK：禁止重写
};

class Derived : public Base {
    void foo() override;  // 错误：Base::foo 是 final
};
```

```cpp
// 正确用法
class Base {
    virtual void foo();
    virtual void bar() final;  // 禁止派生类重写
};

class Derived final : public Base {  // 禁止进一步派生
    void foo() override;  // OK：重写 Base::foo
    // void bar() override;  // 错误：bar 是 final
};
```

**错误示例 4：constexpr vs consteval 混淆**

```cpp
// 错误理解
constexpr int get_value() {
    return 42;
}

int main() {
    int x = 10;
    constexpr int a = get_value();  // OK
    // constexpr int b = x;  // 错误：x 不是常量表达式

    consteval int square(int n) {
        return n * n;
    }

    // int y = square(x);  // 错误：x 不是编译时常量
    int y = square(10);     // OK：10 是编译时常量
}
```

```cpp
// 正确理解
constexpr int get_value() { return 42; }  // 可在编译时或运行时求值
consteval int get_compile_time() { return 42; }  // 必须在编译时求值

int main() {
    constexpr int a = get_value();      // 编译时求值
    int x = get_value();                 // 运行时求值

    constexpr int b = get_compile_time();  // 编译时求值
    // int y = get_compile_time();  // 错误：必须用常量表达式初始化
}
```

## 7. 总结 (Summary)

### 核心要点

1. **关键字总数**：C++23 标准包含约 90+ 个关键字，涵盖数据类型、存储类、控制流、模板、异常等

2. **版本演进**：
   - C++98 奠定了基础关键字集
   - C++11 引入现代特性（`auto`、`nullptr`、`constexpr` 等）
   - C++17 细化语义，移除三字符组
   - C++20 引入协程和概念（`co_await`、`concept`、`requires`）
   - C++23 增强现代特性

3. **替代表示**：提供运算符的字母替代形式，便于缺乏特殊字符的键盘输入

4. **特殊标识符**：`final`、`override`、`import`、`module` 不是关键字，但在特定上下文有特殊含义

### 关键字分类概览

| 类别 | 关键字示例 |
|-----|-----------|
| 基本类型 | `int`, `char`, `float`, `double`, `void`, `bool` |
| 类型修饰 | `signed`, `unsigned`, `short`, `long`, `const`, `volatile` |
| 存储类 | `auto`, `static`, `extern`, `register`, `thread_local`, `mutable` |
| 控制流 | `if`, `else`, `for`, `while`, `do`, `switch`, `break`, `continue`, `return`, `goto` |
| 复合类型 | `class`, `struct`, `union`, `enum` |
| 函数相关 | `inline`, `virtual`, `explicit`, `friend`, `constexpr`, `consteval` |
| 模板 | `template`, `typename`, `concept`, `requires` |
| 异常处理 | `try`, `catch`, `throw`, `noexcept` |
| 类型转换 | `static_cast`, `dynamic_cast`, `const_cast`, `reinterpret_cast` |
| C++11 新增 | `nullptr`, `decltype`, `static_assert`, `alignas`, `alignof` |
| C++20 新增 | `char8_t`, `concept`, `consteval`, `constinit`, `co_await`, `co_return`, `co_yield`, `requires` |
| 替代表示 | `and`, `or`, `not`, `bitand`, `bitor`, `compl`, `xor` 等 |

### 相关概念

- **标识符 (Identifier)** - 变量、函数、类等的名称
- **保留标识符 (Reserved Identifier)** - 不能由程序员定义的标识符
- **预处理器指令 (Preprocessor Directive)** - 编译前处理的指令
- **二字符组 (Digraph)** - 关键字和符号的替代表示
- **概念 (Concept)** - C++20 约束模板参数的机制

### 学习建议

1. 熟悉各 C++ 标准版本引入的新关键字，了解代码的可移植性
2. 理解 `auto` 从存储类说明符到类型推导关键字的语义变化
3. 掌握现代 C++ 特性：`constexpr`、`nullptr`、`decltype` 等
4. 学习 C++20 的协程和概念，了解现代 C++ 编程范式
5. 注意标识符保留规则，避免与标准库和编译器保留名称冲突
6. 使用 `override` 和 `final` 提高代码的可读性和安全性