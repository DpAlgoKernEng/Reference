# 隐式转换 (Implicit Conversions)

## 1. 概述 (Overview)

**隐式转换 (Implicit Conversion)** 是 C++ 语言自动执行的类型转换机制。当某种类型 `T1` 的表达式用于不接受该类型但接受其他类型 `T2` 的上下文中时，编译器会自动执行隐式转换。

### 核心概念

隐式转换在以下场景中自动触发：

- 当表达式用作函数调用的参数，而该函数声明的参数类型为 `T2`
- 当表达式用作期望类型 `T2` 的操作符的操作数
- 当初始化类型为 `T2` 的新对象时，包括返回类型为 `T2` 的函数中的 `return` 语句
- 当表达式用于 switch 语句（`T2` 为整型）
- 当表达式用于 if 语句或循环（`T2` 为 bool）

### 关键要求

程序良构（可编译）的必要条件是：从 `T1` 到 `T2` 存在唯一的**隐式转换序列 (Implicit Conversion Sequence)**。

如果被调用的函数或操作符存在多个重载版本，在构建从 `T1` 到每个可用 `T2` 的隐式转换序列后，由重载决议规则决定编译哪个重载版本。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

隐式转换的设计源于 C 语言，C++ 继承并扩展了这一机制：

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++11 | 引入 `nullptr` 和 `std::nullptr_t`，新增上下文转换规则；引入 `explicit` 转换函数解决 Safe Bool 问题 |
| C++14 | 改进上下文转换规则，允许多个转换函数转换为同一类型时的歧义处理 |
| C++17 | 引入临时量实质化转换；新增函数指针转换（noexcept 函数指针转换） |
| C++20 | 扩展上下文转换到 `explicit` 说明符；放宽数组边界相似性要求 |
| C++23 | 浮点转换规则更加严格，要求目标类型具有相等或更高的转换等级 |

### Safe Bool 问题

在 C++11 之前，设计可在布尔上下文中使用的类存在一个问题：给定用户定义的转换函数（如 `T::operator bool() const`），隐式转换序列允许在该函数调用后再执行一个标准转换序列，这意味着结果 `bool` 可以转换为 `int`，从而允许 `obj << 1;` 或 `int i = obj;` 这样的代码。

早期解决方案包括：
- `std::basic_ios` 最初定义 `operator void*`，使得 `if (std::cin) {...}` 可编译，但 `int n = std::cout;` 不能编译
- 许多第三方库采用更复杂的 Safe Bool 惯用法

C++11 引入 `explicit bool` 转换函数解决了这个问题。

## 3. 语法与参数 (Syntax and Parameters)

### 转换顺序

隐式转换序列按以下顺序组成：

1. 零个或一个**标准转换序列 (Standard Conversion Sequence)**
2. 零个或一个**用户定义转换 (User-Defined Conversion)**
3. 零个或一个**标准转换序列**（仅当使用用户定义转换时）

### 标准转换序列

标准转换序列按以下顺序组成：

#### 第一阶段：值类别转换

零个或一个以下转换：
- **左值到右值转换 (Lvalue-to-rvalue Conversion)**
- **数组到指针转换 (Array-to-pointer Conversion)**
- **函数到指针转换 (Function-to-pointer Conversion)**

#### 第二阶段：数值提升或转换

零个或一个**数值提升 (Numeric Promotion)** 或**数值转换 (Numeric Conversion)**

#### 第三阶段：函数指针转换（C++17 起）

零个或一个**函数指针转换 (Function Pointer Conversion)**

#### 第四阶段：限定转换

零个或一个**限定转换 (Qualification Conversion)**

### 用户定义转换

用户定义转换由零个或一个非显式单参数转换构造函数或非显式转换函数调用组成。

### 上下文转换

#### 上下文布尔转换

在以下上下文中，期望 `bool` 类型，如果声明 `bool t(e);` 良构则执行隐式转换（即考虑 `explicit T::operator bool() const;` 这样的显式转换函数）：

- `if`、`while`、`for` 的控制表达式
- 内置逻辑操作符 `!`、`&&` 和 `||` 的操作数
- 条件操作符 `?:` 的第一个操作数
- `static_assert` 声明中的谓词
- `noexcept` 说明符中的表达式
- `explicit` 说明符中的表达式（C++20 起）

