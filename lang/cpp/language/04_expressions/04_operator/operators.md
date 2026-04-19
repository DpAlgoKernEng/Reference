# 运算符重载 (Operator Overloading)

## 1. 概述 (Overview)

运算符重载是 C++ 提供的一种机制，允许为用户自定义类型（类类型和枚举类型）自定义运算符的行为。通过运算符重载，用户定义的类型可以像内置类型一样使用运算符进行操作，从而提供更直观、更自然的语法接口。

### 核心概念

**运算符函数 (Operator Function)** 是具有特殊函数名的函数，其名称格式为 `operator` 后跟运算符符号。当表达式中出现运算符且至少有一个操作数是类类型或枚举类型时，编译器会通过重载决议 (Overload Resolution) 确定调用的用户定义函数。

### 主要用途

1. **提供直观的语法接口**：使自定义类型支持算术、比较、流操作等运算符
2. **实现语义一致性**：让自定义类型的行为与内置类型保持一致
3. **增强代码可读性**：使用 `a + b` 比 `a.add(b)` 更直观
4. **支持标准库算法**：如容器和算法需要比较、赋值等运算符

### 技术定位

运算符重载是 C++ 实现抽象数据类型的核心机制之一，与函数重载、模板等特性共同构成了 C++ 的多态性体系。它是实现自定义数值类型、智能指针、迭代器、函数对象等高级抽象的基础。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

运算符重载是 C++ 从 C 语言演化过程中引入的重要特性。设计动机源于以下需求：

1. **数值计算领域**：需要自定义复数、矩阵、分数等数学类型，使其能像内置数值类型一样进行运算
2. **抽象数据类型**：需要为容器、字符串等类型提供自然的访问语法
3. **代码可读性**：避免使用冗长的成员函数调用语法

### 版本变更

| C++ 版本 | 主要变更 |
|---------|---------|
| C++98 | 引入运算符重载基本机制，定义了可重载运算符集合 |
| C++11 | 引入移动语义，支持移动赋值运算符；引入 `operator""` 字面量运算符 |
| C++17 | 修复重载的 `&&`、`||`、`,` 失去特殊序列属性的问题 |
| C++20 | 引入三向比较运算符 `<=>`；支持重写候选（Rewritten Candidates）用于比较运算符；引入 `operator co_await` |
| C++23 | 允许 `operator()` 和 `operator[]` 声明为静态成员函数；支持多参数下标运算符 |

### 缺陷报告

| 缺陷报告 | 应用版本 | 原行为 | 修正行为 |
|---------|---------|--------|---------|
| CWG 1481 | C++98 | 非成员前缀递增运算符参数只能是类类型、枚举类型或其引用类型 | 移除类型限制 |
| CWG 2931 | C++23 | 显式对象成员运算符函数不能有类类型、枚举类型或其引用类型的参数 | 禁止此情况 |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

运算符函数具有特殊的函数命名形式：

| 语法形式 | 说明 | 示例 |
|---------|------|-----|
| `operator` op | 重载标点运算符 | `operator+`, `operator==` |
| `operator new` / `operator new[]` | 分配函数 | 自定义内存分配 |
| `operator delete` / `operator delete[]` | 释放函数 | 自定义内存释放 |
| `operator co_await` | 协程等待运算符 (C++20) | 用于协程表达式 |

其中 `op` 可以是以下运算符之一：
```
+ - * / % ^ & | ~ ! = < > += -= *= /= %= ^= &= |= << >> >>= <<= == != <= >= <=>(C++20) && || ++ -- , ->* -> () []
```

### 可重载与不可重载运算符

**可重载运算符**：
- 算术运算符：`+`, `-`, `*`, `/`, `%`
- 关系运算符：`==`, `!=`, `<`, `>`, `<=`, `>=`, `<=>`
- 逻辑运算符：`!`, `&&`, `||`
- 位运算符：`~`, `&`, `|`, `^`, `<<`, `>>`
- 赋值运算符：`=`, `+=`, `-=`, `*=`, `/=`, `%=`, `^=`, `&=`, `|=`, `<<=`, `>>=`
- 递增递减：`++`, `--`
- 成员访问：`->`, `->*`
- 函数调用：`()`
- 下标访问：`[]`
- 逗号运算符：`,`
- 内存管理：`new`, `delete`, `new[]`, `delete[]`
- 协程运算符：`co_await` (C++20)

