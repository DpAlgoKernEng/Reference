# C++17 标准

来源：cppreference.com < cpp

## 标准概览

C++17 是继 C++14 这一较小更新之后的**主要版本 (major version)**。它引入了许多重要的新语言特性和库功能。该标准于 2017 年 12 月发布。

以下特性被合并到了 C++17 中：

- **文件系统 TS**：文件系统库 (`<filesystem>`)。
- **库基础 v1 TS**：包括 `std::any`, `std::optional`, `std::string_view`, `std::apply`, 多态分配器, 搜索器等特性。
- **库基础 v2 TS**：`std::void_t`, `std::conjunction`, `std::disjunction`, `std::negation`, `std::not_fn`, `std::gcd`, `std::lcm`。
- **并行算法 v1 TS**：包括执行策略、`std::reduce`, `std::inclusive_scan`, `std::exclusive_scan`，但移除了 `exception_list`。
- **数学特殊函数 IS**：数学特殊函数。
- **C11**：`std::aligned_alloc`, `std::timespec_get`。

### C++ 语言版本演进

| 版本 | 发布年份 | 主要特点 |
|---|---|---|
| C++98 | 1998 | 第一个正式标准 |
| C++03 | 2003 | 对 C++98 的小幅修订 |
| C++11 | 2011 | 引入自动类型推导、移动语义、lambda 表达式、智能指针等 |
| C++14 | 2014 | 对 C++11 的小幅修订，增强泛型编程 |
| **C++17** | **2017** | **结构化绑定、`if constexpr`、文件系统库等** |
| C++20 | 2020 | 概念、范围、协程、模块等 |
| C++23 | 2023 | 标准化了更多现代 C++ 实践 |
| C++26 | 开发中 | 下一个主要版本 |

> **注意**：C++17 专注于提高语言和库的实用性，许多特性旨在简化代码编写和提高性能。

## 已废弃/移除的特性

### 被移除的特性 (Removed features)

#### `std::auto_ptr`

- **说明**：在 C++11 中已被弃用的智能指针。由于其危险的拷贝语义和与移动语义不兼容，在 C++17 中被正式移除。
- **替代方案**：使用 `std::unique_ptr`。

#### 已弃用的函数对象

- **说明**：指旧的函数对象包装器，如 `std::unary_function` 和 `std::binary_function`。它们的功能已被更现代的类型特征（如 `std::is_invocable`）取代。

#### `std::random_shuffle`

- **说明**：用于随机打乱序列的函数，因其使用默认的随机数生成器而被认为是不安全或不可控的。
- **替代方案**：使用 `std::shuffle` 并传入一个用户指定的统一随机数生成器（URNG）。

#### `std::unexpected`

- **说明**：用于处理未预期的异常，与旧的动态异常规范（dynamic exception specification）相关。
- **替代方案**：使用 `std::terminate` 或自定义的未处理异常处理机制，配合 `noexcept`。

#### 过时的 `iostream` 别名

- **说明**：一些旧的 `iostream` 类型别名，如 `iostream streambuf_type` 等，因其模糊性被移除。

#### 三字符组 (Trigraphs)

- **说明**：用于在源代码中用三个字符序列表示单个字符（如 `??=` 表示 `#`），主要为了解决某些键盘布局无法输入特定字符的问题。现代编译器和编辑器已普遍支持所需字符。
- **替代方案**：直接使用字符。

#### `register` 关键字

- **说明**：提示编译器将变量存储在寄存器中以提高访问速度。现代编译器的优化能力已经非常强大，能够自行做出最佳决策。
- **替代方案**：将其移除，不产生任何效果。

#### `bool` 类型的自增 (`bool` increment)

- **说明**：允许对 `bool` 类型变量进行 `++` 运算。这被认为是一个设计缺陷，因为 `bool` 的语义是真/假。
- **替代方案**：使用显式的逻辑运算 `b = true;`。

#### 动态异常规范 (Dynamic exception specification)

