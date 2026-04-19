# 成员访问操作符 (Member Access Operators)

## 1. 概述 (Overview)

成员访问操作符 (Member Access Operators) 是 C++ 中用于访问对象成员（数据成员和成员函数）的一组操作符。这些操作符提供了对数组元素、指针指向对象、对象成员以及成员指针的访问能力。

成员访问操作符包括以下几种：

| 操作符名称 | 语法 | 可否重载 | 说明 |
|-----------|------|---------|------|
| 下标操作符 (subscript) | `a[b]` | 是 | 访问数组元素或容器元素 |
| 解引用操作符 (indirection) | `*a` | 是 | 访问指针指向的对象 |
| 取地址操作符 (address-of) | `&a` | 是 | 获取对象或函数的地址 |
| 对象成员操作符 (member of object) | `a.b` | 否 | 访问对象的成员 |
| 指针成员操作符 (member of pointer) | `a->b` | 是 | 通过指针访问对象成员 |
| 对象成员指针操作符 (pointer to member of object) | `a.*b` | 否 | 通过成员指针访问对象成员 |
| 指针成员指针操作符 (pointer to member of pointer) | `a->*b` | 是 | 通过成员指针访问指针指向对象的成员 |

**核心作用**：
- 提供对数组、指针、对象成员的安全访问机制
- 支持用户自定义类型实现类似内置类型的行为
- 是 C++ 面向对象编程和泛型编程的基础设施

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

成员访问操作符的设计源于 C 语言，C++ 在继承这些操作符的基础上进行了扩展：

**C 语言时期**：
- 基础的下标访问 `a[i]`
- 解引用 `*ptr` 和取地址 `&obj`
- 结构体成员访问 `obj.member` 和 `ptr->member`

**C++ 扩展**：
- 引入成员指针 (pointer to member) 概念及其访问操作符 `.*` 和 `->*`
- 支持操作符重载，允许自定义类型模拟内置类型行为
- 引入智能指针和迭代器的成员访问语义

### 版本变更历史

| C++ 版本 | 主要变更 |
|---------|---------|
| **C++98** | 确立基本的成员访问操作符语义和重载规则 |
| **C++11** | • 下标操作符应用于数组右值结果为 xvalue（CWG 1213）<br>• 成员函数可添加 ref-qualifier `&` 和 `&&` |
| **C++17** | 下标表达式的求值顺序规范化 |
| **C++20** | 不推荐在 `a[b,c]` 中使用未加括号的逗号表达式作为下标 |
| **C++23** | • 禁止在下标操作符右侧使用未加括号的逗号表达式<br>• 支持多维下标操作符 `a[...]`<br>• 显式对象成员函数的地址获取规则 |

### 关键缺陷报告

| 缺陷报告 | 问题 | 修正 |
|---------|------|------|
| CWG 1213 | 数组右值的下标结果被误分类为 lvalue | 修正为 xvalue |
| CWG 1458 | 对不完整类型取地址的行为未定义 | 改为未指定行为 |
| CWG 1642 | 成员指针访问的右操作数可以是 lvalue | 修正为必须是 rvalue |
| CWG 2614 | E2 为引用成员或枚举器时 E1.E2 的结果不清晰 | 明确语义 |
| CWG 2748 | 通过空指针访问静态成员的行为不清晰 | 明确为未定义行为 |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 下标操作符 (Subscript Operator)

**语法形式**：

```cpp
expr1[expr2]                           // (1) 基本形式
expr1[{expr, ...}]                     // (2) 初始化列表形式 (C++11 起)
expr1[expr2, expr, ...]                // (3) 多维下标形式 (C++23 起)
```

**参数要求**：
- 形式 (1)：其中一个表达式必须是 "T 类型数组" 的 glvalue 或 "指向 T 的指针" 的 prvalue，另一个必须是整数或无作用域枚举类型的 prvalue
- 形式 (2)(3)：仅用于调用重载的 `operator[]`

**语义规则**：
- 内置下标表达式 `E1[E2]` 与表达式 `*(E1 + E2)` 完全等价（除值类别和求值顺序）
- 应用于数组：结果是 lvalue（数组为 lvalue）或 xvalue（数组非 lvalue）
- 应用于指针：结果总是 lvalue

**重载签名**：

```cpp
// 成员函数
R& T::operator[](S b);

// 非成员函数
T& operator[](T*, std::ptrdiff_t);
T& operator[](std::ptrdiff_t, T*);
```

