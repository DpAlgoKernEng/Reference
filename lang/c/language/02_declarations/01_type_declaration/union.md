# Union 声明 - 联合体

## 1. 概述

联合体（Union）是 C 语言中的一种特殊数据类型，它允许在同一内存空间中存储不同类型的数据。与结构体（struct）不同，结构体的成员在内存中按顺序分配空间，而联合体的所有成员共享同一块内存空间，这意味着在同一时刻只能存储其中一个成员的值。

联合体的主要特点：
- **内存共享**：所有成员共用同一块内存区域
- **大小由最大成员决定**：联合体的大小至少能容纳其最大的成员
- **互斥存储**：同一时刻只有一个成员是"活跃"的
- **类型双关（Type Punning）**：可以用于重新解释同一内存内容的不同类型表示

## 2. 来源与演变

### 历史背景

联合体起源于 C 语言早期设计，与结构体同时出现。其设计动机主要来自以下需求：

1. **内存节省**：在早期计算机内存资源有限的背景下，联合体提供了一种在不同类型数据间共享内存的方式
2. **硬件抽象**：直接映射硬件寄存器的不同解释方式
3. **类型转换**：提供一种安全的类型重新解释机制

### 标准演进

| 标准 | 版本 | 主要变化 |
|------|------|----------|
| C89/C90 | ISO/IEC 9899:1990 | 基础联合体语法和语义定义 |
| C99 | ISO/IEC 9899:1999 | 明确类型双关行为（DR 283），之前为未定义行为 |
| C11 | ISO/IEC 9899:2011 | 引入匿名联合体（anonymous union） |
| C23 | ISO/IEC 9899:2024 | 新增属性说明符（attr-spec-seq）支持 |

### 重要缺陷报告

| DR | 应用于 | 发布时行为 | 修正行为 |
|------|------|------|------|
| DR 283 | C99 TC3 | 类型双关行为未定义 | 明确为重新解释对象表示 |
| DR 499 | C11 | 匿名结构体/联合体成员被视为封闭结构体/联合体的成员 | 保持其内存布局 |

## 3. 语法与参数

### 基本语法

```
union attr-spec-seq(可选) name(可选) { struct-declaration-list }    (1)
union attr-spec-seq(可选) name                                       (2)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `name` | 正在定义的联合体名称 |
| `struct-declaration-list` | 任意数量的变量声明、位域声明和静态断言声明。不允许不完整类型和函数类型的成员 |
| `attr-spec-seq` | (C23) 可选的属性列表，应用于联合体类型。形式 (2) 若不以 `;` 结尾（即非前向声明）则不允许使用 |

### 声明示例

```c
// 完整定义
union Data {
    int i;
    float f;
    char str[20];
};

// 前向声明
union Data;

// 匿名联合体 (C11)
union {
    int x;
    float y;
};

// 带标签的联合体
union tagged_value {
    int type;
    union {
        int int_val;
        float float_val;
        char* str_val;
    } value;
};
```

### 成员限制

联合体成员的限制条件：
- 不允许不完整类型（incomplete type）成员
- 不允许函数类型成员
- 成员可以是位域（bit-field）

## 4. 底层原理

### 内存布局

联合体的内存布局特点：

```
假设有以下联合体定义：
union Example {
    int i;       // 4 字节
    double d;    // 8 字节
    char c;      // 1 字节
};

内存布局示意：
+--------+--------+--------+--------+--------+--------+--------+--------+
| byte 0 | byte 1 | byte 2 | byte 3 | byte 4 | byte 5 | byte 6 | byte 7 |
+--------+--------+--------+--------+--------+--------+--------+--------+
| <-------- i -------->     | <------------------- d -----------------> |
| c       |                                              | 尾部填充      |
+--------+--------+--------+--------+--------+--------+--------+--------+

- 大小：8 字节（等于最大成员 double 的大小）
- 对齐：8 字节对齐（取成员中最严格的对齐要求）
- 可能有尾部填充以满足对齐要求
```

### 大小计算规则

联合体大小遵循以下规则：

1. **基础大小**：至少等于最大成员的大小
2. **对齐填充**：可能添加尾部填充以满足最严格的对齐要求
3. **最终大小**：必须是对齐要求的整数倍

### 指针转换

```c
union S {
    int i;
    float f;
} s;

