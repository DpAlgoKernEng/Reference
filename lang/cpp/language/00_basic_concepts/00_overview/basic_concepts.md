# C++ 基础概念 (Basic Concepts)

## 1. 概述 (Overview)

C++ 基础概念定义了描述 C++ 编程语言时所使用的特定术语和核心概念。理解这些基础概念是掌握 C++ 语言的关键，它们构成了整个语言体系的基石。

### 核心概念

C++ 程序本质上是一系列文本文件的集合（通常是头文件和源文件），这些文件包含**声明（Declarations）**，经过**翻译（Translation）**过程转换为可执行程序。程序的执行始于 C++ 实现调用其 `main` 函数。

### C++ 与 C 的关键区别

相比 C 语言，C++ 引入了更多概念：

| 特性 | C 语言 | C++ |
|------|--------|-----|
| 实体类型 | 对象、函数、类型 | 增加引用、类成员、模板、命名空间等 |
| 预处理 | 宏是主要扩展机制 | 宏不是 C++ 实体，推荐使用模板和内联函数 |
| 名称查找 | 简单的作用域规则 | 复杂的名称查找（Argument-Dependent Lookup）|
| 类型系统 | 基本类型 + 派生类型 | 增加类、模板、引用等复杂类型 |

### 关键术语

| 术语 | 英文原文 | 说明 |
|------|----------|------|
| 关键字 | Keywords | 具有特殊含义的保留字 |
| 标识符 | Identifiers | 用于命名实体 |
| 实体 | Entities | 程序中的各种构成元素（值、对象、引用等）|
| 声明 | Declarations | 引入实体、关联名称、定义属性 |
| 定义 | Definitions | 定义使用实体所需的所有属性 |
| 作用域 | Scope | 名称有效的程序区域 |
| 链接属性 | Linkage | 使名称在不同作用域或翻译单元中引用同一实体 |
| 类型 | Types | 描述对象、引用、函数和表达式的属性 |

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 由比雅尼·斯特劳斯特鲁普（Bjarne Stroustrup）在 1979 年开始开发，最初名为 "C with Classes"。它在 C 语言的基础上增加了面向对象编程特性，同时保持与 C 的兼容性。

### 设计动机

C++ 概念体系的设计目标包括：

1. **零开销原则**：不用的东西就不付出代价，用了的东西编译器会生成最优代码
2. **直接映射到硬件**：保持 C 语言的底层控制能力
3. **抽象机制**：提供类、模板等高级抽象，且没有性能损失
4. **多范式支持**：过程式、面向对象、泛型、函数式编程

### 标准化进程

| 标准版本 | 年份 | 主要概念变化 |
|----------|------|--------------|
| C++98 | 1998 | 首个 ISO 标准，确立了核心概念体系 |
| C++03 | 2003 | 缺陷修复版本，概念体系基本不变 |
| C++11 | 2011 | **重大更新**：引入 `auto`、lambda、右值引用、智能指针、结构化绑定基础 |
| C++14 | 2014 | 泛型 lambda、`auto` 返回类型推导、变量模板 |
| C++17 | 2017 | 结构化绑定（Structured Bindings）、折叠表达式、`if constexpr`、内联变量 |
| C++20 | 2020 | **重大更新**：概念（Concepts）、范围（Ranges）、协程（Coroutines）、模块（Modules）|
| C++23 | 2023 | `auto` 参数、`this` 推导、`std::optional`  monadic 操作等 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 程序结构

C++ 程序是由文本文件（通常是头文件 `.h`/`.hpp` 和源文件 `.cpp`）组成的序列，包含**声明**，经过翻译成为可执行程序。

```cpp
// 程序示例：基本的 C++ 程序结构
#include <iostream>  // 头文件包含（预处理器指令）
#include <string>

// 命名空间声明
namespace mylib {

// 类定义（用户定义类型）
class Person {
public:
    // 构造函数
    Person(const std::string& name, int age)
        : name_(name), age_(age) {}

    // 成员函数
    void greet() const {
        std::cout << "Hello, I'm " << name_ << std::endl;
    }

private:
    std::string name_;  // 成员变量
    int age_;
};

// 函数模板
template<typename T>
T add(T a, T b) {
    return a + b;
}

}  // namespace mylib

// main 函数 - 程序入口点
int main() {
    // 使用类
    mylib::Person person("Alice", 30);
    person.greet();

    // 使用函数模板
    int sum = mylib::add(5, 3);
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
```

### 关键字 (Keywords)

C++ 中的某些单词具有特殊含义，称为**关键字（Keywords）**，不能用作标识符。

