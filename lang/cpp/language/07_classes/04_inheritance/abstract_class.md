# 抽象类（Abstract Class）

## 1. 概述（Overview）

**抽象类（Abstract Class）** 是 C++ 中一种特殊的类类型，它定义了一个抽象类型，不能被直接实例化，但可以作为基类被继承。抽象类通过**纯虚函数（Pure Virtual Function）** 来定义接口规范，强制派生类实现特定功能。

抽象类的核心特征：
- 包含至少一个纯虚函数
- 不能直接创建对象实例
- 可以声明指针和引用
- 用于表示通用概念（如 Shape、Animal），作为具体类（如 Circle、Dog）的基类

## 2. 来源与演变（Origin and Evolution）

### 设计动机

抽象类是面向对象编程中**接口与实现分离**原则的重要体现。其设计动机包括：

1. **定义契约**：为派生类提供统一的接口规范
2. **实现多态**：通过基类指针/引用操作不同派生类对象
3. **代码复用**：提取公共行为到基类中
4. **延迟绑定**：将具体实现延迟到派生类中完成

### 版本变更

| DR（缺陷报告） | 应用版本 | 原发布行为 | 修正行为 |
|---------------|---------|-----------|---------|
| CWG 390 | C++98 | 未定义的纯虚析构函数可能被调用 | 此情况下必须提供定义 |
| CWG 2153 | C++98 | 纯说明符可出现在友元声明中 | 明确禁止 |

## 3. 语法与参数（Syntax and Parameters）

### 纯虚函数语法

纯虚函数是在声明时使用 `= 0` 语法标记的虚函数：

```
declarator virt-specifier(optional) = 0
```

**语法说明**：
- `declarator`：函数声明符（包括函数名和参数列表）
- `virt-specifier`（可选）：`override` 或 `final`
- `= 0`：纯说明符（pure-specifier），必须紧跟在声明符或虚说明符之后

### 语法限制

| 限制项 | 说明 |
|-------|------|
| 成员函数定义体 | 纯说明符不能出现在函数定义体中 |
| 友元声明 | 纯说明符不能出现在友元声明中 |
| 多声明 | 可以在单个声明语句中声明多个纯虚函数 |

### 正确语法示例

```cpp
struct Base
{
    virtual int g();
    virtual ~Base() {}
};

struct A : Base
{
    // OK：声明三个成员虚函数，其中两个是纯虚函数
    virtual int f() = 0, g() override = 0, h();

    // OK：析构函数也可以是纯虚函数
    ~A() = 0;

    // 错误：纯说明符不能出现在函数定义体中
    // virtual int b() = 0 {}  // 编译错误
};
```

## 4. 底层原理（Underlying Principles）

### 抽象类的判定条件

一个类被判定为抽象类，当且仅当：

1. **定义**了至少一个纯虚函数，或
2. **继承**了至少一个函数，且该函数的最终覆盖函数是纯虚函数

### 对象创建限制

| 操作 | 合法性 | 说明 |
|-----|-------|------|
| 创建抽象类对象 | 非法 | 编译时错误 |
| 声明抽象类指针 | 合法 | 常用于多态 |
| 声明抽象类引用 | 合法 | 常用于多态 |
| 作为参数类型 | 非法 | 参数类型不能是抽象类 |
| 作为返回类型 | 非法 | 返回类型不能是抽象类 |
| 作为显式转换类型 | 非法 | 不能转换为抽象类 |

### 纯虚函数的定义

虽然纯虚函数声明使用 `= 0`，但**可以**提供定义：

1. **定义位置**：必须在类体外部定义
2. **析构函数特殊要求**：纯虚析构函数**必须**提供定义
3. **调用方式**：派生类可使用限定函数标识符调用基类纯虚函数定义

### 虚函数调用规则

在构造函数和析构函数中调用虚函数的特殊规则：

```cpp
struct Abstract
{
    virtual void f() = 0; // 纯虚函数
    virtual void g() {}   // 非纯虚函数

    ~Abstract()
    {
        g();           // OK：调用 Abstract::g()
        // f();        // 未定义行为！虚调用纯虚函数
        Abstract::f(); // OK：非虚调用，直接调用 Abstract::f()
    }
};

// 纯虚函数的定义（在类体外部）
void Abstract::f()
{
    std::cout << "A::f()\n";
}
```

**关键原则**：
- 在构造函数/析构函数中进行**虚调用**纯虚函数是**未定义行为**
- 使用**限定调用**（如 `Abstract::f()`）是合法的

## 5. 使用场景（Use Cases）

### 适用场景

1. **定义接口规范**：为组件定义统一的行为契约
2. **实现多态性**：通过基类接口操作不同派生类对象
3. **策略模式**：定义算法族，由派生类实现具体策略
4. **工厂模式**：抽象产品接口，具体产品由派生类实现
5. **框架设计**：提供可扩展的基类框架

### 最佳实践

