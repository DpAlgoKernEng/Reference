# C++11 标准

来源：cppreference.com < cpp

## 标准概览

C++11 是 C++ 语言的第二个主要版本，也是自 C++98 以来最重要的一次更新。它引入了大量新特性，旨在标准化现有的编程实践并为 C++ 程序员提供更强大的抽象能力。

C++11 在 2011 年 8 月 12 日被 ISO 正式批准。其前身为 "C++0x"，因为最初预计在 2010 年之前发布。从 C++03 到 C++11 历时 8 年，是版本间最长的间隔。自 C++11 起，C++ 标准大致每 3 年更新一次。

### C++ 语言版本演进

| 版本 | 发布年份 | 主要特点 |
|---|---|---|
| C++98 | 1998 | 第一个正式标准 |
| C++03 | 2003 | 对 C++98 的小幅修订 |
| **C++11** | **2011** | **引入自动类型推导、移动语义、lambda 表达式、智能指针等** |
| C++14 | 2014 | 对 C++11 的小幅修订，增强泛型编程 |
| C++17 | 2017 | 结构化绑定、`if constexpr`、文件系统库等 |
| C++20 | 2020 | 概念、范围、协程、模块等 |
| C++23 | 2023 | 标准化了更多现代 C++ 实践 |
| C++26 | 开发中 | 下一个主要版本 |

> **注意**：C++11 是现代 C++（Modern C++）时代的起点，引入的许多特性至今仍是核心编程范式。

## 从其他来源合并的特性

- **TR1**：除了特殊函数（Special Functions）外的所有 TR1 特性。
- **Boost**：线程库、`exception_ptr`、`error_code` 和 `error_condition`、迭代器改进（`begin`, `end`, `next`, `prev`）。
- **C**：C 风格的 Unicode 转换函数。

## 核心语言特性

### `auto` 和 `decltype`

- **`auto`**：允许编译器根据初始化器自动推导变量类型。
  ```cpp
  auto i = 42;        // i 是 int
  auto d = 3.14;      // d 是 double
  auto& r = i;        // r 是 int&
  const auto ci = i;  // ci 是 const int
  ```
- **`decltype`**：获取表达式的类型，而不求值该表达式。
  ```cpp
  int x = 5;
  decltype(x) y = x;        // y 是 int
  decltype((x)) z = x;      // z 是 int& (因为 (x) 是左值)
  decltype(5 + 3.14) sum;   // sum 是 double
  ```

### 默认函数和删除函数 (Defaulted and deleted functions)

- **`= default`**：显式要求编译器生成特殊成员函数（构造函数、析构函数、赋值运算符）的默认实现。
- **`= delete`**：显式禁止编译器生成特殊成员函数，或禁止某些函数的调用。
  ```cpp
  class NonCopyable {
  public:
      NonCopyable() = default; // 默认构造函数
      ~NonCopyable() = default; // 默认析构函数
      NonCopyable(const NonCopyable&) = delete; // 禁止拷贝构造
      NonCopyable& operator=(const NonCopyable&) = delete; // 禁止拷贝赋值
  };

  class NoIntConstructor {
  public:
      NoIntConstructor(int) = delete; // 禁止用 int 构造
      NoIntConstructor(double d) { /* ... */ } // 允许用 double 构造
  };
  ```

### `final` 和 `override`

- **`final`**：用于类，表示该类不能被继承；用于虚函数，表示该虚函数不能被进一步重写。
- **`override`**：用于虚函数，显式声明该函数旨在重写基类中的虚函数。如果基类中没有匹配的虚函数，编译器会报错。
  ```cpp
  class Base {
  public:
      virtual void func1();
      virtual void func2() final; // func2 不能被重写
  };

  class Derived final : public Base { // Derived 不能被继承
  public:
      void func1() override; // 正确地重写基类函数
      // void func2() override; // 错误！func2 在基类中是 final
  };
  ```

### 返回类型后置 (Trailing return type)

