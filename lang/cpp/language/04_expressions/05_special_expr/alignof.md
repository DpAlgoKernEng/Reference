# alignof 运算符 (C++11 起)

## 1. 概述 (Overview)

`alignof` 是 C++11 标准引入的编译期运算符，用于查询类型的对齐要求 (alignment requirement)。对齐要求指的是该类型的对象在内存中存储时，其起始地址必须满足的字节边界限制。

### 主要用途

- 查询类型在内存中的对齐边界
- 在泛型编程中获取类型信息
- 与 `alignas` 说明符配合使用，自定义对齐要求
- 底层内存管理和性能优化

### 技术定位

`alignof` 是类型特征 (type trait) 的一部分，属于编译期求值的常量表达式，返回类型为 `std::size_t`。它与 `sizeof` 运算符类似，都是在编译期计算，不会产生运行时开销。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++11 之前，C++ 没有标准化的方法来查询类型的对齐要求。开发者需要依赖编译器扩展或平台特定的宏来获取对齐信息，这导致代码可移植性差。

### 设计动机

- **统一接口**：为不同编译器提供标准化的对齐查询方式
- **泛型编程支持**：在模板元编程中获取类型的对齐信息
- **底层编程需求**：满足系统编程、硬件交互等场景对精确控制内存布局的要求
- **与 C 兼容**：C11 标准引入了类似的 `_Alignof` 运算符

### 版本变更

| 标准 | 文档章节 | 说明 |
|------|----------|------|
| C++11 | 5.3.6 | 首次引入 `alignof` 运算符 |
| C++14 | 5.3.6 | 无重大变更 |
| C++17 | 8.3.6 | 章节编号调整 |
| C++20 | 7.6.2.5 | 章节编号调整 |
| C++23 | 7.6.2.6 | 章节编号调整 |

### 缺陷报告

**CWG 1305 (C++11)**：早期标准中，type-id 不能表示元素类型完整但边界未知的数组引用类型，现已修正为允许这种用法。

## 3. 语法与参数 (Syntax and Parameters)

### 核心语法

```cpp
alignof( type-id )
```

### 参数说明

- **type-id**：类型标识符，可以是以下之一：
  - 完整对象类型 (complete object type)
  - 元素类型完整的数组类型
  - 指向上述类型的引用类型

### 返回值

- **类型**：`std::size_t`
- **含义**：返回类型对齐要求，以字节为单位
- **特性**：编译期常量表达式

### 类型推导规则

| 类型类别 | 返回值 |
|----------|--------|
| 基本类型 | 该类型本身的对齐要求 |
| 引用类型 | 被引用类型的对齐要求 |
| 数组类型 | 元素类型的对齐要求 |
| 类类型 | 根据成员变量的对齐要求计算 |
| 空类 | 通常为 1 |

## 4. 底层原理 (Underlying Principles)

### 实现机制

`alignof` 运算符在编译期完成求值，其实现涉及以下机制：

1. **编译器内部表示**：编译器在内部为每个类型维护对齐信息
2. **常量折叠**：结果在编译期直接替换为常量值
3. **类型推导**：对于引用和数组类型，自动推导到实际对齐要求

### 对齐计算规则

对齐值的计算遵循以下原则：

- 基本类型的对齐要求由目标平台 ABI (Application Binary Interface) 决定
- 结构体的对齐要求是其成员中最大的对齐要求
- 数组的对齐要求等同于其元素类型的对齐要求
- 编译器可能添加填充字节以满足对齐要求

### 性能特征

- **编译期求值**：零运行时开销
- **编译时间影响**：可忽略不计
- **内存占用**：不占用额外内存

### 平台差异

不同平台可能有不同的对齐要求：

| 类型 | 32位系统典型值 | 64位系统典型值 |
|------|----------------|----------------|
| char | 1 | 1 |
| short | 2 | 2 |
| int | 4 | 4 |
| long | 4 | 8 |
| double | 8 | 8 |
| 指针 | 4 | 8 |

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 底层内存管理

在自定义内存分配器中，确保分配的内存满足类型的对齐要求：

```cpp
void* allocate_aligned(size_t size, size_t alignment);
```

#### 2. 泛型编程

在模板中获取类型信息：

```cpp
template<typename T>
void process() {
    constexpr size_t alignment = alignof(T);
    // 根据对齐要求优化处理
}
```

#### 3. 与 alignas 配合

动态指定对齐要求：

```cpp
struct alignas(alignof(long double)) Foo {
    // 结构体成员
};
```

#### 4. 静态断言验证

确保类型满足特定对齐要求：

```cpp
static_assert(alignof(MyType) == 16, "MyType must be 16-byte aligned");
```

### 最佳实践

1. **优先使用 alignof 而非硬编码值**
   ```cpp
   // 推荐
   constexpr size_t alignment = alignof(double);

   // 不推荐
   constexpr size_t alignment = 8; // 平台相关
   ```

