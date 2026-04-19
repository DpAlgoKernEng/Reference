# 访问说明符 (Access Specifiers)

## 1. 概述

访问说明符（Access Specifiers）是 C++ 中用于控制类成员和基类可访问性的关键字。它们定义了类成员的封装边界，是面向对象编程中封装（Encapsulation）特性的核心实现机制。

C++ 提供三种访问说明符：
- **public**：公共访问，任何代码都可以访问
- **protected**：受保护访问，仅类本身、友元和派生类可访问
- **private**：私有访问，仅类本身和友元可访问

访问说明符应用于两个场景：
1. **成员访问控制**：在类的成员说明中定义后续成员的可访问性
2. **继承访问控制**：在派生类声明中定义继承自基类的成员的可访问性

## 2. 来源与演变

### 历史背景

访问控制机制最早由 **Simula 67** 语言引入，用于实现数据封装。Bjarne Stroustrup 在设计 C++ 时借鉴了这一概念，将其作为面向对象编程的核心特性之一。

### 设计动机

访问说明符的设计目标：
1. **封装实现细节**：将类的内部实现与公共接口分离
2. **保护数据完整性**：防止外部代码直接修改内部状态
3. **提供清晰的接口**：明确区分"用户可见"和"实现细节"
4. **支持继承层次**：控制派生类对基类成员的访问权限

### C++11 变化

- 对于标准布局类型（standard-layout types），所有非静态数据成员必须具有相同的访问级别
- 在 C++11 之前，地址顺序保证仅适用于未被访问说明符分隔的成员

### C++23 变化

- 移除了访问说明符影响类布局的限制

### 缺陷报告

| DR | 应用版本 | 原行为 | 修正行为 |
|----|---------|--------|---------|
| CWG 1873 | C++98 | 受保护成员对派生类的友元可访问 | 修改为不可访问 |

## 3. 语法与参数

### 成员访问说明符语法

```cpp
public:     // 后续成员具有公共访问权限
    member-declarations

protected:  // 后续成员具有受保护访问权限
    member-declarations

private:    // 后续成员具有私有访问权限
    member-declarations
```

### 继承访问说明符语法

```cpp
class Derived : public Base       // 公共继承
class Derived : protected Base    // 受保护继承
class Derived : private Base     // 私有继承
```

### 访问级别对照表

| 访问说明符 | 类内部 | 友元 | 派生类 | 外部代码 |
|-----------|--------|------|--------|---------|
| `public` | 可访问 | 可访问 | 可访问 | 可访问 |
| `protected` | 可访问 | 可访问 | 可访问 | 不可访问 |
| `private` | 可访问 | 可访问 | 不可访问 | 不可访问 |

### 继承访问规则

| 继承方式 | 基类 public 成员 | 基类 protected 成员 | 基类 private 成员 |
|---------|-----------------|--------------------|--------------------|
| `public` 继承 | 派生类中仍为 public | 派生类中仍为 protected | 不可访问 |
| `protected` 继承 | 派生类中变为 protected | 派生类中仍为 protected | 不可访问 |
| `private` 继承 | 派生类中变为 private | 派生类中变为 private | 不可访问 |

### 默认访问级别

| 类类型 | 成员默认访问 | 继承默认访问 |
|-------|-------------|-------------|
| `class` | private | private |
| `struct` | public | public |
| `union` | public | N/A |

## 4. 底层原理

### 访问检查机制

访问检查是编译时操作，不产生运行时开销。编译器在编译阶段执行以下步骤：

1. **名称查找**：首先确定名称引用的实体
2. **重载决议**：选择最佳匹配的函数
3. **访问检查**：最后检查访问权限

这种设计确保了"将 `private` 改为 `public` 永远不会改变程序行为"的原则。

### 访问与可见性

访问控制**不影响可见性**：
- 私有成员仍然参与重载决议
- 到不可访问基类的隐式转换仍被考虑
- 私有成员函数仍参与候选函数集

```cpp
class Base {
private:
    void f(int);  // 私有但仍可见
public:
    void f(double);
};

void g(Base& b) {
    b.f(42);  // 调用 f(double)，因为 f(int) 不可访问
              // 但 f(int) 仍参与了重载决议
}
```

