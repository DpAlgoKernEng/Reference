# 比较运算符 (Comparison Operators)

## 1. 概述

比较运算符（comparison operators）是 C++ 中用于比较两个操作数的运算符，返回布尔类型（bool）的值。C++ 提供了丰富的比较运算符，包括传统的双向比较运算符和 C++20 引入的三向比较运算符（three-way comparison operator，又称太空船运算符 spaceship operator）。

比较运算符分为两大类：

| 类别 | 运算符 | 说明 |
|------|--------|------|
| **相等运算符** (equality operators) | `==`, `!=` | 判断两个值是否相等 |
| **关系运算符** (relational operators) | `<`, `>`, `<=`, `>=` | 判断两个值的大小关系 |
| **三向比较运算符** (three-way comparison operator) | `<=>` | C++20 引入，返回完整的比较结果 |

比较运算符是 C++ 表达式系统的基础组件，广泛用于条件判断、排序算法、容器实现等场景。

## 2. 来源与演变

### 历史背景

比较运算符从 C 语言继承而来，是 C/C++ 语言中最基础的运算符之一。早期的比较运算符设计用于：

- 条件分支判断（if、while、for）
- 排序和查找算法
- 指针比较

### C++11 变化

- 引入 `std::nullptr_t` 类型，支持与空指针比较
- 新增 `constexpr` 支持，部分比较操作可在编译期执行

### C++17 变化

- 函数指针转换（function pointer conversions）在比较操作前执行

### C++20 重大变化

**引入三向比较运算符 `<=>`**

三向比较运算符是 C++20 最重要的新特性之一，它：

- 返回一个比较类别类型（comparison category type）
- 支持自动生成其他比较运算符
- 简化了用户定义类型的比较实现

**特性测试宏**

| 特性测试宏 | 值 | 说明 |
|-----------|-----|------|
| `__cpp_impl_three_way_comparison` | 201907L | 编译器支持三向比较 |
| `__cpp_lib_three_way_comparison` | 201907L | 标准库支持三向比较 |

### 废弃与移除

- **C++20**: 移除指针与空指针常量的关系比较（CWG 583）
- **C++20**: 标准库中大量 `operator!=` 重载被移除（可通过 `operator==` 自动生成）
- **C++20**: `std::rel_ops` 命名空间被废弃

## 3. 语法与参数

### 运算符列表

| 运算符名称 | 语法 | 可重载 | 类内声明示例 | 类外声明示例 |
|-----------|------|--------|--------------|--------------|
| 等于 | `a == b` | 是 | `bool T::operator==(const U& b) const;` | `bool operator==(const T& a, const U& b);` |
| 不等于 | `a != b` | 是 | `bool T::operator!=(const U& b) const;` | `bool operator!=(const T& a, const U& b);` |
| 小于 | `a < b` | 是 | `bool T::operator<(const U& b) const;` | `bool operator<(const T& a, const U& b);` |
| 大于 | `a > b` | 是 | `bool T::operator>(const U& b) const;` | `bool operator>(const T& a, const U& b);` |
| 小于等于 | `a <= b` | 是 | `bool T::operator<=(const U& b) const;` | `bool operator<=(const T& a, const U& b);` |
| 大于等于 | `a >= b` | 是 | `bool T::operator>=(const U& b) const;` | `bool operator>=(const T& a, const U& b);` |
| 三向比较 (C++20) | `a <=> b` | 是 | `R T::operator<=>(const U& b) const;` | `R operator<=>(const T& a, const U& b);` |

**说明**：
- `R` 是三向比较的返回类型（比较类别类型）
- `U` 可以是任意类型，包括 `T` 本身
- 内置运算符返回 `bool`，用户定义运算符可返回任意类型

### 双向比较运算符语法

#### 关系运算符

```cpp
lhs < rhs    // (1) 小于
lhs > rhs    // (2) 大于
lhs <= rhs   // (3) 小于等于
lhs >= rhs   // (4) 大于等于
```

#### 相等运算符

```cpp
lhs == rhs   // (5) 等于
lhs != rhs   // (6) 不等于
```

**返回值说明**：

