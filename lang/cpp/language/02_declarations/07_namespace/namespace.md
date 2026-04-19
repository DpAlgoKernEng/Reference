# namespace - 命名空间

## 1. 概述

`namespace`（命名空间）是 C++ 提供的一种作用域划分机制，用于解决大型项目中的名称冲突（name collision）问题。

在命名空间块内声明的实体被放置在命名空间作用域（namespace scope）中，这防止了它们与其他作用域中同名的实体发生冲突。所有命名空间块之外声明的实体属于**全局命名空间**（global namespace），可以使用前导 `::` 显式引用。

命名空间的核心功能包括：
- **名称隔离**：将相关的标识符组织在一起，避免与其他代码中的同名实体冲突
- **代码组织**：提供逻辑分组机制，便于代码维护和理解
- **版本管理**：配合内联命名空间实现库的版本控制

## 2. 来源与演变

### 历史背景

在命名空间引入之前，C++ 开发者面临以下问题：

1. **全局名称冲突**：大型项目中，不同模块可能定义相同的类名、函数名
2. **第三方库冲突**：使用多个第三方库时，它们的符号可能相互冲突
3. **缺乏组织机制**：无法逻辑上分组相关的代码

### 首次引入

命名空间在 **C++98** 标准中首次引入，作为 C++ 核心语言特性之一。

### C++11 变化

- 新增 **内联命名空间**（inline namespace）：成员自动在 enclosing namespace 中可见
- 未命名命名空间中的名称明确具有**内部链接**（internal linkage）
- 内联命名空间支持版本控制和 ABI 兼容性

### C++17 变化

- 新增**嵌套命名空间定义**（nested namespace definition）：`namespace A::B::C { ... }` 简化语法
- 支持命名空间属性（attributes）

### C++20 变化

- 新增嵌套内联命名空间定义：`namespace A::B::inline C { ... }`
- 允许 using 声明引入作用域枚举器（scoped enumerator）

## 3. 语法与参数

### 基本语法

| 语法形式 | 说明 | 版本 |
|---------|------|------|
| `namespace ns-name { declarations }` | 命名命名空间定义 | C++98 |
| `inline namespace ns-name { declarations }` | 内联命名空间定义 | C++11 |
| `namespace { declarations }` | 未命名命名空间定义 | C++98 |
| `ns-name :: member-name` | 限定名称访问 | C++98 |
| `using namespace ns-name;` | using 指令 | C++98 |
| `using ns-name :: member-name;` | using 声明 | C++98 |
| `namespace name = qualified-namespace;` | 命名空间别名 | C++98 |
| `namespace ns-name :: member-name { declarations }` | 嵌套命名空间定义 | C++17 |
| `namespace ns-name :: inline member-name { declarations }` | 嵌套内联命名空间定义 | C++20 |

### 命名空间定义

```cpp
// 基本形式
inline(可选) namespace attr(可选) identifier { namespace-body }
```

| 参数 | 说明 |
|------|------|
| `inline` | 使该命名空间成为内联命名空间（C++11 起）|
| `attr` | 属性序列（C++17 起）|
| `identifier` | 命名空间名称，可以是新标识符或现有命名空间名 |
| `namespace-body` | 声明序列，可包含类、函数、变量、嵌套命名空间等 |

### 命名空间定义类型

| 类型 | 说明 |
|------|------|
| **original-namespace-definition** | 使用新标识符首次定义命名空间 |
| **extension-namespace-definition** | 扩展现有命名空间（重新打开）|
| **nested-namespace-definition** | 嵌套定义（C++17 起）|

### using 声明语法

```cpp
// C++17 之前
using typename(可选) nested-name-specifier unqualified-id;

// C++17 起
using declarator-list;
```

### using 指令语法

```cpp
attr(可选) using namespace nested-name-specifier(可选) namespace-name;
```

## 4. 底层原理

### 名称查找机制

命名空间影响两种名称查找：

1. **非限定名称查找**（unqualified name lookup）：从不带 `::` 的名称开始，从当前作用域向外查找
2. **限定名称查找**（qualified name lookup）：使用 `namespace::name` 形式，在指定命名空间中查找

### 内联命名空间原理

内联命名空间的核心机制：

- **隐式 using 指令**：自动在 enclosing namespace 中插入 using 指令
- **ADL 传递性**：参数依赖查找（Argument-Dependent Lookup）时，内联命名空间和其 enclosing namespace 被同时考虑
- **特化透明性**：可以在 enclosing namespace 中对内联命名空间中的模板进行特化

