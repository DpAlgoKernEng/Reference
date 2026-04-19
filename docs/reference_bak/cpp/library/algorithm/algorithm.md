# C++算法库（Algorithm Library）

来源：cppreference.com

## 算法库概述

算法库定义了用于各种目的（如搜索、排序、计数、操作）的函数，这些函数对元素范围进行操作。注意，范围被定义为`[first, last)`，其中`last`指的是要检查或修改的最后一个元素**之后**的元素。

## 主要功能分类

### 一、约束算法和范围算法（C++20起）

C++20在`std::ranges`命名空间中提供了大多数算法的约束版本。在这些算法中，范围可以指定为迭代器-哨兵对或单个`range`参数，并且支持投影和成员函数指针可调用对象。此外，大多数算法的返回类型已更改为返回算法执行期间计算出的所有潜在有用信息。

```cpp
std::vector<int> v {7, 1, 4, 0, -1};
std::ranges::sort(v); // 约束算法
```

### 二、执行策略（C++17起）

大多数算法都有接受执行策略的重载。标准库算法支持几种执行策略，库提供相应的执行策略类型和对象。用户可以通过使用相应类型的执行策略对象调用并行算法来静态选择执行策略。

| 头文件 | 组件 | 说明 |
|--------|------|------|
| `<execution>` | `std::execution`命名空间 | 执行策略类型和对象 |
| `<execution>` | `sequenced_policy`/`parallel_policy`/`parallel_unsequenced_policy`/`unsequenced_policy`(C++17)(C++17)(C++17)(C++20) | 执行策略类型（类） |
| `<execution>` | `seq`/`par`/`par_unseq`/`unseq`(C++17)(C++17)(C++17)(C++20) | 全局执行策略对象（常量） |
| `<execution>` | `is_execution_policy`(C++17) | 测试类是否表示执行策略（类模板） |

### 三、非修改序列操作

#### 批量操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `for_each` | 对元素范围应用函数（函数模板） |
| `<algorithm>` | `ranges::for_each`(C++20) | 对元素范围应用函数（算法函数对象） |
| `<algorithm>` | `for_each_n`(C++17) | 对序列的前N个元素应用函数对象（函数模板） |
| `<algorithm>` | `ranges::for_each_n`(C++20) | 对序列的前N个元素应用函数对象（算法函数对象） |

#### 搜索操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `all_of`/`any_of`/`none_of`(C++11)(C++11)(C++11) | 检查谓词是否对范围内的所有、任何或无元素为真（函数模板） |
| `<algorithm>` | `ranges::all_of`/`ranges::any_of`/`ranges::none_of`(C++20)(C++20)(C++20) | 检查谓词是否对范围内的所有、任何或无元素为真（算法函数对象） |
| `<algorithm>` | `find`/`find_if`/`find_if_not`(C++11) | 查找满足特定条件的第一个元素（函数模板） |
| `<algorithm>` | `ranges::find`/`ranges::find_if`/`ranges::find_if_not`(C++20)(C++20)(C++20) | 查找满足特定条件的第一个元素（算法函数对象） |
| `<algorithm>` | `count`/`count_if` | 返回满足特定条件的元素数量（函数模板） |
| `<algorithm>` | `ranges::count`/`ranges::count_if`(C++20)(C++20) | 返回满足特定条件的元素数量（算法函数对象） |
| `<algorithm>` | `mismatch` | 查找两个范围不同的第一个位置（函数模板） |
| `<algorithm>` | `ranges::mismatch`(C++20) | 查找两个范围不同的第一个位置（算法函数对象） |
| `<algorithm>` | `equal` | 确定两组元素是否相同（函数模板） |
| `<algorithm>` | `ranges::equal`(C++20) | 确定两组元素是否相同（算法函数对象） |

### 四、修改序列操作

#### 复制操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `copy`/`copy_if`(C++11) | 将元素范围复制到新位置（函数模板） |
| `<algorithm>` | `ranges::copy`/`ranges::copy_if`(C++20)(C++20) | 将元素范围复制到新位置（算法函数对象） |
| `<algorithm>` | `copy_n`(C++11) | 将指定数量的元素复制到新位置（函数模板） |
| `<algorithm>` | `ranges::copy_n`(C++20) | 将指定数量的元素复制到新位置（算法函数对象） |
| `<algorithm>` | `move`(C++11) | 将元素范围移动到新位置（函数模板） |
| `<algorithm>` | `ranges::move`(C++20) | 将元素范围移动到新位置（算法函数对象） |

