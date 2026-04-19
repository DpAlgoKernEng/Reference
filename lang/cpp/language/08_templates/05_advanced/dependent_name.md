# Dependent Name - 依赖名

## 1. 概述

在 C++ 模板（包括类模板和函数模板）的定义中，某些构造的含义可能因实例化而异。具体来说，类型和表达式可能依赖于类型模板参数的类型或非类型模板参数的值。这类名称被称为**依赖名（Dependent Name）**。

依赖名是 C++ 模板元编程的核心概念之一，它决定了名称查找和绑定的时机与方式：

- **非依赖名（Non-dependent Name）**：在模板定义时即进行查找和绑定
- **依赖名（Dependent Name）**：推迟到模板实例化时才进行查找和绑定

理解依赖名对于正确编写模板代码、避免常见的编译错误至关重要，特别是在使用嵌套类型、模板成员等场景中。

## 2. 来源与演变

### 历史背景

依赖名的概念源于 C++ 模板的**两阶段查找（Two-phase Lookup）**机制：

1. **第一阶段（定义阶段）**：模板定义时，编译器检查模板的语法正确性，并对非依赖名进行查找和绑定
2. **第二阶段（实例化阶段）**：模板实例化时，编译器对依赖名进行查找和绑定

这种设计确保了模板的灵活性和类型安全。

### C++ 版本演进

| 版本 | 变化内容 |
|------|----------|
| C++98 | 引入依赖名概念，确立两阶段查找机制 |
| C++11 | 引入 `decltype` 产生唯一的依赖类型；新增 `sizeof...`、`alignof`、`noexcept` 等非类型依赖表达式 |
| C++14 | 依赖名规则适用于变量模板特化；返回类型推导相关规则 |
| C++17 | 引入折叠表达式（fold expression）；using-declaration 包展开为空时的查找规则 |
| C++20 | `requires` 表达式永远不会类型依赖；特定上下文中依赖名无需 `typename` |
| C++23 | 成员访问表达式中非依赖模板名的查找规则调整 |
| C++26 | 引入包索引说明符和包索引表达式的依赖类型规则 |

### 缺陷报告（Defect Reports）

多个 CWG（Core Working Group）缺陷报告完善了依赖名的定义和行为，例如：

- **CWG 382**：`typename` 消歧符允许在模板作用域外使用
- **CWG 468**：`template` 消歧符允许在模板作用域外使用
- **CWG 1413**：明确了哪些表达式是值依赖的

## 3. 语法与参数

### typename 消歧符

在模板定义中，当一个依赖名表示类型时，必须使用 `typename` 关键字进行消歧：

```cpp
template<typename T>
void foo(const std::vector<T>& v)
{
    // 正确：明确指出 const_iterator 是类型
    typename std::vector<T>::const_iterator it = v.begin();

    // 错误：会被解析为乘法表达式
    // std::vector<T>::const_iterator* p;
}
```

**语法形式**：
```cpp
typename nested-name-specifier identifier
typename nested-name-specifier template<...> identifier
```

**使用位置**：
- 模板定义中声明类型别名
- 函数参数类型声明
- 返回类型声明
- 成员变量声明

### template 消歧符

当一个依赖名表示模板时，必须使用 `template` 关键字进行消歧：

```cpp
template<typename T>
struct S {
    template<typename U>
    void foo() {}
};

template<typename T>
void bar()
{
    S<T> s;
    s.template foo<T>();  // 正确：明确指出 foo 是模板
    // s.foo<T>();        // 错误：< 被解析为小于运算符
}
```

**语法形式**：
```cpp
expression.template identifier<...>
nested-name-specifier template identifier<...>
```

**使用位置**：
- 在 `::`（作用域解析运算符）之后
- 在 `->`（指针成员访问运算符）之后
- 在 `.`（成员访问运算符）之后

### C++20 简化规则

在以下上下文中，依赖名自动被视为类型名，无需 `typename`：

- 命名空间作用域的简单声明或函数定义的声明说明符序列
- 类成员声明
- 参数声明
- 别名声明中的类型标识
- 尾置返回类型
- 类型模板参数的默认参数
- 类型转换表达式的类型标识

## 4. 底层原理

### 绑定规则

#### 非依赖名绑定

非依赖名在模板定义时就进行查找和绑定，即使实例化时存在更好的匹配：

