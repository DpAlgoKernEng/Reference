# C++ 文本宏替换 (Replacing Text Macros)

## 1. 概述 (Overview)

文本宏替换（Text Macro Replacement）是 C++ 预处理器的核心功能，允许定义标识符为宏，使其在预处理阶段被替换为指定的替换列表。C++ 支持两种类型的宏：

- **对象式宏（Object-like Macros）**：简单的标识符替换
- **函数式宏（Function-like Macros）**：带参数的宏，支持参数替换和可变参数

C++11 引入了可变参数宏，C++20 引入了 `__VA_OPT__` 运算符用于条件性处理可变参数，C++20 还引入了语言特性检测宏系统。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

宏替换机制继承自 C 语言，是 C++ 元编程能力的基础。宏在预处理阶段工作，不进行类型检查，提供了强大的文本替换能力。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 基础宏定义：对象式宏、函数式宏、`#` 和 `##` 运算符 |
| C++11 | 可变参数宏、`__VA_ARGS__`、`__func__`、`__STDC_HOSTED__`、保留宏名称规则 |
| C++14 | 无重大变更 |
| C++17 | `__STDCPP_DEFAULT_NEW_ALIGNMENT__` 宏 |
| C++20 | `__VA_OPT__` 运算符、语言特性检测宏系统、`likely`/`unlikely` 可定义为函数式宏 |
| C++23 | 扩展浮点类型宏（`__STDCPP_FLOAT16_T__` 等）、允许通用字符名通过标记拼接形成 |

### 设计动机

宏替换解决了以下核心问题：

1. **常量定义**：定义编译时常量，避免魔法数字
2. **代码复用**：通过函数式宏简化重复代码
3. **条件编译**：配合 `#ifdef` 实现平台适配
4. **调试支持**：通过 `__FILE__`、`__LINE__` 等宏提供诊断信息
5. **元编程**：通过 `#` 和 `##` 运算符实现代码生成
6. **特性检测**：通过特性检测宏判断编译器支持的功能

## 3. 语法与参数 (Syntax and Parameters)

### `#define` 指令语法

| 语法形式 | 说明 | 版本 |
|----------|------|------|
| `#define 标识符 替换列表(可选)` | (1) 对象式宏 | C++98 |
| `#define 标识符(参数) 替换列表(可选)` | (2) 函数式宏 | C++98 |
| `#define 标识符(参数, ...) 替换列表(可选)` | (3) 可变参数函数式宏 | C++11 |
| `#define 标识符(...) 替换列表(可选)` | (4) 纯可变参数函数式宏 | C++11 |

### `#undef` 指令语法

```cpp
#undef 标识符
```

取消标识符的宏定义。如果标识符未定义为宏，则忽略该指令。

### 对象式宏

对象式宏将标识符简单替换为替换列表：

```cpp
#define PI 3.14159
#define MAX_SIZE 100
#define DEBUG
```

### 函数式宏

函数式宏支持参数替换：

```cpp
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))
```

**调用语法**：宏名后紧跟 `(`，参数列表由匹配的 `)` 终止，跳过中间匹配的括号对。

**参数要求**：
- 版本 (2)：参数数量必须与定义一致
- 版本 (3,4)：参数数量不能少于参数列表中的参数数量（C++20 起不计算 `...`）
- 如果标识符后没有括号，则不被替换

### 可变参数宏（C++11）

使用 `__VA_ARGS__` 访问额外参数：

```cpp
#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)
#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
```

### `__VA_OPT__` 运算符（C++20）

```cpp
__VA_OPT__(内容)
```

- 如果 `__VA_ARGS__` 非空，替换为 `内容`
- 如果 `__VA_ARGS__` 为空，替换为空

```cpp
#define F(...) f(0 __VA_OPT__(,) __VA_ARGS__)
F(a, b, c) // 替换为 f(0, a, b, c)
F()        // 替换为 f(0)
```

### `#` 字符串化运算符

将参数转换为字符串字面量：

```cpp
#define STRINGIFY(x) #x
STRINGIFY(hello)  // 替换为 "hello"
```

