# 重载函数的地址 (Address of Overloaded Function)

## 1. 概述 (Overview)

在 C++ 中，当多个函数共享同一个名称但具有不同的参数列表时，这些函数构成**重载函数**（overloaded functions）。除了函数调用表达式（此时会进行重载决议 overload resolution）之外，重载函数的名称还可能出现在其他上下文中，此时需要确定具体引用的是哪个函数。

**重载函数的地址**（Address of overloaded function）是指在非调用上下文中获取重载函数集合中特定函数的地址时，编译器根据目标类型自动选择匹配函数的机制。这是 C++ 类型系统的重要组成部分，确保了函数指针和函数引用的正确初始化和赋值。

## 2. 来源与演变 (Origin and Evolution)

### 首次引入

重载函数的地址解析机制在 **C++98** 标准中首次引入，作为函数重载（function overloading）特性的核心组成部分。该机制定义在标准 `[over.over]` 章节中。

### 历史背景

在 C++ 引入函数重载之前，每个函数必须具有唯一的名称。函数重载允许同名函数存在，但这也带来了一个问题：当需要获取函数地址（如赋值给函数指针）时，编译器如何确定具体是哪个函数？

为了解决这个问题，C++ 标准定义了一套完整的地址解析机制，通过目标类型（target type）来唯一确定重载集合中的函数。

### 各版本变更

| 版本 | 变更内容 |
|------|----------|
| C++98 | 首次定义重载函数地址解析机制 |
| C++11 | 明确列表初始化（list-initialization）也是重载函数地址的上下文 |
| C++17 | 增加函数指针转换（function pointer conversion）支持 |
| C++20 | 增加约束（constraints）相关的消除规则；增加偏序约束比较 |
| C++23 | 增加显式对象成员函数（explicit object member functions）支持 |
| C++26 | 增加占位类型推导（placeholder type deduction）支持 |

### 缺陷报告修正

| DR | 应用版本 | 原行为 | 修正后行为 |
|----|----------|--------|------------|
| CWG 202 | C++98 | 非类型模板参数不是重载函数地址的上下文 | 是有效的上下文 |
| CWG 250 | C++98 | 使用非推导模板参数生成的函数模板特化不会被选中 | 也会被选中 |
| CWG 1153 | C++98 | 不清楚给定函数类型是否匹配目标类型 | 明确了匹配规则 |
| CWG 1563 | C++11 | 不清楚列表初始化是否是重载函数地址的上下文 | 明确是有效上下文 |

## 3. 语法与参数 (Syntax and Parameters)

### 适用的 7 种上下文

重载函数名称可出现在以下 7 种上下文中：

| 序号 | 上下文 | 目标类型 |
|------|--------|----------|
| 1 | 对象或引用声明的初始化器 | 被初始化的对象或引用 |
| 2 | 赋值表达式的右侧 | 赋值表达式的左侧 |
| 3 | 函数调用参数 | 对应的函数参数 |
| 4 | 用户定义运算符参数 | 对应的运算符参数 |
| 5 | `return` 语句 | 函数或转换的返回值 |
| 6 | 显式转换或 `static_cast` 参数 | 对应的转换类型 |
| 7 | 非类型模板参数 | 对应的模板参数 |

### 语法形式

在上述上下文中，重载函数的名称可以：

1. 直接使用函数名
2. 使用取地址运算符 `&` 加函数名
3. 被冗余的括号包围

```cpp
// 以下三种形式等价
int (*pf)(int) = f;       // 形式 1: 直接使用函数名
int (*pf)(int) = &f;      // 形式 2: 使用 & 运算符
int (*pf)(int) = (f);     // 形式 3: 冗余括号
int (*pf)(int) = (&f);    // 形式 4: 组合使用
```

### 目标类型与函数类型的匹配规则

