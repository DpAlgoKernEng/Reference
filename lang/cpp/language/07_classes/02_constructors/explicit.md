# explicit 说明符

## 1. 概述

`explicit` 是 C++ 中的函数说明符（function specifier），用于指定构造函数、转换函数（conversion function，C++11 起）或推导指引（deduction guide，C++17 起）为显式（explicit）的。

被声明为 `explicit` 的函数**不能用于隐式转换和复制初始化**，只能通过直接初始化（direct-initialization）、显式类型转换等方式调用。这一机制帮助开发者避免意外的类型转换，增强类型安全性。

`explicit` 关键字仅可出现在类定义中构造函数或转换函数声明的 decl-specifier-seq 中。

## 2. 来源与演变

### 首次引入

`explicit` 说明符首次在 **C++98** 标准中引入，最初仅用于修饰单参数构造函数，用于防止编译器进行隐式类型转换。

### 历史背景

在 `explicit` 出现之前，C++ 存在以下问题：

1. **隐式转换陷阱**：单参数构造函数会自动成为转换构造函数（converting constructor），可能导致意外的类型转换
2. **接口设计困难**：无法阻止不期望的隐式转换
3. **调试困难**：隐式转换产生的临时对象难以追踪

`explicit` 的引入解决了这些问题，使开发者能够明确控制类型转换行为。

### C++11 变化

- `explicit` 可用于**转换函数**（conversion operator），而不仅限于构造函数
- 显式转换函数在条件上下文（如 `if` 语句）中仍可被隐式调用

### C++17 变化

- `explicit` 可用于**推导指引**（deduction guide）

### C++20 变化

- 新增**条件 explicit** 语法：`explicit(expression)`
- 当常量表达式求值为 `true` 时，函数为显式；否则为非显式
- 功能测试宏：`__cpp_conditional_explicit` = `201806L`

| 版本 | 变更内容 |
|------|----------|
| C++98 | 引入 `explicit` 用于构造函数 |
| C++11 | 扩展到转换函数 |
| C++17 | 扩展到推导指引 |
| C++20 | 增加条件 `explicit(expression)` 语法 |

## 3. 语法与参数

### 基本语法

```cpp
explicit                                          // (1) 基本形式
explicit ( expression )                           // (2) 条件形式 (C++20 起)
```

### 语法说明

| 形式 | 版本 | 说明 |
|------|------|------|
| `explicit` | C++98 | 指定函数为显式，禁止隐式转换 |
| `explicit(expression)` | C++20 | 条件显式，当 `expression` 为 `true` 时为显式 |

### 参数说明

- **expression**：上下文转换为 `bool` 类型的常量表达式（contextually converted constant expression of type bool）

### 适用范围

| 函数类型 | 版本支持 | 说明 |
|----------|----------|------|
| 构造函数 | C++98 起 | 除复制/移动构造函数外的构造函数 |
| 转换函数 | C++11 起 | 用户定义的类型转换运算符 |
| 推导指引 | C++17 起 | 类模板参数推导指引 |

### 限制

- `explicit` 仅能出现在类定义中的声明处
- 复制构造函数和移动构造函数不能声明为 `explicit`（虽然语法上允许，但无实际意义）

### C++20 解析规则变更

在 C++20 中，`explicit` 后跟的 `(` 始终被视为 `explicit` 说明符的一部分：

```cpp
struct S {
    explicit (S)(const S&);    // C++20: 错误，C++17: 正确
    explicit (operator int)(); // C++20: 错误，C++17: 正确
};
```

这种变更避免了与条件 `explicit` 语法的歧义。

## 4. 底层原理

### 隐式转换 vs 显式转换

C++ 中的初始化分为多种形式：

| 初始化形式 | 示例 | 是否考虑 explicit 函数 |
|------------|------|------------------------|
| 直接初始化（direct-initialization） | `T t(arg)` | 是 |
| 直接列表初始化（direct-list-initialization） | `T t{arg}` | 是 |
| 复制初始化（copy-initialization） | `T t = arg` | 否 |
| 复制列表初始化（copy-list-initialization） | `T t = {arg}` | 否 |

### 编译器行为

当编译器遇到类型转换时：

1. **隐式转换场景**（复制初始化、函数参数传递等）：
   - 编译器查找非 `explicit` 的转换构造函数或转换函数
   - `explicit` 函数被排除在候选集之外

2. **显式转换场景**（直接初始化、static_cast 等）：
   - 编译器考虑所有函数，包括 `explicit` 函数

### 转换构造函数（Converting Constructor）

在 C++11 之前，单参数构造函数如果不声明为 `explicit`，则称为转换构造函数：

```cpp
struct A {
    A(int);  // 转换构造函数：int 可隐式转换为 A
};

struct B {
    explicit B(int);  // 显式构造函数：int 不可隐式转换为 B
};
```

C++11 起，所有未声明为 `explicit` 的构造函数（包括多参数构造函数）都是转换构造函数。

### 转换函数与条件上下文

显式转换函数在条件上下文中有特殊处理：

