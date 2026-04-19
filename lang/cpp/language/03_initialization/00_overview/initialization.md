# 初始化 (Initialization)

## 1. 概述 (Overview)

**初始化 (Initialization)** 是指在对象构造时为其提供初始值的过程。这是 C++ 中一个基础而关键的概念，直接影响对象的初始状态和程序的正确性。

初始化可以在以下场景中发生：
- 变量声明时的初始化器（initializer）部分
- `new` 表达式中
- 函数调用时：函数参数和函数返回值也会被初始化

初始化与赋值（assignment）是两个不同的概念：
- **初始化**：对象创建时赋予初始值
- **赋值**：已存在对象被赋予新值

## 2. 来源与演变 (Origin and Evolution)

### C++98 标准

初始化机制在 C++98 中首次标准化，提供了以下初始化方式：
- 默认初始化（default initialization）
- 复制初始化（copy initialization）
- 直接初始化（direct initialization）
- 聚合初始化（aggregate initialization）

### C++11 变化

C++11 引入了**列表初始化（list initialization）**，使用花括号 `{}` 语法：
- 统一了初始化语法
- 新增 `std::initializer_list` 支持
- 引入了值初始化（value initialization）的完善语义

### C++14 变化

- 变量模板（variable templates）的初始化规则被明确

### C++17 变化

- 内联变量（inline variables）的初始化规则
- 部分有序动态初始化（partially-ordered dynamic initialization）
- 明确了不同翻译单元间静态初始化的顺序规则

### C++20 变化

- 引入**指定初始化器（designated initializer）**，允许按名称初始化聚合类型的特定成员

### 缺陷报告

| 缺陷编号 | 适用版本 | 原行为 | 修正行为 |
|---------|---------|--------|---------|
| CWG 270 | C++98 | 类模板静态数据成员初始化顺序未指定 | 明确为无序（显式特化和定义除外） |
| CWG 441 | C++98 | 非局部引用不总是在动态初始化前初始化 | 视为静态初始化，总在动态初始化前完成 |
| CWG 1415 | C++98 | 块作用域 extern 变量声明可以是定义 | 禁止（此类声明不允许初始化器） |
| CWG 2599 | C++98 | 不明确函数参数求值是否属于初始化 | 明确属于初始化的一部分 |

## 3. 语法与参数 (Syntax and Parameters)

### 初始化器语法

对于每个声明符，初始化器（如果存在）可以是以下形式之一：

| 语法形式 | 名称 | 版本 |
|---------|------|------|
| `=` expression | 复制初始化语法 | - |
| `= {}` / `= {` initializer-list `}` / `= {` designated-initializer-list `}` | 列表初始化语法（等号形式） | C++11 起 / C++20 指定初始化器 |
| `(` expression-list `)` / `(` initializer-list `)` | 直接初始化语法 | C++11 前 / C++11 起 |
| `{}` / `{` initializer-list `}` / `{` designated-initializer-list `}` | 列表初始化语法（花括号形式） | C++11 起 / C++20 指定初始化器 |

### 术语定义

| 术语 | 说明 |
|------|------|
| expression | 任意表达式（非括号包围的逗号表达式除外） |
| expression-list | 逗号分隔的表达式列表 |
| initializer-list | 逗号分隔的初始化子句列表 |
| designated-initializer-list | 逗号分隔的指定初始化器子句列表 |

### 初始化子句语法

初始化子句（initializer clause）可以是：

| 语法 | 版本 |
|------|------|
| expression | - |
| `{}` | - |
| `{` initializer-list `}` | - |
| `{` designated-initializer-list `}` | C++20 起 |

语法 (2-4) 统称为**花括号包围的初始化器列表（brace-enclosed initializer list）**。

### 初始化语义

初始化的语义规则如下：

**无初始化器的情况**：
- 对象：默认初始化
- 引用：程序非良构（ill-formed）

**空括号初始化器 `()`**：
- 对象：值初始化
- 引用：程序非良构

