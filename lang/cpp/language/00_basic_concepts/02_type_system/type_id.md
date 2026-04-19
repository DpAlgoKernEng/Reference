# 类型（Type）

## 1. 概述（Overview）

对象、引用、函数（包括函数模板特化）和表达式都有一个称为**类型（type）** 的属性。类型既限制了对这些实体允许的操作，也为原本通用的位序列提供了语义意义。

### 类型的作用

| 作用 | 说明 |
|------|------|
| 操作限制 | 定义可执行的合法操作 |
| 语义赋予 | 为二进制位提供解释意义 |
| 编译检查 | 支持类型安全的编译时检查 |

## 2. 来源与演变（Origin and Evolution）

### 标准演变

| 标准 | 主要变更 |
|------|----------|
| C++98 | 定义基础类型系统：基本类型、复合类型 |
| C++11 | 新增 `nullptr_t`、`char16_t`、`char32_t`、右值引用、作用域枚举 |
| C++17 | 沿用既有类型系统 |
| C++20 | 新增 `char8_t` 类型 |
| C++23 | 新增扩展浮点类型、固定宽度浮点类型 |

### 废弃的类型类别

| 类别 | 状态 | 说明 |
|------|------|------|
| POD 类型 | C++20 起废弃 | 被 trivially copyable 和 standard-layout 替代 |
| 平凡类型 | C++26 起废弃 | 被 trivially copyable 替代 |

## 3. 语法与参数（Syntax and Parameters）

### 3.1 类型分类

C++ 类型系统由以下类型组成：

```
类型
├── 基本类型（fundamental types）(see also std::is_fundamental);
│   ├── void 类型 (see also std::is_void);
│   ├── std::nullptr_t（C++11 起）(see also std::is_null_pointer);
│   └── 算术类型（arithmetic types）(see also std::is_arithmetic)
│       ├── 整型类型（integral types）(including cv-qualified versions, see also std::is_integral, a synonym for integral type is integer type):
│       │   ├── bool
│       │   ├── 字符类型
│       │   │   ├── 窄字符类型：char, signed char, unsigned char, char8_t（C++20 起）
│       │   │   └── 宽字符类型：char16_t, char32_t（C++11 起）, wchar_t
│       │   ├── 有符号整型
│       │   │   ├── 标准：signed char, short, int, long, long long
│       │   │   └── 扩展（实现定义）
│       │   └── 无符号整型
│       │       ├── 标准：unsigned char, unsigned short, unsigned, unsigned long, unsigned long long
│       │       └── 扩展（实现定义）
│       └── 浮点类型 (see also std::is_floating_point)
│           ├── 标准：float, double, long double
│           └── 扩展（C++23 起）：固定宽度浮点类型等
│
└── 复合类型（compound types）(see also std::is_compound)
    ├── 引用类型 (see also std::is_reference)
    │   ├── 左值引用 (see also std::is_lvalue_reference)
    │   └── 右值引用（C++11 起）(see also std::is_rvalue_reference)
    ├── 指针类型 (see also std::is_pointer)
    │   ├── 对象指针
    │   ├── 函数指针
    │   └── 成员指针 (see also std::is_member_pointer)
    │       ├── 数据成员指针 (see also std::is_member_object_pointer)
    │       └── 成员函数指针 (see also std::is_member_function_pointer)
    ├── 数组类型 (see also std::is_array)
    ├── 函数类型 (see also std::is_function)
    ├── 枚举类型 (see also std::is_enum)
    │   ├── 无作用域枚举
    │   └── 有作用域枚举（C++11 起） (see also std::is_scoped_enum)
    └── 类类型
        ├── 非联合体类 (see also std::is_class)
        └── 联合体 (see also std::is_union)
```

### 3.2 CV 限定符

对于除引用和函数外的每个非 cv 限定类型，类型系统支持该类型的三个额外的 cv 限定版本：

| 限定符 | 说明 |
|--------|------|
| `const` | 常量，不可修改 |
| `volatile` | 易变，禁止优化 |
| `const volatile` | 常量且易变 |

### 3.3 其他类型类别

