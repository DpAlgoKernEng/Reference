# std符号索引（std Symbol Index）

来源：cppreference.com

## C++标准库符号索引

本页面试图列出标准库命名空间`std`中可用的所有符号。符号书写规则如下：

- 函数名称带`()`。
- 模板带`<>`。

`std`子命名空间（如chrono）中的符号未在此列出，但命名空间名称（以▶图标前置）是对应页面的链接。

宏符号、仅用于说明的符号和已从标准库中移除的符号在单独的页面中列出。

## `_ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z`

### 宏（Macros）

### _ (下划线)

- `_Exit()` (C++11起)

### A

#### 数学函数
- `abs()` (int)
- `abs()` (float)
- `abs<>()` (`std::complex`)
- `abs<>()` (`std::valarray`)
- `acos()`
- `acos<>()` (`std::complex`) (C++11起)
- `acos<>()` (`std::valarray`)
- `acosf()` (C++11起)
- `acosh()` (C++11起)
- `acosh<>()` (`std::complex`) (C++11起)
- `acoshf()` (C++11起)
- `acoshl()` (C++11起)
- `acosl()` (C++11起)

#### 算法函数
- `accumulate<>()`
- `adjacent_difference<>`
- `adjacent_find<>`
- `all_of<>()` (C++11起)
- `any_of<>()` (C++11起)

#### 类型特征
- `add_const<>` (C++11起)
- `add_const_t<>` (C++14起)
- `add_cv<>` (C++11起)
- `add_cv_t<>` (C++14起)
- `add_pointer<>` (C++11起)
- `add_pointer_t<>` (C++14起)
- `add_lvalue_reference<>` (C++11起)
- `add_lvalue_reference_t<>` (C++14起)
- `add_rvalue_reference<>` (C++11起)
- `add_rvalue_reference_t<>` (C++14起)

#### 内存管理
- `addressof<>` (C++11起)
- `align()` (C++11起)
- `aligned_alloc()` (C++17起)
- `aligned_storage<>` (C++11起)
- `aligned_storage_t<>` (C++14起)
- `aligned_union<>` (C++11起)
- `aligned_union_t<>` (C++14起)

#### 容器和分配器
- `allocator<>`
- `allocator_traits<>` (C++11起)
- `allocate_shared<>()` (C++11起)

#### 原子操作
- `atomic<>` (C++11起)
- `atomic_flag` (C++11起)
- `atomic_ref<>` (C++20起)

#### 其他
- `array<>` (C++11起)
- `async<>()` (C++11起)

### B

#### 迭代器和容器
- `back_insert_iterator<>`
- `back_inserter<>`

#### 异常类
- `bad_alloc`
- `bad_cast`
- `bad_exception`

#### 容器类
- `basic_string<>`
- `basic_string_view<>` (C++17起)

#### 数学函数
- `beta()` (C++17起)

### C

#### 字符处理
- `cbrt()` (C++11起)
- `ceil()` (C++11起)

#### 时间函数
- `clock()`

#### 复数函数
- `conj<>()`

#### 容器类
- `complex<>`

#### 条件变量
- `condition_variable` (C++11起)

#### 常用函数
- `cos()`
- `cosh()`

### D

#### 容器操作
- `data<>()` (C++17起)
- `distance<>()`

#### 数学函数
- `div()`

#### 类型特征
- `decay<>` (C++11起)

### E

#### 容器类
- `endian` (C++20起)

#### 错误处理
- `error_code` (C++11起)
- `exception`

#### 数学函数
- `exp()`
- `exp2()` (C++11起)

#### 实用工具
- `exchange<>()` (C++14起)

### F

#### 文件操作
- `FILE`
- `fclose()`
- `fopen()`

#### 数学函数
- `fabs()`
- `floor()`

#### 容器类
- `forward_list<>` (C++11起)

### G

#### 数学函数
- `gcd<>()` (C++17起)

#### 容器操作
- `get<>()` (`std::pair`) (C++11起)
- `get<>()` (`std::tuple`) (C++11起)

#### 时间函数
- `gmtime()`

### H

#### 哈希
- `hash<>` (C++11起)

### I

#### 输入输出
- `ifstream`
- `istream`
- `istringstream`

#### 整数类型
- `int16_t` (C++11起)
- `int32_t` (C++11起)
- `int64_t` (C++11起)
- `int8_t` (C++11起)

#### 实用工具
- `invoke<>()` (C++17起)

### J

#### 错误处理
- `jmp_buf`

### K

#### 数学常数
- `kilo` (C++11起)

### L

#### 数学函数
- `lcm<>()` (C++17起)
- `log()`
- `log10()`

#### 容器类
- `list<>`

### M

#### 内存管理
- `make_shared<>()` (C++11起)
- `make_unique<>()` (C++14起)
- `malloc()`

#### 容器类
- `map<>`
- `multimap<>`
- `multiset<>`

#### 数学函数
- `max()`
- `min()`

### N

#### 数学函数
- `nan()` (C++11起)
- `nearbyint()` (C++11起)

#### 常量
- `nullopt` (C++17起)
- `nullptr_t` (C++11起)

### O