**C++98/03 关键字：**
```
asm        auto      bool       break     case
catch      char      class      const     const_cast
continue   default   delete     do        double
dynamic_cast  else   enum       explicit  export
extern     false     float      for       friend
goto       if        inline     int       long
mutable    namespace new        operator  private
protected  public    register   reinterpret_cast  return
short      signed    sizeof     static    static_cast
struct     switch    template   this      throw
true       try       typedef    typeid    typename
union      unsigned  using      virtual   void
volatile   wchar_t   while
```

**C++11 新增：**
```
alignas    alignof   char16_t   char32_t  constexpr
decltype   noexcept  nullptr    static_assert  thread_local
```

**C++20 新增：**
```
concept    consteval constinit  co_await  co_return
co_yield   requires
```

### 实体 (Entities)

C++ 程序的**实体（Entities）**包括：

| 实体类型 | 说明 | 引入方式 |
|----------|------|----------|
| 值（Values） | 计算的结果 | 表达式求值 |
| 对象（Objects） | 存储数据的内存区域 | 变量定义 |
| 引用（References） | 对象的别名 | 引用声明 |
| 结构化绑定（Structured Bindings）| 解构对象的绑定（C++17起）| `auto [a, b] = ...` |
| 函数（Functions） | 可执行代码块 | 函数定义 |
| 枚举器（Enumerators） | 枚举类型的命名常量 | 枚举定义 |
| 类型（Types） | 描述数据的属性 | 类型定义（typedef/using）|
| 类成员（Class Members） | 类的成员变量和函数 | 类定义 |
| 模板（Templates） | 参数化类型或函数 | 模板定义 |
| 模板特化（Template Specializations）| 模板的特定实例 | 显式/部分特化 |
| 包（Packs） | 可变参数模板参数（C++11起）| 模板参数包 |
| 命名空间（Namespaces） | 作用域容器 | 命名空间定义 |

**注意**：预处理器宏（Preprocessor Macros）**不是** C++ 实体。

```cpp
// 实体示例
namespace example {  // 命名空间实体

// 类定义 - 引入类型实体和类成员实体
class MyClass {
public:
    int member_var;           // 类成员（非静态数据成员）
    void member_func();       // 类成员（成员函数）
    static int static_var;    // 类成员（静态数据成员）
};

// 函数定义 - 函数实体
void global_func() {}

// 变量定义 - 对象实体
int global_var = 10;

// 引用 - 引用实体
int& ref = global_var;

// 枚举定义 - 类型实体和枚举器实体
enum Color { Red, Green, Blue };  // Red, Green, Blue 是枚举器

// 模板定义 - 模板实体
template<typename T>
T template_func(T val) { return val; }

// 别名声明 - 类型实体
template<typename T>
using Vec = std::vector<T>;

}  // namespace example

// C++17 结构化绑定
std::pair<int, double> get_data() { return {1, 2.0}; }

void demo() {
    auto [x, y] = get_data();  // x 和 y 是结构化绑定实体
}
```

### 声明与定义 (Declarations and Definitions)

**声明（Declarations）**可以：
- 引入实体
- 将实体与名称关联
- 定义实体的属性

**定义（Definitions）**是定义使用实体所需的所有属性的声明。

```cpp
// 声明 vs 定义示例

// 变量声明（非定义）- 使用 extern
extern int global_var;      // 仅声明，告诉编译器在其他地方定义

// 变量定义
int global_var = 10;        // 定义，分配存储空间并初始化
int another_var;            // 定义（默认初始化）

// 函数声明（函数原型）
void func(int x);           // 仅声明

// 函数定义
void func(int x) {          // 定义，包含函数体
    // ...
}

// 类声明（前向声明）
class MyClass;              // 仅声明，不完整的类型

// 类定义
class MyClass {             // 定义，完整的类型
public:
    void method();
    int data;
};

// ODR（One Definition Rule）示例
// 程序只能有一个非内联函数或变量的定义

// 头文件中的内联函数 - 可以在多个翻译单元中定义
inline int add(int a, int b) {
    return a + b;
}

// C++17 内联变量 - 可以在多个翻译单元中定义
inline int counter = 0;
```

### 作用域 (Scope)

每个名称仅在程序的特定部分有效，这个区域称为其**作用域（Scope）**。

| 作用域类型 | 描述 | 示例 |
|------------|------|------|
| 块作用域（Block Scope） | 在代码块 `{}` 内声明 | 局部变量 |
| 类作用域（Class Scope） | 在类定义内 | 类成员 |
| 枚举作用域（Enum Scope） | 在枚举定义内（C++11起为强枚举）| 枚举器 |
| 命名空间作用域（Namespace Scope） | 在命名空间内 | 命名空间成员 |
| 文件作用域/全局作用域 | 在所有代码块和命名空间之外 | 全局变量 |
| 函数作用域（Function Scope） | 仅在函数体内 | 标签（label） |

