# CPM.cmake - CMake 包管理器完整指南

## 1. 概述与背景

### 1.1 工具定位

CPM.cmake 是一个轻量级的 CMake 包管理器，专为 C/C++ 项目设计。它无需安装额外工具，通过单个 CMake 脚本文件即可管理项目依赖，简化了依赖管理流程。

**核心特点：**
- 零依赖：仅需 CMake 3.14+，无需额外工具链
- 轻量级：单文件实现，约 1000 行代码
- 声明式：在 CMakeLists.txt 中声明式管理依赖
- Git 优先：原生支持 Git 仓库作为依赖源
- 缓存机制：自动缓存下载的依赖，避免重复下载

### 1.2 发展历史

| 年份 | 版本 | 重要特性 |
|------|------|---------|
| 2019 | 0.1 | 项目启动，基础依赖管理 |
| 2020 | 0.15 | 添加源码缓存支持 |
| 2021 | 0.25 | 支持本地包开发模式 |
| 2022 | 0.30 | 优化下载性能，添加更多选项 |
| 2023 | 0.38 | 稳定版本，支持 CMake 3.27+ |

### 1.3 核心特性

**1. 声明式依赖管理**

```cmake
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)
```

**2. 多种依赖源支持**
- Git 仓库（GitHub、GitLab、自定义 Git）
- 压缩包 URL
- 本地目录

**3. 智能缓存机制**
- 下载的依赖缓存到本地目录
- 支持多项目共享缓存
- 离线构建支持

**4. 开发友好**
- 支持本地包开发模式
- 可覆盖依赖源码路径
- 简化依赖调试流程

### 1.4 适用场景

| 场景 | 适用性 | 说明 |
|------|--------|------|
| 小型项目 | ✅ 高度推荐 | 无需复杂配置即可管理依赖 |
| 跨平台项目 | ✅ 推荐 | CMake 原生跨平台支持 |
| CI/CD 环境 | ✅ 推荐 | 无需预装包管理工具 |
| 本地库开发 | ✅ 推荐 | 支持本地源码覆盖 |
| 大型项目 | ⚠️ 谨慎使用 | 无二进制缓存可能影响构建时间 |
| 企业环境 | ⚠️ 视情况 | 需考虑网络访问和缓存策略 |

### 1.5 对比分析

| 特性 | CPM.cmake | vcpkg | Conan |
|------|-----------|-------|-------|
| 外部工具依赖 | ❌ 不需要 | ✅ 需要 | ✅ 需要 |
| 学习曲线 | 低 | 中 | 中 |
| 安装复杂度 | 极低 | 中 | 中 |
| 二进制缓存 | ❌ | ✅ | ✅ |
| 包数量 | 无限制（Git） | 2000+ | 1500+ |
| CI 集成 | 简单 | 简单 | 灵活 |
| 离线构建 | ✅ 支持 | 需配置 | 需配置 |
| 多版本共存 | ❌ | ❌ | ✅ |
| 依赖隔离 | 项目级 | 系统级 | 项目级 |

**选择建议：**
- **选择 CPM**：小型项目、快速原型、不想安装额外工具、主要依赖 Git 托管的库
- **选择 vcpkg**：需要二进制缓存、大量官方包、MSVC 开发环境
- **选择 Conan**：多版本依赖、企业级项目、需要高级依赖管理功能

## 2. 安装与配置

### 2.1 多平台安装

**方式一：下载到项目中（推荐）**

```bash
# 在项目根目录创建 cmake 目录
mkdir cmake

# 下载 CPM.cmake
wget -O cmake/CPM.cmake \
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.0/CPM.cmake
```

**方式二：自动下载（FetchContent）**

```cmake
# CMakeLists.txt 开头
cmake_minimum_required(VERSION 3.20)
project(MyProject)

include(FetchContent)
FetchContent_Declare(
  cpm
  URL https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.0/CPM.cmake
)
FetchContent_MakeAvailable(cpm)
```

**方式三：Git 子模块**

```bash
# 添加为子模块
git submodule add https://github.com/cpm-cmake/CPM.cmake.git cmake/CPM.cmake

# 在 CMakeLists.txt 中引用
include(cmake/CPM.cmake/CPM.cmake)
```

### 2.2 版本管理

**版本选择策略：**

