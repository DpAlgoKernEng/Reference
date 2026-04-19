# CMake - 跨平台构建系统生成器

## 1. 概述与背景

### 1.1 工具定位

CMake 是一个开源的跨平台构建系统生成器，它通过 `CMakeLists.txt` 文件描述项目的构建过程，生成平台原生的构建文件。CMake 本身不直接构建项目，而是生成平台特定的构建系统，如 Makefile、Visual Studio 项目文件、Ninja 文件等。

CMake 的核心设计理念是"构建系统生成器"而非"构建系统"，这使得开发者只需维护一套构建配置，即可在多个平台上生成对应的本地构建文件。

### 1.2 发展历史

| 年份 | 版本 | 重要特性 |
|------|------|----------|
| 2000 | 0.1 | Kitware 发布初始版本 |
| 2002 | 1.4 | 支持 Windows 平台 |
| 2006 | 2.4 | 引入 CPack 打包工具 |
| 2008 | 2.6 | 增强跨平台支持 |
| 2013 | 2.8 | 引入 CMake Presets |
| 2015 | 3.0 | 现代 CMake 风格确立 |
| 2018 | 3.12 | 增强依赖管理 |
| 2020 | 3.18 | 支持 C++20 |
| 2022 | 3.24 | 改进预设系统 |
| 2023 | 3.28 | 支持 C++23 |

### 1.3 核心特性

CMake 具有以下核心特性：

| 特性 | 描述 |
|------|------|
| 跨平台 | 支持 Windows、Linux、macOS、BSD 等 |
| 多生成器 | 可生成 Makefile、Ninja、VS、Xcode 等构建文件 |
| 依赖管理 | 支持 find_package、FetchContent、vcpkg 集成 |
| 测试支持 | 内置 CTest 测试框架 |
| 打包工具 | 内置 CPack 打包系统 |
| 预设系统 | 通过 CMakePresets.json 统一配置 |
| 目标导向 | 现代目标属性传播机制 |

### 1.4 适用场景

CMake 适用于以下场景：

- **跨平台项目开发**：需要在多个操作系统上构建的项目
- **开源项目**：便于用户在不同平台上编译
- **大型 C/C++ 项目**：模块化构建管理
- **嵌入式开发**：交叉编译支持
- **CI/CD 流程**：标准化的构建流程
- **IDE 集成**：生成项目文件用于 Visual Studio、CLion 等

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| CMake | 跨平台、生态成熟、IDE 支持好 | 语法复杂、学习曲线陡峭 |
| Meson | 语法简洁、构建速度快 | 生态较小、社区支持有限 |
| Bazel | 构建可重现、适合大型项目 | 配置复杂、学习成本高 |
| Make | 简单直接、广泛使用 | 跨平台支持弱、维护困难 |
| Ninja | 构建速度极快 | 需要其他工具生成 |
| xmake | 现代化语法、中文社区 | 国际化程度较低 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# macOS - Homebrew
brew install cmake

# macOS - 官方安装包
# 从 https://cmake.org/download/ 下载 .dmg 安装

# Ubuntu/Debian
sudo apt update
sudo apt install cmake

# Ubuntu/Debian - 安装最新版本（推荐）
sudo apt install software-properties-common
sudo add-apt-repository ppa:kitware/ppa
sudo apt update
sudo apt install cmake

# CentOS/RHEL
sudo yum install cmake

# Fedora
sudo dnf install cmake

# Windows - winget
winget install cmake

# Windows - Chocolatey
choco install cmake

# Windows - Scoop
scoop install cmake

# Windows - 官方安装包
# 从 https://cmake.org/download/ 下载 .msi 安装
```

### 2.2 版本管理

CMake 版本选择建议：

| 项目类型 | 推荐版本 | 原因 |
|----------|----------|------|
| 新项目 | 3.20+ | 现代特性、更好的预设支持 |
| 开源项目 | 3.16+ | 广泛兼容性 |
| 企业项目 | 3.14+ | 稳定可靠 |
| 旧项目维护 | 遵循现有要求 | 避免兼容问题 |

在 CMakeLists.txt 中设置最低版本：

```cmake
# 设置最低版本要求
cmake_minimum_required(VERSION 3.16)

# 同时设置策略级别
cmake_minimum_required(VERSION 3.16...3.28)
```

### 2.3 环境配置

```bash
# Linux/macOS - 配置 PATH（如需）
export PATH="/usr/local/cmake/bin:$PATH"

# 添加到 shell 配置文件
echo 'export PATH="/usr/local/cmake/bin:$PATH"' >> ~/.bashrc
# 或
echo 'export PATH="/usr/local/cmake/bin:$PATH"' >> ~/.zshrc

