# typeid 运算符

## 1. 概述 (Overview)

`typeid` 是 C++ 的类型识别运算符，用于在运行时或编译时查询类型信息。它返回一个 `std::type_info` 对象的引用，该对象包含类型的详细信息。

### 概念定义

`typeid` 运算符（operator）用于获取类型或表达式的类型信息。它是一个左值表达式，引用一个具有静态存储期的对象，该对象的类型是 `std::type_info` 的 const 限定版本或其派生类型。

### 主要用途

- **多态类型识别**：在运行时确定多态对象的实际动态类型
- **静态类型识别**：在编译时确定非多态类型的类型信息
- **类型比较**：通过 `std::type_info` 提供的接口进行类型相等性比较

### 技术定位

`typeid` 是 C++ 运行时类型信息（RTTI，Runtime Type Information）系统的核心组成部分，与 `dynamic_cast` 一起构成了 C++ 的类型反射机制。

---

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`typeid` 运算符在 C++ 标准化过程中被引入，旨在提供安全的运行时类型识别机制。在 C++ 早期，开发者往往使用非标准的方法（如自定义类型标签）来实现类型识别。

### 设计动机

1. **类型安全的向下转型**：配合 `dynamic_cast` 实现安全的向下转型
2. **异构容器管理**：在存储不同类型对象的容器中识别对象类型
3. **调试与诊断**：在调试时输出类型名称，辅助问题定位
4. **序列化/反序列化**：根据对象类型选择相应的处理方式

### 版本变更

| 版本 | 变更内容 |
|------|----------|
| C++98 | 首次引入 `typeid` 运算符 |
| C++11 | 将左值表达式要求改为泛左值（glvalue）表达式 |
| C++17 | 对纯右值（prvalue）参数正式执行临时量实质化 |

### 缺陷报告修正

| 缺陷报告 | 应用版本 | 原始行为 | 修正后行为 |
|----------|----------|----------|------------|
| CWG 492 | C++98 | 对 cv 限定类型的引用，结果表示被引用的类型 | 结果表示去 cv 限定的被引用类型 |
| CWG 1416 | C++98 | 关于顶层 cv 限定的描述可能被误解 | 改进了措辞 |
| CWG 1431 | C++98 | `typeid` 只允许抛出 `std::bad_typeid` | 允许抛出可匹配的派生类 |
| CWG 1954 | C++98 | 不清楚空指针解引用是否可在子表达式中检查 | 仅在顶层检查 |

---

## 3. 语法与参数 (Syntax and Parameters)

### 核心语法

```cpp
typeid( type )       // (1) 类型形式
typeid( expression ) // (2) 表达式形式
```

### 参数说明

#### 形式 (1)：类型形式

| 参数 | 说明 |
|------|------|
| `type` | 类型标识符，可以是基本类型、类类型、指针类型等。如果类型是引用类型，结果表示去 cv 限定的被引用类型。 |

**要求**：
- 类型不能是不完整类型（incomplete type）
- 必须能访问 `std::type_info` 的定义

#### 形式 (2)：表达式形式

| 参数 | 说明 |
|------|------|
| `expression` | 任意表达式，用于推断其类型。根据表达式类型和求值特性，行为有所不同。 |

**要求**：
- 如果表达式类型或其引用类型是类类型，该类不能是不完整类型
- 如果 `std::type_info` 定义不可见，程序是病态的（ill-formed）

### 返回值

返回 `const std::type_info&`，引用一个代表类型信息的对象。该对象具有静态存储期。

### cv 限定符处理

无论类型或表达式类型是否带有 cv 限定符（const、volatile），`typeid` 的结果总是表示去 cv 限定的类型：

```cpp
typeid(const T) == typeid(T)    // true
typeid(volatile T) == typeid(T) // true
typeid(const volatile T&) == typeid(T) // true
```

---

## 4. 底层原理 (Underlying Principles)

