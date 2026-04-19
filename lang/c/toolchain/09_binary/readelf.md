# readelf - ELF 文件分析工具

## 1. 概述与背景

### 1.1 工具定位

readelf 是 GNU binutils 工具链提供的 ELF（Executable and Linkable Format）文件分析工具，用于深入解析 ELF 文件的内部结构和元数据信息。它是 Linux/Unix 系统下二进制文件分析的必备工具。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1991 | 1.x | 随 binutils 首次发布，支持基本 ELF 解析 |
| 1999 | 2.10 | 增加 64 位 ELF 支持 |
| 2005 | 2.16 | 完善动态段解析 |
| 2010 | 2.20 | 增加版本信息显示 |
| 2015 | 2.25 | 改进符号表输出格式 |
| 2020 | 2.35 | 支持 LLVM LTO 对象文件 |

### 1.3 核心特性

- **完整 ELF 解析**：支持所有 ELF 文件类型（可执行文件、目标文件、共享库、核心转储）
- **详细段信息**：显示文件头、程序头、段头的完整信息
- **符号表分析**：解析静态符号表和动态符号表
- **动态链接分析**：展示依赖库、重定位、版本信息
- **架构无关**：支持多种 CPU 架构（x86、ARM、RISC-V 等）
- **脚本友好**：输出格式便于 grep/awk 等工具处理

### 1.4 适用场景

| 场景 | 说明 |
|------|------|
| 编译调试 | 分析目标文件结构，定位编译问题 |
| 逆向工程 | 了解二进制文件布局和符号信息 |
| 动态链接分析 | 查看依赖库、符号解析、重定位信息 |
| 安全审计 | 检查安全特性（RELRO、NX 等） |
| 性能优化 | 分析段布局，优化内存对齐 |
| 嵌入式开发 | 分析固件格式、检查加载地址 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| readelf | ELF 解析最全面、输出详细 | 不支持反汇编 |
| objdump | 支持反汇编、多格式支持 | ELF 信息不如 readelf 详细 |
| nm | 符号表查看简洁 | 功能单一 |
| file | 快速识别文件类型 | 信息有限 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# Linux - Debian/Ubuntu
sudo apt install binutils

# Linux - CentOS/RHEL
sudo yum install binutils

# Linux - Fedora
sudo dnf install binutils

# Linux - Arch Linux
sudo pacman -S binutils

# macOS (使用 Mach-O 格式，用 otool 替代)
brew install binutils
```

### 2.2 版本管理

```bash
# 查看版本
readelf --version

# 查看支持的架构
readelf --help | grep -A 20 "Supported architectures"
```

### 2.3 环境配置

readelf 不需要特殊环境配置，安装后即可直接使用。相关环境变量：

| 环境变量 | 说明 |
|----------|------|
| PATH | 包含 readelf 的目录 |
| LD_LIBRARY_PATH | 解析时库搜索路径 |

### 2.4 验证安装

```bash
# 验证命令可用
which readelf

# 验证版本
readelf --version

# 测试基本功能
echo 'int main(){return 0;}' | gcc -x c - -o /tmp/test
readelf -h /tmp/test
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 编译示例程序
cat > hello.c << 'EOF'
#include <stdio.h>

int global_var = 42;
static int static_var = 100;

void hello(void) {
    printf("Hello, World!\n");
}

int main(int argc, char *argv[]) {
    hello();
    return 0;
}
EOF

gcc hello.c -o hello

# 显示所有信息
readelf -a hello
```

### 3.2 项目结构

ELF 文件的核心结构：

```
ELF 文件
├── ELF Header          # 文件头，描述文件类型、架构等
├── Program Headers     # 程序头表，描述可加载段
├── Section Headers     # 段头表，描述各段信息
├── Sections           # 各段内容
│   ├── .interp        # 解释器路径
│   ├── .text          # 代码段
│   ├── .rodata        # 只读数据
│   ├── .data          # 已初始化数据
│   ├── .bss           # 未初始化数据
│   ├── .symtab        # 符号表
│   ├── .strtab        # 字符串表
│   ├── .dynamic       # 动态链接信息
│   └── ...            # 其他段
└── Section Data       # 段数据
```

### 3.3 基本命令

| 选项 | 说明 | 示例 |
|------|------|------|
| `-a` | 显示所有信息 | `readelf -a program` |
| `-h` | 显示 ELF 文件头 | `readelf -h program` |
| `-l` | 显示程序头（段） | `readelf -l program` |
| `-S` | 显示段头表 | `readelf -S program` |
| `-s` | 显示符号表 | `readelf -s program` |
| `-r` | 显示重定位信息 | `readelf -r program` |
| `-d` | 显示动态段 | `readelf -d program` |
| `-V` | 显示版本信息 | `readelf -V program` |
| `-n` | 显示注释段 | `readelf -n program` |
| `-e` | 显示所有头 | `readelf -e program` |
| `-I` | 显示架构信息 | `readelf -I program` |

### 3.4 常用操作

```bash
# 快速查看文件类型
readelf -h program | grep Type

