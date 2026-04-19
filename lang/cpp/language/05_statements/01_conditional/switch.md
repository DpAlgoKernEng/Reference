# switch 语句 (switch statement)

## 1. 概述 (Overview)

**switch 语句**是 C++ 中根据条件值将控制转移到多个语句之一的多路选择语句。

### 核心概念

- **多路分支**：根据条件值选择执行对应的 case 分支
- **case 标签**：标记分支入口点的常量表达式标签
- **default 标签**：处理所有 case 都不匹配情况的默认分支
- **fall-through**：执行完一个 case 后继续执行下一个 case（除非遇到 break）
- **初始化语句**：C++17 起支持在条件前声明变量

### 技术定位

switch 语句属于 C++ 的**选择语句**（selection statements）类别，与 if 语句共同构成条件控制结构。相比 C 语言，C++ 在 switch 语句上引入了多项现代特性：

| 特性 | 版本 | 功能 |
|------|------|------|
| 属性支持 | C++11 | 允许在 switch 和标签上应用属性 |
| 初始化语句 | C++17 | 在条件前声明变量 |
| `[[fallthrough]]` 属性 | C++17 | 标注有意的 fall-through |
| 结构化绑定条件 | C++26 | 条件可以是结构化绑定声明 |

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

switch 语句源自 C 语言，设计动机是提供高效的多路分支选择机制。C++ 在继承 C 语言 switch 语义的基础上，逐步引入了现代编程所需的特性。

### 版本变更

| 标准 | 主要变更 |
|------|----------|
| C++98 | 继承 C 语言 switch 语句语义 |
| C++11 | 新增属性（`attr`）支持 |
| C++17 | 新增初始化语句（`init-statement`）和 `[[fallthrough]]` 属性 |
| C++23 | 初始化语句支持别名声明 |
| C++26 | 条件支持结构化绑定声明 |

### 缺陷报告

| DR | 版本 | 原行为 | 修正行为 |
|-----|------|--------|----------|
| CWG 1767 | C++98 | 不受整数提升影响的条件类型不能被提升 | 不提升这些类型的条件 |
| CWG 2629 | C++98 | 条件可以是浮点变量声明 | 禁止浮点变量声明 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```cpp
attr(可选) switch ( init-statement(可选) condition ) statement
```

**case 标签：**
```cpp
attr(可选) case constant-expression :
```

**default 标签：**
```cpp
attr(可选) default:
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr` | (C++11 起) 任意数量的属性 |
| `init-statement` | (C++17 起) 表达式语句、简单声明或别名声明 |
| `condition` | 条件：表达式或简单声明 |
| `statement` | 语句（通常是复合语句） |
| `constant-expression` | 调整后的 switch 条件类型的转换常量表达式 |

### 初始化语句（init-statement）

C++17 起，init-statement 可以是：

1. **表达式语句**（可以为空语句 `;`）
2. **简单声明**：通常声明带初始化器的变量或结构化绑定
3. **别名声明**（C++23 起）

```cpp
// 典型用法
switch (auto dev = Device{}; dev.state()) {
    case Device::SLEEP: /* ... */ break;
    case Device::READY: /* ... */ break;
}
```

### 条件（Condition）

条件可以是表达式或简单声明：

**表达式形式**：
- 求值结果作为 switch 的匹配值

**声明形式**：
- 非结构化绑定声明：声明的变量作为决策变量
- 结构化绑定声明（C++26 起）：引入的变量 `e` 作为决策变量

### 类型限制

条件只能产生以下类型：

| 类型 | 说明 |
|------|------|
| 整数类型 | char、short、int、long 等 |
| 枚举类型 | 有作用域或无作用域枚举 |
| 类类型 | 必须能隐式转换为整数或枚举类型 |

**类型转换规则**：
1. 类类型值会被上下文隐式转换为整数或枚举类型
2. 如果类型受整数提升影响，值会被转换为提升后的类型

### case 标签约束

1. **常量表达式**：case 后必须是编译期常量
2. **唯一性**：同一 switch 内的所有 case 值必须唯一
3. **类型匹配**：常量表达式类型必须与调整后的条件类型兼容
4. **default 唯一**：每个 switch 最多一个 default 标签

