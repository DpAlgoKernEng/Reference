# `typedef` 说明符

## 1. 概述

`typedef` 是 C++ 中的类型别名说明符（type alias specifier），用于创建一个可以在任何地方代替（可能是复杂的）类型名称的别名。

`typedef` 说明符在声明中使用时，指定该声明是一个 **typedef 声明**（typedef declaration），而不是变量或函数声明。通过 `typedef` 声明的标识符成为 **typedef 名称**（typedef name），它是其声明类型的同义词，而非新类型的定义。

typedef 名称是现有类型的别名，不能用于改变已有类型名称的含义。typedef 名称仅在其可见的作用域内有效：不同函数或类声明可以定义同名但含义不同的 typedef 名称。

## 2. 来源与演变

### 历史背景

`typedef` 继承自 C 语言，其设计动机包括：

1. **简化复杂类型声明**：如函数指针、数组指针等复杂类型
2. **提高代码可移植性**：通过 typedef 定义平台相关类型
3. **增强代码可读性**：为类型赋予有意义的名称

### C++ 版本变化

| 版本 | 变化 |
|------|------|
| C++98 | 从 C 继承 `typedef` 语义 |
| C++11 | 引入类型别名（type alias）和别名模板（alias template），提供更清晰的语法 |
| C++20 | 对"用于链接目的的 typedef 名称"增加了 C 兼容性限制 |

### C++11 类型别名

从 C++11 开始，类型别名（type alias）提供了与 `typedef` 相同功能但更清晰的语法：

```cpp
// typedef 语法
typedef unsigned long ulong;

// C++11 类型别名语法
using ulong = unsigned long;
```

类型别名还支持模板化，这是 `typedef` 无法实现的：

```cpp
template<typename T>
using Vec = std::vector<T, std::allocator<T>>;  // C++11 支持

// typedef 无法直接实现模板别名
```

### 缺陷报告修正

| 缺陷报告 | 应用版本 | 原行为 | 修正后行为 |
|---------|---------|--------|-----------|
| CWG 576 | C++98 | `typedef` 不允许出现在整个函数定义中 | 允许出现在函数体内 |
| CWG 2071 | C++98 | `typedef` 可出现在不含声明符的声明中 | 现已禁止 |

## 3. 语法与参数

### 基本语法

```cpp
typedef type-declaration;
```

其中 `type-declaration` 是一个声明，去掉 `typedef` 关键字后将声明一个变量或函数。

### 语法位置

`typedef` 说明符通常出现在声明的开头，但也可以出现在类型说明符之后，或两个类型说明符之间：

```cpp
// 常见位置（开头）
typedef unsigned long ulong;

// 允许的位置（类型说明符后）
unsigned typedef long ulong;  // 合法但不推荐

// 允许的位置（类型说明符之间）
long unsigned typedef int long ullong;  // 等价于 typedef unsigned long long int ullong;
```

`typedef` 说明符只能与类型说明符组合，不能与其他说明符（如存储类说明符）组合。

### 声明形式

typedef 声明可以在同一行声明一个或多个标识符，包括：

| 声明类型 | 语法示例 | 说明 |
|---------|---------|------|
| 基本类型别名 | `typedef int int_t;` | int_t 是 int 的别名 |
| 指针类型别名 | `typedef int* intp_t;` | intp_t 是 int* 的别名 |
| 数组类型别名 | `typedef int arr_t[10];` | arr_t 是 10 元素 int 数组的别名 |
| 函数指针别名 | `typedef int (*fp)(int, ulong);` | fp 是函数指针类型别名 |
| 引用类型别名 | `typedef int (&fp)(int, ulong);` | fp 是函数引用类型别名 |

### 一行声明多个标识符

```cpp
typedef int int_t, *intp_t, (&fp)(int, ulong), arr_t[10];
// int_t  -> int
// intp_t -> int*
// fp     -> 函数引用，参数为 (int, ulong)，返回 int
// arr_t  -> int[10]
```

