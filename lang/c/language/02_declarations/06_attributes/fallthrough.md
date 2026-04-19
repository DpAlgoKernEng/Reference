# C 属性：fallthrough（C23 起）

## 1. 概述

`[[fallthrough]]` 是 C23 标准引入的属性（attribute），用于指示 switch 语句中从一个 case 标签到下一个 case 标签的穿透（fallthrough）行为是**有意为之**的，编译器在启用穿透警告时不应对此发出诊断信息。

该属性定义在 `<stdalign.h>` 头文件中，是 C23 标准属性系统的一部分。

### 核心用途

- 消除编译器对有意穿透的警告
- 提高代码可读性和意图表达
- 与静态分析工具配合使用

## 2. 来源与演变

### 首次引入

`[[fallthrough]]` 属性在 **C23** 标准中首次引入。这一特性借鉴自 C++17 标准中的同名属性。

### 历史背景

在 C23 之前，处理 switch 语句中的穿透行为通常有以下方式：

1. **编译器扩展**：如 GCC 的 `__attribute__((fallthrough))`
2. **注释标记**：使用 `/* fall through */` 等注释
3. **禁用警告**：全局禁用穿透警告（不推荐）

C23 标准化该属性后，提供了可移植、统一的解决方案。

### C++ 对应版本

- C++17：首次引入 `[[fallthrough]]` 属性
- C23：从 C++ 借鉴并纳入标准

### 标准化动机

| 方式 | 问题 |
|------|------|
| 注释标记 | 非标准化，不同编译器格式不同 |
| 编译器扩展 | 不可移植 |
| 禁用警告 | 可能遗漏真正的问题 |
| `[[fallthrough]]` | 标准化、可移植、明确意图 |

## 3. 语法与参数

### 语法形式

```c
[[fallthrough]]
[[__fallthrough__]]  // 替代形式，避免与宏冲突
```

### 语法规则

`[[fallthrough]]` 只能作为**穿透声明**（fallthrough declaration）使用：

```c
[[fallthrough]];  // 正确：作为独立声明
```

### 使用限制

| 限制条件 | 说明 |
|----------|------|
| 位置要求 | 必须在 switch 语句内使用 |
| 后续语句要求 | 紧随其后必须是同一 switch 的 case 或 default 标签 |
| 语句形式 | 必须以分号结尾，形成独立声明 |

### 正确使用位置

```c
switch (expr) {
    case 1:
        // ... 代码 ...
        [[fallthrough]];  // 正确：后续是 case 2
    case 2:
        // ...
}
```

## 4. 底层原理

### 编译器处理机制

`[[fallthrough]]` 属性是一个**提示性属性**（hint attribute），它不影响程序的运行时行为：

1. **编译时检查**：编译器验证属性使用的合法性
2. **诊断抑制**：抑制 `-Wimplicit-fallthrough` 类型的警告
3. **语义验证**：确保后续确实存在 case 或 default 标签

### 非法使用检测

编译器会对以下非法使用发出诊断：

```c
switch (n) {
    case 1:
        [[fallthrough]];  // 错误：后续没有 case/default 标签
}
```

### 与编译器警告的配合

| 编译器 | 警告选项 | 行为 |
|--------|----------|------|
| GCC | `-Wimplicit-fallthrough` | 检测无标记的穿透 |
| Clang | `-Wimplicit-fallthrough` | 检测无标记的穿透 |
| MSVC | `/W4` | 检测穿透行为 |

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 多 case 共享代码 | 多个 case 执行相同的处理逻辑 |
| 条件穿透 | 满足特定条件时穿透到下一个 case |
| 状态机实现 | 状态转换中的有意穿透 |

### 最佳实践

1. **仅在有意穿透时使用**：不要滥用此属性掩盖逻辑错误
2. **配合注释**：增强代码可读性
3. **保持一致性**：项目中统一使用标准属性而非编译器扩展

### 注意事项

- `[[fallthrough]]` 本身不执行任何代码，只是一个声明
- 必须紧跟 case 或 default 标签，中间不能有其他语句
- 在循环、条件语句中使用时需要特别注意后续标签的要求

### 常见陷阱

```c
switch (n) {
    case 1:
        if (condition) {
            [[fallthrough]];  // OK：if 结束后是 case 2
        }
    case 2:
        break;
}
```

## 6. 代码示例

### 基础用法

```c
#include <stdbool.h>

void g(void) {}
void h(void) {}
void i(void) {}

void f(int n) {
    switch (n) {
        case 1:
        case 2:           // 多 case 共享代码（无需 fallthrough）
            g();
            [[fallthrough]];  // 明确标记有意穿透
        case 3:           // 编译器不会对此穿透发出警告
            h();
            break;
        case 4:
            break;
    }
}

int main(void) {
    f(2);
    return 0;
}
```

