# 数组声明 (Array Declaration)

## 1. 概述 (Overview)

数组（Array）是一种由连续分配的非空对象序列组成的类型，这些对象具有特定的**元素类型**。数组中对象的数量（即数组大小）在数组生命周期内不会改变。

数组是 C 语言中最基本、最重要的数据结构之一，它提供了一种将相同类型的多个元素组织在一起的方式。数组在内存中连续存储，这使得可以通过索引高效地访问任意元素，同时也是 C 语言指针运算的基础。

**主要特点**：
- 元素在内存中连续存储
- 支持随机访问（通过索引）
- 数组名在大多数表达式中会退化为指向首元素的指针
- 数组大小在生命周期内固定不变

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

数组作为 C 语言的核心特性，自 C89/C90 标准起就是语言规范的一部分。其设计借鉴了 B 语言和 BCPL 语言的数组概念。

### 版本演变

| 标准 | 变更内容 |
|------|----------|
| C89/C90 | 基础数组特性：常量大小数组、未知大小数组、数组到指针的转换 |
| C99 | 引入变长数组（VLA）、灵活数组成员、函数参数中的 `static` 关键字 |
| C11 | 新增 `__STDC_NO_VLA__` 宏用于检测 VLA 支持；`_Alignof` 操作符不触发数组到指针转换 |
| C23 | VLA 支持变为可选（仅自动存储期 VLA）；数组类型与其元素类型的限定符关联规则变更；`alignof` 操作符不触发转换；属性说明符支持 |

### 设计动机

数组的连续内存布局设计是为了：
1. 支持高效的指针运算
2. 与底层硬件内存模型直接对应
3. 提供零开销抽象，适合系统编程

## 3. 语法与参数 (Syntax and Parameters)

### 声明语法

```
[ static(可选) 限定符(可选) 表达式(可选) ] 属性说明符(可选)    // (1)
[ 限定符(可选) static(可选) 表达式(可选) ] 属性说明符(可选)    // (2)
[ 限定符(可选) * ] 属性说明符(可选)                          // (3)
```

**语法元素说明**：

| 元素 | 说明 |
|------|------|
| expression | 除逗号运算符外的任意表达式，指定数组元素数量 |
| qualifiers | `const`、`restrict` 或 `volatile` 的任意组合，仅允许在函数参数列表中使用 |
| static | 表示数组至少包含指定数量的元素（仅函数参数） |
| * | 表示未指定大小的 VLA（仅函数原型作用域） |
| attr-spec-seq | (C23) 可选的属性列表，应用于声明的数组 |

### 数组类型分类

1. **已知常量大小的数组**：表达式为大于零的整数常量表达式
2. **变长数组（VLA）**：表达式为非常量表达式（C99 起）
3. **未知大小的数组**：表达式被省略

### 函数参数中的特殊语法

在函数参数列表中，数组声明符允许使用额外语法：

```c
// 使用 static 表示数组最小大小
void func(int a[static 10]);  // 参数必须指向至少 10 个元素的数组

// 使用限定符
void func(const int a[const 10]);  // 第二个 const 限定指针本身
```

## 4. 底层原理 (Underlying Principles)

### 内存布局

数组元素在内存中连续存储，元素地址按其索引顺序递增：

```c
int a[5];  // 假设 int 为 4 字节
// 内存布局：&a[0] < &a[1] < &a[2] < &a[3] < &a[4]
// 相邻元素地址差值 = sizeof(int) = 4
```

### 数组到指针的转换规则

除以下情况外，任何数组类型的左值表达式都会隐式转换为指向其首元素的指针：

1. 作为 `&`（取地址）运算符的操作数
2. 作为 `sizeof` 运算符的操作数
3. 作为 `typeof` 和 `typeof_unqual` 的操作数（C23 起）
4. 作为用于数组初始化的字符串字面量
5. 作为 `_Alignof`/`alignof` 的操作数（C11 起）

**转换结果不是左值**。

### 函数参数的数组退化

当数组类型用于函数参数列表时，它会被转换为相应的指针类型：

```c
// 以下两个声明等价
void f(int a[2]);
void f(int* a);
```

### 变长数组（VLA）的运行时分配

VLA 的存储空间在控制流经过声明时分配，其生命周期在声明作用域结束时终止。每次经过声明时，大小表达式都会被重新求值：

