# C++工具库（Utility Library）

来源：cppreference.com

## 工具库概述

C++包含各种工具库，提供从位计算到部分函数应用的功能。这些库可以大致分为两组：

1. **语言支持库**
2. **通用工具库**

---

## 语言支持库

语言支持库提供与语言特性密切交互并支持常见语言习语的类和函数。

### 类型支持

- 基本类型（如`std::size_t`、`std::nullptr_t`）
- RTTI（如`std::type_info`）

### 元编程（C++11起）

- 类型特征（如`std::is_integral`、`std::rank`）
- 编译时常量（如`std::integer_sequence`、`std::ratio`）

### 常量求值上下文（C++20起）

| 头文件 | 函数/类 |
|--------|---------|
| `<type_traits>` | `is_constant_evaluated`(C++20) - 检测调用是否发生在常量求值上下文中（函数） |
| `<type_traits>` | `is_within_lifetime`(C++26) - 在编译时检查指针是否在对象生命周期内（函数） |

### 实现属性（C++20起）

头文件`<version>`提供关于C++标准库的实现相关的信息（如版本号和发布日期），还定义了库特性测试宏。

### 程序工具

- 终止（如`std::abort`、`std::atexit`）
- 环境（如`std::system`）
- 信号（如`std::raise`）

### 动态内存管理

- 智能指针（如`std::shared_ptr`）
- 分配器（如`std::allocator`或`std::pmr::memory_resource`）
- C风格内存管理（如`std::malloc`）

### 错误处理

- 异常（如`std::exception`、`std::terminate`）
- 断言（如`assert`）

### 源代码信息捕获（C++20起）

| 头文件 | 类 |
|--------|----|
| `<source_location>` | `source_location`(C++20) - 表示源代码信息的类，如文件名、行号和函数名 |

### 初始化列表（C++11起）

| 头文件 | 类模板 |
|--------|--------|
| `<initializer_list>` | `initializer_list`(C++11) - 引用在列表初始化中创建的临时数组 |

### 三路比较（C++20起）

| 头文件 | 组件 |
|--------|------|
| `<compare>` | `three_way_comparable`/`three_way_comparable_with`(C++20) - 指定`operator<=>`在给定类型上产生一致结果的概念 |
| `<compare>` | `partial_ordering`(C++20) - 支持所有6个运算符的三路比较结果类型，不可替换，允许不可比较的值 |
| `<compare>` | `weak_ordering`(C++20) - 支持所有6个运算符且不可替换的三路比较结果类型 |
| `<compare>` | `strong_ordering`(C++20) - 支持所有6个运算符且可替换的三路比较结果类型 |
| `<compare>` | `is_eq`/`is_neq`/`is_lt`/`is_lteq`/`is_gt`/`is_gteq`(C++20) - 命名比较函数 |
| `<compare>` | `compare_three_way`(C++20) - 实现`x <=> y`的约束函数对象 |
| `<compare>` | `compare_three_way_result`(C++20) - 获取给定类型上三路比较运算符`<=>`的结果类型 |
| `<compare>` | `common_comparison_category`(C++20) - 所有给定类型都能转换到的最强比较类别 |
| `<compare>` | `strong_order`(C++20) - 执行三路比较并产生`std::strong_ordering`类型结果 |
| `<compare>` | `weak_order`(C++20) - 执行三路比较并产生`std::weak_ordering`类型结果 |
| `<compare>` | `partial_order`(C++20) - 执行三路比较并产生`std::partial_ordering`类型结果 |
| `<compare>` | `compare_strong_order_fallback`(C++20) - 执行三路比较并产生`std::strong_ordering`类型结果，即使`operator<=>`不可用 |
| `<compare>` | `compare_weak_order_fallback`(C++20) - 执行三路比较并产生`std::weak_ordering`类型结果，即使`operator<=>`不可用 |
| `<compare>` | `compare_partial_order_fallback`(C++20) - 执行三路比较并产生`std::partial_ordering`类型结果，即使`operator<=>`不可用 |

