# sizeof 运算符

## 1. 概述

`sizeof` 是 C++ 中用于查询类型或对象大小（以字节为单位）的编译期运算符。它返回对象表示（object representation）所需的字节数，结果类型为 `std::size_t`。

sizeof 是一个**编译期操作**，其结果在编译时就已确定，是常量表达式（constant expression）。它是 C++ 中获取类型大小的主要方式，广泛用于内存管理、数组操作和类型计算。

## 2. 来源与演变

### 首次引入

`sizeof` 运算符在 **C 语言**时期就已存在，并随 C++ 一起继承而来。它是 C/C++ 语言的基础运算符之一。

### 历史背景

在早期编程中，由于不同平台的数据类型大小可能不同，程序员需要一种可移植的方式来确定类型大小：

1. **平台差异问题**：不同架构（如 16 位、32 位、64 位系统）上，`int`、指针等类型的大小各不相同
2. **内存计算需求**：动态内存分配、数组元素数量计算等场景需要准确知道类型大小
3. **可移植性要求**：代码需要在不同平台间移植，硬编码大小值不可行

sizeof 运算符的出现解决了这些问题，提供了编译期自动计算类型大小的能力。

### C++11 变化

- **位域限制放宽**：C++11 之前，sizeof 不能用于位域左值（lvalue）；C++11 起，不能用于位域泛左值（glvalue）
- **不求值表达式**：明确 sizeof 的操作数是未求值操作数（unevaluated operand），不会实际执行表达式

### C++17 变化

- **临时量具体化**：对于纯右值（prvalue）参数，形式上会进行临时量具体化（temporary materialization），但要求类型必须可析构，否则程序非良构

### C++20 变化

- **新增 char8_t**：`sizeof(char8_t)` 为 1，与 char 系列类型一致

## 3. 语法与参数

### 基本语法

```cpp
sizeof( type )      // (1) 类型形式
sizeof expression   // (2) 表达式形式
```

### 两种形式对比

| 形式 | 语法 | 说明 | 示例 |
|------|------|------|------|
| 类型形式 | `sizeof( type )` | 直接查询类型的大小 | `sizeof(int)` |
| 表达式形式 | `sizeof expression` | 查询表达式类型的对象表示大小 | `sizeof 42` |

### 参数说明

- **type**：类型标识符（type-id），可以是任何完整类型（complete type）
- **expression**：表达式，其优先级不低于 sizeof（例如 `sizeof a + b` 解析为 `(sizeof a) + b` 而非 `sizeof (a + b)`）

### 返回值

返回 `std::size_t` 类型的常量表达式，表示对象表示的字节数。

### 约束条件

sizeof 运算符有以下使用限制：

| 不能使用的类型/对象 | 原因 |
|-------------------|------|
| 函数类型 | 函数不是对象，没有大小 |
| 不完整类型（incomplete type） | 类型信息不完整，无法确定大小 |
| 位域（bit-field） | 位域不是完整字节，无法取地址 |

### 特殊规则

| 规则 | 说明 |
|------|------|
| **引用类型** | sizeof 引用类型返回被引用类型的大小 |
| **空类** | sizeof 空类返回非零值（通常为 1） |
| **字符类型** | `sizeof(char)`、`sizeof(signed char)`、`sizeof(unsigned char)`、`sizeof(std::byte)`（C++17）、`sizeof(char8_t)`（C++20）总是为 1 |
| **类类型** | 返回完整对象占用的字节数，包括填充字节（padding） |

## 4. 底层原理

### 编译期计算

sizeof 是**编译期运算符**，编译器在编译时就计算出结果：

1. **类型推导**：编译器根据类型或表达式推导出实际类型
2. **对齐要求**：考虑类型的对齐要求（alignment requirement）
3. **填充字节**：为满足对齐要求添加填充字节
4. **平台相关**：根据目标平台的 ABI（Application Binary Interface）确定最终大小

### 字节定义

根据计算机架构，一个字节可能由 8 位或更多位组成，确切位数记录在 `<climits>` 头文件中的 `CHAR_BIT` 宏中。

**关键保证**：
- C++ 标准规定 `sizeof(char)` 总是为 1
- 一个字节至少包含 8 位
- 在大多数现代系统中，`CHAR_BIT` 为 8

### 类类型大小计算

对于类类型，sizeof 返回的大小包括：

