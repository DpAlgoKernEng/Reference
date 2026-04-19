# C++ 范围库（Ranges library）（C++20起）

来源：cppreference.com

## 范围库概述

范围库是对算法库和迭代器库的扩展和泛化，通过使它们可组合且不易出错来增强其功能。

该库创建和操作范围**视图**（view），这些是轻量级对象，间接表示可迭代序列（**范围**）。范围是以下内容的抽象：

- `(begin, end)` - 迭代器对，例如通过容器隐式转换创建的范围
- `(begin + 0, size)` - 计数序列，例如由`views::counted`返回的范围
- `(begin, predicate)` - 条件终止序列，例如由`views::take_while`返回的范围
- `(begin, ..)` - 无界序列，例如由`views::iota`返回的范围

范围库包括范围算法（急切应用）和范围适配器（惰性应用）。适配器可以组成管道，使得操作在视图迭代时发生。

```cpp
// 命名空间别名
namespace std { namespace views = ranges::views; } // (C++20起)
```

## 核心组件

### 一、范围访问

| 函数 | 说明 |
|------|------|
| `ranges::begin`(C++20) | 返回范围起始的迭代器（定制点对象） |
| `ranges::end`(C++20) | 返回范围结束的哨兵（定制点对象） |
| `ranges::cbegin`(C++20) | 返回只读范围起始的迭代器（定制点对象） |
| `ranges::cend`(C++20) | 返回只读范围结束的哨兵（定制点对象） |
| `ranges::rbegin`(C++20) | 返回范围的反向迭代器（定制点对象） |
| `ranges::rend`(C++20) | 返回范围的反向结束迭代器（定制点对象） |
| `ranges::size`(C++20) | 返回等于范围大小的整数（定制点对象） |
| `ranges::empty`(C++20) | 检查范围是否为空（定制点对象） |
| `ranges::data`(C++20) | 获取连续范围起始的指针（定制点对象） |

### 二、范围概念

| 概念 | 说明 |
|------|------|
| `ranges::range`(C++20) | 指定类型为范围 |
| `ranges::view`(C++20) | 指定范围是视图（具有常数时间复制/移动/赋值） |
| `ranges::sized_range`(C++20) | 指定范围在常数时间知道其大小 |
| `ranges::input_range`(C++20) | 指定迭代器类型满足`input_iterator`的范围 |
| `ranges::forward_range`(C++20) | 指定迭代器类型满足`forward_iterator`的范围 |
| `ranges::bidirectional_range`(C++20) | 指定迭代器类型满足`bidirectional_iterator`的范围 |
| `ranges::random_access_range`(C++20) | 指定迭代器类型满足`random_access_iterator`的范围 |
| `ranges::contiguous_range`(C++20) | 指定迭代器类型满足`contiguous_iterator`的范围 |

### 三、范围工厂

| 工厂 | 说明 |
|------|------|
| `ranges::empty_view`(C++20) | 没有元素的空视图（类模板） |
| `ranges::single_view`(C++20) | 包含单个指定值元素的视图（类模板） |
| `ranges::iota_view`(C++20) | 通过重复递增初始值生成的序列视图（类模板） |
| `ranges::basic_istream_view`(C++20) | 通过对关联输入流连续应用`operator>>`获得元素的视图（类模板） |

### 四、范围适配器

| 适配器 | 说明 |
|--------|------|
| `views::filter`(C++20) | 由满足谓词的范围元素组成的视图（类模板） |
| `views::transform`(C++20) | 对每个元素应用变换函数的序列视图（类模板） |
| `views::take`(C++20) | 由另一个视图前N个元素组成的视图（类模板） |
| `views::drop`(C++20) | 跳过前N个元素的视图（类模板） |
| `views::join`(C++20) | 通过展平范围视图获得的序列视图（类模板） |
| `views::reverse`(C++20) | 反向迭代另一个双向视图元素的视图（类模板） |
| `views::split`(C++20) | 使用分隔符拆分视图获得的子范围视图（类模板） |

