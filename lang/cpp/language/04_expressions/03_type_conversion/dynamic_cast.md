# `dynamic_cast` 转换

## 1. 概述 (Overview)

`dynamic_cast` 是 C++ 中用于安全地沿继承层次结构进行指针和引用转换的运算符。它支持向上转换（upcast）、向下转换（downcast）和侧向转换（sidecast），是 C++ 运行时类型识别（RTTI，Run-Time Type Identification）机制的核心组成部分。

`dynamic_cast` 的主要特点：
- **类型安全**：在运行时检查转换的有效性，避免不安全的类型转换
- **多态支持**：专门用于多态类型的转换
- **失败处理**：转换失败时指针返回空指针，引用抛出异常
- **继承层次**：支持单继承、多继承和虚继承的复杂场景

### 技术定位

`dynamic_cast` 属于 C++ 四种类型转换运算符之一（其他三种为 `static_cast`、`const_cast`、`reinterpret_cast`），专门处理具有继承关系的类类型之间的安全转换。它是在运行时而非编译时进行类型检查的转换方式。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`dynamic_cast` 在 C++ 早期发展阶段被引入，旨在解决继承层次结构中的类型安全转换问题。在 `dynamic_cast` 出现之前，开发者只能使用 C 风格的类型转换或 `static_cast`，这些方式在处理向下转换时无法保证类型安全。

### 设计动机

1. **安全性需求**：传统的 C 风格转换和 `static_cast` 在进行向下转换时不进行运行时检查，可能导致未定义行为
2. **多态支持**：面向对象编程中，基类指针指向派生类对象是常见模式，需要安全的方式"还原"为派生类类型
3. **复杂继承**：多继承和虚继承场景下，对象布局复杂，需要运行时信息才能正确处理转换

### 版本变更

| C++ 版本 | 变更内容 |
|---------|---------|
| C++98 | 引入 `dynamic_cast`，作为 RTTI 的一部分 |
| C++11 | 结果类型分类细化：lvalue reference 返回 lvalue，rvalue reference 返回 xvalue，pointer 返回 prvalue |
| C++17 | rvalue reference 的 expression 必须是 glvalue（prvalue 需要 materialization） |

### 缺陷报告修正

- **CWG 1269** (C++11)：修正了当 `target-type` 为右值引用类型时，对 xvalue 表达式不执行运行时检查的问题
- **CWG 2861** (C++98)：明确了当 expression 指向/引用类型不可访问对象时，行为未定义

---

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
dynamic_cast<target-type>(expression)
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `target-type` | 目标类型，可以是：指向完整类类型的指针、指向完整类类型的引用、或指向（可选 cv 限定的）void 的指针 |
| `expression` | 如果 `target-type` 是引用，则为完整类类型的 glvalue（C++11 前）| lvalue（C++11 起）；如果 `target-type` 是指针，则为指向完整类类型的指针 prvalue |

### 结果类型

| 条件 | 结果类型分类 |
|------|------------|
| C++11 前，`target-type` 是引用类型 | lvalue |
| C++11 前，`target-type` 是指针类型 | rvalue |
| C++11 起，`target-type` 是左值引用类型 | lvalue（expression 必须是 lvalue）|
| C++11 起，`target-type` 是右值引用类型 | xvalue |
| C++11 起，`target-type` 是指针类型 | prvalue |

---

## 4. 底层原理 (Underlying Principles)

### 运行时类型识别（RTTI）

`dynamic_cast` 的实现依赖于运行时类型识别机制，这需要编译器为每个多态类生成类型信息：

1. **type_info 对象**：每个多态类都有一个关联的 `std::type_info` 对象，存储类型名称等信息
2. **vtable 指针**：多态类对象包含指向虚函数表（vtable）的指针
3. **类型信息链接**：vtable 中包含指向 `type_info` 的指针

### 转换机制详解

#### 1. 相同类型或添加 cv 限定

```cpp
Derived* d = new Derived();
Derived* d2 = dynamic_cast<Derived*>(d);  // 返回原值
const Derived* cd = dynamic_cast<const Derived*>(d);  // 添加 const
```