```c
void func(int n) {
    int a[n];  // 每次调用 func 时，n 可能不同，数组大小也随之变化
}
```

## 5. 使用场景 (Use Cases)

### 适用场景

1. **存储固定数量的同类型元素**
   ```c
   int scores[100];  // 存储 100 个分数
   ```

2. **字符串处理**
   ```c
   char name[50];    // 存储最多 49 个字符的字符串（含 '\0'）
   ```

3. **多维数据表示**
   ```c
   int matrix[3][4]; // 3x4 矩阵
   ```

4. **函数参数传递（配合 static）**
   ```c
   void process(double data[static 100]) {
       // 编译器可进行边界检查和优化
   }
   ```

5. **灵活数组成员（动态大小结构体）**
   ```c
   struct buffer {
       size_t len;
       char data[];  // 灵活数组成员
   };
   ```

### 最佳实践

1. **始终传递数组大小**：数组退化为指针后，函数无法知道原始大小
   ```c
   void process(int arr[], size_t size);
   ```

2. **使用 static 提示编译器优化**
   ```c
   void func(int a[static 10]);  // 明确告知编译器数组最小大小
   ```

3. **避免使用寄存器数组并转换**
   ```c
   register int a[5];
   int *p = a;  // 未定义行为
   ```

### 常见陷阱

1. **数组不可赋值**
   ```c
   int a[3], b[3];
   // a = b;  // 错误：数组不是可修改的左值
   ```

2. **sizeof 对数组与指针的区别**
   ```c
   int a[10];
   int *p = a;
   sizeof(a);  // 数组大小：10 * sizeof(int)
   sizeof(p);  // 指针大小：sizeof(int*)
   ```

3. **VLA 的作用域限制**
   ```c
   // int global_vla[n];  // 错误：VLA 不能在文件作用域
   // static int local_vla[n];  // 错误：VLA 不能有静态存储期
   ```

4. **未知大小数组的使用限制**
   ```c
   extern int x[];  // 不完整类型
   // sizeof(x);    // 错误：不能对不完整类型使用 sizeof
   ```

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