| 类别 | 包含的类型 | 类型特征 |
|------|------------|----------|
| 对象类型 | 非函数、非引用、非 void 的类型 | `std::is_object` |
| 标量类型 | 算术类型、枚举类型、指针类型、成员指针类型、`nullptr_t` | `std::is_scalar` |
| 隐式生命周期类型 | 标量类型、隐式生命周期类类型、数组类型 | — |
| 平凡可复制类型 | 标量类型、平凡可复制类类型、此类数组 | `std::is_trivially_copyable` |
| 标准布局类型 | 标量类型、标准布局类类型、此类数组 | `std::is_standard_layout` |

### 3.4 程序定义类型

程序定义类型 是 C++ 标准中的一个分类术语，指由用户程序自己定义的类型，而非标准库或编译器实现提供的类型。

```markdown
定义拆解                                   
┌──────────────┬────────────────────────────────────────────────────────────────────┐                                                                            
│     术语     │                                含义                                │                                                                            
├──────────────┼────────────────────────────────────────────────────────────────────┤
│ 程序定义特化 │ 用户自己写的模板显式特化或偏特化，不是标准库的，也不是编译器内置的 │
├──────────────┼────────────────────────────────────────────────────────────────────┤
│ 程序定义类型 │ 用户自定义的类型，不包括标准库类型和编译器内置类型                 │
└──────────────┴────────────────────────────────────────────────────────────────────┘

包含的类型

1. 非闭包的类类型或枚举类型
// 程序定义类型
struct MyStruct { int x; };           // 用户定义的类类型
class MyClass { };                     // 用户定义的类类型
enum class Color { Red, Green, Blue }; // 用户定义的枚举类型

// 不是程序定义类型
std::string s;     // 标准库类型
std::vector<int> v; // 标准库类型

2. Lambda 闭包类型（C++11 起）
// 程序定义类型
auto lambda = [](int x) { return x * 2; };
// lambda 的类型是编译器生成的闭包类，属于程序定义类型

// 不是程序定义类型（假设编译器提供的内置 lambda）
// 某些实现可能提供特殊的 lambda，但这很罕见

3. 程序定义特化的实例化
template<typename T>
struct Traits { };

// 程序定义特化
template<>
struct Traits<int> {
    static constexpr int value = 42;
};

// Traits<int> 是程序定义类型

与其他类型的关系
所有类型
├── 标准库类型（std::string, std::vector 等）
├── 实现定义类型（编译器内置类型，如 __int128）
└── 程序定义类型
    ├── 用户定义的 class/struct
    ├── 用户定义的 enum/enum class
    ├── Lambda 闭包类型
    └── 用户模板特化的实例化

实际意义
这个区分在标准中主要用于：
1. 类型特征判断：如 std::is_trivially_copyable 对程序定义类型的行为
2. ODR（单一定义规则）：程序定义类型有特定的链接和定义规则
3. 模板实例化：区分标准库特化和用户特化

简单理解
程序定义类型 = 用户自己写的类型 + Lambda 闭包类型
不是 std:: 开头的，也不是编译器内置扩展（如 __int128），就是程序定义类型。
```

### 3.5 类型命名

名称可以通过以下方式声明为引用类型：

- 类声明（class declaration）
- 联合体声明（union declaration）
- 枚举声明（enum declaration）
- typedef 声明（typedef declaration）
- 类型别名声明（type alias declaration）

### 3.6 类型标识（type-id）

用于引用无名类型的语法。类型标识 `T` 的语法与声明类型为 `T` 的变量或函数的语法完全相同，只是省略了标识符：

```cpp
int* p;                  // 指向 int 的指针声明
static_cast<int*>(p);    // 类型标识是 "int*"

int a[3];                // 3 个 int 的数组声明
new int[3];              // 类型标识是 "int[3]"

void f(int);                          // 函数声明
std::function<void(int)> x = f;       // 类型模板参数是类型标识 "void(int)"
```

**类型标识的使用场景**：
- 强制转换表达式中指定目标类型
- `sizeof`、`alignof`、`alignas`、`new`、`typeid` 的参数
- 类型别名声明的右侧
- 函数声明的尾置返回类型
- 模板类型参数的默认参数
- 模板类型参数的模板实参

