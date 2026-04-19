# static_cast 转换

## 1. 概述

`static_cast` 是 C++ 中最常用的显式类型转换（explicit type conversion）运算符之一。它结合了隐式转换（implicit conversion）和用户定义转换（user-defined conversion）的能力，提供了相对安全的类型转换机制。

与 C 风格的类型转换不同，`static_cast` 在编译时进行类型检查，能够检测出明显的类型不兼容错误。它是 C++ 四种新型转换运算符之一（另外三种是 `const_cast`、`dynamic_cast` 和 `reinterpret_cast`），旨在使类型转换的意图更加明确，提高代码的可读性和安全性。

`static_cast` 的主要用途包括：
- 基类与派生类之间的指针或引用转换
- 基本数据类型之间的转换
- 枚举类型与整数类型之间的转换
- `void*` 指针与其他指针类型之间的转换
- 显式调用转换构造函数或转换运算符

## 2. 来源与演变

### 首次引入

`static_cast` 在 **C++98** 标准中首次引入，是 C++ 为解决 C 风格类型转换（C-style cast）安全性问题而设计的新式转换运算符之一。

### 历史背景

在 `static_cast` 出现之前，C++ 继承了 C 语言的类型转换语法：

```cpp
// C 风格转换
int i = 10;
double d = (double)i;        // 旧式转换
double d2 = double(i);        // 函数风格转换
```

C 风格转换的问题：
1. **不够明确**：无法从语法上区分不同类型的转换意图
2. **容易误用**：可能意外移除 const 属性
3. **难以搜索**：在代码中难以定位所有类型转换位置
4. **安全性差**：缺乏编译时检查

C++ 标准委员会设计了四种新型转换运算符来解决这些问题：
- `static_cast`：相关类型之间的转换
- `const_cast`：移除或添加 const/volatile 属性
- `dynamic_cast`：多态类型的安全向下转型
- `reinterpret_cast`：不相关类型的底层重新解释

### 版本演变

#### C++11 更新

1. **右值引用转换**：支持将左值转换为右值引用（xvalue），这是 `std::move` 的实现基础
2. **作用域枚举转换**：支持作用域枚举（scoped enumeration）与整数/浮点类型之间的转换
3. **函数指针转换**：增加了对函数指针转换的支持

#### C++17 更新

1. **指针互转换性规则细化**：明确了指针互转换对象（pointer-interconvertible objects）的概念
2. **void* 转换语义增强**：改进了 `void*` 与其他指针类型之间的转换规则

#### C++20 更新

1. **枚举转换优化**：明确了作用域枚举转换到整数类型时的行为
2. **聚合类型初始化**：支持通过 `static_cast` 进行聚合类型的初始化

#### C++23 更新

1. **浮点类型转换**：明确了不同浮点类型之间的转换规则，特别是处理精度损失的情况

## 3. 语法与参数

### 基本语法

