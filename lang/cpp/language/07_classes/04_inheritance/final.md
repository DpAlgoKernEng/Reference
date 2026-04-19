# `final` 说明符 (C++11 起)

## 1. 概述

`final` 是 C++11 引入的一个标识符（identifier），具有特殊含义。它有两种用途：

1. **用于虚函数**：指定该虚函数不能在派生类中被重写（override）
2. **用于类定义**：指定该类不能被继承

`final` 不是保留关键字，而是一个具有特殊含义的标识符。在其他上下文中，它可以用于命名对象和函数。

## 2. 来源与演变

### 首次引入

`final` 说明符在 **C++11** 标准中首次引入，用于增强类的继承控制能力。

### 设计动机

在 `final` 引入之前，C++ 开发者无法在语言层面阻止类的继承或虚函数的重写。这导致：

1. **脆弱基类问题**：基类的实现变化可能破坏派生类
2. **API 稳定性问题**：无法确保关键接口不被修改
3. **性能优化受限**：编译器无法进行去虚拟化（devirtualization）优化

`final` 的引入解决了这些问题，提供了：
- 明确的设计意图表达
- 编译时错误检测
- 编译器优化机会

### C++14 变化

- `final` 可以用于联合体（union）定义，此时会影响 `std::is_final` 的结果（虽然联合体本身不能被继承）

### 标准演进

| 版本 | 标准章节 |
|------|---------|
| C++11 | [class], [class.virtual] |
| C++14 | [class], [class.virtual] |
| C++17 | [class], [class.virtual] |
| C++20 | [class], [class.virtual] |
| C++23 | [class], [class.virtual] |

### 缺陷报告

**CWG 1318**：修复了一个解析歧义问题。当类定义中类名后紧跟 `final` 且成员列表为空时，`final` 始终被解释为说明符而非标识符。

## 3. 语法与参数

### 语法形式

#### 虚函数声明中

```cpp
// (1) 成员函数声明
declarator virt-specifier-seq(optional) pure-specifier(optional)

// (2) 类内成员函数定义
declarator virt-specifier-seq(optional) function-body
```

在虚函数声明或定义中，`final` 出现在声明符之后，纯说明符（`=0`）之前：

```cpp
virtual void foo() final;        // 声明
virtual void foo() final = 0;    // 纯虚函数
virtual void foo() final override;  // 可与 override 组合
virtual void foo() override final;  // 顺序可互换
```

#### 类定义中

```cpp
// (3) 类定义
class-key attr(optional) class-head-name class-virt-specifier(optional) base-clause(optional)
```

在类定义中，`final` 出现在类名之后，基类列表之前：

```cpp
class Derived final : Base { };  // 类不能被继承
struct FinalClass final { };       // 结构体不能被继承
```

### 参数说明

| 位置 | 说明 |
|------|------|
| virt-specifier-seq | 可包含 `override`、`final` 或两者组合（顺序不限） |
| class-virt-specifier | 只能是 `final` |

### 语法规则

1. **用于成员函数**：`final` 只能用于虚函数，非虚函数使用 `final` 会导致编译错误
2. **用于类定义**：`final` 只能出现在类定义中，不能出现在类声明（前置声明）中
3. **与 override 组合**：可以同时使用 `final` 和 `override`，顺序不限

## 4. 底层原理

### 编译时检测

`final` 是一个**编译时**特性，不产生任何运行时开销：

1. **虚函数重写检测**：编译器在编译派生类时检查是否重写了 `final` 虚函数，如果是则报错
2. **类继承检测**：编译器在编译派生类定义时检查基类是否为 `final`，如果是则报错

### 去虚拟化优化

`final` 为编译器提供了重要的优化机会：

```cpp
struct Base {
    virtual void foo() {}
};

struct Derived final : Base {
    void foo() override {}
};

void callFoo(Base* b) {
    b->foo();  // 如果 b 实际指向 Derived，编译器知道不能有更深层派生类
}
```