| 运算符 | 返回 true 条件 | 返回 false 条件 |
|--------|---------------|-----------------|
| `<` | lhs 小于 rhs | 其他情况 |
| `>` | lhs 大于 rhs | 其他情况 |
| `<=` | lhs 小于或等于 rhs | 其他情况 |
| `>=` | lhs 大于或等于 rhs | 其他情况 |
| `==` | lhs 等于 rhs | 其他情况 |
| `!=` | lhs 不等于 rhs | 其他情况 |

### 三向比较运算符语法 (C++20)

```cpp
lhs <=> rhs
```

**返回值**：

三向比较运算符返回一个对象，使得：

- `(a <=> b) < 0` 如果 `a < b`
- `(a <=> b) > 0` 如果 `a > b`
- `(a <=> b) == 0` 如果 `a` 和 `b` 相等/等价

**比较类别类型**：

| 类型 | 说明 |
|------|------|
| `std::strong_ordering` | 强排序，整数类型返回此类型 |
| `std::weak_ordering` | 弱排序 |
| `std::partial_ordering` | 部分排序，浮点类型返回此类型 |

### 参数类型要求

**算术类型比较**：

如果两个操作数都是算术类型或枚举类型：

- 执行通常的算术转换（usual arithmetic conversions）
- 转换后的值进行比较
- 返回 `bool` 类型的纯右值（prvalue）

**指针类型比较**：

如果至少一个操作数是指针：

- 执行指针转换（pointer conversions）
- 执行限定转换（qualification conversions）
- 转换为复合指针类型（composite pointer type）后比较

## 4. 底层原理

### 内置算术比较

对于内置类型的算术比较，执行以下步骤：

1. **左值到右值转换**（lvalue-to-rvalue conversion）
2. **通常的算术转换**（usual arithmetic conversions）
3. 比较转换后的值

**示例：有符号与无符号比较**

```cpp
#include <iostream>

int main() {
    int a = -1;
    unsigned int c = 1;

    // 通常的算术转换：int 转换为 unsigned int
    // -1 转换为 unsigned int 得到 UINT_MAX
    std::cout << std::boolalpha
              << " -1 < 1u ? " << (a < c) << "\n"   // false! 陷阱
              << " -1 > 1u ? " << (a > c) << "\n";  // true
    return 0;
}
```

### 内置指针相等比较

指针相等比较有三种可能结果：

| 比较结果 | `p == q` 返回值 | `p != q` 返回值 |
|---------|----------------|-----------------|
| 相等 | `true` | `false` |
| 不相等 | `false` | `true` |
| 未指定 | 未指定的 bool 值 | 未指定的 bool 值 |

**相等条件**：

- 两个空指针比较相等
- 两个指针指向同一函数，比较相等
- 两个指针表示相同地址（指向或超过同一对象末尾），比较相等

**未指定情况**：

- 一个指针指向完整对象，另一个指向不同对象的尾后地址
- 成员指针指向虚函数

### 内置指针关系比较

指针关系比较有四种可能结果：

| 比较结果 | `p > q` | `p < q` | `p >= q` | `p <= q` |
|---------|---------|---------|----------|----------|
| 相等 | `false` | `false` | `true` | `true` |
| p 更大 | `true` | `false` | `true` | `false` |
| q 更大 | `false` | `true` | `false` | `true` |
| 未指定 | 未指定的 bool 值 | 未指定的 bool 值 | 未指定的 bool 值 | 未指定的 bool 值 |

**定义良好的比较**：

指针关系比较在以下情况下有明确定义：

1. 指向同一数组中不同元素的指针
2. 指向同一对象成员的指针
3. 指向对象和其尾后指针的比较

**指针全序（Pointer Total Order）**

每个程序中存在一个**实现定义的严格全序**：

- 与上述偏序一致
- 未指定的结果变为实现定义的
- 通过 `std::less`, `std::greater` 等使用

### 三向比较运算符原理 (C++20)

**整数类型比较**：

返回 `std::strong_ordering` 类型：

```cpp
std::strong_ordering::equal      // 两数算术相等
std::strong_ordering::less        // 第一个数小于第二个
std::strong_ordering::greater    // 其他情况
```

