# C++容器库（Containers Library）

来源：cppreference.com

## 容器库概述

容器库是一个通用的类模板和算法集合，允许程序员轻松实现常见的数据结构，如队列、列表和栈。容器分为以下几类：

1. **序列容器** - 可顺序访问的数据结构
2. **关联容器** - 可快速搜索的排序数据结构（O(log n)复杂度）
3. **无序关联容器**（C++11起）- 可快速搜索的无序（哈希）数据结构（平均O(1)，最坏O(n)复杂度）

容器管理为其元素分配的存储空间，并提供成员函数来访问它们，可以直接访问或通过迭代器（具有类似指针属性的对象）访问。

## 容器分类详解

### 一、序列容器（Sequence Containers）

实现可顺序访问的数据结构。

| 容器 | 说明 |
|------|------|
| `array`(C++11) | 固定大小的就地连续数组（类模板） |
| `vector` | 动态连续数组（类模板） |
| `inplace_vector`(C++26) | 动态可调整大小、固定容量的就地连续数组（类模板） |
| `deque` | 双端队列（类模板） |
| `forward_list`(C++11) | 单链表（类模板） |
| `list` | 双链表（类模板） |

### 二、关联容器（Associative Containers）

实现可快速搜索的排序数据结构。

| 容器 | 说明 |
|------|------|
| `set` | 按键排序的唯一键集合（类模板） |
| `map` | 按键排序的键值对集合，键唯一（类模板） |
| `multiset` | 按键排序的键集合（类模板） |
| `multimap` | 按键排序的键值对集合（类模板） |

### 三、无序关联容器（Unordered Associative Containers）（C++11起）

实现可快速搜索的无序（哈希）数据结构。

| 容器 | 说明 |
|------|------|
| `unordered_set`(C++11) | 按键哈希的唯一键集合（类模板） |
| `unordered_map`(C++11) | 按键哈希的键值对集合，键唯一（类模板） |
| `unordered_multiset`(C++11) | 按键哈希的键集合（类模板） |
| `unordered_multimap`(C++11) | 按键哈希的键值对集合（类模板） |

### 四、容器适配器（Container Adaptors）

为序列容器提供不同的接口。

| 容器 | 说明 |
|------|------|
| `stack` | 适配容器以提供栈（LIFO数据结构）（类模板） |
| `queue` | 适配容器以提供队列（FIFO数据结构）（类模板） |
| `priority_queue` | 适配容器以提供优先队列（类模板） |
| `flat_set`(C++23) | 适配容器以提供按键排序的唯一键集合（类模板） |
| `flat_map`(C++23) | 适配两个容器以提供按键排序的唯一键值对集合（类模板） |

### 五、视图（Views）（C++20起）

提供与元素的非拥有数组的一维或多维视图交互的灵活工具。

| 容器 | 说明 |
|------|------|
| `span`(C++20) | 连续对象序列的非拥有视图（类模板） |
| `mdspan`(C++23) | 多维非拥有数组视图（类模板） |

---

## 扩展知识详解

### 一、序列容器深度解析

#### 1. array（C++11）
```cpp
#include <array>
#include <iostream>

void array_examples() {
    // 固定大小数组
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    
    // 访问元素
    std::cout << "First element: " << arr[0] << std::endl;
    std::cout << "Last element: " << arr.at(4) << std::endl;
    
    // 容量信息
    std::cout << "Size: " << arr.size() << std::endl;
    std::cout << "Empty: " << arr.empty() << std::endl;
    
    // 迭代器
    for (const auto& element : arr) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}
```

#### 2. vector
```cpp
#include <vector>
#include <iostream>
#include <algorithm>

void vector_examples() {
    // 动态数组
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 添加元素
    vec.push_back(6);
    vec.emplace_back(7);
    
    // 插入元素
    vec.insert(vec.begin() + 2, 10);
    
    // 容量管理
    std::cout << "Size: " << vec.size() << std::endl;
    std::cout << "Capacity: " << vec.capacity() << std::endl;
    vec.shrink_to_fit();  // 释放多余内存
    
    // 访问元素
    std::cout << "Element at index 2: " << vec[2] << std::endl;
    
    // 算法操作
    std::sort(vec.begin(), vec.end());
    auto it = std::find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        std::cout << "Found 5 at position: " << (it - vec.begin()) << std::endl;
    }
}
```

