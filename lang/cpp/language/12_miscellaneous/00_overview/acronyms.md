# C++ 缩写词汇表

## 1. 概述 (Overview)

本文档收录了 C++ 社区中广泛使用的缩写词汇（Acronyms），涵盖语言特性、编译器技术、优化策略、标准库概念以及标准化组织工作组等方面的术语。这些缩写在 C++ 文档、论文、邮件列表和日常讨论中频繁出现，理解它们对于深入学习 C++ 和参与社区交流至关重要。

### 1.1 文档目的

- 提供 C++ 领域常见缩写的标准化解释
- 帮助开发者快速理解技术文献中的专业术语
- 建立统一的术语认知框架

### 1.2 适用范围

本文档适用于以下人群：
- C++ 开发者（初级到高级）
- 编译器实现者
- 标准化参与者
- 技术文档撰写者

---

## 2. 来源与演变 (Origin and Evolution)

### 2.1 缩写的产生背景

C++ 语言自 1983 年诞生以来，经过数十年发展，形成了庞大而复杂的生态系统。随着新特性的不断引入和社区讨论的深入，大量缩写应运而生：

- **标准化进程产物**：如 CWG、LWG、EWG 等工作组缩写，源于 ISO C++ 标准化委员会的组织架构
- **编程惯用语**：如 RAII、CRTP、PIMPL 等，源于最佳实践和设计模式的总结
- **编译器技术**：如 RVO、NRVO、LTO 等优化技术的简称
- **社区文化**：如 SFINAE、AAA 等源自社区讨论和最佳实践推广

### 2.2 标准版本影响

| C++ 版本 | 新增重要缩写 | 说明 |
|---------|-------------|------|
| C++11 | `nullptr`, `auto`, `decltype` 相关术语 | 现代C++开端，引入大量新概念 |
| C++17 | CTAD, IFNDR | 类模板参数推导、无诊断要求 |
| C++20 | CPO, RACO, RAO, SIOF | Ranges库、模块相关术语 |
| C++23 | HALO, Deducing this 相关 | 协程优化、显式对象参数 |

### 2.3 外部参考

本文档基于 Arthur O'Dwyer 2019 年发表的"A C++ acronym glossary"以及 cppreference 官方文档整理而成。

---

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 语言核心概念类

| 缩写 | 全称 | 中文含义 | 相关参考 |
|------|------|---------|----------|
| ADL | Argument-Dependent Lookup | 参数依赖查找 | 又称 Koenig 查找 |
| CTAD | Class Template Argument Deduction | 类模板参数推导 | C++17 引入 |
| NTTP | Non-Type Template Parameter | 非类型模板参数 | C++20 扩展支持类类型 |
| ODR | One Definition Rule | 单一定义规则 | 链接和编译核心规则 |
| SFINAE | Substitution Failure Is Not An Error | 替换失败并非错误 | 模板元编程基础 |
| UB | Undefined Behavior | 未定义行为 | 程序正确性关键概念 |
| IFNDR | Ill-Formed, No Diagnostic Required | 非良构但无需诊断 | C++20 正式术语 |
| NTBS | Null-Terminated Byte Strings | 空终止字节字符串 | C 风格字符串 |

### 3.2 设计模式与惯用语类

| 缩写 | 全称 | 中文含义 | 典型应用 |
|------|------|---------|----------|
| RAII | Resource Acquisition Is Initialization | 资源获取即初始化 | 智能指针、锁管理 |
| SBRM | Scope-Bound Resource Management | 作用域绑定资源管理 | RAII 的别名 |
| CRTP | Curiously Recurring Template Pattern | 奇异递归模板模式 | `std::enable_shared_from_this` |
| PIMPL | Pointer to IMPLementation | 实现指针惯用法 | 编译防火墙、ABI 稳定 |
| NVI | Non-Virtual Interface | 非虚接口模式 | 模板方法模式变体 |
| ABC | Abstract Base Class | 抽象基类 | 接口设计 |
| AAA | Almost Always Auto | 几乎总是用 auto | Herb Sutter 提倡的风格 |
| SCARY | 见下文详解 | SCARY 初始化 | 标准库容器迭代器设计 |

