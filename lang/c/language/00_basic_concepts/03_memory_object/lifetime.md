# 生命周期（Lifetime）

## 1. 概述（Overview）

**生命周期（Lifetime）** 是程序执行期间的一个时间段，在此期间对象存在、具有常量地址、保留其最后存储的值（除非值是不确定的），对于变长数组（VLA）还保留其大小（C99 起）。

### 核心概念

| 概念 | 说明 |
|------|------|
| 存在性 | 对象在内存中占据空间 |
| 地址恒定 | 对象的地址在其生命周期内不变 |
| 值保持 | 对象保留最后存储的值（除非值不确定） |
| 大小保持 | VLA 在生命周期内保持其大小（C99 起） |

### 与存储期的关系

对于具有自动、静态和线程存储期的对象，其生命周期等于其存储期。但对于分配存储期的对象，生命周期与存储期不同。

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C89/C90 | 定义基础生命周期概念，与存储期关联 |
| C99 | 引入变长数组（VLA）的大小保持特性；定义临时生命周期 |
| C11 | 修改临时生命周期的结束时机；引入线程存储期 |
| C17 | 沿用既有规则 |

### C99 的重要变更

C99 引入了**临时生命周期（temporary lifetime）** 概念，用于处理结构体和联合体的非左值表达式中的数组成员。同时，变长数组的大小在其生命周期内保持不变。

### C11 的变更

C11 修改了临时生命周期的结束时机：
- **C99**：在下一个序列点结束
- **C11**：在包含的完整表达式或完整声明符结束时结束

## 3. 语法与参数（Syntax and Parameters）

### 3.1 自动存储期对象的生命周期

**非 VLA 自动对象**：
- 生命周期在进入声明它的块时开始
- 生命周期在块执行结束时结束（正常结束或跳转结束）

**VLA 自动对象**（C99 起）：
- 生命周期在执行声明时开始
- 生命周期在块执行结束时结束
- 在声明之前，VLA 不存在

```c
void func(int n) {
    // 非声明点，VLA 不存在
    int vla[n];  // 生命周期从此开始
    // 使用 vla
}  // 生命周期在此结束
```

### 3.2 静态存储期对象的生命周期

- 生命周期在程序启动时开始
- 生命周期在程序结束时终止
- 整个程序执行期间都存在

```c
int global_var = 100;  // 静态存储期，整个程序执行期间存在

static int file_local = 200;  // 内部链接，静态存储期
```

### 3.3 线程存储期对象的生命周期（C11 起）

- 生命周期在线程启动时开始
- 生命周期在线程结束时终止
- 每个线程有独立的实例

```c
_Thread_local int thread_var = 0;  // 每个线程独立实例
```

### 3.4 分配存储期对象的生命周期

对于通过分配函数（`malloc`、`calloc`、`realloc`）分配的对象：

- **开始时机**：分配函数返回时（包括 `realloc` 返回）
- **结束时机**：调用 `realloc` 或释放函数（`free`）时

```c
int* p = malloc(sizeof(int));  // 生命周期从此开始
*p = 42;
free(p);  // 生命周期在此结束
```

**注意**：分配的对象没有声明类型，首次用于访问该对象的左值表达式的类型成为其有效类型。

### 3.5 临时生命周期（C99 起）

具有数组成员的结构体和联合体对象，如果通过非左值表达式指定，则具有临时生命周期。

**开始时机**：引用该对象的表达式被求值时

**结束时机**：
- **C99**：下一个序列点
- **C11**：包含的完整表达式或完整声明符结束时

**重要约束**：修改具有临时生命周期的对象是未定义行为。

```c
struct T { double a[4]; };
struct T f(void) { return (struct T){3.15}; }

double g1(double* x) { return *x; }
void g2(double* x) { *x = 1.0; }

int main(void)
{
    double d = g1(f().a);
    // C99: UB - 在 g1 中访问 a[0]，其生命周期已在 g1 开始的序列点结束
    // C11: OK - d 为 3.15

    g2(f().a);
    // C99: UB - 修改已结束生命周期的 a[0]
    // C11: UB - 尝试修改临时对象
}
```

## 4. 底层原理（Underlying Principles）

### 4.1 生命周期与存储期的区别

| 存储期类型 | 生命周期 | 存储期 |
|------------|----------|--------|
| 自动（非 VLA） | 块开始 → 块结束 | 同左 |
| 自动（VLA） | 声明执行 → 块结束 | 同左，但存在性不同 |
| 静态 | 程序启动 → 程序结束 | 同左 |
| 线程（C11 起） | 线程启动 → 线程结束 | 同左 |
| 分配 | 分配函数返回 → 释放函数调用 | 同左 |

