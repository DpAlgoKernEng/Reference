# C 表达式详解

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

C 表达式是运算符及其操作数的序列，用于指定计算。表达式可以产生结果、副作用，或者指定对象或函数。

```c
#include <stdio.h>

void basic_expression_examples() {
    printf("=== Basic Expression Examples ===\n");
    
    // 1. 算术表达式
    int a = 10, b = 5;
    int sum = a + b;           // 加法表达式
    int diff = a - b;          // 减法表达式
    int product = a * b;       // 乘法表达式
    int quotient = a / b;      // 除法表达式
    int remainder = a % b;     // 取模表达式
    
    printf("Sum: %d\n", sum);
    printf("Difference: %d\n", diff);
    printf("Product: %d\n", product);
    printf("Quotient: %d\n", quotient);
    printf("Remainder: %d\n", remainder);
    
    // 2. 关系表达式
    int greater = a > b;       // 大于表达式
    int equal = a == b;        // 等于表达式
    int less_equal = a <= b;   // 小于等于表达式
    
    printf("Greater: %d\n", greater);
    printf("Equal: %d\n", equal);
    printf("Less or equal: %d\n", less_equal);
    
    // 3. 逻辑表达式
    int logical_and = (a > 0) && (b > 0);  // 逻辑与表达式
    int logical_or = (a > 0) || (b < 0);   // 逻辑或表达式
    int logical_not = !(a == b);           // 逻辑非表达式
    
    printf("Logical AND: %d\n", logical_and);
    printf("Logical OR: %d\n", logical_or);
    printf("Logical NOT: %d\n", logical_not);
    
    // 4. 赋值表达式
    int x = 42;                // 简单赋值
    int y = x += 10;           // 复合赋值
    int z = (x = y = 5);       // 链式赋值
    
    printf("x: %d, y: %d, z: %d\n", x, y, z);
    
    // 5. 条件表达式
    int max = (a > b) ? a : b; // 条件表达式
    printf("Max of %d and %d is %d\n", a, b, max);
    
    // 6. 副作用表达式
    int counter = 0;
    int result = (++counter) + (counter++); // 包含副作用的表达式
    printf("Counter: %d, Result: %d\n", counter, result);
    
    // 7. 函数调用表达式
    int length = printf("Hello, World!\n"); // 函数调用表达式
    printf("Characters printed: %d\n", length);
}

int main() {
    basic_expression_examples();
    return 0;
}
```

### 1.2 表达式的本质

表达式是 C 语言计算的基本单元，它将值、变量、运算符和函数组合起来形成可执行的计算单元：

```c
#include <stdio.h>
#include <string.h>

// 模拟表达式求值过程
void expression_nature() {
    printf("=== Expression Nature ===\n");
    
    // 1. 表达式产生结果
    int arithmetic_result = 2 + 3 * 4; // 结果为 14
    printf("Arithmetic result: %d\n", arithmetic_result);
    
    // 2. 表达式产生副作用
    int side_effect_counter = 0;
    int side_effect_result = printf("Side effect: %d\n", ++side_effect_counter);
    printf("Characters printed: %d\n", side_effect_result);
    
    // 3. 表达式指定对象
    int array[5] = {1, 2, 3, 4, 5};
    int* ptr = &array[2]; // 表达式指定数组元素的地址
    printf("Array element: %d\n", *ptr);
    
    // 4. 复杂表达式组合
    char str1[] = "Hello";
    char str2[] = "World";
    int complex_result = (strlen(str1) + strlen(str2)) * 2;
    printf("Complex expression result: %d\n", complex_result);
    
    // 5. 嵌套表达式
    int nested_result = ((1 + 2) * (3 + 4)) - (5 / 2);
    printf("Nested expression result: %d\n", nested_result);
}

int main() {
    expression_nature();
    return 0;
}
```

### 1.3 表达式与语句的关系

表达式是构成语句的基础，但表达式本身不是完整的语句：

```c
#include <stdio.h>

void expression_statement_relationship() {
    printf("=== Expression Statement Relationship ===\n");
    
    // 1. 表达式语句（表达式后加分号）
    int x = 42;           // 声明语句包含表达式
    x = x + 1;            // 赋值表达式语句
    printf("x = %d\n", x); // 函数调用表达式语句
    
    // 2. 控制结构中的表达式
    if (x > 0) {          // 条件表达式
        printf("Positive\n");
    }
    
    for (int i = 0; i < 3; i++) { // 初始化、条件、迭代表达式
        printf("Loop iteration %d\n", i);
    }
    
    // 3. 复杂表达式语句
    int a = 1, b = 2, c = 3;
    int result = (a++, b++, c++, a + b + c); // 逗号表达式
    printf("Comma expression result: %d\n", result);
    
    // 4. 表达式序列
    int sequence_result = (a = 10, b = 20, c = 30, a + b + c);
    printf("Sequence expression result: %d\n", sequence_result);
}

int main() {
    expression_statement_relationship();
    return 0;
}
```

## 2. 表达式分类与值类别

### 2.1 值类别详解

C 表达式根据其值类别分为不同的类型：

```c
#include <stdio.h>

void value_categories() {
    printf("=== Value Categories ===\n");
    
    // 1. 左值表达式 (lvalue)
    int variable = 42;           // variable 是左值
    int* ptr = &variable;        // &variable 产生左值的地址
    variable = 100;              // 左值可以出现在赋值的左侧
    
    printf("Variable address: %p\n", (void*)ptr);
    printf("Variable value: %d\n", variable);
    
    // 2. 非左值表达式 (rvalue)
    int result = 2 + 3;          // 2 + 3 是非左值表达式
    int temp = (variable + 5);   // (variable + 5) 是非左值表达式
    
    printf("Result: %d\n", result);
    printf("Temp: %d\n", temp);
    
    // 3. 函数指示符
    void (*func_ptr)(void) = value_categories; // 函数名是函数指示符
    printf("Function address: %p\n", (void*)func_ptr);
    
    // 4. 数组表达式
    int array[5] = {1, 2, 3, 4, 5};
    int* array_ptr = array;      // 数组名在大多数上下文中转换为指针
    
    printf("Array elements: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // 5. 结构体成员表达式
    struct Point {
        int x, y;
    } point = {10, 20};
    
    point.x = 30;                // point.x 是左值
    int member_value = point.y;  // point.y 是左值，但用作右值
    
    printf("Point: (%d, %d)\n", point.x, point.y);
}

int main() {
    value_categories();
    return 0;
}
```

### 2.2 表达式类型分类

```c
#include <stdio.h>

void expression_type_classification() {
    printf("=== Expression Type Classification ===\n");
    
    // 1. 整型表达式
    int int_expr = 42 + 8;
    unsigned int uint_expr = 100U - 20U;
    
    printf("Int expression: %d\n", int_expr);
    printf("Unsigned int expression: %u\n", uint_expr);
    
    // 2. 浮点型表达式
    float float_expr = 3.14f * 2.0f;
    double double_expr = 2.718281828 * 3.0;
    
    printf("Float expression: %f\n", float_expr);
    printf("Double expression: %lf\n", double_expr);
    
    // 3. 指针表达式
    int array[5] = {1, 2, 3, 4, 5};
    int* ptr_expr = array + 2;   // 指针算术表达式
    int deref_expr = *(array + 1); // 解引用表达式
    
    printf("Pointer expression result: %d\n", *ptr_expr);
    printf("Dereference expression result: %d\n", deref_expr);
    
    // 4. 字符表达式
    char char_expr = 'A' + 1;    // 字符算术表达式
    int ascii_value = 'Z';       // 字符到整数的转换
    
    printf("Character expression: %c\n", char_expr);
    printf("ASCII value: %d\n", ascii_value);
    
    // 5. 逻辑表达式
    int logical_expr1 = (5 > 3) && (2 < 4);  // 逻辑与表达式
    int logical_expr2 = (1 == 2) || (3 != 4); // 逻辑或表达式
    int logical_expr3 = !(5 == 5);            // 逻辑非表达式
    
    printf("Logical AND: %d\n", logical_expr1);
    printf("Logical OR: %d\n", logical_expr2);
    printf("Logical NOT: %d\n", logical_expr3);
}

int main() {
    expression_type_classification();
    return 0;
}
```

## 3. 运算符详解

### 3.1 算术运算符

```c
#include <stdio.h>
#include <math.h>

void arithmetic_operators() {
    printf("=== Arithmetic Operators ===\n");
    
    // 1. 基本算术运算符
    int a = 15, b = 4;
    int add_result = a + b;      // 加法
    int sub_result = a - b;      // 减法
    int mul_result = a * b;      // 乘法
    int div_result = a / b;      // 除法（整数除法）
    int mod_result = a % b;      // 取模
    
    printf("Addition: %d + %d = %d\n", a, b, add_result);
    printf("Subtraction: %d - %d = %d\n", a, b, sub_result);
    printf("Multiplication: %d * %d = %d\n", a, b, mul_result);
    printf("Division: %d / %d = %d\n", a, b, div_result);
    printf("Modulus: %d %% %d = %d\n", a, b, mod_result);
    
    // 2. 浮点数算术
    double x = 15.5, y = 4.2;
    double f_add = x + y;
    double f_sub = x - y;
    double f_mul = x * y;
    double f_div = x / y;
    
    printf("Float addition: %.2f + %.2f = %.2f\n", x, y, f_add);
    printf("Float subtraction: %.2f - %.2f = %.2f\n", x, y, f_sub);
    printf("Float multiplication: %.2f * %.2f = %.2f\n", x, y, f_mul);
    printf("Float division: %.2f / %.2f = %.2f\n", x, y, f_div);
    
    // 3. 一元算术运算符
    int positive = +a;           // 正号
    int negative = -a;           // 负号
    int increment_pre = ++a;     // 前置递增
    int decrement_post = a--;    // 后置递减
    
    printf("Positive: +%d = %d\n", a-1, positive);
    printf("Negative: -%d = %d\n", a+1, negative);
    printf("Pre-increment: ++%d = %d\n", a-1, increment_pre);
    printf("Post-decrement: %d-- = %d\n", a, decrement_post);
    
    // 4. 位运算符
    int bitwise_a = 12;  // 1100 in binary
    int bitwise_b = 10;  // 1010 in binary
    
    int bitwise_and = bitwise_a & bitwise_b;    // 按位与
    int bitwise_or = bitwise_a | bitwise_b;     // 按位或
    int bitwise_xor = bitwise_a ^ bitwise_b;    // 按位异或
    int bitwise_not = ~bitwise_a;               // 按位取反
    int left_shift = bitwise_a << 1;            // 左移
    int right_shift = bitwise_a >> 1;           // 右移
    
    printf("Bitwise AND: %d & %d = %d (binary: 1100 & 1010 = 1000)\n", 
           bitwise_a, bitwise_b, bitwise_and);
    printf("Bitwise OR: %d | %d = %d (binary: 1100 | 1010 = 1110)\n", 
           bitwise_a, bitwise_b, bitwise_or);
    printf("Bitwise XOR: %d ^ %d = %d (binary: 1100 ^ 1010 = 0110)\n", 
           bitwise_a, bitwise_b, bitwise_xor);
    printf("Bitwise NOT: ~%d = %d\n", bitwise_a, bitwise_not);
    printf("Left shift: %d << 1 = %d\n", bitwise_a, left_shift);
    printf("Right shift: %d >> 1 = %d\n", bitwise_a, right_shift);
}

int main() {
    arithmetic_operators();
    return 0;
}
```