2. **在编译期验证假设**
   ```cpp
   static_assert(alignof(int) >= 4, "int alignment assumption failed");
   ```

3. **结合 std::alignment_of 使用**
   ```cpp
   #include <type_traits>
   static_assert(alignof(int) == std::alignment_of<int>::value);
   ```

### 注意事项

1. **类型完整性**：`alignof` 只能用于完整类型，不能用于不完整类型（如前置声明的类）
2. **引用类型的处理**：`alignof(T&)` 等同于 `alignof(T)`
3. **数组类型的处理**：`alignof(T[N])` 等同于 `alignof(T)`
4. **空类的对齐**：空类的大小至少为 1，对齐要求通常也为 1

### 常见陷阱

#### 陷阱 1：误认为对齐等于大小

```cpp
// 错误：对齐和大小可能不同
struct Example {
    char c;  // 大小 1，但结构体对齐可能是 4 或 8
};

// 正确理解
static_assert(sizeof(Example) != alignof(Example)); // 可能成立
```

#### 陷阱 2：忽略平台差异

```cpp
// 危险：假设 long 的对齐是 8
// 在某些 32 位系统上可能失败
static_assert(alignof(long) == 8); // 32位系统上失败
```

#### 陷阱 3：与 alignas 混淆

```cpp
// alignof 是查询，alignas 是指定
alignof(int);      // 查询 int 的对齐要求
alignas(16) int x; // 指定 x 的对齐要求为 16
```

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

int main() {
    // 基本类型的对齐要求
    std::cout << "alignof(char) = " << alignof(char) << '\n';
    std::cout << "alignof(short) = " << alignof(short) << '\n';
    std::cout << "alignof(int) = " << alignof(int) << '\n';
    std::cout << "alignof(long) = " << alignof(long) << '\n';
    std::cout << "alignof(double) = " << alignof(double) << '\n';

    // 指针类型的对齐要求
    std::cout << "alignof(int*) = " << alignof(int*) << '\n';
    std::cout << "alignof(char*) = " << alignof(char*) << '\n';

    return 0;
}
```

**可能的输出：**
```
alignof(char) = 1
alignof(short) = 2
alignof(int) = 4
alignof(long) = 8
alignof(double) = 8
alignof(int*) = 8
alignof(char*) = 8
```

### 结构体对齐

```cpp
#include <iostream>

struct Foo {
    int   i;    // 4 bytes
    float f;    // 4 bytes
    char  c;    // 1 byte + 3 bytes padding
};

struct alignas(alignof(long double)) Foo2 {
    // 对齐要求与 long double 相同
    int data;
};

struct Empty {};

struct alignas(64) Empty64 {};

int main() {
    std::cout << "alignof(Foo) = " << alignof(Foo) << '\n';
    std::cout << "sizeof(Foo) = " << sizeof(Foo) << '\n';

    std::cout << "alignof(Foo2) = " << alignof(Foo2) << '\n';
    std::cout << "alignof(Empty) = " << alignof(Empty) << '\n';
    std::cout << "alignof(Empty64) = " << alignof(Empty64) << '\n';

    return 0;
}
```

**可能的输出：**
```
alignof(Foo) = 4
sizeof(Foo) = 12
alignof(Foo2) = 16
alignof(Empty) = 1
alignof(Empty64) = 64
```

### 引用和数组类型

```cpp
#include <iostream>
#include <type_traits>

int main() {
    // 引用类型的对齐
    static_assert(alignof(int&) == alignof(int));
    static_assert(alignof(double&) == alignof(double));

    // 数组类型的对齐
    int arr[10];
    static_assert(alignof(decltype(arr)) == alignof(int));

    std::cout << "alignof(int&) = " << alignof(int&) << '\n';
    std::cout << "alignof(int[10]) = " << alignof(int[10]) << '\n';

    // 与 std::alignment_of 对比
    std::cout << "std::alignment_of<int>::value = "
              << std::alignment_of<int>::value << '\n';

    return 0;
}
```

### 高级用法：自定义对齐分配器

```cpp
#include <iostream>
#include <memory>
#include <new>

template<typename T, std::size_t Alignment>
class AlignedAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    template<typename U>
    struct rebind {
        using other = AlignedAllocator<U, Alignment>;
    };

    pointer allocate(size_type n) {
        std::size_t size = n * sizeof(T);
        std::size_t align = alignof(T) > Alignment ? alignof(T) : Alignment;

        void* ptr = ::operator new(size, std::align_val_t{align});
        return static_cast<pointer>(ptr);
    }

    void deallocate(pointer p, size_type n) {
        std::size_t align = alignof(T) > Alignment ? alignof(T) : Alignment;
        ::operator delete(p, std::align_val_t{align});
    }
};

