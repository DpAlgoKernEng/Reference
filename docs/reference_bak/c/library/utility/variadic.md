# C语言可变参数函数（Variadic Functions）

来源：cppreference.com

## 可变参数函数概述

可变参数函数是指可以接受可变数量参数的函数（如`printf`）。

可变参数函数的声明使用省略号作为最后一个参数，例如：
```c
int printf(const char* format, ...);
```

## 可变参数处理工具

访问函数体中的可变参数需要使用以下库工具：

| 类别 | 工具 | 说明 |
|------|------|------|
| **类型** | `va_list` | 保存`va_start`、`va_arg`、`va_end`和`va_copy`所需信息（typedef） |
| **宏** | `va_start` | 启用对可变参数函数参数的访问（函数宏） |
| **宏** | `va_arg` | 访问下一个可变参数函数参数（函数宏） |
| **宏** | `va_copy`(C99) | 复制可变参数函数参数（函数宏） |
| **宏** | `va_end` | 结束可变参数函数参数的遍历（函数宏） |

## 扩展知识详解

### 一、可变参数函数工作机制

#### 1. 基本使用模式
```c
#include <stdarg.h>
#include <stdio.h>

// 可变参数函数的基本结构
int sum_integers(int count, ...) {
    va_list args;
    int sum = 0;
    
    // 初始化参数列表
    va_start(args, count);
    
    // 逐个访问参数
    for (int i = 0; i < count; i++) {
        int value = va_arg(args, int);
        sum += value;
    }
    
    // 清理参数列表
    va_end(args);
    
    return sum;
}

void demonstrate_basic_usage(void) {
    int result = sum_integers(4, 10, 20, 30, 40);
    printf("Sum: %d\n", result);  // 输出: Sum: 100
}
```

### 二、参数类型和提升规则

#### 1. 整数类型提升
```c
#include <stdarg.h>
#include <stdio.h>

void integer_promotion_demo(int count, ...) {
    va_list args;
    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        // char和short会被提升为int
        int value = va_arg(args, int);
        printf("Value %d: %d\n", i, value);
    }
    
    va_end(args);
}

void test_integer_promotion(void) {
    char c = 'A';
    short s = 100;
    int i = 1000;
    
    // 传递时都会被提升为int
    integer_promotion_demo(3, c, s, i);
}
```

#### 2. 浮点类型提升
```c
#include <stdarg.h>
#include <stdio.h>

void float_promotion_demo(int count, ...) {
    va_list args;
    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        // float会被提升为double
        double value = va_arg(args, double);
        printf("Float value %d: %f\n", i, value);
    }
    
    va_end(args);
}

void test_float_promotion(void) {
    float f = 3.14f;
    double d = 2.718;
    
    // float被提升为double
    float_promotion_demo(2, f, d);
}
```

### 三、va_copy的使用（C99起）

```c
#include <stdarg.h>
#include <stdio.h>

void print_args_twice(int count, ...) {
    va_list args1, args2;
    
    // 初始化第一个参数列表
    va_start(args1, count);
    
    // 复制参数列表
    va_copy(args2, args1);
    
    // 第一次打印
    printf("First pass:\n");
    for (int i = 0; i < count; i++) {
        int value = va_arg(args1, int);
        printf("  %d\n", value);
    }
    
    // 第二次打印（使用复制的参数列表）
    printf("Second pass:\n");
    for (int i = 0; i < count; i++) {
        int value = va_arg(args2, int);
        printf("  %d\n", value);
    }
    
    // 清理
    va_end(args1);
    va_end(args2);
}

void demonstrate_va_copy(void) {
    print_args_twice(3, 10, 20, 30);
}
```

### 四、安全的可变参数处理

