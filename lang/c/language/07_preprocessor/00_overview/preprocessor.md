# 预处理器 (Preprocessor)

## 1. 概述 (Overview)

预处理器 (preprocessor) 是 C 语言编译过程中的一个关键组件，在**翻译阶段 4 (translation phase 4)** 执行，早于实际编译器的工作。预处理器处理源代码中的预处理指令，生成一个单一的文件，然后将该文件传递给编译器进行编译。

预处理器的主要职责是**文本转换**，它不进行语法分析，仅按照指令规则对源代码进行文本级别的替换、删除和合并操作。

### 技术定位

预处理器位于编译流程的前端：

```
源代码 → 预处理器 → 预处理后的代码 → 编译器 → 目标代码
```

## 2. 来源与演变 (Origin and Evolution)

### 首次引入

预处理器的概念最早出现在 C 语言诞生之初，由 Dennis Ritchie 在 20 世纪 70 年代初期设计。最初的预处理器功能相对简单，主要解决以下问题：

1. **代码复用**：通过 `#include` 机制共享头文件
2. **常量定义**：通过 `#define` 避免魔法数字
3. **条件编译**：支持跨平台代码

### C 标准演变

| 标准 | 主要变化 |
|------|----------|
| C89/C90 | 标准化预处理指令，定义基础指令集 |
| C99 | 新增 `_Pragma` 运算符 |
| C23 | 新增 `#elifdef`, `#elifndef`, `#embed`, `#warning` 指令，`__has_include` 检查 |

### C23 新特性

C23 标准对预处理器进行了重要扩展：

- **`#elifdef` / `#elifndef`**：简化条件编译嵌套
- **`#embed`**：支持二进制资源嵌入
- **`#warning`**：标准化之前广泛使用的非标准扩展
- **`__has_include`**：编译时检查头文件是否存在

## 3. 语法与参数 (Syntax and Parameters)

### 指令格式

每个预处理指令占据一行，具有以下格式：

```
# 字符
预处理指令（见下表）
参数（取决于具体指令）
换行符
```

### 标准预处理指令

| 指令 | 用途 | 版本 |
|------|------|------|
| `#define` | 定义宏 | C89 |
| `#undef` | 取消宏定义 | C89 |
| `#include` | 包含文件 | C89 |
| `#if` | 条件编译 | C89 |
| `#ifdef` | 如果定义了宏 | C89 |
| `#ifndef` | 如果未定义宏 | C89 |
| `#else` | 否则分支 | C89 |
| `#elif` | 否则如果 | C89 |
| `#elifdef` | 否则如果定义了宏 | C23 |
| `#elifndef` | 否则如果未定义宏 | C23 |
| `#endif` | 结束条件编译 | C89 |
| `#line` | 设置行号和文件名 | C89 |
| `#embed` | 嵌入二进制资源 | C23 |
| `#error` | 产生编译错误 | C89 |
| `#warning` | 产生编译警告 | C23 |
| `#pragma` | 编译器指示 | C89 |

### 空指令

`#` 后紧跟换行符的空指令是合法的，不会产生任何效果：

```c
#  // 空指令，无效果
```

### 相关运算符

| 运算符 | 用途 | 示例 |
|--------|------|------|
| `#` | 字符串化运算符 | `#define STR(x) #x` |
| `##` | 标记拼接运算符 | `#define CONCAT(a, b) a##b` |
| `_Pragma` | pragma 的运算符形式 (C99) | `_Pragma("pack(1)")` |
| `__has_include` | 检查头文件是否存在 (C23) | `#if __has_include(<header.h>)` |

## 4. 底层原理 (Underlying Principles)

### 翻译阶段

根据 C 标准，编译过程分为 8 个翻译阶段，预处理器在第 4 阶段执行：

| 阶段 | 操作 |
|------|------|
| 1 | 字符映射（三字符组替换） |
| 2 | 行拼接（反斜杠换行） |
| 3 | 标记化（注释替换为空格） |
| **4** | **预处理（执行预处理指令、宏展开）** |
| 5 | 字符集映射 |
| 6 | 字符串字面量拼接 |
| 7 | 编译（语法分析、代码生成） |
| 8 | 链接 |

### 处理流程

```
源文件
    │
    ├── #include 指令 ──→ 递归处理被包含文件 ──→ 文件内容替换
    │
    ├── 宏定义 ──→ 宏展开 ──→ 文本替换
    │
    └── 条件编译 ──→ 条件判断 ──→ 保留或删除代码块
    │
    ↓
单一预处理后的文件 ──→ 传递给编译器
```

### 宏展开机制

宏展开遵循以下规则：

1. **递归展开**：宏体中的宏会被递归展开
2. **非递归自引用**：宏自引用时不会无限递归
3. **参数替换**：函数式宏的参数在展开前先进行替换

