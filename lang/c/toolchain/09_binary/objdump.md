# objdump - 二进制反汇编工具

## 1. 概述与背景

### 1.1 工具定位

objdump 是 GNU Binary Utilities (binutils) 工具链提供的二进制分析工具，主要用于反汇编可执行文件、查看符号表、分析段信息和重定位表。作为逆向工程、调试分析和安全审计的核心工具，objdump 提供了对 ELF/COFF/Mach-O 等多种二进制格式的深入分析能力。

### 1.2 核心特性

| 特性 | 说明 |
|------|------|
| 反汇编能力 | 支持多种 CPU 架构（x86、ARM、MIPS、PowerPC 等） |
| 格式支持 | ELF、COFF、PE、Mach-O 等主流二进制格式 |
| 符号分析 | 静态符号表和动态符号表解析 |
| 段信息查看 | 显示所有段头、段内容和段属性 |
| 重定位分析 | 静态重定位和动态重定位查看 |
| 源码关联 | 支持源码与汇编混合显示（需调试信息） |
| 架构独立 | 支持交叉分析不同架构的二进制文件 |

### 1.3 适用场景

| 场景 | 说明 |
|------|------|
| 逆向工程 | 分析闭源程序的工作原理和算法实现 |
| 调试分析 | 配合调试器定位程序崩溃和异常行为 |
| 安全审计 | 发现二进制文件中的安全漏洞和风险点 |
| 性能优化 | 分析热点代码，指导手动优化 |
| 编译器验证 | 检查编译器生成的汇编代码质量 |
| 动态库分析 | 查看动态库的符号导出和依赖关系 |

### 1.4 对比分析

| 工具 | 优势 | 劣势 | 适用场景 |
|------|------|------|---------|
| objdump | 反汇编能力强、架构支持广 | 输出格式较原始 | 反汇编分析、交叉编译 |
| readelf | ELF 格式解析详细 | 仅支持 ELF 格式 | ELF 结构分析 |
| nm | 符号表查看简单快速 | 功能单一 | 符号查找 |
| IDA Pro | 图形化界面、自动分析 | 商业软件、昂贵 | 专业逆向工程 |
| Ghidra | 免费、功能强大 | 学习曲线陡峭 | 复杂逆向分析 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# Linux (Debian/Ubuntu)
sudo apt update
sudo apt install binutils

# Linux (CentOS/RHEL)
sudo yum install binutils

# Linux (Arch Linux)
sudo pacman -S binutils

# macOS
# macOS 默认使用 otool，可通过 Homebrew 安装 binutils
brew install binutils
# 安装后命令为 gobjdump
gobjdump --version

# Windows (MinGW)
# 随 MinGW-w64 安装
# 下载地址：https://sourceforge.net/projects/mingw-w64/

# Windows (WSL)
sudo apt install binutils

# 从源码编译
wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz
tar xf binutils-2.42.tar.xz
cd binutils-2.42
./configure --prefix=/usr/local
make
sudo make install
```

### 2.2 版本验证

```bash
# 查看版本
objdump --version

# 查看支持的架构
objdump --help | grep "supported targets"

# 查看支持的选项
objdump --help
```

输出示例：
```
GNU objdump (GNU Binutils) 2.42
Copyright (C) 2024 Free Software Foundation, Inc.
...
支持的架构: i386, i386:x86-64, i386:x86-64:32, i8086, iamcu, l1om, k1om
```

### 2.3 环境配置

```bash
# 配置 PATH（如果未自动添加）
export PATH=$PATH:/usr/local/bin

# 设置默认架构
export OBJDUMP_DEFAULT_ARCH=x86_64

# 添加别名
alias od='objdump'
alias odd='objdump -d'
alias odh='objdump -h'
alias odt='objdump -t'
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 创建测试程序
cat > test.c << 'EOF'
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(1, 2);
    printf("Result: %d\n", result);
    return 0;
}
EOF

# 编译（带调试信息）
gcc -g test.c -o test

# 反汇编整个程序
objdump -d test

# 反汇编并显示源码
objdump -d -S test

# 查看符号表
objdump -t test

