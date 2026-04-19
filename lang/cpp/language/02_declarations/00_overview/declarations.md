# C++ 声明 (Declarations)

## 1. 概述 (Overview)

**声明 (Declarations)** 是将名称引入（或重新引入）C++ 程序的方式。并非所有声明都会实际声明内容，每种实体的声明方式各不相同。**定义 (Definitions)** 是足以使用该名称所标识实体的声明。

声明是 C++ 程序的基础构建块，它告诉编译器名称的存在及其类型信息。理解声明的语法规则对于正确编写 C++ 程序至关重要。

### 声明的分类

C++ 中的声明可以是以下之一：

- 函数定义 (Function definition)
- 模板声明 (Template declaration)，包括部分模板特化 (Partial template specialization)
- 显式模板实例化 (Explicit template instantiation)
- 显式模板特化 (Explicit template specialization)
- 命名空间定义 (Namespace definition)
- 链接说明 (Linkage specification)
- 属性声明 (Attribute declaration) — C++11 起，语法为 `attr ;`
- 空声明 (`;`)
- 无 decl-specifier-seq 的函数声明
- 块声明 (block-declaration)

### 无 decl-specifier-seq 的函数声明

这是一种特殊形式的函数声明，语法为：

```cpp
attr(可选) declarator ;
```

| 参数 | 说明 |
|------|------|
| `attr` | (C++11 起) 任意数量的属性序列 |
| `declarator` | 函数声明符 |

**限制：** 此声明必须声明构造函数、析构函数或用户定义类型转换函数，且只能作为模板声明、显式特化或显式实例化的一部分。

### 块声明 (Block Declaration)

块声明是可以出现在块内的声明，包括：

- asm 声明
- 类型别名声明 (type alias declaration) — C++11 起
- 命名空间别名定义 (namespace alias definition)
- using 声明 (using-declaration)
- using 指令 (using directive)
- using-enum 声明 — C++20 起
- `static_assert` 声明 — C++11 起
- 不透明枚举声明 (opaque enum declaration) — C++11 起
- 简单声明

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C++ 的声明语法继承自 C 语言，但在此基础上进行了显著的扩展。声明语法的设计目标是提供一种统一的方式来描述程序中的各种实体。"声明镜像使用"原则使声明语法与使用语法相对应，便于理解。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++98 | 基础声明语法确立，包含存储类说明符、类型说明符、声明符等核心概念 |
| C++11 | 引入 `auto` 类型推导、`decltype` 说明符、属性声明、`constexpr` 说明符、`thread_local` 存储类说明符、参数包声明语法 |
| C++17 | 允许 `inline` 用于变量声明、引入结构化绑定声明（也是一种简单声明）、类模板参数推导 |
| C++20 | 引入 `consteval` 说明符、`constinit` 说明符、`using-enum-declaration`、requires-clause 用于函数声明约束 |
| C++26 | 引入包索引说明符 (pack indexing specifier) |

### 缺陷报告

| DR | 应用版本 | 原有行为 | 修正行为 |
|-----|----------|----------|----------|
| CWG 482 | C++98 | 重声明的声明符不能是限定符形式 | 允许限定声明符 |
| CWG 569 | C++98 | 单独的分号不是有效的声明 | 作为空声明，无效果 |
| CWG 1830 | C++98 | decl-specifier-seq 中允许重复函数说明符 | 禁止重复 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 简单声明 (Simple Declaration)

简单声明是引入、创建并可选地初始化一个或多个标识符（通常是变量）的语句。

**基本语法：**

```cpp
decl-specifier-seq init-declarator-list(可选) ;    // (1)
attr decl-specifier-seq init-declarator-list;       // (2) C++11 起
```

**参数说明：**

| 参数 | 说明 |
|------|------|
| `attr` | (C++11 起) 任意数量的属性序列 |
| `decl-specifier-seq` | 说明符序列（见下文） |
| `init-declarator-list` | 带可选初始化器的声明符逗号分隔列表。声明命名类/结构体/联合体或命名枚举时可省略 |

**注意：** C++17 起，结构化绑定声明也是一种简单声明。

