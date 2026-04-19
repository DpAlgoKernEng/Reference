# libstdc++ - GNU C++ 标准库

## 1. 概述与背景

### 1.1 工具定位

libstdc++ 是 GCC（GNU Compiler Collection）项目的 C++ 标准库实现，是 Linux 系统上默认的 C++ 标准库。它提供了完整的 C++ 标准库实现，包括 STL 容器、算法、字符串处理、IO 操作、多线程支持等核心功能。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2001 | GCC 3.0 | 完整 C++98 支持 |
| 2008 | GCC 4.3 | C++0x 初步支持 |
| 2013 | GCC 4.8 | 完整 C++11 支持 |
| 2015 | GCC 5.1 | 双 ABI 引入 |
| 2016 | GCC 6.1 | 完整 C++14 支持 |
| 2018 | GCC 8.1 | 完整 C++17 支持 |
| 2020 | GCC 10.1 | C++20 支持 |
| 2022 | GCC 12.1 | C++23 部分支持 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 开源许可 | GPL v3 |
| C++ 标准支持 | C++98 ~ C++23 |
| GCC 集成 | GCC 默认 C++ 标准库 |
| 跨平台 | Linux、Windows(MinGW)、macOS |
| 线程安全 | 符合 C++11 内存模型 |
| ABI 兼容 | 支持双 ABI 机制 |

### 1.4 适用场景

| 场景 | 推荐 |
|------|------|
| Linux 服务器开发 | 强烈推荐 |
| GCC 工具链项目 | 强烈推荐 |
| 跨平台兼容性项目 | 推荐 |
| macOS 开发 | 不推荐（使用 libc++） |
| 嵌入式 Linux | 推荐（或 uClibc++） |

### 1.5 对比分析

| 特性 | libstdc++ | libc++ |
|------|-----------|--------|
| 开发者 | GCC 团队 | LLVM 团队 |
| 许可证 | GPL v3 | Apache 2.0 + MIT |
| Linux 默认 | 是 | 否 |
| macOS 默认 | 否 | 是 |
| 代码体积 | 较大 | 较小 |
| C++20 支持 | 完整 | 完整 |
| ABI 稳定性 | 稳定 | 稳定 |
| 商业使用 | 需注意 GPL | 宽松 |

## 2. 安装与配置

### 2.1 多平台安装

**Linux（Debian/Ubuntu）：**

```bash
# 运行时库
sudo apt install libstdc++6

# 开发头文件
sudo apt install libstdc++-dev

# 查看安装版本
dpkg -l | grep libstdc++
```

**Linux（CentOS/RHEL）：**

```bash
# 运行时库
sudo yum install libstdc++

# 开发包
sudo yum install libstdc++-devel
```

**Windows（MinGW）：**

```bash
# MinGW-w64 自带 libstdc++
pacman -S mingw-w64-x86_64-gcc
```

### 2.2 版本管理

libstdc++ 版本与 GCC 版本绑定，升级 GCC 即升级 libstdc++：

```bash
# 查看 GCC 版本
g++ --version

# 查看 libstdc++ 库版本
strings /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep GLIBCXX | tail -5

# 查看支持的 C++ 标准
g++ -std=c++2a -dM -E - < /dev/null | grep CPLUSPLUS
```

### 2.3 环境配置

**编译链接选项：**

```bash
# 默认自动链接 libstdc++
g++ program.cpp -o program

# 显式指定链接
g++ program.cpp -lstdc++ -o program

# 静态链接 libstdc++
g++ -static-libstdc++ program.cpp -o program

# 完全静态链接
g++ -static program.cpp -o program
```

**CMake 配置：**

```cmake
cmake_minimum_required(VERSION 3.10)
project(myapp LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 使用标准模式

add_executable(myapp main.cpp)

# 显式链接 libstdc++（通常不需要）
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using libstdc++ with GCC")
endif()
```

### 2.4 验证安装

```cpp
#include <iostream>
#include <cxxabi.h>

int main() {
#ifdef __GLIBCXX__
    std::cout << "libstdc++ version: " << __GLIBCXX__ << '\n';
#endif
    std::cout << "GCC version: " << __GNUC__ << '.' 
              << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__ << '\n';
    return 0;
}
```

## 3. 基础使用

### 3.1 C++ 标准支持

