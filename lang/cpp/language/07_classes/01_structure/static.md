# static 成员 (static members)

## 1. 概述 (Overview)

在类定义内部，`static` 关键字用于声明不与类实例绑定的成员。静态成员（static members）包括静态数据成员和静态成员函数，它们属于类本身而非类的某个对象实例。

**核心特性**：
- **静态数据成员**：独立于任何对象存在，整个程序只有一个实例
- **静态成员函数**：不与任何对象关联，没有 `this` 指针
- **存储期**：具有静态存储期（static storage duration）或线程存储期（thread storage duration，C++11 起）

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`static` 成员的概念源于 C++ 对类的封装和资源共享的需求。在没有静态成员的情况下：
- 共享数据需要使用全局变量，破坏了封装性
- 类级别的操作需要通过额外机制实现

静态成员的引入解决了这些问题：
- 提供类级别的数据共享，保持封装性
- 实现类级别的操作（如工厂方法、单例模式）

### C++98

- 引入静态成员的基本语法
- 静态数据成员需要在类外定义
- const 静态整型成员可以在类内初始化

### C++11 变化

- 支持 `thread_local` 修饰静态数据成员，实现线程局部存储
- 支持 `constexpr` 静态数据成员，必须在类内初始化

### C++17 变化

- 允许 `inline` 静态数据成员，可在类内定义并初始化
- `constexpr` 静态数据成员隐式为 `inline`，无需类外定义
- 非 inline 的 const 静态成员仍需类外定义（但已废弃）

### 版本对比

| 特性 | C++98 | C++11 | C++17 |
|------|-------|-------|-------|
| 类外定义 | 必需 | 必需 | 可选（inline） |
| const 整型类内初始化 | 支持 | 支持 | 支持 |
| constexpr 类内初始化 | N/A | 支持 | 支持（隐式 inline） |
| inline 静态数据成员 | N/A | N/A | 支持 |
| thread_local 支持 | N/A | 支持 | 支持 |

## 3. 语法与参数 (Syntax and Parameters)

### 静态成员声明语法

```cpp
class ClassName {
public:
    // 静态数据成员声明
    static Type static_data_member;

    // 静态成员函数声明
    static ReturnType static_member_function(Parameters);

private:
    static Type private_static_member;
};
```

**语法要点**：
- `static` 关键字通常出现在其他说明符之前
- 可以出现在说明符序列的任意位置
- 静态成员名称不能与包含类同名

### 静态数据成员定义

```cpp
// 类外定义（C++98/C++11 风格）
Type ClassName::static_data_member = initial_value;

// C++17 inline 定义（类内定义）
class ClassName {
    inline static Type inline_member = initial_value;  // C++17
};
```

### 访问静态成员

```cpp
// 方式一：限定名
ClassName::static_member;

// 方式二：成员访问表达式
ClassName obj;
obj.static_member;      // 通过对象访问
obj_ptr->static_member; // 通过指针访问
```

### 常量静态成员

```cpp
struct X {
    // const 整型/枚举类型
    const static int n = 1;           // 类内初始化
    const static int m{2};            // C++11 列表初始化
    const static int k;               // 仅声明

    // constexpr 静态成员（C++11）
    constexpr static int arr[] = {1, 2, 3};
    constexpr static double value = 3.14;
};
const int X::k = 3;  // 类外定义
```

### 参数说明

| 说明符 | 适用类型 | 说明 |
|--------|----------|------|
| `static` | 数据成员/成员函数 | 声明为静态成员 |
| `const` | 数据成员 | 常量，整型/枚举可在类内初始化 |
| `constexpr` | 数据成员 | 常量表达式，必须在类内初始化（C++11） |
| `inline` | 数据成员 | 允许类内定义（C++17） |
| `thread_local` | 数据成员 | 线程局部存储（C++11） |

### 静态成员函数限制

```cpp
class X {
    static void f();  // 正确

    // 以下声明是非法的：
    // static virtual void g();      // 错误：不能是 virtual
    // static void h() const;        // 错误：不能有 cv-限定
    // static void i() volatile;     // 错误：不能有 cv-限定
    // static void j() &;            // 错误：不能有引用限定符
};
```

## 4. 底层原理 (Underlying Principles)

### 内存布局

静态数据成员存储在全局/静态数据区，与对象实例完全分离：

```
+------------------+
| 全局/静态数据区  |
|------------------|
| ClassName::static_member |  <- 静态数据成员
+------------------+

+------------------+
| 栈/堆            |
|------------------|
| obj1 (非静态数据)|  <- 对象实例 1
| obj2 (非静态数据)|  <- 对象实例 2
+------------------+
```

