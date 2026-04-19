# 位域 (Bit-fields)

## 1. 概述

**位域 (Bit-field)** 是 C 语言中结构体或联合体的成员声明方式，允许程序员显式指定成员占用的位数。相邻的位域成员可以被打包共享或跨越单个字节，从而实现对内存空间的精细化控制。

位域的主要用途：
- 节省内存空间，当只需要存储少量数据时
- 硬件寄存器映射，精确匹配硬件位布局
- 网络协议实现，处理位级别的数据打包

## 2. 来源与演变

### 历史背景

位域特性自 **C89/C90** 标准就已存在，主要用于：
- 系统编程中对硬件寄存器的精确控制
- 内存受限环境下优化存储空间
- 网络协议和文件格式的位级数据打包

### 版本演进

| 标准 | 新增特性 |
|------|---------|
| C89/C90 | 基本位域支持（int、signed int、unsigned int） |
| C99 | 新增 `_Bool` 类型的单比特位域支持 |
| C11 | 新增原子类型位域支持（实现定义） |
| C23 | 新增位精确整数类型 `_BitInt(N)` 支持 |

### C 语言与 C++ 的差异

| 特性 | C 语言 | C++ 语言 |
|------|--------|---------|
| int 类型符号性 | 实现定义（可 signed 或 unsigned） | 始终为 signed |
| 位宽超过类型宽度 | 不允许 | 允许（多余位为填充位） |

## 3. 语法与参数

### 基本语法

```c
identifier(可选) : width
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `identifier` | 位域成员的名称。可选参数：如果省略，则创建无名位域用于填充（padding） |
| `width` | 整数常量表达式，值必须大于等于 0 且小于等于底层类型的位数。当值大于 0 时，表示位域占用的位数。值为 0 时仅允许用于无名位域，表示下一个位域从新的分配单元边界开始 |

### 允许的类型

位域只能使用以下类型（可带 const 或 volatile 限定符）：

| 类型 | 说明 | 示例 |
|------|------|------|
| `unsigned int` | 无符号位域 | `unsigned int b : 3;` 范围 [0, 7] |
| `signed int` | 有符号位域 | `signed int b : 3;` 范围 [-4, 3] |
| `int` | 符号性由实现定义 | `int b : 3;` 范围可能是 [0, 7] 或 [-4, 3] |
| `_Bool` | 单比特位域 (C99 起) | `_Bool x : 1;` 范围 [0, 1] |
| `_BitInt(N)` | 位精确整数 (C23 起) | `_BitInt(5) : 4;` 范围 [-8, 7] |

### 类型与值范围

位域的宽度决定了其可存储的值范围：

```c
// 3 位无符号位域：范围 [0, 7]
unsigned int u : 3;

// 3 位有符号位域：范围 [-4, 3]
signed int s : 3;

// C23: 4 位位精确整数
_BitInt(5) x : 4;        // 范围 [-8, 7]
unsigned _BitInt(5) y : 4; // 范围 [0, 15]
```

## 4. 底层原理

### 内存布局

位域成员在内存中的布局由编译器决定，通常相邻位域会被打包在一起：

```
假设 unsigned int 为 32 位：
+--------+--------+--------+--------+
|  b1    | unused |   b2   |   b3  |
| 5 bits | 11 bits| 6 bits | 2 bits|
+--------+--------+--------+--------+
            4 字节 (32 bits)
