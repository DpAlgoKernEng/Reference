# delete 表达式

## 1. 概述 (Overview)

delete 表达式 (delete expression) 用于销毁先前由 new 表达式 (new-expression) 分配的对象，并释放所获得的内存区域。它是 C++ 中手动管理动态内存的核心机制之一，与 new 表达式配对使用，构成了 C++ 手动内存管理的基础。

delete 表达式有两种形式：
- **单对象形式**：`delete ptr`，用于删除单个对象
- **数组形式**：`delete[] ptr`，用于删除对象数组

delete 表达式的结果类型始终为 void。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

delete 表达式自 C++ 诞生之初就存在，是为了配合 new 表达式进行手动内存管理而设计的。它解决了 C 语言中 `malloc/free` 无法自动调用构造函数和析构函数的问题。

### 版本变更

| 版本 | 变更内容 |
|------|---------|
| C++98 | 引入基本的 delete 和 delete[] 语法 |
| C++11 | 解决了 lambda 表达式紧跟 delete 的语法歧义问题 |
| C++14 | 改进了 new 表达式组合时的释放函数选择规则；增加了 size-aware 全局释放函数的选择逻辑 |
| C++17 | 引入对齐感知的释放函数 (alignment-aware deallocation functions)，支持 `std::align_val_t` 参数 |
| C++20 | 引入析构式删除 (destroying delete)，允许跳过析构函数调用 |
| C++26 | 将删除不完整类型（且完整类型有非平凡析构函数或释放函数）的行为从"未定义"改为"程序非法" |

### 缺陷报告

| DR | 版本 | 问题 | 修复 |
|----|------|------|------|
| CWG 288 | C++98 | 对于单对象形式，比较的是操作数的静态类型与动态类型 | 改为比较被删除对象的静态类型与其动态类型 |
| CWG 353 | C++98 | 析构函数抛出异常时是否会调用释放函数未指定 | 明确总是会调用释放函数 |
| CWG 599 | C++98 | 单对象形式可接受任何类型的空指针（包括函数指针） | 除了对象指针类型外，其他指针类型都被拒绝 |
| CWG 1642 | C++98 | 表达式可以是指针左值 | 不再允许 |
| CWG 2474 | C++98 | 删除指向相似但不同类型的对象指针会导致未定义行为 | 改为行为良好定义 |
| CWG 2624 | C++98 | 从非分配式 operator new[] 获得的指针可以传递给 delete[] | 禁止 |
| CWG 2758 | C++98 | 释放函数和析构函数的访问控制检查方式不清晰 | 予以明确 |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
::(可选) delete 表达式        // (1) 单对象形式
::(可选) delete[] 表达式      // (2) 数组形式
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `::` | 可选的作用域解析运算符。如果存在，则只查找全局释放函数 |
| 表达式 | 必须是以下之一：<br>- 可隐式转换为对象指针的类类型表达式<br>- 对象指针类型的纯右值 (prvalue) |

### 形式说明

1. **单对象形式**：销毁由 new 表达式创建的单个非数组对象
2. **数组形式**：销毁由 `new[]` 表达式创建的数组

## 4. 底层原理 (Underlying Principles)

### 执行流程

delete 表达式的执行分为两个主要阶段：

#### 阶段一：析构函数调用

如果被删除的对象不是空指针，且释放函数不是析构式删除（C++20 起），delete 表达式会：

1. 对于单对象形式：调用该对象的析构函数
2. 对于数组形式：从最后一个元素到第一个元素，依次调用每个元素的析构函数

析构函数必须从 delete 表达式出现的位置可访问。

#### 阶段二：释放函数调用

无论析构函数是否抛出异常，delete 表达式都会调用释放函数：

- 单对象形式调用 `operator delete`
- 数组形式调用 `operator delete[]`

### 释放函数查找规则

释放函数的名称在 ptr 指向的对象的**动态类型**作用域中查找：