### 协程支持（C++20起）

协程支持的类型，如`std::coroutine_traits`、`std::coroutine_handle`。

### 可变参数函数

支持接受任意数量参数的函数（通过`va_start`、`va_arg`、`va_end`等）。

---

## 通用工具库

### 交换操作

| 头文件 | 组件 |
|--------|------|
| `<utility>` | `swap` - 交换两个对象的值（函数模板） |
| `<utility>` | `exchange`(C++14) - 用新值替换参数并返回其先前值（函数模板） |
| `<concepts>` | `ranges::swap`(C++20) - 交换两个对象的值（定制点对象） |

### 类型操作（C++11起）

| 头文件 | 组件 |
|--------|------|
| `<utility>` | `forward`(C++11) - 转发函数参数并使用类型模板参数保持其值类别（函数模板） |
| `<utility>` | `forward_like`(C++23) - 转发函数参数，如同将其强制转换为指定类型模板参数的表达式的值类别和常量性（函数模板） |
| `<utility>` | `move`(C++11) - 将参数转换为xvalue（函数模板） |
| `<utility>` | `move_if_noexcept`(C++11) - 如果移动构造函数不抛出异常，则将参数转换为xvalue（函数模板） |
| `<utility>` | `as_const`(C++17) - 获取参数的const引用（函数模板） |
| `<utility>` | `declval`(C++11) - 在未求值上下文中获取模板类型参数对象的引用（函数模板） |
| `<utility>` | `to_underlying`(C++23) - 将枚举转换为其底层类型（函数模板） |

### 整数比较函数（C++20起）

| 头文件 | 组件 |
|--------|------|
| `<utility>` | `cmp_equal`/`cmp_not_equal`/`cmp_less`/`cmp_greater`/`cmp_less_equal`/`cmp_greater_equal`(C++20) - 比较两个整数值，确保有符号负数小于无符号数（函数模板） |
| `<utility>` | `in_range`(C++20) - 检查整数值是否在给定整数类型的范围内（函数模板） |

### 关系运算符（C++20前）

| 头文件 | 组件 |
|--------|------|
| `<utility>` | `std::rel_ops`命名空间中的`operator!=`/`operator>`/`operator<=`/`operator>=`（C++20弃用） - 基于用户定义的`operator==`和`operator<`自动生成比较运算符（函数模板） |

### 构造标签（C++11起）

| 头文件 | 组件 |
|--------|------|
| `<utility>` | `piecewise_construct`/`piecewise_construct_t`(C++11) - 逐段构造标签 |
| `<utility>` | `in_place`/`in_place_type`/`in_place_index`/`in_place_t`/`in_place_type_t`/`in_place_index_t`(C++17) - 就地构造标签 |
| `<utility>` | `nontype`/`nontype_t`(C++26) - 值构造标签 |

### 对和元组

| 头文件 | 组件 |
|--------|------|
| `<utility>` | `pair` - 实现二元元组，即一对值（类模板） |
| `<tuple>` | `tuple`(C++11) - 实现固定大小容器，可容纳可能不同类型元素（类模板） |
| `<tuple>` | `apply`(C++17) - 使用参数元组调用函数（函数模板） |
| `<tuple>` | `make_from_tuple`(C++17) - 使用参数元组构造对象（函数模板） |

#### 元组协议（C++11起）

| 头文件 | 组件 |
|--------|------|
| `<tuple>` | `tuple_size`(C++11) - 获取类似元组类型的元素数量（类模板） |
| `<utility>` | `tuple_element`(C++11) - 获取类似元组类型的元素类型（类模板） |

### 和类型和类型擦除包装器（C++17起）

