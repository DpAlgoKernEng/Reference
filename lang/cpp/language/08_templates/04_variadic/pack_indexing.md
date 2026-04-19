# Pack Indexing - 包索引（C++26）

## 1. 概述 (Overview)

**Pack indexing（包索引）** 是 C++26 引入的新特性，允许通过指定索引访问参数包（parameter pack）中的元素。这是一种**包扩展（pack expansion）**机制，语法形式为 `P...[I]`，其中 `P` 是参数包，`I` 是编译期常量索引。

包索引提供了一种直观、类型安全的方式来提取参数包中的特定元素，无需递归模板展开或使用 `std::tuple` 等辅助结构。

## 2. 来源与演变 (Origin and Evolution)

### 引入背景

在 C++26 之前，访问参数包中的第 N 个元素需要复杂的模板元编程技巧，常见方法包括：

1. **递归模板展开**：通过递归特化逐步展开参数包
2. **`std::tuple` + `std::get`**：将参数包转为 tuple 后访问
3. **折叠表达式结合索引序列**：使用 `std::index_sequence` 遍历

这些方法要么代码冗长，要么引入额外的运行时开销。

### C++26 新特性

| 特性 | 描述 |
|------|------|
| Pack indexing expression | 通过 `id-expression...[I]` 访问包中的值 |
| Pack indexing specifier | 通过 `typedef-name...[I]` 访问包中的类型 |
| 编译期常量索引要求 | 索引必须是编译期常量，不能使用运行时值 |
| 特性测试宏 | `__cpp_pack_indexing` 值为 `202311L` |

### 版本兼容性说明

C++26 之前，`Ts...[N]` 是声明函数参数包（无名数组大小为 N）的有效语法，参数类型会被调整为指针。C++26 起此语法被重新解释为包索引，可能影响现有代码行为。

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

包索引有两种语法形式：

| 语法 | 形式 | 说明 |
|------|------|------|
| 包索引表达式 | `id-expression ...[ expression ]` | 访问包中的值元素 |
| 包索引说明符 | `typedef-name ...[ expression ]` | 访问包中的类型元素 |

### 参数说明

| 参数 | 说明 |
|------|------|
| `id-expression` | 标识参数包的表达式，必须通过以下声明引入：非类型模板参数包、函数参数包、lambda 初始化捕获包、结构化绑定包 |
| `typedef-name` | 命名参数包的标识符或简单模板 ID，必须通过类型模板参数包声明引入 |
| `expression` | 转换后的常量表达式，类型为 `std::size_t`，索引范围必须为 `[0, sizeof...(P))` |

### 约束条件

1. **索引必须是编译期常量**：不能使用运行时变量或函数返回值作为索引
2. **只能索引 id-expression**：不能索引复杂表达式
3. **不能索引模板模板参数包**：模板模板参数包不支持包索引

## 4. 底层原理 (Underlying Principles)

### 实例化机制

设 `P` 为包含元素 `P0, P1, ..., Pn-1` 的非空参数包，`I` 为有效索引。实例化扩展 `P...[I]` 时，产生参数包 `P` 的元素 `PI`。

```
参数包 P: P0, P1, P2, ..., Pn-1
索引 I: 2

P...[I] -> P2
```

### 编译期求值

包索引完全在编译期求值：

- 索引表达式必须是**转换后的常量表达式（converted constant expression）**
- 索引值在编译期确定，编译器直接定位到对应元素
- 无运行时开销

### decltype 行为

对包索引表达式应用 `decltype` 与直接对 id-expression 应用 `decltype` 行为相同：

```cpp
void f()
{
    [](auto... args)
    {
        using T0 = decltype(args...[0]);   // T0 是 double（值类型）
        using T1 = decltype((args...[0])); // T1 是 double&（引用类型）
    }(3.14);
}
```

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 描述 |
|------|------|
| 类型提取 | 从类型参数包中提取特定位置的类型 |
| 值提取 | 从值参数包中提取特定位置的值 |
| 泛型编程 | 简化变参模板的实现 |
| 结构化绑定 | 结合结构化绑定包使用 |

