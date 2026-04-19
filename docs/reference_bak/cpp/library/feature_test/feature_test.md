# C++特性测试（Feature Testing）（C++20起）

来源：cppreference.com

## 特性测试概述

标准定义了一组预处理宏，对应于C++11或之后版本中引入的C++语言和库特性。它们旨在成为检测这些特性存在的简单且可移植的方法。

---

## 属性特性测试

### `__has_cpp_attribute`宏

检查对名为attribute-token的属性（经过宏展开后）的支持。

对于每个标准属性，是否`__has_cpp_attribute`展开为下表中给出的值（即属性添加到工作草案中的年份和月份）或是0是实现定义的。只有当标准属性使实现按照推荐方式行为时（发出诊断消息、影响类布局等），它才会展开为表中给定的值。

供应商特定属性的存在由非零值确定。

`__has_cpp_attribute`可以在#if和#elif的表达式中展开。它被#ifdef、#ifndef、#elifdef、#elifndef（C++23起）和defined视为已定义的宏，但不能在其他地方使用。

### 标准属性及其测试值

| 属性标记 | 属性 | 值 | 标准 | 论文 |
|----------|------|----|------|------|
| `assume` | `[[assume]]` | `202207L` | C++23 | P1774R8 |
| `carries_dependency` | `[[carries_dependency]]` | `200809L` | C++11 | N2556 N2643 |
| `deprecated` | `[[deprecated]]` | `201309L` | C++14 | N3760 |
| `fallthrough` | `[[fallthrough]]` | `201603L` | C++17 | P0188R1 |
| `indeterminate` | `[[indeterminate]]` | `202403L` | C++26 | P2795R5 |
| `likely` | `[[likely]]` | `201803L` | C++20 | P0479R5 |
| `maybe_unused` | `[[maybe_unused]]` | `201603L` | C++17 | P0212R1 |
| `no_unique_address` | `[[no_unique_address]]` | `201803L` | C++20 | P0840R2 |
| `nodiscard` | `[[nodiscard]]` | `201603L` | C++17 | P0189R1 |
| `[[nodiscard]]` with reason | `201907L` | C++20 | P1301R4 |
| `noreturn` | `[[noreturn]]` | `200809L` | C++11 | N2761 |
| `unlikely` | `[[unlikely]]` | `201803L` | C++20 | P0479R5 |

标准属性总数：11个

---

## 语言特性测试

以下宏可用于检测当前实现是否实现了语言特性。它们在每个翻译单元中预先定义。

每个宏展开为一个整型字面量，对应于相应特性包含在工作草案中的年份和月份。当特性发生重大更改时，宏将相应更新。

### 语言特性宏示例

| 宏名称 | 特性 | 值 | 标准 | 论文 |
|--------|------|----|------|------|
| `__cpp_aggregate_bases` | 带基类的聚合类 | `201603L` | C++17 | P0017R1 |
| `__cpp_alias_templates` | 别名模板 | `200704L` | C++11 | N2258 |
| `__cpp_concepts` | 概念 | `201907L` | C++20 | P0734R0 P1084R2 P1452R2 |
| `__cpp_constexpr` | constexpr | `200704L` | C++11 | N2235 |
| `__cpp_if_consteval` | `if consteval` | `202106L` | C++23 | P1938R3 |
| `__cpp_modules` | 模块 | `201907L` | C++20 | P1103R3 P1811R0 |
| `__cpp_static_assert` | static_assert | `200410L` | C++11 | N1720 |

语言特性宏总数：71个

---

## 库特性测试

以下宏可用于检测当前实现是否实现了标准库特性。与语言特性测试宏不同，它们不是预先定义的。相反，它们由头文件<version>提供。

对于每个库特性测试宏，包含相关标准库组件的头文件也会提供它。

每个宏展开为一个整型字面量，对应于相应特性包含在工作草案中的年份和月份。当特性发生重大更改时，宏将相应更新。

### 库特性宏示例

