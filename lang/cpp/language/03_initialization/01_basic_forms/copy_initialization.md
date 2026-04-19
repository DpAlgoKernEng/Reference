# 复制初始化（Copy-initialization）

## 1. 概述

**复制初始化（Copy-initialization）** 是 C++ 中一种对象初始化方式，用于从另一个对象初始化新对象。其语法特点是在声明变量时使用等号（`=`）连接类型和初始值。

复制初始化的核心特征：
- 使用 `=` 语法进行初始化
- 不考虑 `explicit` 构造函数
- 可能调用复制构造函数或移动构造函数
- 与直接初始化（direct-initialization）相比限制更多

## 2. 来源与演变

### 历史背景

复制初始化的概念源自 C 语言的对象初始化方式。在 C++ 中，复制初始化被赋予了更丰富的语义，涉及构造函数选择、类型转换等机制。

### C++11 变化

- 移动语义引入后，复制初始化可能调用移动构造函数
- 花括号初始化器 `T object = {other}` 被重新分类为列表初始化（list initialization）
- 禁止窄化转换（narrowing conversion）

### C++17 变化

- 引入**复制省略（copy elision）** 保证：当初始化器是同类型的纯右值（prvalue）时，直接使用该表达式初始化目标对象，不创建临时对象

### 缺陷报告修正

| DR | 适用版本 | 原始行为 | 修正行为 |
|----|---------|---------|---------|
| CWG 5 | C++98 | 转换构造函数创建的临时对象继承目标类型的 cv 限定 | 临时对象不具有 cv 限定 |
| CWG 177 | C++98 | 复制初始化类对象时创建的临时对象的值类别未指定 | 明确指定为右值（rvalue） |

## 3. 语法与参数

### 语法形式

| 语法 | 编号 | 说明 |
|------|------|------|
| `T object = other;` | (1) | 命名变量声明，使用等号初始化 |
| `T object = {other};` | (2) | (C++11 前) 标量类型的花括号初始化 |
| `f(other)` | (3) | 函数参数按值传递 |
| `return other;` | (4) | 函数按值返回 |
| `throw object;` / `catch (T object)` | (5) | 异常抛出或捕获 |
| `T array[N] = {other-sequence};` | (6) | 聚合初始化中的元素初始化 |

### 触发场景详解

1. **命名变量声明**：当非引用类型的命名变量（自动、静态或线程局部存储期）使用等号后跟表达式的初始化器声明时

2. **花括号初始化**（C++11 前）：标量类型使用等号后跟花括号括起的表达式声明时。注意：C++11 起此为列表初始化，不允许窄化转换

3. **函数参数传递**：按值向函数传递参数时

4. **函数返回**：从按值返回的函数返回时

5. **异常处理**：按值抛出或捕获异常时

6. **聚合初始化**：作为聚合初始化的一部分，初始化每个提供了初始化器的元素

## 4. 底层原理

### 初始化过程

复制初始化的执行过程遵循以下规则：

#### 规则 1：同类纯右值优化（C++17 起）

如果 `T` 是类类型，且初始化器是与 `T` 同类型的纯右值（prvalue），则直接使用该表达式初始化目标对象，不进行临时对象的具体化。这被称为**复制省略**。

#### 规则 2：同类或派生类初始化

如果 `T` 是类类型，且初始化器的类型（去除 cv 限定后）是 `T` 或 `T` 的派生类：
- 考察 `T` 的非显式（non-explicit）构造函数
- 通过重载决议选择最佳匹配
- 调用选中的构造函数初始化对象

#### 规则 3：用户定义转换

如果 `T` 是类类型但初始化器类型不是 `T` 或其派生类，或 `T` 是非类类型但初始化器是类类型：
- 考察可将初始化器类型转换为 `T` 的用户定义转换序列
- 通过重载决议选择最佳转换
- 转换结果用于直接初始化目标对象
- C++17 前：转换结果作为临时对象，需要可访问的复制/移动构造函数（即使被优化掉）
- C++17 起：转换结果是纯右值表达式

#### 规则 4：标准转换

如果 `T` 和初始化器都不是类类型：
- 使用标准转换将初始化器的值转换为 `T` 的无 cv 限定版本

### 与直接初始化的区别