**浮点类型比较**：

返回 `std::partial_ordering` 类型：

```cpp
std::partial_ordering::less       // a 小于 b
std::partial_ordering::greater    // a 大于 b
std::partial_ordering::equivalent // a 等价于 b（如 -0.0 和 +0.0）
std::partial_ordering::unordered  // 无法比较（如 NaN 和任何值）
```

**指针比较**：

返回 `std::strong_ordering` 类型：

```cpp
std::strong_ordering::equal      // 指针相等
std::strong_ordering::less       // q 大于 p
std::strong_ordering::greater    // p 大于 q
// 未指定结果：如果双向比较结果未指定
```

### 运算符结合性

比较运算符从左到右结合，这意味着：

```cpp
a < b < c   // 解析为 (a < b) < c，而不是 (a < b) && (b < c)
```

这是一个常见的陷阱：

```cpp
int a = 3, b = 2, c = 1;
bool result = (a < b < c);  // 结果为 true！
// 等价于 (a < b) < c
// = (3 < 2) < 1
// = false < 1
// = 0 < 1
// = true
```

### 重载决议

在重载决议中，以下函数签名参与竞争：

**算术类型**（每对提升后的算术类型 `L` 和 `R`）：

```cpp
bool operator<(L, R);
bool operator>(L, R);
bool operator<=(L, R);
bool operator>=(L, R);
bool operator==(L, R);
bool operator!=(L, R);
```

**指针类型**（每个指针类型 `P`）：

```cpp
bool operator<(P, P);
bool operator>(P, P);
bool operator<=(P, P);
bool operator>=(P, P);
bool operator==(P, P);
bool operator!=(P, P);
```

**成员指针类型**（每个成员指针类型 `MP` 和 `std::nullptr_t`）：

```cpp
bool operator==(MP, MP);
bool operator!=(MP, MP);
```

## 5. 使用场景

### 适合使用比较运算符的场景

| 场景 | 说明 |
|------|------|
| 条件判断 | if、while、for 语句中的条件表达式 |
| 排序算法 | `std::sort`、`std::stable_sort` 等 |
| 关联容器 | `std::map`、`std::set` 等需要比较的容器 |
| 查找算法 | `std::find`、`std::binary_search` 等 |
| 自定义类型比较 | 重载运算符实现自定义比较逻辑 |

### 严格弱序要求（Strict Weak Ordering）

用户定义的 `operator<` 必须满足严格弱序要求：

1. **反自反性**（Irreflexivity）：`!(a < a)` 为 true
2. **不对称性**（Asymmetry）：如果 `a < b`，则 `!(b < a)`
3. **传递性**（Transitivity）：如果 `a < b` 且 `b < c`，则 `a < c`
4. **不可比性的传递性**（Transitivity of incomparability）：如果 `!(a < b)` 且 `!(b < a)`，且 `!(b < c)` 且 `!(c < b)`，则 `!(a < c)` 且 `!(c < a)`

标准库容器和算法（如 `std::sort`、`std::map`）都要求严格弱序。

### 相等与等价的区别

对于同时满足 EqualityComparable 和 LessThanComparable 的类型：

- **相等**（equality）：`a == b` 的值
- **等价**（equivalence）：`!(a < b) && !(b < a)` 的值

等价并不意味着相等，只意味着在排序意义上不可区分。

### 最佳实践

#### 1. 使用三向比较运算符简化代码 (C++20)

```cpp
// ❌ 传统方式：需要定义多个运算符
struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    // ... 还需要定义 <=, >, >=
};

// ✅ C++20 方式：只定义三向比较运算符
struct Point {
    int x, y;

    auto operator<=>(const Point& other) const = default;
    // 编译器自动生成 ==, !=, <, >, <=, >=
};
```

#### 2. 有符号与无符号比较要谨慎

```cpp
// ❌ 危险：有符号与无符号比较
int i = -1;
unsigned int u = 1;
if (i < u) {  // false！因为 i 被转换为很大的无符号数
    // ...
}

// ✅ 安全：使用相同类型
if (i < static_cast<int>(u)) {  // 正确
    // ...
}
```

#### 3. 指针比较时注意未定义行为

