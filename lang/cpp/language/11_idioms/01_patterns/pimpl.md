# PImpl - 实现指针惯用法

## 1. 概述

**PImpl**（Pointer to Implementation，实现指针）是一种 C++ 编程技术，通过将类的实现细节放入单独的类中，并通过不透明指针（opaque pointer）访问，从而将这些实现细节从类的对象表示中移除。

### 核心概念

PImpl 惯用法将类分为两个部分：
- **接口类（Interface Class）**：包含公共成员、受保护成员和虚函数，对外可见
- **实现类（Implementation Class）**：包含私有数据成员和非虚私有成员函数，隐藏在实现文件中

### 基本结构

```cpp
// --------------------
// 接口文件 (widget.h)
struct widget
{
    // 公共成员
private:
    struct impl;                          // 实现类的前向声明
    std::experimental::propagate_const<   // const 转发指针包装器
        std::unique_ptr<                  // 独占所有权不透明指针
            impl>> pImpl;                 // 指向前向声明的实现类
};

// ---------------------------
// 实现文件 (widget.cpp)
struct widget::impl
{
    // 实现细节
};
```

### 主要用途

1. **构建稳定的 ABI（Application Binary Interface）**：允许库更新而不破坏二进制兼容性
2. **减少编译依赖**：实现细节的更改不会触发用户代码的重新编译
3. **隐藏实现细节**：将私有成员从公共头文件中移除

## 2. 来源与演变

### 历史背景

PImpl 惯用法也被称为 "Cheshire Cat" 或 "Compilation Firewall"（编译防火墙），是 C++ 社区长期使用的经典设计模式。

### 设计动机

在传统 C++ 类设计中，私有成员存在以下问题：

| 问题 | 影响 |
|------|------|
| 私有数据成员参与对象表示 | 影响类的大小和内存布局 |
| 私有成员函数参与重载决议 | 在成员访问检查之前进行 |
| 实现细节变化 | 需要重新编译所有使用该类的代码 |

PImpl 通过将私有成员移至实现类，解决了这些编译依赖问题。

### C++ 标准演进

- **C++11 前**：使用原始指针和手动内存管理
- **C++11**：引入 `std::unique_ptr`，简化 PImpl 实现
- **C++17**：`std::experimental::propagate_const` 提供 const 正确性支持
- **C++20 及以后**：持续优化，与模块（Modules）等特性配合使用

### 替代方案对比

| 方案 | 描述 | 优缺点 |
|------|------|--------|
| 内联实现 | 私有成员和公共成员在同一类中 | 简单，但无 ABI 稳定性 |
| 纯抽象类（OOP 工厂） | 用户获得指向轻量或抽象基类的指针 | ABI 稳定，但有 vtable 依赖 |
| PImpl | 通过不透明指针访问实现类 | ABI 稳定，无隐藏依赖 |

## 3. 语法与参数

### 基本模板结构

```cpp
// 接口类定义
class Widget {
public:
    // 公共成员函数声明
    Widget();
    explicit Widget(int value);
    ~Widget();

    Widget(Widget&&);
    Widget& operator=(Widget&&);

    // 禁用复制（或实现深拷贝）
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

    void doSomething();
    void doSomething() const;

private:
    class impl;                                    // 前向声明
    std::unique_ptr<impl> pImpl;                   // 不透明指针
    // 或使用 propagate_const 保证 const 正确性
    // std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
};
```

### 关键组件说明

| 组件 | 说明 |
|------|------|
| `impl` 前向声明 | 在接口类中声明，在实现文件中定义 |
| `std::unique_ptr` | 管理实现对象的生命周期 |
| `propagate_const` | 确保 const 成员函数调用 const 实现 |
| 转发函数 | 接口类中的函数将调用转发给实现类 |

### 特殊成员函数要求

由于 `std::unique_ptr` 要求在实例化删除器时指向类型必须是完整类型，因此：

```cpp
// 必须在实现文件中定义（impl 是完整类型的地方）
Widget::~Widget() = default;
Widget::Widget(Widget&&) = default;
Widget& Widget::operator=(Widget&&) = default;
```

