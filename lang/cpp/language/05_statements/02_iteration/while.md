# while 循环语句

## 1. 概述 (Overview)

`while` 循环是 C++ 中最基本的循环控制结构之一，用于在条件为真时重复执行语句。它属于迭代语句（iteration statement）的一种，通过条件判断来控制循环的执行。

**核心特点：**
- 条件在前，循环体在后（入口条件循环）
- 每次迭代前先判断条件，条件为真才执行循环体
- 条件为假时，循环体可能一次都不执行
- 适用于循环次数不确定的场景

**技术定位：**
- 属于 C++ 流程控制语句
- 与 `for` 循环、`do-while` 循环并列
- 最灵活的循环结构，可实现任意循环逻辑

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`while` 循环继承自 C 语言，是最早出现的循环结构之一。其设计动机是提供一种简单直观的条件循环机制，适用于"执行到某个条件不满足为止"的场景。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 基础语法：`while ( condition ) statement` |
| C++11 | 增加属性（attribute）支持；条件声明语法现代化 |
| C++26 | 支持结构化绑定声明作为条件；非平凡无限循环的前向进度保证 |

### 设计动机

- **简洁性**：提供最直观的"当...时重复"语义
- **灵活性**：条件可以是表达式或声明，适应不同编程风格
- **安全性**：入口条件设计避免不必要的循环体执行
- **一致性**：与 `if` 语句的条件语法保持一致

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
attr(optional) while ( condition ) statement
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr` | (C++11 起) 任意数量的属性，可选 |
| `condition` | 条件：可以是表达式或声明 |
| `statement` | 循环体语句，通常是复合语句（代码块） |

### 条件（condition）形式

条件可以是以下两种形式之一：

#### 1. 表达式形式

```cpp
while ( expression )
    statement
```

表达式的值会被上下文转换为 `bool` 类型。如果转换失败，程序非法。

#### 2. 声明形式

**C++11 之前：**
```cpp
type-specifier-seq declarator = assignment-expression
```

**C++11 起：**
```cpp
attribute-specifier-seq(optional) decl-specifier-seq declarator brace-or-equal-initializer
```

**C++26 起：支持结构化绑定声明：**
```cpp
auto [a, b, c] = expression
```

### 声明形式的限制

**非结构化绑定声明：**
- 声明符不能指定函数或数组
- 类型说明符序列只能包含类型说明符和 `constexpr`
- 不能定义类或枚举

**结构化绑定声明（C++26 起）：**
- 初始化器中的表达式不能是数组类型
- 说明符序列只能包含类型说明符和 `constexpr`

### 决策变量（Decision Variable）

- **普通声明**：决策变量就是被声明的变量本身
- **结构化绑定声明（C++26）**：决策变量是声明引入的隐藏变量 `e`

### 条件判断规则

当控制流到达条件时，条件会产生一个值，用于决定是否执行循环体：

1. **表达式条件**：表达式的值上下文转换为 `bool`
2. **声明条件**：决策变量的值上下文转换为 `bool`

## 4. 底层原理 (Underlying Principles)

### 等价实现

`while` 循环在语义上等价于以下结构：

```cpp
/* label */:
{
    if ( condition )
    {
        statement
        goto /* label */;
    }
}
```

### 变量生命周期

**关键特性：** 如果条件是声明，则声明的变量在每次迭代时都会：
1. 重新创建（构造）
2. 迭代结束时销毁（析构）

```cpp
while (int x = get_value())  // x 每次迭代重新构造和析构
{
    // 使用 x
}  // x 在此处销毁
```

### 作用域规则

无论 `statement` 是否为复合语句，它总是引入一个块作用域：

```cpp
while (--x >= 0)
    int i;        // i 在此声明

// i 已超出作用域

// 等价于：

while (--x >= 0)
{
    int i;        // i 在块内声明
}                 // i 在此处销毁
```

### 前向进度保证（Forward Progress Guarantee）

**C++26 起：** 如果一个循环满足以下所有条件：
- 是无限循环
- 没有可观察的行为
- 不是平凡无限循环（trivial infinite loop）

则行为未定义，编译器可以移除这样的循环。

```cpp
// 未定义行为（C++26）：可能被编译器移除
while (true) {}

