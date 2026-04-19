# C++迭代器库（Iterator Library）

来源：cppreference.com

## 迭代器库概述

迭代器是泛化指针的概念，它允许C++程序以统一的方式处理不同的数据结构（例如容器和范围（C++20起））。迭代器库提供了迭代器的定义、迭代器特征、适配器和实用函数。

由于迭代器是指针的抽象，它们的语义是C++中指针语义的泛化。这确保了每个接受迭代器的函数模板都能像处理常规指针一样工作。

## 迭代器类型

### 一、迭代器概念（C++20起）

基于新的[概念系统](concept.html)，与C++17迭代器不同。虽然基本分类保持相似，但各个迭代器类别的要求略有不同。

| 概念 | 说明 |
|------|------|
| `indirectly_readable`(C++20) | 指定类型可以通过应用运算符`*`间接读取（概念） |
| `indirectly_writable`(C++20) | 指定值可以写入迭代器引用的对象（概念） |
| `weakly_incrementable`(C++20) | 指定`semiregular`类型可以用前后增量运算符递增（概念） |
| `input_iterator`(C++20) | 指定类型是输入迭代器，即其引用的值可以读取，并且可以前后递增（概念） |
| `output_iterator`(C++20) | 指定类型是给定值类型的输出迭代器，即可向其写入该类型的值并可前后递增（概念） |
| `forward_iterator`(C++20) | 指定`input_iterator`是前向迭代器，支持相等比较和多遍访问（概念） |
| `bidirectional_iterator`(C++20) | 指定`forward_iterator`是双向迭代器，支持向后移动（概念） |
| `random_access_iterator`(C++20) | 指定`bidirectional_iterator`是随机访问迭代器，支持常数时间推进和下标访问（概念） |
| `contiguous_iterator`(C++20) | 指定`random_access_iterator`是连续迭代器，引用内存中连续的元素（概念） |

### 二、迭代器原语

| 组件 | 说明 |
|------|------|
| `iterator_traits` | 为迭代器属性提供统一接口（类模板） |
| `input_iterator_tag`等 | 用于指示迭代器类别的空类类型（类） |

### 三、迭代器适配器

| 适配器 | 说明 |
|--------|------|
| `reverse_iterator` | 反向遍历的迭代器适配器（类模板） |
| `back_insert_iterator` | 在容器末尾插入的迭代器适配器（类模板） |
| `move_iterator`(C++11) | 解引用为右值的迭代器适配器（类模板） |
| `common_iterator`(C++20) | 将迭代器类型及其哨兵适配为通用迭代器类型（类模板） |

### 四、迭代器操作

| 操作 | 说明 |
|------|------|
| `advance` | 按给定距离推进迭代器（函数模板） |
| `distance` | 返回两个迭代器之间的距离（函数模板） |
| `next`(C++11) | 递增迭代器（函数模板） |
| `prev`(C++11) | 递减迭代器（函数模板） |

### 五、范围访问（C++11起）

| 函数 | 说明 |
|------|------|
| `begin`/`cbegin`(C++11)(C++14) | 返回容器或数组开头的迭代器（函数模板） |
| `end`/`cend`(C++11)(C++14) | 返回容器或数组结尾的迭代器（函数模板） |
| `size`(C++17) | 返回容器或数组的大小（函数模板） |

## 迭代器分类

### 传统迭代器类别（C++17前为五种，C++17后为六种）
1. LegacyInputIterator
2. LegacyOutputIterator
3. LegacyForwardIterator
4. LegacyBidirectionalIterator
5. LegacyRandomAccessIterator
6. LegacyContiguousIterator（C++17起）

### 迭代器操作支持矩阵

