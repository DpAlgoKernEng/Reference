# constexpr 说明符 (C++11 起)

## 1. 概述

`constexpr` 是 C++11 引入的关键字，用于声明变量、结构化绑定 (C++26 起) 或函数的值可以出现在常量表达式 (constant expression) 中。constexpr 说明符声明实体的值可以在编译期计算，从而允许这些实体用于只允许编译期常量表达式的场景。

### 核心定义

- **constexpr 变量**：编译期常量，值在编译期确定
- **constexpr 函数**：在常量表达式上下文中调用时可在编译期求值的函数
- **constexpr 构造函数**：允许在编译期构造对象
- **constexpr 析构函数** (C++20 起)：允许在编译期销毁对象

### 主要用途

1. 性能优化：将运行期计算转移到编译期
2. 类型安全：替代宏定义和魔法数字
3. 模板元编程：简化编译期计算
4. 常量初始化：确保静态存储期对象的常量初始化

## 2. 来源与演变

### 历史背景

在 C++11 之前，C++ 开发者面临以下问题：

1. **宏定义的局限**：`#define` 定义的常量没有类型安全，调试困难
2. **const 的不足**：`const` 变量的值不一定能在编译期确定
3. **模板元编程复杂**：编译期计算需要复杂的模板技巧

`constexpr` 的设计目标：
- 提供类型安全的编译期常量
- 简化编译期计算的语法
- 统一常量表达式的处理方式

### 版本演变

#### C++11 首次引入

- constexpr 变量和函数
- 严格的函数体限制：只允许单一 return 语句
- constexpr 构造函数
- constexpr 函数必须是 constexpr-suitable（字面类型参数和返回值）

#### C++14 放宽限制

- 允许局部变量声明和循环
- 允许多条语句
- constexpr 成员函数不再隐式 const
- 支持变量模板 (variable template)

#### C++17 增强

- constexpr lambda 表达式
- if constexpr 语句
- constexpr 函数隐式 inline

#### C++20 重大扩展

- 允许动态内存分配（在常量求值期间）
- 允许 try 块和 inline assembly
- constexpr 析构函数
- 允许虚函数为 constexpr
- 允许在 constexpr 函数中改变 union 的活跃成员
- 放宽对非平凡默认初始化的限制

#### C++23 进一步放宽

- 允许非字面类型变量
- 允许 goto 语句和标签
- 允许静态 constexpr 变量
- 移除返回类型和参数类型的字面类型要求

#### C++26 新特性

- 结构化绑定可声明为 constexpr
- 从 void* 的 constexpr 转换（支持类型擦除）

### 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|-----------|-----|------|------|
| `__cpp_constexpr` | 200704L | C++11 | constexpr |
| `__cpp_constexpr` | 201304L | C++14 | 放宽 constexpr，非 const constexpr 方法 |
| `__cpp_constexpr` | 201603L | C++17 | constexpr lambda |
| `__cpp_constexpr` | 201907L | C++20 | constexpr 函数中的平凡默认初始化和 asm 声明 |
| `__cpp_constexpr` | 202002L | C++20 | 常量求值中改变 union 活跃成员 |
| `__cpp_constexpr` | 202110L | C++23 | constexpr 函数中的非字面变量、标签和 goto 语句 |
| `__cpp_constexpr` | 202207L | C++23 | 放宽部分 constexpr 限制 |
| `__cpp_constexpr` | 202211L | C++23 | constexpr 函数中允许静态 constexpr 变量 |
| `__cpp_constexpr` | 202306L | C++26 | 从 void* 的 constexpr 转换 |
| `__cpp_constexpr_in_decltype` | 201711L | C++11 (DR) | 常量求值需要时生成函数和变量定义 |
| `__cpp_constexpr_dynamic_alloc` | 201907L | C++20 | constexpr 函数中的动态存储期操作 |

## 3. 语法与参数

### 基本语法

```cpp
constexpr 变量声明
constexpr 函数声明
constexpr 构造函数声明
constexpr 析构函数声明 (C++20 起)
```

### constexpr 变量

变量或变量模板 (C++14 起) 可以声明为 constexpr，需满足以下条件：

| 条件 | 说明 |
|------|------|
| 声明是定义 | 必须是定义而非声明 |
| 类型是字面类型 | 必须是字面类型 (literal type) |
| 初始化 | 必须被初始化 |
| 初始化表达式 | (C++26 前) 初始化的完整表达式必须是常量表达式 |
| 常量可初始化 | (C++26 起) 必须是常量可初始化的 |
| 常量析构 | (C++20 起) 必须具有常量析构函数 |

