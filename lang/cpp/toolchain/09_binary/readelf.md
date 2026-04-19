# readelf - ELF 文件分析工具

## 1. 概述与背景

### 1.1 工具定位

readelf 是 GNU 工具链（binutils）提供的专业 ELF（Executable and Linkable Format）文件分析工具。它能够详细解析和显示 ELF 文件的内部结构，包括文件头、程序头、段头、符号表、动态段等关键信息。作为 Linux 平台下最权威的 ELF 分析工具，readelf 广泛应用于程序开发、调试、逆向工程和安全分析等领域。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1991 | binutils 1.0 | 初期版本，基本 ELF 支持 |
| 1996 | binutils 2.0 | 完整 ELF32 支持 |
| 2000 | binutils 2.10 | ELF64 支持，完善动态段分析 |
| 2010 | binutils 2.20 | 增强 DWARF 调试信息支持 |
| 2020 | binutils 2.35 | 改进安全性，支持新架构 |
| 2023 | binutils 2.41 | 最新版本，全面支持 RISC-V 等 |

### 1.3 核心特性

- **完整结构解析**：精确显示 ELF 文件所有组成部分
- **多架构支持**：x86/x86-64、ARM、RISC-V、MIPS 等
- **动态链接分析**：详细展示依赖库和符号绑定
- **符号表查看**：完整符号信息，包括类型、绑定、可见性
- **重定位信息**：清晰显示代码和数据重定位条目
- **版本控制**：分析符号版本和依赖版本

### 1.4 适用场景

| 场景 | 说明 |
|------|------|
| 程序调试 | 分析段布局、符号地址、入口点 |
| 动态链接问题 | 查看依赖库、RPATH、符号版本 |
| 性能优化 | 分析段大小、对齐、内存布局 |
| 安全分析 | 检查 RELRO、Stack Canary、PIE |
| 逆向工程 | 提取符号、字符串、元数据 |
| 移植适配 | 检查架构、ABI、调用约定 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 | 主要用途 |
|------|------|------|---------|
| readelf | ELF 结构详细、输出结构化 | 无反汇编功能 | ELF 结构分析 |
| objdump | 支持反汇编、多格式输出 | ELF 信息较简略 | 反汇编调试 |
| nm | 符号查看简单直接 | 信息有限 | 快速符号查找 |
| file | 快速识别文件类型 | 信息量少 | 文件类型判断 |
| ldd | 查看动态依赖 | 仅动态库依赖 | 依赖分析 |

## 2. 安装与配置

### 2.1 多平台安装

readelf 随 binutils 包一起安装：

```bash
# Debian/Ubuntu
sudo apt update
sudo apt install binutils

# CentOS/RHEL
sudo yum install binutils

# Fedora
sudo dnf install binutils

# Arch Linux
sudo pacman -S binutils

# macOS（使用 Mach-O 格式，安装 binutils 提供 readelf）
brew install binutils
# 注意：macOS 原生工具是 otool 和 dyld_info
```

### 2.2 版本管理

```bash
# 查看版本
readelf --version

# 检查支持的架构
readelf --help | grep -A 20 "supported architectures"

# 查看安装位置
which readelf
```

### 2.3 环境配置

```bash
# 添加到 PATH（如果需要）
export PATH="/usr/local/bin:$PATH"

# 设置默认架构（交叉编译时）
export READELF_ARCH=x86_64

# 别名设置（可选）
alias relf='readelf'
alias relfa='readelf -a'
```

### 2.4 验证安装

```bash
# 检查版本
readelf --version

# 测试基本功能
readelf -h /bin/ls

# 预期输出：显示 ELF 文件头信息
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 显示所有信息（最全面）
readelf -a program

# 显示文件头（基本信息）
readelf -h program

# 显示程序头（内存段）
readelf -l program

# 显示段头（节区）
readelf -S program

# 显示符号表（函数和变量）
readelf -s program
```

### 3.2 项目结构

典型的 ELF 文件分析场景：

```
项目目录/
├── program          # 可执行文件
├── libfoo.so        # 共享库
├── obj.o            # 目标文件
└── libfoo.a         # 静态库
```

### 3.3 基本命令

**常用命令速查：**

```bash
# 1. 文件类型识别
readelf -h program | grep Type

# 2. 查看依赖库
readelf -d program | grep NEEDED

# 3. 查找符号
readelf -s program | grep main

# 4. 查看段大小
readelf -S program | grep -E "\.text|\.data|\.bss"

# 5. 检查入口点
readelf -h program | grep Entry

# 6. 查看动态符号
readelf -s --wide program | grep FUNC
```

