# C++ 名称查找完全详解

  [1. 名称查找基本概念](#1-名称查找基本概念)
  
  [2. 查找类型分类](#2-查找类型分类)
  
  [3. 限定名称查找](#3-限定名称查找)
  
  [4. 未限定名称查找](#4-未限定名称查找)
  
  [5. 参数相关查找（ADL）](#5-参数相关查找adl)
  
  [6. 结构体黑客（Struct Hack）](#6-结构体黑客struct-hack)
  
  [7. 重载解析与名称查找](#7-重载解析与名称查找)
  
  [8. 模板名称查找](#8-模板名称查找)
  
  [9. 最佳实践](#9-最佳实践)
  
  [10. 标准参考](#10-标准参考)
  
  [11. 总结](#11-总结)

## 1. 名称查找基本概念

### 1.1 什么是名称查找

名称查找（Name Lookup）是指当一个名称在程序中遇到时，将其与引入该名称的声明相关联的过程。这是 C++ 编译过程中的关键步骤，决定了程序中每个标识符的含义。

```cpp
#include <iostream>
#include <vector>

int main() {
    // 编译器需要执行多个名称查找操作：
    
    // 1. 对名称 std 进行未限定名称查找 -> 找到命名空间 std
    // 2. 对名称 cout 进行限定名称查找 -> 在 std 中找到变量声明
    // 3. 对名称 endl 进行限定名称查找 -> 在 std 中找到函数模板声明
    std::cout << "Hello, World!" << std::endl;
    
    // 4. 对名称 vector 进行限定名称查找 -> 在 std 中找到类模板声明
    std::vector<int> vec;
    
    return 0;
}
```

### 1.2 名称查找的重要性

名称查找是编译器理解程序语义的基础：

```cpp
#include <iostream>

// 全局作用域中的声明
int global_var = 100;
void global_function() { std::cout << "Global function\n"; }

namespace ns {
    // 命名空间作用域中的声明
    int ns_var = 200;
    void ns_function() { std::cout << "Namespace function\n"; }
    
    class MyClass {
    public:
        static int class_var;
        static void class_function() { std::cout << "Class function\n"; }
        
        void member_function() { 
            std::cout << "Member function\n"; 
        }
    };
    
    int MyClass::class_var = 300;
}

void lookup_example() {
    // 不同位置的名称查找示例
    global_var = 10;           // 全局作用域查找
    global_function();         // 全局作用域查找
    
    ns::ns_var = 20;           // 限定名称查找
    ns::ns_function();         // 限定名称查找
    
    ns::MyClass::class_var = 30;     // 嵌套限定名称查找
    ns::MyClass::class_function();   // 嵌套限定名称查找
    
    ns::MyClass obj;
    obj.member_function();     // 成员访问查找
}
```

## 2. 查找类型分类

### 2.1 限定名称查找 vs 未限定名称查找

名称查找根据是否使用作用域解析运算符分为两种类型：

```cpp
#include <iostream>

namespace outer {
    namespace inner {
        int value = 42;
        void function() { std::cout << "Function\n"; }
    }
    
    class Container {
    public:
        static int static_member;
        int instance_member;
        
        Container(int val) : instance_member(val) {}
        static void static_method() { std::cout << "Static method\n"; }
        void instance_method() { std::cout << "Instance method\n"; }
    };
    
    int Container::static_member = 100;
}

void lookup_types_example() {
    using namespace outer;
    
    // 1. 未限定名称查找
    value = 10;        // 在当前作用域及外层作用域中查找
    function();        // 在当前作用域及外层作用域中查找
    
    // 2. 限定名称查找
    outer::inner::value = 20;     // 明确指定查找路径
    outer::inner::function();     // 明确指定查找路径
    
    // 3. 混合查找
    Container::static_member = 30;  // 限定类作用域查找
    Container obj(50);
    obj.instance_member = 40;       // 成员访问查找
}
```

### 2.2 特殊查找情况

```cpp
#include <iostream>

template<typename T>
class TemplateClass {
public:
    typedef T value_type;
    
    template<typename U>
    void template_method(U param) {
        // 在模板上下文中的名称查找
        value_type val = param;  // 查找 value_type
    }
};

namespace ns {
    template<typename T>
    class NestedTemplate {
    public:
        void method() {
            // 复杂的模板限定名称查找
            typename TemplateClass<T>::value_type val{};
        }
    };
}

void special_lookup_cases() {
    // 1. 模板中的类型查找
    TemplateClass<int> tc;
    tc.template_method(42);
    
    // 2. 嵌套模板中的查找
    ns::NestedTemplate<double> nt;
    nt.method();
    
    // 3. 消除歧义的 template 关键字使用
    // obj.template method<int>();  // 当需要明确指定模板参数时
}
```

## 3. 限定名称查找

### 3.1 基本限定名称查找

限定名称查找通过作用域解析运算符 `::` 明确指定查找路径：

```cpp
#include <iostream>
#include <vector>

namespace first {
    int value = 100;
    
    namespace second {
        int value = 200;
        
        class MyClass {
        public:
            static int value;
            int member;
            
            MyClass() : member(300) {}
            static void static_func() { std::cout << "Static function\n"; }
        };
        
        int MyClass::value = 400;
    }
}

void qualified_lookup_example() {
    // 逐级限定查找
    int a = first::value;                    // 100
    int b = first::second::value;            // 200
    int c = first::second::MyClass::value;   // 400
    
    first::second::MyClass obj;
    int d = obj.member;                      // 300
    
    first::second::MyClass::static_func();   // 调用静态函数
    
    // 标准库中的限定查找
    std::vector<int> vec;
    std::cout << "Size: " << vec.size() << std::endl;
}
```

### 3.2 复杂限定查找

```cpp
#include <iostream>

namespace outer {
    namespace middle {
        namespace inner {
            int deeply_nested_value = 42;
            
            template<typename T>
            class ComplexTemplate {
            public:
                template<typename U>
                struct NestedStruct {
                    static T static_value;
                    U nested_value;
                };
            };
            
            template<typename T>
            template<typename U>
            T ComplexTemplate<T>::NestedStruct<U>::static_value = T{};
        }
    }
}

void complex_qualified_lookup() {
    // 深度嵌套的限定查找
    int val1 = outer::middle::inner::deeply_nested_value;
    
    // 模板类中的嵌套结构体查找
    using NestedType = outer::middle::inner::ComplexTemplate<int>::NestedStruct<double>;
    NestedType::static_value = 100;
    
    NestedType obj;
    obj.nested_value = 3.14;
    
    std::cout << "Static value: " << NestedType::static_value << std::endl;
    std::cout << "Nested value: " << obj.nested_value << std::endl;
}
```

## 4. 未限定名称查找

### 4.1 基本未限定名称查找

未限定名称查找在当前作用域及外层作用域中搜索名称：

```cpp
#include <iostream>

int global_var = 1000;

namespace ns {
    int ns_var = 200;
    
    void ns_function() {
        std::cout << "Namespace function\n";
    }
    
    class OuterClass {
    private:
        static int outer_static;
        
    public:
        class InnerClass {
        private:
            int inner_member;
            
        public:
            InnerClass() : inner_member(300) {}
            
            void method() {
                // 未限定名称查找顺序：
                // 1. InnerClass 作用域
                // 2. OuterClass 作用域
                // 3. ns 命名空间作用域
                // 4. 全局作用域
                
                inner_member = 10;     // 找到 InnerClass::inner_member
                outer_static = 20;     // 找到 OuterClass::outer_static
                ns_var = 30;           // 找到 ns::ns_var
                global_var = 40;       // 找到 ::global_var
                
                ns_function();         // 找到 ns::ns_function
            }
        };
        
        static void outer_method() {
            outer_static = 50;         // 找到 OuterClass::outer_static
            ns_var = 60;               // 找到 ns::ns_var
            global_var = 70;           // 找到 ::global_var
        }
    };
    
    int OuterClass::outer_static = 100;
}

void unqualified_lookup_example() {
    ns::OuterClass::InnerClass obj;
    obj.method();
    
    ns::OuterClass::outer_method();
    
    // 在当前作用域查找
    global_var = 80;  // 找到 ::global_var
}
```

### 4.2 作用域遮蔽

```cpp
#include <iostream>

int x = 100;  // 全局变量

void scope_shadowing_example() {
    int x = 200;  // 局部变量遮蔽全局变量
    
    {
        int x = 300;  // 更内层的局部变量遮蔽外层变量
        std::cout << "Innermost x: " << x << std::endl;  // 输出 300
    }
    
    std::cout << "Local x: " << x << std::endl;  // 输出 200
}

namespace example {
    int y = 400;
    
    void function() {
        int y = 500;  // 函数内局部变量遮蔽命名空间变量
        std::cout << "Local y: " << y << std::endl;  // 输出 500
        
        std::cout << "Global x: " << x << std::endl;  // 从外层作用域查找
    }
}

class ShadowingClass {
private:
    int z = 600;
    
public:
    void method() {
        int z = 700;  // 局部变量遮蔽成员变量
        std::cout << "Local z: " << z << std::endl;     // 输出 700
        std::cout << "Member z: " << this->z << std::endl;  // 使用 this 访问成员
    }
};
```

## 5. 参数相关查找（ADL）

### 5.1 ADL 基本概念

参数相关查找（Argument-Dependent Lookup, ADL）是一种特殊的未限定名称查找，它会考虑函数参数类型的关联命名空间：

```cpp
#include <iostream>

namespace ns {
    class MyClass {
    public:
        int value;
        MyClass(int v) : value(v) {}
    };
    
    // 在参数类型的命名空间中定义的函数
    void print(const MyClass& obj) {
        std::cout << "ns::print: " << obj.value << std::endl;
    }
    
    MyClass operator+(const MyClass& a, const MyClass& b) {
        return MyClass(a.value + b.value);
    }
}

void adl_basic_example() {
    ns::MyClass obj1(10);
    ns::MyClass obj2(20);
    
    // ADL 会查找参数类型 ns::MyClass 所在的命名空间 ns
    print(obj1);           // 调用 ns::print
    
    auto obj3 = obj1 + obj2;  // ADL 查找 ns 中的 operator+
    print(obj3);           // 调用 ns::print
}
```

### 5.2 ADL 复杂情况

```cpp
#include <iostream>
#include <vector>

namespace first {
    struct TypeA {
        int value;
        TypeA(int v) : value(v) {}
    };
    
    void process(TypeA a) {
        std::cout << "first::process(TypeA): " << a.value << std::endl;
    }
}

namespace second {
    struct TypeB {
        int value;
        TypeB(int v) : value(v) {}
    };
    
    void process(TypeB b) {
        std::cout << "second::process(TypeB): " << b.value << std::endl;
    }
    
    // 友元函数也可以通过 ADL 找到
    struct TypeC {
        int value;
        TypeC(int v) : value(v) {}
        
        friend void show(const TypeC& c) {
            std::cout << "friend show: " << c.value << std::endl;
        }
    };
}

namespace both {
    struct TypeD {
        int value;
        TypeD(int v) : value(v) {}
    };
    
    // 多个命名空间中的重载函数
    void handle(first::TypeA a, both::TypeD d) {
        std::cout << "both::handle: " << a.value << ", " << d.value << std::endl;
    }
    
    void handle(second::TypeB b, both::TypeD d) {
        std::cout << "both::handle: " << b.value << ", " << d.value << std::endl;
    }
}

void adl_complex_example() {
    first::TypeA a(100);
    second::TypeB b(200);
    second::TypeC c(300);
    both::TypeD d(400);
    
    // ADL 查找
    process(a);  // 调用 first::process
    process(b);  // 调用 second::process
    show(c);     // 调用友元函数 second::TypeC::show
    
    // 多参数 ADL
    handle(a, d);  // 调用 both::handle(first::TypeA, both::TypeD)
    handle(b, d);  // 调用 both::handle(second::TypeB, both::TypeD)
}
```

### 5.3 ADL 与标准库

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

namespace custom {
    struct Point {
        int x, y;
        Point(int x_, int y_) : x(x_), y(y_) {}
    };
    
    // 为自定义类型提供 swap 函数
    void swap(Point& a, Point& b) {
        std::cout << "Custom swap called\n";
        std::swap(a.x, b.x);
        std::swap(a.y, b.y);
    }
    
    // 输出操作符
    std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
}

void adl_standard_library_example() {
    using namespace custom;
    
    Point p1(1, 2);
    Point p2(3, 4);
    
    std::cout << "Before swap: " << p1 << " " << p2 << std::endl;
    
    // ADL 会找到 custom::swap 而不是 std::swap
    using std::swap;  // 提供后备选项
    swap(p1, p2);     // ADL 优先选择 custom::swap
    
    std::cout << "After swap: " << p1 << " " << p2 << std::endl;
    
    // 标准库算法也使用 ADL
    std::vector<Point> points = {{1, 1}, {2, 2}, {3, 3}};
    
    // std::sort 会使用 ADL 查找 compare 函数（如果提供）
    // 或使用默认的 operator<
}
```

## 6. 结构体黑客（Struct Hack）

### 6.1 结构体黑客概念

结构体黑客（Struct Hack）或类型/非类型隐藏是指在同一作用域内，某些名称的出现可能引用一个不是 typedef 的类/结构/联合/枚举的声明，而该名称的其他所有出现要么都引用同一个变量、非静态数据成员或枚举器，要么都引用可能重载的函数或函数模板名称。

```cpp
#include <iostream>

// 结构体黑客示例
struct stat {  // 类声明
    int st_mode;
    // ... 其他成员
};

void struct_hack_example() {
    struct stat {  // 局部结构体声明，遮蔽全局的 struct stat
        int x, y;
    };
    
    // 在这个作用域内，stat 指向局部结构体
    struct stat local_stat = {10, 20};
    std::cout << "Local stat: " << local_stat.x << ", " << local_stat.y << std::endl;
    
    // 如果需要访问全局的 struct stat，需要使用详细类型说明符
    struct ::stat global_stat;  // 使用全局作用域的 struct stat
    global_stat.st_mode = 0755;
}

// 另一种结构体黑客情况
void another_struct_hack() {
    struct S {
        int value;
    };
    
    int S = 42;  // 变量 S 遮蔽了结构体 S
    
    // 在这个作用域内，S 指向变量
    std::cout << "Variable S: " << S << std::endl;
    
    // 如果需要创建结构体实例，需要使用详细类型说明符
    struct S obj = {100};  // 使用详细类型说明符
    std::cout << "Struct S value: " << obj.value << std::endl;
}
```

### 6.2 结构体黑客的实际应用

```cpp
#include <iostream>

// 保持 C 兼容性的结构体黑客
struct FILE {  // 结构体声明
    int handle;
    // ... 其他成员
};

void c_compatibility_example() {
    struct FILE {  // 局部结构体，用于演示
        int descriptor;
    };
    
    // 在局部作用域中，FILE 指向局部结构体
    struct FILE local_file = {123};
    std::cout << "Local FILE descriptor: " << local_file.descriptor << std::endl;
    
    // 实际的 C 标准库中，FILE* 是一个不完整类型
    // FILE* fp = fopen("test.txt", "r");  // 使用全局的 FILE
}

// 函数重载与结构体黑客
struct Calculator {
    int result;
};

void Calculator(int initial_value) {  // 函数名与结构体名相同
    std::cout << "Calculator function called with: " << initial_value << std::endl;
}

void struct_function_hack() {
    // 这里 Calculator 指向函数
    Calculator(42);
    
    // 如果要创建结构体实例，需要使用详细类型说明符
    struct Calculator calc = {100};
    std::cout << "Calculator result: " << calc.result << std::endl;
}
```

## 7. 重载解析与名称查找

### 7.1 名称查找与重载解析的关系

对于函数和函数模板名称，名称查找可以将多个声明与相同名称关联，并可能从 argument-dependent lookup 获取额外的声明。这些声明集被传递给重载解析，后者选择将要使用的声明。

```cpp
#include <iostream>

// 全局作用域中的重载函数
void print(int value) {
    std::cout << "int: " << value << std::endl;
}

void print(double value) {
    std::cout << "double: " << value << std::endl;
}

void print(const char* value) {
    std::cout << "string: " << value << std::endl;
}

namespace ns {
    // 命名空间中的重载函数
    void print(int value) {
        std::cout << "ns::int: " << value << std::endl;
    }
    
    class MyClass {
    public:
        // 类中的重载函数
        void print(int value) {
            std::cout << "MyClass::int: " << value << std::endl;
        }
        
        void print(double value) {
            std::cout << "MyClass::double: " << value << std::endl;
        }
    };
}

void overload_resolution_example() {
    using namespace ns;
    
    // 重载解析会选择最匹配的函数
    print(42);        // 调用 ::print(int)
    print(3.14);      // 调用 ::print(double)
    print("Hello");   // 调用 ::print(const char*)
    
    MyClass obj;
    obj.print(42);    // 调用 MyClass::print(int)
    obj.print(3.14);  // 调用 MyClass::print(double)
    
    // 使用限定名称明确指定查找范围
    ::print(100);     // 明确调用全局函数
    ns::print(200);   // 明确调用命名空间函数
}
```

### 7.2 ADL 与重载解析

```cpp
#include <iostream>

namespace math {
    struct Complex {
        double real, imag;
        Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    };
    
    // 为 Complex 类型定义的运算符
    Complex operator+(const Complex& a, const Complex& b) {
        return Complex(a.real + b.real, a.imag + b.imag);
    }
    
    Complex operator*(const Complex& a, const Complex& b) {
        return Complex(a.real * b.real - a.imag * b.imag,
                      a.real * b.imag + a.imag * b.real);
    }
    
    std::ostream& operator<<(std::ostream& os, const Complex& c) {
        return os << c.real << " + " << c.imag << "i";
    }
    
    // 数学函数
    Complex sqrt(const Complex& c) {
        // 简化的平方根实现
        double magnitude = std::sqrt(c.real * c.real + c.imag * c.imag);
        return Complex(std::sqrt(magnitude), 0);
    }
}

namespace utils {
    // 通用打印函数
    template<typename T>
    void print(const T& value) {
        std::cout << "Generic print: " << value << std::endl;
    }
    
    // 为特定类型特化的打印函数
    void print(const math::Complex& c) {
        std::cout << "Specialized print for Complex: " << c << std::endl;
    }
}

void adl_overload_example() {
    using namespace math;
    
    Complex a(3, 4);
    Complex b(1, 2);
    
    // ADL 会找到 math 命名空间中的运算符
    auto sum = a + b;    // 调用 math::operator+
    auto product = a * b; // 调用 math::operator*
    
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Product: " << product << std::endl;
    
    // ADL 与模板结合
    using namespace utils;
    print(sum);          // 调用 utils::print(const math::Complex&)
    
    // ADL 查找 sqrt 函数
    auto root = sqrt(sum);  // 调用 math::sqrt
    print(root);            // 调用 utils::print(const math::Complex&)
}
```

## 8. 模板名称查找

### 8.1 模板中的名称查找

模板中的名称查找分为两阶段：依赖名称的查找在实例化时进行，非依赖名称的查找在模板定义时进行。

```cpp
#include <iostream>
#include <vector>

// 全局函数模板
template<typename T>
void global_template(T value) {
    std::cout << "Global template: " << value << std::endl;
}

namespace ns {
    // 命名空间中的函数模板
    template<typename T>
    void ns_template(T value) {
        std::cout << "Namespace template: " << value << std::endl;
    }
    
    template<typename T>
    class TemplateClass {
    private:
        T data;
        
    public:
        TemplateClass(const T& value) : data(value) {}
        
        // 非依赖名称查找（在模板定义时进行）
        void non_dependent_lookup() {
            global_template(data);  // 查找定义时可见的 global_template
        }
        
        // 依赖名称查找（在模板实例化时进行）
        template<typename U>
        void dependent_lookup(U param) {
            ns_template(param);     // 在实例化时查找
        }
        
        // 使用 typename 关键字的依赖类型查找
        template<typename Container>
        void container_operation(const Container& c) {
            typename Container::value_type val = c[0];  // 依赖类型查找
            std::cout << "Container value: " << val << std::endl;
        }
    };
}

void template_lookup_example() {
    ns::TemplateClass<int> obj(42);
    obj.non_dependent_lookup();
    obj.dependent_lookup(3.14);
    
    std::vector<int> vec = {1, 2, 3};
    obj.container_operation(vec);
}
```

### 8.2 两阶段查找详解

```cpp
#include <iostream>

int global_var = 100;

template<typename T>
class TwoPhaseLookup {
public:
    void method() {
        // 第一阶段：非依赖名称查找（模板定义时）
        global_var = 200;  // 查找定义时可见的 global_var
        
        // 第二阶段：依赖名称查找（模板实例化时）
        dependent_function(T{});  // 在实例化时查找
    }
    
    // 依赖函数模板调用
    template<typename U>
    void call_dependent_template(U param) {
        dependent_template(param);  // 在实例化时查找
    }
};

// 在模板定义后声明的函数
void dependent_function(int value) {
    std::cout << "Dependent function called with: " << value << std::endl;
}

template<typename T>
void dependent_template(T value) {
    std::cout << "Dependent template called with: " << value << std::endl;
}

void two_phase_lookup_example() {
    TwoPhaseLookup<int> obj;
    obj.method();  // 此时 dependent_function 必须可见
    
    obj.call_dependent_template(3.14);
}
```

## 9. 最佳实践

### 9.1 避免名称查找问题

```cpp
#include <iostream>

// 1. 使用明确的限定名称
namespace project {
    namespace utility {
        void helper_function() {
            std::cout << "Project utility helper\n";
        }
    }
    
    namespace core {
        void core_function() {
            // 明确指定命名空间
            utility::helper_function();
        }
    }
}

// 2. 避免过度使用 using 声明
void best_practices_example() {
    // 好的做法：明确限定
    std::cout << "Explicit qualification\n";
    std::vector<int> vec;
    
    // 避免在头文件中使用 using namespace
    // using namespace std;  // 不推荐在头文件中使用
    
    // 在实现文件中可以有限制地使用
    {
        using std::cout;
        using std::endl;
        cout << "Limited using declarations" << endl;
    }
}

// 3. 合理使用 ADL
namespace math_utils {
    struct Point {
        int x, y;
        Point(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
    };
    
    // 为自定义类型提供相关的函数
    Point operator+(const Point& a, const Point& b) {
        return Point(a.x + b.x, a.y + b.y);
    }
    
    std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
    
    void swap(Point& a, Point& b) {
        std::swap(a.x, b.x);
        std::swap(a.y, b.y);
    }
}

void adl_best_practices() {
    using namespace math_utils;
    
    Point a(1, 2);
    Point b(3, 4);
    
    auto sum = a + b;  // ADL 自然工作
    std::cout << "Sum: " << sum << std::endl;
    
    using std::swap;   // 提供后备
    swap(a, b);        // ADL 选择适当的 swap
}
```

### 9.2 模板中的最佳实践

```cpp
#include <iostream>
#include <type_traits>

// 1. 使用 typename 明确依赖类型
template<typename Container>
void process_container(const Container& c) {
    // 明确指出 value_type 是类型而非变量
    typename Container::value_type first_element = c.front();
    std::cout << "First element: " << first_element << std::endl;
}

// 2. 使用 std::enable_if 进行 SFINAE
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_if_integral(T value) {
    std::cout << "Integral value: " << value << std::endl;
}

template<typename T>
typename std::enable_if<!std::is_integral<T>::value, void>::type
print_if_integral(T value) {
    std::cout << "Non-integral value: " << value << std::endl;
}

// 3. 使用概念（C++20）约束模板参数
#if __cplusplus >= 202002L
#include <concepts>

template<std::integral T>
void process_integral(T value) {
    std::cout << "Processing integral: " << value << std::endl;
}

template<std::floating_point T>
void process_floating(T value) {
    std::cout << "Processing floating: " << value << std::endl;
}
#endif

void template_best_practices() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    process_container(vec);
    
    print_if_integral(42);
    print_if_integral(3.14);
    
#if __cplusplus >= 202002L
    process_integral(100);
    process_floating(2.71);
#endif
}
```

## 10. 标准参考

C++ 名称查找的相关标准条款：

- C++23 标准（ISO/IEC 14882:2024）：
  - 6.5 名称查找 [basic.lookup]
  - 6.5.1 未限定名称查找 [basic.lookup.unqual]
  - 6.5.2 限定名称查找 [basic.lookup.qual]
  - 6.5.3 基于参数的查找 [basic.lookup.argdep]
- C++20 标准（ISO/IEC 14882:2020）：
  - 6.5 名称查找 [basic.lookup]
  - 6.5.1 未限定名称查找 [basic.lookup.unqual]
  - 6.5.2 限定名称查找 [basic.lookup.qual]
  - 6.5.3 基于参数的查找 [basic.lookup.argdep]
- C++17 标准（ISO/IEC 14882:2017）：
  - 6.4 名称查找 [basic.lookup]
  - 6.4.1 未限定名称查找 [basic.lookup.unqual]
  - 6.4.2 限定名称查找 [basic.lookup.qual]
  - 6.4.3 基于参数的查找 [basic.lookup.argdep]
- C++14 标准（ISO/IEC 14882:2014）：
  - 3.4 名称查找 [basic.lookup]
  - 3.4.1 未限定名称查找 [basic.lookup.unqual]
  - 3.4.2 限定名称查找 [basic.lookup.qual]
  - 3.4.3 基于参数的查找 [basic.lookup.argdep]
- C++11 标准（ISO/IEC 14882:2011）：
  - 3.4 名称查找 [basic.lookup]
  - 3.4.1 未限定名称查找 [basic.lookup.unqual]
  - 3.4.2 限定名称查找 [basic.lookup.qual]
  - 3.4.3 基于参数的查找 [basic.lookup.argdep]

相关的标准参考还包括：

- 3.3 作用域 [basic.scope]
- 6.6 名称解析 [basic.link]
- 13 重载 [over]
- 14 模板 [temp]
- 15 异常处理 [except]

## 11. 总结

C++ 名称查找是编译器理解程序语义的核心机制，它决定了程序中每个标识符的含义。深入理解名称查找机制对于编写正确、高效的 C++ 程序至关重要。

### 11.1 核心要点回顾

**名称查找类型**：

- 限定名称查找：通过 `::` 明确指定查找路径
- 未限定名称查找：在当前及外层作用域中搜索
- 参数相关查找（ADL）：考虑参数类型关联的命名空间

**查找顺序**：

- 局部作用域
- 外层作用域
- 命名空间作用域
- 全局作用域

**特殊机制**：

- 结构体黑客：同一作用域内类型与非类型名称的遮蔽
- 两阶段查找：模板中依赖与非依赖名称的不同处理时机
- 重载解析：多个同名声明的选择机制

### 11.2 实践建议

- 明确限定：在可能的情况下使用明确的限定名称，提高代码可读性
- 谨慎使用 using：避免在头文件中使用 `using namespace` 声明
- 合理利用 ADL：为自定义类型提供相关的运算符和函数
- 模板中的 typename：在模板中访问依赖类型时使用 `typename`
- 避免名称冲突：使用命名空间组织代码，避免全局名称污染

### 11.3 历史演进

C++ 名称查找机制在不同标准版本中持续演进：

- C++98/03：建立了基本的名称查找框架
- C++11：完善了 ADL 和模板相关的查找规则
- C++14：进一步澄清了复杂的查找场景
- C++17：改进了模板中的名称查找
- C++20：引入概念，提供了更强的模板约束机制
- C++23：继续完善边界情况的处理

通过深入理解和正确应用名称查找规则，开发者可以编写出更加清晰、健壮和可维护的 C++ 程序，避免因名称解析错误导致的编译问题和运行时错误。
