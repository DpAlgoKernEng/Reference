# C++ 异常（Exceptions）

来源：cppreference.com

## 异常概览

| 异常相关主题 | 说明 |
|--------------|------|
| **基础概念** | try块、抛出异常、处理异常 |
| **异常规范** | noexcept规范（C++11）、动态规范（C++17前） |
| **现代特性** | noexcept运算符（C++11）、constexpr异常（C++26） |

**异常处理**提供了一种从程序执行某点向之前经过的执行点关联的处理程序传递控制和信息的方法（即异常处理沿调用栈向上传递控制）。

计算throw表达式将抛出异常。异常也可以在其他上下文中抛出。

为了捕获异常，throw表达式必须在try块内，且try块必须包含匹配异常对象类型的处理程序。

---

## 异常处理机制

### 一、异常抛出与捕获

```cpp
try {
    // 可能抛出异常的代码
    throw std::runtime_error("错误信息");
} catch (const std::exception& e) {
    // 处理异常
    std::cerr << "捕获异常: " << e.what() << std::endl;
}
```

### 二、函数异常规范

声明函数时，可提供以下规范限制函数可能抛出的异常类型：

#### 1. 动态异常规范（C++17前已弃用）
```cpp
void func() throw(std::exception);  // C++17前
```

#### 2. noexcept规范（C++11）
```cpp
void func() noexcept;               // 不抛出异常
void func() noexcept(false);        // 可能抛出异常
```

### 三、异常处理错误

异常处理过程中出现的错误由`std::terminate`和`std::unexpected`（C++17前）处理。

---

## 异常使用场景

### 一、错误处理

虽然throw表达式可用于将控制转移到执行栈上任意代码块（类似于`std::longjmp`），但其预期用途是**错误处理**。

#### 抛出异常的典型错误场景：

1. **后置条件失败**：如无法生成有效的返回值对象
2. **前置条件失败**：无法满足必须调用的另一个函数的前置条件
3. **类不变量破坏**：（对于非私有成员函数）无法（重新）建立类不变量

> 💡 **特别说明**：这意味着构造函数（参见RAII）和大多数运算符的失败应该通过抛出异常来报告。

#### 宽契约函数

**宽契约函数**使用异常指示不可接受的输入：
```cpp
std::string str = "hello";
try {
    char ch = str.at(10);  // 无前置条件，但抛出异常指示越界
} catch (const std::out_of_range& e) {
    std::cerr << "索引越界" << std::endl;
}
```

---

## 异常安全保证

函数报告错误条件后，可能提供关于程序状态的额外保证。通常认可以下四个级别的异常保证（逐级包含）：

### 1. 不抛出（Nothrow/Nofail）异常保证
- **Nothrow**：函数从不抛出异常
  - 析构函数和可能在栈展开期间调用的函数应为nothrow
  - 析构函数默认为`noexcept`（C++11）
- **Nofail**：函数总是成功
  - 交换操作、移动构造函数等提供强异常保证的函数应为nofail

### 2. 强异常保证（Strong Exception Guarantee）
- 如果函数抛出异常，程序状态回滚到函数调用前的状态
- 示例：`std::vector::push_back`

### 3. 基本异常保证（Basic Exception Guarantee）
- 如果函数抛出异常，程序处于有效状态
- 无资源泄漏，所有对象不变量保持完整

### 4. 无异常保证（No Exception Guarantee）
- 如果函数抛出异常，程序可能不处于有效状态
- 可能发生资源泄漏、内存损坏或其他破坏不变量的错误

### 5. 异常中性保证（Exception-neutral Guarantee）
- 如果异常从模板参数抛出（如`std::sort`的`Compare`函数对象或`std::make_shared`中`T`的构造函数），则原样传播给调用者

---

## 异常对象处理

### 一、可抛出类型

虽然任何完整类型和cv void指针都可作为异常对象抛出，但所有标准库函数都按值抛出无名对象，这些对象的类型直接或间接派生自`std::exception`。

### 二、最佳捕获实践

为避免不必要的异常对象复制和对象切片，处理程序的最佳实践是**按引用捕获**：

```cpp
try {
    // 抛出异常
} catch (const std::exception& e) {  // 按引用捕获
    // 处理
} catch (...) {  // 捕获所有异常
    // 处理未知异常
}
```

### 三、用户定义异常

用户定义异常通常遵循标准模式：

```cpp
class CustomException : public std::exception {
private:
    std::string message;
    
public:
    CustomException(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};
```

---

## 扩展知识详解

### 一、现代异常特性

#### 1. noexcept规范与运算符（C++11）

```cpp
void func() noexcept {
    // 不抛出异常的函数
}

void another_func() noexcept(noexcept(func())) {
    // 根据func()是否noexcept决定
    func();
}

// noexcept运算符
static_assert(noexcept(func()), "func should be noexcept");
```

#### 2. constexpr异常（C++26）

```cpp
// Feature-test宏
#if __cpp_constexpr_exceptions >= 202411L
// 支持constexpr异常
#endif
```

### 二、异常处理最佳实践

#### 1. RAII与异常安全
```cpp
class Resource {
public:
    Resource() { /* 获取资源 */ }
    ~Resource() noexcept { /* 释放资源 */ }
};

void func() {
    Resource res;  // RAII确保异常安全
    // 即使抛出异常，析构函数也会被调用
}
```

