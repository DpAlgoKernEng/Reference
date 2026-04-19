# auto - 占位类型说明符

## 1. 概述 (Overview)

`auto` 是 C++11 引入的**占位类型说明符**（placeholder type specifier），用于声明变量时让编译器自动推导其类型。它指定一个**占位类型**（placeholder type），该类型将在后续由初始化器推导确定。

`auto` 的核心价值在于：
- **简化代码**：避免书写冗长的类型名称
- **类型安全**：编译器自动推导，避免人为类型错误
- **泛型编程**：支持泛型 lambda 和函数模板的简洁表达

`auto` 定义在 C++ 标准的类型说明符章节中，属于语言核心特性。

### 两种占位类型形式

| 形式 | 引入版本 | 说明 |
|------|---------|------|
| `auto` | C++11 | 使用模板参数推导规则推导类型 |
| `decltype(auto)` | C++14 | 类型为 `decltype(expr)`，其中 expr 是初始化器或返回语句 |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++11 之前，`auto` 关键字具有完全不同的语义——它是**存储持续时间说明符**（storage duration specifier），用于声明自动存储期的变量。由于所有局部变量默认就是自动存储期，这个用法几乎无人使用。

C++11 重新定义了 `auto` 的语义，将其转变为类型推导的关键字，解决了以下问题：
- 容器迭代器类型名称冗长
- 模板代码中类型名称复杂
- 泛型编程中类型表达困难

### 版本演变

#### C++11：正式引入

- `auto` 作为占位类型说明符
- 用于变量声明时的类型推导
- 支持函数返回类型的尾置返回语法

#### C++14：增强功能

- 引入 `decltype(auto)` 语法
- 支持函数返回类型自动推导
- 支持 lambda 参数使用 `auto`（泛型 lambda）

#### C++17：扩展应用

- 支持结构化绑定声明
- 支持非类型模板参数使用 `auto`

#### C++20：约束支持

- 支持概念约束（concept constraint）
- 支持简写函数模板语法

#### C++23：新增用法

- 支持 `auto` 作为函数式转换的类型说明符

### 特性测试宏

| 宏 | 值 | 标准 | 特性 |
|----|-----|------|------|
| `__cpp_decltype_auto` | `201304L` | C++14 | `decltype(auto)` |

### 缺陷报告

| DR | 应用版本 | 已发布行为 | 修正行为 |
|----|---------|-----------|---------|
| CWG 1265 | C++11 | auto 可同时声明函数和变量 | 禁止 |
| CWG 1346 | C++11 | 括号表达式列表不能赋给 auto 变量 | 允许 |
| CWG 1347 | C++11 | auto 可声明两个不同类型的变量 | 禁止 |
| CWG 1852 | C++14 | decltype(auto) 中的 auto 也被视为占位符 | 不再视为占位符 |
| CWG 1892 | C++11 | 函数指针的返回类型可以是 auto | 禁止 |
| CWG 2476 | C++11 | CWG 1892 的修正禁止了函数指针返回类型推导 | 允许从初始化器推导 |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
// 语法形式 (1)：auto 占位类型
type-constraint(可选) auto