| 迭代器类别 | 写入 | 读取 | 递增 | 递减 | 随机访问 | 连续存储 |
|------------|------|------|------|------|----------|----------|
| LegacyIterator |  |  | 必需 |  |  |  |
| LegacyOutputIterator | 必需 |  | 必需 |  |  |  |
| LegacyInputIterator |  | 必需 | 必需 |  |  |  |
| LegacyForwardIterator |  | 必需 | 必需 | 必需 |  |  |
| LegacyBidirectionalIterator |  | 必需 | 必需 | 必需 |  |  |
| LegacyRandomAccessIterator |  | 必需 | 必需 | 必需 | 必需 |  |
| LegacyContiguousIterator |  | 必需 | 必需 | 必需 | 必需 | 必需 |

## 核心定义

### 一、类型和可写性

输入迭代器`i`支持表达式`*i`，结果是某种对象类型`T`，称为迭代器的**值类型**。

输出迭代器`i`有一组非空的类型可以**写入**（C++20前）或`indirectly_writable`（C++20起）到迭代器；对于每种这样的类型`T`，表达式`*i = o`有效，其中`o`是类型`T`的值。

对于每个定义了相等性的迭代器类型`X`（C++20前），都有一个对应的有符号整数（C++20前）整数类型（C++20起），称为迭代器的**差值类型**。

### 二、解引用性和有效性

就像指向数组的常规指针保证存在指向数组最后一个元素之后的指针值一样，对于任何迭代器类型都存在指向相应序列最后一个元素之后的迭代器值。这样的值称为**超出末端**值。

对于表达式`*i`有定义的迭代器`i`的值称为**可解引用**的。标准库从不假定超出末端值是可解引用的。

迭代器也可以具有**奇异**值，即不与任何序列关联的值。大多数表达式对奇异值的结果未定义；唯一的例外是：

1. 将非奇异值赋给持有奇异值的迭代器
2. 销毁持有奇异值的迭代器
3. 对于满足DefaultConstructible要求的迭代器，使用值初始化的迭代器作为复制或移动（C++11起）操作的源

在这些情况下，奇异值以与其他值相同的方式被覆盖。可解引用值总是非奇异的。

**无效**迭代器是可能为奇异的迭代器。

### 三、范围（Range）

标准库中操作数据结构的大多数算法模板都有使用范围的接口。

#### C++20前的范围定义
- 迭代器`j`称为从迭代器`i`**可达**当且仅当存在有限次应用表达式`++i`的序列使得`i == j`
- **范围**是一对指定计算开始和结束的迭代器
- 范围`[i,i)`是空范围；一般而言，范围`[i,j)`指的是从`i`指向的元素开始到`j`指向的元素（不包括）的数据结构中的元素
- 范围`[i,j)`**有效**当且仅当`j`从`i`可达

#### C++20后的范围定义
范围可以由以下两种方式表示：
1. `[i,s)`，其中迭代器`i`和**哨兵**`s`指定计算的开始和结束（`i`和`s`可以是不同类型）
2. `i+[0,n)`，其中迭代器`i`和计数`n`指定开始和要应用计算的元素数量

**迭代器-哨兵范围**：迭代器和哨兵表示的范围是可比较的。`[i,s)`为空当`i == s`；否则`[i,s)`指的是从`i`指向的元素开始到第一个使`j == s`的迭代器`j`指向的元素（如果有）的数据结构中的元素。

**计数范围**：计数范围`i+[0,n)`为空当`n == 0`；否则`i+[0,n)`指的是从`i`指向的元素开始的`n`个元素。

范围应用函数的结果对无效范围未定义。

---

## 扩展知识详解

### 一、迭代器概念深度解析

