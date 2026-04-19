# LLDB - LLVM 调试器

## 1. 概述与背景

### 1.1 工具定位

LLDB (Low Level Debugger) 是 LLVM 项目的原生调试器，专为调试 C、C++、Objective-C 和 Swift 程序而设计。作为 Xcode 的默认调试器，它深度集成于 Apple 生态系统，同时在 Linux 和 Windows 平台也提供良好支持。

**核心定位：**
- 现代、高性能的程序调试工具
- LLVM 编译器基础设施的官方调试组件
- 跨平台调试解决方案

### 1.2 发展历史

| 年份 | 版本/事件 | 里程碑特性 |
|------|-----------|-----------|
| 2010 | 初始发布 | 作为 LLVM 项目的一部分启动开发 |
| 2012 | Xcode 4.3 | 成功替换 GDB 成为 Xcode 默认调试器 |
| 2015 | LLVM 3.7 | 完整支持 Linux 平台调试 |
| 2017 | LLVM 5.0 | 增强对 Swift 语言的调试支持 |
| 2019 | LLVM 9.0 | 改进 Windows 平台支持 |
| 2022 | LLVM 15 | 性能优化，大项目调试提速 30% |
| 2024 | 当前版本 | 完善 Rust 调试，支持 DAP 协议 |

### 1.3 核心特性

**架构优势：**
- 基于 LLVM 的模块化架构，各组件可独立复用
- 使用 Clang AST 实现精确的表达式解析
- 支持 JIT 编译表达式，运行时执行复杂代码
- 原生多线程架构，充分利用多核 CPU

**功能亮点：**
- 支持条件断点、监视点、符号断点
- 强大的表达式求值引擎
- Python 脚本扩展能力
- 与 IDE 深度集成（Xcode、VSCode、CLion）

### 1.4 适用场景

| 场景 | 适用性 | 说明 |
|------|--------|------|
| macOS/iOS 开发 | ★★★★★ | 官方首选调试器 |
| C/C++ 项目调试 | ★★★★☆ | 跨平台支持完善 |
| Swift 开发 | ★★★★★ | 原生支持，类型推断准确 |
| 嵌入式开发 | ★★★☆☆ | 需要配合 OpenOCD |
| Linux 服务器调试 | ★★★★☆ | 性能优于 GDB |
| Windows 开发 | ★★★☆☆ | 支持完善中 |

### 1.5 对比分析

| 特性 | LLDB | GDB | 优势方 |
|------|------|-----|--------|
| 启动速度 | 快 | 较慢 | LLDB |
| 表达求值 | 使用 Clang | 内置解析器 | LLDB |
| Swift 支持 | 原生 | 不支持 | LLDB |
| 跨平台 | macOS/Linux/Windows | 全平台 | GDB |
| 远程调试 | 支持 | 成熟 | GDB |
| 脚本扩展 | Python | Python/Guile | 平局 |
| 社区资源 | 较少 | 丰富 | GDB |

## 2. 安装与配置

### 2.1 多平台安装

**macOS 安装：**

```bash
# 方式一：安装 Xcode 命令行工具（推荐）
xcode-select --install

# 方式二：安装完整 Xcode（包含更多开发工具）
# 从 App Store 下载安装

# 验证安装
lldb --version
# 输出示例：lldb-1500.0.200.58
```

**Linux 安装：**

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install lldb

# CentOS/RHEL/Fedora
sudo dnf install lldb

# Arch Linux
sudo pacman -S lldb

# 验证安装
lldb --version
```

**Windows 安装：**

```powershell
# 方式一：使用 Scoop
scoop install llvm

# 方式二：从 LLVM 官网下载安装包
# https://releases.llvm.org/download.html

# 验证安装
lldb --version
```

### 2.2 版本管理

```bash
# 查看当前版本
lldb --version

# 查看详细配置
(lldb) platform status

# 检查支持的架构
(lldb) platform list
```

### 2.3 环境配置

**配置文件 `.lldbinit`：**

```bash
# ~/.lldbinit - LLDB 启动配置文件

