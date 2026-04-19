# 指针声明 (Pointer Declaration)

## 1. 概述

指针（Pointer）是 C++ 中最基础也最重要的概念之一。指针声明用于声明指针类型或指向成员的指针类型的变量。指针是一个存储内存地址的变量，通过指针可以直接操作内存，实现间接访问数据、动态内存管理、函数回调等高级功能。

指针类型的值可以是以下四种之一：
- **指向对象或函数的指针**：指针指向某个具体的对象或函数
- **指向对象末尾后一位的指针**（pointer past the end）：表示对象存储空间之后第一个字节的地址
- **空指针值**（null pointer）：不指向任何对象或函数的特殊值
- **无效指针值**（invalid pointer value）：其他无效的指针值

### 核心概念

指向对象的指针表示该对象在内存中占用的第一个字节的地址。指向对象末尾后一位的指针表示该对象存储空间结束后第一个字节的地址。

需要注意：两个表示相同地址的指针可能具有不同的值。

## 2. 来源与演变

### 历史背景

指针的概念源自 C 语言，是 C/C++ 区别于其他高级语言的核心特性之一。指针的设计动机包括：

1. **直接内存访问**：允许程序员直接操作内存地址
2. **动态内存管理**：支持运行时分配和释放内存
3. **函数回调**：通过函数指针实现回调机制
4. **多态实现**：通过基类指针调用派生类方法

### C++98 标准

指针声明的基本语法在 C++98 中就已确立，包括：
- 普通指针声明
- 成员指针声明
- const/volatile 限定符支持

### C++11 变化

- 引入 `nullptr` 关键字，作为类型安全的空指针常量
- 新增 `std::nullptr_t` 类型
- 支持属性（attributes）在指针声明中使用

### C++17 变化

- 复合指针类型规则完善，支持 noexcept 函数指针转换

### 缺陷报告修正

| 缺陷报告 | 问题 | 修正 |
|---------|------|------|
| CWG 73 | 指向对象的指针与指向数组末尾后一位的指针比较问题 | 对于非空和非函数指针，比较它们表示的地址 |
| CWG 903 | 任何求值为 0 的整型常量表达式都是空指针常量 | 限制为值为 0 的整数字面量 |
| CWG 1438 | 以任何方式使用无效指针值都是未定义行为 | 解引用和传递给释放函数以外的是实现定义行为 |
| CWG 1512 | 复合指针类型规则不完整 | 使规则完整 |
| CWG 2206 | void 指针和函数指针有复合指针类型 | 它们没有复合指针类型 |
| CWG 2381 | 确定复合指针类型时不允许函数指针转换 | 允许转换 |
| CWG 2822 | 存储区域结束时指针值可能失效 | 指针有效性基于求值上下文 |
| CWG 2933 | 函数指针总是无效的 | 函数指针总是有效的 |

## 3. 语法与参数

### 基本语法

指针声明的声明符有以下两种形式：

| 形式 | 语法 | 说明 |
|------|------|------|
| 指针声明符 | `*` attr(可选) cv(可选) declarator | 声明 D 为指向类型 S 的指针 |
| 成员指针声明符 | nested-name-specifier `*` attr(可选) cv(可选) declarator | 声明 D 为指向类 C 的非静态成员的指针 |

### 参数说明

| 参数 | 说明 |
|------|------|
| nested-name-specifier | 名称和作用域解析运算符 `::` 的序列，用于指定成员所属的类 |
| attr (C++11 起) | 属性列表 |
| cv | const/volatile 限定符，应用于被声明的指针（而非被指向的类型） |
| declarator | 除引用声明符外的任何声明符（可以是另一个指针声明符，允许多级指针） |

### const 限定符位置规则

const 限定符的位置决定了它是修饰指针本身还是修饰被指向的对象：

| 语法 | 含义 |
|------|------|
| `const T*` | 指向常量对象的指针 |
| `T const*` | 指向常量对象的指针（等价写法） |
| `T* const` | 指向对象的常量指针 |
| `const T* const` | 指向常量对象的常量指针 |
| `T const* const` | 指向常量对象的常量指针（等价写法） |

**关键规则**：
- 如果 `cv` 出现在 `*` 之前，它是声明说明符序列的一部分，应用于所指向的对象
- 如果 `cv` 出现在 `*` 之后，它是声明符的一部分，应用于被声明的指针本身

### 指针类型分类

| 类型 | 声明示例 | 说明 |
|------|----------|------|
| 对象指针 | `int* p;` | 指向对象的指针 |
| 函数指针 | `void (*pf)(int);` | 指向函数的指针 |
| void 指针 | `void* pv;` | 可指向任何对象类型 |
| 数据成员指针 | `int C::* pm;` | 指向类数据成员的指针 |
| 成员函数指针 | `void (C::* pmf)(int);` | 指向类成员函数的指针 |

