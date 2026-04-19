# C 文本宏替换 (Replacing Text Macros)

## 1. 概述 (Overview)

文本宏替换（Text Macro Replacement）是 C 预处理器的核心功能，允许定义标识符为宏，使其在预处理阶段被替换为指定的替换列表。C 语言支持两种类型的宏：

- **对象式宏（Object-like Macros）**：简单的标识符替换
- **函数式宏（Function-like Macros）**：带参数的宏，支持参数替换

此外，C99 引入了可变参数宏，C23 引入了 `__VA_OPT__` 用于条件性处理可变参数。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

宏替换机制源自 C 语言早期设计，是 C 语言元编程能力的基础。宏在预处理阶段工作，不进行类型检查，提供了强大的文本替换能力。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C89/C90 | 基础宏定义：对象式宏、函数式宏、`#` 和 `##` 运算符 |
| C95 | 引入 `__STDC_VERSION__` 宏 |
| C99 | 可变参数宏、`__VA_ARGS__`、`__func__`、多项预定义宏 |
| C11 | 新增特性检测宏（`__STDC_NO_*`）、UTF 编码宏 |
| C17 | 无重大变更 |
| C23 | `__VA_OPT__` 运算符、新增多项 IEC 60559 相关宏、`__STDC_EMBED_*` 宏 |

### 设计动机

宏替换解决了以下核心问题：

1. **常量定义**：定义编译时常量，避免魔法数字
2. **代码复用**：通过函数式宏简化重复代码
3. **条件编译**：配合 `#ifdef` 实现平台适配
4. **调试支持**：通过 `__FILE__`、`__LINE__` 等宏提供诊断信息
5. **元编程**：通过 `#` 和 `##` 运算符实现代码生成

## 3. 语法与参数 (Syntax and Parameters)

### `#define` 指令语法

| 语法形式 | 说明 | 版本 |
|----------|------|------|
| `#define 标识符 替换列表(可选)` | (1) 对象式宏 | C89 |
| `#define 标识符(参数) 替换列表` | (2) 函数式宏 | C89 |
| `#define 标识符(参数, ...) 替换列表` | (3) 可变参数函数式宏 | C99 |
| `#define 标识符(...) 替换列表` | (4) 纯可变参数函数式宏 | C99 |

### `#undef` 指令语法

```
#undef 标识符
```

取消标识符的宏定义。如果标识符未定义为宏，则忽略该指令。

### 对象式宏

对象式宏将标识符简单替换为替换列表：

```c
#define PI 3.14159
#define MAX_SIZE 100
#define DEBUG
```

### 函数式宏

函数式宏支持参数替换：

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))
```

**调用语法**：宏名后紧跟 `(`，参数列表由匹配的 `)` 终止，跳过中间匹配的括号对。

**参数要求**：参数数量必须与定义一致，否则程序非法。

### 可变参数宏（C99）

使用 `__VA_ARGS__` 访问额外参数：

```c
#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)
#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
```

### `__VA_OPT__` 运算符（C23）

```c
__VA_OPT__(内容)
```

- 如果 `__VA_ARGS__` 非空，替换为 `内容`
- 如果 `__VA_ARGS__` 为空，替换为空

```c
#define F(...) f(0 __VA_OPT__(,) __VA_ARGS__)
F(a, b, c) // 替换为 f(0, a, b, c)
F()        // 替换为 f(0)
```

### `#` 字符串化运算符

将参数转换为字符串字面量：

