# libc++ - LLVM C++ 标准库

## 1. 概述与背景

### 1.1 工具定位

libc++ 是 LLVM 项目官方实现的 C++ 标准库，由 Apple 和 LLVM 社区联合开发。它旨在提供完整、高性能、可移植的 C++ 标准库实现，特别强调代码体积优化和现代 C++ 标准支持。

### 1.2 发展历史

| 年份 | 版本 | 里程碑事件 |
|------|------|-----------|
| 2010 | 初始发布 | Apple 启动 libc++ 项目 |
| 2011 | C++11 | 完整支持 C++11 标准 |
| 2013 | Clang 默认 | macOS 将 libc++ 设为默认标准库 |
| 2017 | C++17 | 支持 C++17 标准特性 |
| 2020 | C++20 | 大部分 C++20 特性支持 |
| 2023 | C++23 | 部分 C++23 特性实验性支持 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 开源许可 | Apache 2.0 + LLVM Exception（允许商业闭源使用） |
| C++ 标准支持 | C++11 ~ C++23 |
| 性能优化 | 专注于代码大小和运行速度的平衡优化 |
| 跨平台 | Linux、macOS、Windows、FreeBSD、Android |
| 异常支持 | 可配置禁用（适合嵌入式和游戏开发） |
| RTTI 支持 | 可配置禁用（减少代码体积） |
| ABI 稳定 | 提供稳定的二进制接口 |

### 1.4 适用场景

| 场景 | 推荐理由 |
|------|----------|
| macOS/iOS 开发 | 系统默认标准库，完美集成 |
| 嵌入式开发 | 代码体积小，可禁用异常和 RTTI |
| 游戏开发 | 性能优化，可裁剪特性 |
| 跨平台项目 | 与 Clang 紧密集成，一致性高 |
| 开源商业项目 | Apache 2.0 许可证友好 |
| LLVM 生态开发 | 与其他 LLVM 工具无缝集成 |

### 1.5 对比分析

#### libc++ vs libstdc++ vs MSVC STL

| 特性 | libc++ | libstdc++ | MSVC STL |
|------|--------|-----------|----------|
| 开发者 | LLVM | GCC | Microsoft |
| 许可证 | Apache 2.0 | GPL v3 | 专有 |
| 代码体积 | 较小 | 中等 | 较大 |
| 编译速度 | 快 | 中等 | 中等 |
| macOS 默认 | ✅ | ❌ | ❌ |
| Linux 默认 | ❌ | ✅ | ❌ |
| Windows 默认 | ❌ | ❌ | ✅ |
| C++20 支持 | 完整 | 完整 | 完整 |
| C++23 支持 | 部分 | 部分 | 部分 |
| 异常禁用 | 完全支持 | 支持 | 部分支持 |
| 模块支持 | 实验性 | 实验性 | 实验性 |
| 二进制体积 | ~1-2MB | ~2-3MB | ~3-4MB |

## 2. 安装与配置

### 2.1 多平台安装

#### macOS 安装

macOS 上 libc++ 是默认 C++ 标准库，无需额外安装：

```bash
# 系统自带，Xcode Command Line Tools 包含
xcode-select --install

# 查看版本
clang++ --version
# Apple clang version 15.0.0
# Target: arm64-apple-darwin23.0.0

# 查看 libc++ 版本
clang++ -std=c++20 -xc++ - -E -dM < /dev/null | grep _LIBCPP_VERSION
# #define _LIBCPP_VERSION 15000
```

#### Linux 安装

**Debian/Ubuntu 系列：**

```bash
# 安装开发包
sudo apt update
sudo apt install -y libc++-dev libc++abi-dev

# 安装文档（可选）
sudo apt install -y libc++-dev-doc

# 验证安装
apt list --installed | grep libc++
```

**Fedora/RHEL 系列：**

```bash
# 安装开发包
sudo dnf install -y libcxx-devel libcxxabi-devel

# 验证安装
rpm -qa | grep libcxx
```

**Arch Linux：**

```bash
# 安装
sudo pacman -S libc++

# 验证
pacman -Qs libc++
```

**源码编译安装：**