1. **非静态数据成员**：所有非静态数据成员的大小之和
2. **填充字节**（padding）：为满足对齐要求插入的字节
3. **虚函数表指针**：如果类有虚函数，包含虚表指针（通常 4 或 8 字节）
4. **虚基类指针**：如果使用虚继承，可能包含虚基类指针

**示例**：
```cpp
struct CharChar {
    char c;    // 1 字节
    char c2;   // 1 字节
};             // sizeof = 2（紧凑排列，无需填充）

struct CharIntChar {
    char c;     // 1 字节
                // 3 字节填充（为 int 对齐）
    int i;      // 4 字节
    char c2;    // 1 字节
                // 3 字节填充（整个对象大小需为 int 对齐的整数倍）
};              // sizeof = 12
```

### 空类大小

空类（empty class）的 sizeof 结果为非零值（通常为 1）：

```cpp
struct Empty {};
sizeof(Empty);  // 结果为 1
```

**原因**：
- C++ 标准要求每个对象必须有唯一地址
- 如果允许零大小，数组中多个空类对象会有相同地址
- 通过分配最小大小（1 字节）确保地址唯一性

### 不求值操作数

sizeof 的表达式操作数是**未求值操作数**（unevaluated operand）：

```cpp
int* p = nullptr;
sizeof(*p);  // 安全，不会解引用指针
```

**不执行的操作**：
- 不求值表达式
- 不进行左值到右值转换
- 不进行数组到指针转换
- 不进行函数到指针转换

**形式执行的操作**（C++17 起）：
- 对于纯右值参数，形式上进行临时量具体化
- 要求类型必须可析构，否则程序非良构

## 5. 使用场景

### 适合使用 sizeof 的场景

| 场景 | 示例 |
|------|------|
| 计算数组元素数量 | `sizeof(arr) / sizeof(arr[0])` |
| 动态内存分配 | `malloc(sizeof(int) * n)` |
| 类型大小判断 | `static_assert(sizeof(void*) == 8, "64-bit required")` |
| 跨平台代码 | 根据类型大小选择不同实现 |
| 序列化/反序列化 | 确定对象占用的字节数 |

### 常见用法模式

#### 1. 计算数组长度

```cpp
int arr[10];
size_t length = sizeof(arr) / sizeof(arr[0]);  // 10
```

#### 2. 类型大小验证

```cpp
static_assert(sizeof(int) == 4, "int must be 4 bytes");
static_assert(sizeof(void*) == 8, "64-bit platform required");
```

#### 3. 多态类型处理

```cpp
Derived d;
Base& b = d;
sizeof(b);  // 返回 Base 的大小，而非 Derived
            // sizeof 不进行多态，只看静态类型
```

### 最佳实践

#### 1. 优先使用类型形式

```cpp
// ✅ 推荐：类型形式，意图明确
sizeof(int)
sizeof(MyClass)

// ⚠️ 可接受：表达式形式
sizeof variable
sizeof obj->member

// ❌ 避免：容易混淆的表达式
sizeof a + b  // 解析为 (sizeof a) + b
```

#### 2. 避免误用

```cpp
// ❌ 错误：函数类型
sizeof(void());  // 编译错误

// ❌ 错误：不完整类型
sizeof(int[]);   // 编译错误

// ❌ 错误：位域（C++11 前）
struct Bit { unsigned b : 1; };
Bit bit;
sizeof(bit.b);   // 编译错误
```

#### 3. 使用 constexpr 变量

```cpp
// ✅ 推荐：使用 constexpr 提高可读性
constexpr size_t int_size = sizeof(int);
constexpr size_t ptr_size = sizeof(void*);
```

### 性能考虑

- **编译期计算**：sizeof 在编译期完成，运行时无开销
- **常量表达式**：结果可用于模板参数、数组大小等编译期上下文
- **无运行时求值**：表达式形式不会实际执行表达式

### 常见陷阱

#### 陷阱 1：指针与数组混淆

```cpp
int arr[10];
int* p = arr;

sizeof(arr);  // 40（数组大小）
sizeof(p);    // 4 或 8（指针大小）

// 数组退化为指针后，sizeof 无法获取原数组大小
void func(int arr[]) {
    sizeof(arr);  // 指针大小，而非数组大小！
}
```

#### 陷阱 2：多态类型误判

