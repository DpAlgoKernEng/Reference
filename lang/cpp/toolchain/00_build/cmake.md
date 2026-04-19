# CMake - 跨平台构建系统生成器

## 1. 概述与背景

### 1.1 工具定位

CMake 是一个开源的跨平台构建系统生成器，它通过 `CMakeLists.txt` 文件描述项目的构建过程，能够生成平台原生的构建文件，如 Unix Makefile、Ninja 构建文件、Visual Studio 项目文件、Xcode 项目等。CMake 本身不直接编译代码，而是生成对应平台的构建文件，再由底层构建工具完成实际的编译工作。

### 1.2 发展历史

| 年份 | 版本 | 重要特性 |
|------|------|----------|
| 2000 | 1.0 | Kitware 发布首个版本 |
| 2002 | 2.0 | 支持 Windows 平台 |
| 2006 | 2.6 | 引入属性系统、测试支持 |
| 2013 | 2.8 | 引入 FetchContent、生成器表达式 |
| 2015 | 3.0 | 现代 CMake 起点，target 导向 |
| 2018 | 3.12 | 增强 find_package、编译特性 |
| 2020 | 3.19 | CMake Presets 官方支持 |
| 2023 | 3.28 | C++20 模块初步支持 |

### 1.3 核心特性

- **跨平台支持**：一套构建描述生成多平台构建文件
- **依赖管理**：`find_package` 和 `FetchContent` 两种依赖获取方式
- **生成器表达式**：条件编译配置，适应不同构建类型
- **目标导向**：现代 CMake 以 target 为核心，属性传递清晰
- **预设系统**：`CMakePresets.json` 统一配置，便于共享
- **测试支持**：内置 CTest 测试框架
- **打包支持**：CPack 支持多种安装包格式

### 1.4 适用场景

| 场景 | 描述 |
|------|------|
| 跨平台项目 | 需要在 Windows/Linux/macOS 上构建的项目 |
| 大型项目 | 管理复杂依赖和模块化构建 |
| IDE 集成 | 生成 Visual Studio、CLion、VS Code 等项目配置 |
| 开源库开发 | 提供统一的构建和安装流程 |
| 嵌入式开发 | 交叉编译工具链配置 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| CMake | 跨平台、IDE 支持、社区庞大 | 学习曲线陡峭、语法复杂 |
| Makefile | 简单直观、无需依赖 | 不跨平台、大型项目难维护 |
| Meson | 语法现代、编译快 | 社区小、生态不如 CMake |
| Bazel | 增量构建、大型单体仓库 | 配置复杂、学习成本高 |
| XMake | 轻量、Lua 语法 | 生态相对较小 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# macOS (Homebrew)
brew install cmake

# Ubuntu/Debian
sudo apt update && sudo apt install cmake

# Windows (winget)
winget install Kitware.CMake

# Windows (Chocolatey)
choco install cmake

# Fedora
sudo dnf install cmake

# Arch Linux
sudo pacman -S cmake
```

### 2.2 版本管理

```bash
# 查看版本
cmake --version

# 下载特定版本（官方）
# https://cmake.org/download/

# 使用 Kitware 仓库获取最新版本（Ubuntu）
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
sudo apt update && sudo apt install cmake
```

### 2.3 环境配置

```bash
# 设置默认生成器
export CMAKE_GENERATOR=Ninja

# 设置构建类型
export CMAKE_BUILD_TYPE=Debug

# 添加到 PATH（Windows）
# 将 C:\Program Files\CMake\bin 添加到系统 PATH
```

### 2.4 验证安装

```bash
cmake --version
# cmake version 3.28.0

cmake --help
# 查看可用生成器列表
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 1. 创建项目结构
mkdir myproject && cd myproject
mkdir src build

# 2. 创建源文件
cat > src/main.cpp << 'EOF'
#include <iostream>
int main() {
    std::cout << "Hello CMake!" << std::endl;
    return 0;
}
EOF