#### 1. 错误检查和边界处理
```c
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING
} arg_type_t;

void safe_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    for (const char* p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++;  // 跳过'%'
            switch (*p) {
                case 'd':
                    {
                        int value = va_arg(args, int);
                        printf("%d", value);
                    }
                    break;
                case 'f':
                    {
                        double value = va_arg(args, double);
                        printf("%f", value);
                    }
                    break;
                case 's':
                    {
                        char* value = va_arg(args, char*);
                        if (value != NULL) {
                            printf("%s", value);
                        } else {
                            printf("(null)");
                        }
                    }
                    break;
                default:
                    printf("%%");  // 打印原始%
                    if (*p != '\0') {
                        printf("%c", *p);
                    }
                    break;
            }
        } else {
            putchar(*p);
        }
    }
    
    va_end(args);
}

void demonstrate_safe_printf(void) {
    safe_printf("Integer: %d, Float: %f, String: %s\n", 
                42, 3.14159, "Hello");
}
```

#### 2. 参数计数验证
```c
#include <stdarg.h>
#include <stdio.h>

int sum_with_validation(int expected_count, int actual_count, ...) {
    if (expected_count != actual_count) {
        fprintf(stderr, "Expected %d arguments, got %d\n", 
                expected_count, actual_count);
        return -1;
    }
    
    va_list args;
    va_start(args, actual_count);
    
    int sum = 0;
    for (int i = 0; i < actual_count; i++) {
        int value = va_arg(args, int);
        sum += value;
    }
    
    va_end(args);
    return sum;
}

void demonstrate_validation(void) {
    int result = sum_with_validation(3, 3, 10, 20, 30);
    if (result >= 0) {
        printf("Sum: %d\n", result);
    }
}
```

### 五、实际应用示例

#### 1. 日志系统实现
```c
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} log_level_t;

void log_message(log_level_t level, const char* format, ...) {
    // 获取当前时间
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    // 打印时间戳和日志级别
    char level_str[10];
    switch (level) {
        case LOG_DEBUG:   strcpy(level_str, "DEBUG");   break;
        case LOG_INFO:    strcpy(level_str, "INFO");    break;
        case LOG_WARNING: strcpy(level_str, "WARNING"); break;
        case LOG_ERROR:   strcpy(level_str, "ERROR");   break;
    }
    
    printf("[%04d-%02d-%02d %02d:%02d:%02d] [%s] ",
           tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
           tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
           level_str);
    
    // 处理可变参数
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}

void demonstrate_logging(void) {
    log_message(LOG_INFO, "Application started with PID %d", getpid());
    log_message(LOG_WARNING, "Configuration file %s not found", "config.ini");
    log_message(LOG_ERROR, "Failed to connect to database: %s", "timeout");
}
```

#### 2. 格式化字符串构建器
```c
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* format_string(const char* format, ...) {
    va_list args1, args2;
    va_start(args1, format);
    va_copy(args2, args1);
    
    // 第一次调用计算所需缓冲区大小
    int size = vsnprintf(NULL, 0, format, args1);
    va_end(args1);
    
    if (size < 0) {
        va_end(args2);
        return NULL;
    }
    
    // 分配内存
    char* buffer = malloc(size + 1);
    if (buffer == NULL) {
        va_end(args2);
        return NULL;
    }
    
    // 第二次调用实际格式化
    vsnprintf(buffer, size + 1, format, args2);
    va_end(args2);
    
    return buffer;
}

void demonstrate_string_formatting(void) {
    char* message = format_string("User %s (ID: %d) logged in at %s", 
                                  "John Doe", 12345, "2025-02-09 10:30:00");
    if (message != NULL) {
        printf("Formatted message: %s\n", message);
        free(message);
    }
}
```

#### 3. 多类型参数处理
```c
#include <stdarg.h>
#include <stdio.h>

typedef enum {
    ARG_INT,
    ARG_DOUBLE,
    ARG_STRING,
    ARG_CHAR
} arg_kind_t;

void print_mixed_args(int count, ...) {
    va_list args;
    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        arg_kind_t kind = va_arg(args, arg_kind_t);
        
        switch (kind) {
            case ARG_INT:
                {
                    int value = va_arg(args, int);
                    printf("Integer: %d\n", value);
                }
                break;
            case ARG_DOUBLE:
                {
                    double value = va_arg(args, double);
                    printf("Double: %f\n", value);
                }
                break;
            case ARG_STRING:
                {
                    char* value = va_arg(args, char*);
                    printf("String: %s\n", value ? value : "(null)");
                }
                break;
            case ARG_CHAR:
                {
                    int value = va_arg(args, int);  // char被提升为int
                    printf("Character: %c\n", value);
                }
                break;
        }
    }
    
    va_end(args);
}

void demonstrate_mixed_args(void) {
    print_mixed_args(4,
                     ARG_INT, 42,
                     ARG_STRING, "Hello",
                     ARG_DOUBLE, 3.14159,
                     ARG_CHAR, 'X');
}
```

