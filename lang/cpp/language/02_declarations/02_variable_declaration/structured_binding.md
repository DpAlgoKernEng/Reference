# 结构化绑定声明 (Structured Binding Declaration)

## 1. 概述

**结构化绑定声明**（Structured Binding Declaration）是 C++17 引入的一项特性，用于将指定的名称绑定到初始化器的子对象或元素上。

与引用类似，结构化绑定是现有对象的别名。但与引用不同的是，结构化绑定不一定是引用类型。这一特性使得从复合类型（如数组、元组、结构体）中提取元素变得更加简洁和直观。

结构化绑定声明在 `<tuple>` 等头文件中有相关支持，其核心语法位于 C++ 语言核心层面。

## 2. 来源与演变

### 首次引入

结构化绑定声明首次在 **C++17** 标准中引入，特性测试宏为 `__cpp_structured_bindings`，初始值为 `201606L`。

### 历史背景

在结构化绑定出现之前，C++ 开发者需要通过以下方式解包复合类型：

1. **使用 std::tie**：需要预先声明变量
2. **手动访问成员**：通过 `.first`、`.second` 或成员名访问
3. **结构化返回值处理繁琐**：需要临时变量或多步操作

结构化绑定的出现解决了这些问题，提供了：
- 简洁的语法，一行代码完成解包
- 自动类型推导
- 与 `auto` 关键字配合使用

### C++17 核心特性

- 引入结构化绑定基本语法
- 支持三种绑定方式：数组、元组式、数据成员

### C++20 变化

- 结构化绑定可被 lambda 表达式捕获
- 禁止使用约束（constraint）修饰结构化绑定
- `volatile` 说明符在结构化绑定中弃用

### C++26 变化

- 支持在结构化绑定中使用属性（attribute）
- 支持引入结构化绑定包（structured binding pack）
- 允许结构化绑定大小为 0（仅限空包情况）
- 标识符后可跟属性说明符序列

### 特性测试宏

| 宏 | 值 | 标准 | 特性 |
|---|---|---|---|
| `__cpp_structured_bindings` | `201606L` | C++17 | 结构化绑定 |
| `__cpp_structured_bindings` | `202403L` | C++26 | 结构化绑定支持属性 |
| `__cpp_structured_bindings` | `202411L` | C++26 | 结构化绑定可引入包 |

## 3. 语法与参数

### 基本语法

```cpp
attr(可选) decl-specifier-seq ref-qualifier(可选) [ sb-identifier-list ] initializer ;
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr` | 可选的属性序列（C++26 起支持） |
| `decl-specifier-seq` | 说明符序列，可包含：`constexpr`、`constinit`（C++26）、`static`、`thread_local`、`const`、`volatile`（C++20 起弃用）、`auto` |
| `ref-qualifier` | 引用限定符，可为 `&` 或 `&&` |
| `sb-identifier-list` | 逗号分隔的标识符列表，每个标识符后可跟属性说明符序列（C++26） |
| `initializer` | 初始化器 |

### 初始化器形式

| 形式 | 语法 | 说明 |
|------|------|------|
| 形式 1 | `= expression` | 拷贝初始化 |
| 形式 2 | `{ expression }` | 直接初始化（列表） |
| 形式 3 | `( expression )` | 直接初始化（圆括号） |

`expression` 不能是无括号的逗号表达式。

### 标识符列表规则

- C++26 起，列表中的一个标识符可以以省略号 `...` 开头，引入**结构化绑定包**（structured binding pack）
- 该标识符必须声明模板实体

### 结构化绑定大小规则

**C++26 之前**：标识符数量必须等于 `E` 的结构化绑定大小。

**C++26 起**：
设标识符数量为 N，结构化绑定大小为 S：
- 若无结构化绑定包：N 必须等于 S
- 若有结构化绑定包：非包元素数量（N - 1）必须小于等于 S，包元素数量为 `S - N + 1`（可为零）

```cpp
struct C { int x, y, z; };

template<class T>
void demo()
{
    auto [a, b, c] = C();          // OK: a, b, c 分别引用 x, y, z
    auto [d, ...e] = C();          // OK: d 引用 x; ...e 引用 y 和 z
    auto [...f, g] = C();          // OK: ...f 引用 x 和 y; g 引用 z
    auto [h, i, j, ...k] = C();    // OK: 包 k 为空
    auto [l, m, n, o, ...p] = C(); // 错误: 结构化绑定大小不足
}
```

