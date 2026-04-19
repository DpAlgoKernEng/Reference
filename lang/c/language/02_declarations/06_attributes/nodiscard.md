# C 属性：nodiscard (C23 起)

## 1. 概述

`nodiscard` 是 C23 标准引入的一个函数属性（attribute），用于提示编译器在函数返回值被丢弃时发出警告。该属性可以应用于函数声明、结构体/联合体声明或枚举声明。

当一个声明为 `nodiscard` 的函数被调用，或者返回声明为 `nodiscard` 的结构体/联合体/枚举的函数被调用时，如果返回值被丢弃（除强制转换为 void 外），编译器会被鼓励发出警告。

## 2. 来源与演变

### 首次引入

`nodiscard` 属性在 **C23** 标准中首次引入，是 C 语言标准属性系统的一部分。

### 历史背景

在 `nodiscard` 出现之前，C 语言开发者面临以下问题：

1. **返回值被意外忽略**：错误码、资源句柄等重要返回值可能被无意中丢弃
2. **运行时错误难以定位**：忽略返回值导致的错误往往在运行时才被发现
3. **缺乏编译时检查**：没有标准化的方式提示开发者注意返回值

`nodiscard` 的出现解决了这些问题：
- 提供编译时警告机制
- 减少因忽略返回值导致的 bug
- 与 C++ 的同名属性保持一致

### 与 C++ 的关系

`nodiscard` 属性源自 **C++17** 标准，C23 将其引入 C 语言，保持了两个语言的兼容性：
- C++17: 首次引入 `[[nodiscard]]`
- C++20: 新增带消息的 `[[nodiscard("message")]]` 形式
- C23: 引入相同语法，包含带消息的形式

## 3. 语法与参数

### 基本语法

| 语法形式 | 说明 |
|---------|------|
| `[[nodiscard]]` | 基本形式，不包含说明信息 |
| `[[__nodiscard__]]` | 带双下划线的替代形式 |
| `[[nodiscard(string-literal)]]` | 带说明消息的形式 |
| `[[__nodiscard__(string-literal)]]` | 带说明消息的替代形式 |

### 参数说明

| 参数 | 说明 |
|------|------|
| `string-literal` | 可选的字符串字面量，用于解释为什么返回值不应被丢弃 |

### 应用位置

`nodiscard` 属性可应用于以下声明：

1. **函数声明**：标记函数的返回值不应被丢弃
2. **结构体声明**：标记该类型的返回值不应被丢弃
3. **联合体声明**：标记该类型的返回值不应被丢弃
4. **枚举声明**：标记该类型的返回值不应被丢弃

### 语法示例

```c
// 函数声明
[[nodiscard]] int get_error_code(void);
[[nodiscard("check return value for errors")]] int open_file(const char *path);

// 结构体声明
struct [[nodiscard]] error_info {
    int status;
    char message[256];
};

// 枚举声明
enum [[nodiscard]] status {
    STATUS_OK = 0,
    STATUS_ERROR = -1
};
```

## 4. 底层原理

### 编译器处理机制

`nodiscard` 是一个**编译时属性**，其处理流程如下：

1. **属性解析**：编译器在解析声明时识别 `nodiscard` 属性
2. **调用检查**：在函数调用表达式中，编译器检查被调用函数是否具有 `nodiscard` 属性
3. **类型检查**：如果返回类型本身声明了 `nodiscard`，编译器同样检查
4. **丢弃值检测**：编译器分析返回值是否被丢弃（即未被使用）
5. **警告生成**：如果满足条件且不是强制转换为 void，则发出警告

### 触发警告的条件

警告在以下情况下会被触发：

```
调用 nodiscard 函数
    ↓
返回值被丢弃（非 void 转换）
    ↓
编译器发出警告
```

**不会触发警告的情况**：
- 返回值被赋值给变量
- 返回值被用于表达式
- 返回值被显式转换为 `(void)`

**会触发警告的情况**：
- 函数调用作为独立语句，返回值未被使用
- 返回值被隐式丢弃

### 编译器实现差异

| 编译器 | 支持版本 | 警告级别 |
|--------|---------|---------|
| GCC | GCC 10+ | `-Wattributes`（默认启用） |
| Clang | Clang 9+ | `-Wignored-attributes`（默认启用） |
| MSVC | VS 2019 16.10+ | `/W4` 或更高 |

## 5. 使用场景

### 适用场景

| 场景 | 原因 |
|------|------|
| 错误码返回函数 | 忽略错误码可能导致程序错误行为 |
| 资源获取函数 | 返回的资源句柄需要被管理 |
| 状态查询函数 | 返回的状态信息影响后续逻辑 |
| 纯函数 | 返回值是函数的唯一目的 |
| 工厂函数 | 返回的新对象需要被使用 |

### 最佳实践

1. **为错误处理函数添加 nodiscard**：确保调用者处理错误码
2. **为资源获取函数添加 nodiscard**：防止资源泄漏
3. **使用消息参数**：提供清晰的说明
4. **避免滥用**：仅在真正需要时使用

### 正确用法示例

```c
// 错误码函数
[[nodiscard]] int file_open(const char *path);

// 带说明消息
[[nodiscard("check if allocation succeeded")]] void* mem_alloc(size_t size);

// 重要状态类型
struct [[nodiscard]] result {
    int value;
    int error_code;
};
```

### 常见陷阱

1. **返回指针时类型本身不是 nodiscard**：函数返回指针，指针指向 nodiscard 类型，这不会触发警告
2. **强制转换为 void 绕过警告**：虽然可以绕过，但不推荐
3. **在 void 返回函数上使用 nodiscard**：无意义，不会触发警告

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>

