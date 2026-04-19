# this 指针

## 1. 概述

`this` 指针是 C++ 中一个特殊的隐式指针，仅在类的非静态成员函数中可用。它是一个纯右值（prvalue）表达式，其值是调用该成员函数的对象的地址。通过 `this` 指针，成员函数可以访问调用它的对象实例。

`this` 指针是 C++ 面向对象编程的核心机制之一，它使得成员函数能够区分不同对象实例的数据成员，是实现封装和对象行为的关键基础设施。

## 2. 来源与演变

### 首次引入

`this` 指针在 **C++ 的前身 C with Classes**（约 1980 年）中就已经存在，并在 **C++98** 标准中正式规范化。它是 C++ 最早的语言特性之一。

### 历史背景

在 C++ 之前，C 语言的结构体只能包含数据，不能包含函数。C++ 引入成员函数后，需要一个机制让成员函数知道它正在操作哪个对象实例。`this` 指针就是为了解决这个问题而设计的：

- **Smalltalk 影响**：Smalltalk 等面向对象语言中的 `self` 概念启发了 `this` 的设计
- **实现成员函数**：使成员函数能够访问当前对象的数据成员
- **支持链式调用**：通过返回 `*this` 实现流式接口

### C++11 变化

- 允许在 lambda 表达式的捕获列表中使用 `this`
- 允许在 lambda 表达式体内使用 `this`
- 支持在默认成员初始化器中使用 `this`

### 标准缺陷修正

| 缺陷报告 | 版本 | 原问题 | 修正内容 |
|---------|------|--------|---------|
| CWG 760 | C++98 | 嵌套类中使用 `this` 时，未明确关联嵌套类还是外围类 | `this` 始终关联最内层的嵌套类 |
| CWG 2271 | C++98 | 构造非 const 对象时 `this` 可以被别名化 | 此情况下也禁止别名 |
| CWG 2869 | C++98 | 不清楚 `this` 是否可以在非关联类的静态成员函数中使用 | 已明确 |

## 3. 语法与声明

### 基本语法

```
this
```

`this` 是一个关键字，不需要声明或定义，直接在成员函数中使用即可。

### 可用上下文

`this` 指针可以在以下上下文中使用：

| 上下文 | 版本要求 | 说明 |
|--------|---------|------|
| 非静态成员函数体内 | C++98 | 包括成员初始化列表 |
| 非静态成员函数声明中 | C++98 | 在 cv-限定符序列之后，包括异常规范和尾置返回类型 |
| 默认成员初始化器 | C++11 起 | |
| lambda 表达式的捕获列表 | C++11 起 | |
| lambda 表达式体内 | C++11 起 | |

### 类型规则

`this` 指针的类型取决于成员函数的 cv-限定符：

| 成员函数类型 | `this` 的类型 |
|-------------|--------------|
| 普通成员函数 | `X*` |
| const 成员函数 | `const X*` |
| volatile 成员函数 | `volatile X*` |
| const volatile 成员函数 | `const volatile X*` |

**注意**：构造函数和析构函数不能声明 cv-限定符，因此其中 `this` 的类型始终是 `X*`，即使在构造或析构 const 对象时也是如此。

### 类模板中的 this

在类模板中，`this` 是依赖表达式（dependent expression）。可以使用显式的 `this->` 来强制另一个表达式成为依赖表达式：

```cpp
template<typename T>
struct D : B<T>
{
    D()
    {
        // var = 1;    // 错误：var 未在此作用域中声明
        this->var = 1; // 正确：this-> 使 var 成为依赖表达式
    }
};
```

## 4. 底层原理

### 实现机制

`this` 指针通常通过以下方式实现：

1. **调用约定传递**：编译器将对象地址作为隐藏参数传递给成员函数
2. **寄存器/栈存储**：对象地址存储在特定寄存器（如 x86 的 ecx）或栈位置

**典型的成员函数调用转换**：

```cpp
// 源代码
obj.method(arg);

// 编译器转换后（伪代码）
ClassType::method(&obj, arg);  // 对象地址作为第一个参数
```

### 内存模型

```
+-------------------+
| 对象实例 (Object) |
+-------------------+
| 成员变量 x        |  <-- this 指向此处
| 成员变量 y        |
| ...               |
+-------------------+

this 的值 = 对象的起始地址
```

