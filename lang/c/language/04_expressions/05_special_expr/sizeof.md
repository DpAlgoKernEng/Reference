# sizeof 操作符

## 1. 概述 (Overview)

`sizeof` 是 C 语言中的编译时操作符，用于查询对象或类型的大小（以字节为单位）。当需要知道对象或类型的实际大小时使用此操作符。

### 核心特性

- **返回值类型**：`size_t`（无符号整数类型）
- **计算时机**：通常是编译时计算（变长数组 VLA 除外）
- **用途**：内存分配、数组遍历、类型大小查询

### 关键特性

- `sizeof(char)`、`sizeof(signed char)` 和 `sizeof(unsigned char)` 始终返回 1
- 可用于任何完整类型，但不能用于函数类型、不完整类型（包括 void）或位域左值
- 对于结构和联合类型，返回值包括内部和尾部填充字节

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

sizeof 操作符自 C 语言诞生之初就存在，最早出现在 C89/C90 标准中。其设计目的是提供一种可移植的方式来确定类型和对象的大小，避免硬编码假设。

### 版本演变

| 标准 | 版本 | 主要变更 |
|------|------|----------|
| C89/C90 | ISO/IEC 9899:1990 | 首次标准化，定义了基本的 sizeof 操作符 |
| C99 | ISO/IEC 9899:1999 | 新增对变长数组（VLA）的支持，VLA 的 sizeof 在运行时计算 |
| C11 | ISO/IEC 9899:2011 | 添加 _Alignof 操作符，与 sizeof 共享同一章节 |
| C17 | ISO/IEC 9899:2018 | 无重大变更 |
| C23 | ISO/IEC 9899:2024 | 将 _Alignof 更名为 alignof，sizeof 保持不变 |

### C99 的重要改进

C99 标准引入变长数组（VLA，Variable Length Array）后，sizeof 操作符的行为发生了重要变化：

- 对于 VLA 类型，sizeof 在**运行时**计算
- 对于非 VLA 类型，sizeof 仍然是**编译时**常量表达式
- 如果 VLA 的 size 表达式不影响 sizeof 的结果，是否求值该表达式是未指定的

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

sizeof 操作符有两种语法形式：

**形式 1：类型形式**
```c
sizeof( type )
```

**形式 2：表达式形式**
```c
sizeof expression
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `type` | 类型名，必须是完整类型（不能是函数类型、void 或不完整数组类型） |
| `expression` | 表达式，除了 VLA 类型外，表达式不会被求值 |

### 返回值

两种形式都返回 `size_t` 类型的值，表示对象表示的字节数。

### 关键规则

1. **类型形式**
   - 返回类型 `type` 的对象表示的字节数
   - 类型必须完整（fully defined）

2. **表达式形式**
   - 返回表达式类型的对象表示的字节数
   - 不对表达式应用隐式转换
   - 除 VLA 外，表达式不会被求值

### 限制

以下类型不能用于 sizeof：
- 函数类型（如 `void func(void)`）
- 不完整类型（如 `void`、未指定大小的数组 `char[]`）
- 位域左值

## 4. 底层原理 (Underlying Principles)

### 字节与位

根据计算机架构的不同，一个字节可能由 8 位或更多位组成。确切的位数由 `CHAR_BIT` 宏提供（定义在 `<limits.h>` 中）。在大多数现代系统中，`CHAR_BIT` 为 8。

### 对象表示

`sizeof` 返回的是对象表示（object representation）的大小，包括：

1. **成员占用空间**：所有数据成员的实际大小
2. **内部填充**（internal padding）：成员之间的对齐填充
3. **尾部填充**（trailing padding）：结构体末尾的填充字节

### 结构体填充原理

尾部填充的设计确保：如果该对象是数组的元素，则数组中下一个元素的对齐要求能够得到满足。换言之，`sizeof(T)` 返回的是 `T[]` 数组中一个元素的大小。

**示例**：
```c
struct Example {
    char c;    // 1 byte
    // 3 bytes padding (assuming 4-byte alignment for int)
    int i;     // 4 bytes
};             // sizeof(struct Example) = 8, not 5
```

### 编译时常量

对于非 VLA 类型，`sizeof` 的结果是编译时常量，可用于：

- 整型常量表达式
- 数组大小声明
- case 标签
- 位域宽度
- 枚举常量初始化

### VLA 的运行时计算

对于变长数组，sizeof 的行为特殊：

```c
int n = 5;
int vla[n];           // VLA
size_t size = sizeof(vla);  // 运行时计算，结果为 n * sizeof(int)
```

**重要细节**：
- 如果类型是 VLA 且改变其大小表达式的值不会影响 sizeof 的结果，则是否求值该大小表达式是未指定的
- 如果表达式的类型是 VLA，则表达式会被求值，数组大小在运行时计算

## 5. 使用场景 (Use Cases)

### 典型应用场景

#### 1. 内存分配

```c
int *arr = malloc(sizeof(int) * 100);  // 分配 100 个 int 的空间
```

#### 2. 计算数组元素个数

```c
int arr[] = {1, 2, 3, 4, 5};
size_t count = sizeof(arr) / sizeof(arr[0]);  // 计算元素个数为 5
```

**注意**：此方法仅适用于真正的数组，如果数组已退化为指针（如函数参数），则会得到错误结果。

#### 3. 避免硬编码类型大小

```c
// 不推荐：硬编码假设
int *p = malloc(400);  // 假设 int 是 4 字节