```cpp
// 作用域示例
namespace outer {           // 命名空间作用域开始
    int ns_var = 10;        // 命名空间作用域

    class MyClass {         // 类作用域开始
    public:
        int member;         // 类作用域
        static int static_member;  // 类作用域

        void method() {     // 函数定义在类内，但方法体是块作用域
            int local = 5;  // 块作用域
            member = local; // 可以访问类成员
        }
    };                      // 类作用域结束

    enum class Color {      // C++11 强枚举，有自己的作用域
        Red, Green, Blue    // 枚举作用域
    };
}                           // 命名空间作用域结束

void scope_demo() {
    int x = 1;              // 块作用域（外层）
    {
        int x = 2;          // 块作用域（内层），隐藏外层的 x
        outer::ns_var = x;  // 访问命名空间作用域变量
    }
    // 这里的 x 仍然是 1

    outer::MyClass obj;
    obj.member = 10;        // 通过对象访问类作用域成员

    outer::Color c = outer::Color::Red;  // 强枚举需要作用域限定
}
```

### 名称查找 (Name Lookup)

程序中遇到的名称通过与引入它们的声明关联来解析，这个过程称为**名称查找（Name Lookup）**。

```cpp
// 名称查找示例
namespace ns {
    void func(int);     // #1
    void func(double);  // #2
}

void func(char);        // #3

void example() {
    using ns::func;     // using 声明引入 ns::func

    func(1);            // 调用 ns::func(int) #1
    func(1.0);          // 调用 ns::func(double) #2
    func('a');          // 调用 func(char) #3
}

// 参数依赖查找（ADL，Argument-Dependent Lookup）
namespace mylib {
    class MyType {};
    void process(MyType t) {}  // 定义在命名空间内
}

void adl_demo() {
    mylib::MyType obj;
    process(obj);       // ADL：编译器也会在 mylib 命名空间查找 process
}
```

### 链接属性 (Linkage)

**链接（Linkage）**使名称在不同作用域或翻译单元中引用相同的实体。

| 链接类型 | 说明 | 关键字 |
|----------|------|--------|
| 外部链接（External Linkage） | 可在不同翻译单元中访问 | 默认（全局命名空间）或 `extern` |
| 内部链接（Internal Linkage） | 仅在当前翻译单元内访问 | `static`、`const`（C++中）、匿名命名空间 |
| 无链接（No Linkage） | 仅在声明的作用域内访问 | 局部变量、函数参数、类成员 |
| 模块链接（Module Linkage）| 仅在模块内访问（C++20起）| 模块接口/实现单元 |

```cpp
// 链接属性示例

// 外部链接
int ext_var = 10;               // 外部链接
void ext_func() {}              // 外部链接
extern const int ext_const = 5; // 显式 extern，外部链接

// 内部链接
static int int_var = 20;        // static - 内部链接
static void int_func() {}       // static - 内部链接
const int int_const = 30;       // const 默认内部链接（在 C++ 中）
constexpr int ce_var = 40;      // constexpr 默认内部链接

// 匿名命名空间 - C++ 推荐方式，所有成员内部链接
namespace {
    int anon_var = 50;
    void anon_func() {}
}

// 无链接
void func() {
    int local = 60;             // 无链接
    static int static_local;    // 无链接（但静态存储期）
}

class MyClass {
    int member;                 // 无链接（类成员）
    static int static_member;   // 外部链接（如果是定义）或内部链接
};
```

### 类型系统 (Type System)

每个对象、引用、函数、表达式都与一个**类型（Type）**相关联。

**类型分类：**

| 类型类别 | 具体类型 | 示例 |
|----------|----------|------|
| 基本类型（Fundamental） | `void`、`bool`、`char`、`int`、`float`、`double` 等 | `int x;` |
| 复合类型（Compound） | 数组、函数、指针、引用、成员指针 | `int* p;` |
| 用户定义类型（User-defined） | 类、结构体、联合体、枚举 | `class C {};` |
| 完整类型（Complete） | 已定义所有成员的类型 | `struct S { int x; };` |
| 不完整类型（Incomplete） | 仅声明未定义的类型 | `struct S;` |
| CV 限定类型（CV-qualified） | `const`、`volatile` 限定的类型 | `const int x;` |

