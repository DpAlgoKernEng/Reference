# perf - Linux 性能分析工具

## 1. 概述与背景

### 1.1 工具定位

perf 是 Linux 内核自带的性能分析工具，全称 Performance Events for Linux。它利用硬件性能计数器（PMU）和内核追踪点进行采样分析，是 Linux 平台上最强大的性能分析工具之一。

perf 采用采样（Sampling）技术，通过定期中断 CPU 来记录程序执行状态，性能开销极低（通常低于 5%），适合生产环境使用。

### 1.2 发展历史

| 年份 | 版本 | 里程碑 |
|------|------|--------|
| 2009 | 2.6.31 | perf 工具首次合入 Linux 内核 |
| 2010 | 2.6.35 | 添加 perf record、perf report 命令 |
| 2012 | 3.5 | 支持 Intel PEBS（精确事件采样） |
| 2014 | 3.14 | 引入 eBPF 支持增强追踪能力 |
| 2016 | 4.6 | 改进调用栈展开，支持 JIT 符号解析 |
| 2020 | 5.8 | 增强 AMD 处理器支持 |
| 2023 | 6.x | 持续优化硬件计数器支持 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 极低开销 | 硬件采样，开销通常 < 5% |
| 多事件支持 | CPU、缓存、内存、分支预测等 |
| 调用链分析 | 支持函数调用栈追踪 |
| 实时监控 | perf top 实时显示热点 |
| 系统级分析 | 可分析整个系统或指定进程 |
| 火焰图生成 | 支持生成可视化火焰图 |
| 源码注解 | perf annotate 源码级热点分析 |

### 1.4 适用场景

| 场景 | 推荐 |
|------|------|
| 多线程程序性能分析 | perf + 火焰图 |
| 系统级性能瓶颈定位 | perf top / perf record -a |
| CPU 热点函数定位 | perf record -g |
| 硬件缓存分析 | perf stat -e cache-* |
| 实时性能监控 | perf top |
| 内存访问模式分析 | perf mem |
| 锁竞争分析 | perf lock |

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| perf | 内核原生、开销极低、硬件支持 | 需要内核符号、学习曲线陡峭 |
| gprof | 精确调用次数、易用 | 仅支持单线程、插桩开销大 |
| Valgrind | 内存检测全面 | 开销极大（20-100x） |
| Intel VTune | 功能全面、GUI 友好 | 商业软件、需 Intel CPU |
| eBPF/bpftrace | 动态追踪、灵活 | 内核版本要求高 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# Ubuntu/Debian
sudo apt install linux-tools-common linux-tools-generic linux-tools-$(uname -r)

# CentOS/RHEL/Fedora
sudo yum install perf        # CentOS/RHEL
sudo dnf install perf        # Fedora

# Arch Linux
sudo pacman -S perf

# openSUSE
sudo zypper install perf
```

### 2.2 版本管理

perf 版本与内核版本紧密绑定，建议使用与运行内核匹配的版本：

```bash
# 查看内核版本
uname -r

# 查看 perf 版本
perf --version

# 确保 linux-tools 版本匹配
# Ubuntu 可能需要：
sudo apt install linux-tools-$(uname -r)
```

### 2.3 环境配置

```bash
# 调整权限（允许非 root 用户使用 perf）
# kernel.perf_event_paranoid 值说明：
#  2: 仅允许 root 使用
#  1: 允许 CPU 采样（默认）
#  0: 允许内核数据访问
# -1: 无限制
sudo sysctl kernel.perf_event_paranoid=1

# 永久生效
echo "kernel.perf_event_paranoid=1" | sudo tee -a /etc/sysctl.d/99-perf.conf

# 符号信息（调试需要）
echo 0 | sudo tee /proc/sys/kernel/kptr_restrict
```

### 2.4 验证安装

```bash
# 检查 perf 命令
perf --version

# 列出可用事件
perf list