```cpp
Base* ptr = new Derived();
sizeof(*ptr);  // Base 的大小，而非 Derived
```

#### 陷阱 3：结构体对齐

```cpp
struct Example {
    char a;   // 1 字节
              // 3 字节填充
    int b;    // 4 字节
};
sizeof(Example);  // 8，不是 5
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <cstddef>

int main() {
    // 基本类型大小
    std::cout << "sizeof(char): " << sizeof(char) << '\n';           // 1
    std::cout << "sizeof(short): " << sizeof(short) << '\n';         // 通常 2
    std::cout << "sizeof(int): " << sizeof(int) << '\n';           // 通常 4
    std::cout << "sizeof(long): " << sizeof(long) << '\n';          // 平台相关
    std::cout << "sizeof(void*): " << sizeof(void*) << '\n';       // 指针大小

    // 类型形式 vs 表达式形式
    int x = 42;
    std::cout << "sizeof(int): " << sizeof(int) << '\n';           // 类型形式
    std::cout << "sizeof x: " << sizeof x << '\n';                 // 表达式形式
    std::cout << "sizeof(x): " << sizeof(x) << '\n';               // 表达式形式（加括号）

    // 数组大小
    int arr[10];
    std::cout << "Array size: " << sizeof(arr) << '\n';            // 40
    std::cout << "Array length: " << sizeof(arr)/sizeof(arr[0]) << '\n'; // 10

    return 0;
}
```

### 高级用法

```cpp
#include <iostream>
#include <cstddef>

struct Empty {};
struct Base { int a; };
struct Derived : Base { int b; };
struct Bit { unsigned b : 1; };

// 演示结构体对齐
struct CharChar { char c; char c2; };           // 通常 2 字节
struct CharIntChar { char c; int i; char c2; };  // 通常 12 字节（64位系统）

int main() {
    // 空类大小
    Empty e;
    std::cout << "sizeof(Empty): " << sizeof e << '\n';  // 1（非零）

    // 引用类型
    int ref = 10;
    int& r = ref;
    std::cout << "sizeof(int&): " << sizeof r << '\n';  // sizeof(int)，而非指针大小

    // 继承类
    Derived d;
    Base& b = d;
    std::cout << "sizeof(Derived): " << sizeof d << '\n';  // Derived 大小
    std::cout << "sizeof(Base&): " << sizeof b << '\n';    // Base 大小（静态类型）

    // 指针大小
    std::cout << "sizeof(int*): " << sizeof(int*) << '\n';        // 指针大小
    std::cout << "sizeof(void(*)()): " << sizeof(void(*)()) << '\n'; // 函数指针大小

    // 结构体对齐
    std::cout << "sizeof(CharChar): " << sizeof(CharChar) << '\n';           // 2
    std::cout << "sizeof(CharIntChar): " << sizeof(CharIntChar) << '\n';     // 12

    return 0;
}
```

### 完整示例：跨平台类型大小检测

```cpp
#include <iostream>
#include <cstddef>
#include <climits>

// 编译期检测
static_assert(CHAR_BIT == 8, "This code requires 8-bit bytes");
static_assert(sizeof(char) == 1, "sizeof(char) must be 1");

struct PlatformInfo {
    static constexpr size_t pointer_size = sizeof(void*);
    static constexpr size_t int_size = sizeof(int);
    static constexpr size_t long_size = sizeof(long);
    static constexpr bool is_64bit = (sizeof(void*) == 8);
};

int main() {
    std::cout << "Platform Information:\n";
    std::cout << "==================\n";
    std::cout << "Pointer size: " << PlatformInfo::pointer_size << " bytes\n";
    std::cout << "int size: " << PlatformInfo::int_size << " bytes\n";
    std::cout << "long size: " << PlatformInfo::long_size << " bytes\n";
    std::cout << "64-bit platform: " << (PlatformInfo::is_64bit ? "Yes" : "No") << "\n";
    std::cout << "CHAR_BIT: " << CHAR_BIT << " bits/byte\n";
    std::cout << "Maximum size_t: " << sizeof(std::size_t) << " bytes\n";

    return 0;
}
```

### 常见错误及修正

#### 错误 1：函数类型

```cpp
// ❌ 错误：sizeof 不能用于函数类型
// sizeof(void());           // 编译错误
// sizeof(int(double));      // 编译错误

// ✅ 修正：使用函数指针
sizeof(void(*)());           // 函数指针大小
sizeof(int(*)(double));      // 函数指针大小
```

