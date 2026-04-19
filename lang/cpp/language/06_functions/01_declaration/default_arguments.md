# 默认参数 (Default Arguments)

## 1. 概述

默认参数 (Default Arguments) 允许函数在调用时省略一个或多个尾随参数 (trailing arguments)。通过在函数声明的参数列表中为参数指定默认值，调用者可以选择不提供这些参数，编译器会自动使用预设的默认值。

默认参数是 C++ 函数声明的重要特性，广泛应用于简化 API 设计、提供灵活的调用接口、减少函数重载数量等场景。

## 2. 来源与演变

### 首次引入

默认参数在 **C++98** 标准中首次引入，是 C++ 语言的原始特性之一。

### 历史背景

在默认参数出现之前，开发者需要实现多个重载函数来提供类似的灵活性：

```cpp
// 没有默认参数时的替代方案
void point(int x, int y);
void point(int x) { point(x, 4); }  // 重载版本
void point() { point(3, 4); }        // 重载版本
```

默认参数的引入解决了以下问题：
- 减少代码重复
- 简化 API 设计
- 提供更清晰的接口语义

### C++11 变化

- 允许参数包 (parameter pack) 后跟具有默认参数的参数
- 允许函数参数包使用默认参数
- Lambda 表达式支持默认参数

### C++20 变化

- 非内联函数在不同翻译单元中的默认参数必须一致（如果存在的话）

### C++23 变化

- 显式对象参数 (explicit object parameter) 不能有默认参数

### 缺陷报告修正

| DR | 应用版本 | 原行为 | 修正后行为 |
|----|---------|--------|-----------|
| CWG 217 | C++98 | 可以为类模板的非模板成员函数添加默认参数 | 禁止 |
| CWG 1344 | C++98 | 类外定义添加默认参数可能使成员函数变为特殊成员函数 | 禁止 |
| CWG 1716 | C++98 | 每次调用函数都评估默认参数，即使调用者提供了参数 | 仅在未提供参数时评估 |
| CWG 2082 | C++98 | 默认参数禁止在未评估上下文中使用局部变量和前置参数 | 允许在未评估上下文中使用 |
| CWG 2233 | C++11 | 从参数包展开的参数不能出现在有默认参数的参数之后 | 允许 |
| CWG 2683 | C++98 | 类模板嵌套类的成员函数的类外定义可以有默认参数 | 禁止 |

## 3. 语法与参数

### 基本语法

默认参数在函数声明的参数列表中使用以下语法：

```cpp
// 完整声明语法
attr(可选) decl-specifier-seq declarator = initializer

// 抽象声明器语法（用于函数类型声明）
attr(可选) decl-specifier-seq abstract-declarator(可选) = initializer
```

### 参数说明

| 语法元素 | 说明 |
|---------|------|
| `attr` | 可选的属性说明符 |
| `decl-specifier-seq` | 声明说明符序列（如类型说明符） |
| `declarator` | 声明器（参数名称） |
| `abstract-declarator` | 抽象声明器（可选的参数名称） |
| `initializer` | 默认值表达式 |

### 基本示例

```cpp
void point(int x = 3, int y = 4);

point(1, 2); // 调用 point(1, 2)
point(1);    // 调用 point(1, 4)
point();     // 调用 point(3, 4)
```

### 声明位置规则

1. **只能在函数声明和 Lambda 表达式中使用**
2. **不能用于函数指针、函数引用或 typedef 声明**

```cpp
// 允许的位置
void f(int x = 0);                    // 函数声明
auto lambda = [](int x = 0) {};        // Lambda 表达式 (C++11)

// 不允许的位置
typedef void (*Func)(int = 0);         // 错误：函数指针
using FuncRef = void (&)(int = 0);     // 错误：函数引用
```

## 4. 底层原理

### 尾随参数规则

在函数声明中，一旦某个参数具有默认参数，其后的所有参数必须：

1. **在同一作用域的同一或先前声明中提供默认参数**：

```cpp
void f(int n, int k = 1);
void f(int n = 0, int k); // OK：k 的默认参数由先前的声明提供

void g(int, int = 7);

void h() {
    void g(int = 1, int); // 错误：不是同一作用域
}
```

2. **或是从参数包展开的参数** (C++11 起)：

```cpp
template<class... T>
struct C {
    void f(int n = 0, T...); // OK
};

C<int> c; // 实例化为 void C::f(int n = 0, int)

template<class... T>
void h(int i = 0, T... args); // OK：函数参数包
```

3. **省略号 `...` 不算作参数**：

```cpp
int g(int n = 0, ...); // OK：省略号可以跟在有默认参数的参数后
```

### 作用域与重声明规则

默认参数通过合并所有可见声明中的默认参数来获得：

