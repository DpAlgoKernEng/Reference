# 未定义行为 (Undefined Behavior)

## 1. 概述 (Overview)

如果违反了语言的某些规则，**未定义行为** 会使整个程序失去意义。

### C++ 程序行为分类

C++ 标准精确定义了不属于以下类别的每个 C++ 程序的可观察行为：

| 行为类别 | 英文名称 | 说明 |
|---------|---------|------|
| **非良构** | Ill-formed | 语法错误或可诊断的语义错误 |
| **非良构，无需诊断** | Ill-formed, no diagnostic required | 语义错误，但通常不可诊断 |
| **实现定义行为** | Implementation-defined behavior | 行为因实现而异，必须文档说明 |
| **未指定行为** | Unspecified behavior | 行为因实现而异，无需文档说明 |
| **错误行为** | Erroneous behavior | C++26 起，推荐诊断的错误行为 |
| **未定义行为** | Undefined behavior | 程序行为无任何限制 |
| **运行时未定义行为** | Runtime-undefined behavior | C++11 起，常量表达式求值时除外 |

### 良构程序

**良构程序 (Well-formed program)** 是不违反语法规则、可诊断语义规则和单一定义规则 (ODR) 的程序。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

未定义行为是 C++ 从 C 语言继承的核心概念，设计动机包括：

1. **性能优先**：避免运行时检查，允许激进优化
2. **可移植性**：不同平台可有不同实现
3. **硬件差异**：不同处理器架构行为不同

### 版本演进

| C++ 标准版本 | 发布年份 | 相关改进 |
|-------------|---------|---------|
| C++98 | 1998 | 确立行为分类框架 |
| C++11 | 2011 | 添加运行时未定义行为概念 |
| C++23 | 2023 | 添加 `[[assume]]` 属性、`std::unreachable` |
| C++26 | 2026 | 添加错误行为 (Erroneous behavior)、`[[indeterminate]]` 属性 |

### C++26 新概念：错误行为

C++26 引入了**错误行为 (Erroneous behavior)**，这是介于未定义行为和良定义行为之间的新类别：

| 特性 | 说明 |
|-----|------|
| 定义 | 实现推荐诊断的错误行为 |
| 诊断 | 允许且推荐发出诊断 |
| 终止 | 允许在操作后不确定时间终止执行 |
| 常量表达式 | 求值永不产生错误行为 |

```cpp
// C++26 错误行为示例
void f() {
    int d1, d2;       // d1, d2 有错误值
    int e1 = d1;      // 错误行为
    int e2 = d1;      // 错误行为
    assert(e1 == e2); // 成立
}

unsigned char g(bool b) {
    unsigned char c;     // c 有错误值
    unsigned char d = c; // 无错误行为，但 d 有错误值
    int e = d;           // 错误行为
    return b ? d : 0;    // 若 b 为真则是错误行为
}
```

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 相关属性（C++23 起）

```cpp
// [[assume]] 属性：指定表达式在给定点总是为真
void process(int* p) {
    [[assume(p != nullptr)]];  // 假设 p 非空
    // ... 使用 p
}

// std::unreachable 函数：标记不可达执行点
#include <utility>

void handle(int x) {
    switch (x) {
        case 0: /* ... */ break;
        case 1: /* ... */ break;
        default: std::unreachable();  // 标记不可达
    }
}
```

### 3.2 相关属性（C++26 起）

```cpp
// [[indeterminate]] 属性：指定对象有不确定值
#include <new>

void process() {
    int x [[indeterminate]];  // 显式标记为不确定值
    // 读取 x 是错误行为而非 UB
}
```

### 3.3 编译器诊断要求

| 程序类别 | 诊断要求 |
|---------|---------|
| 非良构 | 必须诊断 |
| 非良构，无需诊断 | 不要求诊断 |
| UB 情况 | 不要求诊断 |

标准文本使用 **shall**、**shall not** 和 **ill-formed** 来表示这些要求。

## 4. 底层原理 (Underlying Principles)

### 4.1 为什么需要未定义行为

```
C++ 设计目标
      ↓
性能 + 可移植性
      ↓
避免运行时检查
      ↓
允许编译器假设程序正确
      ↓
实现激进优化
```

### 4.2 UB 与优化的关系

**核心原则**：因为正确的 C++ 程序不存在未定义行为，编译器可以在启用优化时对实际存在 UB 的程序产生意外结果。

