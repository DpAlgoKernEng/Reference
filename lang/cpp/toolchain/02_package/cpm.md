# CPM - CMake 包管理器详细指南

## 1. 概述与背景

### 1.1 工具定位

CPM.cmake 是一个轻量级、零依赖的 CMake 包管理器，其核心理念是**将依赖管理完全集成到 CMake 构建系统中**，无需额外安装任何工具或运行时环境。它通过 CMake 的 FetchContent 模块实现依赖获取，让开发者能够像管理普通 CMake 项目一样管理第三方库。

### 1.2 发展历史

| 年份 | 版本 | 里程碑事件 |
|------|------|-----------|
| 2019 | 0.1 | 项目启动，基于 FetchContent 封装 |
| 2020 | 0.15 | 添加本地包开发支持 |
| 2021 | 0.27 | 改进缓存机制，支持源码缓存 |
| 2022 | 0.35 | 支持 CMake 3.14+ 最低版本 |
| 2023 | 0.38 | 完善依赖解析，优化性能 |

CPM.cmake 由 CPM-cmake 社区维护，目前在 GitHub 上拥有 1500+ stars，是中小型 C++ 项目依赖管理的热门选择。

### 1.3 核心特性

| 特性 | 描述 | 优势 |
|------|------|------|
| **零依赖** | 仅需 CMake 3.14+ | 无需安装包管理器本身 |
| **声明式配置** | 在 CMakeLists.txt 中声明 | 配置即文档 |
| **智能缓存** | 本地缓存下载的依赖 | 避免重复下载 |
| **本地开发** | 支持使用本地源码 | 方便调试和开发 |
| **版本灵活** | 支持 Git 标签、分支、提交 | 精确控制依赖版本 |
| **CI 友好** | 无外部依赖 | CI 配置简单 |

### 1.4 适用场景

| 场景 | 适用性 | 理由 |
|------|--------|------|
| 小型开源项目 | ✅ 高度推荐 | 配置简单，无需额外工具 |
| 团队内部项目 | ✅ 推荐 | 易于上手，学习成本低 |
| 跨平台项目 | ✅ 推荐 | CMake 原生支持 |
| 教学示例项目 | ✅ 推荐 | 用户体验友好 |
| 大型企业项目 | ⚠️ 需评估 | 缺少二进制缓存可能影响构建速度 |
| 需要二进制缓存 | ❌ 不推荐 | 使用 vcpkg 或 Conan |

### 1.5 对比分析

| 特性 | CPM.cmake | vcpkg | Conan | FetchContent |
|------|-----------|-------|-------|--------------|
| **外部工具** | ❌ 不需要 | ✅ 需要 | ✅ 需要 | ❌ 不需要 |
| **学习曲线** | 低 | 中 | 中 | 低 |
| **二进制缓存** | ❌ | ✅ | ✅ | ❌ |
| **包数量** | 无限制* | 2000+ | 1500+ | 无限制* |
| **版本控制** | Git | 自有系统 | 自有系统 | Git |
| **离线构建** | ⚠️ 需缓存 | ✅ | ✅ | ⚠️ 需缓存 |
| **依赖解析** | 简单 | 完善 | 完善 | 无 |
| **CI 集成** | 简单 | 简单 | 灵活 | 简单 |

*CPM.cmake 和 FetchContent 可以从任何 Git 仓库或 URL 获取包，不限于中央仓库。

**选择建议：**

- **选择 CPM.cmake**：小型项目、不想安装额外工具、需要简单配置
- **选择 vcpkg**：Windows 开发、需要二进制缓存、微软生态
- **选择 Conan**：复杂依赖关系、需要二进制缓存、企业级项目

## 2. 安装与配置

### 2.1 多平台安装

CPM.cmake 本身无需安装，只需将 `CPM.cmake` 文件包含到项目中即可。以下是三种推荐的集成方式：

**方式 1：直接下载到项目（推荐）**

```bash
# 创建 cmake 目录
mkdir -p cmake

# 下载 CPM.cmake
wget -O cmake/CPM.cmake \
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.0/CPM.cmake

# 或使用 curl
curl -Lo cmake/CPM.cmake \
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.0/CPM.cmake
```

