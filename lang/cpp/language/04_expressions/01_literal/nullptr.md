# nullptr - 空指针字面量 (C++11 起)

## 1. 概述 (Overview)

`nullptr` 是 C++11 引入的关键字，表示空指针字面量（pointer literal）。它是 `std::nullptr_t` 类型的纯右值（prvalue）。

`nullptr` 可以隐式转换为任意指针类型和任意指向成员指针类型的空指针值。这种设计解决了传统空指针表示方法（如 `NULL` 宏和整数 `0`）存在的类型安全问题。

**核心特性**：
- 类型安全的空指针常量
- 可隐式转换为任意指针类型
- 不可转换为整数类型（避免了重载决议的歧义）

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++11 之前，空指针常量通常使用以下方式表示：

1. **整数 0**：直接使用字面量 `0`
2. **NULL 宏**：由实现定义的空指针常量，通常展开为 `0` 或 `((void*)0)`

### 设计动机

传统表示方法存在严重问题：

```cpp
void f(int);
void f(int*);

f(0);      // 调用 f(int) - 可能不是预期行为
f(NULL);   // 调用 f(int) - 如果 NULL 定义为 0
```

这种歧义性导致程序错误难以发现和维护。

### 解决方案

C++11 引入 `nullptr`：
- 明确的类型：`std::nullptr_t`
- 只能转换为指针类型，不能转换为整数类型
- 解决了重载决议的歧义问题

### 版本支持

| C++ 标准 | 支持情况 |
|---------|---------|
| C++11   | 引入 `nullptr` 关键字 |
| C++14   | 无变更 |
| C++17   | 无变更 |
| C++20   | 无变更 |
| C++23   | 无变更 |

## 3. 语法与参数 (Syntax and Parameters)

### 语法

```
nullptr
```

`nullptr` 是一个关键字，不需要任何参数。

### 类型说明

| 属性 | 说明 |
|-----|------|
| 类型 | `std::nullptr_t` |
| 值类别 | 纯右值（prvalue） |
| 隐式转换 | 可转换为任意指针类型和成员指针类型 |

### 头文件

使用 `nullptr` 不需要包含任何头文件，但使用 `std::nullptr_t` 类型需要：

```cpp
#include <cstddef>  // 包含 std::nullptr_t 定义
```

## 4. 底层原理 (Underlying Principles)

### 类型系统设计

`nullptr` 的核心在于其独特的类型 `std::nullptr_t`：

```cpp
namespace std {
    typedef decltype(nullptr) nullptr_t;
}
```

### 转换规则

**指针转换**：
- `nullptr` 可以隐式转换为任意对象指针类型
- `nullptr` 可以隐式转换为任意成员指针类型
- 转换结果是该类型的空指针值

**禁止的转换**：
- 不能隐式转换为算术类型（如 `int`、`long`）
- 不能隐式转换为 `bool` 以外的非指针类型

### 与 NULL 的区别

```cpp
void f(int);
void f(int*);

f(nullptr);  // 调用 f(int*) - 类型安全
f(NULL);     // 歧义：可能调用 f(int) 或 f(int*)
f(0);        // 调用 f(int) - 明确但可能非预期
```

### 空指针常量定义

空指针常量包括：
1. 整数字面量 `0`
2. 计算结果为 `0` 的常量表达式（C++11 之前）
3. `nullptr`（C++11 起）
4. `std::nullptr_t` 类型的值

## 5. 使用场景 (Use Cases)

### 推荐场景

**1. 初始化指针**

```cpp
int* p = nullptr;           // 推荐
int* q = 0;                 // 不推荐
int* r = NULL;               // 不推荐
```

**2. 指针判空**

```cpp
if (ptr == nullptr) {        // 推荐
    // 处理空指针情况
}

if (!ptr) {                  // 也推荐，更简洁
    // 处理空指针情况
}
```

**3. 函数返回空指针**

```cpp
int* find(int* arr, int size, int value) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == value) {
            return &arr[i];
        }
    }
    return nullptr;          // 推荐
}
```

**4. 模板编程中的类型安全**

```cpp
template<typename T>
void process(T* ptr) {
    if (ptr == nullptr) {
        // 类型安全
    }
}
```

### 最佳实践

1. **总是使用 `nullptr` 而非 `NULL` 或 `0`**：确保类型安全
2. **模板代码中优先使用 `nullptr`**：避免模板实例化时的类型问题
3. **函数重载时使用 `nullptr`**：确保正确的重载决议

### 常见陷阱

**陷阱 1：误用于整数上下文**

```cpp
int x = nullptr;  // 编译错误！不能转换为整数
```

**陷阱 2：模板类型推导**

```cpp
template<typename T>
void func(T arg);

func(nullptr);  // T 推导为 std::nullptr_t，可能不是预期
func(static_cast<int*>(nullptr));  // 明确指定指针类型
```

**陷阱 3：字面量的特殊性质**

`nullptr` 是字面量，但经过函数传递后可能失去"空指针常量"性质：

```cpp
template<class T>
constexpr T clone(const T& t) {
    return t;
}

void g(int*);

g(nullptr);        // 正确：nullptr 是空指针常量
g(clone(nullptr)); // 正确：结果仍是空指针值

// 对于 0 和 NULL：
// g(clone(0));    // 错误：非字面量的 0 不是空指针常量
// g(clone(NULL)); // 错误：同上
```

