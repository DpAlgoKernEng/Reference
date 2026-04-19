# C++ 表达式 (Expressions)

## 1. 概述 (Overview)

### 定义

表达式 (Expression) 是由**操作符 (Operators)** 和**操作数 (Operands)** 组成的序列，用于指定一个计算操作。

### 核心特性

每个 C++ 表达式都有两个独立的属性：
- **类型 (Type)**：表达式的数据类型
- **值类别 (Value Category)**：表达式的值分类

### 表达式求值

表达式求值可能产生两种结果：
1. **计算结果**：例如 `2 + 2` 的求值结果是 `4`
2. **副作用 (Side Effects)**：例如 `std::printf("%d", 4)` 会在标准输出打印字符 '4'

### 主要组成

表达式系统包含以下核心概念：
- 值类别 (Value Categories)
- 操作符 (Operators)
- 类型转换 (Conversions)
- 内存分配 (Memory Allocation)
- 主表达式 (Primary Expressions)
- 字面量 (Literals)

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 表达式系统继承自 C 语言，并在标准演进过程中不断扩展和完善。

### 版本变更

| 版本 | 主要变更 |
|------|---------|
| C++98 | 基础表达式系统，包含 lvalue 和 rvalue 两类值类别 |
| C++11 | 引入 xvalue、glvalue、prvalue 值类别；新增 lambda 表达式、nullptr 字面量、noexcept 操作符、alignof 操作符 |
| C++14 | 完善聚合初始化中的完整表达式定义；改进成员初始化器语义 |
| C++17 | 引入 fold expressions（折叠表达式）；弃值表达式的临时量具体化；临时量具体化规则调整 |
| C++20 | 新增 requires expressions（约束表达式）、spaceship operator（三路比较操作符 `<=>`）、char8_t 字面量、立即调用概念 |
| C++26 | 引入 pack indexing expression（包索引表达式） |

### 解决的问题

- **类型安全**：通过严格的类型系统和值类别分类，提高代码安全性
- **表达式优化**：明确的求值顺序规则，便于编译器优化
- **语义清晰**：区分不同值类别，支持移动语义等高级特性
- **扩展性**：操作符重载机制允许用户自定义类型的表达式行为

---

## 3. 语法与参数 (Syntax and Parameters)

### 值类别 (Value Categories)

表达式按其值特性分类：

| 类别 | 英文 | 说明 | 引入版本 |
|------|------|------|---------|
| 左值 | lvalue | 有身份，不可移动 | C++98 |
| 右值 | rvalue | 可移动 | C++98 |
| 广义左值 | glvalue | 有身份（lvalue 或 xvalue） | C++11 |
| 纯右值 | prvalue | 可移动，无身份 | C++11 |
| 亡值 | xvalue | 有身份，可移动 | C++11 |

### 操作符分类

#### 常用操作符

| 类型 | 操作符示例 |
|------|-----------|
| 赋值 | `a = b`, `a += b`, `a -= b`, `a *= b`, `a /= b`, `a %= b`, `a &= b`, `a |= b`, `a ^= b`, `a <<= b`, `a >>= b` |
| 自增/自减 | `++a`, `--a`, `a++`, `a--` |
| 算术 | `+a`, `-a`, `a + b`, `a - b`, `a * b`, `a / b`, `a % b`, `~a`, `a & b`, `a | b`, `a ^ b`, `a << b`, `a >> b` |
| 逻辑 | `!a`, `a && b`, `a || b` |
| 比较 | `a == b`, `a != b`, `a < b`, `a > b`, `a <= b`, `a >= b`, `a <=> b` (C++20) |
| 成员访问 | `a[...]`, `*a`, `&a`, `a->b`, `a.b`, `a->*b`, `a.*b` |
| 其他 | 逗号 `a, b`，条件 `a ? b : c`，函数调用 `a(...)` |

#### 特殊操作符