### 3.2 解引用操作符 (Indirection Operator)

**语法形式**：

```cpp
*expr
```

**参数要求**：
- `expr` 必须是指向对象或函数的指针

**语义规则**：
- 结果是指针指向对象或函数的 lvalue 引用
- 如果指针不指向有效对象或函数，行为未定义
- `void*` 不能解引用
- 不完整类型的指针可解引用，但结果只能用于允许不完整类型的上下文

**重载签名**：

```cpp
// 成员函数
R& T::operator*();

// 非成员函数
R& operator*(T a);
T& operator*(T*);
```

### 3.3 取地址操作符 (Address-of Operator)

**语法形式**：

```cpp
&expr                    // (1) 对象/函数地址
&class::member           // (2) 成员指针
```

**参数要求**：
- 形式 (1)：`expr` 为对象或函数类型的 lvalue
- 形式 (2)：`member` 为非静态成员或变体成员（非显式对象成员函数）

**语义规则**：
- 形式 (1)：返回指向对象或函数的 prvalue 指针
- 形式 (2)：返回指向成员的 prvalue 指针（成员指针）
- 如果类型定义了自定义 `operator&`，使用 `std::addressof` 获取真实地址
- 重载函数的地址获取需要上下文解析

**C++23 注意**：
- 显式对象成员函数的地址必须使用限定标识符，如 `&C::method`

### 3.4 成员访问操作符 (Member Access Operators)

**语法形式**：

```cpp
expr.template id-expr          // (1) 对象成员访问
expr->template id-expr          // (2) 指针成员访问
expr.pseudo-destructor         // (3) 伪析构函数调用（对象）
expr->pseudo-destructor        // (4) 伪析构函数调用（指针）
```

**参数要求**：
- 形式 (1)：`expr` 为完整类类型 `T` 的表达式
- 形式 (2)：`expr` 为指向完整类类型 `T*` 的指针
- 形式 (3)(4)：`expr` 为标量类型

**id-expr 规则**：
- 命名数据成员或成员函数
- 可限定：`E1.B::E2` 或 `E1->B::E2`
- 可使用 template 消歧符：`E1.template E2`

**访问静态成员**：
- 如果 `id-expr` 命名静态成员或枚举器，`expr` 是弃值表达式

**访问非静态数据成员的结果**：

| E1 类型 | E2 类型 | 结果类型 |
|---------|---------|---------|
| lvalue | 非引用成员 | lvalue |
| rvalue/xvalue | 非引用成员 | xvalue |
| 任意 | 引用成员 `T&` 或 `T&&` | lvalue |

### 3.5 成员指针访问操作符 (Pointer-to-Member Access Operators)

**语法形式**：

```cpp
lhs.*rhs        // (1) 对象的成员指针访问
lhs->*rhs       // (2) 指针的成员指针访问
```

**参数要求**：
- 形式 (1)：`lhs` 为类类型 `T` 的表达式
- 形式 (2)：`lhs` 为指向类类型 `T*` 的指针
- `rhs` 为指向 `T` 或其基类 `B` 的成员指针类型的 rvalue

**语义规则**：

1. **数据成员指针**：
   - `lhs` 为 lvalue → 结果为 lvalue
   - `lhs` 为 rvalue/xvalue → 结果为 xvalue

2. **成员函数指针**：
   - 结果是特殊 prvalue，只能作为成员函数调用的左操作数

3. **空成员指针**：
   - 如果 `rhs` 为空成员指针值，行为未定义

4. **ref-qualifier 规则**（C++11 起）：

| lhs 类型 | 成员函数 ref-qualifier | 合法性 |
|---------|----------------------|--------|
| rvalue | `&` | 非法（除非是 const 非 volatile） |
| lvalue | `&&` | 非法 |

**重载签名**：

```cpp
// 成员函数
R& T::operator->*(S b);

// 非成员函数
R& operator->*(T a, S b);
R& operator->*(D*, R B::*);
```

## 4. 底层原理 (Underlying Principles)

### 4.1 下标操作符的实现机制

**内置语义**：

```cpp
E1[E2] ≡ *(E1 + E2)
```

指针算术规则：
1. 如果 `E1` 是指针，指向数组元素或尾后位置
2. 指针调整遵循数组指针算术规则
3. 结果类型为被指向类型 `T`

**内存访问模式**：