### 禁止用法

`typedef` 说明符**不能**出现在：

```cpp
// 函数参数声明中禁止
void f1(typedef int param); // 错误

// 函数定义的声明说明符序列中禁止
typedef int f2() {}         // 错误

// 不含声明符的声明中禁止
typedef struct X {};        // 错误

// 与存储类说明符组合禁止
// typedef static unsigned int uint;  // 错误
```

### 链接目的的 typedef 名称

如果 typedef 声明定义了一个无名类或枚举，则该声明中声明的第一个 typedef 名称是该类型的**链接目的 typedef 名称**（typedef name for linkage purposes）：

```cpp
typedef struct { /* ... */ } S;  // S 是链接目的 typedef 名称
```

这样定义的类或枚举类型具有外部链接（除非在无名命名空间中）。

**C++20 限制**：通过 typedef 定义的无名类应仅包含 C 兼容的构造：
- 只能声明非静态数据成员、成员枚举或成员类
- 不能有基类或默认成员初始化器
- 不能包含 lambda 表达式
- 所有成员类也必须满足这些要求（递归）

```cpp
// C++20 错误示例
typedef struct { void f() {} } C_Incompatible;  // 错误：有成员函数
```

## 4. 底层原理

### 类型别名本质

`typedef` 创建的是**别名**（alias），而非新类型：

```cpp
typedef unsigned long ulong;

unsigned long l1;
ulong l2;

// l1 和 l2 类型完全相同
// 它们是同一类型的两个名称
```

这意味着：

1. **类型等价**：typedef 名称与原类型完全等价，可互换使用
2. **不能重定义类型含义**：无法用 typedef 改变已有类型名的含义
3. **只能重新声明相同类型**：同一作用域内 typedef 名称只能重新声明为相同类型

### const 修饰符行为

理解 typedef 与 const 的交互非常重要：

```cpp
typedef int* intp_t;

const intp_t p1 = 0;  // 等价于 int* const p1 = 0（指针本身是 const）
const int* p2;        // 指向 const int 的指针

// p1 和 p2 类型不同！
// p1: 指针本身不可变，指向 int
// p2: 指针可变，指向 const int
```

**规则**：typedef 名称被视为一个整体类型，const 修饰的是这个整体类型。

### 作用域规则

typedef 名称仅在其可见的作用域内有效：

```cpp
void func1() {
    typedef int INT;  // INT 在 func1 内有效
}

void func2() {
    typedef double INT;  // 合法：不同作用域，INT 可指代不同类型
}

class ClassA {
    typedef int INT;  // INT 在 ClassA 内有效
};

class ClassB {
    typedef double INT;  // 合法：不同类，INT 可指代不同类型
};
```

### 重声明规则

typedef 名称只能重新声明为相同类型：

```cpp
typedef int integer;
typedef int integer;     // 合法：重新声明相同类型
// typedef long integer; // 错误：不能改变类型含义
```

## 5. 使用场景

### 适合使用 typedef 的场景

| 场景 | 示例 | 优势 |
|------|------|------|
| 简化复杂类型声明 | `typedef int (*Callback)(int, int);` | 提高可读性 |
| 跨平台类型定义 | `typedef int32_t my_int;` | 便于移植 |
| 函数指针声明 | `typedef void (*EventHandler)();` | 简化回调声明 |
| 结构体标签简化 | `typedef struct S { ... } S;` | 避免 `struct S` 写法 |
| 隐藏实现细节 | `typedef std::map<std::string, int> NameMap;` | 降低耦合 |
| 模板元编程 | `typedef const T type;` | 类型特征成员定义 |

### typedef vs using (C++11)