### 包索引表达式适用范围

包索引表达式中的 `id-expression` 必须由以下声明引入：

1. **非类型模板参数包（non-type template parameter pack）**
2. **函数参数包（function parameter pack）**
3. **Lambda 初始化捕获包（lambda init-capture pack）**
4. **结构化绑定包（structured binding pack）**

### 包索引说明符适用范围

包索引说明符可用作：

1. **简单类型说明符（simple type specifier）**
2. **基类说明符（base class specifier）**
3. **嵌套名说明符（nested name specifier）**
4. **显式析构函数调用的类型**

### 限制与注意事项

**限制 1：索引必须是编译期常量**

```cpp
int runtime_idx();

void bar(auto... args)
{
    auto a = args...[0];           // OK: 字面量常量
    const int n = 1;
    auto b = args...[n];           // OK: 编译期常量
    int m = 2;
    auto c = args...[m];           // 错误: 'm' 不是常量表达式
    auto d = args...[runtime_idx()]; // 错误: 运行时函数调用
}
```

**限制 2：不能索引模板模板参数包**

```cpp
template <template <typename...> typename... Temps>
using A = Temps...[0]<>;  // 错误: 'Temps' 是模板模板参数包

template <template <typename...> typename... Temps>
using B = Temps<>...[0];  // 错误: 'Temps<>' 不是参数包名
```

**限制 3：只能索引 id-expression**

```cpp
template <std::size_t I, auto... Vals>
constexpr auto identity_at = (Vals)...[I];  // 错误

// 正确写法
template <std::size_t I, auto... Vals>
constexpr auto identity_at = Vals...[I];    // OK
```

### C++26 前后行为差异

```cpp
template <typename... Ts>
void f(Ts... [1]);

void foo()
{
    f<char, bool>(nullptr, nullptr);
    // C++26 之前：调用 void f<char, bool>(char*, bool*)
    // C++26 起：错误，期望 1 个参数但提供了 2 个
}
```

如需保持旧行为，应显式命名参数包或调整为指针类型：

```cpp
template <typename... Ts>
void g(Ts... args[1]);  // OK: 命名参数包

template <typename... Ts>
void h(Ts*...);         // OK: 指针类型参数包
```

## 6. 代码示例 (Examples)

### 基础用法：值提取

```cpp
#include <cstddef>

// 从函数参数包中提取指定位置的元素
template <std::size_t I, typename... Ts>
constexpr auto element_at(Ts... args)
{
    return args...[I];
}

static_assert(element_at<0>(3, 5, 9) == 3);
static_assert(element_at<2>(3, 5, 9) == 9);
// static_assert(element_at<3>(3, 5, 9) == 4);  // 错误: 越界
// static_assert(element_at<0>() == 1);        // 错误: 空包越界
```

### 基础用法：类型提取

```cpp
#include <type_traits>

// 从类型参数包中提取指定位置的类型
template <typename... Ts>
using last_type_t = Ts...[sizeof...(Ts) - 1];

static_assert(std::is_same_v<last_type_t<int>, int>);
static_assert(std::is_same_v<last_type_t<bool, char>, char>);
static_assert(std::is_same_v<last_type_t<float, int, bool*>, bool*>);
// static_assert(std::is_same_v<last_type_t<>, int>);  // 错误: 空包越界
```

### 高级用法：结构化绑定包索引

```cpp
#include <tuple>

template <std::size_t I, typename Tup>
constexpr auto structured_binding_element_at(Tup tup)
{
    auto [...elems] = tup;  // 结构化绑定包
    return elems...[I];
}

struct Point
{
    int x;
    int y;
    int z;
};

static_assert(structured_binding_element_at<0>(Point{true, 4}) == true);
static_assert(structured_binding_element_at<1>(Point{true, 4}) == 4);
```

### 高级用法：非类型模板参数包索引

```cpp
#include <cstddef>

// 非类型模板参数包索引
template <std::size_t I, std::size_t... Vals>
constexpr std::size_t double_at = Vals...[I] * 2;

static_assert(double_at<0, 1, 2, 3> == 2);   // 1 * 2
static_assert(double_at<1, 1, 2, 3> == 4);   // 2 * 2
static_assert(double_at<2, 1, 2, 3> == 6);   // 3 * 2
```

