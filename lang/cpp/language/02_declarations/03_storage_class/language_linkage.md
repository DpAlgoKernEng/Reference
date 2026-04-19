# 语言链接 (Language Linkage)

## 1. 概述 (Overview)

**语言链接 (Language Linkage)** 是 C++ 提供的一种机制，用于实现不同编程语言编写的程序单元之间的链接。

每个函数类型、具有外部链接的函数名以及具有外部链接的变量名，都有一个称为"语言链接"的属性。语言链接封装了与另一种编程语言编写的程序单元进行链接所需的全部要求，包括：
- 调用约定 (Calling Convention)
- 名称修饰 (Name Mangling) 算法
- 其他链接相关的要求

### 保证支持的语言链接

C++ 标准保证支持以下两种语言链接：

| 语言链接 | 说明 |
|---------|------|
| `"C++"` | 默认的语言链接 |
| `"C"` | 使 C++ 程序能够链接 C 语言编写的函数，或在 C++ 程序中定义可被 C 代码调用的函数 |

### 核心用途

1. 在 C++ 程序中调用 C 库函数
2. 在 C++ 程序中定义可被 C 代码调用的函数
3. C++20 起，可用于将声明从其模块中分离（参见模块所有权）

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 语言链接机制的引入主要是为了解决 C++ 与 C 语言之间的互操作性问题。由于 C++ 支持函数重载等特性，编译器需要对函数名进行修饰（name mangling）以区分不同的重载版本，而 C 语言不进行名称修饰。这种差异导致 C++ 代码无法直接调用 C 库函数。

### 设计动机

- 实现与现有 C 库的无缝集成
- 允许 C++ 代码被其他语言调用
- 提供跨语言调用的标准化接口

### 标准版本变更

| 标准 | 章节 | 说明 |
|------|------|------|
| C++98 | 7.5 Linkage specifications | 初始引入语言链接规范 |
| C++03 | 7.5 Linkage specifications | 无重大变更 |
| C++11 | 7.5 Linkage specifications | 无重大变更 |
| C++14 | 7.5 Linkage specifications | 无重大变更 |
| C++17 | 10.5 Linkage specifications | 章节号调整 |
| C++20 | 9.11 Linkage specifications | 新增模块所有权相关用途 |
| C++23 | 9.11 Linkage specifications | 无重大变更 |

### 缺陷报告修复历史

