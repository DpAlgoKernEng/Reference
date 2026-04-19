# 直接初始化 (Direct-initialization)

## 1. 概述

直接初始化 (Direct-initialization) 是 C++ 中一种初始化对象的方式，它从显式指定的构造函数参数集合来初始化对象。与复制初始化 (copy-initialization) 不同，直接初始化会考虑所有构造函数和所有用户定义的转换函数，包括 `explicit` 构造函数。

直接初始化是 C++ 中最灵活的初始化形式之一，它提供了比复制初始化更宽松的类型转换规则。

## 2. 来源与演变

### 首次引入

直接初始化的概念从 C++ 诞生之初就存在，是 C++ 面向对象特性的核心组成部分。

### C++11 变化

- 新增了使用花括号 `{}` 进行直接初始化的形式（形式 2），这与列表初始化 (list-initialization) 相关
- Lambda 表达式中捕获变量的初始化（形式 7）

### C++17 变化

- 对于 prvalue 表达式的处理方式发生重大变化：当初始化器是与目标类型相同的 prvalue 时，直接使用该 prvalue 初始化目标对象，而不是先创建临时对象再复制/移动。这被称为强制省略复制操作 (mandatory copy elision)

### C++20 变化

- 允许使用直接初始化语法初始化数组类型（形式 1），此时数组按照聚合初始化的方式处理，但允许窄化转换 (narrowing conversions)

## 3. 语法与参数

### 语法形式

| 形式 | 语法 | 说明 | 版本 |
|------|------|------|------|
| 1 | `T object(arg);` `T object(arg1, arg2, ...);` | 使用圆括号的直接初始化 | - |
| 2 | `T object{arg};` | 使用花括号的直接初始化 | C++11 起 |
| 3 | `T(other)` `T(arg1, arg2, ...)` | 函数式转型表达式 | - |
| 4 | `static_cast<T>(other)` | static_cast 表达式 | - |
| 5 | `new T(args, ...)` | new 表达式 | - |
| 6 | `Class::Class() : member(args, ...) { ... }` | 构造函数初始化列表 | - |
| 7 | `[arg]() { ... }` | Lambda 捕获初始化 | C++11 起 |

### 适用场景

直接初始化在以下情况下执行：

1. **圆括号初始化**：使用非空的圆括号表达式列表或花括号初始化列表初始化
2. **花括号初始化非类类型**：使用单个花括号包围的初始化器初始化非类类型对象
3. **函数式转型**：通过函数式转型或圆括号表达式列表初始化 prvalue 临时对象（C++17 前）/ prvalue 的结果对象（C++17 起）
4. **static_cast**：通过 static_cast 表达式初始化 prvalue 临时对象（C++17 前）/ prvalue 的结果对象（C++17 起）
5. **new 表达式**：通过带有初始化器的 new 表达式初始化动态存储期的对象
6. **构造函数初始化列表**：初始化基类或非静态成员
7. **Lambda 捕获**：从 Lambda 表达式中按复制捕获的变量初始化闭包对象成员

### 参数说明

- **T**：目标类型，可以是类类型、非类类型或数组类型（C++20 起）
- **arg, arg1, arg2, ...**：传递给构造函数的参数
- **other**：源对象或表达式

## 4. 底层原理

### 初始化过程

直接初始化根据目标类型 `T` 的不同，执行不同的初始化策略：

#### 数组类型 (C++20 起)

```cpp
struct A {
    explicit A(int i = 0) {}
};

A a[2](A(1)); // OK: a[0] 用 A(1) 初始化, a[1] 用 A() 初始化
A b[2]{A(1)}; // 错误: b[1] 的隐式复制列表初始化选择了 explicit 构造函数
```

- 按照**聚合初始化**的方式处理
- 允许**窄化转换** (narrowing conversions)
- 没有初始化器的元素会被**值初始化** (value-initialized)

#### 类类型

**C++17 起**：如果初始化器是与 `T` 相同类型的 prvalue 表达式（忽略 cv 限定符），则直接使用该 prvalue 初始化目标对象，而不需要创建临时对象。

**一般情况**：
1. 考察 `T` 的构造函数
2. 通过重载决议 (overload resolution) 选择最佳匹配
3. 调用选中的构造函数初始化对象

**C++20 起**：如果目标类型是聚合类，按照聚合初始化处理，但有以下区别：
- 允许窄化转换
- 不允许指定初始化器 (designated initializers)
- 绑定到引用的临时对象不会延长生命周期
- 没有花括号省略 (brace elision)
- 没有初始化器的元素会被值初始化

```cpp
struct B {
    int a;
    int&& r;
};

int f();
int n = 10;

B b1{1, f()};            // OK, 生命周期延长
B b2(1, f());            // 合法, 但悬垂引用
B b3{1.0, 1};            // 错误: 窄化转换
B b4(1.0, 1);            // 合法, 但悬垂引用
B b5(1.0, std::move(n)); // OK
```

