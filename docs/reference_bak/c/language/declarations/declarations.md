# C语言声明（Declarations）

来源：cppreference.com

## 声明概览

| 声明类型 | 相关内容 |
|----------|----------|
| **基本类型** | 指针、数组、枚举、结构体、联合体 |
| **类型修饰符** | const、volatile、restrict、原子类型 |
| **存储说明符** | typedef、static、extern、_Thread_local等 |
| **现代特性** | constexpr（C23）、属性（C23）、对齐说明符 |

**声明**是C语言构造，用于向程序引入一个或多个标识符，并指定其含义和属性。

声明可出现在任何作用域中。每个声明以分号结尾（类似语句），由两部分（C23前）或三部分（C23起）组成：

---

## 声明语法

### 基本声明结构

| 语法形式 | 说明 |
|----------|------|
| `specifiers-and-qualifiers declarators-and-initializers(optional) ;` | (1) 简单声明 |
| `attr-spec-seq specifiers-and-qualifiers declarators-and-initializers ;` | (2) 带属性声明（C23） |
| `attr-spec-seq ;` | (3) 属性声明（C23） |

### 声明组成部分

#### 1. 类型说明符和限定符（specifiers-and-qualifiers）

空白分隔的列表，顺序任意，包括：

- **类型说明符**：
  - `void`
  - 算术类型名称
  - 原子类型名称（C11）
  - typedef引入的名称
  - `struct`/`union`/`enum`说明符
  - typeof说明符（C23）

- **存储类说明符**（零个或一个）：
  - `typedef`, `constexpr`(C23), `auto`, `register`
  - `static`, `extern`, `_Thread_local`

- **类型限定符**（零个或多个）：
  - `const`, `volatile`, `restrict`(C99), `_Atomic`(C11)

- **函数说明符**（仅声明函数时）：
  - `inline`(C99), `_Noreturn`(C11)

- **对齐说明符**（零个或多个）：
  - `_Alignas`(C11/C23前), `alignas`(C23)

#### 2. 声明符和初始化器（declarators-and-initializers）

逗号分隔的声明符列表（每个声明符提供额外类型信息和/或要声明的标识符）。声明符可伴随初始化器。

枚举、结构体、联合体声明可省略声明符，此时仅引入枚举常量和/或标签。

#### 3. 属性序列（attr-spec-seq）（C23）

可选属性列表，应用于声明实体，或单独形成属性声明。

### 声明示例

```c
int a, *b=NULL; // "int"是类型说明符
                // "a"是声明符
                // "*b"是声明符，NULL是初始化器

const int *f(void); // "int"是类型说明符
                    // "const"是类型限定符
                    // "*f(void)"是声明符

enum COLOR {RED, GREEN, BLUE} c; // "enum COLOR {RED, GREEN, BLUE}"是类型说明符
                                 // "c"是声明符
```

每个声明中引入的标识符类型由类型说明符指定的类型和声明符应用的类型修改组合确定。使用auto说明符时可推断变量类型（C23）。

属性（C23）可出现在说明符和限定符中，此时应用于前面说明符确定的类型。

---

## 声明符详解

每个声明符是以下之一：

### 声明符类型

| 语法形式 | 说明 |
|----------|------|
| `identifier attr-spec-seq(optional)` | (1) 标识符声明符 |
| `(` declarator `)` | (2) 括号声明符 |
| `* attr-spec-seq(optional) qualifiers(optional) declarator` | (3) 指针声明符 |
| `noptr-declarator [static(optional) qualifiers(optional) expression]`<br>`noptr-declarator [qualifiers(optional) *]` | (4) 数组声明符 |
| `noptr-declarator (parameters-or-identifiers)` | (5) 函数声明符 |

### 声明符说明