```cpp
int a[5];
int b[5];

// ❌ 未定义行为：比较不同数组的元素指针
bool result = (a < b);  // 未定义！

// ✅ 安全：只在同一数组内比较
bool result = (&a[0] < &a[4]);  // 定义良好的比较
```

### 常见陷阱

#### 陷阱 1：链式比较错误

```cpp
// ❌ 错误：链式比较不按预期工作
int x = 5;
int y = 10;
int z = 15;
if (x < y < z) {  // 解析为 (x < y) < z = true < 15 = 1 < 15 = true
    // 这个条件总是为 true（只要 y > x）
}

// ✅ 正确：使用逻辑运算符
if (x < y && y < z) {
    // 现在才是真正的范围检查
}
```

#### 陷阱 2：浮点数相等比较

```cpp
double a = 0.1 + 0.2;
double b = 0.3;

// ❌ 危险：直接比较浮点数
if (a == b) {  // 可能为 false！
    // ...
}

// ✅ 安全：使用 epsilon 比较
constexpr double epsilon = 1e-9;
if (std::abs(a - b) < epsilon) {
    // ...
}
```

#### 陷阱 3：指针与空指针常量比较

```cpp
void f(char* p) {
    // C++20 前：可能编译通过但已废弃
    // C++20 起：编译错误
    if (p > 0) { /* ... */ }        // 错误：不能与空指针常量比较
    if (p > nullptr) { /* ... */ } // 错误：不能与 nullptr 比较

    // ✅ 正确方式
    if (p != nullptr) { /* ... */ }  // 只能用相等运算符
}
```

#### 陷阱 4：数组比较

```cpp
char a[] = "hello";
char b[] = "hello";

// ❌ 错误：比较的是指针，不是内容
if (a == b) {  // 总是 false（比较两个不同数组的地址）
    // ...
}

// ✅ 正确：使用字符串比较函数
if (std::strcmp(a, b) == 0) {
    // ...
}

// 或者使用 std::string
std::string s1 = "hello";
std::string s2 = "hello";
if (s1 == s2) {  // 正确：比较内容
    // ...
}
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <compare>  // C++20

int main() {
    // 基本类型比较
    int a = 10, b = 20;

    std::cout << std::boolalpha;
    std::cout << "a == b: " << (a == b) << "\n";  // false
    std::cout << "a != b: " << (a != b) << "\n";  // true
    std::cout << "a < b:  " << (a < b) << "\n";   // true
    std::cout << "a > b:  " << (a > b) << "\n";   // false
    std::cout << "a <= b: " << (a <= b) << "\n";  // true
    std::cout << "a >= b: " << (a >= b) << "\n";  // false

    // C++20 三向比较
    auto result = a <=> b;
    if (result < 0) {
        std::cout << "a 小于 b\n";
    } else if (result > 0) {
        std::cout << "a 大于 b\n";
    } else {
        std::cout << "a 等于 b\n";
    }

    return 0;
}
```

### 指针比较

```cpp
#include <iostream>

int main() {
    std::cout << std::boolalpha;

    // 数组元素指针比较
    char a[4] = "abc";
    char* p1 = &a[1];
    char* p2 = &a[2];
    std::cout << "数组元素指针比较:\n"
              << "p1 == p2? " << (p1 == p2) << "\n"  // false
              << "p1 <  p2? " << (p1 < p2) << "\n"; // true

    // 类成员指针比较
    struct Foo { int n1; int n2; };
    Foo f;
    int* p3 = &f.n1;
    int* p4 = &f.n2;
    std::cout << "类成员指针比较:\n"
              << "p3 == p4? " << (p3 == p4) << "\n"  // false
              << "p3 <  p4? " << (p3 < p4) << "\n"; // true

    // 联合成员指针比较
    union Union { int n; double d; };
    Union u;
    int* p5 = &u.n;
    double* p6 = &u.d;
    void* v5 = p5;
    void* v6 = p6;
    std::cout << "联合成员指针比较:\n"
              << "p5 == v6? " << (p5 == v6) << "\n"  // true（同一地址）
              << "p5 <  v6? " << (p5 < v6) << "\n"; // false（相同地址）

    return 0;
}
```

