# Friend Declaration - 友元声明

## 1. 概述

友元声明（Friend Declaration）是 C++ 中一种特殊的访问控制机制，它出现在类体中，用于授予函数或另一个类访问该类私有（private）和保护（protected）成员的权限。

### 核心概念

- **友元函数（Friend Function）**：被声明为友元的非成员函数可以访问类的私有和保护成员
- **友元类（Friend Class）**：被声明为友元的类的所有成员函数都可以访问当前类的私有和保护成员
- **单向关系**：友元关系是单向的，A 是 B 的友元，并不意味着 B 是 A 的友元

### 技术定位

友元机制打破了类的封装边界，为特定场景提供了灵活的访问控制方案，常用于：
- 运算符重载（特别是输入输出流运算符）
- 紧密耦合的类之间的协作
- 工厂模式或构建器模式中需要访问内部状态的场景

## 2. 来源与演变

### 历史背景

友元机制在 **C++98** 标准中首次引入，是 C++ 面向对象设计的重要组成部分。其设计动机源于以下需求：

1. **运算符重载的便利性**：某些运算符（如 `operator<<` 和 `operator>>`）需要作为非成员函数实现，但又需要访问类的私有成员
2. **类间协作**：多个类之间需要紧密合作，共享内部实现细节
3. **避免破坏封装的妥协方案**：相比于将成员设为 public，友元提供了一种可控的"例外访问"机制

### 版本演变

| 版本 | 变化 |
|------|------|
| **C++98** | 首次引入友元声明，支持友元函数和友元类 |
| **C++11** | 支持简单类型说明符（simple-type-specifier）声明友元类，如 `friend Y;` |
| **C++20** | 模块（Module）中的友元声明具有与封闭类相同的链接属性 |
| **C++26** | 支持变参友元声明（Variadic Friend Declarations），可在单个声明中列出多个友元类型 |

### 缺陷报告

| DR | 问题 | 修正 |
|----|------|------|
| CWG 45 | 友元类的嵌套类对授予友元的类没有特殊访问权限 | 嵌套类与外围类具有相同的访问权限 |
| CWG 500 | 友元类不能继承授予友元的类的私有/保护成员，但其嵌套类可以 | 两者都可以继承 |
| CWG 1439 | 非局部类中的友元声明规则未覆盖模板声明 | 已覆盖 |
| CWG 1477 | 在类或类模板中首次声明的友元名称在其他命名空间作用域提供匹配声明时不可见 | 此情况下可见 |
| CWG 1804 | 当类模板成员被授予友元时，部分特化的相应成员不是友元 | 这些成员也是友元 |
| CWG 2379 | C++11 中引用函数模板全特化的友元声明可以声明为 constexpr | 已禁止 |
| CWG 2588 | 友元声明引入的名称的链接属性不明确 | 已明确 |

## 3. 语法与参数

### 基本语法

```
| 语法 | 说明 | 版本 |
|------|------|------|
| `friend` function-declaration | (1) 函数友元声明 | |
| `friend` function-definition | (2) 函数友元定义 | |
| `friend` elaborated-type-specifier `;` | (3) 详细类型说明符 | C++26 前 |
| `friend` simple-type-specifier `;` | (4) 简单类型说明符 | C++11 起 |
| `friend` typename-specifier `;` | (4) typename 说明符 | C++11 起，C++26 前 |
| `friend` friend-type-specifier-list `;` | (5) 变参友元类型列表 | C++26 起 |
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `function-declaration` | 函数声明，指定一个或多个函数为友元 |
| `function-definition` | 函数定义，同时定义并声明为友元 |
| `elaborated-type-specifier` | 详细类型说明符，如 `class X` 或 `struct Y` |
| `simple-type-specifier` | 简单类型说明符，直接使用类型名 |
| `typename-specifier` | 关键字 `typename` 后跟限定标识符或限定简单模板标识符 |
| `friend-type-specifier-list` | 以逗号分隔的类型说明符列表，每个说明符后可跟省略号（`...`） |

### 友元函数声明语法

```cpp
class Y {
    int data;  // 私有成员