1. **标识符声明符**：此声明符引入的标识符
2. **括号声明符**：任何声明符可用括号包围；引入指向数组和指向函数的指针时必需
3. **指针声明符**：声明`S * cvr D;`将`D`声明为指向`S`确定类型的cvr限定指针
4. **数组声明符**：声明`S D[N]`将`D`声明为`N`个`S`确定类型对象的数组
5. **函数声明符**：声明`S D(params)`将`D`声明为接受参数`params`并返回`S`的函数

### 声明符工作原理

当声明符引入的标识符出现在与声明符形式相同的表达式中时，它具有类型说明符序列指定的类型。

### 复杂声明符示例

```c
struct C
{
    int member; // "int"是类型说明符
                // "member"是声明符
} obj, *pObj = &obj;
// "struct C { int member; }"是类型说明符
// 声明符"obj"定义struct C类型的对象
// 声明符"*pObj"声明指向C的指针
// 初始化器"= &obj"提供指针初始值

int a = 1, *p = NULL, f(void), (*pf)(double);
// 类型说明符是"int"
// 声明符"a"定义int类型对象，初始化器"=1"
// 声明符"*p"定义指向int的指针，初始化器"=NULL"
// 声明符"f(void)"声明接受void返回int的函数
// 声明符"(*pf)(double)"定义指向函数的指针

int (*(*foo)(double))[3] = NULL;
// 复杂声明符解析：
// 1. "(*(*foo)(double))[3]" - 数组声明符：数组of 3 int
// 2. "*(*foo)(double))" - 指针声明符：指向数组of 3 int的指针
// 3. "(*foo)(double)" - 函数声明符：接受double返回指针的函数
// 4. "(*foo)" - 指针声明符：指向函数的指针
// 5. "foo" - 标识符
// 最终：foo是"指向接受double返回指向数组of 3 int的指针的函数的指针"
```

不是另一声明符一部分的每个声明符末尾都是序列点。

所有情况下，`attr-spec-seq`是可选属性序列（C23）。紧跟标识符后出现时，应用于被声明的对象或函数。

---

## 定义（Definitions）

**定义**是提供所声明标识符所有信息的声明。

### 各类定义

#### 1. 枚举和typedef定义
每个枚举或typedef声明都是定义。

#### 2. 函数定义
包含函数体的声明是函数定义：

```c
int foo(double); // 声明
int foo(double x) { return x; } // 定义
```

#### 3. 对象定义
分配存储的声明（自动或静态，但非extern）是定义：

```c
extern int n; // 声明
int n = 10; // 定义
```

#### 4. 结构体/联合体定义
指定成员列表的声明是定义：

```c
struct X; // 声明
struct X { int n; }; // 定义
```

---

## 重声明（Redeclaration）

同一作用域中，如果同一标识符的另一个声明已出现，声明不能引入该标识符，除非：

### 允许的重声明情况

#### 1. 具有链接的对象声明可重复：
```c
extern int x;
int x = 10; // OK
extern int x; // OK

static int n;
static int n = 10; // OK
static int n; // OK
```

#### 2. 非VLA typedef可重复（只要命名相同类型）：
```c
typedef int int_t;
typedef int int_t; // OK
```

#### 3. 结构体和联合体声明可重复：
```c
struct X;
struct X { int n; };
struct X;
```

这些规则简化了头文件使用。

---

## 扩展知识详解

### 一、声明与定义区别

| 类型 | 声明 | 定义 |
|------|------|------|
| **extern变量** | `extern int x;` | `int x = 10;` |
| **函数** | `int func(int);` | `int func(int x) { return x; }` |
| **结构体** | `struct S;` | `struct S { int n; };` |
| **枚举** | `enum E;` | `enum E { A, B };` |

### 二、复杂声明符解析技巧

使用**右左法则**解析复杂声明：

```c
int (*(*foo)(double))[3];
// 1. foo -> 标识符
// 2. *foo -> 指向...
// 3. (*foo)(double) -> 接受double返回...的函数指针
// 4. *(*foo)(double) -> 指向...的指针
// 5. (*(*foo)(double))[3] -> 3个元素数组的指针
// 结果：指向函数的指针，该函数接受double参数，返回指向3个int数组的指针
```