## 4. 底层原理

### 绑定过程

结构化绑定声明首先引入一个唯一命名的变量（此处记为 `e`）来保存初始化器的值：

1. **数组情况（无引用限定符）**：若 `expression` 具有数组类型 `cv1 A` 且无引用限定符，定义 `e` 为 `attr(可选) specifiers A e;`，其中 `specifiers` 是 `decl-specifier-seq` 中除 `auto` 外的说明符序列。然后根据初始化器形式初始化元素：
   - 形式 (1)：拷贝初始化
   - 形式 (2, 3)：直接初始化

2. **其他情况**：定义 `e` 为 `attr(可选) decl-specifier-seq ref-qualifier(可选) e initializer;`

记 `E` 为 `std::remove_reference_t<decltype((e))>` 的类型。

**结构化绑定大小**是指结构化绑定声明需要引入的结构化绑定数量。

### 三种绑定情况

结构化绑定根据 `E` 的类型执行以下三种绑定之一：

#### 情况 1：绑定数组

当 `E` 是数组类型时，名称绑定到数组元素。

- 结构化绑定大小等于数组元素数量
- 每个结构化绑定成为引用对应数组元素的左值
- **引用类型**为数组元素类型（若数组类型 `E` 有 cv 限定符，元素类型也继承该限定符）

```cpp
int a[2] = {1, 2};

auto [x, y] = a;    // 创建 e[2]，将 a 复制到 e
                    // x 引用 e[0]，y 引用 e[1]
auto& [xr, yr] = a; // xr 引用 a[0]，yr 引用 a[1]
```

#### 情况 2：绑定元组式类型

当 `E` 是非联合类类型，且 `std::tuple_size<E>` 是完整类型并含有 `value` 成员时（无论该成员的类型或可访问性如何），使用元组式绑定协议。

**要求**：
- `std::tuple_size<E>::value` 必须是良构的整型常量表达式
- 结构化绑定大小等于 `std::tuple_size<E>::value`

**初始化过程**：
对于每个结构化绑定，引入一个类型为"到 `std::tuple_element<I, E>::type` 的引用"的变量：
- 若对应的初始化器是左值，则为左值引用
- 否则为右值引用

第 I 个变量的初始化器为：
- `e.get<I>()`：如果在 `E` 的作用域中通过类成员访问查找找到一个函数模板，且其第一个模板参数是非类型参数
- 否则 `get<I>(e)`：仅通过参数依赖查找（ADL），忽略非 ADL 查找

**注意**：在这些初始化表达式中，`e` 是左值（若实体 `e` 的类型是左值引用，即引用限定符为 `&` 或为 `&&` 但初始化表达式是左值），否则是亡值。这实际上执行了一种完美转发。

**引用类型**：第 I 个结构化绑定的引用类型为 `std::tuple_element<I, E>::type`。

```cpp
float x{};
char  y{};
int   z{};

std::tuple<float&, char&&, int> tpl(x, std::move(y), z);
const auto& [a, b, c] = tpl;
// 使用 Tpl = const std::tuple<float&, char&&, int>;
// a 是引用 x 的结构化绑定（从 get<0>(tpl) 初始化）
// decltype(a) 为 std::tuple_element<0, Tpl>::type，即 float&
// b 是引用 y 的结构化绑定（从 get<1>(tpl) 初始化）
// decltype(b) 为 std::tuple_element<1, Tpl>::type，即 char&&
// c 是引用 tpl 第三个元素的结构化绑定
// decltype(c) 为 std::tuple_element<2, Tpl>::type，即 const int
```

#### 情况 3：绑定数据成员

当 `E` 是非联合类类型，且 `std::tuple_size<E>` 不是完整类型时，名称绑定到可访问的数据成员。

**条件**：
- `E` 的每个非静态数据成员必须是 `E` 的直接成员或 `E` 同一基类的成员
- 在结构化绑定的上下文中以 `e.name` 形式命名时必须良构
- `E` 不能有匿名联合成员
- 结构化绑定大小等于非静态数据成员数量