// 推荐：使用 sizeof
int *p = malloc(sizeof(int) * 100);  // 可移植性好
```

#### 4. 类型大小查询

```c
printf("int 大小：%zu 字节\n", sizeof(int));
printf("double 大小：%zu 字节\n", sizeof(double));
printf("指针大小：%zu 字节\n", sizeof(void*));
```

### 最佳实践

1. **使用 `sizeof` 而非硬编码大小**
   ```c
   // 好的做法
   memset(buffer, 0, sizeof(buffer));

   // 不好的做法
   memset(buffer, 0, 1024);  // 如果 buffer 大小改变会出错
   ```

2. **表达式形式不需要括号**
   ```c
   int x;
   size_t s1 = sizeof x;     // 正确，表达式形式
   size_t s2 = sizeof(x);    // 也正确，等同于 sizeof x
   size_t s3 = sizeof(int);  // 正确，类型形式必须有括号
   // size_t s4 = sizeof int; // 错误！类型形式必须使用括号
   ```

3. **使用 `%zu` 格式说明符打印 `size_t`**
   ```c
   printf("Size: %zu\n", sizeof(int));  // C99 起
   ```

### 常见陷阱

#### 陷阱 1：数组作为函数参数

```c
// 错误示例
void process(int arr[]) {
    size_t count = sizeof(arr) / sizeof(arr[0]);  // 错误！
    // arr 在函数参数中是指针，不是数组
}
```

**修正**：
```c
void process(int arr[], size_t count) {
    // 将数组大小作为参数传递
}
```

#### 陷阱 2：字符字面量的类型

```c
char c = 'a';
printf("%zu\n", sizeof c);   // 输出：1（char 类型）
printf("%zu\n", sizeof 'a');  // 输出：4（int 类型，C 语言中字符字面量是 int）
```

**注意**：在 C 语言中，字符字面量（如 `'a'`）的类型是 `int`，不是 `char`。

#### 陷阱 3：字符串字面量

```c
printf("%zu\n", sizeof "hello");  // 输出：6（包含终止空字符 '\0'）
```

字符串字面量 `"hello"` 的类型是 `char[6]`（包含空终止符）。

#### 陷阱 4：对指针使用 sizeof

```c
int arr[10];
int *p = arr;

printf("%zu\n", sizeof(arr));  // 输出：40（假设 int 为 4 字节）
printf("%zu\n", sizeof(p));    // 输出：8（64 位系统指针大小）
```

### C99 新特性：VLA 支持

```c
int n = 10;
int vla[n];
printf("%zu\n", sizeof(vla));  // 运行时计算，输出：40（假设 int 为 4 字节）
```

**注意**：VLA 的 sizeof 是运行时计算，不能用于编译时常量表达式。

## 6. 代码示例 (Examples)

### 示例 1：基本类型大小查询

```c
#include <stdio.h>
#include <limits.h>