```cpp
// 类型示例
// 基本类型
int i = 10;
double d = 3.14;
bool flag = true;

// 复合类型
int arr[10];                    // 数组类型
int* ptr = &i;                  // 指针类型
int& ref = i;                   // 引用类型
void func();                    // 函数类型
int (*func_ptr)();              // 函数指针类型

// 用户定义类型
struct Point { int x, y; };     // 结构体类型
class Vector {                  // 类类型
public:
    double data[3];
};
union Data { int i; float f; }; // 联合体类型
enum Status { OK, Error };      // 枚举类型
enum class StrongEnum { A, B }; // 强枚举类型（C++11）

// CV 限定
const int ci = 5;               // const 限定
volatile int vi;                // volatile 限定
const volatile int cvi = 0;     // const + volatile

// 不完整类型
struct Incomplete;              // 不完整类型（仅声明）
extern Incomplete* p;           // 可以使用指针
// Incomplete obj;              // 错误：不能创建对象
```

### 变量 (Variables)

声明的对象和声明的引用，如果**不是**非静态数据成员，则称为**变量（Variables）**。

```cpp
// 变量示例
class MyClass {
    int member;         // 非静态数据成员，不是变量
    static int static_member;  // 静态数据成员，是变量
};

int MyClass::static_member = 0;  // 定义

void func() {
    int local = 10;     // 局部变量
    int& ref = local;   // 引用变量（绑定到 local）
    static int s_local; // 静态局部变量

    extern int ext;     // 声明外部变量
}

int global = 20;        // 全局变量
int& global_ref = global;  // 全局引用变量
const int const_var = 30;   // const 变量
```

---

## 4. 底层原理 (Underlying Principles)

### 翻译模型

C++ 程序从源代码到可执行文件的**翻译（Translation）**过程包含多个阶段：

```
源文件(.cpp) → 预处理 → 编译 → 汇编 → 链接 → 可执行文件
                  ↓        ↓       ↓       ↓
               宏展开   语法分析  目标文件  可执行程序
               头包含   语义分析  (.o/.obj) 库链接
               条件编译 代码生成  符号表
```

**预处理器**在翻译阶段之前执行：
- 宏替换
- 文件包含 (`#include`)
- 条件编译 (`#if`, `#ifdef`)

**注意**：预处理器宏**不是** C++ 实体，不参与名称查找和类型检查。

### 存储模型

C++ 的对象和变量有**存储期（Storage Duration）**，决定其生命周期：

| 存储期类型 | 生命周期 | 分配位置 |
|------------|----------|----------|
| 自动存储期（Automatic） | 进入块时创建，退出时销毁 | 栈 |
| 静态存储期（Static） | 程序整个运行期间 | 数据段（全局/静态区）|
| 线程存储期（Thread） | 线程整个运行期间（C++11）| 线程本地存储 |
| 动态存储期（Dynamic） | 由 `new`/`delete` 控制 | 堆 |

```cpp
// 存储期示例
thread_local int tls_var = 0;   // C++11 线程存储期

class StorageDemo {
    static int static_member;   // 静态存储期
    int member;                 // 与对象生命周期相同

public:
    void demo() {
        int auto_var = 10;      // 自动存储期
        static int static_local = 20;  // 静态存储期（块作用域）
        int* dyn = new int(30); // 动态存储期

        delete dyn;             // 手动释放
    }
};

int StorageDemo::static_member = 0;  // 定义
```

### 对象模型

C++ 的**对象（Object）**模型包含以下关键概念：

1. **对齐（Alignment）**：对象地址必须是其对齐要求的倍数
2. **生命周期（Lifetime）**：对象存在的时段
3. **存储（Storage）**：对象占用的内存

```cpp
// 对象模型示例
#include <type_traits>
#include <cstddef>

struct alignas(16) AlignedData {  // 显式指定对齐要求
    char data[32];
};

void object_model_demo() {
    // 对齐查询
    std::size_t int_align = alignof(int);        // 通常是 4
    std::size_t double_align = alignof(double);  // 通常是 8

    // 对象大小
    std::size_t int_size = sizeof(int);          // 通常是 4

    // 动态对象创建
    int* p = new int(42);  // 分配并构造对象
    delete p;              // 销毁并释放对象

    // 数组对象
    int* arr = new int[10];  // 分配数组
    delete[] arr;            // 释放数组
}
```

### 名称查找机制

C++ 的**名称查找（Name Lookup）**机制比 C 更复杂：

1. **非限定名称查找**：在当前作用域逐级向外查找
2. **限定名称查找**：使用 `::` 在指定作用域查找
3. **参数依赖查找（ADL）**：根据参数类型在其关联命名空间查找
4. **模板名称查找**：考虑依赖名称和非依赖名称

