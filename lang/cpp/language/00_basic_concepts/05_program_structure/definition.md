# 定义与 ODR (Definitions and ODR)

## 1. 概述 (Overview)

**定义 (Definition)** 是完全定义声明所引入实体的声明。每个声明都是定义，除了特定例外情况。

**单一定义规则 (One Definition Rule, ODR)** 是 C++ 程序的核心约束：任何变量、函数、类类型、枚举类型、概念或模板在任意翻译单元中只允许有一个定义。

### 核心概念

```
声明 (Declaration) ─┬─> 定义 (Definition)：完全定义实体
                    │
                    └─> 非定义声明：仅声明实体存在
```

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

ODR 是 C++ 类型系统和链接模型的基础，确保程序在链接时的正确性。C++ 的 ODR 比 C 更严格，要求类型定义在所有翻译单元中完全相同。

### 版本演进

| C++ 标准版本 | 发布年份 | 相关改进 |
|-------------|---------|---------|
| C++98 | 1998 | 基础 ODR 体系 |
| C++11 | 2011 | Lambda 表达式 ODR；`constexpr` 变量 |
| C++17 | 2017 | 内联变量；结构化绑定 ODR-use |
| C++20 | 2020 | 概念 ODR；模块中 ODR 变化 |
| C++26 | 2026 | volatile 对象 ODR-use 规则细化 |

### 缺陷报告

| 缺陷报告 | 问题 | 解决方案 |
|---------|------|---------|
| CWG 261 | 多态类的释放函数可能被 ODR-use | 补充构造/析构函数的 ODR-use 情况 |
| CWG 1472 | 常量引用即使立即 lvalue-to-rvalue 转换也被 ODR-use | 此时不 ODR-use |
| CWG 1614 | 取纯虚函数地址会 ODR-use 它 | 不 ODR-use |
| CWG 1741 | 常量对象立即 lvalue-to-rvalue 转换被 ODR-use | 不 ODR-use |
| CWG 2300 | 不同翻译单元的 lambda 闭包类型不能相同 | ODR 下可以相同 |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 定义 vs 声明

```cpp
// 定义
int x = 42;                    // 变量定义
int f(int n) { return n; }     // 函数定义
struct S { int a; };           // 类定义

// 非定义声明
extern int x;                  // extern 声明
int f(int);                    // 函数原型
struct S;                      // 前向声明
```

### 3.2 常见非定义声明

| 声明类型 | 示例 | 说明 |
|---------|------|------|
| 函数声明（无函数体） | `int f(int);` | 仅声明 |
| extern 声明（无初始化器） | `extern const int a;` | 仅声明 |
| 静态数据成员声明（类内） | `static int i;` | 仅声明 |
| 类名前向声明 | `struct S;` | 仅声明 |
| 不透明枚举声明 (C++11) | `enum Color : int;` | 仅声明 |
| 模板参数声明 | `template<typename T>` | 仅声明 |
| typedef 声明 | `typedef S S2;` | 仅声明 |
| 别名声明 (C++11) | `using S2 = S;` | 仅声明 |
| using 声明 | `using N::d;` | 仅声明 |
| 显式实例化声明 (C++11) | `extern template f<int>;` | 仅声明 |

## 4. 底层原理 (Underlying Principles)

### 4.1 定义的条件

**定义**是完全定义声明所引入实体的声明。每个声明都是定义，**除了**以下情况：

#### 非定义声明详解

**1. 函数声明（无函数体）**

```cpp
int f(int);           // 声明，不定义 f
int f(int x) {        // 定义 f 和 x
    return x;
}
```

**2. extern 声明（无初始化器）**

```cpp
extern const int a;   // 声明，不定义 a
extern const int b = 1; // 定义 b（有初始化器）
```

**3. 类内静态数据成员声明**

```cpp
struct S {
    int n;               // 定义 S::n
    static int i;        // 声明，不定义 S::i
    inline static int x; // 定义 S::x (C++17)
};

int S::i;                // 定义 S::i
```