- **语法**：`auto function_name(参数列表) -> 返回类型`。
- **用途**：当返回类型依赖于模板参数或难以在函数名前直接写出时非常有用，尤其在泛型编程中。
  ```cpp
  template<typename T, typename U>
  auto add(T t, U u) -> decltype(t + u) {
      return t + u;
  }
  ```

### 右值引用 (Rvalue references)

- **说明**：`T&&` 是一种新的引用类型，可以绑定到临时对象（右值）。
- **用途**：实现移动语义和完美转发，是 C++11 性能提升的核心。
  ```cpp
  class MyString {
  private:
      char* data;
      size_t len;
  public:
      // 移动构造函数
      MyString(MyString&& other) noexcept 
          : data(other.data), len(other.len) {
          other.data = nullptr; // 源对象被置空
          other.len = 0;
      }
      // 移动赋值运算符
      MyString& operator=(MyString&& other) noexcept {
          if (this != &other) {
              delete[] data;
              data = other.data;
              len = other.len;
              other.data = nullptr;
              other.len = 0;
          }
          return *this;
      }
  };
  ```

### 移动构造函数和移动赋值运算符

- **说明**：利用右值引用实现，允许将资源（如动态内存）从一个对象“移动”到另一个对象，而不是进行昂贵的深拷贝。
- **扩展解释**：这是 C++11 性能优化的关键。`std::vector` 等标准库容器因此获得了巨大的性能提升。

### 有作用域的枚举 (Scoped enums)

- **语法**：`enum class EnumName { ... };`
- **优势**：
  1. **强类型**：枚举值不会隐式转换为整数。
  2. **作用域限定**：枚举值必须通过 `EnumName::Value` 访问。
  3. **可指定底层类型**：`enum class Color : char { Red, Green, Blue };`
- **示例**：
  ```cpp
  enum class Color { Red, Green, Blue };
  enum class TrafficLight { Red, Yellow, Green };

  Color c = Color::Red;
  // int i = c; // 错误！不能隐式转换
  int i = static_cast<int>(c); // 正确
  ```

### `constexpr` 和字面量类型 (Literal types)

- **`constexpr`**：
  - **变量**：声明一个编译时的常量表达式。
  - **函数**：声明一个可能在编译时求值的函数（如果传入的参数是常量表达式）。
- **字面量类型**：可以在 `constexpr` 上下文中使用的类型，如算术类型、指针、引用、字面量类等。
- **示例**：
  ```cpp
  constexpr int square(int x) {
      return x * x; // 如果 x 是 constexpr，则此函数在编译时求值
  }

  constexpr int result = square(5); // result 在编译时就是 25

  struct Point {
      constexpr Point(int x, int y) : x_(x), y_(y) {}
      constexpr int x() const { return x_; }
      constexpr int y() const { return y_; }
  private:
      int x_, y_;
  };

  constexpr Point p(1, 2);
  constexpr int px = p.x(); // 在编译时求值
  ```

### 列表初始化 (List initialization)

- **语法**：使用大括号 `{}` 进行初始化。
- **统一性**：可以用于几乎所有对象的初始化，包括聚合类型、POD 类型、容器、内置数组等。
- **防缩窄转换**：列表初始化会阻止可能导致信息丢失的隐式转换。
  ```cpp
  int arr[] = {1, 2, 3}; // C 风格数组
  std::vector<int> v = {1, 2, 3}; // 容器
  std::map<int, std::string> m = {{1, "one"}, {2, "two"}}; // 关联容器
  
  class MyClass {
  public:
      MyClass(int a, double b) : a_(a), b_(b) {}
  private:
      int a_;
      double b_;
  };
  MyClass obj{10, 3.14}; // 直接列表初始化
  MyClass obj2 = {10, 3.14}; // 拷贝列表初始化

  // 防止缩窄转换
  int narrow = {3.14}; // 错误！double 到 int 是缩窄转换
  ```

### 委托构造函数和继承构造函数 (Delegating and inherited constructors)