```cpp
// ADL 详细示例
namespace math {
    class Complex {
    public:
        double real, imag;
        Complex(double r, double i) : real(r), imag(i) {}
    };

    // 运算符定义在命名空间内
    Complex operator+(const Complex& a, const Complex& b) {
        return Complex(a.real + b.real, a.imag + b.imag);
    }
}

void adl_example() {
    math::Complex a(1.0, 2.0);
    math::Complex b(3.0, 4.0);

    // ADL：编译器发现 operator+ 的参数类型来自 math 命名空间
    // 因此也在 math 命名空间查找 operator+
    math::Complex c = a + b;  // 使用 math::operator+
}
```

---

## 5. 使用场景 (Use Cases)

### 适用场景

C++ 基础概念在以下场景中尤为重要：

1. **系统级编程**：操作系统内核、驱动程序、嵌入式系统
2. **高性能计算**：游戏引擎、高频交易、科学计算
3. **库开发**：标准库实现、第三方库（Boost、Qt 等）
4. **跨平台开发**：需要直接控制硬件和内存的应用

### 最佳实践

#### 1. 实体命名和组织

```cpp
// 推荐使用命名空间组织代码
namespace myapp {
namespace utils {

// 类名使用 PascalCase
class FileReader {
public:
    // 成员函数使用 camelCase
    bool open(const std::string& filename);

    // 私有成员使用下划线后缀
private:
    std::string filename_;
    std::ifstream file_;
};

// 枚举使用强枚举（C++11起）
enum class FileMode { Read, Write, Append };

// 常量使用 constexpr 或 const
constexpr size_t BUFFER_SIZE = 1024;
const std::string APP_VERSION = "1.0.0";

// 模板使用描述性名称
template<typename InputIt, typename OutputIt>
OutputIt transformData(InputIt first, InputIt last, OutputIt dest);

}  // namespace utils
}  // namespace myapp
```

#### 2. 声明与定义的分离

```cpp
// ===== myclass.h =====
#ifndef MYCLASS_H
#define MYCLASS_H

namespace mylib {

// 类声明 - 在头文件中
class MyClass {
public:
    MyClass();              // 构造函数声明
    ~MyClass();             // 析构函数声明

    void publicMethod();    // 公共接口声明

    // 内联函数可以直接在头文件定义
    int getValue() const { return value_; }

    // 静态成员声明
    static int instanceCount;

private:
    int value_;
};

// 模板声明和定义通常都在头文件
template<typename T>
T add(T a, T b);

}  // namespace mylib

#endif

// ===== myclass.cpp =====
#include "myclass.h"

namespace mylib {

// 静态成员定义
int MyClass::instanceCount = 0;

// 成员函数定义
MyClass::MyClass() : value_(0) {
    ++instanceCount;
}

MyClass::~MyClass() {
    --instanceCount;
}

void MyClass::publicMethod() {
    // 实现
}

// 模板定义
template<typename T>
T add(T a, T b) {
    return a + b;
}

// 显式实例化（可选）
template int add<int>(int, int);
template double add<double>(double, double);

}  // namespace mylib
```

#### 3. 链接属性的正确使用

```cpp
// ===== constants.h =====
#ifndef CONSTANTS_H
#define CONSTANTS_H

// C++17 内联变量 - 可以在多个翻译单元定义
inline constexpr double PI = 3.14159265358979323846;
inline constexpr int MAX_SIZE = 100;

// C++17 之前的替代方案：constexpr 函数
constexpr double getPi() { return 3.14159265358979323846; }

// 外部链接的变量声明
extern int globalCounter;  // 在一个 .cpp 文件中定义

#endif

// ===== utils.cpp =====
// 推荐使用匿名命名空间代替 static
namespace {
    // 这些符号只在当前翻译单元可见（内部链接）
    int helperVar = 0;

    void internalHelper() {
        // ...
    }
}

// 或者使用 static（传统方式）
static int anotherHelperVar = 0;

// 导出的接口
namespace utils {

void publicFunction() {
    internalHelper();  // 可以访问匿名命名空间的函数
}

}  // namespace utils
```

#### 4. 作用域最小化原则

```cpp
void scope_best_practice() {
    // 在需要时声明变量
    for (int i = 0; i < 10; ++i) {
        // i 只在循环内有效
    }

    // C++17 结构化绑定 - 解构对象并限制作用域
    std::map<std::string, int> data;
    if (auto [iter, success] = data.insert({"key", 42}); success) {
        // iter 和 success 只在这个 if 块内有效
        std::cout << iter->first << std::endl;
    }

    // C++20 带初始化的范围 for
    std::vector<int> vec = {1, 2, 3};
    for (size_t i = 0; auto& elem : vec) {
        // i 是索引，elem 是元素引用
        elem += static_cast<int>(i++);
    }

    // 使用 namespace 限制名称污染
    {
        using namespace std::chrono;
        auto now = system_clock::now();  // 在此块内可以直接使用
    }
}
```

### 常见陷阱

