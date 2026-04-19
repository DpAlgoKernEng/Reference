# 赋值运算符 (Assignment Operators)

## 1. 概述 (Overview)

赋值运算符 (Assignment Operators) 是 C++ 中用于修改对象值的一类运算符。它们将右侧操作数的值赋给左侧操作数所引用的对象，从而改变对象的存储内容。

### 主要类型

赋值运算符分为两大类：

**简单赋值运算符 (Simple Assignment Operator)**
- 语法：`a = b`
- 用途：将 `b` 的值直接赋给 `a`，替换 `a` 原有的内容

**复合赋值运算符 (Compound Assignment Operators)**
- 包括：`+=`、`-=`、`*=`、`/=`、`%=`、`&=`、`|=`、`^=`、`<<=`、`>>=`
- 用途：将运算结果赋值给左操作数，等价于 `a = a op b`

### 技术定位

赋值运算符是 C++ 中最基本的运算符之一，是表达式语句的核心组成部分。它们支持：
- 内置类型的直接赋值
- 类类型的拷贝赋值 (Copy Assignment) 和移动赋值 (Move Assignment)
- 运算符重载 (Operator Overloading) 以支持自定义类型

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

赋值运算符起源于 C 语言，并在 C++ 中得到扩展和完善。其设计动机包括：

1. **简洁性**：`a = a + b` 可简化为 `a += b`，减少代码量
2. **效率优化**：复合赋值运算符只需计算左操作数的地址一次
3. **表达式连续性**：支持链式赋值，如 `a = b = c = 0`

### 版本演进

| 版本 | 特性变更 |
|------|---------|
| C++98 | 基础赋值运算符，支持内置类型和类类型 |
| C++11 | 引入移动赋值 (Move Assignment)；支持初始化列表 (Initializer List) 作为右操作数 |
| C++20 | 对 volatile 限定类型的赋值进行弃用标记（部分后续修正） |

### 重要缺陷报告