### 3.2 关系和逻辑运算符

```c
#include <stdio.h>
#include <stdbool.h>

void relational_logical_operators() {
    printf("=== Relational and Logical Operators ===\n");
    
    // 1. 关系运算符
    int a = 10, b = 20;
    
    int equal = (a == b);        // 等于
    int not_equal = (a != b);    // 不等于
    int less = (a < b);          // 小于
    int greater = (a > b);       // 大于
    int less_equal = (a <= b);   // 小于等于
    int greater_equal = (a >= b); // 大于等于
    
    printf("Equal: %d == %d = %s\n", a, b, equal ? "true" : "false");
    printf("Not equal: %d != %d = %s\n", a, b, not_equal ? "true" : "false");
    printf("Less: %d < %d = %s\n", a, b, less ? "true" : "false");
    printf("Greater: %d > %d = %s\n", a, b, greater ? "true" : "false");
    printf("Less or equal: %d <= %d = %s\n", a, b, less_equal ? "true" : "false");
    printf("Greater or equal: %d >= %d = %s\n", a, b, greater_equal ? "true" : "false");
    
    // 2. 逻辑运算符
    bool x = true, y = false;
    
    bool logical_and = x && y;   // 逻辑与（短路求值）
    bool logical_or = x || y;    // 逻辑或（短路求值）
    bool logical_not = !x;       // 逻辑非
    
    printf("Logical AND: %s && %s = %s\n", 
           x ? "true" : "false", y ? "true" : "false", 
           logical_and ? "true" : "false");
    printf("Logical OR: %s || %s = %s\n", 
           x ? "true" : "false", y ? "true" : "false", 
           logical_or ? "true" : "false");
    printf("Logical NOT: !%s = %s\n", 
           x ? "true" : "false", logical_not ? "true" : "false");
    
    // 3. 短路求值演示
    int counter = 0;
    
    // 逻辑与短路：如果第一个操作数为假，不会计算第二个操作数
    bool short_circuit_and = (false && (++counter > 0));
    printf("Short circuit AND - Counter: %d, Result: %s\n", 
           counter, short_circuit_and ? "true" : "false");
    
    // 逻辑或短路：如果第一个操作数为真，不会计算第二个操作数
    bool short_circuit_or = (true || (++counter > 0));
    printf("Short circuit OR - Counter: %d, Result: %s\n", 
           counter, short_circuit_or ? "true" : "false");
    
    // 4. 复杂关系表达式
    int score = 85;
    bool is_passing = (score >= 60) && (score <= 100);
    bool is_excellent = (score >= 90) && (score <= 100);
    bool is_valid_score = (score >= 0) && (score <= 100);
    
    printf("Score: %d\n", score);
    printf("Is passing: %s\n", is_passing ? "true" : "false");
    printf("Is excellent: %s\n", is_excellent ? "true" : "false");
    printf("Is valid score: %s\n", is_valid_score ? "true" : "false");
}

int main() {
    relational_logical_operators();
    return 0;
}
```

### 3.3 赋值运算符

```c
#include <stdio.h>

void assignment_operators() {
    printf("=== Assignment Operators ===\n");
    
    // 1. 基本赋值运算符
    int a = 10;                  // 简单赋值
    printf("Initial value of a: %d\n", a);
    
    // 2. 复合赋值运算符
    a += 5;                      // 等价于 a = a + 5
    printf("After a += 5: %d\n", a);
    
    a -= 3;                      // 等价于 a = a - 3
    printf("After a -= 3: %d\n", a);
    
    a *= 2;                      // 等价于 a = a * 2
    printf("After a *= 2: %d\n", a);
    
    a /= 4;                      // 等价于 a = a / 4
    printf("After a /= 4: %d\n", a);
    
    a %= 3;                      // 等价于 a = a % 3
    printf("After a %%= 3: %d\n", a);
    
    // 3. 位运算复合赋值
    int b = 12;                  // 1100 in binary
    printf("Initial value of b: %d (binary: 1100)\n", b);
    
    b &= 10;                     // 等价于 b = b & 10 (1010)
    printf("After b &= 10: %d (binary: 1000)\n", b);
    
    b |= 5;                      // 等价于 b = b | 5 (0101)
    printf("After b |= 5: %d (binary: 1101)\n", b);
    
    b ^= 3;                      // 等价于 b = b ^ 3 (0011)
    printf("After b ^= 3: %d (binary: 1110)\n", b);
    
    b <<= 1;                     // 等价于 b = b << 1
    printf("After b <<= 1: %d (binary: 11100)\n", b);
    
    b >>= 2;                     // 等价于 b = b >> 2
    printf("After b >>= 2: %d (binary: 111)\n", b);
    
    // 4. 链式赋值
    int x, y, z;
    x = y = z = 42;              // 从右到左结合
    printf("Chain assignment: x = %d, y = %d, z = %d\n", x, y, z);
    
    // 5. 赋值表达式的值
    int result = (x = 100);      // 赋值表达式返回被赋的值
    printf("Assignment expression result: %d\n", result);
    
    // 6. 指针赋值
    int value = 42;
    int* ptr1 = &value;
    int* ptr2;
    ptr2 = ptr1;                 // 指针赋值
    printf("Pointer assignment: *ptr1 = %d, *ptr2 = %d\n", *ptr1, *ptr2);
    
    // 7. 数组赋值（通过循环）
    int src_array[5] = {1, 2, 3, 4, 5};
    int dest_array[5];
    for (int i = 0; i < 5; i++) {
        dest_array[i] = src_array[i];  // 元素赋值
    }
    
    printf("Array copy: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", dest_array[i]);
    }
    printf("\n");
}

int main() {
    assignment_operators();
    return 0;
}
```

### 3.4 递增和递减运算符

```c
#include <stdio.h>

void increment_decrement_operators() {
    printf("=== Increment and Decrement Operators ===\n");
    
    // 1. 前置递增/递减
    int a = 5;
    int pre_inc_result = ++a;    // 先递增，再使用值
    printf("Pre-increment: a = 5, ++a = %d, a = %d\n", pre_inc_result, a);
    
    int b = 5;
    int pre_dec_result = --b;    // 先递减，再使用值
    printf("Pre-decrement: b = 5, --b = %d, b = %d\n", pre_dec_result, b);
    
    // 2. 后置递增/递减
    int c = 5;
    int post_inc_result = c++;   // 先使用值，再递增
    printf("Post-increment: c = 5, c++ = %d, c = %d\n", post_inc_result, c);
    
    int d = 5;
    int post_dec_result = d--;   // 先使用值，再递减
    printf("Post-decrement: d = 5, d-- = %d, d = %d\n", post_dec_result, d);
    
    // 3. 在表达式中的使用
    int x = 10, y = 20;
    int expr1 = (++x) + (y++);   // x先递增，y后递增
    printf("Expression (++x) + (y++): x = %d, y = %d, result = %d\n", x, y, expr1);
    
    int p = 1, q = 2;
    int expr2 = (p++) + (++q);   // p后递增，q先递增
    printf("Expression (p++) + (++q): p = %d, q = %d, result = %d\n", p, q, expr2);
    
    // 4. 指针递增/递减
    int array[5] = {10, 20, 30, 40, 50};
    int* ptr = array;
    
    printf("Pointer operations:\n");
    printf("Initial: *ptr = %d\n", *ptr);
    printf("After ptr++: *(ptr++) = %d, *ptr = %d\n", *(ptr++), *ptr);
    printf("After ++ptr: *(++ptr) = %d, *ptr = %d\n", *(++ptr), *ptr);
    
    // 5. 复杂表达式中的递增/递减
    int i = 0;
    int complex_expr = (++i) + (i++) + (++i);
    printf("Complex expression (++i) + (i++) + (++i): i = %d, result = %d\n", i, complex_expr);
    
    // 6. 递增/递减与数组访问
    int numbers[5] = {1, 2, 3, 4, 5};
    int* num_ptr = numbers;
    int sum = 0;
    
    for (int j = 0; j < 5; j++) {
        sum += *(num_ptr++);     // 递增指针并累加值
    }
    printf("Sum using pointer increment: %d\n", sum);
    
    // 7. 递增/递减的安全使用
    int safe_counter = 0;
    while (safe_counter < 3) {
        printf("Safe counter iteration: %d\n", safe_counter++);
    }
}

int main() {
    increment_decrement_operators();
    return 0;
}
```

### 3.5 条件运算符

```c
#include <stdio.h>

void conditional_operator() {
    printf("=== Conditional Operator ===\n");
    
    // 1. 基本条件运算符
    int a = 10, b = 20;
    int max = (a > b) ? a : b;   // 如果 a > b 则返回 a，否则返回 b
    int min = (a < b) ? a : b;   // 如果 a < b 则返回 a，否则返回 b
    
    printf("Max of %d and %d is %d\n", a, b, max);
    printf("Min of %d and %d is %d\n", a, b, min);
    
    // 2. 条件运算符的嵌套
    int x = 15, y = 25, z = 5;
    int largest = (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
    printf("Largest of %d, %d, %d is %d\n", x, y, z, largest);
    
    // 3. 条件运算符与不同类型
    int score = 85;
    char grade = (score >= 90) ? 'A' : 
                 (score >= 80) ? 'B' : 
                 (score >= 70) ? 'C' : 
                 (score >= 60) ? 'D' : 'F';
    printf("Score %d gets grade %c\n", score, grade);
    
    // 4. 条件运算符与指针
    int value1 = 100, value2 = 200;
    int* ptr1 = &value1;
    int* ptr2 = &value2;
    int* selected_ptr = (value1 > value2) ? ptr1 : ptr2;
    printf("Selected pointer points to: %d\n", *selected_ptr);
    
    // 5. 条件运算符的副作用
    int counter = 0;
    int result = (counter++ > 0) ? counter : (counter += 10);
    printf("Counter: %d, Result: %d\n", counter, result);
    
    // 6. 条件运算符与函数调用
    int positive_func(int x) { return x * 2; }
    int negative_func(int x) { return x * -1; }
    
    int number = -5;
    int processed = (number > 0) ? positive_func(number) : negative_func(number);
    printf("Number %d processed to %d\n", number, processed);
    
    // 7. 复杂条件表达式
    int age = 25;
    int income = 50000;
    char* status = (age >= 18) ? 
                   ((income >= 30000) ? "Eligible for loan" : "Income too low") : 
                   "Underage";
    printf("Age %d, Income %d: %s\n", age, income, status);
    
    // 8. 条件运算符与数组
    int even_numbers[5] = {2, 4, 6, 8, 10};
    int odd_numbers[5] = {1, 3, 5, 7, 9};
    int index = 2;
    int selected_value = (index % 2 == 0) ? even_numbers[index] : odd_numbers[index];
    printf("Index %d selected value: %d\n", index, selected_value);
}

int main() {
    conditional_operator();
    return 0;
}
```