// 平凡无限循环：行为定义良好
while (true) {
    volatile int x = 0;  // 有可观察行为
}
```

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 说明 |
|------|------|
| 输入验证 | 读取输入直到获得有效值 |
| 状态轮询 | 持续检查某个条件状态 |
| 资源消耗 | 重复处理直到资源耗尽 |
| 搜索操作 | 查找满足条件的元素 |
| 事件循环 | 处理事件直到退出条件 |

### 最佳实践

1. **确保循环能终止**
   ```cpp
   int count = 0;
   while (count < max_iterations && !found) {
       // 确保有终止条件
       ++count;
   }
   ```

2. **使用声明形式简化代码**
   ```cpp
   // 推荐：简洁清晰
   while (char c = *ptr++) {
       process(c);
   }

   // 不推荐：冗余
   char c = *ptr++;
   while (c) {
       process(c);
       c = *ptr++;
   }
   ```

3. **复杂条件使用函数封装**
   ```cpp
   while (should_continue()) {
       // 清晰的语义
   }
   ```

### 常见陷阱

1. **无限循环**
   ```cpp
   int i = 0;
   while (i < 10) {
       // 忘记更新 i
       process(i);
   }  // 无限循环！
   ```

2. **条件中的副作用**
   ```cpp
   while (i++ < 10) {
       // i 在判断时已递增
       // 循环体处理的是 i+1
   }
   ```

3. **浮点数比较**
   ```cpp
   double x = 0.0;
   while (x != 1.0) {  // 危险：浮点精度问题
       x += 0.1;
   }
   ```

### 与其他循环的对比

| 特性 | while | for | do-while |
|------|-------|-----|----------|
| 条件位置 | 循环开始 | 循环开始 | 循环结束 |
| 最少执行次数 | 0 次 | 0 次 | 1 次 |
| 初始化语法 | 无 | 内置 | 无 |
| 迭代语法 | 无 | 内置 | 无 |
| 适用场景 | 不确定次数 | 已知次数 | 至少执行一次 |

## 6. 代码示例 (Examples)

### 基础示例

#### 示例 1：单语句循环体

```cpp
#include <iostream>

int main() {
    int i = 0;
    while (i < 10)
        i++;  // 单语句，无需花括号
    std::cout << i << '\n';  // 输出：10
    return 0;
}
```

#### 示例 2：复合语句循环体

```cpp
#include <iostream>

int main() {
    int j = 2;
    while (j < 9)
    {
        std::cout << j << ' ';
        j += 2;
    }
    std::cout << '\n';  // 输出：2 4 6 8
    return 0;
}
```

#### 示例 3：条件声明形式

```cpp
#include <iostream>

int main() {
    char cstr[] = "Hello";
    int k = 0;
    while (char c = cstr[k++])  // c 在每次迭代创建和销毁
        std::cout << c;
    std::cout << '\n';  // 输出：Hello
    return 0;
}
```

### 进阶示例

#### 示例 4：输入验证

```cpp
#include <iostream>
#include <limits>

