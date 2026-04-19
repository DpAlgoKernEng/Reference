# GCC - GNU 编译器集合

## 1. 概述与背景

### 1.1 定位

GCC 是 GNU 项目核心编译器，支持多语言（C/C++/Fortran/Go）、多平台（x86/ARM/RISC-V），是开源项目的事实标准编译器。由 GNU 项目于 1987 年首次发布，经过 30+ 年发展，成为最成熟的开源编译器。

### 1.2 核心特性

| 特性 | 说明 |
|------|------|
| 多语言 | C/C++/Fortran/Go/Ada/D |
| 多平台 | x86/ARM/RISC-V/MIPS/PowerPC |
| 优化 | 多级优化 Pass、LTO、PGO |
| 跨编译 | 支持交叉编译 |
| 开源 | GPL 许可证，社区活跃 |

### 1.3 适用场景

- 系统软件开发（内核、驱动、固件）
- 嵌入式交叉编译
- 高性能计算
- 开源项目标准编译器

## 2. 安装配置

### 2.1 多平台安装

```bash
# macOS (Homebrew)
brew install gcc
brew install gcc@12  # 安装特定版本

# Ubuntu/Debian
sudo apt update
sudo apt install gcc g++ build-essential

# Fedora/RHEL
sudo dnf install gcc gcc-c++ make

# Windows (MSYS2/MinGW)
pacman -S mingw-w64-x86_64-gcc

# Windows (Scoop)
scoop install gcc
```

### 2.2 版本管理

```bash
# 查看版本
gcc --version
g++ --version

# 查看详细配置
gcc -v

# 检查支持的 C 标准
gcc -std=c  # 按 Tab 键列出所有选项

# 多版本切换
sudo update-alternatives --config gcc
```

### 2.3 验证安装

```bash
# 基础验证
echo 'int main(){return 0;}' > test.c
gcc test.c -o test && ./test; echo "Exit code: $?"

# 功能验证
echo '#include <stdio.h>
int main(){printf("Hello GCC %d.%d\n", __GNUC__, __GNUC_MINOR__); return 0;}' > test.c
gcc test.c -o test && ./test
```

## 3. 基础使用

### 3.1 编译流程

```
源代码 (.c/.cpp)
    ↓ 预处理 (-E)
预处理文件 (.i)
    ↓ 编译 (-S)
汇编文件 (.s)
    ↓ 汇编 (-c)
目标文件 (.o)
    ↓ 链接
可执行文件
```

### 3.2 快速入门

```bash
# 单文件编译
gcc main.c -o main

# 指定 C++ 标准
g++ -std=c++17 main.cpp -o main

# 多文件项目
gcc -c main.c utils.c
gcc main.o utils.o -o program -lm

# 完整构建命令
gcc -Wall -Wextra -O2 -I./include -L./lib main.c -o main -lmylib
```

### 3.3 常用操作

| 操作 | 命令 |
|------|------|
| 预处理 | `gcc -E main.c > main.i` |
| 编译为汇编 | `gcc -S main.c -o main.s` |
| 只编译不链接 | `gcc -c main.c -o main.o` |
| 链接目标文件 | `gcc a.o b.o -o program` |
| 查看符号表 | `nm main.o` |
| 查看依赖库 | `ldd main` |
| 反汇编 | `objdump -d main` |

### 3.4 常用选项详解

| 类别 | 选项 | 说明 |
|------|------|------|
| 输出 | `-o file` | 指定输出文件名 |
| 优化 | `-O0/-O1/-O2/-O3/-Os` | 优化级别 |
| 调试 | `-g` | 生成调试信息 |
| 警告 | `-Wall -Wextra -Wpedantic` | 启用警告 |
| 标准 | `-std=c11 -std=c++17` | 语言标准 |
| 头文件 | `-I/path` | 头文件搜索路径 |
| 库 | `-L/path -lname` | 库搜索路径和库名 |
| 预处理 | `-DMACRO -DMACRO=value` | 定义宏 |

## 4. 进阶特性

### 4.1 高级配置

