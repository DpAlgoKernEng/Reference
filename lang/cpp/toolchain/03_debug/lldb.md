# LLDB - LLVM 调试器

## 1. 概述与背景

### 1.1 工具定位

LLDB 是 LLVM 项目的原生调试器，专为调试 C、C++、Objective-C 和 Swift 程序而设计。作为现代调试器，LLDB 充分利用 LLVM 的现有基础设施，包括 Clang 表达式解析器、LLVM 反汇编器等核心组件。

LLDB 已成为 macOS 和 iOS 开发的默认调试器，同时也在 Linux 和 Windows 平台上得到广泛支持。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2010 | 首次发布 | 作为 LLVM 项目的一部分启动开发 |
| 2013 | Xcode 5 | 取代 GDB 成为 Xcode 默认调试器 |
| 2015 | LLDB 3.7 | 支持 Windows 平台 |
| 2017 | LLDB 4.0 | 增强 Swift 调试支持 |
| 2019 | LLDB 9.0 | 改进表达式求值和性能 |
| 2022 | LLDB 14 | 增强 Python 脚本支持 |
| 2024 | LLDB 18 | 完善 C++20 调试支持 |

### 1.3 核心特性

- **Clang 集成**：使用 Clang 作为表达式解析器，支持完整的 C/C++ 语法
- **多线程调试**：原生支持多线程程序调试，可同时查看多个线程状态
- **Python 脚本**：内置 Python 解释器，支持脚本化和扩展
- **内存效率**：相比 GDB 内存占用更小，启动更快
- **跨平台**：支持 macOS、Linux、Windows、FreeBSD 等

### 1.4 适用场景

| 场景 | 说明 |
|------|------|
| macOS/iOS 开发 | Xcode 默认调试器，与 Apple 生态深度集成 |
| C/C++ 调试 | 完整支持 C++17/20 标准调试 |
| Swift 开发 | 原生支持 Swift 语言调试 |
| 嵌入式开发 | 配合 ARM/嵌入式工具链使用 |
| 内核调试 | 支持 macOS 内核扩展调试 |

### 1.5 与 GDB 对比分析

| 操作 | GDB | LLDB | 说明 |
|------|-----|------|------|
| 启动程序 | `gdb ./prog` | `lldb ./prog` | 命令基本一致 |
| 运行程序 | `run` | `run` / `r` | LLDB 支持简写 |
| 继续执行 | `continue` | `continue` / `c` | 相同 |
| 单步执行 | `next` / `step` | `next` / `n` / `step` / `s` | LLDB 更灵活 |
| 设置断点 | `break main` | `breakpoint set -n main` / `b main` | LLDB 有两种风格 |
| 打印变量 | `print var` | `expression var` / `p var` | LLDB 使用 expression |
| 查看调用栈 | `backtrace` | `thread backtrace` / `bt` | LLDB 强调线程概念 |
| 查看内存 | `x/10x addr` | `memory read addr` / `x/10x addr` | LLDB 兼容 GDB 风格 |
| 条件断点 | `break foo if x>5` | `b -c 'x>5'` | 语法略有不同 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS：**

```bash
# 方式一：Xcode 命令行工具（推荐）
xcode-select --install

# 方式二：完整 Xcode
# 从 App Store 安装 Xcode

# 验证安装
lldb --version
```

**Linux（Debian/Ubuntu）：**

```bash
# 安装 LLVM 工具链
sudo apt update
sudo apt install lldb

# 完整安装
sudo apt install llvm lldb clang

# 验证安装
lldb --version
```

**Linux（CentOS/RHEL）：**

```bash
# EPEL 仓库
sudo yum install epel-release
sudo yum install lldb

# 或使用 dnf
sudo dnf install lldb
```

**Windows：**

```bash
# 方式一：LLVM 安装包
# 下载 https://releases.llvm.org/

# 方式二：Chocolatey
choco install llvm

# 方式三：MSYS2
pacman -S mingw-w64-x86_64-lldb

# 验证安装
lldb --version
```

