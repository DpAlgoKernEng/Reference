# GDB - GNU 调试器

## 1. 概述与背景

### 1.1 工具定位

GDB（GNU Debugger）是 GNU 项目开发的标准调试器，是 Linux 和 Unix 系统上最广泛使用的 C/C++ 调试工具。作为 GNU 工具链的核心组件，GDB 提供了强大的程序分析和调试能力，支持源码级调试、反汇编分析、内存检查等功能。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1986 | 1.0 | 最初发布，支持基本调试功能 |
| 1990 | 4.0 | 重构架构，支持多目标平台 |
| 2000 | 5.0 | 添加 Python 脚本支持 |
| 2009 | 7.0 | 引入反向调试功能 |
| 2015 | 7.10 | 增强多线程调试支持 |
| 2020 | 9.1 | 改进 Rust 语言支持 |
| 2023 | 13.x | 增强 C++20 标准支持 |

### 1.3 核心特性

| 特性 | 描述 |
|------|------|
| 源码级调试 | 支持断点设置、单步执行、变量查看 |
| 多语言支持 | C、C++、Go、Rust、Fortran、Assembly |
| 多架构支持 | x86、ARM、MIPS、RISC-V、PowerPC |
| 远程调试 | 通过网络或串口调试嵌入式设备 |
| 核心转储分析 | 分析程序崩溃后的核心转储文件 |
| Python 扩展 | 使用 Python 脚本扩展调试功能 |
| TUI 界面 | 终端用户界面，可视化源码和汇编 |

### 1.4 适用场景

| 场景 | 说明 |
|------|------|
| 程序崩溃分析 | 通过核心转储定位崩溃原因 |
| 逻辑错误调试 | 单步跟踪查找逻辑问题 |
| 性能问题排查 | 分析程序执行流程和热点 |
| 嵌入式开发 | 远程调试目标板程序 |
| 多线程调试 | 分析线程同步和死锁问题 |
| 内存问题 | 检测内存泄漏和越界访问 |

### 1.5 对比分析

| 调试器 | 优势 | 劣势 |
|--------|------|------|
| GDB | 开源免费、跨平台、功能强大、社区活跃 | 命令行界面学习曲线陡峭 |
| LLDB | macOS/iOS 原生支持、集成 Xcode | Linux 支持不如 GDB 完善 |
| Visual Studio Debugger | 图形界面友好、功能丰富 | 仅限 Windows 平台 |
| WinDbg | Windows 内核调试能力强大 | 界面复杂、Windows 专用 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# macOS
brew install gdb

# macOS 需要证书签名才能调试
# 创建证书并签名
codesign --entitlements gdb-entitlement.xml -fs gdb-cert $(which gdb)

# Ubuntu/Debian
sudo apt update
sudo apt install gdb

# CentOS/RHEL
sudo yum install gdb

# Arch Linux
sudo pacman -S gdb

# Windows (MinGW)
# 随 MinGW-w64 安装，或单独安装
pacman -S mingw-w64-x86_64-gdb
```

### 2.2 版本管理

```bash
# 查看版本
gdb --version

# 常见版本选择
# Ubuntu LTS 通常提供稳定版本
# 开发版可从源码编译：
wget https://ftp.gnu.org/gnu/gdb/gdb-13.2.tar.xz
tar xf gdb-13.2.tar.xz
cd gdb-13.2
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
```

### 2.3 环境配置

```bash
# ~/.gdbinit 基础配置
set pagination off          # 禁用分页
set print pretty on         # 美化打印
set print array on          # 打印数组索引
set print array-indexes on
set history save on         # 保存命令历史
set history size 10000

# 编译时添加调试信息
gcc -g -O0 program.c -o program
g++ -g -O0 program.cpp -o program

# 保留符号表（stripped 前调试）
gcc -g program.c -o program.debug
strip -o program program.debug
```

### 2.4 验证安装

```bash
# 检查 GDB 版本和配置
gdb --version

