# GCC - GNU 编译器集合

## 1. 概述与背景

### 1.1 工具定位

GCC（GNU Compiler Collection）是最广泛使用的开源编译器集合，支持 C、C++、Fortran、Go、Objective-C 等多种编程语言。作为 GNU 项目的核心组件，GCC 是 Linux 系统的默认编译器，也是跨平台开发的重要工具。

### 1.2 发展历史

| 年份 | 版本 | 重要特性 |
|------|------|----------|
| 1987 | 1.0 | GNU C 编译器首次发布 |
| 1992 | 2.0 | 支持 C++ |
| 2001 | 3.0 | 重构架构，新增 Java 支持 |
| 2005 | 4.0 | 新增优化框架 GIMPLE |
| 2008 | 4.3 | 支持 C++0x 草案特性 |
| 2015 | 5.1 | 支持 C++14 标准 |
| 2017 | 7.1 | 支持 C++17 标准 |
| 2020 | 10.1 | 支持 C++20 标准 |
| 2023 | 13.1 | 支持 C++23 标准 |

### 1.3 核心特性

- **多语言支持**: C、C++、Fortran、Go、Objective-C 等
- **跨平台编译**: 支持 x86、ARM、RISC-V、MIPS 等多种架构
- **丰富的优化**: 从基础优化到激进优化多种级别
- **GNU 扩展**: 提供 GNU 特有的语言扩展
- **插件系统**: 支持编译器插件扩展功能
- **LTO 支持**: 链接时优化提升性能

### 1.4 适用场景

| 场景 | 说明 |
|------|------|
| 系统开发 | Linux 内核、系统库编译 |
| 嵌入式开发 | ARM、RISC-V 等嵌入式平台 |
| 跨平台编译 | Windows 到 Linux 交叉编译 |
| 开源项目 | 大量开源项目的默认编译器 |
| 教学实验 | 编译原理学习的实践平台 |

### 1.5 对比分析

| 编译器 | 优势 | 劣势 |
|--------|------|------|
| GCC | 成熟稳定、支持广泛、GNU 工具链集成 | 编译速度较慢、错误信息有时冗长 |
| Clang | 编译速度快、错误信息清晰、模块化架构 | GNU 扩展支持有限 |
| MSVC | Windows 平台最优支持、Visual Studio 集成 | 跨平台支持有限 |
| Intel ICC | Intel CPU 极致优化、性能卓越 | 商业许可、平台限制 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS:**

```bash
# 使用 Homebrew
brew install gcc

# 验证安装
gcc --version
g++ --version
```

**Ubuntu/Debian:**

```bash
# 安装基础包
sudo apt update
sudo apt install gcc g++ build-essential

# 安装多版本（可选）
sudo apt install gcc-11 g++-11 gcc-12 g++-12

# 切换默认版本
sudo update-alternatives --config gcc
```

**Windows:**

```bash
# 方式 1: MSYS2（推荐）
pacman -S mingw-w64-x86_64-gcc

# 方式 2: MinGW-w64
# 下载安装器从 https://mingw-w64.org/

# 方式 3: WSL
# 安装后在 WSL 中使用 Linux 方式安装
```

### 2.2 版本管理

```bash
# 查看当前版本
gcc --version

# 查看详细信息
gcc -v

# 查看支持的 C++ 标准
g++ -std=c++ -help 2>&1 | grep -A 10 "c++ standard"

# Ubuntu 多版本管理
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11
```

### 2.3 环境配置

```bash
# 设置编译器路径
export CC=gcc
export CXX=g++

# 添加库路径
export LIBRARY_PATH=/usr/local/lib:$LIBRARY_PATH
export C_INCLUDE_PATH=/usr/local/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=/usr/local/include:$CPLUS_INCLUDE_PATH

# 添加到 shell 配置文件
echo 'export CC=gcc' >> ~/.bashrc
echo 'export CXX=g++' >> ~/.bashrc
```

### 2.4 验证安装