```cpp
static_cast<target-type>(expression)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `target-type` | 目标类型，可以是引用类型、指针类型或值类型 |
| `expression` | 要转换的表达式，可以是左值、右值或表达式 |

### 返回值

返回类型为 `target-type` 的值，具体类别取决于目标类型：
- 如果 `target-type` 是左值引用类型：结果是左值（lvalue）
- 如果 `target-type` 是函数类型的右值引用：结果是左值（C++11 起）
- 如果 `target-type` 是对象类型的右值引用：结果是亡值（xvalue）（C++11 起）
- 其他情况：结果是纯右值（prvalue）

### 限制条件

`static_cast` 不能用于：
- 移除 const、volatile 或 `__unaligned` 属性（需使用 `const_cast`）
- 不相关类型之间的转换（需使用 `reinterpret_cast`）
- 多态类型的不安全向下转型（应使用 `dynamic_cast`）

### 转换规则概览

| 转换类型 | 说明 | 示例 |
|---------|------|------|
| 向下转型 | 基类引用/指针 → 派生类引用/指针 | `static_cast<Derived&>(base_ref)` |
| 左值转亡值 | 左值 → 右值引用 | `static_cast<T&&>(lvalue)` |
| void 转换 | 任意类型 → void | `static_cast<void>(expr)` |
| 初始化转换 | 使用直接初始化语义 | `static_cast<int>(3.14)` |
| 逆向转换 | 标准转换序列的逆操作 | `void* → int*` |
| 枚举转换 | 枚举 ↔ 整数/浮点 | `static_cast<int>(enum_val)` |
| 指针转换 | void* ↔ 其他指针 | `static_cast<int*>(void_ptr)` |
| 成员指针转换 | 派生类成员指针 → 基类成员指针 | `static_cast<int Base::*>(derived_mem_ptr)` |

## 4. 底层原理

### 编译时类型检查

`static_cast` 的所有类型检查都在编译时进行，不会产生运行时开销。编译器会验证转换的合法性，包括：
- 类型是否相关
- cv 限定符（const/volatile）是否兼容
- 类的继承关系是否有效

### 类型转换机制

#### 1. 向下转型（Downcast）

向下转型是将基类指针或引用转换为派生类指针或引用：

```cpp
struct Base { int m; };
struct Derived : Base { int n; };

Base b;
Derived d;
Base& br = d;                        // 向上转型：隐式
Derived& dr = static_cast<Derived&>(br);  // 向下转型：需要显式
```

**关键规则**：
- `Derived` 必须是完整类型（complete type）
- `Base` 必须是 `Derived` 的基类
- 不能通过 `static_cast` 转换虚基类
- cv 限定符不能被"增强"（const 不能转 non-const）

**警告**：向下转型不会进行运行时检查。如果基类引用实际上不指向派生类对象，会导致未定义行为（Undefined Behavior）。

#### 2. 左值转亡值（Lvalue to Xvalue）

C++11 起，可以将左值转换为右值引用，生成亡值（xvalue）：

```cpp
std::vector<int> v = {1, 2, 3};
std::vector<int> v2 = static_cast<std::vector<int>&&>(v);  // 移动语义
// v 被移动后处于有效但未指定的状态
```

这是 `std::move` 的实现原理：

```cpp
template<typename T>
constexpr typename std::remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}
```

#### 3. Void 转换

将任意类型转换为 `void` 时，表达式被丢弃，不产生任何结果：

```cpp
static_cast<void>(some_function());  // 丢弃返回值
```

这等价于以下代码，但更明确地表达了丢弃值的意图：

```cpp
(void)some_function();  // C 风格
some_function();         // 直接调用
```

#### 4. 初始化转换

当目标类型可以通过直接初始化（direct-initialization）构造时，可以使用 `static_cast`：

```cpp
// 等价于 int temp(3.14);
int n = static_cast<int>(3.14);  // n = 3

// 等价于 std::vector<int> temp(10);
std::vector<int> v = static_cast<std::vector<int>>(10);  // v 有 10 个元素
```

#### 5. 逆向标准转换

`static_cast` 可以执行标准转换序列的逆操作：

```cpp
void* pv = &n;                // 隐式转换：int* → void*
int* pi = static_cast<int*>(pv);  // 逆向转换：void* → int*
```

允许的逆向转换：
- 指针转换的逆操作
- 布尔转换的逆操作
- 数值提升的逆操作

不允许的逆向转换：
- 左值到右值转换
- 数组到指针转换
- 函数到指针转换
- 空指针转换
- 空成员指针转换
- 布尔转换（C++17 前）

#### 6. 枚举转换

**作用域枚举 → 整数/浮点**（C++11 起）：

```cpp
enum class Color { Red = 1, Green = 2, Blue = 3 };
Color c = Color::Green;
int n = static_cast<int>(c);  // n = 2
```

**整数/枚举 → 枚举**：

```cpp
enum Color { Red = 1, Green, Blue };
Color c = static_cast<Color>(2);  // c = Green

