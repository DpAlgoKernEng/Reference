# C 语言其他运算符

## 1. 概述 (Overview)

C 语言中的其他运算符（Other operators）是指不属于主要运算符类别（如算术、逻辑、比较等）的一组运算符集合。这些运算符虽然在分类上相对独立，但在 C 语言编程中扮演着至关重要的角色。

主要包括以下运算符：

| 运算符 | 名称 | 示例 | 描述 |
|--------|------|------|------|
| `(...)` | 函数调用运算符 (function call) | `f(...)` | 调用函数 `f()`，传递零个或多个参数 |
| `,` | 逗号运算符 (comma operator) | `a, b` | 求值表达式 `a`，丢弃其返回值，然后求值表达式 `b`，返回 `b` 的结果 |
| `(type)` | 类型转换运算符 (type cast) | `(type)a` | 将 `a` 的类型转换为 `type` |
| `? :` | 条件运算符 (conditional operator) | `a ? b : c` | 如果 `a` 为真（非零），求值 `b`，否则求值 `c` |
| `sizeof` | sizeof 运算符 | `sizeof a` | 获取 `a` 的字节大小 |
| `_Alignof` | _Alignof 运算符（C11 起） | `_Alignof(type)` | 获取 `type` 所需的对齐要求 |
| `typeof` | typeof 运算符 | `typeof(a)` | 获取 `a` 的类型 |

这些运算符涵盖了函数调用、类型操作、流程控制和内存信息查询等核心功能，是 C 语言表达能力和灵活性的重要组成部分。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

这些运算符大多源自 C 语言的早期设计，体现了 C 语言作为"可移植汇编语言"的设计哲学：

**C89/C90 标准时期**
- 确立了函数调用、逗号运算符、类型转换、条件运算符和 sizeof 运算符的基本语义
- 函数调用支持隐式声明机制（未声明的标识符默认视为返回 `int` 的无原型函数）

**C99 标准**
- 移除了隐式函数声明（不再允许未声明的标识符自动作为函数）
- 引入了变长数组（VLA），影响 sizeof 运算符的求值时机
- 增加了函数参数中 `static` 关键字的使用，用于数组参数的边界检查提示

**C11 标准**
- 引入 `_Alignof` 运算符，用于查询类型的对齐要求
- 增加了 `_Alignas` 关键字用于指定对齐
- 强化了类型安全性

**C23 标准**
- 引入 `typeof` 运算符，支持在编译时获取表达式的类型
- 引入 `nullptr_t` 类型，影响条件运算符的类型推导规则
- 移除了无原型函数的调用支持（标记为过时特性）

### 设计动机

| 运算符 | 设计动机 |
|--------|----------|
| 函数调用 | 提供统一的过程调用机制，支持参数传递和返回值 |
| 逗号运算符 | 允许在单个表达式中执行多个操作，常用于 `for` 循环 |
| 条件运算符 | 提供简洁的三元选择语法，避免冗长的 `if-else` 结构 |
| sizeof | 编译时确定对象大小，支持可移植的内存操作 |
| _Alignof | 支持硬件对齐要求，便于底层内存管理 |
| typeof | 增强泛型编程能力，简化类型推导 |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 函数调用运算符

**语法形式**

```
expression ( argument-list(可选) )
```

**参数说明**

| 参数 | 说明 |
|------|------|
| `expression` | 函数指针类型的表达式（左值转换后） |
| `argument-list` | 逗号分隔的表达式列表（不能是逗号运算符），可以是任意完整对象类型。调用无参函数时可省略 |

**调用行为**

函数调用的行为取决于被调用函数的原型（prototype）是否在调用点可见：

**有原型函数调用**
1. 参数数量必须与参数数量匹配（除非使用省略号参数）
2. 每个参数的类型必须存在从实参到形参的隐式转换
3. C99 起：对于使用 `static` 的数组参数，实参必须指向至少包含指定数量元素的数组
4. 参数以未指定的顺序求值，无序且无序列关系
5. 执行参数值的复制（按值传递）
6. 如有尾部省略号参数，对剩余参数执行默认参数提升

**无原型函数调用（C23 前）**
1. 参数以未指定的顺序求值
2. 对每个参数执行默认参数提升
3. 执行参数复制
4. 行为未定义的情况：
   - 参数数量与形参数量不匹配
   - 提升后的参数类型与提升后的形参类型不兼容
   - 例外：有符号/无符号版本的相同整数类型视为兼容
   - 例外：`void*` 与字符类型指针视为兼容

