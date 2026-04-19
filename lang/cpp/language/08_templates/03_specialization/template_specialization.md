# 模板显式特化 (Explicit Template Specialization)

## 1. 概述

模板显式特化（Explicit Template Specialization），也称完全特化（Full Specialization），是 C++ 模板机制中的重要特性。它允许开发者为特定的模板参数组合提供定制化的实现，覆盖主模板（Primary Template）的默认行为。

### 核心概念

- **主模板（Primary Template）**：定义通用行为的原始模板
- **显式特化**：为特定模板参数提供的专门实现
- **特化声明**：使用 `template <>` 语法声明特化版本

### 可特化的模板类型

以下 10 种模板可以被完全特化：

| 序号 | 模板类型 | 说明 |
|------|---------|------|
| 1 | 函数模板（Function Template） | 全局函数模板 |
| 2 | 类模板（Class Template） | 全局类模板 |
| 3 | 变量模板（Variable Template） | C++14 起 |
| 4 | 类模板的成员函数 | 类模板中的成员函数 |
| 5 | 类模板的静态数据成员 | 类模板中的静态成员变量 |
| 6 | 类模板的成员类 | 类模板中的嵌套类 |
| 7 | 类模板的成员枚举 | 类模板中的枚举类型 |
| 8 | 类或类模板的成员类模板 | 嵌套的类模板 |
| 9 | 类或类模板的成员函数模板 | 嵌套的函数模板 |
| 10 | 类或类模板的成员变量模板 | C++14 起 |

## 2. 来源与演变

### 首次引入

模板显式特化在 **C++98** 标准中首次引入，是 C++ 模板系统的基础特性之一。

### 设计动机

模板特化的设计动机包括：

1. **类型特定优化**：针对特定类型提供更高效的实现
2. **特殊语义处理**：某些类型需要特殊处理逻辑
3. **类型萃取实现**：是实现类型萃取（Type Traits）的核心技术
4. **避免代码膨胀**：为特定类型提供专门实现，减少模板实例化

### 标准演进

| 版本 | 变化内容 |
|------|---------|
| C++98 | 首次引入显式特化语法 |
| C++11 | 新增 `constexpr`、`noexcept` 说明符处理规则；属性（attributes）不继承到特化 |
| C++14 | 支持变量模板的显式特化 |
| C++20 | 明确 `constinit` 和 `consteval` 不从主模板继承到特化 |

### 缺陷报告修正

| DR 编号 | 适用版本 | 原有行为 | 修正行为 |
|---------|---------|---------|---------|
| CWG 531 | C++98 | 未指定命名空间作用域中定义特化成员的语法 | 已指定 |
| CWG 727 | C++98 | 不允许在类作用域中进行部分和完全特化 | 允许在任何作用域中特化 |
| CWG 730 | C++98 | 非模板类的成员模板不能被完全特化 | 允许特化 |
| CWG 2478 | C++20 | 不明确主模板的 `constinit` 和 `consteval` 是否继承到特化 | 不继承 |
| CWG 2604 | C++11 | 不明确主模板的属性（attributes）是否继承到特化 | 不继承 |

## 3. 语法与参数

### 基本语法

```cpp
template <> declaration
```

其中 `declaration` 是针对特定模板参数的具体实现声明。

### 函数模板特化语法

```cpp
// 主模板
template<typename T>
void func(T param);

// 显式特化
template<>
void func<int>(int param);  // 显式指定模板参数

// 或省略模板参数（当可从函数参数推导时）
template<>
void func(int param);        // 模板参数推导
```

### 类模板特化语法

```cpp
// 主模板
template<typename T>
class MyClass {
    // 通用实现
};

// 显式特化
template<>
class MyClass<int> {
    // 针对 int 的专门实现
};
```

### 参数说明

| 语法元素 | 说明 |
|---------|------|
| `template <>` | 空模板参数列表，表示这是一个完全特化 |
| `<具体类型>` | 特化的目标类型，如 `<int>`、`<std::string>` |
| `declaration` | 函数声明或类定义 |

### 特化声明规则