```bash
# 创建测试文件
cat > test.cpp << 'EOF'
#include <iostream>
int main() {
    std::cout << "GCC Version Test" << std::endl;
    return 0;
}
EOF

# 编译并运行
g++ -std=c++17 test.cpp -o test
./test

# 检查特性支持
g++ -dM -E - < /dev/null | grep -i cplusplus
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 单文件编译
g++ main.cpp -o program

# 指定 C++ 标准
g++ -std=c++17 main.cpp -o program

# 开启警告
g++ -Wall -Wextra main.cpp -o program

# 包含调试信息
g++ -g main.cpp -o program_debug
```

### 3.2 编译流程详解

GCC 编译过程分为四个阶段：

```bash
# 1. 预处理（.cpp -> .ii）
g++ -E main.cpp -o main.ii

# 2. 编译（.ii -> .s）
g++ -S main.ii -o main.s

# 3. 汇编（.s -> .o）
g++ -c main.s -o main.o

# 4. 链接（.o -> 可执行文件）
g++ main.o -o main

# 一步完成
g++ main.cpp -o main
```

### 3.3 常用编译选项

**C++ 标准指定:**

| 选项 | 说明 |
|------|------|
| `-std=c++11` | C++11 标准 |
| `-std=c++14` | C++14 标准 |
| `-std=c++17` | C++17 标准 |
| `-std=c++20` | C++20 标准 |
| `-std=c++23` | C++23 草案 |
| `-std=gnu++17` | C++17 + GNU 扩展 |

**优化选项:**

| 选项 | 说明 |
|------|------|
| `-O0` | 不优化（默认） |
| `-O1` | 基本优化 |
| `-O2` | 标准优化（推荐） |
| `-O3` | 激进优化 |
| `-Os` | 优化体积 |
| `-Ofast` | 最快（可能不符合标准） |

**警告选项:**

| 选项 | 说明 |
|------|------|
| `-Wall` | 常见警告 |
| `-Wextra` | 额外警告 |
| `-Wpedantic` | 严格标准 |
| `-Werror` | 警告视为错误 |
| `-Wno-unused` | 禁用未使用警告 |

**调试选项:**

| 选项 | 说明 |
|------|------|
| `-g` | 生成调试信息 |
| `-g3` | 最大调试信息 |
| `-ggdb` | GDB 专用格式 |

### 3.4 链接选项

```bash
# 链接 pthread 线程库
g++ main.cpp -lpthread -o main

# 链接动态链接库
g++ main.cpp -ldl -o main

# 链接数学库
g++ main.cpp -lm -o main

# 指定库搜索路径
g++ main.cpp -L/usr/local/lib -lmylib -o main

# 指定头文件搜索路径
g++ -I/usr/local/include main.cpp -o main

# 运行时库路径
g++ main.cpp -Wl,-rpath,/usr/local/lib -o main
```

## 4. 进阶特性

### 4.1 链接时优化（LTO）

```bash
# 启用 LTO
g++ -flto -O2 main.cpp -o main

# 多线程 LTO（加速编译）
g++ -flto=auto -O2 main.cpp -o main

# CMake 配置 LTO
cmake -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON ..
```

### 4.2 Profile-Guided Optimization（PGO）

```bash
# 步骤 1: 插桩编译
g++ -fprofile-generate -O2 main.cpp -o main_instrumented

# 步骤 2: 运行收集数据
./main_instrumented
# 生成 *.gcda 文件

# 步骤 3: 使用 profile 优化编译
g++ -fprofile-use -O2 main.cpp -o main_optimized
```

### 4.3 GNU 扩展特性

```cpp
// 零长度数组
struct flex_array {
    int len;
    char data[0];  // GNU 扩展
};

// case 范围
switch (c) {
    case 'a' ... 'z':  // GNU 扩展
        break;
}

// 语句表达式
#define MAX(a, b) ({ \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a > _b ? _a : _b; \
})
```

### 4.4 预定义宏

```bash
# 查看所有预定义宏
g++ -dM -E - < /dev/null

# 常用预定义宏
__GNUC__          # GCC 主版本
__GNUC_MINOR__    # 次版本
__GNUC_PATCHLEVEL__  # 补丁版本
__cplusplus       # C++ 标准版本
__linux__         # Linux 平台
__APPLE__         # macOS 平台
__x86_64__        # x86-64 架构
__arm__           # ARM 架构
```