    // 非成员函数 operator<< 将有访问 Y 私有成员的权限
    friend std::ostream& operator<<(std::ostream& out, const Y& o);

    // 其他类的成员函数也可以是友元
    friend char* X::foo(int);

    // 构造函数和析构函数也可以是友元
    friend X::X(char), X::~X();
};
```

### 友元类声明语法

```cpp
class A {
    int data;        // 私有数据成员
    class B {};      // 私有嵌套类型
    enum { a = 100 }; // 私有枚举器

    // 友元类前向声明（详细类型说明符）
    friend class X;

    // 友元类声明（简单类型说明符）- C++11 起
    friend Y;

    // C++26 起：合并声明
    // friend class X, Y;
};
```

### 模板友元语法

```cpp
class A {
    // 每个 B<T> 都是 A 的友元
    template<typename T>
    friend class B;

    // 每个 f<T> 都是 A 的友元
    template<typename T>
    friend void f(T) {}
};
```

## 4. 底层原理

### 访问控制机制

C++ 的访问控制（public、protected、private）是**编译时**检查机制，而非运行时机制。友元声明的作用是：

1. **编译器指令**：告诉编译器在访问检查时，将指定的函数或类视为"可信"的
2. **单向授权**：仅授予友元访问当前类的权限，不建立反向关系
3. **不改变成员可见性**：成员仍然是私有的，只是对特定实体开放访问

### 链接属性

| 情况 | 链接属性 |
|------|---------|
| 函数/函数模板在友元声明中首次声明并定义，且封闭类在导出声明中定义（C++20） | 与封闭类相同 |
| 函数/函数模板在友元声明中声明，且存在对应的非友元声明可达 | 由先前声明决定 |
| 其他情况 | 按常规规则决定 |

### 名称查找规则

友元声明引入的名称：

1. **成为最内层封闭命名空间的成员**
2. **仅对参数依赖查找（ADL）可见**，除非在命名空间作用域提供匹配声明

```cpp
namespace N {
    class A {
        friend void f();  // f 成为 N 的成员
    };
    // 此时 f 仅在 ADL 中可见（通过 A 的参数）
}

// 需要在命名空间作用域提供声明才能普通查找
namespace N {
    void f();  // 声明后可普通查找
}
```

### 友元关系特性

| 特性 | 说明 |
|------|------|
| **非传递性** | A 是 B 的友元，B 是 C 的友元，并不意味着 A 是 C 的友元 |
| **非继承性** | 友元关系不会被继承，派生类不继承基类的友元 |
| **访问说明符无关** | 友元声明可以出现在 private、protected 或 public 区域，效果相同 |

## 5. 使用场景

### 适用场景

| 场景 | 说明 |
|------|------|
| **运算符重载** | `operator<<` 和 `operator>>` 通常需要访问类的私有成员 |
| **类间协作** | 两个类紧密协作，需要访问彼此的内部状态 |
| **工厂/构建器模式** | 工厂类需要访问产品的私有构造函数或成员 |
| **单元测试** | 测试类需要访问被测类的私有成员进行验证 |

### 最佳实践

1. **最小化友元使用**：友元打破了封装，应谨慎使用
2. **优先使用公共接口**：如果可以通过公共成员函数实现，不要使用友元
3. **集中声明**：将所有友元声明放在类的开头或结尾，便于管理
4. **明确注释**：解释为什么需要友元关系

### 注意事项

#### 友元声明不定义新类

```cpp
class X {
    friend class Y {};  // 错误！不能在友元声明中定义类
};
```

#### 局部类的友元查找

当局部类声明非限定函数或类为友元时，仅查找最内层非类作用域中的函数和类：

```cpp
class F {};
int f();

int main() {
    extern int g();

    class Local {
        friend int f();    // 错误：main() 中没有声明 f
        friend int g();    // OK：main() 中有 g 的声明
        friend class F;    // 友元局部 F（稍后定义）
        friend class ::F;  // 友元全局 F
    };

    class F {};  // 局部 F
}
```

### 常见陷阱

1. **误认为友元是双向的**
2. **误认为友元关系可以继承**
3. **在模板中使用友元时的语法错误**
4. **局部类中友元查找范围混淆**

## 6. 代码示例

### 基础用法：友元函数

```cpp
#include <iostream>

