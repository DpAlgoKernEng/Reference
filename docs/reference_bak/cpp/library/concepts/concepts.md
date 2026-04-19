# C++概念库（Concepts Library）（C++20起）

来源：cppreference.com

## 概念库概述

概念库提供基本库概念的定义，可用于对模板参数执行编译时验证，并基于类型的属性执行函数分派。这些概念为程序中的等式推理提供了基础。

标准库中的大多数概念都施加了语法和语义要求。如果一个标准概念的语法要求得到满足，则称该概念被**满足**；如果它被满足且其语义要求（如果有）也得到满足，则称该概念被**建模**。

一般来说，只有语法要求可以由编译器检查。如果程序的有效性或含义取决于模板参数序列是否建模了一个概念，而该概念被满足但未被建模，或者在使用点未满足语义要求，则程序是病态的，无需诊断。

## 主要概念分类

### 一、核心语言概念

| 头文件 | 概念 | 说明 |
|--------|------|------|
| `<concepts>` | `same_as`(C++20) | 指定类型与另一类型相同（概念） |
| `<concepts>` | `derived_from`(C++20) | 指定类型派生自另一类型（概念） |
| `<concepts>` | `convertible_to`(C++20) | 指定类型可隐式转换为另一类型（概念） |
| `<concepts>` | `common_reference_with`(C++20) | 指定两个类型共享公共引用类型（概念） |
| `<concepts>` | `common_with`(C++20) | 指定两个类型共享公共类型（概念） |
| `<concepts>` | `integral`(C++20) | 指定类型为整数类型（概念） |
| `<concepts>` | `signed_integral`(C++20) | 指定类型为有符号整数类型（概念） |
| `<concepts>` | `unsigned_integral`(C++20) | 指定类型为无符号整数类型（概念） |
| `<concepts>` | `floating_point`(C++20) | 指定类型为浮点类型（概念） |
| `<concepts>` | `assignable_from`(C++20) | 指定类型可从另一类型赋值（概念） |
| `<concepts>` | `swappable`/`swappable_with`(C++20) | 指定类型可交换或两个类型可相互交换（概念） |

### 二、对象生命周期概念

| 头文件 | 概念 | 说明 |
|--------|------|------|
| `<concepts>` | `destructible`(C++20) | 指定类型的对象可被销毁（概念） |
| `<concepts>` | `constructible_from`(C++20) | 指定类型的变量可从参数类型集构造或绑定（概念） |
| `<concepts>` | `default_initializable`(C++20) | 指定类型的对象可默认构造（概念） |
| `<concepts>` | `move_constructible`(C++20) | 指定类型的对象可移动构造（概念） |
| `<concepts>` | `copy_constructible`(C++20) | 指定类型的对象可拷贝构造和移动构造（概念） |

### 三、比较概念

| 头文件 | 概念 | 说明 |
|--------|------|------|
| `<concepts>` | `equality_comparable`/`equality_comparable_with`(C++20) | 指定`operator ==`为等价关系（概念） |
| `<concepts>` | `totally_ordered`/`totally_ordered_with`(C++20) | 指定类型上的比较运算符产生全序（概念） |
| `<compare>` | `three_way_comparable`/`three_way_comparable_with`(C++20) | 指定`operator <=>`在给定类型上产生一致结果（概念） |

### 四、对象概念

| 头文件 | 概念 | 说明 |
|--------|------|------|
| `<concepts>` | `movable`(C++20) | 指定类型的对象可移动和交换（概念） |
| `<concepts>` | `copyable`(C++20) | 指定类型的对象可拷贝、移动和交换（概念） |
| `<concepts>` | `semiregular`(C++20) | 指定类型的对象可拷贝、移动、交换和默认构造（概念） |
| `<concepts>` | `regular`(C++20) | 指定类型为正则类型，即既是`semiregular`又是`equality_comparable`（概念） |

### 五、可调用概念

| 头文件 | 概念 | 说明 |
|--------|------|------|
| `<concepts>` | `invocable`/`regular_invocable`(C++20) | 指定可调用类型可用给定参数类型集调用（概念） |
| `<concepts>` | `predicate`(C++20) | 指定可调用类型为布尔谓词（概念） |
| `<concepts>` | `relation`(C++20) | 指定可调用类型为二元关系（概念） |
| `<concepts>` | `equivalence_relation`(C++20) | 指定`relation`施加等价关系（概念） |
| `<concepts>` | `strict_weak_order`(C++20) | 指定`relation`施加严格弱序（概念） |

---

## 扩展知识详解

### 一、概念基础理解

