# main 函数 (Main Function)

## 1. 概述 (Overview)

每个在宿主执行环境中运行的 C 程序都包含一个名为 `main` 的函数**定义**（而非原型），该函数是程序的指定入口点。

### main 函数的角色

| 特性 | 说明 |
|-----|------|
| 入口点 | 程序执行的起始位置 |
| 环境类型 | 宿主环境（有操作系统） |
| 定义要求 | 必须是定义，不能只是原型 |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`main` 函数作为程序入口的概念源于 C 语言的 Unix 根源。Unix 操作系统使用 `exec` 系统调用加载程序，并通过约定调用 `main` 函数启动程序。

### 版本演进

| C 标准版本 | 发布年份 | main 函数相关改进 |
|-----------|---------|------------------|
| C89/C90 | 1989/1990 | 标准化 `main` 函数形式，无返回值行为未定义 |
| C99 | 1999 | 允许实现定义的其他签名；无返回语句时隐式返回 0 |
| C23 | 2024 | 沿用 C99 规则 |

### 关键变更

**C99 重要改进**：
- 如果控制流到达函数末尾的 `}` 而未执行 `return`，等同于执行 `return 0;`
- 允许实现定义的其他函数签名

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 函数签名

```c
// 形式 1: 无参数
int main(void) { body }

// 形式 2: 带命令行参数
int main(int argc, char *argv[]) { body }

// 形式 3: 实现定义的其他签名 (C99 起)
/* another implementation-defined signature */
```

### 3.2 参数说明

| 参数 | 类型 | 说明 |
|-----|------|------|
| `argc` | `int` | 非负值，表示传递给程序的参数数量 |
| `argv` | `char*[]` | 指向参数字符串数组的指针 |

#### 参数详解

**argc (Argument Count)**：
- 表示命令行参数的数量
- 非负整数
- 至少为 0（某些系统上至少为 1）

**argv (Argument Vector)**：
- 指向 `argc + 1` 个指针的数组
- `argv[argc]` 保证是空指针 (`NULL`)
- `argv[0]` 到 `argv[argc-1]` 指向参数字符串

#### argv 数组结构

```
argv[0]     --> "program_name"   (程序名，可能为空)
argv[1]     --> "arg1"           (第一个命令行参数)
argv[2]     --> "arg2"           (第二个命令行参数)
...
argv[argc-1]--> "argN"           (最后一个命令行参数)
argv[argc]   = NULL              (空指针终止符)
```

### 3.3 参数命名灵活性

参数名可以自由选择，类型声明也有等效形式：

```c
// 以下都是等效的
int main(int argc, char *argv[])
int main(int argc, char **argv)
int main(int ac, char **av)
int main(int argument_count, char *arguments[])
```

### 3.4 实现定义的扩展形式

常见扩展形式包含环境变量参数：

```c
// 常见扩展：第三参数为环境变量
int main(int argc, char *argv[], char *envp[])
```

| 参数 | 说明 |
|-----|------|
| `envp` | 指向环境变量字符串数组的指针，以 `NULL` 结尾 |

## 4. 底层原理 (Underlying Principles)

### 4.1 程序启动流程

```
程序加载
    ↓
静态存储期对象初始化
    ↓
调用 main 函数
    ↓
程序执行
    ↓
main 返回 → 调用 exit(return_value)
    ↓
执行 atexit 注册的函数
    ↓
刷新并关闭所有流
    ↓
删除 tmpfile 创建的文件
    ↓
控制权返回执行环境
```

### 4.2 main 函数的特殊属性

| 属性 | 说明 |
|-----|------|
| 禁止原型声明 | 程序不能提供 `main` 的原型 |
| 隐式 exit | 首次调用 `main` 的返回等价于调用 `exit()` |
| 递归行为 | 递归调用 `main` 时，返回**不**触发 `exit()` |

### 4.3 返回值的处理

#### C99 之前的规则

```c
// 未定义行为：无返回值的 return
int main(void) {
    return;  // 未定义行为
}

// 未定义行为：到达末尾无 return
int main(void) {
    // ... 没有返回语句
}  // 返回给宿主环境的状态未定义
```

#### C99 及之后的规则

```c
// 隐式返回 0
int main(void) {
    // ... 没有返回语句
}  // 等同于 return 0;

// 非兼容返回类型
void main(void) {  // 返回类型不兼容 int
    // 返回值未指定
}
```

### 4.4 返回值与退出状态