此时无需运行时检查，直接返回调整类型后的值。

#### 2. 向上转换（Upcast）

```cpp
Derived* d = new Derived();
Base* b = dynamic_cast<Base*>(d);  // 派生类指针转基类指针
```

- 若 expression 为空指针，返回目标类型的空指针
- 否则返回指向 `Derived` 对象中唯一 `Base` 子对象的指针
- 此转换也可用隐式转换或 `static_cast` 完成

#### 3. 向下转换（Downcast）

```cpp
Base* b = /* 指向 Derived 对象的指针 */;
Derived* d = dynamic_cast<Derived*>(b);  // 运行时检查
```

运行时检查逻辑：
1. 通过 vtable 获取对象的实际类型信息
2. 检查目标类型 `Derived` 是否是对象实际类型的基类
3. 若是，计算正确的指针偏移并返回；否则返回 `nullptr`（指针）或抛出 `std::bad_cast`（引用）

#### 4. 侧向转换（Sidecast）

在多继承场景中，从一个基类转换到另一个基类：

```cpp
struct A : virtual V {};
struct B : virtual V {};
struct D : A, B {};

A* a = new D();
B* b = dynamic_cast<B*>(a);  // 侧向转换
```

运行时检查：
1. 获取对象的实际（最派生）类型
2. 检查是否存在唯一的目标类型子对象
3. 返回正确的子对象指针

#### 5. 转换到 `void*`

```cpp
V* v = /* 多态类型指针 */;
void* p = dynamic_cast<void*>(v);  // 返回最派生对象的指针
```

这会返回指向最派生对象（most derived object）起始位置的指针。

### 性能特征

| 方面 | 分析 |
|------|------|
| **时间复杂度** | O(1) 到 O(n)，取决于继承深度和复杂度；可能需要遍历继承链 |
| **空间开销** | 增加可执行文件大小（RTTI 信息）；每个多态类额外的类型信息 |
| **编译选项** | 许多编译器允许禁用 RTTI（如 GCC/Clang 的 `-fno-rtti`）以减小体积 |
| **替代方案** | `static_cast` 更快但安全性较低；自定义类型标记可替代 RTTI |

---

## 5. 使用场景 (Use Cases)

### 适用场景

#### 场景一：向下转换

当需要从基类指针/引用安全地获取派生类指针/引用时：

```cpp
Base* base = getObject();
if (Derived* derived = dynamic_cast<Derived*>(base)) {
    // 安全地使用 Derived 的功能
    derived->derivedMethod();
}
```

#### 场景二：侧向转换

在多继承中，从一个基类分支转换到另一个基类分支：

```cpp
struct Interface1 { virtual ~Interface1() = default; };
struct Interface2 { virtual void foo() = 0; };
class Implementation : public Interface1, public Interface2 {
    void foo() override {}
};

Interface1* i1 = new Implementation();
Interface2* i2 = dynamic_cast<Interface2*>(i1);  // 侧向转换
```

#### 场景三：获取最派生对象

使用 `void*` 目标类型获取对象的真实地址：

```cpp
struct V { virtual ~V() = default; };
struct A : virtual V {};
struct B : virtual V {};
struct D : A, B {};

V* v = new D();
void* p = dynamic_cast<void*>(v);  // p 指向 D 对象的起始位置
```

### 最佳实践

1. **优先使用引用版本的异常处理**：当转换失败应当视为错误时
   ```cpp
   try {
       Derived& derived = dynamic_cast<Derived&>(base);
       // 使用 derived
   } catch (const std::bad_cast& e) {
       // 处理转换失败
   }
   ```

2. **指针版本配合空指针检查**：当转换失败是正常情况时
   ```cpp
   if (Derived* derived = dynamic_cast<Derived*>(base)) {
       // 转换成功
   } else {
       // 处理转换失败（正常流程）
   }
   ```

3. **确保类型是多态的**：`dynamic_cast` 要求类型至少有一个虚函数

### 常见陷阱

