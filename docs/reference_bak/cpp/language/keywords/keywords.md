# C++ 关键字详解

  [1. C++关键字概述](#1-c关键字概述)
  
  [2. A-C 关键字详解](#2-a-c-关键字详解)
  
  [3. D-P 关键字详解](#3-d-p-关键字详解)
  
  [4. R-Z 关键字详解](#4-r-z-关键字详解)
  
  [5. 具有特殊含义的标识符](#5-具有特殊含义的标识符)
  
  [6. 预处理器关键字](#6-预处理器关键字)
  
  [7. 保留标识符](#7-保留标识符)
  
  [8. 标准命名空间](#8-标准命名空间)
  
  [9. 关键字使用最佳实践](#9-关键字使用最佳实践)
  
  [10. 总结](#10-总结)

## 1. C++关键字概述

### 1.1 关键字定义

C++关键字是编程语言中具有特殊含义和用途的保留字，这些词不能用作标识符（变量名、函数名、类名等）。关键字构成了C++语言的基础语法和语义。

```cpp
#include <iostream>

void keyword_overview() {
    std::cout << "C++ Keywords Overview:" << std::endl;
    
    // 关键字的特征
    std::cout << "  === Keyword Characteristics ===" << std::endl;
    std::cout << "    - 保留字，不能重新定义或重载" << std::endl;
    std::cout << "    - 具有预定义的语法和语义" << std::endl;
    std::cout << "    - 构成C++语言的基础元素" << std::endl;
    std::cout << "    - 在属性中不被视为保留（不包括属性参数列表）" << std::endl;
    
    // 关键字分类
    std::cout << "  === Keyword Categories ===" << std::endl;
    std::cout << "    - 基本数据类型关键字" << std::endl;
    std::cout << "    - 控制流关键字" << std::endl;
    std::cout << "    - 存储类关键字" << std::endl;
    std::cout << "    - 类型转换关键字" << std::endl;
    std::cout << "    - 操作符关键字" << std::endl;
    std::cout << "    - 访问控制关键字" << std::endl;
    std::cout << "    - 异常处理关键字" << std::endl;
    std::cout << "    - 模板相关关键字" << std::endl;
    std::cout << "    - 并发相关关键字" << std::endl;
    std::cout << "    - 现代C++特性关键字" << std::endl;
    
    // 版本演进
    std::cout << "  === Version Evolution ===" << std::endl;
    std::cout << "    - C++98/03: 基础关键字集" << std::endl;
    std::cout << "    - C++11: 新增32个关键字" << std::endl;
    std::cout << "    - C++14: 新增1个关键字" << std::endl;
    std::cout << "    - C++17: 新增1个关键字" << std::endl;
    std::cout << "    - C++20: 新增10个关键字" << std::endl;
    std::cout << "    - C++23: 新增2个关键字" << std::endl;
    std::cout << "    - C++26: 新增6个关键字" << std::endl;
}
```

### 1.2 关键字使用规则

```cpp
#include <iostream>

void keyword_usage_rules() {
    std::cout << "Keyword Usage Rules:" << std::endl;
    
    // 有效的标识符命名
    std::cout << "  === Valid Identifier Names ===" << std::endl;
    std::cout << "    int variable;           // 正确" << std::endl;
    std::cout << "    double my_function();   // 正确" << std::endl;
    std::cout << "    class MyClass {};       // 正确" << std::endl;
    
    // 无效的标识符命名（编译错误）
    std::cout << "  === Invalid Identifier Names (Compilation Errors) ===" << std::endl;
    // int int;                 // 错误：不能使用关键字作为标识符
    // double class = 3.14;     // 错误：class是关键字
    // void if() {}             // 错误：if是关键字
    
    // 在属性中的例外情况
    std::cout << "  === Exception in Attributes ===" << std::endl;
    std::cout << "    [[deprecated(\"use new_function instead\")]]  // 正确" << std::endl;
    std::cout << "    [[likely]] [[unlikely]]                      // 正确" << std::endl;
    
    // 替代表示法
    std::cout << "  === Alternative Representations ===" << std::endl;
    std::cout << "    and    -> &&" << std::endl;
    std::cout << "    or     -> ||" << std::endl;
    std::cout << "    not    -> !" << std::endl;
    std::cout << "    bitand -> &" << std::endl;
    std::cout << "    bitor  -> |" << std::endl;
}
```

## 2. A-C 关键字详解

### 2.1 alignas (C++11)

```cpp
#include <iostream>
#include <type_traits>

void alignas_keyword() {
    std::cout << "alignas Keyword:" << std::endl;
    
    // 基本用法：指定对齐要求
    std::cout << "  === Basic Usage ===" << std::endl;
    
    // 指定对齐为16字节
    struct alignas(16) AlignedStruct {
        int x;
        double y;
    };
    
    // 使用对齐值
    alignas(32) int aligned_int;
    
    std::cout << "    sizeof(AlignedStruct): " << sizeof(AlignedStruct) << std::endl;
    std::cout << "    alignof(AlignedStruct): " << alignof(AlignedStruct) << std::endl;
    std::cout << "    alignof(aligned_int): " << alignof(aligned_int) << std::endl;
    
    // 与alignof配合使用
    std::cout << "  === With alignof ===" << std::endl;
    std::cout << "    alignof(int): " << alignof(int) << std::endl;
    std::cout << "    alignof(double): " << alignof(double) << std::endl;
    std::cout << "    alignof(std::max_align_t): " << alignof(std::max_align_t) << std::endl;
    
    // 实际应用示例
    std::cout << "  === Practical Examples ===" << std::endl;
    
    // 为SIMD优化对齐数据
    struct SIMDData {
        alignas(16) float data[4];  // 16字节对齐，适合SSE指令
    };
    
    // 硬件寄存器对齐
    struct HardwareRegister {
        alignas(64) uint32_t control;  // 缓存行对齐
        alignas(64) uint32_t status;
    };
}
```

### 2.2 alignof (C++11)

```cpp
#include <iostream>
#include <type_traits>

void alignof_keyword() {
    std::cout << "alignof Keyword:" << std::endl;
    
    // 查询类型的对齐要求
    std::cout << "  === Type Alignment Queries ===" << std::endl;
    std::cout << "    alignof(char): " << alignof(char) << std::endl;
    std::cout << "    alignof(int): " << alignof(int) << std::endl;
    std::cout << "    alignof(double): " << alignof(double) << std::endl;
    std::cout << "    alignof(void*): " << alignof(void*) << std::endl;
    std::cout << "    alignof(std::max_align_t): " << alignof(std::max_align_t) << std::endl;
    
    // 结构体对齐
    struct SimpleStruct {
        char a;
        int b;
        double c;
    };
    
    std::cout << "  === Struct Alignment ===" << std::endl;
    std::cout << "    sizeof(SimpleStruct): " << sizeof(SimpleStruct) << std::endl;
    std::cout << "    alignof(SimpleStruct): " << alignof(SimpleStruct) << std::endl;
    
    // 空类对齐
    struct Empty {};
    std::cout << "  === Empty Class Alignment ===" << std::endl;
    std::cout << "    alignof(Empty): " << alignof(Empty) << std::endl;
    std::cout << "    sizeof(Empty): " << sizeof(Empty) << std::endl;
    
    // 数组对齐
    std::cout << "  === Array Alignment ===" << std::endl;
    std::cout << "    alignof(int[10]): " << alignof(int[10]) << std::endl;
    std::cout << "    alignof(double[5]): " << alignof(double[5]) << std::endl;
}
```

### 2.3 auto (C++11, C++14, C++17, C++23)

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>

void auto_keyword() {
    std::cout << "auto Keyword:" << std::endl;
    
    // C++11: 类型推导
    std::cout << "  === C++11 Type Deduction ===" << std::endl;
    auto i = 42;                    // int
    auto d = 3.14;                  // double
    auto s = "hello";               // const char*
    auto v = std::vector<int>{1,2,3}; // std::vector<int>
    
    std::cout << "    auto i = 42;        // type: " << typeid(i).name() << std::endl;
    std::cout << "    auto d = 3.14;      // type: " << typeid(d).name() << std::endl;
    std::cout << "    auto s = \"hello\";   // type: " << typeid(s).name() << std::endl;
    
    // C++14: 函数返回类型推导
    std::cout << "  === C++14 Function Return Type Deduction ===" << std::endl;
    auto add(int a, int b) {
        return a + b;  // 推导为int
    }
    
    auto multiply(double a, double b) {
        return a * b;  // 推导为double
    }
    
    std::cout << "    add(3, 4) = " << add(3, 4) << std::endl;
    std::cout << "    multiply(2.5, 3.0) = " << multiply(2.5, 3.0) << std::endl;
    
    // C++17: 模板参数推导
    std::cout << "  === C++17 Template Parameter Deduction ===" << std::endl;
    std::pair p1{1, 2.0};           // std::pair<int, double>
    std::vector vec{1, 2, 3, 4, 5}; // std::vector<int>
    
    std::cout << "    std::pair p{1, 2.0}; // type: pair<int, double>" << std::endl;
    
    // C++23: 占位符作为函数参数（示例）
    std::cout << "  === C++23 Placeholder Parameters (Future) ===" << std::endl;
    // auto func(auto x, auto y) {  // C++20概念 + C++23扩展
    //     return x + y;
    // }
}
```

### 2.4 bool, true, false

```cpp
#include <iostream>
#include <type_traits>

void bool_keywords() {
    std::cout << "bool, true, false Keywords:" << std::endl;
    
    // 基本布尔类型
    std::cout << "  === Basic Boolean Type ===" << std::endl;
    bool flag = true;
    bool condition = false;
    
    std::cout << "    sizeof(bool): " << sizeof(bool) << " byte(s)" << std::endl;
    std::cout << "    true value: " << true << std::endl;
    std::cout << "    false value: " << false << std::endl;
    
    // 布尔运算
    std::cout << "  === Boolean Operations ===" << std::endl;
    std::cout << "    true && false = " << (true && false) << std::endl;
    std::cout << "    true || false = " << (true || false) << std::endl;
    std::cout << "    !true = " << (!true) << std::endl;
    
    // 整数到布尔的转换
    std::cout << "  === Integer to Boolean Conversion ===" << std::endl;
    bool b1 = 1;     // true
    bool b2 = 0;     // false
    bool b3 = -1;    // true (非零为真)
    
    std::cout << "    bool b1 = 1;   // " << b1 << std::endl;
    std::cout << "    bool b2 = 0;   // " << b2 << std::endl;
    std::cout << "    bool b3 = -1;  // " << b3 << std::endl;
    
    // 布尔到整数的转换
    std::cout << "  === Boolean to Integer Conversion ===" << std::endl;
    int i1 = true;   // 1
    int i2 = false;  // 0
    
    std::cout << "    int i1 = true;   // " << i1 << std::endl;
    std::cout << "    int i2 = false;  // " << i2 << std::endl;
}
```

### 2.5 break, continue

```cpp
#include <iostream>
#include <vector>

void control_flow_keywords() {
    std::cout << "break, continue Keywords:" << std::endl;
    
    // break关键字
    std::cout << "  === break Keyword ===" << std::endl;
    std::cout << "    Breaking out of loop:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        if (i == 5) {
            std::cout << "      Breaking at i = " << i << std::endl;
            break;
        }
        std::cout << "      i = " << i << std::endl;
    }
    
    // switch语句中的break
    std::cout << "  === break in switch ===" << std::endl;
    int choice = 2;
    switch (choice) {
        case 1:
            std::cout << "      Case 1" << std::endl;
            break;
        case 2:
            std::cout << "      Case 2" << std::endl;
            break;  // 防止fall-through
        case 3:
            std::cout << "      Case 3" << std::endl;
            break;
        default:
            std::cout << "      Default case" << std::endl;
    }
    
    // continue关键字
    std::cout << "  === continue Keyword ===" << std::endl;
    std::cout << "    Skipping even numbers:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        std::cout << "      Odd number: " << i << std::endl;
    }
    
    // 嵌套循环中的break和continue
    std::cout << "  === Nested Loops ===" << std::endl;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == 1 && j == 1) {
                std::cout << "      Breaking inner loop at i=" << i << ", j=" << j << std::endl;
                break;
            }
            std::cout << "      i=" << i << ", j=" << j << std::endl;
        }
    }
}
```

### 2.6 case, default

```cpp
#include <iostream>

void switch_keywords() {
    std::cout << "case, default Keywords:" << std::endl;
    
    // 基本switch语句
    std::cout << "  === Basic switch Statement ===" << std::endl;
    int value = 2;
    switch (value) {
        case 1:
            std::cout << "    Value is 1" << std::endl;
            break;
        case 2:
            std::cout << "    Value is 2" << std::endl;
            break;
        case 3:
            std::cout << "    Value is 3" << std::endl;
            break;
        default:
            std::cout << "    Value is something else" << std::endl;
    }
    
    // 多个case共享代码块
    std::cout << "  === Multiple Cases ===" << std::endl;
    char grade = 'B';
    switch (grade) {
        case 'A':
        case 'B':
        case 'C':
            std::cout << "    Passing grade" << std::endl;
            break;
        case 'D':
        case 'F':
            std::cout << "    Failing grade" << std::endl;
            break;
        default:
            std::cout << "    Invalid grade" << std::endl;
    }
    
    // switch中的作用域
    std::cout << "  === Scope in switch ===" << std::endl;
    int choice = 1;
    switch (choice) {
        case 1: {
            int x = 10;  // 需要花括号创建作用域
            std::cout << "    Local variable x = " << x << std::endl;
            break;
        }
        case 2: {
            int y = 20;  // 每个case可以有自己的局部变量
            std::cout << "    Local variable y = " << y << std::endl;
            break;
        }
        default:
            std::cout << "    Default case" << std::endl;
    }
}
```

### 2.7 char, char8_t, char16_t, char32_t, wchar_t

```cpp
#include <iostream>
#include <type_traits>

void character_type_keywords() {
    std::cout << "Character Type Keywords:" << std::endl;
    
    // char类型
    std::cout << "  === char ===" << std::endl;
    char c1 = 'A';
    char c2 = 65;  // ASCII值
    std::cout << "    char c1 = 'A';     // " << c1 << std::endl;
    std::cout << "    char c2 = 65;      // " << c2 << std::endl;
    std::cout << "    sizeof(char): " << sizeof(char) << " byte(s)" << std::endl;
    
    // char8_t (C++20) - UTF-8字符
    std::cout << "  === char8_t (C++20) ===" << std::endl;
    char8_t u8_char = u8'A';
    std::cout << "    sizeof(char8_t): " << sizeof(char8_t) << " byte(s)" << std::endl;
    
    // char16_t (C++11) - UTF-16字符
    std::cout << "  === char16_t (C++11) ===" << std::endl;
    char16_t u16_char = u'A';
    std::cout << "    sizeof(char16_t): " << sizeof(char16_t) << " byte(s)" << std::endl;
    
    // char32_t (C++11) - UTF-32字符
    std::cout << "  === char32_t (C++11) ===" << std::endl;
    char32_t u32_char = U'A';
    std::cout << "    sizeof(char32_t): " << sizeof(char32_t) << " byte(s)" << std::endl;
    
    // wchar_t - 宽字符
    std::cout << "  === wchar_t ===" << std::endl;
    wchar_t w_char = L'A';
    std::cout << "    sizeof(wchar_t): " << sizeof(wchar_t) << " byte(s)" << std::endl;
    
    // 字符串字面量
    std::cout << "  === String Literals ===" << std::endl;
    const char* str1 = "Hello";           // 普通字符串
    const char8_t* str2 = u8"Hello";      // UTF-8字符串
    const char16_t* str3 = u"Hello";      // UTF-16字符串
    const char32_t* str4 = U"Hello";      // UTF-32字符串
    const wchar_t* str5 = L"Hello";       // 宽字符串
    
    std::cout << "    Ordinary string: " << str1 << std::endl;
}
```

### 2.8 class

```cpp
#include <iostream>
#include <memory>

void class_keyword() {
    std::cout << "class Keyword:" << std::endl;
    
    // 基本类定义
    std::cout << "  === Basic Class Definition ===" << std::endl;
    class BasicClass {
    private:
        int private_data;
    protected:
        int protected_data;
    public:
        int public_data;
        
        BasicClass(int value) : private_data(value), protected_data(value), public_data(value) {}
        
        int get_private_data() const { return private_data; }
        void set_private_data(int value) { private_data = value; }
    };
    
    BasicClass obj(42);
    std::cout << "    obj.public_data = " << obj.public_data << std::endl;
    std::cout << "    obj.get_private_data() = " << obj.get_private_data() << std::endl;
    
    // 继承
    std::cout << "  === Inheritance ===" << std::endl;
    class DerivedClass : public BasicClass {
    public:
        DerivedClass(int value) : BasicClass(value) {}
        
        void access_protected() {
            protected_data = 100;  // 可以访问protected成员
            std::cout << "    Protected data in derived class: " << protected_data << std::endl;
        }
    };
    
    DerivedClass derived(24);
    derived.access_protected();
    
    // 虚函数和多态
    std::cout << "  === Virtual Functions and Polymorphism ===" << std::endl;
    class Base {
    public:
        virtual void virtual_function() {
            std::cout << "    Base::virtual_function()" << std::endl;
        }
        
        virtual ~Base() = default;
    };
    
    class Derived : public Base {
    public:
        void virtual_function() override {
            std::cout << "    Derived::virtual_function()" << std::endl;
        }
    };
    
    std::unique_ptr<Base> ptr = std::make_unique<Derived>();
    ptr->virtual_function();  // 动态绑定
}
```

### 2.9 const, constexpr, consteval, constinit

```cpp
#include <iostream>
#include <array>

void const_keywords() {
    std::cout << "const, constexpr, consteval, constinit Keywords:" << std::endl;
    
    // const关键字
    std::cout << "  === const Keyword ===" << std::endl;
    const int constant_value = 42;
    const char* const_string = "Hello";
    
    std::cout << "    const int constant_value = 42;" << std::endl;
    std::cout << "    Value: " << constant_value << std::endl;
    
    // constexpr关键字 (C++11)
    std::cout << "  === constexpr Keyword (C++11) ===" << std::endl;
    constexpr int compile_time_constant = 100;
    constexpr int square(int x) { return x * x; }
    constexpr int result = square(5);
    
    std::cout << "    constexpr int compile_time_constant = 100;" << std::endl;
    std::cout << "    constexpr int result = square(5) = " << result << std::endl;
    
    // 编译时常量数组
    constexpr std::array<int, 5> compile_time_array = {1, 2, 3, 4, 5};
    std::cout << "    Compile-time array size: " << compile_time_array.size() << std::endl;
    
    // consteval关键字 (C++20)
    std::cout << "  === consteval Keyword (C++20) ===" << std::endl;
    consteval int always_compile_time(int x) {
        return x * 2;
    }
    
    constexpr int compile_result = always_compile_time(10);
    std::cout << "    consteval result: " << compile_result << std::endl;
    
    // constinit关键字 (C++20)
    std::cout << "  === constinit Keyword (C++20) ===" << std::endl;
    constinit int initialized_at_compile_time = 42;
    std::cout << "    constinit int initialized_at_compile_time = " << initialized_at_compile_time << std::endl;
}
```

### 2.10 contract_assert (C++26)

```cpp
#include <iostream>

void contract_assert_keyword() {
    std::cout << "contract_assert Keyword (C++26):" << std::endl;
    
    // 基本合约断言
    std::cout << "  === Basic Contract Assertion ===" << std::endl;
    auto divide = [](double a, double b) -> double {
        contract_assert(b != 0.0);  // 确保除数不为零
        return a / b;
    };
    
    std::cout << "    divide(10.0, 2.0) = " << divide(10.0, 2.0) << std::endl;
    
    // 数组访问检查
    std::cout << "  === Array Access Check ===" << std::endl;
    auto safe_access = [](const std::vector<int>& vec, size_t index) -> int {
        contract_assert(index < vec.size());  // 确保索引有效
        return vec[index];
    };
    
    std::vector<int> test_vec = {1, 2, 3, 4, 5};
    std::cout << "    safe_access(test_vec, 2) = " << safe_access(test_vec, 2) << std::endl;
    
    // 状态不变量检查
    std::cout << "  === State Invariant Check ===" << std::endl;
    class Counter {
    private:
        int count = 0;
        
    public:
        void increment() {
            contract_assert(count >= 0);  // 前置条件
            ++count;
            contract_assert(count > 0);   // 后置条件
        }
        
        int get_count() const {
            contract_assert(count >= 0);  // 不变量
            return count;
        }
    };
    
    Counter counter;
    counter.increment();
    std::cout << "    Counter value: " << counter.get_count() << std::endl;
}
```

### 2.11 co_await, co_return, co_yield (C++20)

```cpp
#include <iostream>
#include <coroutine>
#include <future>

void coroutine_keywords() {
    std::cout << "co_await, co_return, co_yield Keywords (C++20):" << std::endl;
    
    // 协程返回类型
    std::cout << "  === Coroutine Return Types ===" << std::endl;
    
    // 简单协程示例（概念演示）
    std::cout << "  === Coroutine Example (Concept) ===" << std::endl;
    std::cout << "    // Generator example:" << std::endl;
    std::cout << "    Generator<int> fibonacci() {" << std::endl;
    std::cout << "        int a = 0, b = 1;" << std::endl;
    std::cout << "        while (true) {" << std::endl;
    std::cout << "            co_yield a;  // 产生值" << std::endl;
    std::cout << "            auto temp = a;" << std::endl;
    std::cout << "            a = b;" << std::endl;
    std::cout << "            b = temp + b;" << std::endl;
    std::cout << "        }" << std::endl;
    std::cout << "    }" << std::endl;
    
    // co_await示例
    std::cout << "  === co_await Example ===" << std::endl;
    std::cout << "    Task<int> async_operation() {" << std::endl;
    std::cout << "        auto result = co_await some_async_function();" << std::endl;
    std::cout << "        co_return result + 1;" << std::endl;
    std::cout << "    }" << std::endl;
    
    // 协程的优势
    std::cout << "  === Coroutine Advantages ===" << std::endl;
    std::cout << "    - 简化异步代码编写" << std::endl;
    std::cout << "    - 提高代码可读性" << std::endl;
    std::cout << "    - 减少回调地狱" << std::endl;
    std::cout << "    - 支持生成器模式" << std::endl;
    std::cout << "    - 更好的资源管理" << std::endl;
}
```

## 3. D-P 关键字详解

### 3.1 decltype (C++11)

```cpp
#include <iostream>
#include <type_traits>
#include <vector>

void decltype_keyword() {
    std::cout << "decltype Keyword (C++11):" << std::endl;
    
    // 基本用法
    std::cout << "  === Basic Usage ===" << std::endl;
    int x = 42;
    decltype(x) y = x;  // y的类型是int
    
    std::cout << "    int x = 42;" << std::endl;
    std::cout << "    decltype(x) y = x;  // y is int" << std::endl;
    std::cout << "    y = " << y << std::endl;
    
    // 表达式类型推导
    std::cout << "  === Expression Type Deduction ===" << std::endl;
    const int cx = 10;
    decltype(cx) cz = cx;        // const int
    decltype((cx)) cref = cx;    // const int& (表达式是左值)
    
    std::cout << "    const int cx = 10;" << std::endl;
    std::cout << "    decltype(cx) cz = cx;     // type: const int" << std::endl;
    std::cout << "    decltype((cx)) cref = cx; // type: const int&" << std::endl;
    
    // 函数返回类型推导
    std::cout << "  === Function Return Type Deduction ===" << std::endl;
    auto func(int a, double b) -> decltype(a + b) {
        return a + b;
    }
    
    auto result = func(5, 3.14);
    std::cout << "    func(5, 3.14) = " << result << std::endl;
    std::cout << "    Result type: " << typeid(result).name() << std::endl;
    
    // 与模板配合使用
    std::cout << "  === With Templates ===" << std::endl;
    template<typename T, typename U>
    auto add(T t, U u) -> decltype(t + u) {
        return t + u;
    }
    
    auto sum = add(1, 2.5);
    std::cout << "    add(1, 2.5) = " << sum << std::endl;
}
```

### 3.2 delete, default

```cpp
#include <iostream>
#include <memory>

void special_function_keywords() {
    std::cout << "delete, default Keywords:" << std::endl;
    
    // default关键字
    std::cout << "  === default Keyword ===" << std::endl;
    class DefaultExample {
    public:
        DefaultExample() = default;           // 显式要求默认构造函数
        ~DefaultExample() = default;          // 显式要求默认析构函数
        DefaultExample(const DefaultExample&) = default;  // 显式要求拷贝构造函数
        DefaultExample& operator=(const DefaultExample&) = default;  // 拷贝赋值
        
        // 移动构造和移动赋值
        DefaultExample(DefaultExample&&) = default;
        DefaultExample& operator=(DefaultExample&&) = default;
    };
    
    DefaultExample obj1;
    DefaultExample obj2 = obj1;  // 使用默认拷贝构造函数
    std::cout << "    Default special functions used successfully" << std::endl;
    
    // delete关键字
    std::cout << "  === delete Keyword ===" << std::endl;
    class DeleteExample {
    public:
        DeleteExample() = default;
        
        // 禁用拷贝构造函数
        DeleteExample(const DeleteExample&) = delete;
        DeleteExample& operator=(const DeleteExample&) = delete;
        
        // 禁用特定参数类型的构造函数
        DeleteExample(int) = delete;
        
        // 只允许特定类型的参数
        DeleteExample(double) {}
    };
    
    DeleteExample del_obj1;
    // DeleteExample del_obj2 = del_obj1;  // 编译错误：拷贝构造被禁用
    DeleteExample del_obj3(3.14);         // 正确：double参数允许
    // DeleteExample del_obj4(42);          // 编译错误：int参数被禁用
    
    std::cout << "    Delete keyword successfully restricted operations" << std::endl;
    
    // 函数重载删除
    std::cout << "  === Function Overload Deletion ===" << std::endl;
    void process(int) = delete;      // 禁用int版本
    void process(double) {}          // 允许double版本
    void process(const char*) {}     // 允许字符串版本
    
    // process(42);        // 编译错误
    process(3.14);         // 正确
    process("hello");      // 正确
}
```

### 3.3 do, while, for

```cpp
#include <iostream>
#include <vector>

void loop_keywords() {
    std::cout << "do, while, for Keywords:" << std::endl;
    
    // while循环
    std::cout << "  === while Loop ===" << std::endl;
    int i = 0;
    while (i < 5) {
        std::cout << "    while: i = " << i << std::endl;
        ++i;
    }
    
    // do-while循环
    std::cout << "  === do-while Loop ===" << std::endl;
    int j = 0;
    do {
        std::cout << "    do-while: j = " << j << std::endl;
        ++j;
    } while (j < 3);
    
    // for循环（传统）
    std::cout << "  === Traditional for Loop ===" << std::endl;
    for (int k = 0; k < 3; ++k) {
        std::cout << "    for: k = " << k << std::endl;
    }
    
    // 范围for循环 (C++11)
    std::cout << "  === Range-based for Loop (C++11) ===" << std::endl;
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (const auto& element : vec) {
        std::cout << "    range-for: element = " << element << std::endl;
    }
    
    // 基于初始化的for循环 (C++20)
    std::cout << "  === Init-statement for Loop (C++20) ===" << std::endl;
    for (int index = 0; auto& element : vec) {
        std::cout << "    init-for: vec[" << index++ << "] = " << element << std::endl;
    }
    
    // 嵌套循环和控制
    std::cout << "  === Nested Loops ===" << std::endl;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (x == 1 && y == 1) {
                std::cout << "    Breaking inner loop" << std::endl;
                break;
            }
            std::cout << "    x=" << x << ", y=" << y << std::endl;
        }
    }
}
```

### 3.4 dynamic_cast

```cpp
#include <iostream>
#include <typeinfo>

void dynamic_cast_keyword() {
    std::cout << "dynamic_cast Keyword:" << std::endl;
    
    // 基类和派生类
    class Base {
    public:
        virtual ~Base() = default;
        virtual void base_function() {
            std::cout << "    Base function called" << std::endl;
        }
    };
    
    class Derived : public Base {
    public:
        void derived_function() {
            std::cout << "    Derived function called" << std::endl;
        }
    };
    
    class OtherClass {
    public:
        virtual ~OtherClass() = default;
        void other_function() {
            std::cout << "    Other function called" << std::endl;
        }
    };
    
    // 向下转型 (Downcasting)
    std::cout << "  === Downcasting ===" << std::endl;
    Base* base_ptr = new Derived();
    Derived* derived_ptr = dynamic_cast<Derived*>(base_ptr);
    
    if (derived_ptr) {
        std::cout << "    Successfully cast to Derived*" << std::endl;
        derived_ptr->derived_function();
    } else {
        std::cout << "    Cast failed" << std::endl;
    }
    
    delete base_ptr;
    
    // 转型失败处理
    std::cout << "  === Cast Failure Handling ===" << std::endl;
    Base* base_ptr2 = new Base();
    Derived* derived_ptr2 = dynamic_cast<Derived*>(base_ptr2);
    
    if (derived_ptr2) {
        std::cout << "    Cast succeeded" << std::endl;
    } else {
        std::cout << "    Cast failed: base_ptr2 is not a Derived object" << std::endl;
    }
    
    delete base_ptr2;
    
    // 交叉转型 (Crosscasting)
    std::cout << "  === Crosscasting ===" << std::endl;
    // 需要虚继承或多继承场景才能演示
    
    // 引用的dynamic_cast
    std::cout << "  === Reference dynamic_cast ===" << std::endl;
    try {
        Derived derived_obj;
        Base& base_ref = derived_obj;
        Derived& derived_ref = dynamic_cast<Derived&>(base_ref);
        std::cout << "    Successfully cast reference" << std::endl;
        derived_ref.derived_function();
    } catch (const std::bad_cast& e) {
        std::cout << "    Cast failed with exception: " << e.what() << std::endl;
    }
    
    // 与typeid的配合使用
    std::cout << "  === With typeid ===" << std::endl;
    Derived derived_obj;
    Base& base_ref = derived_obj;
    
    if (typeid(base_ref) == typeid(Derived)) {
        std::cout << "    Type is Derived (using typeid)" << std::endl;
    }
}
```

### 3.5 enum, enum class

```cpp
#include <iostream>

void enum_keywords() {
    std::cout << "enum Keywords:" << std::endl;
    
    // 传统枚举
    std::cout << "  === Traditional enum ===" << std::endl;
    enum Color {
        RED,
        GREEN,
        BLUE
    };
    
    Color my_color = RED;
    std::cout << "    Traditional enum values:" << std::endl;
    std::cout << "      RED = " << RED << std::endl;
    std::cout << "      GREEN = " << GREEN << std::endl;
    std::cout << "      BLUE = " << BLUE << std::endl;
    std::cout << "    my_color = " << my_color << std::endl;
    
    // 作用域枚举 (C++11)
    std::cout << "  === Scoped enum (enum class) ===" << std::endl;
    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };
    
    Direction my_direction = Direction::NORTH;
    std::cout << "    Scoped enum requires scope resolution" << std::endl;
    // std::cout << Direction::NORTH << std::endl;  // 编译错误：无隐式转换
    
    // 指定底层类型
    std::cout << "  === Specified Underlying Type ===" << std::endl;
    enum class Status : unsigned char {
        OK = 0,
        ERROR = 1,
        WARNING = 2
    };
    
    Status my_status = Status::OK;
    std::cout << "    sizeof(Status): " << sizeof(Status) << " byte(s)" << std::endl;
    
    // 带值的枚举
    std::cout << "  === Enum with Values ===" << std::endl;
    enum Priority {
        LOW = 10,
        MEDIUM = 50,
        HIGH = 100
    };
    
    std::cout << "    Priority values:" << std::endl;
    std::cout << "      LOW = " << LOW << std::endl;
    std::cout << "      MEDIUM = " << MEDIUM << std::endl;
    std::cout << "      HIGH = " << HIGH << std::endl;
    
    // 连续枚举值
    std::cout << "  === Consecutive Enum Values ===" << std::endl;
    enum FileType {
        TEXT,      // 0
        BINARY,    // 1
        IMAGE,     // 2
        AUDIO      // 3
    };
    
    std::cout << "    Consecutive values:" << std::endl;
    std::cout << "      TEXT = " << TEXT << std::endl;
    std::cout << "      BINARY = " << BINARY << std::endl;
    std::cout << "      IMAGE = " << IMAGE << std::endl;
    std::cout << "      AUDIO = " << AUDIO << std::endl;
}
```

### 3.15 pre, post (C++26)

```cpp
#include <iostream>

void function_contract_keywords() {
    std::cout << "pre, post Keywords (C++26):" << std::endl;
    
    // 前置条件 (pre)
    std::cout << "  === Precondition (pre) ===" << std::endl;
    auto divide = [](double a, double b) pre(b != 0.0) -> double {
        return a / b;
    };
    
    std::cout << "    divide(10.0, 2.0) = " << divide(10.0, 2.0) << std::endl;
    
    // 后置条件 (post)
    std::cout << "  === Postcondition (post) ===" << std::endl;
    auto square = [](int x) post(result >= 0) -> int {
        return x * x;
    };
    
    std::cout << "    square(-5) = " << square(-5) << std::endl;
    
    // 组合使用
    std::cout << "  === Combined pre and post ===" << std::endl;
    auto safe_subscript = [](const std::vector<int>& vec, size_t index) 
                         pre(index < vec.size()) 
                         post(result >= 0) -> int {
        return vec[index];
    };
    
    std::vector<int> test_vec = {10, 20, 30};
    std::cout << "    safe_subscript(test_vec, 1) = " << safe_subscript(test_vec, 1) << std::endl;
    
    // 类成员函数合约
    std::cout << "  === Class Member Function Contracts ===" << std::endl;
    class BankAccount {
    private:
        double balance = 0.0;
        
    public:
        void deposit(double amount) 
            pre(amount > 0.0) 
            post(balance > old(balance)) {
            balance += amount;
        }
        
        void withdraw(double amount) 
            pre(amount > 0.0 && amount <= balance) 
            post(balance < old(balance)) {
            balance -= amount;
        }
        
        double get_balance() const 
            post(result >= 0.0) {
            return balance;
        }
    };
    
    BankAccount account;
    account.deposit(100.0);
    account.withdraw(50.0);
    std::cout << "    Account balance: " << account.get_balance() << std::endl;
}
```

## 4. R-Z 关键字详解

### 4.1 reinterpret_cast

```cpp
#include <iostream>
#include <cstdint>

void reinterpret_cast_keyword() {
    std::cout << "reinterpret_cast Keyword:" << std::endl;
    
    // 基本用法：类型重新解释
    std::cout << "  === Basic Usage ===" << std::endl;
    int value = 42;
    int* int_ptr = &value;
    
    // 将int*重新解释为char*
    char* char_ptr = reinterpret_cast<char*>(int_ptr);
    
    std::cout << "    Original int value: " << value << std::endl;
    std::cout << "    First byte: " << static_cast<int>(*char_ptr) << std::endl;
    
    // 指针到整数的转换
    std::cout << "  === Pointer to Integer ===" << std::endl;
    uintptr_t address = reinterpret_cast<uintptr_t>(int_ptr);
    std::cout << "    Pointer address: 0x" << std::hex << address << std::dec << std::endl;
    
    // 整数到指针的转换
    std::cout << "  === Integer to Pointer ===" << std::endl;
    int* restored_ptr = reinterpret_cast<int*>(address);
    std::cout << "    Restored value: " << *restored_ptr << std::endl;
    
    // 函数指针转换
    std::cout << "  === Function Pointer Conversion ===" << std::endl;
    void (*func_ptr)() = nullptr;
    void* void_ptr = reinterpret_cast<void*>(func_ptr);
    void (*restored_func_ptr)() = reinterpret_cast<void(*)()>(void_ptr);
    
    // 警告：危险操作示例
    std::cout << "  === Dangerous Operations (Educational) ===" << std::endl;
    double pi = 3.14159;
    long long* ll_ptr = reinterpret_cast<long long*>(&pi);
    std::cout << "    Double as long long: " << *ll_ptr << std::endl;
    
    // 与联合体的对比
    std::cout << "  === Comparison with Union ===" << std::endl;
    union {
        int i;
        float f;
    } converter;
    
    converter.i = 0x40490fdb;  // IEEE 754表示的3.14159
    std::cout << "    Union conversion: " << converter.f << std::endl;
}
```

### 4.2 return

```cpp
#include <iostream>
#include <optional>

void return_keyword() {
    std::cout << "return Keyword:" << std::endl;
    
    // 基本返回语句
    std::cout << "  === Basic Return ===" << std::endl;
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    
    std::cout << "    add(3, 4) = " << add(3, 4) << std::endl;
    
    // 早期返回
    std::cout << "  === Early Return ===" << std::endl;
    auto safe_divide = [](double a, double b) -> std::optional<double> {
        if (b == 0.0) {
            return std::nullopt;  // 提前返回
        }
        return a / b;
    };
    
    auto result1 = safe_divide(10.0, 2.0);
    if (result1) {
        std::cout << "    10.0 / 2.0 = " << *result1 << std::endl;
    }
    
    auto result2 = safe_divide(10.0, 0.0);
    if (!result2) {
        std::cout << "    Division by zero detected" << std::endl;
    }
    
    // 返回引用
    std::cout << "  === Return Reference ===" << std::endl;
    auto get_max = [](int& a, int& b) -> int& {
        return (a > b) ? a : b;
    };
    
    int x = 10, y = 20;
    int& max_ref = get_max(x, y);
    max_ref = 100;  // 修改原变量
    std::cout << "    After modification: x=" << x << ", y=" << y << std::endl;
    
    // 返回右值引用 (移动语义)
    std::cout << "  === Return Rvalue Reference ===" << std::endl;
    class Movable {
    public:
        std::string data;
        Movable(const std::string& str) : data(str) {}
        
        // 移动构造函数
        Movable(Movable&& other) noexcept : data(std::move(other.data)) {
            std::cout << "    Movable object moved" << std::endl;
        }
    };
    
    auto create_movable = [](const std::string& str) -> Movable {
        return Movable(str);  // NRVO或移动
    };
    
    Movable obj = create_movable("Hello");
    std::cout << "    Created movable object with data: " << obj.data << std::endl;
}
```

### 4.3 sizeof

```cpp
#include <iostream>
#include <vector>
#include <array>

void sizeof_keyword() {
    std::cout << "sizeof Keyword:" << std::endl;
    
    // 基本数据类型大小
    std::cout << "  === Basic Data Types ===" << std::endl;
    std::cout << "    sizeof(char): " << sizeof(char) << " byte(s)" << std::endl;
    std::cout << "    sizeof(short): " << sizeof(short) << " byte(s)" << std::endl;
    std::cout << "    sizeof(int): " << sizeof(int) << " byte(s)" << std::endl;
    std::cout << "    sizeof(long): " << sizeof(long) << " byte(s)" << std::endl;
    std::cout << "    sizeof(long long): " << sizeof(long long) << " byte(s)" << std::endl;
    std::cout << "    sizeof(float): " << sizeof(float) << " byte(s)" << std::endl;
    std::cout << "    sizeof(double): " << sizeof(double) << " byte(s)" << std::endl;
    std::cout << "    sizeof(long double): " << sizeof(long double) << " byte(s)" << std::endl;
    std::cout << "    sizeof(bool): " << sizeof(bool) << " byte(s)" << std::endl;
    
    // 指针大小
    std::cout << "  === Pointer Sizes ===" << std::endl;
    std::cout << "    sizeof(void*): " << sizeof(void*) << " byte(s)" << std::endl;
    std::cout << "    sizeof(int*): " << sizeof(int*) << " byte(s)" << std::endl;
    std::cout << "    sizeof(char*): " << sizeof(char*) << " byte(s)" << std::endl;
    
    // 数组大小
    std::cout << "  === Array Sizes ===" << std::endl;
    int arr[10];
    std::cout << "    sizeof(int[10]): " << sizeof(arr) << " byte(s)" << std::endl;
    std::cout << "    Array length: " << sizeof(arr) / sizeof(arr[0]) << std::endl;
    
    // 结构体大小
    std::cout << "  === Struct Sizes ===" << std::endl;
    struct SimpleStruct {
        char a;
        int b;
        double c;
    };
    
    std::cout << "    sizeof(SimpleStruct): " << sizeof(SimpleStruct) << " byte(s)" << std::endl;
    
    // 空类大小
    std::cout << "  === Empty Class Size ===" << std::endl;
    struct Empty {};
    std::cout << "    sizeof(Empty): " << sizeof(Empty) << " byte(s)" << std::endl;
    
    // 标准库容器大小
    std::cout << "  === Standard Library ===" << std::endl;
    std::cout << "    sizeof(std::vector<int>): " << sizeof(std::vector<int>) << " byte(s)" << std::endl;
    std::array<int, 5> std_arr;
    std::cout << "    sizeof(std::array<int, 5>): " << sizeof(std_arr) << " byte(s)" << std::endl;
    
    // sizeof vs strlen
    std::cout << "  === sizeof vs strlen ===" << std::endl;
    const char* c_str = "Hello";
    char c_arr[] = "Hello";
    
    std::cout << "    const char* c_str = \"Hello\";" << std::endl;
    std::cout << "    sizeof(c_str): " << sizeof(c_str) << " (pointer size)" << std::endl;
    std::cout << "    strlen(c_str): " << std::strlen(c_str) << " (string length)" << std::endl;
    
    std::cout << "    char c_arr[] = \"Hello\";" << std::endl;
    std::cout << "    sizeof(c_arr): " << sizeof(c_arr) << " (array size)" << std::endl;
    std::cout << "    strlen(c_arr): " << std::strlen(c_arr) << " (string length)" << std::endl;
}
```

### 4.4 static

```cpp
#include <iostream>

void static_keyword() {
    std::cout << "static Keyword:" << std::endl;
    
    // 函数内的static变量
    std::cout << "  === Static Local Variables ===" << std::endl;
    auto counter = []() -> int {
        static int count = 0;  // 只初始化一次
        return ++count;
    };
    
    std::cout << "    First call: " << counter() << std::endl;
    std::cout << "    Second call: " << counter() << std::endl;
    std::cout << "    Third call: " << counter() << std::endl;
    
    // 类内的static成员
    std::cout << "  === Static Class Members ===" << std::endl;
    class StaticExample {
    private:
        static int static_count;
        int instance_count;
        
    public:
        StaticExample() : instance_count(++static_count) {}
        
        static int get_static_count() {
            return static_count;
        }
        
        int get_instance_count() const {
            return instance_count;
        }
    };
    
    int StaticExample::static_count = 0;  // 定义静态成员
    
    StaticExample obj1, obj2, obj3;
    std::cout << "    Static count: " << StaticExample::get_static_count() << std::endl;
    std::cout << "    obj1 instance count: " << obj1.get_instance_count() << std::endl;
    std::cout << "    obj2 instance count: " << obj2.get_instance_count() << std::endl;
    std::cout << "    obj3 instance count: " << obj3.get_instance_count() << std::endl;
    
    // 文件作用域的static变量
    std::cout << "  === Static File Scope ===" << std::endl;
    // static int file_static_var = 42;  // 只在当前文件可见
    
    // 静态函数
    std::cout << "  === Static Functions ===" << std::endl;
    // static void helper_function() { /* 只在当前文件可见 */ }
    
    // C++17内联变量
    std::cout << "  === Inline Static Variables (C++17) ===" << std::endl;
    class InlineStatic {
    public:
        inline static int inline_static_var = 100;  // 内联静态变量
    };
    
    std::cout << "    InlineStatic::inline_static_var = " << InlineStatic::inline_static_var << std::endl;
}
```

### 4.5 struct

```cpp
#include <iostream>

void struct_keyword() {
    std::cout << "struct Keyword:" << std::endl;
    
    // 基本结构体
    std::cout << "  === Basic Struct ===" << std::endl;
    struct Point {
        double x;
        double y;
        
        // 结构体中可以有函数
        double distance_from_origin() const {
            return std::sqrt(x * x + y * y);
        }
    };
    
    Point p{3.0, 4.0};
    std::cout << "    Point p{3.0, 4.0}" << std::endl;
    std::cout << "    Distance from origin: " << p.distance_from_origin() << std::endl;
    
    // POD类型
    std::cout << "  === POD (Plain Old Data) ===" << std::endl;
    struct PODStruct {
        int a;
        double b;
        char c;
    };
    
    PODStruct pod{1, 2.5, 'X'};
    std::cout << "    POD struct members: " << pod.a << ", " << pod.b << ", " << pod.c << std::endl;
    
    // 位域结构体
    std::cout << "  === Bit Fields ===" << std::endl;
    struct BitField {
        unsigned int flag1 : 1;  // 1位
        unsigned int flag2 : 1;  // 1位
        unsigned int value : 4;  // 4位
        unsigned int type : 2;   // 2位
    };
    
    BitField bf{};
    bf.flag1 = 1;
    bf.flag2 = 0;
    bf.value = 10;
    bf.type = 2;
    
    std::cout << "    sizeof(BitField): " << sizeof(BitField) << " byte(s)" << std::endl;
    
    // C风格结构体与C++类的对比
    std::cout << "  === C-style vs C++ Class ===" << std::endl;
    struct CStyle {
        int x;
        int y;
    };  // 默认public访问
    
    class CPPStyle {
    public:  // 需要显式指定
        int x;
        int y;
    };
    
    CStyle c_style{1, 2};
    CPPStyle cpp_style{3, 4};
    
    std::cout << "    C-style struct: " << c_style.x << ", " << c_style.y << std::endl;
    std::cout << "    C++ class: " << cpp_style.x << ", " << cpp_style.y << std::endl;
    
    // 结构化绑定 (C++17)
    std::cout << "  === Structured Bindings (C++17) ===" << std::endl;
    Point point{5.0, 12.0};
    auto [x_coord, y_coord] = point;  // 结构化绑定
    std::cout << "    Structured binding: x=" << x_coord << ", y=" << y_coord << std::endl;
}
```

### 4.6 template

```cpp
#include <iostream>
#include <vector>
#include <type_traits>

void template_keyword() {
    std::cout << "template Keyword:" << std::endl;
    
    // 函数模板
    std::cout << "  === Function Templates ===" << std::endl;
    template<typename T>
    T max_value(T a, T b) {
        return (a > b) ? a : b;
    }
    
    std::cout << "    max_value(3, 7) = " << max_value(3, 7) << std::endl;
    std::cout << "    max_value(3.14, 2.71) = " << max_value(3.14, 2.71) << std::endl;
    
    // 类模板
    std::cout << "  === Class Templates ===" << std::endl;
    template<typename T>
    class Stack {
    private:
        std::vector<T> elements;
        
    public:
        void push(const T& item) {
            elements.push_back(item);
        }
        
        T pop() {
            if (elements.empty()) {
                throw std::runtime_error("Stack is empty");
            }
            T result = elements.back();
            elements.pop_back();
            return result;
        }
        
        bool empty() const {
            return elements.empty();
        }
    };
    
    Stack<int> int_stack;
    int_stack.push(1);
    int_stack.push(2);
    int_stack.push(3);
    std::cout << "    Popped from int_stack: " << int_stack.pop() << std::endl;
    
    // 模板特化
    std::cout << "  === Template Specialization ===" << std::endl;
    template<typename T>
    class Specialized {
    public:
        void print() {
            std::cout << "    Generic template" << std::endl;
        }
    };
    
    // 特化版本
    template<>
    class Specialized<int> {
    public:
        void print() {
            std::cout << "    Specialized for int" << std::endl;
        }
    };
    
    Specialized<double> generic;
    Specialized<int> specialized;
    generic.print();
    specialized.print();
    
    // 可变参数模板 (C++11)
    std::cout << "  === Variadic Templates (C++11) ===" << std::endl;
    template<typename T>
    void print(T&& t) {
        std::cout << t << std::endl;
    }
    
    template<typename T, typename... Args>
    void print(T&& t, Args&&... args) {
        std::cout << t << " ";
        print(args...);
    }
    
    std::cout << "    Variadic print: ";
    print(1, 2.5, "Hello", 'C');
    
    // 别名模板 (C++11)
    std::cout << "  === Alias Templates (C++11) ===" << std::endl;
    template<typename T>
    using Vec = std::vector<T>;
    
    Vec<int> int_vec = {1, 2, 3, 4, 5};
    std::cout << "    Alias template Vec<int> size: " << int_vec.size() << std::endl;
}
```

### 4.7 this

```cpp
#include <iostream>
#include <memory>

void this_keyword() {
    std::cout << "this Keyword:" << std::endl;
    
    // 基本用法
    std::cout << "  === Basic Usage ===" << std::endl;
    class BasicThis {
    private:
        int value;
        
    public:
        BasicThis(int v) : value(v) {}
        
        void set_value(int value) {
            this->value = value;  // 区分成员变量和参数
        }
        
        int get_value() const {
            return this->value;
        }
        
        BasicThis& get_reference() {
            return *this;  // 返回对象自身的引用
        }
    };
    
    BasicThis obj(42);
    obj.set_value(100);
    std::cout << "    obj.get_value(): " << obj.get_value() << std::endl;
    
    // 链式调用
    std::cout << "  === Chained Calls ===" << std::endl;
    class Chainable {
    private:
        int data = 0;
        
    public:
        Chainable& set_data(int value) {
            this->data = value;
            return *this;  // 返回*this以支持链式调用
        }
        
        Chainable& increment() {
            ++this->data;
            return *this;
        }
        
        Chainable& multiply(int factor) {
            this->data *= factor;
            return *this;
        }
        
        int get_data() const {
            return this->data;
        }
    };
    
    Chainable chain_obj;
    int result = chain_obj.set_data(5).increment().multiply(3).get_data();
    std::cout << "    Chained operations result: " << result << std::endl;
    
    // this指针的类型
    std::cout << "  === this Pointer Type ===" << std::endl;
    class ThisType {
    public:
        void regular_function() {
            std::cout << "    Regular function: this type is ThisType*" << std::endl;
        }
        
        void const_function() const {
            std::cout << "    Const function: this type is const ThisType*" << std::endl;
        }
        
        void volatile_function() volatile {
            std::cout << "    Volatile function: this type is volatile ThisType*" << std::endl;
        }
        
        void const_volatile_function() const volatile {
            std::cout << "    Const volatile function: this type is const volatile ThisType*" << std::endl;
        }
    };
    
    ThisType this_obj;
    const ThisType const_this_obj;
    
    this_obj.regular_function();
    this_obj.const_function();
    const_this_obj.const_function();
    
    // 智能指针与this
    std::cout << "  === Smart Pointers and this ===" << std::endl;
    class EnableSharedFromThis : public std::enable_shared_from_this<EnableSharedFromThis> {
    public:
        std::shared_ptr<EnableSharedFromThis> get_shared() {
            return shared_from_this();  // 安全地获取shared_ptr
        }
    };
    
    auto shared_obj = std::make_shared<EnableSharedFromThis>();
    auto another_shared = shared_obj->get_shared();
    std::cout << "    Shared pointer use count: " << shared_obj.use_count() << std::endl;
}
```

### 4.8 thread_local (C++11)

```cpp
#include <iostream>
#include <thread>
#include <vector>

void thread_local_keyword() {
    std::cout << "thread_local Keyword (C++11):" << std::endl;
    
    // 线程局部存储变量
    thread_local int thread_counter = 0;
    
    // 工作函数
    auto worker = [](int id) {
        for (int i = 0; i < 3; ++i) {
            ++thread_counter;
            std::cout << "    Thread " << id << ": counter = " << thread_counter << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    
    // 创建多个线程
    std::cout << "  === Thread Local Storage ===" << std::endl;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(worker, i);
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // 主线程的thread_local变量
    std::cout << "    Main thread counter: " << thread_counter << std::endl;
    
    // 类成员的线程局部存储
    std::cout << "  === Thread Local Class Members ===" << std::endl;
    class ThreadLocalClass {
    public:
        thread_local static int static_counter;
        thread_local int instance_counter = 0;
        
        void increment() {
            ++static_counter;
            ++instance_counter;
        }
        
        void print_counters(int thread_id) {
            std::cout << "    Thread " << thread_id 
                      << ": static_counter = " << static_counter
                      << ", instance_counter = " << instance_counter << std::endl;
        }
    };
    
    thread_local int ThreadLocalClass::static_counter = 0;
    
    // 多线程测试类
    auto class_worker = [](int id) {
        ThreadLocalClass obj;
        for (int i = 0; i < 2; ++i) {
            obj.increment();
            obj.print_counters(id);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    };
    
    std::vector<std::thread> class_threads;
    for (int i = 0; i < 2; ++i) {
        class_threads.emplace_back(class_worker, i);
    }
    
    for (auto& t : class_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    // thread_local与静态变量的对比
    std::cout << "  === Comparison with Static ===" << std::endl;
    static int static_counter = 0;
    
    auto compare_worker = [](int id) {
        ++static_counter;  // 所有线程共享
        ++thread_counter;  // 每个线程独立
        std::cout << "    Thread " << id 
                  << ": static = " << static_counter
                  << ", thread_local = " << thread_counter << std::endl;
    };
    
    std::vector<std::thread> compare_threads;
    for (int i = 0; i < 2; ++i) {
        compare_threads.emplace_back(compare_worker, i);
    }
    
    for (auto& t : compare_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
```

### 4.9 try, catch, throw

```cpp
#include <iostream>
#include <exception>
#include <stdexcept>

void exception_keywords() {
    std::cout << "try, catch, throw Keywords:" << std::endl;
    
    // 基本异常处理
    std::cout << "  === Basic Exception Handling ===" << std::endl;
    try {
        throw std::runtime_error("Something went wrong!");
    } catch (const std::runtime_error& e) {
        std::cout << "    Caught runtime_error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "    Caught exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "    Caught unknown exception" << std::endl;
    }
    
    // 多重异常捕获
    std::cout << "  === Multiple Exception Types ===" << std::endl;
    auto test_exceptions = [](int type) {
        switch (type) {
            case 1:
                throw std::invalid_argument("Invalid argument");
            case 2:
                throw std::out_of_range("Out of range");
            case 3:
                throw 42;  // 抛出整数
            default:
                throw std::logic_error("Logic error");
        }
    };
    
    for (int i = 1; i <= 4; ++i) {
        try {
            test_exceptions(i);
        } catch (const std::invalid_argument& e) {
            std::cout << "    Caught invalid_argument: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "    Caught out_of_range: " << e.what() << std::endl;
        } catch (int value) {
            std::cout << "    Caught integer: " << value << std::endl;
        } catch (const std::exception& e) {
            std::cout << "    Caught std::exception: " << e.what() << std::endl;
        }
    }
    
    // 异常规范和noexcept
    std::cout << "  === Exception Specifications ===" << std::endl;
    auto noexcept_function = []() noexcept {
        // 这个函数承诺不抛出异常
        return 42;
    };
    
    auto throwing_function = []() {
        throw std::runtime_error("This function throws");
    };
    
    std::cout << "    noexcept_function(): " << noexcept_function() << std::endl;
    
    try {
        throwing_function();
    } catch (const std::runtime_error& e) {
        std::cout << "    Caught from throwing_function(): " << e.what() << std::endl;
    }
    
    // 异常安全保证
    std::cout << "  === Exception Safety ===" << std::endl;
    class ExceptionSafe {
    private:
        int* data;
        
    public:
        ExceptionSafe(int size) {
            data = new int[size];  // 可能抛出std::bad_alloc
            // 如果这里抛出异常，析构函数不会被调用
        }
        
        ~ExceptionSafe() {
            delete[] data;
        }
        
        // 提供强异常安全保证
        void safe_operation() {
            int* temp = new int[100];  // 临时资源
            // 执行可能抛出异常的操作
            // ...
            delete[] data;  // 释放旧资源
            data = temp;    // 提交更改
        }
    };
    
    try {
        ExceptionSafe obj(10);
        // obj.safe_operation();  // 演示异常安全
    } catch (const std::exception& e) {
        std::cout << "    Exception in ExceptionSafe: " << e.what() << std::endl;
    }
    
    // 嵌套异常 (C++11)
    std::cout << "  === Nested Exceptions (C++11) ===" << std::endl;
    try {
        try {
            throw std::runtime_error("Inner exception");
        } catch (...) {
            std::throw_with_nested(std::logic_error("Outer exception"));
        }
    } catch (const std::exception& e) {
        std::cout << "    Caught: " << e.what() << std::endl;
        try {
            std::rethrow_if_nested(e);
        } catch (const std::exception& nested) {
            std::cout << "    Nested: " << nested.what() << std::endl;
        }
    }
}
```

### 4.10 using

```cpp
#include <iostream>
#include <vector>
#include <iosfwd>

void using_keyword() {
    std::cout << "using Keyword:" << std::endl;
    
    // 类型别名
    std::cout << "  === Type Aliases ===" << std::endl;
    using IntVector = std::vector<int>;
    using StringPair = std::pair<std::string, std::string>;
    
    IntVector vec = {1, 2, 3, 4, 5};
    StringPair pair{"key", "value"};
    
    std::cout << "    IntVector size: " << vec.size() << std::endl;
    std::cout << "    StringPair: " << pair.first << " = " << pair.second << std::endl;
    
    // 命名空间别名
    std::cout << "  === Namespace Aliases ===" << std::endl;
    namespace fs = std::filesystem;  // 假设包含<filesystem>
    namespace chrono = std::chrono;
    
    // using声明
    std::cout << "  === using Declarations ===" << std::endl;
    {
        using std::cout;
        using std::endl;
        
        cout << "    Using declarations simplify code" << endl;
        cout << "    No need to prefix with std::" << endl;
    }
    
    // 在类中使用using
    std::cout << "  === using in Classes ===" << std::endl;
    class Base {
    public:
        virtual void method() {
            std::cout << "    Base::method()" << std::endl;
        }
    };
    
    class Derived : public Base {
    public:
        using Base::method;  // 继承基类方法到public
        void method(int x) {
            std::cout << "    Derived::method(int): " << x << std::endl;
        }
    };
    
    Derived d;
    d.method();      // 调用基类方法
    d.method(42);    // 调用派生类方法
    
    // 模板别名 (C++11)
    std::cout << "  === Template Aliases (C++11) ===" << std::endl;
    template<typename T>
    using Vec = std::vector<T>;
    
    template<typename T>
    using Ptr = std::shared_ptr<T>;
    
    Vec<double> double_vec = {1.1, 2.2, 3.3};
    Ptr<std::string> string_ptr = std::make_shared<std::string>("Hello");
    
    std::cout << "    Vec<double> size: " << double_vec.size() << std::endl;
    std::cout << "    Ptr<std::string> value: " << *string_ptr << std::endl;
    
    // 函数类型别名
    std::cout << "  === Function Type Aliases ===" << std::endl;
    using BinaryOperation = int(*)(int, int);
    using VoidFunction = void(*)();
    
    auto add = [](int a, int b) -> int { return a + b; };
    auto print_hello = []() { std::cout << "    Hello from lambda!" << std::endl; };
    
    BinaryOperation op = +[](int a, int b) { return a + b; };  // 获取函数指针
    VoidFunction hello = +[]() { std::cout << "    Hello!" << std::endl; };
    
    std::cout << "    Function result: " << op(3, 4) << std::endl;
    hello();
}
```

## 5. 具有特殊含义的标识符

### 5.1 final, override (C++11)

```cpp
#include <iostream>

void special_identifiers() {
    std::cout << "Special Identifiers:" << std::endl;
    
    // override关键字
    std::cout << "  === override Keyword ===" << std::endl;
    class Base {
    public:
        virtual void virtual_method() {
            std::cout << "    Base::virtual_method()" << std::endl;
        }
        
        virtual void another_method(int x) {
            std::cout << "    Base::another_method(" << x << ")" << std::endl;
        }
    };
    
    class Derived : public Base {
    public:
        void virtual_method() override {  // 明确表示这是重写
            std::cout << "    Derived::virtual_method()" << std::endl;
        }
        
        void another_method(int x) override {
            std::cout << "    Derived::another_method(" << x << ")" << std::endl;
        }
        
        // void typo_method() override {  // 编译错误：基类中没有此方法
        //     std::cout << "    This won't compile" << std::endl;
        // }
    };
    
    Derived d;
    Base& b = d;
    b.virtual_method();      // 输出: Derived::virtual_method()
    b.another_method(42);    // 输出: Derived::another_method(42)
    
    // final关键字
    std::cout << "  === final Keyword ===" << std::endl;
    class FinalBase {
    public:
        virtual void cannot_override() final {
            std::cout << "    This method cannot be overridden" << std::endl;
        }
        
        virtual void can_override() {
            std::cout << "    This method can be overridden" << std::endl;
        }
    };
    
    class FinalDerived : public FinalBase {
    public:
        // void cannot_override() {  // 编译错误：final方法不能被重写
        //     std::cout << "    Attempt to override final method" << std::endl;
        // }
        
        void can_override() override {
            std::cout << "    Successfully overridden non-final method" << std::endl;
        }
    };
    
    // final类
    class FinalClass final {
    public:
        void method() {
            std::cout << "    Method in final class" << std::endl;
        }
    };
    
    // class CannotInherit : public FinalClass {  // 编译错误：final类不能被继承
    // };
    
    FinalClass final_obj;
    final_obj.method();
}
```

### 5.2 import, module (C++20)

```cpp
#include <iostream>

void module_keywords() {
    std::cout << "import, module Keywords (C++20):" << std::endl;
    
    // 模块系统介绍
    std::cout << "  === Module System Overview ===" << std::endl;
    std::cout << "    // math_module.cppm" << std::endl;
    std::cout << "    export module math;" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    export int add(int a, int b) {" << std::endl;
    std::cout << "        return a + b;" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    export double pi = 3.14159;" << std::endl;
    
    std::cout << "    // main.cpp" << std::endl;
    std::cout << "    import math;" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    int main() {" << std::endl;
    std::cout << "        int result = add(3, 4);" << std::endl;
    std::cout << "        return 0;" << std::endl;
    std::cout << "    }" << std::endl;
    
    // 模块分区
    std::cout << "  === Module Partitions ===" << std::endl;
    std::cout << "    // math.cppm" << std::endl;
    std::cout << "    export module math;" << std::endl;
    std::cout << "    export import :interface;" << std::endl;
    std::cout << "    export import :implementation;" << std::endl;
    
    std::cout << "    // math-interface.cppm" << std::endl;
    std::cout << "    export module math:interface;" << std::endl;
    std::cout << "    export int add(int a, int b);" << std::endl;
    
    std::cout << "    // math-implementation.cppm" << std::endl;
    std::cout << "    module math:implementation;" << std::endl;
    std::cout << "    int add(int a, int b) { return a + b; }" << std::endl;
    
    // 全局模块片段
    std::cout << "  === Global Module Fragment ===" << std::endl;
    std::cout << "    module;" << std::endl;
    std::cout << "    #include <vector>" << std::endl;
    std::cout << "    #include <string>" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    export module my_module;" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    export void process(const std::vector<std::string>& items);" << std::endl;
    
    // 私有模块片段
    std::cout << "  === Private Module Fragment ===" << std::endl;
    std::cout << "    export module utils;" << std::endl;
    std::cout << "    " << std::cout << "    export int public_function();" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    module :private;" << std::endl;
    std::cout << "    " << std::cout << "    int helper_function() { return 42; }" << std::endl;
    std::cout << "    int public_function() { return helper_function(); }" << std::endl;
}
```

### 5.3 transaction_safe, transaction_safe_dynamic (C++26)

```cpp
#include <iostream>

void transaction_keywords() {
    std::cout << "transaction_safe, transaction_safe_dynamic Keywords (C++26):" << std::endl;
    
    // 事务安全函数
    std::cout << "  === Transaction Safe Functions ===" << std::endl;
    transaction_safe int safe_function(int x, int y) {
        return x + y;  // 无副作用，事务安全
    }
    
    // 非事务安全函数
    std::cout << "  === Non-Transaction Safe Functions ===" << std::endl;
    int global_counter = 0;
    
    int unsafe_function(int x) {
        ++global_counter;  // 有副作用，非事务安全
        return x * 2;
    }
    
    // 事务安全动态
    std::cout << "  === Transaction Safe Dynamic ===" << std::endl;
    transaction_safe_dynamic int dynamic_safe_function(int x) {
        // 运行时检查是否事务安全
        if (x > 0) {
            return x * 2;  // 事务安全路径
        } else {
            // 这里可能有非事务安全操作
            return -1;
        }
    }
    
    // 事务内存示例
    std::cout << "  === Transactional Memory Example ===" << std::endl;
    std::cout << "    atomic {" << std::endl;
    std::cout << "        // 事务块" << std::endl;
    std::cout << "        int result = safe_function(3, 4);" << std::endl;
    std::cout << "        std::cout << \"Result: \" << result << std::endl;" << std::endl;
    std::cout << "    }" << std::endl;
    
    // 事务安全限制
    std::cout << "  === Transaction Safety Restrictions ===" << std::endl;
    std::cout << "    - 不能有I/O操作" << std::endl;
    std::cout << "    - 不能有 volatile 访问" << std::endl;
    std::cout << "    - 不能调用非事务安全函数" << std::endl;
    std::cout << "    - 不能有无限循环" << std::endl;
    std::cout << "    - 不能有异常抛出（除非捕获）" << std::endl;
}
```

## 6. 预处理器关键字

### 6.1 #define, #undef

```cpp
#include <iostream>

void preprocessor_directives() {
    std::cout << "Preprocessor Directives:" << std::endl;
    
    // #define 宏定义
    std::cout << "  === #define Macro ===" << std::endl;
    #define PI 3.14159
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
    #define SQUARE(x) ((x) * (x))
    
    std::cout << "    PI = " << PI << std::endl;
    std::cout << "    MAX(3, 7) = " << MAX(3, 7) << std::endl;
    std::cout << "    SQUARE(5) = " << SQUARE(5) << std::endl;
    
    // 字符串化和连接
    std::cout << "  === Stringizing and Concatenation ===" << std::endl;
    #define STRINGIFY(x) #x
    #define CONCAT(a, b) a##b
    
    std::cout << "    STRINGIFY(hello) = " << STRINGIFY(hello) << std::endl;
    
    int xy = 100;
    std::cout << "    CONCAT(x, y) = " << CONCAT(x, y) << std::endl;
    
    // #undef 取消宏定义
    std::cout << "  === #undef Directive ===" << std::endl;
    #undef PI
    #undef MAX
    
    // 条件编译
    std::cout << "  === Conditional Compilation ===" << std::endl;
    #define DEBUG 1
    #define VERSION 2
    
    #if DEBUG
        std::cout << "    Debug mode enabled" << std::endl;
    #else
        std::cout << "    Release mode" << std::endl;
    #endif
    
    #if VERSION == 1
        std::cout << "    Version 1" << std::endl;
    #elif VERSION == 2
        std::cout << "    Version 2" << std::endl;
    #else
        std::cout << "    Unknown version" << std::endl;
    #endif
    
    // 防止重复包含
    std::cout << "  === Include Guards ===" << std::endl;
    std::cout << "    #ifndef MY_HEADER_H" << std::endl;
    std::cout << "    #define MY_HEADER_H" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    // header content" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    #endif // MY_HEADER_H" << std::endl;
    
    // 预定义宏
    std::cout << "  === Predefined Macros ===" << std::endl;
    std::cout << "    __FILE__: " << __FILE__ << std::endl;
    std::cout << "    __LINE__: " << __LINE__ << std::endl;
    std::cout << "    __DATE__: " << __DATE__ << std::endl;
    std::cout << "    __TIME__: " << __TIME__ << std::endl;
    std::cout << "    __cplusplus: " << __cplusplus << std::endl;
}
```

### 6.2 #include, #pragma

```cpp
#include <iostream>

void include_pragma_directives() {
    std::cout << "#include, #pragma Directives:" << std::endl;
    
    // #include 指令
    std::cout << "  === #include Directive ===" << std::endl;
    std::cout << "    #include <iostream>     // 系统头文件" << std::endl;
    std::cout << "    #include \"myheader.h\"   // 用户头文件" << std::endl;
    std::cout << "    #include <vector>" << std::endl;
    std::cout << "    #include <string>" << std::endl;
    
    // 包含路径搜索顺序
    std::cout << "  === Include Search Order ===" << std::endl;
    std::cout << "    - 当前目录" << std::endl;
    std::cout << "    - 编译器包含路径" << std::endl;
    std::cout << "    - 系统包含路径" << std::endl;
    
    // #pragma 指令
    std::cout << "  === #pragma Directive ===" << std::endl;
    
    // 编译器特定指令
    #pragma once  // 非标准但广泛支持
    
    #pragma message("Compiling this file...")
    
    // 警告控制
    #pragma warning(disable: 4996)  // MSVC禁用特定警告
    
    // 优化控制
    #pragma optimize("", off)
    // 需要优化的代码
    #pragma optimize("", on)
    
    // 区域控制
    #pragma region MyRegion
    // 代码区域
    #pragma endregion MyRegion
    
    // 打包控制
    #pragma pack(push, 1)
    struct PackedStruct {
        char a;
        int b;
    };
    #pragma pack(pop)
    
    std::cout << "    sizeof(PackedStruct): " << sizeof(PackedStruct) << std::endl;
    
    // 其他常见pragma
    std::cout << "  === Common #pragma Uses ===" << std::endl;
    std::cout << "    #pragma once          // 防止重复包含" << std::endl;
    std::cout << "    #pragma warning       // 警告控制" << std::endl;
    std::cout << "    #pragma comment       // 链接器指令" << std::endl;
    std::cout << "    #pragma pack          // 结构体打包" << std::endl;
    std::cout << "    #pragma optimize      // 优化控制" << std::endl;
}
```

## 7. 保留标识符

### 7.1 标准库保留名称

```cpp
#include <iostream>
#include <vector>

void reserved_identifiers() {
    std::cout << "Reserved Identifiers:" << std::endl;
    
    // 标准库保留名称
    std::cout << "  === Standard Library Reserved Names ===" << std::endl;
    std::cout << "    - 所有标准库名称（std命名空间）" << std::endl;
    std::cout << "    - 标准宏名称（如__cplusplus）" << std::endl;
    std::cout << "    - 标准类型名称（如size_t, nullptr_t）" << std::endl;
    
    // 实现保留名称
    std::cout << "  === Implementation Reserved Names ===" << std::endl;
    std::cout << "    - 双下划线开头：__reserved" << std::endl;
    std::cout << "    - 下划线加大写字母：_Reserved" << std::endl;
    std::cout << "    - 全局命名空间中的保留名称" << std::endl;
    
    // 示例：不应使用的名称
    std::cout << "  === Names to Avoid ===" << std::endl;
    // int __reserved;     // 错误：双下划线保留
    // int _Reserved;      // 错误：下划线加大写字母保留
    // int vector;         // 错误：与标准库冲突
    
    // 安全的使用方式
    std::cout << "  === Safe Naming Practices ===" << std::endl;
    int my_vector;          // 正确：添加前缀
    int local_var;          // 正确：描述性名称
    int customer_count;     // 正确：清晰明确
    
    // 命名空间使用
    std::cout << "  === Namespace Usage ===" << std::endl;
    namespace my_lib {
        class vector {      // 正确：在自定义命名空间中
        public:
            void push_back(int) {}
        };
    }
    
    my_lib::vector my_vec;
    std::vector<int> std_vec;
    
    // C++17嵌套命名空间
    std::cout << "  === Nested Namespaces (C++17) ===" << std::endl;
    namespace my::company::lib {
        class utility {
        public:
            static void helper() {}
        };
    }
    
    my::company::lib::utility::helper();
}
```

### 7.2 未来保留字

```cpp
#include <iostream>

void future_reserved_words() {
    std::cout << "Future Reserved Words:" << std::endl;
    
    // C++标准中保留的未来关键字
    std::cout << "  === Future C++ Keywords ===" << std::endl;
    std::cout << "    - 为语言扩展保留的标识符" << std::endl;
    std::cout << "    - 可能在将来版本中成为关键字" << std::endl;
    
    // 当前保留的标识符
    std::cout << "  === Currently Reserved Identifiers ===" << std::endl;
    std::cout << "    - 带有特定前缀的名称" << std::endl;
    std::cout << "    - 标准库实现细节" << std::endl;
    
    // 避免使用的模式
    std::cout << "  === Patterns to Avoid ===" << std::endl;
    std::cout << "    // 避免使用可能成为关键字的名称" << std::endl;
    std::cout << "    int concept;     // C++20 中 concept 成为关键字" << std::endl;
    std::cout << "    int requires;    // C++20 中 requires 成为关键字" << std::endl;
    std::cout << "    int module;      // C++20 中 module 成为关键字" << std::endl;
    
    // 兼容性考虑
    std::cout << "  === Compatibility Considerations ===" << std::endl;
    std::cout << "    - 考虑代码的向后兼容性" << std::endl;
    std::cout << "    - 避免使用过于通用的名称" << std::endl;
    std::cout << "    - 使用命名空间防止冲突" << std::endl;
    
    // 实际建议
    std::cout << "  === Practical Advice ===" << std::endl;
    std::cout << "    - 使用描述性、具体的名称" << std::endl;
    std::cout << "    - 添加项目特定前缀" << std::endl;
    std::cout << "    - 遵循命名约定" << std::endl;
    std::cout << "    - 定期检查代码中的潜在冲突" << std::endl;
}
```

## 8. 标准命名空间

### 8.1 std 命名空间

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void std_namespace() {
    std::cout << "std Namespace:" << std::endl;
    
    // std命名空间概述
    std::cout << "  === std Namespace Overview ===" << std::endl;
    std::cout << "    - 包含所有C++标准库组件" << std::endl;
    std::cout << "    - 防止与用户定义名称冲突" << std::endl;
    std::cout << "    - 组织标准库功能" << std::endl;
    
    // 常用标准库组件
    std::cout << "  === Common Standard Library Components ===" << std::endl;
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9};
    std::sort(numbers.begin(), numbers.end());
    
    std::string message = "Hello, C++!";
    std::cout << "    Sorted numbers: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    std::cout << "    Message: " << message << std::endl;
    
    // using namespace std;
    std::cout << "  === using namespace std ===" << std::endl;
    {
        using namespace std;
        vector<string> items = {"apple", "banana", "cherry"};
        cout << "    Items: ";
        for (const auto& item : items) {
            cout << item << " ";
        }
        cout << endl;
    }
    
    // 选择性using声明
    std::cout << "  === Selective using Declarations ===" << std::endl;
    using std::cout;
    using std::endl;
    using std::string;
    
    string name = "World";
    cout << "    Hello, " << name << "!" << endl;
    
    // 内联命名空间 (C++11)
    std::cout << "  === Inline Namespaces (C++11) ===" << std::endl;
    namespace lib {
        inline namespace v1 {
            void api() { cout << "    v1 API" << endl; }
        }
        namespace v2 {
            void api() { cout << "    v2 API" << endl; }
        }
    }
    
    lib::api();  // 使用v1版本（默认）
    lib::v2::api();  // 显式使用v2版本
    
    // 命名空间别名
    std::cout << "  === Namespace Aliases ===" << std::endl;
    namespace fs = std::filesystem;
    namespace chrono = std::chrono;
    
    std::cout << "    Namespace aliases simplify long namespace names" << std::endl;
}
```

### 8.2 标准库头文件

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <numeric>

void standard_headers() {
    std::cout << "Standard Library Headers:" << std::endl;
    
    // 容器头文件
    std::cout << "  === Container Headers ===" << std::endl;
    std::vector<int> vec = {1, 2, 3};
    std::map<std::string, int> map = {{"one", 1}, {"two", 2}};
    std::set<int> set = {1, 2, 3, 4, 5};
    
    std::cout << "    Vector size: " << vec.size() << std::endl;
    std::cout << "    Map size: " << map.size() << std::endl;
    std::cout << "    Set size: " << set.size() << std::endl;
    
    // 算法头文件
    std::cout << "  === Algorithm Headers ===" << std::endl;
    std::sort(vec.begin(), vec.end());
    auto it = std::find(vec.begin(), vec.end(), 2);
    
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "    Sum: " << sum << std::endl;
    
    // 智能指针
    std::cout << "  === Smart Pointers ===" << std::endl;
    auto ptr = std::make_unique<int>(42);
    auto shared = std::make_shared<std::string>("Hello");
    
    std::cout << "    Unique ptr: " << *ptr << std::endl;
    std::cout << "    Shared ptr: " << *shared << std::endl;
    
    // 函数对象
    std::cout << "  === Function Objects ===" << std::endl;
    std::function<int(int, int)> add = [](int a, int b) { return a + b; };
    std::cout << "    Function result: " << add(3, 4) << std::endl;
    
    // 现代C++头文件
    std::cout << "  === Modern C++ Headers ===" << std::endl;
    std::cout << "    #include <filesystem>    // C++17 文件系统" << std::endl;
    std::cout << "    #include <variant>       // C++17 变体" << std::endl;
    std::cout << "    #include <optional>      // C++17 可选值" << std::endl;
    std::cout << "    #include <any>           // C++17 任意类型" << std::endl;
    std::cout << "    #include <span>          // C++20 跨度" << std::endl;
    std::cout << "    #include <format>        // C++20 格式化" << std::endl;
}
```

## 9. 关键字使用最佳实践

### 9.1 现代C++特性使用指南

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#include <variant>

void modern_cpp_best_practices() {
    std::cout << "Modern C++ Best Practices:" << std::endl;
    
    // auto关键字最佳实践
    std::cout << "  === auto Keyword Best Practices ===" << std::endl;
    auto numbers = std::vector<int>{1, 2, 3, 4, 5};  // 清晰类型推导
    const auto& first = numbers.front();             // 正确使用引用和const
    
    // 避免过度使用auto
    int explicit_type = 42;  // 明确类型有时更好
    
    // 智能指针最佳实践
    std::cout << "  === Smart Pointer Best Practices ===" << std::endl;
    auto unique = std::make_unique<int>(42);      // 使用make_unique
    auto shared = std::make_shared<std::string>("Hello");  // 使用make_shared
    
    // 避免裸new和delete
    // int* raw = new int(42);  // 避免
    
    // constexpr和consteval
    std::cout << "  === constexpr/consteval Best Practices ===" << std::endl;
    constexpr int compile_time_value = 42;  // 编译时常量
    constexpr int square(int x) { return x * x; }  // 编译时函数
    
    // 移动语义最佳实践
    std::cout << "  === Move Semantics Best Practices ===" << std::endl;
    class Resource {
    public:
        Resource() = default;
        Resource(Resource&&) noexcept = default;
        Resource& operator=(Resource&&) noexcept = default;
        
        // 禁用拷贝
        Resource(const Resource&) = delete;
        Resource& operator=(const Resource&) = delete;
    };
    
    Resource res1;
    Resource res2 = std::move(res1);  // 明确移动
    
    // 异常安全
    std::cout << "  === Exception Safety ===" << std::endl;
    try {
        // 资源获取即初始化(RAII)
        auto resource = std::make_unique<Resource>();
        // 操作可能抛出异常
    } catch (const std::exception& e) {
        std::cout << "    Exception caught: " << e.what() << std::endl;
    }
    
    // 现代类型使用
    std::cout << "  === Modern Type Usage ===" << std::endl;
    std::optional<int> maybe_value;        // 可能没有值
    std::variant<int, std::string> value;  // 类型安全的联合
    
    value = 42;
    value = "hello";
    
    // 范围for循环
    std::cout << "  === Range-based for loops ===" << std::endl;
    for (const auto& num : numbers) {
        std::cout << "    " << num << " ";
    }
    std::cout << std::endl;
}
```

### 9.2 性能和安全考虑

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

void performance_safety_considerations() {
    std::cout << "Performance and Safety Considerations:" << std::endl;
    
    // 内存安全
    std::cout << "  === Memory Safety ===" << std::endl;
    std::cout << "    - 使用智能指针代替裸指针" << std::endl;
    std::cout << "    - 避免内存泄漏" << std::endl;
    std::cout << "    - 使用RAII管理资源" << std::endl;
    
    auto safe_ptr = std::make_unique<int>(42);
    // 不需要手动delete
    
    // 类型安全
    std::cout << "  === Type Safety ===" << std::endl;
    std::cout << "    - 使用enum class代替传统enum" << std::endl;
    std::cout << "    - 使用override确保正确重写" << std::endl;
    std::cout << "    - 使用constexpr确保编译时计算" << std::endl;
    
    enum class Color { RED, GREEN, BLUE };
    Color color = Color::RED;
    // 更安全，不会隐式转换
    
    // 性能优化
    std::cout << "  === Performance Optimization ===" << std::endl;
    
    // 移动语义优化
    std::vector<std::string> large_vector;
    std::string large_string = "very long string...";
    
    // 使用移动而不是拷贝
    large_vector.push_back(std::move(large_string));
    
    // 编译时计算
    constexpr int array_size = 100;
    std::array<int, array_size> compile_time_array;
    
    // 并发安全
    std::cout << "  === Concurrency Safety ===" << std::endl;
    std::cout << "    - 使用std::atomic进行原子操作" << std::endl;
    std::cout << "    - 使用std::mutex保护共享数据" << std::endl;
    std::cout << "    - 避免数据竞争" << std::endl;
    
    // 异常安全保证
    std::cout << "  === Exception Safety Guarantees ===" << std::endl;
    std::cout << "    - 基本保证：不泄漏资源" << std::endl;
    std::cout << "    - 强保证：操作成功或状态不变" << std::endl;
    std::cout << "    - 不抛保证：承诺不抛出异常" << std::endl;
    
    // 资源管理
    std::cout << "  === Resource Management ===" << std::endl;
    class FileHandler {
    private:
        std::FILE* file;
        
    public:
        explicit FileHandler(const char* filename) 
            : file(std::fopen(filename, "r")) {
            if (!file) throw std::runtime_error("File open failed");
        }
        
        ~FileHandler() {
            if (file) std::fclose(file);
        }
        
        // 禁用拷贝
        FileHandler(const FileHandler&) = delete;
        FileHandler& operator=(const FileHandler&) = delete;
        
        // 允许移动
        FileHandler(FileHandler&& other) noexcept : file(other.file) {
            other.file = nullptr;
        }
    };
}
```

## 10. 总结

### 10.1 C++关键字演进总结

```cpp
#include <iostream>
#include <vector>
#include <map>

void keyword_evolution_summary() {
    std::cout << "C++ Keyword Evolution Summary:" << std::endl;
    
    // 各版本新增关键字
    std::cout << "  === Keyword Additions by Version ===" << std::endl;
    
    std::map<std::string, std::vector<std::string>> version_keywords = {
        {"C++98/03", {"bool", "catch", "class", "const_cast", "delete", "dynamic_cast", 
                     "explicit", "export", "false", "friend", "mutable", "namespace", 
                     "new", "operator", "private", "protected", "public", "reinterpret_cast", 
                     "static_cast", "template", "this", "throw", "true", "try", "typeid", 
                     "typename", "using", "virtual", "wchar_t"}},
        
        {"C++11", {"alignas", "alignof", "char16_t", "char32_t", "constexpr", "decltype", 
                  "noexcept", "nullptr", "static_assert", "thread_local", "auto", 
                  "override", "final"}},
        
        {"C++14", {"constexpr"}},  // 扩展constexpr功能
        
        {"C++17", {"inline"}},     // 内联变量
        
        {"C++20", {"concept", "requires", "consteval", "constinit", "co_await", "co_return", 
                  "co_yield", "char8_t", "import", "module"}},
        
        {"C++23", {"constexpr"}},  // 进一步扩展constexpr
        
        {"C++26", {"contract_assert", "pre", "post", "transaction_safe", 
                  "transaction_safe_dynamic"}}
    };
    
    for (const auto& [version, keywords] : version_keywords) {
        std::cout << "    " << version << ": ";
        for (const auto& keyword : keywords) {
            std::cout << keyword << " ";
        }
        std::cout << std::endl;
    }
    
    // 现代C++编程范式
    std::cout << "  === Modern C++ Programming Paradigms ===" << std::endl;
    std::cout << "    - RAII (Resource Acquisition Is Initialization)" << std::endl;
    std::cout << "    - Move Semantics" << std::endl;
    std::cout << "    - Constexpr Programming" << std::endl;
    std::cout << "    - Modules" << std::endl;
    std::cout << "    - Coroutines" << std::endl;
    std::cout << "    - Concepts" << std::endl;
    
    // 未来发展趋势
    std::cout << "  === Future Trends ===" << std::endl;
    std::cout << "    - 更多编译时计算" << std::endl;
    std::cout << "    - 更好的并发支持" << std::endl;
    std::cout << "    - 改进的模块系统" << std::endl;
    std::cout << "    - 增强的静态分析" << std::endl;
    std::cout << "    - 更好的跨平台支持" << std::endl;
}
```

### 10.2 学习资源和建议

```cpp
#include <iostream>
#include <vector>

void learning_resources() {
    std::cout << "Learning Resources and Recommendations:" << std::endl;
    
    // 官方标准文档
    std::cout << "  === Official Standards ===" << std::endl;
    std::cout << "    - ISO/IEC 14882:2020 (C++20)" << std::endl;
    std::cout << "    - ISO/IEC 14882:2017 (C++17)" << std::endl;
    std::cout << "    - ISO/IEC 14882:2014 (C++14)" << std::endl;
    std::cout << "    - ISO/IEC 14882:2011 (C++11)" << std::endl;
    
    // 推荐书籍
    std::cout << "  === Recommended Books ===" << std::endl;
    std::cout << "    - \"The C++ Programming Language\" by Bjarne Stroustrup" << std::endl;
    std::cout << "    - \"Effective Modern C++\" by Scott Meyers" << std::endl;
    std::cout << "    - \"C++ Primer\" by Stanley Lippman" << std::endl;
    std::cout << "    - \"A Tour of C++\" by Bjarne Stroustrup" << std::endl;
    
    // 在线资源
    std::cout << "  === Online Resources ===" << std::endl;
    std::cout << "    - cppreference.com" << std::endl;
    std::cout << "    - isocpp.org" << std::endl;
    std::cout << "    - Stack Overflow" << std::endl;
    std::cout << "    - C++ Weekly (YouTube)" << std::endl;
    
    // 学习路径建议
    std::cout << "  === Learning Path ===" << std::endl;
    std::cout << "    1. C++基础语法和关键字" << std::endl;
    std::cout << "    2. 面向对象编程" << std::endl;
    std::cout << "    3. 标准库使用" << std::endl;
    std::cout << "    4. 现代C++特性" << std::endl;
    std::cout << "    5. 高级主题（模板元编程、并发等）" << std::endl;
    
    // 实践建议
    std::cout << "  === Practice Recommendations ===" << std::endl;
    std::cout << "    - 从简单项目开始" << std::endl;
    std::cout << "    - 阅读优秀开源代码" << std::endl;
    std::cout << "    - 参与代码审查" << std::endl;
    std::cout << "    - 使用现代开发工具" << std::endl;
    std::cout << "    - 关注C++标准演进" << std::endl;
    
    // 工具推荐
    std::cout << "  === Development Tools ===" << std::endl;
    std::cout << "    - Clang/LLVM" << std::endl;
    std::cout << "    - GCC" << std::endl;
    std::cout << "    - Visual Studio" << std::endl;
    std::cout << "    - CLion" << std::endl;
    std::cout << "    - vcpkg/conan (包管理)" << std::endl;
}

int main() {
    std::cout << "=== C++ Keywords Complete Reference ===" << std::endl;
    std::cout << std::endl;
    
    // 调用各个演示函数
    special_identifiers();
    module_keywords();
    transaction_keywords();
    preprocessor_directives();
    include_pragma_directives();
    reserved_identifiers();
    future_reserved_words();
    std_namespace();
    standard_headers();
    modern_cpp_best_practices();
    performance_safety_considerations();
    keyword_evolution_summary();
    learning_resources();
    
    std::cout << std::endl;
    std::cout << "=== End of C++ Keywords Reference ===" << std::endl;
    
    return 0;
}
```

### 10.3 C++ 关键字功能分类表（按C++标准版本）

| 功能类别 | 关键字 | C++98 | C++11 | C++14 | C++17 | C++20 | C++23 | 功能描述 |
|---------|--------|-------|-------|-------|-------|-------|-------|---------|
| **基本数据类型** | `void` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 无类型或空类型 |
| | `bool` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 布尔类型 |
| | `char` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 字符类型 |
| | `int` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 整型 |
| | `float` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 单精度浮点型 |
| | `double` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 双精度浮点型 |
| | `wchar_t` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 宽字符类型 |
| | `char8_t` | | | | | ✓ | ✓ | UTF-8字符类型(C++20) |
| | `char16_t` | | ✓ | ✓ | ✓ | ✓ | ✓ | UTF-16字符类型(C++11) |
| | `char32_t` | | ✓ | ✓ | ✓ | ✓ | ✓ | UTF-32字符类型(C++11) |
| **类型限定符** | `const` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 定义不可修改的对象 |
| | `volatile` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 防止编译器优化 |
| | `mutable` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 允许在const对象中修改 |
| **存储类说明符** | `auto` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 自动类型推导(C++11后) |
| | `register` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 建议使用寄存器存储(弃用) |
| | `static` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 静态存储期或内部链接 |
| | `extern` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 外部链接声明 |
| | `thread_local` | | ✓ | ✓ | ✓ | ✓ | ✓ | 线程局部存储(C++11) |
| **控制流语句** | `if` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 条件语句 |
| | `else` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 条件语句的替代分支 |
| | `switch` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 多分支选择语句 |
| | `case` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | switch语句的分支标签 |
| | `default` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | switch语句的默认分支 |
| | `for` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 循环语句 |
| | `while` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 循环语句 |
| | `do` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 循环语句 |
| | `break` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 跳出循环或switch |
| | `continue` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 继续下一次循环 |
| | `goto` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 无条件跳转 |
| | `return` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 函数返回值 |
| **异常处理** | `try` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 异常处理块 |
| | `catch` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 异常捕获 |
| | `throw` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 抛出异常 |
| | `noexcept` | | ✓ | ✓ | ✓ | ✓ | ✓ | 指定函数不抛出异常(C++11) |
| **面向对象编程** | `class` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 类定义 |
| | `struct` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 结构体定义 |
| | `union` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 联合体定义 |
| | `enum` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 枚举定义 |
| | `enum class` | | ✓ | ✓ | ✓ | ✓ | ✓ | 强类型枚举(C++11) |
| | `public` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 公有访问权限 |
| | `protected` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 保护访问权限 |
| | `private` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 私有访问权限 |
| | `virtual` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 虚函数 |
| | `override` | | ✓ | ✓ | ✓ | ✓ | ✓ | 显式重写虚函数(C++11) |
| | `final` | | ✓ | ✓ | ✓ | ✓ | ✓ | 禁止派生或重写(C++11) |
| | `friend` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 友元声明 |
| | `explicit` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 禁止隐式转换 |
| | `this` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 当前对象指针 |
| **运算符与类型操作** | `sizeof` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 获取类型或对象大小 |
| | `typeid` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 获取类型信息 |
| | `alignof` | | ✓ | ✓ | ✓ | ✓ | ✓ | 获取对齐要求(C++11) |
| | `decltype` | | ✓ | ✓ | ✓ | ✓ | ✓ | 获取表达式类型(C++11) |
| | `constexpr` | | ✓ | ✓ | ✓ | ✓ | ✓ | 编译时常量表达式(C++11) |
| | `consteval` | | | | | ✓ | ✓ | 立即函数(C++20) |
| | `constinit` | | | | | ✓ | ✓ | 编译时初始化(C++20) |
| | `typeof` | | | | | | ✓ | 获取表达式类型(C++23) |
| **模板编程** | `template` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 模板定义 |
| | `typename` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 模板类型参数 |
| | `requires` | | | | | ✓ | ✓ | 概念约束(C++20) |
| | `concept` | | | | | ✓ | ✓ | 概念定义(C++20) |
| **内存管理** | `new` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 动态内存分配 |
| | `delete` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 动态内存释放 |
| | `operator` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 运算符重载 |
| **命名空间** | `namespace` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 命名空间定义 |
| | `using` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 使用声明或指令 |
| **类型转换** | `static_cast` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 静态类型转换 |
| | `dynamic_cast` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 动态类型转换 |
| | `const_cast` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 常量性转换 |
| | `reinterpret_cast` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 重新解释转换 |
| **协程支持** | `co_await` | | | | | ✓ | ✓ | 协程等待表达式(C++20) |
| | `co_yield` | | | | | ✓ | ✓ | 协程产出值(C++20) |
| | `co_return` | | | | | ✓ | ✓ | 协程返回(C++20) |
| **模块系统** | `import` | | | | | ✓ | ✓ | 模块导入(C++20) |
| | `module` | | | | | ✓ | ✓ | 模块定义(C++20) |
| | `export` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 导出声明(模块中) |
| **属性说明符** | `alignas` | | ✓ | ✓ | ✓ | ✓ | ✓ | 指定对齐要求(C++11) |
| | `asm` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 内联汇编 |
| | `attribute` | | | | | | ✓ | 通用属性语法(C++23) |
| **其他** | `typedef` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 类型别名 |
| | `nullptr` | | ✓ | ✓ | ✓ | ✓ | ✓ | 空指针常量(C++11) |
| | `static_assert` | | ✓ | ✓ | ✓ | ✓ | ✓ | 静态断言(C++11) |
| | `true` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 布尔真值 |
| | `false` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 布尔假值 |
| | `and` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 逻辑与(替代&&) |
| | `or` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 逻辑或(替代\|\|) |
| | `not` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 逻辑非(替代!) |
| | `bitand` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 位与(替代&) |
| | `bitor` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 位或(替代\|) |
| | `xor` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 位异或(替代^) |
| | `compl` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 位取反(替代~) |
| | `and_eq` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 位与赋值(替代&=) |
| | `or_eq` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 位或赋值(替代\|=) |
| | `xor_eq` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 位异或赋值(替代^=) |
| | `not_eq` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | 不等于(替代!=) |

**各版本关键字数量统计**：

- C++98: 63个关键字
- C++11: 新增13个关键字（总计76个）
- C++14: 无新增关键字
- C++17: 无新增关键字
- C++20: 新增20+个关键字（总计96+个）
- C++23: 预计新增5+个关键字（总计100+个）

**C++ 关键字演进特点**：

- C++98: 奠定了C++的基础，包含从C继承的关键字和C++特有的关键字
- C++11: 重大更新，引入了现代C++特性，如类型推导、移动语义、并发支持等
- C++14: 主要是对C++11的完善和补充，没有新增关键字
- C++17: 引入了结构化绑定、if constexpr等特性，但主要通过现有关键字实现
- C++20: 重大更新，引入了模块、概念、协程等重大特性，新增大量关键字
- C++23: 预计将进一步扩展语言特性，特别是模块系统和元编程能力

**说明**：

- 此表格按照关键字的主要功能进行分类，某些关键字可能具有多重功能
- C++23标准仍在制定中，部分关键字可能有所调整
- 预处理器指令（如`#define`、`#include`等）未包含在此表中
- 某些关键字在不同版本中可能有语义上的扩展或变化
- 替代标记（如`and`、`or`等）在C++98中已存在，但通常不被广泛使用

这个完整的C++关键字详解文档涵盖了从基础到高级的所有关键字，包括：

- A-C关键字：alignas, alignof, auto, bool, break, case, char系列, class, const系列
- D-P关键字：decltype, delete, default, do-while-for, dynamic_cast, enum, final, override
- R-Z关键字：reinterpret_cast, return, sizeof, static, struct, template, this, thread_local, try-catch-throw, using
- 特殊标识符：final, override, import, module, transaction_safe等
- 预处理器关键字：#define, #undef, #include, #pragma等
- 保留标识符和标准命名空间

每个关键字都配有详细的代码示例和解释，涵盖了从C++98到C++26的各种特性。文档结构清晰，内容全面，可以作为学习和参考C++关键字的重要资源。