### 虚函数访问检查

虚函数的访问检查在调用点进行，使用调用表达式中对象类型的访问权限：

```cpp
struct B {
    virtual int f();  // B 中 f 是 public
};

class D : public B {
private:
    int f();  // D 中 f 是 private
};

void func() {
    D d;
    B& b = d;
    b.f();  // OK: 使用 B::f 的访问权限（public）
    d.f();  // 错误: D::f 是 private
}
```

### 多重继承中的访问

当成员通过继承图中的多条路径可达时，取最具访问权限的路径：

```cpp
class W { public: void f(); };
class A : private virtual W {};
class B : public virtual W {};
class C : public A, public B {
    void f() {
        W::f();  // OK: 通过 B 路径可访问
    }
};
```

## 5. 使用场景

### 公共成员访问 (public)

**适用场景**：
- 类的公共接口（API）
- 需要外部代码调用的方法
- 常量、枚举、类型别名等公共类型定义

**最佳实践**：
- 将公共接口放在类定义的开头
- 公共成员应保持稳定，避免频繁修改

### 受保护成员访问 (protected)

**适用场景**：
- 供派生类使用的内部接口
- 需要派生类重写的虚函数
- 派生类需要访问但外部不应访问的数据

**注意事项**：
- 受保护成员只能通过派生类类型访问，不能通过基类类型访问
- 友元函数的访问权限与声明位置相关

### 私有成员访问 (private)

**适用场景**：
- 实现细节
- 内部辅助函数
- 数据成员
- 不应被派生类修改的状态

**最佳实践**：
- 默认将成员设为 private
- 仅在必要时提升访问级别
- 使用 Pimpl 惯用法进一步隐藏实现

### 常见陷阱

#### 陷阱 1：受保护成员通过基类访问

```cpp
struct Base {
protected:
    int i;
};

struct Derived : Base {
    void f(Base& b, Derived& d) {
        ++d.i;   // OK: 通过 Derived 类型访问
        ++i;     // OK: 通过 this 访问
        // ++b.i;  // 错误: 不能通过 Base 类型访问受保护成员
    }
};
```

#### 陷阱 2：typedef 不改变访问权限

```cpp
class A : X {
    class B {};      // B 是 private
public:
    typedef B BB;   // BB 是 public，但 B 仍是 private
};

void f() {
    A::B y;   // 错误: A::B 是 private
    A::BB x;  // OK: A::BB 是 public（但 BB 是 B 的别名）
}
```

#### 陷阱 3：成员重声明必须保持相同访问级别

```cpp
struct S {
    class A;    // S::A 是 public
private:
    class A {}; // 错误: 不能改变访问级别
};
```

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

class Example {
public:              // 公共接口
    void add(int x) {
        n += x;      // OK: 私有成员可在类内访问
    }

    int get() const {
        return n;
    }

private:             // 实现细节
    int n = 0;
};

int main() {
    Example e;
    e.add(1);        // OK: 公共成员可从外部访问
    std::cout << e.get() << std::endl;  // OK
    // e.n = 7;      // 错误: 私有成员不可从外部访问
}
```

### 继承访问控制

```cpp
#include <iostream>

class Base {
public:
    int public_data;
protected:
    int protected_data;
private:
    int private_data;
};

// 公共继承
class PublicDerived : public Base {
    void foo() {
        public_data = 1;        // OK: 仍为 public
        protected_data = 2;     // OK: 仍为 protected
        // private_data = 3;    // 错误: 不可访问
    }
};

// 私有继承
class PrivateDerived : private Base {
    void foo() {
        public_data = 1;        // OK: 变为 private
        protected_data = 2;     // OK: 变为 private
        // private_data = 3;    // 错误: 不可访问
    }
};

int main() {
    PublicDerived pd;
    pd.public_data = 10;        // OK: public 成员
    // pd.protected_data = 20;  // 错误: protected 成员

    PrivateDerived prd;
    // prd.public_data = 10;    // 错误: 已变为 private
}
```

### 受保护成员访问规则

```cpp
struct Base {
protected:
    int i;
private:
    void g(Base& b, struct Derived& d);
};