**方式 2：使用 FetchContent 自动下载**

```cmake
# CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
  cpm
  URL https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.0/CPM.cmake
)
FetchContent_MakeAvailable(cpm)
include(${cpm_SOURCE_DIR}/CPM.cmake)
```

**方式 3：Git 子模块**

```bash
git submodule add https://github.com/cpm-cmake/CPM.cmake.git cmake/CPM.cmake

# 更新
git submodule update --remote
```

### 2.2 版本管理

| CPM 版本 | CMake 最低版本 | 推荐使用场景 |
|---------|---------------|-------------|
| 0.38.x | 3.14 | 最新稳定版，推荐 |
| 0.35.x | 3.14 | 稳定版 |
| 0.27.x | 3.14 | 旧项目兼容 |
| 开发版 | 3.20 | 新特性测试 |

**版本选择建议：**

```cmake
# 使用稳定版本（推荐）
https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.0/CPM.cmake

# 使用特定提交（开发测试）
https://github.com/cpm-cmake/CPM.cmake/archive/refs/heads/master.zip
```

### 2.3 环境配置

CPM.cmake 支持以下环境变量配置：

```bash
# 设置缓存目录（可选）
export CPM_SOURCE_CACHE=~/.cache/CPM  # Linux/macOS
set CPM_SOURCE_CACHE=%LOCALAPPDATA%\CPM  # Windows (CMD)
$env:CPM_SOURCE_CACHE="$env:LOCALAPPDATA\CPM"  # Windows (PowerShell)

# 禁用缓存（调试用）
export CPM_USE_NAMED_CACHE_DIRECTORIES=OFF

# 本地开发模式
export MYLIB_SOURCE_DIR=/path/to/mylib/local/source
```

### 2.4 验证安装

创建测试项目验证 CPM.cmake 是否正常工作：

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(cpm_test)

include(cmake/CPM.cmake)

# 添加测试依赖
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

add_executable(test_cpm main.cpp)
target_link_libraries(test_cpm fmt::fmt)
```

```cpp
// main.cpp
#include <fmt/core.h>

int main() {
    fmt::print("CPM.cmake works!\n");
    return 0;
}
```

```bash
# 构建和运行
cmake -B build
cmake --build build
./build/test_cpm  # 输出: CPM.cmake works!
```

## 3. 基础使用

### 3.1 快速入门

一个完整的 CPM.cmake 项目结构示例：

```
my_project/
├── CMakeLists.txt
├── cmake/
│   └── CPM.cmake
├── src/
│   └── main.cpp
└── test/
    └── test_main.cpp
```

**CMakeLists.txt 配置示例：**

```cmake
cmake_minimum_required(VERSION 3.14)
project(my_project VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含 CPM.cmake
include(cmake/CPM.cmake)

# 添加依赖
CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:catchorg/Catch2@3.4.0")

# 主程序
add_executable(my_app src/main.cpp)
target_link_libraries(my_app fmt::fmt)

# 测试
enable_testing()
CPMAddPackage("gh:catchorg/Catch2@3.4.0")
add_executable(tests test/test_main.cpp)
target_link_libraries(tests Catch2::Catch2WithMain)
```

### 3.2 项目结构

**推荐的项目目录结构：**

```
project/
├── CMakeLists.txt           # 主配置文件
├── cmake/
│   ├── CPM.cmake           # CPM.cmake 脚本
│   ├── Dependencies.cmake  # 依赖声明（可选）
│   └── CompilerOptions.cmake  # 编译选项（可选）
├── src/
│   ├── CMakeLists.txt      # 源码配置
│   └── *.cpp
├── include/
│   └── *.h
├── test/
│   ├── CMakeLists.txt      # 测试配置
│   └── *.cpp
└── third_party/            # 本地第三方库（可选）
```

**依赖集中管理（Dependencies.cmake）：**

```cmake
# cmake/Dependencies.cmake
include(CPM.cmake)

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  GIT_TAG v1.12.0
)