**不可重载运算符**：
- `::` - 作用域解析运算符
- `.` - 成员访问运算符
- `.*` - 成员指针访问运算符
- `?:` - 三元条件运算符

### 参数与调用形式

| 表达式 | 作为成员函数 | 作为非成员函数 |
|-------|-------------|---------------|
| `@a` | `(a).operator@()` | `operator@(a)` |
| `a@b` | `(a).operator@(b)` | `operator@(a, b)` |
| `a=b` | `(a).operator=(b)` | 不能作为非成员 |
| `a(b...)` | `(a).operator()(b...)` | 不能作为非成员 |
| `a[b...]` | `(a).operator[](b...)` | 不能作为非成员 |
| `a->` | `(a).operator->()` | 不能作为非成员 |
| `a@` (后缀) | `(a).operator@(0)` | `operator@(a, 0)` |

### 限制条件

1. **参数类型要求**：运算符函数必须至少有一个函数参数或隐式对象参数的类型是类、类引用、枚举或枚举引用

2. **不可创建新运算符**：不能创建 `**`、`<>`、`&|` 等新运算符

3. **不可改变运算符特性**：不能改变运算符的优先级、结合性或操作数数量

4. **特殊返回值要求**：
   - `operator->` 必须返回原始指针或重载了 `->` 的对象
   - 赋值运算符通常返回左操作数的引用

5. **短路求值失效**：重载的 `&&` 和 `||` 不再具有短路求值特性

## 4. 底层原理 (Underlying Principles)

### 重载决议机制

当表达式中出现运算符且至少有一个操作数是类类型或枚举类型时，编译器执行以下步骤：

1. **候选函数收集**：查找所有匹配签名的运算符函数
   - 成员函数：通过对象类型查找
   - 非成员函数：通过参数依赖查找 (ADL, Argument-Dependent Lookup)

2. **重写候选 (C++20)**：对于比较运算符 `==`, `!=`, `<`, `>`, `<=`, `>=`, `<=>`，编译器还会考虑重写形式：
   - `a == b` 可以匹配 `b == a`（反向调用）
   - `a < b` 可以匹配 `b > a`（反向调用）
   - `a < b` 可以匹配 `(a <=> b) < 0`

3. **最佳匹配选择**：通过重载决议选择最佳匹配函数

4. **函数调用**：生成调用选定运算符函数的代码

### 实现机制

**成员运算符函数**：
```cpp
class X {
public:
    X operator+(const X& rhs) const;
    // 编译器转换为：X operator+(this, const X& rhs)
};

X a, b;
X c = a + b;  // 编译器转换为：a.operator+(b)
```

**非成员运算符函数**：
```cpp
X operator+(const X& lhs, const X& rhs);

X a, b;
X c = a + b;  // 编译器转换为：operator+(a, b)
```

### 后缀递增递减的区分

后缀递增/递减运算符通过一个 `int` 类型的伪参数（通常为 0）与前缀版本区分：

```cpp
struct X {
    X& operator++();    // 前缀递增：++x
    X operator++(int); // 后缀递增：x++
};
```

### 性能特征

| 运算符类型 | 典型返回类型 | 性能考虑 |
|-----------|-------------|---------|
| 算术运算符 (`+`, `-` 等) | 值类型 | 可能涉及临时对象构造 |
| 复合赋值 (`+=`, `-=` 等) | 引用类型 | 避免额外拷贝 |
| 比较运算符 | `bool` | 通常内联，性能好 |
| 前缀递增/递减 | 引用类型 | 无额外拷贝 |
| 后缀递增/递减 | 值类型 | 需要保存旧值，有拷贝开销 |