### 3.4 常用选项

| 选项 | 说明 | 典型用法 |
|------|------|---------|
| `-a` | 显示所有信息 | `readelf -a program` |
| `-h` | 显示 ELF 文件头 | `readelf -h program` |
| `-l` | 显示程序头（段） | `readelf -l program` |
| `-S` | 显示段头表 | `readelf -S program` |
| `-s` | 显示符号表 | `readelf -s program` |
| `-r` | 显示重定位信息 | `readelf -r program` |
| `-d` | 显示动态段 | `readelf -d program` |
| `-V` | 显示版本信息 | `readelf -V program` |
| `-n` | 显示注释段 | `readelf -n program` |
| `-p` | 显示字符串段 | `readelf -p .rodata program` |
| `-x` | 显示段十六进制内容 | `readelf -x .text program` |
| `-e` | 显示所有头 | `readelf -e program` |
| `-I` | 显示架构信息 | `readelf -I program` |
| `--debug-dump` | 显示调试信息 | `readelf --debug-dump=info program` |
| `--version-info` | 显示版本信息 | `readelf --version-info program` |

## 4. 进阶特性

### 4.1 ELF 文件头详细解析

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

**文件头字段详解：**

| 字段 | 说明 | 取值示例 |
|------|------|---------|
| Magic | ELF 魔数 | `7f 45 4c 46` |
| Class | 32/64 位 | ELF64 |
| Data | 字节序 | Little endian |
| Type | 文件类型 | EXEC/DYN/REL |
| Machine | 目标架构 | X86-64/ARM/RISC-V |
| Entry point | 程序入口地址 | 0x1040 |
| Flags | 处理器特定标志 | 0x0 |

### 4.2 程序头分析

程序头描述了如何将文件加载到内存：

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

**程序头类型说明：**

| 类型 | 说明 | 用途 |
|------|------|------|
| PHDR | 程序头表本身 | 描述程序头表位置 |
| INTERP | 解释器路径 | 动态链接器路径 |
| LOAD | 可加载段 | 需要映射到内存的段 |
| DYNAMIC | 动态链接信息 | 指向动态段 |
| NOTE | 注释信息 | 辅助信息 |
| TLS | 线程本地存储 | TLS 模板 |

**段标志：**

| 标志 | 说明 | 含义 |
|------|------|------|
| R | 可读 | 可读取内容 |
| W | 可写 | 可修改内容 |
| E | 可执行 | 包含可执行代码 |

### 4.3 段头详细分析

段头描述了文件的逻辑分区：

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
  [ 4] .data             PROGBITS         0000000000002000  0000000000002000
       0000000000000010  0000000000000010  WA       0     0     4
  [ 5] .bss              NOBITS           0000000000002010  0000000000002010
       0000000000000008  0000000000000000  WA       0     0     1
```

**段类型说明：**

| 类型 | 说明 | 内容 |
|------|------|------|
| NULL | 无效段 | 占位符 |
| PROGBITS | 程序定义内容 | 代码或数据 |
| SYMTAB | 符号表 | 符号信息 |
| STRTAB | 字符串表 | 字符串 |
| RELA | 重定位信息 | 重定位条目 |
| DYNAMIC | 动态链接 | 动态信息 |
| NOTE | 注释 | 元数据 |
| NOBITS | 未初始化数据 | BSS 段 |

### 4.4 符号表分析

符号表是连接编译和链接的关键：

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
     4: 0000000000001040     0 FUNC    GLOBAL DEFAULT    1 _start
     5: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
```

**符号属性详解：**

| 属性 | 说明 | 取值 |
|------|------|------|
| Value | 符号地址 | 虚拟地址 |
| Size | 符号大小 | 字节数 |
| Type | 符号类型 | FUNC/OBJECT/NOTYPE |
| Bind | 绑定类型 | LOCAL/GLOBAL/WEAK |
| Vis | 可见性 | DEFAULT/HIDDEN |
| Ndx | 所在段索引 | 段号或特殊值 |

**符号类型：**

| 类型 | 说明 | 示例 |
|------|------|------|
| FUNC | 函数 | `main`, `add` |
| OBJECT | 变量 | 全局变量 |
| NOTYPE | 未定义 | 外部符号 |
| FILE | 文件名 | 源文件名 |
| SECTION | 段 | 段符号 |

