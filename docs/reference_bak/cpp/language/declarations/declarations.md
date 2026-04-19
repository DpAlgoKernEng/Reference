# C++ 声明（Declarations）

来源：cppreference.com

## 声明概览

| 声明类型 | 相关内容 |
|----------|----------|
| **声明语法** | 声明说明符序列、声明符 |
| **说明符** | typedef、inline、virtual、explicit、存储类说明符、类型说明符 |
| **声明符** | 引用、指针、数组、函数声明符 |
| **特殊声明** | 结构化绑定（C++17）、别名声明（C++11）、using声明等 |
| **模板声明** | 类模板、函数模板、显式实例化、显式特化 |
| **其他声明** | 命名空间定义、链接规范、属性声明（C++11） |

**声明**是将名称引入（或重新引入）C++程序的方式。并非所有声明都实际声明内容，每种实体的声明方式不同。**定义**是足以使用该名称标识实体的声明。

---

## 声明类型

声明是以下之一：

### 一、主声明类型

1. **函数定义**
2. **模板声明**（包括部分模板特化）
3. **显式模板实例化**
4. **显式模板特化**
5. **命名空间定义**
6. **链接规范**

### 二、特殊声明（C++11起）

7. **属性声明** (`attr ;`)

### 三、其他声明类型

8. **空声明** (`;`)
9. **无声明说明符序列的函数声明**：
   ```cpp
   attr(optional) declarator ;
   ```
   此声明必须声明构造函数、析构函数或用户定义类型转换函数。

10. **块声明**（可在块内出现的声明）：
    - `asm` 声明
    - 类型别名声明（C++11）
    - 命名空间别名定义
    - using声明
    - using指令
    - using-enum声明（C++20）
    - `static_assert` 声明（C++11）
    - 不透明枚举声明（C++11）
    - 简单声明

---

## 简单声明（Simple Declaration）

简单声明是引入、创建并可选初始化一个或多个标识符（通常是变量）的语句。

### 语法形式

| 语法 | 说明 |
|------|------|
| `decl-specifier-seq init-declarator-list(optional) ;` | (1) 基本简单声明 |
| `attr decl-specifier-seq init-declarator-list ;` | (2) 带属性的简单声明（C++11） |

### 组成部分

| 组成 | 说明 |
|------|------|
| `attr` | （C++11）任意数量属性序列 |
| `decl-specifier-seq` | 说明符序列 |
| `init-declarator-list` | 逗号分隔的带可选初始化器的声明符列表 |

> 💡 **结构化绑定声明**也是简单声明（C++17）

---

## 说明符详解（Specifiers）

**声明说明符序列**（decl-specifier-seq）是以下空白分隔说明符的序列，顺序任意：

### 一、特殊说明符

| 说明符 | 用途 | 限制 |
|--------|------|------|
| `typedef` | 定义类型别名 | 整个声明为typedef声明 |
| `inline` | 内联函数/变量 | C++17起允许用于变量声明 |
| `virtual` | 虚函数 | 仅用于函数声明 |
| `explicit` | 显式构造函数/转换函数 | 仅用于函数声明 |
| `friend` | 友元声明 | 用于类和函数声明 |
| `constexpr` | 常量表达式 | C++11，用于变量定义、函数声明等 |
| `consteval` | 立即函数 | C++20，仅用于函数声明 |
| `constinit` | 常量初始化 | C++20，用于静态/线程局部变量 |

> ⚠️ `constexpr`、`consteval`、`constinit`中最多只能出现一个。

### 二、存储类说明符

| 说明符 | 说明 |
|--------|------|
| `static` | 静态存储期 |
| `thread_local` | 线程局部存储（C++11） |
| `extern` | 外部链接 |
| `mutable` | 可变成员 |
| `register` | 寄存器建议（C++17前） |

> 💡 `thread_local`可与`extern`或`static`一起使用（C++11）。

### 三、类型说明符序列（type-specifier-seq）

命名类型的说明符序列，每个声明实体的类型是此类型，可由声明符修改。

#### 类型说明符包括：