---

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
┌─────────────────────────────────────┐
│      执行 init-statement（如有）     │
└─────────────────┬───────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│          求值 condition             │
└─────────────────┬───────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│       类型转换（如需要）             │
└─────────────────┬───────────────────┘
                  │
        ┌─────────┴─────────┐
        │                   │
        ▼                   ▼
┌───────────────┐   ┌───────────────┐
│  找到匹配的   │   │  未找到匹配   │
│  case 标签    │   └───────┬───────┘
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

### 标签关联规则

case 和 default 标签与**最内层**包围它的 switch 语句关联：

```cpp
switch (outer) {
    case 1:
        switch (inner) {
            case 1:  // 与内层 switch 关联
                break;
            case 2:
                break;
        }
        break;
    case 2:  // 与外层 switch 关联
        break;
}
```

### Fall-through 行为

case 和 default 标签本身不改变控制流。要退出 switch 语句，需要使用 break 语句。

```cpp
switch (1) {
    case 1:
        std::cout << '1'; // 打印 "1"
    case 2:
        std::cout << '2'; // 然后打印 "2"（fall-through）
}

switch (1) {
    case 1:
        std::cout << '1'; // 打印 "1"
        break;            // 退出 switch
    case 2:
        std::cout << '2';
        break;
}
```

### [[fallthrough]] 属性（C++17）

C++17 引入 `[[fallthrough]]` 属性，用于标注有意的 fall-through，消除编译器警告：

```cpp
switch (i) {
    case 1:
        // ...
        [[fallthrough]]; // 标注有意的 fall-through
    case 2:
        // ...
        break;
}
```

### 变量作用域与跳转限制

由于控制流不允许进入变量作用域，如果声明语句出现在 switch 体内，必须用复合语句限定作用域：

```cpp
switch (1) {
    case 1:
        int x = 0;  // 初始化
        std::cout << x << '\n';
        break;
    default:
        // 编译错误：跳转到 default
        // 会进入 'x' 的作用域但不初始化它
        std::cout << "default\n";
        break;
}

switch (1) {
    case 1:
        {
            int x = 0;
            std::cout << x << '\n';
            break;
        } // 'x' 的作用域在此结束
    default:
        std::cout << "default\n";  // 无错误
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

---

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 状态机实现 | 根据状态值执行对应处理 |
| 命令解析 | 根据命令类型分发处理 |
| 枚举值处理 | 根据枚举值执行不同操作 |
| 协议解析 | 根据消息类型选择处理逻辑 |

### 最佳实践

1. **使用初始化语句限制作用域**（C++17）：
```cpp
switch (auto it = map.find(key); it != map.end() ? it->second : -1) {
    // ...
}
```

2. **使用 `[[fallthrough]]` 标注有意 fall-through**（C++17）：
```cpp
case 1:
    do_something();
    [[fallthrough]];  // 明确意图
case 2:
    // ...
```

3. **使用枚举类型提高可读性**：
```cpp
enum class State { Idle, Running, Stopped };
switch (state) {
    case State::Idle: /* ... */ break;
    case State::Running: /* ... */ break;
    case State::Stopped: /* ... */ break;
}
```

4. **始终添加 default 分支**：
```cpp
default:
    assert(false && "unhandled case");
    break;
```

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 忘记 break | 无意中造成 fall-through | 每个 case 后添加 break 或 `[[fallthrough]]` |
| 变量初始化跳过 | 跳转到 case 跳过变量初始化 | 使用复合语句块限制作用域 |
| 枚举未完全处理 | 遗漏某些枚举值 | 编译器警告 + default 处理 |
| 浮点条件 | C++98 允许浮点变量声明 | 使用整数或枚举类型 |

---

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

int main()
{
    const int i = 2;
    switch (i)
    {
        case 1:
            std::cout << '1';
        case 2:              // 从此 case 开始执行
            std::cout << '2';
        case 3:
            std::cout << '3';
            [[fallthrough]]; // C++17：标注有意的 fall-through
        case 5:
            std::cout << "45";
            break;           // 终止后续语句执行
        case 6:
            std::cout << '6';
    }

    std::cout << '\n';

    switch (i)
    {
        case 4:
            std::cout << 'a';
        default:
            std::cout << 'd'; // 没有匹配的常量表达式
                              // 因此执行 default
    }

    std::cout << '\n';

    switch (i)
    {
        case 4:
            std::cout << 'a'; // 不执行任何内容
    }

    return 0;
}
```

