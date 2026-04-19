# C++标准库头文件（C++ Standard Library Headers）

来源：cppreference.com

## C++标准库头文件分类

**C++标准库**的接口由以下头文件集合定义，按功能分类组织：

### 一、语言支持库（Language Support Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<cfloat>` | C++98 | 浮点类型限制 |
| `<climits>` | C++98 | 整数类型限制 |
| `<compare>` | C++20 | 三路比较运算符支持 |
| `<coroutine>` | C++20 | 协程支持库 |
| `<csetjmp>` | C++98 | 非局部跳转 |
| `<csignal>` | C++98 | 信号管理 |
| `<cstdarg>` | C++98 | 可变参数处理 |
| `<cstddef>` | C++98 | 标准宏和类型定义 |
| `<cstdint>` | C++11 | 固定宽度整数类型 |
| `<cstdlib>` | C++98 | 通用工具函数 |
| `<exception>` | C++98 | 异常处理工具 |
| `<initializer_list>` | C++11 | 初始化列表类模板 |
| `<limits>` | C++98 | 算术类型属性查询 |
| `<new>` | C++98 | 低级内存管理 |
| `<source_location>` | C++20 | 源代码位置信息 |
| `<stdfloat>` | C++23 | 固定宽度浮点类型 |
| `<typeinfo>` | C++98 | 运行时类型信息 |
| `<version>` | C++20 | 库实现状态验证宏 |

### 二、诊断库（Diagnostics Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<cassert>` | C++98 | 调试断言 |
| `<cerrno>` | C++98 | 错误码 |
| `<debugging>` | C++26 | 调试库 |
| `<stacktrace>` | C++23 | 栈跟踪库 |
| `<stdexcept>` | C++98 | 标准异常类型 |
| `<system_error>` | C++11 | 系统错误码 |

### 三、内存管理库（Memory Management Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<memory>` | C++98 | 高级内存管理 |
| `<memory_resource>` | C++17 | 多态分配器和内存资源 |
| `<scoped_allocator>` | C++11 | 嵌套分配器类 |

### 四、元编程库（Metaprogramming Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<type_traits>` | C++11 | 编译时类型信息 |
| `<ratio>` | C++11 | 编译时有理数运算 |

### 五、通用工具库（General Utilities Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<any>` | C++17 | 任意类型容器 |
| `<bitset>` | C++98 | 位集类 |
| `<bit>` | C++20 | 位操作函数 |
| `<charconv>` | C++17 | 字符转换 |
| `<expected>` | C++23 | 期望值容器 |
| `<format>` | C++20 | 格式化库 |
| `<functional>` | C++98 | 函数对象和绑定操作 |
| `<optional>` | C++17 | 可选值容器 |
| `<tuple>` | C++11 | 元组类 |
| `<typeindex>` | C++11 | 类型索引 |
| `<utility>` | C++98 | 各种工具组件 |
| `<variant>` | C++17 | 变体容器 |

### 六、容器库（Containers Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<array>` | C++11 | 固定大小数组 |
| `<deque>` | C++98 | 双端队列 |
| `<flat_map>` | C++23 | 扁平映射容器 |
| `<flat_set>` | C++23 | 扁平集合容器 |
| `<forward_list>` | C++11 | 单向链表 |
| `<inplace_vector>` | C++26 | 就地向量容器 |
| `<list>` | C++98 | 双向链表 |
| `<map>` | C++98 | 映射容器 |
| `<mdspan>` | C++23 | 多维视图 |
| `<queue>` | C++98 | 队列适配器 |
| `<set>` | C++98 | 集合容器 |
| `<span>` | C++20 | 跨越视图 |
| `<stack>` | C++98 | 栈适配器 |
| `<unordered_map>` | C++11 | 无序映射容器 |
| `<unordered_set>` | C++11 | 无序集合容器 |
| `<vector>` | C++98 | 动态数组 |

### 七、迭代器和范围库

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<iterator>` | C++98 | 迭代器 |
| `<generator>` | C++23 | 生成器类 |
| `<ranges>` | C++20 | 范围库 |

### 八、算法库（Algorithms Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<algorithm>` | C++98 | 范围算法 |
| `<numeric>` | C++98 | 数值算法 |

### 九、字符串库（Strings Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<cctype>` | C++98 | 字符分类 |
| `<cstring>` | C++98 | 字符串处理 |
| `<cuchar>` | C++11 | Unicode字符转换 |
| `<cwchar>` | C++98 | 宽字符处理 |
| `<cwctype>` | C++98 | 宽字符分类 |
| `<string_view>` | C++17 | 字符串视图 |
| `<string>` | C++98 | 字符串类 |