### propagate_const 的作用

```cpp
// 没有 propagate_const 的问题
class Widget {
    std::unique_ptr<impl> pImpl;
public:
    void doWork() const {
        pImpl->doWork();  // 调用的是非 const 版本！
    }
};

// 使用 propagate_const 解决
class Widget {
    std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
public:
    void doWork() const {
        pImpl->doWork();  // 正确调用 const 版本
    }
};
```

## 4. 底层原理

### 内存布局

PImpl 改变了类的内存布局：

```
传统类布局:
+------------------+
| 公共成员         |
| 受保护成员       |
| 私有成员         |  <- 所有成员都在对象中
+------------------+

PImpl 类布局:
+------------------+
| 公共成员         |
| 受保护成员       |
| pImpl 指针       |  <- 只有一个指针
+------------------+
         |
         v
+------------------+
| 私有数据成员     |  <- 实现对象（堆上）
| 私有成员函数     |
+------------------+
```

### 编译防火墙机制

PImpl 实现编译防火墙的原理：

1. **头文件只包含前向声明**：`struct impl;` 不需要知道 `impl` 的完整定义
2. **实现细节在 .cpp 文件中**：只有实现文件需要看到 `impl` 的完整定义
3. **更改实现类不影响头文件**：修改 `impl` 的成员只需重新编译 .cpp 文件

### 运行时开销

| 开销类型 | 描述 |
|----------|------|
| 访问开销 | 每次访问私有成员都需要一次指针间接访问 |
| 空间开销 | 接口类增加一个指针大小；实现类可能需要反向引用指针 |
| 生命周期开销 | 堆分配/释放实现对象的开销 |
| 移动语义 | 移动操作只需转移指针，效率高 |

### 编译时优化

- **链接时优化（LTO）**：可以内联跨翻译单元的间接调用
- **去虚拟化**：PImpl 不使用虚函数，比 OOP 工厂更易优化

### 模板类中的 PImpl

当实现类是模板特化时，编译防火墙优势会丢失：

```cpp
// 问题：模板特化需要完整定义
template<class T>
class Container {
    struct impl;  // 如果 impl 依赖 T，用户必须看到完整定义
    std::unique_ptr<impl> pImpl;
};
```

解决方案：使用非模板核心实现：

```cpp
// 非模板基类提供 ABI 稳定接口
class ContainerBase {
    struct impl;  // 不依赖 T
    std::unique_ptr<impl> pImpl;
protected:
    void push_back_fwd(void*);
};

template<class T>
class Container : private ContainerBase {
public:
    void push_back(T* p) { push_back_fwd(p); }
};
```

## 5. 使用场景

### 适合使用 PImpl 的场景

| 场景 | 原因 |
|------|------|
| 库开发 | 需要 ABI 稳定性，允许库更新不破坏用户代码 |
| 隐藏实现细节 | 私有成员包含敏感信息或复杂依赖 |
| 减少头文件依赖 | 私有成员依赖的类型只需在实现文件中包含 |
| 加速编译 | 减少头文件包含，降低编译依赖链 |
| 跨平台开发 | 不同平台可有不同实现，接口统一 |

### 不适合使用 PImpl 的场景

| 场景 | 原因 |
|------|------|
| 头文件库（Header-only） | PImpl 需要独立的翻译单元 |
| 性能关键代码 | 额外的指针间接访问开销 |
| 简单值类型 | 增加复杂性，收益有限 |
| 需要内联优化的函数 | 跨翻译单元的函数难以内联 |

### 最佳实践

1. **使用 `std::unique_ptr` 管理生命周期**
2. **在实现文件中定义特殊成员函数**
3. **使用 `propagate_const` 保证 const 正确性**
4. **考虑禁用复制或实现深拷贝**
5. **为需要访问公共成员的私有函数传递反向引用**

### 常见陷阱