#### 3. deque
```cpp
#include <deque>
#include <iostream>

void deque_examples() {
    std::deque<int> dq = {1, 2, 3, 4, 5};
    
    // 双端操作
    dq.push_front(0);    // 前端插入
    dq.push_back(6);     // 后端插入
    
    dq.pop_front();      // 前端删除
    dq.pop_back();       // 后端删除
    
    // 随机访问
    std::cout << "Element at index 2: " << dq[2] << std::endl;
    
    // 遍历
    for (const auto& element : dq) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}
```

#### 4. list和forward_list
```cpp
#include <list>
#include <forward_list>
#include <iostream>

void list_examples() {
    // 双链表
    std::list<int> lst = {1, 2, 3, 4, 5};
    
    // 插入和删除
    lst.push_front(0);
    lst.push_back(6);
    lst.insert(++lst.begin(), 10);
    
    lst.remove(3);  // 删除值为3的元素
    lst.unique();   // 删除连续重复元素
    
    // 排序
    lst.sort();
    
    // 合并
    std::list<int> lst2 = {7, 8, 9};
    lst.merge(lst2);
    
    // 单链表
    std::forward_list<int> flst = {1, 2, 3, 4, 5};
    flst.push_front(0);
    flst.insert_after(flst.begin(), 10);
}
```

### 二、关联容器深度解析

#### 1. set和multiset
```cpp
#include <set>
#include <iostream>

void set_examples() {
    // 集合 - 唯一键
    std::set<int> s = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 自动排序并去重
    for (const auto& element : s) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    
    // 插入和查找
    auto result = s.insert(7);
    if (result.second) {
        std::cout << "7 inserted successfully\n";
    }
    
    auto it = s.find(5);
    if (it != s.end()) {
        std::cout << "Found: " << *it << std::endl;
    }
    
    // 多重集合 - 允许重复键
    std::multiset<int> ms = {1, 2, 2, 3, 3, 3};
    std::cout << "Count of 2: " << ms.count(2) << std::endl;
    
    // 范围查找
    auto range = ms.equal_range(3);
    std::cout << "Elements with value 3: ";
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
```

#### 2. map和multimap
```cpp
#include <map>
#include <iostream>

void map_examples() {
    // 映射 - 键值对
    std::map<std::string, int> m = {
        {"apple", 5},
        {"banana", 3},
        {"orange", 8}
    };
    
    // 插入元素
    m["grape"] = 12;
    m.insert({"kiwi", 7});
    
    // 访问元素
    std::cout << "Apple count: " << m["apple"] << std::endl;
    
    // 安全访问
    auto it = m.find("banana");
    if (it != m.end()) {
        std::cout << "Banana count: " << it->second << std::endl;
    }
    
    // 遍历
    for (const auto& [key, value] : m) {
        std::cout << key << ": " << value << std::endl;
    }
    
    // 多重映射 - 允许重复键
    std::multimap<std::string, int> mm;
    mm.insert({"fruit", 1});
    mm.insert({"fruit", 2});
    mm.insert({"vegetable", 3});
    
    // 查找所有匹配项
    auto range = mm.equal_range("fruit");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << "Fruit: " << it->second << std::endl;
    }
}
```

### 三、无序关联容器（C++11）

#### 1. unordered_set和unordered_map
```cpp
#include <unordered_set>
#include <unordered_map>
#include <iostream>

void unordered_examples() {
    // 无序集合
    std::unordered_set<int> us = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 哈希存储，无序
    for (const auto& element : us) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Bucket count: " << us.bucket_count() << std::endl;
    std::cout << "Load factor: " << us.load_factor() << std::endl;
    
    // 无序映射
    std::unordered_map<std::string, int> um = {
        {"red", 1},
        {"green", 2},
        {"blue", 3}
    };
    
    // 平均O(1)查找
    auto it = um.find("green");
    if (it != um.end()) {
        std::cout << "Green value: " << it->second << std::endl;
    }
    
    // 桶操作
    for (std::size_t i = 0; i < um.bucket_count(); ++i) {
        std::cout << "Bucket " << i << ": ";
        for (auto it = um.begin(i); it != um.end(i); ++it) {
            std::cout << "[" << it->first << ":" << it->second << "] ";
        }
        std::cout << std::endl;
    }
}
```