**转换规则**：
- 参数替换后用引号包围
- 嵌入的字符串字面量会被转义
- 连续空白符压缩为单个空格
- 前导和尾随空白符移除

### `##` 标记拼接运算符

拼接两个标记：

```cpp
#define CONCAT(a, b) a##b
CONCAT(var, 1)  // 替换为 var1
```

**限制**：只能拼接形成有效标记的标记对。C++23 起允许通过拼接形成通用字符名。

### 预定义宏

#### 必须预定义的宏

| 宏名 | 说明 | 版本 |
|------|------|------|
| `__cplusplus` | C++ 标准版本号 | C++98 |
| `__STDC_HOSTED__` | 宿主实现为 `1`，独立实现为 `0` | C++11 |
| `__FILE__` | 当前文件名（字符串字面量） | C++98 |
| `__LINE__` | 当前行号（整数常量） | C++98 |
| `__DATE__` | 翻译日期，格式 `"Mmm dd yyyy"` | C++98 |
| `__TIME__` | 翻译时间，格式 `"hh:mm:ss"` | C++98 |
| `__STDCPP_DEFAULT_NEW_ALIGNMENT__` | `operator new` 保证的对齐值 | C++17 |
| `__STDCPP_FLOAT16_T__` | 支持时展开为 `1` | C++23 |
| `__STDCPP_FLOAT32_T__` | 支持时展开为 `1` | C++23 |
| `__STDCPP_FLOAT64_T__` | 支持时展开为 `1` | C++23 |
| `__STDCPP_FLOAT128_T__` | 支持时展开为 `1` | C++23 |
| `__STDCPP_BFLOAT16_T__` | 支持时展开为 `1` | C++23 |

#### `__cplusplus` 值

| 版本 | 值 |
|------|-----|
| C++98 | `199711L` |
| C++11 | `201103L` |
| C++14 | `201402L` |
| C++17 | `201703L` |
| C++20 | `202002L` |
| C++23 | `202302L` |

#### 实现可选预定义的宏

| 宏名 | 说明 | 版本 |
|------|------|------|
| `__STDC__` | 实现定义值，通常指示 C 一致性 | C++98 |
| `__STDC_VERSION__` | 实现定义值 | C++11 |
| `__STDC_ISO_10646__` | `wchar_t` 使用 Unicode（C++23 前） | C++11 |
| `__STDC_MB_MIGHT_NEQ_WC__` | `'x' == L'x'` 可能为假 | C++11 |
| `__STDCPP_THREADS__` | 程序可以有多线程 | C++11 |
| `__STDCPP_STRICT_POINTER_SAFETY__` | 严格指针安全性（C++23 移除） | C++11 |

### 语言特性检测宏（C++20）

C++20 引入了一组预处理器宏，用于检测 C++11 及以后版本引入的语言特性。详见特性检测文档。

### 保留宏名称

包含标准库头文件的翻译单元不能 `#define` 或 `#undef` 标准库中声明的名称。

使用标准库的翻译单元不能 `#define` 或 `#undef` 以下名称：
- 关键字
- 具有特殊含义的标识符
- 标准属性标记（但 C++20 起 `likely` 和 `unlikely` 可定义为函数式宏）

## 4. 底层原理 (Underlying Principles)

### 扫描与替换流程

```
源代码
    ↓
扫描标识符
    ↓
发现宏定义？
    ↓
标记为"忽略"（防止递归）
    ↓
参数扫描（函数式宏）
    ↓
参数替换（`#` 和 `##` 操作数不扫描）
    ↓
替换列表展开
    ↓
结果继续扫描
    ↓
替换完成
```

### 防止递归

扫描过程跟踪已替换的宏。如果发现匹配的文本，标记为"忽略"，所有后续扫描都会跳过它。这防止了无限递归。

### 伪递归宏

通过间接方式可以实现伪递归宏：

```cpp
#define EMPTY
#define SCAN(x)     x
#define EXAMPLE_()  EXAMPLE
#define EXAMPLE(n)  EXAMPLE_ EMPTY()(n-1) (n)

EXAMPLE(5)
// 输出: EXAMPLE_ ()(5 -1) (5)