1. **在头文件中定义析构函数**：导致 `unique_ptr` 无法删除不完整类型
2. **忘记 const 传播**：const 成员函数可能调用非 const 实现
3. **移动后使用**：移动后的对象 pImpl 为空，调用成员函数是未定义行为
4. **默认构造后使用**：如果默认构造不初始化 pImpl，使用是未定义行为

## 6. 代码示例

### 基础用法

```cpp
// ----------------------
// 接口文件 (widget.hpp)
#include <memory>
#include <iostream>

class Widget {
public:
    Widget();
    explicit Widget(int value);
    ~Widget();

    Widget(Widget&& other) noexcept;
    Widget& operator=(Widget&& other) noexcept;

    // 禁用复制
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

    void draw() const;
    void draw();

private:
    class impl;
    std::unique_ptr<impl> pImpl;
};

// ---------------------------
// 实现文件 (widget.cpp)
#include "widget.hpp"

class Widget::impl {
public:
    int value;

    impl(int v) : value(v) {}

    void draw() const {
        std::cout << "Drawing widget with value: " << value << std::endl;
    }

    void draw() {
        std::cout << "Drawing non-const widget with value: " << value << std::endl;
    }
};

Widget::Widget() : pImpl(std::make_unique<impl>(0)) {}
Widget::Widget(int value) : pImpl(std::make_unique<impl>(value)) {}
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::draw() const { pImpl->draw(); }
void Widget::draw() { pImpl->draw(); }

// ---------------------------
// 用户代码 (main.cpp)
#include "widget.hpp"

int main() {
    Widget w1(42);
    const Widget w2(100);

    w1.draw();   // 非常量版本
    w2.draw();   // const 版本

    Widget w3 = std::move(w1);  // 移动构造
    w3.draw();

    return 0;
}
```

### 高级用法：带反向引用的 PImpl

```cpp
// ----------------------
// 接口文件 (widget.hpp)
#include <experimental/propagate_const>
#include <memory>

class Widget {
public:
    void draw() const;
    void draw();
    bool shown() const { return true; }  // 公共 API，实现需要调用

    Widget();
    explicit Widget(int n);
    ~Widget();

    Widget(Widget&&);
    Widget(const Widget&) = delete;
    Widget& operator=(Widget&&);
    Widget& operator=(const Widget&) = delete;

private:
    class impl;
    std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
};

// ---------------------------
// 实现文件 (widget.cpp)
#include "widget.hpp"
#include <iostream>

class Widget::impl {
    int n;
public:
    impl(int n) : n(n) {}

    // 接收反向引用以访问公共成员
    void draw(const Widget& w) const {
        if (w.shown()) {  // 调用公共成员函数
            std::cout << "drawing a const widget " << n << '\n';
        }
    }

    void draw(const Widget& w) {
        if (w.shown()) {
            std::cout << "drawing a non-const widget " << n << '\n';
        }
    }
};

void Widget::draw() const { pImpl->draw(*this); }
void Widget::draw() { pImpl->draw(*this); }

Widget::Widget() = default;
Widget::Widget(int n) : pImpl(std::make_unique<impl>(n)) {}
Widget::Widget(Widget&&) = default;
Widget::~Widget() = default;
Widget& Widget::operator=(Widget&&) = default;

// ---------------------------
// 用户代码 (main.cpp)
#include "widget.hpp"

int main() {
    Widget w(7);
    const Widget w2(8);
    w.draw();    // 输出: drawing a non-const widget 7
    w2.draw();   // 输出: drawing a const widget 8
}
```

### 模板类中的 PImpl