| 宏名称 | 特性 | 值 | 标准 | 论文 |
|--------|------|----|------|------|
| `__cpp_lib_any` | std::any | `201606L` | C++17 | P0220R1 P0032R3 |
| `__cpp_lib_apply` | std::apply | `201603L` | C++17 | P0220R1 |
| `__cpp_lib_array_constexpr` | std::array的constexpr | `201603L` | C++17 | P0031R0 |
| `__cpp_lib_barrier` | std::barrier | `201907L` | C++20 | P1135R6 |
| `__cpp_lib_bind_front` | std::bind_front | `201907L` | C++20 | P0356R5 P1651R0 |
| `__cpp_lib_concepts` | 标准库概念 | `202002L` | C++20 | P0898R3 P1754R1 P1964R2 |
| `__cpp_lib_format` | 文本格式化 | `201907L` | C++20 | P0645R10 P1361R2 P1652R1 |
| `__cpp_lib_ranges` | 范围库 | `201911L` | C++20 | P0896R4 P1035R7 P1716R3 |
| `__cpp_lib_span` | std::span | `202002L` | C++20 | P0122R7 LWG3274 P1024R3 P1976R2 |
| `__cpp_lib_variant` | std::variant | `201606L` | C++17 | P0088R3 P0393R3 P0032R3 |

库特性宏总数：246个

---

## 扩展知识详解

### 一、特性测试的重要性

特性测试宏允许开发者编写可移植的代码，能够适应不同编译器和标准库的实现水平。

#### 基本使用模式
```cpp
#include <version>  // C++20及以后版本

// 检查是否有某个头文件
#ifdef __has_include
#  if __has_include(<optional>)
#    include <optional>
#    define HAS_OPTIONAL 1
#  elif __has_include(<experimental/optional>)
#    include <experimental/optional>
#    define HAS_OPTIONAL 1
#    define OPTIONAL_EXPERIMENTAL 1
#  else
#    define HAS_OPTIONAL 0
#  endif
#endif

// 检查特性支持
#ifdef __cpp_concepts
#  if __cpp_concepts >= 201907L
#    define HAS_MODERN_CONCEPTS 1
#  endif
#endif
```

### 二、语言特性详解

#### 1. constexpr演进
```cpp
// C++11: 基础constexpr
constexpr int square(int x) {
    return x * x;  // 只能包含return语句
}

// C++14: 放松限制
constexpr int factorial(int n) {
    if (n <= 1) return 1;  // 可以使用if语句
    return n * factorial(n - 1);
}

// C++20: 虚函数调用等更多特性
constexpr virtual ~Base() = default;

// C++23: 更多constexpr特性
constexpr void complex_function() {
    goto label;  // C++23允许goto
    int x = 42;  // 非字面量变量
label:
    ;
}
```

#### 2. 概念（Concepts）
```cpp
#include <concepts>

// 基础概念使用
template<std::integral T>
T add(T a, T b) {
    return a + b;
}

// 自定义概念
template<typename T>
concept sortable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

template<sortable T>
void sort_container(std::vector<T>& container) {
    std::sort(container.begin(), container.end());
}
```

#### 3. 模块（Modules）
```cpp
// math.ixx - 模块接口单元
export module math;

export namespace math {
    int add(int a, int b) {
        return a + b;
    }
    
    template<typename T>
    T multiply(T a, T b) {
        return a * b;
    }
}

// main.cpp - 使用模块
import math;

int main() {
    int result = math::add(3, 4);
    return 0;
}
```

### 三、库特性详解

#### 1. 智能容器和视图
```cpp
#include <ranges>
#include <vector>
#include <span>

// span - 安全的数组视图
void process_data(std::span<const int> data) {
    for (int value : data) {
        std::cout << value << " ";
    }
}

// ranges - 现代算法
void range_example() {
    std::vector<int> vec = {5, 2, 8, 1, 9, 3};
    
    // 管道式操作
    auto result = vec 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; })
        | std::ranges::to<std::vector>();
    
    // 算法直接作用于范围
    std::ranges::sort(vec);
    auto found = std::ranges::find(vec, 5);
}
```