**4. 类名前向声明**

```cpp
struct S;               // 声明，不定义 S
class Y f(class T p);   // 声明，不定义 Y 和 T
```

**5. 不透明枚举声明 (C++11)**

```cpp
enum Color : int;       // 声明，不定义 Color
```

**6. 模板参数声明**

```cpp
template<typename T>    // 声明，不定义 T
void f(T x);
```

**7. typedef 和别名声明**

```cpp
typedef S S2;           // 声明，不定义 S2
using S2 = S;           // 声明，不定义 S2 (C++11)
```

**8. using 声明**

```cpp
using N::d;             // 声明，不定义 d
```

**9. 显式实例化声明 (C++11)**

```cpp
extern template
f<int, char>;           // 声明，不定义 f<int, char>
```

**10. 显式特化声明（非定义）**

```cpp
template<>
struct A<int>;          // 声明，不定义 A<int>
```

### 4.2 隐式定义

编译器可能隐式定义以下特殊成员函数：

- 默认构造函数
- 复制构造函数
- 移动构造函数
- 复制赋值运算符
- 移动赋值运算符
- 析构函数

### 4.3 单一定义规则 (ODR)

#### 翻译单元内 ODR

在任意翻译单元中，以下实体只允许有一个定义：

- 变量
- 函数
- 类类型
- 枚举类型
- 概念 (C++20)
- 模板

#### 程序整体 ODR

**非内联函数或变量**：
- 整个程序中必须恰好有一个定义
- 如果被 ODR-use 但没有定义，行为未定义

**内联函数或变量 (C++17)**：
- 每个 ODR-use 的翻译单元都必须有定义

**类类型**：
- 在需要完整类型的地方必须有定义

### 4.4 多定义允许的情况

以下实体可以跨翻译单元有多个定义（需满足特定条件）：

| 实体类型 | 条件 |
|---------|------|
| 类类型 | 头文件中定义，标记相同 |
| 枚举类型 | 头文件中定义，标记相同 |
| 内联函数 | 头文件中定义，标记相同 |
| 内联变量 (C++17) | 头文件中定义，标记相同 |
| 模板实体 | 头文件中定义，标记相同 |

**必须满足的条件**：

1. 每个定义出现在不同翻译单元
2. 定义未附加到命名模块 (C++20)
3. 每个定义由相同的标记序列组成
4. 名称查找找到相同实体
5. 对应实体具有相同语言链接
6. 其他一致性要求...

### 4.5 ODR-use（单一定义规则使用）

**非正式定义**：

| 实体 | ODR-use 条件 |
|-----|-------------|
| 对象 | 值被读取（非编译时常量）或写入、地址被取、引用绑定到它 |
| 引用 | 被使用且其引用对象在编译时未知 |
| 函数 | 被调用或地址被取 |

**正式定义**：

变量 `x` 被潜在求值表达式 `expr` ODR-use，除非：

1. `x` 是引用且可在常量表达式中使用
2. `x` 不是引用且满足特定条件（弃值表达式、常量对象等）

```cpp
struct S {
    static const int x = 0; // 静态数据成员
};

const int& f(const int& r);

int n = b ? (1, S::x)      // S::x 不被 ODR-use
          : f(S::x);       // S::x 被 ODR-use：需要定义
```

### 4.6 潜在结果 (Potential Results)

表达式 `E` 的潜在结果集合：

| 表达式类型 | 潜在结果 |
|-----------|---------|
| 标识表达式 | `E` 本身 |
| 下标表达式 `E1[E2]` | 数组操作数的潜在结果 |
| 类成员访问 `E1.E2`（非静态数据成员） | `E1` 的潜在结果 |
| 类成员访问（静态数据成员） | 标识该成员的表达式 |
| 指针成员访问 `E1.*E2` | `E1` 的潜在结果 |
| 括号表达式 `(E1)` | `E1` 的潜在结果 |
| 条件表达式 `E1 ? E2 : E3` | `E2` 和 `E3` 潜在结果的并集 |
| 逗号表达式 `(E1, E2)` | `E2` 的潜在结果 |
| 其他 | 空集 |

