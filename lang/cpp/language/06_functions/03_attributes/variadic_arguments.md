# 变参函数 (Variadic Arguments)

## 1. 概述

变参函数（Variadic Function）允许函数接受可变数量的参数。当函数参数列表的最后一个参数是省略号 `...` 时，该函数即为变参函数。这一特性源自 C 语言，在 C++ 中得到了保留，并增加了类型安全的替代方案。

变参函数的典型应用场景包括：
- 格式化输出函数（如 `printf`、`scanf`）
- 日志记录函数
- 需要灵活参数数量的通用接口

## 2. 来源与演变

### 历史背景

变参函数的概念最早出现在 C 语言中。1983 年，C++ 引入了变参函数语法，当时省略号前不需要逗号。当 C89 从 C++ 采用函数原型时，C89 将语法改为要求省略号前必须有逗号。

为了兼容性，C++98 同时接受两种风格：
- C++ 风格：`f(int n...)`
- C 风格：`f(int n, ...)`

### 版本演变

| 版本 | 变化内容 |
|------|----------|
| C++98 | 同时支持 C++ 风格（无逗号）和 C 风格（有逗号） |
| C++11 | 引入可变参数模板（Variadic Templates）作为类型安全替代；新增 `va_copy` 宏；`std::nullptr_t` 转换为 `void*` |
| C++20 | 在简写函数模板中，逗号可用于区分变参函数和变参模板 |
| C++26 | 省略号前无逗号的 C++ 风格语法被弃用 |

### 与 C 语言的差异

在 C 语言中（直到 C23），省略号前必须至少有一个命名参数，因此 `int printz(...);` 在 C23 之前无效。而在 C++ 中，这种形式是允许的，尽管传递给此类函数的参数无法被访问。这种特性常被用于 SFINAE 中的回退重载，利用省略号在重载决议中的最低优先级。

## 3. 语法与参数

### 函数声明语法

```cpp
// 标准语法（推荐）
int printx(const char* fmt, ...);

// C++ 原始语法（C++26 起弃用）
int printx(const char* fmt...);  // 省略逗号，与上面等价

// 错误：省略号只能是最后一个参数
int printy(..., const char* fmt);  // 编译错误

// 合法但参数无法移植访问
int printz(...);  // 可用于 SFINAE 回退
```

### 参数说明

| 参数类型 | 说明 |
|----------|------|
| 命名参数 | 省略号之前的固定参数，数量至少 0 个 |
| `...` | 表示可变参数部分，可接受 0 个或多个额外参数 |

### `<cstdarg>` 库设施

访问变参函数参数需要使用 `<cstdarg>` 头文件中的宏和类型：

| 设施 | 说明 |
|------|------|
| `va_list` | 保存 `va_start`、`va_arg`、`va_end`、`va_copy` 所需信息的类型 |
| `va_start` | 启用对变参函数参数的访问（函数宏） |
| `va_arg` | 访问下一个变参函数参数（函数宏） |
| `va_copy` (C++11) | 复制变参函数参数（函数宏） |
| `va_end` | 结束变参函数参数遍历（函数宏） |

### 与变参模板的区别

```cpp
// 变参函数（C 风格）
void f1(int n, ...);

// 变参模板（C++11 起，类型安全）
template<typename... Args>
void f2(Args... args);

// 简写函数模板中的区分（C++20）
void g1(auto...);      // 变参模板：template<class... Ts> void g1(Ts...)
void g2(auto, ...);    // 变参函数：template<class T> void g2(T, ...)
```

## 4. 底层原理

### 默认参数提升 (Default Argument Promotions)

当调用变参函数时，变参列表中的参数会经过额外的转换：

| 原始类型 | 提升后类型 |
|----------|-----------|
| `float` | `double`（浮点提升） |
| `bool`, `char`, `short`, 无作用域枚举 | `int` 或更宽的整数类型（整数提升） |
| `std::nullptr_t` (C++11) | `void*` |

### 参数传递机制

变参函数的参数通过栈或寄存器传递（取决于调用约定）。`va_list` 类型维护一个指针或状态对象，用于遍历这些参数：

```
栈布局（示意）:
+-----------------+
| 命名参数 n       |
+-----------------+
| 变参 arg1       |
+-----------------+
| 变参 arg2       |
+-----------------+
| ...             |
+-----------------+
```

### 类型限制