### constexpr 函数

函数或函数模板可以声明为 constexpr。函数是 **constexpr-suitable（符合 constexpr 要求）** 的条件：

#### 函数体限制

| 版本 | 限制 |
|------|------|
| C++11 | 函数体必须是 `= default`、`= delete` 或仅包含一条 return 语句的复合语句 |
| C++14 | 禁止 goto、标签（case/default 除外）、try 块、inline assembly、非字面类型变量定义、静态/线程存储期变量定义 |
| C++20 | 允许 try 块、inline assembly（但在常量表达式中仍不能执行）、允许虚函数 |
| C++23 | 允许非字面类型变量、静态 constexpr 变量、goto 语句和标签 |

#### constexpr-suitable 条件

- (C++20 前) 不能是虚函数
- (C++23 前) 返回类型和每个参数类型必须是字面类型
- (C++20 起) 不能是协程
- 如果是构造函数或析构函数，其类不能有虚基类

### constexpr 构造函数

除了满足 constexpr 函数的要求外，还需满足：

| 版本 | 额外条件 |
|------|---------|
| C++11 | 若类是 union，必须初始化恰好一个变体成员；若类是 union-like class，每个匿名 union 成员必须初始化恰好一个变体成员；每个非变体非静态数据成员和基类子对象必须初始化 |
| C++14 | 委托构造函数的目标必须是 constexpr 构造函数；非委托构造函数选择的每个成员和基类构造函数必须是 constexpr |
| C++23 | 移除初始化限制 |

### constexpr 析构函数 (C++20 起)

析构函数声明为 constexpr 需满足：

- 满足 constexpr 函数的要求
- (C++23 前) 每个子对象的类类型必须有 constexpr 析构函数
- 类不能有虚基类

### 隐式效果

1. **const 隐含**：constexpr 对象声明或非静态成员函数 (C++14 前) 隐含 const
2. **inline 隐含**：函数或静态数据成员的首次声明 (C++17 起) 隐含 inline
3. **声明一致性**：如果函数或函数模板的任何声明有 constexpr，则所有声明都必须有 constexpr

## 4. 底层原理

### 编译期求值机制

constexpr 函数具有双重特性：

```
constexpr int add(int a, int b) { return a + b; }

// 编译期求值
constexpr int x = add(1, 2);  // 编译期计算，x = 3

// 运行期求值
int a = 1, b = 2;
int y = add(a, b);  // 运行期计算
```

### 核心常量表达式

编译器在编译期求值时执行以下检查：

1. **禁止的操作**：
   - 访问 volatile 对象
   - 调用非 constexpr 函数
   - 抛出未捕获的异常 (C++26 前)
   - 执行 inline assembly
   - 无限循环

2. **允许的操作** (逐步放宽)：
   - 字面类型运算
   - 条件分支和循环
   - (C++20) 动态内存分配和释放
   - (C++20) 改变 union 活跃成员
   - (C++23) 非字面类型变量

### 编译器实现

```
源代码 -> constexpr 函数调用
           |
           v
    常量求值上下文？
           |
    +------+------+
    |             |
   Yes           No
    |             |
    v             v
编译期解释执行   生成运行期代码
    |
    v
结果嵌入目标代码
```

### 性能特征

| 特性 | 说明 |
|------|------|
| 编译时间 | 增加（编译期计算开销） |
| 运行时间 | 减少（计算已提前完成） |
| 代码大小 | 可能增加（结果内联） |
| 内存使用 | 编译期内存峰值可能增加 |

### 与 const 的区别

| 特性 | constexpr | const |
|------|-----------|-------|
| 值的确定时机 | 必须编译期确定 | 可在运行期确定 |
| 函数声明 | 可声明 constexpr 函数 | 可声明 const 成员函数（语义不同） |
| 初始化 | 必须初始化 | 可延迟初始化 |
| 用途 | 常量表达式上下文 | 只读语义 |

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 编译期计算 | 数学运算、查表、算法 |
| 模板参数 | 非类型模板参数 |
| 数组边界 | 定义编译期确定的数组大小 |
| 枚举值初始化 | 枚举值的编译期计算 |
| 常量初始化 | 避免静态初始化顺序问题 |
| 类型特征 | 类型萃取和编译期类型检查 |

### 最佳实践

1. **优先使用 constexpr**
   ```cpp
   // 推荐
   constexpr int kBufferSize = 1024;

   // 不推荐
   #define BUFFER_SIZE 1024
   const int kBufferSize = 1024;  // 可能不是编译期常量
   ```