#### 1. 现代迭代器概念示例
```cpp
#include <iterator>
#include <concepts>
#include <vector>
#include <iostream>

// 检查迭代器概念
template<std::input_iterator Iter>
void process_input_iterator(Iter first, Iter last) {
    for (; first != last; ++first) {
        std::cout << *first << " ";
    }
    std::cout << std::endl;
}

template<std::random_access_iterator Iter>
void process_random_access_iterator(Iter first, Iter last) {
    // 可以使用随机访问特性
    auto size = last - first;
    std::cout << "Size: " << size << std::endl;
    
    // 支持下标访问
    if (size > 0) {
        std::cout << "First element: " << first[0] << std::endl;
    }
}

void iterator_concepts_example() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 满足input_iterator概念
    process_input_iterator(vec.begin(), vec.end());
    
    // 满足random_access_iterator概念
    process_random_access_iterator(vec.begin(), vec.end());
}
```

#### 2. 自定义迭代器实现概念
```cpp
#include <iterator>
#include <concepts>

// 自定义迭代器类
template<typename T>
class CustomIterator {
private:
    T* ptr_;
    
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::random_access_iterator_tag;
    
    explicit CustomIterator(T* ptr) : ptr_(ptr) {}
    
    // 前置递增
    CustomIterator& operator++() {
        ++ptr_;
        return *this;
    }
    
    // 后置递增
    CustomIterator operator++(int) {
        CustomIterator temp = *this;
        ++(*this);
        return temp;
    }
    
    // 解引用
    reference operator*() const {
        return *ptr_;
    }
    
    pointer operator->() const {
        return ptr_;
    }
    
    // 相等比较
    bool operator==(const CustomIterator& other) const {
        return ptr_ == other.ptr_;
    }
    
    // 随机访问
    CustomIterator& operator+=(difference_type n) {
        ptr_ += n;
        return *this;
    }
    
    CustomIterator operator+(difference_type n) const {
        return CustomIterator(ptr_ + n);
    }
    
    difference_type operator-(const CustomIterator& other) const {
        return ptr_ - other.ptr_;
    }
};

// 验证自定义迭代器满足概念
static_assert(std::random_access_iterator<CustomIterator<int>>);
```

### 二、迭代器适配器详解

#### 1. 反向迭代器
```cpp
#include <iterator>
#include <vector>
#include <iostream>

void reverse_iterator_example() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 使用反向迭代器
    std::cout << "Forward: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 反向遍历
    std::cout << "Reverse: ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 手动创建反向迭代器
    auto r_it = std::make_reverse_iterator(vec.end());
    auto r_end = std::make_reverse_iterator(vec.begin());
    
    std::cout << "Manual reverse: ";
    for (; r_it != r_end; ++r_it) {
        std::cout << *r_it << " ";
    }
    std::cout << std::endl;
}
```

#### 2. 插入迭代器
```cpp
#include <iterator>
#include <vector>
#include <list>
#include <iostream>

void insert_iterator_example() {
    std::vector<int> source = {1, 2, 3, 4, 5};
    std::list<int> target;
    
    // 后插入迭代器
    std::copy(source.begin(), source.end(), std::back_inserter(target));
    
    std::cout << "After back_inserter: ";
    for (const auto& elem : target) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // 前插入迭代器
    std::vector<int> target2;
    std::copy(source.begin(), source.end(), std::front_inserter(target2));
    
    std::cout << "After front_inserter: ";
    for (const auto& elem : target2) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // 插入迭代器
    std::vector<int> target3 = {10, 20};
    auto insert_it = std::inserter(target3, target3.begin() + 1);
    std::copy(source.begin(), source.begin() + 2, insert_it);
    
    std::cout << "After inserter: ";
    for (const auto& elem : target3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
```

#### 3. 移动迭代器（C++11）
```cpp
#include <iterator>
#include <vector>
#include <memory>
#include <iostream>

void move_iterator_example() {
    std::vector<std::unique_ptr<int>> vec;
    vec.push_back(std::make_unique<int>(1));
    vec.push_back(std::make_unique<int>(2));
    vec.push_back(std::make_unique<int>(3));
    
    std::cout << "Before move: " << vec.size() << " elements" << std::endl;
    
    std::vector<std::unique_ptr<int>> target;
    // 使用移动迭代器避免不必要的复制
    std::copy(std::make_move_iterator(vec.begin()),
              std::make_move_iterator(vec.end()),
              std::back_inserter(target));
    
    std::cout << "After move - Source: " << vec.size() << " elements" << std::endl;
    std::cout << "After move - Target: " << target.size() << " elements" << std::endl;
    
    for (const auto& ptr : target) {
        if (ptr) {
            std::cout << *ptr << " ";
        }
    }
    std::cout << std::endl;
}
```