```
数组: int arr[4] = {10, 20, 30, 40};
      地址:   0x100  0x104  0x108  0x10c

arr[2] 的计算:
  arr → 0x100 (int*)
  2   → 整数偏移
  arr + 2 → 0x100 + 2*sizeof(int) = 0x108
  *(arr + 2) → 访问 0x108 的 int 值 = 30
```

**逆向下标**：

```cpp
int arr[4] = {1, 2, 3, 4};
int* p = &arr[2];
p[1]      // arr[3] = 4
p[-1]     // arr[1] = 2
1[p]      // arr[3] = 4  (等价于 *(1 + p))
(-1)[p]   // arr[1] = 2  (等价于 *(-1 + p))
```

### 4.2 解引用与取地址的关系

**双向转换**：

```cpp
int n = 42;
int* pn = &n;    // 对象 → 指针
int& r = *pn;    // 指针 → 引用（左值）
int m = *pn;     // 指针 → 值（左值到右值转换）

int* pn2 = &*pn; // pn2 == pn（恒等变换）
```

**函数指针的特殊性**：

```cpp
int f() { return 42; }
int (*fp)() = &f;      // 函数地址
int (&fr)() = *fp;     // 函数引用
fr();                   // 等价于 f() 或 (*fp)()
```

### 4.3 成员访问的内存布局

**对象成员访问**：

```cpp
struct A {
    int i;
    double d;
};

A obj;
obj.i    // 编译时确定偏移量
obj.d    // 偏移量 = sizeof(int) + padding
```

内存布局：

```
A obj:
  +--------+-------+--------+
  | int i  | padding| double d |
  +--------+-------+--------+
  0        4       8        16
```

**指针成员访问的递归机制**：

```cpp
struct Wrapper {
    std::unique_ptr<std::string> ptr;
    std::unique_ptr<std::string>& operator->() { return ptr; }
};

Wrapper w;
w->size();  // 调用链：
            // 1. w.operator->() → unique_ptr<string>&
            // 2. unique_ptr<string>::operator->() → string*
            // 3. string*->size() → 实际调用
```

### 4.4 成员指针的实现

**数据成员指针**：

```cpp
struct S {
    int mi;
    double md;
};

int S::* pmi = &S::mi;  // 成员指针存储的是偏移量
S s{42, 3.14};
s.*pmi = 100;           // 编译为: *(int*)((char*)&s + offset_of(pmi)) = 100
```

**成员函数指针**：

```cpp
struct S {
    int f(int n) { return mi + n; }
    int mi;
};

int (S::* pf)(int) = &S::f;  // 存储的是函数入口地址
S s{7};
(s.*pf)(3);                  // 调用成员函数，传入 this = &s
```

### 4.5 CV 限定符传播规则

**成员访问的 CV 传播**：

```cpp
struct S {
    int mi;
    mutable int mj;
};

const S cs{1, 2};
cs.mi;     // const int&
cs.mj;     // int& (mutable 成员忽略 const)

volatile S vs{1, 2};
vs.mi;     // volatile int&
vs.mj;     // volatile int& (volatile 仍然传播)
```

**成员指针访问的 CV 规则**：

```cpp
int S::* pmi = &S::mi;
const S cs{7};
cs.*pmi;  // const int& (cv 限定符取并集)
```

## 5. 使用场景 (Use Cases)

### 5.1 下标操作符

**适用场景**：
- 数组元素访问
- 标准容器元素访问（vector, map, array 等）
- 多维数组访问
- 自定义容器的索引访问

**最佳实践**：

```cpp
// 数组访问
int arr[] = {1, 2, 3, 4, 5};
int value = arr[2];  // 推荐：清晰表达意图

// 容器访问
std::vector<int> vec = {10, 20, 30};
vec[0] = 100;        // 快速访问，不进行边界检查

std::map<std::string, int> scores;
scores["Alice"] = 95;  // 如果键不存在，会插入默认值

// 多维下标（C++23）
// std::mdspan<int, 3, 4> matrix;
// int elem = matrix[i, j];  // 新语法
```

**注意事项**：
- 内置数组的下标不进行边界检查
- `std::vector::operator[]` 也不检查边界，需要安全访问时使用 `at()`
- C++23 前避免在 `a[b, c]` 中使用未加括号的逗号表达式

**常见陷阱**：

```cpp
int arr[5] = {1, 2, 3, 4, 5};
int idx = 10;
arr[idx];  // 未定义行为：越界访问

int* p = arr;
p[10];     // 未定义行为：越界指针解引用

// C++20 弃用、C++23 禁止
int a[3][3];
a[1, 2];   // 曾被解释为 a[(1, 2)] = a[2]
           // C++23 起为错误或调用多维 operator[]
```

