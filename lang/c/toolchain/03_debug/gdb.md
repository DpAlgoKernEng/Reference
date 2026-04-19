# GDB - GNU 调试器

## 1. 概述与背景

### 1.1 工具定位

GDB（GNU Debugger）是 GNU 项目开发的旗舰级调试器，是 Unix/Linux 平台上最广泛使用的 C/C++ 程序调试工具。作为 GNU 工具链的核心组件，GDB 提供了强大的程序分析能力，支持源码级调试、汇编级调试、内存分析、多线程调试等功能。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1986 | 1.0 | Richard Stallman 创建 GDB 初始版本 |
| 1990 | 4.0 | 重大重构，支持多种架构 |
| 2000 | 5.0 | 引入 Python 脚本支持 |
| 2009 | 7.0 | 增加反向调试功能 |
| 2015 | 7.10 | 改进多线程调试支持 |
| 2020 | 9.0 | 增强远程调试能力 |
| 2023 | 13.0 | 现代 C++ 支持、性能优化 |

### 1.3 核心特性

**源码级调试**：支持 C、C++、Go、Rust、Fortran 等多种语言。

**多架构支持**：x86、ARM、MIPS、RISC-V、PowerPC 等主流架构。

**执行控制**：断点、单步执行、条件断点、观察点。

**内存分析**：内存查看、内存泄漏检测、堆栈分析。

**高级功能**：反向调试、核心转储分析、远程调试、Python 脚本扩展。

**多进程/多线程**：完善的线程管理和进程调试能力。

### 1.4 适用场景

**开发阶段**：单步调试、变量检查、逻辑验证。

**Bug 排查**：崩溃分析、内存错误定位、竞态条件检测。

**性能分析**：热点定位、函数调用分析、内存使用分析。

**逆向工程**：二进制分析、汇编级调试、协议逆向。

### 1.5 对比分析

| 调试器 | 优势 | 劣势 |
|--------|------|------|
| **GDB** | 开源免费、跨平台、功能强大、社区成熟 | 学习曲线陡峭、命令行界面复杂 |
| LLDB | LLVM 生态、现代化架构、macOS 默认 | Linux 支持不如 GDB 成熟 |
| Visual Studio Debugger | GUI 友好、可视化强大 | 仅限 Windows、体积大 |
| WinDbg | Windows 内核调试能力强 | 仅限 Windows、界面陈旧 |
| Valgrind | 内存检测专业 | 性能开销大、无源码调试 |

## 2. 安装与配置

### 2.1 多平台安装

**Linux（Ubuntu/Debian）**
```bash
# 安装 GDB
sudo apt update
sudo apt install gdb

# 安装调试符号支持
sudo apt install gdb-multiarch

# 安装 GDB 增强工具
sudo apt install gdb-dashboard
```

**Linux（CentOS/RHEL）**
```bash
sudo yum install gdb
sudo yum install gdb-gdbserver  # 远程调试支持
```

**macOS**
```bash
# 安装 GDB
brew install gdb

# macOS 需要签名（详见问题排查章节）
codesign --sign gdb-cert $(which gdb)
```

**Windows（MinGW）**
```bash
# MinGW 安装时自带 GDB
# 或单独安装
pacman -S mingw-w64-x86_64-gdb
```

**Windows（WSL）**
```bash
# 在 WSL 中安装 Linux 版本
sudo apt install gdb
```

### 2.2 版本管理

```bash
# 查看版本
gdb --version

# 查看配置信息
gdb --configuration

# 查看支持的架构
gdb --show-config
```

### 2.3 环境配置

**创建 .gdbinit 配置文件**

```bash
# ~/.gdbinit - GDB 全局配置文件

# 基础设置
set pagination off          # 关闭分页
set print pretty on         # 美化输出
set print array on          # 数组格式化
set print array-indexes on  # 显示数组索引

# 历史记录
set history save on
set history size 10000
set history filename ~/.gdb_history

# 编码设置
set charset UTF-8
set target-charset UTF-8

# 反汇编设置
set disassembly-flavor intel  # Intel 语法

# 自动加载安全路径
set auto-load safe-path /

# 启动消息
set startup-quietly on
```

**自定义命令定义**