### 4.2 VLA 的特殊处理

变长数组的大小在生命周期内保持，但存储期与普通自动变量相同：

```c
void vla_example(int n) {
    int vla[n];  // 大小在此时确定并保持

    // 即使 n 在后续被修改，vla 的大小不变
    n = 100;  // 不影响 vla 的大小
}
```

### 4.3 悬空指针（Dangling Pointer）

指向生命周期已结束对象的指针具有不确定值：

```c
int* foo(void) {
    int a = 17;  // a 具有自动存储期
    return &a;
}  // a 的生命周期结束

int main(void) {
    int* p = foo();  // p 指向生命周期已结束的对象（悬空指针）
    int n = *p;      // 未定义行为
}
```

### 4.4 有效类型（Effective Type）

对于分配的对象，有效类型由首次访问的左值表达式决定：

```c
int* p = malloc(sizeof(int));
*p = 42;  // 有效类型变为 int

float* fp = (float*)p;
*fp = 3.14f;  // 未定义行为：违反有效类型规则（严格别名）
```

## 5. 使用场景（Use Cases）

### 5.1 静态生命周期

```c
// 全局配置
static struct Config {
    int initialized;
    char name[64];
} app_config;  // 静态存储期，整个程序运行期间存在

void init_config(void) {
    if (!app_config.initialized) {
        strcpy(app_config.name, "MyApp");
        app_config.initialized = 1;
    }
}
```

### 5.2 自动生命周期

```c
void process_data(int size) {
    int buffer[1024];  // 自动存储期
    int* dynamic = malloc(size * sizeof(int));  // 分配存储期

    // 使用 buffer 和 dynamic
    free(dynamic);
}  // buffer 生命周期结束，dynamic 已释放
```

### 5.3 分配生命周期

```c
int* create_array(int size) {
    int* arr = malloc(size * sizeof(int));
    if (arr) {
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
    }
    return arr;  // 返回有效指针
}

void use_array(void) {
    int* my_array = create_array(10);
    // 使用 my_array
    free(my_array);  // 结束生命周期
}
```

### 5.4 常见陷阱

1. **返回局部变量地址**：

```c
int* bad_function(void) {
    int local = 10;
    return &local;  // 错误：返回局部变量地址
}

int* good_function(void) {
    static int local = 10;  // 静态存储期
    return &local;  // 正确
}
```

2. **使用已释放内存**：

```c
void use_after_free(void) {
    int* p = malloc(sizeof(int));
    *p = 42;
    free(p);
    *p = 100;  // 未定义行为：使用已释放内存
}
```

3. **临时对象修改**：

```c
struct Point { int coords[2]; };
struct Point get_point(void) { return (struct Point){{1, 2}}; }

void modify_temp(void) {
    get_point().coords[0] = 10;  // 未定义行为：修改临时对象
}
```

4. **VLA 生命周期与声明点**：

```c
void vla_trap(int n) {
    // sizeof(vla) 在此处是错误：vla 不存在
    int vla[n];
    // vla 存在，可使用
}
```

## 6. 代码示例（Examples）

### 基础用法

```c
#include <stdio.h>
#include <stdlib.h>

int global = 100;  // 静态存储期

void demonstrate_lifetimes(void)
{
    int automatic = 200;       // 自动存储期
    static int local_static = 300;  // 静态存储期

    int* allocated = malloc(sizeof(int));  // 分配存储期
    *allocated = 400;

    printf("global: %d\n", global);
    printf("automatic: %d\n", automatic);
    printf("local_static: %d\n", local_static);
    printf("allocated: %d\n", *allocated);

    free(allocated);  // 结束分配对象的生命周期
}  // automatic 的生命周期结束

int main(void)
{
    demonstrate_lifetimes();
    demonstrate_lifetimes();  // local_static 保持其值
    return 0;
}
```

### 变长数组（VLA）生命周期

```c
#include <stdio.h>

void vla_example(int n)
{
    printf("Creating VLA of size %d\n", n);
    int vla[n];  // VLA 生命周期从此开始

    for (int i = 0; i < n; i++) {
        vla[i] = i * 2;
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", vla[i]);
    }
    printf("\n");
}  // VLA 生命周期在此结束

int main(void)
{
    vla_example(5);
    vla_example(3);
    return 0;
}
```

### 分配对象生命周期

