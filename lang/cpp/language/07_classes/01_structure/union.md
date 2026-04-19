# Union 声明 - 联合体

## 1. 概述

**联合体（union）** 是一种特殊的类类型，它在同一时刻只能持有其非静态数据成员中的一个。与 struct 或 class 不同，union 的所有非静态数据成员共享同一块内存空间，因此 union 的大小至少足以容纳其最大的数据成员。

联合体的核心特点：
- **内存共享**：所有非静态数据成员存储在同一地址
- **互斥存储**：同一时刻只能有一个成员处于活跃状态
- **节省空间**：适用于需要存储多种类型但只用其一的场景

联合体定义在 C++ 核心语言中，无需包含特定头文件。

## 2. 来源与演变

### 历史背景

联合体概念源于 C 语言，最初设计用于实现**类型双关（type punning）**和**节省内存**。在早期计算机内存资源稀缺的年代，union 允许程序员在不同类型之间复用同一块内存区域。

### C++98 标准

联合体被纳入 C++ 标准，继承了 C 语言的基本特性：
- 支持成员函数（包括构造函数和析构函数）
- 不允许虚函数
- 不能有基类，也不能作为基类
- 不能有引用类型的非静态数据成员
- 不能包含具有非平凡特殊成员函数（复制构造函数、复制赋值运算符、析构函数）的非静态数据成员

### C++11 变化

C++11 对联合体进行了重要扩展：

| 特性 | 说明 |
|------|------|
| 非平凡成员支持 | 允许包含具有非平凡特殊成员函数的成员，但相关函数会被删除，需显式定义 |
| 默认成员初始化器 | 最多允许一个变体成员拥有默认成员初始化器 |
| placement new | 需要显式调用析构函数和 placement new 来切换活跃成员 |
| 属性支持 | 允许在联合体声明中使用属性（attribute） |

### C++17 变化

引入 `std::variant` 作为联合体的类型安全替代方案：
- `std::variant` 提供更安全的 discriminated union 实现
- 自动管理成员生命周期
- 支持 `std::visit` 进行类型安全的访问

### C++20 变化

- 标准明确匿名联合体允许 `static_assert` 声明（CWG 1940）

## 3. 语法与参数

### 基本语法

```cpp
union attr class-head-name { member-specification }
```

| 参数 | 说明 |
|------|------|
| `attr` | （C++11 起）可选的属性序列 |
| `class-head-name` | 联合体名称，可省略（匿名联合体）。可包含嵌套名说明符 |
| `member-specification` | 成员列表，包括访问说明符、成员对象和成员函数声明 |

### 联合体的限制

```cpp
union U {
    // 允许的内容
    int n;                    // 非静态数据成员
    void foo();              // 成员函数
    U();                     // 构造函数
    ~U();                    // 析构函数

    // 禁止的内容
    // virtual void bar();   // 错误：不能有虚函数
    // int& ref;             // 错误：不能有引用成员
    // static int s;         // 非匿名联合体可以，匿名联合体不可以
};
```

### 成员访问权限

与 struct 类似，union 的默认成员访问权限是 public：

```cpp
union U {
    int x;      // 默认 public
private:
    double d;   // 显式 private
};
```

### 匿名联合体语法

```cpp
union { member-specification };
```

匿名联合体的额外限制：
- 不能有成员函数
- 不能有静态数据成员
- 所有数据成员必须是 public
- 只允许非静态数据成员和 `static_assert` 声明

## 4. 底层原理

### 内存布局

联合体的所有非静态数据成员共享同一块内存，大小至少等于最大成员的大小：

```
union S {
    int32_t n;      // 4 字节
    uint16_t s[2];  // 4 字节
    uint8_t c;      // 1 字节
};  // 整个 union 占用 4 字节

内存布局示意:
地址:    0x100  0x101  0x102  0x103
n:      [======== int32_t ========]
s[0]:   [==uint16_t==]
s[1]:          [==uint16_t==]
c:      [u8]
```

**关键特性**：
- 所有非静态数据成员具有相同的地址
- 具体分配细节是实现定义的
- 从非活跃成员读取是未定义行为（大多数编译器作为扩展支持）

### 活跃成员与生命周期

联合体成员的生命周期规则：

1. **生命周期开始**：当成员变为活跃成员时
2. **生命周期结束**：当另一个成员变为活跃成员时

```cpp
union U {
    std::string str;
    std::vector<int> vec;
};

U u;
u.str = "hello";     // str 是活跃成员
u.str.~basic_string(); // 显式析构
new (&u.vec) std::vector<int>; // placement new，vec 成为活跃成员
```