- **说明**：`throw(typeid, ...)` 这种在函数声明中指定可能抛出的异常类型的语法。它比 `noexcept` 更复杂且效率更低。
- **替代方案**：使用 `noexcept` 或 `noexcept(false)`。

### 被弃用的特性 (Deprecated features)

#### `std::iterator`

- **说明**：一个辅助基类，用于简化迭代器的定义。由于现代 C++ 更倾向于使用类型特征和概念（C++20），它被弃用。
- **替代方案**：直接定义迭代器所需的类型别名（`value_type`, `difference_type` 等）。

#### `std::raw_storage_iterator`

- **说明**：用于在未初始化的内存中构造对象。被更现代的工具如 `<memory>` 中的 `uninitialized_*` 算法和 `std::construct_at` (C++20) 所取代。

#### `std::get_temporary_buffer` 和 `std::return_temporary_buffer`

- **说明**：用于分配临时内存。它们的设计存在缺陷，容易导致内存泄漏。
- **替代方案**：使用标准容器或 `std::allocator`。

#### `std::is_literal_type`

- **说明**：检查类型是否为字面量类型。其定义在 C++14 和 C++17 中发生了变化，导致其用途变得模糊。
- **替代方案**：使用 `std::is_standard_layout`, `std::is_trivial` 等更具体的类型特征。

#### `std::result_of`

- **说明**：用于推导函数调用表达式的返回类型。它在处理某些复杂情况时行为不明确。
- **替代方案**：使用 `std::invoke_result` (C++17 引入)。

#### 整个 `<codecvt>` 头文件

- **说明**：提供字符编码转换的设施。由于其接口设计不完善和实现复杂性，整个头文件被弃用。
- **替代方案**：使用第三方库（如 ICU）或 C++20 中的 `<format>` 等更现代的设施（如果适用）。

## 新增核心语言特性

### `u8` 字符字面量 (`u8` character literal)

- **说明**：使用 `u8` 前缀创建一个 UTF-8 编码的字符字面量，其类型为 `char8_t` (C++20) 或 `char` (C++17)。
- **示例**：
  ```cpp
  char8_t ch = u8'A'; // 字符 'A'
  ```

### 使 `noexcept` 成为类型系统的一部分 (Made noexcept part of type system)

- **说明**：函数指针和函数的 `noexcept` 说明符现在是其类型的一部分。
- **用途**：
  1. 提高类型安全性，防止将 `noexcept(false)` 的函数赋值给 `noexcept(true)` 的函数指针。
  2. 允许编译器基于 `noexcept` 进行更多优化（如选择移动构造函数而非拷贝构造函数）。
- **示例**：
  ```cpp
  void func1() noexcept;
  void func2();

  void (*p1)() noexcept = func1; // 合法
  // void (*p2)() noexcept = func2; // 错误！func2 可能抛出异常
  ```

### 新的求值顺序规则 (New order of evaluation rules)

- **说明**：C++17 定义了某些表达式操作数的求值顺序，解决了历史遗留的未定义行为问题。
- **规则**：
  1. `a @ b` (`@` 是 `&&`, `||`, `,` 运算符): `a` 总是在 `b` 之前被求值。
  2. `a[b]`, `a->b`, `a.b`: `a` 总是在 `b` 之前被求值。
  3. `f(a, b, c)`: `f` 和所有参数 `a`, `b`, `c` 的求值顺序是**未指明的**，但每个参数内部的求值顺序是确定的。
- **示例**：
  ```cpp
  std::vector<int> v(1);
  v[v.size() - 1] = 42; // C++17 之前行为是未定义的，现在是安全的
  ```

### Lambda 捕获 `*this` (lambda capture of \*this)

- **说明**：允许在 lambda 表达式中按值捕获当前对象（而不是默认的按引用捕获 `this`）。
- **用途**：当 lambda 需要在当前对象被销毁后仍然可以安全调用其成员函数时非常有用。
- **示例**：
  ```cpp
  class MyClass {
      int value = 10;
  public:
      auto get_lambda() {
          // C++11/14 方式 - 捕获 this 指针
          // return [=]() { return value; }; // 可能在对象销毁后访问悬空指针

          // C++17 方式 - 按值捕获 *this
          return [*this]() { return value; };
      }
  };
  ```

