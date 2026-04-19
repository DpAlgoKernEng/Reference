# 常量表达式（Constant Expression）

## 1. 概述（Overview）

**常量表达式（Constant Expression）** 是指可以在编译期求值的表达式。这类表达式可用作非类型模板参数、数组大小以及其他需要常量表达式的上下文中。

### 核心定义

常量表达式是 C++ 类型系统和编译期计算的基础设施，它允许编译器在编译阶段执行计算，从而实现：
- 编译期优化
- 类型安全保证
- 模板元编程支持
- 数组边界检查
- 静态断言验证

### 基本示例

```cpp
int n = 1;
std::array<int, n> a1;  // 错误："n" 不是常量表达式

const int cn = 2;
std::array<int, cn> a2; // 正确："cn" 是常量表达式
```

### 关键概念

- **核心常量表达式（Core Constant Expression）**：满足特定约束的表达式，其求值不会触发不允许的操作
- **字面量类型（Literal Type）**：可以在常量表达式中使用的类型
- **常量初始化（Constant Initialization）**：在编译期完成的初始化
- **常量求值上下文（Constant Evaluation Context）**：要求常量表达式的语言构造

---

## 2. 来源与演变（Origin and Evolution）

### C++98 时代

最初，C++98 定义了**整型常量表达式（Integral Constant Expression）**的概念，用于以下场景：
- 数组边界
- case 标签常量
- 位域长度
- 枚举器初始化器
- 静态数据成员初始化器
- 整型或枚举类型的非类型模板参数

**C++98 整型常量表达式的约束：**
- 只能涉及：算术类型的字面量、枚举器、const 限定的整型/枚举变量、sizeof 表达式
- 不能使用浮点字面量（除非显式转换为整型）
- 不能使用函数、类对象、指针、引用、赋值运算符、递增/递减运算符等

### C++11 革命性变化

C++11 引入了 `constexpr` 关键字，大幅扩展了常量表达式的能力：
- 引入**字面量类型（Literal Type）**概念
- 支持 constexpr 函数和构造函数
- 扩展常量表达式的范畴，包括：
  - 非指针字面量类型的纯右值核心常量表达式
  - 指向静态存储期对象或函数的左值核心常量表达式
  - 特定指针类型的纯右值核心常量表达式

### C++14 增强

- 允许在 constexpr 函数中使用循环和局部变量修改
- 放宽了对对象修改的限制
- 改进了常量表达式的定义，引入"允许结果"的概念

### C++20 重大扩展

- 允许 constexpr 函数中调用虚函数
- 支持 dynamic_cast（在某些条件下）
- 允许 constexpr 动态内存分配
- 支持 std::vector 和 std::string 在 constexpr 上下文中的使用
- 引入立即函数（Immediate Function）概念
- 允许在 constexpr 中使用 new/delete（需在求值期间释放）

### C++26 进一步完善

- 允许从 void* 转换到其他指针类型
- 支持 constexpr 异常（throw/catch）
- 允许 placement new/new[]
- 改进了常量表达式的定义框架，引入"组成值"（Constituent Value）概念

### 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|----------|-----|------|------|
| `__cpp_constexpr_in_decltype` | `201711L` | C++20 | 常量求值所需时生成函数和变量定义 |
| `__cpp_constexpr_dynamic_alloc` | `201907L` | C++20 | constexpr 函数中的动态存储期操作 |
| `__cpp_constexpr` | `202306L` | C++26 | 从 void* 的 constexpr 转换 |
| `__cpp_constexpr` | `202406L` | C++26 | constexpr placement new 和 new[] |
| `__cpp_constexpr_exceptions` | `202411L` | C++26 | constexpr 异常 |

---

## 3. 语法与参数（Syntax and Parameters）

### 3.1 字面量类型定义

以下类型统称为**字面量类型（Literal Type）**：

1. **可能带有 cv 限定符的 void**
2. **标量类型（Scalar Type）**
   - 算术类型（整型、浮点型）
   - 枚举类型
   - 指针类型
   - 指向成员指针类型
3. **引用类型**
4. **字面量类型的数组**
5. **满足以下条件的可能带有 cv 限定符的类类型：**
   - 具有平凡析构函数（C++20 前）/ constexpr 析构函数（C++20 起）
   - 所有非静态非变体数据成员和基类都是非 volatile 的字面量类型
   - 属于以下类型之一：
     - 闭包类型（C++17 起）
     - 联合体聚合类型，满足：
       - 没有变体成员，或
       - 至少有一个非 volatile 字面量类型的变体成员
     - 非联合体聚合类型，且每个匿名联合体成员满足上述条件
     - 具有至少一个 constexpr 构造函数（模板）的类型（非拷贝/移动构造函数）