**有初始化器的情况**：
1. 若被初始化实体是引用，参见引用初始化规则
2. 若被初始化实体是对象（设类型为 `T`）：
   - 语法 (1)：复制初始化
   - 语法 (2) 或 (4)（C++11 起）：列表初始化
   - 语法 (3)：直接初始化

### 初始化类型对照表

| 初始化类型 | 语法示例 | 说明 |
|-----------|---------|------|
| 默认初始化 | `T x;` | 无初始化器 |
| 值初始化 | `T x();` / `T x{};` | 空括号或花括号 |
| 复制初始化 | `T x = expr;` | 等号后跟表达式 |
| 直接初始化 | `T x(expr);` | 括号包围参数 |
| 列表初始化 | `T x{args};` / `T x = {args};` | 花括号包围（C++11 起） |

## 4. 底层原理 (Underlying Principles)

### 非局部变量的初始化阶段

所有具有静态存储期的非局部变量在 `main` 函数开始执行前初始化（除非延迟初始化）。所有具有线程局部存储期的非局部变量在线程函数开始执行前初始化。

初始化分为两个阶段：

#### 静态初始化 (Static Initialization)

静态初始化包含两种形式：

1. **常量初始化（Constant Initialization）**：
   - 如果可能，优先应用常量初始化
   - 通常在编译时完成，对象表示作为程序映像的一部分存储
   - 如果编译器不这样做，仍需保证在动态初始化之前完成

2. **零初始化（Zero Initialization）**：
   - 否则，非局部静态和线程局部变量被零初始化
   - 需要零初始化的变量放置在 `.bss` 段
   - 不占用磁盘空间，程序加载时由操作系统清零

#### 动态初始化 (Dynamic Initialization)

静态初始化完成后，动态初始化按以下情况发生：

| 类型 | 适用对象 | 特点 |
|------|---------|------|
| 无序动态初始化 | 类模板静态数据成员、变量模板（C++14） | 与其他动态初始化顺序不确定 |
| 部分有序动态初始化 | 非隐式/显式实例化的内联变量（C++17） | 同一翻译单元内有顺序保证 |
| 有序动态初始化 | 其他非局部变量 | 按源代码定义顺序 |

**跨翻译单元顺序问题**：
- 不同翻译单元的静态变量初始化顺序不确定
- 不同翻译单元的线程局部变量初始化是无序的

如果非局部变量的初始化抛出异常，将调用 `std::terminate`。

### 早期动态初始化

编译器可以将动态初始化变量作为静态初始化的一部分（编译时），条件：
1. 动态版本不改变任何其他命名空间作用域对象的值
2. 静态版本产生与动态初始化相同的值

```cpp
inline double fd() { return 1.0; }

extern double d1;

double d2 = d1;   // 未指定行为：
                  // 如果 d1 动态初始化，则 d2 动态初始化为 0.0
                  // 如果 d1 静态初始化，则 d2 可能动态初始化为 1.0
                  // 或静态初始化为 0.0

double d1 = fd(); // 可能静态或动态初始化为 1.0
```

### 延迟动态初始化

动态初始化是否发生在 `main` 第一条语句之前（静态变量）或线程初始函数之前（线程局部变量），还是延迟到之后，由实现定义。

如果非内联变量（C++17 起）的初始化被延迟：
- 它发生在同一翻译单元中任何静态/线程存储期变量首次 ODR 使用之前

如果内联变量（C++17 起）的初始化被延迟：
- 它发生在该变量首次 ODR 使用之前

## 5. 使用场景 (Use Cases)

### 适用场景

| 场景 | 推荐方式 | 原因 |
|------|---------|------|
| 简单变量初始化 | 复制初始化 `T x = val;` | 语法简洁直观 |
| 构造函数参数传递 | 直接初始化 `T x(args);` | 避免额外的复制 |
| 容器初始化 | 列表初始化 `std::vector<int> v{1,2,3};` | 统一语法，避免窄化转换 |
| 聚合类型初始化 | 列表初始化或指定初始化器 | 清晰明确 |
| 需要防止窄化转换 | 列表初始化 `{}` | 编译时检查 |
| 类成员初始化 | 成员初始化列表或默认成员初始化器 | 保证初始化顺序正确 |

