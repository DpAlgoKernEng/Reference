# C++26 标准

来源：cppreference.com < cpp

## 标准概览

C++26 是下一代 C++ 标准。

C++26 的当前国际标准 (IS) 计划：P1000R6 (2024-05-14)。

### C++ 语言版本演进

| 版本 | 发布年份 | 主要特点 |
|---|---|---|
| C++98 | 1998 | 第一个正式标准 |
| C++03 | 2003 | 对 C++98 的小幅修订 |
| C++11 | 2011 | 引入自动类型推导、移动语义、lambda 表达式、智能指针等 |
| C++14 | 2014 | 对 C++11 的小幅修订，增强泛型编程 |
| C++17 | 2017 | 结构化绑定、`if constexpr`、文件系统库等 |
| C++20 | 2020 | 概念、模块、协程、范围等 |
| C++23 | 2023 | 标准化了更多现代 C++ 实践 |
| **C++26** | **开发中** | **下一代 C++ 标准** |
| Current status | | |

> **注意**：C++26 仍在积极开发中，此处列出的特性是提案或已接受的特性，最终标准可能会有所不同。

## 新增核心语言特性

（此部分在原文中为空，但下文表格中包含了许多特性）

## 新增标准库特性

### 新模块

### 新增头文件

#### 核心功能

- `<debugging>`：调试支持库。
- `<hazard_pointer>`：危险指针（一种无锁编程技术）。
- `<inplace_vector>`：具有固定容量的动态可调整大小的向量。
- `<linalg>`：基于 BLAS 的自由函数线性代数接口。
- `<rcu>`：Read-Copy Update（读-拷贝-更新，一种并发编程技术）。
- `<simd>`：数据并行类型。
- `<text_encoding>`：文本编码识别。

#### C 兼容性头文件

- `<stdbit.h>`：来自 C23 的标准位操作。
- `<stdchkint.h>`：来自 C23 的整数溢出检查。

## 已废弃/移除的特性

### 被移除的特性 (Removed features)

（此部分在原文中为空）

### 被弃用的特性 (Deprecated features)

（此部分在原文中为空）

## 缺陷报告 (Defect Reports)

（此部分在原文中为空）

## 编译器支持情况

### C++26 核心语言特性支持

（表格展示了 GCC、Clang、MSVC 等编译器对各项 C++26 特性的支持情况。以下是一些重点特性的说明）

#### 核心语言特性亮点

- **包索引 (`Pack indexing`) (P2662R3)**：
  - **说明**：提供了一种新的语法 `Pack...[I]` 来访问参数包中的第 I 个元素，比传统的递归模板元编程或 `std::get` 更简洁。
  - **示例**：
    ```cpp
    template<typename... Ts>
    void func(Ts... args) {
        // 获取第二个参数
        auto second = [&]<std::size_t I>(std::integral_constant<std::size_t, I>) {
            return args...[I];
        }(std::integral_constant<std::size_t, 1>{});
    }
    // C++26 可能的简化语法 (如果标准化):
    // auto second = args...[1];
    ```

- **删除的算术转换枚举 (`Removing deprecated arithmetic conversion on enumerations`) (P2864R2)**：
  - **说明**：移除 C++98 中允许枚举类型隐式转换为其底层整型类型的算术转换规则。这可以提高类型安全性，防止意外的类型混用。
  - **影响**：`enum class E { A, B }; int i = E::A; // 错误！需要显式转换`

- **对不完整类型指针的删除应为非法 (`Deleting a pointer to an incomplete type should be ill-formed`) (P3144R2)**：
  - **说明**：`delete ptr;` 如果 `ptr` 指向一个不完整类型（incomplete type），现在应被视为编译错误，而不是未定义行为。这有助于在编译时捕获错误。

- **带有属性的结构化绑定 (`Attributes for structured bindings`) (P0609R3)**：
  - **说明**：允许为结构化绑定中的每个标识符添加属性。
  - **示例**：
    ```cpp
    auto [x [[maybe_unused]], y] = std::make_pair(1, 2);
    ```