| 操作符 | 功能 | 引入版本 |
|--------|------|---------|
| `static_cast` | 相关类型之间的转换 | C++98 |
| `dynamic_cast` | 继承层次内的类型转换 | C++98 |
| `const_cast` | 添加或移除 cv 限定符 | C++98 |
| `reinterpret_cast` | 不相关类型之间的转换 | C++98 |
| C 风格转换 | 混合使用上述转换 | C++98 |
| `new` | 动态内存分配 | C++98 |
| `delete` | 释放动态内存 | C++98 |
| `sizeof` | 查询类型大小 | C++98 |
| `sizeof...` | 查询包大小 | C++11 |
| `typeid` | 查询类型信息 | C++98 |
| `noexcept` | 检查表达式是否可能抛异常 | C++11 |
| `alignof` | 查询类型对齐要求 | C++11 |

### 类型转换

- **标准转换**：类型间的隐式转换
- `const_cast`：常量性转换
- `static_cast`：静态类型转换
- `dynamic_cast`：动态类型转换
- `reinterpret_cast`：重新解释转换
- **显式转换**：C 风格转换和函数式转换
- **用户定义转换**：自定义类型的转换

### 主表达式 (Primary Expressions)

主表达式是表达式的基本构建块：

- `this` 指针
- 字面量（如 `2`, `"Hello, world"`）
- 标识符表达式
  - 非限定标识符（如 `n`, `cout`）
  - 限定标识符（如 `std::string::npos`）
  - 声明器中待声明的标识符
- 包索引表达式 (C++26)
- lambda 表达式 (C++11)
- fold 表达式 (C++17)
- requires 表达式 (C++20)

### 字面量类型

| 字面量类型 | 说明 | 引入版本 |
|-----------|------|---------|
| 整型字面量 | 十进制、八进制、十六进制、二进制 | C++98 |
| 字符字面量 | `char`, `wchar_t` | C++98 |
| 字符字面量 | `char16_t`, `char32_t` | C++11 |
| 字符字面量 | `char8_t` | C++20 |
| 浮点字面量 | `float`, `double`, `long double` | C++98 |
| 字符串字面量 | `const char[]`, `const wchar_t[]` | C++98 |
| 字符串字面量 | `const char16_t[]`, `const char32_t[]` | C++11 |
| 字符串字面量 | `const char8_t[]` | C++20 |
| 布尔字面量 | `true`, `false` | C++98 |
| 指针字面量 | `nullptr` | C++11 |
| 用户定义字面量 | 自定义类型的常量值 | C++11 |

---

## 4. 底层原理 (Underlying Principles)

### 完整表达式 (Full-expression)

**组成表达式 (Constituent Expression)** 定义：
1. 表达式的组成表达式就是该表达式本身
2. 花括号初始化列表或表达式列表的组成表达式是其各元素的组成表达式
3. 形如 `=` 初始化子句的组成表达式是该初始化子句的组成表达式

```cpp
int num1 = 0;
num1 += 1;  // 情况 1: `num += 1` 的组成表达式就是 `num += 1`

int arr2[2] = {2, 22};  // 情况 2: `{2, 22}` 的组成表达式是 `2` 和 `22`
                        // 情况 3: `= {2, 22}` 的组成表达式也是 `2` 和 `22`
```

**立即子表达式 (Immediate Subexpression)**：
- 操作数的组成表达式
- (C++14 起) 如果表达式创建聚合对象，每个使用的默认成员初始化器的组成表达式
- (C++11 起) 如果是 lambda 表达式，复制捕获实体的初始化和捕获初始化器的组成表达式
- 隐式调用的任何函数调用
- 如果是函数调用，使用的每个默认参数的组成表达式

**完整表达式 (Full-expression)** 包括：
1. 未求值操作数
2. 常量表达式
3. (C++20 起) 立即调用
4. 简单声明的声明器或成员初始化器，包括初始化器的组成表达式
5. 对象生命周期结束时调用的析构函数（生命周期未被扩展的临时对象除外）
6. 不是其他表达式的子表达式且不属于其他完整表达式的表达式

### 潜在求值表达式 (Potentially-evaluated Expression)

