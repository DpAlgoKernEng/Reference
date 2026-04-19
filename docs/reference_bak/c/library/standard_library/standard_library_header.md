# C标准库头文件（C Standard Library Headers）

来源：cppreference.com

## 标准库头文件概览

**C标准库**的接口由以下头文件集合定义：

| 头文件 | 标准 | 用途 |
|--------|------|------|
| `<assert.h>` | C89 | 条件编译宏，用于调试断言 |
| `<complex.h>` | C99 | 复数运算支持 |
| `<ctype.h>` | C89 | 字符类型判断函数 |
| `<errno.h>` | C89 | 错误条件报告宏 |
| `<fenv.h>` | C99 | 浮点环境控制 |
| `<float.h>` | C89 | 浮点类型限制 |
| `<inttypes.h>` | C99 | 整数类型格式转换 |
| `<iso646.h>` | C95 | 运算符替代拼写 |
| `<limits.h>` | C89 | 整数类型范围 |
| `<locale.h>` | C89 | 本地化工具 |
| `<math.h>` | C89 | 常用数学函数 |
| `<setjmp.h>` | C89 | 非局部跳转 |
| `<signal.h>` | C89 | 信号处理 |
| `<stdalign.h>` | C11* | 对齐控制宏（C23已弃用） |
| `<stdarg.h>` | C89 | 可变参数处理 |
| `<stdatomic.h>` | C11 | 原子操作 |
| `<stdbit.h>` | C23 | 位操作宏 |
| `<stdbool.h>` | C99* | 布尔类型宏（C23已弃用） |
| `<stdckdint.h>` | C23 | 检查整数运算溢出 |
| `<stddef.h>` | C89 | 通用宏定义 |
| `<stdint.h>` | C99 | 固定宽度整数类型 |
| `<stdio.h>` | C89 | 输入输出操作 |
| `<stdlib.h>` | C89 | 通用工具函数 |
| `<stdmchar.h>` | C29 | 文本转码（未来版本） |
| `<stdnoreturn.h>` | C11* | 不返回函数宏（C23已弃用） |
| `<string.h>` | C89 | 字符串处理 |
| `<tgmath.h>` | C99 | 类型泛型数学函数 |
| `<threads.h>` | C11 | 线程库 |
| `<time.h>` | C89 | 时间日期工具 |
| `<uchar.h>` | C11 | UTF-16/UTF-32字符工具 |
| `<wchar.h>` | C95 | 扩展多字节和宽字符工具 |
| `<wctype.h>` | C95 | 宽字符类型判断 |

> * 表示该头文件在后续标准中被弃用

---

## 头文件详细说明

### 1. 调试和错误处理
```c
// <assert.h> - 断言宏
#include <assert.h>
assert(ptr != NULL);  // 调试时检查条件

// <errno.h> - 错误码
#include <errno.h>
#include <stdio.h>
extern int errno;
FILE* file = fopen("nonexistent.txt", "r");
if (file == NULL) {
    printf("Error: %d\n", errno);  // 打印错误码
}
```

### 2. 数据类型和限制
```c
// <limits.h> - 整数类型限制
#include <limits.h>
printf("int max: %d\n", INT_MAX);
printf("int min: %d\n", INT_MIN);

// <float.h> - 浮点类型限制
#include <float.h>
printf("float precision: %d\n", FLT_DIG);

// <stdint.h> - 固定宽度整数（C99）
#include <stdint.h>
int32_t id = 123456789;
uint8_t byte = 255;
```

### 3. 字符和字符串处理
```c
// <ctype.h> - 字符类型判断
#include <ctype.h>
if (isalpha('A')) {
    printf("A is alphabetic\n");
}
if (isdigit('5')) {
    printf("5 is digit\n");
}

// <string.h> - 字符串操作
#include <string.h>
char str1[20] = "Hello";
char str2[] = "World";
strcpy(str1, "Hello");
strcat(str1, " ");
strcat(str1, str2);
printf("Combined: %s\n", str1);
```

### 4. 数学运算
```c
// <math.h> - 数学函数
#include <math.h>
double result = sqrt(16.0);
printf("sqrt(16) = %.2f\n", result);

// <complex.h> - 复数运算（C99）
#include <complex.h>
double complex z = 1.0 + 2.0*I;
double complex conjugate = conj(z);
```

### 5. 输入输出
```c
// <stdio.h> - 标准I/O
#include <stdio.h>
printf("Hello, World!\n");
fprintf(stderr, "Error message\n");

// 文件操作
FILE* file = fopen("data.txt", "w");
if (file != NULL) {
    fprintf(file, "Data content\n");
    fclose(file);
}
```

### 6. 内存管理
```c
// <stdlib.h> - 内存管理
#include <stdlib.h>
int* array = (int*)malloc(10 * sizeof(int));
if (array != NULL) {
    for (int i = 0; i < 10; i++) {
        array[i] = i * i;
    }
    free(array);  // 记得释放内存
}
```

