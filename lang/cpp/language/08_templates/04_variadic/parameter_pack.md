# Parameter Pack - 参数包 (C++11)

## 1. 概述

**参数包 (Parameter Pack)** 是 C++11 引入的核心语言特性，是变参模板 (Variadic Template) 的基础构建块。参数包是一种能够接受零个或多个参数的 C++ 实体，使得模板可以处理任意数量和类型的参数。

参数包定义了以下几类实体：

| 类型 | 引入版本 | 说明 |
|------|---------|------|
| 模板参数包 (Template Parameter Pack) | C++11 | 接受零个或多个模板参数 |
| 函数参数包 (Function Parameter Pack) | C++11 | 接受零个或多个函数参数 |
| Lambda 初始化捕获包 (Lambda Init-capture Pack) | C++20 | 为包展开中的每个元素引入初始化捕获 |
| 结构化绑定包 (Structured Binding Pack) | C++26 | 引入零个或多个结构化绑定 |

至少包含一个参数包的模板称为**变参模板 (Variadic Template)**。

## 2. 来源与演变

### 历史背景

在 C++11 之前，C++ 处理可变数量参数的方式非常有限：

1. **C 风格变参函数**：使用 `<cstdarg>` 中的 `va_list`、`va_start`、`va_arg`、`va_end` 宏
   - 类型不安全
   - 只能用于函数参数，不能用于模板
   - 运行时确定参数数量

2. **预处理器宏**：使用 `...` 和 `__VA_ARGS__`
   - 无法进行类型检查
   - 调试困难

参数包的设计目标是：
- 提供类型安全的变参机制
- 支持编译时参数处理
- 与模板系统无缝集成

### 版本演进

| 版本 | 特性 |
|------|------|
| **C++11** | 首次引入参数包和变参模板，特性测试宏 `__cpp_variadic_templates = 200704L` |
| **C++14** | 支持变参模板的泛型 lambda |
| **C++17** | 折叠表达式 (Fold Expression) 简化参数包处理；using 声明中的包展开 |
| **C++20** | Lambda 初始化捕获包；约束类型模板参数包 |
| **C++26** | 结构化绑定包；包索引 (Pack Indexing)，特性测试宏 `__cpp_pack_indexing = 202311L` |

### 缺陷报告

| DR | 应用版本 | 原行为 | 修正行为 |
|----|---------|--------|---------|
| CWG 1533 | C++11 | 包展开可以出现在成员初始化器的成员名中 | 不允许 |
| CWG 2717 | C++11 | 对齐说明符的实例化使用逗号分隔 | 使用空格分隔 |
| CWG 1488 | C++11 | `Ts (&...)[N]` 不允许，因为语法要求括号中的省略号必须有名称 | 语法限制 |

## 3. 语法与参数

### 模板参数包语法

模板参数包可以出现在别名模板、类模板、变量模板 (C++14)、概念 (C++20) 和函数模板参数列表中。

```cpp
// (1) 非类型模板参数包
type ... pack-name(optional)

// (2) 类型模板参数包
typename|class ... pack-name(optional)

// (3) 约束类型模板参数包 (C++20)
type-constraint ... pack-name(optional)

// (4) 模板模板参数包
template <parameter-list> typename|class ... pack-name(optional)
```

### 函数参数包语法

```cpp
// (5) 函数参数包
pack-name ... pack-param-name(optional)
```

### 包展开语法

```cpp
// (6) 包展开
pattern ...
```

包展开将模式 (pattern) 扩展为零个或多个模式的实例化，用逗号分隔（对齐说明符除外，使用空格分隔）。

### 参数说明

| 参数 | 说明 |
|------|------|
| `pack-name` | 参数包的名称（可选） |
| `pack-param-name` | 函数参数包中参数的名称（可选） |
| `pattern` | 包含至少一个参数包名称的模式 |

### 参数包大小确定规则

参数包中元素的数量等于：