| 缺陷报告 | 适用版本 | 原有行为 | 修正行为 |
|---------|---------|---------|---------|
| CWG 4 | C++98 | 具有内部链接的名称可以有语言链接 | 限制为仅具有外部链接的名称 |
| CWG 341 | C++98 | 具有 "C" 语言链接的函数可以与全局变量同名 | 此情况程序为非良构（若出现在不同翻译单元则无需诊断） |
| CWG 564 | C++98 | 若两个声明仅在语言链接规范上不同，则程序非良构 | 改为比较实际的语言链接 |
| CWG 2460 | C++20 | 具有尾置 requires 子句和 "C" 语言链接的友元函数存在冲突行为 | 此情况下忽略 "C" 语言链接 |
| CWG 2483 | C++98 | 出现在 "C" 语言块中的静态成员函数的类型链接为 "C++" | 类型链接应为 "C" |

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```
extern 字符串字面量 { 声明序列(可选) }    // (1) 块形式
extern 字符串字面量 声明                  // (2) 单声明形式
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `字符串字面量` | 指定所需语言链接的非求值字符串字面量，如 `"C"` 或 `"C++"` |
| `声明序列` | 一系列声明，可以包含嵌套的链接规范 |
| `声明` | 单个声明 |

### 语义说明

**形式 (1)** - 块形式：
- 将语言规范应用于声明序列中声明的所有函数类型、具有外部链接的函数名和具有外部链接的变量名

**形式 (2)** - 单声明形式：
- 将语言规范应用于单个声明或定义

### 作用域规则

- 语言规范只能出现在命名空间作用域
- 语言规范的花括号**不建立新的作用域**
- 语言规范可以嵌套，最内层的规范生效
- 直接包含在语言链接规范中的声明，在确定声明名称的链接以及是否为定义时，被视为包含 `extern` 说明符

---

## 4. 底层原理 (Underlying Principles)

### 名称修饰 (Name Mangling)

语言链接的核心作用之一是控制名称修饰：

| 语言链接 | 名称修饰行为 |
|---------|-------------|
| `"C++"` | 进行完整的名称修饰，支持函数重载 |
| `"C"` | 不进行名称修饰，保持原始函数名 |

### 函数类型与函数名的语言链接

语言链接包含两个独立的概念：

1. **函数类型的语言链接** - 表示调用约定
2. **函数名的语言链接** - 表示名称修饰方式

这两个概念是相互独立的，这意味着一个函数可以：
- 函数名具有 C 语言链接，但函数类型是 C++ 函数类型
- 函数名具有 C++ 语言链接，但函数类型是 C 函数类型

### 链接规范继承规则

- 如果实体的两个声明给予不同的语言链接，程序非良构
- 如果实体声明没有链接规范的重声明，则继承该实体（及其类型，如果存在）的语言链接

### "C" 语言链接的特殊规则

当类成员、具有尾置 requires 子句的友元函数 (C++20 起) 或非静态成员函数出现在 "C" 语言块中时：

- 其类型的语言链接保持为 "C++"
- 但参数类型（如果有）保持为 "C"

### 跨命名空间重声明规则

对于 "C" 语言链接，存在特殊的重声明规则：

设 `C` 是声明具有 "C" 语言链接的函数或变量的声明。如果另一个声明 `D` 声明同名实体，且满足以下任一条件，则 `C` 和 `D` 声明同一实体：

1. `D` 声明属于全局作用域的变量
2. 如果 `C` 声明变量，`D` 也声明变量
3. 如果 `C` 声明函数，`D` 也声明函数

这意味着 "C" 语言链接的实体可以在不同命名空间中重声明，但仍声明同一实体。

### 编译器实现差异

目前唯一区分 "C" 和 "C++" 语言链接函数类型的现代编译器是 Oracle Studio。大多数编译器（包括 GCC 和 Clang）不允许仅语言链接不同的重载，这与 C++ 标准要求存在差异：

- GCC Bug 2316
- Clang Bug 6277
- CWG Issue 1555

---

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 调用 C 库函数

当需要在 C++ 程序中调用 C 语言编写的库函数时，使用 `extern "C"` 声明这些函数。

#### 2. 定义可被 C 调用的函数

当需要在 C++ 程序中定义可被 C 代码调用的函数时，使用 `extern "C"` 定义这些函数。

#### 3. 跨语言接口设计

在需要与其他语言（如汇编语言、Fortran 等）交互时，可能需要使用特定的语言链接。

#### 4. 共享头文件

在 C 和 C++ 共享的头文件中，需要根据编译器类型选择性地使用语言链接。

### 最佳实践

#### 头文件保护模式

在 C/C++ 共享的头文件中使用 `__cplusplus` 宏：

```cpp
#ifdef __cplusplus
extern "C" int foo(int, int);  // C++ 编译器看到这个
#else
int foo(int, int);             // C 编译器看到这个
#endif
```

#### 函数指针的语言链接

注意函数指针的语言链接是其类型的一部分：

```cpp
extern "C" typedef void FUNC();  // C 函数类型

FUNC f2;                         // 名字 f2 有 C++ 链接，但类型是 C 函数
extern "C" FUNC f3;              // 名字 f3 有 C 链接，类型也是 C 函数
```

### 常见陷阱

#### 1. 链接冲突

```cpp
extern "C" double f();
static double f();        // 错误：链接冲突

extern "C" static void g(); // 错误：链接冲突
```

#### 2. 语言链接不匹配

```cpp
extern "C" int f();
extern "C++" int f();   // 错误：不同的语言链接

