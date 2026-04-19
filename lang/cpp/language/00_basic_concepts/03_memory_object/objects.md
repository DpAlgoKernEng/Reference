# 对象 (Object)

## 1. 概述 (Overview)

C++ 程序的核心任务是创建、销毁、引用、访问和操作对象。

### 对象的定义

**对象 (Object)** 在 C++ 中具有以下属性：

| 属性 | 说明 | 获取方式 |
|-----|------|---------|
| 大小 (size) | 对象占用的字节数 | `sizeof` 运算符 |
| 对齐要求 (alignment requirement) | 地址对齐边界 | `alignof` 运算符 |
| 存储期 (storage duration) | 对象存在的时间范围 | 自动、静态、动态、线程局部 |
| 生存期 (lifetime) | 对象可访问的时间段 | 由存储期或临时对象决定 |
| 类型 (type) | 对象的数据类型 | 声明时指定 |
| 值 (value) | 对象存储的数据 | 可能是不确定值 |
| 名称 (name) | 可选，对象的标识符 | 声明中指定 |

### 非对象实体

以下实体**不是**对象：
- 值 (value)
- 引用 (reference)
- 函数 (function)
- 枚举器 (enumerator)
- 类型 (type)
- 非静态类成员 (non-static class member)
- 模板 (template)
- 类或函数模板特化
- 命名空间 (namespace)
- 参数包 (parameter pack)
- `this` 指针

### 变量的定义

**变量 (variable)** 是由声明引入的对象或引用，但不是非静态数据成员。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 对象模型继承自 C 语言，但增加了面向对象特性和类型安全机制。C++ 的对象概念比 OOP 中的对象更广泛。

### 版本演进

| C++ 标准版本 | 发布年份 | 对象相关改进 |
|-------------|---------|-------------|
| C++98 | 1998 | 基础对象模型 |
| C++11 | 2011 | 对齐支持、`std::max_align_t`、隐式对象创建初步支持 |
| C++17 | 2017 | `std::byte`、隐式对象创建扩展 |
| C++20 | 2020 | `std::bit_cast`、`[[no_unique_address]]` |
| C++23 | 2023 | `std::start_lifetime_as` 系列函数 |

### 重要缺陷报告

| 缺陷报告 | 问题 | 解决方案 |
|---------|------|---------|
| CWG 633 | 变量只能是对象 | 变量也可以是引用 |
| CWG 734 | 相同值的变量地址未明确 | 生存期重叠则地址必然不同 |
| CWG 1189 | 同类型基类子对象可能同地址 | 必然有不同地址 |
| CWG 1861 | 宽字符类型超大位域无填充位 | 允许填充位 |
| CWG 2719 | 未对齐存储创建对象行为不明确 | 明确为未定义行为 |
| P0593R6 | 对象模型不支持标准库惯用法 | 添加隐式对象创建 |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 获取对象属性

```cpp
// 获取对象大小
sizeof(object)     // 对象大小
sizeof(type)       // 类型大小

// 获取对齐要求
alignof(type)      // 类型对齐要求
std::alignment_of<T>::value  // 类型特性
```

### 3.2 对齐说明符

```cpp
alignas(type)      // 按指定类型的对齐要求对齐
alignas(n)         // 按 n 字节边界对齐（n 必须是 2 的幂）

// 示例
struct alignas(16) SimdVector {
    float data[4];
};
```

### 3.3 相关头文件与工具

```cpp
#include <new>           // operator new, std::start_lifetime_as (C++23)
#include <memory>        // std::align, std::aligned_storage
#include <type_traits>   // std::alignment_of
#include <cstddef>       // std::max_align_t, std::byte (C++17)
#include <cstring>       // std::memcpy, std::memmove
#include <bit>           // std::bit_cast (C++20)
```

## 4. 底层原理 (Underlying Principles)

### 4.1 对象创建

#### 显式创建

对象可通过以下方式显式创建：

| 创建方式 | 说明 |
|---------|------|
| 定义 (definitions) | 变量定义 |
| `new` 表达式 | 动态分配 |
| `throw` 表达式 | 异常对象 |
| 联合体活跃成员切换 | 改变活跃成员 |
| 需要临时对象的表达式 | 临时对象 |

#### 隐式创建（C++17 起）

**隐式生存期类型 (Implicit-lifetime types)** 的对象可通过以下操作隐式创建：