| 头文件 | 组件 |
|--------|------|
| `<optional>` | `optional`(C++17) - 可能持有或不持有对象的包装器（类模板） |
| `<expected>` | `expected`(C++23) - 包含期望值或错误值的包装器（类模板） |
| `<variant>` | `variant`(C++17) - 类型安全的区分联合（类模板） |
| `<any>` | `any`(C++17) - 持有任意可复制构造类型实例的对象（类） |

### 位集

| 头文件 | 组件 |
|--------|------|
| `<bitset>` | `bitset` - 实现固定长度位数组（类模板） |

### 位操作（C++20起）

头文件`<bit>`提供多个函数模板来访问、操作和处理单个位和位序列。可以通过`std::endian`工具检查标量类型的字节序（端序）。

### 函数对象（C++11起）

- 部分函数应用（如`std::bind`）和相关工具：绑定工具如`std::ref`和`std::placeholders`
- 多态函数包装器：`std::function`
- 预定义函子（如`std::plus`、`std::equal_to`）
- 成员指针到函数转换器`std::mem_fn`

### 哈希支持（C++11起）

| 头文件 | 组件 |
|--------|------|
| `<functional>` | `hash`(C++11) - 哈希函数对象（类模板） |

---

## 扩展知识详解

### 一、现代C++工具库演进

#### 1. 基础工具（C++98/03）
```cpp
#include <utility>
#include <functional>

void basic_utilities() {
    // 交换操作
    int a = 1, b = 2;
    std::swap(a, b);
    
    // 函数对象
    std::plus<int> adder;
    int result = adder(3, 4);
    
    // 对类型
    std::pair<int, std::string> p(42, "hello");
    std::cout << p.first << ", " << p.second << std::endl;
}
```

#### 2. 现代化改进（C++11/14）
```cpp
#include <utility>
#include <tuple>
#include <functional>

void modern_utilities() {
    // 移动语义
    std::string str = "hello";
    std::string moved = std::move(str);
    
    // 完美转发
    template<typename T>
    void wrapper(T&& arg) {
        process(std::forward<T>(arg));
    }
    
    // 元组
    auto t = std::make_tuple(1, "hello", 3.14);
    int i;
    std::string s;
    double d;
    std::tie(i, s, d) = t;
    
    // 函数包装器
    std::function<int(int, int)> func = [](int a, int b) { return a + b; };
    int sum = func(3, 4);
}
```

#### 3. 现代特性（C++17/20）
```cpp
#include <optional>
#include <variant>
#include <any>
#include <compare>

void cpp17_20_utilities() {
    // 可选值
    std::optional<int> opt = 42;
    if (opt.has_value()) {
        std::cout << *opt << std::endl;
    }
    
    // 变体
    std::variant<int, std::string> var = 42;
    var = "hello";
    
    // 任意类型
    std::any any_value = 42;
    any_value = std::string("hello");
    
    // 三路比较
    struct Point {
        int x, y;
        auto operator<=>(const Point&) const = default;
    };
    Point p1{1, 2}, p2{3, 4};
    auto cmp = p1 <=> p2;
}
```

#### 4. 前沿发展（C++23/26）
```cpp
#include <expected>
#include <source_location>
#include <bit>

void cpp23_utilities() {
    // 预期值
    std::expected<int, std::string> result = 42;
    if (result) {
        std::cout << *result << std::endl;
    } else {
        std::cout << "Error: " << result.error() << std::endl;
    }
    
    // 源代码位置
    auto loc = std::source_location::current();
    std::cout << "File: " << loc.file_name() 
              << ", Line: " << loc.line() << std::endl;
    
    // 位操作
    uint32_t value = 0b10101010;
    bool has_single_bit = std::has_single_bit(value);
    int bit_count = std::popcount(value);
}
```

### 二、关键工具详解

