# return 语句

## 1. 概述 (Overview)

**return 语句**是 C++ 中用于终止当前函数执行并将控制权（以及可选的返回值）返回给调用者的控制流语句。它是函数返回机制的核心组成部分，在程序控制流中扮演着关键角色。

### 主要用途

- **函数终止**：立即结束当前函数的执行
- **值返回**：将计算结果传递回调用者
- **控制流转移**：将执行权交还给调用栈中的上一级函数
- **协程支持**：通过 `co_return` 支持协程的返回语义（C++20 起）

### 技术定位

return 语句属于**跳转语句 (jump statement)** 的一种，与 `break`、`continue`、`goto` 语句并列。它是 C++ 函数语义的基础设施，涉及对象生命周期管理、复制/移动语义、返回值优化 (RVO) 等多个核心概念。

---

## 2. 来源与演变 (Origin and Evolution)

### C++98 时代

return 语句从 C 语言继承而来，提供了基本的函数返回能力。此时：
- 函数必须显式返回声明类型的值
- void 函数可以省略 return 语句
- 存在序列点 (sequence point) 概念来保证返回值和临时对象析构的顺序

### C++11 标准改进

引入了多项重要改进：
- **列表初始化返回**：支持 `return {args...};` 形式
- **属性支持**：允许在 return 语句前添加属性
- **自动移动语义**：返回局部变量时自动尝试移动而非复制
- **序列化规则**：用"顺序先于 (sequenced-before)"替代序列点概念

### C++14 标准

- **返回类型推导**：支持 `auto` 和 `decltype(auto)` 作为返回类型占位符
- 增强了返回值的类型推导能力

### C++17 标准

- **保证的复制消除 (Guaranteed Copy Elision)**：prvalue 表达式直接初始化返回对象，无需复制/移动构造函数
- 简化了值返回的语义模型

### C++20 标准

- **协程支持**：引入 `co_return` 关键字，用于协程的返回语义
- **扩展的自动移动**：支持从 rvalue reference 参数自动移动

### C++23 标准

- **简化的隐式移动**：move-eligible 表达式直接作为 xvalue 处理
- 移除了双重重载解析机制

### C++26 标准（草案）

- **引用返回安全增强**：禁止将临时表达式的结果绑定到返回的引用

### 版本特性总结表

| 版本 | 特性 | 宏/测试特性 |
|------|------|-------------|
| C++98 | 基本返回语义 | - |
| C++11 | 列表初始化、属性、自动移动 | - |
| C++14 | 返回类型推导 | - |
| C++17 | 保证的复制消除 | - |
| C++20 | 协程 co_return | - |
| C++23 | 简化的隐式移动 | `__cpp_implicit_move >= 202207L` |
| C++26 | 禁止引用绑定临时对象 | - |

### 缺陷报告

| DR | 版本 | 原行为 | 修正行为 |
|-----|------|--------|----------|
| CWG 1541 | C++98 | cv-qualified void 不能省略表达式 | 可以省略 |
| CWG 1579 | C++11 | 不允许转换移动构造函数 | 允许查找转换移动构造函数 |
| CWG 1885 | C++98 | 自动变量销毁顺序不明确 | 添加顺序规则 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```cpp
// (1) 基本形式
attr(optional) return expression(optional) ;

// (2) 列表初始化形式 (C++11 起)
attr(optional) return braced-init-list ;

// (3) 协程返回形式 (C++20 起)
attr(optional) co_return expression(optional) ;

// (4) 协程列表初始化形式 (C++20 起)
attr(optional) co_return braced-init-list ;
```

### 参数说明

| 参数 | 说明 | 版本要求 |
|------|------|----------|
| `attr` | 任意数量的属性序列 | C++11 起 |
| `expression` | 可转换为函数返回类型的表达式 | 所有版本 |
| `braced-init-list` | 花括号包围的初始化列表 | C++11 起 |

### 语法详解

#### (1) 基本形式

```cpp
int add(int a, int b) {
    return a + b;           // 返回表达式结果
}

void log_message() {
    // void 函数可省略 return
}

void early_exit(bool flag) {
    if (flag) return;       // void 函数的提前返回
    // ... 其他代码
}
```

**规则**：
- 表达式可选：在返回类型为 `void`（可有 cv 限定符）的函数中
- 表达式禁止：在构造函数和析构函数中
- 表达式结果需隐式转换为函数返回类型

#### (2) 列表初始化形式