**SCARY 含义详解**：
- **S**eemingly erroneous (appearing **C**onstrained by conflicting generic parameters)
- but **A**ctually work with the **R**ight implementation
- (unconstrained b**Y** the conflict due to minimized dependencies)

指看似错误（因泛型参数冲突而受限），但实际上能正确工作的设计，因为正确的实现不受冲突约束（得益于最小化依赖）。

### 3.3 优化技术类

| 缩写 | 全称 | 中文含义 | 说明 |
|------|------|---------|------|
| RVO | Return Value Optimization | 返回值优化 | 避免拷贝构造 |
| NRVO | Named Return Value Optimization | 命名返回值优化 | 具名对象的返回优化 |
| EBO/EBCO | Empty Base (Class) Optimization | 空基类优化 | 减少空类占用空间 |
| NUA | No Unique Address | 无唯一地址 | `[[no_unique_address]]` 属性 |
| SSO | Small String Optimization | 小字符串优化 | `std::string` 短字符串优化 |
| SOO | Small Object Optimization | 小对象优化 | `std::function`, `std::any` |
| SBO | Small Buffer Optimization | 小缓冲区优化 | 类似 SOO |
| LTO | Link-Time Optimization | 链接时优化 | 跨编译单元优化 |
| PGO | Profile-Guided Optimization | 配置文件引导优化 | 基于运行数据的优化 |
| PDO | Profile-Driven Optimization | 配置驱动优化 | PGO 的别名 |
| WPO | Whole-Program Optimization | 全程序优化 | 程序级优化 |
| IPO | Inter-Procedural Optimization | 过程间优化 | 函数间优化 |
| PCH | Pre-Compiled Header | 预编译头 | 加速编译 |
| TCO | Tail Call Optimization | 尾调用优化 | 递归优化 |
| HALO | Heap Allocation eLision Optimization | 堆分配省略优化 | 协程优化，P0981 |
| DCL | Double-Checked Locking | 双重检查锁定 | 单例模式，需谨慎使用 |

### 3.4 并发与内存管理类

| 缩写 | 全称 | 中文含义 | 相关参考 |
|------|------|---------|----------|
| CAS | Compare-And-Swap; Copy And Swap | 比较并交换；复制并交换 | 原子操作 |
| TLS | Thread-Local Storage | 线程本地存储 | `thread_local` 关键字 |
| MPSC | Multi-Producer Single-Consumer | 多生产者单消费者 | 任务队列模式 |
| RCU | Read-Copy-Update | 读-复制-更新 | `<rcu>` 库 |
| COW | Copy-On-Write | 写时复制 | 字符串、容器优化 |

### 3.5 模块与编译相关类

| 缩写 | 全称 | 中文含义 | 说明 |
|------|------|---------|------|
| TU | Translation Unit | 翻译单元 | 编译的基本单位 |
| BMI | Binary Module Interface | 二进制模块接口 | C++20 模块 |
| CMI | Compiled Module Interfaces | 已编译模块接口 | 模块系统 |
| GMF | Global Module Fragment | 全局模块片段 | 模块声明区域 |
| PMF | Private Module Fragment | 私有模块片段 | 模块实现细节 |
| ABI | Application Binary Interface | 应用程序二进制接口 | 兼容性关键 |

### 3.6 类型与数据类

| 缩写 | 全称 | 中文含义 | 说明 |
|------|------|---------|------|
| POD | Plain Old Data | 纯旧数据 | 标量类型、平凡类型 |
| ADT | Abstract Data Type | 抽象数据类型 | 数据结构理论 |
| RTTI | RunTime Type Identification | 运行时类型识别 | `typeid`, `dynamic_cast` |
| MDT | Most Derived Type | 最派生类型 | 对象实际类型 |
| VLA | Variable-Length Array | 变长数组 | C99 特性，C++ 不支持 |
| FAM | Flexible Array Member | 柔性数组成员 | C 结构体尾部长度 |

### 3.7 编译器与工具类

| 缩写 | 全称 | 中文含义 | 说明 |
|------|------|---------|------|
| GCC | GNU Compiler Collection | GNU 编译器集合 | 开源编译器套件 |
| MSVC | MicroSoft Visual C++ | 微软 Visual C++ | Windows 编译器 |
| EDG | Edison Design Group | Edison 设计组 | 编译器前端供应商 |
| IWYU | Include What You Use | 包含你使用的 | 头文件管理工具 |