### 5.2 解引用与取地址

**适用场景**：
- 指针操作
- 智能指针访问
- 引用绑定
- 函数指针处理

**最佳实践**：

```cpp
// 智能指针
std::unique_ptr<int> uptr = std::make_unique<int>(42);
int value = *uptr;  // 解引用获取值

// 原始指针
int n = 10;
int* pn = &n;       // 取地址
int& ref = *pn;     // 解引用绑定引用

// 避免自定义 operator& 的干扰
struct Custom {
    void* operator&() { return nullptr; }
};

Custom obj;
Custom* ptr = std::addressof(obj);  // 使用 std::addressof 获取真实地址
```

**常见陷阱**：

```cpp
int* p = nullptr;
*p;  // 未定义行为：解引用空指针

int* p2;
*p2;  // 未定义行为：解引用未初始化指针

void* vp = &n;
*vp;  // 编译错误：不能解引用 void*
```

### 5.3 成员访问操作符

**适用场景**：
- 结构体/类成员访问
- 通过指针访问成员
- 智能指针成员访问
- 迭代器元素访问

**最佳实践**：

```cpp
struct Point {
    int x, y;

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }
};

// 对象成员访问
Point p{10, 20};
p.x = 15;          // 直接访问
p.move(5, 5);      // 成员函数调用

// 指针成员访问
Point* pp = &p;
pp->y = 25;        // 通过指针访问
pp->move(3, 3);    // 通过指针调用成员函数

// 智能指针
auto sp = std::make_shared<Point>(1, 2);
sp->x = 5;         // 智能指针支持 -> 操作符

// 迭代器
std::vector<Point> points = {{1, 2}, {3, 4}};
points[0].x = 10;  // 组合使用
```

**伪析构函数**：

```cpp
template<typename T>
void destroy(T* p) {
    p->~T();  // 对于标量类型，调用伪析构函数
              // 对于类类型，调用真实析构函数
}

int* pi = new int(42);
destroy(pi);  // pi->~int() 合法但无实际效果
```

### 5.4 成员指针访问

**适用场景**：
- 动态选择成员
- 回调机制
- 反射式编程
- 序列化/反序列化

**最佳实践**：

```cpp
struct Person {
    std::string name;
    int age;
    double salary;

    void print() { std::cout << name << ": " << age << std::endl; }
};

// 数据成员指针
int Person::* agePtr = &Person::age;
Person p{"Alice", 30, 50000.0};
p.*agePtr = 31;  // 通过成员指针修改

// 成员函数指针
void (Person::* printPtr)() = &Person::print;
(p.*printPtr)();  // 通过成员指针调用

// 在容器中批量操作
std::vector<Person> staff = { /* ... */ };
for (auto& emp : staff) {
    emp.*agePtr += 1;  // 每人年龄加 1
}
```

**类型安全的成员选择**：

```cpp
// 根据字段名选择成员
int Person::* selectMember(const std::string& name) {
    if (name == "age") return &Person::age;
    throw std::invalid_argument("Unknown member");
}

Person p{"Bob", 25, 30000.0};
auto member = selectMember("age");
p.*member = 26;
```

**常见陷阱**：

```cpp
int Person::* pmi = nullptr;
Person p;
p.*pmi;  // 未定义行为：空成员指针

struct Base { int i; };
struct Derived : Base { int j; };
int Base::* pbi = &Base::i;
Derived d;
d.*pbi;  // 正确：基类成员指针可用于派生类

int Derived::* pdi = &Derived::j;
Base b;
b.*pdi;  // 错误：派生类成员指针不能用于基类对象
```

### 5.5 操作符重载场景

**智能指针**：

```cpp
template<typename T>
class SmartPtr {
    T* ptr;
public:
    SmartPtr(T* p = nullptr) : ptr(p) {}

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    ~SmartPtr() { delete ptr; }
};

SmartPtr<std::string> sp(new std::string("hello"));
*sp = "world";      // 通过 operator* 修改
std::cout << sp->size();  // 通过 operator-> 访问成员
```

**自定义容器**：

```cpp
template<typename T, size_t N>
class FixedArray {
    T data[N];
public:
    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }

    T* operator&() { return data; }  // 慎用：可能破坏预期
};

FixedArray<int, 10> arr;
arr[5] = 42;
```