#### 1. 基本概念语法
```cpp
#include <concepts>
#include <type_traits>

// 定义简单概念
template<typename T>
concept integral_type = std::is_integral_v<T>;

// 使用requires表达式
template<typename T>
concept addable = requires(T a, T b) {
    a + b;
};

// 复杂约束的概念
template<typename T>
concept container = requires(T t) {
    typename T::value_type;
    typename T::size_type;
    t.size();
    t.begin();
    t.end();
};

// 用法示例
template<integral_type T>
void process_number(T value) {
    // 只接受整数类型
}

template<addable T>
auto add_numbers(T a, T b) {
    return a + b;
}
```

#### 2. 核心语言概念
```cpp
#include <concepts>
#include <iostream>

// same_as概念
template<std::same_as<int> T>
void only_accept_int(T value) {
    std::cout << "Integer: " << value << std::endl;
}

// convertible_to概念
template<std::convertible_to<double> T>
double convert_to_double(T value) {
    return static_cast<double>(value);
}

// integral和floating_point概念
template<std::integral T>
void process_integral(T value) {
    std::cout << "Integral: " << value << std::endl;
}

template<std::floating_point T>
void process_floating_point(T value) {
    std::cout << "Floating point: " << value << std::endl;
}
```

### 二、对象生命周期概念

#### 1. 构造和析构概念
```cpp
#include <concepts>
#include <memory>

// destructible概念
template<std::destructible T>
void safe_delete(T* ptr) {
    delete ptr;  // 确保类型可被销毁
}

// constructible_from概念
template<std::constructible_from<int> T>
T create_from_int(int value) {
    return T{value};  // 可从int构造
}

// move_constructible和copy_constructible
template<std::move_constructible T>
T move_example(T&& value) {
    return std::move(value);  // 可移动构造
}

template<std::copy_constructible T>
T copy_example(const T& value) {
    return T{value};  // 可拷贝构造
}
```

#### 2. 正则类型概念
```cpp
#include <concepts>
#include <iostream>

// semiregular概念 - 支持拷贝、移动、默认构造
template<std::semiregular T>
void semiregular_operations() {
    T obj{};           // 默认构造
    T copy{obj};       // 拷贝构造
    T moved{std::move(obj)};  // 移动构造
    copy = moved;      // 赋值
}

// regular概念 - semiregular + 可比较
template<std::regular T>
bool regular_operations(const T& a, const T& b) {
    return a == b;     // 可相等比较
}
```

### 三、比较和排序概念

#### 1. 相等比较概念
```cpp
#include <concepts>
#include <iostream>

// equality_comparable概念
template<std::equality_comparable T>
bool are_equal(const T& a, const T& b) {
    return a == b;
}

// totally_ordered概念
template<std::totally_ordered T>
void sort_elements(std::vector<T>& vec) {
    std::sort(vec.begin(), vec.end());
}

// equality_comparable_with概念
template<std::equality_comparable_with<int> T>
bool compare_with_int(const T& value, int target) {
    return value == target;
}
```

#### 2. 三路比较概念（C++20）
```cpp
#include <compare>
#include <concepts>

struct Point {
    int x, y;
    
    auto operator<=>(const Point&) const = default;
};

// three_way_comparable概念
template<std::three_way_comparable T>
std::strong_ordering compare_three_way(const T& a, const T& b) {
    return a <=> b;
}

void three_way_example() {
    Point p1{1, 2};
    Point p2{3, 4};
    
    auto result = p1 <=> p2;
    if (result < 0) {
        std::cout << "p1 < p2\n";
    } else if (result > 0) {
        std::cout << "p1 > p2\n";
    } else {
        std::cout << "p1 == p2\n";
    }
}
```

### 四、可调用概念

#### 1. 基本可调用概念
```cpp
#include <concepts>
#include <functional>

// invocable概念
template<std::invocable<int> F>
auto invoke_with_int(F&& f) {
    return std::invoke(std::forward<F>(f), 42);
}

// predicate概念
template<std::predicate<int> P>
bool test_predicate(P&& pred, int value) {
    return std::invoke(std::forward<P>(pred), value);
}

// relation概念
template<std::relation<int, int> R>
bool test_relation(R&& rel, int a, int b) {
    return std::invoke(std::forward<R>(rel), a, b);
}

// 使用示例
void callable_concepts_example() {
    // invocable
    auto lambda = [](int x) { return x * 2; };
    auto result = invoke_with_int(lambda);
    
    // predicate
    auto is_even = [](int x) { return x % 2 == 0; };
    bool even = test_predicate(is_even, 42);
    
    // relation
    auto less = std::less<int>{};
    bool smaller = test_relation(less, 1, 2);
}
```