### 六、现代C特性集成

#### 1. 与v*系列函数配合使用
```c
#include <stdarg.h>
#include <stdio.h>

// 包装printf系列函数
void my_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void my_fprintf(FILE* stream, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);
}

void my_sprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
}

void demonstrate_vfunctions(void) {
    my_printf("Direct output: %d %s\n", 42, "test");
    
    char buffer[256];
    my_sprintf(buffer, "Buffered: %f", 3.14159);
    printf("%s\n", buffer);
}
```

#### 2. 错误处理集成
```c
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void fatal_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    fprintf(stderr, "FATAL ERROR: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    
    va_end(args);
    exit(EXIT_FAILURE);
}

void warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    fprintf(stderr, "WARNING: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    
    va_end(args);
}

void demonstrate_error_handling(void) {
    int critical_value = -1;
    if (critical_value < 0) {
        fatal_error("Invalid critical value: %d", critical_value);
    }
    
    warning("This is just a warning message");
}
```

## 示例代码详解

### 原始示例分析
```c
#include <stdarg.h>
#include <stdio.h>

void simple_printf(const char* fmt, ...) {
    va_list args;
    
    for (va_start(args, fmt); *fmt != '\0'; ++fmt) {
        switch(*fmt) {
            case 'd':
                {
                    int i = va_arg(args, int);
                    printf("%d\n", i);
                    break;
                }
            case 'c':
                {
                    // char变量会被提升为int
                    // C中的字符字面量本身就是int
                    int c = va_arg(args, int);
                    printf("%c\n", c);
                    break;
                }
            case 'f':
                {
                    double d = va_arg(args, double);
                    printf("%f\n", d);
                    break;
                }
            default:
                puts("Unknown formatter!");
                goto END;
        }
    }
END:
    va_end(args);
}

int main(void) {
    simple_printf("dcff", 3, 'a', 1.969, 42.5);
}
```

### 输出结果
```
3
a
1.969000
42.500000
```

### 代码要点解析

1. **参数初始化**：`va_start(args, fmt)`初始化参数列表
2. **参数访问**：`va_arg(args, type)`按类型访问参数
3. **参数清理**：`va_end(args)`清理参数列表
4. **类型提升**：`char`和`float`在传递时会被提升
5. **控制流**：使用`goto`跳出循环进行清理

## 标准参考

### C23标准（ISO/IEC 9899:2024）
- 7.16 可变参数 <stdarg.h>

### C17标准（ISO/IEC 9899:2018）
- 7.16 可变参数 <stdarg.h>

### C11标准（ISO/IEC 9899:2011）
- 7.16 可变参数 <stdarg.h> (p: 269-272)

### C99标准（ISO/IEC 9899:1999）
- 7.15 可变参数 <stdarg.h> (p: 249-252)

### C89/C90标准（ISO/IEC 9899:1990）
- 4.8 可变参数 <stdarg.h>

## 相关链接

| 内容 | 说明 |
|------|------|
| [C++可变参数函数文档] | C++对应文档 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=c/variadic&oldid=180120>
- 最后修改时间：2025年2月4日 01:22
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C语言可变参数函数为程序提供了灵活的参数处理能力。正确理解和使用`va_list`、`va_start`、`va_arg`、`va_copy`和`va_end`是掌握C语言高级编程技能的关键。类型提升规则、参数安全处理和现代C特性集成都是实际开发中需要注意的重要方面。通过合理设计可变参数函数，可以构建更加灵活和强大的C应用程序。从简单的数值处理到复杂的日志系统，可变参数函数在系统编程中发挥着重要作用。