| CMake 版本 | 推荐 CPM 版本 | 说明 |
|------------|---------------|------|
| 3.14 - 3.19 | 0.30+ | 最低要求 |
| 3.20 - 3.25 | 0.35+ | 推荐 |
| 3.26+ | 0.38+ | 最新特性支持 |

**版本更新：**

```bash
# 更新到最新版本
wget -O cmake/CPM.cmake \
  https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake

# 更新到指定版本
wget -O cmake/CPM.cmake \
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.2/CPM.cmake
```

### 2.3 环境配置

**缓存目录配置：**

```cmake
# 自定义缓存目录（可选）
set(CPM_SOURCE_CACHE "${CMAKE_BINARY_DIR}/cpm-cache")

# 或使用环境变量
if(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_SOURCE_CACHE $ENV{CPM_SOURCE_CACHE})
endif()
```

**默认缓存位置：**

```
~/.cache/CPM/              # Linux
~/Library/Caches/CPM/      # macOS
%LOCALAPPDATA%\CPM\        # Windows
```

### 2.4 验证安装

```cmake
# 在 CMakeLists.txt 中添加验证
include(cmake/CPM.cmake)

# 检查 CPM 是否正确加载
if(NOT COMMAND CPMAddPackage)
  message(FATAL_ERROR "CPM.cmake not loaded correctly")
endif()

# 打印 CPM 版本信息（如果需要）
message(STATUS "CPM_SOURCE_CACHE: ${CPM_SOURCE_CACHE}")
```

**命令行验证：**

```bash
cmake -B build
# 输出中应包含 CPM 相关信息
```

## 3. 基础使用

### 3.1 快速入门

**完整示例项目：**

```cmake
cmake_minimum_required(VERSION 3.20)
project(HelloCPM)

# 引入 CPM
include(cmake/CPM.cmake)

# 添加依赖：fmt 格式化库
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

# 添加依赖：Catch2 测试框架
CPMAddPackage(
  NAME Catch2
  GITHUB_REPOSITORY catchorg/Catch2
  GIT_TAG v3.4.0
)

# 创建可执行文件
add_executable(hello main.cpp)
target_link_libraries(hello fmt::fmt)

# 启用测试
enable_testing()
add_executable(tests test.cpp)
target_link_libraries(tests Catch2::Catch2WithMain)
```

**main.cpp：**

```cpp
#include <fmt/core.h>

int main() {
    fmt::print("Hello, CPM!\n");
    return 0;
}
```

### 3.2 项目结构

**推荐项目结构：**

```
myproject/
├── CMakeLists.txt
├── cmake/
│   └── CPM.cmake          # CPM 脚本
├── src/
│   └── main.cpp
├── include/
│   └── mylib.h
├── test/
│   └── test.cpp
└── README.md
```

**多目录项目示例：**

```cmake
cmake_minimum_required(VERSION 3.20)
project(MultiDirProject)

include(cmake/CPM.cmake)

# 公共依赖
CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  GIT_TAG v1.12.0
)

# 库目录
add_subdirectory(library)
# 程序目录
add_subdirectory(program)
# 测试目录
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  add_subdirectory(test)
endif()
```

### 3.3 基本命令

**CPMAddPackage 核心参数：**

```cmake
CPMAddPackage(
  # 必需参数
  NAME <package_name>              # 包名称

  # 来源选项（选择其一）
  GITHUB_REPOSITORY <owner/repo>   # GitHub 仓库
  GIT_REPOSITORY <url>             # Git 仓库 URL
  URL <url>                       # 压缩包 URL
  SOURCE_DIR <path>                # 本地目录

  # 版本选项
  GIT_TAG <tag>                    # Git 标签、分支或提交
  VERSION <version>                # 版本号

  # 可选参数
  OPTIONS "KEY=VALUE" ...          # CMake 选项
  EXCLUDE_FROM_ALL YES             # 排除在 ALL 之外
  DOWNLOAD_ONLY YES                # 仅下载不配置
)
```

**示例：添加多个依赖**

```cmake
# 1. Git 仓库（标签）
CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  GIT_TAG v3.11.2
)

# 2. Git 仓库（分支）
CPMAddPackage(
  NAME mylib
  GITHUB_REPOSITORY user/mylib
  GIT_TAG main
)

# 3. Git 仓库（特定提交）
CPMAddPackage(
  NAME experimental
  GITHUB_REPOSITORY user/exp
  GIT_TAG a1b2c3d4e5f6
)

# 4. URL 下载
CPMAddPackage(
  NAME googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz
)

# 5. 本地目录
CPMAddPackage(
  NAME local_lib
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/../local_lib
)
```