**迭代器**：

```cpp
template<typename T>
class ListIterator {
    Node<T>* current;
public:
    T& operator*() const { return current->data; }
    Node<T>* operator->() const { return current; }

    ListIterator& operator++() {
        current = current->next;
        return *this;
    }
};
```

## 6. 代码示例 (Examples)

### 6.1 下标操作符完整示例

```cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    // 数组下标访问
    int arr[4] = {1, 2, 3, 4};
    int* p = &arr[2];

    std::cout << "数组访问演示:" << std::endl;
    std::cout << "p[1] = " << p[1] << std::endl;     // 4
    std::cout << "p[-1] = " << p[-1] << std::endl;   // 2
    std::cout << "1[p] = " << 1[p] << std::endl;     // 4
    std::cout << "(-1)[p] = " << (-1)[p] << std::endl; // 2

    // 标准容器下标
    std::vector<int> vec = {10, 20, 30};
    vec[1] = 25;
    std::cout << "vec[1] = " << vec[1] << std::endl; // 25

    // map 的下标操作（会自动插入不存在的键）
    std::map<std::string, int> ages;
    ages["Alice"] = 30;  // 插入新键
    ages["Bob"] = 25;
    std::cout << "Alice's age: " << ages["Alice"] << std::endl;
    std::cout << "Charlie's age: " << ages["Charlie"] << std::endl; // 插入默认值 0

    // 初始化列表形式 (C++11)
    std::map<std::pair<int, int>, std::string> coords;
    coords[{1, 2}] = "Point A";
    coords[{3, 4}] = "Point B";
    std::cout << "({1,2}): " << coords[{1, 2}] << std::endl;

    return 0;
}
```

**输出**：

```
数组访问演示:
p[1] = 4
p[-1] = 2
1[p] = 4
(-1)[p] = 2
vec[1] = 25
Alice's age: 30
Charlie's age: 0
({1,2}): Point A
```

### 6.2 解引用与取地址示例

```cpp
#include <iostream>
#include <memory>

int global_func() { return 42; }

int main() {
    // 解引用操作
    int n = 1;
    int* pn = &n;

    int& r = *pn;       // lvalue 绑定到引用
    int m = *pn;        // 解引用 + 左值到右值转换

    std::cout << "n = " << n << ", *pn = " << *pn << std::endl;
    std::cout << "r = " << r << ", m = " << m << std::endl;

    // 取地址与恒等变换
    int* pn2 = &*pn;    // pn2 == pn
    std::cout << "pn == pn2: " << (pn == pn2 ? "true" : "false") << std::endl;

    // 函数指针
    int (*fp)() = &global_func;
    int (&fr)() = *fp;

    std::cout << "global_func() = " << global_func() << std::endl;
    std::cout << "fp() = " << fp() << std::endl;
    std::cout << "fr() = " << fr() << std::endl;

    // 智能指针解引用
    auto uptr = std::make_unique<int>(100);
    std::cout << "*uptr = " << *uptr << std::endl;

    // 处理自定义 operator&
    struct Custom {
        void* operator&() { return nullptr; }
        int value;
    };

    Custom c{42};
    Custom* bad_ptr = &c;           // 返回 nullptr（错误）
    Custom* good_ptr = std::addressof(c);  // 获取真实地址

    std::cout << "&c = " << bad_ptr << std::endl;
    std::cout << "std::addressof(c) = " << good_ptr << std::endl;

    return 0;
}
```

**输出**：

```
n = 1, *pn = 1
r = 1, m = 1
pn == pn2: true
global_func() = 42
fp() = 42
fr() = 42
*uptr = 100
&c = 0
std::addressof(c) = 0x7ffee... (实际地址)
```

### 6.3 成员访问操作符示例