### 3.6 逗号运算符

```c
#include <stdio.h>

void comma_operator() {
    printf("=== Comma Operator ===\n");
    
    // 1. 基本逗号运算符
    int a = 1, b = 2, c = 3;
    int result = (a++, b++, c++, a + b + c);  // 从左到右求值，返回最后一个表达式的值
    printf("a = %d, b = %d, c = %d, result = %d\n", a, b, c, result);
    
    // 2. 逗号运算符在 for 循环中的使用
    printf("For loop with comma operator:\n");
    for (int i = 0, j = 10; i < 5; i++, j--) {
        printf("i = %d, j = %d\n", i, j);
    }
    
    // 3. 逗号运算符与副作用
    int x = 5, y = 10;
    int comma_result = (printf("Side effect 1\n"), 
                        printf("Side effect 2\n"), 
                        x + y);
    printf("Comma result: %d\n", comma_result);
    
    // 4. 复杂逗号表达式
    int p = 1, q = 2, r = 3;
    int complex_comma = (p += 5, q *= 2, r -= 1, p + q + r);
    printf("p = %d, q = %d, r = %d, complex comma result = %d\n", p, q, r, complex_comma);
    
    // 5. 逗号运算符与条件表达式
    int condition = 1;
    int ternary_with_comma = condition ? (printf("True branch\n"), 42) : (printf("False branch\n"), 24);
    printf("Ternary with comma result: %d\n", ternary_with_comma);
    
    // 6. 逗号运算符在宏定义中的使用
    #define SWAP(a, b) do { int temp = a; a = b; b = temp; } while(0)
    #define COMPLEX_OPERATION(x, y) ((x)++, (y)++, (x) + (y))
    
    int m = 10, n = 20;
    int macro_result = COMPLEX_OPERATION(m, n);
    printf("After macro: m = %d, n = %d, result = %d\n", m, n, macro_result);
    
    // 7. 逗号运算符的求值顺序保证
    int array[5] = {0};
    int index = 0;
    int sequence_result = (array[index++] = 1, 
                          array[index++] = 2, 
                          array[index++] = 3, 
                          index);
    printf("Array elements: ");
    for (int i = 0; i < 3; i++) {
        printf("%d ", array[i]);
    }
    printf("\nSequence result: %d\n", sequence_result);
}

int main() {
    comma_operator();
    return 0;
}
```

## 4. 运算符优先级与结合性

### 4.1 运算符优先级表

```c
#include <stdio.h>

void operator_precedence() {
    printf("=== Operator Precedence ===\n");
    
    // 1. 算术运算符优先级
    int a = 10, b = 5, c = 2;
    int result1 = a + b * c;     // * 优先级高于 +
    int result2 = (a + b) * c;   // 使用括号改变优先级
    printf("a + b * c = %d + %d * %d = %d\n", a, b, c, result1);
    printf("(a + b) * c = (%d + %d) * %d = %d\n", a, b, c, result2);
    
    // 2. 关系运算符与逻辑运算符优先级
    int x = 15, y = 10, z = 20;
    int result3 = x > y && y < z; // 关系运算符优先级高于逻辑运算符
    int result4 = (x > y) && (y < z); // 明确使用括号
    printf("x > y && y < z = %d > %d && %d < %d = %s\n", 
           x, y, y, z, result3 ? "true" : "false");
    
    // 3. 赋值运算符优先级
    int p = 5, q = 10, r = 15;
    int result5 = p = q = r;     // 赋值运算符从右到左结合
    printf("p = q = r = %d, so p = %d, q = %d, r = %d\n", result5, p, q, r);
    
    // 4. 复杂表达式优先级
    int complex_result = 2 + 3 * 4 - 5 / 2;
    // 等价于: 2 + (3 * 4) - (5 / 2) = 2 + 12 - 2 = 12
    printf("2 + 3 * 4 - 5 / 2 = %d\n", complex_result);
    
    // 5. 位运算符优先级
    int bitwise_a = 12, bitwise_b = 10, bitwise_c = 5;
    int bitwise_result = bitwise_a & bitwise_b | bitwise_c;
    // 等价于: (bitwise_a & bitwise_b) | bitwise_c
    printf("bitwise_a & bitwise_b | bitwise_c = (%d & %d) | %d = %d\n", 
           bitwise_a, bitwise_b, bitwise_c, bitwise_result);
    
    // 6. 条件运算符优先级
    int condition_a = 10, condition_b = 5;
    int condition_result = condition_a > condition_b ? condition_a : condition_b + 10;
    // 等价于: (condition_a > condition_b) ? condition_a : (condition_b + 10)
    printf("condition_a > condition_b ? condition_a : condition_b + 10 = %d\n", condition_result);
    
    // 7. 逗号运算符优先级（最低）
    int comma_a = 1, comma_b = 2, comma_c = 3;
    int comma_result = comma_a++, comma_b++, comma_c++, comma_a + comma_b + comma_c;
    printf("Comma operator result: %d\n", comma_result);
    printf("After comma operations: a = %d, b = %d, c = %d\n", comma_a, comma_b, comma_c);
    
    // 8. 括号的使用（最高优先级）
    int paren_result = ((2 + 3) * 4) - (5 / (2 + 1));
    printf("((2 + 3) * 4) - (5 / (2 + 1)) = %d\n", paren_result);
}

int main() {
    operator_precedence();
    return 0;
}
```

### 4.2 结合性详解

```c
#include <stdio.h>

void operator_associativity() {
    printf("=== Operator Associativity ===\n");
    
    // 1. 左结合性（大多数运算符）
    int a = 10, b = 5, c = 2;
    int left_assoc = a - b - c;  // 等价于 (a - b) - c
    printf("a - b - c = %d - %d - %d = %d (equivalent to (%d - %d) - %d = %d)\n", 
           a, b, c, left_assoc, a, b, c, (a - b) - c);
    
    // 2. 右结合性（赋值运算符）
    int x = 5, y = 10, z = 15;
    int right_assoc = x = y = z; // 等价于 x = (y = z)
    printf("x = y = z = %d = %d = %d = %d (equivalent to x = (y = %d) = %d)\n", 
           x, y, z, right_assoc, z, right_assoc);
    
    // 3. 条件运算符的右结合性
    int condition1 = 1, condition2 = 0, condition3 = 1;
    int ternary_result = condition1 ? 10 : condition2 ? 20 : 30;
    // 等价于: condition1 ? 10 : (condition2 ? 20 : 30)
    printf("condition1 ? 10 : condition2 ? 20 : 30 = %d\n", ternary_result);
    
    // 4. 幂运算符的右结合性（通过函数模拟）
    // C 中没有内置的幂运算符，但可以演示右结合性概念
    int power_result = 2; // 2^(2^3) = 2^8 = 256
    // 实际计算: 2^(2^3) = 2^8 = 256
    printf("Right associativity concept: 2^(2^3) = 2^8 = %d\n", 256);
    
    // 5. 复合赋值运算符的右结合性
    int value = 100;
    value += 5 *= 2;         // 等价于 value += (5 *= 2)
    printf("value += 5 *= 2 = %d (equivalent to value += (5 *= 2) = %d)\n", 
           value, 100 + (5 * 2));
    
    // 6. 一元运算符的右结合性
    int unary_value = 5;
    int unary_result = ++unary_value; // 前置递增是右结合的
    printf("++unary_value = %d\n", unary_result);
    
    // 7. 复杂表达式中的结合性
    int complex_a = 2, complex_b = 3, complex_c = 4;
    int complex_result = complex_a = complex_b = complex_c;
    printf("Complex right associativity: a = b = c = %d, so a = %d, b = %d, c = %d\n", 
           complex_result, complex_a, complex_b, complex_c);
    
    // 8. 混合优先级和结合性的表达式
    int mixed_result = 2 + 3 * 4 - 5 / 2 + 1;
    // 求值顺序: 2 + (3 * 4) - (5 / 2) + 1 = 2 + 12 - 2 + 1 = 13
    printf("Mixed precedence and associativity: 2 + 3 * 4 - 5 / 2 + 1 = %d\n", mixed_result);
}

int main() {
    operator_associativity();
    return 0;
}
```

## 5. 表达式求值顺序

### 5.1 求值顺序规则

```c
#include <stdio.h>

// 全局计数器用于演示求值顺序
static int evaluation_counter = 0;

int get_value(const char* name) {
    printf("Getting value for %s: %d\n", name, ++evaluation_counter);
    return evaluation_counter;
}

void evaluation_order() {
    printf("=== Evaluation Order ===\n");
    
    // 1. 顺序点和未定义行为
    evaluation_counter = 0;
    printf("\n1. Undefined behavior example (avoid this):\n");
    int undefined_var = 10;
    // int undefined_result = undefined_var++ + undefined_var++; // 未定义行为！
    
    // 2. 顺序点保证的求值顺序
    evaluation_counter = 0;
    printf("\n2. Sequence points (&&, ||, ?:, ,):\n");
    
    // 逻辑与的短路求值
    int and_result = (get_value("A") > 0) && (get_value("B") > 0);
    printf("AND result: %d\n", and_result);
    
    evaluation_counter = 0;
    // 逻辑或的短路求值
    int or_result = (get_value("C") < 0) || (get_value("D") > 0);
    printf("OR result: %d\n", or_result);
    
    // 3. 函数参数的求值顺序（未指定）
    printf("\n3. Function parameter evaluation order (unspecified):\n");
    evaluation_counter = 0;
    
    void print_three_values(int a, int b, int c) {
        printf("Function received: %d, %d, %d\n", a, b, c);
    }
    
    // 参数求值顺序是未指定的，可能是从左到右或从右到左
    print_three_values(get_value("Param1"), get_value("Param2"), get_value("Param3"));
    
    // 4. 逗号运算符的求值顺序（从左到右）
    evaluation_counter = 0;
    printf("\n4. Comma operator evaluation order (left to right):\n");
    int comma_result = (get_value("Comma1"), get_value("Comma2"), get_value("Comma3"));
    printf("Comma result: %d\n", comma_result);
    
    // 5. 条件运算符的求值顺序
    evaluation_counter = 0;
    printf("\n5. Conditional operator evaluation order:\n");
    int condition = 1;
    int ternary_result = condition ? get_value("TrueBranch") : get_value("FalseBranch");
    printf("Ternary result: %d\n", ternary_result);
    
    evaluation_counter = 0;
    condition = 0;
    int ternary_result2 = condition ? get_value("TrueBranch2") : get_value("FalseBranch2");
    printf("Ternary result 2: %d\n", ternary_result2);
    
    // 6. 初始化列表的求值顺序（C99 起从左到右）
    evaluation_counter = 0;
    printf("\n6. Initialization list evaluation order:\n");
    int init_array[] = {get_value("Init1"), get_value("Init2"), get_value("Init3")};
    printf("Array elements: %d, %d, %d\n", init_array[0], init_array[1], init_array[2]);
    
    // 7. 复合字面量的求值顺序
    evaluation_counter = 0;
    printf("\n7. Compound literal evaluation order:\n");
    struct Point { int x, y; };
    struct Point p = {get_value("PointX"), get_value("PointY")};
    printf("Point: (%d, %d)\n", p.x, p.y);
}

int main() {
    evaluation_order();
    return 0;
}
```