- **委托构造函数**：一个类的构造函数可以调用同一个类的另一个构造函数。
  ```cpp
  class Box {
  public:
      Box(int w, int h, int l) : width(w), height(h), length(l) {}
      Box(int side) : Box(side, side, side) {} // 委托构造
      Box() : Box(1) {} // 进一步委托
  private:
      int width, height, length;
  };
  ```
- **继承构造函数**：派生类可以继承基类的构造函数（C++11 引入，C++14 扩展）。
  ```cpp
  class Base {
  public:
      Base(int x) : val(x) {}
      Base(double d, int x) : val(static_cast<int>(d) + x) {}
  protected:
      int val;
  };

  class Derived : public Base {
  public:
      using Base::Base; // 继承 Base 的所有构造函数
  };
  ```

### 类内初始值 (Brace-or-equal initializers)

- **说明**：允许在类定义内部为非静态数据成员提供初始值。
- **示例**：
  ```cpp
  class MyClass {
  public:
      int x = 10;        // 就地初始化
      std::string s{"Hello"};
      double d{3.14};
  };
  ```

### `nullptr`

- **说明**：引入 `nullptr` 作为空指针常量，其类型是 `std::nullptr_t`。
- **优势**：解决了 C 风格宏 `NULL`（通常是 `0` 或 `(void*)0`）带来的歧义问题。
  ```cpp
  void func(int);
  void func(char*);

  func(NULL); // 可能调用 func(int)，因为 NULL 是 0
  func(nullptr); // 明确调用 func(char*)
  ```

### `long long`

- **说明**：引入 `long long` 和 `unsigned long long` 类型，保证至少 64 位。

### `char16_t` 和 `char32_t`

- **说明**：引入这两种新的字符类型，用于存储 UTF-16 和 UTF-32 编码的代码单元。
- **示例**：
  ```cpp
  char16_t u16_char = u'A'; // UTF-16 字符
  char32_t u32_char = U'€'; // UTF-32 字符
  ```

### 类型别名 (Type aliases)

- **语法**：`using NewTypeName = ExistingType;`
- **优势**：相比 `typedef`，语法更清晰，更易读，特别是对于复杂的类型。
  ```cpp
  // 传统 typedef
  typedef void (*FuncType)(int, double);
  // C++11 using
  using FuncType = void(*)(int, double);

  // 对于模板别名，using 更强大
  template<typename T>
  using Vec = std::vector<T, MyAllocator<T>>; // 可以直接使用 Vec<int>
  // typedef 无法直接实现模板别名
  ```

### 可变参数模板 (Variadic templates)

- **说明**：模板可以接受可变数量和类型的参数。
- **用途**：实现类型安全的泛型函数（如 `std::make_tuple`）、完美转发、递归模板元编程等。
- **示例**：
  ```cpp
  // 递归展开
  template<typename T>
  void print(T&& t) {
      std::cout << t << std::endl;
  }

  template<typename T, typename... Args>
  void print(T&& t, Args&&... args) {
      std::cout << t << " ";
      print(args...); // 递归调用
  }

  // 使用
  print(1, 2.5, "hello", 'c');
  ```

### 广义联合体 (Generalized (non-trivial) unions)

- **说明**：C++11 允许联合体包含有非平凡（non-trivial）特殊成员函数（如用户定义的构造函数、析构函数）的类型。
- **注意**：程序员需要手动管理联合体内对象的生命周期。

### 广义 POD (Generalized PODs)

- **说明**：C++11 引入了更细粒度的概念来取代旧的 POD（Plain Old Data）定义：
  - **平凡类型 (Trivial Type)**：拥有平凡的构造/析构/拷贝/移动函数。
  - **标准布局类型 (Standard-layout Type)**：内存布局满足 C 兼容性要求（如没有虚函数、基类等）。
- **用途**：为需要与 C 或其他语言互操作的代码提供更精确的控制。

### Unicode 字符串字面量