### 3.2 核心常量表达式

**核心常量表达式**是其求值**不会**求值以下任何语言构造的表达式：

#### 禁止的语言构造

| 语言构造 | 版本 | 相关提案 |
|---------|------|---------|
| this 指针（除特定情况外） | | N2235 |
| 通过静态或线程存储期块变量的声明 | C++23 | P2242R3 |
| 调用未声明为 constexpr 的函数 | | |
| 调用声明但未定义的 constexpr 函数 | | |
| constexpr 函数模板实例化失败 | | |
| constexpr 虚函数调用（对象动态类型为 constexpr-unknown） | | |
| 超过实现定义的限制 | | |
| 导致未定义行为的表达式 | | |
| lambda 表达式 | C++17 前 | |
| 特定对象上的左值到右值转换 | | |
| 访问联合体的非活跃成员 | | |
| 值不确定的对象上的左值到右值转换 | | |
| reinterpret_cast | | |
| 伪析构函数调用 | C++20 前 | |
| 递增/递减运算符 | C++14 前 | |
| 修改生命周期始于表达式外的对象 | C++14 起 | |
| 对生命周期始于表达式外的对象调用析构函数 | C++20 起 | |
| 多态类型的 typeid 表达式（某些情况） | | |
| new 表达式（特定条件除外） | C++20 起 | |
| delete 表达式（特定条件除外） | C++20 起 | |
| 协程的 await/yield 表达式 | C++20 起 | |
| 结果未指定的三路比较 | C++20 起 | |
| 结果未指定的相等/关系运算符 | | |
| 赋值/复合赋值运算符 | C++14 前 | |
| throw 表达式 | C++26 前 | |
| asm 声明 | | |
| va_arg 宏调用 | | |
| goto 语句 | | |
| lambda 内捕获外部变量（某些情况） | | |

#### 详细示例

**1. 函数调用约束**

```cpp
constexpr int n = std::numeric_limits<int>::max(); // 正确：max() 是 constexpr
constexpr int m = std::time(nullptr);              // 错误：std::time() 不是 constexpr
```

**2. 未定义行为检查**

```cpp
constexpr double d1 = 2.0 / 1.0;                    // 正确
constexpr double d2 = 2.0 / 0.0;                     // 错误：未定义
constexpr int n = std::numeric_limits<int>::max() + 1; // 错误：溢出

int x, y, z[30];
constexpr auto e1 = &y - &x;        // 错误：未定义行为
constexpr auto e2 = &z[20] - &z[3]; // 正确：同一数组内

constexpr std::bitset<2> a;
constexpr bool b = a[2]; // UB，但是否检测未指定
```

**3. 左值到右值转换**

```cpp
int main()
{
    const std::size_t tabsize = 50;
    int tab[tabsize]; // 正确：tabsize 是常量表达式
                      // 因为 tabsize 可用于常量表达式
                      // 它具有 const 限定的整型类型，且
                      // 其初始化器是常量初始化器

    std::size_t n = 50;
    const std::size_t sz = n;
    int tab2[sz]; // 错误：sz 不是常量表达式
                  // 因为 sz 不能用于常量表达式
                  // 因为其初始化器不是常量初始化器
}
```

**4. 对象修改（C++14）**

```cpp
constexpr int incr(int& n)
{
    return ++n;
}

constexpr int g(int k)
{
    constexpr int x = incr(k); // 错误：incr(k) 不是核心常量表达式
                               // 因为 k 的生命周期始于 incr(k) 之外
    return x;
}

constexpr int h(int k)
{
    int x = incr(k); // 正确：x 不需要用核心常量表达式初始化
    return x;
}

constexpr int y = h(1); // 正确：用值 2 初始化 y
                        // h(1) 是核心常量表达式因为
                        // k 的生命周期始于 h(1) 内部
```

**5. 异常处理（C++26）**