// 语法形式 (2)：decltype(auto) 占位类型 (C++14 起)
type-constraint(可选) decltype(auto)
```

### type-constraint（C++20 起）

`type-constraint` 是一个可选的概念名称，可以带限定符和模板参数列表：

```cpp
std::same_as<int> auto x = 42;  // C++20：约束 auto 推导为 int
Sortable auto& container = get_container();  // 约束类型必须满足 Sortable 概念
```

约束表达式的构成：
- 如果 `type-constraint` 是 `Concept<A1, ..., An>`，约束表达式为 `Concept<T, A1, ..., An>`
- 如果 `type-constraint` 是 `Concept`（无参数列表），约束表达式为 `Concept<T>`

推导失败条件：约束表达式无效或返回 `false`

### 修饰符支持

`auto` 可以与修饰符组合使用：

```cpp
const auto x = 10;           // const int
auto& ref = x;               // int&
const auto& cref = x;        // const int&
auto* ptr = &x;               // int*
const auto* cptr = &x;       // const int*
```

注意：`decltype(auto)` 必须是声明类型的唯一组成部分，不能添加修饰符。

### 可出现的位置

| 位置 | 引入版本 | 说明 |
|------|---------|------|
| 变量声明 | C++11 | 从初始化器推导类型 |
| 函数返回类型（尾置） | C++11 | `auto f() -> int` |
| 函数返回类型（推导） | C++14 | `auto f() { return 0; }` |
| Lambda 参数 | C++14 | 泛型 lambda |
| 非类型模板参数 | C++17 | `template<auto N>` |
| 结构化绑定 | C++17 | `auto [a, b] = pair;` |
| new 表达式 | C++11 | `auto p = new auto(5);` |
| 函数式转换 | C++23 | `auto(42)` |
| 函数参数 | C++20 | 简写函数模板 |
| 非类型模板参数 | C++17 | `template<auto N>` |

### 参数声明上下文

#### Lambda 参数 (C++14)

当 lambda 表达式的参数使用 `auto` 时，该 lambda 成为**泛型 lambda**（generic lambda）：

```cpp
auto print = [](const auto& value) {
    std::cout << value << std::endl;
};
print(42);    // 接受 int
print(3.14);  // 接受 double
print("hello"); // 接受 const char*
```

#### 非类型模板参数 (C++17)

非类型模板参数可以使用 `auto`，类型从对应的模板参数推导：

```cpp
template<auto N>
void func() {
    std::cout << N << std::endl;
}

func<42>();   // N 为 int
func<'a'>();  // N 为 char
```

#### 函数参数 (C++20)

函数声明中的参数使用 `auto` 时，声明为**简写函数模板**：

```cpp
// 简写函数模板
void process(auto x) {
    std::cout << x << std::endl;
}

// 等价于
template<typename T>
void process(T x) {
    std::cout << x << std::endl;
}
```

### 推导规则

#### auto 的推导

使用与**模板参数推导**相同的规则：

```cpp
auto x = 5;           // int（非 const、非引用）
auto& r = x;          // int&
const auto& cr = x;   // const int&
auto* p = &x;         // int*
```

#### decltype(auto) 的推导

类型为 `decltype(expr)`，保留值类别：

```cpp
decltype(auto) c1 = a;   // decltype(a) -> int
decltype(auto) c2 = (a);  // decltype((a)) -> int&（注意括号的影响）
```

### 初始化器要求

使用 `auto` 声明变量时，必须提供非空初始化器：

```cpp
auto x = 10;    // 正确
auto y;         // 错误：缺少初始化器
auto z{};       // 正确（C++11：initializer_list；C++17 起：值初始化）
```

### 花括号初始化列表

```cpp
auto a = {1, 2, 3};   // std::initializer_list<int>
auto b = {5};         // std::initializer_list<int>
auto c{5};            // C++17 起：int（C++11/14 为 initializer_list）
auto d{1, 2};         // C++17 起：错误（DR n3922）
```

## 4. 底层原理 (Underlying Principles)

### 类型推导机制

`auto` 的类型推导与模板参数推导采用**相同的规则**：

1. **去除顶层 const/volatile**：除非声明引用或指针
2. **数组退化为指针**：除非声明引用
3. **函数退化为函数指针**

```cpp
const int ci = 10;
auto x = ci;        // int（顶层 const 被丢弃）
const auto y = ci;  // const int（显式添加 const）
auto& r = ci;       // const int&（保留底层 const）

int arr[5];
auto a = arr;       // int*（数组退化为指针）
auto& ar = arr;     // int(&)[5]（引用保留数组类型）
```

### 推导过程

```
初始化器表达式
    ↓
模板参数推导规则
    ↓
确定 auto 替换的类型 T
    ↓
应用修饰符（const、&、*）
    ↓
