# 默认比较 (C++20 起)

## 1. 概述 (Overview)

**默认比较 (Default comparisons)** 是 C++20 引入的重要特性，允许将比较操作符函数显式默认化 (`= default`)，从而请求编译器为类自动生成相应的默认比较实现。

### 核心概念

**默认化比较操作符函数 (Defaulted comparison operator function)** 是满足以下所有条件的非模板比较操作符函数：

- 是某个类 `C` 的非静态成员或友元
- 在 `C` 中或 `C` 完整的上下文中定义为默认化
- 具有两个 `const C&` 类型的参数或两个 `C` 类型的参数（其中隐式对象参数（如果有）被视为第一个参数）

支持的比较操作符包括：`<=>`（三路比较）、`==`、`!=`、`<`、`>`、`<=`、`>=`。

### 技术定位

默认比较特性位于 C++ 语言核心特性层面，是 C++20 "比较宇宙" (Comparison Universe) 的重要组成部分，与三路比较操作符 (`<=>`)、比较类别类型 (`std::strong_ordering` 等) 协同工作，大幅简化了类的比较操作实现。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++20 之前，为自定义类型实现完整的比较操作需要手动编写大量重复代码：

```cpp
// C++17 及之前的做法
struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
    bool operator>(const Point& other) const {
        return other < *this;
    }
    bool operator<=(const Point& other) const {
        return !(other < *this);
    }
    bool operator>=(const Point& other) const {
        return !(*this < other);
    }
};
```

### 设计动机

- **减少样板代码**：手动实现所有比较操作符繁琐且容易出错
- **保证一致性**：手动实现可能出现逻辑不一致
- **简化维护**：成员变量变化时需要修改多处代码
- **提高可靠性**：编译器生成的代码更可靠

### 版本变更

| 版本 | 变更内容 |
|------|---------|
| C++20 | 首次引入默认比较特性，支持所有比较操作符的默认化 |
| C++20 | 引入三路比较操作符 `<=>`（太空船操作符）|
| C++20 | 引入比较类别类型 (`std::strong_ordering`, `std::weak_ordering`, `std::partial_ordering`) |

### 缺陷报告修正

| DR 编号 | 应用版本 | 原始行为 | 修正行为 |
|---------|---------|---------|---------|
| CWG 2539 | C++20 | 合成三路比较即使显式转换不可用也会选择 static_cast | 此情况下不选择 static_cast |
| CWG 2546 | C++20 | 默认化的次要操作符在重载解析选择不可用重写候选时未定义为删除 | 此情况下定义为删除 |
| CWG 2547 | C++20 | 不清楚非类的比较操作符函数是否可以默认化 | 不能默认化 |
| CWG 2568 | C++20 | 比较操作符函数的隐式定义可能违反成员访问规则 | 访问检查从等价于函数体的上下文执行 |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

#### 三路比较操作符

```cpp
// 成员函数形式
struct ClassName {
    auto operator<=>(const ClassName&) const = default;           // 自动推导返回类型
    std::strong_ordering operator<=>(const ClassName&) const = default; // 显式指定返回类型
};

// 友元函数形式
struct ClassName {
    friend auto operator<=>(const ClassName&, const ClassName&) = default;
};
```

#### 相等比较操作符

```cpp
// 显式声明
struct ClassName {
    bool operator==(const ClassName&) const = default;
};

// 隐式声明（由 operator<=> 自动生成）
struct ClassName {
    auto operator<=>(const ClassName&) const = default;
    // 编译器自动生成：bool operator==(const ClassName&) const = default;
};
```

#### 次要比较操作符

```cpp
struct ClassName {
    bool operator!=(const ClassName&) const = default;
    bool operator<(const ClassName&) const = default;
    bool operator>(const ClassName&) const = default;
    bool operator<=(const ClassName&) const = default;
    bool operator>=(const ClassName&) const = default;
};
```

### 参数要求

默认化比较操作符函数的参数必须满足以下条件之一：