# 测试基本功能
gdb --batch --eval-command="show version"

# 检查 Python 支持
gdb --batch --eval-command="python print('Python OK')"
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 编译带调试信息的程序
gcc -g -O0 hello.c -o hello

# 启动 GDB
gdb ./hello

# GDB 交互流程
(gdb) break main      # 在 main 设断点
(gdb) run             # 启动程序
(gdb) next            # 单步执行
(gdb) print var       # 查看变量
(gdb) continue        # 继续执行
(gdb) quit            # 退出
```

### 3.2 项目结构

```
project/
├── src/
│   ├── main.c
│   └── utils.c
├── include/
│   └── utils.h
├── build/
│   └── program       # 带调试信息的可执行文件
└── .gdbinit          # GDB 配置文件
```

### 3.3 基本命令

#### 执行控制

| 命令 | 缩写 | 说明 |
|------|------|------|
| `run [args]` | `r` | 启动程序 |
| `start` | | 启动并停在 main |
| `continue` | `c` | 继续执行 |
| `next` | `n` | 单步（不进入函数） |
| `step` | `s` | 单步（进入函数） |
| `finish` | `fin` | 执行到函数返回 |
| `until` | `u` | 执行到某行 |
| `quit` | `q` | 退出 GDB |

#### 断点管理

| 命令 | 说明 |
|------|------|
| `break main` | 在 main 设断点 |
| `break file.c:42` | 在指定行设断点 |
| `break func if x > 10` | 条件断点 |
| `info breakpoints` | 查看断点 |
| `delete 1` | 删除断点 1 |
| `disable 1` | 禁用断点 1 |
| `enable 1` | 启用断点 1 |
| `tbreak func` | 临时断点（触发一次后删除） |

#### 查看变量

| 命令 | 说明 |
|------|------|
| `print var` | 打印变量值 |
| `print *ptr` | 打印指针指向值 |
| `print arr[0]@10` | 打印数组前 10 个元素 |
| `display var` | 自动显示变量 |
| `info locals` | 显示局部变量 |
| `info args` | 显示函数参数 |
| `ptype var` | 显示变量类型 |
| `set var = 10` | 修改变量值 |

#### 查看调用栈

| 命令 | 缩写 | 说明 |
|------|------|------|
| `backtrace` | `bt` | 显示调用栈 |
| `backtrace full` | | 显示调用栈和局部变量 |
| `frame 2` | | 切换到栈帧 2 |
| `up` / `down` | | 上下移动栈帧 |
| `info frame` | | 显示当前栈帧详情 |

### 3.4 常用操作

```bash
# 内存查看
x/10x &var    # 10 个十六进制单元
x/10i &func   # 10 条汇编指令
x/10s &str    # 10 个字符串
x/10c &arr    # 10 个字符

# 查看内存映射
info proc mappings

# 监视点
watch var      # 监视变量写入
rwatch var     # 监视变量读取
awatch var     # 监视变量读写
info watchpoints

# 查看寄存器
info registers
info all-registers
print $rax     # 打印特定寄存器
```

## 4. 进阶特性

### 4.1 高级配置

```bash
# ~/.gdbinit 高级配置

# 设置源码路径
set substitute-path /build/src /home/user/src

# 设置反汇编风格
set disassembly-flavor intel  # 或 att

# 启用 TUI 模式快捷键
set tui active

# 设置停止点自动显示
set stop-on-solib-events 1

# 自动加载安全路径
set auto-load safe-path /

# 定义便捷命令
define pl
  printf "%s\n", $arg0
end

define plist
  set $node = $arg0
  while $node
    print *$node
    set $node = $node->next
  end
end
```

### 4.2 扩展功能

#### 反向调试

```bash
# 启用程序录制
record

# 执行程序
continue