```

### 分配单元 (Allocation Unit)

分配单元是编译器分配给位域的基础内存单位，通常是一个 `unsigned int` 的大小（如 4 字节）。

### 无名位域与填充

**无名位域**用于引入填充位：

```c
struct S {
    unsigned a : 5;
    unsigned   : 11;  // 11 位填充，不可访问
    unsigned b : 6;
};
```

### 宽度为 0 的特殊位域

宽度为 0 的无名位域强制下一个位域从新的分配单元边界开始：

```c
struct S {
    unsigned b1 : 5;    // 分配单元 1 开始
    unsigned    : 0;    // 强制对齐到下一个分配单元
    unsigned b2 : 6;    // 分配单元 2 开始
    unsigned b3 : 15;
};
// 通常占用 8 字节（2 个分配单元）
```

### 位域的限制

位域有以下限制：
- **不能取地址**：位域不一定从字节边界开始
- **不能使用指针**：不存在指向位域的指针
- **不能使用 sizeof**：位域不是完整对象
- **不能使用 alignas/_Alignas**：对齐要求不适用于位域

### 实现定义的行为

| 行为 | 说明 |
|------|------|
| `int` 的符号性 | 由编译器决定是 signed 还是 unsigned |
| 允许的类型 | 部分编译器允许 `char`、`short`、`long` 等类型 |
| 跨分配单元边界 | 位域是否可以跨越分配单元边界由实现定义 |
| 位序 | 位域在分配单元内的排列顺序（从左到右或从右到左） |
| 原子类型支持 | 是否允许原子类型位域（C11 起） |

## 5. 使用场景

### 适合使用位域的场景

| 场景 | 说明 |
|------|------|
| 硬件寄存器映射 | 精确匹配硬件寄存器的位布局 |
| 网络协议头解析 | 处理 TCP/IP 等协议的位级字段 |
| 内存受限环境 | 节省存储空间 |
| 标志位集合 | 存储多个布尔标志 |

### 不适合使用位域的场景

| 场景 | 原因 |
|------|------|
| 需要取地址的操作 | 位域不支持取地址 |
| 跨平台数据交换 | 位序和布局依赖实现 |
| 高性能计算 | 位域访问可能比普通成员慢 |
| 多线程环境 | 位域操作不是原子的 |

### 最佳实践

1. **使用明确的有符号/无符号类型**：避免使用 `int`，改用 `signed int` 或 `unsigned int`
2. **避免跨分配单元**：使用宽度为 0 的无名位域确保对齐
3. **不要依赖位序**：不同平台位域排列顺序可能不同
4. **文档化实现依赖**：明确标注代码中的实现定义行为

### 注意事项

#### 未定义行为

- 对位域使用 `offsetof` 宏

#### 未指定行为

- 位域所在分配单元的对齐方式

#### 实现定义行为

- `int` 类型位域的符号性
- 是否允许非标准类型（如 `char`、`short`、`long` 等）
- 位域是否可跨越分配单元边界
- 位域在分配单元内的排列顺序
- C99 起：`_Bool` 类型位域宽度不能超过 1

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>

struct Flags {
    unsigned int readable   : 1;
    unsigned int writable   : 1;
    unsigned int executable : 1;
    unsigned int reserved   : 5;  // 保留位
};

int main(void) {
    struct Flags f = {1, 1, 0, 0};

    printf("Readable: %u\n", f.readable);     // 输出: 1
    printf("Writable: %u\n", f.writable);      // 输出: 1
    printf("Executable: %u\n", f.executable); // 输出: 0

    // 修改标志
    f.executable = 1;
    f.writable = 0;

    return 0;
}
```

### 位域溢出行为

```c
#include <stdio.h>

struct S {
    // 3 位无符号位域，允许值范围 [0, 7]
    unsigned int b : 3;
};

int main(void) {
    struct S s = {7};

    printf("初始值: %u\n", s.b);  // 输出: 7

    ++s.b;  // 无符号溢出

    printf("溢出后: %u\n", s.b);  // 输出: 0（回绕）

    return 0;
}
```

### 无名位域与填充

```c
#include <stdio.h>

struct Packed {
    unsigned b1 : 5;
    unsigned    : 11;  // 无名位域：填充
    unsigned b2 : 6;
    unsigned b3 : 2;
};

struct Aligned {
    unsigned b1 : 5;
    unsigned    : 0;   // 零宽度：强制新分配单元
    unsigned b2 : 6;
    unsigned b3 : 15;
};

int main(void) {
    printf("Packed 大小: %zu 字节\n", sizeof(struct Packed)); // 通常 4 字节
    printf("Aligned 大小: %zu 字节\n", sizeof(struct Aligned)); // 通常 8 字节

    return 0;
}
```

### 硬件寄存器映射