```cpp
#include <iostream>
#include <memory>
#include <string>

struct Point {
    int x, y;

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    static int count;  // 静态成员
    static void printCount() { std::cout << "Count: " << count << std::endl; }

    enum Color { RED, GREEN, BLUE };
};

int Point::count = 0;

template<typename T>
struct SmartPointer {
    T* ptr;

    SmartPointer(T* p) : ptr(p) {}

    T* operator->() { return ptr; }
    T& operator*() { return *ptr; }
};

int main() {
    // 对象成员访问
    Point p{10, 20};
    p.x = 15;
    p.move(5, 5);
    std::cout << "p.x = " << p.x << ", p.y = " << p.y << std::endl;

    // 静态成员访问
    p.count = 100;  // 通过对象访问静态成员
    Point::count = 200;  // 通过类名访问
    p.printCount();

    // 枚举器访问
    Point p2{1, 1};
    std::cout << "RED = " << p2.RED << std::endl;  // 通过对象访问枚举器

    // 指针成员访问
    Point* pp = &p;
    pp->x = 20;
    pp->move(10, 10);
    std::cout << "pp->x = " << pp->x << std::endl;

    // 智能指针
    auto sp = std::make_shared<Point>(Point{5, 5});
    sp->x = 10;
    sp->move(2, 2);
    std::cout << "sp->x = " << sp->x << std::endl;

    // 自定义智能指针（递归调用 operator->）
    SmartPointer<Point> smp(new Point{100, 200});
    smp->x = 150;
    smp->move(10, 10);
    std::cout << "smp->x = " << smp->x << std::endl;

    delete smp.ptr;  // 清理（仅用于演示）

    // 伪析构函数
    int* pi = new int(42);
    pi->~int();  // 伪析构函数调用，无实际效果
    delete pi;

    return 0;
}
```

**输出**：

```
p.x = 20, p.y = 25
Count: 200
RED = 0
pp->x = 20
sp->x = 12
smp->x = 160
```

### 6.4 成员指针访问示例

```cpp
#include <iostream>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;
    mutable int access_count;  // mutable 成员

    void introduce() const {
        std::cout << "Hi, I'm " << name << ", " << age << " years old." << std::endl;
        ++access_count;
    }

    void birthday() { ++age; }

    void printInfo() & {  // ref-qualifier: 只能用于 lvalue
        std::cout << "Lvalue person: " << name << std::endl;
    }

    void printInfo() && {  // ref-qualifier: 只能用于 rvalue
        std::cout << "Rvalue person: " << name << std::endl;
    }
};

int main() {
    // 数据成员指针
    int Person::* age_ptr = &Person::age;
    std::string Person::* name_ptr = &Person::name;

    Person alice{"Alice", 30, 0};
    Person bob{"Bob", 25, 0};

    // 通过成员指针访问
    alice.*age_ptr = 31;
    std::cout << alice.name << "'s age: " << alice.*age_ptr << std::endl;

    // 批量操作
    std::vector<Person> people = {alice, bob};
    for (auto& p : people) {
        p.*age_ptr += 1;  // 每人年龄加 1
    }

    // 成员函数指针
    void (Person::* intro_ptr)() const = &Person::introduce;
    (alice.*intro_ptr)();  // 调用成员函数

    void (Person::* bd_ptr)() = &Person::birthday;
    (alice.*bd_ptr)();  // 生日，年龄加 1

    std::cout << "Alice's new age: " << alice.age << std::endl;

    // 指针的成员指针访问
    Person* p_bob = &bob;
    std::cout << "Bob's age: " << p_bob->*age_ptr << std::endl;
    (p_bob->*intro_ptr)();

    // mutable 成员的特殊行为
    const Person const_person{"Charlie", 40, 0};
    const_person.introduce();  // 可以修改 access_count
    std::cout << "Access count: " << const_person.access_count << std::endl;

    // const 对象的成员指针访问
    // const_person.*age_ptr = 41;  // 错误：const 对象不能修改非 mutable 成员
    int Person::* mutable_ptr = &Person::access_count;
    // const_person.*mutable_ptr = 1;  // 错误：成员指针不能用于修改 const 对象的 mutable 成员

    // ref-qualifier 示例 (C++11)
    alice.printInfo();  // 调用 lvalue 版本
    Person{"Temp", 0, 0}.printInfo();  // 调用 rvalue 版本

    // 成员函数指针与 ref-qualifier
    // void (Person::* print_lvalue)() & = &Person::printInfo;  // C++11 语法
    // (alice.*print_lvalue)();

    return 0;
}
```

**输出**：

```
Alice's age: 31
Hi, I'm Alice, 31 years old.
Alice's new age: 32
Bob's age: 25
Hi, I'm Bob, 26 years old.
Access count: 1
Lvalue person: Alice
Rvalue person: Temp
```

### 6.5 常见错误示例

