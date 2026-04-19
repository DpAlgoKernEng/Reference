# 类型别名与别名模板 (Type Alias, Alias Template)

## 1. 概述

**类型别名 (Type Alias)** 是 C++11 引入的一种为已定义类型创建同义词的机制，功能类似于传统的 `typedef` 声明，但语法更加清晰直观。

**别名模板 (Alias Template)** 是类型别名的模板化形式，它允许创建一个代表类型族的名称，通过模板参数化来定义一族类型别名。

类型别名声明使用 `using` 关键字，是 C++ 现代编程风格中替代 `typedef` 的推荐方式。相比 `typedef`，`using` 语法更加统一，并且支持模板化。

## 2. 来源与演变

### 首次引入

类型别名和别名模板在 **C++11** 标准中引入，作为对传统 `typedef` 机制的现代化改进。

### 历史背景

在 C++11 之前，创建类型别名只能使用 `typedef` 关键字：

```cpp
// 旧式 typedef 语法
typedef std::ios_base::fmtflags flags;
typedef void (*func)(int, int);  // 函数指针别名
```

`typedef` 存在以下局限性：
1. **语法不够直观**：尤其是函数指针等复杂类型的别名定义
2. **不支持模板化**：无法创建依赖于模板参数的类型别名
3. **可读性差**：对于复杂类型，理解成本较高

C++11 引入 `using` 语法解决了这些问题：
- 语法形式统一：`using 别名 = 原类型;`
- 支持模板化：可以创建别名模板
- 更好的可读性：别名名称在前，原类型在后

### C++20 增强

C++20 为别名模板增加了约束支持，允许使用 `requires` 子句限制模板参数：

```cpp
template<typename T>
requires std::is_integral_v<T>
using IntegralPtr = T*;
```

### 功能测试宏

| 功能测试宏 | 值 | 标准 | 功能 |
|-----------|-----|------|------|
| `__cpp_alias_templates` | `200704L` | C++11 | 别名模板 |

## 3. 语法与参数

### 基本语法

类型别名声明有以下三种形式：

| 形式 | 语法 | 说明 |
|------|------|------|
| (1) | `using identifier attr(optional) = type-id ;` | 类型别名声明 |
| (2) | `template < template-parameter-list > using identifier attr(optional) = type-id ;` | 别名模板声明 |
| (3) | `template < template-parameter-list > requires constraint using identifier attr(optional) = type-id ;` | 带约束的别名模板 (C++20) |

### 参数说明

| 参数 | 说明 |
|------|------|
| `attr` | 可选的属性序列，可应用于类型别名 |
| `identifier` | 此声明引入的名称，类型别名 (1) 中成为类型名，别名模板 (2) 中成为模板名 |
| `template-parameter-list` | 模板参数列表，与模板声明中的格式相同 |
| `constraint` | 约束表达式，限制别名模板接受的模板参数 (C++20) |
| `type-id` | 抽象声明符或任何有效的类型标识符。注意：type-id 不能直接或间接引用 identifier 自身 |

### 声明位置

- **类型别名声明 (1)**：可出现在块作用域、类作用域或命名空间作用域
- **别名模板 (2, 3)**：只能在类作用域或命名空间作用域声明

### 与 typedef 的等价性

类型别名声明与 typedef 声明在语义上完全等价：

```cpp
// 以下两种写法完全等价
using flags = std::ios_base::fmtflags;
typedef std::ios_base::fmtflags flags;

// 函数指针别名
using func = void (*)(int, int);
typedef void (*func)(int, int);  // 等价的 typedef 形式
```

## 4. 底层原理

### 类型别名的本质

类型别名**不引入新类型**，它只是为现有类型创建一个同义词。类型别名和原类型完全等价，可以互换使用：

```cpp
using IntPtr = int*;
IntPtr p1 = nullptr;
int* p2 = p1;      // 完全合法，IntPtr 和 int* 是同一类型
```

### 别名模板的实例化机制

当别名模板被特化时，编译器执行以下步骤：
1. 将模板实参代入模板参数
2. 在 type-id 中替换所有模板参数
3. 生成最终的类型

```cpp
template<class T>
using Ptr = T*;

Ptr<int> p;  // 等价于 int* p;
```

### 依赖类型的后续替换

当别名模板特化的结果是依赖模板 ID 时，后续的替换会继续应用于该模板 ID：

