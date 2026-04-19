# C/C++ 文档风格指南

## 术语规范

### 基础概念

| 中文 | 英文 | 说明 |
|------|------|------|
| 编译单元 | translation unit | 源文件编译单元 |
| 声明 | declaration | 引入名称 |
| 定义 | definition | 分配存储/实现 |
| 链接 | linkage | 名称可见性 |
| 内部链接 | internal linkage | 仅本编译单元可见 |
| 外部链接 | external linkage | 所有编译单元可见 |
| 未定义行为 | undefined behavior | UB 不确定行为 |
| 实现定义行为 | implementation-defined behavior | 编译器决定行为 |

### 类型系统

| 中文 | 英文 | 说明 |
|------|------|------|
| 基本类型 | fundamental type | int, char, double 等 |
| 用户定义类型 | user-defined type | class, struct, enum |
| 结构体 | struct | 数据结构类型 |
| 类 | class | 面向对象类型 |
| 枚举 | enum | 枚举类型 |
| 联合体 | union | 共用内存类型 |
| 类型别名 | type alias | using/typedef |
| 自动类型 | auto | 自动类型推导 |
| 空指针类型 | nullptr_t | nullptr 的类型 |

### 指针与引用

| 中文 | 英文 | 说明 |
|------|------|------|
| 指针 | pointer | 内存地址变量 |
| 引用 | reference | 别名 |
| 空指针 | null pointer | nullptr |
| 野指针 | dangling pointer | 无效指针 |
| 智能指针 | smart pointer | 自动内存管理指针 |
| 原始指针 | raw pointer | 传统指针 |
| 常量指针 | const pointer | 指向常量的指针 |
| 指针常量 | constant pointer | 不可变的指针 |

### 内存管理

| 中文 | 英文 | 说明 |
|------|------|------|
| 栈 | stack | 自动管理的内存区域 |
| 堆 | heap | 动态分配的内存区域 |
| RAII | RAII | 资源获取即初始化 |
| 构造函数 | constructor | 对象初始化函数 |
| 析构函数 | destructor | 对象销毁函数 |
| 拷贝构造 | copy constructor | 对象拷贝 |
| 移动构造 | move constructor | 对象移动 |
| 拷贝赋值 | copy assignment | 赋值运算符 |
| 移动赋值 | move assignment | 移动赋值运算符 |
| 右值引用 | rvalue reference | 移动语义基础 |
| 左值引用 | lvalue reference | 传统引用 |

### 智能指针

| 中文 | 英文 | 说明 |
|------|------|------|
| 独占指针 | unique_ptr | 独占所有权 |
| 共享指针 | shared_ptr | 共享所有权 |
| 弱引用指针 | weak_ptr | 弱引用 |
| 引用计数 | reference count | 共享指针计数 |

### 模板与泛型

| 中文 | 英文 | 说明 |
|------|------|------|
| 模板 | template | 泛型编程基础 |
| 函数模板 | function template | 泛型函数 |
| 类模板 | class template | 泛型类 |
| 模板特化 | template specialization | 特殊类型处理 |
| 可变参数模板 | variadic template | 可变数量参数 |
| 模板参数推导 | template argument deduction | CTAD |
| 概念 | concept | C++20 约束 |
| 约束 | constraint | 模板参数限制 |

### STL 容器

| 中文 | 英文 | 说明 |
|------|------|------|
| 容器 | container | STL 数据结构 |
| 向量 | vector | 动态数组 |
| 列表 | list | 双向链表 |
| 双端队列 | deque | 双端队列 |
| 数组 | array | 固定大小数组 |
| 集合 | set | 有序唯一元素集合 |
| 映射 | map | 有序键值对 |
| 无序集合 | unordered_set | 哈希集合 |
| 无序映射 | unordered_map | 哈希映射 |
| 栈 | stack | 后进先出栈 |
| 队列 | queue | 先进先出队列 |

### 迭代器与算法