| 操作 | 说明 |
|-----|------|
| `unsigned char`/`std::byte` 数组生存期开始 | 在数组中创建对象 |
| 分配函数调用 | `operator new`、`std::malloc`、`std::calloc`、`std::realloc`、`std::aligned_alloc` |
| 对象表示复制函数 | `std::memcpy`、`std::memmove`、`std::bit_cast` (C++20) |
| 特定函数 | `std::start_lifetime_as`、`std::start_lifetime_as_array` (C++23) |

```cpp
#include <cstdlib>

struct X { int a, b; };

X* MakeX() {
    // std::malloc 隐式创建 X 类型对象
    X* p = static_cast<X*>(std::malloc(sizeof(X)));
    p->a = 1;
    p->b = 2;
    return p;
}
```

### 4.2 对象表示与值表示

| 概念 | 定义 |
|-----|------|
| **对象表示** | 对象占用的字节序列（`unsigned char[N]`） |
| **值表示** | 对象表示中参与表示值的位集合 |
| **填充位** | 对象表示中不参与值表示的位 |

```cpp
struct S {
    char c;   // 1 字节值
              // 3 字节填充（假设 alignof(float) == 4）
    float f;  // 4 字节值
};  // 总大小: 8 字节
```

#### 平凡可复制类型

对于**平凡可复制类型 (TriviallyCopyable types)**：
- 复制字节即可复制值
- 不同对象表示可能表示相同值（如 NaN、填充位）

#### 字符类型特殊性

`char`、`signed char`、`unsigned char` 类型：
- 每个位都参与值表示
- 没有填充位、陷阱位
- 每种位模式表示唯一值

### 4.3 子对象 (Subobjects)

对象可包含子对象：

| 子对象类型 | 说明 |
|-----------|------|
| 成员对象 | 非静态数据成员 |
| 基类子对象 | 继承的基类部分 |
| 数组元素 | 数组的各个元素 |

#### 完整对象与最派生对象

| 概念 | 定义 |
|-----|------|
| **完整对象** | 不是其他对象子对象的对象 |
| **最派生对象** | 完整对象、成员对象、数组元素（区别于基类子对象） |
| **潜在构造子对象** | 非静态数据成员、非虚直接基类、虚基类（非抽象类） |

### 4.4 对象大小

#### 零大小对象条件

对象 `obj` 可能为零大小，当且仅当：
1. `obj` 是潜在重叠子对象
2. `obj` 是没有虚函数和虚基类的类类型
3. `obj` 没有非零大小的子对象

```cpp
struct Empty {};

struct Derived : Empty {
    int value;
};  // Empty 基类子对象大小为 0（空基类优化）

struct WithNoUnique {
    [[no_unique_address]] Empty e;  // C++20: 可能为零大小
    int value;
};
```

#### 存储连续性

非平凡可复制或非标准布局类型的对象，其存储可能不连续。

### 4.5 对象地址

| 规则 | 说明 |
|-----|------|
| 地址定义 | 非位域、非零大小对象的地址是其占用首字节地址 |
| 嵌套关系 | 子对象嵌套于父对象；提供存储的对象嵌套被存储对象 |

#### 地址唯一性

两个生存期重叠的非位域对象：

**可能相同地址**：
- 一个嵌套于另一个
- 零大小子对象（类型不相似）
- 潜在非唯一对象（字符串字面量、初始化列表后备数组）

**必然不同地址**：
- 其他所有情况

```cpp
static const char test1 = 'x';
static const char test2 = 'x';
const bool b = &test1 != &test2;  // 必然为 true

// 字符串字面量可能共享存储
static const char (&r)[] = "x";
static const char* s = "x";
const bool b2 = r != s;  // 结果未指定
```

### 4.6 多态对象

**多态对象**：声明或继承至少一个虚函数的类类型对象。

| 特性 | 多态对象 | 非多态对象 |
|-----|---------|-----------|
| 类型信息存储 | 运行时存储额外信息（通常为虚表指针） | 无额外存储 |
| 类型确定 | 运行时确定 | 编译时确定 |
| `dynamic_cast` | 支持 | 不支持 |
| `typeid` | 返回实际类型 | 返回静态类型 |

