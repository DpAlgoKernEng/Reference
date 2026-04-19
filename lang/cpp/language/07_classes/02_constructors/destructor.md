# 析构函数 (Destructor)

## 1. 概述 (Overview)

析构函数（Destructor）是一种特殊的成员函数，在对象生命周期结束时自动调用。析构函数的目的是释放对象在其生命周期期间可能获取的资源。

析构函数的核心作用：
- **资源释放**：释放对象占用的内存、文件句柄、网络连接等资源
- **清理操作**：执行必要的清理工作，如刷新缓冲区、关闭连接
- **对象销毁**：完成对象销毁前的最后操作

| 特性 | 说明 |
|------|------|
| 调用时机 | 对象生命周期结束时自动调用 |
| 函数签名 | `~ClassName()`，无返回值，无参数 |
| 可否重载 | 不可重载（每个类只能有一个析构函数） |
| 是否可为协程 | C++20 起不可为协程 |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

析构函数的概念起源于 C++ 的前身语言，其设计动机是解决 C 语言中手动资源管理的问题。在 C 语言中，开发者需要手动调用清理函数，这容易导致：
- 资源泄漏（忘记释放）
- 重复释放（多次调用清理函数）
- 异常安全问题（异常发生时资源未释放）

C++ 引入析构函数后，通过 RAII（Resource Acquisition Is Initialization）惯用语，将资源生命周期与对象生命周期绑定，自动解决了上述问题。

### C++11 变化

- 引入 `noexcept` 异常说明符
- 析构函数默认为 `noexcept`（除非基类或成员的析构函数为 `noexcept(false)`）
- 支持显式默认和删除析构函数

### C++17 变化

- 析构函数异常说明简化为只支持 `noexcept`
- 移除动态异常说明

### C++20 变化

- 引入"候选析构函数"（prospective destructor）概念
- 析构函数不能是协程
- 允许析构函数为 `constexpr`

### C++23 变化

- 如果满足 `constexpr` 函数要求，隐式定义的析构函数为 `constexpr`

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
class ClassName {
public:
    ~ClassName();  // 析构函数声明
};

// 析构函数定义
ClassName::~ClassName() {
    // 清理代码
}
```

### 语法形式

| 语法元素 | 说明 |
|----------|------|
| `class-name-with-tilde` | `~` 后跟类名（可以是注入类名） |
| `parameter-list` | 参数列表（可选，通常为空） |
| `except` | 异常说明（C++17 起为 `noexcept` 说明） |
| `attr` | 属性列表（C++11 起） |

### 声明说明符

在析构函数声明中，只允许以下说明符：
- `constexpr`（C++11 起）
- `friend`
- `inline`
- `virtual`

不允许有返回类型。

### 标识符表达式形式

1. **在类成员声明中**：
   - 对于普通类：`~` 后跟所在类的注入类名
   - 对于类模板：`~` 后跟当前实例化的类名（C++20 起为注入类名）

2. **在其他上下文中**：
   - 限定标识符，其终端未限定标识符为 `~` 后跟类名

### 纯虚析构函数

```cpp
class AbstractBase {
public:
    virtual ~AbstractBase() = 0;  // 纯虚析构函数声明
};

