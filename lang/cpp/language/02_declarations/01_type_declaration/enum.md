# 枚举声明 (Enumeration Declaration)

## 1. 概述 (Overview)

**枚举 (Enumeration)** 是一种独特的类型，其值被限制在一个值范围内，该范围可能包含若干显式命名的常量（称为"**枚举器 (enumerators)**"）。

枚举常量的值是属于某种整型类型的值，该整型类型称为枚举的**底层类型 (underlying type)**。枚举与其底层类型具有相同的大小、值表示和对齐要求。此外，枚举的每个值与底层类型的对应值具有相同的表示形式。

### 枚举类型分类

C++ 提供两种截然不同的枚举类型：

| 类型 | 关键字 | 特点 |
|------|--------|------|
| 无作用域枚举 (unscoped enumeration) | `enum` | 枚举器在枚举作用域外可见，可隐式转换为整型 |
| 有作用域枚举 (scoped enumeration) | `enum class` 或 `enum struct` | 枚举器仅在枚举作用域内可见，禁止隐式转换（C++11 起） |

### 核心特性

- 枚举值限制为预定义的命名常量集合
- 底层类型默认为整型，可显式指定
- 提供类型安全的常量定义机制
- 支持位标志和选项组合模式

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

枚举类型起源于 C 语言，最早用于定义一组相关的命名常量。C++ 继承了这一特性，并逐步增强其类型安全性和表达能力。

### 版本演进

| 版本 | 特性 | 说明 |
|------|------|------|
| C++98 | 无作用域枚举 | 基本枚举功能，枚举器泄露到外围作用域 |
| C++11 | 有作用域枚举 | 使用 `enum class` 或 `enum struct`，提供更强的类型安全 |
| C++11 | 固定底层类型 | 允许显式指定枚举的底层类型 |
| C++11 | 不透明枚举声明 | 允许前置声明枚举类型 |
| C++11 | 枚举器属性 | 支持为枚举器添加属性 |
| C++17 | 列表初始化 | 允许从整数值使用列表初始化创建枚举值 |
| C++20 | `using enum` 声明 | 简化枚举器的访问，可将枚举器引入当前作用域 |

### 设计动机

1. **解决命名冲突**：无作用域枚举的枚举器会污染外围命名空间，有作用域枚举解决了这个问题
2. **增强类型安全**：有作用域枚举禁止隐式转换为整型，避免意外错误
3. **前置声明支持**：固定底层类型的枚举可前置声明，改善编译依赖
4. **大小可控**：显式指定底层类型可精确控制枚举大小

### 特性测试宏

| 宏 | 值 | 标准 | 特性 |
|----|-----|------|------|
| `__cpp_enumerator_attributes` | `201411L` | C++17 | 枚举器属性 |
| `__cpp_using_enum` | `201907L` | C++20 | `using enum` 声明 |

### 缺陷报告修正

| 缺陷报告 | 适用版本 | 原行为 | 修正后行为 |
|----------|----------|--------|------------|
| CWG 377 | C++98 | 无整型能表示所有枚举器值时行为未指定 | 此情况下枚举格式错误 (ill-formed) |
| CWG 518 | C++98 | 枚举器列表后不允许尾随逗号 | 允许尾随逗号 |
| CWG 1514 | C++11 | 类成员声明中固定底层类型枚举重定义可能被解析为位域 | 始终解析为重定义 |
| CWG 1638 | C++11 | 不透明枚举声明语法禁止模板特化使用 | 允许嵌套名说明符 |
| CWG 1766 | C++98 | 超范围值转换为无固定底层类型枚举结果未指定 | 行为未定义 |
| CWG 1966 | C++11 | CWG 1514 修正使条件表达式的 `:` 被解析为 enum-base | 仅对成员声明说明符应用修正 |
| CWG 2156 | C++11 | 枚举定义可通过 using 声明定义枚举类型 | 禁止 |
| CWG 2157 | C++11 | CWG 1966 修正未覆盖限定枚举名 | 已覆盖 |
| CWG 2530 | C++98 | 枚举器列表可包含多个相同标识符的枚举器 | 禁止 |
| CWG 2590 | C++98 | 枚举的大小、值表示和对齐要求不依赖底层类型 | 全部与底层类型相同 |
| CWG 2621 | C++20 | using enum 声明中的枚举名查找不清晰 | 已澄清 |
| CWG 2877 | C++20 | using enum 声明中的枚举名查找不是仅类型查找 | 改为仅类型查找 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 枚举声明语法