# 查看所有段信息
objdump -x test
```

### 3.2 常用选项详解

| 选项 | 说明 | 示例 |
|------|------|------|
| `-d` | 反汇编可执行段（.text） | `objdump -d program` |
| `-D` | 反汇编所有段 | `objdump -D program` |
| `-S` | 源码与汇编混合显示 | `objdump -d -S program` |
| `-l` | 显示源码文件名和行号 | `objdump -d -l program` |
| `-x` | 显示所有头信息 | `objdump -x program` |
| `-h` | 显示段头表 | `objdump -h program` |
| `-t` | 显示符号表 | `objdump -t program` |
| `-T` | 显示动态符号表 | `objdump -T program` |
| `-r` | 显示重定位信息 | `objdump -r object.o` |
| `-R` | 显示动态重定位 | `objdump -R program` |
| `-p` | 显示程序头 | `objdump -p program` |
| `-C` | C++ 符号解码（demangle） | `objdump -t -C program` |
| `-M intel` | 使用 Intel 语法 | `objdump -d -M intel program` |
| `-j .text` | 仅处理指定段 | `objdump -d -j .text program` |
| `--start-address=ADDR` | 起始地址 | `objdump -d --start-address=0x1000 program` |
| `--stop-address=ADDR` | 结束地址 | `objdump -d --stop-address=0x2000 program` |

### 3.3 反汇编输出解析

#### 3.3.1 基本反汇编

```bash
objdump -d test
```

输出示例与解析：
```
test:     file format elf64-x86-64      # 文件格式

Disassembly of section .text:           # 段名称

0000000000001149 <main>:                # 函数地址和名称
    1149:	f3 0f 1e fa              endbr64           # 地址、机器码、指令
    114d:	55                       push   %rbp       # 保存栈帧指针
    114e:	48 89 e5                 mov    %rsp,%rbp  # 设置新栈帧
    1151:	48 83 ec 10              sub    $0x10,%rsp # 分配栈空间
    1155:	be 02 00 00 00           mov    $0x2,%esi  # 第二个参数
    115a:	bf 01 00 00 00           mov    $0x1,%edi  # 第一个参数
    115f:	e8 ce ff ff ff           call   1132 <add> # 调用函数
    1164:	89 45 fc                 mov    %eax,-0x4(%rbp) # 保存返回值
    1167:	8b 45 fc                 mov    -0x4(%rbp),%eax  # 读取返回值
    116a:	c9                       leaveq            # 恢复栈帧
    116b:	c3                       retq              # 返回
```

#### 3.3.2 Intel 语法输出

```bash
# 使用 Intel 语法（更易读）
objdump -d -M intel test
```

输出示例：
```
0000000000001149 <main>:
    1149:	f3 0f 1e fa              endbr64
    114d:	55                       push   rbp
    114e:	48 89 e5                 mov    rbp,rsp
    1151:	48 83 ec 10              sub    rsp,0x10
    1155:	be 02 00 00 00           mov    esi,0x2
    115a:	bf 01 00 00 00           mov    edi,0x1
    115f:	e8 ce ff ff ff           call   1132 <add>
```

#### 3.3.3 源码混合显示

```bash
# 需要编译时带 -g 调试信息
objdump -d -S test
```

输出示例：
```c
int add(int a, int b) {
    1132:	f3 0f 1e fa              endbr64
    1136:	55                       push   %rbp
    1137:	48 89 e5                 mov    %rsp,%rbp
    113a:	89 7d fc                 mov    %edi,-0x4(%rbp)
    113d:	89 75 f8                 mov    %esi,-0x8(%rbp)
    return a + b;
    1140:	8b 55 fc                 mov    -0x4(%rbp),%edx
    1143:	8b 45 f8                 mov    -0x8(%rbp),%eax
    1146:	01 d0                    add    %edx,%eax
}
```

### 3.4 段信息分析

```bash
# 查看段头表
objdump -h test
```

输出示例：
```
Idx Name          Size      VMA               LMA               File off  Algn
  0 .interp       0000001c  0000000000000238  0000000000000238  00000238  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA

  1 .text         00000085  0000000000001040  0000000000001040  00001040  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE

  2 .rodata       00000020  0000000000002000  0000000000002000  00002000  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, DATA

  3 .data         00000010  0000000000003000  0000000000003000  00003000  2**4
                  CONTENTS, ALLOC, LOAD, DATA

  4 .bss          00000008  0000000000003010  0000000000003010  00003010  2**4
                  ALLOC
```

段属性说明：
| 标志 | 说明 |
|------|------|
| CONTENTS | 段在文件中有内容 |
| ALLOC | 运行时需要分配内存 |
| LOAD | 需要加载到内存 |
| READONLY | 只读 |
| CODE | 可执行代码 |
| DATA | 数据段 |

## 4. 进阶特性

### 4.1 符号表分析

#### 4.1.1 静态符号表

```bash
# 查看所有符号
objdump -t test