- **语法**：
  - `u8"..."`：UTF-8 字符串字面量。
  - `u"..."`：UTF-16 字符串字面量。
  - `U"..."`：UTF-32 字符串字面量。
- **示例**：
  ```cpp
  const char* utf8_str = u8"Hello, 世界";
  const char16_t* utf16_str = u"Hello, 世界";
  const char32_t* utf32_str = U"Hello, 世界";
  ```

### 用户定义字面量 (User-defined literals)

- **说明**：允许程序员定义自己的后缀，用于创建自定义类型的字面量。
- **示例**：
  ```cpp
  constexpr long double operator"" _cm(long double x) {
      return x * 10.0;
  }
  constexpr long double operator"" _m(long double x) {
      return x * 1000.0;
  }
  constexpr long double operator"" _mm(long double x) {
      return x;
  }

  long double height = 3.4_cm; // 3.4 厘米
  long double length = 2.5_m;  // 2.5 米
  long double width = 10.0_mm; // 10.0 毫米
  ```

### 属性 (Attributes)

- **语法**：`[[attribute]]`，类似于 C++/CLI 和 C# 中的特性。
- **标准属性**：
  - `[[noreturn]]`：指示函数不返回。
  - `[[carries_dependency]]`：用于内存序编程，告知编译器依赖关系。
  - `[[deprecated]]`（C++14 引入）：标记已弃用的声明。
- **示例**：
  ```cpp
  [[noreturn]] void terminate() {
      std::abort();
  }

  // 自定义属性（编译器/平台相关）
  [[gnu::always_inline]] inline void fast_func() { /* ... */ }
  ```

### Lambda 表达式 (Lambda expressions)

- **语法**：`[capture](parameters) -> return_type { body }`
- **用途**：在需要函数对象的地方（如 STL 算法）内联定义匿名函数，使代码更简洁。
- **示例**：
  ```cpp
  std::vector<int> v = {1, 2, 3, 4, 5};
  // 捕获引用
  int factor = 2;
  std::for_each(v.begin(), v.end(), [factor](int& n) { n *= factor; });
  
  // 捕获引用和 this
  class MyClass {
      int offset = 10;
  public:
      void process(std::vector<int>& vec) {
          std::for_each(vec.begin(), vec.end(), 
                        [this](int& n) { n += offset; });
      }
  };

  // 泛型 lambda (C++14)
  auto lambda = [](const auto& a, const auto& b) { return a + b; };
  ```

### `noexcept` 说明符和 `noexcept` 运算符

- **`noexcept` 说明符**：`void func() noexcept;` 声明函数不会抛出异常。这有助于编译器优化，并影响移动语义的选择。
- **`noexcept` 运算符**：`noexcept(expression)` 是一个 `constexpr` 布尔值，用于检查一个表达式在求值时是否会抛出异常。
  ```cpp
  void func() noexcept {
      // ...
  }

  template<typename T>
  void wrapper(T&& t) noexcept(noexcept(func())) {
      func();
  }
  ```

### `alignof` 和 `alignas`

- **`alignof(type)`**：一个运算符，返回类型的对齐要求。
- **`alignas(alignment)`**：一个说明符，指定变量或类型的对齐方式。
  ```cpp
  struct alignas(16) S {
      int n; // S 的对齐为 16 字节
  };
  std::cout << alignof(S) << std::endl; // 输出 16
  ```

### 多线程内存模型 (Multithreaded memory model)

- **说明**：C++11 标准定义了多线程环境下的内存可见性和操作顺序的规则，为原子操作、内存序和线程同步提供了基础。

### 线程局部存储 (Thread-local storage)

- **说明**：`thread_local` 关键字用于声明线程局部存储变量。每个线程都有该变量的一个独立副本。
  ```cpp
  thread_local int thread_id = 0; // 每个线程都有自己的 thread_id
  ```

### GC 接口 (GC interface, C++23 中移除)

- **说明**：C++11 引入了与垃圾回收器交互的接口，但从未被广泛实现，并在 C++23 中被移除。