```c
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

```c
#define CONCAT(a, b) a##b
CONCAT(var, 1)  // 替换为 var1
```

**限制**：只能拼接形成有效标记的标记对。

### 预定义宏

#### 必须预定义的宏

| 宏名 | 说明 | 版本 |
|------|------|------|
| `__STDC__` | 展开为整数常量 `1`，表示符合标准的实现 | C89 |
| `__STDC_VERSION__` | 展开为版本号长整型常量 | C95 |
| `__STDC_HOSTED__` | 宿主实现为 `1`，独立实现为 `0` | C99 |
| `__FILE__` | 当前文件名（字符串字面量） | C89 |
| `__LINE__` | 当前行号（整数常量） | C89 |
| `__DATE__` | 翻译日期，格式 `"Mmm dd yyyy"` | C89 |
| `__TIME__` | 翻译时间，格式 `"hh:mm:ss"` | C89 |

#### `__STDC_VERSION__` 值

| 版本 | 值 |
|------|-----|
| C95 | `199409L` |
| C99 | `199901L` |
| C11 | `201112L` |
| C17 | `201710L` |
| C23 | `202311L` |

#### 实现可选预定义的宏

| 宏名 | 说明 | 版本 |
|------|------|------|
| `__STDC_ISO_10646__` | `wchar_t` 使用 Unicode | C99 |
| `__STDC_IEC_559__` | IEC 60559 浮点支持（C23 起弃用） | C99 |
| `__STDC_IEC_559_COMPLEX__` | IEC 60559 复数支持（C23 起弃用） | C99 |
| `__STDC_UTF_16__` | `char16_t` 使用 UTF-16 编码 | C11/C23 |
| `__STDC_UTF_32__` | `char32_t` 使用 UTF-32 编码 | C11/C23 |
| `__STDC_MB_MIGHT_NEQ_WC__` | `'x' == L'x'` 可能为假 | C99 |
| `__STDC_ANALYZABLE__` | 可分析性支持 | C11 |
| `__STDC_LIB_EXT1__` | 边界检查接口支持 | C11 |
| `__STDC_NO_ATOMICS__` | 不支持原子操作 | C11 |
| `__STDC_NO_COMPLEX__` | 不支持复数 | C11 |
| `__STDC_NO_THREADS__` | 不支持多线程 | C11 |
| `__STDC_NO_VLA__` | 不支持变长数组 | C11 |
| `__STDC_EMBED_NOT_FOUND__` | 展开为 `0` | C23 |
| `__STDC_EMBED_FOUND__` | 展开为 `1` | C23 |
| `__STDC_EMBED_EMPTY__` | 展开为 `2` | C23 |
| `__STDC_IEC_60559_BFP__` | IEC 60559 二进制浮点 | C23 |
| `__STDC_IEC_60559_DFP__` | IEC 60559 十进制浮点 | C23 |
| `__STDC_IEC_60559_COMPLEX__` | IEC 60559 复数运算 | C23 |
| `__STDC_IEC_60559_TYPES__` | IEC 60559 交换和扩展类型 | C23 |

## 4. 底层原理 (Underlying Principles)

### 宏展开流程

```
源代码
    ↓
扫描标识符
    ↓
识别宏定义
    ↓
参数替换（函数式宏）
    ↓
# 运算符处理（字符串化）
    ↓
## 运算符处理（标记拼接）
    ↓
递归展开（直到无法继续）
    ↓
替换完成
```

### 展开规则

1. **先扫描后展开**：先识别宏调用，再进行参数替换
2. **递归展开**：替换后的内容如果包含宏，继续展开
3. **防止递归**：宏在展开过程中不会再次展开自身
4. **参数独立展开**：参数在替换到替换列表之前先展开

### 字符串化（`#`）处理

```c
#define STR(x) #x
STR(hello world)  // → "hello world"

// 嵌入字符串转义
#define STR2(x) #x
STR2("hello")     // → "\"hello\""
```

### 标记拼接（`##`）处理

```c
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

```c
#define DEBUG(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
// 当 __VA_ARGS__ 为空时，移除前面的逗号
```

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 定义常量

```c
#define MAX_BUFFER_SIZE 1024
#define PI 3.14159265358979
#define VERSION "1.0.0"
```

#### 2. 简化代码

```c
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
```

#### 3. 调试日志

```c
#ifdef DEBUG
    #define LOG(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define LOG(fmt, ...)
#endif
```

#### 4. 条件编译

```c
#define PLATFORM_WINDOWS 1
#define PLATFORM_LINUX 2

#if PLATFORM == PLATFORM_WINDOWS
    // Windows 代码
#elif PLATFORM == PLATFORM_LINUX
    // Linux 代码
#endif
```

#### 5. 代码生成

```c
#define DECLARE_GETTER(type, name) \
    type get_##name(void) { return name; } \
    void set_##name(type value) { name = value; }

static int count;
DECLARE_GETTER(int, count)  // 生成 get_count() 和 set_count()
```

#### 6. 字符串化与拼接

```c
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b
#define MAKE_FUNC(name) void func_##name(void) { }

