# switch 语句 (switch statement)

## 1. 概述 (Overview)

**switch 语句**是 C 语言中根据整数值选择执行多个代码分支之一的多路选择语句。

### 核心概念

- **多路分支**：根据一个整数值从多个分支中选择执行
- **case 标签**：标记每个分支的入口点，后跟常量表达式
- **default 标签**：当所有 case 都不匹配时的默认执行分支
- **fall-through**：执行完一个 case 后继续执行下一个 case（除非遇到 break）

### 技术定位

switch 语句属于 C 语言的**选择语句**（selection statements）类别，与 if 语句共同构成条件控制结构。switch 语句特别适合处理多个离散值的分支选择，编译器可能将其优化为跳转表（jump table）以提高性能。

### 适用场景

- 根据枚举值执行不同操作
- 处理命令解析、状态机
- 需要根据整数离散值进行多分支选择的场景

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

switch 语句源自 BCPL 和 B 语言，设计动机是提供一种高效的多路分支选择机制，避免使用多层嵌套的 if-else 语句，同时允许编译器进行跳转表优化。

### 版本变更

| 标准 | 章节 | 主要变更 |
|------|------|----------|
| C89/C90 | 3.6.4.2 | 初始标准化 |
| C99 | 6.8.4.2 | 引入块作用域规则，VLA 约束 |
| C11 | 6.8.4.2 | 保持不变 |
| C17 | 6.8.4.2 | 保持不变 |
| C23 | 6.8.5.3 | 新增属性说明符序列（`attr-spec-seq`）支持，case/default 语句可选 |

### C23 新特性

1. **属性支持**：switch 语句和 case/default 标签可以添加属性说明符序列
2. **语句可选**：case 和 default 后的语句变为可选（允许空标签）

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

**switch 语句：**
```c
attr-spec-seq(可选) switch ( expression ) statement
```

**case 标签：**
```c
// C23 之前
case constant-expression : statement

// C23 起
attr-spec-seq(可选) case constant-expression : statement(可选)
```

**default 标签：**
```c
// C23 之前
default : statement

// C23 起
attr-spec-seq(可选) default : statement(可选)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr-spec-seq` | (C23 起) 可选的属性列表，应用于 switch 语句或标签 |
| `expression` | 任意整数类型表达式（char、有符号/无符号整数、枚举） |
| `statement` | 任意语句（通常是复合语句），内部可包含 case/default 标签 |
| `constant-expression` | 整数常量表达式 |

### 表达式要求

- **类型限制**：必须是整数类型（包括字符类型和枚举类型）
- **求值规则**：表达式在 switch 语句执行时求值一次

### case 标签约束

1. 所有 case 后的常量表达式值必须唯一（转换到提升后的表达式类型后）
2. 常量表达式必须是编译期可确定的整数值
3. 每个 switch 最多只能有一个 default 标签

---

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
┌─────────────────────────────────────┐
│         求值 expression             │
└─────────────────┬───────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│    查找匹配的 case 常量表达式        │
└─────────────────┬───────────────────┘
                  │
        ┌─────────┴─────────┐
        │                   │
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│   找到匹配    │   │  未找到匹配   │
│   跳转至该    │   │               │
│   case 标签   │   └───────┬───────┘
└───────┬───────┘           │
        │           ┌───────┴───────┐
        │           │               │
        │           ▼               ▼
        │   ┌───────────────┐ ┌───────────────┐
        │   │ 有 default    │ │ 无 default    │
        │   │ 跳转至 default│ │ 不执行任何语句│
        │   └───────┬───────┘ └───────────────┘
        │           │
        └───────────┴───────────┐
                                  │
                                  ▼
                    ┌─────────────────────────┐
                    │ 顺序执行直到 break 或    │
                    │ switch 语句结束          │
                    └─────────────────────────┘
```

### Fall-through 机制

switch 语句的一个重要特性是 **fall-through**（贯穿执行）：

- 执行完一个 case 分支后，程序会继续执行下一个 case 分支
- 除非遇到 `break` 语句或到达 switch 语句末尾
- 这是 C 语言有意为之的设计，允许多个 case 共享代码

```c
switch(1) {
    case 1 : puts("1"); // 打印 "1"
    case 2 : puts("2"); // 然后打印 "2"（fall-through）
}