struct Derived : Base {
    friend void h(Base& b, Derived& d);

    void f(Base& b, Derived& d) {
        ++d.i;     // OK: 通过 Derived 类型访问
        ++i;       // OK: 通过 this 访问
        // ++b.i;  // 错误: 不能通过 Base 类型访问
    }
};

void Base::g(Base& b, Derived& d) {
    ++i;          // OK: Base 成员函数可访问
    ++b.i;        // OK: Base 成员函数可访问
    ++d.i;        // OK: Base 成员函数可访问
}

void h(Base& b, Derived& d) {
    ++d.i;        // OK: Derived 的友元可访问
    // ++b.i;     // 错误: 不是 Base 的友元
}
```

### 私有成员与复制构造

```cpp
class S {
private:
    int n;

public:
    S() : n(10) {}
    S(const S& other) : n(other.n) {}  // OK: 同类对象可访问私有成员
};

int main() {
    S s1;
    S s2 = s1;  // 复制构造，可访问 other.n
}
```

### 常见错误及修正

#### 错误 1：错误地通过基类访问受保护成员

```cpp
// 错误示例
struct Base {
protected:
    int value;
};

struct Derived : Base {
    void process(Base& b) {
        // b.value = 10;  // 错误: 不能通过 Base 类型访问
    }
};

// 修正：使用派生类类型
struct Derived : Base {
    void process(Derived& d) {
        d.value = 10;  // OK: 通过 Derived 类型访问
    }
};
```

#### 错误 2：忘记友元声明

```cpp
// 错误示例
class Secret {
    int data;
public:
    Secret(int d) : data(d) {}
};

void printSecret(const Secret& s) {
    // std::cout << s.data;  // 错误: data 是私有成员
}

// 修正：添加友元声明
class Secret {
    friend void printSecret(const Secret& s);
private:
    int data;
public:
    Secret(int d) : data(d) {}
};

void printSecret(const Secret& s) {
    std::cout << s.data;  // OK: 友元可访问私有成员
}
```

#### 错误 3：私有继承导致接口不可用

```cpp
// 错误示例
class Base {
public:
    void interface() {}
};

class Derived : private Base {};

void useBase(Base& b) {
    b.interface();
}

int main() {
    Derived d;
    // useBase(d);  // 错误: 私有继承，Base 不可访问
}

// 修正：使用公共继承或提供转发函数
class Derived : private Base {
public:
    using Base::interface;  // 暴露特定接口
};
```

## 7. 总结

### 核心要点

1. **三种访问级别**：public（公共）、protected（受保护）、private（私有）
2. **两种应用场景**：成员访问控制和继承访问控制
3. **编译时检查**：访问控制是编译时机制，无运行时开销
4. **访问不影响可见性**：私有成员仍参与重载决议

### 访问级别选择指南

| 场景 | 推荐访问级别 |
|------|-------------|
| 公共 API 方法 | public |
| 供派生类重写的虚函数 | protected virtual |
| 内部辅助函数 | private |
| 数据成员 | private（默认） |
| 常量/枚举 | public |
| 实现细节类型 | private |

### 继承方式选择指南

| 继承方式 | 适用场景 |
|---------|---------|
| public 继承 | "是一个"关系（is-a），接口继承 |
| protected 继承 | 限制外部访问，允许派生类使用 |
| private 继承 | "用...实现"关系（implementation-in-terms-of） |

### 最佳实践

1. **默认使用 private**：数据成员和实现细节应设为 private
2. **最小暴露原则**：仅暴露必要的接口
3. **优先组合而非继承**：private 继承可用组合替代
4. **使用 friend 谨慎**：友元破坏封装，应限制使用
5. **保持接口稳定**：public 成员变更会影响所有使用者

### 相关概念

| 概念 | 关系 |
|------|------|
| friend 声明 | 授予非成员访问私有/受保护成员的权限 |
| 封装 | 访问说明符是实现封装的核心机制 |
| Pimpl 惯用法 | 通过指针隐藏私有实现细节 |
| 标准布局类型 | C++11 要求所有非静态数据成员具有相同访问级别 |

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/access
- C++ Standard: [class.access]
- Effective C++, Scott Meyers, Item 22