```cpp
std::pair<int, std::string> get_data() {
    return {42, "hello"};   // 复制列表初始化
}

std::vector<int> get_numbers() {
    return {1, 2, 3, 4, 5};  // 直接构造
}
```

**规则**：
- 使用复制列表初始化构造返回值
- 适用于支持初始化列表的类型

#### (3-4) 协程返回形式

```cpp
std::future<int> async_task() {
    co_return 42;           // 协程返回
}
```

**规则**：
- 在协程中使用 `co_return` 代替 `return`
- 对应协程的最终挂起点

### 操作数术语

`expression` 或 `braced-init-list`（C++11 起）被称为 return 语句的**操作数 (operand)**。

### 隐式返回规则

如果控制流到达以下函数的末尾而未遇到 return 语句：
- 返回类型为 void（可有 cv 限定符）的函数
- 构造函数
- 析构函数
- 返回类型为 void 的函数的 function try block

则自动执行 `return;`。

如果控制流到达 main 函数末尾，自动执行 `return 0;`。

如果控制流到达值返回函数（main 函数和特定协程除外）末尾而未遇到 return 语句，行为**未定义**。

---

## 4. 底层原理 (Underlying Principles)

### 执行顺序与生命周期

#### C++11 之前：序列点模型

在 C++11 之前，函数调用结果的复制初始化与表达式末尾所有临时对象的析构之间存在一个**序列点 (sequence point)**。

```cpp
std::string get_string() {
    return std::string("hello") + " world";
    // 序列点：返回值已构造，临时对象尚未析构
}
```

#### C++11 之后：顺序化模型

从 C++11 开始，采用更精确的"顺序先于 (sequenced-before)"关系：

1. 函数调用结果的复制初始化
2. **顺序先于** → 表达式末尾所有临时对象的析构
3. **顺序先于** → return 语句所在块的局部变量析构

```cpp
std::string create_string() {
    std::string local = "test";
    return local + "_suffix";
    // 执行顺序：
    // 1. 返回值构造 (local + "_suffix")
    // 2. 临时对象析构 ("_suffix" 的临时 string)
    // 3. local 析构
}
```

### 自动移动语义 (Automatic Move)

#### 移动条件 (Move-Eligible)

表达式满足以下条件时为**可移动 (move-eligible)**：

1. 表达式是一个（可能带括号的）标识符表达式
2. 该标识符命名一个自动存储期的变量
3. 该变量类型是：
   - 非易变对象类型（C++11 起）
   - 或非易变对象类型的右值引用（C++20 起）
4. 该变量声明在：
   - 最内层函数或 lambda 表达式的函数体中
   - 或作为该函数/lambda 的参数

#### 重载解析机制

**C++11 - C++20：双重重载解析**

```cpp
MoveOnly return_object(MoveOnly arg) {
    return arg;  // arg 是 move-eligible
    // 第一次重载解析：将 arg 视为右值
    // 如果匹配移动构造函数，则使用移动语义
    // 否则第二次重载解析：将 arg 视为左值
    // 使用复制构造函数
}
```

**C++23 起：简化的隐式移动**

```cpp
MoveOnly return_object(MoveOnly arg) {
    return arg;  // arg 直接作为 xvalue
    // 无需双重重载解析
    // 直接选择移动构造函数
}
```

可移动表达式直接作为**亡值 (xvalue)** 处理，简化了重载解析流程。

### 复制消除 (Copy Elision)

#### 保证的复制消除 (C++17 起)

当 `expression` 是**纯右值 (prvalue)** 时：

```cpp
std::string create_string() {
    return std::string("hello");  // prvalue
    // 直接在调用者的返回值位置构造
    // 无复制/移动构造函数调用
}

Widget create_widget() {
    return Widget();  // prvalue
    // 保证无额外复制/移动
}
```

**原理**：prvalue 不再创建临时对象，而是直接指定构造目标位置（延迟求值）。

#### 返回值优化 (RVO/NRVO)

虽然 C++17 只保证 prvalue 的复制消除，但编译器通常会优化命名变量的返回：

```cpp
std::string named_rvo() {
    std::string result = "hello";  // NRVO 候选
    return result;  // 编译器可能消除复制
}
```

### 引用返回的安全性

#### C++26 新规则

如果函数返回引用类型，而 return 语句将返回的引用绑定到临时表达式的结果，程序**非法 (ill-formed)**：

```cpp
// C++26: 编译错误
int& dangerous_return() {
    return 42;  // 错误：绑定临时对象到引用返回
}

// C++26: 编译错误
const int& also_dangerous() {
    return int{42};  // 错误：绑定临时对象
}
```