### 3.2 逗号运算符

**语法形式**

```
lhs , rhs
```

**参数说明**

| 参数 | 说明 |
|------|------|
| `lhs` | 任意表达式 |
| `rhs` | 任意表达式（但不能是另一个逗号运算符，即左结合性） |

**求值顺序**
1. 求值左操作数 `lhs`，结果被丢弃
2. 序列点发生，确保 `lhs` 的所有副作用完成
3. 求值右操作数 `rhs`，返回其结果（非左值）

**使用限制**

逗号运算符在以下上下文中不能出现在表达式的顶层（因为逗号有其他含义）：
- 函数调用的参数列表
- 初始化表达式或初始化列表
- 泛型选择
- 数组边界（顶层禁止，但可以用括号包裹）
- 常量表达式（任何层级都禁止）

### 3.3 条件运算符

**语法形式**

```
condition ? expression-true : expression-false
```

**参数说明**

| 参数 | 说明 |
|------|------|
| `condition` | 标量类型的表达式 |
| `expression-true` | 当 `condition` 不等于零时求值的表达式 |
| `expression-false` | 当 `condition` 等于零时求值的表达式 |

**允许的表达式组合**

`expression-true` 和 `expression-false` 必须满足以下条件之一：
- 两个表达式都是算术类型
- 两个表达式是相同的结构体或联合体类型
- 两个表达式都是 `void` 类型
- 两个表达式都是指针类型，指向兼容的类型（忽略 cvr 限定符）
- C23 起：两个表达式都是 `nullptr_t` 类型
- 一个是指针，另一个是空指针常量（如 `NULL`）或 `nullptr_t` 值（C23 起）
- 一个是指向对象的指针，另一个是 `void*`（可能有 cvr 限定）

**公共类型（Common Type）推导**

1. 算术类型：应用常规算术转换
2. 结构体/联合体：公共类型即为该结构体/联合体类型
3. 都是 `void`：结果为 `void` 表达式
4. 一个是指针，一个是空指针常量：公共类型是指针类型
5. 两个指针：公共类型是合并了 cvr 限定符的指针类型
6. 一个是 `void*`：公共类型是带有合并 cvr 限定符的 `void*`
7. C23 起：两个 `nullptr_t`：公共类型为 `nullptr_t`

### 3.4 sizeof 运算符

详见 sizeof 运算符文档。

### 3.5 _Alignof 运算符

详见 _Alignof 运算符文档。

### 3.6 typeof 运算符

详见 typeof 运算符文档。

## 4. 底层原理 (Underlying Principles)

### 4.1 函数调用实现机制

**调用约定（Calling Convention）**

函数调用的底层实现涉及以下步骤：

1. **参数传递**
   - 参数通过寄存器或栈传递，取决于调用约定
   - 常见的调用约定包括：cdecl、stdcall、fastcall、System V AMD64 ABI 等
   - 默认参数提升：`float` 提升为 `double`，小整数类型提升为 `int` 或 `unsigned int`

2. **栈帧创建**
   - 调用者保存返回地址
   - 被调用者创建栈帧，保存寄存器和局部变量

3. **控制流转移**
   - CPU 跳转到函数入口地址
   - 执行函数体
   - 返回值通过寄存器（如 RAX）或栈返回

4. **栈帧清理**
   - 被调用者恢复寄存器
   - 返回调用者
   - 调用者清理参数（取决于调用约定）

**序列点保证**

函数调用在函数体开始执行前有一个序列点，这保证了：
- 所有参数求值和副作用在函数体执行前完成
- 参数求值之间的顺序是未指定的

```
(*pf[f1()]) (f2(), f3() + f4());
// f1, f2, f3, f4 可能以任意顺序调用
// 但函数体开始前，所有参数都已求值完成
```

**函数指针与函数指示符**

函数调用实际上定义为对函数指针的调用。函数指示符会自动转换为函数指针：

```c
int f(void) { return 1; }
int (*pf)(void) = f;

int main(void) {
    f();      // f 转换为指针，然后调用
    (&f)();   // 创建函数指针，然后调用

    pf();     // 直接调用
    (****pf)(); // 获取函数指示符，转换为指针...重复 4 次，然后调用
}
```

### 4.2 逗号运算符求值机制

**序列点语义**

逗号运算符是少数保证求值顺序的运算符之一：