### 5.2 副作用与求值顺序

```c
#include <stdio.h>

void side_effects_evaluation() {
    printf("=== Side Effects and Evaluation Order ===\n");
    
    // 1. 修改和访问同一变量的副作用
    int a = 5;
    printf("Initial a = %d\n", a);
    
    // 安全的副作用使用
    int safe_result = (a += 3) + (a *= 2);  // 明确的顺序点
    printf("After safe operations: a = %d, result = %d\n", a, safe_result);
    
    // 2. 数组访问中的副作用
    int array[5] = {10, 20, 30, 40, 50};
    int index = 0;
    printf("Array elements using side effects:\n");
    
    // 安全的数组遍历
    for (int i = 0; i < 5; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }
    
    // 3. 指针操作中的副作用
    int* ptr = array;
    printf("Pointer arithmetic with side effects:\n");
    for (int i = 0; i < 5; i++) {
        printf("*(ptr++) = %d\n", *(ptr++));
        ptr--; // 补偿递增
    }
    
    // 4. 函数调用中的副作用
    int global_counter = 0;
    
    int increment_counter() {
        return ++global_counter;
    }
    
    printf("\nFunction call side effects:\n");
    global_counter = 0;
    int func_result = increment_counter() + increment_counter() * increment_counter();
    printf("Global counter: %d, Function result: %d\n", global_counter, func_result);
    
    // 5. 条件表达式中的副作用
    int condition_var = 10;
    printf("\nConditional expression side effects:\n");
    int conditional_result = (condition_var > 5) ? 
                            (printf("True branch executed\n"), condition_var *= 2) : 
                            (printf("False branch executed\n"), condition_var /= 2);
    printf("Condition var: %d, Result: %d\n", condition_var, conditional_result);
    
    // 6. 复合赋值中的副作用
    int compound_var = 100;
    printf("\nCompound assignment side effects:\n");
    int compound_result = (compound_var += 10) * (compound_var -= 5);
    printf("Compound var: %d, Result: %d\n", compound_var, compound_result);
    
    // 7. 避免未定义行为的技巧
    printf("\nSafe practices to avoid undefined behavior:\n");
    
    // 使用临时变量避免同一表达式中多次修改
    int safe_var = 10;
    int temp = safe_var + 1;
    safe_var = temp;
    printf("Safe modification: %d\n", safe_var);
    
    // 使用顺序点分隔修改操作
    int sequence_var = 5;
    sequence_var++;  // 第一个修改
    sequence_var *= 2;  // 第二个修改（在顺序点之后）
    printf("Sequence point safe: %d\n", sequence_var);
}

int main() {
    side_effects_evaluation();
    return 0;
}
```

## 6. 类型转换与强制转换

### 6.1 隐式类型转换

```c
#include <stdio.h>

void implicit_conversions() {
    printf("=== Implicit Conversions ===\n");
    
    // 1. 整型提升
    char char_val = 65;
    short short_val = 1000;
    int int_result = char_val + short_val;  // char 和 short 都提升为 int
    printf("char(%d) + short(%d) = int(%d)\n", char_val, short_val, int_result);
    
    // 2. 通常算术转换
    int int_val = 10;
    float float_val = 3.14f;
    double double_result = int_val + float_val;  // int 转换为 float，然后 float 转换为 double
    printf("int(%d) + float(%.2f) = double(%.2f)\n", int_val, float_val, double_result);
    
    // 3. 有符号和无符号整数转换
    int signed_val = -1;
    unsigned int unsigned_val = 1U;
    unsigned int mixed_result = signed_val + unsigned_val;  // signed 转换为 unsigned
    printf("signed(%d) + unsigned(%u) = unsigned(%u)\n", signed_val, unsigned_val, mixed_result);
    
    // 4. 浮点数到整数的转换
    double double_val = 3.7;
    int truncated = double_val;  // 截断小数部分
    printf("double(%.1f) converted to int = %d\n", double_val, truncated);
    
    // 5. 整数到布尔的转换
    int non_zero = 42;
    int zero = 0;
    _Bool bool_from_non_zero = non_zero;  // 非零值转换为 true
    _Bool bool_from_zero = zero;          // 零值转换为 false
    printf("int(%d) to bool = %s\n", non_zero, bool_from_non_zero ? "true" : "false");
    printf("int(%d) to bool = %s\n", zero, bool_from_zero ? "true" : "false");
    
    // 6. 指针和整数的转换
    int array[5] = {1, 2, 3, 4, 5};
    int* ptr = array;
    ptr = ptr + 2;  // 指针算术
    printf("Pointer arithmetic: array[2] = %d\n", *ptr);
    
    // 7. void* 转换
    void* void_ptr = array;
    int* int_ptr = void_ptr;  // void* 可以隐式转换为任何对象指针
    printf("void* to int* conversion: *int_ptr = %d\n", *int_ptr);
    
    // 8. 字符和整数的转换
    char letter = 'A';
    int ascii_value = letter;  // char 转换为 int
    char back_to_char = ascii_value;  // int 转换为 char
    printf("char('%c') to int = %d, back to char = '%c'\n", letter, ascii_value, back_to_char);
    
    // 9. 复杂表达式中的类型转换
    short s = 30000;
    long l = 50000L;
    long long_result = s + l;  // short 提升为 int，然后 int 转换为 long
    printf("short(%d) + long(%ld) = long(%ld)\n", s, l, long_result);
}

int main() {
    implicit_conversions();
    return 0;
}
```

### 6.2 显式类型转换（强制转换）

```c
#include <stdio.h>

void explicit_casts() {
    printf("=== Explicit Casts ===\n");
    
    // 1. 基本强制转换语法
    double double_val = 3.14159;
    int int_val = (int)double_val;  // C 风格强制转换
    printf("double(%.5f) cast to int = %d\n", double_val, int_val);
    
    // 2. 不同类型的强制转换
    int large_int = 1000000;
    short short_val = (short)large_int;  // 可能溢出
    printf("int(%d) cast to short = %d\n", large_int, short_val);
    
    // 3. 有符号和无符号转换
    int negative = -1;
    unsigned int unsigned_negative = (unsigned int)negative;
    printf("int(%d) cast to unsigned int = %u\n", negative, unsigned_negative);
    
    // 4. 浮点数之间的转换
    long double ld_val = 123.456789012345L;
    float float_val = (float)ld_val;
    printf("long double(%.15Lf) cast to float = %.15f\n", ld_val, float_val);
    
    // 5. 指针强制转换
    int array[5] = {10, 20, 30, 40, 50};
    void* void_ptr = (void*)array;
    int* int_ptr = (int*)void_ptr;
    printf("Pointer cast: array[0] = %d, *(int*)void_ptr = %d\n", array[0], *int_ptr);
    
    // 6. 整数和指针之间的转换
    int address_as_int = (int)(intptr_t)array;  // 安全的指针到整数转换
    int* restored_ptr = (int*)(intptr_t)address_as_int;
    printf("Address as int: %d, restored pointer points to: %d\n", address_as_int, *restored_ptr);
    
    // 7. 函数指针强制转换
    int add(int a, int b) { return a + b; }
    int (*func_ptr)(int, int) = (int (*)(int, int))add;
    int func_result = func_ptr(5, 3);
    printf("Function pointer cast result: %d\n", func_result);
    
    // 8. 复杂表达式中的强制转换
    double complex_expr = (double)(5 / 2) + 3.14;  // 整数除法后转换为 double
    double correct_expr = (double)5 / 2 + 3.14;    // 正确的转换顺序
    printf("Complex cast (wrong): %.2f\n", complex_expr);
    printf("Complex cast (correct): %.2f\n", correct_expr);
    
    // 9. 避免精度损失的强制转换
    float precise_float = 3.14159265359f;
    double more_precise = (double)precise_float;
    printf("float(%.12f) cast to double = %.12f\n", precise_float, more_precise);
    
    // 10. 安全的强制转换实践
    unsigned char byte_val = 255;
    int safe_conversion = (int)byte_val;
    printf("unsigned char(%u) safely cast to int = %d\n", byte_val, safe_conversion);
}

int main() {
    explicit_casts();
    return 0;
}
```

## 7. 常量表达式

### 7.1 常量表达式定义与使用

```c
#include <stdio.h>

void constant_expressions() {
    printf("=== Constant Expressions ===\n");
    
    // 1. 整型常量表达式
    const int ARRAY_SIZE = 10;  // 常量表达式
    int array[ARRAY_SIZE];      // 用于数组大小
    
    printf("Array size: %d\n", ARRAY_SIZE);
    
    // 2. 枚举常量表达式
    enum Color { RED = 1, GREEN = 2, BLUE = 4 };
    int color_flags = RED | GREEN;  // 常量表达式
    printf("Color flags: %d\n", color_flags);
    
    // 3. sizeof 表达式（常量表达式）
    const size_t int_size = sizeof(int);  // 常量表达式
    printf("Size of int: %zu bytes\n", int_size);
    
    // 4. 地址常量表达式
    static int static_var = 42;
    int* const ptr_to_static = &static_var;  // 地址常量
    printf("Static variable address: %p\n", (void*)ptr_to_static);
    
    // 5. 字符常量表达式
    const char NEWLINE = '\n';
    const char TAB = '\t';
    printf("Character constants: newline=%d, tab=%d%c", NEWLINE, TAB, NEWLINE);
    
    // 6. 浮点常量表达式
    const double PI = 3.141592653589793238;
    const float E = 2.718281828f;
    printf("Mathematical constants: PI=%.15f, E=%.10f\n", PI, E);
    
    // 7. 复杂常量表达式
    const int COMPLEX_CONST = (ARRAY_SIZE * 2) + (RED | GREEN);
    printf("Complex constant: %d\n", COMPLEX_CONST);
    
    // 8. 条件常量表达式
    const int MAX_VALUE = (ARRAY_SIZE > 5) ? ARRAY_SIZE : 10;
    printf("Max value: %d\n", MAX_VALUE);
    
    // 9. 位运算常量表达式
    const int BIT_MASK = (1 << 3) | (1 << 5);  // 设置第3位和第5位
    printf("Bit mask: %d (binary: ", BIT_MASK);
    for (int i = 7; i >= 0; i--) {
        printf("%d", (BIT_MASK >> i) & 1);
    }
    printf(")\n");
    
    // 10. 字符串字面量作为常量表达式
    const char* const MESSAGE = "Hello, World!";
    printf("String constant: %s\n", MESSAGE);
}

int main() {
    constant_expressions();
    return 0;
}
```