#### 2. 异常类型层次
```cpp
class MathException : public std::exception {
    // 数学异常基类
};

class DivideByZero : public MathException {
    // 除零异常
};

class Overflow : public MathException {
    // 溢出异常
};
```

#### 3. 异常传播
```cpp
void low_level_func() {
    throw std::runtime_error("底层错误");
}

void mid_level_func() {
    low_level_func();  // 异常传播
}

void high_level_func() {
    try {
        mid_level_func();
    } catch (const std::exception& e) {
        // 处理异常
        throw;  // 重新抛出
    }
}
```

### 三、异常与性能

#### 1. 异常开销
```cpp
// 无异常情况下的优化
void fast_func() noexcept {
    // 编译器可进行更多优化
}
```

#### 2. 异常安全容器操作
```cpp
std::vector<int> vec(1000);
try {
    vec.push_back(42);  // 提供强异常保证
} catch (...) {
    // vec状态保持不变
}
```

---

## 实际应用示例

### 一、完整异常处理示例

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

class BankAccount {
private:
    double balance;
    
public:
    BankAccount(double initial_balance) : balance(initial_balance) {
        if (initial_balance < 0) {
            throw std::invalid_argument("初始余额不能为负");
        }
    }
    
    void deposit(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("存款金额必须为正数");
        }
        balance += amount;
    }
    
    void withdraw(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("取款金额必须为正数");
        }
        if (amount > balance) {
            throw std::runtime_error("余额不足");
        }
        balance -= amount;
    }
    
    double get_balance() const noexcept {
        return balance;
    }
};

void process_transactions() {
    try {
        BankAccount account(1000.0);
        
        account.deposit(500.0);
        account.withdraw(200.0);
        
        std::cout << "最终余额: " << account.get_balance() << std::endl;
        
    } catch (const std::invalid_argument& e) {
        std::cerr << "参数错误: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "运行时错误: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "未知错误: " << e.what() << std::endl;
    }
}
```

### 二、模板异常安全示例

```cpp
template<typename T>
class SafeVector {
private:
    std::vector<T> data;
    
public:
    // 提供强异常保证
    void push_back(const T& value) {
        data.push_back(value);  // std::vector::push_back提供强异常保证
    }
    
    // 提供nothrow保证
    size_t size() const noexcept {
        return data.size();
    }
    
    // 提供基本异常保证
    T& at(size_t index) {
        return data.at(index);  // 可能抛出std::out_of_range
    }
};
```

### 三、现代异常特性应用

```cpp
// C++11 noexcept示例
class NoexceptExample {
public:
    ~NoexceptExample() noexcept = default;  // 默认noexcept
    
    void safe_operation() noexcept {
        // 不抛出异常的操作
    }
    
    void conditional_noexcept() noexcept(noexcept(safe_operation())) {
        safe_operation();
    }
};

// C++26 constexpr异常（如果支持）
#if __cpp_constexpr_exceptions >= 202411L
constexpr void compile_time_check() {
    if (false) {
        throw std::runtime_error("编译时异常");
    }
}
#endif
```

---

## 特性测试宏

| 特性测试宏 | 值 | 标准 | 特性 |
|------------|----|------|------|
| `__cpp_constexpr_exceptions` | `202411L` | C++26 | constexpr异常 |

---

## 参考文献与外部链接

1. H. Sutter (2004) "When and How to Use Exceptions" in Dr. Dobb's
2. H. Sutter, A. Alexandrescu (2004), "C++ Coding Standards", Item 70
3. C++ Core Guidelines I.10: Use exceptions to signal a failure to perform a required task
4. B. Stroustrup (2000), "The C++ Programming Language" Appendix E
5. H. Sutter (2000) "Exceptional C++"
6. D. Abrahams (2001) "Exception Safety in Generic Components"
7. D. Abrahams (2001) "Error and Exception Handling"
8. isocpp.org Super-FAQ "What should I throw?"
9. C++ Core Guidelines E.14: Use purpose-designed user-defined types as exceptions (not built-in types)
10. C++ Core Guidelines E.15: Throw by value, catch exceptions from a hierarchy by reference
11. S. Meyers (1996) "More Effective C++" Item 13
12. isocpp.org Super-FAQ "What should I catch?"
13. H. Sutter, A. Alexandrescu (2004) "C++ Coding Standards" Item 73

---

## 标准参考文献

### 相关C++标准章节：

- C++23标准
  - 14.4 Exception handling
- C++20标准
  - 14.4 Exception handling
- C++17标准
  - 18.4 Exception handling
- C++14标准
  - 15.4 Exception handling
- C++11标准
  - 15.4 Exception handling
- C++98/C++03标准
  - 15.4 Exception handling

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/language/exceptions&oldid=178582>
- 最后修改时间：2024年12月20日 13:54
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++异常处理机制是现代C++编程的重要组成部分，提供了结构化错误处理和资源管理的强有力工具。理解和正确应用不同级别的异常安全保证，对于编写健壮、可靠的C++程序至关重要。noexcept规范和运算符使程序员能够更好地控制异常行为，提高程序性能。按引用捕获异常对象是最佳实践，可避免对象切片和不必要的复制开销。RAII与异常处理的结合，为资源管理提供了优雅的解决方案。现代C++引入的constexpr异常等特性，进一步扩展了异常处理的应用场景。掌握异常处理的最佳实践和设计模式，能够显著提升代码质量和可维护性。