1. 先求值 `lhs`
2. **序列点发生**：所有副作用完成
3. 再求值 `rhs`

这使得逗号运算符可以安全地组合有副作用的操作：

```c
int i = 0;
int arr[10];
int x = (arr[i] = i, i++, arr[i]); // 安全：保证先赋值，再自增，最后访问
```

**为什么需要序列点**

如果没有序列点，以下代码的行为将是未定义的：

```c
// 如果没有序列点
int x = (a++, a++); // 两次 a++ 之间没有序列点 -> UB
```

但有了序列点：

```c
int x = (a++, a++); // 第一个 a++ 在序列点完成，然后第二个 a++ -> 定义明确
```

### 4.3 条件运算符实现原理

**分支预测优化**

条件运算符在底层实现上可能有优势：

```c
int max = (a > b) ? a : b;
```

编译器可能生成条件传送指令（conditional move）：

```asm
cmovg rax, rbx    ; 如果 a > b，则 rax = rbx
```

这避免了分支预测失败的开销，比等价的 `if-else` 更高效。

**类型推导算法**

条件运算符的类型推导遵循复杂的规则：

1. 算术类型：应用常规算术转换（usual arithmetic conversions）
2. 指针类型：合并 cvr 限定符，计算复合类型

示例：

```c
const int* p1;
volatile int* p2;
auto result = 1 ? p1 : p2; // result 类型: const volatile int*
```

**编译时检测技巧**

利用条件运算符的特性可以检测整数常量表达式：

```c
#define ICE(x) (sizeof(*(1 ? ((void*)((x) * 0l)) : (int*)1)))

// 如果 x 是整数常量表达式
ICE(42) -> sizeof(*(1 ? NULL : (int*)1)) -> sizeof(int) -> 4

// 如果 x 不是整数常量表达式
ICE(var) -> sizeof(*(void*)(var)) -> 错误：void 类型不完整
```

### 4.4 sizeof 运算符原理

**编译时求值（大多数情况）**

`sizeof` 在编译时确定大小，不实际求值表达式：

```c
int x = 10;
sizeof(x++); // x 不会自增！sizeof 只需要类型信息
```

**VLA 例外**

变长数组（VLA）的大小在运行时确定：

```c
int n = 10;
int arr[n]; // VLA
sizeof(arr); // 运行时求值
```

### 4.5 _Alignof 运算符原理

**对齐要求**

对齐（alignment）是数据在内存中存储时的地址约束：
- `char` 的对齐要求通常是 1
- `int` 的对齐要求通常是 4（4 字节对齐）
- `double` 的对齐要求通常是 8（8 字节对齐）

对齐值必须是 2 的幂次。

**硬件架构影响**

对齐要求源于硬件设计：
- 某些架构要求对齐访问，否则触发异常
- x86/x64 支持非对齐访问，但性能较低
- ARM 某些指令要求对齐

## 5. 使用场景 (Use Cases)

### 5.1 函数调用

**正确用法**

```c
// 1. 标准函数调用
int result = printf("Hello, %s!\n", name);

// 2. 函数指针调用
int (*compare)(const void*, const void*) = strcmp;
int cmp = compare("a", "b");

// 3. 回调函数模式
void sort(int* arr, size_t n, int (*cmp)(int, int)) {
    for (size_t i = 0; i < n - 1; i++) {
        if (cmp(arr[i], arr[i + 1]) > 0) {
            // 交换
        }
    }
}
```

**常见陷阱**

```c
// 陷阱 1: 参数求值顺序未定义
int i = 0;
printf("%d, %d, %d\n", i++, i++, i++); // 未定义行为

// 陷阱 2: 忘记函数原型
void f(); // 无原型声明
f(1, 2.0f); // C23 前可能有 UB，C23 后不支持

// 正确做法
void f(int, double); // 有原型声明
```

**最佳实践**
- 始终在使用前声明函数原型
- 避免在参数中使用有副作用的表达式多次操作同一变量
- 使用 `static` 数组参数提示优化机会

### 5.2 逗号运算符

**正确用法**

```c
// 1. for 循环中的多表达式
for (int i = 0, j = n - 1; i < j; i++, j--) {
    // 双指针遍历
}

// 2. 宏中执行多个操作
#define SWAP(x, y) ((x) = (y), (y) = (temp), (temp) = (x))

// 3. 在必须使用表达式的上下文中执行副作用
int result;
if ((process_data(), result = get_result(), result > 0)) {
    // 处理结果
}
```

