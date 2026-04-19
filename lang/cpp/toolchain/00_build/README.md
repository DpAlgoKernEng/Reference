# C++ 构建系统

本目录收录 C++ 项目构建系统文档。

## 构建系统概览

| 系统 | 说明 | 特点 |
|------|------|------|
| [CMake](cmake.md) | 跨平台构建 | 业界标准，功能强大 |
| [Make](make.md) | 传统构建工具 | 灵活，轻量级 |
| [Ninja](ninja.md) | 快速构建工具 | 极速，专注增量 |
| [Meson](meson.md) | 现代构建系统 | 快速，用户友好 |
| [Bazel](bazel.md) | Google 构建系统 | 大规模，可重现 |
| [xmake](xmake.md) | Lua 构建系统 | 跨平台，易用 |

## 构建系统对比

| 特性 | CMake | Make | Ninja | Meson | Bazel | xmake |
|------|-------|------|-------|-------|-------|-------|
| 跨平台 | ✅ | ❌ | ✅ | ✅ | ✅ | ✅ |
| IDE 集成 | ✅ | 部分 | ❌ | ✅ | ❌ | 部分 |
| 构建速度 | 中 | 中 | ✅快 | ✅快 | 中 | 快 |
| 依赖管理 | ✅ | ❌ | ❌ | ✅ | ✅ | ✅ |
| 配置语言 | DSL | Makefile | Ninja | Python | Starlark | Lua |
| 学习曲线 | 中 | 中 | 低 | 低 | 高 | 低 |

## 选择指南

### 项目规模

| 规模 | 推荐 |
|------|------|
| 小型项目 | xmake/Meson/Ninja |
| 中型项目 | CMake/Meson |
| 大型企业 | CMake/Bazel |
| 跨平台项目 | CMake/Meson/xmake |

### 特殊需求

| 需求 | 推荐 |
|------|------|
| IDE 集成 | CMake |
| 极速构建 | Ninja |
| 可重现构建 | Bazel |
| 快速上手 | xmake/Meson |
| 现代风格 | Meson/xmake |

## CMake 基础

### 项目结构

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myapp src/main.cpp)

find_package(Boost REQUIRED)
target_link_libraries(myapp Boost::boost)
```

### 构建流程

```bash
cmake -B build -S .
cmake --build build
cmake --install build
```

## Make 基础

### Makefile 结构

```makefile
CC = gcc
CFLAGS = -Wall -O2

SRCS = main.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)

myapp: $(OBJS)
    $(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
    $(CC) $(CFLAGS) -c $< -o $@
```

## Ninja 基础

### build.ninja 结构

```ninja
rule compile
    command = gcc -c $in -o $out

rule link
    command = gcc $in -o $out

build main.o: compile main.cpp
build myapp: link main.o
```

## Meson 基础

### meson.build 结构

```python
project('myproject', 'cpp',
  version: '1.0',
  default_options: ['cpp_std=c++17'])

executable('myapp', 'src/main.cpp')

dependency('boost', required: true)
```

## Bazel 基础

### BUILD 结构

```python
cc_binary(
    name = "myapp",
    srcs = ["src/main.cpp"],
    deps = ["@boost//:boost"],
)
```

## xmake 基础

### xmake.lua 结构

```lua
target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    set_languages("c++17")
    add_packages("boost")
```

## CI/CD 集成

### GitHub Actions

```yaml
- name: Configure
  run: cmake -B build
- name: Build
  run: cmake --build build
- name: Test
  run: ctest --test-dir build
```

## 相关文档

- [编译器](../01_compiler/) - GCC、Clang 等
- [调试器](../03_debug/) - GDB、LLDB 等
- [包管理](../02_package/) - vcpkg、Conan 等