```cpp
// 基本语法形式
enum-key attr(可选) enum-head-name(可选) enum-base(可选) { enumerator-list(可选) }  // (1)
enum-key attr(可选) enum-head-name(可选) enum-base(可选) { enumerator-list, }        // (2)
enum-key attr(可选) enum-head-name enum-base(可选) ;                                 // (3) C++11 起
```

**语法说明**：
- 形式 (1)(2)：枚举说明符 (enum-specifier)，定义枚举类型及其枚举器
- 形式 (2)：允许枚举器列表后跟尾随逗号
- 形式 (3)：不透明枚举声明 (opaque enum declaration)，定义枚举类型但不定义枚举器，此后类型为完整类型且大小已知

### 参数详解

| 参数 | 说明 |
|------|------|
| `enum-key` | C++11 前：`enum`；C++11 起：`enum`、`enum class` 或 `enum struct` 之一 |
| `attr` | C++11 起，可选的任意数量属性序列 |
| `enum-head-name` | 被声明的枚举名称。C++11 起可带嵌套名说明符（用于重新声明）；仅在不透明枚举声明时不可省略（无作用域非不透明枚举可省略） |
| `enum-base` | C++11 起，冒号后跟整型类型说明符（忽略 cv 限定），指定固定底层类型 |
| `enumerator-list` | 枚举器定义列表，以逗号分隔，每项为标识符或 `identifier = constant-expression`；C++17 起标识符后可跟属性 |

### 无作用域枚举语法

```cpp
// 形式 1：底层类型未固定
enum name(可选) { enumerator = constant-expression, ... }

// 形式 2：底层类型固定 (C++11 起)
enum name(可选) : type { enumerator = constant-expression, ... }

// 形式 3：不透明枚举声明 (C++11 起)
enum name : type ;
```

**底层类型规则**：
- 形式 1：底层类型为实现定义的整型，能表示所有枚举器值；不大于 `int`（除非值超出）；空列表时底层类型如同有唯一值为 0 的枚举器
- 形式 2：底层类型为指定的 `type`
- 形式 3：必须指定名称和底层类型

### 有作用域枚举语法 (C++11 起)

```cpp
// 形式 1：底层类型默认为 int
enum struct|class name { enumerator = constant-expression, ... }

// 形式 2：指定底层类型
enum struct|class name : type { enumerator = constant-expression, ... }

// 形式 3：不透明枚举声明，底层类型为 int
enum struct|class name ;

// 形式 4：不透明枚举声明，指定底层类型
enum struct|class name : type ;
```

**注意**：`enum class` 和 `enum struct` 完全等价。

### 枚举器定义

```cpp
identifier                    // 简单标识符
identifier = constant-expression  // 带初始值
```

**值分配规则**：
- 首个枚举器若无初始值：值为 0
- 后续枚举器若无初始值：值为前一个枚举器值 + 1

```cpp
enum Foo { a, b, c = 10, d, e = 1, f, g = f + c };
// a = 0, b = 1, c = 10, d = 11, e = 1, f = 2, g = 12
```

### using enum 声明语法 (C++20 起)

```cpp
using enum using-enum-declarator ;
```

其中 `declarator` 为（可能带限定符的）标识符或简单模板标识符，必须命名一个非依赖的枚举类型。

---

## 4. 底层原理 (Underlying Principles)

### 枚举值的内部表示

枚举类型在底层以整型实现，其特性如下：

1. **大小与对齐**：枚举的大小、值表示和对齐要求与其底层类型完全相同
2. **值表示**：每个枚举值与底层类型的对应值具有相同的位模式
3. **底层类型推导**：
   - 无固定底层类型时：由实现定义，能表示所有枚举器值的最小整型（不超过 `int` 或 `unsigned int`，除非值超出）
   - 有固定底层类型时：使用指定的类型

### 枚举的有效值范围

对于无固定底层类型的枚举，其有效值范围由以下规则确定：