```cpp
struct Base1 {
    virtual ~Base1() {}  // 多态类型
};

struct Derived1 : Base1 {};

struct Base2 {};  // 非多态类型

struct Derived2 : Base2 {};

// sizeof(Base1) 可能是 8（含虚表指针）
// sizeof(Base2) 是 1（空类）
```

### 4.7 严格别名规则

通过不同于创建类型的表达式访问对象通常是未定义行为，但有例外情况。

详见 `reinterpret_cast` 文档中的例外列表。

### 4.8 对齐

**对齐要求**：类型的一个属性，表示该类型对象可分配地址之间的字节数间隔。对齐值总是 2 的幂。

| 分类 | 说明 |
|-----|------|
| **基本对齐** | ≤ `std::max_align_t` 的对齐 |
| **扩展对齐** | > `std::max_align_t` 的对齐 |

#### 填充位

为满足成员对齐要求，类可能在成员后插入填充字节。

```cpp
struct S {
    char a;  // 大小: 1, 对齐: 1
    char b;  // 大小: 1, 对齐: 1
};  // 总大小: 2, 对齐: 1

struct X {
    int n;   // 大小: 4, 对齐: 4
    char c;  // 大小: 1, 对齐: 1
    // 3 字节填充
};  // 总大小: 8, 对齐: 4
```

## 5. 使用场景 (Use Cases)

### 5.1 对象创建方式选择

| 场景 | 推荐方式 |
|-----|---------|
| 局部变量 | 自动存储期（栈分配） |
| 全局/静态数据 | 静态存储期 |
| 动态大小数据 | `new`/`make_unique`/`make_shared` |
| 原始内存操作 | `std::malloc` + 隐式对象创建 |
| 类型双关 | `std::bit_cast` (C++20) 或 `std::memcpy` |

### 5.2 空基类优化 (EBO)

```cpp
// 利用空基类优化减少对象大小
struct Empty {};

struct WithoutEBO {
    Empty e;  // 通常占用 1 字节
    int value;
};  // 大小: 8（含填充）

struct WithEBO : Empty {
    int value;
};  // 大小: 4（Empty 不占空间）
```

### 5.3 注意事项

1. **未对齐访问**：在不满足对齐要求的存储中创建对象是未定义行为
2. **严格别名**：避免通过不相关类型指针访问对象
3. **填充位**：结构体比较应逐成员进行，不能用 `memcmp`
4. **隐式对象创建**：使用 `std::malloc` 等分配时需注意对象隐式创建规则

## 6. 代码示例 (Examples)

### 6.1 对象创建与销毁

```cpp
#include <iostream>
#include <memory>

class Widget {
public:
    Widget(int v) : value(v) {
        std::cout << "Widget(" << value << ") 构造\n";
    }
    ~Widget() {
        std::cout << "~Widget(" << value << ") 析构\n";
    }
private:
    int value;
};

int main() {
    // 自动存储期
    Widget w1(1);

    // 动态存储期
    Widget* w2 = new Widget(2);
    delete w2;

    // 智能指针（推荐）
    auto w3 = std::make_unique<Widget>(3);

    return 0;
}
```

### 6.2 对象表示与值表示

```cpp
#include <iostream>
#include <cstring>

struct S {
    char c;    // 1 字节
               // 可能的填充
    float f;   // 4 字节
};

int main() {
    std::cout << "sizeof(S) = " << sizeof(S) << '\n';
    std::cout << "alignof(S) = " << alignof(S) << '\n';

    S s1 = {'a', 3.14f};
    S s2 = s1;

    // 修改填充位
    auto* bytes = reinterpret_cast<unsigned char*>(&s1);
    bytes[1] = 0xFF;  // 修改可能的填充

    // 值仍相等（填充位不影响值）
    std::cout << "s1.c == s2.c: " << (s1.c == s2.c) << '\n';
    std::cout << "s1.f == s2.f: " << (s1.f == s2.f) << '\n';

    return 0;
}
```

### 6.3 子对象与地址

```cpp
#include <iostream>

struct Empty {};

struct Base {
    int x;
};

struct Derived : Base, Empty {
    int y;
};

int main() {
    Derived d;

    // 基类子对象地址
    Base* base = &d;
    Empty* empty = &d;

    std::cout << "Derived 地址: " << &d << '\n';
    std::cout << "Base 子对象: " << base << '\n';
    std::cout << "Empty 子对象: " << empty << '\n';

    // Base 和 Derived 地址相同（Base 是首个基类）
    std::cout << "Base == Derived: " << (base == static_cast<void*>(&d)) << '\n';

    // Empty 可能有不同地址（空基类优化可能导致相同）
    std::cout << "Empty == Derived: " << (empty == static_cast<void*>(&d)) << '\n';

    return 0;
}
```