### 纯右值特性

`this` 是纯右值（prvalue），具有以下特性：

- **不可修改**：不能对 `this` 赋值（`this = ...` 是非法的）
- **不可取地址**：不能对 `this` 取地址（`&this` 是非法的）
- **临时性**：表达式求值后不产生持久化的对象

### 构造函数中的别名限制

在对象构造期间，如果通过非从构造函数 `this` 指针获得的泛左值访问对象或其子对象的值，则获得的值是未指定的：

```cpp
extern struct D d;

struct D
{
    D(int a) : a(a), b(d.a) {} // b(a) 或 b(this->a) 才是正确的
    int a, b;
};

D d = D(1); // 因为 b(d.a) 不是通过 this 获取 a，d.b 的值未指定
```

## 5. 使用场景

### 典型使用场景

| 场景 | 说明 |
|------|------|
| 区分成员与参数 | 当参数名与成员变量名相同时 |
| 链式调用 | 返回 `*this` 以支持连续操作 |
| 自引用 | 在成员函数中传递当前对象 |
| 显式成员访问 | 在模板类中访问基类成员 |
| 自删除 | 在引用计数对象中释放自身 |

### 最佳实践

1. **区分同名的成员变量和参数**

```cpp
void setValue(int x) {
    this->x = x;  // 清晰区分成员变量 x 和参数 x
}
```

2. **实现链式调用**

```cpp
class Builder {
public:
    Builder& setName(const std::string& name) {
        m_name = name;
        return *this;  // 返回引用以支持链式调用
    }

    Builder& setAge(int age) {
        m_age = age;
        return *this;
    }
};

// 使用
Builder b;
b.setName("Alice").setAge(25);  // 链式调用
```

3. **在成员初始化列表中使用**

```cpp
class T {
    int x, y;
    T(int x) : x(x),        // 使用参数 x 初始化成员 x
               y(this->x)  // 使用成员 x 初始化成员 y
    {}
};
```

### 常见陷阱

1. **在静态成员函数中使用 `this`**

```cpp
class T {
    static void foo() {
        // this->x = 1;  // 错误：静态成员函数没有 this 指针
    }
};
```

2. **构造函数中的别名问题**

```cpp
// 错误示例：在构造函数中通过外部引用访问成员
extern T globalT;
T::T() : member(globalT.member) {}  // 危险：member 的值可能未定义
```

3. **delete this 的危险性**

```cpp
void suicide() {
    delete this;  // 危险！调用后不能再访问任何成员
    // this->x = 1;  // 未定义行为
}
```

### delete this 的正确用法

`delete this` 只能在以下条件全部满足时使用：

1. 对象是通过 `new` 分配的（不是 `new[]`，不是栈对象，不是全局对象）
2. 调用后不再访问任何成员变量或成员函数
3. 调用者不再持有该对象的任何指针或引用

典型应用场景：引用计数智能指针（如 `std::shared_ptr`）的内部实现：

```cpp
class RefCounted {
    int m_refCount;
public:
    void addRef() { ++m_refCount; }
    void release() {
        if (--m_refCount == 0) {
            delete this;  // 引用计数归零时自删除
        }
    }
};
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

class T {
    int x;

public:
    void foo() {
        x = 6;        // 等价于 this->x = 6
        this->x = 5;  // 显式使用 this->
    }

    void foo() const {
        // x = 7;     // 错误：*this 是 const 的
    }

    // 参数名遮蔽成员变量
    void foo(int x) {
        this->x = x;  // 非限定的 x 指向参数
                      // this-> 用于消歧
    }

    int y;
    T(int x) : x(x),        // 使用参数 x 初始化成员 x
               y(this->x)   // 使用成员 x 初始化成员 y
    {}

    // 重载赋值运算符
    T& operator=(const T& b) {
        x = b.x;
        return *this;       // 许多重载运算符返回 *this
    }
};
```

### 高级用法：链式调用

```cpp
#include <iostream>
#include <string>

class StringBuilder {
    std::string m_data;

public:
    StringBuilder& append(const std::string& s) {
        m_data += s;
        return *this;  // 返回引用支持链式调用
    }

    StringBuilder& append(int num) {
        m_data += std::to_string(num);
        return *this;
    }

    StringBuilder& clear() {
        m_data.clear();
        return *this;
    }

    std::string build() const {
        return m_data;
    }
};

int main() {
    StringBuilder sb;
    std::string result = sb.append("Hello, ")
                           .append("World! ")
                           .append(2024)
                           .build();
    std::cout << result << std::endl;  // 输出: Hello, World! 2024
    return 0;
}
```