# Windows - 配置环境变量（安装时自动配置）
# 手动配置：系统属性 -> 高级 -> 环境变量 -> Path
```

### 2.4 验证安装

```bash
# 检查版本
cmake --version

# 查看帮助
cmake --help

# 列出可用生成器
cmake --help-generators

# 查看 cmake-gui 可用性
cmake-gui --version

# 查看 ctest 可用性
ctest --version

# 查看 cpack 可用性
cpack --version
```

## 3. 基础使用

### 3.1 快速入门

创建第一个 CMake 项目：

```bash
# 创建项目目录
mkdir my_project && cd my_project

# 创建源代码目录和文件
mkdir src
cat > src/main.c << 'EOF'
#include <stdio.h>

int main(void) {
    printf("Hello, CMake!\n");
    return 0;
}
EOF

# 创建 CMakeLists.txt
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.16)
project(HelloCMake C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(hello src/main.c)
EOF

# 配置和构建
cmake -B build
cmake --build build

# 运行程序
./build/hello
```

### 3.2 项目结构

标准的 C 项目结构：

```
project/
├── CMakeLists.txt          # 主配置文件
├── CMakePresets.json       # 预设配置（可选）
├── cmake/                  # CMake 模块目录
│   ├── FindXXX.cmake       # 自定义查找模块
│   └── XXXConfig.cmake     # 配置文件
├── include/                # 公共头文件
│   └── mylib.h
├── src/                    # 源代码目录
│   ├── CMakeLists.txt      # 子目录配置
│   ├── main.c              # 主程序
│   └── lib/                # 库源码
│       ├── CMakeLists.txt
│       ├── mylib.c
│       └── mylib_internal.h
├── test/                   # 测试目录
│   ├── CMakeLists.txt
│   └── test_mylib.c
├── build/                  # 构建输出目录（不提交）
└── README.md               # 项目说明
```

### 3.3 基本命令

CMakeLists.txt 核心命令详解：

```cmake
# ==================== 项目配置 ====================

# 设置最低 CMake 版本（必须在 project() 之前）
cmake_minimum_required(VERSION 3.16)

# 定义项目（建议指定语言）
project(MyProject
    VERSION 1.0.0
    DESCRIPTION "A sample C project"
    LANGUAGES C
)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)  # 禁用 GNU 扩展

# ==================== 目标定义 ====================

# 可执行文件
add_executable(myapp
    src/main.c
    src/utils.c
)

# 静态库
add_library(mylib STATIC
    src/lib/mylib.c
)

# 动态库
add_library(myshared SHARED
    src/lib/mylib.c
)

# 头文件库（仅头文件）
add_library(myheader INTERFACE)
target_include_directories(myheader INTERFACE include)

# ==================== 目标属性 ====================

# 链接库
target_link_libraries(myapp PRIVATE mylib)