#### 陷阱一：非多态类型

```cpp
// 错误：非多态类型无法使用 dynamic_cast
struct NonPolymorphic {};
struct Derived : NonPolymorphic {};

NonPolymorphic* np = new Derived();
// Derived* d = dynamic_cast<Derived*>(np);  // 编译错误！
```

**修正**：确保类型是多态的（至少有一个虚函数）

```cpp
struct Polymorphic { virtual ~Polymorphic() = default; };
struct Derived : Polymorphic {};

Polymorphic* p = new Derived();
Derived* d = dynamic_cast<Derived*>(p);  // 正确
```

#### 陷阱二：构造函数/析构函数中误用

```cpp
struct Base { virtual ~Base() = default; };
struct Derived : Base {
    Derived() {
        Base* b = this;
        // Derived* d = dynamic_cast<Derived*>(b);  // 未定义行为！
    }
};
```

**说明**：在构造或析构期间，对象被视为当前正在构造/析构的类的类型，而非最派生类型。

#### 陷阱三：私有/保护继承

```cpp
class Base { virtual void f() {} };
class Derived : private Base {};  // 私有继承

Base* b = new Derived();
// Derived* d = dynamic_cast<Derived*>(b);  // 返回 nullptr！
```

**原因**：`dynamic_cast` 要求目标类型与对象类型之间存在公共继承关系。

#### 陷阱四：歧义转换

```cpp
struct A { virtual void f() {} };
struct B : A {};
struct C : A {};
struct D : B, C {};  // 菱形继承，A 有两个副本

A* a = new D();
// B* b = dynamic_cast<B*>(a);  // 歧义！a 指向哪个 A？
```

**修正**：使用虚继承消除歧义

```cpp
struct A { virtual void f() {} };
struct B : virtual A {};
struct C : virtual A {};
struct D : B, C {};  // 虚继承，A 只有一个副本
```

### 与其他转换的比较

| 转换方式 | 运行时检查 | 多态类型要求 | 性能 | 适用场景 |
|---------|-----------|-------------|------|---------|
| `dynamic_cast` | 是 | 是 | 较低 | 安全的向下/侧向转换 |
| `static_cast` | 否 | 否 | 高 | 已知安全的类型转换 |
| `reinterpret_cast` | 否 | 否 | 最高 | 底层重新解释 |
| C 风格转换 | 否 | 否 | 高 | 不推荐使用 |

---

## 6. 代码示例 (Examples)

### 基础用法：向下转换

```cpp
#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual ~Base() = default;  // 确保是多态类型
    virtual void name() { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    void name() override { std::cout << "Derived\n"; }
    void derivedOnly() { std::cout << "Derived-only method\n"; }
};

int main() {
    Base* b1 = new Base;
    Base* b2 = new Derived;

    // 尝试向下转换
    if (Derived* d = dynamic_cast<Derived*>(b1)) {
        d->derivedOnly();
    } else {
        std::cout << "b1 is not a Derived\n";
    }

    if (Derived* d = dynamic_cast<Derived*>(b2)) {
        d->derivedOnly();  // 会执行
    } else {
        std::cout << "b2 is not a Derived\n";
    }

    delete b1;
    delete b2;
    return 0;
}
```

**输出**：
```
b1 is not a Derived
Derived-only method
```

### 高级用法：侧向转换与引用版本

```cpp
#include <iostream>
#include <exception>

// 虚基类
struct V {
    virtual void f() {}  // 使 V 成为多态类型
    virtual ~V() = default;
};

struct A : virtual V { virtual void aMethod() { std::cout << "A method\n"; } };
struct B : virtual V { virtual void bMethod() { std::cout << "B method\n"; } };

struct D : A, B {
    void f() override { std::cout << "D::f\n"; }
};

int main() {
    D d;  // 最派生对象
    A& a = d;  // 向上转换（隐式）

    // 向下转换
    try {
        D& new_d = dynamic_cast<D&>(a);
        new_d.f();  // 输出 "D::f"
    } catch (const std::bad_cast& e) {
        std::cout << "Downcast failed: " << e.what() << "\n";
    }

    // 侧向转换：从 A 到 B
    try {
        B& new_b = dynamic_cast<B&>(a);
        new_b.bMethod();  // 输出 "B method"
    } catch (const std::bad_cast& e) {
        std::cout << "Sidecast failed: " << e.what() << "\n";
    }

    // 获取最派生对象地址
    V* v = &d;
    void* most_derived = dynamic_cast<void*>(v);
    std::cout << "Most derived object at: " << most_derived << "\n";

    return 0;
}
```

