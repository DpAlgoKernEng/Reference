# 移动构造函数 (Move Constructor)

## 1. 概述 (Overview)

移动构造函数 (Move Constructor) 是 C++11 引入的一种特殊构造函数，它接受同类型对象作为参数，通过"移动"而非"复制"的方式转移资源所有权。移动构造函数的参数类型为右值引用 (rvalue reference)，通常形式为 `T(T&& other)`。

与拷贝构造函数不同，移动构造函数可以修改参数对象的状态，将其持有的资源（如动态分配的内存、文件描述符、TCP 套接字、线程句柄等）转移到新创建的对象中，而源对象则被置于有效但未定义的状态。

**核心特点**：
- 参数类型为右值引用 `T&&`
- 可实现高效的资源转移
- 避免不必要的深拷贝操作
- 支持异常安全性优化

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++11 之前，对象传递只能通过拷贝构造函数完成。对于持有大量资源的对象（如动态数组、字符串等），拷贝操作会带来显著的性能开销。C++11 引入移动语义 (move semantics)，通过移动构造函数实现资源的转移而非复制。

### 设计动机

1. **性能优化**：避免临时对象产生的不必要拷贝开销
2. **资源管理**：支持不可复制资源（如 `std::unique_ptr`、文件句柄）的所有权转移
3. **语义清晰**：明确区分"复制"和"移动"两种不同的操作语义

### 版本变更

| C++ 版本 | 变更内容 |
|---------|---------|
| C++11 | 首次引入移动构造函数和移动语义 |
| C++17 | 复制消除 (copy elision) 成为强制性优化，prvalue 不再触发移动构造函数调用 |
| C++20 | 引入"合格构造函数" (eligible constructor) 概念，优化重载决议规则 |
| C++23 | 隐式定义的移动构造函数可以是 `constexpr` 函数 |

### 缺陷报告 (Defect Reports)

| DR 编号 | 适用版本 | 已发布行为 | 修正行为 |
|---------|---------|-----------|---------|
| CWG 1353 | C++11 | 默认移动构造函数删除条件未考虑多维数组类型 | 考虑这些类型 |
| CWG 1402 | C++11 | 调用非平凡拷贝构造函数的默认移动构造函数被删除；已删除的移动构造函数仍参与重载决议 | 允许调用此类拷贝构造函数；在重载决议中被忽略 |
| CWG 1491 | C++11 | 含有右值引用类型的非静态数据成员的默认移动构造函数被删除 | 此情况下不删除 |
| CWG 2094 | C++11 | volatile 子对象使默认移动构造函数非平凡 | 不影响平凡性 |
| CWG 2595 | C++20 | 存在更约束但不满足约束的移动构造函数时，移动构造函数不合格 | 此情况下可以合格 |

## 3. 语法与参数 (Syntax and Parameters)

### 声明语法

```cpp
// (1) 类内声明
class-name(parameter-list);

// (2) 类内定义
class-name(parameter-list) function-body

// (3) 显式默认声明
class-name(single-parameter-list) = default;

// (4) 删除声明
class-name(parameter-list) = delete;

// (5) 类外定义
class-name::class-name(parameter-list) function-body

// (6) 类外显式默认
class-name::class-name(single-parameter-list) = default;
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `class-name` | 正在声明移动构造函数的类名 |
| `parameter-list` | 非空参数列表，满足以下条件：<br>1. 第一个参数类型为 `T&&`、`const T&&`、`volatile T&&` 或 `const volatile T&&`<br>2. 要么没有其他参数，要么其他参数都有默认实参 |
| `single-parameter-list` | 仅包含一个参数的参数列表，类型为 `T&&`、`const T&&`、`volatile T&&` 或 `const volatile T&&`，且无默认实参 |
| `function-body` | 移动构造函数的函数体 |

### 语法说明

1. **声明 (1)**：在类定义内部声明移动构造函数
2. **类内定义 (2-4)**：在类定义内部定义移动构造函数
3. **显式默认 (3, 6)**：使用 `= default` 显式要求编译器生成默认实现
4. **删除 (4)**：使用 `= delete` 禁止移动构造函数的生成
5. **类外定义 (5, 6)**：在类定义外部定义移动构造函数（类内必须有声明）

### 示例

```cpp
struct X
{
    X(X&& other); // 移动构造函数
//  X(X other);   // 错误：参数类型不正确
};

