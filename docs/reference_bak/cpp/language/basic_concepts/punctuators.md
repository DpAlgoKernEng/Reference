# C++ 标点符号（Punctuators）完全详解

  [1. 标点符号总览](#1-标点符号总览)
  
  [2. 详细标点符号说明](#2-详细标点符号说明)
  
  [3. 运算符优先级和结合性](#3-运算符优先级和结合性)
  
  [4. 特殊用法和现代 C++ 特性](#4-特殊用法和现代-c-特性)
  
  [5. 常见错误和最佳实践](#5-常见错误和最佳实践)
  
  [6. 标准参考](#6-标准参考)
  
  [7. 总结](#7-总结)

C++ 标点符号是构成 C++ 程序语法结构的基础元素，每个符号都有其特定的语义和多种用途。深入理解这些标点符号对于编写高质量的 C++ 程序至关重要。

## 1. 标点符号总览

C++ 语言中共有 60 多个标点符号，涵盖了从基本语法结构到高级语言特性的各种用途。这些符号可以分为以下几类：

- 分隔符类：`{ }` `[ ]` `( )`
- 预处理类：`#` `##`
- 语句控制类：`;` `:` `...`
- 运算符类：算术、逻辑、位运算、赋值等
- 特殊用途类：`?` `->` `.` `::` `,` `.*` `->*` `<=>`

与 C 相比，C++ 增加了许多新的标点符号，以支持面向对象编程、模板编程、异常处理、Lambda 表达式等现代特性。

## 2. 详细标点符号说明

### 2.1 大括号 `{ }`

大括号是 C++ 语言中最重要的分隔符之一，用于界定各种代码块和结构。

**复合语句界定**：

复合语句（代码块）是由大括号界定的一组声明和语句：

```cpp
#include <iostream>

int main() {
    // 外层复合语句
    int x = 10;
    
    {
        // 内层复合语句（嵌套作用域）
        int y = 20;
        int x = 30;  // 隐藏外层的 x
        std::cout << "内层: x=" << x << ", y=" << y << std::endl;
    }
    
    std::cout << "外层: x=" << x << std::endl;  // 输出外层的 x=10
    
    // 函数体本身就是一个复合语句
    return 0;
}
```

**类定义**：

在类定义中，大括号界定成员声明列表：

```cpp
// 类定义
class Point {
private:
    int x;
    int y;
    
public:
    Point(int x_val = 0, int y_val = 0) : x(x_val), y(y_val) {}
    
    int getX() const { return x; }
    int getY() const { return y; }
    
    void setX(int x_val) { x = x_val; }
    void setY(int y_val) { y = y_val; }
};

// 结构体定义（默认公有成员）
struct Rectangle {
    Point top_left;
    Point bottom_right;
    int color;
    
    // 构造函数
    Rectangle(Point tl, Point br, int c) 
        : top_left(tl), bottom_right(br), color(c) {}
};

// 嵌套类
class Outer {
public:
    class Inner {
    public:
        static int value;
        void func() {
            std::cout << "Inner class function" << std::endl;
        }
    };
};

int Outer::Inner::value = 0;  // 静态成员定义
```

**枚举定义**：

在枚举定义中，大括号界定枚举器列表：

```cpp
// 基本枚举
enum Color {
    RED,
    GREEN,
    BLUE
};

// 带值的枚举
enum HttpStatus {
    OK = 200,
    NOT_FOUND = 404,
    SERVER_ERROR = 500
};

// 混合枚举
enum Priority {
    LOW = 1,
    MEDIUM,      // 自动为 2
    HIGH = 10,
    CRITICAL     // 自动为 11
};

// 作用域枚举（C++11）
enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

// 带底层类型的枚举（C++11）
enum class Status : unsigned char {
    ACTIVE = 1,
    INACTIVE = 0
};
```

**初始化列表**：

在初始化中，大括号用于界定初始值：

```cpp
#include <vector>
#include <map>
#include <initializer_list>

// 数组初始化
int numbers[5] = {1, 2, 3, 4, 5};
int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};

// STL 容器初始化（C++11）
std::vector<int> vec = {1, 2, 3, 4, 5};
std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};

// 类初始化列表
class Data {
public:
    int id;
    std::string name;
    std::vector<int> values;
    
    // 初始化列表构造函数
    Data(std::initializer_list<int> init_list) 
        : values(init_list) {}
    
    // 统一初始化语法（C++11）
    Data(int i, const std::string& n) 
        : id(i), name(n) {}
};

// 使用统一初始化
Data d1{1, "test"};
Data d2{1, 2, 3, 4, 5};

// Lambda 表达式
auto lambda = [](int x, int y) -> int {
    return x + y;
};
```

**命名空间定义**：

```cpp
// 命名空间定义
namespace MyNamespace {
    int value = 42;
    
    class MyClass {
    public:
        void func() {
            std::cout << "MyClass::func()" << std::endl;
        }
    };
    
    namespace Inner {
        int inner_value = 100;
    }
}

// 嵌套命名空间（C++17）
namespace A::B::C {
    int nested_value = 200;
}

// 内联命名空间（C++11）
namespace Library {
    inline namespace Version1 {
        void func() {
            std::cout << "Version 1 implementation" << std::endl;
        }
    }
}
```

**异常处理**：

```cpp
#include <exception>
#include <stdexcept>

void exception_example() {
    try {
        // 可能抛出异常的代码
        throw std::runtime_error("Something went wrong");
    }
    catch (const std::exception& e) {
        // 异常处理代码块
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    catch (...) {
        // 捕获所有异常
        std::cout << "Caught unknown exception" << std::endl;
    }
}
```

### 2.2 中括号 `[ ]`

中括号主要用于数组相关的操作和现代 C++ 特性。

**数组下标运算符**：

```cpp
#include <iostream>
#include <vector>

int main() {
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // 基本下标访问
    std::cout << "arr[0] = " << arr[0] << std::endl;
    std::cout << "arr[9] = " << arr[9] << std::endl;
    
    // 表达式下标
    int index = 5;
    std::cout << "arr[index] = " << arr[index] << std::endl;
    std::cout << "arr[index + 2] = " << arr[index + 2] << std::endl;
    
    // 指针与下标（等价操作）
    int* ptr = arr;
    std::cout << "ptr[3] = " << ptr[3] << std::endl;  // 等价于 *(ptr + 3)
    std::cout << "*(ptr + 3) = " << *(ptr + 3) << std::endl;  // 等价于 ptr[3]
    
    // STL 容器下标
    std::vector<int> vec = {10, 20, 30, 40, 50};
    std::cout << "vec[2] = " << vec[2] << std::endl;
    
    return 0;
}
```

**数组声明**：

```cpp
// 基本数组声明
int arr1[10];           // 10个整数的数组
char str[256];          // 256个字符的数组
double matrix[3][4];    // 3行4列的二维数组

// 可变长度数组（C99，C++不直接支持，但可以通过其他方式实现）
void function(int size) {
    // C++ 方式：使用 vector
    std::vector<int> local_array(size);  // 动态大小数组
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size)); // 二维动态数组
}

// std::array（C++11）
#include <array>
std::array<int, 10> std_arr;  // 固定大小数组，具有 STL 容器接口
```

**Lambda 捕获列表**：

```cpp
#include <iostream>
#include <functional>

int main() {
    int x = 10;
    int y = 20;
    
    // 值捕获
    auto lambda1 = [x, y]() {
        return x + y;
    };
    
    // 引用捕获
    auto lambda2 = [&x, &y]() {
        x += 5;
        y += 5;
        return x + y;
    };
    
    // 混合捕获
    auto lambda3 = [x, &y]() {
        y += x;
        return y;
    };
    
    // 捕获所有按值
    auto lambda4 = [=]() {
        return x + y;
    };
    
    // 捕获所有按引用
    auto lambda5 = [&]() {
        x += 1;
        y += 1;
        return x + y;
    };
    
    // 移动捕获（C++14）
    auto lambda6 = [value = std::move(x)]() {
        return value;
    };
    
    std::cout << "lambda1: " << lambda1() << std::endl;
    std::cout << "lambda2: " << lambda2() << std::endl;
    std::cout << "lambda3: " << lambda3() << std::endl;
    std::cout << "lambda4: " << lambda4() << std::endl;
    std::cout << "lambda5: " << lambda5() << std::endl;
    
    return 0;
}
```

**属性说明符（C++11）**：

```cpp
// C++11 属性
[[noreturn]] void never_returns() {
    throw std::runtime_error("Never returns");
}

[[deprecated("Use new_function instead")]] 
void old_function() {
    // ...
}

// C++17 [[fallthrough]]
enum class State { START, PROCESSING, END };

void state_machine(State s) {
    switch (s) {
        case State::START:
            initialize();
            [[fallthrough]];  // 明确表示故意贯穿
        case State::PROCESSING:
            process();
            break;
        case State::END:
            finalize();
            break;
    }
}

// C++17 [[maybe_unused]]
void function([[maybe_unused]] int debug_param) {
    #ifndef NDEBUG
    std::cout << "Debug param: " << debug_param << std::endl;
    #endif
}
```

**结构化绑定（C++17）**：

```cpp
#include <tuple>
#include <map>
#include <string>

int main() {
    // 元组结构化绑定
    std::tuple<int, std::string, double> t{42, "hello", 3.14};
    auto [id, name, value] = t;
    
    std::cout << "ID: " << id << ", Name: " << name << ", Value: " << value << std::endl;
    
    // 数组结构化绑定
    int arr[] = {1, 2, 3};
    auto [a, b, c] = arr;
    
    // 结构体结构化绑定
    struct Point {
        int x, y;
    };
    
    Point p{10, 20};
    auto [x, y] = p;
    
    // map 遍历中的结构化绑定
    std::map<std::string, int> m{{"one", 1}, {"two", 2}};
    for (const auto& [key, value] : m) {
        std::cout << key << " => " << value << std::endl;
    }
    
    return 0;
}
```

### 2.3 小括号 `( )`

小括号是 C++ 语言中最多功能的标点符号之一。

**表达式分组**：

```cpp
#include <iostream>

int main() {
    int a = 10, b = 5, c = 2;
    
    // 改变运算优先级
    int result1 = a + b * c;      // 20 (乘法优先级高)
    int result2 = (a + b) * c;    // 30 (括号改变优先级)
    
    // 复杂表达式分组
    int complex = ((a + b) * c) - (a / b);
    
    std::cout << "result1 = " << result1 << ", result2 = " << result2 
              << ", complex = " << complex << std::endl;
    
    return 0;
}
```

**函数调用**：

```cpp
#include <iostream>
#include <cmath>

// 函数声明
int add(int a, int b);
double calculate(double x, double y, double z);

int main() {
    // 基本函数调用
    int sum = add(10, 20);
    
    // 嵌套函数调用
    double result = calculate(std::sqrt(16.0), std::pow(2.0, 3.0), 5.0);
    
    // 函数调用作为参数
    std::cout << "Sum: " << sum << ", Result: " << result << std::endl;
    
    // 函数对象调用
    auto lambda = [](int x) { return x * 2; };
    int doubled = lambda(5);
    
    // std::function 调用
    std::function<int(int)> func = lambda;
    int result2 = func(10);
    
    return 0;
}

int add(int a, int b) {
    return a + b;
}

double calculate(double x, double y, double z) {
    return (x + y) * z;
}
```

**类型转换（强制类型转换）**：

```cpp
#include <iostream>

int main() {
    double d = 3.14159;
    int i = static_cast<int>(d);  // 将 double 转换为 int (3)
    
    float f = 2.7f;
    double df = static_cast<double>(f);  // float to double
    
    // C 风格转换
    int j = (int)d;
    
    // 旧式转换
    int k = int(d);
    
    // 复杂类型转换
    char c = static_cast<char>(65);  // ASCII 65 = 'A'
    
    std::cout << "d = " << d << ", i = " << i << std::endl;
    std::cout << "c = " << c << std::endl;
    
    return 0;
}
```

**函数声明和定义**：

```cpp
// 函数声明中的参数列表
int function(int a, int b, char c);

// 函数指针声明
int (*func_ptr)(int, int);

// 函数引用（C++11）
using FuncRef = int (&)(int, int);

// 函数定义
int add(int a, int b) {
    return a + b;
}

// 默认参数（C++）
int multiply(int a, int b = 1) {
    return a * b;
}

// 可变参数模板（C++11）
template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args) << std::endl;  // 折叠表达式（C++17）
}

// 返回类型后置语法（C++11）
auto get_value() -> int {
    return 42;
}

// Lambda 表达式参数
auto lambda = [](int x, int y) -> int {
    return x + y;
};
```

**控制语句条件界定**：

```cpp
#include <iostream>

int main() {
    int x = 10;
    
    // if 语句
    if ((x > 5) && (x < 15)) {
        std::cout << "x 在范围内" << std::endl;
    }
    
    // for 语句
    for (int i = 0; i < (x / 2); i++) {
        std::cout << "i = " << i << std::endl;
    }
    
    // while 语句
    while ((x--) > 0) {
        std::cout << "x = " << x << std::endl;
    }
    
    // 范围 for（C++11）
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (const auto& item : vec) {
        std::cout << "item = " << item << std::endl;
    }
    
    return 0;
}
```

**宏定义中的参数列表**：

```cpp
#include <iostream>

// 基本函数式宏
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

// 复杂宏定义
#define SWAP(a, b, type) do { \
    type temp = a; \
    a = b; \
    b = temp; \
} while(0)

// 可变参数宏（C99/C++11）
#define DEBUG_PRINT(fmt, ...) \
    printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)

// C++17 __VA_OPT__ 支持
#define PRINT_IF_DEBUG(...) \
    __VA_OPT__(printf(__VA_ARGS__))

int main() {
    int a = 10, b = 20;
    int max_val = MAX(a, b);
    int square_val = SQUARE(a + 1);
    
    std::cout << "Max: " << max_val << ", Square: " << square_val << std::endl;
    
    SWAP(a, b, int);
    std::cout << "After swap: a=" << a << ", b=" << b << std::endl;
    
    DEBUG_PRINT("Debug message: a=%d, b=%d", a, b);
    
    return 0;
}
```

**模板参数列表**：

```cpp
// 模板参数列表
template<typename T, typename U>
class Pair {
private:
    T first;
    U second;
    
public:
    Pair(const T& f, const U& s) : first(f), second(s) {}
    
    T getFirst() const { return first; }
    U getSecond() const { return second; }
};

// 模板特化
template<>
class Pair<int, int> {
    // 特化实现
};

// 可变参数模板（C++11）
template<typename... Args>
class Tuple {
    // 可变参数模板实现
};

// 模板模板参数
template<template<typename> class Container, typename T>
class Wrapper {
    Container<T> container;
};
```

**异常规范**：

```cpp
// 异常规范（C++11）
void no_except_function() noexcept {
    // 保证不抛出异常
}

void may_throw_function() noexcept(false) {
    // 可能抛出异常
}

// 动态异常规范（已弃用）
void old_style_function() throw(std::runtime_error) {
    // 仅允许抛出 runtime_error
}
```

### 2.4 预处理符号 `#` 和 `##`

**`#` 字符串化运算符**：

```cpp
#include <iostream>

// 基本字符串化
#define STRINGIFY(x) #x
#define PRINT_VAR(var) std::cout << #var " = " << var << std::endl

// 高级字符串化示例
#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << #condition \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(0)

int main() {
    int number = 42;
    const char* str = STRINGIFY(Hello World);  // "Hello World"
    PRINT_VAR(number);  // 输出: number = 42
    
    int x = 10;
    ASSERT(x > 0);  // 如果 x <= 0，则输出错误信息
    
    return 0;
}
```

**`##` 标记粘贴运算符**：

```cpp
#include <iostream>

// 基本标记粘贴
#define CONCAT(a, b) a##b
#define DECLARE_VAR(type, name) type CONCAT(var_, name)

// 高级标记粘贴示例
#define BUILD_FIELD(struct_name, field_type, field_name) \
    field_type CONCAT(struct_name, _##field_name)

#define GENERATE_GETTER_SETTER(type, name) \
    void CONCAT(set_, name)(type value) { \
        CONCAT(global_, name) = value; \
    } \
    type CONCAT(get_, name)() { \
        return CONCAT(global_, name); \
    }

int global_counter = 0;
GENERATE_GETTER_SETTER(int, counter)

int main() {
    DECLARE_VAR(int, counter) = 10;  // 等价于 int var_counter = 10;
    std::cout << "var_counter = " << var_counter << std::endl;
    
    int CONCAT(num, 1) = 100;        // 等价于 int num1 = 100;
    std::cout << "num1 = " << num1 << std::endl;
    
    // 使用生成的函数
    set_counter(42);
    std::cout << "counter = " << get_counter() << std::endl;
    
    return 0;
}
```

**预处理指令**：

```cpp
// #include 指令
#include <iostream>
#include "myheader.h"

// #define 指令
#define BUFFER_SIZE 1024
#define MAX(a,b) ((a) > (b) ? (a) : (b))

// #if/#ifdef/#ifndef 条件编译
#if defined(DEBUG)
    #define LOG(msg) std::cout << "[DEBUG] " << msg << std::endl
#elif defined(RELEASE)
    #define LOG(msg) ((void)0)
#else
    #define LOG(msg) std::cout << "[INFO] " << msg << std::endl
#endif

// #pragma 指令
#pragma once
#pragma pack(push, 1)

struct PackedStruct {
    char a;
    int b;
    short c;
};

#pragma pack(pop)

// #error 和 #warning
#if __cplusplus < 201103L
    #error "C++11 or later required"
#endif

#ifdef EXPERIMENTAL_FEATURE
    #warning "Experimental feature enabled"
#endif

// #line 指令
#line 100 "custom_file.cpp"
int custom_line_number() {
    return __LINE__;  // 返回 101
}
```

### 2.5 分号 `;`

分号是 C++ 语言中的语句终止符。

**语句结束符**：

```cpp
#include <iostream>

int main() {
    // 表达式语句
    int x = 10;
    x = x + 5;
    std::cout << "x = " << x << std::endl;
    
    // 空语句
    while (std::cin.get() != '\n')
        ;  // 等待换行符
    
    // 复合语句不需要分号
    {
        int y = 20;
        std::cout << "y = " << y << std::endl;
    }  // 不需要分号
    
    return 0;
}
```

**声明结束符**：

```cpp
// 变量声明
int a, b, c;
double x = 1.0, y = 2.0;
char str[100];

// 函数声明
int function(int a, int b);
void procedure();

// 类声明
class Point {
private:
    int x;
    int y;
public:
    Point(int x_val, int y_val);
};  // 注意这里的分号

// 枚举声明
enum Color {
    RED,
    GREEN,
    BLUE
};  // 注意这里的分号

// 命名空间别名
namespace fs = std::filesystem;  // C++17

// 类型别名
using IntPtr = int*;  // C++11
typedef int* IntPointer;  // 传统方式
```

**for 语句结构**：

```cpp
#include <iostream>
#include <vector>

int main() {
    // for 语句的三个子句
    for (int i = 0; i < 10; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // 复杂的 for 语句
    for (int i = 0, j = 10; i < j; i++, j--) {
        std::cout << "i=" << i << ", j=" << j << std::endl;
    }
    
    // 空初始化子句
    int k = 0;
    for (; k < 5; k++) {
        std::cout << "k=" << k << std::endl;
    }
    
    // 死循环（空条件子句）
    int count = 0;
    for (;; ) {
        if (++count > 3) break;
        std::cout << "count=" << count << std::endl;
    }
    
    // 范围 for（C++11）
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

### 2.6 冒号 `:`

冒号在 C++ 语言中有多种用途。

**条件运算符**：

```cpp
#include <iostream>

int main() {
    int a = 10, b = 20;
    
    // 基本条件运算符
    int max = (a > b) ? a : b;
    std::string result = (a > b) ? "a is larger" : "b is larger";
    
    std::cout << "Max: " << max << ", Result: " << result << std::endl;
    
    // 嵌套条件运算符
    int grade = 85;
    std::string letter_grade = 
        (grade >= 90) ? "A" :
        (grade >= 80) ? "B" :
        (grade >= 70) ? "C" :
        (grade >= 60) ? "D" : "F";
    
    std::cout << "Grade: " << grade << " -> " << letter_grade << std::endl;
    
    return 0;
}
```

**标签声明**：

```cpp
#include <iostream>

int main() {
    int i = 0;
    
    // 基本标签
    start:
        std::cout << "i = " << i << std::endl;
        if (++i < 3) goto start;
    
    // switch 语句中的标签
    int choice = 2;
    switch (choice) {
        case 1:
            std::cout << "Choice 1" << std::endl;
            break;
        case 2:
            std::cout << "Choice 2" << std::endl;
            break;
        case 3:
            std::cout << "Choice 3" << std::endl;
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
    }
    
    return 0;
}
```

**继承列表**：

```cpp
// 基类列表
class Base1 {
public:
    virtual void func1() {}
};

class Base2 {
public:
    virtual void func2() {}
};

class Derived : public Base1, private Base2 {
public:
    void func1() override {
        std::cout << "Derived::func1()" << std::endl;
    }
    
    // 使用 Base2 的功能
    void use_base2() {
        func2();  // 可以访问私有继承的成员函数
    }
};

// 虚继承
class VirtualBase {
public:
    int value;
};

class Derived1 : virtual public VirtualBase {
public:
    Derived1() { value = 1; }
};

class Derived2 : virtual public VirtualBase {
public:
    Derived2() { value = 2; }
};

class FinalDerived : public Derived1, public Derived2 {
    // value 只有一份拷贝，避免菱形继承问题
};
```

**构造函数初始化列表**：

```cpp
#include <iostream>
#include <string>
#include <vector>

class MyClass {
private:
    int id;
    std::string name;
    std::vector<int> data;
    const int constant_value;
    
public:
    // 构造函数初始化列表
    MyClass(int i, const std::string& n, int const_val) 
        : id(i), 
          name(n), 
          data(10, 0),  // 初始化 vector，10个元素都为0
          constant_value(const_val) {
        std::cout << "MyClass constructed" << std::endl;
    }
    
    // 委托构造函数（C++11）
    MyClass(int i) : MyClass(i, "default", 42) {}
    
    // 移动构造函数（C++11）
    MyClass(MyClass&& other) noexcept
        : id(other.id),
          name(std::move(other.name)),
          data(std::move(other.data)),
          constant_value(other.constant_value) {
        std::cout << "MyClass moved" << std::endl;
    }
};
```

**访问限定符**：

```cpp
class AccessExample {
public:      // 公有成员
    int public_member;
    void public_function() {}
    
protected:   // 保护成员
    int protected_member;
    void protected_function() {}
    
private:     // 私有成员
    int private_member;
    void private_function() {}
    
public:
    // 友元声明
    friend class FriendClass;
    friend void friend_function(AccessExample& obj);
};
```

**位字段成员声明**：

```cpp
#include <iostream>

// 位字段结构体
struct Flags {
    unsigned int flag1 : 1;  // 1位
    unsigned int flag2 : 1;  // 1位
    unsigned int type : 2;   // 2位 (0-3)
    unsigned int status : 4; // 4位 (0-15)
    unsigned int reserved : 24; // 24位保留
};

// 复杂位字段示例
struct PacketHeader {
    unsigned int version : 4;    // 版本号 (0-15)
    unsigned int ihl : 4;        // 首部长度 (0-15)
    unsigned int tos : 8;        // 服务类型 (0-255)
    unsigned int length : 16;    // 总长度 (0-65535)
};

int main() {
    Flags flags{1, 0, 2, 5, 0};
    
    std::cout << "flag1: " << flags.flag1 << std::endl;
    std::cout << "flag2: " << flags.flag2 << std::endl;
    std::cout << "type: " << flags.type << std::endl;
    std::cout << "status: " << flags.status << std::endl;
    
    PacketHeader header{4, 5, 0, 1000};
    std::cout << "Version: " << header.version 
              << ", IHL: " << header.ihl 
              << ", Length: " << header.length << std::endl;
    
    return 0;
}
```

**基于范围的 for 循环**：

```cpp
#include <iostream>
#include <vector>
#include <map>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 基于范围的 for 循环（C++11）
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    // 初始化和范围
    for (const auto& [key, value] : std::map<int, std::string>{{1, "one"}, {2, "two"}}) {
        std::cout << key << " => " << value << std::endl;
    }
    
    return 0;
}
```

### 2.7 省略号 `...`

省略号用于表示可变参数。

**可变参数函数**：

```cpp
#include <iostream>
#include <cstdarg>

// 可变参数函数实现
int sum(int count, ...) {
    va_list args;
    va_start(args, count);
    
    int total = 0;
    for (int i = 0; i < count; i++) {
        int value = va_arg(args, int);
        total += value;
    }
    
    va_end(args);
    return total;
}

// 格式化输出函数
void custom_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    vprintf(format, args);
    
    va_end(args);
}

int main() {
    int result = sum(5, 1, 2, 3, 4, 5);
    std::cout << "Sum: " << result << std::endl;
    
    custom_printf("Hello %s, you are %d years old\n", "Alice", 25);
    
    return 0;
}
```

**可变参数模板**：

```cpp
#include <iostream>

// 可变参数模板（C++11）
template<typename T>
void print(T&& t) {
    std::cout << t << std::endl;
}

template<typename T, typename... Args>
void print(T&& t, Args&&... args) {
    std::cout << t << " ";
    print(args...);
}

// C++17 折叠表达式
template<typename... Args>
void print_fold(Args&&... args) {
    (std::cout << ... << args) << std::endl;
}

// 参数包展开
template<typename... Args>
void expand_example(Args&&... args) {
    // sizeof... 运算符
    std::cout << "Number of arguments: " << sizeof...(args) << std::endl;
    
    // 参数包展开
    auto tuple = std::make_tuple(args...);
}

int main() {
    print(1, 2.5, "hello", 'c');
    print_fold(1, 2.5, "hello", 'c');
    
    expand_example(1, 2.5, "hello", 'c');
    
    return 0;
}
```

**可变参数宏（C99/C++11）**：

```cpp
#include <iostream>

// 基本可变参数宏
#define DEBUG_PRINT(fmt, ...) \
    printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)

#define LOG(level, fmt, ...) \
    printf("[%s] " fmt "\n", #level, ##__VA_ARGS__)

// C++17 __VA_OPT__ 支持
#define PRINT_ARGS(...) \
    std::cout << "Arguments:" __VA_OPT__(#__VA_ARGS__) << std::endl

int main() {
    DEBUG_PRINT("Simple message");
    DEBUG_PRINT("Value: %d", 42);
    DEBUG_PRINT("Name: %s, Age: %d", "Bob", 30);
    
    LOG(INFO, "Application started");
    LOG(ERROR, "File not found: %s", "data.txt");
    
    PRINT_ARGS();
    PRINT_ARGS(1, 2, 3);
    
    return 0;
}
```

**异常处理**：

```cpp
#include <iostream>
#include <exception>

void exception_example() {
    try {
        // 可能抛出异常的代码
        throw std::runtime_error("Something went wrong");
    }
    catch (...) {
        // 捕获所有异常
        std::cout << "Caught unknown exception" << std::endl;
    }
}
```

### 2.8 问号 `?`

问号是条件运算符的一部分。

**基本条件运算符**：

```cpp
#include <iostream>

int main() {
    int a = 10, b = 20;
    
    // 基本用法
    int max = (a > b) ? a : b;
    std::string min = (a < b) ? "a is smaller" : "b is smaller";
    
    std::cout << "Max: " << max << ", Min: " << min << std::endl;
    
    // 在表达式中使用
    int result = (a > 0) ? (a * 2) : (a / 2);
    std::cout << "Result: " << result << std::endl;
    
    return 0;
}
```

**复杂条件运算符**：

```cpp
#include <iostream>

int main() {
    // 嵌套条件运算符
    int score = 85;
    std::string grade = 
        (score >= 90) ? "A" :
        (score >= 80) ? "B" :
        (score >= 70) ? "C" :
        (score >= 60) ? "D" : "F";
    
    std::cout << "Score: " << score << ", Grade: " << grade << std::endl;
    
    // 条件运算符与指针
    int x = 10, y = 20;
    int* ptr = (x > y) ? &x : &y;
    std::cout << "Larger value: " << *ptr << std::endl;
    
    // 条件运算符与智能指针
    #include <memory>
    auto ptr1 = std::make_unique<int>(10);
    auto ptr2 = std::make_unique<int>(20);
    auto selected_ptr = (ptr1 && *ptr1 > *ptr2) ? std::move(ptr1) : std::move(ptr2);
    
    return 0;
}
```

### 2.9 作用域解析符 `::`

作用域解析符是 C++ 特有的重要符号。

**命名空间访问**：

```cpp
#include <iostream>

// 命名空间
namespace MyNamespace {
    int value = 42;
    
    class MyClass {
    public:
        static void func() {
            std::cout << "MyNamespace::MyClass::func()" << std::endl;
        }
    };
    
    namespace Inner {
        int inner_value = 100;
    }
}

// 全局作用域
int value = 100;

int main() {
    // 访问命名空间成员
    std::cout << MyNamespace::value << std::endl;
    MyNamespace::MyClass::func();
    std::cout << MyNamespace::Inner::inner_value << std::endl;
    
    // 全局作用域
    std::cout << ::value << std::endl;  // 全局 value
    
    return 0;
}
```

**类成员访问**：

```cpp
#include <iostream>

class MyClass {
public:
    static int static_value;
    int instance_value = 10;
    
    static void static_func() {
        std::cout << "Static function" << std::endl;
    }
    
    void instance_func() {
        std::cout << "Instance function" << std::endl;
    }
};

int MyClass::static_value = 42;

int main() {
    // 访问静态成员
    std::cout << MyClass::static_value << std::endl;
    MyClass::static_func();
    
    // 访问实例成员
    MyClass obj;
    std::cout << obj.instance_value << std::endl;
    obj.instance_func();
    
    return 0;
}
```

**枚举值访问**：

```cpp
// 作用域枚举（C++11）
enum class Color {
    RED,
    GREEN,
    BLUE
};

enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

int main() {
    Color c = Color::RED;
    Direction d = Direction::NORTH;
    
    // 必须使用作用域解析符访问枚举值
    if (c == Color::RED) {
        std::cout << "Color is red" << std::endl;
    }
    
    return 0;
}
```

### 2.10 点号 `.` 和箭头 `->`

这两个运算符用于成员访问。

**点号运算符**：

```cpp
#include <iostream>
#include <string>

class Person {
public:
    std::string name;
    int age;
    double height;
    
    Person(const std::string& n, int a, double h) 
        : name(n), age(a), height(h) {}
    
    void introduce() const {
        std::cout << "Hi, I'm " << name << ", " << age << " years old." << std::endl;
    }
};

struct Address {
    std::string street;
    std::string city;
    int zip;
};

class Employee {
public:
    Person person;
    Address address;
    double salary;
    
    Employee(const Person& p, const Address& addr, double s)
        : person(p), address(addr), salary(s) {}
};

int main() {
    // 基本类成员访问
    Person p("Alice", 25, 5.6);
    std::cout << "Name: " << p.name << ", Age: " << p.age 
              << ", Height: " << p.height << std::endl;
    
    // 调用成员函数
    p.introduce();
    
    // 修改成员
    p.name = "Bob";
    p.age = 30;
    
    // 嵌套结构体成员访问
    Address addr{"123 Main St", "New York", 10001};
    Employee emp(p, addr, 75000.0);
    
    std::cout << "Employee: " << emp.person.name 
              << ", Age: " << emp.person.age 
              << ", Salary: " << emp.salary << std::endl;
    
    return 0;
}
```

**箭头运算符**：

```cpp
#include <iostream>
#include <memory>

class Node {
public:
    int data;
    std::unique_ptr<Node> next;
    
    Node(int d) : data(d) {}
};

int main() {
    // 智能指针
    auto node = std::make_unique<Node>(42);
    std::cout << "Node data: " << node->data << std::endl;
    
    // 原始指针
    Node* raw_ptr = new Node(100);
    std::cout << "Raw pointer data: " << raw_ptr->data << std::endl;
    delete raw_ptr;
    
    // 链表示例
    auto head = std::make_unique<Node>(1);
    head->next = std::make_unique<Node>(2);
    head->next->next = std::make_unique<Node>(3);
    
    // 遍历链表
    Node* current = head.get();
    while (current != nullptr) {
        std::cout << "Data: " << current->data << std::endl;
        current = current->next.get();
    }
    
    return 0;
}
```

### 2.11 成员指针运算符 `.*` 和 `->*`

这些是 C++ 特有的运算符，用于通过成员指针访问成员。

**成员指针**：

```cpp
#include <iostream>

class MyClass {
public:
    int value;
    void func() {
        std::cout << "MyClass::func(), value = " << value << std::endl;
    }
    
    MyClass(int v) : value(v) {}
};

int main() {
    // 数据成员指针
    int MyClass::* member_ptr = &MyClass::value;
    
    // 成员函数指针
    void (MyClass::* func_ptr)() = &MyClass::func;
    
    MyClass obj(42);
    
    // 通过成员指针访问数据成员
    std::cout << "obj.*member_ptr = " << obj.*member_ptr << std::endl;
    
    // 通过成员指针调用成员函数
    (obj.*func_ptr)();
    
    // 动态分配对象
    MyClass* ptr = new MyClass(100);
    
    // 通过指针和成员指针访问
    std::cout << "ptr->*member_ptr = " << ptr->*member_ptr << std::endl;
    (ptr->*func_ptr)();
    
    delete ptr;
    
    return 0;
}
```

### 2.12 一元运算符

**按位取反 `~`**：

```cpp
#include <iostream>
#include <bitset>

int main() {
    unsigned char a = 0b10101010;  // 170 in decimal
    unsigned char b = ~a;          // 0b01010101 = 85 in decimal
    
    std::cout << "a = 0x" << std::hex << static_cast<int>(a) 
              << " (" << std::dec << static_cast<int>(a) << ")" << std::endl;
    std::cout << "~a = 0x" << std::hex << static_cast<int>(b) 
              << " (" << std::dec << static_cast<int>(b) << ")" << std::endl;
    
    // std::bitset 示例
    std::bitset<8> bits(a);
    std::cout << "Bits: " << bits << std::endl;
    std::cout << "Inverted: " << ~bits << std::endl;
    
    return 0;
}
```

**逻辑非 `!`**：

```cpp
#include <iostream>
#include <memory>

int main() {
    int a = 10;
    int b = 0;
    std::unique_ptr<int> ptr;
    
    std::cout << "a = " << a << ", !a = " << !a << std::endl;
    std::cout << "b = " << b << ", !b = " << !b << std::endl;
    std::cout << "ptr = " << (ptr ? "not null" : "null") 
              << ", !ptr = " << (!ptr ? "true" : "false") << std::endl;
    
    // 在条件判断中的应用
    if (!a) {
        std::cout << "a is zero or false" << std::endl;
    } else {
        std::cout << "a is non-zero or true" << std::endl;
    }
    
    // 智能指针检查
    if (!ptr) {
        std::cout << "ptr is null" << std::endl;
    }
    
    return 0;
}
```

**一元加减 `+` `-`**：

```cpp
#include <iostream>

int main() {
    int a = 10;
    int b = -20;
    
    // 一元加（通常无效果）
    int pos_a = +a;
    int pos_b = +b;
    
    // 一元减（取负）
    int neg_a = -a;
    int neg_b = -b;
    
    std::cout << "a = " << a << ", +a = " << pos_a << ", -a = " << neg_a << std::endl;
    std::cout << "b = " << b << ", +b = " << pos_b << ", -b = " << neg_b << std::endl;
    
    // 应用于表达式
    int result = -(a + b) + (+a * 2);
    std::cout << "Result: " << result << std::endl;
    
    return 0;
}
```

**取地址 `&` 和间接引用 `*`**：

```cpp
#include <iostream>
#include <memory>

int main() {
    int value = 42;
    int* ptr = &value;  // 取地址
    
    std::cout << "Value: " << value << std::endl;
    std::cout << "Address of value: " << &value << std::endl;
    std::cout << "Pointer value: " << ptr << std::endl;
    std::cout << "Dereferenced pointer: " << *ptr << std::endl;
    
    // 通过指针修改值
    *ptr = 100;
    std::cout << "Modified value: " << value << std::endl;
    
    // 智能指针
    auto smart_ptr = std::make_unique<int>(200);
    std::cout << "Smart pointer value: " << *smart_ptr << std::endl;
    
    // 指针算术
    int arr[5] = {1, 2, 3, 4, 5};
    int* arr_ptr = arr;  // 数组名退化为指针
    
    std::cout << "Array elements: ";
    for (int i = 0; i < 5; i++) {
        std::cout << *(arr_ptr + i) << " ";  // 等价于 arr_ptr[i]
    }
    std::cout << std::endl;
    
    return 0;
}
```

**自增自减 `++` `--`**：

```cpp
#include <iostream>
#include <vector>

int main() {
    int a = 10;
    int b = 10;
    
    // 前缀自增
    int pre_inc = ++a;
    std::cout << "a (before: 10): " << a << ", pre_inc: " << pre_inc << std::endl;
    
    // 后缀自增
    int post_inc = b++;
    std::cout << "b (before: 10): " << b << ", post_inc: " << post_inc << std::endl;
    
    // 自减运算
    int c = 20;
    int pre_dec = --c;
    std::cout << "c after pre-decrement: " << c << ", pre_dec: " << pre_dec << std::endl;
    
    // 在表达式中的应用
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();
    
    std::cout << "Vector traversal: ";
    while (it != vec.end()) {
        std::cout << *it++ << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

### 2.13 二元运算符

**算术运算符**：

```cpp
#include <iostream>
#include <complex>

int main() {
    int a = 17, b = 5;
    
    // 基本算术运算
    int sum = a + b;      // 22
    int diff = a - b;     // 12
    int prod = a * b;     // 85
    int quot = a / b;     // 3 (整数除法)
    int rem = a % b;      // 2 (取模)
    
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "a + b = " << sum << std::endl;
    std::cout << "a - b = " << diff << std::endl;
    std::cout << "a * b = " << prod << std::endl;
    std::cout << "a / b = " << quot << std::endl;
    std::cout << "a % b = " << rem << std::endl;
    
    // 浮点数运算
    double x = 17.0, y = 5.0;
    double fdiv = x / y;  // 3.4 (浮点除法)
    std::cout << "x / y = " << fdiv << std::endl;
    
    // 复数运算
    std::complex<double> c1(3.0, 4.0);
    std::complex<double> c2(1.0, 2.0);
    std::complex<double> c_sum = c1 + c2;
    std::complex<double> c_prod = c1 * c2;
    
    std::cout << "Complex sum: " << c_sum << std::endl;
    std::cout << "Complex product: " << c_prod << std::endl;
    
    return 0;
}
```

**位运算符**：

```cpp
#include <iostream>
#include <bitset>

// 打印二进制表示的辅助函数
template<size_t N>
void print_binary(const std::bitset<N>& bits) {
    std::cout << bits << std::endl;
}

int main() {
    unsigned int a = 0b1010;  // 10 in decimal
    unsigned int b = 0b1100;  // 12 in decimal
    
    std::bitset<8> bits_a(a);
    std::bitset<8> bits_b(b);
    
    std::cout << "a = " << a << " (binary: ";
    print_binary(bits_a);
    std::cout << "b = " << b << " (binary: ";
    print_binary(bits_b);
    
    // 按位与
    unsigned int and_result = a & b;
    std::bitset<8> bits_and(and_result);
    std::cout << "a & b = " << and_result << " (binary: ";
    print_binary(bits_and);
    
    // 按位或
    unsigned int or_result = a | b;
    std::bitset<8> bits_or(or_result);
    std::cout << "a | b = " << or_result << " (binary: ";
    print_binary(bits_or);
    
    // 按位异或
    unsigned int xor_result = a ^ b;
    std::bitset<8> bits_xor(xor_result);
    std::cout << "a ^ b = " << xor_result << " (binary: ";
    print_binary(bits_xor);
    
    // 左移
    unsigned int left_shift = a << 2;
    std::bitset<8> bits_left(left_shift);
    std::cout << "a << 2 = " << left_shift << " (binary: ";
    print_binary(bits_left);
    
    // 右移
    unsigned int right_shift = a >> 1;
    std::bitset<8> bits_right(right_shift);
    std::cout << "a >> 1 = " << right_shift << " (binary: ";
    print_binary(bits_right);
    
    return 0;
}
```

**关系运算符**：

```cpp
#include <iostream>
#include <string>

int main() {
    int a = 10, b = 20;
    
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "a == b: " << (a == b) << std::endl;
    std::cout << "a != b: " << (a != b) << std::endl;
    std::cout << "a < b: " << (a < b) << std::endl;
    std::cout << "a > b: " << (a > b) << std::endl;
    std::cout << "a <= b: " << (a <= b) << std::endl;
    std::cout << "a >= b: " << (a >= b) << std::endl;
    
    // 字符串比较
    std::string str1 = "hello";
    std::string str2 = "hello";
    std::string str3 = "world";
    
    std::cout << "str1 == str2: " << (str1 == str2) << std::endl;
    std::cout << "str1 < str3: " << (str1 < str3) << std::endl;
    
    // 三向比较运算符（C++20）
    auto result = a <=> b;
    if (result < 0) {
        std::cout << "a < b" << std::endl;
    } else if (result > 0) {
        std::cout << "a > b" << std::endl;
    } else {
        std::cout << "a == b" << std::endl;
    }
    
    return 0;
}
```

**逻辑运算符**：

```cpp
#include <iostream>
#include <memory>

int main() {
    int a = 1, b = 0, c = 1;
    std::unique_ptr<int> ptr1(new int(42));
    std::unique_ptr<int> ptr2;
    
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
    
    // 逻辑与
    int and_result = a && b;
    std::cout << "a && b = " << and_result << std::endl;
    
    int and_result2 = a && c;
    std::cout << "a && c = " << and_result2 << std::endl;
    
    // 逻辑或
    int or_result = a || b;
    std::cout << "a || b = " << or_result << std::endl;
    
    int or_result2 = b || b;
    std::cout << "b || b = " << or_result2 << std::endl;
    
    // 智能指针逻辑运算
    std::cout << "ptr1 && ptr2: " << (ptr1 && ptr2) << std::endl;
    std::cout << "ptr1 || ptr2: " << (ptr1 || ptr2) << std::endl;
    
    // 短路求值示例
    int x = 0;
    if (x != 0 && (10 / x) > 1) {
        std::cout << "This won't execute" << std::endl;
    } else {
        std::cout << "Short-circuit prevented division by zero" << std::endl;
    }
    
    return 0;
}
```

### 2.14 赋值运算符

**简单赋值 `=`**：

```cpp
#include <iostream>
#include <string>
#include <memory>

class MyClass {
public:
    int value;
    
    MyClass(int v = 0) : value(v) {
        std::cout << "Constructor: value = " << value << std::endl;
    }
    
    // 拷贝构造函数
    MyClass(const MyClass& other) : value(other.value) {
        std::cout << "Copy constructor: value = " << value << std::endl;
    }
    
    // 拷贝赋值运算符
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            value = other.value;
            std::cout << "Copy assignment: value = " << value << std::endl;
        }
        return *this;
    }
    
    // 移动构造函数（C++11）
    MyClass(MyClass&& other) noexcept : value(other.value) {
        other.value = 0;
        std::cout << "Move constructor: value = " << value << std::endl;
    }
    
    // 移动赋值运算符（C++11）
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            value = other.value;
            other.value = 0;
            std::cout << "Move assignment: value = " << value << std::endl;
        }
        return *this;
    }
    
    ~MyClass() {
        std::cout << "Destructor: value = " << value << std::endl;
    }
};

int main() {
    MyClass a(10);  // 初始化
    MyClass b;
    
    b = a;       // 拷贝赋值
    std::cout << "a.value = " << a.value << ", b.value = " << b.value << std::endl;
    
    // 移动赋值
    MyClass c(20);
    b = std::move(c);  // 移动赋值
    std::cout << "c.value = " << c.value << ", b.value = " << b.value << std::endl;
    
    // 链式赋值
    int x, y, z;
    x = y = z = 42;
    std::cout << "x = " << x << ", y = " << y << ", z = " << z << std::endl;
    
    return 0;
}
```

**复合赋值运算符**：

```cpp
#include <iostream>

class Counter {
private:
    int count;
    
public:
    Counter(int c = 0) : count(c) {}
    
    // 复合赋值运算符
    Counter& operator+=(const Counter& other) {
        count += other.count;
        return *this;
    }
    
    Counter& operator-=(const Counter& other) {
        count -= other.count;
        return *this;
    }
    
    Counter& operator*=(int factor) {
        count *= factor;
        return *this;
    }
    
    Counter& operator/=(int divisor) {
        if (divisor != 0) {
            count /= divisor;
        }
        return *this;
    }
    
    // 前置自增
    Counter& operator++() {
        ++count;
        return *this;
    }
    
    // 后置自增
    Counter operator++(int) {
        Counter temp(*this);
        ++(*this);
        return temp;
    }
    
    int getCount() const { return count; }
};

std::ostream& operator<<(std::ostream& os, const Counter& c) {
    os << "Counter(" << c.getCount() << ")";
    return os;
}

int main() {
    Counter a(20);
    Counter b(5);
    
    std::cout << "Initial values: a = " << a << ", b = " << b << std::endl;
    
    // 各种复合赋值
    a += b;   // a = a + b
    std::cout << "After +=: a = " << a << std::endl;
    
    a -= b;   // a = a - b
    std::cout << "After -=: a = " << a << std::endl;
    
    a *= 2;   // a = a * 2
    std::cout << "After *= 2: a = " << a << std::endl;
    
    a /= 4;   // a = a / 4
    std::cout << "After /= 4: a = " << a << std::endl;
    
    ++a;      // 前置自增
    std::cout << "After ++: a = " << a << std::endl;
    
    Counter c = a++;  // 后置自增
    std::cout << "After postfix ++: a = " << a << ", c = " << c << std::endl;
    
    return 0;
}
```

### 2.15 逗号运算符 `,`

逗号运算符具有最低的优先级，用于分隔表达式。

**逗号运算符**：

```cpp
#include <iostream>

int main() {
    // 逗号运算符：从左到右求值，返回最右边表达式的值
    int a = 1, b = 2, c = 3;
    int result = (a++, b++, c++);  // result = 3, a=2, b=3, c=4
    
    std::cout << "a = " << a << ", b = " << b << ", c = " << c 
              << ", result = " << result << std::endl;
    
    // 在 for 循环中的应用
    for (int i = 0, j = 10; i < j; i++, j--) {
        std::cout << "i = " << i << ", j = " << j << std::endl;
    }
    
    // 复杂的逗号表达式
    int x = 0, y = 0;
    int complex_result = (x = 5, y = 10, x + y);  // complex_result = 15
    
    std::cout << "x = " << x << ", y = " << y 
              << ", complex_result = " << complex_result << std::endl;
    
    return 0;
}
```

**列表分隔符**：

```cpp
#include <iostream>
#include <vector>
#include <initializer_list>

// 函数参数分隔
void function(int a, int b, int c) {
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
}

// 初始化列表构造函数
class Data {
public:
    std::vector<int> values;
    
    Data(std::initializer_list<int> init_list) : values(init_list) {}
};

// 声明分隔
int x, y, z;

int main() {
    // 函数调用参数分隔
    function(10, 20, 30);
    
    // 变量初始化
    x = 1, y = 2, z = 3;
    std::cout << "x = " << x << ", y = " << y << ", z = " << z << std::endl;
    
    // 统一初始化语法（C++11）
    std::vector<int> vec{1, 2, 3, 4, 5};
    Data data{10, 20, 30, 40, 50};
    
    std::cout << "Vector: ";
    for (const auto& item : vec) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Data values: ";
    for (const auto& item : data.values) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

## 3. 运算符优先级和结合性

### 3.1 完整优先级表

| 优先级 | 运算符 | 描述 | 结合性 |
|--------|--------|------|--------|
| 1 | `::` | 作用域解析 | 左到右 |
| 2 | `()` `[]` `->` `.` `++` `--` | 后缀运算符 | 左到右 |
| 3 | `++` `--` `+` `-` `!` `~` `*` `&` `(type)` `sizeof` `alignof` `noexcept` `new` `delete` | 一元运算符 | 右到左 |
| 4 | `.*` `->*` | 成员指针 | 左到右 |
| 5 | `*` `/` `%` | 乘法运算符 | 左到右 |
| 6 | `+` `-` | 加法运算符 | 左到右 |
| 7 | `<<` `>>` | 位移运算符 | 左到右 |
| 8 | `<` `<=` `>` `>=` `<=>` | 关系运算符 | 左到右 |
| 9 | `==` `!=` | 相等运算符 | 左到右 |
| 10 | `&` | 按位与 | 左到右 |
| 11 | `^` | 按位异或 | 左到右 |
| 12 | `\|` |
| 13 | `&&` | 逻辑与 | 左到右 |
| 14 | `\|\|` | 逻辑或 | 左到右 |
| 15 | `?:` | 条件运算符 | 右到左 |
| 16 | `=` `+=` `-=` `*=` `/=` `%=` `<<=` `>>=` `&=` `^=` `|=` | 赋值运算符 | 右到左 |
| 17 | `throw` | 抛出异常 | 右到左 |
| 18 | `,` | 逗号运算符 | 左到右 |

### 3.2 优先级示例

```cpp
#include <iostream>

int main() {
    int a = 5, b = 3, c = 2;
    
    // 乘法优先级高于加法
    int result1 = a + b * c;        // 5 + (3 * 2) = 11
    int result2 = (a + b) * c;      // (5 + 3) * 2 = 16
    
    std::cout << "a + b * c = " << result1 << std::endl;
    std::cout << "(a + b) * c = " << result2 << std::endl;
    
    // 关系运算符优先级高于逻辑运算符
    int x = 10, y = 20, z = 15;
    bool result3 = x > y && y < z;   // (x > y) && (y < z) = false && true = false
    bool result4 = x > y || y < z;   // (x > y) || (y < z) = false || true = true
    
    std::cout << "x > y && y < z = " << result3 << std::endl;
    std::cout << "x > y || y < z = " << result4 << std::endl;
    
    // 赋值运算符的右结合性
    int p, q, r;
    p = q = r = 42;  // p = (q = (r = 42))
    
    std::cout << "p = " << p << ", q = " << q << ", r = " << r << std::endl;
    
    // 复杂表达式
    int complex = a + b * c - (a / b) && (c || b);
    // 等价于: ((a + (b * c)) - (a / b)) && (c || b)
    
    std::cout << "Complex expression result: " << complex << std::endl;
    
    return 0;
}
```

## 4. 特殊用法和现代 C++ 特性

### 4.1 C++11 特性

**统一初始化语法**：

```cpp
#include <iostream>
#include <vector>
#include <map>

class Point {
public:
    int x, y;
    
    Point(int x_val, int y_val) : x(x_val), y(y_val) {}
    
    // 初始化列表构造函数
    Point(std::initializer_list<int> list) {
        auto it = list.begin();
        x = *it++;
        if (it != list.end()) y = *it;
    }
};

int main() {
    // 统一初始化语法
    int a{5};
    double b{3.14};
    char c{'A'};
    
    // 容器初始化
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::map<std::string, int> map{{"one", 1}, {"two", 2}, {"three", 3}};
    
    // 类初始化
    Point p1{10, 20};        // 普通构造函数
    Point p2{30, 40, 50};    // 初始化列表构造函数（只取前两个值）
    
    std::cout << "p1: (" << p1.x << ", " << p1.y << ")" << std::endl;
    std::cout << "p2: (" << p2.x << ", " << p2.y << ")" << std::endl;
    
    return 0;
}
```

**Lambda 表达式**：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // 基本 Lambda 表达式
    auto square = [](int x) { return x * x; };
    std::cout << "Square of 5: " << square(5) << std::endl;
    
    // 带捕获的 Lambda 表达式
    int factor = 10;
    auto multiply = [factor](int x) { return x * factor; };
    std::cout << "5 * factor: " << multiply(5) << std::endl;
    
    // 引用捕获
    auto modify = [&factor](int x) { factor += x; };
    modify(5);
    std::cout << "Modified factor: " << factor << std::endl;
    
    // 混合捕获
    int a = 1, b = 2;
    auto mixed = [a, &b]() {
        b += a;
        return b;
    };
    std::cout << "Mixed capture result: " << mixed() << std::endl;
    
    // 泛型 Lambda（C++14）
    auto printer = [](const auto& value) {
        std::cout << value << " ";
    };
    
    std::cout << "Vector elements: ";
    std::for_each(numbers.begin(), numbers.end(), printer);
    std::cout << std::endl;
    
    return 0;
}
```

**自动类型推导**：

```cpp
#include <iostream>
#include <vector>
#include <map>

int main() {
    // auto 关键字
    auto i = 42;           // int
    auto d = 3.14;         // double
    auto s = "hello";      // const char*
    auto v = std::vector<int>{1, 2, 3};  // std::vector<int>
    
    std::cout << "i = " << i << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "s = " << s << std::endl;
    
    // decltype 关键字
    decltype(i) j = 100;   // int
    decltype(d) e = 2.71;  // double
    
    std::cout << "j = " << j << std::endl;
    std::cout << "e = " << e << std::endl;
    
    // 返回类型后置语法
    auto func = [](int x, int y) -> int {
        return x + y;
    };
    
    std::cout << "Function result: " << func(3, 4) << std::endl;
    
    return 0;
}
```

### 4.2 C++14 特性

**泛型 Lambda**：

```cpp
#include <iostream>
#include <vector>

int main() {
    // 泛型 Lambda（C++14）
    auto printer = [](const auto& value) {
        std::cout << value << " ";
    };
    
    std::vector<int> int_vec = {1, 2, 3};
    std::vector<double> double_vec = {1.1, 2.2, 3.3};
    std::vector<std::string> string_vec = {"hello", "world"};
    
    std::cout << "Int vector: ";
    for (const auto& item : int_vec) {
        printer(item);
    }
    std::cout << std::endl;
    
    std::cout << "Double vector: ";
    for (const auto& item : double_vec) {
        printer(item);
    }
    std::cout << std::endl;
    
    std::cout << "String vector: ";
    for (const auto& item : string_vec) {
        printer(item);
    }
    std::cout << std::endl;
    
    return 0;
}
```

**变量模板**：

```cpp
#include <iostream>
#include <type_traits>

// 变量模板（C++14）
template<typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;

template<typename T>
constexpr bool is_floating_point_v = std::is_floating_point<T>::value;

int main() {
    std::cout << "int is integral: " << is_integral_v<int> << std::endl;
    std::cout << "double is integral: " << is_integral_v<double> << std::endl;
    std::cout << "double is floating point: " << is_floating_point_v<double> << std::endl;
    
    return 0;
}
```

### 4.3 C++17 特性

**结构化绑定**：

```cpp
#include <iostream>
#include <tuple>
#include <map>

int main() {
    // 元组结构化绑定
    std::tuple<int, std::string, double> t{42, "hello", 3.14};
    auto [id, name, value] = t;
    
    std::cout << "ID: " << id << ", Name: " << name << ", Value: " << value << std::endl;
    
    // 数组结构化绑定
    int arr[] = {1, 2, 3};
    auto [a, b, c] = arr;
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
    
    // 结构体结构化绑定
    struct Point {
        int x, y;
    };
    
    Point p{10, 20};
    auto [x, y] = p;
    std::cout << "Point: (" << x << ", " << y << ")" << std::endl;
    
    // map 遍历中的结构化绑定
    std::map<std::string, int> m{{"one", 1}, {"two", 2}};
    for (const auto& [key, value] : m) {
        std::cout << key << " => " << value << std::endl;
    }
    
    return 0;
}
```

**if constexpr**：

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
void process(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Processing integral: " << value << std::endl;
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Processing floating point: " << value << std::endl;
    } else {
        std::cout << "Processing other type" << std::endl;
    }
}

int main() {
    process(42);      // integral
    process(3.14);    // floating point
    process("hello"); // other type
    
    return 0;
}
```

### 4.4 C++20 特性

**三向比较运算符（飞船运算符）**：

```cpp
#include <iostream>
#include <compare>

class Point {
public:
    int x, y;
    
    Point(int x_val, int y_val) : x(x_val), y(y_val) {}
    
    // 默认三向比较（C++20）
    auto operator<=>(const Point&) const = default;
    
    // 或者自定义三向比较
    // std::strong_ordering operator<=>(const Point& other) const {
    //     if (auto cmp = x <=> other.x; cmp != 0) return cmp;
    //     return y <=> other.y;
    // }
};

int main() {
    Point p1(1, 2);
    Point p2(3, 4);
    
    auto result = p1 <=> p2;
    
    if (result < 0) {
        std::cout << "p1 < p2" << std::endl;
    } else if (result > 0) {
        std::cout << "p1 > p2" << std::endl;
    } else {
        std::cout << "p1 == p2" << std::endl;
    }
    
    // 三向比较运算符还支持所有其他比较运算符
    std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
    std::cout << "p1 != p2: " << (p1 != p2) << std::endl;
    std::cout << "p1 < p2: " << (p1 < p2) << std::endl;
    std::cout << "p1 <= p2: " << (p1 <= p2) << std::endl;
    std::cout << "p1 > p2: " << (p1 > p2) << std::endl;
    std::cout << "p1 >= p2: " << (p1 >= p2) << std::endl;
    
    return 0;
}
```

**概念（Concepts）**：

```cpp
#include <iostream>
#include <concepts>

// 定义概念
template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

// 使用概念约束模板
template<Integral T>
void process_integral(T value) {
    std::cout << "Processing integral: " << value << std::endl;
}

template<FloatingPoint T>
void process_floating_point(T value) {
    std::cout << "Processing floating point: " << value << std::endl;
}

// 或者使用 requires 子句
template<typename T>
requires std::integral<T>
void process_integral_v2(T value) {
    std::cout << "Processing integral (v2): " << value << std::endl;
}

int main() {
    process_integral(42);
    process_floating_point(3.14);
    process_integral_v2(100);
    
    return 0;
}
```

## 5. 常见错误和最佳实践

### 5.1 常见陷阱

**悬空 else 问题**：

```cpp
#include <iostream>

// 错误理解
void bad_example(int a, int b, int c) {
    if (a > 0)
        if (b > 0)
            std::cout << "Both a and b are positive" << std::endl;
    else  // 这个 else 属于哪个 if？
        std::cout << "This is confusing" << std::endl;
}

// 正确写法
void good_example(int a, int b, int c) {
    if (a > 0) {
        if (b > 0) {
            std::cout << "Both a and b are positive" << std::endl;
        } else {
            std::cout << "a is positive, b is not" << std::endl;
        }
    } else {
        std::cout << "a is not positive" << std::endl;
    }
}
```

**赋值与比较混淆**：

```cpp
#include <iostream>

void assignment_vs_comparison() {
    int a = 10;
    
    // 错误写法
    // if (a = 5) {  // 这是赋值，不是比较
    //     std::cout << "This always executes" << std::endl;
    // }
    
    // 正确写法
    if (a == 5) {
        std::cout << "a equals 5" << std::endl;
    } else {
        std::cout << "a does not equal 5" << std::endl;
    }
    
    // Yoda 条件（防止意外赋值）
    if (5 == a) {
        std::cout << "a equals 5" << std::endl;
    }
    
    // 如果写错为 =，编译器会报错
    // if (5 = a) {  // 编译错误
    //     // ...
    // }
}
```

**逗号运算符的误用**：

```cpp
#include <iostream>

void comma_operator_mistakes() {
    // 错误：这不是逗号运算符
    // int a = 1, 2, 3;  // 编译错误！
    
    // 正确的声明
    int a = 1, b = 2, c = 3;  // 声明分隔符
    
    // 正确使用逗号运算符
    int result = (1, 2, 3);  // result = 3，使用逗号运算符
    std::cout << "result = " << result << std::endl;
    
    // 在 for 循环中的正确使用
    for (int i = 0, j = 10; i < j; i++, j--) {
        std::cout << "i = " << i << ", j = " << j << std::endl;
    }
}
```

### 5.2 最佳实践

**括号的使用**：

```cpp
#include <iostream>

void best_practices() {
    int a = 10, b = 5, c = 2;
    
    // 即使优先级明确，也使用括号提高可读性
    int result1 = a + b * c;      // 可以工作，但...
    int result2 = a + (b * c);    // 更清晰
    
    // 复杂表达式必须使用括号
    int complex = ((a + b) * c) - (a / b);
    
    std::cout << "result1 = " << result1 << ", result2 = " << result2 
              << ", complex = " << complex << std::endl;
}
```

**智能指针的使用**：

```cpp
#include <iostream>
#include <memory>

class Resource {
public:
    Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " acquired" << std::endl;
    }
    
    ~Resource() {
        std::cout << "Resource " << id_ << " released" << std::endl;
    }
    
    void use() {
        std::cout << "Using resource " << id_ << std::endl;
    }
    
private:
    int id_;
};

void smart_pointer_best_practices() {
    // 使用 make_unique 创建对象
    auto resource = std::make_unique<Resource>(1);
    resource->use();
    
    // 使用 make_shared 创建共享对象
    auto shared_resource = std::make_shared<Resource>(2);
    shared_resource->use();
    
    // 避免直接使用 new/delete
    // Resource* raw_ptr = new Resource(3);  // 不推荐
    // delete raw_ptr;  // 容易忘记或出现异常安全问题
    
    // 使用智能指针自动管理资源
}
```

**现代 C++ 特性使用**：

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void modern_cpp_best_practices() {
    // 使用 auto 进行类型推导
    auto numbers = std::vector{1, 2, 3, 4, 5};  // C++17 类模板参数推导
    
    // 使用范围 for 循环
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // 使用 Lambda 表达式
    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        std::cout << n * n << " ";
    });
    std::cout << std::endl;
    
    // 使用 constexpr 提高编译时计算
    constexpr int square(int x) {
        return x * x;
    }
    
    constexpr int result = square(5);  // 编译时计算
    std::cout << "Square of 5: " << result << std::endl;
}
```

## 6. 标准参考

这些标点符号在各版本 C++ 标准中均有定义：

- C++23 标准：ISO/IEC 14882:2024，5.12 运算符和标点符号 [lex.operators]
- C++20 标准：ISO/IEC 14882:2020，5.12 运算符和标点符号 [lex.operators]
- C++17 标准：ISO/IEC 14882:2017，5.12 运算符和标点符号 [lex.operators]
- C++14 标准：ISO/IEC 14882:2014，2.13 运算符和标点符号 [lex.operators]
- C++11 标准：ISO/IEC 14882:2011，2.13 运算符和标点符号 [lex.operators]
- C++03 标准：ISO/IEC 14882:2003，2.12 运算符和标点符号 [lex.operators]
- C++98 标准：ISO/IEC 14882:1998，2.12 运算符和标点符号 [lex.operators]

## 7. 总结

C++ 标点符号是语言语法的基础组成部分，每个符号都有其特定的语义和多种用途。与 C 语言相比，C++ 增加了许多新的标点符号以支持面向对象编程、模板编程、异常处理、Lambda 表达式等现代特性。正确理解和使用这些标点符号对于编写高质量的 C++ 程序至关重要。

通过深入学习每个标点符号的功能、用法和最佳实践，开发者可以：

- 提高代码可读性：正确使用标点符号使代码结构清晰
- 避免常见错误：理解优先级和结合性避免逻辑错误
- 利用现代特性：掌握 C++11、C++14、C++17、C++20 的新特性
- 编写高效代码：合理使用运算符优化性能
- 增强代码安全性：避免悬空 else、赋值比较混淆等问题

随着 C++ 标准的不断演进，新的标点符号和语法特性不断被引入，如三向比较运算符 `<=>`（C++20）、结构化绑定 `[ ]`（C++17）等，这些都进一步丰富了 C++ 的表达能力。

掌握这些标点符号不仅是语法学习的要求，更是成为优秀 C++ 程序员的基础。通过持续学习和实践，开发者可以更好地利用 C++ 的强大功能来构建高质量的软件系统。