### 说明符 (Specifiers)

**声明说明符 (decl-specifier-seq)** 是以下以空白分隔的说明符序列，顺序不限：

#### typedef 说明符
如果存在 `typedef` 说明符，整个声明是 typedef 声明，每个声明符引入新的类型名称而非对象或函数。

#### 函数说明符
- `inline` — 仅允许在函数声明中使用；C++17 起也允许用于变量声明
- `virtual` — 仅允许在函数声明中使用
- `explicit` — 仅允许在函数声明中使用

**注意：** 函数说明符在 decl-specifier-seq 中不允许重复（CWG 1830）。

#### 其他说明符
- `friend` — 允许在类和函数声明中使用
- `constexpr` (C++11 起) — 仅允许在变量定义、函数和函数模板声明、字面量类型静态数据成员声明中使用
- `consteval` (C++20 起) — 仅允许在函数和函数模板声明中使用
- `constinit` (C++20 起) — 仅允许在具有静态或线程存储期的变量声明中使用

**规则：** `constexpr`、`consteval`、`constinit` 三者中最多只能有一个出现在 decl-specifier-seq 中。

#### 存储类说明符
`register` (C++17 前)、`static`、`thread_local` (C++11 起)、`extern`、`mutable`

**规则：** 只能有一个存储类说明符，但 `thread_local` 可与 `extern` 或 `static` 同时出现（C++11 起）。

#### 类型说明符 (type-specifier-seq)

类型说明符序列命名一个类型，声明引入的每个实体的类型由此类型（可能被声明符修改）决定。此说明符序列也用于 type-id。

**包含的说明符（顺序不限）：**

- 类说明符 (class specifier)
- 枚举说明符 (enum specifier)
- 简单类型说明符 (simple type specifier)：
  - 字符类型：`char`, `char8_t` (C++20), `char16_t` (C++11), `char32_t` (C++11), `wchar_t`
  - 整型：`bool`, `short`, `int`, `long`, `signed`, `unsigned`
  - 浮点类型：`float`, `double`
  - 空类型：`void`
  - `auto` — C++11 起
  - `decltype` 说明符 — C++11 起
  - 包索引说明符 — C++26 起
  - 前面声明的类名（可限定）
  - 前面声明的枚举名（可限定）
  - 前面声明的 typedef-name 或类型别名（C++11 起）（可限定）
  - 带模板参数的模板名（可限定，可使用 template 消歧符）
  - 不带模板参数的模板名（可限定）— C++17 起，参见类模板参数推导
- 详细类型说明符 (elaborated type specifier)
- typename 说明符
- cv 限定符

**类型组合规则：**

| 组合 | 说明 |
|------|------|
| `const` | 可与除自身外的任何类型说明符组合 |
| `volatile` | 可与除自身外的任何类型说明符组合 |
| `signed`/`unsigned` | 可与 `char`, `long`, `short`, `int` 组合 |
| `short`/`long` | 可与 `int` 组合 |
| `long` | 可与 `double` 组合 |
| `long long` | C++11 起，`long` 可出现两次 |

**属性位置：** 属性可以出现在 decl-specifier-seq 中，此时应用于前面说明符确定的类型。

**重复规则：** 除 `long` 可出现两次外（C++11 起），decl-specifier-seq 中任何说明符的重复都是错误。例如 `const static const` 或 `virtual inline virtual` 都是错误的。

### 声明符 (Declarators)

init-declarator-list 是一个或多个 init-declarator 的逗号分隔序列。

**init-declarator 语法：**

```cpp
declarator initializer(可选)           // (1)
declarator requires-clause              // (2) C++20 起
```

| 参数 | 说明 |
|------|------|
| `declarator` | 声明符 |
| `initializer` | 可选的初始化器（引用或 const 对象必须初始化）。详见初始化 |
| `requires-clause` | requires-clause，为函数声明添加约束 |

**多声明符处理规则：** init-declarator 序列 `S D1, D2, D3;` 中的每个 init-declarator 都按独立声明处理：`S D1; S D2; S D3;`。