```cpp
#include <iostream>
#include <memory>

struct Base { int i; };
struct Derived : Base { int j; };

int main() {
    // 错误 1: 空指针解引用
    int* p1 = nullptr;
    // *p1 = 42;  // 未定义行为

    // 错误 2: 越界数组访问
    int arr[5] = {1, 2, 3, 4, 5};
    // arr[10] = 100;  // 未定义行为

    // 错误 3: 空成员指针访问
    int Person::* pm = nullptr;
    // alice.*pm;  // 未定义行为

    // 错误 4: 类型不匹配的成员指针
    struct Person { int age; };
    int Person::* pmi = &Person::age;
    Person p{30};
    // int* pi = &(p.*pmi);  // 类型不匹配

    // 错误 5: reinterpet_cast 后的成员访问
    struct A { int i; };
    struct B { int j; };
    struct D : A, B {};

    D d;
    static_cast<B&>(d).j = 10;      // OK
    // reinterpret_cast<B&>(d).j = 20;  // 未定义行为

    // 错误 6: ref-qualifier 不匹配 (C++11)
    struct Test {
        void foo() & {}   // 只能用于 lvalue
        void bar() && {}  // 只能用于 rvalue
    };

    Test t;
    t.foo();  // OK
    // Test().foo();  // 错误：rvalue 不能调用 & 限定的成员函数

    // Test t2;
    // std::move(t2).bar();  // OK
    // t.bar();  // 错误：lvalue 不能调用 && 限定的成员函数

    // 错误 7: C++23 中的下标逗号表达式
    int a[3] = {1, 2, 3};
    // int x = a[1, 2];  // C++23 前等于 a[2]；C++23 起错误或调用多维 operator[]
    int y = a[(1, 2)];  // OK：a[2]

    std::cout << "y = " << y << std::endl;

    return 0;
}
```

### 6.6 自定义操作符重载示例

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

// 自定义数组类
template<typename T>
class Array {
    std::vector<T> data;
public:
    Array(std::initializer_list<T> init) : data(init) {}

    // 下标操作符（带边界检查）
    T& operator[](size_t index) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // 解引用操作符
    T& operator*() { return data.front(); }
    const T& operator*() const { return data.front(); }

    // 成员访问操作符
    T* operator->() { return &data.front(); }
    const T* operator->() const { return &data.front(); }

    size_t size() const { return data.size(); }
};

// 智能指针包装器
template<typename T>
class SmartWrapper {
    std::unique_ptr<T> ptr;
public:
    SmartWrapper(T* p = nullptr) : ptr(p) {}

    T* operator->() { return ptr.get(); }
    const T* operator->() const { return ptr.get(); }

    T& operator*() { return *ptr; }
    const T& operator*() const { return *ptr; }

    // 防止取地址（返回包装器本身的地址）
    SmartWrapper* operator&() { return this; }
    const SmartWrapper* operator&() const { return this; }

    T* get() { return ptr.get(); }
};

struct Point {
    int x, y;
    void print() const { std::cout << "(" << x << ", " << y << ")" << std::endl; }
};

int main() {
    // 自定义数组
    Array<int> arr = {10, 20, 30, 40, 50};

    std::cout << "Array access:" << std::endl;
    arr[2] = 25;
    std::cout << "arr[2] = " << arr[2] << std::endl;
    std::cout << "*arr = " << *arr << std::endl;
    std::cout << "arr->size() = " << arr->size() << std::endl;

    try {
        arr[10] = 100;  // 抛出异常
    } catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    // 智能指针包装器
    SmartWrapper<Point> sw(new Point{5, 10});

    std::cout << "\nSmartWrapper:" << std::endl;
    sw->x = 15;
    sw->y = 20;
    sw->print();

    Point& p = *sw;
    std::cout << "Point: (" << p.x << ", " << p.y << ")" << std::endl;

    // 自定义 operator& 的效果
    SmartWrapper<Point>* addr = &sw;  // 返回包装器地址
    std::cout << "Address of wrapper: " << addr << std::endl;

    // 获取原始指针
    Point* raw_ptr = sw.get();
    std::cout << "Raw pointer: " << raw_ptr << std::endl;

    return 0;
}
```

**输出**：

```
Array access:
arr[2] = 25
*arr = 10
arr->size() = 5
Exception: Index out of range