| 特性 | 复制初始化 | 直接初始化 |
|------|-----------|-----------|
| 语法 | `T obj = other` | `T obj(other)` 或 `T obj{other}` |
| explicit 构造函数 | 不考虑 | 可使用 |
| 隐式转换 | 必须直接转换到目标类型 | 可转换到构造函数参数类型 |

### 等号的语义

复制初始化中的等号 `=` 与赋值运算符无关：
- 它不是赋值操作
- 重载赋值运算符不影响复制初始化
- 它仅表示"从...初始化"的语义

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 函数参数传递 | 按值传递参数时隐式使用 |
| 函数返回值 | 按值返回时隐式使用 |
| 异常处理 | 按值抛出/捕获异常 |
| 变量初始化 | 使用等号语法的显式初始化 |
| 聚合初始化 | 数组元素的初始化 |

### 最佳实践

1. **理解隐式转换的限制**
   - 复制初始化要求初始化器能直接转换为目标类型
   - 不会进行多步隐式转换

2. **区分复制初始化与赋值**
   - 复制初始化发生在对象构造时
   - 赋值发生在已构造对象上

3. **利用复制省略**
   - 现代 C++ 编译器会优化复制操作
   - C++17 起某些情况强制省略复制

### 常见陷阱

#### explicit 构造函数限制

```cpp
struct Exp { explicit Exp(const char*) {} }; // 不可从 const char* 隐式转换

Exp e1("abc");   // OK：直接初始化
Exp e2 = "abc";  // 错误：复制初始化不考虑 explicit 构造函数
```

#### 多步转换限制

```cpp
struct S { S(std::string) {} }; // 可从 std::string 隐式构造

S s("abc");    // OK：直接初始化，const char[4] -> std::string -> S
S s = "abc";   // 错误：复制初始化，无法从 const char[4] 直接转换到 S
S s = "abc"s;  // OK：复制初始化，std::string -> S
```

## 6. 代码示例

### 基础用法

```cpp
#include <string>
#include <memory>

int main() {
    // 基本类型复制初始化
    int n = 3.14;         // 浮点-整型转换
    const int b = n;      // const 不影响复制初始化
    int c = b;            // 可以从 const int 复制

    // 类类型复制初始化
    std::string s = "test";  // OK：构造函数非 explicit

    // 移动语义与复制初始化
    std::string s2 = std::move(s);  // 这仍然是"复制初始化"，但调用移动构造函数

    return 0;
}
```

### explicit 与非 explicit 构造函数对比

```cpp
#include <memory>

struct Exp {
    explicit Exp(const char*) {}
};

struct Imp {
    Imp(const char*) {}
};

int main() {
    // explicit 构造函数
    Exp e1("abc");    // OK：直接初始化
    // Exp e2 = "abc"; // 错误：复制初始化不考虑 explicit 构造函数

    // 非 explicit 构造函数
    Imp i1("abc");    // OK：直接初始化
    Imp i2 = "abc";   // OK：复制初始化可以使用非 explicit 构造函数

    // 实际例子：unique_ptr
    // std::unique_ptr<int> p = new int(1);  // 错误：构造函数是 explicit
    std::unique_ptr<int> p(new int(1));     // OK：直接初始化

    return 0;
}
```

### 用户定义转换与复制初始化

```cpp
#include <iostream>

struct A {
    operator int() { return 12; }  // 转换运算符
};

struct B {
    B(int) {}  // 从 int 构造
};

int main() {
    A a;

    // 多步转换的限制
    // B b1 = a;    // 错误：无法从 A 直接转换到 B

    // 显式多步转换
    B b2{a};        // OK：直接初始化，调用 A::operator int()，然后 B::B(int)
    B b3 = {a};     // OK：列表初始化（C++11 起）
    auto b4 = B{a}; // OK：auto 推导 + 列表初始化

    // 注意：这不是赋值
    // b0 = a;      // 错误：需要重载赋值运算符

    return 0;
}
```

### 常见错误及修正

#### 错误 1：复制初始化与 explicit 构造函数