```cpp
constexpr void check(int i)
{
    if (i < 0)
        throw i;
}

constexpr bool is_ok(int i)
{
    try {
        check(i);
    } catch (...) {
        return false;
    }
    return true;
}

constexpr bool always_throw()
{
    throw 12;
    return true;
}

static_assert(is_ok(5));       // 正确
static_assert(!is_ok(-1));     // C++26 起正确
static_assert(always_throw()); // 错误：未捕获的异常
```

**6. Lambda 捕获约束**

```cpp
void g()
{
    const int n = 0;

    constexpr int j = *&n; // 正确：不在 lambda 表达式内

    [=]
    {
        constexpr int i = n;   // 正确：'n' 未被 odr 使用且未在此捕获
        constexpr int j = *&n; // 错误：'&n' 会是 'n' 的 odr 使用
    };
}
```

### 3.3 整型常量表达式

**整型常量表达式（Integral Constant Expression）**是整型或无作用域枚举类型的表达式，隐式转换为纯右值，且转换后的表达式是核心常量表达式。

如果类类型表达式用在需要整型常量表达式的地方，该表达式会上下文隐式转换为整型或无作用域枚举类型。

```cpp
enum class Color : int { Red, Green, Blue };

constexpr Color c = Color::Red;
int arr[(int)c]; // 正确：(int)c 是整型常量表达式
```

### 3.4 转换常量表达式

**类型 T 的转换常量表达式（Converted Constant Expression）**是隐式转换为类型 T 的表达式，其中：
- 转换后的表达式是常量表达式
- 隐式转换序列只包含：
  - constexpr 用户定义转换
  - 左值到右值转换
  - 整型提升
  - 非窄化整型转换
  - 浮点提升
  - 非窄化浮点转换
  - (C++17 起) 数组到指针转换
  - (C++17 起) 函数到指针转换
  - (C++17 起) 函数指针转换
  - (C++17 起) 限定转换
  - (C++17 起) 从 std::nullptr_t 的空指针转换
  - (C++17 起) 从 std::nullptr_t 的空成员指针转换
- 如果发生引用绑定，只能是直接绑定

**需要转换常量表达式的上下文：**
- case 标签的常量表达式
- 底层类型固定时的枚举器初始化器
- 整型和枚举（C++17 前）的非类型模板参数
- (C++14 起) 数组边界
- (C++14 起) new 表达式中除第一维外的维度
- (C++26 起) 包索引表达式和说明符的索引

**布尔类型的上下文转换常量表达式：**

需要布尔类型上下文转换常量表达式的上下文：
- noexcept 规范
- (C++23 前) static_assert 声明
- (C++17-C++23) constexpr if 语句
- (C++20 起) 条件 explicit 说明符

### 3.5 常量初始化

#### C++26 前

变量或临时对象 obj 是**常量初始化的（constant-initialized）**，如果满足以下条件：
- 它有初始化器，或其类型是 const 默认可构造的
- 其初始化的完整表达式是常量表达式（在要求常量表达式的上下文中），但如果 obj 是对象，该完整表达式也可以对 obj 及其子对象调用 constexpr 构造函数，即使这些对象是非字面量类类型

#### C++26 起

变量 var 是**常量可初始化的（constant-initializable）**，如果满足以下条件：
- 其初始化的完整表达式是常量表达式（在要求常量表达式的上下文中）
- 紧接 var 的初始化声明之后，var 声明的对象或引用是 constexpr 可表示的
- 如果 var 声明的对象或引用 x 具有静态或线程存储期，x 在 var 的初始化声明后的最近命名空间作用域点是 constexpr 可表示的

常量可初始化变量是**常量初始化的**，如果：
- 它有初始化器，或其类型是 const 默认可构造的

### 3.6 可用于常量表达式的实体

变量是**潜在常量的（potentially-constant）**，如果它是：
- constexpr 变量，或
- 具有引用或非 volatile const 限定整型或枚举类型

#### C++26 前

对象或引用在点 P **可用于常量表达式**，如果它是以下之一：
- 在 P 可用于常量表达式的变量
- 生命周期延长到在 P 可用于常量表达式的变量的非 volatile const 限定字面量类型的临时对象
- 模板参数对象
- 字符串字面量对象
- 上述任一的非 mutable 子对象
- 上述任一的引用成员

#### C++26 起

对象或引用在点 P **潜在可用于常量表达式**，如果它是以下之一：
- 在 P 可用于常量表达式的变量
- 生命周期延长到在 P 可用于常量表达式的变量的非 volatile const 限定字面量类型的临时对象
- 模板参数对象
- 字符串字面量对象
- 上述任一的非 mutable 子对象
- 上述任一的引用成员

