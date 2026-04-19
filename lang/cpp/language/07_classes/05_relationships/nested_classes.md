# 嵌套类 (Nested Classes)

## 1. 概述

**嵌套类 (Nested Class)** 是指在另一个类（称为外围类或包围类，enclosing class）内部声明的类、结构体或联合体。嵌套类是外围类的成员，其名称存在于外围类的作用域中。

嵌套类的主要特点：
- 嵌套类的名称位于外围类的作用域内
- 嵌套类拥有与外围类其他成员相同的访问权限（包括访问私有成员）
- 嵌套类独立于外围类，没有特殊的 `this` 指针访问权限

## 2. 来源与演变

### 首次引入

嵌套类自 **C++98** 标准起就存在，是 C++ 面向对象特性的基本组成部分。

### 历史背景

嵌套类的设计动机包括：
- **逻辑分组**：将相关的类组织在一起，体现类之间的逻辑关系
- **封装性**：通过将辅助类声明为私有嵌套类，隐藏实现细节
- **命名空间管理**：避免全局命名空间污染
- **访问控制**：嵌套类可以访问外围类的私有成员

### 标准演进

| 版本 | 变化 |
|------|------|
| C++98 | 首次引入嵌套类概念 |
| C++11 | 允许在未求值的上下文（如 `sizeof`）中使用非静态成员 |

### 缺陷报告

| DR | 适用版本 | 原行为 | 修正行为 |
|-----|---------|--------|----------|
| CWG 45 | C++98 | 嵌套类成员无法访问外围类及其友元 | 嵌套类拥有与外围类其他成员相同的访问权限（同时解决了 CWG #8 和 #10） |

## 3. 语法与声明

### 基本语法

```cpp
class OuterClass {        // 外围类
    // ...
    class NestedClass {   // 嵌套类
        // ...
    };
    // ...
};
```

### 嵌套类成员的外部定义

嵌套类的成员可以在外围类的作用域内进行外部定义：

```cpp
struct enclose {
    struct inner {
        static int x;
        void f(int i);
    };
};

int enclose::inner::x = 1;        // 静态成员定义
void enclose::inner::f(int i) {}  // 成员函数定义
```

### 前向声明与延迟定义

嵌套类可以先声明后定义：

```cpp
class enclose {
    class nested1;    // 前向声明
    class nested2;    // 前向声明
    class nested1 {}; // 在外围类内部定义
};

class enclose::nested2 {}; // 在外围类外部定义
```

### 访问说明符

嵌套类遵循成员访问说明符：
- `public`：可在外围类外部访问
- `protected`：仅在外围类及其派生类中可访问
- `private`：仅在外围类内部可访问

## 4. 底层原理

### 名称查找规则

从嵌套类的成员函数进行名称查找时：
1. 首先检查嵌套类自身的作用域
2. 然后检查外围类的作用域
3. 最后检查全局作用域

### 访问权限

嵌套类作为外围类的成员，拥有以下访问权限：
- 可以访问外围类的所有成员（包括 `private` 和 `protected`）
- 可以访问外围类的静态成员（无需实例）
- 访问外围类的非静态成员需要通过对象实例

### 独立性

嵌套类与外围类的关系：
- 嵌套类**不拥有**外围类的 `this` 指针
- 嵌套类不能直接访问外围类的非静态成员
- 每个嵌套类对象独立于外围类对象存在

### 友元函数的特殊规则

在嵌套类内部定义的友元函数**没有特殊权限**访问外围类的成员：

```cpp
class enclose {
    int private_data;
    struct inner {
        friend void f(inner&) {
            // 不能直接访问 enclose::private_data
        }
    };
};
```

## 5. 使用场景

### 适合使用嵌套类的场景

| 场景 | 说明 |
|------|------|
| 实现细节隐藏 | 将辅助类声明为私有嵌套类，隐藏实现 |
| 逻辑分组 | 将紧密相关的类组织在一起 |
| 避免命名冲突 | 嵌套类名称位于外围类作用域内 |
| 迭代器实现 | 容器类的迭代器通常实现为嵌套类 |
| 回调/策略模式 | 将策略类或回调类嵌套在使用它的类中 |

### 最佳实践

1. **优先使用私有嵌套类**：当嵌套类仅用于实现细节时，声明为 `private` 或 `protected`
2. **保持嵌套类简洁**：嵌套类应较小且功能单一
3. **考虑 Pimpl 惯用法**：对于大型实现细节，考虑使用 Pimpl 而非复杂嵌套类

### 常见陷阱

1. **误认为嵌套类自动拥有外围类实例**
2. **忘记访问非静态成员需要外围类对象**
3. **过度使用嵌套类导致类定义臃肿**

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

// 全局变量
int x = 100, y = 200;

class enclose {  // 外围类
private:
    int x;              // 私有非静态成员
    static int s;       // 私有静态成员

public:
    enclose(int val) : x(val) {}

    // 嵌套类
    struct inner {
        void f(int i) {
            // x = i;    // 错误：不能直接访问非静态成员
            s = i;       // 正确：可以访问静态成员
            ::x = i;     // 正确：访问全局 x
            y = i;       // 正确：访问全局 y
        }