### 算术类型比较示例

```cpp
#include <iostream>
#include <limits>

int main() {
    std::cout << std::boolalpha;

    // 有符号与无符号比较陷阱
    int s = -1;
    unsigned int u = 1;

    std::cout << "有符号与无符号比较:\n"
              << " -1 <  1u ? " << (s < u) << "\n"   // false！陷阱
              << " -1 >  1u ? " << (s > u) << "\n";  // true

    // 解释：-1 转换为 unsigned int 得到 UINT_MAX

    // 浮点数比较
    double d1 = 0.1 + 0.2;
    double d2 = 0.3;
    std::cout << "\n浮点数比较:\n"
              << "0.1 + 0.2 == 0.3 ? " << (d1 == d2) << "\n";  // 可能为 false

    // 特殊浮点值
    double nan = std::numeric_limits<double>::quiet_NaN();
    double inf = std::numeric_limits<double>::infinity();

    std::cout << "\n特殊浮点值比较:\n"
              << "NaN == NaN ? " << (nan == nan) << "\n"    // false
              << "inf == inf ? " << (inf == inf) << "\n";  // true

    return 0;
}
```

### 三向比较运算符完整示例 (C++20)

```cpp
#include <iostream>
#include <compare>
#include <string>

struct Person {
    std::string name;
    int age;

    // C++20: 定义三向比较运算符
    std::strong_ordering operator<=>(const Person& other) const {
        if (auto cmp = name <=> other.name; cmp != 0) {
            return cmp;
        }
        return age <=> other.age;
    }

    // C++20: operator== 会自动生成
    // 但我们也可以显式定义
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

int main() {
    Person alice{"Alice", 30};
    Person bob{"Bob", 25};
    Person alice2{"Alice", 30};

    std::cout << std::boolalpha;

    // 使用自动生成的所有比较运算符
    std::cout << "alice == alice2: " << (alice == alice2) << "\n";  // true
    std::cout << "alice != bob: " << (alice != bob) << "\n";        // true
    std::cout << "alice < bob: " << (alice < bob) << "\n";          // true ("Alice" < "Bob")
    std::cout << "alice <= alice2: " << (alice <= alice2) << "\n";  // true

    // 使用三向比较运算符
    auto result = alice <=> bob;
    if (result < 0) {
        std::cout << "alice 小于 bob\n";
    }

    return 0;
}
```

### 自定义类型的比较运算符重载

```cpp
#include <iostream>
#include <cmath>

class Vector2D {
public:
    double x, y;

    Vector2D(double x, double y) : x(x), y(y) {}

    // 重载相等运算符
    bool operator==(const Vector2D& other) const {
        constexpr double epsilon = 1e-9;
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon;
    }

    // 重载不等运算符
    bool operator!=(const Vector2D& other) const {
        return !(*this == other);
    }

    // 重载小于运算符（用于排序等场景）
    bool operator<(const Vector2D& other) const {
        double len1 = x * x + y * y;
        double len2 = other.x * other.x + other.y * other.y;
        return len1 < len2;
    }

    // 其他关系运算符可以类似实现，或使用 std::rel_ops (C++20 前)
};

int main() {
    Vector2D v1(1.0, 2.0);
    Vector2D v2(1.0, 2.0);
    Vector2D v3(3.0, 4.0);

    std::cout << std::boolalpha;
    std::cout << "v1 == v2: " << (v1 == v2) << "\n";  // true
    std::cout << "v1 < v3: " << (v1 < v3) << "\n";    // true

    return 0;
}
```

### 常见错误及修正

#### 错误 1：有符号与无符号比较

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    int i = -1;

    // ❌ 错误：有符号与无符号比较
    // if (i < v.size()) {  // v.size() 返回 size_t (无符号)
    //     // 这个条件为 false！
    //     std::cout << "条件为真\n";
    // }

    // ✅ 修正：使用有符号类型
    if (i < static_cast<int>(v.size())) {
        std::cout << "条件为真\n";
    }

    // ✅ 更好的修正：检查负值
    if (i >= 0 && static_cast<size_t>(i) < v.size()) {
        std::cout << "索引有效\n";
    }

    return 0;
}
```

#### 错误 2：链式比较误用

```cpp
#include <iostream>