### 基于范围的 for 循环 (Range-for)

- **语法**：`for (declaration : range) { ... }`
- **用途**：简化对容器或数组的遍历。
  ```cpp
  std::vector<int> v = {1, 2, 3, 4, 5};
  for (const auto& element : v) {
      std::cout << element << " ";
  }
  ```

### `static_assert`

- **说明**：在编译时进行断言检查，如果条件不满足则产生编译错误。
- **用途**：用于模板元编程、检查编译时已知的条件。
  ```cpp
  static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
  template<typename T>
  void func(T t) {
      static_assert(std::is_integral<T>::value, "T must be integral");
      // ...
  }
  ```

## 标准库特性

### 新增头文件

C++11 引入了多个新的标准库头文件。

- `<array>`：固定大小数组容器 `std::array`。
- `<atomic>`：原子操作库。
- `<cfenv>`：浮点环境控制。
- `<chrono>`：时间处理库。
- `<cinttypes>`：C 风格的整数格式化。
- `<condition_variable>`：条件变量，用于线程同步。
- `<cstdint>`：确定宽度的整数类型。
- `<cuchar>`：C 风格的 Unicode 转换。
- `<forward_list>`：单向链表容器 `std::forward_list`。
- `<future>`：异步任务和未来值。
- `<initializer_list>`：支持列表初始化的类型。
- `<mutex>`：互斥锁，用于线程同步。
- `<random>`：随机数生成库。
- `<ratio>`：编译时有理数算术。
- `<regex>`：正则表达式库。
- `<scoped_allocator>`：嵌套分配器。
- `<system_error>`：系统错误报告。
- `<thread>`：线程支持。
- `<tuple>`：元组类型。
- `<typeindex>`：类型索引，用于 `typeid`。
- `<type_traits>`：类型特征。
- `<unordered_map>`：哈希表映射容器。
- `<unordered_set>`：哈希表集合容器。

### 标准库功能增强

#### 并发支持库 (Concurrency support library)

通过 `<thread>`, `<mutex>`, `<condition_variable>`, `<future>`, `<atomic>` 等头文件提供。

#### `emplace()` 和右值引用的广泛使用

标准库容器和相关组件广泛使用了右值引用和移动语义，提供了 `emplace`, `emplace_back`, `emplace_front` 等函数，直接在容器内构造元素，避免不必要的拷贝或移动。

#### `std::unique_ptr`

- **说明**：智能指针，独占拥有一个动态分配的对象。当 `unique_ptr` 被销毁时，它所拥有的对象也会被自动删除。
- **优势**：比 `std::auto_ptr`（已弃用）更安全，支持移动语义，开销极小。
  ```cpp
  std::unique_ptr<int> ptr(new int(42));
  // 或者使用工厂函数 (C++14 起)
  auto ptr2 = std::make_unique<int>(42);
  ```

#### `std::move_iterator`

- **说明**：一个迭代器适配器，它将普通迭代器的解引用操作转换为返回右值引用，从而可以触发移动语义。
- **用途**：与 `std::move` 结合使用，将容器内容移动到另一个容器，而不是拷贝。

#### `std::initializer_list`

- **说明**：一个轻量级的代理对象，用于访问初始化列表（由大括号 `{}` 创建）。构造函数可以接受 `std::initializer_list` 来支持列表初始化。
  ```cpp
  std::vector<int> v = {1, 2, 3, 4}; // 使用 initializer_list 构造函数
  ```

#### 有状态和有作用域的分配器 (Stateful and scoped allocators)

- **说明**：标准库容器可以使用有状态的分配器实例，并且 `<scoped_allocator>` 提供了处理嵌套分配器的机制。

#### `std::forward_list`

- **说明**：单向链表，只提供前向迭代器，内存效率高于 `std::list`。

#### 时间库 (Chrono library)

- **说明**：`<chrono>` 提供了类型安全的时间处理，包括时钟、时间点和持续时间。
  ```cpp
  auto start = std::chrono::high_resolution_clock::now();
  // ... some work ...
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken: " << duration.count() << " ms\n";
  ```