# 筛选函数符号
objdump -t test | grep ' F '

# 筛选未定义符号（外部依赖）
objdump -t test | grep '*UND*'

# 查看 C++ 符号（自动解码）
objdump -t -C test
```

符号类型说明：
```
0000000000001132 g     F .text  00000012              add
|               |     | |      |                     |
|               |     | |      |                     符号名
|               |     | |      符号大小
|               |     | |所在段
|               |     | 符号类型（F=函数, O=对象, etc.）
|               |     符号绑定（g=全局, l=本地, w=弱符号）
|               符号属性
符号值（地址）
```

#### 4.1.2 动态符号表

```bash
# 查看动态符号
objdump -T test

# 查看动态库导出符号
objdump -T libmylib.so

# 查看动态库导入符号
objdump -T program | grep 'UND'
```

输出示例：
```
DYNAMIC SYMBOL TABLE:
0000000000000000      DF *UND*  00000000  GLIBC_2.2.5  printf
0000000000000000      DF *UND*  00000000  GLIBC_2.2.5  __libc_start_main
0000000000001149 g    DF .text  0000002a  Base        main
```

### 4.2 重定位分析

#### 4.2.1 静态重定位

```bash
# 查看目标文件重定位信息
objdump -r test.o
```

输出示例：
```
RELOCATION RECORDS FOR [.text]:
OFFSET           TYPE                     VALUE
0000000000000007 R_X86_64_32              .rodata
000000000000000d R_X86_64_PC32            add
0000000000000016 R_X86_64_PLT32            printf-0x0000000000000004
```

重定位类型说明：
| 类型 | 说明 |
|------|------|
| R_X86_64_64 | 64 位绝对地址 |
| R_X86_64_PC32 | 32 位 PC 相对地址 |
| R_X86_64_PLT32 | PLT 表项相对地址 |
| R_X86_64_GOTPCRELX | GOT 表项相对地址 |

#### 4.2.2 动态重定位

```bash
# 查看可执行文件动态重定位
objdump -R test
```

输出示例：
```
DYNAMIC RELOCATION RECORDS
OFFSET           TYPE                     VALUE
0000000000003db8 R_X86_64_RELATIVE       *ABS*+0x0000000000001130
0000000000004018 R_X86_64_JUMP_SLOT       printf@GLIBC_2.2.5
0000000000004020 R_X86_64_JUMP_SLOT       __libc_start_main@GLIBC_2.2.5
```

### 4.3 特定函数反汇编

```bash
# 反汇编特定函数
objdump -d test | grep -A 30 "<main>"

# 使用 --disassemble 选项（更精确）
objdump --disassemble=main test

# 反汇编地址范围
objdump -d --start-address=0x1149 --stop-address=0x1180 test

# 反汇编并显示行号
objdump -d -l test | grep -A 20 "main():"
```

### 4.4 多架构支持

```bash
# 显示支持的目标格式
objdump --help | grep "supported targets"

# 指定目标架构
objdump -d -m i386:x86-64 test

# 反汇编 ARM 二进制
arm-linux-gnueabi-objdump -d arm_program

# 反汇编 MIPS 二进制
mips-linux-gnu-objdump -d mips_program
```

## 5. 动态库分析

### 5.1 查看动态库依赖

```bash
# 查看程序依赖的动态库
objdump -p test | grep NEEDED

# 查看动态库依赖
objdump -p libmylib.so | grep NEEDED
```

输出示例：
```
  NEEDED      libc.so.6
  NEEDED      libpthread.so.0
  NEEDED      libm.so.6
```

### 5.2 查看动态库版本

```bash
# 查看版本需求
objdump -p test | grep VERSION

# 查看符号版本
objdump -T test | grep GLIBC
```

### 5.3 分析动态库结构

```bash
# 查看所有程序头
objdump -p libmylib.so

# 查看 .dynamic 段
objdump -p libmylib.so | grep -A 50 "Dynamic Section"

# 查看导出符号
objdump -T libmylib.so | grep ' DF '
```

## 6. 性能优化与大文件处理

### 6.1 处理大型二进制文件

```bash
# 仅反汇编需要的段
objdump -d -j .text large_program

# 仅反汇编需要的函数
objdump --disassemble=main large_program

# 使用地址范围
objdump -d --start-address=0x1000 --stop-address=0x2000 large_program