int main(void) {
    // 基本类型大小
    printf("基本类型大小（字节）：\n");
    printf("  char:          %zu\n", sizeof(char));
    printf("  short:         %zu\n", sizeof(short));
    printf("  int:           %zu\n", sizeof(int));
    printf("  long:          %zu\n", sizeof(long));
    printf("  long long:     %zu\n", sizeof(long long));
    printf("  float:         %zu\n", sizeof(float));
    printf("  double:        %zu\n", sizeof(double));
    printf("  void*:         %zu\n", sizeof(void*));
    printf("  CHAR_BIT:      %d (bits per byte)\n", CHAR_BIT);

    return 0;
}
```

**典型输出**（64 位系统）：
```
基本类型大小（字节）：
  char:          1
  short:         2
  int:           4
  long:          8
  long long:     8
  float:         4
  double:        8
  void*:         8
  CHAR_BIT:      8 (bits per byte)
```

### 示例 2：数组和指针

```c
#include <stdio.h>

int main(void) {
    int arr[10];
    int *ptr = arr;

    printf("sizeof(arr) = %zu\n", sizeof(arr));     // 40
    printf("sizeof(ptr) = %zu\n", sizeof(ptr));     // 8 (64-bit)
    printf("Element count = %zu\n", sizeof(arr) / sizeof(arr[0]));  // 10

    // 字符串字面量
    printf("sizeof(\"hello\") = %zu\n", sizeof("hello"));  // 6

    return 0;
}
```

### 示例 3：结构体和填充

```c
#include <stdio.h>
#include <stddef.h>

struct Packed {
    char a;    // 1 byte
    int b;     // 4 bytes
    char c;    // 1 byte
};             // 可能填充到 12 字节（取决于对齐要求）

struct Example {
    char c;    // 1 byte
               // 3 bytes padding
    int i;     // 4 bytes
};

int main(void) {
    printf("sizeof(struct Packed) = %zu\n", sizeof(struct Packed));
    printf("sizeof(struct Example) = %zu\n", sizeof(struct Example));

    printf("Offset of a: %zu\n", offsetof(struct Packed, a));
    printf("Offset of b: %zu\n", offsetof(struct Packed, b));
    printf("Offset of c: %zu\n", offsetof(struct Packed, c));

    return 0;
}
```

**典型输出**：
```
sizeof(struct Packed) = 12
sizeof(struct Example) = 8
Offset of a: 0
Offset of b: 4
Offset of c: 8
```

### 示例 4：动态内存分配

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // 分配单个对象
    int *p = malloc(sizeof(int));
    if (p != NULL) {
        *p = 42;
        printf("Value: %d\n", *p);
        free(p);
    }

    // 分配数组
    size_t n = 10;
    double *arr = malloc(sizeof(double) * n);
    if (arr != NULL) {
        for (size_t i = 0; i < n; i++) {
            arr[i] = i * 1.5;
        }
        free(arr);
    }

    return 0;
}
```

### 示例 5：VLA 示例（C99）

```c
#include <stdio.h>

int main(void) {
    int n;
    printf("Enter array size: ");
    scanf("%d", &n);

    int vla[n];  // 变长数组

    for (int i = 0; i < n; i++) {
        vla[i] = i * i;
    }

    printf("VLA size: %zu bytes\n", sizeof(vla));
    printf("Element count: %zu\n", sizeof(vla) / sizeof(vla[0]));

    return 0;
}
```

### 示例 6：常见错误对比

