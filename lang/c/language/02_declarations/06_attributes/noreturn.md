# C 属性：noreturn, _Noreturn (C23 起)

## 1. 概述 (Overview)

`noreturn` 是 C 语言中的一个函数属性（attribute），用于指示函数不会返回到调用者。当一个函数被标记为 `noreturn` 属性时，编译器可以据此进行优化，并可能在函数意外返回时发出警告。

该属性的主要用途包括：
- 帮助编译器进行代码优化（如消除不必要的返回后代码）
- 提供更好的静态分析能力
- 避免函数末尾的"控制流到达非 void 函数末尾"警告
- 明确表达程序员的意图，提高代码可读性

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C 语言早期版本中，没有标准的方式指示函数不会返回。程序员只能通过注释或文档来说明这一行为，编译器无法利用这些信息进行优化。

### C11 标准引入

C11 标准首次引入了 `_Noreturn` 函数说明符（function specifier），这是一个关键字形式的语法：

```c
_Noreturn void my_exit(void);
```

### C23 标准变更

从 C23 标准开始，`_Noreturn` 关键字被废弃，改用属性语法 `[[noreturn]]`。这一变更的原因包括：

1. **统一属性语法**：C23 引入了通用属性语法框架，`noreturn` 作为属性更加一致
2. **与 C++ 兼容**：C++11 已采用 `[[noreturn]]` 属性语法，C23 保持一致性
3. **减少关键字污染**：使用属性语法避免增加新的保留关键字

### 版本对比

| 标准 | 语法 | 状态 |
|------|------|------|
| C11 | `_Noreturn` | 已废弃 |
| C23 | `[[noreturn]]` | 推荐使用 |
| C23 | `[[_Noreturn]]` | 已废弃 |

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```c
[[noreturn]]
[[__noreturn__]]
[[_Noreturn]]      // 已废弃 (C23 起)
[[___Noreturn__]]  // 已废弃 (C23 起)
```

### 语法说明

| 语法 | 说明 |
|------|------|
| `[[noreturn]]` | 标准属性形式，推荐使用 |
| `[[__noreturn__]]` | 带双下划线的替代形式，避免与宏冲突 |
| `[[_Noreturn]]` | 旧式属性形式，已废弃，仅为向后兼容保留 |
| `[[___Noreturn__]]` | 带双下划线的旧式形式，已废弃 |

### 参数说明

`noreturn` 属性不接受任何参数，它是一个无参数的属性。

### 使用位置

该属性应用于函数声明，通常放置在函数声明或定义之前：

```c
[[noreturn]] void fatal_error(const char *message);
```

## 4. 底层原理 (Underlying Principles)

### 实现机制

当编译器遇到 `noreturn` 属性时，可以进行以下优化：

1. **消除死代码**：函数调用后的代码可以安全删除
2. **优化控制流**：编译器知道函数不会返回，可以简化控制流图
3. **寄存器分配优化**：调用 `noreturn` 函数后不需要保存调用者的寄存器状态
4. **警告抑制**：避免在函数末尾产生"控制流到达非 void 函数末尾"的警告

### 行为约束

根据标准规定：

- 标记为 `noreturn` 的函数如果实际返回（通过 `return` 语句或执行到函数体末尾），行为是未定义的（undefined behavior）
- 函数仍可以通过其他方式"退出"：
  - 调用 `longjmp()`
  - 调用其他 `noreturn` 函数
  - 抛出异常（C++ 中）

### 编译器行为

如果编译器能够检测到 `noreturn` 函数实际上返回了，建议发出诊断信息（但不是必须的）。

## 5. 使用场景 (Use Cases)

### 适用场景

`noreturn` 属性适用于以下类型的函数：

1. **终止程序函数**：如 `abort()`、`exit()`、`_Exit()`
2. **错误处理函数**：遇到致命错误时终止程序
3. **无限循环函数**：设计为永远运行的函数
4. **异常处理函数**：抛出异常或进行非局部跳转

### 标准库应用

以下 C 标准库函数声明了 `noreturn` 属性（C23 之前使用 `_Noreturn` 说明符）：

| 函数 | 头文件 | 说明 |
|------|--------|------|
| `abort()` | `<stdlib.h>` | 异常终止程序 |
| `exit()` | `<stdlib.h>` | 正常终止程序 |
| `_Exit()` | `<stdlib.h>` | 快速退出程序 |
| `quick_exit()` | `<stdlib.h>` | 快速退出程序 |
| `thrd_exit()` | `<threads.h>` | 退出线程 |
| `longjmp()` | `<setjmp.h>` | 非局部跳转 |

### 最佳实践

1. **仅在确实不返回的函数上使用**：确保函数真的不会返回
2. **文档化退出方式**：在注释中说明函数如何终止执行
3. **避免误用**：不要在可能正常返回的函数上使用

### 注意事项