```cpp
#include <iostream>

void g(double) { std::cout << "g(double)\n"; }

template<class T>
struct S {
    void f() const {
        g(1);  // "g" 是非依赖名，在此处绑定
    }
};

void g(int) { std::cout << "g(int)\n"; }

int main() {
    g(1);      // 调用 g(int)

    S<int> s;
    s.f();     // 调用 g(double)！
}
```

#### 依赖名绑定

依赖名的绑定推迟到查找发生时：

- **非 ADL 查找**：检查模板定义上下文中可见的外部链接函数声明
- **ADL 查找**：检查模板定义上下文或模板实例化上下文中可见的外部链接函数声明

### 依赖类型（Dependent Types）

以下类型属于依赖类型：

| 类型类别 | 说明 |
|----------|------|
| 模板参数 | `T`、`typename T::type` 等 |
| 未知特化的成员 | `T::type` 当 `T` 为依赖类型时 |
| 依赖类型的 cv 限定版本 | `const T&` 当 `T` 为依赖类型时 |
| 由依赖类型构造的复合类型 | 指向依赖类型的指针、引用等 |
| 元素类型或边界依赖的数组类型 | `T[N]` 当 `T` 或 `N` 为依赖时 |
| 模板标识符 | `A<T>` 当 `T` 为依赖时 |
| `decltype` 应用于类型依赖表达式 | `decltype(t)` 当 `t` 为类型依赖时 |
| 函数参数包 | 包含函数参数包的函数类型 |

### 类型依赖表达式（Type-dependent Expressions）

以下表达式是类型依赖的：

- 任何子表达式为类型依赖表达式的表达式
- `this`（当类为依赖类型时）
- 包含依赖声明的标识符表达式
- 包含依赖模板标识符的表达式
- 转换到依赖类型的转型表达式
- 创建依赖类型对象的 `new` 表达式
- 引用依赖类型成员的成员访问表达式

**永不类型依赖的表达式**：

- 字面量
- 伪析构函数调用
- `sizeof`、`sizeof...`、`alignof`、`noexcept` 表达式
- `throw` 表达式
- `typeid` 表达式
- `delete` 表达式
- `requires` 表达式（C++20）

### 值依赖表达式（Value-dependent Expressions）

以下表达式是值依赖的：

- 在需要常量表达式的上下文中使用的表达式，且其子表达式为值依赖
- 类型依赖的标识符表达式
- 非类型模板参数的名称
- 从值依赖表达式初始化的常量
- 操作数为类型依赖表达式的 `sizeof`、`typeid`、`alignof`

### 当前实例化（Current Instantiation）

在类模板定义中，某些名称可以被推导为引用**当前实例化**：

```cpp
template<class T>
class A {
    A* p1;       // A 是当前实例化
    A<T>* p2;    // A<T> 是当前实例化
    A<T*> p3;    // A<T*> 不是当前实例化
};
```

**判定规则**：
- 模板参数等价于模板参数本身时，为当前实例化
- 嵌套类的注入类名
- 偏特化定义中的特化名称

### 未知特化（Unknown Specializations）

某些名称被推导为属于**未知特化**：

```cpp
template<typename T>
struct Base {};

template<typename T>
struct Derived : Base<T> {
    void f() {
        // Derived<T> 是当前实例化
        // 但没有 "unknown_type" 在当前实例化中
        // 存在依赖基类 Base<T>
        // 因此 "unknown_type" 是未知特化的成员
        typename Derived<T>::unknown_type z;
    }
};
```

## 5. 使用场景

### 典型应用场景

| 场景 | 说明 |
|------|------|
| 访问模板参数的嵌套类型 | 使用 `typename T::type` |
| 调用模板参数的模板成员 | 使用 `T::template foo<X>()` |
| 访问容器迭代器类型 | `typename std::vector<T>::iterator` |
| 模板元编程 | 类型萃取、SFINAE 等技术 |

### 最佳实践

#### 1. 始终使用 typename 声明依赖类型

```cpp
// 推荐
template<typename T>
void process(typename T::value_type value) {
    typename T::iterator it;
}

// 错误：缺少 typename
template<typename T>
void process(T::value_type value) {  // 编译错误
    T::iterator it;  // 编译错误
}
```

#### 2. 使用 template 访问依赖模板成员

```cpp
template<typename T>
void call_foo() {
    T t;
    t.template bar<int>();  // 正确
    // t.bar<int>();        // 错误：< 被解析为小于运算符
}
```

