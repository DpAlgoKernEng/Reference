# C++ 函数（Functions）

来源：cppreference.com

## 函数概览

| 函数相关主题 | 说明 |
|--------------|------|
| **声明与定义** | 函数声明、参数列表、函数定义 |
| **参数机制** | 默认参数、可变参数 |
| **现代特性** | 内联说明符、Lambda表达式（C++11）、协程（C++20） |
| **函数调用** | 参数依赖查找（ADL）、函数调用运算符 |
| **函数对象** | 函数对象、重载、运算符重载 |

**函数**是C++实体，将语句序列（**函数体**）与**名称**和零个或多个**函数参数**列表关联。

---

## 函数基础概念

### 一、函数定义示例

```cpp
// 函数名："isodd"
// 参数列表有一个参数，名为"n"，类型为int
// 返回类型为bool
bool isodd(int n)
{                 // 函数体开始
    return n % 2;
}                 // 函数体结束
```

### 二、函数调用机制

当函数被调用时（如在函数调用表达式中）：
- 参数从实参初始化（调用时提供或默认值）
- 函数体中的语句被执行

如果参数列表以`...`结尾，可以向函数提供额外实参，此类函数称为**可变参数函数**。

```cpp
int main()
{
    for (int arg : {-3, -2, -1, 0, 1, 2, 3})
        std::cout << isodd(arg) << ' '; // isodd被调用7次
                                        // 每次n从arg拷贝初始化
}
```

### 三、函数终止方式

函数可通过以下方式终止：
- **返回**：使用return语句
- **抛出异常**：抛出异常
- **协程挂起**：C++20协程可挂起执行以供后续恢复

---

## 函数声明与定义

### 一、声明作用域

- **函数声明**：可出现在任何作用域
- **函数定义**：只能出现在命名空间作用域，或对于成员函数和友元函数，出现在类作用域

### 二、成员函数

在类体中声明且无friend说明符的函数是**类成员函数**，具有许多附加属性。

---

## 函数类型系统

### 一、函数不是对象

- 不存在函数数组
- 函数不能按值传递或从其他函数返回
- 允许函数指针和引用（除main函数和大多数标准库函数外），可在函数本身不能使用的地方使用

因此我们说这些函数是"可寻址的"。

### 二、函数类型组成

每个函数都有类型，包含：
1. **返回类型**
2. **所有参数类型**（经过数组到指针、函数到指针转换）
3. **是否noexcept**（C++17）
4. **非静态成员函数的cv限定和引用限定**（C++11）
5. **语言链接**

> ⚠️ **注意**：不存在cv限定的函数类型（不要与cv限定函数如`int f() const;`或返回cv限定类型的函数如`std::string const f();`混淆）。任何添加到函数类型别名的cv限定符都会被忽略。

---

## 函数重载

### 一、重载规则

同一作用域中的多个函数可以有相同名称，只要：
- 参数列表不同
- 对于非静态成员函数，cv/引用限定不同（C++11）

这称为**函数重载**。

> ⚠️ **限制**：仅返回类型和noexcept说明符（C++17）不同的函数声明不能重载。

### 二、重载解析

重载函数的地址确定方式不同。

---

## 现代C++函数特性

### 一、Lambda表达式（C++11）

C++使用**lambda表达式**实现匿名函数：

```cpp
auto lambda = [](int x, int y) { return x + y; };
int result = lambda(3, 4);  // 结果：7
```

### 二、协程（C++20）

函数可以是**协程**，能够挂起执行以供后续恢复：

```cpp
generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        auto temp = a;
        a = b;
        b = temp + b;
    }
}
```

---

## 函数对象（Function Objects）

### 一、支持的可调用类型

除了函数左值，函数调用表达式还支持：
1. **函数指针**
2. **重载函数调用运算符的类类型值**
3. **可转换为函数指针的值**（包括lambda表达式）（C++11）

这些类型统称为**函数对象**，在C++标准库中广泛使用。

### 二、标准库函数对象

标准库提供预定义函数对象模板和组合方法：

| 函数对象 | 说明 |
|----------|------|
| `std::less` | 小于比较 |
| `std::mem_fn` | 成员函数适配器 |
| `std::bind` | 绑定参数（C++11） |
| `std::function` | 通用函数包装器（C++11） |
| `std::not_fn` | 取反适配器（C++17） |
| `std::bind_front` | 前置绑定（C++20） |
| `std::bind_back` | 后置绑定 |
| `std::move_only_function` | 移动-only函数包装器（C++23） |
| `std::copyable_function` | 可拷贝函数包装器 |
| `std::function_ref` | 函数引用（C++26） |