- **`= delete("reason");`** (P2573R2)：
  - **说明**：允许为被删除的函数提供一个字符串字面量作为原因，编译器在尝试调用该函数时报错时会包含这个原因。
  - **示例**：
    ```cpp
    struct MyClass {
        void old_method() = delete("Use new_method instead.");
    };
    ```

- **变参友元 (`Variadic friends`) (P2893R3)**：
  - **说明**：允许声明变参模板友元函数。
  - **示例**：
    ```cpp
    template<typename T>
    class MyClass {
        template<typename... Args>
        friend void friend_func(Args&&... args); // 变参友元
    };
    ```

### C++26 标准库特性支持

（表格展示了 GCC libstdc++、Clang libc++、MSVC STL 等标准库实现对各项 C++26 库特性的支持情况。以下是一些重点特性的说明）

#### 标准库特性亮点

- **`std::text_encoding` (P1885R12)**：
  - **说明**：提供一种标准化的方式来识别和处理不同的文本编码（如 UTF-8, UTF-16, ISO-8859-1 等）。

- **`std::function_ref` (P0792R14)**：
  - **说明**：一个轻量级、非拥有式的可调用对象包装器，类似于 `std::function` 但不进行类型擦除的动态分配。它只持有对现有可调用对象的引用，开销极小。
  - **用途**：用于函数参数，当您只需要传递一个可调用对象且保证其在调用期间有效时。
  - **示例**：
    ```cpp
    void apply_operation(int& value, std::function_ref<void(int&)> op) {
        op(value);
    }
    ```

- **`std::inplace_vector` (P0843R14)**：
  - **说明**：一个固定容量的动态数组容器，其存储空间在对象内部（on-stack 或 in-place），类似于 `std::array` 但大小可变（在容量范围内）。
  - **优势**：避免了动态内存分配，性能更好，适用于小数据集。

- **`std::span::at()` (P2821R5)**：
  - **说明**：为 `std::span` 添加了带边界检查的元素访问方法 `at()`，类似于 `std::vector::at()`。

- **字符串和字符串视图的连接 (`Concatenation of strings and string views`) (P2591R5)**：
  - **说明**：简化了 `std::string`, `std::string_view`, C 风格字符串之间的连接操作。

- **`std::ranges::generate_random` (P1068R11)**：
  - **说明**：一个范围算法，用于生成随机数序列。

- **`views::concat` (P2542R8)**：
  - **说明**：一个范围适配器，用于将多个范围连接成一个范围。

- **执行库 (`Execution library`) (P2300R10)**：
  - **说明**：一个强大的异步编程和执行模型库，旨在提供比 `<future>` 和 `<thread>` 更高级、更灵活的并发原语。包括 `std::execution::sender`, `std::execution::receiver`, `std::execution::scheduler` 等概念。

- **`std::simd` (P1928R15)**：
  - **说明**：一个用于数据并行编程的库，旨在简化 SIMD（单指令多数据）指令的使用，提高数值计算性能。

- **`std::atomic_ref::address()` (P2835R7)**：
  - **说明**：获取 `std::atomic_ref` 所引用对象的地址。

- **`std::aligned_accessor` (P2897R7)**：
  - **说明**：为 `std::mdspan` 提供一个访问器，用于表达指针对齐的要求。

- **`<debugging>` (P2546R5)**：
  - **说明**：提供标准化的调试支持设施。

- **`std::linalg` (P1673R13)**：
  - **说明**：线性代数库，提供与 BLAS 兼容的接口。

---

**信息来源**：[https://en.cppreference.com/mwiki/index.php?title=cpp/26&oldid=179945](https://en.cppreference.com/mwiki/index.php?title=cpp/26&oldid=179945)

**最后修改时间**：2025年1月28日 16:32

**离线版本获取时间**：2025年2月9日 16:39
