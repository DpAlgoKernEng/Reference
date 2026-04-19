# 基于范围的 for 循环 (Range-based for loop)

## 1. 概述 (Overview)

基于范围的 for 循环（Range-based for loop）是 C++11 引入的一种循环语句，用于遍历范围内的元素。它提供了一种比传统 for 循环更简洁、更可读的方式来遍历容器或序列中的所有元素。

### 核心概念

基于范围的 for 循环会自动遍历给定范围（range-initializer）中的每个元素，将每个元素的值依次赋给循环变量（item-declaration），使开发者无需手动管理迭代器或索引。这种语法特别适合遍历标准库容器（如 `std::vector`、`std::map`）、数组、初始化列表等序列类型。

### 技术定位

- **简化遍历**：避免手动编写迭代器循环的样板代码
- **提高安全性**：自动处理边界条件，减少越界错误
- **增强可读性**：意图明确，代码更简洁
- **泛型编程友好**：适用于任何支持迭代器的容器类型

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C++11 之前，遍历容器需要使用迭代器或索引，代码冗长且容易出错。例如，遍历一个 `std::vector` 需要编写：

```cpp
for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    // 使用 *it
}
```

### 设计动机

基于范围的 for 循环借鉴了其他现代编程语言（如 Python、C#、Java）中的 foreach 语法，旨在：

1. **简化常见任务**：遍历容器是最常见的操作之一，值得提供专用语法
2. **降低错误率**：自动管理迭代器生命周期，避免迭代器失效等问题
3. **统一遍历语法**：无论是数组、容器还是初始化列表，都使用相同的语法

### 版本演变

| C++ 版本 | 特性变更 | 说明 |
|---------|---------|------|
| C++11 | 首次引入 | 基本语法，支持容器、数组和初始化列表 |
| C++17 | begin/end 类型可不同 | 允许 `begin()` 和 `end()` 返回不同类型，支持哨兵模式 |
| C++20 | 初始化语句 | 支持 `for (init; declaration : range)` 形式的初始化语句 |
| C++23 | 临时对象生命周期扩展 | 扩展 range-initializer 中所有临时对象的生命周期 |

### 特性测试宏

| 宏名称 | 值 | 标准 | 特性 |
|--------|-----|------|------|
| `__cpp_range_based_for` | 200907L | C++11 | 基于范围的 for 循环 |
| `__cpp_range_based_for` | 201603L | C++17 | begin/end 类型可不同 |
| `__cpp_range_based_for` | 202211L | C++23 | 临时对象生命周期扩展 |

## 3. 语法与参数 (Syntax and Parameters)

### 基本语法

```cpp
attr(可选) for ( init-statement(可选) item-declaration : range-initializer ) statement
```

### 参数说明

| 参数 | 可选性 | 说明 |
|------|--------|------|
| `attr` | 可选 | 任意数量的属性（attribute） |
| `init-statement` | C++20 起，可选 | 初始化语句，可以是表达式语句、简单声明或别名声明（C++23），必须以分号结尾 |
| `item-declaration` | 必需 | 循环变量的声明，每个元素的声明 |
| `range-initializer` | 必需 | 表达式或花括号初始化列表，定义要遍历的范围 |
| `statement` | 必需 | 循环体，通常是复合语句 |

### init-statement 形式（C++20 起）

`init-statement` 可以是以下之一：

1. **表达式语句**：可以是一个空语句（单独的分号）
2. **简单声明**：通常是一个带初始化器的变量声明，可以声明多个变量或使用结构化绑定
3. **别名声明**（C++23 起）：`using` 别名声明

```cpp
// 表达式语句
for (; auto x : vec) { /* ... */ }

// 简单声明
for (auto n = vec.size(); auto x : vec) { /* ... */ }

// 别名声明 (C++23)
for (using T = int; T x : vec) { /* ... */ }
```

### item-declaration 限制

`item-declaration` 必须是一个简单声明，且具有以下限制：

- 只能有一个声明符
- 声明符不能有初始化器
- 声明说明符序列只能包含类型说明符和 `constexpr`，不能定义类或枚举