- 省略号前的最后一个参数不能是引用类型
- 最后一个参数类型必须与默认参数提升后的类型兼容
- 非平凡类型的传递是条件支持的，语义由实现定义

### 重载决议优先级

变参参数在重载决议中具有最低优先级，这使得变参函数常被用作 SFINAE 中的"兜底"选项：

```cpp
template<typename T>
void f(T t, int);        // 优先级 1：精确匹配

template<typename T>
void f(T t, double);     // 优先级 2：浮点转换

template<typename T>
void f(T t, ...);        // 优先级 3：变参（最低）
```

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 格式化 I/O | 如 `printf`、`scanf` 系列函数 |
| 日志函数 | 需要灵活参数的日志记录 |
| SFINAE 回退 | 利用最低重载优先级实现类型检测 |
| C 兼容接口 | 与 C 代码库交互 |

### 不适用场景

| 场景 | 推荐替代 | 原因 |
|------|---------|------|
| 类型安全要求高 | 可变参数模板 | 变参函数缺乏类型检查 |
| 需要访问参数类型 | 可变参数模板 | 变参函数不保留类型信息 |
| 参数类型相同 | `std::initializer_list` | 更安全、语法更清晰 |
| 仅限 C++ 环境 | 可变参数模板 | 性能更优、类型安全 |

### 注意事项

1. **类型安全问题**：变参函数不进行类型检查，错误的格式字符串可能导致未定义行为
2. **参数数量问题**：函数本身无法知道传入了多少参数，需要通过命名参数或约定传递
3. **非 POD 类型限制**：C++11 前，非 POD 类类型参数导致未定义行为；C++11 起条件支持
4. **`va_start` 限制**：不能将参数包或 lambda 捕获作为 `va_start` 的最后一个参数

## 6. 代码示例

### 基础用法

```cpp
#include <cstdarg>
#include <iostream>
#include <string>

// 简单的求和函数，接受可变数量的 int 参数
int sum(int count, ...) {
    va_list args;
    va_start(args, count);  // 初始化 args，count 是最后一个命名参数

    int total = 0;
    for (int i = 0; i < count; ++i) {
        total += va_arg(args, int);  // 获取下一个 int 参数
    }

    va_end(args);  // 清理
    return total;
}

int main() {
    std::cout << sum(3, 1, 2, 3) << std::endl;      // 输出: 6
    std::cout << sum(5, 10, 20, 30, 40, 50) << std::endl;  // 输出: 150
    return 0;
}
```

### 高级用法：格式化日志

```cpp
#include <cstdarg>
#include <cstdio>
#include <string>
#include <ctime>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

void log_message(LogLevel level, const char* format, ...) {
    // 获取时间戳
    std::time_t now = std::time(nullptr);
    char time_buf[20];
    std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    // 级别字符串
    const char* level_str = nullptr;
    switch (level) {
        case LogLevel::INFO:    level_str = "INFO"; break;
        case LogLevel::WARNING: level_str = "WARN"; break;
        case LogLevel::ERROR:   level_str = "ERROR"; break;
    }

    // 输出前缀
    std::printf("[%s] [%s] ", time_buf, level_str);

    // 输出变参内容
    va_list args;
    va_start(args, format);
    std::vprintf(format, args);
    va_end(args);

    std::printf("\n");
}

int main() {
    log_message(LogLevel::INFO, "User %s logged in", "Alice");
    log_message(LogLevel::WARNING, "Memory usage: %d%%", 85);
    log_message(LogLevel::ERROR, "Failed to open file: %s (code: %d)", "config.txt", 2);
    return 0;
}
```

### C++11 替代方案：可变参数模板

```cpp
#include <iostream>

// 使用可变参数模板实现类型安全的求和
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // C++17 折叠表达式
}

// C++11 版本
template<typename T>
T sum_cpp11(T value) {
    return value;
}

template<typename T, typename... Args>
T sum_cpp11(T first, Args... rest) {
    return first + sum_cpp11(rest...);
}

int main() {
    // 类型安全，编译期检查
    std::cout << sum(1, 2, 3, 4, 5) << std::endl;        // 输出: 15
    std::cout << sum(1.5, 2.5, 3.0) << std::endl;       // 输出: 7.0
    // std::cout << sum(1, "hello", 3);  // 编译错误：类型不匹配

    return 0;
}
```

### 使用 std::initializer_list (C++11)