**声明符语义：** 每个声明符恰好引入一个对象、引用、函数或（对于 typedef 声明）类型别名，其类型由 decl-specifier-seq 提供，并可能被声明符中的运算符（如 `&` 引用、`[]` 数组、`()` 函数）修改。这些运算符可以递归应用。

**声明符语法：**

| 语法 | 版本 | 说明 |
|------|------|------|
| `unqualified-id attr(可选)` | | 声明的名称 |
| `qualified-id attr(可选)` | | 使用限定标识符定义或重声明命名空间成员或类成员 |
| `... identifier attr(可选)` | C++11 起 | 参数包，仅出现在参数声明中 |
| `* attr(可选) cv(可选) declarator` | | 指针声明符：`S * D;` 声明 `D` 为指向 `S` 类型对象的指针 |
| `nested-name-specifier * attr(可选) cv(可选) declarator` | | 成员指针声明符：`S C::* D;` 声明 `D` 为指向 `C` 类 `S` 类型成员的指针 |
| `& attr(可选) declarator` | | 左值引用声明符：`S & D;` 声明 `D` 为 `S` 类型的左值引用 |
| `&& attr(可选) declarator` | C++11 起 | 右值引用声明符：`S && D;` 声明 `D` 为 `S` 类型的右值引用 |
| `noptr-declarator [ constexpr(可选) ] attr(可选)` | | 数组声明符 |
| `noptr-declarator ( parameter-list ) cv(可选) ref(可选) except(可选) attr(可选)` | | 函数声明符，可带可选的尾置返回类型（C++11 起） |

**说明：**
- `noptr-declarator` 是任意有效声明符，但如果以 `*`、`&` 或 `&&` 开头，必须用括号括起
- `cv` 是 `const` 和 `volatile` 限定符序列，每个限定符最多出现一次
- `nested-name-specifier` 是名称和作用域解析运算符 `::` 的序列
- C++11 起，`attr` 是可选的属性序列。紧跟标识符后出现时，应用于被声明的对象

---

## 4. 底层原理 (Underlying Principles)

### 声明解析机制

C++ 编译器解析声明时遵循"声明镜像使用"原则（declaration mirrors use）。声明符的语法结构反映了如何使用被声明的实体。

**解析顺序（从内到外）：**
1. 标识符本身
2. 标识符右侧的数组 `[]` 和函数 `()` 修饰符
3. 标识符左侧的指针 `*` 和引用 `&` 修饰符

**螺旋法则 (Spiral Rule)：**
一种流行的声明解析方法，从变量名开始，按螺旋方式向外阅读声明。

### 块声明与作用域隐藏

当块声明出现在块内，且声明引入的标识符之前在外层块中已声明，则外层声明在块的剩余部分被隐藏。

### 自动存储期变量

如果声明引入了具有自动存储期的变量，则在其声明语句执行时初始化。块中声明的所有自动变量在退出块时（无论通过异常、goto 还是正常结束）按初始化相反的顺序销毁。

### 多声明符处理

init-declarator 序列 `S D1, D2, D3;` 中的每个 init-declarator 都按独立声明处理：
```cpp
S D1; S D2; S D3;
```

### 声明名隐藏规则

变量/函数声明会隐藏同名类（但不会隐藏同名 typedef）。详细信息参见名称隐藏相关文档。

---

## 5. 使用场景 (Use Cases)

### 适用场景

1. **变量声明** — 引入具有特定类型的命名对象
2. **函数声明** — 声明函数签名（可与定义分离）
3. **类型别名** — 使用 `typedef` 或 `using` 创建类型别名
4. **模板声明** — 声明模板函数或类
5. **命名空间管理** — 声明和使用命名空间成员
6. **构造/析构/转换函数** — 使用无 decl-specifier-seq 的函数声明形式

### 最佳实践

1. **优先使用 `auto`** (C++11 起) — 让编译器自动推导类型，减少冗余
2. **合理使用 `const`/`constexpr`** — 提高代码安全性和可优化性
3. **使用尾置返回类型** — 简化复杂函数指针声明（C++11 起）
4. **避免复杂声明符** — 使用 `using` 或 `typedef` 简化复杂类型
5. **每个声明一行** — 提高可读性，避免初始化顺序混淆