### 实现机制

#### 静态类型识别

对于非多态类型，`typeid` 在编译时确定类型信息：

1. 编译器为每种类型生成一个 `std::type_info` 对象（或其派生类对象）
2. `typeid` 表达式直接指向该静态对象
3. 无运行时开销

```cpp
int x;
typeid(x); // 编译时确定，指向 int 的 type_info 对象
```

#### 动态类型识别

对于多态类型（polymorphic type，即至少有一个虚函数的类），`typeid` 需要运行时确定动态类型：

1. 编译器为每个多态类生成虚函数表（vtable，virtual table）
2. 虚函数表中包含指向 `type_info` 对象的指针
3. `typeid` 通过对象的 vptr 访问 vtable 中的 type_info 指针
4. 运行时通过一次虚函数表查找获取实际类型

```cpp
struct Base { virtual void foo() {} };
struct Derived : Base {};

Base* ptr = new Derived;
typeid(*ptr); // 运行时查找，返回 Derived 的 type_info
```

### 核心算法

```
typeid(expression) 求值流程：

1. 确定 expression 的静态类型 T
2. T 是否是多态类型？
   ├─ 否：返回 typeid(静态类型 T)
   │       不求值 expression
   │
   └─ 是：expression 是否是泛左值（glvalue）？
           ├─ 否：返回 typeid(静态类型 T)
           │       不求值 expression（C++17 前不进行临时量实质化）
           │
           └─ 是：求值 expression
                  检查是否为空指针解引用
                  ├─ 是：抛出 std::bad_typeid
                  └─ 否：通过 vtable 获取动态类型
                          返回对应的 type_info 对象
```

### 性能特征

| 场景 | 时间复杂度 | 空间开销 | 求值行为 |
|------|-----------|----------|----------|
| 非多态类型 | O(1) 编译时 | 每类型一个 type_info 对象 | 不求值表达式 |
| 多态类型 | O(1) 运行时 | 额外的 vtable 条目 | 求值表达式（glvalue） |
| 空指针解引用 | - | - | 抛出异常 |

### 类型信息存储

`std::type_info` 类通常包含：

- **类型名称**：通过 `name()` 成员函数获取（实现定义的格式）
- **哈希码**：通过 `hash_code()` 获取，用于容器中的类型比较
- **比较操作**：`operator==` 和 `operator!=` 用于类型相等性比较
- **before 关系**：`before()` 用于定义类型的全序关系

---

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 运行时多态类型识别

```cpp
void processShape(Shape* shape) {
    if (typeid(*shape) == typeid(Circle)) {
        // 处理圆形特定逻辑
    } else if (typeid(*shape) == typeid(Rectangle)) {
        // 处理矩形特定逻辑
    }
}
```

#### 2. 类型名称输出（调试用）

```cpp
template<typename T>
void printTypeName(const T& obj) {
    std::cout << "Type: " << typeid(obj).name() << std::endl;
}
```

#### 3. 类型比较与哈希

```cpp
#include <typeindex>
#include <unordered_map>

std::unordered_map<std::type_index, Handler> handlers;
handlers[std::type_index(typeid(MyType))] = myHandler;
```

#### 4. 异构容器中的类型索引

```cpp
std::vector<std::any> container;
// ... 添加元素
for (const auto& item : container) {
    std::cout << "Item type: " << item.type().name() << std::endl;
}
```

### 最佳实践

1. **优先使用虚函数**：如果可以通过虚函数解决问题，避免使用 `typeid`
2. **缓存 type_info 引用**：如果多次使用同一类型的 type_info，缓存引用避免重复查找
3. **使用 type_index 作为键**：在关联容器中使用 `std::type_index` 而非 `std::type_info`
4. **注意 name() 可移植性**：`name()` 返回的字符串是实现定义的，不同编译器格式不同

### 注意事项

#### 表达式求值规则