#### 交换操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<utility>`(C++11) | `swap` | 交换两个对象的值（函数模板） |
| `<algorithm>` | `swap_ranges` | 交换两个元素范围（函数模板） |
| `<algorithm>` | `ranges::swap_ranges`(C++20) | 交换两个元素范围（算法函数对象） |
| `<algorithm>` | `iter_swap` | 交换两个迭代器指向的元素（函数模板） |

#### 转换操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `transform` | 对元素范围应用函数，将结果存储在目标范围（函数模板） |
| `<algorithm>` | `ranges::transform`(C++20) | 对元素范围应用函数（算法函数对象） |
| `<algorithm>` | `replace`/`replace_if` | 用另一个值替换满足特定条件的所有值（函数模板） |
| `<algorithm>` | `ranges::replace`/`ranges::replace_if`(C++20)(C++20) | 用另一个值替换满足特定条件的所有值（算法函数对象） |

#### 生成操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `fill` | 将给定值复制赋值给范围内的每个元素（函数模板） |
| `<algorithm>` | `ranges::fill`(C++20) | 为元素范围分配特定值（算法函数对象） |
| `<algorithm>` | `generate` | 将连续函数调用的结果赋值给范围内的每个元素（函数模板） |
| `<algorithm>` | `ranges::generate`(C++20) | 在范围中保存函数结果（算法函数对象） |

#### 删除操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `remove`/`remove_if` | 删除满足特定条件的元素（函数模板） |
| `<algorithm>` | `ranges::remove`/`ranges::remove_if`(C++20)(C++20) | 删除满足特定条件的元素（算法函数对象） |
| `<algorithm>` | `unique` | 删除范围内的连续重复元素（函数模板） |
| `<algorithm>` | `ranges::unique`(C++20) | 删除范围内的连续重复元素（算法函数对象） |

#### 顺序改变操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `reverse` | 反转范围内的元素顺序（函数模板） |
| `<algorithm>` | `ranges::reverse`(C++20) | 反转范围内的元素顺序（算法函数对象） |
| `<algorithm>` | `rotate` | 旋转范围内的元素顺序（函数模板） |
| `<algorithm>` | `ranges::rotate`(C++20) | 旋转范围内的元素顺序（算法函数对象） |
| `<algorithm>` | `shuffle`(C++11) | 随机重新排列范围内的元素（函数模板） |
| `<algorithm>` | `ranges::shuffle`(C++20) | 随机重新排列范围内的元素（算法函数对象） |

### 五、排序和相关操作

#### 排序操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `sort` | 将范围排序为升序（函数模板） |
| `<algorithm>` | `ranges::sort`(C++20) | 将范围排序为升序（算法函数对象） |
| `<algorithm>` | `stable_sort` | 对元素范围排序，同时保持相等元素间的顺序（函数模板） |
| `<algorithm>` | `ranges::stable_sort`(C++20) | 对元素范围排序，同时保持相等元素间的顺序（算法函数对象） |

#### 二分搜索操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `lower_bound` | 返回指向不小于给定值的第一个元素的迭代器（函数模板） |
| `<algorithm>` | `ranges::lower_bound`(C++20) | 返回指向不小于给定值的第一个元素的迭代器（算法函数对象） |
| `<algorithm>` | `binary_search` | 确定部分有序范围内是否存在元素（函数模板） |
| `<algorithm>` | `ranges::binary_search`(C++20) | 确定部分有序范围内是否存在元素（算法函数对象） |

#### 集合操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<algorithm>` | `includes` | 如果一个序列是另一个序列的子序列则返回true（函数模板） |
| `<algorithm>` | `ranges::includes`(C++20) | 如果一个序列是另一个序列的子序列则返回true（算法函数对象） |
| `<algorithm>` | `set_union` | 计算两个集合的并集（函数模板） |
| `<algorithm>` | `ranges::set_union`(C++20) | 计算两个集合的并集（算法函数对象） |