# 测试运行
perf stat ls
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 最简单的使用：统计程序执行
perf stat ./program

# 采样并生成报告
perf record -g ./program
perf report

# 实时查看系统热点
perf top
```

### 3.2 核心命令

| 命令 | 功能 | 典型场景 |
|------|------|----------|
| perf stat | 统计模式 | 获取程序整体性能指标 |
| perf record | 采样模式 | 记录性能数据供后续分析 |
| perf report | 报告查看 | 分析 perf record 采集的数据 |
| perf top | 实时监控 | 实时显示系统热点函数 |
| perf annotate | 源码注释 | 查看函数级别的热点分布 |
| perf list | 事件列表 | 列出所有可用性能事件 |

### 3.3 基本命令详解

#### perf stat - 统计模式

统计程序运行期间的各种硬件和软件计数器：

```bash
# 基本统计
perf stat ./program

# 详细统计（包含 L1/dTLB/LLC）
perf stat -d ./program

# 指定事件
perf stat -e cycles,instructions,cache-misses ./program

# 多次运行取平均值
perf stat -r 10 ./program

# 系统级统计
perf stat -a sleep 10
```

**输出示例：**

```
Performance counter stats for './program':

          1,234.56 msec task-clock                #    0.9 CPUs utilized
                 5      context-switches          #    0.004 K/sec
                 0      cpu-migrations            #    0.000 K/sec
               123      page-faults               #    0.099 K/sec
     1,234,567,890      cycles                    #    0.998 GHz
       456,789,012      instructions              #    0.37  insn per cycle
          12,345,678      cache-misses            #    0.009 M/sec

       1.234567890 seconds time elapsed
```

#### perf record - 采样模式

采集程序的执行样本，生成 perf.data 文件：

```bash
# 基本采样
perf record ./program

# 采集调用栈（推荐）
perf record -g ./program

# 指定采样频率（99 Hz 平衡精度和开销）
perf record -F 99 -g ./program

# 指定事件采样
perf record -e cycles,cache-misses -g ./program

# 追踪运行中的进程
perf record -p <pid> -g

# 追踪指定线程
perf record -t <tid> -g

# 全系统采样
perf record -a -g sleep 10
```

#### perf report - 报告查看

交互式分析 perf.data：

```bash
# 查看报告
perf report

# 显示调用链
perf report -g

# 按符号排序
perf report --sort=symbol

# 输出到终端
perf report --stdio

# 过滤特定符号
perf report --comms=program_name
```

**报告导航快捷键：**

| 快捷键 | 功能 |
|--------|------|
| `+` / `-` | 展开/折叠调用链 |
| `Enter` | 查看函数详情 |
| `/` | 搜索符号 |
| `a` | 注释当前函数 |
| `h` | 显示帮助 |
| `q` | 退出 |

### 3.4 常用操作

#### perf top - 实时监控

```bash
# 实时系统热点
perf top

# 显示调用链
perf top -g

# 监控指定进程
perf top -p <pid>

# 指定事件
perf top -e cache-misses

# 指定采样频率
perf top -F 1000
```

## 4. 进阶特性

### 4.1 事件系统

perf 支持多种事件类型：

```bash
# 查看所有事件
perf list

# 查看硬件事件
perf list hw

# 查看软件事件
perf list sw

# 查看缓存事件
perf list cache

# 查看追踪点
perf list tracepoint
```

**常用事件列表：**

| 事件类型 | 事件名称 | 说明 |
|----------|----------|------|
| 硬件 | `cycles` | CPU 周期 |
| 硬件 | `instructions` | 指令数 |
| 硬件 | `cache-misses` | 缓存未命中 |
| 硬件 | `cache-references` | 缓存引用 |
| 硬件 | `branch-misses` | 分支预测失败 |
| 硬件 | `stalled-cycles-frontend` | 前端停顿周期 |
| 硬件 | `stalled-cycles-backend` | 后端停顿周期 |
| 软件 | `page-faults` | 页错误 |
| 软件 | `context-switches` | 上下文切换 |
| 软件 | `cpu-migrations` | CPU 迁移 |
| 软件 | `task-clock` | 任务时钟 |

### 4.2 源码级分析

```bash
# 源码注释分析
perf annotate