### 4.5 动态段分析

动态段包含动态链接所需的信息：

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
 0x000000000000001a (FINI_ARRAY)         0x3dc0
 0x000000000000001c (FINI_ARRAYSZ)       8
 0x000000006ffffef5 (GNU_HASH)           0x300
 0x0000000000000005 (STRTAB)             0x380
 0x0000000000000006 (SYMTAB)             0x358
```

**动态标签类型：**

| 标签 | 说明 | 用途 |
|------|------|------|
| NEEDED | 依赖库 | 列出所需的共享库 |
| INIT | 初始化函数地址 | 程序初始化 |
| FINI | 终止函数地址 | 程序清理 |
| INIT_ARRAY | 初始化数组 | 多个初始化函数 |
| FINI_ARRAY | 终止数组 | 多个终止函数 |
| RPATH | 运行时搜索路径 | 库查找路径 |
| RUNPATH | 运行时路径 | 替代 RPATH |
| SONAME | 共享库名称 | 库标识名 |

### 4.6 版本信息分析

符号版本用于兼容性管理：

```bash
readelf -V program
```

输出示例：

```
Version symbols:
  0000:   0 (*local*)       2 (GLIBC_2.2.5)   0 (*local*)   0 (*local*)
  0020:   0 (*local*)       0 (*local*)       0 (*local*)   0 (*local*)

Version definition section '.gnu.version_d' contains 2 entries:
  Addr: 0x0000000000000488  Offset: 0x000488  Link: 5
    00:00: Rev: 1  Flags: none  Index: 2  Cnt: 1  Name: GLIBC_2.2.5
```

### 4.7 重定位信息分析

重定位信息描述了需要运行时修正的地址：

```bash
readelf -r program
```

输出示例：

```
Relocation section '.rela.plt' contains 3 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000003fe8  000100000007 R_X86_64_JUMP_SLOT 0000000000000000 printf + 0
000000003ff0  000200000007 R_X86_64_JUMP_SLOT 0000000000000000 puts + 0
000000003ff8  000300000007 R_X86_64_JUMP_SLOT 0000000000000000 exit + 0
```

**重定位类型（x86-64）：**

| 类型 | 说明 | 用途 |
|------|------|------|
| R_X86_64_64 | 64 位绝对地址 | 数据引用 |
| R_X86_64_PC32 | PC 相对地址 | 代码引用 |
| R_X86_64_JUMP_SLOT | PLT 跳转槽 | 函数调用 |
| R_X86_64_GLOB_DAT | 全局数据 | 数据访问 |
| R_X86_64_RELATIVE | 相对调整 | 加载基址 |

### 4.8 段内容查看

```bash
# 查看字符串段内容
readelf -p .rodata program

# 查看段十六进制内容
readelf -x .text program

# 查看多个段
readelf -x .text -x .data program

# 查看宽格式输出
readelf -s --wide program
```

### 4.9 调试信息分析

```bash
# 显示 DWARF 调试信息
readelf --debug-dump=info program

# 显示行号信息
readelf --debug-dump=line program

# 显示所有调试信息
readelf --debug-dump=all program
```

## 5. 性能优化

### 5.1 调优策略

**分析段大小优化：**

```bash
# 查看所有段的大小
readelf -S --wide program | awk '{print $2, $6}' | sort -k2 -n -r

# 分析代码段大小
readelf -S program | grep .text

# 分析数据段大小
readelf -S program | grep -E '\.data|\.bss|\.rodata'
```

**内存布局优化：**

```bash
# 检查段对齐
readelf -l program | grep Align

# 分析加载段数量
readelf -l program | grep LOAD | wc -l

# 检查段权限（减少可写可执行段）
readelf -l program | grep -E 'R.*E|W.*E'
```

### 5.2 最佳实践

**使用技巧：**

1. **快速识别文件类型**

```bash
# 判断文件类型
readelf -h program | grep Type

# 输出：
#   Type: EXEC (可执行文件)
#   Type: DYN (共享库)
#   Type: REL (目标文件)
```

2. **查找依赖问题**

```bash
# 列出所有依赖库
readelf -d program | grep NEEDED

# 检查 RPATH 设置
readelf -d program | grep -E 'RPATH|RUNPATH'
```

3. **符号查找技巧**

```bash
# 查找函数符号
readelf -s program | grep FUNC

# 查找未定义符号
readelf -s program | grep UND

