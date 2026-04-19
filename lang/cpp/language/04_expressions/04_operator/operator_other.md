# C++ 其他运算符 (Other Operators)

## 1. 概述 (Overview)

C++ 提供了三个重要的运算符：**函数调用运算符 (function call operator)**、**逗号运算符 (comma operator)** 和**条件运算符 (conditional operator)**。这些运算符为 C++ 提供了灵活的表达能力：

- **函数调用运算符** `()`：为任何对象提供函数语义，是函数对象（functor）的核心机制
- **逗号运算符** `,`：允许在一个表达式中顺序执行多个子表达式
- **条件运算符** `? :`：唯一的三元运算符，根据条件选择执行两个表达式之一

### 运算符概览表

| 运算符名称 | 语法 | 可重载 | 类内原型示例 | 类外原型示例 |
|-----------|------|--------|------------|------------|
| 函数调用 | `a(a1, a2)` | 是 | `R T::operator()(Arg1 &a1, Arg2 &a2, ...);` | N/A |
| 逗号 | `a, b` | 是 | `T2& T::operator,(T2 &b);` | `T2& operator,(const T &a, T2 &b);` |
| 条件运算符 | `a ? b : c` | 否 | N/A | N/A |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

这三个运算符都源自 C 语言，在 C++ 中得到了显著增强：

**函数调用运算符**
- C++ 引入了运算符重载机制，使得 `operator()` 可以被重载
- 这是 C++ 函数对象（functor）设计模式的基础
- 标准库大量使用此特性（如 `std::function`、`std::plus` 等）

**逗号运算符**
- 继承自 C 语言，用于在只能放一个表达式的地方执行多个操作
- C++ 允许重载，但标准库未使用此特性
- C++17 明确了求值顺序的保证

**条件运算符**
- 继承自 C 语言的三元运算符
- C++ 不允许重载，保证其语义的一致性
- C++11 引入了返回类型推导规则（支持协变返回类型）

### 版本演变

| 版本 | 变更内容 |
|------|---------|
| C++11 | 条件运算符支持 `std::nullptr_t`；支持协变返回类型；引入右值引用语义 |
| C++17 | 函数调用表达式中，函数表达式在所有参数之前求值；逗号运算符保证求值顺序 |
| C++20 | 弃用在下标运算符中使用未加括号的逗号表达式 |
| C++23 | 禁止在下标运算符中使用未加括号的逗号表达式 |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 函数调用运算符

#### 语法形式

```
function(arg1, arg2, arg3, ...)
```

#### 参数说明

| 参数 | 说明 |
|------|------|
| `function` | 函数类型的左值或函数指针类型的纯右值；对于成员函数调用，可以是成员函数指针或对象表达式 |
| `arg1, arg2, ...` | 任意表达式列表或花括号初始化列表（C++11 起），顶层不允许使用逗号运算符 |

#### 函数调用特性

- 对于非成员函数或静态成员函数：`function` 可以是引用函数的左值（此时函数到指针的转换被抑制），或函数指针类型的纯右值
- 函数名可以重载，通过重载决议决定调用哪个版本
- 如果是虚函数，会在运行时通过动态分发调用最终覆盖者

#### 求值顺序

| 版本 | 求值顺序规则 |
|------|------------|
| C++17 之前 | 函数表达式和所有参数表达式的求值顺序不确定，相互之间无序 |
| C++17 起 | 函数表达式在所有参数及默认参数之前求值；参数表达式之间的求值顺序不确定但顺序确定 |

### 3.2 逗号运算符

#### 语法形式

```
E1, E2
```

#### 语义说明

在逗号表达式 `E1, E2` 中：
1. 首先求值 `E1`，结果被丢弃（如果具有类类型，其析构在包含的完整表达式结束时发生）
2. `E1` 的所有副作用在 `E2` 求值开始前完成
3. 结果的类型、值和值类别与 `E2` 完全相同

#### 重要区别

逗号在各种逗号分隔的列表中（如函数参数列表 `f(a, b, c)`、初始化列表 `int a[] = {1, 2, 3}`）**不是**逗号运算符。如需在这些上下文中使用逗号运算符，必须加括号：`f(a, (n++, n + b), c)`。

#### 下标运算符中的使用限制