### 最佳实践

#### 1. 优先使用列表初始化（C++11 起）

```cpp
// 推荐：列表初始化防止窄化转换
int x{42};           // OK
int y{3.14};         // 编译错误：窄化转换
std::vector<int> v{1, 2, 3};  // 清晰简洁

// 避免：可能发生隐式窄化转换
int z = 3.14;        // 警告或静默截断
```

#### 2. 避免使用空括号导致的最令人恼火的解析 (Most Vexing Parse)

```cpp
// 错误！这声明了一个函数
std::string s();     // 函数声明，不是初始化

// 正确方式
std::string s{};     // 值初始化（C++11 起）
std::string s;       // 默认初始化
```

#### 3. 静态局部变量实现线程安全的延迟初始化

```cpp
Singleton& getInstance() {
    static Singleton instance;  // C++11 起保证线程安全
    return instance;
}
```

### 常见陷阱

#### 陷阱 1：跨翻译单元的初始化顺序问题

```cpp
// 文件 A.cpp
extern int globalValue;
int dependentValue = globalValue + 1;  // 危险！globalValue 可能未初始化

// 文件 B.cpp
int globalValue = calculateValue();  // 动态初始化

// 解决方案：使用函数封装
int getGlobalValue() {
    static int value = calculateValue();  // 首次调用时初始化
    return value;
}
```

#### 陷阱 2：未初始化的变量

```cpp
int x;           // 默认初始化，值未定义（非类类型）
std::string s;   // 默认初始化，调用默认构造函数，值为空串
int* p;          // 默认初始化，指针值未定义
```

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <string>
#include <iostream>

int main() {
    // 默认初始化
    std::string s1;           // 空字符串

    // 复制初始化
    std::string s3 = "hello"; // 调用复制构造函数

    // 直接初始化
    std::string s4("hello");  // 直接调用构造函数

    // 列表初始化（C++11 起）
    std::string s5{'a'};      // 直接列表初始化

    // 聚合初始化
    char a[3] = {'a', 'b'};   // a[2] 为 '\0'

    // 引用初始化
    char& c = a[0];           // c 引用 a[0]

    std::cout << "s1: " << s1 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    std::cout << "s4: " << s4 << std::endl;
    std::cout << "s5: " << s5 << std::endl;
    std::cout << "c: " << c << std::endl;

    return 0;
}
```

### 非局部变量初始化示例

```cpp
// File: initialization_order.cpp

#include <iostream>

// 静态初始化（常量初始化）
const int kConstValue = 42;

// 静态初始化（零初始化）
int globalZero;  // 初始化为 0

// 动态初始化
int globalDynamic = []() {
    std::cout << "Dynamic initialization" << std::endl;
    return 100;
}();

// 延迟初始化演示
struct Logger {
    Logger() { std::cout << "Logger created" << std::endl; }
    void log(const char* msg) { std::cout << "Log: " << msg << std::endl; }
};

int main() {
    std::cout << "main() started" << std::endl;
    std::cout << "kConstValue: " << kConstValue << std::endl;
    std::cout << "globalZero: " << globalZero << std::endl;
    std::cout << "globalDynamic: " << globalDynamic << std::endl;
    return 0;
}

// 输出顺序：
// Dynamic initialization
// main() started
// kConstValue: 42
// globalZero: 0
// globalDynamic: 100
```

### 高级用法：使用函数封装解决初始化顺序问题

```cpp
#include <iostream>

// 危险方式：可能因初始化顺序问题导致未定义行为
// extern FileManager& getFileManager();
// Logger logger(getFileManager().getLogPath());  // getFileManager 可能未初始化

// 安全方式：使用函数封装
class FileManager {
public:
    static FileManager& getInstance() {
        static FileManager instance;  // C++11 起线程安全
        return instance;
    }

    std::string getLogPath() const {
        return "/var/log/app.log";
    }

private:
    FileManager() = default;
};

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance(FileManager::getInstance().getLogPath());
        return instance;
    }

    void log(const std::string& msg) {
        std::cout << "[" << path_ << "] " << msg << std::endl;
    }

