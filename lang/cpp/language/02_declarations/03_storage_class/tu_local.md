# 翻译单元局部实体 (Translation-unit-local entities, C++20)

## 1. 概述

**翻译单元局部实体**（Translation-unit-local entities，简称 TU-local 实体）是 C++20 引入的重要概念，用于防止本应局限于单个翻译单元的实体被暴露或使用到其他翻译单元中。

TU-local 实体机制主要服务于 C++20 模块系统，确保：
- 内部链接（internal linkage）的实体不会意外泄露到模块接口之外
- 模块边界清晰，封装性得到保障
- 不同翻译单元之间的命名冲突和 ODR（One Definition Rule）违规问题得以避免

## 2. 来源与演变

### C++20 引入

TU-local 实体概念在 **C++20** 标准中引入，与模块（Modules）特性紧密相关。

### 设计动机

在 C++20 模块出现之前，内部链接的实体通过头文件传播时，其可见性自然受限。但模块系统引入后，出现了一个新问题：

**问题示例**：

```cpp
// 模块单元（无 TU-local 约束）
export module Foo;

import <iostream>;

namespace
{
   class LolWatchThis {        // 内部链接，本不应被导出
       static void say_hello()
       {
           std::cout << "Hello, everyone!\n";
       }
   };
}

export LolWatchThis lolwut() { // LolWatchThis 作为返回类型被暴露！
    return LolWatchThis();
}
```

```cpp
// main.cpp
import Foo;

int main()
{
    auto evil = lolwut();        // 'evil' 的类型是 'LolWatchThis'
    decltype(evil)::say_hello(); // 'LolWatchThis' 的定义不再是内部的！
}
```

上述代码在没有 TU-local 约束的情况下，本应局限于翻译单元内部的类被意外暴露给了模块使用者。

### 解决方案

C++20 通过定义 TU-local 实体和"暴露"（exposure）规则，明确禁止或弃用这类可能导致内部实体泄露的声明。

## 3. 语法与定义

### TU-local 实体的判定规则

一个实体是 **TU-local** 当且仅当满足以下条件之一：

| 规则 | 描述 |
|------|------|
| 规则 1.1 | 类型、函数、变量或模板具有内部链接的名称 |
| 规则 1.2 | 类型、函数、变量或模板没有链接名称，且在 TU-local 实体的定义中声明或由 lambda 表达式引入 |
| 规则 2 | 无名类型，定义在类说明符、函数体或初始化器之外，或由仅声明 TU-local 实体的定义类型说明符引入 |
| 规则 3 | TU-local 模板的特化 |
| 规则 4 | 模板特化，其任意模板实参是 TU-local 的 |
| 规则 5 | 模板特化，其（可能实例化的）声明是一个暴露 |

### TU-local 值或对象的判定规则

一个值或对象是 **TU-local** 当且仅当满足以下条件之一：

| 规则 | 描述 |
|------|------|
| 规则 1 | 是 TU-local 函数，或指向 TU-local 函数/变量的指针 |
| 规则 2 | 是类或数组类型的对象，且其任意子对象或引用类型非静态数据成员所引用的对象/函数是 TU-local 且可在常量表达式中使用 |

### "命名"实体规则

声明 D **命名**（names）实体 E 当且仅当满足以下条件之一：

1. D 包含一个 lambda 表达式，其闭包类型是 E
2. E 不是函数或函数模板，且 D 包含指代 E 的标识表达式、类型说明符、嵌套名称说明符、模板名或概念名
3. E 是函数或函数模板，且 D 包含命名 E 的表达式或引用包含 E 的重载集的标识表达式

### "暴露"定义

声明是一个**暴露**（exposure）当且仅当：
- 它命名了一个 TU-local 实体（忽略特定情况），或
- 它定义了一个初始化为 TU-local 值的 constexpr 变量

**忽略的情况**：
1. 非内联函数或函数模板的函数体（但不包括占位符返回类型的推导返回类型）
2. 变量或变量模板的初始化器（但不包括变量的类型）
3. 类定义中的友元声明
4. 对用常量表达式初始化的、具有内部链接或无链接的非 volatile const 对象或引用的引用（非 odr-use）

## 4. 底层原理

### 链接与可见性

TU-local 实体机制建立在 C++ 链接规则之上：

