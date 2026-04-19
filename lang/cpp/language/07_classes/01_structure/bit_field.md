# Bit-field - 位域

## 1. 概述

**位域（Bit-field）** 是 C++ 中一种特殊的类数据成员声明方式，允许开发者显式指定成员变量占用的位数。相邻的位域成员可能（也可能不）被打包共享和跨越单个字节。

位域主要用于：
- 精确控制数据结构的内存布局
- 与硬件寄存器或协议格式进行交互
- 在内存受限环境中节省存储空间

## 2. 来源与演变

### 历史背景

位域起源于 C 语言，最初设计用于：
- 与硬件设备驱动程序交互
- 处理紧凑的二进制数据格式
- 在内存极其有限的系统上节省空间

### C 与 C++ 的差异

| 特性 | C 语言 | C++ |
|------|--------|-----|
| `int b : 3;` 的符号性 | 由实现定义（可能为有符号或无符号） | 明确为有符号，范围 [-4, 3] |
| 位域宽度限制 | 不能超过底层类型的宽度 | 无此限制（超出部分为填充位） |

### C++11 变化

- 新增属性（attributes）支持，可在位域声明中使用

### C++20 变化

- 新增默认成员初始化器支持：`int b : 3 = 0;`
- 解决了位域大小与默认初始化器之间的歧义问题

### 缺陷报告

| DR | 版本 | 问题 | 修正 |
|-----|------|------|------|
| CWG 324 | C++98 | 位域赋值返回值是否为位域未明确 | 添加位域规范 |
| CWG 739 | C++98 | 未明确声明 signed 或 unsigned 的位域符号性由实现定义 | 与底层类型一致 |
| CWG 2229 | C++98 | 未命名位域可以用 cv 限定类型声明 | 禁止 |
| CWG 2511 | C++98 | 位域类型不允许 cv 限定 | 允许 cv 限定的枚举类型 |

## 3. 语法与参数

### 声明语法

```cpp
// 基本形式
identifier(可选) attr(可选) : size

// 带默认初始化器（C++20 起）
identifier(可选) attr(可选) : size brace-or-equal-initializer
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `identifier` | 位域的名称。可选：未命名的位域引入指定数量的填充位 |
| `attr` | （C++11 起）任意数量的属性序列 |
| `size` | 整型常量表达式，值大于或等于零。大于零时表示位域占用的位数；零仅允许用于未命名位域，具有特殊含义 |
| `brace-or-equal-initializer` | （C++20 起）默认成员初始化器 |

### 类型限制

位域的类型只能是：
- 整型（包括 `bool`）
- （可能有 cv 限定的）枚举类型

**注意**：未命名位域不能用 cv 限定类型声明。

### 特殊语法

```cpp
struct S {
    unsigned int b : 3;     // 有名称的位域，占用 3 位
    unsigned int : 2;       // 未命名位域，2 位填充
    unsigned int : 0;       // 零宽度位域，强制对齐到下一分配单元
};
```

## 4. 底层原理

### 内存布局

位域在内存中的布局是**由实现定义的**，包括：

1. **分配顺序**：某些平台从左到右分配，其他平台从右到左
2. **跨字节**：某些平台允许位域跨越字节边界，其他平台不允许
3. **对齐方式**：位域如何在其分配单元内对齐

### 位域打包示例

```cpp
struct S {
    unsigned char b1 : 3;  // 第一个字节的低 3 位
    unsigned char    : 2;  // 接下来的 2 位作为填充
    unsigned char b2 : 6;  // 不适合第一个字节，开始新的字节
    unsigned char b3 : 2;  // 第二个字节剩余的位
};
// 通常占用 2 字节
```

### 值范围限制

位域的位数限制了其可存储的值范围：

```cpp
struct S {
    unsigned int b : 3;  // 3 位无符号，范围 0-7
};
```

### 类型与宽度的关系

如果位域的指定大小大于其类型的大小，值仍然受类型限制：

```cpp
struct S {
    std::uint8_t b : 1000;  // 仍然只能存储 0-255
};
```

超出类型大小的位成为**填充位（padding bits）**。

### 位域的约束

1. **不能取地址**：位域不一定从字节边界开始，因此无法获取位域的地址
2. **无指针和非常量引用**：无法创建指向位域的指针或非常量引用
3. **常量引用绑定**：从位域初始化常量引用时，会创建一个临时对象

```cpp
struct S { int b : 4; };
S s{5};