```c
#include <stdio.h>

// 错误示例：数组作为函数参数
void wrong_count(int arr[]) {
    // 错误！arr 是指针，不是数组
    printf("Wrong count: %zu\n", sizeof(arr) / sizeof(arr[0]));
}

// 正确做法：传递数组大小
void right_count(int arr[], size_t size) {
    printf("Right count: %zu\n", size);
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5};

    // 正确：在定义数组的作用域中使用 sizeof
    size_t count = sizeof(arr) / sizeof(arr[0]);
    printf("Array element count: %zu\n", count);

    // 调用函数
    wrong_count(arr);   // 输出错误结果（指针大小 / int 大小）
    right_count(arr, count);  // 输出正确结果

    return 0;
}
```

### 示例 7：综合示例（来自 cppreference）

```c
#include <stdio.h>

int main(void) {
    short x;

    // 类型参数形式
    printf("sizeof(float)          = %zu\n", sizeof(float));
    printf("sizeof(void(*)(void))  = %zu\n", sizeof(void(*)(void)));
    printf("sizeof(char[10])       = %zu\n", sizeof(char[10]));

    // 表达式参数形式
    printf("sizeof 'a'             = %zu\n", sizeof 'a');  // 'a' 的类型是 int
    printf("sizeof &main           = %zu\n", sizeof &main);
    printf("sizeof \"hello\"         = %zu\n", sizeof "hello");  // 类型是 char[6]
    printf("sizeof x               = %zu\n", sizeof x);  // x 的类型是 short
    printf("sizeof (x+1)           = %zu\n", sizeof(x + 1));  // x+1 的类型是 int

    return 0;
}
```

**可能输出**（64 位指针，32 位 int）：
```
sizeof(float)          = 4
sizeof(void(*)(void))  = 8
sizeof(char[10])       = 10
sizeof 'a'             = 4
sizeof &main           = 8
sizeof "hello"         = 6
sizeof x               = 2
sizeof (x+1)           = 4
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| **操作符类型** | 编译时一元操作符（VLA 除外） |
| **返回类型** | `size_t`（无符号整数类型） |
| **语法形式** | `sizeof(type)` 或 `sizeof expression` |
| **表达式求值** | 非 VLA 表达式不求值；VLA 表达式求值 |
| **编译时常量** | 非 VLA 可用于整型常量表达式 |

### sizeof 与其他操作的对比

| 操作符/函数 | 编译时/运行时 | 用途 |
|-------------|---------------|------|
| `sizeof` | 编译时（VLA 除外） | 获取类型或对象大小 |
| `_Alignof`/`alignof` | 编译时 | 获取对齐要求 |
| `strlen()` | 运行时 | 计算字符串长度 |

### 关键注意事项

1. **数组与指针的区别**
   - 数组：`sizeof` 返回整个数组大小
   - 指针：`sizeof` 返回指针本身大小

2. **字符字面量的类型**
   - C 语言：`'a'` 是 `int` 类型
   - C++：`'a'` 是 `char` 类型

3. **字符串字面量**
   - 包含终止空字符 `'\0'`
   - `sizeof("hello")` 返回 6

4. **结构体填充**
   - `sizeof` 包括内部和尾部填充
   - 不要假设结构体大小等于成员大小之和

### 学习建议

1. **掌握基础用法**：熟练使用 `sizeof` 查询类型大小、计算数组元素个数
2. **理解填充机制**：学习结构体内存布局和对齐填充原理
3. **避免常见陷阱**：注意数组退化、指针与数组的区别
4. **理解 C99 新特性**：掌握 VLA 的运行时大小计算
5. **使用正确格式**：打印 `size_t` 使用 `%zu`（C99 起）

### 相关操作符

- **`_Alignof` / `alignof`（C11/C23）**：查询类型的对齐要求
- **`offsetof`**：查询结构体成员的偏移量

### 参考资料

- C23 标准（ISO/IEC 9899:2024）：6.5.3.4 The sizeof and alignof operators
- C17 标准（ISO/IEC 9899:2018）：6.5.3.4 The sizeof and _Alignof operators
- C11 标准（ISO/IEC 9899:2011）：6.5.3.4 The sizeof and _Alignof operators
- C99 标准（ISO/IEC 9899:1999）：6.5.3.4 The sizeof operator
- C89/C90 标准（ISO/IEC 9899:1990）：3.3.3.4 The sizeof operator