```cpp
// 合法
for (int x : vec) { }
for (const auto& x : vec) { }
for (auto&& x : vec) { }

// 非法
for (int x = 0 : vec) { }     // 错误：不能有初始化器
for (int x, y : vec) { }      // 错误：不能有多个声明符
```

## 4. 底层原理 (Underlying Principles)

### 等价代码展开

编译器将基于范围的 for 循环展开为等价的传统循环。不同 C++ 版本的展开方式略有不同。

#### C++11 到 C++14 的展开

```cpp
{
    auto&& __range = range-initializer;
    for (auto __begin = __begin-expr, __end = __end-expr;
         __begin != __end; ++__begin) {
        item-declaration = *__begin;
        statement
    }
}
```

#### C++17 到 C++19 的展开

```cpp
{
    auto&& __range = range-initializer;
    auto __begin = __begin-expr;
    auto __end = __end-expr;
    for (; __begin != __end; ++__begin) {
        item-declaration = *__begin;
        statement
    }
}
```

**关键变化**：`begin` 和 `end` 的类型可以不同，只需支持 `!=` 比较即可。

#### C++20 及以后的展开

```cpp
{
    init-statement
    auto&& __range = range-initializer;
    auto __begin = __begin-expr;
    auto __end = __end-expr;
    for (; __begin != __end; ++__begin) {
        item-declaration = *__begin;
        statement
    }
}
```

### begin-expr 和 end-expr 的确定规则

编译器按照以下顺序确定 `__begin-expr` 和 `__end-expr`：

#### 规则 1：数组类型

如果 `__range` 是数组类型的引用：

- 如果数组有已知边界 N，则 `__begin-expr` 为 `__range`，`__end-expr` 为 `__range + N`
- 如果数组边界未知或不完整类型，程序非良构

```cpp
int arr[5] = {1, 2, 3, 4, 5};
for (int x : arr) { }  // begin = arr, end = arr + 5
```

#### 规则 2：类成员函数

如果 `__range` 是类类型的引用，且在该类的作用域中查找名称 "begin" 和 "end" 都能找到至少一个声明：

- `__begin-expr` 为 `__range.begin()`
- `__end-expr` 为 `__range.end()`

```cpp
std::vector<int> vec = {1, 2, 3};
for (int x : vec) { }  // 调用 vec.begin() 和 vec.end()
```

#### 规则 3：非成员函数

如果上述规则都不适用，则通过参数依赖查找（ADL, Argument-Dependent Lookup）查找 `begin` 和 `end`：

- `__begin-expr` 为 `begin(__range)`
- `__end-expr` 为 `end(__range)`

**注意**：不执行非 ADL 查找（常规的非限定查找）。

### 临时对象生命周期

#### 基本规则

如果 `range-initializer` 返回临时对象，其生命周期会扩展到循环结束，因为它绑定到转发引用 `__range`。

#### C++23 前的问题

在 C++23 之前，只有绑定到 `__range` 的临时对象才会延长生命周期，其他中间临时对象不会：

```cpp
// C++23 前：未定义行为
for (auto& x : foo().items()) {
    // foo() 返回的临时对象可能在循环开始前就析构
}
```

#### C++23 的改进

从 C++23 开始，`range-initializer` 中所有临时对象的生命周期都会延长：

```cpp
// C++23：安全
for (auto& x : foo().items()) {
    // foo() 返回的对象生命周期延长到循环结束
}
```

#### 使用 init-statement 的解决方案（C++20）

在 C++23 之前，可以使用 `init-statement` 解决：

```cpp
for (T thing = foo(); auto& x : thing.items()) {
    // thing 的生命周期延续到循环结束
}
```

#### 注意事项

即使在 C++23 中，按值传递的函数参数也不会延长生命周期（在某些 ABI 中，它们在调用者而非被调用者中销毁）：

```cpp
using T = std::list<int>;
const T& f1(const T& t) { return t; }
const T& f2(T t) { return t; }  // 总是返回悬垂引用

T g();

void foo() {
    for (auto e : f1(g())) {} // OK: g() 返回值的生命周期延长
    for (auto e : f2(g())) {} // UB: f2 的值参数过早销毁
}
```