enum class Scoped { A = 1, B = 2 };
Scoped s = static_cast<Scoped>(2);  // s = Scoped::B
```

**注意**：如果整数值超出枚举值范围，行为未定义。

#### 7. 指针转换

**void* → 其他指针**：

```cpp
int n = 42;
void* pv = &n;
int* pi = static_cast<int*>(pv);  // 合法
double* pd = static_cast<double*>(pv);  // 可能导致对齐问题
```

**对齐要求**：如果指针地址不满足目标类型的对齐要求，结果是未指定的（C++17 前）或未定义行为（C++17 起）。

#### 8. 成员指针转换

基类成员指针可以转换为派生类成员指针：

```cpp
struct Base { int m; };
struct Derived : Base {};

int Base::* pm = &Base::m;           // 基类成员指针
int Derived::* pdm = static_cast<int Derived::*>(pm);  // 转换
```

### 指针互转换性

两个对象 `a` 和 `b` 是**指针互转换的**（pointer-interconvertible），如果：
1. 它们是同一个对象，或
2. 一个是联合体对象，另一个是该对象的非静态数据成员，或
3. 一个是标准布局类对象，另一个是该对象的第一个非静态数据成员或基类子对象，或
4. 存在对象 `c`，使得 `a` 与 `c` 指针互转换，且 `c` 与 `b` 指针互转换

```cpp
union U { int a; double b; } u;
void* x = &u;                         // x 指向 u
double* y = static_cast<double*>(x);  // y 指向 u.b
char* z = static_cast<char*>(x);      // z 指向 u
```

### 性能特征

| 特征 | 说明 |
|------|------|
| 编译时开销 | 编译时类型检查，无运行时开销 |
| 运行时开销 | 基本类型转换：无开销<br>类层次转换：可能需要指针偏移 |
| 内存开销 | 无额外内存占用 |
| 代码大小 | 通常编译为 0-2 条指令 |

## 5. 使用场景

### 适用场景

| 场景 | 示例 | 说明 |
|------|------|------|
| 数值类型转换 | `static_cast<double>(i)` | 显式标记类型转换，避免隐式转换警告 |
| 向下转型（确定安全时） | `static_cast<Derived*>(base_ptr)` | 确定基类指针实际指向派生类时 |
| void* 恢复 | `static_cast<int*>(void_ptr)` | 将 `void*` 恢复为原始指针类型 |
| 移动语义 | `static_cast<T&&>(obj)` | 实现移动语义（通常使用 `std::move`） |
| 作用域枚举转换 | `static_cast<int>(enum_val)` | 作用域枚举需要显式转换 |
| 消除歧义 | `static_cast<SpecificFunc>(overload)` | 函数重载歧义消解 |

### 不适用场景

| 场景 | 推荐替代 | 原因 |
|------|---------|------|
| 移除 const | `const_cast` | `static_cast` 不能移除 const |
| 多态向下转型 | `dynamic_cast` | `dynamic_cast` 有运行时类型检查 |
| 不相关类型转换 | `reinterpret_cast` | 类型不相关时需要底层重新解释 |
| 跨继承层次转换 | `reinterpret_cast` | 无继承关系的类型转换 |

### 最佳实践

#### 1. 优先使用 `static_cast` 而非 C 风格转换

```cpp
// ❌ 不推荐：C 风格转换
double d = (double)i;
Derived* pd = (Derived*)pb;

// ✅ 推荐：显式标记转换类型
double d = static_cast<double>(i);
Derived* pd = static_cast<Derived*>(pb);
```

**理由**：
- 意图更明确，代码更易读
- 编译器可以进行更多检查
- 易于搜索和重构

#### 2. 向下转型使用 `dynamic_cast` 进行安全检查

```cpp
Base* pb = get_object();

// ❌ 危险：无运行时检查
Derived* pd = static_cast<Derived*>(pb);
pd->derived_method();  // 如果 pb 不指向 Derived，未定义行为

// ✅ 安全：使用 dynamic_cast
if (Derived* pd = dynamic_cast<Derived*>(pb)) {
    pd->derived_method();  // 安全执行
} else {
    // 处理转换失败的情况
}
```

#### 3. 使用 `std::move` 而非直接使用 `static_cast`

```cpp
std::vector<int> v1 = {1, 2, 3};