#### 非类类型（源类型为类类型）

1. 考察源类型及其基类的转换函数
2. 通过重载决议选择最佳匹配
3. 使用选中的用户定义转换进行转换

#### 非类类型（源类型也是非类类型）

1. 如果 `T` 是 `bool` 且源类型是 `std::nullptr_t`，初始化值为 `false`
2. 否则，使用标准转换 (standard conversions) 将源值转换为目标类型

### 与复制初始化的区别

| 特性 | 直接初始化 | 复制初始化 |
|------|-----------|-----------|
| 考虑 explicit 构造函数 | 是 | 否 |
| 考虑 explicit 转换函数 | 是 | 否 |
| 使用圆括号语法 | 是 | 否 |
| 允许窄化转换（花括号形式） | 否（花括号形式） | 否 |

## 5. 使用场景

### 适用场景

1. **需要调用 explicit 构造函数**：当构造函数被声明为 `explicit` 时，必须使用直接初始化

```cpp
std::unique_ptr<int> p(new int(1));  // OK: 直接初始化
// std::unique_ptr<int> p = new int(1); // 错误: 复制初始化不允许 explicit 构造函数
```

2. **需要显式指定多个构造参数**：传递多个参数时，直接初始化语法更清晰

```cpp
std::string s1("test");      // const char* 构造函数
std::string s2(10, 'a');     // 10 个 'a' 字符
```

3. **函数式转型**：需要显式类型转换时

```cpp
double d = 3.14;
int i = int(d);              // 函数式转型
```

4. **成员初始化列表**：在构造函数中初始化基类和成员

```cpp
class Derived : public Base {
    std::string name;
public:
    Derived(const std::string& n) : Base(42), name(n) {} // 直接初始化
};
```

### 注意事项

#### Most Vexing Parse 问题

当使用圆括号形式的直接初始化语法时，可能会与函数声明产生歧义。编译器总是选择将其解释为函数声明：

```cpp
std::string foo1(std::istreambuf_iterator<char>(file),
                 std::istreambuf_iterator<char>());
// 这被解释为函数声明，而不是变量定义！
// 返回类型: std::string
// 第一个参数: std::istreambuf_iterator<char>，名为 file
// 第二个参数: 函数指针类型 std::istreambuf_iterator<char>(*)()
```

**解决方案**：

```cpp
// C++11 前：添加额外圆括号
std::string str1((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());

// C++11 起：使用花括号初始化
std::string str2(std::istreambuf_iterator<char>{file}, {});
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <memory>
#include <string>

struct Foo {
    int mem;
    explicit Foo(int n) : mem(n) {}
};

int main() {
    // 直接初始化：调用构造函数
    std::string s1("test");        // const char* 构造函数
    std::string s2(10, 'a');       // 构造 10 个 'a' 的字符串

    // explicit 构造函数只能使用直接初始化
    std::unique_ptr<int> p(new int(1));  // OK
    // std::unique_ptr<int> p = new int(1); // 错误：explicit 构造函数

    // explicit 类类型
    Foo f(2);               // OK：直接初始化
    // Foo f2 = 2;         // 错误：explicit 构造函数

    std::cout << s1 << ' ' << s2 << ' ' << *p << ' ' << f.mem << '\n';
    // 输出: test aaaaaaaaaa 1 2

    return 0;
}
```

### 函数式转型

```cpp
#include <iostream>

int main() {
    double d = 3.14159;

    // 直接初始化形式的类型转换
    int i = int(d);           // 函数式转型
    double d2 = double(i);    // 转回 double

    // 与 static_cast 对比
    int j = static_cast<int>(d);

    std::cout << i << " " << d2 << " " << j << std::endl;
    // 输出: 3 3 3

    return 0;
}
```

### new 表达式中的直接初始化

```cpp
#include <iostream>

class Point {
public:
    int x, y;
    Point(int a, int b) : x(a), y(b) {
        std::cout << "Point constructed: (" << x << ", " << y << ")\n";
    }
};

int main() {
    // 使用直接初始化创建动态对象
    int* p1 = new int(42);           // 单个 int，值为 42
    Point* p2 = new Point(3, 4);     // 使用构造函数

    std::cout << *p1 << std::endl;    // 输出: 42
    std::cout << "(" << p2->x << ", " << p2->y << ")" << std::endl;

    delete p1;
    delete p2;

    return 0;
}
```

### 成员初始化列表

```cpp
#include <iostream>
#include <string>
#include <vector>

class Student {
    std::string name;
    std::vector<int> scores;
    int id;

public:
    // 直接初始化在成员初始化列表中的应用
    Student(const std::string& n, const std::vector<int>& s, int i)
        : name(n),              // 复制初始化
          scores(s),            // 复制初始化
          id(i)                 // 复制初始化
    {}

    // 使用直接初始化语义
    Student(int i, std::string&& n)
        : name(std::move(n)),   // 移动构造
          scores{100, 90, 85},  // 列表初始化
          id(i)
    {}

    void print() const {
        std::cout << "ID: " << id << ", Name: " << name << std::endl;
    }
};

int main() {
    Student s1("Alice", {90, 85, 92}, 1001);
    Student s2(1002, std::string("Bob"));

    s1.print();
    s2.print();

    return 0;
}
```