### 特殊情况

#### 初始化列表

如果 `range-initializer` 是花括号初始化列表，`__range` 会被推导为 `std::initializer_list` 的引用。

```cpp
for (int x : {1, 2, 3, 4, 5}) {
    // __range 的类型是 std::initializer_list<int>&&
}
```

#### 成员查找的特殊性

成员优先规则：如果范围类型有名为 "begin" 和 "end" 的成员，无论它们是类型、数据成员、函数还是枚举值，也无论其可访问性如何，都会使用成员解释。

```cpp
class meow {
    enum { begin = 1, end = 2 };
    // ...
};

// 即使有命名空间作用域的 begin/end 函数
// 这个类也不能用于基于范围的 for 循环
```

## 5. 使用场景 (Use Cases)

### 适用场景

#### 1. 遍历标准库容器

```cpp
std::vector<int> vec = {1, 2, 3, 4, 5};

// 只读访问
for (const auto& x : vec) {
    std::cout << x << ' ';
}

// 修改元素
for (auto& x : vec) {
    x *= 2;
}
```

#### 2. 遍历数组

```cpp
int arr[] = {1, 2, 3, 4, 5};
for (int x : arr) {
    std::cout << x << ' ';
}
```

#### 3. 遍历初始化列表

```cpp
for (int x : {1, 2, 3, 4, 5}) {
    std::cout << x << ' ';
}
```

#### 4. 泛型编程

```cpp
template<typename Container>
void print(const Container& c) {
    // 使用转发引用，适用于任何容器
    for (auto&& x : c) {
        std::cout << x << ' ';
    }
}
```

### 最佳实践

#### 1. 选择合适的访问方式

| 访问方式 | 适用场景 | 示例 |
|---------|---------|------|
| `auto` | 小类型，只读，需要拷贝 | `for (auto x : vec)` |
| `const auto&` | 只读访问，避免拷贝 | `for (const auto& x : vec)` |
| `auto&` | 需要修改元素 | `for (auto& x : vec)` |
| `auto&&` | 泛型代码，转发引用 | `for (auto&& x : container)` |

#### 2. 使用转发引用进行泛型编程

在泛型代码中，推荐使用 `auto&&`：

```cpp
template<typename Container>
void process(Container& c) {
    for (auto&& x : c) {
        // 对于 vector<int>，x 是 int&
        // 对于 const vector<int>，x 是 const int&
        // 对于 vector<bool>，x 是 proxy reference
    }
}
```

#### 3. 避免写时复制（Copy-on-Write）陷阱

对于具有写时复制语义的对象，基于范围的 for 循环可能触发深拷贝：

```cpp
struct cow_string { /* ... */ };  // 写时复制字符串

cow_string str = /* ... */;

// 可能触发深拷贝（非 const 版本的 begin()）
for (auto x : str) { /* ... */ }

// 避免深拷贝
for (auto x : std::as_const(str)) { /* ... */ }
```

### 常见陷阱

#### 陷阱 1：重声明错误

不能在循环体重声明 `init-statement` 中引入的名称：

```cpp
// 错误：重声明
for (int i : {1, 2, 3})
    int i = 1;  // 错误：重声明
```

#### 陷阱 2：迭代器失效

虽然循环会自动管理迭代器，但如果在循环中修改容器结构（如插入/删除元素），仍可能导致迭代器失效：

```cpp
std::vector<int> vec = {1, 2, 3, 4, 5};

// 危险：迭代器可能失效
for (auto& x : vec) {
    if (x == 3) {
        vec.push_back(6);  // 可能导致迭代器失效
    }
}
```

#### 陷阱 3：临时对象生命周期（C++23 前）

```cpp
// C++23 前：未定义行为
for (auto& x : getVector().items()) {
    // getVector() 返回的临时对象可能已析构
}

// 正确做法（C++20 起）
for (auto temp = getVector(); auto& x : temp.items()) {
    // temp 的生命周期延续到循环结束
}
```

#### 陷阱 4：成员查找的意外行为