### 十、文本处理库（Text Processing Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<clocale>` | C++98 | 本地化 |
| `<codecvt>` | C++11* | 字符编码转换 |
| `<locale>` | C++98 | 本地化工具 |
| `<regex>` | C++11 | 正则表达式 |
| `<text_encoding>` | C++26 | 文本编码标识 |

### 十一、数值库（Numerics Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<cfenv>` | C++11 | 浮点环境 |
| `<cmath>` | C++98 | 数学函数 |
| `<complex>` | C++98 | 复数类型 |
| `<linalg>` | C++26 | 线性代数 |
| `<numbers>` | C++20 | 数学常数 |
| `<random>` | C++11 | 随机数 |
| `<simd>` | C++26 | SIMD操作 |
| `<valarray>` | C++98 | 数值数组 |

### 十二、时间库（Time Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<chrono>` | C++11 | 时间工具 |
| `<ctime>` | C++98 | C风格时间函数 |

### 十三、输入输出库（Input/Output Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<cinttypes>` | C++11 | 格式化宏 |
| `<cstdio>` | C++98 | C风格I/O |
| `<filesystem>` | C++17 | 文件系统 |
| `<fstream>` | C++98 | 文件流 |
| `<iomanip>` | C++98 | I/O格式控制 |
| `<iosfwd>` | C++98 | I/O前向声明 |
| `<iostream>` | C++98 | 标准流对象 |
| `<ios>` | C++98 | I/O基础类 |
| `<istream>` | C++98 | 输入流 |
| `<ostream>` | C++98 | 输出流 |
| `<print>` | C++23 | 打印库 |
| `<spanstream>` | C++23 | 跨越流 |
| `<sstream>` | C++98 | 字符串流 |
| `<streambuf>` | C++98 | 流缓冲区 |
| `<strstream>` | C++98* | 字符串流 |
| `<syncstream>` | C++20 | 同步流 |

### 十四、并发支持库（Concurrency Support Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<atomic>` | C++11 | 原子操作 |
| `<barrier>` | C++20 | 屏障 |
| `<condition_variable>` | C++11 | 条件变量 |
| `<future>` | C++11 | 异步计算 |
| `<hazard_pointer>` | C++26 | 危险指针 |
| `<latch>` | C++20 | 门闩 |
| `<mutex>` | C++11 | 互斥量 |
| `<rcu>` | C++26 | 读-拷贝-更新 |
| `<semaphore>` | C++20 | 信号量 |
| `<shared_mutex>` | C++14 | 共享互斥量 |
| `<stdatomic.h>` | C++23 | C风格原子头文件 |
| `<stop_token>` | C++20 | 停止令牌 |
| `<thread>` | C++11 | 线程类 |

### 十五、执行支持库（Execution Support Library）

| 头文件 | 标准 | 功能 |
|--------|------|------|
| `<execution>` | C++17 | 执行策略 |

---

## C兼容性头文件

C++标准库包含C标准库的兼容性头文件两种形式：

### 1. C风格头文件（xxx.h）
- 用于C/C++互操作性
- 将名称放在全局命名空间
- C++23中重新启用

### 2. C++风格头文件（cxxx）
- 将名称放在`std`命名空间
- 推荐在纯C++代码中使用

### 对应关系示例：

| C风格头文件 | C++风格头文件 |
|------------|---------------|
| `<assert.h>` | `<cassert>` |
| `<ctype.h>` | `<cctype>` |
| `<errno.h>` | `<cerrno>` |
| `<float.h>` | `<cfloat>` |
| `<limits.h>` | `<climits>` |
| `<math.h>` | `<cmath>` |
| `<stdio.h>` | `<cstdio>` |
| `<stdlib.h>` | `<cstdlib>` |
| `<string.h>` | `<cstring>` |

### 特殊C兼容性头文件

#### 1. 有效但特殊的头文件
```cpp
// <stdatomic.h> (C++23)
#include <stdatomic.h>  // C风格原子操作
_Atomic(int) counter = ATOMIC_VAR_INIT(0);
```

#### 2. 空头文件（已弃用或无意义）
```cpp
// 这些头文件已被弃用或在C++中无意义
#include <ccomplex>      // C++17弃用，C++20移除
#include <ciso646>       // C++20移除
#include <cstdalign>     // C++17弃用，C++20移除
```

---

## 扩展知识详解

### 一、现代C++特性演进