## 4. 底层原理

### 指针的内存表示

指针存储的是内存地址。指向对象的指针表示该对象在内存中占用的第一个字节的地址：

```
内存地址:    0x1000    0x1004    0x1008    0x100C
            +--------+--------+--------+--------+
对象 c:     |   x    |   y    |
            +--------+--------+
指针:       px -> 0x1000 (指向 c.x)
            py -> 0x1004 (指向 c.y)
            pxe -> 0x1004 (指向 c.x 末尾后一位)
```

### 指针值的特性

1. **相同地址可能有不同的指针值**：两个表示相同地址的指针可能有不同的值
2. **末尾后一位指针**：指向对象存储空间后第一个字节的地址
3. **指针算术**：指针支持加减运算，用于遍历数组

```cpp
struct C { int x, y; } c;

int* px = &c.x;    // px 的值是"指向 c.x 的指针"
int* pxe = px + 1; // pxe 的值是"指向 c.x 末尾后一位的指针"
int* py = &c.y;    // py 的值是"指向 c.y 的指针"

assert(pxe == py); // 可能触发或不触发：== 比较是否表示相同地址
*pxe = 1;          // 未定义行为！即使断言不触发也不能解引用
```

### 空指针

每个指针类型都有一个特殊的**空指针值**（null pointer value）。空指针不指向任何对象或函数，对空指针解引用是未定义行为。

空指针常量的形式：
- 值为零的整数字面量（C++11 之前）
- `nullptr`（C++11 起）
- `NULL` 宏（实现定义）

零初始化和值初始化也会将指针初始化为其空指针值。

### 无效指针

指针值 p 在求值上下文 e 中有效，当满足以下条件之一：
- p 是空指针值
- p 是函数指针
- p 是指向对象 o 或其末尾后一位的指针，且 e 在对象 o 的存储期内

无效指针的使用：
- **解引用**：未定义行为
- **传递给释放函数**：未定义行为
- **其他操作**（如复制）：实现定义行为

```cpp
int* f() {
    int obj;
    int* local_ptr = new (&obj) int;  // placement new

    *local_ptr = 1;  // 正确：在 obj 的存储期间

    return local_ptr;
}

int* ptr = f();  // obj 的存储期间已结束

int* copy = ptr; // 实现定义行为
*ptr = 2;        // 未定义行为：解引用无效指针
delete ptr;      // 未定义行为：对无效指针调用释放函数
```

### void 指针

指向任何对象类型的指针都可以隐式转换为 `void*`（可能有 cv 限定）。反向转换需要 `static_cast` 或显式转换，并产生原始指针值。

`void*` 与 `char*` 具有相同的大小、表示和对齐要求。

void 指针常用于传递未知类型的对象，常见于 C 接口：
- `std::malloc` 返回 `void*`
- `std::qsort` 期望用户提供的回调接受两个 `const void*` 参数
- `pthread_create` 期望用户提供的回调接受并返回 `void*`

### 指针比较

指针比较的规则：
- 表示相同地址的两个指针比较相等
- 两个空指针值比较相等
- 同一数组的元素指针按数组索引顺序比较
- 具有相同成员访问权限的非静态数据成员指针按声明顺序比较

许多实现提供指针的严格全序排序（如连续虚拟地址空间中的地址）。不支持此特性的实现提供 `std::less` 的指针特化版本，保证严格全序，使得可以将任意来源的指针用作标准关联容器（如 `std::set` 或 `std::map`）的键。

### 复合指针类型

当比较运算符或条件运算符的操作数为指针或成员指针时，需要确定复合指针类型作为公共类型。复合指针类型的确定规则较为复杂，涉及类型相似性、cv 限定符合并等。

```cpp
using p = void*;
using q = const int*;
// 复合指针类型是 "const void*"

using pi = int**;
using pci = const int**;
// 复合指针类型是 "const void* const*"
```

## 5. 使用场景

### 适合使用指针的场景

| 场景 | 说明 |
|------|------|
| 动态内存管理 | 配合 `new`/`delete` 进行内存分配和释放 |
| C 接口交互 | `void*` 用于传递未知类型的对象，常见于 C 库接口 |
| 函数回调 | 函数指针用于实现回调机制 |
| 多态 | 基类指针指向派生类对象，实现运行时多态 |
| 可选参数 | 空指针表示参数缺失 |
| 数组遍历 | 指针算术遍历连续内存 |

### 指针与引用的选择

| 特性 | 指针 | 引用 |
|------|------|------|
| 可重新绑定 | 可以 | 不可以 |
| 可为空 | 可以 | 不可以 |
| 需要解引用 | 需要 | 不需要 |
| 支持算术运算 | 支持 | 不支持 |
| 重新赋值语义 | 明确 | 需要额外机制 |