1. 如果存在 `::`，则只在全局命名空间查找
2. 否则，优先查找类特定的释放函数，然后才是全局释放函数
3. 只考虑常规释放函数声明

### 释放函数选择优先级

**C++17 起，对齐感知释放函数：**
- 如果类型的对齐要求超过 `__STDCPP_DEFAULT_NEW_ALIGNMENT__`，优先选择带 `std::align_val_t` 参数的释放函数
- 否则，优先选择不带 `std::align_val_t` 参数的释放函数

**C++20 起，析构式删除 (destroying delete)：**
- 如果至少有一个释放函数是析构式删除，忽略所有非析构式删除

**类特定释放函数：**
- 优先选择无 `std::size_t` 参数的（size-unaware）
- 其次选择有 `std::size_t` 参数的（size-aware）

**C++14 起，全局释放函数：**
- 如果类型完整，且（仅限数组形式）操作数是指向具有非平凡析构函数的类类型或其数组的指针，选择带 `std::size_t` 参数的全局函数
- 否则，选择带或不带 `std::size_t` 参数的函数是未指定的

### 传递给释放函数的参数

- 第一个参数：要回收的存储块指针
- 可选的 `std::size_t` 参数：存储块大小
- 可选的 `std::align_val_t` 参数：对齐要求（C++17 起）

## 5. 使用场景 (Use Cases)

### 适用场景

1. **手动内存管理**：当需要精确控制对象生命周期时使用
2. **与 new 配对使用**：每个 new 必须有对应的 delete
3. **多态对象删除**：通过基类指针删除派生类对象

### 最佳实践

#### 配对使用原则

```cpp
// 正确：new 和 delete 配对
int* p = new int;
delete p;

// 正确：new[] 和 delete[] 配对
int* arr = new int[10];
delete[] arr;

// 错误：不配对会导致未定义行为
int* wrong = new int[10];
delete wrong;  // 未定义行为！
```

#### 多态对象删除

当通过基类指针删除派生类对象时，基类的析构函数必须是虚函数：

```cpp
class Base {
public:
    virtual ~Base() = default;  // 必须是虚析构函数
};

class Derived : public Base { };

Base* ptr = new Derived;
delete ptr;  // 正确：调用 Derived 的析构函数
```

#### 空指针处理

```cpp
int* p = nullptr;
delete p;  // 安全：默认释放函数对空指针不做任何操作
```

### 常见陷阱

1. **类型不匹配**：使用单对象 new 但用数组 delete[]，或反之
2. **重复删除**：同一指针删除两次会导致未定义行为
3. **删除 void 指针**：`void*` 不能被删除，因为它不是对象指针类型
4. **删除非 new 分配的内存**：删除栈对象或静态对象会导致未定义行为
5. **缺少虚析构函数**：通过基类指针删除派生对象时基类析构函数必须为虚

### C++11 Lambda 表达式注意事项

由于 `delete` 后紧跟的方括号总是被解释为数组形式，因此紧跟在 `delete` 后的空捕获列表 lambda 表达式必须用括号包围：

```cpp
// 错误：解析错误
// delete []{ return new int; }();

// 正确：用括号包围 lambda
delete ([]{ return new int; })();
```

## 6. 代码示例 (Examples)

### 示例 1：基本用法

```cpp
#include <iostream>

class Widget {
public:
    Widget() { std::cout << "Widget constructed\n"; }
    ~Widget() { std::cout << "Widget destroyed\n"; }
};

int main() {
    // 单对象删除
    Widget* w = new Widget;
    delete w;  // 输出: Widget destroyed

    return 0;
}
```

### 示例 2：数组删除

```cpp
#include <iostream>

class Item {
public:
    Item() { std::cout << "Item created\n"; }
    ~Item() { std::cout << "Item destroyed\n"; }
};

int main() {
    // 数组删除：析构顺序从后向前
    Item* items = new Item[3];
    // 输出: Item created (x3)

    delete[] items;
    // 输出: Item destroyed (x3，顺序从最后一个到第一个)

    return 0;
}
```