// int* p = &s.b;      // 错误：不能取位域地址
// int& r = s.b;       // 错误：不能创建位域的非常量引用
const int& cr = s.b;   // OK：绑定到临时对象
```

### 实现定义的行为

以下行为由实现定义：
- 有符号位域超出范围的赋值或初始化结果
- 有符号位域超出范围的递增结果
- 位域在类对象内的实际分配细节

## 5. 使用场景

### 适合使用位域的场景

| 场景 | 原因 |
|------|------|
| 硬件寄存器映射 | 精确控制位级布局 |
| 网络协议解析 | 与协议格式匹配 |
| 内存受限环境 | 节省存储空间 |
| 标志位集合 | 紧凑存储多个布尔或小范围值 |

### 不适合使用位域的场景

| 场景 | 推荐替代 | 原因 |
|------|---------|------|
| 需要可移植性 | 手动位操作 | 位域布局是实现定义的 |
| 需要取地址 | 普通成员变量 | 位域不能取地址 |
| 需要引用语义 | 普通成员变量 | 位域不支持非常量引用 |
| 需要跨平台兼容 | `std::bitset` | 位域布局因平台而异 |

### 最佳实践

1. **仅用于内存布局已知且固定的场景**：如硬件寄存器
2. **避免依赖位域布局**：不同编译器可能产生不同布局
3. **使用未命名位域进行填充**：控制对齐
4. **使用零宽度位域强制对齐**：确保后续成员从新的分配单元开始

### 注意事项

1. **可移植性问题**：位域布局在不同编译器和平台上可能不同
2. **线程安全**：相邻位域可能被同时访问，需要特别注意同步
3. **有符号位域**：溢出行为由实现定义

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

struct Flags {
    unsigned int is_active : 1;      // 1 位：布尔标志
    unsigned int priority : 3;        // 3 位：0-7 优先级
    unsigned int mode : 2;            // 2 位：4 种模式
    unsigned int        : 2;          // 填充位
    unsigned int value : 8;           // 8 位：值 0-255
};

int main() {
    Flags f{};
    f.is_active = 1;
    f.priority = 5;
    f.mode = 3;
    f.value = 100;

    std::cout << "Active: " << f.is_active << '\n';
    std::cout << "Priority: " << f.priority << '\n';
    std::cout << "Mode: " << f.mode << '\n';
    std::cout << "Value: " << f.value << '\n';
    std::cout << "Size of Flags: " << sizeof(Flags) << " bytes\n";

    return 0;
}
```

### 溢出行为

```cpp
#include <iostream>

struct S {
    // 3 位无符号字段，允许值范围 0-7
    unsigned int b : 3;
};

int main() {
    S s = {6};

    ++s.b;  // 存储 7
    std::cout << s.b << '\n';  // 输出: 7

    ++s.b;  // 值 8 不适合此位域
    std::cout << s.b << '\n';  // 由实现定义，通常输出 0

    return 0;
}
```

### 零宽度位域强制对齐

```cpp
#include <iostream>

struct WithoutPadding {
    unsigned char b1 : 3;
    unsigned char b2 : 2;  // 与 b1 共享字节
};

struct WithPadding {
    unsigned char b1 : 3;
    unsigned char : 0;     // 强制从新字节开始
    unsigned char b2 : 2;
};

int main() {
    std::cout << "WithoutPadding: " << sizeof(WithoutPadding) << " bytes\n";  // 通常 1
    std::cout << "WithPadding: " << sizeof(WithPadding) << " bytes\n";        // 通常 2

    return 0;
}
```

### 位域布局演示