// ❌ 不推荐：直接使用 static_cast
std::vector<int> v2 = static_cast<std::vector<int>&&>(v1);

// ✅ 推荐：使用 std::move
std::vector<int> v2 = std::move(v1);
```

#### 4. 函数重载歧义消解

```cpp
#include <iostream>
#include <algorithm>

void process(int x) { std::cout << "int: " << x << "\n"; }
void process(double x) { std::cout << "double: " << x << "\n"; }

int main() {
    // ❌ 错误：歧义
    // auto f = process;  // 编译错误：不知道选择哪个重载

    // ✅ 正确：显式指定函数类型
    void (*f)(int) = process;  // 选择 int 版本
    f(42);
}
```

#### 5. 作用域枚举与整数互转

```cpp
enum class Status { OK = 0, Error = 1, Unknown = 2 };

Status s = Status::Error;
int n = static_cast<int>(s);  // n = 1

Status s2 = static_cast<Status>(0);  // s2 = Status::OK

// ⚠️ 注意：值必须在枚举范围内
Status s3 = static_cast<Status>(100);  // 未定义行为！
```

### 常见陷阱

#### 陷阱 1：向下转型不安全

```cpp
struct Base { virtual ~Base() = default; };
struct Derived : Base { void foo() {} };
struct Other : Base {};

Base* b = new Other;

// ❌ 危险：编译通过但运行时崩溃
Derived* d = static_cast<Derived*>(b);
d->foo();  // 未定义行为！

// ✅ 安全：使用 dynamic_cast
if (Derived* d = dynamic_cast<Derived*>(b)) {
    d->foo();
}
delete b;
```

#### 陷阱 2：const 不能被移除

```cpp
const int ci = 42;

// ❌ 错误：static_cast 不能移除 const
// int* pi = static_cast<int*>(&ci);  // 编译错误

// ✅ 需要 const_cast（但危险）
int* pi = const_cast<int*>(&ci);  // 编译通过，但修改 ci 是未定义行为
```

#### 陷阱 3：枚举值越界

```cpp
enum class Color { Red = 0, Green = 1, Blue = 2 };

// ❌ 危险：值越界
Color c = static_cast<Color>(100);  // 未定义行为

// ✅ 安全：检查范围
int value = 100;
Color c = (value >= 0 && value <= 2)
    ? static_cast<Color>(value)
    : Color::Red;  // 默认值
```

#### 陷阱 4：void* 转换时的对齐问题

```cpp
// ❌ 危险：对齐问题
char buffer[10];
void* pv = buffer;
int* pi = static_cast<int*>(pv);  // 对齐可能不正确
*pi = 42;  // 可能崩溃！

// ✅ 安全：使用 alignas
alignas(int) char buffer[sizeof(int)];
void* pv = buffer;
int* pi = static_cast<int*>(pv);  // 正确对齐
*pi = 42;  // 安全
```

#### 陷阱 5：转换后的指针生命周期

```cpp
int* pi = new int(42);
void* pv = pi;
delete pi;  // 释放内存

// ❌ 危险：悬空指针
int* pi2 = static_cast<int*>(pv);
*pi2 = 100;  // 未定义行为！
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <vector>

struct Base {
    int m = 42;
    virtual const char* hello() const {
        return "Hello from Base!\n";
    }
    virtual ~Base() = default;
};

struct Derived : Base {
    int n = 100;
    const char* hello() const override {
        return "Hello from Derived!\n";
    }
};

