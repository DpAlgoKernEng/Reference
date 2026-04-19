# C++ 替换文本宏详解

  [1. C++ 替换文本宏基本概念](#1-c-替换文本宏基本概念)
  
  [2. 对象宏详解](#2-对象宏详解)
  
  [3. 函数宏详解](#3-函数宏详解)
  
  [4. 特殊运算符](#4-特殊运算符)
  
  [5. 宏定义与取消定义](#5-宏定义与取消定义)
  
  [6. 宏替换规则](#6-宏替换规则)
  
  [7. 预定义宏](#7-预定义宏)
  
  [8. 高级宏技巧](#8-高级宏技巧)
  
  [9. C++ 特有特性](#9-c-特有特性)
  
  [10. 最佳实践](#10-最佳实践)
  
  [11. 标准参考](#11-标准参考)
  
  [12. 总结](#12-总结)

## 1. C++ 替换文本宏基本概念

### 1.1 什么是 C++ 替换文本宏

C++ 替换文本宏（Text Macro Replacement）是 C++ 预处理器提供的一种文本替换机制，它继承了 C 语言的宏替换功能，并在某些方面进行了扩展。宏定义使用 `#define` 指令创建，允许程序员定义标识符与替换文本之间的映射关系。在预处理阶段，预处理器会将源代码中所有匹配的标识符替换为对应的替换文本。

C++ 中的宏主要分为两类：

- 对象宏（Object-like Macros）：类似常量的宏定义
- 函数宏（Function-like Macros）：类似函数调用的宏定义

```cpp
#include <iostream>

// 对象宏示例
#define PI 3.14159
#define MAX_BUFFER_SIZE 1024
#define DEBUG_MODE 1

// 函数宏示例
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main() {
    std::cout << "PI value: " << PI << std::endl;
    std::cout << "Buffer size: " << MAX_BUFFER_SIZE << std::endl;
    
    int x = 5;
    std::cout << "Square of " << x << ": " << SQUARE(x) << std::endl;
    std::cout << "Max of 10 and 20: " << MAX(10, 20) << std::endl;
    
    #if DEBUG_MODE
    std::cout << "Debug mode is enabled" << std::endl;
    #endif
    
    return 0;
}
```

### 1.2 C++ 与 C 宏的区别

虽然 C++ 继承了 C 的宏替换机制，但在某些方面有所不同：

- 标准库保护：C++ 对标准库名称有更严格的保护
- 可变参数宏：C++11 引入了可变参数宏支持
- 语言特性检测：C++ 提供了语言特性检测宏
- 预定义宏：C++ 有自己特定的预定义宏

```cpp
#include <iostream>
#include <vector>

// C++ 特有的预定义宏
#define SHOW_CPP_MACROS() \
    std::cout << "__cplusplus: " << __cplusplus << std::endl; \
    std::cout << "__STDC_HOSTED__: " << __STDC_HOSTED__ << std::endl; \
    std::cout << "__STDCPP_THREADS__: " << __STDCPP_THREADS__ << std::endl;

int main() {
    SHOW_CPP_MACROS();
    
    // C++ 标准库使用示例
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "Vector size: " << vec.size() << std::endl;
    
    return 0;
}
```

### 1.3 替换文本宏的作用

替换文本宏在 C++ 编程中发挥着重要作用：

- 常量定义：定义程序中使用的常量值
- 代码简化：简化复杂表达式的书写
- 条件编译：配合条件编译指令实现功能开关
- 代码生成：通过宏生成重复的代码结构
- 平台适配：根据平台定义不同的实现
- 调试支持：在调试和发布版本中包含不同代码
- 性能优化：内联展开以提高性能

## 2. 对象宏详解

### 2.1 基本对象宏

基本对象宏是最简单的宏形式，将标识符替换为简单的文本：

```cpp
#include <iostream>

// 简单数值宏
#define COUNT 100
#define FLAG 1
#define ENABLE_FEATURE 0

// 简单字符串宏
#define PROGRAM_NAME "MyProgram"
#define VERSION_STRING "1.0.0"

// 简单表达式宏
#define TWO_PI (2 * 3.14159)
#define KILOBYTE 1024

int main() {
    std::cout << "Program: " << PROGRAM_NAME << " v" << VERSION_STRING << std::endl;
    std::cout << "Count: " << COUNT << std::endl;
    
    if (FLAG) {
        std::cout << "Flag is set" << std::endl;
    }
    
    #if ENABLE_FEATURE
    std::cout << "Feature is enabled" << std::endl;
    #else
    std::cout << "Feature is disabled" << std::endl;
    #endif
    
    std::cout << "Two PI: " << TWO_PI << std::endl;
    std::cout << "1 KB = " << KILOBYTE << " bytes" << std::endl;
    
    return 0;
}
```

### 2.2 常量对象宏

常量对象宏用于定义程序中使用的各种常量：

```cpp
#include <iostream>

// 数学常量
#define PI 3.14159265359
#define E 2.71828182846
#define SQRT2 1.41421356237

// 物理常量
#define SPEED_OF_LIGHT 299792458  // m/s
#define GRAVITY 9.80665  // m/s^2

// 计算机科学常量
#define BITS_PER_BYTE 8
#define BYTES_PER_KB 1024
#define BYTES_PER_MB (1024 * 1024)

// 颜色常量
#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF

int main() {
    std::cout << "Mathematical constants:" << std::endl;
    std::cout << "  PI = " << PI << std::endl;
    std::cout << "  E = " << E << std::endl;
    std::cout << "  SQRT2 = " << SQRT2 << std::endl;
    
    std::cout << "Physical constants:" << std::endl;
    std::cout << "  Speed of light = " << SPEED_OF_LIGHT << " m/s" << std::endl;
    std::cout << "  Gravity = " << GRAVITY << " m/s^2" << std::endl;
    
    std::cout << "Computer science constants:" << std::endl;
    std::cout << "  Bits per byte = " << BITS_PER_BYTE << std::endl;
    std::cout << "  1 KB = " << BYTES_PER_KB << " bytes" << std::endl;
    std::cout << "  1 MB = " << BYTES_PER_MB << " bytes" << std::endl;
    
    std::cout << "Colors:" << std::endl;
    std::cout << "  Red = 0x" << std::hex << COLOR_RED << std::dec << std::endl;
    std::cout << "  Green = 0x" << std::hex << COLOR_GREEN << std::dec << std::endl;
    std::cout << "  Blue = 0x" << std::hex << COLOR_BLUE << std::dec << std::endl;
    
    return 0;
}
```

### 2.3 字符串对象宏

字符串对象宏用于定义程序中使用的字符串常量：

```cpp
#include <iostream>
#include <string>

// 程序信息
#define APP_NAME "MyApplication"
#define APP_VERSION "2.1.3"
#define APP_AUTHOR "John Doe"
#define APP_COPYRIGHT "Copyright (c) 2024"

// 路径和文件名
#define CONFIG_FILE "config.ini"
#define LOG_FILE "application.log"
#define TEMP_DIR "/tmp/"

// 错误消息
#define ERROR_INVALID_INPUT "Invalid input provided"
#define ERROR_FILE_NOT_FOUND "File not found"
#define ERROR_PERMISSION_DENIED "Permission denied"

// 数据库连接字符串
#define DB_HOST "localhost"
#define DB_PORT 5432
#define DB_NAME "myapp_db"
#define DB_USER "myapp_user"

int main() {
    std::cout << "Application Information:" << std::endl;
    std::cout << "  Name: " << APP_NAME << std::endl;
    std::cout << "  Version: " << APP_VERSION << std::endl;
    std::cout << "  Author: " << APP_AUTHOR << std::endl;
    std::cout << "  " << APP_COPYRIGHT << std::endl;
    
    std::cout << "File paths:" << std::endl;
    std::cout << "  Config: " << CONFIG_FILE << std::endl;
    std::cout << "  Log: " << LOG_FILE << std::endl;
    std::cout << "  Temp: " << TEMP_DIR << std::endl;
    
    std::cout << "Error messages:" << std::endl;
    std::cout << "  Invalid input: " << ERROR_INVALID_INPUT << std::endl;
    std::cout << "  File not found: " << ERROR_FILE_NOT_FOUND << std::endl;
    std::cout << "  Permission denied: " << ERROR_PERMISSION_DENIED << std::endl;
    
    std::cout << "Database connection:" << std::endl;
    std::cout << "  Host: " << DB_HOST << ":" << DB_PORT << std::endl;
    std::cout << "  Database: " << DB_NAME << std::endl;
    std::cout << "  User: " << DB_USER << std::endl;
    
    return 0;
}
```

### 2.4 复杂表达式对象宏

复杂表达式对象宏用于定义复杂的表达式或语句：

```cpp
#include <iostream>
#include <memory>

// 内存分配宏
#define SAFE_NEW(ptr, type) do { \
    try { \
        ptr = new type; \
    } catch (...) { \
        std::cerr << "Memory allocation failed" << std::endl; \
        ptr = nullptr; \
    } \
} while(0)

// 数组大小计算宏
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// 位操作宏
#define SET_BIT(x, n) ((x) |= (1 << (n)))
#define CLEAR_BIT(x, n) ((x) &= ~(1 << (n)))
#define TOGGLE_BIT(x, n) ((x) ^= (1 << (n)))
#define CHECK_BIT(x, n) (((x) >> (n)) & 1)

// 循环宏
#define FOR_EACH(i, start, end) for (int i = (start); i < (end); i++)
#define REVERSE_FOR_EACH(i, start, end) for (int i = (end) - 1; i >= (start); i--)

// 安全释放宏
#define SAFE_DELETE(ptr) do { \
    if (ptr != nullptr) { \
        delete ptr; \
        ptr = nullptr; \
    } \
} while(0)

int main() {
    // 测试数组大小宏
    int numbers[] = {1, 2, 3, 4, 5};
    std::cout << "Array size: " << ARRAY_SIZE(numbers) << std::endl;
    
    // 测试位操作宏
    unsigned int value = 0;
    SET_BIT(value, 3);
    std::cout << "After setting bit 3: " << value << std::endl;
    std::cout << "Bit 3 is set: " << CHECK_BIT(value, 3) << std::endl;
    
    TOGGLE_BIT(value, 3);
    std::cout << "After toggling bit 3: " << value << std::endl;
    std::cout << "Bit 3 is set: " << CHECK_BIT(value, 3) << std::endl;
    
    // 测试循环宏
    std::cout << "Forward loop: ";
    FOR_EACH(i, 0, 5) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Reverse loop: ";
    REVERSE_FOR_EACH(i, 0, 5) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // 测试内存分配宏
    int* array;
    SAFE_NEW(array, int[10]);
    if (array != nullptr) {
        std::cout << "Memory allocated successfully" << std::endl;
        SAFE_DELETE(array);
        std::cout << "Memory freed successfully" << std::endl;
    }
    
    return 0;
}
```

## 3. 函数宏详解

### 3.1 基本函数宏

基本函数宏接受一个或多个参数，并将它们替换到指定的位置：

```cpp
#include <iostream>
#include <algorithm>

// 基本数学函数宏
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))

// 比较函数宏
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

// 交换函数宏
#define SWAP(a, b, type) do { type temp = a; a = b; b = temp; } while(0)

// 调试函数宏
#define DEBUG_PRINT(fmt, ...) std::cout << "[DEBUG] " << fmt << std::endl

int main() {
    int x = -5;
    std::cout << "Square of " << x << ": " << SQUARE(x) << std::endl;
    std::cout << "Cube of " << x << ": " << CUBE(x) << std::endl;
    std::cout << "Absolute value of " << x << ": " << ABS(x) << std::endl;
    
    int a = 10, b = 20;
    std::cout << "Max of " << a << " and " << b << ": " << MAX(a, b) << std::endl;
    std::cout << "Min of " << a << " and " << b << ": " << MIN(a, b) << std::endl;
    std::cout << "Clamp 25 between 10 and 20: " << CLAMP(25, 10, 20) << std::endl;
    
    std::cout << "Before swap: a=" << a << ", b=" << b << std::endl;
    SWAP(a, b, int);
    std::cout << "After swap: a=" << a << ", b=" << b << std::endl;
    
    DEBUG_PRINT("This is a debug message with value");
    
    return 0;
}
```

### 3.2 多参数函数宏

多参数函数宏接受多个参数，提供更复杂的功能：

```cpp
#include <iostream>
#include <string>
#include <cstring>

// 字符串处理宏
#define STRING_EQUAL(s1, s2) (std::strcmp((s1), (s2)) == 0)
#define STRING_COPY(dest, src, size) do { \
    std::strncpy((dest), (src), (size) - 1); \
    (dest)[(size) - 1] = '\0'; \
} while(0)

// 数组操作宏
#define ARRAY_INIT(arr, value) do { \
    for (size_t i = 0; i < ARRAY_SIZE(arr); i++) { \
        (arr)[i] = (value); \
    } \
} while(0)

#define ARRAY_SUM(arr, size, result) do { \
    result = 0; \
    for (size_t i = 0; i < (size); i++) { \
        result += (arr)[i]; \
    } \
} while(0)

#define ARRAY_FIND(arr, size, value, index) do { \
    index = -1; \
    for (size_t i = 0; i < (size); i++) { \
        if ((arr)[i] == (value)) { \
            index = i; \
            break; \
        } \
    } \
} while(0)

// 内存操作宏
#define MEM_ZERO(ptr, size) std::memset((ptr), 0, (size))
#define MEM_COPY(dest, src, size) std::memcpy((dest), (src), (size))

// 时间测量宏
#define TIME_OPERATION(op, result) do { \
    auto start = std::chrono::high_resolution_clock::now(); \
    op; \
    auto end = std::chrono::high_resolution_clock::now(); \
    result = std::chrono::duration<double>(end - start).count(); \
} while(0)

// 数组大小宏（用于多参数宏）
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main() {
    // 测试字符串宏
    const char* str1 = "hello";
    const char* str2 = "hello";
    const char* str3 = "world";
    
    std::cout << "String equal test:" << std::endl;
    std::cout << "  \"" << str1 << "\" == \"" << str2 << "\": " 
              << (STRING_EQUAL(str1, str2) ? "true" : "false") << std::endl;
    std::cout << "  \"" << str1 << "\" == \"" << str3 << "\": " 
              << (STRING_EQUAL(str1, str3) ? "true" : "false") << std::endl;
    
    // 测试数组初始化宏
    int numbers[10];
    ARRAY_INIT(numbers, 42);
    std::cout << "Array initialized with 42:" << std::endl;
    for (int i = 0; i < ARRAY_SIZE(numbers); i++) {
        std::cout << "  numbers[" << i << "] = " << numbers[i] << std::endl;
    }
    
    // 测试数组求和宏
    int values[] = {1, 2, 3, 4, 5};
    int sum;
    ARRAY_SUM(values, ARRAY_SIZE(values), sum);
    std::cout << "Sum of array: " << sum << std::endl;
    
    // 测试数组查找宏
    int target = 3;
    int index;
    ARRAY_FIND(values, ARRAY_SIZE(values), target, index);
    std::cout << "Index of " << target << " in array: " << index << std::endl;
    
    return 0;
}
```

### 3.3 可变参数函数宏

可变参数函数宏（自 C++11 起）允许接受可变数量的参数：

```cpp
#include <iostream>
#include <cstdio>

// 基本可变参数宏
#define PRINT_ARGS(...) std::printf(__VA_ARGS__)
#define DEBUG_LOG(fmt, ...) std::fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) std::fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

// 格式化输出宏
#define PRINT(fmt, ...) std::printf(fmt, ##__VA_ARGS__)
#define FPRINT(file, fmt, ...) std::fprintf(file, fmt, ##__VA_ARGS__)

// 条件输出宏
#define CONDITIONAL_PRINT(condition, fmt, ...) do { \
    if (condition) { \
        std::printf(fmt, ##__VA_ARGS__); \
    } \
} while(0)

// 数组初始化宏（可变参数）
#define INIT_ARRAY(arr, ...) do { \
    int temp[] = {__VA_ARGS__}; \
    size_t size = sizeof(temp) / sizeof(temp[0]); \
    size_t arr_size = sizeof(arr) / sizeof((arr)[0]); \
    size_t copy_size = (size < arr_size) ? size : arr_size; \
    for (size_t i = 0; i < copy_size; i++) { \
        (arr)[i] = temp[i]; \
    } \
} while(0)

int main() {
    // 测试基本可变参数宏
    PRINT_ARGS("Hello, %s! You are %d years old.\n", "Alice", 25);
    DEBUG_LOG("Debug message with value: %d", 42);
    ERROR_LOG("Error occurred: %s", "File not found");
    
    // 测试条件输出宏
    int debug_mode = 1;
    CONDITIONAL_PRINT(debug_mode, "Debug mode is enabled\n");
    CONDITIONAL_PRINT(!debug_mode, "This won't be printed\n");
    
    // 测试数组初始化宏
    int numbers[10];
    INIT_ARRAY(numbers, 1, 2, 3, 4, 5);
    std::cout << "Initialized array: ";
    for (int i = 0; i < 5; i++) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

### 3.4 C++20 VA_OPT 宏

C++20 引入了 `__VA_OPT__` 机制，提供了更灵活的可变参数处理：

```cpp
#include <iostream>
#include <cstdio>

// C++20 VA_OPT 示例
#define SHOW_LIST(...) puts(#__VA_ARGS__)
#define PRINT_WITH_PREFIX(prefix, ...) \
    std::printf("%s", prefix); \
    __VA_OPT__(std::printf(" - ")); \
    std::printf(__VA_ARGS__); \
    std::printf("\n")

#define FUNCTION_CALL(name, ...) \
    std::printf("Calling %s", #name); \
    __VA_OPT__(std::printf(" with args: " #__VA_ARGS__)); \
    std::printf("\n")

#define OPTIONAL_ARGS(...) \
    std::printf("Arguments: "); \
    __VA_OPT__(std::printf(__VA_ARGS__)); \
    __VA_OPT__(std::printf(" ")); \
    std::printf("(end)\n")

int main() {
    // 测试 SHOW_LIST 宏
    SHOW_LIST();            // expands to puts("")
    SHOW_LIST(1, "x", 3);   // expands to puts("1, \"x\", 3")
    
    // 测试 PRINT_WITH_PREFIX 宏
    PRINT_WITH_PREFIX("Info");           // 输出: Info
    PRINT_WITH_PREFIX("Warning", "File not found");  // 输出: Warning - File not found
    
    // 测试 FUNCTION_CALL 宏
    FUNCTION_CALL(initialize);           // 输出: Calling initialize
    FUNCTION_CALL(connect, "localhost", 8080);  // 输出: Calling connect with args: "localhost", 8080
    
    // 测试 OPTIONAL_ARGS 宏
    OPTIONAL_ARGS();                     // 输出: Arguments: (end)
    OPTIONAL_ARGS("arg1", "arg2");       // 输出: Arguments: arg1, arg2 (end)
    
    return 0;
}
```

## 4. 特殊运算符

### 4.1 字符串化运算符 (#)

字符串化运算符 `#` 将宏参数转换为字符串字面量：

```cpp
#include <iostream>
#include <string>

// 基本字符串化
#define STRINGIFY(x) #x
#define PRINT_VAR_NAME(var) std::cout << "Variable name: " << #var << std::endl
#define PRINT_VAR_VALUE(var) std::cout << "Variable " << #var << " = " << var << std::endl

// 调试宏
#define DEBUG_VAR(var) std::cout << "DEBUG: " << #var << " = " << STRINGIFY(var) << std::endl
#define TRACE_FUNCTION(func) std::cout << "TRACE: Entering function " << #func << std::endl

// 条件编译宏
#define COMPILE_TIME_ASSERT(condition) \
    typedef char compile_time_assert_failed[(condition) ? 1 : -1]

int main() {
    // 测试基本字符串化
    int my_variable = 42;
    std::cout << "Stringified value: " << STRINGIFY(my_variable) << std::endl;
    std::cout << "Stringified expression: " << STRINGIFY(2 + 3 * 4) << std::endl;
    
    // 测试变量名字符串化
    PRINT_VAR_NAME(my_variable);
    PRINT_VAR_VALUE(my_variable);
    
    // 测试调试宏
    DEBUG_VAR(my_variable);
    DEBUG_VAR(2 + 3);
    
    // 测试函数跟踪宏
    TRACE_FUNCTION(main);
    TRACE_FUNCTION(calculate_sum);
    
    return 0;
}
```

### 4.2 标记粘贴运算符 (##)

标记粘贴运算符 `##` 将两个标记连接成一个新的标记：

```cpp
#include <iostream>

// 基本标记粘贴
#define CONCAT(a, b) a##b
#define CREATE_VARIABLE(type, name) type var_##name
#define CREATE_FUNCTION(name) void function_##name()

// 自动生成函数名
#define GENERATE_GETTER(type, field) \
    type get_##field() { return field; }

#define GENERATE_SETTER(type, field) \
    void set_##field(type value) { field = value; }

// 自动生成结构体成员
#define DECLARE_MEMBER(type, name) type member_##name;
#define ACCESS_MEMBER(name) member_##name

// 自动生成枚举值
#define ENUM_VALUE(prefix, name) prefix##_##name

// 测试函数声明
CREATE_FUNCTION(test) {
    std::cout << "This is the test function" << std::endl;
}

int main() {
    // 测试基本标记粘贴
    int xy = CONCAT(1, 2);
    std::cout << "Concatenated value: " << xy << std::endl;
    
    // 测试变量创建
    CREATE_VARIABLE(int, counter);
    var_counter = 100;
    std::cout << "Variable value: " << var_counter << std::endl;
    
    // 测试函数调用
    function_test();
    
    // 测试结构体成员
    struct {
        DECLARE_MEMBER(int, id)
        DECLARE_MEMBER(char*, name)
        DECLARE_MEMBER(double, value)
    } my_struct;
    
    my_struct.ACCESS_MEMBER(id) = 1;
    my_struct.ACCESS_MEMBER(name) = (char*)"test";
    my_struct.ACCESS_MEMBER(value) = 3.14;
    
    std::cout << "Struct members:" << std::endl;
    std::cout << "  id: " << my_struct.ACCESS_MEMBER(id) << std::endl;
    std::cout << "  name: " << my_struct.ACCESS_MEMBER(name) << std::endl;
    std::cout << "  value: " << my_struct.ACCESS_MEMBER(value) << std::endl;
    
    // 测试枚举值
    enum Colors {
        ENUM_VALUE(COLOR, RED) = 0xFF0000,
        ENUM_VALUE(COLOR, GREEN) = 0x00FF00,
        ENUM_VALUE(COLOR, BLUE) = 0x0000FF
    };
    
    std::cout << "Colors:" << std::endl;
    std::cout << "  Red: 0x" << std::hex << COLOR_RED << std::dec << std::endl;
    std::cout << "  Green: 0x" << std::hex << COLOR_GREEN << std::dec << std::endl;
    std::cout << "  Blue: 0x" << std::hex << COLOR_BLUE << std::dec << std::endl;
    
    return 0;
}
```

### 4.3 运算符组合使用

字符串化和标记粘贴运算符可以组合使用，创建更强大的宏：

```cpp
#include <iostream>

// 组合使用示例
#define CREATE_GETTER_SETTER(type, field) \
    type get_##field() { return field; } \
    void set_##field(type value) { field = value; } \
    void print_##field() { std::cout << #field " = " << field << std::endl; }

// 自动生成结构体和相关函数
#define DECLARE_STRUCT_MEMBER(type, name) type name;
#define GENERATE_STRUCT_ACCESSORS(type, name) \
    type get_##name(MyStruct* s) { return s->name; } \
    void set_##name(MyStruct* s, type value) { s->name = value; }

// 调试宏组合
#define DEBUG_PRINT_VAR(var) \
    std::cout << "DEBUG: " << #var << " = " << STRINGIFY(var) << std::endl

#define STRINGIFY(x) #x

// 自动生成配置结构
#define CONFIG_FIELD(type, name, default_value) \
    type name;

#define CONFIG_GETTER(type, name) \
    type get_config_##name(Config* c) { return c->name; }

#define CONFIG_SETTER(type, name) \
    void set_config_##name(Config* c, type value) { c->name = value; }

struct MyStruct {
    DECLARE_STRUCT_MEMBER(int, id)
    DECLARE_STRUCT_MEMBER(char*, name)
    DECLARE_STRUCT_MEMBER(double, value)
};

struct Config {
    CONFIG_FIELD(int, max_connections, 100)
    CONFIG_FIELD(char*, server_name, (char*)"localhost")
    CONFIG_FIELD(double, timeout, 30.0)
};

int main() {
    // 测试组合宏
    // 生成访问器函数
    GENERATE_STRUCT_ACCESSORS(int, id)
    GENERATE_STRUCT_ACCESSORS(char*, name)
    GENERATE_STRUCT_ACCESSORS(double, value)
    
    MyStruct s = {1, (char*)"test", 3.14};
    std::cout << "Struct values:" << std::endl;
    std::cout << "  id: " << s.id << std::endl;
    std::cout << "  name: " << s.name << std::endl;
    std::cout << "  value: " << s.value << std::endl;
    
    // 测试配置结构
    // 生成配置访问器
    CONFIG_GETTER(int, max_connections)
    CONFIG_SETTER(int, max_connections)
    
    Config config = {100, (char*)"localhost", 30.0};
    std::cout << "Config values:" << std::endl;
    std::cout << "  max_connections: " << config.max_connections << std::endl;
    
    config.max_connections = 200;
    std::cout << "  max_connections after update: " << config.max_connections << std::endl;
    
    return 0;
}
```

## 5. 宏定义与取消定义

### 5.1 #define 指令

`#define` 指令用于定义宏，有多种使用形式：

```cpp
#include <iostream>

// 1. 简单对象宏
#define BUFFER_SIZE 1024
#define PI 3.14159

// 2. 字符串对象宏
#define PROGRAM_NAME "MyProgram"
#define VERSION "1.0.0"

// 3. 表达式对象宏
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

// 4. 语句对象宏
#define SWAP(a, b, type) do { type temp = a; a = b; b = temp; } while(0)

// 5. 多行宏定义
#define PRINT_HEADER() do { \
    std::cout << "====================" << std::endl; \
    std::cout << "  Program Header    " << std::endl; \
    std::cout << "====================" << std::endl; \
} while(0)

// 6. 条件宏定义
#if defined(DEBUG)
    #define LOG_DEBUG(msg) std::cout << "[DEBUG] " << msg << std::endl
#else
    #define LOG_DEBUG(msg) do {} while(0)
#endif

int main() {
    std::cout << "Program: " << PROGRAM_NAME << " v" << VERSION << std::endl;
    std::cout << "Buffer size: " << BUFFER_SIZE << std::endl;
    std::cout << "PI: " << PI << std::endl;
    
    int a = 10, b = 20;
    std::cout << "Max of " << a << " and " << b << ": " << MAX(a, b) << std::endl;
    std::cout << "Square of " << a << ": " << SQUARE(a) << std::endl;
    
    std::cout << "Before swap: a=" << a << ", b=" << b << std::endl;
    SWAP(a, b, int);
    std::cout << "After swap: a=" << a << ", b=" << b << std::endl;
    
    PRINT_HEADER();
    
    LOG_DEBUG("This is a debug message");
    
    return 0;
}
```

### 5.2 #undef 指令

`#undef` 指令用于取消先前定义的宏：

```cpp
#include <iostream>

#define DEBUG 1
#define MAX_VALUE 100
#define FUNCTION_NAME "main"

int main() {
    std::cout << "Initial values:" << std::endl;
    std::cout << "  DEBUG: " << DEBUG << std::endl;
    std::cout << "  MAX_VALUE: " << MAX_VALUE << std::endl;
    std::cout << "  FUNCTION_NAME: " << FUNCTION_NAME << std::endl;
    
    // 取消定义宏
    #undef DEBUG
    #undef MAX_VALUE
    #undef FUNCTION_NAME
    
    // 重新定义宏
    #define DEBUG 0
    #define MAX_VALUE 200
    #define FUNCTION_NAME "updated_main"
    
    std::cout << "Updated values:" << std::endl;
    std::cout << "  DEBUG: " << DEBUG << std::endl;
    std::cout << "  MAX_VALUE: " << MAX_VALUE << std::endl;
    std::cout << "  FUNCTION_NAME: " << FUNCTION_NAME << std::endl;
    
    // 条件性取消定义
    #ifdef NON_EXISTENT_MACRO
        // 这段代码不会执行
        #undef NON_EXISTENT_MACRO
    #else
        std::cout << "NON_EXISTENT_MACRO is not defined" << std::endl;
    #endif
    
    return 0;
}
```

### 5.3 宏定义的重定义

宏可以在同一翻译单元中重新定义，但必须完全相同：

```cpp
#include <iostream>

// 初始定义
#define VALUE 42
#define FUNCTION(x) ((x) * 2)

int main() {
    std::cout << "Initial VALUE: " << VALUE << std::endl;
    std::cout << "FUNCTION(5): " << FUNCTION(5) << std::endl;
    
    // 重新定义相同的宏（允许）
    #undef VALUE
    #define VALUE 42
    
    #undef FUNCTION
    #define FUNCTION(x) ((x) * 2)
    
    std::cout << "Redefined VALUE: " << VALUE << std::endl;
    std::cout << "Redefined FUNCTION(5): " << FUNCTION(5) << std::endl;
    
    // 尝试定义不同的宏（会导致编译错误）
    /*
    #undef VALUE
    #define VALUE 100  // 如果取消注释，这将是合法的重新定义
    */
    
    return 0;
}
```

## 6. 宏替换规则

### 6.1 替换过程

宏替换是预处理器的一个重要阶段，遵循特定的规则：

```cpp
#include <iostream>

// 演示替换过程
#define A 1
#define B A
#define C B

#define X(Y) Y + 1
#define Z X(2)

int main() {
    // 替换过程演示
    std::cout << "A = " << A << std::endl;  // 直接替换为 1
    std::cout << "B = " << B << std::endl;  // B -> A -> 1
    std::cout << "C = " << C << std::endl;  // C -> B -> A -> 1
    
    std::cout << "Z = " << Z << std::endl;  // Z -> X(2) -> 2 + 1 -> 3
    
    return 0;
}
```

### 6.2 参数替换

函数宏的参数替换遵循特定的规则：

```cpp
#include <iostream>

#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define PRINT_DEBUG(msg) std::cout << "[DEBUG] " << #msg << std::endl

int main() {
    int x = 5;
    // 参数替换: x -> 5
    std::cout << "SQUARE(x) = " << SQUARE(x) << std::endl;  // ((5) * (5)) = 25
    
    int a = 10, b = 20;
    // 参数替换: a -> 10, b -> 20
    std::cout << "MAX(a, b) = " << MAX(a, b) << std::endl;  // ((10) > (20) ? (10) : (20)) = 20
    
    // 字符串化参数替换
    PRINT_DEBUG(Hello World);  // std::cout << "[DEBUG] " << "Hello World" << std::endl;
    
    return 0;
}
```

### 6.3 递归替换

宏替换避免无限递归：

```cpp
#include <iostream>

// 递归替换示例
#define PING PONG
#define PONG PING

#define FOO BAR
#define BAR BAZ
#define BAZ FOO

int main() {
    // 这些不会导致无限递归，因为预处理器会检测并停止
    // std::cout << "PING = " << PING << std::endl;  // 这会导致编译错误
    // std::cout << "FOO = " << FOO << std::endl;    // 这会导致编译错误
    
    // 正确的递归使用示例
    #define VERSION_1 1
    #define VERSION VERSION_1
    
    std::cout << "Version: " << VERSION << std::endl;  // VERSION -> VERSION_1 -> 1
    
    return 0;
}
```

### 6.4 替换顺序

宏替换遵循从左到右的顺序：

```cpp
#include <iostream>

#define FIRST second
#define second third
#define third 3

#define A B
#define B C
#define C 100

int main() {
    // 替换顺序演示
    std::cout << "FIRST = " << FIRST << std::endl;  // FIRST -> second -> third -> 3
    std::cout << "A = " << A << std::endl;          // A -> B -> C -> 100
    
    return 0;
}
```

## 7. 预定义宏

### 7.1 标准预定义宏

C++ 标准定义了一系列预定义宏：

```cpp
#include <iostream>
#include <chrono>

int main() {
    std::cout << "Standard predefined macros:" << std::endl;
    
    // C++ 特定宏
    std::cout << "  __cplusplus: " << __cplusplus << std::endl;
    std::cout << "  __STDC_HOSTED__: " << __STDC_HOSTED__ << std::endl;
    
    // 文件和行信息
    std::cout << "  __FILE__: " << __FILE__ << std::endl;
    std::cout << "  __LINE__: " << __LINE__ << std::endl;
    
    // 编译时间和日期
    std::cout << "  __DATE__: " << __DATE__ << std::endl;
    std::cout << "  __TIME__: " << __TIME__ << std::endl;
    
    // C++17 特定宏
    #if __cplusplus >= 201703L
    std::cout << "  __STDCPP_DEFAULT_NEW_ALIGNMENT__: " << __STDCPP_DEFAULT_NEW_ALIGNMENT__ << std::endl;
    #endif
    
    // C++20 特定宏
    #if __cplusplus >= 202002L
    std::cout << "  __STDCPP_THREADS__: " << __STDCPP_THREADS__ << std::endl;
    #endif
    
    return 0;
}
```

### 7.2 实现特定预定义宏

编译器和实现可能定义额外的预定义宏：

```cpp
#include <iostream>

int main() {
    std::cout << "Implementation-specific predefined macros:" << std::endl;
    
    // 编译器检测
    #ifdef __GNUC__
    std::cout << "  Compiled with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << std::endl;
    #endif
    
    #ifdef __clang__
    std::cout << "  Compiled with Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << std::endl;
    #endif
    
    #ifdef _MSC_VER
    std::cout << "  Compiled with MSVC " << _MSC_VER << std::endl;
    #endif
    
    // 平台检测
    #ifdef __linux__
    std::cout << "  Running on Linux" << std::endl;
    #endif
    
    #ifdef _WIN32
    std::cout << "  Running on Windows" << std::endl;
    #endif
    
    #ifdef __APPLE__
    std::cout << "  Running on macOS" << std::endl;
    #endif
    
    // 架构检测
    #ifdef __x86_64__
    std::cout << "  Running on x86_64 architecture" << std::endl;
    #endif
    
    #ifdef __arm__
    std::cout << "  Running on ARM architecture" << std::endl;
    #endif
    
    return 0;
}
```

### 7.3 预定义宏的使用

预定义宏在实际编程中的应用：

```cpp
#include <iostream>
#include <cstdlib>

// 根据 C++ 标准版本启用特性
#if __cplusplus >= 201103L
    #include <memory>
    #define HAS_CPP11_FEATURES 1
#else
    #define HAS_CPP11_FEATURES 0
#endif

// 根据平台定义不同实现
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
#endif

// 调试宏
#ifdef DEBUG
    #define DBG_PRINT(fmt, ...) std::fprintf(stderr, "[DEBUG %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DBG_PRINT(fmt, ...) do {} while(0)
#endif

[[noreturn]] void fatal_error(const char* msg) {
    std::fprintf(stderr, "Fatal error: %s\n", msg);
    std::exit(EXIT_FAILURE);
}

int main() {
    std::cout << "Program started at " << __DATE__ << " " << __TIME__ << std::endl;
    std::cout << "Compiled from file: " << __FILE__ << std::endl;
    std::cout << "Current line: " << __LINE__ << std::endl;
    
    #if HAS_CPP11_FEATURES
    std::cout << "C++11 features are available" << std::endl;
    #else
    std::cout << "Using legacy C++ features" << std::endl;
    #endif
    
    DBG_PRINT("Debug message with value: %d", 42);
    
    std::cout << "Sleeping for 1 second..." << std::endl;
    SLEEP(1000);
    std::cout << "Awake!" << std::endl;
    
    return 0;
}
```

## 8. 高级宏技巧

### 8.1 宏工厂模式

使用宏创建代码生成工厂：

```cpp
#include <iostream>

// 函数工厂宏
#define FUNCTION_FACTORY(name, operation) \
    int function_##name(int a, int b) { \
        return a operation b; \
    }

// 创建多个函数
FUNCTION_FACTORY(add, +)
FUNCTION_FACTORY(subtract, -)
FUNCTION_FACTORY(multiply, *)
FUNCTION_FACTORY(divide, /)

// 结构体工厂宏
#define STRUCT_FACTORY(type_name, ...) \
    struct type_name { \
        __VA_ARGS__ \
    };

// 创建结构体
STRUCT_FACTORY(Point, int x; int y;)
STRUCT_FACTORY(Rectangle, int width; int height; char* name;)

// 枚举工厂宏
#define ENUM_FACTORY(enum_name, ...) \
    enum enum_name { \
        __VA_ARGS__ \
    };

// 创建枚举
ENUM_FACTORY(Color, RED = 0xFF0000, GREEN = 0x00FF00, BLUE = 0x0000FF)

int main() {
    // 测试函数工厂
    std::cout << "Add: " << function_add(10, 5) << std::endl;
    std::cout << "Subtract: " << function_subtract(10, 5) << std::endl;
    std::cout << "Multiply: " << function_multiply(10, 5) << std::endl;
    std::cout << "Divide: " << function_divide(10, 5) << std::endl;
    
    // 测试结构体工厂
    Point p = {10, 20};
    Rectangle r = {100, 50, (char*)"MyRectangle"};
    
    std::cout << "Point: (" << p.x << ", " << p.y << ")" << std::endl;
    std::cout << "Rectangle: " << r.name << " (" << r.width << " x " << r.height << ")" << std::endl;
    
    // 测试枚举工厂
    Color c = RED;
    std::cout << "Color RED: 0x" << std::hex << c << std::dec << std::endl;
    
    return 0;
}
```

### 8.2 条件宏定义

根据条件定义不同的宏实现：

```cpp
#include <iostream>

// 根据平台定义不同实现
#if defined(_WIN32)
    #define PLATFORM_NAME "Windows"
    #define PATH_SEPARATOR '\\'
#elif defined(__linux__)
    #define PLATFORM_NAME "Linux"
    #define PATH_SEPARATOR '/'
#elif defined(__APPLE__)
    #define PLATFORM_NAME "macOS"
    #define PATH_SEPARATOR '/'
#else
    #define PLATFORM_NAME "Unknown"
    #define PATH_SEPARATOR '/'
#endif

// 根据编译器定义优化
#if defined(__GNUC__)
    #define FORCE_INLINE __attribute__((always_inline)) inline
    #define NO_RETURN [[noreturn]]
#elif defined(_MSC_VER)
    #define FORCE_INLINE __forceinline
    #define NO_RETURN __declspec(noreturn)
#else
    #define FORCE_INLINE inline
    #define NO_RETURN [[noreturn]]
#endif

// 根据调试模式定义
#ifdef DEBUG
    #define ASSERT(condition) \
        do { \
            if (!(condition)) { \
                std::fprintf(stderr, "Assertion failed: %s at %s:%d\n", \
                        #condition, __FILE__, __LINE__); \
                std::abort(); \
            } \
        } while(0)
    #define LOG_DEBUG(fmt, ...) \
        std::fprintf(stderr, "[DEBUG %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define ASSERT(condition) do {} while(0)
    #define LOG_DEBUG(fmt, ...) do {} while(0)
#endif

NO_RETURN void fatal_error(const char* msg) {
    std::fprintf(stderr, "Fatal error: %s\n", msg);
    std::exit(EXIT_FAILURE);
}

int main() {
    std::cout << "Platform: " << PLATFORM_NAME << std::endl;
    std::cout << "Path separator: " << PATH_SEPARATOR << std::endl;
    
    int x = 5;
    ASSERT(x > 0);
    LOG_DEBUG("Variable x = %d", x);
    
    // 如果取消注释下面这行，程序会终止
    // fatal_error("Test fatal error");
    
    return 0;
}
```

### 8.3 宏调试技术

使用宏进行调试和诊断：

```cpp
#include <iostream>
#include <cstdlib>

// 调试级别控制
#define DEBUG_LEVEL 2

#if DEBUG_LEVEL >= 3
    #define TRACE(fmt, ...) std::fprintf(stderr, "[TRACE] " fmt "\n", ##__VA_ARGS__)
#elif DEBUG_LEVEL >= 2
    #define TRACE(fmt, ...) std::fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#elif DEBUG_LEVEL >= 1
    #define TRACE(fmt, ...) std::fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__)
#else
    #define TRACE(fmt, ...) do {} while(0)
#endif

// 函数进入/退出跟踪
#define FUNCTION_ENTER(name) TRACE("Entering function: %s", #name)
#define FUNCTION_EXIT(name) TRACE("Exiting function: %s", #name)

// 变量监视宏
#define WATCH_VAR(name, value) TRACE("Variable %s = %s = %d", #name, #value, (int)(value))

int calculate_sum(int a, int b) {
    FUNCTION_ENTER(calculate_sum);
    WATCH_VAR(a, a);
    WATCH_VAR(b, b);
    
    int result = a + b;
    WATCH_VAR(result, result);
    
    FUNCTION_EXIT(calculate_sum);
    return result;
}

int main() {
    TRACE("Program started");
    
    int x = 10, y = 20;
    int sum = calculate_sum(x, y);
    
    TRACE("Sum of %d and %d is %d", x, y, sum);
    
    TRACE("Program finished");
    
    return 0;
}
```

### 8.4 宏性能优化

使用宏进行性能优化：

```cpp
#include <iostream>
#include <chrono>

// 内联函数宏
#define INLINE_MAX(a, b) ((a) > (b) ? (a) : (b))
#define INLINE_MIN(a, b) ((a) < (b) ? (a) : (b))
#define INLINE_ABS(x) ((x) < 0 ? -(x) : (x))

// 位操作优化宏
#define BIT_SET(x, n) ((x) |= (1UL << (n)))
#define BIT_CLEAR(x, n) ((x) &= ~(1UL << (n)))
#define BIT_TOGGLE(x, n) ((x) ^= (1UL << (n)))
#define BIT_CHECK(x, n) (((x) >> (n)) & 1)

// 循环展开宏
#define UNROLL_LOOP_4(var, start, operation) \
    do { \
        var = start; \
        operation; var++; \
        operation; var++; \
        operation; var++; \
        operation; var++; \
    } while(0)

// 编译时计算宏
#define CONST_POWER_OF_2(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))

// 性能测试宏
#define TIME_OPERATION(op, result) \
    do { \
        auto start = std::chrono::high_resolution_clock::now(); \
        op; \
        auto end = std::chrono::high_resolution_clock::now(); \
        result = std::chrono::duration<double>(end - start).count(); \
    } while(0)

int main() {
    // 测试内联宏性能
    int a = 100, b = 200;
    std::cout << "Max of " << a << " and " << b << ": " << INLINE_MAX(a, b) << std::endl;
    std::cout << "Min of " << a << " and " << b << ": " << INLINE_MIN(a, b) << std::endl;
    std::cout << "Absolute value of " << -42 << ": " << INLINE_ABS(-42) << std::endl;
    
    // 测试位操作宏
    unsigned int flags = 0;
    BIT_SET(flags, 3);
    std::cout << "Bit 3 set: " << BIT_CHECK(flags, 3) << std::endl;
    
    BIT_TOGGLE(flags, 3);
    std::cout << "Bit 3 toggled: " << BIT_CHECK(flags, 3) << std::endl;
    
    // 测试常量计算宏
    std::cout << "Is 16 power of 2: " << (CONST_POWER_OF_2(16) ? "yes" : "no") << std::endl;
    std::cout << "Is 15 power of 2: " << (CONST_POWER_OF_2(15) ? "yes" : "no") << std::endl;
    
    // 测试性能
    double time_taken;
    int sum = 0;
    
    TIME_OPERATION(
        for (int i = 0; i < 1000000; i++) {
            sum += INLINE_MAX(i, 500000);
        },
        time_taken
    );
    
    std::cout << "Time taken for 1M operations: " << time_taken << " seconds" << std::endl;
    std::cout << "Final sum: " << sum << std::endl;
    
    return 0;
}
```

## 9. C++ 特有特性

### 9.1 模板与宏结合

C++ 中宏与模板的结合使用：

```cpp
#include <iostream>
#include <type_traits>

// 模板特化与宏结合
#define SPECIALIZE_TEMPLATE(type, value) \
    template<> \
    struct DefaultValue<type> { \
        static const type value() { return value; } \
    };

template<typename T>
struct DefaultValue {
    static T value() { return T{}; }
};

// 使用宏特化模板
SPECIALIZE_TEMPLATE(int, -1)
SPECIALIZE_TEMPLATE(double, -1.0)
SPECIALIZE_TEMPLATE(char, '\0')

// 模板元编程与宏
#define DEFINE_TYPE_TRAIT(trait_name, condition) \
    template<typename T> \
    struct trait_name { \
        static constexpr bool value = condition; \
    };

DEFINE_TYPE_TRAIT(IsIntegral, std::is_integral<T>::value)
DEFINE_TYPE_TRAIT(IsFloatingPoint, std::is_floating_point<T>::value)

int main() {
    std::cout << "Default values:" << std::endl;
    std::cout << "  int: " << DefaultValue<int>::value() << std::endl;
    std::cout << "  double: " << DefaultValue<double>::value() << std::endl;
    std::cout << "  char: " << DefaultValue<char>::value() << std::endl;
    
    std::cout << "Type traits:" << std::endl;
    std::cout << "  int is integral: " << IsIntegral<int>::value << std::endl;
    std::cout << "  double is floating point: " << IsFloatingPoint<double>::value << std::endl;
    
    return 0;
}
```

### 9.2 C++ 标准库保护

C++ 对标准库名称的保护机制：

```cpp
#include <iostream>
#include <vector>
#include <string>

// 错误示例：尝试重新定义标准库名称（会导致编译错误）
/*
#define vector MyVector  // 错误：不能重新定义标准库名称
#define string MyString  // 错误：不能重新定义标准库名称
*/

// 正确做法：使用命名空间
namespace MyNamespace {
    class vector {
        // 自定义 vector 实现
    };
    
    class string {
        // 自定义 string 实现
    };
}

int main() {
    // 使用标准库
    std::vector<int> std_vec = {1, 2, 3};
    std::string std_str = "Hello";
    
    std::cout << "Standard library vector size: " << std_vec.size() << std::endl;
    std::cout << "Standard library string: " << std_str << std::endl;
    
    // 使用自定义命名空间
    MyNamespace::vector my_vec;
    MyNamespace::string my_str;
    
    std::cout << "Custom namespace objects created successfully" << std::endl;
    
    return 0;
}
```

### 9.3 C++ 语言特性检测

使用宏检测 C++ 语言特性：

```cpp
#include <iostream>

// C++ 特性检测宏
#define HAS_FEATURE(feature) __has_cpp_attribute(feature)

// C++11 特性检测
#if __cplusplus >= 201103L
    #define HAS_CPP11 1
    #define HAS_AUTO 1
    #define HAS_LAMBDA 1
#else
    #define HAS_CPP11 0
    #define HAS_AUTO 0
    #define HAS_LAMBDA 0
#endif

// C++14 特性检测
#if __cplusplus >= 201402L
    #define HAS_CPP14 1
    #define HAS_VARIABLE_TEMPLATES 1
#else
    #define HAS_CPP14 0
    #define HAS_VARIABLE_TEMPLATES 0
#endif

// C++17 特性检测
#if __cplusplus >= 201703L
    #define HAS_CPP17 1
    #define HAS_IF_CONSTEXPR 1
#else
    #define HAS_CPP17 0
    #define HAS_IF_CONSTEXPR 0
#endif

// C++20 特性检测
#if __cplusplus >= 202002L
    #define HAS_CPP20 1
    #define HAS_CONCEPTS 1
#else
    #define HAS_CPP20 0
    #define HAS_CONCEPTS 0
#endif

int main() {
    std::cout << "C++ Language Feature Detection:" << std::endl;
    std::cout << "  C++11 support: " << (HAS_CPP11 ? "Yes" : "No") << std::endl;
    std::cout << "  auto keyword: " << (HAS_AUTO ? "Yes" : "No") << std::endl;
    std::cout << "  lambda expressions: " << (HAS_LAMBDA ? "Yes" : "No") << std::endl;
    
    std::cout << "  C++14 support: " << (HAS_CPP14 ? "Yes" : "No") << std::endl;
    std::cout << "  variable templates: " << (HAS_VARIABLE_TEMPLATES ? "Yes" : "No") << std::endl;
    
    std::cout << "  C++17 support: " << (HAS_CPP17 ? "Yes" : "No") << std::endl;
    std::cout << "  if constexpr: " << (HAS_IF_CONSTEXPR ? "Yes" : "No") << std::endl;
    
    std::cout << "  C++20 support: " << (HAS_CPP20 ? "Yes" : "No") << std::endl;
    std::cout << "  concepts: " << (HAS_CONCEPTS ? "Yes" : "No") << std::endl;
    
    // 根据支持的特性使用不同代码
    #if HAS_AUTO
    auto value = 42;
    std::cout << "Using auto: value = " << value << std::endl;
    #endif
    
    #if HAS_LAMBDA
    auto lambda = [](int x) { return x * 2; };
    std::cout << "Using lambda: lambda(5) = " << lambda(5) << std::endl;
    #endif
    
    return 0;
}
```

## 10. 最佳实践

### 10.1 宏定义规范

遵循规范的宏定义方式：

```cpp
#include <iostream>

// 1. 宏名使用大写字母和下划线
#define MAX_BUFFER_SIZE 1024
#define DEFAULT_TIMEOUT 30
#define ENABLE_DEBUG_LOGGING 1

// 2. 复杂表达式使用括号包围
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define IS_EVEN(x) (((x) % 2) == 0)

// 3. 多行宏使用 do-while(0) 包装
#define SAFE_DELETE(ptr) \
    do { \
        if (ptr != nullptr) { \
            delete ptr; \
            ptr = nullptr; \
        } \
    } while(0)

#define LOG_ERROR(msg) \
    do { \
        std::fprintf(stderr, "[ERROR %s:%d] %s\n", __FILE__, __LINE__, msg); \
    } while(0)

// 4. 条件编译宏使用标准命名
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) \
        std::fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif

int main() {
    std::cout << "MAX_BUFFER_SIZE: " << MAX_BUFFER_SIZE << std::endl;
    std::cout << "Square of 5: " << SQUARE(5) << std::endl;
    std::cout << "Max of 10 and 20: " << MAX(10, 20) << std::endl;
    std::cout << "Is 4 even: " << (IS_EVEN(4) ? "yes" : "no") << std::endl;
    
    int* ptr = new int(42);
    SAFE_DELETE(ptr);
    std::cout << "Memory safely deleted" << std::endl;
    
    LOG_ERROR("This is an error message");
    DEBUG_PRINT("This is a debug message with value %d", 42);
    
    return 0;
}
```

### 10.2 宏使用模式

推荐的宏使用模式：

```cpp
#include <iostream>
#include <memory>

// 1. 配置宏模式
#define CONFIG_MAX_CONNECTIONS 100
#define CONFIG_BUFFER_SIZE 4096
#define CONFIG_ENABLE_SSL 1

// 2. 平台抽象模式
#if defined(_WIN32)
    #define PLATFORM_WINDOWS 1
    #define SLEEP_FUNC(ms) Sleep(ms)
#elif defined(__unix__)
    #define PLATFORM_UNIX 1
    #define SLEEP_FUNC(ms) usleep((ms) * 1000)
#endif

// 3. 功能检测模式
#if __cplusplus >= 201103L
    #define HAS_CPP11_FEATURES 1
#else
    #define HAS_CPP11_FEATURES 0
#endif

// 4. 调试模式
#ifdef DEBUG
    #define DBG_MODE 1
    #define DBG_ASSERT(condition) \
        do { \
            if (!(condition)) { \
                std::fprintf(stderr, "Assertion failed: %s\n", #condition); \
                std::abort(); \
            } \
        } while(0)
#else
    #define DBG_MODE 0
    #define DBG_ASSERT(condition) do {} while(0)
#endif

// 5. 错误处理模式
#define RETURN_ON_ERROR(condition, error_code) \
    do { \
        if (condition) { \
            return error_code; \
        } \
    } while(0)

#define GOTO_ON_ERROR(condition, label) \
    do { \
        if (condition) { \
            goto label; \
        } \
    } while(0)

int process_data(int* data, size_t size) {
    RETURN_ON_ERROR(data == nullptr, -1);
    RETURN_ON_ERROR(size == 0, -2);
    
    DBG_ASSERT(size > 0);
    DBG_PRINT("Processing %zu elements", size);
    
    for (size_t i = 0; i < size; i++) {
        data[i] *= 2;
    }
    
    return 0;
}

int main() {
    std::cout << "Configuration:" << std::endl;
    std::cout << "  Max connections: " << CONFIG_MAX_CONNECTIONS << std::endl;
    std::cout << "  Buffer size: " << CONFIG_BUFFER_SIZE << std::endl;
    std::cout << "  SSL enabled: " << (CONFIG_ENABLE_SSL ? "yes" : "no") << std::endl;
    
    std::cout << "Platform: " << 
           #ifdef PLATFORM_WINDOWS
           "Windows"
           #elif PLATFORM_UNIX
           "Unix"
           #else
           "Unknown"
           #endif
           << std::endl;
    
    std::cout << "C++11 features: " << (HAS_CPP11_FEATURES ? "available" : "not available") << std::endl;
    std::cout << "Debug mode: " << (DBG_MODE ? "enabled" : "disabled") << std::endl;
    
    int data[] = {1, 2, 3, 4, 5};
    int result = process_data(data, 5);
    std::cout << "Process result: " << result << std::endl;
    
    return 0;
}
```

### 10.3 宏维护策略

有效的宏维护策略：

```cpp
#include <iostream>

// 1. 版本兼容性维护
#if __cplusplus >= 202002L
    #define MODERN_CPP_FEATURES 1
    #define CPP20_CONCEPTS 1
#elif __cplusplus >= 201703L
    #define MODERN_CPP_FEATURES 1
    #define CPP20_CONCEPTS 0
#else
    #define MODERN_CPP_FEATURES 0
    #define CPP20_CONCEPTS 0
#endif

// 2. 弃用警告维护
#if defined(OLD_API) && !defined(SUPPRESS_DEPRECATED_WARNINGS)
    #warning "OLD_API is deprecated and will be removed in future versions"
#endif

// 3. 配置验证维护
#if defined(ENABLE_FEATURE_A) && defined(ENABLE_FEATURE_B)
    #if ENABLE_FEATURE_A && !ENABLE_FEATURE_B
    #error "Feature A requires Feature B to be enabled"
    #endif
#endif

// 4. 条件性功能维护
#if defined(__GNUC__) && (__GNUC__ >= 4)
    #define GCC_MODERN_FEATURES 1
    #define GCC_ATTRIBUTE_UNUSED __attribute__((unused))
#else
    #define GCC_MODERN_FEATURES 0
    #define GCC_ATTRIBUTE_UNUSED
#endif

// 5. 文档化宏维护
/*
 * FEATURE_ADVANCED_MATH: 启用高级数学功能
 * 默认值: 1 (启用)
 * 依赖: 需要 C++11 或更高版本
 */
#define FEATURE_ADVANCED_MATH 1

/*
 * MAX_BUFFER_SIZE: 设置最大缓冲区大小
 * 默认值: 8192 字节
 * 范围: 1024-65536
 */
#define MAX_BUFFER_SIZE 8192

int main() {
    std::cout << "Modern C++ features: " << (MODERN_CPP_FEATURES ? "available" : "not available") << std::endl;
    std::cout << "C++20 concepts: " << (CPP20_CONCEPTS ? "yes" : "no") << std::endl;
    std::cout << "GCC modern features: " << (GCC_MODERN_FEATURES ? "available" : "not available") << std::endl;
    std::cout << "Advanced math feature: " << (FEATURE_ADVANCED_MATH ? "enabled" : "disabled") << std::endl;
    std::cout << "Max buffer size: " << MAX_BUFFER_SIZE << std::endl;
    
    // 使用 GCC 特定属性
    int unused_variable GCC_ATTRIBUTE_UNUSED = 42;
    std::cout << "GCC attribute test completed" << std::endl;
    
    return 0;
}
```

### 10.4 常见错误避免

识别和避免宏的常见错误：

```cpp
#include <iostream>

// 错误1: 参数未加括号
// #define SQUARE_WRONG(x) x * x  // 错误
#define SQUARE_CORRECT(x) ((x) * (x))  // 正确

// 错误2: 表达式未加括号
// #define MAX_WRONG(a, b) a > b ? a : b  // 错误
#define MAX_CORRECT(a, b) ((a) > (b) ? (a) : (b))  // 正确

// 错误3: 宏展开副作用
#define INCREMENT_WRONG(x) ((x)++)  // 错误：多次展开会有副作用
#define INCREMENT_CORRECT(x) ((x) + 1)  // 正确

// 错误4: 多语句宏未包装
// #define SWAP_WRONG(a, b, type) type temp = a; a = b; b = temp;  // 错误
#define SWAP_CORRECT(a, b, type) \
    do { \
        type temp = a; \
        a = b; \
        b = temp; \
    } while(0)  // 正确

// 错误5: 递归宏定义
// #define RECURSIVE PING
// #define PING RECURSIVE  // 错误：会导致问题

int main() {
    // 测试正确的宏使用
    int x = 5;
    std::cout << "Square of " << x << ": " << SQUARE_CORRECT(x) << std::endl;
    std::cout << "Square of 2+3: " << SQUARE_CORRECT(2 + 3) << std::endl;  // 应该是 25，不是 11
    
    int a = 10, b = 20;
    std::cout << "Max of " << a << " and " << b << ": " << MAX_CORRECT(a, b) << std::endl;
    std::cout << "Max of 5+5 and 3*7: " << MAX_CORRECT(5 + 5, 3 * 7) << std::endl;  // 应该是 21，不是 5
    
    int y = 10;
    std::cout << "Increment " << y << ": " << INCREMENT_CORRECT(y) << std::endl;  // 不会改变 y 的值
    
    std::cout << "Before swap: a=" << a << ", b=" << b << std::endl;
    SWAP_CORRECT(a, b, int);
    std::cout << "After swap: a=" << a << ", b=" << b << std::endl;
    
    return 0;
}
```

## 11. 标准参考

### 11.1 C++ 标准版本

替换文本宏相关标准条款：

- C++23 标准 (ISO/IEC 14882:2024): 15.10.4 宏替换
- C++20 标准 (ISO/IEC 14882:2020): 15.10.3 宏替换
- C++17 标准 (ISO/IEC 14882:2017): 16.3 宏替换
- C++14 标准 (ISO/IEC 14882:2014): 16.3 宏替换
- C++11 标准 (ISO/IEC 14882:2011): 16.3 宏替换
- C++98/C++03 标准 (ISO/IEC 14882:1998/2003): 16.3 宏替换

### 11.2 相关标准条款

- 15.1 翻译阶段
- 15.10 预处理指令
- 15.10.3 宏替换
- 15.10.8 预定义宏名
- 15.10.9 预定义的宏名称

### 11.3 缺陷报告

相关缺陷报告：

**CWG 2908 (C++98)**：

- 已发布的行为：目前尚不清楚 `__LINE__` 是否扩展到现在物理行号或当前逻辑行号
- 正确的行为：扩展到当前物理行号

**LWG 294 (C++98)**：

- 已发布的行为：包含标准库标题的翻译单元可以包含定义在其他标准库标头中声明的名称宏
- 正确的行为：禁止

**P2621R2 (C++23)**：

- 已发布的行为：不允许使用通用字符名称通过标记串联形成
- 正确的行为：允许

## 12. 总结

### 12.1 核心要点

C++ 替换文本宏是 C++ 预处理器的核心功能之一，提供了以下关键能力：

- 文本替换：在编译前替换源代码中的标识符
- 常量定义：定义程序中使用的常量值
- 代码简化：简化复杂表达式的书写
- 条件编译：配合条件编译指令实现功能开关
- 代码生成：通过宏生成重复的代码结构
- 平台适配：根据平台定义不同的实现
- 调试支持：在调试和发布版本中包含不同代码
- 性能优化：内联展开以提高性能

### 12.2 使用建议

- 规范命名：宏名使用大写字母和下划线
- 括号保护：复杂表达式和参数使用括号包围
- 避免副作用：宏参数避免有副作用的表达式
- 多语句包装：多语句宏使用 do-while(0) 包装
- 文档注释：为复杂宏添加详细注释
- 适度使用：避免过度使用宏导致代码难以理解
- 测试验证：充分测试宏在各种情况下的行为
- 标准库保护：避免重新定义标准库名称

### 12.3 发展趋势

替换文本宏在 C++ 语言的发展中持续演进：

- C++98/C++03：建立了基本的宏替换框架
- C++11：增加了可变参数宏和 `__VA_ARGS__`
- C++14：保持了 C++11 的宏替换特性
- C++17：保持了 C++14 的宏替换特性
- C++20：新增了 `__VA_OPT__` 机制，提供了更灵活的可变参数处理
- C++23：进一步完善了宏替换功能

随着 C++ 语言的不断发展，宏替换功能变得更加强大和灵活，为开发者提供了更好的代码生成和条件编译支持。C++20 引入的 `__VA_OPT__` 机制进一步增强了可变参数宏的能力，使得宏定义更加灵活和强大。同时，C++ 也加强了对标准库名称的保护，避免宏定义与标准库冲突。