SCAN(EXAMPLE(5))
// 输出: EXAMPLE_ ()(5 -1 -1) (5 -1) (5)
```

**原理**：通过延迟展开和中间宏，在每次替换后重新进入扫描，实现类似递归的效果。

### 参数扫描规则

- 函数式宏的参数在被放入替换列表**之前**先扫描
- `#` 和 `##` 运算符的操作数**不**被扫描

### 字符串化（`#`）处理

```cpp
#define STR(x) #x
STR(hello world)  // → "hello world"

// 嵌入字符串转义
#define STR2(x) #x
STR2("hello")     // → "\"hello\""

// __VA_ARGS__ 字符串化
#define SHOW(...) #__VA_ARGS__
SHOW(1, "x", int) // → "1, \"x\", int"
```

### 标记拼接（`##`）处理

```cpp
#define CONCAT(a, b) a##b
CONCAT(func, _name)  // → func_name

// 有效拼接示例
// 标识符 + 标识符 = 更长的标识符
// 数字 + 数字 = 数字
// 运算符 + 运算符 = 复合运算符（如 + 和 = 变成 +=）
```

### `#` 和 `##` 求值顺序

`#` 和 `##` 运算符的求值顺序是未指定的。

### GNU 扩展

部分编译器支持 `##` 放在逗号和 `__VA_ARGS__` 之间：

```cpp
#define DEBUG(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
// 当 __VA_ARGS__ 为空时，移除前面的逗号
```

C++20 起，可使用 `__VA_OPT__` 实现相同效果：

```cpp
#define DEBUG(fmt, ...) fprintf(stderr, fmt __VA_OPT__(,) __VA_ARGS__)
```

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 定义常量

```cpp
#define MAX_BUFFER_SIZE 1024
#define PI 3.14159265358979
#define VERSION "1.0.0"
```

#### 2. 简化代码

```cpp
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
```

#### 3. 调试日志

```cpp
#ifdef DEBUG
    #define LOG(fmt, ...) std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << fmt << std::endl
#else
    #define LOG(fmt, ...)
#endif
```

#### 4. 条件编译

```cpp
#if __cplusplus >= 202002L
    // C++20 代码
#elif __cplusplus >= 201703L
    // C++17 代码
#else
    // 旧版本代码
#endif
```

#### 5. 特性检测（C++20）

```cpp
#if __has_include(<optional>)
    #include <optional>
#endif

#if __has_cpp_attribute([[likely]])
    #define LIKELY [[likely]]
#else
    #define LIKELY
#endif
```

#### 6. 代码生成

```cpp
#define DECLARE_GETTER(type, name) \
    type get_##name() const { return name; } \
    void set_##name(type value) { name = value; }

class MyClass {
    int count;
public:
    DECLARE_GETTER(int, count)
};
```

#### 7. 字符串化与拼接

```cpp
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b
#define MAKE_FUNC(name) void func_##name() { }

MAKE_FUNC(test)  // 生成 void func_test() { }
```

### 最佳实践

1. **宏名使用全大写**：区分宏与普通标识符
2. **参数使用括号包围**：避免运算符优先级问题
3. **使用 `do { ... } while(0)` 包装多语句宏**：
   ```cpp
   #define SWAP(a, b) do { \
       int temp = (a); \
       (a) = (b); \
       (b) = temp; \
   } while(0)
   ```
4. **避免带副作用的参数**：宏参数可能被多次求值
5. **优先使用 `constexpr` 和 `inline` 函数**：享受类型安全和调试支持

### 常见陷阱

#### 陷阱 1：运算符优先级

```cpp
#define DOUBLE(x) x * 2
int result = DOUBLE(1 + 2);  // 展开为 1 + 2 * 2 = 5，而非 6

// 修正
#define DOUBLE(x) ((x) * 2)
```

#### 陷阱 2：参数副作用

```cpp
#define SQUARE(x) ((x) * (x))
int a = 5;
int result = SQUARE(a++);  // 未定义行为：a 被自增两次
```

#### 陷阱 3：模板参数中的逗号