# 指定符号
perf annotate main

# 显示汇编和源码
perf annotate --source

# 指定数据文件
perf annotate -i perf.data
```

**输出示例：**

```
Percent |      Source code & Disassembly of program
---------------------------------------------------
       :      void hot_function() {
 15.00 :        mov    $0x0, %eax
 20.00 :        add    $0x1, %eax
 25.00 :        cmp    $0x1000, %eax
 10.00 :        jne    -0x10
       :      }
```

### 4.3 火焰图可视化

火焰图是分析性能热点的强大可视化工具：

```bash
# 1. 采样数据
perf record -F 99 -g ./program

# 2. 导出数据
perf script > perf.out

# 3. 下载 FlameGraph 工具
git clone https://github.com/brendangregg/FlameGraph

# 4. 生成火焰图
FlameGraph/stackcollapse-perf.pl perf.out | FlameGraph/flamegraph.pl > flame.svg

# 或使用 perf report 的 folded 输出
perf report --stdio --no-children -n -g folded,0.5,callchain > data.folded
FlameGraph/flamegraph.pl data.folded > flame.svg
```

**火焰图解读：**

| 要素 | 含义 |
|------|------|
| 横轴 | 函数调用占比（宽度越大，耗时越多） |
| 纵轴 | 调用栈深度 |
| 颜色 | 随机配色（无特殊含义） |
| 宽框 | 热点函数（需重点关注） |

## 5. 性能优化

### 5.1 调优策略

**采样频率选择：**

```bash
# 低频（49 Hz）- 开销小，精度适中
perf record -F 49 -g ./program

# 推荐（99 Hz）- 平衡精度和开销
perf record -F 99 -g ./program

# 高频（999 Hz）- 高精度，开销较大
perf record -F 999 -g ./program
```

**调用栈记录：**

```bash
# 使用帧指针（需要编译选项 -fno-omit-frame-pointer）
perf record -g fp ./program

# 使用 DWARF 调试信息（默认，需要 -g 编译）
perf record -g dwarf ./program

# 使用 LBR（Intel Last Branch Record，低开销）
perf record -g call-graph,lbr ./program
```

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| 采样频率 | 推荐 `-F 99`（99 Hz）平衡精度和开销 |
| 调用栈 | 始终使用 `-g` 采集调用链 |
| 多次采样 | 多次运行取平均，避免偏差 |
| 编译选项 | 使用 `-g` 和 `-fno-omit-frame-pointer` |
| 符号表 | 确保 `/usr/lib/debug` 包含调试符号 |
| 火焰图 | 可视化快速定位热点函数 |
| 系统分析 | `-a` 全系统采样发现整体瓶颈 |

## 6. 问题排查

### 6.1 常见问题

**权限问题：**

```bash
# 问题：Permission denied
# 解决：调整 perf_event_paranoid
sudo sysctl kernel.perf_event_paranoid=1

# 问题：Cannot read kernel map
# 解决：调整 kptr_restrict
echo 0 | sudo tee /proc/sys/kernel/kptr_restrict
```

**符号缺失：**

```bash
# 问题：函数显示为 [unknown]
# 解决：安装调试符号包

# Ubuntu/Debian
sudo apt install linux-image-$(uname -r)-dbgsym

# CentOS/RHEL
sudo debuginfo-install kernel-$(uname -r)
```

**调用栈不完整：**

```bash
# 问题：调用链显示不完整
# 解决：编译时添加调试选项
gcc -g -fno-omit-frame-pointer program.c -o program
```

### 6.2 调试技巧

```bash
# 查看当前 perf 配置
perf report --debug info