```bash
# 克隆源码
git clone https://github.com/llvm/llvm-project.git
cd llvm-project

# 配置构建
cmake -B build -S runtimes \
    -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DLIBCXX_ENABLE_ASSERTIONS=OFF \
    -DLIBCXX_ENABLE_SHARED=ON \
    -DLIBCXX_ENABLE_STATIC=ON

# 编译（使用所有核心）
cmake --build build -j$(nproc)

# 安装
sudo cmake --install build

# 验证
clang++ -stdlib=libc++ -xc++ - -E -dM < /dev/null | grep _LIBCPP_VERSION
```

#### Windows 安装

**使用 Visual Studio：**

```powershell
# Visual Studio 安装时勾选 "C++ Clang tools for Windows"
# 包含 libc++ 支持

# 验证
clang-cl /?
```

**使用 vcpkg：**

```bash
# 安装 vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# 安装 libc++
.\vcpkg install libcxx:x64-windows

# 集成到项目
.\vcpkg integrate install
```

### 2.2 版本管理

```bash
# 查看当前 libc++ 版本
echo '#include <version>' | clang++ -stdlib=libc++ -xc++ -E -dM - | grep _LIBCPP_VERSION

# 版本号含义
# _LIBCPP_VERSION = 15000 表示 15.0.0

# 查看支持的 C++ 标准
clang++ -stdlib=libc++ -std=c++23 -xc++ - - <<< 'int main(){}'
```

### 2.3 环境配置

**环境变量设置：**

```bash
# 设置默认使用 libc++
export CXX=clang++
export CXXFLAGS="-stdlib=libc++"

# 添加到 ~/.bashrc 或 ~/.zshrc
echo 'export CXX=clang++' >> ~/.bashrc
echo 'export CXXFLAGS="-stdlib=libc++"' >> ~/.bashrc
source ~/.bashrc
```

**CMake 工具链文件：**

```cmake
# toolchain-libcxx.cmake
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# 设置 libc++ 头文件路径（如果非标准位置）
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    /usr/include/c++/v1
    /usr/include
)

# 链接库
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++ -lc++abi")
```

### 2.4 验证安装

**完整验证脚本：**

```cpp
// test_libcxx.cpp
#include <iostream>
#include <version>
#include <vector>
#include <format>

int main() {
    std::cout << "=== libc++ 验证 ===" << std::endl;
    
#ifdef _LIBCPP_VERSION
    std::cout << "libc++ 版本: " << _LIBCPP_VERSION << std::endl;
#else
    std::cout << "未使用 libc++" << std::endl;
    return 1;
#endif

#ifdef __cpp_lib_format
    std::cout << "C++20 format 支持: 是" << std::endl;
#else
    std::cout << "C++20 format 支持: 否" << std::endl;
#endif

#ifdef __cpp_lib_span
    std::cout << "C++20 span 支持: 是" << std::endl;
#endif

    // 测试 std::format
    std::string msg = std::format("测试成功！版本 {}", _LIBCPP_VERSION);
    std::cout << msg << std::endl;
    
    return 0;
}
```

```bash
# 编译运行
clang++ -stdlib=libc++ -std=c++20 test_libcxx.cpp -o test_libcxx
./test_libcxx
```

## 3. 基础使用

### 3.1 快速入门

**第一个 libc++ 程序：**

```cpp
// hello.cpp
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> messages = {
        "Hello",
        "from",
        "libc++"
    };
    
    for (const auto& msg : messages) {
        std::cout << msg << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

```bash
# 编译
clang++ -stdlib=libc++ -std=c++17 hello.cpp -o hello

# 运行
./hello
# 输出: Hello from libc++
```

### 3.2 项目结构

**典型 CMake 项目结构：**

```
myproject/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   └── utils.cpp
├── include/
│   └── utils.hpp
└── test/
    └── test_main.cpp