SmartWrapper:
(15, 20)
Point: (15, 20)
Address of wrapper: 0x7ffee... (包装器地址)
Raw pointer: 0x... (实际对象地址)
```

## 7. 总结 (Summary)

### 7.1 核心要点

| 操作符 | 语法 | 可重载 | 核心语义 | 典型用途 |
|--------|------|--------|----------|----------|
| 下标 | `a[b]` | 是 | `*(a + b)` | 数组/容器元素访问 |
| 解引用 | `*a` | 是 | 获取指针指向的对象 | 指针操作、迭代器 |
| 取地址 | `&a` | 是 | 获取对象地址 | 指针创建、函数指针 |
| 对象成员 | `a.b` | 否 | 访问对象成员 | 结构体/类成员访问 |
| 指针成员 | `a->b` | 是 | 访问指针指向对象的成员 | 指针、智能指针 |
| 对象成员指针 | `a.*b` | 否 | 通过成员指针访问对象成员 | 动态成员选择 |
| 指针成员指针 | `a->*b` | 是 | 通过成员指针访问指针指向对象的成员 | 动态成员选择 |

### 7.2 关键规则

1. **下标操作符**：
   - `a[b]` 等价于 `*(a + b)`
   - 支持逆向下标：`a[b]` 等价于 `b[a]`
   - C++23 起支持多维下标 `a[x, y, z]`

2. **成员访问**：
   - `a->b` 等价于 `(*a).b`
   - `operator->` 可递归调用直到返回原始指针
   - 静态成员访问时，对象表达式被弃值

3. **成员指针**：
   - 数据成员指针存储偏移量
   - 成员函数指针存储函数地址
   - 空成员指针的解引用为未定义行为

4. **CV 限定符传播**：
   - 成员访问结果的 CV 限定符是对象和成员的并集
   - `mutable` 成员忽略 `const` 限定符

### 7.3 技术对比

| 特性 | C 风格 | C++ 风格 |
|------|--------|----------|
| 数组访问 | `arr[i]` | `vec[i]` 或 `arr.at(i)` |
| 指针访问 | `ptr->member` | `smart_ptr->member` |
| 成员选择 | 编译时固定 | 可通过成员指针动态选择 |
| 边界检查 | 无 | 可选（`at()` vs `[]`） |
| 类型安全 | 弱 | 强（模板、重载） |

### 7.4 学习建议

1. **基础掌握**：
   - 理解内置操作符的语义和限制
   - 掌握 `.` 和 `->` 的区别与联系
   - 理解 `&` 和 `*` 的互补关系

2. **进阶实践**：
   - 实现自定义智能指针类，重载 `operator*` 和 `operator->`
   - 实现自定义容器类，重载 `operator[]`
   - 使用成员指针实现动态成员选择

3. **高级应用**：
   - 理解成员函数指针与回调机制
   - 实现序列化/反序列化框架
   - 设计类型安全的反射系统

4. **注意事项**：
   - 避免空指针和越界访问
   - 慎重重载 `operator&`（使用 `std::addressof` 作为后备）
   - 理解 C++23 对下标操作符的变更

5. **标准库学习**：
   - 研究标准容器（`vector`, `map`）的 `operator[]` 实现
   - 研究迭代器的 `operator*` 和 `operator->` 实现
   - 理解智能指针（`unique_ptr`, `shared_ptr`）的成员访问语义

### 7.5 标准库应用

成员访问操作符在标准库中被广泛使用：

| 标准库组件 | 重载的操作符 | 用途 |
|-----------|-------------|------|
| `std::vector` | `operator[]` | 元素访问 |
| `std::map` | `operator[]` | 元素访问/插入 |
| `std::unique_ptr` | `operator*`, `operator->` | 智能指针访问 |
| `std::shared_ptr` | `operator*`, `operator->` | 共享指针访问 |
| 迭代器 | `operator*`, `operator->` | 元素访问 |
| `std::bitset` | `operator[]` | 位访问 |
| `std::valarray` | `operator[]` | 数组访问 |

### 7.6 版本兼容性

| C++ 版本 | 新特性/变更 | 影响 |
|---------|------------|------|
| C++98 | 基础成员访问操作符 | 确立基础语义 |
| C++11 | ref-qualifier、xvalue | 成员函数可限定调用者值类别 |
| C++17 | 求值顺序规范 | 下标操作符求值顺序确定 |
| C++20 | 弃用下标中的逗号表达式 | `a[b,c]` 被弃用 |
| C++23 | 多维下标操作符 | `a[x,y,z]` 语法合法 |

### 参考资源

- C++ 标准文档：ISO/IEC 14882
- cppreference: Member access operators
- 《C++ Primer》（第 5 版）第 4、13、14 章
- 《Effective C++》条款 28：避免返回 handles 指向对象内部成分