```cpp
void f(int, int);     // #1
void f(int, int = 7); // #2 OK：添加默认参数

void h() {
    f(3); // #1 和 #2 都在作用域内，调用 f(3, 7)
    void f(int = 1, int); // 错误：不会从外部作用域获取默认参数
}

void m() {
    void f(int, int); // 内部作用域声明，无默认参数
    f(4); // 错误：参数不足
    void f(int, int = 6);
    f(4); // OK：调用 f(4, 6)
    void f(int, int = 6); // 错误：第二个参数已有默认参数
}

void f(int = 1, int); // #3 OK：为 #2 添加默认参数

void n() {
    f(); // #1, #2, #3 都在作用域内，调用 f(1, 7)
}
```

### 名称查找与绑定

默认参数中的名称在声明点进行查找、访问检查和绑定，但在函数调用点执行：

```cpp
int a = 1;

int f(int);
int g(int x = f(a)); // 查找 f 找到 ::f，查找 a 找到 ::a
                     // ::a 的值（此时为 1）不被使用

void h() {
    a = 2; // 修改 ::a 的值
    {
        int a = 3;
        g(); // 调用 f(2)，然后用结果调用 g()
    }
}
```

### 函数类型

默认参数不是函数类型的一部分：

```cpp
int f(int = 0);

void h() {
    int j = f(1);
    int k = f(); // 调用 f(0)
}

int (*p1)(int) = &f; // OK
int (*p2)()    = &f; // 错误：f 的类型是 int(int)
```

## 5. 使用场景

### 适合使用默认参数的场景

| 场景 | 示例 | 说明 |
|------|------|------|
| 可选配置参数 | `void open(const char* path, int mode = 0);` | 提供常用默认值 |
| 回调函数默认行为 | `void sort(int (*cmp)(int, int) = default_cmp);` | 默认比较函数 |
| 扩展 API 兼容性 | `void process(int x, int y = 0, int z = 0);` | 新增参数保持兼容 |
| 简化常用调用 | `void log(const char* msg, int level = INFO);` | 使用最常用的默认值 |

### 不适合使用默认参数的场景

| 场景 | 原因 | 替代方案 |
|------|------|---------|
| 虚函数覆盖 | 派生类不继承基类的默认参数 | 使用重载或 NVI 模式 |
| 函数指针 | 默认参数不参与类型 | 使用 std::function + lambda |
| 内联函数跨翻译单元 | 默认参数必须一致 | 确保声明一致或使用头文件 |

### 最佳实践

1. **将默认参数放在函数声明中，而非定义中**

```cpp
// 头文件 (.h)
void f(int x = 0);

// 源文件 (.cpp)
void f(int x) {
    // 实现
}
```

2. **使用 using 声明携带默认参数**

```cpp
namespace N {
    void f(int, int = 1);
}

using N::f;

void g() {
    f(7); // 调用 f(7, 1)
}

namespace N {
    void f(int = 2, int);
}

void h() {
    f(); // 调用 f(2, 1)
}
```

3. **注意空格避免复合赋值符号**

```cpp
void f1(int*=0);         // 错误：'*=' 被解析为复合赋值运算符
void g1(const int&=0);   // 错误：'&=' 被解析为复合赋值运算符
void f2(int* = 0);       // OK
void g2(const int& = 0); // OK
void h(int&&=0);         // OK：'&&' 本身是一个标记
```

### 线程安全性与评估时机

默认参数在每次函数调用时（当没有为对应参数提供实参时）都会被评估：

```cpp
int counter() {
    static int count = 0;
    return ++count;
}

void f(int n = counter());

void g() {
    f(); // n = 1
    f(); // n = 2
    f(10); // n = 10，不评估 counter()
    f(); // n = 3
}
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <string>

// 基本的默认参数使用
void print_message(const std::string& msg = "Hello, World!") {
    std::cout << msg << std::endl;
}

// 多个默认参数
void draw_rect(int x, int y, int width = 10, int height = 10) {
    std::cout << "Rectangle at (" << x << ", " << y << ") "
              << "with size " << width << "x" << height << std::endl;
}

int main() {
    print_message();                     // 输出：Hello, World!
    print_message("Custom message");     // 输出：Custom message

    draw_rect(0, 0);                     // 使用默认宽高
    draw_rect(5, 5, 20);                  // 部分使用默认参数
    draw_rect(5, 5, 20, 30);              // 全部指定

    return 0;
}
```

### 成员函数默认参数

```cpp
#include <iostream>

class Widget {
public:
    void resize(int width = 100, int height = 100);
    void set_color(int r = 0, int g = 0, int b = 0);

    // 非默认构造函数
    Widget(int id);
};

// 类外定义可以添加默认参数
void Widget::resize(int width, int height) {
    std::cout << "Resizing to " << width << "x" << height << std::endl;
}

// 错误示例：不能将构造函数变为默认构造函数
// Widget::Widget(int id = 1) {}  // 错误！

int main() {
    Widget w(42);
    w.resize();        // 使用默认值
    w.resize(200);      // height 使用默认值
    w.resize(200, 150); // 全部指定

    return 0;
}
```

