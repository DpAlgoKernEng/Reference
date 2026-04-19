# C++ 类型完全详解

  [1. 类型基本概念](#1-类型基本概念)

  [2. 类型分类](#2-类型分类)
  
  [3. 基本类型](#3-基本类型)
  
  [4. 复合类型](#4-复合类型)
  
  [5. 类型特征层次结构](#5-类型特征层次结构)
  
  [6. 类型命名](#6-类型命名)
  
  [7. 静态类型与动态类型](#7-静态类型与动态类型)
  
  [8. 不完整类型](#8-不完整类型)
  
  [9. 最佳实践](#9-最佳实践)
  
  [10. 标准参考](#10-标准参考)
  
  [11. 总结](#11-总结)

## 1. 类型基本概念

### 1.1 什么是 C++ 类型

在 C++ 中，类型（Type）是对象、引用、函数和表达式的一个基本属性，它既限制了这些实体允许的操作，又为原本通用的比特序列提供了语义意义。

```cpp
#include <iostream>
#include <typeinfo>

void type_basics() {
    // 不同类型的变量示例
    int integer_var = 42;           // 整数类型
    double double_var = 3.14;       // 浮点类型
    char char_var = 'A';            // 字符类型
    bool bool_var = true;           // 布尔类型
    
    // 类型决定了允许的操作
    int result = integer_var + 10;  // 整数支持算术运算
    // char_var + 10.5;             // 错误：混合类型运算需要转换
    
    std::cout << "Type basics:" << std::endl;
    std::cout << "  int: " << typeid(integer_var).name() << " = " << integer_var << std::endl;
    std::cout << "  double: " << typeid(double_var).name() << " = " << double_var << std::endl;
    std::cout << "  char: " << typeid(char_var).name() << " = " << char_var << std::endl;
    std::cout << "  bool: " << typeid(bool_var).name() << " = " << bool_var << std::endl;
    
    // 类型为比特序列提供语义意义
    unsigned char bytes[] = {0x40, 0x49, 0x0F, 0xDB};
    float* float_ptr = reinterpret_cast<float*>(bytes);
    int* int_ptr = reinterpret_cast<int*>(bytes);
    
    std::cout << "  Same bytes as float: " << *float_ptr << std::endl;
    std::cout << "  Same bytes as int: " << *int_ptr << std::endl;
}

// 类型操作示例
void type_operations() {
    // 算术类型支持算术运算
    int a = 10, b = 20;
    int sum = a + b;
    
    // 指针类型支持指针运算
    int array[5] = {1, 2, 3, 4, 5};
    int* ptr = array;
    ptr++;  // 指针算术
    
    // 函数类型支持函数调用
    auto lambda = [](int x) { return x * 2; };
    int doubled = lambda(21);
    
    std::cout << "Type operations:" << std::endl;
    std::cout << "  Sum: " << sum << std::endl;
    std::cout << "  Array element: " << *ptr << std::endl;
    std::cout << "  Lambda result: " << doubled << std::endl;
}
```

### 1.2 类型的重要性

类型在 C++ 中具有以下重要作用：

```cpp
#include <iostream>
#include <vector>
#include <memory>

void type_importance() {
    std::cout << "Type importance examples:" << std::endl;
    
    // 1. 内存分配
    int int_array[10];              // 分配 10 * sizeof(int) 字节
    double double_array[10];        // 分配 10 * sizeof(double) 字节
    
    std::cout << "  int array size: " << sizeof(int_array) << " bytes" << std::endl;
    std::cout << "  double array size: " << sizeof(double_array) << " bytes" << std::endl;
    
    // 2. 运算规则
    int int_a = 5, int_b = 2;
    double double_a = 5.0, double_b = 2.0;
    
    std::cout << "  Integer division: " << int_a << " / " << int_b << " = " << (int_a / int_b) << std::endl;
    std::cout << "  Double division: " << double_a << " / " << double_b << " = " << (double_a / double_b) << std::endl;
    
    // 3. 参数传递和类型安全
    void int_function(int value) {
        std::cout << "    Integer parameter: " << value << std::endl;
    }
    
    void double_function(double value) {
        std::cout << "    Double parameter: " << value << std::endl;
    }
    
    int_function(42);
    double_function(3.14);
    
    // 4. 类型安全
    // int* ptr = 42;  // 编译错误：类型不匹配
    
    // 5. 模板类型推导
    auto value1 = 42;        // 推导为 int
    auto value2 = 3.14;      // 推导为 double
    auto value3 = "Hello";   // 推导为 const char*
    
    std::cout << "  Auto deduced types:" << std::endl;
    std::cout << "    value1: " << typeid(value1).name() << " = " << value1 << std::endl;
    std::cout << "    value2: " << typeid(value2).name() << " = " << value2 << std::endl;
    std::cout << "    value3: " << typeid(value3).name() << " = " << value3 << std::endl;
}
```

## 2. 类型分类

### 2.1 基本类型与复合类型

C++ 类型系统将类型分为基本类型和复合类型两大类：

```cpp
#include <iostream>
#include <type_traits>

void type_classification() {
    std::cout << "Type classification:" << std::endl;
    
    // 基本类型 (fundamental types)
    void void_type_example();                    // void 类型
    std::nullptr_t null_ptr_type_example();      // nullptr_t 类型 (C++11)
    bool bool_type_example = true;               // 布尔类型
    char char_type_example = 'A';                // 字符类型
    int int_type_example = 42;                   // 整数类型
    float float_type_example = 3.14f;            // 浮点类型
    
    // 复合类型 (compound types)
    int* pointer_type_example = &int_type_example;        // 指针类型
    int& reference_type_example = int_type_example;       // 引用类型
    int array_type_example[5] = {1, 2, 3, 4, 5};         // 数组类型
    auto function_type_example = [](int x) { return x; }; // 函数类型
    enum EnumType { VALUE1, VALUE2 } enum_type_example;   // 枚举类型
    struct StructType { int member; } struct_type_example; // 类类型
    
    std::cout << "  Fundamental types:" << std::endl;
    std::cout << "    is_fundamental<void>: " << std::is_fundamental_v<void> << std::endl;
    std::cout << "    is_fundamental<bool>: " << std::is_fundamental_v<bool> << std::endl;
    std::cout << "    is_fundamental<int>: " << std::is_fundamental_v<int> << std::endl;
    
    std::cout << "  Compound types:" << std::endl;
    std::cout << "    is_compound<int*>: " << std::is_compound_v<int*> << std::endl;
    std::cout << "    is_compound<int&>: " << std::is_compound_v<int&> << std::endl;
    std::cout << "    is_compound<int[5]>: " << std::is_compound_v<int[5]> << std::endl;
}

// 标量类型示例
void scalar_types() {
    std::cout << "Scalar types:" << std::endl;
    
    // 算术类型
    bool bool_scalar = true;
    int int_scalar = 42;
    double double_scalar = 3.14;
    
    // 枚举类型
    enum Color { RED, GREEN, BLUE };
    Color enum_scalar = RED;
    
    // 指针类型
    int* pointer_scalar = nullptr;
    
    // 成员指针类型
    struct Example {
        int member;
        void method() {}
    };
    int Example::* member_pointer_scalar = &Example::member;
    void (Example::* function_pointer_scalar)() = &Example::method;
    
    // std::nullptr_t (C++11)
    std::nullptr_t nullptr_scalar = nullptr;
    
    std::cout << "  is_scalar<bool>: " << std::is_scalar_v<bool> << std::endl;
    std::cout << "  is_scalar<int>: " << std::is_scalar_v<int> << std::endl;
    std::cout << "  is_scalar<Color>: " << std::is_scalar_v<Color> << std::endl;
    std::cout << "  is_scalar<int*>: " << std::is_scalar_v<int*> << std::endl;
    std::cout << "  is_scalar<decltype(member_pointer_scalar)>: " << std::is_scalar_v<decltype(member_pointer_scalar)> << std::endl;
    std::cout << "  is_scalar<std::nullptr_t>: " << std::is_scalar_v<std::nullptr_t> << std::endl;
}
```

### 2.2 对象类型

对象类型是一种（可能是 cv 修饰的）类型，它不是函数类型，不是引用类型，也不是（可能是 cv 修饰的）void。

```cpp
#include <iostream>
#include <type_traits>

void object_types() {
    std::cout << "Object types:" << std::endl;
    
    // 对象类型示例
    int object1 = 42;                    // int 是对象类型
    double object2 = 3.14;               // double 是对象类型
    char object3 = 'A';                  // char 是对象类型
    struct Point { int x, y; } object4;  // 类类型是对象类型
    enum Status { OK, ERROR } object5;   // 枚举类型是对象类型
    int array[10];                       // 数组类型是对象类型
    int* pointer = nullptr;              // 指针类型是对象类型
    
    std::cout << "  is_object<int>: " << std::is_object_v<int> << std::endl;
    std::cout << "  is_object<double>: " << std::is_object_v<double> << std::endl;
    std::cout << "  is_object<Point>: " << std::is_object_v<Point> << std::endl;
    std::cout << "  is_object<Status>: " << std::is_object_v<Status> << std::endl;
    std::cout << "  is_object<int[10]>: " << std::is_object_v<int[10]> << std::endl;
    std::cout << "  is_object<int*>: " << std::is_object_v<int*> << std::endl;
    
    // 非对象类型示例
    void function_example();             // 函数类型不是对象类型
    int& reference_example = object1;    // 引用类型不是对象类型
    void* void_ptr = nullptr;            // void* 是对象类型（指向 void 的指针）
    // void void_object;                 // void 不是对象类型
    
    std::cout << "  is_object<void>: " << std::is_object_v<void> << std::endl;
    std::cout << "  is_object<int&>: " << std::is_object_v<int&> << std::endl;
    std::cout << "  is_object<void*>: " << std::is_object_v<void*> << std::endl;
    std::cout << "  is_object<decltype(function_example)>: " << std::is_object_v<decltype(function_example)> << std::endl;
}
```

### 2.3 已弃用的类别

**（在 C++20 中已弃用）**：

以下类型统称为 POD 类型 （另见 std::is_pod）：

- 标量类型
- POD 类
- 这些类型的数组
- 这些类型的 cv 限定版本

**(C++11 起，在 C++26 中已弃用)**：

下列类型统称为 trivial types（另见 std::is_trivial）：

- 标量类型
- 无特性类类型
- 这些类型的数组
- 这些类型的 cv 限定版本

### 2.4 程序定义类型

一个程序定义的特化是一个显式特化或部分特化 ，它不是 C++ 标准库的一部分，也不是由实现定义的。

一个程序定义的类型是以下类型之一：

- 一个非-闭包 (C++11) 类类型或枚举类型，它不是 C++ 标准库的一部分，也不是由实现定义的。
- 一个非实现提供的 lambda 表达式的闭包类型(C++11)。
- 一个程序定义特化的实例化。

## 3. 基本类型

### 3.1 void 类型

void 类型表示无类型，主要用于以下场景：

```cpp
#include <iostream>

void void_type_examples() {
    std::cout << "Void type examples:" << std::endl;
    
    // 1. 函数返回类型
    void print_message() {
        std::cout << "    Hello from void function" << std::endl;
    }
    
    print_message();
    
    // 2. 通用指针类型
    void* generic_ptr;
    int value = 42;
    generic_ptr = &value;
    
    // 使用时需要类型转换
    int* int_ptr = static_cast<int*>(generic_ptr);
    std::cout << "    Value through void pointer: " << *int_ptr << std::endl;
    
    // 3. 函数参数（表示接受任意参数）
    void generic_function(void* data, size_t size) {
        std::cout << "    Processing " << size << " bytes of data" << std::endl;
    }
    
    char buffer[100];
    generic_function(buffer, sizeof(buffer));
    
    // 4. void 不是对象类型
    // void void_object;  // 错误：不能声明 void 类型的对象
}

// C++11 的 std::nullptr_t
void nullptr_t_examples() {
    #if __cplusplus >= 201103L
    std::cout << "nullptr_t examples:" << std::endl;
    
    // std::nullptr_t 是一种标量类型
    std::nullptr_t null_ptr = nullptr;
    void* void_ptr = nullptr;
    int* int_ptr = nullptr;
    
    std::cout << "    nullptr type: " << typeid(nullptr).name() << std::endl;
    std::cout << "    nullptr value: " << (null_ptr == nullptr ? "null" : "not null") << std::endl;
    
    // nullptr 可以隐式转换为任何指针类型
    if (void_ptr == nullptr) {
        std::cout << "    void_ptr is null" << std::endl;
    }
    
    if (int_ptr == nullptr) {
        std::cout << "    int_ptr is null" << std::endl;
    }
    #endif
}
```

### 3.2 算术类型

**布尔类型**：

```cpp
#include <iostream>
#include <type_traits>

void bool_type_examples() {
    std::cout << "Bool type examples:" << std::endl;
    
    // 基本布尔类型
    bool flag1 = true;
    bool flag2 = false;
    bool flag3 = 1;    // 非零值转换为 true
    bool flag4 = 0;    // 零值转换为 false
    
    std::cout << "  bool values: " << flag1 << ", " << flag2 << ", " << flag3 << ", " << flag4 << std::endl;
    
    // 布尔类型与其他类型的转换
    int int_value = 42;
    bool converted_bool = static_cast<bool>(int_value);  // 非零值转换为 true
    std::cout << "  Converted from int " << int_value << " to bool: " << converted_bool << std::endl;
    
    double zero_double = 0.0;
    bool zero_bool = static_cast<bool>(zero_double);  // 零值转换为 false
    std::cout << "  Converted from double " << zero_double << " to bool: " << zero_bool << std::endl;
    
    // 布尔类型到其他类型的转换
    int true_as_int = static_cast<int>(true);
    int false_as_int = static_cast<int>(false);
    std::cout << "  true as int: " << true_as_int << ", false as int: " << false_as_int << std::endl;
    
    // 类型特征
    std::cout << "  is_arithmetic<bool>: " << std::is_arithmetic_v<bool> << std::endl;
    std::cout << "  is_integral<bool>: " << std::is_integral_v<bool> << std::endl;
}

// 布尔类型在条件表达式中的使用
void bool_in_conditions() {
    bool condition = true;
    
    std::cout << "Bool in conditions:" << std::endl;
    
    if (condition) {
        std::cout << "  Condition is true" << std::endl;
    } else {
        std::cout << "  Condition is false" << std::endl;
    }
    
    // 布尔类型在循环中的使用
    bool continue_loop = true;
    int counter = 0;
    
    while (continue_loop && counter < 3) {
        std::cout << "  Loop iteration " << counter << std::endl;
        counter++;
        if (counter >= 3) {
            continue_loop = false;
        }
    }
}
```

**字符类型**：

```cpp
#include <iostream>
#include <type_traits>
#include <climits>

void char_type_examples() {
    std::cout << "Character type examples:" << std::endl;
    
    // 窄字符类型
    char c1 = 'A';              // 普通字符类型
    signed char c2 = -128;      // 有符号字符类型
    unsigned char c3 = 255;     // 无符号字符类型
    
    std::cout << "  char: " << c1 << " (" << static_cast<int>(c1) << ")" << std::endl;
    std::cout << "  signed char: " << static_cast<int>(c2) << std::endl;
    std::cout << "  unsigned char: " << static_cast<int>(c3) << std::endl;
    
    // 字符类型范围
    std::cout << "  CHAR_MIN: " << CHAR_MIN << ", CHAR_MAX: " << CHAR_MAX << std::endl;
    std::cout << "  SCHAR_MIN: " << SCHAR_MIN << ", SCHAR_MAX: " << SCHAR_MAX << std::endl;
    std::cout << "  UCHAR_MAX: " << UCHAR_MAX << std::endl;
    
    // C++11 引入的字符类型
    #if __cplusplus >= 201103L
    char16_t utf16_char = u'A';
    char32_t utf32_char = U'A';
    std::cout << "  char16_t: " << utf16_char << std::endl;
    std::cout << "  char32_t: " << utf32_char << std::endl;
    #endif
    
    // C++20 引入的字符类型
    #if __cplusplus >= 202002L
    char8_t utf8_char = u8'A';
    std::cout << "  char8_t: " << static_cast<int>(utf8_char) << std::endl;
    #endif
    
    // 宽字符类型
    wchar_t wide_char = L'A';
    std::cout << "  wchar_t: " << wide_char << std::endl;
    
    // 类型特征
    std::cout << "  is_arithmetic<char>: " << std::is_arithmetic_v<char> << std::endl;
    std::cout << "  is_integral<char>: " << std::is_integral_v<char> << std::endl;
}

// 字符类型用于字符串处理
void char_string_processing() {
    // 字符数组（字符串）
    char str1[] = "Hello";
    char str2[20] = "World";
    
    // 字符类型用于原始内存访问
    int number = 0x12345678;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&number);
    
    std::cout << "Character string processing:" << std::endl;
    std::cout << "  String 1: " << str1 << std::endl;
    std::cout << "  String 2: " << str2 << std::endl;
    
    std::cout << "  Bytes of integer 0x" << std::hex << number << std::dec << ": ";
    for (size_t i = 0; i < sizeof(int); i++) {
        printf("0x%02X ", bytes[i]);
    }
    std::cout << std::endl;
}
```

**整数类型**：

```cpp
#include <iostream>
#include <type_traits>
#include <climits>

void integer_type_examples() {
    std::cout << "Integer type examples:" << std::endl;
    
    // 标准有符号整数类型
    signed char sc = SCHAR_MAX;
    short s = SHRT_MAX;
    int i = INT_MAX;
    long l = LONG_MAX;
    long long ll = LLONG_MAX;
    
    // 标准无符号整数类型
    unsigned char uc = UCHAR_MAX;
    unsigned short us = USHRT_MAX;
    unsigned int ui = UINT_MAX;
    unsigned long ul = ULONG_MAX;
    unsigned long long ull = ULLONG_MAX;
    
    std::cout << "  signed char: " << static_cast<int>(sc) << std::endl;
    std::cout << "  short: " << s << std::endl;
    std::cout << "  int: " << i << std::endl;
    std::cout << "  long: " << l << std::endl;
    std::cout << "  long long: " << ll << std::endl;
    
    std::cout << "  unsigned char: " << static_cast<unsigned>(uc) << std::endl;
    std::cout << "  unsigned short: " << us << std::endl;
    std::cout << "  unsigned int: " << ui << std::endl;
    std::cout << "  unsigned long: " << ul << std::endl;
    std::cout << "  unsigned long long: " << ull << std::endl;
    
    // 整数类型范围
    std::cout << "  CHAR_BIT: " << CHAR_BIT << std::endl;
    std::cout << "  sizeof(char): " << sizeof(char) << " bytes" << std::endl;
    std::cout << "  sizeof(short): " << sizeof(short) << " bytes" << std::endl;
    std::cout << "  sizeof(int): " << sizeof(int) << " bytes" << std::endl;
    std::cout << "  sizeof(long): " << sizeof(long) << " bytes" << std::endl;
    std::cout << "  sizeof(long long): " << sizeof(long long) << " bytes" << std::endl;
    
    // 类型特征
    std::cout << "  is_arithmetic<int>: " << std::is_arithmetic_v<int> << std::endl;
    std::cout << "  is_integral<int>: " << std::is_integral_v<int> << std::endl;
    std::cout << "  is_signed<int>: " << std::is_signed_v<int> << std::endl;
    std::cout << "  is_unsigned<unsigned int>: " << std::is_unsigned_v<unsigned int> << std::endl;
}

// 整数溢出示例
void integer_overflow_examples() {
    std::cout << "Integer overflow examples:" << std::endl;
    
    // 有符号整数溢出 - 未定义行为
    int max_int = INT_MAX;
    std::cout << "  INT_MAX: " << max_int << std::endl;
    // int overflow = max_int + 1;  // 未定义行为
    
    // 无符号整数溢出 - 模运算
    unsigned int max_uint = UINT_MAX;
    std::cout << "  UINT_MAX: " << max_uint << std::endl;
    unsigned int overflow = max_uint + 1;  // 模运算，结果为 0
    std::cout << "  UINT_MAX + 1 = " << overflow << std::endl;
    
    // 无符号整数回绕
    unsigned char uc = 0;
    uc--;  // 回绕到 255
    std::cout << "  0 - 1 (unsigned char) = " << static_cast<int>(uc) << std::endl;
}
```

**浮点类型**：

```cpp
#include <iostream>
#include <type_traits>
#include <limits>
#include <cmath>

void floating_point_examples() {
    std::cout << "Floating-point type examples:" << std::endl;
    
    // 标准浮点类型
    float f = 3.141592653589793238f;
    double d = 3.141592653589793238;
    long double ld = 3.141592653589793238L;
    
    std::cout << "  float: " << f << std::endl;
    std::cout << "  double: " << d << std::endl;
    std::cout << "  long double: " << ld << std::endl;
    
    // 浮点数特殊值
    float infinity = std::numeric_limits<float>::infinity();
    float nan_value = std::numeric_limits<float>::quiet_NaN();
    float negative_zero = -0.0f;
    
    std::cout << "  Infinity: " << infinity << std::endl;
    std::cout << "  NaN: " << nan_value << std::endl;
    std::cout << "  Negative zero: " << negative_zero << std::endl;
    
    // 浮点数比较
    if (std::isnan(nan_value)) {
        std::cout << "  NaN value detected" << std::endl;
    }
    
    if (std::isinf(infinity)) {
        std::cout << "  Infinity value detected" << std::endl;
    }
    
    // 浮点数精度示例
    float sum = 0.0f;
    for (int i = 0; i < 1000000; i++) {
        sum += 0.1f;
    }
    std::cout << "  Sum of 0.1f added 1,000,000 times: " << sum << std::endl;
    std::cout << "  Expected: " << 100000.0f << std::endl;
    
    // 类型特征
    std::cout << "  is_arithmetic<float>: " << std::is_arithmetic_v<float> << std::endl;
    std::cout << "  is_floating_point<float>: " << std::is_floating_point_v<float> << std::endl;
    std::cout << "  is_floating_point<int>: " << std::is_floating_point_v<int> << std::endl;
}

// 浮点数范围和精度
void floating_point_limits() {
    std::cout << "Floating-point limits:" << std::endl;
    
    std::cout << "  Float limits:" << std::endl;
    std::cout << "    min: " << std::numeric_limits<float>::min() << std::endl;
    std::cout << "    max: " << std::numeric_limits<float>::max() << std::endl;
    std::cout << "    epsilon: " << std::numeric_limits<float>::epsilon() << std::endl;
    std::cout << "    digits: " << std::numeric_limits<float>::digits10 << " decimal digits" << std::endl;
    
    std::cout << "  Double limits:" << std::endl;
    std::cout << "    min: " << std::numeric_limits<double>::min() << std::endl;
    std::cout << "    max: " << std::numeric_limits<double>::max() << std::endl;
    std::cout << "    epsilon: " << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << "    digits: " << std::numeric_limits<double>::digits10 << " decimal digits" << std::endl;
    
    std::cout << "  Long double limits:" << std::endl;
    std::cout << "    min: " << std::numeric_limits<long double>::min() << std::endl;
    std::cout << "    max: " << std::numeric_limits<long double>::max() << std::endl;
    std::cout << "    epsilon: " << std::numeric_limits<long double>::epsilon() << std::endl;
    std::cout << "    digits: " << std::numeric_limits<long double>::digits10 << " decimal digits" << std::endl;
}
```

## 4. 复合类型

### 4.1 引用类型

```cpp
#include <iostream>
#include <type_traits>

void reference_type_examples() {
    std::cout << "Reference type examples:" << std::endl;
    
    // 左值引用类型
    int value = 42;
    int& lvalue_ref = value;  // 对象类型的左值引用
    
    std::cout << "  Original value: " << value << std::endl;
    std::cout << "  Reference value: " << lvalue_ref << std::endl;
    
    lvalue_ref = 100;  // 通过引用修改原值
    std::cout << "  After modification through reference: " << value << std::endl;
    
    // 函数类型的引用（较少使用）
    void function_example() {
        std::cout << "    Function called through reference" << std::endl;
    }
    
    void (&function_ref)() = function_example;  // 函数类型的左值引用
    function_ref();
    
    // 右值引用类型 (C++11)
    #if __cplusplus >= 201103L
    int&& rvalue_ref = 42;  // 对象类型的右值引用
    std::cout << "  Rvalue reference: " << rvalue_ref << std::endl;
    
    // 移动语义示例
    std::string source = "Hello";
    std::string target = std::move(source);  // 移动构造
    std::cout << "  Source after move: '" << source << "'" << std::endl;
    std::cout << "  Target after move: '" << target << "'" << std::endl;
    #endif
    
    // 类型特征
    std::cout << "  is_reference<int&>: " << std::is_reference_v<int&> << std::endl;
    std::cout << "  is_lvalue_reference<int&>: " << std::is_lvalue_reference_v<int&> << std::endl;
    #if __cplusplus >= 201103L
    std::cout << "  is_rvalue_reference<int&&>: " << std::is_rvalue_reference_v<int&&> << std::endl;
    #endif
}

// 引用作为函数参数
void reference_as_parameters() {
    std::cout << "Reference as parameters:" << std::endl;
    
    // 通过引用传递避免拷贝
    void modify_value(int& ref) {
        ref = 100;
    }
    
    int value = 42;
    std::cout << "  Before modification: " << value << std::endl;
    modify_value(value);
    std::cout << "  After modification: " << value << std::endl;
    
    // 常量引用避免拷贝同时防止修改
    void read_value(const int& cref) {
        std::cout << "    Reading value: " << cref << std::endl;
        // cref = 200;  // 错误：不能修改常量引用
    }
    
    int const_value = 200;
    read_value(const_value);
    read_value(300);  // 临时对象可以绑定到常量引用
}
```

### 4.2 指针类型

```cpp
#include <iostream>
#include <type_traits>
#include <memory>

void pointer_type_examples() {
    std::cout << "Pointer type examples:" << std::endl;
    
    // 指向对象的指针
    int value = 42;
    int* object_ptr = &value;
    
    std::cout << "  Object pointer: " << object_ptr << " -> " << *object_ptr << std::endl;
    
    // 指向函数的指针
    int add(int a, int b) {
        return a + b;
    }
    
    int (*function_ptr)(int, int) = add;
    int result = function_ptr(10, 20);
    std::cout << "  Function pointer result: " << result << std::endl;
    
    // 指向成员的指针
    struct Example {
        int member;
        void method() {
            std::cout << "    Member function called" << std::endl;
        }
    };
    
    int Example::* member_ptr = &Example::member;
    void (Example::* method_ptr)() = &Example::method;
    
    Example obj;
    obj.*member_ptr = 100;
    std::cout << "  Member pointer value: " << obj.member << std::endl;
    
    (obj.*method_ptr)();
    
    // 空指针
    int* null_ptr = nullptr;
    if (null_ptr == nullptr) {
        std::cout << "  Null pointer detected" << std::endl;
    }
    
    // 类型特征
    std::cout << "  is_pointer<int*>: " << std::is_pointer_v<int*> << std::endl;
    std::cout << "  is_member_pointer<decltype(member_ptr)>: " << std::is_member_pointer_v<decltype(member_ptr)> << std::endl;
    std::cout << "  is_member_object_pointer<decltype(member_ptr)>: " << std::is_member_object_pointer_v<decltype(member_ptr)> << std::endl;
    std::cout << "  is_member_function_pointer<decltype(method_ptr)>: " << std::is_member_function_pointer_v<decltype(method_ptr)> << std::endl;
}

// 智能指针 (C++11)
void smart_pointer_examples() {
    #if __cplusplus >= 201103L
    std::cout << "Smart pointer examples:" << std::endl;
    
    // unique_ptr - 独占所有权
    auto unique_ptr = std::make_unique<int>(42);
    std::cout << "  unique_ptr value: " << *unique_ptr << std::endl;
    
    // shared_ptr - 共享所有权
    auto shared_ptr1 = std::make_shared<int>(100);
    {
        auto shared_ptr2 = shared_ptr1;  // 共享所有权
        std::cout << "  shared_ptr value: " << *shared_ptr1 << std::endl;
        std::cout << "  Reference count: " << shared_ptr1.use_count() << std::endl;
    }  // shared_ptr2 销毁
    std::cout << "  Reference count after scope: " << shared_ptr1.use_count() << std::endl;
    
    // weak_ptr - 弱引用
    std::weak_ptr<int> weak_ptr = shared_ptr1;
    if (auto locked_ptr = weak_ptr.lock()) {
        std::cout << "  weak_ptr value: " << *locked_ptr << std::endl;
    }
    #endif
}
```

### 4.3 数组类型

```cpp
#include <iostream>
#include <type_traits>

void array_type_examples() {
    std::cout << "Array type examples:" << std::endl;
    
    // 一维数组
    int arr1[5] = {1, 2, 3, 4, 5};
    
    // 多维数组
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    
    // 字符数组（字符串）
    char str1[] = "Hello";
    char str2[20] = "World";
    
    // 可变长度数组 (C++ 不支持，但可以使用动态分配)
    size_t size = 5;
    int* vla = new int[size];  // 模拟 VLA
    for (size_t i = 0; i < size; ++i) {
        vla[i] = static_cast<int>(i * 10);
    }
    
    std::cout << "  Array elements: ";
    for (int i = 0; i < 5; i++) {
        std::cout << arr1[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "  Matrix element [1][2]: " << matrix[1][2] << std::endl;
    std::cout << "  String 1: " << str1 << std::endl;
    std::cout << "  String 2: " << str2 << std::endl;
    
    std::cout << "  VLA elements: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << vla[i] << " ";
    }
    std::cout << std::endl;
    
    delete[] vla;
    
    // 数组退化为指针
    int* ptr = arr1;  // 数组名退化为指向首元素的指针
    std::cout << "  Array decayed to pointer: " << ptr << " -> " << *ptr << std::endl;
    
    // 类型特征
    std::cout << "  is_array<int[5]>: " << std::is_array_v<int[5]> << std::endl;
    std::cout << "  is_array<int*>: " << std::is_array_v<int*> << std::endl;
    std::cout << "  extent<int[5]>: " << std::extent_v<int[5]> << std::endl;
    std::cout << "  extent<int[3][4], 1>: " << std::extent_v<int[3][4], 1> << std::endl;
}

// 数组作为函数参数
void array_as_parameters() {
    std::cout << "Array as parameters:" << std::endl;
    
    // 三种等价的数组参数声明方式
    void process_array1(int* arr, size_t size) {
        std::cout << "    Processing array via pointer: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
    
    void process_array2(int arr[], size_t size) {
        std::cout << "    Processing array via array syntax: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
    
    void process_array3(int arr[5], size_t size) {
        std::cout << "    Processing array via sized array syntax: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
    
    int test_array[] = {10, 20, 30, 40, 50};
    size_t array_size = sizeof(test_array) / sizeof(test_array[0]);
    
    process_array1(test_array, array_size);
    process_array2(test_array, array_size);
    process_array3(test_array, array_size);
}
```

### 4.4 函数类型

```cpp
#include <iostream>
#include <type_traits>
#include <functional>

void function_type_examples() {
    std::cout << "Function type examples:" << std::endl;
    
    // 基本函数类型
    int add(int a, int b) {
        return a + b;
    }
    
    double divide(double a, double b) {
        return a / b;
    }
    
    void print_message(const std::string& msg) {
        std::cout << "    " << msg << std::endl;
    }
    
    // 函数指针
    int (*add_ptr)(int, int) = add;
    double (*divide_ptr)(double, double) = divide;
    void (*print_ptr)(const std::string&) = print_message;
    
    std::cout << "  Add result: " << add_ptr(10, 20) << std::endl;
    std::cout << "  Divide result: " << divide_ptr(10.0, 3.0) << std::endl;
    print_ptr("Hello from function pointer");
    
    // 函数引用（较少使用）
    int (&add_ref)(int, int) = add;
    std::cout << "  Add result via reference: " << add_ref(15, 25) << std::endl;
    
    // Lambda 表达式 (C++11)
    #if __cplusplus >= 201103L
    auto multiply = [](int a, int b) { return a * b; };
    auto greet = [](const std::string& name) { 
        std::cout << "    Hello, " << name << "!" << std::endl; 
    };
    
    std::cout << "  Multiply result: " << multiply(6, 7) << std::endl;
    greet("World");
    
    // std::function (C++11)
    std::function<int(int, int)> func_add = add;
    std::function<int(int, int)> func_lambda = multiply;
    
    std::cout << "  std::function add result: " << func_add(30, 40) << std::endl;
    std::cout << "  std::function multiply result: " << func_lambda(8, 9) << std::endl;
    #endif
    
    // 类型特征
    std::cout << "  is_function<decltype(add)>: " << std::is_function_v<decltype(add)> << std::endl;
    std::cout << "  is_function<int(*)(int, int)>: " << std::is_function_v<int(*)(int, int)> << std::endl;
    std::cout << "  is_function<int(int, int)>: " << std::is_function_v<int(int, int)> << std::endl;
}

// 高阶函数示例
void higher_order_functions() {
    std::cout << "Higher-order functions:" << std::endl;
    
    // 接受函数作为参数的函数
    void apply_operation(int a, int b, int (*operation)(int, int)) {
        int result = operation(a, b);
        std::cout << "    Operation result: " << result << std::endl;
    }
    
    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }
    
    apply_operation(10, 20, add);
    apply_operation(10, 20, multiply);
    
    // 返回函数的函数
    auto get_operation(bool is_add) -> int(*)(int, int) {
        if (is_add) {
            return [](int a, int b) { return a + b; };
        } else {
            return [](int a, int b) { return a * b; };
        }
    }
    
    auto add_func = get_operation(true);
    auto mult_func = get_operation(false);
    
    std::cout << "    Add function result: " << add_func(5, 6) << std::endl;
    std::cout << "    Multiply function result: " << mult_func(5, 6) << std::endl;
}
```

### 4.5 枚举类型

```cpp
#include <iostream>
#include <type_traits>

void enum_type_examples() {
    std::cout << "Enum type examples:" << std::endl;
    
    // 无作用域枚举类型
    enum Color {
        RED,
        GREEN,
        BLUE
    };
    
    enum Status {
        OK = 200,
        NOT_FOUND = 404,
        SERVER_ERROR = 500
    };
    
    Color my_color = GREEN;
    Status http_status = NOT_FOUND;
    
    std::cout << "  Color value: " << my_color << std::endl;
    std::cout << "  HTTP Status: " << http_status << std::endl;
    
    // 有作用域枚举类型 (C++11)
    #if __cplusplus >= 201103L
    enum class Direction {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };
    
    enum class HttpStatus : unsigned short {
        OK = 200,
        NOT_FOUND = 404,
        SERVER_ERROR = 500
    };
    
    Direction dir = Direction::NORTH;
    HttpStatus status = HttpStatus::OK;
    
    // 有作用域枚举不能隐式转换为整数
    // int dir_value = dir;  // 错误
    int dir_value = static_cast<int>(dir);  // 需要显式转换
    unsigned short status_value = static_cast<unsigned short>(status);
    
    std::cout << "  Direction value: " << dir_value << std::endl;
    std::cout << "  HttpStatus value: " << status_value << std::endl;
    #endif
    
    // 枚举与整数的转换
    enum Priority {
        LOW = 1,
        MEDIUM = 2,
        HIGH = 3
    };
    
    Priority priority = static_cast<Priority>(2);  // 可以直接赋整数值
    std::cout << "  Priority from int: " << priority << std::endl;
    
    // 类型特征
    std::cout << "  is_enum<Color>: " << std::is_enum_v<Color> << std::endl;
    #if __cplusplus >= 201103L
    std::cout << "  is_enum<Direction>: " << std::is_enum_v<Direction> << std::endl;
    std::cout << "  is_scoped_enum<Direction>: " << std::is_scoped_enum_v<Direction> << std::endl;
    #endif
}

// 枚举的实际应用
void enum_practical_usage() {
    std::cout << "Enum practical usage:" << std::endl;
    
    // 状态机示例
    enum class State {
        IDLE,
        RUNNING,
        PAUSED,
        STOPPED
    };
    
    class StateMachine {
    private:
        State current_state = State::IDLE;
        
    public:
        void start() {
            if (current_state == State::IDLE) {
                current_state = State::RUNNING;
                std::cout << "    State changed to RUNNING" << std::endl;
            }
        }
        
        void pause() {
            if (current_state == State::RUNNING) {
                current_state = State::PAUSED;
                std::cout << "    State changed to PAUSED" << std::endl;
            }
        }
        
        void stop() {
            current_state = State::STOPPED;
            std::cout << "    State changed to STOPPED" << std::endl;
        }
        
        State get_state() const {
            return current_state;
        }
    };
    
    StateMachine machine;
    machine.start();
    machine.pause();
    machine.stop();
    
    std::cout << "    Final state: " << static_cast<int>(machine.get_state()) << std::endl;
}
```

### 4.6 类类型

```cpp
#include <iostream>
#include <type_traits>

void class_type_examples() {
    std::cout << "Class type examples:" << std::endl;
    
    // 非联合类类型
    class SimpleClass {
    private:
        int data;
        
    public:
        SimpleClass(int value = 0) : data(value) {}
        int get_data() const { return data; }
        void set_data(int value) { data = value; }
    };
    
    // 联合类型
    union DataUnion {
        int integer;
        float floating;
        char bytes[4];
        
        DataUnion() : integer(0) {}  // 需要显式构造函数
    };
    
    // 创建对象
    SimpleClass obj1(42);
    SimpleClass obj2;  // 使用默认构造函数
    
    DataUnion union_obj;
    union_obj.integer = 100;
    std::cout << "  Union integer: " << union_obj.integer << std::endl;
    
    union_obj.floating = 3.14f;
    std::cout << "  Union float: " << union_obj.floating << std::endl;
    
    // 访问类成员
    std::cout << "  obj1 data: " << obj1.get_data() << std::endl;
    obj2.set_data(200);
    std::cout << "  obj2 data: " << obj2.get_data() << std::endl;
    
    // 类型特征
    std::cout << "  is_class<SimpleClass>: " << std::is_class_v<SimpleClass> << std::endl;
    std::cout << "  is_union<DataUnion>: " << std::is_union_v<DataUnion> << std::endl;
    std::cout << "  is_class<int>: " << std::is_class_v<int> << std::endl;
}

// 继承示例
void inheritance_examples() {
    std::cout << "Inheritance examples:" << std::endl;
    
    class Base {
    protected:
        int base_data;
        
    public:
        Base(int value = 0) : base_data(value) {}
        virtual ~Base() = default;
        virtual void virtual_function() {
            std::cout << "    Base::virtual_function()" << std::endl;
        }
    };
    
    class Derived : public Base {
    private:
        int derived_data;
        
    public:
        Derived(int base_val, int derived_val) 
            : Base(base_val), derived_data(derived_val) {}
            
        void virtual_function() override {
            std::cout << "    Derived::virtual_function()" << std::endl;
        }
        
        void derived_function() {
            std::cout << "    Derived::derived_function()" << std::endl;
        }
    };
    
    Derived obj(100, 200);
    Base& base_ref = obj;
    
    base_ref.virtual_function();  // 多态调用
    obj.derived_function();       // 直接调用
    
    std::cout << "  Base data accessible: " << std::is_base_of_v<Base, Derived> << std::endl;
}
```

## 5. 类型特征层次结构

### 5.1 类型特征概念

C++ 提供了丰富的类型特征来在编译时查询类型的属性：

```cpp
#include <iostream>
#include <type_traits>

void type_traits_hierarchy() {
    std::cout << "Type traits hierarchy:" << std::endl;
    
    // 基本类型特征
    std::cout << "  Fundamental types:" << std::endl;
    std::cout << "    is_fundamental<int>: " << std::is_fundamental_v<int> << std::endl;
    std::cout << "    is_fundamental<void>: " << std::is_fundamental_v<void> << std::endl;
    std::cout << "    is_fundamental<int*>: " << std::is_fundamental_v<int*> << std::endl;
    
    // 算术类型特征
    std::cout << "  Arithmetic types:" << std::endl;
    std::cout << "    is_arithmetic<int>: " << std::is_arithmetic_v<int> << std::endl;
    std::cout << "    is_arithmetic<double>: " << std::is_arithmetic_v<double> << std::endl;
    std::cout << "    is_arithmetic<bool>: " << std::is_arithmetic_v<bool> << std::endl;
    
    // 整数类型特征
    std::cout << "  Integer types:" << std::endl;
    std::cout << "    is_integral<int>: " << std::is_integral_v<int> << std::endl;
    std::cout << "    is_integral<char>: " << std::is_integral_v<char> << std::endl;
    std::cout << "    is_integral<bool>: " << std::is_integral_v<bool> << std::endl;
    
    // 浮点类型特征
    std::cout << "  Floating-point types:" << std::endl;
    std::cout << "    is_floating_point<float>: " << std::is_floating_point_v<float> << std::endl;
    std::cout << "    is_floating_point<double>: " << std::is_floating_point_v<double> << std::endl;
    std::cout << "    is_floating_point<int>: " << std::is_floating_point_v<int> << std::endl;
    
    // 复合类型特征
    std::cout << "  Compound types:" << std::endl;
    std::cout << "    is_compound<int*>: " << std::is_compound_v<int*> << std::endl;
    std::cout << "    is_compound<int&>: " << std::is_compound_v<int&> << std::endl;
    std::cout << "    is_compound<int[5]>: " << std::is_compound_v<int[5]> << std::endl;
}

// 标量类型特征
void scalar_type_traits() {
    std::cout << "Scalar type traits:" << std::endl;
    
    // 标量类型包括算术类型、枚举类型、指针类型、成员指针类型和 nullptr_t
    std::cout << "  is_scalar<int>: " << std::is_scalar_v<int> << std::endl;
    std::cout << "  is_scalar<double>: " << std::is_scalar_v<double> << std::endl;
    std::cout << "  is_scalar<enum>: " << []{ 
        enum Color { RED, GREEN, BLUE }; 
        return std::is_scalar_v<Color>; 
    }() << std::endl;
    std::cout << "  is_scalar<int*>: " << std::is_scalar_v<int*> << std::endl;
    std::cout << "  is_scalar<int Color::*>: " << []{ 
        struct Color { int value; }; 
        return std::is_scalar_v<int Color::*>; 
    }() << std::endl;
    
    #if __cplusplus >= 201103L
    std::cout << "  is_scalar<std::nullptr_t>: " << std::is_scalar_v<std::nullptr_t> << std::endl;
    #endif
}

// 对象类型特征
void object_type_traits() {
    std::cout << "Object type traits:" << std::endl;
    
    // 对象类型是除函数类型、引用类型和 void 之外的类型
    std::cout << "  is_object<int>: " << std::is_object_v<int> << std::endl;
    std::cout << "  is_object<int[10]>: " << std::is_object_v<int[10]> << std::endl;
    std::cout << "  is_object<struct>: " << []{ 
        struct Point { int x, y; }; 
        return std::is_object_v<Point>; 
    }() << std::endl;
    std::cout << "  is_object<void>: " << std::is_object_v<void> << std::endl;
    std::cout << "  is_object<int&>: " << std::is_object_v<int&> << std::endl;
    std::cout << "  is_object<void(int)>: " << std::is_object_v<void(int)> << std::endl;
}
```

### 5.2 特殊类型类别

```cpp
#include <iostream>
#include <type_traits>

void special_type_categories() {
    std::cout << "Special type categories:" << std::endl;
    
    // 隐式生命周期类型 (C++17)
    std::cout << "  Implicit lifetime types:" << std::endl;
    std::cout << "    is_implicit_lifetime<int>: " << std::is_implicit_lifetime_v<int> << std::endl;
    std::cout << "    is_implicit_lifetime<int[10]>: " << std::is_implicit_lifetime_v<int[10]> << std::endl;
    
    // 平凡可拷贝类型
    std::cout << "  Trivially copyable types:" << std::endl;
    std::cout << "    is_trivially_copyable<int>: " << std::is_trivially_copyable_v<int> << std::endl;
    std::cout << "    is_trivially_copyable<double>: " << std::is_trivially_copyable_v<double> << std::endl;
    
    struct TrivialStruct {
        int a;
        double b;
    };
    std::cout << "    is_trivially_copyable<TrivialStruct>: " << std::is_trivially_copyable_v<TrivialStruct> << std::endl;
    
    struct NonTrivialStruct {
        int a;
        NonTrivialStruct() : a(0) {}  // 用户定义构造函数
    };
    std::cout << "    is_trivially_copyable<NonTrivialStruct>: " << std::is_trivially_copyable_v<NonTrivialStruct> << std::endl;
    
    // 标准布局类型 (C++11)
    std::cout << "  Standard layout types:" << std::endl;
    std::cout << "    is_standard_layout<int>: " << std::is_standard_layout_v<int> << std::endl;
    
    struct StandardLayout {
        int a;
        double b;
    };
    std::cout << "    is_standard_layout<StandardLayout>: " << std::is_standard_layout_v<StandardLayout> << std::endl;
    
    struct NonStandardLayout : StandardLayout {
        int c;
    };
    std::cout << "    is_standard_layout<NonStandardLayout>: " << std::is_standard_layout_v<NonStandardLayout> << std::endl;
}

// 已弃用的类型类别
void deprecated_type_categories() {
    std::cout << "Deprecated type categories:" << std::endl;
    
    // POD 类型 (C++20 中已弃用)
    std::cout << "  POD types (deprecated in C++20):" << std::endl;
    std::cout << "    is_pod<int>: " << std::is_pod_v<int> << std::endl;
    
    struct PODStruct {
        int a;
        double b;
    };
    std::cout << "    is_pod<PODStruct>: " << std::is_pod_v<PODStruct> << std::endl;
    
    // Trivial types (C++26 中将弃用)
    std::cout << "  Trivial types (to be deprecated in C++26):" << std::endl;
    std::cout << "    is_trivial<int>: " << std::is_trivial_v<int> << std::endl;
    std::cout << "    is_trivial<double>: " << std::is_trivial_v<double> << std::endl;
}
```

## 6. 类型命名

### 6.1 类型声明方式

类型可以通过多种方式声明和命名：

```cpp
#include <iostream>
#include <vector>
#include <functional>

void type_naming() {
    std::cout << "Type naming:" << std::endl;
    
    // 1. class 声明
    class MyClass {
    public:
        int data;
        MyClass(int value = 0) : data(value) {}
    };
    
    // 2. union 声明
    union MyUnion {
        int integer;
        float floating;
    };
    
    // 3. enum 声明
    enum MyEnum { VALUE1, VALUE2, VALUE3 };
    
    // 4. typedef 声明
    typedef int Integer;
    typedef std::vector<int> IntVector;
    
    // 5. 类型别名声明 (C++11)
    using Double = double;
    using StringVector = std::vector<std::string>;
    
    // 使用声明的类型
    MyClass obj(42);
    MyUnion union_obj;
    MyEnum enum_value = VALUE2;
    Integer int_value = 100;
    IntVector vec = {1, 2, 3, 4, 5};
    Double double_value = 3.14;
    
    std::cout << "  MyClass data: " << obj.data << std::endl;
    std::cout << "  Enum value: " << enum_value << std::endl;
    std::cout << "  Integer value: " << int_value << std::endl;
    std::cout << "  Vector size: " << vec.size() << std::endl;
    std::cout << "  Double value: " << double_value << std::endl;
}

// 类型标识符 (type-id)
void type_identifiers() {
    std::cout << "Type identifiers:" << std::endl;
    
    // 类型标识符的语法与变量声明相同，但省略标识符
    int* p;               // 声明指针变量
    // static_cast<int*>(p); // type-id 是 "int*"
    
    int a[3];   // 声明数组变量
    // new int[3]; // type-id 是 "int[3]"
    
    // 复杂类型标识符示例
    int (*(*x[2])())[3];      // 声明复杂变量
    // new (int (*(*[2])())[3]); // type-id 是 "int (*(*[2])())[3]"
    
    void f(int);                    // 声明函数
    // std::function<void(int)> func = f; // type template parameter 是 "void(int)"
    
    std::vector<int> v;       // 声明 vector 变量
    // sizeof(std::vector<int>); // type-id 是 "std::vector<int>"
    
    std::cout << "  Complex type identifiers demonstrated in comments" << std::endl;
}

// 抽象声明符
void abstract_declarators() {
    std::cout << "Abstract declarators:" << std::endl;
    
    // 声明语法中移除名称的部分被称为抽象声明符
    // int* p;        // 声明符是 "*p"，抽象声明符是 "*"
    // int a[10];     // 声明符是 "a[10]"，抽象声明符是 "[10]"
    // int (*f)(int); // 声明符是 "(*f)(int)"，抽象声明符是 "(*)(int)"
    
    // 在强制类型转换中使用抽象声明符
    int value = 42;
    void* void_ptr = &value;
    int* int_ptr = static_cast<int*>(void_ptr);  // 使用抽象声明符 "*"
    
    std::cout << "  Value through cast: " << *int_ptr << std::endl;
    
    // 在 sizeof 中使用类型标识符
    std::cout << "  sizeof(int*): " << sizeof(int*) << std::endl;
    std::cout << "  sizeof(int[10]): " << sizeof(int[10]) << std::endl;
}
```

### 6.2 类型标识符的使用场景

```cpp
#include <iostream>
#include <typeinfo>
#include <functional>

void type_id_usage_scenarios() {
    std::cout << "Type ID usage scenarios:" << std::endl;
    
    // 1. 在强制类型转换表达式中指定目标类型
    double double_value = 3.14;
    int int_value = static_cast<int>(double_value);
    std::cout << "  Cast double to int: " << int_value << std::endl;
    
    // 2. 作为 sizeof、alignof 的参数
    std::cout << "  sizeof(int): " << sizeof(int) << std::endl;
    #if __cplusplus >= 201103L
    std::cout << "  alignof(int): " << alignof(int) << std::endl;
    #endif
    
    // 3. 在类型别名声明的右侧
    using MyInt = int;
    MyInt my_int = 42;
    std::cout << "  MyInt value: " << my_int << std::endl;
    
    // 4. 作为函数声明的尾部返回类型 (C++11)
    #if __cplusplus >= 201103L
    auto get_value() -> int {
        return 100;
    }
    std::cout << "  Function with trailing return type: " << get_value() << std::endl;
    #endif
    
    // 5. 作为模板类型参数的默认参数
    template<typename T = int>
    void template_function(T value = T{}) {
        std::cout << "    Template function value: " << value << std::endl;
    }
    
    template_function<>();  // 使用默认类型 int
    template_function<double>(3.14);  // 显式指定类型 double
    
    // 6. 作为模板类型参数
    std::vector<int> int_vector(5, 42);
    std::cout << "  Vector size: " << int_vector.size() << std::endl;
}

// 详细类型说明符
void elaborated_type_specifiers() {
    std::cout << "Elaborated type specifiers:" << std::endl;
    
    // 复杂类型说明符可用于引用先前声明的类名或枚举名
    struct Point {
        int x, y;
        Point(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
    };
    
    // 使用详细类型说明符
    struct Point p1(10, 20);  // 详细类型说明符 "struct Point"
    Point p2(30, 40);         // 简单类型说明符 "Point"
    
    std::cout << "  Point 1: (" << p1.x << ", " << p1.y << ")" << std::endl;
    std::cout << "  Point 2: (" << p2.x << ", " << p2.y << ")" << std::endl;
    
    // 枚举的详细类型说明符
    enum Color { RED, GREEN, BLUE };
    enum Color color = GREEN;
    std::cout << "  Color value: " << color << std::endl;
}
```

## 7. 静态类型与动态类型

### 7.1 静态类型

**静态类型**是由程序编译时分析产生的表达式的类型，在程序执行时不会改变：

```cpp
#include <iostream>
#include <typeinfo>

void static_type_examples() {
    std::cout << "Static type examples:" << std::endl;
    
    // 变量的静态类型在声明时确定
    int integer_var = 42;
    double double_var = 3.14;
    std::string string_var = "Hello";
    
    std::cout << "  integer_var static type: " << typeid(integer_var).name() << std::endl;
    std::cout << "  double_var static type: " << typeid(double_var).name() << std::endl;
    std::cout << "  string_var static type: " << typeid(string_var).name() << std::endl;
    
    // 表达式的静态类型由表达式构成确定
    auto sum = integer_var + 10;  // int + int -> int
    auto product = integer_var * double_var;  // int * double -> double
    
    std::cout << "  sum static type: " << typeid(sum).name() << std::endl;
    std::cout << "  product static type: " << typeid(product).name() << std::endl;
    
    // 函数返回值的静态类型
    int get_int() { return 42; }
    double get_double() { return 3.14; }
    
    auto int_result = get_int();      // 静态类型是 int
    auto double_result = get_double(); // 静态类型是 double
    
    std::cout << "  int_result static type: " << typeid(int_result).name() << std::endl;
    std::cout << "  double_result static type: " << typeid(double_result).name() << std::endl;
}

// 模板中的静态类型
void template_static_types() {
    std::cout << "Template static types:" << std::endl;
    
    template<typename T>
    void process_value(T value) {
        std::cout << "    Processing value of type: " << typeid(T).name() << std::endl;
        std::cout << "    Value: " << value << std::endl;
    }
    
    process_value(42);        // T 推导为 int
    process_value(3.14);      // T 推导为 double
    process_value("Hello");   // T 推导为 const char*
    
    // 显式指定模板参数
    process_value<int>(100);
    process_value<double>(2.71);
}
```

### 7.2 动态类型

**动态类型**是指如果某个左值表达式引用了一个多态对象，其最派生对象的类型：

```cpp
#include <iostream>
#include <typeinfo>

void dynamic_type_examples() {
    std::cout << "Dynamic type examples:" << std::endl;
    
    // 多态对象示例
    struct Base {
        virtual ~Base() = default;  // 虚析构函数使类成为多态类
        virtual void virtual_function() {
            std::cout << "    Base::virtual_function()" << std::endl;
        }
    };
    
    struct Derived : Base {
        void virtual_function() override {
            std::cout << "    Derived::virtual_function()" << std::endl;
        }
    };
    
    Derived derived_obj;
    Base& base_ref = derived_obj;
    Base* base_ptr = &derived_obj;
    
    // 静态类型 vs 动态类型
    std::cout << "  Static type of base_ref: " << typeid(decltype(base_ref)).name() << std::endl;
    std::cout << "  Dynamic type of base_ref: " << typeid(base_ref).name() << std::endl;
    
    std::cout << "  Static type of *base_ptr: " << typeid(decltype(*base_ptr)).name() << std::endl;
    std::cout << "  Dynamic type of *base_ptr: " << typeid(*base_ptr).name() << std::endl;
    
    // 虚函数调用使用动态类型
    base_ref.virtual_function();    // 调用 Derived::virtual_function()
    base_ptr->virtual_function();   // 调用 Derived::virtual_function()
    
    // typeid 操作符在运行时确定动态类型
    if (typeid(base_ref) == typeid(Derived)) {
        std::cout << "  base_ref dynamically is of type Derived" << std::endl;
    }
}

// dynamic_cast 和动态类型
void dynamic_cast_examples() {
    std::cout << "dynamic_cast examples:" << std::endl;
    
    struct Base {
        virtual ~Base() = default;
    };
    
    struct Derived1 : Base {
        int data1 = 100;
    };
    
    struct Derived2 : Base {
        double data2 = 3.14;
    };
    
    Base* ptr1 = new Derived1();
    Base* ptr2 = new Derived2();
    
    // 使用 dynamic_cast 进行安全的向下转换
    if (Derived1* d1 = dynamic_cast<Derived1*>(ptr1)) {
        std::cout << "  ptr1 points to Derived1 with data1: " << d1->data1 << std::endl;
    }
    
    if (Derived2* d2 = dynamic_cast<Derived2*>(ptr1)) {
        std::cout << "  ptr1 points to Derived2" << std::endl;
    } else {
        std::cout << "  ptr1 does not point to Derived2" << std::endl;
    }
    
    // 对于右值表达式，动态类型始终与静态类型相同
    int value = 42;
    int& ref = value;
    
    std::cout << "  Static type of ref: " << typeid(decltype(ref)).name() << std::endl;
    std::cout << "  Dynamic type of ref: " << typeid(ref).name() << std::endl;
    // 对于引用，如果引用的对象不是多态对象，动态类型与静态类型相同
    
    delete ptr1;
    delete ptr2;
}
```

## 8. 不完整类型

### 8.1 不完整类型概念

**不完整类型**是指缺乏足够信息来确定该类型对象大小的类型：

```cpp
#include <iostream>

void incomplete_types() {
    std::cout << "Incomplete types:" << std::endl;
    
    // 1. 类型 void（可能是 cv-限定的）
    // void void_obj;  // 错误：void 不是对象类型
    void* void_ptr = nullptr;  // void* 是完整类型
    
    // 2. 未完全定义的对象类型
    struct ForwardDeclared;  // 前向声明，不完整类型
    // ForwardDeclared obj;  // 错误：不完整类型
    
    struct ForwardDeclared {
        int data;
    };  // 现在是完整类型
    
    ForwardDeclared complete_obj;  // 现在可以声明对象
    std::cout << "  Forward declared struct size: " << sizeof(ForwardDeclared) << std::endl;
    
    // 3. 未知边界数组
    extern int unknown_array[];  // 未知边界数组，不完整类型
    // sizeof(unknown_array);  // 错误：不完整类型
    
    int known_array[10];  // 已知边界数组，完整类型
    std::cout << "  Known array size: " << sizeof(known_array) << std::endl;
    
    // 4. 元素类型不完整的数组
    struct IncompleteElement;  // 不完整类型
    // IncompleteElement array[5];  // 错误：元素类型不完整
    
    struct IncompleteElement {
        int data;
    };  // 现在是完整类型
    
    IncompleteElement complete_array[5];  // 现在可以声明数组
    std::cout << "  Complete array size: " << sizeof(complete_array) << std::endl;
}

// 枚举类型的不完整性
void enum_incomplete_types() {
    std::cout << "Enum incomplete types:" << std::endl;
    
    // 枚举类型从声明点直到其底层类型确定都是不完整的
    enum class IncompleteEnum;  // 前向声明，不完整类型
    // IncompleteEnum value;  // 错误：不完整类型
    
    enum class IncompleteEnum : int {  // 现在指定底层类型，变为完整类型
        VALUE1,
        VALUE2
    };
    
    IncompleteEnum complete_value = IncompleteEnum::VALUE1;  // 现在可以使用
    std::cout << "  Enum value: " << static_cast<int>(complete_value) << std::endl;
}
```

### 8.2 不完整类型的要求

在某些上下文中需要类型是完整的：

```cpp
#include <iostream>

void incomplete_type_requirements() {
    std::cout << "Incomplete type requirements:" << std::endl;
    
    struct Incomplete;
    
    // 以下操作需要完整类型：
    
    // 1. 具有返回类型 T 或参数类型 T 的函数的定义或调用
    // void func(Incomplete param);  // 错误：参数类型不完整
    
    // 2. 类型为 T 的对象的定义
    // Incomplete obj;  // 错误：不完整类型
    
    // 3. 类型为 T 的非静态类数据成员的声明
    struct Container {
        // Incomplete member;  // 错误：成员类型不完整
    };
    
    // 4. new 表达式用于创建类型为 T 的对象
    // Incomplete* ptr = new Incomplete;  // 错误：不完整类型
    
    // 5. 将类型为 T 的 glvalue 应用于左值到右值转换
    // Incomplete& ref;  // 错误：不完整类型
    
    // 6. 隐式或显式转换为类型 T
    // Incomplete obj = some_value;  // 错误：不完整类型
    
    // 7. 转换为类型 T* 或 T& 的标准转换
    // Incomplete* ptr = nullptr;  // 实际上这是允许的
    
    std::cout << "  Demonstrated in comments - operations requiring complete types" << std::endl;
}

// 不完整类型的完成
void completing_incomplete_types() {
    std::cout << "Completing incomplete types:" << std::endl;
    
    // 一个类类型在翻译单元的某个时刻可能被视为不完整，而在稍后被视为完整
    struct X;            // 声明 X，但未定义
    extern X* xp;        // xp 是指向不完整类型的指针
    
    // void foo() {
    //     xp++;            // 错误：X 是不完整的
    // }
    
    struct X { int i; }; // 定义 X，现在是完整类型
    X x;                 // OK：X 的定义是可达的
    
    // void bar() {
    //     xp = &x;         // OK：限定转换
    //     xp++;            // OK：X 是完整的
    // }
    
    std::cout << "  Class type completion demonstrated in comments" << std::endl;
    
    // 数组类型的完成
    extern int arr[];   // 未知边界数组，不完整类型
    // sizeof(arr);     // 错误：不完整类型
    
    int arr[10];        // 现在 arr 的类型是完整类型
    std::cout << "  Completed array size: " << sizeof(arr) << std::endl;
}

// 未知边界数组的特殊性
void unknown_bound_arrays() {
    std::cout << "Unknown bound arrays:" << std::endl;
    
    extern int unknown_arr[];   // 未知边界数组类型
    typedef int UNKA[];         // 通过 typedef 命名的未知边界数组
    
    // UNKA* arrp;         // arrp 是指向不完整类型的指针
    // UNKA** arrpp;
    
    // void foo() {
    //     arrp++;         // 错误：UNKA 是不完整类型
    //     arrpp++;        // OK：sizeof UNKA* 是已知的
    // }
    
    int known_arr[10];        // 现在 unknown_arr 的类型变为完整
    
    // void bar() {
    //     arrp = &known_arr;    // OK：限定转换
    //     arrp++;               // 错误：UNKA 无法完成
    // }
    
    std::cout << "  Unknown bound arrays demonstrated in comments" << std::endl;
}
```

## 9. 最佳实践

### 9.1 类型安全编程

```cpp
#include <iostream>
#include <type_traits>
#include <memory>

// 1. 使用类型特征进行编译时检查
template<typename T>
void safe_arithmetic_operation(T value) {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
    
    std::cout << "Safe arithmetic operation on value: " << value << std::endl;
    // 执行算术操作...
}

// 2. 使用强类型枚举避免隐式转换
enum class Status {
    OK = 200,
    NOT_FOUND = 404,
    SERVER_ERROR = 500
};

void handle_status(Status status) {
    switch (status) {
        case Status::OK:
            std::cout << "  Status OK" << std::endl;
            break;
        case Status::NOT_FOUND:
            std::cout << "  Status Not Found" << std::endl;
            break;
        case Status::SERVER_ERROR:
            std::cout << "  Status Server Error" << std::endl;
            break;
    }
}

void type_safety_best_practices() {
    std::cout << "Type safety best practices:" << std::endl;
    
    // 安全的算术操作
    safe_arithmetic_operation(42);
    safe_arithmetic_operation(3.14);
    
    // 使用强类型枚举
    handle_status(Status::OK);
    handle_status(Status::NOT_FOUND);
    
    // 避免危险的类型转换
    int int_value = 42;
    double double_value = static_cast<double>(int_value);  // 明确的类型转换
    std::cout << "  Safe cast: " << int_value << " -> " << double_value << std::endl;
    
    // 使用 nullptr 而不是 NULL 或 0
    #if __cplusplus >= 201103L
    int* ptr = nullptr;  // 类型安全的空指针
    if (ptr == nullptr) {
        std::cout << "  Safe null pointer check" << std::endl;
    }
    #endif
}

// 3. 使用智能指针管理资源
void smart_pointer_usage() {
    std::cout << "Smart pointer usage:" << std::endl;
    
    #if __cplusplus >= 201103L
    // unique_ptr - 独占所有权
    auto unique_resource = std::make_unique<int>(42);
    std::cout << "  unique_ptr value: " << *unique_resource << std::endl;
    
    // shared_ptr - 共享所有权
    auto shared_resource = std::make_shared<std::string>("Hello");
    {
        auto another_ref = shared_resource;  // 共享所有权
        std::cout << "  shared_ptr value: " << *shared_resource << std::endl;
        std::cout << "  Reference count: " << shared_resource.use_count() << std::endl;
    }
    std::cout << "  Reference count after scope: " << shared_resource.use_count() << std::endl;
    #endif
}
```

### 9.2 类型设计最佳实践

```cpp
#include <iostream>
#include <type_traits>

// 1. 使用类型别名提高可读性
using UserId = unsigned int;
using EmailAddress = std::string;
using Temperature = double;

class User {
private:
    UserId id_;
    EmailAddress email_;
    Temperature preferred_temp_;
    
public:
    User(UserId id, const EmailAddress& email, Temperature temp)
        : id_(id), email_(email), preferred_temp_(temp) {}
    
    UserId get_id() const { return id_; }
    const EmailAddress& get_email() const { return email_; }
    Temperature get_preferred_temp() const { return preferred_temp_; }
};

// 2. 使用 constexpr 和类型特征进行编译时优化
template<typename T>
constexpr bool is_fast_arithmetic_v = std::is_arithmetic_v<T> && sizeof(T) <= sizeof(long long);

template<typename T>
void optimized_operation(T value) {
    if constexpr (is_fast_arithmetic_v<T>) {
        std::cout << "  Fast arithmetic operation on: " << value << std::endl;
    } else {
        std::cout << "  Generic operation on: " << value << std::endl;
    }
}

void type_design_best_practices() {
    std::cout << "Type design best practices:" << std::endl;
    
    // 使用有意义的类型别名
    User user(12345, "user@example.com", 22.5);
    std::cout << "  User ID: " << user.get_id() << std::endl;
    std::cout << "  User Email: " << user.get_email() << std::endl;
    std::cout << "  User Preferred Temp: " << user.get_preferred_temp() << std::endl;
    
    // 编译时优化
    optimized_operation(42);
    optimized_operation(3.14);
    optimized_operation(std::string("Hello"));
}

// 3. 使用 SFINAE 和概念进行模板约束
#if __cplusplus >= 201103L
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_if_integral(T value) {
    std::cout << "  Integral value: " << value << std::endl;
}

template<typename T>
typename std::enable_if<!std::is_integral<T>::value, void>::type
print_if_integral(T value) {
    std::cout << "  Non-integral value: " << value << std::endl;
}
#endif

#if __cplusplus >= 202002L
template<std::integral T>
void process_integral(T value) {
    std::cout << "  Processing integral: " << value << std::endl;
}

template<std::floating_point T>
void process_floating(T value) {
    std::cout << "  Processing floating: " << value << std::endl;
}
#endif

void template_constraint_examples() {
    std::cout << "Template constraint examples:" << std::endl;
    
    #if __cplusplus >= 201103L
    print_if_integral(42);
    print_if_integral(3.14);
    #endif
    
    #if __cplusplus >= 202002L
    process_integral(100);
    process_floating(2.71);
    #endif
}
```

### 9.3 性能考虑的类型选择

```cpp
#include <iostream>
#include <chrono>
#include <vector>

// 1. 选择合适的整数类型
void integer_type_selection() {
    std::cout << "Integer type selection:" << std::endl;
    
    // 使用 int 作为默认选择（通常是最快的操作类型）
    int int_sum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        int_sum += i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto int_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 使用 long long（可能较慢）
    long long ll_sum = 0;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        ll_sum += i;
    }
    end = std::chrono::high_resolution_clock::now();
    auto ll_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  int operations time: " << int_duration.count() << " microseconds" << std::endl;
    std::cout << "  long long operations time: " << ll_duration.count() << " microseconds" << std::endl;
}

// 2. 浮点数类型选择
void floating_point_type_selection() {
    std::cout << "Floating-point type selection:" << std::endl;
    
    const int iterations = 1000000;
    
    // float 运算（通常较快）
    auto start = std::chrono::high_resolution_clock::now();
    volatile float f_sum = 0.0f;
    for (int i = 0; i < iterations; ++i) {
        f_sum += 0.1f;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto float_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // double 运算（精度更高，可能稍慢）
    start = std::chrono::high_resolution_clock::now();
    volatile double d_sum = 0.0;
    for (int i = 0; i < iterations; ++i) {
        d_sum += 0.1;
    }
    end = std::chrono::high_resolution_clock::now();
    auto double_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  float operations time: " << float_duration.count() << " microseconds" << std::endl;
    std::cout << "  double operations time: " << double_duration.count() << " microseconds" << std::endl;
}

// 3. 内存对齐考虑
#include <memory>

void alignment_considerations() {
    std::cout << "Alignment considerations:" << std::endl;
    
    // 使用 alignas 确保适当的对齐
    struct alignas(16) AlignedStruct {
        float x, y, z, w;  // 适合 SIMD 操作
    };
    
    AlignedStruct aligned_obj;
    std::cout << "  Aligned struct address: " << &aligned_obj << std::endl;
    std::cout << "  Is 16-byte aligned: " << 
        ((reinterpret_cast<uintptr_t>(&aligned_obj) % 16 == 0) ? "Yes" : "No") << std::endl;
    
    // 结构体对齐
    struct PackedStruct {
        char a;
        int b;
        char c;
    };
    
    std::cout << "  Packed struct size: " << sizeof(PackedStruct) << std::endl;
    std::cout << "  Packed struct alignment: " << alignof(PackedStruct) << std::endl;
    
    // 优化结构体布局以减少填充
    struct OptimizedStruct {
        int b;     // 4 bytes
        char a;    // 1 byte
        char c;    // 1 byte
        // 2 bytes padding
    };
    
    std::cout << "  Optimized struct size: " << sizeof(OptimizedStruct) << std::endl;
    std::cout << "  Optimized struct alignment: " << alignof(OptimizedStruct) << std::endl;
}

// 4. 容器类型选择
void container_type_selection() {
    std::cout << "Container type selection:" << std::endl;
    
    // 根据使用场景选择合适的容器
    const size_t size = 10000;
    
    // vector - 连续内存，随机访问快
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> vec(size);
    for (size_t i = 0; i < size; ++i) {
        vec[i] = static_cast<int>(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto vector_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // deque - 双端队列，两端插入删除快
    start = std::chrono::high_resolution_clock::now();
    std::deque<int> deq;
    for (size_t i = 0; i < size; ++i) {
        deq.push_back(static_cast<int>(i));
    }
    end = std::chrono::high_resolution_clock::now();
    auto deque_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  Vector initialization time: " << vector_duration.count() << " microseconds" << std::endl;
    std::cout << "  Deque initialization time: " << deque_duration.count() << " microseconds" << std::endl;
}
```

## 10. 标准参考

C++ 类型系统的相关标准条款：

- C++23 标准（ISO/IEC 14882:2024）：
  - 6.8.2 基本类型 [basic.fundamental]
  - 6.8.3 复合类型 [basic.compound]
  - 6.8.4 类型 [basic.types]
- C++20 标准（ISO/IEC 14882:2020）：
  - 6.8.2 基本类型 [basic.fundamental]
  - 6.8.3 复合类型 [basic.compound]
  - 6.8.4 类型 [basic.types]
- C++17 标准（ISO/IEC 14882:2017）：
  - 6.9.1 基本类型 [basic.fundamental]
  - 6.9.2 复合类型 [basic.compound]
  - 6.9.3 类型 [basic.types]
- C++14 标准（ISO/IEC 14882:2014）：
  - 3.9.1 基本类型 [basic.fundamental]
  - 3.9.2 复合类型 [basic.compound]
  - 3.9 类型 [basic.types]
- C++11 标准（ISO/IEC 14882:2011）：
  - 3.9.1 基本类型 [basic.fundamental]
  - 3.9.2 复合类型 [basic.compound]
  - 3.9 类型 [basic.types]
- C++98 标准（ISO/IEC 14882:1998）：
  - 3.9.1 基本类型 [basic.fundamental]
  - 3.9.2 复合类型 [basic.compound]
  - 3.9 类型 [basic.types]

相关的标准参考还包括：

- 5.2 表达式 [expr]
- 8 声明 [dcl.dcl]
- 9 类 [class]
- 14 模板 [temp]
- 20 类型支持 [support.types]
- 23 诊断库 [diagnostics]
- 28 类型特征 [meta]

## 11. 总结

C++ 类型系统是语言的核心组成部分，为程序提供了强大的类型安全和表达能力。深入理解 C++ 类型系统对于编写高质量的 C++ 程序至关重要。

### 11.1 核心要点回顾

**类型分类**：

- 基本类型：void、nullptr_t、算术类型（布尔、字符、整数、浮点）
- 复合类型：引用、指针、数组、函数、枚举、类（非联合、联合）

**重要概念**：

- 静态类型：编译时确定的类型
- 动态类型：运行时多态对象的实际类型
- 不完整类型：缺乏足够信息确定大小的类型
- 类型特征：编译时查询类型属性的工具

**现代特性**：

- 强类型枚举（C++11）：避免隐式转换
- 右值引用（C++11）：支持移动语义
- 智能指针（C++11）：自动内存管理
- 类型别名（C++11）：提高可读性
- 概念（C++20）：模板约束

### 11.2 实践建议

- 类型安全：使用强类型、类型特征和编译时检查
- 性能优化：选择合适的类型、注意对齐和内存布局
- 现代特性：充分利用 C++11 及以后版本的新特性
- 资源管理：使用 RAII 和智能指针自动管理资源
- 可读性：使用有意义的类型别名和清晰的类型设计

### 11.3 历史演进

C++ 类型系统在不同标准版本中的发展：

- C++98/03：建立了基本的类型系统框架
- C++11：引入了 nullptr_t、强类型枚举、右值引用、智能指针等重要特性
- C++14：完善了类型推导和泛型编程特性
- C++17：增加了结构化绑定、if constexpr 等特性
- C++20：引入了概念、范围等现代化特性
- C++23：继续扩展类型系统，增加新的类型特性

通过深入理解和正确应用 C++ 类型系统，开发者可以编写出更加安全、高效和可维护的 C++ 程序，充分发挥 C++ 在系统编程和高性能计算中的优势。