# 输出到文件避免终端缓冲
objdump -d large_program > disasm.txt
```

### 6.2 筛选优化

```bash
# 筛选特定段
objdump -h program | grep .text

# 筛选特定符号类型
objdump -t program | grep ' F ' | grep -v 'UND'

# 使用 awk 处理输出
objdump -t program | awk '$2=="g" && $3=="F" {print}'

# 提取函数列表
objdump -t program | awk '$3=="F" && $4==".text" {print $NF}' | sort
```

### 6.3 批量处理脚本

```bash
#!/bin/bash
# disasm_all.sh - 批量反汇编目录下所有可执行文件

for file in "$@"; do
    if [ -f "$file" ] && file "$file" | grep -q "executable"; then
        echo "=== Disassembling $file ==="
        objdump -d "$file" > "${file}.disasm"
    fi
done
```

## 7. 问题排查

### 7.1 常见问题

#### 问题 1：找不到符号信息

```bash
# 问题：objdump -t 显示符号很少
# 原因：编译时使用了 -s（strip）选项

# 解决方法：查看动态符号表
objdump -T program

# 或查看字符串
strings program | grep -i function_name
```

#### 问题 2：无法反汇编

```bash
# 问题：objdump: can't disassemble for architecture
# 原因：架构不匹配

# 解决方法：指定正确架构
file program  # 查看文件类型
objdump -d -m i386:x86-64 program

# 使用交叉编译工具链
arm-linux-gnueabi-objdump -d arm_program
```

#### 问题 3：C++ 符号乱码

```bash
# 问题：符号显示为 _Z4funci
# 原因：未使用 C++ 符号解码

# 解决方法：使用 -C 选项
objdump -t -C program

# 或使用 c++filt
objdump -t program | c++filt
```

#### 问题 4：没有调试信息

```bash
# 问题：objdump -S 不显示源码
# 原因：编译时未带 -g 选项

# 解决方法：重新编译
gcc -g program.c -o program

# 或使用 addr2line 配合源码
addr2line -e program 0x1149
```

### 7.2 调试技巧

```bash
# 查找特定字符串引用
objdump -d program | grep -B5 -A5 "string_address"

# 分析函数调用关系
objdump -d program | grep "call.*<.*>"

# 查找跳转目标
objdump -d program | grep "jmp\|je\|jne\|jg\|jl"

# 统计指令使用频率
objdump -d program | awk '{print $3}' | sort | uniq -c | sort -rn | head -20
```

## 8. 实战案例

### 8.1 案例 1：分析函数调用栈

```bash
# 反汇编并保存
objdump -d -M intel program > disasm.txt

# 查找函数入口点
grep "^[0-9a-f].*<.*>:" disasm.txt

# 追踪特定函数调用
grep -A 50 "<function_name>:" disasm.txt | grep "call"

# 分析返回地址
grep -A 5 "ret" disasm.txt
```

### 8.2 案例 2：定位段错误

```bash
# 1. 获取崩溃地址（从 core dump 或日志）
crash_addr=0x555555555149

# 2. 查找所在函数
objdump -t program | awk '$1 ~ /^[0-9a-f]+$/ && $3=="F" && $4==".text" {
    if (strtonum("0x"$1) <= strtonum("0x555555555149") &&
        strtonum("0x"$1)+strtonum("0x"$5) > strtonum("0x555555555149"))
        print $0
}'

# 3. 反汇编该函数
objdump -d program | grep -B5 -A30 "<function_name>:"

# 4. 结合 addr2line 定位源码
addr2line -e program -f 0x555555555149
```

### 8.3 案例 3：检查编译优化

```bash
# 编译两个版本
gcc -O0 program.c -o program_O0
gcc -O3 program.c -o program_O3

# 对比函数大小
objdump -t program_O0 | grep "main"
objdump -t program_O3 | grep "main"

# 对比汇编代码
objdump -d program_O0 > O0.asm
objdump -d program_O3 > O3.asm
diff O0.asm O3.asm
```

### 8.4 案例 4：分析动态库加载

```bash
# 查看依赖库
objdump -p program | grep NEEDED

# 查看符号绑定
objdump -T program

# 检查是否使用了特定函数
objdump -T program | grep "function_name"