### 常见陷阱

1. **声明符优先级混淆** — `int* p[3]` 与 `int (*p)[3]` 含义不同
2. **重复说明符** — `const static const` 是错误写法
3. **存储类说明符冲突** — 同一声明中不能有多个存储类说明符（`thread_local` 例外）
4. **作用域隐藏** — 内层块的声明会隐藏外层同名的声明
5. **const 对象未初始化** — const 对象必须初始化
6. **类型说明符限制** — 类型说明符只能有一个（有例外组合）

---

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <type_traits>

// 简单变量声明
int x = 10;
const double pi = 3.14159;

// 指针和引用声明
int* ptr = &x;
int& ref = x;

// 数组声明
int arr[5] = {1, 2, 3, 4, 5};

// 函数声明
int add(int a, int b);

// typedef 声明
typedef int Integer;
Integer value = 100;  // 等价于 int value = 100;
```

### 复杂声明符解析

```cpp
#include <type_traits>

struct S
{
    int member;
    // decl-specifier-seq 是 "int"
    // declarator 是 "member"
} obj, *pObj(&obj);
// decl-specifier-seq 是 "struct S { int member; }"
// declarator "obj" 声明一个 S 类型的对象
// declarator "*pObj" 声明一个指向 S 的指针
//     初始化器 "(&obj)" 对其初始化

int i = 1, *p = nullptr, f(), (*pf)(double);
// decl-specifier-seq 是 "int"
// declarator "i" 声明 int 类型变量
// declarator "*p" 声明 int* 类型变量
// declarator "f()" 声明（但未定义）返回 int 的无参函数
// declarator "(*pf)(double)" 声明函数指针，接受 double 返回 int

int (*(*var1)(double))[3] = nullptr;
// 1. "(*(*var1)(double))[3]" 是数组声明符：
//    声明类型是："(*(*var1)(double)" 的 3 元素数组
// 2. "(*(*var1)(double))" 是指针声明符：
//    声明类型是："(*var1)(double)" 指向 3 元素数组的指针
// 3. "(*var1)(double)" 是函数声明符：
//    声明类型是："(*var1)" 接受 double 参数，返回指向 3 元素数组的指针的函数
// 4. "(*var1)" 是指针声明符：
//    声明类型是："var1" 指向函数的指针
// 5. "var1" 是标识符
// 最终：var1 是"指向接受 double 参数并返回指向 3 个 int 元素数组的指针的函数"的指针

// C++11 尾置返回类型替代语法：
auto (*var2)(double) -> int (*)[3] = nullptr;
// decl-specifier-seq 是 "auto"
// declarator 是 "(*var2)(double) -> int (*)[3]"

int main()
{
    static_assert(std::is_same_v<decltype(var1), decltype(var2)>);
}
```

### C++11/17/20 新特性用法

```cpp
// C++11: auto 类型推导
auto x = 42;              // x 是 int
auto y = 3.14;            // y 是 double
auto& ref = x;            // ref 是 int&
auto* ptr = &x;           // ptr 是 int*

// C++11: decltype 说明符
decltype(x) z = 100;      // z 是 int

// C++11: constexpr 说明符
constexpr int square(int n) {
    return n * n;
}
constexpr int sq = square(5);  // 编译期计算

// C++11: 属性声明
[[noreturn]] void fatal_error() {
    throw std::runtime_error("fatal");
}

// C++11: 参数包声明
template<typename... Args>
void func(Args... args);

// C++17: inline 变量
struct MyClass {
    static inline int count = 0;  // inline 静态数据成员
};

// C++17: 结构化绑定
std::pair<int, double> get_values() { return {1, 2.0}; }
auto [a, b] = get_values();  // a 是 int, b 是 double

// C++20: consteval 说明符
consteval int compile_time_only(int n) {
    return n * 2;
}
// int x = compile_time_only(5);  // 正确：编译期求值
// int y = compile_time_only(x);  // 错误：x 不是编译期常量

// C++20: constinit 说明符
constinit int global = 42;  // 强制编译期初始化，避免静态初始化顺序问题

