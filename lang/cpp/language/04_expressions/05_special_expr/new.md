# new 表达式（new expression）

## 1. 概述（Overview）

**new 表达式**（new expression）是 C++ 中用于创建和初始化具有**动态存储期**（dynamic storage duration）对象的核心机制。所谓动态存储期，是指对象的生命周期不限于其创建时所在的作用域，而是由程序员显式控制。

### 核心概念

- **动态存储期**：对象在堆上分配，生命周期持续到被显式销毁（通过 delete 表达式）
- **动态数组**：可以在运行时确定数组大小，这是 C++ 中直接创建动态数组的唯一方式
- **placement new**：在已分配的内存区域中构造对象，不分配新内存

### 技术定位

new 表达式是 C++ 内存管理的核心组成部分，负责：
1. 调用分配函数（operator new）获取内存
2. 在获取的内存上构造对象
3. 返回指向构造对象的指针

## 2. 来源与演变（Origin and Evolution）

### 历史背景

new 表达式起源于 C++ 的早期设计，为了解决 C 语言中 `malloc`/`free` 与构造函数/析构函数之间的脱节问题。malloc 只能分配内存，无法调用构造函数；而 new 表达式将内存分配与对象构造合二为一，确保资源获取即初始化（RAII）的原则得以实现。

### 版本演变

| C++ 版本 | 主要变更 |
|---------|---------|
| C++98 | 引入基础 new 表达式，支持动态数组和 placement new |
| C++11 | 支持列表初始化（brace-init）、auto 类型推导占位符 |
| C++14 | 允许编译器省略或合并可替换分配函数的调用 |
| C++17 | 支持对齐超过 `__STDCPP_DEFAULT_NEW_ALIGNMENT__` 的对象；支持类模板参数推导 |
| C++20 | new 表达式可在常量表达式中求值（分配函数调用被省略） |

### 设计动机

1. **类型安全**：返回类型化的指针，而非 `void*`
2. **自动构造**：自动调用构造函数进行初始化
3. **异常处理**：分配失败时抛出异常（可通过 `std::nothrow` 改变行为）
4. **可定制性**：允许用户自定义分配函数（operator new）

## 3. 语法与参数（Syntax and Parameters）

### 语法形式

| 语法 | 说明 |
|------|------|
| `::`(可选) `new` `(`type`)` new-initializer(可选) | 带括号的类型（语法形式 1） |
| `::`(可选) `new` type new-initializer(可选) | 不带括号的类型（语法形式 2） |
| `::`(可选) `new` `(`placement-args`)` `(`type`)` new-initializer(可选) | placement new，带括号类型（语法形式 3） |
| `::`(可选) `new` `(`placement-args`)` type new-initializer(可选) | placement new，不带括号类型（语法形式 4） |

### 参数说明

| 参数 | 说明 |
|------|------|
| `type` | 目标类型标识符（type-id），可以是数组类型，从 C++11 起可包含占位符类型说明符，从 C++17 起可包含待推导的类模板名 |
| `new-initializer` | 括号包围的表达式列表，或花括号包围的初始化列表（C++11 起） |
| `placement-args` | 额外的传递给分配函数的参数 |
| `::` | 可选的作用域解析符，指定使用全局分配函数 |

### 括号的使用场景

当类型中包含括号时，必须使用语法形式（1）或（3）：

```cpp
new int(*[10])();    // 错误：被解析为 (new int) (*[10]) ()
new (int (*[10])()); // 正确：分配一个包含 10 个函数指针的数组
```

类型标识符会贪婪解析，包含所有可以作为声明符部分的标记：

```cpp
new int + 1; // 正确：解析为 (new int) + 1
new int * 1; // 错误：解析为 (new int*) (1)
```

### new-initializer 的必要性

new-initializer 不是可选的情况：
- 类型是未知边界数组时
- 类型中使用了占位符（`auto` 或 `decltype(auto)`）（C++11 起）
- 类型中使用了需要推导参数的类模板（C++17 起）