### 三、现代C特性

#### 1. constexpr（C23）
```c
constexpr int MAX_SIZE = 1000;  // 编译时常量
```

#### 2. 属性（C23）
```c
[[deprecated]] int old_function(void);
[[noreturn]] void fatal_error(void);
```

#### 3. typeof（C23）
```c
int x = 42;
typeof(x) y = x;  // y的类型与x相同
```

### 四、可变修改类型（VM Types）（C99）

如果声明符的任何部分是变长数组（VLA）声明符，则整个声明符的类型称为"可变修改类型"：

```c
void func(int n) {
    int arr[n];        // VLA，可变修改类型
    int (*p)[n] = NULL; // 指向VLA的指针，也是可变修改类型
}
```

限制：
- 只能在块作用域或函数原型作用域声明
- 不能是结构体或联合体成员
- 在goto、switch、longjmp中使用有限制

---

## 重要注意事项

### 一、C89限制

| 限制 | 说明 |
|------|------|
| **块内声明位置** | C89中块作用域内声明必须在块开始，语句前 |
| **隐式函数声明** | C89中返回int的函数可隐式声明 |
| **旧式函数定义** | C89中int类型参数无需声明 |

### 二、其他重要规则

1. **空声明符禁止**：简单声明必须至少有一个声明符或声明至少一个结构体/联合体/枚举标签，或引入至少一个枚举常量

2. **静态断言**（C11）：
```c
static_assert(sizeof(int) >= 4, "int too small");  // 声明但不引入标识符
```

3. **属性声明**（C23）：
```c
[[deprecated]];  // 属性声明，不引入标识符
;                // 单独分号是语句，不是属性声明
```

---

## 实际应用示例

### 一、复杂指针声明

```c
// 函数指针数组
int (*array[10])(int, int);

// 指向函数的指针，该函数返回指向数组的指针
int (*(*func_ptr)(void))[5];

// 指向数组的指针
int (*ptr_to_array)[10];
```

### 二、现代C声明

```c
// C23特性
constexpr int BUFFER_SIZE = 1024;
[[nodiscard]] int process_data(void);
typeof(sizeof(int)) size_var;  // size_t类型

// VLA（C99）
void matrix_multiply(int rows, int cols) {
    int matrix[rows][cols];  // 可变修改类型
}
```

### 三、结构体和联合体

```c
// 不完整声明
struct Node;

// 完整定义
struct Node {
    int data;
    struct Node* next;
};

// 联合体
union Data {
    int i;
    float f;
    char str[20];
};
```

---

## 标准参考文献

### 各版本C标准对声明的定义：

- **C23标准 (ISO/IEC 9899:2024)**
  - 6.7 声明

- **C17标准 (ISO/IEC 9899:2018)**
  - 6.7 声明 (p: 78-105)

- **C11标准 (ISO/IEC 9899:2011)**
  - 6.7 声明 (p: 108-145)

- **C99标准 (ISO/IEC 9899:1999)**
  - 6.7 声明 (p: 97-130)

- **C89/C90标准 (ISO/IEC 9899:1990)**
  - 3.5 声明

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C++声明文档] | C++语言声明对照学习 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=c/language/declarations&oldid=179842>
- 最后修改时间：2025年1月25日 01:45
- 离线版本获取时间：2025年2月9日 16:39

---

✅ 掌握C语言声明机制是编写正确、高效代码的基础。从简单变量声明到复杂的函数指针声明，每个声明符都遵循严格的语法规则。理解声明符的工作原理和解析方法，特别是"右左法则"，能够帮助程序员正确理解和编写复杂的声明。现代C标准（C11/C23）引入的静态断言、属性、constexpr等特性，进一步增强了类型安全和代码表达能力。正确区分声明与定义，以及理解重声明规则，对于大型项目中头文件的正确使用至关重要。