# C++20 标准

来源：cppreference.com < cpp

## 标准概览

C++20 是继 C++17 之后的**主要版本 (major version)**，它引入了许多重大的新特性，包括**概念 (Concepts)、模块 (Modules)、协程 (Coroutines) 和范围 (Ranges)**，以及其他众多语言和库功能。该标准于 2020 年 12 月发布。

### C++ 语言版本演进

| 版本 | 发布年份 | 主要特点 |
|---|---|---|
| C++98 | 1998 | 第一个正式标准 |
| C++03 | 2003 | 对 C++98 的小幅修订 |
| C++11 | 2011 | 引入自动类型推导、移动语义、lambda 表达式、智能指针等 |
| C++14 | 2014 | 对 C++11 的小幅修订，增强泛型编程 |
| C++17 | 2017 | 结构化绑定、`if constexpr`、文件系统库等 |
| **C++20** | **2020** | **概念、模块、协程、范围等** |
| C++23 | 2023 | 标准化了更多现代 C++ 实践 |
| C++26 | 开发中 | 下一个主要版本 |

> **注意**：C++20 是一个革命性的版本，旨在提高 C++ 的表达能力、性能和易用性，特别是在泛型编程和并发编程方面。

## 新增核心语言特性

### 特性测试宏 (Feature test macros)

- **说明**：标准化了用于检测编译器是否支持特定 C++ 特性的宏（如 `__cpp_lib_concepts`），使代码更具可移植性。
- **示例**：
  ```cpp
  #if __cpp_lib_concepts >= 201907L
  // 使用概念
  #endif
  ```

### 三向比较运算符 `<=>` 和 `operator==() = default`

- **`<=>` (Spaceship Operator)**:
  - **说明**：提供了一种统一的比较方式，编译器可以自动生成 `<`, `<=`, `>`, `>=` 运算符。
  - **返回值**：返回一个表示 `<`, `=`, `>` 关系的特殊类型（`std::strong_ordering`, `std::weak_ordering`, `std::partial_ordering`）。
  - **示例**：
    ```cpp
    struct Point {
        int x, y;
        auto operator<=>(const Point&) const = default; // C++20 自动生成所有比较
    };
    ```
- **`operator==() = default`**:
  - **说明**：允许为聚合类型或类显式地要求编译器默认生成相等比较运算符。

### 指定初始化器 (Designated initializers)

- **说明**：从 C99 借鉴而来，允许通过指定成员名来初始化结构体，顺序可以任意。
- **示例**：
  ```cpp
  struct Point { int x, y; };
  Point p = { .y = 20, .x = 10 }; // 顺序无关
  ```

### 范围 `for` 循环中的 Init-语句和初始化器 (Init-statements and initializers in range-`for`)

- **说明**：允许在范围 `for` 循环的初始化部分声明和初始化变量。
- **示例**：
  ```cpp
  for (auto v = get_vector(); auto& element : v) {
      // 使用 element
  }
  ```

### `char8_t`

- **说明**：引入一个新的字符类型 `char8_t`，专门用于表示 UTF-8 编码单元，以区分 `char`（用途不明确）。

### 新属性 (`[[no_unique_address]]`, `[[likely]]`, `[[unlikely]]`)

- **`[[no_unique_address]]`**：
  - **说明**：用于指示非静态数据成员可以与其下一个分配的存储位置重叠（如果该成员没有唯一地址要求）。通常用于空基类优化 (EBO)。
  - **示例**：
    ```cpp
    struct Empty {};
    struct WithEBO {
        int i;
        [[no_unique_address]] Empty e; // e 可能不占用额外空间
    };
    ```
- **`[[likely]]` 和 `[[unlikely]]`**：
  - **说明**：为 `if`, `switch` 等语句提供分支预测提示。
  - **示例**：
    ```cpp
    if (condition) [[likely]] {
        // 预期为真的分支
    } else [[unlikely]] {
        // 预期为假的分支
    }
    ```

### Lambda 初始化捕获中的包展开 (Pack-expansions in lambda init-captures)

- **说明**：允许在 lambda 的初始化捕获 (`[x = expr]`) 中使用参数包展开。
- **示例**：
  ```cpp
  template<typename... Args>
  auto make_lambda(Args&&... args) {
      return [...args = std::forward<Args>(args)]() { /* 使用 args */ };
  }
  ```

### 放宽 `typename` 的使用要求 (Removed the requirement to use `typename` to disambiguate types in many contexts)

- **说明**：在许多上下文中，不再需要使用 `typename` 来指明模板参数是类型。

### `consteval` 和 `constinit`

- **`consteval` (立即函数)**：
  - **说明**：声明一个函数必须在编译时求值。比 `constexpr` 更严格。
  - **示例**：
    ```cpp
    consteval int square(int n) {
        return n * n;
    }

    constexpr int x = square(5); // 合法
    int y = square(5);          // 错误！必须在常量表达式中使用
    ```
- **`constinit` (常量初始化)**：
  - **说明**：确保变量具有静态或线程存储期，并且其初始化必须是常量初始化。
  - **示例**：
    ```cpp
    constinit static int global = 42; // 保证静态初始化
    ```