union Y
{
    Y(Y&& other, int num = 1); // 带多个参数的移动构造函数
//  Y(Y&& other, int num);     // 错误：`num` 没有默认实参
};
```

## 4. 底层原理 (Underlying Principles)

### 调用时机

移动构造函数在以下场景被调用：

1. **对象初始化**：`T a = std::move(b);` 或 `T a(std::move(b));`，其中 `b` 类型为 `T`
2. **函数参数传递**：`f(std::move(a));`，其中 `a` 类型为 `T`，函数签名为 `void f(T t)`
3. **函数返回**：在返回类型为 `T` 的函数中执行 `return a;`，其中 `a` 类型为 `T`

**注意**：当初始化器为 prvalue（纯右值）时，C++17 起移动构造函数调用被完全避免（强制复制消除）；C++17 之前则常被优化消除。

### 隐式声明条件

若满足以下所有条件，编译器将为类类型隐式声明移动构造函数：

1. 没有用户声明的拷贝构造函数
2. 没有用户声明的拷贝赋值运算符
3. 没有用户声明的移动赋值运算符
4. 没有用户声明的析构函数

隐式声明的移动构造函数签名为 `T::T(T&&)`，为非显式 (non-explicit) 的 inline 公共成员。

### 隐式定义行为

若隐式声明的移动构造函数既未删除也非平凡，编译器将在 ODR 使用或常量求值需要时生成函数体：

- **联合体类型**：按 `std::memmove` 方式复制对象表示
- **非联合体类类型**：按声明顺序对所有直接基类子对象和非静态数据成员进行逐成员移动构造，使用 xvalue 实参进行直接初始化
- **引用类型成员**：绑定到源引用所引用的同一对象或函数

### 删除条件

隐式声明或显式默认的移动构造函数在以下情况下被定义为删除：

1. 类 `T` 的某个潜在构造子对象 `M` 具有删除或不可访问的析构函数
2. 对子对象 `M` 的移动构造函数重载决议：
   - 未找到可用候选
   - 若子对象为变体成员 (variant member)，选择了非平凡函数

被删除的移动构造函数不参与重载决议（否则会阻止从右值进行拷贝初始化）。

### 平凡性条件

移动构造函数在满足以下所有条件时为平凡 (trivial)：

1. 非用户提供的（隐式定义或默认的）
2. 类没有虚成员函数
3. 类没有虚基类
4. 每个直接基类的移动构造函数都是平凡的
5. 每个非静态类类型成员的移动构造函数都是平凡的

**平凡移动构造函数**执行与平凡拷贝构造函数相同的操作，即通过 `std::memmove` 复制对象表示。所有与 C 语言兼容的数据类型都是平凡可移动的。

### 合格性条件

| 版本 | 条件 |
|------|------|
| C++20 前 | 移动构造函数未删除即合格 |
| C++20 起 | 满足以下条件：<br>1. 未删除<br>2. 关联约束（如有）满足<br>3. 没有更约束的移动构造函数满足其约束 |

合格移动构造函数的平凡性决定类是否为隐式生存期类型 (implicit-lifetime type) 和平凡可复制类型 (trivially copyable type)。

### 异常规范

隐式声明或首次声明显式默认的移动构造函数具有 `noexcept` 规范（C++17 起）。

## 5. 使用场景 (Use Cases)

### 适用场景

1. **资源所有权转移**：管理独占资源（如 `std::unique_ptr`、文件句柄、线程）
2. **性能关键代码**：避免大型对象的深拷贝开销
3. **容器操作**：`std::vector` 扩容时移动元素而非拷贝
4. **工厂模式返回值**：高效返回大对象

### 最佳实践

1. **声明为 noexcept**：为实现强异常保证，用户定义的移动构造函数不应抛出异常。`std::vector` 在重新分配元素时依赖 `std::move_if_noexcept` 在移动和拷贝之间选择。

```cpp
class Buffer {
public:
    Buffer(Buffer&& other) noexcept  // 推荐声明为 noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
private:
    int* data_;
    size_t size_;
};
```

2. **使用 std::move 转发成员**：对类类型成员使用 `std::move`，对非类类型成员使用直接赋值或 `std::exchange`。

3. **保持源对象有效性**：移动后的源对象应处于有效但未定义状态，可安全析构。

### 注意事项

1. **重载决议规则**：
   - 若同时提供拷贝和移动构造函数，右值选择移动构造函数，左值选择拷贝构造函数
   - 若仅提供拷贝构造函数（接受 `const` 引用），则所有参数类别都会选择它

2. **Rule of Five**：若定义了移动构造函数，通常也应定义移动赋值运算符、析构函数，并考虑拷贝语义。

3. **用户声明阻止隐式生成**：任何用户声明的拷贝构造函数、拷贝赋值运算符、移动赋值运算符或析构函数都会阻止隐式移动构造函数的生成。

### 常见陷阱

1. **析构函数阻止隐式移动**：声明析构函数会阻止隐式生成移动构造函数
2. **忘记置空源对象指针**：移动后源对象的指针未置空，导致双重释放
3. **对 const 对象或引用无法移动**：`const T&&` 和 `T&` 无法用于移动

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

struct A
{
    std::string s;
    int k;

    A() : s("test"), k(-1) {}
    A(const A& o) : s(o.s), k(o.k) { std::cout << "move failed!\n"; }
    A(A&& o) noexcept :
        s(std::move(o.s)),       // 显式移动类类型成员
        k(std::exchange(o.k, 0)) // 显式移动非类类型成员
    {}
};

A f(A a)
{
    return a;
}

struct B : A
{
    std::string s2;
    int n;
    // 隐式移动构造函数 B::(B&&)
    // 调用 A 的移动构造函数
    // 调用 s2 的移动构造函数
    // 对 n 进行位拷贝
};

struct C : B
{
    ~C() {} // 析构函数阻止隐式移动构造函数 C::(C&&)
};

struct D : B
{
    D() {}
    ~D() {}           // 析构函数会阻止隐式移动构造函数 D::(D&&)
    D(D&&) = default; // 强制生成移动构造函数
};

int main()
{
    std::cout << "Trying to move A\n";
    A a1 = f(A()); // 按值返回时移动构造目标对象
                   // 从函数参数移动

    std::cout << "Before move, a1.s = " << std::quoted(a1.s)
        << " a1.k = " << a1.k << '\n';

    A a2 = std::move(a1); // 从 xvalue 移动构造
    std::cout << "After move, a1.s = " << std::quoted(a1.s)
        << " a1.k = " << a1.k << '\n';


    std::cout << "\nTrying to move B\n";
    B b1;

    std::cout << "Before move, b1.s = " << std::quoted(b1.s) << "\n";

    B b2 = std::move(b1); // 调用隐式移动构造函数
    std::cout << "After move, b1.s = " << std::quoted(b1.s) << "\n";


    std::cout << "\nTrying to move C\n";
    C c1;
    C c2 = std::move(c1); // 调用拷贝构造函数

    std::cout << "\nTrying to move D\n";
    D d1;
    D d2 = std::move(d1);
}
```

