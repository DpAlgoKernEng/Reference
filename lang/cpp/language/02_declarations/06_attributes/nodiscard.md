# C++ 属性: nodiscard (since C++17) - 禁止丢弃返回值

## 1. 概述

`[[nodiscard]]` 是 C++17 引入的一个属性（attribute），用于标记函数、枚举或类的返回值不应被丢弃。当一个被 `nodiscard` 标记的函数被调用时，如果返回值被丢弃（未使用），编译器会发出警告。这有助于防止开发者无意中忽略重要的返回值，如错误代码、资源句柄等。

`nodiscard` 的字面含义是"不丢弃"，它是一种**编译时诊断工具**，帮助开发者发现潜在的逻辑错误。

## 2. 来源与演变

### 历史背景

在 C++17 之前，开发者经常面临以下问题：

1. **错误代码被忽略**：函数返回错误状态，但调用方忘记检查
2. **资源泄漏风险**：返回资源句柄的函数，返回值被忽略导致资源无法释放
3. **重要信息丢失**：状态查询结果、计算结果被无意忽略

传统解决方案（如 `[[gnu::warn_unused_result]]`）是编译器扩展，缺乏标准化。

### C++17 引入

`[[nodiscard]]` 在 **C++17** 标准中正式引入，作为标准化属性，提供跨编译器的一致支持。

### C++20 增强

C++20 对 `nodiscard` 进行了增强，新增带消息的形式：

```cpp
[[nodiscard("PURE FUN")]] int strategic_value(int x, int y);
```

字符串字面量可用于解释为什么返回值不应被丢弃，编译器会在警告中显示该消息。

### 缺陷报告

| DR | 应用版本 | 原发布行为 | 修正行为 |
|----|----------|------------|----------|
| P1771R1 | C++17 | `[[nodiscard]]` 应用于构造函数无效果 | 当构造的对象被丢弃时，可以触发警告 |

## 3. 语法与参数

### 语法形式

| 语法 | 版本 | 说明 |
|------|------|------|
| `[[nodiscard]]` | C++17 起 | 基本形式 |
| `[[nodiscard(` 字符串字面量 `)]]` | C++20 起 | 带自定义警告消息 |

### 参数说明

| 参数 | 说明 |
|------|------|
| 字符串字面量 | 一个不求值的字符串字面量，用于解释为何返回值不应被丢弃。编译器通常会在警告信息中包含此字符串。 |

### 应用位置

`[[nodiscard]]` 可出现在以下声明中：

1. **函数声明**：标记函数返回值不应被丢弃
2. **枚举声明**：标记枚举类型的返回值不应被丢弃
3. **类声明**：标记类类型的返回值不应被丢弃

### 触发警告的条件

当以下情况发生时，编译器被鼓励发出警告：

1. 一个声明为 `nodiscard` 的函数被调用，且返回值被丢弃
2. 一个返回声明为 `nodiscard` 的枚举或类的函数（按值返回）被调用，且返回值被丢弃
3. 一个声明为 `nodiscard` 的构造函数通过显式类型转换或 `static_cast` 调用，且结果被丢弃
4. 一个声明为 `nodiscard` 的枚举或类类型的对象通过显式类型转换或 `static_cast` 初始化，且结果被丢弃

**注意**：强制转换为 `void` 是例外，不会触发警告：

```cpp
[[nodiscard]] int get_value();
static_cast<void>(get_value());  // OK，显式丢弃，不产生警告
```

## 4. 底层原理

### 编译器行为

`[[nodiscard]]` 是一个**编译时属性**，其工作原理如下：

1. **属性解析**：编译器在解析声明时记录 `nodiscard` 属性
2. **表达式分析**：当分析丢弃值表达式（discarded-value expression）时
3. **类型检查**：检查被丢弃值的类型是否带有 `nodiscard` 属性
4. **警告生成**：如果条件满足，编译器发出诊断警告

### 编译期开销

