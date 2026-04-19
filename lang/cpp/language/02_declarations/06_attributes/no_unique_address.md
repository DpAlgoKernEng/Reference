# C++ 属性：no_unique_address（C++20 起）

## 1. 概述 (Overview)

`[[no_unique_address]]` 是 C++20 引入的属性（attribute），用于标记非静态数据成员，允许该成员与其他非静态数据成员或基类子对象共享内存地址。这意味着如果成员具有空类类型（如无状态分配器），编译器可以将其优化为不占用任何空间，就像空基类优化一样。

**核心用途**：
- 消除空类成员带来的额外空间开销
- 允许编译器复用成员的尾部填充（tail padding）
- 优化包含无状态成员的类大小

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++20 之前，C++ 已经有**空基类优化**（Empty Base Optimization, EBO）机制，允许空类作为基类时不占用任何空间。然而，这一优化**仅适用于基类**，对于作为成员的空类对象则无法应用。

### 设计动机

考虑以下场景：

```cpp
struct Empty {};  // 空类，sizeof(Empty) >= 1

struct Allocator { /* 无状态分配器，空类 */ };

struct Container {
    int data;
    Allocator alloc;  // 即使 Allocator 为空，仍占用至少 1 字节
};
```

在 C++20 之前，`Container` 的大小至少为 `sizeof(int) + 1`，因为每个对象（包括成员）必须有唯一的地址。这导致了不必要的空间浪费，尤其对于包含多个无状态策略对象的模板类。

### 解决方案

C++20 引入 `[[no_unique_address]]` 属性，将成员标记为"可能重叠"（potentially-overlapping），使其可以与其他成员共享地址，从而实现与空基类优化类似的效果。

### 标准版本

| 标准版本 | 章节 | 说明 |
|---------|------|------|
| C++20 | 9.12.10 [dcl.attr.nouniqueaddr] | 首次引入 |
| C++23 | 9.12.11 [dcl.attr.nouniqueaddr] | 继续沿用 |

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```cpp
[[no_unique_address]]
```

### 用法规则

| 要素 | 说明 |
|-----|------|
| **适用对象** | 非静态数据成员的声明 |
| **不适用** | 位域（bit-field）、静态成员、函数 |
| **位置** | 放置在成员类型声明之前 |

### 语法示例

```cpp
struct MyStruct {
    int i;
    [[no_unique_address]] Empty e;  // 正确：应用于非静态数据成员
};
```

### 编译器兼容性说明

| 编译器 | 属性形式 | 备注 |
|--------|---------|------|
| GCC | `[[no_unique_address]]` | C++20 起支持 |
| Clang | `[[no_unique_address]]` | C++20 起支持 |
| MSVC | `[[msvc::no_unique_address]]` | 标准形式 `[[no_unique_address]]` 被忽略 |

**注意**：MSVC 即使在 C++20 模式下也会忽略 `[[no_unique_address]]`，需要使用 `[[msvc::no_unique_address]]` 代替。

## 4. 底层原理 (Underlying Principles)

### 对象地址唯一性规则

在 C++ 中，同一类型的两个不同对象必须具有不同的地址。这是对象身份识别的基础规则。因此：

- 空类对象的大小至少为 1 字节
- 同类型的两个成员不能共享同一地址

### 可能重叠子对象 (Potentially-overlapping Subobject)

当一个成员被标记为 `[[no_unique_address]]` 时，它成为"可能重叠子对象"。编译器可以：

1. **空类成员优化**：如果成员是空类类型，可以完全不占用空间
2. **尾部填充复用**：如果成员有尾部填充，可用于存储其他成员

### 地址共享限制

同一类型的两个可能重叠子对象**不能共享同一地址**：

```cpp
struct Z {
    char c;
    [[no_unique_address]] Empty e1, e2;  // e1 和 e2 类型相同，不能共享地址
};
// sizeof(Z) >= 2，因为 e1 和 e2 必须有不同地址
```

但不同类型的可能重叠子对象可以共享地址：

```cpp
struct W {
    char c[2];
    [[no_unique_address]] Empty e1, e2;
    // e1 可与 c[0] 共享地址
    // e2 可与 c[1] 共享地址
    // sizeof(W) 可以仅为 2
};
```

### 内存布局对比

| 结构体 | 无 `[[no_unique_address]]` | 有 `[[no_unique_address]]` |
|--------|---------------------------|---------------------------|
| `struct X { int i; Empty e; }` | >= 8 字节（含填充） | >= 5 字节 |
| `struct Y { int i; [[no_unique_address]] Empty e; }` | - | = 4 字节（优化后） |

## 5. 使用场景 (Use Cases)

### 适用场景

1. **无状态分配器**：标准库容器中的分配器通常是空类

```cpp
template<typename T, typename Allocator = std::allocator<T>>
class vector {
    [[no_unique_address]] Allocator alloc_;  // 无状态分配器不占用空间
    T* data_;
    size_t size_, capacity_;
};
```

2. **策略类**：无状态的策略对象

```cpp
template<typename Compare = std::less<int>>
class sorter {
    [[no_unique_address]] Compare comp;  // 比较器可能是无状态的
    // ...
};
```

3. **可选的空成员**：条件性存在的空类型成员

```cpp
template<bool EnableDebug>
class Processor {
    int data_;
    [[no_unique_address]] std::conditional_t<EnableDebug, DebugHelper, Empty> debug_;
};
```

### 最佳实践

- 仅用于空类或希望复用尾部填充的成员
- 配合静态断言验证优化效果
- 注意编译器兼容性

### 常见陷阱

1. **同类型多成员不会完全优化**