对象或引用在点 P **可用于常量表达式**，如果它是潜在可用于常量表达式的对象或引用，且在 P 是 constexpr 可表示的。

### 3.7 显式常量求值表达式

以下表达式（包括到目标类型的转换）是**显式常量求值的（manifestly constant-evaluated）**：

- 数组边界
- new 表达式中除第一维外的维度
- 位域长度
- 枚举初始化器
- 对齐说明符
- case 标签的常量表达式
- 非类型模板参数
- noexcept 规范中的表达式
- static_assert 声明中的表达式
- constexpr 变量的初始化器
- constexpr if 语句的条件
- 条件 explicit 说明符中的表达式
- 立即调用
- 概念定义、嵌套要求和 requires 子句中的约束表达式（确定约束是否满足时）
- 引用类型或 const 限定整型/枚举类型变量的初始化器（仅当初始化器是常量表达式时）
- 静态和线程局部变量的初始化器（仅当初始化器的所有子表达式都是常量表达式时）

求值是否发生在显式常量求值上下文中可以通过 `std::is_constant_evaluated()` 和 `if consteval`（C++23 起）检测。

### 3.8 常量求值所需的函数和变量

以下表达式或转换是**潜在常量求值的（potentially constant evaluated）**：
- 显式常量求值表达式
- 潜在求值表达式
- 花括号初始化列表的直接子表达式（可能需要常量求值来确定转换是否窄化）
- 模板实体内出现的取址表达式（可能需要常量求值来确定是否值依赖）
- 上述的子表达式（非嵌套未求值操作数的子表达式）

函数是**常量求值所需的（needed for constant evaluation）**，如果它是 constexpr 函数且被潜在常量求值的表达式命名。

变量是**常量求值所需的**，如果它是：
- constexpr 变量，或
- 非 volatile const 限定整型或引用类型的变量，且其标识符表达式是潜在常量求值的

如果函数或变量是常量求值所需的，则会触发默认函数的定义或函数模板特化/变量模板特化（C++14 起）的实例化。

### 3.9 常量子表达式

**常量子表达式（Constant Subexpression）**是其作为表达式 e 的子表达式求值不会阻止 e 成为核心常量表达式的表达式，其中 e 不是以下表达式：
- throw 表达式
- (C++20 起) yield 表达式
- 赋值表达式
- 逗号表达式

---

## 4. 底层原理（Underlying Principles）

### 4.1 编译期求值机制

常量表达式的求值发生在编译期，由编译器内置的解释器执行：

**执行模型：**
1. **词法分析**：识别表达式中的各个成分
2. **语法分析**：构建表达式树
3. **语义分析**：检查类型、作用域、访问权限
4. **常量求值**：执行计算并验证约束
5. **结果存储**：将结果嵌入生成的代码中

### 4.2 核心常量表达式的判断标准

编译器在确定表达式是否为核心常量表达式时，采用以下策略：

**静态检查：**
- 检查是否包含禁止的语言构造
- 验证类型约束
- 检查声明可见性

**动态检查（编译期）：**
- 执行函数调用
- 检查未定义行为
- 验证运行时约束（如数组边界）

**抽象机模型：**
在常量求值期间，编译器模拟一个抽象机：
- 对象具有明确的存储期和生命周期
- 引用具有明确的绑定
- 所有操作必须产生确定的结果

### 4.3 constexpr 函数的编译期执行

当 constexpr 函数在常量求值上下文中调用时：

```cpp
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int f5 = factorial(5); // 编译期计算
```

编译器会：
1. 展开函数调用
2. 在编译期创建函数栈帧（抽象）
3. 执行函数体
4. 验证所有操作都是常量表达式允许的
5. 返回结果

### 4.4 constexpr-unknown 实体

在常量求值过程中，某些对象的动态类型被标记为 **constexpr-unknown**：

```cpp
constexpr void process(const Base& base) {
    // 在这里，base 的动态类型可能是 constexpr-unknown
    // 如果它引用的对象不能用于常量表达式
}
```

这适用于：
- 不能用于常量表达式的对象
- 引用绑定的对象（当引用本身不能用于常量表达式时）

### 4.5 组成值与组成引用（C++26）

**组成值（Constituent Values）**定义：
- 如果对象 obj 具有标量类型，组成值就是 obj 的值
- 否则，组成值是 obj 的任何直接子对象（非活跃联合体成员除外）的组成值