```c
#define M  A + M   // M 自引用，M 不会被展开
#define A  1
int x = M;         // 展开为: int x = 1 + M; (M 保持不变)
```

## 5. 使用场景 (Use Cases)

### 预处理器能力总览

| 能力 | 控制指令 | 用途 |
|------|----------|------|
| 条件编译 | `#if`, `#ifdef`, `#ifndef`, `#else`, `#elif`, `#endif` | 跨平台代码、调试开关 |
| 文本宏替换 | `#define`, `#undef`, `#`, `##` | 常量定义、代码生成 |
| 文件包含 | `#include`, `__has_include` | 模块化、代码复用 |
| 错误/警告 | `#error`, `#warning` | 编译时检查 |
| 实现控制 | `#pragma`, `_Pragma` | 编译器特定行为 |
| 信息控制 | `#line` | 调试信息控制 |

### 条件编译场景

#### 跨平台支持

```c
#ifdef _WIN32
    #include <windows.h>
    #define PLATFORM_NAME "Windows"
#elif defined(__linux__)
    #include <unistd.h>
    #define PLATFORM_NAME "Linux"
#elif defined(__APPLE__)
    #include <dispatch/dispatch.h>
    #define PLATFORM_NAME "macOS"
#else
    #define PLATFORM_NAME "Unknown"
#endif
```

#### 调试开关

```c
#ifdef DEBUG
    #define LOG(msg) printf("[DEBUG] %s:%d: %s\n", __FILE__, __LINE__, msg)
#else
    #define LOG(msg) ((void)0)  // 生产环境空操作
#endif
```

### 宏定义场景

#### 常量定义

```c
#define MAX_BUFFER_SIZE 1024
#define PI 3.14159265358979
#define VERSION "1.0.0"
```

#### 函数式宏

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
```

#### 代码生成

```c
#define DECLARE_GETTER(type, name) \
    type get_##name(void) { return name; }

#define DECLARE_SETTER(type, name) \
    void set_##name(type value) { name = value; }

// 使用
static int count;
DECLARE_GETTER(int, count)    // 生成 get_count()
DECLARE_SETTER(int, count)    // 生成 set_count(int)
```

### 最佳实践

1. **使用 `#pragma once` 或头文件保护**
   ```c
   // 方式 1: 传统的头文件保护
   #ifndef MYHEADER_H
   #define MYHEADER_H
   // 头文件内容
   #endif

   // 方式 2: 非标准但广泛支持的 #pragma once
   #pragma once
   // 头文件内容
   ```

2. **函数式宏参数加括号**
   ```c
   // 正确：每个参数都用括号包围
   #define SQUARE(x) ((x) * (x))
   ```

3. **使用 `do { ... } while(0)` 包裹多语句宏**
   ```c
   #define SAFE_FREE(ptr) do { \
       free(ptr); \
       ptr = NULL; \
   } while(0)
   ```

4. **条件编译时避免逻辑复杂化**
   ```c
   // 不推荐：嵌套过深
   #if defined(A)
       #if defined(B)
           // ...
       #endif
   #endif

   // 推荐：使用逻辑运算符
   #if defined(A) && defined(B)
       // ...
   #endif
   ```

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 宏参数副作用 | `MAX(i++, j)` 中 i 会自增两次 | 使用内联函数或避免副作用参数 |
| 运算符优先级 | `#define DOUBLE(x) x * 2` 展开错误 | 用括号包围整个宏体和参数 |
| 分号问题 | 宏展开后多余分号 | 使用 `do { } while(0)` |
| 名称冲突 | 宏名与变量/函数冲突 | 使用项目前缀，如 `PROJECT_MAX` |

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

// 常量定义
#define MAX_SIZE 100
#define GREETING "Hello, World!"

// 函数式宏
#define ABS(x) ((x) < 0 ? -(x) : (x))

int main(void) {
    printf("%s\n", GREETING);

    int values[] = {-5, 10, -15, 20};
    for (int i = 0; i < 4; i++) {
        printf("ABS(%d) = %d\n", values[i], ABS(values[i]));
    }

    return 0;
}
```

### 条件编译

```c
#include <stdio.h>

// 编译时检测特性
#if __STDC_VERSION__ >= 201112L
    #define HAS_C11 1
#else
    #define HAS_C11 0
#endif

// C23 新语法示例
#if __STDC_VERSION__ >= 202311L
    #ifdef FEATURE_X
        // C23 新增的 elifdef
        #elifdef FEATURE_Y
            // 如果未定义 FEATURE_X 但定义了 FEATURE_Y
        #elifndef FEATURE_Z
            // 如果都未定义
    #endif
#endif

