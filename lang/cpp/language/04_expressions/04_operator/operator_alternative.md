# 替代操作符表示 (Alternative Operator Representations)

## 1. 概述 (Overview)

替代操作符表示是 C++（及 C）语言为解决字符编码限制而提供的一组替代标记。C++ 源代码可以使用任何包含 ISO 646:1983 不变字符集的非 ASCII 7 位字符集编写。然而，多个 C++ 操作符和标点符号需要使用 ISO 646 字符集之外的字符：`{, }, [, ], #, \, ^, |, ~`。

为了能够在某些或全部这些符号不存在的字符编码（如德国 DIN 66003）下编写代码，C++ 定义了由 ISO 646 兼容字符组成的替代方案。

**技术定位**：语言特性 —— 词法分析阶段

**核心价值**：解决历史字符集编码的限制问题，使代码能在有限的字符编码环境下编写。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在计算机发展的早期阶段，不同国家和地区使用不同的字符编码标准。ISO 646 是一个 7 位字符编码标准，它将 ASCII 的某些字符位置留给了各国自定义使用。这导致在某些国家的字符集中，C++ 所需的一些特殊符号（如 `{, }, [, ], #` 等）可能不存在或被替换为其他字符。

### 设计动机

为了解决这一问题，C++ 标准委员会设计了替代标记机制：

1. **双字符组（Digraphs）**：由两个字符组成的替代标记
2. **三字符组（Trigraphs）**：由三个字符组成的替代标记（C++17 移除）
3. **关键字替代**：使用英文字母组成的替代标记

### 版本变更历史

| 版本 | 变更内容 |
|------|----------|
| C++98 | 引入三字符组和替代标记 |
| C++11 | 继续支持所有替代标记 |
| C++17 | **移除三字符组**（Trigraphs）|
| C++20 | 保持替代标记支持 |
| C++23 | 保持替代标记支持 |

### 三字符组移除原因

三字符组在 C++17 中被移除，主要原因包括：

1. **可读性问题**：三字符组会在注释和字符串中意外触发
2. **现代编码普及**：UTF-8 等现代编码标准已广泛使用
3. **编译复杂性**：早期词法处理增加了编译器复杂度
4. **实际使用率低**：现代开发环境很少需要此特性

## 3. 语法与参数 (Syntax and Parameters)

### 替代标记对照表

#### 操作符替代关键字

| 主标记 | 替代标记 | 说明 |
|--------|----------|------|
| `&&` | `and` | 逻辑与 |
| `&=` | `and_eq` | 位与赋值 |
| `&` | `bitand` | 位与 |
| `\|` | `bitor` | 位或 |
| `~` | `compl` | 位取反 |
| `!` | `not` | 逻辑非 |
| `!=` | `not_eq` | 不等于 |
| `\|\|` | `or` | 逻辑或 |
| `\|=` | `or_eq` | 位或赋值 |
| `^` | `xor` | 位异或 |
| `^=` | `xor_eq` | 位异或赋值 |

#### 标点符号替代

| 主标记 | 替代标记 | 说明 |
|--------|----------|------|
| `{` | `<%` | 左花括号 |
| `}` | `%>` | 右花括号 |
| `[` | `<:` | 左方括号 |
| `]` | `:>` | 右方括号 |
| `#` | `%:` | 预处理指令符 |
| `##` | `%:%:` | 宏连接符 |

### 三字符组（已移除）

以下三字符组在 C++17 前有效：

| 主标记 | 三字符组 | 说明 |
|--------|----------|------|
| `{` | `??<` | 左花括号 |
| `}` | `??>` | 右花括号 |
| `[` | `??(` | 左方括号 |
| `]` | `??)` | 右方括号 |
| `#` | `??=` | 预处理指令符 |
| `\` | `??/` | 反斜杠 |
| `^` | `??'` | 位异或 |
| `\|` | `??!` | 位或 |
| `~` | `??-` | 位取反 |

### 关键字列表

以下替代标记是 C++ 关键字：

```cpp
and, and_eq, bitand, bitor, compl,
not, not_eq, or, or_eq, xor, xor_eq
```

## 4. 底层原理 (Underlying Principles)

### 词法分析阶段

替代标记的处理发生在编译的**词法分析（Lexical Analysis）阶段**：

1. **三字符组处理**（C++17 前）：在识别注释和字符串字面量之前
2. **双字符组和关键字替代处理**：作为词法单元（Token）的一部分

### 等价性规则

在语言的各个方面，每个替代标记的行为与其主标记**完全相同**，除了拼写不同外：

```cpp
// 以下两行完全等价
if (a and b)    // 使用替代标记
if (a && b)     // 使用主标记
```