1. **函数指针类型目标**：只能选择非成员函数、显式对象成员函数（C++23 起）和静态成员函数
2. **函数引用类型目标**：同上
3. **成员函数指针类型目标**：只能选择隐式对象成员函数（非静态成员函数）

### 占位类型推导 (C++26 起)

如果目标类型包含占位类型，会进行占位类型推导（placeholder type deduction），然后使用推导出的类型作为目标类型。

## 4. 底层原理 (Underlying Principles)

### 函数选择机制

当获取重载函数的地址时，编译器从重载集合中选择一个函数集合 `S`：

#### 第一步：构建候选集合 S

1. **无目标类型时**：选中所有同名的非模板函数
2. **有目标类型时**：
   - 选择类型 `F` 与目标函数类型 `FT` 完全匹配的非模板函数（C++17 起可能应用函数指针转换）
   - 通过模板参数推导为每个函数模板生成的特化也加入集合 `S`

> **注意**：如果目标类型是成员函数指针类型，则忽略函数所属的类。

#### 第二步：消除不合适的函数

按照以下顺序消除集合 `S` 中的函数：

| 优先级 | 消除规则 | 版本要求 |
|--------|----------|----------|
| 1 | 消除所有关联约束不被满足的函数 | C++20 起 |
| 2 | 如果 `S` 包含非模板函数，消除所有函数模板特化 | 所有版本 |
| 3 | 消除比其他非模板函数约束更弱的非模板函数 | C++20 起 |
| 4 | 消除比其他函数模板特化更不特化的函数模板特化 | 所有版本 |

#### 第三步：验证唯一性

经过消除后，集合 `S` 中必须**恰好剩下一个函数**。否则，程序是**病态的**（ill-formed），编译器会报错。

### 选择流程图

```
构建候选集合 S
      |
      v
[有约束吗?] --(是)--> 消除约束不满足的函数 (C++20+)
      |
      v
[包含非模板函数?] --(是)--> 消除所有函数模板特化
      |
      v
[有多个非模板函数?] --(是)--> 保留约束更强的函数 (C++20+)
      |
      v
[有多个模板特化?] --(是)--> 保留更特化的特化
      |
      v
[只剩一个函数?] --(否)--> 编译错误
      |
     (是)
      |
      v
   选择成功
```

### 类型匹配详解

类型匹配时需要考虑的因素：

1. **参数类型**：必须完全匹配（考虑 cv 限定符）
2. **返回类型**：必须匹配目标类型
3. **调用约定**：必须一致
4. **成员函数**：需要考虑对象类型（`this` 指针）

## 5. 使用场景 (Use Cases)

### 场景 1：函数指针初始化

最常见的使用场景，将重载函数赋值给函数指针。

```cpp
int f(int);
int f(double);

int (*pf1)(int) = f;      // 选择 int f(int)
int (*pf2)(double) = f;   // 选择 int f(double)
```

### 场景 2：函数指针赋值

在赋值表达式的右侧使用重载函数。

```cpp
int (*pf)(int) = nullptr;
pf = f;    // 根据左侧类型选择合适的重载
```

### 场景 3：作为函数参数

将重载函数作为回调函数传递。

```cpp
void register_callback(int (*callback)(int));
register_callback(f);  // 选择 int f(int)
```

### 场景 4：用户定义运算符

在自定义运算符中使用重载函数。

```cpp
struct Executor {
    void operator<<(int (*func)(double));
};
Executor{} << f;  // 选择 int f(double)
```

### 场景 5：返回函数指针

函数返回函数指针时使用。

```cpp
auto get_handler() -> int (*)(int) {
    return f;  // 选择 int f(int)
}
```

### 场景 6：显式类型转换

使用 `static_cast` 或 C 风格转换明确指定目标类型。

```cpp
auto p = static_cast<int(*)(int)>(f);  // 选择 int f(int)
```

### 场景 7：非类型模板参数

将重载函数作为模板参数传递。

```cpp
template<int(*F)(int)>
struct Handler {};

Handler<f> h;  // 选择 int f(int)
```

