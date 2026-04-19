# C++ 作用域（Scope）完全详解

  [1. 作用域基本概念](#1-作用域基本概念)

  [2. 通用作用域规则](#2-通用作用域规则)

  [3. 块作用域（Block Scope）](#3-块作用域block-scope)

  [4. 函数参数作用域（Function Parameter Scope）](#4-函数参数作用域function-parameter-scope)

  [5. Lambda 作用域（Lambda Scope）](#5-lambda-作用域lambda-scope)

  [6. 命名空间作用域（Namespace Scope）](#6-命名空间作用域namespace-scope)

  [7. 类作用域（Class Scope）](#7-类作用域class-scope)

  [8. 枚举作用域（Enumeration Scope）](#8-枚举作用域enumeration-scope)

  [9. 模板参数作用域（Template Parameter Scope）](#9-模板参数作用域template-parameter-scope)

  [10. 合约断言作用域（Contract Assertion Scope）](#10-合约断言作用域contract-assertion-scope)

  [11. 声明点规则](#11-声明点规则)

  [12. 作用域查找与解析](#12-作用域查找与解析)

  [13. 最佳实践](#13-最佳实践)

  [14. 标准参考](#14-标准参考)

  [15. 总结](#15-总结)

C++ 作用域是程序中声明可见性和生命周期的重要概念，它决定了程序中各个声明在何处可以被访问和使用。与 C 语言相比，C++ 作用域机制更加复杂和丰富，支持面向对象、模板、Lambda 表达式等现代特性。

## 1. 作用域基本概念

### 1.1 作用域定义

作用域是程序中声明可见的区域，即声明可以被使用和访问的代码范围。在 C++ 中，每个声明都有其特定的作用域，这决定了它在程序中的可见性和生命周期。

```cpp
#include <iostream>

// 全局作用域
int global_var = 100;

void function_example() {
    // 块作用域
    int local_var = 200;
    
    std::cout << "Global variable: " << global_var << std::endl;
    std::cout << "Local variable: " << local_var << std::endl;
}

int main() {
    function_example();
    
    // global_var 在此处可见
    std::cout << "In main, global_var = " << global_var << std::endl;
    
    // local_var 在此处不可见（超出作用域）
    // std::cout << "In main, local_var = " << local_var << std::endl; // 编译错误
    
    return 0;
}
```

### 1.2 作用域层次结构

C++ 程序具有层次化的作用域结构：

```cpp
#include <iostream>

// 全局作用域
int global_var = 100;

namespace Outer {  // 命名空间作用域
    int namespace_var = 200;
    
    namespace Inner {  // 嵌套命名空间作用域
        int nested_var = 300;
        
        class MyClass {  // 类作用域
        public:
            static int static_member;  // 类作用域
            int instance_member = 400; // 类作用域
            
            void member_function() {   // 函数作用域
                int function_var = 500; // 块作用域
                
                if (true) {
                    int block_var = 600; // 嵌套块作用域
                    std::cout << "Block var: " << block_var << std::endl;
                }
                
                std::cout << "Function var: " << function_var << std::endl;
            }
        };
        
        int MyClass::static_member = 700;
    }
}

int main() {
    Outer::Inner::MyClass obj;
    obj.member_function();
    
    std::cout << "Global var: " << global_var << std::endl;
    std::cout << "Namespace var: " << Outer::namespace_var << std::endl;
    std::cout << "Nested var: " << Outer::Inner::nested_var << std::endl;
    std::cout << "Static member: " << Outer::Inner::MyClass::static_member << std::endl;
    
    return 0;
}
```

## 2. 通用作用域规则

### 2.1 作用域层次关系

每个作用域都存在于另一个作用域中，形成层次结构：

```cpp
#include <iostream>

// 全局作用域
int x;  // 程序点 X

{
    // 作用域 S
    {
        // 作用域 T
        int y;  // 程序点 Y
        std::cout << "In innermost scope" << std::endl;
    }
}

void scope_hierarchy_example() {
    std::cout << "Scope hierarchy demonstration" << std::endl;
    
    // 在程序点 Y：
    // 1. 全局作用域、作用域 S 和作用域 T 都是外围作用域
    // 2. 作用域 T 是立即作用域
    // 3. 作用域 S 是作用域 T 的父作用域
    // 4. 全局作用域是作用域 S 的父作用域
}
```

### 2.2 父作用域概念

父作用域是包含当前作用域且不是模板参数作用域的最小作用域：

```cpp
#include <iostream>

// 全局作用域（父作用域）
int global_var = 100;

namespace MyNamespace {  // 命名空间作用域（父作用域）
    int namespace_var = 200;
    
    class MyClass {  // 类作用域
    public:
        static int static_member;  // 父作用域是命名空间作用域
        int instance_member = 300;
        
        void member_function() {   // 函数作用域
            int local_var = 400;   // 父作用域是函数作用域
            
            // 块作用域
            {
                int block_var = 500;  // 父作用域是函数作用域
                std::cout << "Block var: " << block_var << std::endl;
            }
        }
    };
    
    int MyClass::static_member = 600;
}

void parent_scope_example() {
    MyNamespace::MyClass obj;
    obj.member_function();
    
    std::cout << "Global var: " << global_var << std::endl;
    std::cout << "Namespace var: " << MyNamespace::namespace_var << std::endl;
}
```

## 3. 块作用域（Block Scope）

### 3.1 块作用域定义

块作用域由以下语句引入：

- 选择语句 (if, switch)
- 迭代语句 (for, range-for, while, do-while)
- 处理器 (catch)
- 复合语句（大括号包围的代码块）

```cpp
#include <iostream>
#include <vector>

void block_scope_examples() {
    int outer_var = 100;  // 外层块作用域
    
    // if 语句引入块作用域
    if (true) {
        int if_var = 200;  // if 块作用域
        std::cout << "If var: " << if_var << std::endl;
        std::cout << "Outer var: " << outer_var << std::endl;
    }
    
    // switch 语句引入块作用域
    switch (outer_var) {
        case 100: {
            int switch_var = 300;  // switch 块作用域
            std::cout << "Switch var: " << switch_var << std::endl;
            break;
        }
    }
    
    // for 循环引入块作用域
    for (int i = 0; i < 3; i++) {  // i 具有 for 循环块作用域
        int loop_var = i * 100;
        std::cout << "Loop var: " << loop_var << std::endl;
    }
    
    // range-for 循环引入块作用域（C++11）
    std::vector<int> vec = {1, 2, 3};
    for (const auto& item : vec) {  // item 具有 range-for 块作用域
        std::cout << "Range item: " << item << std::endl;
    }
    
    // while 循环引入块作用域
    int counter = 0;
    while (counter < 2) {
        int while_var = counter * 50;
        std::cout << "While var: " << while_var << std::endl;
        counter++;
    }
    
    // do-while 循环引入块作用域
    int do_counter = 0;
    do {
        int do_var = do_counter * 25;
        std::cout << "Do var: " << do_var << std::endl;
        do_counter++;
    } while (do_counter < 2);
}
```

### 3.2 块变量与函数声明

块作用域中的变量称为块变量（局部变量）：

```cpp
#include <iostream>

// 全局变量
int global_counter = 0;

void block_variables_example() {
    int local_var = 100;  // 块变量
    
    // 在块作用域中声明函数（需要 extern）
    extern void external_function();  // 声明在全局作用域中
    
    // C++20 模块中的限制
    // export module my_module;  // 模块声明
    // 在块作用域中不能声明附加到命名模块的函数
    
    if (true) {
        int inner_var = 200;  // 内层块变量
        std::cout << "Inner var: " << inner_var << std::endl;
        std::cout << "Local var: " << local_var << std::endl;
    }
    
    // inner_var 在此处不可见
    // std::cout << "Inner var: " << inner_var << std::endl; // 编译错误
}

// 外部函数定义
void external_function() {
    std::cout << "External function called" << std::endl;
}
```

### 3.3 作用域冲突规则

块作用域中的声明可能与父作用域中的声明产生冲突：

```cpp
#include <iostream>

void scope_conflict_example() {
    int x = 100;  // 外层变量
    
    if (int condition_var = 42) {  // if 语句中声明变量
        // int condition_var = 50;  // 错误：重复声明
        std::cout << "Condition var: " << condition_var << std::endl;
        std::cout << "X: " << x << std::endl;
    }
    
    // for 循环中的变量声明冲突示例
    for (int i = 0; i < 3; i++) {
        // int i = 10;  // 错误：重复声明
        std::cout << "Loop i: " << i << std::endl;
    }
}

void function_parameter_conflict() {
    int param = 100;  // 函数参数名冲突示例
    
    // void inner_function(int param) {  // 错误：在块作用域中声明函数参数
    //     extern int param;  // 错误：与函数参数冲突
    // }
}
```

## 4. 函数参数作用域（Function Parameter Scope）

### 4.1 函数参数作用域定义

函数参数作用域包含函数参数声明，并根据函数类型有不同的范围：

```cpp
#include <iostream>

// 函数声明中的参数作用域（函数原型）
void function_prototype(int param1, double param2);

// 函数定义中的参数作用域
void function_definition(int param1, double param2) {
    // param1 和 param2 在整个函数体中可见
    std::cout << "Param1: " << param1 << ", Param2: " << param2 << std::endl;
    
    // 在嵌套块中也可见
    {
        std::cout << "In nested block - Param1: " << param1 << std::endl;
    }
}

// Lambda 表达式中的参数作用域（C++11）
auto lambda_example = [](int lambda_param) {
    // lambda_param 在整个 Lambda 体中可见
    std::cout << "Lambda param: " << lambda_param << std::endl;
    
    // 嵌套块中也可见
    {
        std::cout << "In lambda nested block - Param: " << lambda_param << std::endl;
    }
};

// 类模板推导指南中的参数作用域（C++17）
template<typename T>
struct MyTemplate {
    T value;
    MyTemplate(T v) : value(v) {}
};

// 推导指南
template<typename U>
MyTemplate(U) -> MyTemplate<U>;  // U 的作用域在推导指南中

void function_parameter_scope_example() {
    function_definition(10, 3.14);
    lambda_example(42);
    
    MyTemplate obj(100);  // 使用推导指南
    std::cout << "Template value: " << obj.value << std::endl;
}
```

### 4.2 复杂参数作用域示例

```cpp
#include <iostream>
#include <concepts>  // C++20

// C++20 requires 表达式中的参数作用域
template<typename T>
concept Addable = requires(T a, T b) {
    a + b;  // a 和 b 的作用域在 requires 表达式中
};

void complex_parameter_scopes() {
    // 函数模板参数作用域
    auto lambda = [](auto&& param) {
        std::cout << "Generic lambda param" << std::endl;
    };
    
    lambda(42);
    lambda(3.14);
    
    // 可变参数模板参数作用域
    auto variadic_lambda = [](auto... params) {
        std::cout << "Variadic lambda with " << sizeof...(params) << " parameters" << std::endl;
    };
    
    variadic_lambda(1, 2, 3, 4, 5);
}
```

## 5. Lambda 作用域（Lambda Scope）

### 5.1 Lambda 作用域定义

每个 Lambda 表达式引入一个 Lambda 作用域，从捕获列表之后开始，到函数体结束：

```cpp
#include <iostream>
#include <functional>

void lambda_scope_examples() {
    int outer_var = 100;
    
    // 基本 Lambda 作用域
    auto basic_lambda = [outer_var]() {
        // outer_var 在 Lambda 作用域中可见
        std::cout << "Basic lambda - outer var: " << outer_var << std::endl;
        
        // Lambda 内部声明的变量
        int lambda_local = 200;
        std::cout << "Lambda local: " << lambda_local << std::endl;
    };
    
    basic_lambda();
    
    // 带初始化捕获的 Lambda 作用域（C++14）
    auto init_capture_lambda = [x = 42, y = outer_var * 2]() {
        // x 和 y 在 Lambda 作用域中声明和初始化
        std::cout << "Init capture - x: " << x << ", y: " << y << std::endl;
        
        // Lambda 内部变量
        int inner_var = 300;
        std::cout << "Inner var: " << inner_var << std::endl;
    };
    
    init_capture_lambda();
    
    // 嵌套 Lambda 作用域
    auto outer_lambda = [outer_var]() {
        std::cout << "Outer lambda - outer var: " << outer_var << std::endl;
        
        auto inner_lambda = [outer_var]() {
            // 内层 Lambda 可以访问外层捕获的变量
            std::cout << "Inner lambda - outer var: " << outer_var << std::endl;
        };
        
        inner_lambda();
    };
    
    outer_lambda();
}
```

### 5.2 Lambda 捕获与作用域

```cpp
#include <iostream>
#include <memory>

void lambda_capture_scopes() {
    int value = 100;
    std::unique_ptr<int> ptr = std::make_unique<int>(200);
    
    // 值捕获
    auto value_lambda = [value]() {
        std::cout << "Value capture: " << value << std::endl;
        // value 在 Lambda 作用域中是副本
    };
    
    // 引用捕获
    auto ref_lambda = [&value]() {
        std::cout << "Reference capture: " << value << std::endl;
        value += 10;  // 修改原始变量
    };
    
    // 移动捕获（C++14）
    auto move_lambda = [ptr = std::move(ptr)]() {
        std::cout << "Move capture: " << *ptr << std::endl;
        // ptr 在 Lambda 作用域中
    };
    
    value_lambda();
    ref_lambda();
    move_lambda();
    
    std::cout << "Modified value: " << value << std::endl;
}
```

## 6. 命名空间作用域（Namespace Scope）

### 6.1 命名空间作用域定义

每个命名空间定义引入一个命名空间作用域，包含该命名空间的所有声明：

```cpp
#include <iostream>

// 全局命名空间作用域
int global_var = 100;

namespace Outer {  // Outer 命名空间作用域
    int outer_var = 200;
    
    void outer_function() {
        std::cout << "Outer function" << std::endl;
    }
    
    namespace Inner {  // Inner 命名空间作用域
        int inner_var = 300;
        
        class InnerClass {
        public:
            static int static_member;
            void inner_method() {
                std::cout << "Inner method" << std::endl;
            }
        };
        
        int InnerClass::static_member = 400;
    }
    
    // 命名空间中的类重新声明
    class OuterClass;  // 前向声明
}

// 命名空间的分离定义
namespace Outer {
    // 可以在不同位置添加更多声明
    int additional_var = 500;
    
    class OuterClass {
    public:
        void method() {
            std::cout << "Outer class method" << std::endl;
        }
    };
}

void namespace_scope_example() {
    std::cout << "Global var: " << global_var << std::endl;
    std::cout << "Outer var: " << Outer::outer_var << std::endl;
    std::cout << "Inner var: " << Outer::Inner::inner_var << std::endl;
    std::cout << "Static member: " << Outer::Inner::InnerClass::static_member << std::endl;
    
    Outer::outer_function();
    Outer::Inner::InnerClass inner_obj;
    inner_obj.inner_method();
    
    Outer::OuterClass outer_obj;
    outer_obj.method();
}
```

### 6.2 命名空间作用域的扩展

```cpp
#include <iostream>

namespace First {
    int first_var = 100;
    
    void first_function() {
        std::cout << "First function" << std::endl;
    }
}

// 命名空间的扩展定义
namespace First {
    int second_var = 200;  // 添加到 First 命名空间
    
    void second_function() {
        std::cout << "Second function" << std::endl;
    }
}

// 内联命名空间（C++11）
namespace Library {
    inline namespace Version1 {
        void version_function() {
            std::cout << "Version 1 function" << std::endl;
        }
    }
}

// 嵌套命名空间定义（C++17）
namespace A::B::C {
    int nested_value = 300;
    
    void nested_function() {
        std::cout << "Nested function" << std::endl;
    }
}

void extended_namespace_example() {
    std::cout << "First var: " << First::first_var << std::endl;
    std::cout << "Second var: " << First::second_var << std::endl;
    
    First::first_function();
    First::second_function();
    
    Library::version_function();  // 通过内联命名空间直接访问
    
    std::cout << "Nested value: " << A::B::C::nested_value << std::endl;
    A::B::C::nested_function();
}
```

## 7. 类作用域（Class Scope）

### 7.1 类作用域定义

每个类或类模板声明引入一个类作用域，包含该类的所有成员声明：

```cpp
#include <iostream>
#include <memory>

class OuterClass {  // OuterClass 类作用域开始
public:
    static int static_member;  // 类作用域
    int instance_member = 100; // 类作用域
    
    // 嵌套类
    class NestedClass {  // NestedClass 类作用域
    public:
        int nested_member = 200;
        
        void nested_method() {
            std::cout << "Nested method" << std::endl;
        }
    };
    
    // 枚举在类作用域中
    enum class Color {
        Red,
        Green,
        Blue
    };
    
    // 类内定义的成员函数
    void outer_method() {  // 函数作用域（属于类作用域）
        int local_var = 300;  // 块作用域
        std::cout << "Outer method - local var: " << local_var << std::endl;
    }
    
    // 类外定义的成员函数
    void another_method();
};

int OuterClass::static_member = 400;

// 类外定义的成员函数（仍在类作用域中）
void OuterClass::another_method() {
    std::cout << "Another method" << std::endl;
    // 可以访问类的所有成员
    std::cout << "Static member: " << static_member << std::endl;
    std::cout << "Instance member: " << instance_member << std::endl;
}

void class_scope_example() {
    OuterClass outer_obj;
    OuterClass::NestedClass nested_obj;
    
    std::cout << "Static member: " << OuterClass::static_member << std::endl;
    std::cout << "Instance member: " << outer_obj.instance_member << std::endl;
    std::cout << "Nested member: " << nested_obj.nested_member << std::endl;
    
    outer_obj.outer_method();
    outer_obj.another_method();
    nested_obj.nested_method();
    
    OuterClass::Color color = OuterClass::Color::Red;
    std::cout << "Color value: " << static_cast<int>(color) << std::endl;
}
```

### 7.2 类作用域的特殊特性

```cpp
#include <iostream>
#include <memory>

class ComplexClass {
private:
    int private_member = 100;
    
public:
    static int static_member;
    
    // 友元函数可以访问私有成员
    friend void friend_function(const ComplexClass& obj);
    
    // 友元类
    friend class FriendClass;
    
    // 嵌套类可以访问外层类的成员
    class InnerClass {
    public:
        void access_outer(ComplexClass& outer) {
            // 可以访问外层类的私有成员
            std::cout << "Accessing outer private member: " << outer.private_member << std::endl;
        }
    };
    
    // 静态成员函数
    static void static_method() {
        std::cout << "Static method - static member: " << static_member << std::endl;
        // 不能访问实例成员
        // std::cout << private_member << std::endl; // 错误
    }
    
    // 实例成员函数
    void instance_method() {
        std::cout << "Instance method - private member: " << private_member << std::endl;
        std::cout << "Static member: " << static_member << std::endl;
    }
};

int ComplexClass::static_member = 200;

void friend_function(const ComplexClass& obj) {
    // 友元函数可以访问私有成员
    std::cout << "Friend function accessing private member: " << obj.private_member << std::endl;
}

class FriendClass {
public:
    void access_complex_class(ComplexClass& obj) {
        // 友元类可以访问私有成员
        std::cout << "Friend class accessing private member: " << obj.private_member << std::endl;
    }
};

void class_special_features() {
    ComplexClass obj;
    ComplexClass::InnerClass inner_obj;
    FriendClass friend_obj;
    
    obj.instance_method();
    ComplexClass::static_method();
    
    friend_function(obj);
    friend_obj.access_complex_class(obj);
    inner_obj.access_outer(obj);
}
```

## 8. 枚举作用域（Enumeration Scope）

### 8.1 枚举作用域定义

枚举声明引入枚举作用域，包含枚举器列表：

```cpp
#include <iostream>

// 传统枚举（无作用域枚举）
enum OldEnum {
    OLD_VALUE1,  // 枚举作用域
    OLD_VALUE2,
    OLD_VALUE3
};

// 作用域枚举（C++11）
enum class NewEnum {
    VALUE1,  // 枚举作用域
    VALUE2,
    VALUE3
};

// 带底层类型的枚举（C++11）
enum class TypedEnum : unsigned char {
    TYPED_VALUE1,  // 枚举作用域
    TYPED_VALUE2,
    TYPED_VALUE3
};

void enumeration_scope_example() {
    // 传统枚举值直接在声明的作用域中可见
    OldEnum old_val = OLD_VALUE1;
    std::cout << "Old enum value: " << old_val << std::endl;
    
    // 作用域枚举值必须通过作用域解析符访问
    NewEnum new_val = NewEnum::VALUE1;
    std::cout << "New enum value: " << static_cast<int>(new_val) << std::endl;
    
    // 带类型的枚举
    TypedEnum typed_val = TypedEnum::TYPED_VALUE1;
    std::cout << "Typed enum value: " << static_cast<int>(typed_val) << std::endl;
    
    // 枚举值的作用域示例
    if (old_val == OLD_VALUE1) {
        std::cout << "Old value is VALUE1" << std::endl;
    }
    
    if (new_val == NewEnum::VALUE1) {
        std::cout << "New value is VALUE1" << std::endl;
    }
}
```

### 8.2 枚举作用域的高级特性

```cpp
#include <iostream>

// 枚举的前向声明（C++11）
enum class ForwardEnum;  // 前向声明

// 枚举的完整定义
enum class ForwardEnum : int {
    FIRST,
    SECOND,
    THIRD
};

// 枚举类的成员函数
class EnumUser {
public:
    enum class Status {
        ACTIVE,
        INACTIVE,
        PENDING
    };
    
    Status current_status = Status::ACTIVE;
    
    void set_status(Status s) {
        current_status = s;
    }
    
    Status get_status() const {
        return current_status;
    }
    
    std::string status_to_string() const {
        switch (current_status) {
            case Status::ACTIVE:
                return "ACTIVE";
            case Status::INACTIVE:
                return "INACTIVE";
            case Status::PENDING:
                return "PENDING";
            default:
                return "UNKNOWN";
        }
    }
};

// 位枚举（通常用于标志）
enum class Permissions : unsigned int {
    READ = 1,
    WRITE = 2,
    EXECUTE = 4
};

Permissions operator|(Permissions a, Permissions b) {
    return static_cast<Permissions>(
        static_cast<unsigned int>(a) | static_cast<unsigned int>(b)
    );
}

void advanced_enumeration_example() {
    EnumUser user;
    user.set_status(EnumUser::Status::PENDING);
    std::cout << "User status: " << user.status_to_string() << std::endl;
    
    // 位枚举使用
    Permissions perms = Permissions::READ | Permissions::WRITE;
    std::cout << "Permissions value: " << static_cast<unsigned int>(perms) << std::endl;
    
    // 枚举与 switch 语句
    ForwardEnum fe = ForwardEnum::SECOND;
    switch (fe) {
        case ForwardEnum::FIRST:
            std::cout << "First" << std::endl;
            break;
        case ForwardEnum::SECOND:
            std::cout << "Second" << std::endl;
            break;
        case ForwardEnum::THIRD:
            std::cout << "Third" << std::endl;
            break;
    }
}
```

## 9. 模板参数作用域（Template Parameter Scope）

### 9.1 模板参数作用域定义

模板声明引入模板参数作用域，包含模板参数列表：

```cpp
#include <iostream>
#include <type_traits>

// 类模板参数作用域
template<
    typename T,                    // T 的作用域从这里开始
    int Size,                      // Size 的作用域
    typename U = T                 // U 的作用域，可以使用 T
>
class TemplateClass {
private:
    T data[Size];                  // 使用模板参数
    U extra_data;
    
public:
    TemplateClass() {
        std::cout << "Template class instantiated with Size = " << Size << std::endl;
    }
    
    void print_info() {
        std::cout << "Type T is integral: " << std::is_integral<T>::value << std::endl;
        std::cout << "Type U is same as T: " << std::is_same<T, U>::value << std::endl;
    }
};

// 函数模板参数作用域
template<
    typename ParamT,               // ParamT 的作用域
    typename ResultT = ParamT      // ResultT 的作用域，可以使用 ParamT
>
ResultT template_function(ParamT param) {
    std::cout << "Template function called with param: " << param << std::endl;
    return static_cast<ResultT>(param);
}

// 模板模板参数作用域
template<
    template<typename> class Container,  // Container 的作用域
    typename Element                     // Element 的作用域
>
class TemplateTemplateClass {
private:
    Container<Element> container;
    
public:
    void add_element(const Element& elem) {
        container.push_back(elem);
    }
};

void template_parameter_scope_example() {
    TemplateClass<int, 5> obj1;
    obj1.print_info();
    
    TemplateClass<double, 10, float> obj2;
    obj2.print_info();
    
    auto result1 = template_function(42);
    auto result2 = template_function<int, double>(100);
    
    std::cout << "Result1: " << result1 << std::endl;
    std::cout << "Result2: " << result2 << std::endl;
}
```

### 9.2 复杂模板参数作用域

```cpp
#include <iostream>
#include <concepts>
#include <vector>

// C++20 概念与模板参数作用域
template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template<
    Integral TInt,                 // TInt 必须是整数类型
    FloatingPoint TFloat,          // TFloat 必须是浮点类型
    typename TResult = decltype(TInt{} + TFloat{})
>
TResult mixed_template_function(TInt i, TFloat f) {
    return i + f;
}

// 可变参数模板参数作用域
template<
    typename... Types              // Types 参数包的作用域
>
class VariadicTemplate {
public:
    void print_types() {
        std::cout << "Number of types: " << sizeof...(Types) << std::endl;
        
        // C++17 折叠表达式
        ((std::cout << typeid(Types).name() << " "), ...);
        std::cout << std::endl;
    }
};

// 模板模板参数的复杂作用域
template<
    template<typename, typename> class Container,  // Container 的作用域
    template<typename> class Allocator,            // Allocator 的作用域
    typename T                                     // T 的作用域
>
class ComplexTemplate {
private:
    Container<T, Allocator<T>> container;
    
public:
    void add_element(const T& elem) {
        container.push_back(elem);
    }
};

void complex_template_example() {
    auto result = mixed_template_function(10, 3.14f);
    std::cout << "Mixed template result: " << result << std::endl;
    
    VariadicTemplate<int, double, std::string> var_obj;
    var_obj.print_types();
}
```

## 10. 合约断言作用域（Contract Assertion Scope）

### 10.1 定义合同断言的范围

每个合约断言 C 引入了一个合同断言范围，这个范围涵盖了 C。

### 10.2 后置条件断言的限制

如果一个后置条件断言包含一个非名称独立（即 name-dependent）的标识符，并且这个后置条件断言与某函数 func 关联，而其声明目标范围之一是以下情况时，程序将被判断为不合法 (ill-formed)：

**函数参数范围**:

当某个声明 D 的目标作用域为 func 的函数参数范围时。

**最近包含预设条件断言的 Lambda 表达式范围**：

当声明 D 关联的是某个 Lambda 表达式时，最近封闭的 λ 作用域可能会影响到前置条件断言。

### 10.3 合约断言范围解析

根据 C++ 合约语意，每当你写入一个断言语句（例如 [[expects: condition]], [[ensures: condition]])，这个语句引入一个新的 “合同断言范围”，这个新范围会从逻辑上影响你使用的内部变量（如 this, 或任何其他引用）及类/对象成员。因此，断言中所用的各种符号都必须在当前适合的作用区域里定义。举例如下：

```cpp
// Example 1：简单断言语境中的参数使用
bool test_function(bool condition)
{
    // [expects:] 断言属于局部端点的合同断彦范围
    auto ret = [=]() -> bool {    
         // lambda 使用 capture-by-value [=];
         [[expects: condition == true]];   // 此环境取 condition 在 local copy
          return condition;
    };
    bool result = ret();
    [[ensures: condition && result || (!condition && !result)]];
    
    printf("The output is %d\n", result);
    return result; 
}
```

## 11. 声明点规则

### 11.1 声明点基本规则

名称在其声明点之后可见，但具体位置取决于声明类型：

```cpp
#include <iostream>
#include <functional>

void declaration_point_examples() {
    int x = 32;  // 外层 x 在作用域中
    
    {
        int x = x;  // 内层 x 在初始化器之前就在作用域中
                    // 这不会用外层 x 初始化内层 x
                    // 而是用内层 x 自身的未定义值初始化
        std::cout << "Inner x (uninitialized): " << x << std::endl;
    }
    
    // 递归 Lambda 示例
    std::function<int(int)> factorial = [&](int n) -> int {
        return n > 1 ? n * factorial(n - 1) : 1;
    };
    // 函数名 factorial 在 Lambda 中可见并可以正确捕获
    
    std::cout << "Factorial of 5: " << factorial(5) << std::endl;
}
```

### 11.2 类和枚举声明点

```cpp
#include <iostream>
#include <memory>

// 类声明点示例
class SelfReference {
public:
    // 在类定义中可以引用自身类型
    std::shared_ptr<SelfReference> next;
    
    SelfReference() = default;
    
    void set_next(std::shared_ptr<SelfReference> n) {
        next = n;
    }
};

// 继承中的声明点
class Base {
public:
    virtual void base_method() {
        std::cout << "Base method" << std::endl;
    }
};

class Derived : public Base {  // Base 在此处作用域中
public:
    void derived_method() {
        base_method();  // 可以调用基类方法
        std::cout << "Derived method" << std::endl;
    }
};

// 枚举声明点
enum EnumExample : int {  // EnumExample 在冒号后作用域中
    ENUM_VAL1 = sizeof(EnumExample),  // 可以使用自身类型
    ENUM_VAL2,
    ENUM_VAL3
};

void class_enum_declaration_points() {
    auto obj1 = std::make_shared<SelfReference>();
    auto obj2 = std::make_shared<SelfReference>();
    obj1->set_next(obj2);
    
    Derived derived_obj;
    derived_obj.derived_method();
    
    std::cout << "Enum value 1: " << ENUM_VAL1 << std::endl;
}
```

### 11.3 类型别名声明点

```cpp
#include <iostream>

void type_alias_declaration_points() {
    using T = int;  // 外层 T 在分号后作用域中
    
    {
        using T = T*;  // 内层 T 在分号后作用域中
                      // 外层 T 在分号前仍在作用域中
                      // 等同于 T = int*
        T ptr = nullptr;  // T 是 int*
        std::cout << "Type alias example" << std::endl;
    }
}

// using 声明的声明点
class Base1 {
protected:
    static const int value1 = 100;
};

class Base2 {
protected:
    static const int value2 = 200;
};

class DerivedUsing : public Base1, public Base2 {
public:
    using Base1::value1;  // value1 在逗号后作用域中
    using Base2::value2;  // value2 在此处作用域中
    
    void print_values() {
        std::cout << "Value1: " << value1 << std::endl;
        std::cout << "Value2: " << value2 << std::endl;
    }
};
```

### 11.4 枚举器声明点

```cpp
#include <iostream>

void enumerator_declaration_points() {
    const int x = 12;
    
    {
        enum {
            x = x + 1,  // 枚举器 x 在逗号后作用域中
                       // 外层 x 在逗号前仍在作用域中
                       // 枚举器 x 初始化为 13
            y = x + 1   // y 初始化为 14
        };
        
        std::cout << "Enumerator x: " << x << std::endl;
        std::cout << "Enumerator y: " << y << std::endl;
    }
}
```

## 12. 作用域查找与解析

### 12.1 名称查找规则

C++ 使用未限定名称查找将名称与其声明关联：

```cpp
#include <iostream>

int global_var = 100;  // 全局作用域

namespace NS {
    int namespace_var = 200;  // 命名空间作用域
    
    class MyClass {
    public:
        static int class_var;  // 类作用域
        int member_var = 300;  // 类作用域
        
        void method() {
            int local_var = 400;  // 块作用域
            
            // 名称查找顺序：局部 -> 类 -> 命名空间 -> 全局
            std::cout << "Local var: " << local_var << std::endl;      // 局部作用域
            std::cout << "Member var: " << member_var << std::endl;    // 类作用域
            std::cout << "Class var: " << class_var << std::endl;      // 类作用域
            std::cout << "Namespace var: " << namespace_var << std::endl; // 命名空间作用域
            std::cout << "Global var: " << global_var << std::endl;    // 全局作用域
        }
    };
    
    int MyClass::class_var = 500;
}

void name_lookup_example() {
    NS::MyClass obj;
    obj.method();
}
```

### 12.2 作用域解析运算符

使用作用域解析运算符 `::` 进行限定名称查找：

```cpp
#include <iostream>

int value = 100;  // 全局作用域

namespace Outer {
    int value = 200;  // 命名空间作用域
    
    namespace Inner {
        int value = 300;  // 嵌套命名空间作用域
        
        class MyClass {
        public:
            static int value;  // 类作用域
            int member = 400;  // 类作用域
            
            void method() {
                int value = 500;  // 块作用域
                
                // 使用作用域解析运算符访问不同作用域的同名变量
                std::cout << "Local value: " << value << std::endl;
                std::cout << "Class value: " << MyClass::value << std::endl;
                std::cout << "Inner namespace value: " << Inner::value << std::endl;
                std::cout << "Outer namespace value: " << Outer::value << std::endl;
                std::cout << "Global value: " << ::value << std::endl;
            }
        };
        
        int MyClass::value = 600;
    }
}

void scope_resolution_example() {
    Outer::Inner::MyClass obj;
    obj.method();
}
```

## 13. 最佳实践

### 13.1 作用域管理最佳实践

合理的使用作用域可以提高代码质量和可维护性：

```cpp
#include <iostream>
#include <vector>
#include <memory>

// 1. 最小化变量作用域
void minimal_scope_example() {
    // 只在需要时声明变量
    for (int i = 0; i < 10; i++) {
        std::cout << "Index: " << i << std::endl;
        // i 的作用域限制在 for 循环内
    }
    
    // i 在此处不可见，避免意外使用
    
    // 2. 使用块作用域进行逻辑分组
    {
        // 初始化相关变量
        auto init_value = std::make_unique<int>(42);
        std::vector<int> buffer(100);
        
        // 使用这些变量进行初始化操作
        for (size_t j = 0; j < buffer.size(); j++) {
            buffer[j] = *init_value + static_cast<int>(j);
        }
        
        // 相关变量作用域结束，避免后续误用
    }
}

// 3. 合理使用命名空间
namespace utilities {
    namespace detail {
        // 实现细节放在 detail 命名空间中
        void internal_helper() {
            std::cout << "Internal helper function" << std::endl;
        }
    }
    
    void public_function() {
        detail::internal_helper();  // 内部函数
        std::cout << "Public function" << std::endl;
    }
}

// 4. 类设计中的作用域考虑
class BestPracticeClass {
private:
    int private_data = 0;  // 私有成员，限制访问
    
    // 私有辅助函数
    void private_helper() {
        std::cout << "Private helper" << std::endl;
    }
    
public:
    // 公共接口
    void public_method() {
        private_helper();  // 内部调用
        std::cout << "Public method" << std::endl;
    }
    
    // 访问器函数
    int get_data() const { return private_data; }
    void set_data(int value) { private_data = value; }
};

void best_practices_example() {
    minimal_scope_example();
    utilities::public_function();
    
    BestPracticeClass obj;
    obj.public_method();
    obj.set_data(100);
    std::cout << "Data: " << obj.get_data() << std::endl;
}
```

### 13.2 避免作用域相关的常见错误

```cpp
#include <iostream>

// 1. 避免变量隐藏
void avoid_shadowing() {
    int x = 100;
    
    // 不好的做法：隐藏外层变量
    /*
    {
        int x = 200;  // 隐藏外层 x
        std::cout << "Inner x: " << x << std::endl;
    }
    */
    
    // 好的做法：使用不同名称
    {
        int inner_x = 200;
        std::cout << "Inner x: " << inner_x << std::endl;
    }
    
    std::cout << "Outer x: " << x << std::endl;
}

// 2. 注意变量初始化时机
void initialization_timing() {
    int x = 10;
    
    {
        // 错误示例：使用未初始化的变量
        // int y = y + 1;  // 使用未初始化的 y
        
        // 正确示例：明确初始化
        int z = x + 1;  // 使用已初始化的外层变量
        std::cout << "z = " << z << std::endl;
    }
}

// 3. Lambda 捕获注意事项
void lambda_capture_best_practices() {
    int value = 100;
    
    // 值捕获 - 安全但可能效率较低
    auto value_lambda = [value]() {
        std::cout << "Value capture: " << value << std::endl;
    };
    
    // 引用捕获 - 高效但需要注意生命周期
    auto ref_lambda = [&value]() {
        std::cout << "Reference capture: " << value << std::endl;
    };
    
    // 移动捕获 - 用于独占资源（C++14）
    auto ptr = std::make_unique<int>(200);
    auto move_lambda = [moved_ptr = std::move(ptr)]() {
        std::cout << "Move capture: " << *moved_ptr << std::endl;
    };
    
    value_lambda();
    ref_lambda();
    move_lambda();
}
```

### 13.3 现代 C++ 作用域使用

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

// 1. 结构化绑定作用域（C++17）
void structured_binding_scope() {
    std::pair<int, std::string> data{42, "Hello"};
    
    // 结构化绑定
    auto [id, message] = data;  // id 和 message 在此处作用域中
    
    std::cout << "ID: " << id << ", Message: " << message << std::endl;
    
    // 在循环中使用结构化绑定
    std::vector<std::pair<int, std::string>> vec{{1, "One"}, {2, "Two"}, {3, "Three"}};
    for (const auto& [num, text] : vec) {
        std::cout << num << " -> " << text << std::endl;
    }
}

// 2. if 和 switch 初始化语句作用域（C++17）
void if_switch_scope() {
    // if 初始化语句
    if (auto ptr = std::make_unique<int>(42); ptr) {
        std::cout << "If init - Value: " << *ptr << std::endl;
        // ptr 在此处作用域中
    }
    // ptr 在此处超出作用域
    
    // switch 初始化语句
    switch (int status = 1; status) {
        case 1:
            std::cout << "Status is 1" << std::endl;
            break;
        default:
            std::cout << "Unknown status" << std::endl;
            break;
    }
    // status 在此处超出作用域
}

// 3. 模板和概念作用域（C++20）
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>  // Numeric 概念在模板参数作用域中
void numeric_function(T value) {
    std::cout << "Numeric value: " << value << std::endl;
}

void modern_cpp_scope_examples() {
    structured_binding_scope();
    if_switch_scope();
    numeric_function(3.14);
    numeric_function(42);
}
```

## 14. 标准参考

C++ 作用域的相关标准条款：

- C++23 标准（ISO/IEC 14882:2024）：6.4 作用域 [basic.scope]
- C++20 标准（ISO/IEC 14882:2020）：6.4 作用域 [basic.scope]
- C++17 标准（ISO/IEC 14882:2017）：6.3 作用域 [basic.scope]
- C++14 标准（ISO/IEC 14882:2014）：3.3 作用域 [basic.scope]
- C++11 标准（ISO/IEC 14882:2011）：3.3 作用域 [basic.scope]
- C++98 标准（ISO/IEC 14882:1998）：3.3 声明区域和作用域 [basic.scope]

相关的标准参考还包括：

- 3.4 名称查找 [basic.lookup]
- 6.5 程序和链接 [basic.start]
- 11 类 [class]
- 12 特殊成员函数 [special]
- 13 重载 [over]
- 14 模板 [temp]

## 15. 总结

C++ 作用域是程序设计中的核心概念，它通过控制声明的可见性和生命周期来帮助组织代码结构和管理内存。与 C 语言相比，C++ 作用域机制更加复杂和丰富，支持面向对象、模板、Lambda 表达式等现代特性。

### 15.1 核心要点回顾

**多种作用域类型**：

- 块作用域：在复合语句中声明的标识符
- 函数参数作用域：函数参数的声明范围
- Lambda 作用域：Lambda 表达式的特殊作用域
- 命名空间作用域：命名空间内声明的标识符
- 类作用域：类成员的声明范围
- 枚举作用域：枚举器的声明范围
- 模板参数作用域：模板参数的声明范围

**作用域层次结构**：

- 作用域形成层次化结构，每个作用域都有父作用域
- 内层作用域可以隐藏外层作用域中的同名声明
- 作用域必须正确嵌套，不能交叉重叠

**名称查找机制**：

- 未限定名称查找按照作用域层次进行
- 限定名称查找使用作用域解析运算符
- 不同类型的声明存在于不同的命名空间中

**现代 C++ 特性**：

- Lambda 表达式的特殊作用域规则
- 结构化绑定的作用域（C++17）
- if 和 switch 初始化语句的作用域（C++17）
- 概念和约束的作用域（C++20）

### 15.2 实践建议

- 最小化作用域：在最接近使用点的地方声明变量
- 避免变量隐藏：使用清晰的命名避免意外隐藏
- 合理使用命名空间：组织代码结构，避免名称冲突
- 注意 Lambda 捕获：选择合适的捕获方式，注意生命周期
- 利用现代特性：使用 C++11/14/17/20 的新作用域特性
- 清晰的命名：使用有意义的变量名提高代码可读性

### 15.3 历史演进

C++ 作用域机制在不同标准版本中持续演进：

- C++98/03：建立了基本的作用域规则
- C++11：引入 Lambda 表达式、作用域枚举等新特性
- C++14：扩展 Lambda 特性，引入泛型 Lambda
- C++17：引入结构化绑定、if/switch 初始化语句
- C++20：引入概念和约束，进一步完善作用域规则
- C++23：继续完善和标准化作用域相关特性

通过深入理解和正确应用作用域概念，开发者可以编写出更加清晰、安全和高效的 C++ 程序，避免常见的作用域相关错误，提高代码的可维护性和可读性。