switch(1) {
    case 1 : puts("1"); // 打印 "1"
             break;     // 退出 switch
    case 2 : puts("2");
             break;
}
```

### 编译器优化

编译器通常使用以下技术优化 switch 语句：

| 优化技术 | 适用场景 | 时间复杂度 |
|----------|----------|------------|
| 跳转表（Jump Table） | case 值连续或密集 | O(1) |
| 二分查找 | case 值稀疏但有序 | O(log n) |
| 线性查找 | case 数量较少 | O(n) |

### 块作用域（C99 起）

switch 语句建立独立的块作用域：

```c
switch (int x = expr)  // x 在此声明
{
    // switch 体
}
// x 在此超出作用域
```

### VLA 约束（C99 起）

变长数组（VLA）或可变修改类型（variably-modified type）的标识符，如果有 case 或 default 标签在其作用域内，则整个 switch 语句必须在其作用域内：

```c
switch (expr)
{
        int i = 4;   // 非 VLA，可以在此声明
        f(i);        // 永远不会被调用
//      int a[i];    // 错误：VLA 不能在此声明
    case 0:
        i = 17;
    default:
        int a[i];    // VLA 可以在此声明
        printf("%d\n", i);
}
```

---

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 状态机实现 | 根据状态值执行对应处理 |
| 命令解析 | 根据命令类型分发处理 |
| 消息处理 | 根据消息类型选择处理逻辑 |
| 枚举值处理 | 根据枚举值执行不同操作 |

### 最佳实践

1. **始终使用 break**：除非有意使用 fall-through，否则每个 case 都应以 break 结尾
2. **添加 default 分支**：处理意外情况，提高代码健壮性
3. **使用枚举类型**：提高代码可读性和类型安全性
4. **标注 fall-through**：有意使用 fall-through 时添加注释

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 忘记 break | 无意中造成 fall-through | 每个 case 后添加 break |
| 忘记 default | 未处理意外情况 | 始终添加 default 分支 |
| 变量初始化 | 跳过变量初始化代码 | 使用复合语句块 |
| VLA 声明位置 | 在 case 前声明 VLA | 将 VLA 声明移至 case 后 |
| 非常量表达式 | case 使用非常量值 | 使用编译期常量 |

---

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

void func(int x)
{
    printf("func(%d): ", x);
    switch(x)
    {
        case 1: printf("case 1, ");
        case 2: printf("case 2, ");
        case 3: printf("case 3.\n"); break;
        case 4: printf("case 4, ");
        case 5:
        case 6: printf("case 5 or case 6, ");
        default: printf("default.\n");
    }
}

int main(void)
{
    for(int i = 1; i < 9; ++i) func(i);
    return 0;
}
```

**输出：**
```
func(1): case 1, case 2, case 3.
func(2): case 2, case 3.
func(3): case 3.
func(4): case 4, case 5 or case 6, default.
func(5): case 5 or case 6, default.
func(6): case 5 or case 6, default.
func(7): default.
func(8): default.
```

### 正确用法示例

```c
#include <stdio.h>

typedef enum {
    CMD_START,
    CMD_STOP,
    CMD_PAUSE,
    CMD_RESUME
} Command;

void process_command(Command cmd)
{
    switch (cmd)
    {
        case CMD_START:
            printf("Starting...\n");
            break;

        case CMD_STOP:
            printf("Stopping...\n");
            break;

        case CMD_PAUSE:
            printf("Pausing...\n");
            break;

        case CMD_RESUME:
            printf("Resuming...\n");
            break;

        default:
            printf("Unknown command: %d\n", cmd);
            break;
    }
}

int main(void)
{
    process_command(CMD_START);
    process_command(CMD_STOP);
    return 0;
}
```

### 多 case 共享代码

```c
#include <stdio.h>
#include <ctype.h>

int is_vowel(int c)
{
    switch (tolower(c))
    {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            return 1;  // 有意使用 fall-through
        default:
            return 0;
    }
}

int main(void)
{
    char test[] = "Hello World";
    for (int i = 0; test[i]; i++)
    {
        if (is_vowel(test[i]))
            printf("%c is a vowel\n", test[i]);
    }
    return 0;
}
```

### 变量声明与初始化

```c
#include <stdio.h>

int main(void)
{
    int x = 2;

    switch (x)
    {
        case 1:
        {
            int value = 10;  // 使用块限制作用域
            printf("value = %d\n", value);
            break;
        }
        case 2:
        {
            int value = 20;
            printf("value = %d\n", value);
            break;
        }
        default:
            printf("default case\n");
            break;
    }

    // value 在此不可见
    return 0;
}
```

### 常见错误及修正

