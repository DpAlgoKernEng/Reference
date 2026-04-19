# alignof - 对齐查询运算符

## 1. 概述

`alignof` 运算符是 C 语言中用于查询类型对齐要求（alignment requirement）的一元运算符。它返回指定类型在内存中存储时所需的最小对齐字节数，结果为 `size_t` 类型的整数常量。

对齐（alignment）是指数据对象在内存中的地址必须满足的边界要求。例如，一个对齐要求为 4 的类型，其对象的地址必须是 4 的倍数。正确理解和使用对齐对于底层编程、硬件交互和性能优化至关重要。

该运算符在 **C11** 标准中以 `_Alignof` 关键字形式引入，**C23** 标准中增加了更简洁的 `alignof` 关键字，同时将 `_Alignof` 标记为弃用。

## 2. 来源与演变

### 首次引入

`_Alignof` 运算符在 **C11** 标准中首次引入，这是 C 语言对内存对齐支持的重要组成部分。C11 标准同时引入了 `_Alignas` 对齐说明符、`max_align_t` 类型以及 `<stdalign.h>` 头文件。

### 历史背景

在 C11 之前，C 语言缺乏标准化的对齐查询机制：

1. **编译器扩展**：不同编译器提供各自的对齐查询扩展
   - GCC: `__alignof__`
   - MSVC: `__alignof`
   - 各编译器行为可能存在差异

2. **手动计算**：开发者需要根据类型大小和平台特性估算对齐要求

3. **移植性问题**：依赖编译器扩展的代码难以移植

C11 引入标准化的 `_Alignof` 解决了这些问题：
- 提供统一的语法和语义
- 确保跨编译器的一致性
- 支持泛型编程和类型特性查询

### C11 特性

- 关键字：`_Alignof`
- 头文件：`<stdalign.h>` 提供便捷宏 `alignof`
- 语法：`_Alignof(type-name)`
- 返回类型：`size_t` 类型的整数常量

### C23 变化

- 新增关键字：`alignof`（不再需要下划线前缀）
- 弃用：`_Alignof` 标记为弃用状态
- 语义保持一致，仅语法更简洁
- `<stdalign.h>` 头文件在 C23 中可能不再需要

### 版本对比

| 特性 | C11 | C23 |
|------|-----|-----|
| 主要关键字 | `_Alignof` | `alignof` |
| 便捷宏 | `alignof` (通过 `<stdalign.h>`) | 直接关键字 |
| 语义 | 返回对齐要求 | 返回对齐要求 |
| 状态 | 标准 | 弃用 |

## 3. 语法与参数

### 基本语法

**C11 语法（C23 中弃用）：**

```c
_Alignof(type-name)
```

**C23 语法：**

```c
alignof(type-name)
```

**C11-C17 使用便捷宏：**

```c
#include <stdalign.h>
alignof(type-name)  // 展开为 _Alignof(type-name)
```

### 参数说明

**type-name**：类型名称

- 必须是完整的对象类型（complete object type）
- 不能是函数类型（function type）
- 不能是不完整类型（incomplete type），如 `void` 或未定义的结构体
- 如果是数组类型，返回元素类型的对齐要求

### 返回值

- 类型：`size_t`（定义在 `<stddef.h>` 中）
- 值：指定类型的对齐要求（以字节为单位）
- 特性：编译期常量（integer constant expression）

### 操作数求值

`alignof` 的操作数**不被求值**（unevaluated operand）：

- 类型名中使用的标识符不需要已定义
- 如果类型是变长数组（VLA），其大小表达式不被求值

### 标准定义

根据 C 标准（C11 6.5.3.4, C23 6.5.3.4）：

> The `_Alignof` (or `alignof` in C23) operator yields the alignment requirement of its operand type. The operand is not evaluated.

## 4. 底层原理

### 对齐的概念

对齐（alignment）是内存访问的基本要求：

1. **硬件要求**：许多处理器要求特定类型的数据从特定边界开始存储
2. **性能优化**：正确对齐的数据访问更快
3. **原子性保证**：某些原子操作要求特定的对齐

### 内存布局示例

```
假设 int 类型的对齐要求为 4 字节：

地址:  0x1000  0x1001  0x1002  0x1003  0x1004  0x1005  0x1006  0x1007
       +-------+-------+-------+-------+-------+-------+-------+-------+
       |  int (正确对齐，地址是4的倍数)      |  int (正确对齐)           |
       +-------+-------+-------+-------+-------+-------+-------+-------+

地址:  0x1001  0x1002  0x1003  0x1004
       +-------+-------+-------+-------+
       |  int (未对齐，可能影响性能或导致硬件异常)  |
       +-------+-------+-------+-------+
```