```

**CMakeLists.txt 配置：**

```cmake
cmake_minimum_required(VERSION 3.20)
project(myproject LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 方法1：全局设置使用 libc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# 方法2：设置编译器（更可靠）
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# 可执行文件
add_executable(myapp
    src/main.cpp
    src/utils.cpp
)

target_include_directories(myapp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接 libc++ 和 libc++abi
target_link_libraries(myapp PRIVATE
    c++
    c++abi
)

# 启用警告
target_compile_options(myapp PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
```

### 3.3 基本命令

**编译命令详解：**

```bash
# 基本编译
clang++ -stdlib=libc++ main.cpp -o main

# 指定 C++ 标准
clang++ -stdlib=libc++ -std=c++20 main.cpp -o main

# 包含调试信息
clang++ -stdlib=libc++ -g main.cpp -o main

# 优化级别
clang++ -stdlib=libc++ -O2 main.cpp -o main

# 启用所有警告
clang++ -stdlib=libc++ -Wall -Wextra -Wpedantic main.cpp

# 指定头文件搜索路径
clang++ -stdlib=libc++ -I/usr/include/c++/v1 main.cpp

# 链接选项
clang++ -stdlib=libc++ main.cpp -lc++ -lc++abi -o main
```

**常用编译选项组合：**

| 用途 | 命令 |
|------|------|
| 开发调试 | `clang++ -stdlib=libc++ -std=c++20 -g -O0 -Wall` |
| 发布构建 | `clang++ -stdlib=libc++ -std=c++20 -O3 -DNDEBUG` |
| 体积优化 | `clang++ -stdlib=libc++ -std=c++20 -Os -DNDEBUG` |
| 静态分析 | `clang++ -stdlib=libc++ --analyze main.cpp` |

### 3.4 常用操作

**查看预定义宏：**

```bash
# 查看 libc++ 版本宏
clang++ -stdlib=libc++ -xc++ - -E -dM < /dev/null | grep _LIBCPP

# 查看特性测试宏
clang++ -stdlib=libc++ -std=c++20 -xc++ - -E -dM < /dev/null | grep __cpp_lib
```

**查看标准库路径：**

```bash
# 查看 libc++ 头文件路径
clang++ -stdlib=libc++ -xc++ -v -E /dev/null 2>&1 | grep "include/c++"

# 查看库文件路径
ldconfig -p | grep libc++
```

## 4. 进阶特性

### 4.1 高级配置

**禁用异常和 RTTI：**

```cmake
# CMakeLists.txt - 禁用异常和 RTTI
if(NOT CMAKE_CXX_FLAGS MATCHES "-fexceptions")
    string(APPEND CMAKE_CXX_FLAGS " -fno-exceptions")
endif()

if(NOT CMAKE_CXX_FLAGS MATCHES "-frtti")
    string(APPEND CMAKE_CXX_FLAGS " -fno-rtti")
endif()

# 注意：禁用异常时需要定义 libc++ 的宏
add_definitions(-D_LIBCPP_NO_EXCEPTIONS)
```

**条件编译示例：**

```cpp
// config.hpp
#ifdef _LIBCPP_VERSION
    #define USE_LIBCPP 1
    #define LIBCPP_VERSION _LIBCPP_VERSION
#else
    #define USE_LIBCPP 0
    #define LIBCPP_VERSION 0
#endif

// 根据标准库选择实现
#if USE_LIBCPP
    #include <format>
    std::string format_string(const std::string& fmt, auto... args) {
        return std::vformat(fmt, std::make_format_args(args...));
    }
#else
    #include <fmt/format.h>
    std::string format_string(const std::string& fmt, auto... args) {
        return fmt::vformat(fmt, fmt::make_format_args(args...));
    }
#endif
```

### 4.2 扩展功能

**C++20 std::format 高级用法：**

```cpp
#include <format>
#include <iostream>
#include <chrono>
#include <vector>

int main() {
    // 基本格式化
    std::string s1 = std::format("Hello, {}!", "World");
    std::cout << s1 << '\n';
    
    // 位置参数
    std::string s2 = std::format("{0} {1} {0}", "Hello", "World");
    std::cout << s2 << '\n';  // Hello World Hello
    
    // 数字格式化
    int num = 42;
    double pi = 3.14159265;
    std::cout << std::format("Dec: {}, Hex: {:#x}, Oct: {:#o}\n", num, num, num);
    std::cout << std::format("Pi = {:.2f}, {:.4f}\n", pi, pi);
    
    // 宽度和对齐
    std::cout << std::format("{:>10}\n", "right");   // 右对齐
    std::cout << std::format("{:<10}\n", "left");    // 左对齐
    std::cout << std::format("{:^10}\n", "center");  // 居中
    
    // 时间格式化（C++20）
    using namespace std::chrono;
    auto now = system_clock::now();
    std::cout << std::format("{:%Y-%m-%d %H:%M:%S}\n", now);
    
    return 0;
}
```

**C++20 std::span 容器视图：**

```cpp
#include <span>
#include <vector>
#include <array>
#include <iostream>

// 不拷贝数据的函数参数
void print_data(std::span<const int> data) {
    for (auto val : data) {
        std::cout << val << " ";
    }
    std::cout << '\n';
}

// 修改数据
void double_values(std::span<int> data) {
    for (auto& val : data) {
        val *= 2;
    }
}

int main() {
    // 从 vector 创建
    std::vector<int> vec = {1, 2, 3, 4, 5};
    print_data(vec);
    
    // 从数组创建
    int arr[] = {10, 20, 30, 40};
    print_data(arr);
    
    // 从 std::array 创建
    std::array<int, 3> stdArr = {100, 200, 300};
    print_data(stdArr);
    
    // 子跨度
    std::span<int> full(vec);
    std::span<int> first3 = full.first(3);
    std::span<int> last2 = full.last(2);
    std::span<int> middle = full.subspan(1, 3);
    
    // 修改操作
    double_values(vec);
    print_data(vec);
    
    return 0;
}
```

**C++23 std::expected 错误处理：**

```cpp
#include <expected>
#include <string>
#include <iostream>
#include <fstream>

// 使用 expected 作为返回类型
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");
    }
    return a / b;
}

// 文件读取示例
std::expected<std::string, int> read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        return std::unexpected(errno);
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    return content;
}

