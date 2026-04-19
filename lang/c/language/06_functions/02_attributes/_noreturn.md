# _Noreturn 函数说明符 (C11 起, C23 起弃用)

## 1. 概述

`_Noreturn` 是 C 语言中的一个函数说明符（function specifier），用于声明函数不会返回到其调用点。该关键字在 C11 标准中引入，用于告知编译器被声明的函数不会正常返回，从而允许编译器进行特定的优化并产生警告信息。

`_Noreturn` 通常通过头文件 `<stdnoreturn.h>` 中提供的便捷宏 `noreturn` 来使用，以提高代码可读性。

## 2. 来源与演变

### 历史背景

在 `_Noreturn` 引入之前，开发者难以向编译器表达"此函数不会返回"的语义信息。这导致：

1. **编译器无法优化**：编译器需要为函数调用后的代码保留执行路径
2. **静态分析困难**：工具难以检测控制流问题
3. **警告信息不足**：编译器可能对"不可达代码"产生误报

### C11 标准引入

`_Noreturn` 在 **C11 标准**（ISO/IEC 9899:2011）中正式引入，作为函数说明符使用。设计目的：

- 提供标准化的方式声明不返回函数
- 支持编译器优化（如消除死代码、优化寄存器使用）
- 改善静态分析工具的控制流分析能力

### C23 标准变更

**重要变更**：在 **C23 标准**（ISO/IEC 9899:2024）中：

- `_Noreturn` 函数说明符被标记为**弃用（deprecated）**
- 推荐使用 C++ 风格的 `[[noreturn]]` 属性替代
- 宏 `noreturn` 也被标记为弃用

这一变更是为了与 C++ 标准保持一致性，C++11 引入了 `[[noreturn]]` 属性。

### 版本对比

| 标准 | 状态 | 推荐写法 |
|------|------|----------|
| C11 | 新增 | `_Noreturn void func()` 或 `noreturn void func()` |
| C17 | 保留 | 同上 |
| C23 | 弃用 | `[[noreturn]] void func()` |

## 3. 语法与参数

### 基本语法

```c
_Noreturn 函数声明
```

### 语法位置

`_Noreturn` 关键字出现在函数声明中，位于函数返回类型之前：

```c
_Noreturn void function_name(parameters);
```

### 便捷宏语法

通过 `<stdnoreturn.h>` 头文件，可以使用更易读的 `noreturn` 宏：

```c
#include <stdnoreturn.h>

noreturn void function_name(parameters);
```

### 参数说明

`_Noreturn` 是一个函数说明符，不接受任何参数。

### 使用规则

1. **可重复出现**：`_Noreturn` 可以在同一函数声明中出现多次，效果等同于出现一次
2. **位置要求**：必须出现在函数声明中，不能出现在函数定义的函数体内
3. **行为要求**：被声明的函数必须确实不返回，否则行为未定义

### C23 新语法

从 C23 开始，推荐使用属性语法：

```c
[[noreturn]] void function_name(parameters);
```

## 4. 底层原理

### 编译器优化机制

当编译器遇到 `_Noreturn` 声明时，会进行以下优化：

1. **消除死代码**：编译器可以安全地移除函数调用后的代码
2. **优化寄存器使用**：无需保存函数调用后的返回地址和寄存器状态
3. **控制流分析**：改善静态分析工具的控制流图构建

### 内存布局影响

```c
// 无 _Noreturn 声明时，编译器需要准备返回路径
void normal_func(void) {
    // 函数体
    // 编译器生成返回指令
}

// 有 _Noreturn 声明时，编译器可以省略返回相关代码
_Noreturn void exit_func(void) {
    exit(1);  // 永不返回
    // 无需生成返回指令
}
```

### 未定义行为

如果声明为 `_Noreturn` 的函数实际上返回了（通过执行 return 语句或到达函数体末尾），行为是**未定义的**。编译器可以：

- 产生警告信息（推荐但不强制）
- 生成任意代码
- 导致程序崩溃或不可预测行为

### 可能的返回方式

声明为 `_Noreturn` 的函数可能通过以下方式"返回"：

1. **longjmp**：通过 `longjmp()` 跳转到 setjmp 保存的执行点
2. **程序终止**：调用 `exit()`, `abort()` 等终止函数
3. **异常机制**：抛出异常（C++ 环境）

## 5. 使用场景

### 适用场景

| 场景 | 说明 | 示例函数 |
|------|------|----------|
| 程序终止函数 | 结束程序执行 | `exit()`, `abort()`, `_Exit()` |
| 错误处理函数 | 遇到致命错误后终止 | 自定义错误处理函数 |
| 异常处理 | 实现异常机制 | `longjmp()` |
| 线程退出 | 终止当前线程 | `thrd_exit()` |
| 无限循环 | 函数包含无限循环 | 嵌入式系统主循环 |

### 标准库中的使用

C 标准库中声明为 `noreturn` 的函数：

| 函数 | 头文件 | 说明 |
|------|--------|------|
| `abort()` | `<stdlib.h>` | 异常终止程序 |
| `exit()` | `<stdlib.h>` | 正常终止程序 |
| `_Exit()` | `<stdlib.h>` | 快速终止程序 |
| `quick_exit()` | `<stdlib.h>` | 快速退出程序 |
| `thrd_exit()` | `<threads.h>` | 退出线程 |
| `longjmp()` | `<setjmp.h>` | 非本地跳转 |