        void g(enclose* p, int i) {
            p->x = i;    // 正确：通过对象访问非静态成员
        }
    };

    static int get_s() { return s; }
};

int enclose::s = 0;

int main() {
    enclose obj(42);
    enclose::inner in;

    in.f(10);
    in.g(&obj, 20);

    std::cout << "静态成员 s: " << enclose::get_s() << std::endl;
    std::cout << "全局 x: " << x << std::endl;

    return 0;
}
```

### 高级用法：迭代器实现

```cpp
#include <iostream>
#include <vector>

class IntVector {
private:
    std::vector<int> data;

public:
    IntVector(std::initializer_list<int> init) : data(init) {}

    // 公有嵌套类：迭代器
    class Iterator {
    private:
        std::vector<int>::iterator it;

    public:
        Iterator(std::vector<int>::iterator i) : it(i) {}

        int& operator*() { return *it; }
        Iterator& operator++() { ++it; return *this; }
        bool operator!=(const Iterator& other) const { return it != other.it; }
    };

    Iterator begin() { return Iterator(data.begin()); }
    Iterator end() { return Iterator(data.end()); }

    // 私有嵌套类：实现细节
    class Helper {
    public:
        static void print(const IntVector& v) {
            for (int i : v.data) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
    };

    friend class Helper;
};

int main() {
    IntVector vec{1, 2, 3, 4, 5};

    // 使用公有嵌套类（迭代器）
    for (int i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### 访问控制示例

```cpp
#include <iostream>

class enclose {
private:
    struct nested {  // 私有嵌套类
        void g() { std::cout << "nested::g()" << std::endl; }
    };

public:
    static nested f() { return nested{}; }
};

int main() {
    // enclose::nested n1;           // 错误：'nested' 是私有的

    // 正确：不直接命名 'nested'
    enclose::f().g();                 // OK

    auto n2 = enclose::f();           // OK：auto 推导
    n2.g();

    return 0;
}
```

### 常见错误及修正

#### 错误 1：直接访问外围类非静态成员

```cpp
class outer {
    int value = 42;
public:
    class inner {
        void wrong() {
            // value = 10;  // 错误：不能直接访问非静态成员
        }

        void correct(outer& o) {
            o.value = 10;     // 正确：通过对象访问
        }
    };
};
```

#### 错误 2：C++11 前在 sizeof 中使用非静态成员

```cpp
class outer {
    int x;
public:
    struct inner {
        void f() {
            // C++98: 错误
            // C++11: 正确（sizeof 操作数未求值）
            int size = sizeof(x);
        }
    };
};
```

#### 错误 3：友元函数访问外围类私有成员

```cpp
class outer {
    int private_data;
public:
    struct inner {
        friend void helper(inner&) {
            // outer::private_data = 0;  // 错误：友元函数无权访问外围类私有成员
        }
    };
};

// 修正：将友元函数声明为外围类的友元
class outer_fixed {
    int private_data;
    friend void helper2();  // 声明友元
public:
    struct inner {
        friend void helper2();  // 同一友元函数
    };
};

void helper2() {
    outer_fixed::inner obj;
    // 现在可以访问 outer_fixed::private_data
}
```

## 注意事项

1. **非静态成员访问**：嵌套类访问外围类的非静态成员必须通过对象实例
2. **访问说明符生效**：私有嵌套类不能在外围类外部直接命名
3. **auto 类型推导**：可以使用 `auto` 存储私有嵌套类对象
4. **sizeof 规则**：C++11 起，`sizeof` 中可以使用外围类非静态成员

## 相关概念

| 概念 | 关系 |
|------|------|
| 局部类 (Local Class) | 在函数内部定义的类 |
| Pimpl 惯用法 | 使用嵌套类隐藏实现细节的替代方案 |
| 友元类 (Friend Class) | 授予另一个类访问私有成员的权限 |
| 命名空间 (Namespace) | 另一种组织代码和避免命名冲突的方式 |

## 7. 总结

嵌套类是 C++ 提供的一种组织代码结构的机制：

**核心特性**：
- 嵌套类名称位于外围类作用域
- 嵌套类可访问外围类的所有成员（包括私有成员）
- 嵌套类独立于外围类对象存在

**使用建议**：
1. 使用私有嵌套类隐藏实现细节
2. 访问非静态成员时需要外围类对象
3. C++11 起，`sizeof` 等未求值上下文可使用非静态成员
4. 避免过度嵌套导致代码复杂

**典型应用**：
- 容器迭代器
- 回调对象
- 辅助数据结构
- 状态机状态类

## 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 11.4.12 Nested class declarations [class.nest]
- C++20 标准 (ISO/IEC 14882:2020): 11.4.10 Nested class declarations [class.nest]
- C++17 标准 (ISO/IEC 14882:2017): 12.2.5 Nested class declarations [class.nest]
- cppreference: https://en.cppreference.com/w/cpp/language/nested_types