1. **声明位置**：特化必须出现在主模板声明之后
2. **命名空间**：特化可以在与主模板相同的命名空间中声明
3. **前置声明**：特化可以前置声明，后续再定义
4. **使用前声明**：特化必须在首次使用（导致隐式实例化）之前声明

## 4. 底层原理

### 编译器处理机制

当编译器遇到模板实例化请求时，按以下顺序匹配：

```
1. 检查是否存在显式特化 -> 使用特化版本
2. 检查是否存在部分特化 -> 选择最特化版本
3. 使用主模板 -> 实例化通用版本
```

### 特化优先级

```
显式特化 > 部分特化 > 主模板
```

### 实例化时机

| 实例化类型 | 说明 |
|-----------|------|
| 隐式实例化 | 代码使用模板时自动触发 |
| 显式实例化 | 使用 `template class MyClass<int>;` 语法 |
| 显式特化 | 提供完全定制的实现，不进行通用实例化 |

### 函数模板 vs 类模板特化

| 特性 | 函数模板特化 | 类模板特化 |
|------|-------------|-----------|
| 参数推导 | 支持 | 不支持 |
| 重载 | 不参与重载决议 | N/A |
| 默认参数 | 不能在特化中指定 | 可在特化中指定 |

### 说明符继承规则

显式特化中的 `inline`、`constexpr`、`constinit`、`consteval` 由特化本身决定，**不继承**自主模板：

```cpp
template<class T>
inline void f(T) { }  // 主模板是 inline

template<>
void f<>(int) { }      // 特化版本不是 inline

template<class T>
void g(T) { }          // 主模板不是 inline

template<>
inline void g<>(int) { }  // 特化版本是 inline
```

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| 类型萃取（Type Traits） | 为特定类型提供特化的 `true_type`/`false_type` |
| 性能优化 | 针对特定类型提供更高效的算法实现 |
| 平台适配 | 针对不同平台提供特化实现 |
| 特殊语义处理 | 如指针类型的特殊处理 |
| 避免编译错误 | 某些类型无法使用通用模板实现 |

### 最佳实践

1. **保持接口一致**：特化版本应与主模板保持相同的接口语义
2. **文档说明**：清晰文档化特化的原因和行为差异
3. **优先使用重载**：函数模板优先考虑重载而非特化
4. **避免过度特化**：只在确实需要时进行特化

### 常见陷阱

#### 陷阱 1：特化声明顺序错误

```cpp
// 错误示例：特化在隐式实例化之后
template<class T>
void sort(Array<T>& v);

void f(Array<String>& v) {
    sort(v);  // 隐式实例化 sort(Array<String>&)
}

template<>  // 错误！已在前面隐式实例化
void sort<String>(Array<String>& v);
```

#### 陷阱 2：混淆函数重载与特化

```cpp
// 这不是特化！这是一个独立的非模板函数
void func(int x) { }

// 这才是特化
template<typename T>
void func(T x);

template<>
void func<int>(int x) { }
```

#### 陷阱 3：默认参数在特化中重复

```cpp
template<typename T>
void func(T x = T{});  // 主模板有默认参数

template<>
void func<int>(int x = 0);  // 错误！不能在特化中指定默认参数

// 正确做法
template<>
void func<int>(int x);  // 特化中不指定默认参数
```

## 6. 代码示例

### 基础用法：类型萃取

```cpp
#include <type_traits>

// 主模板：默认情况
template<typename T>
struct is_void : std::false_type {};

// 显式特化：T = void
template<>
struct is_void<void> : std::true_type {};

int main()
{
    static_assert(is_void<char>::value == false,
        "for any type T other than void, the class is derived from false_type");
    static_assert(is_void<void>::value == true,
        "but when T is void, the class is derived from true_type");
}
```

### 类模板特化

```cpp
#include <iostream>

// 主模板
template<typename T>
class Container {
public:
    void print() {
        std::cout << "Generic container" << std::endl;
    }
};

// 显式特化：针对 int
template<>
class Container<int> {
public:
    void print() {
        std::cout << "Integer container" << std::endl;
    }
};

int main() {
    Container<double> c1;
    c1.print();  // 输出: Generic container

    Container<int> c2;
    c2.print();  // 输出: Integer container

    return 0;
}
```

### 函数模板特化