```bash
# ~/.gdbinit 中定义自定义命令

# 打印链表
define plist
  set $node = $arg0
  while $node
    print *$node
    set $node = $node->next
  end
end

# 打印字符串数组
define parray
  set $arr = $arg0
  set $len = $arg1
  set $i = 0
  while $i < $len
    printf "arr[%d] = %s\n", $i, $arr[$i]
    set $i = $i + 1
  end
end

# 文档说明
document plist
Print all elements of a linked list.
Usage: plist <head_pointer>
end
```

### 2.4 验证安装

```bash
# 编译测试程序
cat > test.c << 'EOF'
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 10;
    int y = 20;
    int result = add(x, y);
    printf("Result: %d\n", result);
    return 0;
}
EOF

gcc -g test.c -o test

# 验证 GDB 功能
gdb -batch -ex "break main" -ex "run" -ex "print x" -ex "quit" ./test
```

## 3. 基础使用

### 3.1 快速入门

**编译准备**

```bash
# 编译时必须包含调试信息
gcc -g program.c -o program

# 推荐：禁用优化，保留调试信息
gcc -g -O0 program.c -o program

# 完整调试信息（包含宏定义）
gcc -g3 program.c -o program

# 分离调试符号（减小可执行文件体积）
objcopy --only-keep-debug program program.debug
strip program
objcopy --add-gnu-debuglink=program.debug program
```

**启动 GDB**

```bash
# 方式 1：直接调试可执行文件
gdb ./program

# 方式 2：带参数启动
gdb --args ./program arg1 arg2 arg3

# 方式 3：分析核心转储文件
gdb ./program core

# 方式 4：附加到运行中的进程
gdb -p <pid>

# 方式 5：远程调试
gdb ./program
target remote <host>:<port>
```

### 3.2 项目结构

**GDB 相关文件**

```
project/
├── .gdbinit           # 项目级 GDB 配置
├── .gdb_commands      # GDB 命令脚本
├── src/
│   └── main.c
├── build/
│   └── program        # 编译后的可执行文件
└── core.*             # 核心转储文件
```

**编译选项配置**

```bash
# CMakeLists.txt
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")

# Makefile
CFLAGS += -g -O0
CXXFLAGS += -g -O0
```

### 3.3 基本命令

**执行控制命令**

| 命令 | 缩写 | 说明 | 示例 |
|------|------|------|------|
| `run [args]` | `r` | 启动程序 | `run --config app.conf` |
| `start` | | 启动并停在 main | `start` |
| `continue` | `c` | 继续执行 | `continue` |
| `next` | `n` | 单步（不进入函数） | `next 5` |
| `step` | `s` | 单步（进入函数） | `step` |
| `finish` | `fin` | 执行到函数返回 | `finish` |
| `until` | `u` | 执行到某行 | `until 100` |
| `jump` | `j` | 跳转到指定行 | `jump 50` |
| `quit` | `q` | 退出 GDB | `quit` |

**断点管理命令**

| 命令 | 说明 | 示例 |
|------|------|------|
| `break main` | 在函数设断点 | `break main` |
| `break file.c:42` | 在文件行号设断点 | `break utils.c:100` |
| `break func if cond` | 条件断点 | `break foo if x > 100` |
| `break *address` | 在地址设断点 | `break *0x4005a0` |
| `info breakpoints` | 查看断点列表 | `info breakpoints` |
| `delete [num]` | 删除断点 | `delete 1` |
| `disable [num]` | 禁用断点 | `disable 1-5` |
| `enable [num]` | 启用断点 | `enable once 2` |
| `clear` | 清除所有断点 | `clear main` |

### 3.4 常用操作

**变量查看与修改**

```bash
# 打印变量
print var
print *ptr
print arr[0]@10          # 打印数组前 10 个元素
print/x var              # 十六进制格式
print/d var              # 十进制格式
print/t var              # 二进制格式
print/c var              # 字符格式

# 打印结构体
print *struct_ptr
print struct_var.member

# 打印 STL 容器
set print pretty on
print vec
print map
print "string"

# 修改变量值
set var = 100
set ptr->value = 200

# 自动显示
display var              # 每次停止时显示变量
display/x var            # 十六进制格式
undisplay 1              # 取消自动显示
```

**调用栈操作**

```bash
# 查看调用栈
backtrace                # 或 bt
backtrace full           # 显示局部变量
backtrace 10             # 显示前 10 层

# 切换栈帧
frame 2                  # 切换到栈帧 2
up                       # 上移一层
down                     # 下移一层
info frame               # 显示当前栈帧信息
info locals              # 显示局部变量
info args                # 显示函数参数
```

**内存查看**