| 版本 | 规则 |
|------|------|
| C++20-C++23 | 弃用在下标运算符的第二个参数中使用未加括号的逗号表达式 |
| C++23 起 | 禁止在下标运算符的第二个参数中使用未加括号的逗号表达式，如 `a[b, c]` 非法或被解释为 `a.operator[](b, c)` |

### 3.3 条件运算符

#### 语法形式

```
E1 ? E2 : E3
```

#### 求值语义

1. 首先求值 `E1` 并按语境转换为 `bool`
2. `E1` 的值计算和所有副作用完成后：
   - 如果结果为 `true`，求值 `E2`
   - 如果结果为 `false`，求值 `E3`
3. 未被选中的表达式不会被求值

#### 类型确定规则

条件表达式的类型和值类别由以下规则确定：

**规则 1：E2 或 E3 具有 void 类型**
- 如果两者都是 void，结果是 void 类型的纯右值
- 如果 void 类型的操作数是（可能带括号的）throw 表达式，结果具有另一个表达式的类型和值类别
- 否则，程序非良构

**规则 2：E2 和 E3 是相同类型的位域**
- 如果 E2 和 E3 是相同值类别的泛左值位域，类型为 `cv1 T` 和 `cv2 T`，则操作数被视为 `cv T` 类型（cv 是 cv1 和 cv2 的并集）

**规则 3：E2 和 E3 类型不同**
- 尝试形成隐式转换序列，使两者能转换为共同类型
- 如果两者能互相转换，或形成歧义转换序列，程序非良构
- 如果恰好一个方向能形成转换序列，则应用该转换

**规则 4：E2 和 E3 类型相同**
- C++11 前：如果两者是相同类型的左值，结果是该类型的左值
- C++11 起：如果两者是相同类型和相同值类别的泛左值，结果具有相同的类型和值类别
- 否则，结果是纯右值

**规则 5：结果类型的计算**
- 如果 E2 和 E3 是算术或枚举类型，应用通常的算术转换
- 如果至少一个是指针，应用指针转换以获得复合指针类型
- 如果至少一个是指向成员指针，应用指向成员指针转换
- C++11 起：如果两者都是空指针常量且至少一个类型是 `std::nullptr_t`，结果类型为 `std::nullptr_t`

## 4. 底层原理 (Underlying Principles)

### 4.1 函数调用实现机制

#### 参数传递

1. **参数初始化**：每个函数参数用对应参数初始化（必要时进行隐式转换）
2. **默认参数**：如果没有对应参数，使用默认参数
3. **this 指针转换**：对于成员函数调用，this 指针转换为函数期望的类型
4. **参数生命周期**：参数的初始化和析构发生在包含函数调用的完整表达式的上下文中

#### 返回值优化

C++17 起，对于传递给或从函数返回的类类型对象，如果满足以下条件：
- 所有拷贝/移动构造函数和析构函数都是平凡或删除的
- 至少有一个未删除的拷贝或移动构造函数

实现可以创建临时对象保存参数或返回值，允许小对象（如 `std::complex`、`std::span`）在寄存器中传递。

#### 值类别

函数调用表达式的值类别：
- **左值**：如果函数返回左值引用或函数的右值引用
- **亡值**：如果函数返回对象的右值引用
- **纯右值**：其他情况

#### 变参函数

对于变参函数（variadic function），匹配省略号参数的所有参数应用默认参数提升。

### 4.2 逗号运算符求值机制

#### 求值顺序保证

逗号运算符提供严格的前序定序（sequenced-before）关系：
1. `E1` 被求值
2. `E1` 的值计算和所有副作用完成
3. `E2` 被求值
4. 结果是 `E2` 的值

**注意**：用户定义的 `operator,` 在 C++17 之前不能保证这种定序关系。

#### 生命周期扩展

如果 `E2` 是临时表达式（C++17 起），逗号表达式的结果就是该临时表达式。当结果绑定到引用时，临时对象的生命周期会被扩展。

### 4.3 条件运算符类型推导

#### 隐式转换序列形成

当 E2 和 E3 类型不同时，编译器尝试形成隐式转换序列：

```
struct A {};
struct B : A {};
using T = const B;
A a = true ? A() : T(); // Y = A(), TY = A, X = T(), TX = const B, 目标类型 = const A
```

#### 复合指针类型

当操作数是指针时，结果类型由复合指针类型规则确定：

```
int* intPtr;
using Mixed = decltype(true ? nullptr : intPtr);
static_assert(std::is_same_v<Mixed, int*>); // nullptr 变为 int*
```