// 以下指针转换是合法的
int* pi = (int*)&s;      // 指向联合体的指针可转换为成员指针
union S* ps = (union S*)pi;  // 成员指针可转换回联合体指针
```

### 类型双关（Type Punning）

从 C99 TC3 开始，通过非活跃成员访问联合体是合法的类型双关操作：

```c
union {
    float f;
    uint32_t bits;
} u;
u.f = 1.0f;
uint32_t float_bits = u.bits;  // 合法：重新解释 float 的位表示
```

**重要说明**：
- 如果新类型大小大于最后写入的类型，多余字节的内容未指定
- 可能产生陷阱表示（trap representation）
- 不同字节序（endianness）的机器上结果不同

### 时间复杂度

| 操作 | 时间复杂度 |
|------|-----------|
| 成员访问 | O(1) |
| 大小计算（编译时） | O(1) |
| 初始化 | O(1) |

## 5. 使用场景

### 适合使用联合体的场景

| 场景 | 说明 |
|------|------|
| 节省内存 | 多个互斥数据共享内存空间 |
| 类型双关 | 安全地重新解释数据的位表示 |
| 硬件寄存器映射 | 同一寄存器可能有多种解释 |
| 变体类型实现 | 实现类似"标签联合"的数据结构 |
| 网络协议解析 | 同一数据在不同视角下的解释 |

### 最佳实践

#### 1. 带标签的联合体（Tagged Union）

```c
// 推荐做法：使用标签区分当前活跃成员
enum ValueType { TYPE_INT, TYPE_FLOAT, TYPE_STRING };

struct TaggedValue {
    enum ValueType type;
    union {
        int int_val;
        float float_val;
        char* string_val;
    } data;
};
```

#### 2. 类型安全的访问

```c
// 在访问前检查标签
void print_value(const struct TaggedValue* v) {
    switch (v->type) {
        case TYPE_INT:
            printf("%d\n", v->data.int_val);
            break;
        case TYPE_FLOAT:
            printf("%f\n", v->data.float_val);
            break;
        case TYPE_STRING:
            printf("%s\n", v->data.string_val);
            break;
    }
}
```

#### 3. 网络数据解析

```c
// 解析网络数据包
union PacketData {
    struct {
        uint8_t header;
        uint8_t payload[7];
    } raw;
    struct {
        uint8_t header;
        uint16_t command;
        uint32_t parameter;
        uint8_t checksum;
    } parsed;
};
```

### 常见陷阱

| 陷阱 | 风险 | 解决方案 |
|------|------|----------|
| 忘记当前活跃成员 | 读取错误数据 | 使用标签联合体模式 |
| 字节序问题 | 移植性问题 | 明确处理字节序转换 |
| 未初始化访问 | 未定义行为 | 确保首次写入后才能读取 |
| 陷阱表示 | 硬件异常 | 避免产生非法位模式 |

### 线程安全性

联合体本身**不是线程安全**的：
- 多线程同时读写同一联合体成员会导致数据竞争
- 多线程读写不同成员但共享同一内存同样有竞争风险
- 需要使用同步机制（如互斥锁）保护访问

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>
#include <stdint.h>

int main(void) {
    // 定义联合体
    union Data {
        int i;
        float f;
        char str[4];
    };

    union Data data;

    // 存储整数
    data.i = 42;
    printf("data.i = %d\n", data.i);

    // 存储浮点数（覆盖之前的值）
    data.f = 3.14f;
    printf("data.f = %f\n", data.f);
    // 注意：此时 data.i 的值已不可预测

    // 存储字符串
    data.str[0] = 'A';
    data.str[1] = 'B';
    data.str[2] = 'C';
    data.str[3] = '\0';
    printf("data.str = %s\n", data.str);

    return 0;
}
```

### 高级用法：类型双关

```c
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

int main(void) {
    // 类型双关示例：查看浮点数的内部表示
    union S {
        uint32_t u32;
        uint16_t u16[2];
        uint8_t u8;
    } s = {0x12345678};  // s.u32 现在是活跃成员

    printf("Union S has size %zu and holds %x\n", sizeof s, s.u32);

    s.u16[0] = 0x0011;  // s.u16 现在是活跃成员

    // 从 s.u32 或 s.u8 读取会重新解释对象表示
    // printf("s.u8 is now %x\n", s.u8);   // 未指定，可能是 11 或 00
    // printf("s.u32 is now %x\n", s.u32); // 未指定

    // 指针比较：所有成员指针都等于联合体地址
    assert((uint8_t*)&s == &s.u8);

    // 尾部填充示例
    union pad {
        char c[5];  // 占用 5 字节
        float f;    // 占用 4 字节，要求 4 字节对齐
    } p = { .f = 1.23 };  // 大小为 8 以满足 float 对齐

    printf("size of union of char[5] and float is %zu\n", sizeof p);

    return 0;
}
```