int main() {
    // 1. 向下转型（确定安全的情况）
    Derived d;
    Base& br = d;  // 向上转型：隐式
    std::cout << "1) " << br.hello();

    Derived& dr = static_cast<Derived&>(br);  // 向下转型
    std::cout << "1) " << dr.hello();

    // 2. 数值类型转换
    double pi = 3.14159;
    int n = static_cast<int>(pi);  // n = 3
    std::cout << "2) n = " << n << "\n";

    // 3. void 转换（丢弃值）
    static_cast<void>(n);  // 显式丢弃

    // 4. void* 恢复
    int value = 42;
    void* pv = &value;
    int* pi = static_cast<int*>(pv);
    std::cout << "4) *pi = " << *pi << "\n";

    return 0;
}
```

输出：
```
1) Hello from Base!
1) Hello from Derived!
2) n = 3
4) *pi = 42
```

### 高级用法

#### 移动语义实现

```cpp
#include <iostream>
#include <vector>
#include <utility>

// 简化的 std::move 实现
template<typename T>
constexpr typename std::remove_reference<T>::type&&
my_move(T&& t) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};

    // 使用自定义 move
    std::vector<int> v2 = my_move(v1);

    std::cout << "v1.size() = " << v1.size() << "\n";  // 0 或未指定
    std::cout << "v2.size() = " << v2.size() << "\n";  // 5

    return 0;
}
```

#### 枚举转换

```cpp
#include <iostream>

enum class Status {
    OK = 200,
    NotFound = 404,
    Error = 500
};

// 枚举到整数的工具函数
template<typename E>
constexpr auto to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

int main() {
    Status s = Status::NotFound;

    // 转换为整数
    int code = static_cast<int>(s);
    std::cout << "HTTP status: " << code << "\n";  // 404

    // 使用工具函数
    std::cout << "HTTP status: " << to_underlying(s) << "\n";  // 404

    // 从整数转换（需确保值合法）
    Status s2 = static_cast<Status>(200);  // OK
    std::cout << "s2 is OK: " << (s2 == Status::OK) << "\n";

    return 0;
}
```

#### 多态类型的安全向下转型

```cpp
#include <iostream>
#include <memory>