- **多态类型的泛左值**：表达式会被求值
- **其他情况**：表达式不会被求值

```cpp
int x = 0;
typeid(x++); // x 不会递增，因为 int 是非多态类型

struct Poly { virtual void foo() {} };
Poly p;
typeid(p); // p 会被求值（虽然是同一对象，但表达求值语义）
```

#### 空指针解引用异常

```cpp
Poly* ptr = nullptr;
typeid(*ptr); // 抛出 std::bad_typeid
```

#### 对象构造/析构期间的行为

在构造函数或析构函数中使用 `typeid` 时，返回的是当前正在构造或析构的类的类型信息：

```cpp
struct Base {
    Base() {
        std::cout << typeid(*this).name() << std::endl; // 输出 "Base"
    }
    virtual ~Base() = default;
};

struct Derived : Base {
    Derived() {
        std::cout << typeid(*this).name() << std::endl; // 输出 "Derived"
    }
};
```

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 指针与对象的混淆 | `typeid(ptr)` 获取指针类型，`typeid(*ptr)` 获取对象类型 | 注意解引用 |
| type_info 对象地址比较 | `&typeid(T1) == &typeid(T2)` 不保证成立 | 使用 `==` 运算符比较 |
| name() 结果的可移植性 | 不同编译器输出格式不同 | 仅用于调试，不用于逻辑判断 |
| 空指针解引用 | 多态类型空指针解引用会抛出异常 | 使用前检查指针有效性 |

---

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>
#include <typeinfo>

int main() {
    // 基本类型
    int i = 42;
    double d = 3.14;

    std::cout << "i is " << typeid(i).name() << std::endl;
    std::cout << "d is " << typeid(d).name() << std::endl;

    // 类型形式
    std::cout << "int is " << typeid(int).name() << std::endl;
    std::cout << "double* is " << typeid(double*).name() << std::endl;

    // cv 限定符被忽略
    const int ci = 10;
    std::cout << "const int == int: "
              << (typeid(ci) == typeid(int)) << std::endl; // true

    return 0;
}
```

### 多态类型示例

```cpp
#include <iostream>
#include <typeinfo>
#include <string>

// 非多态类
struct Base {
    int data;
};

struct Derived : Base {
    double value;
};

// 多态类
struct PolyBase {
    virtual void foo() {}
    virtual ~PolyBase() = default;
};

struct PolyDerived : PolyBase {
    void foo() override {}
};

int main() {
    // 非多态类型：静态绑定
    Derived d;
    Base& ref = d;
    std::cout << "Non-polymorphic: " << typeid(ref).name() << std::endl;
    // 输出 "Base"（静态类型）

    // 多态类型：动态绑定
    PolyDerived pd;
    PolyBase& pref = pd;
    std::cout << "Polymorphic: " << typeid(pref).name() << std::endl;
    // 输出 "PolyDerived"（动态类型）

    return 0;
}
```

### 表达式求值行为

```cpp
#include <iostream>
#include <typeinfo>

struct NonPoly { int x; };
struct Poly { virtual void foo() {} };

int globalCounter = 0;

NonPoly getNonPoly() {
    std::cout << "getNonPoly() called" << std::endl;
    globalCounter++;
    return NonPoly{42};
}

Poly getPoly() {
    std::cout << "getPoly() called" << std::endl;
    globalCounter++;
    return Poly{};
}

int main() {
    // 非多态类型：不求值表达式
    std::cout << "Testing non-polymorphic..." << std::endl;
    typeid(getNonPoly()); // 不会调用 getNonPoly()
    std::cout << "Counter: " << globalCounter << std::endl; // 0

    // 多态类型但为纯右值（C++17 前）：不求值表达式
    // C++17 起：临时量实质化，但函数调用不被执行
    std::cout << "Testing polymorphic prvalue..." << std::endl;
    typeid(getPoly());
    std::cout << "Counter: " << globalCounter << std::endl;

    return 0;
}
```

### 类型比较与映射

```cpp
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <string>
#include <functional>

