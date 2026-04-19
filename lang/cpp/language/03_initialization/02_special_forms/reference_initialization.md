# 引用初始化 (Reference Initialization)

## 1. 概述

引用初始化（Reference Initialization）是将引用绑定到对象的过程。引用是 C++ 中的一种复合类型，它为已存在的对象提供了一个别名。一旦初始化完成，引用就无法重新绑定到另一个对象。

引用可以绑定到：
- 类型为 `T` 的对象
- 类型为 `T` 的函数
- 可隐式转换为 `T` 类型的对象

C++ 中有两种引用类型：
- **左值引用（lvalue reference）**：使用 `T&` 声明，绑定到左值
- **右值引用（rvalue reference）**：使用 `T&&` 声明（C++11 起），绑定到右值或通过 `std::move` 转换的左值

引用初始化发生在以下场景：
1. 声明具名左值引用变量时
2. 声明具名右值引用变量时（C++11 起）
3. 函数调用时，参数为引用类型
4. 函数返回语句中，返回类型为引用类型
5. 使用成员初始化器初始化引用类型的非静态数据成员时

## 2. 来源与演变

### C++98：引用的基础

引用在 C++ 早期版本中就已存在，主要用于：
- 避免对象复制的函数参数传递
- 运算符重载的支持
- 创建对象的别名

C++98 标准定义了左值引用的初始化规则，包括直接绑定和间接绑定两种方式。

### C++11：右值引用引入

C++11 引入了右值引用（`T&&`），这是 C++ 语言发展中的重要里程碑：

- 支持移动语义（move semantics），实现资源的高效转移
- 支持完美转发（perfect forwarding）
- 新增列表初始化语法支持引用

### C++17：临时量实质化

C++17 引入了临时量实质化转换（temporary materialization conversion）的概念，明确了纯右值（prvalue）到泛左值（glvalue）的转换过程。这统一了临时对象创建的语义模型。

### C++20：指派初始化器

C++20 支持使用指派初始化器（designated initializers）进行引用的列表初始化。

### C++26：悬空引用检测

C++26 规定：如果函数返回语句中返回的引用绑定到临时表达式的结果，程序是不合法的（ill-formed），这将帮助编译器检测悬空引用问题。

### 重要缺陷报告

| DR 编号 | 适用版本 | 原行为 | 修正后行为 |
|---------|----------|--------|------------|
| CWG 391 | C++98 | 用类类型右值初始化 const 引用可能创建临时对象 | 不创建临时对象 |
| CWG 589 | C++98 | 引用不能直接绑定到数组或类右值 | 允许绑定 |
| CWG 656 | C++98 | 间接绑定时的用户定义转换结果处理不正确 | 直接绑定到转换结果 |
| CWG 1295 | C++11 | 引用可以绑定到位域 xvalue | 禁止绑定 |
| CWG 2481 | C++17 | 临时量实质化未添加 cv 限定 | 添加 cv 限定 |

## 3. 语法与参数

### 非列表初始化语法

#### 左值引用初始化

```cpp
T& ref = target;    // 复制初始化
T& ref(target);    // 直接初始化
```

#### 右值引用初始化（C++11 起）

```cpp
T&& ref = target;  // 复制初始化
T&& ref(target);   // 直接初始化
```

#### 函数参数引用绑定

```cpp
void func(T& param);        // 左值引用参数
void func(T&& param);       // 右值引用参数
func(target);               // 调用时绑定
```

#### 函数返回引用

```cpp
T& func() {
    return target;          // 返回引用
}
```

#### 成员初始化器

```cpp
class Class {
    T& ref_member;
public:
    Class(...): ref_member(target) { ... }
};
```

### 列表初始化语法（C++11 起）

#### 普通列表初始化

```cpp
T& ref = {arg1, arg2, ...};
T& ref{arg1, arg2, ...};
T&& ref = {arg1, arg2, ...};
T&& ref{arg1, arg2, ...};
func({arg1, arg2, ...});
```

#### 指派列表初始化（C++20 起）

