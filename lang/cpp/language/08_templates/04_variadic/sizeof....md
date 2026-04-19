# `sizeof...` 运算符

## 1. 概述 (Overview)

`sizeof...` 运算符是 C++11 引入的编译期运算符，用于查询参数包 (parameter pack) 中的元素数量。它是可变参数模板 (variadic templates) 特性的重要组成部分，专门用于获取模板参数包或函数参数包的大小。

**技术定位**：
- 编译期运算符，结果在编译时确定
- 与 `sizeof` 运算符不同，`sizeof...` 专门用于参数包
- 返回值为 `std::size_t` 类型的编译期常量

**主要用途**：
- 在编译期确定参数包包含的元素数量
- 配合可变参数模板进行编译期计算
- 用于静态断言、模板元编程等场景

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++11 之前，C++ 不支持真正的可变参数模板。开发者需要使用以下方式模拟可变参数：
- C 风格的可变参数函数 (`va_list`、`va_start`、`va_end` 宏)
- 预处理器宏
- 代码生成工具

这些方法存在类型不安全、运行期开销、代码可读性差等问题。

### 设计动机

C++11 引入可变参数模板 (Variadic Templates) 特性，解决了以下问题：
1. **类型安全**：模板参数保留类型信息，避免类型擦除
2. **编译期处理**：所有展开在编译期完成，无运行期开销
3. **递归展开**：支持递归式模板实例化，实现灵活的编译期计算

`sizeof...` 运算符作为配套工具，允许开发者在编译期获知参数包大小，从而实现更复杂的模板元编程逻辑。

### 版本变更

| 版本 | 变更内容 |
|------|----------|
| C++11 | 首次引入 `sizeof...` 运算符 |
| C++14 | 配合 `constexpr` 放宽限制，可用于更多常量表达式场景 |
| C++17 | 配合折叠表达式 (fold expressions) 使用更加广泛 |
| C++20 | 在概念 (concepts) 约束中可用于参数包大小检查 |

## 3. 语法与参数 (Syntax and Parameters)

### 核心语法

```cpp
sizeof...( pack )
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `pack` | 参数包名称，可以是模板参数包或函数参数包 |

### 返回值

返回 `std::size_t` 类型的编译期常量，表示参数包中元素的数量。

### 语法要点

1. **必须是参数包**：`sizeof...` 的操作数必须是参数包，不能用于普通变量或类型
2. **编译期常量**：结果是编译期确定的，可用于模板参数、数组大小、静态断言等场景
3. **参数包类型**：
   - 模板参数包 (template parameter pack)：`typename... Ts`
   - 函数参数包 (function parameter pack)：`Ts... args`

### 相关语法对比

| 运算符 | 用途 | 操作数 | 返回类型 |
|--------|------|--------|----------|
| `sizeof` | 获取类型或表达式的大小（字节） | 类型或表达式 | `std::size_t` |
| `sizeof...` | 获取参数包的元素数量 | 参数包 | `std::size_t` |

## 4. 底层原理 (Underlying Principles)

### 实现机制

`sizeof...` 是一个编译期运算符，其工作原理如下：

1. **编译期求值**：编译器在模板实例化阶段计算参数包的大小
2. **类型无关**：不关心参数包中具体元素的类型，只计算数量
3. **零开销抽象**：编译后直接替换为常量值，无任何运行期开销

### 编译器处理流程

```cpp
template<typename... Ts>
void func(Ts... args) {
    constexpr std::size_t count = sizeof...(Ts);    // 模板参数包大小
    constexpr std::size_t args_count = sizeof...(args); // 函数参数包大小
    // 两者相等：count == args_count
}
```

编译器处理步骤：
1. 解析模板参数包声明
2. 记录参数包中参数的数量
3. 遇到 `sizeof...` 时，直接返回已记录的数量
4. 将结果作为编译期常量嵌入生成的代码

### 性能特征

| 特性 | 说明 |
|------|------|
| 时间复杂度 | O(1)，编译期直接获取 |
| 空间开销 | 零运行期开销 |
| 编译期开销 | 极小，仅涉及常量替换 |
| 类型检查 | 无，仅计数不涉及类型操作 |

### 与模板实例化关系

```cpp
// 当调用 func(1, 2.0, 'a') 时
template<typename... Ts>
void func(Ts... args) {
    constexpr auto n = sizeof...(Ts);  // n = 3
}
// 编译器实例化为：
// void func<int, double, char>(int, double, char) { constexpr auto n = 3; }
```

## 5. 使用场景 (Use Cases)

### 适用场景

1. **编译期数组大小确定**

```cpp
template<typename... Ts>
constexpr auto make_array(Ts&&... ts) {
    using CT = std::common_type_t<Ts...>;
    return std::array<CT, sizeof...(Ts)>{std::forward<Ts>(ts)...};
}
```

2. **静态断言检查**

```cpp
template<typename... Ts>
void process() {
    static_assert(sizeof...(Ts) > 0, "At least one type required");
    static_assert(sizeof...(Ts) <= 10, "Too many types");
}
```

3. **递归终止条件**

```cpp
template<typename T>
void print(T value) {
    std::cout << value << std::endl;
}