int main() {
    int a = 3, b = 2, c = 1;

    // ❌ 错误：链式比较的误解
    std::cout << std::boolalpha;
    std::cout << "(a < b < c) = " << (a < b < c) << "\n";  // true!
    // 解析为 (a < b) < c = (3 < 2) < 1 = false < 1 = 0 < 1 = true

    // ✅ 正确：使用逻辑运算符
    std::cout << "(a < b && b < c) = " << (a < b && b < c) << "\n";  // false

    // ✅ 更清晰的范围检查
    bool inRange = (b >= a && b <= c);
    std::cout << "b 在 [a, c] 范围内: " << inRange << "\n";

    return 0;
}
```

#### 错误 3：指针与空指针常量比较

```cpp
#include <iostream>

void f(char* p) {
    // ❌ C++20 前已废弃，C++20 起编译错误
    // if (p > 0) { /* ... */ }

    // ❌ 同样错误
    // if (p > nullptr) { /* ... */ }

    // ✅ 正确：只使用相等运算符
    if (p != nullptr) {
        std::cout << "指针非空\n";
    }
}

int main() {
    char* p = nullptr;
    f(p);

    return 0;
}
```

#### 错误 4：数组名比较

```cpp
#include <iostream>
#include <cstring>

int main() {
    char a[] = "hello";
    char b[] = "hello";

    // ❌ 错误：比较数组地址而非内容
    std::cout << std::boolalpha;
    std::cout << "a == b: " << (a == b) << "\n";  // false（比较地址）

    // ✅ 正确：使用字符串比较函数
    std::cout << "strcmp(a, b) == 0: " << (std::strcmp(a, b) == 0) << "\n";  // true

    // ✅ 更好：使用 std::string
    std::string s1 = "hello";
    std::string s2 = "hello";
    std::cout << "s1 == s2: " << (s1 == s2) << "\n";  // true

    return 0;
}
```

## 7. 总结

### 核心要点

比较运算符是 C++ 表达式系统的基础组件，具有以下特点：

1. **丰富的运算符**：包括 6 个双向比较运算符和 1 个三向比较运算符
2. **广泛的支持**：支持算术类型、指针类型、成员指针类型
3. **可重载**：用户定义类型可自定义比较语义
4. **C++20 增强**：三向比较运算符简化了比较操作的定义

### 技术对比

| 运算符类型 | 返回值 | C++ 版本 | 主要用途 |
|-----------|--------|---------|---------|
| `==`, `!=` | `bool` | C++98 | 相等性判断 |
| `<`, `>`, `<=`, `>=` | `bool` | C++98 | 大小关系判断 |
| `<=>` | 比较类别类型 | C++20 | 统一比较，简化代码 |

### 版本演进

| 版本 | 变化 |
|------|------|
| C++98 | 基本比较运算符 |
| C++11 | 支持 `std::nullptr_t` 与空指针比较 |
| C++17 | 函数指针转换支持 |
| C++20 | 三向比较运算符、移除部分废弃特性 |

### 学习建议

1. **理解转换规则**：有符号与无符号比较时，理解通常的算术转换
2. **避免常见陷阱**：链式比较、浮点数相等、数组比较等
3. **善用 C++20 特性**：三向比较运算符可大幅简化代码
4. **遵循严格弱序**：自定义 `operator<` 时确保满足严格弱序要求
5. **注意指针比较**：只在同一数组或对象内进行指针关系比较

### 标准库支持

标准库大量使用了比较运算符：

- **容器类**：`std::map`、`std::set` 等关联容器依赖比较运算符
- **算法类**：`std::sort`、`std::find` 等算法使用比较运算符
- **工具类**：`std::less`、`std::greater` 等比较函数对象
- **智能指针**：`std::shared_ptr`、`std::unique_ptr` 支持比较运算符

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/operator_comparison
- C++ Standard: [expr.rel], [expr.eq], [expr.spaceship]
- "The C++ Programming Language" by Bjarne Stroustrup
- "Effective C++" by Scott Meyers