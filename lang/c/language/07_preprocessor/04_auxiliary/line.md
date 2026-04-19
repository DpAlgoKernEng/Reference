# C 文件名和行号信息 (Filename and Line Information)

## 1. 概述 (Overview)

`#line` 指令是 C 预处理器的控制指令，用于修改预处理器中的当前行号和文件名。这会影响 `__LINE__` 和 `__FILE__` 预定义宏的展开结果。

### 核心功能

- **修改行号**：更改当前的预处理器行号
- **修改文件名**：更改当前的预处理器文件名
- **影响宏展开**：改变 `__LINE__` 和 `__FILE__` 宏的值

### 主要用途

`#line` 指令主要用于自动代码生成工具，这些工具从其他语言或格式生成 C 源代码。通过 `#line` 指令，生成的 C 文件可以引用原始源文件的行号和文件名，便于调试和错误定位。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`#line` 指令从 C 语言早期就存在，最初是为了支持代码生成工具。当从其他语言（如 Yacc/Bison 语法文件、Lex 词法文件等）生成 C 代码时，`#line` 指令可以将错误信息映射回原始源文件，而不是生成的 C 文件。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C89/C90 | 标准化 `#line` 指令，行号上限为 32767 |
| C99 | 行号上限提升至 2147483647 |
| C11 | 无重大变更 |
| C17 | 无重大变更 |

### 设计动机

`#line` 指令解决了以下核心问题：

1. **错误定位**：将错误信息映射到原始源文件
2. **调试支持**：在调试器中显示正确的源代码位置
3. **代码生成工具**：支持 Yacc、Bison、Lex 等工具
4. **日志记录**：在日志中显示原始文件的行号

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

| 语法 | 说明 |
|------|------|
| `#line lineno` | (1) 仅更改行号 |
| `#line lineno "filename"` | (2) 更改行号和文件名 |

### 参数说明

#### lineno（行号）

- 必须是至少一个十进制数字的序列
- 始终解释为十进制数（即使以 0 开头）
- 允许使用预处理标记（宏常量或表达式），只要它们展开为有效的十进制整数

**行号范围**：

| 版本 | 有效范围 | 超出范围的行为 |
|------|----------|----------------|
| C89/C90 | 1 - 32767 | 未定义行为 |
| C99 起 | 1 - 2147483647 | 未定义行为 |

如果 `lineno` 为 0 或超过上限，行为未定义。

#### filename（文件名）

- 必须是有效的字符串
- 可以是字符串字面量或展开为字符串的宏
- 后续的 `__FILE__` 宏将展开为此文件名

### 行为说明

#### 形式 (1)

- 将当前预处理器行号更改为 `lineno`
- 后续的 `__LINE__` 宏将展开为 `lineno` 加上此后遇到的实际源代码行数

#### 形式 (2)

- 同时更改当前预处理器文件名
- 后续的 `__FILE__` 宏将产生 `filename`

## 4. 底层原理 (Underlying Principles)

### 处理流程

```
预处理阶段
    ↓
遇到 #line 指令
    ↓
解析参数
    ↓
┌─────────────────────────────────┐
│ 仅行号        │ 行号 + 文件名   │
└─────────────────────────────────┘
    ↓                ↓
更新内部行号    更新内部行号和文件名
    ↓                ↓
后续 __LINE__   后续 __FILE__ 和 __LINE__
宏受影响         宏受影响
```

### 行号计算

```
#line 100
// __LINE__ 在此行为 100

int a;      // __LINE__ = 101
int b;      // __LINE__ = 102
// ...
```

### DR 464：未指定行为

`#line __LINE__` 后的行号是**未指定的**（unspecified）。

原因：`__LINE__` 可能有两个值：
1. 到目前为止看到的换行符数量
2. 到目前为止看到的换行符数量加上结束 `#line` 指令的换行符

**DR 464**：此缺陷报告追溯性地适用于所有 C 标准版本。

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 代码生成工具

Yacc/Bison 生成的解析器使用 `#line` 指令：

```c
/* 由 yacc 生成的代码 */
#line 42 "parser.y"
/* 错误将指向 parser.y 的第 42 行 */
```

#### 2. 模板系统

从模板生成代码时保持原始行号：

```c
/* 模板引擎生成 */
#line 15 "template.html"
/* HTML 模板中的代码 */
```

#### 3. 调试辅助

在特定位置强制设置行号，便于调试：

```c
#line 1000 "debug_section"
// 错误信息将显示 debug_section:1000
```

#### 4. 测试框架

在测试代码中控制错误消息的显示：

```c
#line 1 "test_case"
assert(condition);  // 错误消息显示 test_case:1
```

### 最佳实践

1. **用于代码生成工具**：这是 `#line` 的主要用途
2. **保持一致性**：如果修改行号，也要修改文件名
3. **记录原始位置**：在注释中说明为什么使用 `#line`
4. **避免滥用**：不要在手动编写的代码中随意使用

### 常见陷阱

#### 陷阱 1：行号超出范围

```c
#line 99999999999999999 "file.c"  // 未定义行为：超出范围
```

#### 陷阱 2：行号为 0

```c
#line 0 "file.c"  // 未定义行为：行号不能为 0
```

#### 陷阱 3：使用 `__LINE__` 自引用

```c
#line __LINE__  // 未指定行为：DR 464
```

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：更改行号和文件名

```c
#include <assert.h>
#define FNAME "test.c"

int main(void)
{
#line 777 FNAME
        assert(2+2 == 5);
}
```