**常见陷阱**

```c
// 陷阱 1: 初始化列表中的逗号
int a[2] = {1, 2, 3}; // 错误：初始化器过多

// 陷阱 2: 数组声明中的逗号
int arr[2, 3]; // 错误：逗号在顶层

// 正确用法
int a[2] = {(1, 2), 3}; // arr[0] = 2, arr[1] = 3
int arr[(2, 3)]; // VLA，大小为 3

// 陷阱 3: 常量表达式
static int n = (1, 2); // 错误：逗号运算符不能用于常量表达式
```

**最佳实践**
- 优先使用 `for` 循环的多表达式特性，而非独立的逗号表达式
- 宏中使用时确保表达式有返回值
- 逗号运算符应包裹在括号中以明确意图

### 5.3 条件运算符

**正确用法**

```c
// 1. 简单条件选择
int max = (a > b) ? a : b;
int abs_val = (x < 0) ? -x : x;

// 2. 链式条件
const char* grade =
    score >= 90 ? "A" :
    score >= 80 ? "B" :
    score >= 70 ? "C" :
    score >= 60 ? "D" : "F";

// 3. 枚举映射
enum vehicle { bus, airplane, train, car, horse, feet };

enum vehicle choose(char arg) {
    return arg == 'B' ? bus      :
           arg == 'A' ? airplane :
           arg == 'T' ? train    :
           arg == 'C' ? car      :
           arg == 'H' ? horse    :
                        feet     ;
}
```

**常见陷阱**

```c
// 陷阱 1: 运算符优先级
int x = a ? b : c + d;   // 解析为 a ? b : (c + d)
int y = a ? b : c, d;    // 解析为 (a ? b : c), d

// 陷阱 2: 副作用在未选择的分支
int x = 1, y = 2, z;
z = (x > 0) ? y++ : x++; // 只有 y 会自增

// 陷阱 3: 类型不兼容
int* p = NULL;
float* q = NULL;
auto r = 1 ? p : q; // 错误：类型不兼容
```

**最佳实践**
- 复杂条件使用 `if-else` 更清晰
- 避免在条件运算符中使用复杂的副作用
- 链式条件右对齐以提高可读性
- 始终使用括号明确优先级

### 5.4 sizeof 运算符

**正确用法**

```c
// 1. 计算数组元素数量
int arr[10];
size_t count = sizeof(arr) / sizeof(arr[0]); // count = 10

// 2. 动态内存分配
int* p = malloc(sizeof(int) * 100);

// 3. 可移植的大小计算
struct Data data;
size_t size = sizeof(data); // 不依赖具体平台
```

**最佳实践**
- 使用 `sizeof(var)` 而非 `sizeof(type)`，减少维护成本
- 使用 `size_t` 类型存储 `sizeof` 结果
- 注意 VLA 会在运行时求值

### 5.5 _Alignof 运算符

**正确用法**

```c
// 1. 检查对齐要求
printf("int 对齐要求: %zu\n", _Alignof(int));
printf("double 对齐要求: %zu\n", _Alignof(double));

// 2. 验证结构体对齐
struct S { char c; int i; };
printf("struct S 对齐要求: %zu\n", _Alignof(struct S));

// 3. 自定义对齐分配
void* aligned_alloc(size_t alignment, size_t size) {
    void* ptr;
    posix_memalign(&ptr, alignment, size);
    return ptr;
}
```

**最佳实践**
- 用于调试对齐相关问题
- 配合 `_Alignas` 使用确保对齐
- 动态内存分配时考虑对齐要求

## 6. 代码示例 (Examples)

### 6.1 函数调用示例

**基础用法**

```c
#include <stdio.h>
#include <stdlib.h>

// 有原型函数声明
void print_message(const char* msg);
int add(int a, int b);

int main(void) {
    print_message("Hello, World!");

    int result = add(3, 5);
    printf("3 + 5 = %d\n", result);

    return 0;
}

void print_message(const char* msg) {
    printf("%s\n", msg);
}

int add(int a, int b) {
    return a + b;
}
```

**函数指针与回调**

```c
#include <stdio.h>
#include <stdlib.h>

// 比较函数类型
typedef int (*CompareFunc)(const void*, const void*);

// 简单排序
void bubble_sort(int* arr, size_t n, CompareFunc cmp) {
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (cmp(&arr[j], &arr[j + 1]) > 0) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int ascending(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

int descending(const void* a, const void* b) {
    return *(int*)b - *(int*)a;
}

int main(void) {
    int arr[] = {5, 2, 8, 1, 9, 3};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    bubble_sort(arr, n, ascending);
    printf("升序: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    bubble_sort(arr, n, descending);
    printf("降序: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
```