## 4. 底层原理（Underlying Principles）

### 4.1 静态类型与动态类型

**静态类型（Static Type）**：编译时分析程序得出的表达式类型。静态类型在程序执行期间不会改变。

**动态类型（Dynamic Type）**：如果某个泛左值表达式引用多态对象，其最派生对象的类型称为动态类型。

```cpp
struct B { virtual ~B() {} };  // 多态类型
struct D : B {};                // 多态类型

D d;            // 最派生对象
B* ptr = &d;

// (*ptr) 的静态类型是 B
// (*ptr) 的动态类型是 D
```

对于纯右值表达式，动态类型始终与静态类型相同。

### 4.2 不完整类型（Incomplete Types）

**不完整类型**包括：

| 类型 | 说明 |
|------|------|
| `void`（可能 cv 限定） | 永远不完整 |
| 不完整定义的对象类型 | 类已声明但未定义 |
| 未知边界数组 | `extern int arr[];` |
| 不完整类型元素的数组 | 元素类型不完整 |
| 枚举类型 | 从声明点到确定底层类型之前 |

**需要完整类型的上下文**：

- 定义或调用返回类型或参数类型为 T 的函数
- 定义类型为 T 的对象
- 声明类型为 T 的非静态类数据成员
- 类型 T 或元素类型为 T 的对象的 new 表达式
- 对类型 T 的泛左值应用左值到右值转换
- 隐式或显式转换为类型 T
- 到 `T*` 或 `T&` 的标准转换、`dynamic_cast` 或 `static_cast`
- 对类型 T 的表达式应用类成员访问运算符
- 对类型 T 应用 `typeid`、`sizeof` 或 `alignof`
- 对指向 T 的指针应用算术运算符
- 定义基类为 T 的类
- 赋值给类型 T 的左值
- 类型为 T、`T&` 或 `T*` 的异常处理器

### 4.3 类型的完成

不完整定义的对象类型可以被完成：

```cpp
struct X;            // X 的声明，未提供定义
extern X* xp;        // xp 指向不完整类型

void foo() {
    xp++;            // 错误：X 不完整
}

struct X { int i; }; // X 的定义
X x;                 // OK：X 的定义可达

void bar() {
    xp = &x;         // OK：类型是 "pointer to X"
    xp++;            // OK：X 已完整
}
```

**重要规则**：
- 未知边界数组的指针或引用永久指向或引用不完整类型
- 由 typedef 声明命名的未知边界数组永久引用不完整类型

### 4.4 详述类型说明符

详述类型说明符可用于引用先前声明的类名或枚举名，即使名称被非类型声明隐藏：

```cpp
struct Node { int value; };
int Node = 0;           // 隐藏类型 Node

Node n;                 // 错误：Node 是变量
struct Node n2;         // OK：使用详述类型说明符
```

## 5. 使用场景（Use Cases）

### 5.1 基本类型选择

| 需求 | 推荐类型 |
|------|----------|
| 布尔值 | `bool` |
| 小整数 | `int` 或 `short` |
| 大整数 | `long long` |
| 字符 | `char`、`char8_t`、`char16_t`、`char32_t`、`wchar_t` |
| 单精度浮点 | `float` |
| 双精度浮点 | `double` |
| 空指针 | `std::nullptr_t`（C++11 起） |

### 5.2 复合类型使用

```cpp
// 引用类型
int& lref = x;           // 左值引用
int&& rref = 10;         // 右值引用（C++11 起）

// 指针类型
int* ptr = &x;           // 对象指针
void (*fp)(int);         // 函数指针
int Class::* pm;         // 成员指针

// 数组类型
int arr[10];

// 函数类型
int func(int, double);

// 枚举类型
enum Color { Red, Green, Blue };
enum class Status { OK, Error };  // C++11 起

// 类类型
struct Point { int x, y; };
union Data { int i; float f; };
```

### 5.3 类型特征

```cpp
#include <type_traits>

// 检查类型属性
static_assert(std::is_integral<int>::value);
static_assert(std::is_floating_point<double>::value);
static_assert(std::is_pointer<int*>::value);
static_assert(std::is_reference<int&>::value);

// C++17 简化语法
static_assert(std::is_integral_v<int>);
static_assert(std::is_floating_point_v<double>);
```

