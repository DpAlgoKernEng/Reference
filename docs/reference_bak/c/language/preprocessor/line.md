# C 文件名和行信息详解

[1. C 文件名和行信息基本概念](#1-c-文件名和行信息基本概念)

[2. #line 指令语法详解](#2-line-指令语法详解)

[3. 预定义宏 __LINE__ 和 __FILE__](#3-预定义宏-__line__-和-__file__)

[4. #line 指令应用实践](#4-line-指令应用实践)

[5. 代码生成工具集成](#5-代码生成工具集成)

[6. 调试和错误报告优化](#6-调试和错误报告优化)

[7. 最佳实践](#7-最佳实践)

[8. 标准参考](#8-标准参考)

[9. 总结](#9-总结)

## 1. C 文件名和行信息基本概念

### 1.1 什么是文件名和行信息控制

文件名和行信息控制是指通过 `#line` 指令来修改预处理器中的当前行号和文件名信息。这是 C 语言预处理器提供的一个特殊功能，主要用于：

- 代码生成工具：从其他语言生成 C 代码时保持原始源文件的行号信息
- 调试信息优化：改善调试器和错误报告中的文件位置信息
- 宏展开跟踪：在复杂的宏展开中提供准确的位置信息
- 条件编译管理：在条件编译块中维护正确的行号信息

### 1.2 #line 指令的核心作用

`#line` 指令的主要作用是修改预处理器维护的两个重要状态：

```c
// 基本示例
#line 100
// 从这里开始，__LINE__ 将从 100 开始计数

#line 200 "original_file.c"
// 从这里开始，__LINE__ 从 200 开始，__FILE__ 变为 "original_file.c"
```

### 1.3 预处理器状态管理

预处理器维护的两个关键状态变量：

```c
// 预处理器内部状态示例
// 假设当前状态：
// 当前行号: 42
// 当前文件名: "main.c"

printf("当前行号: %d\n", __LINE__);  // 输出: 当前行号: 43
printf("当前文件: %s\n", __FILE__);  // 输出: 当前文件: main.c

#line 1000 "generated_code.c"
// 状态更新为：
// 当前行号: 1000
// 当前文件名: "generated_code.c"

printf("更新后行号: %d\n", __LINE__);  // 输出: 更新后行号: 1001
printf("更新后文件: %s\n", __FILE__);  // 输出: 更新后文件: generated_code.c
```

## 2. #line 指令语法详解

### 2.1 基本语法形式

`#line` 指令有两种语法形式：

```c
// 形式 1：仅修改行号
#line lineno

// 形式 2：同时修改行号和文件名
#line lineno "filename"
```

### 2.2 语法参数详解

#### 2.2.1 行号参数 (lineno)

行号参数的要求和限制：

```c
// 合法的行号示例
#line 1          // 最小有效行号
#line 100        // 普通行号
#line 32767      // C89/C90 最大行号
#line 2147483647 // C99 及以后的最大行号

// 非法的行号示例
#line 0          // 未定义行为（行号必须 >= 1）
#line -1         // 未定义行为（负数）
#line abc        // 编译错误（非数字）
#line 0x10       // 可能被接受，但始终解释为十进制

// 宏展开的行号
#define CURRENT_LINE __LINE__
#define SET_LINE_TO(x) #line x

#line 500
printf("当前行号: %d\n", __LINE__);  // 输出: 当前行号: 501

#line CURRENT_LINE  // 宏展开后可能产生意外结果
printf("宏展开行号: %d\n", __LINE__);
```

#### 2.2.2 文件名参数 (filename)

文件名参数的处理规则：

```c
// 合法的文件名示例
#line 100 "main.c"
#line 200 "path/to/file.h"
#line 300 "file with spaces.c"
#line 400 "file\"with\"quotes.c"  // 转义引号

// 特殊字符处理
#line 500 "file\nwith\nnewlines.c"  // 换行符可能被处理
#line 600 "file\twith\ttabs.c"      // 制表符可能被处理

// 宏中的文件名
#define ORIGINAL_FILE "original_source.c"
#define GENERATED_FILE "generated_code.c"

#line 100 ORIGINAL_FILE
// 等效于 #line 100 "original_source.c"

#line 200 GENERATED_FILE
// 等效于 #line 200 "generated_code.c"
```

### 2.3 语法转换规则

`#line` 指令的处理过程：

```c
// 1. 宏展开阶段
#define LINE_NUMBER 1000
#define FILE_NAME "generated.c"

#line LINE_NUMBER FILE_NAME
// 预处理器首先展开宏：
// #line 1000 "generated.c"

// 2. 行号计算规则
#line 100
// __LINE__ 从 100 开始
printf("Line 1: %d\n", __LINE__);  // 输出: Line 1: 101
printf("Line 2: %d\n", __LINE__);  // 输出: Line 2: 102

// 3. 文件名继承规则
// 在文件 "main.c" 中
printf("File 1: %s\n", __FILE__);  // 输出: File 1: main.c

#line 200 "new_file.c"
printf("File 2: %s\n", __FILE__);  // 输出: File 2: new_file.c

#line 300  // 仅修改行号，文件名保持不变
printf("File 3: %s\n", __FILE__);  // 输出: File 3: new_file.c
```

## 3. 预定义宏 __LINE__ 和  __FILE__

### 3.1 __LINE__ 宏详解

`__LINE__` 宏提供当前源代码行号信息：

```c
// __LINE__ 宏的基本使用
#include <stdio.h>

void demonstrate_line_macro(void) {
    printf("当前行号: %d\n", __LINE__);  // 输出当前行号
    
#line 1000
    printf("修改后行号: %d\n", __LINE__);  // 输出: 修改后行号: 1001
    
    // 在宏中的使用
    #define PRINT_LINE() printf("宏中的行号: %d\n", __LINE__)
    PRINT_LINE();  // 输出宏定义处的行号，不是调用处
    
    // 条件编译中的行号
    #if __LINE__ > 1005
        printf("条件编译行号检查: %d\n", __LINE__);
    #endif
}

// 行号计算示例
void line_number_calculation(void) {
    printf("起始行号: %d\n", __LINE__);  // 假设为 25
    
#line 50
    printf("重置后行号: %d\n", __LINE__);  // 51
    
    // 后续行号基于重置值计算
    printf("后续行号 1: %d\n", __LINE__);  // 52
    printf("后续行号 2: %d\n", __LINE__);  // 53
}
```

### 3.2 __FILE__ 宏详解

`__FILE__` 宏提供当前源文件名信息：

```c
// __FILE__ 宏的基本使用
#include <stdio.h>

void demonstrate_file_macro(void) {
    printf("当前文件: %s\n", __FILE__);  // 输出当前文件名
    
#line 100 "custom_file.c"
    printf("修改后文件: %s\n", __FILE__);  // 输出: custom_file.c
    
    // 在包含文件中的使用
    #include "included_file.h"  // __FILE__ 在 included_file.h 中会显示实际文件名
}

// 文件名处理示例
void file_name_handling(void) {
    printf("原始文件: %s\n", __FILE__);
    
    // 路径处理
    const char* filename = __FILE__;
    const char* basename = filename;
    
    // 简单的文件名提取（实际应用中可能需要更复杂的处理）
    for (const char* p = filename; *p; p++) {
        if (*p == '/' || *p == '\\') {
            basename = p + 1;
        }
    }
    
    printf("文件基名: %s\n", basename);
}

// 条件编译中的文件检查
void conditional_compilation_with_file(void) {
#if defined(__FILE__) && __LINE__ > 0
    printf("文件和行号信息可用: %s:%d\n", __FILE__, __LINE__);
#endif

    // 基于文件名的条件编译
    #if defined(DEBUG) && __FILE__[0] == 'd'  // 简化的调试文件检查
        printf("调试模式下的文件: %s\n", __FILE__);
    #endif
}
```

### 3.3 组合使用示例

`__LINE__` 和 `__FILE__` 的组合使用：

```c
// debug_macros.h - 调试宏定义
#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include <stdio.h>

// 基本调试宏
#define DEBUG_PRINT(fmt, ...) \
    fprintf(stderr, "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define ERROR_PRINT(fmt, ...) \
    fprintf(stderr, "ERROR [%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define WARNING_PRINT(fmt, ...) \
    fprintf(stderr, "WARNING [%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// 带条件的调试宏
#ifdef DEBUG
    #define DBG_PRINT(fmt, ...) DEBUG_PRINT(fmt, ##__VA_ARGS__)
#else
    #define DBG_PRINT(fmt, ...) do {} while(0)
#endif

// 函数进入/退出跟踪
#define FUNCTION_ENTER() \
    DBG_PRINT("进入函数 %s", __func__)

#define FUNCTION_EXIT() \
    DBG_PRINT("退出函数 %s", __func__)

// 断言宏
#define MY_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            ERROR_PRINT("断言失败: %s", #condition); \
            abort(); \
        } \
    } while(0)

#endif

// main.c
#include <stdio.h>
#include <stdlib.h>
#include "debug_macros.h"

int main(void) {
    DEBUG_PRINT("程序开始执行");
    
    int x = 5;
    DBG_PRINT("变量 x 的值: %d", x);
    
    MY_ASSERT(x > 0);
    
    if (x < 0) {
        ERROR_PRINT("x 应该是非负数，但得到 %d", x);
    } else if (x == 0) {
        WARNING_PRINT("x 为零，这可能是意外的");
    }
    
    DEBUG_PRINT("程序执行完成");
    return 0;
}
```

## 4. #line 指令应用实践

### 4.1 错误报告优化

使用 `#line` 改善错误报告的准确性：

```c
// error_reporting.h
#ifndef ERROR_REPORTING_H
#define ERROR_REPORTING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 错误报告结构
typedef struct {
    const char* file;
    int line;
    const char* message;
    int error_code;
} ErrorInfo;

// 错误报告宏
#define REPORT_ERROR(msg) \
    do { \
        fprintf(stderr, "错误 [%s:%d]: %s\n", __FILE__, __LINE__, msg); \
        exit(EXIT_FAILURE); \
    } while(0)

#define REPORT_WARNING(msg) \
    do { \
        fprintf(stderr, "警告 [%s:%d]: %s\n", __FILE__, __LINE__, msg); \
    } while(0)

// 带错误代码的报告
#define REPORT_ERROR_CODE(code, msg) \
    do { \
        fprintf(stderr, "错误 %d [%s:%d]: %s\n", code, __FILE__, __LINE__, msg); \
        exit(code); \
    } while(0)

// 条件错误报告
#define CHECK_CONDITION(condition, msg) \
    do { \
        if (!(condition)) { \
            REPORT_ERROR(msg); \
        } \
    } while(0)

#endif

// generated_code.c - 模拟生成的代码
// 这个文件模拟由代码生成工具创建的代码
#line 1 "original_source.dsl"

#include "error_reporting.h"
#include <stdio.h>

void generated_function(void) {
    #line 42 "original_source.dsl"
    int important_variable = 0;
    
    #line 45 "original_source.dsl"
    if (important_variable < 0) {
        #line 46 "original_source.dsl"
        REPORT_ERROR("重要变量不能为负数");
    }
    
    #line 50 "original_source.dsl"
    printf("生成的函数执行完成\n");
}

// main.c
#include <stdio.h>
#include "error_reporting.h"

int main(void) {
    printf("错误报告优化示例\n");
    
    #line 100 "main_application.c"
    generated_function();
    
    #line 105 "main_application.c"
    CHECK_CONDITION(1 == 1, "这个检查应该通过");
    
    printf("程序执行完成\n");
    return 0;
}
```

### 4.2 调试信息增强

使用 `#line` 增强调试信息：

```c
// debug_enhancement.h
#ifndef DEBUG_ENHANCEMENT_H
#define DEBUG_ENHANCEMENT_H

#include <stdio.h>
#include <time.h>

// 调试级别定义
#define DEBUG_LEVEL_NONE    0
#define DEBUG_LEVEL_ERROR   1
#define DEBUG_LEVEL_WARNING 2
#define DEBUG_LEVEL_INFO    3
#define DEBUG_LEVEL_DEBUG   4

#ifndef CURRENT_DEBUG_LEVEL
    #define CURRENT_DEBUG_LEVEL DEBUG_LEVEL_INFO
#endif

// 时间戳函数
static inline void print_timestamp(void) {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    printf("[%04d-%02d-%02d %02d:%02d:%02d] ", 
           local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
           local->tm_hour, local->tm_min, local->tm_sec);
}

// 增强的调试宏
#if CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_ERROR
    #define DEBUG_ERROR(fmt, ...) \
        do { \
            print_timestamp(); \
            fprintf(stderr, "[ERROR %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } while(0)
#else
    #define DEBUG_ERROR(fmt, ...) do {} while(0)
#endif

#if CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_WARNING
    #define DEBUG_WARNING(fmt, ...) \
        do { \
            print_timestamp(); \
            fprintf(stderr, "[WARNING %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } while(0)
#else
    #define DEBUG_WARNING(fmt, ...) do {} while(0)
#endif

#if CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_INFO
    #define DEBUG_INFO(fmt, ...) \
        do { \
            print_timestamp(); \
            fprintf(stdout, "[INFO %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } while(0)
#else
    #define DEBUG_INFO(fmt, ...) do {} while(0)
#endif

#if CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG
    #define DEBUG_DEBUG(fmt, ...) \
        do { \
            print_timestamp(); \
            fprintf(stdout, "[DEBUG %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } while(0)
#else
    #define DEBUG_DEBUG(fmt, ...) do {} while(0)
#endif

// 函数跟踪宏
#define FUNCTION_TRACE_ENTER() \
    DEBUG_DEBUG("进入函数 %s", __func__)

#define FUNCTION_TRACE_EXIT() \
    DEBUG_DEBUG("退出函数 %s", __func__)

// 变量跟踪宏
#define TRACE_VARIABLE(var) \
    DEBUG_DEBUG("变量 %s = %s (类型: %s)", #var, \
                _Generic((var), \
                    int: "%d", \
                    float: "%f", \
                    double: "%f", \
                    char*: "%s", \
                    default: "%p"), var, #var)

#endif

// complex_module.c
#include "debug_enhancement.h"
#include <math.h>

#line 100 "math_operations.dsl"

double complex_calculation(double x, double y) {
    #line 105 "math_operations.dsl"
    FUNCTION_TRACE_ENTER();
    
    #line 107 "math_operations.dsl"
    TRACE_VARIABLE(x);
    TRACE_VARIABLE(y);
    
    #line 110 "math_operations.dsl"
    if (x < 0 || y < 0) {
        #line 111 "math_operations.dsl"
        DEBUG_WARNING("负数输入可能导致意外结果: x=%f, y=%f", x, y);
    }
    
    #line 114 "math_operations.dsl"
    double result = sqrt(x * x + y * y);
    
    #line 116 "math_operations.dsl"
    TRACE_VARIABLE(result);
    
    #line 118 "math_operations.dsl"
    FUNCTION_TRACE_EXIT();
    return result;
}

// main.c
#include <stdio.h>
#include "debug_enhancement.h"

int main(void) {
    DEBUG_INFO("程序开始执行");
    
    #line 200 "application_main.dsl"
    double result = complex_calculation(3.0, 4.0);
    
    #line 203 "application_main.dsl"
    DEBUG_INFO("计算结果: %f", result);
    
    #line 205 "application_main.dsl"
    DEBUG_INFO("程序执行完成");
    return 0;
}
```

### 4.3 条件编译管理

使用 `#line` 管理复杂的条件编译：

```c
// conditional_compilation.h
#ifndef CONDITIONAL_COMPILATION_H
#define CONDITIONAL_COMPILATION_H

#include <stdio.h>

// 平台检测
#if defined(_WIN32)
    #define PLATFORM "Windows"
    #define PLATFORM_ID 1
#elif defined(__linux__)
    #define PLATFORM "Linux"
    #define PLATFORM_ID 2
#elif defined(__APPLE__)
    #define PLATFORM "macOS"
    #define PLATFORM_ID 3
#else
    #define PLATFORM "Unknown"
    #define PLATFORM_ID 0
#endif

// 编译器检测
#if defined(__GNUC__)
    #define COMPILER "GCC"
    #define COMPILER_ID 1
#elif defined(_MSC_VER)
    #define COMPILER "MSVC"
    #define COMPILER_ID 2
#elif defined(__clang__)
    #define COMPILER "Clang"
    #define COMPILER_ID 3
#else
    #define COMPILER "Unknown"
    #define COMPILER_ID 0
#endif

// 版本检测
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#endif

// platform_specific.c
#include "conditional_compilation.h"
#include <stdio.h>

void platform_specific_function(void) {
    printf("平台信息: %s\n", PLATFORM);
    printf("编译器信息: %s\n", COMPILER);
    printf("版本信息: %d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

#if PLATFORM_ID == 1  // Windows
    #line 100 "windows_implementation.c"
    
    #include <windows.h>
    
    void platform_specific_implementation(void) {
        #line 105 "windows_implementation.c"
        printf("Windows 特定实现\n");
        
        #line 107 "windows_implementation.c"
        #ifdef UNICODE
            printf("使用 Unicode 支持\n");
        #else
            printf("使用 ANSI 支持\n");
        #endif
    }
    
#elif PLATFORM_ID == 2  // Linux
    #line 200 "linux_implementation.c"
    
    #include <unistd.h>
    
    void platform_specific_implementation(void) {
        #line 205 "linux_implementation.c"
        printf("Linux 特定实现\n");
        
        #line 207 "linux_implementation.c"
        printf("进程 ID: %d\n", getpid());
    }
    
#elif PLATFORM_ID == 3  // macOS
    #line 300 "macos_implementation.c"
    
    #include <sys/sysctl.h>
    
    void platform_specific_implementation(void) {
        #line 305 "macos_implementation.c"
        printf("macOS 特定实现\n");
        
        #line 307 "macos_implementation.c"
        int mib[2];
        size_t len;
        char buffer[256];
        
        mib[0] = CTL_KERN;
        mib[1] = KERN_OSTYPE;
        len = sizeof(buffer);
        
        if (sysctl(mib, 2, buffer, &len, NULL, 0) == 0) {
            printf("操作系统类型: %s\n", buffer);
        }
    }
    
#else  // Unknown
    #line 400 "unknown_implementation.c"
    
    void platform_specific_implementation(void) {
        #line 405 "unknown_implementation.c"
        printf("未知平台实现\n");
    }
    
#endif

// feature_specific.c
#include "conditional_compilation.h"
#include <stdio.h>

// 根据编译器选择不同的实现
#if COMPILER_ID == 1  // GCC
    #line 500 "gcc_features.c"
    
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
    
    void compiler_specific_features(void) {
        #line 506 "gcc_features.c"
        printf("GCC 特定功能\n");
        
        #line 508 "gcc_features.c"
        #pragma GCC optimize("O3")
        for (int i = 0; i < 1000; i++) {
            // GCC 优化循环
        }
    }
    
    #pragma GCC diagnostic pop
    
#elif COMPILER_ID == 2  // MSVC
    #line 600 "msvc_features.c"
    
    #pragma warning(push)
    #pragma warning(disable: 4100)
    
    void compiler_specific_features(void) {
        #line 606 "msvc_features.c"
        printf("MSVC 特定功能\n");
        
        #line 608 "msvc_features.c"
        __pragma(optimize("t", on))
        for (int i = 0; i < 1000; i++) {
            // MSVC 优化循环
        }
        __pragma(optimize("", off))
    }
    
    #pragma warning(pop)
    
#elif COMPILER_ID == 3  // Clang
    #line 700 "clang_features.c"
    
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-variable"
    
    void compiler_specific_features(void) {
        #line 706 "clang_features.c"
        printf("Clang 特定功能\n");
        
        #line 708 "clang_features.c"
        #pragma clang loop unroll(enable)
        for (int i = 0; i < 1000; i++) {
            // Clang 循环展开
        }
    }
    
    #pragma clang diagnostic pop
    
#endif

// main.c
#include <stdio.h>
#include "conditional_compilation.h"

int main(void) {
    printf("条件编译管理示例\n");
    printf("当前平台: %s\n", PLATFORM);
    printf("当前编译器: %s\n", COMPILER);
    
    platform_specific_function();
    platform_specific_implementation();
    compiler_specific_features();
    
    return 0;
}
```

## 5. 代码生成工具集成

### 5.1 代码生成器示例

展示如何在代码生成工具中使用 `#line`：

```c
// code_generator.c - 简单的代码生成器示例
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DSL (领域特定语言) 解析结构
typedef struct {
    const char* filename;
    int line_number;
    const char* content;
} DslLine;

// 生成的 C 代码模板
const char* generated_header = 
"#include <stdio.h>\n"
"#include <stdlib.h>\n\n"
"// 自动生成的代码 - 请勿手动修改\n\n";

// 代码生成函数
void generate_c_code(const DslLine* dsl_lines, int count, const char* output_file) {
    FILE* output = fopen(output_file, "w");
    if (!output) {
        fprintf(stderr, "无法创建输出文件: %s\n", output_file);
        return;
    }
    
    // 写入头部
    fprintf(output, "%s", generated_header);
    
    // 为每个 DSL 行生成对应的 C 代码
    for (int i = 0; i < count; i++) {
        const DslLine* line = &dsl_lines[i];
        
        // 插入 #line 指令以保持原始位置信息
        fprintf(output, "#line %d \"%s\"\n", line->line_number, line->filename);
        
        // 解析 DSL 内容并生成 C 代码
        if (strncmp(line->content, "PRINT:", 6) == 0) {
            const char* message = line->content + 6;
            fprintf(output, "    printf(\"%%s\\n\", \"%s\");\n", message);
        } else if (strncmp(line->content, "VAR:", 4) == 0) {
            const char* var_def = line->content + 4;
            fprintf(output, "    int %s = 0;\n", var_def);
        } else if (strncmp(line->content, "FUNC:", 5) == 0) {
            const char* func_name = line->content + 5;
            fprintf(output, "void %s(void) {\n", func_name);
        } else if (strcmp(line->content, "ENDFUNC") == 0) {
            fprintf(output, "}\n\n");
        }
    }
    
    fclose(output);
    printf("代码生成完成: %s\n", output_file);
}

// DSL 解析器
DslLine* parse_dsl_file(const char* dsl_file, int* count) {
    FILE* input = fopen(dsl_file, "r");
    if (!input) {
        fprintf(stderr, "无法打开 DSL 文件: %s\n", dsl_file);
        return NULL;
    }
    
    DslLine* lines = malloc(sizeof(DslLine) * 1000);  // 假设最多 1000 行
    if (!lines) {
        fclose(input);
        return NULL;
    }
    
    char buffer[1024];
    int line_number = 1;
    int parsed_count = 0;
    
    while (fgets(buffer, sizeof(buffer), input) && parsed_count < 1000) {
        // 移除换行符
        char* newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';
        
        // 跳过空行和注释
        if (buffer[0] == '\0' || buffer[0] == '#') {
            line_number++;
            continue;
        }
        
        // 存储解析的行
        lines[parsed_count].filename = dsl_file;
        lines[parsed_count].line_number = line_number;
        lines[parsed_count].content = strdup(buffer);
        
        parsed_count++;
        line_number++;
    }
    
    fclose(input);
    *count = parsed_count;
    return lines;
}

// DSL 示例文件内容 (dsl_example.dsl):
/*
# 这是一个简单的 DSL 示例
FUNC:hello_world
PRINT:Hello from generated code!
PRINT:This code was generated from DSL
ENDFUNC

VAR:counter
PRINT:Counter variable created
*/

// main_generator.c
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "用法: %s <dsl_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    const char* dsl_file = argv[1];
    const char* output_file = argv[2];
    
    printf("代码生成器启动\n");
    printf("DSL 文件: %s\n", dsl_file);
    printf("输出文件: %s\n", output_file);
    
    int count;
    DslLine* lines = parse_dsl_file(dsl_file, &count);
    
    if (lines) {
        generate_c_code(lines, count, output_file);
        
        // 释放内存
        for (int i = 0; i < count; i++) {
            free((void*)lines[i].content);
        }
        free(lines);
    }
    
    return 0;
}
```

### 5.2 模板引擎集成

在模板引擎中使用 `#line` 保持源文件信息：

```c
// template_engine.h
#ifndef TEMPLATE_ENGINE_H
#define TEMPLATE_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 模板变量结构
typedef struct {
    const char* name;
    const char* value;
} TemplateVariable;

// 模板块结构
typedef struct {
    const char* filename;
    int line_number;
    const char* content;
    int is_code;  // 1 表示代码块，0 表示文本
} TemplateBlock;

// 模板引擎上下文
typedef struct {
    TemplateBlock* blocks;
    int block_count;
    TemplateVariable* variables;
    int variable_count;
    const char* output_file;
} TemplateContext;

// 模板引擎函数声明
TemplateContext* create_template_context(const char* template_file);
void process_template(TemplateContext* ctx);
void destroy_template_context(TemplateContext* ctx);

#endif

// template_engine.c
#include "template_engine.h"

// 解析模板文件
TemplateContext* create_template_context(const char* template_file) {
    TemplateContext* ctx = malloc(sizeof(TemplateContext));
    if (!ctx) return NULL;
    
    memset(ctx, 0, sizeof(TemplateContext));
    ctx->output_file = "generated_output.c";
    
    // 解析模板文件（简化实现）
    FILE* input = fopen(template_file, "r");
    if (!input) {
        free(ctx);
        return NULL;
    }
    
    ctx->blocks = malloc(sizeof(TemplateBlock) * 1000);
    ctx->block_count = 0;
    
    char buffer[2048];
    int line_number = 1;
    
    while (fgets(buffer, sizeof(buffer), input) && ctx->block_count < 1000) {
        TemplateBlock* block = &ctx->blocks[ctx->block_count];
        
        // 检查是否为代码块
        if (strncmp(buffer, "{{", 2) == 0) {
            block->is_code = 1;
            // 移除 {{ 和 }}
            char* content = buffer + 2;
            char* end = strstr(content, "}}");
            if (end) *end = '\0';
            block->content = strdup(content);
        } else {
            block->is_code = 0;
            block->content = strdup(buffer);
        }
        
        block->filename = template_file;
        block->line_number = line_number;
        ctx->block_count++;
        line_number++;
    }
    
    fclose(input);
    return ctx;
}

// 处理模板生成代码
void process_template(TemplateContext* ctx) {
    FILE* output = fopen(ctx->output_file, "w");
    if (!output) return;
    
    fprintf(output, "// 自动生成的代码\n");
    fprintf(output, "// 基于模板: %s\n\n", ctx->blocks[0].filename);
    
    for (int i = 0; i < ctx->block_count; i++) {
        TemplateBlock* block = &ctx->blocks[i];
        
        // 插入 #line 指令保持源文件信息
        fprintf(output, "#line %d \"%s\"\n", block->line_number, block->filename);
        
        if (block->is_code) {
            // 处理代码块
            if (strcmp(block->content, "BEGIN_MAIN") == 0) {
                fprintf(output, "int main(void) {\n");
            } else if (strcmp(block->content, "END_MAIN") == 0) {
                fprintf(output, "    return 0;\n");
                fprintf(output, "}\n");
            } else if (strncmp(block->content, "PRINT:", 6) == 0) {
                const char* message = block->content + 6;
                fprintf(output, "    printf(\"%s\\n\");\n", message);
            }
        } else {
            // 处理文本块（作为注释输出）
            fprintf(output, "// %s", block->content);
        }
    }
    
    fclose(output);
    printf("模板处理完成: %s\n", ctx->output_file);
}

void destroy_template_context(TemplateContext* ctx) {
    if (!ctx) return;
    
    for (int i = 0; i < ctx->block_count; i++) {
        free((void*)ctx->blocks[i].content);
    }
    free(ctx->blocks);
    free(ctx);
}

// template_example.tpl - 模板文件示例:
/*
{{BEGIN_MAIN}}
{{PRINT:Hello from template!}}
{{PRINT:This code was generated from template}}
{{PRINT:Line numbers preserved with #line}}
{{END_MAIN}}
*/

// main_template.c
#include <stdio.h>
#include "template_engine.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "用法: %s <template_file>\n", argv[0]);
        return 1;
    }
    
    TemplateContext* ctx = create_template_context(argv[1]);
    if (ctx) {
        process_template(ctx);
        destroy_template_context(ctx);
    }
    
    return 0;
}
```

## 6. 调试和错误报告优化

### 6.1 高级调试系统

构建完整的调试系统利用 `#line` 和预定义宏：

```c
// advanced_debug.h
#ifndef ADVANCED_DEBUG_H
#define ADVANCED_DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 调试配置
#define DEBUG_ENABLED 1
#define DEBUG_LEVEL 3  // 0=无, 1=错误, 2=警告, 3=信息, 4=调试
#define DEBUG_OUTPUT_FILE "debug.log"

// 调试级别枚举
typedef enum {
    DEBUG_NONE = 0,
    DEBUG_ERROR = 1,
    DEBUG_WARNING = 2,
    DEBUG_INFO = 3,
    DEBUG_DEBUG = 4
} DebugLevel;

// 调试上下文
typedef struct {
    FILE* log_file;
    DebugLevel current_level;
    int thread_id;
    char module_name[64];
} DebugContext;

// 全局调试上下文
extern DebugContext g_debug_ctx;

// 初始化调试系统
int debug_init(const char* module_name, DebugLevel level);
void debug_cleanup(void);

// 时间戳函数
void debug_timestamp(char* buffer, size_t size);

// 核心调试函数
void debug_log(DebugLevel level, const char* file, int line, const char* format, ...);

// 调试宏定义
#if DEBUG_ENABLED && DEBUG_LEVEL >= DEBUG_ERROR
    #define DBG_ERROR(fmt, ...) \
        debug_log(DEBUG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define DBG_ERROR(fmt, ...) do {} while(0)
#endif

#if DEBUG_ENABLED && DEBUG_LEVEL >= DEBUG_WARNING
    #define DBG_WARNING(fmt, ...) \
        debug_log(DEBUG_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define DBG_WARNING(fmt, ...) do {} while(0)
#endif

#if DEBUG_ENABLED && DEBUG_LEVEL >= DEBUG_INFO
    #define DBG_INFO(fmt, ...) \
        debug_log(DEBUG_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define DBG_INFO(fmt, ...) do {} while(0)
#endif

#if DEBUG_ENABLED && DEBUG_LEVEL >= DEBUG_DEBUG
    #define DBG_DEBUG(fmt, ...) \
        debug_log(DEBUG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define DBG_DEBUG(fmt, ...) do {} while(0)
#endif

// 函数进入/退出跟踪
#define DBG_ENTER() \
    DBG_DEBUG("进入函数 %s", __func__)

#define DBG_EXIT() \
    DBG_DEBUG("退出函数 %s", __func__)

// 变量跟踪
#define DBG_VAR(var) \
    DBG_DEBUG("变量 %s = " DBG_VAR_FORMAT(var), #var, var)

#define DBG_VAR_FORMAT(var) _Generic((var), \
    int: "%d", \
    unsigned int: "%u", \
    long: "%ld", \
    unsigned long: "%lu", \
    float: "%f", \
    double: "%f", \
    char*: "%s", \
    const char*: "%s", \
    default: "%p")

// 断言宏
#define DBG_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            DBG_ERROR("断言失败: %s at %s:%d", #condition, __FILE__, __LINE__); \
            abort(); \
        } \
    } while(0)

#endif

// advanced_debug.c
#include "advanced_debug.h"
#include <stdarg.h>
#include <pthread.h>

// 全局调试上下文
DebugContext g_debug_ctx = {0};

// 时间戳函数
void debug_timestamp(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", local);
}

// 初始化调试系统
int debug_init(const char* module_name, DebugLevel level) {
    g_debug_ctx.log_file = fopen(DEBUG_OUTPUT_FILE, "a");
    if (!g_debug_ctx.log_file) {
        return -1;
    }
    
    g_debug_ctx.current_level = level;
    g_debug_ctx.thread_id = (int)pthread_self();
    
    if (module_name) {
        strncpy(g_debug_ctx.module_name, module_name, sizeof(g_debug_ctx.module_name) - 1);
    }
    
    DBG_INFO("调试系统初始化完成，模块: %s", module_name ? module_name : "unknown");
    return 0;
}

// 清理调试系统
void debug_cleanup(void) {
    if (g_debug_ctx.log_file) {
        DBG_INFO("调试系统关闭");
        fclose(g_debug_ctx.log_file);
        g_debug_ctx.log_file = NULL;
    }
}

// 核心调试函数
void debug_log(DebugLevel level, const char* file, int line, const char* format, ...) {
    if (level > g_debug_ctx.current_level || !g_debug_ctx.log_file) {
        return;
    }
    
    // 获取文件基名
    const char* basename = file;
    for (const char* p = file; *p; p++) {
        if (*p == '/' || *p == '\\') {
            basename = p + 1;
        }
    }
    
    // 时间戳
    char timestamp[32];
    debug_timestamp(timestamp);
    
    // 日志级别字符串
    const char* level_str = "";
    switch (level) {
        case DEBUG_ERROR:   level_str = "ERROR"; break;
        case DEBUG_WARNING: level_str = "WARN";  break;
        case DEBUG_INFO:    level_str = "INFO";  break;
        case DEBUG_DEBUG:   level_str = "DEBUG"; break;
        default:            level_str = "UNKNOWN"; break;
    }
    
    // 写入日志头
    fprintf(g_debug_ctx.log_file, "[%s] [%s] [%s:%d] [%s] ", 
            timestamp, level_str, basename, line, g_debug_ctx.module_name);
    
    // 写入日志内容
    va_list args;
    va_start(args, format);
    vfprintf(g_debug_ctx.log_file, format, args);
    va_end(args);
    
    fprintf(g_debug_ctx.log_file, "\n");
    fflush(g_debug_ctx.log_file);
}

// math_module.c
#include "advanced_debug.h"
#include <math.h>

#line 100 "math_operations.dsl"

double calculate_distance(double x1, double y1, double x2, double y2) {
    #line 105 "math_operations.dsl"
    DBG_ENTER();
    
    #line 107 "math_operations.dsl"
    DBG_VAR(x1); DBG_VAR(y1); DBG_VAR(x2); DBG_VAR(y2);
    
    #line 109 "math_operations.dsl"
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    #line 112 "math_operations.dsl"
    DBG_VAR(dx); DBG_VAR(dy);
    
    #line 114 "math_operations.dsl"
    double distance = sqrt(dx * dx + dy * dy);
    
    #line 116 "math_operations.dsl"
    DBG_VAR(distance);
    
    #line 118 "math_operations.dsl"
    DBG_EXIT();
    return distance;
}

#line 200 "validation.dsl"

int validate_input(double value, double min_val, double max_val) {
    #line 205 "validation.dsl"
    DBG_ENTER();
    
    #line 207 "validation.dsl"
    DBG_VAR(value); DBG_VAR(min_val); DBG_VAR(max_val);
    
    #line 209 "validation.dsl"
    if (value < min_val) {
        #line 210 "validation.dsl"
        DBG_WARNING("输入值 %f 低于最小值 %f", value, min_val);
        DBG_EXIT();
        return 0;
    }
    
    #line 215 "validation.dsl"
    if (value > max_val) {
        #line 216 "validation.dsl"
        DBG_WARNING("输入值 %f 高于最大值 %f", value, max_val);
        DBG_EXIT();
        return 0;
    }
    
    #line 221 "validation.dsl"
    DBG_INFO("输入值 %f 在有效范围内 [%f, %f]", value, min_val, max_val);
    DBG_EXIT();
    return 1;
}

// main_debug.c
#include <stdio.h>
#include "advanced_debug.h"

int main(void) {
    // 初始化调试系统
    if (debug_init("main_module", DEBUG_DEBUG) != 0) {
        fprintf(stderr, "调试系统初始化失败\n");
        return 1;
    }
    
    DBG_INFO("程序开始执行");
    
    #line 300 "application_main.dsl"
    
    double dist = calculate_distance(0.0, 0.0, 3.0, 4.0);
    DBG_INFO("计算距离: %f", dist);
    
    #line 305 "application_main.dsl"
    
    int valid = validate_input(5.0, 0.0, 10.0);
    DBG_INFO("输入验证结果: %s", valid ? "有效" : "无效");
    
    #line 310 "application_main.dsl"
    
    DBG_INFO("程序执行完成");
    
    // 清理调试系统
    debug_cleanup();
    return 0;
}
```

### 6.2 错误处理框架

构建完整的错误处理框架：

```c
// error_framework.h
#ifndef ERROR_FRAMEWORK_H
#define ERROR_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

// 错误代码定义
typedef enum {
    ERR_NONE = 0,
    ERR_INVALID_INPUT = 1001,
    ERR_OUT_OF_MEMORY = 1002,
    ERR_FILE_NOT_FOUND = 1003,
    ERR_PERMISSION_DENIED = 1004,
    ERR_INTERNAL_ERROR = 9999
} ErrorCode;

// 错误信息结构
typedef struct {
    ErrorCode code;
    const char* file;
    int line;
    const char* function;
    const char* message;
    const char* context;
} ErrorInfo;

// 错误处理上下文
typedef struct {
    jmp_buf jump_buffer;
    ErrorInfo last_error;
    int error_occurred;
    void (*error_handler)(const ErrorInfo* error);
} ErrorContext;

// 全局错误上下文
extern ErrorContext g_error_ctx;

// 错误处理函数
void error_init(void (*handler)(const ErrorInfo* error));
void error_cleanup(void);
void error_set_handler(void (*handler)(const ErrorInfo* error));

// 错误抛出宏
#define ERROR_THROW(code, msg) \
    do { \
        g_error_ctx.last_error.code = code; \
        g_error_ctx.last_error.file = __FILE__; \
        g_error_ctx.last_error.line = __LINE__; \
        g_error_ctx.last_error.function = __func__; \
        g_error_ctx.last_error.message = msg; \
        g_error_ctx.last_error.context = NULL; \
        g_error_ctx.error_occurred = 1; \
        if (g_error_ctx.error_handler) { \
            g_error_ctx.error_handler(&g_error_ctx.last_error); \
        } \
        longjmp(g_error_ctx.jump_buffer, code); \
    } while(0)

// 带上下文的错误抛出
#define ERROR_THROW_CTX(code, msg, ctx) \
    do { \
        g_error_ctx.last_error.code = code; \
        g_error_ctx.last_error.file = __FILE__; \
        g_error_ctx.last_error.line = __LINE__; \
        g_error_ctx.last_error.function = __func__; \
        g_error_ctx.last_error.message = msg; \
        g_error_ctx.last_error.context = ctx; \
        g_error_ctx.error_occurred = 1; \
        if (g_error_ctx.error_handler) { \
            g_error_ctx.error_handler(&g_error_ctx.last_error); \
        } \
        longjmp(g_error_ctx.jump_buffer, code); \
    } while(0)

// 错误检查宏
#define ERROR_CHECK(condition, code, msg) \
    do { \
        if (!(condition)) { \
            ERROR_THROW(code, msg); \
        } \
    } while(0)

// 内存分配检查
#define ERROR_CHECK_MALLOC(ptr) \
    ERROR_CHECK(ptr != NULL, ERR_OUT_OF_MEMORY, "内存分配失败")

// 文件操作检查
#define ERROR_CHECK_FILE(file) \
    ERROR_CHECK(file != NULL, ERR_FILE_NOT_FOUND, "文件打开失败")

// 错误处理块
#define ERROR_TRY \
    do { \
        int __error_code = setjmp(g_error_ctx.jump_buffer); \
        if (__error_code == 0) {

#define ERROR_CATCH \
        } else {

#define ERROR_END \
        } \
    } while(0)

// 获取最后错误
const ErrorInfo* error_get_last(void);
int error_occurred(void);
void error_clear(void);

#endif

// error_framework.c
#include "error_framework.h"
#include <string.h>

// 全局错误上下文
ErrorContext g_error_ctx = {0};

// 默认错误处理器
static void default_error_handler(const ErrorInfo* error) {
    fprintf(stderr, "错误 [%d] 在 %s:%d %s(): %s", 
            error->code, error->file, error->line, error->function, error->message);
    
    if (error->context) {
        fprintf(stderr, " (上下文: %s)", error->context);
    }
    fprintf(stderr, "\n");
}

// 初始化错误处理系统
void error_init(void (*handler)(const ErrorInfo* error)) {
    g_error_ctx.error_handler = handler ? handler : default_error_handler;
    g_error_ctx.error_occurred = 0;
    memset(&g_error_ctx.last_error, 0, sizeof(ErrorInfo));
}

// 清理错误处理系统
void error_cleanup(void) {
    error_clear();
}

// 设置错误处理器
void error_set_handler(void (*handler)(const ErrorInfo* error)) {
    g_error_ctx.error_handler = handler ? handler : default_error_handler;
}

// 获取最后错误
const ErrorInfo* error_get_last(void) {
    return g_error_ctx.error_occurred ? &g_error_ctx.last_error : NULL;
}

// 检查是否有错误
int error_occurred(void) {
    return g_error_ctx.error_occurred;
}

// 清除错误状态
void error_clear(void) {
    g_error_ctx.error_occurred = 0;
    memset(&g_error_ctx.last_error, 0, sizeof(ErrorInfo));
}

// file_operations.c
#include "error_framework.h"
#include <stdio.h>
#include <stdlib.h>

#line 100 "file_processing.dsl"

FILE* open_file_safely(const char* filename, const char* mode) {
    #line 105 "file_processing.dsl"
    FILE* file = fopen(filename, mode);
    
    #line 107 "file_processing.dsl"
    if (!file) {
        #line 108 "file_processing.dsl"
        char context[256];
        snprintf(context, sizeof(context), "文件名: %s, 模式: %s", filename, mode);
        ERROR_THROW_CTX(ERR_FILE_NOT_FOUND, "无法打开文件", context);
    }
    
    #line 113 "file_processing.dsl"
    return file;
}

#line 200 "memory_management.dsl"

void* allocate_memory_safely(size_t size) {
    #line 205 "memory_management.dsl"
    void* ptr = malloc(size);
    
    #line 207 "memory_management.dsl"
    if (!ptr) {
        #line 208 "memory_management.dsl"
        char context[64];
        snprintf(context, sizeof(context), "请求大小: %zu 字节", size);
        ERROR_THROW_CTX(ERR_OUT_OF_MEMORY, "内存分配失败", context);
    }
    
    #line 213 "memory_management.dsl"
    return ptr;
}

// data_processing.c
#include "error_framework.h"
#include <string.h>

#line 300 "data_validation.dsl"

int process_data(const char* data, size_t length) {
    #line 305 "data_validation.dsl"
    ERROR_CHECK(data != NULL, ERR_INVALID_INPUT, "数据指针为空");
    ERROR_CHECK(length > 0, ERR_INVALID_INPUT, "数据长度必须大于0");
    ERROR_CHECK(length <= 1000000, ERR_INVALID_INPUT, "数据长度超出限制");
    
    #line 309 "data_validation.dsl"
    // 模拟数据处理
    char* buffer = allocate_memory_safely(length + 1);
    
    #line 312 "data_validation.dsl"
    memcpy(buffer, data, length);
    buffer[length] = '\0';
    
    #line 315 "data_validation.dsl"
    printf("处理数据: %s (长度: %zu)\n", buffer, length);
    free(buffer);
    
    #line 318 "data_validation.dsl"
    return 0;
}

// main_error.c
#include <stdio.h>
#include "error_framework.h"

int main(void) {
    // 初始化错误处理系统
    error_init(NULL);
    
    printf("错误处理框架示例\n");
    
    #line 400 "application_main.dsl"
    
    // 安全的文件操作
    ERROR_TRY
        FILE* file = open_file_safely("nonexistent.txt", "r");
        printf("文件打开成功\n");
        fclose(file);
    ERROR_CATCH
        const ErrorInfo* error = error_get_last();
        if (error) {
            printf("捕获到错误: %d - %s\n", error->code, error->message);
        }
    ERROR_END
    
    #line 415 "application_main.dsl"
    
    // 安全的内存操作
    ERROR_TRY
        void* ptr = allocate_memory_safely(1024 * 1024 * 1024);  // 1GB
        printf("内存分配成功\n");
        free(ptr);
    ERROR_CATCH
        const ErrorInfo* error = error_get_last();
        if (error) {
            printf("捕获到错误: %d - %s\n", error->code, error->message);
            if (error->context) {
                printf("错误上下文: %s\n", error->context);
            }
        }
    ERROR_END
    
    #line 430 "application_main.dsl"
    
    // 数据处理示例
    ERROR_TRY
        const char* test_data = "Hello, World!";
        process_data(test_data, strlen(test_data));
    ERROR_CATCH
        const ErrorInfo* error = error_get_last();
        if (error) {
            printf("数据处理错误: %d - %s\n", error->code, error->message);
        }
    ERROR_END
    
    #line 440 "application_main.dsl"
    
    // 错误的输入处理
    ERROR_TRY
        process_data(NULL, 100);
    ERROR_CATCH
        const ErrorInfo* error = error_get_last();
        if (error) {
            printf("输入验证错误: %d - %s\n", error->code, error->message);
        }
    ERROR_END
    
    #line 450 "application_main.dsl"
    
    printf("程序执行完成\n");
    
    // 清理错误处理系统
    error_cleanup();
    return 0;
}
```

## 7. 最佳实践

### 7.1 #line 指令使用规范

编写清晰、可维护的 `#line` 指令代码：

```c
// line_directive_best_practices.h
#ifndef LINE_DIRECTIVE_BEST_PRACTICES_H
#define LINE_DIRECTIVE_BEST_PRACTIVES_H

#include <stdio.h>

// 1. 使用有意义的行号和文件名
#define ORIGINAL_SOURCE_FILE "original_source.dsl"
#define GENERATED_SOURCE_FILE "generated_code.c"

// 2. 定义行号常量
#define MATH_OPERATIONS_START_LINE 100
#define FILE_OPERATIONS_START_LINE 200
#define NETWORK_OPERATIONS_START_LINE 300

// 3. 使用宏封装复杂的 #line 指令
#define SET_LINE_INFO(line, file) \
    _Pragma(STRINGIFY(line)) \
    _Pragma(STRINGIFY(file))

#define STRINGIFY(x) #x

// 4. 在代码生成工具中使用标准格式
#define GENERATED_CODE_HEADER(filename) \
    "#line 1 \"" filename "\"\n" \
    "/* 自动生成的代码 - 基于 " filename " */\n\n"

// 5. 保持行号的连续性
void demonstrate_line_continuity(void) {
    printf("原始行号: %d\n", __LINE__);  // 假设为 30
    
#line 1000 "generated_source.c"
    printf("生成代码行号: %d\n", __LINE__);  // 1001
    
    // 保持连续性
#line 1002 "generated_source.c"
    printf("连续行号: %d\n", __LINE__);  // 1003
}

// 6. 使用条件编译保护
#ifdef CODE_GENERATION_TOOL
    #line 2000 "original_dsl_file.dsl"
    
    // DSL 解析生成的代码
    void generated_function_from_dsl(void) {
        #line 2005 "original_dsl_file.dsl"
        printf("来自 DSL 的生成函数\n");
        
        #line 2007 "original_dsl_file.dsl"
        // 更多生成的代码...
    }
#endif

// 7. 错误报告中的行号使用
void error_reporting_with_line_info(void) {
    int error_line = __LINE__;
    const char* error_file = __FILE__;
    
    fprintf(stderr, "错误发生在 %s:%d\n", error_file, error_line);
    
    // 在特定位置设置行号以提供更准确的错误信息
#line 5000 "error_reporting_module.c"
    // 错误处理代码...
}

// 8. 调试信息中的行号管理
#ifdef DEBUG
    #define DEBUG_LINE_INFO() \
        fprintf(stderr, "调试信息 [%s:%d]\n", __FILE__, __LINE__)
#else
    #define DEBUG_LINE_INFO() do {} while(0)
#endif

void debugging_with_line_info(void) {
    DEBUG_LINE_INFO();
    
#line 3000 "debug_module.c"
    // 调试相关代码
    DEBUG_LINE_INFO();
}

#endif

// main_best_practices.c
#include <stdio.h>
#include "line_directive_best_practices.h"

int main(void) {
    printf("C 文件名和行信息最佳实践示例\n");
    
    demonstrate_line_continuity();
    error_reporting_with_line_info();
    debugging_with_line_info();
    
    return 0;
}
```

### 7.2 代码生成工具最佳实践

构建高质量的代码生成工具：

```c
// code_generator_best_practices.h
#ifndef CODE_GENERATOR_BEST_PRACTICES_H
#define CODE_GENERATOR_BEST_PRACTICES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 代码生成器配置
typedef struct {
    const char* input_file;
    const char* output_file;
    const char* template_file;
    int preserve_line_numbers;
    int generate_debug_info;
    const char* module_name;
} CodeGenConfig;

// 生成的代码信息
typedef struct {
    const char* source_file;
    int source_line;
    const char* generated_content;
    size_t content_length;
} GeneratedCodeInfo;

// 代码生成器上下文
typedef struct {
    CodeGenConfig config;
    FILE* output_stream;
    int current_line;
    char current_file[256];
    GeneratedCodeInfo* code_blocks;
    int block_count;
    int block_capacity;
} CodeGenerator;

// 代码生成器函数
CodeGenerator* create_code_generator(const CodeGenConfig* config);
int generate_code(CodeGenerator* generator);
void destroy_code_generator(CodeGenerator* generator);

// 代码块管理
int add_code_block(CodeGenerator* generator, const char* source_file, 
                   int source_line, const char* content);
void write_line_directive(CodeGenerator* generator, const char* file, int line);

// 格式化输出函数
void write_formatted_code(CodeGenerator* generator, const char* format, ...);
void write_comment(CodeGenerator* generator, const char* comment);

#endif

// code_generator_best_practices.c
#include "code_generator_best_practices.h"
#include <stdarg.h>

// 创建代码生成器
CodeGenerator* create_code_generator(const CodeGenConfig* config) {
    CodeGenerator* generator = malloc(sizeof(CodeGenerator));
    if (!generator) return NULL;
    
    memset(generator, 0, sizeof(CodeGenerator));
    generator->config = *config;
    generator->block_capacity = 100;
    generator->code_blocks = malloc(sizeof(GeneratedCodeInfo) * generator->block_capacity);
    
    if (!generator->code_blocks) {
        free(generator);
        return NULL;
    }
    
    generator->output_stream = fopen(config->output_file, "w");
    if (!generator->output_stream) {
        free(generator->code_blocks);
        free(generator);
        return NULL;
    }
    
    // 写入生成器头部信息
    fprintf(generator->output_stream, 
            "/*\n"
            " * 自动生成的代码\n"
            " * 模块: %s\n"
            " * 源文件: %s\n"
            " * 生成时间: %s %s\n"
            " * 请勿手动修改\n"
            " */\n\n", 
            config->module_name ? config->module_name : "unknown",
            config->input_file,
            __DATE__, __TIME__);
    
    strcpy(generator->current_file, config->output_file);
    generator->current_line = 1;
    
    return generator;
}

// 添加代码块
int add_code_block(CodeGenerator* generator, const char* source_file, 
                   int source_line, const char* content) {
    if (generator->block_count >= generator->block_capacity) {
        generator->block_capacity *= 2;
        GeneratedCodeInfo* new_blocks = realloc(generator->code_blocks, 
                                               sizeof(GeneratedCodeInfo) * generator->block_capacity);
        if (!new_blocks) return -1;
        generator->code_blocks = new_blocks;
    }
    
    GeneratedCodeInfo* block = &generator->code_blocks[generator->block_count];
    block->source_file = source_file;
    block->source_line = source_line;
    block->generated_content = content;
    block->content_length = strlen(content);
    
    generator->block_count++;
    return 0;
}

// 写入行指令
void write_line_directive(CodeGenerator* generator, const char* file, int line) {
    if (generator->config.preserve_line_numbers) {
        fprintf(generator->output_stream, "#line %d \"%s\"\n", line, file);
        generator->current_line++;
    }
}

// 格式化写入代码
void write_formatted_code(CodeGenerator* generator, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(generator->output_stream, format, args);
    va_end(args);
    
    // 计算写入的行数（简化实现）
    generator->current_line++;
}

// 写入注释
void write_comment(CodeGenerator* generator, const char* comment) {
    fprintf(generator->output_stream, "/* %s */\n", comment);
    generator->current_line++;
}

// 生成代码主函数
int generate_code(CodeGenerator* generator) {
    if (!generator) return -1;
    
    // 写入包含文件
    write_comment(generator, "包含必要的头文件");
    write_line_directive(generator, "generator_template.h", 10);
    write_formatted_code(generator, "#include <stdio.h>\n");
    write_formatted_code(generator, "#include <stdlib.h>\n\n");
    
    // 生成主函数
    write_comment(generator, "主函数实现");
    write_line_directive(generator, "generator_template.h", 20);
    write_formatted_code(generator, "int main(void) {\n");
    
    write_line_directive(generator, "generator_template.h", 25);
    write_formatted_code(generator, "    printf(\"Hello from generated code!\\n\");\n");
    
    write_line_directive(generator, "generator_template.h", 30);
    write_formatted_code(generator, "    return 0;\n");
    write_formatted_code(generator, "}\n");
    
    return 0;
}

// 销毁代码生成器
void destroy_code_generator(CodeGenerator* generator) {
    if (!generator) return;
    
    if (generator->output_stream) {
        fclose(generator->output_stream);
    }
    
    if (generator->code_blocks) {
        free(generator->code_blocks);
    }
    
    free(generator);
}

// main_generator.c
#include <stdio.h>
#include "code_generator_best_practices.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "用法: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    CodeGenConfig config = {
        .input_file = argv[1],
        .output_file = argv[2],
        .template_file = NULL,
        .preserve_line_numbers = 1,
        .generate_debug_info = 1,
        .module_name = "example_module"
    };
    
    CodeGenerator* generator = create_code_generator(&config);
    if (!generator) {
        fprintf(stderr, "创建代码生成器失败\n");
        return 1;
    }
    
    printf("开始生成代码...\n");
    printf("输入文件: %s\n", config.input_file);
    printf("输出文件: %s\n", config.output_file);
    
    if (generate_code(generator) != 0) {
        fprintf(stderr, "代码生成失败\n");
        destroy_code_generator(generator);
        return 1;
    }
    
    printf("代码生成完成\n");
    destroy_code_generator(generator);
    return 0;
}
```

## 8. 标准参考

### 8.1 C 标准版本演进

C 语言标准对 `#line` 指令的支持演进：

- C89/C90 (ISO/IEC 9899:1990)：定义了基本的 `#line` 语法，行号限制为 32767
- C99 (ISO/IEC 9899:1999)：扩展了行号限制到 2147483647
- C11 (ISO/IEC 9899:2011)：保持 C99 的 `#line` 支持
- C17 (ISO/IEC 9899:2018)：保持之前的 `#line` 支持
- C23 (ISO/IEC 9899:2024)：继续支持现有的 `#line` 机制

### 8.2 相关标准条款

**C23, C17, C11, C99, C89**：

- 6.10.4 Line control：详细描述了 `#line` 指令的语法、语义和处理过程
- J.1 Unspecified behavior：描述了 `#line __LINE__` 后续行号的未指定行为

### 8.3 编译器实现差异

不同编译器对 `#line` 指令的实现可能存在细微差异：

```c
// standard_compliance.h
#ifndef STANDARD_COMPLIANCE_H
#define STANDARD_COMPLIANCE_H

#include <stdio.h>

// C89/C90 标准合规性检查
#if defined(__STDC_VERSION__) && __STDC_VERSION__ == 0
    #define C89_COMPLIANT 1
    #pragma message "符合 C89/C90 标准"
    
    // C89 行号限制测试
    #define C89_MAX_LINE 32767
    
    #if C89_MAX_LINE > 32767
        #pragma message "警告: 行号超出 C89 限制"
    #endif
    
#else
    #define C89_COMPLIANT 0
    #pragma message "使用 C89 之后的 C 标准"
#endif

// C99 标准合规性检查
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define C99_COMPLIANT 1
    #pragma message "符合 C99 标准"
    
    // C99 行号限制测试
    #define C99_MAX_LINE 2147483647
    
    #if C99_MAX_LINE > 32767
        #pragma message "支持扩展的行号范围"
    #endif
    
#else
    #define C99_COMPLIANT 0
    #pragma message "使用 C99 之前的 C 标准"
#endif

// C11 标准合规性检查
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #define C11_COMPLIANT 1
    #pragma message "符合 C11 标准"
#else
    #define C11_COMPLIANT 0
    #pragma message "使用 C11 之前的 C 标准"
#endif

// C17 标准合规性检查
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201710L
    #define C17_COMPLIANT 1
    #pragma message "符合 C17 标准"
#else
    #define C17_COMPLIANT 0
    #pragma message "使用 C17 之前的 C 标准"
#endif

// 编译器特定标准支持
#if defined(__GNUC__)
    #if __GNUC__ >= 3
        #define GCC_C_COMPLIANT 1
        #if C99_COMPLIANT
            #pragma message "GCC 完全支持 C99"
        #endif
        #if C11_COMPLIANT
            #pragma message "GCC 完全支持 C11"
        #endif
        #if C17_COMPLIANT
            #pragma message "GCC 完全支持 C17"
        #endif
    #else
        #define GCC_C_COMPLIANT 0
        #pragma message "旧版 GCC，标准支持有限"
    #endif
#endif

#if defined(__clang__)
    #if __clang_major__ >= 3
        #define CLANG_C_COMPLIANT 1
        #if C99_COMPLIANT
            #pragma message "Clang 完全支持 C99"
        #endif
        #if C11_COMPLIANT
            #pragma message "Clang 完全支持 C11"
        #endif
        #if C17_COMPLIANT
            #pragma message "Clang 完全支持 C17"
        #endif
    #else
        #define CLANG_C_COMPLIANT 0
        #pragma message "旧版 Clang，标准支持有限"
    #endif
#endif

#if defined(_MSC_VER)
    #if _MSC_VER >= 1500
        #define MSVC_C_COMPLIANT 1
        #if C99_COMPLIANT
            #pragma message "MSVC 支持 C99 (部分)"
        #endif
        #if C11_COMPLIANT
            #pragma message "MSVC 支持 C11"
        #endif
    #else
        #define MSVC_C_COMPLIANT 0
        #pragma message "旧版 MSVC，标准支持有限"
    #endif
#endif

// 标准合规性测试函数
void test_standard_compliance(void) {
    printf("标准合规性测试:\n");
    printf("  C89 支持: %s\n", C89_COMPLIANT ? "是" : "否");
    printf("  C99 支持: %s\n", C99_COMPLIANT ? "是" : "否");
    printf("  C11 支持: %s\n", C11_COMPLIANT ? "是" : "否");
    printf("  C17 支持: %s\n", C17_COMPLIANT ? "是" : "否");
    
    printf("\n编译器支持:\n");
    printf("  GCC 支持: %s\n", GCC_C_COMPLIANT ? "是" : "否");
    printf("  Clang 支持: %s\n", CLANG_C_COMPLIANT ? "是" : "否");
    printf("  MSVC 支持: %s\n", MSVC_C_COMPLIANT ? "是" : "否");
    
    // 行号测试
    printf("\n行号测试:\n");
    printf("  当前行号: %d\n", __LINE__);
    
#line 10000
    printf("  重置后行号: %d\n", __LINE__);
    
    // 文件名测试
    printf("  当前文件: %s\n", __FILE__);
    
#line 20000 "test_file.c"
    printf("  重置后文件: %s\n", __FILE__);
}

#endif

// main_standard.c
#include <stdio.h>
#include "standard_compliance.h"

int main(void) {
    printf("C 标准合规性测试\n");
    test_standard_compliance();
    return 0;
}
```

## 9. 总结

### 9.1 核心要点

C 文件名和行信息控制通过 `#line` 指令提供了强大的源代码位置管理能力：

- 基本功能：修改预处理器维护的行号和文件名信息
- 语法形式：`#line lineno` 和 `#line lineno "filename"` 两种形式
- 预定义宏：`__LINE__` 和 `__FILE__` 提供当前位置信息
- 应用场景：代码生成工具、调试信息优化、错误报告改进

### 9.2 使用建议

为了有效使用 `#line` 指令，请遵循以下建议：

- 明确目的：清楚了解使用 `#line` 的具体目的和预期效果
- 保持一致性：在代码生成工具中保持行号和文件名的一致性
- 避免滥用：不要过度使用 `#line`，以免造成代码可读性问题
- 测试验证：验证 `#line` 对调试器和错误报告的实际影响
- 文档说明：为复杂的 `#line` 使用添加适当的注释说明

### 9.3 发展趋势

C 文件名和行信息控制的发展趋势：

- 标准化完善：标准对 `#line` 指令的支持不断完善
- 工具集成：IDE 和调试工具对 `#line` 的支持更加完善
- 代码生成：代码生成工具对 `#line` 的使用更加普遍
- 调试优化：调试器对 `#line` 生成的调试信息处理更加智能

通过深入理解 `#line` 指令的原理和最佳实践，开发者可以构建更加高效、可维护的 C 程序，特别是在代码生成和调试优化方面发挥重要作用。