```cpp
// C++11 示例
double* p = new double[]{1, 2, 3};  // 创建 double[3] 类型数组
auto p = new auto('c');              // 创建 char 类型对象，p 是 char*

// C++20 示例
auto q = new std::integral auto(1);  // 正确：q 是 int*
auto q = new std::floating_point auto(true); // 错误：类型约束不满足

// C++17 示例
auto r = new std::pair(1, true);     // 正确：r 是 std::pair<int, bool>*
auto r = new std::vector;            // 错误：无法推导元素类型
```

## 4. 底层原理（Underlying Principles）

### 内存分配机制

new 表达式通过调用**分配函数**（allocation function）来获取内存：

| 对象类型 | 调用的分配函数 |
|---------|--------------|
| 非数组类型 | `operator new` |
| 数组类型 | `operator new[]` |

### 查找顺序

1. 如果 new 表达式以 `::` 开头（如 `::new T`），则在全局作用域查找分配函数
2. 如果 `T` 是类类型，则首先在类 `T` 的作用域中查找
3. 否则在全局作用域查找

### 内存传递参数

调用分配函数时，new 表达式传递的参数：

```cpp
new T;                           // 调用 operator new(sizeof(T))
                                 // C++17: 或 operator new(sizeof(T), std::align_val_t(alignof(T)))
new T[5];                        // 调用 operator new[](sizeof(T)*5 + overhead)
                                 // C++17: 或 operator new[](sizeof(T)*5+overhead, std::align_val_t(alignof(T)))
new(2, f) T;                     // 调用 operator new(sizeof(T), 2, f)
                                 // C++17: 或 operator new(sizeof(T), std::align_val_t(alignof(T)), 2, f)
```

### 数组分配开销

数组分配可能包含额外的开销（overhead），用于存储数组元素数量等信息，以便 `delete[]` 表达式调用正确数量的析构函数。Itanium C++ ABI 和 MSVC 要求：如果元素类型是平凡可析构的（trivially destructible），则数组分配开销为零。

### 对齐处理（C++17 起）

当分配的对象的对齐要求超过 `__STDCPP_DEFAULT_NEW_ALIGNMENT__` 时，new 表达式会将包装在 `std::align_val_t` 中的对齐要求作为第二个参数传递给分配函数。

如果重载解析失败（例如类特定分配函数签名不同），会进行第二次重载解析，这次不包括对齐参数。

### 初始化流程

创建的对象根据以下规则初始化：

#### 单对象（非数组类型）

| new-initializer | 初始化方式 |
|----------------|-----------|
| 缺失 | 默认初始化（default-initialization） |
| 括号包围的表达式列表 | 直接初始化（direct-initialization） |
| 花括号包围的初始化列表（C++11 起） | 列表初始化（list-initialization） |

#### 数组对象

| new-initializer | 初始化方式 |
|----------------|-----------|
| 缺失 | 每个元素默认初始化 |
| 一对空括号 `()` | 每个元素值初始化（value-initialization） |
| 花括号包围的初始化列表（C++11 起） | 聚合初始化（aggregate-initialization） |
| 括号包围的非空表达式列表（C++20 起） | 聚合初始化 |

### 初始化失败处理

如果初始化因抛出异常而终止，程序会查找匹配的释放函数（deallocation function），然后：

1. **找到匹配的释放函数**：调用该函数释放内存，然后异常继续传播
2. **未找到匹配的释放函数**：异常继续传播但不释放内存（可能导致内存泄漏）

释放函数的查找作用域：
- 如果 new 表达式不以 `::` 开头，且分配类型是类类型 `T` 或类类型 `T` 的数组，则在类 `T` 的作用域中搜索
- 否则在全局作用域搜索

## 5. 使用场景（Use Cases）

### 动态数组

new 表达式是 C++ 中直接创建**动态数组**（运行时确定大小的数组）的唯一方式：

```cpp
int n = 42;
double a[n][5];            // 错误：数组大小必须是常量
auto p1 = new double[n][5]; // 正确：第一维可以是非常量
auto p2 = new double[5][n]; // 错误：只有第一维可以是非常量
auto p3 = new (double[n][5]); // 错误：动态数组不能使用语法形式 (1)
```

### Placement New

placement new 用于在已分配的内存区域中构造对象：