| 形式 | 第一个参数 | 第二个参数 |
|------|-----------|-----------|
| 成员函数 | 隐式对象参数 | `const C&` 或 `C` |
| 友元函数 | `const C&` 或 `C` | `const C&` 或 `C` |

**注意**：两个参数类型必须相同（考虑 cv 限定符）。

### 正确与错误示例对比

```cpp
struct X {
    // ✓ 正确：标准的成员函数形式
    bool operator==(const X&) const = default;

    // ✓ 正确：显式对象参数（C++23 Deducing this）
    bool operator==(this X, X) = default;

    // ✗ 错误：隐式对象参数类型为 X&（非 const）
    bool operator==(const X&) = default;  // 错误
};

struct Y {
    // ✓ 正确：友元函数，参数类型相同
    friend bool operator==(Y, Y) = default;

    // ✗ 错误：参数类型不同
    friend bool operator==(Y, const Y&) = default;  // 错误
};

// ✗ 错误：非友元的非成员函数
bool operator==(const Y&, const Y&) = default;  // 错误
```

## 4. 底层原理 (Underlying Principles)

### 默认比较顺序

编译器为类 `C` 生成比较操作时，会构建**子对象列表 (Subobject List)**，按以下顺序排列：

1. **直接基类子对象**：按声明顺序
2. **非静态数据成员**：按声明顺序

**数组展开规则**：如果成员子对象是数组类型，则展开为其元素序列，按下标递增顺序。展开是递归进行的，直到没有数组类型的子对象。

```cpp
struct S {};

struct T : S {
    int arr[2][2];
} t;

// 对象 t 的子对象列表（5个子对象）：
// (S)t → t[0][0] → t[0][1] → t[1][0] → t[1][1]
```

### 比较类别类型

C++20 定义了三种比较类别类型：

| 类型 | 等价值 | 不可比值 |
|------|--------|---------|
| `std::strong_ordering` | 不可区分 | 不允许 |
| `std::weak_ordering` | 可区分 | 不允许 |
| `std::partial_ordering` | 可区分 | 允许 |

**类型选择指南**：
- **强排序 (strong_ordering)**：等价值完全等价（如整数比较）
- **弱排序 (weak_ordering)**：等价值有区别但比较时视为等价（如不区分大小写的字符串比较）
- **偏序 (partial_ordering)**：存在不可比较的值（如浮点数 NaN）

### 合成三路比较

**合成三路比较 (Synthesized Three-way Comparison)** 用于在不支持 `<=>` 的类型之间构建三路比较。

设 `T` 为目标类型，`a` 和 `b` 为同类型的泛左值：

**判断流程**：

```
1. 如果 a <=> b 的重载解析有可用候选，且能显式转换为 T
   → 合成比较为 static_cast<T>(a <=> b)

2. 否则，如果满足以下任一条件，合成比较未定义：
   - a <=> b 的重载解析找到至少一个可行候选
   - T 不是比较类别类型
   - a == b 的重载解析无可用候选
   - a < b 的重载解析无可用候选

3. 否则，根据 T 的类型：
```

**各类型的合成结果**：

```cpp
// T = std::strong_ordering
a == b ? std::strong_ordering::equal :
a < b  ? std::strong_ordering::less :
         std::strong_ordering::greater

// T = std::weak_ordering
a == b ? std::weak_ordering::equivalent :
a < b  ? std::weak_ordering::less :
         std::weak_ordering::greater

// T = std::partial_ordering
a == b ? std::partial_ordering::equivalent :
a < b  ? std::partial_ordering::less :
b < a  ? std::partial_ordering::greater :
         std::partial_ordering::unordered
```

### 返回类型推导

#### 占位符返回类型 (auto)

当 `operator<=>` 声明返回类型为 `auto` 时，编译器会自动推导：

1. 对每个子对象 `x_i` 执行重载解析 `x_i <=> x_i`
2. 如果重载解析失败，或结果类型不是比较类别类型，则定义为删除
3. 最终返回类型为 `std::common_comparison_category_t<R_1, R_2, ..., R_n>`

