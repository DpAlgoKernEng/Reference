# 函数模板 (Function Template)

## 1. 概述

函数模板（Function Template）是 C++ 中泛型编程的核心机制之一，它定义了一族函数。通过函数模板，可以编写与类型无关的代码，由编译器根据实际使用类型自动生成具体函数实例。

函数模板允许程序员创建一个通用的函数定义，该定义可以用于多种不同的数据类型，而无需为每种类型单独编写重复代码。编译器在遇到模板调用时，会根据传入的参数类型自动实例化（instantiate）相应的函数版本。

函数模板定义在 `template` 关键字之后，包含模板参数列表和函数声明。

## 2. 来源与演变

### 历史背景

函数模板的概念源于 C++ 对泛型编程（Generic Programming）的支持需求。在模板引入之前，程序员需要为每种数据类型编写单独的函数版本，或者使用宏（macro）来实现类似功能，但宏缺乏类型安全性。

### C++98 引入

函数模板在 **C++98** 标准中首次引入，提供了：
- 类型安全的泛型函数定义
- 编译期类型推导
- 自动实例化机制

### C++11 变化

- 引入**变参模板**（Variadic Templates），支持模板参数包
- 引入 `extern template` 显式实例化声明，避免重复实例化
- 支持模板参数的默认参数

### C++20 变化

- 引入**约束**（Constraints）：可在模板参数列表后使用 `requires` 子句
- 引入**简写函数模板**（Abbreviated Function Templates）：使用 `auto` 占位符声明模板参数
- 移除了 `export` 关键字（C++11 已废弃）

### 废弃特性

| 特性 | 版本 | 说明 |
|------|------|------|
| `export template` | C++98 引入，C++11 移除 | 声明导出模板，允许跨编译单元实例化；实现复杂且互不兼容 |

## 3. 语法与参数

### 基本语法

```cpp
// (1) 基本形式
template < parameter-list > function-declaration

// (2) 带约束的形式 (C++20 起)
template < parameter-list > requires constraint function-declaration

// (3) 简写函数模板 (C++20 起)
function-declaration-with-placeholders

// (4) 导出模板 (已移除)
export template < parameter-list > function-declaration  // C++11 移除
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `parameter-list` | 非空的逗号分隔模板参数列表，每个参数可以是：非类型参数、类型参数、模板参数、参数包（C++11 起） |
| `function-declaration` | 函数声明，声明的函数名成为模板名 |
| `constraint` | 约束表达式，限制此函数模板接受的模板参数（C++20 起） |
| `function-declaration-with-placeholders` | 至少一个参数使用 `auto` 或 `Concept auto` 占位符的函数声明（C++20 起） |

### 简写函数模板 (C++20 起)

当函数声明的参数列表中使用占位符类型（`auto` 或 `Concept auto`）时，该声明实际上声明了一个函数模板：

```cpp
void f1(auto);                          // 等价于 template<class T> void f1(T)
void f2(C1 auto);                       // 等价于 template<C1 T> void f2(T)，若 C1 是概念
void f3(C2 auto...);                    // 等价于 template<C2... Ts> void f3(Ts...)
void f4(const C3 auto*, C4 auto&);      // 等价于 template<C3 T, C4 U> void f4(const T*, U&)
template<class T, C U>
void g(T x, U y, C auto z);             // 等价于 template<class T, C U, C W> void g(T x, U y, W z)
```

### 函数模板签名

每个函数模板都有一个签名，包含：
- 函数名
- 参数类型列表
- 返回类型
- 尾置 `requires` 子句（C++20 起）
- 模板头签名（模板参数列表，不含参数名和默认参数）

如果函数模板是类成员，其签名还包含：
- 所属类（而非命名空间）
- 尾置 `requires` 子句（C++20 起）
- ref-qualifier（如有）
- cv-限定符（C++11 起）

## 4. 底层原理

### 模板实例化

函数模板本身不是类型或函数，源文件中只有模板定义时不会生成任何代码。必须通过**实例化**（Instantiation）才能生成实际函数。

#### 显式实例化

```cpp
// (1) 显式实例化定义（无模板参数推导）
template return-type name < argument-list > ( parameter-list );

// (2) 显式实例化定义（带模板参数推导）
template return-type name ( parameter-list );

// (3) 显式实例化声明（C++11 起）
extern template return-type name < argument-list > ( parameter-list );

// (4) 显式实例化声明（带推导，C++11 起）
extern template return-type name ( parameter-list );
```

**示例：**

```cpp
template<typename T>
void f(T s) {
    std::cout << s << '\n';
}

template void f<double>(double);  // 实例化 f<double>(double)
template void f<>(char);          // 实例化 f<char>(char)，参数推导
template void f(int);             // 实例化 f<int>(int)，参数推导
```

**显式实例化声明（`extern template`）**：阻止隐式实例化，使用其他编译单元提供的显式实例化定义。

#### 隐式实例化

当代码引用函数且需要函数定义存在时，如果该函数尚未显式实例化，则发生隐式实例化：

```cpp
#include <iostream>

template<typename T>
void f(T s) {
    std::cout << s << '\n';
}

