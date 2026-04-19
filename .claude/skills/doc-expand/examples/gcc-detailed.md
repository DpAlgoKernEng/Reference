# GCC - GNU 编译器集合详尽指南

本文档为 detailed 模式示例，目标 300-400 行。

## 1. 概述与背景

### 1.1 工具定位

GCC（GNU Compiler Collection）是 GNU 项目核心编译器，支持 C、C++、Fortran、Go 等语言。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1987 | 1.0 | 初始 C 编译器 |
| 1999 | 2.95 | EGCS 合并 |
| 2005 | 4.0 | 新优化框架 |
| 2020 | 10.1 | C++20 支持 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 多语言 | C/C++/Fortran/Go/Ada |
| 多平台 | x86/ARM/RISC-V/MIPS |
| 优化 | 多级优化 Pass |
| 扩展 | 插件、自定义 Pass |

### 1.4 适用场景

- 系统软件开发（内核、驱动）
- 嵌入式交叉编译
- 高性能计算
- 开源项目标准编译器

### 1.5 对比分析

| 编译器 | 优势 | 劣势 |
|------|------|------|
| GCC | 广泛支持、成熟稳定 | 编译速度较慢 |
| Clang | 快速、模块化、诊断友好 | 平台支持较少 |
| MSVC | Windows 集成 | 仅 Windows |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# macOS
brew install gcc

# Ubuntu/Debian
sudo apt install gcc g++ build-essential

# Fedora
sudo dnf install gcc gcc-c++

# Windows (MinGW)
winget install mingw

# 源码编译
git clone https://gcc.gnu.org/git/gcc.git
cd gcc
./configure --prefix=/usr/local/gcc
make -j4
make install
```

### 2.2 版本管理

```bash
gcc --version
g++ --version

# 查看配置
gcc -v

# 检查支持的特性
gcc -E -dM - < /dev/null | grep CPLUSPLUS
```

### 2.3 环境配置

| 变量 | 说明 |
|------|------|
| `CC` | C 编译器路径 |
| `CXX` | C++ 编译器路径 |
| `CFLAGS` | C 编译选项 |
| `CXXFLAGS` | C++ 编译选项 |
| `LD_LIBRARY_PATH` | 库搜索路径 |

### 2.4 验证安装

```bash
# 测试编译
echo '#include <stdio.h>
int main() { printf("Hello GCC\n"); return 0; }' > test.c
gcc test.c -o test && ./test

# 测试 C++
echo '#include <iostream>
int main() { std::cout << "Hello G++\n"; return 0; }' > test.cpp
g++ test.cpp -o testcpp && ./testcpp
```

## 3. 基础使用

### 3.1 编译流程

```
源代码 → 预处理 → 编译 → 汇编 → 链接 → 可执行文件
   .c      .i      .s     .o      exe
```

### 3.2 基本命令

```bash
# 单步编译
gcc main.c -o main

# 分步编译
gcc -E main.c -o main.i    # 预处理
gcc -S main.i -o main.s    # 编译
gcc -c main.s -o main.o    # 汇编
gcc main.o -o main         # 链接

# 多文件
gcc -c a.c b.c
gcc a.o b.o -o program
```

### 3.3 常用选项

| 类别 | 选项 | 说明 |
|------|------|------|
| 输出 | `-o file` | 输出文件名 |
| 优化 | `-O0/-O1/-O2/-O3` | 优化级别 |
| 调试 | `-g` | 调试信息 |
| 警告 | `-Wall/-Wextra` | 启用警告 |
| 标准 | `-std=c11/-std=c++17` | 语言标准 |
| 包含 | `-I/path` | 头文件路径 |
| 库 | `-L/path -lname` | 库路径和库名 |

### 3.4 常用操作

```bash
# Debug 构建
gcc -O0 -g -Wall main.c -o main_debug

# Release 构建
gcc -O2 -DNDEBUG main.c -o main_release

# 查看预处理结果
gcc -E main.c | head -50

# 查看符号表
nm main.o

# 查看依赖库
ldd main
```

## 4. 进阶特性

### 4.1 高级配置

```bash
# 特定架构优化
gcc -march=native -O3 main.c

# LTO 链接时优化
gcc -flto -O3 main.c other.c