#### 陷阱 1：ODR（One Definition Rule）违规

```cpp
// 错误示例
// file1.cpp
int global = 10;  // 定义

// file2.cpp
int global;       // 又是定义！违反 ODR

// 正确做法
// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H
extern int global;  // 声明
#endif

// file1.cpp
#include "constants.h"
int global = 10;    // 唯一定义

// file2.cpp
#include "constants.h"
// 使用声明的 global
```

#### 陷阱 2：名称查找的意外行为

```cpp
// 问题示例
void func(double);

namespace ns {
    void func(int);

    void test() {
        using ::func;       // 引入全局 func
        func(1.0);          // 调用 func(double)，不是 func(int)！
                            // using 声明会隐藏同名的其他声明
    }
}

// 更好的做法
namespace ns {
    void func(int);

    void test() {
        func(1.0);          // 明确调用 ns::func(int)，1.0 转换为 1
        ::func(1.0);        // 如果需要调用全局版本，显式指定
    }
}
```

#### 陷阱 3：不完全类型的误用

```cpp
// 错误示例
struct Incomplete;  // 前向声明

void bad_func() {
    Incomplete obj;              // 错误：不能创建不完全类型的对象
    sizeof(Incomplete);          // 错误：不能对不完全类型使用 sizeof
    delete new Incomplete;       // 错误：不能 delete 不完全类型指针
}

// 正确做法
struct Complete {
    int data;
};

void good_func() {
    Complete obj;                // OK
    std::cout << sizeof(Complete);  // OK
}

// 不完全类型的合法使用
struct Forward;                  // 声明
Forward* ptr = nullptr;          // OK：可以声明指针
extern Forward& ref;             // OK：可以声明引用
void func(Forward* p);           // OK：函数参数
class Container {
    Forward* ptr;                // OK：成员指针（如果是指针成员）
    // Forward member;            // 错误：数据成员需要完整类型
};
```

---

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：完整的 C++ 程序结构

```cpp
#include <iostream>
#include <string>
#include <vector>

// 命名空间组织
namespace company::app {  // C++17 嵌套命名空间定义

// 类模板定义
template<typename T>
class Container {
public:
    void add(const T& item) {
        data_.push_back(item);
    }

    const T& get(size_t index) const {
        return data_.at(index);
    }

    size_t size() const { return data_.size(); }

private:
    std::vector<T> data_;
};

// 普通类定义
class Person {
public:
    Person(std::string name, int age)
        : name_(std::move(name)), age_(age) {}

    void display() const {
        std::cout << name_ << " (" << age_ << ")" << std::endl;
    }

private:
    std::string name_;
    int age_;
};

// 函数重载
void print(const std::string& msg) {
    std::cout << "String: " << msg << std::endl;
}

void print(int value) {
    std::cout << "Int: " << value << std::endl;
}

// 内联变量（C++17）
inline int globalCounter = 0;

}  // namespace company::app

// 主函数
int main() {
    using namespace company::app;

    // 使用类模板
    Container<Person> people;
    people.add(Person("Alice", 30));
    people.add(Person("Bob", 25));

    // 使用范围 for（C++11）
    for (size_t i = 0; i < people.size(); ++i) {
        people.get(i).display();
    }

    // 函数重载
    print("Hello");
    print(42);

    // C++17 结构化绑定
    std::pair<int, std::string> data = {1, "one"};
    auto [num, str] = data;
    std::cout << num << " = " << str << std::endl;

    return 0;
}
```

#### 示例 2：声明与定义分离

```cpp
// ===== math_utils.h =====
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

namespace math {

// 类声明
class Calculator {
public:
    Calculator();
    ~Calculator();

    double add(double a, double b);
    double multiply(double a, double b);

    static int getInstanceCount();

private:
    static int instanceCount_;
    int operationCount_;
};

// 函数模板声明
template<typename T>
T square(T value);

// 变量声明（外部链接）
extern double pi;

// 内联变量定义（C++17）
inline constexpr double e = 2.718281828459045;

}  // namespace math

#endif

// ===== math_utils.cpp =====
#include "math_utils.h"

namespace math {

// 静态成员定义
int Calculator::instanceCount_ = 0;

// 变量定义
double pi = 3.141592653589793;

// 成员函数定义
Calculator::Calculator() : operationCount_(0) {
    ++instanceCount_;
}

Calculator::~Calculator() {
    --instanceCount_;
}

double Calculator::add(double a, double b) {
    ++operationCount_;
    return a + b;
}

double Calculator::multiply(double a, double b) {
    ++operationCount_;
    return a * b;
}

int Calculator::getInstanceCount() {
    return instanceCount_;
}

// 模板定义
template<typename T>
T square(T value) {
    return value * value;
}

// 显式实例化
template int square<int>(int);
template double square<double>(double);

}  // namespace math

// ===== main.cpp =====
#include "math_utils.h"
#include <iostream>

int main() {
    using namespace math;

    Calculator calc;
    std::cout << "2 + 3 = " << calc.add(2, 3) << std::endl;
    std::cout << "4 * 5 = " << calc.multiply(4, 5) << std::endl;

    std::cout << "PI = " << pi << std::endl;
    std::cout << "E = " << e << std::endl;

    std::cout << "Square of 5 = " << square(5) << std::endl;

    return 0;
}
```

