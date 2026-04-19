# C++命名要求（Named Requirements）

来源：cppreference.com

## C++命名要求概述

**命名要求**是C++标准中用于定义标准库期望的规范性文本中使用的命名要求。程序员有责任确保库模板使用满足这些要求的模板参数实例化，否则可能导致非常复杂的编译器诊断信息。

C++20中，其中一些要求已使用概念语言特性形式化。

---

## 命名要求分类

### 一、基本要求（Basic Requirements）

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **DefaultConstructible** | C++98 | 指定类型的对象可以进行默认构造 |
| **MoveConstructible** | C++11 | 指定类型的对象可以从右值构造 |
| **CopyConstructible** | C++98 | 指定类型的对象可以从左值构造 |
| **MoveAssignable** | C++11 | 指定类型的对象可以从右值赋值 |
| **CopyAssignable** | C++98 | 指定类型的对象可以从左值赋值 |
| **Destructible** | C++98 | 指定类型的对象可以被销毁 |

### 二、类型属性（Type Properties）

> ⚠️ 注意：标准未定义此类别中指定名称的命名要求。这些是核心语言定义的类型类别。此处作为命名要求列出仅为了保持一致性。

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **ScalarType** | C++98 | 非数组类型或类类型的对象类型 |
| **PODType** | C++98* | POD（Plain Old Data）类型，与C结构体兼容（C++20弃用） |
| **TriviallyCopyable** | C++11 | 这些类型的对象在复制其底层字节后可以保持其值 |
| **TrivialType** | C++11* | 这些类型的对象可以被平凡地构造和复制（C++26弃用） |
| **StandardLayoutType** | C++11 | 这些类型对于与用其他编程语言编写的代码通信很有用 |
| **ImplicitLifetimeType** | C++23 | 这些类型的对象可以被隐式创建，其生命周期可以被隐式开始 |

### 三、库级要求（Library-wide Requirements）

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **BooleanTestable** | C++20 | 布尔运算（operator&&, operator||, operator!）具有通常语义 |
| **EqualityComparable** | C++98 | `operator==` 是等价关系 |
| **LessThanComparable** | C++98 | `operator<` 是严格弱序关系 |
| **Swappable** | C++98 | 可以通过非成员函数调用swap()进行交换 |
| **ValueSwappable** | C++11 | 解引用到Swappable类型的LegacyIterator |
| **NullablePointer** | C++11 | 支持空值的指针类类型 |
| **Hash** | C++11 | 对于不同输入值具有低概率给出相同输出的函数对象 |
| **Allocator** | C++98 | 包含分配信息的类类型 |
| **FunctionObject** | C++98 | 可以使用函数调用语法调用的对象 |
| **Callable** | C++11 | 定义了调用操作的类型 |
| **Predicate** | C++98 | 对一个参数返回可转换为bool的值且不修改参数的函数对象 |
| **BinaryPredicate** | C++98 | 对两个参数返回可转换为bool的值且不修改参数的函数对象 |
| **Compare** | C++98 | 建立排序关系的二元谓词 |

---

## 容器相关要求

### 容器要求

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **Container** | C++98 | 允许使用迭代器访问元素的数据结构 |
| **ReversibleContainer** | C++98 | 使用双向迭代器的容器 |
| **AllocatorAwareContainer** | C++11 | 使用分配器的容器 |
| **SequenceContainer** | C++98 | 线性存储元素的容器 |
| **ContiguousContainer** | C++17 | 元素存储在相邻内存地址的容器 |
| **AssociativeContainer** | C++98 | 通过关联键存储元素的容器 |
| **UnorderedAssociativeContainer** | C++11 | 通过关联键存储在桶中的元素容器 |

### 容器元素要求

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **DefaultInsertable** | C++11 | 元素可以在未初始化存储中默认构造 |
| **CopyInsertable** | C++11 | 元素可以在未初始化存储中拷贝构造 |
| **MoveInsertable** | C++11 | 元素可以在未初始化存储中移动构造 |
| **EmplaceConstructible** | C++11 | 元素可以在未初始化存储中构造 |
| **Erasable** | C++11 | 元素可以使用分配器销毁 |

---