### 最佳实践

1. **优先使用智能指针**：`std::unique_ptr` 和 `std::shared_ptr` 管理所有权
2. **使用 `nullptr` 而非 `NULL` 或 `0`**：类型安全
3. **避免裸指针所有权**：裸指针应用于非所有权场景
4. **检查空指针**：在解引用前检查指针是否为空
5. **使用 `const` 修饰不可变数据**：`const T*` 表示指向不可变数据

### 常见陷阱

1. **悬空指针**：指向已销毁对象的指针
2. **内存泄漏**：分配后未释放
3. **双重释放**：同一指针多次 `delete`
4. **空指针解引用**：未检查空指针
5. **指针算术越界**：数组越界访问
6. **类型不匹配**：错误的指针类型转换

## 6. 代码示例

### 基础用法：对象指针

```cpp
#include <iostream>

int main() {
    int n = 10;
    int* p = &n;        // 指向 int 的指针

    std::cout << "Value: " << *p << '\n';     // 解引用：输出 10
    std::cout << "Address: " << p << '\n';    // 输出地址

    *p = 20;            // 通过指针修改值
    std::cout << "New value: " << n << '\n';  // 输出 20

    return 0;
}
```

### 基础用法：const 指针

```cpp
#include <iostream>

int main() {
    int x = 10;
    int y = 20;

    // 指向常量对象的指针（底层 const）
    const int* pc = &x;
    // *pc = 30;  // 错误：不能修改被指向的对象
    pc = &y;      // 正确：可以修改指针本身

    // 常量指针（顶层 const）
    int* const cp = &x;
    *cp = 30;     // 正确：可以修改被指向的对象
    // cp = &y;  // 错误：不能修改指针本身

    // 指向常量对象的常量指针
    const int* const cpc = &x;
    // *cpc = 40;  // 错误
    // cpc = &y;   // 错误

    std::cout << x << ' ' << *pc << ' ' << *cp << '\n';
    return 0;
}
```

### 基础用法：void 指针

```cpp
#include <iostream>
#include <cstdlib>  // for malloc/free

int main() {
    int n = 42;
    void* pv = &n;  // 任何对象指针都可以隐式转换为 void*

    // void* 需要显式转换回原类型才能使用
    int* pi = static_cast<int*>(pv);
    std::cout << *pi << '\n';  // 输出 42

    // 常见于 C 接口
    void* buffer = std::malloc(100);  // 返回 void*
    std::free(buffer);

    return 0;
}
```

### 基础用法：函数指针

```cpp
#include <iostream>

// 普通函数
void hello(int n) {
    std::cout << "Hello " << n << "!\n";
}

int square(int n) {
    return n * n;
}

int main() {
    // 函数指针声明和初始化
    void (*pf)(int) = hello;   // & 可选
    int (*ps)(int) = square;

    // 通过函数指针调用
    pf(1);                      // 输出 "Hello 1!"
    std::cout << ps(5) << '\n'; // 输出 25

    // 函数指针数组
    void (*funcs[])(int) = {hello, hello, hello};
    funcs[0](10);               // 输出 "Hello 10!"

    // 使用类型别名简化
    using IntFunc = int(int);
    IntFunc* pf2 = square;
    std::cout << pf2(3) << '\n'; // 输出 9

    return 0;
}
```

### 基础用法：成员指针

```cpp
#include <iostream>
#include <functional>

struct MyClass {
    int data;
    void print(int n) const {
        std::cout << "Data: " << data << ", n: " << n << '\n';
    }
};

int main() {
    // 数据成员指针
    int MyClass::* pmd = &MyClass::data;
    MyClass obj{42};

    obj.*pmd = 100;                    // 通过成员指针修改数据
    std::cout << obj.*pmd << '\n';     // 输出 100

    // 成员函数指针
    void (MyClass::* pmf)(int) const = &MyClass::print;
    (obj.*pmf)(5);                     // 输出 "Data: 100, n: 5"

    MyClass* pobj = &obj;
    (pobj->*pmf)(10);                  // 输出 "Data: 100, n: 10"

    // 使用 std::mem_fn 简化
    auto fn = std::mem_fn(&MyClass::print);
    fn(obj, 20);                       // 输出 "Data: 100, n: 20"

    return 0;
}
```

### 高级用法：多级指针

```cpp
#include <iostream>

int main() {
    int n = 10;
    int* p = &n;
    int** pp = &p;    // 指向指针的指针
    int*** ppp = &pp; // 三级指针

    std::cout << **pp << '\n';    // 输出 10
    std::cout << ***ppp << '\n';  // 输出 10

    // 常量与多级指针
    const int* pc = &n;       // 指向 const int 的指针
    const int** ppc = &pc;    // 指向 const int* 的指针

    // 注意：int** 不能转换为 const int**（但可以转换为 const int* const*）
    // ppc = &p;  // 错误！

    return 0;
}
```

