# WinDbg - Windows 调试器

## 1. 概述与背景

### 1.1 工具定位

WinDbg 是微软官方提供的 Windows 平台调试器，支持用户态和内核态调试。作为 Windows 调试工具集（Debugging Tools for Windows）的核心组件，WinDbg 是 Windows 平台最强大的调试工具，广泛应用于驱动开发、系统级调试、崩溃分析等场景。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1996 | WinDbg 初版 | 随 Windows NT 4.0 SDK 发布 |
| 2000 | WinDbg 6.0 | 支持 Windows 2000 内核调试 |
| 2006 | WinDbg 6.6 | 引入 SOS 扩展，支持 .NET 调试 |
| 2017 | WinDbg Preview | Microsoft Store 版本，现代化 UI |
| 2019 | WinDbg Preview | 引入 Time Travel Debugging (TTD) |
| 2023 | WinDbg Preview | 增强脚本支持和数据可视化 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 双模式调试 | 支持用户态和内核态调试 |
| 多目标调试 | 可调试本地进程、远程机器、虚拟机 |
| 转储分析 | 分析崩溃转储文件（minidump、full dump） |
| 符号支持 | 自动下载微软公共符号，支持私有符号 |
| 扩展生态 | 丰富的调试扩展命令（!analyze、!process 等） |
| 脚本能力 | 支持 JavaScript 和旧版脚本语法 |
| 时间旅行 | WinDbg Preview 支持 TTD 录制和回放 |

### 1.4 适用场景

| 场景 | 描述 |
|------|------|
| 应用程序崩溃分析 | 分析崩溃转储，定位异常原因 |
| 内核驱动调试 | 调试 Windows 内核和驱动程序 |
| 性能问题排查 | 分析 CPU 占用、内存泄漏 |
| 恶意代码分析 | 逆向分析和恶意软件调试 |
| 实时调试 | 附加到运行中的进程进行调试 |

### 1.5 对比分析

| 调试器 | 优势 | 劣势 |
|--------|------|------|
| WinDbg | 内核调试能力强、免费、微软官方支持 | 学习曲线陡峭、UI 相对简陋 |
| WinDbg Preview | 现代化 UI、TTD 支持、脚本能力强 | 仅支持 Windows 10+ |
| Visual Studio | 集成开发、易用性好 | 内核调试支持有限 |
| x64dbg | 开源、插件丰富、逆向友好 | 仅用户态、无官方支持 |
| GDB | 跨平台、开源 | Windows 支持较弱 |

## 2. 安装与配置

### 2.1 多平台安装

**方式一：Windows SDK 安装**

```cmd
# 下载 Windows SDK
# https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/

# 安装时选择 "Debugging Tools for Windows"
# 默认路径：C:\Program Files (x86)\Windows Kits\10\Debuggers\x64
```

**方式二：Microsoft Store 安装（WinDbg Preview）**

```powershell
# 通过 Microsoft Store 搜索 "WinDbg Preview" 安装
# 或使用 winget
winget install Microsoft.WinDbg
```

**方式三：独立安装包**

```cmd
# 下载独立安装包
# https://developer.microsoft.com/en-us/windows/hardware/download-windbg

# 安装后添加到 PATH
setx PATH "%PATH%;C:\Program Files (x86)\Windows Kits\10\Debuggers\x64"
```

### 2.2 版本管理

| 版本 | 路径 | 用途 |
|------|------|------|
| x64 | `...\Debuggers\x64` | 64 位系统调试 |
| x86 | `...\Debuggers\x86` | 32 位系统调试 |
| Preview | `%LocalAppData%\Microsoft\WindowsApps` | 现代化版本 |

### 2.3 环境配置

**符号路径配置**

```cmd
; 设置符号服务器路径
set _NT_SYMBOL_PATH=srv*C:\Symbols*https://msdl.microsoft.com/download/symbols

; 包含私有符号
set _NT_SYMBOL_PATH=srv*C:\Symbols*https://msdl.microsoft.com/download/symbols;C:\MySymbols
```

