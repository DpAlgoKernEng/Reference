# C++14 标准

来源：cppreference.com < cpp

## 标准概览

C++14 是在 C++11 这一重大版本之后的**次要版本 (minor version)**。它主要专注于对 C++11 功能的优化、改进和缺陷修复，而不是引入革命性的新特性。C++14 的批准于 2014 年 8 月 18 日宣布，并于 2014 年 12 月 15 日正式发布。

在批准之前，C++14 有时被称为 "C++1y"，表示其预计在 2010 年代发布。

### C++ 语言版本演进

| 版本 | 发布年份 | 主要特点 |
|---|---|---|
| C++98 | 1998 | 第一个正式标准 |
| C++03 | 2003 | 对 C++98 的小幅修订 |
| C++11 | 2011 | 引入自动类型推导、移动语义、lambda 表达式、智能指针等 |
| **C++14** | **2014** | **对 C++11 的小幅修订，增强泛型编程** |
| C++17 | 2017 | 结构化绑定、`if constexpr`、文件系统库等 |
| C++20 | 2020 | 概念、范围、协程、模块等 |
| C++23 | 2023 | 标准化了更多现代 C++ 实践 |
| C++26 | 开发中 | 下一个主要版本 |

> **注意**：C++14 是一个“点发布”（point release），旨在打磨和改进 C++11，使其更容易使用，并为 C++17 奠定基础。

## 新增核心语言特性

### 变量模板 (Variable templates)

- **说明**：允许定义模板化的变量。
- **用途**：简化类型特征（type traits）的使用，创建与类型相关的常量。
- **示例**：
  ```cpp
  // C++11 方式
  template<typename T>
  struct is_void {
      static const bool value = std::is_same<T, void>::value;
  };
  // 使用
  bool b = is_void<int>::value;

  // C++14 变量模板
  template<typename T>
  constexpr bool is_void_v = std::is_same_v<T, void>;
  // 使用
  bool b = is_void_v<int>;
  ```

### 泛型 Lambda 表达式 (Generic lambdas)

- **说明**：Lambda 表达式的参数可以使用 `auto` 类型说明符。
- **用途**：创建可接受多种类型参数的通用 lambda 函数，常用于算法。
- **示例**：
  ```cpp
  // C++11 Lambda
  auto lambda1 = [](int a, int b) { return a + b; };
  auto lambda2 = [](double a, double b) { return a + b; };

  // C++14 泛型 Lambda
  auto generic_lambda = [](auto a, auto b) { return a + b; };
  int i = generic_lambda(1, 2);        // int
  double d = generic_lambda(1.5, 2.5); // double
  std::string s = generic_lambda(std::string("a"), std::string("b")); // string
  ```

### Lambda 初始化捕获 (Lambda init-capture)

- **说明**：允许在 lambda 表达式的捕获列表中初始化捕获的变量。
- **用途**：可以移动一个对象到 lambda 中，或者捕获表达式的结果。
- **语法**：`[identifier = expression]` 或 `[&identifier = expression]`
- **示例**：
  ```cpp
  std::unique_ptr<int> ptr = std::make_unique<int>(42);
  
  // C++11 无法直接移动 ptr 到 lambda
  
  // C++14 初始化捕获 - 移动捕获
  auto lambda = [p = std::move(ptr)]() {
      // p 在 lambda 内部是 unique_ptr<int>
      std::cout << *p << std::endl;
  };
  
  // 捕获表达式结果
  int x = 10;
  auto lambda2 = [value = x * 2]() {
      std::cout << value << std::endl; // 输出 20
  };
  ```

### new/delete 省略 (new/delete elision)

- **说明**：在特定的优化场景下，允许编译器省略不必要的 `new` 和 `delete` 调用，例如当一个对象被 `new` 创建后立即被 `delete`，且中间没有其他使用时。
- **扩展解释**：这是一个编译器优化提示，旨在减少不必要的内存分配和释放开销。

### 放宽的 `constexpr` 函数限制 (Relaxed restrictions on constexpr functions)