**输出**：

```
Trying to move A
Before move, a1.s = "test" a1.k = -1
After move, a1.s = "" a1.k = 0

Trying to move B
Before move, b1.s = "test"
After move, b1.s = ""

Trying to move C
move failed!

Trying to move D
```

### 高级用法：资源管理类

```cpp
#include <iostream>
#include <algorithm>

class DynamicArray {
public:
    // 默认构造函数
    DynamicArray() : data_(nullptr), size_(0) {}

    // 带大小构造
    explicit DynamicArray(size_t size) : data_(new int[size]), size_(size) {
        std::fill(data_, data_ + size, 0);
    }

    // 析构函数
    ~DynamicArray() {
        delete[] data_;
    }

    // 拷贝构造函数
    DynamicArray(const DynamicArray& other)
        : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Copy constructor called\n";
    }

    // 移动构造函数
    DynamicArray(DynamicArray&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;  // 重要：置空源对象指针
        other.size_ = 0;
        std::cout << "Move constructor called\n";
    }

    // 拷贝赋值运算符
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    // 移动赋值运算符
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    size_t size() const { return size_; }
    int* data() { return data_; }
    const int* data() const { return data_; }

private:
    int* data_;
    size_t size_;
};

int main() {
    DynamicArray arr1(1000);
    std::cout << "arr1 size: " << arr1.size() << "\n";

    // 移动构造
    DynamicArray arr2 = std::move(arr1);
    std::cout << "arr2 size: " << arr2.size() << "\n";
    std::cout << "arr1 size after move: " << arr1.size() << "\n";  // 0

    return 0;
}
```