### 三、范围访问函数

#### 1. 现代范围访问
```cpp
#include <iterator>
#include <vector>
#include <array>
#include <string>
#include <iostream>

void range_access_example() {
    // 容器
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "Vector size: " << std::size(vec) << std::endl;
    std::cout << "Vector empty: " << std::empty(vec) << std::endl;
    
    // 数组
    int arr[] = {10, 20, 30};
    std::cout << "Array size: " << std::size(arr) << std::endl;
    
    // 字符串
    std::string str = "Hello";
    std::cout << "String size: " << std::size(str) << std::endl;
    std::cout << "String data: " << std::data(str) << std::endl;
    
    // 使用begin/end
    std::cout << "Vector elements: ";
    for (auto it = std::begin(vec); it != std::end(vec); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 常量begin/end
    std::cout << "Const vector elements: ";
    for (auto it = std::cbegin(vec); it != std::cend(vec); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
```

### 四、迭代器操作函数

#### 1. advance和distance函数
```cpp
#include <iterator>
#include <vector>
#include <list>
#include <iostream>

void iterator_operations_example() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::list<int> lst = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 对随机访问迭代器使用advance
    auto vec_it = vec.begin();
    std::advance(vec_it, 5);
    std::cout << "Vector element at position 5: " << *vec_it << std::endl;
    
    // 对双向迭代器使用advance
    auto lst_it = lst.begin();
    std::advance(lst_it, 3);
    std::cout << "List element at position 3: " << *lst_it << std::endl;
    
    // 计算距离
    auto dist1 = std::distance(vec.begin(), vec_it);
    auto dist2 = std::distance(lst.begin(), lst_it);
    std::cout << "Distance in vector: " << dist1 << std::endl;
    std::cout << "Distance in list: " << dist2 << std::endl;
    
    // 使用next和prev
    auto next_it = std::next(vec.begin(), 3);
    auto prev_it = std::prev(vec.end(), 2);
    std::cout << "Next element: " << *next_it << std::endl;
    std::cout << "Prev element: " << *prev_it << std::endl;
}
```

#### 2. 范围算法操作（C++20）
```cpp
#include <iterator>
#include <vector>
#include <iostream>

void ranges_iterator_operations_example() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用ranges::advance
    auto it = vec.begin();
    std::ranges::advance(it, 3);
    std::cout << "Element after advance: " << *it << std::endl;
    
    // 使用ranges::distance
    auto dist = std::ranges::distance(vec.begin(), it);
    std::cout << "Range distance: " << dist << std::endl;
    
    // 使用ranges::next
    auto next_it = std::ranges::next(vec.begin(), 5);
    std::cout << "Element using ranges::next: " << *next_it << std::endl;
    
    // 使用ranges::prev
    auto prev_it = std::ranges::prev(vec.end(), 3);
    std::cout << "Element using ranges::prev: " << *prev_it << std::endl;
}
```

### 五、算法概念和工具

#### 1. 间接可调用概念
```cpp
#include <iterator>
#include <concepts>
#include <vector>
#include <algorithm>
#include <functional>

// 满足间接一元可调用概念的函数对象
struct PrintFunction {
    template<std::indirectly_readable Iter>
    void operator()(Iter it) const {
        std::cout << *it << " ";
    }
};

// 满足间接一元谓词概念
struct EvenPredicate {
    template<std::indirectly_readable Iter>
    bool operator()(Iter it) const {
        return *it % 2 == 0;
    }
};

void indirect_callable_example() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8};
    
    // 使用间接可调用对象
    PrintFunction print_func;
    std::cout << "All elements: ";
    std::for_each(vec.begin(), vec.end(), print_func);
    std::cout << std::endl;
    
    // 使用间接谓词
    EvenPredicate even_pred;
    auto count = std::count_if(vec.begin(), vec.end(), even_pred);
    std::cout << "Even elements count: " << count << std::endl;
}
```