#### 协变返回类型

条件运算符支持协变返回类型，允许派生类覆盖函数返回指向派生类的指针或引用。

### 4.4 重载决议中的条件运算符

虽然条件运算符不能被重载，但对于每个提升的算术类型对 `L` 和 `R`，以及每个指针、指向成员指针或作用域枚举类型 `P`，以下函数签名参与重载决议：

```cpp
LR operator?:(bool, L, R);  // LR 是对 L 和 R 进行通常算术转换的结果
P operator?:(bool, P, P);
```

这些签名仅用于重载决议，实际的条件运算符不能被重载。

## 5. 使用场景 (Use Cases)

### 5.1 函数调用运算符

#### 适用场景

1. **函数对象（Functor）**：创建可调用对象，封装状态和行为
2. **Lambda 表达式**：底层通过重载 `operator()` 实现
3. **回调机制**：将对象作为回调传递
4. **策略模式**：通过不同的函数对象实现不同策略

#### 最佳实践

```cpp
// 1. 函数对象封装状态
struct Multiplier {
    int factor;
    Multiplier(int f) : factor(f) {}
    int operator()(int x) const { return x * factor; }
};

// 2. 可变参数函数调用
struct Printer {
    void operator()(const std::string& s) const {
        std::cout << s << '\n';
    }
    void operator()(int i, double d) const {
        std::cout << i << ", " << d << '\n';
    }
};

// 3. 结合标准库算法
std::vector<int> v = {1, 2, 3, 4, 5};
std::transform(v.begin(), v.end(), v.begin(), Multiplier(2));
```

#### 注意事项

- 成员函数调用默认等价于 `this->member_function()`
- 析构函数的返回类型是 `void`
- 变参函数调用时，匹配省略号的参数会进行默认参数提升

### 5.2 逗号运算符

#### 适用场景

1. **for 循环**：在更新部分执行多个操作
2. **返回语句**：返回前执行清理操作
3. **初始化表达式**：构造函数初始化列表中执行多个操作

#### 最佳实践

```cpp
// 1. for 循环中的使用
for (int i = 0, j = 10; i <= j; ++i, --j) {
    std::cout << "i = " << i << " j = " << j << '\n';
}

// 2. 返回语句中的使用（记录日志后返回）
int process() {
    return log("Processing complete"), compute_result();
}

// 3. 构造函数初始化列表
class Widget {
    int data;
public:
    Widget(const Config& cfg)
        : data(validate(cfg), cfg.value) {}  // 先验证再初始化
};
```

#### 常见陷阱

```cpp
// 错误：在函数参数中使用逗号运算符
f(a, n++, n + b);     // 实际传递三个参数，n 可能未递增
f(a, (n++, n + b));   // 正确：传递两个参数

// 错误：在下标运算符中使用逗号（C++23 起）
arr[i, j];            // 错误：被解释为 arr.operator[](i, j) 或非法
arr[(i, j)];          // 正确：逗号表达式作为下标，结果为 arr[j]

// 注意：类类型的 operator, 可以被重载
struct Bad {
    Bad operator,(int x) { return *this; }
};
Bad a, b;
a, 5, b;              // 调用重载的 operator,
```

### 5.3 条件运算符

#### 适用场景

1. **简洁的条件赋值**：根据条件选择值
2. **编译时条件**：模板元编程中的条件选择
3. **避免代码重复**：简化简单的 if-else 结构
4. **左值使用**：根据条件选择不同的对象进行修改

#### 最佳实践

```cpp
// 1. 简洁的条件赋值
int max_val = (a > b) ? a : b;
std::string status = (error_code == 0) ? "success" : "failure";

// 2. 作为左值
int x = 10, y = 20;
(x > y ? x : y) = 100;  // 将较大的变量设为 100

// 3. throw 表达式配合使用
std::string value = ptr ? *ptr : throw std::runtime_error("null pointer");

// 4. constexpr 中的条件选择（C++11）
constexpr int abs(int x) {
    return x >= 0 ? x : -x;
}
```

#### 常见陷阱