2. **函数优先声明为 constexpr**
   ```cpp
   // 推荐：允许编译期和运行期两种求值方式
   constexpr int square(int x) { return x * x; }

   // 不推荐：限制只能运行期求值
   int square(int x) { return x * x; }
   ```

3. **使用 constexpr 构造函数创建字面类型**
   ```cpp
   struct Point {
       constexpr Point(double x, double y) : x_(x), y_(y) {}
       constexpr double distance() const { return x_ * x_ + y_ * y_; }
   private:
       double x_, y_;
   };
   ```

4. **利用 if constexpr 进行条件编译 (C++17)**
   ```cpp
   template<typename T>
   constexpr auto get_value(T t) {
       if constexpr (std::is_pointer_v<T>) {
           return *t;
       } else {
           return t;
       }
   }
   ```

### 常见陷阱

#### 陷阱 1：constexpr 函数不保证编译期求值

```cpp
constexpr int add(int a, int b) { return a + b; }

int x = 1, y = 2;
int z = add(x, y);  // 运行期求值！需要使用 constexpr 上下文强制编译期求值

// 强制编译期求值的方法
constexpr int z2 = add(1, 2);           // 方法 1
static_assert(add(1, 2) == 3);         // 方法 2
std::array<int, add(1, 2)> arr;        // 方法 3
```

#### 陷阱 2：C++11 的严格限制

```cpp
// C++11 错误：只能有一条 return 语句
constexpr int factorial(int n) {
    int result = 1;           // 错误：不允许局部变量
    for (int i = 2; i <= n; ++i)  // 错误：不允许循环
        result *= i;
    return result;
}

// C++11 正确：必须使用递归
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// C++14 及以后：允许循环和局部变量
constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i)
        result *= i;
    return result;
}
```

#### 陷阱 3：非 constexpr-suitable 函数

```cpp
// C++23 前：可能无法编译
constexpr void f(int& i) {
    static int x = 0;   // C++23 前：错误，禁止静态变量
    i = 0;
}
```

#### 陷阱 4：运行期与编译期行为不一致

```cpp
constexpr int div(int a, int b) {
    return b != 0 ? a / b : 0;  // 编译期检查
}

int x = 10, y = 0;
int result = div(x, y);  // 运行期：除零行为未定义
```

### 注意事项

1. **编译时间权衡**：大量 constexpr 计算会显著增加编译时间
2. **调试困难**：编译期错误信息可能难以理解
3. **编译器支持**：不同编译器对 constexpr 特性的支持程度不同
4. **代码膨胀**：constexpr 函数可能被内联到多处
5. **跨模块限制**：C++20 前，constexpr 变量不应引用翻译单元局部实体
6. **常量析构优化**：如果变量具有常量析构函数，即使其析构函数非平凡，也无需生成调用析构函数的机器码
7. **立即函数限制**：非 lambda、非特殊成员函数、非模板的 constexpr 函数不会隐式成为立即函数（immediate function），如需强制编译期求值需显式标记为 `consteval`（C++20 起）

### 检测编译期求值 (C++17 前)

C++17 前，可以使用 `noexcept` 运算符检测 constexpr 函数调用是否为常量表达式：

```cpp
constexpr int f();  // 声明但未定义
constexpr bool b1 = noexcept(f());  // false：未定义的 constexpr 函数

constexpr int f() { return 0; }     // 定义
constexpr bool b2 = noexcept(f());  // true：f() 是常量表达式
```

注意：此技巧在 C++17 后不再有效，因为 `noexcept` 对常量表达式不再总是返回 `true`。

## 6. 代码示例

### 基础用法：编译期阶乘计算

```cpp
#include <iostream>

// C++11 风格：递归
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// C++14 风格：循环和局部变量
constexpr int factorial_cxx14(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i)
        result *= i;
    return result;
}

int main() {
    // 编译期计算
    constexpr int fact4 = factorial(4);      // 编译期求值，结果为 24
    static_assert(fact4 == 24, "factorial(4) should be 24");

    // 用于模板参数
    std::array<int, factorial(5)> arr;       // 大小为 120 的数组

    // 运行期计算
    int n = 10;
    std::cout << factorial(n) << std::endl;  // 运行期求值

    return 0;
}
```

### 高级用法：字面类型与 constexpr 构造函数