- **零运行时开销**：属性仅影响编译时诊断，不生成任何运行时代码
- **编译时间影响**：极小，仅在表达式分析阶段增加少量检查

### 标准库应用

C++ 标准库广泛使用 `nodiscard` 属性，主要应用于：

| 类别 | 函数示例 |
|------|----------|
| 内存分配 | `operator new`, `operator new[]`, `allocate` |
| 间接访问 | `std::launder`, `std::assume_aligned` |
| 空容器检查 | `empty()` 方法（各类容器） |
| 异步操作 | `std::async`（C++26 前） |

## 5. 使用场景

### 适合使用 nodiscard 的场景

| 场景 | 原因 | 示例 |
|------|------|------|
| 错误处理 | 返回错误码或状态的函数 | `[[nodiscard]] Error open_file(const char* path);` |
| 资源获取 | 返回资源句柄的函数 | `[[nodiscard]] Handle create_resource();` |
| 状态查询 | 返回重要状态的函数 | `[[nodiscard]] bool is_valid() const;` |
| 工厂函数 | 创建新对象的函数 | `[[nodiscard]] static Object create();` |
| 纯函数 | 无副作用，结果必须使用的函数 | `[[nodiscard]] int calculate_sum(int a, int b);` |

### 最佳实践

1. **用于有意义的返回值**：仅在返回值确实需要被使用时添加
2. **添加解释消息（C++20）**：解释为何不应丢弃
3. **结合文档使用**：在文档中说明返回值的重要性
4. **一致性应用**：对同类函数保持一致的标记策略

### 注意事项

1. **警告非错误**：`nodiscard` 产生的是警告，不是错误，程序仍可编译
2. **显式丢弃**：强制转换为 `void` 可显式丢弃，避免警告
3. **引用返回值**：按引用返回 nodiscard 类型不会触发警告
4. **构造函数**：C++17 原版对构造函数无效果，P1771R1 后修复

### 常见陷阱

```cpp
// 陷阱 1：按引用返回不会触发警告
struct [[nodiscard]] Info {};
Info& get_info() { static Info i; return i; }
void f() { get_info(); }  // 无警告，因为是引用返回

// 陷阱 2：丢弃的是临时对象
struct [[nodiscard]] Resource { ~Resource() { /* cleanup */ } };
Resource create() { return {}; }
void bad() { create(); }  // 警告：资源立即被清理，可能不是预期行为
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <string>

// 基本的 nodiscard 函数
[[nodiscard]] int calculate_area(int width, int height) {
    return width * height;
}

// 带自定义消息的 nodiscard (C++20)
[[nodiscard("必须检查文件是否成功打开")]]
bool open_file(const std::string& filename) {
    std::cout << "Opening " << filename << std::endl;
    return true;  // 简化示例
}

int main() {
    // 触发警告：返回值被丢弃
    calculate_area(5, 3);  // 编译器可能警告

    // 正确使用：存储返回值
    int area = calculate_area(5, 3);
    std::cout << "Area: " << area << std::endl;

    // 触发警告并显示自定义消息
    open_file("data.txt");  // 编译器可能警告：必须检查文件是否成功打开

    // 正确使用
    if (open_file("data.txt")) {
        std::cout << "File opened successfully" << std::endl;
    }

    return 0;
}
```

### 高级用法：类和枚举