MAKE_FUNC(test)  // 生成 void func_test(void) { }
```

### 最佳实践

1. **宏名使用全大写**：区分宏与普通标识符
2. **参数使用括号包围**：避免运算符优先级问题
3. **使用 `do { ... } while(0)` 包装多语句宏**：
   ```c
   #define SWAP(a, b) do { \
       int temp = (a); \
       (a) = (b); \
       (b) = temp; \
   } while(0)
   ```
4. **避免带副作用的参数**：宏参数可能被多次求值
5. **优先使用 `inline` 函数**：享受类型安全和调试支持

### 常见陷阱

#### 陷阱 1：运算符优先级

```c
#define DOUBLE(x) x * 2
int result = DOUBLE(1 + 2);  // 展开为 1 + 2 * 2 = 5，而非 6

// 修正
#define DOUBLE(x) ((x) * 2)
```

#### 陷阱 2：参数副作用

```c
#define SQUARE(x) ((x) * (x))
int a = 5;
int result = SQUARE(a++);  // 未定义行为：a 被自增两次
```

#### 陷阱 3：逗号分隔符问题

```c
#define CALL(func, args) func(args)
CALL(printf, ("hello, %d", 42));  // 逗号被解释为参数分隔符
// 参数数量不匹配！
```

#### 陷阱 4：分号问题

```c
#define FUNC() do_something();
if (cond)
    FUNC();  // 展开后的分号导致 else 匹配错误
else
    other();
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：对象式与函数式宏

```c
#include <stdio.h>

// 对象式宏
#define PI 3.14159
#define MAX_SIZE 100

// 函数式宏
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

int main(void)
{
    printf("PI = %f\n", PI);
    printf("MAX_SIZE = %d\n", MAX_SIZE);
    printf("MAX(3, 5) = %d\n", MAX(3, 5));
    printf("SQUARE(4) = %d\n", SQUARE(4));
    return 0;
}
```

#### 示例 2：字符串化与拼接

```c
#include <stdio.h>

#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b

// 函数工厂宏
#define FUNCTION(name, a) int fun_##name(int x) { return (a) * x; }

FUNCTION(quadruple, 4)
FUNCTION(double, 2)

int main(void)
{
    printf("quadruple(13): %d\n", fun_quadruple(13));
    printf("double(21): %d\n", fun_double(21));
    printf("Stringified: %s\n", STRINGIFY(hello world));

    int CONCAT(var, 1) = 10;  // 创建变量 var1
    printf("var1 = %d\n", var1);

    return 0;
}
```

**输出**：
```
quadruple(13): 52
double(21): 42
Stringified: hello world
var1 = 10
```

### 高级用法

#### 示例 3：可变参数宏（C99）

```c
#include <stdio.h>

// 基本可变参数宏
#define LOG(fmt, ...) printf("[LOG] " fmt "\n", ##__VA_ARGS__)

// 字符串化 __VA_ARGS__
#define SHOW_LIST(...) puts(#__VA_ARGS__)

int main(void)
{
    LOG("Simple message");
    LOG("Value: %d", 42);
    LOG("Values: %d, %s", 10, "hello");

    SHOW_LIST();              // 输出空字符串
    SHOW_LIST(1, "x", int);   // 输出 "1, \"x\", int"

    return 0;
}
```

#### 示例 4：`__VA_OPT__` 使用（C23）

```c
#include <stdio.h>

// 使用 __VA_OPT__ 条件性添加逗号
#define F(...) f(0 __VA_OPT__(,) __VA_ARGS__)
#define G(X, ...) f(0, X __VA_OPT__(,) __VA_ARGS__)
#define SDEF(sname, ...) S sname __VA_OPT__(= { __VA_ARGS__ })

typedef struct { int a, b; } S;

int f(int first, ...) { return first; }

int main(void)
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

```c
#include <stdio.h>

void print_info(void)
{
    printf("File: %s\n", __FILE__);
    printf("Line: %d\n", __LINE__);
    printf("Date: %s\n", __DATE__);
    printf("Time: %s\n", __TIME__);
    printf("STDC: %d\n", __STDC__);
    printf("STDC_VERSION: %ldL\n", __STDC_VERSION__);
#ifdef __STDC_HOSTED__
    printf("Hosted: %d\n", __STDC_HOSTED__);
#endif
}

int main(void)
{
    print_info();
    return 0;
}
```

#### 示例 6：函数工厂模式

```c
#include <stdio.h>

// 定义函数工厂宏
#define FUNCTION(name, a) int fun_##name(int x) { return (a) * x; }

// 使用工厂宏创建函数
FUNCTION(quadruple, 4)
FUNCTION(double, 2)