# 启用命令别名
command alias bpl breakpoint list
command alias bpe breakpoint enable
command alias bpd breakpoint disable

# 设置默认启动参数
settings set target.run-args arg1 arg2 arg3

# 启用历史命令保存
settings set save-history-on-quit true

# 设置源码搜索路径
settings set target.source-search-paths /path/to/src

# 加载 Python 脚本
command script import /path/to/custom_script.py
```

**环境变量：**

```bash
# 设置 LLDB 资源目录
export LLDB_RESOURCES_DIR=/usr/share/lldb

# 启用详细日志
export LLDB_DEBUG_LOG=1
```

### 2.4 验证安装

```bash
# 运行基础测试
echo 'int main(){return 0;}' > test.c && clang -g test.c -o test

# 启动 LLDB
lldb ./test

# 在 LLDB 中验证
(lldb) target create ./test
Current executable set to './test' (x86_64).
(lldb) b main
Breakpoint 1: where = test`main at test.c:1, address = 0x0000000100000f50
(lldb) run
Process 12345 launched: './test' (x86_64)
Process 12345 stopped
(lldb) quit
```

## 3. 基础使用

### 3.1 快速入门

**基础调试流程：**

```bash
# 1. 编译带调试信息的程序
clang -g -O0 program.c -o program

# 2. 启动 LLDB
lldb ./program

# 3. 设置断点
(lldb) b main

# 4. 运行程序
(lldb) run

# 5. 单步调试
(lldb) n

# 6. 查看变量
(lldb) p variable

# 7. 继续执行
(lldb) c

# 8. 退出
(lldb) quit
```

### 3.2 项目结构

典型的调试项目结构：

```
project/
├── src/             # 源代码
│   ├── main.c
│   └── utils.c
├── include/         # 头文件
├── build/           # 编译输出
│   └── debug/       # 调试版本
│       └── program
├── .lldbinit        # LLDB 配置
└── Makefile
```

### 3.3 基本命令

**执行控制命令：**

| 命令 | 简写 | 功能 | 示例 |
|------|------|------|------|
| `run [args]` | `r` | 启动程序 | `run arg1 arg2` |
| `continue` | `c` | 继续执行 | `c` |
| `next` | `n` | 单步（不进入函数） | `n` |
| `step` | `s` | 单步（进入函数） | `s` |
| `finish` | - | 执行到函数返回 | `finish` |
| `until` | `u` | 执行到指定位置 | `until 42` |

**断点管理命令：**

```bash
# 设置断点
(lldb) b main                    # 函数断点
(lldb) b file.c:42               # 行断点
(lldb) b -n func_name            # 命名断点
(lldb) b -c 'x > 10 && y < 5'    # 条件断点

# 管理断点
(lldb) breakpoint list            # 列出所有断点
(lldb) breakpoint delete 1        # 删除 1 号断点
(lldb) breakpoint disable 1       # 禁用 1 号断点
(lldb) breakpoint enable 1         # 启用 1 号断点

# 断点命令（命中时执行）
(lldb) breakpoint command add 1
Enter your debugger command(s). Type 'DONE' to end.
> p x
> DONE
```

### 3.4 常用操作

**变量查看与修改：**

```bash
# 打印变量
(lldb) p var                 # 打印变量值 p *ptr               # 打印指针指向内容 p arr[0]@10           # 打印数组前 10 个元素

# 使用 frame variable
(lldb) frame variable         # 显示当前帧所有变量 frame variable --no-args  # 仅显示局部变量

# 修改变量
(lldb) expr var = 100         # 修改变量值

# 格式化输出
(lldb) p -f x var             # 十六进制 p -f c 65              # 字符格式 p -T var              # 显示类型
```

**调用栈导航：**

```bash
# 查看调用栈
(lldb) thread backtrace       # 显示完整调用栈 thread backtrace 5      # 显示最近 5 帧

# 栈帧导航
(lldb) frame select 2         # 选择第 2 帧 up                      # 上一帧 down                    # 下一帧

# 切换线程
(lldb) thread list            # 列出所有线程 thread select 2        # 切换到线程 2
```