每个结构化绑定成为引用 `e` 的下一个成员（按声明顺序）的左值；该左值的类型为 `e.mI` 的类型，其中 `mI` 指向第 I 个成员。支持位域。

**引用类型**：第 I 个结构化绑定的引用类型为 `e.mI` 的类型（若不是引用类型），或 `mI` 的声明类型（若是引用类型）。

```cpp
#include <iostream>

struct S
{
    mutable int x1 : 2;
    volatile double y1;
};

S f() { return S{1, 2.3}; }

int main()
{
    const auto [x, y] = f(); // x 是标识 2 位位域的 int 左值
                             // y 是 const volatile double 左值
    std::cout << x << ' ' << y << '\n';  // 1 2.3
    x = -2;   // OK: x 是 mutable
//  y = -2.;  // 错误: y 是 const 限定的
    std::cout << x << ' ' << y << '\n';  // -2 2.3
}
```

### 初始化顺序

设 `valI` 为第 I 个结构化绑定所命名的对象或引用：

1. `e` 的初始化先序于任何 `valI` 的初始化
2. `valI` 的初始化先序于任何 `valJ`（I < J）的初始化

### decltype 行为

对于结构化绑定 `x`，`decltype(x)` 返回该结构化绑定的**引用类型**。

在元组式情况下，这是 `std::tuple_element` 返回的类型，可能不是引用类型，尽管实际上总是引入一个隐藏的引用。这有效地模拟了绑定到一个结构体的行为，该结构体的非静态数据成员具有 `std::tuple_element` 返回的类型，绑定本身的引用性只是实现细节。

```cpp
std::tuple<int, int&> f();

auto [x, y] = f();       // decltype(x) 为 int
                         // decltype(y) 为 int&

const auto [z, w] = f(); // decltype(z) 为 const int
                         // decltype(w) 为 int&
```

### get 成员查找规则

对成员 `get` 的查找：
- 忽略可访问性
- 忽略非类型模板参数的确切类型

即使一个私有的 `template<char*> void get();` 成员会导致成员解释被使用，即使它是非良构的。

## 5. 使用场景

### 适合使用结构化绑定的场景

| 场景 | 示例 |
|------|------|
| 解包 `std::pair` 或 `std::tuple` | `auto [first, second] = my_pair;` |
| 遍历 map | `for (auto& [key, value] : my_map) { ... }` |
| 解包结构体 | `auto [x, y, z] = my_point;` |
| 处理函数返回的复合值 | `auto [iter, success] = container.insert(value);` |
| 解包数组 | `auto [a, b, c] = my_array;` |

### 最佳实践

1. **优先使用 `const auto&`**：避免不必要的拷贝
2. **使用 `auto&&` 转发**：在泛型代码中正确转发
3. **注意生命周期**：避免悬垂引用
4. **理解隐藏变量**：声明前的说明符应用于隐藏变量 `e`

```cpp
// 好的实践：遍历 map
std::map<std::string, int> scores;
for (const auto& [name, score] : scores) {
    std::cout << name << ": " << score << "\n";
}

// 好的实践：处理插入结果
if (auto [iter, success] = myset.insert("hello"); success) {
    std::cout << "插入成功\n";
}
```

### 注意事项

#### 声明前的说明符应用于隐藏变量

`[` 之前的部分应用于隐藏变量 `e`，而非结构化绑定：

```cpp
int a = 1, b = 2;
const auto& [x, y] = std::tie(a, b); // x 和 y 的类型是 int&
auto [z, w] = std::tie(a, b);        // z 和 w 的类型仍然是 int&
assert(&z == &a);                    // 通过
```

#### 元组式解释优先

如果 `std::tuple_size<E>` 是完整类型并含有 `value` 成员，即使程序可能因此变得非良构，也会使用元组式解释：

```cpp
struct A { int x; };

namespace std
{
    template<>
    struct tuple_size<::A> { void value(); };
}

auto [x] = A{}; // 错误：不考虑"数据成员"解释
```

#### 临时对象的生命周期延长

如果存在引用限定符且表达式是纯右值，临时对象的生命周期会被延长：

```cpp
int a = 1;

const auto& [x] = std::make_tuple(a); // OK，不悬垂
auto&       [y] = std::make_tuple(a); // 错误：不能将 auto& 绑定到右值
auto&&      [z] = std::make_tuple(a); // OK
```