## 迭代器要求

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **LegacyIterator** | C++98 | 访问数据结构内数据的通用概念 |
| **LegacyInputIterator** | C++98 | 可用于读取数据的迭代器 |
| **LegacyOutputIterator** | C++98 | 可用于写入数据的迭代器 |
| **LegacyForwardIterator** | C++98 | 可多次用于读取数据的迭代器 |
| **LegacyBidirectionalIterator** | C++98 | 可递增和递减的迭代器 |
| **LegacyRandomAccessIterator** | C++98 | 可在常数时间内前进的迭代器 |
| **LegacyContiguousIterator** | C++17 | 指向连续分配元素的迭代器 |
| **ConstexprIterator** | C++20 | 可在常量表达式求值期间使用的迭代器 |

---

## 流I/O要求

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **UnformattedInputFunction** | C++98 | 不跳过前导空白字符并计算处理字符数的流输入函数 |
| **FormattedInputFunction** | C++98 | 跳过前导空白字符的流输入函数 |
| **UnformattedOutputFunction** | C++98 | 基本流输出函数 |
| **FormattedOutputFunction** | C++98 | 在错误时设置failbit并返回流引用的流输出函数 |

---

## 格式化要求（C++20）

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **BasicFormatter** | C++20 | 抽象给定格式化参数类型和字符类型的格式化操作 |
| **Formatter** | C++20 | 定义格式化库使用的函数 |

---

## 随机数生成要求

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **SeedSequence** | C++11 | 消费整数序列并产生32位无符号值序列 |
| **UniformRandomBitGenerator** | C++11 | 返回均匀分布的随机无符号整数 |
| **RandomNumberEngine** | C++11 | 由种子定义的确定性UniformRandomBitGenerator |
| **RandomNumberEngineAdaptor** | C++11 | 转换另一个RandomNumberEngine输出的RandomNumberEngine |
| **RandomNumberDistribution** | C++11 | 根据给定数学概率密度函数返回随机数 |

---

## 并发要求

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **BasicLockable** | C++11 | 为执行代理（即线程）提供独占所有权语义 |
| **Lockable** | C++11 | 支持尝试获取锁的BasicLockable |
| **TimedLockable** | C++11 | 支持定时获取锁的Lockable |
| **SharedLockable** | C++14 | 为执行代理（即线程）提供共享所有权语义 |
| **SharedTimedLockable** | C++14 | 支持定时获取锁的SharedLockable |
| **Mutex** | C++11 | 保护数据竞争和顺序一致同步的Lockable |
| **TimedMutex** | C++11 | 保护数据竞争和顺序一致同步的TimedLockable |
| **SharedMutex** | C++17 | 支持共享所有权语义的Mutex |
| **SharedTimedMutex** | C++14 | 支持共享所有权语义的TimedMutex |

---

## 范围要求（C++20）

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **RangeAdaptorObject** | C++20 | 从`viewable_range`和额外参数创建范围适配器的函数对象 |
| **RangeAdaptorClosureObject** | C++20 | 支持管道运算符的函数对象 |

---

## 多维视图定制（C++23）

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **LayoutMapping** | C++23 | 控制mdspan中多维索引的映射 |
| **LayoutMappingPolicy** | C++23 | 保存LayoutMapping要求的策略 |
| **AccessorPolicy** | C++23 | 控制mdspan中数据句柄访问的策略 |

---

## 其他要求

| 要求名称 | 标准版本 | 说明 |
|----------|----------|------|
| **UnaryTypeTrait** | C++11 | 描述类型属性 |
| **BinaryTypeTrait** | C++11 | 描述两个类型间的关系 |
| **TransformationTrait** | C++11 | 修改类型属性 |
| **Clock** | C++11 | 聚合持续时间、时间点和获取当前时间点的函数 |
| **TrivialClock** | C++11 | 不抛出异常的Clock |
| **CharTraits** | C++98 | 为字符类型定义类型和函数 |
| **BitmaskType** | C++98 | 位集、整数或枚举 |
| **NumericType** | C++98 | 初始化等效于赋值的类型 |
| **RegexTraits** | C++11 | 为正则表达式库定义类型和函数 |
| **LiteralType** | C++11 | 具有constexpr构造函数的类型 |

---

## 扩展知识详解

### 一、概念化演进