- 若 `e_min` 是最小枚举器值，`e_max` 是最大枚举器值
- 设 `b` 为底层类型的位数
- 若底层类型有符号：范围为 `max(-2^(b-1), e_min)` 到 `min(2^(b-1)-1, e_max)`
- 若底层类型无符号：范围为 `0` 到 `max(2^b - 1, e_max)`

### 类型转换

| 转换方向 | 无作用域枚举 | 有作用域枚举 |
|----------|--------------|--------------|
| 枚举 → 整型 | 隐式转换（整型提升） | 需 `static_cast` |
| 整型 → 枚举 | 需 `static_cast`（可能 UB） | 需 `static_cast`（可能 UB） |
| 枚举 → 浮点 | 隐式转换 | 需 `static_cast` |

**注意事项**：使用 `static_cast` 从整型转换到枚举时，如果值超出枚举的有效范围，行为是未定义的（CWG 1766 起）。

```cpp
enum access_t { read = 1, write = 2, exec = 4 }; // 枚举器：1, 2, 4；范围：0..7
access_t rwe = static_cast<access_t>(7);  // OK
access_t x = static_cast<access_t>(8.0);  // 未定义行为 (CWG 1766 起)
access_t y = static_cast<access_t>(8);    // 未定义行为 (CWG 1766 起)

enum foo { a = 0, b = UINT_MAX };          // 范围：[0, UINT_MAX]
foo x = foo(-1);                           // 未定义行为 (CWG 1766 起)
```

### 列表初始化特殊规则 (C++17 起)

从整数初始化枚举可使用列表初始化，条件如下：
- 是直接列表初始化
- 初始化列表仅有一个元素
- 枚举是有作用域枚举或底层类型固定的无作用域枚举
- 转换非窄化

```cpp
enum byte : unsigned char {};  // byte 是新整数类型；参见 std::byte (C++17)
byte b{42};        // OK (C++17 起，直接列表初始化)
byte c = {42};     // 错误：复制列表初始化
byte d = byte{42}; // OK (C++17 起)
byte e{-1};        // 错误：窄化转换

struct A { byte b; };
A a1 = {{42}};     // 错误（构造函数参数的复制列表初始化）
A a2 = {byte{42}}; // OK (C++17 起)

void f(byte);
f({42});           // 错误（函数参数的复制列表初始化）

enum class Handle : std::uint32_t { Invalid = 0 };
Handle h{42};      // OK (C++17 起)
```

### 用于链接目的的枚举名

没有 typedef 名用于链接目的且含有枚举器的无名枚举，对于链接目的而言，由其底层类型和首个枚举器表示；此类枚举称为**以枚举器作为链接名**。

### 类成员枚举解析规则 (C++11 起)

在成员声明的声明说明符中，序列 `enum` enum-head-name `:` 总是被解析为枚举声明的一部分：

```cpp
struct S {
    enum E1 : int {};
    enum E1 : int {}; // 错误：枚举重定义，而非类型 enum E1 的零长度位域
};

enum E2 { e1 };

void f() {
    false ? new enum E2 : int(); // OK：'int' 不被解析为底层类型
}
```

---

## 5. 使用场景 (Use Cases)

### 适用场景

#### 场景一：定义状态机状态
```cpp
enum class State { Idle, Running, Paused, Stopped };
```

#### 场景二：位标志组合
```cpp
enum class Permission : unsigned int {
    Read    = 1 << 0,  // 1
    Write   = 1 << 1,  // 2
    Execute = 1 << 2,  // 4
    All     = Read | Write | Execute
};
```

#### 场景三：类型安全常量
```cpp
enum class Handle : std::uint32_t { Invalid = 0 };
Handle h{42};  // C++17 起，类型安全
```

#### 场景四：新整数类型定义 (C++17 起)
```cpp
enum byte : unsigned char {};  // 类似 std::byte
byte b{42};  // 直接列表初始化
```

### 最佳实践

#### 优先使用有作用域枚举 (C++11 起)
```cpp
// 推荐
enum class Color { Red, Green, Blue };

// 避免（无作用域枚举会污染命名空间）
enum Color { Red, Green, Blue };  // Red, Green, Blue 泄露到外围作用域
```

#### 显式指定底层类型
```cpp
// 明确大小，利于序列化和跨平台
enum class MessageType : uint8_t {
    Data = 0x01,
    Control = 0x02,
    Error = 0xFF
};
```