#### 上下文隐式转换

在以下上下文中，期望特定类型 `T`，类类型 `E` 的表达式 `e` 需满足特定条件：

- delete 表达式的参数（`T` 为任意对象指针类型）
- 整型常量表达式，使用字面类时（`T` 为任意整型或无作用域枚举类型，选定的用户定义转换函数必须为 `constexpr`）
- `switch` 语句的控制表达式（`T` 为任意整型或枚举类型）

## 4. 底层原理 (Underlying Principles)

### 值转换 (Value Transformations)

值转换改变表达式的值类别：

#### 左值到右值转换

左值（C++11 前）/ 泛左值（C++11 起）可以转换为右值（C++11 前）/ 纯右值（C++11 起）：

- 如果 `T` 不是类类型，转换结果的类型为 `T` 的无 cv 限定版本
- 否则，转换结果的类型为 `T`

转换结果：
- 如果 `T` 是（可能有 cv 限定的）`std::nullptr_t`，结果为空指针常量
- 如果 `T` 是类类型，转换从泛左值复制初始化结果对象
- 如果对象包含无效指针值，行为由实现定义
- 如果值表示中的位对对象类型无效，行为未定义
- 否则，读取对象，结果为包含的值

**注意**：此转换模拟从内存位置读取值到 CPU 寄存器的操作。

#### 数组到指针转换

"N 个 `T` 的数组"或"T 的未知边界数组"类型的左值或右值可以隐式转换为"指向 `T` 的指针"类型的纯右值。结果指针指向数组的第一个元素。

#### 函数到指针转换

函数类型的左值可以隐式转换为指向该函数的指针纯右值。这不适用于非静态成员函数，因为不存在引用非静态成员函数的左值。

#### 临时量实质化转换（C++17 起）

任何完整类型 `T` 的纯右值可以转换为同一类型 `T` 的亡值。此转换通过以临时对象为结果对象求值纯右值来初始化类型 `T` 的临时对象，并产生指代该临时对象的亡值。

发生场景：
- 将引用绑定到纯右值
- 访问类纯右值的非静态数据成员
- 对类纯右值调用隐式对象成员函数
- 对数组纯右值执行数组到指针转换或下标操作
- 从花括号初始化列表初始化 `std::initializer_list<T>` 类型的对象
- 纯右值作为弃值表达式出现

### 整型提升 (Integral Promotion)

小整型（如 `char`）和无作用域枚举类型的纯右值可转换为较大整型（如 `int`）的纯右值。此转换始终保留值。

#### 从整型提升

- `bool` 类型可转换为 `int`，`false` 变为 `0`，`true` 变为 `1`
- 对于其他整型 `T`：
  - 位域值：如果 `int` 能表示位域的所有值，则提升为 `int`；否则提升为 `unsigned int`
  - 如果 `T` 的整型转换等级低于 `int` 的等级：
    - 如果 `int` 能表示 `T` 的所有值，提升为 `int`
    - 否则提升为 `unsigned int`
  - 对于 `char8_t`（C++20 起）、`char16_t`、`char32_t`、`wchar_t`（C++11 起），提升为能表示其底层类型所有值的以下类型之一：`int`、`unsigned int`、`long`、`unsigned long`、`long long`（C++11 起）、`unsigned long long`（C++11 起）

#### 从枚举类型提升

- 底层类型未固定的无作用域枚举类型可转换为能容纳其整个值范围的以下类型之一：`int`、`unsigned int`、`long`、`unsigned long`、`long long`（C++11 起）、`unsigned long long`（C++11 起）
- 底层类型固定的无作用域枚举类型可转换为其底层类型；如果底层类型也需要提升，则提升为提升后的底层类型

### 浮点提升 (Floating-point Promotion)

`float` 类型的纯右值可转换为 `double` 类型的纯右值，值不变。

### 数值转换 (Numeric Conversions)

与提升不同，数值转换可能改变值，可能导致精度损失。

#### 整型转换

整型或无作用域枚举类型的纯右值可转换为任意其他整型：