编译器可以**假设未定义行为不会发生**，并基于此假设进行优化。

### 4.3 UB 优化示例

#### 有符号溢出

```cpp
int foo(int x) {
    return x + 1 > x;  // 要么为真，要么因溢出而 UB
}

// 编译器优化后：
foo(int):
    mov     eax, 1
    ret
```

**优化逻辑**：假设 `x + 1` 不会溢出（溢出是 UB），则 `x + 1 > x` 恒为真。

#### 数组越界访问

```cpp
int table[4] = {};

bool exists_in_table(int v) {
    // 前 4 次迭代返回 true，或因越界访问而 UB
    for (int i = 0; i <= 4; i++)
        if (table[i] == v)
            return true;
    return false;
}

// 编译器优化后：
exists_in_table(int):
    mov     eax, 1
    ret
```

**优化逻辑**：假设循环不会越界，则循环必然在某次迭代中找到 `v`（否则越界是 UB 可忽略）。

#### 未初始化标量

```cpp
#include <cstdio>

int main() {
    bool p;  // 未初始化的局部变量
    if (p)   // UB：访问未初始化标量
        std::puts("p is true");
    if (!p)  // UB：访问未初始化标量
        std::puts("p is false");
}

// 可能输出：
// p is true
// p is false
```

```cpp
std::size_t f(int x) {
    std::size_t a;
    if (x)  // 要么 x 非零，要么 UB
        a = 42;
    return a;
}

// 编译器优化后：
f(int):
    mov     eax, 42
    ret
```

#### 无效标量值

```cpp
int f() {
    bool b = true;
    unsigned char* p = reinterpret_cast<unsigned char*>(&b);
    *p = 10;  // 设置为无效值
    // 读取 b 现在是 UB
    return b == 0;
}

// 编译器优化后：
f():
    mov     eax, 11  // 返回 11 而非 0 或 1！
    ret
```

#### 空指针解引用

```cpp
int foo(int* p) {
    int x = *p;        // 如果 p 是 nullptr，这里 UB
    if (!p)
        return x;      // 要么上面已 UB，要么此分支永不执行
    else
        return 0;
}

int bar() {
    int* p = nullptr;
    return *p;         // 无条件 UB
}

// 编译器优化后：
foo(int*):
    xor     eax, eax
    ret

bar():
    ret                // 可能完全删除！
```

#### realloc 后访问原指针

```cpp
#include <cstdlib>
#include <iostream>

int main() {
    int* p = (int*)std::malloc(sizeof(int));
    int* q = (int*)std::realloc(p, sizeof(int));
    *p = 1;  // UB：访问传递给 realloc 的指针
    *q = 2;
    if (p == q)  // UB：访问传递给 realloc 的指针
        std::cout << *p << *q << '\n';
}

// 可能输出：12
```

#### 无副作用无限循环

```cpp
#include <iostream>

bool fermat() {
    const int max_value = 1000;

    // 无副作用的无限循环是 UB
    for (int a = 1, b = 1, c = 1; true; ) {
        if (((a*a*a) == ((b*b*b) + (c*c*c))))
            return true;
        a++;
        if (a > max_value) { a = 1; b++; }
        if (b > max_value) { b = 1; c++; }
        if (c > max_value) c = 1;
    }

    return false;
}

int main() {
    std::cout << "Fermat's Last Theorem ";
    fermat()
        ? std::cout << "has been disproved!\n"
        : std::cout << "has not been disproved.\n";
}

// 可能输出：Fermat's Last Theorem has been disproved!
// （因为无限循环是 UB，编译器可以假设它不发生）
```

### 4.4 非良构程序的编译器处理

编译器**允许**以赋予非良构程序意义的方式扩展语言。唯一要求是发出诊断消息（编译器警告），除非程序是"非良构，无需诊断"。

```cpp
// GCC 示例：即使 --pedantic-errors 未启用
// 也只会发出警告（语言扩展允许变长数组）

#include <iostream>

double a{1.0};

struct S {
    S(int, double, double);
    S();
};

S s1 = {1, 2, 3.0};  // OK
S s2{a, 2, 3};       // narrowing 转换

// GCC 输出：
// main.cpp:17:6: error: type 'double' cannot be narrowed to 'int'
// in initializer list [-Wc++11-narrowing]
```

