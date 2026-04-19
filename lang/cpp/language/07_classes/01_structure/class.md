# 类声明 (Class Declaration)

## 1. 概述

类（Class）是 C++ 中用户定义类型的核心机制，通过 class-specifier（类说明符）在声明语法的 decl-specifier-seq 中定义。类允许开发者创建封装了数据和操作这些数据的函数的自定义类型，是面向对象编程的基础。

类声明支持三种关键字：`class`、`struct` 和 `union`。其中 `class` 和 `struct` 几乎完全相同，唯一的区别在于默认成员访问权限和默认基类访问权限。`union` 则引入一种特殊的联合体类型。

## 2. 来源与演变

### 历史背景

类概念最早源于 Simula 67 语言（1967年），Bjarne Stroustrup 在设计 C++ 时借鉴了 Simula 的类概念，将其引入 C 语言的前身 "C with Classes"（1979年）。类的设计目标是：

- 提供数据封装机制
- 支持面向对象编程
- 保持与 C 语言的高效性和兼容性
- 实现零开销抽象原则

### C++98 标准

类的基本语法和语义在 C++98 中确立，包括：
- 访问控制（public、protected、private）
- 继承机制
- 虚函数和多态
- 构造函数和析构函数

### C++11 变化

- 新增 `final` 关键字，禁止类被继承
- 新增属性（attributes）支持，可包含 `alignas` 说明符
- 局部类可以用作模板参数
- 继承构造函数（using-declaration for constructors）

### C++17 变化

- 成员类模板的推导指引（deduction guides）可以在类定义中声明

### C++20 变化

- 新增 using-enum-declaration 语法
- 在模块中定义的类成员函数不再自动成为 inline

## 3. 语法与参数

### 类说明符语法

**命名类定义**：

```cpp
class-key attr(可选) class-head-name final(可选) base-clause(可选) { member-specification }
```

**无名类定义**：

```cpp
class-key attr(可选) base-clause(可选) { member-specification }
```

### 参数说明

| 参数 | 说明 |
|------|------|
| class-key | 类关键字，取值为 `class`、`struct` 或 `union`。`class` 和 `struct` 仅在默认成员访问权限和默认基类访问权限上有区别 |
| attr | (C++11 起) 任意数量的属性，可包含 `alignas` 说明符 |
| class-head-name | 被定义的类名称，可带限定符 |
| final | (C++11 起) 如果存在，该类不能被派生 |
| base-clause | 基类列表及每个基类的继承方式 |
| member-specification | 访问说明符、成员对象和成员函数声明及定义的列表 |

### 前向声明语法

```cpp
class-key attr identifier ;
```

前向声明声明一个将在当前作用域后续定义的类类型。在定义出现之前，该类名具有不完整类型（incomplete type）。

### 成员说明语法

成员说明是类定义体中大括号包围的内容，可包含：

1. **成员声明**：
```cpp
attr(可选) decl-specifier-seq(可选) member-declarator-list(可选) ;
```

2. **函数定义**：同时声明和定义成员函数或友元函数

3. **访问说明符**：`public:`、`protected:`、`private:`

4. **using 声明**：引入基类成员或继承构造函数

5. **static_assert 声明**

6. **成员模板声明**

7. **别名声明**（C++11 起）

8. **推导指引**（C++17 起）

9. **using-enum-declaration**（C++20 起）

## 4. 底层原理

### 类类型的完整性

类在以下时机变为完整类型（complete type）：
- 类定义结束时（右大括号之后）
- 对于嵌套类，在外围类定义结束时

在不完整类型阶段，只能：
- 声明指向该类的指针或引用
- 声明以该类为参数或返回类型的函数
- 声明该类的静态数据成员

### 名称查找与作用域

类定义中的名称查找遵循以下规则：
- 类成员名称在类作用域内可见
- 前向声明在局部作用域中会隐藏外围作用域中同名的类、变量、函数等声明
- 详细类型说明符（elaborated type specifier）可引入新的类名

### 局部类的特性

定义在函数体内的类称为局部类（local class），其特点包括：

| 特性 | 说明 |
|------|------|
| 名称作用域 | 仅在函数作用域内有效 |
| 成员声明位置 | 只能在类定义中声明，嵌套类除外 |
| 静态数据成员 | 不能有静态数据成员 |
| 成员函数链接 | 无链接 |
| 成员函数定义 | 必须完全在类体内定义 |
| 成员模板 | C++14 前不允许，闭包类型例外 |
| 友元模板 | 不能有友元模板 |
| 友元函数定义 | 不能在类定义内定义友元函数 |