### 7.2 常量表达式在编译时计算

```c
#include <stdio.h>

// 1. 编译时常量函数（C11 起）
#if __STDC_VERSION__ >= 201112L
#define static_assert _Static_assert

void compile_time_constants() {
    printf("=== Compile-time Constants ===\n");
    
    // 1. 静态断言（编译时检查）
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
    static_assert(sizeof(void*) == sizeof(intptr_t), "Pointer size mismatch");
    
    // 2. 常量表达式优化
    const int MULTIPLIER = 7;
    const int CALCULATED = MULTIPLIER * 13 + 5;  // 编译时计算
    
    // 3. 数组大小常量表达式
    int compile_time_array[CALCULATED];  // 编译时确定大小
    printf("Compile-time array size: %zu\n", sizeof(compile_time_array) / sizeof(compile_time_array[0]));
    
    // 4. 枚举常量表达式
    enum {
        BUFFER_SIZE = 1024,
        ALIGNMENT = 16,
        PADDING = BUFFER_SIZE % ALIGNMENT ? ALIGNMENT - (BUFFER_SIZE % ALIGNMENT) : 0
    };
    
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Alignment: %d\n", ALIGNMENT);
    printf("Padding: %d\n", PADDING);
    
    // 5. 位字段大小常量表达式
    struct BitField {
        unsigned int flag1 : 1;
        unsigned int flag2 : 2;
        unsigned int value : 5;
        unsigned int reserved : (sizeof(unsigned int) * 8 - 8);
    };
    
    printf("Bit field size: %zu bytes\n", sizeof(struct BitField));
    
    // 6. 复杂的编译时常量表达式
    const int PRIME1 = 17;
    const int PRIME2 = 19;
    const int PRODUCT = PRIME1 * PRIME2;
    const int SUM = PRIME1 + PRIME2;
    const int DIFFERENCE = (PRIME1 > PRIME2) ? (PRIME1 - PRIME2) : (PRIME2 - PRIME1);
    
    printf("Prime1: %d, Prime2: %d\n", PRIME1, PRIME2);
    printf("Product: %d\n", PRODUCT);
    printf("Sum: %d\n", SUM);
    printf("Difference: %d\n", DIFFERENCE);
    
    // 7. 条件编译中的常量表达式
    #if PRODUCT > 300
        printf("Product is large\n");
    #else
        printf("Product is small\n");
    #endif
}
#else
void compile_time_constants() {
    printf("=== Compile-time Constants (C11 features not available) ===\n");
    printf("This compiler does not support C11 static_assert\n");
}
#endif

int main() {
    compile_time_constants();
    return 0;
}
```

## 8. 主要表达式

### 8.1 常量和字面量

```c
#include <stdio.h>

void constants_and_literals() {
    printf("=== Constants and Literals ===\n");
    
    // 1. 整数常量
    int decimal = 42;           // 十进制
    int octal = 052;            // 八进制（以0开头）
    int hexadecimal = 0x2A;     // 十六进制（以0x开头）
    long long_int = 1000000L;   // 长整型后缀
    unsigned int unsigned_int = 4294967295U;  // 无符号后缀
    
    printf("Decimal: %d\n", decimal);
    printf("Octal 052 = Decimal %d\n", octal);
    printf("Hexadecimal 0x2A = Decimal %d\n", hexadecimal);
    printf("Long: %ld\n", long_int);
    printf("Unsigned: %u\n", unsigned_int);
    
    // 2. 浮点常量
    float float_literal = 3.14f;        // float 后缀
    double double_literal = 3.14159;    // 默认 double
    long double long_double_literal = 3.141592653589793238L;  // long double 后缀
    
    printf("Float: %.2f\n", float_literal);
    printf("Double: %.15f\n", double_literal);
    printf("Long double: %.18Lf\n", long_double_literal);
    
    // 3. 科学计数法
    double scientific1 = 1.23e10;       // 1.23 × 10^10
    double scientific2 = 4.56E-5;       // 4.56 × 10^-5
    float scientific3 = 7.89e-3f;       // float 类型
    
    printf("Scientific notation: %.2e, %.2e, %.2e\n", scientific1, scientific2, scientific3);
    
    // 4. 字符常量
    char single_char = 'A';
    char escape_char = '\n';            // 换行符
    char octal_char = '\101';           // 八进制表示的 'A'
    char hex_char = '\x41';             // 十六进制表示的 'A'
    
    printf("Character constants: '%c', escape char (newline), octal '\\101' = '%c', hex '\\x41' = '%c'\n", 
           single_char, octal_char, hex_char);
    
    // 5. 字符串字面量
    const char* string_literal = "Hello, World!";
    const char* multiline = "This is a \
long string that spans \
multiple lines";
    
    printf("String literal: %s\n", string_literal);
    printf("Multiline string: %s\n", multiline);
    
    // 6. 相邻字符串字面量的连接
    const char* concatenated = "Hello, " "World" "!";  // 编译时连接
    printf("Concatenated string: %s\n", concatenated);
    
    // 7. 空字符串和空字符
    const char* empty_string = "";
    char null_char = '\0';
    
    printf("Empty string length: %zu\n", strlen(empty_string));
    printf("Null character: %d\n", null_char);
    
    // 8. 特殊字符常量
    char tab = '\t';
    char backslash = '\\';
    char quote = '\'';
    char double_quote = '\"';
    
    printf("Special characters: tab='%c', backslash='%c', quote='%c', double quote='%c'\n", 
           tab, backslash, quote, double_quote);
}

int main() {
    constants_and_literals();
    return 0;
}
```

### 8.2 标识符和函数名

```c
#include <stdio.h>
#include <string.h>

// 全局变量
static int global_counter = 0;

// 函数声明
int calculate_sum(int a, int b);
void display_result(int result);
static void internal_function(void);

void identifiers_and_function_names() {
    printf("=== Identifiers and Function Names ===\n");
    
    // 1. 变量标识符
    int local_variable = 42;
    const int CONSTANT_VALUE = 100;
    static int static_variable = 200;
    
    printf("Local variable: %d\n", local_variable);
    printf("Constant value: %d\n", CONSTANT_VALUE);
    printf("Static variable: %d\n", static_variable);
    
    // 2. 函数标识符
    int sum = calculate_sum(10, 20);
    display_result(sum);
    
    // 3. 函数指针标识符
    int (*function_ptr)(int, int) = calculate_sum;
    int ptr_result = function_ptr(5, 15);
    printf("Function pointer result: %d\n", ptr_result);
    
    // 4. 数组标识符
    int array[5] = {1, 2, 3, 4, 5};
    int* array_ptr = array;  // 数组名作为指针
    
    printf("Array elements: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // 5. 结构体标识符
    struct Point {
        int x;
        int y;
    } point1 = {10, 20};
    
    struct Point point2;
    point2.x = 30;
    point2.y = 40;
    
    printf("Point1: (%d, %d)\n", point1.x, point1.y);
    printf("Point2: (%d, %d)\n", point2.x, point2.y);
    
    // 6. 联合体标识符
    union Data {
        int i;
        float f;
        char str[20];
    } data;
    
    data.i = 100;
    printf("Union integer: %d\n", data.i);
    
    data.f = 3.14f;
    printf("Union float: %.2f\n", data.f);
    
    // 7. 枚举标识符
    enum Color { RED, GREEN, BLUE };
    enum Color favorite_color = GREEN;
    
    printf("Favorite color: %d\n", favorite_color);
    
    // 8. 内部函数调用
    internal_function();
}

// 函数定义
int calculate_sum(int a, int b) {
    return a + b;
}

void display_result(int result) {
    printf("Calculation result: %d\n", result);
}

static void internal_function(void) {
    global_counter++;
    printf("Internal function called, global counter: %d\n", global_counter);
}

int main() {
    identifiers_and_function_names();
    return 0;
}
```

## 9. 未计算的表达式

### 9.1 sizeof 运算符

```c
#include <stdio.h>
#include <stddef.h>

void unevaluated_expressions() {
    printf("=== Unevaluated Expressions ===\n");
    
    // 1. sizeof 基本用法
    size_t int_size = sizeof(int);
    size_t char_size = sizeof(char);
    size_t double_size = sizeof(double);
    size_t pointer_size = sizeof(int*);
    
    printf("Size of int: %zu bytes\n", int_size);
    printf("Size of char: %zu bytes\n", char_size);
    printf("Size of double: %zu bytes\n", double_size);
    printf("Size of int*: %zu bytes\n", pointer_size);
    
    // 2. sizeof 数组
    int array[10];
    size_t array_size = sizeof(array);
    size_t element_count = sizeof(array) / sizeof(array[0]);
    
    printf("Array total size: %zu bytes\n", array_size);
    printf("Array element count: %zu\n", element_count);
    
    // 3. sizeof 结构体
    struct Person {
        char name[50];
        int age;
        double height;
    };
    
    size_t struct_size = sizeof(struct Person);
    printf("Size of struct Person: %zu bytes\n", struct_size);
    
    // 4. sizeof 未计算表达式
    int x = 42;
    size_t expr_size = sizeof(x + 1);  // x + 1 不会被计算
    printf("Size of expression (x + 1): %zu bytes\n", expr_size);
    
    // 5. sizeof 函数调用（不执行）
    size_t func_size = sizeof(printf("This won't be printed\n"));  // printf 不会被调用
    printf("Size of printf return value: %zu bytes\n", func_size);
    
    // 6. sizeof 字符串字面量
    size_t string_size = sizeof("Hello, World!");
    printf("Size of string literal: %zu bytes\n", string_size);
    
    // 7. sizeof 指针和数组的区别
    char str[] = "Hello";
    char* ptr = str;
    
    size_t array_char_size = sizeof(str);    // 包含 '\0' 的完整大小
    size_t pointer_char_size = sizeof(ptr);  // 指针大小
    
    printf("Size of char array: %zu bytes\n", array_char_size);
    printf("Size of char pointer: %zu bytes\n", pointer_char_size);
    
    // 8. sizeof 位字段
    structBitFields {
        unsigned int a : 3;
        unsigned int b : 4;
        unsigned int c : 5;
    };
    
    size_t bitfield_size = sizeof(structBitFields);
    printf("Size of bitfield struct: %zu bytes\n", bitfield_size);
    
    // 9. sizeof 与对齐
    struct AlignedStruct {
        char a;
        int b;
        char c;
    };
    
    size_t aligned_size = sizeof(struct AlignedStruct);
    printf("Size of aligned struct: %zu bytes\n", aligned_size);
    
    // 10. sizeof 在宏中的使用
    #define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
    
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t macro_length = ARRAY_LENGTH(numbers);
    printf("Array length using macro: %zu\n", macro_length);
}

int main() {
    unevaluated_expressions();
    return 0;
}
```