int main() {
    int value;
    while (true) {
        std::cout << "Enter a positive number: ";
        if (std::cin >> value && value > 0) {
            break;  // 有效输入，退出循环
        }
        std::cout << "Invalid input!\n";
        std::cin.clear();  // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "You entered: " << value << '\n';
    return 0;
}
```

#### 示例 5：使用 continue 和 break

```cpp
#include <iostream>

int main() {
    int i = 0;
    while (i < 20) {
        ++i;
        if (i % 2 == 0)
            continue;  // 跳过偶数
        if (i > 10)
            break;     // 大于 10 时终止
        std::cout << i << ' ';  // 输出：1 3 5 7 9
    }
    std::cout << "\nFinal i: " << i << '\n';  // 输出：11
    return 0;
}
```

#### 示例 6：条件变量的生命周期

```cpp
#include <iostream>

struct Tracker {
    int id;
    Tracker(int i) : id(i) { std::cout << "Construct " << id << '\n'; }
    ~Tracker() { std::cout << "Destruct " << id << '\n'; }
    operator bool() { return id < 3; }
};

int main() {
    int count = 0;
    while (Tracker t = Tracker(count++)) {
        std::cout << "Loop body, t.id = " << t.id << '\n';
    }
    std::cout << "After loop\n";
    return 0;
}
// 输出：
// Construct 0
// Loop body, t.id = 0
// Destruct 0
// Construct 1
// Loop body, t.id = 1
// Destruct 1
// Construct 2
// Loop body, t.id = 2
// Destruct 2
// Construct 3
// Destruct 3
// After loop
```

#### 示例 7：C++26 结构化绑定声明（预期特性）

```cpp
#include <iostream>
#include <tuple>

std::tuple<int, int, bool> get_state() {
    static int calls = 0;
    return {++calls, calls * 2, calls < 5};
}

int main() {
    // C++26 起：结构化绑定作为条件
    while (auto [x, y, valid] = get_state(); valid) {
        std::cout << "x=" << x << ", y=" << y << '\n';
    }
    return 0;
}
```

### 常见错误与修正

#### 错误 1：分号导致的空循环体

```cpp
// 错误：分号使循环体为空
int i = 0;
while (i < 10);  // 警告：此处可能有分号
{
    ++i;  // 这不是循环体！
}
```

修正：
```cpp
int i = 0;
while (i < 10)  // 移除分号
{
    ++i;
}
```

#### 错误 2：赋值与比较混淆

```cpp
// 错误：使用了赋值而非比较（编译器通常会警告）
int x = 5;
while (x = 10) {  // 永远为真！
    // ...
}
```

修正：
```cpp
int x = 5;
while (x == 10) {  // 使用比较运算符
    // ...
}
```

#### 错误 3：修改循环变量后 continue

```cpp
// 错误：continue 跳过了必要的变量更新
int i = 0;
while (i < 10) {
    if (should_skip(i)) {
        continue;  // 跳过了 ++i，可能无限循环
    }
    process(i);
    ++i;
}
```

修正：
```cpp
int i = 0;
while (i < 10) {
    bool skip = should_skip(i);
    if (!skip) {
        process(i);
    }
    ++i;  // 确保每次迭代都更新
}
```

## 7. 总结 (Summary)

### 核心要点

1. **语法简洁**：`while (condition) statement` 是最直观的循环形式
2. **入口条件**：条件在循环体执行前判断，可能零次执行
3. **条件灵活**：支持表达式和声明两种形式（C++26 起支持结构化绑定）
4. **作用域规则**：循环体总是引入块作用域
5. **变量生命周期**：条件声明中的变量每次迭代重新创建和销毁

### 技术对比

| 特性 | while | for | do-while |
|------|-------|-----|----------|
| 语法复杂度 | 简单 | 中等 | 简单 |
| 初始化支持 | 无 | 有 | 无 |
| 最少执行次数 | 0 | 0 | 1 |
| 代码清晰度 | 高（条件循环） | 高（计数循环） | 中 |

### 选择建议

- **使用 while**：当循环次数不确定，或需要在循环外使用循环变量时
- **使用 for**：当循环次数确定，或需要局部循环变量时
- **使用 do-while**：当循环体至少需要执行一次时

### 学习建议

1. **理解等价结构**：`while` 可以用 `goto` 和 `if` 模拟，理解这一点有助于掌握其语义
2. **注意变量作用域**：循环体内的变量作用域限于循环体内
3. **避免无限循环**：确保循环条件最终会变为假
4. **善用 break/continue**：合理使用流程控制语句简化逻辑
5. **关注 C++26 新特性**：结构化绑定条件将提供更强大的功能

### 关键记忆点

```
while (condition) statement
```
- 条件为真 -> 执行循环体 -> 重新判断
- 条件为假 -> 跳过循环
- `break` -> 终止循环
- `continue` -> 跳到下一次迭代

---

## 参考资料

- C++26 标准 (ISO/IEC 14882:2026)
- C++23 标准 (ISO/IEC 14882:2023)
- C++17 标准 (ISO/IEC 14882:2017)
- C++11 标准 (ISO/IEC 14882:2011)
- C++98 标准 (ISO/IEC 14882:1998)