### 常见错误及修正

#### 错误 1：Most Vexing Parse

```cpp
#include <fstream>
#include <string>

int main() {
    std::ifstream file("data.txt");

    // 错误：这声明了一个函数，而不是变量！
    std::string str1(std::istreambuf_iterator<char>(file),
                     std::istreambuf_iterator<char>());
    // 编译器将此解释为函数声明

    // 修正 1：使用额外圆括号 (C++11 前)
    std::string str2((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

    // 修正 2：使用花括号 (C++11 起)
    std::string str3{std::istreambuf_iterator<char>{file},
                      std::istreambuf_iterator<char>{}};

    // 修正 3：使用复制初始化（如果适用）
    std::string str4 = std::string(std::istreambuf_iterator<char>(file),
                                    std::istreambuf_iterator<char>());

    return 0;
}
```

#### 错误 2：混淆直接初始化与复制初始化

```cpp
#include <memory>

class Widget {
public:
    explicit Widget(int x) : value(x) {}
    int value;
};

int main() {
    // 直接初始化：OK
    Widget w1(42);
    std::unique_ptr<int> p1(new int(42));

    // 复制初始化：错误（因为构造函数是 explicit）
    // Widget w2 = 42;           // 错误！
    // std::unique_ptr<int> p2 = new int(42); // 错误！

    // 使用 auto 要特别注意
    auto w3 = Widget(42);        // OK：这是直接初始化的 auto 推导
    // auto w4 = 42;             // 这会推导为 int，不是 Widget

    return 0;
}
```

#### 错误 3：花括号直接初始化与列表初始化混淆

```cpp
#include <iostream>
#include <vector>

int main() {
    // 圆括号直接初始化 vs 花括号直接初始化
    std::vector<int> v1(5, 10);   // 5 个元素，每个值为 10
    std::vector<int> v2{5, 10};   // 2 个元素：5 和 10

    std::cout << "v1 size: " << v1.size() << std::endl;  // 输出: 5
    std::cout << "v2 size: " << v2.size() << std::endl;  // 输出: 2

    // 窄化转换检查
    int i1 = 3.14;        // OK（但可能警告）：复制初始化允许窄化
    int i2(3.14);         // OK：直接初始化允许窄化
    // int i3{3.14};      // 错误：列表初始化禁止窄化

    return 0;
}
```

## 7. 总结

直接初始化是 C++ 中灵活且强大的初始化方式，主要特点如下：

| 特性 | 说明 |
|------|------|
| **灵活性** | 可调用所有构造函数，包括 explicit 构造函数 |
| **语法形式** | 圆括号 `T(arg)` 或花括号 `T{arg}`（C++11 起） |
| **类型转换** | 支持所有用户定义转换，包括 explicit 转换函数 |
| **适用范围** | 变量定义、new 表达式、static_cast、构造函数初始化列表等 |

### 核心要点

1. **直接初始化 vs 复制初始化**：直接初始化更宽松，可以调用 explicit 构造函数
2. **Most Vexing Parse**：使用圆括号语法时需注意函数声明歧义问题
3. **花括号形式（C++11 起）**：更安全，禁止窄化转换，可避免 Most Vexing Parse
4. **C++17 强制复制省略**：prvalue 直接初始化目标对象，无需创建临时对象

### 使用建议

1. 需要调用 explicit 构造函数时，使用直接初始化
2. C++11 及以后，优先使用花括号语法避免 Most Vexing Parse
3. 理解直接初始化与复制初始化的区别，选择合适的初始化方式
4. 注意构造函数初始化列表中使用的是直接初始化语义

## 相关概念

| 概念 | 说明 |
|------|------|
| [复制初始化 (copy-initialization)](copy_initialization.md) | 使用 `=` 语法的初始化，不调用 explicit 构造函数 |
| [列表初始化 (list-initialization)](list_initialization.md) | 使用花括号 `{}` 的初始化方式 |
| [聚合初始化 (aggregate initialization)](aggregate_initialization.md) | 对聚合类型使用花括号初始化 |
| [值初始化 (value-initialization)](value_initialization.md) | 使用 `T()` 或 `T{}` 进行的初始化 |
| [默认初始化 (default-initialization)](default_initialization.md) | 不提供初始化器时的初始化方式 |
| [explicit 说明符](../explicit.md) | 指定构造函数和转换函数为显式 |
| [复制省略 (copy elision)](copy_elision.md) | 编译器优化，省略不必要的复制操作 |

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/direct_initialization
- C++ Standard: [dcl.init]
- Effective Modern C++, Scott Meyers, Item 7: Distinguish between () and {} when creating objects