**未求值操作数 (Unevaluated Operands)**：
- `typeid` 操作符的操作数（多态类类型的 glvalue 除外）
- `sizeof` 操作符的操作数
- `noexcept` 操作符的操作数
- `decltype` 说明符的操作数
- (C++20 起) 概念定义的约束表达式
- (C++20 起) requires 子句中 requires 关键字后的表达式
- (C++20 起) requires 表达式中要求序列里的表达式

**潜在求值表达式**：不是未求值操作数，也不是未求值操作数的子表达式。

潜在求值表达式会触发 ODR 使用 (ODR-use)。

### 弃值表达式 (Discarded-value Expression)

**定义**：仅用于副作用，计算值被丢弃的表达式。

**适用场景**：
- 表达式语句的完整表达式
- 内置逗号操作符的左操作数
- 转换到 `void` 类型的转换表达式的操作数

**转换规则**：
- 数组到指针和函数到指针的转换永远不会应用
- 仅当表达式是 volatile 限定的 glvalue 且具有特定形式时，才应用左值到右值转换：
  - id 表达式
  - 数组下标表达式
  - 类成员访问表达式
  - 间接访问
  - 指向成员操作
  - 第二和第三操作数都是这些表达式的条件表达式
  - 右操作数是这些表达式的逗号表达式

### 表达式等价 (Expression-equivalence)

(C++20 起) 多个表达式 e1, e2, ..., eN 满足以下条件时**表达式等价**：
1. 它们具有相同的效果
2. 要么都是常量子表达式，要么都不是
3. 要么都是 noexcept，要么都不是

---

## 5. 使用场景 (Use Cases)

### 值类别的应用

| 场景 | 值类别要求 | 说明 |
|------|-----------|------|
| 绑定到非 const 左值引用 | lvalue | 只能绑定左值 |
| 移动语义 | rvalue (prvalue/xvalue) | 移动构造函数接受右值 |
| 完美转发 | 保留原值类别 | `std::forward` 保持值类别 |
| 返回值优化 | prvalue | 允许复制消除 |

### 操作符优先级

操作符优先级定义了操作符绑定到参数的顺序。使用括号可以明确优先级，括号具有最高优先级。

### 操作符重载

操作符重载允许为用户定义类型指定操作符行为：
- 大多数操作符可重载
- 保持语义一致性
- 不能改变优先级和结合性

### 求值顺序注意事项

- 函数参数的求值顺序未指定
- 子表达式的求值顺序需要特别关注
- 使用序列点规则（C++17 前）或序列关系规则（C++17 起）

### 常见陷阱

1. **未定义行为的求值顺序**：
```cpp
int i = 0;
int arr[] = {i++, i++, i++};  // 未定义行为
```

2. **临时对象生命周期**：
```cpp
const int& r = 42;  // OK: 临时量生命周期延长
int&& rr = 42;      // OK: 右值引用绑定到临时量
```

3. **volatile 变量的弃值表达式**：
```cpp
volatile int v = 0;
v;  // 触发左值到右值转换，产生读取操作
```

---

## 6. 代码示例 (Examples)

### 基础示例：主表达式和字面量

```cpp
#include <iostream>
#include <string>

int main() {
    // 主表达式：字面量
    int a = 42;              // 整型字面量
    double b = 3.14;         // 浮点字面量
    bool flag = true;         // 布尔字面量
    char c = 'A';            // 字符字面量
    const char* s = "hello"; // 字符串字面量

    // C++11: nullptr 字面量
    int* ptr = nullptr;

    // 主表达式：标识符
    std::cout << a << std::endl;  // cout 和 endl 是标识符

    // 主表达式：this 指针（在成员函数中）
    struct Point {
        int x, y;
        void print() {
            std::cout << this->x << ", " << this->y << std::endl;
        }
    };

    Point p{1, 2};
    p.print();  // 输出: 1, 2

    return 0;
}
```

### 完整表达式示例