### 条件穿透示例

```c
#include <stdio.h>

void process(int value) {
    switch (value) {
        case 1:
            printf("Case 1\n");
            [[fallthrough]];  // 有意穿透到 case 2
        case 2:
            printf("Case 1 or 2\n");
            break;
        case 3:
            printf("Case 3\n");
            if (value > 0) {
                [[fallthrough]];  // 条件穿透
            }
            break;  // 如果 value <= 0，不会穿透
        case 4:
            printf("Case 3 or 4\n");
            break;
    }
}

int main(void) {
    process(1);  // 输出: Case 1, Case 1 or 2
    process(3);  // 输出: Case 3, Case 3 or 4
    return 0;
}
```

### 常见错误及修正

#### 错误 1：无后续 case 标签

```c
// 错误示例
void wrong1(int n) {
    switch (n) {
        case 1:
            [[fallthrough]];  // 错误：后续没有 case/default 标签
    }
}

// 修正
void correct1(int n) {
    switch (n) {
        case 1:
            [[fallthrough]];
        case 2:  // 必须有后续 case
            break;
    }
}
```

#### 错误 2：在循环中错误使用

```c
// 错误示例
void wrong2(int n) {
    switch (n) {
        case 1:
            while (false) {
                [[fallthrough]];  // 错误：循环内无法穿透到 case
            }
        case 2:
            break;
    }
}
```

#### 错误 3：遗漏标记的有意穿透

```c
// 不推荐：未标记的有意穿透
void not_recommended(int n) {
    switch (n) {
        case 1:
            do_something();
            // 缺少 [[fallthrough]]，编译器可能警告
        case 2:
            do_another_thing();
            break;
    }
}

// 推荐：明确标记
void recommended(int n) {
    switch (n) {
        case 1:
            do_something();
            [[fallthrough]];  // 明确表示有意穿透
        case 2:
            do_another_thing();
            break;
    }
}
```

### 完整示例

```c
#include <stdio.h>
#include <stdbool.h>

// 状态机示例
typedef enum {
    STATE_INIT,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_DISCONNECTING,
    STATE_DISCONNECTED
} State;

void handle_state(State state, bool force) {
    switch (state) {
        case STATE_INIT:
            printf("Initializing...\n");
            [[fallthrough]];  // 初始化后立即开始连接
        case STATE_CONNECTING:
            printf("Connecting...\n");
            // 不穿透，等待连接完成
            break;
        case STATE_CONNECTED:
            printf("Connected.\n");
            if (force) {
                printf("Force disconnect requested.\n");
                [[fallthrough]];  // 强制断开时直接进入断开状态
            } else {
                break;
            }
        case STATE_DISCONNECTING:
            printf("Disconnecting...\n");
            [[fallthrough]];  // 断开连接后进入已断开状态
        case STATE_DISCONNECTED:
            printf("Disconnected.\n");
            break;
    }
}

int main(void) {
    printf("=== Normal flow ===\n");
    handle_state(STATE_INIT, false);

    printf("\n=== Force disconnect ===\n");
    handle_state(STATE_CONNECTED, true);

    return 0;
}
```

## 7. 总结

`[[fallthrough]]` 属性是 C23 标准中的重要改进，为 switch 语句中的有意穿透提供了标准化表达方式。

### 核心要点

| 要点 | 说明 |
|------|------|
| 标准化 | C23 引入，与 C++17 兼容 |
| 用途 | 标记有意穿透，消除编译器警告 |
| 限制 | 必须在 switch 内，后续必须是 case/default |
| 运行时 | 无运行时开销 |

### 技术对比

| 方式 | 可移植性 | 标准化 | 推荐度 |
|------|----------|--------|--------|
| `[[fallthrough]]` | 高 | C23 标准 | 推荐 |
| `/* fall through */` | 高 | 非正式 | 可接受 |
| `__attribute__((fallthrough))` | 低 | GCC 扩展 | 不推荐 |
| 禁用警告 | 高 | - | 不推荐 |

### 使用建议

1. **启用编译器警告**：使用 `-Wimplicit-fallthrough` 等选项
2. **明确标记**：所有有意穿透都应使用 `[[fallthrough]]` 标记
3. **避免滥用**：不要用此属性掩盖逻辑错误
4. **配合注释**：必要时添加注释说明穿透原因

## 参考资料

- cppreference: https://en.cppreference.com/w/c/language/attributes/fallthrough
- C23 Standard (ISO/IEC 9899:2024)
- GCC Documentation: -Wimplicit-fallthrough