### 3.8 标准化组织类

| 缩写 | 全称 | 中文含义 | 职责 |
|------|------|---------|------|
| WG21 | ISO C++ Standards Committee | C++ 标准委员会 | 国际标准化组织 |
| CWG | Core Working Group | 核心工作组 | 语言核心议题 |
| LWG | Library Working Group | 库工作组 | 标准库议题 |
| EWG | Evolution Working Group | 演进工作组 | 新特性提案 |
| EWGI | Evolution Working Group Incubator | 演进工作组孵化器 | 提案初步评审 |
| LEWG | Library Evolution Working Group | 库演进工作组 | 库新特性提案 |
| LEWGI | Library Evolution Working Group Incubator | 库演进工作组孵化器 | 库提案初步评审 |

### 3.9 标准文档类

| 缩写 | 全称 | 中文含义 | 说明 |
|------|------|---------|------|
| DIS | Draft International Standard | 国际标准草案 | 标准化阶段 |
| FDIS | Final Draft International Standard | 最终国际标准草案 | 发布前最后阶段 |
| DR | Defect Report | 缺陷报告 | 标准缺陷修正 |
| NAD | Not A Defect | 非缺陷 | 关闭的 DR |

### 3.10 库与框架类

| 缩写 | 全称 | 中文含义 | 相关参考 |
|------|------|---------|----------|
| STL | Standard Template Library | 标准模板库 | C++ 标准库前身 |
| CPO | Customization Point Object | 定制点对象 | Ranges 库核心概念 |
| RAO | Range Adaptor Object | 范围适配器对象 | 视图组合 |
| RACO | Range Adaptor Closure Object | 范围适配器闭包对象 | 管道操作符支持 |
| AFO | Algorithm Function Objects | 算法函数对象 | 类似 CPO |
| PMR | Polymorphic Memory Resources | 多态内存资源 | `<memory_resource>` |
| SIMD | Single Instruction Multiple Data | 单指令多数据 | 数据并行类型 |

### 3.11 其他常用缩写

| 缩写 | 全称 | 中文含义 | 说明 |
|------|------|---------|------|
| API | Application Programming Interface | 应用程序编程接口 | 接口规范 |
| ELF | Executable and Linkable Format | 可执行可链接格式 | Unix/Linux 目标文件格式 |
| EH | Exception Handling | 异常处理 | 语言特性 |
| ICE | Internal Compiler Error; Integer Constant Expression | 编译器内部错误；整型常量表达式 | 双重含义 |
| NDR | No Diagnostic Required | 无需诊断 | 编译器行为 |
| NSDMI | Non-Static Data Member Initialization | 非静态数据成员初始化 | 类内初始化 |
| UDC | User-Defined Conversion operator | 用户定义转换运算符 | 类型转换 |
| UDL | User-Defined Literals | 用户定义字面量 | C++11 特性 |
| UFCS | Universal Function Call Syntax | 通用函数调用语法 | D 语言概念 |
| TMP | Template Meta Programming | 模板元编程 | 编译期计算 |
| CTRE | Compile-Time Regular Expressions | 编译期正则表达式 | CTRE 库 |
| QoI | Quality of Implementation | 实现质量 | 编译器优化程度 |
| PID | Process IDentifier | 进程标识符 | `std::thread::get_id()` |
| SEH | Structured Exception Handling | 结构化异常处理 | Windows 机制 |
| OOP | Object-Oriented Programming | 面向对象编程 | 编程范式 |
| IIILE | Immediately Invoked Initializing Lambda Expression | 立即调用初始化 Lambda 表达式 | IIFE 的 C++ 变体 |
| SOCCC | Select On Container Copy Construction | 容器复制构造时选择 | 分配器传播 |
| POCCA | Propagate on Container Copy Assignment | 容器复制赋值时传播 | 分配器传播 |
| POCMA | Propagate on Container Move Assignment | 容器移动赋值时传播 | 分配器传播 |
| POCS | Propagate on Container Swap | 容器交换时传播 | 分配器传播 |
| SMF | Special Member Function | 特殊成员函数 | 六大特殊成员函数 |

---

## 4. 底层原理 (Underlying Principles)

本文档为词汇表性质，不涉及具体技术实现原理。以下补充部分关键概念的底层机制说明。