```bash
# 内存查看命令格式：x/NFU address
# N: 数量, F: 格式, U: 单位

# 十六进制
x/10x &var               # 10 个十六进制字（word）
x/10xb &var              # 10 个十六进制字节
x/10xw &var              # 10 个十六进制字（4字节）

# 指令
x/10i main               # main 函数前 10 条指令
x/20i $pc                # 当前 PC 位置 20 条指令

# 字符串
x/s str                  # 打印字符串
x/10s arr                # 打印 10 个字符串

# 内存映射
info proc mappings       # 显示内存映射
info target              # 显示目标信息
```

**监视点**

```bash
# 监视变量写入
watch variable

# 监视变量读取
rwatch variable

# 监视变量读写
awatch variable

# 监视表达式
watch arr[i]             # 监视数组元素
watch ptr->value         # 监视结构体成员

# 查看监视点
info watchpoints
```

## 4. 进阶特性

### 4.1 高级配置

**条件断点与命令断点**

```bash
# 条件断点
break foo if x > 100
break bar if strcmp(str, "error") == 0

# 命令断点（触发时执行命令）
break main
commands 1
  print x
  print y
  continue
end

# 忽略断点 N 次
ignore 1 100             # 忽略断点 1 共 100 次
```

**反向调试**

```bash
# 启用录制
record                   # 开始录制执行过程
record full              # 完整录制（内存 + 指令）

# 反向执行
reverse-continue         # 反向继续执行
reverse-next             # 反向单步（不进入函数）
reverse-step             # 反向单步（进入函数）
reverse-finish           # 反向执行到函数调用处

# 停止录制
record stop
```

**核心转储分析**

```bash
# 生成核心转储
ulimit -c unlimited      # 允许生成 core 文件

# 分析核心转储
gdb ./program core.12345

# 分析命令
bt                       # 查看崩溃时的调用栈
info registers           # 查看寄存器状态
info signals             # 查看信号信息
thread apply all bt      # 所有线程的调用栈
```

### 4.2 扩展功能

**Python 脚本扩展**

```python
# ~/.gdbinit 中加载
source ~/gdb_scripts.py

# gdb_scripts.py
import gdb

class PrintList(gdb.Command):
    """打印链表所有节点"""
    
    def __init__(self):
        super(PrintList, self).__init__("plist", gdb.COMMAND_DATA)
    
    def invoke(self, arg, from_tty):
        node = gdb.parse_and_eval(arg)
        while node:
            print(node.dereference())
            node = node['next']

PrintList()

# 自定义美化和打印器
class MyVectorPrinter:
    def __init__(self, val):
        self.val = val
    
    def to_string(self):
        size = self.val['_M_impl']['_M_finish'] - self.val['_M_impl']['_M_start']
        return f"MyVector of size {size}"

def lookup_type(val):
    if str(val.type) == 'std::vector':
        return MyVectorPrinter(val)
    return None

gdb.pretty_printers.append(lookup_type)
```

**GDB 命令脚本**

```bash
# debug_script.gdb
# 自动化调试脚本

# 加载符号
file ./program

# 设置断点
break main
break foo
break bar if x > 10

# 设置观察点
watch global_counter

# 运行参数
set args -v --config app.conf

# 启动程序
run

# 执行命令序列
next 5
print x
print y
continue

# 退出
quit
```

**使用方式**

```bash
# 方式 1：命令行加载
gdb -x debug_script.gdb

# 方式 2：GDB 内加载
gdb
source debug_script.gdb
```

### 4.3 插件生态

**GDB Dashboard**

```bash
# 安装
git clone https://github.com/cyrus-and/gdb-dashboard
# 将 dashboard.py 复制到 ~/.gdbinit.d/

# 配置 ~/.gdbinit
source ~/.gdbinit.d/dashboard.py

# 功能
dashboard -assembly  # 显示汇编
dashboard -memory    # 显示内存
dashboard -stack     # 显示栈
dashboard -threads   # 显示线程
```

**GEF（GDB Enhanced Features）**

```bash
# 安装
bash -c "$(curl -fsSL https://gef.blah.cat/sh)"

# 特性
# - 丰富的颜色输出
# - 漏洞检测辅助
# - 堆可视化
# - 寄存器状态显示

# 常用命令
heap chunks            # 查看堆块
heap bins              # 查看堆 bins
vmmap                  # 内存映射
checksec               # 安全检查
```

**PEDA（Python Exploit Development Assistance）**