**例外情况**：字符串化操作符（`#`）可以显示拼写差异：

```cpp
#define STRINGIFY(x) #x

STRINGIFY(and)    // 结果: "and"
STRINGIFY(&&)     // 结果: "&&"
```

### 三字符组的早期处理

三字符组在注释和字符串字面量识别**之前**被替换，这导致了意外的行为：

```cpp
// 注释示例：下一行会被执行吗?????/
// 实际效果：上面的注释会延伸到下一行

const char* s = "输入日期??/??/??";
// 实际解析为: "输入日期\\??"
```

### 编码兼容性设计

替代标记的设计原则：

1. **仅使用 ISO 646 不变字符**：确保在所有受支持的字符集中可用
2. **语义完全等价**：不引入新的语义
3. **向后兼容**：不影响现有代码

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 历史代码维护

某些遗留系统可能使用替代标记编写，维护时需要理解这些标记：

```cpp
// 历史代码可能这样写
%:include <iostream>
int main() <%
    std::cout << "Hello World\n";
%>
```

#### 2. 有限字符集环境

在某些极其受限的嵌入式系统或特殊终端环境中：

```cpp
// 当键盘或终端不支持某些符号时
if (a not_eq b and c or_eq d) <%
    // 代码逻辑
%>
```

#### 3. 代码混淆（不推荐）

```cpp
// 使用替代标记可能让代码更难阅读
auto result = a bitand b xor compl c;
// 等价于: auto result = a & b ^ ~c;
```

### 最佳实践

#### 推荐做法

1. **现代代码中使用主标记**：现代开发环境完全支持标准 ASCII 字符

```cpp
// 推荐：清晰易懂
if (a && b || c != d) {
    // ...
}
```

2. **理解但避免使用**：了解替代标记以便阅读遗留代码

```cpp
// 不推荐：可读性差
if (a and b or c not_eq d) <%
    // ...
%>
```

#### C 语言兼容性

在 C 语言中，替代关键字定义在 `<iso646.h>` 头文件中：

```cpp
// C 语言
#include <iso646.h>
if (a and b) { /* ... */ }

// C++ 语言
// <iso646.h> 和 <ciso646> 不定义任何内容
// 替代关键字是语言内置的
if (a and b) { /* ... */ }
```

### 常见陷阱

#### 陷阱 1：三字符组的意外替换（C++17 前）

```cpp
// 错误示例：三字符组在字符串中被替换
const char* path = "C:\??\temp";  // 可能被错误解析
// 建议使用原始字符串或转义
const char* path = R"(C:\??\temp)";
```

#### 陷阱 2：混淆替代标记和普通标识符

```cpp
// 错误：and 是关键字，不能作为变量名
int and = 5;  // 编译错误

// 正确：使用不同的标识符
int result_and = 5;
```

#### 陷阱 3：字符串化操作符的差异

```cpp
#include <iostream>

#define STRINGIFY(x) #x

int main() {
    std::cout << STRINGIFY(and) << '\n';   // 输出: and
    std::cout << STRINGIFY(&&) << '\n';    // 输出: &&
    // 两者不等价！
}
```

## 6. 代码示例 (Examples)

### 基础用法示例

以下示例展示多种替代标记的使用：

```cpp
%:include <iostream>

struct X
<%
    compl X() <%%> // 析构函数: ~X() {}
    X() <%%>
    X(const X bitand) = delete; // 复制构造函数: const X&
    // X(X and) = delete; // 移动构造函数: X&& (不推荐这样注释)

    bool operator not_eq(const X bitand other)
    <%
       return this not_eq bitand other;
    %>
%>;

int main(int argc, char* argv<::>)
<%
    // lambda with reference-capture:
    auto greet = <:bitand:>(const char* name)
    <%
        std::cout << "Hello " << name
                  << " from " << argv<:0:> << '\n';
    %>;

    if (argc > 1 and argv<:1:> not_eq nullptr)
        greet(argv<:1:>);
    else
        greet("Anon");
%>
```

**等价的标准代码**：

```cpp
#include <iostream>

struct X
{
    ~X() {} // 析构函数
    X() {}
    X(const X&) = delete; // 复制构造函数
    // X(X&&) = delete; // 移动构造函数

    bool operator!=(const X& other)
    {
       return this != &other;
    }
};

int main(int argc, char* argv[])
{
    // lambda with reference-capture:
    auto greet = [&](const char* name)
    {
        std::cout << "Hello " << name
                  << " from " << argv[0] << '\n';
    };

    if (argc > 1 && argv[1] != nullptr)
        greet(argv[1]);
    else
        greet("Anon");
}
```