```cpp
// ---------------------
// 头文件 (ptr_vector.hpp)
#include <memory>

// 非模板基类提供 ABI 稳定接口
class ptr_vector_base {
    struct impl;  // 不依赖 T
    std::unique_ptr<impl> pImpl;

protected:
    void push_back_fwd(void* p);
    void print() const;

public:
    ptr_vector_base();
    ~ptr_vector_base();
};

template<class T>
class ptr_vector : private ptr_vector_base {
public:
    void push_back(T* p) { push_back_fwd(p); }
    void print() const { ptr_vector_base::print(); }
};

// -----------------------
// 源文件 (ptr_vector.cpp)
#include "ptr_vector.hpp"
#include <iostream>
#include <vector>

struct ptr_vector_base::impl {
    std::vector<void*> vp;

    void push_back(void* p) {
        vp.push_back(p);
    }

    void print() const {
        for (void const* const p : vp) {
            std::cout << p << '\n';
        }
    }
};

void ptr_vector_base::push_back_fwd(void* p) { pImpl->push_back(p); }
ptr_vector_base::ptr_vector_base() : pImpl(std::make_unique<impl>()) {}
ptr_vector_base::~ptr_vector_base() {}
void ptr_vector_base::print() const { pImpl->print(); }

// ---------------------------
// 用户代码 (main.cpp)
#include "ptr_vector.hpp"

int main() {
    int x{}, y{}, z{};
    ptr_vector<int> v;
    v.push_back(&x);
    v.push_back(&y);
    v.push_back(&z);
    v.print();
}
```

### 常见错误及修正

#### 错误 1：在头文件中定义析构函数

```cpp
// 错误：在 impl 不完整时定义析构函数
class Widget {
    class impl;
    std::unique_ptr<impl> pImpl;
public:
    ~Widget() = default;  // 错误！impl 是不完整类型
};

// 修正：在实现文件中定义析构函数
// widget.hpp
class Widget {
    class impl;
    std::unique_ptr<impl> pImpl;
public:
    ~Widget();  // 仅声明
};

// widget.cpp
Widget::~Widget() = default;  // impl 在此处是完整类型
```

#### 错误 2：忘记 const 传播

```cpp
// 错误：const 成员函数调用非 const 实现
class Widget {
    class impl;
    std::unique_ptr<impl> pImpl;  // 非 const 传播
public:
    void process() const {
        pImpl->process();  // 调用非 const 版本！
    }
};

// 修正：使用 propagate_const
class Widget {
    class impl;
    std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
public:
    void process() const {
        pImpl->process();  // 正确调用 const 版本
    }
};
```

#### 错误 3：移动后使用

```cpp
// 错误：移动后使用对象
Widget w1(42);
Widget w2 = std::move(w1);
w1.draw();  // 未定义行为！w1.pImpl 为空

// 修正：检查有效性或避免使用移动后的对象
Widget w1(42);
Widget w2 = std::move(w1);
// 不再使用 w1，或添加有效性检查
```

## 7. 总结

### 核心要点

PImpl 惯用法是 C++ 中实现**编译防火墙**和**ABI 稳定性**的重要技术：

| 特性 | 说明 |
|------|------|
| 编译隔离 | 实现细节更改不触发用户代码重编译 |
| ABI 稳定 | 库可更新实现而不破坏二进制兼容性 |
| 隐藏细节 | 私有成员从头文件中移除 |
| 减少依赖 | 头文件依赖移至实现文件 |

### 技术对比

| 方面 | PImpl | OOP 工厂 | 内联实现 |
|------|-------|----------|----------|
| ABI 稳定性 | 优秀 | 良好（vtable 限制） | 无 |
| 编译隔离 | 优秀 | 良好 | 无 |
| 运行时开销 | 指针间接 | 虚函数调用 | 无 |
| 代码复杂度 | 中等 | 较高 | 低 |
| 移动语义 | 友好 | 需要额外处理 | 自然 |

### 使用建议

1. **库开发优先考虑**：开发需要长期维护的库时，PImpl 提供重要的 ABI 稳定性
2. **权衡开销**：性能关键代码需评估指针间接访问的开销
3. **正确实现**：使用 `unique_ptr` + `propagate_const` + 实现文件定义特殊成员函数
4. **模板类注意**：模板类中使用 PImpl 需要特殊处理以保持编译防火墙

### 相关资源

- GotW #28: The Fast Pimpl Idiom
- GotW #100: Compilation Firewalls
- C++ Core Guidelines: T.61, T.84
- cppreference: https://en.cppreference.com/w/cpp/language/pimpl