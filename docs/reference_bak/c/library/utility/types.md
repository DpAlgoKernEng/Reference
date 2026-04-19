# C语言类型支持（Type Support）

来源：cppreference.com

## 类型支持概述

C语言提供基本类型支持，包括基本数据类型、类型相关宏和便利宏。

## 基本类型

### 附加基本类型和便利宏

| 头文件 | 类型/宏 | 说明 |
|--------|---------|------|
| `<stddef.h>` | `size_t` | sizeof运算符返回的无符号整数类型（typedef） |
| `<stddef.h>` | `ptrdiff_t` | 减去两个指针时返回的有符号整数类型（typedef） |
| `<stddef.h>` | `nullptr_t`(C23) | 预定义空指针常量nullptr的类型（typedef） |
| `<stddef.h>` | `NULL` | 实现定义的空指针常量（宏常量） |
| `<stddef.h>` | `max_align_t`(C11) | 对齐要求与任何其他标量类型一样大的类型（typedef） |
| `<stddef.h>` | `offsetof` | 从结构体类型开始到指定成员的字节偏移量（函数宏） |
| `<stdbool.h>` | `bool`(C99)(C23移除) | 便利宏，展开为_Bool（关键字宏） |
| `<stdbool.h>` | `true`(C99)(C23移除) | 展开为整数常量1（宏常量） |
| `<stdbool.h>` | `false`(C99)(C23移除) | 展开为整数常量0（宏常量） |
| `<stdbool.h>` | `__bool_true_false_are_defined`(C99)(C23弃用) | 展开为整数常量1（宏常量） |
| `<stdalign.h>` | `alignas`(C11)(C23移除) | 便利宏，展开为关键字_Alignas（关键字宏） |
| `<stdalign.h>` | `alignof`(C11)(C23移除) | 便利宏，展开为关键字_Alignof（关键字宏） |
| `<stdalign.h>` | `__alignas_is_defined`(C11)(C23移除) | 展开为整数常量1（宏常量） |
| `<stdalign.h>` | `__alignof_is_defined`(C11)(C23移除) | 展开为整数常量1（宏常量） |
| `<stdnoreturn.h>` | `noreturn`(C11)(C23弃用) | 便利宏，展开为_Noreturn（关键字宏） |

### 固定宽度整数类型（C99起）

### 数值限制

## 扩展知识详解

### 一、类型系统基础

#### 1. 基本整数类型
```c
#include <stdio.h>
#include <stddef.h>

void basic_types_demo() {
    // 基本整数类型
    char c = 'A';
    short s = 100;
    int i = 1000;
    long l = 100000L;
    long long ll = 1000000000LL;
    
    // 无符号类型
    unsigned char uc = 255;
    unsigned short us = 65535;
    unsigned int ui = 4294967295U;
    unsigned long ul = 4294967295UL;
    unsigned long long ull = 18446744073709551615ULL;
    
    printf("Sizes: char=%zu, short=%zu, int=%zu, long=%zu, long long=%zu\n",
           sizeof(char), sizeof(short), sizeof(int), sizeof(long), sizeof(long long));
}
```

#### 2. 浮点类型
```c
#include <float.h>
#include <stdio.h>

void float_types_demo() {
    float f = 3.14f;
    double d = 3.141592653589793;
    long double ld = 3.141592653589793238L;
    
    printf("Float precision: %d digits\n", FLT_DIG);
    printf("Double precision: %d digits\n", DBL_DIG);
    printf("Long double precision: %d digits\n", LDBL_DIG);
}
```

### 二、类型相关宏详解

#### 1. size_t和ptrdiff_t
```c
#include <stdio.h>
#include <stddef.h>

void size_ptrdiff_demo() {
    int array[100];
    int *p1 = &array[10];
    int *p2 = &array[50];
    
    // size_t - 用于表示大小
    size_t array_size = sizeof(array);
    printf("Array size: %zu bytes\n", array_size);
    
    // ptrdiff_t - 用于表示指针差值
    ptrdiff_t diff = p2 - p1;
    printf("Pointer difference: %td elements\n", diff);
}
```

#### 2. 布尔类型（C99起）
```c
#include <stdbool.h>
#include <stdio.h>

void bool_demo() {
    bool flag1 = true;
    bool flag2 = false;
    bool flag3 = 5 > 3;  // true
    
    printf("flag1: %d\n", flag1);
    printf("flag2: %d\n", flag2);
    printf("flag3: %d\n", flag3);
    
    // 逻辑运算
    bool result = flag1 && flag3;  // true
    printf("AND result: %d\n", result);
    
    result = flag1 || flag2;  // true
    printf("OR result: %d\n", result);
    
    result = !flag2;  // true
    printf("NOT result: %d\n", result);
}
```

