# C 替换文本宏详解

  [1. 替换文本宏基本概念](#1-替换文本宏基本概念)
  
  [2. 对象宏详解](#2-对象宏详解)
  
  [3. 函数宏详解](#3-函数宏详解)
  
  [4. 特殊运算符](#4-特殊运算符)
  
  [5. 宏定义与取消定义](#5-宏定义与取消定义)
  
  [6. 宏替换规则](#6-宏替换规则)
  
  [7. 预定义宏](#7-预定义宏)
  
  [8. 高级宏技巧](#8-高级宏技巧)
  
  [9. 最佳实践](#9-最佳实践)
  
  [10. 标准参考](#10-标准参考)
  
  [11. 总结](#11-总结)

## 1. 替换文本宏基本概念

### 1.1 什么是替换文本宏

替换文本宏（Text Macro Replacement）是 C 预处理器提供的一种文本替换机制，允许程序员定义标识符与替换文本之间的映射关系。在预处理阶段，预处理器会将源代码中所有匹配的标识符替换为对应的替换文本。

宏定义使用 `#define` 指令创建，有两种基本形式：

- 对象宏（Object-like Macros）：类似常量的宏定义
- 函数宏（Function-like Macros）：类似函数调用的宏定义

```c
#include <stdio.h>

// 对象宏示例
#define PI 3.14159
#define MAX_BUFFER_SIZE 1024
#define DEBUG_MODE 1

// 函数宏示例
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main() {
    printf("PI value: %f\n", PI);
    printf("Buffer size: %d\n", MAX_BUFFER_SIZE);
    
    int x = 5;
    printf("Square of %d: %d\n", x, SQUARE(x));
    printf("Max of 10 and 20: %d\n", MAX(10, 20));
    
    #if DEBUG_MODE
    printf("Debug mode is enabled\n");
    #endif
    
    return 0;
}
```

### 1.2 替换文本宏的作用

替换文本宏在 C 编程中发挥着重要作用：

- 常量定义：定义程序中使用的常量值
- 代码简化：简化复杂表达式的书写
- 条件编译：配合条件编译指令实现功能开关
- 代码生成：通过宏生成重复的代码结构
- 平台适配：根据平台定义不同的实现
- 调试支持：在调试和发布版本中包含不同代码

### 1.3 替换文本宏的类型

C 语言中的宏主要分为两类：

#### 1.3.1 对象宏（Object-like Macros）

对象宏类似于常量，将标识符替换为指定的文本：

```c
#include <stdio.h>

#define BUFFER_SIZE 1024
#define VERSION "1.0.0"
#define ENABLE_DEBUG 1
#define NULL_POINTER ((void*)0)

int main() {
    char buffer[BUFFER_SIZE];
    printf("Version: %s\n", VERSION);
    
    #if ENABLE_DEBUG
    printf("Debug information\n");
    #endif
    
    void* ptr = NULL_POINTER;
    printf("Null pointer: %p\n", ptr);
    
    return 0;
}
```

#### 1.3.2 函数宏（Function-like Macros）

函数宏类似于函数调用，可以接受参数并进行替换：

```c
#include <stdio.h>

#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SWAP(a, b, type) do { type temp = a; a = b; b = temp; } while(0)
#define PRINT_DEBUG(msg) do { printf("[DEBUG] %s\n", msg); } while(0)

int main() {
    int x = 5;
    printf("Square of %d: %d\n", x, SQUARE(x));
    
    int a = 10, b = 20;
    printf("Max of %d and %d: %d\n", a, b, MAX(a, b));
    
    printf("Before swap: a=%d, b=%d\n", a, b);
    SWAP(a, b, int);
    printf("After swap: a=%d, b=%d\n", a, b);
    
    PRINT_DEBUG("This is a debug message");
    
    return 0;
}
```

## 2. 对象宏详解

### 2.1 基本对象宏

基本对象宏是最简单的宏形式，将标识符替换为简单的文本：

```c
#include <stdio.h>

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
    printf("Program: %s v%s\n", PROGRAM_NAME, VERSION_STRING);
    printf("Count: %d\n", COUNT);
    
    if (FLAG) {
        printf("Flag is set\n");
    }
    
    #if ENABLE_FEATURE
    printf("Feature is enabled\n");
    #else
    printf("Feature is disabled\n");
    #endif
    
    printf("Two PI: %f\n", TWO_PI);
    printf("1 KB = %d bytes\n", KILOBYTE);
    
    return 0;
}
```

### 2.2 常量对象宏

常量对象宏用于定义程序中使用的各种常量：

```c
#include <stdio.h>

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
    printf("Mathematical constants:\n");
    printf("  PI = %f\n", PI);
    printf("  E = %f\n", E);
    printf("  SQRT2 = %f\n", SQRT2);
    
    printf("Physical constants:\n");
    printf("  Speed of light = %d m/s\n", SPEED_OF_LIGHT);
    printf("  Gravity = %f m/s^2\n", GRAVITY);
    
    printf("Computer science constants:\n");
    printf("  Bits per byte = %d\n", BITS_PER_BYTE);
    printf("  1 KB = %d bytes\n", BYTES_PER_KB);
    printf("  1 MB = %d bytes\n", BYTES_PER_MB);
    
    printf("Colors:\n");
    printf("  Red = 0x%06X\n", COLOR_RED);
    printf("  Green = 0x%06X\n", COLOR_GREEN);
    printf("  Blue = 0x%06X\n", COLOR_BLUE);
    
    return 0;
}
```

### 2.3 字符串对象宏

字符串对象宏用于定义程序中使用的字符串常量：

```c
#include <stdio.h>

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
    printf("Application Information:\n");
    printf("  Name: %s\n", APP_NAME);
    printf("  Version: %s\n", APP_VERSION);
    printf("  Author: %s\n", APP_AUTHOR);
    printf("  %s\n", APP_COPYRIGHT);
    
    printf("File paths:\n");
    printf("  Config: %s\n", CONFIG_FILE);
    printf("  Log: %s\n", LOG_FILE);
    printf("  Temp: %s\n", TEMP_DIR);
    
    printf("Error messages:\n");
    printf("  Invalid input: %s\n", ERROR_INVALID_INPUT);
    printf("  File not found: %s\n", ERROR_FILE_NOT_FOUND);
    printf("  Permission denied: %s\n", ERROR_PERMISSION_DENIED);
    
    printf("Database connection:\n");
    printf("  Host: %s:%d\n", DB_HOST, DB_PORT);
    printf("  Database: %s\n", DB_NAME);
    printf("  User: %s\n", DB_USER);
    
    return 0;
}
```

### 2.4 复杂表达式对象宏

复杂表达式对象宏用于定义复杂的表达式或语句：

```c
#include <stdio.h>
#include <stdlib.h>

// 内存分配宏
#define SAFE_MALLOC(ptr, size) do { \
    ptr = malloc(size); \
    if (ptr == NULL) { \
        fprintf(stderr, "Memory allocation failed\n"); \
        exit(EXIT_FAILURE); \
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
#define SAFE_FREE(ptr) do { \
    if (ptr != NULL) { \
        free(ptr); \
        ptr = NULL; \
    } \
} while(0)

int main() {
    // 测试数组大小宏
    int numbers[] = {1, 2, 3, 4, 5};
    printf("Array size: %zu\n", ARRAY_SIZE(numbers));
    
    // 测试位操作宏
    unsigned int value = 0;
    SET_BIT(value, 3);
    printf("After setting bit 3: %u\n", value);
    printf("Bit 3 is set: %d\n", CHECK_BIT(value, 3));
    
    TOGGLE_BIT(value, 3);
    printf("After toggling bit 3: %u\n", value);
    printf("Bit 3 is set: %d\n", CHECK_BIT(value, 3));
    
    // 测试循环宏
    printf("Forward loop: ");
    FOR_EACH(i, 0, 5) {
        printf("%d ", i);
    }
    printf("\n");
    
    printf("Reverse loop: ");
    REVERSE_FOR_EACH(i, 0, 5) {
        printf("%d ", i);
    }
    printf("\n");
    
    // 测试内存分配宏
    int* array;
    SAFE_MALLOC(array, 10 * sizeof(int));
    printf("Memory allocated successfully\n");
    
    // 测试安全释放宏
    SAFE_FREE(array);
    printf("Memory freed successfully\n");
    
    return 0;
}
```

## 3. 函数宏详解

### 3.1 基本函数宏

基本函数宏接受一个或多个参数，并将它们替换到指定的位置：

```c
#include <stdio.h>

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
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)

int main() {
    int x = -5;
    printf("Square of %d: %d\n", x, SQUARE(x));
    printf("Cube of %d: %d\n", x, CUBE(x));
    printf("Absolute value of %d: %d\n", x, ABS(x));
    
    int a = 10, b = 20;
    printf("Max of %d and %d: %d\n", a, b, MAX(a, b));
    printf("Min of %d and %d: %d\n", a, b, MIN(a, b));
    printf("Clamp 25 between 10 and 20: %d\n", CLAMP(25, 10, 20));
    
    printf("Before swap: a=%d, b=%d\n", a, b);
    SWAP(a, b, int);
    printf("After swap: a=%d, b=%d\n", a, b);
    
    DEBUG_PRINT("This is a debug message with value %d", 42);
    
    return 0;
}
```

### 3.2 多参数函数宏

多参数函数宏接受多个参数，提供更复杂的功能：

```c
#include <stdio.h>
#include <string.h>

// 字符串处理宏
#define STRING_EQUAL(s1, s2) (strcmp((s1), (s2)) == 0)
#define STRING_COPY(dest, src, size) do { \
    strncpy((dest), (src), (size) - 1); \
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
#define MEM_ZERO(ptr, size) memset((ptr), 0, (size))
#define MEM_COPY(dest, src, size) memcpy((dest), (src), (size))

// 时间测量宏
#define TIME_OPERATION(op, result) do { \
    clock_t start = clock(); \
    op; \
    clock_t end = clock(); \
    result = ((double)(end - start)) / CLOCKS_PER_SEC; \
} while(0)

// 数组大小宏（用于多参数宏）
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main() {
    // 测试字符串宏
    char str1[] = "hello";
    char str2[] = "hello";
    char str3[] = "world";
    
    printf("String equal test:\n");
    printf("  \"%s\" == \"%s\": %s\n", str1, str2, STRING_EQUAL(str1, str2) ? "true" : "false");
    printf("  \"%s\" == \"%s\": %s\n", str1, str3, STRING_EQUAL(str1, str3) ? "true" : "false");
    
    // 测试数组初始化宏
    int numbers[10];
    ARRAY_INIT(numbers, 42);
    printf("Array initialized with 42:\n");
    for (int i = 0; i < ARRAY_SIZE(numbers); i++) {
        printf("  numbers[%d] = %d\n", i, numbers[i]);
    }
    
    // 测试数组求和宏
    int values[] = {1, 2, 3, 4, 5};
    int sum;
    ARRAY_SUM(values, ARRAY_SIZE(values), sum);
    printf("Sum of array: %d\n", sum);
    
    // 测试数组查找宏
    int target = 3;
    int index;
    ARRAY_FIND(values, ARRAY_SIZE(values), target, index);
    printf("Index of %d in array: %d\n", target, index);
    
    return 0;
}
```

### 3.3 可变参数函数宏

可变参数函数宏（自 C99 起）允许接受可变数量的参数：

```c
#include <stdio.h>

// 基本可变参数宏
#define PRINT_ARGS(...) printf(__VA_ARGS__)
#define DEBUG_LOG(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

// 格式化输出宏
#define PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define FPRINT(file, fmt, ...) fprintf(file, fmt, ##__VA_ARGS__)

// 条件输出宏
#define CONDITIONAL_PRINT(condition, fmt, ...) do { \
    if (condition) { \
        printf(fmt, ##__VA_ARGS__); \
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
    printf("Initialized array: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    return 0;
}
```

### 3.4 C23 VA_OPT 宏

C23 引入了 `__VA_OPT__` 机制，提供了更灵活的可变参数处理：

```c
#include <stdio.h>

// C23 VA_OPT 示例
#define SHOW_LIST(...) puts(#__VA_ARGS__)
#define PRINT_WITH_PREFIX(prefix, ...) \
    printf("%s", prefix); \
    __VA_OPT__(printf(" - ")); \
    printf(__VA_ARGS__); \
    printf("\n")

#define FUNCTION_CALL(name, ...) \
    printf("Calling %s", #name); \
    __VA_OPT__(printf(" with args: " #__VA_ARGS__)); \
    printf("\n")

#define OPTIONAL_ARGS(...) \
    printf("Arguments: "); \
    __VA_OPT__(printf(__VA_ARGS__)); \
    __VA_OPT__(printf(" ")); \
    printf("(end)\n")

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

```c
#include <stdio.h>

// 基本字符串化
#define STRINGIFY(x) #x
#define PRINT_VAR_NAME(var) printf("Variable name: %s\n", #var)
#define PRINT_VAR_VALUE(var) printf("Variable %s = %d\n", #var, var)

// 调试宏
#define DEBUG_VAR(var) printf("DEBUG: %s = %s\n", #var, STRINGIFY(var))
#define TRACE_FUNCTION(func) printf("TRACE: Entering function %s\n", #func)

// 条件编译宏
#define COMPILE_TIME_ASSERT(condition) \
    typedef char compile_time_assert_failed[(condition) ? 1 : -1]

int main() {
    // 测试基本字符串化
    int my_variable = 42;
    printf("Stringified value: %s\n", STRINGIFY(my_variable));
    printf("Stringified expression: %s\n", STRINGIFY(2 + 3 * 4));
    
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

```c
#include <stdio.h>

// 基本标记粘贴
#define CONCAT(a, b) a##b
#define CREATE_VARIABLE(type, name) type var_##name
#define CREATE_FUNCTION(name) void function_##name(void)

// 自动生成函数名
#define GENERATE_GETTER(type, field) \
    type get_##field(void) { return field; }

#define GENERATE_SETTER(type, field) \
    void set_##field(type value) { field = value; }

// 自动生成结构体成员
#define DECLARE_MEMBER(type, name) type member_##name;
#define ACCESS_MEMBER(name) member_##name

// 自动生成枚举值
#define ENUM_VALUE(prefix, name) prefix##_##name

// 测试函数声明
CREATE_FUNCTION(test) {
    printf("This is the test function\n");
}

int main() {
    // 测试基本标记粘贴
    int xy = CONCAT(1, 2);
    printf("Concatenated value: %d\n", xy);
    
    // 测试变量创建
    CREATE_VARIABLE(int, counter);
    var_counter = 100;
    printf("Variable value: %d\n", var_counter);
    
    // 测试函数调用
    function_test();
    
    // 测试结构体成员
    struct {
        DECLARE_MEMBER(int, id)
        DECLARE_MEMBER(char*, name)
        DECLARE_MEMBER(double, value)
    } my_struct;
    
    my_struct.ACCESS_MEMBER(id) = 1;
    my_struct.ACCESS_MEMBER(name) = "test";
    my_struct.ACCESS_MEMBER(value) = 3.14;
    
    printf("Struct members:\n");
    printf("  id: %d\n", my_struct.ACCESS_MEMBER(id));
    printf("  name: %s\n", my_struct.ACCESS_MEMBER(name));
    printf("  value: %f\n", my_struct.ACCESS_MEMBER(value));
    
    // 测试枚举值
    enum Colors {
        ENUM_VALUE(COLOR, RED) = 0xFF0000,
        ENUM_VALUE(COLOR, GREEN) = 0x00FF00,
        ENUM_VALUE(COLOR, BLUE) = 0x0000FF
    };
    
    printf("Colors:\n");
    printf("  Red: 0x%06X\n", COLOR_RED);
    printf("  Green: 0x%06X\n", COLOR_GREEN);
    printf("  Blue: 0x%06X\n", COLOR_BLUE);
    
    return 0;
}
```

### 4.3 运算符组合使用

字符串化和标记粘贴运算符可以组合使用，创建更强大的宏：

```c
#include <stdio.h>

// 组合使用示例
#define CREATE_GETTER_SETTER(type, field) \
    type get_##field(void) { return field; } \
    void set_##field(type value) { field = value; } \
    void print_##field(void) { printf(#field " = %" #type "\n", field); }

// 自动生成结构体和相关函数
#define DECLARE_STRUCT_MEMBER(type, name) type name;
#define GENERATE_STRUCT_ACCESSORS(type, name) \
    type get_##name(struct MyStruct* s) { return s->name; } \
    void set_##name(struct MyStruct* s, type value) { s->name = value; }

// 调试宏组合
#define DEBUG_PRINT_VAR(var) \
    printf("DEBUG: %s = %s\n", #var, STRINGIFY(var))

#define STRINGIFY(x) #x

// 自动生成配置结构
#define CONFIG_FIELD(type, name, default_value) \
    type name;

#define CONFIG_GETTER(type, name) \
    type get_config_##name(struct Config* c) { return c->name; }

#define CONFIG_SETTER(type, name) \
    void set_config_##name(struct Config* c, type value) { c->name = value; }

int main() {
    // 测试组合宏
    struct MyStruct {
        DECLARE_STRUCT_MEMBER(int, id)
        DECLARE_STRUCT_MEMBER(char*, name)
        DECLARE_STRUCT_MEMBER(double, value)
    };
    
    // 生成访问器函数
    GENERATE_STRUCT_ACCESSORS(int, id)
    GENERATE_STRUCT_ACCESSORS(char*, name)
    GENERATE_STRUCT_ACCESSORS(double, value)
    
    struct MyStruct s = {1, "test", 3.14};
    printf("Struct values:\n");
    printf("  id: %d\n", get_id(&s));
    printf("  name: %s\n", get_name(&s));
    printf("  value: %f\n", get_value(&s));
    
    // 测试配置结构
    struct Config {
        CONFIG_FIELD(int, max_connections, 100)
        CONFIG_FIELD(char*, server_name, "localhost")
        CONFIG_FIELD(double, timeout, 30.0)
    };
    
    // 生成配置访问器
    CONFIG_GETTER(int, max_connections)
    CONFIG_SETTER(int, max_connections)
    
    struct Config config = {100, "localhost", 30.0};
    printf("Config values:\n");
    printf("  max_connections: %d\n", get_config_max_connections(&config));
    
    set_config_max_connections(&config, 200);
    printf("  max_connections after update: %d\n", get_config_max_connections(&config));
    
    return 0;
}
```

## 5. 宏定义与取消定义

### 5.1 #define 指令

`#define` 指令用于定义宏，有多种使用形式：

```c
#include <stdio.h>

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
    printf("====================\n"); \
    printf("  Program Header    \n"); \
    printf("====================\n"); \
} while(0)

// 6. 条件宏定义
#if defined(DEBUG)
    #define LOG_DEBUG(msg) printf("[DEBUG] %s\n", msg)
#else
    #define LOG_DEBUG(msg) do {} while(0)
#endif

int main() {
    printf("Program: %s v%s\n", PROGRAM_NAME, VERSION);
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("PI: %f\n", PI);
    
    int a = 10, b = 20;
    printf("Max of %d and %d: %d\n", a, b, MAX(a, b));
    printf("Square of %d: %d\n", a, SQUARE(a));
    
    printf("Before swap: a=%d, b=%d\n", a, b);
    SWAP(a, b, int);
    printf("After swap: a=%d, b=%d\n", a, b);
    
    PRINT_HEADER();
    
    LOG_DEBUG("This is a debug message");
    
    return 0;
}
```

### 5.2 #undef 指令

`#undef` 指令用于取消先前定义的宏：

```c
#include <stdio.h>

#define DEBUG 1
#define MAX_VALUE 100
#define FUNCTION_NAME "main"

int main() {
    printf("Initial values:\n");
    printf("  DEBUG: %d\n", DEBUG);
    printf("  MAX_VALUE: %d\n", MAX_VALUE);
    printf("  FUNCTION_NAME: %s\n", FUNCTION_NAME);
    
    // 取消定义宏
    #undef DEBUG
    #undef MAX_VALUE
    #undef FUNCTION_NAME
    
    // 重新定义宏
    #define DEBUG 0
    #define MAX_VALUE 200
    #define FUNCTION_NAME "updated_main"
    
    printf("Updated values:\n");
    printf("  DEBUG: %d\n", DEBUG);
    printf("  MAX_VALUE: %d\n", MAX_VALUE);
    printf("  FUNCTION_NAME: %s\n", FUNCTION_NAME);
    
    // 条件性取消定义
    #ifdef NON_EXISTENT_MACRO
        // 这段代码不会执行
        #undef NON_EXISTENT_MACRO
    #else
        printf("NON_EXISTENT_MACRO is not defined\n");
    #endif
    
    return 0;
}
```

### 5.3 宏定义的重定义

宏可以在同一翻译单元中重新定义，但必须完全相同：

```c
#include <stdio.h>

// 初始定义
#define VALUE 42
#define FUNCTION(x) ((x) * 2)

int main() {
    printf("Initial VALUE: %d\n", VALUE);
    printf("FUNCTION(5): %d\n", FUNCTION(5));
    
    // 重新定义相同的宏（允许）
    #undef VALUE
    #define VALUE 42
    
    #undef FUNCTION
    #define FUNCTION(x) ((x) * 2)
    
    printf("Redefined VALUE: %d\n", VALUE);
    printf("Redefined FUNCTION(5): %d\n", FUNCTION(5));
    
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

```c
#include <stdio.h>

// 演示替换过程
#define A 1
#define B A
#define C B

#define X(Y) Y + 1
#define Z X(2)

int main() {
    // 替换过程演示
    printf("A = %d\n", A);  // 直接替换为 1
    printf("B = %d\n", B);  // B -> A -> 1
    printf("C = %d\n", C);  // C -> B -> A -> 1
    
    printf("Z = %d\n", Z);  // Z -> X(2) -> 2 + 1 -> 3
    
    return 0;
}
```

### 6.2 参数替换

函数宏的参数替换遵循特定的规则：

```c
#include <stdio.h>

#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define PRINT_DEBUG(msg) printf("[DEBUG] %s\n", #msg)

int main() {
    int x = 5;
    // 参数替换: x -> 5
    printf("SQUARE(x) = %d\n", SQUARE(x));  // ((5) * (5)) = 25
    
    int a = 10, b = 20;
    // 参数替换: a -> 10, b -> 20
    printf("MAX(a, b) = %d\n", MAX(a, b));  // ((10) > (20) ? (10) : (20)) = 20
    
    // 字符串化参数替换
    PRINT_DEBUG(Hello World);  // printf("[DEBUG] %s\n", "Hello World");
    
    return 0;
}
```

### 6.3 递归替换

宏替换避免无限递归：

```c
#include <stdio.h>

// 递归替换示例
#define PING PONG
#define PONG PING

#define FOO BAR
#define BAR BAZ
#define BAZ FOO

int main() {
    // 这些不会导致无限递归，因为预处理器会检测并停止
    // printf("PING = %d\n", PING);  // 这会导致编译错误
    // printf("FOO = %d\n", FOO);    // 这会导致编译错误
    
    // 正确的递归使用示例
    #define VERSION_1 1
    #define VERSION VERSION_1
    
    printf("Version: %d\n", VERSION);  // VERSION -> VERSION_1 -> 1
    
    return 0;
}
```

### 6.4 替换顺序

宏替换遵循从左到右的顺序：

```c
#include <stdio.h>

#define FIRST second
#define second third
#define third 3

#define A B
#define B C
#define C 100

int main() {
    // 替换顺序演示
    printf("FIRST = %d\n", FIRST);  // FIRST -> second -> third -> 3
    printf("A = %d\n", A);          // A -> B -> C -> 100
    
    return 0;
}
```

## 7. 预定义宏

### 7.1 标准预定义宏

C 标准定义了一系列预定义宏：

```c
#include <stdio.h>
#include <time.h>

int main() {
    printf("Standard predefined macros:\n");
    
    // 基本标准宏
    printf("  __STDC__: %d\n", __STDC__);
    printf("  __STDC_VERSION__: %ld\n", __STDC_VERSION__);
    printf("  __STDC_HOSTED__: %d\n", __STDC_HOSTED__);
    
    // 文件和行信息
    printf("  __FILE__: %s\n", __FILE__);
    printf("  __LINE__: %d\n", __LINE__);
    
    // 编译时间和日期
    printf("  __DATE__: %s\n", __DATE__);
    printf("  __TIME__: %s\n", __TIME__);
    
    // C23 特定宏
    #if __STDC_VERSION__ >= 202311L
    printf("  __STDC_UTF_16__: %d\n", __STDC_UTF_16__);
    printf("  __STDC_UTF_32__: %d\n", __STDC_UTF_32__);
    #endif
    
    return 0;
}
```

### 7.2 实现特定预定义宏

编译器和实现可能定义额外的预定义宏：

```c
#include <stdio.h>

int main() {
    printf("Implementation-specific predefined macros:\n");
    
    // 编译器检测
    #ifdef __GNUC__
    printf("  Compiled with GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #endif
    
    #ifdef __clang__
    printf("  Compiled with Clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
    #endif
    
    #ifdef _MSC_VER
    printf("  Compiled with MSVC %d\n", _MSC_VER);
    #endif
    
    // 平台检测
    #ifdef __linux__
    printf("  Running on Linux\n");
    #endif
    
    #ifdef _WIN32
    printf("  Running on Windows\n");
    #endif
    
    #ifdef __APPLE__
    printf("  Running on macOS\n");
    #endif
    
    // 架构检测
    #ifdef __x86_64__
    printf("  Running on x86_64 architecture\n");
    #endif
    
    #ifdef __arm__
    printf("  Running on ARM architecture\n");
    #endif
    
    return 0;
}
```

### 7.3 预定义宏的使用

预定义宏在实际编程中的应用：

```c
#include <stdio.h>
#include <stdlib.h>

// 根据标准版本启用特性
#if __STDC_VERSION__ >= 201112L
    #include <stdalign.h>
    #define HAS_C11_FEATURES 1
#else
    #define HAS_C11_FEATURES 0
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
    #define DBG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DBG_PRINT(fmt, ...) do {} while(0)
#endif

int main() {
    printf("Program started at %s %s\n", __DATE__, __TIME__);
    printf("Compiled from file: %s\n", __FILE__);
    printf("Current line: %d\n", __LINE__);
    
    #if HAS_C11_FEATURES
    printf("C11 features are available\n");
    #else
    printf("Using legacy C features\n");
    #endif
    
    DBG_PRINT("Debug message with value: %d", 42);
    
    printf("Sleeping for 1 second...\n");
    SLEEP(1000);
    printf("Awake!\n");
    
    return 0;
}
```

## 8. 高级宏技巧

### 8.1 宏工厂模式

使用宏创建代码生成工厂：

```c
#include <stdio.h>

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
    typedef struct { \
        __VA_ARGS__ \
    } type_name;

// 创建结构体
STRUCT_FACTORY(Point, int x; int y;)
STRUCT_FACTORY(Rectangle, int width; int height; char* name;)

// 枚举工厂宏
#define ENUM_FACTORY(enum_name, ...) \
    typedef enum { \
        __VA_ARGS__ \
    } enum_name;

// 创建枚举
ENUM_FACTORY(Color, RED = 0xFF0000, GREEN = 0x00FF00, BLUE = 0x0000FF)

int main() {
    // 测试函数工厂
    printf("Add: %d\n", function_add(10, 5));
    printf("Subtract: %d\n", function_subtract(10, 5));
    printf("Multiply: %d\n", function_multiply(10, 5));
    printf("Divide: %d\n", function_divide(10, 5));
    
    // 测试结构体工厂
    Point p = {10, 20};
    Rectangle r = {100, 50, "MyRectangle"};
    
    printf("Point: (%d, %d)\n", p.x, p.y);
    printf("Rectangle: %s (%d x %d)\n", r.name, r.width, r.height);
    
    // 测试枚举工厂
    Color c = RED;
    printf("Color RED: 0x%06X\n", c);
    
    return 0;
}
```

### 8.2 条件宏定义

根据条件定义不同的宏实现：

```c
#include <stdio.h>

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
    #define NO_RETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
    #define FORCE_INLINE __forceinline
    #define NO_RETURN __declspec(noreturn)
#else
    #define FORCE_INLINE inline
    #define NO_RETURN
#endif

// 根据调试模式定义
#ifdef DEBUG
    #define ASSERT(condition) \
        do { \
            if (!(condition)) { \
                fprintf(stderr, "Assertion failed: %s at %s:%d\n", \
                        #condition, __FILE__, __LINE__); \
                abort(); \
            } \
        } while(0)
    #define LOG_DEBUG(fmt, ...) \
        fprintf(stderr, "[DEBUG %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define ASSERT(condition) do {} while(0)
    #define LOG_DEBUG(fmt, ...) do {} while(0)
#endif

NO_RETURN void fatal_error(const char* msg) {
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(EXIT_FAILURE);
}

int main() {
    printf("Platform: %s\n", PLATFORM_NAME);
    printf("Path separator: %c\n", PATH_SEPARATOR);
    
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

```c
#include <stdio.h>
#include <stdlib.h>

// 调试级别控制
#define DEBUG_LEVEL 2

#if DEBUG_LEVEL >= 3
    #define TRACE(fmt, ...) fprintf(stderr, "[TRACE] " fmt "\n", ##__VA_ARGS__)
#elif DEBUG_LEVEL >= 2
    #define TRACE(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#elif DEBUG_LEVEL >= 1
    #define TRACE(fmt, ...) fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__)
#else
    #define TRACE(fmt, ...) do {} while(0)
#endif

// 函数进入/退出跟踪
#define FUNCTION_ENTER(name) TRACE("Entering function: %s", #name)
#define FUNCTION_EXIT(name) TRACE("Exiting function: %s", #name)

// 变量监视宏
#define WATCH_VAR(name, value) TRACE("Variable %s = %s = %d", #name, #value, (int)(value))

// 内存分配跟踪
#define MALLOC_TRACE(ptr, size) TRACE("Allocated %zu bytes at %p", (size_t)(size), (void*)(ptr))
#define FREE_TRACE(ptr) TRACE("Freed memory at %p", (void*)(ptr))

void* debug_malloc(size_t size) {
    void* ptr = malloc(size);
    MALLOC_TRACE(ptr, size);
    return ptr;
}

void debug_free(void* ptr) {
    FREE_TRACE(ptr);
    free(ptr);
}

#define DEBUG_MALLOC(size) debug_malloc(size)
#define DEBUG_FREE(ptr) debug_free(ptr)

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
    
    // 测试内存跟踪
    int* array = (int*)DEBUG_MALLOC(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        array[i] = i;
    }
    DEBUG_FREE(array);
    
    TRACE("Program finished");
    
    return 0;
}
```

### 8.4 宏性能优化

使用宏进行性能优化：

```c
#include <stdio.h>
#include <time.h>

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
        clock_t start = clock(); \
        op; \
        clock_t end = clock(); \
        result = ((double)(end - start)) / CLOCKS_PER_SEC; \
    } while(0)

int main() {
    // 测试内联宏性能
    int a = 100, b = 200;
    printf("Max of %d and %d: %d\n", a, b, INLINE_MAX(a, b));
    printf("Min of %d and %d: %d\n", a, b, INLINE_MIN(a, b));
    printf("Absolute value of %d: %d\n", -42, INLINE_ABS(-42));
    
    // 测试位操作宏
    unsigned int flags = 0;
    BIT_SET(flags, 3);
    printf("Bit 3 set: %u\n", BIT_CHECK(flags, 3));
    
    BIT_TOGGLE(flags, 3);
    printf("Bit 3 toggled: %u\n", BIT_CHECK(flags, 3));
    
    // 测试常量计算宏
    printf("Is 16 power of 2: %s\n", CONST_POWER_OF_2(16) ? "yes" : "no");
    printf("Is 15 power of 2: %s\n", CONST_POWER_OF_2(15) ? "yes" : "no");
    
    // 测试性能
    double time_taken;
    int sum = 0;
    
    TIME_OPERATION(
        for (int i = 0; i < 1000000; i++) {
            sum += INLINE_MAX(i, 500000);
        },
        time_taken
    );
    
    printf("Time taken for 1M operations: %f seconds\n", time_taken);
    printf("Final sum: %d\n", sum);
    
    return 0;
}
```

## 9. 最佳实践

### 9.1 宏定义规范

遵循规范的宏定义方式：

```c
#include <stdio.h>

// 1. 宏名使用大写字母和下划线
#define MAX_BUFFER_SIZE 1024
#define DEFAULT_TIMEOUT 30
#define ENABLE_DEBUG_LOGGING 1

// 2. 复杂表达式使用括号包围
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define IS_EVEN(x) (((x) % 2) == 0)

// 3. 多行宏使用 do-while(0) 包装
#define SAFE_FREE(ptr) \
    do { \
        if (ptr != NULL) { \
            free(ptr); \
            ptr = NULL; \
        } \
    } while(0)

#define LOG_ERROR(msg) \
    do { \
        fprintf(stderr, "[ERROR %s:%d] %s\n", __FILE__, __LINE__, msg); \
    } while(0)

// 4. 条件编译宏使用标准命名
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) \
        fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif

int main() {
    printf("MAX_BUFFER_SIZE: %d\n", MAX_BUFFER_SIZE);
    printf("Square of 5: %d\n", SQUARE(5));
    printf("Max of 10 and 20: %d\n", MAX(10, 20));
    printf("Is 4 even: %s\n", IS_EVEN(4) ? "yes" : "no");
    
    char* ptr = malloc(100);
    SAFE_FREE(ptr);
    printf("Memory safely freed\n");
    
    LOG_ERROR("This is an error message");
    DEBUG_PRINT("This is a debug message with value %d", 42);
    
    return 0;
}
```

### 9.2 宏使用模式

推荐的宏使用模式：

```c
#include <stdio.h>
#include <stdlib.h>

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
#if __STDC_VERSION__ >= 201112L
    #define HAS_C11_FEATURES 1
#else
    #define HAS_C11_FEATURES 0
#endif

// 4. 调试模式
#ifdef DEBUG
    #define DBG_MODE 1
    #define DBG_ASSERT(condition) \
        do { \
            if (!(condition)) { \
                fprintf(stderr, "Assertion failed: %s\n", #condition); \
                abort(); \
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
    RETURN_ON_ERROR(data == NULL, -1);
    RETURN_ON_ERROR(size == 0, -2);
    
    DBG_ASSERT(size > 0);
    DBG_PRINT("Processing %zu elements", size);
    
    for (size_t i = 0; i < size; i++) {
        data[i] *= 2;
    }
    
    return 0;
}

int main() {
    printf("Configuration:\n");
    printf("  Max connections: %d\n", CONFIG_MAX_CONNECTIONS);
    printf("  Buffer size: %d\n", CONFIG_BUFFER_SIZE);
    printf("  SSL enabled: %s\n", CONFIG_ENABLE_SSL ? "yes" : "no");
    
    printf("Platform: %s\n", 
           #ifdef PLATFORM_WINDOWS
           "Windows"
           #elif PLATFORM_UNIX
           "Unix"
           #else
           "Unknown"
           #endif
    );
    
    printf("C11 features: %s\n", HAS_C11_FEATURES ? "available" : "not available");
    printf("Debug mode: %s\n", DBG_MODE ? "enabled" : "disabled");
    
    int data[] = {1, 2, 3, 4, 5};
    int result = process_data(data, 5);
    printf("Process result: %d\n", result);
    
    return 0;
}
```

### 9.3 宏维护策略

有效的宏维护策略：

```c
#include <stdio.h>

// 1. 版本兼容性维护
#if __STDC_VERSION__ >= 202311L
    #define MODERN_C_FEATURES 1
    #define UTF16_SUPPORT __STDC_UTF_16__
#elif __STDC_VERSION__ >= 201112L
    #define MODERN_C_FEATURES 1
    #define UTF16_SUPPORT 0
#else
    #define MODERN_C_FEATURES 0
    #define UTF16_SUPPORT 0
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
 * 依赖: 需要 C99 或更高版本
 */
#define FEATURE_ADVANCED_MATH 1

/*
 * MAX_BUFFER_SIZE: 设置最大缓冲区大小
 * 默认值: 8192 字节
 * 范围: 1024-65536
 */
#define MAX_BUFFER_SIZE 8192

int main() {
    printf("Modern C features: %s\n", MODERN_C_FEATURES ? "available" : "not available");
    printf("UTF-16 support: %s\n", UTF16_SUPPORT ? "yes" : "no");
    printf("GCC modern features: %s\n", GCC_MODERN_FEATURES ? "available" : "not available");
    printf("Advanced math feature: %s\n", FEATURE_ADVANCED_MATH ? "enabled" : "disabled");
    printf("Max buffer size: %d\n", MAX_BUFFER_SIZE);
    
    // 使用 GCC 特定属性
    int unused_variable GCC_ATTRIBUTE_UNUSED = 42;
    printf("GCC attribute test completed\n");
    
    return 0;
}
```

### 9.4 常见错误避免

识别和避免宏的常见错误：

```c
#include <stdio.h>

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
    printf("Square of %d: %d\n", x, SQUARE_CORRECT(x));
    printf("Square of 2+3: %d\n", SQUARE_CORRECT(2 + 3));  // 应该是 25，不是 11
    
    int a = 10, b = 20;
    printf("Max of %d and %d: %d\n", a, b, MAX_CORRECT(a, b));
    printf("Max of 5+5 and 3*7: %d\n", MAX_CORRECT(5 + 5, 3 * 7));  // 应该是 21，不是 5
    
    int y = 10;
    printf("Increment %d: %d\n", y, INCREMENT_CORRECT(y));  // 不会改变 y 的值
    
    printf("Before swap: a=%d, b=%d\n", a, b);
    SWAP_CORRECT(a, b, int);
    printf("After swap: a=%d, b=%d\n", a, b);
    
    return 0;
}
```

## 10. 标准参考

### 10.1 C 标准版本

替换文本宏相关标准条款：

- C23 标准 (ISO/IEC 9899:2024): 6.10.4 宏替换 (第 187-184 页)
- C17 标准 (ISO/IEC 9899:2018): 6.10.3 宏替换 (第 121-126 页)
- C11 标准 (ISO/IEC 9899:2011): 6.10.3 宏替换 (第 166-173 页)
- C99 标准 (ISO/IEC 9899:1999): 6.10.3 宏替换 (第 151-158 页)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.8.3 宏替换

### 10.2 相关标准条款

- 5.1.1.2 翻译阶段
- 6.10 预处理指令
- 6.10.3 宏替换
- 6.10.8 预定义宏名
- 6.10.9 预定义的宏名称 (C23)

### 10.3 缺陷报告

相关缺陷报告：

**DR 321 (C99)**：

- 已发布的行为：目前尚不清楚 L'x' == 'x' 是否始终成立在基本字符集中
- 正确的行为：`__STDC_MB_MIGHT_NEQ_WC__` 为此目的添加的

## 11. 总结

### 11.1 核心要点

替换文本宏是 C 预处理器的核心功能之一，提供了以下关键能力：

- 文本替换：在编译前替换源代码中的标识符
- 常量定义：定义程序中使用的常量值
- 代码简化：简化复杂表达式的书写
- 条件编译：配合条件编译指令实现功能开关
- 代码生成：通过宏生成重复的代码结构
- 平台适配：根据平台定义不同的实现

### 11.2 使用建议

- 规范命名：宏名使用大写字母和下划线
- 括号保护：复杂表达式和参数使用括号包围
- 避免副作用：宏参数避免有副作用的表达式
- 多语句包装：多语句宏使用 do-while(0) 包装
- 文档注释：为复杂宏添加详细注释
- 适度使用：避免过度使用宏导致代码难以理解
- 测试验证：充分测试宏在各种情况下的行为

### 11.3 发展趋势

替换文本宏在 C 语言的发展中持续演进：

- C89/C90：建立了基本的宏替换框架
- C99：增加了可变参数宏和 `__VA_ARGS__`
- C11：保持了 C99 的宏替换特性
- C17：保持了 C11 的宏替换特性
- C23：新增了 `__VA_OPT__` 机制，提供了更灵活的可变参数处理

随着 C 语言的不断发展，宏替换功能变得更加强大和灵活，为开发者提供了更好的代码生成和条件编译支持。C23 引入的 `__VA_OPT__` 机制进一步增强了可变参数宏的能力，使得宏定义更加灵活和强大。