#### 2. 间接可复制和可移动概念
```cpp
#include <iterator>
#include <vector>
#include <memory>

void indirect_copy_move_example() {
    std::vector<std::unique_ptr<int>> source;
    source.push_back(std::make_unique<int>(1));
    source.push_back(std::make_unique<int>(2));
    source.push_back(std::make_unique<int>(3));
    
    // 间接可移动
    std::vector<std::unique_ptr<int>> target;
    std::move(source.begin(), source.end(), std::back_inserter(target));
    
    // 间接可复制（对支持复制的类型）
    std::vector<int> int_source = {1, 2, 3, 4, 5};
    std::vector<int> int_target;
    std::copy(int_source.begin(), int_source.end(), std::back_inserter(int_target));
}
```

### 六、哨兵和计数迭代器（C++20）

#### 1. 默认哨兵和不可达哨兵
```cpp
#include <iterator>
#include <iostream>

void sentinel_example() {
    // 默认哨兵 - 用于知道范围边界的迭代器
    std::default_sentinel_t default_sentinel;
    
    // 不可达哨兵 - 总是与任何可递增类型不相等
    std::unreachable_sentinel_t unreachable_sentinel;
    
    std::cout << "Sentinel examples demonstrated" << std::endl;
}

// 计数迭代器示例
void counted_iterator_example() {
    int arr[] = {1, 2, 3, 4, 5};
    
    // 创建计数迭代器
    std::counted_iterator<int*> count_it(arr, 5);
    std::default_sentinel_t sentinel;
    
    std::cout << "Counted iterator elements: ";
    while (count_it != sentinel) {
        std::cout << *count_it << " ";
        ++count_it;
    }
    std::cout << std::endl;
}
```

### 七、流迭代器

#### 1. 输入输出流迭代器
```cpp
#include <iterator>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

void stream_iterator_example() {
    // 输出流迭代器
    std::ostringstream oss;
    std::ostream_iterator<int> out_it(oss, " ");
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::copy(vec.begin(), vec.end(), out_it);
    
    std::cout << "Stream output: " << oss.str() << std::endl;
    
    // 输入流迭代器
    std::istringstream iss("10 20 30 40 50");
    std::istream_iterator<int> in_it(iss);
    std::istream_iterator<int> eos;  // end-of-stream iterator
    
    std::vector<int> input_vec;
    std::copy(in_it, eos, std::back_inserter(input_vec));
    
    std::cout << "Stream input: ";
    for (const auto& elem : input_vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
```

### 八、实际应用示例