**组成引用（Constituent References）**定义：
- obj 的任何引用类型的直接成员
- obj 的任何直接子对象（非活跃联合体成员除外）的组成引用

### 4.6 constexpr 可表示性（C++26）

**constexpr 可引用（constexpr-referenceable）**：
- 静态存储期的对象在程序中的任何点都是 constexpr 可引用的
- 自动存储期的对象 obj 从点 P 是 constexpr 可引用的，如果变量 var 的最小封闭作用域和 P 的最小封闭作用域是不与 requires 表达式的参数列表关联的同一函数参数作用域，其中 var 是 obj 对应的变量或其生命周期延长到的变量

**constexpr 可表示（constexpr-representable）**：
对象或引用 x 在点 P 是 constexpr 可表示的，如果满足：
- x 的每个组成值如果指向对象 obj，则 obj 从 P 是 constexpr 可引用的
- x 的每个组成值如果指向对象末尾之后，则该对象从 P 是 constexpr 可引用的
- x 的每个组成引用如果引用对象 obj，则 obj 从 P 是 constexpr 可引用的

### 4.7 性能特征

**编译期计算的开销：**
- **编译时间**：增加编译时间，但减少运行时间
- **内存使用**：编译器需要维护抽象执行状态
- **代码大小**：结果内联，可能增加代码大小

**优化收益：**
- 消除运行时计算
- 启用更多编译期优化
- 允许更好的类型安全
- 支持静态断言和模板元编程

**实现限制：**
编译器通常会施加以下限制：
- 递归深度限制
- 求值步数限制
- 内存分配限制
- 模板实例化深度限制

---

## 5. 使用场景（Use Cases）

### 5.1 模板元编程

常量表达式是模板元编程的基础：

```cpp
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

constexpr int f5 = Factorial<5>::value; // 编译期计算：120
```

### 5.2 编译期断言

使用 static_assert 进行编译期验证：

```cpp
constexpr int buffer_size = 1024;
static_assert(buffer_size > 0, "Buffer size must be positive");
static_assert(buffer_size % 16 == 0, "Buffer size must be 16-byte aligned");
```

### 5.3 数组边界和位域

```cpp
constexpr int max_items = 100;
int buffer[max_items]; // 使用常量表达式作为数组边界

struct Flags {
    unsigned int readable : 1;
    unsigned int writable : 1;
    unsigned int executable : 1;
};

constexpr int flag_width = 1; // 位域长度需要常量表达式
```

### 5.4 枚举器初始化

```cpp
constexpr int base_value = 10;
enum class Status {
    OK = base_value,
    Error = base_value + 1,
    Unknown = base_value + 2
};
```

### 5.5 非类型模板参数

```cpp
template<int Size>
class FixedBuffer {
    char data[Size];
public:
    constexpr int size() const { return Size; }
};

constexpr int default_size = 256;
FixedBuffer<default_size> buffer; // 使用常量表达式作为模板参数
```

### 5.6 constexpr 函数

定义可在编译期执行的函数：

```cpp
constexpr int square(int x) {
    return x * x;
}

constexpr int sq5 = square(5); // 编译期求值
int runtime_value = 5;
int sq_runtime = square(runtime_value); // 运行时求值
```

### 5.7 字面量类型类

```cpp
class Complex {
    double re, im;
public:
    constexpr Complex(double r = 0, double i = 0) : re(r), im(i) {}
    constexpr double real() const { return re; }
    constexpr double imag() const { return im; }
    constexpr Complex operator+(const Complex& other) const {
        return Complex(re + other.re, im + other.im);
    }
};

constexpr Complex c1(1.0, 2.0);
constexpr Complex c2(3.0, 4.0);
constexpr Complex c3 = c1 + c2; // 编译期复数运算
```

### 5.8 编译期数据结构（C++20）

```cpp
#include <vector>
#include <algorithm>

constexpr std::vector<int> sort_values(std::vector<int> v) {
    std::sort(v.begin(), v.end());
    return v;
}

constexpr auto sorted = sort_values({3, 1, 4, 1, 5, 9, 2, 6});
// sorted 在编译期完成排序
```

### 5.9 编译期字符串处理

```cpp
constexpr size_t str_length(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

constexpr auto msg = "Hello, World!";
constexpr auto len = str_length(msg); // 编译期计算：13
```