### 高级用法：继承与指针

```cpp
#include <iostream>

struct Base {
    int x;
    virtual void foo() { std::cout << "Base::foo\n"; }
};

struct Derived : Base {
    int y;
    void foo() override { std::cout << "Derived::foo\n"; }
};

int main() {
    Derived d;
    Base* pb = &d;  // 派生类指针隐式转换为基类指针

    pb->foo();       // 虚函数调用：输出 "Derived::foo"
    pb->x = 10;      // 访问基类成员

    // 基类成员指针可转换为派生类成员指针
    int Base::* pmb = &Base::x;
    int Derived::* pmd = pmb;
    d.*pmd = 20;
    std::cout << d.x << '\n';  // 输出 20

    return 0;
}
```

### 高级用法：成员指针与标准库

```cpp
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> v = {"a", "ab", "abc"};
    std::vector<std::size_t> l;

    // 使用 std::mem_fn 包装成员函数指针
    std::transform(v.begin(), v.end(), std::back_inserter(l),
                   std::mem_fn(&std::string::size));

    for (std::size_t n : l) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
    // 输出：1 2 3

    return 0;
}
```

### 常见错误 1：空指针解引用

```cpp
// 错误：未检查空指针
int* p = nullptr;
// std::cout << *p;  // 未定义行为！

// 修正：检查空指针
int* p = nullptr;
if (p != nullptr) {
    std::cout << *p;
}

// 更好的修正：使用引用或智能指针
int& ref = /* 确保有效的引用 */;
```

### 常见错误 2：悬空指针

```cpp
// 错误：返回指向局部变量的指针
int* bad_function() {
    int local = 10;
    return &local;  // 返回后 local 已销毁
}

// 修正：返回值或使用动态分配
int good_function() {
    return 10;
}

// 或者使用智能指针
#include <memory>
std::unique_ptr<int> better_function() {
    return std::make_unique<int>(10);
}
```

### 常见错误 3：指针类型不匹配

```cpp
// 错误：void* 隐式转换
void* pv = /* ... */;
// int* pi = pv;  // 错误：不能隐式转换

// 修正：使用 static_cast
int* pi = static_cast<int*>(pv);
```

### 常见错误 4：错误的 const 转换

```cpp
int n = 10;
int* p = &n;
const int** pp = &p;  // 错误！这会绕过 const 保护

// 正确做法
const int* cp = &n;
const int** ppc = &cp;

// 或者
const int* const* ppcp = &p;
```

## 7. 总结

指针是 C++ 中功能强大但也容易出错的特性。核心要点：

### 指针类型分类

| 类型 | 用途 | 特点 |
|------|------|------|
| 对象指针 | 指向数据对象 | 支持算术运算 |
| 函数指针 | 指向函数 | 用于回调机制 |
| void 指针 | 通用指针 | 可接受任何对象指针 |
| 成员指针 | 指向类成员 | 需要通过对象访问 |

### 关键概念

1. **指针值**：可以是有效地址、空指针或无效指针
2. **const 限定**：区分顶层 const（指针本身）和底层 const（被指向对象）
3. **空指针**：使用 `nullptr`（C++11）而非 `NULL` 或 `0`
4. **智能指针**：优先使用 `std::unique_ptr` 和 `std::shared_ptr` 管理所有权

### 技术对比

| 特性 | 原始指针 | 智能指针 | 引用 |
|------|----------|----------|------|
| 可为空 | 是 | 是（unique_ptr/shared_ptr） | 否 |
| 可重新绑定 | 是 | 是 | 否 |
| 所有权语义 | 无 | 有 | 无 |
| 内存管理 | 手动 | 自动 | 无 |
| 语法开销 | 低 | 中 | 低 |

### 使用建议

1. **优先使用智能指针**：避免手动管理内存
2. **优先使用引用**：在不需要重新绑定或空值时
3. **明确所有权语义**：裸指针表示非所有权观察
4. **检查指针有效性**：解引用前确保非空
5. **使用 `nullptr`**：替代 `NULL` 和 `0`

### 相关概念

| 概念 | 关系 |
|------|------|
| 引用 | 更安全的指针替代品，不可重新绑定，不可为空 |
| 智能指针 | 封装裸指针，自动管理内存 |
| 迭代器 | 类似指针的抽象，用于容器遍历 |
| `std::span` (C++20) | 对连续内存的非拥有视图 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/pointer
- C++ Standard: [dcl.ptr]
- Effective Modern C++, Scott Meyers
- The C++ Programming Language, Bjarne Stroustrup