### 对齐规则的实现

编译器根据以下因素确定对齐要求：

| 类型 | 典型对齐（32位系统） | 典型对齐（64位系统） |
|------|---------------------|---------------------|
| `char` | 1 | 1 |
| `short` | 2 | 2 |
| `int` | 4 | 4 |
| `long` | 4 | 8 |
| `long long` | 4 或 8 | 8 |
| `float` | 4 | 4 |
| `double` | 8 | 8 |
| `指针` | 4 | 8 |

注意：具体对齐要求取决于目标平台 ABI（Application Binary Interface）。

### 结构体对齐

结构体的对齐要求等于其成员中最大的对齐要求：

```c
struct Example {
    char c;    // 对齐: 1
    int n;     // 对齐: 4
    double d;  // 对齐: 8
};

// alignof(struct Example) = 8 (max(1, 4, 8))
```

结构体大小可能包含填充字节（padding）以满足成员对齐要求。

### 与 sizeof 的关系

`alignof` 和 `sizeof` 的关系：

- `sizeof` 返回类型占用的存储大小
- `alignof` 返回类型的对齐要求
- 两者都是编译期常量
- 对齐要求通常是类型大小或更小

```c
sizeof(int) == 4       // 占用 4 字节
alignof(int) == 4      // 需要 4 字节对齐

sizeof(char) == 1      // 占用 1 字节
alignof(char) == 1      // 需要 1 字节对齐（无特殊要求）
```

### 时间复杂度

`alignof` 是编译期运算：

- **编译期**：O(1)，编译器直接查询类型信息
- **运行期**：无开销，结果在编译期已确定

## 5. 使用场景

### 适用场景

| 场景 | 用途 |
|------|------|
| 动态内存分配 | 确保分配的内存满足类型的对齐要求 |
| 自定义内存管理 | 实现对齐感知的内存池 |
| 硬件交互 | 满足硬件寄存器或 DMA 缓冲区的对齐要求 |
| 泛型编程 | 编写类型特性查询代码 |
| 跨平台代码 | 避免硬编码对齐值 |
| 静态断言 | 编译期验证类型对齐 |

### 最佳实践

#### 1. 动态内存分配的对齐保证

```c
#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>

void* aligned_alloc_type(size_t count, size_t type_size, size_t type_align) {
    // 确保分配的内存满足类型对齐要求
    size_t total_size = count * type_size;

    // 使用标准 aligned_alloc (C11)
    // 注意：某些实现要求 size 是 align 的倍数
    return aligned_alloc(type_align, total_size);
}

// 使用示例
int* int_array = aligned_alloc_type(100, sizeof(int), alignof(int));
```

#### 2. 静态断言验证

```c
#include <stdalign.h>
#include <stddef.h>

// 编译期验证自定义结构体的对齐
struct SIMD_Vector {
    float data[4];
};

// 确保 SIMD_Vector 满足 16 字节对齐（SIMD 要求）
static_assert(alignof(struct SIMD_Vector) >= 16,
              "SIMD_Vector must be 16-byte aligned");
```

#### 3. 泛型内存操作

```c
#include <stdalign.h>
#include <stddef.h>
#include <string.h>

// 类型安全的内存复制
#define COPY_ALIGNED(dst, src, type) \
    do { \
        static_assert(alignof(typeof(dst)) >= alignof(type), \
                     "Destination alignment insufficient"); \
        static_assert(alignof(typeof(src)) >= alignof(type), \
                     "Source alignment insufficient"); \
        memcpy(&(dst), &(src), sizeof(type)); \
    } while(0)
```

### 注意事项

#### 1. 类型限制

```c
// ✅ 正确：完整对象类型
alignof(int);           // OK
alignof(double);        // OK
alignof(int[10]);       // OK，返回元素类型 int 的对齐

// ❌ 错误：不完整类型
// alignof(void);       // 编译错误
// alignof(int[]);      // 编译错误（不完整数组类型）

// ❌ 错误：函数类型
// alignof(int(int));   // 编译错误
```

#### 2. 数组类型的对齐

```c
#include <stdalign.h>
#include <stdio.h>

int main(void) {
    printf("alignof(int) = %zu\n", alignof(int));
    printf("alignof(int[10]) = %zu\n", alignof(int[10]));
    // 两者输出相同，数组类型的对齐等于元素类型的对齐
}
```

#### 3. 变长数组（VLA）

```c
#include <stdalign.h>

void vla_example(int n) {
    int vla[n];
    // alignof(typeof(vla)) 在编译期求值
    // n 不会被求值（unevaluated operand）
    // 返回 int 的对齐要求
}
```

### 常见陷阱