| 链接类型 | 可见性范围 | 示例 |
|---------|-----------|------|
| 无链接 | 仅当前作用域 | 局部变量、块内定义的类 |
| 内部链接 | 当前翻译单元 | `static` 全局变量、匿名命名空间中的实体 |
| 外部链接 | 整个程序 | 普通函数、非 static 全局变量 |

### 模块系统交互

模块接口单元（module interface unit）中的声明会受到额外约束：

```
模块接口单元
    ├── 导出声明（export）
    │   └── 必须不是暴露（exposure）
    ├── 私有模块片段（private-module-fragment）
    │   └── 可以包含 TU-local 实体
    └── 模块分区（module partition）
        └── 受到暴露约束
```

### 约束检查机制

编译器在以下时机检查 TU-local 约束：

1. **编译时静态检查**：在模块接口单元中检测暴露
2. **链接时检查**：跨翻译单元的 TU-local 实体引用检测
3. **模板实例化时检查**：在模板特化实例化时验证

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 模块内部实现细节 | 使用 `static` 或匿名命名空间封装模块内部实现 |
| 防止 API 污染 | 确保内部辅助函数/类型不会出现在模块公共接口中 |
| 模块封装 | 实现真正的模块封装，防止实现细节泄露 |

### 最佳实践

1. **使用 `static` 或匿名命名空间**：将模块内部实体声明为内部链接
2. **避免暴露内部类型**：不要在导出函数的签名中使用内部类型
3. **谨慎使用模板**：确保模板参数不包含 TU-local 实体
4. **理解暴露规则**：理解什么构成暴露，避免意外违规

### 常见陷阱

1. **返回类型暴露**：内部类型作为返回类型被暴露
2. **模板参数暴露**：TU-local 类型作为模板参数
3. **ADL 暴露**：通过参数依赖查找暴露内部函数
4. **constexpr 暴露**：constexpr 变量初始化引用 TU-local 实体

## 6. 代码示例

### 基础示例：TU-local 实体定义

```cpp
// TU-local 实体示例（具有内部链接）
namespace { // 匿名命名空间中所有名称具有内部链接
    int tul_var = 1;                    // TU-local 变量
    int tul_func() { return 1; }        // TU-local 函数
    struct tul_type { int mem; };       // TU-local (类) 类型
}

template<typename T>
static int tul_func_temp() { return 1; }  // TU-local 模板

// TU-local 模板特化
template<>
static int tul_func_temp<int>() { return 3; } // TU-local 特化

// 模板特化使用 TU-local 模板参数
template <> struct std::hash<tul_type> {      // TU-local 特化
    std::size_t operator()(const tul_type& t) const { return 4u; }
};
```

### 基础示例：TU-local 值和对象

```cpp
static int tul_var = 1;             // TU-local 变量
static int tul_func() { return 1; } // TU-local 函数

int* tul_var_ptr = &tul_var;        // TU-local: 指向 TU-local 变量的指针
int (* tul_func_ptr)() = &tul_func; // TU-local: 指向 TU-local 函数的指针

constexpr static int tul_const = 1; // 可在常量表达式中使用的 TU-local 变量
int tul_arr[] = { tul_const };      // TU-local: constexpr TU-local 对象的数组
struct tul_class { int mem; };
tul_class tul_obj{tul_const};       // TU-local: 包含 constexpr TU-local 成员
```

### 高级示例：命名实体规则

```cpp
// lambda 命名
auto x = [] {}; // 命名 decltype(x)

// 非函数（模板）命名
int y1 = 1;                      // 命名 y1（标识表达式）
struct y2 { int mem; };
y2 y2_obj{1};                    // 命名 y2（类型说明符）
struct y3 { int mem_func(); };
int y3::mem_func() { return 0; } // 命名 y3（嵌套名称说明符）
template<typename T> int y4 = 1;
int var = y4<y2>;                // 命名 y4（模板名）
template<typename T> concept y5 = true;
template<typename T> void func(T&&) requires y5<T>; // 命名 y5（概念名）

// 函数（模板）命名
int z1(int arg)    { std::cout << "no overload"; return 0; }
int z2(int arg)    { std::cout << "overload 1";  return 1; }
int z2(double arg) { std::cout << "overload 2";  return 2; }

int val1 = z1(0); // 命名 z1
int val2 = z2(0); // 命名 z2（int z2(int)）
```