- 目标类型为无符号：结果是源值模 2^n 的最小无符号值（n 为目标类型的位数）
- 目标类型为有符号：
  - 源整数能在目标类型中表示：值不变
  - 否则：C++20 前为实现定义；C++20 起为源值模 2^n 的唯一值
- 源类型为 `bool`：`false` 转换为零，`true` 转换为一
- 目标类型为 `bool`：见布尔转换

#### 浮点转换

- C++23 前：浮点类型的纯右值可转换为任意其他浮点类型的纯右值
- C++23 起：浮点类型的纯右值可转换为相等或更高浮点转换等级的任意其他浮点类型的纯右值；标准浮点类型的纯右值可转换为任意其他标准浮点类型的纯右值

转换规则：
- 源值能在目标类型中精确表示：值不变
- 源值在目标类型的两个可表示值之间：结果为实现定义的两者之一（若支持 IEEE 算术，默认舍入到最近）
- 否则：行为未定义

#### 浮点-整型转换

**浮点到整型**：截断小数部分
- 截断值不能适应目标类型：行为未定义（即使目标类型是无符号的）
- 目标类型为 `bool`：见布尔转换

**整型到浮点**：尽可能精确
- 值能适应但不能精确表示：实现定义选择最接近的较高或较低可表示值
- 值不能适应：行为未定义
- 源类型为 `bool`：`false` 转换为零，`true` 转换为一

#### 指针转换

- **空指针转换**：空指针常量可转换为任意指针类型，结果为该类型的空指针值
- **void 指针转换**：指向任意（可有 cv 限定）对象类型 `T` 的指针可转换为指向（相同 cv 限定）`void` 的指针
- **派生到基类转换**：指向（可有 cv 限定）`Derived` 的指针可转换为指向（相同 cv 限定）`Base` 的指针

#### 成员指针转换

- **空成员指针转换**：空指针常量可转换为任意成员指针类型
- **基类到派生类转换**：`Base` 类的成员指针可转换为 `Derived` 类的成员指针

#### 布尔转换

整型、浮点型、无作用域枚举、指针和成员指针类型的纯右值可转换为 `bool` 类型：

- 零值（整型、浮点型、无作用域枚举）、空指针、空成员指针变为 `false`
- 所有其他值变为 `true`

### 限定转换 (Qualification Conversions)

#### 相似类型

非正式地，如果忽略顶层 cv 限定符后两个类型满足以下条件之一，则它们是**相似的 (Similar)**：

- 它们是相同类型
- 它们都是指针，且指向的类型相似
- 它们都是同一类的成员指针，且指向的成员类型相似
- 它们都是数组，且数组元素类型相似

#### 限定分解

类型 `T` 的**限定分解 (Qualification-Decomposition)** 是组件序列 `cv_i` 和 `P_i`，使得 `T` 为 "cv_0 P_0 cv_1 P_1 ... cv_{n-1} P_{n-1} cv_n U"，其中：
- 每个 `cv_i` 是 const 和 volatile 的集合
- 每个 `P_i` 是"指向"、"类 `C_i` 的类型为...的成员指针"、"N 个...的数组"或"未知边界...的数组"

#### 转换规则

如果满足以下条件，类型 `T1` 的纯右值可转换为类型 `T2`：

- `T1` 和 `T2` 相似
- 对于每个非零 i，如果 const 在 `cv1_i` 中，则 const 也在 `cv2_i` 中（volatile 同理）
- 对于每个非零 i，如果 `cv1_i` 和 `cv2_i` 不同，则 const 在 `cv2_k` 中对于所有 k 在 [1, i) 中

### 函数指针转换（C++17 起）

- 指向非抛出函数的指针可转换为指向可能抛出函数的指针
- 指向非抛出成员函数的指针可转换为指向可能抛出成员函数的指针

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 函数参数传递

```cpp
void func(double d);

int main() {
    int i = 42;
    func(i);  // int 隐式转换为 double
}
```

#### 2. 初始化

```cpp
double d = 42;      // int 隐式转换为 double
bool b = 0;         // int 隐式转换为 bool
```

#### 3. 算术运算