#### 3. 利用 ADL 进行参数依赖查找

```cpp
namespace MyLib {
    struct Data {};

    void serialize(const Data& d) { /* ... */ }
}

template<typename T>
void save(const T& obj) {
    serialize(obj);  // ADL 会查找 T 所在命名空间中的 serialize
}

// 使用
MyLib::Data d;
save(d);  // 会调用 MyLib::serialize
```

### 常见陷阱

#### 陷阱 1：非依赖名的早期绑定

```cpp
void g(double);

template<class T>
struct S {
    void f() { g(1); }  // g 绑定到 g(double)
};

void g(int);  // 太晚了

S<int> s;
s.f();  // 调用 g(double)，而非 g(int)
```

#### 陷阱 2：依赖基类的名称隐藏

```cpp
template<typename T>
struct Base {
    void foo() {}
};

template<typename T>
struct Derived : Base<T> {
    void bar() {
        // foo();       // 错误：不查找依赖基类
        this->foo();    // 正确：通过 this 访问
        Base<T>::foo(); // 正确：显式指定基类
    }
};
```

#### 陷阱 3：无法为标准库类型重载运算符

```cpp
// 问题：std::pair 在 std 命名空间中
std::ostream& operator<<(std::ostream& os, std::pair<int, int> p) {
    return os << p.first << ',' << p.second;
}

// 某些模板上下文中无法找到此运算符
std::copy(v.begin(), v.end(),
          std::ostream_iterator<std::pair<int, int>>(std::cout, " "));
// 错误：ADL 只查找 std 命名空间，找不到全局的 operator<<
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <vector>
#include <list>

// 示例 1：使用 typename 声明依赖类型
template<typename Container>
void print_first(const Container& c) {
    // 必须使用 typename，因为 iterator 是依赖类型
    typename Container::const_iterator it = c.begin();
    if (it != c.end()) {
        std::cout << *it << std::endl;
    }
}

// 示例 2：在成员函数中使用 typename
template<typename T>
class Wrapper {
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    // 类型别名不需要 typename（已确定为类型）
    using size_type = typename std::vector<T>::size_type;

    void process(const T& value) {
        // 访问当前实例化的成员，不需要 typename
        value_type v = value;
    }
};

int main() {
    std::vector<int> v = {1, 2, 3};
    print_first(v);

    std::list<double> l = {1.5, 2.5, 3.5};
    print_first(l);

    Wrapper<int> w;
    w.process(42);

    return 0;
}
```

### 高级用法

```cpp
#include <iostream>
#include <type_traits>

// 示例 3：使用 template 访问依赖模板成员
template<typename T>
struct Traits {
    template<typename U>
    using rebind = U;

    template<typename U>
    static void print_type() {
        std::cout << "Type: " << typeid(U).name() << std::endl;
    }
};

template<typename T>
void use_traits() {
    // 必须使用 template 关键字
    typename Traits<T>::template rebind<double> d = 3.14;
    Traits<T>::template print_type<int>();
}

// 示例 4：当前实例化与未知特化
template<typename T>
class Container {
public:
    using iterator = T*;

    void foo() {
        // 当前实例化的成员，不需要 typename
        iterator it = nullptr;

        // 显式引用当前实例化
        typename Container<T>::iterator it2 = nullptr;
    }
};

template<typename T>
struct Base {
    using type = int;
};

template<typename T>
struct Derived : Base<T> {
    void test() {
        // 错误：type 是依赖基类的成员，不会被查找
        // type x = 0;

        // 正确：显式指定
        typename Base<T>::type x = 0;

        // 正确：通过 this 访问
        // 但对于类型，仍需要 typename
    }
};

// 示例 5：使用 using 简化依赖类型
template<typename T>
class Simplified {
public:
    // 一次性使用 typename
    using value_type = typename std::vector<T>::value_type;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    void process(const_iterator begin, const_iterator end) {
        // 使用简化的类型别名，无需 typename
        for (iterator it = begin; it != end; ++it) {
            // ...
        }
    }
};

// 示例 6：SFINAE 和依赖类型
template<typename T, typename = void>
struct has_type : std::false_type {};

template<typename T>
struct has_type<T, std::void_t<typename T::type>> : std::true_type {};

struct WithType { using type = int; };
struct WithoutType {};

static_assert(has_type<WithType>::value, "WithType has type");
static_assert(!has_type<WithoutType>::value, "WithoutType has no type");

int main() {
    use_traits<int>();
    return 0;
}
```