#### 陷阱 1：误用为表达式

```c
// ❌ 错误：一些编译器允许表达式，但这是非标准扩展
int x;
// printf("%zu\n", alignof(x));  // 非标准！某些编译器支持

// ✅ 正确：必须使用类型名
printf("%zu\n", alignof(typeof(x)));  // C23 或编译器扩展
printf("%zu\n", alignof(int));        // 标准写法
```

#### 陷阱 2：与 C++ 的差异

```cpp
// C++ 语法（同样使用 alignof）
alignof(int);      // 正确
alignof(double*);  // 正确

// C 语言需要完整类型
alignof(int);      // 正确
```

注意：C++ 的 `alignof` 与 C 的语义基本相同，但 C++11 引入的 `alignas` 说明符与 C 的 `_Alignas` 略有差异。

## 6. 代码示例

### 基础用法

```c
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
    // 基本类型的对齐要求
    printf("Alignment of char = %zu\n", alignof(char));
    printf("Alignment of short = %zu\n", alignof(short));
    printf("Alignment of int = %zu\n", alignof(int));
    printf("Alignment of long = %zu\n", alignof(long));
    printf("Alignment of float = %zu\n", alignof(float));
    printf("Alignment of double = %zu\n", alignof(double));
    printf("Alignment of max_align_t = %zu\n", alignof(max_align_t));

    // 数组类型的对齐
    printf("alignof(float[10]) = %zu\n", alignof(float[10]));

    // 结构体类型的对齐
    printf("alignof(struct{char c; int n;}) = %zu\n",
            alignof(struct {char c; int n;}));

    return 0;
}

/*
可能的输出（64位系统）：
Alignment of char = 1
Alignment of short = 2
Alignment of int = 4
Alignment of long = 8
Alignment of float = 4
Alignment of double = 8
Alignment of max_align_t = 16
alignof(float[10]) = 4
alignof(struct{char c; int n;}) = 4
*/
```

### 高级用法：对齐感知的内存分配器

```c
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 对齐感知的内存块结构
typedef struct {
    size_t size;      // 数据大小
    size_t align;     // 对齐要求
    char data[];      // 柔性数组
} AlignedBlock;

// 分配对齐的内存块
AlignedBlock* aligned_block_create(size_t size, size_t align) {
    // 确保对齐是 2 的幂
    if ((align & (align - 1)) != 0) {
        return NULL;  // 无效的对齐值
    }

    // 计算需要的总大小
    size_t header_size = offsetof(AlignedBlock, data);
    size_t padding = align - (header_size % align);
    if (padding == align) padding = 0;

    size_t total_size = header_size + padding + size;

    // 分配内存
    AlignedBlock* block = malloc(total_size);
    if (block) {
        block->size = size;
        block->align = align;
    }

    return block;
}

// 获取对齐的数据指针
void* aligned_block_data(AlignedBlock* block) {
    size_t header_size = offsetof(AlignedBlock, data);
    size_t padding = block->align - (header_size % block->align);
    if (padding == block->align) padding = 0;

    return (char*)block + header_size + padding;
}

int main(void) {
    // 为 double 数组分配对齐内存
    size_t count = 10;
    AlignedBlock* block = aligned_block_create(
        count * sizeof(double),
        alignof(double)
    );

    if (block) {
        double* data = aligned_block_data(block);

        // 验证对齐
        printf("Allocated block: size=%zu, align=%zu\n",
               block->size, block->align);
        printf("Data pointer: %p\n", (void*)data);
        printf("Is aligned: %s\n",
               ((uintptr_t)data % alignof(double)) == 0 ? "yes" : "no");

        // 使用内存
        for (size_t i = 0; i < count; i++) {
            data[i] = (double)i * 1.5;
        }

        free(block);
    }

    return 0;
}
```

### 实用工具：类型特性查询

```c
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>

// 打印类型信息
#define PRINT_TYPE_INFO(type) \
    do { \
        printf(#type ":\n"); \
        printf("  sizeof:  %zu bytes\n", sizeof(type)); \
        printf("  alignof: %zu bytes\n", alignof(type)); \
        printf("\n"); \
    } while(0)

// 自定义结构体
struct Packed {
    char a;
    int b;
    char c;
};

struct Aligned {
    double a;
    int b;
    char c;
};

int main(void) {
    printf("=== Basic Types ===\n\n");
    PRINT_TYPE_INFO(char);
    PRINT_TYPE_INFO(short);
    PRINT_TYPE_INFO(int);
    PRINT_TYPE_INFO(long);
    PRINT_TYPE_INFO(long long);
    PRINT_TYPE_INFO(float);
    PRINT_TYPE_INFO(double);
    PRINT_TYPE_INFO(void*);

    printf("=== Custom Types ===\n\n");
    PRINT_TYPE_INFO(struct Packed);
    PRINT_TYPE_INFO(struct Aligned);

    printf("=== max_align_t ===\n\n");
    PRINT_TYPE_INFO(max_align_t);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：对函数类型使用 alignof

```c
// ❌ 错误：函数类型没有对齐要求
// printf("%zu\n", alignof(int(int)));  // 编译错误