| 特性 | typedef | using |
|------|---------|-------|
| 基本类型别名 | `typedef int INT;` | `using INT = int;` |
| 模板别名 | 不支持 | `template<typename T> using Vec = std::vector<T>;` |
| 函数指针 | `typedef void (*FP)(int);` | `using FP = void(*)(int);` |
| 可读性 | 类型在前，别名在后 | 别名在前，类型在后 |
| C 语言兼容 | 完全兼容 | 不兼容 |

### 最佳实践

1. **优先使用 `using`（C++11+）**：语法更清晰，支持模板别名

```cpp
// C++11 推荐方式
using ErrorCode = int;
using Callback = void(*)(int, std::string);

// 模板别名（typedef 无法实现）
template<typename T>
using Vec = std::vector<T>;
```

2. **为函数指针使用别名**：大幅提高声明和使用的可读性

```cpp
// 复杂的函数指针难以阅读
void (*callback)(int, const char*);

// 使用 typedef 简化
typedef void (*Callback)(int, const char*);
Callback cb = myFunction;  // 清晰易读
```

3. **使用语义化命名**：别名名称应反映其用途

```cpp
// 推荐
typedef int ErrorCode;
typedef unsigned long ThreadId;

// 不推荐
typedef int I;  // 名称无意义
```

### 常见陷阱

#### 陷阱 1：const 与指针的交互

```cpp
typedef int* IntPtr;

const IntPtr p1 = nullptr;  // int* const p1（指针是 const）
const int* p2 = nullptr;    // 指向 const int 的指针

// p1 和 p2 类型不同！
```

#### 陷阱 2：结构体标签与 typedef 名称冲突

```cpp
typedef struct Node {
    struct listNode* next;  // 声明新的不完整结构体 listNode
} listNode;  // 错误：与之前声明的结构体名称冲突

// 修正方案
typedef struct Node {
    struct Node* next;  // 自引用
} Node;
```

#### 陷阱 3：C++20 对链接 typedef 名称的限制

```cpp
// C++20 错误：带有 typedef 名称用于链接的结构体不能有成员函数
typedef struct { void f() {} } C_Incompatible;  // C++20 编译错误

// 修正：移除成员函数或使用命名结构体
typedef struct { int a; int b; } C_Compatible;  // 正确
struct NamedStruct { void f() {} };  // 正确
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

// 简单类型别名
typedef unsigned long ulong;

// 复杂声明：一行声明多个不同类型
typedef int int_t, *intp_t, (&fp)(int, ulong), arr_t[10];

int main() {
    // 以下两个对象类型相同
    unsigned long l1;
    ulong l2;  // l1 和 l2 类型相同

    // 数组类型
    int a1[10];
    arr_t a2;   // a1 和 a2 类型相同

    // 指针类型与 const 的交互
    const intp_t p1 = 0;  // int* const p1 = 0
    const int* p2;        // p1 和 p2 类型不同！

    std::cout << "l1 and l2 are the same type" << std::endl;

    return 0;
}
```

### 结构体与指针别名

```cpp
#include <iostream>

// C 风格：避免每次写 struct 关键字
typedef struct { int a; int b; } S, *pS;

int main() {
    S s1 = {1, 2};
    pS ps1 = &s1;
    S* ps2 = &s1;  // ps1 和 ps2 类型相同

    std::cout << "s1.a = " << s1.a << std::endl;
    std::cout << "ps1->b = " << ps1->b << std::endl;

    return 0;
}
```

### 函数指针别名

```cpp
#include <iostream>
#include <string>

// 函数指针别名 - typedef 方式
typedef int (*CompareFunc)(const std::string&, const std::string&);

// 函数指针别名 - C++11 using 方式（推荐）
using CompareFuncModern = int (*)(const std::string&, const std::string&);

// 比较函数
int compareLength(const std::string& a, const std::string& b) {
    return static_cast<int>(a.length() - b.length());
}

// 使用函数指针的函数
void sortStrings(std::string arr[], int n, CompareFunc cmp) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (cmp(arr[j], arr[j+1]) > 0) {
                std::swap(arr[j], arr[j+1]);
            }
        }
    }
}

int main() {
    std::string words[] = {"apple", "pie", "strawberry", "a"};

    // 使用函数指针
    sortStrings(words, 4, compareLength);

    for (const auto& w : words) {
        std::cout << w << " ";
    }
    // 输出: a pie apple strawberry

    return 0;
}
```