### 返回类型推导 (C++14 起)

当返回类型为占位符时：

```cpp
auto deduce_return() {
    return 42;  // 推导为 int
}

decltype(auto) preserve_ref() {
    int x = 42;
    return (x);  // 推导为 int&（注意括号）
}

decltype(auto) preserve_val() {
    int x = 42;
    return x;  // 推导为 int
}
```

**规则**：
- `auto`：按值返回，忽略引用和 cv 限定符
- `decltype(auto)`：使用 `decltype` 规则推导，保留引用性

---

## 5. 使用场景 (Use Cases)

### 基本返回场景

#### 1. 值返回函数

```cpp
int calculate(int x, int y) {
    return x + y;  // 返回计算结果
}
```

#### 2. void 函数

```cpp
void process() {
    // 隐式 return; 在函数末尾
}

void early_exit(bool condition) {
    if (condition) {
        return;  // 提前退出
    }
    // 继续处理
}
```

#### 3. 构造函数和析构函数

```cpp
class Widget {
public:
    Widget() {
        // 不允许 return expression;
        // 但可以 return; 提前退出
    }

    ~Widget() {
        // 同样不允许 return expression;
    }
};
```

### 高级返回场景

#### 1. 返回复杂对象

```cpp
std::pair<std::string, int> get_config() {
    return {"server", 8080};  // 列表初始化
}

std::vector<int> get_primes() {
    return {2, 3, 5, 7, 11};  // 初始化列表
}
```

#### 2. 返回局部变量（自动移动）

```cpp
std::string create_message() {
    std::string msg = "Hello";
    msg += " World";
    return msg;  // 自动移动或 NRVO
}

std::unique_ptr<Widget> create_widget() {
    auto ptr = std::make_unique<Widget>();
    return ptr;  // 移动语义
}
```

#### 3. void 函数返回 void 表达式

```cpp
void log_debug() {
    std::cout << "Debug message\n";
}

void wrapper() {
    return log_debug();  // 返回 void 表达式
}
```

#### 4. 协程返回（C++20）

```cpp
#include <coroutine>
#include <future>

std::future<int> async_compute() {
    co_return 42;  // 协程返回
}
```

### 最佳实践

#### 1. 利用 RVO/NRVO

```cpp
// 推荐：返回 prvalue，保证复制消除
std::string get_name() {
    return std::string("Alice");  // C++17 保证无复制
}

// 推荐：返回局部变量，允许编译器优化
std::string build_name() {
    std::string name = "Bob";
    name += " Smith";
    return name;  // NRVO 候选
}
```

#### 2. 按值返回移动语义类型

```cpp
// 正确：按值返回
std::unique_ptr<Resource> create_resource() {
    return std::make_unique<Resource>();  // 移动或复制消除
}

// 错误：返回局部变量的指针/引用
int* bad_return() {
    int local = 42;
    return &local;  // 未定义行为：返回局部变量地址
}
```

#### 3. 明确所有返回路径

```cpp
int classify(int value) {
    if (value < 0) return -1;
    if (value == 0) return 0;
    return 1;  // 确保所有路径都有返回
}
```

### 常见陷阱

#### 1. 返回局部变量的引用

```cpp
// 错误：返回局部变量的引用
int& get_value() {
    int value = 42;
    return value;  // 未定义行为
}

// 正确：按值返回
int get_value() {
    int value = 42;
    return value;  // OK
}

// 正确：返回参数或成员
int& get_ref(int& param) {
    return param;  // OK
}
```

#### 2. 值返回函数缺少 return

```cpp
// 错误：非 void 函数缺少 return
int get_number() {
    // 没有 return 语句
    // 未定义行为！
}

// 特例：main 函数
int main() {
    // 隐式 return 0;
}
```

#### 3. 返回临时对象的引用

```cpp
// 错误（C++26 起编译错误）
const std::string& get_string_ref() {
    return std::string("temp");  // 悬挂引用
}

// 正确：按值返回
std::string get_string() {
    return std::string("temp");  // OK
}
```

#### 4. 移动后使用

```cpp
std::string get_moved() {
    std::string local = "data";
    return std::move(local);  // 不推荐
}

void caller() {
    std::string s = get_moved();
    // local 已被移动，但函数已返回，无问题
    // 但 std::move 是多余的
}

// 推荐：让编译器自动处理
std::string get_auto_move() {
    std::string local = "data";
    return local;  // 编译器自动移动或消除复制
}
```