| 编号 | 版本 | 问题描述 | 修正方案 |
|------|------|---------|---------|
| CWG 1527 | C++11 | 类类型对象的右操作数只能在使用用户定义赋值运算符时使用初始化列表 | 移除用户定义赋值运算符的限制 |
| CWG 1538 | C++11 | `E1 = {E2}` 等价于 `E1 = T(E2)`，引入了 C 风格转换 | 修改为等价于 `E1 = T{E2}` |
| CWG 2654 | C++20 | volatile 限定类型的复合赋值运算符弃用不一致 | 均不弃用 |
| CWG 2768 | C++11 | 非表达式初始化子句到标量值的赋值执行直接列表初始化 | 改为执行拷贝列表初始化 |
| CWG 2901 | C++98 | 通过 int 左值赋值给 unsigned int 对象的值不明确 | 明确定义 |
| P2327R1 | C++20 | volatile 类型的位运算复合赋值运算符被弃用，但在某些平台上仍有用 | 不再弃用 |

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```
target-expr = new-value              // (1) 简单赋值表达式
target-expr op new-value             // (2) 复合赋值表达式
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `target-expr` | 赋值目标表达式，必须是可修改的左值 (Modifiable Lvalue)，优先级必须高于赋值表达式 |
| `op` | 复合赋值运算符，包括 `*=`、`/=`、`%=`、`+=`、`-=`、`<<=`、`>>=`、`&=`、`^=`、`|=` |
| `new-value` | 赋值源表达式或初始化子句 (Initializer Clause, C++11 起)，不能是逗号表达式 |

### 运算符列表

| 运算符名称 | 语法 | 可重载 | 类内定义原型 | 类外定义原型 |
|-----------|------|--------|--------------|--------------|
| 简单赋值 | `a = b` | 是 | `T& T::operator=(const T2& b);` | N/A |
| 加法赋值 | `a += b` | 是 | `T& T::operator+=(const T2& b);` | `T& operator+=(T& a, const T2& b);` |
| 减法赋值 | `a -= b` | 是 | `T& T::operator-=(const T2& b);` | `T& operator-=(T& a, const T2& b);` |
| 乘法赋值 | `a *= b` | 是 | `T& T::operator*=(const T2& b);` | `T& operator*=(T& a, const T2& b);` |
| 除法赋值 | `a /= b` | 是 | `T& T::operator/=(const T2& b);` | `T& operator/=(T& a, const T2& b);` |
| 取模赋值 | `a %= b` | 是 | `T& T::operator%=(const T2& b);` | `T& operator%=(T& a, const T2& b);` |
| 位与赋值 | `a &= b` | 是 | `T& T::operator&=(const T2& b);` | `T& operator&=(T& a, const T2& b);` |
| 位或赋值 | `a |= b` | 是 | `T& T::operator|=(const T2& b);` | `T& operator|=(T& a, const T2& b);` |
| 位异或赋值 | `a ^= b` | 是 | `T& T::operator^=(const T2& b);` | `T& operator^=(T& a, const T2& b);` |
| 左移赋值 | `a <<= b` | 是 | `T& T::operator<<=(const T2& b);` | `T& operator<<=(T& a, const T2& b);` |
| 右移赋值 | `a >>= b` | 是 | `T& T::operator>>=(const T2& b);` | `T& operator>>=(T& a, const T2& b);` |

> **注意**：`T2` 可以是任何类型，包括 `T` 本身。所有内置赋值运算符返回 `*this`，大多数用户定义的重载也应返回 `*this` 以保持一致性。

## 4. 底层原理 (Underlying Principles)

### 简单赋值的实现机制

对于内置简单赋值运算符：

1. **类型要求**：`target-expr` 必须是可修改的左值
2. **值替换**：将 `target-expr` 引用的对象的值替换为 `new-value` 的结果
3. **类型转换**：
   - 如果 `new-value` 是表达式，隐式转换为 `target-expr` 的 cv 无限定类型
   - 对于整数类型，如果结果类型与目标类型的符号性不同，使用相同的值表示进行转换
4. **返回值**：返回指向 `target-expr` 的左值，类型与 `target-expr` 相同；如果是位字段 (Bit-field)，结果也是位字段

### 复合赋值的实现机制

每个内置复合赋值表达式 `E1 op= E2` 的行为完全等同于 `E1 = E1 op E2`，但有以下区别：
- `E1` 只被求值一次
- 对于 `+=` 和 `-=`，`E1` 的类型可以是算术类型或指向完全定义对象类型的指针
- 对于其他复合赋值运算符，`E1` 的类型必须是算术类型

### 重载决议 (Overload Resolution)

#### 简单赋值运算符的内置候选

对于每个类型 `T`：
- `T*& operator=(T*&, T*);`
- `T*volatile& operator=(T*volatile&, T*);`

对于每个枚举或成员指针类型 `T`（可选 volatile 限定）：
- `T& operator=(T&, T);`

对于每对 `A1`（算术类型，可选 volatile 限定）和 `A2`（提升后的算术类型）：
- `A1& operator=(A1&, A2);`

#### 复合赋值运算符的内置候选

对于每对 `A1`（算术类型，可选 volatile 限定）和 `A2`（提升后的算术类型）：
- `A1& operator*=(A1&, A2);`
- `A1& operator/=(A1&, A2);`
- `A1& operator+=(A1&, A2);`
- `A1& operator-=(A1&, A2);`

对于每对 `I1`（整数类型，可选 volatile 限定）和 `I2`（提升后的整数类型）：
- `I1& operator%=(I1&, I2);`
- `I1& operator<<=(I1&, I2);`
- `I1& operator>>=(I1&, I2);`
- `I1& operator&=(I1&, I2);`
- `I1& operator^=(I1&, I2);`
- `I1& operator|=(I1&, I2);`

对于每个可选 cv 限定的对象类型 `T`：
- `T*& operator+=(T*&, std::ptrdiff_t);`
- `T*& operator-=(T*&, std::ptrdiff_t);`
- `T*volatile& operator+=(T*volatile&, std::ptrdiff_t);`
- `T*volatile& operator-=(T*volatile&, std::ptrdiff_t);`

### 拷贝赋值与移动赋值

**拷贝赋值 (Copy Assignment)**：用 `b` 的副本替换 `a` 的内容，`b` 不被修改。对于类类型，通过特殊的成员函数实现。

**移动赋值 (Move Assignment)**（C++11 起）：用 `b` 的内容替换 `a` 的内容，尽可能避免复制，`b` 可能被修改。对于类类型，通过特殊的成员函数实现。

对于非类类型，拷贝赋值和移动赋值无法区分，统称为**直接赋值 (Direct Assignment)**。

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 变量初始化后的值修改
```cpp
int value;
value = 42;  // 直接赋值
```

#### 2. 链式赋值
```cpp
int a, b, c;
a = b = c = 0;  // 所有变量初始化为 0
```

#### 3. 复合运算赋值
```cpp
int sum = 0;
sum += 10;      // 累加
sum *= 2;       // 翻倍
```

#### 4. 指针操作
```cpp
int* ptr;
ptr = nullptr;      // 空指针赋值
ptr = &variable;     // 地址赋值
ptr += 5;            // 指针算术（偏移 5 个元素）
```

#### 5. 类类型赋值（C++11 起）
```cpp
std::complex<double> z;
z = {1.0, 2.0};      // 使用初始化列表赋值
```

### 最佳实践

1. **优先使用复合赋值**：`a += b` 比 `a = a + b` 更简洁且更安全（避免求值两次）
2. **注意返回值**：赋值表达式返回左值引用，可用于链式操作
3. **类类型设计**：重载赋值运算符时应返回 `*this` 的引用

### 常见陷阱

#### 1. 位字段赋值溢出
当目标位字段无法表示赋值表达式的值时，结果为实现定义的值。

#### 2. 重叠对象赋值
如果 `target-expr` 和 `new-value` 标识重叠对象（除非重叠完全且类型相同），行为未定义。

#### 3. volatile 限定类型（C++20）
对 volatile 限定类型的简单赋值在 C++20 中被弃用（除非是弃值表达式或未求值操作数）。

#### 4. 缩窄转换（C++11）
```cpp
int n;
n = {1.0};  // 编译错误：缩窄转换
```

## 6. 代码示例 (Examples)

### 示例 1：基本赋值操作

```cpp
#include <iostream>

