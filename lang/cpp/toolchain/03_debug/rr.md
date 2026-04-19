# rr - Mozilla 时间旅行调试器

## 1. 概述与背景

### 1.1 工具定位

rr（Record and Replay）是 Mozilla 开发的一款革命性调试工具，它实现了程序的记录与确定性重放。与传统调试器只能向前执行不同，rr 支持时间旅行调试——可以在调试过程中向前和向后执行，极大地简化了复杂 bug 的定位和修复。

rr 的核心价值在于：
- **确定性重放**：相同输入下，每次运行行为完全一致
- **时间旅行**：支持反向执行，直接回到问题发生前
- **低性能开销**：约 1.5x 的性能损失，适合实际开发场景
- **GDB 兼容**：完全兼容 GDB 命令，学习成本低

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2012 | 1.0 | Mozilla 内部项目启动 |
| 2014 | 2.0 | 开源发布，支持基本时间旅行 |
| 2016 | 3.0 | 支持 SIMD 指令，提升稳定性 |
| 2018 | 4.0 | 支持 AMD Zen 处理器 |
| 2020 | 5.0 | 改进多线程支持 |
| 2022 | 5.6 | 支持 ARM 架构实验性版本 |
| 2024 | 最新 | 持续优化，广泛集成 IDE |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 时间旅行 | 支持向前/向后执行，reverse-continue 等命令 |
| 确定性重放 | 多次运行结果完全相同，消除竞态条件的不确定性 |
| 低开销 | 约 1.5x 性能损失，适合日常调试 |
| GDB 集成 | 使用熟悉的 GDB 命令接口 |
| 断点快照 | 自动记录执行状态，随时回溯 |
| 多线程支持 | 正确处理多线程程序 |
| 完整系统调用 | 支持绝大多数 Linux 系统调用 |

### 1.4 适用场景

rr 特别适合以下调试场景：

1. **间歇性 Bug**：那些"有时出现，有时不出现"的问题
2. **竞态条件**：多线程程序中的时序问题
3. **内存问题**：use-after-free、内存泄漏、缓冲区溢出
4. **复杂状态机**：需要理解状态演变过程的场景
5. **性能问题分析**：结合 perf 进行性能调优

### 1.5 对比分析

| 工具 | 优势 | 劣势 | 适用场景 |
|------|------|------|----------|
| rr | 时间旅行、确定性、低开销 | 仅限 Linux | 复杂 bug 定位 |
| GDB | 广泛平台支持、简单直接 | 无反向执行 | 常规调试 |
| Valgrind | 内存检测全面 | 高开销 | 内存问题检测 |
| sanitizers | 运行时检测 | 无回溯能力 | 持续集成 |
| UndoDB | 商业支持、跨平台 | 收费 | 企业级调试 |

## 2. 安装与配置

### 2.1 多平台安装

**Ubuntu/Debian：**

```bash
# Ubuntu 18.04+
sudo apt update
sudo apt install rr

# Ubuntu 16.04 需要添加 PPA
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install rr
```

**Fedora/RHEL：**

```bash
# Fedora
sudo dnf install rr

# RHEL/CentOS (需要 EPEL)
sudo yum install epel-release
sudo yum install rr
```

**Arch Linux：**

```bash
sudo pacman -S rr
```

**从源码编译：**

```bash
# 安装依赖
sudo apt install cmake g++-multilib libcapnp-dev

# 克隆并编译
git clone https://github.com/mozilla/rr
cd rr && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
sudo make install
```

### 2.2 版本管理

```bash
# 查看版本
rr --version

# 检查 CPU 支持
rr record --help  # 会显示 CPU 特性支持状态
```

### 2.3 环境配置

**内核参数调整：**

```bash
# 检查当前设置
cat /proc/sys/kernel/perf_event_paranoid

# rr 需要值 <= 1
echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid

# 永久设置
echo "kernel.perf_event_paranoid = 1" | sudo tee -a /etc/sysctl.conf
sudo sysctl -p
```

**虚拟机环境：**

```bash
# VMware 需要禁用虚拟化性能计数器
# 在 .vmx 文件中添加：
vpmc.enable = "FALSE"

# VirtualBox 需要启用嵌套虚拟化
VBoxManage modifyvm "VM Name" --nested-hw-virt on
```

### 2.4 验证安装

```bash
# 测试基本功能
echo 'int main() { return 0; }' > test.c
gcc -g test.c -o test
rr record ./test
rr replay
# 在 GDB 中输入 quit 退出

# 验证时间旅行
rr replay
# (gdb) start
# (gdb) step
# (gdb) reverse-step  # 如果成功则安装正确
```

## 3. 基础使用

### 3.1 快速入门