- **说明**：C++11 对 `constexpr` 函数的要求非常严格（只能包含一条 `return` 语句等）。C++14 大幅放宽了这些限制。
- **C++14 允许**：
  - 函数体中可以包含声明、循环（`for`, `while`）、条件语句（`if`, `switch`）、`try`/`catch` 块（尽管不能在常量表达式中抛出异常）。
  - 可以修改局部变量和静态/线程局部变量。
- **示例**：
  ```cpp
  // C++11 constexpr 函数 (非常受限)
  constexpr int factorial_v11(int n) {
      return n <= 1 ? 1 : (n * factorial_v11(n - 1));
  }

  // C++14 constexpr 函数 (更灵活)
  constexpr int factorial_v14(int n) {
      int result = 1;
      for (int i = 2; i <= n; ++i) {
          result *= i;
      }
      return result;
  }

  constexpr int fact5 = factorial_v14(5); // 在编译时计算
  ```

### 二进制字面量 (Binary literals)

- **说明**：使用 `0b` 或 `0B` 前缀表示二进制整数字面量。
- **用途**：提高涉及位操作的代码的可读性。
- **示例**：
  ```cpp
  int flags = 0b10101010; // 等同于 0xAA
  ```

### 数字分隔符 (Digit separators)

- **说明**：使用单引号 `'` 作为数字字面量中的分隔符，提高可读性。
- **示例**：
  ```cpp
  int million = 1'000'000;
  long long big_number = 123'456'789'012'345LL;
  double pi = 3.141'592'653'589'793;
  ```

### 函数的返回类型推导 (Return type deduction for functions)

- **说明**：函数的返回类型可以使用 `auto` 让编译器推导。
- **语法**：`auto function_name(parameters) { return expression; }`
- **扩展**：对于递归函数，必须至少有一个 `return` 语句在递归调用之前，以便编译器能推导类型。
- **示例**：
  ```cpp
  // 简单情况
  auto add(int a, int b) {
      return a + b; // 返回类型被推导为 int
  }

  // 递归函数
  auto factorial(int n) {
      if (n <= 1) return 1; // 第一个 return 帮助推导类型
      return n * factorial(n - 1);
  }
  ```

### 带默认非静态成员初始值的聚合类 (Aggregate classes with default non-static member initializers)

- **说明**：聚合类（aggregate class，即没有用户提供的构造函数、私有/保护非静态成员、基类或虚函数的类）现在可以拥有带有默认初始值的非静态数据成员。
- **示例**：
  ```cpp
  // C++11 中这不是聚合类
  struct Point {
      int x = 0;
      int y = 0;
  };
  // Point p{1, 2}; // C++11 错误

  // C++14 中这是聚合类，可以使用列表初始化
  struct Point {
      int x = 0;
      int y = 0;
  };
  Point p{1, 2}; // C++14 正确
  Point p2{};    // x=0, y=0
  ```

## 新增标准库特性

### `std::make_unique`

- **说明**：为 `std::unique_ptr` 提供了对应的工厂函数，类似于 `std::make_shared`。
- **用途**：以异常安全的方式创建 `unique_ptr`，避免直接使用 `new`。
- **示例**：
  ```cpp
  // C++11 (需要直接 new)
  std::unique_ptr<int> ptr1(new int(42));
  // C++14 (推荐)
  auto ptr2 = std::make_unique<int>(42);
  auto ptr3 = std::make_unique<std::vector<int>>(10, 1);
  ```

### `std::shared_timed_mutex` 和 `std::shared_lock`

- **`std::shared_timed_mutex`**：
  - **说明**：一个支持超时的共享互斥锁（读写锁）。
  - **用途**：允许多个读线程同时访问共享资源，但写线程独占访问。
- **`std::shared_lock`**：
  - **说明**：用于获取共享互斥锁的共享锁（读锁）。
- **示例**：
  ```cpp
  std::shared_timed_mutex mtx;
  int shared_data = 0;

  // 读线程
  void reader() {
      std::shared_lock<std::shared_timed_mutex> lock(mtx);
      std::cout << "Data: " << shared_data << std::endl;
  }

  // 写线程
  void writer() {
      std::unique_lock<std::shared_timed_mutex> lock(mtx);
      shared_data++;
  }
  ```

### `std::integer_sequence`

