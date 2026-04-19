# rr - Mozilla 时间旅行调试器

## 1. 概述与背景

### 1.1 工具定位

rr（Record and Replay）是由 Mozilla 开发的革命性调试工具，它实现了确定性重放和时间旅行调试功能。与传统调试器不同，rr 允许开发者在调试过程中向前和向后执行程序，这对于追踪难以复现的 bug（如竞态条件、内存损坏、时序问题）具有无可比拟的优势。

### 1.2 发展历史

| 年份 | 里程碑 | 说明 |
|------|--------|------|
| 2010 | 项目启动 | Mozilla 开始研发 rr |
| 2013 | 开源发布 | 在 GitHub 上开源 |
| 2015 | GDB 集成 | 深度集成 GDB，提供熟悉的调试界面 |
| 2017 | 多进程支持 | 支持多进程程序调试 |
| 2019 | 性能优化 | 大幅降低性能开销至 1.5x |
| 2021 | 广泛应用 | Firefox、Chromium 等大型项目采用 |
| 2023 | 持续发展 | 支持更多架构和系统调用 |

### 1.3 核心特性

rr 的核心优势体现在以下几个方面：

| 特性 | 说明 | 价值 |
|------|------|------|
| 时间旅行 | 可向前/向后执行 | 轻松定位问题根源 |
| 确定性重放 | 多次运行结果完全相同 | 消除不可复现问题 |
| 低开销 | 约 1.5x 性能损失 | 可用于生产环境调试 |
| GDB 集成 | 无缝使用 GDB 命令 | 零学习成本 |
| 完整记录 | 记录所有非确定性事件 | 保留完整执行现场 |

### 1.4 适用场景

rr 特别适合以下调试场景：

1. **竞态条件调试**：多线程程序的时序问题难以复现，rr 的确定性重放让问题变得可重复
2. **内存问题追踪**：内存损坏、use-after-free 等问题，可以通过反向执行找到根源
3. **偶发性崩溃**：一次性记录，多次分析，无需反复尝试复现
4. **复杂状态机**：通过时间旅行快速定位状态转换错误
5. **性能问题分析**：通过反向执行定位性能退化点

### 1.5 对比分析

| 工具 | 优势 | 劣势 | 适用场景 |
|------|------|------|----------|
| rr | 时间旅行、确定性重放、低开销 | 仅支持 Linux | 复杂 bug 追踪 |
| GDB | 广泛平台支持、成熟稳定 | 无法反向执行 | 常规调试 |
| LLDB | macOS/iOS 支持、现代化 | 功能相对有限 | Apple 平台调试 |
| Valgrind | 内存检测全面 | 性能开销大（20-50x） | 内存问题检测 |
| AddressSanitizer | 快速内存检测 | 无法反向执行 | CI/CD 集成 |

## 2. 安装与配置

### 2.1 多平台安装

**Ubuntu/Debian：**

```bash
# Ubuntu 18.04+
sudo apt update
sudo apt install rr

# 验证安装
rr --version
```

**Fedora/RHEL：**

```bash
sudo dnf install rr
rr --version
```

**Arch Linux：**

```bash
sudo pacman -S rr
rr --version
```

**从源码编译（适用于所有发行版）：**

```bash
# 安装依赖
sudo apt install cmake g++ gdb python3

# 克隆源码
git clone https://github.com/mozilla/rr
cd rr

# 构建和安装
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
sudo make install

# 配置权限
sudo bash -c 'echo "kernel.perf_event_paranoid = 1" >> /etc/sysctl.d/99-rr.conf'
sudo sysctl -p /etc/sysctl.d/99-rr.conf
```

### 2.2 版本管理

```bash
# 查看版本
rr --version

# 查看支持的 CPU 特性
rr --version  # 会显示 CPU 支持信息

# 更新到最新版本（Ubuntu）
sudo apt update && sudo apt upgrade rr
```

### 2.3 环境配置

**系统权限配置：**

