# 基本类型 (Fundamental Types)

## 1. 概述 (Overview)

基本类型（Fundamental Types）是 C++ 类型系统的基础构建块，由以下类型组成：

| 类型分类 | 包含类型 | 说明 |
|---------|---------|------|
| `void` | `void` | 空类型，表示无值 |
| 空指针类型 | `std::nullptr_t` | `nullptr` 的类型 (C++11) |
| 整数类型 | `bool`、字符类型、标准整数 | 表示整数值 |
| 浮点类型 | `float`、`double`、`long double` | 表示实数 |

所有基本类型都可以带有 cv 限定符（const、volatile）。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 的基本类型继承自 C 语言，并在此基础上进行了扩展和完善。C++98 标准确立了基础类型体系，后续标准持续增强。

### 版本演进

| C++ 标准版本 | 发布年份 | 基本类型相关改进 |
|-------------|---------|----------------|
| C++98 | 1998 | 标准化基础类型体系 |
| C++11 | 2011 | 引入 `std::nullptr_t`、`char16_t`、`char32_t`、`long long` |
| C++20 | 2020 | 引入 `char8_t`；强制二进制补码表示有符号整数 |
| C++23 | 2023 | 扩展浮点类型支持 |

### 重要缺陷报告

| 缺陷报告 | 问题 | 解决方案 |
|---------|------|---------|
| CWG 1759 | `char` 无法保证存储 UTF-8 码元 0x80 | C++11 起保证可存储 |
| CWG 2689 | cv 限定的 `std::nullptr_t` 不是基本类型 | 修正为基本类型 |
| CWG 2723 | 浮点类型的可表示值范围未明确 | 明确定义范围规则 |
| P2460R2 | `wchar_t` 要求过大无法在 Windows 上满足 | 移除不合理要求 |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 void 类型

```cpp
void                    // 空类型
const void              // const 限定的 void
volatile void           // volatile 限定的 void
```

**特性**：
- 空值集合的类型
- 不完整类型，无法被完成
- 不能定义 `void` 类型的对象
- 不能创建 `void` 数组或 `void` 引用
- 允许 `void*` 指针和返回 `void` 的函数

### 3.2 std::nullptr_t

```cpp
#include <cstddef>

std::nullptr_t          // nullptr 的类型
decltype(nullptr)       // 等价定义
```

**特性**：
- 空指针字面量 `nullptr` 的类型
- 独立类型，既不是指针类型也不是成员指针类型
- 所有纯右值都是空指针常量
- `sizeof(std::nullptr_t) == sizeof(void*)`

### 3.3 整数类型

#### 标准整数类型

```cpp
// 基本整数类型
int                     // 基本整数类型

// 符号修饰符
signed int              // 有符号（默认）
unsigned int            // 无符号

// 大小修饰符
short int               // 短整数，至少 16 位
long int                // 长整数，至少 32 位
long long int           // 长长整数，至少 64 位 (C++11)

// 组合形式（顺序任意）
unsigned long long int  // 等价于 long int unsigned long
```

#### 类型说明符等价关系

| 类型说明符 | 等价类型 |
|-----------|---------|
| `short` | `short int` |
| `signed short` | `short int` |
| `unsigned short` | `unsigned short int` |
| `int` | `signed int` |
| `signed` | `signed int` |
| `unsigned` | `unsigned int` |
| `long` | `long int` |
| `unsigned long` | `unsigned long int` |
| `long long` | `long long int` (C++11) |

### 3.4 布尔类型

```cpp
bool                    // 布尔类型
true                    // 真值
false                   // 假值
```

**特性**：
- 整数类型的一种
- 只能持有 `true` 或 `false` 两个值
- `sizeof(bool)` 由实现定义，可能不为 1

### 3.5 字符类型

```cpp
char                    // 基本字符类型
signed char             // 有符号字符类型
unsigned char           // 无符号字符类型
wchar_t                 // 宽字符类型
char16_t                // UTF-16 字符 (C++11)
char32_t                // UTF-32 字符 (C++11)
char8_t                 // UTF-8 字符 (C++20)
```

**各字符类型特性**：

| 类型 | 用途 | 大小 |
|-----|------|-----|
| `char` | 基本字符表示、多字节字符串 | 至少 8 位 |
| `signed char` | 有符号字符 | 同 `char` |
| `unsigned char` | 无符号字符、原始内存访问 | 同 `char` |
| `wchar_t` | 宽字符 | 平台相关（Linux 32 位，Windows 16 位） |
| `char16_t` | UTF-16 码元 | 16 位 |
| `char32_t` | UTF-32 码元 | 32 位 |
| `char8_t` | UTF-8 码元 (C++20) | 8 位 |