1. **类说明符**（class/struct specifier）
2. **枚举说明符**（enum specifier）
3. **简单类型说明符**：
   - 基本类型：`char`, `char8_t`(C++20), `char16_t`(C++11), `char32_t`(C++11), `wchar_t`, `bool`, `short`, `int`, `long`, `signed`, `unsigned`, `float`, `double`, `void`
   - `auto`(C++11)
   - `decltype`(C++11)
   - `pack indexing specifier`(C++26)
   - 先前声明的类名、枚举名、typedef名或类型别名（可限定）
   - 带模板参数的模板名（可限定，可使用模板消除符）

4. **详述类型说明符**（elaborated type specifier）
5. **typename说明符**
6. **cv限定符**（const/volatile）

#### 类型说明符组合规则：

- `const`可与除自身外的任何类型说明符组合
- `volatile`可与除自身外的任何类型说明符组合
- `signed`/`unsigned`可与`char`, `short`, `int`, `long`组合
- `short`/`long`可与`int`组合
- `long`可与`double`组合
- `long`可与`long`组合（C++11）

属性可出现在decl-specifier-seq中，此时应用于前面说明符确定的类型。

重复任何说明符（如`const static const`）是错误，但`long`允许出现两次（C++11）。

---

## 声明符详解（Declarators）

`init-declarator-list`是逗号分隔的一个或多个初始化声明符序列：

### 初始化声明符语法

| 语法 | 说明 |
|------|------|
| `declarator initializer(optional)` | (1) 带初始化器的声明符 |
| `declarator requires-clause` | (2) 带约束子句的声明符（C++20） |

每个初始化声明符序列`S D1, D2, D3;`按`S D1; S D2; S D3;`独立处理。

每个声明符引入恰好一个对象、引用、函数或（typedef声明）类型别名。

### 声明符类型

| 语法 | 说明 |
|------|------|
| `unqualified-id attr(optional)` | (1) 未限定标识符声明符 |
| `qualified-id attr(optional)` | (2) 限定标识符声明符 |
| `... identifier attr(optional)` | (3) 参数包（C++11） |
| `* attr(optional) cv(optional) declarator` | (4) 指针声明符 |
| `nested-name-specifier * attr(optional) cv(optional) declarator` | (5) 成员指针声明符 |
| `& attr(optional) declarator` | (6) 左值引用声明符 |
| `&& attr(optional) declarator` | (7) 右值引用声明符（C++11） |
| `noptr-declarator [constexpr(optional)] attr(optional)` | (8) 数组声明符 |
| `noptr-declarator (parameter-list) cv(optional) ref(optional) except(optional) attr(optional)` | (9) 函数声明符 |

### 详细说明

1. **未限定标识符声明符**：声明的名称
2. **限定标识符声明符**：使用限定标识符定义或重新声明先前声明的命名空间成员或类成员
3. **参数包**：仅出现在参数声明中
4. **指针声明符**：`S * D;`将`D`声明为指向`S`类型（由decl-specifier-seq确定）的指针
5. **成员指针声明符**：`S C::* D;`将`D`声明为指向`C`成员的指针
6. **左值引用声明符**：`S & D;`将`D`声明为指向`S`类型的左值引用
7. **右值引用声明符**：`S && D;`将`D`声明为指向`S`类型的右值引用
8. **数组声明符**：`noptr-declarator`为任何有效声明符，但若以`*`, `&`, `&&`开始需用括号包围
9. **函数声明符**：可有尾置返回类型（C++11）

属性序列（C++11）可出现在任何位置，紧跟标识符后时应用于被声明对象。

cv限定符序列中每个限定符最多出现一次。

---

## 扩展知识详解

### 一、声明解析技巧

使用**螺旋法则**解析复杂声明：

```cpp
int (*(*foo)(double))[3];
// 解析过程：
// 1. foo -> 标识符
// 2. *foo -> 指向...
// 3. (*foo)(double) -> 接受double返回...的函数指针
// 4. *(*foo)(double) -> 指向...的指针
// 5. (*(*foo)(double))[3] -> 3元素数组的指针
// 最终：指向函数的指针，该函数接受double参数，返回指向3个int数组的指针
```

### 二、现代C++声明特性

#### 1. 自动类型推导（C++11）
```cpp
auto x = 42;           // int
auto y = 3.14;         // double
decltype(x) z = x;     // int
```