```cpp
// 错误：类型不兼容导致编译错误
auto result = true ? 3.14 : "hello";  // 错误：double 和 const char* 不能转换

// 错误：值类别问题
int a = 1, b = 2;
auto& ref = true ? a : b;  // 错误：纯右值不能绑定到非 const 引用

// 注意：避免过度嵌套
// 难以阅读
int x = a ? (b ? c : d) : (e ? f : g);

// 更清晰的写法
int x;
if (a) {
    x = b ? c : d;
} else {
    x = e ? f : g;
}

// 错误：忽略求值顺序
int i = 0;
int arr[] = {1, 2, 3};
int val = (i++ < 2) ? arr[i] : 0;  // 未定义行为：i 在条件中被修改
```

### 5.4 标准库中的应用

标准库大量重载 `operator()` 作为函数对象：

| 类别 | 类名 | 用途 |
|------|------|------|
| 算术运算 | `std::plus`, `std::minus`, `std::multiplies`, `std::divides`, `std::modulus`, `std::negate` | 算术运算 |
| 比较运算 | `std::equal_to`, `std::not_equal_to`, `std::greater`, `std::less`, `std::greater_equal`, `std::less_equal` | 比较运算 |
| 逻辑运算 | `std::logical_and`, `std::logical_or`, `std::logical_not` | 逻辑运算 |
| 位运算 | `std::bit_and`, `std::bit_or`, `std::bit_xor` | 位运算 |
| 功能包装 | `std::function`, `std::move_only_function`, `std::copyable_function` | 函数包装 |
| 智能指针 | `std::default_delete` | 删除器 |
| 随机数 | 随机数引擎和分布 | 生成随机数 |

## 6. 代码示例 (Examples)

### 6.1 函数调用运算符示例

#### 基础用法

```cpp
#include <cstdio>
#include <iostream>

// 基本函数调用
void f() {
    puts("function called");
}

// 成员函数调用
struct S {
    int f1(double d) {
        return printf("%f \n", d);  // 变参函数调用
    }

    int f2() {
        return f1(7);  // 成员函数调用，等价于 this->f1()
                       // 整数参数转换为 double
    }
};

int main() {
    f();    // 函数调用
    S s;
    s.f2(); // 成员函数调用
    return 0;
}
```

#### 高级用法：函数对象

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

// 1. 带状态的函数对象
class Counter {
    int count = 0;
public:
    int operator()() {
        return ++count;
    }
    int get() const { return count; }
};

// 2. 多态函数对象
struct Adder {
    int operator()(int a, int b) const { return a + b; }
    double operator()(double a, double b) const { return a + b; }
};

// 3. 可变参数函数对象
struct Logger {
    template<typename... Args>
    void operator()(Args&&... args) {
        ((std::cout << args << " "), ...);
        std::cout << '\n';
    }
};

int main() {
    // 使用计数器
    Counter counter;
    std::cout << counter() << '\n';  // 1
    std::cout << counter() << '\n';  // 2
    std::cout << counter() << '\n';  // 3

    // 使用加法器
    Adder adder;
    std::cout << adder(1, 2) << '\n';      // 3
    std::cout << adder(1.5, 2.5) << '\n';  // 4

    // 使用日志器
    Logger logger;
    logger("Error:", 404, "File not found");

    // 在算法中使用
    std::vector<int> v = {1, 2, 3, 4, 5};
    Counter counter2;
    std::for_each(v.begin(), v.end(), [&counter2](int x) {
        std::cout << counter2() << ": " << x << '\n';
    });

    return 0;
}
```

#### 常见错误

```cpp
#include <iostream>

struct BadExample {
    // 错误：operator() 不能是静态的
    // static void operator()() {}  // 编译错误

    // 正确：operator() 必须是成员函数
    void operator()() { std::cout << "called\n"; }

    // 注意：可以有多个重载
    void operator()(int x) { std::cout << x << '\n'; }
};

int main() {
    BadExample ex;
    ex();    // 调用无参版本
    ex(42);  // 调用有参版本

    // 错误：尝试通过对象调用 operator() 的地址
    // auto f = &BadExample::operator();  // 需要复杂的语法

    // 正确：使用 std::function 或 lambda
    auto f = [ex]() mutable { ex(); };
    f();

    return 0;
}
```

### 6.2 逗号运算符示例

#### 基础用法

```cpp
#include <iostream>