#### 1. C++11核心特性
```cpp
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>

void modern_cpp11() {
    // 智能指针
    auto ptr = std::make_unique<int>(42);
    
    // 线程支持
    std::thread t([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    t.join();
    
    // 原子操作
    std::atomic<int> counter{0};
    counter.fetch_add(1);
}
```

#### 2. C++17现代化特性
```cpp
#include <optional>
#include <variant>
#include <filesystem>
#include <any>

void modern_cpp17() {
    // 可选值
    std::optional<int> opt = 42;
    if (opt.has_value()) {
        std::cout << *opt << std::endl;
    }
    
    // 变体
    std::variant<int, std::string> var = 42;
    var = "Hello";
    
    // 文件系统
    std::filesystem::path p = "/home/user";
    if (std::filesystem::exists(p)) {
        std::cout << "Path exists\n";
    }
}
```

#### 3. C++20前沿特性
```cpp
#include <ranges>
#include <format>
#include <span>
#include <bit>

void modern_cpp20() {
    // 范围库
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto even = vec | std::views::filter([](int n) { return n % 2 == 0; });
    
    // 格式化库
    std::string msg = std::format("Value: {}", 42);
    
    // 跨越视图
    std::span<int> sp(vec);
    
    // 位操作
    bool is_pow2 = std::has_single_bit(8u);
}
```

#### 4. C++23新特性
```cpp
#include <print>
#include <expected>
#include <stacktrace>

void modern_cpp23() {
    // 打印库
    std::print("Hello, {}!\n", "World");
    
    // 期望值
    std::expected<int, std::string> result = 42;
    if (result) {
        std::cout << *result << std::endl;
    }
    
    // 栈跟踪
    std::cout << std::stacktrace::current() << std::endl;
}
```

### 二、头文件使用最佳实践

#### 1. 最小包含原则
```cpp
// 只包含需要的头文件
#include <vector>        // 需要std::vector
#include <algorithm>     // 需要std::sort
#include <iostream>      // 需要std::cout

// 避免包含不必要的头文件
// #include <string>     // 如果不需要就不包含
```

#### 2. 前向声明优化
```cpp
// 头文件中使用前向声明
class MyClass;           // 前向声明
void process(const MyClass& obj);

// 实现文件中包含完整定义
#include "MyClass.h"
void process(const MyClass& obj) {
    // 实现代码
}
```

#### 3. C++风格头文件优先
```cpp
// 推荐：使用C++风格头文件
#include <cstdio>        // 而不是<stdio.h>
#include <cstdlib>       // 而不是<stdlib.h>

int main() {
    std::printf("Hello, World!\n");  // 使用std::命名空间
    return 0;
}
```

### 三、各组件详细说明

#### 1. 智能指针（C++11）
```cpp
#include <memory>

void smart_pointer_example() {
    // 独占指针
    auto unique = std::make_unique<int>(42);
    
    // 共享指针
    auto shared = std::make_shared<std::string>("Hello");
    
    // 弱指针
    std::weak_ptr<std::string> weak = shared;
    
    // 自定义删除器
    auto custom_deleter = std::unique_ptr<FILE, decltype(&fclose)>(
        fopen("file.txt", "r"), &fclose);
}
```

#### 2. 并发支持（C++11/14/20）
```cpp
#include <thread>
#include <mutex>
#include <future>
#include <atomic>

void concurrency_example() {
    // 线程
    std::thread t([]() { /* 工作 */ });
    
    // 互斥量
    std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    
    // 异步任务
    auto future = std::async(std::launch::async, []() {
        return 42;
    });
    int result = future.get();
    
    // 原子操作
    std::atomic<int> atomic_var{0};
}
```

#### 3. 容器现代化（C++11/17/20）
```cpp
#include <vector>
#include <unordered_map>
#include <optional>
#include <span>

void container_example() {
    // 初始化列表
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 结构化绑定（C++17）
    std::unordered_map<std::string, int> map = {{"key", 42}};
    for (const auto& [key, value] : map) {
        std::cout << key << ": " << value << std::endl;
    }
    
    // 跨越视图（C++20）
    std::span<const int> sp(vec);
    for (int value : sp) {
        std::cout << value << " ";
    }
}
```

#### 4. 格式化和I/O（C++20/23）
```cpp
#include <format>
#include <print>

void formatting_example() {
    // 格式化库（C++20）
    std::string msg = std::format("Hello, {}! Value: {}", "World", 42);
    
    // 打印库（C++23）
    std::print("Direct print: {}\n", 42);
    
    // 范围格式化
    std::vector<int> vec = {1, 2, 3};
    std::print("Vector: {::d}\n", vec);
}
```

### 四、库设计哲学