# 分析 PLT/GOT
objdump -d -j .plt program
objdump -s -j .got program
```

## 9. 工具对比

### 9.1 objdump vs readelf

| 功能 | objdump | readelf |
|------|---------|---------|
| 反汇编代码 | ✅ `-d` | ❌ |
| ELF 头信息 | ✅ `-x` | ✅ `-h` |
| 段头信息 | ✅ `-h` | ✅ `-S` |
| 程序头 | ✅ `-p` | ✅ `-l` |
| 符号表 | ✅ `-t/-T` | ✅ `-s` |
| 重定位信息 | ✅ `-r/-R` | ✅ `-r` |
| 动态段 | ✅ `-p` | ✅ `-d` |
| 架构支持 | 多架构 | 仅 ELF |

### 9.2 objdump vs otool (macOS)

| 功能 | objdump | otool |
|------|---------|-------|
| 反汇编 | `-d` | `-tV` |
| 段头 | `-h` | `-l` |
| 符号表 | `-t` | `-tv` |
| 动态库依赖 | `-p \| grep NEEDED` | `-L` |
| Mach-O 支持 | 需要交叉版本 | ✅ 原生 |

```bash
# macOS otool 常用命令
otool -tV program         # 反汇编
otool -L program          # 动态库依赖
otool -l program          # 段信息
otool -hv program         # Mach-O 头
otool -I program         # 间接符号表
```

### 9.3 objdump vs nm

| 功能 | objdump | nm |
|------|---------|-----|
| 符号列表 | `-t` | 默认 |
| 符号排序 | 需要 `sort` | `-n` |
| 符号类型 | 详细信息 | 简洁 |
| C++ 解码 | `-C` | `-C` |
| 动态符号 | `-T` | `-D` |

```bash
# nm 快速符号查看
nm program
nm -C program          # C++ 解码
nm -D program          # 动态符号
nm -n program          # 按地址排序
nm -S program          # 显示符号大小
```

## 10. 最佳实践

### 10.1 反汇编技巧

```bash
# 使用 Intel 语法更易读
alias objdump-intel='objdump -d -M intel'

# 创建便捷别名
alias odd='objdump -d'
alias oddi='objdump -d -M intel'
alias ods='objdump -d -S'
alias odh='objdump -h'
alias odt='objdump -t'

# 反汇编并保存为文件
objdump -d -M intel -S program > program.asm

# 提取所有函数名
objdump -t program | awk '$3=="F" && $4==".text" {print $NF}'
```

### 10.2 配合其他工具

```bash
# 与 grep 配合筛选
objdump -d program | grep -A 20 "<main>:"

# 与 awk 配合处理
objdump -t program | awk '/F .text/{print}'

# 与 less 配合浏览
objdump -d program | less

# 与 vim 配合编辑
objdump -d program | vim -

# 与 gdb 配合调试
gdb program
(gdb) disassemble main
```

### 10.3 输出格式化

```bash
# 生成函数调用图
objdump -d program | \
  awk '/^[0-9a-f].*<.*>:/ {func=$0} /call.*</ {print func " -> " $0}'

# 提取函数大小统计
objdump -t program | \
  awk '$3=="F" && $4==".text" {size=strtonum("0x"$5); if(size>0) print $NF, size}' | \
  sort -k2 -n -r | head -20

# 统计指令使用
objdump -d program | \
  awk '{print $3}' | sort | uniq -c | sort -rn | head -20
```

## 11. 参考资源

### 11.1 官方文档

- GNU Binutils 官方手册: https://sourceware.org/binutils/docs/
- objdump 手册页: `man objdump`
- ELF 格式规范: https://refspecs.linuxfoundation.org/elf/elf.pdf

### 11.2 学习资源

- 《程序员的自我修养：链接、装载与库》
- 《深入理解计算机系统》（CSAPP）
- GNU Binutils 源码: https://sourceware.org/git/?p=binutils-gdb.git

### 11.3 相关工具

| 工具 | 用途 |
|------|------|
| readelf | ELF 文件详细分析 |
| nm | 符号表查看 |
| strings | 提取可打印字符串 |
| file | 文件类型识别 |
| size | 查看段大小 |
| strip | 去除符号信息 |
| addr2line | 地址转源码行号 |
| c++filt | C++ 符号解码 |

### 11.4 调试信息格式

| 格式 | 说明 | 编译选项 |
|------|------|---------|
| DWARF | 现代标准格式 | `-g` (默认) |
| STABS | 旧格式 | `-gstabs` |
| COFF | Windows 格式 | `-gcoff` |

```bash
# 查看 DWARF 调试信息
readelf --debug-dump=info program
objdump -W program
```