#### 错误 2：不完整类型

```cpp
// ❌ 错误：不完整类型
struct Incomplete;           // 前向声明，不完整类型
// sizeof(Incomplete);        // 编译错误

// ✅ 修正：使用完整类型
struct Complete { int x; };
sizeof(Complete);            // OK
```

#### 错误 3：位域

```cpp
struct BitField {
    unsigned a : 3;
    unsigned b : 5;
};

BitField bf;

// ❌ 错误：不能对位域使用 sizeof
// sizeof(bf.a);              // 编译错误

// ✅ 修正：使用整个结构体
sizeof(bf);                  // 整个结构体的大小
sizeof(BitField);            // 同上
```

#### 错误 4：表达式求值误解

```cpp
int* p = nullptr;

// ❌ 危险想法：认为 sizeof 会解引用
// 实际上 sizeof 不求值，所以这是安全的
sizeof(*p);                  // 安全！不会解引用空指针

// 另一个例子
int x = 0;
int y = sizeof(x++);         // sizeof 在编译期求值，x 不会自增
// 此时 x 仍然是 0
```

### 实用工具函数

```cpp
#include <cstddef>
#include <iostream>

// 计算数组元素数量
template<typename T, std::size_t N>
constexpr std::size_t array_size(T (&)[N]) noexcept {
    return N;
}

// 类型大小打印工具
template<typename T>
void print_size(const char* name) {
    std::cout << "sizeof(" << name << "): " << sizeof(T) << " bytes\n";
}

int main() {
    int arr[10];
    std::cout << "Array length: " << array_size(arr) << '\n';  // 10

    print_size<int>("int");
    print_size<double>("double");
    print_size<int[10]>("int[10]");

    return 0;
}
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| **编译期计算** | 结果在编译期确定，无运行时开销 |
| **常量表达式** | 结果是 `std::size_t` 类型的常量表达式 |
| **平台相关** | 不同平台类型大小可能不同 |
| **不求值操作数** | 表达式形式不会实际求值 |
| **引用处理** | sizeof 引用返回被引用类型的大小 |

### 关键规则

1. **字符类型大小为 1**：`sizeof(char)`、`sizeof(signed char)`、`sizeof(unsigned char)` 总是为 1
2. **空类大小非零**：`sizeof(Empty)` 至少为 1
3. **包含填充字节**：类类型大小包括对齐填充
4. **不能用于不完整类型**：函数类型、不完整类型、位域不能使用 sizeof

### 使用建议

| 建议 | 原因 |
|------|------|
| 优先使用类型形式 | 意图更明确，避免混淆 |
| 使用 constexpr 变量存储结果 | 提高代码可读性 |
| 注意结构体对齐 | 理解实际内存占用 |
| 数组长度计算用模板函数 | 避免指针退化问题 |

### 相关运算符对比

| 运算符 | 功能 | 引入版本 |
|--------|------|---------|
| `sizeof` | 查询类型或对象大小 | C++98 |
| `alignof` | 查询类型对齐要求 | C++11 |
| `sizeof...` | 查询参数包元素数量 | C++11 |

### 平台差异

不同平台上类型大小可能不同：

| 类型 | 32 位系统 | 64 位系统（LP64） | 64 位系统（LLP64/Windows） |
|------|----------|------------------|---------------------------|
| `int` | 4 字节 | 4 字节 | 4 字节 |
| `long` | 4 字节 | 8 字节 | 4 字节 |
| `pointer` | 4 字节 | 8 字节 | 8 字节 |
| `long long` | 8 字节 | 8 字节 | 8 字节 |

**注意**：LP64（Linux/macOS）和 LLP64（Windows）模型在 `long` 类型大小上有差异。

### 学习建议

1. **理解字节概念**：字节大小由 `CHAR_BIT` 决定，通常为 8 位
2. **掌握对齐规则**：结构体大小受对齐要求影响
3. **注意平台差异**：跨平台代码需考虑类型大小差异
4. **避免常见陷阱**：指针与数组、多态类型、空类大小

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/sizeof
- C++ Standard: [expr.sizeof]
- Effective C++, Scott Meyers
- The C++ Programming Language, Bjarne Stroustrup