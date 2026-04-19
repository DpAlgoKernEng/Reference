# `consteval` 说明符 (C++20)

## 1. 概述

`consteval` 是 C++20 引入的函数说明符，用于声明**立即函数**（immediate function）。与 `constexpr` 不同，`consteval` 强制要求每次对函数的调用都必须在编译时产生一个常量表达式（constant expression）。

简单来说：
- `constexpr` 函数：**可以**在编译时求值，也可以在运行时求值
- `consteval` 函数：**必须**在编译时求值

`consteval` 定义在 C++ 核心语言中，无需包含任何头文件。

## 2. 来源与演变

### 首次引入

`consteval` 说明符首次在 **C++20** 标准中引入，作为对 `constexpr` 的补充和增强。

### 历史背景

在 `consteval` 出现之前：

1. **`constexpr` 的局限性**：`constexpr` 函数虽然可以在编译时求值，但编译器不强制要求。如果传入的参数不是编译时常量，函数会在运行时执行，这可能导致：
   - 性能不如预期（本应在编译时计算）
   - 编译时与运行时行为不一致

2. **模板元编程的复杂性**：为确保某些函数在编译时执行，开发者需要借助复杂的模板技巧

`consteval` 的引入解决了这些问题：
- 强制编译时求值，保证性能
- 编译时检查参数是否为常量表达式
- 更清晰的代码意图表达

### C++23 变化

C++23 通过缺陷报告（DR20）增强了 `consteval` 的传播行为：
- `consteval if` 语句的真分支被视为立即函数上下文
- 简化了立即函数的嵌套调用规则

### 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|-----------|-----|------|------|
| `__cpp_consteval` | `201811L` | C++20 | 立即函数 |
| `__cpp_consteval` | `202211L` | C++23 (DR20) | consteval 传播增强 |

## 3. 语法与参数

### 基本语法

```cpp
consteval 返回类型 函数名(参数列表)
{
    // 函数体
}
```

### 声明规则

1. **隐含 inline**：`consteval` 说明符隐含 `inline`，函数默认是内联的
2. **不能与 constexpr 同时使用**：同一个函数声明不能同时指定 `consteval` 和 `constexpr`
3. **重声明一致性**：如果首次声明使用 `consteval`，所有重声明也必须使用 `consteval`

### 适用限制

`consteval` **不能**应用于以下函数：

| 函数类型 | 原因 |
|---------|------|
| 析构函数 | 析构时机由对象生命周期决定，无法强制编译时执行 |
| `operator new`（分配函数） | 内存分配通常在运行时进行 |
| `operator delete`（解分配函数） | 内存释放通常在运行时进行 |

### 核心概念

#### 立即函数（Immediate Function）

使用 `consteval` 声明的函数称为立即函数。其特点：
- 是一种 `constexpr` 函数，需满足 `constexpr` 函数的要求
- 每次潜在求值的调用都必须产生编译时常量

#### 立即调用（Immediate Invocation）

满足以下条件的立即函数调用称为立即调用：
- 调用的最内层非块作用域不是立即函数的函数参数作用域
- 不是 C++23 `consteval if` 语句的真分支

立即调用**必须**产生常量表达式。

#### 立即函数上下文（Immediate Function Context）

在以下上下文中，对立即函数的调用不需要产生常量表达式：
- 立即函数的函数体内
- C++23 的 `consteval if` 语句真分支

## 4. 底层原理

### 编译时求值机制

`consteval` 函数的工作流程：

```
源代码
    │
    ▼
编译器解析 consteval 函数调用
    │
    ├── 参数是编译时常量？
    │       │
    │       ├── 是 → 编译时求值 → 生成常量
    │       │
    │       └── 否 → 编译错误
    │
    ▼
目标代码（已包含求值结果）
```

### 与 constexpr 的对比

| 特性 | `constexpr` | `consteval` |
|------|-------------|-------------|
| 编译时求值 | 允许，但不强制 | **必须** |
| 运行时求值 | 允许 | 禁止 |
| 参数要求 | 无强制要求 | 必须是编译时常量 |
| 函数指针 | 可以获取 | 可获取但不能用于常量表达式 |
| 隐含属性 | inline | inline |
| 引入版本 | C++11 | C++20 |

### 函数指针处理

可以获取立即函数的指针或引用，但有限制：

```cpp
consteval int f() { return 42; }
consteval auto g() { return &f; }           // OK: 在立即函数内返回指针
consteval int h(int (*p)() = g()) { return p(); }  // OK
constexpr int r = h();                      // OK
constexpr auto e = g();                     // 错误：立即函数指针不能作为常量表达式结果
```

### 编译器实现原理

