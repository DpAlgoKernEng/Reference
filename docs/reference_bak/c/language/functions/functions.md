# C语言函数（Functions）

来源：cppreference.com

## 函数概览

| 函数相关主题 | 说明 |
|--------------|------|
| **函数声明** | 声明函数名称、返回类型和参数 |
| **函数定义** | 提供函数实现（函数体） |
| **可变参数** | 支持可变数量参数 |
| **内联函数** | C99引入的内联优化 |
| **_Noreturn** | C11引入的不返回函数标记 |

**函数**是C语言构造，将复合语句（函数体）与标识符（函数名）关联。

每个C程序从`main`函数开始执行，该函数要么终止，要么调用其他用户定义或库函数。

---

## 函数基础概念

### 一、函数定义示例

```c
// 函数定义
// 定义名为"sum"的函数，函数体为"{ return x+y; }"
int sum(int x, int y) 
{
    return x + y;
}
```

### 二、函数声明与定义

函数通过**函数声明**或**函数定义**引入：

- **函数声明**：声明函数签名（名称、返回类型、参数）
- **函数定义**：提供函数实现（函数体）

### 三、参数与参数传递

函数可接受零个或多个**参数**，这些参数从函数调用运算符的**实参**初始化，并可通过return语句向调用者返回值。

```c
int n = sum(1, 2); // 参数x和y用实参1和2初始化
```

---

## 函数特性详解

### 一、函数体

函数体在函数定义中提供。每个在表达式中使用的非内联函数（C99起）必须在程序中只定义一次。

### 二、函数作用域

C语言不支持嵌套函数（除非通过非标准编译器扩展）：

- 每个函数定义必须出现在文件作用域
- 函数无法访问调用者的局部变量

### 三、函数示例

```c
int main(void) // main函数定义
{
    int sum(int, int); // 函数声明（可出现在任何作用域）
    int x = 1;         // main中的局部变量
    sum(1, 2);         // 函数调用
    
    // int sum(int a, int b) // 错误：不支持嵌套函数
    // {
    //     return a + b; 
    // }
}

int sum(int a, int b) // 函数定义
{
    // return x + a + b; // 错误：main的x在sum中不可访问
    return a + b;
}
```

---

## 扩展知识详解

### 一、函数声明与定义区别

| 类型 | 函数声明 | 函数定义 |
|------|----------|----------|
| **目的** | 告知编译器函数签名 | 提供函数实现 |
| **位置** | 可在函数内或文件作用域 | 必须在文件作用域 |
| **函数体** | 无 | 有 |
| **示例** | `int func(int, int);` | `int func(int a, int b) { return a + b; }` |

### 二、函数参数传递

#### 1. 值传递
```c
void func(int x) {
    x = 10;  // 不影响调用者
}
```

#### 2. 指针传递
```c
void func(int* x) {
    *x = 10;  // 修改调用者变量
}
```

#### 3. 数组传递（实际是指针）
```c
void func(int arr[], int size) {
    // arr实际是指向int的指针
}
```

### 三、可变参数函数

```c
#include <stdarg.h>
#include <stdio.h>

int sum(int count, ...) {
    va_list args;
    va_start(args, count);
    
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }
    
    va_end(args);
    return total;
}

// 使用示例
int result = sum(3, 10, 20, 30);  // 结果：60
```

### 四、内联函数（C99）

```c
inline int square(int x) {
    return x * x;
}
```

> 💡 **注意**：inline只是建议，编译器可选择是否内联。

### 五、不返回函数（C11）

```c
#include <stdlib.h>

_Noreturn void fatal_error(const char* msg) {
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(EXIT_FAILURE);
}
```

### 六、函数指针

```c
// 函数指针声明
int (*func_ptr)(int, int);

// 函数指针赋值
func_ptr = sum;

// 通过函数指针调用
int result = func_ptr(3, 4);  // 调用sum(3, 4)
```

### 七、递归函数

```c
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
```

---

## 重要注意事项

### 一、函数命名规范

```c
// 推荐命名风格
int calculate_sum(int a, int b);     // 下划线分隔
int CalculateSum(int a, int b);      // 驼峰命名（较少用）
```

### 二、函数设计原则

1. **单一职责**：每个函数应只做一件事
2. **命名清晰**：函数名应清楚表达其功能
3. **参数合理**：避免过多参数
4. **返回值明确**：明确返回值含义

### 三、函数与存储期

```c
// 静态函数（文件作用域）
static int helper_function(int x) {
    return x * 2;
}

// 外部函数（可被其他文件调用）
int public_function(int x) {
    return helper_function(x) + 1;
}
```

---

## 实际应用示例

### 一、完整函数示例

```c
#include <stdio.h>
#include <string.h>

// 函数声明
int max(int a, int b);
void print_array(int arr[], int size);
int find_element(int arr[], int size, int target);

// main函数
int main(void) {
    int numbers[] = {5, 2, 8, 1, 9};
    int size = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("数组最大值: %d\n", max(5, 8));
    print_array(numbers, size);
    
    int index = find_element(numbers, size, 8);
    if (index != -1) {
        printf("找到元素8，索引为: %d\n", index);
    }
    
    return 0;
}

// 函数定义
int max(int a, int b) {
    return (a > b) ? a : b;
}

void print_array(int arr[], int size) {
    printf("数组内容: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int find_element(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;  // 未找到
}
```

### 二、现代C特性应用

```c
// C11 _Noreturn函数
#include <stdnoreturn.h>

noreturn void terminate_program(void) {
    printf("程序终止\n");
    exit(EXIT_FAILURE);
}

// C99 内联函数
inline static int min(int a, int b) {
    return (a < b) ? a : b;
}
```

---

## 标准参考文献

### 各版本C标准对函数的定义：

- **C23标准 (ISO/IEC 9899:2024)**
  - 6.7.7.4 函数声明符（包括原型）
  - 6.9.2 函数定义

- **C17标准 (ISO/IEC 9899:2018)**
  - 6.7.6.3 函数声明符（包括原型）(p: 96-98)
  - 6.9.1 函数定义 (p: 113-115)

- **C11标准 (ISO/IEC 9899:2011)**
  - 6.7.6.3 函数声明符（包括原型）(p: 133-136)
  - 6.9.1 函数定义 (p: 156-158)

- **C99标准 (ISO/IEC 9899:1999)**
  - 6.7.5.3 函数声明符（包括原型）(p: 118-121)
  - 6.9.1 函数定义 (p: 141-143)

- **C89/C90标准 (ISO/IEC 9899:1990)**
  - 3.5.4.3 函数声明符（包括原型）
  - 3.7.1 函数定义

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C++函数声明文档] | C++语言函数对照学习 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=c/language/functions&oldid=179341>
- 最后修改时间：2025年1月7日 22:59
- 离线版本获取时间：2025年2月9日 16:39

---

✅ 函数是C语言程序的基本构建块，理解函数的声明、定义、参数传递机制对于编写结构化程序至关重要。从简单的数学函数到复杂的可变参数函数，每种函数类型都有其特定用途。现代C标准引入的inline函数和_Noreturn属性为性能优化和错误处理提供了新的工具。掌握函数指针和递归等高级特性，能够编写更加灵活和强大的C程序。正确使用static关键字控制函数链接性，有助于构建模块化和可维护的代码。