# 查找特定符号
readelf -s program | grep main
```

4. **安全特性检查**

```bash
# 检查 RELRO（部分）
readelf -l program | grep GNU_RELRO

# 检查栈保护
readelf -s program | grep __stack_chk_fail

# 检查 PIE
readelf -h program | grep Type | grep DYN
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：找不到依赖库**

```bash
# 查看依赖库列表
readelf -d program | grep NEEDED

# 输出示例：
#  0x0000000000000001 (NEEDED)    Shared library: [libfoo.so.1]
#  0x0000000000000001 (NEEDED)    Shared library: [libc.so.6]

# 检查 RPATH 设置
readelf -d program | grep -E 'RPATH|RUNPATH'
```

**问题 2：符号未定义**

```bash
# 查找未定义符号
readelf -s program | grep UND

# 查看动态符号
readelf -s program | grep -E 'FUNC|OBJECT' | grep UND
```

**问题 3：段权限错误**

```bash
# 检查可写可执行段（安全风险）
readelf -l program | grep -E 'W.*E'

# 正常情况：应该没有 W+E 段
# 如果存在，说明代码段可写，存在安全风险
```

**问题 4：入口点错误**

```bash
# 查看入口点地址
readelf -h program | grep Entry

# 查看入口点对应符号
readelf -s program | grep $(readelf -h program | grep Entry | awk '{print $4}')
```

**问题 5：架构不匹配**

```bash
# 检查文件架构
readelf -h program | grep Machine

# 输出示例：
#   Machine: Advanced Micro Devices X86-64
#   Machine: ARM
#   Machine: RISC-V
```

### 6.2 调试技巧

**技巧 1：对比两个文件的差异**

```bash
# 对比段布局
diff <(readelf -S file1) <(readelf -S file2)

# 对比符号表
diff <(readelf -s file1) <(readelf -s file2)
```

**技巧 2：提取关键信息**

```bash
# 提取所有函数名称
readelf -s program | grep FUNC | awk '{print $NF}'

# 提取全局变量
readelf -s program | grep OBJECT | grep GLOBAL

# 提取依赖库列表
readelf -d program | grep NEEDED | awk -F'[][]' '{print $2}'
```

**技巧 3：生成报告**

```bash
# 生成完整分析报告
(
  echo "=== ELF 文件分析报告 ==="
  echo ""
  echo "文件类型："
  readelf -h program | grep Type
  echo ""
  echo "架构："
  readelf -h program | grep Machine
  echo ""
  echo "入口点："
  readelf -h program | grep Entry
  echo ""
  echo "依赖库："
  readelf -d program | grep NEEDED
  echo ""
  echo "段大小："
  readelf -S program | grep -E '\.text|\.data|\.bss|\.rodata'
) > elf_report.txt
```

## 7. 集成实践

### 7.1 工具链集成

**与 GCC 集成：**

```bash
# 编译后立即检查
gcc -o program program.c && readelf -h program

# Makefile 集成
.PHONY: analyze
analyze: program
	readelf -a program > analysis.txt
```

**与 GDB 集成：**

```bash
# 获取入口点地址供 GDB 使用
ENTRY=$(readelf -h program | grep Entry | awk '{print $4}')
gdb -ex "break *$ENTRY" program
```

**与 objdump 配合：**

```bash
# readelf 分析结构，objdump 反汇编
readelf -S program > sections.txt
objdump -d program > disasm.txt
```

### 7.2 CI/CD 配置

**GitLab CI 示例：**

```yaml
elf-analysis:
  stage: test
  script:
    - readelf -h program
    - readelf -d program | grep NEEDED
    - |
      if readelf -l program | grep -q 'W.*E'; then
        echo "ERROR: Found writable and executable segment"
        exit 1
      fi
  artifacts:
    paths:
      - analysis.txt
```

**Jenkins Pipeline 示例：**

```groovy
stage('ELF Analysis') {
    steps {
        sh '''
            readelf -h program > elf_header.txt
            readelf -d program > dynamic.txt
            readelf -s program > symbols.txt
        '''
    }
    post {
        always {
            archiveArtifacts artifacts: '*.txt'
        }
    }
}
```

### 7.3 实战案例

**案例 1：分析程序依赖**

```bash
#!/bin/bash
# analyze_deps.sh - 分析程序依赖

PROGRAM=$1

echo "=== 依赖库分析 ==="
echo ""
echo "直接依赖："
readelf -d $PROGRAM | grep NEEDED | awk -F'[][]' '{print $2}'

echo ""
echo "RPATH 设置："
readelf -d $PROGRAM | grep -E 'RPATH|RUNPATH'

echo ""
echo "动态链接器："
readelf -l $PROGRAM | grep INTERP -A1 | tail -n1 | awk '{print $1}'
```

