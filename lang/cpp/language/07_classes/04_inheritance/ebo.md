# 空基类优化 (Empty Base Optimization, EBO)

## 1. 概述

**空基类优化** (Empty Base Optimization, EBO) 是 C++ 编译器的一种优化技术，它允许空基类子对象在派生类对象中占用零字节空间。

在 C++ 中，即使是空类（没有非静态数据成员的类），其对象大小也至少为 1 字节。这是为了保证同一类型的不同对象实例拥有不同的地址。然而，当空类作为基类时，编译器可以将其完全从对象布局中优化掉，使得派生类对象不会因为继承空基类而增加额外的大小。

### 核心概念

| 概念 | 说明 |
|------|------|
| 空类 (Empty Class) | 没有非静态数据成员的类或结构体 |
| 基类子对象 (Base Subobject) | 派生类对象中包含的基类部分 |
| EBO | 编译器将空基类子对象优化为 0 字节的技术 |

## 2. 来源与演变

### C++98 标准引入

空基类优化从 C++98 标准开始就被允许。当时的 C++ 标准规定：
- 任何完整对象或成员子对象的大小至少为 1 字节
- 但基类子对象不受此限制

### C++11 变化

C++11 对标准布局类型 (StandardLayoutType) 提出了明确要求，其中包含 EBO 的相关规定：
- 标准布局类型要求所有非静态数据成员声明在同一个类中
- 不能有与第一个非静态数据成员相同类型的基类

这些要求确保了 `reinterpret_cast` 转换后的指针能够正确指向初始成员。

### C++20 变化

C++20 引入了 `[[no_unique_address]]` 属性，将 EBO 的思想扩展到成员子对象：
- 使用此属性标记的空类成员可以被优化为 0 字节
- 这是对 EBO 技术的进一步扩展

### 设计动机

EBO 的设计动机主要来自于标准库的实现需求：
1. **内存效率**：无状态分配器 (stateless allocator) 不应增加容器大小
2. **性能优化**：减少内存占用，提高缓存命中率
3. **类型安全**：保持 C++ 类型系统的完整性

## 3. 语法与参数

### 基本规则

空基类优化遵循以下规则：

```
规则 1: 空类对象大小 >= 1 字节
规则 2: 空基类子对象可以被优化为 0 字节
规则 3: EBO 在特定条件下被禁止
```

### EBO 禁止条件

当满足以下条件时，EBO 被禁止：

| 条件 | 说明 |
|------|------|
| 基类类型与第一个非静态数据成员类型相同 | 两个相同类型的基类子对象必须有不同的地址 |
| 基类是第一个非静态数据成员的基类 | 同上原因 |

### C++20 新语法：[[no_unique_address]]

```cpp
struct Empty {};  // 空类

struct X {
    int i;
    [[no_unique_address]] Empty e;  // 空成员可被优化
};

// sizeof(X) == sizeof(int)  (EBO 扩展到成员)
```

**属性说明**：

| 属性 | 版本 | 作用 |
|------|------|------|
| `[[no_unique_address]]` | C++20 | 允许空成员子对象被优化为 0 字节 |

## 4. 底层原理

### 对象大小保证

C++ 标准要求任何对象的大小至少为 1 字节，以确保不同对象实例拥有不同的地址：

```cpp
struct Empty {};

static_assert(sizeof(Empty) >= 1);  // 保证通过
```

这是为了满足以下约束：
- 同类型的不同对象必须有不同的地址
- `&a != &b` 对于不同的对象 a 和 b 必须成立

### EBO 工作机制

当空类作为基类时，编译器可以将其优化掉：

```
无 EBO 的内存布局：
+--------+--------+
| Base   | int i  |
| (1字节)| (4字节)|
+--------+--------+

应用 EBO 后的内存布局：
+--------+
| int i  |
| (4字节)|
+--------+
Base 基类子对象被完全优化掉
```

### 地址约束与 EBO 禁止

当派生类的第一个非静态数据成员与基类有类型关联时，EBO 被禁止：

```
派生类对象内存布局（EBO 被禁止的情况）：

Derived2 : Base
+--------+--------+--------+--------+
| Base   | Base c | padding| int i  |
| 基类   | 成员   |        |        |
+--------+--------+--------+--------+

两个 Base 子对象必须有不同地址，所以不能优化
```

### 编译器实现差异

不同编译器在多重继承时的 EBO 实现有所差异：