# 验证符号解析
perf report --stdio | head -50

# 检查事件是否支持
perf stat -e cycles,instructions,cache-misses echo test

# 详细日志模式
perf record -vvv ./program
```

## 7. 集成实践

### 7.1 工具链集成

**与 GDB 配合：**

```bash
# 先用 perf 定位热点
perf record -g ./program
perf report

# 再用 GDB 深入调试
gdb ./program
# 在热点函数设置断点
(gdb) break hot_function
```

**与 Valgrind 配合：**

```bash
# perf 定位 CPU 热点
perf record -g ./program

# Valgrind 分析内存问题
valgrind --leak-check=full ./program

# Valgrind 分析缓存性能
valgrind --tool=cachegrind ./program
```

### 7.2 CMake 集成

```cmake
# 添加 perf 性能分析目标
add_custom_target(perf-profile
    COMMAND perf record -F 99 -g -o perf.data $<TARGET_FILE:myapp>
    COMMAND perf report -i perf.data
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS myapp
    COMMENT "Running perf profile analysis"
)

# 添加 perf stat 目标
add_custom_target(perf-stat
    COMMAND perf stat -d $<TARGET_FILE:myapp>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS myapp
    COMMENT "Running perf stat"
)

# 添加火焰图生成目标
add_custom_target(flamegraph
    COMMAND perf record -F 99 -g -o perf.data $<TARGET_FILE:myapp>
    COMMAND perf script -i perf.data > perf.out
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/FlameGraph/stackcollapse-perf.pl perf.out > perf.folded
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/FlameGraph/flamegraph.pl perf.folded > flame.svg
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS myapp
    COMMENT "Generating flame graph"
)
```

### 7.3 实战案例

**案例一：多线程程序性能分析**

```bash
# 1. 编译程序（带调试信息）
g++ -g -fno-omit-frame-pointer -pthread main.cpp -o myapp

# 2. 运行 perf 采样
perf record -F 99 -g ./myapp

# 3. 分析报告
perf report -g

# 4. 生成火焰图
perf script > perf.out
FlameGraph/stackcollapse-perf.pl perf.out | FlameGraph/flamegraph.pl > flame.svg
```

**案例二：缓存未命中分析**

```bash
# 分析缓存性能
perf stat -e cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./program

# 定位缓存热点
perf record -e cache-misses -g ./program
perf report -g
```

**案例三：系统性能瓶颈定位**

```bash
# 全系统采样 10 秒
perf record -a -g sleep 10

# 查看报告
perf report -g

# 实时监控
perf top -a
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| perf Wiki | https://perf.wiki.kernel.org/ |
| Linux 内核文档 | https://www.kernel.org/doc/Documentation/perf.txt |
| Brendan Gregg 博客 | http://www.brendangregg.com/perf.html |
| FlameGraph 项目 | https://github.com/brendangregg/FlameGraph |

### 8.2 学习路径

| 阶段 | 内容 | 推荐资源 |
|------|------|----------|
| 入门 | perf stat、perf record、perf report | perf --help |
| 进阶 | 事件选择、调用栈分析、火焰图 | Brendan Gregg 演讲 |
| 高级 | eBPF 集成、自定义事件、内核追踪 | Linux 内核源码 |

### 8.3 与 gprof 对比总结

| 特性 | perf | gprof |
|------|------|-------|
| 工作方式 | 采样 | 插桩 |
| 性能开销 | 极低（< 5%） | 中等（10-30%） |
| 多线程支持 | 支持 | 不支持 |
| 系统调用分析 | 支持 | 不支持 |
| 精确调用次数 | 不支持 | 支持 |
| 实时监控 | 支持 | 不支持 |
| 硬件事件 | 支持 | 不支持 |
| 火焰图 | 原生支持 | 需 gprof2dot |
| 内核函数分析 | 支持 | 不支持 |
| 适用场景 | 生产环境、系统分析 | 开发调试、单线程程序 |