## 4. 进阶特性

### 4.1 高级配置

**条件断点与命中计数：**

```bash
# 条件断点
(lldb) b file.c:42 -c 'x > 100 && y != 0'

# 命中计数断点（第 N 次命中时触发）
(lldb) break modify 1 --ignore-count 10  # 忽略前 10 次

# 组合使用
(lldb) b loop_func -c 'i == 42' --ignore-count 100
```

**监视点设置：**

```bash
# 监视变量写入
(lldb) watch set variable global_var

# 监视内存地址
(lldb) watch set expression -w write -- 0x7fff5fbff8c0

# 监视读写
(lldb) watch set expression -w read_write -- ptr

# 查看监视点
(lldb) watchpoint list
```

### 4.2 扩展功能

**表达式求值：**

```bash
# 运行时执行代码
(lldb) expr printf("Debug: %d\n", x)

# 调用函数
(lldb) p (int)strlen("hello")

# 分配内存测试
(lldb) expr char *buf = (char *)malloc(100)

# 定义临时变量
(lldb) expr int temp = 42

# 使用 extern 声明
(lldb) expr extern int external_var; p external_var
```

**内存操作：**

```bash
# 读取内存
(lldb) memory read 0x1000         # 读取指定地址 x/10x 0x1000            # GDB 风格：10 个十六进制单元 x/20i $rip              # 反汇编 20 条指令

# 写入内存
(lldb) memory write 0x1000 0x42    # 写入单字节 memory write 0x1000 -s 4 {0x12345678}  # 写入 4 字节

# 内存搜索
(lldb) memory find 0x1000 0x2000 0x42  # 在范围内搜索字节值
```

### 4.3 插件生态

**Python 脚本扩展：**

```python
# ~/.lldbinit 中加载脚本
command script import ~/lldb_scripts.py

# lldb_scripts.py 内容示例
import lldb

def print_hello(debugger, command, result, internal_dict):
    print("Hello from Python!")

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('command script add -f lldb_scripts.print_hello hello')
```

**自定义命令别名：**

```bash
# ~/.lldbinit 配置
command alias plist expression -l c --
command alias pv frame variable
command alias img image lookup

# 使用自定义命令
(lldb) plist my_array
(lldb) pv local_var
```

## 5. 性能优化

### 5.1 调优策略

**减少符号加载时间：**

```bash
# 延迟加载符号
(lldb) settings set target.inline-breakpoint-strategy never

# 禁用源码缓存自动更新
(lldb) settings set target.auto-update-symbols false

# 排除不需要的符号
(lldb) settings set target.skip-prologue false
```

**断点性能优化：**

```bash
# 使用硬件断点（数量有限但速度快）
(lldb) breakpoint set -n func --hardware

# 避免复杂条件断点，改用命中计数
(lldb) break modify 1 --ignore-count 1000

# 禁用不必要的断点而非删除
(lldb) breakpoint disable 2
```

### 5.2 最佳实践

| 场景 | 建议 | 说明 |
|------|------|------|
| 调试大项目 | 使用 `.lldbinit` 预设命令 | 避免重复输入 |
| 多线程调试 | `thread select` + `thread backtrace` | 定位线程问题 |
| 远程调试 | 使用 `platform connect` | 减少数据传输 |
| 内存泄漏 | 结合 `watchpoint` 追踪 | 监视分配/释放 |
| 性能分析 | 使用 `frame select` 检查热点 | 减少断点数量 |

## 6. 问题排查

### 6.1 常见问题

**问题 1：找不到源文件**

```bash
# 问题：显示 "file not found"
# 解决：添加源码搜索路径
(lldb) settings set target.source-search-paths /path/to/source

# 或使用 target create 时指定
(lldb) target create ./program --sysroot /custom/root
```

**问题 2：符号信息缺失**