#### 3. 对齐支持（C11起）
```c
#include <stdalign.h>
#include <stdio.h>

void alignment_demo() {
    // 使用_Alignas指定对齐
    _Alignas(16) char aligned_array[100];
    
    // 使用alignas宏（C11起）
    struct alignas(32) AlignedStruct {
        int x;
        double y;
    };
    
    struct AlignedStruct s;
    
    printf("Alignment of char: %zu\n", _Alignof(char));
    printf("Alignment of int: %zu\n", _Alignof(int));
    printf("Alignment of double: %zu\n", _Alignof(double));
    printf("Alignment of AlignedStruct: %zu\n", _Alignof(struct AlignedStruct));
    
    // 使用alignof宏
    printf("Max alignment: %zu\n", alignof(max_align_t));
}
```

#### 4. offsetof宏
```c
#include <stddef.h>
#include <stdio.h>

void offsetof_demo() {
    struct Person {
        char name[50];
        int age;
        double salary;
    };
    
    printf("Offset of name: %zu\n", offsetof(struct Person, name));
    printf("Offset of age: %zu\n", offsetof(struct Person, age));
    printf("Offset of salary: %zu\n", offsetof(struct Person, salary));
    
    // 实际应用：基于偏移量访问成员
    struct Person p = {"John", 30, 50000.0};
    char *base = (char*)&p;
    
    // 通过偏移量访问age成员
    int *age_ptr = (int*)(base + offsetof(struct Person, age));
    printf("Age via offset: %d\n", *age_ptr);
}
```

### 三、固定宽度整数类型（C99起）

```c
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

void fixed_width_types_demo() {
    // 精确宽度类型
    int8_t i8 = 127;
    uint8_t u8 = 255;
    int16_t i16 = 32767;
    uint16_t u16 = 65535;
    int32_t i32 = 2147483647;
    uint32_t u32 = 4294967295U;
    int64_t i64 = 9223372036854775807LL;
    uint64_t u64 = 18446744073709551615ULL;
    
    // 最小宽度类型
    int_least8_t least8;
    int_least16_t least16;
    
    // 最快类型
    int_fast8_t fast8;
    int_fast16_t fast16;
    
    // 最大类型
    intmax_t imax = INTMAX_MAX;
    uintmax_t umax = UINTMAX_MAX;
    
    // 打印格式
    printf("int8_t: %" PRId8 "\n", i8);
    printf("uint8_t: %" PRIu8 "\n", u8);
    printf("int32_t: %" PRId32 "\n", i32);
    printf("uint64_t: %" PRIu64 "\n", u64);
}
```

### 四、数值限制

```c
#include <limits.h>
#include <float.h>
#include <stdio.h>

void numeric_limits_demo() {
    // 整数限制
    printf("CHAR_BIT: %d\n", CHAR_BIT);
    printf("SCHAR_MIN: %d\n", SCHAR_MIN);
    printf("SCHAR_MAX: %d\n", SCHAR_MAX);
    printf("UCHAR_MAX: %u\n", UCHAR_MAX);
    printf("SHRT_MIN: %d\n", SHRT_MIN);
    printf("SHRT_MAX: %d\n", SHRT_MAX);
    printf("USHRT_MAX: %u\n", USHRT_MAX);
    printf("INT_MIN: %d\n", INT_MIN);
    printf("INT_MAX: %d\n", INT_MAX);
    printf("UINT_MAX: %u\n", UINT_MAX);
    printf("LONG_MIN: %ld\n", LONG_MIN);
    printf("LONG_MAX: %ld\n", LONG_MAX);
    printf("ULONG_MAX: %lu\n", ULONG_MAX);
    printf("LLONG_MIN: %lld\n", LLONG_MIN);
    printf("LLONG_MAX: %lld\n", LLONG_MAX);
    printf("ULLONG_MAX: %llu\n", ULLONG_MAX);
    
    // 浮点限制
    printf("FLT_DIG: %d\n", FLT_DIG);
    printf("DBL_DIG: %d\n", DBL_DIG);
    printf("FLT_EPSILON: %e\n", FLT_EPSILON);
    printf("DBL_EPSILON: %e\n", DBL_EPSILON);
}
```

## 实际应用示例

### 一、安全的内存管理
```c
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

// 安全的内存分配函数
void* safe_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // 检查溢出
    if (size > SIZE_MAX / 2) {
        return NULL;
    }
    
    return malloc(size);
}

// 数组分配
int* allocate_int_array(size_t count) {
    if (count > SIZE_MAX / sizeof(int)) {
        return NULL;  // 防止溢出
    }
    
    return (int*)safe_malloc(count * sizeof(int));
}
```

