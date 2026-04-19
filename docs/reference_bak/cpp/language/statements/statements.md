# C++ 语句详解

  [1. 语句基本概念](#1-语句基本概念)  
  
  [2. 语句类型详解](#2-语句类型详解)  
  
  [3. 标签语句深入分析](#3-标签语句深入分析)  
  
  [4. 复合语句与作用域](#4-复合语句与作用域)  
  
  [5. 表达式语句详解](#5-表达式语句详解)  
  
  [6. 选择语句深入探讨](#6-选择语句深入探讨)  
  
  [7. 迭代语句全面解析](#7-迭代语句全面解析)  
  
  [8. 跳转语句深度剖析](#8-跳转语句深度剖析)  
  
  [9. 断言语句（C++26）](#9-断言语句c26)  
  
  [10. 声明语句](#10-声明语句)  
  
  [11. 异常处理语句](#11-异常处理语句)  
  
  [12. 原子块和同步块（TM TS）](#12-原子块和同步块tm-ts)  
  
  [13. 属性说明符（C++11）](#13-属性说明符c11)  
  
  [14. 控制流限制语句](#14-控制流限制语句)  
  
  [15. 子语句与包含关系](#15-子语句与包含关系)  
  
  [16. 常见陷阱与最佳实践](#16-常见陷阱与最佳实践)  
  
  [17. 标准参考](#17-标准参考)  
  
  [18. 总结](#18-总结)

## 1. 语句基本概念

### 1.1 语句的本质

在 C++ 语言中，语句（Statement）是程序执行的基本单位，它指示编译器执行特定的操作。语句与表达式有着本质的区别：

- 表达式（Expression）：计算并产生一个值，如 `a + b`、`func()`、`x = 5`
- 语句（Statement）：执行一个操作，如 `a = b + c;`、`if (x > 0) {...}`

语句是构成程序控制流的基本元素，它们按照顺序执行，形成程序的逻辑结构。

### 1.2 语句与函数的关系

任何函数的主体都是一系列语句。例如：

```cpp
#include <iostream>

// 函数主体是一系列语句
int calculate_sum(int a, int b) {
    int result = a + b;  // 声明语句
    std::cout << "Calculating sum\n";  // 表达式语句
    return result;  // 返回语句
}

int main() {
    int n = 1;                        // 声明语句
    n = n + 1;                        // 表达式语句
    std::cout << "n = " << n << '\n'; // 表达式语句
    return 0;                         // 返回语句
}
```

### 1.3 语句的分类依据

C++ 语言根据语句的功能将其分为多种类型：

- 标记语句：用于控制流目的标记语句
- 表达式语句：执行表达式操作
- 复合语句：组织其他语句的容器
- 选择语句：根据条件选择执行路径
- 迭代语句：重复执行代码块
- 跳转语句：改变程序执行顺序
- 断言语句（C++26）：合同断言
- 声明语句：引入标识符到块中
- 异常处理语句：捕获和处理异常
- 原子块和同步块（TM TS）：事务内存支持

这种分类反映了结构化程序设计的核心思想：顺序、选择、循环，以及现代 C++ 的高级特性。

## 2. 语句类型详解

### 2.1 十种语句类型的特征对比

| 类型 | 特征 | 用途 | 控制流影响 |
|------|------|------|------------|
| 标记语句 | 标签标记的语句 | 控制流标记 | 标记跳转目标 |
| 表达式语句 | 表达式+分号 | 执行操作 | 顺序执行 |
| 复合语句 | `{}`包围的语句序列 | 组织代码块 | 引入新作用域 |
| 选择语句 | 条件分支 | 决策控制 | 改变执行路径 |
| 迭代语句 | 循环执行 | 重复操作 | 循环控制 |
| 跳转语句 | 无条件转移 | 流程控制 | 跳转执行 |
| 断言语句 | 合同断言 | 契约编程 | 条件终止 |
| 声明语句 | 引入标识符 | 变量声明 | 作用域管理 |
| 异常处理语句 | 异常捕获 | 错误处理 | 异常传播 |
| 原子块语句 | 事务内存 | 并发控制 | 原子执行 |

### 2.2 语句的语法结构

每种语句都有其特定的语法结构：

```cpp
// 标记语句
label: statement
case constexpr: statement
default: statement

// 表达式语句
expression;

// 复合语句
{ statement-seq }

// 选择语句
if (condition) statement
if (condition) statement else statement
switch (condition) statement

// 迭代语句
while (condition) statement
do statement while (expression);
for (init; condition; expression) statement
for (range-declaration : range) statement

// 跳转语句
break;
continue;
return expression;
return braced-init-list;
goto identifier;

// 断言语句（C++26）
contract_assert(predicate);

// 声明语句
declaration

// 异常处理语句
try compound-statement handler-seq

// 原子块语句（TM TS）
synchronized compound-statement
atomic_noexcept compound-statement
atomic_cancel compound-statement
atomic_commit compound-statement
```

### 2.3 语句的嵌套特性

C++ 语言允许语句的任意嵌套，这为复杂的程序逻辑提供了支持：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    
    // 三层嵌套循环
    for (size_t i = 0; i < matrix.size(); ++i) {
        std::cout << "Outer loop: " << i << "\n";
        
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            std::cout << "  Middle loop: " << j << "\n";
            
            // 条件语句嵌套
            if (matrix[i][j] > 5) {
                std::cout << "    Value > 5: " << matrix[i][j] << "\n";
                
                // 内层循环
                for (int k = 0; k < matrix[i][j]; ++k) {
                    if (k == 3) {
                        std::cout << "      Breaking at k=3\n";
                        break;
                    }
                    std::cout << "        k=" << k << "\n";
                }
            }
        }
    }
    
    return 0;
}
```

## 3. 标签语句深入分析

### 3.1 标签的语法与语义

标签（Label）是用于标记语句的标识符，允许跳转语句跳转到该位置。标签具有函数作用域，必须在同一个函数内唯一。

```cpp
#include <iostream>

int main() {
    int x = 10;
    
    // 普通标签
    start:
        std::cout << "Start of program\n";
        
        if (x > 5) {
            goto middle;
        }
        
        std::cout << "This won't be printed\n";
        
    middle:
        std::cout << "Middle section\n";
        
        if (x < 20) {
            goto end;
        }
        
        std::cout << "This also won't be printed\n";
        
    end:
        std::cout << "End of program\n";
        
    return 0;
}
```

### 3.2 三种标签类型详解

**普通标签**：

普通标签用于 `goto` 语句的跳转目标：

```cpp
#include <iostream>

void demonstrate_goto() {
    int i = 0;
    
    loop_start:
        if (i >= 5) {
            goto loop_end;
        }
        
        std::cout << "Iteration " << i << "\n";
        i++;
        goto loop_start;
        
    loop_end:
        std::cout << "Loop finished\n";
}
```

**Case 标签**：

Case 标签用于 `switch` 语句中的分支：

```cpp
#include <iostream>

void demonstrate_switch() {
    int choice = 2;
    
    switch (choice) {
        case 1:
            std::cout << "Choice 1 selected\n";
            break;
            
        case 2:
            std::cout << "Choice 2 selected\n";
            // fall-through to case 3
            
        case 3:
            std::cout << "Choice 3 selected (fall-through)\n";
            break;
            
        case 4:
        case 5:
            std::cout << "Choice 4 or 5 selected\n";
            break;
            
        default:
            std::cout << "Invalid choice\n";
    }
}
```

**Default 标签**：

Default 标签用于 `switch` 语句中的默认分支：

```cpp
#include <iostream>

void demonstrate_default() {
    char grade = 'X';
    
    switch (grade) {
        case 'A':
            std::cout << "Excellent\n";
            break;
            
        case 'B':
            std::cout << "Good\n";
            break;
            
        case 'C':
            std::cout << "Average\n";
            break;
            
        case 'D':
            std::cout << "Below Average\n";
            break;
            
        case 'F':
            std::cout << "Fail\n";
            break;
            
        default:
            std::cout << "Invalid grade: " << grade << "\n";
    }
}
```

### 3.3 标签的作用域与生命周期

标签具有函数作用域，这意味着：

- 标签在声明它的函数内可见
- 同一函数内的标签名称必须唯一
- 标签不能跨越函数边界

```cpp
#include <iostream>

void function1() {
    label1:
        std::cout << "In function1\n";
        goto label2;  // 错误：无法跳转到其他函数的标签
}

void function2() {
    label2:
        std::cout << "In function2\n";
}

int main() {
    function1();
    return 0;
}
```

### 3.4 标签的高级用法

**多重标签**:

一个语句可以有多个标签：

```cpp
#include <iostream>

int main() {
    int x = 1;
    
    switch (x) {
        case 1:
        case 2:
        case 3:
            std::cout << "Small number: " << x << "\n";
            break;
            
        case 4:
        case 5:
            std::cout << "Medium number: " << x << "\n";
            break;
            
        default:
            std::cout << "Large number: " << x << "\n";
    }
    
    return 0;
}
```

**标签与复合语句**:

标签可以标记复合语句：

```cpp
#include <iostream>

int main() {
    int i = 0;
    
    outer_loop:
    {
        int j = 0;
        
        inner_loop:
        {
            if (i >= 3) {
                goto end_program;
            }
            
            if (j >= 2) {
                i++;
                goto outer_loop;
            }
            
            std::cout << "i=" << i << ", j=" << j << "\n";
            j++;
            goto inner_loop;
        }
    }
    
    end_program:
        std::cout << "Program ended\n";
        
    return 0;
}
```

### 3.5 C++23 标签的新特性

从 C++23 开始，标签可以与程序中的任何其他实体具有相同的名称：

```cpp
#include <iostream>

void cpp23_label_features() {
    int label = 42;  // 变量名可以与标签名相同
    
    {
        goto label; // 跳转到标签
        label:      // 标签可以出现在块的末尾
            std::cout << "Jumped to label\n";
    }
    
    std::cout << "Variable label: " << label << "\n";  // 输出 42
}
```

## 4. 复合语句与作用域

### 4.1 复合语句的本质

复合语句（Compound Statement）是由大括号 `{}` 包围的语句序列。它不仅是代码组织的工具，更是作用域管理的核心机制。

```cpp
#include <iostream>

int global_var = 100;  // 全局作用域

int main() {
    int outer_var = 10;  // main函数作用域
    
    {
        int inner_var = 20;  // 块作用域
        std::cout << "Global: " << global_var 
                  << ", Outer: " << outer_var 
                  << ", Inner: " << inner_var << "\n";
        
        {
            int nested_var = 30;  // 嵌套块作用域
            std::cout << "All vars: " << global_var 
                      << ", " << outer_var 
                      << ", " << inner_var 
                      << ", " << nested_var << "\n";
        }
        
        // std::cout << nested_var;  // 错误：超出作用域
    }
    
    // std::cout << inner_var;  // 错误：超出作用域
    
    return 0;
}
```

### 4.2 块作用域的规则

**作用域嵌套规则**:

```cpp
#include <iostream>

int x = 100;  // 全局变量

int main() {
    int x = 10;  // 局部变量，遮蔽全局变量
    
    {
        int x = 20;  // 内层局部变量，遮蔽外层变量
        std::cout << "Inner x: " << x << "\n";  // 输出 20
        
        {
            std::cout << "Still inner x: " << x << "\n";  // 输出 20
        }
    }
    
    std::cout << "Outer x: " << x << "\n";  // 输出 10
    std::cout << "Global x: " << ::x << "\n";  // 使用作用域解析符访问全局变量
    
    return 0;
}
```

**变量生命周期**:

```cpp
#include <iostream>

class Resource {
public:
    Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " created\n";
    }
    
    ~Resource() {
        std::cout << "Resource " << id_ << " destroyed\n";
    }
    
private:
    int id_;
};

void demonstrate_lifetime() {
    std::cout << "=== Function start ===\n";
    
    {
        Resource block_resource(1);
        std::cout << "Block resource created\n";
        
        {
            Resource nested_resource(2);
            std::cout << "Nested resource created\n";
            std::cout << "Block resource still accessible\n";
        }
        
        std::cout << "Nested resource destroyed\n";
        std::cout << "Block resource still accessible\n";
    }
    
    std::cout << "Block resource destroyed\n";
    std::cout << "=== Function end ===\n";
}
```

### 4.3 复合语句的高级应用

**作用域隔离**:

```cpp
#include <iostream>

#define SWAP(a, b, type) do { \
    type temp_swap_var = (a); \
    (a) = (b); \
    (b) = temp_swap_var; \
} while(0)

int main() {
    int x = 10, y = 20;
    std::cout << "Before swap: x=" << x << ", y=" << y << "\n";
    
    SWAP(x, y, int);
    
    std::cout << "After swap: x=" << x << ", y=" << y << "\n";
    
    // temp_swap_var 在宏中定义，不会污染外部作用域
    return 0;
}
```

**RAII 与复合语句**:

```cpp
#include <iostream>
#include <fstream>
#include <memory>

void demonstrate_raii() {
    std::cout << "=== RAII demonstration ===\n";
    
    {
        // 文件资源自动管理
        std::ofstream file("test.txt");
        if (file.is_open()) {
            file << "Hello, RAII!\n";
            std::cout << "File written successfully\n";
        }
        
        // 智能指针自动管理内存
        auto ptr = std::make_unique<int>(42);
        std::cout << "Smart pointer value: " << *ptr << "\n";
        
        // 在块结束时，file 自动关闭，ptr 自动释放内存
    }
    
    std::cout << "Resources automatically cleaned up\n";
}
```

## 5. 表达式语句详解

### 5.1 表达式语句的本质

表达式语句是以分号 `;` 结尾的表达式。它是 C++ 程序中最常见的语句类型，用于执行各种操作。

```cpp
#include <iostream>
#include <vector>

int global_counter = 0;

int increment_counter() {
    return ++global_counter;
}

int main() {
    int a = 5, b = 10;
    
    // 赋值表达式语句
    a = b + 3;
    
    // 函数调用表达式语句
    std::cout << "a = " << a << "\n";
    
    // 自增/自减表达式语句
    a++;
    --b;
    
    // 逗号表达式语句
    int result = (a++, b--, a + b);
    
    // 条件表达式语句
    int max = (a > b) ? a : b;
    
    std::cout << "Result: " << result << ", Max: " << max << "\n";
    
    // STL 容器操作表达式语句
    std::vector<int> vec = {1, 2, 3};
    vec.push_back(4);  // 表达式语句
    vec.pop_back();    // 表达式语句
    
    // 函数调用的副作用
    std::cout << "Counter before: " << global_counter << "\n";
    int new_value = increment_counter();
    std::cout << "Counter after: " << global_counter 
              << ", returned: " << new_value << "\n";
    
    return 0;
}
```

### 5.2 空语句的用途

空语句是没有表达式的表达式语句，通常用于循环体或携带标签。

```cpp
#include <iostream>
#include <string>

void demonstrate_null_statements() {
    // 1. 用于循环体
    std::string str = "Hello, World!";
    auto it = str.begin();
    
    // 跳过所有非字母字符
    while (it != str.end() && 
           !((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z')))
        ++it;  // 空语句
    
    if (it != str.end()) {
        std::cout << "First letter: " << *it << "\n";
    }
    
    // 2. 用于携带标签
    int i = 0;
    
    loop_start:
        if (i >= 5) {
            goto loop_end;
        }
        
        std::cout << "Count: " << i << "\n";
        i++;
        ;  // 空语句
        goto loop_start;
        
    loop_end:
        std::cout << "Loop finished\n";
}
```

### 5.3 表达式语句的副作用

许多表达式语句具有副作用，即除了计算值外还会改变程序状态：

```cpp
#include <iostream>
#include <vector>

int global_counter = 0;

int increment_counter() {
    return ++global_counter;
}

int main() {
    int x = 5;
    
    // 1. 赋值操作的副作用
    int y = (x = 10);  // x 被修改，y 获得新值
    std::cout << "x = " << x << ", y = " << y << "\n";
    
    // 2. 自增/自减的副作用
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();
    
    std::cout << "Vector elements: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << *it++ << " ";  // it 被修改
    }
    std::cout << "\n";
    
    // 3. 函数调用的副作用
    std::cout << "Counter before: " << global_counter << "\n";
    int new_value = increment_counter();
    std::cout << "Counter after: " << global_counter 
              << ", returned: " << new_value << "\n";
    
    // 4. 逗号表达式的副作用
    int a = 1, b = 2;
    int result = (a++, b++, a + b);  // a 和 b 都被修改
    std::cout << "a = " << a << ", b = " << b 
              << ", result = " << result << "\n";
    
    return 0;
}
```

### 5.4 表达式语句的最佳实践

**避免复杂的副作用**:

```cpp
#include <iostream>
#include <vector>

std::vector<int> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// 危险的用法：在表达式中多次修改同一变量
void dangerous_usage() {
    int i = 0;
    // arr[i] = i++;  // 未定义行为：i 被多次修改
    // arr[i++] = i;  // 未定义行为：i 被多次修改
    
    // 安全的用法
    arr[i] = i;
    i++;
}

// 安全的用法示例
void safe_usage() {
    int i = 0;
    
    // 清晰的顺序
    arr[i] = 42;
    i++;
    
    // 或者使用逗号表达式确保顺序
    int j = 0;
    (arr[j] = 99, j++);
    
    std::cout << "Safe assignments completed\n";
}
```

**函数调用的副作用管理**:

```cpp
#include <iostream>
#include <chrono>

// 具有副作用的函数
auto get_current_time() {
    static int call_count = 0;
    call_count++;
    std::cout << "Function called " << call_count << " times\n";
    return std::chrono::high_resolution_clock::now();
}

int main() {
    // 每次调用都有副作用
    auto t1 = get_current_time();
    auto t2 = get_current_time();
    auto t3 = get_current_time();
    
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);
    
    std::cout << "Time intervals: " << duration1.count() 
              << " μs, " << duration2.count() << " μs\n";
    
    return 0;
}
```

## 6. 选择语句深入探讨

### 6.1 `if` 语句的详细分析

**基本语法与执行流程**:

```cpp
#include <iostream>

void basic_if_examples() {
    int x = 10;
    
    // 简单 if 语句
    if (x > 5) {
        std::cout << "x is greater than 5\n";
    }
    
    // if-else 语句
    if (x > 15) {
        std::cout << "x is greater than 15\n";
    } else {
        std::cout << "x is not greater than 15\n";
    }
    
    // 嵌套 if 语句
    if (x > 0) {
        if (x < 20) {
            std::cout << "x is between 0 and 20\n";
        } else {
            std::cout << "x is 20 or greater\n";
        }
    } else {
        std::cout << "x is zero or negative\n";
    }
}
```

**悬空 else 问题**:

```cpp
#include <iostream>

void dangling_else_problem() {
    int a = 5, b = 10, c = 15;
    
    // 可能引起混淆的代码
    if (a > 0)
        if (b > 0)
            std::cout << "Both a and b are positive\n";
        else
            std::cout << "This else belongs to the inner if!\n";
    
    // 使用大括号明确意图
    if (a > 0) {
        if (b > 0) {
            std::cout << "Both a and b are positive\n";
        }
    } else {
        std::cout << "This else belongs to the outer if\n";
    }
}
```

**条件表达式的类型转换**:

```cpp
#include <iostream>
#include <cmath>

void condition_evaluation() {
    // 整数条件
    int x = 0;
    if (x) {
        std::cout << "x is non-zero\n";
    } else {
        std::cout << "x is zero\n";
    }
    
    // 指针条件
    char *ptr = nullptr;
    if (ptr) {
        std::cout << "Pointer is not NULL\n";
    } else {
        std::cout << "Pointer is NULL\n";
    }
    
    ptr = const_cast<char*>("Hello");
    if (ptr) {
        std::cout << "Pointer points to: " << ptr << "\n";
    }
    
    // 浮点数条件（注意精度问题）
    double d = 0.1 + 0.2 - 0.3;
    if (d == 0.0) {
        std::cout << "d equals 0.0\n";
    } else {
        std::cout << "d = " << std::setprecision(20) << d 
                  << " (not exactly 0 due to floating point precision)\n";
    }
    
    // 使用 epsilon 比较浮点数
    const double epsilon = 1e-10;
    if (std::abs(d) < epsilon) {
        std::cout << "d is effectively zero\n";
    }
}
```

### 6.2 `switch` 语句深入分析

**基本语法与执行机制**:

```cpp
#include <iostream>

void basic_switch_examples() {
    int choice = 2;
    
    // 基本 switch 语句
    switch (choice) {
        case 1:
            std::cout << "Choice 1\n";
            break;
            
        case 2:
            std::cout << "Choice 2\n";
            break;
            
        case 3:
            std::cout << "Choice 3\n";
            break;
            
        default:
            std::cout << "Invalid choice\n";
    }
    
    // 没有 break 的 switch（fall-through）
    std::cout << "\nFall-through example:\n";
    switch (choice) {
        case 1:
            std::cout << "Case 1\n";
            // fall-through
            
        case 2:
            std::cout << "Case 2\n";
            // fall-through
            
        case 3:
            std::cout << "Case 3\n";
            break;
            
        default:
            std::cout << "Default case\n";
    }
}
```

**switch 语句的限制与特性**:

```cpp
#include <iostream>

void switch_limitations() {
    int value = 2;
    char ch = 'B';
    
    // 1. switch 只能用于整型表达式
    switch (value) {
        case 1:
            std::cout << "Integer case 1\n";
            break;
            
        case 2:
            std::cout << "Integer case 2\n";
            break;
    }
    
    // 2. 字符在 switch 中被视为整数
    switch (ch) {
        case 'A':
            std::cout << "Character A\n";
            break;
            
        case 'B':
            std::cout << "Character B\n";
            break;
            
        case 'C':
            std::cout << "Character C\n";
            break;
    }
    
    // 3. case 标签必须是常量表达式
    const int CONSTANT = 3;
    switch (value) {
        case CONSTANT:  // 合法：const 变量可以作为 case 标签
            std::cout << "Constant case\n";
            break;
            
        // case value:  // 非法：变量不能作为 case 标签
        //     std::cout << "Variable case\n";
        //     break;
    }
}
```

**switch 语句的最佳实践**:

```cpp
#include <iostream>

// 使用枚举提高 switch 的可读性
enum class Color {
    RED = 1,
    GREEN = 2,
    BLUE = 3
};

void color_handler(Color color) {
    switch (color) {
        case Color::RED:
            std::cout << "Red color selected\n";
            break;
            
        case Color::GREEN:
            std::cout << "Green color selected\n";
            break;
            
        case Color::BLUE:
            std::cout << "Blue color selected\n";
            break;
    }
}

// 处理多个相同情况
void grade_handler(char grade) {
    switch (grade) {
        case 'A':
        case 'B':
        case 'C':
            std::cout << "Passing grade: " << grade << "\n";
            break;
            
        case 'D':
        case 'F':
            std::cout << "Failing grade: " << grade << "\n";
            break;
            
        default:
            std::cout << "Invalid grade: " << grade << "\n";
    }
}

int main() {
    color_handler(Color::RED);
    color_handler(Color::GREEN);
    
    grade_handler('A');
    grade_handler('D');
    grade_handler('X');
    
    return 0;
}
```

### 6.3 C++17 `if constexpr` 语句

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
void process_value(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Processing integer: " << value << "\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Processing floating point: " << value << "\n";
    } else {
        std::cout << "Processing other type\n";
    }
}

int main() {
    process_value(42);      // 处理整数
    process_value(3.14);    // 处理浮点数
    process_value("Hello"); // 处理其他类型
    
    return 0;
}
```

### 6.4 C++23 `if consteval` 语句

```cpp
#include <iostream>

consteval int square(int x) {
    return x * x;
}

int main() {
    if consteval {
        // 在常量求值上下文中
        constexpr int result = square(5);
        std::cout << "Compile-time result: " << result << "\n";
    } else {
        // 在运行时上下文中
        int x = 5;
        int result = square(x);
        std::cout << "Runtime result: " << result << "\n";
    }
    
    return 0;
}
```

## 7. 迭代语句全面解析

### 7.1 `while` 循环详解

**基本语法与执行流程**:

```cpp
#include <iostream>
#include <vector>

void while_loop_examples() {
    // 基本 while 循环
    int i = 0;
    while (i < 5) {
        std::cout << "While loop iteration: " << i << "\n";
        i++;
    }
    
    // 条件在循环体末尾检查的变体
    std::cout << "\nModified while loop:\n";
    i = 0;
    while (true) {  // 无限循环
        std::cout << "Modified while iteration: " << i << "\n";
        i++;
        if (i >= 5) {
            break;
        }
    }
    
    // 使用迭代器的 while 循环
    std::cout << "\nIterator-based while loop:\n";
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();
    while (it != vec.end()) {
        std::cout << "Element: " << *it << "\n";
        ++it;
    }
}
```

**while 循环的常见应用场景**:

```cpp
#include <iostream>
#include <sstream>
#include <memory>

void while_loop_applications() {
    // 1. 用户输入验证
    std::string input;
    std::cout << "Enter a positive number: ";
    std::getline(std::cin, input);
    
    std::stringstream ss(input);
    int number;
    while (!(ss >> number) || number <= 0) {
        std::cout << "Invalid input. Enter a positive number: ";
        std::getline(std::cin, input);
        ss.clear();
        ss.str(input);
    }
    std::cout << "Valid number entered: " << number << "\n";
    
    // 2. 文件读取
    std::istringstream file_stream("Line 1\nLine 2\nLine 3\n");
    std::string line;
    std::cout << "File contents:\n";
    while (std::getline(file_stream, line)) {
        std::cout << line << "\n";
    }
    
    // 3. 链表遍历
    struct Node {
        int data;
        std::unique_ptr<Node> next;
        
        Node(int d) : data(d), next(nullptr) {}
    };
    
    auto head = std::make_unique<Node>(1);
    head->next = std::make_unique<Node>(2);
    head->next->next = std::make_unique<Node>(3);
    
    Node* current = head.get();
    while (current != nullptr) {
        std::cout << "Node data: " << current->data << "\n";
        current = current->next.get();
    }
}
```

### 7.2 `do-while` 循环详解

**基本语法与执行流程**:

```cpp
#include <iostream>

void do_while_examples() {
    // 基本 do-while 循环
    int i = 0;
    do {
        std::cout << "Do-while iteration: " << i << "\n";
        i++;
    } while (i < 5);
    
    // 至少执行一次的特性
    std::cout << "\nAt least once execution:\n";
    i = 10;  // 初始条件不满足 while 条件
    do {
        std::cout << "This will execute at least once: " << i << "\n";
        i++;
    } while (i < 5);
    
    // 菜单驱动程序示例
    std::cout << "\nMenu example:\n";
    int choice;
    do {
        std::cout << "1. Option 1\n";
        std::cout << "2. Option 2\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                std::cout << "Option 1 selected\n";
                break;
                
            case 2:
                std::cout << "Option 2 selected\n";
                break;
                
            case 3:
                std::cout << "Exiting...\n";
                break;
                
            default:
                std::cout << "Invalid choice\n";
        }
    } while (choice != 3);
}
```

**do-while 与 while 的比较**:

```cpp
#include <iostream>

void compare_while_do_while() {
    bool condition = false;
    
    std::cout << "While loop with false condition:\n";
    while (condition) {
        std::cout << "This will not execute\n";
    }
    
    std::cout << "Do-while loop with false condition:\n";
    do {
        std::cout << "This will execute once\n";
    } while (condition);
    
    // 实际应用场景对比
    std::cout << "\nUser input validation:\n";
    
    // 使用 while（可能一次都不执行）
    std::cout << "While version:\n";
    std::string input;
    while (std::cout << "Enter a number > 0: " && 
           std::getline(std::cin, input) && 
           (input.empty() || std::stoi(input) <= 0)) {
        std::cout << "Invalid number\n";
    }
    
    // 使用 do-while（至少执行一次提示）
    std::cout << "Do-while version:\n";
    do {
        std::cout << "Enter a number > 0: ";
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << "Invalid input\n";
        } else if (std::stoi(input) <= 0) {
            std::cout << "Number must be positive\n";
        }
    } while (input.empty() || std::stoi(input) <= 0);
}
```

### 7.3 `for` 循环详解

**基本语法与执行流程**:

```cpp
#include <iostream>
#include <vector>

void for_loop_examples() {
    // 基本 for 循环
    std::cout << "Basic for loop:\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "Iteration " << i << "\n";
    }
    
    // 多个初始化和更新表达式
    std::cout << "\nMultiple expressions:\n";
    for (int i = 0, j = 10; i < 5 && j > 5; i++, j--) {
        std::cout << "i = " << i << ", j = " << j << "\n";
    }
    
    // 省略部分表达式
    std::cout << "\nOmitted expressions:\n";
    int k = 0;
    for (; k < 3; ) {
        std::cout << "k = " << k << "\n";
        k++;
    }
    
    // 无限循环
    std::cout << "\nInfinite for loop (first 5 iterations):\n";
    int count = 0;
    for (;;) {
        std::cout << "Infinite loop iteration " << count << "\n";
        count++;
        if (count >= 5) {
            break;
        }
    }
}
```

**for 循环的高级用法**:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void advanced_for_loops() {
    // 1. 数组遍历
    int arr[] = {1, 2, 3, 4, 5};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    
    std::cout << "Array traversal:\n";
    for (size_t i = 0; i < size; i++) {
        std::cout << "arr[" << i << "] = " << arr[i] << "\n";
    }
    
    // 2. 逆序遍历
    std::cout << "\nReverse traversal:\n";
    for (int i = static_cast<int>(size) - 1; i >= 0; i--) {
        std::cout << "arr[" << i << "] = " << arr[i] << "\n";
    }
    
    // 3. 步长不为1的遍历
    std::cout << "\nStep size traversal:\n";
    for (size_t i = 0; i < size; i += 2) {
        std::cout << "Even indices: arr[" << i << "] = " << arr[i] << "\n";
    }
    
    // 4. 嵌套循环
    std::cout << "\nNested loops:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << "(" << i << ", " << j << ") ";
        }
        std::cout << "\n";
    }
    
    // 5. 循环变量的作用域
    std::cout << "\nScope demonstration:\n";
    for (int i = 0; i < 2; i++) {
        std::cout << "Outer loop i = " << i << "\n";
        
        for (int i = 0; i < 2; i++) {  // 内层 i 遮蔽外层 i
            std::cout << "  Inner loop i = " << i << "\n";
        }
        
        // std::cout << "Outer i after inner loop: " << i << "\n";  // 外层 i 仍然可用
    }
}
```

### 7.4 C++11 范围 for 循环详解

```cpp
#include <iostream>
#include <vector>
#include <map>

void range_for_examples() {
    // 1. 基本范围 for 循环
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "Vector elements:\n";
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    // 2. 修改容器元素
    std::cout << "\nModifying elements:\n";
    for (auto& element : vec) {
        element *= 2;
    }
    
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    // 3. 映射遍历
    std::map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
    std::cout << "\nMap elements:\n";
    for (const auto& [key, value] : map) {
        std::cout << key << " = " << value << "\n";
    }
    
    // 4. 数组遍历
    int arr[] = {10, 20, 30, 40, 50};
    std::cout << "\nArray elements:\n";
    for (const auto& element : arr) {
        std::cout << element << " ";
    }
    std::cout << "\n";
}
```

### 7.5 C++20 范围 for 循环初始化

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void cpp20_range_for_init() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // C++20 范围 for 循环初始化
    for (auto it = vec.begin(); auto& element : std::vector<int>(it, it + 3)) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    // 更实用的例子：过滤和遍历
    std::vector<int> filtered;
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(filtered), 
                 [](int x) { return x > 2; });
    
    std::cout << "Filtered elements (> 2):\n";
    for (const auto& element : filtered) {
        std::cout << element << " ";
    }
    std::cout << "\n";
}
```

### 7.6 迭代语句的性能与优化

```cpp
#include <iostream>
#include <vector>
#include <chrono>

void performance_comparison() {
    const size_t SIZE = 1000000;
    std::vector<int> vec(SIZE);
    
    // 初始化向量
    for (size_t i = 0; i < SIZE; ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 1. 基本 for 循环
    long long sum1 = 0;
    for (size_t i = 0; i < SIZE; i++) {
        sum1 += vec[i];
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Basic for loop time: " << duration1.count() 
              << " microseconds, sum = " << sum1 << "\n";
    
    start = std::chrono::high_resolution_clock::now();
    
    // 2. 迭代器遍历
    long long sum2 = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        sum2 += *it;
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Iterator traversal time: " << duration2.count() 
              << " microseconds, sum = " << sum2 << "\n";
    
    start = std::chrono::high_resolution_clock::now();
    
    // 3. 范围 for 循环
    long long sum3 = 0;
    for (const auto& element : vec) {
        sum3 += element;
    }
    
    end = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Range for loop time: " << duration3.count() 
              << " microseconds, sum = " << sum3 << "\n";
}
```

## 8. 跳转语句深度剖析

### 8.1 `break` 语句详解

**基本用法**:

```cpp
#include <iostream>
#include <vector>

void break_statement_examples() {
    // 1. 在循环中使用 break
    std::cout << "Break in for loop:\n";
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            std::cout << "Breaking at i = " << i << "\n";
            break;
        }
        std::cout << "i = " << i << "\n";
    }
    
    // 2. 在 switch 中使用 break
    std::cout << "\nBreak in switch:\n";
    int choice = 2;
    switch (choice) {
        case 1:
            std::cout << "Case 1\n";
            break;  // 防止 fall-through
            
        case 2:
            std::cout << "Case 2\n";
            break;
            
        case 3:
            std::cout << "Case 3\n";
            break;
            
        default:
            std::cout << "Default case\n";
    }
    
    // 3. 嵌套循环中的 break
    std::cout << "\nBreak in nested loops:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) {
                std::cout << "Breaking inner loop at (" << i << ", " << j << ")\n";
                break;
            }
            std::cout << "  (" << i << ", " << j << ")\n";
        }
        std::cout << "Outer loop i = " << i << "\n";
    }
}
```

**break 的限制与注意事项**:

```cpp
#include <iostream>

void break_limitations() {
    // 1. break 只能跳出最近的一层循环或 switch
    std::cout << "Break limitation demonstration:\n";
    for (int i = 0; i < 2; i++) {
        std::cout << "Outer loop: " << i << "\n";
        
        for (int j = 0; j < 3; j++) {
            std::cout << "  Inner loop: " << j << "\n";
            
            if (j == 1) {
                std::cout << "    Breaking inner loop\n";
                break;  // 只跳出内层循环
            }
        }
        std::cout << "Back to outer loop\n";
    }
    
    // 2. 在非循环/switch 语句中使用 break 是错误的
    // if (true) {
    //     break;  // 编译错误
    // }
}
```

### 8.2 `continue` 语句详解

**基本用法**:

```cpp
#include <iostream>

void continue_statement_examples() {
    // 1. 在 for 循环中使用 continue
    std::cout << "Continue in for loop:\n";
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            std::cout << "Skipping even number: " << i << "\n";
            continue;
        }
        std::cout << "Processing odd number: " << i << "\n";
    }
    
    // 2. 在 while 循环中使用 continue
    std::cout << "\nContinue in while loop:\n";
    int j = 0;
    while (j < 5) {
        j++;
        if (j == 3) {
            std::cout << "Skipping iteration 3\n";
            continue;
        }
        std::cout << "Processing iteration: " << j << "\n";
    }
    
    // 3. 在 do-while 循环中使用 continue
    std::cout << "\nContinue in do-while loop:\n";
    int k = 0;
    do {
        k++;
        if (k % 3 == 0) {
            std::cout << "Skipping multiple of 3: " << k << "\n";
            continue;
        }
        std::cout << "Processing: " << k << "\n";
    } while (k < 10);
}
```

**continue 与 break 的对比**:

```cpp
#include <iostream>

void continue_vs_break() {
    std::cout << "Using break:\n";
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            std::cout << "Breaking at " << i << "\n";
            break;  // 完全退出循环
        }
        std::cout << "Processing " << i << "\n";
    }
    
    std::cout << "\nUsing continue:\n";
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            std::cout << "Continuing at " << i << "\n";
            continue;  // 跳过本次迭代，继续下一次
        }
        std::cout << "Processing " << i << "\n";
    }
}
```

### 8.3 `return` 语句详解

**基本用法**:

```cpp
#include <iostream>
#include <optional>
#include <string>

// 无返回值函数
void print_message(const std::string& msg) {
    if (msg.empty()) {
        std::cout << "Error: Empty message\n";
        return;  // 提前返回
    }
    std::cout << "Message: " << msg << "\n";
}

// 有返回值函数
int calculate_square(int x) {
    if (x < 0) {
        std::cout << "Warning: Negative input\n";
        return 0;  // 返回默认值
    }
    return x * x;
}

// 多个返回点
int find_max(int a, int b, int c) {
    if (a >= b && a >= c) {
        return a;
    }
    if (b >= c) {
        return b;
    }
    return c;
}

// 使用 optional 的现代返回方式
std::optional<int> safe_divide(int a, int b) {
    if (b == 0) {
        return std::nullopt;  // 返回空值表示错误
    }
    return a / b;
}

void return_examples() {
    print_message("Hello, World!");
    print_message("");
    
    std::cout << "Square of 5: " << calculate_square(5) << "\n";
    std::cout << "Square of -3: " << calculate_square(-3) << "\n";
    
    std::cout << "Max of 3, 7, 2: " << find_max(3, 7, 2) << "\n";
    
    auto result = safe_divide(10, 2);
    if (result) {
        std::cout << "Division result: " << *result << "\n";
    } else {
        std::cout << "Division by zero error\n";
    }
}
```

**return 语句的高级用法**:

```cpp
#include <iostream>
#include <memory>
#include <vector>

// 早期返回优化
int* find_element(std::vector<int>& vec, int target) {
    if (vec.empty()) {
        return nullptr;  // 早期返回，避免深层嵌套
    }
    
    for (auto& element : vec) {
        if (element == target) {
            return &element;
        }
    }
    
    return nullptr;  // 未找到
}

// RAII 与返回值
std::unique_ptr<int> create_resource(int value) {
    if (value <= 0) {
        return nullptr;  // 返回空指针表示错误
    }
    
    auto resource = std::make_unique<int>(value);
    std::cout << "Resource created with value: " << *resource << "\n";
    return resource;
}

void advanced_return_examples() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int* found = find_element(vec, 3);
    if (found) {
        std::cout << "Found element: " << *found << "\n";
    } else {
        std::cout << "Element not found\n";
    }
    
    auto resource = create_resource(42);
    if (resource) {
        std::cout << "Resource value: " << *resource << "\n";
    } else {
        std::cout << "Resource creation failed\n";
    }
}
```

### 8.4 `goto` 语句详解

**基本用法与争议**:

```cpp
#include <iostream>
#include <memory>

void goto_basic_examples() {
    int i = 0;
    
    // 基本的 goto 使用
    if (i == 0) {
        goto skip_initialization;
    }
    
    std::cout << "This won't be printed\n";
    
skip_initialization:
    std::cout << "Skipped initialization\n";
    
    // 循环模拟
    std::cout << "\nGoto-based loop:\n";
    int j = 0;
    
loop_start:
    if (j >= 5) {
        goto loop_end;
    }
    
    std::cout << "Iteration " << j << "\n";
    j++;
    goto loop_start;
    
loop_end:
    std::cout << "Loop finished\n";
}

// 错误处理中的 goto（推荐用法）
int complex_function() {
    int* ptr1 = nullptr;
    int* ptr2 = nullptr;
    FILE* file = nullptr;
    int result = -1;
    
    ptr1 = new (std::nothrow) int(42);
    if (!ptr1) {
        goto cleanup;
    }
    
    ptr2 = new (std::nothrow) int(84);
    if (!ptr2) {
        goto cleanup;
    }
    
    file = fopen("data.txt", "r");
    if (!file) {
        goto cleanup;
    }
    
    // 正常处理逻辑
    *ptr1 = 42;
    *ptr2 = 84;
    result = 0;  // 成功
    
cleanup:
    if (file) {
        fclose(file);
    }
    if (ptr2) {
        delete ptr2;
    }
    if (ptr1) {
        delete ptr1;
    }
    
    return result;
}
```

**goto 的合理使用场景**:

```cpp
#include <iostream>
#include <memory>

// 状态机实现
enum class State {
    START,
    PROCESSING,
    FINISHED,
    ERROR
};

void state_machine_example() {
    State current_state = State::START;
    
state_start:
    std::cout << "Entering START state\n";
    current_state = State::PROCESSING;
    goto state_processing;
    
state_processing:
    std::cout << "Entering PROCESSING state\n";
    // 模拟处理逻辑
    bool success = true;  // 假设处理成功
    if (success) {
        current_state = State::FINISHED;
        goto state_finished;
    } else {
        current_state = State::ERROR;
        goto state_error;
    }
    
state_finished:
    std::cout << "Entering FINISHED state\n";
    goto state_end;
    
state_error:
    std::cout << "Entering ERROR state\n";
    goto state_end;
    
state_end:
    std::cout << "State machine ended\n";
}

// 多层嵌套循环的跳出
void nested_loop_break_example() {
    bool found = false;
    
    for (int i = 0; i < 10 && !found; i++) {
        for (int j = 0; j < 10 && !found; j++) {
            for (int k = 0; k < 10 && !found; k++) {
                if (i + j + k == 15) {
                    std::cout << "Found combination: " 
                              << i << " + " << j << " + " << k << " = 15\n";
                    found = true;
                    goto search_complete;  // 一次性跳出所有循环
                }
            }
        }
    }
    
search_complete:
    if (found) {
        std::cout << "Search completed successfully\n";
    } else {
        std::cout << "No combination found\n";
    }
}
```

## 9. 断言语句（C++26）

### 9.1 断言语句的引入背景

C++26 标准引入了断言语句（Assertion Statement），允许在代码中添加契约断言。这一特性增强了代码的可维护性和调试能力。

```cpp
#include <iostream>

// C++26 断言语句示例
void process_positive_number(int x) {
    contract_assert(x > 0);  // 契约断言
    std::cout << "Processing positive number: " << x << "\n";
}

void assertion_examples() {
    try {
        process_positive_number(5);   // 正常执行
        process_positive_number(-3);  // 触发断言
    } catch (const std::exception& e) {
        std::cout << "Assertion failed: " << e.what() << "\n";
    }
}
```

### 9.2 断言语句的使用场景

```cpp
#include <iostream>
#include <vector>

class SafeVector {
private:
    std::vector<int> data_;
    
public:
    void push_back(int value) {
        contract_assert(value >= 0);  // 确保值非负
        data_.push_back(value);
    }
    
    int& at(size_t index) {
        contract_assert(index < data_.size());  // 确保索引有效
        return data_.at(index);
    }
    
    size_t size() const {
        return data_.size();
    }
};

void safe_vector_example() {
    SafeVector vec;
    
    try {
        vec.push_back(42);   // 正常
        vec.push_back(-5);   // 触发断言
    } catch (const std::exception& e) {
        std::cout << "Assertion failed: " << e.what() << "\n";
    }
    
    try {
        vec.at(0);  // 正常
        vec.at(10); // 触发断言
    } catch (const std::exception& e) {
        std::cout << "Assertion failed: " << e.what() << "\n";
    }
}
```

## 10. 声明语句

### 10.1 声明语句的本质

声明语句将一个或多个标识符引入到块中。在 C++ 中，声明语句比 C 语言更加丰富和复杂。

```cpp
#include <iostream>
#include <vector>
#include <memory>

void declaration_examples() {
    // 基本变量声明
    int x = 42;
    double y = 3.14;
    
    // 自动类型推导
    auto z = x + y;
    
    // 列表初始化
    std::vector<int> vec{1, 2, 3, 4, 5};
    
    // 智能指针声明
    auto ptr = std::make_unique<int>(100);
    
    // 引用声明
    int& ref = x;
    
    // 常量声明
    const int constant = 1000;
    
    // 静态变量声明
    static int static_var = 0;
    static_var++;
    
    std::cout << "x = " << x << ", y = " << y << ", z = " << z << "\n";
    std::cout << "Vector size: " << vec.size() << "\n";
    std::cout << "Pointer value: " << *ptr << "\n";
    std::cout << "Reference value: " << ref << "\n";
    std::cout << "Constant value: " << constant << "\n";
    std::cout << "Static variable: " << static_var << "\n";
}
```

### 10.2 现代 C++ 声明特性

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

void modern_declaration_examples() {
    // 1. auto 关键字
    auto value = 42;
    auto text = std::string("Hello");
    auto lambda = [](int x) { return x * 2; };
    
    std::cout << "Auto value: " << value << "\n";
    std::cout << "Auto text: " << text << "\n";
    std::cout << "Lambda result: " << lambda(5) << "\n";
    
    // 2. decltype 关键字
    decltype(value) another_value = 100;
    std::cout << "Decltype value: " << another_value << "\n";
    
    // 3. 列表初始化
    std::vector<int> numbers{1, 2, 3, 4, 5};
    std::vector<std::string> words{"Hello", "World"};
    
    std::cout << "Numbers: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n";
    
    std::cout << "Words: ";
    for (const auto& word : words) {
        std::cout << word << " ";
    }
    std::cout << "\n";
    
    // 4. 结构化绑定（C++17）
    std::pair<int, std::string> pair{42, "Answer"};
    auto [number, answer] = pair;
    std::cout << "Structured binding: " << number << ", " << answer << "\n";
}
```

## 11. 异常处理语句

### 11.1 try-catch 语句详解

```cpp
#include <iostream>
#include <stdexcept>
#include <memory>

void exception_handling_examples() {
    try {
        // 抛出标准异常
        throw std::runtime_error("Something went wrong!");
    } catch (const std::runtime_error& e) {
        std::cout << "Caught runtime error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Caught generic exception: " << e.what() << "\n";
    } catch (...) {
        std::cout << "Caught unknown exception\n";
    }
    
    // 嵌套异常处理
    try {
        try {
            throw std::invalid_argument("Invalid argument");
        } catch (const std::exception& e) {
            std::cout << "Inner catch: " << e.what() << "\n";
            throw;  // 重新抛出异常
        }
    } catch (const std::exception& e) {
        std::cout << "Outer catch: " << e.what() << "\n";
    }
}

class Resource {
public:
    Resource() {
        std::cout << "Resource acquired\n";
    }
    
    ~Resource() {
        std::cout << "Resource released\n";
    }
    
    void do_something() {
        throw std::runtime_error("Operation failed");
    }
};

void exception_safety_example() {
    try {
        Resource res;
        res.do_something();  // 这会抛出异常
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
        // Resource 的析构函数会自动调用，确保资源释放
    }
}
```

### 11.2 异常规范与 noexcept

```cpp
#include <iostream>
#include <stdexcept>

// 不抛出异常的函数
void noexcept_function() noexcept {
    std::cout << "This function doesn't throw exceptions\n";
}

// 可能抛出异常的函数
void throwing_function() {
    throw std::runtime_error("This function throws");
}

// 条件 noexcept
template<typename T>
void conditional_noexcept(T value) noexcept(std::is_nothrow_copy_constructible_v<T>) {
    T copy = value;  // 可能抛出异常，取决于 T 的特性
    std::cout << "Copy created\n";
}

void noexcept_examples() {
    noexcept_function();
    
    try {
        throwing_function();
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }
    
    // 条件 noexcept 示例
    conditional_noexcept(42);        // int 是无异常复制构造的
    conditional_noexcept(std::string("Hello"));  // string 可能抛出异常
}
```

## 12. 原子块和同步块（TM TS）

### 12.1 事务内存概念

事务内存（Transactional Memory）是并发编程的一种高级特性，允许将一系列操作作为一个原子事务执行。

```cpp
#include <iostream>
#include <thread>
#include <atomic>

// 注意：以下代码需要支持事务内存的编译器
// 目前大多数编译器尚未完全支持 TM TS

void atomic_block_example() {
    std::atomic<int> counter{0};
    
    // 同步块示例（概念性代码）
    /*
    synchronized {
        // 这里的所有操作都是原子的
        counter++;
        std::cout << "Counter: " << counter.load() << "\n";
    }
    */
    
    std::cout << "Atomic block example (conceptual)\n";
}

void transactional_memory_example() {
    // 原子块示例（概念性代码）
    /*
    atomic_noexcept {
        // 原子执行，不会抛出异常
        perform_atomic_operations();
    }
    
    atomic_cancel {
        // 原子执行，发生异常时回滚
        perform_atomic_operations();
    }
    
    atomic_commit {
        // 原子执行，发生异常时提交
        perform_atomic_operations();
    }
    */
    
    std::cout << "Transactional memory example (conceptual)\n";
}
```

## 13. 属性说明符（C++11）

### 13.1 属性说明符的引入背景

C++11 标准引入了属性说明符（Attribute Specifier），允许为语句、声明等附加元信息。这一特性增强了代码的可读性、可维护性和编译器优化能力。

```cpp
#include <iostream>
#include <vector>

// C++11 属性说明符示例
[[deprecated("Use new_function instead")]]
int old_function(int x) {
    return x * 2;
}

[[noreturn]]
void terminate_program() {
    std::terminate();
}

void attribute_examples() {
    int result1 = old_function(5);  // 编译器可能发出警告
    
    std::cout << "Result: " << result1 << "\n";
    
    // terminate_program();  // 这个函数不会返回
}
```

### 13.2 常用属性说明符

**`[[deprecated]]` 属性**:

```cpp
#include <iostream>

[[deprecated("This function is deprecated, use calculate_new instead")]]
int calculate_old(int x, int y) {
    return x + y;
}

int calculate_new(int x, int y) {
    return x * y;
}

void deprecated_example() {
    int old_result = calculate_old(3, 4);    // 可能产生警告
    int new_result = calculate_new(3, 4);
    
    std::cout << "Old result: " << old_result 
              << ", New result: " << new_result << "\n";
}
```

**`[[noreturn]]` 属性**:

```cpp
#include <iostream>
#include <cstdlib>

[[noreturn]]
void fatal_error(const char* message) {
    std::cerr << "Fatal error: " << message << "\n";
    std::abort();
}

[[noreturn]]
void infinite_loop() {
    while (true) {
        // 永远不会返回
    }
}

void noreturn_example() {
    std::cout << "Before fatal error\n";
    // fatal_error("Something went wrong");  // 程序终止
    std::cout << "This won't be printed\n";
}
```

**`[[maybe_unused]]` 属性（C++17）**:

```cpp
#include <iostream>

void maybe_unused_example() {
    [[maybe_unused]] int debug_variable = 42;
    
#ifdef DEBUG
    std::cout << "Debug variable: " << debug_variable << "\n";
#endif
    
    // 即使在非调试模式下，也不会产生未使用变量警告
}
```

### 13.3 属性说明符在语句中的应用

```cpp
#include <iostream>
#include <vector>

void statement_attributes_example() {
    int x = 10;
    
    // 在 if 语句中使用属性
    [[likely]] if (x > 0) {
        std::cout << "Positive number\n";
    } else {
        std::cout << "Non-positive number\n";
    }
    
    // 在循环中使用属性
    for (int i = 0; i < 100; i++) {
        [[unlikely]] if (i == 99) {
            std::cout << "Last iteration\n";
            break;
        }
        // 处理其他情况
    }
    
    // 在 switch 语句中使用属性
    switch (x) {
        case 10:
            [[fallthrough]];  // 明确表示贯穿执行
        case 20:
            std::cout << "x is 10 or 20\n";
            break;
            
        default:
            std::cout << "Other value\n";
    }
}
```

## 14. 控制流限制语句

### 14.1 控制流限制语句的概念

控制流限制语句是指那些限制控制流转移范围的语句，主要包括：

- try 块的复合语句
- 处理程序的复合语句
- constexpr if 语句的所有子语句（C++17）
- consteval if 语句的所有子语句（C++23）

```cpp
#include <iostream>
#include <stdexcept>

void control_flow_restriction_examples() {
    // 1. try 块的复合语句
    try {
        int x = 42;
        if (x > 0) {
            goto inner_label;  // 错误：不能跳出 try 块
        }
        
    inner_label:
        std::cout << "Inside try block\n";
    } catch (...) {
        std::cout << "Exception caught\n";
    }
    
    // 2. constexpr if 语句的子语句（C++17）
    constexpr int value = 42;
    
    if constexpr (value > 0) {
        std::cout << "Positive value\n";
        // goto outside;  // 错误：不能跳出 constexpr if
    } else {
        std::cout << "Non-positive value\n";
    }
    
// outside:
    // std::cout << "Outside constexpr if\n";
}
```

### 14.2 控制流限制的规则

```cpp
#include <iostream>
#include <stdexcept>

void control_flow_rules() {
    // 规则1：goto 目标标签只能由同一控制流限制语句内的语句引用
    try {
        goto valid_label;  // 合法：在同一 try 块内
        
    valid_label:
        std::cout << "Valid goto within try block\n";
        
        // goto invalid_label;  // 错误：不能跳出 try 块
    } catch (...) {
        std::cout << "Exception handler\n";
    }
    
// invalid_label:
    // std::cout << "This label is outside try block\n";
    
    // 规则2：switch 语句中的 case/default 标签只能与同一 switch 语句相关联
    int choice = 1;
    switch (choice) {
        case 1:
            std::cout << "Case 1\n";
            // case 2:  // 错误：不能在另一个 switch 中定义 case
            //     std::cout << "Case 2\n";
            break;
            
        default:
            std::cout << "Default case\n";
    }
}
```

## 15. 子语句与包含关系

### 15.1 子语句的定义

语句的子语句是指构成该语句的内部语句：

- 对于标记语句，其子语句是被标记的语句
- 对于复合语句，其子语句是其中的所有语句
- 对于选择语句，其子语句是其各个分支的语句
- 对于迭代语句，其子语句是循环体语句

```cpp
#include <iostream>

void substatement_examples() {
    // 1. 标记语句的子语句
    label:
        std::cout << "This is the substatement of the labeled statement\n";
    
    // 2. 复合语句的子语句
    {
        std::cout << "First substatement\n";
        std::cout << "Second substatement\n";
        if (true) {
            std::cout << "Nested substatement\n";
        }
    }
    
    // 3. 选择语句的子语句
    if (true) {
        std::cout << "If substatement\n";
    } else {
        std::cout << "Else substatement\n";
    }
    
    // 4. 迭代语句的子语句
    for (int i = 0; i < 3; ++i) {
        std::cout << "Loop substatement " << i << "\n";
    }
}
```

### 15.2 包含关系

如果语句 S1 是语句 S2 的子语句，或者 S1 包含 S2 的子语句，或者 S1 包含另一个包含 S2 的语句，则称语句 S1 包含语句 S2。

```cpp
#include <iostream>

void containment_examples() {
    // 外层语句包含内层语句
    if (true) {  // 外层 if 语句
        for (int i = 0; i < 3; ++i) {  // 中层 for 语句
            if (i == 1) {  // 内层 if 语句
                std::cout << "Deeply nested statement\n";
            }
        }
    }
    
    // 复合语句包含其中的所有语句
    {
        int x = 42;
        std::cout << "Statement 1\n";
        if (x > 0) {
            std::cout << "Statement 2\n";
        }
        std::cout << "Statement 3\n";
    }
}
```

## 16. 常见陷阱与最佳实践

### 16.1 常见陷阱

**悬空 else 问题**：

```cpp
#include <iostream>

void dangling_else_trap() {
    int a = 5, b = 10;
    
    // 错误的理解：else 属于外层 if
    if (a > 0)
        if (b > 0)
            std::cout << "Both positive\n";
        else
            std::cout << "This else belongs to inner if!\n";  // 实际上是这样
    
    // 正确的做法：使用大括号明确意图
    if (a > 0) {
        if (b > 0) {
            std::cout << "Both positive\n";
        }
    } else {
        std::cout << "a is not positive\n";
    }
}
```

**switch 中的 fall-through**：

```cpp
#include <iostream>

void fallthrough_trap() {
    int grade = 85;
    char letter_grade;
    
    // 危险的 fall-through
    switch (grade / 10) {
        case 10:
        case 9:
            letter_grade = 'A';
            // break;  // 忘记 break，导致 fall-through
        case 8:
            letter_grade = 'B';
            // break;  // 忘记 break
        case 7:
            letter_grade = 'C';
            break;
        default:
            letter_grade = 'F';
    }
    
    std::cout << "Grade " << grade << " -> Letter grade: " 
              << letter_grade << "\n";
    
    // 安全的做法：明确使用 fallthrough 属性（C++17）
    switch (grade / 10) {
        case 10:
        case 9:
            letter_grade = 'A';
            [[fallthrough]];
        case 8:
            letter_grade = 'B';
            [[fallthrough]];
        case 7:
            letter_grade = 'C';
            break;
        default:
            letter_grade = 'F';
    }
    
    std::cout << "Corrected grade " << grade << " -> Letter grade: " 
              << letter_grade << "\n";
}
```

**循环变量的作用域问题**：

```cpp
#include <iostream>

void loop_scope_trap() {
    // C++ 风格：循环变量在循环内声明
    for (int j = 0; j < 3; j++) {
        std::cout << "C++ style: " << j << "\n";
    }
    // std::cout << "j = " << j << "\n";  // 编译错误：j 超出作用域
    
    // 如果需要在循环外访问变量
    int k;
    for (k = 0; k < 3; k++) {
        std::cout << "Accessible outside: " << k << "\n";
    }
    std::cout << "k after loop: " << k << "\n";  // 可以访问
}
```

### 16.2 最佳实践

**使用大括号提高可读性**：

```cpp
#include <iostream>

void brace_best_practices() {
    int x = 5;
    
    // 始终使用大括号，即使只有一条语句
    if (x > 0) {
        std::cout << "Positive\n";
    }
    
    // 嵌套结构使用适当的缩进和空行
    if (x > 0) {
        std::cout << "x is positive\n";
        
        if (x > 10) {
            std::cout << "x is greater than 10\n";
        } else {
            std::cout << "x is between 0 and 10\n";
        }
    }
    
    // 循环也使用大括号
    for (int i = 0; i < 3; i++) {
        std::cout << "Iteration " << i << "\n";
    }
}
```

**合理使用 continue 和 break**：

```cpp
#include <iostream>
#include <vector>

void loop_control_best_practices() {
    std::vector<int> vec = {1, -2, 3, -4, 5, 0, 7};
    
    // 使用 continue 跳过无效数据
    std::cout << "Processing positive numbers only:\n";
    for (const auto& element : vec) {
        if (element <= 0) {
            continue;  // 跳过非正数
        }
        std::cout << "Processing: " << element << "\n";
    }
    
    // 使用 break 提前退出
    std::cout << "\nFinding first negative number:\n";
    for (const auto& element : vec) {
        if (element < 0) {
            std::cout << "First negative number found: " 
                      << element << "\n";
            break;
        }
    }
}
```

**错误处理中的 goto 使用**：

```cpp
#include <iostream>
#include <memory>
#include <fstream>

int resource_management_example() {
    std::unique_ptr<int> ptr1;
    std::unique_ptr<int> ptr2;
    std::ifstream file;
    int result = -1;
    
    ptr1 = std::make_unique<int>(42);
    if (!ptr1) {
        std::cerr << "Failed to allocate memory for ptr1\n";
        goto cleanup;
    }
    
    ptr2 = std::make_unique<int>(84);
    if (!ptr2) {
        std::cerr << "Failed to allocate memory for ptr2\n";
        goto cleanup;
    }
    
    file.open("data.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        goto cleanup;
    }
    
    // 正常处理逻辑
    *ptr1 = 42;
    *ptr2 = 84;
    result = 0;  // 成功
    
cleanup:
    if (file.is_open()) {
        file.close();
    }
    // unique_ptr 会自动释放内存
    
    return result;
}
```

**函数设计中的早期返回**：

```cpp
#include <iostream>
#include <string>
#include <optional>

std::optional<int> validate_and_process(const std::string& input) {
    // 早期返回处理错误情况
    if (input.empty()) {
        std::cerr << "Error: Empty input\n";
        return std::nullopt;
    }
    
    if (input.length() > 100) {
        std::cerr << "Error: Input too long\n";
        return std::nullopt;
    }
    
    // 正常处理逻辑
    std::cout << "Processing input: " << input << "\n";
    return static_cast<int>(input.length());  // 成功
}

void early_return_example() {
    std::vector<std::string> test_inputs = {
        "",
        "This is a very long string that exceeds the maximum allowed length",
        "Valid input"
    };
    
    for (const auto& input : test_inputs) {
        auto result = validate_and_process(input);
        if (result) {
            std::cout << "Result for input: " << *result << "\n";
        } else {
            std::cout << "Processing failed for input\n";
        }
    }
}
```

## 17. 标准参考

### 17.1 C++ 标准版本演进

语句相关标准条款的演进：

- C++26 标准（预计）：9.8 语句，引入 `contract_assert` 语句，增强属性说明符支持
- C++23 标准（ISO/IEC 14882:2024）：8.8 语句，`if consteval` 语句，标签可以与任何实体同名，范围 for 循环初始化
- C++20 标准（ISO/IEC 14882:2020）：8.8 语句，范围 for 循环初始化，`[[likely]]` 和 `[[unlikely]]` 属性
- C++17 标准（ISO/IEC 14882:2017）：8.8 语句，`if constexpr` 语句，`[[fallthrough]]` 属性，`[[maybe_unused]]` 属性
- C++14 标准（ISO/IEC 14882:2014）：8.8 语句，保持了 C++11 的语句特性
- C++11 标准（ISO/IEC 14882:2011）：8.8 语句，引入属性说明符 `[[attribute]]`，范围 for 循环，委托构造函数和继承构造函数
- C++98/C++03 标准（ISO/IEC 14882:1998, 2003）：8.8 语句，建立了基本的语句框架

### 17.2 相关标准条款详解

**9.8 语句（C++26 草案）**:

该条款详细定义了多种语句类型：

- 标记语句（Labeled Statements）：`identifier: statement`
- 表达式语句（Expression Statements）：`expression;`
- 复合语句（Compound Statements）：`{ statement-seq }`
- 选择语句（Selection Statements）：`if` 和 `switch`
- 迭代语句（Iteration Statements）：`while`、`do`、`for` 和范围 for
- 跳转语句（Jump Statements）：`goto`、`continue`、`break` 和 `return`
- 断言语句（Assertion Statements）：`contract_assert`
- 声明语句（Declaration Statements）：各种声明
- 异常处理语句（Exception Handling Statements）：`try` 和 `catch`
- 原子块语句（Atomic Block Statements）：事务内存支持

**语义规则**:

标准中关于语句的重要语义规则：

- 标签具有函数作用域
- 复合语句引入新的块作用域
- `break` 语句只能出现在循环或 `switch` 语句中
- `continue` 语句只能出现在循环语句中
- `return` 语句可以在任何函数中使用
- `goto` 语句只能跳转到同一函数内的标签
- 控制流限制语句限制跳转范围

### 17.3 编译器实现差异

不同编译器在语句处理上的差异：

```cpp
#include <iostream>

void compiler_differences() {
    // 1. 属性说明符支持
    #ifdef __has_cpp_attribute
        #if __has_cpp_attribute(deprecated)
            [[deprecated("Use new_function instead")]]
            void old_function() {
                std::cout << "Old function\n";
            }
        #endif
    #endif
    
    // 2. 范围 for 循环支持
    std::vector<int> vec = {1, 2, 3};
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    
    // 3. 结构化绑定支持（C++17）
    #if __cplusplus >= 201703L
        std::pair<int, std::string> p{42, "Hello"};
        auto [number, text] = p;
        std::cout << "Structured binding: " << number << ", " << text << "\n";
    #endif
    
    // 4. if constexpr 支持（C++17）
    #if __cplusplus >= 201703L
        if constexpr (sizeof(int) >= 4) {
            std::cout << "int is at least 4 bytes\n";
        }
    #endif
}
```

## 18. 总结

### 18.1 核心要点回顾

C++ 语言的语句系统是程序执行的基础，包含多种核心类型：

- 复合语句：提供代码组织和作用域管理
- 表达式语句：执行各种操作和计算
- 选择语句：实现条件分支逻辑
- 迭代语句：支持循环执行模式
- 跳转语句：提供灵活的控制流转移
- 异常处理语句：提供健壮的错误处理机制
- 声明语句：引入标识符和类型到程序中
- 属性说明符：提供编译时元信息
- 现代特性：如范围 for、constexpr if 等

### 18.2 设计原则与最佳实践

**结构化编程原则**：

- 单一入口单一出口：每个函数应该有一个入口和一个出口
- 避免复杂的跳转：尽量减少 `goto` 的使用
- 清晰的控制流：使用适当的缩进和注释
- 早期返回：在函数开始处处理错误情况

**代码可读性**：

```cpp
#include <iostream>
#include <vector>

// 好的代码风格示例
void good_coding_style() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    int sum = 0;
    
    // 清晰的循环结构
    for (const auto& number : numbers) {
        // 明确的条件判断
        if (number > 0) {
            sum += number;
        }
    }
    
    std::cout << "Sum of positive numbers: " << sum << "\n";
}
```

**错误处理策略**：

```cpp
#include <iostream>
#include <optional>
#include <string>

// 统一的错误处理模式
enum class ErrorCode {
    SUCCESS = 0,
    ERROR_NULL_POINTER = -1,
    ERROR_INVALID_INPUT = -2,
    ERROR_MEMORY_ALLOCATION = -3
};

std::optional<int> process_data(const std::vector<int>& data) {
    // 参数验证
    if (data.empty()) {
        std::cerr << "Error: Empty data\n";
        return std::nullopt;
    }
    
    // 数据处理
    int sum = 0;
    for (const auto& element : data) {
        sum += element * 2;
    }
    
    return sum;
}

void error_handling_example() {
    std::vector<int> test_data = {1, 2, 3, 4, 5};
    
    auto result = process_data(test_data);
    if (result) {
        std::cout << "Processing result: " << *result << "\n";
    } else {
        std::cout << "Processing failed\n";
    }
}
```

### 18.3 未来发展趋势

**C++26 新特性的影响**:

C++26 标准引入的特性将进一步增强语句的功能：

- 更好的契约编程支持：通过 `contract_assert` 提供更强的断言能力
- 增强的属性说明符：提供更多的编译时信息
- 改进的优化支持：编译器可以基于新特性进行更好的优化

**现代 C++ 编程实践**:

现代 C++ 编程越来越注重：

- 安全性：避免未定义行为和缓冲区溢出
- 可维护性：清晰的代码结构和良好的注释
- 性能：高效的算法和数据结构选择
- 可移植性：遵循标准，避免编译器特定的扩展
- 资源管理：使用 RAII 和智能指针自动管理资源

**实践项目建议**:

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>

// 综合练习项目：简单的文本处理工具
class TextProcessor {
private:
    std::vector<std::string> lines_;
    
public:
    bool load_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << "\n";
            return false;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            lines_.push_back(line);
        }
        
        return true;
    }
    
    bool save_to_file(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to create file: " << filename << "\n";
            return false;
        }
        
        for (const auto& line : lines_) {
            file << line << "\n";
        }
        
        return true;
    }
    
    void add_line(const std::string& line) {
        lines_.push_back(line);
    }
    
    void print() const {
        for (size_t i = 0; i < lines_.size(); ++i) {
            std::cout << (i + 1) << ": " << lines_[i] << "\n";
        }
    }
    
    size_t line_count() const {
        return lines_.size();
    }
};

void text_processor_example() {
    TextProcessor processor;
    
    // 添加测试数据
    std::vector<std::string> test_lines = {
        "First line",
        "Second line",
        "Third line",
        "Fourth line",
        "Fifth line"
    };
    
    for (const auto& line : test_lines) {
        processor.add_line(line);
    }
    
    std::cout << "Text processor contents:\n";
    processor.print();
    
    std::cout << "Total lines: " << processor.line_count() << "\n";
    
    // 保存到文件
    if (processor.save_to_file("output.txt")) {
        std::cout << "File saved successfully\n";
    }
}
```

通过深入理解和熟练掌握 C++ 语言的语句系统，开发者可以编写出更加高效、安全和可维护的代码。语句不仅是程序执行的基本单位，更是实现复杂逻辑和算法的基础构建块。随着 C++ 标准的不断演进，新的语句特性和最佳实践将继续推动现代 C++ 编程的发展。
