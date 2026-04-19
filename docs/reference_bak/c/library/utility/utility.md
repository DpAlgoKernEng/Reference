# C语言工具库（Utility Library）

来源：cppreference.com

## 工具库概述

C语言包含各种工具库，提供广泛的功能支持。这些库可以大致分为两组：

1. **语言支持库**
2. **通用工具库**

## 语言支持库

语言支持库提供与语言特性密切交互并支持常见语言习语的函数。

### 类型支持

基本类型支持，如：
- `size_t` - 无符号整数类型，通常用于表示对象大小
- `NULL` - 空指针常量

### 动态内存管理

动态内存分配和管理函数，如：
- `malloc` - 动态分配内存
- `calloc` - 分配并清零内存
- `realloc` - 重新调整内存大小
- `free` - 释放内存

### 错误处理

错误检测和处理机制，如：
- `assert` - 断言宏，用于调试时检查条件

### 可变参数函数

支持接受任意数量参数的函数，通过以下宏实现：
- `va_start` - 初始化可变参数列表
- `va_arg` - 获取下一个可变参数
- `va_end` - 清理可变参数列表

## 扩展知识详解

### 一、语言支持库详解

#### 1. 类型支持和基本类型
```c
#include <stdio.h>
#include <stddef.h>

void type_support_demo() {
    // size_t - 用于表示大小和计数
    size_t array_size = sizeof(int) * 10;
    printf("Size of array: %zu bytes\n", array_size);
    
    // NULL指针
    int *ptr = NULL;
    if (ptr == NULL) {
        printf("Pointer is null\n");
    }
    
    // ptrdiff_t - 指针差值类型
    int arr[10];
    ptrdiff_t diff = &arr[9] - &arr[0];
    printf("Pointer difference: %td\n", diff);
}
```

#### 2. 动态内存管理
```c
#include <stdio.h>
#include <stdlib.h>

void dynamic_memory_demo() {
    // 基本内存分配
    int *numbers = (int*)malloc(10 * sizeof(int));
    if (numbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    // 初始化内存
    for (int i = 0; i < 10; i++) {
        numbers[i] = i * i;
    }
    
    // 重新分配内存
    int *resized = (int*)realloc(numbers, 20 * sizeof(int));
    if (resized == NULL) {
        free(numbers);  // 原内存需要释放
        fprintf(stderr, "Reallocation failed\n");
        return;
    }
    numbers = resized;
    
    // 使用calloc分配并清零
    int *cleared = (int*)calloc(5, sizeof(int));
    if (cleared != NULL) {
        printf("Calloc allocated and cleared 5 integers\n");
        free(cleared);
    }
    
    // 释放内存
    free(numbers);
}
```

#### 3. 错误处理和断言
```c
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void error_handling_demo(int value) {
    // 使用断言进行调试检查
    assert(value >= 0);  // 如果value < 0，程序会终止
    
    // 运行时错误检查
    if (value < 0) {
        fprintf(stderr, "Error: Negative value not allowed\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Processing value: %d\n", value);
}

// 在发布版本中禁用断言
#ifdef NDEBUG
    // 断言被禁用
#else
    // 断言启用
#endif
```

#### 4. 可变参数函数
```c
#include <stdio.h>
#include <stdarg.h>

// 计算平均值的可变参数函数
double average(int count, ...) {
    va_list args;
    double sum = 0.0;
    
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        double value = va_arg(args, double);
        sum += value;
    }
    va_end(args);
    
    return count > 0 ? sum / count : 0.0;
}

// 格式化输出函数
void custom_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void variadic_demo() {
    double avg = average(4, 10.5, 20.0, 30.5, 40.0);
    printf("Average: %.2f\n", avg);
    
    custom_printf("Formatted output: %d, %s, %.2f\n", 42, "hello", 3.14);
}
```

### 二、通用工具库详解

#### 1. 程序工具
```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// 程序终止处理函数
void cleanup_handler(void) {
    printf("Cleanup handler called\n");
    // 执行清理工作
}

// 信号处理函数
void signal_handler(int sig) {
    printf("Received signal: %d\n", sig);
    exit(EXIT_SUCCESS);
}

void program_utilities_demo() {
    // 注册程序终止处理函数
    atexit(cleanup_handler);
    
    // 设置信号处理器
    signal(SIGINT, signal_handler);
    
    // 执行系统命令
    int result = system("echo Hello from system command");
    if (result == -1) {
        printf("Failed to execute system command\n");
    }
    
    // 发送信号
    // raise(SIGINT);  // 取消注释会发送中断信号
}
```