---

## 扩展知识详解

### 一、基础概念和使用

#### 1. 范围访问函数
```cpp
#include <ranges>
#include <vector>
#include <iostream>
#include <string>

void range_access_examples() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::string str = "Hello";
    
    // 基本范围访问
    auto begin_it = std::ranges::begin(vec);
    auto end_it = std::ranges::end(vec);
    std::cout << "Vector size: " << std::ranges::size(vec) << std::endl;
    std::cout << "Vector empty: " << std::ranges::empty(vec) << std::endl;
    
    // 数据访问（适用于连续范围）
    if (std::ranges::data(vec)) {
        std::cout << "Vector data pointer: " << std::ranges::data(vec) << std::endl;
    }
    
    // 只读访问
    auto cbegin_it = std::ranges::cbegin(str);
    auto cend_it = std::ranges::cend(str);
    std::cout << "String size: " << std::ranges::size(str) << std::endl;
    
    // 反向访问
    auto rbegin_it = std::ranges::rbegin(str);
    auto rend_it = std::ranges::rend(str);
    std::cout << "Reverse string: ";
    for (auto it = rbegin_it; it != rend_it; ++it) {
        std::cout << *it;
    }
    std::cout << std::endl;
}
```

#### 2. 范围概念检查
```cpp
#include <ranges>
#include <vector>
#include <list>
#include <array>
#include <iostream>

template<typename T>
void check_range_concepts(const T& range) {
    std::cout << "Type: " << typeid(T).name() << std::endl;
    std::cout << "  is_range: " << std::ranges::range<T> << std::endl;
    std::cout << "  is_sized_range: " << std::ranges::sized_range<T> << std::endl;
    std::cout << "  is_input_range: " << std::ranges::input_range<T> << std::endl;
    std::cout << "  is_forward_range: " << std::ranges::forward_range<T> << std::endl;
    std::cout << "  is_bidirectional_range: " << std::ranges::bidirectional_range<T> << std::endl;
    std::cout << "  is_random_access_range: " << std::ranges::random_access_range<T> << std::endl;
    std::cout << "  is_contiguous_range: " << std::ranges::contiguous_range<T> << std::endl;
    std::cout << "  is_view: " << std::ranges::view<T> << std::endl;
    std::cout << std::endl;
}

void range_concept_examples() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lst = {1, 2, 3, 4, 5};
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    
    check_range_concepts(vec);
    check_range_concepts(lst);
    check_range_concepts(arr);
}
```

### 二、范围工厂详解

#### 1. 基本范围工厂
```cpp
#include <ranges>
#include <iostream>

void range_factory_examples() {
    // empty_view
    auto empty_view = std::views::empty<int>;
    std::cout << "Empty view size: " << std::ranges::size(empty_view) << std::endl;
    std::cout << "Empty view empty: " << std::ranges::empty(empty_view) << std::endl;
    
    // single_view
    auto single_view = std::views::single(42);
    std::cout << "Single view size: " << std::ranges::size(single_view) << std::endl;
    std::cout << "Single view element: " << *std::ranges::begin(single_view) << std::endl;
    
    // iota_view
    auto iota_view = std::views::iota(0, 10);
    std::cout << "Iota view elements: ";
    for (const auto& elem : iota_view) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // C++23 repeat_view
    auto repeat_view = std::views::repeat(7, 5);  // 重复7五次
    std::cout << "Repeat view elements: ";
    for (const auto& elem : repeat_view) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
```

#### 2. 输入流视图（C++20）
```cpp
#include <ranges>
#include <sstream>
#include <iostream>

void istream_view_example() {
    std::stringstream ss("1 2 3 4 5");
    auto istream_range = std::views::istream<int>(ss);
    
    std::cout << "Istream view elements: ";
    for (const auto& elem : istream_range) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
```

### 三、范围适配器详解