### 模板类中使用 this

```cpp
template<typename T>
struct Base {
    int value;
};

template<typename T>
struct Derived : Base<T> {
    void setValue(int v) {
        // value = v;        // 错误：编译器无法找到 value
        this->value = v;     // 正确：显式使用 this->

        // 或者使用 Base<T>::value = v;
    }
};
```

### 引用计数对象

```cpp
#include <iostream>

class RefCounted {
    int m_refCount = 1;
    int m_data;

public:
    RefCounted(int data) : m_data(data) {}

    void addRef() {
        ++m_refCount;
    }

    void release() {
        if (--m_refCount == 0) {
            delete this;  // 引用计数归零时自删除
        }
    }

    int getData() const { return m_data; }
};

int main() {
    RefCounted* obj = new RefCounted(42);

    obj->addRef();          // 引用计数变为 2
    std::cout << obj->getData() << std::endl;  // 42

    obj->release();         // 引用计数变为 1
    // obj 仍然有效

    obj->release();         // 引用计数变为 0，对象被删除
    // obj 现在是悬空指针，不能再使用

    return 0;
}
```

### 常见错误及修正

#### 错误 1：在嵌套类中错误使用 this

```cpp
class Outer {
    int a[sizeof(*this)];  // 错误：不在成员函数内

    void f() {
        int b[sizeof(*this)];  // 正确：在成员函数内

        struct Inner {
            int c[sizeof(*this)];  // 错误：不在 Inner 的成员函数内
                                   // this 不关联 Outer
        };
    }
};

// 修正：使用显式大小
class Outer {
    static constexpr int SIZE = 10;
    int a[SIZE];  // 正确

    void f() {
        struct Inner {
            int c[Outer::SIZE];  // 正确：使用外部常量
        };
    }
};
```

#### 错误 2：delete this 后继续使用对象

```cpp
// 错误示例
class Bad {
public:
    void destroy() {
        delete this;
        doSomething();  // 未定义行为！访问已销毁的对象
    }

    void doSomething() {
        // ...
    }
};

// 修正：delete this 后立即返回
class Good {
public:
    void destroy() {
        delete this;
        return;  // 立即返回，不执行任何后续操作
    }
};
```

#### 错误 3：对非堆对象调用 delete this

```cpp
// 错误示例
class T {
public:
    void destroy() { delete this; }
};

int main() {
    T obj;            // 栈对象
    obj.destroy();    // 未定义行为！对非堆对象调用 delete

    T* p = &obj;
    p->destroy();     // 同样未定义行为

    return 0;
}

// 正确用法
int main() {
    T* obj = new T();  // 堆对象
    obj->destroy();    // 正确：对象是通过 new 分配的
    // obj 现在是悬空指针，不能再使用
    return 0;
}
```

## 7. 总结

`this` 指针是 C++ 面向对象编程的基础设施，它使成员函数能够访问调用对象实例。核心要点：

| 特性 | 说明 |
|------|------|
| 本质 | 纯右值（prvalue），值为对象地址 |
| 类型 | 取决于成员函数的 cv-限定符 |
| 作用域 | 仅在非静态成员函数中可用 |
| 用途 | 区分成员与参数、链式调用、自引用 |

**核心使用建议**：

1. 当成员变量与参数同名时，使用 `this->` 消歧
2. 实现链式调用时返回 `*this`
3. 在模板派生类中访问基类成员时使用 `this->`
4. 避免使用 `delete this`，除非实现引用计数等特殊场景
5. 不要在静态成员函数中使用 `this`

**相关概念**：

| 概念 | 关系 |
|------|------|
| 成员函数 | `this` 作为隐式参数传递 |
| 静态成员函数 | 没有 `this` 指针 |
| const 成员函数 | `this` 类型为 `const X*` |
| lambda 捕获 | 可捕获 `this` 以访问成员 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/this
- C++ Standard: [class.this]
- The C++ Programming Language, Bjarne Stroustrup