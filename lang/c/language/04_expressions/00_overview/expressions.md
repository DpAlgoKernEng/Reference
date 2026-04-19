# C 语言表达式（Expressions）

## 1. 概述

表达式（Expression）是由**运算符（operators）**和**操作数（operands）**组成的序列，用于指定一个计算操作。

表达式求值可能产生以下效果：
- **产生结果**：例如 `2 + 2` 求值产生结果 `4`
- **产生副作用**：例如 `printf("%d", 4)` 向标准输出流发送字符 `'4'`
- **指定对象或函数**：例如标识符表达式指定一个变量或函数

表达式是 C 程序的核心构建块，几乎所有的计算操作都通过表达式来完成。理解表达式的工作原理是掌握 C 语言的基础。

## 2. 来源与演变

### 首次引入

表达式概念自 **C89/C90** 标准（ISO/IEC 9899:1990）即已存在，作为 C 语言的基础组成部分被定义。

### 历史背景

表达式的演变与 C 语言的发展紧密相关：

**C89/C90 标准**：
- 定义了基本表达式类型和运算符
- 规定了值类别（value categories）
- 确立了求值顺序的基本规则

**C99 标准（ISO/IEC 9899:1999）**：
- 新增复合字面量（compound literals）
- 引入变长数组（VLA），影响 sizeof 表达式
- 新增浮点运算控制和异常处理

**C11 标准（ISO/IEC 9899:2011）**：
- 新增泛型选择（generic selections）表达式
- 引入 `_Alignof` 运算符
- 增强浮点运算控制 pragma

**C17 标准（ISO/IEC 9899:2018）**：
- 对现有表达式规则进行澄清和小幅修正

**C23 标准（ISO/IEC 9899:2024）**：
- 新增预定义常量 `true`、`false`（类型为 `bool`）
- 新增预定义常量 `nullptr`（类型为 `nullptr_t`）
- `_Alignof` 改名为 `alignof`
- 新增 `char8_t` 类型支持
- 字符常量类型调整

### 版本对比表

| 特性 | C89/C90 | C99 | C11 | C23 |
|------|---------|-----|-----|-----|
| 复合字面量 | ❌ | ✅ | ✅ | ✅ |
| 泛型选择 | ❌ | ❌ | ✅ | ✅ |
| `_Alignof`/`alignof` | ❌ | ❌ | ✅ | ✅ (改名为 alignof) |
| `true`/`false` 常量 | ❌ | ❌ | ❌ | ✅ |
| `nullptr` | ❌ | ❌ | ❌ | ✅ |
| `char8_t` 类型 | ❌ | ❌ | ❌ | ✅ |

## 3. 语法与参数

### 主要表达式类型

#### 1. 主表达式（Primary Expressions）

主表达式是表达式的最基本形式，包括：

| 类型 | 示例 | 说明 |
|------|------|------|
| 常量和字面量 | `2`, `"Hello, world"` | 字面值表达式 |
| 适当声明的标识符 | `n`, `printf` | 变量名或函数名 |
| 泛型选择（C11 起） | `_Generic(...)` | 根据类型选择表达式 |
| 括号表达式 | `(a + b)` | 保证优先级 |

#### 2. 常量和字面量

**整型常量（Integer constants）**：
```c
int decimal = 42;        // 十进制
int octal = 052;         // 八进制（以 0 开头）
int hex = 0x2A;          // 十六进制（以 0x 开头）
```

**字符常量（Character constants）**：
- 类型为 `int`，可转换为字符类型
- C11 起：`char16_t`、`char32_t`、`wchar_t` 类型
- C23 起：`char8_t` 类型

**浮点常量（Floating constants）**：
```c
float f = 3.14f;
double d = 3.14;
long double ld = 3.14L;
```

**字符串字面量（String literals）**：
- 类型为字符数组：`char[]`, `char8_t[]`(C23), `char16_t[]`, `char32_t[]`(C11), `wchar_t[]`
- 表示以空字符结尾的字符串

**复合字面量（Compound literals，C99 起）**：
```c
// 直接在代码中创建结构体、联合或数组值
struct Point { int x; int y; };
struct Point p = (struct Point){10, 20};  // 复合字面量
int* arr = (int[]){1, 2, 3, 4, 5};        // 数组复合字面量
```