```cpp
// 错误：模板参数中的逗号被解释为宏参数分隔符
assert(std::is_same_v<int, int>);  // 逗号导致参数数量不匹配

// 修正：使用额外括号
assert((std::is_same_v<int, int>));
```

#### 陷阱 4：分号问题

```cpp
#define FUNC() do_something();
if (cond)
    FUNC();  // 展开后的分号导致 else 匹配错误
else
    other();
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：对象式与函数式宏

```cpp
#include <iostream>

// 对象式宏
#define PI 3.14159
#define MAX_SIZE 100

// 函数式宏
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

int main()
{
    std::cout << "PI = " << PI << std::endl;
    std::cout << "MAX_SIZE = " << MAX_SIZE << std::endl;
    std::cout << "MAX(3, 5) = " << MAX(3, 5) << std::endl;
    std::cout << "SQUARE(4) = " << SQUARE(4) << std::endl;
    return 0;
}
```

#### 示例 2：字符串化与拼接

```cpp
#include <iostream>

#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b

// 函数工厂宏
#define FUNCTION(name, a) int fun_##name() { return a; }

FUNCTION(abcd, 12)
FUNCTION(fff, 2)
FUNCTION(qqq, 23)

#undef FUNCTION
#define FUNCTION 34
#define OUTPUT(a) std::cout << "output: " #a << '\n'

int main()
{
    std::cout << "abcd: " << fun_abcd() << '\n';
    std::cout << "fff: " << fun_fff() << '\n';
    std::cout << "qqq: " << fun_qqq() << '\n';

    std::cout << FUNCTION << '\n';
    OUTPUT(million);  // 注意无需引号

    return 0;
}
```

**输出**：
```
abcd: 12
fff: 2
qqq: 23
34
output: million
```

### 高级用法

#### 示例 3：可变参数宏（C++11）

```cpp
#include <iostream>

// 基本可变参数宏
#define LOG(fmt, ...) std::cout << fmt << std::endl

// 字符串化 __VA_ARGS__
#define SHOW_LIST(...) std::cout << #__VA_ARGS__ << std::endl

int main()
{
    LOG("Simple message");
    LOG("Value: ", 42);

    SHOW_LIST();              // 输出空字符串
    SHOW_LIST(1, "x", int);   // 输出 "1, \"x\", int"

    return 0;
}
```

#### 示例 4：`__VA_OPT__` 使用（C++20）

```cpp
#include <iostream>

// 使用 __VA_OPT__ 条件性添加逗号
#define F(...) f(0 __VA_OPT__(,) __VA_ARGS__)
#define G(X, ...) f(0, X __VA_OPT__(,) __VA_ARGS__)
#define SDEF(sname, ...) S sname __VA_OPT__(= { __VA_ARGS__ })

struct S { int a, b; };

int f(int first) { return first; }

int main()
{
    F(a, b, c);    // 替换为 f(0, a, b, c)
    F();           // 替换为 f(0)

    G(a, b, c);    // 替换为 f(0, a, b, c)
    G(a, );        // 替换为 f(0, a)
    G(a);          // 替换为 f(0, a)

    SDEF(foo);         // 替换为 S foo;
    SDEF(bar, 1, 2);   // 替换为 S bar = { 1, 2 };

    return 0;
}
```

#### 示例 5：预定义宏使用

```cpp
#include <iostream>

void print_info()
{
    std::cout << "File: " << __FILE__ << std::endl;
    std::cout << "Line: " << __LINE__ << std::endl;
    std::cout << "Date: " << __DATE__ << std::endl;
    std::cout << "Time: " << __TIME__ << std::endl;
    std::cout << "C++ Version: " << __cplusplus << std::endl;
#ifdef __STDC_HOSTED__
    std::cout << "Hosted: " << __STDC_HOSTED__ << std::endl;
#endif
#ifdef __STDCPP_DEFAULT_NEW_ALIGNMENT__
    std::cout << "Default New Alignment: " << __STDCPP_DEFAULT_NEW_ALIGNMENT__ << std::endl;
#endif
}

int main()
{
    print_info();
    return 0;
}
```

#### 示例 6：可变参数与嵌套宏

```cpp
#include <iostream>