### 进一步放宽的 `constexpr`

- **说明**：允许在 `constexpr` 函数中使用 `try-catch` 块、动态分配内存（在编译时模拟）等。

### 有符号整数为二进制补码 (Signed integers are 2's complement)

- **说明**：标准现在明确要求有符号整数必须使用二进制补码表示，消除了平台差异。

### 统一的位移运算符行为 (Bitwise shift operators unified behavior)

- **说明**：明确了左移和右移运算符的行为，特别是对负数和溢出的处理。

### 使用圆括号进行聚合初始化 (aggregate initialization using parentheses)

- **说明**：允许使用圆括号 `()` 而不仅仅是花括号 `{}` 来初始化聚合类型。
- **示例**：
  ```cpp
  struct Point { int x, y; };
  Point p1{1, 2}; // C++17 之前
  Point p2(1, 2); // C++20 合法
  ```

### 协程 (Coroutines)

- **说明**：引入了对协程的原生支持，允许函数在执行过程中暂停并恢复。这是 C++ 并发和异步编程的重大进步。
- **关键概念**：
  - `co_await`: 挂起协程直到等待的操作完成。
  - `co_yield`: 从协程中产生一个值（用于生成器）。
  - `co_return`: 从协程中返回一个最终值。
- **用途**：实现生成器、任务、异步操作等。

### 模块 (Modules)

- **说明**：提供了一种新的代码组织和编译单元管理方式，旨在替代传统的头文件包含机制，提高编译速度和封装性。
- **示例**：
  ```cpp
  // math.ixx (模块接口单元)
  export module math;
  export int add(int a, int b) {
      return a + b;
  }

  // main.cpp
  import math;
  int main() {
      int result = add(1, 2);
  }
  ```

### 约束和概念 (Constraints and concepts)

- **说明**：一种模板元编程工具，允许对模板参数施加语义约束，提高错误信息的可读性和代码的表达能力。
- **示例**：
  ```cpp
  template<typename T>
  concept Integral = std::is_integral_v<T>;

  template<Integral T>
  T gcd(T a, T b) {
      // ...
  }
  ```

### 简化函数模板 (Abbreviated function template)

- **说明**：允许在函数参数声明中使用 `auto`，编译器会为其生成一个概念约束的模板。
- **示例**：
  ```cpp
  // C++17
  template<typename T>
  void func(const T& t);
  // C++20
  void func(const auto& t);
  ```

### DR11: `new` 表达式可以推导数组大小

- **说明**：允许在使用 `new` 分配数组时，让编译器从初始化器列表推导数组的大小。
- **示例**：
  ```cpp
  auto* arr = new int[]{1, 2, 3, 4, 5}; // 推导为 int[5]
  ```

## 新增标准库特性

### 新增头文件

#### 核心功能

- `<bit>`：位操作工具，如 `std::bit_cast`, `std::rotl`。
- `<compare>`：三向比较支持，定义了比较类别类型。
- `<concepts>`：标准库概念。
- `<coroutine>`：协程支持库。
- `<format>`：文本格式化库，现代化的 `printf`/`iostream` 替代品。
- `<numbers>`：数学常量，如 `std::numbers::pi`。
- `<ranges>`：范围库。
- `<source_location>`：源码位置信息。
- `<span>`：非拥有式连续对象序列视图。
- `<syncstream>`：同步输出流。
- `<version>`：标准库版本信息。

#### 线程支持库

- `<barrier>`：屏障，用于线程同步。
- `<latch>`：门闩，用于一次性线程同步。
- `<semaphore>`：信号量。
- `<stop_token>`：线程取消机制。

### 标准库功能增强

#### 库特性测试宏

- **说明**：为库功能提供特性测试宏。

#### 格式化库 (`<format>`)

- **说明**：提供了类型安全、可扩展、高性能的文本格式化功能。
- **示例**：
  ```cpp
  #include <format>
  std::string s = std::format("Hello, {}!", "World");
  ```

#### 概念库 (`<concepts>`)

- **说明**：提供了标准库中常用的概念，如 `std::integral`, `std::same_as`。

#### `<chrono>` 中的日历和时区库

- **说明**：提供了处理日期、时间点和时区的强大工具。
- **示例**：
  ```cpp
  #include <chrono>
  using namespace std::chrono;
  auto now = system_clock::now();
  zoned_time zt{current_zone(), now};
  ```

#### `std::source_location`

- **说明**：获取当前代码的文件名、行号、列号和函数名。
- **示例**：
  ```cpp
  void log(const std::source_location& loc = std::source_location::current()) {
      std::cout << "File: " << loc.file_name() << ", Line: " << loc.line() << std::endl;
  }
  ```

#### `std::span`

- **说明**：轻量级的、非拥有的数组视图，用于安全地传递数组信息。
- **示例**：
  ```cpp
  void process(std::span<int> data) {
      for (int i : data) { /* ... */ }
  }
  ```

#### `std::endian`