### 5.10 最佳实践

**优先使用 constexpr：**

```cpp
// 好：可以在编译期或运行期使用
constexpr int compute(int x) {
    return x * 2 + 1;
}

// 不如上面灵活
int compute_runtime(int x) {
    return x * 2 + 1;
}
```

**区分编译期和运行期：**

```cpp
constexpr int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

// 编译期使用
constexpr int p2_10 = power(2, 10);

// 运行期使用
int base = get_base();
int exp = get_exp();
int p_runtime = power(base, exp);
```

**使用 std::is_constant_evaluated()：**

```cpp
#include <type_traits>

constexpr double compute(double x) {
    if (std::is_constant_evaluated()) {
        // 编译期路径：避免昂贵的操作
        return x * x;
    } else {
        // 运行期路径：可以使用库函数
        return std::sqrt(x * x);
    }
}
```

### 5.11 常见陷阱

**陷阱 1：constexpr 函数中的未定义行为**

```cpp
constexpr int bad_divide(int x, int y) {
    return x / y; // 如果 y == 0，编译期会报错
}

constexpr int result = bad_divide(10, 0); // 编译错误！
```

**陷阱 2：生命周期问题**

```cpp
constexpr int& get_ref() {
    int x = 42;
    return x; // 错误：返回局部变量的引用
}

constexpr int value = get_ref(); // 编译错误！
```

**陷阱 3：volatile 变量**

```cpp
volatile int vi = 42;
constexpr int* p = &vi; // 错误：volatile 变量不能用于常量表达式
```

**陷阱 4：lambda 捕获限制**

```cpp
void test() {
    int x = 10;
    auto lambda = [x]() constexpr {
        return x * 2;
    };

    constexpr auto result = lambda(); // 正确：x 被捕获

    auto lambda2 = [&x]() constexpr {
        return x * 2;
    };
    constexpr auto result2 = lambda2(); // 错误：引用捕获的 x 不能用于常量表达式
}
```

**陷阱 5：联合体的非活跃成员**

```cpp
union U {
    int i;
    float f;
};

constexpr U u{42};
constexpr float value = u.f; // 错误：访问非活跃成员
```

---

## 6. 代码示例（Examples）

### 6.1 基础用法

**示例 1：简单的 constexpr 变量**

```cpp
#include <iostream>

constexpr int max_size = 100;
constexpr double pi = 3.14159265358979;

int main() {
    int array[max_size]; // 正确：使用常量表达式作为数组大小
    std::cout << "PI = " << pi << std::endl;
    return 0;
}
```

**示例 2：constexpr 函数**

```cpp
#include <iostream>

constexpr int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    constexpr int fib10 = fibonacci(10); // 编译期计算
    std::cout << "fib(10) = " << fib10 << std::endl; // 输出：55

    int n;
    std::cin >> n;
    int fib_n = fibonacci(n); // 运行时计算
    std::cout << "fib(" << n << ") = " << fib_n << std::endl;

    return 0;
}
```

**示例 3：constexpr 类**

```cpp
#include <iostream>

class Point {
    int x_, y_;
public:
    constexpr Point(int x = 0, int y = 0) : x_(x), y_(y) {}
    constexpr int x() const { return x_; }
    constexpr int y() const { return y_; }
    constexpr Point operator+(const Point& other) const {
        return Point(x_ + other.x_, y_ + other.y_);
    }
};

int main() {
    constexpr Point p1(1, 2);
    constexpr Point p2(3, 4);
    constexpr Point p3 = p1 + p2;

    static_assert(p3.x() == 4, "p3.x should be 4");
    static_assert(p3.y() == 6, "p3.y should be 6");

    std::cout << "p3 = (" << p3.x() << ", " << p3.y() << ")" << std::endl;
    return 0;
}
```

### 6.2 高级用法

**示例 4：编译期字符串处理**

```cpp
#include <iostream>
#include <array>

constexpr size_t str_len(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

constexpr bool str_eq(const char* s1, const char* s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        ++s1;
        ++s2;
    }
    return *s1 == *s2;
}

int main() {
    constexpr auto len = str_len("Hello");
    static_assert(len == 5, "String length should be 5");

    constexpr auto eq = str_eq("test", "test");
    static_assert(eq, "Strings should be equal");

    return 0;
}
```

**示例 5：编译期计算（C++14）**