```cpp
struct Mixed {
    int i;                    // strong_ordering
    float f;                  // partial_ordering
    auto operator<=>(const Mixed&) const = default;
    // 返回类型推导为 std::partial_ordering
};
```

#### 非占位符返回类型

显式指定返回类型时：

- 不能包含占位符类型（如 `decltype(auto)`）
- 如果某子对象的合成三路比较未定义，则定义为删除

### 比较执行过程

设 `x` 和 `y` 为默认化 `operator<=>` 的参数，子对象分别为 `x_i` 和 `y_i`：

**三路比较执行**：
1. 按递增的 `i` 顺序比较 `x_i` 和 `y_i`
2. 如果某次比较结果 `v_i != 0`（上下文转换为 bool 为 true），立即返回 `v_i` 的副本
3. 所有子对象比较相等时，返回 `static_cast<R>(std::strong_ordering::equal)`

**相等比较执行**：
1. 按递增的 `i` 顺序比较 `x_i == y_i`
2. 如果某次比较结果为 false，立即返回 false
3. 所有子对象比较相等时，返回 true

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 简单数据类

```cpp
struct Point {
    int x, y;
    auto operator<=>(const Point&) const = default;
};

// 自动获得所有比较操作：
// ==, !=, <, >, <=, >=
```

#### 2. 容器元素类型

```cpp
struct Person {
    std::string name;
    int age;
    auto operator<=>(const Person&) const = default;
};

std::set<Person> people;  // 自动排序
std::map<Person, std::string> info;  // 可作为键
```

#### 3. 复杂类型比较

```cpp
struct Employee {
    std::string name;
    int id;
    double salary;
    auto operator<=>(const Employee&) const = default;
};
```

### 最佳实践

#### 推荐做法

```cpp
// ✓ 推荐：使用 auto 推导返回类型
struct Good {
    int value;
    auto operator<=>(const Good&) const = default;
};

// ✓ 推荐：显式指定返回类型以控制比较语义
struct StrongCompare {
    int value;
    std::strong_ordering operator<=>(const StrongCompare&) const = default;
};
```

#### 避免的做法

```cpp
// ✗ 避免：不必要的显式 operator==
struct Redundant {
    int value;
    auto operator<=>(const Redundant&) const = default;
    bool operator==(const Redundant&) const = default; // 不必要
};

// ✗ 避免：参数类型不匹配
struct Wrong {
    bool operator==(const Wrong&) = default;  // 错误：缺少 const
};
```

### 常见陷阱

#### 1. 成员访问权限

```cpp
class Private {
    int secret;
public:
    auto operator<=>(const Private&) const = default;
    // 即使 secret 是私有的，编译器也能访问
};
```

#### 2. 虚函数与默认化

```cpp
struct Base {
    virtual std::strong_ordering operator<=>(const Base&) const = default;
    // 可以将 operator<=> 声明为 virtual
};

struct Derived : Base {
    std::strong_ordering operator<=>(const Base&) const override = default;
    // 注意：派生类的 operator<=> 参数类型仍然是 const Base&
};
```

#### 3. 引用限定符

```cpp
struct RefQualified {
    // ✓ 正确：const 限定的成员函数
    auto operator<=>(const RefQualified&) const & = default;

    // ✗ 错误：右值引用限定符
    // auto operator<=>(const RefQualified&) const && = default;
};
```

#### 4. 比较顺序与性能

```cpp
struct Ordered {
    std::string expensive_field;  // 昂贵的比较
    int cheap_field;              // 廉价的比较

    // 建议：将廉价比较字段放在前面
    auto operator<=>(const Ordered&) const = default;
};
```

### 性能考虑

- **短路求值**：比较在发现不相等元素时立即返回，后续成员不会比较
- **字段顺序**：将最可能不同的字段放在前面可以提高性能
- **比较成本**：将廉价比较字段放在前面，昂贵的放在后面

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：简单类型的默认比较