```bash
# 安装
git clone https://github.com/longld/peda.git ~/peda
echo "source ~/peda/peda.py" >> ~/.gdbinit

# 常用命令
aslr                   # ASLR 状态
checksec               # 安全检查
pattern_create 200     # 创建 pattern
pattern_offset <value> # 计算 offset
```

### 4.4 TUI 模式

**启动 TUI**

```bash
# 启动时启用 TUI
gdb -tui ./program

# 运行中切换
tui enable              # 启用 TUI
tui disable             # 禁用 TUI
```

**窗口布局**

```bash
layout src              # 源码窗口
layout asm              # 汇编窗口
layout split            # 源码+汇编
layout regs             # 寄存器窗口

# 窗口控制
focus cmd               # 焦点命令窗口
focus src               # 焦点源码窗口
focus asm               # 焦点汇编窗口
```

**TUI 快捷键**

| 快捷键 | 功能 |
|--------|------|
| `Ctrl+x a` | 切换 TUI 模式 |
| `Ctrl+x 1` | 单窗口模式 |
| `Ctrl+x 2` | 双窗口模式 |
| `Ctrl+x s` | 切换窗口焦点 |
| `Ctrl+l` | 刷新窗口 |
| `Page Up/Down` | 滚动源码 |

## 5. 性能优化

### 5.1 调优策略

**符号加载优化**

```bash
# 延迟加载符号（加速启动）
set symbol-reloading off

# 只加载当前共享库符号
set auto-solib-add off
# 需要时手动加载
sharedlibrary mylib

# 禁用分页（脚本化调试）
set pagination off

# 禁用确认提示
set confirm off
```

**断点优化**

```bash
# 使用硬件断点（更快）
set breakpoint pending on

# 禁用不必要的断点
disable 2 3 4

# 使用条件断点替代单步
break foo if x == target_value

# 使用跟踪点（tracepoint）替代断点
trace main
actions
  collect $regs
  collect x, y
end
```

**内存读取优化**

```bash
# 限制显示元素数量
set print elements 200

# 禁用美化（加速输出）
set print pretty off

# 限制递归深度
set max-depth 10
```

### 5.2 最佳实践

**调试符号管理**

```bash
# 1. 开发环境：完整调试信息
gcc -g3 -O0 program.c -o program

# 2. 测试环境：部分优化
gcc -g -O2 program.c -o program

# 3. 生产环境：分离调试符号
gcc -g program.c -o program
objcopy --only-keep-debug program program.debug
strip --strip-debug --strip-unneeded program
objcopy --add-gnu-debuglink=program.debug program
```

**自动化调试脚本**

```bash
#!/bin/bash
# auto_debug.sh - 自动化调试脚本

PROGRAM=$1
COREFILE=$2

gdb -batch \
    -ex "file $PROGRAM" \
    -ex "core $COREFILE" \
    -ex "bt" \
    -ex "info registers" \
    -ex "info threads" \
    -ex "thread apply all bt" \
    -ex "quit"
```

**多线程调试技巧**

```bash
# 设置线程停止模式
set scheduler-locking off   # 所有线程运行（默认）
set scheduler-locking on    # 只运行当前线程
set scheduler-locking step  # 单步时锁定

# 线程操作
info threads                # 列出所有线程
thread 2                    # 切换到线程 2
thread apply all bt          # 所有线程的调用栈
thread apply all print x     # 所有线程打印变量 x

# 线程断点
break foo thread 2          # 仅在线程 2 停止
```

## 6. 问题排查

### 6.1 常见问题

**macOS 签名问题**

```bash
# 问题：Unable to find Mach task port

# 解决：创建签名证书
# 1. 打开 Keychain Access
# 2. Certificate Assistant -> Create a Certificate
# 3. Name: gdb-cert, Type: Code Signing

# 2. 签名 GDB
codesign --sign gdb-cert $(which gdb)

# 3. 允许调试
codesign --entitlements gdb.xml -f -s gdb-cert $(which gdb)

# gdb.xml 内容
cat > gdb.xml << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>com.apple.security.cs.debugger</key>
    <true/>
</dict>
</plist>
EOF
```

**核心转储文件找不到**

```bash
# 问题：无法生成或找到 core 文件

# 检查核心转储限制
ulimit -c

# 启用核心转储
ulimit -c unlimited

# 检查核心转储位置
cat /proc/sys/kernel/core_pattern

# 设置核心转储位置
echo "/tmp/core.%p" | sudo tee /proc/sys/kernel/core_pattern

# systemd 环境修改
sudo systemctl edit apport
# 设置: Storage=none
```