完整的 rr 调试流程：

```bash
# 第一步：编译程序（需要调试符号）
gcc -g -O0 myprogram.c -o myprogram

# 第二步：记录程序执行
rr record ./myprogram arg1 arg2

# 第三步：重放并调试
rr replay

# 在 GDB 中进行时间旅行调试 break main
(gdb) continue
(gdb) reverse-continue
```

### 3.2 项目结构

rr 执行后会产生以下文件结构：

```
~/.local/share/rr/
├── myprogram-0/           # 记录目录（每次运行递增）
│   ├── data.mmap          # 内存映射数据
│   ├── events             # 事件日志
│   ├── mmap_pack          # 打包的内存页
│   ├── version            # rr 版本信息
│   ├── stats              # 统计信息
│   └── task->tid maps/    # 线程相关数据
└── latest-trace -> myprogram-0/  # 最新记录的符号链接
```

### 3.3 基本命令

**记录命令选项：**

```bash
# 基本记录
rr record ./program

# 带参数记录
rr record ./program --config config.yaml

# 指定输出目录
rr record -o ./trace_dir ./program

# 记录子进程
rr record --nested=alloc ./program

# 限制记录大小
rr record --max-stack-signal-size=1M ./program
```

**重放命令选项：**

```bash
# 重放最近的记录
rr replay

# 重放指定记录
rr replay ./trace_dir

# 连接到外部 GDB
rr replay -s 50505  # 指定端口

# 静默模式（不自动启动 GDB）
rr replay -d
```

### 3.4 常用操作

**查看记录信息：**

```bash
# 列出所有记录
ls ~/.local/share/rr/

# 查看记录统计
rr pack ~/.local/share/rr/myprogram-0

# 删除旧记录
rm -rf ~/.local/share/rr/old-record-*
```

**清理磁盘空间：**

```bash
# 打包压缩记录
rr pack ~/.local/share/rr/myprogram-0

# 查看记录大小
du -sh ~/.local/share/rr/*
```

## 4. 进阶特性

### 4.1 高级配置

**自定义 GDB 初始化：**

```bash
# 创建 .gdbinit_rr
cat > ~/.gdbinit_rr << 'EOF'
# rr 常用别名
alias rc = reverse-continue
alias rn = reverse-next
alias rs = reverse-step
alias rf = reverse-finish

# 自动加载断点
set breakpoint pending on
set pagination off
EOF

# 使用自定义配置
rr replay -x ~/.gdbinit_rr
```

**记录特定事件：**

```bash
# 只记录特定信号
rr record --signal-policy=filter-signal ./program

# 设置文件监控
rr record --file-monitor=auto ./program
```

### 4.2 扩展功能

**搜索执行历史：**

```bash
(gdb) when              # 当前事件编号
(gdb) when-ticks        # 当前 tick 计数
(gdb) when-commit       # 当前提交点

# 跳转到特定事件 goto-event 12345
```

**调用栈快照：**

```bash
# 查看调用栈 backtrace

# 反向调用栈（从当前位置向前） backtrace
(gdb) reverse-continue
(gdb) backtrace
```

**表达式求值历史：**

```bash
# 查看变量历史值 print var
(gdb) reverse-step
(gdb) print var    # 查看之前的值

# 使用 $_gdb_major_ 和 $_gdb_minor_ 等便利变量
```

### 4.3 插件生态

**GDB 插件增强：**

```bash
# 安装 GEF (GDB Enhanced Features)
bash -c "$(curl -fsSL https://gef.blah.cat/sh)"

# 安装 pwndbg
git clone https://github.com/pwndbg/pwndbg
cd pwndbg
./setup.sh
```

## 5. 性能优化

### 5.1 调优策略

**减少记录开销：**

```bash
# 优化编译选项
gcc -g -O2 program.c -o program  # 使用 -O2 而非 -O0

# 限制记录范围
rr record --num-cpu-timers=100 ./program

# 使用压缩
rr record --compression=auto ./program
```

**磁盘空间管理：**

```bash
# 定期打包
rr pack ~/.local/share/rr/*

# 设置最大记录数
export RR_TRACE_DIR=/fast-ssd/rr-traces

# 使用 tmpfs 加速
mkdir -p /dev/shm/rr-traces
rr record -o /dev/shm/rr-traces ./program
```

### 5.2 最佳实践

1. **编译优化**：调试时使用 `-O0`，性能分析时使用 `-O2`
2. **分离关注**：先用 GDB 快速定位，再用 rr 深入分析
3. **记录管理**：定期清理旧记录，打包重要记录
4. **脚本化**：将常用操作写成脚本，提高效率

## 6. 问题排查

### 6.1 常见问题