**输出**：
```
D::f
B method
Most derived object at: 0x... (地址值)
```

### 常见错误：引用转换失败

```cpp
#include <iostream>
#include <exception>
#include <typeinfo>

class Base { virtual void dummy() {} };
class Derived : public Base {};

int main() {
    Base b;  // Base 对象，非 Derived

    try {
        Derived& d = dynamic_cast<Derived&>(b);  // 抛出异常！
        std::cout << "This will not be printed\n";
    } catch (const std::bad_cast& e) {
        std::cout << "Caught: " << e.what() << "\n";
        std::cout << "Type: " << typeid(e).name() << "\n";
    }

    return 0;
}
```

**输出**：
```
Caught: std::bad_cast
Type: St8bad_cast
```

### 完整示例：构造函数中的转换

```cpp
#include <iostream>

struct V {
    virtual void f() {}  // 多态基类
    virtual ~V() = default;
};

struct A : virtual V {};

struct B : virtual V {
    B(V* v, A* a) {
        // 构造期间，对象被视为 B 类型
        // 从 V* 转换到 B* 是合法的（V 是 B 的基类）
        if (B* b = dynamic_cast<B*>(v)) {
            std::cout << "Cast from V* to B* succeeded\n";
        }

        // 从 A* 转换到 B* 是未定义行为（A 不是 B 的基类）
        // 即使最终对象会是 D（继承 B 和 A）
        // 在 B 的构造函数中，这仍然是 UB
        // auto b2 = dynamic_cast<B*>(a);  // 未定义行为！
    }
};

struct D : A, B {
    D() : B(static_cast<A*>(this), this) {
        std::cout << "D constructed\n";
    }
};

int main() {
    D d;
    return 0;
}
```

**输出**：
```
Cast from V* to B* succeeded
D constructed
```

---

## 7. 总结 (Summary)

### 核心要点

1. **用途**：`dynamic_cast` 用于沿继承层次结构安全地转换指针和引用
2. **多态要求**：仅适用于多态类型（至少有一个虚函数的类型）
3. **运行时检查**：向下转换和侧向转换在运行时进行类型安全验证
4. **失败处理**：指针版本返回 `nullptr`，引用版本抛出 `std::bad_cast`
5. **性能考量**：依赖 RTTI，有运行时开销；可通过编译选项禁用 RTTI

### 与 `static_cast` 的选择

| 情况 | 推荐使用 |
|------|---------|
| 向上转换（派生类到基类） | `static_cast` 或隐式转换 |
| 确定类型的向下转换 | `static_cast`（更高效）|
| 不确定类型的向下转换 | `dynamic_cast`（更安全）|
| 多继承侧向转换 | `dynamic_cast` |
| 非多态类型转换 | `static_cast` |

### 学习建议

1. **理解 RTTI 机制**：学习虚函数表、类型信息对象的工作原理
2. **掌握失败处理**：区分指针和引用版本的失败处理方式
3. **注意构造/析构限制**：了解在这些特殊成员函数中使用 `dynamic_cast` 的注意事项
4. **权衡性能与安全**：在性能敏感场景考虑 `static_cast` 配合自定义类型标记
5. **阅读标准文档**：参考 ISO C++ 标准中 `[expr.dynamic.cast]` 章节获取精确语义

### 参考链接

- C++23 标准：[expr.dynamic.cast]
- 相关运算符：`static_cast`、`const_cast`、`reinterpret_cast`
- 头文件：`<typeinfo>`（`std::bad_cast`、`std::type_info`）