### 4.1 RAII 原理

RAII（Resource Acquisition Is Initialization）是 C++ 资源管理的核心思想：

```
资源生命周期 = 对象生命周期
```

- **构造时获取**：对象构造时申请资源
- **析构时释放**：对象析构时释放资源（通过析构函数自动调用）
- **异常安全**：栈展开时自动调用析构函数，保证资源释放

### 4.2 SFINAE 原理

SFINAE（Substitution Failure Is Not An Error）是模板元编程的基础：

- 模板参数替换时，如果替换失败，不视为编译错误
- 编译器继续尝试其他重载
- 常用于条件编译和类型特征检测
- C++20 的 Concepts 提供了更清晰的替代方案

### 4.3 RVO/NRVO 原理

返回值优化的实现机制：

1. **RVO**：编译器直接在调用者栈帧构造返回值对象，避免拷贝/移动
2. **NRVO**：对于命名对象，编译器优化返回路径，将局部对象直接构造在调用者栈帧
3. **C++17 保证**：对于 prvalue 情况，拷贝省略（copy elision）是强制的

### 4.4 ADL 查找规则

参数依赖查找的查找范围：

1. 在参数类型的命名空间中查找函数
2. 在参数类型的关联类中查找函数
3. 忽略通常的名字查找限制

---

## 5. 使用场景 (Use Cases)

### 5.1 适用场景

1. **阅读技术文档**：快速理解 C++ 文档中的缩写术语
2. **参与社区讨论**：准确使用和理解专业术语
3. **撰写技术文档**：正确使用标准缩写
4. **编译器相关开发**：理解编译器输出和优化术语
5. **参与标准化讨论**：理解工作组缩写和文档术语

### 5.2 最佳实践

| 场景 | 建议 |
|------|------|
| 技术写作 | 首次出现缩写时给出全称 |
| 代码注释 | 对不常见缩写添加解释 |
| 代码评审 | 确保团队成员理解关键缩写 |
| 学习进阶 | 按类别逐步掌握相关缩写 |

### 5.3 常见陷阱

1. **缩写歧义**：部分缩写有多种含义（如 ICE），需根据上下文判断
2. **过时概念**：部分缩写对应的概念已过时（如 POD 在 C++11 后被细分）
3. **平台差异**：部分缩写为特定平台概念（如 SEH 为 Windows 特有）

---

## 6. 代码示例 (Examples)

### 6.1 RAII 典型用法

```cpp
#include <memory>
#include <fstream>
#include <mutex>

// 智能指针 - 自动管理内存
void example_unique_ptr() {
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    // 函数结束时自动释放内存
}

// 文件流 - 自动管理文件句柄
void example_file() {
    std::ofstream file("example.txt");
    file << "Hello, RAII!";
    // 函数结束时自动关闭文件
}

// 锁守卫 - 自动管理互斥锁
std::mutex mtx;
void example_lock() {
    std::lock_guard<std::mutex> lock(mtx);
    // 临界区代码
    // 函数结束时自动解锁
}
```

### 6.2 CRTP 模式示例

```cpp
// CRTP: 奇异递归模板模式
template <typename Derived>
class Printable {
public:
    void print() const {
        static_cast<const Derived*>(this)->print_impl();
    }
};

class Document : public Printable<Document> {
public:
    void print_impl() const {
        std::cout << "Document content" << std::endl;
    }
};

// std::enable_shared_from_this 使用 CRTP
class Node : public std::enable_shared_from_this<Node> {
public:
    std::shared_ptr<Node> get_self() {
        return shared_from_this();
    }
};
```

### 6.3 PIMPL 惯用法

```cpp
// Widget.h - 公共头文件
class Widget {
public:
    Widget();
    ~Widget();
    void doSomething();
private:
    class Impl;  // 前向声明
    std::unique_ptr<Impl> pImpl;
};

// Widget.cpp - 实现文件
class Widget::Impl {
public:
    void doSomething() {
        // 实际实现
    }
private:
    // 私有数据成员隐藏在实现中
};

Widget::Widget() : pImpl(std::make_unique<Impl>()) {}
Widget::~Widget() = default;  // 析构函数必须在 Impl 完整类型处定义

void Widget::doSomething() {
    pImpl->doSomething();
}
```