```cpp
template<typename...>
using void_t = void;

template<typename T>
void_t<typename T::foo> f();

f<int>();  // 错误：int 没有嵌套类型 foo
// 替换失败发生在 void_t<typename T::foo> 的实例化过程中
```

### 禁止递归引用

别名模板产生的类型**不允许直接或间接使用其自身类型**：

```cpp
template<class T>
struct A;

template<class T>
using B = typename A<T>::U;  // type-id 是 A<T>::U

template<class T>
struct A { typedef B<T> U; };

B<short> b;  // 错误：B<short> 通过 A<short>::U 使用了自身类型
```

### 模板参数推导的限制

别名模板**不参与模板参数推导**，当推导模板模板参数时，编译器不会考虑别名模板：

```cpp
template<template<class> class Container>
void func(Container<int>);

template<class T>
using MyVec = std::vector<T>;

// func(MyVec<int>{});  // 错误：无法推导 Container 为 MyVec
```

### 禁止特化

别名模板**不能被部分特化或完全特化**：

```cpp
template<class T>
using Ptr = T*;

// 以下都是非法的：
// template<> using Ptr<int> = int*;           // 错误：不能完全特化
// template<class T> using Ptr<T*> = T**;      // 错误：不能部分特化
```

### C++20 Lambda 类型差异

在 C++20 中，别名模板声明中出现的 Lambda 表达式，即使不依赖模板参数，在不同的模板实例化中也会产生不同的闭包类型：

```cpp
template<class T>
using A = decltype([] {});  // C++20 起

// A<int> 和 A<char> 引用不同的闭包类型
static_assert(!std::is_same_v<A<int>, A<char>>);  // 通过
```

## 5. 使用场景

### 适合使用类型别名的场景

| 场景 | 示例 |
|------|------|
| 简化复杂类型声明 | `using Func = void (*)(int, int);` |
| 隐藏模板参数 | `using String = std::basic_string<char>;` |
| 提高代码可读性 | `using size_type = std::size_t;` |
| 泛型编程中的类型萃取 | `using type = typename T::type;` |

### 适合使用别名模板的场景

| 场景 | 示例 |
|------|------|
| 简化模板类型 | `template<class T> using Vec = std::vector<T, MyAlloc<T>>;` |
| 类型萃取 | `template<class T> using Invoke = typename T::type;` |
| SFINAE 技巧 | `template<class...> using void_t = void;` |
| 条件类型约束 | `template<bool B, class T> using EnableIf = std::enable_if_t<B, T>;` |

### 最佳实践

1. **优先使用 `using` 而非 `typedef`**：语法更清晰，支持模板化

```cpp
// 推荐
using Callback = void (*)(int, int);
using StringMap = std::map<std::string, std::string>;

// 不推荐
typedef void (*Callback)(int, int);
typedef std::map<std::string, std::string> StringMap;
```

2. **使用别名模板简化复杂模板类型**：

```cpp
// 隐藏模板参数细节
template<class CharT>
using mystring = std::basic_string<CharT, std::char_traits<CharT>>;

mystring<char> s1;    // std::string
mystring<wchar_t> s2; // std::wstring
```

3. **在泛型编程中使用类型别名**：

```cpp
template<typename Container>
void process(const Container& c) {
    using value_type = typename Container::value_type;
    // 使用 value_type 进行泛型操作
}
```

### 常见陷阱

1. **别名模板不支持特化**

```cpp
template<class T>
using Ptr = T*;

// 错误：无法特化别名模板
// template<> using Ptr<void> = void;  // 编译错误

// 替代方案：使用 traits 类
template<class T>
struct PtrTraits { using type = T*; };

template<>
struct PtrTraits<void> { using type = void; };

template<class T>
using Ptr = typename PtrTraits<T>::type;
```

2. **类型别名不引入新类型**

```cpp
using IntPtr = int*;
using IntPtr2 = int*;

// IntPtr 和 IntPtr2 是同一类型，不是不同类型
static_assert(std::is_same_v<IntPtr, IntPtr2>);  // 通过
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>

// 类型别名 - 等价于 typedef
using flags = std::ios_base::fmtflags;
flags fl = std::ios_base::dec;

// 函数指针别名
using func = void (*)(int, int);

void example(int a, int b) {
    std::cout << "Called with " << a << " and " << b << std::endl;
}

// 别名模板
template<class T>
using ptr = T*;

int main() {
    // 使用函数指针别名
    func f = example;
    f(1, 2);

    // 使用别名模板
    ptr<int> x = new int(42);
    std::cout << "Value: " << *x << std::endl;
    delete x;

    return 0;
}
```