当类或函数标记为 `final` 时，编译器可以进行**去虚拟化（devirtualization）**：
- 直接调用函数，跳过虚函数表查找
- 内联优化更容易实施

### 标识符性质

`final` 不是关键字，而是**具有特殊含义的标识符**：

```cpp
int final = 42;          // 合法：final 作为变量名
void final();            // 合法：final 作为函数名
struct final { };        // 合法：final 作为类名
```

### 解析规则

在特定序列中，`final` 始终被解释为说明符：

```
class-key identifier final : 或 {
```

例如：

```cpp
struct A final { };  // final 是说明符，A 不能被继承
```

## 5. 使用场景

### 适合使用 final 的场景

| 场景 | 说明 |
|------|------|
| API 设计 | 确保关键类不被继承，保持 API 稳定性 |
| 性能优化 | 允许编译器进行去虚拟化优化 |
| 设计意图表达 | 明确表达"禁止继承"的设计决策 |
| 安全控制 | 防止子类破坏基类的不变式 |

### 使用 final 的类

| 典型用例 | 示例 |
|---------|------|
| 不可变类 | `class ImmutableValue final { ... };` |
| 工具类 | `class StringUtils final { ... };` |
| 具体实现类 | `class ConcreteImpl final : public Interface { ... };` |

### 使用 final 的函数

| 典型用例 | 示例 |
|---------|------|
| 终结实现 | 基类模板方法，子类不应再覆盖 |
| 性能关键函数 | 允许编译器优化 |

### 最佳实践

1. **谨慎使用**：`final` 限制了代码扩展性，应在确定不需要继承时使用
2. **配合 override**：在派生类中使用 `final` 时同时使用 `override`，提高可读性
3. **文档说明**：在文档中说明使用 `final` 的原因
4. **性能测试**：声称的性能优化应通过基准测试验证

### 注意事项

1. **非虚函数不能用 final**：会导致编译错误
2. **声明中不能用 final**：`class A final;` 是错误的
3. **联合体中 final 无效**：联合体本身不能被继承，但 `final` 会影响 `std::is_final`

## 6. 代码示例

### 基础用法

```cpp
#include <iostream>

// 基类
struct Base {
    virtual void foo() {
        std::cout << "Base::foo()" << std::endl;
    }

    virtual void bar() {
        std::cout << "Base::bar()" << std::endl;
    }
};

// 派生类 A
struct A : Base {
    // foo() 被标记为 final，不能再被覆盖
    void foo() final override {
        std::cout << "A::foo() - final" << std::endl;
    }

    // bar() 可以被进一步覆盖
    void bar() override {
        std::cout << "A::bar()" << std::endl;
    }
};

// 进一步派生
struct B : A {
    // void foo() override;  // 错误：foo 在 A 中是 final

    void bar() override {     // 正确：bar 可以被覆盖
        std::cout << "B::bar()" << std::endl;
    }
};

// final 类
struct FinalClass final : Base {
    void foo() override {
        std::cout << "FinalClass::foo()" << std::endl;
    }
};

// struct Derived : FinalClass { };  // 错误：FinalClass 是 final

int main() {
    B b;
    b.foo();  // 调用 A::foo()
    b.bar();  // 调用 B::bar()

    return 0;
}
```

### 高级用法

```cpp
#include <iostream>
#include <type_traits>

// 设计模式：模板方法模式中使用 final
class DataProcessor {
public:
    // 公共接口 - 模板方法
    void process() final {
        readData();
        doProcess();   // 调用派生类实现
        writeData();
    }

    virtual ~DataProcessor() = default;

protected:
    virtual void doProcess() = 0;

private:
    void readData() {
        std::cout << "Reading data..." << std::endl;
    }

    void writeData() {
        std::cout << "Writing data..." << std::endl;
    }
};

// 具体实现类 - 标记为 final
class ImageProcessor final : public DataProcessor {
protected:
    void doProcess() override {
        std::cout << "Processing image..." << std::endl;
    }
};

// 验证 is_final
struct NotFinal {};
struct IsFinal final {};

static_assert(!std::is_final_v<NotFinal>, "NotFinal should not be final");
static_assert(std::is_final_v<IsFinal>, "IsFinal should be final");

int main() {
    ImageProcessor processor;
    processor.process();

    // ImageProcessor 不能被继承
    // class AdvancedImageProcessor : public ImageProcessor { };  // 错误

    return 0;
}
```