## 5. 使用场景 (Use Cases)

### 5.1 避免 UB 的策略

| 策略 | 工具/方法 |
|-----|----------|
| 静态分析 | Clang-Tidy, PVS-Studio, cppcheck |
| 编译器警告 | `-Wall -Wextra -Wpedantic` |
| 运行时检测 | AddressSanitizer, UBSan, Valgrind |
| 代码审查 | 关注常见 UB 模式 |
| 现代 C++ 特性 | `std::span`, `std::optional`, `std::array` |

### 5.2 常见 UB 清单

| 类别 | UB 情况 |
|-----|--------|
| **内存访问** | 数组越界、空指针解引用、悬空指针、未对齐访问 |
| **整数操作** | 有符号溢出、除以零、移位超出范围 |
| **指针操作** | 无效指针比较、realloc 后访问原指针 |
| **表达式求值** | 无序列点多次修改同一标量 |
| **类型系统** | 严格别名违规、访问无效标量值 |
| **对象生命周期** | 访问未初始化变量、访问已释放内存 |
| **控制流** | 无副作用的无限循环、非 void 函数缺少返回 |
| **并发** | 数据竞争 |

### 5.3 使用 [[assume]] 优化（C++23）

```cpp
#include <vector>
#include <algorithm>

void process(std::vector<int>& v, int start, int count) {
    // 假设条件满足，允许编译器优化
    [[assume(start >= 0)]];
    [[assume(count > 0)]];
    [[assume(start + count <= static_cast<int>(v.size()))]];

    for (int i = start; i < start + count; ++i) {
        v[i] *= 2;
    }
}
```

### 5.4 注意事项

1. **不要依赖 UB 进行检测**
2. **不同优化级别行为可能不同**
3. **不同编译器行为可能不同**
4. **Sanitizer 有运行时开销，仅用于调试**
5. **C++26 的错误行为提供了更安全的选择**

## 6. 代码示例 (Examples)

### 6.1 安全的边界检查

```cpp
#include <vector>
#include <optional>
#include <iostream>

std::optional<int> safe_access(const std::vector<int>& v, std::size_t index) {
    if (index < v.size()) {
        return v[index];
    }
    return std::nullopt;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    if (auto val = safe_access(v, 3)) {
        std::cout << "v[3] = " << *val << '\n';
    }

    if (auto val = safe_access(v, 10)) {
        std::cout << "v[10] = " << *val << '\n';
    } else {
        std::cout << "索引越界\n";
    }

    return 0;
}
```

### 6.2 安全的整数运算

```cpp
#include <iostream>
#include <limits>
#include <optional>

std::optional<int> safe_add(int a, int b) {
    if ((b > 0 && a > std::numeric_limits<int>::max() - b) ||
        (b < 0 && a < std::numeric_limits<int>::min() - b)) {
        return std::nullopt;
    }
    return a + b;
}

int main() {
    if (auto result = safe_add(100, 200)) {
        std::cout << "100 + 200 = " << *result << '\n';
    }

    if (auto result = safe_add(std::numeric_limits<int>::max(), 1)) {
        std::cout << "结果: " << *result << '\n';
    } else {
        std::cout << "溢出！\n";
    }

    return 0;
}
```

### 6.3 使用 std::span 避免越界（C++20）

```cpp
#include <span>
#include <vector>
#include <iostream>

void process(std::span<int> data) {
    for (auto& elem : data) {
        elem *= 2;
    }
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // 安全：span 自动跟踪大小
    process(v);

    for (int i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

### 6.4 使用 std::unreachable（C++23）

```cpp
#include <utility>
#include <iostream>

enum class Color { Red, Green, Blue };

std::string_view color_name(Color c) {
    switch (c) {
        case Color::Red:   return "Red";
        case Color::Green: return "Green";
        case Color::Blue:  return "Blue";
    }
    std::unreachable();  // 告诉编译器此处不可达
}

int main() {
    std::cout << color_name(Color::Red) << '\n';
    std::cout << color_name(Color::Blue) << '\n';
    return 0;
}
```

### 6.5 常见错误及修正

#### 错误示例 1：有符号溢出检测

```cpp
// 错误：依赖溢出行为
#include <iostream>
#include <limits>

