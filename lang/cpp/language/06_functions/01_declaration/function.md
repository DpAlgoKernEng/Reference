# 函数声明 (Function Declaration)

## 1. 概述

函数声明（Function Declaration）用于引入函数名称及其类型。函数定义（Function Definition）则将函数名称/类型与函数体关联起来。函数是 C++ 程序的基本构建块，封装了一段可重用的代码逻辑。

函数声明可以出现在任何作用域中。在类作用域中的函数声明会引入类成员函数（除非使用了 `friend` 说明符）。函数声明定义了函数的接口，包括函数名、返回类型、参数列表以及可能的异常规范等。

## 2. 来源与演变

### 首次引入

函数的概念从 C 语言继承而来，C++ 在此基础上进行了大量扩展。

### C++11 变化

- **尾置返回类型（Trailing Return Type）**：允许将返回类型放在参数列表之后，使用 `auto` 和 `->` 语法
- **返回类型推导**：支持 `decltype(auto)` 作为返回类型占位符
- **属性（Attributes）**：支持在函数声明中使用属性
- **删除函数（Deleted Functions）**：使用 `= delete` 显式删除函数
- **默认函数（Defaulted Functions）**：使用 `= default` 显式默认特殊成员函数
- **`noexcept` 规范**：引入新的异常规范机制
- **引用限定符（Ref-qualifier）**：成员函数可以使用 `&` 或 `&&` 限定

### C++14 变化

- **返回类型推导扩展**：普通函数可以使用 `auto` 进行返回类型推导
- 支持多个 `return` 语句，只要它们推导出相同的类型
- 递归函数可以使用返回类型推导

### C++17 变化

- **异常规范作为类型的一部分**：`noexcept` 规范成为函数类型的一部分
- 动态异常规范被移除

### C++20 变化

- **约束（Constraints）**：函数声明可以包含 `requires` 子句
- **缩写函数模板（Abbreviated Function Templates）**：参数可以使用 `auto` 或概念类型
- `__cpp_return_type_deduction` 特性测试宏

### C++23 变化

- **显式对象参数（Explicit Object Parameter）**：使用 `this` 关键字声明显式对象参数，又称 "Deducing this"
- 显式对象参数简化了成员函数的泛型编程

### C++26 变化

- **带原因的删除函数**：`delete` 可以附带字符串说明删除原因
- 省略号前的逗号变为弃用特性

## 3. 语法与参数

### 函数声明语法

```cpp
// 常规函数声明语法
noptr-declarator ( parameter-list ) cv(可选) ref(可选) except(可选) attr(可选)

// 尾置返回类型声明语法 (C++11 起)
noptr-declarator ( parameter-list ) cv(可选) ref(可选) except(可选) attr(可选) -> trailing
```

#### 语法元素说明

| 元素 | 说明 |
|------|------|
| `noptr-declarator` | 任何有效的声明符，但如果以 `*`、`&` 或 `&&` 开头，必须用括号包围 |
| `parameter-list` | 可能为空的、以逗号分隔的函数参数列表 |
| `attr` | 属性列表（C++11 起），应用于函数类型而非函数本身 |
| `cv` | `const`/`volatile` 限定，仅允许在非静态成员函数声明中使用 |
| `ref` | 引用限定符（C++11 起），仅允许在非静态成员函数声明中使用 |
| `except` | 异常规范：`noexcept` 规范（C++17 起）或动态异常规范（C++17 前） |
| `trailing` | 尾置返回类型 |

### 参数列表语法

```cpp
attr(可选) decl-specifier-seq declarator                          // 命名参数
attr(可选) this decl-specifier-seq declarator                      // 显式对象参数 (C++23)
attr(可选) decl-specifier-seq declarator = initializer             // 带默认值的参数
attr(可选) decl-specifier-seq abstract-declarator(可选)            // 未命名参数
attr(可选) this decl-specifier-seq abstract-declarator(可选)      // 未命名显式对象参数 (C++23)
attr(可选) decl-specifier-seq abstract-declarator(可选) = initializer // 带默认值的未命名参数
void                                                              // 无参数
```

### 参数说明

| 元素 | 说明 |
|------|------|
| `attr` | 属性列表 |
| `decl-specifier-seq` | 类型说明符序列 |
| `declarator` | 声明符，包含参数名称 |
| `abstract-declarator` | 抽象声明符，不包含参数名称 |
| `initializer` | 默认参数值 |
| `this` | 显式对象参数说明符（C++23 起） |