// 纯虚析构函数必须有定义
AbstractBase::~AbstractBase() {}
```

## 4. 底层原理 (Underlying Principles)

### 析构函数调用时机

析构函数在以下情况下被隐式调用：

| 场景 | 存储类型 | 说明 |
|------|----------|------|
| 程序终止 | 静态存储期 | 全局对象、静态局部变量 |
| 线程退出 | 线程局部存储期 | `thread_local` 变量（C++11 起） |
| 作用域结束 | 自动存储期 | 局部变量、引用绑定的临时对象 |
| delete 表达式 | 动态存储期 | `delete` 或 `delete[]` |
| 完整表达式结束 | 临时对象 | 未被延长生命周期的临时对象 |
| 栈展开 | 自动存储期 | 异常传播时未捕获的对象 |

### 潜在调用析构函数的场景

类 `C` 的析构函数在以下场景中被"潜在调用"：

- 显式或隐式调用
- `new` 表达式创建 `C` 类型对象数组
- `return` 语句的结果对象为 `C` 类型
- 聚合初始化数组，元素类型为 `C`
- 聚合初始化类对象，有 `C` 类型的成员（非匿名联合）
- 非委托构造函数中，潜在构造的子对象为 `C` 类型
- 构造 `C` 类型的异常对象

如果潜在调用的析构函数被删除或不可访问，程序非良构。

### 析构顺序（Destruction Sequence）

对于用户定义或隐式定义的析构函数，析构顺序如下：

1. 执行析构函数体
2. 销毁析构函数体内分配的自动对象
3. 按声明的**逆序**调用所有非静态非变体数据成员的析构函数
4. 按构造的**逆序**调用所有直接非虚基类的析构函数
5. 如果对象是最底层派生类，按构造的**逆序**调用所有虚基类的析构函数

```
构造顺序：基类 -> 成员 -> 函数体
析构顺序：函数体 -> 成员 -> 基类（与构造相反）
```

### 平凡析构函数 (Trivial Destructor)

类的析构函数为平凡析构函数需满足以下所有条件：

| 条件 | 说明 |
|------|------|
| 非用户提供 | 隐式声明或在首次声明时显式定义为默认 |
| 非虚函数 | 析构函数不是虚函数 |
| 基类平凡 | 所有直接基类具有平凡析构函数 |
| 成员平凡 | 所有非静态类类型成员具有平凡析构函数 |

**平凡析构函数的特点**：
- 不执行任何操作
- 不需要通过 `delete` 表达式销毁对象
- 可通过简单释放存储来处置对象
- 所有与 C 语言兼容的数据类型（POD 类型）都是平凡可析构的

### 隐式声明与隐式定义

**隐式声明**：如果类没有用户声明的候选析构函数（C++20 起），编译器会自动声明一个内联公有析构函数。

**隐式定义**：如果隐式声明的析构函数未被删除，在 ODR 使用时编译器会隐式定义它。隐式定义的析构函数函数体为空。

**异常说明**：隐式声明的析构函数默认为 `noexcept`，除非：
- 某个基类或成员的析构函数为 `noexcept(false)`
- C++17 前隐式定义会直接调用具有不同异常说明的函数

### 删除的析构函数

隐式声明或显式默认的析构函数在以下情况下被定义为删除：

| 条件 | 说明 |
|------|------|
| 成员/基类不可析构 | 某个潜在构造的子对象的析构函数被删除或不可访问 |
| 变体成员问题 | 变体成员具有非平凡析构函数 |
| 虚析构函数问题 | 析构函数为虚函数，但查找释放函数时出现歧义或函数被删除/不可访问 |

## 5. 使用场景 (Use Cases)

### 适合使用析构函数的场景

| 场景 | 说明 |
|------|------|
| RAII 资源管理 | 析构函数中自动释放资源 |
| 智能指针实现 | `unique_ptr`、`shared_ptr` 的核心 |
| 文件/锁管理 | 自动关闭文件、释放锁 |
| 自定义容器 | 释放容器管理的内存 |

### 虚析构函数

**何时需要虚析构函数**：当类可能被继承，且需要通过基类指针删除派生类对象时。

```cpp
// 正确用法
class Base {
public:
    virtual ~Base() = default;  // 虚析构函数
};

class Derived : public Base {};

Base* b = new Derived;
delete b;  // 安全：正确调用派生类析构函数
```

**设计准则**：基类析构函数应该：
- 公有且虚函数（当需要多态删除时）
- 或保护且非虚函数（当不允许多态删除时）

### 纯虚析构函数

用于使类成为抽象类，但没有其他适合声明为纯虚函数的函数。

```cpp
class AbstractBase {
public:
    virtual ~AbstractBase() = 0;  // 纯虚析构函数
};