# 3. 创建 CMakeLists.txt
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.16)
project(MyProject CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(myapp src/main.cpp)
EOF

# 4. 配置并构建
cmake -B build
cmake --build build

# 5. 运行
./build/myapp
```

### 3.2 项目结构

```
project/
├── CMakeLists.txt          # 主构建文件
├── CMakePresets.json       # 预设配置（可选）
├── cmake/                  # CMake 模块目录
│   ├── FindXXX.cmake       # 自定义查找模块
│   └── CompilerOptions.cmake
├── include/                # 公共头文件
│   └── mylib/
│       └── mylib.h
├── src/                    # 源代码
│   ├── main.cpp
│   └── lib/
│       ├── CMakeLists.txt  # 子目录构建文件
│       ├── mylib.cpp
│       └── mylib.h
├── tests/                  # 测试代码
│   └── CMakeLists.txt
├── build/                  # 构建目录（out-of-source）
└── install/                # 安装目录
```

### 3.3 基本命令

| 命令 | 说明 | 示例 |
|------|------|------|
| `cmake_minimum_required` | 设置最低版本 | `cmake_minimum_required(VERSION 3.16)` |
| `project` | 定义项目名称和语言 | `project(MyApp C CXX)` |
| `add_executable` | 添加可执行目标 | `add_executable(app main.cpp)` |
| `add_library` | 添加库目标 | `add_library(mylib STATIC lib.cpp)` |
| `target_link_libraries` | 链接库到目标 | `target_link_libraries(app mylib)` |
| `target_include_directories` | 添加头文件目录 | `target_include_directories(app include)` |
| `target_compile_options` | 添加编译选项 | `target_compile_options(app -Wall)` |
| `target_compile_features` | 添加编译特性 | `target_compile_features(app cxx_std_17)` |
| `add_subdirectory` | 添加子目录 | `add_subdirectory(src)` |
| `set` | 设置变量 | `set(SOURCES a.cpp b.cpp)` |
| `option` | 定义构建选项 | `option(ENABLE_TESTS "Enable tests" ON)` |
| `find_package` | 查找外部包 | `find_package(Boost REQUIRED)` |
| `install` | 安装规则 | `install(TARGETS app DESTINATION bin)` |

### 3.4 常用操作

```bash
# 配置项目
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 构建项目
cmake --build build

# 并行构建（指定线程数）
cmake --build build -j4

# 清理构建
cmake --build build --target clean

# 安装
cmake --install build --prefix /usr/local

# 运行测试
cd build && ctest --output-on-failure
```

## 4. 进阶特性

### 4.1 高级配置

```cmake
# 条件编译
option(USE_OPENMP "Enable OpenMP" OFF)
if(USE_OPENMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(myapp OpenMP::OpenMP_CXX)
endif()

# 平台判断
if(WIN32)
    target_compile_definitions(myapp PRIVATE WINDOWS_BUILD)
elseif(UNIX)
    target_compile_definitions(myapp PRIVATE UNIX_BUILD)
endif()

# 生成器表达式
target_compile_options(myapp PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# 自定义命令
add_custom_command(
    OUTPUT generated.h
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/generate.py
    DEPENDS generate.py
    COMMENT "Generating header file"
)

# 自定义目标
add_custom_target(generate_headers
    DEPENDS generated.h
)
add_dependencies(myapp generate_headers)
```

### 4.2 扩展功能

```cmake
# FetchContent - 下载外部依赖
include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)
target_link_libraries(myapp nlohmann_json::nlohmann_json)

# ExternalProject - 构建外部项目
include(ExternalProject)
ExternalProject_Add(
    external_lib
    GIT_REPOSITORY https://github.com/example/lib.git
    GIT_TAG main
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

# 配置文件生成
configure_file(
    ${CMAKE_SOURCE_DIR}/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
    @ONLY
)

# 文件操作
file(GLOB SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")
```

### 4.3 插件生态

| 模块 | 功能 | 用途 |
|------|------|------|
| CMakePresets.json | 预设配置 | 统一配置管理 |
| CTest | 测试框架 | 自动化测试 |
| CPack | 打包工具 | 生成安装包 |
| CMakeUserPresets.json | 用户配置 | 本地开发设置 |

## 5. 性能优化

### 5.1 调优策略

```cmake
# 使用 Ninja 生成器（更快）
cmake -G Ninja -B build

# 预编译头
target_precompile_headers(myapp PRIVATE
    <vector>
    <string>
    <iostream>
)

# Unity 构建（减少编译单元）
set(CMAKE_UNITY_BUILD ON)

# 并行编译
set(CMAKE_BUILD_PARALLEL_LEVEL 8)

# 缓存编译结果（ccache）
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()
```

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| Out-of-source 构建 | 在单独目录构建，不污染源码 |
| target_* 命令 | 避免 directory 级别的全局设置 |
| 属性传递 | 正确使用 PUBLIC/PRIVATE/INTERFACE |
| 最小化 GLOB | 明确列出源文件，避免增量构建问题 |
| 版本锁定 | 在 CMakeLists.txt 中锁定最低版本 |
| 现代 CMake | 优先使用 target 属性而非全局变量 |

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| 找不到头文件 | include 路径未设置 | 使用 `target_include_directories` |
| 链接错误 | 库未正确链接 | 检查 `target_link_libraries` |
| 重复定义 | 源文件被多次包含 | 检查 `add_executable/add_library` |
| 依赖未找到 | find_package 失败 | 设置 `CMAKE_PREFIX_PATH` |
| 生成器不匹配 | 生成器与编译器冲突 | 使用 `-G` 指定正确生成器 |

### 6.2 调试技巧

```bash
# 打印变量
cmake -B build --debug-output

# 查看详细输出
cmake --build build -v

# 查看缓存变量
cmake -B build -LH

# 图形化界面
cmake-gui

# 分析依赖
cmake --graphviz=deps.dot -B build
dot -Tpng deps.dot -o deps.png
```

```cmake
# 在 CMakeLists.txt 中调试
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

# 条件调试
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build enabled")
endif()
```

## 7. 集成实践

### 7.1 工具链集成

```cmake
# vcpkg 工具链
cmake -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

# Conan 包管理器
find_package(conan REQUIRED)
conan_cmake_run(REQUIRES fmt/10.0.0
                BASIC_SETUP
                BUILD missing)
```

### 7.2 CI/CD 配置

```yaml
# GitHub Actions 示例
name: Build
on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
        build_type: [Debug, Release]
    
    steps:
      - uses: actions/checkout@v4
      
      - name: Configure CMake
        run: cmake -B build -DCMAKE_BUILD_TYPE=${{ matrix.build_type }}
      
      - name: Build
        run: cmake --build build --config ${{ matrix.build_type }}
      
      - name: Test
        run: cd build && ctest -C ${{ matrix.build_type }} --output-on-failure
```

### 7.3 实战案例

```cmake
# 完整项目示例
cmake_minimum_required(VERSION 3.16)
project(MyLibrary VERSION 1.0.0 LANGUAGES CXX)

# 选项
option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 库目标
add_library(mylib
    src/core.cpp
    src/utils.cpp
)
target_include_directories(mylib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(mylib PUBLIC cxx_std_17)

# 版本信息
target_compile_definitions(mylib PRIVATE
    MYLIB_VERSION="${PROJECT_VERSION}"
)

# 测试
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 示例
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 安装
install(TARGETS mylib
    EXPORT mylibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT mylibTargets
    FILE mylibTargets.cmake
    NAMESPACE mylib::
    DESTINATION lib/cmake/mylib
)
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| 官方文档 | https://cmake.org/documentation |
| 命令参考 | https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html |
| 变量参考 | https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html |
| 模块参考 | https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html |

### 8.2 学习路径

| 阶段 | 内容 | 推荐资源 |
|------|------|----------|
| 入门 | 基础命令、项目结构 | CMake Tutorial (官方) |
| 进阶 | target 属性、生成器表达式 | Modern CMake (gitbook) |
| 高级 | 自定义模块、打包、测试 | Professional CMake |
| 实战 | 大型项目配置 | 开源项目 CMakeLists.txt |

### 8.3 社区资源

- **Modern CMake**: https://cliutils.gitlab.io/modern-cmake
- **CMake 最佳实践**: https://github.com/cpp-best-practices/cppbestpractices
- **CGold**: https://cgold.readthedocs.io
- **CMake Community Wiki**: https://gitlab.kitware.com/cmake/community/-/wikis/Home