## 5. 使用场景 (Use Cases)

### 赋值运算符

**适用场景**：管理资源的类（如动态内存、文件句柄）

**最佳实践**：
- 拷贝赋值运算符应正确处理自赋值
- 移动赋值运算符应确保 `noexcept`（利于容器优化）
- 使用 copy-and-swap 惯用语简化实现

**经典实现**：
```cpp
class T {
    int* mArray;
    size_t size;
public:
    // 拷贝赋值 - 传统方式
    T& operator=(const T& other) {
        if (this == &other)  // 防止自赋值
            return *this;

        if (size != other.size) {
            int* temp = new int[other.size];  // 先分配
            delete[] mArray;                   // 再释放
            mArray = temp;
            size = other.size;
        }
        std::copy(other.mArray, other.mArray + other.size, mArray);
        return *this;
    }

    // 移动赋值 (C++11)
    T& operator=(T&& other) noexcept {
        if (this == &other)
            return *this;

        delete[] mArray;
        mArray = std::exchange(other.mArray, nullptr);
        size = std::exchange(other.size, 0);
        return *this;
    }
};
```

**copy-and-swap 惯用语**：
```cpp
T& operator=(T other) noexcept {  // 值传递，自动处理拷贝和移动
    std::swap(size, other.size);
    std::swap(mArray, other.mArray);
    return *this;
}  // other 的析构函数自动释放旧资源
```

### 流插入/提取运算符

**适用场景**：需要与 I/O 流交互的自定义类型

**最佳实践**：
- 必须实现为非成员函数（左操作数是 `std::ostream` 或 `std::istream`）
- 通常声明为友元以访问私有成员
- 返回流的引用以支持链式调用

```cpp
class Fraction {
    int n, d;
public:
    int num() const { return n; }
    int den() const { return d; }

    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        return os << f.n << '/' << f.d;
    }

    friend std::istream& operator>>(std::istream& is, Fraction& f) {
        char slash;
        return is >> f.n >> slash >> f.d;
    }
};
```

### 函数调用运算符

**适用场景**：创建函数对象 (Functor)、回调、策略模式

**特点**：可以保存状态，比函数指针更灵活

```cpp
struct Linear {
    double a, b;

    double operator()(double x) const {
        return a * x + b;
    }
};

Linear f{2, 1};  // 表示函数 2x + 1
double result = f(5);  // 调用 operator()，结果为 11
```

**C++23 静态调用运算符**：
```cpp
struct SwapThem {
    template<typename T>
    static void operator()(T& lhs, T& rhs) {
        std::ranges::swap(lhs, rhs);
    }
};

SwapThem swap_them;
int a = 1, b = 2;
swap_them(a, b);  // OK
SwapThem::operator()(a, b);  // OK
```

### 比较运算符

**适用场景**：需要排序、查找、容器存储的类型

**C++20 前的典型实现**：
```cpp
struct Record {
    std::string name;
    unsigned int floor;
    double weight;

    friend bool operator<(const Record& l, const Record& r) {
        return std::tie(l.name, l.floor, l.weight)
             < std::tie(r.name, r.floor, l.weight);
    }
};

// 基于一个运算符实现其他
inline bool operator> (const X& lhs, const X& rhs) { return rhs < lhs; }
inline bool operator<=(const X& lhs, const X& rhs) { return !(lhs > rhs); }
inline bool operator>=(const X& lhs, const X& rhs) { return !(lhs < rhs); }
inline bool operator==(const X& lhs, const X& rhs) { /* 实际比较 */ }
inline bool operator!=(const X& lhs, const X& rhs) { return !(lhs == rhs); }
```

**C++20 三向比较运算符**：
```cpp
struct Record {
    std::string name;
    unsigned int floor;
    double weight;

    auto operator<=>(const Record&) const = default;  // 自动生成所有比较
};
```

### 下标运算符

**适用场景**：容器类、数组包装类