private:
    explicit Logger(const std::string& path) : path_(path) {}
    std::string path_;
};

int main() {
    Logger::getInstance().log("Application started");
    return 0;
}
```

### C++20 指定初始化器示例

```cpp
#include <iostream>

struct Config {
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    std::string title = "Window";
};

int main() {
    // C++20 指定初始化器
    Config cfg{
        .width = 1920,
        .height = 1080,
        .title = "My Application"
        // fullscreen 使用默认值 false
    };

    std::cout << "Width: " << cfg.width << std::endl;
    std::cout << "Height: " << cfg.height << std::endl;
    std::cout << "Fullscreen: " << cfg.fullscreen << std::endl;
    std::cout << "Title: " << cfg.title << std::endl;

    return 0;
}
```

### 常见错误及修正

#### 错误 1："最令人恼火的解析"

```cpp
#include <string>

class Widget {
public:
    Widget() = default;
    explicit Widget(int value) : value_(value) {}
private:
    int value_ = 0;
};

int main() {
    // 错误：这声明了一个函数，不是变量！
    // std::string s();  // 函数声明

    // 修正：使用花括号（C++11 起）
    std::string s{};

    // 错误：同样是函数声明
    // Widget w();  // 函数声明

    // 修正方式 1：花括号
    Widget w1{};

    // 修正方式 2：直接省略括号
    Widget w2;

    return 0;
}
```

#### 错误 2：跨翻译单元初始化顺序依赖

```cpp
// ============
// == File 1 ==
#include "a.h"
#include "b.h"

B b;
A::A() { b.Use(); }  // 危险：b 可能未初始化

// ============
// == File 2 ==
#include "a.h"

A a;

// ============
// == File 3 ==
#include "a.h"
#include "b.h"

extern A a;
extern B b;

int main() {
    a.Use();
    b.Use();
}

// 问题：如果 a 在 main 之前初始化，b 可能仍未初始化
// 解决方案：使用函数封装（参见高级用法示例）
```

## 7. 总结 (Summary)

### 核心要点

初始化是 C++ 中赋予对象初始值的关键机制，主要包括：

| 初始化类型 | 语法 | 主要用途 |
|-----------|------|---------|
| 默认初始化 | `T x;` | 无显式初始值 |
| 值初始化 | `T x{};` / `T x();` | 确保零值或默认构造 |
| 复制初始化 | `T x = expr;` | 从表达式初始化 |
| 直接初始化 | `T x(args);` | 直接调用构造函数 |
| 列表初始化 | `T x{args};` | 统一初始化语法（C++11 起） |

### 初始化阶段

对于非局部变量：
1. **静态初始化**：编译时或程序加载时完成
   - 常量初始化优先
   - 否则零初始化
2. **动态初始化**：运行时完成
   - 注意跨翻译单元的顺序问题

### 最佳实践建议

1. **优先使用列表初始化 `{}`**：统一语法，防止窄化转换
2. **避免使用 `T x()`**：会被解析为函数声明
3. **跨翻译单元依赖使用函数封装**：解决静态初始化顺序问题
4. **理解默认初始化的行为**：内置类型值未定义，类类型调用默认构造函数
5. **C++20 指定初始化器**：提高聚合类型初始化的可读性

### 相关概念

| 概念 | 关系 |
|------|------|
| 复制消除 (Copy Elision) | 优化初始化过程中的复制操作 |
| 转换构造函数 (Converting Constructor) | 隐式转换时使用的构造函数 |
| 复制构造函数 (Copy Constructor) | 复制初始化时调用 |
| 默认构造函数 (Default Constructor) | 默认初始化和值初始化时调用 |
| 移动构造函数 (Move Constructor) | 移动语义相关初始化 |
| `new` 表达式 | 动态内存分配时的初始化 |

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/initialization
- C++ Standard: [basic.start]
- Effective C++, Scott Meyers, Item 4: Make sure that objects are initialized before they're used