### 四、容器适配器

#### 1. stack, queue, priority_queue
```cpp
#include <stack>
#include <queue>
#include <iostream>

void adaptor_examples() {
    // 栈 - LIFO
    std::stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    
    while (!st.empty()) {
        std::cout << "Stack top: " << st.top() << std::endl;
        st.pop();
    }
    
    // 队列 - FIFO
    std::queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    
    while (!q.empty()) {
        std::cout << "Queue front: " << q.front() << std::endl;
        std::cout << "Queue back: " << q.back() << std::endl;
        q.pop();
    }
    
    // 优先队列 - 最大堆
    std::priority_queue<int> pq;
    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(2);
    
    while (!pq.empty()) {
        std::cout << "Priority queue top: " << pq.top() << std::endl;
        pq.pop();
    }
}
```

#### 2. flat容器（C++23）
```cpp
#include <flat_set>
#include <flat_map>
#include <iostream>

void flat_examples() {
    // flat_set - 基于vector的有序集合
    std::flat_set<int> fs = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 保持排序，但基于连续内存
    for (const auto& element : fs) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    
    // flat_map - 基于vector的有序映射
    std::flat_map<std::string, int> fm = {
        {"alpha", 1},
        {"beta", 2},
        {"gamma", 3}
    };
    
    for (const auto& [key, value] : fm) {
        std::cout << key << ": " << value << std::endl;
    }
}
```

### 五、视图容器（C++20/23）

#### 1. span（C++20）
```cpp
#include <span>
#include <vector>
#include <iostream>

void span_examples() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 创建span视图
    std::span<int> sp(vec);
    
    // 子视图
    std::span<int> sub_sp = sp.subspan(2, 5);  // 从索引2开始的5个元素
    
    for (const auto& element : sub_sp) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    
    // 固定大小的span
    std::span<int, 5> fixed_sp(vec.data(), 5);
    std::cout << "Fixed span size: " << fixed_sp.size() << std::endl;
}
```

### 六、实际应用示例

#### 1. 性能比较和选择指南
```cpp
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <chrono>
#include <iostream>

class ContainerPerformanceTester {
public:
    static void test_insertion_performance() {
        const int elements = 100000;
        
        // vector插入性能
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<int> vec;
        for (int i = 0; i < elements; ++i) {
            vec.push_back(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto vec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // deque插入性能
        start = std::chrono::high_resolution_clock::now();
        std::deque<int> deq;
        for (int i = 0; i < elements; ++i) {
            deq.push_back(i);
        }
        end = std::chrono::high_resolution_clock::now();
        auto deq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // set插入性能
        start = std::chrono::high_resolution_clock::now();
        std::set<int> s;
        for (int i = 0; i < elements; ++i) {
            s.insert(i);
        }
        end = std::chrono::high_resolution_clock::now();
        auto set_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "Vector insertion time: " << vec_time.count() << "ms\n";
        std::cout << "Deque insertion time: " << deq_time.count() << "ms\n";
        std::cout << "Set insertion time: " << set_time.count() << "ms\n";
    }
    
    static void test_lookup_performance() {
        const int elements = 100000;
        std::vector<int> vec(elements);
        std::set<int> s;
        std::unordered_set<int> us;
        
        // 初始化数据
        for (int i = 0; i < elements; ++i) {
            vec[i] = i;
            s.insert(i);
            us.insert(i);
        }
        
        // vector查找（需要遍历）
        auto start = std::chrono::high_resolution_clock::now();
        auto it = std::find(vec.begin(), vec.end(), elements/2);
        auto end = std::chrono::high_resolution_clock::now();
        auto vec_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // set查找（O(log n)）
        start = std::chrono::high_resolution_clock::now();
        auto s_it = s.find(elements/2);
        end = std::chrono::high_resolution_clock::now();
        auto set_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // unordered_set查找（平均O(1)）
        start = std::chrono::high_resolution_clock::now();
        auto us_it = us.find(elements/2);
        end = std::chrono::high_resolution_clock::now();
        auto us_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "Vector lookup time: " << vec_time.count() << "μs\n";
        std::cout << "Set lookup time: " << set_time.count() << "μs\n";
        std::cout << "Unordered set lookup time: " << us_time.count() << "μs\n";
    }
};
```