```cpp
struct Bad {
    [[no_unique_address]] Empty e1, e2;  // 两者不能共享地址
    // sizeof(Bad) >= 2
};
```

2. **MSVC 兼容性问题**

```cpp
// 不可移植的写法
struct Foo {
    [[no_unique_address]] Empty e;  // MSVC 会忽略
};

// 跨平台写法
struct Foo {
#if defined(_MSC_VER)
    [[msvc::no_unique_address]] Empty e;
#else
    [[no_unique_address]] Empty e;
#endif
};
```

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

struct Empty {};  // 空类

// 未使用 no_unique_address
struct X {
    int i;
    Empty e;
};

// 使用 no_unique_address
struct Y {
    int i;
    [[no_unique_address]] Empty e;
};

int main() {
    // 空类对象至少占用 1 字节
    static_assert(sizeof(Empty) >= 1);

    // X 的大小：int + 至少 1 字节的 Empty + 填充
    static_assert(sizeof(X) >= sizeof(int) + 1);

    // Y 的大小：仅 int，Empty 被优化掉
    std::cout << "sizeof(Y) == sizeof(int): "
              << std::boolalpha << (sizeof(Y) == sizeof(int)) << '\n';

    return 0;
}
```

**输出**：
```
sizeof(Y) == sizeof(int): true
```

### 高级用法：同类型多成员的地址分配

```cpp
#include <iostream>

struct Empty {};

struct Z {
    char c;
    [[no_unique_address]] Empty e1, e2;  // e1 和 e2 同类型，不能共享地址
};

struct W {
    char c[2];
    [[no_unique_address]] Empty e1, e2;  // e1 与 c[0]、e2 与 c[1] 可共享地址
};

int main() {
    // Z 中 e1 和 e2 不能共享同一地址（同类型）
    // 但各自可以与 c 共享地址
    static_assert(sizeof(Z) >= 2);
    std::cout << "sizeof(Z) = " << sizeof(Z) << '\n';

    // W 中 e1 可与 c[0] 共享，e2 可与 c[1] 共享
    std::cout << "sizeof(W) = " << sizeof(W) << '\n';
    std::cout << "sizeof(W) == 2: " << std::boolalpha << (sizeof(W) == 2) << '\n';

    return 0;
}
```

**输出**：
```
sizeof(Z) = 2
sizeof(W) = 2
sizeof(W) == 2: true
```

### 实际应用：容器中的无状态分配器

```cpp
#include <iostream>
#include <memory>

// 自定义无状态分配器
template<typename T>
struct StatelessAllocator {
    using value_type = T;

    T* allocate(size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, size_t) { ::operator delete(p); }
};

// 不使用 no_unique_address
template<typename T>
class VectorOld {
    int* data_;
    size_t size_, capacity_;
    StatelessAllocator<T> alloc_;  // 占用空间
};

// 使用 no_unique_address
template<typename T>
class VectorNew {
    int* data_;
    size_t size_, capacity_;
    [[no_unique_address]] StatelessAllocator<T> alloc_;  // 不占用空间
};

int main() {
    std::cout << "VectorOld<int> size: " << sizeof(VectorOld<int>) << '\n';
    std::cout << "VectorNew<int> size: " << sizeof(VectorNew<int>) << '\n';
    std::cout << "优化效果: 节省 "
              << sizeof(VectorOld<int>) - sizeof(VectorNew<int>) << " 字节\n";

    return 0;
}
```

**输出**（64位系统）：
```
VectorOld<int> size: 24
VectorNew<int> size: 16
优化效果: 节省 8 字节
```

### 常见错误及修正

**错误示例**：错误地应用于静态成员

```cpp
struct Error {
    [[no_unique_address]] static Empty e;  // 错误：不能应用于静态成员
};
```

**修正**：`[[no_unique_address]]` 只能应用于非静态数据成员

```cpp
struct Correct {
    [[no_unique_address]] Empty e;  // 正确：应用于非静态成员
};
```

**错误示例**：期望同类型成员完全优化

```cpp
struct ExpectationError {
    [[no_unique_address]] Empty e1, e2;
    // 错误预期：sizeof == 1 或 0
    // 实际：sizeof >= 2（同类型对象必须有不同地址）
};
```

**修正**：使用不同类型或嵌套结构

```cpp
struct Fixed {
    struct Empty1 : Empty {};
    struct Empty2 : Empty {};
    [[no_unique_address]] Empty1 e1;
    [[no_unique_address]] Empty2 e2;  // 不同类型，可以共享地址
    // sizeof(Fixed) 可以 == 1
};
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|-----|------|
| **引入版本** | C++20 |
| **语法** | `[[no_unique_address]]` |
| **作用** | 允许成员与其他成员共享地址 |
| **主要用途** | 优化空类成员的空间占用 |
| **核心原理** | 将成员标记为"可能重叠子对象" |

### 与空基类优化的对比

| 特性 | 空基类优化 (EBO) | `[[no_unique_address]]` |
|-----|-----------------|------------------------|
| 适用对象 | 基类 | 非静态数据成员 |
| 引入版本 | C++98 | C++20 |
| 使用方式 | 自动（继承即可） | 显式属性标记 |
| 同类型限制 | 无 | 不能共享地址 |

### 学习建议

1. **理解对象地址唯一性规则**：这是理解该属性行为的基础
2. **掌握适用场景**：主要用于无状态对象（分配器、策略类等）
3. **注意编译器差异**：MSVC 需要特殊处理
4. **配合静态断言**：使用 `static_assert` 验证优化效果

### 相关特性

- 空基类优化 (Empty Base Optimization, EBO)
- `alignas` 属性（对齐说明符）
- 标准布局类型 (Standard Layout Type)