### 常见错误及修正

#### 错误 1：析构函数阻止隐式移动

```cpp
// 错误示例
struct Widget {
    std::string name;
    ~Widget() {}  // 用户声明的析构函数阻止隐式移动构造函数
};

Widget w1;
Widget w2 = std::move(w1);  // 调用拷贝构造函数，而非移动！

// 修正方案
struct WidgetFixed {
    std::string name;
    ~Widget() = default;
    WidgetFixed(WidgetFixed&&) = default;  // 显式默认移动构造函数
};
```

#### 错误 2：移动后使用源对象

```cpp
// 危险示例
std::unique_ptr<int> p1 = std::make_unique<int>(42);
std::unique_ptr<int> p2 = std::move(p1);
std::cout << *p1;  // 未定义行为！p1 已为空

// 正确做法
std::unique_ptr<int> p1 = std::make_unique<int>(42);
std::unique_ptr<int> p2 = std::move(p1);
if (p1) {  // 检查是否有效
    std::cout << *p1;
}
```

#### 错误 3：忘记移动基类和成员

```cpp
// 错误示例
class Base {
public:
    Base(Base&& other) : data_(std::move(other.data_)) {}
private:
    std::vector<int> data_;
};

class Derived : public Base {
public:
    Derived(Derived&& other)
        // 忘记移动基类！
        : name_(std::move(other.name_))  // 只移动了成员
    {}
private:
    std::string name_;
};

// 正确做法
class DerivedFixed : public Base {
public:
    DerivedFixed(DerivedFixed&& other)
        : Base(std::move(other))        // 移动基类
        , name_(std::move(other.name_)) // 移动成员
    {}
private:
    std::string name_;
};
```

## 7. 总结 (Summary)

### 核心要点

1. **定义**：移动构造函数接受右值引用参数 `T(T&&)`，实现资源转移而非复制
2. **性能优势**：避免深拷贝开销，特别适用于资源密集型对象
3. **隐式生成条件**：无用户声明的拷贝/移动操作和析构函数
4. **异常安全**：建议声明为 `noexcept` 以支持容器优化
5. **Rule of Five**：定义移动构造函数时应同时考虑其他特殊成员函数

### 技术对比

| 特性 | 拷贝构造函数 | 移动构造函数 |
|------|------------|------------|
| 参数类型 | `const T&` | `T&&` |
| 资源处理 | 深拷贝 | 资源转移 |
| 源对象状态 | 保持不变 | 有效但未定义 |
| 性能开销 | 可能较高 | 通常较低 |
| 隐式生成条件 | 无用户声明的拷贝操作、移动操作、析构函数 | 同左，且无用户声明的析构函数 |

### 学习建议

1. 理解左值、右值、xvalue、prvalue 等值类别概念
2. 掌握 `std::move` 的本质（类型转换为右值引用）
3. 实践资源管理类的完整"Rule of Five"实现
4. 了解 `std::move_if_noexcept` 在异常安全场景的应用
5. 研究标准库容器（如 `std::vector`、`std::string`）的移动语义实现

### 相关概念

- **转换构造函数 (Converting Constructor)**
- **拷贝赋值 (Copy Assignment)**
- **拷贝构造函数 (Copy Constructor)**
- **复制消除 (Copy Elision)**
- **默认构造函数 (Default Constructor)**
- **析构函数 (Destructor)**
- **移动赋值 (Move Assignment)**
- **初始化 (Initialization)**
  - 聚合初始化 (Aggregate Initialization)
  - 常量初始化 (Constant Initialization)
  - 拷贝初始化 (Copy Initialization)
  - 默认初始化 (Default Initialization)
  - 直接初始化 (Direct Initialization)
  - 初始化列表 (Initializer List)
  - 列表初始化 (List Initialization)
  - 引用初始化 (Reference Initialization)
  - 值初始化 (Value Initialization)
  - 零初始化 (Zero Initialization)

---

*参考来源：[cppreference - Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor)*