// 基本的 nodiscard 函数
[[nodiscard]] int get_status(void) {
    return 0;  // 0 表示成功
}

// 带消息的 nodiscard
[[nodiscard("检查分配是否成功")]] void* safe_alloc(size_t size) {
    return malloc(size);
}

int main(void) {
    get_status();  // 编译器警告：忽略 nodiscard 返回值

    int status = get_status();  // OK：返回值被使用
    if (status == 0) {
        printf("Success\n");
    }

    return status;
}
```

### 结构体/枚举标记示例

```c
#include <stdio.h>

// 标记结构体为 nodiscard
struct [[nodiscard]] error_info {
    int status;
    const char *message;
};

// 返回 nodiscard 结构体的函数
struct error_info enable_safety_mode(void) {
    return (struct error_info){0, "Safety enabled"};
}

// 标记枚举为 nodiscard
enum [[nodiscard]] operation_result {
    OP_SUCCESS = 0,
    OP_FAILURE = -1
};

enum operation_result perform_operation(int value) {
    return value >= 0 ? OP_SUCCESS : OP_FAILURE;
}

int main(void) {
    enable_safety_mode();  // 编译器警告

    struct error_info info = enable_safety_mode();  // OK
    printf("Status: %d, Message: %s\n", info.status, info.message);

    perform_operation(42);  // 编译器警告
    (void)perform_operation(-1);  // OK：显式转换为 void

    return 0;
}
```

### 高级用法：完整示例

```c
#include <stdio.h>
#include <stdlib.h>

// 错误处理结构体
struct [[nodiscard("必须检查操作结果")]] result {
    int value;
    int error;
};

// 资源句柄类型
struct [[nodiscard]] file_handle {
    FILE *fp;
    int is_valid;
};

// 带详细消息的函数
[[nodiscard("PURE FUN")]] int strategic_value(int x, int y) {
    return x ^ y;
}

// 返回 nodiscard 结构体
struct result compute(int a, int b) {
    struct result r = {0, 0};
    if (b == 0) {
        r.error = -1;
        return r;
    }
    r.value = a / b;
    return r;
}

// 打开文件
struct file_handle open_file(const char *path) {
    struct file_handle h;
    h.fp = fopen(path, "r");
    h.is_valid = (h.fp != NULL);
    return h;
}

int main(void) {
    // 错误示例：忽略返回值
    // strategic_value(4, 2);  // 警告

    // 正确用法：使用返回值
    int z = strategic_value(0, 0);
    printf("Strategic value: %d\n", z);

    // 正确用法：检查结果
    struct result r = compute(10, 2);
    if (r.error == 0) {
        printf("Result: %d\n", r.value);
    }

    // 正确用法：处理资源句柄
    struct file_handle h = open_file("test.txt");
    if (h.is_valid) {
        printf("File opened successfully\n");
        fclose(h.fp);
    }

    return z;
}
```

### 常见错误及修正

#### 错误 1：忽略错误码

```c
// 错误示例：忽略返回的错误码
[[nodiscard]] int open_device(int id);

void bad_usage(void) {
    open_device(1);  // 警告：忽略 nodiscard 返回值
    // 设备可能打开失败，但代码继续执行...
}

// 正确用法：检查返回值
[[nodiscard]] int open_device(int id);

void good_usage(void) {
    int result = open_device(1);
    if (result != 0) {
        // 处理错误
        printf("Failed to open device\n");
        return;
    }
    // 设备打开成功，继续执行
}
```

#### 错误 2：返回指针的函数

```c
struct [[nodiscard]] error_info { int status; };

// 注意：返回的是指针，不是 nodiscard 类型本身
struct error_info* foo(void) {
    static struct error_info e = {0};
    return &e;
}

void f1(void) {
    foo();  // 不会警告！返回的是指针，不是 nodiscard 结构体
}

// 正确理解：nodiscard 标记的是类型，不是指针
```

#### 错误 3：强制转换为 void 绕过警告

```c
[[nodiscard]] int important_function(void);

void bad_bypass(void) {
    (void)important_function();  // 技术上可行，但不推荐
    // 这明确表示"我知道我要丢弃这个值"
}

// 推荐：如果确实需要丢弃，添加注释说明原因
void acceptable_case(void) {
    // 这里的返回值我们故意忽略，因为...
    (void)important_function();
}
```

## 7. 总结

### 核心要点

`nodiscard` 属性是 C23 标准引入的编译时警告机制，主要作用包括：

| 特性 | 说明 |
|------|------|
| 编译时检查 | 在编译阶段发现潜在问题 |
| 零运行时开销 | 不影响程序运行性能 |
| 可选消息 | 可提供额外说明信息 |
| 灵活应用 | 可用于函数、结构体、联合体、枚举 |

### 版本支持

| 标准 | 状态 |
|------|------|
| C23 | 完整支持 |
| C17 及更早 | 不支持 |

### 使用建议

1. **优先用于错误处理函数**：确保调用者检查错误码
2. **为资源获取函数添加标记**：防止资源泄漏
3. **使用消息参数**：提供清晰的说明帮助开发者理解
4. **不要滥用**：仅在真正需要时使用，避免警告疲劳

### 与 C++ 的兼容性

`nodiscard` 属性在 C 和 C++ 中具有相同的语法和语义，便于代码在两种语言间移植。

### 相关概念

| 属性 | 关系 |
|------|------|
| `[[maybe_unused]]` | 相反用途，抑制未使用警告 |
| `[[deprecated]]` | 标记为废弃，建议不再使用 |
| `[[fallthrough]]` | 标记 switch 穿透意图 |

## 参考资料

- cppreference: https://en.cppreference.com/w/c/language/attributes/nodiscard
- ISO/IEC 9899:2023 (C23 Standard)