### 6.4 多态对象

```cpp
#include <iostream>
#include <typeinfo>

struct Base {
    virtual ~Base() {}
    virtual void foo() { std::cout << "Base::foo\n"; }
};

struct Derived : Base {
    void foo() override { std::cout << "Derived::foo\n"; }
};

int main() {
    Derived d;
    Base& ref = d;

    // 静态类型 vs 动态类型
    std::cout << "静态类型: " << typeid(decltype(ref)).name() << '\n';
    std::cout << "动态类型: " << typeid(ref).name() << '\n';

    // 虚函数调用
    ref.foo();  // 调用 Derived::foo

    // RTTI
    Derived* pd = dynamic_cast<Derived*>(&ref);
    if (pd) {
        std::cout << "dynamic_cast 成功\n";
    }

    // 大小差异
    std::cout << "sizeof(Base) = " << sizeof(Base) << '\n';

    return 0;
}
```

### 6.5 对齐与填充

```cpp
#include <iostream>
#include <cstddef>

struct Unaligned {
    char a;
    int b;
    char c;
};

struct Aligned {
    alignas(16) int value;
};

int main() {
    std::cout << "=== Unaligned ===" << '\n';
    std::cout << "sizeof: " << sizeof(Unaligned) << '\n';
    std::cout << "alignof: " << alignof(Unaligned) << '\n';
    std::cout << "offset a: " << offsetof(Unaligned, a) << '\n';
    std::cout << "offset b: " << offsetof(Unaligned, b) << '\n';
    std::cout << "offset c: " << offsetof(Unaligned, c) << '\n';

    std::cout << "\n=== Aligned (16-byte) ===" << '\n';
    std::cout << "sizeof: " << sizeof(Aligned) << '\n';
    std::cout << "alignof: " << alignof(Aligned) << '\n';

    // 检查对齐
    Aligned arr[2];
    std::cout << "arr[0] 对齐: " << (reinterpret_cast<uintptr_t>(&arr[0]) % 16) << '\n';
    std::cout << "arr[1] 对齐: " << (reinterpret_cast<uintptr_t>(&arr[1]) % 16) << '\n';

    return 0;
}
```

### 6.6 隐式对象创建（C++17）

```cpp
#include <iostream>
#include <cstdlib>
#include <cstring>

struct Point {
    double x, y;
};

int main() {
    // 使用 std::malloc 隐式创建对象
    void* raw = std::malloc(sizeof(Point));
    Point* p = static_cast<Point*>(raw);
    p->x = 1.0;
    p->y = 2.0;
    std::cout << "Point(" << p->x << ", " << p->y << ")\n";
    std::free(raw);

    // 使用 std::memcpy 创建对象
    Point original{3.0, 4.0};
    unsigned char buffer[sizeof(Point)];
    std::memcpy(buffer, &original, sizeof(Point));
    Point* copy = reinterpret_cast<Point*>(buffer);
    std::cout << "Copy(" << copy->x << ", " << copy->y << ")\n";

    return 0;
}
```

### 6.7 常见错误及修正

#### 错误示例 1：违反严格别名规则

```cpp
// 错误：通过不相关类型指针访问对象
#include <iostream>

int main() {
    int i = 0x12345678;

    // 未定义行为！
    float* fp = reinterpret_cast<float*>(&i);
    std::cout << *fp << '\n';  // UB!

    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <cstring>
#include <bit>

int main() {
    int i = 0x12345678;

    // 方法1: std::bit_cast (C++20)
    // float f = std::bit_cast<float>(i);

    // 方法2: std::memcpy
    float f;
    std::memcpy(&f, &i, sizeof(float));
    std::cout << f << '\n';

    // 方法3: 通过 char* 访问（合法）
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&i);
    std::cout << "bytes: ";
    for (size_t j = 0; j < sizeof(int); ++j) {
        std::cout << std::hex << static_cast<int>(bytes[j]) << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

#### 错误示例 2：未对齐访问

```cpp
// 错误：在不满足对齐要求的地址创建对象
#include <iostream>
#include <cstdlib>