### 3.4 常用操作

**带选项添加包：**

```cmake
CPMAddPackage(
  NAME boost
  GITHUB_REPOSITORY boostorg/boost
  GIT_TAG boost-1.83.0
  OPTIONS
    "BUILD_SHARED_LIBS ON"
    "BOOST_ENABLE_CMAKE ON"
    "BUILD_TESTING OFF"
)
```

**排除不必要的组件：**

```cmake
CPMAddPackage(
  NAME opencv
  GITHUB_REPOSITORY opencv/opencv
  GIT_TAG 4.8.0
  OPTIONS
    "BUILD_TESTS OFF"
    "BUILD_PERF_TESTS OFF"
    "BUILD_EXAMPLES OFF"
    "BUILD_opencv_python3 OFF"
)
```

**条件添加依赖：**

```cmake
option(USE_FMT "Use fmt library" ON)

if(USE_FMT)
  CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 9.1.0
  )
endif()
```

## 4. 进阶特性

### 4.1 高级配置

**本地包开发模式**

在开发依赖库时，无需每次修改后重新下载，使用本地源码：

```cmake
# 方式一：环境变量覆盖
if(DEFINED ENV{MYLIB_SOURCE_DIR})
  set(MYLIB_SOURCE_DIR $ENV{MYLIB_SOURCE_DIR})
else()
  CPMAddPackage(
    NAME mylib
    GITHUB_REPOSITORY user/mylib
    GIT_TAG main
  )
endif()

# 方式二：CPM 本地包模式
CPMAddPackage(
  NAME mylib
  GITHUB_REPOSITORY user/mylib
  GIT_TAG main
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/../mylib  # 优先使用本地
)
```

**开发环境配置脚本：**

```bash
#!/bin/bash
# dev-setup.sh

export MYLIB_SOURCE_DIR=/path/to/local/mylib
export ANOTHERLIB_SOURCE_DIR=/path/to/local/anotherlib

cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

**包版本约束：**

```cmake
# 使用 FindPackage 模式
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
  FIND_PACKAGE_ARGUMENTS "REQUIRED"
)

# 添加版本检查
find_package(fmt 9.0 REQUIRED)
if(fmt_FOUND)
  message(STATUS "Found fmt: ${fmt_VERSION}")
endif()
```

### 4.2 扩展功能

**自定义下载命令：**

```cmake
# 使用自定义下载选项
CPMAddPackage(
  NAME large_lib
  GIT_REPOSITORY https://internal.company.com/large_lib.git
  GIT_TAG v2.0.0
  GIT_SHALLOW TRUE        # 浅克隆
  GIT_PROGRESS TRUE       # 显示进度
)
```

**依赖冲突解决：**

```cmake
# 禁用依赖的依赖
CPMAddPackage(
  NAME libA
  GITHUB_REPOSITORY user/libA
  GIT_TAG v1.0.0
  EXCLUDE_FROM_ALL YES    # 不构建默认目标
)

CPMAddPackage(
  NAME libB
  GITHUB_REPOSITORY user/libB
  GIT_TAG v1.0.0
  EXCLUDE_FROM_ALL YES
)

# 显式链接需要的库
add_executable(myapp main.cpp)
target_link_libraries(myapp libA::libA)
```

**下载仅头文件库：**

```cmake
CPMAddPackage(
  NAME stb
  GITHUB_REPOSITORY nothings/stb
  GIT_TAG master
  DOWNLOAD_ONLY YES
)

# 手动添加头文件目录
add_executable(myapp main.cpp)
target_include_directories(myapp PRIVATE ${stb_SOURCE_DIR})
```

### 4.3 插件生态

**CPM 与其他工具集成：**

```cmake
# 与 vcpkg 混用
include(cmake/CPM.cmake)

# 使用 vcpkg 的包
find_package(Boost REQUIRED COMPONENTS filesystem)

# 使用 CPM 的包
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)
```

**自定义 CPM 模块：**

```cmake
# cmake/CPMCustom.cmake
function(CPMAddPackageFromGitHub name repo tag)
  CPMAddPackage(
    NAME ${name}
    GITHUB_REPOSITORY ${repo}
    GIT_TAG ${tag}
  )
endfunction()