#### Lambda 捕获限制

**C++17**：结构化绑定不能被 lambda 捕获。

**C++20 起**：结构化绑定可以被 lambda 捕获。

```cpp
#include <cassert>

int main()
{
    struct S { int p{6}, q{7}; };
    const auto& [b, d] = S{};
    auto l = [b, d] { return b * d; }; // C++20 起有效
    assert(l() == 42);
}
```

#### 约束限制

结构化绑定不能被约束：

```cpp
template<class T>
concept C = true;

C auto [x, y] = std::pair{1, 2}; // 错误：被约束
```

#### C++26 空包支持

C++26 起，结构化绑定大小可以为 0，只要 `sb-identifier-list` 恰好包含一个只能引入空结构化绑定包的标识符：

```cpp
auto return_empty() -> std::tuple<>;

template <class>
void test_empty()
{
    auto [] = return_empty();           // 错误
    auto [...args] = return_empty();    // OK, args 是空包
    auto [one, ...rest] = return_empty(); // 错误，结构化绑定大小不足
}
```

### 相关工具

| 工具 | 说明 |
|------|------|
| `std::tie` (C++11) | 创建左值引用元组或将元组解包为独立对象 |

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <set>
#include <string>
#include <tuple>

int main()
{
    // 示例 1：解包 std::pair
    std::pair<int, std::string> p = {42, "hello"};
    auto [num, str] = p;
    std::cout << num << ": " << str << "\n";  // 42: hello

    // 示例 2：解包数组
    int arr[] = {1, 2, 3};
    auto [a, b, c] = arr;
    std::cout << a << " " << b << " " << c << "\n";  // 1 2 3

    // 示例 3：解包结构体
    struct Point { double x, y; };
    Point pt = {3.0, 4.0};
    auto [x, y] = pt;
    std::cout << "Point(" << x << ", " << y << ")\n";  // Point(3, 4)

    return 0;
}
```

### 高级用法

```cpp
#include <iomanip>
#include <iostream>
#include <set>
#include <string>

int main()
{
    // 示例：遍历集合并处理插入结果
    std::set<std::string> myset{"hello"};

    for (int i{2}; i; --i)
    {
        if (auto [iter, success] = myset.insert("Hello"); success)
            std::cout << "插入成功。值为 "
                      << std::quoted(*iter) << "。\n";
        else
            std::cout << "值 " << std::quoted(*iter)
                      << " 已存在于集合中。\n";
    }

    // 示例：结构化绑定与位域
    struct BitFields
    {
        // C++20: 位域的默认成员初始化器
        int b : 4 {1}, d : 4 {2}, p : 4 {3}, q : 4 {4};
    };

    {
        const auto [b, d, p, q] = BitFields{};
        std::cout << b << ' ' << d << ' ' << p << ' ' << q << '\n';
        // 输出: 1 2 3 4
    }

    {
        const auto [b, d, p, q] = []{ return BitFields{4, 3, 2, 1}; }();
        std::cout << b << ' ' << d << ' ' << p << ' ' << q << '\n';
        // 输出: 4 3 2 1
    }

    {
        BitFields s;

        auto& [b, d, p, q] = s;
        std::cout << b << ' ' << d << ' ' << p << ' ' << q << '\n';
        // 输出: 1 2 3 4

        b = 4, d = 3, p = 2, q = 1;
        std::cout << s.b << ' ' << s.d << ' ' << s.p << ' ' << s.q << '\n';
        // 输出: 4 3 2 1
    }

    return 0;
}
```

输出：
```
插入成功。值为 "Hello"。
值 "Hello" 已存在于集合中。
1 2 3 4
4 3 2 1
1 2 3 4
4 3 2 1
```

### 常见错误及修正

#### 错误 1：绑定数量不匹配

```cpp
// 错误：标识符数量与结构化绑定大小不匹配
int arr[3] = {1, 2, 3};
auto [a, b] = arr;     // 错误：需要 3 个标识符

// 修正：使用正确数量的标识符
auto [a, b, c] = arr;  // OK
```

#### 错误 2：绑定私有成员

```cpp
class Private {
    int x, y;  // 私有成员
public:
    Private(int a, int b) : x(a), y(b) {}
};