**输出**：
```
test: test.c:777: int main(): Assertion `2+2 == 5' failed.
```

#### 示例 2：仅更改行号

```c
#include <stdio.h>

int main(void)
{
    printf("Current line: %d\n", __LINE__);  // 输出: 6

#line 100
    printf("After #line: %d\n", __LINE__);   // 输出: 100
    printf("Next line: %d\n", __LINE__);      // 输出: 101

    return 0;
}
```

### 高级用法

#### 示例 3：代码生成工具模拟

```c
#include <stdio.h>
#include <assert.h>

/* 模拟 Yacc/Bison 生成的代码 */

/* 原始文件: calc.y */
/* 以下是生成的 C 代码 */

#line 42 "calc.y"

void yyerror(const char *msg)
{
    /* 错误消息将指向 calc.y:42 */
    fprintf(stderr, "Error at line %d in %s: %s\n", __LINE__, __FILE__, msg);
}

#line 100 "calc.y"

int yylex(void)
{
    /* 词法分析代码 */
    /* 错误消息将指向 calc.y:100 */
    printf("Lexing at %s:%d\n", __FILE__, __LINE__);
    return 0;
}

int main(void)
{
    printf("File: %s, Line: %d\n", __FILE__, __LINE__);
    yylex();
    return 0;
}
```

**输出**：
```
File: calc.y, Line: 114
Lexing at calc.y:100
```

#### 示例 4：使用宏作为参数

```c
#include <stdio.h>

#define NEW_LINE 500
#define NEW_FILE "generated.c"

int main(void)
{
    printf("Before: %s:%d\n", __FILE__, __LINE__);

#line NEW_LINE NEW_FILE
    printf("After: %s:%d\n", __FILE__, __LINE__);

    return 0;
}
```

**输出**：
```
Before: example.c:8
After: generated.c:500
```

#### 示例 5：模板引擎示例

```c
#include <stdio.h>

/* 模拟模板引擎生成的代码 */
/* 原始模板文件: page.template */

#line 1 "page.template"

void render_header(void)
{
    printf("<html><head><title>Page</title></head><body>\n");
}

#line 15 "page.template"

void render_content(void)
{
    printf("<div class=\"content\">\n");
    printf("    <h1>Welcome</h1>\n");
    printf("</div>\n");
}

#line 30 "page.template"

void render_footer(void)
{
    printf("</body></html>\n");
}

int main(void)
{
    render_header();
    render_content();
    render_footer();

    printf("Template executed at %s:%d\n", __FILE__, __LINE__);
    return 0;
}
```

#### 示例 6：条件调试

```c
#include <stdio.h>

#ifdef DEBUG_MODE
    #define DEBUG_LINE 9999
    #define DEBUG_FILE "debug.log"
#else
    #define DEBUG_LINE __LINE__
    #define DEBUG_FILE __FILE__
#endif

void debug_print(const char *msg)
{
#line DEBUG_LINE DEBUG_FILE
    printf("[DEBUG %s:%d] %s\n", __FILE__, __LINE__, msg);
}

int main(void)
{
    debug_print("Starting program");
    printf("Normal output\n");
    debug_print("Ending program");
    return 0;
}
```

### 常见错误及修正

#### 错误示例 1：行号为 0

```c
// 错误：行号为 0 导致未定义行为
#line 0 "file.c"

// 修正：使用有效的行号
#line 1 "file.c"
```

#### 错误示例 2：行号超出范围

```c
// 错误：行号超过 2147483647
#line 9999999999999999999 "file.c"

// 修正：使用有效范围内的行号
#line 1000000 "file.c"
```

#### 错误示例 3：未指定行为

```c
// 未指定行为：#line __LINE__
#line __LINE__  // 行号未指定

// 修正：避免这种用法
#line 100 "file.c"
```

## 7. 总结 (Summary)

### 核心要点

1. **两种语法**：`#line lineno` 和 `#line lineno "filename"`
2. **主要用途**：代码生成工具映射错误到原始文件
3. **影响宏**：修改 `__LINE__` 和 `__FILE__` 的值
4. **行号范围**：C99 起为 1 至 2147483647
5. **未指定行为**：`#line __LINE__` 的行为未指定（DR 464）

### 参数约束

| 参数 | 约束 | 违反约束的后果 |
|------|------|----------------|
| lineno | ≥ 1 且在有效范围内 | 未定义行为 |
| lineno | 必须是十进制数字序列 | 程序非法 |
| filename | 必须是有效字符串 | 程序非法 |

### 使用场景对比

| 场景 | 适用性 | 说明 |
|------|--------|------|
| 代码生成工具 | ✅ 推荐 | 主要设计用途 |
| 手写代码调试 | ⚠️ 谨慎 | 可能造成混淆 |
| 测试框架 | ✅ 可用 | 控制错误消息显示 |
| 生产代码 | ❌ 不推荐 | 应避免使用 |

### 学习建议

1. **理解设计目的**：`#line` 主要用于代码生成工具
2. **避免在手写代码中使用**：除非有特殊需求
3. **注意 DR 464**：`#line __LINE__` 的行为未指定
4. **检查行号范围**：确保行号在有效范围内
5. **保持一致性**：修改行号时同时修改文件名

---

**标准参考**：
- C17 (ISO/IEC 9899:2018): 6.10.4 Line control, J.1 Unspecified behavior
- C11 (ISO/IEC 9899:2011): 6.10.4 Line control
- C99 (ISO/IEC 9899:1999): 6.10.4 Line control
- C89/C90 (ISO/IEC 9899:1990): 3.8.4 Line control