```cpp
int a = 5;
double b = 2.5;
auto c = a + b;     // int 隐式转换为 double，结果为 double
```

#### 4. 条件判断

```cpp
int* ptr = nullptr;
if (ptr) {          // 指针隐式转换为 bool
    // ...
}
```

### 最佳实践

#### 推荐做法

1. **优先使用显式转换**：当意图不明确时，使用 `static_cast` 等显式转换
2. **避免意外的隐式转换**：使用 `explicit` 关键字防止构造函数的隐式转换
3. **注意精度损失**：浮点到整型的转换会截断小数部分

### 常见陷阱

#### 1. 符号问题

```cpp
int i = -1;
unsigned int u = i;  // 意外的隐式转换，u 变为 UINT_MAX
```

#### 2. 精度损失

```cpp
double d = 3.14159265358979;
float f = d;         // 精度损失
```

#### 3. 指针与 bool 转换

```cpp
int* p = nullptr;
bool b = p;          // p 隐式转换为 false
```

#### 4. 多级指针的 const 转换

```cpp
char** p = 0;
const char** pcc = p;  // 错误！不能直接转换
const char* const * p2 = p;  // 正确
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：整型提升

```cpp
#include <iostream>
#include <type_traits>

void print_type_info() {
    char c = 'A';
    short s = 100;

    // char 和 short 在算术运算中提升为 int
    auto result = c + s;

    std::cout << "Type of c + s: "
              << (std::is_same_v<decltype(result), int> ? "int" : "other")
              << std::endl;
}

int main() {
    print_type_info();
    return 0;
}
```

#### 示例 2：布尔转换

```cpp
#include <iostream>

void test_bool_conversion() {
    int i = 42;
    int* p = nullptr;
    double d = 0.0;

    std::cout << "int 42 to bool: " << static_cast<bool>(i) << std::endl;  // true
    std::cout << "nullptr to bool: " << static_cast<bool>(p) << std::endl; // false
    std::cout << "0.0 to bool: " << static_cast<bool>(d) << std::endl;     // false
}

int main() {
    test_bool_conversion();
    return 0;
}
```

#### 示例 3：指针转换

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {};

void test_pointer_conversion() {
    Derived d;
    Base* base_ptr = &d;        // 派生类指针转换为基类指针

    int arr[5] = {1, 2, 3, 4, 5};
    int* ptr = arr;             // 数组到指针转换

    void* void_ptr = ptr;       // 任意对象指针转换为 void*

    std::cout << "Conversion successful" << std::endl;
}

int main() {
    test_pointer_conversion();
    return 0;
}
```

### 高级用法

#### 示例 4：用户定义转换

```cpp
#include <cassert>

template<typename T>
class zero_init {
    T val;
public:
    zero_init() : val(static_cast<T>(0)) {}
    zero_init(T val) : val(val) {}
    operator T&() { return val; }
    operator T() const { return val; }
};

int main() {
    zero_init<int> i;
    assert(i == 0);

    i = 7;
    assert(i == 7);

    // C++14 起：两个转换函数都转换为同一类型 int
    switch (i) {}     // C++14 前错误（多个转换函数），C++14 起正确
    switch (i + 0) {} // 始终正确（隐式转换）

    return 0;
}
```

#### 示例 5：上下文转换

```cpp
#include <iostream>

struct Testable {
    explicit operator bool() const { return true; }
};

int main() {
    Testable t;

    // 上下文转换到 bool，考虑 explicit 转换函数
    if (t) {
        std::cout << "t is true in boolean context" << std::endl;
    }

    // 直接初始化，不使用隐式转换
    bool b = static_cast<bool>(t);

    return 0;
}
```

#### 示例 6：限定转换

```cpp
#include <iostream>

void test_qualification_conversion() {
    int x = 10;
    int* p = &x;
    const int* cp = p;           // 添加 const：OK

    // 多级指针的限定转换
    char** ppc = nullptr;
    char* const* pcpc = ppc;     // OK：添加 const 到第二级
    const char* const* pcpc2 = pcpc; // OK：添加 const 到第一级

    // 错误示例（取消注释会导致编译错误）
    // const char** pcc = ppc;   // 错误！不能直接转换
    // char** ppcc = pcpc2;      // 错误！不能移除 const

    std::cout << "Qualification conversions work correctly" << std::endl;
}

int main() {
    test_qualification_conversion();
    return 0;
}
```