### 示例 3：虚析构函数与多态

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() {  // 虚析构函数
        std::cout << "Base destructor\n";
    }
};

class Derived : public Base {
public:
    ~Derived() override {
        std::cout << "Derived destructor\n";
    }
};

int main() {
    Base* ptr = new Derived;
    delete ptr;  // 正确：先调用 Derived::~Derived，再调用 Base::~Base

    return 0;
}
// 输出：
// Derived destructor
// Base destructor
```

### 示例 4：错误用法示范

```cpp
#include <iostream>

class BadBase {
public:
    ~BadBase() { std::cout << "BadBase destructor\n"; }  // 非虚析构函数
};

class BadDerived : public BadBase {
public:
    ~BadDerived() { std::cout << "BadDerived destructor\n"; }
};

int main() {
    // 错误示例 1：非虚析构函数导致的资源泄漏
    BadBase* ptr = new BadDerived;
    delete ptr;  // 未定义行为！只调用 BadBase::~BadBase

    // 错误示例 2：不配对的 new/delete
    int* arr = new int[10];
    delete arr;  // 未定义行为！应该用 delete[] arr;

    // 错误示例 3：重复删除
    int* p = new int(42);
    delete p;
    delete p;  // 未定义行为！

    return 0;
}
```

### 示例 5：类特定释放函数

```cpp
#include <iostream>
#include <new>

class MyClass {
public:
    MyClass() { std::cout << "MyClass constructed\n"; }
    ~MyClass() { std::cout << "MyClass destroyed\n"; }

    // 类特定的释放函数
    void operator delete(void* ptr) {
        std::cout << "Custom operator delete called\n";
        ::operator delete(ptr);
    }
};

int main() {
    MyClass* obj = new MyClass;
    delete obj;  // 调用自定义的 operator delete

    return 0;
}
// 输出：
// MyClass constructed
// MyClass destroyed
// Custom operator delete called
```

### 示例 6：C++17 对齐感知删除

```cpp
#include <iostream>
#include <new>

struct alignas(32) AlignedType {
    int data[4];

    void operator delete(void* ptr, std::align_val_t align) {
        std::cout << "Aligned delete called, alignment: "
                  << static_cast<size_t>(align) << "\n";
        ::operator delete(ptr, align);
    }
};

int main() {
    AlignedType* p = new AlignedType;
    delete p;  // 自动调用对齐感知的释放函数

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **配对原则**：`new` 与 `delete` 配对，`new[]` 与 `delete[]` 配对，混用会导致未定义行为
2. **执行顺序**：先调用析构函数（数组从后向前），再调用释放函数
3. **多态要求**：通过基类指针删除派生对象时，基类析构函数必须为虚
4. **空指针安全**：删除空指针是安全的，默认释放函数不做任何操作
5. **释放函数查找**：在动态类型的作用域中查找，优先类特定，其次全局

### 与其他机制对比

| 特性 | delete 表达式 | free() (C) | 智能指针 |
|------|--------------|------------|----------|
| 调用析构函数 | 是 | 否 | 是 |
| 类型安全 | 是 | 否 | 是 |
| 自动管理 | 否 | 否 | 是 |
| 异常安全 | 需手动保证 | 需手动保证 | 自动保证 |

### 学习建议

1. 现代C++中应优先使用智能指针（`std::unique_ptr`、`std::shared_ptr`）而非裸指针和 delete
2. 如果必须使用 delete，确保遵循 RAII (Resource Acquisition Is Initialization) 原则
3. 深理解析构函数和释放函数的调用顺序，这对资源管理至关重要
4. 熟悉 C++17 的对齐感知释放函数和 C++20 的析构式删除等新特性

### 参考链接

- [new 表达式](new.md) - 与 delete 配对的内存分配表达式
- [operator delete](../../memory/new/operator_delete.md) - 释放函数详解
- [析构函数](../destructor.md) - 对象销毁机制