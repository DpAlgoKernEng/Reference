# 复制赋值运算符（Copy Assignment Operator）

## 1. 概述 (Overview)

**复制赋值运算符**（copy assignment operator）是一种特殊的非模板非静态成员函数，名称为 `operator=`，可以使用相同类类型的参数调用，并将参数的内容复制到当前对象，而不会改变参数对象本身。

复制赋值运算符是 C++ 中实现对象复制语义的核心机制之一，与复制构造函数（copy constructor）共同定义了类对象的复制行为。当对象出现在赋值表达式左侧时，编译器会通过重载决议选择调用复制赋值运算符。

### 核心特征

| 特征 | 说明 |
|------|------|
| 名称 | `operator=` |
| 类型 | 非模板非静态成员函数 |
| 参数 | 同类型对象（`T`、`T&`、`const T&`、`volatile T&` 或 `const volatile T&`） |
| 行为 | 复制内容，不修改参数 |
| 返回值 | 通常为 `T&` 以支持链式赋值 |

## 2. 来源与演变 (Origin and Evolution)

### C++98 首次引入

复制赋值运算符自 C++98 标准起就作为核心语言特性存在。它是 C++ 实现值语义（value semantics）的关键组成部分，确保对象可以通过赋值操作进行复制。

### 设计动机

在 C++ 之前（C 语言时代）：

1. **结构体复制**：C 语言支持结构体的浅复制，但无法自定义复制行为
2. **资源管理困难**：涉及指针和动态内存时，浅复制会导致双重释放等问题

C++ 引入复制赋值运算符解决了这些问题：

- 允许类设计者定义"深复制"语义
- 支持资源获取即初始化（RAII）模式
- 与复制构造函数配合实现完整的值语义

### C++11 变化

- 新增 `= default` 语法，显式要求编译器生成默认实现
- 新增 `= delete` 语法，禁止生成复制赋值运算符
- 引入移动语义后，声明移动构造函数或移动赋值运算符会导致隐式声明的复制赋值运算符被定义为删除
- 如果类有用户声明的析构函数或复制构造函数，隐式定义的复制赋值运算符生成被弃用

### C++14 变化

- 隐式定义的复制赋值运算符在某些条件下可成为 `constexpr` 函数

### C++20 变化

- 引入"合格复制赋值运算符"（eligible copy assignment operator）概念，考虑约束条件

### C++23 变化

- 隐式定义的复制赋值运算符总是 `constexpr`

## 3. 语法与参数 (Syntax and Parameters)

### 声明语法

| 语法形式 | 说明 |
|---------|------|
| `return-type operator=(parameter-list);` | 类内声明（声明形式） |
| `return-type operator=(parameter-list) function-body` | 类内定义 |
| `return-type operator=(parameter-list-no-default) = default;` | 显式默认（C++11 起） |
| `return-type operator=(parameter-list) = delete;` | 显式删除（C++11 起） |
| `return-type class-name::operator=(parameter-list) function-body` | 类外定义 |

### 参数说明

| 参数 | 说明 |
|------|------|
| `class-name` | 声明复制赋值运算符的类名，下文描述中记为 `T` |
| `parameter-list` | 单参数列表，类型为 `T`、`T&`、`const T&`、`volatile T&` 或 `const volatile T&` |
| `parameter-list-no-default` | 无默认参数的单参数列表 |
| `function-body` | 函数体 |
| `return-type` | 任意类型，但推荐 `T&` 以支持链式赋值 |

### 有效参数类型

复制赋值运算符的参数必须是以下类型之一：

- `T`（传值）
- `T&`（左值引用）
- `const T&`（常量左值引用）
- `volatile T&`（volatile 左值引用）
- `const volatile T&`（const volatile 左值引用）

### 语法示例

```cpp
struct X
{
    X& operator=(X& other);     // 复制赋值运算符
    X operator=(X other);       // 传值方式是允许的
//  X operator=(const X other); // 错误：参数类型不正确
};

union Y
{
    // 复制赋值运算符可以有多种语法形式
    auto operator=(Y& other) -> Y&;       // OK：尾置返回类型
    Y& operator=(this Y& self, Y& other); // OK：显式对象参数（C++23）
//  Y& operator=(Y&, int num = 1);        // 错误：有其他非对象参数
};
```

### 返回值约定

虽然返回值可以是任意类型，但标准约定返回 `T&`（对当前对象的引用），原因：

1. **支持链式赋值**：`a = b = c` 能够正常工作
2. **一致性**：与内置类型的赋值行为一致
3. **条件表达式**：可用于条件语句中

## 4. 底层原理 (Underlying Principles)

### 隐式声明机制

如果类没有提供用户定义的复制赋值运算符，编译器会自动声明一个内联（inline）公有成员。隐式声明的形式取决于类的成员：

**条件判断流程**：