### 六、数值操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<numeric>` | `iota`(C++11) | 用起始值的连续增量填充范围（函数模板） |
| `<numeric>` | `accumulate` | 求和或折叠元素范围（函数模板） |
| `<numeric>` | `reduce`(C++17) | 类似于std::accumulate，但不按顺序（函数模板） |
| `<numeric>` | `partial_sum` | 计算元素范围的部分和（函数模板） |

### 七、未初始化内存操作

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<memory>` | `uninitialized_copy` | 将对象范围复制到未初始化的内存区域（函数模板） |
| `<memory>` | `ranges::uninitialized_copy`(C++20) | 将对象范围复制到未初始化的内存区域（算法函数对象） |
| `<memory>` | `uninitialized_fill` | 将对象复制到由范围定义的未初始化内存区域（函数模板） |
| `<memory>` | `ranges::uninitialized_fill`(C++20) | 将对象复制到由范围定义的未初始化内存区域（算法函数对象） |
| `<memory>` | `uninitialized_move`(C++17) | 将对象范围移动到未初始化的内存区域（函数模板） |
| `<memory>` | `ranges::uninitialized_move`(C++20) | 将对象范围移动到未初始化的内存区域（算法函数对象） |

---

## 扩展知识详解

### 一、现代算法库演进

#### 1. 基础算法使用（C++98/03）
```cpp
#include <algorithm>
#include <vector>
#include <iostream>

void basic_algorithms() {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 排序
    std::sort(vec.begin(), vec.end());
    
    // 查找
    auto it = std::find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        std::cout << "Found 5 at position: " << (it - vec.begin()) << std::endl;
    }
    
    // 计数
    int count = std::count(vec.begin(), vec.end(), 1);
    std::cout << "Count of 1: " << count << std::endl;
    
    // 遍历
    std::for_each(vec.begin(), vec.end(), [](int n) {
        std::cout << n << " ";
    });
    std::cout << std::endl;
}
```

#### 2. 现代改进（C++11/14）
```cpp
#include <algorithm>
#include <vector>
#include <numeric>
#include <functional>

void modern_algorithms() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 移动操作
    std::vector<int> dest(vec.size());
    std::move(vec.begin(), vec.end(), dest.begin());
    
    // 条件操作
    auto it = std::find_if(dest.begin(), dest.end(), 
                          [](int n) { return n > 3; });
    
    // 变换操作
    std::vector<int> squared(dest.size());
    std::transform(dest.begin(), dest.end(), squared.begin(),
                   [](int n) { return n * n; });
    
    // 数值操作
    int sum = std::accumulate(squared.begin(), squared.end(), 0);
    std::cout << "Sum: " << sum << std::endl;
}
```

#### 3. 范围算法（C++20）
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <iostream>

void ranges_algorithms() {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 范围排序
    std::ranges::sort(vec);
    
    // 范围查找
    auto it = std::ranges::find(vec, 5);
    if (it != vec.end()) {
        std::cout << "Found 5" << std::endl;
    }
    
    // 范围计数
    auto count = std::ranges::count(vec, 1);
    std::cout << "Count of 1: " << count << std::endl;
    
    // 管道操作
    auto even_count = vec | std::views::filter([](int n) { return n % 2 == 0; })
                          | std::ranges::distance;
    std::cout << "Even numbers: " << even_count << std::endl;
}
```

### 二、关键算法详解

#### 1. 排序算法家族
```cpp
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>

void sorting_algorithms() {
    std::vector<int> vec(10000);
    std::iota(vec.begin(), vec.end(), 1);
    
    // 随机打乱
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);
    
    // 基本排序
    std::vector<int> sorted = vec;
    std::sort(sorted.begin(), sorted.end());
    
    // 稳定排序
    std::vector<std::pair<int, int>> pairs = {{3, 1}, {1, 2}, {3, 3}, {2, 4}};
    std::stable_sort(pairs.begin(), pairs.end(), 
                     [](const auto& a, const auto& b) { return a.first < b.first; });
    
    // 部分排序
    std::vector<int> partial = vec;
    std::partial_sort(partial.begin(), partial.begin() + 10, partial.end());
    
    // 第N个元素
    std::vector<int> nth = vec;
    std::nth_element(nth.begin(), nth.begin() + 5000, nth.end());
    std::cout << "Median: " << nth[5000] << std::endl;
}
```