---

## 扩展知识详解

### 一、参数依赖查找（ADL）

```cpp
namespace ns {
    struct Point { int x, y; };
    void print(const Point& p) {
        std::cout << p.x << "," << p.y;
    }
}

int main() {
    ns::Point p{1, 2};
    print(p);  // ADL找到ns::print
}
```

### 二、默认参数

```cpp
void func(int a, int b = 10, int c = 20) {
    // b和c有默认值
}

func(1);        // 等价于func(1, 10, 20)
func(1, 2);     // 等价于func(1, 2, 20)
func(1, 2, 3);  // 使用所有参数
```

### 三、可变参数模板（现代替代方案）

```cpp
template<typename... Args>
void print(Args... args) {
    ((std::cout << args << " "), ...);  // C++17折叠表达式
}
```

### 四、函数指针与std::function

```cpp
// 函数指针
int add(int a, int b) { return a + b; }
int (*func_ptr)(int, int) = add;

// std::function（C++11）
std::function<int(int, int)> func_obj = add;
```

### 五、内联函数

```cpp
inline int square(int x) {
    return x * x;
}
```

---

## 实际应用示例

### 一、完整函数示例

```cpp
#include <iostream>
#include <functional>
#include <vector>

// 重载函数
void print(int value) {
    std::cout << "Integer: " << value << std::endl;
}

void print(double value) {
    std::cout << "Double: " << value << std::endl;
}

// 默认参数函数
void greet(const std::string& name = "World") {
    std::cout << "Hello, " << name << "!" << std::endl;
}

// 可变参数模板函数
template<typename T>
void variadic_print(T&& t) {
    std::cout << t << std::endl;
}

template<typename T, typename... Args>
void variadic_print(T&& t, Args&&... args) {
    std::cout << t << " ";
    variadic_print(args...);
}

// Lambda示例
auto make_multiplier = [](int factor) {
    return [factor](int value) { return value * factor; };
};

int main() {
    // 函数重载调用
    print(42);      // 调用print(int)
    print(3.14);    // 调用print(double)
    
    // 默认参数
    greet();        // Hello, World!
    greet("C++");   // Hello, C++!
    
    // 可变参数模板
    variadic_print(1, 2.5, "Hello", 'A');
    
    // Lambda使用
    auto doubler = make_multiplier(2);
    std::cout << "Double of 5: " << doubler(5) << std::endl;
    
    // std::function
    std::function<int(int, int)> adder = [](int a, int b) { return a + b; };
    std::cout << "5 + 3 = " << adder(5, 3) << std::endl;
    
    return 0;
}
```

### 二、函数对象应用

```cpp
#include <algorithm>
#include <vector>
#include <functional>

int main() {
    std::vector<int> vec = {5, 2, 8, 1, 9};
    
    // 使用函数对象排序
    std::sort(vec.begin(), vec.end(), std::greater<int>());
    
    // 使用lambda表达式
    std::sort(vec.begin(), vec.end(), [](int a, int b) { return a < b; });
    
    // 使用std::function
    std::function<bool(int, int)> compare = std::less<int>();
    std::sort(vec.begin(), vec.end(), compare);
    
    return 0;
}
```

---

## 标准参考文献

### 相关C++标准章节：

- C++23标准
  - 7.6 Function definitions
- C++20标准
  - 7.6 Function definitions
- C++17标准
  - 11.4 Function definitions
- C++14标准
  - 8.4 Function definitions
- C++11标准
  - 8.4 Function definitions
- C++98/C++03标准
  - 8.4 Function definitions

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C函数文档] | C语言函数对照学习 |
| [Lambda表达式] | C++11 Lambda详细说明 |
| [协程] | C++20协程机制 |
| [函数对象] | 函数对象使用指南 |
| [重载解析] | 重载函数选择规则 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/language/functions&oldid=159983>
- 最后修改时间：2023年10月2日 05:33
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++函数系统是语言的核心特性，从基本函数到现代lambda表达式和协程，提供了强大的抽象和性能优化能力。理解函数重载、参数依赖查找、函数对象等高级特性，对于编写高效、可维护的C++代码至关重要。标准库提供的丰富函数对象和适配器，为函数式编程风格提供了强大支持。掌握默认参数、可变参数模板、内联优化等技术，能够显著提升代码的灵活性和性能。函数指针和std::function等机制，为回调和策略模式等设计模式提供了实现基础。