### 5.4 常见陷阱

1. **字符类型的符号性**：

```cpp
char c = 200;          // 实现定义行为
unsigned char uc = 200; // 明确为 200
signed char sc = 200;   // 可能溢出
```

2. **不完整类型使用**：

```cpp
struct Node;  // 不完整类型
// sizeof(Node);  // 错误：不完整类型

struct Node { int data; Node* next; };  // 完整类型
sizeof(Node);  // OK
```

3. **类型标识语法**：

```cpp
// 错误：在 sizeof 中定义新类型
// sizeof(struct { int x; });

// 正确：先定义类型
using T = struct { int x; };
sizeof(T);  // OK
```

## 6. 代码示例（Examples）

### 基本类型使用

```cpp
#include <iostream>
#include <cstdint>

int main()
{
    // 布尔类型
    bool b = true;

    // 字符类型
    char c = 'A';
    char8_t c8 = u8'A';   // C++20
    char16_t c16 = u'A';  // C++11
    char32_t c32 = U'A';  // C++11
    wchar_t wc = L'A';

    // 整型类型
    short s = 100;
    int i = 1000;
    long l = 100000L;
    long long ll = 10000000000LL;  // C++11

    // 无符号整型
    unsigned int ui = 4000000000U;
    unsigned long long ull = 18000000000000000000ULL;

    // 浮点类型
    float f = 3.14f;
    double d = 3.14159265358979;
    long double ld = 3.141592653589793238L;

    // 空指针类型（C++11）
    std::nullptr_t np = nullptr;

    std::cout << "int: " << i << std::endl;
    std::cout << "double: " << d << std::endl;

    return 0;
}
```

### 引用类型（C++11 起）

```cpp
#include <iostream>
#include <utility>

void process_lvalue(int& x) {
    std::cout << "lvalue reference: " << x << std::endl;
}

void process_rvalue(int&& x) {
    std::cout << "rvalue reference: " << x << std::endl;
}

int main()
{
    int a = 10;

    // 左值引用
    int& lref = a;
    process_lvalue(lref);
    process_lvalue(a);

    // 右值引用
    int&& rref = 20;
    process_rvalue(std::move(a));
    process_rvalue(30);

    // const 引用可以绑定到右值
    const int& cref = 40;
    std::cout << "const ref: " << cref << std::endl;

    return 0;
}
```

### 成员指针

```cpp
#include <iostream>

class Widget {
public:
    int value;
    void print() { std::cout << "value: " << value << std::endl; }
};

int main()
{
    // 数据成员指针
    int Widget::* pvalue = &Widget::value;

    // 成员函数指针
    void (Widget::* pprint)() = &Widget::print;

    Widget w{42};

    // 通过成员指针访问
    std::cout << "value: " << w.*pvalue << std::endl;
    (w.*pprint)();

    Widget* pw = &w;
    std::cout << "value: " << pw->*pvalue << std::endl;
    (pw->*pprint)();

    return 0;
}
```

### 类型特征使用（C++11 起）

```cpp
#include <iostream>
#include <type_traits>

// C++17 简化的类型特征
template<typename T>
void analyze_type() {
    std::cout << "Type analysis:\n";
    std::cout << "  is_integral: " << std::is_integral_v<T> << "\n";
    std::cout << "  is_floating_point: " << std::is_floating_point_v<T> << "\n";
    std::cout << "  is_pointer: " << std::is_pointer_v<T> << "\n";
    std::cout << "  is_reference: " << std::is_reference_v<T> << "\n";
    std::cout << "  is_array: " << std::is_array_v<T> << "\n";
    std::cout << "  is_class: " << std::is_class_v<T> << "\n";
}

int main()
{
    analyze_type<int>();
    analyze_type<double>();
    analyze_type<int*>();
    analyze_type<int&>();
    analyze_type<int[10]>();

    // 条件类型（C++17）
    using Number = std::conditional_t<sizeof(void*) == 8, long long, int>;
    std::cout << "sizeof(Number): " << sizeof(Number) << std::endl;

    return 0;
}
```