**常见错误**

```c
#include <stdio.h>

// 错误 1: 参数求值顺序依赖
void print_values(int a, int b, int c) {
    printf("%d, %d, %d\n", a, b, c);
}

int main(void) {
    int i = 0;

    // 错误：参数求值顺序未定义
    // print_values(i++, i++, i++); // 可能输出 0,1,2 或 2,1,0 或其他

    // 正确：明确求值顺序
    int a = i++;
    int b = i++;
    int c = i++;
    print_values(a, b, c); // 输出 0, 1, 2

    return 0;
}
```

### 6.2 逗号运算符示例

**基础用法**

```c
#include <stdio.h>

int main(void) {
    // 简单的逗号表达式
    int x = (1, 2, 3); // x = 3
    printf("x = %d\n", x);

    // 在 for 循环中
    for (int i = 0, j = 10; i < j; i++, j--) {
        printf("i = %d, j = %d\n", i, j);
    }

    return 0;
}
```

**高级用法：交换宏**

```c
#include <stdio.h>

#define SWAP(type, a, b) ((b) = (type)(a) + (b), (a) = (type)(b) - (a), (b) = (type)(b) - (a))

int main(void) {
    int x = 10, y = 20;
    printf("交换前: x = %d, y = %d\n", x, y);

    SWAP(int, x, y);
    printf("交换后: x = %d, y = %d\n", x, y);

    return 0;
}
```

**常见错误**

```c
#include <stdio.h>

int main(void) {
    // 错误 1: 初始化列表中的逗号（顶层逗号）
    // int arr[3] = 1, 2, 3; // 语法错误

    // 正确：使用花括号
    int arr[3] = {1, 2, 3};

    // 错误 2: 常量表达式中的逗号
    // static int n = (1, 2); // 编译错误

    // 正确：使用常量
    static int n = 2;

    // 错误 3: 数组声明中的顶层逗号
    // int matrix[2, 3]; // 错误：解析为 int matrix[3]

    // 正确：使用括号
    int matrix[(2, 3)]; // VLA，大小为 3

    printf("n = %d\n", n);
    return 0;
}
```

### 6.3 条件运算符示例

**基础用法**

```c
#include <stdio.h>

int main(void) {
    int a = 10, b = 20;

    // 求最大值
    int max = (a > b) ? a : b;
    printf("max = %d\n", max);

    // 求绝对值
    int x = -5;
    int abs_x = (x < 0) ? -x : x;
    printf("abs(%d) = %d\n", x, abs_x);

    // 字符串选择
    const char* result = (a > b) ? "a 更大" : "b 更大或相等";
    printf("%s\n", result);

    return 0;
}
```

**高级用法：状态机**

```c
#include <stdio.h>

enum vehicle { bus, airplane, train, car, horse, feet };

const char* vehicle_name(enum vehicle v) {
    return v == bus      ? "公交车" :
           v == airplane ? "飞机"   :
           v == train    ? "火车"   :
           v == car      ? "汽车"   :
           v == horse    ? "马"     :
                          "步行"   ;
}

enum vehicle choose_transport(char preference) {
    return preference == 'B' ? bus      :
           preference == 'A' ? airplane :
           preference == 'T' ? train    :
           preference == 'C' ? car      :
           preference == 'H' ? horse    :
                               feet     ;
}

int main(void) {
    char pref = 'A';
    enum vehicle v = choose_transport(pref);
    printf("选择交通方式: %s\n", vehicle_name(v));

    return 0;
}
```

**常见错误**

```c
#include <stdio.h>

int main(void) {
    // 错误 1: 运算符优先级混淆
    int a = 5, b = 3, c = 2;
    int result1 = a > b ? b : c + 1;  // 解析为 a > b ? b : (c + 1) = 3
    printf("result1 = %d\n", result1);

    // 更清晰的写法
    int result2 = (a > b) ? b : (c + 1);
    printf("result2 = %d\n", result2);

    // 错误 2: 类型不兼容
    // int* p = NULL;
    // float* q = NULL;
    // auto r = (a > 0) ? p : q; // 错误：类型不兼容

    // 正确：使用 void* 或显式转换
    int* p = NULL;
    float* q = NULL;
    void* r = (a > 0) ? (void*)p : (void*)q;

    return 0;
}
```