### 隐式创建对象

当通过赋值表达式 `E1 = E2` 切换活跃成员时（使用内置赋值或平凡赋值运算符），如果修改会导致类型别名规则下的未定义行为，编译器会隐式创建对象：

```cpp
union A { int x; int y[4]; };
struct B { A a; };
union C { B b; int k; };

int f() {
    C c;
    c.b.a.y[3] = 4;  // OK: 隐式创建 c.b 和 c.b.a.y
    return c.b.a.y[3];
}
```

### 平凡特殊成员函数

联合体的平凡移动构造函数、移动赋值运算符、复制构造函数和复制赋值运算符会复制对象表示：

- 如果源和目标不是同一对象：在复制前为目标中对应的嵌套对象开始生命周期
- 如果是同一对象：什么都不做
- 复制后两个联合体对象具有相同的活跃成员

## 5. 使用场景

### 类型双关（Type Punning）

联合体常用于在不同类型间重新解释相同的位模式：

| 场景 | 说明 |
|------|------|
| 浮点数解析 | 查看浮点数的内部表示 |
| 字节序转换 | 大小端数据转换 |
| 网络协议 | 解析不同格式的数据包 |
| 硬件接口 | 寄存器映射 |

**注意**：标准规定从非活跃成员读取是未定义行为，但大多数编译器支持这种用法。

### 节省内存

当数据结构需要在多种类型中存储一种时：

```cpp
struct Config {
    enum Type { INT, FLOAT, STRING } type;
    union {
        int int_val;
        float float_val;
        char string_val[32];
    };
};
```

### 标签联合（Tagged Union）

结合枚举标签实现类型安全的多态：

```cpp
struct TaggedValue {
    enum Type { CHAR, INT, DOUBLE } tag;
    union {
        char c;
        int i;
        double d;
    };
};
```

### 匿名联合体

成员直接注入到外围作用域，简化访问：

```cpp
struct Data {
    enum Kind { NUMBER, TEXT } kind;
    union {
        int number;
        const char* text;
    };  // 匿名联合体
};

Data d;
d.kind = Data::NUMBER;
d.number = 42;  // 直接访问，无需 d.value.number
```

### 现代 C++ 替代方案

| 方案 | 优点 | 适用场景 |
|------|------|---------|
| `std::variant` (C++17) | 类型安全、自动生命周期管理 | 新代码首选 |
| `std::any` (C++17) | 完全动态类型 | 需要存储任意类型 |
| `std::optional` (C++17) | 表示可能为空的值 | 可选值语义 |

## 6. 代码示例

### 基础用法

```cpp
#include <cstdint>
#include <iostream>

union S {
    std::int32_t n;     // 占用 4 字节
    std::uint16_t s[2]; // 占用 4 字节
    std::uint8_t c;     // 占用 1 字节
};                      // 整个 union 占用 4 字节

int main() {
    S s = {0x12345678}; // 初始化第一个成员，s.n 是活跃成员

    std::cout << std::hex << "s.n = " << s.n << '\n';

    s.s[0] = 0x0011;    // s.s 现在是活跃成员
    std::cout << "s.c = " << +s.c << '\n'    // 0x11 或 0x00，取决于平台
              << "s.n = " << s.n << '\n';      // 值取决于字节序
    return 0;
}
```

### 类联合体实现标签联合

```cpp
#include <iostream>

// S 有一个非静态数据成员 (tag)，三个枚举成员 (CHAR, INT, DOUBLE)
// 和三个变体成员 (c, i, d)
struct S {
    enum { CHAR, INT, DOUBLE } tag;
    union {
        char c;
        int i;
        double d;
    };
};

void print_s(const S& s) {
    switch (s.tag) {
        case S::CHAR:    std::cout << s.c << '\n'; break;
        case S::INT:     std::cout << s.i << '\n'; break;
        case S::DOUBLE:  std::cout << s.d << '\n'; break;
    }
}

int main() {
    S s = {S::CHAR, 'a'};
    print_s(s);
    s.tag = S::INT;
    s.i = 123;
    print_s(s);
    return 0;
}
```

### 复杂成员的正确处理（C++11）