**源码路径配置**

```cmd
set _NT_SOURCE_PATH=C:\Source\Project1;C:\Source\Project2
```

### 2.4 验证安装

```cmd
; 检查版本
windbg -version

; 验证符号配置
windbg -y srv*C:\Symbols*https://msdl.microsoft.com/download/symbols -z notepad.dmp
```

## 3. 基础使用

### 3.1 快速入门

**调试可执行文件**

```cmd
; 启动并调试程序
windbg notepad.exe

; 带参数启动
windbg notepad.exe C:\test.txt

; 启动时断在入口点
windbg -G notepad.exe
```

**附加到进程**

```cmd
; 通过 PID 附加
windbg -p 1234

; 通过进程名附加
windbg -pn notepad.exe

; 非侵入式附加
windbg -pv -p 1234
```

**调试转储文件**

```cmd
; 打开崩溃转储
windbg -z C:\dumps\crash.dmp

; 分析转储
!analyze -v
```

### 3.2 项目结构

WinDbg 工作目录结构：

```
C:\DebugWorkspace\
├── dumps\           # 转储文件
├── symbols\         # 本地符号缓存
├── scripts\         # 调试脚本
├── extensions\      # 自定义扩展
└── logs\           # 调试日志
```

### 3.3 基本命令

**执行控制命令**

| 命令 | 说明 | 示例 |
|------|------|------|
| `g` | 继续执行 | `g` |
| `g [address]` | 执行到指定地址 | `g 00401000` |
| `t` | 单步进入（Step Into） | `t` |
| `p` | 单步跳过（Step Over） | `p` |
| `gu` | 执行到返回 | `gu` |
| `q` | 退出调试 | `q` |

**断点命令**

```cmd
; 设置断点
bp main                    ; 函数断点
bp notepad!WinMain        ; 模块!函数
bp 0x00401000             ; 地址断点
bp kernel32!CreateFileW   ; API 断点

; 条件断点
bp main "j @eax=0 ''; 'g'"
bp main ".if (@eax > 0x100) {} .else {g}"

; 条件断点 - 统计调用次数
bp main "r $t0=$t0+1; g"
bp main "r $t0=$t0+1; .if ($t0 == 10) {} .else {g}"

; 查看断点列表
bl

; 禁用/启用断点
bd 1                       ; 禁用 1 号断点
be 1                       ; 启用 1 号断点

; 删除断点
bc 1                       ; 删除 1 号断点
bc *                       ; 删除所有断点
```

### 3.4 常用操作

**查看内存**

```cmd
; 按格式查看内存
db 0x00400000             ; 字节（ASCII）
dw 0x00400000             ; 字（WORD）
dd 0x00400000             ; 双字（DWORD）
dq 0x00400000             ; 四字（QWORD）

; 查看字符串
da 0x00400000             ; ASCII 字符串
du 0x00400000             ; Unicode 字符串

; 查看指针数组
dps esp L10               ; 显示 10 个指针和符号

; 查看栈
dds esp                    ; 栈内容（双字 + 符号）
k                          ; 调用栈

; 内存搜索
s -a 0 L?80000000 "error"  ; 搜索 ASCII 字符串
s -d 0 L?80000000 12345678 ; 搜索 DWORD
```

**查看调用栈**

```cmd
; 显示调用栈
k                          ; 基本调用栈
kv                         ; 详细信息（帧号、参数）
kP                         ; 带参数类型

; 切换栈帧
.frame /c 2               ; 切换到第 2 帧
.frame /r                 ; 显示当前帧寄存器

; 查看其他线程
~* k                       ; 所有线程调用栈
~0 k                       ; 0 号线程调用栈
```

**寄存器操作**

```cmd
; 显示所有寄存器
r

; 显示特定寄存器
r eax
r eip

; 显示浮点寄存器
rF

; 显示 SSE 寄存器
rX

; 修改寄存器
r eax=0x12345678
r eip=main
```

## 4. 进阶特性