int main() {
    f<double>(1);        // 实例化并调用 f<double>(double)
    f<>('a');             // 实例化并调用 f<char>(char)
    f(7);                 // 实例化并调用 f<int>(int)
    void (*pf)(std::string) = f;  // 实例化 f<string>(string)
    pf("∇");              // 调用 f<string>(string)
}
```

### 模板参数推导

实例化函数模板时，模板参数必须已知，但不一定都要显式指定。编译器可以从函数参数推导缺失的模板参数：

```cpp
template<typename To, typename From>
To convert(From f);

void g(double d) {
    int i = convert<int>(d);        // 调用 convert<int,double>(double)
    char c = convert<char>(d);      // 调用 convert<char,double>(double)
    int(*ptr)(float) = convert;     // 实例化 convert<int, float>(float)
}
```

**推导时机**：
- 函数调用表达式
- 取函数模板地址
- 初始化函数引用
- 形成成员函数指针

### 模板参数替换

当所有模板参数确定后（指定、推导或默认参数），函数参数列表中的每个模板参数都会被替换为对应实参。

**SFINAE 原则**：替换失败（Substitution Failure）不是错误，只是将该函数模板从重载集中移除。这允许使用模板元编程技术操纵重载集。

**类型调整**：替换后，数组类型和函数类型的参数调整为指针，顶层 cv-限定符被丢弃：

```cpp
template<class T>
void f(T t);

template<class X>
void g(const X x);

f<int>(1);        // 函数类型 void(int)，t 是 int
f<const int>(1);  // 函数类型 void(int)，t 是 const int

g<int>(1);        // 函数类型 void(int)，x 是 const int
g<const int>(1);  // 函数类型 void(int)，x 是 const int
```

### 重载解析与偏序

当多个函数模板特化匹配时，编译器执行**偏序**（Partial Ordering）选择最匹配的版本。

**偏序规则**（非正式）："A 比 B 更特化"意味着"A 接受的类型比 B 少"。

**偏序过程**：
1. 为每个模板参数生成唯一的虚构类型/值/模板
2. 使用变换后的模板作为实参模板，另一模板作为形参模板进行推导
3. 双向比较，确定哪个更特化

### 函数重载 vs 函数特化

**重要规则**：只有非模板函数和主模板参与重载解析，特化版本不是重载。

```cpp
template<class T> void f(T);      // #1：所有类型的重载
template<> void f(int*);          // #2：#1 的特化（int 指针）
template<class T> void f(T*);     // #3：所有指针类型的重载

f(new int(1));  // 调用 #3，虽然 #2 是完美匹配，但 #3 在重载时更优先被选中
```

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 泛型算法 | 如排序、查找，适用于多种容器和元素类型 |
| 类型无关操作 | 如 `std::swap`、`std::max`、`std::min` |
| 运算符重载 | 无法显式指定模板参数的运算符 |
| 回调函数 | 接受不同签名的函数对象 |
| 容器适配器 | 如 `std::sort` 接受不同迭代器类型 |

### 最佳实践

#### 1. 模板定义放在头文件

模板定义通常应放在头文件中，因为编译器需要看到完整定义才能实例化：

```cpp
// ❌ 错误：分离声明和定义
// header.h
template<typename T> void f(T);

// source.cpp
template<typename T> void f(T) { /* 定义 */ }  // 其他编译单元无法实例化

// ✅ 正确：定义放在头文件
// header.h
template<typename T> void f(T) { /* 定义 */ }
```

#### 2. 使用显式实例化减少编译时间

对于大型模板，可在一个编译单元显式实例化：

```cpp
// template_impl.cpp
#include "my_template.h"
template class MyClass<int>;
template class MyClass<double>;
```

#### 3. 使用 `extern template` 避免重复实例化

```cpp
// header.h
template<typename T> void f(T);
extern template void f<int>(int);  // 声明，不在此实例化

// source.cpp
template<typename T> void f(T) { /* 定义 */ }
template void f<int>(int);         // 显式实例化定义
```

### 常见陷阱

#### 1. 重载与特化的混淆

```cpp
// ❌ 错误理解：特化参与重载
template<class T> void f(T);      // #1
template<> void f(int*);          // 特化 #1
template<class T> void f(T*);     // #2

f(new int(1));  // 调用 #2，而非特化版本！
```

#### 2. 函数模板不支持偏特化

```cpp
// ❌ 错误：函数模板没有偏特化
template<typename T> void f(T);
template<typename T> void f<T*>;  // 编译错误！

// ✅ 正确：使用重载
template<typename T> void f(T);
template<typename T> void f(T*);   // 重载，非偏特化
```

#### 3. 声明顺序影响特化

```cpp
template<class T> void f(T);      // #1
template<> void f<>(int*);        // #2：特化 #1
template<class T> void f(T*);     // #3：#2 不特化此模板
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <string>

// 基本函数模板
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// 多个模板参数
template<typename T, typename U>
void printPair(T first, U second) {
    std::cout << "(" << first << ", " << second << ")" << std::endl;
}