# 自动向量化
gcc -O3 -ftree-vectorize main.c

# Profile-guided 优化
gcc -fprofile-generate main.c -o main
./main  # 运行生成 profile
gcc -fprofile-use main.c -o main_optimized
```

### 4.2 扩展功能

```bash
# 预定义宏查询
gcc -dM -E - < /dev/null

# 函数属性
__attribute__((always_inline))
__attribute__((deprecated))
__attribute__((packed))

# 内联汇编
asm("movl %1, %0" : "=r"(result) : "r"(input));
```

### 4.3 插件生态

| 插件 | 功能 |
|------|------|
| GCC Python Plugin | Python 脚本控制 |
| GCC Melt | DSL 扩展 |
| Custom Pass | 自定义优化 |

## 5. 性能优化

### 5.1 优化级别详解

| 级别 | 特性 | 适用场景 |
|------|------|---------|
| `-O0` | 无优化，保持源码映射 | Debug |
| `-O1` | 基本优化，编译快 | 测试 |
| `-O2` | 标准优化，推荐默认 | Release |
| `-O3` | 激进优化，可能增大体积 | 性能优先 |
| `-Os` | 优化体积 | 嵌入式 |
| `-Ofast` | 不遵守标准极限优化 | 特殊场景 |

### 5.2 调优策略

```bash
# 编译时间分析
time gcc -O0 main.c
time gcc -O3 main.c

# 二进制大小对比
size main_O0 main_O3

# 性能测试
perf stat ./main

# 缓存优化
gcc -O3 -fomit-frame-pointer main.c
```

### 5.3 最佳实践

- Release 用 `-O2` 或 `-O3`
- Debug 用 `-O0 -g`
- 嵌入式用 `-Os`
- 本机优化用 `-march=native`

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决 |
|------|------|------|
| undefined reference | 缺库或链接顺序 | `-l库名` 或调整顺序 |
| segmentation fault | 内存错误 | gdb 调试 |
| warning: implicit | 缺头文件 | `#include` |
| linker error | 多定义 | 检查重复符号 |

### 6.2 调试技巧

```bash
# 详细错误信息
gcc -Wall -Wextra -Werror main.c

# 查看警告来源
gcc -Wshadow main.c

# 静态分析
gcc -fanalyzer main.c

# GDB 调试
gcc -g main.c -o main
gdb main
(gdb) break main
(gdb) run
```

### 6.3 故障诊断

```bash
# 检查编译器配置
gcc -v

# 检查头文件搜索路径
gcc -E -v main.c

# 检查库搜索路径
gcc -print-search-dirs

# 生成依赖关系
gcc -M main.c
```

## 7. 集成实践

### 7.1 工具链集成

| 工具 | 集成方式 |
|------|---------|
| Make | Makefile CC=gcc |
| CMake | project(... C CXX) |
| Autotools | configure.ac |
| Meson | meson.build |

### 7.2 CI/CD 配置

```yaml
# GitHub Actions
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - run: gcc -Wall -O2 main.c -o main
      - run: ./main

# GitLab CI
build:
  script:
    - gcc -c src/*.c
    - gcc *.o -o app
```

### 7.3 实战案例

案例：多模块项目构建

```bash
# 项目结构
project/
├── src/
│   ├── main.c
│   ├── utils.c
│   └── utils.h
├── lib/
│   └── external.a
└── build/

# 编译命令
gcc -c src/utils.c -Isrc -o build/utils.o
gcc -c src/main.c -Isrc -o build/main.o
gcc build/*.o lib/external.a -o build/app -lm
```

要点：
- `-Isrc` 指定头文件路径
- `-lm` 链接数学库
- 分步编译便于调试

## 8. 参考资源

### 8.1 官方文档

- [GCC Manual](https://gcc.gnu.org/onlinedocs/)
- [GCC Internals](https://gcc.gnu.org/onlinedocs/gccint/)

### 8.2 社区资源

- [GCC Wiki](https://gcc.gnu.org/wiki/)
- GCC mailing lists

### 8.3 学习路径

1. 基础：编译流程和常用选项
2. 进阶：优化策略和调试技巧
3. 高级：自定义 Pass 和插件开发