int main() {
    // 基本使用
    auto result1 = divide(10, 3);
    if (result1) {
        std::cout << "Result: " << result1.value() << '\n';
    } else {
        std::cout << "Error: " << result1.error() << '\n';
    }
    
    auto result2 = divide(10, 0);
    if (!result2) {
        std::cout << "Error: " << result2.error() << '\n';
    }
    
    // 使用 and_then 链式操作
    auto result3 = divide(20, 4)
        .and_then([](int n) { return divide(n, 2); })
        .transform([](int n) { return n * 10; });
    
    if (result3) {
        std::cout << "Chained result: " << result3.value() << '\n';
    }
    
    // 文件操作
    auto content = read_file("example.txt");
    if (content) {
        std::cout << "File content: " << content.value() << '\n';
    } else {
        std::cout << "Error code: " << content.error() << '\n';
    }
    
    return 0;
}
```

### 4.3 调试支持

**启用调试模式：**

```bash
# 启用调试迭代器（检测迭代器失效）
clang++ -stdlib=libc++ -D_LIBCPP_DEBUG=1 program.cpp

# 启用完整调试模式
clang++ -stdlib=libc++ -D_LIBCPP_ENABLE_DEBUG_MODE=1 program.cpp

# 启用边界检查
clang++ -stdlib=libc++ -D_LIBCPP_ENABLE_BOUND_CHECKS=1 program.cpp
```

**调试模式配置：**

```cpp
// 在代码中启用调试
#define _LIBCPP_DEBUG 1
#define _LIBCPP_ENABLE_DEBUG_MODE 1

#include <vector>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3};
    
    // 调试模式会检测越界访问
    try {
        int val = v.at(10);  // 抛出异常
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
    
    return 0;
}
```

## 5. 性能优化

### 5.1 调优策略

**编译优化级别：**

| 级别 | 说明 | 使用场景 |
|------|------|----------|
| `-O0` | 无优化 | 调试开发 |
| `-O1` | 基本优化 | 快速编译 |
| `-O2` | 标准优化 | 发布构建 |
| `-O3` | 激进优化 | 性能关键 |
| `-Os` | 体积优化 | 嵌入式、移动端 |
| `-Ofast` | 极致优化 | 科学计算 |

**链接时优化（LTO）：**

```bash
# 启用 LTO
clang++ -stdlib=libc++ -O2 -flto program.cpp -o program