### 最佳实践

1. **使用便捷宏**：优先使用 `<stdnoreturn.h>` 中的 `noreturn` 宏，提高可读性
2. **确保不返回**：确保函数确实不会返回，否则会导致未定义行为
3. **C23 使用属性**：在新代码中使用 `[[noreturn]]` 属性替代 `_Noreturn`
4. **文档说明**：在函数注释中说明为什么函数不返回

### 常见陷阱

1. **误用于可能返回的函数**：如果函数在某些条件下可能返回，不应使用 `_Noreturn`
2. **忽略弃用警告**：C23 标准中 `_Noreturn` 已弃用，应迁移到 `[[noreturn]]`
3. **混淆声明位置**：`_Noreturn` 用于声明，不是用于函数体

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

// 使用 noreturn 宏声明不返回函数
noreturn void exit_now(int i)
{
    if (i > 0) {
        exit(i);
    }
    // 如果 i <= 0，行为未定义！
}

int main(void)
{
    puts("Preparing to exit...");
    exit_now(2);
    puts("This code is never executed.");
    return 0;
}
```

输出：
```
Preparing to exit...
```

### 正确用法示例

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

// 错误处理函数 - 永不返回
noreturn void fatal_error(const char *message)
{
    fprintf(stderr, "Fatal error: %s\n", message);
    exit(EXIT_FAILURE);
}

// 无限循环函数 - 永不返回
noreturn void event_loop(void)
{
    while (1) {
        process_events();  // 处理事件
    }
    // 永远不会到达这里
}

int main(void)
{
    int status = initialize();
    if (status != 0) {
        fatal_error("Initialization failed");
    }

    event_loop();  // 永不返回
    return 0;       // 永远不会执行
}
```

### C23 推荐用法

```c
#include <stdio.h>
#include <stdlib.h>

// C23 推荐的属性语法
[[noreturn]] void terminate_program(int code)
{
    printf("Terminating with code %d\n", code);
    exit(code);
}

int main(void)
{
    terminate_program(0);
    return 0;  // 永远不会执行
}
```

### 常见错误及修正

#### 错误 1：函数实际返回

```c
#include <stdnoreturn.h>

// 错误：函数可能返回
noreturn void bad_func(int x)
{
    if (x > 0) {
        exit(1);
    }
    // 如果 x <= 0，函数会返回！未定义行为
}

// 修正：确保所有路径都不返回
noreturn void good_func(int x)
{
    if (x > 0) {
        exit(1);
    } else {
        abort();  // 确保所有路径都不返回
    }
}
```

#### 错误 2：忘记包含头文件

```c
// 错误：未包含 stdnoreturn.h，noreturn 未定义
noreturn void func(void);  // 编译错误

// 修正：包含正确头文件
#include <stdnoreturn.h>
noreturn void func(void);  // 正确

// 或使用原生关键字
_Noreturn void func(void);  // 不需要头文件
```

#### 错误 3：混用 C 和 C++ 语法

```c
// 在 C11/C17 代码中
// 错误：C11 不支持属性语法
[[noreturn]] void func(void);  // C11 不支持

// 正确：使用 _Noreturn 或 noreturn 宏
_Noreturn void func(void);     // C11 正确写法
// 或
#include <stdnoreturn.h>
noreturn void func(void);      // C11 正确写法
```

## 7. 总结

### 核心要点

1. **语义表达**：`_Noreturn` 用于声明不会返回的函数，提供明确的语义信息
2. **编译器优化**：允许编译器进行死代码消除和寄存器优化
3. **版本演进**：C11 引入，C23 弃用，推荐使用 `[[noreturn]]` 属性

### 技术对比

| 特性 | `_Noreturn` (C11) | `[[noreturn]]` (C23) |
|------|-------------------|---------------------|
| 语法类型 | 函数说明符 | 属性 |
| 头文件要求 | `noreturn` 宏需要 `<stdnoreturn.h>` | 无需头文件 |
| C++ 兼容 | 不兼容 | 与 C++11+ 兼容 |
| 当前状态 | C23 弃用 | 推荐使用 |

### 学习建议

1. **新项目**：直接使用 C23 的 `[[noreturn]]` 属性语法
2. **维护旧代码**：了解 `_Noreturn` 和 `noreturn` 宏的用法
3. **跨语言开发**：C/C++ 混合项目中优先使用 `[[noreturn]]` 保持兼容
4. **调试技巧**：利用编译器警告检测 `_Noreturn` 函数的实际返回行为

### 相关概念

| 概念 | 说明 |
|------|------|
| `[[noreturn]]` | C23/C++ 属性，替代 `_Noreturn` |
| `[[_Noreturn]]` | C23 中已弃用的属性形式 |
| `stdnoreturn.h` | 提供 `noreturn` 宏的头文件 |
| `setjmp`/`longjmp` | 非本地跳转机制，常与 `_Noreturn` 配合使用 |

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.7.4 Function specifiers, 7.23 _Noreturn <stdnoreturn.h>
- C17 标准 (ISO/IEC 9899:2018): 6.7.4 Function specifiers, 7.23 _Noreturn <stdnoreturn.h>
- C11 标准 (ISO/IEC 9899:2011): 6.7.4 Function specifiers, 7.23 _Noreturn <stdnoreturn.h>