```cpp
T& ref = {.des1 = arg1, .des2{arg2} ...};
T& ref{.des1 = arg1, .des2{arg2} ...};
T&& ref = {.des1 = arg1, .des2{arg2} ...};
T&& ref{.des1 = arg1, .des2{arg2} ...};
func({.des1 = arg1, .des2{arg2} ...});
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `T` | 被引用的类型 |
| `ref` | 要初始化的引用变量 |
| `target` | 初始化表达式 |
| `func-refpar` | 带引用类型参数的函数 |
| `func-refret` | 返回引用类型的函数 |
| `Class` | 类名 |
| `ref-member` | 引用类型的非静态数据成员 |
| `des1, des2, ...` | 指派符 |
| `arg1, arg2, ...` | 初始化列表中的初始值 |

### 核心定义

#### 引用相关（Reference-Related）

对于两个类型 `T1` 和 `T2`，设它们的 cv 无限定版本分别为 `U1` 和 `U2`，如果满足以下任一条件，则 `T1` 与 `T2` 是**引用相关**的：
- `U1` 与 `U2` 相似（similar）
- `U1` 是 `U2` 的基类

#### 引用兼容（Reference-Compatible）

如果指向 `T2` 类型的纯右值可以通过标准转换序列转换为指向 `T1` 类型的指针，则 `T1` 与 `T2` 是**引用兼容**的。

## 4. 底层原理

### 绑定机制

引用初始化遵循以下绑定规则：

1. **直接绑定（Direct Binding）**：引用直接绑定到目标对象或其基类子对象
2. **间接绑定（Indirect Binding）**：引用绑定到从目标转换或创建的临时对象

编译器首先尝试直接绑定，如果不可行则尝试间接绑定。如果两种绑定方式都不可用，程序不合法。

### 直接绑定规则

#### 左值引用直接绑定

当满足以下所有条件时，左值引用直接绑定：

```cpp
// 条件：引用为左值引用 + target 为非位域左值 + T 与 U 引用兼容
double d = 2.0;
double& rd = d;        // rd 绑定到 d
const double& rcd = d; // rcd 绑定到 d

struct A {};
struct B : A {} b;
A& ra = b;             // ra 绑定到 b 中的 A 子对象
const A& rca = b;      // rca 绑定到 b 中的 A 子对象
```

#### 转换函数后的直接绑定

```cpp
struct A {};
struct B : A {
    operator int&() { return n; }
    int n;
};

int& ir = B();  // ir 绑定到 B::operator int&() 的结果
```

#### 右值引用直接绑定（C++11 起）

```cpp
struct A {};
struct B : A {};
extern B f();

const A& rca2 = f();  // 绑定到 B 右值的 A 子对象
A&& rra = f();        // 同上

int i2 = 42;
int&& rri = static_cast<int&&>(i2);  // 直接绑定到 i2
```

### 间接绑定规则

当直接绑定不可用时，编译器考虑间接绑定：

```cpp
const std::string& rs = "abc";  // rs 绑定到从字符数组复制初始化的临时对象
const double& rcd2 = 2;         // rcd2 绑定到值为 2.0 的临时对象
int i3 = 2;
double&& rrd3 = i3;             // rrd3 绑定到值为 2.0 的临时对象
```

### 临时量实质化（C++17 起）

从 C++17 开始，当纯右值需要绑定到引用时，会进行临时量实质化转换：

```cpp
const int& r = 42;  // 42 是纯右值，实质化为临时对象，r 绑定到该临时对象
```

临时对象的类型会被调整为添加目标引用的 cv 限定符。

### 类型转换优先级

1. 如果 `T` 或 `U` 是类类型，考虑用户定义的转换函数
2. 否则，创建 `T` 类型的临时对象，从 target 复制初始化
3. 引用绑定到该临时对象

## 5. 使用场景

### 适用场景

| 场景 | 推荐引用类型 | 说明 |
|------|-------------|------|
| 函数参数传递（只读） | `const T&` | 避免复制，同时禁止修改 |
| 函数参数传递（需要修改） | `T&` | 允许函数修改实参 |
| 实现移动语义 | `T&&` | C++11 起，转移资源所有权 |
| 范围 for 循环 | `const auto&` 或 `auto&` | 避免元素复制 |
| 运算符重载 | `T&` 或 `T&&` | 支持链式调用 |

### 临时对象生命周期延长

当引用绑定到临时对象时，临时对象的生命周期会延长到与引用相同：

```cpp
const std::string& s = std::string("hello");  // 临时对象生命周期延长
// s 在此作用域内有效
```

### 生命周期延长的例外情况

| 情况 | 说明 |
|------|------|
| 函数返回引用绑定临时对象 | 临时对象在返回语句结束时销毁（C++26 前为悬空引用） |
| 函数调用中的引用参数 | 临时对象存活到完整表达式结束 |
| new 表达式中的引用成员 | 临时对象存活到完整表达式结束，而非对象生命周期 |
| 聚合类直接初始化（括号）中的引用成员 | 临时对象存活到完整表达式结束（C++20 起） |

### 注意事项

#### 悬空引用

```cpp
// 错误示例：返回局部变量的引用
int& bad_func() {
    int local = 42;
    return local;  // 悬空引用！
}