int main() {
    int n = 0;        // 这是初始化，不是赋值

    n = 1;            // 直接赋值
    std::cout << n << ' ';

    n = {};           // 零初始化后赋值（C++11）
    std::cout << n << ' ';

    n = 'a';          // 整数提升后赋值
    std::cout << n << ' ';

    n = {'b'};        // 显式转换后赋值（C++11）
    std::cout << n << ' ';

    n = 1.0;          // 浮点转换后赋值
    std::cout << n << ' ';

    // n = {1.0};     // 编译错误：缩窄转换

    int& r = n;       // 引用初始化，不是赋值
    r = 2;            // 通过引用赋值
    std::cout << n << ' ';

    int* p;
    p = &n;           // 直接赋值
    p = nullptr;      // 空指针转换后赋值
    std::cout << p << ' ';

    struct { int a; std::string s; } obj;
    obj = {1, "abc"}; // 使用花括号初始化列表赋值（C++11）
    std::cout << obj.a << ':' << obj.s << '\n';

    return 0;
}
```

**输出**：
```
1 0 97 98 1 2 (nil) 1:abc
```

### 示例 2：复合赋值运算符

```cpp
#include <iostream>

int main() {
    int a = 10;

    // 算术复合赋值
    a += 5;       // a = 15
    std::cout << "a += 5: " << a << '\n';

    a -= 3;       // a = 12
    std::cout << "a -= 3: " << a << '\n';

    a *= 2;       // a = 24
    std::cout << "a *= 2: " << a << '\n';

    a /= 4;       // a = 6
    std::cout << "a /= 4: " << a << '\n';

    a %= 5;       // a = 1
    std::cout << "a %= 5: " << a << '\n';

    // 位运算复合赋值
    int b = 0b1010;
    b &= 0b1100;  // b = 0b1000 = 8
    std::cout << "b &= 0b1100: " << b << '\n';

    b |= 0b0011;  // b = 0b1011 = 11
    std::cout << "b |= 0b0011: " << b << '\n';

    b ^= 0b1111;  // b = 0b0100 = 4
    std::cout << "b ^= 0b1111: " << b << '\n';

    b <<= 2;      // b = 0b10000 = 16
    std::cout << "b <<= 2: " << b << '\n';

    b >>= 1;      // b = 0b1000 = 8
    std::cout << "b >>= 1: " << b << '\n';

    return 0;
}
```

**输出**：
```
a += 5: 15
a -= 3: 12
a *= 2: 24
a /= 4: 6
a %= 5: 1
b &= 0b1100: 8
b |= 0b0011: 11
b ^= 0b1111: 4
b <<= 2: 16
b >>= 1: 8
```

### 示例 3：指针算术复合赋值

```cpp
#include <iostream>