### 高级示例：模块接口单元约束

**翻译单元 #1（模块接口）**：

```cpp
export module A;
static void f() {}
inline void it() { f(); }         // 错误：是 f 的暴露
static inline void its() { f(); } // OK
template<int> void g() { its(); } // OK
template void g<0>();

decltype(f) *fp;                             // 错误：f（虽然不是其类型）是 TU-local
auto &fr = f;                                // OK
constexpr auto &fr2 = fr;                    // 错误：是 f 的暴露
constexpr static auto fp2 = fr;              // OK
struct S { void (&ref)(); } s{f};            // OK：值是 TU-local
constexpr extern struct W { S &s; } wrap{s}; // OK：值不是 TU-local

static auto x = []{ f(); }; // OK
auto x2 = x;                // 错误：闭包类型是 TU-local
int y = ([]{ f(); }(), 0);  // 错误：闭包类型不是 TU-local
int y2 = (x, 0);            // OK

namespace N
{
    struct A {};
    void adl(A);
    static void adl(int);
}
void adl(double);

inline void h(auto x) { adl(x); } // OK，但特化可能是暴露
```

**翻译单元 #2（模块实现）**：

```cpp
module A;
void other()
{
    g<0>();                  // OK：特化已显式实例化
    g<1>();                  // 错误：实例化使用 TU-local 的 its
    h(N::A{});               // 错误：重载集包含 TU-local 的 N::adl(int)
    h(0);                    // OK：调用 adl(double)
    adl(N::A{});             // OK；N::adl(int) 未找到，调用 N::adl(N::A)
    fr();                    // OK：调用 f
    constexpr auto ptr = fr; // 错误：fr 在此处不可用于常量表达式
}
```

### 常见错误及修正

#### 错误 1：导出内部类型

```cpp
// 错误示例
export module M;
namespace {
    struct InternalType { int x; }; // 内部链接类型
}
export InternalType make(); // 错误：暴露 TU-local 类型

// 修正：不导出使用内部类型的函数，或使用外部链接类型
export module M;
struct ExportedType { int x; }; // 外部链接类型
export ExportedType make(); // OK
```

#### 错误 2：constexpr 暴露

```cpp
// 错误示例
export module M;
static int internal_value = 42;
constexpr int& ref = internal_value; // 错误：constexpr 变量暴露 TU-local 实体

// 修正：不使用 constexpr，或使用外部链接变量
export module M;
namespace {
    int internal_value = 42;
}
int& ref = internal_value; // OK：非 constexpr
```

## 7. 总结

### 核心要点

1. **TU-local 实体**：具有内部链接或在 TU-local 上下文中定义的实体
2. **暴露规则**：在模块接口单元中，暴露 TU-local 实体的声明是非法的
3. **设计目的**：确保模块封装性，防止实现细节泄露

### TU-local 判定速查表

| 情况 | 是否 TU-local |
|------|--------------|
| `static` 全局变量/函数 | 是 |
| 匿名命名空间中的实体 | 是 |
| TU-local 模板的特化 | 是 |
| 使用 TU-local 参数的模板特化 | 是 |
| lambda 表达式的闭包类型（在 TU-local 上下文中） | 是 |
| 指向 TU-local 函数的指针 | 是（值） |

### 学习建议

1. **理解链接基础**：先掌握 C++ 链接规则（内部/外部/无链接）
2. **熟悉模块系统**：TU-local 规则与模块紧密相关
3. **实践验证**：使用编译器诊断暴露错误
4. **阅读标准**：参考 C++20 标准 [basic.link] 和 [module.interface] 章节

### 技术对比

| 特性 | 传统头文件 | C++20 模块 |
|------|----------|-----------|
| 内部链接实体传播 | 仅限于包含头文件的翻译单元 | 需要显式导出 |
| 实现细节隐藏 | 通过 `static`/匿名命名空间 | 通过 TU-local 规则强制 |
| ODR 违规检测 | 链接时 | 编译时 |
| 封装保障 | 弱（可能通过模板参数泄露） | 强（TU-local 规则） |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/tu_local
- C++20 标准: [basic.link], [module.interface]
- "Understanding C++ Modules: Part 2" - 用于理解 TU-local 设计动机