### 存储期与生命周期

| 类型 | 存储期 | 初始化时机 | 销毁时机 |
|------|--------|-----------|----------|
| 静态数据成员 | 静态存储期 | 程序启动时 | 程序结束时 |
| thread_local 静态成员 | 线程存储期 | 线程启动时 | 线程结束时 |

### 静态成员函数特性

```cpp
class Example {
    static void static_func() {
        // 没有 this 指针
        // 只能访问静态成员
        // 不能是 virtual（无 vtable 参与）
    }
};
```

**实现原理**：
- 静态成员函数本质上是普通函数，只是作用域被限制在类内
- 函数指针类型为 `ReturnType (*)(Parameters)`，而非成员函数指针

### ODR 使用规则

**ODR-use (One Definition Rule use)**：当静态成员被取地址、绑定到引用或用于需要其地址的上下文时，需要类外定义。

```cpp
struct X {
    static const int n = 1;      // 仅值使用，无需类外定义
    static constexpr int m = 4;  // C++17 起隐式 inline
};

const int* p = &X::n;  // ODR-use，需要类外定义（C++17 前）
const int* q = &X::m;  // C++17 起无需类外定义
```

### 链接属性

| 场景 | 链接属性 |
|------|----------|
| 命名空间作用域中的类，静态成员 | 外部链接 |
| 无名命名空间中的类，静态成员 | 内部链接 |
| 局部类（函数内定义） | 不能有静态数据成员 |
| 无名类 | 不能有静态数据成员 |

## 5. 使用场景 (Use Cases)

### 适合使用静态成员的场景

| 场景 | 说明 |
|------|------|
| 类级别的计数器 | 统计创建的对象数量 |
| 单例模式 | 实现全局唯一的实例 |
| 工厂方法 | 创建对象的静态方法 |
| 常量定义 | 类相关的常量 |
| 工具函数 | 不依赖对象状态的操作 |
| 资源共享 | 多对象共享同一资源 |

### 最佳实践

1. **优先使用 inline 静态成员（C++17）**：避免类外定义的复杂性
2. **静态常量优先使用 constexpr**：编译时计算，隐式 inline
3. **避免静态成员初始化顺序依赖**：使用函数内静态变量（Meyers' Singleton）
4. **线程安全考虑**：多线程环境下注意静态成员的同步

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 初始化顺序问题 | 静态成员初始化顺序不确定 | 使用函数返回静态局部变量 |
| 忘记类外定义 | ODR-use 时链接错误 | 使用 inline（C++17）或添加类外定义 |
| const 成员取地址 | 值使用不需要定义，取地址需要 | C++17 使用 constexpr 或添加定义 |
| 多线程竞争 | 静态成员被多线程访问 | 使用同步机制 |

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

class Counter {
public:
    Counter() { ++count_; }
    ~Counter() { --count_; }

    // 静态成员函数
    static int get_count() { return count_; }

private:
    // 静态数据成员
    static int count_;
};

// 类外定义
int Counter::count_ = 0;

int main() {
    std::cout << Counter::get_count() << std::endl;  // 0

    Counter c1, c2;
    std::cout << Counter::get_count() << std::endl;  // 2

    {
        Counter c3;
        std::cout << Counter::get_count() << std::endl;  // 3
    }

    std::cout << Counter::get_count() << std::endl;  // 2

    return 0;
}
```

### C++17 inline 静态数据成员

```cpp
#include <iostream>
#include <string>

class ModernStyle {
public:
    // C++17: inline 静态数据成员，无需类外定义
    inline static int counter = 0;
    inline static const std::string class_name = "ModernStyle";

    // constexpr 静态成员隐式 inline
    constexpr static int max_size = 100;

    void increment() { ++counter; }
};

int main() {
    ModernStyle obj1, obj2;

    obj1.increment();
    obj2.increment();

    std::cout << ModernStyle::counter << std::endl;      // 2
    std::cout << ModernStyle::class_name << std::endl;   // ModernStyle
    std::cout << ModernStyle::max_size << std::endl;      // 100

    return 0;
}
```

### 单例模式（静态成员实现）

```cpp
#include <iostream>

class Singleton {
public:
    // 删除拷贝和移动
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 静态访问方法
    static Singleton& instance() {
        static Singleton inst;  // Meyers' Singleton
        return inst;
    }

    void do_something() {
        std::cout << "Singleton working..." << std::endl;
    }

private:
    Singleton() = default;  // 私有构造函数
};