// 已知常量大小数组的声明与初始化
int main(void) {
    // 方式一：显式指定大小
    int a[5] = {1, 2, 3, 4, 5};

    // 方式二：由初始化器推断大小
    int b[] = {1, 2, 3};  // 等价于 int b[3]

    // 方式三：部分初始化（其余为零）
    int c[5] = {1, 2};    // {1, 2, 0, 0, 0}

    // 字符数组初始化
    char str[] = "hello";  // {'h', 'e', 'l', 'l', 'o', '\0'}

    // 多维数组
    int matrix[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    printf("a[0] = %d\n", a[0]);
    printf("str = %s\n", str);

    return 0;
}
```

### 函数参数中的数组

```c
#include <stdio.h>

// 使用 static 确保参数数组有最小大小
void print_array(int a[static 5], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

// 使用限定符
void add_to_array(double dest[restrict 10], const double src[restrict 10]) {
    // restrict 允许编译器优化，假设 dest 和 src 不重叠
    for (int i = 0; i < 10; i++) {
        dest[i] += src[i];
    }
}

// 指向数组的指针（不发生退化）
void process_matrix(int (*matrix)[3], int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    print_array(arr, 6);

    double d[10] = {0}, s[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    add_to_array(d, s);

    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    process_matrix(matrix, 2);

    return 0;
}
```

### 变长数组（VLA）

```c
#include <stdio.h>
#include <stdlib.h>

void process_vla(int n) {
    // VLA：运行时确定大小
    int vla[n];

    for (int i = 0; i < n; i++) {
        vla[i] = i * i;
    }

    printf("VLA 大小: %zu 字节\n", sizeof(vla));
    // 注意：sizeof 对 VLA 在运行时计算

    // 多维 VLA
    int matrix[n][n * 2];
}

// 函数原型中使用未指定大小的 VLA
void vla_func(int n, int a[*]);  // 原型作用域

// 实际定义
void vla_func(int n, int a[n]) {
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
}

int main(void) {
    process_vla(5);

    int arr[] = {10, 20, 30};
    vla_func(3, arr);

    return 0;
}
```

### 灵活数组成员

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 结构体末尾的灵活数组成员
struct string_buffer {
    size_t length;
    char data[];  // 灵活数组成员（C99 起）
};

int main(void) {
    // 一次分配内存给结构体和数据
    size_t len = 20;
    struct string_buffer *buf = malloc(sizeof(*buf) + len * sizeof(char));

    if (buf) {
        buf->length = len;
        strncpy(buf->data, "Hello, World!", len);
        printf("Length: %zu, Data: %s\n", buf->length, buf->data);
        free(buf);
    }

    return 0;
}
```

### 常见错误及修正

```c
#include <stdio.h>

int main(void) {
    // 错误 1：数组赋值
    int a[3] = {1, 2, 3}, b[3] = {4, 5, 6};
    // a = b;  // 编译错误！

    // 修正：使用循环或 memcpy
    for (int i = 0; i < 3; i++) a[i] = b[i];

    // 错误 2：文件作用域 VLA
    // extern int n;
    // int vla[n];  // 编译错误：VLA 不能有链接

    // 错误 3：VLA 作为结构体成员
    // struct bad {
    //     int n;
    //     int arr[n];  // 编译错误！
    // };

    // 修正：使用灵活数组成员或指针
    struct good {
        int n;
        int *arr;  // 使用指针
    };

    // 错误 4：静态存储期 VLA
    // void func(int n) {
    //     static int vla[n];  // 编译错误！
    // }

    // 错误 5：零长度数组（非标准）
    // int zero_arr[0];  // 非标准，某些编译器作为扩展支持

    // 修正：使用灵活数组成员或至少 1 元素

    // 错误 6：sizeof 对未知大小数组
    extern int unknown[];
    // sizeof(unknown);  // 编译错误！

    // 错误 7：对数组使用 _Atomic
    typedef int A[2];
    // _Atomic A a0;  // 编译错误！

    // 修正：创建原子类型的数组
    _Atomic int a2[2];  // 正确

    return 0;
}
```

### 限定符行为示例

```c
#include <stdio.h>

// C23 之前的限定符行为
typedef int A[2][3];

void demo_qualifiers(void) {
    const A a = {{4, 5, 6}, {7, 8, 9}};
    // a[0] 的类型是 const int*（指向 const int 的指针）

    // int* pi = a[0];  // 错误：const int* 不能赋值给 int*

    // C23 起：数组类型与其元素类型被视为相同限定
    // void* unqual_ptr = a;  // C23 中错误，之前可能允许

    // 原子类型的数组
    _Atomic int atomic_arr[3] = {0};  // 正确
    // _Atomic(int[3]) bad;  // 错误：不能对数组类型应用 _Atomic
}
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 内存布局 | 连续存储，支持指针运算 |
| 大小确定时机 | 常量数组：编译时；VLA：运行时 |
| 退化规则 | 除少数例外，数组名转换为指向首元素的指针 |
| 赋值限制 | 数组本身不可赋值，含数组成员的结构体可赋值 |

### 数组类型对比

| 类型 | 大小 | 存储期限制 | 引入版本 |
|------|------|-----------|----------|
| 常量大小数组 | 编译时确定 | 无限制 | C89 |
| 变长数组（VLA） | 运行时确定 | 仅自动或分配存储期 | C99 |
| 未知大小数组 | 不完整类型 | 需后续定义或初始化 | C89 |
| 灵活数组成员 | 动态分配 | 仅结构体最后成员 | C99 |

### 版本兼容性注意事项

1. **VLA 支持**：C99 引入，C11/C23 成为可选特性。使用前应检测 `__STDC_NO_VLA__` 宏。

2. **函数参数 static**：C99 引入，用于优化提示和边界检查。

3. **灵活数组成员**：C99 引入，替代非标准的零长度数组扩展。

4. **限定符语义**：C23 对数组类型与其元素类型的限定符关联规则有变更。

### 学习建议

1. 深入理解数组到指针的转换规则，这是 C 语言指针运算的基础。

2. 注意区分数组名与指针变量的 `sizeof` 结果。

3. 在函数参数中使用 `static` 提供额外信息给编译器，便于优化。

4. 对于动态大小的数据，优先使用灵活数组成员而非非标准扩展。

5. 编写可移植代码时，应检测 VLA 支持情况或避免使用 VLA。