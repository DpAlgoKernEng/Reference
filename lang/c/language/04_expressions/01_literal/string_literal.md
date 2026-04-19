# 字符串字面量 (String Literals)

## 1. 概述 (Overview)

字符串字面量是在源代码中嵌入字符序列的语法结构，它在编译时创建一个未命名的字符数组对象。字符串字面量是 C 语言中处理文本数据的基础方式。

**核心特点**：
- 用双引号 `"` 包围字符序列
- 编译时自动添加空字符 `\0` 作为终止符
- 创建具有静态存储期的未命名数组
- 支持 UTF-8、UTF-16、UTF-32 等多种编码

**技术定位**：
- 属于词法元素中的字面量类型
- 与字符常量（单引号）不同，字符串字面量表示字符数组
- 在程序运行期间始终存在（静态存储期）

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

字符串字面量是 C 语言的核心特性之一，用于在代码中直接表示文本数据。最初仅支持窄字符串和宽字符串两种形式。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C89/C90 | 定义基本的窄字符串字面量和宽字符串字面量 `L"..."` |
| C99 | 允许窄字符串与宽字符串连接；支持通用字符名 `\u...` 和 `\U...` |
| C11 | 新增 `u8"..."` (UTF-8)、`u"..."` (UTF-16)、`U"..."` (UTF-32) |
| C23 | `u8"..."` 类型从 `char[]` 改为 `char8_t[]`；禁止不同编码前缀的字符串连接 |

### 设计动机

- 提供在源代码中嵌入文本的标准方式
- 支持国际化字符集（Unicode）
- 解决不同编码间的互操作问题

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```c
"字符序列"          // (1) 窄字符串字面量，类型为 char[N]
u8"字符序列"        // (2) UTF-8 字符串字面量 (C11)，类型为 char8_t[N] (C23)
u"字符序列"         // (3) UTF-16 字符串字面量 (C11)，类型为 char16_t[N]
U"字符序列"         // (4) UTF-32 字符串字面量 (C11)，类型为 char32_t[N]
L"字符序列"         // (5) 宽字符串字面量，类型为 wchar_t[N]
```

### s-char 定义

s-char-sequence 中的每个字符可以是：

1. **基本源字符集字符**：除双引号 `"`、反斜杠 `\` 和换行符外的任意字符
2. **转义序列**：
   - 特殊字符转义：`\'` `\"` `\?` `\\` `\a` `\b` `\f` `\n` `\r` `\t` `\v`
   - 十六进制转义：`\x...`
   - 八进制转义：`\...`
3. **通用字符名** (C99+)：`\u....` 或 `\U........`

### 类型与存储

| 前缀 | 类型 | 编码 | 说明 |
|------|------|------|------|
| 无 | `char[N]` | 执行窄编码 | 使用执行字符集编码 |
| `u8` | `char8_t[N]` (C23) | UTF-8 | 每个字符为 UTF-8 码单元 |
| `u` | `char16_t[N]` | UTF-16 (C23) | 每个字符为 UTF-16 码单元 |
| `U` | `char32_t[N]` | UTF-32 (C23) | 每个字符为 UTF-32 码单元 |
| `L` | `wchar_t[N]` | 执行宽编码 | 使用执行宽字符集编码 |

**注意**：`N` 是字符串的码单元数（包括终止空字符）。

## 4. 底层原理 (Underlying Principles)

### 编译阶段处理

字符串字面量在翻译阶段 6 和 7 进行处理：

```
阶段 6：相邻字符串字面量连接
阶段 7：添加终止空字符，创建静态数组
```

### 字符串连接规则

```c
// C99 起：无前缀字符串继承有前缀字符串的编码
L"Δx = %" "d"    // 结果为 L"Δx = %d"

// C23 起：不同编码前缀的连接是错误的
u8"Hello" u"World"  // 编译错误
```

### 内存布局