int h();                // 默认 C++ 语言链接
extern "C" int h();     // 错误：不同的语言链接
```

#### 3. 跨命名空间重定义

```cpp
extern "C" {
    int x;
    int g() { return 1; }
}

namespace A {
    int x;                // 错误：重定义 "x"
    int g() { return 1; } // 错误：重定义 "g"
}
```

#### 4. 类型不匹配的重声明

```cpp
namespace A {
    extern "C" int x();
    extern "C" int y();
}

int x;  // 错误：将 "x" 重声明为不同类型的实体

namespace B {
    void y();  // 错误：以不同类型重声明 "y"
}
```

---

## 6. 代码示例 (Examples)

### 6.1 基础用法：调用 C 库函数

```cpp
// 声明 C 库函数
extern "C"
{
    int open(const char *path_name, int flags);  // C 函数声明
}

int main()
{
    int fd = open("test.txt", 0);  // 从 C++ 程序调用 C 函数
    return 0;
}
```

### 6.2 基础用法：定义可被 C 调用的函数

```cpp
#include <iostream>

// 此 C++ 函数可以被 C 代码调用
extern "C" void handler(int)
{
    std::cout << "Callback invoked\n";  // 函数内部可以使用 C++ 特性
}
```

### 6.3 高级用法：函数指针与语言链接

```cpp
// 声明一个具有 C 链接的函数 f1，它接受一个指向 C 函数的指针
extern "C" void f1(void(*pf)());

// 声明 FUNC 为 C 函数类型
extern "C" typedef void FUNC();

FUNC f2;              // 名字 f2 有 C++ 链接，但其类型是 C 函数
extern "C" FUNC f3;   // 名字 f3 有 C 链接，其类型也是 C 函数

// pf2 是指向 C++ 函数的指针，该函数接受一个指向 C 函数的指针作为参数
void (*pf2)(FUNC*);

extern "C"
{
    static void f4();  // 函数名 f4 有内部链接（无语言链接）
                       // 但函数类型有 C 语言链接
}
```

### 6.4 高级用法：类成员的特殊处理

```cpp
extern "C"
{
    class X
    {
        void mf();            // 函数 mf 及其类型有 C++ 语言链接
        void mf2(void(*)());  // 函数 mf2 有 C++ 语言链接
                              // 参数类型是"指向 C 函数的指针"
    };
}

template<typename T>
struct A { struct B; };

extern "C"
{
    template<typename T>
    struct A<T>::B
    {
        // C 语言链接被忽略（C++20 起）
        friend void f(B*) requires true {}
    };
}

namespace Q
{
    extern "C" void f();  // 非错误
}
```

### 6.5 高级用法："C" 链接的跨命名空间重声明

```cpp
extern "C"
{
    int x;
    int f();
    int g() { return 1; }
}

namespace A
{
    int x;                // 错误：重定义 "x"
    int f();              // OK，重声明 "f"
    int g() { return 1; } // 错误：重定义 "g"
}
```

### 6.6 常见错误：声明与定义的链接不匹配

```cpp
// 错误示例
extern "C" int f();
extern "C++" int f();  // 错误：不同的语言链接

// 正确示例
extern "C" int g();
int g();               // OK，继承 C 语言链接
```

### 6.7 常见错误：链接冲突

```cpp
// 错误示例
extern "C" double f();
static double f();        // 错误：链接冲突

extern "C" static void g(); // 错误：链接冲突

// 解释：
// extern "C" int x; 是声明而非定义
// 等价于 extern "C" { extern int x; }

extern "C" { int x; }     // 声明且定义
```

### 6.8 实践示例：跨平台头文件设计

```cpp
// mylib.h - 可被 C 和 C++ 共享的头文件

#ifndef MYLIB_H
#define MYLIB_H

#ifdef __cplusplus
extern "C" {
#endif

// C 函数声明
int mylib_init(void);
void mylib_cleanup(void);
int mylib_process(const char* input, char* output, int max_len);

#ifdef __cplusplus
}
#endif