```cpp
#include <iostream>

constexpr int gcd(int a, int b) {
    while (b != 0) {
        auto temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

constexpr int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

int main() {
    constexpr int g = gcd(48, 18);
    constexpr int l = lcm(4, 6);

    static_assert(g == 6, "GCD should be 6");
    static_assert(l == 12, "LCM should be 12");

    std::cout << "GCD(48, 18) = " << g << std::endl;
    std::cout << "LCM(4, 6) = " << l << std::endl;

    return 0;
}
```

**示例 6：constexpr 动态内存分配（C++20）**

```cpp
#include <iostream>
#include <memory>

constexpr int* allocate_and_fill(int size, int value) {
    int* ptr = new int[size];
    for (int i = 0; i < size; ++i) {
        ptr[i] = value + i;
    }
    return ptr;
}

constexpr void deallocate(int* ptr) {
    delete[] ptr;
}

constexpr int compute_sum(int size) {
    int* arr = allocate_and_fill(size, 10);
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
    }
    deallocate(arr); // 必须在常量求值期间释放
    return sum;
}

int main() {
    constexpr int sum = compute_sum(5);
    static_assert(sum == 10 + 11 + 12 + 13 + 14, "Sum should be 60");
    std::cout << "Sum = " << sum << std::endl;
    return 0;
}
```

**示例 7：constexpr 异常处理（C++26）**

```cpp
#include <iostream>
#include <stdexcept>

constexpr int safe_divide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero");
    }
    return a / b;
}

constexpr int divide_with_check(int a, int b) {
    try {
        return safe_divide(a, b);
    } catch (const std::exception& e) {
        return -1;
    }
}

int main() {
    constexpr int r1 = divide_with_check(10, 2);
    static_assert(r1 == 5, "Result should be 5");

    constexpr int r2 = divide_with_check(10, 0);
    static_assert(r2 == -1, "Should return -1 for division by zero");

    std::cout << "10 / 2 = " << r1 << std::endl;
    std::cout << "10 / 0 handled = " << r2 << std::endl;

    return 0;
}
```

**示例 8：模板元编程与常量表达式**

```cpp
#include <iostream>
#include <array>

template<typename T, std::size_t N>
constexpr std::array<T, N> make_sequence(T start, T step) {
    std::array<T, N> result{};
    for (std::size_t i = 0; i < N; ++i) {
        result[i] = start + static_cast<T>(i) * step;
    }
    return result;
}

template<std::size_t N>
constexpr std::size_t sum_array(const std::array<int, N>& arr) {
    std::size_t sum = 0;
    for (std::size_t i = 0; i < N; ++i) {
        sum += arr[i];
    }
    return sum;
}

int main() {
    constexpr auto seq = make_sequence<int, 5>(1, 2);
    // seq = {1, 3, 5, 7, 9}

    constexpr auto total = sum_array(seq);
    static_assert(total == 25, "Sum should be 25");

    std::cout << "Sequence: ";
    for (auto val : seq) {
        std::cout << val << " ";
    }
    std::cout << "\nSum = " << total << std::endl;

    return 0;
}
```

### 6.3 常见错误与修正

**错误 1：非 constexpr 函数调用**

```cpp
// 错误示例
int get_value() { return 42; } // 非 constexpr 函数
constexpr int value = get_value(); // 编译错误！

// 修正：将函数声明为 constexpr
constexpr int get_value() { return 42; }
constexpr int value = get_value(); // 正确
```

**错误 2：使用运行时值作为常量表达式**

```cpp
// 错误示例
int main() {
    int n;
    std::cin >> n;
    int arr[n]; // 错误：n 不是常量表达式

    return 0;
}

// 修正：使用 constexpr 变量或动态分配
int main() {
    constexpr int size = 100;
    int arr[size]; // 正确

    // 或者
    int n;
    std::cin >> n;
    auto arr = std::make_unique<int[]>(n);

    return 0;
}
```

**错误 3：constexpr 函数中的禁止操作**

```cpp
// 错误示例（C++14 前）
constexpr int increment(int& x) {
    return ++x; // C++11 中错误：修改参数
}

// 修正：C++14 及以后允许
constexpr int increment(int x) {
    return x + 1; // 返回新值
}
```

**错误 4：静态存储期对象的限制**