```
内联命名空间结构示意：

namespace Lib {
    inline namespace Lib_1 {
        template<typename T> class A;
    }
}

// 用户可以在 Lib 中特化 Lib_1::A
template<> class Lib::A<MyClass> { ... };  // OK
```

### 未命名命名空间原理

未命名命名空间被编译器转换为：

1. 一个带有**唯一名称**的命名空间（整个程序唯一，但同一翻译单元内的多个定义共享相同名称）
2. 一个隐式 **using 指令**在当前作用域

```cpp
namespace {
    int i;
}

// 等价于：
namespace __unique_name__ {
    int i;
}
using namespace __unique_name__;
```

**链接特性**：
- C++11 起：未命名命名空间及其内部所有名称都具有**内部链接**（internal linkage）
- 这意味着：其他翻译单元无法访问这些名称

### using 指令 vs using 声明

| 特性 | using 指令 | using 声明 |
|------|-----------|-----------|
| 引入名称 | 整个命名空间的名称 | 单个名称 |
| 冲突处理 | 允许冲突，使用时歧义 | 声明时冲突则报错 |
| 后续扩展 | 可见新添加的成员 | 不可见新添加的成员 |
| ADL 影响 | 无 | 无 |
| 可见性 | 在 enclosing namespace 可见 | 在当前作用域可见 |

## 5. 使用场景

### 适合使用命名空间的场景

| 场景 | 说明 |
|------|------|
| 库开发 | 避免库符号与用户代码冲突 |
| 大型项目 | 组织代码模块，防止名称冲突 |
| 版本控制 | 使用内联命名空间管理库版本 |
| 封装实现 | 使用未命名命名空间隐藏实现细节 |

### 内联命名空间的使用场景

```cpp
// 版本控制示例
namespace MyLib {
    namespace v1 {
        void func() { /* 旧版本 */ }
    }
    inline namespace v2 {  // 当前默认版本
        void func() { /* 新版本 */ }
    }
}

// 用户代码
MyLib::func();     // 调用 v2::func()
MyLib::v1::func(); // 显式调用旧版本
```

### 未命名命名空间的使用场景

```cpp
// 替代 static 关键字，限制作用域
namespace {
    int internal_counter = 0;  // 仅在本翻译单元可见
    void helper_function() {}   // 内部辅助函数
}
```

### 最佳实践

1. **头文件中避免 using 指令**：污染全局命名空间，可能导致用户代码冲突
2. **使用命名空间别名**：简化长命名空间名称
3. **优先使用 using 声明**：比 using 指令更精确，避免意外引入名称
4. **库代码使用命名空间前缀**：避免宏展开等问题

### 常见陷阱

1. **using 指令导致的歧义**：
```cpp
namespace A { void f(int); }
namespace B { void f(int); }
using namespace A;
using namespace B;
f(1);  // 歧义：A::f 还是 B::f？
```

2. **ADL 意外行为**：
```cpp
namespace A {
    struct X {};
    void f(X) {}
}
using namespace A;
A::X x;
f(x);  // 调用 A::f，ADL 生效
```

3. **未命名命名空间中的外部链接实体**：
```cpp
namespace {
    extern int x;  // C++11 前是外部链接但不可访问
                   // C++11 起是内部链接
}
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>
#include <vector>

// 命名空间定义
namespace MyMath {
    const double PI = 3.14159265358979;

    double square(double x) {
        return x * x;
    }

    double circle_area(double radius) {
        return PI * square(radius);
    }
}

int main() {
    // 方式1：限定名称访问
    std::cout << "Area: " << MyMath::circle_area(2.0) << std::endl;

    // 方式2：using 声明
    using MyMath::PI;
    std::cout << "PI = " << PI << std::endl;

    // 方式3：using 指令
    {
        using namespace MyMath;
        std::cout << "Square: " << square(3.0) << std::endl;
    }

    return 0;
}
```

### 命名空间扩展

```cpp
namespace MyLib {
    void func1();  // 首次声明
}

// 扩展命名空间
namespace MyLib {
    void func2();  // 添加新成员
}

// 命名空间外定义成员
void MyLib::func1() {
    // 实现
}
```

### 嵌套命名空间（C++17）

```cpp
// 传统写法
namespace A {
    namespace B {
        namespace C {
            void func();
        }
    }
}

// C++17 简化写法
namespace A::B::C {
    void func();
}

// C++20 嵌套内联命名空间
namespace A::B::inline C {
    void func();
}
// 等价于：
namespace A::B {
    inline namespace C {
        void func();
    }
}
```

### 内联命名空间版本控制