### 不完整类型示例

```cpp
#include <iostream>

// 不完整类型声明
struct Node;
extern Node* global_node;

// 使用不完整类型
Node* create_node();

// 定义完整类型
struct Node {
    int data;
    Node* next;
};

Node* create_node() {
    return new Node{0, nullptr};
}

// 未知边界数组
extern int arr[];
int arr[10];  // 完整定义

int main()
{
    Node* n = create_node();
    n->data = 42;
    std::cout << "Node data: " << n->data << std::endl;
    delete n;

    // 数组使用
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    return 0;
}
```

### 常见错误及修正

```cpp
#include <iostream>
#include <memory>

/* 错误：字符类型符号性问题 */
void error_char_sign()
{
    char c = 200;  // 实现定义行为
    // std::cout << c << std::endl;  // 可能为负数
}

/* 正确：使用明确的类型 */
void correct_char_sign()
{
    unsigned char uc = 200;  // 明确为 200
    std::cout << static_cast<int>(uc) << std::endl;
}

/* 错误：不完整类型操作 */
struct Incomplete;
void error_incomplete()
{
    // Incomplete* p = new Incomplete;  // 错误：不完整类型
}

/* 正确：先完成类型 */
struct Complete { int x; };
void correct_complete()
{
    Complete* p = new Complete{42};
    std::cout << p->x << std::endl;
    delete p;
}

/* 错误：类型标识中定义新类型 */
void error_type_id()
{
    // sizeof(struct { int x; });  // 错误
}

/* 正确：先定义类型 */
using MyStruct = struct { int x; };
void correct_type_id()
{
    std::cout << sizeof(MyStruct) << std::endl;
}

/* 错误：指针算术需要完整类型 */
extern int unknown_arr[];
void error_pointer_arithmetic()
{
    // unknown_arr++;  // 错误：数组大小未知
}

/* 正确：使用已知大小的数组 */
int known_arr[10];
void correct_pointer_arithmetic()
{
    int* p = known_arr;
    p++;  // OK
}

int main()
{
    correct_char_sign();
    correct_complete();
    correct_type_id();
    correct_pointer_arithmetic();

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **类型定义语义**：类型限制操作并提供语义意义
2. **两大类别**：基本类型和复合类型
3. **CV 限定符**：`const`、`volatile`、`const volatile`
4. **静态/动态类型**：编译时类型 vs 运行时类型
5. **不完整类型**：`void`、未定义类、未知边界数组

### 类型系统概览

| 类别 | 包含类型 |
|------|----------|
| 基本类型 | `void`、`nullptr_t`、算术类型 |
| 算术类型 | 整型、浮点型 |
| 整型 | `bool`、字符类型、有/无符号整型 |
| 复合类型 | 引用、指针、数组、函数、枚举、类 |

### 与 C 语言的区别

| 特性 | C++ | C |
|------|-----|---|
| 引用类型 | 有 | 无 |
| 成员指针 | 有 | 无 |
| 右值引用 | C++11 起 | 无 |
| 作用域枚举 | C++11 起 | 无 |
| 类类型 | 完整 OOP | 简单结构体 |
| 类型特征 | 丰富 | 无 |
| `nullptr_t` | C++11 起 | 无 |

### 最佳实践

- 使用 `auto` 和类型推导减少冗余（C++11 起）
- 使用类型特征进行编译时类型检查
- 理解静态类型与动态类型的区别
- 注意不完整类型的使用限制
- 使用 `<cstdint>` 中的固定宽度整数类型

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 6.8.2 Fundamental types [basic.fundamental]
- C++20 标准 (ISO/IEC 14882:2020): 6.8.2 Fundamental types [basic.fundamental]
- C++17 标准 (ISO/IEC 14882:2017): 6.9.1 Fundamental types [basic.fundamental]
- C++14 标准 (ISO/IEC 14882:2014): 3.9.1 Fundamental types [basic.fundamental]
- C++11 标准 (ISO/IEC 14882:2011): 3.9.1 Fundamental types [basic.fundamental]
- C++98 标准 (ISO/IEC 14882:1998): 3.9.1 Fundamental types [basic.fundamental]