class Box {
private:
    double width;

public:
    Box(double w) : width(w) {}

    // 声明友元函数
    friend void printWidth(Box box);
    friend std::ostream& operator<<(std::ostream& os, const Box& box);
};

// 友元函数定义（非成员函数）
void printWidth(Box box) {
    // 可以访问 Box 的私有成员
    std::cout << "Width: " << box.width << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Box& box) {
    return os << "Box(width=" << box.width << ")";
}

int main() {
    Box box(10.0);
    printWidth(box);  // 输出: Width: 10
    std::cout << box << std::endl;  // 输出: Box(width=10)
    return 0;
}
```

### 基础用法：友元类

```cpp
#include <iostream>

class Engine;  // 前向声明

class Car {
private:
    int speed;
    Engine* engine;

public:
    Car(int s) : speed(s), engine(nullptr) {}

    // Engine 是 Car 的友元
    friend class Engine;
};

class Engine {
public:
    void setSpeed(Car& car, int newSpeed) {
        // 可以访问 Car 的私有成员
        car.speed = newSpeed;
        std::cout << "Speed set to: " << car.speed << std::endl;
    }

    int getSpeed(const Car& car) {
        return car.speed;  // 访问私有成员
    }
};

int main() {
    Car car(60);
    Engine engine;
    engine.setSpeed(car, 100);  // 输出: Speed set to: 100
    std::cout << "Current speed: " << engine.getSpeed(car) << std::endl;
    return 0;
}
```

### 高级用法：类内定义友元函数

```cpp
#include <iostream>

class X {
    int a;

public:
    X(int val) : a(val) {}

    // 在类内定义友元函数（始终是 inline 的）
    friend void friend_set(X& p, int i) {
        p.a = i;  // 访问私有成员
    }

    void member_set(int i) {
        a = i;  // 成员函数
    }

    int get() const { return a; }
};

int main() {
    X x(10);
    std::cout << "Initial: " << x.get() << std::endl;  // 输出: 10

    friend_set(x, 20);  // 调用友元函数
    std::cout << "After friend_set: " << x.get() << std::endl;  // 输出: 20

    x.member_set(30);  // 调用成员函数
    std::cout << "After member_set: " << x.get() << std::endl;  // 输出: 30

    return 0;
}
```

### 高级用法：模板友元

```cpp
#include <iostream>

// 模板友元类
template<typename T>
class Builder;

class Product {
private:
    int id;
    std::string name;

    // 私有构造函数
    Product(int i, const std::string& n) : id(i), name(n) {}

public:
    void display() const {
        std::cout << "Product[" << id << "]: " << name << std::endl;
    }

    // Builder 是 Product 的模板友元
    template<typename T>
    friend class Builder;
};

template<typename T>
class Builder {
public:
    Product create(int id, const std::string& name) {
        // 可以访问 Product 的私有构造函数
        return Product(id, name);
    }
};

int main() {
    Builder<int> builder;
    Product p = builder.create(1, "Widget");
    p.display();  // 输出: Product[1]: Widget
    return 0;
}
```

### 高级用法：模板友元运算符

```cpp
#include <iostream>

template<typename T>
class Foo {
public:
    Foo(const T& val) : data(val) {}

private:
    T data;

    // 为每个 T 生成一个非模板 operator<<，并使其成为 Foo<T> 的友元
    friend std::ostream& operator<<(std::ostream& os, const Foo& obj) {
        return os << obj.data;
    }
};

int main() {
    Foo<double> obj(1.23);
    std::cout << obj << std::endl;  // 输出: 1.23
    return 0;
}
```

### 常见错误及修正

#### 错误 1：友元关系非传递

```cpp
class A {
    friend class B;
private:
    int secret;
};

class B {
    friend class C;
public:
    void accessA(A& a) {
        a.secret;  // OK: B 是 A 的友元
    }
};

class C {
public:
    void accessA(A& a) {
        // a.secret;  // 错误！C 不是 A 的友元
        // 友元关系不传递
    }
};
```

#### 错误 2：友元关系非继承

```cpp
class Base {
    friend class Friend;
private:
    int baseSecret;
};