### 2.2 版本管理

```bash
# 查看版本
lldb --version

# macOS 查看 Xcode 版本
xcodebuild -version

# 切换 Xcode 版本（多版本共存）
sudo xcode-select -s /Applications/Xcode_15.0.app

# Linux 查看安装的 LLVM 版本
llvm-config --version
```

### 2.3 环境配置

创建或编辑 `~/.lldbinit` 配置文件：

```bash
# ~/.lldbinit - LLDB 配置文件

# 设置启动欢迎信息关闭
settings set stop-discount 0

# 设置命令别名
command alias bpl breakpoint list
command alias bpd breakpoint delete

# 启用源代码缓存
settings set target.inline-breakpoint-strategy always

# 设置默认架构（可选）
settings set target.default-arch x86_64

# Python 脚本路径
settings set target.inline-breakpoint-strategy always
```

加载配置文件：

```bash
# 在 LLDB 中重新加载配置
(lldb) command source ~/.lldbinit

# 指定配置文件启动
lldb -s ~/.lldbinit-custom ./myapp
```

### 2.4 验证安装

```bash
# 检查版本
$ lldb --version
lldb-1500.0.200.68
Apple Swift version 5.9 (swiftlang-5.9.0.128.108 clang-1500.0.40.1)

# 检查功能
$ lldb -b -o "version" -o "quit"
(lldb) version
lldb-1500.0.200.68

# 测试调试功能
$ cat > test.c << 'EOF'
#include <stdio.h>
int main() {
    int x = 10;
    printf("x = %d\n", x);
    return 0;
}
EOF
$ clang -g test.c -o test
$ lldb ./test
(lldb) b main
(lldb) run
(lldb) p x
(int) $0 = 10
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 编译带调试信息的程序
clang -g main.c -o myapp

# 启动调试
lldb ./myapp

# 基本调试流程
(lldb) b main              # 在 main 函数设置断点 b main.c:20         # 在指定行设置断点 run [args]          # 启动程序并传递参数 n                   # 单步执行 p var               # 打印变量 q                   # 退出调试
```

### 3.2 项目结构

典型调试项目的文件组织：

```
project/
├── src/
│   ├── main.c
│   ├── utils.c
│   └── utils.h
├── build/
│   └── myapp              # 带调试符号的可执行文件
├── .lldbinit              # 项目级 LLDB 配置
└── debug.gdb              # 可选：导出的调试设置
```

编译调试版本：

```bash
# C/C++ 项目
clang -g -O0 src/*.c -o build/myapp

# CMake 项目
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Makefile 项目
CFLAGS="-g -O0" make
```

### 3.3 基本命令详解

**程序控制：**

```bash
(lldb) run [args]          # 启动程序，可带参数 run < input.txt     # 重定向输入 run > output.txt    # 重定向输出 process launch --stop-at-entry  # 启动但停在入口 continue  /  c             # 继续执行 next     /  n             # 单步（不进入函数） step     /  s             # 单步（进入函数） finish                   # 执行到当前函数返回 process interrupt        # 中断正在运行的程序 process kill              # 终止程序
```

**断点管理：**

```bash
# 设置断点
(lldb) b main                        # 函数名断点 b file.c:42                  # 文件行号断点 b -n my_function            # 明确指定函数名 b -S MySelector             # Objective-C 选择器 b -r regular_expr            # 正则表达式断点

# 条件断点
(lldb) b main.c:50 -c 'x > 10' b main.c:50 -c 'strcmp(name, "test") == 0'

# 断点命令（命中时执行）
(lldb) b main.c:20
(lldb) breakpoint command add 1
Enter your debugger command(s). Type 'DONE' to end.
> p x
> p y
> DONE

# 管理断点
(lldb) breakpoint list               # 列出所有断点 break disable 1             # 禁用断点 break enable 1              # 启用断点 break delete 1              # 删除断点 break delete                 # 删除所有断点
```