**多版本实现**：
```cpp
struct Container {
    std::vector<int> data;

    int& operator[](size_t idx) { return data[idx]; }
    const int& operator[](size_t idx) const { return data[idx]; }
};
```

**C++23 多参数下标**：
```cpp
template<typename T, size_t Z, size_t Y, size_t X>
struct Array3d {
    std::array<T, X * Y * Z> m{};

    T& operator[](size_t z, size_t y, size_t x) {
        return m[z * Y * X + y * X + x];
    }
};

Array3d<int, 4, 3, 2> v;
v[3, 2, 1] = 42;  // 直接三维访问
```

### 算术运算符

**最佳实践**：使用复合赋值运算符实现二元运算符

```cpp
class X {
public:
    X& operator+=(const X& rhs) {
        // 实际加法逻辑
        return *this;
    }

    friend X operator+(X lhs, const X& rhs) {  // 值传递 lhs
        lhs += rhs;  // 复用复合赋值
        return lhs;
    }
};
```

### 常见陷阱

1. **自赋值未处理**：赋值运算符必须检查自赋值
2. **短路求值失效**：重载 `&&` 和 `||` 会失去短路特性
3. **破坏语义一致性**：`operator+` 不应实现减法
4. **返回类型错误**：赋值运算符应返回引用，后缀递增应返回值
5. **成员与非成员选择不当**：二元运算符通常应为非成员以支持对称性

## 6. 代码示例 (Examples)

### 示例 1：分数类完整实现

```cpp
#include <iostream>
#include <numeric>  // for std::gcd (C++17)

class Fraction {
    constexpr int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    int n, d;  // 分子、分母

public:
    constexpr Fraction(int n, int d = 1)
        : n(n / gcd(n, d)), d(d / gcd(n, d)) {}

    constexpr int num() const { return n; }
    constexpr int den() const { return d; }

    // 复合赋值
    constexpr Fraction& operator*=(const Fraction& rhs) {
        int new_n = n * rhs.n / gcd(n * rhs.n, d * rhs.d);
        d = d * rhs.d / gcd(n * rhs.n, d * rhs.d);
        n = new_n;
        return *this;
    }

    // 流输出
    friend std::ostream& operator<<(std::ostream& out, const Fraction& f) {
        return out << f.num() << '/' << f.den();
    }

    // 比较
    friend constexpr bool operator==(const Fraction& lhs, const Fraction& rhs) {
        return lhs.num() == rhs.num() && lhs.den() == rhs.den();
    }

    friend constexpr bool operator!=(const Fraction& lhs, const Fraction& rhs) {
        return !(lhs == rhs);
    }

    // 二元运算符
    friend constexpr Fraction operator*(Fraction lhs, const Fraction& rhs) {
        return lhs *= rhs;
    }
};

int main() {
    constexpr Fraction f1{3, 8}, f2{1, 2}, f3{10, 2};

    std::cout << f1 << " * " << f2 << " = " << f1 * f2 << '\n'
              << f2 << " * " << f3 << " = " << f2 * f3 << '\n'
              << 2 << " * " << f1 << " = " << 2 * f1 << '\n';

    static_assert(f3 == Fraction{5, 1});  // 编译期验证
}
```

**输出**：
```
3/8 * 1/2 = 3/16
1/2 * 5/1 = 5/2
2 * 3/8 = 3/4
```

### 示例 2：迭代器风格的递增递减

```cpp
struct Iterator {
    int* ptr;

    // 前缀递增：返回新值的引用
    Iterator& operator++() {
        ++ptr;
        return *this;
    }

    // 后缀递增：返回旧值
    Iterator operator++(int) {
        Iterator old = *this;
        ++(*this);  // 复用前缀版本
        return old;
    }

    // 前缀递减
    Iterator& operator--() {
        --ptr;
        return *this;
    }

    // 后缀递减
    Iterator operator--(int) {
        Iterator old = *this;
        --(*this);
        return old;
    }

    int& operator*() { return *ptr; }
};
```

### 示例 3：智能指针风格的成员访问