### 隐藏模板参数

```cpp
#include <string>

// 隐藏复杂的模板参数
template<class CharT>
using mystring = std::basic_string<CharT, std::char_traits<CharT>>;

int main() {
    mystring<char> s1 = "Hello";      // std::string
    mystring<wchar_t> s2 = L"World";  // std::wstring

    return 0;
}
```

### 泛型编程中的应用

```cpp
#include <iostream>
#include <typeinfo>

// 定义成员类型别名
template<typename T>
struct Container {
    using value_type = T;
};

// 使用成员类型别名进行泛型编程
template<typename ContainerT>
void info(const ContainerT& c) {
    typename ContainerT::value_type T;
    std::cout << "ContainerT is `" << typeid(decltype(c)).name() << "`\n"
              << "value_type is `" << typeid(T).name() << "`\n";
}

int main() {
    Container<int> c;
    info(c);
    return 0;
}
```

### 简化 enable_if 语法

```cpp
#include <type_traits>

// 类型萃取辅助别名
template<typename T>
using Invoke = typename T::type;

template<typename Condition>
using EnableIf = Invoke<std::enable_if<Condition::value>>;

// 使用别名简化 SFINAE
template<typename T, typename = EnableIf<std::is_polymorphic<T>>>
int polymorphic_only(T) { return 1; }

struct Polymorphic { virtual ~Polymorphic() {} };
struct NotPolymorphic {};

int main() {
    Polymorphic p;
    polymorphic_only(p);  // OK

    // NotPolymorphic np;
    // polymorphic_only(np);  // 编译错误：enable_if 禁止此调用

    return 0;
}
```

### void_t 类型萃取技巧

```cpp
#include <type_traits>

// SFINAE 常用技巧：检测类型是否具有某个成员
template<typename, typename = void>
struct has_type_member : std::false_type {};

template<typename T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

// 使用示例
struct WithType { using type = int; };
struct WithoutType {};

static_assert(has_type_member<WithType>::value);     // 通过
static_assert(!has_type_member<WithoutType>::value); // 通过
```

### 常见错误及修正

#### 错误 1：递归引用

```cpp
// 错误：别名模板间接引用自身
template<class T>
struct A;

template<class T>
using B = typename A<T>::U;  // type-id 引用了 A<T>::U

template<class T>
struct A { typedef B<T> U; };  // A<T>::U 就是 B<T>

// B<short> b;  // 错误：B<short> 通过 A<short>::U 使用了自身类型
```

#### 错误 2：Lambda 类型假设

```cpp
// C++20 起：不同实例化产生不同的闭包类型
template<class T>
using LambdaType = decltype([]{});

// LambdaType<int> 和 LambdaType<char> 是不同的类型！
// static_assert(std::is_same_v<LambdaType<int>, LambdaType<char>>);  // C++20 起失败
```

#### 错误 3：期望模板参数推导

```cpp
#include <vector>

template<template<class> class Container>
void func(Container<int>) {}

template<class T>
using MyVec = std::vector<T>;

// MyVec<int> v;
// func(v);  // 错误：编译器无法推导 Container 为 MyVec
// 原因：别名模板不参与模板模板参数推导

// 正确方式：直接使用原始模板
std::vector<int> v;
func(v);  // OK，Container 推导为 std::vector
```

## 7. 总结

类型别名和别名模板是 C++ 现代编程的重要工具：

| 特性 | 类型别名 | 别名模板 |
|------|---------|---------|
| 引入版本 | C++11 | C++11 |
| 语法 | `using name = type;` | `template<...> using name = type;` |
| 等价于 | `typedef` | 无前置等价物 |
| 支持特化 | N/A | 否 |
| 支持约束 | N/A | C++20 |

核心要点：
1. **语义等价**：类型别名与 `typedef` 完全等价，只是语法更清晰
2. **不引入新类型**：类型别名只是现有类型的同义词
3. **支持模板化**：别名模板是 `typedef` 无法实现的功能
4. **禁止特化**：别名模板不能被部分或完全特化
5. **不参与推导**：别名模板不参与模板模板参数推导

使用建议：
- 新代码中优先使用 `using` 而非 `typedef`
- 复杂类型使用类型别名提高可读性
- 需要参数化类型别名时使用别名模板
- 需要特化行为时使用 traits 类而非别名模板

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/type_alias
- C++ Standard: [dcl.typedef]
- CWG Defect Report 1558