**符号找不到**

```bash
# 问题：Missing separate debuginfo

# 解决：安装调试符号包
# Ubuntu/Debian
sudo apt install libc6-dbg
sudo apt install libstdc++6-dbg

# CentOS/RHEL
sudo debuginfo-install glibc
sudo debuginfo-install libstdc++

# 手动加载符号
add-symbol-file mylib.so 0x7ffff7a00000
```

**优化代码调试困难**

```bash
# 问题：变量优化掉、执行顺序错乱

# 方案 1：禁用优化
gcc -O0 -g program.c -o program

# 方案 2：使用 volatile
volatile int debug_flag = 1;

# 方案 3：使用特定优化级别
gcc -Og -g program.c -o program  # 调试友好优化

# 方案 4：内联控制
gcc -fno-inline -g program.c -o program
```

### 6.2 调试技巧

**查找内存泄漏**

```bash
# 使用 watchpoint 监控内存分配
break malloc
commands
  silent
  printf "malloc(%d) = %p\n", $rdi, $rax
  continue
end

# 监控 free 调用
break free
commands
  silent
  printf "free(%p)\n", $rdi
  continue
end
```

**定位段错误**

```bash
# 方法 1：使用核心转储
ulimit -c unlimited
./program
# 崩溃后
gdb ./program core

# 方法 2：直接运行
gdb ./program
run
# 崩溃时自动停止
bt
info registers
x/10i $pc

# 方法 3：信号捕获
catch signal SIGSEGV
```

**多进程调试**

```bash
# 跟踪子进程
set follow-fork-mode child

# 同时调试父子进程
set detach-on-fork off

# 查看进程信息
info inferiors
inferior 2                # 切换到进程 2
```

**调试动态库加载**

```bash
# 停止在库加载时
set stop-on-solib-events 1

# 查看已加载库
info sharedlibrary

# 手动加载库符号
sharedlibrary mylib
```

## 7. 集成实践

### 7.1 工具链集成

**与 GCC/Clang 集成**

```bash
# GCC 编译选项
gcc -g -O0 -fno-omit-frame-pointer program.c -o program

# Clang 特定选项
clang -g -fno-limit-debug-info program.c -o program

# CMake 集成
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS="-g -O0" ..
```

**与 IDE 集成**

```bash
# VSCode launch.json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "(gdb) Launch",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/program",
      "args": ["arg1", "arg2"],
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "setupCommands": [
        {
          "description": "Enable pretty-printing",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ]
    }
  ]
}

# CLion
# 自动集成 GDB/LLDB
# Settings -> Build -> Debugger -> GDB
```

**与 Make 集成**

```makefile
# Makefile
CC = gcc
CFLAGS = -g -O0 -Wall
LDFLAGS = -g

.PHONY: debug

debug: program
	gdb --args ./program $(ARGS)

debug-core: program core
	gdb ./program core

valgrind: program
	valgrind --leak-check=full ./program
```

### 7.2 CI/CD 配置

**自动化调试脚本**

```yaml
# .gitlab-ci.yml
debug:test:
  stage: test
  script:
    - gcc -g -O0 program.c -o program
    - ./program || gdb -batch -ex "bt" -ex "quit" ./program core
  artifacts:
    paths:
      - core.*
    when: on_failure
```

**GitHub Actions 集成**

```yaml
# .github/workflows/debug.yml
name: Debug Analysis

on: [push]

jobs:
  debug:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install GDB
        run: sudo apt install gdb
        
      - name: Build
        run: gcc -g -O0 program.c -o program
        
      - name: Test with GDB
        run: |
          ulimit -c unlimited
          ./program || true
          if [ -f core* ]; then
            gdb -batch \
              -ex "bt" \
              -ex "info registers" \
              -ex "quit" \
              ./program core*
          fi
```

### 7.3 实战案例

**案例 1：调试段错误**

```bash
# 编译程序
gcc -g segfault_demo.c -o segfault_demo

# 启用核心转储
ulimit -c unlimited

# 运行程序（崩溃）
./segfault_demo
Segmentation fault (core dumped)

# 分析核心转储
gdb ./segfault_demo core.12345

(gdb) bt
#0  0x0000555555555169 in crash_function () at segfault_demo.c:10
#1  0x000055555555518a in main () at segfault_demo.c:15

(gdb) frame 0
(gdb) list
5    void crash_function() {
6        int *ptr = NULL;
7        printf("About to crash...\n");
8        *ptr = 42;  // 空指针解引用
9    }

(gdb) print ptr
$1 = (int *) 0x0
```