```c
char* p = "Hello";
// 编译器创建静态数组：{'H', 'e', 'l', 'l', 'o', '\0'}
// p 指向该数组的第一个元素
```

### 字符串字面量的不可修改性

字符串字面量存储在只读内存区域（如 `.rodata`），修改会导致未定义行为：

```c
char* p = "Hello";
p[1] = 'M';  // 未定义行为！

char a[] = "Hello";
a[1] = 'M';  // 正确：a 是副本，不是字面量本身
```

### 字符串字面量的合并

编译器可以将相同的字符串字面量合并到同一地址：

```c
"def" == 3 + "abcdef";  // 可能为 1 或 0，取决于实现
```

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 推荐用法 | 示例 |
|------|----------|------|
| ASCII 文本 | 窄字符串字面量 | `"Hello"` |
| Unicode 文本 | UTF-8 字符串 | `u8"你好"` |
| 国际化应用 | UTF-32 字符串 | `U"世界"` |
| 格式化输出 | 宽字符串 | `L"%d"` |
| 数组初始化 | 字符串初始化 | `char s[] = "text"` |

### 最佳实践

1. **使用 `const` 修饰指针**：防止意外修改字符串字面量
   ```c
   const char* s = "Hello";  // 推荐做法
   ```

2. **UTF-8 文本优先使用 `u8` 前缀**：保证跨平台一致性

3. **避免字符串字面量中的十六进制歧义**：
   ```c
   char* p = "\xff" "f";  // 正确：分隔十六进制转义和后续字符
   ```

### 常见陷阱

| 陷阱 | 问题 | 解决方案 |
|------|------|----------|
| 修改字符串字面量 | 未定义行为，可能崩溃 | 使用字符数组副本 |
| 相同字面量比较 | 可能指向同一地址 | 不依赖地址比较 |
| 嵌入空字符 | `strlen` 返回第一个空字符前的长度 | 注意实际数组大小 |
| 十六进制转义歧义 | `\xfff` 被解释为 `\xfff`（超出范围） | 使用字符串连接分隔 |
| 不同编码连接 | C23 中为编译错误 | 统一使用相同编码前缀 |

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

int main(void) {
    // 字符串字面量作为指针
    const char* greeting = "Hello, World!";
    printf("%s\n", greeting);

    // 字符串字面量初始化数组
    char message[] = "Hello";
    message[0] = 'h';  // 正确：修改的是副本
    printf("%s\n", message);

    return 0;
}
```

### 多种编码示例

```c
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>
#include <locale.h>

int main(void) {
    setlocale(LC_ALL, "en_US.utf8");

    // 窄字符串（执行字符集）
    char s1[] = "你好";

    // UTF-8 字符串
    char s2[] = u8"你好";  // C23 前为 char[]
    // char8_t s2[] = u8"你好";  // C23 起为 char8_t[]

    // UTF-16 字符串
    char16_t s3[] = u"你好";

    // UTF-32 字符串
    char32_t s4[] = U"你好";

    // 宽字符串
    wchar_t s5[] = L"你好";

    printf("窄字符串: %s\n", s1);
    wprintf(L"宽字符串: %ls\n", s5);

    return 0;
}
```

### 字符串连接

```c
#include <stdio.h>

int main(void) {
    // 相邻字符串自动连接
    const char* long_text = "This is a very long "
                            "string that spans "
                            "multiple lines.";
    printf("%s\n", long_text);

    // 与宏结合
    #define PRId16 "d"
    const char* format = "%" PRId16;  // 连接为 "%d"
    printf("Format: %s\n", format);

    return 0;
}
```

### 常见错误示例

```c
#include <stdio.h>