### `constexpr`

#### 编译时 `if` (`if constexpr`)

- **说明**：条件为 `constexpr` 的 `if` 语句。在编译时，不满足条件的分支会被完全丢弃，不会参与模板实例化。
- **用途**：极大地简化了模板元编程，特别是编写通用代码。
- **示例**：
  ```cpp
  template<typename T>
  void func(T t) {
      if constexpr (std::is_integral_v<T>) {
          std::cout << "Integral: " << t << std::endl;
      } else if constexpr (std::is_floating_point_v<T>) {
          std::cout << "Floating point: " << t << std::endl;
      } else {
          std::cout << "Other type" << std::endl;
      }
  }
  ```

#### `constexpr` Lambda 表达式 (`constexpr lambda`)

- **说明**：在 `constexpr` 函数或上下文中定义的 lambda 表达式会隐式地是 `constexpr`。
- **示例**：
  ```cpp
  constexpr auto make_adder(int offset) {
      return [offset](int n) { return n + offset; }; // 隐式 constexpr
  }

  constexpr auto add_5 = make_adder(5);
  constexpr int result = add_5(10); // 编译时计算
  ```

### 变量

#### 内联变量 (`inline variables`)

- **说明**：允许在头文件中定义变量，并使用 `inline` 关键字确保其在所有翻译单元中的唯一性，避免重复定义错误。
- **用途**：用于定义常量、静态成员变量等，简化了之前需要在源文件中定义的繁琐过程。
- **示例**：
  ```cpp
  // header.hpp
  struct MyClass {
      static inline const int constant = 42; // C++17
      // C++14 等价写法: static constexpr int constant = 42; (隐式 inline)
  };

  inline const std::string global_config = "config.xml"; // 头文件中定义全局常量
  ```

#### 结构化绑定 (`structured bindings`)

- **说明**：允许将数组、结构体、元组等包含多个元素的对象“解包”到多个独立的变量中。
- **语法**：`auto [var1, var2, ...] = expression;`
- **示例**：
  ```cpp
  // 解包数组
  int arr[3] = {1, 2, 3};
  auto [a, b, c] = arr; // a=1, b=2, c=3

  // 解包结构体
  struct Point { int x, y; };
  Point p = {10, 20};
  auto [x, y] = p; // x=10, y=20

  // 解包 std::pair / std::tuple
  std::map<int, std::string> m = {{1, "one"}, {2, "two"}};
  for (const auto& [key, value] : m) {
      std::cout << key << " => " << value << std::endl;
  }

  std::tuple<int, double, std::string> t = {42, 3.14, "hello"};
  auto [i, d, s] = t; // i=42, d=3.14, s="hello"
  ```

#### `if` 和 `switch` 的初始化器 (`initializers for if and switch`)

- **说明**：允许在 `if` 或 `switch` 语句的条件部分声明并初始化一个变量。
- **语法**：`if (init-statement; condition)` 或 `switch (init-statement; condition)`
- **用途**：使作用域更清晰，减少变量泄漏。
- **示例**：
  ```cpp
  // C++14 方式
  {
      auto result = some_operation();
      if (result.valid()) {
          use(result);
      }
  } // result 的作用域结束

  // C++17 方式
  if (auto result = some_operation(); result.valid()) {
      use(result);
  } // result 的作用域仅限于 if 语句
  ```

#### 保证的拷贝省略 (`guaranteed copy elision`)