**预定义常量（C23 起）**：
```c
bool b = true;           // 预定义常量 true
bool b2 = false;         // 预定义常量 false
nullptr_t np = nullptr;  // 预定义常量 nullptr
```

### 运算符分类

| 类别 | 运算符 | 示例 |
|------|--------|------|
| **赋值** | `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `\|=``, `^=`, `<<=`, `>>=` | `a = b`, `a += b` |
| **自增/自减** | `++`, `--` | `++a`, `a++`, `--a`, `a--` |
| **算术** | `+`, `-`, `*`, `/`, `%`, `~`, `&`, `\|``, `^`, `<<`, `>>` | `a + b`, `a * b` |
| **逻辑** | `!`, `&&`, `\|\|`` | `!a`, `a && b`, `a \|\| b` |
| **比较** | `==`, `!=`, `<`, `>`, `<=`, `>=` | `a == b`, `a < b` |
| **成员访问** | `[]`, `*`, `&`, `->`, `.` | `a[b]`, `*a`, `&a`, `a->b`, `a.b` |
| **其他** | `()`, `,`, `(type)`, `?:`, `sizeof`, `_Alignof`(C23前)/`alignof`(C23起) | `a(...)`, `a, b`, `(int)a`, `a ? b : c` |

### 运算符优先级

运算符优先级定义了运算符绑定到其参数的顺序。优先级从高到低：

| 优先级 | 运算符 | 结合性 |
|--------|--------|--------|
| 1（最高） | `()` `[]` `->` `.` `++`(后缀) `--`(后缀) | 左到右 |
| 2 | `++`(前缀) `--`(前缀) `+`(一元) `-`(一元) `!` `~` `*` `&` `sizeof` `(type)` | 右到左 |
| 3 | `*` `/` `%` | 左到右 |
| 4 | `+` `-` | 左到右 |
| 5 | `<<` `>>` | 左到右 |
| 6 | `<` `<=` `>` `>=` | 左到右 |
| 7 | `==` `!=` | 左到右 |
| 8 | `&` | 左到右 |
| 9 | `^` | 左到右 |
| 10 | `\|`` | 左到右 |
| 11 | `&&` | 左到右 |
| 12 | `\|\|`` | 左到右 |
| 13 | `?:` | 右到左 |
| 14 | `=` `+=` `-=` `*=` `/=` `%=` `&=` `^=` `\|=` `<<=` `>>=` | 右到左 |
| 15（最低） | `,` | 左到右 |

### 替代表示

某些运算符有替代拼写（alternative representations）：

| 原运算符 | 替代拼写 |
|----------|----------|
| `&&` | `and` |
| `\|\|`` | `or` |
| `!` | `not` |
| `&` | `bitand` |
| `\|`` | `bitor` |
| `^` | `xor` |
| `~` | `compl` |
| `&=` | `and_eq` |
| `\|=` | `or_eq` |
| `^=` | `xor_eq` |
| `!=` | `not_eq` |

### 值类别（Value Categories）

表达式根据其值被分类为：

| 类别 | 说明 | 示例 |
|------|------|------|
| **左值（lvalue）** | 指定一个对象，可以取地址 | `x`, `arr[i]`, `*ptr` |
| **非左值对象** | 不代表对象的表达式 | `x + y`, `x++` |
| **函数指示符（function designator）** | 代表函数的表达式 | `printf`, `func_name` |

### 类型转换

**隐式转换（Implicit conversions）**：
当操作数的类型与运算符的期望不匹配时，自动进行类型转换。

**显式转换（Casts）**：
使用类型转换运算符显式转换值：
```c
int a = 10;
double b = (double)a / 3;  // 显式转换
```

### 常量表达式

常量表达式可在编译时求值，用于：
- 非变长数组（VLA，C99 起）的数组大小
- 静态初始化器
- 枚举常量值

## 4. 底层原理

### 求值顺序

C 语言对表达式求值顺序有特定规则：

1. **参数和子表达式的求值顺序**：
   - 大多数情况下，求值顺序是**未指定的（unspecified）**
   - 编译器可以按任意顺序求值