# 查看依赖库
readelf -d program | grep NEEDED

# 查找符号
readelf -s program | grep main

# 查看代码段
readelf -x .text program

# 查看只读数据
readelf -p .rodata program
```

## 4. 进阶特性

### 4.1 ELF 文件头详解

```bash
readelf -h program
```

输出示例：
```
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x1040
  Start of program headers:          64 (bytes into file)
  Start of section headers:          7120 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         13
  Size of section headers:           64 (bytes)
  Number of section headers:         31
  Section header string table index: 30
```

| 字段 | 说明 | 取值范围 |
|------|------|----------|
| Magic | ELF 魔数 | `7f 45 4c 46` |
| Class | 文件类别 | ELF32/ELF64 |
| Data | 字节序 | 小端/大端 |
| Type | 文件类型 | REL/EXEC/DYN/CORE |
| Machine | 目标架构 | x86-64/ARM/RISC-V 等 |
| Entry point | 程序入口地址 | 虚拟地址 |

### 4.2 程序头分析

```bash
readelf -l program
```

输出示例：
```
Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000000040 0x0000000000000040
                 0x00000000000002d8 0x00000000000002d8 R      0x8
  INTERP         0x0000000000000238 0x0000000000000238 0x0000000000000238
                 0x000000000000001c 0x000000000000001c R      0x1
  LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x00000000000007b8 0x00000000000007b8 R E    0x1000
  LOAD           0x0000000000000db8 0x0000000000001db8 0x0000000000001db8
                 0x0000000000000228 0x0000000000000230 RW     0x1000
```

| 类型 | 说明 | 用途 |
|------|------|------|
| PHDR | 程序头表本身 | 程序头表位置信息 |
| INTERP | 解释器路径 | 动态链接器路径 |
| LOAD | 可加载段 | 需要加载到内存的段 |
| DYNAMIC | 动态链接信息 | 动态链接器使用 |
| NOTE | 注释信息 | 辅助信息 |
| TLS | 线程本地存储 | TLS 模板 |

段标志含义：

| 标志 | 说明 | 权限 |
|------|------|------|
| R | 可读 | 读权限 |
| W | 可写 | 写权限 |
| E | 可执行 | 执行权限 |

### 4.3 段头详解

```bash
readelf -S program
```

输出示例：
```
Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  0000000000000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .interp           PROGBITS         0000000000000238  0000000000000238
       000000000000001c  0000000000000000   A       0     0     1
  [ 2] .text             PROGBITS         0000000000001040  0000000000001040
       0000000000000080  0000000000000000  AX       0     0     4
  [ 3] .rodata           PROGBITS         0000000000002000  0000000000002000
       0000000000000020  0000000000000000   A       0     0     4
```

常见段类型：

| 类型 | 说明 | 内容 |
|------|------|------|
| NULL | 无效段 | 未使用 |
| PROGBITS | 程序定义内容 | 代码、数据 |
| SYMTAB | 符号表 | 符号信息 |
| STRTAB | 字符串表 | 字符串数据 |
| RELA | 重定位信息 | 重定位条目 |
| DYNAMIC | 动态链接 | 动态信息 |
| NOTE | 注释 | 元数据 |
| BSS | 未初始化数据 | 零初始化数据 |

段标志：

| 标志 | 说明 |
|------|------|
| W | 可写 |
| A | 分配内存 |
| X | 可执行 |
| M | 可合并 |
| S | 字符串表 |

### 4.4 符号表分析

```bash
readelf -s program
```

输出示例：
```
Symbol table '.symtab' contains 10 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS program.c
     2: 0000000000001132    18 FUNC    GLOBAL DEFAULT    1 add
     3: 0000000000001149    42 FUNC    GLOBAL DEFAULT    1 main
```

符号属性解析：

| 属性 | 说明 | 取值 |
|------|------|------|
| Value | 符号地址 | 虚拟地址 |
| Size | 符号大小 | 字节数 |
| Type | 符号类型 | NOTYPE/OBJECT/FUNC/SECTION/FILE |
| Bind | 绑定类型 | LOCAL/GLOBAL/WEAK |
| Vis | 可见性 | DEFAULT/PROTECTED/HIDDEN |
| Ndx | 所在段索引 | 段号或特殊值（UND/ABS） |

### 4.5 动态段解析

```bash
readelf -d program
```

输出示例：
```
Dynamic section at offset 0x2db8 contains 10 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000000c (INIT)               0x1000
 0x000000000000000d (FINI)               0x11c0
 0x0000000000000019 (INIT_ARRAY)         0x3db8
 0x000000000000001b (INIT_ARRAYSZ)       8