#### 使用 using enum 简化代码 (C++20 起)
```cpp
enum class Status { OK, Error, Pending };

void process() {
    using enum Status;
    switch (current_status) {
        case OK:      // 无需 Status::OK
        case Error:
        case Pending: break;
    }
}
```

#### 在类中使用 using enum
```cpp
enum class fruit { orange, apple };

struct S {
    using enum fruit;  // 将 orange, apple 引入 S 的作用域
};

void f() {
    S s;
    s.orange;   // OK: 指向 fruit::orange
    S::orange;  // OK: 指向 fruit::orange
}
```

### 常见陷阱

#### 陷阱一：无作用域枚举命名冲突
```cpp
enum Color { Red, Green, Blue };
enum TrafficLight { Red, Yellow, Green };  // 错误：Red, Green 重定义
```

#### 陷阱二：越界转换导致未定义行为
```cpp
enum class Access : int { Read = 1, Write = 2, Exec = 4 };
Access x = static_cast<Access>(8);  // UB: 8 超出有效范围
```

#### 陷阱三：有作用域枚举的隐式转换
```cpp
enum class Color { Red, Green, Blue };
Color c = Color::Red;
int n = c;           // 错误：无隐式转换
int n = static_cast<int>(c);  // 正确
```

#### 陷阱四：无固定底层类型的枚举前置声明
```cpp
enum Color;  // 错误：无固定底层类型的枚举不能前置声明
enum class Color;  // 正确：底层类型默认为 int
enum Color : int;   // 正确：指定了底层类型
```

#### 陷阱五：using enum 名称冲突
```cpp
enum class fruit { orange, apple };
enum class color { red, orange };

void f() {
    using enum fruit;    // OK
    // using enum color; // 错误：color::orange 和 fruit::orange 冲突
}
```

---

## 6. 代码示例 (Examples)

### 基础用法

#### 示例 1：无作用域枚举
```cpp
#include <iostream>

enum Color { red, green, blue };

int main() {
    Color r = red;  // 枚举器在作用域内可见

    switch(r) {
        case red:   std::cout << "red\n";   break;
        case green: std::cout << "green\n"; break;
        case blue:  std::cout << "blue\n";  break;
    }

    int n = blue;  // 隐式转换为 int，n = 2
    std::cout << "blue value: " << n << '\n';

    return 0;
}
```

#### 示例 2：有作用域枚举 (C++11)
```cpp
#include <iostream>

enum class Color { red, green = 20, blue };

int main() {
    Color r = Color::blue;  // 必须使用作用域限定符

    switch(r) {
        case Color::red:   std::cout << "red\n";   break;
        case Color::green: std::cout << "green\n"; break;
        case Color::blue:  std::cout << "blue\n";  break;
    }

    // int n = r;  // 错误：无隐式转换
    int n = static_cast<int>(r);  // 正确，n = 21
    std::cout << "blue value: " << n << '\n';

    return 0;
}
```

#### 示例 3：指定底层类型
```cpp
#include <cstdint>
#include <iostream>

// 16 位枚举
enum smallenum : std::int16_t { a, b, c };

// 颜色：red(0), yellow(1), green(20), blue(21)
enum color {
    red,
    yellow,
    green = 20,
    blue
};

// 字符类型枚举
enum class altitude : char {
    high = 'h',
    low = 'l',  // CWG 518 后允许尾随逗号
};

int main() {
    std::cout << "sizeof(smallenum): " << sizeof(smallenum) << '\n';  // 2
    std::cout << "sizeof(altitude): " << sizeof(altitude) << '\n';    // 1

    altitude al = altitude::high;
    std::cout << static_cast<char>(al) << '\n';  // 'h'

    return 0;
}
```

#### 示例 4：枚举器值计算
```cpp
#include <iostream>

int main() {
    // d = 0, e = 1, f = 3
    enum { d, e, f = e + 2 };
    std::cout << "d = " << d << ", e = " << e << ", f = " << f << '\n';

    return 0;
}
```

### 高级用法