```c
#include <stdio.h>

int main(void)
{
    int x = 1;

    // 错误 1：忘记 break
    printf("Error 1 - Missing break:\n");
    switch (x)
    {
        case 1:
            printf("  case 1\n");
            // 忘记 break！
        case 2:
            printf("  case 2 (unexpected!)\n");
            break;
    }

    // 正确写法
    printf("Correct 1:\n");
    switch (x)
    {
        case 1:
            printf("  case 1\n");
            break;  // 添加 break
        case 2:
            printf("  case 2\n");
            break;
    }

    // 错误 2：在 case 前声明变量并初始化
    printf("Error 2 - Initialization skipped:\n");
    switch (x)
    {
        int value = 100;  // 这行代码被跳过！
        case 1:
            printf("  value = %d (undefined!)\n", value);  // 未定义行为
            break;
    }

    // 正确写法：在 case 内声明
    printf("Correct 2:\n");
    switch (x)
    {
        case 1:
        {
            int value = 100;  // 在 case 内声明
            printf("  value = %d\n", value);
            break;
        }
    }

    // 错误 3：case 使用非常量
    // int y = 2;
    // switch (x)
    // {
    //     case y:  // 错误：case 必须是常量表达式
    //         break;
    // }

    return 0;
}
```

### 高级用法：状态机

```c
#include <stdio.h>

typedef enum {
    STATE_IDLE,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_DISCONNECTING,
    STATE_ERROR
} State;

typedef enum {
    EVENT_CONNECT,
    EVENT_CONNECTED,
    EVENT_DISCONNECT,
    EVENT_DISCONNECTED,
    EVENT_ERROR
} Event;

State current_state = STATE_IDLE;

void handle_event(Event event)
{
    switch (current_state)
    {
        case STATE_IDLE:
            switch (event)
            {
                case EVENT_CONNECT:
                    printf("Connecting...\n");
                    current_state = STATE_CONNECTING;
                    break;
                default:
                    printf("Invalid event in IDLE state\n");
                    break;
            }
            break;

        case STATE_CONNECTING:
            switch (event)
            {
                case EVENT_CONNECTED:
                    printf("Connected!\n");
                    current_state = STATE_CONNECTED;
                    break;
                case EVENT_ERROR:
                    printf("Connection failed\n");
                    current_state = STATE_ERROR;
                    break;
                default:
                    break;
            }
            break;

        case STATE_CONNECTED:
            switch (event)
            {
                case EVENT_DISCONNECT:
                    printf("Disconnecting...\n");
                    current_state = STATE_DISCONNECTING;
                    break;
                case EVENT_ERROR:
                    printf("Connection error\n");
                    current_state = STATE_ERROR;
                    break;
                default:
                    break;
            }
            break;

        case STATE_DISCONNECTING:
            if (event == EVENT_DISCONNECTED)
            {
                printf("Disconnected\n");
                current_state = STATE_IDLE;
            }
            break;

        case STATE_ERROR:
            switch (event)
            {
                case EVENT_CONNECT:
                    printf("Retrying...\n");
                    current_state = STATE_CONNECTING;
                    break;
                default:
                    break;
            }
            break;
    }
}

int main(void)
{
    handle_event(EVENT_CONNECT);
    handle_event(EVENT_CONNECTED);
    handle_event(EVENT_DISCONNECT);
    handle_event(EVENT_DISCONNECTED);
    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 条件类型 | 整数类型（char、有符号/无符号整数、枚举） |
| case 值 | 必须是唯一的编译期整数常量表达式 |
| default | 最多一个，处理所有不匹配的情况 |
| fall-through | 执行完 case 后继续下一个 case，除非遇到 break |
| 作用域 | C99 起建立独立块作用域 |

### switch vs if-else 对比

| 特性 | switch 语句 | if-else 语句 |
|------|-------------|--------------|
| 条件类型 | 仅整数类型 | 任意标量类型 |
| 分支匹配 | 精确值匹配 | 范围判断灵活 |
| 可读性 | 多分支时更清晰 | 条件复杂时更直观 |
| 性能 | 可能优化为跳转表 | 线性判断 |
| fall-through | 支持 | 不支持 |

### 学习建议

1. **理解 fall-through**：这是 switch 最重要的特性，需要谨慎处理
2. **使用 break**：养成每个 case 都添加 break 的习惯
3. **善用枚举**：使用枚举类型提高代码可读性
4. **注意作用域**：变量声明和初始化要放在正确的位置
5. **编译器优化**：理解 switch 的性能特性，合理使用

---

## 参考资料

- C23 标准 (ISO/IEC 9899:2024): 6.8.5.3 The switch statement
- C17 标准 (ISO/IEC 9899:2018): 6.8.4.2 The switch statement (p: 108-109)
- C11 标准 (ISO/IEC 9899:2011): 6.8.4.2 The switch statement (p: 149-150)
- C99 标准 (ISO/IEC 9899:1999): 6.8.4.2 The switch statement (p: 134-135)
- C89/C90 标准 (ISO/IEC 9899:1990): 3.6.4.2 The switch statement