```cpp
#include <bit>
#include <cstdint>
#include <iostream>

struct S {
    // 通常占用 2 字节
    unsigned char b1 : 3;  // 第一个字节的前 3 位
    unsigned char    : 2;  // 接下来 2 位填充
    unsigned char b2 : 6;  // 6 位，不适合第一个字节，开始新字节
    unsigned char b3 : 2;  // 第二个字节剩余的位
};

int main() {
    std::cout << "sizeof(S): " << sizeof(S) << '\n';  // 通常输出 2

    S s;
    s.b1 = 0b111;
    s.b2 = 0b101111;
    s.b3 = 0b11;

    // 使用 bit_cast 查看布局
    auto i = std::bit_cast<std::uint16_t>(s);
    // 通常输出: 1110000011110111
    // 分解:    b1 u  a   b2  b3
    // u = 用户指定的填充, a = 编译器添加的对齐填充
    for (auto b = i; b; b >>= 1) {
        std::cout << (b & 1);
    }
    std::cout << '\n';

    return 0;
}
```

### C++20 默认初始化器

```cpp
#include <iostream>

struct S {
    int x1 : 8 = 42;      // OK: "= 42" 是默认初始化器
    int x2 : 8 {42};      // OK: "{42}" 是默认初始化器
};

int main() {
    S s;
    std::cout << s.x1 << '\n';  // 输出: 42
    std::cout << s.x2 << '\n';  // 输出: 42
    return 0;
}
```

### 常见错误及修正

#### 错误 1：尝试获取位域地址

```cpp
struct S { int b : 4; };
S s{5};

// ❌ 错误：不能获取位域地址
// int* p = &s.b;  // 编译错误

// ✅ 修正：复制值
int val = s.b;
int* p = &val;  // OK
```

#### 错误 2：忽略实现定义的行为

```cpp
// ❌ 错误：假设位域布局
struct HardwareReg {
    uint16_t flag1 : 1;
    uint16_t flag2 : 1;
    uint16_t value : 14;
};
// 不同平台可能产生不同布局！

// ✅ 修正：使用固定宽度类型和手动位操作
uint16_t reg;
bool flag1 = reg & 0x0001;
bool flag2 = reg & 0x0002;
uint16_t value = (reg >> 2) & 0x3FFF;
```

#### 错误 3：有符号位域溢出

```cpp
struct S { signed int b : 3; };  // 范围: -4 到 3

S s;
s.b = 4;  // ❌ 溢出！行为由实现定义

// ✅ 修正：使用无符号类型或检查范围
struct T { unsigned int b : 3; };  // 范围: 0 到 7
T t;
t.b = 4;  // OK
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 内存控制 | 精确指定成员占用的位数 |
| 空间节省 | 相邻位域可共享字节 |
| 布局依赖实现 | 分配顺序、跨字节等行为因平台而异 |
| 无地址 | 不能获取位域地址或创建非常量引用 |
| 类型限制 | 仅限整型和枚举类型 |

### 技术对比

| 方案 | 优点 | 缺点 |
|------|------|------|
| 位域 | 语法简洁，编译器自动处理 | 布局不可移植 |
| 手动位操作 | 完全控制，可移植 | 代码冗长 |
| `std::bitset` | 固定大小，标准接口 | 仅支持布尔位 |
| `std::vector<bool>` | 动态大小 | 特殊化，非真正容器 |

### 使用建议

1. **硬件交互场景**：位域适合映射硬件寄存器布局
2. **跨平台代码**：避免依赖位域布局，使用手动位操作
3. **标志位存储**：位域可用于紧凑存储多个标志
4. **C++20 起**：可使用默认初始化器提高代码清晰度

### 相关概念

| 概念 | 说明 |
|------|------|
| `std::bitset` | 固定长度的位数组类模板 |
| `std::vector<bool>` | 空间高效的动态位集（特化版本） |
| 位操作库 (C++20) | 访问、操作和处理单个位和位序列的工具 |

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 11.4.10 Bit-fields [class.bit]
- C++20 标准 (ISO/IEC 14882:2020): 11.4.9 Bit-fields [class.bit]
- C++17 标准 (ISO/IEC 14882:2017): 12.2.4 Bit-fields [class.bit]
- cppreference: https://en.cppreference.com/w/cpp/language/bit_field