### 高级用法

#### 示例 3：跨文件链接与匿名命名空间

```cpp
// ===== internal.h =====
#ifndef INTERNAL_H
#define INTERNAL_H

namespace app {

// 公共接口
void initialize();
void shutdown();
int getStatus();

// 外部可见的变量声明
extern int globalConfig;

}  // namespace app

#endif

// ===== internal.cpp =====
#include "internal.h"

// 匿名命名空间 - 内部链接，仅在当前文件可见
namespace {
    // 这些符号不会导出到其他翻译单元
    int internalState = 0;
    bool initialized = false;

    void logInternal(const char* msg) {
        // 内部日志实现
        (void)msg;
    }

    class InternalHelper {
    public:
        void doWork() { ++internalState; }
    };

    // 静态变量（C++11 线程安全初始化）
    InternalHelper& getHelper() {
        static InternalHelper helper;  // 静态局部变量
        return helper;
    }
}

// 命名空间 app 的实现
namespace app {

// 变量定义（一个定义规则）
int globalConfig = 0;

void initialize() {
    if (!initialized) {
        logInternal("Initializing...");
        getHelper().doWork();
        initialized = true;
        internalState = 1;
    }
}

void shutdown() {
    if (initialized) {
        logInternal("Shutting down...");
        internalState = 0;
        initialized = false;
    }
}

int getStatus() {
    return internalState;
}

}  // namespace app

// ===== other.cpp =====
#include "internal.h"

void useApp() {
    app::initialize();
    app::globalConfig = 42;

    // 以下会编译错误：
    // internalState = 10;        // 错误：未声明
    // logInternal("test");       // 错误：未声明

    app::shutdown();
}
```

#### 示例 4：名称查找与 ADL

```cpp
#include <iostream>
#include <vector>

// 自定义命名空间
namespace graphics {

class Point {
public:
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
};

class Color {
public:
    unsigned char r, g, b;
    Color(unsigned char r, unsigned char g, unsigned char b)
        : r(r), g(g), b(b) {}
};

// 运算符重载在命名空间内
Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Color& c) {
    os << "RGB(" << (int)c.r << ", " << (int)c.g << ", " << (int)c.b << ")";
    return os;
}

// 普通函数
void render(const Point& p, const Color& c) {
    std::cout << "Rendering point " << p << " with color " << c << std::endl;
}

}  // namespace graphics

// 使用示例
int main() {
    using namespace graphics;

    Point p1(1.0, 2.0);
    Point p2(3.0, 4.0);

    // ADL：编译器在 graphics 命名空间查找 operator+
    Point p3 = p1 + p2;

    // ADL：编译器在 graphics 命名空间查找 operator<<
    std::cout << "p3 = " << p3 << std::endl;

    Color red(255, 0, 0);
    std::cout << "Color: " << red << std::endl;

    // ADL：函数调用
    render(p3, red);  // 直接在 graphics 命名空间查找

    return 0;
}
```

### 常见错误及修正

#### 错误 1：违反一个定义规则（ODR）

```cpp
// ========== 错误示例 ==========
// constants.h
const int MAX_SIZE = 100;  // 问题：每个包含此头文件的 cpp 都有独立定义

// file1.cpp
#include "constants.h"
// MAX_SIZE 在此定义，内部链接

// file2.cpp
#include "constants.h"
// MAX_SIZE 再次定义，不同的对象！

// 如果在头文件中使用，可能导致代码膨胀和链接问题

// ========== 正确做法 ==========
// C++17 方式：内联变量
// constants.h
inline constexpr int MAX_SIZE = 100;  // 所有翻译单元共享同一对象

// 或者使用 constexpr 函数
constexpr int getMaxSize() { return 100; }

// C++17 之前的方式：在头文件中声明，一个 cpp 文件中定义
// constants.h
extern const int MAX_SIZE;  // 声明

// constants.cpp
#include "constants.h"
const int MAX_SIZE = 100;   // 唯一定义
```

#### 错误 2：不完全类型的错误使用