### 函数定义语法

```cpp
// 常规函数定义
attr(可选) decl-specifier-seq(可选) declarator virt-specifier-seq(可选) function-body

// 带约束的函数定义 (C++20 起)
attr(可选) decl-specifier-seq(可选) declarator requires-clause function-body
```

#### 函数体语法

```cpp
ctor-initializer(可选) compound-statement    // 常规函数体
function-try-block                            // 函数 try 块
= default ;                                   // 显式默认函数 (C++11)
= delete ;                                    // 显式删除函数 (C++11)
= delete ( string-literal ) ;                 // 带原因的删除函数 (C++26)
```

### 返回类型推导

当函数声明的 `decl-specifier-seq` 包含 `auto` 关键字时，可以省略尾置返回类型：

```cpp
auto f() { return 42; }             // 返回类型推导为 int
decltype(auto) g() { return x; }    // 返回类型使用 decltype 规则推导
```

### 显式对象参数 (C++23)

```cpp
struct C {
    void f(this C& self);                    // OK
    template<typename Self>
    void g(this Self&& self);                // OK，支持模板

    void p(this C) const;                    // 错误：不允许 const
    static void q(this C);                   // 错误：不允许 static
    void r(int, this C);                     // 错误：必须是第一个参数
};
```

## 4. 底层原理

### 参数类型调整

函数的参数类型列表（parameter-type-list）通过以下步骤确定：

1. **类型推导**：确定每个参数的类型
2. **类型调整**：数组类型 `T[]` 调整为指针 `T*`，函数类型 `T()` 调整为函数指针 `T(*)()`
3. **移除顶层 cv 限定符**：形成函数类型时移除参数的顶层 `const`/`volatile` 限定符
4. **形成参数类型列表**：转换后的参数类型列表加上省略号或参数包的存在性

```cpp
void f(char*);         // #1
void f(char[]) {}      // 定义 #1，char[] 调整为 char*
void f(const char*) {} // OK，另一个重载
void f(char* const) {} // 错误：重定义 #1
```

### 函数类型确定

函数类型由以下组成：
- 返回类型
- 参数类型列表
- cv 限定符（仅成员函数）
- 引用限定符（仅成员函数，C++11 起）
- 异常规范（C++17 起，noexcept 成为类型的一部分）

```cpp
// f1 的类型是 "返回 void、接受 int、带 noreturn 属性的函数"
void f1(int a) [[noreturn]];

// f2 的类型是 "返回 int、接受 int 指针的 constexpr noexcept 函数"
constexpr auto f2(int[] b) noexcept -> int;

struct X {
    // f3 的类型是 "返回 const int、无参数的 const 成员函数"
    const int f3() const;
};
```

### 函数签名

函数签名（Function Signature）包含：
- 函数名称
- 参数类型列表
- 对于成员函数：所属类、cv 限定符、引用限定符（C++11 起）、尾置 requires 子句（C++20 起）

注意：`noexcept` 规范和属性不影响函数签名（但 C++17 起 `noexcept` 影响函数类型）。

### `__func__` 预定义变量

在函数体内，预定义变量 `__func__` 可用：

```cpp
static const char __func__[] = "function-name";
```

该变量具有块作用域和静态存储期。

## 5. 使用场景

### 函数声明适用场景

| 场景 | 说明 |
|------|------|
| 头文件中声明 API | 分离接口与实现 |
| 前向声明 | 解决循环依赖问题 |
| 类成员函数声明 | 定义类接口 |
| 函数重载 | 提供多种调用方式 |

### 尾置返回类型适用场景

| 场景 | 说明 |
|------|------|
| 返回类型依赖参数名 | 如 `template<class T, class U> auto add(T t, U u) -> decltype(t + u)` |
| 复杂返回类型 | 如返回函数指针类型 |
| Lambda 表达式 | 保持一致性 |

### 返回类型推导适用场景

| 场景 | 说明 |
|------|------|
| 泛型代码 | 返回类型依赖模板参数 |
| 简化代码 | 避免重复类型名称 |
| 转发函数 | 完美转发返回类型 |

### 显式对象参数适用场景 (C++23)

| 场景 | 说明 |
|------|------|
| 简化成员函数模板 | 无需显式指定对象类型 |
| 实现递归 lambda | 通过显式 `this` 参数 |
| 区分值/引用语义 | 清晰表达对象传递方式 |

### 删除函数适用场景