```c
#include <stdio.h>
#include <stdlib.h>

int* create_and_use(int size)
{
    int* arr = malloc(size * sizeof(int));
    if (!arr) return NULL;

    for (int i = 0; i < size; i++) {
        arr[i] = i * i;
    }

    return arr;  // 生命周期继续，由调用者管理
}

int main(void)
{
    int* data = create_and_use(10);
    if (data) {
        for (int i = 0; i < 10; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
        free(data);  // 结束生命周期
    }
    return 0;
}
```

### 临时生命周期示例

```c
#include <stdio.h>

struct Array {
    double data[4];
};

struct Array make_array(void)
{
    return (struct Array){{1.0, 2.0, 3.0, 4.0}};
}

double read_first(struct Array a)
{
    return a.data[0];  // OK：读取临时对象的副本
}

int main(void)
{
    // C11: OK - 完整表达式结束时临时对象生命周期结束
    double value = read_first(make_array());
    printf("value = %f\n", value);

    // 以下在 C99 和 C11 中都是 UB：修改临时对象
    // make_array().data[0] = 100.0;

    return 0;
}
```

### 常见错误及修正

```c
#include <stdio.h>
#include <stdlib.h>

/* 错误：返回局部变量地址 */
int* bad_return_local(void)
{
    int local = 42;
    return &local;  // 悬空指针
}

/* 正确：使用静态变量 */
int* good_return_static(void)
{
    static int local = 42;
    return &local;
}

/* 正确：使用分配内存 */
int* good_return_allocated(void)
{
    int* p = malloc(sizeof(int));
    if (p) *p = 42;
    return p;
}

/* 错误：双重释放 */
void double_free_error(void)
{
    int* p = malloc(sizeof(int));
    free(p);
    // free(p);  // 错误：双重释放
}

/* 正确：释放后置 NULL */
void safe_free_pattern(void)
{
    int* p = malloc(sizeof(int));
    if (p) {
        *p = 42;
        free(p);
        p = NULL;  // 防止意外使用
    }
}

/* 错误：使用已释放内存 */
void use_after_free_error(void)
{
    int* p = malloc(sizeof(int));
    *p = 42;
    free(p);
    // printf("%d\n", *p);  // 未定义行为
}

/* 正确：先使用后释放 */
void correct_order(void)
{
    int* p = malloc(sizeof(int));
    if (p) {
        *p = 42;
        printf("%d\n", *p);  // 先使用
        free(p);  // 后释放
    }
}

int main(void)
{
    int* p1 = bad_return_local();
    // printf("%d\n", *p1);  // 未定义行为

    int* p2 = good_return_static();
    printf("static: %d\n", *p2);  // 正确

    int* p3 = good_return_allocated();
    if (p3) {
        printf("allocated: %d\n", *p3);
        free(p3);
    }

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **定义**：生命周期是对象存在、地址恒定、值保持的时间段
2. **与存储期关系**：自动/静态/线程存储期对象的生命周期等于存储期
3. **分配对象**：生命周期从分配函数返回到释放函数调用
4. **临时对象**：具有数组成员的结构体/联合体非左值表达式有临时生命周期

### 生命周期类型对比

| 类型 | 开始 | 结束 | 适用对象 |
|------|------|------|----------|
| 自动（非 VLA） | 块入口 | 块出口 | 局部变量 |
| 自动（VLA） | 声明执行 | 块出口 | 变长数组（C99 起） |
| 静态 | 程序启动 | 程序结束 | 全局变量、static 局部变量 |
| 线程 | 线程启动 | 线程结束 | _Thread_local 变量（C11 起） |
| 分配 | malloc/calloc/realloc 返回 | free/realloc 调用 | 动态分配内存 |
| 临时 | 表达式求值 | 完整表达式结束 | 非左值结构体/联合体（C99 起） |

### 未定义行为

| 行为 | 说明 |
|------|------|
| 访问生命周期外的对象 | 悬空指针解引用 |
| 修改临时对象 | 始终是 UB |
| 使用不确定值的指针 | 指向已结束生命周期对象的指针 |

### 最佳实践

- 不要返回局部变量的地址
- 及时释放分配的内存，避免内存泄漏
- 释放后将指针置为 NULL，防止悬空指针
- 注意临时对象的生命周期限制
- 理解 VLA 的声明点与生命周期的关系

### 参考资料

- C17 标准 (ISO/IEC 9899:2018): 6.2.4 Storage durations of objects (p: 30)
- C11 标准 (ISO/IEC 9899:2011): 6.2.4 Storage durations of objects (p: 38-39)
- C99 标准 (ISO/IEC 9899:1999): 6.2.4 Storage durations of objects (p: 32)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.1.2.4 Storage durations of objects