### 常见错误及修正

#### 错误 1：对非虚函数使用 final

```cpp
struct Base {
    void foo() final;  // 错误：foo 不是虚函数
};

// 修正：将函数声明为 virtual
struct Base {
    virtual void foo() final;  // 正确
};
```

#### 错误 2：尝试继承 final 类

```cpp
struct Base final { };

struct Derived : Base { };  // 错误：不能继承 final 类

// 修正：移除基类的 final 或改变设计
struct Base { };             // 移除 final
struct Derived : Base { };   // 正确
```

#### 错误 3：尝试重写 final 函数

```cpp
struct Base {
    virtual void foo();
};

struct A : Base {
    void foo() final;
};

struct B : A {
    void foo() override;  // 错误：foo 在 A 中是 final
};

// 修正：移除 A 中 foo 的 final 或不重写
struct A : Base {
    void foo() override;  // 移除 final
};
```

#### 错误 4：在类声明中使用 final

```cpp
class MyClass final;  // 错误：final 不能用于类声明

// 修正：final 只能用于类定义
class MyClass final { };  // 正确
```

### 特殊用法示例

```cpp
// final 不是关键字，可以作为标识符
struct final final {  // 定义名为 final 的 final 结构体
    int value;
};

void test() {
    // final 作为类型名
    final f{42};           // 正确：声明类型为 final 的变量

    // final 作为变量名（在不同作用域）
    int final = 10;        // 正确：final 作为变量名

    // 嵌套类中使用
    struct Outer {
        struct Inner final { };  // Inner 是 final 的
    };
}

// 联合体中使用 final（影响 std::is_final）
union MyUnion final {  // 合法，但 union 本身不能继承
    int i;
    float f;
};

static_assert(std::is_final_v<MyUnion>, "MyUnion is final");
```

## 注意事项

1. **标识符性质**：`final` 不是关键字，可以在其他上下文中作为标识符使用
2. **编译时检查**：所有 `final` 相关错误都在编译时检测
3. **运行时无开销**：`final` 不产生任何运行时成本
4. **设计权衡**：`final` 限制了代码扩展性，应权衡使用
5. **联合体特殊性**：联合体使用 `final` 只影响 `std::is_final`，因为联合体本身不能被继承

## 相关概念

| 概念 | 关系 |
|------|------|
| `override` | 配对使用，明确表示重写基类虚函数 |
| `virtual` | `final` 只能用于虚函数 |
| `std::is_final` | 类型特征，检查类是否为 `final` |
| 去虚拟化 | `final` 帮助编译器优化虚函数调用 |

## 7. 总结

`final` 说明符是 C++11 引入的重要特性，提供了类继承和虚函数重写的编译时控制机制。

**核心要点**：

| 特性 | 说明 |
|------|------|
| 用途 | 阻止虚函数重写 / 阻止类被继承 |
| 性质 | 具有特殊含义的标识符，非关键字 |
| 检查时机 | 编译时 |
| 运行时开销 | 无 |
| 优化机会 | 去虚拟化、内联优化 |

**使用建议**：

1. 在 API 稳定性要求高的类上使用 `final`
2. 在性能关键的虚函数上使用 `final`
3. 配合 `override` 使用提高代码可读性
4. 在文档中说明使用 `final` 的原因
5. 谨慎使用，避免过度限制代码扩展性

## 参考资料

- cppreference.com: https://en.cppreference.com/w/cpp/language/final
- C++23 标准 (ISO/IEC 14882:2024): [class], [class.virtual]
- C++11 标准 (ISO/IEC 14882:2011): [class], [class.virtual]
- CWG 1318: 关于 `final` 解析歧义的缺陷报告