#### 2. 并发支持
```cpp
#include <atomic>
#include <thread>
#include <barrier>

// 原子操作演进
void atomic_example() {
    // C++20: 原子等待
    std::atomic<bool> flag{false};
    std::thread t([&flag]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        flag.store(true);
    });
    
    flag.wait(false);  // 高效等待
    t.join();
}

// 屏障
void barrier_example() {
    std::barrier<> barrier(3);
    
    auto worker = [&barrier]() {
        // 工作...
        barrier.arrive_and_wait();  // 等待所有线程到达
        // 继续工作...
    };
}
```

#### 3. 格式化库
```cpp
#include <format>
#include <print>

void formatting_example() {
    // std::format - 类型安全的格式化
    std::string msg = std::format("Hello, {}! Value: {}", "World", 42);
    
    // std::print - 现代打印
    std::print("Direct output: {}\n", msg);
    
    // 复杂类型格式化
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::print("Vector: {::d}\n", vec);
}
```

### 四、实际应用示例

#### 1. 跨平台兼容性库
```cpp
// modern_optional.hpp
#pragma once

#ifdef __has_include
#  if __has_include(<optional>)
#    include <optional>
     template<typename T>
     using optional = std::optional<T>;
#    define MODERN_OPTIONAL_AVAILABLE 1
#  elif __has_include(<experimental/optional>)
#    include <experimental/optional>
     template<typename T>
     using optional = std::experimental::optional<T>;
#    define MODERN_OPTIONAL_AVAILABLE 1
#    define MODERN_OPTIONAL_EXPERIMENTAL 1
#  else
     // 回退实现
     template<typename T>
     class simple_optional {
         T value_;
         bool has_value_;
     public:
         simple_optional() : has_value_(false) {}
         simple_optional(const T& value) : value_(value), has_value_(true) {}
         bool has_value() const { return has_value_; }
         const T& value() const { return value_; }
     };
     template<typename T>
     using optional = simple_optional<T>;
#    define MODERN_OPTIONAL_AVAILABLE 0
#  endif
#endif
```

#### 2. 特性检测工具类
```cpp
// feature_detector.hpp
#pragma once

#include <version>

class FeatureDetector {
public:
    static constexpr bool has_concepts() {
#ifdef __cpp_concepts
        return __cpp_concepts >= 201907L;
#else
        return false;
#endif
    }
    
    static constexpr bool has_format() {
#ifdef __cpp_lib_format
        return __cpp_lib_format >= 201907L;
#else
        return false;
#endif
    }
    
    static constexpr bool has_span() {
#ifdef __cpp_lib_span
        return __cpp_lib_span >= 202002L;
#else
        return false;
#endif
    }
    
    static constexpr bool has_ranges() {
#ifdef __cpp_lib_ranges
        return __cpp_lib_ranges >= 201911L;
#else
        return false;
#endif
    }
};
```

#### 3. 条件编译的现代实现
```cpp
// string_util.hpp
#pragma once

#include <string>
#include <string_view>

#ifdef __cpp_lib_string_contains
// 使用C++23的contains特性
inline bool contains(const std::string& str, const std::string& substr) {
    return str.contains(substr);
}
#else
// 回退实现
inline bool contains(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}
#endif

#ifdef __cpp_lib_format
#include <format>
template<typename... Args>
std::string format_string(const std::string& fmt, Args&&... args) {
    return std::format(fmt, std::forward<Args>(args)...);
}
#else
#include <sstream>
template<typename... Args>
std::string format_string(const std::string& fmt, Args&&... args) {
    // 简单的sprintf风格实现
    // 实际实现会更复杂
    std::ostringstream oss;
    // 格式化逻辑...
    return oss.str();
}
#endif
```

### 五、最佳实践

#### 1. 特性检测模式
```cpp
// 推荐的特性检测模式
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // 使用std::format
    auto result = std::format("Value: {}", value);
#else
    // 回退到sprintf或iostream
    std::ostringstream oss;
    oss << "Value: " << value;
    auto result = oss.str();
#endif
```