得到最终变量类型
```

### decltype(auto) 的特殊性

`decltype(auto)` 使用 `decltype` 的语义，保留表达式的值类别：

| 表达式 | decltype(expr) | 说明 |
|--------|----------------|------|
| `x`（标识符） | T | 变量的声明类型 |
| `(x)` | T& | 左值引用（如果 x 是左值） |
| `x + y` | T | 右值（纯右值） |
| `std::move(x)` | T&& | 右值引用 |
| `func()` | 根据返回类型 | 可能是 T、T& 或 T&& |

### 编译器实现

编译器处理 `auto` 的大致流程：

1. **词法分析**：识别 `auto` 关键字
2. **语法分析**：解析声明符和初始化器
3. **语义分析**：
   - 分析初始化器表达式
   - 应用类型推导规则
   - 检查约束（C++20）
4. **代码生成**：用推导出的具体类型替换 `auto`

### 时间/空间复杂度

- **编译时开销**：类型推导发生在编译期，增加编译时间
- **运行时开销**：零开销，推导的类型与手写类型完全相同
- **代码大小**：无额外开销

## 5. 使用场景 (Use Cases)

### 适合使用 auto 的场景

| 场景 | 示例 | 优势 |
|------|------|------|
| 迭代器声明 | `auto it = vec.begin();` | 避免冗长的类型名 |
| 范围 for 循环 | `for (auto& x : vec)` | 简洁易读 |
| 泛型编程 | `auto result = func(args);` | 类型自动适配 |
| Lambda 表达式 | `auto f = [](int x) { return x; };` | 无法手写 lambda 类型 |
| 智能指针 | `auto ptr = std::make_unique<T>();` | 避免类型重复 |
| 复杂模板类型 | `auto m = std::map<std::string, std::vector<int>>{};` | 简化复杂类型 |

### 不适合使用 auto 的场景

| 场景 | 问题 | 建议 |
|------|------|------|
| 需要明确类型时 | 可读性下降 | 显式写出类型 |
| 可能产生意外转换 | 类型推导不符合预期 | 显式指定类型 |
| 代码审查时 | 类型不直观 | 适度使用 |
| 代理类型（如 vector<bool>） | 可能产生悬垂引用 | 避免使用 auto& |

### 最佳实践

#### 1. 使用 auto 简化迭代器

```cpp
// 不推荐
std::map<std::string, std::vector<int>>::iterator it = myMap.begin();

// 推荐
auto it = myMap.begin();
```

#### 2. 使用 auto 配合智能指针

```cpp
// 推荐
auto ptr = std::make_unique<MyClass>();
auto sptr = std::make_shared<MyClass>();
```

#### 3. 使用 decltype(auto) 完美转发返回值

```cpp
// 完美转发函数返回值，保留值类别
template<class F, class... Args>
decltype(auto) PerfectForward(F fun, Args&&... args) {
    return fun(std::forward<Args>(args)...);
}
```

#### 4. 使用 auto& 避免不必要的拷贝

```cpp
for (auto& item : container) {
    // 修改 item
}

for (const auto& item : container) {
    // 只读访问
}
```

### 常见陷阱

#### 陷阱 1：auto 丢弃顶层 const

```cpp
const int ci = 10;
auto x = ci;  // x 是 int，不是 const int
x = 20;       // 可以修改