#### 1. 基础适配器
```cpp
#include <ranges>
#include <vector>
#include <iostream>

void basic_adaptor_examples() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // filter适配器
    auto even_numbers = vec | std::views::filter([](int n) { return n % 2 == 0; });
    std::cout << "Even numbers: ";
    for (const auto& n : even_numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // transform适配器
    auto squares = vec | std::views::transform([](int n) { return n * n; });
    std::cout << "Squares: ";
    for (const auto& n : squares) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // take适配器
    auto first_five = vec | std::views::take(5);
    std::cout << "First five: ";
    for (const auto& n : first_five) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // drop适配器
    auto skip_three = vec | std::views::drop(3);
    std::cout << "Skip three: ";
    for (const auto& n : skip_three) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}
```

#### 2. 复合适配器
```cpp
#include <ranges>
#include <vector>
#include <iostream>

void composite_adaptor_examples() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 管道语法：过滤偶数并取前3个的平方
    auto pipeline_result = vec 
                         | std::views::filter([](int n) { return n % 2 == 0; })
                         | std::views::take(3)
                         | std::views::transform([](int n) { return n * n; });
    
    std::cout << "Pipeline result: ";
    for (const auto& n : pipeline_result) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 传统函数调用语法
    auto functional_result = std::views::transform(
        std::views::take(
            std::views::filter(vec, [](int n) { return n % 2 == 0; }),
            3
        ),
        [](int n) { return n * n; }
    );
    
    std::cout << "Functional result: ";
    for (const auto& n : functional_result) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}
```

#### 3. 高级适配器
```cpp
#include <ranges>
#include <vector>
#include <iostream>
#include <string>

void advanced_adaptor_examples() {
    // join适配器
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4, 5}, {6}};
    auto flattened = nested | std::views::join;
    std::cout << "Flattened: ";
    for (const auto& n : flattened) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // reverse适配器
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto reversed = vec | std::views::reverse;
    std::cout << "Reversed: ";
    for (const auto& n : reversed) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // split适配器
    std::string text = "hello,world,cpp,ranges";
    auto split_view = text | std::views::split(',') 
                           | std::views::transform([](auto&& rng) {
                               return std::string_view(&*rng.begin(), std::ranges::distance(rng));
                           });
    
    std::cout << "Split elements: ";
    for (const auto& part : split_view) {
        std::cout << "[" << part << "] ";
    }
    std::cout << std::endl;
}
```

### 四、C++23新增特性

#### 1. zip和相关适配器（C++23）
```cpp
#include <ranges>
#include <vector>
#include <iostream>

void zip_adaptor_examples() {
    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::vector<std::string> words = {"one", "two", "three", "four", "five"};
    
    // zip适配器
    auto zipped = std::views::zip(nums, words);
    std::cout << "Zipped pairs: ";
    for (const auto& [num, word] : zipped) {
        std::cout << "(" << num << "," << word << ") ";
    }
    std::cout << std::endl;
    
    // enumerate适配器
    auto enumerated = words | std::views::enumerate;
    std::cout << "Enumerated: ";
    for (const auto& [index, word] : enumerated) {
        std::cout << "(" << index << "," << word << ") ";
    }
    std::cout << std::endl;
    
    // adjacent适配器
    auto adjacent_pairs = nums | std::views::adjacent<2>;
    std::cout << "Adjacent pairs: ";
    for (const auto& [a, b] : adjacent_pairs) {
        std::cout << "(" << a << "," << b << ") ";
    }
    std::cout << std::endl;
}
```

#### 2. chunk和slide适配器（C++23）
```cpp
#include <ranges>
#include <vector>
#include <iostream>

void chunk_slide_examples() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // chunk适配器
    auto chunks = vec | std::views::chunk(3);
    std::cout << "Chunks of 3: ";
    for (const auto& chunk : chunks) {
        std::cout << "[";
        for (const auto& elem : chunk) {
            std::cout << elem << " ";
        }
        std::cout << "] ";
    }
    std::cout << std::endl;
    
    // slide适配器
    auto sliding = vec | std::views::slide(3);
    std::cout << "Sliding window of 3: ";
    for (const auto& window : sliding) {
        std::cout << "[";
        for (const auto& elem : window) {
            std::cout << elem << " ";
        }
        std::cout << "] ";
    }
    std::cout << std::endl;
}
```

