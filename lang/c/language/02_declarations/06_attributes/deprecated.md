# C 属性: deprecated (C23 起)

## 1. 概述 (Overview)

`[[deprecated]]` 是 C23 标准引入的属性（attribute），用于标记已弃用的名称或实体。被标记的代码虽然仍然可以使用，但编译器会发出警告，提示开发者这些代码不建议继续使用。

该属性的主要用途：
- 标记即将移除的 API，便于渐进式迁移
- 提供弃用原因和替代方案的提示
- 在大型项目中维护 API 兼容性

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 `[[deprecated]]` 属性引入之前，C 语言缺乏标准化的弃用标记机制。开发者通常采用以下方式：

1. **注释标记**：在注释中说明函数已弃用，但编译器无法识别
2. **编译器扩展**：使用 `__attribute__((deprecated))` (GCC/Clang) 或 `__declspec(deprecated)` (MSVC)
3. **文档说明**：在 API 文档中标注弃用信息

这些方法存在跨平台兼容性问题，不同编译器有不同的语法。

### C23 标准化

`[[deprecated]]` 属性在 **C23** 标准中正式引入，采用 C++11 风格的属性语法。此举带来了：

- **统一语法**：所有符合 C23 标准的编译器使用相同语法
- **跨平台兼容**：代码可在不同编译器间移植
- **可选消息**：支持附带说明字符串，解释弃用原因

### 与 C++ 的关系

C++ 自 C++14 起支持 `[[deprecated]]` 属性。C23 的设计与 C++ 保持一致，便于代码在两种语言间共享。

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```c
[[deprecated]]                    // (1) 无消息版本
[[__deprecated__]]               // (1) 替代形式

[[deprecated("message")]]        // (2) 带消息版本
[[__deprecated__("message")]]   // (2) 替代形式
```

### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| `message` | 字符串字面量 | 可选参数，用于解释弃用原因和/或建议替代实体 |

### 适用范围

`[[deprecated]]` 属性可用于以下声明：

| 实体类型 | 示例 |
|---------|------|
| 结构体/联合体 | `struct [[deprecated]] S;` |
| 类型定义名 | `[[deprecated]] typedef S* PS;` |
| 对象 | `[[deprecated]] int x;` |
| 结构体/联合体成员 | `union U { [[deprecated]] int n; };` |
| 函数 | `[[deprecated]] void f(void);` |
| 枚举类型 | `enum [[deprecated]] E {};` |
| 枚举常量 | `enum { A [[deprecated]], B [[deprecated]] = 42 };` |

### 弃用状态规则

- **新增弃用**：非弃用的名称可以重新声明为弃用
- **不可撤销**：已弃用的名称无法通过重新声明移除弃用属性

```c
void func(void);              // 正常函数
[[deprecated]] void func(void); // OK: 重新声明为弃用

[[deprecated]] void oldFunc(void);
void oldFunc(void);           // 仍然保持弃用状态！
```

## 4. 底层原理 (Underlying Principles)

### 编译器行为

当编译器遇到使用 `[[deprecated]]` 标记的实体时：

1. **编译继续**：代码正常编译，不会产生错误
2. **警告输出**：编译器发出警告信息
3. **消息包含**：如果指定了消息字符串，编译器会在警告中包含该信息

### 实现机制

```c
// 编译器内部处理示意
[[deprecated("Use newFunc() instead.")]]
void oldFunc(void) { /* ... */ }

// 调用时，编译器生成警告
void caller(void) {
    oldFunc();  // 警告: 'oldFunc' is deprecated: Use newFunc() instead.
}
```

### 编译器标志

| 编译器 | 控制警告的标志 |
|--------|---------------|
| GCC | `-Wdeprecated-declarations` (默认启用), `-Wno-deprecated-declarations` (禁用) |
| Clang | 同 GCC |
| MSVC | 警告等级 C4996 |

## 5. 使用场景 (Use Cases)

### 适合使用 deprecated 的场景

| 场景 | 说明 |
|------|------|
| API 演进 | 旧 API 被新 API 替代时，标记旧 API 为弃用 |
| 安全更新 | 发现安全隐患的函数，提示用户迁移 |
| 命名规范变更 | 函数/变量命名不符合新规范时标记弃用 |
| 平台迁移 | 特定平台专有代码逐步移除 |

### 最佳实践

1. **提供替代方案**：在消息字符串中说明应使用哪个函数替代
2. **说明弃用原因**：简要说明为何弃用（安全、性能、命名等）
3. **渐进式弃用**：先标记弃用，若干版本后再移除
4. **版本记录**：在消息中注明弃用开始的版本

```c
// 推荐做法：提供清晰的替代方案和版本信息
[[deprecated("Since v2.0: Use calculateTotal() instead. Will be removed in v3.0.")]]
int calcTotal(int* arr, int len);

// 推荐做法：新的实现
int calculateTotal(const int* arr, size_t len);
```

### 常见陷阱

1. **忘记提供替代方案**：用户不知道应使用什么替代
2. **长期保留弃用代码**：代码库积累大量弃用但未移除的代码
3. **警告泛滥**：过度使用导致警告过多，降低有效性

## 6. 代码示例 (Examples)

### 基础用法