// 修正
const auto y = ci;  // y 是 const int
```

#### 陷阱 2：auto 与花括号初始化

```cpp
auto x{1};        // C++17 起：int
auto y = {1};     // std::initializer_list<int>
auto z{1, 2};     // C++17 起：错误！
auto w = {1, 2};  // std::initializer_list<int>
```

#### 陷阱 3：vector<bool> 的代理类型

```cpp
std::vector<bool> v = {true, false, true};
auto& x = v[0];  // 错误！vector<bool>::reference 是代理类型，不能绑定到非 const 引用
const auto& y = v[0];  // 正确
auto z = v[0];  // 正确，得到 bool 值
```

#### 陷阱 4：decltype(auto) 与括号

```cpp
int a = 10;
decltype(auto) b = a;   // int
decltype(auto) c = (a); // int&（注意！修改 c 会修改 a）
```

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <memory>

int main() {
    // 基本类型推导
    auto i = 42;           // int
    auto d = 3.14;         // double
    auto s = "hello";      // const char*
    auto str = std::string("hello");  // std::string

    // 带修饰符
    const auto ci = 100;   // const int
    auto& ref = i;         // int&
    const auto& cref = i;  // const int&
    auto* ptr = &i;        // int*

    // 迭代器
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 范围 for 循环
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 智能指针
    auto uptr = std::make_unique<int>(42);
    auto sptr = std::make_shared<double>(3.14);

    return 0;
}
```

### 高级用法

```cpp
#include <iostream>
#include <utility>
#include <type_traits>

// C++14：自动推导返回类型
auto add(int a, int b) {
    return a + b;  // 返回类型推导为 int
}

// 泛型 lambda（C++14）
auto lambda = [](auto x, auto y) {
    return x + y;
};

// C++20：概念约束的 auto
// 需要 #include <concepts>
// std::integral auto twice(std::integral auto x) {
//     return x * 2;
// }

// C++17：非类型模板参数 auto
template<auto N>
struct Counter {
    static constexpr auto value = N;
};

// 完美转发返回值
template<class F, class... Args>
decltype(auto) PerfectForward(F fun, Args&&... args) {
    return fun(std::forward<Args>(args)...);
}

int main() {
    // 函数返回类型推导
    auto result = add(1, 2);  // int
    std::cout << "add(1, 2) = " << result << std::endl;

    // 泛型 lambda
    std::cout << "lambda(1, 2.5) = " << lambda(1, 2.5) << std::endl;

    // C++17：结构化绑定
    auto pair = std::make_pair(1, 3.14);
    auto [first, second] = pair;
    std::cout << "pair: " << first << ", " << second << std::endl;

    // decltype(auto) 示例
    int a = 10;
    decltype(auto) b = a;   // int
    decltype(auto) c = (a); // int&（注意括号）

    std::cout << "before: a = " << a << std::endl;
    c = 20;  // 修改 a
    std::cout << "after: a = " << a << std::endl;

    // 类型检查
    static_assert(std::is_same_v<decltype(b), int>);
    static_assert(std::is_same_v<decltype(c), int&>);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：声明多个变量类型不一致

```cpp
// 错误：推导出不同类型
auto a = 1, b = 2.0;  // 错误：a 是 int，b 是 double

// 修正：分别声明
auto a = 1;
auto b = 2.0;
```

#### 错误 2：缺少初始化器

```cpp
// 错误：auto 变量必须初始化
auto x;  // 错误

// 修正：提供初始化器
auto x = 0;
```

#### 错误 3：使用 auto 声明函数和变量

```cpp
// 错误：不能同时声明函数和变量
auto f() -> int, i = 0;  // 错误

// 修正：分开声明
auto f() -> int;
auto i = 0;
```

#### 错误 4：花括号初始化的误解

```cpp
// C++17 起
auto a{1};        // int（正确）
auto b{1, 2};     // 错误！直接列表初始化不允许多个元素

// 修正：使用等号形式
auto c = {1, 2};  // std::initializer_list<int>
```

#### 错误 5：代理类型的悬垂引用

```cpp
// 错误：vector<bool> 的 operator[] 返回代理类型
std::vector<bool> vb = {true, false};
auto& x = vb[0];  // 错误！绑定了临时对象

// 修正方案 1：使用值拷贝
auto y = vb[0];  // 正确：拷贝 bool 值

// 修正方案 2：使用 const auto&
const auto& z = vb[0];  // 正确：const 引用可以绑定临时对象
```

### C++20 简写函数模板示例

```cpp
#include <iostream>
#include <concepts>
#include <vector>

// C++20：简写函数模板
void print(const auto& value) {
    std::cout << value << std::endl;
}