| 实践 | 说明 |
|-----|------|
| 纯虚析构函数 | 如果需要，必须提供定义 |
| 接口类 | 纯虚函数 + 虚析构函数 = 接口类 |
| 非虚接口（NVI）模式 | 公共非虚函数调用私有虚函数 |
| 默认实现 | 可为纯虚函数提供默认实现 |

### 常见陷阱

1. **忘记实现纯虚函数**：派生类若不实现所有纯虚函数，仍然是抽象类
2. **析构函数中虚调用**：导致未定义行为
3. **纯虚函数无定义**：如果析构函数是纯虚函数，必须提供定义
4. **继承链中的纯虚覆盖**：派生类可以重新将虚函数声明为纯虚

## 6. 代码示例（Examples）

### 示例 1：基础抽象类定义与继承

```cpp
#include <iostream>

// 抽象基类
struct Abstract
{
    virtual void f() = 0;  // 纯虚函数
}; // "Abstract" 是抽象类

// 具体派生类
struct Concrete : Abstract
{
    void f() override {}   // 非纯虚函数（实现了纯虚函数）
    virtual void g();      // 非纯虚函数
}; // "Concrete" 是非抽象类

// 再次抽象的派生类
struct Abstract2 : Concrete
{
    void g() override = 0; // 纯虚函数覆盖
}; // "Abstract2" 是抽象类

int main()
{
    // Abstract a;   // 错误：抽象类不能实例化
    Concrete b;      // OK
    Abstract& a = b; // OK：可以引用抽象基类
    a.f();           // 虚调用到 Concrete::f()
    // Abstract2 a2; // 错误：抽象类（g() 的最终覆盖是纯虚函数）

    return 0;
}
```

### 示例 2：纯虚函数定义与调用

```cpp
#include <iostream>

struct Abstract
{
    virtual void f() = 0; // 纯虚函数
    virtual void g() {}    // 非纯虚函数

    ~Abstract()
    {
        g();           // OK：调用 Abstract::g()
        // f();        // 未定义行为！
        Abstract::f(); // OK：非虚调用
    }
};

// 纯虚函数的定义
void Abstract::f()
{
    std::cout << "A::f()\n";
}

struct Concrete : Abstract
{
    void f() override
    {
        Abstract::f(); // OK：调用纯虚函数定义
    }

    void g() override {}

    ~Concrete()
    {
        g(); // OK：调用 Concrete::g()
        f(); // OK：调用 Concrete::f()
    }
};

int main()
{
    Concrete c;
    return 0;
}
```

输出：
```
A::f()
```

### 示例 3：常见错误与修正

**错误示例 1：抽象类实例化**

```cpp
struct Shape
{
    virtual double area() const = 0;
};

int main()
{
    Shape s;  // 错误：不能实例化抽象类
    return 0;
}
```

**修正方法：**

```cpp
struct Circle : Shape
{
    double radius;
    double area() const override { return 3.14159 * radius * radius; }
};

int main()
{
    Circle c;  // OK：具体类可以实例化
    Shape* s = &c;  // OK：抽象类指针
    return 0;
}
```

**错误示例 2：纯虚析构函数无定义**

```cpp
struct Base
{
    virtual ~Base() = 0;  // 纯虚析构函数
};

// 错误：缺少定义！链接时会出错

struct Derived : Base
{
    ~Derived() override {}  // 隐式调用 Base::~Base()
};

int main()
{
    Derived d;  // 链接错误：Base::~Base() 未定义
    return 0;
}
```

**修正方法：**

```cpp
struct Base
{
    virtual ~Base() = 0;  // 纯虚析构函数声明
};

// 必须提供定义！
Base::~Base() {}

struct Derived : Base
{
    ~Derived() override {}
};

int main()
{
    Derived d;  // OK
    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

| 要点 | 说明 |
|-----|------|
| 定义 | 包含至少一个纯虚函数的类 |
| 实例化 | 不能直接实例化，可声明指针/引用 |
| 继承 | 派生类必须实现所有纯虚函数才能实例化 |
| 定义体 | 纯虚函数可以在类外提供定义 |
| 析构函数 | 纯虚析构函数必须提供定义 |

### 技术对比

| 特性 | 纯虚函数 | 普通虚函数 |
|-----|---------|-----------|
| 声明语法 | `virtual void f() = 0;` | `virtual void f();` |
| 必须实现 | 派生类必须覆盖才能实例化 | 派生类可选择覆盖 |
| 默认实现 | 可选（类外定义） | 通常在类内定义 |
| 类是否抽象 | 是 | 否（除非继承其他纯虚函数） |

### 学习建议

1. **理解抽象类与接口的关系**：C++ 中抽象类可用于实现 Java/C# 中的接口概念
2. **掌握纯虚函数的定义规则**：特别是析构函数的特殊要求
3. **注意构造/析构中的虚调用**：避免未定义行为
4. **合理设计继承层次**：抽象类作为接口层，具体类实现细节

### 参考链接

- [virtual 关键字](https://en.cppreference.com/w/cpp/language/virtual)