```c
#include <stdio.h>

// 简单的弃用标记
[[deprecated]]
void TriassicPeriod(void)
{
    puts("Triassic Period: [251.9 - 208.5] million years ago.");
}

// 带消息的弃用标记
[[deprecated("Use NeogenePeriod() instead.")]]
void JurassicPeriod(void)
{
    puts("Jurassic Period: [201.3 - 152.1] million years ago.");
}

// 带替代建议的函数弃用
[[deprecated("Use calcSomethingDifferently(int).")]]
int calcSomething(int x)
{
    return x * 2;
}

int main(void)
{
    TriassicPeriod();   // 警告: 'TriassicPeriod' is deprecated
    JurassicPeriod();   // 警告: 'JurassicPeriod' is deprecated: Use NeogenePeriod() instead.
    return 0;
}
```

**可能的编译输出：**

```
Triassic Period: [251.9 - 208.5] million years ago.
Jurassic Period: [201.3 - 152.1] million years ago.

prog.c:23:5: warning: 'TriassicPeriod' is deprecated [-Wdeprecated-declarations]
    TriassicPeriod();
    ^
prog.c:3:3: note: 'TriassicPeriod' has been explicitly marked deprecated here
[[deprecated]]
  ^
prog.c:24:5: warning: 'JurassicPeriod' is deprecated: Use NeogenePeriod() instead. [-Wdeprecated-declarations]
    JurassicPeriod();
    ^
prog.c:9:3: note: 'JurassicPeriod' has been explicitly marked deprecated here
[[deprecated("Use NeogenePeriod() instead.")]]
  ^
2 warnings generated.
```

### 高级用法：结构体和枚举成员弃用

```c
#include <stdio.h>

// 弃用整个结构体
struct [[deprecated("Use ModernConfig instead.")]] LegacyConfig {
    int oldSetting1;
    int oldSetting2;
};

// 弃用结构体成员
struct Config {
    int enabled;
    int timeout;
    [[deprecated("Use 'enabled' field instead.")]]
    int disabled;  // 旧字段，保留兼容性
};

// 弃用枚举常量
enum Status {
    STATUS_OK = 0,
    STATUS_ERROR = 1,
    [[deprecated("Use STATUS_OK instead.")]]
    STATUS_SUCCESS = 0,  // 别名
};

// 弃用枚举类型
enum [[deprecated("Use HttpStatusCode instead.")]] LegacyStatus {
    LEGACY_OK = 200,
    LEGACY_NOT_FOUND = 404
};

int main(void)
{
    struct LegacyConfig cfg;  // 警告
    struct Config c;
    c.disabled = 0;            // 警告

    enum Status s = STATUS_SUCCESS;  // 警告
    return 0;
}
```

### API 版本迁移示例

```c
#include <stdio.h>
#include <string.h>

// 新版本的 API
char* readFileContent(const char* filename, size_t* outSize)
{
    static char buffer[1024];
    // 模拟读取文件
    strcpy(buffer, "file content");
    if (outSize) *outSize = strlen(buffer);
    return buffer;
}

// 旧版本 API（已弃用）
[[deprecated("Since v2.0: Use readFileContent() with size parameter for safety.")]
char* readFile(const char* filename)
{
    return readFileContent(filename, NULL);
}

// 类型定义弃用
[[deprecated("Use size_t for buffer sizes.")]]
typedef unsigned int bufsize_t;

int main(void)
{
    char* content;

    // 旧用法（会警告）
    content = readFile("data.txt");

    // 新用法（推荐）
    size_t size;
    content = readFileContent("data.txt", &size);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：试图移除弃用属性

```c
// ❌ 错误：重新声明不会移除弃用属性
[[deprecated]] void oldFunc(void);

void oldFunc(void);  // 仍然保持弃用状态！

// ✅ 正确做法：如果确实需要恢复使用，应该移除原声明中的 [[deprecated]]
```

#### 错误 2：弃用消息不清晰

```c
// ❌ 错误：消息不够清晰
[[deprecated]]
void process(void);

// ✅ 修正：提供具体替代方案
[[deprecated("Use processV2() instead. See migration guide in docs/api.md.")]]
void process(void);
```

#### 错误 3：对强制转换弃用的忽略

```c
// 即使有弃用警告，代码仍然可以编译运行
// 不要忽略这些警告！

// ✅ 正确做法：在项目中启用 -Werror=deprecated-declarations
// 将弃用警告视为编译错误，强制团队更新代码
```

## 7. 总结 (Summary)

`[[deprecated]]` 属性是 C23 标准提供的重要工具，用于平滑过渡 API 更新：

**核心特性：**
- 编译时警告，非错误（代码仍可运行）
- 支持可选的说明消息
- 统一的跨平台语法

**使用建议：**

| 建议 | 说明 |
|------|------|
| 总是提供替代方案 | 在消息字符串中说明应使用的替代函数 |
| 注明版本信息 | 帮助用户了解弃用时间线 |
| 渐进式迁移 | 先弃用，保留若干版本后再移除 |
| 作为编译错误处理 | 在 CI 中使用 `-Werror=deprecated-declarations` |

**技术对比：**

| 方案 | 优点 | 缺点 |
|------|------|------|
| `[[deprecated]]` | 标准化、跨平台、支持消息 | 需要 C23 编译器 |
| `__attribute__((deprecated))` | 广泛支持 | 编译器扩展，非标准 |
| 注释标记 | 简单 | 无编译器支持 |

## 参考资料

- cppreference: https://en.cppreference.com/w/c/language/attributes/deprecated
- C23 Standard (ISO/IEC 9899:2024)
- C++14 Standard: `[[deprecated]]` attribute