| 返回值 | 宏常量 | 含义 |
|-------|-------|------|
| `0` | `EXIT_SUCCESS` | 成功终止 |
| 非零 | `EXIT_FAILURE` | 失败终止（通常为 1） |

```c
#include <stdlib.h>

int main(void) {
    // 成功终止
    return EXIT_SUCCESS;  // 或 return 0;

    // 失败终止
    return EXIT_FAILURE;
}
```

### 4.5 宿主环境 vs 独立环境

| 特性 | 宿主环境 | 独立环境 |
|-----|---------|---------|
| 入口点名称 | `main` | 实现定义 |
| 入口点类型 | 标准定义 | 实现定义 |
| 典型场景 | 操作系统应用程序 | 引导加载器、操作系统内核、嵌入式系统 |

## 5. 使用场景 (Use Cases)

### 5.1 典型应用场景

| 场景 | 推荐形式 |
|-----|---------|
| 简单程序 | `int main(void)` |
| 命令行工具 | `int main(int argc, char *argv[])` |
| 需要环境变量 | `int main(int argc, char *argv[], char *envp[])`（扩展） |
| 库/模块测试 | 带参数的 main 用于测试配置 |

### 5.2 命令行参数处理最佳实践

```c
#include <stdio.h>
#include <stdlib.h>

void print_usage(const char *program_name) {
    fprintf(stderr, "用法: %s <输入文件> <输出文件>\n", program_name);
}

int main(int argc, char *argv[]) {
    // 检查参数数量
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    // 处理文件...
    printf("输入: %s, 输出: %s\n", input_file, output_file);

    return EXIT_SUCCESS;
}
```

### 5.3 注意事项

1. **argv 可修改性**：
   - 参数字符串可以被修改
   - 修改会持续到程序终止
   - 修改不会传回宿主环境

2. **argv[0] 的不确定性**：
   - 可能是程序名、空字符串或 `NULL`
   - 不应依赖其值

3. **字符大小写**：
   - 某些环境可能将参数转换为小写

4. **环境变量访问**：
   - 可移植方式：使用 `getenv()`
   - 扩展方式：`envp` 参数或 `environ` 全局变量

## 6. 代码示例 (Examples)

### 6.1 基本用法：打印命令行参数

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("argc = %d\n", argc);

    for (int i = 0; i < argc; ++i) {
        printf("argv[%d] --> \"%s\"\n", i, argv[i]);
    }

    // 验证 argv[argc] == NULL
    printf("argv[%d] = %p\n", argc, (void*)argv[argc]);

    return 0;
}
```

**执行示例**：
```bash
$ ./a.out hello world
argc = 3
argv[0] --> "./a.out"
argv[1] --> "hello"
argv[2] --> "world"
argv[3] = (nil)
```

### 6.2 命令行选项解析

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int verbose = 0;
    const char *filename = NULL;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("用法: %s [-v|--verbose] <文件>\n", argv[0]);
            return 0;
        } else if (argv[i][0] != '-') {
            filename = argv[i];
        }
    }

    if (filename == NULL) {
        fprintf(stderr, "错误: 未指定文件\n");
        return EXIT_FAILURE;
    }

    if (verbose) {
        printf("处理文件: %s\n", filename);
    }

    // 处理文件...

    return EXIT_SUCCESS;
}
```

### 6.3 使用 strtok 解析参数

```c
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // 演示修改 argv 字符串
    for (int i = 1; i < argc; ++i) {
        printf("原始参数: %s\n", argv[i]);

        // 使用 strtok 分割（会修改字符串）
        char *token = strtok(argv[i], ",;:");

        while (token != NULL) {
            printf("  token: %s\n", token);
            token = strtok(NULL, ",;:");
        }
    }

    return 0;
}
```

### 6.4 访问环境变量（可移植方式）

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // 可移植的环境变量访问方式
    const char *path = getenv("PATH");
    const char *home = getenv("HOME");
    const char *user = getenv("USER");

    if (path) {
        printf("PATH: %s\n", path);
    }
    if (home) {
        printf("HOME: %s\n", home);
    }
    if (user) {
        printf("USER: %s\n", user);
    }

    return 0;
}
```

### 6.5 访问环境变量（扩展方式）

```c
#include <stdio.h>

// 扩展形式：使用 envp 参数
int main(int argc, char *argv[], char *envp[]) {
    printf("环境变量列表:\n");

    for (char **env = envp; *env != NULL; ++env) {
        printf("  %s\n", *env);
    }

    return 0;
}
```

### 6.6 递归调用 main

```c
#include <stdio.h>