### 内存布局

类实例的内存布局取决于：
- 非静态数据成员的声明顺序
- 访问说明符（可能影响布局，实现相关）
- 继承关系和虚函数表指针
- 对齐要求

## 5. 使用场景

### 适合使用类的场景

| 场景 | 原因 |
|------|------|
| 数据封装 | 通过访问控制保护内部状态 |
| 实现抽象数据类型 | 提供清晰的接口，隐藏实现细节 |
| 资源管理（RAII） | 利用构造/析构管理资源生命周期 |
| 多态设计 | 通过虚函数实现运行时多态 |
| 代码组织 | 将相关数据和操作组织在一起 |

### 前向声明的使用场景

1. **打破循环依赖**：当两个类相互引用时
2. **减少编译依赖**：当只需要指针或引用类型时，可减少头文件包含
3. **加速编译**：减少头文件包含可加快编译速度

### 最佳实践

1. **优先使用 `class` 还是 `struct`**：
   - 如果所有成员都是公开的（POD 类型），使用 `struct`
   - 如果需要封装，使用 `class`

2. **合理使用前向声明**：
   - 在头文件中尽量使用前向声明代替包含
   - 将完整的类定义放在源文件中

3. **访问说明符的组织**：
   - 将公开接口放在 `public` 区域
   - 将实现细节放在 `private` 区域
   - 将派生类可访问的成员放在 `protected` 区域

### 常见陷阱

1. **前向声明与完整定义混淆**：
   - 前向声明后，在解引用指针前必须包含完整定义
   - sizeof、成员访问等操作需要完整类型

2. **局部类的限制**：
   - 不能有静态数据成员
   - 成员函数必须内联定义

3. **访问说明符影响布局**：
   - 不同访问区域的成员可能有不同的内存布局（实现相关）

## 6. 代码示例

### 基础用法：命名类定义

```cpp
#include <string>
#include <iostream>

// 命名类定义
class Person {
public:
    // 构造函数
    Person(const std::string& name, int age)
        : name_(name), age_(age) {}

    // 成员函数
    void introduce() const {
        std::cout << "Name: " << name_
                  << ", Age: " << age_ << std::endl;
    }

    // 访问器
    int getAge() const { return age_; }
    void setAge(int age) { age_ = age; }

private:
    std::string name_;
    int age_;
};

int main() {
    Person p("Alice", 25);
    p.introduce();  // Name: Alice, Age: 25
    return 0;
}
```

### 高级用法：前向声明与循环依赖

```cpp
#include <iostream>

// 前向声明
class Vector;

// Matrix 类定义
class Matrix {
public:
    Matrix(int rows, int cols) : rows_(rows), cols_(cols) {}

    // 声明友元函数，参数使用前向声明的 Vector
    friend Vector multiply(const Matrix& m, const Vector& v);

    void print() const {
        std::cout << "Matrix " << rows_ << "x" << cols_ << std::endl;
    }

private:
    int rows_;
    int cols_;
};

// Vector 类定义
class Vector {
public:
    explicit Vector(int size) : size_(size) {}

    friend Vector multiply(const Matrix& m, const Vector& v);

    void print() const {
        std::cout << "Vector size: " << size_ << std::endl;
    }

private:
    int size_;
};

// 友元函数定义
Vector multiply(const Matrix& m, const Vector& v) {
    return Vector(m.rows_ * v.size_);
}

int main() {
    Matrix mat(3, 4);
    Vector vec(5);

    mat.print();       // Matrix 3x4
    vec.print();       // Vector size: 5

    Vector result = multiply(mat, vec);
    result.print();    // Vector size: 15

    return 0;
}
```

### 高级用法：final 关键字防止继承

```cpp
#include <iostream>

// 使用 final 防止类被继承
class Base final {
public:
    virtual void foo() {
        std::cout << "Base::foo()" << std::endl;
    }
};

// 错误：不能继承 final 类
// class Derived : public Base { };  // 编译错误

int main() {
    Base b;
    b.foo();
    return 0;
}
```

### 高级用法：成员说明完整示例