# 头文件目录
target_include_directories(myapp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 编译选项
target_compile_options(myapp PRIVATE -Wall -Wextra -pedantic)

# 编译定义
target_compile_definitions(myapp PRIVATE DEBUG_MODE)

# ==================== 变量和选项 ====================

# 设置变量
set(MY_VAR "value")

# 定义构建选项
option(ENABLE_TESTS "Enable testing" ON)
option(USE_SHARED_LIB "Use shared library" OFF)

# 条件判断
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# ==================== 子目录 ====================

# 添加子目录
add_subdirectory(src/lib)
```

### 3.4 常用操作

| 命令 | 说明 | 示例 |
|------|------|------|
| `cmake_minimum_required(VERSION x.x)` | 设置最低版本 | `cmake_minimum_required(VERSION 3.16)` |
| `project(name [LANGUAGES C CXX])` | 定义项目 | `project(MyApp C)` |
| `add_executable(name src...)` | 添加可执行文件 | `add_executable(app main.c)` |
| `add_library(name [STATIC\|SHARED] src...)` | 添加库 | `add_library(mylib STATIC lib.c)` |
| `target_link_libraries(name lib...)` | 链接库 | `target_link_libraries(app mylib)` |
| `target_include_directories(name dir...)` | 添加头文件目录 | `target_include_directories(app include)` |
| `add_subdirectory(dir)` | 添加子目录 | `add_subdirectory(src/lib)` |
| `set(var value)` | 设置变量 | `set(SRC_FILES main.c utils.c)` |
| `option(name "desc" [ON\|OFF])` | 定义选项 | `option(BUILD_TESTS "Build tests" ON)` |
| `find_package(name)` | 查找包 | `find_package(Threads REQUIRED)` |
| `file(GLOB var pattern)` | 文件匹配 | `file(GLOB SRCS src/*.c)` |

## 4. 进阶特性

### 4.1 高级配置

**变量和缓存：**

```cmake
# 缓存变量（可通过命令行修改）
set(MY_OPTION "default" CACHE STRING "Description")

# 环境变量
set(ENV{MY_VAR} "value")

# 从缓存读取
$CACHE{MY_OPTION}

# 条件变量设置
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
```

**自定义命令和目标：**

```cmake
# 自定义命令生成文件
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated.h
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/template.h
        ${CMAKE_CURRENT_BINARY_DIR}/generated.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/template.h
    COMMENT "Generating header file"
)

# 自定义目标
add_custom_target(generate_headers
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/generated.h
)

# 依赖自定义目标
add_executable(myapp src/main.c)
add_dependencies(myapp generate_headers)
```

**属性传播：**

```cmake
# PUBLIC: 传播给使用者和自身
# PRIVATE: 仅自身使用
# INTERFACE: 仅使用者使用

add_library(mylib STATIC src/lib.c)
target_include_directories(mylib
    PUBLIC include           # 使用者和自身都可见
    PRIVATE src              # 仅自身可见
    INTERFACE interface      # 仅使用者可见
)
```

### 4.2 扩展功能

**查找外部包：**

```cmake
# 查找系统包
find_package(Threads REQUIRED)
target_link_libraries(myapp PRIVATE Threads::Threads)

# 查找库文件
find_library(ZLIB_LIBRARIES z)
if(ZLIB_LIBRARIES)
    target_link_libraries(myapp PRIVATE ${ZLIB_LIBRARIES})
endif()

# 查找头文件
find_path(MATH_INCLUDE_DIR math.h)
if(MATH_INCLUDE_DIR)
    target_include_directories(myapp PRIVATE ${MATH_INCLUDE_DIR})
endif()

# 自定义 Find 模块
# cmake/FindMyLib.cmake
find_path(MYLIB_INCLUDE_DIR mylib.h)
find_library(MYLIB_LIBRARY mylib)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MyLib DEFAULT_MSG
    MYLIB_INCLUDE_DIR MYLIB_LIBRARY)
```

**FetchContent 依赖管理：**

```cmake
include(FetchContent)

# 下载并使用外部项目
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(json)

target_link_libraries(myapp PRIVATE nlohmann_json::nlohmann_json)
```

### 4.3 生成器

| 生成器 | 平台 | 说明 |
|--------|------|------|
| `Unix Makefiles` | Linux/macOS | 生成 Makefile |
| `Ninja` | 跨平台 | 生成 Ninja 文件，构建最快 |
| `Visual Studio 17 2022` | Windows | 生成 VS 2022 项目 |
| `Visual Studio 16 2019` | Windows | 生成 VS 2019 项目 |
| `Xcode` | macOS | 生成 Xcode 项目 |
| `CodeBlocks - Ninja` | 跨平台 | CodeBlocks IDE |
| `CLion` | 跨平台 | CLion 原生支持 |

```bash
# 指定生成器
cmake -G Ninja -B build

# 查看所有生成器
cmake --help-generators

# 使用生成器表达式
cmake -G "Visual Studio 17 2022" -A x64 -B build
```

## 5. 性能优化

### 5.1 构建优化

```cmake
# 并行构建
cmake --build build --parallel 8

# 使用 Ninja 生成器（推荐）
cmake -G Ninja -B build

# 预编译头
target_precompile_headers(myapp PRIVATE src/pch.h)

# Unity 构建（合并编译单元）
set(CMAKE_UNITY_BUILD ON)

# 缓存编译结果
set(CMAKE_C_COMPILER_LAUNCHER ccache)
```

### 5.2 配置优化

```cmake
# 构建类型优化
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)
endif()

# 链接时优化 (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

# 静态运行时库 (MSVC)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 隐藏符号（减小二进制大小）
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
```

### 5.3 最佳实践

| 实践 | 说明 |
|------|------|
| 使用 Ninja | 比 Make 快 2-5 倍 |
| 外部构建 | 永远使用 `-B build` |
| 目标导向 | 使用 `target_*` 命令 |
| 避免全局变量 | 减少 `include_directories` 等全局命令 |
| 使用预设 | 通过 CMakePresets.json 统一配置 |
| 增量构建 | 利用 ccache 加速 |

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| `Could not find CMAKE_C_COMPILER` | 编译器未安装 | 安装 GCC/Clang/MSVC |
| `Could not find XXX` | 依赖未找到 | 检查安装路径或 CMAKE_PREFIX_PATH |
| `undefined reference to XXX` | 链接错误 | 检查 target_link_libraries |
| `Policy CMPXXXX is not set` | 版本兼容警告 | 设置 cmake_policy 或提高版本要求 |
| `file COPY cannot find` | 文件路径错误 | 使用绝对路径或正确的相对路径 |
| `The C compiler identification is unknown` | 编译器配置错误 | 检查编译器路径和环境变量 |

### 6.2 调试技巧

```bash
# 详细输出
cmake -B build --debug-output

# 查看变量值
cmake -B build -LH

# 查看缓存变量
cmake -B build -LA

# 追踪执行过程
cmake -B build --trace

# 追踪特定命令
cmake -B build --trace-expand

# 查看依赖图
cmake --graphviz=deps.dot build
dot -Tpng deps.dot -o deps.png

# 清理构建
cmake --build build --target clean

# 完全重新配置
rm -rf build && cmake -B build
```

## 7. 集成实践

### 7.1 工具链集成

**vcpkg 集成：**

```cmake
# 设置 vcpkg 工具链
cmake -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

# 在 CMakeLists.txt 中使用 vcpkg 包
find_package(fmt REQUIRED)
target_link_libraries(myapp PRIVATE fmt::fmt)

# 在 CMakePresets.json 中配置
```

**Conan 集成：**

```bash
# 生成 Conan 配置
conan install . --output-folder=build --build=missing

# CMake 配置
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
```

### 7.2 CMake Presets

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "default",
      "hidden": true,
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/${presetName}"
    },
    {
      "name": "debug",
      "displayName": "Debug",
      "inherits": "default",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_C_FLAGS_DEBUG": "-g -O0"
      }
    },
    {
      "name": "release",
      "displayName": "Release",
      "inherits": "default",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_C_FLAGS_RELEASE": "-O3"
      }
    },
    {
      "name": "macos-debug",
      "displayName": "macOS Debug",
      "inherits": "debug",
      "condition": {
        "type": "equals",
        "lhs": "${hostSystemName}",
        "rhs": "Darwin"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "debug",
      "configurePreset": "debug"
    },
    {
      "name": "release",
      "configurePreset": "release"
    }
  ]
}
```

```bash
# 使用预设
cmake --preset debug
cmake --build --preset debug
```

### 7.3 CI/CD 配置

**GitHub Actions：**

```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        build_type: [Debug, Release]

    steps:
      - uses: actions/checkout@v4

      - name: Configure
        run: cmake -B build -DCMAKE_BUILD_TYPE=${{ matrix.build_type }}

      - name: Build
        run: cmake --build build --config ${{ matrix.build_type }}

      - name: Test
        run: ctest --test-dir build -C ${{ matrix.build_type }} --output-on-failure
```

### 7.4 实战案例

**多目录项目构建：**

```cmake
# 根目录 CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(MyProject C)

# 添加库子目录
add_subdirectory(src/mylib)

# 添加可执行文件
add_executable(myapp src/main.c)
target_link_libraries(myapp PRIVATE mylib)

# src/mylib/CMakeLists.txt
add_library(mylib STATIC mylib.c)
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 地址 |
|------|------|
| 官方网站 | https://cmake.org/ |
| 官方文档 | https://cmake.org/documentation/ |
| 命令参考 | https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html |
| 变量参考 | https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html |
| 生成器表达式 | https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html |

### 8.2 学习路径

| 阶段 | 内容 | 资源 |
|------|------|------|
| 入门 | 基本命令、项目结构 | CMake Tutorial (官方) |
| 进阶 | 目标属性、依赖管理 | Modern CMake (gitbook) |
| 高级 | 自定义命令、预设系统 | Professional CMake |
| 实战 | 工具链集成、CI/CD | 项目实践 |

### 8.3 社区资源

| 资源 | 说明 |
|------|------|
| Modern CMake | https://cliutils.gitlab.io/modern-cmake/ |
| CMake Examples | https://github.com/ttroy50/cmake-examples |
| Awesome CMake | https://github.com/onqtam/awesome-cmake |
| CMake Discourse | https://discourse.cmake.org/ |
| Stack Overflow | [cmake] 标签 |

### 8.4 常用速查

```bash
# 常用构建命令
cmake -B build                          # 配置项目
cmake --build build                     # 构建项目
cmake --build build --parallel 8        # 并行构建
cmake --build build --target install    # 安装
ctest --test-dir build                  # 运行测试
cpack --config build/CPackConfig.cmake  # 打包

# 清理命令
cmake --build build --target clean      # 清理构建
rm -rf build                            # 完全清理

# 调试命令
cmake -B build -LH                      # 显示变量
cmake -B build --graphviz=deps.dot      # 生成依赖图
```