```c
#include <stdint.h>

// 模拟 UART 控制寄存器
struct UARTControl {
    uint32_t data_bits    : 4;  // 数据位数 (5-8)
    uint32_t parity       : 2;  // 校验模式
    uint32_t stop_bits    : 1;  // 停止位
    uint32_t enable       : 1;  // 使能位
    uint32_t reserved     : 24; // 保留
};

// 模拟状态寄存器
struct UARTStatus {
    uint32_t tx_empty    : 1;   // 发送缓冲区空
    uint32_t rx_ready    : 1;   // 接收数据就绪
    uint32_t frame_error  : 1;  // 帧错误
    uint32_t parity_error : 1;  // 校验错误
    uint32_t overrun      : 1;  // 溢出错误
    uint32_t reserved     : 27;
};

int main(void) {
    volatile struct UARTControl *ctrl = (volatile struct UARTControl *)0x40001000;
    volatile struct UARTStatus *status = (volatile struct UARTStatus *)0x40001004;

    // 配置 UART：8 数据位，无校验，1 停止位
    ctrl->data_bits = 8;
    ctrl->parity = 0;
    ctrl->stop_bits = 0;
    ctrl->enable = 1;

    // 检查接收状态
    if (status->rx_ready) {
        // 读取数据...
    }

    return 0;
}
```

### 常见错误及修正

#### 错误 1：对位域取地址

```c
// 错误：不能对位域取地址
struct S {
    unsigned int x : 4;
};

int main(void) {
    struct S s = {5};
    // unsigned int *p = &s.x;  // 编译错误！

    // 正确方式：通过整体结构体操作
    s.x = 10;
    return 0;
}
```

#### 错误 2：依赖 int 的符号性

```c
#include <stdio.h>

struct Flags {
    int sign_bit : 1;  // 符号性不确定！
};

int main(void) {
    struct Flags f = {1};

    // 错误：sign_bit 可能是 -1 或 1，取决于编译器
    printf("sign_bit = %d\n", f.sign_bit);

    // 正确方式：明确指定符号性
    struct Fixed {
        unsigned int sign_bit : 1;  // 明确为无符号
    };

    return 0;
}
```

#### 错误 3：使用 sizeof 计算位域大小

```c
#include <stdio.h>

struct S {
    unsigned int a : 4;
    unsigned int b : 4;
};

int main(void) {
    struct S s;

    // 错误：sizeof(s.a) 不合法
    // printf("a 的大小: %zu\n", sizeof(s.a));  // 编译错误

    // 正确方式：计算整个结构体大小
    printf("结构体大小: %zu 字节\n", sizeof(s));

    return 0;
}
```

#### 错误 4：跨平台位序依赖

```c
// 错误：假设位域从低到高排列
struct NetworkHeader {
    uint16_t version : 4;
    uint16_t ihl : 4;
    // 注意：不同平台可能按不同顺序存储
};

// 正确方式：使用位运算确保可移植性
uint16_t header = 0x4500;
uint8_t version = (header >> 12) & 0x0F;
uint8_t ihl = (header >> 8) & 0x0F;
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 内存节省 | 精确控制成员占用位数 |
| 硬件映射 | 匹配硬件寄存器布局 |
| 访问限制 | 不能取地址、不能用指针、不能用 sizeof |
| 可移植性 | 位序和布局依赖实现 |

### 类型选择建议

| 需求 | 推荐类型 |
|------|---------|
| 明确无符号 | `unsigned int` |
| 明确有符号 | `signed int` |
| 布尔标志 | `_Bool` (C99) 或 `unsigned int : 1` |
| 特定位宽整数 | `_BitInt(N)` (C23) |

### 使用建议

1. **明确指定符号性**：使用 `signed int` 或 `unsigned int`，避免 `int`
2. **了解实现定义行为**：不同编译器可能有不同行为
3. **避免跨平台依赖**：位序和布局因平台而异
4. **用于内存映射硬件**：这是位域最主要的用途

### 相关概念

| 概念 | 关系 |
|------|------|
| 结构体 (Struct) | 位域只能在结构体或联合体中声明 |
| 联合体 (Union) | 位域也可用于联合体 |
| 对齐 (Alignment) | 零宽度位域影响对齐 |
| 字节序 (Endianness) | 影响位域在内存中的排列 |

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.7.2.1 Structure and union specifiers
- C17 标准 (ISO/IEC 9899:2018): 6.7.2.1 Structure and union specifiers
- C11 标准 (ISO/IEC 9899:2011): 6.7.2.1 Structure and union specifiers
- C99 标准 (ISO/IEC 9899:1999): 6.7.2.1 Structure and union specifiers
- C89/C90 标准 (ISO/IEC 9899:1990): 3.5.2.1 Structure and union specifiers
- cppreference: https://en.cppreference.com/w/c/language/bit_field