**CPU 不支持：**

```
[FATAL /build/rr/src/PerfCounters.cc:xxx]
Perf counters not available. Check kernel.perf_event_paranoid.
```

解决方法：
```bash
echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid
```

**虚拟机中运行失败：**

```
[ERROR] Virtualization not supported
```

解决方法：
```bash
# VMware: 编辑 .vmx 文件
vpmc.enable = "FALSE"

# VirtualBox
VBoxManage modifyvm "VM" --nested-hw-virt on
```

**记录文件损坏：**

```bash
# 尝试修复
rr pack ~/.local/share/rr/myprogram-0

# 如果无法修复，重新记录
rm -rf ~/.local/share/rr/myprogram-0
rr record ./program
```

### 6.2 调试技巧

**找不到符号：**

```bash
# 在 GDB 中加载符号 file /path/to/program-with-symbols
(gdb) set solib-search-path /path/to/libs/
```

**多线程调试：**

```bash
# 查看所有线程 info threads
(gdb) thread 2          # 切换线程
(gdb) thread apply all backtrace  # 所有线程调用栈
```

**内存断点：**

```bash
# 监视内存变化 watch *0x7fffffffe000
(gdb) rwatch *ptr      # 读监视
(gdb) awatch *ptr      # 读写监视
```

## 7. 集成实践

### 7.1 工具链集成

**CMake 集成：**

```cmake
# CMakeLists.txt
find_program(RR_PROGRAM rr)

if(RR_PROGRAM)
    add_custom_target(rr-record
        COMMAND ${RR_PROGRAM} record $<TARGET_FILE:myapp>
        DEPENDS myapp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Recording execution with rr"
    )

    add_custom_target(rr-replay
        COMMAND ${RR_PROGRAM} replay
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Replaying with rr"
    )
endif()
```

**Makefile 集成：**

```makefile
RR = rr
RR_FLAGS = record

rr-record: $(TARGET)
	$(RR) $(RR_FLAGS) ./$(TARGET)

rr-replay:
	$(RR) replay
```

### 7.2 CI/CD 配置

**GitHub Actions：**

```yaml
name: rr Debug

on: [push]

jobs:
  rr-test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

      - name: Install rr
        run: sudo apt install -y rr

      - name: Build with debug symbols
        run: gcc -g -O0 main.c -o program

      - name: Record execution
        run: rr record ./program || true

      - name: Archive trace
        if: failure()
        uses: actions/upload-artifact@v3
        with:
          name: rr-trace
          path: ~/.local/share/rr/
```

**GitLab CI：**

```yaml
rr-debug:
  image: ubuntu:22.04
  before_script:
    - apt update && apt install -y rr gcc
    - echo 1 > /proc/sys/kernel/perf_event_paranoid || true
  script:
    - gcc -g main.c -o program
    - rr record ./program || true
  artifacts:
    paths:
      - ~/.local/share/rr/
    when: on_failure
```

### 7.3 实战案例

**案例 1：调试竞态条件**

```bash
# 记录多线程程序
rr record ./multithreaded_app

# 重放并分析
rr replay
# (gdb) break pthread_mutex_lock
# (gdb) continue
# (gdb) info threads
# (gdb) reverse-continue  # 回到问题发生前
```

**案例 2：定位 use-after-free**

```bash
# 记录崩溃程序
rr record ./app_with_bug

# 重放
rr replay
# (gdb) continue  # 让程序崩溃
# (gdb) reverse-continue  # 回到 free 调用点
# (gdb) backtrace
```

**案例 3：分析间歇性失败**

```bash
# 多次记录
for i in {1..10}; do
    rr record -o ./trace_$i ./flaky_test
    if [ $? -ne 0 ]; then
        echo "Failure captured in trace_$i"
        break
    fi
done

# 分析失败的记录
rr replay ./trace_5
```

## 8. 参考资源

### 8.1 官方文档

- **GitHub 仓库**：https://github.com/mozilla/rr
- **官方 Wiki**：https://github.com/mozilla/rr/wiki
- **问题跟踪**：https://github.com/mozilla/rr/issues
- **邮件列表**：rr-users@mozilla.org

### 8.2 学习路径

| 阶段 | 资源 | 时间 |
|------|------|------|
| 入门 | 官方 README + 基础教程 | 1-2 小时 |
| 进阶 | 时间旅行调试实践 | 1 周 |
| 高级 | 集成 CI/CD + 团队应用 | 2 周 |
| 专家 | 内部原理 + 贡献代码 | 持续 |

**推荐教程：**

1. Mozilla 官方 rr 教程
2. "Time Travel Debugging with rr" 视频系列
3. GDB 时间旅行命令参考手册