**输出：**
```
2345
d
```

### 枚举处理

```cpp
#include <iostream>

int main()
{
    // 使用枚举时，许多编译器会在未处理某些枚举值时发出警告
    enum color { RED, GREEN, BLUE };
    switch (RED)
    {
        case RED:
            std::cout << "red\n";
            break;
        case GREEN:
            std::cout << "green\n";
            break;
        case BLUE:
            std::cout << "blue\n";
            break;
    }

    return 0;
}
```

### 初始化语句（C++17）

```cpp
#include <iostream>

struct Device
{
    enum State { SLEEP, READY, BAD };
    auto state() const { return m_state; }

private:
    State m_state{};
};

int main()
{
    // C++17 init-statement 语法
    // 当没有到整数或枚举类型的隐式转换时很有用
    switch (auto dev = Device{}; dev.state())
    {
        case Device::SLEEP:
            std::cout << "Device is sleeping\n";
            break;
        case Device::READY:
            std::cout << "Device is ready\n";
            break;
        case Device::BAD:
            std::cout << "Device is in bad state\n";
            break;
    }

    return 0;
}
```

### 变量作用域处理

```cpp
#include <iostream>

int main()
{
    // 正确：使用复合语句限制变量作用域
    switch (1)
    {
        case 1:
            {
                int x = 0;  // x 的作用域限制在此块内
                std::cout << x << '\n';
                break;
            }
        default:
            std::cout << "default\n";  // 无错误
            break;
    }

    // 变量声明在 switch 外
    int y = 10;
    switch (1)
    {
        case 1:
            std::cout << y << '\n';  // 使用外部变量
            break;
        default:
            break;
    }

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>

int main()
{
    // 错误 1：忘记 break
    std::cout << "Error 1 - Missing break:\n";
    int x = 1;
    switch (x)
    {
        case 1:
            std::cout << "  case 1\n";
            // 忘记 break！
        case 2:
            std::cout << "  case 2 (unexpected!)\n";
            break;
    }

    // 正确写法
    std::cout << "Correct 1:\n";
    switch (x)
    {
        case 1:
            std::cout << "  case 1\n";
            break;
        case 2:
            std::cout << "  case 2\n";
            break;
    }

    // 错误 2：变量初始化被跳过
    std::cout << "Error 2 - Variable initialization skipped:\n";
    // switch (x)
    // {
    //     case 1:
    //         int y = 42;  // 初始化
    //         std::cout << y << '\n';
    //         break;
    //     default:
    //         // 错误：跳转到这里会跳过 y 的初始化
    //         std::cout << "default\n";
    //         break;
    // }

    // 正确写法：使用复合语句
    std::cout << "Correct 2:\n";
    switch (x)
    {
        case 1:
            {
                int y = 42;
                std::cout << y << '\n';
                break;
            }
        default:
            std::cout << "default\n";
            break;
    }

    return 0;
}
```

### 高级用法：Duff's Device

Duff's Device 是一种利用 switch 语句 fall-through 特性的循环展开技术：

```cpp
#include <iostream>

// Duff's Device: 循环展开
void duff_copy(char* to, const char* from, int count)
{
    int n = (count + 7) / 8;
    switch (count % 8)
    {
        case 0: do { *to++ = *from++;
        case 7:      *to++ = *from++;
        case 6:      *to++ = *from++;
        case 5:      *to++ = *from++;
        case 4:      *to++ = *from++;
        case 3:      *to++ = *from++;
        case 2:      *to++ = *from++;
        case 1:      *to++ = *from++;
                } while (--n > 0);
    }
}

int main()
{
    const char src[] = "Hello, World!";
    char dst[20] = {};

    duff_copy(dst, src, sizeof(src) - 1);

    std::cout << "Copied: " << dst << '\n';
    return 0;
}
```