#### 1. 移动语义和完美转发
```cpp
#include <utility>
#include <memory>

class MyClass {
public:
    // 移动构造函数
    MyClass(MyClass&& other) noexcept 
        : data_(std::move(other.data_)) {
    }
    
    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
        }
        return *this;
    }
    
    // 完美转发构造函数
    template<typename T>
    explicit MyClass(T&& value) 
        : data_(std::forward<T>(value)) {
    }
    
private:
    std::vector<int> data_;
};

// 完美转发函数模板
template<typename Func, typename... Args>
auto invoke(Func&& f, Args&&... args) 
    -> decltype(std::forward<Func>(f)(std::forward<Args>(args)...)) {
    return std::forward<Func>(f)(std::forward<Args>(args)...);
}
```

#### 2. 智能包装器
```cpp
#include <optional>
#include <variant>
#include <expected>

// 可选值使用
std::optional<int> find_value(const std::vector<int>& vec, int target) {
    auto it = std::find(vec.begin(), vec.end(), target);
    if (it != vec.end()) {
        return *it;
    }
    return std::nullopt;
}

// 变体类型使用
using JsonValue = std::variant<std::monostate, int, double, std::string, 
                               std::vector<class JsonValue>>;

// 预期值使用
std::expected<std::string, std::error_code> read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return std::unexpected(std::make_error_code(std::errc::no_such_file_or_directory));
    }
    return std::string(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>());
}
```

#### 3. 元组和结构化绑定
```cpp
#include <tuple>
#include <string>

// 创建和使用元组
auto create_person() {
    return std::make_tuple("John", "Doe", 30);
}

void use_person() {
    auto [first, last, age] = create_person();  // C++17结构化绑定
    
    // 元组操作
    auto person = create_person();
    std::get<0>(person);  // 获取第一个元素
    std::get<std::string>(person);  // 按类型获取
    
    // 应用函数到元组
    auto print_person = [](const std::string& first, 
                          const std::string& last, int age) {
        std::cout << first << " " << last << ", " << age << " years old\n";
    };
    std::apply(print_person, person);
}
```

#### 4. 位操作工具
```cpp
#include <bit>
#include <bitset>

void bit_operations() {
    uint32_t value = 0b10101010;
    
    // 位计数操作
    int popcount = std::popcount(value);        // 设置位数
    int countl_zero = std::countl_zero(value);  // 前导零位数
    int countr_zero = std::countr_zero(value);  // 后导零位数
    
    // 位操作
    bool single_bit = std::has_single_bit(value);  // 是否只有一个设置位
    uint32_t bit_ceil = std::bit_ceil(value);      // 大于等于的2的幂
    uint32_t bit_floor = std::bit_floor(value);    // 小于等于的2的幂
    
    // 位旋转
    uint32_t rotated = std::rotl(value, 4);  // 向左旋转4位
    
    // 字节序检查
    if (std::endian::native == std::endian::little) {
        std::cout << "Little endian system\n";
    }
}
```

### 三、实际应用示例

#### 1. 现代错误处理框架
```cpp
#include <expected>
#include <system_error>
#include <string>

template<typename T>
class Result {
public:
    using value_type = T;
    using error_type = std::error_code;
    
    Result(T value) : result_(std::move(value)) {}
    Result(std::error_code ec) : result_(std::unexpected(ec)) {}
    
    bool is_success() const { return result_.has_value(); }
    bool is_error() const { return !result_.has_value(); }
    
    const T& value() const { return result_.value(); }
    const std::error_code& error() const { return result_.error(); }
    
private:
    std::expected<T, std::error_code> result_;
};

// 使用示例
Result<std::string> load_config(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return std::make_error_code(std::errc::no_such_file_or_directory);
    }
    return std::string(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>());
}
```