// 错误：无法绑定私有成员
auto [x, y] = Private(1, 2);  // 错误

// 修正：提供公共访问器或使用元组式绑定
```

#### 错误 3：悬垂引用

```cpp
// 返回局部变量的引用
auto&& [x, y] = []{ return std::make_pair(1, 2); }();
// 临时对象生命周期延长，安全

auto& [a, b] = []{ return std::make_pair(1, 2); }();
// 错误：不能将非 const 左值引用绑定到右值

// 修正：使用 const 引用或右值引用
const auto& [x, y] = []{ return std::make_pair(1, 2); }();  // OK
auto&& [a, b] = []{ return std::make_pair(1, 2); }();        // OK
```

#### 错误 4：C++17 中 Lambda 捕获

```cpp
struct S { int x, y; };
auto [a, b] = S{1, 2};

// C++17 错误：不能捕获结构化绑定
// auto lambda = [a, b] { return a + b; };  // 错误

// 修正 C++17：先复制到普通变量
int x = a, y = b;
auto lambda = [x, y] { return x + y; };  // OK

// C++20 起：可以直接捕获
auto lambda = [a, b] { return a + b; };  // C++20 OK
```

## 7. 总结

结构化绑定声明是 C++17 引入的强大特性，简化了复合类型元素的访问。

### 核心要点

| 特性 | 说明 |
|------|------|
| 引入版本 | C++17 |
| 主要用途 | 解包数组、元组、结构体 |
| 类型推导 | 配合 `auto` 使用 |
| 三种绑定方式 | 数组、元组式、数据成员 |
| C++26 增强 | 属性支持、结构化绑定包 |

### 技术对比

| 特性 | 结构化绑定 | std::tie |
|------|-----------|----------|
| 语法简洁性 | 更简洁 | 需要预先声明变量 |
| 类型推导 | 自动 | 需显式类型 |
| 可读性 | 高 | 中等 |
| 适用范围 | 数组/元组/结构体 | 元组 |

### 缺陷报告

| 缺陷编号 | 应用版本 | 原始行为 | 修正行为 |
|---------|---------|---------|---------|
| CWG 2285 | C++17 | `expression` 可引用标识符列表中的名称 | 此情况下声明非良构 |
| CWG 2312 | C++17 | 情况 3 中 `mutable` 的含义丢失 | 保留其含义 |
| CWG 2313 | C++17 | 情况 2 中结构化绑定变量可被重声明 | 不能重声明 |
| CWG 2339 | C++17 | 情况 2 中缺少 `I` 的定义 | 添加定义 |
| CWG 2341 (P1091R3) | C++17 | 结构化绑定不能有静态存储期 | 允许 |
| CWG 2386 | C++17 | 只要 `std::tuple_size<E>` 是完整类型就使用元组式协议 | 仅当有 `value` 成员时使用 |
| CWG 2506 | C++17 | 若 `expression` 是 cv 限定数组类型，cv 限定传递给 `E` | 丢弃该 cv 限定 |
| CWG 2635 | C++20 | 结构化绑定可被约束 | 禁止 |
| CWG 2867 | C++17 | 初始化顺序不明确 | 明确化 |
| P0961R1 | C++17 | 情况 2 中找到任何 `get` 就使用成员 | 仅当找到具有非类型参数的函数模板时 |
| P0969R0 | C++17 | 情况 3 要求成员为 public | 仅要求在声明上下文中可访问 |

### 学习建议

1. **优先在循环中使用**：遍历 `std::map` 时使用 `auto& [key, value]` 提高可读性
2. **注意生命周期**：理解隐藏变量 `e` 的作用
3. **理解三种绑定方式**：正确判断使用哪种绑定协议
4. **关注 C++26 新特性**：结构化绑定包和属性支持
5. **理解 decltype 行为**：结构化绑定的 `decltype` 返回引用类型

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.6 Structured binding declarations [dcl.struct.bind]
- C++20 标准 (ISO/IEC 14882:2020): 9.6 Structured binding declarations [dcl.struct.bind]
- C++17 标准 (ISO/IEC 14882:2017): 11.5 Structured binding declarations [dcl.struct.bind]
- cppreference: https://en.cppreference.com/w/cpp/language/structured_binding