# 复制消除（Copy Elision）

## 1. 概述（Overview）

**复制消除（Copy Elision）** 是 C++ 中一种编译器优化技术，当满足特定条件时，编译器可以省略从同类型源对象创建类对象的过程，即使所选的构造函数或析构函数具有副作用。

这种优化通过直接在目标位置构造对象，避免了不必要的拷贝或移动操作，从而提高了程序性能。复制消除是 C++ 中少数几种可以改变可观察副作用的优化之一。

### 主要形式

| 形式 | 说明 |
|------|------|
| NRVO（Named Return Value Optimization） | 命名返回值优化：函数返回命名局部对象时的优化 |
| URVO（Unnamed Return Value Optimization） | 未命名返回值优化：函数返回临时对象时的优化（C++17 起成为强制行为） |
| 异常处理中的消除 | throw 表达式和 catch 处理中的对象复制消除 |
| 协程参数消除 | C++20 起，协程参数的复制消除 |

---

## 2. 来源与演变（Origin and Evolution）

### 历史背景

复制消除优化最早由编译器厂商自发实现，用于优化函数返回值等场景中的临时对象创建。早期 C++ 标准将此类优化定义为"允许但非强制"，不同编译器的实现程度不一。

### 版本演进

| 版本 | 变更内容 |
|------|----------|
| C++11 之前 | 复制消除是唯一被允许的改变可观察副作用的优化形式 |
| C++11 | 引入了移动语义，扩展了异常处理中的复制消除规则；即使无法消除复制，编译器也会尝试使用移动构造函数 |
| C++14 | 增加了分配消除和扩展作为第二种允许改变可观察副作用的优化形式 |
| C++17 | 引入**保证复制消除（Guaranteed Copy Elision）**：prvalue（纯右值）语义重构，URVO 成为强制行为，不再被视为"优化" |
| C++20 | 增加协程参数复制消除支持 |
| C++23 | 在 return 语句中，即使源操作数是左值，也会被视为右值 |

### 相关特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|-----------|-----|------|------|
| `__cpp_guaranteed_copy_elision` | `201606L` | C++17 | 通过简化的值类别实现保证复制消除 |

### 缺陷报告

| DR | 应用于 | 已发布行为 | 正确行为 |
|----|--------|-----------|----------|
| CWG 1967 | C++11 | 使用移动构造函数进行复制消除时，仍考虑被移动对象的生命周期 | 不再考虑 |
| CWG 2426 | C++17 | 返回 prvalue 时不要求析构函数 | 析构函数可能被调用 |
| CWG 2930 | C++98 | 只有拷贝/移动操作可以被消除，但非拷贝/移动构造函数也可被复制初始化选择 | 消除相关复制初始化的任何对象构造 |

---

## 3. 语法与参数（Syntax and Parameters）

### NRVO（命名返回值优化）

在返回类型为类的函数的 return 语句中，当操作数是具有自动存储期的非 volatile 对象 `obj` 的名称（非函数参数或异常处理参数）时，可以通过直接在函数调用的结果对象中构造 `obj` 来省略结果对象的复制初始化。

```cpp
T f() {
    T obj;          // 局部对象
    return obj;     // NRVO：obj 可能直接构造在返回值位置
}
```

### URVO（未命名返回值优化）

**C++17 前**：当类对象 `target` 用临时类对象 `obj` 进行复制初始化，且 `obj` 未绑定到引用时，可以通过直接在 `target` 中构造 `obj` 来省略复制初始化。

**C++17 起**：prvalue 不再立即物化，而是直接在其最终目标的存储位置构造。这意味着 URVO 不再是"优化"，而是语言规范的强制行为。

```cpp
T f() {
    return T();     // C++17 前：可能消除复制；C++17 起：保证不复制
}
T x = T(T(f()));    // C++17 起：x 直接由 f() 的结果初始化，无移动
```

### 异常处理中的复制消除（C++11 起）

1. **throw 表达式**：当操作数是具有自动存储期的非 volatile 对象 `obj` 的名称，且该对象所属的作用域不包含最内层包围的 try 块时，可以通过直接在异常对象中构造 `obj` 来省略异常对象的复制初始化。

2. **catch 处理器**：如果程序语义不变（除处理器参数的构造函数和析构函数执行外），可以将处理器参数视为异常对象的别名，省略其复制初始化。

### 协程参数消除（C++20 起）

协程参数的副本可以被消除。在此情况下，如果程序语义不变（除参数副本对象的构造函数和析构函数执行外），对该副本的引用将被替换为对相应参数的引用。

---

## 4. 底层原理（Underlying Principles）

### 对象身份统一