## 5. 使用场景 (Use Cases)

### 5.1 声明与定义分离

| 场景 | 做法 |
|-----|------|
| 头文件 | 放置声明 |
| 源文件 | 放置定义 |
| 内联函数/模板 | 头文件中定义 |

### 5.2 避免 ODR 违规

| 场景 | 解决方案 |
|-----|---------|
| 头文件中的非内联函数 | 使用 `inline` 或移到源文件 |
| 头文件中的变量 | 使用 `inline` (C++17) 或 `extern` |
| 不同定义的相同类名 | 使用命名空间隔离 |

### 5.3 注意事项

1. **C vs C++ 的 ODR 差异**：
   - C 没有程序范围的类型 ODR
   - C++ 要求类型定义标记完全相同

2. **静态常量成员**：
   - 类内声明 + 类外定义（如果 ODR-use）
   - `constexpr` 隐式内联 (C++17)

3. **模板**：
   - 定义必须在头文件
   - 显式实例化可放在源文件

## 6. 代码示例 (Examples)

### 6.1 声明与定义

```cpp
// header.hpp
#ifndef HEADER_HPP
#define HEADER_HPP

// 声明
extern int global_var;
void func();

// 定义（内联）
inline int inline_var = 42;

// 类定义（允许跨翻译单元）
class MyClass {
public:
    void method();  // 声明

    static int static_member;  // 声明
    inline static int inline_member = 0;  // 定义 (C++17)
};

// 模板定义（允许跨翻译单元）
template<typename T>
T add(T a, T b) {
    return a + b;
}

#endif

// source.cpp
#include "header.hpp"

// 定义
int global_var = 100;

void func() {
    // 实现
}

int MyClass::static_member = 0;  // 定义
```

### 6.2 ODR-use 示例

```cpp
#include <iostream>

struct S {
    static const int x = 0;  // 声明，无需外部定义
    static int y;            // 声明，需要外部定义
};

int S::y = 0;  // 定义

const int& f(const int& r);

int main() {
    // S::x 不被 ODR-use：值是编译时常量
    int a = S::x;
    int b = S::x + 1;

    // S::x 被 ODR-use：需要绑定引用
    const int& ref = S::x;  // 需要 S::x 的定义！

    // S::y 被 ODR-use：需要定义
    int c = S::y;

    std::cout << a << b << c << '\n';
    return 0;
}

// 如果 S::x 被 ODR-use，需要添加：
// const int S::x;  // 定义
```

### 6.3 内联变量 (C++17)

```cpp
// header.hpp
struct Config {
    // C++17: 内联静态数据成员
    inline static int version = 1;

    // C++17 前: 需要 extern 声明 + 源文件定义
    static int old_style;
};

// C++17: 内联变量
inline int global_config = 42;

// source.cpp
int Config::old_style = 0;  // 传统方式需要单独定义
```

### 6.4 ODR 违规示例

```cpp
// file1.cpp
struct S {
    int x;
};

// file2.cpp
struct S {
    int y;  // 不同的定义！
};

// 链接时行为未定义！
// 解决方案：使用头文件或命名空间
```

### 6.5 模板 ODR

```cpp
// header.hpp
template<typename T>
class Container {
public:
    void add(const T& item);

    // 定义在头文件中
    T get(int index) const {
        return data[index];
    }

private:
    T data[100];
};

// 声明可以在头文件
template<typename T>
void Container<T>::add(const T& item) {
    // 实现
}

// source.cpp: 显式实例化
template class Container<int>;
```

### 6.6 常见错误及修正

#### 错误示例 1：头文件中的非内联变量