### 逻辑操作符替代示例

```cpp
#include <iostream>

int main() {
    bool a = true;
    bool b = false;

    // 使用替代标记
    if (a and b) {
        std::cout << "a and b is true\n";
    }

    if (a or b) {
        std::cout << "a or b is true\n";
    }

    if (not a) {
        std::cout << "not a is true\n";
    }

    // 等价的主标记
    if (a && b) {
        std::cout << "a && b is true\n";
    }

    if (a || b) {
        std::cout << "a || b is true\n";
    }

    if (!a) {
        std::cout << "!a is true\n";
    }

    return 0;
}
```

### 位操作符替代示例

```cpp
#include <iostream>
#include <bitset>

int main() {
    unsigned int a = 0b1100;
    unsigned int b = 0b1010;

    // 使用替代标记
    std::cout << "a bitand b = " << std::bitset<4>(a bitand b) << '\n';
    std::cout << "a bitor b = " << std::bitset<4>(a bitor b) << '\n';
    std::cout << "a xor b = " << std::bitset<4>(a xor b) << '\n';
    std::cout << "compl a = " << std::bitset<4>(compl a) << '\n';

    // 等价的主标记
    std::cout << "a & b = " << std::bitset<4>(a & b) << '\n';
    std::cout << "a | b = " << std::bitset<4>(a | b) << '\n';
    std::cout << "a ^ b = " << std::bitset<4>(a ^ b) << '\n';
    std::cout << "~a = " << std::bitset<4>(~a) << '\n';

    return 0;
}
```

### 预处理指令替代示例

```cpp
%:include <iostream>
%:define DEBUG 1

%:if DEBUG
    %:define LOG(msg) std::cout << msg << '\n'
%:else
    %:define LOG(msg)
%:endif

int main() <%
    LOG("Debug mode enabled");
    return 0;
%>
```

**等价的标准代码**：

```cpp
#include <iostream>
#define DEBUG 1

#if DEBUG
    #define LOG(msg) std::cout << msg << '\n'
#else
    #define LOG(msg)
#endif

int main() {
    LOG("Debug mode enabled");
    return 0;
}
```

### 常见错误及修正

#### 错误 1：在现代代码中使用三字符组（C++17 移除）

```cpp
// 错误（C++17）：三字符组已移除
// ??=define FOO 1  // 意图：#define FOO 1

// 正确：使用主标记
#define FOO 1
```

#### 错误 2：替代标记作为标识符

```cpp
// 错误：and 是关键字
int and = 10;  // 编译错误

// 正确：使用其他名称
int and_value = 10;
```

#### 错误 3：宏定义中的混淆

```cpp
// 错误示例：宏参数名使用替代关键字
#define MUL(and, b) ((and) * (b))  // 错误！
// 正确：
#define MUL(a, b) ((a) * (b))
```

## 7. 总结 (Summary)

### 核心要点

1. **替代标记本质**：提供使用 ISO 646 兼容字符编写 C++ 代码的能力
2. **两种类型**：
   - 双字符组和关键字替代（仍然有效）
   - 三字符组（C++17 已移除）
3. **完全等价**：替代标记与主标记在语义上完全相同
4. **C/C++ 差异**：C 语言需要包含 `<iso646.h>`，C++ 内置支持

### 技术对比

| 特性 | 双字符组 | 三字符组 | 关键字替代 |
|------|----------|----------|------------|
| 引入版本 | C++98 | C++98 | C++98 |
| 当前状态 | 支持 | **C++17 移除** | 支持 |
| 处理阶段 | 词法分析 | 早期预处理 | 词法分析 |
| 典型用途 | 标点替代 | 字符集兼容 | 操作符替代 |
| 推荐程度 | 不推荐 | **禁用** | 不推荐 |

### 学习建议

1. **了解但避免使用**：理解替代标记以便维护遗留代码，但现代代码应使用主标记
2. **注意 C++17 变更**：如果需要维护旧代码，了解三字符组的行为
3. **关注字符串化**：理解 `#` 操作符对替代标记的影响
4. **跨平台开发**：在跨平台或国际团队中，确保使用标准 ASCII 字符

### 参考资源

- C++23 标准 (ISO/IEC 14882:2024)：5.5 Alternative tokens [lex.digraph]
- C++20 标准 (ISO/IEC 14882:2020)：5.5 Alternative tokens [lex.digraph]
- C++17 标准 (ISO/IEC 14882:2017)：5.5 Alternative tokens [lex.digraph]
- C++14 标准 (ISO/IEC 14882:2014)：2.6 Alternative tokens [lex.digraph]