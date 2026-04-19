# C++ Idioms（编程惯用法）

## 1. 概述

C++ idioms（编程惯用法）是 C++ 社区在长期实践中总结出的特定模式、技术和最佳实践。这些惯用法利用 C++ 独特的语言特性（如模板、RAII、对象生命周期管理）来解决常见编程问题。

**主要用途**：
- 提高代码质量和可维护性
- 避免常见陷阱和错误
- 优化性能，遵循零开销原则
- 降低编译依赖，提升构建效率

**在 C++ 技术体系中的位置**：

```
C++ 知识体系
├── 语言核心特性
├── 标准库 (STL)
├── 编程惯用法 (Idioms)  ← 本文档
├── 设计模式 (Design Patterns)
└── 最佳实践 (Best Practices)
```

与通用设计模式（如 GoF 模式）不同，C++ idioms 直接利用 C++ 语言特性，更贴近底层实现，解决 C++ 特有的问题。

## 2. 来源与演变

### 历史背景

C++ idioms 的形成经历了三个主要阶段：

| 时期 | 代表性 Idioms | 驱动因素 |
|------|--------------|----------|
| 1990s | RAII、Rule of Three | 异常安全、资源管理 |
| 2000s | CRTP、Pimpl | 模板元编程、编译优化 |
| 2010s+ | Rule of Five/Zero | 移动语义 (Move Semantics，C++11) |

### 设计动机

C++ idioms 的出现是为了解决以下核心问题：

1. **资源管理复杂性**
   - C++ 手动管理内存和系统资源
   - RAII 将资源生命周期绑定到对象生命周期

2. **编译效率问题**
   - C++ 头文件包含导致编译依赖传递
   - Pimpl 模式隔离实现细节，降低重编译范围

3. **性能与抽象的平衡**
   - 零开销原则要求不使用的特性不应产生运行时成本
   - CRTP 实现静态多态，避免虚函数开销

### 版本演进

| C++ 版本 | 影响的 Idioms | 主要变化 |
|----------|--------------|----------|
| C++98/03 | RAII、Rule of Three | 异常安全成为核心考量 |
| C++11 | Rule of Five/Zero | 移动语义引入，五法则扩展 |
| C++17 | constexpr 增强 | 更多编译期计算优化 |
| C++20 | Concepts | 模板约束更清晰，部分 SFINAE 被 Concepts 替代 |

## 3. 分类与特征

C++ idioms 按用途可分为三大类：

### 设计模式类 (Patterns)

利用 C++ 特性实现的特定设计模式。

| Idiom | 英文全称 | 核心思想 |
|-------|----------|----------|
| [CRTP](01_patterns/crtp.md) | Curiously Recurring Template Pattern | 奇异递归模板，基类模板参数派生自自身 |
| [Pimpl](01_patterns/pimpl.md) | Pointer to Implementation | 指向实现的指针，隐藏实现细节 |

**特征**：
- 利用模板或指针实现编译期或运行期隔离
- 降低耦合，提升编译效率
- 增强封装性

### 资源管理类 (Resource Management)

C++ 资源管理的核心惯用法，确保资源正确获取和释放。

| Idiom | 英文全称 | 核心思想 |
|-------|----------|----------|
| [RAII](02_resource_management/raii.md) | Resource Acquisition Is Initialization | 资源获取即初始化，绑定生命周期 |
| [Rule of Three/Five/Zero](02_resource_management/rule_of_three.md) | - | 定义特殊成员函数的规则 |

**特征**：
- 构造函数获取资源，析构函数释放资源
- 异常安全保证
- 消除资源泄漏风险

### 设计原则类 (Principles)

C++ 语言设计的核心哲学原则。

| Principle | 英文全称 | 核心思想 |
|-----------|----------|----------|
| [Zero-overhead Principle](03_principles/zero_overhead_principle.md) | Zero-overhead Principle | 不使用的特性不应产生成本 |

**特征**：
- 指导 C++ 语言设计的核心哲学
- 抽象不应带来运行时开销
- 性能可预测

## 4. 底层原理

### RAII 原理

RAII 基于 C++ 对象生命周期机制：

```cpp
class Resource {
public:
    Resource() : handle_(acquire()) {}  // 构造时获取
    ~Resource() { release(handle_); }    // 析构时释放
    // 禁止拷贝，避免重复释放
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
private:
    Handle handle_;
};
```

**实现机制**：
- 栈展开 (Stack Unwinding) 保证析构函数调用
- 对象离开作用域时自动销毁
- 异常安全：即使发生异常，局部对象仍被正确析构

### CRTP 原理

CRTP 利用模板实例化实现静态多态：

```cpp
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
};

class Concrete : public Base<Concrete> {
public:
    void implementation() { /* 具体实现 */ }
};
```

**实现机制**：
- 模板实例化在编译期生成具体类
- 静态类型转换消除运行时开销
- 无虚函数表 (Virtual Table，vtable)，减少内存占用

### Pimpl 原理

Pimpl 利用指针的不透明性隔离实现：

```cpp
// Widget.h
class Widget {
public:
    Widget();
    ~Widget();
private:
    class Impl;           // 前向声明
    std::unique_ptr<Impl> pimpl_;  // 不透明指针
};

// Widget.cpp
class Widget::Impl {
    // 所有私有成员和实现细节
};
```

**实现机制**：
- 指针大小固定，不依赖具体类型
- 前向声明消除头文件依赖
- 实现变更不触发使用方重编译

## 5. 使用场景

### RAII 适用场景

| 场景 | 示例 |
|------|------|
| 内存管理 | `std::unique_ptr`, `std::shared_ptr` |
| 文件操作 | `std::fstream` |
| 锁管理 | `std::lock_guard`, `std::unique_lock` |
| 网络连接 | 自定义 Socket RAII 封装 |