### 3.4 常用操作

**查看变量：**

```bash
(lldb) p var                   # 打印变量 p *ptr                 # 打印指针指向的值 p arr[0]              # 打印数组元素 p arr[0]@10           # 打印数组（GDB 风格） frame variable        # 显示当前帧所有变量 frame variable --no-args  # 仅显示局部变量 frame variable -F    # 格式化显示 p/x var               # 十六进制格式 p/d var               # 十进制格式 p/t var               # 二进制格式 p/c 65                # 字符格式

# 类型信息
(lldb) p typeof(var)           # 获取变量类型 type lookup MyClass    # 查看类型定义
```

**查看调用栈：**

```bash
(lldb) thread backtrace         # 显示完整调用栈 bt                     # 简写 thread backtrace -c 5      # 只显示 5 帧 thread select 2           # 切换到第 2 帧 up                      # 向上移动一帧 down                    # 向下移动一帧 frame info              # 当前帧信息 frame select 3           # 选择第 3 帧
```

## 4. 进阶特性

### 4.1 高级断点功能

**监视点（Watchpoint）：**

```bash
# 设置监视点（变量变化时停止）
(lldb) watch set variable global_var watch set expression &my_struct->field

# 条件监视点
(lldb) watch set variable x -w write  # 仅写监视 watch set variable y -w read   # 仅读监视 watch list                   # 列出监视点 watch delete 1               # 删除监视点
```

**符号断点：**

```bash
# Objective-C 方法断点
(lldb) b -S viewDidLoad

# C++ 方法断点
(lldb) b MyClass::myMethod

# 动态库加载断点
(lldb) b -s libmylib.so -n my_func
```

### 4.2 表达式求值

```bash
# 在调试时执行任意代码 p printf("Debug: x = %d\n", x)    # 调用函数 p (int)malloc(100)                # 调用 malloc p strlen("hello")                 # 调用库函数

# 定义变量
(lldb) expr int $temp = 10 p $temp + 5

# 修改变量
(lldb) expr var = 20 p var

# 复杂表达式
(lldb) expr for(int i=0; i<10; i++) { printf("%d ", i); }
```

### 4.3 内存操作

```bash
# 读取内存
(lldb) memory read 0x7fff5fbff000           # 读取内存 x/10x 0x7fff5fbff000               # GDB 风格：10个十六进制 mem read -fx 0x1000 -c 32         # 格式化读取 memory read -c 100 &buffer      # 读取 buffer

# 写入内存
(lldb) memory write 0x1000 0x42 0x43 0x44    # 写入字节

# 查找内存
(lldb) memory find -s "pattern" 0x1000 0x2000  # 查找字符串

# 内存区域信息
(lldb) memory region 0x7fff5fbff000          # 显示内存区域信息
```

### 4.4 多线程调试

```bash
# 线程管理
(lldb) thread list                    # 列出所有线程 thread select 2               # 切换到线程 2 thread backtrace all         # 显示所有线程调用栈

# 线程步骤
(lldb) thread step-in                 # 当前线程单步进入 thread step-over             # 当前线程单步跳过 thread step-out              # 当前线程跳出

# 仅运行当前线程
(lldb) thread select 2
(lldb) process continue --run-mode this-thread
```

### 4.5 Python 脚本扩展

```python
# 在 LLDB 中使用 Python
(lldb) script
>>> import lldb
>>> print(lldb.frame)                     # 当前栈帧
>>> print(lldb.process)                   # 当前进程
>>> print(lldb.target)                    # 当前目标

# 执行 LLDB 命令
>>> lldb.debugger.HandleCommand("p var")

# 创建自定义命令
# ~/.lldbinit
command script add -f myscript.mycommand mycmd
```

自定义 Python 脚本示例：