1. **模板/函数参数包**：为该参数包提供的参数数量
2. **Lambda 初始化捕获包** (C++20)：其初始化器包展开中的元素数量
3. **结构化绑定包** (C++26)：初始化器的结构化绑定大小减去声明中非包元素的数量

## 4. 底层原理

### 包展开机制

包展开是参数包的核心机制。当一个模式后跟省略号 `...`，且该模式包含至少一个参数包名称时，编译器会将其展开为一系列实例化：

```cpp
template<class... Us>
void f(Us... pargs) {}

template<class... Ts>
void g(Ts... args)
{
    f(&args...); // "&args..." 是包展开
                 // "&args" 是其模式
}

g(1, 0.2, "a"); // Ts... args 展开为 int E1, double E2, const char* E3
                // &args... 展开为 &E1, &E2, &E3
                // Us... pargs 展开为 int* E1, double* E2, const char** E3
```

### 多包同时展开

如果同一模式中出现两个参数包的名称，它们会同时展开，且必须具有相同的长度：

```cpp
template<typename...>
struct Tuple {};

template<typename T1, typename T2>
struct Pair {};

template<class... Args1>
struct zip
{
    template<class... Args2>
    struct with
    {
        typedef Tuple<Pair<Args1, Args2>...> type;
        // Pair<Args1, Args2>... 是包展开
        // Pair<Args1, Args2> 是模式
    };
};

typedef zip<short, int>::with<unsigned short, unsigned>::type T1;
// Pair<Args1, Args2>... 展开为
// Pair<short, unsigned short>, Pair<int, unsigned int>
```

### 嵌套包展开

当包展开嵌套在另一个包展开中时，最内层的包展开先展开其参数包，外层包展开必须引用不同的参数包：

```cpp
template<class... Args>
void g(Args... args)
{
    f(h(args...) + args...); // 嵌套包展开：
    // 内层包展开是 "args..."，先展开
    // 外层包展开是 h(E1, E2, E3) + args...，后展开
    // 结果为 h(E1, E2, E3) + E1, h(E1, E2, E3) + E2, h(E1, E2, E3) + E3
}
```

### 空包处理

当参数包的元素数量为零时，包展开的实例化产生一个空列表，不改变外围结构的语法解释：

```cpp
template<class... Bases>
struct X : Bases... { };  // 空包时：struct X { };

template<class... Args>
void f(Args... args)
{
    X<Args...> x(args...);  // 空包时：X<> x;
}

template void f<>();  // OK：X<> 没有基类，x 是值初始化的 X<> 类型变量
```

### 编译期计算

参数包的所有展开都在编译期完成，运行时没有任何开销。`sizeof...` 运算符用于获取参数包大小：

```cpp
template<class... Types>
struct count
{
    static const std::size_t value = sizeof...(Types);
};
```

## 5. 使用场景

### 包展开位置

参数包可以在以下上下文中展开：

| 展开位置 | 展开结果类型 | 分隔符 |
|---------|-------------|-------|
| 函数参数列表 | 函数参数 | 逗号 |
| 圆括号初始化器 | 函数参数 | 逗号 |
| 花括号初始化列表 | 初始化元素 | 逗号 |
| 模板参数列表 | 模板参数 | 逗号 |
| 函数参数声明 | 函数参数声明 | 逗号 |
| 模板参数声明 | 模板参数声明 | 逗号 |
| 基类说明符列表 | 基类说明符 | 逗号 |
| 成员初始化列表 | 成员初始化器 | 逗号 |
| Lambda 捕获列表 | 捕获项 | 逗号 |
| 对齐说明符列表 | 对齐说明符 | **空格** |
| 属性列表 | 属性 | 逗号 |
| Using 声明列表 (C++17) | 声明 | 逗号 |

### 最佳实践

#### 1. 模板参数包位置

在主类模板中，模板参数包必须是模板参数列表的最后一个参数。但在函数模板中，如果后续参数可以从函数参数推导或具有默认参数，参数包可以出现在前面：