#### 2. 函数式编程工具
```cpp
#include <functional>
#include <tuple>
#include <utility>

// 部分应用函数
template<typename Func, typename... BoundArgs>
auto partial(Func&& f, BoundArgs&&... bound_args) {
    return [f = std::forward<Func>(f), 
            bound_args = std::make_tuple(std::forward<BoundArgs>(bound_args)...)](auto&&... rest_args) {
        return std::apply([&](auto&&... bound) {
            return f(std::forward<decltype(bound)>(bound)..., 
                    std::forward<decltype(rest_args)>(rest_args)...);
        }, bound_args);
    };
}

// 函数组合
template<typename F, typename G>
auto compose(F&& f, G&& g) {
    return [f = std::forward<F>(f), g = std::forward<G>(g)](auto&&... args) {
        return f(g(std::forward<decltype(args)>(args)...));
    };
}

// 使用示例
auto add = [](int a, int b) { return a + b; };
auto multiply_by_2 = [](int x) { return x * 2; };

auto add_5 = partial(add, 5);
auto composed = compose(multiply_by_2, add_5);

int result = composed(3);  // (3 + 5) * 2 = 16
```

#### 3. 类型擦除容器
```cpp
#include <any>
#include <vector>
#include <typeinfo>

class TypeErasedContainer {
public:
    template<typename T>
    void push(T&& value) {
        data_.push_back(std::forward<T>(value));
    }
    
    template<typename T>
    T& get(size_t index) {
        return std::any_cast<T&>(data_.at(index));
    }
    
    const std::type_info& type(size_t index) const {
        return data_.at(index).type();
    }
    
    size_t size() const { return data_.size(); }
    
private:
    std::vector<std::any> data_;
};

// 使用示例
void type_erased_example() {
    TypeErasedContainer container;
    container.push(42);
    container.push(std::string("hello"));
    container.push(3.14);
    
    int int_val = container.get<int>(0);
    std::string str_val = container.get<std::string>(1);
    double double_val = container.get<double>(2);
}
```

#### 4. 现代配置系统
```cpp
#include <variant>
#include <string>
#include <unordered_map>
#include <optional>

class ConfigValue {
public:
    using Value = std::variant<int, double, std::string, bool>;
    
    template<typename T>
    ConfigValue(T&& value) : value_(std::forward<T>(value)) {}
    
    template<typename T>
    std::optional<T> as() const {
        if (const T* ptr = std::get_if<T>(&value_)) {
            return *ptr;
        }
        return std::nullopt;
    }
    
    const std::type_info& type() const {
        return value_.type();
    }
    
private:
    Value value_;
};

class Config {
public:
    void set(const std::string& key, ConfigValue value) {
        config_[key] = std::move(value);
    }
    
    template<typename T>
    std::optional<T> get(const std::string& key) const {
        auto it = config_.find(key);
        if (it != config_.end()) {
            return it->second.as<T>();
        }
        return std::nullopt;
    }
    
private:
    std::unordered_map<std::string, ConfigValue> config_;
};

// 使用示例
void config_example() {
    Config config;
    config.set("port", 8080);
    config.set("host", std::string("localhost"));
    config.set("debug", true);
    
    auto port = config.get<int>("port");
    auto host = config.get<std::string>("host");
    auto debug = config.get<bool>("debug");
}
```

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C工具库文档] | C语言对应文档 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/utility&oldid=179910>
- 最后修改时间：2025年1月28日 11:50
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++工具库是现代C++编程的基础，提供了从基本类型操作到高级类型擦除的全面功能。理解工具库的演进历程和各组件的使用方法，对于编写高效、安全的C++代码至关重要。从C++98的基础工具到C++23的前沿特性，工具库不断演进以满足现代软件开发的需求。掌握移动语义、完美转发、智能包装器等关键概念，能够显著提高代码的性能和可维护性。正确使用三路比较、位操作、函数对象等现代特性，可以让代码更加简洁和表达力强。类型擦除和和类型等高级工具为处理复杂的数据结构提供了灵活的解决方案。工具库的设计遵循零开销抽象原则，确保了性能的同时提供了强大的功能。持续学习和实践工具库的新特性，是成为优秀C++开发者的重要途径。