```c
int a = 1;
int b = a + ++a;  // 未定义行为！求值顺序未指定
```

2. **序列点（Sequence points）**：
   - 序列点定义了求值的确定时刻
   - 在序列点之前，所有副作用必须完成

**常见序列点**：
- 逻辑与 `&&` 的第一个操作数求值后
- 逻辑或 `||` 的第一个操作数求值后
- 逗号运算符 `,` 的第一个操作数求值后
- 条件运算符 `?:` 的第一个操作数求值后
- 函数调用时，参数求值完成后、函数体执行前
- 完整表达式结束时

### 未求值表达式

某些运算符的操作数**不会被求值**：

| 运算符 | 说明 |
|--------|------|
| `sizeof` | 操作数不求值（除非是 VLA，C99 起） |
| `_Alignof`/`alignof` | 操作数不求值 |
| 泛型选择的控制表达式 | 不求值 |
| VLA 的大小表达式（作为 `_Alignof`/`alignof` 的操作数时） | 不求值 |

```c
size_t n = sizeof(printf("%d", 4));  // 不会执行控制台输出！
// printf 不会被调用，sizeof 只计算返回类型的大小
```

### 浮点运算控制（C99 起）

浮点运算可能引发异常并通过 `math_errhandling` 报告错误。

**标准 pragma**：
- `FENV_ACCESS`：控制浮点环境访问
- `FP_CONTRACT`：控制浮点表达式收缩
- `CX_LIMITED_RANGE`：控制复数乘除优化

**浮点求值精度和舍入方向**：
- 可以通过 `fenv.h` 中的函数控制
- 影响浮点运算的执行方式

### 内存布局

表达式在内存中的行为取决于其值类别：

```
左值表达式:          非左值表达式:
┌─────────────┐     ┌─────────────┐
│ 内存地址    │     │ 临时值      │
│ 0x1000      │     │ (无地址)    │
│ 值: 42      │     │ 值: 42      │
└─────────────┘     └─────────────┘
可取地址 &expr      不可取地址
可赋值              不可赋值
```

## 5. 使用场景

### 适用场景

| 场景 | 推荐表达式类型 | 示例 |
|------|----------------|------|
| 数学计算 | 算术表达式 | `a + b * c`, `x % y` |
| 条件判断 | 比较和逻辑表达式 | `a > b`, `x && y` |
| 指针操作 | 指针表达式 | `*ptr`, `arr[i]`, `ptr->member` |
| 位操作 | 位运算表达式 | `flags \| BIT_MASK``, `val & 0xFF` |
| 类型转换 | 类型转换表达式 | `(int)float_val` |
| 编译时计算 | 常量表达式 | `sizeof(arr) / sizeof(arr[0])` |
| 类型泛型代码 | 泛型选择（C11 起） | `_Generic(x, int: f, double: g)` |

### 最佳实践

#### 1. 避免未定义的求值顺序

```c
// ❌ 错误：未定义行为
int a = 1;
int b = a + ++a;  // a 的修改和读取顺序未定义

// ✅ 正确：明确求值顺序
int a = 1;
int temp = a + 1;
int b = a + temp;  // 或分两步写
```

#### 2. 使用 sizeof 计算数组大小

```c
// ✅ 推荐：使用 sizeof
int arr[] = {1, 2, 3, 4, 5};
size_t count = sizeof(arr) / sizeof(arr[0]);  // count = 5

// ❌ 不推荐：硬编码大小
#define ARR_SIZE 5
int arr[ARR_SIZE] = {1, 2, 3, 4, 5};
```

#### 3. 合理使用括号明确优先级

```c
// ❌ 不清晰：依赖优先级记忆
int result = a & b | c && d;

// ✅ 清晰：使用括号
int result = (a & b) | (c && d);
```

#### 4. 复合字面量的使用（C99 起）

```c
// ✅ 临时对象使用复合字面量
void print_point(struct Point p);
print_point((struct Point){10, 20});  // 无需声明变量

// ✅ 数组操作
int* temp_arr = (int[]){1, 2, 3, 4, 5};
```

### 常见陷阱

#### 陷阱 1：运算符优先级误解

```c
// ❌ 常见错误：误解优先级
int a = 1, b = 2, c = 3;
int result = a & b + c;  // 等价于 a & (b + c)，不是 (a & b) + c