```cpp
#include <string>
#include <iostream>

namespace Library {
    inline namespace v2_0 {  // 当前版本
        std::string getVersion() { return "2.0"; }

        class Widget {
        public:
            void process() { std::cout << "v2 process\n"; }
        };
    }

    namespace v1_0 {  // 旧版本
        std::string getVersion() { return "1.0"; }

        class Widget {
        public:
            void process() { std::cout << "v1 process\n"; }
        };
    }
}

int main() {
    // 默认使用内联命名空间
    Library::Widget w1;
    w1.process();  // v2 process

    // 显式使用旧版本
    Library::v1_0::Widget w2;
    w2.process();  // v1 process

    return 0;
}
```

### 未命名命名空间

```cpp
namespace {
    // 这些名称仅在当前翻译单元可见
    int internal_state = 0;

    void internal_helper() {
        internal_state++;
    }
}

void public_api() {
    internal_helper();
    // internal_state 可以直接使用
}
```

### 命名空间别名

```cpp
namespace very_long_library_name_version_1_0 {
    void func();
}

// 创建别名
namespace vlib = very_long_library_name_version_1_0;

int main() {
    vlib::func();  // 更简洁
    return 0;
}
```

### 常见错误及修正

#### 错误 1：using 声明冲突

```cpp
namespace A { void f(int); }
namespace B { void f(int); }

void foo() {
    using A::f;
    using B::f;  // OK：声明时不冲突
    f(1);         // 错误：调用时歧义
}

// 修正：显式限定
void foo_fixed() {
    using A::f;
    using B::f;
    A::f(1);  // 明确调用
}
```

#### 错误 2：头文件中的 using 指令

```cpp
// 错误：在头文件中使用 using 指令
// mylib.h
namespace MyLib {
    void func();
}

using namespace MyLib;  // 不好！污染全局命名空间

// 修正：移除 using 指令，让用户自行决定
// mylib_fixed.h
namespace MyLib {
    void func();
}
// 用户代码中按需使用 using
```

#### 错误 3：命名空间外非法定义

```cpp
namespace A {
    namespace B {
        void func();
    }
}

void func() {}  // 错误：不是 A::B::func 的定义

namespace C {
    void A::B::func() {}  // 错误：C 不包含 A
}

// 修正：在正确的命名空间外定义
void A::B::func() {}  // OK：在全局命名空间定义 A::B::func
```

#### 错误 4：using 声明后的命名空间扩展

```cpp
namespace A {
    void f(int);
}
using A::f;  // 只引入 f(int)

namespace A {
    void f(char);  // 新增重载
}

void foo() {
    f('a');  // 调用 f(int)，不是 f(char)！
    // using 声明不感知后续扩展
}

// 修正：需要再次使用 using 声明或使用 using 指令
void bar() {
    using A::f;  // 此时引入 f(int) 和 f(char)
    f('a');      // 正确调用 f(char)
}
```

## 注意事项

1. **链接性**：未命名命名空间中的名称具有内部链接（C++11 起）
2. **ADL 行为**：using 指令不影响参数依赖查找
3. **友元声明**：类中的友元声明在最近的外围命名空间中声明名称，但不可见
4. **头文件最佳实践**：避免在头文件的全局作用域使用 using 指令
5. **模板特化**：内联命名空间允许在 enclosing namespace 中特化其内部模板

## 相关概念

| 概念 | 关系 |
|------|------|
| 全局作用域 | 全局命名空间所在的作用域 |
| 类作用域 | 类成员的作用域，类似命名空间作用域 |
| 未命名命名空间 | 替代 static 关键字实现内部链接 |
| 命名空间别名 | 为现有命名空间创建简短别名 |
| ADL | 参数依赖查找，会考虑命名空间 |

## 7. 总结

命名空间是 C++ 中组织代码和管理名称可见性的核心机制。

**核心特性**：
- **名称隔离**：防止大型项目中的名称冲突
- **代码组织**：逻辑分组相关功能
- **版本控制**：内联命名空间支持库版本管理

**关键对比**：

| 特性 | using 指令 | using 声明 |
|------|-----------|-----------|
| 精确性 | 引入整个命名空间 | 引入单个名称 |
| 安全性 | 较低，可能引入意外名称 | 较高，明确指定名称 |
| 推荐场景 | 源文件内部、小作用域 | 精确控制名称引入 |

**使用建议**：
1. 库代码始终使用命名空间
2. 头文件中避免 using 指令
3. 优先使用 using 声明而非 using 指令
4. 使用命名空间别名简化长名称
5. 内部实现使用未命名命名空间隐藏

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/namespace
- C++ Standard: [namespace.qual], [namespace.udir]
- C++ Core Guidelines: SF.7 - Don't write using namespace at global scope in a header file