# ThinLTO（更快，内存占用更少）
clang++ -stdlib=libc++ -O2 -flto=thin program.cpp -o program

# 完整 LTO 配置
clang++ -stdlib=libc++ -O3 -flto -ffat-lto-objects program.cpp
```

**Profile-Guided Optimization（PGO）：**

```bash
# 第一步：编译并插桩
clang++ -stdlib=libc++ -O2 -fprofile-generate program.cpp -o program_pgo

# 第二步：运行收集数据
./program_pgo  # 运行典型工作负载

# 第三步：使用 profile 优化编译
clang++ -stdlib=libc++ -O2 -fprofile-use program.cpp -o program_optimized

# 合并多个 profile
llvm-profdata merge -output=program.profdata *.profraw
```

### 5.2 最佳实践

**减小二进制体积：**

```bash
# 体积优化
clang++ -stdlib=libc++ -Os -DNDEBUG program.cpp -o program

# 剥离符号
strip program

# 使用 LTO 减小体积
clang++ -stdlib=libc++ -Os -flto program.cpp -o program

# 静态链接 libc++（增加移植性，减小依赖）
clang++ -stdlib=libc++ -static-libstdc++ program.cpp  # 注意：这会用静态版本
```

**内存优化技巧：**

```cpp
#include <vector>
#include <string>

// 使用 shrink_to_fit 释放多余内存
std::vector<int> big_vec(1000000);
big_vec.clear();
big_vec.shrink_to_fit();  // 释放内存

// 使用 reserve 避免重分配
std::vector<int> vec;
vec.reserve(1000);  // 预分配
for (int i = 0; i < 1000; ++i) {
    vec.push_back(i);  // 无重分配
}

// 使用 SSO（Short String Optimization）
// libc++ 的 std::string 有优化的 SSO
std::string short_str = "Hello";  // 栈上存储
std::string long_str = "This is a very long string...";  // 堆上存储
```

**性能基准测试：**

```cpp
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>

template<typename Func>
auto benchmark(Func&& func, int iterations = 1000) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    return duration.count() / static_cast<double>(iterations);
}

int main() {
    std::vector<int> data(10000);
    std::iota(data.begin(), data.end(), 0);
    
    auto sort_time = benchmark([&data]() {
        auto copy = data;
        std::sort(copy.begin(), copy.end());
    });
    
    std::cout << "Average sort time: " << sort_time << " microseconds\n";
    
    return 0;
}
```

## 6. 问题排查

### 6.1 常见问题

#### 链接错误

**问题：undefined reference to `__cxa_throw`**

```bash
# 错误原因：未链接 libc++abi
/usr/bin/ld: main.cpp:(.text+0x10): undefined reference to `__cxa_throw'

# 解决方案：链接 libc++abi
clang++ -stdlib=libc++ program.cpp -lc++abi
```

**问题：找不到标准库符号**

```bash
# 错误信息
error: unable to find library -lc++

# 解决方案：安装开发包或指定路径
# Debian/Ubuntu
sudo apt install libc++-dev libc++abi-dev

# 或指定路径
clang++ -stdlib=libc++ -L/usr/lib/llvm-15/lib program.cpp -lc++ -lc++abi
```

#### 头文件问题

**问题：'vector' file not found**

```bash
# 错误信息
fatal error: 'vector' file not found

# 解决方案1：指定头文件路径
clang++ -stdlib=libc++ -I/usr/include/c++/v1 program.cpp

# 解决方案2：检查安装
ls /usr/include/c++/v1  # 应该看到 vector, string 等文件
```

**问题：C++20 特性不可用**

```bash
# 错误信息
error: no member named 'format' in namespace 'std'

# 解决方案：确保使用 C++20 标准
clang++ -stdlib=libc++ -std=c++20 program.cpp

# 检查 libc++ 版本
echo '#include <version>' | clang++ -stdlib=libc++ -xc++ -E -dM - | grep _LIBCPP_VERSION
# 需要 _LIBCPP_VERSION >= 12000 (LLVM 12+) 才有完整 C++20 支持
```

#### 混用标准库问题

**问题：符号冲突**