#### 2. 谓词和关系概念
```cpp
#include <concepts>

// equivalence_relation概念
template<std::equivalence_relation<int, int> R>
bool is_equivalence(R&& rel, int a, int b) {
    // 检查等价关系属性
    return std::invoke(std::forward<R>(rel), a, b);
}

// strict_weak_order概念
template<std::strict_weak_order<int, int> R>
void sort_with_order(std::vector<int>& vec, R&& order) {
    std::sort(vec.begin(), vec.end(), std::forward<R>(order));
}
```

### 五、实际应用示例

#### 1. 泛型算法约束
```cpp
#include <concepts>
#include <vector>
#include <algorithm>

// 约束的查找算法
template<std::forward_iterator I, std::sentinel_for<I> S, typename T>
    requires std::equality_comparable_with<std::iter_value_t<I>, T>
I constrained_find(I first, S last, const T& value) {
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return first;
}

// 约束的排序算法
template<std::random_access_iterator I>
    requires std::totally_ordered<std::iter_value_t<I>>
void constrained_sort(I first, I last) {
    std::sort(first, last);
}

// 使用示例
void algorithm_constraints_example() {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    
    auto it = constrained_find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        std::cout << "Found: " << *it << std::endl;
    }
    
    constrained_sort(vec.begin(), vec.end());
}
```

#### 2. 容器概念约束
```cpp
#include <concepts>
#include <vector>
#include <list>
#include <set>

// 容器概念
template<typename C>
concept sequence_container = 
    std::ranges::range<C> &&
    requires(C& c, const C& cc) {
        typename C::value_type;
        typename C::size_type;
        typename C::iterator;
        c.size();
        c.begin();
        c.end();
        cc.begin();
        cc.end();
    };

// 关联容器概念
template<typename C>
concept associative_container = 
    sequence_container<C> &&
    requires(C& c, const typename C::key_type& k) {
        c.find(k);
        c.count(k);
    };

// 使用约束的泛型函数
template<sequence_container C>
void print_container(const C& container) {
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

template<associative_container C>
bool contains_key(const C& container, const typename C::key_type& key) {
    return container.find(key) != container.end();
}
```

#### 3. 类型特征推理
```cpp
#include <concepts>
#include <type_traits>

// 复合概念示例
template<typename T>
concept numeric = std::integral<T> || std::floating_point<T>;

template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept signed_numeric = numeric<T> && std::is_signed_v<T>;

// 带条件的概念
template<typename T>
concept comparable_container = 
    std::ranges::range<T> && 
    std::equality_comparable<std::ranges::range_value_t<T>>;

// 使用复合概念
template<numeric T>
T safe_divide(T a, T b) {
    if (b == T{0}) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

template<comparable_container C>
bool containers_equal(const C& a, const C& b) {
    return std::ranges::equal(a, b);
}
```

#### 4. 模板特化和重载
```cpp
#include <concepts>

// 基于概念的函数重载
template<std::integral T>
void process(T value) {
    std::cout << "Processing integral: " << value << std::endl;
}

template<std::floating_point T>
void process(T value) {
    std::cout << "Processing floating point: " << value << std::endl;
}

template<std::constructible_from<std::string_view> T>
void process(const T& value) {
    std::cout << "Processing string-like: " << value << std::endl;
}

// 概念优先级示例
template<typename T>
void ambiguous_function(T value) {
    std::cout << "Generic version" << std::endl;
}

template<std::integral T>
void ambiguous_function(T value) {
    std::cout << "Integral version: " << value << std::endl;
}

// 使用示例
void overload_example() {
    process(42);           // integral版本
    process(3.14);         // floating_point版本
    process("hello");      // constructible_from版本
    
    ambiguous_function(42);    // integral版本（更特化）
    ambiguous_function(3.14);  // generic版本
}
```

### 六、高级概念模式

#### 1. 自定义概念组合
```cpp
#include <concepts>

// 自定义复合概念
template<typename T>
concept sortable_range = 
    std::ranges::random_access_range<T> &&
    std::totally_ordered<std::ranges::range_value_t<T>>;

template<typename T>
concept hashable = requires(T t) {
    std::hash<T>{}(t);
};

template<typename T>
concept streamable = requires(std::ostream& os, T t) {
    os << t;
};

// 复杂业务逻辑概念
template<typename Container, typename Predicate>
concept filterable_container = 
    std::ranges::range<Container> &&
    std::predicate<Predicate, std::ranges::range_value_t<Container>> &&
    std::constructible_from<Container>;

// 使用复杂概念
template<sortable_range R>
void sort_range(R& range) {
    std::ranges::sort(range);
}

template<hashable T>
std::size_t hash_value(const T& value) {
    return std::hash<T>{}(value);
}

template<streamable T>
void print_value(const T& value) {
    std::cout << value << std::endl;
}
```