| 中文 | 英文 | 说明 |
|------|------|------|
| 迭代器 | iterator | 遍历容器的对象 |
| 算法 | algorithm | STL 算法 |
| 范围 | range | C++20 范围库 |
| 视图 | view | 惰性求值范围 |
| 谓词 | predicate | 返回布尔值的函数 |
| 函数对象 | function object | 重载 () 的对象 |
| Lambda | lambda expression | 匿名函数 |

### 并发编程

| 中文 | 英文 | 说明 |
|------|------|------|
| 线程 | thread | 执行线程 |
| 互斥量 | mutex | 互斥同步原语 |
| 读写锁 | shared_mutex | 读写分离锁 |
| 条件变量 | condition_variable | 条件等待 |
| 原子操作 | atomic | 不可中断的操作 |
| 内存模型 | memory model | 多线程内存可见性 |
| 内存顺序 | memory order | 原子操作顺序 |
| 锁守卫 | lock_guard | RAII 锁管理 |
| 唯一锁 | unique_lock | 灵活锁管理 |
| 异步 | async | 异步任务 |
| Future | future | 异步结果 |
| Promise | promise | 异步承诺 |

### 现代 C++ 特性

| 中文 | 英文 | 说明 |
|------|------|------|
| 结构化绑定 | structured binding | C++17 多变量绑定 |
| 可选值 | optional | C++17 可能存在或不存在 |
| 变体 | variant | C++17 类型安全联合体 |
| 任意类型 | any | C++17 任意类型值 |
| 字符串视图 | string_view | C++17 字符串视图 |
| 范围 for | range-based for | C++11 范围遍历 |
| 初始化列表 | initializer_list | 统一初始化 |
| constexpr | constexpr | 编译期计算 |
| if constexpr | if constexpr | C++17 编译期条件 |
| span | span | C++20 数组视图 |

### 常用标准库

| 中文 | 英文 | 说明 |
|------|------|------|
| 字符串 | string | 字符串类 |
| 字符串流 | stringstream | 字符串流 |
| 文件流 | fstream | 文件流 |
| 输入输出 | iostream | 标准输入输出 |
| 智能指针头 | <memory> | 智能指针 |
| 容器头 | <vector>, <map> 等 | STL 容器 |
| 算法头 | <algorithm> | STL 算法 |
| 线程头 | <thread> | 多线程 |
| 原子头 | <atomic> | 原子操作 |
| 时间头 | <chrono> | 时间处理 |
| 正则 | <regex> | 正则表达式 |
| 文件系统 | <filesystem> | C++17 文件系统 |

## 代码示例规范

### 头文件包含

```cpp
// 标准库
#include <vector>
#include <algorithm>
#include <string>
#include <memory>

// 第三方库
#include <boost/asio.hpp>

// 项目头文件
#include "myproject/utils.h"
```

### 命名规范

```cpp
// 命名空间：小写+下划线
namespace my_project::utils {}

// 类型名：PascalCase
class UserManager {};
struct UserData {};

// 函数/变量：snake_case
void process_data();
int user_count = 0;

// 常量：全大写+下划线
constexpr int MAX_SIZE = 100;

// 模板参数：PascalCase
template<typename InputIterator>
void process(InputIterator first);
```

### 类定义规范

```cpp
class UserManager {
public:
    UserManager() = default;
    explicit UserManager(const std::string& name);

    // 禁止拷贝
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

    // 允许移动
    UserManager(UserManager&&) noexcept = default;

    void add_user(const User& user);
    const User* find_user(int id) const;

private:
    std::vector<std::unique_ptr<User>> users_;
    std::string name_;
};
```

### 模板定义

```cpp
// 函数模板
template<typename T>
T max(T a, T b) { return a > b ? a : b; }

// 类模板
template<typename T>
class Container {
public:
    void push(const T& value);
    void push(T&& value);
private:
    std::vector<T> data_;
};

// 概念约束（C++20）
template<std::integral T>
T add(T a, T b) { return a + b; }
```

## 版本标注

### C++11 特性

```cpp
auto x = 10;                           // auto
for (const auto& item : container) {}  // 范围 for
auto func = [](int x) { return x * 2; }; // lambda
std::unique_ptr<int> p = std::make_unique<int>(10); // 智能指针
constexpr int fib(int n) { return n <= 1 ? n : fib(n-1) + fib(n-2); }
```