#### C++20概念示例
```cpp
#include <concepts>

// DefaultConstructible概念
template<typename T>
concept default_constructible = std::is_default_constructible_v<T>;

// MoveConstructible概念
template<typename T>
concept move_constructible = std::is_move_constructible_v<T>;

// CopyConstructible概念
template<typename T>
concept copy_constructible = std::is_copy_constructible_v<T>;

// 组合概念
template<typename T>
concept regular = std::copyable<T> && std::equality_comparable<T>;
```

### 二、各类要求详细说明

#### 1. 基本构造要求
```cpp
// DefaultConstructible示例
class DefaultConstructibleExample {
public:
    DefaultConstructibleExample() = default;  // 默认构造函数
    int value;
};

// MoveConstructible示例
class MoveConstructibleExample {
public:
    MoveConstructibleExample(MoveConstructibleExample&& other) noexcept
        : data(std::move(other.data)) {
        other.data = nullptr;
    }
    
private:
    int* data;
};

// CopyConstructible示例
class CopyConstructibleExample {
public:
    CopyConstructibleExample(const CopyConstructibleExample& other)
        : value(other.value) {
    }
    
private:
    int value;
};
```

#### 2. 比较要求
```cpp
// EqualityComparable示例
class Point {
public:
    int x, y;
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

// LessThanComparable示例
class ComparablePoint {
public:
    int x, y;
    
    bool operator<(const ComparablePoint& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};
```

#### 3. 容器要求实现
```cpp
// Container要求示例
template<typename T>
class SimpleContainer {
public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    
    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }
    
private:
    T* data_;
    std::size_t size_;
};

// SequenceContainer要求示例
template<typename T>
class VectorLike {
public:
    using value_type = T;
    using iterator = T*;
    
    void push_back(const T& value) {
        // 实现序列容器功能
    }
    
    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
};
```

#### 4. 迭代器要求实现
```cpp
// LegacyIterator要求示例
template<typename T>
class SimpleIterator {
public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;
    
    SimpleIterator& operator++() {
        ++ptr_;
        return *this;
    }
    
    reference operator*() const {
        return *ptr_;
    }
    
    bool operator==(const SimpleIterator& other) const {
        return ptr_ == other.ptr_;
    }
    
private:
    T* ptr_;
};

// LegacyRandomAccessIterator要求示例
template<typename T>
class RandomAccessIterator {
public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    
    // 随机访问操作
    RandomAccessIterator& operator+=(difference_type n) {
        ptr_ += n;
        return *this;
    }
    
    RandomAccessIterator operator+(difference_type n) const {
        return RandomAccessIterator(ptr_ + n);
    }
    
    reference operator[](difference_type n) const {
        return ptr_[n];
    }
    
    difference_type operator-(const RandomAccessIterator& other) const {
        return ptr_ - other.ptr_;
    }
    
private:
    T* ptr_;
};
```

#### 5. 函数对象要求
```cpp
// FunctionObject要求示例
struct FunctionObject {
    int operator()(int x, int y) const {
        return x + y;
    }
};

// Predicate要求示例
struct IsEven {
    bool operator()(int x) const {
        return x % 2 == 0;
    }
};

// Compare要求示例
struct Less {
    bool operator()(const int& a, const int& b) const {
        return a < b;
    }
};
```

### 三、并发要求实现

#### 1. Lockable要求实现
```cpp
// BasicLockable示例
class SimpleMutex {
public:
    void lock() {
        // 获取锁的实现
        while (locked_.exchange(true, std::memory_order_acquire)) {
            // 自旋等待
        }
    }
    
    void unlock() {
        locked_.store(false, std::memory_order_release);
    }
    
private:
    std::atomic<bool> locked_{false};
};

// Lockable示例
class TimedMutex {
public:
    void lock() {
        // 阻塞式获取锁
    }
    
    bool try_lock() {
        // 尝试获取锁
        return !locked_.exchange(true, std::memory_order_acquire);
    }
    
    void unlock() {
        locked_.store(false, std::memory_order_release);
    }
    
private:
    std::atomic<bool> locked_{false};
};
```