// 必须提供定义
AbstractBase::~AbstractBase() {}

class Derived : public AbstractBase {};

// AbstractBase obj;  // 编译错误：抽象类
Derived obj;         // OK
```

### 异常与析构函数

**核心原则**：析构函数不应抛出异常。

```cpp
// 危险：析构函数中抛出异常
class Bad {
public:
    ~Bad() noexcept(false) {
        throw std::runtime_error("Error");  // 危险！
    }
};
```

**原因**：
- 如果析构函数在栈展开期间抛出异常，`std::terminate` 会被调用
- 栈展开期间可通过 `std::uncaught_exceptions()` 检测

### 最佳实践

1. **优先使用 `= default`**：让编译器生成默认析构函数
2. **基类使用虚析构函数**：确保多态删除安全
3. **析构函数中避免抛出异常**：保证异常安全
4. **使用 RAII 管理资源**：避免手动管理

### 常见陷阱

| 陷阱 | 问题 | 解决方案 |
|------|------|----------|
| 非虚析构函数 | 通过基类指针删除派生类导致未定义行为 | 基类析构函数声明为虚函数 |
| 析构函数抛异常 | 栈展开时导致程序终止 | 析构函数使用 `try-catch` 捕获所有异常 |
| 手动调用析构函数 | 作用域结束时重复调用导致未定义行为 | 避免手动调用，让编译器自动处理 |

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

struct A
{
    int i;

    A(int num) : i(num)
    {
        std::cout << "ctor a" << i << '\n';
    }

    ~A()
    {
        std::cout << "dtor a" << i << '\n';
    }
};

A a0(0);  // 静态存储期对象

int main()
{
    A a1(1);  // 自动存储期对象
    A* p;

    { // 嵌套作用域
        A a2(2);
        p = new A(3);
    } // a2 离开作用域，调用析构函数

    delete p; // 调用 a3 的析构函数
} // a1 离开作用域，调用析构函数
// 程序结束时 a0 被销毁

// 输出:
// ctor a0
// ctor a1
// ctor a2
// ctor a3
// dtor a2
// dtor a3
// dtor a1
// dtor a0
```

### RAII 资源管理示例

```cpp
#include <iostream>
#include <fstream>
#include <string>

class FileHandler {
public:
    explicit FileHandler(const std::string& filename)
        : file_(std::fopen(filename.c_str(), "r"))
    {
        if (!file_) {
            throw std::runtime_error("Cannot open file");
        }
        std::cout << "File opened\n";
    }

    ~FileHandler() {
        if (file_) {
            std::fclose(file_);
            std::cout << "File closed\n";
        }
    }

    // 禁止复制
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    // 允许移动
    FileHandler(FileHandler&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }

private:
    std::FILE* file_;
};

void processFile() {
    FileHandler handler("example.txt");
    // 使用文件...
} // 自动调用析构函数关闭文件
```

### 虚析构函数示例

```cpp
#include <iostream>

class Base {
public:
    Base() { std::cout << "Base constructor\n"; }
    virtual ~Base() { std::cout << "Base destructor\n"; }  // 虚析构函数
};

class Derived : public Base {
public:
    Derived() { std::cout << "Derived constructor\n"; }
    ~Derived() override { std::cout << "Derived destructor\n"; }
};

int main() {
    Base* ptr = new Derived();
    delete ptr;  // 正确：先调用 Derived::~Derived()，再调用 Base::~Base()
    return 0;
}

// 输出:
// Base constructor
// Derived constructor
// Derived destructor
// Base destructor
```

### 常见错误及修正

#### 错误 1：非虚析构函数导致资源泄漏