编译器处理 `consteval` 的步骤：
1. **语法检查**：验证声明是否符合 `consteval` 规则
2. **调用点分析**：识别立即调用
3. **常量表达式求值**：在编译时执行函数并验证结果为常量
4. **错误报告**：如果无法产生常量表达式，报告编译错误

## 5. 使用场景

### 适合使用 consteval 的场景

| 场景 | 说明 |
|------|------|
| 编译时计算 | 确保计算一定在编译时完成 |
| 元编程辅助 | 强制编译时求值，简化模板代码 |
| 性能关键代码 | 保证零运行时开销 |
| 类型安全 | 结合 `static_assert` 进行编译时验证 |
| 配置解析 | 解析编译时常量配置 |

### 不适合使用 consteval 的场景

| 场景 | 推荐替代 | 原因 |
|------|---------|------|
| 参数可能在运行时确定 | `constexpr` | `consteval` 会导致编译错误 |
| 需要运行时分支逻辑 | 普通函数 | 无法满足编译时求值要求 |
| 函数可能有运行时副作用 | 普通函数 | `consteval` 函数必须是纯函数 |

### consteval vs constexpr vs constinit

| 说明符 | 用途 | 求值时机 |
|--------|------|---------|
| `constexpr` | 函数或变量 | 编译时或运行时 |
| `consteval` | 函数 | 必须编译时 |
| `constinit` | 变量（静态初始化） | 编译时初始化 |

### 最佳实践

1. **优先考虑 constexpr**：只有当确实需要强制编译时求值时才使用 `consteval`
2. **参数设计**：确保所有参数都可以在编译时获得
3. **组合使用**：`consteval` 函数可以调用 `constexpr` 函数
4. **错误信息**：利用编译器错误快速定位非编译时常量

### 注意事项

1. **编译时间**：过度使用 `consteval` 可能增加编译时间
2. **调试困难**：编译时错误信息可能复杂
3. **代码可移植性**：需要 C++20 或更高版本编译器
4. **二进制兼容**：立即函数通常是内联的，可能影响 ABI

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

// constexpr 函数：可在编译时或运行时求值
constexpr unsigned factorial(unsigned n)
{
    return n < 2 ? 1 : n * factorial(n - 1);
}

// consteval 函数：必须在编译时求值
consteval unsigned combination(unsigned m, unsigned n)
{
    return factorial(n) / factorial(m) / factorial(n - m);
}

// 编译时断言
static_assert(factorial(6) == 720);
static_assert(combination(4, 8) == 70);

int main(int argc, const char*[])
{
    // constexpr 变量：编译时求值
    constexpr unsigned x{factorial(4)};
    std::cout << x << '\n';  // 输出: 24

    // 运行时求值（argc 在编译时未知）
    [[maybe_unused]]
    unsigned y = factorial(argc);  // OK: constexpr 允许运行时求值

    // 编译错误：consteval 要求编译时常量
    // unsigned z = combination(argc, 7);  // 错误：'argc' 不是常量表达式

    return 0;
}
```

输出：
```
24
```

### 立即函数嵌套调用

```cpp
consteval int sqr(int n)
{
    return n * n;
}

// 立即函数内调用立即函数：不需要结果是常量表达式
consteval int sqrsqr(int n)
{
    return sqr(sqr(n));  // OK: 处于立即函数上下文
}

// constexpr 函数内调用立即函数：必须产生常量
constexpr int dblsqr(int n)
{
    // return 2 * sqr(n);  // 错误：sqr(n) 不是常量表达式
                           // 因为 dblsqr 不是 consteval
    return 2 * n * n;       // OK: 不调用 consteval 函数
}

int main()
{
    constexpr int r = sqr(100);      // OK: 立即调用
    constexpr int r2 = sqrsqr(3);    // OK: 结果为 81
    // int x = 100;
    // int r3 = sqr(x);              // 错误：x 不是常量表达式

    return 0;
}
```

### 函数指针与立即函数

```cpp
consteval int f() { return 42; }

// 返回立即函数指针的立即函数
consteval auto get_func() { return &f; }

// 使用默认参数调用
consteval int call_with_ptr(int (*p)() = get_func())
{
    return p();  // 在立即函数上下文中调用
}

int main()
{
    constexpr int r = call_with_ptr();  // OK: 返回 42

    // 错误：立即函数指针不能作为常量表达式的结果
    // constexpr auto ptr = get_func();

    // 可以获取指针，但只能用于非常量表达式上下文
    auto ptr = get_func();  // OK: 非常量表达式上下文

    return 0;
}
```

### 编译时字符串处理

```cpp
#include <array>
#include <cstddef>