```cpp
#include <iostream>

int main() {
    // 完整表达式示例
    int num = 0;
    num += 1;  // 完整表达式是 `num += 1`

    // 初始化器中的完整表达式
    int arr[2] = {2, 22};  // 每个初始化器都是完整表达式的一部分

    // 函数调用作为完整表达式
    std::cout << "Hello" << std::endl;

    // 弃值表达式
    42;              // 字面量被丢弃（可能产生警告）
    (void)42;        // 显式丢弃，无警告

    // 逗号表达式的弃值
    int x = 0;
    x++, x++;        // 左操作数是弃值表达式

    return 0;
}
```

### 值类别示例

```cpp
#include <iostream>
#include <utility>
#include <string>

struct Widget {
    std::string name;

    Widget(const std::string& n) : name(n) {
        std::cout << "构造: " << name << std::endl;
    }

    Widget(const Widget& w) : name(w.name + "_copy") {
        std::cout << "拷贝构造: " << name << std::endl;
    }

    Widget(Widget&& w) noexcept : name(std::move(w.name)) {
        std::cout << "移动构造: " << name << std::endl;
    }

    ~Widget() {
        std::cout << "析构: " << name << std::endl;
    }
};

Widget createWidget() {
    return Widget("temp");  // 返回 prvalue
}

int main() {
    // lvalue: 有身份，不可移动
    Widget w1("lvalue");

    // prvalue: 可移动，无身份
    Widget w2 = Widget("prvalue");

    // xvalue: 有身份，可移动
    Widget w3 = std::move(w1);  // std::move 将 lvalue 转换为 xvalue

    // 函数返回 prvalue
    Widget w4 = createWidget();

    return 0;
}
```

### 类型转换示例

```cpp
#include <iostream>
#include <memory>
#include <stdexcept>

class Base {
public:
    virtual void foo() { std::cout << "Base::foo" << std::endl; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void foo() override { std::cout << "Derived::foo" << std::endl; }
    void bar() { std::cout << "Derived::bar" << std::endl; }
};

int main() {
    // static_cast: 相关类型转换
    double d = 3.14;
    int i = static_cast<int>(d);
    std::cout << "static_cast: " << i << std::endl;

    // dynamic_cast: 多态类型转换
    Base* base = new Derived();
    Derived* derived = dynamic_cast<Derived*>(base);
    if (derived) {
        derived->bar();
    }
    delete base;

    // const_cast: 移除 const
    const int ci = 42;
    int& ri = const_cast<int&>(ci);
    ri = 100;  // 未定义行为（原对象是 const）
    std::cout << "const_cast: " << ci << std::endl;

    // reinterpret_cast: 不相关类型转换
    int* p = new int(42);
    long addr = reinterpret_cast<long>(p);
    std::cout << "reinterpret_cast: " << addr << std::endl;
    delete p;

    return 0;
}
```

### Lambda 表达式示例 (C++11)

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // 基本 lambda
    auto greet = []() { std::cout << "Hello, Lambda!" << std::endl; };
    greet();

    // 带参数的 lambda
    auto add = [](int a, int b) { return a + b; };
    std::cout << "add(3, 4) = " << add(3, 4) << std::endl;

    // 捕获外部变量
    int x = 10;
    auto capture_by_value = [x]() { return x * 2; };
    auto capture_by_ref = [&x]() { x *= 2; };

    std::cout << "capture_by_value() = " << capture_by_value() << std::endl;
    capture_by_ref();
    std::cout << "after capture_by_ref, x = " << x << std::endl;

    // 泛型 lambda (C++14)
    auto generic_add = [](auto a, auto b) { return a + b; };
    std::cout << "generic_add(1, 2.5) = " << generic_add(1, 2.5) << std::endl;

    // 使用 lambda 的算法
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    int count = std::count_if(v.begin(), v.end(), [](int n) { return n > 3; });
    std::cout << "Count > 3: " << count << std::endl;

    return 0;
}
```

### 常见错误及修正

#### 错误 1：未定义行为的求值顺序

```cpp
#include <iostream>

// 错误示例
void bad_example() {
    int i = 0;
    int arr[] = {i++, i++, i++};  // 未定义行为！
    // 输出可能不一致
    std::cout << arr[0] << arr[1] << arr[2] << std::endl;
}