# 使用自定义函数
include(cmake/CPMCustom.cmake)
CPMAddPackageFromGitHub(fmt fmtlib/fmt 9.1.0)
```

## 5. 性能优化

### 5.1 调优策略

**缓存优化：**

```cmake
# 1. 统一缓存目录
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpm-cache")

# 2. 使用浅克隆减少下载时间
set(CPM_USE_LATEST_TAG ON)  # 使用最新标签

# 3. 并行下载（需要 CMake 3.16+）
set(CPM_DOWNLOAD_ALL ON)    # 强制下载所有依赖
```

**减少重新配置：**

```cmake
# 使用条件检查避免重复下载
if(NOT TARGET fmt::fmt)
  CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 9.1.0
  )
endif()
```

**构建优化：**

```cmake
# 禁用不需要的组件加速构建
CPMAddPackage(
  NAME opencv
  GITHUB_REPOSITORY opencv/opencv
  GIT_TAG 4.8.0
  OPTIONS
    "BUILD_SHARED_LIBS ON"         # 共享库更快
    "BUILD_TESTS OFF"
    "BUILD_PERF_TESTS OFF"
    "BUILD_EXAMPLES OFF"
    "BUILD_DOCS OFF"
    "BUILD_JAVA OFF"
    "BUILD_opencv_python3 OFF"
)
```

### 5.2 最佳实践

**实践一：版本锁定**

```cmake
# 创建 dependencies.cmake 统一管理版本
# cmake/dependencies.cmake

set(FMT_VERSION "9.1.0")
set(SPDLOG_VERSION "1.12.0")
set(CATCH2_VERSION "3.4.0")

# 使用时
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG ${FMT_VERSION}
)
```

**实践二：可选依赖管理**

```cmake
# 声明选项
option(ENABLE_TESTS "Enable tests" ON)
option(ENABLE_BENCHMARKS "Enable benchmarks" OFF)
option(ENABLE_DOCS "Enable documentation" OFF)

# 条件添加
if(ENABLE_TESTS)
  CPMAddPackage(NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    GIT_TAG v${CATCH2_VERSION}
  )
endif()

if(ENABLE_BENCHMARKS)
  CPMAddPackage(NAME benchmark
    GITHUB_REPOSITORY google/benchmark
    GIT_TAG v1.8.2
  )
endif()
```

**实践三：依赖隔离**

```cmake
# 使用 EXCLUDE_FROM_ALL 避免构建不需要的目标
CPMAddPackage(
  NAME some_lib
  GITHUB_REPOSITORY user/some_lib
  GIT_TAG v1.0.0
  EXCLUDE_FROM_ALL YES
)

# 仅当需要时构建
add_custom_target(build_some_lib
  COMMAND ${CMAKE_COMMAND} --build . --target some_lib
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
```

## 6. 问题排查

### 6.1 常见问题

**问题一：下载失败**

```
CMake Error: Failed to download package
```

解决方案：

```cmake
# 1. 使用镜像源
CPMAddPackage(
  NAME fmt
  GIT_REPOSITORY https://gitee.com/mirrors/fmt.git
  GIT_TAG 9.1.0
)

# 2. 使用 URL 作为备选
CPMAddPackage(
  NAME fmt
  URL https://github.com/fmtlib/fmt/releases/download/9.1.0/fmt-9.1.0.zip
)
```

**问题二：版本冲突**

```
CMake Error: Target "fmt" already exists
```

解决方案：

```cmake
# 检查目标是否已存在
if(NOT TARGET fmt::fmt)
  CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 9.1.0
  )