CPMAddPackage(
  NAME catch2
  GITHUB_REPOSITORY catchorg/Catch2
  VERSION 3.4.0
)
```

### 3.3 基本命令

CPM.cmake 提供的核心命令：`CPMAddPackage`

**命令格式：**

```cmake
CPMAddPackage(
  NAME <包名>
  [GITHUB_REPOSITORY <owner/repo>]
  [GIT_TAG <tag|branch|commit>]
  [VERSION <版本号>]
  [URL <下载地址>]
  [SOURCE_DIR <本地路径>]
  [OPTIONS "选项1=值" "选项2=值"]
  [EXCLUDE_FROM_ALL YES]
  [NO_UPDATE YES]
)
```

**常用参数说明：**

| 参数 | 必需 | 描述 | 示例 |
|------|------|------|------|
| NAME | ✅ | 包名称 | `NAME fmt` |
| GITHUB_REPOSITORY | ❌ | GitHub 仓库 | `GITHUB_REPOSITORY fmtlib/fmt` |
| GIT_TAG | ❌ | Git 标签/分支/提交 | `GIT_TAG v9.1.0` |
| VERSION | ❌ | 版本号 | `VERSION 9.1.0` |
| URL | ❌ | 直接下载地址 | `URL https://...tar.gz` |
| SOURCE_DIR | ❌ | 本地源码路径 | `SOURCE_DIR /path/to/src` |
| OPTIONS | ❌ | CMake 选项 | `OPTIONS "BUILD_TESTS OFF"` |
| EXCLUDE_FROM_ALL | ❌ | 排除默认构建 | `EXCLUDE_FROM_ALL YES` |

### 3.4 常用操作

**操作 1：添加 GitHub 包**

```cmake
# 简写形式
CPMAddPackage("gh:fmtlib/fmt#9.1.0")

# 完整形式
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)
```

**操作 2：添加 URL 包**

```cmake
CPMAddPackage(
  NAME catch2
  URL https://github.com/catchorg/Catch2/releases/download/v3.4.0/catch2-3.4.0.tar.gz
  URL_HASH MD5=abc123...  # 可选，验证下载完整性
)
```

**操作 3：使用本地包**

```cmake
# 开发模式：使用本地源码
if(DEFINED ENV{MYLIB_SOURCE_DIR})
  set(MYLIB_SOURCE_DIR $ENV{MYLIB_SOURCE_DIR})
endif()

CPMAddPackage(
  NAME mylib
  SOURCE_DIR ${MYLIB_SOURCE_DIR}
  OPTIONS
    "MYLIB_BUILD_TESTS OFF"
    "MYLIB_SHARED ON"
)
```

**操作 4：条件依赖**

```cmake
option(USE_SPDLOG "Use spdlog for logging" ON)

if(USE_SPDLOG)
  CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.12.0
  )
  target_link_libraries(my_app spdlog::spdlog)
endif()
```

## 4. 进阶特性

### 4.1 高级配置

**版本指定方式详解：**

```cmake
# 1. Git 标签（推荐）
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG v9.1.0  # 或 9.1.0（不带 v 前缀）
)

# 2. Git 分支
CPMAddPackage(
  NAME mylib
  GITHUB_REPOSITORY user/mylib
  GIT_TAG main  # 或 develop、feature-branch
)

# 3. Git 提交哈希
CPMAddPackage(
  NAME mylib
  GITHUB_REPOSITORY user/mylib
  GIT_TAG a1b2c3d4e5f6...  # 完整或短哈希
)

# 4. 版本号（自动查找标签）
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 9.1.0  # 自动匹配 v9.1.0 或 9.1.0 标签
)

# 5. 最新版本
CPMAddPackage(
  NAME mylib
  GITHUB_REPOSITORY user/mylib
  GIT_TAG latest  # 使用最新 release
)
```

**依赖选项配置：**

```cmake
CPMAddPackage(
  NAME boost
  GITHUB_REPOSITORY boostorg/boost
  GIT_TAG boost-1.84.0
  OPTIONS
    "BOOST_ENABLE_CMAKE ON"
    "BUILD_TESTING OFF"
    "BUILD_EXAMPLES OFF"
  EXCLUDE_FROM_ALL YES  # 只构建需要的组件
)
```