### 高级用法：Lambda 初始化捕获包索引

```cpp
#include <cstddef>
#include <string>

template <std::size_t I, typename... Args>
constexpr auto foo(Args... args)
{
    return [...members = args](auto op)
    {
        return members...[I] + op;  // Lambda 初始化捕获包索引
    };
}

static_assert(foo<0>(4, "Hello", true)(5) == 9);
// foo<1>(3, std::string("C++"))("26") 返回 "C++26"
```

### 高级用法：包索引作为函数参数类型

```cpp
template <typename...>
struct type_seq {};

template <typename... Ts>
auto f(Ts...[0] arg, type_seq<Ts...>)
{
    return arg;
}

// Ts...[0] 作为非推导上下文
// "Hello" 隐式转换为 std::string_view
// std::same_as<std::string_view> auto a = f("Hello", type_seq<std::string_view>{});
```

### 高级用法：组合使用（splice 函数）

```cpp
#include <tuple>

template <std::size_t... Indices, typename Decomposable>
constexpr auto splice(Decomposable d)
{
    auto [...elems] = d;
    return std::make_tuple(elems...[Indices]...);  // 多索引提取
}

struct Point
{
    int x;
    int y;
    int z;
};

int main()
{
    constexpr Point p{.x = 1, .y = 4, .z = 3};
    static_assert(splice<2, 1, 0>(p) == std::make_tuple(3, 4, 1));
    static_assert(splice<1, 1, 0, 0>(p) == std::make_tuple(4, 4, 1, 1));
}
```

### 常见错误及修正

#### 错误 1：运行时索引

```cpp
// 错误：使用运行时变量作为索引
void bad(auto... args)
{
    int idx = 0;
    auto x = args...[idx];  // 错误: idx 不是常量表达式
}

// 修正：使用编译期常量
template <std::size_t I, typename... Args>
constexpr auto good(Args... args)
{
    return args...[I];  // OK: I 是模板参数，编译期常量
}
```

#### 错误 2：索引复杂表达式

```cpp
template <std::size_t I, typename... Args>
constexpr decltype(auto) get_bad(Args&&... args)
{
    return std::forward<Args>(args)...[I];  // 错误: 不能索引 std::forward<Args>(args)
}

// 修正：分开处理
template <std::size_t I, typename... Args>
constexpr decltype(auto) get_good(Args&&... args)
{
    return std::forward<Args...[I]>(args...[I]);  // OK
}
```

#### 错误 3：越界访问

```cpp
template <typename... Ts>
using first_t = Ts...[0];

// first_t<> first;  // 错误: 空包无法索引

// 修正：添加静态断言
template <typename... Ts>
requires (sizeof...(Ts) > 0)
using first_safe_t = Ts...[0];
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 语法 | `P...[I]`，其中 P 是参数包，I 是编译期常量索引 |
| 两种形式 | 包索引表达式（值）、包索引说明符（类型） |
| 编译期求值 | 索引必须在编译期确定，无运行时开销 |
| C++26 新增 | 简化参数包元素访问，替代复杂的模板元编程 |

### 技术对比

| 方法 | 复杂度 | 运行时开销 | 代码简洁性 |
|------|--------|-----------|-----------|
| 递归模板 | 高 | 无 | 冗长 |
| `std::tuple` + `std::get` | 中 | 可能存在 | 中等 |
| 包索引（C++26） | 低 | 无 | 简洁 |

### 学习建议

1. **理解编译期特性**：包索引完全在编译期求值，索引必须是常量表达式
2. **区分两种形式**：表达式形式访问值，说明符形式访问类型
3. **注意兼容性**：C++26 前后 `Ts...[N]` 语法含义不同，迁移代码时需注意
4. **使用特性测试宏**：通过 `__cpp_pack_indexing` 检测编译器支持

### 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/pack_indexing
- Feature-test macro: `__cpp_pack_indexing` = `202311L` (C++26)