#### 2. 结构化绑定（C++17）
```cpp
std::pair<int, double> p{1, 2.5};
auto [a, b] = p;       // 结构化绑定
```

#### 3. 内联变量（C++17）
```cpp
inline int global_var = 42;  // 内联变量
```

#### 4. 约束声明（C++20）
```cpp
template<typename T>
void func(T x) requires std::integral<T> {
    // 约束函数声明
}
```

### 三、声明与作用域

```cpp
void func() {
    int x = 10;        // 自动变量，执行时初始化
    {
        int x = 20;    // 隐藏外层声明
        // 使用内层x
    }
    // x恢复为10
}  // 自动变量销毁（逆序）
```

### 四、复杂声明示例

```cpp
// 函数指针数组
int (*array[10])(int, int);

// 指向成员函数的指针
int (Class::*ptr)(int) = &Class::method;

// 右值引用（移动语义）
std::vector<int> vec1{1, 2, 3};
std::vector<int> vec2 = std::move(vec1);  // 移动构造

// 完美转发引用
template<typename T>
void wrapper(T&& arg) {  // 万能引用
    func(std::forward<T>(arg));
}
```

---

## 实际应用示例

### 完整示例代码

```cpp
#include <type_traits>

struct S {
    int member;
    // decl-specifier-seq是"int"
    // declarator是"member"
} obj, *pObj(&obj);
// decl-specifier-seq是"struct S { int member; }"
// declarator "obj"声明S类型的对象
// declarator "*pObj"声明指向S的指针，初始化器"(&obj)"初始化它

int i = 1, *p = nullptr, f(), (*pf)(double);
// decl-specifier-seq是"int"
// declarator "i"声明int变量，初始化器"= 1"
// declarator "*p"声明int*变量，初始化器"= nullptr"
// declarator "f()"声明（但不定义）无参返回int的函数
// declarator "(*pf)(double)"声明指向函数的指针

int (*(*var1)(double))[3] = nullptr;
// 复杂声明符解析：
// 1. "(*(*var1)(double))[3]"是数组声明符：数组of 3 elements
// 2. "(*(*var1)(double))"是指针声明符：指向数组of 3 elements的指针
// 3. "(*var1)(double)"是函数声明符：接受"(double)"返回指针的函数
// 4. "(*var1)"是指针声明符：指向函数的指针
// 5. "var1"是标识符
// 最终：var1是"指向接受double返回指向3个int数组的指针的函数的指针"

// C++11替代语法：
auto (*var2)(double) -> int (*)[3] = nullptr;
// 使用尾置返回类型

int main() {
    static_assert(std::is_same_v<decltype(var1), decltype(var2)>);
}
```

---

## 缺陷报告（Defect Reports）

| 报告编号 | 应用于 | 发布行为 | 修正行为 |
|----------|--------|----------|----------|
| CWG 482 | C++98 | 重声明的声明符不能限定 | 允许限定声明符 |
| CWG 569 | C++98 | 单独分号不是有效声明 | 是空声明，无效果 |
| CWG 1830 | C++98 | decl-specifier-seq中函数说明符重复被允许 | 禁止重复 |

---

## 标准参考文献

### 相关C++标准章节：

- C++23标准
  - 9 Declarations
- C++20标准
  - 9 Declarations
- C++17标准
  - 9 Declarations
- C++14标准
  - 9 Declarations
- C++11标准
  - 9 Declarations
- C++98/C++03标准
  - 9 Declarations

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C声明文档] | C语言声明对照学习 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/language/declarations&oldid=169173>
- 最后修改时间：2024年1月24日 23:10
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++声明系统是语言的核心，从基本变量声明到复杂的模板声明，每个语法元素都有其特定用途。掌握声明符的解析方法（螺旋法则、声明镜像使用等）对于理解复杂声明至关重要。现代C++引入的auto类型推导、结构化绑定、约束声明等特性，使声明更加灵活和安全。正确理解各种说明符的作用和组合规则，以及声明与作用域的关系，是编写高质量C++代码的基础。属性声明和内联变量等新特性为大型项目提供了更好的控制和性能优化机会。