// 带概念约束的简写函数模板
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

Numeric auto add(Numeric auto a, Numeric auto b) {
    return a + b;
}

// 处理容器
void print_all(const auto& container) {
    for (const auto& elem : container) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    print(42);
    print(3.14);
    print("hello");

    auto result = add(1, 2.5);  // double
    std::cout << "add(1, 2.5) = " << result << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5};
    print_all(vec);

    return 0;
}
```

### new 表达式与 auto

```cpp
#include <iostream>
#include <memory>

int main() {
    // new auto：类型从初始化器推导
    auto p1 = new auto(42);        // int*
    auto p2 = new auto(3.14);      // double*
    auto p3 = new auto('a');       // char*

    std::cout << *p1 << ", " << *p2 << ", " << *p3 << std::endl;

    // 与智能指针配合
    auto up = std::make_unique<int>(42);
    auto sp = std::make_shared<double>(3.14);

    // 清理
    delete p1;
    delete p2;
    delete p3;

    return 0;
}
```

#### 错误 6：new 表达式中 auto 使用不当

```cpp
// 错误：new auto 缺少初始化器
auto p1 = new auto;  // 错误！需要初始化器

// 正确：new auto 必须有初始化器
auto p2 = new auto(42);  // int*，指向值为 42 的 int

// 多个 auto 不一致
auto p3 = new auto(1), p4 = new auto(2.0);  // 错误！类型不一致

// 正确：分开声明
auto p5 = new auto(1);
auto p6 = new auto(2.0);
```

#### 错误 7：函数式转换的误用 (C++23)

```cpp
// C++23 起，auto 可用于函数式转换
auto x = auto(42);  // int，值为 42

// 错误：decltype(auto) 不能用于函数式转换
// auto y = decltype(auto)(42);  // 错误！

// 注意：函数式转换创建的是纯右值
int a = 10;
auto b = auto(a);  // int，创建副本，非引用
b = 20;  // 不影响 a
```

## 7. 总结 (Summary)

### 核心要点

`auto` 是 C++11 引入的类型推导关键字，核心特性包括：

- **自动类型推导**：编译器根据初始化器推导变量类型
- **与模板参数推导规则一致**：使用相同的推导机制
- **零运行时开销**：所有推导发生在编译期
- **支持多种修饰符**：可与 `const`、`&`、`*` 组合使用

### auto vs decltype(auto) 对比

| 特性 | auto | decltype(auto) |
|------|------|----------------|
| 引入版本 | C++11 | C++14 |
| 推导规则 | 模板参数推导 | decltype 语义 |
| 值类别 | 丢弃引用和 cv 限定符 | 保留值类别 |
| 适用场景 | 通用类型推导 | 完美转发返回值 |
| 可添加修饰符 | 是 | 否 |

### 使用建议

1. **优先使用 auto 简化代码**：特别是迭代器、智能指针等复杂类型
2. **注意 const 正确性**：使用 `const auto&` 避免拷贝和修改
3. **理解推导规则**：了解何时丢弃顶层 const
4. **谨慎使用花括号初始化**：理解 `auto x{...}` 与 `auto x = {...}` 的区别
5. **完美转发使用 decltype(auto)**：保留返回值的值类别

### 相关概念

| 概念 | 关系 |
|------|------|
| `decltype` | 类型查询操作符，提供更精确的类型推导 |
| 模板参数推导 | auto 使用相同的推导规则 |
| 概念约束 (Concepts) | C++20 起可约束 auto 推导的类型 |
| 结构化绑定 | C++17 起 auto 可用于解构 |

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.2.9.6 Placeholder type specifiers [dcl.spec.auto]
- C++20 标准 (ISO/IEC 14882:2020): 9.2.8.5 Placeholder type specifiers [dcl.spec.auto]
- C++17 标准 (ISO/IEC 14882:2017): 10.1.7.4 The `auto` specifier [dcl.spec.auto]
- cppreference: https://en.cppreference.com/w/cpp/language/auto