int main() {
    std::cout << "Custom alignment allocator example\n";
    std::cout << "Default int alignment: " << alignof(int) << '\n';
    std::cout << "Custom alignment: 64\n";

    AlignedAllocator<int, 64> alloc;
    int* p = alloc.allocate(10);

    std::cout << "Allocated at address: " << p << '\n';
    std::cout << "Address is 64-byte aligned: "
              << (reinterpret_cast<std::uintptr_t>(p) % 64 == 0) << '\n';

    alloc.deallocate(p, 10);

    return 0;
}
```

### 编译期验证

```cpp
#include <iostream>

// 确保类型的对齐满足特定要求
template<typename T>
struct AlignedBuffer {
    alignas(T) unsigned char buffer[sizeof(T)];

    static_assert(alignof(AlignedBuffer) >= alignof(T),
                  "Buffer alignment insufficient");
};

// 平台特性检查
struct SIMDVector {
    double data[4];
};

int main() {
    // 编译期断言
    static_assert(alignof(int) >= 4,
                  "int should have at least 4-byte alignment");

    static_assert(alignof(double) >= alignof(float),
                  "double alignment should be at least as strict as float");

    // 检查自定义结构
    std::cout << "SIMDVector alignment: " << alignof(SIMDVector) << '\n';

    // 使用对齐缓冲区
    AlignedBuffer<double> buf;
    std::cout << "AlignedBuffer alignment: " << alignof(buf) << '\n';

    return 0;
}
```

### 常见错误及修正

#### 错误 1：用于不完整类型

```cpp
// 错误示例
struct Incomplete;  // 前置声明，不完整类型

// 编译错误：不能对不完整类型使用 alignof
// auto align = alignof(Incomplete);

// 修正：提供完整定义
struct Incomplete {
    int x;
};

auto align = alignof(Incomplete);  // OK
```

#### 错误 2：误用于变量

```cpp
#include <iostream>

int main() {
    int x = 10;

    // 错误：alignof 用于类型，不是变量
    // auto align = alignof(x);  // 编译错误

    // 修正：使用 decltype 或直接使用类型
    auto align1 = alignof(decltype(x));  // OK
    auto align2 = alignof(int);          // OK

    std::cout << "Alignment: " << align1 << '\n';

    return 0;
}
```

#### 错误 3：混淆 alignof 和 alignas

```cpp
// 错误理解
// alignof：查询类型的对齐要求（只读）
// alignas：指定变量或类型的对齐要求（可写）

// 正确用法
alignas(16) int aligned_var;  // 设置对齐为 16
auto align = alignof(int);     // 查询 int 的对齐要求

// 不能用 alignof 来设置对齐
// alignof(int) = 16;  // 错误：不能修改
```

## 7. 总结 (Summary)

### 核心要点

1. **编译期求值**：`alignof` 在编译期计算，无运行时开销
2. **返回类型**：返回 `std::size_t` 类型的常量表达式
3. **适用类型**：完整对象类型、完整元素类型的数组、引用类型
4. **推导规则**：引用类型返回被引用类型的对齐，数组类型返回元素类型的对齐
5. **标准支持**：C++11 起引入，各版本标准均有支持

### 技术对比

| 特性 | alignof | alignas | sizeof | std::alignment_of |
|------|---------|---------|--------|-------------------|
| 引入版本 | C++11 | C++11 | C++98 | C++11 |
| 操作对象 | 类型 | 类型/变量 | 类型/变量 | 类型 |
| 功能 | 查询对齐 | 指定对齐 | 查询大小 | 查询对齐 |
| 结果类型 | std::size_t | - | std::size_t | std::size_t |
| 求值时机 | 编译期 | 编译期 | 编译期 | 编译期 |

### 与 C 语言的对比

- C11 标准引入了 `_Alignof` 运算符，功能相同
- C++ 中的 `alignof` 更符合 C++ 语法风格
- C++ 提供了更丰富的类型特征支持（如 `std::alignment_of`）

### 学习建议

1. **理解内存布局**：深入理解内存对齐的概念和意义
2. **实践验证**：在不同平台上测试对齐值的差异
3. **结合使用**：掌握 `alignof` 与 `alignas` 的配合使用
4. **阅读标准**：参考 C++ 标准文档了解精确语义
5. **性能优化**：学习对齐对性能的影响，优化内存访问

### 扩展阅读

- C++ 标准文档关于对齐的章节
- ABI (Application Binary Interface) 规范
- 内存对齐对 CPU 缓存的影响
- SIMD 指令集对数据对齐的要求
- C++17 结构化绑定与对齐的关系

---

**参考文档：**
- C++23 标准 (ISO/IEC 14882:2024) - 7.6.2.6 Alignof
- cppreference: https://en.cppreference.com/w/cpp/language/alignof