#### 示例 5：类成员枚举
```cpp
#include <iostream>

struct X {
    enum direction { left = 'l', right = 'r' };
};

int main() {
    X x;
    X* p = &x;

    int a = X::direction::left;  // C++11 起允许
    int b = X::left;             // 无作用域枚举器直接访问
    int c = x.left;              // 成员访问
    int d = p->left;             // 指针成员访问

    std::cout << "left = " << static_cast<char>(b) << '\n';  // 'l'

    return 0;
}
```

#### 示例 6：枚举与位运算
```cpp
#include <iostream>

enum class Permission : unsigned int {
    None    = 0,
    Read    = 1 << 0,
    Write   = 1 << 1,
    Execute = 1 << 2
};

// 位运算操作符重载
constexpr Permission operator|(Permission a, Permission b) {
    return static_cast<Permission>(
        static_cast<unsigned int>(a) | static_cast<unsigned int>(b)
    );
}

constexpr Permission operator&(Permission a, Permission b) {
    return static_cast<Permission>(
        static_cast<unsigned int>(a) & static_cast<unsigned int>(b)
    );
}

constexpr bool has_permission(Permission flags, Permission test) {
    return (static_cast<unsigned int>(flags) &
            static_cast<unsigned int>(test)) != 0;
}

int main() {
    Permission user_perm = Permission::Read | Permission::Write;

    if (has_permission(user_perm, Permission::Read)) {
        std::cout << "Has read permission\n";
    }
    if (!has_permission(user_perm, Permission::Execute)) {
        std::cout << "No execute permission\n";
    }

    return 0;
}
```

#### 示例 7：using enum 声明 (C++20)
```cpp
#include <iostream>

enum E { x };

void f() {
    int E;          // 隐藏枚举 E
    using enum E;  // OK：通过类型查找找到枚举
}

using F = E;
using enum F;  // OK

template<class T> using EE = T;

void g() {
    using enum EE<E>;  // OK
}

enum class fruit { orange, apple };
enum class color { red, orange };

struct S {
    using enum fruit;  // OK：将 orange, apple 引入 S 的作用域
};

void h() {
    S s;
    s.orange;   // OK: 指向 fruit::orange
    S::orange;  // OK: 指向 fruit::orange
}
```

#### 示例 8：列表初始化创建枚举 (C++17)
```cpp
#include <cstdint>

enum byte : unsigned char {};  // 新整数类型
enum class Handle : std::uint32_t { Invalid = 0 };

int main() {
    byte b{42};        // OK: 直接列表初始化
    // byte c = {42};  // 错误: 复制列表初始化不允许
    byte d = byte{42}; // OK

    Handle h{42};      // OK

    return 0;
}
```

#### 示例 9：输出操作符重载
```cpp
#include <iostream>

enum color { red, yellow, green = 20, blue };

// 枚举类型可以重载操作符
std::ostream& operator<<(std::ostream& os, color c) {
    switch(c) {
        case red:    os << "red";    break;
        case yellow: os << "yellow"; break;
        case green:  os << "green";  break;
        case blue:   os << "blue";   break;
        default:     os.setstate(std::ios_base::failbit);
    }
    return os;
}

enum class altitude : char { high = 'h', low = 'l' };

std::ostream& operator<<(std::ostream& os, altitude al) {
    return os << static_cast<char>(al);
}

int main() {
    color col = red;
    altitude a = altitude::low;

    std::cout << "col = " << col << '\n'
              << "a = "   << a   << '\n';

    return 0;
}
```

#### 示例 10：C++20 using enum 完整示例
```cpp
#include <iostream>

namespace cxx20 {
    enum class long_long_long_name { x, y };

    void using_enum_demo() {
        std::cout << "C++20 `using enum`: __cpp_using_enum == ";
        switch (auto rnd = []{ return long_long_long_name::x; }; rnd()) {
#if defined(__cpp_using_enum)
            using enum long_long_long_name;
            case x: std::cout << __cpp_using_enum << "; x\n"; break;
            case y: std::cout << __cpp_using_enum << "; y\n"; break;
#else
            case long_long_long_name::x: std::cout << "?; x\n"; break;
            case long_long_long_name::y: std::cout << "?; y\n"; break;
#endif
        }
    }
}

int main() {
    cxx20::using_enum_demo();
    return 0;
}
```

### 常见错误及修正

#### 错误 1：无作用域枚举命名污染
```cpp
// 错误示例
enum Status { OK, Error };  // OK 和 Error 泄露到全局作用域
int OK = 0;  // 错误：重定义

// 修正：使用有作用域枚举
enum class Status { OK, Error };
int OK = 0;  // OK
```