// 错误示例：返回绑定到临时的引用
const int& also_bad() {
    return 42;  // 悬空引用！临时对象在函数返回时销毁
}
```

#### 引用成员与聚合初始化

```cpp
struct A {
    int&& r;
};

A a1{7};   // OK：临时对象 7 的生命周期延长
A a2(7);   // 合法但危险：a2.r 是悬空引用（C++20 起）
```

#### new 表达式中的引用成员

```cpp
struct S {
    const std::pair<int, int>& mp;
};

S* p = new S{1, {2, 3}};
// 临时 pair {2, 3} 的生命周期在分号处结束
// p->mp 是悬空引用！
delete p;
```

### 最佳实践

1. **优先使用 `const T&` 作为只读函数参数**
2. **避免返回局部变量或临时对象的引用**
3. **注意引用成员的生命周期管理**
4. **使用 `std::move` 显式转换左值为右值引用**
5. **在范围 for 循环中使用引用避免复制**

## 6. 代码示例

### 基础用法：左值引用

```cpp
#include <iostream>

int main() {
    // 左值引用绑定到对象
    int n = 1;
    int& r1 = n;           // r1 是 n 的别名
    const int& cr(n);      // 引用可以更加 cv 限定
    volatile int& cv{n};   // 任何初始化语法都可以使用
    int& r2 = r1;          // 另一个绑定到 n 的左值引用

    // int& bad = cr;      // 错误：cv 限定更少
    int& r3 = const_cast<int&>(cr);  // 需要 const_cast

    // 引用绑定到函数
    void (&rf)(int) = foo;

    // 引用绑定到数组
    int ar[3];
    int (&ra)[3] = ar;

    // 引用绑定到基类子对象
    struct A {};
    struct B : A { int n; operator int&() { return n; } };
    B b;
    A& base_ref = b;        // 引用绑定到基类子对象
    int& converted_ref = b; // 引用绑定到转换结果

    return 0;
}
```

### 基础用法：右值引用（C++11 起）

```cpp
#include <iostream>
#include <utility>

struct A {};
struct B : A {};

B bar() { return B(); }

int main() {
    // 右值引用绑定到右值
    // int& bad = 1;        // 错误：左值引用不能绑定到右值
    const int& cref = 1;     // const 左值引用可以绑定到右值
    int&& rref = 1;          // 右值引用绑定到右值

    // 右值引用绑定到临时对象
    const A& cref2 = bar();  // 引用绑定到 B 临时对象的 A 子对象
    A&& rref2 = bar();       // 同上

    // 右值引用绑定到转换后的左值
    int n = 42;
    int&& xref = static_cast<int&&>(n);  // 直接绑定到 n
    // int&& copy_ref = n;  // 错误：不能绑定到左值
    double&& copy_ref = n;   // 绑定到值为 42.0 的临时对象

    return 0;
}
```

### 高级用法：移动语义

```cpp
#include <iostream>
#include <utility>
#include <vector>

class Buffer {
private:
    int* data_;
    size_t size_;

public:
    // 构造函数
    Buffer(size_t size) : data_(new int[size]), size_(size) {}