```cpp
// 错误：基类析构函数非虚
class BaseBad {
public:
    ~BaseBad() { std::cout << "~BaseBad\n"; }  // 非虚析构函数
};

class DerivedBad : public BaseBad {
public:
    DerivedBad() : data_(new int[100]) {}
    ~DerivedBad() { delete[] data_; std::cout << "~DerivedBad\n"; }
private:
    int* data_;
};

// 未定义行为：只调用 BaseBad::~BaseBad()，DerivedBad 的资源泄漏
BaseBad* p = new DerivedBad();
delete p;  // 资源泄漏！

// 修正：使用虚析构函数
class BaseGood {
public:
    virtual ~BaseGood() { std::cout << "~BaseGood\n"; }
};

class DerivedGood : public BaseGood {
public:
    DerivedGood() : data_(new int[100]) {}
    ~DerivedGood() override { delete[] data_; std::cout << "~DerivedGood\n"; }
private:
    int* data_;
};

BaseGood* p2 = new DerivedGood();
delete p2;  // 正确：两个析构函数都被调用
```

#### 错误 2：析构函数中抛出异常

```cpp
#include <iostream>
#include <stdexcept>

// 错误：析构函数抛出异常
class Dangerous {
public:
    ~Dangerous() noexcept(false) {
        throw std::runtime_error("Oops!");  // 危险！
    }
};

int main() {
    try {
        Dangerous d;
        throw std::runtime_error("Another exception");
    } catch (...) {
        // Dangerous 的析构函数在栈展开时被调用
        // 再次抛出异常导致 std::terminate()
    }
    return 0;
}

// 修正：在析构函数中捕获所有异常
class Safe {
public:
    ~Safe() {
        try {
            // 可能抛出异常的代码
        } catch (...) {
            // 捕获并处理异常，不让其逃逸
            std::cerr << "Exception caught in destructor\n";
        }
    }
};
```

#### 错误 3：手动调用析构函数

```cpp
#include <iostream>

class Widget {
public:
    Widget() { std::cout << "Widget constructed\n"; }
    ~Widget() { std::cout << "Widget destroyed\n"; }
};

int main() {
    Widget w;
    w.~Widget();  // 危险：手动调用析构函数
    // 作用域结束时析构函数再次被调用 -> 未定义行为
    return 0;
}

// 修正：仅在使用 placement new 时手动调用
#include <new>

void placementNewExample() {
    alignas(Widget) char buffer[sizeof(Widget)];
    Widget* p = new (buffer) Widget();  // placement new
    p->~Widget();  // 正确：手动调用析构函数
    // 无需 delete，因为 buffer 在栈上
}
```

## 7. 总结 (Summary)

### 核心要点

| 特性 | 说明 |
|------|------|
| 自动调用 | 对象生命周期结束时自动调用 |
| 不可重载 | 每个类只能有一个析构函数 |
| 析构顺序 | 函数体 -> 成员 -> 基类（与构造顺序相反） |
| 虚析构函数 | 多态基类必须使用虚析构函数 |
| 异常安全 | 析构函数不应抛出异常 |

### 技术对比

| 特性 | 构造函数 | 析构函数 |
|------|----------|----------|
| 参数 | 可有参数 | 无参数 |
| 返回值 | 无 | 无 |
| 可重载 | 是 | 否 |
| 可为虚 | 否 | 是 |
| 调用时机 | 对象创建时 | 对象销毁时 |
| 继承调用顺序 | 基类 -> 派生类 | 派生类 -> 基类 |

### 学习建议

1. **理解 RAII**：析构函数是 RAII 的核心，掌握资源管理模式
2. **记住虚析构函数规则**：多态基类必须有虚析构函数
3. **避免析构函数中抛异常**：保证异常安全
4. **使用智能指针**：现代 C++ 应优先使用 `unique_ptr`、`shared_ptr` 管理资源

### 相关概念

| 概念 | 关系 |
|------|------|
| 构造函数 | 对象生命周期起点 |
| RAII | 资源管理的核心惯用语 |
| `std::unique_ptr` | 自动调用析构函数的智能指针 |
| `std::shared_ptr` | 引用计数智能指针 |
| Copy elision | 复制消除优化 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/destructor
- C++ Standard: [class.dtor]
- Effective C++, Scott Meyers, Item 7: Declare destructors virtual in polymorphic base classes