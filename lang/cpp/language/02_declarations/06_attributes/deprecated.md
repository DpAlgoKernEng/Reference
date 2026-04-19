# C++ attribute: deprecated (C++14 起)

## 1. 概述 (Overview)

`[[deprecated]]` 是 C++14 引入的标准属性（attribute），用于标记已弃用的名称或实体。被标记的实体仍然可以使用，但由于某些原因不推荐继续使用。编译器通常会对使用这些实体的代码发出警告。

该属性提供了一种标准化的方式来标记过时代码，帮助开发者逐步迁移到新的 API，同时保持向后兼容性。

## 2. 来源与演变 (Origin and Evolution)

### 首次引入

`[[deprecated]]` 属性首次在 **C++14** 标准中引入，是 C++ 属性系统的一部分。C++11 引入了统一的属性语法 `[[...]]`，C++14 则是第一个添加标准属性的版本。

### 历史背景

在 C++14 之前，开发者需要依赖编译器特定的扩展来标记弃用代码：

```cpp
// GCC/Clang 方式
__attribute__((deprecated)) void oldFunction();

// MSVC 方式
__declspec(deprecated) void oldFunction();

// 跨平台宏定义
#if defined(__GNUC__) || defined(__clang__)
    #define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
    #define DEPRECATED __declspec(deprecated)
#endif
```

C++14 的 `[[deprecated]]` 提供了：
- 标准化的跨平台语法
- 可选的弃用原因说明
- 与编译器无关的统一接口

### C++17 扩展

C++17 将 `[[deprecated]]` 属性扩展支持**枚举器（enumerator）**，允许单独标记枚举值：

```cpp
enum Color {
    Red,
    Green [[deprecated("Use ModernGreen instead")]],
    Blue
};
```

### 标准演进

| 标准 | 条款编号 | 变化 |
|------|---------|------|
| C++14 | 7.6.5 | 首次引入 |
| C++17 | 10.6.4 | 支持枚举器 |
| C++20 | 9.12.4 | 无重大变化 |
| C++23 | 9.12.5 | 无重大变化 |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
[[deprecated]]                          // (1) 无说明版本
[[deprecated( string-literal )]]        // (2) 带说明版本
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `string-literal` | 可选的字符串字面量，用于解释弃用原因或建议替代实体。这是一个不求值（unevaluated）的字符串字面量。 |

### 适用实体

`[[deprecated]]` 属性可用于以下实体的声明：

| 实体类型 | 示例 |
|---------|------|
| 类/结构体/联合体 | `struct [[deprecated]] S;` |
| typedef 名称 | `[[deprecated]] typedef S* PS;` |
| 别名声明 | `using PS [[deprecated]] = S*;` |
| 非成员变量 | `[[deprecated]] int x;` |
| 静态数据成员 | `struct S { [[deprecated]] static constexpr char CR{13}; };` |
| 非静态数据成员 | `union U { [[deprecated]] int n; };` |
| 函数 | `[[deprecated]] void f();` |
| 命名空间 | `namespace [[deprecated]] NS { int x; }` |
| 枚举 | `enum [[deprecated]] E {};` |
| 枚举器 (C++17 起) | `enum { A [[deprecated]], B [[deprecated]] = 42 };` |
| 模板特化 | `template<> struct [[deprecated]] X<int> {};` |

### 重要规则

1. **弃用传播性**：声明为非弃用的名称可以重新声明为弃用
2. **不可撤销**：声明为弃用的名称不能通过重新声明（不带属性）来取消弃用状态

```cpp
// 非弃用 -> 弃用：允许
void func();                     // 非弃用
[[deprecated]] void func();      // OK：可以重新声明为弃用

// 弃用 -> 非弃用：不允许
[[deprecated]] void oldFunc();
void oldFunc();                  // 仍然保持弃用状态
```

## 4. 底层原理 (Underlying Principles)

### 编译器处理机制

`[[deprecated]]` 属性是一个**编译时属性**，其工作原理如下：

1. **声明阶段**：编译器在解析声明时记录实体的弃用状态
2. **使用阶段**：当编译器遇到对弃用实体的引用时，生成警告信息
3. **链接阶段**：属性信息不影响符号链接，仅影响编译诊断

### 属性存储

编译器在内部维护实体的元数据，包括：
- 弃用状态标志
- 可选的弃用说明字符串

这些信息存储在编译器的符号表（symbol table）中，用于后续诊断。

### 诊断行为

| 编译器 | 默认警告级别 | 输出格式 |
|--------|-------------|---------|
| GCC | `-Wdeprecated-declarations` | `warning: 'xxx' is deprecated` |
| Clang | `-Wdeprecated-declarations` | `warning: 'xxx' is deprecated: message` |
| MSVC | `/W3` 或更高 | `warning C4996: 'xxx' was declared deprecated` |