```cpp
// 错误示例
struct Widget {
    explicit Widget(int x) : value(x) {}
    int value;
};

// Widget w = 42;  // 编译错误：explicit 构造函数

// 修正方法 1：使用直接初始化
Widget w1(42);      // OK

// 修正方法 2：使用列表初始化（C++11）
Widget w2{42};      // OK

// 修正方法 3：移除 explicit 说明符（如果允许隐式转换是期望行为）
struct Widget2 {
    Widget2(int x) : value(x) {}  // 非 explicit
    int value;
};
Widget2 w3 = 42;    // OK
```

#### 错误 2：多步隐式转换

```cpp
#include <string>

struct Parser {
    Parser(std::string s) : data(std::move(s)) {}
    std::string data;
};

// 错误示例
// Parser p = "hello";  // 编译错误：const char* -> std::string -> Parser 是两步转换

// 修正方法 1：显式转换
Parser p1 = std::string("hello");  // OK：单步转换

// 修正方法 2：使用字符串字面量后缀
using namespace std::string_literals;
Parser p2 = "hello"s;  // OK：已经是 std::string

// 修正方法 3：添加接受 const char* 的构造函数
struct Parser2 {
    Parser2(std::string s) : data(std::move(s)) {}
    Parser2(const char* s) : data(s) {}  // 添加此构造函数
    std::string data;
};
Parser2 p3 = "hello";  // OK
```

#### 错误 3：混淆复制初始化与赋值

```cpp
#include <iostream>

class Counter {
public:
    Counter(int v) : value(v) {
        std::cout << "构造: " << value << std::endl;
    }
    Counter(const Counter& other) : value(other.value) {
        std::cout << "复制构造: " << value << std::endl;
    }
    Counter& operator=(const Counter& other) {
        value = other.value;
        std::cout << "赋值: " << value << std::endl;
        return *this;
    }

private:
    int value;
};

int main() {
    Counter c1 = 5;  // 复制初始化：输出"构造: 5"（可能被优化）
    // 注意：这不是赋值！不调用 operator=

    Counter c2(10);  // 直接初始化：输出"构造: 10"
    c2 = c1;         // 赋值：输出"赋值: 5"

    return 0;
}
```

## 7. 总结

### 核心要点

| 概念 | 说明 |
|------|------|
| 触发场景 | 变量声明、函数参数传递、返回值、异常处理、聚合初始化 |
| 语法特征 | 使用 `=` 进行初始化 |
| 构造函数限制 | 不考虑 explicit 构造函数 |
| 转换限制 | 必须能直接转换到目标类型 |
| 与赋值区别 | 初始化不是赋值，不调用赋值运算符 |

### 复制初始化 vs 直接初始化

```cpp
struct T { T(int); explicit T(double); };

T t1 = 42;    // 复制初始化：OK，使用 T(int)
T t2(42);     // 直接初始化：OK，使用 T(int)
T t3 = 3.14;  // 复制初始化：错误，T(double) 是 explicit
T t4(3.14);   // 直接初始化：OK，使用 T(double)
```

### 学习建议

1. **理解隐式转换的限制**：复制初始化要求单步隐式转换，这是设计 `explicit` 的核心目的

2. **区分初始化与赋值**：复制初始化发生在对象构造时，与赋值运算符无关

3. **善用现代 C++ 特性**：
   - C++11 的列表初始化可替代部分复制初始化场景
   - C++17 的复制省略保证可优化性能

4. **设计 API 时考虑 explicit**：
   - 单参数构造函数通常应声明为 `explicit` 以避免意外转换
   - 允许隐式转换的构造函数应明确语义（如 `std::string(const char*)`）

## 相关概念

| 概念 | 关系 |
|------|------|
| 直接初始化（direct-initialization） | 更灵活的初始化方式，可使用 explicit 构造函数 |
| 列表初始化（list-initialization） | C++11 引入，使用花括号 |
| 复制省略（copy elision） | 编译器优化，避免不必要的复制 |
| 转换构造函数（converting constructor） | 非 explicit 构造函数，可被复制初始化使用 |
| 复制构造函数（copy constructor） | 用于复制对象的特殊构造函数 |
| 移动构造函数（move constructor） | 用于移动语义的特殊构造函数 |
| `explicit` 说明符 | 阻止隐式转换的关键字 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/copy_initialization
- C++ Standard: [dcl.init]
- The C++ Programming Language, Bjarne Stroustrup