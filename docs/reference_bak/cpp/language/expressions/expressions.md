# C++ 表达式详解

  [1. 基本语法与语义](#1-基本语法与语义)  
  
  [2. 表达式分类与值类别](#2-表达式分类与值类别)  
  
  [3. 运算符详解](#3-运算符详解)  
  
  [4. 运算符优先级与结合性](#4-运算符优先级与结合性)  
  
  [5. 表达式求值顺序](#5-表达式求值顺序)  
  
  [6. 类型转换与强制转换](#6-类型转换与强制转换)  
  
  [7. 常量表达式](#7-常量表达式)  
  
  [8. 主要表达式](#8-主要表达式)  
  
  [9. 未计算的表达式](#9-未计算的表达式)  
  
  [10. 常见应用场景](#10-常见应用场景)  
  
  [11. 最佳实践与代码规范](#11-最佳实践与代码规范)  
  
  [12. 标准参考](#12-标准参考)  
  
  [13. 总结](#13-总结)

## 1. 基本语法与语义

### 1.1 表达式定义

C++ 表达式是运算符及其操作数的序列，用于指定计算。表达式可以产生结果、副作用，或者指定对象或函数。每个 C++ 表达式都有两个独立的属性：类型和值类别。

```cpp
#include <iostream>
#include <string>

void basic_expression_examples() {
    std::cout << "=== Basic Expression Examples ===\n";
    
    // 1. 算术表达式
    int a = 10, b = 5;
    int sum = a + b;           // 加法表达式
    int diff = a - b;          // 减法表达式
    int product = a * b;       // 乘法表达式
    int quotient = a / b;      // 除法表达式
    int remainder = a % b;     // 取模表达式
    
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Difference: " << diff << "\n";
    std::cout << "Product: " << product << "\n";
    std::cout << "Quotient: " << quotient << "\n";
    std::cout << "Remainder: " << remainder << "\n";
    
    // 2. 关系表达式
    bool greater = a > b;       // 大于表达式
    bool equal = a == b;        // 等于表达式
    bool less_equal = a <= b;   // 小于等于表达式
    
    std::cout << "Greater: " << greater << "\n";
    std::cout << "Equal: " << equal << "\n";
    std::cout << "Less or equal: " << less_equal << "\n";
    
    // 3. 逻辑表达式
    bool logical_and = (a > 0) && (b > 0);  // 逻辑与表达式
    bool logical_or = (a > 0) || (b < 0);   // 逻辑或表达式
    bool logical_not = !(a == b);           // 逻辑非表达式
    
    std::cout << "Logical AND: " << logical_and << "\n";
    std::cout << "Logical OR: " << logical_or << "\n";
    std::cout << "Logical NOT: " << logical_not << "\n";
    
    // 4. 赋值表达式
    int x = 42;                // 简单赋值
    int y = x += 10;           // 复合赋值
    int z = (x = y = 5);       // 链式赋值
    
    std::cout << "x: " << x << ", y: " << y << ", z: " << z << "\n";
    
    // 5. 条件表达式
    int max = (a > b) ? a : b; // 条件表达式
    std::cout << "Max of " << a << " and " << b << " is " << max << "\n";
    
    // 6. 副作用表达式
    int counter = 0;
    int result = (++counter) + (counter++); // 包含副作用的表达式
    std::cout << "Counter: " << counter << ", Result: " << result << "\n";
    
    // 7. 函数调用表达式
    std::string message = "Hello, World!";
    size_t length = message.length(); // 函数调用表达式
    std::cout << "Message: " << message << ", Length: " << length << "\n";
    
    // 8. C++ 特有的表达式
    auto lambda = [](int n) { return n * 2; }; // Lambda 表达式
    int lambda_result = lambda(21);
    std::cout << "Lambda result: " << lambda_result << "\n";
}

int main() {
    basic_expression_examples();
    return 0;
}
```

### 1.2 表达式的本质

表达式是 C++ 语言计算的基本单元，它将值、变量、运算符和函数组合起来形成可执行的计算单元。C++ 表达式相比 C 语言具有更丰富的特性，包括类型安全、运算符重载、Lambda 表达式等。

```cpp
#include <iostream>
#include <memory>
#include <vector>

class ExpressionDemo {
private:
    int value_;
    static int instance_count_;
    
public:
    ExpressionDemo(int value) : value_(value) {
        std::cout << "ExpressionDemo instance " << ++instance_count_ << " constructed with value " << value_ << "\n";
    }
    
    ~ExpressionDemo() {
        std::cout << "ExpressionDemo instance " << instance_count_-- << " destructed with value " << value_ << "\n";
    }
    
    int getValue() const { return value_; }
    void setValue(int value) { value_ = value; }
    
    // 重载运算符
    ExpressionDemo operator+(const ExpressionDemo& other) const {
        return ExpressionDemo(value_ + other.value_);
    }
    
    ExpressionDemo& operator+=(int increment) {
        value_ += increment;
        return *this;
    }
};

int ExpressionDemo::instance_count_ = 0;

void expression_nature() {
    std::cout << "=== Expression Nature ===\n";
    
    // 1. 表达式产生结果
    int arithmetic_result = 2 + 3 * 4; // 结果为 14
    std::cout << "Arithmetic result: " << arithmetic_result << "\n";
    
    // 2. 表达式产生副作用
    static int side_effect_counter = 0;
    auto side_effect_lambda = [&]() {
        return ++side_effect_counter;
    };
    
    int side_effect_result = side_effect_lambda() + side_effect_lambda();
    std::cout << "Side effect counter: " << side_effect_counter << ", Result: " << side_effect_result << "\n";
    
    // 3. 表达式指定对象
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin() + 2; // 表达式指定迭代器位置
    std::cout << "Vector element: " << *it << "\n";
    
    // 4. 复杂表达式组合
    std::string str1 = "Hello";
    std::string str2 = "World";
    int complex_result = static_cast<int>((str1.length() + str2.length()) * 2);
    std::cout << "Complex expression result: " << complex_result << "\n";
    
    // 5. 嵌套表达式
    int nested_result = ((1 + 2) * (3 + 4)) - (5 / 2);
    std::cout << "Nested expression result: " << nested_result << "\n";
    
    // 6. 运算符重载表达式
    ExpressionDemo obj1(10);
    ExpressionDemo obj2(20);
    ExpressionDemo obj3 = obj1 + obj2; // 使用重载的 + 运算符
    std::cout << "Operator overload result: " << obj3.getValue() << "\n";
    
    // 7. 复合赋值表达式
    ExpressionDemo obj4(5);
    obj4 += 15; // 使用重载的 += 运算符
    std::cout << "Compound assignment result: " << obj4.getValue() << "\n";
    
    // 8. Lambda 表达式
    auto multiply = [](int a, int b) { return a * b; };
    int lambda_result = multiply(6, 7);
    std::cout << "Lambda expression result: " << lambda_result << "\n";
}

int main() {
    expression_nature();
    return 0;
}
```

### 1.3 表达式与语句的关系

表达式是构成语句的基础，但表达式本身不是完整的语句。C++ 中的表达式可以更加复杂，支持运算符重载、模板、Lambda 等特性。

```cpp
#include <iostream>
#include <functional>

void expression_statement_relationship() {
    std::cout << "=== Expression Statement Relationship ===\n";
    
    // 1. 表达式语句（表达式后加分号）
    int x = 42;           // 声明语句包含表达式
    x = x + 1;            // 赋值表达式语句
    std::cout << "x = " << x << "\n"; // 函数调用表达式语句
    
    // 2. 控制结构中的表达式
    if (x > 0) {          // 条件表达式
        std::cout << "Positive\n";
    }
    
    for (int i = 0; i < 3; i++) { // 初始化、条件、迭代表达式
        std::cout << "Loop iteration " << i << "\n";
    }
    
    // 3. 复杂表达式语句
    int a = 1, b = 2, c = 3;
    int result = (a++, b++, c++, a + b + c); // 逗号表达式
    std::cout << "Comma expression result: " << result << "\n";
    
    // 4. 表达式序列
    int sequence_result = (a = 10, b = 20, c = 30, a + b + c);
    std::cout << "Sequence expression result: " << sequence_result << "\n";
    
    // 5. Lambda 表达式语句
    auto print_lambda = [](const std::string& msg) {
        std::cout << "Lambda message: " << msg << "\n";
    };
    print_lambda("Hello from lambda");
    
    // 6. 函数对象表达式
    std::function<int(int, int)> adder = [](int x, int y) { return x + y; };
    int func_obj_result = adder(15, 25);
    std::cout << "Function object result: " << func_obj_result << "\n";
    
    // 7. 模板表达式
    template<typename T>
    T max_value(T a, T b) {
        return (a > b) ? a : b;
    }
    
    int int_max = max_value(10, 20);
    double double_max = max_value(3.14, 2.71);
    std::cout << "Max int: " << int_max << ", Max double: " << double_max << "\n";
    
    // 8. 异常处理表达式
    try {
        throw std::runtime_error("Test exception");
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
}

int main() {
    expression_statement_relationship();
    return 0;
}
```

## 2. 表达式分类与值类别

### 2.1 值类别详解

C++11 引入了更精细的值类别分类，包括左值(lvalue)、右值(rvalue)、广义左值(glvalue)、纯右值(prvalue)和将亡值(xvalue)。

```cpp
#include <iostream>
#include <memory>
#include <string>

void value_categories() {
    std::cout << "=== Value Categories ===\n";
    
    // 1. 左值表达式 (lvalue)
    int variable = 42;           // variable 是左值
    int* ptr = &variable;        // &variable 产生左值的地址
    variable = 100;              // 左值可以出现在赋值的左侧
    
    std::cout << "Variable address: " << ptr << "\n";
    std::cout << "Variable value: " << variable << "\n";
    
    // 2. 纯右值表达式 (prvalue)
    int result = 2 + 3;          // 2 + 3 是纯右值表达式
    int temp = (variable + 5);   // (variable + 5) 是纯右值表达式
    
    std::cout << "Result: " << result << "\n";
    std::cout << "Temp: " << temp << "\n";
    
    // 3. 将亡值表达式 (xvalue)
    std::string str = "Hello";
    std::string&& rvalue_ref = std::move(str); // std::move 产生将亡值
    std::cout << "Moved string: " << rvalue_ref << "\n";
    
    // 4. 函数指示符
    void (*func_ptr)(void) = value_categories; // 函数名是函数指示符
    std::cout << "Function address: " << reinterpret_cast<void*>(func_ptr) << "\n";
    
    // 5. 数组表达式
    int array[5] = {1, 2, 3, 4, 5};
    int* array_ptr = array;      // 数组名在大多数上下文中转换为指针
    
    std::cout << "Array elements: ";
    for (int i = 0; i < 5; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
    
    // 6. 结构体成员表达式
    struct Point {
        int x, y;
    } point = {10, 20};
    
    point.x = 30;                // point.x 是左值
    int member_value = point.y;  // point.y 是左值，但用作右值
    
    std::cout << "Point: (" << point.x << ", " << point.y << ")\n";
    
    // 7. 智能指针表达式
    auto smart_ptr = std::make_unique<int>(100);
    int& ref = *smart_ptr;       // 解引用产生左值
    ref = 200;                   // 可以赋值
    std::cout << "Smart pointer value: " << *smart_ptr << "\n";
    
    // 8. Lambda 表达式（纯右值）
    auto lambda = [](int x) { return x * 2; };
    int lambda_result = lambda(21); // Lambda 调用是纯右值
    std::cout << "Lambda result: " << lambda_result << "\n";
}

int main() {
    value_categories();
    return 0;
}
```

### 2.2 表达式类型分类

```cpp
#include <iostream>
#include <type_traits>
#include <string>

void expression_type_classification() {
    std::cout << "=== Expression Type Classification ===\n";
    
    // 1. 整型表达式
    int int_expr = 42 + 8;
    unsigned int uint_expr = 100U - 20U;
    
    std::cout << "Int expression: " << int_expr << "\n";
    std::cout << "Unsigned int expression: " << uint_expr << "\n";
    
    // 2. 浮点型表达式
    float float_expr = 3.14f * 2.0f;
    double double_expr = 2.718281828 * 3.0;
    
    std::cout << "Float expression: " << float_expr << "\n";
    std::cout << "Double expression: " << double_expr << "\n";
    
    // 3. 指针表达式
    int array[5] = {1, 2, 3, 4, 5};
    int* ptr_expr = array + 2;   // 指针算术表达式
    int deref_expr = *(array + 1); // 解引用表达式
    
    std::cout << "Pointer expression result: " << *ptr_expr << "\n";
    std::cout << "Dereference expression result: " << deref_expr << "\n";
    
    // 4. 字符表达式
    char char_expr = 'A' + 1;    // 字符算术表达式
    int ascii_value = 'Z';       // 字符到整数的转换
    
    std::cout << "Character expression: " << char_expr << "\n";
    std::cout << "ASCII value: " << ascii_value << "\n";
    
    // 5. 逻辑表达式
    bool logical_expr1 = (5 > 3) && (2 < 4);  // 逻辑与表达式
    bool logical_expr2 = (1 == 2) || (3 != 4); // 逻辑或表达式
    bool logical_expr3 = !(5 == 5);            // 逻辑非表达式
    
    std::cout << "Logical AND: " << logical_expr1 << "\n";
    std::cout << "Logical OR: " << logical_expr2 << "\n";
    std::cout << "Logical NOT: " << logical_expr3 << "\n";
    
    // 6. 字符串表达式
    std::string str1 = "Hello";
    std::string str2 = "World";
    std::string concat_expr = str1 + " " + str2; // 字符串连接表达式
    
    std::cout << "String concatenation: " << concat_expr << "\n";
    
    // 7. 模板表达式
    template<typename T>
    T template_expr(T a, T b) {
        return a + b;
    }
    
    int template_int_result = template_expr(10, 20);
    double template_double_result = template_expr(3.14, 2.71);
    
    std::cout << "Template int result: " << template_int_result << "\n";
    std::cout << "Template double result: " << template_double_result << "\n";
    
    // 8. 自动类型推导表达式
    auto auto_int = 42;
    auto auto_double = 3.14;
    auto auto_string = std::string("Auto");
    
    std::cout << "Auto int: " << auto_int << "\n";
    std::cout << "Auto double: " << auto_double << "\n";
    std::cout << "Auto string: " << auto_string << "\n";
    
    // 9. decltype 表达式
    int x = 42;
    decltype(x) y = x; // y 的类型与 x 相同
    decltype(x + 1.0) z = x + 1.0; // z 的类型是 double
    
    std::cout << "Decltype y: " << y << "\n";
    std::cout << "Decltype z: " << z << "\n";
}

int main() {
    expression_type_classification();
    return 0;
}
```

## 3. 运算符详解

### 3.1 算术运算符

```cpp
#include <iostream>
#include <cmath>
#include <complex>

void arithmetic_operators() {
    std::cout << "=== Arithmetic Operators ===\n";
    
    // 1. 基本算术运算符
    int a = 15, b = 4;
    int add_result = a + b;      // 加法
    int sub_result = a - b;      // 减法
    int mul_result = a * b;      // 乘法
    int div_result = a / b;      // 除法（整数除法）
    int mod_result = a % b;      // 取模
    
    std::cout << "Addition: " << a << " + " << b << " = " << add_result << "\n";
    std::cout << "Subtraction: " << a << " - " << b << " = " << sub_result << "\n";
    std::cout << "Multiplication: " << a << " * " << b << " = " << mul_result << "\n";
    std::cout << "Division: " << a << " / " << b << " = " << div_result << "\n";
    std::cout << "Modulus: " << a << " % " << b << " = " << mod_result << "\n";
    
    // 2. 浮点数算术
    double x = 15.5, y = 4.2;
    double f_add = x + y;
    double f_sub = x - y;
    double f_mul = x * y;
    double f_div = x / y;
    
    std::cout << "Float addition: " << x << " + " << y << " = " << f_add << "\n";
    std::cout << "Float subtraction: " << x << " - " << y << " = " << f_sub << "\n";
    std::cout << "Float multiplication: " << x << " * " << y << " = " << f_mul << "\n";
    std::cout << "Float division: " << x << " / " << y << " = " << f_div << "\n";
    
    // 3. 一元算术运算符
    int positive = +a;           // 正号
    int negative = -a;           // 负号
    int increment_pre = ++a;     // 前置递增
    int decrement_post = a--;    // 后置递减
    
    std::cout << "Positive: +" << (a-1) << " = " << positive << "\n";
    std::cout << "Negative: -" << (a+1) << " = " << negative << "\n";
    std::cout << "Pre-increment: ++" << (a-1) << " = " << increment_pre << "\n";
    std::cout << "Post-decrement: " << a << "-- = " << decrement_post << "\n";
    
    // 4. 位运算符
    int bitwise_a = 12;  // 1100 in binary
    int bitwise_b = 10;  // 1010 in binary
    
    int bitwise_and = bitwise_a & bitwise_b;    // 按位与
    int bitwise_or = bitwise_a | bitwise_b;     // 按位或
    int bitwise_xor = bitwise_a ^ bitwise_b;    // 按位异或
    int bitwise_not = ~bitwise_a;               // 按位取反
    int left_shift = bitwise_a << 1;            // 左移
    int right_shift = bitwise_a >> 1;           // 右移
    
    std::cout << "Bitwise AND: " << bitwise_a << " & " << bitwise_b << " = " << bitwise_and 
              << " (binary: 1100 & 1010 = 1000)\n";
    std::cout << "Bitwise OR: " << bitwise_a << " | " << bitwise_b << " = " << bitwise_or 
              << " (binary: 1100 | 1010 = 1110)\n";
    std::cout << "Bitwise XOR: " << bitwise_a << " ^ " << bitwise_b << " = " << bitwise_xor 
              << " (binary: 1100 ^ 1010 = 0110)\n";
    std::cout << "Bitwise NOT: ~" << bitwise_a << " = " << bitwise_not << "\n";
    std::cout << "Left shift: " << bitwise_a << " << 1 = " << left_shift << "\n";
    std::cout << "Right shift: " << bitwise_a << " >> 1 = " << right_shift << "\n";
    
    // 5. 复数算术运算符
    std::complex<double> c1(3.0, 4.0);  // 3 + 4i
    std::complex<double> c2(1.0, 2.0);  // 1 + 2i
    
    auto c_add = c1 + c2;
    auto c_sub = c1 - c2;
    auto c_mul = c1 * c2;
    auto c_div = c1 / c2;
    
    std::cout << "Complex addition: " << c1 << " + " << c2 << " = " << c_add << "\n";
    std::cout << "Complex subtraction: " << c1 << " - " << c2 << " = " << c_sub << "\n";
    std::cout << "Complex multiplication: " << c1 << " * " << c2 << " = " << c_mul << "\n";
    std::cout << "Complex division: " << c1 << " / " << c2 << " = " << c_div << "\n";
}

int main() {
    arithmetic_operators();
    return 0;
}
```

### 3.2 关系和逻辑运算符

```cpp
#include <iostream>
#include <string>
#include <compare>

void relational_logical_operators() {
    std::cout << "=== Relational and Logical Operators ===\n";
    
    // 1. 关系运算符
    int a = 10, b = 20;
    
    bool equal = (a == b);        // 等于
    bool not_equal = (a != b);    // 不等于
    bool less = (a < b);          // 小于
    bool greater = (a > b);       // 大于
    bool less_equal = (a <= b);   // 小于等于
    bool greater_equal = (a >= b); // 大于等于
    
    std::cout << "Equal: " << a << " == " << b << " = " << equal << "\n";
    std::cout << "Not equal: " << a << " != " << b << " = " << not_equal << "\n";
    std::cout << "Less: " << a << " < " << b << " = " << less << "\n";
    std::cout << "Greater: " << a << " > " << b << " = " << greater << "\n";
    std::cout << "Less or equal: " << a << " <= " << b << " = " << less_equal << "\n";
    std::cout << "Greater or equal: " << a << " >= " << b << " = " << greater_equal << "\n";
    
    // 2. 逻辑运算符
    bool x = true, y = false;
    
    bool logical_and = x && y;   // 逻辑与（短路求值）
    bool logical_or = x || y;    // 逻辑或（短路求值）
    bool logical_not = !x;       // 逻辑非
    
    std::cout << "Logical AND: " << x << " && " << y << " = " << logical_and << "\n";
    std::cout << "Logical OR: " << x << " || " << y << " = " << logical_or << "\n";
    std::cout << "Logical NOT: !" << x << " = " << logical_not << "\n";
    
    // 3. 短路求值演示
    int counter = 0;
    
    // 逻辑与短路：如果第一个操作数为假，不会计算第二个操作数
    bool short_circuit_and = (false && (++counter > 0));
    std::cout << "Short circuit AND - Counter: " << counter << ", Result: " << short_circuit_and << "\n";
    
    // 逻辑或短路：如果第一个操作数为真，不会计算第二个操作数
    bool short_circuit_or = (true || (++counter > 0));
    std::cout << "Short circuit OR - Counter: " << counter << ", Result: " << short_circuit_or << "\n";
    
    // 4. 复杂关系表达式
    int score = 85;
    bool is_passing = (score >= 60) && (score <= 100);
    bool is_excellent = (score >= 90) && (score <= 100);
    bool is_valid_score = (score >= 0) && (score <= 100);
    
    std::cout << "Score: " << score << "\n";
    std::cout << "Is passing: " << is_passing << "\n";
    std::cout << "Is excellent: " << is_excellent << "\n";
    std::cout << "Is valid score: " << is_valid_score << "\n";
    
    // 5. 字符串比较
    std::string str1 = "Hello";
    std::string str2 = "World";
    
    bool str_equal = (str1 == str2);
    bool str_less = (str1 < str2);
    bool str_greater = (str1 > str2);
    
    std::cout << "String comparison:\n";
    std::cout << "\"" << str1 << "\" == \"" << str2 << "\" = " << str_equal << "\n";
    std::cout << "\"" << str1 << "\" < \"" << str2 << "\" = " << str_less << "\n";
    std::cout << "\"" << str1 << "\" > \"" << str2 << "\" = " << str_greater << "\n";
    
    // 6. C++20 三路比较运算符
    #if __cplusplus >= 202002L
    auto compare_result = a <=> b;
    if (compare_result < 0) {
        std::cout << a << " is less than " << b << "\n";
    } else if (compare_result > 0) {
        std::cout << a << " is greater than " << b << "\n";
    } else {
        std::cout << a << " is equal to " << b << "\n";
    }
    #endif
}

int main() {
    relational_logical_operators();
    return 0;
}
```

### 3.3 赋值运算符

```cpp
#include <iostream>
#include <memory>
#include <vector>

class Assignable {
private:
    int value_;
    
public:
    Assignable(int value = 0) : value_(value) {
        std::cout << "Assignable constructed with value " << value_ << "\n";
    }
    
    Assignable(const Assignable& other) : value_(other.value_) {
        std::cout << "Assignable copy constructed from " << other.value_ << "\n";
    }
    
    Assignable& operator=(const Assignable& other) {
        std::cout << "Assignable assigned from " << other.value_ << "\n";
        if (this != &other) {
            value_ = other.value_;
        }
        return *this;
    }
    
    Assignable& operator+=(int increment) {
        value_ += increment;
        std::cout << "Assignable += " << increment << ", new value: " << value_ << "\n";
        return *this;
    }
    
    int getValue() const { return value_; }
};

void assignment_operators() {
    std::cout << "=== Assignment Operators ===\n";
    
    // 1. 基本赋值运算符
    int a = 10;                  // 简单赋值
    std::cout << "Initial value of a: " << a << "\n";
    
    // 2. 复合赋值运算符
    a += 5;                      // 等价于 a = a + 5
    std::cout << "After a += 5: " << a << "\n";
    
    a -= 3;                      // 等价于 a = a - 3
    std::cout << "After a -= 3: " << a << "\n";
    
    a *= 2;                      // 等价于 a = a * 2
    std::cout << "After a *= 2: " << a << "\n";
    
    a /= 4;                      // 等价于 a = a / 4
    std::cout << "After a /= 4: " << a << "\n";
    
    a %= 3;                      // 等价于 a = a % 3
    std::cout << "After a %= 3: " << a << "\n";
    
    // 3. 位运算复合赋值
    int b = 12;                  // 1100 in binary
    std::cout << "Initial value of b: " << b << " (binary: 1100)\n";
    
    b &= 10;                     // 等价于 b = b & 10 (1010)
    std::cout << "After b &= 10: " << b << " (binary: 1000)\n";
    
    b |= 5;                      // 等价于 b = b | 5 (0101)
    std::cout << "After b |= 5: " << b << " (binary: 1101)\n";
    
    b ^= 3;                      // 等价于 b = b ^ 3 (0011)
    std::cout << "After b ^= 3: " << b << " (binary: 1110)\n";
    
    b <<= 1;                     // 等价于 b = b << 1
    std::cout << "After b <<= 1: " << b << " (binary: 11100)\n";
    
    b >>= 2;                     // 等价于 b = b >> 2
    std::cout << "After b >>= 2: " << b << " (binary: 111)\n";
    
    // 4. 链式赋值
    int x, y, z;
    x = y = z = 42;              // 从右到左结合
    std::cout << "Chain assignment: x = " << x << ", y = " << y << ", z = " << z << "\n";
    
    // 5. 赋值表达式的值
    int result = (x = 100);      // 赋值表达式返回被赋的值
    std::cout << "Assignment expression result: " << result << "\n";
    
    // 6. 指针赋值
    int value = 42;
    int* ptr1 = &value;
    int* ptr2;
    ptr2 = ptr1;                 // 指针赋值
    std::cout << "Pointer assignment: *ptr1 = " << *ptr1 << ", *ptr2 = " << *ptr2 << "\n";
    
    // 7. 智能指针赋值
    auto smart_ptr1 = std::make_shared<int>(100);
    std::shared_ptr<int> smart_ptr2;
    smart_ptr2 = smart_ptr1;     // 智能指针赋值
    std::cout << "Smart pointer assignment: *smart_ptr1 = " << *smart_ptr1 
              << ", *smart_ptr2 = " << *smart_ptr2 << "\n";
    std::cout << "Shared pointer use count: " << smart_ptr1.use_count() << "\n";
    
    // 8. 自定义类赋值
    Assignable obj1(10);
    Assignable obj2(20);
    obj2 = obj1;                 // 使用重载的赋值运算符
    std::cout << "Object assignment result: " << obj2.getValue() << "\n";
    
    // 9. 复合赋值与自定义类
    Assignable obj3(5);
    obj3 += 15;                  // 使用重载的复合赋值运算符
    std::cout << "Compound assignment result: " << obj3.getValue() << "\n";
    
    // 10. 移动赋值
    std::vector<int> vec1 = {1, 2, 3};
    std::vector<int> vec2;
    vec2 = std::move(vec1);      // 移动赋值
    std::cout << "Vector1 size after move: " << vec1.size() << "\n";
    std::cout << "Vector2 size after move: " << vec2.size() << "\n";
}

int main() {
    assignment_operators();
    return 0;
}
```

### 3.4 递增和递减运算符

```cpp
#include <iostream>
#include <memory>

class Counter {
private:
    int value_;
    
public:
    Counter(int value = 0) : value_(value) {
        std::cout << "Counter constructed with value " << value_ << "\n";
    }
    
    // 前置递增
    Counter& operator++() {
        std::cout << "Prefix increment: " << value_ << " -> " << (value_ + 1) << "\n";
        ++value_;
        return *this;
    }
    
    // 后置递增
    Counter operator++(int) {
        Counter temp(*this);
        std::cout << "Postfix increment: " << value_ << " -> " << (value_ + 1) << "\n";
        ++value_;
        return temp;
    }
    
    // 前置递减
    Counter& operator--() {
        std::cout << "Prefix decrement: " << value_ << " -> " << (value_ - 1) << "\n";
        --value_;
        return *this;
    }
    
    // 后置递减
    Counter operator--(int) {
        Counter temp(*this);
        std::cout << "Postfix decrement: " << value_ << " -> " << (value_ - 1) << "\n";
        --value_;
        return temp;
    }
    
    int getValue() const { return value_; }
};

void increment_decrement_operators() {
    std::cout << "=== Increment and Decrement Operators ===\n";
    
    // 1. 前置递增/递减
    int a = 5;
    int pre_inc_result = ++a;    // 先递增，再使用值
    std::cout << "Pre-increment: a = 5, ++a = " << pre_inc_result << ", a = " << a << "\n";
    
    int b = 5;
    int pre_dec_result = --b;    // 先递减，再使用值
    std::cout << "Pre-decrement: b = 5, --b = " << pre_dec_result << ", b = " << b << "\n";
    
    // 2. 后置递增/递减
    int c = 5;
    int post_inc_result = c++;   // 先使用值，再递增
    std::cout << "Post-increment: c = 5, c++ = " << post_inc_result << ", c = " << c << "\n";
    
    int d = 5;
    int post_dec_result = d--;   // 先使用值，再递减
    std::cout << "Post-decrement: d = 5, d-- = " << post_dec_result << ", d = " << d << "\n";
    
    // 3. 在表达式中的使用
    int x = 10, y = 20;
    int expr1 = (++x) + (y++);   // x先递增，y后递增
    std::cout << "Expression (++x) + (y++): x = " << x << ", y = " << y << ", result = " << expr1 << "\n";
    
    int p = 1, q = 2;
    int expr2 = (p++) + (++q);   // p后递增，q先递增
    std::cout << "Expression (p++) + (++q): p = " << p << ", q = " << q << ", result = " << expr2 << "\n";
    
    // 4. 指针递增/递减
    int array[5] = {10, 20, 30, 40, 50};
    int* ptr = array;
    
    std::cout << "Pointer operations:\n";
    std::cout << "Initial: *ptr = " << *ptr << "\n";
    std::cout << "After ptr++: *(ptr++) = " << *(ptr++) << ", *ptr = " << *ptr << "\n";
    std::cout << "After ++ptr: *(++ptr) = " << *(++ptr) << ", *ptr = " << *ptr << "\n";
    
    // 5. 复杂表达式中的递增/递减
    int i = 0;
    int complex_expr = (++i) + (i++) + (++i);
    std::cout << "Complex expression (++i) + (i++) + (++i): i = " << i << ", result = " << complex_expr << "\n";
    
    // 6. 递增/递减与数组访问
    int numbers[5] = {1, 2, 3, 4, 5};
    int* num_ptr = numbers;
    int sum = 0;
    
    for (int j = 0; j < 5; j++) {
        sum += *(num_ptr++);     // 递增指针并累加值
    }
    std::cout << "Sum using pointer increment: " << sum << "\n";
    
    // 7. 递增/递减的安全使用
    int safe_counter = 0;
    while (safe_counter < 3) {
        std::cout << "Safe counter iteration: " << safe_counter++ << "\n";
    }
    
    // 8. 自定义类的递增/递减运算符
    Counter counter(3);
    Counter pre_inc_counter = ++counter;    // 前置递增
    Counter post_inc_counter = counter++;   // 后置递增
    
    std::cout << "Final counter value: " << counter.getValue() << "\n";
    std::cout << "Pre-increment result value: " << pre_inc_counter.getValue() << "\n";
    std::cout << "Post-increment result value: " << post_inc_counter.getValue() << "\n";
}

int main() {
    increment_decrement_operators();
    return 0;
}
```

### 3.5 条件运算符

```cpp
#include <iostream>
#include <string>
#include <memory>

void conditional_operator() {
    std::cout << "=== Conditional Operator ===\n";
    
    // 1. 基本条件运算符
    int a = 10, b = 20;
    int max = (a > b) ? a : b;   // 如果 a > b 则返回 a，否则返回 b
    int min = (a < b) ? a : b;   // 如果 a < b 则返回 a，否则返回 b
    
    std::cout << "Max of " << a << " and " << b << " is " << max << "\n";
    std::cout << "Min of " << a << " and " << b << " is " << min << "\n";
    
    // 2. 条件运算符的嵌套
    int x = 15, y = 25, z = 5;
    int largest = (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
    std::cout << "Largest of " << x << ", " << y << ", " << z << " is " << largest << "\n";
    
    // 3. 条件运算符与不同类型
    int score = 85;
    char grade = (score >= 90) ? 'A' : 
                 (score >= 80) ? 'B' : 
                 (score >= 70) ? 'C' : 
                 (score >= 60) ? 'D' : 'F';
    std::cout << "Score " << score << " gets grade " << grade << "\n";
    
    // 4. 条件运算符与指针
    int value1 = 100, value2 = 200;
    int* ptr1 = &value1;
    int* ptr2 = &value2;
    int* selected_ptr = (value1 > value2) ? ptr1 : ptr2;
    std::cout << "Selected pointer points to: " << *selected_ptr << "\n";
    
    // 5. 条件运算符的副作用
    int counter = 0;
    int result = (counter++ > 0) ? counter : (counter += 10);
    std::cout << "Counter: " << counter << ", Result: " << result << "\n";
    
    // 6. 条件运算符与函数调用
    auto positive_func = [](int x) { return x * 2; };
    auto negative_func = [](int x) { return x * -1; };
    
    int number = -5;
    int processed = (number > 0) ? positive_func(number) : negative_func(number);
    std::cout << "Number " << number << " processed to " << processed << "\n";
    
    // 7. 复杂条件表达式
    int age = 25;
    int income = 50000;
    std::string status = (age >= 18) ? 
                        ((income >= 30000) ? "Eligible for loan" : "Income too low") : 
                        "Underage";
    std::cout << "Age " << age << ", Income " << income << ": " << status << "\n";
    
    // 8. 条件运算符与智能指针
    auto ptr_a = std::make_shared<int>(10);
    auto ptr_b = std::make_shared<int>(20);
    auto selected_smart_ptr = (ptr_a.use_count() > ptr_b.use_count()) ? ptr_a : ptr_b;
    std::cout << "Selected smart pointer value: " << *selected_smart_ptr << "\n";
    
    // 9. 条件运算符与字符串
    std::string str1 = "Hello";
    std::string str2 = "World";
    std::string selected_string = (str1.length() > str2.length()) ? str1 : str2;
    std::cout << "Longer string: " << selected_string << "\n";
    
    // 10. 条件运算符与Lambda表达式
    auto condition_lambda = [](bool condition) {
        return condition ? 
               [](int x) { return x * 2; } : 
               [](int x) { return x * 3; };
    };
    
    auto func = condition_lambda(true);
    int lambda_result = func(5);
    std::cout << "Lambda conditional result: " << lambda_result << "\n";
}

int main() {
    conditional_operator();
    return 0;
}
```

### 3.6 逗号运算符

```cpp
#include <iostream>
#include <vector>
#include <memory>

void comma_operator() {
    std::cout << "=== Comma Operator ===\n";
    
    // 1. 基本逗号运算符
    int a = 1, b = 2, c = 3;
    int result = (a++, b++, c++, a + b + c);  // 从左到右求值，返回最后一个表达式的值
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << ", result = " << result << "\n";
    
    // 2. 逗号运算符在 for 循环中的使用
    std::cout << "For loop with comma operator:\n";
    for (int i = 0, j = 10; i < 5; i++, j--) {
        std::cout << "i = " << i << ", j = " << j << "\n";
    }
    
    // 3. 逗号运算符与副作用
    int x = 5, y = 10;
    int comma_result = (std::cout << "Side effect 1\n", 
                        std::cout << "Side effect 2\n", 
                        x + y);
    std::cout << "Comma result: " << comma_result << "\n";
    
    // 4. 复杂逗号表达式
    int p = 1, q = 2, r = 3;
    int complex_comma = (p += 5, q *= 2, r -= 1, p + q + r);
    std::cout << "p = " << p << ", q = " << q << ", r = " << r << ", complex comma result = " << complex_comma << "\n";
    
    // 5. 逗号运算符与条件表达式
    int condition = 1;
    int ternary_with_comma = condition ? (std::cout << "True branch\n", 42) : (std::cout << "False branch\n", 24);
    std::cout << "Ternary with comma result: " << ternary_with_comma << "\n";
    
    // 6. 逗号运算符在宏定义中的使用
    #define SWAP(a, b) do { auto temp = a; a = b; b = temp; } while(0)
    #define COMPLEX_OPERATION(x, y) ((x)++, (y)++, (x) + (y))
    
    int m = 10, n = 20;
    int macro_result = COMPLEX_OPERATION(m, n);
    std::cout << "After macro: m = " << m << ", n = " << n << ", result = " << macro_result << "\n";
    
    // 7. 逗号运算符的求值顺序保证
    int array[5] = {0};
    int index = 0;
    int sequence_result = (array[index++] = 1, 
                          array[index++] = 2, 
                          array[index++] = 3, 
                          index);
    std::cout << "Array elements: ";
    for (int i = 0; i < 3; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << "\nSequence result: " << sequence_result << "\n";
    
    // 8. 逗号运算符与智能指针
    auto ptr1 = std::make_shared<int>(100);
    auto ptr2 = std::make_shared<int>(200);
    int smart_ptr_result = (ptr1.reset(), ptr2.reset(), 42);
    std::cout << "Smart pointer comma result: " << smart_ptr_result << "\n";
    std::cout << "Ptr1 use count: " << (ptr1 ? ptr1.use_count() : 0) << "\n";
    std::cout << "Ptr2 use count: " << (ptr2 ? ptr2.use_count() : 0) << "\n";
    
    // 9. 逗号运算符与Lambda表达式
    auto lambda_comma = [](int a, int b) {
        return (a++, b++, a + b);
    };
    
    int lambda_result = lambda_comma(5, 10);
    std::cout << "Lambda comma result: " << lambda_result << "\n";
    
    // 10. 逗号运算符与容器操作
    std::vector<int> vec;
    int container_result = (vec.push_back(1), vec.push_back(2), vec.push_back(3), static_cast<int>(vec.size()));
    std::cout << "Container comma result: " << container_result << "\n";
    std::cout << "Vector elements: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

int main() {
    comma_operator();
    return 0;
}
```

## 4. 运算符优先级与结合性

### 4.1 运算符优先级表

```cpp
#include <iostream>
#include <cmath>

void operator_precedence() {
    std::cout << "=== Operator Precedence ===\n";
    
    // 1. 算术运算符优先级
    int a = 10, b = 5, c = 2;
    int result1 = a + b * c;     // * 优先级高于 +
    int result2 = (a + b) * c;   // 使用括号改变优先级
    std::cout << "a + b * c = " << a << " + " << b << " * " << c << " = " << result1 << "\n";
    std::cout << "(a + b) * c = (" << a << " + " << b << ") * " << c << " = " << result2 << "\n";
    
    // 2. 关系运算符与逻辑运算符优先级
    int x = 15, y = 10, z = 20;
    bool result3 = x > y && y < z; // 关系运算符优先级高于逻辑运算符
    bool result4 = (x > y) && (y < z); // 明确使用括号
    std::cout << "x > y && y < z = " << x << " > " << y << " && " << y << " < " << z << " = " << result3 << "\n";
    
    // 3. 赋值运算符优先级
    int p = 5, q = 10, r = 15;
    int result5 = p = q = r;     // 赋值运算符从右到左结合
    std::cout << "p = q = r = " << result5 << ", so p = " << p << ", q = " << q << ", r = " << r << "\n";
    
    // 4. 复杂表达式优先级
    int complex_result = 2 + 3 * 4 - 5 / 2;
    // 等价于: 2 + (3 * 4) - (5 / 2) = 2 + 12 - 2 = 12
    std::cout << "2 + 3 * 4 - 5 / 2 = " << complex_result << "\n";
    
    // 5. 位运算符优先级
    int bitwise_a = 12, bitwise_b = 10, bitwise_c = 5;
    int bitwise_result = bitwise_a & bitwise_b | bitwise_c;
    // 等价于: (bitwise_a & bitwise_b) | bitwise_c
    std::cout << "bitwise_a & bitwise_b | bitwise_c = (" << bitwise_a << " & " << bitwise_b << ") | " << bitwise_c << " = " << bitwise_result << "\n";
    
    // 6. 条件运算符优先级
    int condition_a = 10, condition_b = 5;
    int condition_result = condition_a > condition_b ? condition_a : condition_b + 10;
    // 等价于: (condition_a > condition_b) ? condition_a : (condition_b + 10)
    std::cout << "condition_a > condition_b ? condition_a : condition_b + 10 = " << condition_result << "\n";
    
    // 7. 逗号运算符优先级（最低）
    int comma_a = 1, comma_b = 2, comma_c = 3;
    int comma_result = comma_a++, comma_b++, comma_c++, comma_a + comma_b + comma_c;
    std::cout << "Comma operator result: " << comma_result << "\n";
    std::cout << "After comma operations: a = " << comma_a << ", b = " << comma_b << ", c = " << comma_c << "\n";
    
    // 8. 括号的使用（最高优先级）
    int paren_result = ((2 + 3) * 4) - (5 / (2 + 1));
    std::cout << "((2 + 3) * 4) - (5 / (2 + 1)) = " << paren_result << "\n";
    
    // 9. 成员访问运算符优先级
    struct Point {
        int x, y;
        int distance() const { return static_cast<int>(std::sqrt(x*x + y*y)); }
    };
    
    Point pt = {3, 4};
    int member_result = pt.x + pt.y * pt.distance(); // . 优先级高于 * 和 +
    std::cout << "Member access precedence: " << member_result << "\n";
    
    // 10. 函数调用与其它运算符的优先级
    auto func = [](int x) { return x * 2; };
    int func_result = func(3) + 4 * 5; // 函数调用优先级高于算术运算符
    std::cout << "Function call precedence: " << func_result << "\n";
}

int main() {
    operator_precedence();
    return 0;
}
```

### 4.2 结合性详解

```cpp
#include <iostream>
#include <memory>

void operator_associativity() {
    std::cout << "=== Operator Associativity ===\n";
    
    // 1. 左结合性（大多数运算符）
    int a = 10, b = 5, c = 2;
    int left_assoc = a - b - c;  // 等价于 (a - b) - c
    std::cout << "a - b - c = " << a << " - " << b << " - " << c << " = " << left_assoc 
              << " (equivalent to (" << a << " - " << b << ") - " << c << " = " << (a - b) - c << ")\n";
    
    // 2. 右结合性（赋值运算符）
    int x = 5, y = 10, z = 15;
    int right_assoc = x = y = z; // 等价于 x = (y = z)
    std::cout << "x = y = z = " << x << " = " << y << " = " << z << " = " << right_assoc 
              << " (equivalent to x = (y = " << z << ") = " << right_assoc << ")\n";
    
    // 3. 条件运算符的右结合性
    int condition1 = 1, condition2 = 0, condition3 = 1;
    int ternary_result = condition1 ? 10 : condition2 ? 20 : 30;
    // 等价于: condition1 ? 10 : (condition2 ? 20 : 30)
    std::cout << "condition1 ? 10 : condition2 ? 20 : 30 = " << ternary_result << "\n";
    
    // 4. 幂运算符的右结合性（通过函数模拟）
    // C++ 中没有内置的幂运算符，但可以演示右结合性概念
    auto power = [](int base, int exp) {
        int result = 1;
        for (int i = 0; i < exp; i++) {
            result *= base;
        }
        return result;
    };
    
    int power_result = power(2, power(2, 3)); // 2^(2^3) = 2^8 = 256
    std::cout << "Right associativity concept: 2^(2^3) = 2^8 = " << power_result << "\n";
    
    // 5. 复合赋值运算符的右结合性
    int value = 100;
    value += 5 *= 2;         // 等价于 value += (5 *= 2)
    std::cout << "value += 5 *= 2 = " << value << " (equivalent to value += (5 *= 2) = " << (100 + (5 * 2)) << ")\n";
    
    // 6. 一元运算符的右结合性
    int unary_value = 5;
    int unary_result = ++unary_value; // 前置递增是右结合的
    std::cout << "++unary_value = " << unary_result << "\n";
    
    // 7. 复杂表达式中的结合性
    int complex_a = 2, complex_b = 3, complex_c = 4;
    int complex_result = complex_a = complex_b = complex_c;
    std::cout << "Complex right associativity: a = b = c = " << complex_result 
              << ", so a = " << complex_a << ", b = " << complex_b << ", c = " << complex_c << "\n";
    
    // 8. 混合优先级和结合性的表达式
    int mixed_result = 2 + 3 * 4 - 5 / 2 + 1;
    // 求值顺序: 2 + (3 * 4) - (5 / 2) + 1 = 2 + 12 - 2 + 1 = 13
    std::cout << "Mixed precedence and associativity: 2 + 3 * 4 - 5 / 2 + 1 = " << mixed_result << "\n";
    
    // 9. 逻辑运算符的左结合性
    bool logical_a = true, logical_b = false, logical_c = true;
    bool logical_result = logical_a && logical_b && logical_c; // 等价于 (logical_a && logical_b) && logical_c
    std::cout << "Logical AND left associativity: " << logical_result << "\n";
    
    // 10. 位运算符的左结合性
    int bit_a = 12, bit_b = 10, bit_c = 5;
    int bit_result = bit_a & bit_b & bit_c; // 等价于 (bit_a & bit_b) & bit_c
    std::cout << "Bitwise AND left associativity: " << bit_result << "\n";
}

int main() {
    operator_associativity();
    return 0;
}
```

## 5. 表达式求值顺序

### 5.1 求值顺序规则

```cpp
#include <iostream>
#include <functional>

// 全局计数器用于演示求值顺序
static int evaluation_counter = 0;

int get_value(const std::string& name) {
    std::cout << "Getting value for " << name << ": " << ++evaluation_counter << "\n";
    return evaluation_counter;
}

void evaluation_order() {
    std::cout << "=== Evaluation Order ===\n";
    
    // 1. 顺序点和未定义行为
    evaluation_counter = 0;
    std::cout << "\n1. Undefined behavior example (avoid this):\n";
    int undefined_var = 10;
    // int undefined_result = undefined_var++ + undefined_var++; // 未定义行为！
    
    // 2. 顺序点保证的求值顺序
    evaluation_counter = 0;
    std::cout << "\n2. Sequence points (&&, ||, ?:, ,):\n";
    
    // 逻辑与的短路求值
    bool and_result = (get_value("A") > 0) && (get_value("B") > 0);
    std::cout << "AND result: " << and_result << "\n";
    
    evaluation_counter = 0;
    // 逻辑或的短路求值
    bool or_result = (get_value("C") < 0) || (get_value("D") > 0);
    std::cout << "OR result: " << or_result << "\n";
    
    // 3. 函数参数的求值顺序（未指定）
    std::cout << "\n3. Function parameter evaluation order (unspecified):\n";
    evaluation_counter = 0;
    
    auto print_three_values = [](int a, int b, int c) {
        std::cout << "Function received: " << a << ", " << b << ", " << c << "\n";
    };
    
    // 参数求值顺序是未指定的，可能是从左到右或从右到左
    print_three_values(get_value("Param1"), get_value("Param2"), get_value("Param3"));
    
    // 4. 逗号运算符的求值顺序（从左到右）
    evaluation_counter = 0;
    std::cout << "\n4. Comma operator evaluation order (left to right):\n";
    int comma_result = (get_value("Comma1"), get_value("Comma2"), get_value("Comma3"));
    std::cout << "Comma result: " << comma_result << "\n";
    
    // 5. 条件运算符的求值顺序
    evaluation_counter = 0;
    std::cout << "\n5. Conditional operator evaluation order:\n";
    int condition = 1;
    int ternary_result = condition ? get_value("TrueBranch") : get_value("FalseBranch");
    std::cout << "Ternary result: " << ternary_result << "\n";
    
    evaluation_counter = 0;
    condition = 0;
    int ternary_result2 = condition ? get_value("TrueBranch2") : get_value("FalseBranch2");
    std::cout << "Ternary result 2: " << ternary_result2 << "\n";
    
    // 6. 初始化列表的求值顺序（C++11 起从左到右）
    evaluation_counter = 0;
    std::cout << "\n6. Initialization list evaluation order:\n";
    int init_array[] = {get_value("Init1"), get_value("Init2"), get_value("Init3")};
    std::cout << "Array elements: " << init_array[0] << ", " << init_array[1] << ", " << init_array[2] << "\n";
    
    // 7. 成员初始化列表的求值顺序
    evaluation_counter = 0;
    std::cout << "\n7. Member initializer list evaluation order:\n";
    
    class MemberInitDemo {
    public:
        int a = get_value("MemberA");
        int b = get_value("MemberB");
        int c = get_value("MemberC");
        
        MemberInitDemo() {
            std::cout << "Constructor called\n";
        }
    };
    
    MemberInitDemo member_obj;
    std::cout << "Member values: " << member_obj.a << ", " << member_obj.b << ", " << member_obj.c << "\n";
    
    // 8. Lambda 捕获的求值顺序
    evaluation_counter = 0;
    std::cout << "\n8. Lambda capture evaluation order:\n";
    
    int lambda_capture1 = get_value("LambdaCapture1");
    int lambda_capture2 = get_value("LambdaCapture2");
    
    auto lambda = [lambda_capture1, lambda_capture2]() {
        std::cout << "Lambda captured values: " << lambda_capture1 << ", " << lambda_capture2 << "\n";
    };
    
    lambda();
}

int main() {
    evaluation_order();
    return 0;
}
```

### 5.2 副作用与求值顺序

```cpp
#include <iostream>
#include <vector>
#include <memory>

void side_effects_evaluation() {
    std::cout << "=== Side Effects and Evaluation Order ===\n";
    
    // 1. 修改和访问同一变量的副作用
    int a = 5;
    std::cout << "Initial a = " << a << "\n";
    
    // 安全的副作用使用
    int safe_result = (a += 3) + (a *= 2);  // 明确的顺序点
    std::cout << "After safe operations: a = " << a << ", result = " << safe_result << "\n";
    
    // 2. 容器访问中的副作用
    std::vector<int> vec = {10, 20, 30, 40, 50};
    size_t index = 0;
    std::cout << "Vector elements using side effects:\n";
    
    // 安全的容器遍历
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << "vec[" << i << "] = " << vec[i] << "\n";
    }
    
    // 3. 智能指针操作中的副作用
    auto ptr = std::make_shared<std::vector<int>>(std::vector<int>{1, 2, 3, 4, 5});
    std::cout << "Smart pointer operations with side effects:\n";
    
    for (size_t i = 0; i < ptr->size(); i++) {
        std::cout << "*ptr[" << i << "] = " << (*ptr)[i] << "\n";
    }
    
    // 4. 函数调用中的副作用
    static int global_counter = 0;
    
    auto increment_counter = []() {
        return ++global_counter;
    };
    
    std::cout << "\nFunction call side effects:\n";
    global_counter = 0;
    int func_result = increment_counter() + increment_counter() * increment_counter();
    std::cout << "Global counter: " << global_counter << ", Function result: " << func_result << "\n";
    
    // 5. 条件表达式中的副作用
    int condition_var = 10;
    std::cout << "\nConditional expression side effects:\n";
    int conditional_result = (condition_var > 5) ? 
                            (std::cout << "True branch executed\n", condition_var *= 2) : 
                            (std::cout << "False branch executed\n", condition_var /= 2);
    std::cout << "Condition var: " << condition_var << ", Result: " << conditional_result << "\n";
    
    // 6. 复合赋值中的副作用
    int compound_var = 100;
    std::cout << "\nCompound assignment side effects:\n";
    int compound_result = (compound_var += 10) * (compound_var -= 5);
    std::cout << "Compound var: " << compound_var << ", Result: " << compound_result << "\n";
    
    // 7. 避免未定义行为的技巧
    std::cout << "\nSafe practices to avoid undefined behavior:\n";
    
    // 使用临时变量避免同一表达式中多次修改
    int safe_var = 10;
    int temp = safe_var + 1;
    safe_var = temp;
    std::cout << "Safe modification: " << safe_var << "\n";
    
    // 使用顺序点分隔修改操作
    int sequence_var = 5;
    sequence_var++;  // 第一个修改
    sequence_var *= 2;  // 第二个修改（在顺序点之后）
    std::cout << "Sequence point safe: " << sequence_var << "\n";
    
    // 8. Lambda 表达式中的副作用
    int lambda_var = 0;
    auto lambda_with_side_effect = [&lambda_var]() {
        lambda_var += 10;
        std::cout << "Lambda side effect: lambda_var = " << lambda_var << "\n";
        return lambda_var;
    };
    
    std::cout << "\nLambda side effects:\n";
    int lambda_result1 = lambda_with_side_effect();
    int lambda_result2 = lambda_with_side_effect();
    std::cout << "Lambda results: " << lambda_result1 << ", " << lambda_result2 << "\n";
    
    // 9. 异常安全的副作用
    try {
        std::vector<int> safe_vec;
        safe_vec.push_back(1);
        safe_vec.push_back(2);
        std::cout << "Safe vector operations completed\n";
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }
}

int main() {
    side_effects_evaluation();
    return 0;
}
```

## 6. 类型转换与强制转换

### 6.1 隐式类型转换

```cpp
#include <iostream>
#include <string>
#include <memory>

void implicit_conversions() {
    std::cout << "=== Implicit Conversions ===\n";
    
    // 1. 整型提升
    char char_val = 65;
    short short_val = 1000;
    int int_result = char_val + short_val;  // char 和 short 都提升为 int
    std::cout << "char(" << static_cast<int>(char_val) << ") + short(" << short_val << ") = int(" << int_result << ")\n";
    
    // 2. 通常算术转换
    int int_val = 10;
    float float_val = 3.14f;
    double double_result = int_val + float_val;  // int 转换为 float，然后 float 转换为 double
    std::cout << "int(" << int_val << ") + float(" << float_val << ") = double(" << double_result << ")\n";
    
    // 3. 有符号和无符号整数转换
    int signed_val = -1;
    unsigned int unsigned_val = 1U;
    unsigned int mixed_result = signed_val + unsigned_val;  // signed 转换为 unsigned
    std::cout << "signed(" << signed_val << ") + unsigned(" << unsigned_val << ") = unsigned(" << mixed_result << ")\n";
    
    // 4. 浮点数到整数的转换
    double double_val = 3.7;
    int truncated = double_val;  // 截断小数部分
    std::cout << "double(" << double_val << ") converted to int = " << truncated << "\n";
    
    // 5. 整数到布尔的转换
    int non_zero = 42;
    int zero = 0;
    bool bool_from_non_zero = non_zero;  // 非零值转换为 true
    bool bool_from_zero = zero;          // 零值转换为 false
    std::cout << "int(" << non_zero << ") to bool = " << bool_from_non_zero << "\n";
    std::cout << "int(" << zero << ") to bool = " << bool_from_zero << "\n";
    
    // 6. 指针和整数的转换
    int array[5] = {1, 2, 3, 4, 5};
    int* ptr = array;
    ptr = ptr + 2;  // 指针算术
    std::cout << "Pointer arithmetic: array[2] = " << *ptr << "\n";
    
    // 7. void* 转换
    void* void_ptr = array;
    int* int_ptr = static_cast<int*>(void_ptr);  // void* 可以隐式转换为任何对象指针
    std::cout << "void* to int* conversion: *int_ptr = " << *int_ptr << "\n";
    
    // 8. 字符和整数的转换
    char letter = 'A';
    int ascii_value = letter;  // char 转换为 int
    char back_to_char = ascii_value;  // int 转换为 char
    std::cout << "char('" << letter << "') to int = " << ascii_value << ", back to char = '" << back_to_char << "'\n";
    
    // 9. 复杂表达式中的类型转换
    short s = 30000;
    long l = 50000L;
    long long_result = s + l;  // short 提升为 int，然后 int 转换为 long
    std::cout << "short(" << s << ") + long(" << l << ") = long(" << long_result << ")\n";
    
    // 10. 字符串转换
    std::string str = "Hello";
    const char* c_str = str.c_str();  // std::string 到 const char* 的转换
    std::cout << "std::string to const char*: " << c_str << "\n";
    
    // 11. 智能指针转换
    auto smart_ptr = std::make_shared<int>(42);
    std::shared_ptr<const int> const_smart_ptr = smart_ptr;  // 隐式转换为const版本
    std::cout << "Smart pointer conversion: " << *const_smart_ptr << "\n";
    
    // 12. 数组到指针的转换
    int arr[5] = {1, 2, 3, 4, 5};
    int* arr_ptr = arr;  // 数组名隐式转换为指向首元素的指针
    std::cout << "Array to pointer conversion: " << *arr_ptr << "\n";
}

int main() {
    implicit_conversions();
    return 0;
}
```

### 6.2 显式类型转换（强制转换）

```cpp
#include <iostream>
#include <string>
#include <memory>
#include <typeinfo>

void explicit_casts() {
    std::cout << "=== Explicit Casts ===\n";
    
    // 1. C 风格强制转换
    double double_val = 3.14159;
    int int_val = (int)double_val;  // C 风格强制转换
    std::cout << "C-style cast: double(" << double_val << ") to int = " << int_val << "\n";
    
    // 2. static_cast
    int large_int = 1000000;
    short short_val = static_cast<short>(large_int);  // 可能溢出
    std::cout << "static_cast: int(" << large_int << ") to short = " << short_val << "\n";
    
    // 3. const_cast
    const int const_val = 42;
    int& non_const_ref = const_cast<int&>(const_val);
    std::cout << "const_cast: const int(" << const_val << ") to non-const reference\n";
    
    // 4. reinterpret_cast
    int address_as_int = 0x12345678;
    int* restored_ptr = reinterpret_cast<int*>(address_as_int);
    std::cout << "reinterpret_cast: int(" << std::hex << address_as_int << std::dec << ") to pointer\n";
    
    // 5. dynamic_cast
    class Base {
    public:
        virtual ~Base() = default;
    };
    
    class Derived : public Base {
    public:
        int value = 100;
    };
    
    Base* base_ptr = new Derived();
    Derived* derived_ptr = dynamic_cast<Derived*>(base_ptr);
    if (derived_ptr) {
        std::cout << "dynamic_cast: successful, derived value = " << derived_ptr->value << "\n";
    }
    
    delete base_ptr;
    
    // 6. 不同类型的强制转换
    int negative = -1;
    unsigned int unsigned_negative = static_cast<unsigned int>(negative);
    std::cout << "int(" << negative << ") cast to unsigned int = " << unsigned_negative << "\n";
    
    // 7. 浮点数之间的转换
    long double ld_val = 123.456789012345L;
    float float_val = static_cast<float>(ld_val);
    std::cout << "long double(" << ld_val << ") cast to float = " << float_val << "\n";
    
    // 8. 指针强制转换
    int array[5] = {10, 20, 30, 40, 50};
    void* void_ptr = static_cast<void*>(array);
    int* int_ptr = static_cast<int*>(void_ptr);
    std::cout << "Pointer cast: array[0] = " << array[0] << ", *(int*)void_ptr = " << *int_ptr << "\n";
    
    // 9. 整数和指针之间的转换
    uintptr_t address_as_uint = reinterpret_cast<uintptr_t>(array);  // 安全的指针到整数转换
    int* restored_ptr2 = reinterpret_cast<int*>(address_as_uint);
    std::cout << "Address as uint: " << address_as_uint << ", restored pointer points to: " << *restored_ptr2 << "\n";
    
    // 10. 函数指针强制转换
    int add(int a, int b) { return a + b; }
    int (*func_ptr)(int, int) = static_cast<int (*)(int, int)>(add);
    int func_result = func_ptr(5, 3);
    std::cout << "Function pointer cast result: " << func_result << "\n";
    
    // 11. 复杂表达式中的强制转换
    double complex_expr = static_cast<double>(5 / 2) + 3.14;  // 整数除法后转换为 double
    double correct_expr = static_cast<double>(5) / 2 + 3.14;    // 正确的转换顺序
    std::cout << "Complex cast (wrong): " << complex_expr << "\n";
    std::cout << "Complex cast (correct): " << correct_expr << "\n";
    
    // 12. 避免精度损失的强制转换
    float precise_float = 3.14159265359f;
    double more_precise = static_cast<double>(precise_float);
    std::cout << "float(" << precise_float << ") cast to double = " << more_precise << "\n";
    
    // 13. 安全的强制转换实践
    unsigned char byte_val = 255;
    int safe_conversion = static_cast<int>(byte_val);
    std::cout << "unsigned char(" << static_cast<int>(byte_val) << ") safely cast to int = " << safe_conversion << "\n";
    
    // 14. typeid 和类型信息
    std::cout << "Type of int_val: " << typeid(int_val).name() << "\n";
    std::cout << "Type of double_val: " << typeid(double_val).name() << "\n";
}

int main() {
    explicit_casts();
    return 0;
}
```

## 7. 常量表达式

### 7.1 常量表达式定义与使用

```cpp
#include <iostream>
#include <array>
#include <type_traits>

void constant_expressions() {
    std::cout << "=== Constant Expressions ===\n";
    
    // 1. 整型常量表达式
    constexpr int ARRAY_SIZE = 10;  // 常量表达式
    std::array<int, ARRAY_SIZE> arr;      // 用于数组大小
    
    std::cout << "Array size: " << ARRAY_SIZE << "\n";
    
    // 2. 枚举常量表达式
    enum Color { RED = 1, GREEN = 2, BLUE = 4 };
    constexpr int color_flags = RED | GREEN;  // 常量表达式
    std::cout << "Color flags: " << color_flags << "\n";
    
    // 3. sizeof 表达式（常量表达式）
    constexpr size_t int_size = sizeof(int);  // 常量表达式
    std::cout << "Size of int: " << int_size << " bytes\n";
    
    // 4. 地址常量表达式
    static int static_var = 42;
    int* const ptr_to_static = &static_var;  // 地址常量
    std::cout << "Static variable address: " << ptr_to_static << "\n";
    
    // 5. 字符常量表达式
    constexpr char NEWLINE = '\n';
    constexpr char TAB = '\t';
    std::cout << "Character constants: newline=" << static_cast<int>(NEWLINE) 
              << ", tab=" << static_cast<int>(TAB) << NEWLINE;
    
    // 6. 浮点常量表达式
    constexpr double PI = 3.141592653589793238;
    constexpr float E = 2.718281828f;
    std::cout << "Mathematical constants: PI=" << PI << ", E=" << E << "\n";
    
    // 7. 复杂常量表达式
    constexpr int COMPLEX_CONST = (ARRAY_SIZE * 2) + (RED | GREEN);
    std::cout << "Complex constant: " << COMPLEX_CONST << "\n";
    
    // 8. 条件常量表达式
    constexpr int MAX_VALUE = (ARRAY_SIZE > 5) ? ARRAY_SIZE : 10;
    std::cout << "Max value: " << MAX_VALUE << "\n";
    
    // 9. 位运算常量表达式
    constexpr int BIT_MASK = (1 << 3) | (1 << 5);  // 设置第3位和第5位
    std::cout << "Bit mask: " << BIT_MASK << " (binary: ";
    for (int i = 7; i >= 0; i--) {
        std::cout << ((BIT_MASK >> i) & 1);
    }
    std::cout << ")\n";
    
    // 10. 字符串字面量作为常量表达式
    constexpr const char* MESSAGE = "Hello, World!";
    std::cout << "String constant: " << MESSAGE << "\n";
    
    // 11. constexpr 函数
    constexpr int factorial(int n) {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    }
    
    constexpr int FACTORIAL_5 = factorial(5);
    std::cout << "Factorial of 5: " << FACTORIAL_5 << "\n";
    
    // 12. constexpr 类
    class ConstexprPoint {
    private:
        int x_, y_;
    public:
        constexpr ConstexprPoint(int x, int y) : x_(x), y_(y) {}
        constexpr int getX() const { return x_; }
        constexpr int getY() const { return y_; }
        constexpr int distanceSquared() const { return x_ * x_ + y_ * y_; }
    };
    
    constexpr ConstexprPoint point(3, 4);
    constexpr int dist_sq = point.distanceSquared();
    std::cout << "Point(" << point.getX() << ", " << point.getY() 
              << ") distance squared: " << dist_sq << "\n";
    
    // 13. 模板参数中的常量表达式
    template<int N>
    struct ArrayWrapper {
        std::array<int, N> data;
    };
    
    ArrayWrapper<ARRAY_SIZE> wrapper;
    std::cout << "Template parameter array size: " << wrapper.data.size() << "\n";
}

int main() {
    constant_expressions();
    return 0;
}
```

### 7.2 常量表达式在编译时计算

```cpp
#include <iostream>
#include <type_traits>

// 1. 编译时常量函数
constexpr int compile_time_function(int x) {
    return x * x + 2 * x + 1;
}

void compile_time_constants() {
    std::cout << "=== Compile-time Constants ===\n";
    
    // 1. 静态断言（编译时检查）
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
    static_assert(std::is_same_v<int, int>, "Type check");
    
    // 2. 常量表达式优化
    constexpr int MULTIPLIER = 7;
    constexpr int CALCULATED = MULTIPLIER * 13 + 5;  // 编译时计算
    
    // 3. 数组大小常量表达式
    int compile_time_array[CALCULATED];  // 编译时确定大小
    std::cout << "Compile-time array size: " << sizeof(compile_time_array) / sizeof(compile_time_array[0]) << "\n";
    
    // 4. 枚举常量表达式
    enum {
        BUFFER_SIZE = 1024,
        ALIGNMENT = 16,
        PADDING = BUFFER_SIZE % ALIGNMENT ? ALIGNMENT - (BUFFER_SIZE % ALIGNMENT) : 0
    };
    
    std::cout << "Buffer size: " << BUFFER_SIZE << "\n";
    std::cout << "Alignment: " << ALIGNMENT << "\n";
    std::cout << "Padding: " << PADDING << "\n";
    
    // 5. 位字段大小常量表达式
    struct BitField {
        unsigned int flag1 : 1;
        unsigned int flag2 : 2;
        unsigned int value : 5;
        unsigned int reserved : (sizeof(unsigned int) * 8 - 8);
    };
    
    std::cout << "Bit field size: " << sizeof(struct BitField) << " bytes\n";
    
    // 6. 复杂的编译时常量表达式
    constexpr int PRIME1 = 17;
    constexpr int PRIME2 = 19;
    constexpr int PRODUCT = PRIME1 * PRIME2;
    constexpr int SUM = PRIME1 + PRIME2;
    constexpr int DIFFERENCE = (PRIME1 > PRIME2) ? (PRIME1 - PRIME2) : (PRIME2 - PRIME1);
    
    std::cout << "Prime1: " << PRIME1 << ", Prime2: " << PRIME2 << "\n";
    std::cout << "Product: " << PRODUCT << "\n";
    std::cout << "Sum: " << SUM << "\n";
    std::cout << "Difference: " << DIFFERENCE << "\n";
    
    // 7. 条件编译中的常量表达式
    #if PRODUCT > 300
        std::cout << "Product is large\n";
    #else
        std::cout << "Product is small\n";
    #endif
    
    // 8. constexpr 函数在编译时计算
    constexpr int COMPILE_TIME_RESULT = compile_time_function(5);
    std::cout << "Compile-time function result: " << COMPILE_TIME_RESULT << "\n";
    
    // 9. 模板元编程中的常量表达式
    template<int N>
    struct Factorial {
        static constexpr int value = N * Factorial<N-1>::value;
    };
    
    template<>
    struct Factorial<0> {
        static constexpr int value = 1;
    };
    
    constexpr int TEMPLATE_FACTORIAL = Factorial<5>::value;
    std::cout << "Template factorial of 5: " << TEMPLATE_FACTORIAL << "\n";
    
    // 10. 编译时字符串处理
    constexpr const char* compile_time_string = "Hello";
    constexpr size_t STRING_LENGTH = sizeof("Hello") - 1; // 减去null终止符
    std::cout << "Compile-time string: " << compile_time_string << ", Length: " << STRING_LENGTH << "\n";
}

int main() {
    compile_time_constants();
    return 0;
}
```

## 8. 主要表达式

### 8.1 常量和字面量

```cpp
#include <iostream>
#include <string>
#include <string_view>
#include <complex>

void constants_and_literals() {
    std::cout << "=== Constants and Literals ===\n";
    
    // 1. 整数常量
    int decimal = 42;           // 十进制
    int octal = 052;            // 八进制（以0开头）
    int hexadecimal = 0x2A;     // 十六进制（以0x开头）
    long long_int = 1000000L;   // 长整型后缀
    unsigned int unsigned_int = 4294967295U;  // 无符号后缀
    long long long_long = 1234567890123456789LL; // 长长整型后缀
    
    std::cout << "Decimal: " << decimal << "\n";
    std::cout << "Octal 052 = Decimal " << octal << "\n";
    std::cout << "Hexadecimal 0x2A = Decimal " << hexadecimal << "\n";
    std::cout << "Long: " << long_int << "\n";
    std::cout << "Unsigned: " << unsigned_int << "\n";
    std::cout << "Long long: " << long_long << "\n";
    
    // 2. 浮点常量
    float float_literal = 3.14f;        // float 后缀
    double double_literal = 3.14159;    // 默认 double
    long double long_double_literal = 3.141592653589793238L;  // long double 后缀
    
    std::cout << "Float: " << float_literal << "\n";
    std::cout << "Double: " << double_literal << "\n";
    std::cout << "Long double: " << long_double_literal << "\n";
    
    // 3. 科学计数法
    double scientific1 = 1.23e10;       // 1.23 × 10^10
    double scientific2 = 4.56E-5;       // 4.56 × 10^-5
    float scientific3 = 7.89e-3f;       // float 类型
    
    std::cout << "Scientific notation: " << scientific1 << ", " << scientific2 << ", " << scientific3 << "\n";
    
    // 4. 字符常量
    char single_char = 'A';
    char escape_char = '\n';            // 换行符
    char octal_char = '\101';           // 八进制表示的 'A'
    char hex_char = '\x41';             // 十六进制表示的 'A'
    wchar_t wide_char = L'W';           // 宽字符
    
    std::cout << "Character constants: '" << single_char << "', escape char (newline), octal '\\101' = '" 
              << octal_char << "', hex '\\x41' = '" << hex_char << "'\n";
    
    // 5. 字符串字面量
    const char* c_string_literal = "Hello, World!";
    std::string string_literal = "Hello, World!";
    const wchar_t* wide_string = L"Hello, Wide World!";
    
    std::cout << "C string literal: " << c_string_literal << "\n";
    std::cout << "std::string literal: " << string_literal << "\n";
    std::wcout << L"Wide string: " << wide_string << L"\n";
    
    // 6. 相邻字符串字面量的连接
    const char* concatenated = "Hello, " "World" "!";  // 编译时连接
    std::cout << "Concatenated string: " << concatenated << "\n";
    
    // 7. 空字符串和空字符
    const char* empty_string = "";
    char null_char = '\0';
    
    std::cout << "Empty string length: " << strlen(empty_string) << "\n";
    std::cout << "Null character: " << static_cast<int>(null_char) << "\n";
    
    // 8. 特殊字符常量
    char tab = '\t';
    char backslash = '\\';
    char quote = '\'';
    char double_quote = '\"';
    
    std::cout << "Special characters: tab='" << tab << "', backslash='" << backslash 
              << "', quote='" << quote << "', double quote='" << double_quote << "'\n";
    
    // 9. 原始字符串字面量 (C++11)
    const char* raw_string = R"(This is a "raw" string with \n and other characters)";
    std::cout << "Raw string: " << raw_string << "\n";
    
    // 10. 用户定义字面量 (C++11)
    constexpr long double operator"" _deg(long double deg) {
        return deg * 3.141592653589793238L / 180;
    }
    
    constexpr long double angle = 90.0_deg;
    std::cout << "90 degrees in radians: " << angle << "\n";
    
    // 11. 复数字面量 (C++14)
    std::complex<double> complex_literal(3.0, 4.0);
    std::cout << "Complex literal: " << complex_literal << "\n";
    
    // 12. 布尔字面量
    bool true_literal = true;
    bool false_literal = false;
    
    std::cout << "Boolean literals: true=" << true_literal << ", false=" << false_literal << "\n";
    
    // 13. nullptr 字面量 (C++11)
    int* null_ptr = nullptr;
    std::cout << "nullptr: " << null_ptr << "\n";
    
    // 14. 字符串视图字面量 (C++17)
    #if __cplusplus >= 201703L
    std::string_view sv = "Hello, string_view!";
    std::cout << "string_view: " << sv << "\n";
    #endif
}

int main() {
    constants_and_literals();
    return 0;
}
```

### 8.2 标识符和函数名

```cpp
#include <iostream>
#include <string>
#include <memory>
#include <functional>

// 全局变量
static int global_counter = 0;

// 函数声明
int calculate_sum(int a, int b);
void display_result(int result);
static void internal_function();

class IdentifierDemo {
private:
    int value_;
    static int instance_count_;
    
public:
    IdentifierDemo(int value) : value_(value) {
        std::cout << "IdentifierDemo instance " << ++instance_count_ << " constructed with value " << value_ << "\n";
    }
    
    ~IdentifierDemo() {
        std::cout << "IdentifierDemo instance " << instance_count_-- << " destructed with value " << value_ << "\n";
    }
    
    int getValue() const { return value_; }
    void setValue(int value) { value_ = value; }
    
    // 静态成员函数
    static int getInstanceCount() { return instance_count_; }
};

int IdentifierDemo::instance_count_ = 0;

void identifiers_and_function_names() {
    std::cout << "=== Identifiers and Function Names ===\n";
    
    // 1. 变量标识符
    int local_variable = 42;
    const int CONSTANT_VALUE = 100;
    static int static_variable = 200;
    
    std::cout << "Local variable: " << local_variable << "\n";
    std::cout << "Constant value: " << CONSTANT_VALUE << "\n";
    std::cout << "Static variable: " << static_variable << "\n";
    
    // 2. 函数标识符
    int sum = calculate_sum(10, 20);
    display_result(sum);
    
    // 3. 函数指针标识符
    int (*function_ptr)(int, int) = calculate_sum;
    int ptr_result = function_ptr(5, 15);
    std::cout << "Function pointer result: " << ptr_result << "\n";
    
    // 4. 数组标识符
    int array[5] = {1, 2, 3, 4, 5};
    int* array_ptr = array;  // 数组名作为指针
    
    std::cout << "Array elements: ";
    for (int i = 0; i < 5; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
    
    // 5. 结构体标识符
    struct Point {
        int x, y;
        Point(int x_val, int y_val) : x(x_val), y(y_val) {}
        
        double distance() const {
            return std::sqrt(x * x + y * y);
        }
    };
    
    Point point1(10, 20);
    Point point2(30, 40);
    
    std::cout << "Point1: (" << point1.x << ", " << point1.y << ")\n";
    std::cout << "Point2: (" << point2.x << ", " << point2.y << ")\n";
    std::cout << "Point1 distance from origin: " << point1.distance() << "\n";
    
    // 6. 联合体标识符
    union Data {
        int i;
        float f;
        char str[20];
    };
    
    Data data;
    data.i = 100;
    std::cout << "Union integer: " << data.i << "\n";
    
    data.f = 3.14f;
    std::cout << "Union float: " << data.f << "\n";
    
    // 7. 枚举标识符
    enum Color { RED, GREEN, BLUE };
    enum class ModernColor { Red, Green, Blue }; // 作用域枚举 (C++11)
    
    Color favorite_color = GREEN;
    ModernColor modern_color = ModernColor::Red;
    
    std::cout << "Favorite color: " << favorite_color << "\n";
    // std::cout << "Modern color: " << modern_color << "\n"; // 需要显式转换
    
    // 8. 内部函数调用
    internal_function();
    
    // 9. Lambda 表达式标识符
    auto lambda_func = [](int x, int y) { return x + y; };
    int lambda_result = lambda_func(10, 20);
    std::cout << "Lambda result: " << lambda_result << "\n";
    
    // 10. 函数对象标识符
    std::function<int(int, int)> func_obj = [](int x, int y) { return x * y; };
    int func_obj_result = func_obj(5, 6);
    std::cout << "Function object result: " << func_obj_result << "\n";
    
    // 11. 智能指针标识符
    auto smart_ptr = std::make_unique<IdentifierDemo>(42);
    std::cout << "Smart pointer value: " << smart_ptr->getValue() << "\n";
    
    // 12. 模板标识符
    template<typename T>
    T template_func(T a, T b) {
        return a + b;
    }
    
    int template_int_result = template_func(10, 20);
    double template_double_result = template_func(3.14, 2.71);
    
    std::cout << "Template int result: " << template_int_result << "\n";
    std::cout << "Template double result: " << template_double_result << "\n";
    
    // 13. 自动类型推导标识符
    auto auto_int = 42;
    auto auto_double = 3.14;
    auto auto_string = std::string("Auto");
    
    std::cout << "Auto int: " << auto_int << "\n";
    std::cout << "Auto double: " << auto_double << "\n";
    std::cout << "Auto string: " << auto_string << "\n";
    
    // 14. decltype 标识符
    int x = 42;
    decltype(x) y = x; // y 的类型与 x 相同
    decltype(x + 1.0) z = x + 1.0; // z 的类型是 double
    
    std::cout << "Decltype y: " << y << "\n";
    std::cout << "Decltype z: " << z << "\n";
    
    // 15. 类成员标识符
    IdentifierDemo obj(100);
    std::cout << "Object value: " << obj.getValue() << "\n";
    std::cout << "Static member function result: " << IdentifierDemo::getInstanceCount() << "\n";
}

// 函数定义
int calculate_sum(int a, int b) {
    return a + b;
}

void display_result(int result) {
    std::cout << "Calculation result: " << result << "\n";
}

static void internal_function() {
    global_counter++;
    std::cout << "Internal function called, global counter: " << global_counter << "\n";
}

int main() {
    identifiers_and_function_names();
    return 0;
}
```

## 9. 未计算的表达式

### 9.1 sizeof 运算符

```cpp
#include <iostream>
#include <array>
#include <memory>

void unevaluated_expressions() {
    std::cout << "=== Unevaluated Expressions ===\n";
    
    // 1. sizeof 基本用法
    size_t int_size = sizeof(int);
    size_t char_size = sizeof(char);
    size_t double_size = sizeof(double);
    size_t pointer_size = sizeof(int*);
    
    std::cout << "Size of int: " << int_size << " bytes\n";
    std::cout << "Size of char: " << char_size << " bytes\n";
    std::cout << "Size of double: " << double_size << " bytes\n";
    std::cout << "Size of int*: " << pointer_size << " bytes\n";
    
    // 2. sizeof 数组
    int array[10];
    size_t array_size = sizeof(array);
    size_t element_count = sizeof(array) / sizeof(array[0]);
    
    std::cout << "Array total size: " << array_size << " bytes\n";
    std::cout << "Array element count: " << element_count << "\n";
    
    // 3. sizeof 结构体
    struct Person {
        char name[50];
        int age;
        double height;
    };
    
    size_t struct_size = sizeof(struct Person);
    std::cout << "Size of struct Person: " << struct_size << " bytes\n";
    
    // 4. sizeof 未计算表达式
    int x = 42;
    size_t expr_size = sizeof(x + 1);  // x + 1 不会被计算
    std::cout << "Size of expression (x + 1): " << expr_size << " bytes\n";
    
    // 5. sizeof 函数调用（不执行）
    size_t func_size = sizeof(std::cout << "This won't be printed\n");  // cout 操作不会被执行
    std::cout << "Size of cout return value: " << func_size << " bytes\n";
    
    // 6. sizeof 字符串字面量
    size_t string_size = sizeof("Hello, World!");
    std::cout << "Size of string literal: " << string_size << " bytes\n";
    
    // 7. sizeof 指针和数组的区别
    char str[] = "Hello";
    char* ptr = str;
    
    size_t array_char_size = sizeof(str);    // 包含 '\0' 的完整大小
    size_t pointer_char_size = sizeof(ptr);  // 指针大小
    
    std::cout << "Size of char array: " << array_char_size << " bytes\n";
    std::cout << "Size of char pointer: " << pointer_char_size << " bytes\n";
    
    // 8. sizeof 位字段
    struct BitFields {
        unsigned int a : 3;
        unsigned int b : 4;
        unsigned int c : 5;
    };
    
    size_t bitfield_size = sizeof(struct BitFields);
    std::cout << "Size of bitfield struct: " << bitfield_size << " bytes\n";
    
    // 9. sizeof 与对齐
    struct AlignedStruct {
        char a;
        int b;
        char c;
    };
    
    size_t aligned_size = sizeof(struct AlignedStruct);
    std::cout << "Size of aligned struct: " << aligned_size << " bytes\n";
    
    // 10. sizeof 在模板中的使用
    template<typename T>
    size_t get_size() {
        return sizeof(T);
    }
    
    std::cout << "Size of int (template): " << get_size<int>() << " bytes\n";
    std::cout << "Size of double (template): " << get_size<double>() << " bytes\n";
    
    // 11. sizeof 与智能指针
    auto smart_ptr = std::make_unique<int>(42);
    size_t smart_ptr_size = sizeof(smart_ptr);
    std::cout << "Size of unique_ptr: " << smart_ptr_size << " bytes\n";
    
    // 12. sizeof 与容器
    std::array<int, 10> std_array;
    size_t std_array_size = sizeof(std_array);
    std::cout << "Size of std::array: " << std_array_size << " bytes\n";
    
    // 13. sizeof 与Lambda表达式
    auto lambda = [](int x) { return x * 2; };
    size_t lambda_size = sizeof(lambda);
    std::cout << "Size of lambda: " << lambda_size << " bytes\n";
    
    // 14. constexpr sizeof
    constexpr size_t CONSTEXPR_SIZE = sizeof(int);
    std::cout << "Constexpr sizeof: " << CONSTEXPR_SIZE << " bytes\n";
}

int main() {
    unevaluated_expressions();
    return 0;
}
```

### 9.2 alignof 运算符（C++11）

```cpp
#include <iostream>
#include <memory>

#if __cplusplus >= 201103L
void alignof_operator() {
    std::cout << "=== alignof Operator (C++11) ===\n";
    
    // 1. 基本 alignof 用法
    size_t int_align = alignof(int);
    size_t char_align = alignof(char);
    size_t double_align = alignof(double);
    size_t pointer_align = alignof(int*);
    
    std::cout << "Alignment of int: " << int_align << "\n";
    std::cout << "Alignment of char: " << char_align << "\n";
    std::cout << "Alignment of double: " << double_align << "\n";
    std::cout << "Alignment of int*: " << pointer_align << "\n";
    
    // 2. 结构体对齐要求
    struct SimpleStruct {
        char a;
        int b;
        double c;
    };
    
    size_t struct_align = alignof(struct SimpleStruct);
    std::cout << "Alignment of struct: " << struct_align << "\n";
    
    // 3. 使用 alignas 指定对齐
    struct alignas(16) AlignedStruct {
        int data[4];
    };
    
    size_t specified_align = alignof(AlignedStruct);
    std::cout << "Alignment of alignas(16) struct: " << specified_align << "\n";
    
    // 4. 最大对齐要求
    size_t max_align = alignof(std::max_align_t);
    std::cout << "Maximum alignment: " << max_align << "\n";
    
    // 5. 数组对齐
    int array[10];
    size_t array_align = alignof(array);
    std::cout << "Alignment of int array: " << array_align << "\n";
    
    // 6. 对齐与大小的关系
    struct PaddedStruct {
        char a;
        int b;
    };
    
    size_t padded_size = sizeof(struct PaddedStruct);
    size_t padded_align = alignof(struct PaddedStruct);
    std::cout << "Padded struct size: " << padded_size << ", alignment: " << padded_align << "\n";
    
    // 7. 函数参数的对齐（未计算）
    int dummy = 42;
    size_t param_align = alignof(decltype(dummy + 1));  // 表达式不被计算
    std::cout << "Alignment of expression (dummy + 1): " << param_align << "\n";
    
    // 8. 智能指针对齐
    auto smart_ptr = std::make_unique<int>(42);
    size_t smart_ptr_align = alignof(decltype(smart_ptr));
    std::cout << "Alignment of unique_ptr: " << smart_ptr_align << "\n";
    
    // 9. 模板中的 alignof
    template<typename T>
    size_t get_alignment() {
        return alignof(T);
    }
    
    std::cout << "Alignment of int (template): " << get_alignment<int>() << "\n";
    std::cout << "Alignment of double (template): " << get_alignment<double>() << "\n";
    
    // 10. constexpr alignof
    constexpr size_t CONSTEXPR_ALIGN = alignof(int);
    std::cout << "Constexpr alignof: " << CONSTEXPR_ALIGN << "\n";
}
#else
void alignof_operator() {
    std::cout << "=== alignof Operator (C++11 features not available) ===\n";
    std::cout << "This compiler does not support C++11 alignof operator\n";
}
#endif

int main() {
    alignof_operator();
    return 0;
}
```

## 10. 常见应用场景

### 10.1 数学计算表达式

```cpp
#include <iostream>
#include <cmath>
#include <complex>
#include <vector>

void mathematical_expressions() {
    std::cout << "=== Mathematical Expressions ===\n";
    
    // 1. 基本数学运算
    double a = 10.5, b = 3.2;
    double sum = a + b;
    double difference = a - b;
    double product = a * b;
    double quotient = a / b;
    
    std::cout << "Basic operations:\n";
    std::cout << a << " + " << b << " = " << sum << "\n";
    std::cout << a << " - " << b << " = " << difference << "\n";
    std::cout << a << " * " << b << " = " << product << "\n";
    std::cout << a << " / " << b << " = " << quotient << "\n";
    
    // 2. 复杂数学表达式
    double x = 2.0, y = 3.0;
    double complex_expr = (x * x + y * y) / (x - y) + std::sqrt(x * y);
    std::cout << "Complex expression: (" << x << "² + " << y << "²) / (" << x << " - " << y 
              << ") + √(" << x << " × " << y << ") = " << complex_expr << "\n";
    
    // 3. 三角函数表达式
    double angle = M_PI / 4;  // 45 degrees in radians
    double sin_val = std::sin(angle);
    double cos_val = std::cos(angle);
    double tan_val = std::tan(angle);
    
    std::cout << "Trigonometric functions:\n";
    std::cout << "sin(π/4) = " << sin_val << "\n";
    std::cout << "cos(π/4) = " << cos_val << "\n";
    std::cout << "tan(π/4) = " << tan_val << "\n";
    
    // 4. 指数和对数表达式
    double base = 2.0, exponent = 3.0;
    double power = std::pow(base, exponent);
    double natural_log = std::log(power);
    double log10_val = std::log10(power);
    
    std::cout << "Exponential and logarithmic functions:\n";
    std::cout << base << "^" << exponent << " = " << power << "\n";
    std::cout << "ln(" << power << ") = " << natural_log << "\n";
    std::cout << "log10(" << power << ") = " << log10_val << "\n";
    
    // 5. 取整和取模表达式
    double value = 3.7;
    double floor_val = std::floor(value);
    double ceil_val = std::ceil(value);
    double round_val = std::round(value);
    
    std::cout << "Rounding functions:\n";
    std::cout << "floor(" << value << ") = " << floor_val << "\n";
    std::cout << "ceil(" << value << ") = " << ceil_val << "\n";
    std::cout << "round(" << value << ") = " << round_val << "\n";
    
    // 6. 绝对值表达式
    double negative = -15.5;
    double absolute = std::abs(negative);
    std::cout << "Absolute value: |" << negative << "| = " << absolute << "\n";
    
    // 7. 最大值和最小值表达式
    double val1 = 10.5, val2 = 7.8;
    double max_val = std::fmax(val1, val2);
    double min_val = std::fmin(val1, val2);
    std::cout << "Max and min: max(" << val1 << ", " << val2 << ") = " << max_val 
              << ", min(" << val1 << ", " << val2 << ") = " << min_val << "\n";
    
    // 8. 复合数学表达式
    double radius = 5.0;
    double area = M_PI * radius * radius;
    double circumference = 2 * M_PI * radius;
    std::cout << "Circle calculations: radius=" << radius << ", area=" << area 
              << ", circumference=" << circumference << "\n";
    
    // 9. 复数表达式
    std::complex<double> c1(3.0, 4.0);  // 3 + 4i
    std::complex<double> c2(1.0, 2.0);  // 1 + 2i
    
    auto c_add = c1 + c2;
    auto c_sub = c1 - c2;
    auto c_mul = c1 * c2;
    auto c_div = c1 / c2;
    
    std::cout << "Complex arithmetic:\n";
    std::cout << c1 << " + " << c2 << " = " << c_add << "\n";
    std::cout << c1 << " - " << c2 << " = " << c_sub << "\n";
    std::cout << c1 << " * " << c2 << " = " << c_mul << "\n";
    std::cout << c1 << " / " << c2 << " = " << c_div << "\n";
    
    // 10. 向量数学表达式
    std::vector<double> vec1 = {1.0, 2.0, 3.0};
    std::vector<double> vec2 = {4.0, 5.0, 6.0};
    
    double dot_product = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        dot_product += vec1[i] * vec2[i];
    }
    
    std::cout << "Vector dot product: ";
    for (size_t i = 0; i < vec1.size(); ++i) {
        std::cout << vec1[i] << (i < vec1.size() - 1 ? "×" : "");
    }
    std::cout << " + ";
    for (size_t i = 0; i < vec2.size(); ++i) {
        std::cout << vec2[i] << (i < vec2.size() - 1 ? "×" : "");
    }
    std::cout << " = " << dot_product << "\n";
}

int main() {
    mathematical_expressions();
    return 0;
}
```

### 10.2 位操作表达式

```cpp
#include <iostream>
#include <bitset>
#include <cstdint>

void bitwise_expressions() {
    std::cout << "=== Bitwise Expressions ===\n";
    
    // 1. 基本位运算
    uint8_t a = 0b10101010;  // 170 in decimal
    uint8_t b = 0b11001100;  // 204 in decimal
    
    uint8_t and_result = a & b;    // 按位与
    uint8_t or_result = a | b;     // 按位或
    uint8_t xor_result = a ^ b;    // 按位异或
    uint8_t not_result = ~a;       // 按位取反
    
    std::cout << "Basic bitwise operations:\n";
    std::cout << "a = 0b" << std::bitset<8>(a) << " = " << static_cast<unsigned>(a) << "\n";
    std::cout << "b = 0b" << std::bitset<8>(b) << " = " << static_cast<unsigned>(b) << "\n";
    std::cout << "a & b = 0b" << std::bitset<8>(and_result) << " = " << static_cast<unsigned>(and_result) << "\n";
    std::cout << "a | b = 0b" << std::bitset<8>(or_result) << " = " << static_cast<unsigned>(or_result) << "\n";
    std::cout << "a ^ b = 0b" << std::bitset<8>(xor_result) << " = " << static_cast<unsigned>(xor_result) << "\n";
    std::cout << "~a = 0b" << std::bitset<8>(not_result) << " = " << static_cast<unsigned>(not_result) << "\n";
    
    // 2. 位移操作
    uint8_t value = 0b00001111;  // 15 in decimal
    uint8_t left_shift = value << 2;   // 左移2位
    uint8_t right_shift = value >> 1;  // 右移1位
    
    std::cout << "\nBit shift operations:\n";
    std::cout << "value = 0b" << std::bitset<8>(value) << " = " << static_cast<unsigned>(value) << "\n";
    std::cout << "value << 2 = 0b" << std::bitset<8>(left_shift) << " = " << static_cast<unsigned>(left_shift) << "\n";
    std::cout << "value >> 1 = 0b" << std::bitset<8>(right_shift) << " = " << static_cast<unsigned>(right_shift) << "\n";
    
    // 3. 位字段操作
    struct BitFlags {
        uint8_t flag1 : 1;
        uint8_t flag2 : 1;
        uint8_t flag3 : 1;
        uint8_t flag4 : 1;
        uint8_t data : 4;
    };
    
    union BitUnion {
        struct BitFlags flags;
        uint8_t byte;
    };
    
    union BitUnion bit_union = { .byte = 0b10101101 };
    std::cout << "\nBit field operations:\n";
    std::cout << "Byte: 0b" << std::bitset<8>(bit_union.byte) << "\n";
    std::cout << "Flag1: " << static_cast<unsigned>(bit_union.flags.flag1) 
              << ", Flag2: " << static_cast<unsigned>(bit_union.flags.flag2)
              << ", Flag3: " << static_cast<unsigned>(bit_union.flags.flag3)
              << ", Flag4: " << static_cast<unsigned>(bit_union.flags.flag4) << "\n";
    std::cout << "Data: 0b" << std::bitset<4>(bit_union.flags.data) 
              << " = " << static_cast<unsigned>(bit_union.flags.data) << "\n";
    
    // 4. 位掩码操作
    uint8_t mask = 0b11110000;
    uint8_t data = 0b10101010;
    uint8_t masked = data & mask;      // 应用掩码
    uint8_t cleared = data & ~mask;    // 清除掩码位
    uint8_t set = data | mask;         // 设置掩码位
    
    std::cout << "\nBit mask operations:\n";
    std::cout << "Mask: 0b" << std::bitset<8>(mask) << "\n";
    std::cout << "Data: 0b" << std::bitset<8>(data) << "\n";
    std::cout << "Masked: 0b" << std::bitset<8>(masked) << " = " << static_cast<unsigned>(masked) << "\n";
    std::cout << "Cleared: 0b" << std::bitset<8>(cleared) << " = " << static_cast<unsigned>(cleared) << "\n";
    std::cout << "Set: 0b" << std::bitset<8>(set) << " = " << static_cast<unsigned>(set) << "\n";
    
    // 5. 位测试和设置
    uint8_t test_value = 0b00000000;
    
    // 设置特定位
    test_value |= (1 << 3);  // 设置第3位
    test_value |= (1 << 7);  // 设置第7位
    
    std::cout << "\nBit setting operations:\n";
    std::cout << "After setting bits 3 and 7: 0b" << std::bitset<8>(test_value) << "\n";
    
    // 测试特定位
    bool bit3_set = (test_value & (1 << 3)) != 0;
    bool bit5_set = (test_value & (1 << 5)) != 0;
    
    std::cout << "Bit 3 is " << (bit3_set ? "set" : "clear") << "\n";
    std::cout << "Bit 5 is " << (bit5_set ? "set" : "clear") << "\n";
    
    // 6. 位清除和切换
    uint8_t clear_toggle = test_value;
    
    // 清除特定位
    clear_toggle &= ~(1 << 7);  // 清除第7位
    
    // 切换特定位
    clear_toggle ^= (1 << 3);   // 切换第3位
    
    std::cout << "\nBit clearing and toggling:\n";
    std::cout << "Original: 0b" << std::bitset<8>(test_value) << "\n";
    std::cout << "After clearing bit 7 and toggling bit 3: 0b" << std::bitset<8>(clear_toggle) << "\n";
    
    // 7. 循环移位（通过表达式实现）
    uint8_t rotate_value = 0b11000000;
    uint8_t left_rotate = (rotate_value << 2) | (rotate_value >> 6);
    uint8_t right_rotate = (rotate_value >> 2) | (rotate_value << 6);
    
    std::cout << "\nBit rotation operations:\n";
    std::cout << "Original: 0b" << std::bitset<8>(rotate_value) << "\n";
    std::cout << "Left rotate by 2: 0b" << std::bitset<8>(left_rotate) << "\n";
    std::cout << "Right rotate by 2: 0b" << std::bitset<8>(right_rotate) << "\n";
    
    // 8. 实用位操作函数
    #define BIT_SET(value, bit) ((value) |= (1UL << (bit)))
    #define BIT_CLEAR(value, bit) ((value) &= ~(1UL << (bit)))
    #define BIT_TOGGLE(value, bit) ((value) ^= (1UL << (bit)))
    #define BIT_CHECK(value, bit) (!!((value) & (1UL << (bit))))
    
    uint8_t utility_value = 0;
    BIT_SET(utility_value, 2);
    BIT_SET(utility_value, 5);
    
    std::cout << "\nUtility bit operations:\n";
    std::cout << "After setting bits 2 and 5: 0b" << std::bitset<8>(utility_value) << "\n";
    std::cout << "Bit 2 is " << (BIT_CHECK(utility_value, 2) ? "set" : "clear") << "\n";
    std::cout << "Bit 3 is " << (BIT_CHECK(utility_value, 3) ? "set" : "clear") << "\n";
    
    BIT_TOGGLE(utility_value, 2);
    std::cout << "After toggling bit 2: 0b" << std::bitset<8>(utility_value) << "\n";
    
    // 9. 位计数操作
    uint8_t count_value = 0b10101010;
    int bit_count = 0;
    for (int i = 0; i < 8; i++) {
        if (count_value & (1 << i)) {
            bit_count++;
        }
    }
    
    std::cout << "\nBit counting:\n";
    std::cout << "Value: 0b" << std::bitset<8>(count_value) << "\n";
    std::cout << "Number of set bits: " << bit_count << "\n";
}

int main() {
    bitwise_expressions();
    return 0;
}
```

### 10.3 字符串处理表达式

```cpp
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <regex>

void string_expressions() {
    std::cout << "=== String Processing Expressions ===\n";
    
    // 1. 字符串长度和比较
    std::string str1 = "Hello";
    std::string str2 = "World";
    std::string str3 = "Hello";
    
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    bool cmp1 = (str1 < str2);    // 字典序比较
    bool cmp2 = (str1 == str3);    // 相等比较
    int cmp3 = str1.compare(0, 3, str2, 0, 3); // 前n个字符比较
    
    std::cout << "String operations:\n";
    std::cout << "Length of \"" << str1 << "\": " << len1 << "\n";
    std::cout << "Length of \"" << str2 << "\": " << len2 << "\n";
    std::cout << "\"" << str1 << "\" < \"" << str2 << "\" = " << cmp1 << "\n";
    std::cout << "\"" << str1 << "\" == \"" << str3 << "\" = " << cmp2 << "\n";
    std::cout << "compare(\"" << str1 << "\", \"" << str2 << "\") = " << cmp3 << "\n";
    
    // 2. 字符串复制和连接
    std::string dest = str1;  // 复制字符串
    dest += " "; // 连接空格
    dest += str2;      // 连接第二个字符串
    
    std::cout << "\nString copying and concatenation:\n";
    std::cout << "Copied string: \"" << str1 << "\"\n";
    std::cout << "Concatenated string: \"" << dest << "\"\n";
    
    // 3. 字符串查找
    std::string text = "The quick brown fox jumps over the lazy dog";
    size_t found = text.find("fox");       // 查找子字符串
    size_t first_o = text.find('o');       // 查找字符首次出现
    size_t last_o = text.rfind('o');       // 查找字符最后出现
    
    std::cout << "\nString searching:\n";
    std::cout << "Text: \"" << text << "\"\n";
    std::cout << "Found \"fox\" at position: " << found << "\n";
    std::cout << "First 'o' at position: " << first_o << "\n";
    std::cout << "Last 'o' at position: " << last_o << "\n";
    
    // 4. 字符处理表达式
    std::string mixed_case = "Hello, World! 123";
    std::cout << "\nCharacter processing:\n";
    std::cout << "Original: \"" << mixed_case << "\"\n";
    
    // 转换为大写
    std::string upper_case = mixed_case;
    std::transform(upper_case.begin(), upper_case.end(), upper_case.begin(), ::toupper);
    std::cout << "Uppercase: \"" << upper_case << "\"\n";
    
    // 转换为小写
    std::string lower_case = mixed_case;
    std::transform(lower_case.begin(), lower_case.end(), lower_case.begin(), ::tolower);
    std::cout << "Lowercase: \"" << lower_case << "\"\n";
    
    // 5. 字符分类
    std::string test_chars = "Hello123!@#";
    std::cout << "\nCharacter classification:\n";
    for (char c : test_chars) {
        std::cout << "'" << c << "': ";
        if (std::isalpha(c)) std::cout << "alpha ";
        if (std::isdigit(c)) std::cout << "digit ";
        if (std::isalnum(c)) std::cout << "alnum ";
        if (std::isspace(c)) std::cout << "space ";
        if (std::ispunct(c)) std::cout << "punct ";
        if (std::isupper(c)) std::cout << "upper ";
        if (std::islower(c)) std::cout << "lower ";
        std::cout << "\n";
    }
    
    // 6. 字符串格式化
    std::string formatted;
    int number = 42;
    double pi = 3.14159;
    std::string name = "Alice";
    
    // C++20 格式化库
    #if __cplusplus >= 202002L
    // formatted = std::format("Name: {}, Age: {}, Pi: {:.2f}", name, number, pi);
    #else
    // 使用 stringstream 模拟
    #include <sstream>
    std::stringstream ss;
    ss << "Name: " << name << ", Age: " << number << ", Pi: " << std::fixed << std::setprecision(2) << pi;
    formatted = ss.str();
    #endif
    
    std::cout << "\nFormatted string: \"" << formatted << "\"\n";
    
    // 7. 字符串分割（使用 find 和 substr）
    std::string sentence = "apple,banana,cherry,date";
    std::cout << "\nString tokenization:\n";
    
    size_t start = 0;
    size_t end = sentence.find(',');
    while (end != std::string::npos) {
        std::string token = sentence.substr(start, end - start);
        std::cout << "Token: \"" << token << "\"\n";
        start = end + 1;
        end = sentence.find(',', start);
    }
    std::string last_token = sentence.substr(start);
    std::cout << "Token: \"" << last_token << "\"\n";
    
    // 8. 正则表达式处理
    std::string regex_text = "Contact: john@example.com or jane@test.org";
    std::regex email_regex(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b)");
    
    std::cout << "\nRegular expression matching:\n";
    std::cout << "Text: \"" << regex_text << "\"\n";
    
    std::sregex_iterator iter(regex_text.begin(), regex_text.end(), email_regex);
    std::sregex_iterator end_iter;
    
    for (; iter != end_iter; ++iter) {
        std::cout << "Found email: \"" << iter->str() << "\"\n";
    }
    
    // 9. 字符串视图操作 (C++17)
    #if __cplusplus >= 201703L
    std::string_view sv = "Hello, string_view!";
    std::cout << "\nString view operations:\n";
    std::cout << "String view: \"" << sv << "\"\n";
    std::cout << "Length: " << sv.length() << "\n";
    std::cout << "Substring: \"" << sv.substr(7, 11) << "\"\n";
    #endif
    
    // 10. 字符串构建表达式
    std::string builder;
    builder.reserve(100); // 预分配空间
    
    builder += "Hello";
    builder += ", ";
    builder += "World";
    builder += "!";
    
    std::cout << "\nString building: \"" << builder << "\"\n";
    
    // 11. 字符串替换
    std::string replace_text = "Hello, World! Hello, C++!";
    size_t pos = replace_text.find("Hello");
    while (pos != std::string::npos) {
        replace_text.replace(pos, 5, "Hi");
        pos = replace_text.find("Hello", pos + 2);
    }
    
    std::cout << "After replacement: \"" << replace_text << "\"\n";
}

int main() {
    string_expressions();
    return 0;
}
```

## 11. 最佳实践与代码规范

### 11.1 表达式编写规范

```cpp
#include <iostream>
#include <cmath>
#include <string>

// 好的做法：清晰的表达式编写
void good_expression_practices() {
    std::cout << "=== Good Expression Practices ===\n";
    
    // 1. 使用括号明确优先级
    int a = 10, b = 5, c = 2;
    int result1 = a + b * c;        // 依赖默认优先级
    int result2 = a + (b * c);      // 明确使用括号
    int result3 = (a + b) * c;      // 明确使用括号
    
    std::cout << "Without parentheses: " << a << " + " << b << " * " << c << " = " << result1 << "\n";
    std::cout << "With parentheses (a + (b * c)): " << result2 << "\n";
    std::cout << "With parentheses ((a + b) * c): " << result3 << "\n";
    
    // 2. 避免复杂的嵌套表达式
    int complex_value = 42;
    
    // 不好的做法：复杂嵌套
    // int bad_result = ((complex_value++ * 2) + (++complex_value)) / (complex_value-- - 1);
    
    // 好的做法：分解为简单步骤
    int temp1 = complex_value++ * 2;
    int temp2 = ++complex_value;
    int temp3 = complex_value-- - 1;
    int good_result = (temp1 + temp2) / temp3;
    
    std::cout << "Good practice result: " << good_result << "\n";
    
    // 3. 使用有意义的中间变量
    double radius = 5.0;
    constexpr double PI = 3.141592653589793238;
    
    // 好的做法：清晰的计算步骤
    double radius_squared = radius * radius;
    double area = PI * radius_squared;
    double circumference = 2 * PI * radius;
    
    std::cout << "Circle - Radius: " << radius << ", Area: " << area << ", Circumference: " << circumference << "\n";
    
    // 4. 一致的格式化
    bool condition1 = true;
    bool condition2 = false;
    int value1 = 10;
    int value2 = 20;
    
    // 好的做法：一致的缩进和换行
    int formatted_result = condition1 ? 
                         (value1 + value2) : 
                         (value1 - value2);
    
    std::cout << "Formatted conditional result: " << formatted_result << "\n";
    
    // 5. 避免副作用复杂的表达式
    int counter = 0;
    
    // 不好的做法：副作用复杂的表达式
    // int bad_side_effect = (counter++) + (counter++) + (counter++);
    
    // 好的做法：明确的副作用分离
    int good_side_effect = counter;
    counter++;
    good_side_effect += counter;
    counter++;
    good_side_effect += counter;
    counter++;
    
    std::cout << "Good side effect result: " << good_side_effect << ", Counter: " << counter << "\n";
    
    // 6. 使用 auto 进行类型推导
    auto auto_int = 42;
    auto auto_double = 3.14;
    auto auto_string = std::string("Hello");
    
    std::cout << "Auto types: int=" << auto_int << ", double=" << auto_double << ", string=" << auto_string << "\n";
    
    // 7. constexpr 用于编译时常量
    constexpr int ARRAY_SIZE = 10;
    constexpr double GRAVITY = 9.81;
    
    std::cout << "Constexpr values: ARRAY_SIZE=" << ARRAY_SIZE << ", GRAVITY=" << GRAVITY << "\n";
    
    // 8. 使用命名常量而非魔法数字
    constexpr int MAX_BUFFER_SIZE = 1024;
    constexpr int DEFAULT_PORT = 8080;
    
    std::cout << "Named constants: MAX_BUFFER_SIZE=" << MAX_BUFFER_SIZE << ", DEFAULT_PORT=" << DEFAULT_PORT << "\n";
}

int main() {
    good_expression_practices();
    return 0;
}
```

### 11.2 避免常见错误

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

// 避免的做法：常见的表达式错误
void avoid_common_mistakes() {
    std::cout << "=== Avoid Common Mistakes ===\n";
    
    // 1. 避免混淆赋值和比较
    int x = 5;
    
    // 错误的做法：使用赋值而不是比较
    // if (x = 10) {  // 这是赋值，不是比较！
    //     std::cout << "This will always execute\n";
    // }
    
    // 正确的做法：使用比较
    if (x == 10) {
        std::cout << "x equals 10\n";
    } else {
        std::cout << "x does not equal 10\n";
    }
    
    // 2. 避免浮点数直接比较
    double a = 0.1 + 0.2;
    double b = 0.3;
    
    // 错误的做法：直接比较浮点数
    // if (a == b) {  // 可能不相等由于精度问题
    //     std::cout << "Equal\n";
    // }
    
    // 正确的做法：使用误差范围比较
    double epsilon = 1e-9;
    if (std::abs(a - b) < epsilon) {
        std::cout << "a and b are approximately equal (" << a << " ≈ " << b << ")\n";
    }
    
    // 3. 避免无符号数和有符号数混合比较
    int signed_val = -1;
    unsigned int unsigned_val = 1U;
    
    // 错误的做法：混合比较导致意外结果
    // if (signed_val < unsigned_val) {  // -1 转换为很大的无符号数
    //     std::cout << "This might not be expected\n";
    // }
    
    // 正确的做法：类型匹配
    if (signed_val < static_cast<int>(unsigned_val)) {
        std::cout << "Correct comparison: " << signed_val << " < " << unsigned_val << "\n";
    }
    
    // 4. 避免数组越界访问
    int array[5] = {1, 2, 3, 4, 5};
    int index = 10;
    
    // 错误的做法：不检查边界
    // int dangerous = array[index];  // 未定义行为
    
    // 正确的做法：边界检查
    if (index >= 0 && index < 5) {
        int safe = array[index];
        std::cout << "Safe array access: array[" << index << "] = " << safe << "\n";
    } else {
        std::cout << "Index " << index << " is out of bounds for array of size 5\n";
    }
    
    // 5. 避免空指针解引用
    int* ptr = nullptr;
    
    // 错误的做法：不检查空指针
    // int dangerous_value = *ptr;  // 未定义行为
    
    // 正确的做法：空指针检查
    if (ptr != nullptr) {
        int safe_value = *ptr;
        std::cout << "Safe pointer dereference: " << safe_value << "\n";
    } else {
        std::cout << "Pointer is nullptr, cannot dereference\n";
    }
    
    // 6. 避免除零错误
    int dividend = 10;
    int divisor = 0;
    
    // 错误的做法：不检查除数
    // int dangerous_division = dividend / divisor;  // 未定义行为
    
    // 正确的做法：除数检查
    if (divisor != 0) {
        int safe_division = dividend / divisor;
        std::cout << "Safe division: " << dividend << " / " << divisor << " = " << safe_division << "\n";
    } else {
        std::cout << "Cannot divide by zero\n";
    }
    
    // 7. 避免未定义的求值顺序
    int undefined_var = 10;
    
    // 错误的做法：同一表达式中多次修改变量
    // int undefined_result = undefined_var++ + undefined_var++;  // 未定义行为
    
    // 正确的做法：分离修改操作
    int safe_result = undefined_var++;
    safe_result += undefined_var++;
    std::cout << "Safe modification result: " << safe_result << "\n";
    
    // 8. 避免资源泄漏
    // 错误的做法：忘记释放资源
    // int* raw_ptr = new int(42);
    // // 忘记 delete raw_ptr;
    
    // 正确的做法：使用智能指针
    auto smart_ptr = std::make_unique<int>(42);
    std::cout << "Smart pointer value: " << *smart_ptr << "\n";
    // 自动释放资源
    
    std::cout << "Common mistakes avoided successfully\n";
}

int main() {
    avoid_common_mistakes();
    return 0;
}
```

### 11.3 性能优化表达式

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

// 性能优化的表达式实践
void performance_optimization() {
    std::cout << "=== Performance Optimization ===\n";
    
    // 1. 避免昂贵的重复计算
    const int SIZE = 1000000;
    
    // 不好的做法：在循环中重复计算
    auto start = std::chrono::high_resolution_clock::now();
    long long bad_sum = 0;
    for (int i = 0; i < SIZE; i++) {
        bad_sum += i * (SIZE / 2);  // 重复计算 SIZE / 2
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto bad_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 好的做法：预计算常量表达式
    start = std::chrono::high_resolution_clock::now();
    long long good_sum = 0;
    const int half_size = SIZE / 2;  // 预计算
    for (int i = 0; i < SIZE; i++) {
        good_sum += i * half_size;
    }
    end = std::chrono::high_resolution_clock::now();
    auto good_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Performance comparison:\n";
    std::cout << "Bad approach time: " << bad_time.count() << " microseconds\n";
    std::cout << "Good approach time: " << good_time.count() << " microseconds\n";
    
    // 2. 使用位运算优化
    int value = 16;
    
    // 乘除法优化
    int multiply_by_8 = value << 3;    // 等价于 value * 8
    int divide_by_4 = value >> 2;      // 等价于 value / 4
    
    std::cout << "\nBit operation optimizations:\n";
    std::cout << value << " * 8 = " << (value * 8) << " (shift: " << multiply_by_8 << ")\n";
    std::cout << value << " / 4 = " << (value / 4) << " (shift: " << divide_by_4 << ")\n";
    
    // 3. 循环优化
    std::vector<int> array(1000);
    
    // 不好的做法：重复计算数组大小
    // for (size_t i = 0; i < array.size(); i++) {
    //     array[i] = i;
    // }
    
    // 好的做法：预计算数组大小
    const size_t array_size = array.size();
    for (size_t i = 0; i < array_size; i++) {
        array[i] = i * 2;
    }
    
    std::cout << "\nArray optimization: first 10 elements: ";
    for (int i = 0; i < 10; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
    
    // 4. 条件表达式优化
    bool condition = true;
    int x = 10, y = 20;
    
    // 简单条件的三元运算符可能比 if-else 更快
    int optimized_result = condition ? x : y;
    std::cout << "\nOptimized conditional result: " << optimized_result << "\n";
    
    // 5. 避免不必要的函数调用
    std::string str = "Performance test string";
    
    // 不好的做法：在循环中重复调用
    // for (size_t i = 0; i < str.length(); i++) { ... }
    
    // 好的做法：预计算长度
    size_t str_len = str.length();
    for (size_t i = 0; i < str_len; i++) {
        // 处理字符串
    }
    std::cout << "String length precomputed: " << str_len << "\n";
    
    // 6. 使用适当的整数类型
    // 对于小范围的值，使用较小的类型可以节省内存
    uint8_t small_counter = 0;    // 0-255 足够
    uint16_t medium_counter = 0;  // 0-65535 足够
    
    for (int i = 0; i < 100; i++) {
        small_counter++;
        medium_counter += 100;
    }
    
    std::cout << "Small counter: " << static_cast<unsigned>(small_counter) 
              << ", Medium counter: " << medium_counter << "\n";
    
    // 7. 移动语义优化
    std::vector<int> source_vec(1000, 42);
    
    // 不好的做法：拷贝
    // std::vector<int> dest_vec = source_vec;  // 拷贝
    
    // 好的做法：移动
    std::vector<int> dest_vec = std::move(source_vec);  // 移动
    std::cout << "Vector moved, destination size: " << dest_vec.size() << "\n";
    
    std::cout << "Performance optimizations demonstrated\n";
}

int main() {
    performance_optimization();
    return 0;
}
```

## 12. 标准参考

### 12.1 ISO C++ 标准条款引用

**C++98/C++03 标准 (ISO/IEC 14882:1998/2003)**：

- §5: 表达式
- §5.1: 主要表达式
- §5.2: 后缀表达式
- §5.3: 一元表达式
- §5.4: 强制转换表达式
- §5.5: 乘法表达式
- §5.6: 加法表达式
- §5.7: 移位表达式
- §5.8: 关系表达式
- §5.9: 相等表达式
- §5.10: 与表达式
- §5.11: 异或表达式
- §5.12: 或表达式
- §5.13: 逻辑与表达式
- §5.14: 逻辑或表达式
- §5.15: 条件表达式
- §5.16: 赋值表达式
- §5.17: 表达式语句

**C++11 标准 (ISO/IEC 14882:2011)**：

- §5: 表达式
- §5.1: 主要表达式
- §5.2: 后缀表达式
- §5.3: 一元表达式
- §5.4: 强制转换表达式
- §5.5: 乘法表达式
- §5.6: 加法表达式
- §5.7: 移位表达式
- §5.8: 关系表达式
- §5.9: 相等表达式
- §5.10: 与表达式
- §5.11: 异或表达式
- §5.12: 或表达式
- §5.13: 逻辑与表达式
- §5.14: 逻辑或表达式
- §5.15: 条件表达式
- §5.16: 赋值表达式
- §5.17: 表达式语句
- §5.18: 逗号表达式
- §5.19: 常量表达式

**C++14 标准 (ISO/IEC 14882:2014)**：

- §5: 表达式语义的澄清
- §5.2.2: 函数调用表达式的优化
- §5.3.4: new 表达式的改进
- §5.19: 常量表达式的扩展

**C++17 标准 (ISO/IEC 14882:2017)**：

- §8: 表达式语法
- §8.1: 主要表达式
- §8.2: 后缀表达式
- §8.3: 一元表达式
- §8.4: 强制转换表达式
- §8.5: 乘法表达式
- §8.6: 加法表达式
- §8.7: 移位表达式
- §8.8: 关系表达式
- §8.9: 相等表达式
- §8.10: 位与表达式
- §8.11: 位异或表达式
- §8.12: 位或表达式
- §8.13: 逻辑与表达式
- §8.14: 逻辑或表达式
- §8.15: 条件表达式
- §8.16: 赋值表达式
- §8.17: 表达式语句
- §8.18: 逗号表达式
- §8.19: 常量表达式

**C++20 标准 (ISO/IEC 14882:2020)**：

- §7: 表达式的语义
- §7.1: 主要表达式
- §7.2: 后缀表达式
- §7.3: 一元表达式
- §7.4: 强制转换表达式
- §7.5: 乘法表达式
- §7.6: 加法表达式
- §7.7: 移位表达式
- §7.8: 关系表达式
- §7.9: 相等表达式
- §7.10: 位与表达式
- §7.11: 位异或表达式
- §7.12: 位或表达式
- §7.13: 逻辑与表达式
- §7.14: 逻辑或表达式
- §7.15: 条件表达式
- §7.16: 赋值表达式
- §7.17: 表达式语句
- §7.18: 逗号表达式
- §7.19: 常量表达式

**C++23 标准 (ISO/IEC 14882:2023)**：

- §7: 表达式的更新
- §7.1: 主要表达式
- §7.2: 后缀表达式
- §7.3: 一元表达式
- §7.4: 强制转换表达式
- §7.5: 乘法表达式
- §7.6: 加法表达式
- §7.7: 移位表达式
- §7.8: 关系表达式
- §7.9: 相等表达式
- §7.10: 位与表达式
- §7.11: 位异或表达式
- §7.12: 位或表达式
- §7.13: 逻辑与表达式
- §7.14: 逻辑或表达式
- §7.15: 条件表达式
- §7.16: 赋值表达式
- §7.17: 表达式语句
- §7.18: 逗号表达式
- §7.19: 常量表达式

### 12.2 相关标准文档

- ISO/IEC 14882: Programming Languages - C++
- N3337: C++11 Standard Draft
- N4140: C++14 Standard Draft
- N4659: C++17 Standard Draft
- N4860: C++20 Standard Draft
- N4950: C++23 Standard Draft

## 13. 总结

### 13.1 C++ 表达式的特点

C++ 表达式是 C++ 语言计算的基本单元，具有以下核心特性：

**核心特性**：

- 多样性: 支持算术、关系、逻辑、位运算等多种运算符
- 灵活性: 可以组合形成复杂的计算表达式
- 类型安全: 提供隐式和显式类型转换机制
- 性能优化: 编译器对表达式进行优化
- 标准兼容: 遵循 ISO C++ 标准规范
- 现代特性: 支持 Lambda 表达式、智能指针等现代 C++ 特性

### 13.2 表达式类型比较

| 类型 | 语法示例 | 特点 | 常见用途 |
|------|----------|------|----------|
| 算术表达式 | `a + b * c` | 数值计算 | 数学运算 |
| 关系表达式 | `x > y` | 比较操作 | 条件判断 |
| 逻辑表达式 | `p && q` | 布尔运算 | 控制流程 |
| 赋值表达式 | `var = value` | 变量赋值 | 数据存储 |
| 条件表达式 | `cond ? a : b` | 三元选择 | 简洁条件 |
| Lambda 表达式 | `[](x) { return x*2; }` | 函数对象 | 函数式编程 |
| 智能指针表达式 | `ptr->method()` | 自动内存管理 | 资源管理 |

### 13.3 实际应用场景

**数学计算库**：

```cpp
#include <iostream>
#include <cmath>
#include <vector>

// 数学计算库示例
template<typename T>
struct Vector2D {
    T x, y;
    
    Vector2D(T x_val = 0, T y_val = 0) : x(x_val), y(y_val) {}
    
    // 向量运算表达式
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator*(T scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }
    
    T magnitude() const {
        return std::sqrt(x * x + y * y);
    }
    
    T dot_product(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
};

void mathematical_library_example() {
    std::cout << "=== Mathematical Library Example ===\n";
    
    Vector2D<double> v1(3.0, 4.0);
    Vector2D<double> v2(1.0, 2.0);
    
    // 向量加法
    Vector2D<double> sum = v1 + v2;
    std::cout << "Vector addition: (" << v1.x << ", " << v1.y << ") + (" 
              << v2.x << ", " << v2.y << ") = (" << sum.x << ", " << sum.y << ")\n";
    
    // 向量数乘
    Vector2D<double> scaled = v1 * 2.0;
    std::cout << "Vector scaling: (" << v1.x << ", " << v1.y << ") * 2.0 = (" 
              << scaled.x << ", " << scaled.y << ")\n";
    
    // 向量模长
    double magnitude = v1.magnitude();
    std::cout << "Vector magnitude: |(" << v1.x << ", " << v1.y << ")| = " << magnitude << "\n";
    
    // 点积
    double dot_product = v1.dot_product(v2);
    std::cout << "Dot product: (" << v1.x << ", " << v1.y << ") · (" 
              << v2.x << ", " << v2.y << ") = " << dot_product << "\n";
    
    // 复杂表达式组合
    Vector2D<double> complex_result = (v1 + v2) * v1.magnitude();
    std::cout << "Complex expression result: (" << complex_result.x << ", " << complex_result.y << ")\n";
}

int main() {
    mathematical_library_example();
    return 0;
}
```

**数据处理系统**：

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

// 数据处理系统示例
struct Employee {
    std::string name;
    int age;
    double salary;
    int department_id;
    
    Employee(const std::string& n, int a, double s, int dept) 
        : name(n), age(a), salary(s), department_id(dept) {}
};

// 数据过滤表达式
bool filter_high_salary(const Employee& emp) {
    return emp.salary > 50000.0;
}

bool filter_young_age(const Employee& emp) {
    return emp.age < 30;
}

bool filter_it_department(const Employee& emp) {
    return emp.department_id == 101;
}

// 数据转换表达式
Employee promote_employee(Employee emp) {
    emp.salary *= 1.1;  // 10% 加薪
    return emp;
}

double calculate_bonus(const Employee& emp) {
    return emp.salary * 0.05 * (emp.age > 40 ? 1.5 : 1.0);
}

void data_processing_system() {
    std::cout << "=== Data Processing System ===\n";
    
    std::vector<Employee> employees = {
        Employee("Alice", 25, 45000.0, 101),
        Employee("Bob", 35, 60000.0, 102),
        Employee("Charlie", 45, 75000.0, 101),
        Employee("Diana", 28, 55000.0, 103)
    };
    
    std::cout << "Processing " << employees.size() << " employees:\n";
    
    // 复合过滤表达式
    for (auto& emp : employees) {
        // 复杂条件表达式
        bool should_process = filter_high_salary(emp) && 
                             (filter_young_age(emp) || filter_it_department(emp));
        
        if (should_process) {
            std::cout << "Processing employee: " << emp.name << "\n";
            
            // 数据转换
            Employee promoted = promote_employee(emp);
            double bonus = calculate_bonus(promoted);
            
            std::cout << "  Original salary: " << emp.salary << "\n";
            std::cout << "  Promoted salary: " << promoted.salary << "\n";
            std::cout << "  Bonus: " << bonus << "\n";
        }
    }
    
    // 统计表达式
    int high_salary_count = std::count_if(employees.begin(), employees.end(), filter_high_salary);
    double total_salary = std::accumulate(employees.begin(), employees.end(), 0.0,
        [](double sum, const Employee& emp) { return sum + emp.salary; });
    
    double average_salary = total_salary / employees.size();
    double high_salary_percentage = static_cast<double>(high_salary_count) / employees.size() * 100;
    
    std::cout << "\nStatistics:\n";
    std::cout << "  Total employees: " << employees.size() << "\n";
    std::cout << "  Average salary: " << average_salary << "\n";
    std::cout << "  High salary employees: " << high_salary_count << " (" << high_salary_percentage << "%)\n";
}

int main() {
    data_processing_system();
    return 0;
}
```

### 13.4 最佳实践总结

- 清晰性: 使用括号明确表达式优先级
- 安全性: 避免未定义行为和常见错误
- 性能: 优化重复计算和复杂表达式
- 可读性: 使用有意义的变量名和注释
- 标准兼容: 遵循 ISO C++ 标准规范
- 现代特性: 充分利用 C++11 及以后版本的新特性

### 13.5 学习建议

- 理解基础: 深入掌握运算符优先级和结合性
- 实践应用: 在实际项目中练习表达式编写
- 关注标准: 跟踪 C++ 标准的新特性和改进
- 代码审查: 建立表达式编写的代码审查机制
- 性能意识: 理解表达式对程序性能的影响
- 现代C++: 学习和应用现代C++特性如Lambda、智能指针等

C++ 表达式作为 C++ 语言的核心组成部分，虽然概念相对简单，但在实际编程中具有重要作用。掌握其特性和最佳实践，能够帮助开发者编写更高效、更可靠的 C++ 程序。