```
如果满足以下所有条件：
  - 每个直接基类 B 都有参数类型为 B、const B& 或 const volatile B& 的复制赋值运算符
  - 每个非静态数据成员 M 都有参数类型为 M、const M& 或 const volatile M& 的复制赋值运算符

则隐式声明为：T& T::operator=(const T&);

否则隐式声明为：T& T::operator=(T&);
```

### 隐式定义行为

如果隐式声明的复制赋值运算符未被删除且非平凡（non-trivial），编译器会在需要时（odr-used 或常量求值）生成函数体：

**对于非联合体类类型**：
1. 按初始化顺序对直接基类执行逐成员复制赋值
2. 按声明顺序对非静态数据成员执行逐成员复制赋值
3. 标量类型使用内置赋值
4. 数组类型执行逐元素复制赋值
5. 类类型成员调用其复制赋值运算符（非虚调用）

**对于联合体类型**：
- 复制对象表示（类似 `std::memmove`）

### 平凡复制赋值运算符

复制赋值运算符在以下条件下是**平凡的**（trivial）：

| 条件 | 说明 |
|------|------|
| 非用户提供 | 隐式定义或显式默认 |
| 无虚函数 | 类没有虚成员函数 |
| 无虚基类 | 类没有虚基类 |
| 基类平凡 | 每个直接基类的复制赋值运算符是平凡的 |
| 成员平凡 | 每个非静态类类型成员的复制赋值运算符是平凡的 |

**平凡复制赋值运算符的行为**：
- 类似 `std::memmove` 复制对象表示
- 所有与 C 语言兼容的数据类型（POD 类型）都是平凡可复制赋值的

### 删除的复制赋值运算符

隐式声明或显式默认的复制赋值运算符在以下情况下被定义为删除：

| 条件 | 说明 |
|------|------|
| const 成员 | 有 const 限定的非类类型非静态数据成员 |
| 引用成员 | 有引用类型的非静态数据成员 |
| 不可复制成员 | 潜在构造子对象没有可用的复制赋值运算符 |
| 变体成员 | 变体成员选择了非平凡函数 |
| 声明移动操作 | 声明了移动构造函数或移动赋值运算符（C++11 起） |

### 合格复制赋值运算符

| 标准 | 定义 |
|------|------|
| C++11 前 | 用户声明，或隐式声明且可定义 |
| C++11 - C++17 | 未被删除 |
| C++20 起 | 满足：未被删除、关联约束满足、无更受限的复制赋值运算符 |

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 值语义类 | 需要对象可复制的类 |
| 资源管理 | 管理动态内存、文件句柄等资源 |
| 深复制需求 | 需要复制指针指向的内容而非指针本身 |
| 链式赋值 | 支持 `a = b = c` 语法 |

### 最佳实践

#### 1. Copy-and-Swap 惯用法

```cpp
class A {
public:
    A& operator=(A other) {  // 传值，利用复制构造函数
        swap(*this, other);   // 交换资源
        return *this;
    }  // other 析构时自动释放旧资源
};
```

**优点**：
- 代码简洁
- 自赋值安全
- 异常安全（强异常保证）

#### 2. 传统写法与自赋值检查

```cpp
class C {
public:
    C& operator=(const C& other) {
        if (this != &other) {  // 自赋值检查
            // 复制资源
        }
        return *this;
    }
};
```

#### 3. 使用 = default

```cpp
struct SimpleClass {
    int x;
    double y;
    // 使用编译器生成的默认实现
    SimpleClass& operator=(const SimpleClass&) = default;
};
```

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|---------|
| 自赋值问题 | `a = a` 导致资源释放 | Copy-and-Swap 或显式检查 |
| 浅复制 | 指针成员只复制地址 | 实现深复制 |
| 异常不安全 | 分配失败后对象状态不一致 | Copy-and-Swap 惯用法 |
| 忘记返回 *this | 链式赋值失败 | 总是返回 `*this` |
| 虚基类多次赋值 | 虚基类可能被多次赋值 | 注意继承体系设计 |

### Rule of Three/Five/Zero

| 规则 | 说明 |
|------|------|
| Rule of Three | 如果定义了析构函数、复制构造函数或复制赋值运算符之一，通常需要定义全部三个 |
| Rule of Five | Rule of Three + 移动构造函数 + 移动赋值运算符 |
| Rule of Zero | 使用智能指针和标准容器，让编译器自动生成 |

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>
#include <string>

struct A
{
    int n;
    std::string s1;

    A() = default;
    A(A const&) = default;

    // 用户定义的复制赋值运算符（copy-and-swap 惯用法）
    A& operator=(A other)
    {
        std::cout << "copy assignment of A\n";
        std::swap(n, other.n);
        std::swap(s1, other.s1);
        return *this;
    }
};

struct B : A
{
    std::string s2;
    // 隐式定义的复制赋值运算符
};