```bash
# 特定架构优化
gcc -march=native -mtune=native -O3 main.c

# LTO 链接时优化
gcc -flto -O3 main.c other.c -o program

# Profile-guided 优化 (PGO)
gcc -fprofile-generate -O2 main.c -o main_pgo
./main_pgo  # 运行生成 profile 数据
gcc -fprofile-use -O2 main.c -o main_optimized

# 函数级链接 + 死代码消除
gcc -ffunction-sections -fdata-sections -Wl,--gc-sections main.c

# 生成位置无关代码
gcc -fPIC -shared lib.c -o lib.so
```

### 4.2 优化级别对比

| 级别 | 特性 | 编译时间 | 体积 | 适用场景 |
|------|------|---------|------|---------|
| `-O0` | 无优化 | 最快 | 最大 | Debug |
| `-O1` | 基本优化 | 快 | 大 | 测试 |
| `-O2` | 标准优化 | 中等 | 中 | Release |
| `-O3` | 激进优化 | 慢 | 可能增大 | 性能优先 |
| `-Os` | 优化体积 | 中等 | 最小 | 嵌入式 |
| `-Ofast` | 不遵守标准 | 慢 | - | 特殊场景 |

### 4.3 扩展功能

```bash
# 预定义宏查询
gcc -dM -E - < /dev/null
gcc -dM -E - < /dev/null | grep -i linux

# 静态分析
gcc -fanalyzer main.c

# 自动向量化
gcc -O3 -ftree-vectorize main.c

# 地址消毒器
gcc -fsanitize=address -g main.c -o main_asan

# 未定义行为消毒器
gcc -fsanitize=undefined -g main.c -o main_ubsan
```

### 4.4 GCC 属性扩展

```c
// 函数属性
__attribute__((always_inline)) inline void func() {}
__attribute__((deprecated("use new_func"))) void old_func() {}
__attribute__((noreturn)) void exit_func();

// 变量属性
__attribute__((packed)) struct { char a; int b; };

// 构造/析构函数
__attribute__((constructor)) void init() {}
__attribute__((destructor)) void fini() {}
```

## 5. 问题排查

### 5.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|---------|
| undefined reference | 缺库或链接顺序错误 | `-lname` 或调整链接顺序 |
| segmentation fault | 内存访问错误 | gdb 调试，检查指针 |
| warning: implicit | 缺少头文件 | 添加 `#include` |
| linker error | 重复定义 | 检查符号，使用 `static` |
| 找不到头文件 | 路径未指定 | 使用 `-I` 指定路径 |
| 找不到库 | 路径未指定 | 使用 `-L` 指定路径 |

### 5.2 调试技巧

```bash
# 详细警告输出
gcc -Wall -Wextra -Wpedantic main.c

# 查看预处理结果
gcc -E main.c | head -100

# 查看编译器搜索路径
gcc -print-search-dirs

# 查看头文件搜索路径
gcc -E -v main.c 2>&1 | grep "^ "

# 生成依赖关系
gcc -M main.c
gcc -MM main.c  # 不包含系统头文件
```

### 5.3 GDB 调试

```bash
# 编译带调试信息
gcc -g -O0 main.c -o main

# 启动 GDB
gdb main

# GDB 常用命令 break main
(gdb) run
(gdb) next
(gdb) step
(gdb) print var
(gdb) backtrace
(gdb) quit
```

## 6. 参考资源

### 6.1 官方文档

- [GCC Manual](https://gcc.gnu.org/onlinedocs/)
- [GCC Internals](https://gcc.gnu.org/onlinedocs/gccint/)
- [GCC Optimization Options](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

### 6.2 社区资源

- [GCC Wiki](https://gcc.gnu.org/wiki/)
- GCC mailing lists: gcc@gcc.gnu.org
- [GCC Bugzilla](https://gcc.gnu.org/bugzilla/)

### 6.3 学习路径

| 阶段 | 内容 |
|------|------|
| 入门 | 编译流程、常用选项、基础调试 |
| 进阶 | 优化策略、工具链集成、交叉编译 |
| 高级 | 插件开发、Pass 定制、内部机制 |