```cpp
#include <compare>
#include <iostream>
#include <set>

struct Point {
    int x;
    int y;
    auto operator<=>(const Point&) const = default;
};

int main() {
    Point pt1{1, 1}, pt2{1, 2};
    std::set<Point> s;  // OK，Point 有完整的比较操作
    s.insert(pt1);

    // 自动获得所有比较操作符
    std::cout << std::boolalpha
        << (pt1 == pt2) << ' '   // false
        << (pt1 != pt2) << ' '   // true
        << (pt1 <  pt2) << ' '   // true
        << (pt1 <= pt2) << ' '   // true
        << (pt1 >  pt2) << ' '   // false
        << (pt1 >= pt2) << '\n'; // false

    return 0;
}
```

输出：
```
false true true true false false
```

#### 示例 2：显式声明 operator==

```cpp
#include <iostream>

struct Point {
    int x;
    int y;
    bool operator==(const Point&) const = default;
};

int main() {
    Point pt1{3, 5}, pt2{2, 5};
    std::cout << std::boolalpha
        << (pt1 != pt2) << '\n'   // true
        << (pt1 == pt1) << '\n';  // true

    // 无名结构体，未定义比较操作
    struct { int x{}, y{}; } p, q;
    // if (p == q) {} // 错误：operator== 未定义

    return 0;
}
```

输出：
```
true
true
```

### 高级用法

#### 示例 3：混合比较类别

```cpp
#include <compare>
#include <iostream>
#include <string>

struct Mixed {
    int integer;              // strong_ordering
    float floating;           // partial_ordering

    auto operator<=>(const Mixed& other) const = default;
    // 返回类型自动推导为 std::partial_ordering
};

int main() {
    Mixed a{1, 1.0f}, b{1, 2.0f};

    auto result = a <=> b;

    if (result == std::partial_ordering::less) {
        std::cout << "a < b\n";
    }

    return 0;
}
```

#### 示例 4：隐式生成 operator==

```cpp
#include <compare>

template<typename T>
struct X {
    friend constexpr std::partial_ordering operator<=>(X, X)
        requires (sizeof(T) != 1) = default;
    // 隐式声明：friend constexpr bool operator==(X, X)
    //               requires (sizeof(T) != 1) = default;

    [[nodiscard]] virtual std::strong_ordering operator<=>(const X&) const = default;
    // 隐式声明：[[nodiscard]] virtual bool operator==(const X&) const = default;
};

int main() {
    X<int> a, b;
    // a == b 自动可用
    return 0;
}
```

#### 示例 5：自定义比较顺序

```cpp
#include <compare>
#include <string>

struct Person {
    std::string name;
    int age;

    // 自定义比较：先按年龄，再按姓名
    std::strong_ordering operator<=>(const Person& other) const {
        if (auto cmp = age <=> other.age; cmp != 0)
            return cmp;
        return name <=> other.name;
    }

    // 使用默认的 operator==
    bool operator==(const Person&) const = default;
};
```

### 常见错误及修正

#### 错误 1：参数类型不匹配

```cpp
// ✗ 错误
struct Wrong {
    bool operator==(const Wrong&) = default;  // 错误：缺少 const
};

// ✓ 修正
struct Correct {
    bool operator==(const Wrong&) const = default;  // 正确
};
```

#### 错误 2：友元函数非类成员

```cpp
struct Y {
    friend bool operator==(Y, Y) = default;  // ✓ 正确
};

// ✗ 错误：不是 Y 的友元
bool operator==(const Y&, const Y&) = default;
```

#### 错误 3：次要比较操作符的误解

```cpp
struct HasNoRelational {};

struct C {
    friend HasNoRelational operator<=>(const C&, const C&);
    // operator< 返回 bool，但 operator<=> 返回 HasNoRelational
    bool operator<(const C&) const = default;  // OK，函数被默认化
};

// 默认的 operator< 会调用 operator<=>，然后隐式转换为 bool
```

#### 错误 4：返回类型不支持 bool 转换