#### 2. 容器选择决策框架
```cpp
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

// 容器选择指南
class ContainerSelector {
public:
    // 根据使用场景选择合适的容器
    enum class UsagePattern {
        RANDOM_ACCESS,      // 需要随机访问
        FREQUENT_INSERTION, // 频繁插入删除
        SORTED_DATA,        // 需要排序数据
        UNIQUE_KEYS,        // 唯一键查找
        FAST_LOOKUP         // 快速查找
    };
    
    template<UsagePattern pattern>
    struct container_choice;
    
    template<>
    struct container_choice<UsagePattern::RANDOM_ACCESS> {
        using type = std::vector<int>;  // 连续内存，快速随机访问
    };
    
    template<>
    struct container_choice<UsagePattern::FREQUENT_INSERTION> {
        using type = std::deque<int>;   // 双端高效插入删除
    };
    
    template<>
    struct container_choice<UsagePattern::SORTED_DATA> {
        using type = std::set<int>;     // 自动排序
    };
    
    template<>
    struct container_choice<UsagePattern::UNIQUE_KEYS> {
        using type = std::map<std::string, int>;  // 键值对映射
    };
    
    template<>
    struct container_choice<UsagePattern::FAST_LOOKUP> {
        using type = std::unordered_set<int>;     // 平均O(1)查找
    };
    
    // 使用示例
    static void demonstrate_selection() {
        typename container_choice<UsagePattern::RANDOM_ACCESS>::type data_structure;
        // 根据模式自动选择合适的容器类型
    }
};
```

#### 3. 容器包装器和工具类
```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>

template<typename T>
class SafeVector {
private:
    std::vector<T> vec_;
    
public:
    // 安全访问接口
    T& at(std::size_t index) {
        if (index >= vec_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return vec_[index];
    }
    
    const T& at(std::size_t index) const {
        if (index >= vec_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return vec_[index];
    }
    
    // 自动扩容
    void push_back(const T& value) {
        vec_.push_back(value);
    }
    
    // 范围检查插入
    typename std::vector<T>::iterator insert(std::size_t index, const T& value) {
        if (index > vec_.size()) {
            throw std::out_of_range("Insert index out of range");
        }
        return vec_.insert(vec_.begin() + index, value);
    }
    
    // 批量操作
    template<typename Iterator>
    void append(Iterator first, Iterator last) {
        vec_.insert(vec_.end(), first, last);
    }
    
    // 统计信息
    std::size_t size() const { return vec_.size(); }
    bool empty() const { return vec_.empty(); }
    void clear() { vec_.clear(); }
    
    // 迭代器接口
    auto begin() { return vec_.begin(); }
    auto end() { return vec_.end(); }
    auto begin() const { return vec_.begin(); }
    auto end() const { return vec_.end(); }
    auto cbegin() const { return vec_.cbegin(); }
    auto cend() const { return vec_.cend(); }
};
```

## 迭代器失效规则

### 一、基本规则

只读方法从不使迭代器或引用失效。修改容器内容的方法可能会使迭代器和/或引用失效。

### 二、各类容器的迭代器失效情况

| 容器类别 | 容器 | 插入后迭代器有效？ | 插入后引用有效？ | 删除后迭代器有效？ | 删除后引用有效？ |
|----------|------|-------------------|------------------|-------------------|------------------|
| 序列容器 | vector | 否（容量改变时） | 是（容量改变时） | 是（删除元素前） | 是（删除元素前） |
| 序列容器 | deque | 否（修改首尾元素） | 是 | 是（除删除元素） | 是（除删除元素） |
| 序列容器 | list | 是 | 是 | 是（除删除元素） | 是（除删除元素） |
| 关联容器 | set/map/multiset/multimap | 是 | 是 | 是（除删除元素） | 是（除删除元素） |
| 无序容器 | unordered_set/unordered_map等 | 否（重新哈希时） | 是 | 是（除删除元素） | 是（除删除元素） |