- **说明**：枚举类型，表示系统字节序 (`std::endian::big`, `std::endian::little`, `std::endian::native`)。

#### `<bit>` 中的整数 2 的幂次操作和 `std::bit_cast`

- **说明**：
  - `std::bit_cast`: 按位重新解释对象的类型，比 C 风格强制转换更安全。
  - `std::has_single_bit`, `std::bit_ceil`, `std::bit_floor`: 处理 2 的幂的相关函数。

#### `std::make_shared` 的数组支持

- **说明**：`std::make_shared<T[]>` 和 `std::make_shared<T[N]>`。

#### `std::remove_cvref`

- **说明**：类型特征，移除类型的 `const`、`volatile` 限定符和引用。

#### `std::to_address`

- **说明**：获取指针或智能指针底层的原始地址。

#### 浮点原子操作和 `std::shared_ptr` 原子操作

- **说明**：支持 `std::atomic<float>`, `std::atomic<shared_ptr<T>>` 等。

#### 线程协调类：`std::barrier`, `std::latch`, `std::counting_semaphore`

- **说明**：高级线程同步原语。

#### `std::jthread` 和线程取消类：`std::stop_token`, `std::stop_source`, `std::stop_callback`

- **说明**：
  - `std::jthread`: 可连接的线程，析构时会自动 `join`。
  - `stop_token` 等：提供线程取消的机制。

#### `std::basic_osyncstream`

- **说明**：同步的输出流，避免多线程输出时的交错。

#### `std::basic_string` 和其他 `char8_t` 的使用

- **说明**：标准库全面支持 `char8_t`。

#### `<algorithm>`, `<utility>`, `<complex>` 中的 `constexpr`

- **说明**：许多标准库算法和工具函数现在可以在 `constexpr` 上下文中使用。

#### `string::starts_with` / `ends_with` 和 `string_view::starts_with` / `ends_with`

- **说明**：检查字符串是否以指定前缀开始或以指定后缀结束。

#### `std::assume_aligned`

- **说明**：告知编译器指针是按指定字节对齐的，以进行优化。

#### `std::bind_front`

- **说明**：绑定函数对象的第一个（或前几个）参数。

#### `std::c8rtomb` / `std::mbrtoc8`

- **说明**：`char8_t` 和多字节字符之间的转换函数。

#### `std::make_obj_using_allocator` 等

- **说明**：使用分配器创建对象的工具函数。

#### `std::make_shared_for_overwrite` / `std::make_unique_for_overwrite`

- **说明**：创建未初始化对象的智能指针，用于性能优化。

#### 无序关联容器中的异构查找

- **说明**：允许使用与 `key_type` 不同的类型进行查找，而无需创建临时 `key_type` 对象。

#### `std::pmr::polymorphic_allocator` 的额外成员函数和 `std::byte` 作为默认模板参数

- **说明**：增强了多态内存资源的功能。

#### `execution::unseq`

- **说明**：执行策略，指示算法可以向量化执行。

#### `std::midpoint` 和 `std::lerp`

- **说明**：计算中点和线性插值的函数。

#### `std::ssize`

- **说明**：返回容器大小的有符号整数版本。

#### `std::is_bounded_array`, `std::is_unbounded_array`

- **说明**：类型特征，用于区分 `T[N]` 和 `T[]`。

#### 范围 (Ranges)

- **说明**：一个全新的库，提供了对迭代和算法的现代化、组合式的处理方式，极大地简化了算法和迭代器的使用。

#### 统一的容器擦除：`std::erase` / `std::erase_if`

- **说明**：为序列容器和关联容器提供了统一的擦除接口。
- **示例**：
  ```cpp
  std::vector<int> v = {1, 2, 3, 2, 4};
  std::erase(v, 2);        // 擦除所有值为 2 的元素
  std::erase_if(v, [](int i) { return i % 2 == 0; }); // 擦除所有偶数
  ```

#### `<numbers>` 中的数学常量

- **说明**：提供了常用的数学常量，如 `std::numbers::pi`, `std::numbers::e`。

## 缺陷报告 (Defect Reports)

C++20 修复了大量核心语言和标准库的缺陷报告。

### 修复的核心语言缺陷报告 (共 158 项)

- CWG#555, CWG#581, CWG#682, ..., CWG#2447

### 修复的标准库缺陷报告 (共 345 项)

- LWG#1052, LWG#1203, LWG#2040, ..., LWG#3398

## 编译器支持情况

### C++20 核心语言特性支持

（表格展示了 GCC、Clang、MSVC 等编译器对各项 C++20 特性的支持情况。）

### C++20 标准库特性支持

（表格展示了 GCC libstdc++、Clang libc++、MSVC STL 等标准库实现对各项 C++20 库特性的支持情况。）

---

**信息来源**：[https://en.cppreference.com/mwiki/index.php?title=cpp/20&oldid=178089](https://en.cppreference.com/mwiki/index.php?title=cpp/20&oldid=178089)

**最后修改时间**：2024年11月28日 16:44

**离线版本获取时间**：2025年2月9日 16:39