int main() {
    int x = std::numeric_limits<int>::max();

    if (x + 1 < x) {  // UB！编译器可能删除此检查
        std::cout << "溢出\n";
    }

    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <limits>

int main() {
    int x = std::numeric_limits<int>::max();

    // 正确：运算前检查
    if (x > std::numeric_limits<int>::max() - 1) {
        std::cout << "溢出\n";
    }

    // 或使用无符号整数
    unsigned int ux = std::numeric_limits<unsigned int>::max();
    std::cout << "无符号溢出: " << (ux + 1) << '\n';  // 定义为 0

    return 0;
}
```

#### 错误示例 2：未初始化变量

```cpp
// 错误：使用未初始化变量
#include <iostream>

int main() {
    int x;  // 未初始化
    std::cout << "x = " << x << '\n';  // UB!

    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>

int main() {
    int x{};  // 值初始化为 0
    std::cout << "x = " << x << '\n';

    // 或使用 std::optional 表示可能无值
    return 0;
}
```

#### 错误示例 3：空指针解引用

```cpp
// 错误：未检查空指针
#include <iostream>

int get_value(int* p) {
    return *p;  // 如果 p 是 nullptr 则 UB
}

int main() {
    int* p = nullptr;
    std::cout << get_value(p) << '\n';  // UB!

    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <optional>

std::optional<int> get_value(int* p) {
    if (p == nullptr) {
        return std::nullopt;
    }
    return *p;
}

int main() {
    int* p = nullptr;

    if (auto val = get_value(p)) {
        std::cout << *val << '\n';
    } else {
        std::cout << "无效指针\n";
    }

    return 0;
}
```

#### 错误示例 4：容器越界

```cpp
// 错误：容器越界访问
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3};

    // UB：越界访问
    for (std::size_t i = 0; i <= v.size(); ++i) {
        std::cout << v[i] << '\n';
    }

    return 0;
}
```

**正确做法**：

```cpp
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3};

    // 正确：使用范围 for
    for (int x : v) {
        std::cout << x << '\n';
    }

    // 或正确边界检查
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v.at(i) << '\n';  // at() 会检查边界
    }

    return 0;
}
```

## 7. 总结 (Summary)

### 7.1 核心要点

1. **UB 定义**：程序行为无任何限制，可能产生任何结果

2. **编译器假设**：编译器假设 UB 不会发生，并基于此优化

3. **优化影响**：
   - UB 代码可能被完全删除
   - 检测 UB 的代码可能被优化掉
   - 不同优化级别行为可能不同

4. **C++26 改进**：
   - 错误行为提供更安全的替代
   - `[[indeterminate]]` 显式标记不确定值

### 7.2 C++ 与 C 的 UB 对比

| 特性 | C++ | C |
|-----|-----|---|
| 基本概念 | 相同 | 相同 |
| 运行时 UB | C++11 起有区分 | 无 |
| 错误行为 | C++26 新增 | 无 |
| `[[assume]]` | C++23 | 无 |
| `[[indeterminate]]` | C++26 | 无 |

### 7.3 调试工具推荐

| 工具 | 类型 | 用途 |
|-----|------|------|
| `-Wall -Wextra` | 编译器警告 | 基本问题检测 |
| AddressSanitizer | 运行时检测 | 内存错误 |
| UBSan | 运行时检测 | 未定义行为 |
| Valgrind | 动态分析 | 内存泄漏、越界 |
| Clang-Tidy | 静态分析 | 代码质量 |
| PVS-Studio | 静态分析 | 深度分析 |

### 7.4 学习建议

1. **理解 UB 本质**：
   - UB 不是"随机行为"
   - 编译器会基于"UB 不发生"假设优化

2. **使用现代 C++**：
   - `std::span` 替代原始指针+大小
   - `std::optional` 表示可选值
   - `std::array` 替代 C 数组
   - 容器的 `at()` 方法检查边界

3. **使用工具**：
   - 开发时启用所有警告
   - 测试时使用 sanitizer
   - 发布前使用静态分析

4. **关注 C++26 改进**：
   - 错误行为提供诊断机会
   - `[[indeterminate]]` 显式处理未初始化值

---

**参考资源**：
- ISO/IEC 14882:2024 (C++23 标准)
- "What Every C Programmer Should Know About Undefined Behavior" 系列
- C++26 草案 N4950
- `[[assume]]` 属性、`std::unreachable` 函数