| 编译器 | 多重继承 EBO 策略 |
|--------|------------------|
| GCC | 所有空基类都应用 EBO，地址与派生类对象首地址相同 |
| MSVC | 仅最后一个空基类应用 EBO，其他空基类各占 1 字节 |
| Clang | 与 GCC 行为类似 |

### 标准布局类型要求

C++11 起，标准布局类型 (StandardLayoutType) 要求：

1. 所有非静态数据成员声明在同一个类中
2. 没有与第一个非静态数据成员相同类型的基类

这些要求确保 EBO 的正确行为。

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 无状态分配器 | `std::vector<T, Allocator>` 中 Allocator 为空类时不增加容器大小 |
| 策略模式 | 空策略类作为基类时不增加派生类大小 |
| 类型标签 | 用于类型区分的空标签类 |
| CRTP 模式 | 奇异递归模板模式中的空基类 |

### 标准库应用

EBO 被广泛应用于标准库中：

| 标准库组件 | EBO 应用 |
|-----------|----------|
| `std::vector` | 无状态分配器不增加容器大小 |
| `std::function` | 小函数对象优化 |
| `std::shared_ptr` | 无状态删除器不增加控制块大小 |
| `std::unique_ptr` | 无状态删除器不增加指针大小 |

### boost::compressed_pair

`boost::compressed_pair` 是一个经典应用，它利用 EBO 存储可能为空的类型：

```cpp
// 如果 T1 或 T2 是空类，compressed_pair 利用 EBO 减少内存占用
template<typename T1, typename T2>
class compressed_pair {
    // 使用继承或其他技术实现 EBO
};
```

### 最佳实践

1. **优先使用继承而非成员**：对于空类，继承比成员变量更节省空间
2. **避免与第一个成员类型冲突**：确保基类类型与第一个非静态数据成员类型不同
3. **C++20 使用 [[no_unique_address]]**：对于空类成员使用此属性

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 第一个成员与基类同类型 | EBO 被禁止 | 调整成员顺序或类型 |
| 编译器差异 | 多重继承时不同编译器行为不同 | 避免依赖特定编译器行为 |
| 未考虑对齐 | 忽略内存对齐可能导致意外大小 | 使用 `alignas` 或调整布局 |

## 6. 代码示例

### 基础示例：EBO 工作原理

```cpp
#include <iostream>

// 空类
struct Base {};

// 派生类：应用 EBO
struct Derived1 : Base {
    int i;
};

int main() {
    // 空类对象大小至少 1 字节
    std::cout << "sizeof(Base) = " << sizeof(Base) << std::endl;  // >= 1

    // EBO 应用：Base 基类子对象占用 0 字节
    std::cout << "sizeof(Derived1) = " << sizeof(Derived1) << std::endl;  // == sizeof(int)

    // 验证
    static_assert(sizeof(Base) >= 1, "Empty class must be at least 1 byte");
    static_assert(sizeof(Derived1) == sizeof(int), "EBO should apply");

    return 0;
}
```

**输出示例**：
```
sizeof(Base) = 1
sizeof(Derived1) = 4
```

### 进阶示例：EBO 禁止条件

```cpp
#include <iostream>

struct Base {};  // 空类

// EBO 生效
struct Derived1 : Base {
    int i;
};

// EBO 被禁止：第一个成员是 Base 类型
struct Derived2 : Base {
    Base c;  // 占用 1 字节，后面有填充
    int i;
};

// EBO 被禁止：第一个成员派生自 Base
struct Derived3 : Base {
    Derived1 c;  // 派生自 Base
    int i;
};

int main() {
    std::cout << "sizeof(Base) = " << sizeof(Base) << std::endl;
    std::cout << "sizeof(Derived1) = " << sizeof(Derived1) << std::endl;
    std::cout << "sizeof(Derived2) = " << sizeof(Derived2) << std::endl;
    std::cout << "sizeof(Derived3) = " << sizeof(Derived3) << std::endl;

    // EBO 禁止验证
    static_assert(sizeof(Derived2) == 2 * sizeof(int),
                  "EBO disabled: Base and Base member need different addresses");
    static_assert(sizeof(Derived3) == 3 * sizeof(int),
                  "EBO disabled: Base and Derived1 base need different addresses");

    return 0;
}
```

**输出示例**：
```
sizeof(Base) = 1
sizeof(Derived1) = 4
sizeof(Derived2) = 8
sizeof(Derived3) = 12
```