- **说明**：一个模板工具，用于表示整数序列。常与参数包展开结合使用，实现高级模板元编程。
- **相关别名**：
  - `std::index_sequence`：`std::integer_sequence<size_t, ...>` 的别名。
  - `std::make_integer_sequence`
  - `std::make_index_sequence`
- **示例**：
  ```cpp
  template<typename T, T... Ints>
  void print_sequence(std::integer_sequence<T, Ints...>) {
      ((std::cout << Ints << " "), ...); // C++17 折叠表达式
      std::cout << std::endl;
  }

  print_sequence(std::make_index_sequence<5>{}); // 输出: 0 1 2 3 4
  ```

### `std::exchange`

- **说明**：一个实用函数，用新值替换对象的值，并返回其旧值。
- **用途**：在实现移动赋值运算符等场景中非常有用。
- **示例**：
  ```cpp
  class MyClass {
      std::unique_ptr<int> ptr_;
  public:
      MyClass& operator=(MyClass&& other) noexcept {
          ptr_ = std::exchange(other.ptr_, nullptr);
          return *this;
      }
  };
  ```

### `std::quoted`

- **说明**：一个 I/O 操纵符（manipulator），用于方便地处理带引号的字符串输入输出。
- **用途**：当字符串中包含空格时，可以将其作为一个整体读入或输出。
- **示例**：
  ```cpp
  std::string s = "Hello World";
  std::cout << std::quoted(s) << std::endl; // 输出: "Hello World"

  std::string s2;
  std::istringstream iss("\"Hello World\"");
  iss >> std::quoted(s2); // s2 被赋值为 "Hello World"
  ```

### 对现有库设施的许多小改进

#### 算法库的双范围重载 (Two-range overloads for some algorithms)

- **说明**：为 `std::equal`, `std::is_permutation`, `std::mismatch` 等算法增加了接受两个范围的重载版本，使其更易用。
- **示例**：
  ```cpp
  // C++11
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {1, 2, 3};
  bool eq = std::equal(v1.begin(), v1.end(), v2.begin()); // 需要手动指定第二个范围的开始

  // C++14
  bool eq = std::equal(v1.begin(), v1.end(), v2.begin(), v2.end()); // 更清晰
  ```

#### 类型特征的类型别名版本 (Type alias versions of type traits)

- **说明**：为许多 `<type_traits>` 中的模板提供了 `_t` 后缀的类型别名，简化语法。
- **示例**：
  ```cpp
  // C++11
  std::remove_const<const int>::type i = 42;
  // C++14
  std::remove_const_t<const int> j = 42;
  ```

#### `basic_string`, `duration` 和 `complex` 的用户定义字面量 (User-defined literals)

- **说明**：
  - `std::string` 字面量：`"hello world"s` 创建 `std::string`。
  - `std::chrono::duration` 字面量：`42ms`, `10s`, `5min` 等。
  - `std::complex` 字面量：`1i`, `3.14if` 等。
- **示例**：
  ```cpp
  using namespace std::literals; // 引入字面量后缀
  std::string s = "abc"s; // 等价于 std::string("abc")
  
  auto duration = 42ms + 1s; // 1042 毫秒
  auto complex_num = 3.0 + 4.0i; // std::complex<double>
  ```

## 缺陷报告 (Defect Reports)

C++14 修复了大量核心语言和标准库的缺陷报告。

### 修复的核心语言缺陷报告 (共 276 项)

- CWG#129, CWG#223, CWG#240, ..., CWG#1787

### 修复的标准库缺陷报告 (共 158 项)

- LWG#1214, LWG#1450, LWG#2003, ..., LWG#2360

## 编译器支持情况

### C++14 核心语言特性支持

（表格展示了 GCC、Clang、MSVC 等编译器对各项 C++14 特性的支持情况。）

### C++14 标准库特性支持

（表格展示了 GCC libstdc++、Clang libc++、MSVC STL 等标准库实现对各项 C++14 库特性的支持情况。）

---

**信息来源**：[https://en.cppreference.com/mwiki/index.php?title=cpp/14&oldid=178599](https://en.cppreference.com/mwiki/index.php?title=cpp/14&oldid=178599)

**最后修改时间**：2024年12月20日 14:13

**离线版本获取时间**：2025年2月9日 16:39