#### 1. 自定义容器迭代器
```cpp
#include <iterator>
#include <memory>

template<typename T>
class SimpleVector {
private:
    std::unique_ptr<T[]> data_;
    std::size_t size_;
    std::size_t capacity_;
    
public:
    explicit SimpleVector(std::size_t cap = 10) 
        : data_(std::make_unique<T[]>(cap)), size_(0), capacity_(cap) {}
    
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            resize();
        }
        data_[size_++] = value;
    }
    
    std::size_t size() const { return size_; }
    
    // 迭代器类
    class iterator {
    private:
        T* ptr_;
        
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        explicit iterator(T* ptr) : ptr_(ptr) {}
        
        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }
        
        iterator& operator++() { ++ptr_; return *this; }
        iterator operator++(int) { iterator temp = *this; ++(*this); return temp; }
        iterator& operator--() { --ptr_; return *this; }
        iterator operator--(int) { iterator temp = *this; --(*this); return temp; }
        
        iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
        iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
        
        iterator operator+(difference_type n) const { return iterator(ptr_ + n); }
        iterator operator-(difference_type n) const { return iterator(ptr_ - n); }
        
        difference_type operator-(const iterator& other) const { return ptr_ - other.ptr_; }
        
        bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
        bool operator<(const iterator& other) const { return ptr_ < other.ptr_; }
        bool operator<=(const iterator& other) const { return ptr_ <= other.ptr_; }
        bool operator>(const iterator& other) const { return ptr_ > other.ptr_; }
        bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }
        
        reference operator[](difference_type n) const { return ptr_[n]; }
    };
    
    iterator begin() { return iterator(data_.get()); }
    iterator end() { return iterator(data_.get() + size_); }
    iterator begin() const { return iterator(data_.get()); }
    iterator end() const { return iterator(data_.get() + size_); }
    
private:
    void resize() {
        std::size_t new_capacity = capacity_ * 2;
        auto new_data = std::make_unique<T[]>(new_capacity);
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        data_ = std::move(new_data);
        capacity_ = new_capacity;
    }
};

void custom_container_example() {
    SimpleVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    
    // 使用自定义迭代器
    std::cout << "Custom vector elements: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // 随机访问
    auto it = vec.begin();
    std::advance(it, 2);
    std::cout << "Element at index 2: " << *it << std::endl;
}
```

#### 2. 算法约束示例
```cpp
#include <iterator>
#include <concepts>
#include <vector>
#include <algorithm>
#include <iostream>

// 约束的算法模板
template<std::sortable Iter>
void sort_range(Iter first, Iter last) {
    std::sort(first, last);
}

template<std::permutable Iter>
void reverse_range(Iter first, Iter last) {
    std::reverse(first, last);
}

template<std::indirectly_comparable<Iter, std::equal_to<>> Iter>
std::size_t count_value(Iter first, Iter last, const typename std::iter_value_t<Iter>& value) {
    return std::count(first, last, value);
}

void constrained_algorithms_example() {
    std::vector<int> vec = {5, 2, 8, 1, 9, 3};
    
    std::cout << "Original: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // 排序
    sort_range(vec.begin(), vec.end());
    std::cout << "Sorted: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // 反转
    reverse_range(vec.begin(), vec.end());
    std::cout << "Reversed: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // 计数
    auto count = count_value(vec.begin(), vec.end(), 5);
    std::cout << "Count of 5: " << count << std::endl;
}
```

## 缺陷报告

以下行为改变的缺陷报告已追溯应用于先前发布的C++标准：

| 缺陷报告 | 应用于 | 原行为 | 正确行为 |
|----------|--------|--------|----------|
| CWG 1181 | C++98 | 数组类型不能作为值类型 | 数组类型可以作为值类型 |
| LWG 208 | C++98 | 超出末端迭代器总是非奇异的 | 超出末端迭代器可以是奇异的 |
| LWG 278 | C++98 | 迭代器的有效性未定义 | 定义为总是非奇异的 |

## 相关页面

| 页面 | 说明 |
|------|------|
| 范围库（C++20起） | 范围相关功能 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/iterator&oldid=179898>
- 最后修改时间：2025年1月28日 10:31
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++迭代器库是现代C++编程的核心组件，提供了强大的迭代器抽象和操作能力。从传统的迭代器类别到现代的概念系统，迭代器库不断发展以满足现代应用需求。理解迭代器概念、掌握各种迭代器适配器、合理使用范围访问函数，是编写高效C++程序的关键。正确处理迭代器有效性、优化迭代器操作、实现自定义迭代器，能够构建更加健壮和灵活的应用系统。持续学习和实践迭代器库的新特性，特别是C++20引入的概念系统和哨兵机制，是保持技术前沿的必要途径。迭代器库的成功应用将显著提高C++程序的数据处理能力和算法实现效率。