- 如果 `noreturn` 函数实际上返回，将导致未定义行为
- 即使标记为 `noreturn`，函数仍需要（语法上）有返回类型（通常是 `void`）
- 编译器可能不会检测所有违规情况

### 常见陷阱

1. **忘记调用退出函数**：函数标记为 `noreturn` 但忘记调用 `exit()` 或类似的终止函数
2. **条件返回**：函数在某些条件下可能返回
3. **异常路径遗漏**：未处理所有可能的执行路径

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>
#include <stdlib.h>

// 正确用法：函数确实不会返回
[[noreturn]] void fatal_error(const char *message) {
    fprintf(stderr, "Fatal error: %s\n", message);
    exit(1);
}

int main(void) {
    int value = -1;

    if (value < 0) {
        fatal_error("Value cannot be negative");
        // 下面的代码会被编译器优化掉（死代码消除）
        printf("This will never be printed\n");
    }

    return 0;
}
```

### 无限循环函数

```c
#include <stdio.h>

// 服务器主循环 - 设计为永远运行
[[noreturn]] void server_loop(void) {
    while (1) {
        // 处理客户端请求
        printf("Waiting for connections...\n");
        // ... 服务器逻辑 ...
    }
    // 永远不会到达这里
}

int main(void) {
    server_loop();
    // 不会执行到这里
}
```

### 带条件检查的退出函数

```c
#include <stdio.h>
#include <stdlib.h>

[[noreturn]] void check_allocation(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    // 正确：函数调用 exit() 后不会返回
}

// 或者更常见的模式：
[[noreturn]] void allocation_failed(void) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
}

int main(void) {
    int *data = malloc(sizeof(int) * 1000000000);

    if (data == NULL) {
        allocation_failed();
    }

    // 正常逻辑
    free(data);
    return 0;
}
```

### 使用 longjmp 退出

```c
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>

jmp_buf error_handler;

// noreturn 函数可以通过 longjmp 退出
[[noreturn]] void handle_error(void) {
    printf("Error occurred, jumping back...\n");
    longjmp(error_handler, 1);
    // 不会执行到这里
}

int main(void) {
    if (setjmp(error_handler) == 0) {
        // 正常执行路径
        printf("Starting operation...\n");
        handle_error();
    } else {
        // 错误处理路径
        printf("Recovered from error\n");
    }

    return 0;
}
```

### C11 兼容写法（已废弃）

```c
// C11 风格（已废弃，仅为向后兼容）
_Noreturn void old_style_exit(void) {
    exit(1);
}

// C23 推荐风格
[[noreturn]] void new_style_exit(void) {
    exit(1);
}
```

### 常见错误及修正

#### 错误示例 1：noreturn 函数实际返回

```c
// 错误：函数标记为 noreturn 但实际上可能返回
[[noreturn]] void bad_example(int condition) {
    if (condition) {
        exit(1);
    }
    // 错误：如果 condition 为假，函数会返回
    // 这导致未定义行为！
}
```

#### 修正：确保所有路径都退出

```c
// 正确：所有执行路径都会退出
[[noreturn]] void good_example(int condition) {
    if (condition) {
        exit(1);
    } else {
        exit(2);
    }
    // 永远不会到达这里
}
```

#### 错误示例 2：忘记调用退出函数

```c
// 错误：忘记调用退出函数
[[noreturn]] void forgot_exit(const char *msg) {
    printf("Error: %s\n", msg);
    // 忘记调用 exit() 或 abort()
    // 函数会返回，导致未定义行为！
}
```

#### 修正：确保调用退出函数

```c
// 正确：确保调用退出函数
[[noreturn]] void correct_exit(const char *msg) {
    printf("Error: %s\n", msg);
    exit(1);  // 确保函数不会返回
}
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| **用途** | 指示函数不会返回到调用者 |
| **C11 语法** | `_Noreturn` 关键字（已废弃） |
| **C23 语法** | `[[noreturn]]` 属性（推荐） |
| **优化效果** | 死代码消除、控制流简化、寄存器优化 |
| **违规后果** | 未定义行为（undefined behavior） |

### 与其他语言特性对比

| 特性 | C 语言 | C++ 语言 |
|------|--------|----------|
| 属性语法 | `[[noreturn]]` (C23) | `[[noreturn]]` (C++11) |
| 关键字形式 | `_Noreturn` (C11，已废弃) | 无 |
| C 栈展开 | 不支持 | 支持 |

### 学习建议

1. **优先使用 C23 语法**：使用 `[[noreturn]]` 而非废弃的 `_Noreturn`
2. **理解语义**：`noreturn` 表示函数不会通过正常方式返回
3. **合理使用**：仅在函数确实不会返回时使用此属性
4. **测试覆盖**：确保所有执行路径都正确处理

### 参考资源

- C23 标准 (ISO/IEC 9899:2023)
- C11 标准 (ISO/IEC 9899:2011)
- cppreference: https://en.cppreference.com/w/c/language/attributes/noreturn