```cpp
// 错误：混用 libc++ 和 libstdc++ 会导致 ABI 不兼容
// 避免这种情况！

// 检查是否混用
#ifdef _LIBCPP_VERSION
    // 使用 libc++
#else
    // 使用 libstdc++
#endif
```

```bash
# 确保整个项目使用同一标准库
# 错误示例
clang++ -stdlib=libc++ main.cpp -c -o main.o       # libc++
clang++ -stdlib=libstdc++ utils.cpp -c -o utils.o  # libstdc++  ❌
clang++ main.o utils.o -o program                   # 链接错误！

# 正确做法
clang++ -stdlib=libc++ main.cpp -c -o main.o
clang++ -stdlib=libc++ utils.cpp -c -o utils.o     # 一致使用 ✅
clang++ main.o utils.o -o program
```

### 6.2 调试技巧

**使用 AddressSanitizer：**

```bash
# 编译时启用 ASan
clang++ -stdlib=libc++ -fsanitize=address -g program.cpp -o program

# 运行检测
./program

# 内存泄漏检测
clang++ -stdlib=libc++ -fsanitize=address -fsanitize=leak program.cpp
```

**使用 UndefinedBehaviorSanitizer：**

```bash
# 检测未定义行为
clang++ -stdlib=libc++ -fsanitize=undefined -g program.cpp -o program
./program
```

**使用 ThreadSanitizer：**

```bash
# 检测数据竞争
clang++ -stdlib=libc++ -fsanitize=thread -g program.cpp -o program
./program
```

**详细诊断输出：**

```bash
# 启用详细错误信息
clang++ -stdlib=libc++ -v program.cpp 2>&1 | less

# 查看预处理器输出
clang++ -stdlib=libc++ -E program.cpp > preprocessed.cpp

# 查看模板实例化
clang++ -stdlib=libc++ -ftemplate-backtrace-limit=0 program.cpp
```

## 7. 集成实践

### 7.1 工具链集成

**Clang + libc++ + CMake：**

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(myproject LANGUAGES CXX)

# 设置 Clang 和 libc++
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 使用 -std=c++20 而非 -std=gnu++20

# 编译选项
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

# 链接库
link_libraries(c++ c++abi)

# 可执行文件
add_executable(myapp src/main.cpp)
```

**Bazel 集成：**

```python
# BUILD
cc_library(
    name = "mylib",
    srcs = ["lib.cpp"],
    hdrs = ["lib.h"],
    copts = [
        "-std=c++20",
        "-stdlib=libc++",
    ],
    linkopts = [
        "-lc++",
        "-lc++abi",
    ],
)

cc_binary(
    name = "myapp",
    srcs = ["main.cpp"],
    deps = [":mylib"],
    copts = [
        "-std=c++20",
        "-stdlib=libc++",
    ],
    linkopts = [
        "-lc++",
        "-lc++abi",
    ],
)
```

**Meson 集成：**

```python
# meson.build
project('myproject', 'cpp',
  default_options: ['cpp_std=c++20'])

cpp = meson.get_compiler('cpp')

# 添加 libc++ 编译选项
add_project_arguments('-stdlib=libc++', language: 'cpp')

# 链接 libc++
libcxx_dep = cpp.find_library('c++', required: true)
libcxxabi_dep = cpp.find_library('c++abi', required: true)

executable('myapp', 'main.cpp',
  dependencies: [libcxx_dep, libcxxabi_dep])
```

### 7.2 CI/CD 配置

**GitHub Actions：**

```yaml
# .github/workflows/ci.yml
name: CI

on: [push, pull_request]

jobs:
  build-linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install LLVM
        run: |
          wget https://apt.llvm.org/llvm.sh
          chmod +x llvm.sh
          sudo ./llvm.sh 15
          sudo apt install libc++-15-dev libc++abi-15-dev
      
      - name: Configure
        run: |
          cmake -B build -S . \
            -DCMAKE_C_COMPILER=clang-15 \
            -DCMAKE_CXX_COMPILER=clang++-15 \
            -DCMAKE_CXX_FLAGS="-stdlib=libc++"
      
      - name: Build
        run: cmake --build build --parallel
      
      - name: Test
        run: ctest --test-dir build --output-on-failure

  build-macos:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Configure
        run: cmake -B build -S . -DCMAKE_CXX_FLAGS="-stdlib=libc++"
      
      - name: Build
        run: cmake --build build --parallel
      
      - name: Test
        run: ctest --test-dir build --output-on-failure