#### 2. 集合操作
```cpp
#include <algorithm>
#include <vector>
#include <iostream>

void set_operations() {
    std::vector<int> set1 = {1, 2, 3, 4, 5};
    std::vector<int> set2 = {3, 4, 5, 6, 7};
    
    // 并集
    std::vector<int> union_result;
    std::set_union(set1.begin(), set1.end(),
                   set2.begin(), set2.end(),
                   std::back_inserter(union_result));
    
    // 交集
    std::vector<int> intersection_result;
    std::set_intersection(set1.begin(), set1.end(),
                         set2.begin(), set2.end(),
                         std::back_inserter(intersection_result));
    
    // 差集
    std::vector<int> difference_result;
    std::set_difference(set1.begin(), set1.end(),
                       set2.begin(), set2.end(),
                       std::back_inserter(difference_result));
    
    // 对称差集
    std::vector<int> sym_diff_result;
    std::set_symmetric_difference(set1.begin(), set1.end(),
                                 set2.begin(), set2.end(),
                                 std::back_inserter(sym_diff_result));
}
```

#### 3. 堆操作
```cpp
#include <algorithm>
#include <vector>
#include <iostream>

void heap_operations() {
    std::vector<int> heap = {1, 2, 3, 4, 5, 6, 7};
    
    // 创建堆
    std::make_heap(heap.begin(), heap.end());
    std::cout << "Max element: " << heap.front() << std::endl;
    
    // 向堆中添加元素
    heap.push_back(8);
    std::push_heap(heap.begin(), heap.end());
    
    // 从堆中移除最大元素
    std::pop_heap(heap.begin(), heap.end());
    int max_val = heap.back();
    heap.pop_back();
    std::cout << "Popped max: " << max_val << std::endl;
    
    // 堆排序
    std::sort_heap(heap.begin(), heap.end());
}
```

### 三、并行算法（C++17）

```cpp
#include <algorithm>
#include <execution>
#include <vector>
#include <numeric>

void parallel_algorithms() {
    std::vector<int> vec(1000000);
    std::iota(vec.begin(), vec.end(), 1);
    
    // 顺序执行
    auto sum_seq = std::accumulate(vec.begin(), vec.end(), 0LL);
    
    // 并行执行
    auto sum_par = std::reduce(std::execution::par, vec.begin(), vec.end(), 0LL);
    
    // 并行变换归约
    auto result = std::transform_reduce(
        std::execution::par,
        vec.begin(), vec.end(),
        0LL,
        std::plus<>{},
        [](int n) { return static_cast<long long>(n) * n; }
    );
}
```

### 四、实际应用示例

#### 1. 数据处理管道
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <string>

class DataProcessor {
public:
    // 使用范围算法的数据处理管道
    static std::vector<std::string> process_data(const std::vector<int>& data) {
        return data 
            | std::views::filter([](int n) { return n > 0; })
            | std::views::transform([](int n) { return n * n; })
            | std::views::take(10)
            | std::views::transform([](int n) { return std::to_string(n); })
            | std::ranges::to<std::vector>();
    }
    
    // 传统算法版本
    static std::vector<std::string> process_data_traditional(const std::vector<int>& data) {
        std::vector<int> filtered;
        std::copy_if(data.begin(), data.end(), std::back_inserter(filtered),
                     [](int n) { return n > 0; });
        
        std::vector<int> transformed(filtered.size());
        std::transform(filtered.begin(), filtered.end(), transformed.begin(),
                       [](int n) { return n * n; });
        
        transformed.resize(std::min(transformed.size(), size_t(10)));
        
        std::vector<std::string> result(transformed.size());
        std::transform(transformed.begin(), transformed.end(), result.begin(),
                       [](int n) { return std::to_string(n); });
        
        return result;
    }
};
```

#### 2. 自定义比较器和投影
```cpp
#include <algorithm>
#include <ranges>
#include <vector>
#include <string>

struct Person {
    std::string name;
    int age;
    double salary;
};