#### 错误 2：枚举前置声明错误
```cpp
// 错误示例
enum Color;  // 错误：无固定底层类型不能前置声明

// 修正：指定底层类型
enum Color : int;  // OK
// 或使用有作用域枚举
enum class Color;  // OK，底层类型默认为 int
```

#### 错误 3：越界值转换
```cpp
enum access_t { read = 1, write = 2, exec = 4 };  // 范围：0..7

// 危险：值 8 超出有效范围
access_t x = static_cast<access_t>(8);  // C++11 起：未定义行为

// 修正：确保值在有效范围内
int val = 8;
if (val >= 0 && val <= 7) {
    access_t y = static_cast<access_t>(val);  // 安全
}
```

#### 错误 4：C++11 前模拟有作用域枚举
```cpp
// C++11 有作用域枚举
enum struct E11 { x, y };  // C++11 起

// C++11 前模拟方法
struct E98 { enum { x, y }; };           // 方法 1：嵌套在结构体中
namespace N98 { enum { x, y }; }         // 方法 2：使用命名空间
struct S98 { static const int x = 0, y = 1; };  // 方法 3：静态常量

// 使用示例
void emu() {
    std::cout << (static_cast<int>(E11::y) + E98::y + N98::y + S98::y) << '\n';  // 4
}
```

---

## 7. 总结 (Summary)

### 核心要点

1. **两种枚举类型**：
   - 无作用域枚举 (`enum`)：枚举器泄露到外围作用域，可隐式转换为整型
   - 有作用域枚举 (`enum class`/`enum struct`)：类型安全，需显式转换

2. **底层类型**：
   - C++11 起可显式指定底层类型
   - 未指定时由实现定义（无作用域）或默认为 `int`（有作用域）

3. **C++11 重要增强**：
   - 有作用域枚举提供类型安全
   - 可指定固定底层类型
   - 支持前置声明

4. **C++17 列表初始化**：允许从整数值直接列表初始化枚举

5. **C++20 using enum**：简化枚举器访问，可将枚举器引入当前作用域

### 无作用域枚举 vs 有作用域枚举对比

| 特性 | 无作用域枚举 | 有作用域枚举 |
|------|--------------|--------------|
| 关键字 | `enum` | `enum class` / `enum struct` |
| 枚举器作用域 | 外围作用域 | 枚举作用域内 |
| 隐式整型转换 | 支持 | 禁止 |
| 前置声明 | 需指定底层类型 | 直接支持 |
| 底层类型 | 实现定义 | 默认 `int`，可指定 |
| 命名冲突风险 | 高 | 低 |

### 相关标准库支持

| 工具 | 版本 | 功能 |
|------|------|------|
| `std::is_enum` | C++11 | 检查类型是否为枚举类型 |
| `std::is_scoped_enum` | C++23 | 检查类型是否为有作用域枚举类型 |
| `std::underlying_type` | C++11 | 获取枚举的底层类型 |
| `std::to_underlying` | C++23 | 将枚举转换为其底层类型 |

### 学习建议

1. **现代 C++ 优先使用 `enum class`**：避免命名污染和隐式转换问题
2. **始终指定底层类型**：提高可移植性，便于序列化
3. **使用 `constexpr` 操作符重载**：为枚举添加位运算支持
4. **利用 `using enum` 简化代码**：在适当作用域内使用 C++20 特性减少重复代码

---

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 9.7.1 Enumeration declarations [dcl.enum]
- C++20 标准 (ISO/IEC 14882:2020): 9.7.1 Enumeration declarations [dcl.enum]
- C++17 标准 (ISO/IEC 14882:2017): 10.2 Enumeration declarations [dcl.enum]
- C++14 标准 (ISO/IEC 14882:2014): 7.2 Enumeration declarations [dcl.enum]
- C++11 标准 (ISO/IEC 14882:2011): 7.2 Enumeration declarations [dcl.enum]
- C++03 标准 (ISO/IEC 14882:2003): 7.2 Enumeration declarations [dcl.enum]
- C++98 标准 (ISO/IEC 14882:1998): 7.2 Enumeration declarations [dcl.enum]