### 9.2 _Alignof 运算符（C11）

```c
#include <stdio.h>
#include <stdalign.h>

#if __STDC_VERSION__ >= 201112L
void alignof_operator() {
    printf("=== _Alignof Operator (C11) ===\n");
    
    // 1. 基本 _Alignof 用法
    size_t int_align = _Alignof(int);
    size_t char_align = _Alignof(char);
    size_t double_align = _Alignof(double);
    size_t pointer_align = _Alignof(int*);
    
    printf("Alignment of int: %zu\n", int_align);
    printf("Alignment of char: %zu\n", char_align);
    printf("Alignment of double: %zu\n", double_align);
    printf("Alignment of int*: %zu\n", pointer_align);
    
    // 2. 结构体对齐要求
    struct SimpleStruct {
        char a;
        int b;
        double c;
    };
    
    size_t struct_align = _Alignof(struct SimpleStruct);
    printf("Alignment of struct: %zu\n", struct_align);
    
    // 3. 使用 alignas 指定对齐
    alignas(16) int aligned_int;
    size_t specified_align = _Alignof(aligned_int);
    printf("Alignment of alignas(16) int: %zu\n", specified_align);
    
    // 4. 最大对齐要求
    size_t max_align = _Alignof(max_align_t);
    printf("Maximum alignment: %zu\n", max_align);
    
    // 5. 数组对齐
    int array[10];
    size_t array_align = _Alignof(array);
    printf("Alignment of int array: %zu\n", array_align);
    
    // 6. 对齐与大小的关系
    struct PaddedStruct {
        char a;
        int b;
    };
    
    size_t padded_size = sizeof(struct PaddedStruct);
    size_t padded_align = _Alignof(struct PaddedStruct);
    printf("Padded struct size: %zu, alignment: %zu\n", padded_size, padded_align);
    
    // 7. 函数参数的对齐（未计算）
    int dummy = 42;
    size_t param_align = _Alignof(dummy + 1);  // 表达式不被计算
    printf("Alignment of expression (dummy + 1): %zu\n", param_align);
}
#else
void alignof_operator() {
    printf("=== _Alignof Operator (C11 features not available) ===\n");
    printf("This compiler does not support C11 _Alignof operator\n");
}
#endif

int main() {
    alignof_operator();
    return 0;
}
```

## 10. 常见应用场景

### 10.1 数学计算表达式

```c
#include <stdio.h>
#include <math.h>

void mathematical_expressions() {
    printf("=== Mathematical Expressions ===\n");
    
    // 1. 基本数学运算
    double a = 10.5, b = 3.2;
    double sum = a + b;
    double difference = a - b;
    double product = a * b;
    double quotient = a / b;
    
    printf("Basic operations:\n");
    printf("%.2f + %.2f = %.2f\n", a, b, sum);
    printf("%.2f - %.2f = %.2f\n", a, b, difference);
    printf("%.2f * %.2f = %.2f\n", a, b, product);
    printf("%.2f / %.2f = %.2f\n", a, b, quotient);
    
    // 2. 复杂数学表达式
    double x = 2.0, y = 3.0;
    double complex_expr = (x * x + y * y) / (x - y) + sqrt(x * y);
    printf("Complex expression: (%.2f² + %.2f²) / (%.2f - %.2f) + √(%.2f × %.2f) = %.2f\n", 
           x, y, x, y, x, y, complex_expr);
    
    // 3. 三角函数表达式
    double angle = M_PI / 4;  // 45 degrees in radians
    double sin_val = sin(angle);
    double cos_val = cos(angle);
    double tan_val = tan(angle);
    
    printf("Trigonometric functions:\n");
    printf("sin(π/4) = %.4f\n", sin_val);
    printf("cos(π/4) = %.4f\n", cos_val);
    printf("tan(π/4) = %.4f\n", tan_val);
    
    // 4. 指数和对数表达式
    double base = 2.0, exponent = 3.0;
    double power = pow(base, exponent);
    double natural_log = log(power);
    double log10_val = log10(power);
    
    printf("Exponential and logarithmic functions:\n");
    printf("%.2f^%.2f = %.2f\n", base, exponent, power);
    printf("ln(%.2f) = %.4f\n", power, natural_log);
    printf("log10(%.2f) = %.4f\n", power, log10_val);
    
    // 5. 取整和取模表达式
    double value = 3.7;
    double floor_val = floor(value);
    double ceil_val = ceil(value);
    double round_val = round(value);
    
    printf("Rounding functions:\n");
    printf("floor(%.2f) = %.2f\n", value, floor_val);
    printf("ceil(%.2f) = %.2f\n", value, ceil_val);
    printf("round(%.2f) = %.2f\n", value, round_val);
    
    // 6. 绝对值表达式
    double negative = -15.5;
    double absolute = fabs(negative);
    printf("Absolute value: |%.2f| = %.2f\n", negative, absolute);
    
    // 7. 最大值和最小值表达式
    double val1 = 10.5, val2 = 7.8;
    double max_val = fmax(val1, val2);
    double min_val = fmin(val1, val2);
    printf("Max and min: max(%.2f, %.2f) = %.2f, min(%.2f, %.2f) = %.2f\n", 
           val1, val2, max_val, val1, val2, min_val);
    
    // 8. 复合数学表达式
    double radius = 5.0;
    double area = M_PI * radius * radius;
    double circumference = 2 * M_PI * radius;
    printf("Circle calculations: radius=%.2f, area=%.2f, circumference=%.2f\n", 
           radius, area, circumference);
}

int main() {
    mathematical_expressions();
    return 0;
}
```

### 10.2 位操作表达式

```c
#include <stdio.h>
#include <stdint.h>

void bitwise_expressions() {
    printf("=== Bitwise Expressions ===\n");
    
    // 1. 基本位运算
    uint8_t a = 0b10101010;  // 170 in decimal
    uint8_t b = 0b11001100;  // 204 in decimal
    
    uint8_t and_result = a & b;    // 按位与
    uint8_t or_result = a | b;     // 按位或
    uint8_t xor_result = a ^ b;    // 按位异或
    uint8_t not_result = ~a;       // 按位取反
    
    printf("Basic bitwise operations:\n");
    printf("a = 0b%s = %u\n", "10101010", a);
    printf("b = 0b%s = %u\n", "11001100", b);
    printf("a & b = 0b%08b = %u\n", and_result, and_result);
    printf("a | b = 0b%08b = %u\n", or_result, or_result);
    printf("a ^ b = 0b%08b = %u\n", xor_result, xor_result);
    printf("~a = 0b%08b = %u\n", not_result, not_result);
    
    // 2. 位移操作
    uint8_t value = 0b00001111;  // 15 in decimal
    uint8_t left_shift = value << 2;   // 左移2位
    uint8_t right_shift = value >> 1;  // 右移1位
    
    printf("\nBit shift operations:\n");
    printf("value = 0b%08b = %u\n", value, value);
    printf("value << 2 = 0b%08b = %u\n", left_shift, left_shift);
    printf("value >> 1 = 0b%08b = %u\n", right_shift, right_shift);
    
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
    printf("\nBit field operations:\n");
    printf("Byte: 0b%08b\n", bit_union.byte);
    printf("Flag1: %d, Flag2: %d, Flag3: %d, Flag4: %d\n", 
           bit_union.flags.flag1, bit_union.flags.flag2, 
           bit_union.flags.flag3, bit_union.flags.flag4);
    printf("Data: 0b%04b = %d\n", bit_union.flags.data, bit_union.flags.data);
    
    // 4. 位掩码操作
    uint8_t mask = 0b11110000;
    uint8_t data = 0b10101010;
    uint8_t masked = data & mask;      // 应用掩码
    uint8_t cleared = data & ~mask;    // 清除掩码位
    uint8_t set = data | mask;         // 设置掩码位
    
    printf("\nBit mask operations:\n");
    printf("Mask: 0b%08b\n", mask);
    printf("Data: 0b%08b\n", data);
    printf("Masked: 0b%08b = %u\n", masked, masked);
    printf("Cleared: 0b%08b = %u\n", cleared, cleared);
    printf("Set: 0b%08b = %u\n", set, set);
    
    // 5. 位测试和设置
    uint8_t test_value = 0b00000000;
    
    // 设置特定位
    test_value |= (1 << 3);  // 设置第3位
    test_value |= (1 << 7);  // 设置第7位
    
    printf("\nBit setting operations:\n");
    printf("After setting bits 3 and 7: 0b%08b\n", test_value);
    
    // 测试特定位
    int bit3_set = (test_value & (1 << 3)) != 0;
    int bit5_set = (test_value & (1 << 5)) != 0;
    
    printf("Bit 3 is %s\n", bit3_set ? "set" : "clear");
    printf("Bit 5 is %s\n", bit5_set ? "set" : "clear");
    
    // 6. 位清除和切换
    uint8_t clear_toggle = test_value;
    
    // 清除特定位
    clear_toggle &= ~(1 << 7);  // 清除第7位
    
    // 切换特定位
    clear_toggle ^= (1 << 3);   // 切换第3位
    
    printf("\nBit clearing and toggling:\n");
    printf("Original: 0b%08b\n", test_value);
    printf("After clearing bit 7 and toggling bit 3: 0b%08b\n", clear_toggle);
    
    // 7. 循环移位（通过表达式实现）
    uint8_t rotate_value = 0b11000000;
    uint8_t left_rotate = (rotate_value << 2) | (rotate_value >> 6);
    uint8_t right_rotate = (rotate_value >> 2) | (rotate_value << 6);
    
    printf("\nBit rotation operations:\n");
    printf("Original: 0b%08b\n", rotate_value);
    printf("Left rotate by 2: 0b%08b\n", left_rotate);
    printf("Right rotate by 2: 0b%08b\n", right_rotate);
    
    // 8. 实用位操作函数
    #define BIT_SET(value, bit) ((value) |= (1UL << (bit)))
    #define BIT_CLEAR(value, bit) ((value) &= ~(1UL << (bit)))
    #define BIT_TOGGLE(value, bit) ((value) ^= (1UL << (bit)))
    #define BIT_CHECK(value, bit) (!!((value) & (1UL << (bit))))
    
    uint8_t utility_value = 0;
    BIT_SET(utility_value, 2);
    BIT_SET(utility_value, 5);
    
    printf("\nUtility bit operations:\n");
    printf("After setting bits 2 and 5: 0b%08b\n", utility_value);
    printf("Bit 2 is %s\n", BIT_CHECK(utility_value, 2) ? "set" : "clear");
    printf("Bit 3 is %s\n", BIT_CHECK(utility_value, 3) ? "set" : "clear");
    
    BIT_TOGGLE(utility_value, 2);
    printf("After toggling bit 2: 0b%08b\n", utility_value);
}

int main() {
    bitwise_expressions();
    return 0;
}
```