// ✅ 修正：使用函数指针类型
printf("Function pointer alignment: %zu\n", alignof(int(*)(int)));
```

#### 错误 2：对不完整类型使用 alignof

```c
// ❌ 错误：不完整类型
struct Incomplete;
// printf("%zu\n", alignof(struct Incomplete));  // 编译错误

// ✅ 修正：定义完整类型后使用
struct Complete {
    int x;
};
printf("struct Complete alignment: %zu\n", alignof(struct Complete));
```

#### 错误 3：假设对齐值

```c
// ❌ 错误：硬编码对齐值（不可移植）
// double* ptr = (double*)malloc(100 * sizeof(double) + 8);  // 假设8字节对齐

// ✅ 修正：使用 alignof 动态计算
#include <stdalign.h>
#include <stdlib.h>

double* allocate_doubles(size_t count) {
    size_t align = alignof(double);
    size_t size = count * sizeof(double);

    // C11 aligned_alloc 要求 size 是 align 的倍数
    size_t aligned_size = ((size + align - 1) / align) * align;

    return (double*)aligned_alloc(align, aligned_size);
}
```

### C23 新语法示例

```c
// C23 标准代码（无需 <stdalign.h> 头文件）
#include <stddef.h>
#include <stdio.h>

int main(void) {
    // C23 直接使用 alignof 关键字
    printf("Alignment of int: %zu\n", alignof(int));
    printf("Alignment of double: %zu\n", alignof(double));

    // _Alignof 仍可用，但已弃用
    printf("Using _Alignof: %zu\n", _Alignof(int));  // 不推荐

    return 0;
}
```

## 注意事项

1. **编译期求值**：`alignof` 在编译期计算，不能用于运行时值
2. **类型限制**：只能用于完整对象类型，不支持函数类型和不完整类型
3. **数组处理**：数组类型的对齐等于元素类型的对齐
4. **操作数不求值**：类型名中的表达式不会被求值
5. **平台相关**：具体对齐值取决于目标平台 ABI
6. **版本兼容**：C11 使用 `_Alignof` 或 `<stdalign.h>` 中的 `alignof` 宏；C23 直接使用 `alignof` 关键字

## 相关概念

| 概念 | 关系 |
|------|------|
| `_Alignas` / `alignas` | 对齐说明符，用于指定对象的对齐要求 |
| `max_align_t` | 最大基础对齐类型，对齐要求至少与任何标量类型相同 |
| `aligned_alloc` | 分配指定对齐的内存（C11） |
| `sizeof` | 查询类型大小，同样返回 `size_t` 常量 |
| `_Static_assert` / `static_assert` | 编译期断言，常与 `alignof` 配合使用 |

## 7. 总结

`alignof` 运算符是 C11 引入的类型特性查询工具，提供了标准化的对齐要求查询机制。

### 核心要点

1. **标准化查询**：取代编译器扩展，提供跨平台一致性
2. **编译期常量**：结果在编译时确定，无运行时开销
3. **简单语法**：`alignof(type-name)` 返回 `size_t` 值
4. **类型限制**：仅支持完整对象类型

### 版本选择建议

| 目标标准 | 推荐语法 |
|---------|---------|
| C11/C17 | `#include <stdalign.h>` + `alignof` |
| C23+ | 直接使用 `alignof` 关键字 |

### 使用场景总结

- **内存管理**：动态内存分配的对齐保证
- **硬件交互**：满足硬件特定的对齐要求
- **泛型编程**：类型特性查询和静态断言
- **性能优化**：确保数据结构的最佳内存布局

### 学习建议

1. 理解对齐的底层概念和硬件原因
2. 掌握 `alignof` 与 `sizeof`、`alignas` 的配合使用
3. 了解目标平台的对齐 ABI 规范
4. 熟悉 C11/C23 版本差异和兼容性处理

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.5.3.4 The sizeof and alignof operators
- C17 标准 (ISO/IEC 9899:2018): 6.5.3.4 The sizeof and _Alignof operators
- C11 标准 (ISO/IEC 9899:2011): 6.5.3.4 The sizeof and _Alignof operators
- cppreference: https://en.cppreference.com/w/c/language/_Alignof