当复制消除发生时，实现将被省略初始化的源和目标视为引用同一对象的两种不同方式。

#### 析构时机（C++11 前）

析构发生在两个对象原本会被析构的较晚时间点。

#### 析构时机（C++11 起）

如果所选构造函数的第一个参数是对象类型的右值引用，则该对象的析构发生在目标对象原本会被析构的时间点。否则，析构发生在两个对象原本会被析构的较晚时间点。

### Prvalue 语义重构（C++17 起）

C++17 对 prvalue（纯右值）和临时对象的语义进行了根本性重构：

1. **延迟物化（Deferred Materialization）**：prvalue 直到需要时才被物化，然后直接在其最终目标的存储位置构造。

2. **值传递而非对象传递**：prvalue 被返回和使用时不再创建临时对象，而是以"未物化的值"形式传递。

3. **不需要可访问的复制/移动构造函数**：由于语言语法上看起来像复制/移动的操作实际上并未发生，类型甚至不需要有可访问的复制/移动构造函数。

```cpp
// C++17 起，即使类型不可复制也不可移动，也能编译
struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = delete;
};

NonCopyable make() {
    return NonCopyable();  // OK：保证复制消除
}

NonCopyable x = make();    // OK：无复制/移动发生
```

### 限制条件

C++17 的保证复制消除不适用于以下情况：

```cpp
struct C { /* ... */ };
C f();

struct D : C {
    D() : C(f()) {}    // 初始化基类子对象时不消除
    D(int) : D(g()) {} // 不消除，因为被初始化的 D 对象可能是其他类的基类子对象
};
```

此规则仅适用于已知被初始化对象不是潜在重叠子对象的情况。

---

## 5. 使用场景（Use Cases）

### 适用场景

| 场景 | 说明 |
|------|------|
| 函数返回值 | 返回局部对象或临时对象时自动应用 |
| 函数参数传递 | 传递临时对象给按值传递的参数 |
| 异常抛出与捕获 | throw 语句和 catch 处理中可能消除复制 |
| 初始化表达式 | 用同类型 prvalue 初始化对象 |
| 协程参数（C++20） | 协程参数副本的消除 |

### 最佳实践

1. **优先返回值而非输出参数**：利用 NRVO/URVO 可以高效返回对象。

```cpp
// 推荐：利用 NRVO
std::vector<int> createVector() {
    std::vector<int> result;
    // ... 填充 result
    return result;  // NRVO 应用
}

// 不推荐：使用输出参数
void createVector(std::vector<int>& out) {
    // ... 填充 out
}
```

2. **返回 prvalue 而非命名对象（C++17 起）**：保证复制消除提供更好的确定性。

```cpp
// 推荐：C++17 保证无复制
std::vector<int> createVector() {
    return std::vector<int>{1, 2, 3};
}
```

3. **避免依赖复制/移动构造函数的副作用**：编译器行为可能不一致。

### 注意事项

1. **常量表达式中的限制**：在常量表达式和常量初始化中，从不执行复制消除。

```cpp
struct A {
    void* p;
    constexpr A() : p(this) {}
    A(const A&);  // 禁用平凡可复制性
};

constexpr A a;     // OK：a.p 指向 a

constexpr A f() {
    A x;
    return x;
}

constexpr A b = f();  // 错误：b.p 会悬空，指向 f 内部的 x

constexpr A c = A();   // C++17 前：错误
                       // C++17 起：OK，c.p 指向 c，无临时对象
```

2. **调试模式差异**：某些编译器在调试模式下不执行复制消除，依赖复制/移动构造函数副作用的程序不可移植。

3. **析构函数仍需可访问**：即使返回 prvalue 时 C++17 保证不调用析构函数，析构函数也必须在 return 语句点可访问且未被删除。

### 常见陷阱

1. **函数参数不适用 NRVO**：

```cpp
// 错误示例：参数不适用 NRVO
T f(T param) {
    return param;  // 无法应用 NRVO，但可能使用移动构造
}
```

2. **条件返回影响 NRVO**：

```cpp
// NRVO 可能不应用
T f(bool condition) {
    T a, b;
    if (condition) return a;  // 多个返回对象
    return b;                   // 编译器可能无法应用 NRVO
}
```

---

## 6. 代码示例（Examples）

### 基础用法

```cpp
#include <iostream>

struct Noisy {
    Noisy() { std::cout << "constructed at " << this << '\n'; }
    Noisy(const Noisy&) { std::cout << "copy-constructed\n"; }
    Noisy(Noisy&&) { std::cout << "move-constructed\n"; }
    ~Noisy() { std::cout << "destructed at " << this << '\n'; }
};

Noisy f() {
    Noisy v = Noisy();  // C++17 前：可能消除复制
                        // C++17 起：保证复制消除
    return v;           // NRVO：v 直接构造在返回值位置
}

void g(Noisy arg) {
    std::cout << "&arg = " << &arg << '\n';
}

int main() {
    Noisy v = f();       // 可能的复制消除
    std::cout << "&v = " << &v << '\n';

    g(f());             // 参数传递中的复制消除
}
```