// 演示递归调用 main
// 注意：实际编程中应避免这种模式
int main(void) {
    static int count = 0;

    if (count >= 5) {
        printf("完成！\n");
        return 0;  // 递归返回，不触发 exit
    }

    printf("count = %d\n", count++);
    return main();  // 递归调用
}
```

### 6.7 常见错误及修正

#### 错误示例 1：错误的返回类型

```c
// 错误：使用 void 返回类型
#include <stdio.h>

void main(void) {  // 非标准！
    printf("Hello\n");
    // 返回值未指定
}
```

**正确做法**：

```c
#include <stdio.h>

int main(void) {  // 标准形式
    printf("Hello\n");
    return 0;  // 明确返回值
}
```

#### 错误示例 2：未检查参数数量

```c
// 错误：假设参数存在
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // 危险：argv[1] 可能不存在！
    printf("参数: %s\n", argv[1]);  // 可能崩溃

    return 0;
}
```

**正确做法**：

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "用法: %s <参数>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("参数: %s\n", argv[1]);
    return EXIT_SUCCESS;
}
```

#### 错误示例 3：依赖 argv[0]

```c
// 错误：假设 argv[0] 是有效的程序名
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // 危险：argv[0] 可能为 NULL 或空字符串
    char *name = strrchr(argv[0], '/');  // 可能崩溃
    printf("程序名: %s\n", name ? name + 1 : argv[0]);

    return 0;
}
```

**正确做法**：

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    const char *program_name = "未知程序";

    if (argc > 0 && argv[0] != NULL && argv[0][0] != '\0') {
        program_name = argv[0];
    }

    printf("程序名: %s\n", program_name);
    return 0;
}
```

#### 错误示例 4：C89 中缺少返回语句

```c
// C89 中错误：未定义行为
#include <stdio.h>

int main(void) {
    printf("Hello\n");
    // 缺少 return 语句
}  // C89: 未定义行为；C99+: 隐式 return 0
```

**正确做法（兼容所有标准）**：

```c
#include <stdio.h>

int main(void) {
    printf("Hello\n");
    return 0;  // 显式返回
}
```

## 7. 总结 (Summary)

### 7.1 核心要点

1. **标准签名**：
   - `int main(void)`
   - `int main(int argc, char *argv[])`
   - C99 起：实现定义的其他签名

2. **参数含义**：
   - `argc`：参数计数（非负）
   - `argv`：参数向量（以 `NULL` 结尾的字符串指针数组）

3. **返回值**：
   - `0` / `EXIT_SUCCESS`：成功
   - 非零 / `EXIT_FAILURE`：失败
   - C99 起：缺少 return 隐式返回 0

4. **特殊属性**：
   - 禁止原型声明
   - 首次返回等价于 `exit()`
   - 递归调用时返回不等价于 `exit()`

### 7.2 返回值行为对比

| 情况 | C89/C90 | C99+ |
|-----|---------|------|
| `return 0;` | 成功终止 | 成功终止 |
| 无 return 语句 | 未定义行为 | 隐式 `return 0;` |
| `void main(void)` | 返回值未指定 | 返回值未指定（非标准） |
| `return;`（无值） | 未定义行为 | 未定义行为 |

### 7.3 与 C++ 的对比

| 特性 | C | C++ |
|-----|---|-----|
| 无 return 语句 | C99+ 隐式返回 0 | 始终隐式返回 0 |
| 标准签名 | 两种 + 实现定义 | 两种 + 实现定义 |
| `void main` | 非标准 | 非标准 |

### 7.4 学习建议

1. **始终使用标准签名**：
   - 无参数时用 `int main(void)` 而非 `int main()`
   - 需要命令行参数时用 `int main(int argc, char *argv[])`

2. **始终显式返回值**：
   - 即使 C99 允许省略，显式返回更清晰
   - 使用 `EXIT_SUCCESS` 和 `EXIT_FAILURE` 提高可读性

3. **正确处理参数**：
   - 检查 `argc` 再访问 `argv` 元素
   - 不要依赖 `argv[0]` 的值

4. **可移植性考虑**：
   - 避免使用 `void main`
   - 使用 `getenv()` 而非 `envp` 参数访问环境变量

---

**参考资源**：
- ISO/IEC 9899:2024 (C23 标准) 5.1.2.2.1
- `<stdlib.h>` - `EXIT_SUCCESS`, `EXIT_FAILURE`, `exit()`, `atexit()`, `getenv()`
- `<string.h>` - 字符串处理函数