#define WORD "Hello "
#define OUTER(...) WORD #__VA_ARGS__

int main()
{
    std::cout << OUTER(World) << '\n';         // 输出: Hello World
    std::cout << OUTER(WORD World) << '\n';    // 输出: Hello WORD World
    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：缺少括号

```cpp
// 错误：参数未用括号包围
#define DOUBLE(x) x * 2
int result = DOUBLE(1 + 2);  // 展开为 1 + 2 * 2 = 5

// 修正：参数添加括号
#define DOUBLE(x) ((x) * 2)
int result = DOUBLE(1 + 2);  // 展开为 ((1 + 2) * 2) = 6
```

#### 错误示例 2：多语句宏的 if-else 问题

```cpp
// 错误：多语句宏在 if-else 中出错
#define SWAP(a, b) int temp = a; a = b; b = temp

if (x > y)
    SWAP(x, y);  // 语法错误
else
    do_something();

// 修正：使用 do-while(0) 包装
#define SWAP(a, b) do { \
    int temp = (a); \
    (a) = (b); \
    (b) = temp; \
} while(0)
```

#### 错误示例 3：模板参数中的逗号

```cpp
// 错误：模板参数中的逗号被解释为分隔符
#define CHECK(x) check(x)
CHECK(std::is_same_v<int, int>);  // 参数数量不匹配

// 修正：使用额外的括号保护
#define CHECK(x) check((x))
CHECK(std::is_same_v<int, int>);  // 正确
```

## 7. 总结 (Summary)

### 核心要点

1. **两类宏**：对象式宏（简单替换）和函数式宏（带参数）
2. **可变参数**：C++11 引入 `__VA_ARGS__`，C++20 引入 `__VA_OPT__`
3. **特殊运算符**：`#` 字符串化、`##` 标记拼接
4. **预定义宏**：提供编译器、文件、行号等信息
5. **特性检测**：C++20 引入语言特性检测宏系统
6. **防止递归**：扫描过程跟踪已替换的宏，防止无限递归
7. **保留名称**：不能重定义关键字、标准库名称

### 运算符对比

| 运算符 | 名称 | 功能 | 版本 |
|--------|------|------|------|
| `#` | 字符串化 | 参数转为字符串 | C++98 |
| `##` | 标记拼接 | 拼接两个标记 | C++98 |
| `__VA_ARGS__` | 可变参数 | 代表可变参数列表 | C++11 |
| `__VA_OPT__` | 条件内容 | 可变参数非空时展开 | C++20 |

### 宏 vs 函数

| 特性 | 宏 | 函数 |
|------|-----|------|
| 类型安全 | ❌ 无 | ✅ 有 |
| 调试支持 | ❌ 困难 | ✅ 完善 |
| 作用域 | ❌ 全局 | ✅ 遵循作用域 |
| 内联展开 | ✅ 始终 | 编译器决定 |
| 参数求值 | ❌ 可能多次 | ✅ 仅一次 |
| 递归支持 | ❌ 不支持（仅伪递归） | ✅ 支持 |

### 学习建议

1. **理解展开机制**：宏是文本替换，不是函数调用
2. **谨慎使用宏**：优先使用 `constexpr`、`inline` 函数和 `const` 常量
3. **掌握特殊运算符**：`#` 和 `##` 是元编程的关键
4. **注意版本差异**：`__VA_OPT__` 需要 C++20 支持
5. **使用特性检测**：C++20 特性检测宏比版本号检测更精确
6. **阅读编译器文档**：了解编译器特定的扩展

### 缺陷报告

| 编号 | 版本 | 问题 | 修正 |
|------|------|------|------|
| CWG 2908 | C++98 | `__LINE__` 展开为物理行号还是逻辑行号不清楚 | 展开为当前物理行号 |
| LWG 294 | C++98 | 包含标准库头文件的翻译单元可能定义其他头文件中的名称 | 禁止 |
| P2621R2 | C++23 | 通用字符名不能通过标记拼接形成 | 允许 |

---

**相关参考**：
- C++ 宏符号索引文档
- C++ 语言特性检测文档