### 最佳实践

1. **优先使用 `static_cast`**：当类型推导可能产生歧义时，显式转换使意图更清晰
2. **避免过度重载**：过多相似签名的重载会增加编译器选择难度
3. **使用 `auto` 配合初始化**：C++11 起，可使用 `auto` 简化函数指针声明

### 常见陷阱

1. **类型不匹配**：目标类型与所有重载版本都不匹配时编译失败
2. **歧义选择**：多个函数都能匹配目标类型时产生歧义错误
3. **成员函数指针**：普通成员函数需要使用成员函数指针语法

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

// 重载函数
int f(int) { return 1; }
int f(double) { return 2; }

// 接受函数指针的函数
void g(int(&f1)(int), int(*f2)(double)) {
    std::cout << f1(0) << std::endl;    // 调用 int f(int)
    std::cout << f2(0.0) << std::endl;  // 调用 int f(double)
}

int main() {
    // 1. 初始化
    int (*pf)(double) = f;   // 选择 int f(double)
    int (&rf)(int) = f;      // 选择 int f(int)

    std::cout << "pf(1.5) = " << pf(1.5) << std::endl;  // 输出: 2
    std::cout << "rf(1) = " << rf(1) << std::endl;      // 输出: 1

    // 2. 赋值
    pf = nullptr;
    pf = &f;  // 选择 int f(double)

    // 3. 函数参数
    g(f, f);  // 第一个参数选择 int f(int)
              // 第二个参数选择 int f(double)

    return 0;
}
```

### 成员函数示例

```cpp
#include <iostream>

struct Foo {
    int mf(int) { return 3; }
    int mf(double) { return 4; }
};

int main() {
    // 成员函数指针
    int (Foo::*mpf)(int) = &Foo::mf;  // 选择 int mf(int)

    Foo obj;
    std::cout << (obj.*mpf)(10) << std::endl;  // 输出: 3

    return 0;
}
```

### 模板参数示例

```cpp
#include <iostream>

int f(int) { return 1; }
int f(double) { return 2; }

// 非类型模板参数：函数指针
template<int(*F)(int)>
struct Templ {
    int call(int x) { return F(x); }
};

int main() {
    Templ<f> t;  // 选择 int f(int)
    std::cout << t.call(42) << std::endl;  // 输出: 1

    return 0;
}
```

### 用户定义运算符示例

```cpp
#include <iostream>

int f(int) { return 1; }
int f(double) { return 2; }

struct Emp {
    void operator<<(int (*)(double)) {
        std::cout << "Selected f(double)" << std::endl;
    }
};

int main() {
    Emp{} << f;  // 选择 int f(double)
    return 0;
}
```

### Lambda 返回函数指针

```cpp
#include <iostream>

int f(int) { return 1; }
int f(double) { return 2; }

int main() {
    // Lambda 返回函数指针
    auto foo = []() -> int (*)(int) {
        return f;  // 选择 int f(int)
    };

    auto func = foo();
    std::cout << func(42) << std::endl;  // 输出: 1

    return 0;
}
```

### 常见错误及修正

#### 错误 1：类型不匹配

```cpp
#include <iostream>

int f(int) { return 1; }
int f(double) { return 2; }

int main() {
    // 错误：没有匹配 int(char) 的重载
    // int (*pf)(char) = f;  // 编译错误

    // 修正：使用匹配的类型
    int (*pf)(int) = f;  // OK
    std::cout << pf('a') << std::endl;  // OK，char 隐式转换为 int

    return 0;
}
```

#### 错误 2：歧义选择

```cpp
#include <iostream>

void h(int) { std::cout << "int\n"; }
void h(unsigned int) { std::cout << "unsigned\n"; }