```cpp
struct BadCompare {};

struct D {
    friend BadCompare operator<=>(const D&, const D&);
    bool operator<(const D&) const = default;  // 错误：BadCompare 不能转换为 bool
};
```

### 完整示例：可排序的自定义类型

```cpp
#include <compare>
#include <iostream>
#include <set>
#include <string>
#include <vector>

class Student {
private:
    std::string name_;
    int id_;
    std::vector<int> scores_;

public:
    Student(std::string name, int id, std::vector<int> scores)
        : name_(std::move(name)), id_(id), scores_(std::move(scores)) {}

    // 默认比较：先比较 id，再比较 name，最后比较 scores
    auto operator<=>(const Student&) const = default;

    const std::string& name() const { return name_; }
    int id() const { return id_; }
};

int main() {
    std::set<Student> students;

    students.emplace("Alice", 1001, std::vector<int>{90, 85, 88});
    students.emplace("Bob", 1002, std::vector<int>{85, 90, 92});
    students.emplace("Charlie", 1000, std::vector<int>{88, 87, 85});

    std::cout << "Students sorted by id then name:\n";
    for (const auto& s : students) {
        std::cout << "  " << s.id() << ": " << s.name() << '\n';
    }

    Student s1{"Alice", 1001, {90, 85}};
    Student s2{"Alice", 1001, {90, 85, 88}};

    std::cout << "s1 == s2: " << (s1 == s2) << '\n';  // false（scores 不同）
    std::cout << "s1 < s2: " << (s1 < s2) << '\n';    // true

    return 0;
}
```

输出：
```
Students sorted by id then name:
  1000: Charlie
  1001: Alice
  1002: Bob
s1 == s2: false
s1 < s2: true
```

## 7. 总结 (Summary)

### 核心要点

1. **语法简洁**：`= default` 一行代码替代六种比较操作符的手动实现
2. **自动推导**：使用 `auto` 返回类型可自动推导比较类别
3. **隐式生成**：`operator<=>` 会自动生成 `operator==`
4. **比较顺序**：基类→成员，按声明顺序；数组递归展开
5. **短路求值**：发现不相等立即返回，提高效率

### 比较操作符对照表

| 操作符 | 默认化支持 | 返回类型 | 是否自动生成 |
|--------|-----------|---------|-------------|
| `<=>` | ✓ | auto 或比较类别 | - |
| `==` | ✓ | bool | 由 `<=>` 自动生成 |
| `!=` | ✓ | bool | 可从 `==` 重写 |
| `<` | ✓ | bool | 可从 `<=>` 重写 |
| `>` | ✓ | bool | 可从 `<=>` 重写 |
| `<=` | ✓ | bool | 可从 `<=>` 重写 |
| `>=` | ✓ | bool | 可从 `<=>` 重写 |

### 比较类别类型选择指南

| 场景 | 推荐类型 | 示例 |
|------|---------|------|
| 算术类型、指针 | `std::strong_ordering` | `int`, `T*` |
| 大小写不敏感比较 | `std::weak_ordering` | `CaseInsensitiveString` |
| 浮点数、可能有无穷大或 NaN | `std::partial_ordering` | `float`, `double` |
| 不确定 | `auto` | 由编译器推导 |

### 学习建议

1. **从简单开始**：先用 `auto operator<=>(const T&) const = default` 实现
2. **理解比较类别**：掌握 `strong_ordering`、`weak_ordering`、`partial_ordering` 的区别
3. **注意字段顺序**：影响比较性能和结果
4. **测试边界情况**：NaN、空容器、自比较等
5. **阅读缺陷报告**：了解实现细节和边界情况

### 相关主题

- 三路比较操作符 (`<=>`)
- 比较类别类型 (`<compare>` 头文件)
- 操作符重载
- 重载解析与重写候选

### 参考资源

- [cppreference: Default comparisons](https://en.cppreference.com/w/cpp/language/default_comparisons)
- [cppreference: Operator overloading](https://en.cppreference.com/w/cpp/language/operators)
- C++20 标准文档 §11.11 比较操作符