# 反向操作
reverse-continue    # 反向继续
reverse-next        # 反向单步（不进入函数）
reverse-step        # 反向单步（进入函数）
reverse-finish      # 反向执行到调用点

# 停止录制
record stop
```

#### 核心转储分析

```bash
# 启用核心转储
ulimit -c unlimited

# 分析核心转储文件
gdb ./program core

# 常用分析命令
(gdb) bt              # 查看崩溃时调用栈
(gdb) bt full         # 调用栈及局部变量
(gdb) info registers  # 查看寄存器状态
(gdb) frame 0         # 切换到崩溃帧
(gdb) info locals     # 查看局部变量
```

#### 多线程调试

```bash
# 查看线程
info threads
thread 2              # 切换到线程 2
thread apply all bt   # 所有线程调用栈

# 设置线程断点
break file.c:42 thread 3

# 锁定调度器
set scheduler-locking on   # 只运行当前线程
set scheduler-locking off  # 所有线程可运行
```

### 4.3 插件生态

```bash
# GDB Dashboard (Python 美化)
# 安装
wget -P ~ git.io/gdb-dashboard
# 配置 ~/.gdbinit
source ~/gdb-dashboard/.gdbinit

# GEF (GDB Enhanced Features)
# 适用于逆向分析和漏洞利用
bash -c "$(curl -fsSL https://gef.blah.cat/sh)"

# PEDA (Python Exploit Development Assistance)
# 适用于漏洞开发
git clone https://github.com/longld/peda.git ~/peda
echo "source ~/peda/peda.py" >> ~/.gdbinit

# Voltron (多窗口调试界面)
pip install voltron
# 在 ~/.gdbinit 中添加
source /usr/local/lib/python3.x/site-packages/voltron/entry.py
```

## 5. 性能优化

### 5.1 调优策略

```bash
# 减少符号加载时间
set auto-solib-add off    # 不自动加载共享库符号
sharedlibrary mylib       # 手动加载需要的库

# 优化大程序调试
set index-cache on        # 启用索引缓存
set index-cache dirname ~/.gdb-index-cache

# 设置停止点
set stop-on-solib-events 0  # 禁用库加载停止

# 使用分离调试信息
objcopy --only-keep-debug program program.debug
strip program
objcopy --add-gnu-debuglink=program.debug program
```

### 5.2 最佳实践

| 场景 | 建议 |
|------|------|
| 大型项目 | 使用分离调试信息，减少内存占用 |
| 频繁调试 | 配置 .gdbinit 自动化常用操作 |
| 远程调试 | 使用 gdbserver 减少目标机负载 |
| 多线程程序 | 使用 scheduler-locking 控制线程执行 |
| 嵌入式开发 | 合理设置远程超时和缓冲区大小 |

## 6. 问题排查

### 6.1 常见问题

#### macOS 证书签名问题

```bash
# 错误信息
# Unable to find Mach task port for process-id

# 解决方案
# 1. 创建代码签名证书
# 2. 对 GDB 进行签名
codesign --entitlements gdb-entitlement.xml -fs gdb-cert $(which gdb)

# 3. 重启系统或任务网关
sudo killall taskgated
```

#### 调试信息缺失

```bash
# 错误信息
# No debugging symbols found

# 解决方案
# 编译时添加 -g 选项
gcc -g -O0 program.c -o program

# 或使用 -ggdb 获取 GDB 特定信息
gcc -ggdb -O0 program.c -o program
```

#### 源码路径问题

```bash
# 错误信息
# No such file or directory

# 解决方案
# 设置源码路径替换
set substitute-path /old/path /new/path

# 或添加源码搜索路径
directory /path/to/source
```

### 6.2 调试技巧

#### 打印 STL 容器

```bash
# 启用美化打印
set print pretty on
print vec              # 打印 vector
print map              # 打印 map

# 自定义打印函数（Python）
python
import gdb.printing
# 加载 STL 打印器
end
```

#### 条件断点优化

```bash
# 慢速条件断点
break func if i == 1000000

