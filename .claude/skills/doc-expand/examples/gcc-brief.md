# GCC - GNU 编译器

## 1. 概述

GCC（GNU Compiler Collection）是 GNU 编译器集合，支持 C、C++、Fortran、Go 等语言。作为开源项目标准编译器，GCC 具有跨平台、高度优化、插件扩展等特点。

核心特性：

| 特性 | 说明 |
|------|------|
| 多语言 | C/C++/Fortran/Go/Ada |
| 多平台 | x86/ARM/RISC-V/MIPS |
| 优化 | 多级优化 Pass |
| 开源 | GPL 许可证 |

## 2. 基础使用

### 2.1 快速入门

```bash
# 编译 C 程序
gcc main.c -o main

# 编译 C++ 程序
g++ main.cpp -o main

# 多文件编译
gcc -c a.c b.c
gcc a.o b.o -o program

# 完整构建流程
gcc -E main.c -o main.i    # 预处理
gcc -S main.i -o main.s    # 编译
gcc -c main.s -o main.o    # 汇编
gcc main.o -o main         # 链接
```

### 2.2 常用命令

| 命令 | 说明 |
|------|------|
| `gcc -c file.c` | 只编译不链接 |
| `gcc -o out file.c` | 指定输出文件 |
| `gcc -g file.c` | 包含调试信息 |
| `gcc -O2 file.c` | 标准优化 |
| `gcc -Wall file.c` | 启用所有警告 |
| `gcc -I/path file.c` | 指定头文件路径 |
| `gcc -L/path -lname` | 指定库路径和库名 |

### 2.3 编译流程

```
源代码 → 预处理 → 编译 → 汇编 → 链接 → 可执行文件
   .c      .i      .s     .o      exe
```

## 3. 配置选项

### 3.1 优化选项

| 选项 | 说明 | 适用场景 |
|------|------|---------|
| `-O0` | 不优化 | Debug 调试 |
| `-O1` | 基本优化 | 快速测试 |
| `-O2` | 标准优化 | Release 推荐 |
| `-O3` | 激进优化 | 性能优先 |
| `-Os` | 优化体积 | 嵌入式 |

### 3.2 常用选项

| 选项 | 说明 |
|------|------|
| `-Wall -Wextra` | 启用警告 |
| `-std=c11` | 指定 C 标准 |
| `-std=c++17` | 指定 C++ 标准 |
| `-DDEBUG` | 定义宏 |
| `-DNDEBUG` | 禁用 assert |

### 3.3 Debug vs Release

```bash
# Debug 构建
gcc -O0 -g -Wall main.c -o main_debug

# Release 构建
gcc -O2 -DNDEBUG main.c -o main_release
```

### 3.4 调试技巧

```bash
# 查看预处理结果
gcc -E main.c | head -50

# 查看符号表
nm main.o

# 查看依赖库
ldd main

# 反汇编
objdump -d main | head -30
```

## 4. 参考资源

- [GCC Manual](https://gcc.gnu.org/onlinedocs/)
- [GCC Wiki](https://gcc.gnu.org/wiki/)
- [GCC Optimization Options](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)