#### 2. 随机数要求实现
```cpp
// UniformRandomBitGenerator示例
class SimpleRandomGenerator {
public:
    using result_type = unsigned int;
    
    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return UINT_MAX; }
    
    result_type operator()() {
        // 简单的线性同余生成器
        seed_ = (seed_ * 1103515245 + 12345) & 0x7fffffff;
        return seed_;
    }
    
private:
    result_type seed_{1};
};
```

### 四、现代C++实践

#### 1. 概念约束使用（C++20）
```cpp
#include <concepts>

// 使用标准概念
template<std::integral T>
T add(T a, T b) {
    return a + b;
}

// 自定义概念
template<typename T>
concept sortable_container = 
    std::ranges::random_access_range<T> && 
    std::totally_ordered<std::ranges::range_value_t<T>>;

template<sortable_container Container>
void sort_container(Container& container) {
    std::ranges::sort(container);
}
```

#### 2. 范围适配器要求（C++20）
```cpp
#include <ranges>

// RangeAdaptorObject示例
auto even_filter = [](auto&& range) {
    return std::forward<decltype(range)>(range) 
        | std::views::filter([](const auto& x) { return x % 2 == 0; });
};

// RangeAdaptorClosureObject示例
auto operator|(auto&& range, const auto& adaptor) {
    return adaptor(std::forward<decltype(range)>(range));
}
```

#### 3. 格式化要求（C++20）
```cpp
#include <format>

// Formatter要求示例
template<>
struct std::formatter<MyClass> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
    
    template<typename FormatContext>
    auto format(const MyClass& obj, FormatContext& ctx) {
        return format_to(ctx.out(), "MyClass({})", obj.value);
    }
};
```

---

## 实际应用示例

### 一、容器适配器实现
```cpp
#include <iterator>
#include <memory>

template<typename T, typename Container = std::vector<T>>
class Stack {
    static_assert(std::is_same_v<T, typename Container::value_type>);
    
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    
    // 满足Container要求
    reference top() { return container_.back(); }
    const_reference top() const { return container_.back(); }
    
    void push(const value_type& value) { container_.push_back(value); }
    void pop() { container_.pop_back(); }
    
    bool empty() const { return container_.empty(); }
    size_type size() const { return container_.size(); }
    
private:
    Container container_;
};
```

### 二、迭代器适配器实现
```cpp
#include <iterator>

template<typename Iterator>
class ReverseIterator {
public:
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;
    
    ReverseIterator(Iterator it) : current_(it) {}
    
    reference operator*() const {
        Iterator temp = current_;
        return *--temp;
    }
    
    ReverseIterator& operator++() {
        --current_;
        return *this;
    }
    
    ReverseIterator& operator--() {
        ++current_;
        return *this;
    }
    
    bool operator==(const ReverseIterator& other) const {
        return current_ == other.current_;
    }
    
private:
    Iterator current_;
};
```

### 三、函数对象包装器实现
```cpp
#include <functional>

template<typename Func>
class FunctionWrapper {
public:
    template<typename F>
    FunctionWrapper(F&& f) : func_(std::forward<F>(f)) {}
    
    template<typename... Args>
    auto operator()(Args&&... args) -> decltype(func_(std::forward<Args>(args)...)) {
        return func_(std::forward<Args>(args)...);
    }
    
private:
    std::function<Func> func_;
};

// 使用示例
auto add_lambda = [](int a, int b) { return a + b; };
FunctionWrapper<int(int, int)> wrapper(add_lambda);
int result = wrapper(3, 4);  // 结果: 7
```

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/named_req&oldid=178049>
- 最后修改时间：2024年11月26日 21:46
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++命名要求是标准库设计的基础，定义了模板参数必须满足的约束条件。理解这些要求有助于编写符合标准的通用代码和避免复杂的编译错误。从C++98的基础要求到C++20的概念化，C++语言不断演进以提供更清晰、更安全的约束表达方式。程序员需要确保模板参数满足相应的命名要求，以保证代码的正确性和效率。现代C++通过概念（concepts）将这些要求形式化，提供了编译时检查和更好的错误信息。正确理解和应用这些要求，是成为高级C++开发者的关键技能之一。随着标准的演进，越来越多的要求被概念化，使模板编程更加直观和安全。掌握各种要求的具体含义和实现方式，能够帮助开发者设计出更加健壮和高效的库组件。