**案例 2：安全检查脚本**

```bash
#!/bin/bash
# security_check.sh - ELF 安全特性检查

PROGRAM=$1
PASS=0
FAIL=0

echo "=== 安全特性检查 ==="

# 检查 PIE
if readelf -h $PROGRAM | grep -q "Type:.*DYN"; then
    echo "[PASS] PIE enabled"
    ((PASS++))
else
    echo "[FAIL] PIE not enabled"
    ((FAIL++))
fi

# 检查 RELRO
if readelf -l $PROGRAM | grep -q GNU_RELRO; then
    echo "[PASS] RELRO enabled"
    ((PASS++))
else
    echo "[FAIL] RELRO not enabled"
    ((FAIL++))
fi

# 检查栈保护
if readelf -s $PROGRAM | grep -q __stack_chk_fail; then
    echo "[PASS] Stack canary enabled"
    ((PASS++))
else
    echo "[FAIL] Stack canary not enabled"
    ((FAIL++))
fi

echo ""
echo "总计: $PASS 通过, $FAIL 失败"
```

**案例 3：段大小分析脚本**

```bash
#!/bin/bash
# section_size.sh - 分析段大小

PROGRAM=$1

echo "=== 段大小分析 ==="
echo ""

# 提取段名和大小
readelf -S --wide $PROGRAM | awk '{
    if ($1 ~ /^\[/) {
        name = $2
        for (i=3; i<=NF; i++) {
            if ($i ~ /^0x[0-9a-f]+$/) {
                size = $i
                break
            }
        }
        if (size > 0) {
            printf "%-20s %10d bytes\n", name, strtonum(size)
        }
    }
}' | sort -k2 -n -r | head -10
```

**案例 4：多架构检查**

```bash
#!/bin/bash
# check_arch.sh - 检查多架构库

ARCHS=("x86_64" "i386" "arm" "aarch64")

echo "=== 架构检查 ==="
echo ""

for lib in *.so; do
    if [ -f "$lib" ]; then
        echo "文件: $lib"
        MACHINE=$(readelf -h $lib 2>/dev/null | grep Machine | awk -F: '{print $2}')
        echo "  架构:$MACHINE"
        echo ""
    fi
done
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| binutils 官方文档 | https://sourceware.org/binutils/docs/ |
| readelf 手册页 | `man readelf` |
| ELF 格式规范 | https://refspecs.linuxfoundation.org/elf/elf.pdf |
| LSB 规范 | https://refspecs.linuxbase.org/ |

### 8.2 学习路径

**初级路径：**
1. 理解 ELF 文件格式基础
2. 掌握 readelf 基本命令
3. 学会查看文件头、段头、符号表
4. 理解动态链接基础

**中级路径：**
1. 深入理解段布局和加载过程
2. 掌握重定位机制
3. 学习符号版本管理
4. 实践依赖分析和问题排查

**高级路径：**
1. 研究 ELF 格式细节和扩展
2. 安全特性分析（RELRO、PIE、Stack Canary）
3. 性能优化和段布局调优
4. 自定义工具链集成和自动化脚本

### 8.3 相关工具

| 工具 | 用途 | 说明 |
|------|------|------|
| objdump | 反汇编和查看 | 支持反汇编，readelf 不支持 |
| nm | 符号表查看 | 更简洁的符号输出 |
| ldd | 动态依赖查看 | 直接运行解析依赖 |
| file | 文件类型识别 | 快速判断文件类型 |
| strip | 符号移除 | 减小文件大小 |
| objcopy | 段操作 | 提取/修改段 |

### 8.4 调试信息参考

readelf 支持 DWARF 调试信息解析：

```bash
# 显示调试信息段
readelf -S program | grep debug

# 常见调试段
# .debug_info     - 主要调试信息
# .debug_line     - 行号信息
# .debug_str      - 字符串表
# .debug_abbrev   - 缩写表
# .debug_aranges  - 地址范围
```

---

**总结：** readelf 是 ELF 文件分析的核心工具，通过系统掌握其各项功能，可以深入理解程序的内部结构、解决链接问题、优化性能并增强安全性。建议结合 objdump、nm 等工具配合使用，形成完整的二进制分析工具链。