// C++20: requires-clause 用于函数声明
template<typename T>
  requires std::integral<T>
T add(T a, T b) {
    return a + b;
}
```

### 常见错误及修正

```cpp
// 错误 1：重复的类型说明符
// const static const int x = 10;  // 错误：const 重复
const static int x = 10;           // 正确

// 错误 2：指针数组与数组指针混淆
int* arr1[3];      // 正确：包含 3 个 int* 元素的数组
int (*arr2)[3];    // 正确：指向包含 3 个 int 元素的数组的指针
// int arr2[3]*;   // 错误：语法错误

// 错误 3：多个存储类说明符
// static extern int y;  // 错误：不能同时使用 static 和 extern
static int y;           // 正确

// 错误 4：函数指针声明语法
int* f1(int);           // 正确：返回 int* 的函数
int (*f2)(int);         // 正确：函数指针，返回 int
// int* f3(int)(int);   // 错误：语法错误

// 错误 5：使用未初始化的 const 对象
// const int ci;        // 错误：const 对象必须初始化
const int ci2 = 10;     // 正确

// 错误 6：重复函数说明符
// virtual inline virtual void f();  // 错误：virtual 重复
virtual inline void f();             // 正确

// 错误 7：constexpr/consteval/constinit 冲突
// constexpr consteval int g() { return 1; }  // 错误：不能同时使用
constexpr int g() { return 1; }               // 正确
```

### 无 decl-specifier-seq 的函数声明示例

```cpp
template<typename T>
class MyClass {
public:
    // 构造函数声明（模板实例化的一部分）
    MyClass();                           // 普通构造函数
    MyClass(const MyClass&);             // 拷贝构造函数
    MyClass(MyClass&&);                  // 移动构造函数
    ~MyClass();                          // 析构函数
    operator int() const;                // 类型转换函数
};

// 显式实例化
template class MyClass<int>;

// 显式特化
template<>
class MyClass<double> {
    MyClass();
    ~MyClass();
    operator double() const;
};
```

---

## 7. 总结 (Summary)

### 核心要点

1. **声明 vs 定义** — 声明引入名称，定义提供实体的完整描述
2. **声明结构** — 由说明符序列 (decl-specifier-seq) 和声明符列表 (init-declarator-list) 组成
3. **说明符类型** — 包括 typedef、函数说明符、存储类说明符、类型说明符等
4. **声明符语法** — 支持指针、引用、数组、函数等多种组合
5. **声明镜像使用** — 声明的语法结构反映了如何使用被声明的实体

### 技术对比

| 特性 | C++98 | C++11 | C++17 | C++20 |
|------|-------|-------|-------|-------|
| auto 类型推导 | 无 | 有 | 有 | 有 |
| decltype | 无 | 有 | 有 | 有 |
| constexpr | 无 | 有 | 有 | 有 |
| inline 变量 | 无 | 无 | 有 | 有 |
| consteval | 无 | 无 | 无 | 有 |
| constinit | 无 | 无 | 无 | 有 |
| 结构化绑定 | 无 | 无 | 有 | 有 |
| 参数包声明 | 无 | 有 | 有 | 有 |
| requires-clause | 无 | 无 | 无 | 有 |

### 学习建议

1. **从简单开始** — 先掌握基本变量、指针、引用的声明
2. **理解声明符优先级** — 掌握数组 `[]` 和函数 `()` 优先级高于指针 `*` 的规则
3. **善用工具** — 使用 [cdecl.org](https://cdecl.org) 解析复杂声明
4. **学习现代 C++** — 掌握 `auto`、`decltype` 等现代特性简化声明
5. **实践复杂声明** — 通过阅读和编写复杂声明加深理解
6. **注意版本差异** — 了解各 C++ 版本引入的新特性

### 相关参考

- [C 文档 - 声明](https://en.cppreference.com/w/c/language/declarations)
- [C++ 初始化](https://en.cppreference.com/w/cpp/language/initialization)
- [C++ auto](https://en.cppreference.com/w/cpp/language/auto)
- [C++ decltype](https://en.cppreference.com/w/cpp/language/decltype)