### 3.6 浮点类型

```cpp
float                   // 单精度浮点
double                  // 双精度浮点
long double             // 扩展精度浮点
```

**特性**：

| 类型 | 典型格式 | 精度 |
|-----|---------|------|
| `float` | IEEE-754 binary32 | ~7 位十进制 |
| `double` | IEEE-754 binary64 | ~16 位十进制 |
| `long double` | 平台相关 | 至少同 `double` |

## 4. 底层原理 (Underlying Principles)

### 4.1 整数存储表示

#### 有符号整数表示法

C++20 之前，标准允许三种有符号整数表示法：

| 表示法 | N 位范围 | 状态 |
|-------|---------|------|
| 二进制补码 (Two's complement) | -2^(N-1) 到 2^(N-1)-1 | C++20 起唯一允许 |
| 反码 (One's complement) | -(2^(N-1)-1) 到 2^(N-1)-1 | C++20 前允许 |
| 符号-大小 (Sign-magnitude) | -(2^(N-1)-1) 到 2^(N-1)-1 | C++20 前允许 |

**C++20 重大变更**：所有实际编译器都使用二进制补码，C++20 将其定为唯一合法表示法。

#### UTF-8 码元存储问题

C++11 通过 CWG 1759 解决了 `char` 存储 UTF-8 码元 0x80 的问题：

```cpp
// C++11 起保证可行
char utf8_byte = '\x80';  // UTF-8 码元值 128
```

这导致 C++11 起禁止 `char` 使用 8 位反码或符号-大小表示法。

### 4.2 数据模型

数据模型定义了基本类型的大小配置：

| 模型 | int | long | pointer | 典型应用 |
|-----|-----|------|---------|---------|
| LP32 | 16 位 | 32 位 | 32 位 | Win16 API |
| ILP32 | 32 位 | 32 位 | 32 位 | Win32 API, 32 位 Unix |
| LLP64 | 32 位 | 32 位 | 64 位 | 64 位 Windows API |
| LP64 | 32 位 | 64 位 | 64 位 | 64 位 Unix/Linux/macOS |

### 4.3 类型大小关系

标准保证的大小关系：

```cpp
1 == sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)
```

**极端情况**：理论上允许 64 位字节，所有类型都是 64 位宽，`sizeof` 对所有类型返回 1。

### 4.4 浮点数表示

#### IEEE-754 格式

| 格式 | 位宽 | 符号 | 指数 | 尾数 | 典型用途 |
|-----|-----|-----|-----|-----|---------|
| binary32 | 32 | 1 | 8 | 23 | `float` |
| binary64 | 64 | 1 | 11 | 52 | `double` |
| binary128 | 128 | 1 | 15 | 112 | 部分 `long double` |
| x87 扩展 | 80 | 1 | 15 | 64 | x86 `long double` |

#### 特殊浮点值

| 特殊值 | 说明 |
|-------|------|
| 正/负无穷 | `INFINITY`，如 `1.0 / 0.0` |
| 负零 | `-0.0`，与正零相等但运算行为不同 |
| 非数 (NaN) | `NAN`，不等于任何值（包括自身） |

### 4.5 浮点类型范围定义

C++ 标准对浮点类型范围的定义：

1. **最小保证范围**：从最负有限浮点数到最正有限浮点数
2. **负无穷扩展**：若支持负无穷，范围扩展到所有负实数
3. **正无穷扩展**：若支持正无穷，范围扩展到所有正实数

IEEE-754 格式支持正负无穷，因此所有实数都在其可表示值范围内。

## 5. 使用场景 (Use Cases)

### 5.1 类型选择指南

| 场景 | 推荐类型 | 原因 |
|-----|---------|------|
| 函数无返回值 | `void` | 语义明确 |
| 空指针 | `nullptr` / `std::nullptr_t` | 类型安全 |
| 布尔逻辑 | `bool` | 语义清晰 |
| 一般字符 | `char` | 平台最优处理 |
| 原始内存操作 | `unsigned char` | 无填充、可别名 |
| UTF-8 字符串 | `char8_t` (C++20) 或 `char` | 明确编码意图 |
| UTF-16 字符串 | `char16_t` | 明确 16 位码元 |
| UTF-32 字符串 | `char32_t` | 明确 32 位码元 |
| 宽字符（平台相关） | `wchar_t` | 系统API 兼容 |
| 一般整数运算 | `int` | 平台最优 |
| 位操作 | `unsigned` 系列 | 避免未定义行为 |
| 大整数 | `long long` | 保证 64 位 |
| 对象大小 | `std::size_t` | 与 `sizeof` 一致 |
| 科学计算 | `double` | 精度与性能平衡 |
| 高精度计算 | `long double` | 扩展精度 |

### 5.2 注意事项

#### char 的符号性

```cpp
// char 的符号性取决于编译器和平台
// ARM/PowerPC: 通常为 unsigned
// x86/x64: 通常为 signed

char c = 200;  // 行为可能不同
// 明确需求时应使用 signed char 或 unsigned char
```

#### wchar_t 的平台差异

```cpp
// Linux/macOS: 32 位，通常存储 UTF-32
// Windows: 16 位，存储 UTF-16 码元

// 跨平台代码应避免依赖 wchar_t 的具体大小
```

#### long double 的实现差异

```cpp
// MSVC: 与 double 相同 (64 位)
// GCC/Clang (x86): 80 位 x87 扩展精度
// 部分 ARM: 128 位 IEEE-754 binary128
```

### 5.3 常见陷阱

1. **混淆 `char` 与 `signed char`/`unsigned char`**：三者是不同类型
2. **有符号整数溢出**：未定义行为，不同于无符号整数的模运算
3. **浮点精度问题**：十进制小数可能无法精确表示
4. **`sizeof(bool)` 不保证为 1**：序列化时需注意
5. **`wchar_t` 跨平台不一致**：大小和编码因平台而异

## 6. 代码示例 (Examples)

### 6.1 void 类型使用

```cpp
#include <iostream>

// void 作为返回类型
void print_message(const char* msg) {
    std::cout << msg << std::endl;
}

// void* 作为通用指针
void process_data(void* data, std::size_t size) {
    unsigned char* bytes = static_cast<unsigned char*>(data);
    for (std::size_t i = 0; i < size; ++i) {
        // 处理每个字节
    }
}

int main() {
    print_message("Hello, World!");

    int value = 42;
    process_data(&value, sizeof(value));

    return 0;
}
```

### 6.2 std::nullptr_t 使用

```cpp
#include <cstddef>
#include <iostream>

void f(int*) {
    std::cout << "调用了 f(int*)" << std::endl;
}

void f(double*) {
    std::cout << "调用了 f(double*)" << std::endl;
}

void f(std::nullptr_t) {
    std::cout << "调用了 f(std::nullptr_t)" << std::endl;
}

int main() {
    f(nullptr);       // 调用 f(std::nullptr_t)
    f(static_cast<int*>(nullptr));  // 调用 f(int*)

    std::cout << "sizeof(std::nullptr_t) = " << sizeof(std::nullptr_t) << std::endl;
    std::cout << "sizeof(void*) = " << sizeof(void*) << std::endl;

    return 0;
}
```

### 6.3 整数类型与数据模型

```cpp
#include <iostream>
#include <limits>

int main() {
    std::cout << "=== 数据类型大小 ===" << std::endl;
    std::cout << "sizeof(char): " << sizeof(char) << std::endl;
    std::cout << "sizeof(short): " << sizeof(short) << std::endl;
    std::cout << "sizeof(int): " << sizeof(int) << std::endl;
    std::cout << "sizeof(long): " << sizeof(long) << std::endl;
    std::cout << "sizeof(long long): " << sizeof(long long) << std::endl;
    std::cout << "sizeof(void*): " << sizeof(void*) << std::endl;

    std::cout << "\n=== 整数范围 ===" << std::endl;
    std::cout << "int: " << std::numeric_limits<int>::min()
              << " 到 " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "long long: " << std::numeric_limits<long long>::min()
              << " 到 " << std::numeric_limits<long long>::max() << std::endl;

    // 检测数据模型
    std::cout << "\n=== 数据模型检测 ===" << std::endl;
    if (sizeof(void*) == 8) {
        if (sizeof(long) == 8) {
            std::cout << "LP64 (Unix/Linux 64 位)" << std::endl;
        } else {
            std::cout << "LLP64 (Windows 64 位)" << std::endl;
        }
    } else {
        std::cout << "32 位系统" << std::endl;
    }

    return 0;
}
```

### 6.4 字符类型与编码

```cpp
#include <iostream>
#include <cuchar>  // C++11
#include <string>

int main() {
    // 基本字符类型
    char c = 'A';
    unsigned char uc = 255;
    signed char sc = -128;

    // C++11: UTF-16 和 UTF-32 字符
    char16_t utf16 = u'中';
    char32_t utf32 = U'文';

    // C++20: UTF-8 字符
    // char8_t utf8 = u8'A';  // C++20

    // 宽字符
    wchar_t wc = L'宽';

    std::cout << "char: " << c << std::endl;
    std::cout << "char16_t: " << static_cast<int>(utf16) << std::endl;
    std::cout << "char32_t: " << static_cast<int>(utf32) << std::endl;
    std::cout << "wchar_t 大小: " << sizeof(wchar_t) << " 字节" << std::endl;

    // 平台差异提示
    if (sizeof(wchar_t) == 2) {
        std::cout << "wchar_t = UTF-16 (Windows)" << std::endl;
    } else if (sizeof(wchar_t) == 4) {
        std::cout << "wchar_t = UTF-32 (Linux/macOS)" << std::endl;
    }

    return 0;
}
```

### 6.5 浮点类型与特殊值

```cpp
#include <iostream>
#include <cmath>
#include <limits>

int main() {
    // 浮点类型大小
    std::cout << "=== 浮点类型大小 ===" << std::endl;
    std::cout << "sizeof(float): " << sizeof(float) << std::endl;
    std::cout << "sizeof(double): " << sizeof(double) << std::endl;
    std::cout << "sizeof(long double): " << sizeof(long double) << std::endl;

    // 特殊值
    std::cout << "\n=== 特殊浮点值 ===" << std::endl;
    double pos_inf = INFINITY;
    double neg_zero = -0.0;
    double nan_val = NAN;

    std::cout << "正无穷: " << pos_inf << std::endl;
    std::cout << "负零: " << neg_zero << std::endl;
    std::cout << "NaN: " << nan_val << std::endl;

    // NaN 特性
    std::cout << "\n=== NaN 特性 ===" << std::endl;
    std::cout << "NaN == NaN: " << (nan_val == nan_val) << std::endl;  // false
    std::cout << "std::isnan(NaN): " << std::isnan(nan_val) << std::endl;

    // 负零特性
    std::cout << "\n=== 负零特性 ===" << std::endl;
    std::cout << "0.0 == -0.0: " << (0.0 == neg_zero) << std::endl;  // true
    std::cout << "1.0 / 0.0: " << 1.0 / 0.0 << std::endl;    // inf
    std::cout << "1.0 / -0.0: " << 1.0 / neg_zero << std::endl;  // -inf

    // 精度演示
    std::cout << "\n=== 精度演示 ===" << std::endl;
    double d = 0.1;
    std::cout.precision(17);
    std::cout << "0.1 的实际存储: " << d << std::endl;

    return 0;
}
```

### 6.6 布尔类型

```cpp
#include <iostream>
#include <vector>

int main() {
    bool flag = true;
    bool is_empty = false;

    std::cout << "sizeof(bool): " << sizeof(bool) << std::endl;
    std::cout << "true = " << true << ", false = " << false << std::endl;

    // 布尔在条件语句中
    if (flag) {
        std::cout << "flag 为真" << std::endl;
    }

    // 整数到布尔的隐式转换
    int value = 42;
    bool non_zero = value;  // 非零转换为 true
    std::cout << "42 转换为 bool: " << non_zero << std::endl;

    // 布尔向量（节省空间的特化）
    std::vector<bool> bits = {true, false, true, true, false};
    bits.push_back(true);
    std::cout << "vector<bool> 大小: " << bits.size() << std::endl;

    return 0;
}
```

### 6.7 常见错误及修正

#### 错误示例 1：有符号整数溢出

```cpp
// 错误：有符号整数溢出是未定义行为
#include <iostream>
#include <limits>

int main() {
    int x = std::numeric_limits<int>::max();
    int result = x + 1;  // 未定义行为！
    std::cout << result << std::endl;  // 结果不可预测
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <limits>
#include <stdexcept>

int safe_add(int a, int b) {
    if (b > 0 && a > std::numeric_limits<int>::max() - b) {
        throw std::overflow_error("整数上溢");
    }
    if (b < 0 && a < std::numeric_limits<int>::min() - b) {
        throw std::overflow_error("整数下溢");
    }
    return a + b;
}

int main() {
    try {
        int x = std::numeric_limits<int>::max();
        int result = safe_add(x, 1);
        std::cout << result << std::endl;
    } catch (const std::overflow_error& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    }
    return 0;
}
```

#### 错误示例 2：char 符号性陷阱

```cpp
// 错误：依赖 char 的符号性
#include <iostream>
#include <cctype>

int main() {
    char c = '\x80';  // 128 或 -128，取决于平台

    // 如果 char 是有符号的，c 可能是负数
    // 传给 isalpha 等函数是未定义行为
    if (std::isalpha(c)) {  // 未定义行为！
        std::cout << "是字母" << std::endl;
    }
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <cctype>

int main() {
    // 使用 unsigned char 避免符号扩展问题
    unsigned char c = '\x80';

    // 或显式转换为 unsigned char
    if (std::isalpha(static_cast<unsigned char>(c))) {
        std::cout << "是字母" << std::endl;
    } else {
        std::cout << "不是字母" << std::endl;
    }
    return 0;
}
```

#### 错误示例 3：浮点相等比较

```cpp
// 错误：直接比较浮点数
#include <iostream>

int main() {
    double a = 0.1 + 0.2;
    double b = 0.3;

    if (a == b) {  // 可能为假！
        std::cout << "相等" << std::endl;
    } else {
        std::cout << "不相等" << std::endl;  // 实际输出
    }
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <cmath>
#include <limits>

bool nearly_equal(double a, double b) {
    double diff = std::abs(a - b);
    double max_val = std::max(std::abs(a), std::abs(b));
    return diff <= max_val * std::numeric_limits<double>::epsilon();
}

int main() {
    double a = 0.1 + 0.2;
    double b = 0.3;

    if (nearly_equal(a, b)) {
        std::cout << "近似相等" << std::endl;
    } else {
        std::cout << "不相等" << std::endl;
    }
    return 0;
}
```

## 7. 总结 (Summary)

### 7.1 核心要点

1. **类型分类**：基本类型包括 `void`、`std::nullptr_t`、整数类型和浮点类型

2. **C++20 重要变更**：二进制补码成为有符号整数的唯一合法表示法

3. **字符类型多样性**：
   - `char`/`signed char`/`unsigned char` 是三种不同类型
   - C++11 起：`char16_t`、`char32_t`
   - C++20 起：`char8_t`

4. **平台差异性**：
   - 数据模型（LP64 vs LLP64）影响 `long` 和指针大小
   - `wchar_t` 大小因平台而异
   - `long double` 实现因编译器而异

### 7.2 类型大小速查

| 类型 | 最小保证 | 32 位系统典型值 | 64 位 Unix | 64 位 Windows |
|-----|---------|---------------|-----------|---------------|
| `char` | 8 位 | 8 位 | 8 位 | 8 位 |
| `short` | 16 位 | 16 位 | 16 位 | 16 位 |
| `int` | 16 位 | 32 位 | 32 位 | 32 位 |
| `long` | 32 位 | 32 位 | 64 位 | 32 位 |
| `long long` | 64 位 | 64 位 | 64 位 | 64 位 |
| 指针 | - | 32 位 | 64 位 | 64 位 |

### 7.3 与 C 语言的对比

| 特性 | C++ | C |
|-----|-----|---|
| `bool` | 原生关键字 | `_Bool` (C99)，需 `<stdbool.h>` |
| `nullptr` | 原生支持，有 `std::nullptr_t` | 宏 `NULL` |
| `char8_t` | C++20 原生类型 | C23 引入 |
| `long long` | C++11 | C99 |
| 二进制补码 | C++20 起强制 | C23 起强制 |

### 7.4 学习建议

1. **理解数据模型**：明确目标平台的类型大小配置

2. **掌握类型转换**：
   - 隐式转换规则
   - 整数提升
   - 浮点转换

3. **注意平台差异**：
   - `char` 的符号性
   - `wchar_t` 的大小
   - `long double` 的精度

4. **善用标准库**：
   - `<limits>` - `std::numeric_limits`
   - `<cstddef>` - `std::size_t`、`std::nullptr_t`
   - `<cstdint>` - 定宽整数类型

5. **避免未定义行为**：
   - 有符号整数溢出
   - 浮点相等比较
   - 类型双关（使用 `std::bit_cast` 或 `memcpy`）

---

**参考资源**：
- ISO/IEC 14882:2024 (C++23 标准)
- `<limits>` - 类型特性
- `<cstdint>` - 定宽整数类型
- `<cstddef>` - 基本类型定义