```bash
# 问题：断点无法设置，显示 "no locations"
# 解决：确保编译时包含调试信息
clang -g -O0 program.c -o program

# 检查符号信息
(lldb) image list
(lldb) image lookup -n main
```

**问题 3：多线程死锁调试**

```bash
# 查看所有线程状态
(lldb) thread list

# 检查每个线程
(lldb) thread backtrace all

# 定位死锁位置
(lldb) frame select 2
(lldb) p mutex
```

### 6.2 调试技巧

**技巧 1：命令历史与自动补全**

```bash
# 使用上下箭头浏览历史
# 使用 Tab 键自动补全
(lldb) br<Tab>        # 自动补全为 break
(lldb) break s<Tab>   # 自动补全为 set
```

**技巧 2：批量命令执行**

```bash
# 从文件执行命令
(lldb) command source commands.txt

# 使用 Python 脚本批量操作
(lldb) script
>>> for i in range(10):
...     lldb.debugger.HandleCommand(f'b file.c:{i*10+5}')
```

**技巧 3：保存与恢复调试会话**

```bash
# 保存断点到文件
(lldb) breakpoint write -f breakpoints.txt

# 从文件加载断点
(lldb) breakpoint read -f breakpoints.txt
```

## 7. 集成实践

### 7.1 工具链集成

**与 CMake 集成：**

```cmake
# CMakeLists.txt
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# 生成调试构建
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# 调试
lldb ./build/myprogram
```

**与 Make 集成：**

```makefile
# Makefile
CC = clang
CFLAGS = -g -O0 -Wall

debug: myprogram
	lldb ./myprogram

myprogram: main.o utils.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
```

### 7.2 CI/CD 配置

**GitHub Actions 示例：**

```yaml
name: Debug Test

on: [push]

jobs:
  debug-test:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Build Debug
        run: |
          clang -g -O0 main.c -o program
          
      - name: Run LLDB Test
        run: |
          lldb -s lldb_commands.txt ./program
        continue-on-error: true
        
      - name: Upload Debug Log
        uses: actions/upload-artifact@v3
        with:
          name: debug-log
          path: lldb.log
```

**lldb_commands.txt 内容：**

```
b main
run
p result
quit
```

### 7.3 实战案例

**案例 1：调试段错误**

```bash
# 编译程序
clang -g -O0 buggy.c -o buggy

# 启动 LLDB
lldb ./buggy

# 运行直到崩溃 b main
(lldb) run

# 崩溃后查看调用栈 thread backtrace

# 检查变量 frame variable

# 定位问题代码 frame select 3
(lldb) l
```

**案例 2：调试内存泄漏**

```bash
# 设置监视点追踪内存操作
(lldb) b malloc
(lldb) run

# 在 malloc 断点 hit point list
(lldb) p (void *)$rdi  # 参数：分配大小 p (void *)$rax  # 返回值：地址

# 设置监视点
(lldb) watch set expression $rax
(lldb) c

# 观察内存何时被修改/释放
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 | 说明 |
|------|------|------|
| LLDB 官方文档 | https://lldb.llvm.org/ | 完整命令参考 |
| LLDB 教程 | https://lldb.llvm.org/use/tutorial.html | 入门指南 |
| GDB-LLDB 命令映射 | https://lldb.llvm.org/use/map.html | 命令对照表 |
| API 文档 | https://lldb.llvm.org/python_api/ | Python API |

### 8.2 学习路径

**入门阶段：**
1. 掌握基础命令：run、break、next、step、print
2. 理解调用栈和栈帧概念
3. 学会查看变量和内存

**进阶阶段：**
1. 掌握条件断点和监视点
2. 学习表达式求值和函数调用
3. 配置 `.lldbinit` 提高效率

**高级阶段：**
1. 编写 Python 脚本扩展功能
2. 远程调试和核心转储分析
3. 集成 CI/CD 流程

**推荐资源：**
- 《Debugging with LLDB》- 官方教程
- Xcode 调试文档 - Apple Developer
- LLDB 源码 - GitHub llvm-project/lldb