int main() {
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* p = arr;

    std::cout << "Initial: *p = " << *p << '\n';

    p += 3;       // 指针前进 3 个元素
    std::cout << "After p += 3: *p = " << *p << '\n';

    p -= 1;       // 指针后退 1 个元素
    std::cout << "After p -= 1: *p = " << *p << '\n';

    return 0;
}
```

**输出**：
```
Initial: *p = 0
After p += 3: *p = 3
After p -= 1: *p = 2
```

### 示例 4：类类型运算符重载

```cpp
#include <iostream>

class Counter {
private:
    int value_;

public:
    Counter(int v = 0) : value_(v) {}

    // 简单赋值运算符
    Counter& operator=(const Counter& other) {
        value_ = other.value_;
        std::cout << "Copy assignment called\n";
        return *this;
    }

    // 移动赋值运算符（C++11）
    Counter& operator=(Counter&& other) noexcept {
        value_ = other.value_;
        other.value_ = 0;
        std::cout << "Move assignment called\n";
        return *this;
    }

    // 复合赋值运算符
    Counter& operator+=(int delta) {
        value_ += delta;
        return *this;
    }

    Counter& operator-=(int delta) {
        value_ -= delta;
        return *this;
    }

    int get() const { return value_; }
};

int main() {
    Counter c1(10);
    Counter c2;

    c2 = c1;              // 拷贝赋值
    std::cout << "c2 = " << c2.get() << '\n';

    c2 = Counter(20);    // 移动赋值
    std::cout << "c2 = " << c2.get() << '\n';

    c2 += 5;             // 复合赋值
    std::cout << "After c2 += 5: " << c2.get() << '\n';

    c2 -= 3;
    std::cout << "After c2 -= 3: " << c2.get() << '\n';

    return 0;
}
```

**输出**：
```
Copy assignment called
c2 = 10
Move assignment called
c2 = 20
After c2 += 5: 25
After c2 -= 3: 22
```

### 示例 5：链式赋值

```cpp
#include <iostream>

int main() {
    int a, b, c, d;

    // 链式赋值：从右向左求值
    a = b = c = d = 100;

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
    std::cout << "c = " << c << '\n';
    std::cout << "d = " << d << '\n';

    // 链式复合赋值
    int x = 10;
    x += x *= 2;  // 先计算 x *= 2 (x = 20)，然后 x += 20 (x = 40)
    std::cout << "After x += x *= 2: x = " << x << '\n';

    return 0;
}
```

**输出**：
```
a = 100
b = 100
c = 100
d = 100
After x += x *= 2: x = 40
```

### 常见错误示例

#### 错误 1：混淆初始化与赋值

```cpp
#include <iostream>