endif()
```

**问题三：缓存损坏**

```
CMake Error: Cache directory corrupted
```

解决方案：

```bash
# 清理缓存
rm -rf ~/.cache/CPM/*
rm -rf build/
cmake -B build
```

**问题四：CMake 版本过低**

```
CMake Error: CMake version too old
```

解决方案：

```cmake
# 在 CMakeLists.txt 开头指定最低版本
cmake_minimum_required(VERSION 3.20)  # 使用 CPM 需要 3.14+
```

### 6.2 调试技巧

**启用详细输出：**

```cmake
# 在 CMakeLists.txt 中启用调试
set(CPM_DEBUG ON)

# 查看缓存位置
message(STATUS "CPM cache: ${CPM_SOURCE_CACHE}")

# 查看包信息
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)
message(STATUS "fmt source: ${fmt_SOURCE_DIR}")
message(STATUS "fmt binary: ${fmt_BINARY_DIR}")
```

**检查依赖状态：**

```cmake
# 创建依赖检查目标
add_custom_target(check_deps
  COMMAND ${CMAKE_COMMAND} -E echo "Dependencies:"
  COMMAND ${CMAKE_COMMAND} -E echo "  fmt: ${fmt_SOURCE_DIR}"
  COMMAND ${CMAKE_COMMAND} -E echo "  spdlog: ${spdlog_SOURCE_DIR}"
  VERBATIM
)
```

**导出依赖信息：**

```cmake
# 生成依赖报告
set(DEP_FILE "${CMAKE_BINARY_DIR}/dependencies.txt")
file(WRITE ${DEP_FILE} "Project Dependencies\n")
file(APPEND ${DEP_FILE} "==================\n")
file(APPEND ${DEP_FILE} "fmt: 9.1.0\n")
file(APPEND ${DEP_FILE} "spdlog: 1.12.0\n")
```

## 7. 集成实践

### 7.1 工具链集成

**与 IDE 集成：**

```cmake
# CMakeLists.txt - 支持 IDE
cmake_minimum_required(VERSION 3.20)
project(MyProject)

# IDE 友好设置
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

include(cmake/CPM.cmake)

# 添加依赖
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

add_executable(myapp src/main.cpp)
target_link_libraries(myapp fmt::fmt)
```

**与 vcpkg 混用：**

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(MixedProject)

# vcpkg 工具链
if(DEFINED CMAKE_TOOLCHAIN_FILE)
  include(${CMAKE_TOOLCHAIN_FILE})
endif()

# CPM
include(cmake/CPM.cmake)

# vcpkg 提供的包
find_package(Boost REQUIRED COMPONENTS filesystem system)

# CPM 提供的包
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

add_executable(app src/main.cpp)
target_link_libraries(app
  Boost::filesystem
  Boost::system
  fmt::fmt
)
```

### 7.2 CI/CD 配置

**GitHub Actions 配置：**

```yaml
# .github/workflows/ci.yml
name: CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]

    steps:
      - uses: actions/checkout@v3

      - name: Setup CMake
        uses: jwlawson/actions-setup-cmake@v1
        with:
          cmake-version: '3.25'

      - name: Configure
        run: cmake -B build

      - name: Build
        run: cmake --build build --config Release

      - name: Test
        run: ctest --test-dir build -C Release
```

**GitLab CI 配置：**

```yaml
# .gitlab-ci.yml
stages:
  - build
  - test

cache:
  paths:
    - .cpm-cache/

build:
  stage: build
  image: gcc:latest
  script:
    - apt-get update && apt-get install -y cmake
    - cmake -B build -DCPM_SOURCE_CACHE=$PWD/.cpm-cache
    - cmake --build build
  artifacts:
    paths:
      - build/

test:
  stage: test
  image: gcc:latest
  script:
    - ctest --test-dir build
  dependencies:
    - build
```

**缓存优化配置：**

```yaml
# GitHub Actions 带缓存的配置
name: CI with Cache

on: [push]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v3

      - name: Cache CPM
        uses: actions/cache@v3
        with:
          path: ~/.cache/CPM
          key: ${{ runner.os }}-cpm-${{ hashFiles('**/CMakeLists.txt') }}
          restore-keys: |
            ${{ runner.os }}-cpm-

      - name: Build
        run: |
          cmake -B build
          cmake --build build
```

### 7.3 实战案例

**案例一：跨平台项目配置**

```cmake
cmake_minimum_required(VERSION 3.20)
project(CrossPlatformApp)

include(cmake/CPM.cmake)

# 平台检测
if(WIN32)
  set(PLATFORM_LIBS "")
elseif(APPLE)
  set(PLATFORM_LIBS "")
elseif(UNIX)
  set(PLATFORM_LIBS "pthread")
endif()

# 添加跨平台依赖
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  GIT_TAG v1.12.0
  OPTIONS
    "SPDLOG_FMT_EXTERNAL ON"
)

# 构建可执行文件
add_executable(myapp
  src/main.cpp
  src/logger.cpp
)

target_link_libraries(myapp
  fmt::fmt
  spdlog::spdlog
  ${PLATFORM_LIBS}
)

# 平台特定设置
if(WIN32)
  target_compile_definitions(myapp PRIVATE _WIN32_WINNT=0x0601)
elseif(APPLE)
  set_target_properties(myapp PROPERTIES
    MACOSX_BUNDLE TRUE
  )