// 正确示例
void good_example() {
    int i = 0;
    int arr[] = {0, 1, 2};  // 明确的值
    i = 3;  // 明确的赋值
    std::cout << arr[0] << arr[1] << arr[2] << std::endl;
}

int main() {
    bad_example();   // 不推荐
    good_example();  // 推荐
    return 0;
}
```

#### 错误 2：返回局部变量的引用

```cpp
#include <iostream>
#include <string>

// 错误示例：返回局部变量的引用
int& bad_ref() {
    int x = 42;
    return x;  // 返回局部变量的引用，未定义行为！
}

// 正确示例：返回值
int good_value() {
    int x = 42;
    return x;  // 返回值，安全
}

// 正确示例：返回右值引用
std::string&& good_rvalue_ref() {
    return std::move(std::string("hello"));  // 但仍有风险
}

int main() {
    // int& r = bad_ref();  // 未定义行为
    int v = good_value();  // 安全
    std::cout << v << std::endl;
    return 0;
}
```

#### 错误 3：const_cast 滥用

```cpp
#include <iostream>

// 错误示例：修改 const 对象
void bad_const_cast() {
    const int ci = 42;
    int& ri = const_cast<int&>(ci);
    ri = 100;  // 未定义行为！原对象是 const
    std::cout << ci << std::endl;  // 可能输出 42 或 100
}

// 正确示例：const_cast 用于兼容旧代码
void good_const_cast() {
    int i = 42;
    const int& cri = i;
    // 假设某个旧函数不接受 const 参数
    int& ri = const_cast<int&>(cri);
    ri = 100;  // OK，原对象不是 const
    std::cout << i << std::endl;  // 输出 100
}

int main() {
    bad_const_cast();   // 不推荐
    good_const_cast();  // 推荐
    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

1. **表达式是 C++ 程序的基本计算单元**，由操作符和操作数组成，具有类型和值类别两个属性。

2. **值类别分类**（C++11 起）：
   - **glvalue（广义左值）**：有身份
   - **rvalue（右值）**：可移动
   - **lvalue（左值）**：有身份，不可移动
   - **xvalue（亡值）**：有身份，可移动
   - **prvalue（纯右值）**：无身份，可移动

3. **操作符优先级和求值顺序**是理解表达式行为的关键，使用括号可以明确优先级。

4. **类型转换机制**提供了丰富的类型间转换能力，包括标准转换、各种 cast 操作符和用户定义转换。

5. **完整表达式**定义了表达式求值的边界，对理解对象生命周期和求值顺序至关重要。

### 技术对比

| 特性 | C++98 | C++11 | C++17 | C++20 |
|------|-------|-------|-------|-------|
| 值类别 | lvalue/rvalue | 5 类值类别 | 5 类值类别 | 5 类值类别 |
| lambda | 无 | 有 | 有 | 有 |
| fold expressions | 无 | 无 | 有 | 有 |
| requires expressions | 无 | 无 | 无 | 有 |
| `<=>` 操作符 | 无 | 无 | 无 | 有 |
| `char8_t` | 无 | 无 | 无 | 有 |

### 学习建议

1. **从基础开始**：先掌握主表达式、字面量和基本操作符，再学习复杂的表达式规则。

2. **理解值类别**：值类别是现代 C++ 的核心概念，对理解移动语义和完美转发至关重要。

3. **实践类型转换**：通过编写示例代码理解各种 cast 的适用场景和限制。

4. **关注求值顺序**：避免编写依赖未指定求值顺序的代码，使用序列关系规则（C++17 起）。

5. **阅读标准文档**：cppreference 是优秀的参考资料，遇到疑问时查阅标准条款。

### 深入学习资源

- C++ 标准文档：ISO/IEC 14882
- cppreference：https://en.cppreference.com/w/cpp/language/expressions
- 《C++ Primer》：表达式章节
- 《Effective C++》：条款 20-27（实现、操作符重载）

---

## 参考资料

- cppreference: [Expressions](https://en.cppreference.com/w/cpp/language/expressions)
- C++ 标准文档：§7 [expr]
- 文档最后修改：2024年9月9日