## 5. 性能优化

### 5.1 调优策略

```bash
# 生产环境推荐配置
g++ -std=c++17 -O2 -march=native -mtune=native \
    -DNDEBUG -flto main.cpp -o main_release

# 针对特定 CPU 优化
g++ -march=skylake -mtune=skylake main.cpp -o main

# 优化体积
g++ -Os -DNDEBUG main.cpp -o main_small
```

### 5.2 最佳实践

| 场景 | 推荐选项 |
|------|----------|
| 开发调试 | `-O0 -g3 -Wall -Wextra -fsanitize=address` |
| 单元测试 | `-O1 -g -Wall -Wextra` |
| 性能测试 | `-O2 -march=native` |
| 生产发布 | `-O2 -march=native -DNDEBUG -flto` |
| 最小体积 | `-Os -DNDEBUG -s` |

## 6. 问题排查

### 6.1 常见问题

**编译错误：找不到头文件**

```bash
# 错误信息
fatal error: myheader.h: No such file or directory

# 解决方案
g++ -I/path/to/headers main.cpp -o main
```

**链接错误：未定义的引用**

```bash
# 错误信息
undefined reference to `function_name'

# 解决方案
g++ main.cpp -lmylib -L/path/to/libs -Wl,-rpath,/path/to/libs
```

**运行时错误：找不到共享库**

```bash
# 错误信息
error while loading shared libraries: libmylib.so

# 解决方案 1: 设置 LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/path/to/libs:$LD_LIBRARY_PATH

# 解决方案 2: 使用 rpath
g++ main.cpp -L/path/to/libs -lmylib -Wl,-rpath,/path/to/libs
```

### 6.2 调试技巧

```bash
# 查看预处理结果
g++ -E main.cpp -o main.ii

# 查看汇编代码
g++ -S -fverbose-asm main.cpp -o main.s

# 查看符号表
nm main.o

# 查看依赖库
ldd main

# 内存检测（AddressSanitizer）
g++ -fsanitize=address -g main.cpp -o main_asan
./main_asan

# 未定义行为检测
g++ -fsanitize=undefined -g main.cpp -o main_ubsan
./main_ubsan
```

## 7. 集成实践

### 7.1 CMake 集成

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(MyProject CXX)

# 指定编译器
set(CMAKE_CXX_COMPILER g++)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 通用编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Debug 配置
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address")

# Release 配置
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=native")

# 启用 LTO（可选）
include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT LTO_ERROR)
if(LTO_SUPPORTED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
```

### 7.2 Makefile 集成

```makefile
# Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lpthread

# Debug 构建
DEBUG_FLAGS = -g -O0 -fsanitize=address

# Release 构建
RELEASE_FLAGS = -O2 -DNDEBUG -march=native

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean all

release: CXXFLAGS += $(RELEASE_FLAGS)
release: clean all

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all debug release clean
```

### 7.3 CI/CD 配置

**GitHub Actions 示例:**

```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        compiler: [gcc-11, gcc-12, gcc-13]
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Compiler
        run: |
          sudo apt update
          sudo apt install ${{ matrix.compiler }} ${{ matrix.compiler | replace('gcc', 'g++') }}
      
      - name: Build
        run: |
          mkdir build && cd build
          cmake -DCMAKE_CXX_COMPILER=${{ matrix.compiler }} ..
          cmake --build .
      
      - name: Test
        run: |
          cd build
          ctest --output-on-failure
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| GCC 官网 | https://gcc.gnu.org/ |
| GCC 手册 | https://gcc.gnu.org/onlinedocs/ |
| GCC 选项索引 | https://gcc.gnu.org/onlinedocs/gcc/Option-Index.html |
| GCC Wiki | https://gcc.gnu.org/wiki/ |

### 8.2 学习路径

| 阶段 | 内容 |
|------|------|
| 入门 | 基本编译命令、选项使用 |
| 进阶 | 多文件编译、库链接、Makefile |
| 高级 | 优化技巧、交叉编译、LTO、PGO |
| 专家 | 编译器原理、GCC 插件开发 |