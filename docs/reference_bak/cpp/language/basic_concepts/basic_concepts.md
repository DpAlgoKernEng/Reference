# C++ 基础概念

  [1. 概述](#1-概述)

  [2. C++ 程序的结构与翻译过程](#2-c-程序的结构与翻译过程)
  
  [3. 词法元素：关键字、标识符、注释与字面量](#3-词法元素关键字标识符注释与字面量)
  
  [4. C++ 程序的实体（Entities）](#4-c-程序的实体entities)
  
  [5. 声明、定义与 ODR（唯一定义规则）](#5-声明定义与-odr唯一定义规则)
  
  [6. 函数定义、语句与表达式](#6-函数定义语句与表达式)
  
  [7. 名称查找、作用域与链接性](#7-名称查找作用域与链接性)
  
  [8. 类型系统与变量](#8-类型系统与变量)
  
  [9. 参考资料](#9-参考资料)

## 1. 概述

本节系统性地定义和扩展 C++ 编程语言的核心术语与基础概念，帮助读者建立权威、完整的知识体系。

---

## 2. C++ 程序的结构与翻译过程

一个 C++ 程序是一系列包含声明的文本文件（通常是头文件和源文件），它们会经过翻译 ，成为可执行程序，当 C++ 实现调用其 main 函数时，该程序会被执行。

### 2.1 程序文件类型

- 头文件（Header Files）：通常以 `.h` 或 `.hpp` 结尾，包含类、函数、模板、常量等声明。
- 源文件（Source Files）：通常以 `.cpp`、`.cc`、`.cxx` 结尾，包含函数、类等实现。

### 2.2 翻译与执行流程

- 预处理（Preprocessing）：处理 `#include`、`#define`、`#ifdef` 等指令。
- 编译（Compilation）：将源代码翻译为中间代码（如汇编）。
- 汇编（Assembly）：生成目标文件（机器码）。
- 链接（Linking）：将多个目标文件和库合并为可执行文件。
- 执行（Execution）：C++ 实现调用 `main` 函数作为程序入口。

**示例**：

```cpp
// hello.hpp
#ifndef HELLO_HPP
#define HELLO_HPP
void print_hello();
#endif

// hello.cpp
#include <iostream>
#include "hello.hpp"
void print_hello() {
  std::cout << "Hello, World!" << std::endl;
}

// main.cpp
#include "hello.hpp"
int main() {
  print_hello();
  return 0;
}
```

---

## 3. 词法元素：关键字、标识符、注释与字面量

### 3.1 关键字（Keywords）

C++ 关键字具有特殊语法意义，不能作为标识符使用。

| 类别 | 关键字示例 |
|------|-----------------------------|
| 基本类型 | `int`, `double`, `char`, `void` |
| 控制流 | `if`, `else`, `for`, `while`, `switch`, `case`, `break`, `continue`, `return` |
| 存储类 | `static`, `extern`, `mutable`, `register`, `thread_local` |
| 类型修饰 | `const`, `volatile`, `constexpr`, `typename` |
| 结构化 | `struct`, `class`, `union`, `enum`, `namespace` |
| 其他 | `template`, `using`, `public`, `private`, `protected`, `virtual`, `override`, `final`, `decltype`, `noexcept`, `nullptr` |

### 3.2 标识符（Identifiers）

标识符用于命名变量、函数、类、命名空间等。

**命名规则**：

- 只能由字母、数字、下划线组成，且不能以数字开头。
- 区分大小写。
- 不能与关键字重名。

**示例**：

```cpp
int value;
double _result;
class MyClass {};
namespace my_ns {}
```

### 3.3 注释（Comments）

注释在翻译阶段被忽略。

- 单行注释：`// ...`
- 多行注释：`/* ... */`

### 3.4 字面量（Literals）与转义序列

字面量包括整型、浮点型、字符、字符串等。字符值由字符集和编码决定，程序中的某些字符必须使用转义序列来表示。

**转义序列示例**：

```cpp
char newline = '\n'; // 换行符
char tab = '\t';     // 制表符
char quote = '\"';  // 双引号
```

---

## 4. C++ 程序的实体（Entities）

### 4.1 实体类型

- 值（Value）：如整数、浮点数、布尔值等。
- 对象（Object）：具有存储空间的实体，如变量、数组、类实例。
- 引用（Reference）：为对象起别名。
- 结构化绑定（Structured Binding, C++17）：如 `auto [x, y] = point;`
- 结果绑定（Result Binding, C++26）：C++26 新特性。
- 函数（Function）：可调用的代码块。
- 枚举器（Enumerator）：枚举类型的成员。
- 类型（Type）：如 `int`、`std::string`、自定义类等。
- 类成员（Class Member）：类的成员变量、成员函数。
- 模板（Template）：泛型编程工具。
- 模板特化（Template Specialization）：模板的特定实现。
- 包（Pack, C++11）：参数包。
- 命名空间（Namespace）：用于组织代码。

> 注意：预处理器宏（`#define`）不是 C++ 实体。

**示例**：

```cpp
enum Color { Red, Green, Blue }; // 枚举器
struct Point { int x, y; };      // 类型、对象、类成员
template<typename T> class Box {}; // 模板
namespace math { int add(int, int); } // 命名空间
```

---

## 5. 声明、定义与 ODR（唯一定义规则）

### 5.1 声明（Declaration）

声明用于引入实体、指定名称及其属性，将它们与名称关联并定义其属性。

### 5.2 定义（Definition）

定义是包含实体所有必要属性的声明。

**示例**：

```cpp
extern int a;      // 声明
int a = 10;        // 定义
void foo();        // 声明
void foo() {}      // 定义
```

### 5.3 ODR（One Definition Rule）

每个非内联函数或变量在整个程序中只能有一个定义（odr-used）。

**违反 ODR 的示例**：

```cpp
// file1.cpp
int x = 1;
// file2.cpp
int x = 2; // 错误：重复定义
```

---

## 6. 函数定义、语句与表达式

### 6.1 函数定义结构

函数定义通常包含语句序列，部分语句为表达式，指定程序的计算过程。

**示例**：

```cpp
int add(int a, int b) {
  int sum = a + b; // 表达式
  return sum;      // 语句
}
```

---

## 7. 名称查找、作用域与链接性

### 7.1 名称查找（Name Lookup）

程序中遇到的名称通过名称查找与引入它们的声明相关联。

### 7.2 作用域（Scope）

每个名称只在其作用域的程序部分内有效。

- 块作用域：局部变量、for循环变量等。
- 类作用域：类成员。
- 命名空间作用域：命名空间内的名称。
- 文件作用域：全局变量、函数。

**示例**：

```cpp
int global_var = 1; // 文件作用域
namespace ns {
  int ns_var = 2; // 命名空间作用域
}
class C {
  int member;     // 类作用域
};
void func() {
  int local = 3;  // 块作用域
}
```

### 7.3 链接性（Linkage）

链接性决定名称在不同翻译单元间是否指向同一实体。

- 外部链接性：`extern` 声明的全局变量、函数。
- 内部链接性：`static` 声明的全局变量、函数。
- 无链接性：局部变量、类成员等。

**示例**：

```cpp
// a.cpp
int x = 1;           // 外部链接性
static int y = 2;    // 内部链接性
void foo() {}        // 外部链接性
static void bar() {} // 内部链接性
```

---

## 8. 类型系统与变量

C++ 中的每个对象、引用、函数、表达式都与一个类型相关联，该类型可能是基本类型 、复合类型或用户定义类型 ，完整或不完整等。

### 8.1 类型分类

- 基本类型：`int`, `double`, `char`, `bool` 等。
- 复合类型：指针、引用、数组、函数、类、结构体、联合体、枚举等。
- 用户定义类型：类、结构体、枚举、联合体。
- 完整类型/不完整类型：有无全部定义。

**示例**：

```cpp
int a = 10;           // 基本类型
int* p = &a;          // 指针类型
std::string s = "hi"; // 用户定义类型
struct S;             // 不完整类型
```

### 8.2 变量

非静态数据成员的声明对象和声明引用是变量。

**示例**：

```cpp
int x;           // 变量
double& ref = x; // 引用变量
class A {
  int m;       // 非静态数据成员
  static int n; // 静态成员，不属于变量范畴
};
```

---

## 9. 参考资料

- ISO/IEC 14882:2020 (C++20) - Programming languages — C++
- 《C++ Primer》
- 《Effective C++》
- cppreference.com