```cpp
#include <stdexcept>
#include <cstddef>

// 字面类型：具有 constexpr 构造函数的类
class ConstString {
    const char* p_;
    std::size_t sz_;

public:
    template<std::size_t N>
    constexpr ConstString(const char(&a)[N]) : p_(a), sz_(N - 1) {}

    // C++11：使用条件运算符抛出异常
    constexpr char operator[](std::size_t n) const {
        return n < sz_ ? p_[n] : throw std::out_of_range("");
    }

    constexpr std::size_t size() const { return sz_; }
};

// C++11 风格：递归计算小写字母数
constexpr std::size_t countLower(ConstString s, std::size_t n = 0, std::size_t c = 0) {
    return n == s.size() ? c :
           ('a' <= s[n] && s[n] <= 'z' ? countLower(s, n + 1, c + 1)
                                       : countLower(s, n + 1, c));
}

// 编译期验证
static_assert(countLower("Hello, World!") == 9, "Should have 9 lowercase letters");

int main() {
    constexpr ConstString str("Hello, World!");
    static_assert(str.size() == 13, "Size should be 13");
    static_assert(str[0] == 'H', "First char should be 'H'");
    return 0;
}
```

### 高级用法：constexpr 引用与常量初始化

```cpp
// constexpr 引用
static constexpr int const& x = 42;  // constexpr 引用，绑定到 const int 对象

// 非字面类型的 constexpr 构造函数 (C++20)
// 允许在非字面类型上使用 constexpr 构造函数进行常量初始化
struct NonLiteral {
    constexpr NonLiteral() : value(42) {}  // 即使类不是字面类型
    int value;
};

// 编译期动态内存分配 (C++20)
constexpr int* make_array(int size) {
    int* arr = new int[size];  // 允许在 constexpr 中分配内存
    for (int i = 0; i < size; ++i)
        arr[i] = i * i;
    return arr;
}

constexpr int sum_squares(int size) {
    int* arr = make_array(size);
    int sum = 0;
    for (int i = 0; i < size; ++i)
        sum += arr[i];
    delete[] arr;  // 必须在 constexpr 求值期间释放
    return sum;
}

static_assert(sum_squares(5) == 30, "Sum of 0+1+4+9+16 = 30");
```

### 高级用法：if constexpr (C++17)

```cpp
#include <type_traits>

template<typename T>
constexpr T abs(T x) {
    if constexpr (std::is_signed_v<T>) {
        return x < 0 ? -x : x;
    } else {
        return x;  // 无符号类型，直接返回
    }
}

// 编译期类型判断
template<typename T>
constexpr const char* type_name() {
    if constexpr (std::is_integral_v<T>) {
        return "integral";
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating_point";
    } else {
        return "other";
    }
}

static_assert(abs(-5) == 5);
static_assert(abs(5u) == 5u);  // 无符号版本
```

### 常见错误及修正

#### 错误 1：constexpr 变量未在编译期初始化

```cpp
// 错误：非常量初始化
int runtime_value = 42;
constexpr int ce = runtime_value;  // 编译错误！

// 修正：使用 const 或常量表达式
const int cv = runtime_value;       // 正确
constexpr int ce2 = 42;             // 正确
```

#### 错误 2：C++11 中函数体限制

```cpp
// C++11 错误：多条语句、局部变量、循环
constexpr int sum(int n) {
    int s = 0;           // 错误：不允许局部变量
    for (int i = 1; i <= n; ++i)  // 错误：不允许循环
        s += i;
    return s;
}

// C++11 修正：使用递归
constexpr int sum(int n) {
    return n <= 0 ? 0 : n + sum(n - 1);
}

// C++14 及以后：可以使用循环和局部变量
constexpr int sum_cxx14(int n) {
    int s = 0;
    for (int i = 1; i <= n; ++i)
        s += i;
    return s;
}
```

#### 错误 3：constexpr 成员函数的 const 属性

```cpp
struct S {
    int value;
    // C++11 中，constexpr 成员函数隐式 const
    constexpr int get() const { return value; }  // 必须有 const

    // C++14 起，constexpr 不再隐式 const
    constexpr int get_v2() { return value; }  // C++14 起正确
};
```

#### 错误 4：运行期与编译期行为差异

```cpp
void f(int& i) { i = 0; }  // 非 constexpr 函数

constexpr void g(int& i) {
    f(i);  // C++23 前可在运行期调用，但不能在常量求值中调用
}

int main() {
    int x = 5;
    g(x);  // 运行期调用：正确，x 变为 0

    // constexpr int y = []() {
    //     int z = 5;
    //     g(z);
    //     return z;
    // }();  // 错误：常量求值中调用 g() 无法求值
}
```