---

## 6. 代码示例 (Examples)

### 示例 1：基本返回语义

```cpp
#include <iostream>

// void 函数
void fa(int i) {
    if (i == 2)
        return;  // 提前返回
    std::cout << "fa(" << i << ")\n";
}  // 隐式 return;

// 值返回函数
int fb(int i) {
    if (i > 4)
        return 4;
    std::cout << "fb(" << i << ")\n";
    return 2;
}

int main() {
    fa(1);  // 输出：fa(1)
    fa(2);  // 无输出

    int i = fb(5);  // 返回 4
    i = fb(i);      // 输出：fb(4)，返回 2
    std::cout << "i = " << i << '\n';

    return 0;  // main 函数显式返回
}
```

**输出**：
```
fa(1)
fb(4)
i = 2
```

### 示例 2：列表初始化返回

```cpp
#include <iostream>
#include <string>
#include <utility>

std::pair<std::string, int> fc(const char* p, int x) {
    return {p, x};  // 列表初始化
}

void fd() {
    return fa(10);  // 返回 void 表达式
}

int main() {
    auto [str, num] = fc("Hello", 7);
    std::cout << str << ", " << num << '\n';

    fd();  // 调用返回 void 表达式的函数
    return 0;
}
```

**输出**：
```
Hello, 7
fa(10)
```

### 示例 3：自动移动语义

```cpp
#include <iostream>
#include <utility>

struct MoveOnly {
    MoveOnly() { std::cout << "Default ctor\n"; }
    MoveOnly(MoveOnly&&) { std::cout << "Move ctor\n"; }
    MoveOnly(const MoveOnly&) = delete;
};

// C++11 起：自动移动
MoveOnly move_11(MoveOnly arg) {
    return arg;  // OK：隐式移动
}

// C++20 起：从右值引用参数移动
MoveOnly move_20(MoveOnly&& arg) {
    return arg;  // OK since C++20
}

// C++23 起：返回右值引用
MoveOnly&& move_23(MoveOnly&& arg) {
    return std::move(arg);  // OK since C++23
}

int main() {
    MoveOnly obj1 = move_11(MoveOnly{});
    std::cout << "---\n";

    MoveOnly obj2 = move_20(MoveOnly{});
    std::cout << "---\n";

    return 0;
}
```

**输出**：
```
Default ctor
Move ctor
---
Default ctor
Move ctor
---
```

### 示例 4：返回类型推导 (C++14)

```cpp
#include <iostream>
#include <vector>

// auto 推导
auto get_value() {
    return 42;  // int
}

// decltype(auto) 保留引用
decltype(auto) get_ref(int& x) {
    return (x);  // int&
}

decltype(auto) get_val(int x) {
    return x;  // int
}

// 返回复杂类型
auto get_vector() {
    return std::vector<int>{1, 2, 3, 4, 5};
}

int main() {
    auto val = get_value();
    std::cout << "val = " << val << '\n';

    int x = 100;
    decltype(auto) ref = get_ref(x);
    ref = 200;
    std::cout << "x = " << x << '\n';

    auto vec = get_vector();
    std::cout << "vec size = " << vec.size() << '\n';

    return 0;
}
```

**输出**：
```
val = 42
x = 200
vec size = 5
```

### 示例 5：复制消除演示 (C++17)

```cpp
#include <iostream>

struct Tracker {
    Tracker() { std::cout << "Default ctor\n"; }
    Tracker(const Tracker&) { std::cout << "Copy ctor\n"; }
    Tracker(Tracker&&) { std::cout << "Move ctor\n"; }
    ~Tracker() { std::cout << "Dtor\n"; }
};

// 保证的复制消除
Tracker get_prvalue() {
    return Tracker();  // prvalue，无复制/移动
}

// NRVO 候选（非保证但通常优化）
Tracker get_nrvo() {
    Tracker t;
    return t;  // 编译器可能消除复制
}

int main() {
    std::cout << "--- prvalue return ---\n";
    {
        auto obj1 = get_prvalue();
        // C++17 保证：无复制/移动
    }

    std::cout << "\n--- nrvo return ---\n";
    {
        auto obj2 = get_nrvo();
        // 通常：NRVO 优化
    }

    return 0;
}
```

**典型输出**：
```
--- prvalue return ---
Default ctor
Dtor

--- nrvo return ---
Default ctor
Dtor
```

### 示例 6：协程返回 (C++20)