#### 示例 7：函数指针转换（C++17）

```cpp
#include <iostream>

void normal_func() noexcept {
    std::cout << "noexcept function" << std::endl;
}

void (*p)() = normal_func;  // C++17: noexcept 函数指针转换为普通函数指针

void test_function_pointer_conversion() {
    p();
}

int main() {
    test_function_pointer_conversion();
    return 0;
}
```

### 常见错误及修正

#### 错误 1：意外的隐式转换

```cpp
// 错误：类接受任意类型参数
class BadString {
public:
    BadString(int size) {}  // 隐式转换允许
};

void process_string(const BadString& s) {}

int main() {
    process_string(100);  // 意外的隐式转换：100 转换为 BadString
    return 0;
}

// 修正：使用 explicit 关键字
class GoodString {
public:
    explicit GoodString(int size) {}  // 阻止隐式转换
};

void process_string(const GoodString& s) {}

int main() {
    // process_string(100);  // 编译错误
    process_string(GoodString(100));  // 必须显式构造
    return 0;
}
```

#### 错误 2：精度损失

```cpp
#include <iostream>

int main() {
    double d = 3.141592653589793;
    int i = d;  // 隐式转换，精度损失：i = 3

    // 修正：显式转换并添加警告
    int j = static_cast<int>(d);  // 显式，意图清晰

    std::cout << "d = " << d << ", i = " << i << ", j = " << j << std::endl;
    return 0;
}
```

#### 错误 3：有符号/无符号转换

```cpp
#include <iostream>
#include <vector>

int main() {
    // 错误：有符号/无符号比较
    int size = -1;
    std::vector<int> vec = {1, 2, 3};

    // if (vec.size() > size) { ... }  // 危险！size 转换为无符号大数

    // 修正：使用显式转换
    if (static_cast<int>(vec.size()) > size) {
        std::cout << "Safe comparison" << std::endl;
    }

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 要点 | 说明 |
|------|------|
| 转换顺序 | 标准转换序列 → 用户定义转换 → 标准转换序列 |
| 值转换 | 左值到右值、数组到指针、函数到指针、临时量实质化 |
| 整型提升 | 小整型提升到 int/unsigned int，保留值 |
| 浮点提升 | float 提升到 double，保留值 |
| 数值转换 | 可能损失精度或改变值 |
| 限定转换 | 可添加 const/volatile 限定符 |

### 技术对比

| 特性 | 提升 (Promotion) | 转换 (Conversion) |
|------|-----------------|------------------|
| 值保留 | 是 | 可能丢失 |
| 重载决议 | 优先选择 | 次级选择 |
| 适用类型 | 小整型、float | 所有数值类型 |
| 精度影响 | 无 | 可能存在 |

### 版本特性

| C++ 版本 | 新特性 |
|----------|--------|
| C++11 | `nullptr`、上下文转换、explicit 转换函数 |
| C++14 | 改进上下文转换歧义处理 |
| C++17 | 临时量实质化、函数指针转换 |
| C++20 | 扩展上下文转换、放宽数组边界相似性 |
| C++23 | 更严格的浮点转换规则 |

### 学习建议

1. **理解转换优先级**：提升优于转换，窄化转换需谨慎
2. **善用 explicit**：防止意外的隐式转换，提高类型安全
3. **注意有符号/无符号转换**：避免意外的数值溢出
4. **使用 static_cast**：显式表达转换意图，提高代码可读性
5. **理解 const 正确性**：掌握限定转换规则，正确处理多级指针

### 相关主题

- `const_cast`：移除 const/volatile 限定符
- `static_cast`：显式类型转换
- `dynamic_cast`：多态类型的安全下行转换
- `reinterpret_cast`：低级重新解释转换
- 显式转型 (Explicit Cast)：C 风格转换
- 用户定义转换 (User-Defined Conversion)：通过构造函数和转换运算符

---

*文档生成日期：2026-03-15*
*来源：cppreference - Implicit conversions*