### C++14 特性

```cpp
auto add = [](auto a, auto b) { return a + b; };  // 泛型 lambda
int binary = 0b1010;                               // 二进制字面量
int million = 1'000'000;                           // 数字分隔符
```

### C++17 特性

```cpp
auto [x, y] = getPoint();               // 结构化绑定
if (auto it = map.find(key); it != map.end()) {} // if 初始化
std::optional<int> find(int id);        // optional
if constexpr (std::is_integral_v<T>) {} // if constexpr
```

### C++20 特性

```cpp
template<typename T> concept Numeric = std::is_arithmetic_v<T>; // 概念
auto even = std::views::filter([](int x) { return x % 2 == 0; }); // 范围
void process(std::span<int> data);       // span
std::string s = std::format("Value: {}", 42); // format
```

## 内存管理

### 智能指针

```cpp
// unique_ptr：独占所有权
auto p1 = std::make_unique<int>(10);
std::unique_ptr<int> p2 = std::move(p1);

// shared_ptr：共享所有权
auto sp1 = std::make_shared<int>(10);
auto sp2 = sp1;

// weak_ptr：弱引用
std::weak_ptr<int> wp = sp1;
if (auto sp = wp.lock()) { /* 使用 sp */ }
```

### RAII 模式

```cpp
// ✅ 正确：使用 RAII 管理资源
class FileHandle {
public:
    explicit FileHandle(const std::string& path)
        : handle_(fopen(path.c_str(), "r")) {}
    ~FileHandle() { if (handle_) fclose(handle_); }
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
private:
    FILE* handle_;
};

// ❌ 错误：手动管理资源
void process() {
    FILE* f = fopen("test.txt", "r");
    // ... 如果这里抛出异常，资源泄露
    fclose(f);
}
```

## 异常处理

### 异常安全保证

```cpp
// 强保证：异常发生后状态不变
void strong_guarantee(std::vector<int>& v) {
    auto backup = v;
    try {
        v.push_back(42);
    } catch (...) {
        v = backup;
        throw;
    }
}
```

### 错误处理规范

```cpp
// ✅ 正确：使用具体异常
try {
    process(input);
} catch (const std::invalid_argument& e) {
    std::cerr << "Invalid argument: " << e.what() << std::endl;
} catch (const std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
}

// ❌ 错误：空 catch 块
try { doSomething(); } catch (...) {}  // 隐藏问题
```

## 并发编程

### 互斥量

```cpp
std::mutex mtx;

// lock_guard
{
    std::lock_guard<std::mutex> lock(mtx);
    // 临界区
}

// 避免死锁
std::mutex mtx1, mtx2;
std::lock(mtx1, mtx2);
std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
```

### 条件变量

```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// 生产者
void produce() {
    { std::lock_guard<std::mutex> lock(mtx); ready = true; }
    cv.notify_one();
}

// 消费者
void consume() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });
}
```

### 原子操作

```cpp
std::atomic<int> counter{0};
counter++;  // 原子递增
counter.fetch_add(10);
int old = counter.exchange(0);

// 比较交换
int expected = 10;
bool success = counter.compare_exchange_strong(expected, 20);
```

## 注意事项格式

```cpp
// ❌ 错误示例
// 说明错误原因

// ✅ 正确示例
// 说明正确做法
```

## 性能注意事项

| 场景 | 建议 |
|------|------|
| 大对象传递 | 使用 const 引用或移动语义 |
| 容器预分配 | 使用 `reserve()` |
| 避免不必要拷贝 | 使用 `std::move` 或 `const&` |
| 热路径 | 考虑 `inline` 和 `constexpr` |
| 并发优化 | 减少锁粒度，使用读写锁 |

## 工具与构建

```bash
# 编译选项
-std=c++20          # C++20 标准
-Wall -Wextra       # 警告级别
-O2                 # 生产优化
-g                  # 调试符号
-fsanitize=address  # 地址检测

# 构建系统
cmake -B build && cmake --build build
```