- **说明**：在特定场景下（如从函数返回临时对象，或用临时对象初始化对象），编译器必须省略拷贝/移动构造函数的调用，即使这些构造函数有副作用或被删除。
- **用途**：保证性能，允许返回不可移动或不可拷贝的对象。
- **示例**：
  ```cpp
  struct Immovable {
      Immovable() = default;
      Immovable(const Immovable&) = delete; // 禁止拷贝
      Immovable(Immovable&&) = delete;      // 禁止移动
  };

  Immovable factory() {
      return Immovable{}; // C++17 及以后保证合法，即使没有移动构造函数
  }

  Immovable obj = factory(); // C++17 及以后保证合法
  ```

#### 临时物化 (`temporary materialization`)

- **说明**：一个语义上的概念，指表达式结果被转换为一个临时对象。这与保证拷贝省略紧密相关。

### 模板

#### 折叠表达式 (`fold-expressions`)

- **说明**：对参数包（parameter pack）中的所有参数应用一个二元操作符。
- **语法**：
  - 一元右折叠：`(pack op ...)`
  - 一元左折叠：`(... op pack)`
  - 二元右折叠：`(pack op ... op init)`
  - 二元左折叠：`(init op ... op pack)`
- **示例**：
  ```cpp
  template<typename... Args>
  void print_args(Args&&... args) {
      (std::cout << ... << args) << '\n'; // 右折叠，输出所有参数
  }

  template<typename... Args>
  auto sum(Args&&... args) {
      return (args + ...); // 右折叠
  }
  ```

#### 类模板实参推导 (Class Template Argument Deduction, CTAD)

- **说明**：允许在创建类模板实例时省略模板参数，让编译器根据构造函数的实参自动推导。
- **示例**：
  ```cpp
  // C++14
  std::pair<int, double> p1(1, 3.14);
  std::vector<int> v1{1, 2, 3};

  // C++17
  std::pair p2(1, 3.14);        // 推导为 std::pair<int, double>
  std::vector v2{1, 2, 3};      // 推导为 std::vector<int>
  std::tuple t(4, 3, 2.5);      // 推导为 std::tuple<int, int, double>
  ```

#### 使用 `auto` 声明的非类型模板参数 (`non-type template parameters declared with `auto``)

- **说明**：非类型模板参数可以使用 `auto` 让编译器推导其类型。这使得模板更加通用。
- **示例**：
  ```cpp
  template<auto N> // N 的类型由传递的值决定
  struct Constant {
      static constexpr auto value = N;
  };

  Constant<5> c1;     // N is int
  Constant<'A'> c2;   // N is char
  ```

### 命名空间

#### 简化的嵌套命名空间 (`simplified nested namespaces`)

- **说明**：允许使用 `::` 直接定义嵌套的命名空间。
- **示例**：
  ```cpp
  // C++14 方式
  namespace A {
      namespace B {
          namespace C {
              void func() {}
          }
      }
  }

  // C++17 方式
  namespace A::B::C {
      void func() {}
  }
  ```

#### 声明多个名称的 `using` 声明 (`using`-declaration declaring multiple names`)

- **说明**：一条 `using` 声明可以引入多个同命名空间下的名称。
- **示例**：
  ```cpp
  // C++14 方式
  using std::cout;
  using std::endl;
  using std::cin;

  // C++17 方式
  using std::cout, std::endl, std::cin;
  ```

#### 属性命名空间无需重复 (`attribute namespaces don't have to repeat`)

- **说明**：使用属性时，命名空间前缀只需指定一次。
- **示例**：
  ```cpp
  // C++14 方式
  [[gnu::always_inline]] [[gnu::hot]] [[gnu::const]] void func();

  // C++17 方式
  [[using gnu: always_inline, hot, const]] void func();
  ```

### 新属性 (`new attributes`)

#### `[[fallthrough]]`

- **说明**：用于 `switch` 语句中，显式标记从一个 `case` 标签“贯穿”（fall through）到下一个，以抑制编译器对此的警告。
- **示例**：
  ```cpp
  switch (value) {
      case 1:
          handle_case_1();
          [[fallthrough]]; // 显式表示有意贯穿
      case 2:
          handle_case_2();
          break;
      default:
          handle_default();
  }
  ```

