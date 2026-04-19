# C++ 基本类型详解

  [1. 基本概念](#1-基本概念)
  
  [2. 类型分类](#2-类型分类)
  
  [3. void 类型](#3-void-类型)
  
  [4. std::nullptr_t 类型](#4-stdnullptr_t-类型)
  
  [5. 整数类型](#5-整数类型)
  
  [6. 浮点类型](#6-浮点类型)
  
  [7. 布尔类型](#7-布尔类型)
  
  [8. 字符类型](#8-字符类型)
  
  [9. 数据模型](#9-数据模型)
  
  [10. 类型特征和属性](#10-类型特征和属性)
  
  [11. 最佳实践](#11-最佳实践)
  
  [12. 标准参考](#12-标准参考)
  
  [13. 总结](#13-总结)

## 1. 基本概念

### 1.1 什么是基本类型

在 C++ 中，基本类型（fundamental types）是语言预定义的类型，它们构成了 C++ 类型系统的基础。基本类型不需要通过类、结构体、联合或枚举来定义，而是由编译器直接支持。

基本类型的主要特征包括：

- 内置支持：由编译器直接支持，无需用户定义
- 原子性：不能再分解为更简单的类型
- 平台相关性：大小和表示可能因平台而异
- 性能优化：通常具有最佳的性能特征

```cpp
#include <iostream>
#include <typeinfo>

void fundamental_type_basics() {
    std::cout << "Fundamental type basics:" << std::endl;
    
    // 基本类型的示例
    void void_example();                    // void 类型
    std::nullptr_t nullptr_example;         // nullptr_t 类型 (C++11)
    bool bool_example = true;               // 布尔类型
    char char_example = 'A';                // 字符类型
    int int_example = 42;                   // 整数类型
    float float_example = 3.14f;            // 浮点类型
    
    std::cout << "  void type: " << typeid(void).name() << std::endl;
    std::cout << "  nullptr_t type: " << typeid(std::nullptr_t).name() << std::endl;
    std::cout << "  bool type: " << typeid(bool).name() << " = " << bool_example << std::endl;
    std::cout << "  char type: " << typeid(char).name() << " = " << char_example << std::endl;
    std::cout << "  int type: " << typeid(int).name() << " = " << int_example << std::endl;
    std::cout << "  float type: " << typeid(float).name() << " = " << float_example << std::endl;
}
```

### 1.2 基本类型的重要性

基本类型在 C++ 中扮演着核心角色，具有以下重要性：

```cpp
#include <iostream>
#include <type_traits>

void fundamental_type_importance() {
    std::cout << "Fundamental type importance:" << std::endl;
    
    // 1. 内存分配基础
    int int_array[10];              // 基于 int 大小分配内存
    double double_array[10];        // 基于 double 大小分配内存
    
    std::cout << "  int array size: " << sizeof(int_array) << " bytes" << std::endl;
    std::cout << "  double array size: " << sizeof(double_array) << " bytes" << std::endl;
    
    // 2. 运算规则基础
    int a = 5, b = 2;
    double x = 5.0, y = 2.0;
    
    std::cout << "  Integer division: " << a << " / " << b << " = " << (a / b) << std::endl;
    std::cout << "  Double division: " << x << " / " << y << " = " << (x / y) << std::endl;
    
    // 3. 类型安全基础
    // int* ptr = 42;  // 编译错误：类型不匹配
    
    // 4. 模板推导基础
    auto value1 = 42;        // 推导为 int
    auto value2 = 3.14;      // 推导为 double
    auto value3 = true;      // 推导为 bool
    
    std::cout << "  Auto deduced types:" << std::endl;
    std::cout << "    value1: " << typeid(value1).name() << " = " << value1 << std::endl;
    std::cout << "    value2: " << typeid(value2).name() << " = " << value2 << std::endl;
    std::cout << "    value3: " << typeid(value3).name() << " = " << value3 << std::endl;
    
    // 5. 类型特征检查
    std::cout << "  Type traits:" << std::endl;
    std::cout << "    is_fundamental<int>: " << std::is_fundamental_v<int> << std::endl;
    std::cout << "    is_fundamental<double>: " << std::is_fundamental_v<double> << std::endl;
    std::cout << "    is_arithmetic<int>: " << std::is_arithmetic_v<int> << std::endl;
    std::cout << "    is_integral<int>: " << std::is_integral_v<int> << std::endl;
    std::cout << "    is_floating_point<double>: " << std::is_floating_point_v<double> << std::endl;
}
```

## 2. 类型分类

### 2.1 基本类型分类体系

C++ 基本类型可以按照不同的维度进行分类：

```cpp
#include <iostream>
#include <type_traits>

void fundamental_type_classification() {
    std::cout << "Fundamental type classification:" << std::endl;
    
    // 按语义分类
    std::cout << "=== Semantic Classification ===" << std::endl;
    
    // 1. void 类型
    std::cout << "  Void types:" << std::endl;
    std::cout << "    void: " << std::is_void_v<void> << std::endl;
    std::cout << "    const void: " << std::is_void_v<const void> << std::endl;
    
    // 2. nullptr_t 类型
    std::cout << "  Null pointer types:" << std::endl;
    std::cout << "    std::nullptr_t: " << std::is_null_pointer_v<std::nullptr_t> << std::endl;
    
    // 3. 算术类型
    std::cout << "  Arithmetic types:" << std::endl;
    std::cout << "    is_arithmetic<bool>: " << std::is_arithmetic_v<bool> << std::endl;
    std::cout << "    is_arithmetic<char>: " << std::is_arithmetic_v<char> << std::endl;
    std::cout << "    is_arithmetic<int>: " << std::is_arithmetic_v<int> << std::endl;
    std::cout << "    is_arithmetic<float>: " << std::is_arithmetic_v<float> << std::endl;
    
    // 算术类型的子分类
    std::cout << "=== Arithmetic Type Subclassification ===" << std::endl;
    
    // 整数类型
    std::cout << "  Integral types:" << std::endl;
    std::cout << "    is_integral<bool>: " << std::is_integral_v<bool> << std::endl;
    std::cout << "    is_integral<char>: " << std::is_integral_v<char> << std::endl;
    std::cout << "    is_integral<int>: " << std::is_integral_v<int> << std::endl;
    
    // 浮点类型
    std::cout << "  Floating-point types:" << std::endl;
    std::cout << "    is_floating_point<float>: " << std::is_floating_point_v<float> << std::endl;
    std::cout << "    is_floating_point<double>: " << std::is_floating_point_v<double> << std::endl;
    std::cout << "    is_floating_point<long double>: " << std::is_floating_point_v<long double> << std::endl;
}
```

### 2.2 CV 限定符

基本类型可以带有 CV 限定符（const 和 volatile）：

```cpp
#include <iostream>
#include <type_traits>

void cv_qualified_fundamental_types() {
    std::cout << "CV-qualified fundamental types:" << std::endl;
    
    // const 限定符
    const int const_int = 42;
    const double const_double = 3.14;
    const bool const_bool = true;
    
    std::cout << "  Const qualified types:" << std::endl;
    std::cout << "    const int: " << std::is_const_v<const int> << std::endl;
    std::cout << "    const double: " << std::is_const_v<const double> << std::endl;
    std::cout << "    const bool: " << std::is_const_v<const bool> << std::endl;
    
    // volatile 限定符
    volatile int volatile_int;
    volatile double volatile_double;
    
    std::cout << "  Volatile qualified types:" << std::endl;
    std::cout << "    volatile int: " << std::is_volatile_v<volatile int> << std::endl;
    std::cout << "    volatile double: " << std::is_volatile_v<volatile double> << std::endl;
    
    // const volatile 组合
    const volatile int const_volatile_int = 100;
    
    std::cout << "  Const-volatile qualified types:" << std::endl;
    std::cout << "    const volatile int: " << std::is_const_v<const volatile int> << ", " 
              << std::is_volatile_v<const volatile int> << std::endl;
    
    // 类型特征检查
    std::cout << "  Type trait checks:" << std::endl;
    std::cout << "    is_fundamental<const int>: " << std::is_fundamental_v<const int> << std::endl;
    std::cout << "    is_fundamental<volatile double>: " << std::is_fundamental_v<volatile double> << std::endl;
    std::cout << "    is_fundamental<const volatile bool>: " << std::is_fundamental_v<const volatile bool> << std::endl;
}
```

## 3. void 类型

### 3.1 void 类型基本概念

`void` 类型是一个特殊的类型，表示"无类型"或"空类型"。它具有以下特征：

- 不可实例化：不能声明 `void` 类型的对象
- 不完整类型：`void` 是不完整类型，无法确定其大小
- 用途特殊：主要用于函数返回类型、通用指针类型等场景

```cpp
#include <iostream>
#include <type_traits>

void void_type_detailed() {
    std::cout << "Void type detailed:" << std::endl;
    
    // 1. void 作为函数返回类型
    std::cout << "=== void as function return type ===" << std::endl;
    void print_message() {
        std::cout << "    This function returns nothing (void)" << std::endl;
    }
    
    print_message();
    
    // 2. void* 作为通用指针类型
    std::cout << "=== void* as generic pointer type ===" << std::endl;
    int value = 42;
    void* generic_ptr = &value;
    
    // 使用时需要类型转换
    int* int_ptr = static_cast<int*>(generic_ptr);
    std::cout << "    Value through void pointer: " << *int_ptr << std::endl;
    
    // 3. void 的限制
    std::cout << "=== void restrictions ===" << std::endl;
    // void void_object;  // 错误：不能声明 void 类型的对象
    // void void_array[10];  // 错误：数组元素不能是 void
    // void& void_ref;  // 错误：不能声明 void 的引用
    
    // 4. void 的大小
    std::cout << "=== void size information ===" << std::endl;
    std::cout << "    sizeof(void*) on this platform: " << sizeof(void*) << " bytes" << std::endl;
    
    // 5. 类型特征
    std::cout << "=== void type traits ===" << std::endl;
    std::cout << "    std::is_void<void>: " << std::is_void_v<void> << std::endl;
    std::cout << "    std::is_void<const void>: " << std::is_void_v<const void> << std::endl;
    std::cout << "    std::is_void<void*>: " << std::is_void_v<void*> << std::endl;
    std::cout << "    std::is_fundamental<void>: " << std::is_fundamental_v<void> << std::endl;
}
```

### 3.2 void 类型的应用场景

```cpp
#include <iostream>
#include <memory>

void void_type_usage_scenarios() {
    std::cout << "Void type usage scenarios:" << std::endl;
    
    // 1. 函数无返回值
    std::cout << "=== Functions with no return value ===" << std::endl;
    void initialize_system() {
        std::cout << "    System initialized" << std::endl;
    }
    
    initialize_system();
    
    // 2. 通用指针类型
    std::cout << "=== Generic pointer type ===" << std::endl;
    struct Data {
        int id;
        double value;
    };
    
    Data data = {1, 3.14};
    void* ptr = &data;
    
    // 类型擦除示例
    void process_generic_data(void* data_ptr, size_t size) {
        std::cout << "    Processing " << size << " bytes of data" << std::endl;
        // 实际使用时需要知道具体类型并进行转换
    }
    
    process_generic_data(ptr, sizeof(Data));
    
    // 3. 回调函数
    std::cout << "=== Callback functions ===" << std::endl;
    void (*callback)(void*) = nullptr;
    
    // 4. 内存操作
    std::cout << "=== Memory operations ===" << std::endl;
    char buffer[100];
    void* mem_ptr = buffer;
    std::memset(mem_ptr, 0, sizeof(buffer));
    std::cout << "    Buffer initialized with zeros" << std::endl;
}
```

## 4. std::nullptr_t 类型

### 4.1 std::nullptr_t 基本概念

`std::nullptr_t` 是 C++11 引入的类型，它是空指针字面量 `nullptr` 的类型。这个类型具有以下特点：

- 独立类型：既不是指针类型，也不是成员指针类型
- 单一值：只有一个值，即 `nullptr`
- 可转换性：可以隐式转换为任何指针类型或成员指针类型

```cpp
#include <iostream>
#include <typeinfo>
#include <cstddef>

void nullptr_t_detailed() {
    std::cout << "std::nullptr_t detailed:" << std::endl;
    
    // 1. nullptr 的类型是 std::nullptr_t
    std::cout << "=== nullptr type ===" << std::endl;
    std::nullptr_t null_value = nullptr;
    std::cout << "    Type of nullptr: " << typeid(nullptr).name() << std::endl;
    
    // 2. nullptr 可以隐式转换为任何指针类型
    std::cout << "=== Implicit conversion to pointer types ===" << std::endl;
    int* int_ptr = nullptr;
    double* double_ptr = nullptr;
    void* void_ptr = nullptr;
    
    std::cout << "    int* initialized with nullptr: " << int_ptr << std::endl;
    std::cout << "    double* initialized with nullptr: " << double_ptr << std::endl;
    std::cout << "    void* initialized with nullptr: " << void_ptr << std::endl;
    
    // 3. nullptr 与指针的比较
    std::cout << "=== nullptr comparison ===" << std::endl;
    if (int_ptr == nullptr) {
        std::cout << "    int_ptr is null" << std::endl;
    }
    
    // 4. sizeof 关系
    std::cout << "=== sizeof relationship ===" << std::endl;
    std::cout << "    sizeof(std::nullptr_t): " << sizeof(std::nullptr_t) << " bytes" << std::endl;
    std::cout << "    sizeof(void*): " << sizeof(void*) << " bytes" << std::endl;
    
    // 5. 类型特征
    std::cout << "=== nullptr_t type traits ===" << std::endl;
    std::cout << "    std::is_null_pointer<std::nullptr_t>: " << std::is_null_pointer_v<std::nullptr_t> << std::endl;
    std::cout << "    std::is_pointer<std::nullptr_t>: " << std::is_pointer_v<std::nullptr_t> << std::endl;
    std::cout << "    std::is_fundamental<std::nullptr_t>: " << std::is_fundamental_v<std::nullptr_t> << std::endl;
}
```

### 4.2 std::nullptr_t 的使用场景

```cpp
#include <iostream>
#include <memory>

class ExampleClass {
public:
    void method() {
        std::cout << "    ExampleClass::method() called" << std::endl;
    }
};

void nullptr_t_usage_scenarios() {
    std::cout << "std::nullptr_t usage scenarios:" << std::endl;
    
    // 1. 函数参数中的空指针
    std::cout << "=== Function parameters with null pointers ===" << std::endl;
    void process_pointer(int* ptr) {
        if (ptr == nullptr) {
            std::cout << "      Received null pointer" << std::endl;
        } else {
            std::cout << "      Received valid pointer with value: " << *ptr << std::endl;
        }
    }
    
    process_pointer(nullptr);
    int value = 42;
    process_pointer(&value);
    
    // 2. 成员指针
    std::cout << "=== Member pointers ===" << std::endl;
    void (ExampleClass::*member_ptr)() = nullptr;
    if (member_ptr == nullptr) {
        std::cout << "    Member pointer is null" << std::endl;
    }
    
    // 3. 智能指针初始化
    std::cout << "=== Smart pointer initialization ===" << std::endl;
    std::unique_ptr<int> smart_ptr = nullptr;
    if (smart_ptr == nullptr) {
        std::cout << "    Smart pointer is null" << std::endl;
    }
    
    // 4. 模板中的使用
    std::cout << "=== Template usage ===" << std::endl;
    template<typename T>
    void template_function(T* ptr = nullptr) {
        if (ptr == nullptr) {
            std::cout << "      Template function received null pointer" << std::endl;
        }
    }
    
    template_function<int>();  // 使用默认的 nullptr 参数
    
    // 5. 与 NULL 和 0 的区别
    std::cout << "=== Difference from NULL and 0 ===" << std::endl;
    int* ptr1 = nullptr;     // 类型安全
    int* ptr2 = NULL;        // 可能有重载解析问题
    int* ptr3 = 0;           // 可能有重载解析问题
    
    std::cout << "    nullptr assignment is type-safe" << std::endl;
}
```

## 5. 整数类型

### 5.1 整数类型分类

整数类型是 C++ 中用于表示整数值的基本类型，包括多种变体：

```cpp
#include <iostream>
#include <climits>
#include <type_traits>

void integer_types_classification() {
    std::cout << "Integer types classification:" << std::endl;
    
    // 按符号性分类
    std::cout << "=== Classification by signedness ===" << std::endl;
    
    // 有符号整数类型
    std::cout << "  Signed integer types:" << std::endl;
    signed char schar = -128;
    short sshort = -32768;
    int sint = -2147483648;
    long slong = -2147483648L;
    long long sllong = -9223372036854775808LL;
    
    std::cout << "    signed char: " << static_cast<int>(schar) << std::endl;
    std::cout << "    short: " << sshort << std::endl;
    std::cout << "    int: " << sint << std::endl;
    std::cout << "    long: " << slong << std::endl;
    std::cout << "    long long: " << sllong << std::endl;
    
    // 无符号整数类型
    std::cout << "  Unsigned integer types:" << std::endl;
    unsigned char uchar = 255;
    unsigned short ushort = 65535;
    unsigned int uint = 4294967295U;
    unsigned long ulong = 4294967295UL;
    unsigned long long ullong = 18446744073709551615ULL;
    
    std::cout << "    unsigned char: " << static_cast<int>(uchar) << std::endl;
    std::cout << "    unsigned short: " << ushort << std::endl;
    std::cout << "    unsigned int: " << uint << std::endl;
    std::cout << "    unsigned long: " << ulong << std::endl;
    std::cout << "    unsigned long long: " << ullong << std::endl;
    
    // 特殊整数类型
    std::cout << "=== Special integer types ===" << std::endl;
    bool boolean = true;              // 布尔类型
    char character = 'A';             // 字符类型
    wchar_t wide_char = L'中';         // 宽字符类型
    char16_t utf16_char = u'你';       // UTF-16 字符类型
    char32_t utf32_char = U'🌍';       // UTF-32 字符类型
    char8_t utf8_char = u8'A';         // UTF-8 字符类型 (C++20)
    
    std::cout << "    bool: " << boolean << std::endl;
    std::cout << "    char: " << character << " (" << static_cast<int>(character) << ")" << std::endl;
    std::cout << "    wchar_t: " << wide_char << std::endl;
    std::cout << "    char16_t: " << utf16_char << std::endl;
    std::cout << "    char32_t: " << utf32_char << std::endl;
    std::cout << "    char8_t: " << static_cast<int>(utf8_char) << std::endl;
}
```

### 5.2 整数类型大小和范围

```cpp
#include <iostream>
#include <climits>
#include <limits>

void integer_sizes_and_ranges() {
    std::cout << "Integer sizes and ranges:" << std::endl;
    
    // 字符类型范围
    std::cout << "=== Character Types ===" << std::endl;
    std::cout << "  char: " << CHAR_BIT << " bits" << std::endl;
    std::cout << "  CHAR_MIN: " << CHAR_MIN << ", CHAR_MAX: " << CHAR_MAX << std::endl;
    std::cout << "  SCHAR_MIN: " << SCHAR_MIN << ", SCHAR_MAX: " << SCHAR_MAX << std::endl;
    std::cout << "  UCHAR_MAX: " << UCHAR_MAX << std::endl;
    
    // 短整型范围
    std::cout << "=== Short Types ===" << std::endl;
    std::cout << "  short: " << sizeof(short) * 8 << " bits" << std::endl;
    std::cout << "  SHRT_MIN: " << SHRT_MIN << ", SHRT_MAX: " << SHRT_MAX << std::endl;
    std::cout << "  USHRT_MAX: " << USHRT_MAX << std::endl;
    
    // 整型范围
    std::cout << "=== Int Types ===" << std::endl;
    std::cout << "  int: " << sizeof(int) * 8 << " bits" << std::endl;
    std::cout << "  INT_MIN: " << INT_MIN << ", INT_MAX: " << INT_MAX << std::endl;
    std::cout << "  UINT_MAX: " << UINT_MAX << std::endl;
    
    // 长整型范围
    std::cout << "=== Long Types ===" << std::endl;
    std::cout << "  long: " << sizeof(long) * 8 << " bits" << std::endl;
    std::cout << "  LONG_MIN: " << LONG_MIN << ", LONG_MAX: " << LONG_MAX << std::endl;
    std::cout << "  ULONG_MAX: " << ULONG_MAX << std::endl;
    
    // 长长整型范围
    std::cout << "=== Long Long Types ===" << std::endl;
    std::cout << "  long long: " << sizeof(long long) * 8 << " bits" << std::endl;
    std::cout << "  LLONG_MIN: " << LLONG_MIN << ", LLONG_MAX: " << LLONG_MAX << std::endl;
    std::cout << "  ULLONG_MAX: " << ULLONG_MAX << std::endl;
    
    // 使用 std::numeric_limits 获取更详细信息
    std::cout << "=== Using std::numeric_limits ===" << std::endl;
    std::cout << "  int digits: " << std::numeric_limits<int>::digits << std::endl;
    std::cout << "  int digits10: " << std::numeric_limits<int>::digits10 << std::endl;
    std::cout << "  is_signed<int>: " << std::numeric_limits<int>::is_signed << std::endl;
    std::cout << "  is_modulo<int>: " << std::numeric_limits<int>::is_modulo << std::endl;
}
```

### 5.3 整数溢出行为

```cpp
#include <iostream>
#include <limits>

void integer_overflow_behavior() {
    std::cout << "Integer overflow behavior:" << std::endl;
    
    // 1. 有符号整数溢出 - 未定义行为
    std::cout << "=== Signed Integer Overflow (Undefined Behavior) ===" << std::endl;
    int max_int = std::numeric_limits<int>::max();
    std::cout << "  INT_MAX: " << max_int << std::endl;
    // int overflow = max_int + 1;  // 未定义行为，不要这样做
    
    // 2. 无符号整数溢出 - 模运算
    std::cout << "=== Unsigned Integer Overflow (Modulo Arithmetic) ===" << std::endl;
    unsigned int max_uint = std::numeric_limits<unsigned int>::max();
    std::cout << "  UINT_MAX: " << max_uint << std::endl;
    unsigned int overflow = max_uint + 1;  // 模运算，结果为 0
    std::cout << "  UINT_MAX + 1 = " << overflow << std::endl;
    
    // 3. 无符号整数回绕
    std::cout << "=== Unsigned Integer Wraparound ===" << std::endl;
    unsigned char uc = 0;
    uc--;  // 回绕到 255
    std::cout << "  0 - 1 (unsigned char) = " << static_cast<int>(uc) << std::endl;
    
    // 4. 溢出检测示例
    std::cout << "=== Overflow Detection ===" << std::endl;
    unsigned int a = 4000000000U;
    unsigned int b = 3000000000U;
    
    // 检测无符号溢出
    if (a > std::numeric_limits<unsigned int>::max() - b) {
        std::cout << "  Addition would overflow" << std::endl;
    } else {
        std::cout << "  Addition result: " << a + b << std::endl;
    }
}
```

## 6. 浮点类型

### 6.1 浮点类型分类

C++ 提供了三种标准浮点类型，它们通常对应 IEEE 754 标准：

```cpp
#include <iostream>
#include <limits>
#include <cmath>

void floating_point_types_classification() {
    std::cout << "Floating-point types classification:" << std::endl;
    
    // 1. float - 单精度浮点数
    std::cout << "=== float (single precision) ===" << std::endl;
    float f = 3.141592653589793238f;
    std::cout << "  Value: " << f << std::endl;
    std::cout << "  Size: " << sizeof(float) << " bytes" << std::endl;
    std::cout << "  Precision: " << std::numeric_limits<float>::digits10 << " decimal digits" << std::endl;
    
    // 2. double - 双精度浮点数
    std::cout << "=== double (double precision) ===" << std::endl;
    double d = 3.141592653589793238;
    std::cout << "  Value: " << d << std::endl;
    std::cout << "  Size: " << sizeof(double) << " bytes" << std::endl;
    std::cout << "  Precision: " << std::numeric_limits<double>::digits10 << " decimal digits" << std::endl;
    
    // 3. long double - 扩展精度浮点数
    std::cout << "=== long double (extended precision) ===" << std::endl;
    long double ld = 3.141592653589793238L;
    std::cout << "  Value: " << ld << std::endl;
    std::cout << "  Size: " << sizeof(long double) << " bytes" << std::endl;
    std::cout << "  Precision: " << std::numeric_limits<long double>::digits10 << " decimal digits" << std::endl;
    
    // 类型特征
    std::cout << "=== Type traits ===" << std::endl;
    std::cout << "  is_floating_point<float>: " << std::is_floating_point_v<float> << std::endl;
    std::cout << "  is_floating_point<double>: " << std::is_floating_point_v<double> << std::endl;
    std::cout << "  is_floating_point<long double>: " << std::is_floating_point_v<long double> << std::endl;
}
```

### 6.2 浮点数特殊值

```cpp
#include <iostream>
#include <limits>
#include <cmath>

void floating_point_special_values() {
    std::cout << "Floating-point special values:" << std::endl;
    
    // 1. 无穷大
    std::cout << "=== Infinity ===" << std::endl;
    float infinity = std::numeric_limits<float>::infinity();
    std::cout << "  Positive infinity: " << infinity << std::endl;
    std::cout << "  Negative infinity: " << -infinity << std::endl;
    std::cout << "  1.0 / 0.0 = " << 1.0f / 0.0f << std::endl;
    std::cout << "  -1.0 / 0.0 = " << -1.0f / 0.0f << std::endl;
    
    // 2. NaN (Not a Number)
    std::cout << "=== NaN (Not a Number) ===" << std::endl;
    float nan_value = std::numeric_limits<float>::quiet_NaN();
    std::cout << "  NaN value: " << nan_value << std::endl;
    std::cout << "  0.0 / 0.0 = " << 0.0f / 0.0f << std::endl;
    std::cout << "  sqrt(-1.0) = " << std::sqrt(-1.0f) << std::endl;
    
    // 3. 负零
    std::cout << "=== Negative Zero ===" << std::endl;
    float negative_zero = -0.0f;
    std::cout << "  Negative zero: " << negative_zero << std::endl;
    std::cout << "  1.0 / negative_zero = " << 1.0f / negative_zero << std::endl;
    std::cout << "  1.0 / 0.0 = " << 1.0f / 0.0f << std::endl;
    
    // 4. 特殊值检测
    std::cout << "=== Special Value Detection ===" << std::endl;
    std::cout << "  isinf(infinity): " << std::isinf(infinity) << std::endl;
    std::cout << "  isnan(nan_value): " << std::isnan(nan_value) << std::endl;
    std::cout << "  negative_zero == 0.0f: " << (negative_zero == 0.0f) << std::endl;
    std::cout << "  1.0f / negative_zero < 0: " << (1.0f / negative_zero < 0) << std::endl;
}
```

### 6.3 浮点数精度和舍入

```cpp
#include <iostream>
#include <iomanip>
#include <limits>

void floating_point_precision_and_rounding() {
    std::cout << "Floating-point precision and rounding:" << std::endl;
    
    // 1. 精度演示
    std::cout << "=== Precision Demonstration ===" << std::endl;
    float f = 1.0f / 3.0f;
    double d = 1.0 / 3.0;
    long double ld = 1.0L / 3.0L;
    
    std::cout << std::setprecision(20);
    std::cout << "  float 1/3: " << f << std::endl;
    std::cout << "  double 1/3: " << d << std::endl;
    std::cout << "  long double 1/3: " << ld << std::endl;
    
    // 2. 累积误差演示
    std::cout << "=== Accumulated Error Demonstration ===" << std::endl;
    float sum_f = 0.0f;
    double sum_d = 0.0;
    
    for (int i = 0; i < 1000000; ++i) {
        sum_f += 0.1f;
        sum_d += 0.1;
    }
    
    std::cout << "  Sum of 0.1f added 1,000,000 times: " << sum_f << std::endl;
    std::cout << "  Expected: " << 100000.0f << std::endl;
    std::cout << "  Error: " << (sum_f - 100000.0f) << std::endl;
    
    std::cout << "  Sum of 0.1 added 1,000,000 times: " << sum_d << std::endl;
    std::cout << "  Expected: " << 100000.0 << std::endl;
    std::cout << "  Error: " << (sum_d - 100000.0) << std::endl;
    
    // 3. 机器 epsilon
    std::cout << "=== Machine Epsilon ===" << std::endl;
    std::cout << "  float epsilon: " << std::numeric_limits<float>::epsilon() << std::endl;
    std::cout << "  double epsilon: " << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << "  long double epsilon: " << std::numeric_limits<long double>::epsilon() << std::endl;
}
```

## 7. 布尔类型

### 7.1 bool 类型基础

`bool` 类型是 C++ 中的布尔类型，用于表示真/假值：

```cpp
#include <iostream>
#include <type_traits>

void boolean_type_basics() {
    std::cout << "Boolean type basics:" << std::endl;
    
    // 1. 基本布尔值
    std::cout << "=== Basic Boolean Values ===" << std::endl;
    bool true_value = true;
    bool false_value = false;
    
    std::cout << "  true: " << true_value << std::endl;
    std::cout << "  false: " << false_value << std::endl;
    
    // 2. 整数到布尔的转换
    std::cout << "=== Integer to Boolean Conversion ===" << std::endl;
    bool from_zero = static_cast<bool>(0);      // false
    bool from_nonzero = static_cast<bool>(42);  // true
    bool from_negative = static_cast<bool>(-1); // true
    
    std::cout << "  bool(0): " << from_zero << std::endl;
    std::cout << "  bool(42): " << from_nonzero << std::endl;
    std::cout << "  bool(-1): " << from_negative << std::endl;
    
    // 3. 布尔到整数的转换
    std::cout << "=== Boolean to Integer Conversion ===" << std::endl;
    int from_true = static_cast<int>(true);   // 1
    int from_false = static_cast<int>(false); // 0
    
    std::cout << "  int(true): " << from_true << std::endl;
    std::cout << "  int(false): " << from_false << std::endl;
    
    // 4. 布尔类型特征
    std::cout << "=== Boolean Type Traits ===" << std::endl;
    std::cout << "  is_arithmetic<bool>: " << std::is_arithmetic_v<bool> << std::endl;
    std::cout << "  is_integral<bool>: " << std::is_integral_v<bool> << std::endl;
    std::cout << "  sizeof(bool): " << sizeof(bool) << " bytes" << std::endl;
}
```

### 7.2 布尔类型在条件表达式中的使用

```cpp
#include <iostream>

void boolean_in_conditions() {
    std::cout << "Boolean in conditions:" << std::endl;
    
    // 1. if 语句
    std::cout << "=== if statements ===" << std::endl;
    bool condition = true;
    if (condition) {
        std::cout << "  Condition is true" << std::endl;
    } else {
        std::cout << "  Condition is false" << std::endl;
    }
    
    // 2. while 循环
    std::cout << "=== while loops ===" << std::endl;
    bool continue_loop = true;
    int counter = 0;
    while (continue_loop && counter < 3) {
        std::cout << "  Loop iteration " << counter << std::endl;
        counter++;
        if (counter >= 3) {
            continue_loop = false;
        }
    }
    
    // 3. for 循环条件
    std::cout << "=== for loop conditions ===" << std::endl;
    for (bool flag = true; flag; ) {
        std::cout << "  One-time loop" << std::endl;
        flag = false;  // 确保循环只执行一次
    }
    
    // 4. 三元运算符
    std::cout << "=== ternary operator ===" << std::endl;
    bool flag = true;
    std::string result = flag ? "TRUE" : "FALSE";
    std::cout << "  Ternary result: " << result << std::endl;
    
    // 5. 逻辑运算符
    std::cout << "=== logical operators ===" << std::endl;
    bool a = true;
    bool b = false;
    
    std::cout << "  a && b: " << (a && b) << std::endl;  // 逻辑与
    std::cout << "  a || b: " << (a || b) << std::endl;  // 逻辑或
    std::cout << "  !a: " << (!a) << std::endl;          // 逻辑非
}
```

## 8. 字符类型

### 8.1 字符类型分类

字符类型是用于字符表示的特殊整数类型，包括多种变体：

```cpp
#include <iostream>
#include <type_traits>

void character_types_classification() {
    std::cout << "Character types classification:" << std::endl;
    
    // 1. char 类型
    std::cout << "=== char type ===" << std::endl;
    char c1 = 'A';                    // 普通字符
    signed char c2 = -128;            // 有符号字符
    unsigned char c3 = 255;           // 无符号字符
    
    std::cout << "  char: " << c1 << " (" << static_cast<int>(c1) << ")" << std::endl;
    std::cout << "  signed char: " << static_cast<int>(c2) << std::endl;
    std::cout << "  unsigned char: " << static_cast<int>(c3) << std::endl;
    
    // 2. 宽字符类型
    std::cout << "=== Wide character types ===" << std::endl;
    wchar_t wide_char = L'中';         // 宽字符
    std::cout << "  wchar_t: " << wide_char << std::endl;
    
    // 3. Unicode 字符类型 (C++11)
    std::cout << "=== Unicode character types ===" << std::endl;
    char16_t utf16_char = u'你';       // UTF-16 字符
    char32_t utf32_char = U'🌍';       // UTF-32 字符
    
    std::cout << "  char16_t: " << utf16_char << std::endl;
    std::cout << "  char32_t: " << utf32_char << std::endl;
    
    // 4. UTF-8 字符类型 (C++20)
    std::cout << "=== UTF-8 character type ===" << std::endl;
    char8_t utf8_char = u8'A';         // UTF-8 字符
    std::cout << "  char8_t: " << static_cast<int>(utf8_char) << std::endl;
    
    // 5. 类型特征
    std::cout << "=== Character type traits ===" << std::endl;
    std::cout << "  is_integral<char>: " << std::is_integral_v<char> << std::endl;
    std::cout << "  is_integral<wchar_t>: " << std::is_integral_v<wchar_t> << std::endl;
    std::cout << "  is_integral<char16_t>: " << std::is_integral_v<char16_t> << std::endl;
    std::cout << "  is_integral<char32_t>: " << std::is_integral_v<char32_t> << std::endl;
}
```

### 8.2 字符类型的使用场景

```cpp
#include <iostream>
#include <string>

void character_type_usage_scenarios() {
    std::cout << "Character type usage scenarios:" << std::endl;
    
    // 1. 字符串处理
    std::cout << "=== String processing ===" << std::endl;
    char c_str[] = "Hello, World!";
    std::string cpp_str = "Hello, C++!";
    
    std::cout << "  C-style string: " << c_str << std::endl;
    std::cout << "  C++ string: " << cpp_str << std::endl;
    
    // 2. 原始内存访问
    std::cout << "=== Raw memory access ===" << std::endl;
    int value = 0x12345678;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
    
    std::cout << "  Bytes of integer 0x" << std::hex << value << std::dec << ": ";
    for (size_t i = 0; i < sizeof(int); i++) {
        printf("0x%02X ", bytes[i]);
    }
    std::cout << std::endl;
    
    // 3. 字符编码处理
    std::cout << "=== Character encoding ===" << std::endl;
    char ascii_char = 'A';              // ASCII 字符
    wchar_t wide_char = L'中';           // 宽字符
    char8_t utf8_char = u8'🌍';          // UTF-8 字符 (C++20)
    char16_t utf16_char = u'你';         // UTF-16 字符
    char32_t utf32_char = U'🚀';         // UTF-32 字符
    
    std::cout << "  ASCII char: " << ascii_char << std::endl;
    std::cout << "  Wide char: " << wide_char << std::endl;
    std::cout << "  UTF-8 char: " << static_cast<int>(utf8_char) << std::endl;
    std::cout << "  UTF-16 char: " << utf16_char << std::endl;
    std::cout << "  UTF-32 char: " << utf32_char << std::endl;
}
```

## 9. 数据模型

### 9.1 数据模型概述

不同的平台和架构可能有不同的数据模型，影响基本类型的大小：

```cpp
#include <iostream>
#include <climits>

void data_models_overview() {
    std::cout << "Data models overview:" << std::endl;
    
    std::cout << "=== Current Platform Data Model ===" << std::endl;
    std::cout << "  sizeof(char): " << sizeof(char) << " bytes" << std::endl;
    std::cout << "  sizeof(short): " << sizeof(short) << " bytes" << std::endl;
    std::cout << "  sizeof(int): " << sizeof(int) << " bytes" << std::endl;
    std::cout << "  sizeof(long): " << sizeof(long) << " bytes" << std::endl;
    std::cout << "  sizeof(long long): " << sizeof(long long) << " bytes" << std::endl;
    std::cout << "  sizeof(void*): " << sizeof(void*) << " bytes" << std::endl;
    std::cout << "  sizeof(float): " << sizeof(float) << " bytes" << std::endl;
    std::cout << "  sizeof(double): " << sizeof(double) << " bytes" << std::endl;
    std::cout << "  sizeof(long double): " << sizeof(long double) << " bytes" << std::endl;
    
    // 常见数据模型
    std::cout << "=== Common Data Models ===" << std::endl;
    std::cout << "  LP32 (Win16): int=16, long=32, pointer=32" << std::endl;
    std::cout << "  ILP32 (Unix 32-bit): int=32, long=32, pointer=32" << std::endl;
    std::cout << "  LLP64 (Win64): int=32, long=32, long long=64, pointer=64" << std::endl;
    std::cout << "  LP64 (Unix 64-bit): int=32, long=64, pointer=64" << std::endl;
    
    // C++ 标准保证的大小关系
    std::cout << "=== C++ Standard Size Guarantees ===" << std::endl;
    std::cout << "  1 == sizeof(char) <= sizeof(short) <= sizeof(int)" << std::endl;
    std::cout << "  <= sizeof(long) <= sizeof(long long)" << std::endl;
}
```

### 9.2 平台特定考虑

```cpp
#include <iostream>
#include <type_traits>

void platform_specific_considerations() {
    std::cout << "Platform-specific considerations:" << std::endl;
    
    // 1. 字符类型的符号性
    std::cout << "=== Character Type Signedness ===" << std::endl;
    std::cout << "  is_signed<char>: " << std::is_signed_v<char> << std::endl;
    std::cout << "  is_unsigned<char>: " << std::is_unsigned_v<char> << std::endl;
    std::cout << "  Note: char signedness is implementation-defined" << std::endl;
    
    // 2. 对齐要求
    std::cout << "=== Alignment Requirements ===" << std::endl;
    std::cout << "  alignof(char): " << alignof(char) << std::endl;
    std::cout << "  alignof(int): " << alignof(int) << std::endl;
    std::cout << "  alignof(double): " << alignof(double) << std::endl;
    std::cout << "  alignof(long double): " << alignof(long double) << std::endl;
    
    // 3. 字节序（endianness）相关
    std::cout << "=== Endianness Considerations ===" << std::endl;
    union {
        uint32_t i;
        char c[4];
    } test = {0x01020304};
    
    if (test.c[0] == 1) {
        std::cout << "  Big-endian system detected" << std::endl;
    } else {
        std::cout << "  Little-endian system detected" << std::endl;
    }
    
    // 4. 浮点数表示
    std::cout << "=== Floating-Point Representation ===" << std::endl;
    std::cout << "  std::numeric_limits<float>::is_iec559: " 
              << std::numeric_limits<float>::is_iec559 << std::endl;
    std::cout << "  std::numeric_limits<double>::is_iec559: " 
              << std::numeric_limits<double>::is_iec559 << std::endl;
}
```

## 10. 类型特征和属性

### 10.1 类型特征概念

C++ 提供了丰富的类型特征来在编译时查询类型的属性：

```cpp
#include <iostream>
#include <type_traits>

void type_traits_concepts() {
    std::cout << "Type traits concepts:" << std::endl;
    
    // 基本类型特征
    std::cout << "=== Fundamental Type Traits ===" << std::endl;
    std::cout << "  is_fundamental<int>: " << std::is_fundamental_v<int> << std::endl;
    std::cout << "  is_fundamental<void>: " << std::is_fundamental_v<void> << std::endl;
    std::cout << "  is_fundamental<std::nullptr_t>: " << std::is_fundamental_v<std::nullptr_t> << std::endl;
    
    // 算术类型特征
    std::cout << "=== Arithmetic Type Traits ===" << std::endl;
    std::cout << "  is_arithmetic<int>: " << std::is_arithmetic_v<int> << std::endl;
    std::cout << "  is_arithmetic<double>: " << std::is_arithmetic_v<double> << std::endl;
    std::cout << "  is_arithmetic<bool>: " << std::is_arithmetic_v<bool> << std::endl;
    
    // 整数类型特征
    std::cout << "=== Integer Type Traits ===" << std::endl;
    std::cout << "  is_integral<int>: " << std::is_integral_v<int> << std::endl;
    std::cout << "  is_integral<char>: " << std::is_integral_v<char> << std::endl;
    std::cout << "  is_integral<bool>: " << std::is_integral_v<bool> << std::endl;
    
    // 浮点类型特征
    std::cout << "=== Floating-Point Type Traits ===" << std::endl;
    std::cout << "  is_floating_point<float>: " << std::is_floating_point_v<float> << std::endl;
    std::cout << "  is_floating_point<double>: " << std::is_floating_point_v<double> << std::endl;
    std::cout << "  is_floating_point<int>: " << std::is_floating_point_v<int> << std::endl;
}
```

### 10.2 CV 限定符特征

```cpp
#include <iostream>
#include <type_traits>

void cv_qualifier_traits() {
    std::cout << "CV qualifier traits:" << std::endl;
    
    // const 限定符特征
    std::cout << "=== const qualifier traits ===" << std::endl;
    std::cout << "  is_const<int>: " << std::is_const_v<int> << std::endl;
    std::cout << "  is_const<const int>: " << std::is_const_v<const int> << std::endl;
    std::cout << "  is_const<const volatile int>: " << std::is_const_v<const volatile int> << std::endl;
    
    // volatile 限定符特征
    std::cout << "=== volatile qualifier traits ===" << std::endl;
    std::cout << "  is_volatile<int>: " << std::is_volatile_v<int> << std::endl;
    std::cout << "  is_volatile<volatile int>: " << std::is_volatile_v<volatile int> << std::endl;
    std::cout << "  is_volatile<const volatile int>: " << std::is_volatile_v<const volatile int> << std::endl;
    
    // 移除 CV 限定符
    std::cout << "=== Removing CV qualifiers ===" << std::endl;
    std::cout << "  remove_const<const int>::type is int: " 
              << std::is_same_v<std::remove_const_t<const int>, int> << std::endl;
    std::cout << "  remove_volatile<volatile int>::type is int: " 
              << std::is_same_v<std::remove_volatile_t<volatile int>, int> << std::endl;
    std::cout << "  remove_cv<const volatile int>::type is int: " 
              << std::is_same_v<std::remove_cv_t<const volatile int>, int> << std::endl;
}
```

## 11. 最佳实践

### 11.1 类型选择最佳实践

```cpp
#include <iostream>
#include <cstdint>

void type_selection_best_practices() {
    std::cout << "Type selection best practices:" << std::endl;
    
    // 1. 使用固定宽度整数类型（当需要特定大小时）
    std::cout << "=== Fixed-Width Integer Types ===" << std::endl;
    std::int32_t fixed_int = 42;      // 确保 32 位
    std::int64_t fixed_long = 1000LL; // 确保 64 位
    
    std::cout << "  int32_t: " << fixed_int << std::endl;
    std::cout << "  int64_t: " << fixed_long << std::endl;
    
    // 2. 使用 size_t 进行大小和计数
    std::cout << "=== Using size_t for Sizes and Counts ===" << std::endl;
    std::size_t array_size = 10;
    int array[array_size];
    
    std::cout << "  Array size type: size_t" << std::endl;
    std::cout << "  sizeof(array): " << sizeof(array) << " bytes" << std::endl;
    
    // 3. 使用 nullptr 而不是 NULL 或 0
    std::cout << "=== Using nullptr ===" << std::endl;
    int* ptr = nullptr;  // 类型安全的空指针
    
    if (ptr == nullptr) {
        std::cout << "  Pointer is properly initialized to nullptr" << std::endl;
    }
    
    // 4. 选择合适的浮点类型
    std::cout << "=== Choosing Appropriate Floating-Point Types ===" << std::endl;
    float fast_float = 3.14f;           // 用于性能关键场合
    double precise_double = 3.141592653589793;  // 用于需要高精度的场合
    
    std::cout << "  Use float for performance: " << fast_float << std::endl;
    std::cout << "  Use double for precision: " << precise_double << std::endl;
}
```

### 11.2 类型安全编程

```cpp
#include <iostream>
#include <type_traits>

void type_safe_programming() {
    std::cout << "Type-safe programming practices:" << std::endl;
    
    // 1. 使用类型特征进行编译时检查
    std::cout << "=== Compile-Time Type Checking ===" << std::endl;
    template<typename T>
    void safe_arithmetic_operation(T value) {
        static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
        std::cout << "  Safe arithmetic operation on: " << value << std::endl;
    }
    
    safe_arithmetic_operation(42);
    safe_arithmetic_operation(3.14);
    
    // 2. 使用强类型枚举避免隐式转换
    std::cout << "=== Strongly-Typed Enums ===" << std::endl;
    enum class Color { RED, GREEN, BLUE };
    enum class Status { OK = 200, NOT_FOUND = 404 };
    
    Color color = Color::RED;
    Status status = Status::OK;
    
    // Color invalid = 1;  // 错误：不能隐式转换
    // if (color == status) // 错误：不同类型不能比较
    
    std::cout << "  Strongly-typed enums prevent implicit conversions" << std::endl;
    
    // 3. 避免危险的类型转换
    std::cout << "=== Safe Type Conversions ===" << std::endl;
    int int_value = 42;
    double double_value = static_cast<double>(int_value);  // 明确的类型转换
    std::cout << "  Safe cast: " << int_value << " -> " << double_value << std::endl;
    
    // 4. 使用 auto 进行类型推导（谨慎使用）
    std::cout << "=== Type Deduction with auto ===" << std::endl;
    auto deduced_int = 42;        // 推导为 int
    auto deduced_double = 3.14;   // 推导为 double
    
    std::cout << "  auto deduced int: " << deduced_int << " (type: " 
              << typeid(deduced_int).name() << ")" << std::endl;
    std::cout << "  auto deduced double: " << deduced_double << " (type: " 
              << typeid(deduced_double).name() << ")" << std::endl;
}
```

## 12. 标准参考

C++ 基本类型的相关标准条款：

- C++23 标准（ISO/IEC 14882:2024）：6.8.2 基本类型 [basic.fundamental]
- C++20 标准（ISO/IEC 14882:2020）：6.8.1 基本类型 [basic.fundamental]
- C++17 标准（ISO/IEC 14882:2017）：6.9.1 基本类型 [basic.fundamental]
- C++14 标准（ISO/IEC 14882:2014）：3.9.1 基本类型 [basic.fundamental]
- C++11 标准（ISO/IEC 14882:2011）：3.9.1 基本类型 [basic.fundamental]
- C++98 标准（ISO/IEC 14882:1998）：3.9.1 基本类型 [basic.fundamental]

相关的标准参考还包括：

- 5.2 表达式 [expr]
- 18.2 类型支持 [support.types]
- 18.3 限定符 [support.limits]
- 26 类型特征 [meta]

## 13. 总结

C++ 基本类型是 C++ 类型系统的基础，理解它们的特性和行为对于编写高质量的 C++ 程序至关重要。

### 13.1 核心要点

- void 类型：表示无类型，不能实例化对象
- nullptr_t 类型：空指针字面量的类型，可以隐式转换为任何指针类型
- 整数类型：包括字符类型、标准整数类型和布尔类型
- 浮点类型：float、double 和 long double，通常遵循 IEEE 754 标准
- 平台相关性：类型大小和行为可能因平台而异

### 13.2 最佳实践

- 类型安全：使用强类型、类型特征和编译时检查
- 明确转换：避免隐式转换，使用显式类型转换
- 固定宽度：当需要特定大小时使用 `<cstdint>` 中的类型
- 现代特性：充分利用 C++11 及以后版本的新特性
- 可移植性：考虑不同平台的差异，编写可移植代码

通过深入理解和正确应用 C++ 基本类型，开发者可以编写出更加安全、高效和可维护的 C++ 程序。