### C++20 示例：[[no_unique_address]]

```cpp
#include <iostream>

struct Empty {};  // 空类

// C++20: 使用 [[no_unique_address]] 优化空成员
struct X {
    int i;
    [[no_unique_address]] Empty e;
};

// 未使用属性的对比
struct Y {
    int i;
    Empty e;  // 占用 1 字节
};

int main() {
    std::cout << "sizeof(Empty) = " << sizeof(Empty) << std::endl;
    std::cout << "sizeof(X) = " << sizeof(X) << std::endl;
    std::cout << "sizeof(Y) = " << sizeof(Y) << std::endl;

    // X 使用了 EBO 扩展
    static_assert(sizeof(Empty) >= 1, "Empty class at least 1 byte");
    static_assert(sizeof(X) == sizeof(int), "Empty member optimized out");
    // Y 未优化，至少需要额外的 1 字节加上填充

    return 0;
}
```

**输出示例**：
```
sizeof(Empty) = 1
sizeof(X) = 4
sizeof(Y) = 8
```

### 实际应用：无状态分配器

```cpp
#include <iostream>
#include <memory>
#include <vector>

// 无状态分配器
template<typename T>
class StatelessAllocator {
public:
    using value_type = T;

    StatelessAllocator() noexcept = default;

    template<typename U>
    StatelessAllocator(const StatelessAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) noexcept {
        ::operator delete(p);
    }

    // 相等比较：所有无状态分配器都相等
    bool operator==(const StatelessAllocator&) const noexcept { return true; }
};

int main() {
    // 验证分配器是空类
    static_assert(sizeof(StatelessAllocator<int>) == 1, "Should be empty");

    // vector 利用 EBO，不因分配器增加大小
    // 典型实现：vector 内部存储 (pointer, pointer, pointer, allocator)
    // 无状态分配器通过 EBO 不占用额外空间

    std::cout << "StatelessAllocator size: "
              << sizeof(StatelessAllocator<int>) << std::endl;

    return 0;
}
```

### 常见错误：导致 EBO 失效

```cpp
#include <iostream>

struct Empty {};

// 错误：第一个成员与基类类型相同，EBO 失效
template<typename T>
struct BadContainer : Empty {
    Empty e;      // 这里导致 EBO 失效
    T* data;
    std::size_t size;
};

// 修正：避免类型冲突
template<typename T>
struct GoodContainer : Empty {
    T* data;      // 先声明其他成员
    std::size_t size;
    // 不再需要 Empty 成员，直接继承即可
};

int main() {
    std::cout << "sizeof(BadContainer<int>) = "
              << sizeof(BadContainer<int>) << std::endl;
    std::cout << "sizeof(GoodContainer<int>) = "
              << sizeof(GoodContainer<int>) << std::endl;

    return 0;
}
```

**输出示例**：
```
sizeof(BadContainer<int>) = 24   // 包含额外的 1 字节 + 填充
sizeof(GoodContainer<int>) = 16  // EBO 生效，无额外开销
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| 基本原理 | 空基类子对象可被优化为 0 字节 |
| 目的 | 减少内存占用，提高内存效率 |
| 禁止条件 | 基类类型与第一个非静态数据成员类型冲突 |
| C++20 扩展 | `[[no_unique_address]]` 将 EBO 扩展到成员 |

### 技术对比

| 技术 | 版本 | 作用对象 | 效果 |
|------|------|----------|------|
| EBO | C++98 | 空基类子对象 | 0 字节 |
| `[[no_unique_address]]` | C++20 | 空成员子对象 | 0 字节 |

### 学习建议

1. **理解原理**：掌握对象地址唯一性和 EBO 的关系
2. **注意陷阱**：避免导致 EBO 失效的布局设计
3. **阅读源码**：分析 `std::vector`、`std::unique_ptr` 等标准库实现
4. **跨平台测试**：不同编译器的 EBO 行为可能有差异

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024):
  - 7.6.10 Equality operators [expr.eq]
  - 7.6.2.5 Sizeof [expr.sizeof]
  - 11 Classes [class]
  - 11.4 Class members [class.mem]
- C++20 标准 (ISO/IEC 14882:2020):
  - 7.6.10 Equality operators [expr.eq]
  - 7.6.2.4 Sizeof [expr.sizeof]
  - 11 Classes [class]
  - 11.4 Class members [class.mem]
- cppreference: https://en.cppreference.com/w/cpp/language/ebo