int main() {
    // 逗号运算符在 for 循环中
    for (int i = 0, j = 10; i <= j; ++i, --j) {
        //             ^列表分隔符    ^逗号运算符
        std::cout << "i = " << i << " j = " << j << '\n';
    }

    // 逗号运算符链：结果是最右边表达式的值
    int n = 1;
    int m = (++n, std::cout << "n = " << n << '\n', ++n, 2 * n);
    std::cout << "m = " << (++m, m) << '\n';

    return 0;
}
```

#### 高级用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// 日志包装器
template<typename T>
T log_and_return(const char* msg, T value) {
    std::cout << msg << ": " << value << '\n';
    return value;
}

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5};

    // 在算法中使用逗号运算符
    int sum = 0;
    std::for_each(data.begin(), data.end(), [&sum](int x) {
        sum += x;  // 这里不能用逗号运算符代替分号
    });

    // 在条件表达式中使用逗号运算符
    int result = (sum > 10)
        ? (std::cout << "large\n", sum)
        : (std::cout << "small\n", 0);

    // 错误示例：在数组下标中使用未加括号的逗号
    int arr[] = {1, 2, 3, 4, 5};
    int idx = 1, val = 2;
    // int x = arr[idx, val];  // C++23 起错误，或被解释为 arr.operator[](idx, val)
    int y = arr[(idx, val)];   // 正确：逗号表达式作为下标，等价于 arr[2]
    std::cout << "y = " << y << '\n';

    return 0;
}
```

#### 常见错误

```cpp
#include <iostream>

int compute(int x) { return x * 2; }
int compute(double x) { return x * 3; }

int main() {
    int a = 5, b = 10;

    // 错误：在函数参数中使用逗号运算符（未加括号）
    // compute(a, b);  // 错误：试图调用双参数版本（不存在）

    // 正确：加括号使用逗号运算符
    int result = compute((a, b));  // 等价于 compute(10)
    std::cout << result << '\n';   // 输出 20

    // 错误：混淆逗号运算符和列表分隔符
    int arr[5] = {1, 2, 3, 4, 5};  // 这里是列表分隔符
    // arr[1, 2];  // C++23 起错误
    arr[(1, 2)];  // 正确：访问 arr[2]

    // 注意：逗号运算符返回右值（除非 E2 是左值）
    int x = 1, y = 2;
    // (x, y) = 3;  // 错误：逗号表达式结果是 y 的值，不是左值
    y = 3;  // 直接赋值

    return 0;
}
```

### 6.3 条件运算符示例

#### 基础用法

```cpp
#include <iostream>
#include <string>

int main() {
    // 简单条件选择
    int n = 1 > 2 ? 10 : 11;  // 1 > 2 为 false，所以 n = 11
    std::cout << "n = " << n << '\n';

    // 作为左值
    int m = 10;
    (n == m ? n : m) = 7;  // n == m 为 false，所以 m = 7
    std::cout << "n = " << n << "\nm = " << m << '\n';

    // 字符串类型
    std::string str = 2 + 2 == 4 ? "OK" : throw std::logic_error("Math is broken");
    std::cout << str << '\n';

    return 0;
}
```

#### 高级用法

```cpp
#include <iostream>
#include <string>
#include <type_traits>

// 链式结构
struct Node {
    Node* next;
    int data;

    // 深拷贝构造函数
    Node(const Node& other)
        : next(other.next ? new Node(*other.next) : nullptr)
        , data(other.data)
    {}

    Node(int d) : next(nullptr), data(d) {}
    ~Node() { delete next; }
};

int main() {
    // 配合 throw 表达式
    auto check = [](int* p) -> int {
        return p ? *p : throw std::runtime_error("null pointer");
    };

    int value = 42;
    std::cout << check(&value) << '\n';  // 输出 42
    // check(nullptr);  // 抛出异常

    // 嵌套条件运算符（三目运算符链）
    int score = 85;
    char grade = (score >= 90) ? 'A' :
                 (score >= 80) ? 'B' :
                 (score >= 70) ? 'C' :
                 (score >= 60) ? 'D' : 'F';
    std::cout << "Grade: " << grade << '\n';  // B

    // 类型推导演示
    int* intPtr = nullptr;
    using Mixed = decltype(true ? nullptr : intPtr);
    static_assert(std::is_same_v<Mixed, int*>);  // nullptr 变为 int*

    // 指向成员指针
    struct A { int* m_ptr; } a;
    int* A::* memPtr = &A::m_ptr;

    static_assert(std::is_same_v<decltype(false ? memPtr : nullptr), int* A::*>);
    static_assert(std::is_same_v<decltype(false ? a.*memPtr : nullptr), int*>);

    return 0;
}
```