int main() {
    // 隐式类型推导
    std::cout << max(3, 5) << std::endl;         // int 版本
    std::cout << max(3.14, 2.71) << std::endl;   // double 版本

    // 显式指定模板参数
    std::cout << max<double>(3, 5.5) << std::endl;  // 强制使用 double

    // 多模板参数
    printPair(1, "hello");
    printPair(3.14, 42);

    return 0;
}
```

### 高级用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// 非类型模板参数
template<typename T, int Size>
class FixedArray {
    T data[Size];
public:
    int size() const { return Size; }
    T& operator[](int i) { return data[i]; }
    const T& operator[](int i) const { return data[i]; }
};

// 模板参数推导 (C++17)
template<typename T>
struct Container {
    T value;
    Container(T v) : value(v) {}
};

// C++20 约束
#include <concepts>
template<std::integral T>
T add(T a, T b) {
    return a + b;
}

// C++20 简写函数模板
void process(auto x, auto y) {
    std::cout << x << ", " << y << std::endl;
}

// 变参模板 (C++11)
template<typename... Args>
void printAll(Args... args) {
    (std::cout << ... << args) << std::endl;  // C++17 折叠表达式
}

int main() {
    // 非类型模板参数
    FixedArray<int, 5> arr;
    arr[0] = 10;
    std::cout << "Size: " << arr.size() << std::endl;

    // CTAD (C++17)
    Container c(42);  // Container<int>
    std::cout << c.value << std::endl;

    // C++20 约束
    std::cout << add(1, 2) << std::endl;  // OK
    // add(1.5, 2.5);  // 编译错误：double 不满足 integral

    // C++20 简写
    process(1, "hello");

    // 变参模板
    printAll(1, 2.0, "three");

    return 0;
}
```

### 常见错误及修正

#### 错误 1：特化声明顺序问题

```cpp
// ❌ 错误：特化在错误的主模板之后
template<class T> void f(T);       // #1
template<> void f<>(int*);         // 特化 #1
template<class T> void f(T*);     // #2

void g() {
    f(new int(1));  // 调用 #2，而非特化！
}

// ✅ 修正：正确的声明顺序
template<class T> void f(T);       // #1
template<class T> void f(T*);      // #2
template<> void f<>(int*);         // 特化 #2

void g() {
    f(new int(1));  // 现在调用特化版本
}
```

#### 错误 2：忘记模板定义

```cpp
// ❌ 错误：分离声明和定义
// header.h
template<typename T> T add(T a, T b);  // 仅声明

// impl.cpp
template<typename T> T add(T a, T b) { return a + b; }  // 其他编译单元找不到

// ✅ 修正：定义放在头文件
// header.h
template<typename T> T add(T a, T b) { return a + b; }
```

#### 错误 3：函数模板偏特化

```cpp
// ❌ 错误：尝试偏特化函数模板
template<typename T>
void func(T t) { std::cout << "General\n"; }

template<typename T>
void func<T*>(T* p) { std::cout << "Pointer\n"; }  // 编译错误！

// ✅ 修正：使用重载
template<typename T>
void func(T t) { std::cout << "General\n"; }

template<typename T>
void func(T* p) { std::cout << "Pointer\n"; }  // 重载，正确
```

#### 错误 4：重载歧义

```cpp
// ❌ 错误：歧义调用
template<class T>
void g(T, T = T());   // #1
template<class T, class... U>
void g(T, U...);      // #2

void h() {
    g(42);  // 歧义：#1 和 #2 都匹配
}

// ✅ 修正：显式指定模板参数
g<int>(42);           // 调用 #1
g<int, >(42);         // 调用 #2（空包）
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 定义方式 | `template<参数列表> 函数声明` |
| 实例化 | 显式实例化或隐式实例化 |
| 参数推导 | 编译器自动推导未指定的模板参数 |
| 重载 | 支持与非模板函数和多个模板重载 |
| 特化 | 仅支持全特化，不支持偏特化 |

### 版本特性对比

| 版本 | 新增特性 |
|------|---------|
| C++98 | 基本函数模板、显式实例化 |
| C++11 | 变参模板、`extern template`、参数包 |
| C++17 | 类模板参数推导（CTAD） |
| C++20 | 约束、简写函数模板、Concept auto |

### 关键概念对比

| 概念 | 说明 |
|------|------|
| 函数模板 | 定义函数族的模板 |
| 模板特化 | 为特定类型提供定制实现 |
| 函数重载 | 定义同名但参数不同的多个函数 |
| 模板实例化 | 编译器生成具体函数的过程 |

### 学习建议

1. **理解实例化机制**：模板代码在实例化时才生成，定义通常放在头文件
2. **区分重载与特化**：特化不参与重载解析，主模板被选中后才考虑特化
3. **掌握 SFINAE**：替换失败不是错误，是模板元编程的基础
4. **了解 C++20 简写语法**：`auto` 参数使模板更简洁
5. **注意声明顺序**：特化所属的主模板由声明位置决定

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/function_template
- C++ Standard: [temp.fct]
- "C++ Templates: The Complete Guide" by David Vandevoorde et al.
- "Effective C++" by Scott Meyers, Item 46