void custom_comparisons() {
    std::vector<Person> people = {
        {"Alice", 30, 50000},
        {"Bob", 25, 60000},
        {"Charlie", 35, 45000}
    };
    
    // 按年龄排序（C++20范围算法）
    std::ranges::sort(people, {}, &Person::age);
    
    // 按姓名排序
    std::ranges::sort(people, {}, &Person::name);
    
    // 使用自定义比较器
    std::ranges::sort(people, 
                     [](const Person& a, const Person& b) { 
                         return a.salary > b.salary; 
                     });
    
    // 查找特定人员
    auto it = std::ranges::find(people, "Bob", &Person::name);
    if (it != people.end()) {
        std::cout << "Found: " << it->name << std::endl;
    }
}
```

#### 3. 数值算法应用
```cpp
#include <numeric>
#include <vector>
#include <iostream>
#include <functional>

void numerical_algorithms() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // 累积和
    std::vector<int> partial_sums;
    std::partial_sum(numbers.begin(), numbers.end(), 
                     std::back_inserter(partial_sums));
    
    // 相邻差分
    std::vector<int> differences;
    std::adjacent_difference(numbers.begin(), numbers.end(),
                           std::back_inserter(differences));
    
    // 内积
    std::vector<int> weights = {2, 3, 1, 4, 2};
    int dot_product = std::inner_product(numbers.begin(), numbers.end(),
                                       weights.begin(), 0);
    
    // 现代归约（C++17）
    long long sum = std::reduce(numbers.begin(), numbers.end(), 0LL);
    long long product = std::reduce(numbers.begin(), numbers.end(), 1LL,
                                  std::multiplies<>{});
}
```

#### 4. 内存管理算法
```cpp
#include <memory>
#include <algorithm>
#include <vector>

template<typename T>
class UninitializedVector {
private:
    std::allocator<T> alloc_;
    T* data_;
    size_t size_;
    size_t capacity_;
    
public:
    explicit UninitializedVector(size_t capacity) 
        : size_(0), capacity_(capacity) {
        data_ = alloc_.allocate(capacity_);
    }
    
    ~UninitializedVector() {
        std::destroy_n(data_, size_);
        alloc_.deallocate(data_, capacity_);
    }
    
    template<typename Iterator>
    void assign(Iterator first, Iterator last) {
        size_t count = std::distance(first, last);
        if (count > capacity_) {
            throw std::bad_alloc();
        }
        
        std::uninitialized_copy(first, last, data_);
        size_ = count;
    }
    
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            throw std::bad_alloc();
        }
        
        std::construct_at(data_ + size_, value);
        ++size_;
    }
};
```

## 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|------------|----|------|------|
| `__cpp_lib_parallel_algorithm` | `201603L` | (C++17) | 并行算法 |
| `__cpp_lib_execution` | `201603L` | (C++17) | 执行策略 |
| `__cpp_lib_constexpr_algorithms` | `201806L` | (C++20) | 算法的constexpr |
| `__cpp_lib_sample` | `201603L` | (C++17) | std::sample |

## C标准库对应

| 头文件 | 函数 | 说明 |
|--------|------|------|
| `<cstdlib>` | `qsort` | 对未指定类型的元素范围排序（函数） |
| `<cstdlib>` | `bsearch` | 在数组中搜索未指定类型的元素（函数） |

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C算法文档] | C语言对应文档 |

## 缺陷报告

以下行为变更的缺陷报告已追溯应用于先前发布的C++标准。

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/algorithm&oldid=179901>
- 最后修改时间：2025年1月28日 11:13
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++算法库是现代C++编程的核心组件，提供了从基本搜索排序到高级并行计算的全面功能。理解算法库的演进历程和各组件的使用方法，对于编写高效、安全的C++代码至关重要。从C++98的基础算法到C++20的范围算法，再到C++17的并行算法，算法库不断演进以满足现代软件开发的需求。正确使用排序、搜索、数值计算等关键算法，能够显著提高程序性能。掌握并行算法和范围算法等现代特性，可以让代码更加简洁和表达力强。自定义比较器、投影和执行策略等高级工具为处理复杂的数据结构提供了灵活的解决方案。算法库的设计遵循零开销抽象原则，确保了性能的同时提供了强大的功能。持续学习和实践算法库的新特性，是成为优秀C++开发者的重要途径。