```cpp
struct SmartPtr {
    explicit operator bool() const { return ptr != nullptr; }
};

SmartPtr p;
if (p) { }  // OK: 条件上下文中显式转换函数可被调用
bool b = p; // 错误: 复制初始化不考虑 explicit 转换函数
```

条件上下文包括：
- `if`、`while`、`for` 语句的条件
- 逻辑运算符 `!`、`&&`、`||` 的操作数
- 三元运算符 `?:` 的条件部分
- `static_assert` 的条件

## 5. 使用场景

### 适合使用 explicit 的场景

| 场景 | 原因 |
|------|------|
| 单参数构造函数 | 避免意外的隐式类型转换 |
| 语义不明确的转换 | 强制开发者明确表达转换意图 |
| 资源管理类 | 防止资源意外复制或转换 |
| 强类型包装类 | 保持类型安全性 |

### 不适合使用 explicit 的场景

| 场景 | 原因 |
|------|------|
| 类型本质相同的包装 | 如 `string_view` 从 `string` 的构造 |
| 标准库兼容性 | 某些模板代码依赖隐式转换 |
| 简单值类型转换 | 语义明确的转换可保留隐式性 |

### 最佳实践

1. **默认使用 explicit**：对于单参数构造函数，除非有明确理由允许隐式转换，否则应声明为 `explicit`

2. **代理设计模式**：对于智能指针等代理类，使用显式转换函数提供 `bool` 转换

3. **避免二义性**：当同时存在多个可能的隐式转换路径时，使用 `explicit` 消除二义性

4. **C++20 条件 explicit**：利用条件 `explicit` 实现模板类的条件转换

### 常见陷阱

1. **列表初始化与 explicit**：复制列表初始化不考虑 `explicit` 构造函数，但直接列表初始化会考虑

2. **模板中的 explicit**：模板构造函数也可以是 `explicit`，语义不变

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

struct A {
    A(int) {}              // 转换构造函数
    A(int, int) {}         // 转换构造函数 (C++11 起)
    operator bool() const { return true; }
};

struct B {
    explicit B(int) {}     // 显式构造函数
    explicit B(int, int) {}  // 显式构造函数
    explicit operator bool() const { return true; }
};

int main() {
    // === A 类（非 explicit）===
    A a1 = 1;              // OK: 复制初始化
    A a2(2);               // OK: 直接初始化
    A a3{4, 5};            // OK: 直接列表初始化
    A a4 = {4, 5};         // OK: 复制列表初始化
    A a5 = (A)1;           // OK: 显式转换
    if (a1) { }            // OK: 隐式转换为 bool
    bool na1 = a1;         // OK: 复制初始化使用转换函数

    // === B 类（explicit）===
    // B b1 = 1;           // 错误: 复制初始化不考虑 explicit 构造函数
    B b2(2);               // OK: 直接初始化
    B b3{4, 5};            // OK: 直接列表初始化
    // B b4 = {4, 5};      // 错误: 复制列表初始化不考虑 explicit 构造函数
    B b5 = (B)1;           // OK: 显式转换
    if (b2) { }            // OK: 条件上下文允许 explicit 转换函数
    // bool nb1 = b2;      // 错误: 复制初始化不考虑 explicit 转换函数
    bool nb2 = static_cast<bool>(b2);  // OK: static_cast 使用直接初始化

    return 0;
}
```

### 高级用法：条件 explicit（C++20）

```cpp
#include <iostream>
#include <type_traits>

// 条件 explicit：根据模板参数决定是否为 explicit
template<typename T>
struct Wrapper {
    T value;

    // 当 T 可隐式转换时为 explicit，否则非 explicit
    explicit(!std::is_convertible_v<T, int>)
    Wrapper(T v) : value(std::move(v)) {}
};

// 另一个示例：根据布尔模板参数决定
template<bool IsExplicit>
struct StringWrapper {
    explicit(IsExplicit)
    StringWrapper(const char* str) : data(str) {}

    const char* data;
};

int main() {
    // Wrapper<int>: int 可隐式转换为 int，构造函数为 explicit
    Wrapper<int> w1(42);           // OK: 直接初始化
    // Wrapper<int> w2 = 42;       // 错误: 复制初始化不允许

    // 使用显式指定
    StringWrapper<true> s1("hello");   // 显式版本
    // StringWrapper<true> s2 = "hello";  // 错误

    StringWrapper<false> s3("world");   // 非显式版本
    StringWrapper<false> s4 = "world";  // OK: 允许复制初始化

    return 0;
}
```

### 高级用法：智能指针 bool 转换

```cpp
#include <iostream>

template<typename T>
class SmartPtr {
    T* ptr;

public:
    explicit SmartPtr(T* p = nullptr) : ptr(p) {}

    // 显式 bool 转换：防止意外的算术运算
    explicit operator bool() const {
        return ptr != nullptr;
    }

    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
};