### 二、跨平台类型定义
```c
#include <stdint.h>
#include <stddef.h>

// 确保跨平台兼容性
typedef struct {
    uint32_t id;
    uint64_t timestamp;
    size_t data_size;
    ptrdiff_t offset;
} PortableHeader;

// 固定大小的网络协议头
typedef struct {
    uint16_t version;
    uint16_t flags;
    uint32_t length;
    uint32_t checksum;
} NetworkHeader;
```

### 三、高性能计算类型
```c
#include <stdint.h>

// 使用最快类型进行计算
void performance_calculation() {
    int_fast32_t counter = 0;
    int_fast64_t accumulator = 0;
    
    for (int_fast32_t i = 0; i < 1000000; i++) {
        counter += i;
        accumulator += (int_fast64_t)i * i;
    }
}

// 使用最小宽度类型节省内存
typedef struct {
    int_least8_t small_value;    // 至少8位
    int_least16_t medium_value;  // 至少16位
    int_least32_t large_value;   // 至少32位
} MemoryEfficientStruct;
```

### 四、错误处理和状态码
```c
#include <stdbool.h>
#include <stdint.h>

// 使用布尔类型进行错误检查
bool validate_input(int value) {
    if (value < 0 || value > 1000) {
        return false;
    }
    return true;
}

// 使用固定宽度类型定义错误码
typedef enum {
    ERROR_SUCCESS = 0,
    ERROR_INVALID_PARAMETER = 0x0001,
    ERROR_OUT_OF_MEMORY = 0x0002,
    ERROR_FILE_NOT_FOUND = 0x0003
} ErrorCode;

// 返回错误码的函数
ErrorCode process_data(const uint8_t* data, size_t length) {
    if (data == NULL) {
        return ERROR_INVALID_PARAMETER;
    }
    
    if (length == 0) {
        return ERROR_INVALID_PARAMETER;
    }
    
    // 处理数据...
    return ERROR_SUCCESS;
}
```

## 注意事项

### 一、布尔类型历史变化

#### C99-C23前
```c
// true和false的类型是int，不是_Bool
#include <stdbool.h>
bool b = true;  // b的类型是_Bool，但true是int
```

#### C23起
```c
// true和false的类型是bool
#include <stdbool.h>
bool b = true;  // b和true都是bool类型
```

### 二、对齐相关宏的变化

#### C11-C23前
```c
#include <stdalign.h>
alignas(16) int aligned_var;  // 使用宏
```

#### C23起
```c
// alignas和alignof成为关键字，相关宏被移除
_Alignas(16) int aligned_var;  // 直接使用关键字
```

## 示例代码

```c
#include <stdalign.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

int main(void) {
    // 布尔运算示例
    printf("%d %d %d\n", true && false, true || false, !false);
    printf("%d %d\n", true ^ true, true + true);
    
    // 对齐信息
    printf("%zu\n", alignof(short));
    
    return 0;
}
```

输出结果：
```
0 1 1
0 2
2
```

## 标准参考

### C23标准（ISO/IEC 9899:2024）
- 7.15 对齐 <stdalign.h>
- 7.18 布尔类型和值 <stdbool.h>
- 7.19 通用定义 <stddef.h>
- 7.23 _Noreturn <stdnoreturn.h>

### C17标准（ISO/IEC 9899:2018）
- 7.15 对齐 <stdalign.h>
- 7.18 布尔类型和值 <stdbool.h>
- 7.19 通用定义 <stddef.h>
- 7.23 _Noreturn <stdnoreturn.h>

### C11标准（ISO/IEC 9899:2011）
- 7.15 对齐 <stdalign.h>
- 7.18 布尔类型和值 <stdbool.h>
- 7.19 通用定义 <stddef.h>
- 7.23 _Noreturn <stdnoreturn.h>

### C99标准（ISO/IEC 9899:1999）
- 7.18 布尔类型和值 <stdbool.h>
- 7.19 通用定义 <stddef.h>

## 相关链接

| 内容 | 说明 |
|------|------|
| [C++类型支持库文档] | C++对应文档 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=c/types&oldid=180056>
- 最后修改时间：2025年2月2日 04:58
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C语言类型支持为系统编程提供了坚实的基础。理解各种基本类型、类型相关宏和固定宽度类型的特点，对于编写可移植、高效的C代码至关重要。从C99的布尔类型到C11的对齐支持，再到C23的现代化改进，C语言不断演进以满足现代软件开发的需求。正确使用这些类型和宏，能够避免常见的整数溢出、对齐错误和跨平台兼容性问题。掌握数值限制和固定宽度类型，对于网络编程、嵌入式开发和高性能计算尤为重要。