### 4.1 高级配置

**符号服务器配置**

```cmd
; 完整符号路径配置
.sympath srv*C:\Symbols*https://msdl.microsoft.com/download/symbols

; 添加私有符号路径
.sympath+ C:\MyProject\symbols

; 查看当前符号路径
.sympath

; 重新加载符号
.reload

; 强制重新加载
.reload /f
```

**源码配置**

```cmd
; 设置源码路径
.srcpath C:\Source\MyProject

; 添加源码路径
.srcpath+ C:\Source\Lib

; 源码相关命令
.lsrc                     ; 列出源文件
.open source.c            ; 打开源文件
```

### 4.2 扩展功能

**!analyze 扩展**

```cmd
; 自动分析崩溃
!analyze -v               ; 详细分析

; 显示异常信息
!analyze -show

; 挂起分析
!analyze -hang            ; 分析挂起问题
```

**进程和线程扩展**

```cmd
; 显示进程信息
!process 0 0              ; 列出所有进程
!process 0 7              ; 详细信息
!process                  ; 当前进程

; 显示线程信息
!thread                   ; 当前线程
!threads                  ; 所有线程

; 句柄信息
!handle                   ; 所有句柄
!handle 0x1234            ; 特定句柄详情
```

**内存扩展**

```cmd
; 堆信息
!heap -p -a 0x12345678    ; 查看堆块信息
!heap -s                  ; 堆统计

; 地址空间
!address 0x12345678       ; 地址信息
!address -summary         ; 地址空间摘要

; 池标签
!poolfind TagName         ; 查找池分配
```

### 4.3 插件生态

| 扩展 | 功能 |
|------|------|
| `sos.dll` | .NET 调试扩展 |
| `ext.dll` | 标准扩展命令 |
| `kdexts.dll` | 内核调试扩展 |
| `wow64exts.dll` | WOW64 调试扩展 |
| `MEX.dll` | 扩展命令集 |

**加载扩展**

```cmd
; 加载扩展
.load C:\extensions\mex.dll

; 查看已加载扩展
.chain

; 卸载扩展
.unload mex
```

## 5. 性能优化

### 5.1 调优策略

**符号加载优化**

```cmd
; 仅加载需要的符号
ld *                      ; 加载所有模块符号
ld notepad               ; 仅加载 notepad 符号

; 禁用自动符号加载
.symopt+ 0x100           ; 仅手动加载

; 延迟加载
.symopt+ 0x4             ; 延迟加载
```

**索引优化**

```cmd
; 为 PDB 创建索引
!symquiet               ; 减少符号加载输出
```

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| 使用符号服务器 | 自动获取正确符号 |
| 本地缓存符号 | 避免重复下载 |
| 按需加载 | 只加载需要的模块符号 |
| 使用脚本 | 自动化重复任务 |
| TTD 录制 | 复杂问题录制后分析 |

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| 符号不匹配 | PDB 版本不匹配 | 使用符号服务器 |
| 无法附加 | 权限不足 | 以管理员运行 |
| 断点不命中 | 代码被优化 | 禁用优化或使用内存断点 |
| 调试卡死 | 目标线程阻塞 | 检查线程状态 |
| 扩展加载失败 | 架构不匹配 | 使用正确版本扩展 |

### 6.2 调试技巧

**崩溃分析流程**

```cmd
; 1. 打开转储文件
windbg -z crash.dmp

; 2. 配置符号
.sympath srv*C:\Symbols*https://msdl.microsoft.com/download/symbols

; 3. 加载符号
.reload

; 4. 分析异常
!analyze -v

; 5. 查看调用栈
kP

; 6. 查看局部变量
dv

; 7. 查看内存
db @esp L100
```

**内存访问断点**

```cmd
; 读取断点（4 字节）
ba r4 0x12345678

; 写入断点（4 字节）
ba w4 0x12345678

; 执行断点
ba e1 main

; 条件访问断点
ba w4 0x12345678 "j (@eax=1) ''; 'g'"
```