# 优化：使用计数器
break func
commands
  silent
  if i == 1000000
    print i
    # 可以在这里添加其他操作
  end
  continue
end
```

## 7. 集成实践

### 7.1 工具链集成

```bash
# 与 CMake 集成
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
gdb ./myprogram

# 与 Make 集成
# Makefile
CFLAGS += -g -O0
CXXFLAGS += -g -O0

# 与 IDE 集成
# VSCode (launch.json)
{
  "version": "0.2.0",
  "configurations": [{
    "name": "C/C++ Debug",
    "type": "cppdbg",
    "request": "launch",
    "program": "${workspaceFolder}/build/program",
    "miDebuggerPath": "/usr/bin/gdb",
    "MIMode": "gdb"
  }]
}
```

### 7.2 CI/CD 配置

```yaml
# GitLab CI 自动化调试
debug-crash:
  stage: debug
  script:
    - ulimit -c unlimited
    - ./build/program || true
    - if [ -f core ]; then
        gdb -batch -ex "bt" -ex "info registers" ./build/program core > crash_report.txt;
      fi
  artifacts:
    paths:
      - crash_report.txt
    when: on_failure
```

### 7.3 实战案例

#### 案例一：段错误调试

```bash
# 1. 获取核心转储
ulimit -c unlimited
./program  # 程序崩溃

# 2. 分析核心转储
gdb ./program core

# 3. 查看崩溃位置
(gdb) bt
#0  0x0000000000400511 in process_data (ptr=0x0) at main.c:42
#1  0x0000000000400589 in main () at main.c:67

# 4. 分析变量
(gdb) frame 0
(gdb) info locals
(gdb) print *ptr  # 可能发现 ptr 为 NULL

# 5. 定位原因
# 检查指针初始化和赋值位置
```

#### 案例二：死锁分析

```bash
# 1. 附加到运行进程
gdb -p <pid>

# 2. 查看所有线程
(gdb) info threads
  Id   Target Id         Frame
  1    Thread 1234       __pthread_cond_wait
  2    Thread 1235       __pthread_cond_wait
  3    Thread 1236       __pthread_cond_wait

# 3. 查看各线程调用栈
(gdb) thread apply all bt

# 4. 分析锁状态
(gdb) print mutex_a
(gdb) print mutex_b

# 5. 定位死锁原因
# 多个线程以不同顺序获取锁
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| GDB 官方手册 | https://sourceware.org/gdb/documentation/ |
| GDB Wiki | https://sourceware.org/gdb/wiki/ |
| 在线手册 | `info gdb` 或 `gdb --help` |

### 8.2 学习路径

| 阶段 | 内容 | 建议 |
|------|------|------|
| 入门 | 基本命令、断点、变量查看 | 实践简单程序调试 |
| 进阶 | 条件断点、监视点、调用栈 | 调试中等规模项目 |
| 高级 | 多线程、核心转储、远程调试 | 参与复杂项目调试 |
| 专家 | Python 脚本、插件开发 | 定制调试工具 |

### 8.3 常用速查表

```bash
# 启动调试
gdb ./program                    # 调试程序
gdb --args ./prog arg1 arg2      # 带参数启动
gdb -p <pid>                     # 附加进程
gdb ./program core               # 分析核心转储

# 断点操作
b main                           # 函数断点
b file.c:42                      # 行断点
b func if x > 10                 # 条件断点
info b                           # 查看断点
delete 1                         # 删除断点

# 执行控制
r                                # 运行
c                                # 继续
n                                # 下一步
s                                # 步进
fin                              # 执行到返回

# 数据查看
p var                            # 打印变量
p *ptr@10                        # 打印数组
info locals                      # 局部变量
bt                               # 调用栈

# 线程调试
info threads                     # 查看线程
thread 2                         # 切换线程
thread apply all bt              # 所有线程调用栈
```