#undef FUNCTION
#define FUNCTION 34
#define OUTPUT(a) puts(#a)

int main(void)
{
    printf("quadruple(13): %d\n", fun_quadruple(13));
    printf("double(21): %d\n", fun_double(21));
    printf("%d\n", FUNCTION);
    OUTPUT(billion);  // 输出 "billion"（无需引号）

    return 0;
}
```

**输出**：
```
quadruple(13): 52
double(21): 42
34
billion
```

### 常见错误及修正

#### 错误示例 1：缺少括号

```c
// 错误：参数未用括号包围
#define DOUBLE(x) x * 2
int result = DOUBLE(1 + 2);  // 展开为 1 + 2 * 2 = 5

// 修正：参数添加括号
#define DOUBLE(x) ((x) * 2)
int result = DOUBLE(1 + 2);  // 展开为 ((1 + 2) * 2) = 6
```

#### 错误示例 2：多语句宏的 if-else 问题

```c
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

#### 错误示例 3：参数中的逗号

```c
// 错误：参数中的逗号被解释为分隔符
#define CALL(func, args) func(args)
CALL(printf, ("hello, %d", 42));  // 参数数量不匹配

// 修正：使用额外的括号
#define CALL(func, args) func args
CALL(printf, ("hello, %d", 42));  // 正确：printf("hello, %d", 42)
```

## 7. 总结 (Summary)

### 核心要点

1. **两类宏**：对象式宏（简单替换）和函数式宏（带参数）
2. **可变参数**：C99 引入 `__VA_ARGS__`，C23 引入 `__VA_OPT__`
3. **特殊运算符**：`#` 字符串化、`##` 标记拼接
4. **预定义宏**：提供编译器、文件、行号等信息
5. **宏重定义**：必须与原定义完全相同，否则程序非法

### 运算符对比

| 运算符 | 名称 | 功能 | 版本 |
|--------|------|------|------|
| `#` | 字符串化 | 参数转为字符串 | C89 |
| `##` | 标记拼接 | 拼接两个标记 | C89 |
| `__VA_ARGS__` | 可变参数 | 代表可变参数列表 | C99 |
| `__VA_OPT__` | 条件内容 | 可变参数非空时展开 | C23 |

### 对象式宏 vs 函数式宏

| 特性 | 对象式宏 | 函数式宏 |
|------|----------|----------|
| 参数 | 无 | 有 |
| 调用方式 | 直接使用标识符 | 标识符后跟 `(参数)` |
| 典型用途 | 常量定义 | 代码简化、类型泛型 |
| 参数求值 | 不适用 | 可能多次求值 |

### 宏 vs 函数

| 特性 | 宏 | 函数 |
|------|-----|------|
| 类型安全 | ❌ 无 | ✅ 有 |
| 调试支持 | ❌ 困难 | ✅ 完善 |
| 作用域 | ❌ 全局 | ✅ 遵循作用域 |
| 内联展开 | ✅ 始终 | 编译器决定 |
| 参数求值 | ❌ 可能多次 | ✅ 仅一次 |
| 递归支持 | ❌ 不支持 | ✅ 支持 |

### 学习建议

1. **理解展开机制**：宏是文本替换，不是函数调用
2. **谨慎使用宏**：优先使用 `inline` 函数和 `const` 常量
3. **掌握特殊运算符**：`#` 和 `##` 是元编程的关键
4. **注意版本差异**：`__VA_OPT__` 需要 C23 支持
5. **阅读编译器文档**：了解编译器特定的扩展

### 缺陷报告

| 编号 | 版本 | 问题 | 修正 |
|------|------|------|------|
| DR 321 | C99 | `L'x' == 'x'` 是否总是成立不清楚 | 添加 `__STDC_MB_MIGHT_NEQ_WC__` 用于此目的 |

---

**标准参考**：
- C23 (ISO/IEC 9899:2024): 6.10.4 Macro replacement, 6.10.9 Predefined macro names
- C17 (ISO/IEC 9899:2018): 6.10.3 Macro replacement, 6.10.8 Predefined macro names
- C11 (ISO/IEC 9899:2011): 6.10.3 Macro replacement, 6.10.8 Predefined macro names
- C99 (ISO/IEC 9899:1999): 6.10.3 Macro replacement, 6.10.8 Predefined macro names
- C89/C90 (ISO/IEC 9899:1990): 3.8.3 Macro replacement, 3.8.8 Predefined macro names