```bash
# 检查当前 perf 权限设置
cat /proc/sys/kernel/perf_event_paranoid

# 设置允许非 root 用户使用性能计数器（必需）
# 值为 1 允许所有用户，值为 2 仅允许 root
sudo sysctl kernel.perf_event_paranoid=1

# 永久设置
sudo bash -c 'echo "kernel.perf_event_paranoid = 1" > /etc/sysctl.d/99-rr.conf'
```

**虚拟机环境配置：**

```bash
# 在 VirtualBox 中，需要启用性能计数器
# 关闭虚拟机后执行
VBoxManage modifyvm "VM Name" --paravirt-provider kvm

# 在 VMware 中，需要编辑 .vmx 文件添加
vpmc.enable = "TRUE"
```

### 2.4 验证安装

```bash
# 创建测试程序
cat > test_rr.c << 'EOF'
#include <stdio.h>
int main() {
    int x = 10;
    printf("x = %d\n", x);
    return 0;
}
EOF

# 编译
gcc -g test_rr.c -o test_rr

# 测试 rr 记录
rr record ./test_rr

# 测试 rr 重放
rr replay
```

## 3. 基础使用

### 3.1 快速入门

**完整调试流程：**

```bash
# 1. 编译程序（带调试信息）
gcc -g program.c -o program

# 2. 记录程序执行
rr record ./program [args]

# 3. 重放并调试
rr replay

# 4. 在 GDB 中使用时间旅行
(gdb) break main
(gdb) continue
(gdb) reverse-continue  # 向后执行
```

### 3.2 记录文件结构

rr 会在 `~/.local/share/rr/` 下创建记录目录：

```
~/.local/share/rr/
└── program-0/          # 记录目录
    ├── data-0-0        # 执行追踪数据
    ├── data-0-0.mmap   # 内存映射信息
    ├── events-0-0      # 事件日志
    ├── rr.log          # rr 日志
    └── version         # 版本信息
```

**记录管理：**

```bash
# 列出所有记录
ls ~/.local/share/rr/

# 查看记录详情
rr pack ~/.local/share/rr/program-0

# 删除旧记录
rm -rf ~/.local/share/rr/old-program-0
```

### 3.3 基本命令详解

**记录命令：**

```bash
# 基本记录
rr record ./program

# 带参数记录
rr record ./program arg1 arg2 arg3

# 记录并重放（一次性）
rr record ./program && rr replay

# 指定输出目录
rr record -o ./my_trace ./program

# 记录已运行进程
rr record -p <pid>
```

**重放命令：**

```bash
# 重放最近的记录
rr replay

# 指定记录目录
rr replay ~/.local/share/rr/program-0

# 指定 GDB 端口
rr replay -s 50505

# 只读模式（不修改记录）
rr replay --readonly
```

### 3.4 常用操作

**时间旅行调试流程：**

```bash
# 启动重放
rr replay

# GDB 命令
(gdb) break main          # 设置断点
(gdb) continue            # 运行到断点
(gdb) next                # 单步执行
(gdb) print var           # 打印变量
(gdb) reverse-continue    # 向后执行到上一个断点
(gdb) reverse-next        # 向后单步（跳过函数）
(gdb) reverse-step        # 向后单步（进入函数）
(gdb) reverse-finish      # 向后执行到函数调用点
```

## 4. 进阶特性

### 4.1 高级配置

**性能调优选项：**

```bash
# 减少记录开销（牺牲一些功能）
rr record -n ./program  # 不记录某些系统调用

# 增加缓冲区大小
rr record -b 1GB ./program

# 启用压缩（节省磁盘空间）
rr record --compression ./program
```

**多进程调试：**

```bash
# 记录多进程程序
rr record ./multiprocess_app

# 重放时调试所有进程
rr replay --dbgport=50505

# 在 GDB 中连接特定进程
(gdb) target remote :50505
```

### 4.2 扩展功能

**检查点（Checkpoint）管理：**