// ✅ 正确：使用括号
int result = (a & b) + c;
```

#### 陷阱 2：自增自减的副作用

```c
// ❌ 危险：序列点内的多次修改
int i = 0;
int arr[10];
arr[i++] = i;  // 未定义行为！i 的修改和读取无序列点分隔

// ✅ 正确：分开操作
int i = 0;
arr[i] = i + 1;
i++;
```

#### 陷阱 3：逗号运算符误用

```c
// ⚠️ 注意：逗号运算符返回最后一个值
int a = (1, 2, 3);  // a = 3

// ❌ 常见错误：混淆函数调用参数
printf("%d, %d", a, b);  // 函数参数，不是逗号运算符
int result = (a, b);    // 逗号运算符，result = b
```

#### 陷阱 4：sizeof 与 VLA（C99 起）

```c
// ⚠️ 注意：VLA 的 sizeof 会求值
int n = 5;
int vla[n];
size_t s = sizeof(int[n++]);  // n 会递增！n = 6

// ✅ 静态数组的 sizeof 不求值
int arr[5];
size_t s2 = sizeof(arr[n++]);  // n 不变，还是 5
```

#### 陷阱 5：逻辑运算的短路求值

```c
// ⚠️ 注意：逻辑运算的短路特性
int* ptr = NULL;
if (ptr != NULL && *ptr == 10) {  // 安全：ptr 为 NULL 时不会解引用
    // ...
}

// ❌ 错误：忽略短路特性
if (ptr != NULL || *ptr == 10) {  // 危险：ptr 为 NULL 时仍会解引用！
    // ...
}
```

### 性能考虑

1. **编译时常量表达式**：优先使用常量表达式，允许编译器优化
2. **避免不必要的类型转换**：可能引入额外指令
3. **位运算优化**：乘除 2 的幂次可用移位替代

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>
#include <stddef.h>

int main(void) {
    // 主表达式
    int x = 10;              // 标识符表达式
    const char* msg = "Hello";  // 字符串字面量

    // 算术表达式
    int a = 5, b = 3;
    printf("a + b = %d\n", a + b);   // 8
    printf("a - b = %d\n", a - b);   // 2
    printf("a * b = %d\n", a * b);   // 15
    printf("a / b = %d\n", a / b);   // 1
    printf("a %% b = %d\n", a % b);  // 2

    // 比较表达式
    printf("a == b: %d\n", a == b);  // 0 (false)
    printf("a != b: %d\n", a != b);  // 1 (true)
    printf("a > b: %d\n", a > b);    // 1 (true)

    // 逻辑表达式
    printf("a && b: %d\n", a && b);  // 1 (true)
    printf("a || b: %d\n", a || b);  // 1 (true)
    printf("!a: %d\n", !a);          // 0 (false)

    // sizeof 表达式
    printf("sizeof(int): %zu\n", sizeof(int));
    printf("sizeof(x): %zu\n", sizeof(x));

    return 0;
}
```

### 高级用法

#### 泛型选择（C11 起）

```c
#include <stdio.h>
#include <math.h>

// 泛型打印函数
#define print_type(x) _Generic((x), \
    int: "int", \
    double: "double", \
    float: "float", \
    char: "char", \
    char*: "string", \
    default: "unknown" \
)

// 泛型绝对值函数
#define abs_val(x) _Generic((x), \
    int: abs(x), \
    double: fabs(x), \
    float: fabsf(x) \
)

int main(void) {
    int i = -10;
    double d = -3.14;
    float f = -2.5f;

    printf("Type of i: %s\n", print_type(i));  // int
    printf("Type of d: %s\n", print_type(d));  // double

    printf("abs(i) = %d\n", abs_val(i));   // 10
    printf("abs(d) = %f\n", abs_val(d));   // 3.140000

    return 0;
}
```

#### 复合字面量（C99 起）