| 场景 | 说明 |
|------|------|
| 禁止拷贝/移动 | 如 `unique_ptr` 的拷贝构造函数 |
| 禁止特定操作 | 如禁止堆分配 (`operator new` 删除) |
| 移除危险重载 | 删除可能导致问题的函数 |

### 默认函数适用场景

| 场景 | 说明 |
|------|------|
| 恢复隐式操作 | 显式请求编译器生成默认实现 |
| 简单类 | 保持 trivial 属性 |
| 跨平台二进制兼容 | 保持稳定的 ABI |

### 注意事项

1. **返回类型限制**：函数返回类型不能是函数类型或数组类型（但可以是指向这些类型的指针或引用）
2. **未命名参数**：函数定义中未使用的参数可以不命名
3. **顶层 cv 限定符**：参数的顶层 cv 限定符在函数类型中被移除，但在函数体内仍然有效
4. **函数体内完整性检查**：参数类型和返回类型必须在函数体内完整

### 常见陷阱

- **最令人困惑的解析（Most Vexing Parse）**：编译器倾向于将歧义解释为函数声明
- **迭代器失效**：返回类型推导时递归调用需要先看到返回语句
- **虚函数与返回类型推导**：虚函数不能使用返回类型推导

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <string>

// 简单函数声明与定义
void greet(const std::string& name = "World") {
    std::cout << "Hello, " << name << "!\n";
}

// 前向声明
int calculate(int a, int b);

// 返回指针的函数（传统语法）
void (*get_callback_old())(const std::string&) {
    return greet;
}

// 返回指针的函数（C++11 尾置返回类型）
auto get_callback_new() -> void(*)(const std::string&) {
    return greet;
}

int main() {
    greet();              // Hello, World!
    greet("C++");         // Hello, C++!

    get_callback_old()("old style");  // Hello, old style!
    get_callback_new()("new style");  // Hello, new style!

    return 0;
}

int calculate(int a, int b) {
    return a + b;
}
```

### 返回类型推导示例

```cpp
#include <iostream>
#include <vector>

// 基本返回类型推导
auto add(int a, int b) {
    return a + b;  // 返回类型推导为 int
}

// 多个 return 语句必须推导出相同类型
auto get_value(bool flag) {
    if (flag) {
        return 42;   // int
    }
    return 0;        // int，OK
}

// decltype(auto) 返回类型推导
int x = 10;
decltype(auto) get_ref() {
    return (x);      // 返回 int&，因为 decltype((x)) 是 int&
}

decltype(auto) get_value_auto() {
    return x;        // 返回 int，因为 decltype(x) 是 int
}

// 泛型返回类型推导
template<typename T, typename U>
auto add_generic(T t, U u) {
    return t + u;     // 返回类型依赖于 T 和 U
}

int main() {
    std::cout << add(1, 2) << "\n";           // 3
    std::cout << get_value(true) << "\n";      // 42
    std::cout << get_ref() << "\n";            // 10

    get_ref() = 100;                           // 修改 x
    std::cout << x << "\n";                    // 100

    std::cout << add_generic(1, 2.5) << "\n";  // 3.5

    return 0;
}
```

### 尾置返回类型示例

```cpp
#include <iostream>
#include <vector>
#include <type_traits>

// 返回类型依赖参数名称
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}

// 复杂返回类型（函数指针）
auto get_function() -> int(*)(int, int) {
    return [](int a, int b) { return a + b; };
}

// 返回数组指针
auto get_array_ptr() -> int(*)[5] {
    static int arr[5] = {1, 2, 3, 4, 5};
    return &arr;
}

int main() {
    auto fn = get_function();
    std::cout << fn(3, 4) << "\n";  // 7

    auto ptr = get_array_ptr();
    std::cout << (*ptr)[0] << "\n";  // 1

    return 0;
}
```

### 删除函数与默认函数示例

```cpp
#include <iostream>
#include <string>

class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
    ~NonCopyable() = default;
};

class TrivialClass {
public:
    TrivialClass() = default;
    TrivialClass(const TrivialClass&) = default;
    TrivialClass(TrivialClass&&) = default;
    TrivialClass& operator=(const TrivialClass&) = default;
    TrivialClass& operator=(TrivialClass&&) = default;
    ~TrivialClass() = default;
};

class NoHeapAllocation {
public:
    void* operator new(std::size_t) = delete("Heap allocation disabled");
    void* operator new[](std::size_t) = delete("Array allocation disabled");
};