#endif // MYLIB_H
```

### 6.9 实践示例：C++ 回调函数供 C 使用

```cpp
// callback_handler.cpp
#include <iostream>

// 定义可被 C 代码调用的回调函数
extern "C" void on_event(int event_id, const char* message)
{
    std::cout << "Event " << event_id << ": " << message << std::endl;
}

// 假设有一个 C 库需要注册回调
// extern "C" void register_callback(void (*callback)(int, const char*));
```

### 6.10 编译器差异示例

```cpp
// 注意：此代码展示了编译器实现的差异
// 大多数编译器无法正确处理仅语言链接不同的函数类型

extern "C"   using c_predfun   = int(const void*, const void*);
extern "C++" using cpp_predfun = int(const void*, const void*);

// 标准要求这是非良构的，但大多数编译器接受
static_assert(std::is_same<c_predfun, cpp_predfun>::value,
              "C and C++ language linkages shall not differentiate function types.");

// 以下声明在大多数编译器中不会声明重载
// 因为 c_predfun 和 cpp_predfun 被视为相同类型
void qsort(void* base, std::size_t nmemb, std::size_t size, c_predfun*   compar);
void qsort(void* base, std::size_t nmemb, std::size_t size, cpp_predfun* compar);
```

---

## 7. 总结 (Summary)

### 核心要点

1. **基本概念**：语言链接是函数类型、具有外部链接的函数名和变量名的属性，封装了跨语言链接的要求。

2. **标准支持**：C++ 标准保证支持 `"C++"`（默认）和 `"C"` 两种语言链接。

3. **语法形式**：支持块形式 (`extern "C" { ... }`) 和单声明形式 (`extern "C" void f();`)。

4. **双重独立性**：函数类型的语言链接（调用约定）和函数名的语言链接（名称修饰）是独立的。

5. **特殊规则**：
   - 语言规范的花括号不建立新作用域
   - 语言规范只能出现在命名空间作用域
   - 直接包含在链接规范中的声明被视为包含 `extern` 说明符

6. **"C" 链接特性**：
   - 类成员函数保持 C++ 类型链接
   - 支持跨命名空间重声明同一实体

### 技术对比

| 特性 | "C" 链接 | "C++" 链接 |
|------|---------|-----------|
| 名称修饰 | 无 | 有 |
| 函数重载 | 不支持 | 支持 |
| 调用约定 | C 风格 | 平台/C++ 风格 |
| 跨语言兼容性 | 高（与 C 互操作） | 仅 C++ 内部 |

### 学习建议

1. **从实际需求出发**：先理解为什么需要语言链接（C/C++ 互操作），再学习具体语法。

2. **理解名称修饰**：深入了解 C++ 的名称修饰机制有助于理解语言链接的重要性。

3. **注意编译器差异**：大多数编译器对函数类型语言链接的处理与标准要求存在差异，实际编码时需注意。

4. **遵循最佳实践**：在共享头文件中使用 `__cplusplus` 宏保护 `extern "C"` 声明。

5. **查阅缺陷报告**：了解 CWG 相关缺陷报告有助于深入理解语言链接的边界情况。

---

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.11 Linkage specifications [dcl.link]
- C++20 标准 (ISO/IEC 14882:2020): 9.11 Linkage specifications [dcl.link]
- C++17 标准 (ISO/IEC 14882:2017): 10.5 Linkage specifications [dcl.link]
- C++14 标准 (ISO/IEC 14882:2014): 7.5 Linkage specifications [dcl.link]
- C++11 标准 (ISO/IEC 14882:2011): 7.5 Linkage specifications [dcl.link]
- C++03 标准 (ISO/IEC 14882:2003): 7.5 Linkage specifications [dcl.link]
- C++98 标准 (ISO/IEC 14882:1998): 7.5 Linkage specifications [dcl.link]