**案例 2：调试多线程死锁**

```bash
# 编译多线程程序
gcc -g -pthread deadlock_demo.c -o deadlock_demo

# 启动调试
gdb ./deadlock_demo
(gdb) run

# 程序挂起时 Ctrl+C 中断
(gdb) info threads
  Id   Target Id         Frame 
* 1    Thread 0x7ffff7a00740 (LWP 12345) "deadlock_demo" 
      __lll_lock_wait () at pthread_mutex_lock.c:???
  2    Thread 0x7ffff7200700 (LWP 12346) "deadlock_demo"
      __lll_lock_wait () at pthread_mutex_lock.c:???

(gdb) thread apply all bt

Thread 2 (Thread 0x7ffff7200700):
#0  __lll_lock_wait () at pthread_mutex_lock.c:???
#1  pthread_mutex_lock (mutex=0x555555558048 <mutex_b>)
#2  thread_func (arg=0x0) at deadlock_demo.c:20

Thread 1 (Thread 0x7ffff7a00740):
#0  __lll_lock_wait () at pthread_mutex_lock.c:???
#1  pthread_mutex_lock (mutex=0x555555558010 <mutex_a>)
#2  main () at deadlock_demo.c:35

# 分析：两个线程互相持有对方需要的锁
```

**案例 3：调试内存泄漏**

```bash
# 编译程序
gcc -g leak_demo.c -o leak_demo

# 启动 GDB
gdb ./leak_demo

(gdb) break malloc
Breakpoint 1 at 0x7ffff7e23420

(gdb) commands 1
Type commands for breakpoint(s) 1, one per line.
End with a line containing just "end".
>silent
>printf "malloc(%zu) = %p\n", $rdi, $rax
>continue
>end

(gdb) break free
(gdb) commands 2
>silent
>printf "free(%p)\n", $rdi
>continue
>end

(gdb) run
malloc(100) = 0x5555555592a0
malloc(200) = 0x555555559310
free(0x5555555592a0)
# 注意：第二个分配未释放
```

## 8. 参考资源

### 8.1 官方文档

**GNU GDB 官方资源**

- GDB 官方网站：https://www.gnu.org/software/gdb/
- GDB 在线手册：https://sourceware.org/gdb/current/onlinedocs/gdb/
- GDB Wiki：https://sourceware.org/gdb/wiki/

**命令速查**

```bash
# GDB 内置帮助
help                     # 帮助总览
help running             # 运行命令帮助
help breakpoints         # 断点命令帮助
help data                # 数据查看帮助

# 信息查看
info                     # 所有 info 命令
show                     # 所有 show 命令
set                      # 所有 set 命令
```

### 8.2 学习路径

**初级阶段（1-2 周）**

1. 掌握基本命令：run, break, next, step, print, bt
2. 学会查看变量和调用栈
3. 理解断点类型和条件断点
4. 练习调试简单程序（数组、指针、结构体）

**中级阶段（2-4 周）**

1. 掌握内存查看命令：x, info proc
2. 学会使用监视点：watch, rwatch, awatch
3. 理解多线程调试：info threads, thread
4. 掌握核心转储分析
5. 学习 GDB 脚本编写

**高级阶段（1-2 月）**

1. 掌握反向调试技术
2. 学习 Python 脚本扩展
3. 深入内存分析和漏洞检测
4. 掌握远程调试
5. 使用高级插件（GEF, PEDA）

**推荐资源**

| 资源 | 类型 | 说明 |
|------|------|------|
| GDB Pocket Reference | 图书 | 便携参考手册 |
| Debugging with GDB | 图书 | 官方详细教程 |
| GDB Dashboard | 插件 | 现代化界面增强 |
| GEF | 插件 | 漏洞开发辅助 |
| Reverse Engineering for Beginners | 图书 | 逆向调试入门 |

**进阶主题**

- GDB Python API 开发
- GDB 远程调试协议
- GDB 内部架构原理
- 自定义 GDB 命令开发
- 嵌入式系统调试

---

*本文档详细介绍了 GDB 调试器的安装、配置、基础使用、高级特性及实战技巧，适合作为 GDB 学习和使用的完整参考。*