### 6.4 sizeof 示例

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // 基本类型大小
    printf("sizeof(char) = %zu\n", sizeof(char));
    printf("sizeof(int) = %zu\n", sizeof(int));
    printf("sizeof(double) = %zu\n", sizeof(double));

    // 数组大小
    int arr[10];
    printf("数组大小: %zu 字节\n", sizeof(arr));
    printf("数组元素数: %zu\n", sizeof(arr) / sizeof(arr[0]));

    // 结构体大小（考虑对齐）
    struct S {
        char c;
        int i;
        double d;
    };
    printf("sizeof(struct S) = %zu\n", sizeof(struct S));

    // 动态内存分配
    int* ptr = malloc(sizeof(int) * 100);
    printf("分配了 %zu 字节\n", sizeof(int) * 100);
    free(ptr);

    return 0;
}
```

### 6.5 _Alignof 示例

```c
#include <stdio.h>
#include <stdalign.h>

int main(void) {
    // 基本类型对齐
    printf("_Alignof(char) = %zu\n", _Alignof(char));
    printf("_Alignof(int) = %zu\n", _Alignof(int));
    printf("_Alignof(double) = %zu\n", _Alignof(double));

    // 结构体对齐
    struct S {
        char c;
        int i;
    };
    printf("_Alignof(struct S) = %zu\n", _Alignof(struct S));

    // 使用 alignas 指定对齐
    struct alignas(16) Aligned {
        int x;
    };
    printf("_Alignof(Aligned) = %zu\n", _Alignof(struct Aligned));

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 运算符 | 核心要点 | 注意事项 |
|--------|----------|----------|
| 函数调用 `()` | 参数按值传递，求值顺序未定义 | 始终声明原型，避免无原型调用 |
| 逗号 `,` | 保证左到右求值，有序列点 | 不能在常量表达式中使用 |
| 条件 `? :` | 右结合，可链式，自动类型推导 | 注意运算符优先级和类型兼容性 |
| `sizeof` | 编译时求值（VLA 除外），不实际求值表达式 | 结果为 `size_t` |
| `_Alignof` | C11 引入，查询类型对齐要求 | 只能用于类型 |
| `typeof` | C23 引入，获取表达式类型 | 增强泛型编程能力 |

### 版本差异

| 特性 | C89 | C99 | C11 | C23 |
|------|-----|-----|-----|-----|
| 隐式函数声明 | 支持 | 移除 | - | - |
| 无原型函数调用 | 支持 | 支持 | 支持 | 移除 |
| `_Alignof` | - | - | 引入 | 支持 |
| `alignof` 关键字 | - | - | - | 引入（等同于 `_Alignof`） |
| `typeof` | - | - | - | 引入 |
| `nullptr_t` | - | - | - | 引入 |

### 性能对比

| 运算符 | 编译时开销 | 运行时开销 | 优化建议 |
|--------|-----------|-----------|----------|
| 函数调用 | 低 | 中（参数传递、栈操作） | 内联小函数 |
| 逗号运算符 | 无 | 无 | 适当使用提高可读性 |
| 条件运算符 | 低 | 低（可能优化为条件传送） | 简单条件使用 `? :` |
| `sizeof` | 无 | 无（编译时求值） | 使用 `sizeof(var)` 替代 `sizeof(type)` |
| `_Alignof` | 无 | 无（编译时求值） | 调试时使用 |

### 学习建议

1. **函数调用**
   - 理解参数求值顺序的不确定性
   - 掌握函数指针和回调的使用
   - 了解不同调用约定的区别

2. **逗号运算符**
   - 主要用于 `for` 循环和宏定义
   - 理解序列点的意义
   - 避免在复杂表达式中滥用

3. **条件运算符**
   - 适用于简单的条件选择
   - 链式使用时注意可读性
   - 理解类型推导规则

4. **sizeof**
   - 掌握数组元素数计算技巧
   - 理解 VLA 的运行时求值特性
   - 注意结构体对齐对大小的影响

5. **_Alignof / typeof**
   - 理解对齐的概念和重要性
   - 掌握 C23 的 `typeof` 用法
   - 配合泛型编程使用

### 延伸阅读

- C 语言运算符优先级
- 函数调用约定详解
- 类型系统与类型转换
- 内存对齐与性能优化
- C23 新特性详解