#### 2. 版本兼容性管理
```cpp
// version_compat.hpp
#pragma once

// 定义最小支持的C++标准
#if __cplusplus < 201703L
#  error "C++17 or later required"
#endif

// 特性可用性检查
namespace compat {
    
#if __cpp_lib_format >= 201907L
    inline constexpr bool has_format = true;
#else
    inline constexpr bool has_format = false;
#endif

#if __cpp_lib_ranges >= 201911L
    inline constexpr bool has_ranges = true;
#else
    inline constexpr bool has_ranges = false;
#endif

} // namespace compat
```

---

## 实际代码示例

### 编译器特性转储程序

以下是一个完整的特性检测程序：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>

// 简化版本的特性检测程序
class FeatureDetector {
private:
    static void print_feature(const char* name, long value, long required) {
        printf("%-40s ", name);
        if (value == 0) {
            printf("Not supported\n");
        } else if (value >= required) {
            printf("Supported (v%ld >= req%ld)\n", value, required);
        } else {
            printf("Partial support (v%ld < req%ld)\n", value, required);
        }
    }

public:
    static void detect_language_features() {
        std::cout << "\n=== Language Features ===\n";
        
#ifdef __cpp_constexpr
        print_feature("__cpp_constexpr", __cpp_constexpr, 200704L);
#endif
        
#ifdef __cpp_concepts
        print_feature("__cpp_concepts", __cpp_concepts, 201907L);
#endif
        
#ifdef __cpp_modules
        print_feature("__cpp_modules", __cpp_modules, 201907L);
#endif
        
#ifdef __cpp_if_consteval
        print_feature("__cpp_if_consteval", __cpp_if_consteval, 202106L);
#endif
    }
    
    static void detect_library_features() {
        std::cout << "\n=== Library Features ===\n";
        
#ifdef __cpp_lib_format
        print_feature("__cpp_lib_format", __cpp_lib_format, 201907L);
#endif
        
#ifdef __cpp_lib_ranges
        print_feature("__cpp_lib_ranges", __cpp_lib_ranges, 201911L);
#endif
        
#ifdef __cpp_lib_span
        print_feature("__cpp_lib_span", __cpp_lib_span, 202002L);
#endif
        
#ifdef __cpp_lib_concepts
        print_feature("__cpp_lib_concepts", __cpp_lib_concepts, 202002L);
#endif
    }
};

int main() {
    std::cout << "C++ Standard: " << __cplusplus << std::endl;
    
    FeatureDetector::detect_language_features();
    FeatureDetector::detect_library_features();
    
    return 0;
}
```

---

## 缺陷报告

以下行为改变的缺陷报告已追溯适用于先前发布的C++标准。

| 缺陷报告 | 适用标准 | 发布时的行为 | 正确行为 |
|----------|----------|--------------|----------|
| P2552R3 | C++20 | `__has_cpp_attribute`必须为标准属性展开为非零值 | 可以展开为0 |

---

## 相关链接

| 内容 | 说明 |
|------|------|
| **库特性测试宏** | C++20中定义在<version>头文件中 |
| **预定义宏符号** | 编译器预定义的宏 |
| **宏符号索引** | 所有宏符号的完整索引 |

### 外部链接

1. 特性测试推荐的官方文档
2. 源代码用于转储编译器特性

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/feature_test&oldid=179440>
- 最后修改时间：2025年1月9日 14:53
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++特性测试机制为现代C++开发提供了强大的工具，允许开发者编写能够适应不同编译器和标准库实现水平的可移植代码。从语言特性到库特性的全面检测，特性测试宏使得条件编译和向后兼容性处理变得更加简单和可靠。正确使用这些宏能够帮助开发者充分利用现代C++的先进特性，同时保持对旧版本编译器的支持。随着C++标准的不断演进，特性测试机制也在不断完善，为开发者提供更加精细的控制能力。掌握特性测试的最佳实践，是编写高质量、可维护的现代C++代码的重要技能。