### 10.3 字符串处理表达式

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void string_expressions() {
    printf("=== String Processing Expressions ===\n");
    
    // 1. 字符串长度和比较
    char str1[] = "Hello";
    char str2[] = "World";
    char str3[] = "Hello";
    
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    int cmp1 = strcmp(str1, str2);    // 字典序比较
    int cmp2 = strcmp(str1, str3);    // 相等比较
    int cmp3 = strncmp(str1, str2, 3); // 前n个字符比较
    
    printf("String operations:\n");
    printf("Length of \"%s\": %zu\n", str1, len1);
    printf("Length of \"%s\": %zu\n", str2, len2);
    printf("strcmp(\"%s\", \"%s\") = %d\n", str1, str2, cmp1);
    printf("strcmp(\"%s\", \"%s\") = %d\n", str1, str3, cmp2);
    printf("strncmp(\"%s\", \"%s\", 3) = %d\n", str1, str2, cmp3);
    
    // 2. 字符串复制和连接
    char dest[50];
    char* copy_result = strcpy(dest, str1);  // 复制字符串
    char* concat_result = strcat(dest, " "); // 连接空格
    concat_result = strcat(dest, str2);      // 连接第二个字符串
    
    printf("\nString copying and concatenation:\n");
    printf("Copied string: \"%s\"\n", copy_result);
    printf("Concatenated string: \"%s\"\n", concat_result);
    
    // 3. 字符串查找
    char text[] = "The quick brown fox jumps over the lazy dog";
    char* found = strstr(text, "fox");       // 查找子字符串
    char* first_o = strchr(text, 'o');       // 查找字符首次出现
    char* last_o = strrchr(text, 'o');       // 查找字符最后出现
    
    printf("\nString searching:\n");
    printf("Text: \"%s\"\n", text);
    printf("Found \"fox\" at position: %ld\n", found ? found - text : -1);
    printf("First 'o' at position: %ld\n", first_o ? first_o - text : -1);
    printf("Last 'o' at position: %ld\n", last_o ? last_o - text : -1);
    
    // 4. 字符处理表达式
    char mixed_case[] = "Hello, World! 123";
    printf("\nCharacter processing:\n");
    printf("Original: \"%s\"\n", mixed_case);
    
    // 转换为大写
    for (int i = 0; mixed_case[i]; i++) {
        mixed_case[i] = toupper(mixed_case[i]);
    }
    printf("Uppercase: \"%s\"\n", mixed_case);
    
    // 转换为小写
    for (int i = 0; mixed_case[i]; i++) {
        mixed_case[i] = tolower(mixed_case[i]);
    }
    printf("Lowercase: \"%s\"\n", mixed_case);
    
    // 5. 字符分类
    char test_chars[] = "Hello123!@#";
    printf("\nCharacter classification:\n");
    for (int i = 0; test_chars[i]; i++) {
        char c = test_chars[i];
        printf("'%c': ", c);
        if (isalpha(c)) printf("alpha ");
        if (isdigit(c)) printf("digit ");
        if (isalnum(c)) printf("alnum ");
        if (isspace(c)) printf("space ");
        if (ispunct(c)) printf("punct ");
        if (isupper(c)) printf("upper ");
        if (islower(c)) printf("lower ");
        printf("\n");
    }
    
    // 6. 字符串格式化
    char formatted[100];
    int number = 42;
    double pi = 3.14159;
    char name[] = "Alice";
    
    int formatted_count = sprintf(formatted, "Name: %s, Age: %d, Pi: %.2f", name, number, pi);
    printf("\nFormatted string: \"%s\"\n", formatted);
    printf("Characters written: %d\n", formatted_count);
    
    // 7. 字符串分割（使用 strtok）
    char sentence[] = "apple,banana,cherry,date";
    char* token;
    char sentence_copy[50];
    strcpy(sentence_copy, sentence);  // strtok 会修改原字符串
    
    printf("\nString tokenization:\n");
    token = strtok(sentence_copy, ",");
    while (token != NULL) {
        printf("Token: \"%s\"\n", token);
        token = strtok(NULL, ",");
    }
    
    // 8. 内存操作表达式
    char source[] = "Source data";
    char destination[20];
    
    // 内存复制
    memcpy(destination, source, strlen(source) + 1);
    printf("\nMemory operations:\n");
    printf("Source: \"%s\"\n", source);
    printf("Destination after memcpy: \"%s\"\n", destination);
    
    // 内存设置
    memset(destination, 'X', 5);
    destination[5] = '\0';
    printf("After memset: \"%s\"\n", destination);
    
    // 内存比较
    char mem1[] = "Hello";
    char mem2[] = "Hello";
    char mem3[] = "World";
    
    int mem_cmp1 = memcmp(mem1, mem2, 5);
    int mem_cmp2 = memcmp(mem1, mem3, 5);
    
    printf("memcmp(\"%s\", \"%s\", 5) = %d\n", mem1, mem2, mem_cmp1);
    printf("memcmp(\"%s\", \"%s\", 5) = %d\n", mem1, mem3, mem_cmp2);
    
    // 9. 安全字符串操作（如果支持）
    #ifdef __STDC_LIB_EXT1__
    char safe_dest[50];
    errno_t err = strcpy_s(safe_dest, sizeof(safe_dest), "Safe string copy");
    if (err == 0) {
        printf("Safe string copy successful: \"%s\"\n", safe_dest);
    } else {
        printf("Safe string copy failed\n");
    }
    #endif
}

int main() {
    string_expressions();
    return 0;
}
```

## 11. 最佳实践与代码规范

### 11.1 表达式编写规范

```c
#include <stdio.h>
#include <stdbool.h>

// 好的做法：清晰的表达式编写
void good_expression_practices() {
    printf("=== Good Expression Practices ===\n");
    
    // 1. 使用括号明确优先级
    int a = 10, b = 5, c = 2;
    int result1 = a + b * c;        // 依赖默认优先级
    int result2 = a + (b * c);      // 明确使用括号
    int result3 = (a + b) * c;      // 明确使用括号
    
    printf("Without parentheses: %d + %d * %d = %d\n", a, b, c, result1);
    printf("With parentheses (a + (b * c)): %d\n", result2);
    printf("With parentheses ((a + b) * c): %d\n", result3);
    
    // 2. 避免复杂的嵌套表达式
    int complex_value = 42;
    
    // 不好的做法：复杂嵌套
    // int bad_result = ((complex_value++ * 2) + (++complex_value)) / (complex_value-- - 1);
    
    // 好的做法：分解为简单步骤
    int temp1 = complex_value++ * 2;
    int temp2 = ++complex_value;
    int temp3 = complex_value-- - 1;
    int good_result = (temp1 + temp2) / temp3;
    
    printf("Good practice result: %d\n", good_result);
    
    // 3. 使用有意义的中间变量
    double radius = 5.0;
    double pi = 3.14159;
    
    // 好的做法：清晰的计算步骤
    double radius_squared = radius * radius;
    double area = pi * radius_squared;
    double circumference = 2 * pi * radius;
    
    printf("Circle - Radius: %.2f, Area: %.2f, Circumference: %.2f\n", 
           radius, area, circumference);
    
    // 4. 一致的格式化
    bool condition1 = true;
    bool condition2 = false;
    int value1 = 10;
    int value2 = 20;
    
    // 好的做法：一致的缩进和换行
    int formatted_result = condition1 ? 
                         (value1 + value2) : 
                         (value1 - value2);
    
    printf("Formatted conditional result: %d\n", formatted_result);
    
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
    
    printf("Good side effect result: %d, Counter: %d\n", good_side_effect, counter);
}

int main() {
    good_expression_practices();
    return 0;
}
```

### 11.2 避免常见错误

```c
#include <stdio.h>
#include <stdbool.h>

// 避免的做法：常见的表达式错误
void avoid_common_mistakes() {
    printf("=== Avoid Common Mistakes ===\n");
    
    // 1. 避免混淆赋值和比较
    int x = 5;
    
    // 错误的做法：使用赋值而不是比较
    // if (x = 10) {  // 这是赋值，不是比较！
    //     printf("This will always execute\n");
    // }
    
    // 正确的做法：使用比较
    if (x == 10) {
        printf("x equals 10\n");
    } else {
        printf("x does not equal 10\n");
    }
    
    // 2. 避免浮点数直接比较
    double a = 0.1 + 0.2;
    double b = 0.3;
    
    // 错误的做法：直接比较浮点数
    // if (a == b) {  // 可能不相等由于精度问题
    //     printf("Equal\n");
    // }
    
    // 正确的做法：使用误差范围比较
    double epsilon = 1e-9;
    if (fabs(a - b) < epsilon) {
        printf("a and b are approximately equal (%.17f ≈ %.17f)\n", a, b);
    }
    
    // 3. 避免无符号数和有符号数混合比较
    int signed_val = -1;
    unsigned int unsigned_val = 1U;
    
    // 错误的做法：混合比较导致意外结果
    // if (signed_val < unsigned_val) {  // -1 转换为很大的无符号数
    //     printf("This might not be expected\n");
    // }
    
    // 正确的做法：类型匹配
    if (signed_val < (int)unsigned_val) {
        printf("Correct comparison: %d < %u\n", signed_val, unsigned_val);
    }
    
    // 4. 避免数组越界访问
    int array[5] = {1, 2, 3, 4, 5};
    int index = 10;
    
    // 错误的做法：不检查边界
    // int dangerous = array[index];  // 未定义行为
    
    // 正确的做法：边界检查
    if (index >= 0 && index < 5) {
        int safe = array[index];
        printf("Safe array access: array[%d] = %d\n", index, safe);
    } else {
        printf("Index %d is out of bounds for array of size 5\n", index);
    }
    
    // 5. 避免空指针解引用
    int* ptr = NULL;
    
    // 错误的做法：不检查空指针
    // int dangerous_value = *ptr;  // 未定义行为
    
    // 正确的做法：空指针检查
    if (ptr != NULL) {
        int safe_value = *ptr;
        printf("Safe pointer dereference: %d\n", safe_value);
    } else {
        printf("Pointer is NULL, cannot dereference\n");
    }
    
    // 6. 避免除零错误
    int dividend = 10;
    int divisor = 0;
    
    // 错误的做法：不检查除数
    // int dangerous_division = dividend / divisor;  // 未定义行为
    
    // 正确的做法：除数检查
    if (divisor != 0) {
        int safe_division = dividend / divisor;
        printf("Safe division: %d / %d = %d\n", dividend, divisor, safe_division);
    } else {
        printf("Cannot divide by zero\n");
    }
    
    // 7. 避免未定义的求值顺序
    int undefined_var = 10;
    
    // 错误的做法：同一表达式中多次修改变量
    // int undefined_result = undefined_var++ + undefined_var++;  // 未定义行为
    
    // 正确的做法：分离修改操作
    int safe_result = undefined_var++;
    safe_result += undefined_var++;
    printf("Safe modification result: %d\n", safe_result);
    
    printf("Common mistakes avoided successfully\n");
}