### 6.4 SFINAE 与 Concepts 对比

```cpp
#include <type_traits>
#include <concepts>

// SFINAE 方式 (C++11/14/17)
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
add_one(T value) {
    return value + 1;
}

// Concepts 方式 (C++20) - 更清晰
template<std::integral T>
T add_one_modern(T value) {
    return value + 1;
}

// 或使用 requires
template<typename T>
    requires std::integral<T>
T add_one_requires(T value) {
    return value + 1;
}
```

### 6.5 常见错误示例

```cpp
// 错误: 双重检查锁定在 C++11 之前的实现不安全
// DCLP (Double-Checked Locking Pattern) 的危险
class Singleton {
    static Singleton* instance;
    static std::mutex mtx;
public:
    static Singleton* getInstance_wrong() {
        if (!instance) {              // 第一次检查（无锁）
            std::lock_guard<std::mutex> lock(mtx);
            if (!instance) {          // 第二次检查（有锁）
                instance = new Singleton();  // 危险！可能发生重排序
            }
        }
        return instance;
    }

    // 正确: C++11 使用局部静态变量
    static Singleton& getInstance_correct() {
        static Singleton instance;  // C++11 保证线程安全
        return instance;
    }
};

// 错误: 返回局部变量的指针导致 UB
int* dangerous_return() {
    int local = 42;
    return &local;  // UB: 返回局部变量的地址
}

// 正确: 返回值优化
int safe_return() {
    int local = 42;
    return local;  // RVO/NRVO 会优化掉拷贝
}
```

### 6.6 CTAD 示例

```cpp
#include <vector>
#include <memory>
#include <optional>

// C++17 类模板参数推导
void example_ctad() {
    // 以前需要写 std::vector<int>
    std::vector v{1, 2, 3, 4, 5};  // 推导为 std::vector<int>

    // 智能指针
    std::shared_ptr sp = std::make_shared<int>(42);  // 推导为 std::shared_ptr<int>

    // 可选值
    std::optional opt = 100;  // 推导为 std::optional<int>

    // 自定义推导指引
    template<typename T>
    struct Container {
        Container(T value) : data(value) {}
        T data;
    };

    // 推导指引
    template<typename T>
    Container(T) -> Container<T>;

    Container c(3.14);  // 推导为 Container<double>
}
```

---

## 7. 总结 (Summary)

### 7.1 核心要点

1. **分类记忆**：将缩写按类别（语言核心、设计模式、优化技术、标准化组织等）进行分组记忆
2. **上下文理解**：部分缩写有多重含义（如 ICE），需根据上下文判断
3. **版本相关**：关注缩写对应的 C++ 版本，部分概念在新版本中有更新
4. **实践结合**：结合代码实践理解缩写背后的设计思想

### 7.2 学习建议

| 学习阶段 | 建议掌握的缩写 |
|---------|---------------|
| 初级 | RAII, STL, API, OOP, UB, ODR |
| 中级 | SFINAE, CRTP, PIMPL, RVO, NRVO, ADL, CTAD |
| 高级 | CPO, IFNDR, HALO, SIOF, SCARY, PMR |

### 7.3 参考资源

1. cppreference 官方文档
2. Arthur O'Dwyer "A C++ acronym glossary" (2019)
3. ISO C++ 标准文档
4. WG21 提案文档

---

## 附录：完整缩写速查表

按字母顺序排列的完整缩写列表：