```cpp
#include <iostream>
#include <cstring>

// 主模板
template<typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

// 显式特化：针对 const char*
template<>
const char* max_value<const char*>(const char* a, const char* b) {
    return (strcmp(a, b) > 0) ? a : b;
}

int main() {
    std::cout << max_value(3, 5) << std::endl;          // 输出: 5
    std::cout << max_value("apple", "banana") << std::endl;  // 输出: banana

    return 0;
}
```

### 成员函数特化

```cpp
template<typename T>
struct A {
    void f(T);           // 在主模板中声明
    void h(T) {}         // 在主模板中定义

    template<class X>    // 成员模板
    void g1(T, X);
};

// 特化成员函数
template<>
void A<int>::f(int) { /* ... */ }

// 成员函数特化（即使在类内已定义）
template<>
void A<int>::h(int) { /* ... */ }

// 成员模板特化
template<>
template<class X>
void A<int>::g1(int, X) { /* ... */ }
```

### 命名空间中的特化

```cpp
namespace N {
    template<class T>  // 主模板
    class X { /*...*/ };

    template<>         // 同命名空间中的特化
    class X<int> { /*...*/ };

    template<class T>  // 主模板
    class Y { /*...*/ };

    template<>         // 特化的前置声明
    class Y<double>;
}

// 在命名空间外定义特化
template<>
class N::Y<double> { /*...*/ };  // 正确
```

### 高级用法：嵌套模板特化

```cpp
template<class T1>
struct A {
    template<class T2>
    struct B {
        template<class T3>
        void mf();
    };
};

// 完全特化 A<int>
template<>
struct A<int>;

// 完全特化 A<char>::B<double>
template<>
template<>
struct A<char>::B<double>;

// 完全特化 A<char>::B<char>::mf<double>()
template<>
template<>
template<>
void A<char>::B<char>::mf<double>();
```

### 常见错误及修正

#### 错误 1：在错误位置定义特化

```cpp
// 错误示例：特化定义在首次使用之后
template<class T>
class Array { /*...*/ };

void useArray() {
    Array<int> arr;  // 隐式实例化主模板
}

template<>  // 错误！
class Array<int> { /* 特化版本 */ };
```

```cpp
// 正确示例：特化在首次使用之前声明
template<class T>
class Array;

template<>  // 正确：特化声明在使用之前
class Array<int> { /* 特化版本 */ };

void useArray() {
    Array<int> arr;  // 使用特化版本
}
```

#### 错误 2：静态成员初始化语法错误

```cpp
template<typename T>
struct Q {
    static X x;
};

// 错误：这是函数声明，不是变量定义
template<>
X Q<int>::x();

// 正确：使用大括号进行默认初始化
template<>
X Q<int>::x {};
```

## 7. 总结

### 核心要点

| 要点 | 说明 |
|------|------|
| 语法 | `template <> declaration` |
| 优先级 | 显式特化 > 部分特化 > 主模板 |
| 声明时机 | 必须在首次隐式实例化之前 |
| 说明符继承 | `inline`/`constexpr`/`constinit`/`consteval`/属性不继承 |

### 显式特化 vs 部分特化

| 特性 | 显式特化 | 部分特化 |
|------|---------|---------|
| 模板参数 | 全部指定 | 部分指定 |
| 语法 | `template <>` | `template <部分参数>` |
| 适用对象 | 类模板、函数模板、变量模板 | 仅类模板和变量模板 |
| 函数模板 | 支持（但不参与重载） | 不支持 |

### 学习建议

1. **理解实例化顺序**：掌握编译器选择特化版本的规则
2. **区分重载与特化**：函数模板重载优先于特化
3. **注意声明顺序**：特化必须在首次使用前声明
4. **实践类型萃取**：通过实现简单的 type traits 加深理解

### 相关概念

| 概念 | 关系 |
|------|------|
| 主模板（Primary Template） | 特化的基础模板 |
| 部分特化（Partial Specialization） | 只指定部分模板参数 |
| 显式实例化（Explicit Instantiation） | 强制实例化，不提供新实现 |
| 重载决议（Overload Resolution） | 函数模板特化不参与重载 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/template_specialization
- C++ Standard: [temp.expl.spec]
- Effective C++, Scott Meyers