int main() {
    avoid_common_mistakes();
    return 0;
}
```

### 11.3 性能优化表达式

```c
#include <stdio.h>
#include <time.h>

// 性能优化的表达式实践
void performance_optimization() {
    printf("=== Performance Optimization ===\n");
    
    // 1. 避免昂贵的重复计算
    const int SIZE = 1000000;
    
    // 不好的做法：在循环中重复计算
    clock_t start = clock();
    long long bad_sum = 0;
    for (int i = 0; i < SIZE; i++) {
        bad_sum += i * (SIZE / 2);  // 重复计算 SIZE / 2
    }
    clock_t end = clock();
    double bad_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 好的做法：预计算常量表达式
    start = clock();
    long long good_sum = 0;
    const int half_size = SIZE / 2;  // 预计算
    for (int i = 0; i < SIZE; i++) {
        good_sum += i * half_size;
    }
    end = clock();
    double good_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Performance comparison:\n");
    printf("Bad approach time: %.6f seconds\n", bad_time);
    printf("Good approach time: %.6f seconds\n", good_time);
    
    // 2. 使用位运算优化
    int value = 16;
    
    // 乘除法优化
    int multiply_by_8 = value << 3;    // 等价于 value * 8
    int divide_by_4 = value >> 2;      // 等价于 value / 4
    
    printf("\nBit operation optimizations:\n");
    printf("%d * 8 = %d (shift: %d)\n", value, value * 8, multiply_by_8);
    printf("%d / 4 = %d (shift: %d)\n", value, value / 4, divide_by_4);
    
    // 3. 循环优化
    int array[1000];
    
    // 不好的做法：重复计算数组大小
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        array[i] = i;
    }
    
    // 好的做法：预计算数组大小
    const size_t array_size = sizeof(array) / sizeof(array[0]);
    for (size_t i = 0; i < array_size; i++) {
        array[i] = i * 2;
    }
    
    printf("\nArray optimization: first 10 elements: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // 4. 条件表达式优化
    int condition = 1;
    int x = 10, y = 20;
    
    // 简单条件的三元运算符可能比 if-else 更快
    int optimized_result = condition ? x : y;
    printf("\nOptimized conditional result: %d\n", optimized_result);
    
    // 5. 避免不必要的函数调用
    // 不好的做法：在循环中重复调用
    // for (int i = 0; i < strlen(str); i++) { ... }
    
    // 好的做法：预计算长度
    char str[] = "Performance test string";
    size_t str_len = strlen(str);
    for (size_t i = 0; i < str_len; i++) {
        // 处理字符串
    }
    printf("String length precomputed: %zu\n", str_len);
    
    // 6. 使用适当的整数类型
    // 对于小范围的值，使用较小的类型可以节省内存
    uint8_t small_counter = 0;    // 0-255 足够
    uint16_t medium_counter = 0;  // 0-65535 足够
    
    for (int i = 0; i < 100; i++) {
        small_counter++;
        medium_counter += 100;
    }
    
    printf("Small counter: %u, Medium counter: %u\n", small_counter, medium_counter);
    
    printf("Performance optimizations demonstrated\n");
}

int main() {
    performance_optimization();
    return 0;
}
```

## 12. 标准参考

### 12.1 ISO C 标准条款引用

**C89/C90 标准 (ISO/IEC 9899:1990)**：

- §3.3: 表达式
- §3.4: 常量表达式
- §3.5: 声明符
- §3.6: 语句

**C99 标准 (ISO/IEC 9899:1999)**：

- §6.5: 表达式
- §6.6: 常量表达式
- §6.7: 声明
- §6.8: 语句和块

**C11 标准 (ISO/IEC 9899:2011)**：

- §6.5: 表达式
- §6.6: 常量表达式
- §6.7: 声明
- §6.8: 语句和块
- §6.7.9: 初始化
- §6.10: 预处理指令

**C17 标准 (ISO/IEC 9899:2018)**：

- §6.5: 表达式
- §6.6: 常量表达式
- §6.7: 声明
- §6.8: 语句和块

**C23 标准 (ISO/IEC 9899:2024)**：

- §6.5: 表达式
- §6.6: 常量表达式
- §6.7: 声明
- §6.8: 语句和块

### 12.2 相关标准文档

- ISO/IEC 9899: Programming Languages - C
- N1256: C99 标准草案
- N1570: C11 标准草案
- N2176: C17 标准草案
- N3096: C23 标准草案

## 13. 总结

### 13.1 C 表达式的特点

C 表达式是 C 语言计算的基本单元，具有以下核心特性：

**核心特性**：

- 多样性: 支持算术、关系、逻辑、位运算等多种运算符
- 灵活性: 可以组合形成复杂的计算表达式
- 类型安全: 提供隐式和显式类型转换机制
- 性能优化: 编译器对表达式进行优化
- 标准兼容: 遵循 ISO C 标准规范

### 13.2 表达式类型比较

| 类型 | 语法示例 | 特点 | 常见用途 |
|------|----------|------|----------|
| 算术表达式 | `a + b * c` | 数值计算 | 数学运算 |
| 关系表达式 | `x > y` | 比较操作 | 条件判断 |
| 逻辑表达式 | `p && q` | 布尔运算 | 控制流程 |
| 赋值表达式 | `var = value` | 变量赋值 | 数据存储 |
| 条件表达式 | `cond ? a : b` | 三元选择 | 简洁条件 |

### 13.3 实际应用场景

**数学计算库**：

```c
#include <stdio.h>
#include <math.h>

// 数学计算库示例
typedef struct {
    double x, y;
} Vector2D;

// 向量运算表达式
Vector2D vector_add(Vector2D a, Vector2D b) {
    return (Vector2D){a.x + b.x, a.y + b.y};
}

Vector2D vector_multiply(Vector2D v, double scalar) {
    return (Vector2D){v.x * scalar, v.y * scalar};
}

double vector_magnitude(Vector2D v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

double vector_dot_product(Vector2D a, Vector2D b) {
    return a.x * b.x + a.y * b.y;
}

void mathematical_library_example() {
    printf("=== Mathematical Library Example ===\n");
    
    Vector2D v1 = {3.0, 4.0};
    Vector2D v2 = {1.0, 2.0};
    
    // 向量加法
    Vector2D sum = vector_add(v1, v2);
    printf("Vector addition: (%.1f, %.1f) + (%.1f, %.1f) = (%.1f, %.1f)\n", 
           v1.x, v1.y, v2.x, v2.y, sum.x, sum.y);
    
    // 向量数乘
    Vector2D scaled = vector_multiply(v1, 2.0);
    printf("Vector scaling: (%.1f, %.1f) * 2.0 = (%.1f, %.1f)\n", 
           v1.x, v1.y, scaled.x, scaled.y);
    
    // 向量模长
    double magnitude = vector_magnitude(v1);
    printf("Vector magnitude: |(%.1f, %.1f)| = %.2f\n", v1.x, v1.y, magnitude);
    
    // 点积
    double dot_product = vector_dot_product(v1, v2);
    printf("Dot product: (%.1f, %.1f) · (%.1f, %.1f) = %.2f\n", 
           v1.x, v1.y, v2.x, v2.y, dot_product);
    
    // 复杂表达式组合
    Vector2D complex_result = vector_multiply(vector_add(v1, v2), vector_magnitude(v1));
    printf("Complex expression result: (%.2f, %.2f)\n", 
           complex_result.x, complex_result.y);
}

int main() {
    mathematical_library_example();
    return 0;
}
```

**数据处理系统**：

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 数据处理系统示例
typedef struct {
    char name[50];
    int age;
    double salary;
    int department_id;
} Employee;

// 数据过滤表达式
int filter_high_salary(const Employee* emp) {
    return emp->salary > 50000.0;
}

int filter_young_age(const Employee* emp) {
    return emp->age < 30;
}

int filter_it_department(const Employee* emp) {
    return emp->department_id == 101;
}

// 数据转换表达式
Employee promote_employee(Employee emp) {
    emp.salary *= 1.1;  // 10% 加薪
    return emp;
}

double calculate_bonus(const Employee* emp) {
    return emp->salary * 0.05 * (emp->age > 40 ? 1.5 : 1.0);
}

void data_processing_system() {
    printf("=== Data Processing System ===\n");
    
    Employee employees[] = {
        {"Alice", 25, 45000.0, 101},
        {"Bob", 35, 60000.0, 102},
        {"Charlie", 45, 75000.0, 101},
        {"Diana", 28, 55000.0, 103}
    };
    
    int count = sizeof(employees) / sizeof(employees[0]);
    
    printf("Processing %d employees:\n", count);
    
    // 复合过滤表达式
    for (int i = 0; i < count; i++) {
        Employee* emp = &employees[i];
        
        // 复杂条件表达式
        int should_process = filter_high_salary(emp) && 
                           (filter_young_age(emp) || filter_it_department(emp));
        
        if (should_process) {
            printf("Processing employee: %s\n", emp->name);
            
            // 数据转换
            Employee promoted = promote_employee(*emp);
            double bonus = calculate_bonus(&promoted);
            
            printf("  Original salary: %.2f\n", emp->salary);
            printf("  Promoted salary: %.2f\n", promoted.salary);
            printf("  Bonus: %.2f\n", bonus);
        }
    }
    
    // 统计表达式
    int high_salary_count = 0;
    double total_salary = 0.0;
    
    for (int i = 0; i < count; i++) {
        if (filter_high_salary(&employees[i])) {
            high_salary_count++;
        }
        total_salary += employees[i].salary;
    }
    
    double average_salary = total_salary / count;
    double high_salary_percentage = (double)high_salary_count / count * 100;
    
    printf("\nStatistics:\n");
    printf("  Total employees: %d\n", count);
    printf("  Average salary: %.2f\n", average_salary);
    printf("  High salary employees: %d (%.1f%%)\n", high_salary_count, high_salary_percentage);
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
- 标准兼容: 遵循 ISO C 标准规范

### 13.5 学习建议

- 理解基础: 深入掌握运算符优先级和结合性
- 实践应用: 在实际项目中练习表达式编写
- 关注标准: 跟踪 C 标准的新特性和改进
- 代码审查: 建立表达式编写的代码审查机制
- 性能意识: 理解表达式对程序性能的影响

C 表达式作为 C 语言的核心组成部分，虽然概念相对简单，但在实际编程中具有重要作用。掌握其特性和最佳实践，能够帮助开发者编写更高效、更可靠的 C 程序。