```cpp
#include <iostream>
#include <string>
#include <vector>

union S {
    std::string str;
    std::vector<int> vec;
    ~S() {}  // 需要知道哪个成员是活跃的，只能在类联合体中实现
};

int main() {
    S s = {"Hello, world"};  // str 是活跃成员
    std::cout << "s.str = " << s.str << '\n';

    s.str.~basic_string();    // 显式析构 str
    new (&s.vec) std::vector<int>;  // placement new，vec 成为活跃成员

    s.vec.push_back(10);
    std::cout << s.vec.size() << '\n';

    s.vec.~vector();          // 显式析构 vec
    return 0;
}
```

### 现代 C++17 替代方案

```cpp
#include <iostream>
#include <variant>

int main() {
    // 使用 std::variant 替代联合体
    std::variant<char, int, double> s = 'a';

    // 类型安全的访问
    std::visit([](auto x) { std::cout << x << '\n'; }, s);

    s = 123;  // 自动管理生命周期
    std::visit([](auto x) { std::cout << x << '\n'; }, s);

    return 0;
}
```

### 常见错误及修正

#### 错误 1：未正确管理活跃成员生命周期

```cpp
// 错误：直接切换活跃成员导致未定义行为
union Bad {
    std::string str;
    int n;
};

void wrong_usage() {
    Bad u;
    u.str = "hello";
    u.n = 42;  // 错误：str 的析构函数未被调用，内存泄漏！
}

// 修正：正确管理生命周期
union Good {
    std::string str;
    int n;

    ~Good() {}  // 需要显式析构
};

void correct_usage() {
    Good u;
    new (&u.str) std::string("hello");
    std::cout << u.str << '\n';
    u.str.~basic_string();  // 显式析构
    u.n = 42;               // 现在可以安全使用 n
}
```

#### 错误 2：从非活跃成员读取

```cpp
union U {
    float f;
    int i;
};

void wrong_read() {
    U u;
    u.f = 3.14f;
    // int x = u.i;  // 未定义行为：从非活跃成员读取
}

// 修正：通过 memcpy 实现类型双关（标准允许的方式）
#include <cstring>
void safe_type_punning() {
    float f = 3.14f;
    int i;
    std::memcpy(&i, &f, sizeof(int));  // 标准定义的行为
}
```

#### 错误 3：匿名联合体的使用限制

```cpp
// 错误：匿名联合体的非法用法
void bad_anonymous_union() {
    union {
        static int s;      // 错误：不能有静态成员
        void foo();        // 错误：不能有成员函数
    private:
        int x;             // 错误：不能有 private 成员
    };
}

// 修正：合法的匿名联合体
void good_anonymous_union() {
    union {
        int a;
        const char* p;
        static_assert(sizeof(int) == 4, "int must be 4 bytes");
    };
    a = 1;      // 直接访问
    p = "test"; // 直接访问
}
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 内存共享 | 所有非静态数据成员共享同一内存地址 |
| 大小 | 至少等于最大成员的大小 |
| 活跃成员 | 同一时刻只有一个成员处于活跃状态 |
| 访问限制 | 从非活跃成员读取是未定义行为 |
| 成员函数 | 支持（包括构造/析构函数），但不支持虚函数 |
| 继承 | 不支持继承和作为基类 |

### 技术对比

| 特性 | union | struct | std::variant (C++17) |
|------|-------|--------|----------------------|
| 内存占用 | 最小 | 所有成员之和 | 最大成员 + 标签 |
| 类型安全 | 无 | 有 | 完全 |
| 生命周期管理 | 手动 | 自动 | 自动 |
| 成员函数 | 支持 | 支持 | N/A |
| 访问方式 | 成员名 | 成员名 | std::get/std::visit |

### 使用建议

1. **新代码优先使用 `std::variant`**：类型安全、自动生命周期管理
2. **需要类型双关时**：使用 `std::memcpy` 而非 union（标准保证行为）
3. **内存受限环境**：union 仍是节省内存的有效工具
4. **复杂成员类型**：必须正确管理生命周期，显式调用析构函数和 placement new
5. **避免从非活跃成员读取**：虽然编译器通常支持，但属于未定义行为

### 注意事项

- **未定义行为**：从非活跃成员读取会导致未定义行为
- **生命周期陷阱**：包含非平凡类型成员时需要手动管理生命周期
- **字节序依赖**：联合体的行为依赖于平台字节序
- **初始化限制**：最多一个变体成员可以有默认成员初始化器

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 11.5 Unions [class.union]
- C++20 标准 (ISO/IEC 14882:2020): 11.5 Unions [class.union]
- C++17 标准 (ISO/IEC 14882:2017): 12.3 Unions [class.union]
- cppreference: https://en.cppreference.com/w/cpp/language/union