### 模板元编程中的应用

```cpp
#include <iostream>
#include <type_traits>

// 模板类中使用 typedef 定义成员类型（C++11 之前）
template<typename T>
struct add_const {
    typedef const T type;  // 成员 typedef
};

// C++11 方式（推荐）
template<typename T>
struct add_const_modern {
    using type = const T;
};

int main() {
    // 使用成员 typedef
    add_const<int>::type x = 42;  // x 是 const int 类型

    // 标准库也使用此模式
    static_assert(std::is_same<std::add_const<int>::type, const int>::value,
                  "std::add_const works the same way");

    std::cout << "x = " << x << std::endl;
    return 0;
}
```

### 常见错误及修正

#### 错误 1：typedef 与存储类说明符组合

```cpp
// 错误：typedef 不能与存储类说明符组合
// typedef static unsigned int uint;  // 编译错误

// 修正：分开声明
typedef unsigned int uint;
static uint global_var;  // 正确
```

#### 错误 2：typedef 无声明符的声明

```cpp
// 错误：typedef 声明必须包含声明符
// typedef struct X {};  // C++98 编译错误（CWG 2071）

// 修正：添加声明符
typedef struct X {} X_t;  // 正确
```

#### 错误 3：在函数参数中使用 typedef

```cpp
// 错误：typedef 不能出现在参数声明中
// void f1(typedef int param);  // 编译错误

// 修正：先定义 typedef，再使用
typedef int ParamType;
void f1(ParamType param);  // 正确
```

#### 错误 4：在函数定义中使用 typedef

```cpp
// 错误：typedef 不能出现在函数定义中
// typedef int f2() {}  // 编译错误

// 修正：在函数体外定义 typedef
typedef int (*FuncPtr)();
FuncPtr getFunction();  // 正确
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| 本质 | 创建类型别名，不创建新类型 |
| 作用域 | 遵循普通声明的作用域规则 |
| 位置 | 可出现在声明的多处位置 |
| 兼容性 | 与 C 语言完全兼容 |

### typedef vs using 对比

| 方面 | typedef | using (C++11) |
|------|---------|---------------|
| 基本类型 | `typedef int INT;` | `using INT = int;` |
| 函数指针 | `typedef int (*FP)(int);` | `using FP = int(*)(int);` |
| 模板别名 | 不支持 | `template<typename T> using Vec = std::vector<T>;` |
| 可读性 | 类型在前，别名在后 | 别名在前，类型在后 |
| C 兼容 | 完全兼容 | 不兼容 |
| 推荐度 | C++11 前必需 | C++11+ 推荐使用 |

### 使用建议

1. **C++11 及以后优先使用 `using`**：语法更清晰，支持模板别名
2. **理解 const 位置**：typedef 名称作为一个整体被 const 修饰
3. **避免名称冲突**：注意结构体标签与 typedef 名称的命名
4. **跨平台代码**：使用 typedef 或 using 定义平台无关的类型别名
5. **函数指针使用别名**：大幅提高声明和使用的可读性

### 相关概念

| 概念 | 关系 |
|------|------|
| 类型别名 (type alias) | C++11 引入的替代语法，功能等价 |
| 别名模板 (alias template) | C++11 引入，typedef 无法实现 |
| `decltype` | 类型推导，与 typedef 配合使用 |
| `std::add_const` 等 | 类型特征，内部使用 typedef 定义成员类型 |

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/typedef
- C++ Standard: [dcl.typedef]
- Effective Modern C++, Scott Meyers, Item 9