```c
#include <stdio.h>

struct Point {
    int x;
    int y;
};

void print_point(struct Point p) {
    printf("(%d, %d)\n", p.x, p.y);
}

int sum_array(int* arr, size_t size) {
    int sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

int main(void) {
    // 使用复合字面量创建临时结构体
    print_point((struct Point){10, 20});  // (10, 20)

    // 使用复合字面量创建临时数组
    int total = sum_array((int[]){1, 2, 3, 4, 5}, 5);
    printf("Sum: %d\n", total);  // Sum: 15

    // 复合字面量的作用域行为
    {
        int* p = (int[]){1, 2, 3};  // 作用域限于当前块
        printf("%d\n", p[0]);  // 1
    }
    // p[0] 在此处不再有效

    return 0;
}
```

#### 条件运算符

```c
#include <stdio.h>

int max(int a, int b) {
    return (a > b) ? a : b;  // 条件运算符
}

int main(void) {
    int x = 10, y = 20;

    // 简单条件表达式
    int max_val = (x > y) ? x : y;
    printf("Max: %d\n", max_val);  // Max: 20

    // 嵌套条件运算符
    int a = 5, b = 10, c = 15;
    int max_three = (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
    printf("Max of three: %d\n", max_three);  // Max of three: 15

    // 条件运算符在赋值中
    const char* result = (x % 2 == 0) ? "even" : "odd";
    printf("%d is %s\n", x, result);  // 10 is even

    return 0;
}
```

#### 位运算表达式

```c
#include <stdio.h>

// 标志位定义
#define FLAG_READ    (1 << 0)  // 0001
#define FLAG_WRITE   (1 << 1)  // 0010
#define FLAG_EXECUTE (1 << 2)  // 0100

int main(void) {
    unsigned int flags = 0;

    // 设置标志
    flags |= FLAG_READ;           // 设置读权限
    flags |= FLAG_WRITE;          // 设置写权限
    printf("Flags: 0x%02X\n", flags);  // Flags: 0x03

    // 检查标志
    if (flags & FLAG_READ) {
        printf("Read permission granted\n");
    }

    // 清除标志
    flags &= ~FLAG_WRITE;         // 清除写权限
    printf("Flags after clear: 0x%02X\n", flags);  // Flags: 0x01

    // 切换标志
    flags ^= FLAG_EXECUTE;        // 切换执行权限
    printf("Flags after toggle: 0x%02X\n", flags);  // Flags: 0x05

    // 位运算技巧
    int value = 0xABCD;
    int low_byte = value & 0xFF;          // 提取低字节
    int high_byte = (value >> 8) & 0xFF;  // 提取高字节
    printf("Low byte: 0x%02X, High byte: 0x%02X\n", low_byte, high_byte);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：未定义的求值顺序

```c
#include <stdio.h>

int main(void) {
    int i = 1;

    // ❌ 错误：同一表达式中多次修改同一变量
    int result = i++ + i++;  // 未定义行为

    // ✅ 正确：分开计算
    int a = i++;
    int b = i++;
    int result2 = a + b;

    // ❌ 错误：序列点内修改和读取
    int arr[10];
    arr[i++] = i;  // 未定义行为

    // ✅ 正确：分开操作
    arr[i] = i + 1;
    i++;

    return 0;
}
```

#### 错误 2：运算符优先级误解

```c
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    int a = 1, b = 2, c = 3;

    // ❌ 错误：误解位运算和比较的优先级
    bool result1 = a & b == c;  // 等价于 a & (b == c)，不是 (a & b) == c

    // ✅ 正确：使用括号明确意图
    bool result2 = (a & b) == c;

    // ❌ 错误：误解移位和算术的优先级
    int x = 1 << 2 + 3;  // 等价于 1 << (2 + 3) = 32，不是 (1 << 2) + 3 = 7

    // ✅ 正确：使用括号
    int y = (1 << 2) + 3;  // 7

    printf("x = %d, y = %d\n", x, y);

    return 0;
}
```

#### 错误 3：sizeof 与指针

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // ❌ 错误：sizeof 指针得到指针大小，非数组大小
    int arr[10];
    int* ptr = arr;

    size_t arr_size = sizeof(arr);      // 40 (10 * 4)
    size_t ptr_size = sizeof(ptr);      // 8 (指针大小，64位系统)
    size_t ptr_pointee_size = sizeof(*ptr);  // 4 (int 大小)

    printf("arr_size = %zu, ptr_size = %zu, *ptr_size = %zu\n",
           arr_size, ptr_size, ptr_pointee_size);

    // ✅ 正确：传递数组大小时使用 sizeof(arr) / sizeof(arr[0])
    size_t count = sizeof(arr) / sizeof(arr[0]);
    printf("Array element count: %zu\n", count);  // 10

    // ✅ 建议：使用宏计算数组大小
    #define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
    printf("Array size using macro: %zu\n", ARRAY_SIZE(arr));

    return 0;
}
```

