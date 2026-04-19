# 有用的资源（Useful resources）

来源：cppreference.com

## 标准C++基金会

**[isocpp.org](https://isocpp.org)** — 面向C++新闻、文章和活动的非营利性中心。

- **C++ FAQ** — 由标准C++基金会提供的统一FAQ。
- **邮件列表** — 官方公共邮件列表。

## C++常见问题解答

- **Bjarne Stroustrup FAQ** — 来自该语言创造者的常见问题解答。
- **Bjarne Stroustrup C++11 FAQ** — 关于C++语言标准最新变化的附加FAQ。
- **StackOverflow.com C++ FAQ** — 在程序员问答网站上关于C++常见问题的最佳答案。

## C++标准文档和草案

**JTC1/SC22/WG21** — C++标准委员会，包括C++语言标准草案、提案和缺陷报告。

| 文档 | 版本 | 日期 |
|------|------|------|
| N1146 (PDF) | C++98 | 最终工作草案 |
| N1804 (PDF) | C++早期版本 | 2005-04-27 |
| N3337 (PDF, HTML) | C++11 | 首个发布后草案 (2012-01-16) |
| N4140 (PDF, HTML) | C++14 | 最终工作草案 (2014-10-07) |
| N4659 (PDF, HTML) | C++17 | 最终工作草案 (2017-03-21) |
| N4861 (PDF, HTML) | C++20 | 最终工作草案 (2020-04-01) |
| N4950 (PDF, HTML) | C++23 | 最终工作草案 (2023-05-10) |
| N5001 (PDF) | C++最新草案 | 2024-12-17 |

- **cplusplus/draft** — 当前草案的官方GitHub仓库。
- **eel.is/c++draft** — 当前草案的HTML版本。

## C++语言和库参考

### 商业实现参考
- **Apache** — Apache C++标准库实现的参考手册。
- **IBM AIX 16.1 IBM zOS 3.1.0** — IBM C和C++核心语言实现的参考手册。
- **IBM AIX 16.1** — IBM C++标准库实现的参考手册。
- **MSDN** — Microsoft C++核心语言实现的参考手册。
- **MSDN** — Microsoft C++库实现的参考手册。
  - **Dinkumware** — 与Microsoft Visual C++一起提供的C++标准库实现。
  - **Open STL** — MSVC的现代C++标准库实现。

### 开源实现参考
- **GNU libstdc++** — GNU C++库实现的参考手册。
  - **libstdc++-v3** — GNU C++库实现。
- **libc++ documentation** — clang/llvm项目的C++标准库实现文档。
  - **libc++** — clang/llvm的C++标准库实现。
- **HP** — Dinkumware C++库实现的参考手册（授权给HP的版本）。
- **EWL** — Embedded Warrior C++库实现的参考手册。

### 社区资源
- **cplusplus.com** — 非官方C++标准库参考。
- **SGI STL** (托管在rrsd.com) — STL参考，1994年引入许多后来被C++标准库采用的概念的库。

## C++应用程序二进制接口（ABI）

**Itanium C++ ABI** — 事实上的C++ ABI标准，被以下标准采用：

- **System V ABI**
- **ARM C++ ABI**

## 模式、惯用法、技巧和窍门

- **C++ Core Guidelines** — 由Bjarne Stroustrup和Herb Sutter编写。
- **C++ Patterns** — 社区创建的现代C++示例。
- **More C++ Idioms** — 在wikibooks.org上的内容。
- **CeePlusPlusIdioms** — 在c2.com上的内容。
- **Welcome Back to C++** — 学习现代C++（MSDN）。

## 相关标准

- **JTC1/SC22/WG14** — C标准委员会，包括C语言标准草案、提案和缺陷报告。
- **POSIX.1-2024/IEEE Std 1003.1-2024** — Unix兼容系统的操作系统API。
- **The Unicode Standard** — 书面字符的通用编码标准。

## 其他有用资源

- **C++17 in Tony Tables** — 带示例的C++17新特性。

> ⚠️ **注意**：此部分不完整
> 原因：可能需要添加一些不太正式但有影响力的 文章、博客、网站。

## 相关页面

| 页面 | 说明 |
|------|------|
| 非ANSI/ISO C++库列表 | |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/links&oldid=180171>
- 最后修改时间：2025年2月5日 00:11
- 离线版本获取时间：2025年2月9日 16:39

---

## 扩展知识详解

### 一、标准文档的重要性

C++标准文档是理解和使用C++语言的权威来源。这些文档包含了语言的核心特性、标准库规范以及最新的语言扩展。

#### 1. 如何使用标准文档
```cpp
// 示例：理解标准库行为
// 当遇到标准库函数行为不明确时，查阅N4950等标准文档
#include <vector>
#include <algorithm>

void standard_compliant_example() {
    // 根据C++标准，vector::erase返回指向删除元素后一个元素的迭代器
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.erase(vec.begin() + 2);  // 删除元素3
    // it现在指向元素4，这是标准规定的行为
}
```

#### 2. 标准演进跟踪
```cpp
// C++版本检查示例
#if __cplusplus >= 202302L
    // C++23特性
    #include <print>
#elif __cplusplus >= 202002L
    // C++20特性
    #include <concepts>
#elif __cplusplus >= 201703L
    // C++17特性
    #include <filesystem>
#elif __cplusplus >= 201402L
    // C++14特性
    #include <memory>
#elif __cplusplus >= 201103L
    // C++11特性
    #include <type_traits>
#else
    // C++98/03特性
#endif
```

### 二、开源实现学习

#### 1. GNU libstdc++学习
```cpp
// 理解STL容器实现原理
#include <vector>
#include <memory>

// 参考libstdc++源码理解vector的内存管理策略
template<typename T>
class SimpleVector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

public:
    SimpleVector() : data_(nullptr), size_(0), capacity_(0) {}
    
    ~SimpleVector() {
        delete[] data_;
    }
    
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }
    
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }
};
```

#### 2. libc++ vs libstdc++对比
```cpp
// 头文件包含方式差异示例
#ifdef _LIBCPP_VERSION
    // libc++特定代码
    #include <__algorithm>
#elif defined(__GLIBCXX__)
    // libstdc++特定代码
    #include <bits/stl_algobase.h>
#endif
```

### 三、现代C++学习资源

#### 1. C++ Core Guidelines应用
```cpp
// 遵循C++ Core Guidelines的示例
#include <memory>
#include <vector>

// ES.20: Always initialize an object
class MyClass {
public:
    // C.41: A constructor should create a fully initialized object
    MyClass(int value) : value_(value) {
        // 确保对象完全初始化
    }
    
    // C.128: Virtual functions should specify exactly one of virtual, override, or final
    virtual void virtual_function() {
        // 基类虚函数
    }
    
private:
    int value_{0};  // ES.20: 使用初始化列表
};

// R.11: Avoid calling new and delete explicitly
void resource_management_example() {
    // 使用智能指针而非原始指针
    auto ptr = std::make_unique<MyClass>(42);
    
    // 使用容器而非原始数组
    std::vector<int> numbers = {1, 2, 3, 4, 5};
}
```

#### 2. 现代C++模式应用
```cpp
// 应用现代C++惯用法
#include <optional>
#include <variant>
#include <any>

// 使用std::optional处理可能不存在的值
std::optional<int> find_value(const std::vector<int>& vec, int target) {
    auto it = std::find(vec.begin(), vec.end(), target);
    if (it != vec.end()) {
        return *it;
    }
    return std::nullopt;
}

// 使用std::variant处理多种类型
using Value = std::variant<int, double, std::string>;

void variant_example() {
    Value v = 42;
    // 类型安全的值处理
    std::visit([](const auto& value) {
        std::cout << "Value: " << value << std::endl;
    }, v);
}
```

### 四、跨平台兼容性考虑

#### 1. ABI兼容性
```cpp
// 理解ABI差异对库开发的影响
#ifdef __itanium_abi
    // Itanium ABI相关代码
#elif defined(_MSC_VER)
    // Microsoft ABI相关代码
#endif

// 导出符号兼容性
#ifdef _WIN32
    #define API_EXPORT __declspec(dllexport)
#else
    #define API_EXPORT __attribute__((visibility("default")))
#endif
```

#### 2. 编译器兼容性
```cpp
// 编译器特性检测
#if defined(__GNUC__) && (__GNUC__ >= 9)
    // GCC 9及以上特性
#elif defined(_MSC_VER) && (_MSC_VER >= 1920)
    // MSVC 2019及以上特性
#elif defined(__clang__) && (__clang_major__ >= 10)
    // Clang 10及以上特性
#endif
```

### 五、社区资源利用

#### 1. Stack Overflow最佳实践学习
```cpp
// 应用社区最佳实践
#include <iostream>
#include <string>
#include <vector>

// RAII原则应用
class ResourceWrapper {
public:
    ResourceWrapper(const std::string& name) : name_(name) {
        std::cout << "Acquired resource: " << name_ << std::endl;
    }
    
    ~ResourceWrapper() {
        std::cout << "Released resource: " << name_ << std::endl;
    }
    
    // 禁止拷贝，允许移动（C++11惯用法）
    ResourceWrapper(const ResourceWrapper&) = delete;
    ResourceWrapper& operator=(const ResourceWrapper&) = delete;
    
    ResourceWrapper(ResourceWrapper&& other) noexcept 
        : name_(std::move(other.name_)) {
        other.name_.clear();
    }
    
    ResourceWrapper& operator=(ResourceWrapper&& other) noexcept {
        if (this != &other) {
            name_ = std::move(other.name_);
            other.name_.clear();
        }
        return *this;
    }

private:
    std::string name_;
};
```

#### 2. 设计模式应用
```cpp
// 应用C++惯用设计模式
#include <memory>

// CRTP (Curiously Recurring Template Pattern)惯用法
template<typename Derived>
class EnableSharedFromThis {
public:
    std::shared_ptr<Derived> shared_from_this() {
        return std::static_pointer_cast<Derived>(
            shared_from_this_impl()
        );
    }
    
private:
    std::shared_ptr<void> shared_from_this_impl() {
        return weak_ptr_.lock();
    }
    
    std::weak_ptr<void> weak_ptr_;
    
    // 友元类允许std::shared_ptr访问私有构造函数
    template<typename T>
    friend std::shared_ptr<T> std::make_shared<T>();
};

class MyClass : public EnableSharedFromThis<MyClass> {
public:
    void do_something() {
        auto self = shared_from_this();
        // 安全地传递shared_ptr
    }
};
```

### 六、持续学习建议

#### 1. 跟踪标准发展
```cpp
// 使用特性测试宏跟踪新特性
#include <version>

#ifdef __cpp_lib_concepts
    // Concepts可用
    #include <concepts>
#endif

#ifdef __cpp_lib_format
    // std::format可用
    #include <format>
#endif

#ifdef __cpp_lib_jthread
    // std::jthread可用
    #include <thread>
#endif
```

#### 2. 实践社区模式
```cpp
// 实现社区推荐的最佳实践
#include <type_traits>
#include <utility>

// SFINAE惯用法实现类型检查
template<typename T>
struct has_value_type {
private:
    template<typename U>
    static auto test(int) -> decltype(std::declval<typename U::value_type>(), std::true_type{});
    
    template<typename>
    static std::false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

// 现代替代方案：使用concept（C++20）
#if __cplusplus >= 202002L
template<typename T>
concept has_value_type_v = requires {
    typename T::value_type;
};
#endif
```

✅ C++学习资源生态丰富，从官方标准文档到社区最佳实践，为开发者提供了全面的学习路径。深入理解标准文档有助于编写符合规范的代码，学习开源实现可以提升技术深度，应用核心指南确保代码质量，跟踪社区发展保持技术前沿。正确利用这些资源，结合实践应用现代C++惯用法和设计模式，能够显著提升C++开发技能。持续学习和实践是掌握现代C++的关键，建议开发者定期查阅标准文档，参与社区讨论，实践最佳实践，以保持技术竞争力。