### 常见错误及修正

#### 错误 1：遗漏 typename 关键字

```cpp
// 错误：缺少 typename
template<typename T>
void wrong() {
    T::iterator it;  // 编译错误：T::iterator 被解析为乘法
}

// 修正：添加 typename
template<typename T>
void correct() {
    typename T::iterator it;  // 正确
}
```

#### 错误 2：遗漏 template 关键字

```cpp
template<typename T>
struct A {
    template<typename U>
    void foo() {}
};

// 错误：缺少 template
template<typename T>
void wrong() {
    A<T> a;
    a.foo<int>();  // 编译错误：< 被解析为小于运算符
}

// 修正：添加 template
template<typename T>
void correct() {
    A<T> a;
    a.template foo<int>();  // 正确
}
```

#### 错误 3：依赖基类的名称查找

```cpp
template<typename T>
struct Base {
    int value = 42;
    void foo() {}
};

// 错误：依赖基类的成员不会被查找
template<typename T>
struct DerivedWrong : Base<T> {
    void bar() {
        // int x = value;    // 错误：找不到 value
        // foo();            // 错误：找不到 foo
    }
};

// 修正：使用 this-> 或 Base<T>::
template<typename T>
struct DerivedCorrect : Base<T> {
    void bar() {
        int x = this->value;        // 正确
        this->foo();                // 正确
        int y = Base<T>::value;    // 正确
        Base<T>::foo();            // 正确
    }
};
```

#### 错误 4：非依赖名的意外绑定

```cpp
#include <iostream>

void helper(double) { std::cout << "double\n"; }

template<typename T>
void wrong_example(T t) {
    helper(t);  // helper 是非依赖名，绑定到 helper(double)
}

void helper(int) { std::cout << "int\n"; }

int main() {
    wrong_example(42);  // 输出 "double"，而非预期的 "int"
    return 0;
}

// 修正方法：将 helper 放在模板定义之前
void helper_correct(double) { std::cout << "double\n"; }
void helper_correct(int) { std::cout << "int\n"; }

template<typename T>
void correct_example(T t) {
    helper_correct(t);  // 现在两个重载都可见
}

// 或使用 ADL
namespace N {
    struct MyType {};
    void helper_adl(MyType) { std::cout << "MyType\n"; }
}

template<typename T>
void adl_example(T t) {
    helper_adl(t);  // ADL 会查找 T 所在命名空间
}
```

## 7. 总结

### 核心要点

1. **两阶段查找**：模板编译分为定义阶段和实例化阶段，非依赖名在定义阶段绑定，依赖名在实例化阶段绑定

2. **typename 消歧符**：在模板中使用依赖类型名时必须使用 `typename` 前缀（C++20 特定上下文除外）

3. **template 消歧符**：在模板中访问依赖模板成员时必须使用 `template` 前缀

4. **当前实例化 vs 未知特化**：理解名称是属于当前实例化还是未知特化，影响错误检测时机

5. **依赖基类的名称隐藏**：依赖基类中的名称不会通过非限定查找找到，需要使用 `this->` 或 `Base<T>::` 访问

### 决策指南

| 场景 | 需要使用 | 示例 |
|------|----------|------|
| 访问 `T::type` 等嵌套类型 | `typename` | `typename T::iterator` |
| 访问 `T::template_func<X>()` 等模板成员 | `template` | `T::template func<X>()` |
| 访问依赖基类成员 | `this->` 或基类限定 | `this->member` |
| 非依赖名称查找 | 定义时绑定 | 注意重载函数位置 |

### 学习建议

1. 理解模板的两阶段编译模型是掌握依赖名的关键
2. 使用类型别名（`using`）简化复杂的依赖类型声明
3. 在 IDE 中观察编译器错误信息，理解为何需要 `typename` 或 `template`
4. 遵循"依赖名必须消歧"的原则，避免难以理解的编译错误

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/dependent_name
- C++ Standard: [temp.dep], [temp.dep.type], [temp.dep.expr]
- "C++ Templates: The Complete Guide" by David Vandevoorde, Nicolai M. Josuttis, Douglas Gregor
- Effective C++, Scott Meyers, Item 46