```

**Docker 环境：**

```dockerfile
# Dockerfile
FROM ubuntu:22.04

# 安装 LLVM 和 libc++
RUN apt-get update && apt-get install -y \
    clang-15 \
    libc++-15-dev \
    libc++abi-15-dev \
    cmake \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# 设置编译器
ENV CC=clang-15
ENV CXX=clang++-15
ENV CXXFLAGS="-stdlib=libc++"

WORKDIR /app
COPY . .

RUN cmake -B build -S . -G Ninja
RUN cmake --build build

CMD ["./build/myapp"]
```

### 7.3 实战案例

**案例1：嵌入式项目配置**

```cmake
# 嵌入式项目：禁用异常和 RTTI，最小体积
cmake_minimum_required(VERSION 3.20)
project(embedded_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# 禁用异常和 RTTI
add_compile_options(
    -fno-exceptions
    -fno-rtti
    -D_LIBCPP_NO_EXCEPTIONS
)

# 体积优化
add_compile_options(-Os -ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections -Wl,--strip-all)

# 链接
add_executable(firmware src/main.cpp)
target_link_libraries(firmware c++ c++abi)
```

**案例2：高性能计算项目**

```cmake
# 高性能计算：启用所有优化
cmake_minimum_required(VERSION 3.20)
project(hpc_compute LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# 性能优化选项
add_compile_options(
    -O3
    -march=native
    -ffast-math
    -funroll-loops
    -flto
)

# 并行计算库
find_package(OpenMP REQUIRED)

add_executable(compute src/main.cpp)
target_link_libraries(compute c++ c++abi OpenMP::OpenMP_CXX)
```

**案例3：跨平台游戏引擎**

```cmake
# 跨平台游戏引擎
cmake_minimum_required(VERSION 3.20)
project(game_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# 平台特定配置
if(APPLE)
    # macOS/iOS 使用系统 libc++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=11.0")
elseif(UNIX)
    # Linux 需要链接 libc++abi
    link_libraries(c++ c++abi)
elseif(WIN32)
    # Windows MSVC 兼容
    add_compile_options(-D_CRT_SECURE_NO_WARNINGS)
endif()

# 游戏引擎常见配置
add_compile_options(
    -fno-exceptions        # 游戏常禁用异常
    -fno-rtti             # 减少开销
    -Wall -Wextra
)

add_executable(game src/main.cpp)
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| libc++ 官方文档 | https://libcxx.llvm.org |
| LLVM 项目主页 | https://llvm.org |
| C++ 标准支持状态 | https://libcxx.llvm.org/Status/Cxx20.html |
| libc++ 源码仓库 | https://github.com/llvm/llvm-project/tree/main/libcxx |
| libc++abi 文档 | https://libcxxabi.llvm.org |

### 8.2 学习路径

**初级路径：**

1. 了解 C++ 标准库基础
2. 学习使用标准容器和算法
3. 理解标准库的编译和链接

**中级路径：**

1. 掌握 CMake 与 libc++ 集成
2. 学习 C++17/20 新特性
3. 理解异常处理和 RTTI 配置
4. 掌握性能调优技巧

**高级路径：**

1. 深入理解 libc++ 实现原理
2. 贡献代码到 LLVM 项目
3. 定制 libc++ 配置
4. 嵌入式和特殊场景应用

### 8.3 社区资源

| 资源 | 说明 |
|------|------|
| LLVM Discourse | 官方论坛，问题讨论 |
| LLVM Discord | 社区实时交流 |
| Stack Overflow | `libc++` 标签问答 |
| GitHub Issues | Bug 报告和功能请求 |
| LLVM Weekly | 每周 LLVM 新闻 |

### 8.4 相关工具

| 工具 | 说明 |
|------|------|
| Clang | 配套编译器 |
| libc++abi | 异常处理支持库 |
| compiler-rt | 运行时库（sanitizer 等） |
| libcxx-utils | 辅助工具集 |