#### 2. 日期和时间处理
```c
#include <stdio.h>
#include <time.h>

void date_time_demo() {
    // 获取当前时间
    time_t rawtime;
    time(&rawtime);
    printf("Current time: %ld\n", rawtime);
    
    // 转换为本地时间
    struct tm *local_time = localtime(&rawtime);
    printf("Local time: %04d-%02d-%02d %02d:%02d:%02d\n",
           local_time->tm_year + 1900,
           local_time->tm_mon + 1,
           local_time->tm_mday,
           local_time->tm_hour,
           local_time->tm_min,
           local_time->tm_sec);
    
    // CPU时钟计时
    clock_t start = clock();
    
    // 执行一些操作
    for (int i = 0; i < 1000000; i++) {
        // 模拟工作
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU time used: %.6f seconds\n", cpu_time);
    
    // 格式化时间输出
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
    printf("Formatted time: %s\n", buffer);
}
```

### 三、实际应用示例

#### 1. 内存管理包装器
```c
#include <stdio.h>
#include <stdlib.h>

// 安全的内存分配函数
void* safe_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed for %zu bytes\n", size);
        exit(EXIT_FAILURE);
    }
    
    return ptr;
}

// 安全的内存重分配函数
void* safe_realloc(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        fprintf(stderr, "Memory reallocation failed for %zu bytes\n", size);
        free(ptr);  // 释放原内存
        exit(EXIT_FAILURE);
    }
    
    return new_ptr;
}

void memory_wrapper_demo() {
    int *array = (int*)safe_malloc(10 * sizeof(int));
    
    // 使用数组...
    for (int i = 0; i < 10; i++) {
        array[i] = i * i;
    }
    
    // 扩展数组
    array = (int*)safe_realloc(array, 20 * sizeof(int));
    
    // 继续使用...
    free(array);
}
```

#### 2. 错误处理框架
```c
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

// 错误处理上下文
static jmp_buf error_context;
static int error_code = 0;

// 错误处理函数
void handle_error(int code, const char* message) {
    error_code = code;
    fprintf(stderr, "Error %d: %s\n", code, message);
    longjmp(error_context, code);
}

// 可能出错的函数
void risky_operation(int value) {
    if (value < 0) {
        handle_error(1, "Negative value not allowed");
    }
    
    if (value > 1000) {
        handle_error(2, "Value too large");
    }
    
    printf("Processing value: %d\n", value);
}

void error_framework_demo() {
    int result = setjmp(error_context);
    if (result == 0) {
        // 正常执行
        risky_operation(-5);  // 这会触发错误处理
    } else {
        // 错误处理返回
        printf("Recovered from error code: %d\n", result);
    }
}
```

#### 3. 时间测量工具
```c
#include <stdio.h>
#include <time.h>

// 简单的性能计时器
typedef struct {
    clock_t start;
    clock_t end;
    double elapsed;
} timer_t;

void timer_start(timer_t* timer) {
    timer->start = clock();
}

void timer_stop(timer_t* timer) {
    timer->end = clock();
    timer->elapsed = ((double)(timer->end - timer->start)) / CLOCKS_PER_SEC;
}

void performance_demo() {
    timer_t timer;
    
    timer_start(&timer);
    
    // 执行需要计时的操作
    long sum = 0;
    for (long i = 0; i < 10000000; i++) {
        sum += i;
    }
    
    timer_stop(&timer);
    
    printf("Sum: %ld\n", sum);
    printf("Elapsed time: %.6f seconds\n", timer.elapsed);
}
```

## 通用工具库

### 程序工具

程序控制和环境交互功能：
- `abort` - 异常终止程序
- `atexit` - 注册程序终止时调用的函数
- `exit` - 正常终止程序
- `system` - 执行系统命令
- `getenv` - 获取环境变量

### 信号处理

进程间通信和异常处理：
- `signal` - 设置信号处理器
- `raise` - 发送信号
- `sig_atomic_t` - 原子信号类型

### 日期和时间

时间相关功能：
- `time` - 获取当前日历时间
- `clock` - 获取处理器时间
- `difftime` - 计算时间差
- `strftime` - 格式化时间输出

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C++工具库文档] | C++对应文档 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=c/utility&oldid=130686>
- 最后修改时间：2021年6月24日 02:56
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C语言工具库为系统编程提供了坚实的基础。从基本的类型支持到复杂的动态内存管理，从错误处理到时间日期操作，这些工具构成了C程序开发的核心。正确使用这些库函数，能够编写出高效、可靠和可维护的C程序。随着C语言标准的演进，工具库也在不断完善，提供了更多安全和便利的功能。掌握这些工具的使用方法和最佳实践，是成为优秀C程序员的必经之路。