int main() {
    char* buffer = new char[sizeof(double) + 1];

    // 可能在未对齐地址创建 double
    double* pd = reinterpret_cast<double*>(buffer + 1);
    *pd = 3.14;  // 可能崩溃或性能下降！

    delete[] buffer;
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <memory>
#include <cstddef>

int main() {
    // 方法1: 使用 aligned_alloc
    void* ptr = std::aligned_alloc(alignof(double), sizeof(double));
    double* pd1 = static_cast<double*>(ptr);
    *pd1 = 3.14;
    std::free(ptr);

    // 方法2: 使用 alignas
    struct AlignedBuffer {
        alignas(double) char data[sizeof(double)];
    };
    AlignedBuffer buffer;
    double* pd2 = reinterpret_cast<double*>(&buffer);
    *pd2 = 3.14;

    std::cout << *pd2 << '\n';

    return 0;
}
```

#### 错误示例 3：忽略填充位影响

```cpp
// 错误：使用 memcmp 比较结构体
#include <iostream>
#include <cstring>

struct Person {
    char name[8];
    int age;
};

int main() {
    Person p1 = {"Alice", 25};
    Person p2 = {"Alice", 25};

    // 危险：填充位可能不同
    if (std::memcmp(&p1, &p2, sizeof(Person)) == 0) {
        std::cout << "相等（可能错误）\n";
    }

    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>
#include <cstring>

struct Person {
    char name[8];
    int age;

    bool operator==(const Person& other) const {
        return std::strcmp(name, other.name) == 0 && age == other.age;
    }
};

int main() {
    Person p1 = {"Alice", 25};
    Person p2 = {"Alice", 25};

    if (p1 == p2) {
        std::cout << "相等\n";
    }

    return 0;
}
```

## 7. 总结 (Summary)

### 7.1 核心要点

1. **对象定义**：具有大小、对齐、存储期、生存期、类型、值的存储区域

2. **对象创建**：
   - 显式创建：定义、`new`、`throw`、联合体切换、临时对象
   - 隐式创建：分配函数、复制函数、特定函数（C++17 起）

3. **对象表示**：
   - 对象表示 = 全部字节
   - 值表示 = 参与值表示的位
   - 填充位 = 不参与值表示的位

4. **子对象层次**：
   - 成员对象、基类子对象、数组元素
   - 完整对象 vs 最派生对象

5. **多态对象**：
   - 含虚函数的类类型
   - 运行时类型信息
   - 额外存储开销

### 7.2 C++ 对象 vs OOP 对象

| 特性 | C++ 对象 | OOP 对象 |
|-----|---------|---------|
| 类型范围 | 任意对象类型 | 必须是类类型 |
| 实例概念 | 无 | 有 `instanceof` 机制 |
| 接口概念 | 无 | 有接口机制 |
| 多态 | 需显式启用（虚函数） | 默认启用 |

### 7.3 类型双关方法对比

| 方法 | C++ 版本 | 合法性 | 性能 |
|-----|---------|-------|-----|
| `reinterpret_cast` | C++98 | 通常 UB | - |
| `std::memcpy` | C++98 | 合法 | 优（编译器优化） |
| 联合体类型双关 | C++98 | 有条件支持 | 优 |
| `std::bit_cast` | C++20 | 合法 | 优 |

### 7.4 学习建议

1. **理解对象模型**：
   - 对象表示与值表示的区别
   - 子对象关系与地址规则
   - 多态对象的实现机制

2. **遵循类型安全**：
   - 避免违反严格别名规则
   - 使用 `std::bit_cast` 或 `std::memcpy` 进行类型转换
   - 通过字符类型访问原始内存

3. **注意对齐问题**：
   - 理解填充位的来源
   - 使用 `alignas` 指定对齐
   - 分配内存时考虑对齐要求

4. **正确创建对象**：
   - 理解显式创建 vs 隐式创建
   - 使用 `std::start_lifetime_as` (C++23) 显式控制生存期
   - 避免在未对齐存储中创建对象

---

**参考资源**：
- ISO/IEC 14882:2024 (C++23 标准)
- `<new>` - 对象创建与生存期管理
- `<memory>` - 内存管理工具
- `<bit>` - `std::bit_cast` (C++20)
- `<type_traits>` - 类型特性查询