#### `[[maybe_unused]]`

- **说明**：用于标记变量、函数、类、枚举等，表明它们可能未被使用，以抑制编译器的未使用警告。
- **示例**：
  ```cpp
  void func([[maybe_unused]] int debug_value) {
      // 在 Release 模式下可能不使用 debug_value
  }
  ```

#### `[[nodiscard]]`

- **说明**：用于标记函数，表示其返回值不应被忽略。
- **示例**：
  ```cpp
  [[nodiscard]] int get_critical_value() {
      return 42;
  }

  void process() {
      get_critical_value(); // 编译器很可能会发出警告，因为返回值被忽略了
      (void)get_critical_value(); // 显式忽略返回值，通常不会警告
  }
  ```

### `__has_include`

- **说明**：一个预处理器表达式，用于检查头文件是否存在。
- **语法**：`__has_include("header.h")` 或 `__has_include(<header>)`
- **用途**：编写可移植的代码，根据可用的头文件有条件地编译代码。
- **示例**：
  ```cpp
  #if __has_include(<filesystem>)
      #include <filesystem>
      namespace fs = std::filesystem;
  #elif __has_include(<experimental/filesystem>)
      #include <experimental/filesystem>
      namespace fs = std::experimental::filesystem;
  #endif
  ```

## 新增头文件

C++17 引入了以下新的标准库头文件：

- `<any>`：`std::any` 类型，用于存储任意类型的值。
- `<charconv>`：字符与数值之间的高效转换函数 `std::to_chars` 和 `std::from_chars`。
- `<execution>`：并行算法的执行策略。
- `<filesystem>`：文件系统库。
- `<memory_resource>`：多态内存资源。
- `<optional>`：`std::optional` 类型，用于表示一个可能有值或无值的对象。
- `<string_view>`：`std::string_view` 类型，用于非拥有（non-owning）的字符串视图。
- `<variant>`：`std::variant` 类型，类型安全的联合体。

## 新增标准库特性

### 实用类型 (Utility types)

#### `std::tuple`

- `std::apply`：将一个元组的元素作为参数传递给一个可调用对象。
- `std::make_from_tuple`：从一个元组构造一个对象。
- `deduction guides`：类模板实参推导指南。

#### `std::any`

- **说明**：一个类型安全的容器，可以持有任何拷贝构造的类型。
- **示例**：
  ```cpp
  std::any a = 42;
  a = std::string("hello");
  try {
      int n = std::any_cast<int>(a); // 将抛出 std::bad_any_cast
  } catch (const std::bad_any_cast& e) {
      std::cout << "Bad cast\n";
  }
  std::string s = std::any_cast<std::string>(a); // 成功
  ```

#### `std::optional`

- **说明**：表示一个可能包含值或为空的类型，是“无值”概念的类型安全表示。
- **示例**：
  ```cpp
  std::optional<int> parse_int(const std::string& s) {
      try {
          return std::stoi(s);
      } catch (...) {
          return std::nullopt; // 表示解析失败
      }
  }

  auto result = parse_int("123");
  if (result.has_value()) {
      std::cout << "Parsed: " << result.value() << std::endl;
  }
  ```

#### `std::variant`

- **说明**：类型安全的联合体（tagged union），可以持有多种指定类型中的一种。
- **示例**：
  ```cpp
  std::variant<int, std::string> v;
  v = 42; // 持有 int
  v = "hello"s; // 持有 string

  // 使用 std::visit 访问
  std::visit([](const auto& arg) { std::cout << arg << std::endl; }, v);
  ```

#### 搜索器 (Searchers)

- **说明**：位于 `<functional>` 中，提供高效的字符串搜索算法（如 Boyer-Moore）。
- **示例**：
  ```cpp
  std::string text = "Hello, this is a test string for searching.";
  std::string pattern = "test";

  std::boyer_moore_searcher searcher(pattern.begin(), pattern.end());
  auto it = std::search(text.begin(), text.end(), searcher);
  if (it != text.end()) {
      std::cout << "Found at position: " << (it - text.begin()) << std::endl;
  }
  ```