int main() {
    // 错误：使用 auto 无法推导函数指针类型
    // auto pf = h;  // 编译错误：歧义

    // 修正：使用显式类型或 static_cast
    void (*pf1)(int) = h;  // OK
    auto pf2 = static_cast<void(*)(int)>(h);  // OK

    pf1(0);  // 输出: int

    return 0;
}
```

#### 错误 3：成员函数指针语法

```cpp
#include <iostream>

struct Foo {
    int mf(int) { return 3; }
    int mf(double) { return 4; }
};

int main() {
    // 错误：普通函数指针不能指向成员函数
    // int (*pf)(int) = &Foo::mf;  // 编译错误

    // 修正：使用成员函数指针
    int (Foo::*mpf)(int) = &Foo::mf;  // OK

    Foo obj;
    std::cout << (obj.*mpf)(10) << std::endl;  // 输出: 3

    return 0;
}
```

### 高级用法：模板函数重载

```cpp
#include <iostream>

// 非模板函数
void process(int x) { std::cout << "process(int): " << x << std::endl; }

// 模板函数
template<typename T>
void process(T x) { std::cout << "process<T>: " << x << std::endl; }

// 模板特化
template<>
void process(double x) { std::cout << "process<double>: " << x << std::endl; }

int main() {
    // 非模板函数优先
    void (*pf1)(int) = process;  // 选择非模板 process(int)
    pf1(10);  // 输出: process(int): 10

    // 模板特化
    void (*pf2)(double) = process;  // 选择 process<double> 特化
    pf2(3.14);  // 输出: process<double>: 3.14

    // 其他类型选择模板
    void (*pf3)(char) = process;  // 选择 process<char> 模板实例化
    pf3('A');  // 输出: process<T>: A

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **7 种上下文**：重载函数名称可出现在初始化、赋值、函数参数、运算符参数、返回值、类型转换、模板参数 7 种上下文中
2. **目标类型驱动**：编译器根据目标类型自动选择匹配的重载函数
3. **选择优先级**：非模板函数优先于模板特化；更特化的模板优先于更通用的模板
4. **唯一性要求**：最终必须恰好选中一个函数，否则程序病态

### 类型匹配规则表

| 目标类型 | 可选函数类型 |
|----------|--------------|
| 函数指针 | 非成员函数、静态成员函数、显式对象成员函数（C++23 起） |
| 函数引用 | 同上 |
| 成员函数指针 | 非静态成员函数（隐式对象成员函数） |

### 使用建议

1. **明确目标类型**：确保目标类型与期望的重载版本精确匹配
2. **显式转换消除歧义**：使用 `static_cast` 明确指定类型
3. **理解优先级规则**：非模板 > 模板特化 > 更特化模板 > 更通用模板
4. **注意成员函数特殊性**：成员函数需要使用成员函数指针语法

### 相关概念

| 概念 | 关系 |
|------|------|
| 函数重载 (Function Overloading) | 重载函数地址选择的基础 |
| 重载决议 (Overload Resolution) | 函数调用时的选择机制 |
| 函数指针 (Function Pointer) | 重载函数地址的常见目标类型 |
| 模板参数推导 (Template Argument Deduction) | 函数模板特化选择的关键机制 |

## 参考资料

- C++26 标准 (ISO/IEC 14882:2026): [over.over] Address of overloaded function
- C++23 标准 (ISO/IEC 14882:2024): 12.3 Address of overloaded function [over.over]
- C++20 标准 (ISO/IEC 14882:2020): 12.5 Address of overloaded function [over.over]
- C++17 标准 (ISO/IEC 14882:2017): 16.4 Address of overloaded function [over.over]
- C++14 标准 (ISO/IEC 14882:2014): 13.4 Address of overloaded function [over.over]
- C++11 标准 (ISO/IEC 14882:2011): 13.4 Address of overloaded function [over.over]
- C++98 标准 (ISO/IEC 14882:1998): 13.4 Address of overloaded function [over.over]
- cppreference: https://en.cppreference.com/w/cpp/language/overloaded_address