如果类有名为 `begin` 和 `end` 的成员（即使是数据成员），编译器会优先使用成员解释：

```cpp
class BadRange {
    int begin = 0;
    int end = 10;
public:
    // 没有成员函数 begin() 和 end()
};

BadRange br;
// 错误：会尝试使用 br.begin 和 br.end 作为迭代器
for (int x : br) { }  // 编译错误
```

## 6. 代码示例 (Examples)

### 示例 1：基本用法

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {0, 1, 2, 3, 4, 5};

    // 常量引用访问（只读）
    for (const int& i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // 值访问（拷贝）
    for (auto i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // 转发引用访问
    for (auto&& i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

输出：
```
0 1 2 3 4 5
0 1 2 3 4 5
0 1 2 3 4 5
```

### 示例 2：不同初始化方式

```cpp
#include <iostream>
#include <vector>

int main() {
    // 使用花括号初始化列表
    for (int n : {0, 1, 2, 3, 4, 5}) {
        std::cout << n << ' ';
    }
    std::cout << '\n';

    // 使用数组
    int a[] = {0, 1, 2, 3, 4, 5};
    for (int n : a) {
        std::cout << n << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

输出：
```
0 1 2 3 4 5
0 1 2 3 4 5
```

### 示例 3：C++20 初始化语句

```cpp
#include <iostream>
#include <vector>
#include <type_traits>

int main() {
    std::vector<int> v = {0, 1, 2, 3, 4, 5};

    // 使用初始化语句（C++20）
    for (auto n = v.size(); auto i : v) {
        std::cout << --n + i << ' ';
    }
    std::cout << '\n';

    // typedef 作为初始化语句（C++20）
    for (typedef decltype(v)::value_type elem_t; elem_t i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // 别名声明作为初始化语句（C++23）
    for (using elem_t = decltype(v)::value_type; elem_t i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

输出：
```
5 5 5 5 5 5
0 1 2 3 4 5
0 1 2 3 4 5
```

### 示例 4：常量容器的访问

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {0, 1, 2, 3, 4, 5};
    const auto& cv = v;

    // 对于 const 容器，auto&& 推导为 const int&
    for (auto&& i : cv) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

输出：
```
0 1 2 3 4 5
```

### 示例 5：修改元素

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {0, 1, 2, 3, 4, 5};

    // 使用引用修改元素
    for (auto& i : v) {
        i *= 2;
    }

    for (const auto& i : v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

输出：
```
0 2 4 6 8 10
```

### 示例 6：避免写时复制陷阱

```cpp
#include <iostream>
#include <vector>
#include <memory>

// 模拟写时复制字符串
class cow_string {
    std::shared_ptr<std::string> data;
public:
    cow_string(const char* s) : data(std::make_shared<std::string>(s)) {}

    // 非 const 的 begin() 可能触发深拷贝
    char* begin() {
        if (!data.unique()) {
            data = std::make_shared<std::string>(*data);
        }
        return &(*data)[0];
    }

    const char* begin() const { return data->c_str(); }
    const char* end() const { return data->c_str() + data->size(); }
    char* end() {
        if (!data.unique()) {
            data = std::make_shared<std::string>(*data);
        }
        return &(*data)[0] + data->size();
    }
};

int main() {
    cow_string str = "hello";

    // 可能触发深拷贝
    // for (auto c : str) { }  // 危险

    // 安全：使用 const 版本
    for (auto c : std::as_const(str)) {
        std::cout << c << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

### 示例 7：使用属性

```cpp
#include <iostream>

int main() {
    int a[] = {0, 1, 2, 3, 4, 5};

    // 使用 [[maybe_unused]] 属性
    for ([[maybe_unused]] int n : a) {
        std::cout << 1 << ' ';  // 循环变量不需要使用
    }
    std::cout << '\n';

    return 0;
}
```

输出：
```
1 1 1 1 1 1
```

### 示例 8：临时对象生命周期（C++23）

```cpp
#include <iostream>
#include <vector>

std::vector<int> getVector() {
    return {1, 2, 3, 4, 5};
}

struct Wrapper {
    std::vector<int> items;

    std::vector<int>& getItems() & { return items; }
    std::vector<int> getItems() && { return std::move(items); }
};

Wrapper getWrapper() {
    return Wrapper{{1, 2, 3, 4, 5}};
}

int main() {
    // C++23 前：可能未定义行为
    // C++23 起：安全，临时对象生命周期延长
    for (auto x : getWrapper().getItems()) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    // C++20 的解决方案
    for (auto w = getWrapper(); auto x : w.getItems()) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

### 常见错误及修正

#### 错误 1：循环变量有初始化器

```cpp
// 错误
for (int x = 0 : {1, 2, 3}) { }  // 编译错误：声明符不能有初始化器

// 正确
for (int x : {1, 2, 3}) { }
```

#### 错误 2：在循环体重声明循环变量

```cpp
// 错误
for (int i : {1, 2, 3})
    int i = 1;  // 编译错误：重声明

// 正确
for (int i : {1, 2, 3}) {
    int j = i;  // 使用不同的名称
}
```

#### 错误 3：修改容器结构

```cpp
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // 危险：可能导致迭代器失效
    for (auto x : v) {
        // v.push_back(x);  // 危险：可能使当前迭代器失效
    }

    // 正确：如果需要修改结构，使用索引或传统循环
    for (size_t i = 0; i < v.size(); ++i) {
        v.push_back(v[i]);  // 安全：使用索引
    }

    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **语法简洁**：基于范围的 for 循环提供了遍历容器的简洁语法，避免了迭代器的样板代码。

2. **自动管理**：编译器自动处理迭代器的初始化、递增和比较，减少了出错的机会。

3. **版本演进**：
   - C++11：基础功能
   - C++17：begin/end 类型可以不同（支持哨兵）
   - C++20：支持初始化语句
   - C++23：临时对象生命周期自动延长

4. **访问方式选择**：根据需求选择合适的访问方式（`auto`、`const auto&`、`auto&`、`auto&&`）。

5. **泛型友好**：在泛型代码中使用 `auto&&` 可以适应各种容器类型。

### 与传统 for 循环对比

| 特性 | 基于范围的 for | 传统 for 循环 |
|------|---------------|--------------|
| 语法简洁性 | 高 | 中 |
| 类型安全性 | 自动推导 | 需要手动指定 |
| 边界管理 | 自动 | 手动 |
| 灵活性 | 中（只能遍历整个范围） | 高（可部分遍历、反向等） |
| 修改容器结构 | 不推荐 | 可以 |
| 性能 | 与手写循环相当 | - |

### 学习建议

1. **优先使用**：在遍历整个容器时，优先使用基于范围的 for 循环。

2. **理解原理**：了解编译器的展开机制有助于理解高级用法和陷阱。

3. **注意生命周期**：在 C++23 之前，注意临时对象的生命周期问题，使用 `init-statement` 解决。

4. **泛型编程**：在泛型代码中，使用 `auto&&` 转发引用以适应各种容器类型。

5. **避免修改结构**：不要在基于范围的 for 循环中修改容器结构（插入/删除元素）。

### 缺陷报告

| 缺陷编号 | 应用版本 | 发布行为 | 正确行为 |
|---------|---------|---------|---------|
| CWG 1442 | C++11 | 非成员 `begin`/`end` 的查找是否包含常规非限定查找未明确 | 不进行常规非限定查找 |
| CWG 2220 | C++11 | `init-statement` 中引入的名称可以在循环体重声明 | 程序非良构 |
| CWG 2825 | C++11 | 如果 `range-initializer` 是花括号初始化列表，会查找非成员 `begin`/`end` | 会查找成员 `begin`/`end` |
| P0962R1 | C++11 | 只要存在任一成员 `begin` 或 `end` 就使用成员解释 | 必须两者都存在才使用成员解释 |

### 相关内容

- **`std::for_each`**：对范围内的每个元素应用函数对象
- **迭代器**：理解迭代器有助于深入理解基于范围的 for 循环
- **范围库（C++20）**：提供更强大的范围操作功能