### 状态机实现

```cpp
#include <iostream>
#include <string>

enum class State { Idle, Connecting, Connected, Error };
enum class Event { Connect, Connected, Disconnect, Error };

State current_state = State::Idle;

void handle_event(Event event)
{
    switch (current_state)
    {
        case State::Idle:
            switch (event)
            {
                case Event::Connect:
                    std::cout << "Connecting...\n";
                    current_state = State::Connecting;
                    break;
                default:
                    std::cout << "Invalid event in Idle state\n";
                    break;
            }
            break;

        case State::Connecting:
            switch (event)
            {
                case Event::Connected:
                    std::cout << "Connected!\n";
                    current_state = State::Connected;
                    break;
                case Event::Error:
                    std::cout << "Connection failed\n";
                    current_state = State::Error;
                    break;
                default:
                    break;
            }
            break;

        case State::Connected:
            switch (event)
            {
                case Event::Disconnect:
                    std::cout << "Disconnecting...\n";
                    current_state = State::Idle;
                    break;
                case Event::Error:
                    std::cout << "Connection error\n";
                    current_state = State::Error;
                    break;
                default:
                    break;
            }
            break;

        case State::Error:
            switch (event)
            {
                case Event::Connect:
                    std::cout << "Retrying...\n";
                    current_state = State::Connecting;
                    break;
                default:
                    break;
            }
            break;
    }
}

int main()
{
    handle_event(Event::Connect);
    handle_event(Event::Connected);
    handle_event(Event::Disconnect);
    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 条件类型 | 整数、枚举、可转换为整数/枚举的类类型 |
| case 值 | 必须是唯一的编译期常量表达式 |
| default | 最多一个，处理所有不匹配的情况 |
| fall-through | 执行完 case 后继续下一个 case，除非遇到 break |
| 初始化语句 | C++17 起支持在条件前声明变量 |

### switch vs if-else 对比

| 特性 | switch 语句 | if-else 语句 |
|------|-------------|--------------|
| 条件类型 | 整数、枚举、特定类类型 | 任意可转换为 bool 的类型 |
| 分支匹配 | 精确值匹配 | 支持范围判断 |
| 可读性 | 多分支时更清晰 | 条件复杂时更直观 |
| 性能 | 可能优化为跳转表 | 线性判断 |
| fall-through | 支持 | 不支持 |

### C++ 特有特性

| 特性 | 版本 | 说明 |
|------|------|------|
| 属性 | C++11 | `attr` 应用到 switch 和标签 |
| 初始化语句 | C++17 | `switch (init; cond)` |
| `[[fallthrough]]` | C++17 | 标注有意的 fall-through |
| 结构化绑定 | C++26 | 条件可以是结构化绑定声明 |

### 学习建议

1. **理解 fall-through**：这是 switch 最重要的特性，需要谨慎处理
2. **使用 `[[fallthrough]]`**：标注有意的 fall-through 以消除警告
3. **善用初始化语句**：限制变量作用域，提高代码可读性
4. **注意作用域规则**：变量声明需要放在正确的位置
5. **使用枚举类型**：提高类型安全性和代码可读性

---

## 参考资料

- C++26 标准 (ISO/IEC 14882:2026)
- C++23 标准 (ISO/IEC 14882:2023)
- C++20 标准 (ISO/IEC 14882:2020)
- C++17 标准 (ISO/IEC 14882:2017)
- C++14 标准 (ISO/IEC 14882:2014)
- C++11 标准 (ISO/IEC 14882:2011)
- C++98 标准 (ISO/IEC 14882:1998)
- CWG 1767: Integral promotion of switch conditions
- CWG 2629: Floating-point variables in switch conditions

## 外部链接

1. [Loop unrolling using Duff's Device](https://en.wikipedia.org/wiki/Duff%27s_device)
2. [Duff's device can be used to implement coroutines in C/C++](https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html)