template<typename T, typename... Ts>
void print(T first, Ts... rest) {
    std::cout << first << ", ";
    print(rest...);  // sizeof...(rest) 递减直到为 0
}
```

4. **条件编译分支选择**

```cpp
template<typename... Ts>
void handle() {
    if constexpr (sizeof...(Ts) == 0) {
        std::cout << "Empty pack\n";
    } else if constexpr (sizeof...(Ts) == 1) {
        std::cout << "Single element\n";
    } else {
        std::cout << "Multiple elements: " << sizeof...(Ts) << "\n";
    }
}
```

5. **配合折叠表达式 (C++17)**

```cpp
template<typename... Ts>
auto sum(Ts... args) {
    // sizeof...(Ts) 可用于验证或日志
    static_assert(sizeof...(Ts) > 0, "Need at least one argument");
    return (args + ...);  // 折叠表达式
}
```

### 6. 最佳实践

1. **优先使用 `constexpr` 变量存储结果**

```cpp
template<typename... Ts>
void func(Ts... args) {
    constexpr std::size_t pack_size = sizeof...(Ts);  // 推荐：明确表达意图
    // 使用 pack_size...
}
```

2. **配合 `if constexpr` 实现编译期分支**

```cpp
template<typename... Ts>
void process() {
    if constexpr (sizeof...(Ts) == 0) {
        // 空包处理
    } else {
        // 非空包处理
    }
}
```

3. **在概念约束中使用 (C++20)**

```cpp
template<typename... Ts>
requires (sizeof...(Ts) > 0)  // 至少一个类型参数
auto make_tuple(Ts... args) {
    return std::make_tuple(args...);
}
```

### 常见陷阱

1. **误用于非参数包**

```cpp
// 错误：不能用于普通变量
int x = 10;
// auto size = sizeof...(x);  // 编译错误

// 错误：不能用于类型
// auto size = sizeof...(int);  // 编译错误
```

2. **混淆 `sizeof` 和 `sizeof...`**

```cpp
template<typename... Ts>
void func(Ts... args) {
    // sizeof...(Ts) 返回参数数量
    constexpr auto count = sizeof...(Ts);

    // sizeof...(args) 同样返回参数数量
    constexpr auto arg_count = sizeof...(args);

    // sizeof...(Ts) != sizeof(Ts)... （后者无意义）
}
```

3. **空参数包处理**

```cpp
template<typename... Ts>
void func() {
    constexpr auto n = sizeof...(Ts);  // Ts 为空时，n = 0
    // 需要考虑空包情况
}
```

## 代码示例 (Examples)

### 示例 1：基础用法

```cpp
#include <iostream>

// 函数参数包
template<typename... Args>
void print_count(Args... args) {
    std::cout << "Template parameter pack size: " << sizeof...(Args) << "\n";
    std::cout << "Function parameter pack size: " << sizeof...(args) << "\n";
}

int main() {
    print_count(1, 2.0, 'a', "hello");
    // 输出:
    // Template parameter pack size: 4
    // Function parameter pack size: 4

    print_count();  // 空参数包
    // 输出:
    // Template parameter pack size: 0
    // Function parameter pack size: 0

    return 0;
}
```

### 示例 2：创建固定大小数组

```cpp
#include <array>
#include <iostream>
#include <type_traits>

template<typename... Ts>
constexpr auto make_array(Ts&&... ts) {
    using CT = std::common_type_t<Ts...>;
    return std::array<CT, sizeof...(Ts)>{std::forward<Ts>(ts)...};
}