    // 移动构造函数
    Buffer(Buffer&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 移动赋值运算符
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    ~Buffer() { delete[] data_; }
};

int main() {
    Buffer b1(100);
    Buffer b2(std::move(b1));  // 移动构造：b1 被转移资源

    Buffer b3(200);
    b3 = Buffer(300);          // 移动赋值

    return 0;
}
```

### 高级用法：完美转发

```cpp
#include <iostream>
#include <utility>

// 接收任意参数并完美转发
template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));
}

void process(int& x) {
    std::cout << "左值引用版本: " << x << std::endl;
}

void process(int&& x) {
    std::cout << "右值引用版本: " << x << std::endl;
}

int main() {
    int value = 42;
    wrapper(value);              // 调用左值版本
    wrapper(100);                // 调用右值版本
    wrapper(std::move(value));   // 调用右值版本

    return 0;
}
```

### 常见错误及修正

#### 错误 1：返回局部变量的引用

```cpp
// 错误：返回局部变量的引用
int& bad_func() {
    int local = 42;
    return local;  // 悬空引用！
}

// 修正：返回值而非引用
int good_func() {
    int local = 42;
    return local;  // 返回值的副本
}

// 或：传递引用参数
void better_func(int& result) {
    result = 42;
}
```

#### 错误 2：悬空引用问题

```cpp
#include <sstream>

// 错误：返回绑定到临时的引用
std::ostream& bad() {
    return std::ostringstream() << 'a';
    // ostringstream 临时对象在分号处销毁
    // 返回的引用是悬空的
}

// 修正：返回对象
std::string good() {
    std::ostringstream oss;
    oss << 'a';
    return oss.str();
}
```

#### 错误 3：聚合初始化中的引用成员

```cpp
struct S {
    int&& r;  // 右值引用成员
};

// OK：使用花括号列表初始化，临时对象生命周期延长
S a1{7};

// 危险：使用括号直接初始化，临时对象生命周期不延长（C++20 起）
S a2(7);  // a2.r 是悬空引用！

// 修正：避免在聚合类中使用引用成员，或始终使用列表初始化
```

#### 错误 4：new 表达式中的引用成员

```cpp
struct S {
    const std::pair<int, int>& mp;
};

// 危险：临时对象生命周期在完整表达式结束时结束
S* p = new S{1, {2, 3}};
// p->mp 是悬空引用！

// 修正：避免在动态分配的对象中使用引用成员
// 或使用指针成员
struct S2 {
    std::pair<int, int>* mp;
};
```

## 7. 总结

### 核心要点

1. **引用是别名**：引用一旦初始化就无法重新绑定，它始终是所绑定对象的别名

2. **绑定规则**：
   - 左值引用 `T&` 绑定到左值（或通过 const_cast 转换）
   - `const T&` 可以绑定到右值（临时对象生命周期延长）
   - 右值引用 `T&&` 绑定到右值或通过 `std::move` 转换的左值

3. **直接绑定 vs 间接绑定**：
   - 直接绑定：引用直接绑定到目标对象或其基类子对象
   - 间接绑定：引用绑定到从目标创建的临时对象

4. **生命周期延长**：绑定到临时对象的引用会延长临时对象的生命周期，但有例外情况

### 引用类型对比

| 引用类型 | 可绑定的值类别 | 典型用途 |
|----------|---------------|----------|
| `T&` | 左值 | 输出参数、修改实参 |
| `const T&` | 左值、右值 | 输入参数、避免复制 |
| `T&&` | 右值 | 移动语义、完美转发 |
| `const T&&` | 右值 | 较少使用 |

### 学习建议

1. **理解值类别**：掌握左值、纯右值、亡值的概念
2. **掌握生命周期**：理解临时对象何时创建和销毁
3. **警惕悬空引用**：始终关注引用所绑定对象的生命周期
4. **善用右值引用**：实现高效的资源转移和完美转发

### 相关概念

| 概念 | 关系 |
|------|------|
| 值类别 | 左值、纯右值、亡值的分类 |
| 移动语义 | 基于右值引用的资源转移 |
| 完美转发 | 保持值类别的参数转发 |
| 复制省略 | 编译器优化避免不必要的复制 |
| 临时量实质化 | C++17 引入的纯右值到泛左值转换 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/reference_initialization
- C++ Standard: [dcl.init.ref]
- Effective Modern C++, Scott Meyers, Item 24-30