### 匿名联合体示例 (C11)

```c
#include <stdio.h>

int main(void) {
    struct v {
        union {  // 匿名联合体
            struct { int i, j; };  // 匿名结构体
            struct { long k, l; } w;
        };
        int m;
    } v1;

    v1.i = 2;     // 有效：直接访问匿名联合体/结构体的成员
    v1.j = 3;
    // v1.k = 3; // 无效：内部结构体 w 不是匿名的
    v1.w.k = 5;  // 有效
    v1.m = 10;

    printf("v1.i = %d, v1.j = %d, v1.w.k = %ld, v1.m = %d\n",
           v1.i, v1.j, v1.w.k, v1.m);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：忘记跟踪活跃成员

```c
// 错误：忘记跟踪哪个成员是活跃的
union Value {
    int i;
    float f;
} v;

v.i = 42;
v.f = 3.14f;  // 覆盖了 i
printf("%d\n", v.i);  // 未定义行为：读取非活跃成员

// 修正：使用标签联合体
enum Type { INT, FLOAT };
struct SafeValue {
    enum Type tag;
    union {
        int i;
        float f;
    } data;
};

struct SafeValue sv;
sv.tag = INT;
sv.data.i = 42;

// 读取前检查标签
if (sv.tag == INT) {
    printf("%d\n", sv.data.i);
}
```

#### 错误 2：字节序依赖

```c
// 错误：假设特定字节序
union {
    uint32_t value;
    uint8_t bytes[4];
} u;
u.value = 0x12345678;
printf("first byte: %x\n", u.bytes[0]);  // 结果取决于字节序

// 修正：明确处理字节序
uint32_t value = 0x12345678;
uint8_t bytes[4];
bytes[0] = (value >> 24) & 0xFF;  // 大端序
bytes[1] = (value >> 16) & 0xFF;
bytes[2] = (value >> 8) & 0xFF;
bytes[3] = value & 0xFF;
```

#### 错误 3：未初始化访问

```c
// 错误：未初始化就读取
union Data {
    int i;
    float f;
} data;
printf("%d\n", data.i);  // 未定义行为：读取未初始化的值

// 修正：确保初始化
union Data data = { .i = 0 };  // 使用指定初始化器
// 或
union Data data;
data.i = 0;  // 先写入再读取
printf("%d\n", data.i);
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 内存共享 | 所有成员共享同一内存空间 |
| 大小计算 | 等于最大成员大小 + 尾部填充 |
| 对齐要求 | 取所有成员中最严格的对齐要求 |
| 同时存储 | 同一时刻只能存储一个成员的值 |
| 类型双关 | 从 C99 TC3 起支持通过非活跃成员重新解释数据 |

### 联合体 vs 结构体对比

| 方面 | 联合体 (union) | 结构体 (struct) |
|------|----------------|-----------------|
| 内存分配 | 所有成员共享同一空间 | 成员按顺序分配独立空间 |
| 大小 | 最大成员大小 + 填充 | 所有成员大小之和 + 填充 |
| 同时存储 | 只能存储一个成员 | 可同时存储所有成员 |
| 典型用途 | 节省内存、类型双关 | 组织相关数据 |
| 访问安全性 | 需要跟踪活跃成员 | 所有成员始终有效 |

### 学习建议

1. **理解内存模型**：画出联合体的内存布局图有助于理解
2. **使用标签联合体**：在实际应用中，始终使用标签来跟踪当前活跃成员
3. **注意可移植性**：类型双关可能在不同平台产生不同结果
4. **谨慎使用**：联合体是强大但易错的工具，仅在必要时使用

### 相关概念

- **结构体 (struct)**：成员顺序存储的数据结构
- **位域 (bit-field)**：联合体成员可以是位域
- **匿名联合体 (anonymous union)**：C11 引入，成员可直接访问
- **标签联合体 (tagged union)**：带类型标签的联合体，实现变体类型

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.7.2.1 Structure and union specifiers
- C17 标准 (ISO/IEC 9899:2018): 6.7.2.1 Structure and union specifiers
- C11 标准 (ISO/IEC 9899:2011): 6.7.2.1 Structure and union specifiers
- C99 标准 (ISO/IEC 9899:1999): 6.7.2.1 Structure and union specifiers
- C89/C90 标准 (ISO/IEC 9899:1990): 3.5.2.1 Structure and union specifiers