#### 比率库 (Ratio library)

- **说明**：`<ratio>` 提供了在编译时进行有理数算术的机制。
  ```cpp
  using namespace std::chrono;
  using half_minute = duration<int, ratio<1, 2>>; // 0.5 秒
  ```

#### 新增算法

C++11 为 `<algorithm>` 头文件增加了一系列新算法：

- `std::all_of`, `std::any_of`, `std::none_of`：检查范围内的元素是否全部、任一或没有满足谓词。
- `std::find_if_not`：查找第一个不满足谓词的元素。
- `std::copy_if`：拷贝满足谓词的元素。
- `std::copy_n`：拷贝前 N 个元素。
- `std::move`, `std::move_backward`：移动元素。
- `std::shuffle`（取代 `std::random_shuffle`）：使用指定的随机数生成器打乱序列。
- `std::is_partitioned`, `std::partition_copy`, `std::partition_point`：分区相关算法。
- `std::is_sorted`, `std::is_sorted_until`：排序相关检查。
- `std::is_heap`, `std::is_heap_until`：堆相关检查。
- `std::minmax`, `std::minmax_element`：同时获取最小值和最大值。
- `std::is_permutation`：检查一个序列是否是另一个序列的排列。
- `std::iota`：用递增值填充序列。
- `std::uninitialized_copy_n`：在未初始化的内存中拷贝前 N 个元素。

#### Unicode 转换面 (Unicode conversion facets)

- **说明**：标准库中添加了用于宽字符和多字节字符之间转换的 `std::codecvt` 特化。

#### `std::function`

- **说明**：一个通用的多态函数包装器，可以存储、拷贝和调用任何可调用的目标（函数、lambda 表达式、绑定表达式或其他函数对象）。
  ```cpp
  std::function<int(int, int)> func;
  func = [](int a, int b) { return a + b; };
  int result = func(1, 2); // result = 3
  ```

#### `std::exception_ptr`

- **说明**：一个类型安全的指针，指向一个异常对象。用于跨线程传递异常。
  ```cpp
  std::exception_ptr eptr;
  try {
      throw std::runtime_error("error");
  } catch(...) {
      eptr = std::current_exception();
  }
  // 在另一个线程中
  if (eptr) {
      std::rethrow_exception(eptr);
  }
  ```

#### `std::error_code` 和 `std::error_condition`

- **说明**：提供了平台无关的错误处理机制，可以区分错误码和错误条件。

#### 迭代器改进

- `std::begin`, `std::end`：非成员函数，可以用于获取任何容器（包括 C 风格数组）的开始和结束迭代器。
- `std::next`, `std::prev`：非成员函数，用于获取迭代器的下一个或上一个位置。

#### Unicode 转换函数

从 C 标准库 `<uchar.h>` 引入的函数。

## 缺陷报告 (Defect Reports)

C++11 修复了大量核心语言和标准库的缺陷报告。

### 部分修复的核心语言缺陷报告 (共 741 项)

- CWG#4, CWG#5, CWG#8, ..., CWG#1245, CWG#1246

### 部分修复的标准库缺陷报告 (共 868 项)

- LWG#23, LWG#44, LWG#49, ..., LWG#2042

## 编译器支持情况

### C++11 核心语言特性支持

（表格展示了 GCC、Clang、MSVC 等编译器对各项 C++11 特性的支持情况。）

### C++11 标准库特性支持

（表格展示了 GCC libstdc++、Clang libc++、MSVC STL 等标准库实现对各项 C++11 库特性的支持情况。）

---

**信息来源**：[https://en.cppreference.com/mwiki/index.php?title=cpp/11&oldid=179943](https://en.cppreference.com/mwiki/index.php?title=cpp/11&oldid=179943)

**最后修改时间**：2025年1月28日 16:20

**离线版本获取时间**：2025年2月9日 16:39