```cpp
// ========== 错误示例 ==========
// widget.h
struct WidgetImpl;  // 前向声明（PIMPL 惯用法）

class Widget {
    WidgetImpl* pImpl;  // OK：指针可以是不完全类型
    // WidgetImpl impl; // 错误：成员需要完整类型
public:
    Widget();
    ~Widget();
    void doSomething();
};

// widget.cpp
#include "widget.h"

struct WidgetImpl {
    int data;
    std::vector<int> items;
};

Widget::Widget() : pImpl(new WidgetImpl{}) {}  // OK：此处 WidgetImpl 已完整

// 常见错误：在头文件中尝试使用不完全类型
WidgetImpl* createWidget();  // OK
// void processWidget(WidgetImpl w);  // 错误：参数需要完整类型
// std::vector<WidgetImpl> widgets;   // 错误：vector 需要完整类型

// ========== 正确做法 ==========
// widget.h
struct WidgetImpl;  // 前向声明

class Widget {
    std::unique_ptr<WidgetImpl> pImpl;  // OK：智能指针支持不完全类型
public:
    Widget();
    ~Widget();  // 必须在 cpp 中定义，因为需要完整类型
    Widget(Widget&&) noexcept;           // 移动操作也需在 cpp 中定义
    Widget& operator=(Widget&&) noexcept;

    void doSomething();
};

// widget.cpp
#include "widget.h"
#include <vector>

struct WidgetImpl {
    int data;
    std::vector<int> items;
};

Widget::Widget() : pImpl(std::make_unique<WidgetImpl>()) {}
Widget::~Widget() = default;  // 默认实现，此时 WidgetImpl 完整
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;
```

#### 错误 3：混淆声明和定义

```cpp
// ========== 错误示例 ==========
// utils.h
void helper();  // 函数声明

int value;      // 错误：这是定义，不是声明！
                // 每个包含此头文件的 cpp 都会定义一个独立的 value

// ========== 正确做法 ==========
// utils.h
void helper();           // 函数声明
extern int value;        // 变量声明（使用 extern）

inline void inlineHelper() {}  // 内联函数，可以在头文件定义

// C++17 内联变量
inline int inlineValue = 42;   // 所有翻译单元共享

constexpr int constValue = 10; // constexpr 变量隐式内部链接

// utils.cpp
#include "utils.h"

void helper() {  // 函数定义
    // ...
}

int value = 100;  // 变量定义（仅一处）
```

---

## 7. 总结 (Summary)

### 核心要点

1. **程序结构**：C++ 程序是由文本文件组成的序列，经翻译后成为可执行程序，从 `main` 函数开始执行。

2. **实体系统**：C++ 程序包含多种实体类型（值、对象、引用、函数、类型、模板、命名空间等），预处理器宏**不是** C++ 实体。

3. **声明与定义**：
   - 声明引入实体并关联名称
   - 定义提供使用实体所需的所有信息
   - 程序必须遵循 ODR（一个定义规则）

4. **作用域与链接**：
   - 作用域决定名称在程序中的可见范围
   - 链接属性决定名称是否能跨翻译单元引用相同实体
   - C++ 推荐使用匿名命名空间替代 `static` 实现内部链接

5. **类型系统**：
   - 每个对象、引用、函数、表达式都有类型
   - C++ 支持基本类型、复合类型、用户定义类型
   - 区分完整类型和不完全类型

### C++ 与 C 的关键差异

| 方面 | C | C++ |
|------|---|-----|
| 实体类型 | 较简单 | 更丰富（引用、类、模板等）|
| 链接属性 | `static` 和 `extern` | 增加匿名命名空间、`const` 默认内部链接 |
| 作用域 | 较简单 | 增加类作用域、命名空间作用域、枚举作用域 |
| 名称查找 | 基础 | ADL、复杂的模板名称查找 |
| 存储期 | 3种 | 增加线程存储期（C++11）|
| 宏 | 主要扩展机制 | 推荐使用模板和内联函数替代 |

### 学习建议

1. **深入理解 ODR**：这是 C++ 最重要的规则之一，违反会导致链接错误或难以调试的问题
2. **掌握名称查找**：特别是 ADL，理解为什么某些调用能找到函数而另一些不能
3. **合理使用命名空间**：组织代码、避免名称污染、控制链接属性
4. **理解对象模型**：生命周期、存储期、对齐要求对性能关键代码很重要
5. **学习现代 C++**：C++11/14/17/20/23 引入了许多简化代码和提高安全性的特性

### 参考资源

- **ISO/IEC 14882**: C++ 国际标准
- **The C++ Programming Language** (Bjarne Stroustrup): C++ 之父的经典著作
- **C++ Primer** (Lippman, Lajoie, Moo): 全面的入门和参考书籍
- **cppreference.com**: 在线 C++ 参考文档