### 三、特殊说明

1. **past-the-end迭代器**：通常按正常迭代器规则失效
2. **deque特殊情况**：删除最后一个元素会使past-the-end迭代器失效
3. **clear操作**：使所有迭代器和引用失效

## 线程安全性

### C++11标准的线程安全保证

1. **不同容器的并发访问**：不同容器可以被不同线程同时调用
2. **const成员函数并发访问**：同一容器的const成员函数可以被不同线程同时调用
3. **元素级并发修改**：同一容器的不同元素可以被不同线程同时修改（vector<bool>除外）
4. **迭代器操作**：迭代器操作可以与const成员函数和其他迭代器操作并发执行
5. **容器操作并发性**：只要不改变用户可见结果，容器操作可以内部并行化

## 成员函数表（部分示例）

### vector常用成员函数
- 构造函数：`vector()`, `vector(size_type n)`, `vector(InputIterator first, InputIterator last)`
- 析构函数：`~vector()`
- 赋值：`operator=`, `assign()`
- 迭代器：`begin()`, `end()`, `rbegin()`, `rend()`
- 元素访问：`at()`, `operator[]`, `front()`, `back()`, `data()`
- 容量：`empty()`, `size()`, `max_size()`, `reserve()`, `capacity()`, `shrink_to_fit()`
- 修改器：`clear()`, `insert()`, `erase()`, `push_back()`, `pop_back()`, `resize()`, `swap()`

### map常用成员函数
- 构造函数：`map()`, `map(InputIterator first, InputIterator last)`
- 赋值：`operator=`, `operator[]`
- 迭代器：`begin()`, `end()`
- 容量：`empty()`, `size()`, `max_size()`
- 修改器：`clear()`, `insert()`, `erase()`, `swap()`
- 查找：`count()`, `find()`, `contains()`, `lower_bound()`, `upper_bound()`, `equal_range()`
- 观察器：`key_comp()`, `value_comp()`

## 非成员函数表（部分示例）

### 比较操作符
- `operator==`, `operator!=`, `operator<`, `operator<=`, `operator>`, `operator>=`
- C++20起：`operator<=>`（三路比较）

### 算法操作
- `swap()`：交换两个容器的内容
- `erase()`（C++20）：从容器中删除满足条件的元素
- `erase_if()`（C++20）：从容器中删除满足谓词的元素

## 缺陷报告

### LWG 51（C++98）
- **问题**：容器迭代器可能被任意库操作失效
- **修正**：迭代器只在规定时失效

## 相关命名要求

- Container（容器）
- SequenceContainer（序列容器）
- ContiguousContainer（连续容器）
- ReversibleContainer（可逆容器）
- AssociativeContainer（关联容器）
- AllocatorAwareContainer（分配器感知容器）
- UnorderedAssociativeContainer（无序关联容器）

## 相关页面

| 页面 | 说明 |
|------|------|
| `valarray` | 数值数组、数组掩码和数组切片（类模板） |
| `basic_string` | 存储和操作字符序列（类模板） |
| `basic_string_view`(C++17) | 只读字符串视图（类模板） |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/container&oldid=179897>
- 最后修改时间：2025年1月28日 10:28
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++容器库提供了丰富而强大的数据结构，是现代C++编程的基础。理解各类容器的特性和适用场景，能够帮助开发者选择最合适的数据结构来解决问题。从基础的vector和array到高级的flat容器和span视图，容器库不断演进以满足现代软件开发的需求。掌握迭代器失效规则、线程安全性保证和性能特征，是编写高效、安全C++代码的关键。正确使用容器适配器和现代C++特性，可以让代码更加简洁和表达力强。持续学习和实践容器库的新特性，是成为优秀C++开发者的重要途径。