#### `std::as_const`

- **说明**：将左值引用转换为对应的 `const` 引用。
- **用途**：强制调用 `const` 版本的成员函数。
- **示例**：
  ```cpp
  std::vector<int> v = {1, 2, 3};
  auto& const_v = std::as_const(v);
  // const_v.push_back(4); // 错误！const 版本无 push_back
  ```

#### `std::not_fn`

- **说明**：一个函数适配器，将可调用对象的结果取反。
- **示例**：
  ```cpp
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto it = std::find_if(v.begin(), v.end(), std::not_fn([](int i) { return i % 2 == 0; }));
  // 找到第一个不是偶数的元素，即 1
  ```

### 内存管理 (Memory management)

#### 未初始化内存算法

- `std::destroy_at`：调用对象的析构函数。
- `std::destroy`：调用范围内的所有对象的析构函数。
- `std::destroy_n`：调用范围内的前 N 个对象的析构函数。
- `std::uninitialized_move`：在未初始化的内存中移动元素。
- `std::uninitialized_value_construct`：在未初始化的内存中用值初始化构造对象。

#### `weak_from_this`

- **说明**：`std::enable_shared_from_this` 的成员函数，返回一个 `std::weak_ptr`。
- **用途**：避免 `shared_from_this` 的潜在问题。

#### `std::pmr::memory_resource` 和 `std::polymorphic_allocator`

- **说明**：提供多态内存资源，允许使用不同的内存分配策略。
- **示例**：
  ```cpp
  #include <memory_resource>
  std::pmr::vector<int> v;
  std::pmr::monotonic_buffer_resource pool(1024);
  std::pmr::vector<int> v2(&pool); // 使用特定的内存资源
  ```

#### `std::aligned_alloc`

- **说明**：从 C11 引入，用于分配具有特定对齐要求的内存。

#### 透明的 `std::owner_less`

- **说明**：`std::owner_less<void>` 是一个函数对象，用于比较智能指针（如 `shared_ptr`, `weak_ptr`）的所有权。

#### `std::shared_ptr` 的数组支持

- **说明**：可以直接使用 `std::shared_ptr<T[]>` 和 `std::shared_ptr<T[N]>`。

#### 显式对齐的分配函数

### 编译时编程 (Compile-time programming)

#### `std::byte`

- **说明**：表示内存字节的类型，与 `char` 或 `unsigned char` 不同，它没有任何算术或字符操作的含义。
- **示例**：
  ```cpp
  std::byte b{42};
  std::byte c = b << 2; // 位运算
  ```

#### `std::conjunction`/`std::disjunction`/`std::negation`

- **说明**：元编程中的逻辑运算符，短路求值。
- **示例**：
  ```cpp
  template<typename T>
  using is_small_integral = std::conjunction<
      std::is_integral<T>,
      std::bool_constant<(sizeof(T) <= 4)>
  >;
  ```

#### 类型特征变量模板 (`xxx_v`)

- **说明**：为所有类型特征提供了 `_v` 后缀的变量模板版本。
- **示例**：
  ```cpp
  // C++14
  if (std::is_integral<T>::value) { /* ... */ }
  // C++17
  if (std::is_integral_v<T>) { /* ... */ }
  ```

#### `std::is_swappable`

- **说明**：检查类型是否可交换。

#### `std::is_invocable`

- **说明**：检查类型是否可调用。

#### `std::is_aggregate`

- **说明**：检查类型是否为聚合类型（aggregate）。

#### `std::has_unique_object_representations`

- **说明**：检查类型的对象表示是否是唯一的（即没有填充位或陷阱表示）。

### 算法 (Algorithms)

#### `std::clamp`

- **说明**：将值限制在指定范围内。
- **示例**：
  ```cpp
  int value = 15;
  int clamped = std::clamp(value, 0, 10); // clamped = 10
  ```