```cpp
#include <new>

// 在栈上预分配内存
alignas(T) unsigned char buf[sizeof(T)];

// 使用 placement new 在预分配内存上构造对象
T* tptr = new(buf) T;

// 必须手动调用析构函数
tptr->~T();
```

**应用场景**：
- 自定义内存池
- 共享内存中构造对象
- 性能敏感场景（避免重复分配）

### 智能指针配合

为避免内存泄漏，new 表达式的结果通常存储在智能指针中：

```cpp
#include <memory>

// C++11 起
std::unique_ptr<int> p1 = std::make_unique<int>(42);
std::shared_ptr<int> p2 = std::make_shared<int>(42);

// 或直接使用 new（不推荐）
std::unique_ptr<int> p3(new int(42));
```

### 常见陷阱

#### 内存泄漏

```cpp
// 指针被覆盖
int* p = new int(7);
p = nullptr;  // 内存泄漏！

// 指针超出作用域
void f() {
    int* p = new int(7);
}  // 内存泄漏！

// 异常导致未释放
void f() {
    int* p = new int(7);
    g();        // 可能抛出异常
    delete p;   // 如果 g() 抛出异常，这行不会执行
}
```

**解决方案**：使用智能指针或 RAII 包装类。

#### 数组大小无效

```cpp
// C++11 起的行为
int n = -1;
int* p = new int[n];  // 如果分配函数是不抛出的，返回 nullptr
                      // 否则抛出 std::bad_array_new_length
```

#### placement new 的空指针

```cpp
void* ptr = nullptr;
int* p = new(ptr) int;  // 未定义行为！
```

## 6. 代码示例（Examples）

### 基础用法

```cpp
#include <iostream>

int main() {
    // 分配单个对象
    int* p1 = new int;        // 默认初始化（值不确定）
    int* p2 = new int();      // 值初始化（值为 0）
    int* p3 = new int(42);    // 直接初始化
    int* p4 = new int{42};    // 列表初始化（C++11）

    std::cout << *p3 << std::endl;  // 输出: 42

    delete p1;
    delete p2;
    delete p3;
    delete p4;

    return 0;
}
```

### 动态数组

```cpp
#include <iostream>

int main() {
    size_t n = 5;

    // 动态数组
    int* arr = new int[n];

    // 初始化数组元素
    for (size_t i = 0; i < n; ++i) {
        arr[i] = static_cast<int>(i * 10);
    }

    // 使用数组
    for (size_t i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // 释放数组（必须使用 delete[]）
    delete[] arr;

    return 0;
}
```

### Placement New

```cpp
#include <iostream>
#include <new>

class MyClass {
public:
    MyClass(int x) : value(x) {
        std::cout << "MyClass constructed with value: " << value << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass destroyed with value: " << value << std::endl;
    }
    int getValue() const { return value; }

private:
    int value;
};

int main() {
    // 在栈上分配足够大的内存
    alignas(MyClass) unsigned char buffer[sizeof(MyClass)];

    // 使用 placement new 在 buffer 上构造对象
    MyClass* obj = new(buffer) MyClass(42);

    std::cout << "Object value: " << obj->getValue() << std::endl;

    // 必须手动调用析构函数
    obj->~MyClass();

    // buffer 自动释放（栈内存）
    return 0;
}
```

### 自定义分配函数

```cpp
#include <iostream>
#include <cstdlib>

class MyClass {
public:
    MyClass() { std::cout << "MyClass constructed\n"; }
    ~MyClass() { std::cout << "MyClass destroyed\n"; }

    // 类特定的 operator new
    static void* operator new(size_t size) {
        std::cout << "Custom operator new called, size = " << size << std::endl;
        return std::malloc(size);
    }

    // 类特定的 operator delete
    static void operator delete(void* ptr) {
        std::cout << "Custom operator delete called" << std::endl;
        std::free(ptr);
    }
};

int main() {
    MyClass* obj = new MyClass();  // 调用自定义 operator new

    delete obj;  // 调用自定义 operator delete

    MyClass* arr = new MyClass[3];  // 调用 operator new[]
    delete[] arr;                   // 调用 operator delete[]

    // 使用 :: 强制使用全局分配函数
    MyClass* obj2 = ::new MyClass();
    ::delete obj2;

    return 0;
}
```