### 4.2 扩展功能

**覆盖依赖版本：**

```cmake
# 在项目根目录创建 CPMOverrides.cmake
# 内容：
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY myfork/fmt
  GIT_TAG my-custom-branch
)

# 在 CMakeLists.txt 中加载覆盖文件
include(cmake/CPM.cmake)
if(EXISTS "${CMAKE_SOURCE_DIR}/CPMOverrides.cmake")
  include(CPMOverrides.cmake)
endif()
```

**禁用下载（离线构建）：**

```cmake
# 强制使用本地缓存，不从网络下载
set(CPM_DOWNLOAD_ALL OFF CACHE BOOL "Disable downloads")

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
  NO_UPDATE YES  # 不更新
)
```

**依赖版本冲突解决：**

```cmake
# 如果多个依赖要求同一个库的不同版本
# CPM 会使用第一个找到的版本
CPMAddPackage(
  NAME json
  GITHUB_REPOSITORY nlohmann/json
  VERSION 3.11.0
)

# 后续对 json 的依赖请求会被忽略
# 可以通过 CPM_FORCE_PACKAGE 强制使用特定版本
```

### 4.3 插件生态

CPM.cmake 本身没有插件系统，但可以与以下工具配合使用：

| 工具 | 用途 | 集成方式 |
|------|------|---------|
| **CMake Presets** | 标准化构建配置 | 共享缓存变量 |
| **vcpkg** | 补充二进制依赖 | 先 vcpkg install，再 CPM |
| **Conan** | 复杂依赖管理 | Conan 提供，CPM 消费 |
| **ccache** | 编译缓存 | 环境变量配置 |
| **ninja** | 快速构建 | CMake 生成器 |

**CMake Presets 集成示例：**

```json
// CMakePresets.json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "default",
      "cacheVariables": {
        "CPM_SOURCE_CACHE": "${sourceDir}/.cache/cpm",
        "CMAKE_BUILD_TYPE": "Release"
      }
    }
  ]
}
```

## 5. 性能优化

### 5.1 调优策略

**策略 1：合理使用缓存**

```cmake
# 设置全局缓存目录
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cache/cpm")

# 或使用环境变量
if(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_SOURCE_CACHE $ENV{CPM_SOURCE_CACHE})
else()
  set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cache/cpm")
endif()
```

**缓存目录位置建议：**

| 平台 | 推荐路径 | 说明 |
|------|---------|------|
| Linux | `~/.cache/CPM` | 符合 XDG 标准 |
| macOS | `~/Library/Caches/CPM` | 符合 macOS 标准 |
| Windows | `%LOCALAPPDATA%\CPM` | 符合 Windows 标准 |
| 项目级 | `项目/.cache/cpm` | 团队共享，CI 友好 |

**策略 2：使用 EXCLUDE_FROM_ALL**

```cmake
# 只构建需要的组件，避免编译不需要的目标
CPMAddPackage(
  NAME boost
  GITHUB_REPOSITORY boostorg/boost
  GIT_TAG boost-1.84.0
  EXCLUDE_FROM_ALL YES
)

# 只有显式链接的目标才会被构建
target_link_libraries(my_app Boost::filesystem)
```

**策略 3：并行下载**

```cmake
# CMake 3.16+ 支持 FetchContent 并行
cmake_minimum_required(VERSION 3.16)

# CPM 会自动利用并行下载
# 无需额外配置
```

### 5.2 最佳实践

**实践 1：依赖版本锁定**

```cmake
# 推荐使用 Git 标签或提交哈希，而不是分支
# ✅ 好：确定性版本
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG v9.1.0
)

# ❌ 不推荐：不确定版本
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG main  # 每次构建可能不同
)
```

**实践 2：依赖分离管理**

```cmake
# cmake/Dependencies.cmake
function(my_project_add_dependencies)
  include(CPM)

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
endfunction()
```

**实践 3：CI 缓存优化**