**查找内存泄漏**

```cmd
; 启用堆追踪
!heap -p -a

; 查看堆块信息
!heap -p -a 0x12345678

; 统计堆使用
!heap -s
```

## 7. 集成实践

### 7.1 工具链集成

**与 Visual Studio 配合**

```cmd
; VS 中附加 WinDbg
; 调试 -> 附加到进程 -> 选择 WinDbg

; 从 VS 启动 WinDbg
; 工具 -> 外部工具 -> 添加 WinDbg
```

**与 Sysinternals 工具配合**

```cmd
; 使用 Process Explorer 生成转储
; 右键进程 -> Create Dump File

; 使用 ProcDump 自动捕获
procdump -e -w notepad.exe crash.dmp

; 使用 WinDbg 分析
windbg -z crash.dmp
```

### 7.2 CI/CD 配置

**自动崩溃转储分析脚本**

```javascript
// analyze_crash.js
function RunAnalysis() {
    var dumpPath = host.currentProcess.EnvironmentVariables("DUMP_PATH");
    host.diagnostics.debugLog("Analyzing: " + dumpPath + "\n");
    
    // 执行分析命令
    host.namespace.Debugger.Execute("!analyze -v");
    host.namespace.Debugger.Execute(".ecxr");
    host.namespace.Debugger.Execute("k");
    
    // 输出结果
    host.diagnostics.debugLog("Analysis complete.\n");
}
```

```cmd
; 在 CI 中运行
windbg -c "$$>a<analyze_crash.js" -z %DUMP_PATH%
```

### 7.3 实战案例

**案例一：分析应用程序崩溃**

```cmd
; 场景：程序因访问违规崩溃

; 1. 打开转储文件
windbg -z crash.dmp

; 2. 自动分析
!analyze -v

; 输出示例：
; FAULTING_IP:
; myapp!CApp::ProcessData+23
; 00401234 mov eax, [ebx]
;
; EXCEPTION_RECORD: ffffffffc0000005
; 这表明空指针解引用

; 3. 查看调用栈
kP

; 4. 查看局部变量
dv /t /i

; 5. 反汇编问题代码
uf myapp!CApp::ProcessData
```

**案例二：调试死锁问题**

```cmd
; 场景：程序挂起，疑似死锁

; 1. 附加到进程
windbg -pn myapp.exe

; 2. 查看所有线程
~* k

; 3. 查看等待链
!locks

; 4. 查看关键区
!cs -s

; 5. 分析结果
; 线程 A 持有锁 L1，等待锁 L2
; 线程 B 持有锁 L2，等待锁 L1
; 典型的死锁场景
```

**案例三：Time Travel Debugging**

```cmd
; 场景：难以复现的 bug

; 1. 使用 WinDbg Preview 启动录制
windbgx -ttd notepad.exe

; 2. 操作程序直到问题出现

; 3. 停止录制，生成 .run 文件

; 4. 分析录制
!tt

; 5. 时间旅行
!tt 0:100                ; 跳转到 100ms
g-                        ; 向后执行
t-                        ; 向后单步

; 6. 设置时间断点
ba e1 main "!tt 0:500"
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| WinDbg 文档 | https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/ |
| 命令参考 | https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugger-commands |
| 扩展命令 | https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugger-engine-extensions |
| TTD 文档 | https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-overview |

### 8.2 学习路径

| 阶段 | 内容 |
|------|------|
| 入门 | 基本命令、断点设置、内存查看 |
| 进阶 | 符号配置、扩展命令、脚本编写 |
| 高级 | 内核调试、驱动调试、TTD 分析 |
| 专家 | 自定义扩展开发、性能分析、安全调试 |

### 8.3 推荐书籍

| 书籍 | 作者 | 重点 |
|------|------|------|
| Windows 内核原理与实现 | 潘爱民 | 内核调试基础 |
| Windows 调试艺术 | Mario Hewardt | 高级调试技术 |
| Advanced Windows Debugging | Mario Hewardt | 英文经典教材 |