```cpp
#include <string>
#include <vector>
#include <type_traits>
#include <cassert>

class Example {
public:
    // 构造函数定义
    Example(std::size_t r, std::size_t c)
        : cols_(c), data_(r * c) {}

    // 成员函数定义
    int operator()(std::size_t r, std::size_t c) const {
        return data_[r * cols_ + c];
    }

    int& operator()(std::size_t r, std::size_t c) {
        return data_[r * cols_ + c];
    }

protected:
    // 受保护成员
    int protected_value_{0};

private:
    // 私有成员
    std::size_t cols_;
    std::vector<int> data_;

    // 嵌套类
    struct Nested {
        std::string name;
    };

public:
    // using 声明（示例用途）
    using value_type = int;
    using size_type = std::size_t;
};

// 成员模板示例
template<typename T>
struct TemplateClass {
    // 成员模板
    template<typename U>
    void process(U&& value);

    // 嵌套模板类
    template<typename CharT>
    struct NestedTemplate {
        std::basic_string<CharT> str;
    };

    // static_assert 声明
    static_assert(std::is_floating_point<T>::value,
                  "T must be floating point");
};

// 继承构造函数示例 (C++11)
class Base {
public:
    Base(int x) : x_(x) {}
    Base(int x, int y) : x_(x), y_(y) {}
protected:
    int x_{0};
    int y_{0};
};

class Derived : public Base {
public:
    using Base::Base;  // 继承所有基类构造函数
};
```

### 常见错误及修正

#### 错误 1：前向声明后使用不完整类型

```cpp
// 错误：前向声明后直接使用完整类型操作
class Forward;  // 前向声明

void process(Forward f) {  // 错误：参数类型不完整
    // ...
}

// 修正：使用指针或引用
class Forward;  // 前向声明

void process(Forward* f) {  // 正确：指针可以指向不完整类型
    // 但在解引用前需要包含完整定义
}

void process_ref(Forward& f) {  // 正确：引用也可以
    // 但在使用前需要包含完整定义
}
```

#### 错误 2：局部类的静态成员

```cpp
void func() {
    struct LocalClass {
        static int count;  // 错误：局部类不能有静态数据成员
    };
}

// 修正：使用静态局部变量
void func_correct() {
    static int count = 0;  // 静态局部变量
    struct LocalClass {
        void increment() { ++count; }
    };
}
```

#### 错误 3：局部类成员函数外部定义

```cpp
void func() {
    struct LocalClass {
        void method();
    };

    void LocalClass::method() {}  // 错误：局部类成员函数必须内联定义
}

// 修正：在类体内定义
void func_correct() {
    struct LocalClass {
        void method() {  // 正确：内联定义
            // 函数体
        }
    };
}
```

### 局部类完整示例

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};

    // 定义局部类作为比较器
    struct LocalComparator {
        bool operator()(int n, int m) const {
            return n > m;  // 降序排序
        }
    };

    // C++11 起，局部类可以用作模板参数
    std::sort(v.begin(), v.end(), LocalComparator());

    for (int n : v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
    // 输出：3 2 1

    return 0;
}
```

## 7. 总结

### 核心要点

1. **类定义语法**：使用 `class`、`struct` 或 `union` 关键字定义用户类型
2. **前向声明**：允许声明尚未定义的类，用于打破循环依赖和减少编译依赖
3. **成员说明**：支持数据成员、成员函数、嵌套类、typedef、枚举等多种声明
4. **访问控制**：通过 `public`、`protected`、`private` 实现封装
5. **final 关键字**（C++11）：防止类被继承

### class 与 struct 对比

| 特性 | class | struct |
|------|-------|--------|
| 默认成员访问权限 | private | public |
| 默认继承方式 | private | public |
| 用途建议 | 需要封装的类 | POD 类型或公开数据结构 |

### 局部类限制总结

- 不能有静态数据成员
- 成员函数必须内联定义
- 不能有成员模板（C++14 前限制）
- 不能有友元模板
- 名称仅在函数作用域内有效

### 学习建议

1. 熟练掌握前向声明的使用场景和限制
2. 理解访问控制的设计意图
3. 掌握成员说明的各种语法形式
4. 了解局部类的特殊限制

## 参考资料

- cppreference: https://en.cppreference.com/w/cpp/language/class
- C++ Standard: [class], [class.mem], [class.local]
- The C++ Programming Language, Bjarne Stroustrup