```bash
# 在 GDB 中创建检查点
(gdb) checkpoint
Checkpoint 1 at 0x401123, file main.c, line 42.

# 列出所有检查点
(gdb) info checkpoints

# 跳转到检查点
(gdb) restart checkpoint-id

# 删除检查点
(gdb) delete checkpoint checkpoint-id
```

**事件搜索：**

```bash
# 查看事件日志
(gdb) info record

# 跳转到特定事件
(gdb) when
Current event: 12345
(gdb) goto-event 10000

# 搜索系统调用
(gdb) find-syscall open
```

### 4.3 插件生态

rr 支持多种集成：

| 工具 | 集成方式 | 用途 |
|------|----------|------|
| GDB | 内置 | 命令行调试 |
| VSCode | launch.json | 图形化调试 |
| CLion | 插件 | IDE 集成 |
| Emacs | gud-mode | 编辑器集成 |

## 5. 性能优化

### 5.1 调优策略

**减少记录开销：**

```bash
# 1. 只记录需要的部分
rr record -n ./program  # 最小化记录

# 2. 使用压缩减少 I/O
rr record --compression ./program

# 3. 预分配磁盘空间
fallocate -l 10G ~/.local/share/rr/preallocated
```

**存储优化：**

```bash
# 打包压缩记录
rr pack ~/.local/share/rr/program-0

# 删除不必要的记录
rr pack -d ~/.local/share/rr/program-0

# 合并多个记录
rr pack -m ~/.local/share/rr/program-*
```

### 5.2 最佳实践

1. **编译优化**：记录时使用 `-O0`，调试完成后恢复优化级别
2. **符号信息**：始终包含调试符号（`-g`），可分离到单独文件
3. **记录管理**：定期清理旧记录，使用命名规范
4. **选择性记录**：只记录重现问题所需的最小区间

```bash
# 推荐的编译选项
gcc -g -O0 -fno-omit-frame-pointer program.c -o program

# 分离调试符号
objcopy --only-keep-debug program program.debug
strip program
objcopy --add-gnu-debuglink=program.debug program
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：权限错误**

```bash
# 错误信息
[FATAL] Cannot access performance counters

# 解决方案
sudo sysctl kernel.perf_event_paranoid=1
```

**问题 2：CPU 不支持**

```bash
# 错误信息
[FATAL] Required CPU features not available

# 检查 CPU 支持
cat /proc/cpuinfo | grep -o 'vmx\|svm\|pdpe1gb'

# 解决方案：使用支持虚拟化的 CPU（Intel Haswell+ 或 AMD Zen+）
```

**问题 3：虚拟机中无法使用**

```bash
# 错误信息
[FATAL] Virtualization not supported

# 解决方案：启用嵌套虚拟化或使用裸机环境
# VirtualBox
VBoxManage modifyvm "VM Name" --paravirt-provider kvm

# VMware：编辑 .vmx 文件
vpmc.enable = "TRUE"
```

**问题 4：磁盘空间不足**

```bash
# 错误信息
[FATAL] Out of disk space