int main() {
    // 错误理解：以为这是赋值
    int n = 0;  // 实际上是初始化，不是赋值

    // 正确的赋值
    n = 5;      // 这才是赋值

    std::cout << n << '\n';
    return 0;
}
```

#### 错误 2：对常量赋值

```cpp
#include <iostream>

int main() {
    const int ci = 10;
    // ci = 20;  // 编译错误：不能对 const 对象赋值

    std::cout << ci << '\n';
    return 0;
}
```

#### 错误 3：缩窄转换（C++11）

```cpp
#include <iostream>

int main() {
    int n;
    // n = {1.5};  // 编译错误：列表初始化不允许缩窄转换
    n = 1.5;       // 正确：允许隐式转换（但会丢失精度）

    std::cout << n << '\n';  // 输出 1
    return 0;
}
```

#### 错误 4：自赋值未处理

```cpp
#include <iostream>
#include <cstring>

class String {
private:
    char* data_;
    size_t size_;

public:
    String(const char* s = "") {
        size_ = std::strlen(s);
        data_ = new char[size_ + 1];
        std::strcpy(data_, s);
    }

    ~String() { delete[] data_; }

    // 错误的赋值运算符实现（未处理自赋值）
    String& operator=(const String& other) {
        // 问题：如果是自赋值，delete[] data_ 后 other.data_ 已被释放
        // delete[] data_;
        // size_ = other.size_;
        // data_ = new char[size_ + 1];
        // std::strcpy(data_, other.data_);

        // 正确的实现
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        }
        return *this;
    }

    const char* c_str() const { return data_; }
};

int main() {
    String s1("hello");
    String s2;

    s2 = s1;           // 正常赋值
    std::cout << s2.c_str() << '\n';

    s2 = s2;           // 自赋值，需要正确处理
    std::cout << s2.c_str() << '\n';

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **分类清晰**：赋值运算符分为简单赋值 (`=`) 和复合赋值 (`op=`) 两大类
2. **返回值特性**：所有内置赋值运算符返回左值引用，指向左操作数
3. **类型安全**：支持类型转换，但需注意缩窄转换和精度损失
4. **可重载性**：所有赋值运算符均可重载，建议返回 `*this` 的引用
5. **求值顺序**：复合赋值运算符中左操作数只求值一次

### 技术对比

| 特性 | 简单赋值 | 复合赋值 |
|------|---------|---------|
| 语法 | `a = b` | `a op= b` |
| 等价表达式 | - | `a = a op b` |
| 左操作数求值次数 | 1 | 1（`a = a op b` 为 2） |
| 类型要求 | 可修改左值 | 可修改左值 + 特定类型要求 |
| 指针支持 | 所有指针类型 | 仅 `+=` 和 `-=` 支持指针 |

### 学习建议

1. **掌握基础**：理解简单赋值与初始化的区别
2. **善用复合赋值**：使用 `+=`、`-=` 等替代 `a = a + b`，提高代码简洁性和安全性
3. **注意类型安全**：避免隐式转换导致的精度损失，使用列表初始化防止缩窄转换
4. **重载规范**：重载赋值运算符时遵循"三/五法则"，正确处理自赋值，返回 `*this` 引用
5. **理解移动语义**（C++11）：区分拷贝赋值和移动赋值的语义差异
6. **关注新特性**（C++11 起）：了解初始化列表作为赋值右操作数的用法

### 相关主题

- 运算符优先级 (Operator Precedence)
- 运算符重载 (Operator Overloading)
- 拷贝构造函数 (Copy Constructor)
- 移动语义 (Move Semantics)
- 引用 (References)
- 左值与右值 (Lvalue and Rvalue)