```python
# myscript.py
import lldb

def mycommand(debugger, command, result, internal_dict):
    """自定义调试命令"""
    target = debugger.GetSelectedTarget()
    process = target.GetProcess()
    thread = process.GetSelectedThread()
    frame = thread.GetSelectedFrame()
    
    # 打印当前行号和文件
    line_entry = frame.GetLineEntry()
    print(f"File: {line_entry.GetFileSpec().GetFilename()}")
    print(f"Line: {line_entry.GetLine()}")

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('command script add -f myscript.mycommand mycmd')
    print('Custom command "mycmd" installed.')
```

## 5. 性能优化

### 5.1 调优策略

**启动优化：**

```bash
# 禁用符号加载（加快启动）
(lldb) settings set target.lazy-symbols true

# 预加载符号文件
(lldb) target symbols add /path/to/symbols

# 增加表达式缓存
(lldb) settings set target.max-expr-depth 256
```

**表达式求值优化：**

```bash
# 使用 frame variable 代替 expression（更快）
(lldb) frame variable var    # 快
(lldb) p var                 # 慢（需要编译表达式）

# 禁用动态类型解析
(lldb) expr -d no-dynamic-value -- var
```

### 5.2 最佳实践

| 场景 | 建议 |
|------|------|
| 大型项目 | 使用 `.lldbinit` 预设常用命令别名 |
| 频繁断点 | 使用断点命令自动化输出 |
| 多线程 | 使用 `thread backtrace all` 快速定位 |
| 内存问题 | 使用监视点追踪变量变化 |
| 性能分析 | 结合 Instruments 工具 |

## 6. 问题排查

### 6.1 常见问题

**问题 1：找不到源文件**

```bash
# 设置源文件搜索路径
(lldb) settings set target.source-map /old/path /new/path

# 查看当前映射
(lldb) settings show target.source-map
```

**问题 2：符号缺失**

```bash
# 检查符号文件
(lldb) target symbols list

# 添加 dSYM 文件（macOS）
(lldb) target symbols add /path/to/app.dSYM

# 验证符号
(lldb) image lookup -n main
```

**问题 3：表达式求值失败**

```bash
# 指定语言
(lldb) expr -l c++ -- var

# 禁用优化
(lldb) settings set target.expr-opt none
(lldb) p var
```

**问题 4：权限问题**

```bash
# macOS 允许调试
# 系统偏好设置 -> 安全性与隐私 -> 隐私 -> 开发者工具

# 或使用 sudo
sudo lldb ./myapp
```

### 6.2 调试技巧

**技巧 1：格式化输出**

```bash
# 显示类型
(lldb) p -T var

# 十六进制
(lldb) p -f x var

# 指针数组
(lldb) p *arr@10

# 自定义格式
(lldb) type format add -f hex MyType p myVar
```

**技巧 2：断点日志**

```bash
# 断点自动打印日志（不停止）
(lldb) b main.c:50
(lldb) breakpoint command add 1 -o "p x" -o continue
```

**技巧 3：保存和恢复断点**

```bash
# 保存断点
(lldb) breakpoint write -f breakpoints.txt

# 加载断点
(lldb) breakpoint read -f breakpoints.txt
```

## 7. 集成实践

### 7.1 VSCode 集成

安装 CodeLLDB 扩展后，配置 `.vscode/launch.json`：

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "type": "lldb",
      "request": "launch",
      "name": "Debug",
      "program": "${workspaceFolder}/build/myapp",
      "args": ["arg1", "arg2"],
      "cwd": "${workspaceFolder}",
      "environment": [
        {"name": "PATH", "value": "/usr/local/bin:${env:PATH}"}
      ],
      "preLaunchTask": "build",
      "stopOnEntry": false,
      "externalConsole": false,
      "MIMode": "lldb"
    },
    {
      "type": "lldb",
      "request": "attach",
      "name": "Attach to Process",
      "pid": "${command:pickProcess}",
      "stopOnEntry": true
    }
  ]
}
```

配置任务 `.vscode/tasks.json`：

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build",
      "type": "shell",
      "command": "cmake",
      "args": ["--build", "build"],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

### 7.2 Xcode 集成

Xcode 内置 LLDB 调试器，常用快捷键：

| 快捷键 | 功能 |
|--------|------|
| `Cmd + \` | 设置/取消断点 |
| `Cmd + Y` | 启用/禁用所有断点 |
| `F6` | 单步跳过 |
| `F7` | 单步进入 |
| `F8` | 继续执行 |
| `Ctrl + Cmd + Y` | 跳过当前帧 |

在 Xcode 控制台直接输入 LLDB 命令：

```
(lldb) po self.view
(lldb) frame variable
```

### 7.3 CLion 集成

CLion 原生支持 LLDB：

1. 打开 Settings -> Build -> Debugger
2. 选择 LLDB 作为调试器后端
3. 配置符号路径

### 7.4 CI/CD 配置

**GitHub Actions 示例：**

```yaml
name: Debug Tests