**最佳实践**：
- 优先使用标准库 RAII 类型
- 自定义资源类型时遵循 Rule of Zero/Five
- 避免裸指针和手动 `delete`

### CRTP 适用场景

| 场景 | 用途 |
|------|------|
| 静态多态 | 替代虚函数，避免运行时开销 |
| Mixin 类 | 为派生类添加通用功能 |
| 代码复用 | 编译期生成相似代码 |

**注意事项**：
- 调试困难，模板错误信息复杂
- 增加编译时间和代码体积
- 仅在性能关键路径使用

### Pimpl 适用场景

| 场景 | 用途 |
|------|------|
| 库开发 | 隐藏实现，稳定 ABI |
| 大型项目 | 降低头文件依赖，加速编译 |
| 跨平台代码 | 平台特定实现隔离 |

**注意事项**：
- 增加一次堆分配和指针间接访问
- 需要手动定义析构函数（unique_ptr 完整类型要求）

### 常见陷阱

```cpp
// ❌ 错误：析构函数非虚导致资源泄漏
class Base { ~Base() {} };  // 非 virtual
class Derived : public Base { Resource r; };
Base* p = new Derived;
delete p;  // Derived 析构函数不被调用

// ✅ 正确：使用虚析构函数或禁止多态删除
class Base { public: virtual ~Base() = default; };
```

## 6. 代码示例

### RAII 完整示例

```cpp
#include <memory>
#include <fstream>

// 文件句柄 RAII
class FileHandle {
public:
    explicit FileHandle(const char* path)
        : file_(std::fopen(path, "r")) {
        if (!file_) throw std::runtime_error("Failed to open file");
    }

    ~FileHandle() {
        if (file_) std::fclose(file_);
    }

    // 移动语义
    FileHandle(FileHandle&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file_) std::fclose(file_);
            file_ = other.file_;
            other.file_ = nullptr;
        }
        return *this;
    }

    // 禁止拷贝
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FILE* get() const { return file_; }

private:
    FILE* file_;
};

// 使用
void process_file() {
    FileHandle fh("data.txt");  // 异常安全
    // 使用 fh.get()...
}  // 自动关闭文件
```

### CRTP 完整示例

```cpp
#include <iostream>

// CRTP 基类：可计数对象
template<typename Derived>
class Counter {
public:
    Counter() { ++count_; }
    Counter(const Counter&) { ++count_; }
    ~Counter() { --count_; }

    static int get_count() { return count_; }

private:
    static inline int count_ = 0;
};

// 派生类
class Widget : public Counter<Widget> {
public:
    void do_something() { /* ... */ }
};

class Gadget : public Counter<Gadget> {
    // 独立的计数器
};

int main() {
    Widget w1, w2;
    Gadget g1;

    std::cout << "Widgets: " << Widget::get_count() << "\n";  // 2
    std::cout << "Gadgets: " << Gadget::get_count() << "\n";  // 1
}
```

### Pimpl 完整示例

```cpp
// ==================== Widget.h ====================
#pragma once
#include <memory>

class Widget {
public:
    Widget();
    ~Widget();  // 必须声明，因为 unique_ptr 需要完整类型

    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    // 禁止拷贝（或实现深拷贝）
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

    void do_something();

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

// ==================== Widget.cpp ====================
#include "Widget.h"
#include <string>
#include <vector>

class Widget::Impl {
public:
    void do_something() { /* 实现细节 */ }

private:
    std::string name_;
    std::vector<int> data_;
};

Widget::Widget() : pimpl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::do_something() {
    pimpl_->do_something();
}
```

### Rule of Five 示例

```cpp
class Resource {
public:
    // 构造
    Resource() : data_(nullptr), size_(0) {}
    explicit Resource(size_t size) : data_(new int[size]), size_(size) {}

    // 析构
    ~Resource() { delete[] data_; }

    // 拷贝构造
    Resource(const Resource& other)
        : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 拷贝赋值
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    // 移动构造
    Resource(Resource&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 移动赋值
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

private:
    int* data_;
    size_t size_;
};
```

## 7. 总结

### 核心要点回顾

| Idiom | 核心价值 | 关键技术 |
|-------|----------|----------|
| RAII | 异常安全资源管理 | 析构函数自动调用 |
| Rule of Three/Five/Zero | 资源管理完整性 | 特殊成员函数定义 |
| Pimpl | 编译隔离、ABI 稳定 | 不透明指针 |
| CRTP | 静态多态、性能优化 | 模板实例化 |
| 零开销原则 | 性能可预测 | 编译期决策 |

### 相关技术对比

| 特性 | RAII | Pimpl | CRTP |
|------|------|-------|------|
| 运行时开销 | 无 | 一次指针间接访问 | 无 |
| 编译时开销 | 无 | 降低依赖，加速编译 | 增加模板实例化 |
| 使用复杂度 | 低 | 中 | 高 |
| 适用场景 | 资源管理 | 库开发、编译优化 | 性能关键路径 |

### 学习建议

1. **RAII 是基础** - 必须完全掌握，它是 C++ 资源管理的核心
2. **Rule of Zero 优先** - 优先使用标准库智能指针，避免手动管理
3. **Pimpl 用于库开发** - 当需要稳定 ABI 或降低编译依赖时使用
4. **CRTP 谨慎使用** - 仅在性能关键场景使用，注意调试成本
5. **理解零开销原则** - 这是理解 C++ 性能哲学的关键

### 延伸阅读

- 《Effective C++》 - Scott Meyers
- 《Effective Modern C++》 - Scott Meyers
- 《C++ Core Guidelines》 - Bjarne Stroustrup et al.
- [CppReference - RAII](https://en.cppreference.com/w/cpp/language/raii)