### 五、范围生成器（C++23）

#### 1. 同步协程生成器
```cpp
#include <generator>
#include <ranges>
#include <iostream>

// C++23生成器示例
std::generator<int> fibonacci_sequence(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        int next = a + b;
        a = b;
        b = next;
    }
}

void generator_example() {
    std::cout << "Fibonacci sequence: ";
    for (int value : fibonacci_sequence(10)) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // 生成器是视图，可以使用范围适配器
    auto even_fib = fibonacci_sequence(15) 
                  | std::views::filter([](int n) { return n % 2 == 0; })
                  | std::views::take(5);
    
    std::cout << "Even Fibonacci numbers: ";
    for (int value : even_fib) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}
```

### 六、范围转换（C++23）

#### 1. ranges::to转换
```cpp
#include <ranges>
#include <vector>
#include <list>
#include <iostream>

void range_conversion_examples() {
    // 从视图转换为容器
    auto filtered_view = std::views::iota(1, 11) 
                        | std::views::filter([](int n) { return n % 2 == 0; });
    
    // 转换为vector
    std::vector<int> even_vector = std::ranges::to<std::vector<int>>(filtered_view);
    std::cout << "Even vector: ";
    for (const auto& n : even_vector) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 转换为list
    std::list<int> even_list = std::ranges::to<std::list<int>>(filtered_view);
    std::cout << "Even list: ";
    for (const auto& n : even_list) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 带分配器的转换
    std::vector<int> with_allocator = std::ranges::to<std::vector<int>>(filtered_view, std::allocator<int>{});
    std::cout << "With allocator size: " << with_allocator.size() << std::endl;
}
```

### 七、实际应用场景

#### 1. 数据处理管道
```cpp
#include <ranges>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

struct Person {
    std::string name;
    int age;
    double salary;
};

void data_processing_pipeline() {
    std::vector<Person> people = {
        {"Alice", 30, 50000},
        {"Bob", 25, 45000},
        {"Charlie", 35, 60000},
        {"David", 28, 48000},
        {"Eve", 32, 55000}
    };
    
    // 复杂数据处理管道
    auto processed_data = people
        | std::views::filter([](const Person& p) { return p.age >= 30; })  // 筛选年龄>=30
        | std::views::transform([](const Person& p) {  // 转换为姓名和薪资
            return std::make_pair(p.name, p.salary);
        })
        | std::views::filter([](const auto& pair) { return pair.second > 50000; })  // 薪资>50000
        | std::views::transform([](const auto& pair) {  // 格式化输出
            return pair.first + ": $" + std::to_string(static_cast<int>(pair.second));
        });
    
    std::cout << "Processed data: ";
    for (const auto& item : processed_data) {
        std::cout << item << " | ";
    }
    std::cout << std::endl;
}
```

#### 2. 文件处理示例
```cpp
#include <ranges>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

void file_processing_example() {
    // 模拟文件内容
    std::string file_content = "10\n20\n30\n40\n50\ninvalid\n60\n70";
    std::stringstream ss(file_content);
    
    // 处理文件中的数字
    auto numbers = std::views::istream<std::string>(ss)
                 | std::views::transform([](const std::string& str) -> std::optional<int> {
                     try {
                         return std::stoi(str);
                     } catch (...) {
                         return std::nullopt;
                     }
                 })
                 | std::views::filter([](const std::optional<int>& opt) { return opt.has_value(); })
                 | std::views::transform([](const std::optional<int>& opt) { return opt.value(); })
                 | std::views::filter([](int n) { return n > 30; });  // 只要大于30的数字
    
    std::cout << "Filtered numbers: ";
    for (const auto& n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}
```

### 八、性能优化和最佳实践