on: [push]

jobs:
  test:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Build with Debug Symbols
        run: |
          cmake -B build -DCMAKE_BUILD_TYPE=Debug
          cmake --build build
      
      - name: Run Tests with LLDB
        run: |
          lldb -b -o "b main" -o "run" -o "bt" -o "quit" \
            ./build/myapp --test-args
```

### 7.5 实战案例

**案例：调试多线程竞态条件**

```c
// race_condition.c
#include <pthread.h>
#include <stdio.h>

int counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 1000; i++) {
        counter++;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Counter: %d\n", counter);
    return 0;
}
```

调试步骤：

```bash
$ clang -g -pthread race_condition.c -o race

$ lldb ./race
(lldb) b 8                    # 在 counter++ 设断点 b 9                    # 第二个线程的断点 run                 # 启动

# 当断点命中时
(lldb) thread list            # 查看所有线程 thread backtrace all   # 所有线程调用栈 watch set variable counter  # 监视 counter 变化 continue

# 查看竞态情况
(lldb) p counter
```

**案例：调试内存泄漏**

```bash
# 启用地址检测器
$ clang -g -fsanitize=address leak.c -o leak

$ lldb ./leak
(lldb) b malloc_break
(lldb) run

# 使用堆信息
(lldb) script
>>> import lldb
>>> process = lldb.target.GetProcess()
>>> # 分析内存分配
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| LLDB 官方文档 | https://lldb.llvm.org/ |
| LLDB 命令映射 | https://lldb.llvm.org/use/map.html |
| LLDB 教程 | https://lldb.llvm.org/use/tutorial.html |
| LLDB Python API | https://lldb.llvm.org/python_api.html |
| LLVM 文档 | https://llvm.org/docs/ |

### 8.2 学习路径

| 阶段 | 内容 | 时间 |
|------|------|------|
| 入门 | 基本命令、断点、变量查看 | 1-2 天 |
| 进阶 | 条件断点、监视点、多线程 | 1 周 |
| 高级 | Python 脚本、自定义命令 | 2 周 |
| 专家 | 性能分析、内核调试 | 持续学习 |

### 8.3 社区资源

| 资源 | 说明 |
|------|------|
| LLVM Discourse | https://discourse.llvm.org/ |
| LLDB GitHub | https://github.com/llvm/llvm-project/tree/main/lldb |
| Stack Overflow | 标签 `[lldb]` |
| Apple Developer | Xcode 调试文档 |

### 8.4 常用命令速查

```bash
# 启动和运行 run [args]        # 运行 continue / c     # 继续 process kill      # 终止

# 断点 break main      # 函数断点 b file.c:20    # 行断点 b -c 'x>5'     # 条件断点 break list      # 列出断点 break delete 1  # 删除断点

# 执行控制 next / n        # 单步跳过 step / s        # 单步进入 finish          # 跳出函数

# 查看信息 p var            # 打印变量 bt               # 调用栈 frame variable   # 局部变量 thread list      # 线程列表

# 内存 mem read addr   # 读内存 mem write addr  # 写内存 watch set var   # 监视点
```