#### 输入输出
- `ofstream`
- `ostream`
- `ostringstream`

#### 容器类
- `optional<>` (C++17起)

### P

#### 数学函数
- `pow()`

#### 容器类
- `pair<>`
- `priority_queue<>`

#### 线程支持
- `promise<>` (C++11起)

### Q

#### 标准库函数
- `qsort()`

#### 容器类
- `queue<>`

### R

#### 随机数
- `rand()`
- `random_device` (C++11起)

#### 正则表达式
- `regex` (C++11起)

#### 容器类
- `reverse_iterator<>`

### S

#### 字符串
- `size_t`
- `string`
- `string_view` (C++17起)

#### 数学函数
- `sin()`
- `sqrt()`
- `srand()`

#### 容器类
- `set<>`
- `stack<>`

#### 线程支持
- `shared_ptr<>` (C++11起)
- `shared_mutex` (C++17起)

### T

#### 数学函数
- `tan()`
- `tgamma()` (C++11起)

#### 时间
- `thread` (C++11起)
- `time()`

#### 类型信息
- `tuple<>` (C++11起)
- `type_info`

### U

#### 整数类型
- `uint16_t` (C++11起)
- `uint32_t` (C++11起)
- `uint64_t` (C++11起)
- `uint8_t` (C++11起)

#### 容器类
- `unique_ptr<>` (C++11起)
- `unordered_map<>` (C++11起)
- `unordered_set<>` (C++11起)

### V

#### 变参函数
- `va_list`

#### 容器类
- `valarray<>`
- `variant<>` (C++17起)
- `vector<>`

#### 输入输出
- `vprintf()`
- `vscanf()` (C++11起)

### W

#### 宽字符支持
- `wcerr`
- `wcin`
- `wcout`
- `wstring`
- `wstring_view` (C++17起)

### X

### Y

#### SI前缀
- `yocto` (C++11起)
- `yotta` (C++11起)

### Z

#### SI前缀
- `zepto` (C++11起)
- `zetta` (C++11起)

## 子命名空间

| 命名空间 | 说明 |
|----------|------|
| ▶ `chrono` (C++11起) | 时间库 |
| ▶ `execution` (C++17起) | 执行策略 |
| ▶ `filesystem` (C++17起) | 文件系统 |
| ▶ `literals` (C++14起) | 字面量 |
| ▶ `numbers` (C++20起) | 数学常数 |
| ▶ `placeholders` (C++11起) | 占位符 |
| ▶ `pmr` (C++17起) | 多态分配器 |
| ▶ `ranges` (C++20起) | 范围 |
| ▶ `regex_constants` (C++11起) | 正则表达式常量 |
| ▶ `rel_ops` (C++20起已弃用) | 关系运算符 |
| ▶ `this_thread` (C++11起) | 线程相关函数 |

## 相关页面

| 页面 | 说明 |
|------|------|
| C符号索引 | |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/symbol_index&oldid=180135>
- 最后修改时间：2025年2月4日 13:48
- 离线版本获取时间：2025年2月9日 16:39

---

## 扩展知识详解

### 一、标准库符号分类详解

#### 1. 容器类符号
```cpp
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <array>
#include <deque>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>

// 容器类使用示例
void container_examples() {
    // 顺序容器
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::deque<int> deq = {1, 2, 3, 4, 5};
    std::list<int> lst = {1, 2, 3, 4, 5};
    std::forward_list<int> flst = {1, 2, 3, 4, 5};
    
    // 关联容器
    std::map<std::string, int> mp = {{"one", 1}, {"two", 2}};
    std::set<int> st = {1, 2, 3, 4, 5};
    std::multimap<std::string, int> mmp;
    std::multiset<int> mst;
    
    // 无序关联容器
    std::unordered_map<std::string, int> ump = {{"one", 1}, {"two", 2}};
    std::unordered_set<int> ust = {1, 2, 3, 4, 5};
    
    // 容器适配器
    std::stack<int> stk;
    std::queue<int> que;
    std::priority_queue<int> pque;
}
```

#### 2. 智能指针符号
```cpp
#include <memory>
#include <iostream>

// 智能指针使用示例
void smart_pointer_examples() {
    // unique_ptr - 独占所有权
    auto uptr = std::make_unique<int>(42);
    std::cout << "unique_ptr value: " << *uptr << std::endl;
    
    // shared_ptr - 共享所有权
    auto sptr1 = std::make_shared<std::string>("Hello");
    auto sptr2 = sptr1;  // 共享所有权
    std::cout << "shared_ptr use count: " << sptr1.use_count() << std::endl;
    
    // weak_ptr - 弱引用
    std::weak_ptr<std::string> wptr = sptr1;
    if (auto locked = wptr.lock()) {
        std::cout << "weak_ptr locked: " << *locked << std::endl;
    }
    
    // 自定义删除器
    auto custom_deleter = [](int* p) {
        std::cout << "Custom deleter called for " << *p << std::endl;
        delete p;
    };
    std::unique_ptr<int, decltype(custom_deleter)> ptr(new int(100), custom_deleter);
}
```