int main()
{
    A a1, a2;
    std::cout << "a1 = a2 calls ";
    a1 = a2;  // 用户定义的复制赋值

    B b1, b2;
    b2.s1 = "foo";
    b2.s2 = "bar";
    std::cout << "b1 = b2 calls ";
    b1 = b2;  // 隐式定义的复制赋值

    std::cout << "b1.s1 = " << b1.s1 << "; b1.s2 = " << b1.s2 << '\n';
}
```

**输出**：
```
a1 = a2 calls copy assignment of A
b1 = b2 calls copy assignment of A
b1.s1 = foo; b1.s2 = bar
```

### 高级用法 - 资源管理

```cpp
#include <algorithm>
#include <memory>

struct C
{
    std::unique_ptr<int[]> data;
    std::size_t size;

    // 用户定义的复制赋值运算符（非 copy-and-swap 惯用法）
    // 注意：copy-and-swap 会总是重新分配资源
    C& operator=(const C& other)
    {
        if (this != &other)  // 非自赋值
        {
            if (size != other.size)  // 资源无法重用
            {
                data.reset(new int[other.size]);
                size = other.size;
            }
            std::copy(&other.data[0], &other.data[0] + size, &data[0]);
        }
        return *this;
    }
};
```

### 常见错误及修正

#### 错误 1：忘记处理自赋值

```cpp
// 错误：自赋值会导致资源释放后使用
class String {
    char* data;
public:
    String& operator=(const String& other) {
        delete[] data;                    // 如果 this == &other，other.data 已被删除
        data = new char[strlen(other.data) + 1];  // 使用已释放的内存！
        strcpy(data, other.data);
        return *this;
    }
};

// 修正：添加自赋值检查
class String {
    char* data;
public:
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            data = new char[strlen(other.data) + 1];
            strcpy(data, other.data);
        }
        return *this;
    }
};

// 更好的修正：使用 copy-and-swap
class String {
    char* data;
public:
    String& operator=(String other) {  // 传值，自动复制
        std::swap(data, other.data);
        return *this;
    }  // other 析构时自动释放旧资源
};
```

#### 错误 2：返回值类型错误

```cpp
// 错误：返回 void 导致链式赋值失败
class BadExample {
public:
    void operator=(const BadExample& other) {
        // ...
    }
};

BadExample a, b, c;
a = b = c;  // 编译错误！

// 修正：返回 *this 的引用
class GoodExample {
public:
    GoodExample& operator=(const GoodExample& other) {
        // ...
        return *this;
    }
};
```

#### 错误 3：const 或引用成员导致删除

```cpp
// 问题：const 和引用成员导致复制赋值运算符被删除
struct Problem {
    const int id;        // const 成员
    int& ref;            // 引用成员

    Problem(int i, int& r) : id(i), ref(r) {}
};

Problem p1(1, some_int);
Problem p2(2, another_int);
p1 = p2;  // 编译错误：复制赋值运算符被删除

// 解决方案：使用指针代替引用，移除 const
struct Solution {
    int id;              // 非 const
    int* ref;            // 指针代替引用

    Solution& operator=(const Solution& other) {
        id = other.id;
        ref = other.ref;
        return *this;
    }
};
```

## 7. 总结 (Summary)

### 核心要点

复制赋值运算符是 C++ 值语义的核心组成部分：

| 特性 | 说明 |
|------|------|
| **自动生成** | 编译器会为没有用户定义复制赋值运算符的类隐式声明 |
| **参数类型** | `T`、`T&`、`const T&` 等，推荐 `const T&` 或 copy-and-swap |
| **返回类型** | 推荐 `T&` 以支持链式赋值 |
| **平凡性** | 满足特定条件时为平凡，可使用 `std::memmove` |
| **删除条件** | const 成员、引用成员、声明移动操作等会导致删除 |

### 与相关概念对比

| 概念 | 说明 |
|------|------|
| 复制构造函数 | 从现有对象构造新对象 |
| 移动赋值运算符 | 从右值窃取资源 |
| 移动构造函数 | 从右值构造新对象 |
| 析构函数 | 释放资源 |

### 设计建议

1. **遵循 Rule of Zero**：优先使用智能指针和标准容器，避免手动资源管理
2. **使用 Copy-and-Swap**：实现简洁、异常安全、自赋值安全的复制赋值
3. **注意移动语义**：定义移动操作会导致复制赋值运算符被删除，需显式定义或 `= default`
4. **一致性**：复制赋值运算符应与复制构造函数、析构函数行为一致

### 学习建议

- 理解编译器何时、如何隐式声明和定义复制赋值运算符
- 掌握 Copy-and-Swap 惯用法的原理和实现
- 了解平凡复制赋值运算符的条件和优化意义
- 实践资源管理类的设计

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/copy_assignment
- C++ Standard: [class.copy.assign]
- Effective C++, Scott Meyers, Item 10-12
- Effective Modern C++, Scott Meyers, Item 17