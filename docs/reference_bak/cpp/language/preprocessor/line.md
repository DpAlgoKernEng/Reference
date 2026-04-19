# C++ 文件名和行信息详解

[1. C++ 文件名和行信息基本概念](#1-c-文件名和行信息基本概念)

[2. #line 指令语法详解](#2-line-指令语法详解)

[3. 预定义宏 __LINE__ 和 __FILE__](#3-预定义宏-__line__-和-__file__)

[4. #line 指令应用实践](#4-line-指令应用实践)

[5. 代码生成工具集成](#5-代码生成工具集成)

[6. 调试和错误报告优化](#6-调试和错误报告优化)

[7. 现代 C++ 特性集成](#7-现代-c-特性集成)

[8. 最佳实践](#8-最佳实践)

[9. 标准参考](#9-标准参考)

[10. 总结](#10-总结)

## 1. C++ 文件名和行信息基本概念

### 1.1 什么是文件名和行信息控制

文件名和行信息控制是指通过 `#line` 指令来修改预处理器中的当前行号和文件名信息。这是 C++ 语言预处理器提供的一个特殊功能，主要用于：

- 代码生成工具：从其他语言生成 C++ 代码时保持原始源文件的行号信息
- 调试信息优化：改善调试器和错误报告中的文件位置信息
- 宏展开跟踪：在复杂的宏展开中提供准确的位置信息
- 条件编译管理：在条件编译块中维护正确的行号信息
- 现代 C++ 集成：与 `std::source_location` 等现代特性协同工作

### 1.2 #line 指令的核心作用

`#line` 指令的主要作用是修改预处理器维护的两个重要状态：

```cpp
// 基本示例
#line 100
// 从这里开始，__LINE__ 将从 100 开始计数

#line 200 "original_file.cpp"
// 从这里开始，__LINE__ 从 200 开始，__FILE__ 变为 "original_file.cpp"
```

### 1.3 预处理器状态管理

预处理器维护的两个关键状态变量：

```cpp
// 预处理器内部状态示例
// 假设当前状态：
// 当前行号: 42
// 当前文件名: "main.cpp"

#include <iostream>

std::cout << "当前行号: " << __LINE__ << std::endl;  // 输出: 当前行号: 43
std::cout << "当前文件: " << __FILE__ << std::endl;  // 输出: 当前文件: main.cpp

#line 1000 "generated_code.cpp"
// 状态更新为：
// 当前行号: 1000
// 当前文件名: "generated_code.cpp"

std::cout << "更新后行号: " << __LINE__ << std::endl;  // 输出: 更新后行号: 1001
std::cout << "更新后文件: " << __FILE__ << std::endl;  // 输出: 更新后文件: generated_code.cpp
```

## 2. #line 指令语法详解

### 2.1 基本语法形式

`#line` 指令有两种语法形式：

```cpp
// 形式 1：仅修改行号
#line lineno

// 形式 2：同时修改行号和文件名
#line lineno "filename"
```

### 2.2 语法参数详解

#### 2.2.1 行号参数 (lineno)

行号参数的要求和限制：

```cpp
// 合法的行号示例
#line 1          // 最小有效行号
#line 100        // 普通行号
#line 32767      // C++98/C++03 最大行号
#line 2147483647 // C++11 及以后的最大行号

// 非法的行号示例
#line 0          // 未定义行为（行号必须 >= 1）
#line -1         // 未定义行为（负数）
#line abc        // 编译错误（非数字）
#line 0x10       // 可能被接受，但始终解释为十进制

// 宏展开的行号
#define CURRENT_LINE __LINE__
#define SET_LINE_TO(x) #line x

#line 500
std::cout << "当前行号: " << __LINE__ << std::endl;  // 输出: 当前行号: 501

#line CURRENT_LINE  // 宏展开后可能产生意外结果
std::cout << "宏展开行号: " << __LINE__ << std::endl;
```

#### 2.2.2 文件名参数 (filename)

文件名参数的处理规则：

```cpp
// 合法的文件名示例
#line 100 "main.cpp"
#line 200 "path/to/file.hpp"
#line 300 "file with spaces.cpp"
#line 400 "file\"with\"quotes.cpp"  // 转义引号

// 特殊字符处理
#line 500 "file\nwith\nnewlines.cpp"  // 换行符可能被处理
#line 600 "file\twith\ttabs.cpp"      // 制表符可能被处理

// 宏中的文件名
#define ORIGINAL_FILE "original_source.cpp"
#define GENERATED_FILE "generated_code.cpp"

#line 100 ORIGINAL_FILE
// 等效于 #line 100 "original_source.cpp"

#line 200 GENERATED_FILE
// 等效于 #line 200 "generated_code.cpp"
```

### 2.3 语法转换规则

`#line` 指令的处理过程：

```cpp
// 1. 宏展开阶段
#define LINE_NUMBER 1000
#define FILE_NAME "generated.cpp"

#line LINE_NUMBER FILE_NAME
// 预处理器首先展开宏：
// #line 1000 "generated.cpp"

// 2. 行号计算规则
#line 100
// __LINE__ 从 100 开始
std::cout << "Line 1: " << __LINE__ << std::endl;  // 输出: Line 1: 101
std::cout << "Line 2: " << __LINE__ << std::endl;  // 输出: Line 2: 102

// 3. 文件名继承规则
// 在文件 "main.cpp" 中
std::cout << "File 1: " << __FILE__ << std::endl;  // 输出: File 1: main.cpp

#line 200 "new_file.cpp"
std::cout << "File 2: " << __FILE__ << std::endl;  // 输出: File 2: new_file.cpp

#line 300  // 仅修改行号，文件名保持不变
std::cout << "File 3: " << __FILE__ << std::endl;  // 输出: File 3: new_file.cpp
```

## 3. 预定义宏 __LINE__ 和 __FILE__

### 3.1 __LINE__ 宏详解

`__LINE__` 宏提供当前源代码行号信息：

```cpp
// __LINE__ 宏的基本使用
#include <iostream>

class LineMacroDemo {
public:
    void demonstrateLineMacro() {
        std::cout << "当前行号: " << __LINE__ << std::endl;  // 输出当前行号
        
#line 1000
        std::cout << "修改后行号: " << __LINE__ << std::endl;  // 输出: 修改后行号: 1001
        
        // 在宏中的使用
        #define PRINT_LINE() std::cout << "宏中的行号: " << __LINE__ << std::endl
        PRINT_LINE();  // 输出宏定义处的行号，不是调用处
        
        // 条件编译中的行号
        #if __LINE__ > 1005
            std::cout << "条件编译行号检查: " << __LINE__ << std::endl;
        #endif
    }

    // 行号计算示例
    void lineNumberCalculation() {
        std::cout << "起始行号: " << __LINE__ << std::endl;  // 假设为 25
        
#line 50
        std::cout << "重置后行号: " << __LINE__ << std::endl;  // 51
        
        // 后续行号基于重置值计算
        std::cout << "后续行号 1: " << __LINE__ << std::endl;  // 52
        std::cout << "后续行号 2: " << __LINE__ << std::endl;  // 53
    }
};
```

### 3.2 __FILE__ 宏详解

`__FILE__` 宏提供当前源文件名信息：

```cpp
// __FILE__ 宏的基本使用
#include <iostream>
#include <string>

class FileMacroDemo {
public:
    void demonstrateFileMacro() {
        std::cout << "当前文件: " << __FILE__ << std::endl;  // 输出当前文件名
        
#line 100 "custom_file.cpp"
        std::cout << "修改后文件: " << __FILE__ << std::endl;  // 输出: custom_file.cpp
        
        // 在包含文件中的使用
        // #include "included_file.hpp"  // __FILE__ 在 included_file.hpp 中会显示实际文件名
    }

    // 文件名处理示例
    void fileNameHandling() {
        std::cout << "原始文件: " << __FILE__ << std::endl;
        
        // 路径处理
        std::string filename = __FILE__;
        std::string basename = filename;
        
        // 简单的文件名提取（实际应用中可能需要更复杂的处理）
        for (size_t i = 0; i < filename.length(); ++i) {
            if (filename[i] == '/' || filename[i] == '\\') {
                basename = filename.substr(i + 1);
            }
        }
        
        std::cout << "文件基名: " << basename << std::endl;
    }

    // 条件编译中的文件检查
    void conditionalCompilationWithFile() {
#if defined(__FILE__) && __LINE__ > 0
        std::cout << "文件和行号信息可用: " << __FILE__ << ":" << __LINE__ << std::endl;
#endif

        // 基于文件名的条件编译
        #ifdef DEBUG
            if (std::string(__FILE__).find("debug") != std::string::npos) {
                std::cout << "调试模式下的文件: " << __FILE__ << std::endl;
            }
        #endif
    }
};
```

### 3.3 组合使用示例

`__LINE__` 和 `__FILE__` 的组合使用：

```cpp
// debug_macros.hpp - 调试宏定义
#ifndef DEBUG_MACROS_HPP
#define DEBUG_MACROS_HPP

#include <iostream>
#include <sstream>
#include <string>

// 基本调试宏
#define DEBUG_PRINT(fmt, ...) \
    do { \
        std::ostringstream oss; \
        oss << "[" << __FILE__ << ":" << __LINE__ << "] " << fmt << "\n"; \
        std::cerr << oss.str(); \
    } while(0)

#define ERROR_PRINT(fmt, ...) \
    do { \
        std::ostringstream oss; \
        oss << "ERROR [" << __FILE__ << ":" << __LINE__ << "] " << fmt << "\n"; \
        std::cerr << oss.str(); \
    } while(0)

#define WARNING_PRINT(fmt, ...) \
    do { \
        std::ostringstream oss; \
        oss << "WARNING [" << __FILE__ << ":" << __LINE__ << "] " << fmt << "\n"; \
        std::cerr << oss.str(); \
    } while(0)

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
            std::abort(); \
        } \
    } while(0)

#endif

// main.cpp
#include <iostream>
#include <cstdlib>
#include "debug_macros.hpp"

class MainApplication {
public:
    int run() {
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
};

int main() {
    MainApplication app;
    return app.run();
}
```

## 4. #line 指令应用实践

### 4.1 错误报告优化

使用 `#line` 改善错误报告的准确性：

```cpp
// error_reporting.hpp
#ifndef ERROR_REPORTING_HPP
#define ERROR_REPORTING_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

// 错误报告结构
struct ErrorInfo {
    std::string file;
    int line;
    std::string message;
    int error_code;
    
    ErrorInfo(const std::string& f, int l, const std::string& m, int c = 0)
        : file(f), line(l), message(m), error_code(c) {}
};

// 错误报告宏
#define REPORT_ERROR(msg) \
    do { \
        std::ostringstream oss; \
        oss << "错误 [" << __FILE__ << ":" << __LINE__ << "]: " << msg; \
        std::cerr << oss.str() << std::endl; \
        std::exit(EXIT_FAILURE); \
    } while(0)

#define REPORT_WARNING(msg) \
    do { \
        std::ostringstream oss; \
        oss << "警告 [" << __FILE__ << ":" << __LINE__ << "]: " << msg; \
        std::cerr << oss.str() << std::endl; \
    } while(0)

// 带错误代码的报告
#define REPORT_ERROR_CODE(code, msg) \
    do { \
        std::ostringstream oss; \
        oss << "错误 " << code << " [" << __FILE__ << ":" << __LINE__ << "]: " << msg; \
        std::cerr << oss.str() << std::endl; \
        std::exit(code); \
    } while(0)

// 条件错误报告
#define CHECK_CONDITION(condition, msg) \
    do { \
        if (!(condition)) { \
            REPORT_ERROR(msg); \
        } \
    } while(0)

#endif

// generated_code.cpp - 模拟生成的代码
// 这个文件模拟由代码生成工具创建的代码
#line 1 "original_source.dsl"

#include "error_reporting.hpp"
#include <iostream>

class GeneratedClass {
public:
    void generatedFunction() {
        #line 42 "original_source.dsl"
        int important_variable = 0;
        
        #line 45 "original_source.dsl"
        if (important_variable < 0) {
            #line 46 "original_source.dsl"
            REPORT_ERROR("重要变量不能为负数");
        }
        
        #line 50 "original_source.dsl"
        std::cout << "生成的函数执行完成\n";
    }
};

// main.cpp
#include <iostream>
#include "error_reporting.hpp"

class MainApplication {
public:
    int run() {
        std::cout << "错误报告优化示例\n";
        
        #line 100 "main_application.cpp"
        GeneratedClass gen_obj;
        gen_obj.generatedFunction();
        
        #line 105 "main_application.cpp"
        CHECK_CONDITION(1 == 1, "这个检查应该通过");
        
        std::cout << "程序执行完成\n";
        return 0;
    }
};

int main() {
    MainApplication app;
    return app.run();
}
```

### 4.2 调试信息增强

使用 `#line` 增强调试信息：

```cpp
// debug_enhancement.hpp
#ifndef DEBUG_ENHANCEMENT_HPP
#define DEBUG_ENHANCEMENT_HPP

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

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
inline std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

// 增强的调试宏
#if CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_ERROR
    #define DEBUG_ERROR(fmt, ...) \
        do { \
            std::ostringstream oss; \
            oss << "[" << getCurrentTimestamp() << "] [ERROR " << __FILE__ << ":" << __LINE__ << "] " \
                << fmt << "\n"; \
            std::cerr << oss.str(); \
        } while(0)
#else
    #define DEBUG_ERROR(fmt, ...) do {} while(0)
#endif

#if CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_WARNING
    #define DEBUG_WARNING(fmt, ...) \
        do { \
            std::ostringstream oss; \
            oss << "[" << getCurrentTimestamp() << "] [WARNING " << __FILE__ << ":" << __LINE__ << "] " \
                << fmt << "\n"; \
            std::cerr << oss.str(); \
        } while(0)
#else
    #define DEBUG_WARNING(fmt, ...) do {} while(0)
#endif

#if CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_INFO
    #define DEBUG_INFO(fmt, ...) \
        do { \
            std::ostringstream oss; \
            oss << "[" << getCurrentTimestamp() << "] [INFO " << __FILE__ << ":" << __LINE__ << "] " \
                << fmt << "\n"; \
            std::cout << oss.str(); \
        } while(0)
#else
    #define DEBUG_INFO(fmt, ...) do {} while(0)
#endif

#if CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG
    #define DEBUG_DEBUG(fmt, ...) \
        do { \
            std::ostringstream oss; \
            oss << "[" << getCurrentTimestamp() << "] [DEBUG " << __FILE__ << ":" << __LINE__ << "] " \
                << fmt << "\n"; \
            std::cout << oss.str(); \
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
                typeid(var).name(), #var)

#endif

// complex_module.cpp
#include "debug_enhancement.hpp"
#include <cmath>

#line 100 "math_operations.dsl"

class ComplexCalculation {
public:
    double complexCalculation(double x, double y) {
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
        double result = std::sqrt(x * x + y * y);
        
        #line 116 "math_operations.dsl"
        TRACE_VARIABLE(result);
        
        #line 118 "math_operations.dsl"
        FUNCTION_TRACE_EXIT();
        return result;
    }
};

// main.cpp
#include <iostream>
#include "debug_enhancement.hpp"

class MainApplication {
public:
    int run() {
        DEBUG_INFO("程序开始执行");
        
        #line 200 "application_main.dsl"
        ComplexCalculation calc;
        double result = calc.complexCalculation(3.0, 4.0);
        
        #line 203 "application_main.dsl"
        DEBUG_INFO("计算结果: %f", result);
        
        #line 205 "application_main.dsl"
        DEBUG_INFO("程序执行完成");
        return 0;
    }
};

int main() {
    MainApplication app;
    return app.run();
}
```

### 4.3 条件编译管理

使用 `#line` 管理复杂的条件编译：

```cpp
// conditional_compilation.hpp
#ifndef CONDITIONAL_COMPILATION_HPP
#define CONDITIONAL_COMPILATION_HPP

#include <iostream>
#include <string>

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

// platform_specific.cpp
#include "conditional_compilation.hpp"
#include <iostream>

class PlatformSpecific {
public:
    void platformSpecificFunction() {
        std::cout << "平台信息: " << PLATFORM << std::endl;
        std::cout << "编译器信息: " << COMPILER << std::endl;
        std::cout << "版本信息: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
    }
};

#if PLATFORM_ID == 1  // Windows
    #line 100 "windows_implementation.cpp"
    
    #include <windows.h>
    
    class WindowsImplementation {
    public:
        void platformSpecificImplementation() {
            #line 105 "windows_implementation.cpp"
            std::cout << "Windows 特定实现\n";
            
            #line 107 "windows_implementation.cpp"
            #ifdef UNICODE
                std::cout << "使用 Unicode 支持\n";
            #else
                std::cout << "使用 ANSI 支持\n";
            #endif
        }
    };
    
#elif PLATFORM_ID == 2  // Linux
    #line 200 "linux_implementation.cpp"
    
    #include <unistd.h>
    
    class LinuxImplementation {
    public:
        void platformSpecificImplementation() {
            #line 205 "linux_implementation.cpp"
            std::cout << "Linux 特定实现\n";
            
            #line 207 "linux_implementation.cpp"
            std::cout << "进程 ID: " << getpid() << std::endl;
        }
    };
    
#elif PLATFORM_ID == 3  // macOS
    #line 300 "macos_implementation.cpp"
    
    #include <sys/sysctl.h>
    
    class MacOSImplementation {
    public:
        void platformSpecificImplementation() {
            #line 305 "macos_implementation.cpp"
            std::cout << "macOS 特定实现\n";
            
            #line 307 "macos_implementation.cpp"
            int mib[2];
            size_t len;
            char buffer[256];
            
            mib[0] = CTL_KERN;
            mib[1] = KERN_OSTYPE;
            len = sizeof(buffer);
            
            if (sysctl(mib, 2, buffer, &len, nullptr, 0) == 0) {
                std::cout << "操作系统类型: " << buffer << std::endl;
            }
        }
    };
    
#else  // Unknown
    #line 400 "unknown_implementation.cpp"
    
    class UnknownImplementation {
    public:
        void platformSpecificImplementation() {
            #line 405 "unknown_implementation.cpp"
            std::cout << "未知平台实现\n";
        }
    };
    
#endif

// feature_specific.cpp
#include "conditional_compilation.hpp"
#include <iostream>

// 根据编译器选择不同的实现
#if COMPILER_ID == 1  // GCC
    #line 500 "gcc_features.cpp"
    
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
    
    class GCCFeatures {
    public:
        void compilerSpecificFeatures() {
            #line 506 "gcc_features.cpp"
            std::cout << "GCC 特定功能\n";
            
            #line 508 "gcc_features.cpp"
            #pragma GCC optimize("O3")
            for (int i = 0; i < 1000; i++) {
                // GCC 优化循环
            }
        }
    };
    
    #pragma GCC diagnostic pop
    
#elif COMPILER_ID == 2  // MSVC
    #line 600 "msvc_features.cpp"
    
    #pragma warning(push)
    #pragma warning(disable: 4100)
    
    class MSVCFeatures {
    public:
        void compilerSpecificFeatures() {
            #line 606 "msvc_features.cpp"
            std::cout << "MSVC 特定功能\n";
            
            #line 608 "msvc_features.cpp"
            __pragma(optimize("t", on))
            for (int i = 0; i < 1000; i++) {
                // MSVC 优化循环
            }
            __pragma(optimize("", off))
        }
    };
    
    #pragma warning(pop)
    
#elif COMPILER_ID == 3  // Clang
    #line 700 "clang_features.cpp"
    
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-variable"
    
    class ClangFeatures {
    public:
        void compilerSpecificFeatures() {
            #line 706 "clang_features.cpp"
            std::cout << "Clang 特定功能\n";
            
            #line 708 "clang_features.cpp"
            #pragma clang loop unroll(enable)
            for (int i = 0; i < 1000; i++) {
                // Clang 循环展开
            }
        }
    };
    
    #pragma clang diagnostic pop
    
#endif

// main.cpp
#include <iostream>
#include "conditional_compilation.hpp"

class MainApplication {
public:
    int run() {
        std::cout << "条件编译管理示例\n";
        std::cout << "当前平台: " << PLATFORM << std::endl;
        std::cout << "当前编译器: " << COMPILER << std::endl;
        
        PlatformSpecific platform_obj;
        platform_obj.platformSpecificFunction();
        
        // 根据平台调用相应实现
#if PLATFORM_ID == 1
        WindowsImplementation win_impl;
        win_impl.platformSpecificImplementation();
#elif PLATFORM_ID == 2
        LinuxImplementation linux_impl;
        linux_impl.platformSpecificImplementation();
#elif PLATFORM_ID == 3
        MacOSImplementation macos_impl;
        macos_impl.platformSpecificImplementation();
#else
        UnknownImplementation unknown_impl;
        unknown_impl.platformSpecificImplementation();
#endif
        
        // 根据编译器调用相应实现
#if COMPILER_ID == 1
        GCCFeatures gcc_features;
        gcc_features.compilerSpecificFeatures();
#elif COMPILER_ID == 2
        MSVCFeatures msvc_features;
        msvc_features.compilerSpecificFeatures();
#elif COMPILER_ID == 3
        ClangFeatures clang_features;
        clang_features.compilerSpecificFeatures();
#endif
        
        return 0;
    }
};

int main() {
    MainApplication app;
    return app.run();
}
```

## 5. 代码生成工具集成

### 5.1 代码生成器示例

展示如何在代码生成工具中使用 `#line`：

```cpp
// code_generator.hpp - 简单的代码生成器示例
#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

// DSL (领域特定语言) 解析结构
struct DslLine {
    std::string filename;
    int line_number;
    std::string content;
};

// 生成的 C++ 代码模板
const std::string generated_header = 
R"(#include <iostream>
#include <cstdlib>

// 自动生成的代码 - 请勿手动修改

)";

// 代码生成类
class CodeGenerator {
private:
    std::vector<DslLine> dsl_lines;
    std::string output_file;

public:
    CodeGenerator(const std::string& output_filename) : output_file(output_filename) {}
    
    // 解析 DSL 文件
    bool parseDslFile(const std::string& dsl_file);
    
    // 生成 C++ 代码
    bool generateCppCode();
    
    // 获取解析的行数
    size_t getLineCount() const { return dsl_lines.size(); }
};

#endif

// code_generator.cpp
#include "code_generator.hpp"
#include <fstream>
#include <sstream>

bool CodeGenerator::parseDslFile(const std::string& dsl_file) {
    std::ifstream input(dsl_file);
    if (!input.is_open()) {
        std::cerr << "无法打开 DSL 文件: " << dsl_file << std::endl;
        return false;
    }
    
    std::string line;
    int line_number = 1;
    
    while (std::getline(input, line)) {
        // 跳过空行和注释
        if (line.empty() || line[0] == '#') {
            line_number++;
            continue;
        }
        
        // 存储解析的行
        DslLine dsl_line;
        dsl_line.filename = dsl_file;
        dsl_line.line_number = line_number;
        dsl_line.content = line;
        
        dsl_lines.push_back(dsl_line);
        line_number++;
    }
    
    input.close();
    return true;
}

bool CodeGenerator::generateCppCode() {
    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cerr << "无法创建输出文件: " << output_file << std::endl;
        return false;
    }
    
    // 写入头部
    output << generated_header;
    
    // 为每个 DSL 行生成对应的 C++ 代码
    for (const auto& line : dsl_lines) {
        // 插入 #line 指令以保持原始位置信息
        output << "#line " << line.line_number << " \"" << line.filename << "\"\n";
        
        // 解析 DSL 内容并生成 C++ 代码
        if (line.content.substr(0, 6) == "PRINT:") {
            std::string message = line.content.substr(6);
            output << "    std::cout << \"" << message << "\\n\";\n";
        } else if (line.content.substr(0, 4) == "VAR:") {
            std::string var_def = line.content.substr(4);
            output << "    int " << var_def << " = 0;\n";
        } else if (line.content.substr(0, 5) == "FUNC:") {
            std::string func_name = line.content.substr(5);
            output << "void " << func_name << "() {\n";
        } else if (line.content == "ENDFUNC") {
            output << "}\n\n";
        }
    }
    
    output.close();
    std::cout << "代码生成完成: " << output_file << std::endl;
    return true;
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

// main_generator.cpp
#include <iostream>
#include "code_generator.hpp"

class MainGenerator {
public:
    int run(int argc, char* argv[]) {
        if (argc != 3) {
            std::cerr << "用法: " << argv[0] << " <dsl_file> <output_file>\n";
            return 1;
        }
        
        std::string dsl_file = argv[1];
        std::string output_file = argv[2];
        
        std::cout << "代码生成器启动\n";
        std::cout << "DSL 文件: " << dsl_file << std::endl;
        std::cout << "输出文件: " << output_file << std::endl;
        
        CodeGenerator generator(output_file);
        if (generator.parseDslFile(dsl_file)) {
            if (generator.generateCppCode()) {
                std::cout << "成功生成 " << generator.getLineCount() << " 行代码\n";
            } else {
                std::cerr << "代码生成失败\n";
                return 1;
            }
        } else {
            std::cerr << "DSL 解析失败\n";
            return 1;
        }
        
        return 0;
    }
};

int main(int argc, char* argv[]) {
    MainGenerator generator;
    return generator.run(argc, argv);
}
```

### 5.2 模板引擎集成

在模板引擎中使用 `#line` 保持源文件信息：

```cpp
// template_engine.hpp
#ifndef TEMPLATE_ENGINE_HPP
#define TEMPLATE_ENGINE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

// 模板变量结构
struct TemplateVariable {
    std::string name;
    std::string value;
};

// 模板块结构
struct TemplateBlock {
    std::string filename;
    int line_number;
    std::string content;
    bool is_code;  // true 表示代码块，false 表示文本
};

// 模板引擎上下文
class TemplateContext {
private:
    std::vector<TemplateBlock> blocks;
    std::vector<TemplateVariable> variables;
    std::string output_file;

public:
    TemplateContext(const std::string& out_file) : output_file(out_file) {}
    
    // 解析模板文件
    bool parseTemplateFile(const std::string& template_file);
    
    // 处理模板生成代码
    bool processTemplate();
    
    // 获取块数量
    size_t getBlockCount() const { return blocks.size(); }
};

#endif

// template_engine.cpp
#include "template_engine.hpp"
#include <fstream>
#include <sstream>

bool TemplateContext::parseTemplateFile(const std::string& template_file) {
    std::ifstream input(template_file);
    if (!input.is_open()) {
        std::cerr << "无法打开模板文件: " << template_file << std::endl;
        return false;
    }
    
    std::string line;
    int line_number = 1;
    
    while (std::getline(input, line)) {
        TemplateBlock block;
        
        // 检查是否为代码块
        if (line.substr(0, 2) == "{{" && line.length() >= 4) {
            block.is_code = true;
            // 移除 {{ 和 }}
            size_t end_pos = line.find("}}");
            if (end_pos != std::string::npos) {
                block.content = line.substr(2, end_pos - 2);
            } else {
                block.content = line.substr(2);
            }
        } else {
            block.is_code = false;
            block.content = line;
        }
        
        block.filename = template_file;
        block.line_number = line_number;
        blocks.push_back(block);
        line_number++;
    }
    
    input.close();
    return true;
}

bool TemplateContext::processTemplate() {
    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cerr << "无法创建输出文件: " << output_file << std::endl;
        return false;
    }
    
    output << "// 自动生成的代码\n";
    output << "// 基于模板: " << (blocks.empty() ? "unknown" : blocks[0].filename) << "\n\n";
    
    for (const auto& block : blocks) {
        // 插入 #line 指令保持源文件信息
        output << "#line " << block.line_number << " \"" << block.filename << "\"\n";
        
        if (block.is_code) {
            // 处理代码块
            if (block.content == "BEGIN_MAIN") {
                output << "int main() {\n";
            } else if (block.content == "END_MAIN") {
                output << "    return 0;\n";
                output << "}\n";
            } else if (block.content.substr(0, 6) == "PRINT:") {
                std::string message = block.content.substr(6);
                output << "    std::cout << \"" << message << "\\n\";\n";
            }
        } else {
            // 处理文本块（作为注释输出）
            output << "// " << block.content << "\n";
        }
    }
    
    output.close();
    std::cout << "模板处理完成: " << output_file << std::endl;
    return true;
}

// template_example.tpl - 模板文件示例:
/*
{{BEGIN_MAIN}}
{{PRINT:Hello from template!}}
{{PRINT:This code was generated from template}}
{{PRINT:Line numbers preserved with #line}}
{{END_MAIN}}
*/

// main_template.cpp
#include <iostream>
#include "template_engine.hpp"

class MainTemplate {
public:
    int run(int argc, char* argv[]) {
        if (argc != 2) {
            std::cerr << "用法: " << argv[0] << " <template_file>\n";
            return 1;
        }
        
        std::unique_ptr<TemplateContext> ctx = 
            std::make_unique<TemplateContext>("generated_output.cpp");
        
        if (ctx->parseTemplateFile(argv[1])) {
            if (ctx->processTemplate()) {
                std::cout << "成功处理 " << ctx->getBlockCount() << " 个模板块\n";
            } else {
                std::cerr << "模板处理失败\n";
                return 1;
            }
        } else {
            std::cerr << "模板解析失败\n";
            return 1;
        }
        
        return 0;
    }
};

int main(int argc, char* argv[]) {
    MainTemplate template_app;
    return template_app.run(argc, argv);
}
```

## 6. 调试和错误报告优化

### 6.1 高级调试系统

构建完整的调试系统利用 `#line` 和预定义宏：

```cpp
// advanced_debug.hpp
#ifndef ADVANCED_DEBUG_HPP
#define ADVANCED_DEBUG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <chrono>
#include <iomanip>
#include <thread>

// 调试配置
#define DEBUG_ENABLED 1
#define DEBUG_LEVEL 3  // 0=无, 1=错误, 2=警告, 3=信息, 4=调试
#define DEBUG_OUTPUT_FILE "debug.log"

// 调试级别枚举
enum class DebugLevel {
    NONE = 0,
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DEBUG = 4
};

// 调试上下文
class DebugContext {
private:
    std::ofstream log_file;
    DebugLevel current_level;
    std::thread::id thread_id;
    std::string module_name;

public:
    DebugContext(const std::string& module, DebugLevel level);
    ~DebugContext();
    
    // 时间戳函数
    std::string getTimestamp() const;
    
    // 核心调试函数
    void log(DebugLevel level, const std::string& file, int line, 
             const std::string& format, ...);
    
    // 获取当前级别
    DebugLevel getCurrentLevel() const { return current_level; }
};

// 全局调试上下文
extern std::unique_ptr<DebugContext> g_debug_ctx;

// 初始化调试系统
bool debug_init(const std::string& module_name, DebugLevel level);
void debug_cleanup();

// 调试宏定义
#if DEBUG_ENABLED && DEBUG_LEVEL >= 1
    #define DBG_ERROR(fmt, ...) \
        do { \
            if (g_debug_ctx && g_debug_ctx->getCurrentLevel() >= DebugLevel::ERROR) { \
                g_debug_ctx->log(DebugLevel::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
            } \
        } while(0)
#else
    #define DBG_ERROR(fmt, ...) do {} while(0)
#endif

#if DEBUG_ENABLED && DEBUG_LEVEL >= 2
    #define DBG_WARNING(fmt, ...) \
        do { \
            if (g_debug_ctx && g_debug_ctx->getCurrentLevel() >= DebugLevel::WARNING) { \
                g_debug_ctx->log(DebugLevel::WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
            } \
        } while(0)
#else
    #define DBG_WARNING(fmt, ...) do {} while(0)
#endif

#if DEBUG_ENABLED && DEBUG_LEVEL >= 3
    #define DBG_INFO(fmt, ...) \
        do { \
            if (g_debug_ctx && g_debug_ctx->getCurrentLevel() >= DebugLevel::INFO) { \
                g_debug_ctx->log(DebugLevel::INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
            } \
        } while(0)
#else
    #define DBG_INFO(fmt, ...) do {} while(0)
#endif

#if DEBUG_ENABLED && DEBUG_LEVEL >= 4
    #define DBG_DEBUG(fmt, ...) \
        do { \
            if (g_debug_ctx && g_debug_ctx->getCurrentLevel() >= DebugLevel::DEBUG) { \
                g_debug_ctx->log(DebugLevel::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
            } \
        } while(0)
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
    DBG_DEBUG("变量 %s = %s", #var, std::to_string(var).c_str())

// 断言宏
#define DBG_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            DBG_ERROR("断言失败: %s at %s:%d", #condition, __FILE__, __LINE__); \
            std::abort(); \
        } \
    } while(0)

#endif

// advanced_debug.cpp
#include "advanced_debug.hpp"
#include <cstdarg>
#include <filesystem>

DebugContext::DebugContext(const std::string& module, DebugLevel level)
    : current_level(level), thread_id(std::this_thread::get_id()), module_name(module) {
    
    log_file.open(DEBUG_OUTPUT_FILE, std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "无法打开调试日志文件: " << DEBUG_OUTPUT_FILE << std::endl;
    }
}

DebugContext::~DebugContext() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

std::string DebugContext::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void DebugContext::log(DebugLevel level, const std::string& file, int line, 
                       const std::string& format, ...) {
    if (level > current_level || !log_file.is_open()) {
        return;
    }
    
    // 获取文件基名
    std::string basename = file;
    size_t last_slash = file.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        basename = file.substr(last_slash + 1);
    }
    
    // 日志级别字符串
    std::string level_str;
    switch (level) {
        case DebugLevel::ERROR:   level_str = "ERROR"; break;
        case DebugLevel::WARNING: level_str = "WARN";  break;
        case DebugLevel::INFO:    level_str = "INFO";  break;
        case DebugLevel::DEBUG:   level_str = "DEBUG"; break;
        default:                  level_str = "UNKNOWN"; break;
    }
    
    // 写入日志头
    log_file << "[" << getTimestamp() << "] [" << level_str << "] [" 
             << basename << ":" << line << "] [" << module_name << "] ";
    
    // 写入日志内容
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
    va_end(args);
    
    log_file << buffer << "\n";
    log_file.flush();
}

// 全局调试上下文
std::unique_ptr<DebugContext> g_debug_ctx;

// 初始化调试系统
bool debug_init(const std::string& module_name, DebugLevel level) {
    try {
        g_debug_ctx = std::make_unique<DebugContext>(module_name, level);
        DBG_INFO("调试系统初始化完成，模块: %s", module_name.c_str());
        return true;
    } catch (const std::exception& e) {
        std::cerr << "调试系统初始化失败: " << e.what() << std::endl;
        return false;
    }
}

// 清理调试系统
void debug_cleanup() {
    if (g_debug_ctx) {
        DBG_INFO("调试系统关闭");
        g_debug_ctx.reset();
    }
}

// math_module.cpp
#include "advanced_debug.hpp"
#include <cmath>

#line 100 "math_operations.dsl"

class MathOperations {
public:
    double calculateDistance(double x1, double y1, double x2, double y2) {
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
        double distance = std::sqrt(dx * dx + dy * dy);
        
        #line 116 "math_operations.dsl"
        DBG_VAR(distance);
        
        #line 118 "math_operations.dsl"
        DBG_EXIT();
        return distance;
    }
};

#line 200 "validation.dsl"

class Validation {
public:
    bool validateInput(double value, double min_val, double max_val) {
        #line 205 "validation.dsl"
        DBG_ENTER();
        
        #line 207 "validation.dsl"
        DBG_VAR(value); DBG_VAR(min_val); DBG_VAR(max_val);
        
        #line 209 "validation.dsl"
        if (value < min_val) {
            #line 210 "validation.dsl"
            DBG_WARNING("输入值 %f 低于最小值 %f", value, min_val);
            DBG_EXIT();
            return false;
        }
        
        #line 215 "validation.dsl"
        if (value > max_val) {
            #line 216 "validation.dsl"
            DBG_WARNING("输入值 %f 高于最大值 %f", value, max_val);
            DBG_EXIT();
            return false;
        }
        
        #line 221 "validation.dsl"
        DBG_INFO("输入值 %f 在有效范围内 [%f, %f]", value, min_val, max_val);
        DBG_EXIT();
        return true;
    }
};

// main_debug.cpp
#include <iostream>
#include "advanced_debug.hpp"

class MainDebug {
public:
    int run() {
        // 初始化调试系统
        if (!debug_init("main_module", DebugLevel::DEBUG)) {
            std::cerr << "调试系统初始化失败\n";
            return 1;
        }
        
        DBG_INFO("程序开始执行");
        
        #line 300 "application_main.dsl"
        
        MathOperations math_ops;
        double dist = math_ops.calculateDistance(0.0, 0.0, 3.0, 4.0);
        DBG_INFO("计算距离: %f", dist);
        
        #line 305 "application_main.dsl"
        
        Validation validator;
        bool valid = validator.validateInput(5.0, 0.0, 10.0);
        DBG_INFO("输入验证结果: %s", valid ? "有效" : "无效");
        
        #line 310 "application_main.dsl"
        
        DBG_INFO("程序执行完成");
        
        // 清理调试系统
        debug_cleanup();
        return 0;
    }
};

int main() {
    MainDebug debug_app;
    return debug_app.run();
}
```

### 6.2 错误处理框架

构建完整的错误处理框架：

```cpp
// error_framework.hpp
#ifndef ERROR_FRAMEWORK_HPP
#define ERROR_FRAMEWORK_HPP

#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <csetjmp>

// 错误代码定义
enum class ErrorCode {
    NONE = 0,
    INVALID_INPUT = 1001,
    OUT_OF_MEMORY = 1002,
    FILE_NOT_FOUND = 1003,
    PERMISSION_DENIED = 1004,
    INTERNAL_ERROR = 9999
};

// 错误信息结构
struct ErrorInfo {
    ErrorCode code;
    std::string file;
    int line;
    std::string function;
    std::string message;
    std::string context;
    
    ErrorInfo(ErrorCode c, const std::string& f, int l, const std::string& func,
              const std::string& msg, const std::string& ctx = "")
        : code(c), file(f), line(l), function(func), message(msg), context(ctx) {}
};

// 错误处理上下文
class ErrorContext {
private:
    jmp_buf jump_buffer;
    ErrorInfo last_error;
    bool error_occurred;
    std::function<void(const ErrorInfo&)> error_handler;

public:
    ErrorContext();
    
    // 错误处理函数
    void setErrorHandler(std::function<void(const ErrorInfo&)> handler);
    
    // 错误抛出方法
    [[noreturn]] void throwError(ErrorCode code, const std::string& file, int line,
                                const std::string& function, const std::string& message,
                                const std::string& context = "");
    
    // 错误检查方法
    void checkCondition(bool condition, ErrorCode code, const std::string& file, int line,
                       const std::string& function, const std::string& message);
    
    // 获取最后错误
    const ErrorInfo* getLastError() const;
    bool hasError() const { return error_occurred; }
    void clearError() { error_occurred = false; }
    
    // 跳转缓冲区访问
    jmp_buf& getJumpBuffer() { return jump_buffer; }
};

// 全局错误上下文
extern std::unique_ptr<ErrorContext> g_error_ctx;

// 初始化错误处理系统
bool error_init();
void error_cleanup();

// 错误抛出宏
#define ERROR_THROW(code, msg) \
    do { \
        if (g_error_ctx) { \
            g_error_ctx->throwError(code, __FILE__, __LINE__, __func__, msg); \
        } \
    } while(0)

// 带上下文的错误抛出
#define ERROR_THROW_CTX(code, msg, ctx) \
    do { \
        if (g_error_ctx) { \
            g_error_ctx->throwError(code, __FILE__, __LINE__, __func__, msg, ctx); \
        } \
    } while(0)

// 错误检查宏
#define ERROR_CHECK(condition, code, msg) \
    do { \
        if (g_error_ctx) { \
            g_error_ctx->checkCondition(condition, code, __FILE__, __LINE__, __func__, msg); \
        } \
    } while(0)

// 内存分配检查
#define ERROR_CHECK_MALLOC(ptr) \
    ERROR_CHECK(ptr != nullptr, ErrorCode::OUT_OF_MEMORY, "内存分配失败")

// 文件操作检查
#define ERROR_CHECK_FILE(file) \
    ERROR_CHECK(file != nullptr, ErrorCode::FILE_NOT_FOUND, "文件打开失败")

// 错误处理块
#define ERROR_TRY \
    do { \
        if (g_error_ctx) { \
            int __error_code = setjmp(g_error_ctx->getJumpBuffer()); \
            if (__error_code == 0) {

#define ERROR_CATCH \
            } else {

#define ERROR_END \
            } \
        } \
    } while(0)

#endif

// error_framework.cpp
#include "error_framework.hpp"

ErrorContext::ErrorContext() : error_occurred(false) {
    error_handler = [](const ErrorInfo& error) {
        std::cerr << "错误 [" << static_cast<int>(error.code) << "] 在 " 
                  << error.file << ":" << error.line << " " << error.function 
                  << "(): " << error.message;
        
        if (!error.context.empty()) {
            std::cerr << " (上下文: " << error.context << ")";
        }
        std::cerr << std::endl;
    };
}

void ErrorContext::setErrorHandler(std::function<void(const ErrorInfo&)> handler) {
    error_handler = handler;
}

[[noreturn]] void ErrorContext::throwError(ErrorCode code, const std::string& file, int line,
                                          const std::string& function, const std::string& message,
                                          const std::string& context) {
    last_error = ErrorInfo(code, file, line, function, message, context);
    error_occurred = true;
    
    if (error_handler) {
        error_handler(last_error);
    }
    
    longjmp(jump_buffer, static_cast<int>(code));
}

void ErrorContext::checkCondition(bool condition, ErrorCode code, const std::string& file, int line,
                                 const std::string& function, const std::string& message) {
    if (!condition) {
        throwError(code, file, line, function, message);
    }
}

const ErrorInfo* ErrorContext::getLastError() const {
    return error_occurred ? &last_error : nullptr;
}

// 全局错误上下文
std::unique_ptr<ErrorContext> g_error_ctx;

// 初始化错误处理系统
bool error_init() {
    try {
        g_error_ctx = std::make_unique<ErrorContext>();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "错误处理系统初始化失败: " << e.what() << std::endl;
        return false;
    }
}

// 清理错误处理系统
void error_cleanup() {
    if (g_error_ctx) {
        g_error_ctx.reset();
    }
}

// file_operations.cpp
#include "error_framework.hpp"
#include <fstream>
#include <memory>

#line 100 "file_processing.dsl"

class FileOperations {
public:
    std::unique_ptr<std::ifstream> openFileSafely(const std::string& filename) {
        #line 105 "file_processing.dsl"
        auto file = std::make_unique<std::ifstream>(filename);
        
        #line 107 "file_processing.dsl"
        if (!file->is_open()) {
            #line 108 "file_processing.dsl"
            std::string context = "文件名: " + filename;
            ERROR_THROW_CTX(ErrorCode::FILE_NOT_FOUND, "无法打开文件", context);
        }
        
        #line 113 "file_processing.dsl"
        return file;
    }
};

#line 200 "memory_management.dsl"

class MemoryManagement {
public:
    template<typename T>
    std::unique_ptr<T> allocateMemorySafely(size_t count = 1) {
        #line 205 "memory_management.dsl"
        auto ptr = std::make_unique<T[]>(count);
        
        #line 207 "memory_management.dsl"
        if (!ptr) {
            #line 208 "memory_management.dsl"
            std::string context = "请求大小: " + std::to_string(count * sizeof(T)) + " 字节";
            ERROR_THROW_CTX(ErrorCode::OUT_OF_MEMORY, "内存分配失败", context);
        }
        
        #line 213 "memory_management.dsl"
        return std::unique_ptr<T>(ptr.release());
    }
};

// data_processing.cpp
#include "error_framework.hpp"
#include <cstring>

#line 300 "data_validation.dsl"

class DataProcessing {
public:
    int processData(const std::string& data) {
        #line 305 "data_validation.dsl"
        ERROR_CHECK(!data.empty(), ErrorCode::INVALID_INPUT, "数据为空");
        ERROR_CHECK(data.length() <= 1000000, ErrorCode::INVALID_INPUT, "数据长度超出限制");
        
        #line 309 "data_validation.dsl"
        // 模拟数据处理
        std::cout << "处理数据: " << data << " (长度: " << data.length() << ")\n";
        
        #line 315 "data_validation.dsl"
        return 0;
    }
};

// main_error.cpp
#include <iostream>
#include "error_framework.hpp"

class MainError {
public:
    int run() {
        // 初始化错误处理系统
        if (!error_init()) {
            std::cerr << "错误处理系统初始化失败\n";
            return 1;
        }
        
        std::cout << "错误处理框架示例\n";
        
        #line 400 "application_main.dsl"
        
        // 安全的文件操作
        ERROR_TRY
            FileOperations file_ops;
            auto file = file_ops.openFileSafely("nonexistent.txt");
            std::cout << "文件打开成功\n";
        ERROR_CATCH
            const ErrorInfo* error = g_error_ctx->getLastError();
            if (error) {
                std::cout << "捕获到错误: " << static_cast<int>(error->code) 
                          << " - " << error->message << std::endl;
            }
        ERROR_END
        
        #line 415 "application_main.dsl"
        
        // 安全的内存操作
        ERROR_TRY
            MemoryManagement mem_ops;
            auto ptr = mem_ops.allocateMemorySafely<int>(1024 * 1024 * 256);  // 1GB
            std::cout << "内存分配成功\n";
        ERROR_CATCH
            const ErrorInfo* error = g_error_ctx->getLastError();
            if (error) {
                std::cout << "捕获到错误: " << static_cast<int>(error->code) 
                          << " - " << error->message << std::endl;
                if (!error->context.empty()) {
                    std::cout << "错误上下文: " << error->context << std::endl;
                }
            }
        ERROR_END
        
        #line 430 "application_main.dsl"
        
        // 数据处理示例
        ERROR_TRY
            DataProcessing data_proc;
            data_proc.processData("Hello, World!");
        ERROR_CATCH
            const ErrorInfo* error = g_error_ctx->getLastError();
            if (error) {
                std::cout << "数据处理错误: " << static_cast<int>(error->code) 
                          << " - " << error->message << std::endl;
            }
        ERROR_END
        
        #line 440 "application_main.dsl"
        
        // 错误的输入处理
        ERROR_TRY
            DataProcessing data_proc2;
            data_proc2.processData("");
        ERROR_CATCH
            const ErrorInfo* error = g_error_ctx->getLastError();
            if (error) {
                std::cout << "输入验证错误: " << static_cast<int>(error->code) 
                          << " - " << error->message << std::endl;
            }
        ERROR_END
        
        #line 450 "application_main.dsl"
        
        std::cout << "程序执行完成\n";
        
        // 清理错误处理系统
        error_cleanup();
        return 0;
    }
};

int main() {
    MainError error_app;
    return error_app.run();
}
```

## 7. 现代 C++ 特性集成

### 7.1 与 std::source_location 集成

C++20 引入的 `std::source_location` 与 `#line` 指令的集成：

```cpp
// source_location_integration.hpp
#ifndef SOURCE_LOCATION_INTEGRATION_HPP
#define SOURCE_LOCATION_INTEGRATION_HPP

#include <iostream>
#include <string>
#include <source_location>  // C++20

// 现代调试宏，使用 std::source_location
#define MODERN_DEBUG_PRINT(msg) \
    modern_debug_print(msg, std::source_location::current())

#define MODERN_ERROR_PRINT(msg) \
    modern_error_print(msg, std::source_location::current())

// 传统调试宏，使用 __LINE__ 和 __FILE__
#define TRADITIONAL_DEBUG_PRINT(msg) \
    traditional_debug_print(msg, __FILE__, __LINE__, __func__)

#define TRADITIONAL_ERROR_PRINT(msg) \
    traditional_error_print(msg, __FILE__, __LINE__, __func__)

// 现代调试函数
void modern_debug_print(const std::string& message, 
                       const std::source_location& location = std::source_location::current()) {
    std::cout << "[DEBUG] " << location.file_name() << ":" 
              << location.line() << " in " << location.function_name() 
              << "(): " << message << std::endl;
}

void modern_error_print(const std::string& message, 
                       const std::source_location& location = std::source_location::current()) {
    std::cerr << "[ERROR] " << location.file_name() << ":" 
              << location.line() << " in " << location.function_name() 
              << "(): " << message << std::endl;
}

// 传统调试函数
void traditional_debug_print(const std::string& message, 
                            const char* file, int line, const char* function) {
    std::cout << "[DEBUG] " << file << ":" << line << " in " << function 
              << "(): " << message << std::endl;
}

void traditional_error_print(const std::string& message, 
                            const char* file, int line, const char* function) {
    std::cerr << "[ERROR] " << file << ":" << line << " in " << function 
              << "(): " << message << std::endl;
}

// 源位置信息类
class SourceLocationInfo {
private:
    std::string file_name;
    int line_number;
    std::string function_name;
    int column_number;

public:
    // 构造函数
    SourceLocationInfo(const std::source_location& loc = std::source_location::current())
        : file_name(loc.file_name()), line_number(loc.line()), 
          function_name(loc.function_name()), column_number(loc.column()) {}
    
    // 从传统宏构造
    SourceLocationInfo(const char* file, int line, const char* function, int column = 0)
        : file_name(file), line_number(line), function_name(function), column_number(column) {}
    
    // 获取器方法
    const std::string& getFileName() const { return file_name; }
    int getLineNumber() const { return line_number; }
    const std::string& getFunctionName() const { return function_name; }
    int getColumnNumber() const { return column_number; }
    
    // 转换为字符串
    std::string toString() const {
        return file_name + ":" + std::to_string(line_number) + 
               " in " + function_name + "()";
    }
    
    // 与 #line 指令集成
    void setLineDirective(int new_line, const std::string& new_file = "") {
        line_number = new_line;
        if (!new_file.empty()) {
            file_name = new_file;
        }
    }
};

#endif

// source_location_demo.cpp
#include "source_location_integration.hpp"
#include <vector>

#line 100 "modern_cpp_demo.dsl"

class ModernCppDemo {
public:
    void demonstrateSourceLocation() {
        #line 105 "modern_cpp_demo.dsl"
        std::cout << "=== 现代 C++ 源位置演示 ===\n";
        
        #line 107 "modern_cpp_demo.dsl"
        // 使用现代 source_location
        MODERN_DEBUG_PRINT("这是使用 std::source_location 的调试信息");
        MODERN_ERROR_PRINT("这是使用 std::source_location 的错误信息");
        
        #line 111 "modern_cpp_demo.dsl"
        // 使用传统宏
        TRADITIONAL_DEBUG_PRINT("这是使用传统宏的调试信息");
        TRADITIONAL_ERROR_PRINT("这是使用传统宏的错误信息");
        
        #line 115 "modern_cpp_demo.dsl"
        // 直接使用 SourceLocationInfo
        SourceLocationInfo info;
        std::cout << "当前源位置: " << info.toString() << std::endl;
        
        #line 119 "modern_cpp_demo.dsl"
        // 与 #line 指令集成
        info.setLineDirective(2000, "modified_file.cpp");
        std::cout << "修改后的源位置: " << info.toString() << std::endl;
    }
    
    void demonstrateFunctionTracking() {
        #line 200 "function_tracking.dsl"
        MODERN_DEBUG_PRINT("进入函数跟踪演示");
        
        #line 202 "function_tracking.dsl"
        performComplexOperation();
        
        #line 204 "function_tracking.dsl"
        MODERN_DEBUG_PRINT("退出函数跟踪演示");
    }
    
private:
    void performComplexOperation() {
        #line 300 "complex_operation.dsl"
        MODERN_DEBUG_PRINT("执行复杂操作");
        
        #line 302 "complex_operation.dsl"
        std::vector<int> data(1000);
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = static_cast<int>(i);
        }
        
        #line 307 "complex_operation.dsl"
        MODERN_DEBUG_PRINT("数据处理完成，大小: " + std::to_string(data.size()));
    }
};

// main_source_location.cpp
#include <iostream>
#include "source_location_integration.hpp"

class MainSourceLocation {
public:
    int run() {
        std::cout << "C++20 std::source_location 与 #line 指令集成演示\n";
        
        #line 400 "main_application.dsl"
        
        ModernCppDemo demo;
        demo.demonstrateSourceLocation();
        demo.demonstrateFunctionTracking();
        
        #line 405 "main_application.dsl"
        
        std::cout << "演示完成\n";
        return 0;
    }
};

int main() {
    MainSourceLocation app;
    return app.run();
}
```

### 7.2 模块化调试系统

使用现代 C++ 特性构建模块化调试系统：

```cpp
// modular_debug_system.hpp
#ifndef MODULAR_DEBUG_SYSTEM_HPP
#define MODULAR_DEBUG_SYSTEM_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <chrono>
#include <format>  // C++20
#include <source_location>  // C++20
#include <concepts>  // C++20

// 调试级别概念
template<typename T>
concept DebugLevelType = std::integral<T> && requires(T t) {
    { t >= 0 } -> std::convertible_to<bool>;
};

// 调试输出器概念
template<typename T>
concept DebugOutputter = requires(T t, const std::string& msg) {
    { t.output(msg) } -> std::same_as<void>;
};

// 基础调试输出器
class ConsoleOutputter {
public:
    void output(const std::string& message) {
        std::cout << message << std::endl;
    }
};

class FileOutputter {
private:
    std::ofstream file_stream;
    
public:
    FileOutputter(const std::string& filename) {
        file_stream.open(filename, std::ios::app);
    }
    
    void output(const std::string& message) {
        if (file_stream.is_open()) {
            file_stream << message << std::endl;
            file_stream.flush();
        }
    }
};

// 调试格式化器
class DebugFormatter {
public:
    template<typename... Args>
    static std::string formatMessage(const std::string& format, Args&&... args) {
#if __cplusplus >= 202002L
        return std::format(format, std::forward<Args>(args)...);
#else
        // C++17 及以下版本的简单实现
        std::ostringstream oss;
        oss << format;
        ((oss << " " << args), ...);
        return oss.str();
#endif
    }
    
    static std::string formatTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    
    static std::string formatSourceLocation(const std::source_location& loc) {
        return std::string(loc.file_name()) + ":" + std::to_string(loc.line());
    }
};

// 模块化调试器
template<DebugOutputter Outputter = ConsoleOutputter>
class ModularDebugger {
private:
    Outputter outputter;
    int debug_level;
    std::string module_name;

public:
    ModularDebugger(int level = 3, const std::string& module = "default")
        : debug_level(level), module_name(module) {}
    
    template<typename... Args>
    void debug(const std::string& format, Args&&... args) {
        if (debug_level >= 4) {
            log("DEBUG", format, std::forward<Args>(args)...);
        }
    }
    
    template<typename... Args>
    void info(const std::string& format, Args&&... args) {
        if (debug_level >= 3) {
            log("INFO", format, std::forward<Args>(args)...);
        }
    }
    
    template<typename... Args>
    void warning(const std::string& format, Args&&... args) {
        if (debug_level >= 2) {
            log("WARNING", format, std::forward<Args>(args)...);
        }
    }
    
    template<typename... Args>
    void error(const std::string& format, Args&&... args) {
        if (debug_level >= 1) {
            log("ERROR", format, std::forward<Args>(args)...);
        }
    }
    
private:
    template<typename... Args>
    void log(const std::string& level, const std::string& format, Args&&... args) {
        auto message = DebugFormatter::formatMessage(format, std::forward<Args>(args)...);
        auto timestamp = DebugFormatter::formatTimestamp();
        auto location = DebugFormatter::formatSourceLocation(std::source_location::current());
        
        std::string formatted_message = 
            std::format("[{}] [{}] [{}] [{}] {}", 
                       timestamp, level, location, module_name, message);
        
        outputter.output(formatted_message);
    }
};

// 与 #line 指令集成的调试器
class LineDirectiveAwareDebugger {
private:
    std::string current_file;
    int current_line;
    
public:
    LineDirectiveAwareDebugger() 
        : current_file(__FILE__), current_line(__LINE__) {}
    
    // 设置 #line 指令信息
    void setLineDirective(int line, const std::string& file = "") {
        current_line = line;
        if (!file.empty()) {
            current_file = file;
        }
    }
    
    // 获取当前源位置信息
    std::string getCurrentLocation() const {
        return current_file + ":" + std::to_string(current_line);
    }
    
    template<typename... Args>
    void debug(const std::string& format, Args&&... args) {
        auto message = DebugFormatter::formatMessage(format, std::forward<Args>(args)...);
        auto timestamp = DebugFormatter::formatTimestamp();
        
        std::string formatted_message = 
            std::format("[{}] [DEBUG] [{}] {}", 
                       timestamp, getCurrentLocation(), message);
        
        std::cout << formatted_message << std::endl;
    }
};

#endif

// modular_debug_demo.cpp
#include "modular_debug_system.hpp"

#line 100 "debug_module.dsl"

class DebugModuleDemo {
public:
    void demonstrateModularDebugging() {
        #line 105 "debug_module.dsl"
        std::cout << "=== 模块化调试系统演示 ===\n";
        
        #line 107 "debug_module.dsl"
        // 使用控制台输出器
        ModularDebugger<ConsoleOutputter> console_debugger(4, "console_module");
        console_debugger.debug("控制台调试信息: {}", 42);
        console_debugger.info("控制台信息: {}", "Hello World");
        
        #line 112 "debug_module.dsl"
        // 使用文件输出器
        ModularDebugger<FileOutputter> file_debugger(4, "file_module");
        // 注意：这里需要传递文件名给 FileOutputter 构造函数
        // 为了简化示例，我们使用控制台输出器
        
        #line 117 "debug_module.dsl"
        // 与 #line 指令集成的调试器
        LineDirectiveAwareDebugger line_debugger;
        line_debugger.debug("默认位置调试信息");
        
        #line 2000 "modified_location.cpp"
        line_debugger.setLineDirective(2000, "modified_location.cpp");
        line_debugger.debug("修改位置后的调试信息");
        
        #line 125 "debug_module.dsl"
        line_debugger.setLineDirective(125, "debug_module.dsl");
        line_debugger.debug("恢复位置后的调试信息");
    }
};

// main_modular_debug.cpp
#include <iostream>
#include "modular_debug_system.hpp"

class MainModularDebug {
public:
    int run() {
        std::cout << "现代 C++ 模块化调试系统演示\n";
        
        #line 300 "main_application.dsl"
        
        DebugModuleDemo demo;
        demo.demonstrateModularDebugging();
        
        #line 305 "main_application.dsl"
        
        std::cout << "演示完成\n";
        return 0;
    }
};

int main() {
    MainModularDebug app;
    return app.run();
}
```

## 8. 最佳实践

### 8.1 #line 指令使用规范

编写清晰、可维护的 `#line` 指令代码：

```cpp
// line_directive_best_practices.hpp
#ifndef LINE_DIRECTIVE_BEST_PRACTICES_HPP
#define LINE_DIRECTIVE_BEST_PRACTICES_HPP

#include <iostream>
#include <string>
#include <source_location>  // C++20

// 1. 使用有意义的行号和文件名
#define ORIGINAL_SOURCE_FILE "original_source.dsl"
#define GENERATED_SOURCE_FILE "generated_code.cpp"

// 2. 定义行号常量
namespace LineNumbers {
    constexpr int MATH_OPERATIONS_START = 100;
    constexpr int FILE_OPERATIONS_START = 200;
    constexpr int NETWORK_OPERATIONS_START = 300;
    constexpr int DATABASE_OPERATIONS_START = 400;
}

// 3. 使用宏封装复杂的 #line 指令
#define SET_LINE_INFO(line, file) \
    _Pragma(#line) \
    _Pragma(#file)

#define STRINGIFY(x) #x

// 4. 在代码生成工具中使用标准格式
#define GENERATED_CODE_HEADER(filename) \
    "/*\n" \
    " * 自动生成的代码\n" \
    " * 模块: " filename "\n" \
    " * 生成时间: " __DATE__ " " __TIME__ "\n" \
    " * 请勿手动修改\n" \
    " */\n\n"

// 5. 保持行号的连续性
class LineContinuityDemo {
public:
    void demonstrateLineContinuity() {
        std::cout << "原始行号: " << __LINE__ << std::endl;  // 假设为 30
        
#line 1000 "generated_source.cpp"
        std::cout << "生成代码行号: " << __LINE__ << std::endl;  // 1001
        
        // 保持连续性
#line 1002 "generated_source.cpp"
        std::cout << "连续行号: " << __LINE__ << std::endl;  // 1003
    }

    // 6. 使用条件编译保护
#ifdef CODE_GENERATION_TOOL
    #line 2000 "original_dsl_file.dsl"
    
    // DSL 解析生成的代码
    void generatedFunctionFromDsl() {
        #line 2005 "original_dsl_file.dsl"
        std::cout << "来自 DSL 的生成函数\n";
        
        #line 2007 "original_dsl_file.dsl"
        // 更多生成的代码...
    }
#endif

    // 7. 错误报告中的行号使用
    void errorReportingWithLineInfo() {
        int error_line = __LINE__;
        const char* error_file = __FILE__;
        
        std::cerr << "错误发生在 " << error_file << ":" << error_line << std::endl;
        
        // 在特定位置设置行号以提供更准确的错误信息
#line 5000 "error_reporting_module.cpp"
        // 错误处理代码...
    }

    // 8. 调试信息中的行号管理
#ifdef DEBUG
    #define DEBUG_LINE_INFO() \
        std::cerr << "调试信息 [" << __FILE__ << ":" << __LINE__ << "]\n"
#else
    #define DEBUG_LINE_INFO() do {} while(0)
#endif

    void debuggingWithLineInfo() {
        DEBUG_LINE_INFO();
        
#line 3000 "debug_module.cpp"
        // 调试相关代码
        DEBUG_LINE_INFO();
    }
};

// 9. 现代 C++ 风格的行号管理
class ModernLineManagement {
public:
    // 使用 std::source_location 管理行号信息
    void modernLineInfo(const std::source_location& location = std::source_location::current()) {
        std::cout << "现代行号信息: " << location.file_name() 
                  << ":" << location.line() << std::endl;
    }
    
    // 与 #line 指令结合使用
    void combinedLineManagement() {
        // 传统方式
        std::cout << "传统行号: " << __FILE__ << ":" << __LINE__ << std::endl;
        
        // 现代方式
        modernLineInfo();
        
        // #line 指令修改后
#line 7777 "modified_file.cpp"
        std::cout << "修改后传统行号: " << __FILE__ << ":" << __LINE__ << std::endl;
        modernLineInfo();
    }
};

// 10. 线程安全的行号管理
class ThreadSafeLineManagement {
private:
    thread_local std::string current_file;
    thread_local int current_line;
    
public:
    ThreadSafeLineManagement() 
        : current_file(__FILE__), current_line(__LINE__) {}
    
    void setLineDirective(int line, const std::string& file = "") {
        current_line = line;
        if (!file.empty()) {
            current_file = file;
        }
    }
    
    std::string getCurrentLocation() const {
        return current_file + ":" + std::to_string(current_line);
    }
    
    void threadSafeDebug(const std::string& message) {
        std::cout << "[" << getCurrentLocation() << "] " << message << std::endl;
    }
};

#endif

// main_best_practices.cpp
#include <iostream>
#include <thread>
#include <vector>
#include "line_directive_best_practices.hpp"

class MainBestPractices {
public:
    int run() {
        std::cout << "C++ 文件名和行信息最佳实践示例\n";
        
        LineContinuityDemo continuity_demo;
        continuity_demo.demonstrateLineContinuity();
        continuity_demo.errorReportingWithLineInfo();
        continuity_demo.debuggingWithLineInfo();
        
        ModernLineManagement modern_demo;
        modern_demo.combinedLineManagement();
        
        // 线程安全演示
        std::vector<std::thread> threads;
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back([i]() {
                ThreadSafeLineManagement thread_line_mgr;
                thread_line_mgr.setLineDirective(1000 + i, "thread_" + std::to_string(i) + ".cpp");
                thread_line_mgr.threadSafeDebug("线程安全调试信息");
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        return 0;
    }
};

int main() {
    MainBestPractices app;
    return app.run();
}
```

### 8.2 代码生成工具最佳实践

构建高质量的代码生成工具：

```cpp
// code_generator_best_practices.hpp
#ifndef CODE_GENERATOR_BEST_PRACTICES_HPP
#define CODE_GENERATOR_BEST_PRACTICES_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <format>  // C++20

// 代码生成器配置
struct CodeGenConfig {
    std::string input_file;
    std::string output_file;
    std::string template_file;
    bool preserve_line_numbers = true;
    bool generate_debug_info = true;
    std::string module_name = "unknown";
    std::string namespace_name = "";
};

// 生成的代码信息
struct GeneratedCodeInfo {
    std::string source_file;
    int source_line = 0;
    std::string generated_content;
    size_t content_length = 0;
    std::string comment;
};

// 代码生成器上下文
class CodeGenerator {
private:
    CodeGenConfig config;
    std::ofstream output_stream;
    int current_line = 1;
    std::string current_file;
    std::vector<GeneratedCodeInfo> code_blocks;
    
public:
    explicit CodeGenerator(const CodeGenConfig& cfg) : config(cfg) {
        current_file = cfg.output_file;
    }
    
    bool initialize();
    bool generateCode();
    void finalize();
    
    // 代码块管理
    bool addCodeBlock(const std::string& source_file, int source_line,
                     const std::string& content, const std::string& comment = "");
    
    // 行指令管理
    void writeLineDirective(const std::string& file, int line);
    
    // 格式化输出
    template<typename... Args>
    void writeFormattedCode(const std::string& format, Args&&... args);
    
    void writeComment(const std::string& comment);
    
    // 获取统计信息
    size_t getBlockCount() const { return code_blocks.size(); }
    const std::vector<GeneratedCodeInfo>& getCodeBlocks() const { return code_blocks; }
};

#endif

// code_generator_best_practices.cpp
#include "code_generator_best_practices.hpp"

bool CodeGenerator::initialize() {
    try {
        output_stream.open(config.output_file);
        if (!output_stream.is_open()) {
            std::cerr << "无法创建输出文件: " << config.output_file << std::endl;
            return false;
        }
        
        // 写入生成器头部信息
        output_stream << std::format(
            "/*\n"
            " * 自动生成的代码\n"
            " * 模块: {}\n"
            " * 源文件: {}\n"
            " * 生成时间: {} {}\n"
            " * 请勿手动修改\n"
            " */\n\n", 
            config.module_name, config.input_file, __DATE__, __TIME__);
        
        if (!config.namespace_name.empty()) {
            output_stream << "namespace " << config.namespace_name << " {\n\n";
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "初始化失败: " << e.what() << std::endl;
        return false;
    }
}

bool CodeGenerator::addCodeBlock(const std::string& source_file, int source_line,
                                const std::string& content, const std::string& comment) {
    try {
        GeneratedCodeInfo block;
        block.source_file = source_file;
        block.source_line = source_line;
        block.generated_content = content;
        block.content_length = content.length();
        block.comment = comment;
        
        code_blocks.push_back(block);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "添加代码块失败: " << e.what() << std::endl;
        return false;
    }
}

void CodeGenerator::writeLineDirective(const std::string& file, int line) {
    if (config.preserve_line_numbers) {
        output_stream << "#line " << line << " \"" << file << "\"\n";
        current_line++;
    }
}

template<typename... Args>
void CodeGenerator::writeFormattedCode(const std::string& format, Args&&... args) {
#if __cplusplus >= 202002L
    std::string formatted = std::format(format, std::forward<Args>(args)...);
    output_stream << formatted;
    // 计算写入的行数（简化实现）
    current_line += std::count(formatted.begin(), formatted.end(), '\n');
#else
    // C++17 及以下版本的实现
    output_stream << format;
    ((output_stream << args), ...);
    output_stream << "\n";
    current_line++;
#endif
}

void CodeGenerator::writeComment(const std::string& comment) {
    output_stream << "/* " << comment << " */\n";
    current_line++;
}

bool CodeGenerator::generateCode() {
    if (!initialize()) {
        return false;
    }
    
    try {
        // 写入包含文件
        writeComment("包含必要的头文件");
        writeLineDirective("generator_template.h", 10);
        writeFormattedCode("#include <iostream>\n");
        writeFormattedCode("#include <memory>\n\n");
        
        // 生成主函数
        writeComment("主函数实现");
        writeLineDirective("generator_template.h", 20);
        writeFormattedCode("int main() {{\n");
        
        writeLineDirective("generator_template.h", 25);
        writeFormattedCode("    std::cout << \"Hello from generated code!\\n\";\n");
        
        writeLineDirective("generator_template.h", 30);
        writeFormattedCode("    return 0;\n");
        writeFormattedCode("}}\n");
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "代码生成失败: " << e.what() << std::endl;
        return false;
    }
}

void CodeGenerator::finalize() {
    if (!config.namespace_name.empty()) {
        output_stream << "\n} // namespace " << config.namespace_name << "\n";
    }
    
    if (output_stream.is_open()) {
        output_stream.close();
    }
}

// main_generator.cpp
#include <iostream>
#include "code_generator_best_practices.hpp"

class MainGenerator {
public:
    int run(int argc, char* argv[]) {
        if (argc != 3) {
            std::cerr << "用法: " << argv[0] << " <input_file> <output_file>\n";
            return 1;
        }
        
        CodeGenConfig config;
        config.input_file = argv[1];
        config.output_file = argv[2];
        config.module_name = "example_module";
        config.namespace_name = "Generated";
        config.preserve_line_numbers = true;
        config.generate_debug_info = true;
        
        std::cout << "代码生成器启动\n";
        std::cout << "输入文件: " << config.input_file << std::endl;
        std::cout << "输出文件: " << config.output_file << std::endl;
        
        CodeGenerator generator(config);
        if (generator.generateCode()) {
            generator.finalize();
            std::cout << "代码生成完成，生成了 " 
                      << generator.getBlockCount() << " 个代码块\n";
        } else {
            std::cerr << "代码生成失败\n";
            return 1;
        }
        
        return 0;
    }
};

int main(int argc, char* argv[]) {
    MainGenerator generator_app;
    return generator_app.run(argc, argv);
}
```

## 9. 标准参考

### 9.1 C++ 标准版本演进

C++ 语言标准对 `#line` 指令的支持演进：

- C++98/C++03 (ISO/IEC 14882:1998/2003)：定义了基本的 `#line` 语法，行号限制为 32767
- C++11 (ISO/IEC 14882:2011)：扩展了行号限制到 2147483647
- C++14 (ISO/IEC 14882:2014)：保持 C++11 的 `#line` 支持
- C++17 (ISO/IEC 14882:2017)：保持之前的 `#line` 支持
- C++20 (ISO/IEC 14882:2020)：保持之前的 `#line` 支持，并引入 `std::source_location`
- C++23 (ISO/IEC 14882:2024)：继续支持现有的 `#line` 机制

### 9.2 相关标准条款

**C++23, C++20, C++17, C++14, C++11, C++98**：

- 15.7 Line control [cpp.line]：详细描述了 `#line` 指令的语法、语义和处理过程
- (C++20新增) std::source_location：描述了源位置信息类的使用

### 9.3 编译器实现差异

不同编译器对 `#line` 指令的实现可能存在细微差异：

```cpp
// standard_compliance.hpp
#ifndef STANDARD_COMPLIANCE_HPP
#define STANDARD_COMPLIANCE_HPP

#include <iostream>
#include <string>

// C++98/C++03 标准合规性检查
#if defined(__cplusplus) && __cplusplus == 199711L
    #define CPP98_COMPLIANT 1
    #pragma message "符合 C++98/C++03 标准"
    
    // C++98 行号限制测试
    #define CPP98_MAX_LINE 32767
    
    #if CPP98_MAX_LINE > 32767
        #pragma message "警告: 行号超出 C++98 限制"
    #endif
    
#else
    #define CPP98_COMPLIANT 0
    #pragma message "使用 C++98 之后的 C++ 标准"
#endif

// C++11 标准合规性检查
#if defined(__cplusplus) && __cplusplus >= 201103L
    #define CPP11_COMPLIANT 1
    #pragma message "符合 C++11 标准"
    
    // C++11 行号限制测试
    #define CPP11_MAX_LINE 2147483647
    
    #if CPP11_MAX_LINE > 32767
        #pragma message "支持扩展的行号范围"
    #endif
    
#else
    #define CPP11_COMPLIANT 0
    #pragma message "使用 C++11 之前的 C++ 标准"
#endif

// C++14 标准合规性检查
#if defined(__cplusplus) && __cplusplus >= 201402L
    #define CPP14_COMPLIANT 1
    #pragma message "符合 C++14 标准"
#else
    #define CPP14_COMPLIANT 0
    #pragma message "使用 C++14 之前的 C++ 标准"
#endif

// C++17 标准合规性检查
#if defined(__cplusplus) && __cplusplus >= 201703L
    #define CPP17_COMPLIANT 1
    #pragma message "符合 C++17 标准"
#else
    #define CPP17_COMPLIANT 0
    #pragma message "使用 C++17 之前的 C++ 标准"
#endif

// C++20 标准合规性检查
#if defined(__cplusplus) && __cplusplus >= 202002L
    #define CPP20_COMPLIANT 1
    #pragma message "符合 C++20 标准"
#else
    #define CPP20_COMPLIANT 0
    #pragma message "使用 C++20 之前的 C++ 标准"
#endif

// 编译器特定标准支持
#if defined(__GNUC__)
    #if __GNUC__ >= 4
        #define GCC_CPP_COMPLIANT 1
        #if CPP11_COMPLIANT
            #pragma message "GCC 完全支持 C++11"
        #endif
        #if CPP14_COMPLIANT
            #pragma message "GCC 完全支持 C++14"
        #endif
        #if CPP17_COMPLIANT
            #pragma message "GCC 完全支持 C++17"
        #endif
        #if CPP20_COMPLIANT
            #pragma message "GCC 完全支持 C++20"
        #endif
    #else
        #define GCC_CPP_COMPLIANT 0
        #pragma message "旧版 GCC，标准支持有限"
    #endif
#endif

#if defined(__clang__)
    #if __clang_major__ >= 3
        #define CLANG_CPP_COMPLIANT 1
        #if CPP11_COMPLIANT
            #pragma message "Clang 完全支持 C++11"
        #endif
        #if CPP14_COMPLIANT
            #pragma message "Clang 完全支持 C++14"
        #endif
        #if CPP17_COMPLIANT
            #pragma message "Clang 完全支持 C++17"
        #endif
        #if CPP20_COMPLIANT
            #pragma message "Clang 完全支持 C++20"
        #endif
    #else
        #define CLANG_CPP_COMPLIANT 0
        #pragma message "旧版 Clang，标准支持有限"
    #endif
#endif

#if defined(_MSC_VER)
    #if _MSC_VER >= 1900
        #define MSVC_CPP_COMPLIANT 1
        #if CPP11_COMPLIANT
            #pragma message "MSVC 支持 C++11"
        #endif
        #if CPP14_COMPLIANT
            #pragma message "MSVC 支持 C++14"
        #endif
        #if CPP17_COMPLIANT
            #pragma message "MSVC 支持 C++17"
        #endif
        #if CPP20_COMPLIANT
            #pragma message "MSVC 支持 C++20"
        #endif
    #else
        #define MSVC_CPP_COMPLIANT 0
        #pragma message "旧版 MSVC，标准支持有限"
    #endif
#endif

// 标准合规性测试类
class StandardCompliance {
public:
    static void testStandardCompliance() {
        std::cout << "C++ 标准合规性测试:\n";
        std::cout << "  C++98 支持: " << (CPP98_COMPLIANT ? "是" : "否") << std::endl;
        std::cout << "  C++11 支持: " << (CPP11_COMPLIANT ? "是" : "否") << std::endl;
        std::cout << "  C++14 支持: " << (CPP14_COMPLIANT ? "是" : "否") << std::endl;
        std::cout << "  C++17 支持: " << (CPP17_COMPLIANT ? "是" : "否") << std::endl;
        std::cout << "  C++20 支持: " << (CPP20_COMPLIANT ? "是" : "否") << std::endl;
        
        std::cout << "\n编译器支持:\n";
        std::cout << "  GCC 支持: " << (GCC_CPP_COMPLIANT ? "是" : "否") << std::endl;
        std::cout << "  Clang 支持: " << (CLANG_CPP_COMPLIANT ? "是" : "否") << std::endl;
        std::cout << "  MSVC 支持: " << (MSVC_CPP_COMPLIANT ? "是" : "否") << std::endl;
        
        // 行号测试
        std::cout << "\n行号测试:\n";
        std::cout << "  当前行号: " << __LINE__ << std::endl;
        
#line 10000
        std::cout << "  重置后行号: " << __LINE__ << std::endl;
        
        // 文件名测试
        std::cout << "  当前文件: " << __FILE__ << std::endl;
        
#line 20000 "test_file.cpp"
        std::cout << "  重置后文件: " << __FILE__ << std::endl;
    }
};

#endif

// main_standard.cpp
#include <iostream>
#include "standard_compliance.hpp"

class MainStandard {
public:
    int run() {
        std::cout << "C++ 标准合规性测试\n";
        StandardCompliance::testStandardCompliance();
        return 0;
    }
};

int main() {
    MainStandard app;
    return app.run();
}
```

## 10. 总结

### 10.1 核心要点

C++ 文件名和行信息控制通过 `#line` 指令提供了强大的源代码位置管理能力：

- 基本功能：修改预处理器维护的行号和文件名信息
- 语法形式：`#line lineno` 和 `#line lineno "filename"` 两种形式
- 预定义宏：`__LINE__` 和 `__FILE__` 提供当前位置信息
- 现代集成：与 `std::source_location` 等 C++20 特性协同工作
- 应用场景：代码生成工具、调试信息优化、错误报告改进

### 10.2 使用建议

为了有效使用 `#line` 指令，请遵循以下建议：

- 明确目的：清楚了解使用 `#line` 的具体目的和预期效果
- 保持一致性：在代码生成工具中保持行号和文件名的一致性
- 避免滥用：不要过度使用 `#line`，以免造成代码可读性问题
- 测试验证：验证 `#line` 对调试器和错误报告的实际影响
- 文档说明：为复杂的 `#line` 使用添加适当的注释说明
- 现代集成：在支持的环境中使用 `std::source_location` 等现代特性

### 10.3 发展趋势

C++ 文件名和行信息控制的发展趋势：

- 标准化完善：标准对 `#line` 指令的支持不断完善
- 工具集成：IDE 和调试工具对 `#line` 的支持更加完善
- 代码生成：代码生成工具对 `#line` 的使用更加普遍
- 调试优化：调试器对 `#line` 生成的调试信息处理更加智能
- 现代特性：与 `std::source_location` 等现代 C++ 特性的集成更加紧密

通过深入理解 `#line` 指令的原理和最佳实践，开发者可以构建更加高效、可维护的 C++ 程序，特别是在代码生成和调试优化方面发挥重要作用。现代 C++ 特性如 `std::source_location` 为源代码位置管理提供了更加类型安全和功能丰富的解决方案，值得在新项目中积极采用。