```cpp
#include <iostream>
#include <string>

// nodiscard 类类型
struct [[nodiscard]] ErrorInfo {
    int code;
    std::string message;

    ErrorInfo(int c, std::string m) : code(c), message(std::move(m)) {}
};

// 返回 nodiscard 类型的函数
ErrorInfo process_data(int value) {
    if (value < 0) {
        return {1, "Negative value not allowed"};
    }
    return {0, "Success"};
}

// nodiscard 枚举类型
enum class [[nodiscard]] Status {
    OK,
    Error,
    Pending
};

Status perform_operation() {
    return Status::OK;
}

// nodiscard 构造函数
class Resource {
public:
    [[nodiscard]] Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " created\n";
    }
    ~Resource() {
        std::cout << "Resource " << id_ << " destroyed\n";
    }
private:
    int id_;
};

int main() {
    // 警告：ErrorInfo 被丢弃
    process_data(5);  // 编译器警告

    // 正确：使用返回值
    ErrorInfo result = process_data(5);
    if (result.code != 0) {
        std::cout << "Error: " << result.message << std::endl;
    }

    // 警告：Status 被丢弃
    perform_operation();  // 编译器警告

    // 正确：检查状态
    Status status = perform_operation();
    if (status == Status::OK) {
        std::cout << "Operation succeeded\n";
    }

    // 警告：Resource 构造函数结果被丢弃 (C++20 后)
    Resource(42);  // 资源立即销毁，编译器可能警告

    return 0;
}
```

### 常见错误及修正

#### 错误 1：忽略错误返回值

```cpp
// 错误示例
[[nodiscard]] bool initialize_system() {
    return true;  // 返回初始化是否成功
}

void bad() {
    initialize_system();  // 警告：忽略了初始化结果！
    // 系统可能未正确初始化就继续执行
}

// 修正：检查返回值
void good() {
    if (!initialize_system()) {
        // 处理初始化失败
        return;
    }
    // 继续执行，确保系统已初始化
}
```

#### 错误 2：资源泄漏

```cpp
// 错误示例
struct [[nodiscard]] FileHandle {
    void* ptr;
    ~FileHandle() { /* 关闭文件 */ }
};

[[nodiscard]] FileHandle open_file(const char* path);

void bad() {
    open_file("data.txt");  // 警告：句柄被丢弃，文件立即关闭！
    // 文件在下一行就无法访问了
}

// 修正：保存句柄
void good() {
    FileHandle handle = open_file("data.txt");
    // 使用 handle...
    // 离开作用域时自动关闭
}
```

#### 错误 3：显式丢弃的正确用法

```cpp
[[nodiscard]] int get_status() { return 0; }

void example() {
    get_status();  // 警告：忽略了返回值

    // 如果确实想丢弃，显式转换
    static_cast<void>(get_status());  // OK：明确表达意图，无警告

    // 或者在 C++17 中
    (void)get_status();  // 也可以，但 static_cast<void> 更明确
}
```

## 7. 总结

`[[nodiscard]]` 是一个简单但强大的编译时诊断工具，主要作用：

### 核心价值

1. **防止逻辑错误**：强制开发者关注重要返回值
2. **提高代码质量**：在编译期发现潜在问题
3. **零运行时开销**：仅影响编译时行为
4. **跨平台一致**：标准化属性，所有主流编译器支持

### 版本特性

| 特性 | C++17 | C++20 |
|------|-------|-------|
| 基本形式 `[[nodiscard]]` | 支持 | 支持 |
| 带消息形式 `[[nodiscard("msg")]]` | 不支持 | 支持 |
| 构造函数支持 | 有缺陷 | 完整支持 |

### 使用建议

1. 对返回错误状态、资源句柄、重要结果的函数使用 `nodiscard`
2. C++20 中优先使用带消息的形式，提供更多上下文
3. 如果确实需要丢弃返回值，使用 `static_cast<void>()` 显式表达
4. 保持一致：对同类函数采用一致的标记策略

### 相关概念

| 属性 | 用途 |
|------|------|
| `[[maybe_unused]]` | 抑制未使用警告（与 nodiscard 相反的场景） |
| `[[deprecated]]` | 标记已弃用的实体 |
| `[[fallthrough]]` | 标记 switch 穿透意图 |

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.12.9 Nodiscard attribute [dcl.attr.nodiscard]
- C++20 标准 (ISO/IEC 14882:2020): 9.12.8 Nodiscard attribute [dcl.attr.nodiscard]
- C++17 标准 (ISO/IEC 14882:2017): 10.6.7 Nodiscard attribute [dcl.attr.nodiscard]
- cppreference: https://en.cppreference.com/w/cpp/language/attributes/nodiscard