int main(void) {
    // 错误：修改字符串字面量
    char* p1 = "Hello";
    // p1[0] = 'h';  // 未定义行为！

    // 正确：使用数组副本
    char p2[] = "Hello";
    p2[0] = 'h';  // 正确

    // 错误：C23 中不同编码连接
    // char* bad = u8"Hello" "World";  // 这其实可以，但...
    // char* bad2 = u8"Hello" u"World";  // C23 中错误！

    // 注意：嵌入空字符
    const char* embedded = "abc\0def";
    printf("strlen = %zu, sizeof array = %zu\n",
           __builtin_strlen(embedded),  // 3
           sizeof("abc\0def"));          // 8

    return 0;
}
```

### 完整示例：编码对比

```c
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>
#include <locale.h>

int main(void) {
    setlocale(LC_ALL, "en_US.utf8");

    // 同一字符串在不同编码下的表示
    const char* narrow = "a猫🍌";      // 执行字符集
    const char* utf8 = u8"a猫🍌";      // UTF-8
    const char16_t* utf16 = u"a猫🍌";  // UTF-16
    const char32_t* utf32 = U"a猫🍌";  // UTF-32
    const wchar_t* wide = L"a猫🍌";    // 宽字符

    printf("编码对比：\"a猫🍌\"\n");
    printf("====================\n\n");

    // 打印各编码的码单元
    printf("窄字符串 (char): ");
    for (size_t i = 0; narrow[i] != '\0'; i++) {
        printf("0x%02X ", (unsigned char)narrow[i]);
    }
    printf("(空终止符)\n");

    printf("UTF-8 (char):    ");
    for (size_t i = 0; utf8[i] != '\0'; i++) {
        printf("0x%02X ", (unsigned char)utf8[i]);
    }
    printf("(空终止符)\n");

    printf("UTF-16:          ");
    for (size_t i = 0; utf16[i] != 0; i++) {
        printf("0x%04X ", utf16[i]);
    }
    printf("(空终止符)\n");

    printf("UTF-32:          ");
    for (size_t i = 0; utf32[i] != 0; i++) {
        printf("0x%06X ", utf32[i]);
    }
    printf("(空终止符)\n");

    return 0;
}
```

### 数组初始化示例

```c
#include <stdio.h>

int main(void) {
    // 完整初始化（包含空终止符）
    char a1[] = "abc";    // char[4]: {'a', 'b', 'c', '\0'}

    // 指定大小
    char a2[4] = "abc";   // char[4]: {'a', 'b', 'c', '\0'}

    // 截断空终止符
    char a3[3] = "abc";   // char[3]: {'a', 'b', 'c'}

    printf("a1 大小: %zu\n", sizeof(a1));  // 4
    printf("a2 大小: %zu\n", sizeof(a2));  // 4
    printf("a3 大小: %zu\n", sizeof(a3));  // 3

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 基本语法 | `"字符序列"` 或 `前缀"字符序列"` |
| 存储期 | 静态存储期 |
| 可修改性 | 不可修改（修改导致未定义行为） |
| 自动终止 | 编译时添加 `\0` 终止符 |
| 连接规则 | 相邻字符串自动连接 |

### 编码前缀对比

| 前缀 | 类型 | 编码 | 每字符大小 |
|------|------|------|------------|
| 无 | `char[]` | 执行窄编码 | 1 字节（通常） |
| `u8` | `char8_t[]` (C23) | UTF-8 | 1-4 字节 |
| `u` | `char16_t[]` | UTF-16 | 2 或 4 字节 |
| `U` | `char32_t[]` | UTF-32 | 4 字节 |
| `L` | `wchar_t[]` | 执行宽编码 | 平台相关 |

### 版本兼容性提示

- **C23**：`u8"..."` 类型改为 `char8_t[]`；禁止不同编码前缀的字符串连接
- **C11**：新增 `u8"..."`、`u"..."`、`U"..."`
- **C99**：允许窄字符串与宽字符串连接

### 学习建议

1. 理解字符串字面量与字符数组的区别
2. 掌握不同编码前缀的用途和适用场景
3. 避免修改字符串字面量以防止未定义行为
4. 使用 `const` 修饰指向字符串字面量的指针
5. Unicode 文本优先使用 `u8` 前缀