int main() {
    NonCopyable a;
    // NonCopyable b = a;  // 错误：拷贝构造函数已删除
    NonCopyable c = std::move(a);  // OK：移动构造

    // NoHeapAllocation* p = new NoHeapAllocation;  // 错误：operator new 已删除

    return 0;
}
```

### 显式对象参数示例 (C++23)

```cpp
#include <iostream>
#include <string>

struct Widget {
    int value;

    // 传统成员函数方式
    void print_value() const {
        std::cout << value << "\n";
    }

    // 显式对象参数方式 (C++23)
    void print_explicit(this const Widget& self) {
        std::cout << self.value << "\n";
    }

    // 支持值语义
    void print_by_value(this Widget self) {
        std::cout << self.value << "\n";
    }

    // 泛型成员函数
    template<typename Self>
    void print_generic(this Self&& self) {
        std::cout << self.value << "\n";
    }
};

int main() {
    Widget w{42};
    w.print_value();       // 传统调用
    w.print_explicit();    // 显式对象参数
    w.print_by_value();    // 按值传递
    w.print_generic();     // 泛型版本

    return 0;
}
```

### 常见错误及修正

#### 错误 1：最令人困惑的解析

```cpp
// 错误：被解析为函数声明
std::string str(std::string());  // 声明了一个返回 string、接受函数指针的函数

// 修正方法 1：使用大括号初始化
std::string str{std::string{}};  // OK：对象声明

// 修正方法 2：使用赋值语法
std::string str = std::string();  // OK：对象声明
```

#### 错误 2：返回类型推导不一致

```cpp
// 错误：多个 return 语句推导出不同类型
auto inconsistent(bool flag) {
    if (flag) {
        return 42;    // int
    } else {
        return 3.14;  // double，错误！
    }
}

// 修正：确保所有 return 语句返回相同类型
auto consistent(bool flag) {
    if (flag) {
        return 42.0;  // double
    } else {
        return 3.14;  // double，OK
    }
}
```

#### 错误 3：删除函数的误用

```cpp
struct Bad {
    // 错误：不能在第一次声明后删除
    Bad();
};

Bad::Bad() = delete;  // 错误：必须在第一次声明时删除

// 修正
struct Good {
    Good() = delete;  // OK：在第一次声明时删除
};
```

#### 错误 4：虚函数使用返回类型推导

```cpp
struct Base {
    // 错误：虚函数不能使用返回类型推导
    virtual auto get_value() { return 0; }
};

// 修正：明确指定返回类型
struct FixedBase {
    virtual int get_value() { return 0; }
};
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| 函数声明与定义分离 | 声明引入名称和类型，定义关联函数体 |
| 参数类型调整 | 数组和函数类型自动调整为指针 |
| 返回类型推导 (C++14) | 使用 `auto` 或 `decltype(auto)` 自动推导返回类型 |
| 尾置返回类型 (C++11) | 适用于返回类型依赖参数名的场景 |
| 删除函数 (C++11) | 使用 `= delete` 禁止特定函数 |
| 默认函数 (C++11) | 使用 `= default` 请求编译器生成默认实现 |
| 显式对象参数 (C++23) | 使用 `this` 关键字声明显式对象参数 |

### 技术对比

| 特性 | C++11 前 | C++11 | C++14 | C++17 | C++20 | C++23 | C++26 |
|------|----------|-------|-------|-------|-------|-------|-------|
| 尾置返回类型 | - | 支持 | 支持 | 支持 | 支持 | 支持 | 支持 |
| 返回类型推导 | - | Lambda | 函数 | 函数 | 函数 | 函数 | 函数 |
| 删除函数 | - | 支持 | 支持 | 支持 | 支持 | 支持 | 带原因 |
| 默认函数 | - | 支持 | 支持 | 支持 | 支持 | 支持 | 支持 |
| noexcept 类型 | - | - | - | 支持 | 支持 | 支持 | 支持 |
| 约束 | - | - | - | - | 支持 | 支持 | 支持 |
| 显式对象参数 | - | - | - | - | - | 支持 | 支持 |

### 学习建议

1. **从基础开始**：先掌握基本的函数声明和定义语法
2. **理解类型调整**：掌握参数类型调整规则对理解函数重载至关重要
3. **善用现代特性**：使用 `= default` 和 `= delete` 表达意图
4. **返回类型推导**：在泛型代码中善用 `auto` 返回类型推导
5. **关注签名规则**：理解什么影响函数签名，什么不影响

### 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/function
- C++ Standard: [dcl.fct]
- Effective C++, Scott Meyers