// 编译时计算字符串长度
consteval std::size_t str_len(const char* s)
{
    std::size_t len = 0;
    while (s[len] != '\0') {
        ++len;
    }
    return len;
}

// 编译时字符串哈希
consteval std::size_t str_hash(const char* s)
{
    std::size_t hash = 5381;
    while (*s) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(*s++);
    }
    return hash;
}

int main()
{
    // 编译时计算
    constexpr auto len = str_len("Hello, World!");
    static_assert(len == 13);

    constexpr auto hash = str_hash("test");
    static_assert(hash != 0);  // 编译时验证

    // 编译时用作数组大小
    std::array<char, str_len("hello") + 1> buffer{};

    return 0;
}
```

### 常见错误及修正

#### 错误 1：运行时参数调用 consteval 函数

```cpp
consteval int square(int n) { return n * n; }

int main()
{
    // 错误：运行时变量不能作为 consteval 函数参数
    int x = 5;
    // int result = square(x);  // 编译错误

    // 修正 1：使用常量
    int result1 = square(5);       // OK

    // 修正 2：使用 constexpr 变量
    constexpr int cx = 5;
    int result2 = square(cx);       // OK

    // 修正 3：使用 constexpr 替代 consteval
    // constexpr int square(int n) { return n * n; }
    // int result3 = square(x);     // OK

    return 0;
}
```

#### 错误 2：在 constexpr 函数中调用 consteval 函数

```cpp
consteval int immediate_func(int n) { return n * 2; }

// 错误：constexpr 函数中调用 consteval 函数
// constexpr int wrapper(int n)
// {
//     return immediate_func(n);  // 错误：n 不是常量表达式
// }

// 修正：将 wrapper 也声明为 consteval
consteval int wrapper(int n)
{
    return immediate_func(n);  // OK：处于立即函数上下文
}

int main()
{
    constexpr int r = wrapper(5);  // OK: 返回 10
    return 0;
}
```

#### 错误 3：混合使用 consteval 和 constexpr

```cpp
// 错误：不能同时使用 consteval 和 constexpr
// consteval constexpr int func() { return 1; }

// 正确：只使用其中一个
consteval int func1() { return 1; }    // OK
constexpr int func2() { return 1; }    // OK

// 重声明必须保持一致
consteval int foo();
// constexpr int foo() { return 1; }   // 错误：重声明不一致

int main()
{
    return 0;
}
```

### 高级用法：编译时类型标签

```cpp
#include <iostream>
#include <string_view>

// 编译时字符串转类型标签
template<char... Chars>
struct StringTag {
    static constexpr char value[] = {Chars..., '\0'};
};

// 编译时计算字符串长度
consteval std::size_t ct_strlen(const char* s)
{
    std::size_t len = 0;
    while (s[len] != '\0') ++len;
    return len;
}

// 编译时配置验证
consteval bool validate_config(int mode)
{
    return mode >= 0 && mode <= 3;
}

template<int Mode>
requires validate_config(Mode)
struct Processor {
    void run() const {
        std::cout << "Running in mode " << Mode << "\n";
    }
};

int main()
{
    Processor<1> p;      // OK: 编译时验证通过
    p.run();

    // Processor<5> p2;  // 编译错误：验证失败

    return 0;
}
```

## 7. 总结

### 核心要点

1. **强制编译时求值**：`consteval` 确保函数调用必须产生编译时常量
2. **立即函数概念**：声明的函数称为立即函数，是特殊的 `constexpr` 函数
3. **隐含 inline**：`consteval` 函数默认是内联的
4. **参数限制**：调用时的所有参数必须是编译时常量

### 技术对比

| 特性 | `constexpr` | `consteval` |
|------|-------------|-------------|
| 编译时求值 | 可选 | 强制 |
| 运行时求值 | 允许 | 禁止 |
| 引入版本 | C++11 | C++20 |
| 使用场景 | 通用 | 特定编译时计算 |
| 错误检测 | 运行时/编译时 | 编译时 |

### 使用建议

1. **默认选择 `constexpr`**：对于可能需要灵活性的函数
2. **按需选择 `consteval`**：当编译时求值是硬性要求时
3. **组合使用**：`consteval` 函数内部可调用 `constexpr` 函数
4. **注意编译器支持**：需要 C++20 或更高版本

### 相关概念

| 概念 | 说明 |
|------|------|
| `constexpr` 说明符 | 指定函数或变量可在编译时求值 |
| `constinit` 说明符 | 确保变量使用静态初始化 |
| 常量表达式 | 可在编译时求值的表达式 |
| 立即函数上下文 | 允许非常量调用的特殊上下文 |

### 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/consteval
- C++20 标准: [dcl.consteval]
- P1073R3: Immediate Functions