```yaml
# .github/workflows/ci.yml
name: CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4

      - name: Cache CPM
        uses: actions/cache@v3
        with:
          path: |
            ~/.cache/CPM
            .cache/cpm
          key: ${{ runner.os }}-cpm-${{ hashFiles('**/CMakeLists.txt', '**/*.cmake') }}
          restore-keys: |
            ${{ runner.os }}-cpm-

      - name: Build
        run: |
          cmake -B build -DCPM_SOURCE_CACHE=~/.cache/CPM
          cmake --build build
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：下载失败**

```
CMake Error: Failed to download from https://github.com/...
```

**解决方案：**

```bash
# 1. 检查网络连接
ping github.com

# 2. 使用代理
export https_proxy=http://proxy:port

# 3. 使用镜像源（修改 CPM.cmake 或使用覆盖文件）
# CPMOverrides.cmake:
CPMAddPackage(
  NAME fmt
  URL https://mirror.example.com/fmt-9.1.0.tar.gz
)

# 4. 手动下载后放到缓存目录
```

**问题 2：版本冲突**

```
CMake Error: fmt version 9.1.0 already added
```

**解决方案：**

```cmake
# 检查是否重复添加
# 使用 CPMGetPackage 检查包是否已存在
CPMGetPackage(fmt)

# 或使用 CPMFindPackage（类似 find_package）
CPMFindPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 9.1.0
)
```

**问题 3：找不到依赖目标**

```
CMake Error: Target "fmt::fmt" not found
```

**解决方案：**

```cmake
# 检查依赖是否正确安装
if(TARGET fmt::fmt)
  message(STATUS "fmt found")
else()
  message(FATAL_ERROR "fmt not found, check CPM configuration")
endif()

# 检查依赖提供的实际目标名
# 某些库可能使用不同的目标名，如 fmt::fmt-header-only
```

**问题 4：缓存损坏**

```
CMake Error: Corrupted cache at ~/.cache/CPM/fmt
```

**解决方案：**

```bash
# 清除缓存
rm -rf ~/.cache/CPM/fmt

# 或清除所有缓存
rm -rf ~/.cache/CPM
```

### 6.2 调试技巧

**技巧 1：启用详细日志**

```cmake
# CMakeLists.txt
set(CMAKE_VERBOSE_MAKEFILE ON)

# 启用 CPM 调试输出
set(CPM_DEBUG ON)
```

**技巧 2：检查包状态**

```cmake
# 打印所有已添加的包
get_cmake_property(_variableNames VARIABLES)
list(FILTER _variableNames INCLUDE REGEX "^CPM_PACKAGE_")
foreach(_var IN LISTS _variableNames)
  message(STATUS "${_var} = ${${_var}}")
endforeach()
```

**技巧 3：验证依赖来源**

```cmake
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG v9.1.0
)

# 验证下载路径
if(TARGET fmt::fmt)
  get_target_property(fmt_SOURCE_DIR fmt::fmt INTERFACE_INCLUDE_DIRECTORIES)
  message(STATUS "fmt source: ${fmt_SOURCE_DIR}")
endif()
```

## 7. 集成实践

### 7.1 工具链集成

**与现代 CMake 项目集成：**

```cmake
cmake_minimum_required(VERSION 3.20)
project(modern_cpp_project VERSION 1.0.0 LANGUAGES CXX)

# C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 导出编译命令（IDE 支持）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 包含 CPM
include(cmake/CPM.cmake)
include(cmake/Dependencies.cmake)

# 主目标
add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE
  fmt::fmt
  spdlog::spdlog
)

# 安装规则
install(TARGETS my_app DESTINATION bin)
```

**与 vcpkg 共存：**

```cmake
# 先使用 vcpkg 提供的二进制依赖
find_package(Boost REQUIRED COMPONENTS filesystem system)

# 再使用 CPM 添加 vcpkg 没有的库
CPMAddPackage(
  NAME my_custom_lib
  GITHUB_REPOSITORY user/my_custom_lib
  GIT_TAG v1.0.0
)

target_link_libraries(my_app
  Boost::filesystem
  Boost::system
  my_custom_lib
)
```

### 7.2 CI/CD 配置

**GitHub Actions 完整示例：**

```yaml
name: Build and Test

on:
  push:
    branches: [main, develop]
  pull_request:
    branches: [main]