```

动态标签类型：

| 标签 | 说明 | 用途 |
|------|------|------|
| NEEDED | 依赖库 | 需要加载的共享库 |
| INIT | 初始化函数 | 程序初始化入口 |
| FINI | 终止函数 | 程序终止入口 |
| RPATH | 运行时搜索路径 | 库搜索路径 |
| RUNPATH | 运行时路径 | 替代 RPATH |
| SONAME | 共享库名称 | 库标识名 |

## 5. 性能优化

### 5.1 安全特性检查

```bash
# 检查 RELRO（只读重定位）
readelf -l program | grep GNU_RELRO

# 检查栈可执行性（NX 位）
readelf -l program | grep GNU_STACK

# 检查 PIE（位置无关可执行）
readelf -h program | grep Type
# DYN 表示 PIE，EXEC 表示非 PIE
```

### 5.2 最佳实践

| 场景 | 推荐命令 | 说明 |
|------|----------|------|
| 快速类型检查 | `readelf -h file \| grep Type` | 脚本友好 |
| 依赖分析 | `readelf -d file \| grep NEEDED` | 查找依赖库 |
| 符号查找 | `readelf -s file \| grep func` | 定位符号 |
| 段大小分析 | `readelf -S file` | 优化内存布局 |
| 入口点查找 | `readelf -h file \| grep Entry` | 调试入口 |

## 6. 问题排查

### 6.1 常见问题

**问题 1：找不到依赖库**

```bash
# 查看依赖库
readelf -d program | grep NEEDED

# 输出示例
 0x0000000000000001 (NEEDED)             Shared library: [libcustom.so]
```

**问题 2：符号未定义**

```bash
# 查找未定义符号
readelf -s program | grep UND

# 动态符号表
readelf --dyn-syms program | grep UND
```

**问题 3：文件类型判断**

```bash
# 判断文件类型
readelf -h program | grep Type

# 类型对照
# REL   - 可重定位文件（.o）
# EXEC  - 可执行文件
# DYN   - 共享目标文件（.so 或 PIE）
# CORE  - 核心转储文件
```

### 6.2 调试技巧

```bash
# 查看段映射到程序头的对应关系
readelf -l program

# 分析段地址对齐
readelf -S program | grep -A 1 ".text"

# 查看注释信息（编译器版本等）
readelf -n program

# 查看构建 ID
readelf -n program | grep "Build ID"
```

## 7. 集成实践

### 7.1 工具链集成

```bash
# 编译后自动检查
gcc program.c -o program && readelf -h program

# 与 objdump 配合使用
readelf -s program | head -20
objdump -t program | head -20

# 与 nm 配合使用
readelf -s program | grep FUNC
nm program | grep ' T '
```

### 7.2 CI/CD 配置

```yaml
# .gitlab-ci.yml 示例
binary-analysis:
  stage: test
  script:
    - readelf -h ${BINARY_PATH}
    - readelf -d ${BINARY_PATH} | grep NEEDED
    - readelf -s ${BINARY_PATH} | grep -i "gl_\|glut_\|glfw_"
  artifacts:
    reports:
      binary: binary_report.txt
```

```bash
#!/bin/bash
# 检查脚本：check_binary.sh

BINARY=$1

echo "=== File Type ==="
readelf -h $BINARY | grep Type

echo "=== Dependencies ==="
readelf -d $BINARY | grep NEEDED

echo "=== Security Features ==="
readelf -l $BINARY | grep -E "GNU_RELRO|GNU_STACK"
```

### 7.3 实战案例

**案例 1：分析静态库**

```bash
# 查看静态库包含的目标文件
ar -t libmylib.a

# 提取并分析目标文件
ar -x libmylib.a myobj.o
readelf -s myobj.o
```

**案例 2：检查 ABI 兼容性**

```bash
# 查看共享库的 SONAME
readelf -d libmylib.so | grep SONAME

# 查看符号版本
readelf -V libmylib.so
```

**案例 3：调试动态加载问题**

```bash
# 查看程序入口
readelf -h program | grep "Entry point"

# 查看动态链接器路径
readelf -l program | grep INTERP -A 1

# 查看 RPATH/RUNPATH
readelf -d program | grep -E "RPATH|RUNPATH"
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| binutils 手册 | https://sourceware.org/binutils/docs/ |
| readelf 文档 | https://man7.org/linux/man-pages/man1/readelf.1.html |
| ELF 规范 | https://refspecs.linuxfoundation.org/elf/elf.pdf |
| System V ABI | https://wiki.osdev.org/ELF |

### 8.2 学习路径

| 阶段 | 学习内容 | 推荐资源 |
|------|----------|----------|
| 入门 | ELF 基础、基本命令 | man readelf |
| 进阶 | 段、符号表、动态链接 | 《程序员的自我修养》 |
| 高级 | 安全特性、性能优化 | ABI 规范文档 |
| 实战 | 逆向工程、调试技巧 | CTF Wiki ELF 章节 |