### 性能影响

`[[deprecated]]` 属性：
- **运行时开销**：零开销，属性信息仅在编译期使用
- **编译时开销**：最小，仅增加符号表存储和诊断检查
- **二进制影响**：无影响，不生成额外代码

## 5. 使用场景 (Use Cases)

### 适合使用 `[[deprecated]]` 的场景

| 场景 | 说明 |
|------|------|
| API 版本演进 | 标记旧版 API，引导用户迁移到新版 |
| 函数重命名 | 重命名函数时保留旧名称作为别名 |
| 参数变更 | 参数签名变化时标记旧版本 |
| 设计修正 | 发现设计缺陷后标记问题接口 |
| 平台特定代码 | 标记仅在特定平台有效的代码 |

### 最佳实践

#### 1. 提供迁移指引

```cpp
// 推荐：明确说明替代方案
[[deprecated("Use newFunction(int, int) instead")]]
void oldFunction(int x);

// 不推荐：无说明
[[deprecated]]
void oldFunction(int x);
```

#### 2. 保持向后兼容

```cpp
// 新版函数
void processFile(const std::filesystem::path& path);

// 旧版函数保留，标记弃用
[[deprecated("Use processFile(const std::filesystem::path&) instead")]]
void processFile(const std::string& path) {
    processFile(std::filesystem::path(path));
}
```

#### 3. 版本说明

```cpp
[[deprecated("Since v2.0. Use calculateV2() instead. Will be removed in v3.0.")]]
double calculate(double x);
```

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|---------|
| 过早弃用 | 在替代方案稳定前弃用 | 确保新 API 经过充分测试 |
| 永久弃用 | 弃用但从不移除 | 制定版本路线图，明确移除时间 |
| 无替代方案 | 弃用但无替代方法 | 提供迁移指南或替代代码 |
| 循环依赖 | 新旧 API 相互调用 | 确保迁移路径清晰单向 |

### 线程安全与异常安全

`[[deprecated]]` 属性本身不影响：
- 线程安全性：被标记函数的线程安全特性不变
- 异常安全性：被标记函数的异常保证不变

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

// 无说明版本
[[deprecated]]
void TriassicPeriod()
{
    std::clog << "Triassic Period: [251.9 - 208.5] million years ago.\n";
}

// 带说明版本
[[deprecated("Use NeogenePeriod() instead.")]]
void JurassicPeriod()
{
    std::clog << "Jurassic Period: [201.3 - 152.1] million years ago.\n";
}

[[deprecated("Use calcSomethingDifferently(int).")]]
int calcSomething(int x)
{
    return x * 2;
}

int main()
{
    TriassicPeriod();    // 编译警告
    JurassicPeriod();    // 编译警告（带说明）
}
```

**可能的编译输出**：

```
Triassic Period: [251.9 - 208.5] million years ago.
Jurassic Period: [201.3 - 152.1] million years ago.

main.cpp:20:5: warning: 'TriassicPeriod' is deprecated [-Wdeprecated-declarations]
    TriassicPeriod();
    ^
main.cpp:3:3: note: 'TriassicPeriod' has been explicitly marked deprecated here
[[deprecated]]
  ^
main.cpp:21:5: warning: 'JurassicPeriod' is deprecated: Use NeogenePeriod() instead
    [-Wdeprecated-declarations]
    JurassicPeriod();
    ^
```

### 高级用法

#### 类成员弃用

```cpp
#include <iostream>
#include <string>

class NetworkClient {
public:
    // 新版连接方法
    void connect(const std::string& host, int port);

    // 旧版方法（单参数版本）
    [[deprecated("Use connect(host, port) with explicit port number")]]
    void connect(const std::string& hostAndPort) {
        // 解析并调用新方法
        // 保留向后兼容
    }

    // 弃用的配置选项
    [[deprecated("Use timeout() and setTimeout() instead")]]
    int timeoutMs = 5000;

private:
    int m_timeout = 5000;
};

int main() {
    NetworkClient client;
    client.connect("localhost:8080");  // 警告：建议使用新方法
}
```

#### 枚举值弃用（C++17）

```cpp
#include <iostream>

enum class HttpStatus {
    OK = 200,
    Created = 201,
    BadRequest = 400,
    // 弃用的旧名称
    [[deprecated("Use HttpStatus::BadRequest instead")]]
    InvalidRequest = 400,
    NotFound = 404,
    // 弃用的旧名称
    [[deprecated("Use HttpStatus::NotFound instead")]]
    PageNotFound = 404
};