#### 2. 概念约束的模板元编程
```cpp
#include <concepts>
#include <type_traits>

// 概念辅助工具
template<typename T>
struct concept_detector {
    template<typename U>
    static auto test(int) -> decltype(
        std::declval<std::equality_comparable<U>>(), 
        std::true_type{}
    );
    
    template<typename>
    static std::false_type test(...);
    
    static constexpr bool value = decltype(test<T>(0))::value;
};

// 条件概念
template<typename T, typename U = T>
concept conditionally_comparable = 
    (std::integral<T> && std::integral<U>) ||
    (std::floating_point<T> && std::floating_point<U>) ||
    std::equality_comparable_with<T, U>;

// 使用概念进行类型选择
template<typename T>
struct type_selector {
    using type = std::conditional_t<
        std::integral<T>,
        std::make_signed_t<T>,
        T
    >;
};

template<typename T>
    requires std::integral<T>
using signed_type_t = typename type_selector<T>::type;
```

## 概念验证和错误处理

### 一、等价保持验证

```cpp
#include <concepts>

// 等价保持要求示例
template<typename T>
concept equality_preserving = requires(T a, T b) {
    // 表达式必须等价保持
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

// 稳定性要求
template<typename T>
concept stable_operations = requires(const T a, const T b) {
    // 常量操作数的多次调用应产生相同结果
    { a == b } -> std::same_as<decltype(a == b)>;
};

// 非修改操作示例
template<typename T>
concept non_modifying_operations = requires(const T a, const T b) {
    // 这些操作不应修改操作数
    a == b;
    a < b;
    a > b;
};
```

### 二、隐式表达式变体

```cpp
// 隐式表达式变体示例
template<class T>
concept detailed_c = requires(T a, T b, const T c, const T d) {
    c == d;           // 表达式#1：不修改操作数
    a = std::move(b); // 表达式#2：修改两个操作数
    a = c;            // 表达式#3：修改左操作数a
};

// 上述概念隐式要求以下变体：
// c == d 的所有const/non-const组合
// a = c 的非const lvalue和const rvalue变体
```

## 标准库集成概念

### 一、迭代器概念
```cpp
#include <iterator>
#include <concepts>

// 不同级别的迭代器概念
void iterator_concepts() {
    // input_iterator - 输入迭代器
    // output_iterator - 输出迭代器
    // forward_iterator - 前向迭代器
    // bidirectional_iterator - 双向迭代器
    // random_access_iterator - 随机访问迭代器
    // contiguous_iterator - 连续迭代器
}
```

### 二、范围概念
```cpp
#include <ranges>
#include <concepts>

// 范围相关概念
void range_concepts() {
    // range - 基本范围
    // sized_range - 有大小的范围
    // view - 视图
    // input_range - 输入范围
    // forward_range - 前向范围
    // bidirectional_range - 双向范围
    // random_access_range - 随机访问范围
    // contiguous_range - 连续范围
}
```

## 最佳实践和注意事项

### 一、概念设计原则

1. **单一职责**：每个概念应有明确、单一的目的
2. **正交性**：概念之间应尽量独立，避免重叠
3. **渐进性**：从简单到复杂构建概念层次
4. **实用性**：概念应解决实际的编程问题

### 二、错误诊断改进

```cpp
#include <concepts>
#include <vector>

// 没有概念的错误信息
template<typename T>
void bad_function(T value) {
    value.some_method();  // 模板实例化时才报错
}

// 有概念的错误信息
template<std::invocable<> F>
void good_function(F func) {
    func();  // 编译时就能提供清晰的错误信息
}

// 使用示例
void error_example() {
    // good_function(42);  // 清晰的错误：int不满足invocable概念
}
```

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [命名要求] | C++98/03时代的模板约束机制 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/concepts&oldid=175880>
- 最后修改时间：2024年9月7日 17:31
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++概念库是现代C++模板编程的重要革新，提供了编译时类型检查和约束的强大机制。通过概念，开发者可以编写更加清晰、安全和自我文档化的泛型代码。理解核心语言概念、对象概念和可调用概念，是掌握现代C++模板编程的基础。概念不仅改善了错误诊断，还支持更好的函数重载和特化机制。正确使用概念可以显著提高代码质量和开发效率。从基本的类型检查到复杂的业务逻辑约束，概念为C++模板系统提供了前所未有的表达能力。持续学习和实践概念编程，是跟上C++现代化发展步伐的必要途径。