```cpp
#include <initializer_list>
#include <iostream>
#include <numeric>

// 当所有参数类型相同时，initializer_list 更安全
int sum(std::initializer_list<int> numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 0);
}

int main() {
    std::cout << sum({1, 2, 3, 4, 5}) << std::endl;  // 输出: 15
    return 0;
}
```

### 常见错误及修正

#### 错误 1：类型不匹配

```cpp
#include <cstdio>

// ❌ 错误：格式字符串与实际类型不匹配
void bad_example() {
    double value = 3.14;
    printf("Value: %d\n", value);  // 未定义行为！
    // double 被默认提升为 double，但 %d 期望 int
}

// ✅ 修正：使用正确的格式说明符
void good_example() {
    double value = 3.14;
    printf("Value: %f\n", value);  // 正确：%f 对应 double
}
```

#### 错误 2：忘记 va_end

```cpp
#include <cstdarg>

// ❌ 错误：忘记调用 va_end
int bad_sum(int count, ...) {
    va_list args;
    va_start(args, count);
    int total = 0;
    for (int i = 0; i < count; ++i) {
        total += va_arg(args, int);
    }
    // 忘记 va_end(args)！可能导致资源泄漏
    return total;
}

// ✅ 修正：确保调用 va_end
int good_sum(int count, ...) {
    va_list args;
    va_start(args, count);
    int total = 0;
    for (int i = 0; i < count; ++i) {
        total += va_arg(args, int);
    }
    va_end(args);  // 必须调用
    return total;
}
```

#### 错误 3：传递非 POD 类型（C++11 前）

```cpp
#include <cstdarg>
#include <string>

// ❌ C++98 中危险：传递非 POD 类型
void bad_log(const char* fmt, ...) {
    // 如果传入 std::string 对象，行为未定义（C++98）
}

// ✅ 修正方案：使用 C 风格字符串或改用可变参数模板
void good_log(const char* fmt, ...) {
    // 只传递 POD 类型
}

// ✅ 更好的方案：使用可变参数模板（C++11）
template<typename... Args>
void better_log(const char* fmt, Args&&... args) {
    // 类型安全，支持任意类型
}

int main() {
    std::string msg = "hello";
    // bad_log("%s", msg);  // 危险！
    bad_log("%s", msg.c_str());  // 正确：传递 const char*

    better_log("%s", msg);  // C++11：类型安全
    return 0;
}
```

### SFINAE 应用示例

```cpp
#include <type_traits>

// 利用变参函数的最低重载优先级实现类型检测
template<typename T>
struct is_copy_constructible {
private:
    // 如果 T 可复制构造，选择这个（精确匹配优先）
    static auto test(int) -> decltype(T(std::declval<const T&>()), std::true_type{});

    // 变参回退，最低优先级
    static auto test(...) -> std::false_type;

public:
    static constexpr bool value = decltype(test(std::declval<T>()))::value;
};

int main() {
    static_assert(is_copy_constructible<int>::value, "int is copy constructible");
    static_assert(!is_copy_constructible<void>::value, "void is not copy constructible");
    return 0;
}
```

## 7. 总结

变参函数是 C++ 从 C 继承的特性，允许函数接受可变数量的参数，但存在以下局限性：

| 特性 | 变参函数 | 可变参数模板 |
|------|---------|-------------|
| 类型安全 | 无 | 有 |
| 编译期检查 | 无 | 有 |
| 参数类型信息 | 丢失 | 保留 |
| 性能 | 运行时开销 | 编译期展开，无运行时开销 |
| 非平凡类型支持 | 条件支持 | 完全支持 |

### 核心要点

1. **优先使用现代替代方案**：C++11 起，应优先使用可变参数模板（Variadic Templates）或 `std::initializer_list`
2. **注意默认参数提升**：`float` 提升为 `double`，小整数类型提升为 `int`
3. **正确使用 `<cstdarg>`**：必须成对使用 `va_start`/`va_end`
4. **避免非 POD 类型**：传递类类型对象可能导致未定义行为或实现定义行为
5. **SFINAE 回退**：变参函数在重载决议中优先级最低，适合作为 SFINAE 的兜底选项

### 版本建议

- **C++98/03**：仅能使用变参函数处理可变参数，需谨慎使用
- **C++11+**：推荐使用可变参数模板或 `std::initializer_list`
- **C++20+**：简写函数模板中可用逗号区分变参函数和变参模板

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/variadic_arguments
- C++ Standard: [expr.call], [cstdarg.syn]
- Effective C++, Scott Meyers, Item 37