int main(void) {
    #if HAS_C11
        printf("C11 or later is supported\n");
    #else
        printf("Pre-C11 compiler\n");
    #endif

    return 0;
}
```

### 编译时检查

```c
// 使用 #error 进行编译时检查
#ifndef REQUIRED_VERSION
    #error "REQUIRED_VERSION must be defined"
#endif

#if REQUIRED_VERSION < 2
    #error "REQUIRED_VERSION must be at least 2"
#endif

// C23: 使用 #warning 提示信息
#if defined(DEPRECATED_FEATURE)
    #warning "DEPRECATED_FEATURE is deprecated, use NEW_FEATURE instead"
#endif
```

### C23 embed 示例

```c
#include <stdio.h>

// C23: 嵌入二进制资源
const unsigned char icon_data[] = {
    #embed "icon.png"
};

const unsigned char icon_data_with_limit[] = {
    #embed "icon.png" limit(1024)  // 限制最大 1024 字节
};

int main(void) {
    printf("Icon data size: %zu bytes\n", sizeof(icon_data));
    return 0;
}
```

### 常见错误及修正

#### 错误 1：宏参数缺少括号

```c
// 错误：运算符优先级问题
#define DOUBLE(x) x * 2
int result = DOUBLE(3 + 4);  // 展开为: 3 + 4 * 2 = 11，而非 14

// 修正：参数和整个表达式都用括号包围
#define DOUBLE(x) ((x) * 2)
int result = DOUBLE(3 + 4);  // 展开为: ((3 + 4) * 2) = 14
```

#### 错误 2：参数副作用

```c
// 问题：参数会被求值多次
#define SQUARE(x) ((x) * (x))
int i = 5;
int result = SQUARE(i++);  // 展开为: ((i++) * (i++))，未定义行为！

// 修正方案 1：使用内联函数
static inline int square(int x) {
    return x * x;
}

// 修正方案 2：调用时避免副作用
int i = 5;
int result = SQUARE(i);  // 正确
i++;  // 单独自增
```

#### 错误 3：多语句宏的分号问题

```c
// 问题：if-else 结构中出错
#define INCREMENT(x) x++; x++
int a = 1;
if (condition)
    INCREMENT(a);  // 只有第一个语句属于 if 块！
else
    a = 0;

// 修正：使用 do-while(0)
#define INCREMENT(x) do { x++; x++; } while(0)
int a = 1;
if (condition)
    INCREMENT(a);  // 整个宏作为一个语句
else
    a = 0;
```

#### 错误 4：头文件重复包含

```c
// 问题：缺少头文件保护
// header.h
struct Point { int x, y; };  // 多次包含会导致重定义错误

// 修正：使用头文件保护
#ifndef HEADER_H
#define HEADER_H
struct Point { int x, y; };
#endif

// 或使用 #pragma once（非标准但广泛支持）
#pragma once
struct Point { int x, y; };
```

## 7. 总结 (Summary)

预处理器是 C 语言的重要特性，提供了强大的**文本转换**能力：

### 核心能力

| 能力 | 用途 |
|------|------|
| 条件编译 | 跨平台支持、特性开关、调试代码控制 |
| 宏定义 | 常量、代码生成、简化重复代码 |
| 文件包含 | 模块化设计、库接口声明 |
| 编译控制 | 编译器指示、错误检查 |

### 使用建议

1. **优先使用枚举/const 代替宏常量**
   ```c
   // 推荐
   enum { MAX_SIZE = 100 };
   static const char* VERSION = "1.0.0";

   // 而非
   #define MAX_SIZE 100
   #define VERSION "1.0.0"
   ```

2. **优先使用内联函数代替函数式宏**
   ```c
   // 推荐
   static inline int max(int a, int b) { return a > b ? a : b; }

   // 而非
   #define MAX(a, b) ((a) > (b) ? (a) : (b))
   ```

3. **宏命名使用全大写+下划线**
   ```c
   #define PROJECT_MAX_ITEMS 100
   #define PROJECT_LOG_LEVEL 2
   ```

4. **谨慎使用条件编译**，保持代码可读性

### 与 C++ 的区别

| 特性 | C 预处理器 | C++ 替代方案 |
|------|------------|--------------|
| 常量 | `#define` | `constexpr` |
| 类型安全泛型 | 宏 | 模板 |
| 内联函数 | 函数式宏 | `inline` 函数 |
| 条件编译 | `#ifdef` | `if constexpr` (C++17) |

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.10 Preprocessing directives
- C17 标准 (ISO/IEC 9899:2018): 6.10 Preprocessing directives (p: 117-129)
- C11 标准 (ISO/IEC 9899:2011): 6.10 Preprocessing directives (p: 160-178)
- C99 标准 (ISO/IEC 9899:1999): 6.10 Preprocessing directives (p: 145-162)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.8 Preprocessing directives