#### 1. 惰性求值优势
```cpp
#include <ranges>
#include <vector>
#include <chrono>
#include <iostream>

void lazy_evaluation_benefit() {
    std::vector<int> large_vector(1000000);
    std::iota(large_vector.begin(), large_vector.end(), 1);
    
    // 惰性求值：只处理需要的元素
    auto start = std::chrono::high_resolution_clock::now();
    
    auto result = large_vector
                 | std::views::filter([](int n) { return n % 2 == 0; })  // 过滤偶数
                 | std::views::transform([](int n) { return n * n; })    // 平方
                 | std::views::take(10);                                 // 只取前10个
    
    // 实际计算只在迭代时发生
    std::cout << "First 10 results: ";
    for (const auto& n : result) {
        std::cout << n << " ";
        if (--const_cast<int&>(n) < 0) break;  // 模拟只处理部分结果
    }
    std::cout << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
}
```

#### 2. 内存效率比较
```cpp
#include <ranges>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>

void memory_efficiency_comparison() {
    std::vector<int> data(100000);
    std::iota(data.begin(), data.end(), 1);
    
    // 传统方式：创建中间容器
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> temp1, temp2, result_traditional;
    
    std::copy_if(data.begin(), data.end(), std::back_inserter(temp1),
                 [](int n) { return n % 2 == 0; });
    
    std::transform(temp1.begin(), temp1.end(), std::back_inserter(temp2),
                   [](int n) { return n * n; });
    
    std::copy_n(temp2.begin(), 100, std::back_inserter(result_traditional));
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_traditional = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 范围方式：链式操作，无中间容器
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> result_ranges;
    
    auto range_result = data
                      | std::views::filter([](int n) { return n % 2 == 0; })
                      | std::views::transform([](int n) { return n * n; })
                      | std::views::take(100);
    
    std::ranges::copy(range_result, std::back_inserter(result_ranges));
    
    end = std::chrono::high_resolution_clock::now();
    auto duration_ranges = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Traditional approach time: " << duration_traditional.count() << " microseconds" << std::endl;
    std::cout << "Ranges approach time: " << duration_ranges.count() << " microseconds" << std::endl;
    std::cout << "Memory usage: Ranges approach uses less intermediate memory" << std::endl;
}
```

## 最佳实践建议

### 一、范围使用准则

1. **优先使用管道语法**：`range | view1 | view2`比嵌套函数调用更清晰
2. **理解惰性求值**：视图在迭代时才执行操作
3. **合理组合适配器**：避免不必要的复杂管道
4. **注意所有权**：视图不拥有数据，确保底层数据在使用期间有效

### 二、性能考虑

1. **避免重复计算**：复杂视图应保存在变量中重用
2. **选择合适适配器**：了解各适配器的时间复杂度
3. **及时转换**：需要多次访问时考虑转换为容器
4. **内存局部性**：连续范围比非连续范围访问效率更高

### 三、错误处理

1. **边界检查**：使用`views::take`等避免越界
2. **类型安全**：利用编译期检查确保类型正确
3. **异常安全**：范围操作应保证强异常安全保证

## 相关页面

| 页面 | 说明 |
|------|------|
| 迭代器库 | 基础迭代器概念 |
| 约束算法 | 使用概念的算法 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/ranges&oldid=180221>
- 最后修改时间：2025年2月7日 00:05
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++范围库是现代C++的重要特性，它通过提供可组合、惰性求值的范围操作，大幅简化了数据处理代码。从基础的范围访问到复杂的适配器组合，从简单的过滤变换到高级的zip和chunk操作，范围库为开发者提供了强大的工具。正确理解和使用范围库能够编写更清晰、更高效的代码。惰性求值避免了不必要的中间计算，管道语法提升了代码可读性，而丰富的适配器集合满足了各种数据处理需求。随着C++23引入更多特性如生成器、zip适配器等，范围库变得更加完善。持续学习和实践范围库技术，结合性能分析和最佳实践，能够构建出高质量的现代C++应用程序。