| 缩写 | 全称 | 类别 |
|------|------|------|
| AAA | Almost Always Auto | 风格 |
| ABC | Abstract Base Class | 设计 |
| ABI | Application Binary Interface | 编译 |
| ADL | Argument-Dependent Lookup | 语言 |
| ADT | Abstract Data Type | 理论 |
| AFO | Algorithm Function Objects | 库 |
| API | Application Programming Interface | 通用 |
| BMI | Binary Module Interface | 模块 |
| CAS | Compare-And-Swap | 并发 |
| CMI | Compiled Module Interfaces | 模块 |
| COW | Copy-On-Write | 优化 |
| CPO | Customization Point Object | 库 |
| CRTP | Curiously Recurring Template Pattern | 模式 |
| CTAD | Class Template Argument Deduction | 语言 |
| CTRE | Compile-Time Regular Expressions | 库 |
| CWG | Core Working Group | 组织 |
| DCL | Double-Checked Locking | 并发 |
| DIS | Draft International Standard | 标准 |
| DR | Defect Report | 标准 |
| EBO | Empty Base Optimization | 优化 |
| EDG | Edison Design Group | 编译器 |
| EH | Exception Handling | 语言 |
| ELF | Executable and Linkable Format | 格式 |
| EWG | Evolution Working Group | 组织 |
| EWGI | Evolution Working Group Incubator | 组织 |
| FAM | Flexible Array Member | 语言 |
| FDIS | Final Draft International Standard | 标准 |
| GCC | GNU Compiler Collection | 编译器 |
| GMF | Global Module Fragment | 模块 |
| HALO | Heap Allocation eLision Optimization | 优化 |
| ICE | Internal Compiler Error / Integer Constant Expression | 编译/语言 |
| IFNDR | Ill-Formed, No Diagnostic Required | 语言 |
| IIILE | Immediately Invoked Initializing Lambda Expression | 模式 |
| IPO | Inter-Procedural Optimization | 优化 |
| IWYU | Include What You Use | 工具 |
| LEWG | Library Evolution Working Group | 组织 |
| LEWGI | Library Evolution Working Group Incubator | 组织 |
| LTO | Link-Time Optimization | 优化 |
| LWG | Library Working Group | 组织 |
| MDT | Most Derived Type | 语言 |
| MPSC | Multi-Producer Single-Consumer | 并发 |
| MSVC | MicroSoft Visual C++ | 编译器 |
| NAD | Not A Defect | 标准 |
| NDR | No Diagnostic Required | 语言 |
| NRVO | Named Return Value Optimization | 优化 |
| NSDMI | Non-Static Data Member Initialization | 语言 |
| NTBS | Null-Terminated Byte Strings | 语言 |
| NTTP | Non-Type Template Parameter | 语言 |
| NUA | No Unique Address | 语言 |
| NVI | Non-Virtual Interface | 模式 |
| ODR | One Definition Rule | 语言 |
| OOP | Object-Oriented Programming | 范式 |
| PCH | Pre-Compiled Header | 编译 |
| PDO | Profile-Driven Optimization | 优化 |
| PGO | Profile-Guided Optimization | 优化 |
| PID | Process IDentifier | 系统 |
| PIMPL | Pointer to IMPLementation | 模式 |
| PMF | Private Module Fragment | 模块 |
| PMR | Polymorphic Memory Resources | 库 |
| POCCA | Propagate on Container Copy Assignment | 库 |
| POCMA | Propagate on Container Move Assignment | 库 |
| POCS | Propagate on Container Swap | 库 |
| POD | Plain Old Data | 语言 |
| QoI | Quality of Implementation | 编译 |
| RAII | Resource Acquisition Is Initialization | 模式 |
| RACO | Range Adaptor Closure Object | 库 |
| RAO | Range Adaptor Object | 库 |
| RCU | Read-Copy-Update | 并发 |
| RTTI | RunTime Type Identification | 语言 |
| RVO | Return Value Optimization | 优化 |
| SBO | Small Buffer Optimization | 优化 |
| SBRM | Scope-Bound Resource Management | 模式 |
| SCARY | (见正文详解) | 设计 |
| SEH | Structured Exception Handling | 系统 |
| SFINAE | Substitution Failure Is Not An Error | 语言 |
| SIMD | Single Instruction Multiple Data | 性能 |
| SIOF | Static Initialization Order Fiasco | 语言 |
| SMF | Special Member Function | 语言 |
| SOCCC | Select On Container Copy Construction | 库 |
| SOO | Small Object Optimization | 优化 |
| SSO | Small String Optimization | 优化 |
| STL | Standard Template Library | 库 |
| TCO | Tail Call Optimization | 优化 |
| TLS | Thread-Local Storage | 并发 |
| TMP | Template Meta Programming | 技术 |
| TU | Translation Unit | 编译 |
| UB | Undefined Behavior | 语言 |
| UDC | User-Defined Conversion operator | 语言 |
| UDL | User-Defined Literals | 语言 |
| UFCS | Universal Function Call Syntax | 语言 |
| VLA | Variable-Length Array | 语言 |
| WPO | Whole-Program Optimization | 优化 |