```cpp
// 错误示例
int global_var = 42;
constexpr int* ptr = &global_var; // C++20 前错误：非 constexpr 变量的地址

// 修正：使用 constexpr 变量或 const 全局变量
constexpr int global_const = 42;
constexpr int* ptr = const_cast<int*>(&global_const); // 仍需注意

// 或使用局部变量
constexpr int local_f() {
    int x = 42;
    return x; // 正确：生命周期在求值期间
}
```

**错误 5：void* 转换（C++26 前）**

```cpp
// 错误示例（C++26 前）
constexpr void* ptr = nullptr;
constexpr int* iptr = static_cast<int*>(ptr); // C++26 前错误

// 修正：C++26 允许，或避免使用 void*
constexpr int* iptr = nullptr; // 直接使用空指针
```

---

## 7. 总结（Summary）

### 7.1 核心要点

1. **定义**：常量表达式是可以在编译期求值的表达式，可用于数组边界、模板参数、静态断言等场景

2. **类型要求**：只有字面量类型的对象才能在常量表达式中创建，包括：
   - 标量类型（算术、枚举、指针、成员指针）
   - 引用类型
   - 字面量类型的数组
   - 满足特定条件的类类型

3. **核心约束**：核心常量表达式不能包含：
   - 未声明为 constexpr 的函数调用
   - 未定义行为
   - reinterpret_cast
   - goto 语句
   - 以及其他禁止的语言构造

4. **版本演进**：
   - C++98：仅支持整型常量表达式，受限较多
   - C++11：引入 constexpr，支持 constexpr 函数
   - C++14：放宽限制，允许循环和局部变量修改
   - C++20：大幅扩展，支持动态内存分配、虚函数、std::vector/std::string
   - C++26：支持异常、void* 转换、placement new

### 7.2 技术对比

| 特性 | C++98 | C++11 | C++14 | C++20 | C++26 |
|-----|-------|-------|-------|-------|-------|
| 整型常量表达式 | ✓ | ✓ | ✓ | ✓ | ✓ |
| constexpr 函数 | ✗ | ✓ | ✓ | ✓ | ✓ |
| 函数内循环 | ✗ | ✗ | ✓ | ✓ | ✓ |
| 局部变量修改 | ✗ | ✗ | ✓ | ✓ | ✓ |
| 动态内存分配 | ✗ | ✗ | ✗ | ✓ | ✓ |
| 虚函数调用 | ✗ | ✗ | ✗ | ✓ | ✓ |
| 异常处理 | ✗ | ✗ | ✗ | ✗ | ✓ |
| void* 转换 | ✗ | ✗ | ✗ | ✗ | ✓ |
| placement new | ✗ | ✗ | ✗ | ✗ | ✓ |

### 7.3 学习建议

**初学者路径：**
1. 理解常量表达式的基本概念和用途
2. 掌握 constexpr 变量和函数的定义
3. 学习字面量类型的要求
4. 实践简单的编译期计算（如斐波那契数列、阶乘）

**进阶路径：**
1. 深入理解核心常量表达式的约束
2. 学习 constexpr 类的设计
3. 掌握模板元编程与常量表达式的结合
4. 了解 C++20/26 的新特性

**高级主题：**
1. 常量求值的底层实现机制
2. 编译期优化策略
3. constexpr 容器和算法
4. 反射与常量表达式（未来标准）

### 7.4 实践建议

**优先使用 constexpr：**
- 对所有可能在编译期使用的函数和变量添加 constexpr
- constexpr 函数既可在编译期也可在运行期使用

**注意版本差异：**
- 不同 C++ 标准对常量表达式的支持不同
- 使用特性测试宏（如 `__cpp_constexpr`）检测编译器支持

**避免常见错误：**
- 确保所有 constexpr 函数调用都是有效路径
- 注意生命周期和作用域问题
- 避免未定义行为

**性能考量：**
- 合理使用编译期计算，避免过度编译时间开销
- 对于复杂计算，考虑运行时与编译期的平衡
- 使用 `std::is_constant_evaluated()` 区分路径

### 7.5 相关主题

- `constexpr` 说明符
- 字面量类型
- 模板元编程
- 静态断言（static_assert）
- 编译期编程
- C++20 立即函数（consteval）
- C++23 if consteval

---

## 参考资料

- [C++ Reference: Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression)
- C++ 标准文档（ISO/IEC 14882）
- [C++20 特性：constexpr 动态内存分配](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0784r7.html)
- [C++26 特性：constexpr 异常](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3068r2.html)