```cpp
// ✅ 正确：类模板参数包在末尾
template<typename U, typename... Ts>
struct valid;

// ❌ 错误：类模板参数包不在末尾
// template<typename... Ts, typename U>
// struct Invalid;

// ✅ 正确：函数模板，U 可以从参数推导
template<typename... Ts, typename U, typename=void>
void valid(U, Ts...);

valid(1.0, 1, 2, 3);  // U 推导为 double，Ts 为 {int, int, int}
```

#### 2. 递归展开 vs 折叠表达式

C++17 之前，处理参数包通常需要递归：

```cpp
// C++11/14 风格：递归展开
template<typename T>
void print(T value) {
    std::cout << value << std::endl;
}

template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first << ", ";
    print(rest...);  // 递归调用
}
```

C++17 引入折叠表达式，简化了常见模式：

```cpp
// C++17 风格：折叠表达式
template<typename... Args>
void print(Args... args) {
    ((std::cout << args << ", "), ...);  // 一元右折叠
}
```

#### 3. 完美转发

参数包与 `std::forward` 结合实现完美转发：

```cpp
template<typename... Args>
void wrapper(Args&&... args) {
    func(std::forward<Args>(args)...);  // 完美转发参数包
}
```

### 常见陷阱

#### 陷阱 1：参数包位置错误

```cpp
// ❌ 错误：类模板中参数包不在末尾
template<typename... Ts, typename U>
struct Wrong;  // 编译错误

// ✅ 正确：参数包在末尾
template<typename U, typename... Ts>
struct Right;
```

#### 陷阱 2：空包展开语法问题

```cpp
// ❌ 错误：空包展开可能导致语法问题
template<typename... Ts>
void f(Ts...) {}  // 注意这里没有参数名

// 某些情况下空包展开可能产生意外的语法结果
```

#### 陷阱 3：迭代器失效（使用数组展开技巧）

```cpp
// 使用数组展开技巧执行副作用
template<typename... Ts>
void print_all(Ts... args) {
    int dummy[] = {(std::cout << args << " ", 0)...};
    (void)dummy;  // 抑制未使用变量警告
    // 利用初始化列表保证顺序执行
}
```

## 6. 代码示例

### 基础用法：变参类模板

```cpp
#include <iostream>

// 变参类模板
template<class... Types>
struct Tuple {};

int main() {
    Tuple<> t0;           // Types 包含 0 个参数
    Tuple<int> t1;        // Types 包含 1 个参数：int
    Tuple<int, float> t2; // Types 包含 2 个参数：int 和 float
    // Tuple<0> t3;       // 错误：0 不是类型

    return 0;
}
```

### 基础用法：变参函数模板

```cpp
#include <iostream>

// 变参函数模板
template<class... Types>
void f(Types... args) {}

int main() {
    f();       // OK：args 包含 0 个参数
    f(1);      // OK：args 包含 1 个参数：int
    f(2, 1.0); // OK：args 包含 2 个参数：int 和 double

    return 0;
}
```

### 高级用法：printf 风格函数

```cpp
#include <iostream>

// 基础情况：只有格式字符串
void tprintf(const char* format)
{
    std::cout << format;
}

// 递归变参函数模板
template<typename T, typename... Targs>
void tprintf(const char* format, T value, Targs... Fargs)
{
    for (; *format != '\0'; format++)
    {
        if (*format == '%')
        {
            std::cout << value;
            tprintf(format + 1, Fargs...);  // 递归调用
            return;
        }
        std::cout << *format;
    }
}

int main()
{
    tprintf("% world% %\n", "Hello", '!', 123);
    // 输出：Hello world! 123

    return 0;
}
```

### 高级用法：多继承与成员初始化