int main() {
    Singleton::instance().do_something();

    // Singleton s;  // 错误：构造函数私有
    // Singleton s = Singleton::instance();  // 错误：拷贝构造已删除

    return 0;
}
```

### 常量静态成员

```cpp
#include <iostream>

class Config {
public:
    // const 整型，类内初始化
    const static int VERSION = 1;
    const static int MAX_CONNECTIONS = 100;

    // constexpr 静态成员
    constexpr static double PI = 3.14159265358979;
    constexpr static int BUFFER_SIZE = 1024;

    // 复杂类型的 const 静态成员（C++17 inline）
    inline static const char* APP_NAME = "MyApp";
};

int main() {
    std::cout << Config::VERSION << std::endl;         // 1
    std::cout << Config::MAX_CONNECTIONS << std::endl; // 100
    std::cout << Config::PI << std::endl;              // 3.14159...
    std::cout << Config::BUFFER_SIZE << std::endl;     // 1024
    std::cout << Config::APP_NAME << std::endl;        // MyApp

    return 0;
}
```

### 线程局部静态成员（C++11）

```cpp
#include <iostream>
#include <thread>

class ThreadLocalExample {
public:
    // 每个线程有独立的副本
    thread_local static int thread_counter;

    static void increment() {
        ++thread_counter;
        std::cout << "Thread " << std::this_thread::get_id()
                  << " counter: " << thread_counter << std::endl;
    }
};

// 类外定义
thread_local int ThreadLocalExample::thread_counter = 0;

int main() {
    std::thread t1([]() {
        ThreadLocalExample::increment();  // 1
        ThreadLocalExample::increment();  // 2
    });

    std::thread t2([]() {
        ThreadLocalExample::increment();  // 1
        ThreadLocalExample::increment();  // 2
        ThreadLocalExample::increment();  // 3
    });

    t1.join();
    t2.join();

    return 0;
}
```

### 常见错误及修正

#### 错误 1：忘记类外定义（C++17 前）

```cpp
// 错误：ODR-use 但无类外定义
struct X {
    static int value;
};

int main() {
    int* p = &X::value;  // 链接错误：未定义 X::value
    return 0;
}

// 修正：添加类外定义
struct X {
    static int value;
};
int X::value = 0;  // 类外定义

// 更好的修正（C++17）：使用 inline
struct X {
    inline static int value = 0;  // 无需类外定义
};
```

#### 错误 2：静态成员函数访问非静态成员

```cpp
class X {
    int non_static = 42;

    static void static_func() {
        // 错误：静态成员函数不能访问非静态成员
        // std::cout << non_static;  // 编译错误

        // 正确：只能访问静态成员
        std::cout << static_value;
    }

    static int static_value;
};
int X::static_value = 100;
```

#### 错误 3：const 静态成员取地址无定义

```cpp
struct X {
    const static int n = 1;
};

// 值使用：OK，不需要类外定义
int a = X::n;

// ODR-use（C++17 前）：需要类外定义
const int* p = &X::n;  // C++17 前链接错误

// 修正：添加类外定义
const int X::n;  // 不能有初始化器

// 或使用 constexpr（C++17 起隐式 inline）
struct Y {
    constexpr static int n = 1;
};
const int* q = &Y::n;  // OK
```

## 7. 总结 (Summary)

### 核心要点

1. **静态成员属于类**：不与任何对象实例关联
2. **静态数据成员**：全局唯一实例，静态存储期
3. **静态成员函数**：无 `this` 指针，只能访问静态成员
4. **类外定义规则**：C++17 前需要，inline/conexpr 可省略

### 版本演进建议

| C++ 版本 | 推荐写法 |
|----------|----------|
| C++98 | 类内声明 + 类外定义 |
| C++11 | 使用 constexpr，支持 thread_local |
| C++17 | 优先使用 inline static，constexpr 隐式 inline |

### 相关概念

| 概念 | 关系 |
|------|------|
| `static` 存储说明符 | 类外使用，指定静态存储期和内部链接 |
| 单例模式 | 常用静态成员实现 |
| 工厂模式 | 常用静态成员函数创建对象 |
| 命名空间成员 | 静态成员的替代方案之一 |

### 参考资料

- C++23 标准 (ISO/IEC 14882:2024): 11.4.9 Static members [class.static]
- C++20 标准 (ISO/IEC 14882:2020): 11.4.8 Static members [class.static]
- C++17 标准 (ISO/IEC 14882:2017): 12.2.3 Static members [class.static]
- cppreference: https://en.cppreference.com/w/cpp/language/static