#### 3. 线程支持符号
```cpp
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <iostream>

// 线程支持使用示例
void thread_support_examples() {
    // 线程
    std::thread t([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Thread function executed" << std::endl;
    });
    
    // 互斥量
    std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    
    // 异步操作
    auto future = std::async(std::launch::async, []() {
        return 42;
    });
    int result = future.get();
    std::cout << "Async result: " << result << std::endl;
    
    if (t.joinable()) {
        t.join();
    }
}
```

#### 4. 算法符号
```cpp
#include <algorithm>
#include <vector>
#include <iostream>

// 算法使用示例
void algorithm_examples() {
    std::vector<int> vec = {5, 2, 8, 1, 9, 3};
    
    // 排序
    std::sort(vec.begin(), vec.end());
    
    // 查找
    auto it = std::find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        std::cout << "Found: " << *it << std::endl;
    }
    
    // 变换
    std::transform(vec.begin(), vec.end(), vec.begin(), 
                   [](int x) { return x * 2; });
    
    // 累积
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "Sum: " << sum << std::endl;
}
```

### 二、现代C++特性符号

#### 1. 可选值和变体
```cpp
#include <optional>
#include <variant>
#include <any>
#include <iostream>

// 现代C++特性使用示例
void modern_cpp_features() {
    // std::optional
    std::optional<int> opt = 42;
    if (opt.has_value()) {
        std::cout << "Optional value: " << *opt << std::endl;
    }
    
    // std::variant
    std::variant<int, std::string, double> var = 42;
    std::visit([](const auto& value) {
        std::cout << "Variant value: " << value << std::endl;
    }, var);
    
    // std::any
    std::any any_value = std::string("Hello");
    if (any_value.type() == typeid(std::string)) {
        std::cout << "Any value: " << std::any_cast<std::string>(any_value) << std::endl;
    }
}
```

#### 2. 范围和视图（C++20）
```cpp
#include <ranges>
#include <vector>
#include <iostream>

// 范围使用示例
void ranges_examples() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用范围算法
    auto even_numbers = vec | std::views::filter([](int n) { return n % 2 == 0; })
                            | std::views::take(3);
    
    for (int n : even_numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}
```

#### 3. 格式化（C++20）
```cpp
#include <format>
#include <iostream>

// 格式化使用示例
void format_examples() {
    // std::format
    std::string formatted = std::format("Hello, {}! Value: {:.2f}", "World", 3.14159);
    std::cout << formatted << std::endl;
    
    // std::print
    std::print("Print to stdout: {} {}\n", 42, "test");
}
```

### 三、类型特征符号

#### 1. 类型检查和变换
```cpp
#include <type_traits>
#include <iostream>

// 类型特征使用示例
void type_traits_examples() {
    // 类型检查
    static_assert(std::is_integral_v<int>);
    static_assert(std::is_floating_point_v<double>);
    static_assert(std::is_same_v<int, int>);
    
    // 类型变换
    using add_const_int = std::add_const_t<int>;
    using remove_const_int = std::remove_const_t<const int>;
    using make_unsigned_int = std::make_unsigned_t<int>;
    
    std::cout << "Type traits examples demonstrated" << std::endl;
}
```

### 四、实用工具符号

#### 1. 实用函数模板
```cpp
#include <utility>
#include <functional>
#include <memory>
#include <iostream>

// 实用工具使用示例
void utility_examples() {
    // std::move
    std::string str = "Hello";
    std::string moved = std::move(str);
    
    // std::forward
    auto lambda = [](auto&& arg) {
        return std::forward<decltype(arg)>(arg);
    };
    
    // std::exchange
    int value = 42;
    int old_value = std::exchange(value, 100);
    std::cout << "Old value: " << old_value << ", New value: " << value << std::endl;
    
    // std::bind
    auto bound_func = std::bind([](int a, int b) { return a + b; }, 10, std::placeholders::_1);
    std::cout << "Bound function result: " << bound_func(5) << std::endl;
}
```

### 五、数学和数值符号

#### 1. 数学函数和常数
```cpp
#include <cmath>
#include <numbers>
#include <random>
#include <iostream>

// 数学符号使用示例
void math_examples() {
    // 基本数学函数
    double result = std::sqrt(16.0);
    std::cout << "Square root of 16: " << result << std::endl;
    
    // C++20数学常数
    constexpr double pi = std::numbers::pi;
    constexpr double e = std::numbers::e;
    std::cout << "Pi: " << pi << ", e: " << e << std::endl;
    
    // 随机数生成
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    int random_num = dis(gen);
    std::cout << "Random number: " << random_num << std::endl;
}
```

✅ C++标准库符号索引为开发者提供了全面的参考，涵盖了从基本类型到高级特性的所有标准库组件。通过这个索引，开发者可以快速查找和理解各种标准库符号的用途和用法。现代C++不断引入新特性，如智能指针、线程支持、范围、格式化等，这些都在索引中有详细记录。正确理解和使用这些符号对于编写高质量的C++代码至关重要。建议开发者在学习和使用C++时，经常查阅此索引以确保使用的是最新的标准库特性。持续学习和实践标准库的各种功能，是掌握现代C++编程的关键。