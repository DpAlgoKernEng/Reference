# C 关键字详解

  [1. C 关键字概述](#1-c-关键字概述)
  
  [2. 基本数据类型关键字](#2-基本数据类型关键字)
  
  [3. 控制流关键字](#3-控制流关键字)
  
  [4. 存储类关键字](#4-存储类关键字)
  
  [5. 类型限定符关键字](#5-类型限定符关键字)
  
  [6. 复合类型关键字](#6-复合类型关键字)
  
  [7. 函数相关关键字](#7-函数相关关键字)
  
  [8. C23新增关键字](#8-c23新增关键字)
  
  [9. 预处理器关键字](#9-预处理器关键字)
  
  [10. 最佳实践和使用指南](#10-最佳实践和使用指南)
  
  [11. 总结](#11-总结)

## 1. C 关键字概述

### 1.1 关键字定义

C 关键字是编程语言预定义的、具有特殊意义的词汇，它们构成了 C 语言语法的基础部分。这些关键字被编译器识别并用于构建程序的逻辑结构。

```c
#include <stdio.h>

void keyword_overview() {
    printf("C Keywords Overview:\n");
    
    // 关键字的特征
    printf("  === Keyword Characteristics ===\n");
    printf("    - 保留词，不能重新定义或重载\n");
    printf("    - 具有预定义的语法和语义\n");
    printf("    - 构成C语言的基础元素\n");
    printf("    - 在属性中不被视为保留（自C23起）\n");
    
    // 关键字分类
    printf("  === Keyword Categories ===\n");
    printf("    - 基本数据类型关键字\n");
    printf("    - 控制流关键字\n");
    printf("    - 存储类关键字\n");
    printf("    - 类型限定符关键字\n");
    printf("    - 复合类型关键字\n");
    printf("    - 函数相关关键字\n");
    printf("    - 预处理器关键字\n");
    printf("    - C99/C11/C23新增关键字\n");
    
    // 版本演进
    printf("  === Version Evolution ===\n");
    printf("    - C89/C90: 32个基础关键字\n");
    printf("    - C99: 新增5个关键字\n");
    printf("    - C11: 新增7个关键字\n");
    printf("    - C23: 新增12个关键字\n");
}
```

### 1.2 关键字使用规则

```c
#include <stdio.h>
#include <stdbool.h>

void keyword_usage_rules() {
    printf("Keyword Usage Rules:\n");
    
    // 有效的标识符命名
    printf("  === Valid Identifier Names ===\n");
    int variable;           // 正确
    double my_function();   // 正确
    struct MyStruct {};     // 正确
    
    printf("    int variable;           // 正确\n");
    printf("    double my_function();   // 正确\n");
    printf("    struct MyStruct {};     // 正确\n");
    
    // 无效的标识符命名（编译错误）
    printf("  === Invalid Identifier Names (Compilation Errors) ===\n");
    // int int;              // 错误：不能使用关键字作为标识符
    // double if = 3.14;     // 错误：if是关键字
    // void while() {}       // 错误：while是关键字
    
    // 在属性中的例外情况（C23）
    printf("  === Exception in Attributes (C23) ===\n");
    // [[deprecated("use new_function instead")]]  // 正确
    printf("    [[deprecated(\"use new_function instead\")]]  // 正确\n");
    
    // 替代表示法
    printf("  === Alternative Representations ===\n");
    printf("    <%    -> {\n");
    printf("    %>    -> }\n");
    printf("    <:    -> [\n");
    printf("    :>    -> ]\n");
    printf("    %:    -> #\n");
    printf("    %:%:  -> ##\n");
}
```

## 2. 基本数据类型关键字

### 2.1 void

```c
#include <stdio.h>

void void_keyword() {
    printf("void Keyword:\n");
    
    // 无返回值函数
    printf("  === Void Return Type ===\n");
    void print_message() {
        printf("    Hello from void function!\n");
    }
    
    print_message();
    
    // 通用指针类型
    printf("  === Generic Pointer Type ===\n");
    int num = 42;
    void* ptr = &num;
    int* int_ptr = (int*)ptr;
    
    printf("    Original value: %d\n", num);
    printf("    Value through void pointer: %d\n", *int_ptr);
    
    // 空参数列表
    printf("  === Empty Parameter List ===\n");
    void no_params(void) {
        printf("    Function with no parameters\n");
    }
    
    no_params();
}
```

### 2.2 char

```c
#include <stdio.h>

void char_keyword() {
    printf("char Keyword:\n");
    
    // 基本字符类型
    printf("  === Basic Character Type ===\n");
    char c1 = 'A';
    char c2 = 65;  // ASCII值
    char c3 = '\n'; // 转义字符
    
    printf("    char c1 = 'A';     // %c\n", c1);
    printf("    char c2 = 65;      // %c\n", c2);
    printf("    char c3 = '\\n';    // Newline character\n");
    printf("    sizeof(char): %zu byte(s)\n", sizeof(char));
    
    // 字符串字面量
    printf("  === String Literals ===\n");
    char str1[] = "Hello";
    char str2[] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char* str3 = "World";
    
    printf("    char str1[] = \"Hello\";  // %s\n", str1);
    printf("    char str2[] = {...};     // %s\n", str2);
    printf("    char* str3 = \"World\";   // %s\n", str3);
    
    // 有符号和无符号字符
    printf("  === Signed and Unsigned Char ===\n");
    signed char sc = -128;
    unsigned char uc = 255;
    
    printf("    signed char sc = %d\n", sc);
    printf("    unsigned char uc = %u\n", uc);
}
```

### 2.3 int

```c
#include <stdio.h>
#include <limits.h>

void int_keyword() {
    printf("int Keyword:\n");
    
    // 基本整数类型
    printf("  === Basic Integer Type ===\n");
    int i1 = 42;
    int i2 = -100;
    int i3 = 0x1A;  // 十六进制
    int i4 = 0755;  // 八进制
    
    printf("    int i1 = 42;      // %d\n", i1);
    printf("    int i2 = -100;    // %d\n", i2);
    printf("    int i3 = 0x1A;    // %d (hex)\n", i3);
    printf("    int i4 = 0755;    // %d (octal)\n", i4);
    printf("    sizeof(int): %zu byte(s)\n", sizeof(int));
    
    // 整数范围
    printf("  === Integer Range ===\n");
    printf("    INT_MIN: %d\n", INT_MIN);
    printf("    INT_MAX: %d\n", INT_MAX);
    
    // 短整型和长整型
    printf("  === Short and Long Integers ===\n");
    short s = 32767;
    long l = 2147483647L;
    long long ll = 9223372036854775807LL;
    
    printf("    short s = %hd\n", s);
    printf("    long l = %ld\n", l);
    printf("    long long ll = %lld\n", ll);
}
```

### 2.4 float, double, long double

```c
#include <stdio.h>
#include <float.h>

void floating_point_keywords() {
    printf("Floating Point Keywords:\n");
    
    // 单精度浮点数
    printf("  === float ===\n");
    float f = 3.14159f;
    printf("    float f = 3.14159f;  // %f\n", f);
    printf("    sizeof(float): %zu byte(s)\n", sizeof(float));
    printf("    FLT_DIG: %d decimal digits\n", FLT_DIG);
    
    // 双精度浮点数
    printf("  === double ===\n");
    double d = 3.141592653589793;
    printf("    double d = 3.141592653589793;  // %lf\n", d);
    printf("    sizeof(double): %zu byte(s)\n", sizeof(double));
    printf("    DBL_DIG: %d decimal digits\n", DBL_DIG);
    
    // 长双精度浮点数
    printf("  === long double ===\n");
    long double ld = 3.141592653589793238L;
    printf("    long double ld = 3.141592653589793238L;  // %Lf\n", ld);
    printf("    sizeof(long double): %zu byte(s)\n", sizeof(long double));
    printf("    LDBL_DIG: %d decimal digits\n", LDBL_DIG);
    
    // 浮点数特殊值
    printf("  === Special Floating Point Values ===\n");
    double inf = 1.0/0.0;
    double nan = 0.0/0.0;
    
    printf("    Infinity: %f\n", inf);
    printf("    NaN: %f\n", nan);
}
```

### 2.5 _Bool, bool (C99, C23)

```c
#include <stdio.h>
#include <stdbool.h>

void bool_keywords() {
    printf("_Bool, bool Keywords:\n");
    
    // _Bool类型（C99）
    printf("  === _Bool (C99) ===\n");
    _Bool flag1 = 1;
    _Bool flag2 = 0;
    _Bool flag3 = 2;  // 非零值转换为1
    
    printf("    _Bool flag1 = 1;   // %d\n", flag1);
    printf("    _Bool flag2 = 0;   // %d\n", flag2);
    printf("    _Bool flag3 = 2;   // %d (converted to 1)\n", flag3);
    printf("    sizeof(_Bool): %zu byte(s)\n", sizeof(_Bool));
    
    // bool类型（通过stdbool.h，C99；C23直接支持）
    printf("  === bool (C99/C23) ===\n");
    bool condition1 = true;
    bool condition2 = false;
    
    printf("    bool condition1 = true;   // %d\n", condition1);
    printf("    bool condition2 = false;  // %d\n", condition2);
    
    // 布尔运算
    printf("  === Boolean Operations ===\n");
    printf("    true && false = %d\n", true && false);
    printf("    true || false = %d\n", true || false);
    printf("    !true = %d\n", !true);
    
    // 整数到布尔的转换
    printf("  === Integer to Boolean Conversion ===\n");
    bool b1 = 1;     // true
    bool b2 = 0;     // false
    bool b3 = -1;    // true (非零为真)
    
    printf("    bool b1 = 1;   // %d\n", b1);
    printf("    bool b2 = 0;   // %d\n", b2);
    printf("    bool b3 = -1;  // %d\n", b3);
}
```

## 3. 控制流关键字

### 3.1 if, else

```c
#include <stdio.h>

void if_else_keywords() {
    printf("if, else Keywords:\n");
    
    // 基本if语句
    printf("  === Basic if Statement ===\n");
    int x = 10;
    if (x > 0) {
        printf("    x is positive: %d\n", x);
    }
    
    // if-else语句
    printf("  === if-else Statement ===\n");
    int y = -5;
    if (y > 0) {
        printf("    y is positive: %d\n", y);
    } else {
        printf("    y is not positive: %d\n", y);
    }
    
    // 嵌套if语句
    printf("  === Nested if Statements ===\n");
    int z = 0;
    if (z > 0) {
        printf("    z is positive\n");
    } else if (z < 0) {
        printf("    z is negative\n");
    } else {
        printf("    z is zero\n");
    }
    
    // 悬空else问题
    printf("  === Dangling else Problem ===\n");
    int a = 5, b = 3;
    if (a > 0)
        if (b > 0)
            printf("    Both a and b are positive\n");
        else
            printf("    This else belongs to inner if\n");
    
    // 使用花括号避免歧义
    if (a > 0) {
        if (b > 0) {
            printf("    Both a and b are positive (clear)\n");
        }
    } else {
        printf("    This else belongs to outer if\n");
    }
}
```

### 3.2 switch, case, default

```c
#include <stdio.h>

void switch_keywords() {
    printf("switch, case, default Keywords:\n");
    
    // 基本switch语句
    printf("  === Basic switch Statement ===\n");
    int value = 2;
    switch (value) {
        case 1:
            printf("    Value is 1\n");
            break;
        case 2:
            printf("    Value is 2\n");
            break;
        case 3:
            printf("    Value is 3\n");
            break;
        default:
            printf("    Value is something else\n");
    }
    
    // 多个case共享代码块
    printf("  === Multiple Cases ===\n");
    char grade = 'B';
    switch (grade) {
        case 'A':
        case 'B':
        case 'C':
            printf("    Passing grade\n");
            break;
        case 'D':
        case 'F':
            printf("    Failing grade\n");
            break;
        default:
            printf("    Invalid grade\n");
    }
    
    // switch中的作用域
    printf("  === Scope in switch ===\n");
    int choice = 1;
    switch (choice) {
        case 1: {
            int x = 10;  // 需要花括号创建作用域
            printf("    Local variable x = %d\n", x);
            break;
        }
        case 2: {
            int y = 20;  // 每个case可以有自己的局部变量
            printf("    Local variable y = %d\n", y);
            break;
        }
        default:
            printf("    Default case\n");
    }
    
    // fall-through示例
    printf("  === Fall-through Example ===\n");
    int day = 2;
    switch (day) {
        case 1:
            printf("    Monday ");
            // 注意：没有break，会继续执行下面的case
        case 2:
            printf("Tuesday ");
        case 3:
            printf("Wednesday\n");
            break;
        default:
            printf("    Other day\n");
    }
}
```

### 3.3 while, do, for

```c
#include <stdio.h>

void loop_keywords() {
    printf("while, do, for Keywords:\n");
    
    // while循环
    printf("  === while Loop ===\n");
    int i = 0;
    while (i < 5) {
        printf("    while: i = %d\n", i);
        ++i;
    }
    
    // do-while循环
    printf("  === do-while Loop ===\n");
    int j = 0;
    do {
        printf("    do-while: j = %d\n", j);
        ++j;
    } while (j < 3);
    
    // for循环（传统）
    printf("  === Traditional for Loop ===\n");
    for (int k = 0; k < 3; ++k) {
        printf("    for: k = %d\n", k);
    }
    
    // 无限循环
    printf("  === Infinite Loops ===\n");
    int counter = 0;
    while (1) {  // 或 while(true)
        if (++counter > 2) break;
        printf("    Infinite loop iteration %d\n", counter);
    }
    
    // 嵌套循环和控制
    printf("  === Nested Loops ===\n");
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (x == 1 && y == 1) {
                printf("    Breaking inner loop\n");
                break;
            }
            printf("    x=%d, y=%d\n", x, y);
        }
    }
}
```

### 3.4 break, continue, goto

```c
#include <stdio.h>

void control_flow_keywords() {
    printf("break, continue, goto Keywords:\n");
    
    // break关键字
    printf("  === break Keyword ===\n");
    printf("    Breaking out of loop:\n");
    for (int i = 0; i < 10; ++i) {
        if (i == 5) {
            printf("      Breaking at i = %d\n", i);
            break;
        }
        printf("      i = %d\n", i);
    }
    
    // switch语句中的break
    printf("  === break in switch ===\n");
    int choice = 2;
    switch (choice) {
        case 1:
            printf("      Case 1\n");
            break;
        case 2:
            printf("      Case 2\n");
            break;  // 防止fall-through
        case 3:
            printf("      Case 3\n");
            break;
        default:
            printf("      Default case\n");
    }
    
    // continue关键字
    printf("  === continue Keyword ===\n");
    printf("    Skipping even numbers:\n");
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        printf("      Odd number: %d\n", i);
    }
    
    // goto关键字
    printf("  === goto Keyword ===\n");
    int n = 0;
    goto_label:
    printf("    n = %d\n", n);
    if (++n < 3) {
        goto goto_label;
    }
    
    // 使用goto进行错误处理
    printf("  === goto for Error Handling ===\n");
    FILE* file = fopen("nonexistent.txt", "r");
    if (!file) {
        printf("    File open failed\n");
        goto cleanup;
    }
    
    // 文件操作...
    printf("    File operations would go here\n");
    
    cleanup:
    if (file) {
        fclose(file);
        printf("    File closed\n");
    }
}
```

## 4. 存储类关键字

### 4.1 auto

```c
#include <stdio.h>

void auto_keyword() {
    printf("auto Keyword:\n");
    
    // auto是默认存储类（C中）
    printf("  === auto Storage Class (C) ===\n");
    auto int x = 42;  // 等同于 int x = 42;
    auto double y = 3.14;  // 等同于 double y = 3.14;
    
    printf("    auto int x = %d\n", x);
    printf("    auto double y = %f\n", y);
    
    // 在块作用域中
    {
        auto int z = 100;
        printf("    Inside block: z = %d\n", z);
    }
    // z在此处不可访问
    
    // auto与register的对比
    printf("  === auto vs register ===\n");
    auto int auto_var = 1;      // 存储在内存中
    register int reg_var = 2;   // 建议存储在寄存器中
    
    printf("    auto variable: %d\n", auto_var);
    printf("    register variable: %d\n", reg_var);
    
    // 注意：不能对auto变量取地址（在C++11中含义不同）
    int* ptr = &x;  // 这是可以的
    printf("    Address of auto variable: %p\n", (void*)ptr);
}
```

### 4.2 register

```c
#include <stdio.h>

void register_keyword() {
    printf("register Keyword:\n");
    
    // register建议存储类
    printf("  === register Storage Class ===\n");
    register int counter = 0;
    register double accumulator = 0.0;
    
    printf("    register int counter = %d\n", counter);
    printf("    register double accumulator = %f\n", accumulator);
    
    // register变量的限制
    printf("  === Limitations of register ===\n");
    // register int array[10];  // 错误：不能声明数组为register
    // int* ptr = &counter;     // 错误：不能对register变量取地址
    
    // 实际使用示例
    printf("  === Practical Example ===\n");
    register int i;
    int sum = 0;
    for (i = 0; i < 1000; ++i) {
        sum += i;
    }
    printf("    Sum of 0 to 999: %d\n", sum);
    
    // 现代编译器优化
    printf("  === Modern Compiler Considerations ===\n");
    printf("    Modern compilers often ignore register keyword\n");
    printf("    because they have better optimization algorithms\n");
}
```

### 4.3 static

```c
#include <stdio.h>

// 文件作用域的static变量
static int file_static_var = 42;
static void static_helper_function() {
    printf("    This is a static helper function\n");
}

void static_keyword() {
    printf("static Keyword:\n");
    
    // 函数内的static变量
    printf("  === Static Local Variables ===\n");
    int counter() {
        static int count = 0;  // 只初始化一次
        return ++count;
    }
    
    printf("    First call: %d\n", counter());
    printf("    Second call: %d\n", counter());
    printf("    Third call: %d\n", counter());
    
    // 文件作用域的static变量
    printf("  === Static File Scope ===\n");
    printf("    file_static_var = %d\n", file_static_var);
    static_helper_function();
    
    // static函数
    printf("  === Static Functions ===\n");
    // static_helper_function();  // 只能在当前文件内调用
    
    // static在数组中的应用
    printf("  === Static Arrays ===\n");
    static int static_array[5] = {1, 2, 3, 4, 5};
    printf("    static_array[0] = %d\n", static_array[0]);
    
    // static与extern的对比
    printf("  === static vs extern ===\n");
    printf("    static: internal linkage (file scope only)\n");
    printf("    extern: external linkage (global scope)\n");
}
```

### 4.4 extern

```c
#include <stdio.h>

// 声明外部变量（定义在其他文件中）
extern int external_var;
extern void external_function();

void extern_keyword() {
    printf("extern Keyword:\n");
    
    // 声明外部变量
    printf("  === External Variable Declaration ===\n");
    // extern int external_var;  // 声明，非定义
    printf("    External variable declared\n");
    
    // 声明外部函数
    printf("  === External Function Declaration ===\n");
    // extern void external_function();  // 函数声明默认是extern
    printf("    External function declared\n");
    
    // 多文件示例（概念）
    printf("  === Multi-file Example (Concept) ===\n");
    printf("    // file1.c\n");
    printf("    int global_var = 100;\n");
    printf("    void global_function() { }\n");
    printf("    \n");
    printf("    // file2.c\n");
    printf("    extern int global_var;\n");
    printf("    extern void global_function();\n");
    printf("    \n");
    printf("    void use_globals() {\n");
    printf("        printf(\"%%d\\n\", global_var);\n");
    printf("        global_function();\n");
    printf("    }\n");
    
    // extern "C"（C++中使用）
    printf("  === extern \"C\" (C++ interoperability) ===\n");
    printf("    #ifdef __cplusplus\n");
    printf("    extern \"C\" {\n");
    printf("    #endif\n");
    printf("    \n");
    printf("    void c_function();\n");
    printf("    \n");
    printf("    #ifdef __cplusplus\n");
    printf("    }\n");
    printf("    #endif\n");
}
```

### 4.5 typedef

```c
#include <stdio.h>
#include <stdlib.h>

void typedef_keyword() {
    printf("typedef Keyword:\n");
    
    // 基本类型别名
    printf("  === Basic Type Aliases ===\n");
    typedef unsigned int uint;
    typedef long long int64;
    typedef unsigned char byte;
    
    uint u = 42;
    int64 big_num = 123456789012345LL;
    byte b = 255;
    
    printf("    typedef unsigned int uint;\n");
    printf("    uint u = %u\n", u);
    printf("    typedef long long int64;\n");
    printf("    int64 big_num = %lld\n", big_num);
    printf("    typedef unsigned char byte;\n");
    printf("    byte b = %u\n", b);
    
    // 结构体别名
    printf("  === Struct Aliases ===\n");
    typedef struct {
        int x;
        int y;
    } Point;
    
    Point p = {10, 20};
    printf("    Point p = {%d, %d}\n", p.x, p.y);
    
    // 函数指针别名
    printf("  === Function Pointer Aliases ===\n");
    typedef int (*BinaryOperation)(int, int);
    
    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }
    
    BinaryOperation op1 = add;
    BinaryOperation op2 = multiply;
    
    printf("    Result of op1(3, 4): %d\n", op1(3, 4));
    printf("    Result of op2(3, 4): %d\n", op2(3, 4));
    
    // 复杂类型别名
    printf("  === Complex Type Aliases ===\n");
    typedef int Array3D[10][10][10];
    typedef void (*VoidFunction)(void);
    
    Array3D matrix;
    printf("    sizeof(Array3D): %zu bytes\n", sizeof(matrix));
    
    // 与#define的对比
    printf("  === typedef vs #define ===\n");
    typedef char* PCHAR1;
    #define PCHAR2 char*
    
    PCHAR1 p1, p2;  // p1和p2都是char*
    PCHAR2 p3, p4;  // p3是char*，p4是char（不是char*！）
    
    printf("    typedef char* PCHAR1; PCHAR1 p1, p2;  // Both are char*\n");
    printf("    #define PCHAR2 char*; PCHAR2 p3, p4;  // p3 is char*, p4 is char\n");
}
```

## 5. 类型限定符关键字

### 5.1 const

```c
#include <stdio.h>

void const_keyword() {
    printf("const Keyword:\n");
    
    // 基本const变量
    printf("  === Basic const Variables ===\n");
    const int constant_value = 42;
    // constant_value = 100;  // 编译错误：不能修改const变量
    
    printf("    const int constant_value = %d\n", constant_value);
    
    // const指针
    printf("  === const Pointers ===\n");
    int value1 = 10, value2 = 20;
    
    // 指向const数据的指针
    const int* ptr_to_const = &value1;
    // *ptr_to_const = 30;  // 错误：不能通过指针修改数据
    ptr_to_const = &value2;  // 正确：可以改变指针指向
    
    // const指针
    int* const const_ptr = &value1;
    *const_ptr = 30;         // 正确：可以通过指针修改数据
    // const_ptr = &value2;  // 错误：不能改变指针指向
    
    // const指针指向const数据
    const int* const const_ptr_to_const = &value1;
    // *const_ptr_to_const = 40;  // 错误：不能修改数据
    // const_ptr_to_const = &value2;  // 错误：不能改变指向
    
    printf("    value1 after modifications: %d\n", value1);
    
    // const数组
    printf("  === const Arrays ===\n");
    const int const_array[] = {1, 2, 3, 4, 5};
    // const_array[0] = 10;  // 错误：不能修改const数组元素
    
    printf("    const_array[0] = %d\n", const_array[0]);
    
    // const函数参数
    printf("  === const Function Parameters ===\n");
    void print_array(const int arr[], int size) {
        // arr[0] = 100;  // 错误：arr是const
        for (int i = 0; i < size; ++i) {
            printf("    %d ", arr[i]);  // 正确：只读访问
        }
        printf("\n");
    }
    
    int test_array[] = {10, 20, 30};
    print_array(test_array, 3);
}
```

### 5.2 volatile

```c
#include <stdio.h>

void volatile_keyword() {
    printf("volatile Keyword:\n");
    
    // volatile变量
    printf("  === volatile Variables ===\n");
    volatile int sensor_value = 0;
    // 编译器不会优化对volatile变量的访问
    
    printf("    volatile int sensor_value = %d\n", sensor_value);
    
    // 硬件寄存器示例
    printf("  === Hardware Register Example ===\n");
    // volatile unsigned int* gpio_register = (volatile unsigned int*)0x40000000;
    // *gpio_register = 0xFF;  // 直接写入硬件寄存器
    
    // 多线程环境
    printf("  === Multithreading Context ===\n");
    volatile int shared_flag = 0;
    // 在多线程环境中，shared_flag的值可能随时被其他线程改变
    
    // 信号处理程序
    printf("  === Signal Handler Context ===\n");
    volatile sig_atomic_t signal_flag = 0;
    // 信号处理程序中修改的变量应该声明为volatile
    
    // 与const结合使用
    printf("  === volatile with const ===\n");
    const volatile int hardware_id = 0x12345678;
    // 硬件ID：不会被程序改变（const），但可能被硬件改变（volatile）
    
    printf("    const volatile int hardware_id = 0x%x\n", hardware_id);
    
    // 编译器优化的影响
    printf("  === Compiler Optimization Impact ===\n");
    printf("    Without volatile, compiler may:\n");
    printf("    - Cache variable in register\n");
    printf("    - Eliminate redundant reads/writes\n");
    printf("    - Reorder operations\n");
    printf("    \n");
    printf("    With volatile, compiler must:\n");
    printf("    - Always read from memory\n");
    printf("    - Always write to memory\n");
    printf("    - Preserve operation order\n");
}
```

### 5.3 restrict (C99)

```c
#include <stdio.h>
#include <string.h>

void restrict_keyword() {
    printf("restrict Keyword (C99):\n");
    
    // restrict指针
    printf("  === restrict Pointers ===\n");
    void add_arrays(int n, int* restrict a, int* restrict b, int* restrict c) {
        // 编译器假设a, b, c指向不重叠的内存区域
        for (int i = 0; i < n; ++i) {
            c[i] = a[i] + b[i];
        }
    }
    
    int array1[] = {1, 2, 3, 4, 5};
    int array2[] = {10, 20, 30, 40, 50};
    int result[5];
    
    add_arrays(5, array1, array2, result);
    
    printf("    Result array: ");
    for (int i = 0; i < 5; ++i) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    // restrict的优势
    printf("  === Benefits of restrict ===\n");
    printf("    - 允许编译器进行更好的优化\n");
    printf("    - 消除不必要的内存访问检查\n");
    printf("    - 可能启用向量化优化\n");
    printf("    - 提高程序性能\n");
    
    // 使用限制
    printf("  === Usage Restrictions ===\n");
    printf("    - 只能用于指针\n");
    printf("    - 程序员必须保证指针不重叠\n");
    printf("    - 违反假设会导致未定义行为\n");
    
    // 实际示例
    printf("  === Practical Example ===\n");
    void copy_and_scale(int n, double* restrict src, double* restrict dst, double scale) {
        // 编译器假设src和dst不重叠
        for (int i = 0; i < n; ++i) {
            dst[i] = src[i] * scale;
        }
    }
    
    double source[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double destination[5];
    
    copy_and_scale(5, source, destination, 2.0);
    
    printf("    Scaled array: ");
    for (int i = 0; i < 5; ++i) {
        printf("%.1f ", destination[i]);
    }
    printf("\n");
}
```

### 5.4 _Atomic (C11)

```c
#include <stdio.h>
#include <stdatomic.h>

void atomic_keyword() {
    printf("_Atomic Keyword (C11):\n");
    
    // 基本原子类型
    printf("  === Basic Atomic Types ===\n");
    _Atomic int atomic_counter = 0;
    atomic_int atomic_int_counter = 0;
    
    // 原子操作
    printf("  === Atomic Operations ===\n");
    int old_value = atomic_fetch_add(&atomic_counter, 1);
    printf("    Atomic counter: %d (was %d)\n", atomic_load(&atomic_counter), old_value);
    
    // 原子存储和加载
    atomic_store(&atomic_int_counter, 42);
    int loaded_value = atomic_load(&atomic_int_counter);
    printf("    Atomic int counter: %d\n", loaded_value);
    
    // 原子比较和交换
    printf("  === Compare and Swap ===\n");
    int expected = 42;
    int desired = 100;
    if (atomic_compare_exchange_strong(&atomic_int_counter, &expected, desired)) {
        printf("    Compare and swap succeeded: %d\n", atomic_load(&atomic_int_counter));
    } else {
        printf("    Compare and swap failed. Expected %d, got %d\n", expected, atomic_load(&atomic_int_counter));
    }
    
    // 内存顺序
    printf("  === Memory Ordering ===\n");
    atomic_thread_fence(memory_order_acquire);
    atomic_store_explicit(&atomic_counter, 1, memory_order_release);
    int value = atomic_load_explicit(&atomic_counter, memory_order_acquire);
    atomic_thread_fence(memory_order_seq_cst);
    
    printf("    Value after ordered operations: %d\n", value);
    
    // 原子标志
    printf("  === Atomic Flag ===\n");
    atomic_flag flag = ATOMIC_FLAG_INIT;
    
    if (!atomic_flag_test_and_set(&flag)) {
        printf("    Flag was clear, now set\n");
    }
    
    atomic_flag_clear(&flag);
    if (!atomic_flag_test_and_set(&flag)) {
        printf("    Flag was clear again, now set\n");
    }
    
    // 与其他关键字结合
    printf("  === Combining with Other Keywords ===\n");
    static _Atomic volatile int shared_resource = 0;
    // volatile: 值可能被硬件改变
    // _Atomic: 值可能被其他线程改变
    // static: 文件作用域
    
    atomic_store(&shared_resource, 1);
    printf("    Shared resource: %d\n", atomic_load(&shared_resource));
}
```

## 6. 复合类型关键字

### 6.1 struct

```c
#include <stdio.h>
#include <string.h>

void struct_keyword() {
    printf("struct Keyword:\n");
    
    // 基本结构体
    printf("  === Basic Struct ===\n");
    struct Point {
        double x;
        double y;
    };
    
    struct Point p1 = {3.0, 4.0};
    struct Point p2;
    p2.x = 1.0;
    p2.y = 2.0;
    
    printf("    Point p1{%.1f, %.1f}\n", p1.x, p1.y);
    printf("    Point p2{%.1f, %.1f}\n", p2.x, p2.y);
    
    // 结构体数组
    printf("  === Struct Arrays ===\n");
    struct Person {
        char name[50];
        int age;
    };
    
    struct Person people[3] = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35}
    };
    
    for (int i = 0; i < 3; ++i) {
        printf("    %s is %d years old\n", people[i].name, people[i].age);
    }
    
    // 嵌套结构体
    printf("  === Nested Structs ===\n");
    struct Address {
        char street[100];
        char city[50];
        int zip_code;
    };
    
    struct Employee {
        struct Person personal_info;
        struct Address work_address;
        double salary;
    };
    
    struct Employee emp = {
        {"David", 28},
        {"123 Main St", "New York", 10001},
        75000.0
    };
    
    printf("    Employee: %s, Age: %d, City: %s, Salary: $%.2f\n",
           emp.personal_info.name, emp.personal_info.age,
           emp.work_address.city, emp.salary);
    
    // 结构体指针
    printf("  === Struct Pointers ===\n");
    struct Point* ptr_to_point = &p1;
    
    printf("    Access via pointer: (%.1f, %.1f)\n", ptr_to_point->x, ptr_to_point->y);
    
    // 结构体作为函数参数
    printf("  === Struct as Function Parameters ===\n");
    double distance(struct Point p1, struct Point p2) {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return sqrt(dx*dx + dy*dy);
    }
    
    double dist = distance(p1, p2);
    printf("    Distance between p1 and p2: %.2f\n", dist);
    
    // 位域结构体
    printf("  === Bit Fields ===\n");
    struct Flags {
        unsigned int flag1 : 1;  // 1位
        unsigned int flag2 : 1;  // 1位
        unsigned int value : 4;  // 4位
        unsigned int type : 2;   // 2位
        unsigned int : 0;        // 强制对齐到下一个边界
        unsigned int reserved : 8; // 8位
    };
    
    struct Flags flags = {1, 0, 10, 2, 255};
    printf("    sizeof(struct Flags): %zu byte(s)\n", sizeof(struct Flags));
    printf("    flag1: %u, flag2: %u, value: %u, type: %u\n",
           flags.flag1, flags.flag2, flags.value, flags.type);
}
```

### 6.2 union

```c
#include <stdio.h>

void union_keyword() {
    printf("union Keyword:\n");
    
    // 基本联合体
    printf("  === Basic Union ===\n");
    union Data {
        int i;
        float f;
        char str[20];
    };
    
    union Data data;
    
    data.i = 10;
    printf("    data.i: %d\n", data.i);
    
    data.f = 220.5;
    printf("    data.f: %f\n", data.f);
    
    // 注意：此时data.i的值已经改变
    printf("    data.i after setting f: %d\n", data.i);
    
    strcpy(data.str, "C Programming");
    printf("    data.str: %s\n", data.str);
    
    printf("    sizeof(union Data): %zu byte(s)\n", sizeof(data));
    
    // 联合体的实际应用
    printf("  === Practical Applications ===\n");
    
    // 类型安全的联合体
    enum DataType { INTEGER, FLOAT, STRING };
    
    struct FlexibleData {
        enum DataType type;
        union {
            int i;
            float f;
            char str[20];
        } data;
    };
    
    struct FlexibleData flex_data;
    flex_data.type = STRING;
    strcpy(flex_data.data.str, "Hello Union");
    
    switch (flex_data.type) {
        case INTEGER:
            printf("    Integer: %d\n", flex_data.data.i);
            break;
        case FLOAT:
            printf("    Float: %f\n", flex_data.data.f);
            break;
        case STRING:
            printf("    String: %s\n", flex_data.data.str);
            break;
    }
    
    // 内存优化示例
    printf("  === Memory Optimization Example ===\n");
    union OptimizedStorage {
        struct {
            unsigned char r, g, b, a;
        } rgba;
        unsigned int color;
    };
    
    union OptimizedStorage pixel;
    pixel.rgba.r = 255;
    pixel.rgba.g = 128;
    pixel.rgba.b = 64;
    pixel.rgba.a = 255;
    
    printf("    RGBA: (%u, %u, %u, %u)\n",
           pixel.rgba.r, pixel.rgba.g, pixel.rgba.b, pixel.rgba.a);
    printf("    As integer: 0x%08X\n", pixel.color);
    
    // 联合体数组
    printf("  === Union Arrays ===\n");
    union Data array_of_unions[3];
    
    array_of_unions[0].i = 100;
    array_of_unions[1].f = 3.14f;
    strcpy(array_of_unions[2].str, "Array");
    
    printf("    Union array elements:\n");
    printf("      [0].i = %d\n", array_of_unions[0].i);
    printf("      [1].f = %f\n", array_of_unions[1].f);
    printf("      [2].str = %s\n", array_of_unions[2].str);
}
```

### 6.3 enum

```c
#include <stdio.h>

void enum_keyword() {
    printf("enum Keyword:\n");
    
    // 基本枚举
    printf("  === Basic Enum ===\n");
    enum Color {
        RED,
        GREEN,
        BLUE
    };
    
    enum Color my_color = RED;
    printf("    RED = %d\n", RED);
    printf("    GREEN = %d\n", GREEN);
    printf("    BLUE = %d\n", BLUE);
    printf("    my_color = %d\n", my_color);
    
    // 带值的枚举
    printf("  === Enum with Values ===\n");
    enum Priority {
        LOW = 10,
        MEDIUM = 50,
        HIGH = 100
    };
    
    printf("    Priority values:\n");
    printf("      LOW = %d\n", LOW);
    printf("      MEDIUM = %d\n", MEDIUM);
    printf("      HIGH = %d\n", HIGH);
    
    // 连续枚举值
    printf("  === Consecutive Enum Values ===\n");
    enum FileType {
        TEXT,      // 0
        BINARY,    // 1
        IMAGE,     // 2
        AUDIO      // 3
    };
    
    printf("    Consecutive values:\n");
    printf("      TEXT = %d\n", TEXT);
    printf("      BINARY = %d\n", BINARY);
    printf("      IMAGE = %d\n", IMAGE);
    printf("      AUDIO = %d\n", AUDIO);
    
    // 枚举在switch中的使用
    printf("  === Enum in switch ===\n");
    enum Direction { NORTH, SOUTH, EAST, WEST };
    enum Direction current_direction = EAST;
    
    switch (current_direction) {
        case NORTH:
            printf("    Heading North\n");
            break;
        case SOUTH:
            printf("    Heading South\n");
            break;
        case EAST:
            printf("    Heading East\n");
            break;
        case WEST:
            printf("    Heading West\n");
            break;
    }
    
    // 枚举与typedef结合
    printf("  === Enum with typedef ===\n");
    typedef enum {
        SUCCESS = 0,
        FAILURE = 1,
        WARNING = 2
    } Status;
    
    Status operation_result = SUCCESS;
    if (operation_result == SUCCESS) {
        printf("    Operation succeeded\n");
    }
    
    // 位枚举（用于标志）
    printf("  === Bit Enum (Flags) ===\n");
    enum Permissions {
        READ = 1,     // 001
        WRITE = 2,    // 010
        EXECUTE = 4   // 100
    };
    
    int permissions = READ | WRITE;  // 011
    printf("    Permissions: %d\n", permissions);
    
    if (permissions & READ) {
        printf("    Read permission granted\n");
    }
    
    if (permissions & EXECUTE) {
        printf("    Execute permission granted\n");
    } else {
        printf("    Execute permission denied\n");
    }
    
    // 枚举大小
    printf("  === Enum Size ===\n");
    printf("    sizeof(enum Color): %zu byte(s)\n", sizeof(enum Color));
    printf("    sizeof(enum Permissions): %zu byte(s)\n", sizeof(enum Permissions));
}
```

## 7. 函数相关关键字

### 7.1 return

```c
#include <stdio.h>
#include <stdlib.h>

void return_keyword() {
    printf("return Keyword:\n");
    
    // 基本返回语句
    printf("  === Basic Return ===\n");
    int add(int a, int b) {
        return a + b;
    }
    
    printf("    add(3, 4) = %d\n", add(3, 4));
    
    // 早期返回
    printf("  === Early Return ===\n");
    int safe_divide(int a, int b, int* result) {
        if (b == 0) {
            return -1;  // 错误代码
        }
        *result = a / b;
        return 0;  // 成功
    }
    
    int result;
    if (safe_divide(10, 2, &result) == 0) {
        printf("    10 / 2 = %d\n", result);
    }
    
    if (safe_divide(10, 0, &result) == -1) {
        printf("    Division by zero detected\n");
    }
    
    // void函数中的return
    printf("  === return in void Functions ===\n");
    void print_if_positive(int x) {
        if (x <= 0) {
            return;  // 提前退出
        }
        printf("    Positive number: %d\n", x);
    }
    
    print_if_positive(5);
    print_if_positive(-3);
    
    // 返回指针
    printf("  === Return Pointers ===\n");
    int* find_max(int* arr, int size) {
        if (size <= 0) return NULL;
        
        int* max = arr;
        for (int i = 1; i < size; ++i) {
            if (arr[i] > *max) {
                max = &arr[i];
            }
        }
        return max;
    }
    
    int numbers[] = {3, 7, 2, 9, 1};
    int* max_ptr = find_max(numbers, 5);
    if (max_ptr) {
        printf("    Maximum value: %d\n", *max_ptr);
    }
    
    // 返回结构体
    printf("  === Return Structs ===\n");
    struct Point {
        double x, y;
    };
    
    struct Point create_point(double x, double y) {
        struct Point p = {x, y};
        return p;  // 结构体会被复制
    }
    
    struct Point point = create_point(1.5, 2.5);
    printf("    Created point: (%.1f, %.1f)\n", point.x, point.y);
}
```

### 7.2 inline (C99)

```c
#include <stdio.h>

void inline_keyword() {
    printf("inline Keyword (C99):\n");
    
    // 基本inline函数
    printf("  === Basic inline Function ===\n");
    inline int square(int x) {
        return x * x;
    }
    
    int result = square(5);
    printf("    square(5) = %d\n", result);
    
    // inline的优势
    printf("  === Benefits of inline ===\n");
    printf("    - 减少函数调用开销\n");
    printf("    - 可能提高执行速度\n");
    printf("    - 适用于小型、频繁调用的函数\n");
    
    // inline的限制
    printf("  === Limitations of inline ===\n");
    printf("    - 编译器可能忽略inline建议\n");
    printf("    - 增加代码大小\n");
    printf("    - 不适用于大型或递归函数\n");
    
    // 实际示例
    printf("  === Practical Example ===\n");
    inline int max(int a, int b) {
        return (a > b) ? a : b;
    }
    
    inline int min(int a, int b) {
        return (a < b) ? a : b;
    }
    
    int x = 10, y = 20;
    printf("    max(%d, %d) = %d\n", x, y, max(x, y));
    printf("    min(%d, %d) = %d\n", x, y, min(x, y));
    
    // 循环中的inline函数
    printf("  === inline in Loops ===\n");
    int sum = 0;
    for (int i = 1; i <= 10; ++i) {
        sum += square(i);  // inline函数在循环中很有用
    }
    printf("    Sum of squares 1 to 10: %d\n", sum);
    
    // 与static结合使用
    printf("  === inline with static ===\n");
    static inline int cube(int x) {
        return x * x * x;
    }
    
    printf("    cube(3) = %d\n", cube(3));
    
    // 编译器优化
    printf("  === Compiler Optimization ===\n");
    printf("    现代编译器通常会自动内联合适的函数\n");
    printf("    即使没有inline关键字\n");
    printf("    inline只是给编译器的一个建议\n");
}
```

### 7.3 _Noreturn (C11) - noreturn (C23)

```c
#include <stdio.h>
#include <stdlib.h>

void noreturn_keyword() {
    printf("_Noreturn/noreturn Keywords (C11/C23):\n");
    
    // 基本noreturn函数
    printf("  === Basic noreturn Function ===\n");
    _Noreturn void fatal_error(const char* msg) {
        fprintf(stderr, "Fatal error: %s\n", msg);
        exit(EXIT_FAILURE);
    }
    
    // C23中的noreturn
    // noreturn void exit_program(void) {
    //     printf("Exiting program\n");
    //     exit(0);
    // }
    
    printf("    noreturn functions never return to caller\n");
    
    // 常见的noreturn函数
    printf("  === Common noreturn Functions ===\n");
    printf("    - exit()\n");
    printf("    - abort()\n");
    printf("    - longjmp()\n");
    printf("    - quick_exit()\n");
    
    // 自定义noreturn函数示例
    printf("  === Custom noreturn Examples ===\n");
    _Noreturn void assert_failure(const char* file, int line, const char* expr) {
        fprintf(stderr, "Assertion failed: %s at %s:%d\n", expr, file, line);
        abort();
    }
    
    _Noreturn void infinite_loop(void) {
        while (1) {
            // 永远不会返回
        }
    }
    
    // 编译器优化
    printf("  === Compiler Benefits ===\n");
    printf("    - 编译器可以进行更好的优化\n");
    printf("    - 消除不必要的代码分析\n");
    printf("    - 提供更准确的警告信息\n");
    
    // 静态分析工具
    printf("  === Static Analysis ===\n");
    printf("    - 静态分析工具可以检测不可达代码\n");
    printf("    - 帮助识别潜在的逻辑错误\n");
    
    // 实际应用
    printf("  === Practical Applications ===\n");
    _Noreturn void handle_critical_error(int error_code) {
        switch (error_code) {
            case 1:
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            case 2:
                fprintf(stderr, "File operation failed\n");
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Unknown critical error\n");
                exit(EXIT_FAILURE);
        }
    }
    
    printf("    noreturn helps document program flow\n");
}
```

## 8. C23新增关键字

### 8.1 true, false

```c
#include <stdio.h>
#include <stdbool.h>

void true_false_keywords() {
    printf("true, false Keywords (C23):\n");
    
    // 布尔值字面量
    printf("  === Boolean Literals ===\n");
    bool flag1 = true;
    bool flag2 = false;
    
    printf("    bool flag1 = true;   // %d\n", flag1);
    printf("    bool flag2 = false;  // %d\n", flag2);
    
    // 与C99 bool的对比
    printf("  === C99 vs C23 ===\n");
    printf("    C99: #include <stdbool.h> to use bool, true, false\n");
    printf("    C23: bool, true, false are built-in keywords\n");
    
    // 布尔运算
    printf("  === Boolean Operations ===\n");
    printf("    true && false = %d\n", true && false);
    printf("    true || false = %d\n", true || false);
    printf("    !true = %d\n", !true);
    
    // 在条件语句中使用
    printf("  === Use in Conditional Statements ===\n");
    bool condition = true;
    if (condition) {
        printf("    Condition is true\n");
    } else {
        printf("    Condition is false\n");
    }
    
    // 布尔值函数
    printf("  === Boolean Functions ===\n");
    bool is_even(int n) {
        return (n % 2) == 0;
    }
    
    for (int i = 1; i <= 5; ++i) {
        printf("    %d is %s\n", i, is_even(i) ? "even" : "odd");
    }
}
```

### 8.2 nullptr

```c
#include <stdio.h>

void nullptr_keyword() {
    printf("nullptr Keyword (C23):\n");
    
    // 空指针常量
    printf("  === Null Pointer Constant ===\n");
    int* ptr1 = nullptr;
    char* ptr2 = nullptr;
    void* ptr3 = nullptr;
    
    printf("    int* ptr1 = nullptr;   // %p\n", (void*)ptr1);
    printf("    char* ptr2 = nullptr;  // %p\n", (void*)ptr2);
    printf("    void* ptr3 = nullptr;  // %p\n", (void*)ptr3);
    
    // 与NULL的对比
    printf("  === nullptr vs NULL ===\n");
    printf("    C传统: #define NULL ((void*)0) or 0\n");
    printf("    C23: nullptr is a keyword of type nullptr_t\n");
    
    // 类型安全
    printf("  === Type Safety ===\n");
    // int* ip = NULL;     // 可能警告（从整数转换）
    // int* ip = nullptr;  // 完全类型安全
    
    // 函数参数
    printf("  === Function Parameters ===\n");
    void process_string(const char* str) {
        if (str == nullptr) {
            printf("    Null string provided\n");
            return;
        }
        printf("    String: %s\n", str);
    }
    
    process_string("Hello");
    process_string(nullptr);
    
    // 条件检查
    printf("  === Conditional Checks ===\n");
    int* dynamic_array = nullptr;
    
    if (dynamic_array == nullptr) {
        printf("    Array is not allocated\n");
    }
    
    // 分配内存后
    // dynamic_array = malloc(10 * sizeof(int));
    // if (dynamic_array != nullptr) {
    //     printf("    Array allocated successfully\n");
    // }
}
```

### 8.3 alignas, alignof

```c
#include <stdio.h>
#include <stdalign.h>

void alignas_alignof_keywords() {
    printf("alignas, alignof Keywords (C23):\n");
    
    // alignas指定对齐要求
    printf("  === alignas Alignment ===\n");
    
    // 指定对齐为16字节
    struct alignas(16) AlignedStruct {
        int x;
        double y;
    };
    
    // 使用对齐值
    alignas(32) int aligned_int;
    
    printf("    sizeof(struct AlignedStruct): %zu\n", sizeof(struct AlignedStruct));
    printf("    _Alignof(struct AlignedStruct): %zu\n", _Alignof(struct AlignedStruct));
    printf("    _Alignof(aligned_int): %zu\n", _Alignof(aligned_int));
    
    // alignof查询对齐要求
    printf("  === alignof Alignment Queries ===\n");
    printf("    alignof(char): %zu\n", alignof(char));
    printf("    alignof(int): %zu\n", alignof(int));
    printf("    alignof(double): %zu\n", alignof(double));
    printf("    alignof(void*): %zu\n", alignof(void*));
    printf("    alignof(max_align_t): %zu\n", alignof(max_align_t));
    
    // 实际应用示例
    printf("  === Practical Examples ===\n");
    
    // 为SIMD优化对齐数据
    struct SIMDData {
        alignas(16) float data[4];  // 16字节对齐，适合SSE指令
    };
    
    // 硬件寄存器对齐
    struct HardwareRegister {
        alignas(64) unsigned int control;  // 缓存行对齐
        alignas(64) unsigned int status;
    };
    
    printf("    sizeof(struct SIMDData): %zu\n", sizeof(struct SIMDData));
    printf("    sizeof(struct HardwareRegister): %zu\n", sizeof(struct HardwareRegister));
    
    // 与_Alignas/_Alignof的对比
    printf("  === C11 vs C23 ===\n");
    printf("    C11: _Alignas, _Alignof (underscores)\n");
    printf("    C23: alignas, alignof (no underscores)\n");
}
```

### 8.4 constexpr

```c
#include <stdio.h>

void constexpr_keyword() {
    printf("constexpr Keyword (C23):\n");
    
    // 编译时常量表达式
    printf("  === Compile-time Constants ===\n");
    constexpr int compile_time_constant = 100;
    constexpr int square(int x) { return x * x; }
    constexpr int result = square(5);
    
    printf("    constexpr int compile_time_constant = %d;\n", compile_time_constant);
    printf("    constexpr int result = square(5) = %d\n", result);
    
    // 数组大小
    printf("  === Array Sizes ===\n");
    constexpr int array_size() { return 10; }
    constexpr int size = array_size();
    int test_array[size];  // 编译时确定大小
    
    printf("    Array size determined at compile time: %zu\n", sizeof(test_array)/sizeof(test_array[0]));
    
    // 条件编译
    printf("  === Conditional Compilation ===\n");
    constexpr bool is_debug_mode() { return true; }
    
    if (is_debug_mode()) {
        printf("    Debug mode enabled\n");
    }
    
    // constexpr函数限制
    printf("  === constexpr Function Restrictions ===\n");
    printf("    - 只能包含一条return语句（C23）\n");
    printf("    - 必须在编译时可求值\n");
    printf("    - 不能有副作用\n");
    printf("    - 不能调用非constexpr函数\n");
    
    // 实际应用
    printf("  === Practical Applications ===\n");
    constexpr int max_buffer_size() { return 1024; }
    constexpr int alignment_requirement() { return 16; }
    
    printf("    Max buffer size: %d\n", max_buffer_size());
    printf("    Alignment requirement: %d\n", alignment_requirement());
    
    // 与static_assert结合使用
    printf("  === With static_assert ===\n");
    static_assert(max_buffer_size() > 0, "Buffer size must be positive");
    printf("    static_assert passed\n");
}
```

### 8.5 typeof, typeof_unqual

```c
#include <stdio.h>

void typeof_keywords() {
    printf("typeof, typeof_unqual Keywords (C23):\n");
    
    // typeof推导表达式类型
    printf("  === typeof Type Deduction ===\n");
    int x = 42;
    typeof(x) y = x;  // y的类型是int
    
    printf("    int x = 42;\n");
    printf("    typeof(x) y = x;  // y is int, value: %d\n", y);
    
    // 表达式类型推导
    printf("  === Expression Type Deduction ===\n");
    const int cx = 10;
    typeof(cx) cz = cx;        // const int
    typeof((cx)) cref = cx;    // const int (表达式是左值)
    
    printf("    const int cx = 10;\n");
    printf("    typeof(cx) cz = cx;     // type: const int\n");
    printf("    typeof((cx)) cref = cx; // type: const int (lvalue)\n");
    
    // 复杂表达式
    printf("  === Complex Expressions ===\n");
    double a = 3.14, b = 2.71;
    typeof(a + b) sum = a + b;  // double
    
    printf("    typeof(a + b) sum = %.2f;  // type: %s\n", sum, "double");
    
    // typeof_unqual去除限定符
    printf("  === typeof_unqual ===\n");
    const volatile int cv_int = 100;
    typeof(cv_int) same_type = cv_int;           // const volatile int
    typeof_unqual(cv_int) unqualified = cv_int;  // int
    
    printf("    typeof(cv_int): const volatile int\n");
    printf("    typeof_unqual(cv_int): int\n");
    
    // 实际应用
    printf("  === Practical Applications ===\n");
    #define SWAP(a, b) do { \
        typeof(a) temp = (a); \
        (a) = (b); \
        (b) = temp; \
    } while(0)
    
    int i1 = 10, i2 = 20;
    printf("    Before swap: i1=%d, i2=%d\n", i1, i2);
    SWAP(i1, i2);
    printf("    After swap: i1=%d, i2=%d\n", i1, i2);
    
    double d1 = 1.5, d2 = 2.5;
    printf("    Before swap: d1=%.1f, d2=%.1f\n", d1, d2);
    SWAP(d1, d2);
    printf("    After swap: d1=%.1f, d2=%.1f\n", d1, d2);
}
```

## 9. 预处理器关键字

### 9.1 #define, #undef

```c
#include <stdio.h>

void preprocessor_directives() {
    printf("Preprocessor Directives:\n");
    
    // #define 宏定义
    printf("  === #define Macro ===\n");
    #define PI 3.14159
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
    #define SQUARE(x) ((x) * (x))
    
    printf("    PI = %f\n", PI);
    printf("    MAX(3, 7) = %d\n", MAX(3, 7));
    printf("    SQUARE(5) = %d\n", SQUARE(5));
    
    // 字符串化和连接
    printf("  === Stringizing and Concatenation ===\n");
    #define STRINGIFY(x) #x
    #define CONCAT(a, b) a##b
    
    printf("    STRINGIFY(hello) = %s\n", STRINGIFY(hello));
    
    int xy = 100;
    printf("    CONCAT(x, y) = %d\n", xy);
    
    // #undef 取消宏定义
    printf("  === #undef Directive ===\n");
    #undef PI
    #define PI 3.14159265359
    printf("    PI redefined: %f\n", PI);
    
    // 条件编译
    printf("  === Conditional Compilation ===\n");
    #define DEBUG 1
    #define VERSION 2
    
    #if DEBUG
        printf("    Debug mode enabled\n");
    #else
        printf("    Release mode\n");
    #endif
    
    #if VERSION == 1
        printf("    Version 1\n");
    #elif VERSION == 2
        printf("    Version 2\n");
    #else
        printf("    Unknown version\n");
    #endif
    
    // 防止重复包含
    printf("  === Include Guards ===\n");
    printf("    #ifndef MY_HEADER_H\n");
    printf("    #define MY_HEADER_H\n");
    printf("    \n");
    printf("    // header content\n");
    printf("    \n");
    printf("    #endif // MY_HEADER_H\n");
    
    // 预定义宏
    printf("  === Predefined Macros ===\n");
    printf("    __FILE__: %s\n", __FILE__);
    printf("    __LINE__: %d\n", __LINE__);
    printf("    __DATE__: %s\n", __DATE__);
    printf("    __TIME__: %s\n", __TIME__);
}
```

### 9.2 #include

```c
#include <stdio.h>

void include_directive() {
    printf("#include Directive:\n");
    
    // 系统头文件
    printf("  === System Headers ===\n");
    printf("    #include <stdio.h>     // 标准输入输出\n");
    printf("    #include <stdlib.h>    // 标准库函数\n");
    printf("    #include <string.h>    // 字符串处理\n");
    printf("    #include <math.h>      // 数学函数\n");
    
    // 用户头文件
    printf("  === User Headers ===\n");
    printf("    #include \"myheader.h\"   // 用户定义头文件\n");
    printf("    #include \"../include/utils.h\"  // 相对路径\n");
    
    // 包含路径搜索顺序
    printf("  === Include Search Order ===\n");
    printf("    1. 当前目录\n");
    printf("    2. 编译器包含路径\n");
    printf("    3. 系统包含路径\n");
    
    // 条件包含
    printf("  === Conditional Include ===\n");
    #ifdef _WIN32
        // #include <windows.h>
        printf("    Windows specific includes\n");
    #elif defined(__linux__)
        // #include <unistd.h>
        printf("    Linux specific includes\n");
    #else
        printf("    Other platform includes\n");
    #endif
    
    // 包含保护
    printf("  === Include Protection ===\n");
    printf("    #pragma once  // 现代防重复包含方法\n");
    printf("    \n");
    printf("    // 传统方法\n");
    printf("    #ifndef HEADER_H\n");
    printf("    #define HEADER_H\n");
    printf("    // content\n");
    printf("    #endif\n");
}
```

## 10. 最佳实践和使用指南

### 10.1 关键字使用最佳实践

```c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void best_practices() {
    printf("Best Practices for C Keywords:\n");
    
    // 1. const关键字的最佳实践
    printf("  === const Best Practices ===\n");
    // 函数参数使用const
    void print_string(const char* str) {
        // 确保函数不会修改输入字符串
        printf("    %s\n", str);
    }
    
    // 指针和const的正确使用
    const int* ptr_to_const = NULL;    // 指向const数据的指针
    int* const const_ptr = NULL;       // const指针
    const int* const const_ptr_to_const = NULL; // const指针指向const数据
    
    printf("    Use const for function parameters to prevent accidental modification\n");
    printf("    Use const pointers to protect data integrity\n");
    
    // 2. static关键字的最佳实践
    printf("  === static Best Practices ===\n");
    // 文件作用域static变量
    static int call_count = 0;
    
    void count_calls() {
        static int local_count = 0;  // 只初始化一次
        ++call_count;
        ++local_count;
        printf("    Global calls: %d, Local calls: %d\n", call_count, local_count);
    }
    
    count_calls();
    count_calls();
    
    // 3. inline关键字的最佳实践
    printf("  === inline Best Practices ===\n");
    // 小型、频繁调用的函数适合内联
    static inline int min(int a, int b) {
        return (a < b) ? a : b;
    }
    
    printf("    inline small, frequently called functions\n");
    printf("    Avoid inlining large or recursive functions\n");

    // 4. volatile关键字的最佳实践
    printf("  === volatile Best Practices ===\n");
    // 硬件寄存器访问
    volatile unsigned int* hardware_register = (volatile unsigned int*)0x12345678;
    printf("    Use volatile for memory-mapped hardware registers\n");
    
    // 多线程共享变量
    volatile int shared_counter = 0;
    printf("    Use volatile for variables shared with signal handlers\n");
    
    // 5. restrict关键字的最佳实践
    printf("  === restrict Best Practices ===\n");
    void vector_add(int n, const float* restrict a, 
                   const float* restrict b, float* restrict c) {
        for (int i = 0; i < n; i++) {
            c[i] = a[i] + b[i];
        }
    }
    printf("    Use restrict for performance-critical functions with non-overlapping arrays\n");
    
    // 6. _Atomic关键字的最佳实践
    printf("  === _Atomic Best Practices ===\n");
    _Atomic int atomic_counter = 0;
    atomic_fetch_add(&atomic_counter, 1);
    printf("    Use _Atomic for thread-safe counters and flags\n");
    
    // 7. 类型定义最佳实践
    printf("  === Typedef Best Practices ===\n");
    typedef uint32_t pixel_t;  // 明确的数据类型
    typedef int (*comparator_t)(const void*, const void*);  // 函数指针类型
    
    printf("    Use typedef for improved code readability and portability\n");
    
    // 8. 枚举最佳实践
    printf("  === Enum Best Practices ===\n");
    typedef enum {
        STATE_IDLE = 0,
        STATE_RUNNING,
        STATE_ERROR
    } system_state_t;
    
    system_state_t current_state = STATE_IDLE;
    printf("    Use enums for finite state machines and option sets\n");
}
```

### 10.2 性能优化建议

```c
#include <stdio.h>
#include <time.h>

void performance_optimization() {
    printf("Performance Optimization Tips:\n");
    
    // 1. register关键字的现代使用
    printf("  === register Optimization ===\n");
    // 现代编译器通常自动优化寄存器分配
    for (register int i = 0; i < 1000000; i++) {
        // 频繁使用的循环变量
    }
    printf("    Let compiler handle register allocation in most cases\n");
    
    // 2. const和编译时优化
    printf("  === const Optimization ===\n");
    const int buffer_size = 1024;
    char buffer[buffer_size];  // 编译时常量允许VLAs（C99）
    
    printf("    Use const for values known at compile time\n");
    
    // 3. static和内存布局优化
    printf("  === static Optimization ===\n");
    static const char error_messages[][50] = {
        "Success",
        "Invalid parameter",
        "Out of memory"
    };
    printf("    static data has fixed addresses for faster access\n");
    
    // 4. inline函数优化
    printf("  === inline Optimization ===\n");
    static inline int clamp(int value, int min, int max) {
        return (value < min) ? min : (value > max) ? max : value;
    }
    
    int result = clamp(150, 0, 100);
    printf("    Small frequently-called functions benefit from inlining\n");
    
    // 5. restrict和编译器优化
    printf("  === restrict Optimization ===\n");
    void matrix_multiply(int n, double* restrict a, 
                        double* restrict b, double* restrict c) {
        // 编译器可以 vectorize 这个循环
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                c[i*n + j] = 0;
                for (int k = 0; k < n; k++) {
                    c[i*n + j] += a[i*n + k] * b[k*n + j];
                }
            }
        }
    }
    printf("    restrict enables aggressive optimization for numerical code\n");
    
    // 6. 内存访问模式优化
    printf("  === Memory Access Patterns ===\n");
    struct Data {
        int important;
        char padding[60];  // 防止false sharing
        int rarely_used;
    };
    
    struct Data data_array[100];
    printf("    Structure layout affects cache performance\n");
}
```

### 10.3 安全编程指南

```c
#include <stdio.h>
#include <string.h>

void security_guidelines() {
    printf("Security Programming Guidelines:\n");
    
    // 1. const和代码安全
    printf("  === const Security ===\n");
    void process_user_input(const char* user_input) {
        // 保证不修改用户输入
        size_t len = strlen(user_input);
        printf("    Processing %zu characters safely\n", len);
    }
    printf("    Use const to prevent accidental modification of critical data\n");
    
    // 2. 边界检查和安全
    printf("  === Bounds Checking ===\n");
    void safe_copy(char* dest, const char* src, size_t dest_size) {
        if (dest_size == 0) return;
        size_t src_len = strlen(src);
        size_t copy_len = (src_len < dest_size - 1) ? src_len : dest_size - 1;
        strncpy(dest, src, copy_len);
        dest[copy_len] = '\0';
    }
    printf("    Always validate buffer sizes and string lengths\n");
    
    // 3. 整数溢出防护
    printf("  === Integer Overflow Protection ===\n");
    int safe_addition(int a, int b) {
        if ((b > 0 && a > INT_MAX - b) || 
            (b < 0 && a < INT_MIN - b)) {
            // 处理溢出
            return 0;
        }
        return a + b;
    }
    printf("    Check for integer overflow in arithmetic operations\n");
    
    // 4. 格式化字符串安全
    printf("  === Format String Safety ===\n");
    void safe_print(const char* format, const char* data) {
        printf("%s", data);  // 而不是 printf(data);
    }
    printf("    Never use user input as format string\n");
    
    // 5. 内存初始化安全
    printf("  === Memory Initialization ===\n");
    void secure_memory_handling() {
        char sensitive_data[256];
        // 使用前初始化
        memset(sensitive_data, 0, sizeof(sensitive_data));
        // 使用后清理
        memset(sensitive_data, 0, sizeof(sensitive_data));
    }
    printf("    Always initialize and clean sensitive memory\n");
    
    // 6. 错误处理最佳实践
    printf("  === Error Handling ===\n");
    FILE* safe_file_open(const char* filename, const char* mode) {
        FILE* file = fopen(filename, mode);
        if (file == NULL) {
            perror("File open failed");
            return NULL;
        }
        return file;
    }
    printf("    Always check return values and handle errors gracefully\n");
    
    // 7. 类型安全实践
    printf("  === Type Safety ===\n");
    typedef uint32_t user_id_t;
    typedef uint32_t group_id_t;
    
    void process_user(user_id_t uid, group_id_t gid) {
        // 编译器会阻止意外的类型混合
        printf("    Processing user %u in group %u\n", uid, gid);
    }
    printf("    Use strong typing with typedef to prevent type confusion\n");
}
```

## 11. 总结

### 11.1 C关键字演进总结

```c
#include <stdio.h>

void keyword_evolution_summary() {
    printf("C Keyword Evolution Summary:\n\n");
    
    printf("C89/C90 (ANSI C) - 32 Keywords:\n");
    printf("  auto, break, case, char, const, continue, default, do,\n");
    printf("  double, else, enum, extern, float, for, goto, if,\n");
    printf("  int, long, register, return, short, signed, sizeof, static,\n");
    printf("  struct, switch, typedef, union, unsigned, void, volatile, while\n\n");
    
    printf("C99 Additions - 5 Keywords:\n");
    printf("  _Bool, _Complex, _Imaginary, inline, restrict\n\n");
    
    printf("C11 Additions - 7 Keywords:\n");
    printf("  _Alignas, _Alignof, _Atomic, _Generic, _Noreturn, _Static_assert, _Thread_local\n\n");
    
    printf("C23 Additions - 12+ Keywords:\n");
    printf("  alignas, alignof, bool, constexpr, false, nullptr, static_assert,\n");
    printf("  thread_local, true, typeof, typeof_unqual, (additional keywords)\n\n");
    
    printf("Key Evolutionary Trends:\n");
    printf("  1. Type system enhancements (bool, nullptr, typeof)\n");
    printf("  2. Compile-time computation (constexpr, static_assert)\n");
    printf("  3. Memory model and concurrency (_Atomic, thread_local)\n");
    printf("  4. Syntax modernization (underscore removal)\n");
    printf("  5. Safety and security features\n\n");
    
    printf("Future Directions:\n");
    printf("  - Enhanced module system\n");
    printf("  - Improved template/metaprogramming support\n");
    printf("  - Better concurrency primitives\n");
    printf("  - Additional safety features\n");
}
```

### 11.2 学习资源和建议

```c
#include <stdio.h>

void learning_resources() {
    printf("Learning Resources and Recommendations:\n\n");
    
    printf("Essential References:\n");
    printf("  1. ISO C Standard (latest draft)\n");
    printf("  2. \"The C Programming Language\" by Kernighan and Ritchie\n");
    printf("  3. C Reference on cppreference.com\n");
    printf("  4. Compiler documentation (GCC, Clang, MSVC)\n\n");
    
    printf("Practice Recommendations:\n");
    printf("  1. Start with basic keywords and gradually explore advanced features\n");
    printf("  2. Experiment with different compilers and optimization levels\n");
    printf("  3. Study real-world open source C projects\n");
    printf("  4. Practice secure coding techniques from the beginning\n\n");
    
    printf("Development Tools:\n");
    printf("  1. Modern compilers (GCC, Clang) with latest C standard support\n");
    printf("  2. Static analyzers (Clang Static Analyzer, Coverity)\n");
    printf("  3. Debuggers (GDB, LLDB)\n");
    printf("  4. Profilers (perf, Valgrind)\n\n");
    
    printf("Community Resources:\n");
    printf("  1. Stack Overflow C tag\n");
    printf("  2. C standard committee papers\n");
    printf("  3. Compiler development discussions\n");
    printf("  4. Security-focused C programming communities\n\n");
    
    printf("Learning Path Suggestions:\n");
    printf("  1. Master C89 fundamentals first\n");
    printf("  2. Gradually incorporate C99 features\n");
    printf("  3. Learn C11/C23 features as needed for specific projects\n");
    printf("  4. Focus on writing safe, portable, and maintainable code\n");
    printf("  5. Stay updated with language evolution through standards documents\n");
}

// 主函数示例
int main() {
    printf("=== C Keywords Complete Guide ===\n\n");
    
    best_practices();
    printf("\n");
    
    performance_optimization();
    printf("\n");
    
    security_guidelines();
    printf("\n");
    
    keyword_evolution_summary();
    printf("\n");
    
    learning_resources();
    printf("\n");
    
    printf("=== End of C Keywords Guide ===\n");
    return 0;
}
```

### 11.3 C 关键字功能分类表（按 C 标准版本）

| 功能类别 | 关键字 | C89/C90 | C99 | C11 | C23 | 功能描述 |
|---------|--------|---------|-----|-----|-----|---------|
| **基本数据类型** | `void` | ✓ | ✓ | ✓ | ✓ | 表示无类型或空类型 |
| | `char` | ✓ | ✓ | ✓ | ✓ | 字符类型 |
| | `int` | ✓ | ✓ | ✓ | ✓ | 整型 |
| | `float` | ✓ | ✓ | ✓ | ✓ | 单精度浮点型 |
| | `double` | ✓ | ✓ | ✓ | ✓ | 双精度浮点型 |
| | `_Bool` | | ✓ | ✓ | | 布尔类型(C99) |
| | `bool` | | | | ✓ | 布尔类型(C23) |
| | `_Complex` | | ✓ | ✓ | | 复数类型 |
| | `_Imaginary` | | ✓ | ✓ | | 虚数类型 |
| **类型限定符** | `const` | ✓ | ✓ | ✓ | ✓ | 定义不可修改的对象 |
| | `volatile` | ✓ | ✓ | ✓ | ✓ | 防止编译器优化 |
| | `restrict` | | ✓ | ✓ | ✓ | 指针限定符，指示唯一访问 |
| | `_Atomic` | | | ✓ | ✓ | 原子类型限定符 |
| **存储类说明符** | `auto` | ✓ | ✓ | ✓ | ✓ | 自动存储期(基本不使用) |
| | `register` | ✓ | ✓ | ✓ | ✓ | 建议使用寄存器存储 |
| | `static` | ✓ | ✓ | ✓ | ✓ | 静态存储期或内部链接 |
| | `extern` | ✓ | ✓ | ✓ | ✓ | 外部链接声明 |
| | `typedef` | ✓ | ✓ | ✓ | ✓ | 创建类型别名 |
| | `_Thread_local` | | | ✓ | ✓ | 线程局部存储 |
| **控制流语句** | `if` | ✓ | ✓ | ✓ | ✓ | 条件语句 |
| | `else` | ✓ | ✓ | ✓ | ✓ | 条件语句的替代分支 |
| | `switch` | ✓ | ✓ | ✓ | ✓ | 多分支选择语句 |
| | `case` | ✓ | ✓ | ✓ | ✓ | switch语句的分支标签 |
| | `default` | ✓ | ✓ | ✓ | ✓ | switch语句的默认分支 |
| | `for` | ✓ | ✓ | ✓ | ✓ | 循环语句 |
| | `while` | ✓ | ✓ | ✓ | ✓ | 循环语句 |
| | `do` | ✓ | ✓ | ✓ | ✓ | 循环语句 |
| | `break` | ✓ | ✓ | ✓ | ✓ | 跳出循环或switch |
| | `continue` | ✓ | ✓ | ✓ | ✓ | 继续下一次循环 |
| | `goto` | ✓ | ✓ | ✓ | ✓ | 无条件跳转 |
| | `_Noreturn` | | | ✓ | | 函数不返回(C11) |
| | `noreturn` | | | | ✓ | 函数不返回(C23) |
| **复合类型** | `struct` | ✓ | ✓ | ✓ | ✓ | 结构体类型 |
| | `union` | ✓ | ✓ | ✓ | ✓ | 联合体类型 |
| | `enum` | ✓ | ✓ | ✓ | ✓ | 枚举类型 |
| **函数相关** | `return` | ✓ | ✓ | ✓ | ✓ | 函数返回值 |
| | `inline` | | ✓ | ✓ | ✓ | 内联函数 |
| **运算符** | `sizeof` | ✓ | ✓ | ✓ | ✓ | 获取类型或对象大小 |
| | `_Alignof` | | | ✓ | | 获取对齐要求(C11) |
| | `alignof` | | | | ✓ | 获取对齐要求(C23) |
| **编译时特性** | `_Static_assert` | | | ✓ | | 编译时断言(C11) |
| | `static_assert` | | | | ✓ | 编译时断言(C23) |
| | `_Generic` | | | ✓ | ✓ | 泛型选择 |
| | `constexpr` | | | | ✓ | 编译时常量表达式(C23) |
| | `typeof` | | | | ✓ | 获取表达式类型(C23) |
| | `typeof_unqual` | | | | ✓ | 获取无限定符类型(C23) |
| **布尔值** | `true` | | | | ✓ | 布尔真值(C23) |
| | `false` | | | | ✓ | 布尔假值(C23) |
| **空指针** | `nullptr` | | | | ✓ | 空指针常量(C23) |
| **对齐说明** | `_Alignas` | | | ✓ | | 指定对齐要求(C11) |
| | `alignas` | | | | ✓ | 指定对齐要求(C23) |
| **有符号性** | `signed` | ✓ | ✓ | ✓ | ✓ | 有符号类型 |
| | `unsigned` | ✓ | ✓ | ✓ | ✓ | 无符号类型 |
| **大小修饰符** | `short` | ✓ | ✓ | ✓ | ✓ | 短整型修饰符 |
| | `long` | ✓ | ✓ | ✓ | ✓ | 长整型修饰符 |

**各版本关键字数量统计**：

- C89/C90: 32个关键字
- C99: 新增5个关键字（总计37个）
- C11: 新增7个关键字（总计44个）
- C23: 新增12+个关键字（预计总计56+个）

这个完整的C关键字详解文档涵盖了从基础到高级的所有C语言关键字，包括：

- 每个关键字的详细说明和示例代码
- 不同C标准版本的关键字演进
- 实际编程中的最佳实践
- 性能优化技巧
- 安全编程指南
- 学习资源和建议

文档采用了实用的代码示例和中文注释，便于理解和学习。每个章节都包含了实际应用场景和注意事项，适合不同水平的C程序员参考学习。