### 虚函数默认参数陷阱

```cpp
#include <iostream>

struct Base {
    virtual void f(int a = 7) {
        std::cout << "Base::f(" << a << ")" << std::endl;
    }
};

struct Derived : Base {
    void f(int a) override {
        std::cout << "Derived::f(" << a << ")" << std::endl;
    }
};

int main() {
    Derived d;
    Base& b = d;

    b.f(); // OK：调用 Derived::f(7)，使用 Base 的默认参数！
    // d.f(); // 错误：Derived::f 没有默认参数

    return 0;
}
```

### 模板与默认参数

```cpp
#include <iostream>

// 参数包后的参数可以有默认参数 (C++11)
template<typename... Args>
void log_message(Args... args, int level = 0) {
    std::cout << "Level: " << level << std::endl;
}

// 类模板成员函数
template<typename T>
class Container {
public:
    void add(const T& item, int count = 1);
};

template<typename T>
void Container<T>::add(const T& item, int count) {
    // 必须在类内提供默认参数，类外定义不能重复
}

int main() {
    log_message(1, 2, 3);     // level = 0
    log_message(1, 2, 3, 5);   // level = 5

    return 0;
}
```

### 常见错误及修正

#### 错误 1：非尾随参数有默认参数

```cpp
// 错误：只有尾随参数可以有默认参数
int x(int = 1, int); // 错误！

// 修正：重新排列参数顺序
int x(int, int = 1); // OK
```

#### 错误 2：在类外定义中重复默认参数

```cpp
class C {
    void f(int i = 3);
};

// 错误：默认参数已在类内指定
void C::f(int i = 3) {}

// 修正：类外定义不重复默认参数
void C::f(int i) {}
```

#### 错误 3：使用局部变量作为默认参数

```cpp
void f() {
    int n = 1;
    extern void g(int x = n); // 错误：局部变量不能作为默认参数
    extern void h(int x = sizeof n); // OK：未评估上下文
}
```

#### 错误 4：使用 this 指针作为默认参数

```cpp
class A {
    void f(A* p = this) {} // 错误：this 不允许作为默认参数
};
```

#### 错误 5：使用非静态成员作为默认参数

```cpp
int b;

class X {
    int a;
    int mem1(int i = a);           // 错误：非静态成员不能作为默认参数
    int mem2(int i = b);           // OK：静态成员
    int mem3(int X::* i = &X::a);  // OK：成员指针
    int mem4(int i = x.a);         // OK：成员访问表达式

    static X x;
    static int b;
};
```

#### 错误 6：在默认参数中使用前置参数

```cpp
int a;

int f(int a, int b = a);         // 错误：参数 a 用作默认参数
int g(int a, int b = sizeof a);  // OK：未评估上下文
```

#### 错误 7：运算符函数的默认参数

```cpp
class C {
    int operator[](int i = 0); // 错误：下标运算符不能有默认参数
    int operator()(int x = 0);  // OK：函数调用运算符可以有默认参数
};
```

## 注意事项

1. **虚函数默认参数**：覆盖函数不继承基类的默认参数，调用时根据静态类型决定
2. **友元声明**：如果友元声明指定了默认参数，必须是友元函数定义，且该翻译单元中不允许有其他声明
3. **跨翻译单元**：内联函数在不同翻译单元中的默认参数集合必须相同
4. **评估时机**：默认参数在每次调用时评估，注意性能和线程安全
5. **运算符限制**：除函数调用运算符外，运算符函数不能有默认参数

## 相关概念

| 概念 | 关系 |
|------|------|
| 函数重载 (Function Overloading) | 默认参数可以减少重载数量，但可能引入歧义 |
| 函数模板 (Function Templates) | 模板参数也有类似的默认参数机制 |
| 可变参数模板 (Variadic Templates) | 参数包可以出现在有默认参数的参数之后 |

## 7. 总结

默认参数是 C++ 提供函数调用灵活性的重要机制：

- **简化接口**：减少重载数量，提供更清晰的 API
- **向后兼容**：新增参数时保持已有代码兼容
- **灵活组合**：可在多次声明中累积默认参数

核心使用建议：
1. 将默认参数放在声明中（头文件），而非定义中
2. 只有尾随参数可以有默认参数
3. 注意虚函数中默认参数的静态绑定行为
4. 避免在默认参数中使用可能变化的值（由于每次调用都会评估）
5. 注意跨作用域声明时默认参数的可见性规则

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/default_arguments
- C++ Standard: [dcl.fct.default]
- Effective C++, Scott Meyers, Item 22-24
- C++ Templates: The Complete Guide, David Vandevoorde et al.