### 7. 现代C特性（C11/C23）

#### 原子操作（C11）
```c
#include <stdatomic.h>
#include <threads.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

int worker(void* arg) {
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&counter, 1);
    }
    return 0;
}
```

#### 线程支持（C11）
```c
#include <threads.h>

int thread_func(void* arg) {
    printf("Thread running\n");
    return 0;
}

int main() {
    thrd_t thread;
    thrd_create(&thread, thread_func, NULL);
    thrd_join(thread, NULL);
    return 0;
}
```

#### 对齐控制（C11）
```c
#include <stdalign.h>

// 指定16字节对齐
_Alignas(16) char buffer[256];

// 查询对齐要求
size_t alignment = _Alignof(struct MyStruct);
```

#### 位操作（C23）
```c
#include <stdbit.h>

// 计算前导零位数
unsigned int value = 8;  // 二进制: 1000
int leading_zeros = stdc_leading_zeros_ui(value);
```

#### 检查整数溢出（C23）
```c
#include <stdckdint.h>

int a = INT_MAX;
int b = 1;
int result;
if (ckd_add(&result, a, b)) {
    printf("Overflow detected!\n");
} else {
    printf("Result: %d\n", result);
}
```

---

## 特性测试宏（C23起）

自C23起，各头文件定义了对应的特性测试宏：

| 序号 | 头文件 | 宏名称 | 值 |
|------|--------|--------|-----|
| 1 | `<assert.h>` | `__STDC_VERSION_ASSERT_H__` | 202311L |
| 2 | `<complex.h>` | `__STDC_VERSION_COMPLEX_H__` | 202311L |
| 7 | `<inttypes.h>` | `__STDC_VERSION_INTTYPES_H__` | 202311L |
| 11 | `<math.h>` | `__STDC_VERSION_MATH_H__` | 202311L |
| 16 | `<stdatomic.h>` | `__STDC_VERSION_STDATOMIC_H__` | 202311L |
| 17 | `<stdbit.h>` | `__STDC_VERSION_STDBIT_H__` | 202311L |
| 19 | `<stdckdint.h>` | `__STDC_VERSION_STDCKDINT_H__` | 202311L |

### 使用示例
```c
#include <stdbit.h>

#if defined(__STDC_VERSION_STDBIT_H__) && __STDC_VERSION_STDBIT_H__ >= 202311L
    // 使用C23的位操作函数
    unsigned int count = stdc_count_ones_ui(255);
#else
    // 使用兼容实现
    unsigned int count = count_ones_legacy(255);
#endif
```

---

## 扩展知识详解

### 一、头文件使用最佳实践

#### 1. 最小包含原则
```c
// 只包含需要的头文件
#include <stdio.h>    // 需要printf
#include <stdlib.h>   // 需要malloc/free

// 避免包含不必要的头文件
// #include <math.h>  // 如果不需要数学函数就不要包含
```

#### 2. 头文件保护
```c
// myheader.h
#ifndef MYHEADER_H
#define MYHEADER_H

// 头文件内容
void my_function(void);

#endif  // MYHEADER_H
```

### 二、各版本C标准的演进

#### C89/C90基础功能
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char* str = (char*)malloc(20);
    strcpy(str, "Hello C89");
    printf("%s\n", str);
    free(str);
    return 0;
}
```

#### C99新增特性
```c
// C99引入的特性
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

int main() {
    bool flag = true;           // 布尔类型
    int32_t id = 123456;        // 固定宽度整数
    printf("ID: %"PRId32"\n", id);  // 平台无关的格式化
    return 0;
}
```

#### C11多线程支持
```c
#include <threads.h>
#include <stdio.h>

int thread_func(void* arg) {
    printf("Hello from thread!\n");
    return 0;
}

int main() {
    thrd_t thread;
    thrd_create(&thread, thread_func, NULL);
    thrd_join(thread, NULL);
    return 0;
}
```

#### C23现代化特性
```c
#include <stdckdint.h>
#include <stdbit.h>

void modern_c_example() {
    // 检查溢出
    int a = INT_MAX;
    int b = 1;
    int result;
    if (ckd_add(&result, a, b)) {
        printf("Overflow detected\n");
    }
    
    // 位操作
    unsigned value = 255;
    printf("Ones count: %d\n", stdc_count_ones_ui(value));
}
```

### 三、常见使用模式

#### 1. 安全字符串操作（C11扩展）
```c
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>

void safe_string_copy() {
    char src[] = "Hello, World!";
    char dest[50];
    
    // 安全的字符串复制
    strcpy_s(dest, sizeof(dest), src);
    printf("Copied: %s\n", dest);
}
```

#### 2. 类型泛型数学（C99）
```c
#include <tgmath.h>