```cpp
#include <iostream>
#include <coroutine>
#include <future>

// 简单的协程返回对象
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

Task simple_coroutine() {
    std::cout << "Coroutine started\n";
    co_return;  // 协程返回
}

// 使用 future 的协程
std::future<int> async_add(int a, int b) {
    co_return a + b;  // 返回计算结果
}

int main() {
    simple_coroutine();
    std::cout << "Coroutine finished\n";

    auto future = async_add(10, 32);
    std::cout << "Result: " << future.get() << '\n';

    return 0;
}
```

### 示例 7：常见错误修正

```cpp
#include <iostream>
#include <string>

// ===== 错误示例 =====

// 错误 1：返回局部变量的引用
int& bad_ref_return() {
    int local = 42;
    return local;  // 未定义行为
}

// 错误 2：非 void 函数缺少 return
int missing_return(int x) {
    if (x > 0) return x;
    // 错误：x <= 0 时缺少 return
    // 未定义行为！
}

// 错误 3：返回临时对象的引用
const std::string& bad_temp_ref() {
    return std::string("temp");  // 悬挂引用
}

// ===== 正确示例 =====

// 修正 1：按值返回
int good_value_return() {
    int local = 42;
    return local;  // OK
}

// 修正 2：确保所有路径返回
int complete_return(int x) {
    if (x > 0) return x;
    return -x;  // 所有路径都有返回
}

// 修正 3：按值返回字符串
std::string good_string_return() {
    return std::string("temp");  // OK，RVO
}

int main() {
    // 使用正确的版本
    int val = good_value_return();
    int abs_val = complete_return(-10);
    std::string str = good_string_return();

    std::cout << val << ", " << abs_val << ", " << str << '\n';
    return 0;
}
```

---

## 7. 总结 (Summary)

### 核心要点

1. **基本功能**：return 语句终止函数执行并返回控制权给调用者
2. **返回值处理**：表达式结果隐式转换为函数返回类型
3. **自动移动**：局部变量和参数在返回时自动尝试移动语义
4. **复制消除**：C++17 保证 prvalue 返回值的复制消除
5. **协程支持**：C++20 引入 `co_return` 用于协程返回

### 版本演进关键点

| 版本 | 关键改进 |
|------|----------|
| C++98 | 基本返回语义 |
| C++11 | 列表初始化、自动移动、属性支持 |
| C++14 | 返回类型推导 |
| C++17 | 保证的复制消除 |
| C++20 | 协程 `co_return`、扩展自动移动 |
| C++23 | 简化的隐式移动机制 |
| C++26 | 禁止引用绑定临时对象 |

### 性能优化策略

1. **优先返回 prvalue**：享受保证的复制消除
2. **信任编译器**：局部变量返回会自动移动/NRVO
3. **避免不必要的 std::move**：干扰 RVO/NRVO
4. **按值返回**：现代 C++ 中通常是最佳选择

### 与其他跳转语句对比

| 特性 | return | break | continue | goto |
|------|--------|-------|----------|------|
| 作用域 | 函数级 | 循环/switch | 循环 | 块级 |
| 返回值 | 支持 | 不支持 | 不支持 | 不支持 |
| 清理语义 | 析构局部对象 | 析构作用域对象 | 析构作用域对象 | 析构作用域对象 |
| 推荐度 | 必需 | 推荐 | 推荐 | 谨慎使用 |

### 学习建议

1. **掌握基础**：理解返回值构造、临时对象析构的顺序
2. **理解移动语义**：学习何时触发自动移动、如何优化返回
3. **实践现代写法**：利用 RVO/NRVO，减少手动优化
4. **避免悬挂引用**：理解对象生命周期，避免返回局部引用
5. **跟进标准演进**：C++17/20/23 的返回语义改进显著

---

## 参考资料

- C++26 标准 (ISO/IEC 14882:2026)
- C++23 标准 (ISO/IEC 14882:2023)
- C++20 标准 (ISO/IEC 14882:2020)
- C++17 标准 (ISO/IEC 14882:2017)
- C++14 标准 (ISO/IEC 14882:2014)
- C++11 标准 (ISO/IEC 14882:2011)
- C++98 标准 (ISO/IEC 14882:1998)
- [Copy elision - cppreference](https://en.cppreference.com/w/cpp/language/copy_elision)
- [Move semantics - cppreference](https://en.cppreference.com/w/cpp/language/move_constructor)
- [Coroutines - cppreference](https://en.cppreference.com/w/cpp/language/coroutines)