#### 错误 5：C++23 中的非 constexpr-suitable 函数

C++23 起，可以编写调用永远无法在常量表达式中求值的 constexpr 函数：

```cpp
void f(int& i) { i = 0; }  // 非 constexpr 函数

constexpr void g(int& i) {  // C++23 起合法
    f(i);  // 无条件调用 f，永远不能是常量表达式
}
// 注意：此函数只能在运行期调用，不能用于常量求值上下文
```

## 7. 总结

### 核心要点

1. **双重求值特性**：constexpr 函数可在编译期或运行期求值
2. **逐步放宽限制**：从 C++11 的严格限制到 C++23 的几乎无限制
3. **类型安全**：替代宏定义，提供类型安全的编译期常量
4. **性能优化**：将计算从运行期转移到编译期

### 版本特性对比

| 特性 | C++11 | C++14 | C++17 | C++20 | C++23 |
|------|-------|-------|-------|-------|-------|
| 循环 | 否 | 是 | 是 | 是 | 是 |
| 局部变量 | 否 | 是 | 是 | 是 | 是 |
| 多条语句 | 否 | 是 | 是 | 是 | 是 |
| goto/标签 | 否 | 否 | 否 | 否 | 是 |
| try 块 | 否 | 否 | 否 | 是 | 是 |
| 动态分配 | 否 | 否 | 否 | 是 | 是 |
| 虚函数 | 否 | 否 | 否 | 是 | 是 |
| 非字面类型变量 | 否 | 否 | 否 | 否 | 是 |
| 静态变量 | 否 | 否 | 否 | 否 | 是 |
| 析构函数 | 否 | 否 | 否 | 是 | 是 |

### 使用建议

1. **默认使用 constexpr**：简单函数应声明为 constexpr
2. **避免编译时间过长**：复杂计算可能不适合 constexpr
3. **注意版本差异**：不同 C++ 版本限制不同
4. **使用 consteval (C++20)**：当需要强制编译期求值时
5. **使用 constinit (C++20)**：确保静态初始化

### 相关概念

| 概念 | 关系 |
|------|------|
| `consteval` (C++20) | 强制编译期求值，每次调用必须是常量表达式 |
| `constinit` (C++20) | 断言变量具有静态初始化 |
| `const` | 只读语义，不一定在编译期确定值 |
| 常量表达式 | 可在编译期求值的表达式 |
| 字面类型 | 可用于常量表达式的类型 |

### 缺陷报告

以下缺陷报告已追溯应用于先前发布的 C++ 标准：

| 编号 | 应用版本 | 原始行为 | 修正行为 |
|------|---------|---------|---------|
| CWG 1358 | C++11 | 模板化 constexpr 函数需要至少有一个有效的参数值 | 不再需要 |
| CWG 1359 | C++11 | constexpr union 构造函数必须初始化所有数据成员 | 非空 union 只需初始化一个数据成员 |
| CWG 1366 | C++11 | 函数体为 `= default` 或 `= delete` 的 constexpr 构造函数的类可以有虚基类 | 不能有虚基类 |
| CWG 1595 | C++11 | constexpr 委托构造函数要求所有相关构造函数都是 constexpr | 只要求目标构造函数是 constexpr |
| CWG 1712 | C++14 | constexpr 变量模板要求所有声明都包含 constexpr 说明符 | 不再要求 |
| CWG 1911 | C++11 | 非字面类型的 constexpr 构造函数不被允许 | 允许用于常量初始化 |
| CWG 2004 | C++11 | 包含 mutable 成员的 union 的复制/移动在常量表达式中被允许 | mutable 变体使隐式复制/移动无效 |
| CWG 2022 | C++98 | constexpr 和非 constexpr 函数是否产生相同结果可能依赖于是否执行复制省略 | 假设在常量表达式中总是执行复制省略 |
| CWG 2163 | C++14 | 即使 goto 语句被禁止，标签在 constexpr 函数中仍被允许 | 标签也被禁止 |
| CWG 2268 | C++11 | CWG 2004 的决议禁止了带有 mutable 成员的 union 的复制/移动 | 如果对象在常量表达式中创建则允许 |
| CWG 2278 | C++98 | CWG 2022 的决议无法实现 | 假设在常量表达式中从不执行复制省略 |
| CWG 2531 | C++11 | 如果非 inline 变量用 constexpr 重新声明则变为 inline | 变量不会变为 inline |

### 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/constexpr
- C++ Standard: [dcl.constexpr]
- Effective Modern C++, Scott Meyers, Item 15