| 标准 | 编译选项 | GCC 版本要求 |
|------|----------|-------------|
| C++98 | `-std=c++98` | 所有版本 |
| C++11 | `-std=c++11` | GCC 4.8+ |
| C++14 | `-std=c++14` | GCC 5+ |
| C++17 | `-std=c++17` | GCC 7+ |
| C++20 | `-std=c++20` | GCC 10+ |
| C++23 | `-std=c++23` | GCC 12+（部分） |

### 3.2 主要组件

| 组件 | 头文件 | 说明 |
|------|--------|------|
| 容器 | `<vector>`, `<map>`, `<unordered_map>` | STL 容器 |
| 算法 | `<algorithm>` | 排序、搜索等 |
| 字符串 | `<string>`, `<string_view>` | 字符串处理 |
| IO | `<iostream>`, `<fstream>`, `<sstream>` | 输入输出 |
| 线程 | `<thread>`, `<mutex>`, `<future>` | 多线程支持 |
| 智能指针 | `<memory>` | 资源管理 |
| 正则 | `<regex>` | 正则表达式 |
| 文件系统 | `<filesystem>` | C++17 文件系统 |

### 3.3 基本编译

```bash
# 默认使用 libstdc++
g++ program.cpp -o program

# 指定 C++ 标准
g++ -std=c++17 program.cpp -o program

# 启用所有警告
g++ -std=c++17 -Wall -Wextra -Wpedantic program.cpp -o program

# 优化编译
g++ -std=c++17 -O2 program.cpp -o program
```

### 3.4 常用操作示例

**容器与算法：**

```cpp
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 排序
    std::sort(v.begin(), v.end());
    
    // 查找
    auto it = std::find(v.begin(), v.end(), 5);
    
    // 累加
    int sum = std::accumulate(v.begin(), v.end(), 0);
    
    return 0;
}
```

## 4. 进阶特性

### 4.1 并发支持

**线程管理：**

```cpp
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int counter = 0;

void increment(int n) {
    for (int i = 0; i < n; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        ++counter;
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(increment, 1000);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
```

**原子操作：**

```cpp
#include <atomic>
#include <memory>

std::atomic<int> atomic_counter(0);
std::atomic_flag flag = ATOMIC_FLAG_INIT;

void safe_increment() {
    atomic_counter.fetch_add(1, std::memory_order_relaxed);
}
```

**条件变量：**

```cpp
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> data_queue;

void producer(int value) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        data_queue.push(value);
    }
    cv.notify_one();
}

void consumer() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return !data_queue.empty(); });
    int value = data_queue.front();
    data_queue.pop();
}
```

### 4.2 智能指针

```cpp
#include <memory>

class Resource {
public:
    Resource() { /* 初始化 */ }
    ~Resource() { /* 清理 */ }
};

void use_smart_pointers() {
    // unique_ptr - 独占所有权
    std::unique_ptr<Resource> p1 = std::make_unique<Resource>();
    
    // shared_ptr - 共享所有权
    std::shared_ptr<Resource> p2 = std::make_shared<Resource>();
    std::shared_ptr<Resource> p3 = p2;  // 引用计数增加
    
    // weak_ptr - 弱引用，不增加引用计数
    std::weak_ptr<Resource> wp = p2;
    if (auto sp = wp.lock()) {
        // 使用 sp
    }
}
```

### 4.3 GNU 扩展功能

```cpp
#include <ext/algorithm>      // 扩展算法
#include <ext/functional>     // 扩展函数对象
#include <ext/rope>            // 绳索数据结构

void use_extensions() {
    // 使用 __gnu_cxx 命名空间
    __gnu_cxx::rope<char> r;
    r += "Hello";
    r += " World";
    
    // rope 支持高效的插入和删除操作
    r.insert(5, ",");
}
```

## 5. 性能优化

### 5.1 编译优化策略

```bash
# 标准优化级别
g++ -O2 program.cpp                    # 生产环境推荐
g++ -O3 program.cpp                    # 激进优化
g++ -Os program.cpp                    # 优化体积

# 链接时优化（LTO）
g++ -O2 -flto program.cpp              # 启用 LTO

# 本地优化
g++ -O3 -march=native program.cpp      # 针对本地 CPU 优化

# 性能分析
g++ -pg program.cpp                    # 生成 gmon.out
gprof program gmon.out                 # 分析性能
```

### 5.2 小字符串优化（SSO）

```cpp
#include <string>

void sso_example() {
    // libstdc++ 的 std::string 实现 SSO
    // 短字符串（通常 <= 15 字符）存储在栈上
    std::string short_str = "hello";        // 栈上存储，无堆分配
    std::string long_str = "this is a very long string...";  // 堆分配
    
    // 性能建议：短字符串操作高效
}
```