```cpp
// 错误：header.hpp
int global_var = 42;  // 每个包含此头文件的翻译单元都会定义！

void func() {
    // 非内联函数在头文件中
}
```

**正确做法**：

```cpp
// header.hpp
extern int global_var;  // 声明
inline void func() {    // 内联函数
    // 实现
}

// source.cpp
int global_var = 42;  // 定义
```

#### 错误示例 2：静态常量成员未定义就 ODR-use

```cpp
// 错误
struct S {
    static const int SIZE = 100;
};

const int& get_size() {
    return S::SIZE;  // ODR-use！但没有定义
}
```

**正确做法**：

```cpp
struct S {
    static const int SIZE = 100;
};

const int S::SIZE;  // 定义

const int& get_size() {
    return S::SIZE;  // 现在可以
}

// 或使用 C++17 内联：
struct S {
    inline static const int SIZE = 100;  // 定义
};
```

#### 错误示例 3：模板定义在源文件

```cpp
// 错误：header.hpp
template<typename T>
T add(T a, T b);  // 仅声明

// source.cpp
template<typename T>
T add(T a, T b) {  // 定义
    return a + b;
}

// main.cpp
#include "header.hpp"
int main() {
    add(1, 2);  // 链接错误！模板定义不可见
}
```

**正确做法**：

```cpp
// header.hpp
template<typename T>
T add(T a, T b) {  // 定义在头文件
    return a + b;
}
```

#### 错误示例 4：不同翻译单元中类定义不同

```cpp
// file1.cpp
struct Data {
    int x;
    int y;
};

// file2.cpp
struct Data {
    int y;  // 顺序不同！
    int x;
};

// ODR 违规，行为未定义
```

**正确做法**：

```cpp
// data.hpp
struct Data {
    int x;
    int y;
};

// file1.cpp 和 file2.cpp 都包含 data.hpp
```

## 7. 总结 (Summary)

### 7.1 核心要点

1. **定义 vs 声明**：
   - 定义完全定义实体
   - 声明仅声明实体存在

2. **ODR 规则**：
   - 翻译单元内：每个实体最多一个定义
   - 程序整体：非内联实体恰好一个定义
   - 内联/模板：允许跨翻译单元多定义（需一致）

3. **ODR-use**：
   - 决定何时需要定义
   - 对象：读取/写入/取地址/绑定引用
   - 函数：调用/取地址

### 7.2 声明 vs 定义速查

| 声明 | 是否为定义 |
|-----|-----------|
| `int x = 42;` | 是 |
| `extern int x;` | 否 |
| `extern int x = 42;` | 是 |
| `void f() {}` | 是 |
| `void f();` | 否 |
| `struct S { int x; };` | 是 |
| `struct S;` | 否 |
| `static int S::i;` | 是 |
| `class S { static int i; };` | 否 |

### 7.3 与 C 的差异

| 特性 | C++ | C |
|-----|-----|---|
| 类型 ODR | 严格要求标记相同 | 无程序范围类型 ODR |
| `const` 全局变量 | 内部链接 | 外部链接 |
| 内联函数 | `inline` 关键字 | `static` 或 `inline` |

### 7.4 学习建议

1. **理解声明与定义区别**：
   - 声明告诉编译器存在
   - 定义告诉编译器是什么

2. **遵循 ODR**：
   - 头文件放声明和内联定义
   - 源文件放非内联定义

3. **使用现代 C++ 特性**：
   - `inline` 变量 (C++17)
   - `constexpr` 隐式内联
   - 模块 (C++20)

4. **避免常见错误**：
   - 头文件中非内联变量/函数
   - 不同翻译单元中不一致的定义
   - ODR-use 静态常量成员但未定义

---

**参考资源**：
- ISO/IEC 14882:2024 (C++23 标准) 6.3
- 内联说明符 (`inline`)
- 模板实例化
- 链接 (Linkage)