#### 1. 零开销抽象
```cpp
// 模板提供零开销抽象
template<typename T>
void process_elements(const std::vector<T>& vec) {
    // 编译时优化，无运行时开销
    for (const auto& element : vec) {
        process(element);
    }
}
```

#### 2. RAII原则
```cpp
// 资源获取即初始化
class FileHandler {
private:
    std::FILE* file;
    
public:
    FileHandler(const char* filename) 
        : file(std::fopen(filename, "r")) {
        if (!file) throw std::runtime_error("Failed to open file");
    }
    
    ~FileHandler() {
        if (file) std::fclose(file);
    }
    
    // 禁止拷贝，允许移动
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
    FileHandler(FileHandler&& other) noexcept : file(other.file) {
        other.file = nullptr;
    }
};
```

#### 3. 类型安全
```cpp
// 使用强类型替代原始类型
enum class Color { Red, Green, Blue };
enum class Status { Active, Inactive };

void process_color(Color c) {
    // 编译时类型检查
    switch (c) {
        case Color::Red:   /* 处理红色 */ break;
        case Color::Green: /* 处理绿色 */ break;
        case Color::Blue:  /* 处理蓝色 */ break;
    }
}
```

---

## 实际应用示例

### 一、现代文件处理系统
```cpp
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <format>

class FileManager {
public:
    static bool create_directory(const std::string& path) {
        return std::filesystem::create_directories(path);
    }
    
    static std::vector<std::string> list_files(const std::string& path) {
        std::vector<std::string> files;
        if (std::filesystem::exists(path)) {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().filename().string());
                }
            }
        }
        return files;
    }
    
    static std::string read_file(const std::string& filename) {
        std::ifstream file(filename);
        return std::string(std::istreambuf_iterator<char>(file),
                          std::istreambuf_iterator<char>());
    }
};
```

### 二、并发数据处理
```cpp
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex mtx;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    
public:
    void push(T item) {
        std::lock_guard<std::mutex> lock(mtx);
        data_queue.push(item);
        data_cond.notify_one();
    }
    
    bool try_pop(T& item) {
        std::lock_guard<std::mutex> lock(mtx);
        if (data_queue.empty()) return false;
        item = data_queue.front();
        data_queue.pop();
        return true;
    }
    
    void wait_and_pop(T& item) {
        std::unique_lock<std::mutex> lock(mtx);
        data_cond.wait(lock, [this] { return !data_queue.empty(); });
        item = data_queue.front();
        data_queue.pop();
    }
};
```

### 三、现代网络客户端
```cpp
#include <string>
#include <vector>
#include <optional>
#include <expected>
#include <format>

class HttpClient {
public:
    struct HttpResponse {
        int status_code;
        std::string body;
        std::vector<std::pair<std::string, std::string>> headers;
    };
    
    std::expected<HttpResponse, std::string> get(const std::string& url) {
        // 简化的HTTP请求实现
        try {
            // 实际HTTP请求逻辑
            HttpResponse response;
            response.status_code = 200;
            response.body = std::format("Response from {}", url);
            return response;
        } catch (const std::exception& e) {
            return std::unexpected(std::string(e.what()));
        }
    }
};
```

---

## 标准参考文献

### 相关C++标准章节：

- **C++23标准**（ISO/IEC 14882:2024）
  - 各头文件规范
- **C++20标准**（ISO/IEC 14882:2020）
  - 新增范围库、协程等
- **C++17标准**（ISO/IEC 14882:2017）
  - 文件系统、并行算法等
- **C++14标准**（ISO/IEC 14882:2014）
  - 共享互斥量等改进
- **C++11标准**（ISO/IEC 14882:2011）
  - 智能指针、线程库等

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C标准库头文件文档] | C语言对应文档 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/header&oldid=177958>
- 最后修改时间：2024年11月25日 12:12
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++标准库头文件提供了丰富的功能集合，从基础的语言支持到现代的并发和范围库。理解各头文件的用途和相互关系，对于编写高效、现代的C++程序至关重要。C++不断演进，每个新标准都新增了重要的库组件来满足现代编程需求。正确使用C++风格头文件和现代库特性，能够提高代码的安全性、可读性和性能。从传统的STL容器到现代的范围库和格式化库，C++标准库体现了零开销抽象和类型安全的设计哲学。掌握这些库组件的最佳实践，是成为优秀C++开发者的基础。C/C++兼容性头文件为混合编程提供了便利，但应优先使用纯C++风格的接口。随着C++23和未来标准的推出，更多现代化特性将不断完善C++生态系统。