### 5.3 自定义分配器

```cpp
#include <memory>
#include <vector>

template<typename T>
struct PoolAllocator {
    using value_type = T;
    
    T* allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    
    void deallocate(T* p, size_t) noexcept {
        ::operator delete(p);
    }
};

// 使用自定义分配器的容器
std::vector<int, PoolAllocator<int>> v;
```

## 6. 问题排查

### 6.1 常见问题

**链接错误：**

```bash
# 错误: undefined reference to `std::...'
# 解决方案：
g++ program.cpp -lstdc++ -o program
```

**版本不兼容：**

```bash
# 错误: version `GLIBCXX_3.4.21' not found
# 解决方案：
# 1. 检查当前 libstdc++ 版本
strings /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep GLIBCXX

# 2. 升级 GCC 或使用容器
sudo apt install g++-10
```

**ABI 不匹配：**

```bash
# 链接旧 ABI 库时使用兼容模式
g++ -D_GLIBCXX_USE_CXX11_ABI=0 main.cpp -loldlib
```

### 6.2 调试技巧

**启用调试模式：**

```bash
# 启用调试断言
g++ -D_GLIBCXX_DEBUG program.cpp -o program

# 启用严格调试
g++ -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC program.cpp
```

**迭代器调试：**

```cpp
#define _GLIBCXX_DEBUG

#include <vector>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3};
    auto it = v.begin();
    v.push_back(4);  // 使迭代器失效
    // 调试模式下会检测到错误
    std::cout << *it << '\n';  // 触发断言
}
```

**禁用特性：**

```bash
# 禁用异常（减小体积）
g++ -fno-exceptions program.cpp -o program

# 禁用 RTTI（减小体积）
g++ -fno-rtti program.cpp -o program
```

## 7. 集成实践

### 7.1 ABI 兼容性管理

**双 ABI 机制（GCC 5+）：**

```cpp
// 使用旧 ABI（兼容旧代码）
#define _GLIBCXX_USE_CXX11_ABI 0
std::string s;  // std::basic_string<char>

// 使用新 ABI（默认）
#define _GLIBCXX_USE_CXX11_ABI 1
std::string s;  // std::__cxx11::basic_string<char>
```

**符号版本查看：**

```bash
# 查看符号版本
nm -D /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep _Z

# 查看导出版本
readelf -sV /usr/lib/x86_64-linux-gnu/libstdc++.so.6
```

### 7.2 构建系统集成

**Makefile 集成：**

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lstdc++

SRCS = main.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = myapp

$(TARGET): $(OBJS)
    $(CXX) $(OBJS) $(LDFLAGS) -o $@

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(TARGET)
```

**Meson 构建：**

```meson
project('myapp', 'cpp',
  default_options: ['cpp_std=c++17'])

executable('myapp', 'main.cpp')
```

### 7.3 实战案例

**高性能网络服务：**

```cpp
#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

class NetworkServer {
    std::atomic<bool> running{false};
    std::vector<std::thread> workers;
    std::mutex log_mutex;
    
public:
    void start(int thread_count) {
        running = true;
        for (int i = 0; i < thread_count; ++i) {
            workers.emplace_back(&NetworkServer::worker_loop, this);
        }
    }
    
    void stop() {
        running = false;
        for (auto& t : workers) {
            if (t.joinable()) t.join();
        }
    }
    
private:
    void worker_loop() {
        while (running) {
            // 处理请求
            {
                std::lock_guard<std::mutex> lock(log_mutex);
                // 日志记录
            }
        }
    }
};
```

## 8. 参考资源

### 8.1 官方文档

- libstdc++ 文档: https://gcc.gnu.org/onlinedocs/libstdc++/
- GCC 手册: https://gcc.gnu.org/onlinedocs/gcc/
- C++ 标准支持状态: https://gcc.gnu.org/projects/cxx-status.html

### 8.2 学习路径

1. **基础阶段**：掌握 STL 容器和算法使用
2. **进阶阶段**：学习智能指针、多线程编程
3. **高级阶段**：理解 ABI 兼容性、自定义分配器
4. **专家阶段**：源码分析、性能调优、扩展开发

### 8.3 常用工具

| 工具 | 用途 |
|------|------|
| `nm` | 查看符号表 |
| `strings` | 查看字符串常量 |
| `ldd` | 查看动态库依赖 |
| `objdump` | 反汇编分析 |
| `readelf` | ELF 文件分析 |