**可能的输出**：
```
constructed at 0x7fffd635fd4e
&v = 0x7fffd635fd4e
constructed at 0x7fffd635fd4f
&arg = 0x7fffd635fd4f
destructed at 0x7fffd635fd4f
destructed at 0x7fffd635fd4e
```

### 高级用法：不可移动类型（C++17）

```cpp
#include <iostream>

struct NonMovable {
    int value;
    NonMovable(int v) : value(v) {
        std::cout << "constructed: " << value << '\n';
    }
    NonMovable(const NonMovable&) = delete;
    NonMovable(NonMovable&&) = delete;
    ~NonMovable() {
        std::cout << "destructed: " << value << '\n';
    }
};

NonMovable make(int x) {
    return NonMovable(x);  // C++17 起：保证复制消除
}

int main() {
    NonMovable obj = make(42);  // OK：无复制/移动
    std::cout << "obj.value = " << obj.value << '\n';
}
```

**输出**：
```
constructed: 42
obj.value = 42
destructed: 42
```

### 常见错误及修正

#### 错误 1：在常量表达式中依赖复制消除

```cpp
// 错误示例
struct A {
    void* p;
    constexpr A() : p(this) {}
};

constexpr A f() {
    A x;
    return x;  // 常量表达式中不执行复制消除
}

constexpr A b = f();  // 编译错误：b.p 会悬空
```

**修正**：使用 prvalue（C++17 起）：

```cpp
constexpr A f() {
    return A();  // C++17：保证无临时对象
}

constexpr A b = f();  // OK：b.p 指向 b
```

#### 错误 2：依赖复制构造函数的副作用

```cpp
// 不推荐：依赖副作用的代码不可移植
struct Counter {
    static int copy_count;
    Counter() {}
    Counter(const Counter&) { ++copy_count; }
};
int Counter::copy_count = 0;

Counter f() {
    Counter c;
    return c;  // NRVO 可能不应用（如调试模式）
}

int main() {
    Counter c = f();
    std::cout << Counter::copy_count << '\n';  // 结果可能为 0 或 1
}
```

**修正**：不要依赖复制/移动构造函数的副作用：

```cpp
// 推荐：显式表达意图
std::unique_ptr<Counter> f() {
    return std::make_unique<Counter>();
}
```

---

## 7. 总结（Summary）

### 核心要点

1. **复制消除是编译器优化**，可省略同类型对象之间的复制/移动操作，即使构造函数/析构函数有副作用。

2. **NRVO** 是对命名局部对象返回的优化，仍是可选优化。

3. **C++17 保证复制消除** 重构了 prvalue 语义，使 URVO 成为强制行为，某些场景下甚至不需要可访问的复制/移动构造函数。

4. **常量表达式中不执行复制消除**，这是重要的限制。

5. **不应依赖复制/移动构造函数的副作用**，因为优化行为可能因编译器和编译选项而异。

### 技术对比

| 特性 | C++11 前 | C++11 | C++17 | C++20 |
|------|----------|-------|-------|-------|
| NRVO | 可选优化 | 可选优化 | 可选优化 | 可选优化 |
| URVO | 可选优化 | 可选优化 | **强制行为** | 强制行为 |
| 异常处理消除 | 无 | 支持 | 支持 | 支持 |
| 协程参数消除 | 无 | 无 | 无 | 支持 |
| 返回时尝试移动 | 无 | 支持 | 支持 | 支持（左值也视为右值） |

### 学习建议

1. **理解值类别**：深入学习 glvalue、prvalue、xvalue 的概念，特别是 C++17 的 prvalue 语义变化。

2. **编写优化友好的代码**：优先使用单返回语句、返回 prvalue，让编译器有更多优化空间。

3. **避免依赖副作用**：不要在复制/移动构造函数中编写影响程序逻辑的代码。

4. **使用特性测试宏**：使用 `__cpp_guaranteed_copy_elision` 检测编译器支持。

### 相关主题

- [复制初始化（Copy Initialization）](./copy_initialization.md)
- [复制构造函数（Copy Constructor）](./copy_constructor.md)
- [移动构造函数（Move Constructor）](./move_constructor.md)
- [值类别（Value Categories）](../value_category.md)

---

**参考资料**：
- [cppreference: Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)
- C++17 标准 [class.cdtor] 章节