#### 常见错误

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() = default;
    virtual void foo() { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    void foo() override { std::cout << "Derived\n"; }
};

int main() {
    // 错误：类型不兼容
    // auto x = true ? 1 : 3.14;  // double，1 转换为 1.0
    // auto y = true ? 1 : "hello";  // 错误：int 和 const char* 不能转换

    // 注意：条件运算符的类型在编译时确定
    Base b;
    Derived d;
    Base& ref = true ? b : d;  // 结果是 Base&（静态绑定）
    ref.foo();  // 输出 "Base"（虽然 d 是 Derived，但类型已是 Base&）

    // 如果需要动态绑定，使用指针
    Base* ptr = true ? &b : static_cast<Base*>(&d);
    ptr->foo();  // 根据 true/false 输出 "Base" 或 "Derived"

    // 错误：副作用顺序问题
    int i = 0;
    int arr[] = {1, 2, 3};
    // int val = (i++ < 3) ? arr[i] : 0;  // 危险：i 在条件中递增，结果不确定

    // 正确写法
    int idx = i++;
    int val = (idx < 3) ? arr[idx] : 0;
    std::cout << val << '\n';  // 输出 1

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 运算符 | 核心特性 | 可重载 | 典型用途 |
|--------|---------|--------|---------|
| 函数调用 `()` | 使对象具有函数语义 | 是 | 函数对象、回调、策略模式 |
| 逗号 `,` | 顺序求值，返回最右值 | 是 | for 循环、表达式链 |
| 条件 `? :` | 三元选择，短路求值 | 否 | 条件赋值、编译时选择 |

### 技术对比

#### 函数调用 vs Lambda

| 特性 | 函数对象 | Lambda |
|------|---------|--------|
| 状态管理 | 显式成员变量 | 捕获列表 |
| 可读性 | 需要定义类型 | 内联定义 |
| 复用性 | 可多次实例化 | 通常一次性使用 |
| 模板支持 | 支持模板成员 | C++14 起泛型 lambda |

#### 逗号运算符 vs 分号

| 特性 | 逗号运算符 | 分号 |
|------|-----------|------|
| 表达式层面 | 在表达式内连接多个操作 | 分隔语句 |
| 返回值 | 返回最右值 | 无返回值 |
| 适用位置 | 仅表达式上下文 | 语句之间 |
| 可读性 | 可能降低可读性 | 更清晰 |

#### 条件运算符 vs if-else

| 特性 | 条件运算符 | if-else 语句 |
|------|-----------|-------------|
| 表达式 | 是表达式，有返回值 | 是语句，无返回值 |
| 简洁性 | 适合简单条件选择 | 适合复杂逻辑 |
| 类型要求 | 两分支必须有共同类型 | 无类型要求 |
| 左值使用 | 可作为左值 | 不能作为左值 |

### 学习建议

1. **函数调用运算符**：
   - 理解函数对象的核心概念
   - 掌握与标准库算法的结合使用
   - 了解 Lambda 表达式的底层实现原理

2. **逗号运算符**：
   - 主要用于 for 循环等特定场景
   - 避免过度使用，可能降低代码可读性
   - 注意与列表分隔符的区别

3. **条件运算符**：
   - 优先保证代码可读性，不要过度嵌套
   - 理解类型推导规则
   - 掌握 throw 表达式的配合使用

### 缺陷报告总结

| 缺陷编号 | 标题 | 核心修正 |
|---------|------|---------|
| CWG 462 | 逗号运算符临时对象生命周期 | 第二个操作数为临时对象时，结果即为该临时对象 |
| CWG 587 | 条件运算符 lvalue 结果 | 相同类型但不同 cv 限定时，总是返回 lvalue |
| CWG 1560 | void 操作数条件表达式 | void 操作数不导致另一操作数的 lvalue-to-rvalue 转换 |
| CWG 2283 | C++17 求值顺序 | 函数表达式在参数之前求值 |

### 参考资源

- C++ 标准库函数对象：`<functional>` 头文件
- 条件运算符类型推导：`std::common_type`（C++11 起）
- 运算符重载最佳实践：参考 C++ Core Guidelines

---

**文档版本**：基于 C++23 标准
**最后更新**：2025-02-09
**来源**：cppreference.com