```cpp
template<typename T>
class SmartPtr {
    T* ptr;
public:
    explicit SmartPtr(T* p = nullptr) : ptr(p) {}
    ~SmartPtr() { delete ptr; }

    // 解引用
    T& operator*() const { return *ptr; }

    // 成员访问：必须返回原始指针或重载了 -> 的对象
    T* operator->() const { return ptr; }
};

struct Point { int x, y; };

int main() {
    SmartPtr<Point> p(new Point{1, 2});
    p->x = 10;      // 调用 p.operator->()->x
    int y = (*p).y; // 调用 p.operator*().y
}
```

### 示例 4：函数对象求和

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

struct Sum {
    int sum = 0;
    void operator()(int n) { sum += n; }
};

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    Sum s = std::for_each(v.begin(), v.end(), Sum());
    std::cout << "The sum is " << s.sum << '\n';
}
```

**输出**：
```
The sum is 15
```

### 示例 5：常见错误

```cpp
// 错误 1：赋值运算符未处理自赋值
class Bad {
    char* data;
public:
    Bad& operator=(const Bad& other) {
        delete[] data;  // 危险！可能删除自己的数据
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);  // 如果 &other == this，崩溃
        return *this;
    }
};

// 错误 2：二元运算符不对称
class Complex {
    double re, im;
public:
    Complex operator+(const Complex& rhs) const;  // 只支持 Complex + Complex
    // 问题：double + Complex 无法工作
};

// 错误 3：返回类型错误
class Counter {
    int count;
public:
    Counter operator++() {  // 应返回引用
        ++count;
        return *this;  // 返回临时对象
    }
};
```

## 7. 总结 (Summary)

### 核心要点

1. **基本机制**：运算符重载通过特殊命名的函数实现，编译器自动将运算表达式转换为函数调用

2. **限制条件**：
   - 不可重载 `::`, `.`, `.*`, `?:`
   - 不可创建新运算符
   - 不可改变优先级、结合性和操作数数量

3. **选择原则**：
   - 一元运算符通常为成员
   - 二元运算符通常为非成员（对称性）
   - 必须为成员的运算符：`=`, `()`, `[]`, `->`

4. **典型实现模式**：
   - 用复合赋值实现二元运算
   - 用前缀实现后缀
   - 用 `std::tie` 实现比较运算

### 技术对比

| 特性 | 成员函数 | 非成员函数 |
|-----|---------|-----------|
| 访问私有成员 | 直接访问 | 需声明友元或使用公有接口 |
| 左操作数类型 | 必须是类类型 | 可以是任意类型 |
| 对称性 | 差（左操作数必须匹配） | 好（支持隐式转换） |
| 适用运算符 | `=`, `()`, `[]`, `->`, 一元运算符 | 算术、比较等二元运算符 |

### C++ 版本演进

| 版本 | 关键特性 |
|-----|---------|
| C++98 | 基本运算符重载机制 |
| C++11 | 移动语义、`operator""` 字面量运算符 |
| C++17 | 修复 `&&`, `||`, `,` 的序列属性 |
| C++20 | 三向比较 `<=>`、重写候选 |
| C++23 | 静态 `operator()`, 多参数 `operator[]` |

### 学习建议

1. **优先考虑语义一致性**：重载的运算符行为应与内置运算符语义一致
2. **遵循最小惊讶原则**：`operator+` 应实现加法而非减法
3. **善用工具函数**：利用 `std::tie`, `std::swap` 简化实现
4. **注意异常安全**：赋值运算符应保证异常安全（copy-and-swap）
5. **现代 C++ 优先**：C++20 后优先使用 `operator<=>` 简化比较运算符

### 扩展阅读

- [运算符优先级](./operator_precedence.md)
- [替代运算符语法](./alternative_operators.md)
- [参数依赖查找](./adl.md)
- [比较运算符](./comparison_operators.md)

### 参考资料

1. [Operator Overloading on StackOverflow C++ FAQ](https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading)
2. CppReference - [Operator overloading](https://en.cppreference.com/w/cpp/language/operators)