void generic_math_example() {
    float f = 4.0f;
    double d = 9.0;
    long double ld = 16.0L;
    
    // 自动选择合适的函数
    float sqrt_f = sqrt(f);      // 调用sqrtf
    double sqrt_d = sqrt(d);     // 调用sqrt
    long double sqrt_ld = sqrt(ld); // 调用sqrtl
}
```

#### 3. 宽字符处理（C95）
```c
#include <wchar.h>
#include <locale.h>

void wide_character_example() {
    setlocale(LC_ALL, "");  // 设置本地化
    
    wchar_t wide_str[] = L"Hello, 世界";
    wprintf(L"Wide string: %ls\n", wide_str);
    
    size_t len = wcslen(wide_str);
    wprintf(L"Length: %zu\n", len);
}
```

### 四、内存管理模式

#### 1. 基础内存管理
```c
#include <stdlib.h>

void basic_memory_management() {
    // 动态分配数组
    int* array = malloc(10 * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    // 使用内存
    for (int i = 0; i < 10; i++) {
        array[i] = i * i;
    }
    
    // 释放内存
    free(array);
}
```

#### 2. 重新分配内存
```c
void resize_array() {
    int* array = malloc(5 * sizeof(int));
    // ... 使用数组 ...
    
    // 重新分配更大空间
    array = realloc(array, 10 * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Reallocation failed\n");
        return;
    }
    
    // ... 继续使用 ...
    free(array);
}
```

---

## 实际应用示例

### 一、完整的文件处理程序
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int process_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s: %s\n", 
                filename, strerror(errno));
        return -1;
    }
    
    char buffer[1024];
    int line_count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_count++;
        // 处理每一行
        printf("Line %d: %s", line_count, buffer);
    }
    
    fclose(file);
    return line_count;
}
```

### 二、现代C多线程应用
```c
#include <threads.h>
#include <stdatomic.h>
#include <stdio.h>

atomic_int global_counter = ATOMIC_VAR_INIT(0);

int worker_thread(void* arg) {
    int thread_id = *(int*)arg;
    
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&global_counter, 1);
    }
    
    printf("Thread %d finished\n", thread_id);
    return 0;
}

int main() {
    thrd_t threads[4];
    int thread_ids[4];
    
    for (int i = 0; i < 4; i++) {
        thread_ids[i] = i;
        thrd_create(&threads[i], worker_thread, &thread_ids[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        thrd_join(threads[i], NULL);
    }
    
    printf("Final counter: %d\n", atomic_load(&global_counter));
    return 0;
}
```

### 三、数学库应用
```c
#include <math.h>
#include <complex.h>
#include <tgmath.h>

void math_examples() {
    // 基本数学函数
    double angle = M_PI / 4.0;
    printf("sin(π/4) = %.6f\n", sin(angle));
    printf("cos(π/4) = %.6f\n", cos(angle));
    
    // 复数运算
    double complex z1 = 1.0 + 2.0*I;
    double complex z2 = 3.0 + 4.0*I;
    double complex sum = z1 + z2;
    printf("Complex sum: %.2f + %.2fi\n", creal(sum), cimag(sum));
    
    // 类型泛型数学
    float f = 16.0f;
    double d = 25.0;
    printf("sqrt(16.0f) = %.2f\n", sqrt(f));  // 调用sqrtf
    printf("sqrt(25.0) = %.2f\n", sqrt(d));   // 调用sqrt
}
```

---

## 标准参考文献

### 相关C标准章节：

- **C23标准**（ISO/IEC 9899:2024）
  - 7.1.2 标准头文件
- **C17标准**（ISO/IEC 9899:2018）
  - 7.1.2 标准头文件（p: 131-132）
- **C11标准**（ISO/IEC 9899:2011）
  - 7.1.2 标准头文件（p: 181-182）
- **C99标准**（ISO/IEC 9899:1999）
  - 7.1.2 标准头文件（p: 165）
- **C89/C90标准**（ISO/IEC 9899:1990）
  - 4.1.2 标准头文件

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C++标准库头文件文档] | C++对应文档 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=c/header&oldid=180234>
- 最后修改时间：2025年2月7日 13:59
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C标准库头文件是C语言编程的基础，提供了丰富的功能来处理各种编程任务。从基础的输入输出到现代的多线程和原子操作，每个头文件都有其特定的用途和最佳实践。理解各头文件的功能和使用方法，对于编写高效、可移植的C程序至关重要。随着C语言标准的演进，新的头文件和特性不断被添加，以满足现代编程的需求。掌握特性测试宏的使用，可以编写兼容不同标准版本的代码。正确使用内存管理和错误处理函数，能够提高程序的稳定性和安全性。C标准库的零开销设计哲学确保了其高性能特性，使其成为系统编程和嵌入式开发的首选工具。