```cpp
#include <iostream>
#include <string>

// Mixin 类
struct Printable {
    std::string data;
    Printable(const std::string& s) : data(s) {}
    void print() const { std::cout << data << std::endl; }
};

struct Loggable {
    std::string prefix;
    Loggable(const std::string& p) : prefix(p) {}
    void log(const std::string& msg) const { std::cout << prefix << msg << std::endl; }
};

// 变参模板：继承多个 Mixin
template<class... Mixins>
class X : public Mixins...
{
public:
    // 包展开用于基类初始化列表
    X(const Mixins&... mixins) : Mixins(mixins)... {}
};

int main()
{
    X<Printable, Loggable> x(Printable("Hello"), Loggable("[LOG] "));
    x.print();             // 输出：Hello
    x.log("World");        // 输出：[LOG] World

    return 0;
}
```

### 高级用法：Lambda 捕获包 (C++20)

```cpp
#include <iostream>

template<class... Args>
void f(Args... args)
{
    // Lambda 初始化捕获包
    auto lm = [&, args...] { return g(args...); };
    lm();
}

int g() { return 0; }
template<class T, class... Args>
int g(T first, Args... rest) {
    std::cout << first << " ";
    return g(rest...);
}

int main()
{
    f(1, 2, 3);  // 输出：1 2 3
    return 0;
}
```

### C++17 折叠表达式

```cpp
#include <iostream>

// 一元右折叠
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // ((arg1 + arg2) + arg3) + ...
}

// 一元左折叠
template<typename... Args>
auto sum_left(Args... args) {
    return (... + args);  // ... + (arg(n-1) + argn)
}

// 二元折叠
template<typename... Args>
void print_all(Args... args) {
    ((std::cout << args << " "), ...);  // 逗号表达式
}

int main()
{
    std::cout << sum(1, 2, 3, 4, 5) << std::endl;  // 15
    print_all(1, "hello", 3.14);  // 1 hello 3.14

    return 0;
}
```

### C++26 包索引

```cpp
#include <cassert>

// 包索引：访问第一个和最后一个元素
consteval auto first_plus_last(auto... args) {
    return args...[0] + args...[sizeof...(args) - 1];
}

int main()
{
    static_assert(first_plus_last(5) == 10);
    static_assert(first_plus_last(5, 4) == 9);
    static_assert(first_plus_last(5, 6, 2) == 7);

    return 0;
}
```

## 7. 总结

参数包是 C++ 模板元编程的核心特性，它使得 C++ 能够实现类型安全的变参函数和类。

### 核心要点

| 特性 | 说明 |
|------|------|
| **类型安全** | 所有参数在编译期进行类型检查 |
| **编译期处理** | 包展开在编译期完成，无运行时开销 |
| **灵活性** | 支持任意数量和类型的参数 |
| **可组合性** | 与模板、lambda、继承等特性无缝配合 |

### 演进时间线

| 版本 | 主要特性 |
|------|---------|
| C++11 | 参数包、变参模板 |
| C++17 | 折叠表达式简化参数包处理 |
| C++20 | Lambda 初始化捕获包 |
| C++26 | 包索引直接访问元素 |

### 使用建议

1. **优先使用折叠表达式 (C++17)**：相比递归展开更简洁高效
2. **注意参数包位置**：类模板中必须在末尾，函数模板中可在可推导参数之前
3. **结合完美转发**：使用 `std::forward<Args>(args)...` 保持值类别
4. **利用 sizeof...**：获取参数包大小用于条件分支
5. **使用包索引 (C++26)**：直接访问参数包元素，无需递归

### 相关特性

| 特性 | 说明 |
|------|------|
| `sizeof...` | 查询参数包中元素数量 |
| 折叠表达式 (C++17) | 对包元素进行二元运算归约 |
| 包索引 (C++26) | 访问指定索引的包元素 |
| C 风格变参函数 | 类型不安全的传统变参机制 |

### 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/pack
- C++ Standard: [temp.variadic]
- Effective Modern C++, Scott Meyers, Item 14
- 特性测试宏：`__cpp_variadic_templates`, `__cpp_pack_indexing`