struct Event { virtual ~Event() = default; };
struct ClickEvent : Event { int x, y; };
struct KeyEvent : Event { int keyCode; };
struct MouseEvent : Event { int button; };

int main() {
    // 使用 type_index 作为键
    std::unordered_map<std::type_index, std::string> eventNames = {
        {std::type_index(typeid(ClickEvent)), "Click"},
        {std::type_index(typeid(KeyEvent)), "Key"},
        {std::type_index(typeid(MouseEvent)), "Mouse"}
    };

    Event* e1 = new ClickEvent{100, 200};
    Event* e2 = new KeyEvent{65}; // 'A' key

    std::cout << "Event 1: " << eventNames[std::type_index(typeid(*e1))]
              << std::endl; // "Click"
    std::cout << "Event 2: " << eventNames[std::type_index(typeid(*e2))]
              << std::endl; // "Key"

    delete e1;
    delete e2;

    return 0;
}
```

### 异常处理示例

```cpp
#include <iostream>
#include <typeinfo>
#include <stdexcept>

struct PolyBase {
    virtual void foo() {}
    virtual ~PolyBase() = default;
};

struct PolyDerived : PolyBase {};

int main() {
    PolyBase* ptr = nullptr;

    try {
        // 对多态类型空指针解引用会抛出异常
        std::cout << typeid(*ptr).name() << std::endl;
    } catch (const std::bad_typeid& e) {
        std::cout << "Caught bad_typeid: " << e.what() << std::endl;
    }

    // 非多态类型空指针解引用：未定义行为
    // 不会抛出异常
    int* intPtr = nullptr;
    // typeid(*intPtr); // 危险！未定义行为

    // 安全做法
    if (ptr != nullptr) {
        std::cout << typeid(*ptr).name() << std::endl;
    } else {
        std::cout << "Pointer is null" << std::endl;
    }

    return 0;
}
```

### 完整示例：类型安全的事件处理

```cpp
#include <iostream>
#include <typeinfo>
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>

// 事件基类
struct Event {
    virtual ~Event() = default;
};

struct ClickEvent : Event {
    int x, y;
    ClickEvent(int x, int y) : x(x), y(y) {}
};

struct KeyEvent : Event {
    int keyCode;
    bool isPressed;
    KeyEvent(int code, bool pressed) : keyCode(code), isPressed(pressed) {}
};

// 类型安全的事件分发器
class EventDispatcher {
public:
    using Handler = std::function<void(Event*)>;

    template<typename T>
    void registerHandler(std::function<void(T*)> handler) {
        handlers[std::type_index(typeid(T))] =
            [handler](Event* e) {
                handler(static_cast<T*>(e));
            };
    }

    void dispatch(Event* event) {
        auto it = handlers.find(std::type_index(typeid(*event)));
        if (it != handlers.end()) {
            it->second(event);
        } else {
            std::cout << "No handler for event type: "
                      << typeid(*event).name() << std::endl;
        }
    }

private:
    std::unordered_map<std::type_index, Handler> handlers;
};

int main() {
    EventDispatcher dispatcher;

    // 注册事件处理器
    dispatcher.registerHandler<ClickEvent>([](ClickEvent* e) {
        std::cout << "Click at (" << e->x << ", " << e->y << ")" << std::endl;
    });

    dispatcher.registerHandler<KeyEvent>([](KeyEvent* e) {
        std::cout << "Key " << (e->isPressed ? "pressed" : "released")
                  << ": " << e->keyCode << std::endl;
    });

    // 分发事件
    auto click = std::make_unique<ClickEvent>(100, 200);
    auto keyPress = std::make_unique<KeyEvent>(65, true);
    auto keyRelease = std::make_unique<KeyEvent>(65, false);

    dispatcher.dispatch(click.get());
    dispatcher.dispatch(keyPress.get());
    dispatcher.dispatch(keyRelease.get());

    return 0;
}
```

### 常见错误及修正

#### 错误 1：比较 type_info 对象地址

```cpp
// 错误：不能保证地址相同
const std::type_info& t1 = typeid(int);
const std::type_info& t2 = typeid(int);
if (&t1 == &t2) { /* 不保证成立 */ }