int main() {
    std::array<double, 4ul> arr = make_array(1, 2.71f, 3.14, '*');

    std::cout << "arr = { ";
    for (auto s{arr.size()}; double elem : arr) {
        std::cout << elem << (--s ? ", " : " ");
    }
    std::cout << "}\n";
}
```

输出：
```
arr = { 1, 2.71, 3.14, 42 }
```

### 示例 3：编译期条件分支

```cpp
#include <iostream>
#include <string>

template<typename... Ts>
void process_pack() {
    if constexpr (sizeof...(Ts) == 0) {
        std::cout << "Empty parameter pack\n";
    } else if constexpr (sizeof...(Ts) == 1) {
        std::cout << "Single type in pack\n";
    } else {
        std::cout << "Multiple types: " << sizeof...(Ts) << "\n";
    }
}

int main() {
    process_pack<>();           // 输出: Empty parameter pack
    process_pack<int>();        // 输出: Single type in pack
    process_pack<int, double>(); // 输出: Multiple types: 2
    return 0;
}
```

### 示例 4：递归展开终止条件

```cpp
#include <iostream>

// 基础情况：空包
void print_all() {
    std::cout << "(empty)" << std::endl;
}

// 单参数终止
template<typename T>
void print_all(T value) {
    std::cout << value << std::endl;
}

// 递归情况
template<typename T, typename... Rest>
void print_all(T first, Rest... rest) {
    std::cout << first << " (remaining: " << sizeof...(rest) << ") -> ";
    print_all(rest...);
}

int main() {
    print_all(1, 2.0, 'a', "end");
    // 输出: 1 (remaining: 3) -> 2 (remaining: 2) -> a (remaining: 1) -> end
    return 0;
}
```

### 示例 5：常见错误及修正

```cpp
#include <iostream>

// 错误示例 1：误用于非参数包
void bad_example_1() {
    int x = 42;
    // auto size = sizeof...(x);  // 编译错误：x 不是参数包
}

// 错误示例 2：运行期使用误解
template<typename... Ts>
void bad_example_2(Ts... args) {
    // sizeof... 的结果始终是编译期常量
    // 不能用于需要运行期计算的场合
    // int arr[sizeof...(Ts)];  // 这是正确的，编译期确定大小

    // int size;
    // std::cin >> size;
    // int arr[size];  // 错误：运行期大小不能用于数组
}

// 正确用法：结合 static_assert 进行编译期检查
template<typename... Ts>
void good_example() {
    static_assert(sizeof...(Ts) > 0, "Parameter pack cannot be empty");
    static_assert(sizeof...(Ts) <= 5, "Too many type parameters");

    std::cout << "Processing " << sizeof...(Ts) << " type(s)\n";
}

int main() {
    // good_example();  // 编译错误：Parameter pack cannot be empty
    good_example<int, double>();  // 正确
    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 引入版本 | C++11 |
| 用途 | 获取参数包元素数量 |
| 返回类型 | `std::size_t`（编译期常量） |
| 操作数类型 | 模板参数包、函数参数包 |
| 求值时机 | 编译期 |
| 运行期开销 | 零开销 |

### 技术对比

| 运算符 | 操作数 | 用途 | 示例 |
|--------|--------|------|------|
| `sizeof` | 类型或表达式 | 获取字节大小 | `sizeof(int)` -> 4 |
| `sizeof...` | 参数包 | 获取元素数量 | `sizeof...(Ts)` -> N |

### 与其他特性的关联

- **可变参数模板**：`sizeof...` 的主要应用场景
- **折叠表达式 (C++17)**：常与 `sizeof...` 配合使用
- **`if constexpr` (C++17)**：实现编译期分支
- **概念 (C++20)**：可用于约束参数包大小

### 学习建议

1. **理解参数包概念**：先掌握模板参数包和函数参数包的声明与使用
2. **区分 `sizeof` 和 `sizeof...`**：明确两者用途不同
3. **实践编译期编程**：通过模板元编程练习加深理解
4. **关注编译期常量特性**：理解编译期求值的意义和限制
5. **结合现代 C++ 特性**：与 `if constexpr`、折叠表达式、概念等配合使用

### 参考资源

- cppreference: [sizeof... operator](https://en.cppreference.com/w/cpp/language/sizeof...)
- C++ 标准文档: §14.5.3 Variadic templates