endif()
```

**案例二：模块化项目结构**

```cmake
# 根目录 CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(ModularProject)

include(cmake/CPM.cmake)
include(cmake/dependencies.cmake)  # 集中管理依赖

add_subdirectory(core)
add_subdirectory(network)
add_subdirectory(app)
```

```cmake
# cmake/dependencies.cmake
# 核心依赖
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  GIT_TAG v1.12.0
  OPTIONS "SPDLOG_FMT_EXTERNAL ON"
)

# 网络依赖
if(ENABLE_NETWORK)
  CPMAddPackage(
    NAME asio
    GITHUB_REPOSITORY chriskohlhoff/asio
    GIT_TAG asio-1.28.0
    OPTIONS "ASIO_STANDALONE ON"
  )
endif()

# 测试依赖
if(BUILD_TESTING)
  CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    GIT_TAG v3.4.0
  )
endif()
```

```cmake
# core/CMakeLists.txt
add_library(core
  logger.cpp
  config.cpp
)

target_link_libraries(core
  PUBLIC fmt::fmt
  PUBLIC spdlog::spdlog
)

target_include_directories(core
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

```cmake
# app/CMakeLists.txt
add_executable(myapp main.cpp)

target_link_libraries(myapp
  PRIVATE core
  PRIVATE $<BUILD_INTERFACE:network>
)
```

**案例三：企业级项目配置**

```cmake
cmake_minimum_required(VERSION 3.20)
project(EnterpriseApp VERSION 1.0.0)

include(cmake/CPM.cmake)

# 企业内部仓库支持
function(CPMAddPrivatePackage name repo tag)
  CPMAddPackage(
    NAME ${name}
    GIT_REPOSITORY https://git.company.com/libs/${repo}
    GIT_TAG ${tag}
    OPTIONS
      "COMPANY_LICENSE_CHECK ON"
  )
endfunction()

# 外部依赖
CPMAddPackage(NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

# 内部依赖
CPMAddPrivatePackage(company_lib company-lib v2.1.0)

# 构建配置
add_executable(enterprise_app
  src/main.cpp
  src/api.cpp
  src/db.cpp
)

target_link_libraries(enterprise_app
  PRIVATE fmt::fmt
  PRIVATE company_lib::company_lib
)

# 安装配置
install(TARGETS enterprise_app
  RUNTIME DESTINATION bin
)

install(FILES config/app.conf
  DESTINATION etc/enterprise_app
)
```

## 8. 参考资源

### 8.1 官方文档

- **GitHub 仓库**：https://github.com/cpm-cmake/CPM.cmake
- **官方文档**：https://cpm-cmake.readthedocs.io/
- **示例项目**：https://github.com/cpm-cmake/CPM.cmake/tree/master/examples

### 8.2 学习路径

**初级阶段（1-2 天）：**
1. 安装 CPM.cmake 到项目
2. 学习基本 CPMAddPackage 用法
3. 添加 2-3 个常用库依赖
4. 完成项目构建和测试

**中级阶段（3-5 天）：**
1. 学习本地包开发模式
2. 掌握选项配置技巧
3. 配置 CI/CD 环境
4. 实践缓存优化策略

**高级阶段（1 周+）：**
1. 深入理解 CPM 内部机制
2. 自定义扩展功能
3. 与其他工具链集成
4. 企业级项目实践

### 8.3 相关工具

| 工具 | 说明 |
|------|------|
| CMake | CPM 的基础构建系统 |
| vcpkg | 微软 C++ 包管理器 |
| Conan | C/C++ 包管理器 |
| FetchContent | CMake 内置依赖管理 |
| ExternalProject | CMake 外部项目模块 |

### 8.4 推荐库列表

| 库名 | GitHub | 说明 |
|------|---------|------|
| fmt | fmtlib/fmt | 格式化库 |
| spdlog | gabime/spdlog | 日志库 |
| Catch2 | catchorg/Catch2 | 测试框架 |
| nlohmann_json | nlohmann/json | JSON 库 |
| asio | chriskohlhoff/asio | 网络库 |
|CLI11 | CLIUtils/CLI11 | 命令行解析 |

---

**总结：** CPM.cmake 是一个简洁高效的 CMake 包管理器，适合小型到中型 C/C++ 项目。其零依赖、声明式的特点使其成为 CI/CD 环境和跨平台项目的理想选择。通过合理使用缓存、版本锁定和本地开发模式，可以显著提升开发效率。