int main() {
    SmartPtr<int> p1(new int(42));
    SmartPtr<int> p2;

    if (p1) {
        std::cout << "p1 is valid: " << *p1 << std::endl;
    }

    // p1 + 1;  // 错误: explicit 转换防止意外的算术运算

    // 这也是为什么 std::unique_ptr 使用 explicit operator bool
    // 防止代码如: if (ptr1 + ptr2) 被编译（两个 bool 相加）

    return 0;
}
```

### 常见错误及修正

#### 错误 1：混淆复制初始化与直接初始化

```cpp
struct Widget {
    explicit Widget(int x) : value(x) {}
    int value;
};

// ❌ 错误：复制初始化不考虑 explicit 构造函数
// Widget w = 42;

// ✅ 修正：使用直接初始化
Widget w(42);       // 正确
Widget w2{42};      // 正确（直接列表初始化）

// ✅ 修正：使用显式转换
Widget w3 = Widget(42);
Widget w4 = static_cast<Widget>(42);
```

#### 错误 2：混淆复制列表初始化与直接列表初始化

```cpp
struct Config {
    explicit Config(int timeout, int retry) : timeout(timeout), retry(retry) {}
    int timeout;
    int retry;
};

// ❌ 错误：复制列表初始化不考虑 explicit 构造函数
// Config cfg = {30, 3};

// ✅ 修正：使用直接列表初始化
Config cfg{30, 3};      // 正确
Config cfg2(30, 3);     // 正确

// 注意区分：
Config c1 = {30, 3};    // 复制列表初始化 - 错误
Config c2{30, 3};       // 直接列表初始化 - 正确
```

#### 错误 3：忽略 explicit 转换函数在非条件上下文的限制

```cpp
struct BoolWrapper {
    explicit operator bool() const { return true; }
};

BoolWrapper bw;

// ✅ 正确：条件上下文
if (bw) { }
while (bw) { break; }

// ❌ 错误：非条件上下文的复制初始化
// bool b = bw;

// ✅ 修正：使用显式转换
bool b = static_cast<bool>(bw);
bool b2 = bw ? true : false;  // 条件运算符的条件部分
```

### 正确用法示例：防止隐式转换的资源类

```cpp
#include <iostream>
#include <string>

class FilePath {
    std::string path;

public:
    // 使用 explicit 防止意外的 string 隐式转换
    explicit FilePath(const std::string& p) : path(p) {}
    explicit FilePath(const char* p) : path(p) {}

    const std::string& get() const { return path; }
};

void openFile(const FilePath& path) {
    std::cout << "Opening: " << path.get() << std::endl;
}

int main() {
    // ✅ 正确：显式构造
    openFile(FilePath("data.txt"));

    // ❌ 错误：防止了意外的隐式转换
    // openFile("data.txt");          // 编译错误
    // openFile(std::string("x"));    // 编译错误

    // 这样设计的好处：API 使用者必须明确表达意图
    FilePath fp1("config.ini");       // 直接初始化
    // FilePath fp2 = "test.txt";    // 错误：阻止意外构造

    return 0;
}
```

## 注意事项

1. **复制/移动构造函数**：虽然可以声明为 `explicit`，但这会导致无法按值传递和返回该类型对象，通常不建议

2. **模板与 explicit**：模板构造函数和模板转换函数也可以是 `explicit`，语义相同

3. **条件 explicit 的表达式**：必须是编译期常量表达式，可使用类型特征（type traits）

4. **C++20 解析变更**：`explicit` 后的 `(` 在 C++20 中始终作为条件 explicit 的一部分解析

## 相关概念

| 概念 | 关系 |
|------|------|
| converting constructor | 未声明为 `explicit` 的构造函数 |
| direct-initialization | 允许使用 `explicit` 构造函数 |
| copy-initialization | 不允许使用 `explicit` 构造函数 |
| list-initialization | 直接/复制列表初始化对 `explicit` 行为不同 |
| user-defined conversion | 用户定义的类型转换函数 |

## 7. 总结

`explicit` 说明符是 C++ 类型安全的重要工具：

| 特性 | 说明 |
|------|------|
| 核心作用 | 防止意外的隐式类型转换 |
| 适用范围 | 构造函数、转换函数、推导指引 |
| 初始化限制 | 禁止复制初始化和复制列表初始化 |
| C++20 增强 | 支持条件 `explicit(expression)` |

**核心建议**：

1. **默认使用 `explicit`**：对于单参数构造函数，除非有明确的隐式转换需求
2. **智能指针模式**：使用 `explicit operator bool()` 提供安全的 bool 转换
3. **条件 explicit**：C++20 中利用条件 explicit 实现灵活的模板设计

**选择指南**：

| 场景 | 建议 |
|------|------|
| 包装类型 | 使用 `explicit` |
| 资源管理类型 | 使用 `explicit` |
| 语义明确的值类型 | 可不使用 `explicit` |
| 需要条件控制 | C++20 使用 `explicit(expression)` |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/explicit
- C++ Standard: [dcl.fct.spec]
- Effective C++, Scott Meyers, Item 27: Minimize casting