#### 错误 4：逻辑运算的副作用

```c
#include <stdio.h>

int counter = 0;

int increment(void) {
    return ++counter;
}

int main(void) {
    int a = 0;

    // ⚠️ 注意：短路求值可能导致某些代码不执行
    if (a > 0 && increment()) {
        printf("Both true\n");
    }
    printf("Counter after &&: %d\n", counter);  // 0 (increment 未调用)

    counter = 0;
    if (a <= 0 || increment()) {
        printf("At least one true\n");
    }
    printf("Counter after ||: %d\n", counter);  // 0 (increment 未调用)

    // ✅ 如果需要确保函数被调用，不要依赖短路特性
    counter = 0;
    int result = increment();
    if (a > 0 && result) {
        printf("Both true\n");
    }
    printf("Counter after explicit call: %d\n", counter);  // 1

    return 0;
}
```

## 7. 总结

### 核心要点

表达式是 C 程序的基础构建块，具有以下核心特性：

| 特性 | 说明 |
|------|------|
| **组成** | 由运算符和操作数构成 |
| **求值** | 可能产生结果、副作用、或指定对象/函数 |
| **优先级** | 决定运算符绑定顺序 |
| **值类别** | 左值、非左值对象、函数指示符 |
| **类型转换** | 支持隐式转换和显式转换（cast） |

### 表达式类型对比

| 类型 | 特点 | 示例 |
|------|------|------|
| 主表达式 | 最基本的形式 | 标识符、常量、字面量 |
| 算术表达式 | 数学计算 | `a + b * c` |
| 逻辑表达式 | 条件判断 | `a && b \|\| c` |
| 位运算表达式 | 底层操作 | `flags \| MASK` |
| 条件表达式 | 三目运算 | `a ? b : c` |
| 赋值表达式 | 赋值操作 | `a = b` |
| 常量表达式 | 编译时求值 | `sizeof(int)` |

### 学习建议

1. **掌握优先级表**：理解运算符优先级避免常见错误
2. **注意求值顺序**：避免未定义行为
3. **善用括号**：明确表达式意图
4. **理解值类别**：区分左值和非左值
5. **利用新特性**：C99 复合字面量、C11 泛型选择
6. **阅读标准**：深入了解规范定义

### 关键注意事项

1. **求值顺序未指定**：大多数二元运算符的操作数求值顺序未指定
2. **序列点规则**：在序列点之间，同一对象的修改次数不超过一次
3. **sizeof 特殊性**：sizeof 的操作数通常不求值（VLA 除外）
4. **逻辑运算短路**：`&&` 和 `\|\|` 具有短路求值特性
5. **类型提升**：算术运算中的隐式类型转换

## 参考资料

- C23 标准 (ISO/IEC 9899:2024):
  - 6.5 Expressions
  - 6.6 Constant expressions
- C17 标准 (ISO/IEC 9899:2018):
  - 6.5 Expressions (p: 55-75)
  - 6.6 Constant expressions (p: 76-77)
- C11 标准 (ISO/IEC 9899:2011):
  - 6.5 Expressions (p: 76-105)
  - 6.6 Constant expressions (p: 106-107)
- C99 标准 (ISO/IEC 9899:1999):
  - 6.5 Expressions (p: 67-94)
  - 6.6 Constant expressions (p: 95-96)
- C89/C90 标准 (ISO/IEC 9899:1990):
  - 3.3 EXPRESSIONS
  - 3.4 CONSTANT EXPRESSIONS

---

*本页面最后修改于 2024 年 12 月 17 日*

*来源：cppreference.com*