class Derived : public Base {
private:
    int derivedSecret;
};

class Friend {
public:
    void access(Base& b) {
        b.baseSecret;  // OK
    }

    void access(Derived& d) {
        d.baseSecret;     // OK: Friend 是 Base 的友元
        // d.derivedSecret;  // 错误！Friend 不是 Derived 的友元
    }
};
```

#### 错误 3：模板友元部分特化

```cpp
template<class T>
class A {};      // 主模板

template<class T>
class A<T*> {};  // 部分特化

template<>
class A<int> {}; // 全特化

class X {
    template<class T>
    friend class A<T*>;  // 错误！不能引用部分特化

    friend class A<int>; // OK：可以引用全特化
};
```

### 完整示例：流插入/提取运算符

```cpp
#include <iostream>
#include <sstream>

class MyClass {
    int i;                   // 友元可以访问非公开、非静态成员
    static inline int id{6}; // 以及静态（可能是 inline）成员

    friend std::ostream& operator<<(std::ostream& out, const MyClass&);
    friend std::istream& operator>>(std::istream& in, MyClass&);
    friend void change_id(int);

public:
    MyClass(int i = 0) : i(i) {}
};

std::ostream& operator<<(std::ostream& out, const MyClass& mc) {
    return out << "MyClass::id = " << MyClass::id << "; i = " << mc.i;
}

std::istream& operator>>(std::istream& in, MyClass& mc) {
    return in >> mc.i;
}

void change_id(int id) {
    MyClass::id = id;
}

int main() {
    MyClass mc(7);
    std::cout << mc << std::endl;
    // mc.i = 333*2;  // 错误: i 是私有成员

    std::istringstream("100") >> mc;
    std::cout << mc << std::endl;

    // MyClass::id = 222*3;  // 错误: id 是私有成员
    change_id(9);
    std::cout << mc << std::endl;

    return 0;
}

// 输出:
// MyClass::id = 6; i = 7
// MyClass::id = 6; i = 100
// MyClass::id = 9; i = 100
```

## 7. 总结

### 核心要点

| 特性 | 说明 |
|------|------|
| **目的** | 授予特定函数或类访问私有/保护成员的权限 |
| **方向性** | 单向关系，非双向 |
| **传递性** | 不传递，A 是 B 的友元，B 是 C 的友元，A 不是 C 的友元 |
| **继承性** | 不继承，派生类不继承基类的友元 |
| **位置无关** | 可放在 private、protected 或 public 区域，效果相同 |

### 技术对比

| 访问方式 | 访问权限 | 封装性 | 推荐场景 |
|---------|---------|--------|---------|
| 公共成员 | 所有人可访问 | 最弱 | 接口成员 |
| 保护成员 | 类及派生类可访问 | 中等 | 供派生类使用的实现细节 |
| 私有成员 | 仅类本身可访问 | 最强 | 内部实现细节 |
| 友元 | 特定函数/类可访问 | 可控例外 | 运算符重载、紧密协作的类 |

### 使用建议

1. **谨慎使用**：友元破坏封装，只在必要时使用
2. **优先公共接口**：尽可能通过公共成员函数实现功能
3. **明确意图**：为友元声明添加注释，说明必要性
4. **集中管理**：将友元声明放在类的统一位置
5. **避免循环依赖**：友元关系可能导致紧密耦合

### 相关概念

| 概念 | 关系 |
|------|------|
| **访问说明符** | public/protected/private 定义成员的可见性 |
| **类类型** | 定义包含数据成员和成员函数的类型 |
| **运算符重载** | 友元的典型应用场景之一 |

## 参考资料

- C++26 标准 (ISO/IEC 14882:2026): 11.8.4 Friends [class.friend], 13.7.5 Friends [temp.friend]
- C++23 标准 (ISO/IEC 14882:2024): 11.8.4 Friends [class.friend], 13.7.5 Friends [temp.friend]
- C++20 标准 (ISO/IEC 14882:2020): 11.9.3 Friends [class.friend], 13.7.4 Friends [temp.friend]
- cppreference: https://en.cppreference.com/w/cpp/language/friend