# 检查记录大小
du -sh ~/.local/share/rr/*

# 清理旧记录
rm -rf ~/.local/share/rr/old-records-*
```

### 6.2 调试技巧

**定位崩溃根源：**

```bash
# 1. 记录崩溃程序
rr record ./crash_app

# 2. 重放并定位
rr replay
(gdb) continue  # 运行到崩溃点
# 程序崩溃

# 3. 反向追踪
(gdb) reverse-continue  # 跳转到问题根源
(gdb) bt                # 查看调用栈
(gdb) info locals       # 检查变量状态
```

**调试竞态条件：**

```bash
# 1. 记录多线程程序
rr record ./multithreaded_app

# 2. 重放并分析
rr replay
(gdb) info threads
(gdb) thread 2  # 切换到问题线程
(gdb) reverse-step  # 反向单步执行
```

## 7. 集成实践

### 7.1 工具链集成

**Makefile 集成：**

```makefile
# Makefile 添加
RR_TRACE_DIR = .rr-traces

debug-rr: $(TARGET)
	@mkdir -p $(RR_TRACE_DIR)
	rr record -o $(RR_TRACE_DIR)/$(TARGET) ./$(TARGET)

replay-rr:
	rr replay $(RR_TRACE_DIR)/$(TARGET)
```

**VSCode 集成：**

```json
// .vscode/launch.json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "rr Record",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/myapp",
      "args": [],
      "preLaunchTask": "build",
      "postDebugTask": "rr-record"
    },
    {
      "name": "rr Replay",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/myapp",
      "miDebuggerServerAddress": "localhost:50505",
      "preLaunchTask": "rr-replay-server"
    }
  ]
}
```

### 7.2 CI/CD 配置

**GitLab CI 示例：**

```yaml
# .gitlab-ci.yml
rr-debug:
  image: ubuntu:22.04
  before_script:
    - apt update && apt install -y rr gdb gcc
  script:
    - gcc -g test.c -o test
    - rr record ./test || true
    - rr replay --batch
  artifacts:
    paths:
      - ~/.local/share/rr/
    expire_in: 1 week
  when: on_failure
```

### 7.3 实战案例

**案例 1：调试 Use-After-Free**

```bash
# 1. 记录崩溃
rr record ./use_after_free_demo

# 2. 重放
rr replay

# 3. 在 GDB 中分析
(gdb) continue
# 程序崩溃

# 4. 反向查找到释放点
(gdb) reverse-continue
# 跳转到 free() 调用

# 5. 检查变量
(gdb) print ptr
(gdb) print *ptr
```

**案例 2：调试时序依赖 Bug**

```bash
# 1. 记录多次运行
for i in {1..10}; do
  rr record -o trace_$i ./timing_bug_app
done

# 2. 比较失败和成功的运行
rr replay trace_failed
rr replay trace_success

# 3. 在失败运行中定位
(gdb) break condition_check
(gdb) reverse-continue
```

**案例 3：多进程服务器调试**

```bash
# 1. 记录服务器
rr record ./server

# 2. 发送请求（另一个终端）
curl http://localhost:8080/api

# 3. 停止服务器 (Ctrl+C)

# 4. 重放并分析
rr replay
(gdb) info inferiors
(gdb) inferior 2  # 切换到工作进程
(gdb) backtrace
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| GitHub 仓库 | https://github.com/mozilla/rr |
| 官方文档 | https://rr-project.org/ |
| Wiki | https://github.com/mozilla/rr/wiki |
| GDB 文档 | https://sourceware.org/gdb/documentation/ |

### 8.2 学习路径

**初级阶段：**
1. 安装 rr 并验证环境
2. 学习基本记录和重放命令
3. 掌握 GDB 时间旅行命令
4. 练习简单的 bug 追踪

**中级阶段：**
1. 学习检查点和事件搜索
2. 掌握多进程调试
3. 集成到 IDE（VSCode/CLion）
4. 性能调优实践

**高级阶段：**
1. 内核级别的调试
2. 系统调用分析
3. 自定义扩展开发
4. 大型项目应用（Firefox/Chromium）

### 8.3 社区资源

- **邮件列表**：rr-dev@googlegroups.com
- **Stack Overflow**：标签 `[rr-debugger]`
- **示例项目**：GitHub 搜索 `topic:rr-debugger`

### 8.4 限制与注意事项

| 限制 | 说明 | 解决方案 |
|------|------|----------|
| 平台限制 | 仅支持 Linux | 使用 WSL2 或 Docker |
| CPU 要求 | 需要 Intel Haswell+ 或 AMD Zen+ | 检查 CPU 支持 |
| 虚拟化 | 部分虚拟机不支持 | 启用嵌套虚拟化 |
| 系统调用 | 部分调用不支持重放 | 查看 rr 文档列表 |
| 磁盘空间 | 记录文件较大 | 使用压缩和定期清理 |

---

**总结**：rr 是 Linux 平台下强大的时间旅行调试工具，特别适合难以复现的复杂 bug。虽然有平台限制，但其确定性重放和时间旅行能力为调试带来了革命性的改变。建议将其作为 C/C++ Linux 开发的标准工具之一。