### 使用 nothrow

```cpp
#include <iostream>
#include <new>

int main() {
    // 不抛出异常的 new
    int* p = new(std::nothrow) int[1000000000000];

    if (p == nullptr) {
        std::cout << "Allocation failed, returned nullptr" << std::endl;
    } else {
        std::cout << "Allocation succeeded" << std::endl;
        delete[] p;
    }

    return 0;
}
```

### 常见错误示例

```cpp
#include <iostream>

int main() {
    // 错误 1：内存泄漏
    int* p1 = new int(42);
    p1 = new int(100);  // 第一个分配的内存泄漏！
    delete p1;

    // 错误 2：数组使用 delete 而非 delete[]
    int* arr = new int[10];
    delete arr;  // 未定义行为！应该是 delete[] arr

    // 错误 3：忘记 delete
    int* p2 = new int(42);
    // 函数结束但没有 delete p2

    // 错误 4：重复 delete
    int* p3 = new int(42);
    delete p3;
    delete p3;  // 未定义行为！

    return 0;
}
```

### 正确用法示例

```cpp
#include <iostream>
#include <memory>

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

int main() {
    // 推荐方式 1：使用智能指针
    {
        auto p1 = std::make_unique<Resource>(1);
        auto p2 = std::make_shared<Resource>(2);
        // 自动释放，无需手动 delete
    }

    // 推荐方式 2：如果必须使用裸指针，确保配对
    {
        int* p = new int(42);
        std::cout << "Value: " << *p << std::endl;
        delete p;  // 确保释放
    }

    // 推荐方式 3：RAII 包装
    {
        std::unique_ptr<int[]> arr = std::make_unique<int[]>(10);
        arr[0] = 100;
        std::cout << "arr[0] = " << arr[0] << std::endl;
        // 自动释放数组
    }

    return 0;
}
```

## 7. 总结（Summary）

### 核心要点

1. **new 表达式** 是 C++ 动态内存管理的核心，将内存分配与对象构造合二为一
2. **动态存储期** 使对象生命周期独立于作用域，由程序员显式控制
3. **动态数组** 是唯一可直接创建运行时大小数组的方式
4. **placement new** 允许在预分配内存上构造对象，常用于内存池和性能优化
5. **异常安全** 是使用 new 的重要考量，推荐使用智能指针管理

### 内存管理最佳实践

| 场景 | 推荐方式 | 原因 |
|------|---------|------|
| 单对象所有权 | `std::unique_ptr` + `std::make_unique` | 零开销，自动管理 |
| 共享所有权 | `std::shared_ptr` + `std::make_shared` | 引用计数，自动管理 |
| 数组 | `std::vector` | 自动管理，支持动态扩容 |
| 固定大小数组 | `std::array` | 栈分配，无开销 |
| 自定义内存管理 | placement new + RAII | 控制分配位置，避免碎片 |

### 技术对比

| 特性 | new 表达式 | malloc |
|------|----------|--------|
| 类型安全 | 返回类型化指针 | 返回 void* |
| 构造函数 | 自动调用 | 不调用 |
| 内存来源 | 可定制（operator new） | 堆（系统调用） |
| 失败处理 | 抛出异常 | 返回 NULL |
| 大小计算 | 自动 | 手动 |

### 学习建议

1. **理解底层机制**：掌握 operator new/operator delete 的工作原理
2. **使用智能指针**：现代 C++ 中应尽量避免直接使用裸指针管理动态内存
3. **了解 RAII**：资源获取即初始化是 C++ 内存管理的核心原则
4. **注意配对使用**：new 与 delete、new[] 与 delete[] 必须配对
5. **学习标准库容器**：std::vector、std::string 等已封装好内存管理

### 相关主题

- constructor（构造函数）
- copy elision（拷贝省略）
- default constructor（默认构造函数）
- delete expression（delete 表达式）
- destructor（析构函数）
- initialization（初始化）
  - aggregate initialization（聚合初始化）
  - default initialization（默认初始化）
  - direct initialization（直接初始化）
  - list initialization（列表初始化）
  - value initialization（值初始化）