env:
  CPM_SOURCE_CACHE: ${{ github.workspace }}/.cache/cpm

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        build_type: [Release, Debug]

    steps:
      - uses: actions/checkout@v4

      - name: Cache CPM packages
        uses: actions/cache@v3
        with:
          path: ${{ env.CPM_SOURCE_CACHE }}
          key: ${{ runner.os }}-cpm-${{ hashFiles('**/CMakeLists.txt', 'cmake/*.cmake') }}
          restore-keys: |
            ${{ runner.os }}-cpm-

      - name: Configure
        run: cmake -B build -DCMAKE_BUILD_TYPE=${{ matrix.build_type }}

      - name: Build
        run: cmake --build build --config ${{ matrix.build_type }}

      - name: Test
        working-directory: build
        run: ctest -C ${{ matrix.build_type }} --output-on-failure
```

**GitLab CI 示例：**

```yaml
# .gitlab-ci.yml
stages:
  - build
  - test

variables:
  CPM_SOURCE_CACHE: "$CI_PROJECT_DIR/.cache/cpm"

cache:
  key: "$CI_JOB_NAME"
  paths:
    - .cache/cpm/

build:linux:
  stage: build
  image: gcc:12
  script:
    - cmake -B build -DCMAKE_BUILD_TYPE=Release
    - cmake --build build
  artifacts:
    paths:
      - build/

test:linux:
  stage: test
  image: gcc:12
  script:
    - cd build && ctest --output-on-failure
  dependencies:
    - build:linux
```

### 7.3 实战案例

**案例：构建一个现代 C++ CLI 工具**

项目需求：
- 使用 CLI11 解析命令行
- 使用 spdlog 日志
- 使用 fmt 格式化
- 使用 Catch2 测试

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(mycli VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# 包含 CPM
include(cmake/CPM.cmake)

# 依赖管理
CPMAddPackage(
  NAME CLI11
  GITHUB_REPOSITORY CLIUtils/CLI11
  GIT_TAG v2.3.2
)

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

# 主程序
add_executable(mycli
  src/main.cpp
  src/command.cpp
)

target_link_libraries(mycli PRIVATE
  CLI11::CLI11
  fmt::fmt
  spdlog::spdlog
)

# 测试
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    GIT_TAG v3.4.0
  )

  enable_testing()
  add_executable(tests
    test/test_main.cpp
    test/test_command.cpp
  )
  target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    mycli
  )

  include(CTest)
  include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
  catch_discover_tests(tests)
endif()

# 安装
install(TARGETS mycli DESTINATION bin)
```

**目录结构：**

```
mycli/
├── CMakeLists.txt
├── cmake/
│   └── CPM.cmake
├── src/
│   ├── main.cpp
│   └── command.cpp
├── include/
│   └── command.hpp
├── test/
│   ├── test_main.cpp
│   └── test_command.cpp
└── README.md
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 | 说明 |
|------|------|------|
| GitHub 仓库 | https://github.com/cpm-cmake/CPM.cmake | 源码和 issue |
| 官方文档 | https://cpm-cmake.readthedocs.io/ | 详细使用指南 |
| 示例项目 | https://github.com/cpm-cmake/CPM.cmake/tree/master/examples | 实战代码 |
| Release Notes | https://github.com/cpm-cmake/CPM.cmake/releases | 版本更新 |

### 8.2 学习路径

**初级路径（1-2 天）：**

1. 理解 CMake FetchContent 基础
2. 学习 CPM.cmake 基本用法
3. 实践小型项目依赖管理

**中级路径（3-5 天）：**

1. 掌握版本控制策略
2. 学习本地开发模式
3. CI/CD 集成实践

**高级路径（1-2 周）：**

1. 性能优化和缓存策略
2. 与其他包管理器协作
3. 企业级项目实践

**推荐学习资源：**

| 类型 | 资源 | 说明 |
|------|------|------|
| 教程 | "Modern CMake for C++" | 系统学习 CMake |
| 示例 | CPM.cmake examples | 官方示例 |
| 视频 | "Dependency Management with CMake" | YouTube 演讲 |
| 社区 | r/cmake | Reddit 社区 |