void handleStatus(HttpStatus status) {
    switch (status) {
        case HttpStatus::OK:
            std::cout << "Success\n";
            break;
        case HttpStatus::BadRequest:
            std::cout << "Bad Request\n";
            break;
        case HttpStatus::NotFound:
            std::cout << "Not Found\n";
            break;
    }
}

int main() {
    handleStatus(HttpStatus::OK);           // 无警告
    handleStatus(HttpStatus::InvalidRequest); // 警告：弃用的枚举值
}
```

#### 命名空间弃用

```cpp
#include <iostream>

// 旧版命名空间
namespace [[deprecated("Use v2::StringUtils instead")]]
v1 {
    void process() {
        std::cout << "v1 processing\n";
    }
}

// 新版命名空间
namespace v2 {
    void process() {
        std::cout << "v2 processing\n";
    }
}

int main() {
    v1::process();  // 警告：弃用的命名空间
    v2::process();  // 无警告
}
```

#### 模板特化弃用

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
struct Serializer {
    static void serialize(const T& value);
};

// 弃用的特化版本
template<>
struct [[deprecated("Use JSON serializer instead")]]
Serializer<char*> {
    static void serialize(const char* value) {
        std::cout << "Serializing C-string: " << value << "\n";
    }
};

// 推荐的新版本
template<>
struct Serializer<std::string> {
    static void serialize(const std::string& value) {
        std::cout << "Serializing string: " << value << "\n";
    }
};

int main() {
    Serializer<char*>::serialize("hello");      // 警告
    Serializer<std::string>::serialize("hello"); // 无警告
}
```

### 常见错误及修正

#### 错误 1：试图取消弃用状态

```cpp
// 头文件 A.h
[[deprecated]] void oldFunc();

// 头文件 B.h（试图取消弃用）
void oldFunc();  // 错误：无法取消弃用状态

// 修正：接受弃用状态或重命名
[[deprecated("Use newFunc() instead")]]
void oldFunc();  // 保持一致
void newFunc();  // 提供替代方案
```

#### 错误 2：弃用声明位置错误

```cpp
// 错误：属性放在定义而非声明处
void deprecatedFunc();
[[deprecated]] void deprecatedFunc() {}  // 位置错误

// 修正：属性放在声明处
[[deprecated]] void deprecatedFunc();
void deprecatedFunc() {}  // 定义不需要重复属性
```

#### 错误 3：无替代方案的弃用

```cpp
// 不推荐：只标记弃用，无替代方案
[[deprecated]]
void criticalFunction();

// 修正：提供替代方案
[[deprecated("Use newCriticalFunction() which handles errors better")]]
void criticalFunction() {
    newCriticalFunction();  // 委托给新实现
}

void newCriticalFunction() {
    // 新实现
}
```

## 7. 总结 (Summary)

### 核心要点

`[[deprecated]]` 属性是 C++ 中标记过时代码的标准机制：

| 特性 | 说明 |
|------|------|
| 引入版本 | C++14 |
| 功能 | 标记实体为弃用状态 |
| 编译行为 | 发出警告（非错误） |
| 运行开销 | 零开销 |
| 字符串说明 | C++14 起支持 |

### 技术对比

| 特性 | `[[deprecated]]` | 编译器扩展 | 注释 |
|------|-----------------|-----------|------|
| 标准化 | 标准 C++ | 编译器特定 | `[[deprecated]]` 更可移植 |
| 字符串说明 | 支持 | 大多支持 | 相同功能 |
| 枚举器支持 | C++17 起 | 编译器依赖 | 标准更统一 |
| 命名空间支持 | 支持 | 部分支持 | 标准更完整 |

### 学习建议

1. **优先使用标准属性**：`[[deprecated]]` 比编译器扩展更可移植
2. **始终提供说明**：帮助用户理解为何弃用及如何迁移
3. **制定迁移计划**：弃用不是终点，需要明确的移除时间表
4. **保持向后兼容**：在弃用期间保留旧接口功能
5. **使用 CI 检查**：将弃用警告纳入 CI 流程，防止新增使用

### 相关属性

| 属性 | 用途 |
|------|------|
| `[[nodiscard]]` | 提醒调用者不要忽略返回值 |
| `[[maybe_unused]]` | 抑制未使用实体警告 |
| `[[fallthrough]]` | 标记 switch 穿透意图 |

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.12.5 Deprecated attribute [dcl.attr.deprecated]
- C++20 标准 (ISO/IEC 14882:2020): 9.12.4 Deprecated attribute [dcl.attr.deprecated]
- C++17 标准 (ISO/IEC 14882:2017): 10.6.4 Deprecated attribute [dcl.attr.deprecated]
- C++14 标准 (ISO/IEC 14882:2014): 7.6.5 Deprecated attribute [dcl.attr.deprecated]
- cppreference: https://en.cppreference.com/w/cpp/language/attributes/deprecated