## 6. 代码示例 (Examples)

### 示例 1：基础用法

```cpp
#include <iostream>

int main() {
    int* ptr = nullptr;

    if (ptr == nullptr) {
        std::cout << "ptr is null" << std::endl;
    }

    // 重置指针
    int value = 42;
    ptr = &value;
    std::cout << "ptr now points to: " << *ptr << std::endl;

    // 再次置空
    ptr = nullptr;

    return 0;
}
```

**输出**：
```
ptr is null
ptr now points to: 42
```

### 示例 2：函数重载中的类型安全

```cpp
#include <iostream>

void process(int value) {
    std::cout << "Processing integer: " << value << std::endl;
}

void process(int* ptr) {
    if (ptr) {
        std::cout << "Processing pointer to: " << *ptr << std::endl;
    } else {
        std::cout << "Processing null pointer" << std::endl;
    }
}

void process(std::nullptr_t) {
    std::cout << "Processing nullptr explicitly" << std::endl;
}

int main() {
    int x = 10;

    process(0);         // 调用 process(int)
    process(&x);        // 调用 process(int*)
    process(nullptr);   // 调用 process(std::nullptr_t)

    return 0;
}
```

**输出**：
```
Processing integer: 0
Processing pointer to: 10
Processing nullptr explicitly
```

### 示例 3：模板编程中的应用

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
void checkPointer(T ptr) {
    if constexpr (std::is_pointer_v<T>) {
        if (ptr == nullptr) {
            std::cout << "Null pointer of type: " << typeid(T).name() << std::endl;
        } else {
            std::cout << "Valid pointer: " << ptr << std::endl;
        }
    } else {
        std::cout << "Not a pointer type" << std::endl;
    }
}

int main() {
    int x = 5;

    checkPointer(&x);       // int*
    checkPointer(nullptr);  // std::nullptr_t
    checkPointer(42);       // int

    return 0;
}
```

### 示例 4：nullptr 保留空指针语义

此示例来自 cppreference，演示 `nullptr` 即使不再是字面量也保留空指针的含义：

```cpp
#include <cstddef>
#include <iostream>

template<class T>
constexpr T clone(const T& t)
{
    return t;
}

void g(int*)
{
    std::cout << "Function g called\n";
}

int main()
{
    g(nullptr);        // 正确
    g(NULL);           // 正确（如果 NULL 是字面量）
    g(0);              // 正确

    g(clone(nullptr)); // 正确
//  g(clone(NULL));    // 错误：非字面量的零不能作为空指针常量
//  g(clone(0));       // 错误：非字面量的零不能作为空指针常量
}
```

**输出**：
```
Function g called
Function g called
Function g called
Function g called
```

### 示例 5：常见错误对比

```cpp
#include <iostream>

void f(int) {
    std::cout << "f(int) called" << std::endl;
}

void f(int*) {
    std::cout << "f(int*) called" << std::endl;
}

int main() {
    // 错误示范：使用 0 或 NULL 可能导致歧义
    f(0);        // 调用 f(int)，可能非预期
    f((int*)0);  // 调用 f(int*)，需要显式转换

    // 正确做法：使用 nullptr
    f(nullptr); // 明确调用 f(int*)

    return 0;
}
```

**输出**：
```
f(int) called
f(int*) called
f(int*) called
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|-----|------|
| 类型 | `std::nullptr_t`，专用的空指针类型 |
| 引入版本 | C++11 |
| 主要优势 | 类型安全、解决重载歧义 |
| 隐式转换 | 可转换为任意指针类型，不可转换为整数 |

### nullptr vs NULL vs 0 对比

| 特性 | nullptr | NULL | 0 |
|-----|---------|------|---|
| 类型 | `std::nullptr_t` | 实现定义（通常 `int` 或 `void*`） | `int` |
| 类型安全 | 是 | 否 | 否 |
| 重载安全 | 是 | 否 | 否 |
| 可移植性 | 高 | 中等 | 高 |
| C++ 版本要求 | C++11 起 | 所有版本 | 所有版本 |

### 学习建议

1. **现代 C++ 优先使用 `nullptr`**：所有需要空指针的地方都应使用 `nullptr`
2. **理解类型系统**：`nullptr` 的类型安全设计是 C++ 类型系统的重要进步
3. **避免使用 NULL 和 0 作为空指针**：它们在现代 C++ 中已被废弃
4. **注意模板类型推导**：传递 `nullptr` 给模板函数时注意类型推导结果

### 相关资源

- **标准参考**：
  - C++23 [conv.ptr] 7.3.12 指针转换
  - C++11 [conv.ptr] 4.10 指针转换

- **相关内容**：
  - `NULL` 宏：实现定义的空指针常量
  - `std::nullptr_t`：空指针字面量 `nullptr` 的类型定义
  - C23 标准也引入了 `nullptr` 关键字

### 迁移指南

**从旧代码迁移**：

```cpp
// 旧代码
int* p = 0;
int* q = NULL;

// 新代码
int* p = nullptr;
int* q = nullptr;
```

**搜索替换策略**：
- 全局搜索 `= NULL` 替换为 `= nullptr`
- 全局搜索 `= 0` 需要谨慎，确认是指针初始化再替换
- 使用静态分析工具检测不安全的空指针使用