struct Shape {
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

struct Circle : Shape {
    double radius;
    Circle(double r) : radius(r) {}
    void draw() const override {
        std::cout << "Drawing circle with radius " << radius << "\n";
    }
    void setRadius(double r) { radius = r; }
};

struct Square : Shape {
    double side;
    Square(double s) : side(s) {}
    void draw() const override {
        std::cout << "Drawing square with side " << side << "\n";
    }
};

int main() {
    std::unique_ptr<Shape> shape = std::make_unique<Circle>(5.0);

    // ❌ 危险：使用 static_cast（无运行时检查）
    // Circle* c = static_cast<Circle*>(shape.get());

    // ✅ 安全：使用 dynamic_cast
    if (Circle* circle = dynamic_cast<Circle*>(shape.get())) {
        circle->setRadius(10.0);  // 安全调用
        circle->draw();
    } else if (Square* square = dynamic_cast<Square*>(shape.get())) {
        square->draw();
    }

    // 当确定类型时，static_cast 效率更高
    Shape* s = new Circle(3.0);
    Circle* c = static_cast<Circle*>(s);  // 如果确定是 Circle
    c->draw();
    delete s;

    return 0;
}
```

### 常见错误及修正

#### 错误 1：不安全的向下转型

```cpp
#include <iostream>

struct Base {
    int m = 10;
    virtual ~Base() = default;
};
struct Derived : Base {
    int n = 20;
    void foo() { std::cout << "Derived::foo()\n"; }
};
struct Other : Base {
    int x = 30;
};

int main() {
    Base* b = new Other;

    // ❌ 错误：编译通过但运行时崩溃
    Derived* d = static_cast<Derived*>(b);
    d->foo();  // 未定义行为！

    // ✅ 修正：使用 dynamic_cast
    if (Derived* d = dynamic_cast<Derived*>(b)) {
        d->foo();
    } else {
        std::cout << "Not a Derived object\n";
    }

    delete b;
    return 0;
}
```

#### 错误 2：试图移除 const

```cpp
#include <iostream>

void modify(int* p) {
    *p = 100;
}

int main() {
    const int ci = 42;

    // ❌ 错误：static_cast 不能移除 const
    // modify(static_cast<int*>(&ci));  // 编译错误

    // ❌ 危险：使用 const_cast 移除 const
    modify(const_cast<int*>(&ci));  // 编译通过，但修改 const 对象是 UB
    std::cout << ci << "\n";  // 可能输出 42 或 100，未定义

    // ✅ 正确：不要修改 const 对象
    int mutable_var = 42;
    modify(&mutable_var);  // 安全

    return 0;
}
```

#### 错误 3：枚举值越界

```cpp
#include <iostream>

enum class Permission { Read = 1, Write = 2, Execute = 4 };

int main() {
    // ❌ 错误：枚举值越界
    Permission p = static_cast<Permission>(100);  // 未定义行为

    // ✅ 修正：检查值范围
    int value = 100;
    Permission p2 = (value >= 1 && value <= 7 && (value & (value - 1)) == 0)
        ? static_cast<Permission>(value)
        : Permission::Read;  // 使用默认值

    std::cout << static_cast<int>(p2) << "\n";

    return 0;
}
```

#### 错误 4：转换后的对象切片

```cpp
#include <iostream>

struct Base {
    int m = 10;
    virtual void foo() { std::cout << "Base\n"; }
};
struct Derived : Base {
    int n = 20;
    void foo() override { std::cout << "Derived\n"; }
};

int main() {
    Derived d;
    Base& br = d;
    br.foo();  // 输出 "Derived"（多态）

    // ❌ 错误：对象切片
    Base b = static_cast<Base>(d);  // 切片！丢失 Derived 部分
    b.foo();  // 输出 "Base"（不再多态）

    // ✅ 正确：使用指针或引用
    Base& br2 = d;  // 不切片
    br2.foo();  // 输出 "Derived"

    return 0;
}
```

## 7. 总结

`static_cast` 是 C++ 中最重要的类型转换工具之一，它提供了编译时类型检查，确保类型转换的安全性。以下是核心要点：

### 核心要点

| 要点 | 说明 |
|------|------|
| 编译时检查 | 所有类型检查在编译时完成，无运行时开销 |
| 相关类型转换 | 只能用于相关类型之间的转换 |
| 不能移除 const | 需要 `const_cast` |
| 无运行时检查 | 向下转型不进行类型验证，需自行确保安全 |

### 与其他转换运算符的对比

| 转换类型 | 用途 | 安全性 | 运行时检查 |
|---------|------|--------|-----------|
| `static_cast` | 相关类型转换 | 中等 | 无 |
| `dynamic_cast` | 多态向下转型 | 高 | 有 |
| `const_cast` | 移除/添加 const | 低 | 无 |
| `reinterpret_cast` | 底层重新解释 | 最低 | 无 |
| C 风格转换 | 所有上述转换的组合 | 最低 | 无 |

### 使用建议

1. **优先使用 `static_cast`**：比 C 风格转换更安全、更明确
2. **向下转型使用 `dynamic_cast`**：当类型不确定时，使用 `dynamic_cast` 进行运行时检查
3. **移动语义使用 `std::move`**：不要直接使用 `static_cast<T&&>`
4. **避免移除 const**：修改 const 对象是未定义行为
5. **枚举转换检查范围**：确保转换的整数值在枚举值范围内

### 学习建议

1. 理解四种新型转换运算符的区别和适用场景
2. 掌握对象模型和继承层次中指针转换的内存布局
3. 实践多态类型的向下转型，比较 `static_cast` 和 `dynamic_cast`
4. 学习移动语义和 `std::move` 的实现原理
5. 了解 `void*` 转换的内存对齐要求

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 7.6.1.9 Static cast [expr.static.cast]
- C++20 标准 (ISO/IEC 14882:2020): 7.6.1.8 Static cast [expr.static.cast]
- C++17 标准 (ISO/IEC 14882:2017): 8.2.9 Static cast [expr.static.cast]
- cppreference.com: https://en.cppreference.com/w/cpp/language/static_cast
- 《Effective C++》, Scott Meyers, Item 27: Minimize casting
- 《More Effective C++》, Scott Meyers, Item 2: Prefer C++-style casts