// 正确：使用 == 运算符
if (t1 == t2) { /* 保证成立 */ }

// 或使用 hash_code
if (t1.hash_code() == t2.hash_code()) { /* 保证成立 */ }
```

#### 错误 2：误解 name() 的可移植性

```cpp
// 错误：依赖 name() 的特定输出格式
if (std::string(typeid(obj).name()) == "MyClass") { /* 不可移植 */ }

// 正确：使用 type_index 或直接比较
if (typeid(obj) == typeid(MyClass)) { /* 可移植 */ }

// 或者使用 type_index 作为容器键
std::unordered_map<std::type_index, std::string> typeNames = {
    {std::type_index(typeid(MyClass)), "MyClass"}
};
```

#### 错误 3：忽略多态类型的要求

```cpp
// 错误：非多态类型无法获取动态类型
struct NonPolyBase { int x; };
struct NonPolyDerived : NonPolyBase { double y; };

NonPolyBase* ptr = new NonPolyDerived;
std::cout << typeid(*ptr).name() << std::endl; // 输出 "NonPolyBase"

// 正确：添加虚函数使其成为多态类型
struct PolyBase { virtual ~PolyBase() = default; int x; };
struct PolyDerived : PolyBase { double y; };

PolyBase* polyPtr = new PolyDerived;
std::cout << typeid(*polyPtr).name() << std::endl; // 输出 "PolyDerived"
```

---

## 7. 总结 (Summary)

### 核心要点

1. **双形式语法**：`typeid(type)` 用于类型，`typeid(expression)` 用于表达式
2. **静态与动态识别**：非多态类型在编译时确定，多态类型在运行时通过 vtable 确定
3. **返回 type_info 引用**：返回 `const std::type_info&`，代表类型的运行时信息
4. **cv 限定符忽略**：结果总是表示去 cv 限定的类型
5. **表达式求值语义**：仅对多态类型的泛左值表达式求值

### 技术对比

| 特性 | typeid | decltype | auto |
|------|--------|----------|------|
| 求值时机 | 部分运行时 | 编译时 | 编译时 |
| 多态支持 | 是（动态类型） | 否 | 否 |
| 返回值 | type_info 引用 | 类型 | 类型 |
| 表达式求值 | 视情况 | 否 | 否 |
| 主要用途 | 运行时类型识别 | 类型推导 | 类型推导 |

### 性能考虑

- 非多态类型：零运行时开销
- 多态类型：一次 vtable 查找（常数时间）
- 建议：优先使用虚函数替代 `typeid` 类型判断

### 学习建议

1. **理解 RTTI 机制**：学习虚函数表和 `type_info` 的底层实现
2. **掌握求值规则**：理解何时表达式会被求值
3. **实践类型安全设计**：用 `typeid` 和 `type_index` 实现类型安全的事件系统
4. **避免滥用**：优先使用多态和虚函数，仅在必要时使用 `typeid`
5. **了解编译器差异**：`name()` 输出在不同编译器间不同（GCC 使用 name mangling，MSVC 使用可读名称）

### 相关主题

- `std::type_info`：类型信息类
- `std::type_index`：用于容器键的类型索引包装
- `dynamic_cast`：安全的向下转型
- `std::any`：类型安全的任意类型容器
- `std::variant`：类型安全的联合体

---

## 参考资料

- C++ 标准文档：[expr.typeid]
- cppreference: https://en.cppreference.com/w/cpp/language/typeid
- 《C++ Primer》（第5版）：第19章
- 《Effective C++》：条款27