#### 并行算法和执行策略

- **说明**：标准算法可以接受执行策略（如 `std::execution::par`）来并行执行。

#### `std::inclusive_scan` 和 `std::exclusive_scan`

- **说明**：前缀扫描算法。

#### `std::gcd` 和 `std::lcm`

- **说明**：计算最大公约数和最小公倍数。

#### `std::reduce`

- **说明**：类似 `std::accumulate`，但支持并行执行和非确定性求值顺序。

### 迭代器和容器 (Iterators and containers)

#### map/set 的 `extract` 和 `merge`

- **说明**：允许高效地在不同 map/set 之间移动节点。

#### map/unordered_map 的 `try_emplace` 和 `insert_or_assign`

- **说明**：更安全和直观的插入/赋值方法。

#### 连续迭代器 (LegacyContiguousIterator)

- **说明**：一个迭代器概念，保证元素在内存中连续排列。

#### 非成员 `std::size`/`std::empty`/`std::data`

- **说明**：通用的函数，可用于任何支持 `.size()`, `.empty()`, `.data()` 的类型（包括 C 风格数组）。
- **示例**：
  ```cpp
  int arr[5] = {1, 2, 3, 4, 5};
  std::cout << std::size(arr) << std::endl; // 5
  ```

### 数值计算 (Numerics)

#### 数学特殊函数

- **说明**：从 C11 和 TS 引入，如贝塞尔函数、伽马函数等。

#### 三维 `std::hypot`

- **说明**：`std::hypot(x, y, z)` 计算 `sqrt(x*x + y*y + z*z)`。

### 其他 (Others)

#### 缓存行接口 (Cache line interface)

- **说明**：`std::hardware_destructive_interference_size` 和 `std::hardware_constructive_interference_size`，用于优化并发性能。

#### `std::launder`

- **说明**：一个函数，用于在某些涉及对象重用的低级内存操作中，告知编译器获取对象的新指针。

#### `std::uncaught_exceptions`

- **说明**：返回当前未处理的异常数量，用于改进异常安全代码。

#### `std::to_chars`/`std::from_chars`

- **说明**：高效、无 locale 依赖的数值与字符转换。

#### `std::atomic<T>::is_always_lock_free`

- **说明**：一个 `constexpr` 值，指示 `std::atomic<T>` 是否在所有目标平台上都是无锁的。

#### `std::scoped_lock`

- **说明**：一个通用的互斥锁包装器，可以同时锁定多个互斥量，避免死锁。
- **示例**：
  ```cpp
  std::mutex mtx1, mtx2;
  {
      std::scoped_lock lock(mtx1, mtx2); // 自动按一致顺序锁定
      // 访问受 mtx1 和 mtx2 保护的数据
  } // 自动解锁
  ```

#### `std::timespec_get`

- **说明**：从 C11 引入，用于获取高精度时间。

#### `std::chrono::duration` 和 `std::chrono::time_point` 的舍入函数

## 缺陷报告 (Defect Reports)

C++17 修复了大量核心语言和标准库的缺陷报告。

### 修复的核心语言缺陷报告 (共 290 项)

- CWG#92, CWG#150, CWG#212, ..., CWG#2276

### 修复的标准库缺陷报告 (共 363 项)

- LWG#839, LWG#1041, LWG#1150, ..., LWG#2956

## 编译器支持情况

### C++17 核心语言特性支持

（表格展示了 GCC、Clang、MSVC 等编译器对各项 C++17 特性的支持情况。）

### C++17 标准库特性支持

（表格展示了 GCC libstdc++、Clang libc++、MSVC STL 等标准库实现对各项 C++17 库特性的支持情况。）

---

**信息来源**：[https://en.cppreference.com/mwiki/index.php?title=cpp/17&oldid=179939](https://en.cppreference.com/mwiki/index.php?title=cpp/17&oldid=179939)

**最后修改时间**：2025年1月28日 16:10

**离线版本获取时间**：2025年2月9日 16:39