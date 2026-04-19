# perf - Linux 性能分析工具

## 1. 概述与背景

### 1.1 工具定位

perf 是 Linux 内核自带的性能分析工具，通过硬件性能计数器（PMU）和内核追踪点进行采样分析。作为 Linux 性能分析的核心工具，perf 提供了极低开销的性能剖析能力，是生产环境性能诊断的首选工具。

### 1.2 发展历史

| 年份 | 内核版本 | 特性 |
|------|---------|------|
| 2009 | 2.6.31 | perf_events 子系统首次引入 |
| 2010 | 2.6.35 | 支持 tracepoint 事件 |
| 2012 | 3.5 | 引入 perf probe 动态追踪 |
| 2014 | 3.14 | 支持 Intel PT 硬件追踪 |
| 2016 | 4.6 | 改进 BPF 集成 |
| 2018 | 4.18 | 增强 eBPF 支持 |
| 2020 | 5.8 | 支持 AMD IBS 采样 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 硬件性能计数器 | 利用 CPU PMU 采集硬件事件 |
| 内核追踪点 | 监控内核函数和系统调用 |
| 极低开销 | 采样模式下开销 < 5% |
| 多线程支持 | 原生支持多线程分析 |
| 实时监控 | perf top 实时查看热点 |
| 火焰图支持 | 可视化调用栈数据 |
| 源码级分析 | perf annotate 源码注释 |

### 1.4 适用场景

| 场景 | 说明 |
|------|------|
| CPU 热点分析 | 定位 CPU 密集型函数 |
| 缓存性能分析 | 分析 cache miss、TLB miss |
| 多线程性能 | 分析线程调度、锁竞争 |
| 系统调用分析 | 追踪系统调用频率和耗时 |
| 内核性能分析 | 分析内核函数性能 |
| 实时性能监控 | 生产环境实时监控 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| perf | 低开销、多线程、系统级分析 | 需要内核支持、学习曲线陡峭 |
| gprof | 精确调用次数、简单易用 | 高开销、不支持多线程 |
| Valgrind/Callgrind | 功能丰富、检测内存问题 | 性能开销极大（20-100x） |
| Intel VTune | 图形界面、硬件分析全面 | 商业软件、依赖 Intel CPU |

## 2. 安装与配置

### 2.1 多平台安装

**Ubuntu/Debian:**

```bash
# 安装 perf 工具
sudo apt install linux-tools-common linux-tools-generic linux-tools-$(uname -r)

# 如果版本不匹配，安装通用版本
sudo apt install linux-tools-common linux-tools-generic
```

**CentOS/RHEL:**

```bash
# 安装 perf
sudo yum install perf

# 或使用 dnf
sudo dnf install perf
```

**Fedora:**

```bash
sudo dnf install perf
```

### 2.2 版本管理

```bash
# 查看 perf 版本
perf --version

# 查看内核版本
uname -r

# 检查 perf 与内核版本匹配
perf version
```

### 2.3 环境配置

```bash
# 调整权限允许非 root 用户使用 perf
sudo sysctl -w kernel.perf_event_paranoid=1

# 永久设置
echo "kernel.perf_event_paranoid=1" | sudo tee -a /etc/sysctl.conf

# 权限级别说明：
# -1: 无限制（所有用户可访问）
#  0: 允许访问 CPU 相关事件
#  1: 允许内核级别的追踪（默认）
#  2: 仅允许用户空间追踪

# 调整符号可见性
echo 0 | sudo tee /proc/sys/kernel/kptr_restrict

# 永久设置
echo "kernel.kptr_restrict=0" | sudo tee -a /etc/sysctl.conf
```

### 2.4 验证安装

```bash
# 测试 perf 是否正常工作
perf stat ls

# 列出所有支持的事件
perf list

# 测试硬件事件
perf stat -e cycles,instructions ls
```

## 3. 基础使用

### 3.1 快速入门

perf 提供四种主要模式：

| 模式 | 命令 | 用途 |
|------|------|------|
| 统计模式 | `perf stat` | 统计程序整体性能指标 |
| 采样模式 | `perf record` | 采样分析并保存数据 |
| 报告模式 | `perf report` | 分析采样数据 |
| 实时模式 | `perf top` | 实时显示热点函数 |

### 3.2 perf stat - 统计模式

**基本用法:**

```bash
# 统计程序整体性能
perf stat ./program

# 详细统计（包括 L1、LLC 缓存）
perf stat -d ./program

# 指定事件统计
perf stat -e cycles,instructions,cache-misses,branch-misses ./program

# 多次运行取平均
perf stat -r 10 ./program

# 追踪运行中的进程
perf stat -p <pid>

# 系统级统计
perf stat -a sleep 10
```

**输出示例解读:**

```
Performance counter stats for './program':

      1,234.56 msec task-clock                #    0.9 CPUs utilized
             5      context-switches          #    0.004 K/sec
             0      cpu-migrations            #    0.000 K/sec
           123      page-faults               #    0.099 K/sec
 1,234,567,890      cycles                    #    0.998 GHz
   456,789,012      instructions              #    0.37 insn per cycle
      12,345,678      cache-misses            #    0.009 M/sec

   1.234567890 seconds time elapsed
```

| 指标 | 说明 |
|------|------|
| task-clock | 任务占用 CPU 的时间 |
| context-switches | 上下文切换次数 |
| cpu-migrations | CPU 迁移次数 |
| page-faults | 页错误次数 |
| cycles | CPU 周期数 |
| instructions | 执行指令数 |
| IPC (insn per cycle) | 每周期指令数，越高越好 |

### 3.3 perf record - 采样模式

**基本采样:**

```bash
# 基本采样
perf record ./program

# 采集调用栈（推荐）
perf record -g ./program

# 指定采样频率（99 Hz，避免与系统周期同步）
perf record -F 99 -g ./program

# 指定事件
perf record -e cycles,cache-misses -g ./program

# 追踪特定进程
perf record -p <pid> -g

# 追踪线程
perf record -t <tid> -g

# 全系统采样
perf record -a -g sleep 10

# 指定 CPU
perf record -C 0,1 -g ./program

# 保存到指定文件
perf record -o custom.data -g ./program
```

**采样参数说明:**

| 参数 | 说明 | 推荐值 |
|------|------|--------|
| `-F` | 采样频率 | 99 或 999 |
| `-g` | 采集调用栈 | 推荐开启 |
| `-e` | 指定事件 | cycles |
| `-p` | 进程 PID | - |
| `-a` | 全系统 | - |
| `-C` | 指定 CPU | - |

### 3.4 perf report - 报告查看

```bash
# 查看报告
perf report

# 显示调用链
perf report -g

# 按 symbol 排序
perf report --sort=symbol

# 显示子函数开销
perf report --children

# 输出到文件
perf report --stdio > report.txt

# 使用 TUI 界面
perf report -t
```

**报告导航快捷键:**

| 快捷键 | 功能 |
|--------|------|
| `+` / `-` | 展开/折叠调用链 |
| `Enter` | 查看函数详情 |
| `/` | 搜索符号 |
| `a` | 查看汇编注释 |
| `q` | 退出 |

### 3.5 perf top - 实时监控

```bash
# 实时热点
perf top

# 实时调用链
perf top -g

# 指定进程
perf top -p <pid>

# 指定事件
perf top -e cache-misses

# 指定 CPU
perf top -C 0,1

# 更新频率
perf top -D 1000  # 1 秒更新
```

## 4. 进阶特性

### 4.1 高级事件配置

**常用硬件事件:**

| 事件类型 | 说明 |
|----------|------|
| `cycles` | CPU 周期 |
| `instructions` | 指令数 |
| `cache-misses` | 缓存未命中 |
| `cache-references` | 缓存引用 |
| `branch-misses` | 分支预测失败 |
| `L1-dcache-loads` | L1 数据缓存加载 |
| `LLC-loads` | 最后一级缓存加载 |
| `dTLB-load-misses` | 数据 TLB 未命中 |

```bash
# 查看所有事件
perf list

# 查看硬件事件
perf list hw

# 查看软件事件
perf list sw

# 查看 tracepoint
perf list tracepoint

# 查看缓存事件
perf list cache
```

### 4.2 perf annotate - 源码注释

```bash
# 源码级分析
perf annotate

# 指定符号
perf annotate main

# 显示汇编和源码
perf annotate --source

# 指定数据文件
perf annotate -i perf.data
```

**输出示例:**

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

### 4.3 perf probe - 动态追踪

```bash
# 添加探测点
perf probe -x /path/to/binary function_name

# 添加内核探测点
sudo perf probe -a 'do_sys_open filename:string'

# 列出探测点
perf probe -l

# 删除探测点
perf probe -d probe_name

# 追踪探测点
perf record -e probe_name -a sleep 10
```

## 5. 性能优化

### 5.1 火焰图可视化

**生成火焰图:**

```bash
# 1. 采样
perf record -F 99 -g ./program

# 2. 转换数据
perf script > perf.out

# 3. 下载 FlameGraph 工具
git clone https://github.com/brendangregg/FlameGraph

# 4. 生成火焰图
FlameGraph/stackcollapse-perf.pl perf.out | FlameGraph/flamegraph.pl > flame.svg

# 或使用 perf report 的折叠输出
perf report --stdio --no-children -n -g folded,0.5,callchain > data.folded
FlameGraph/flamegraph.pl data.folded > flame.svg
```

**火焰图解读:**

| 元素 | 说明 |
|------|------|
| 横轴 | 函数调用占比 |
| 纵轴 | 调用栈深度 |
| 颜色 | 随机配色（无特殊含义） |
| 宽框 | 热点函数（耗时多） |

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| 采样频率 | `-F 99` 平衡精度和开销 |
| 调用栈采集 | 建议使用 `-g` |
| 多次采样 | 平均多次结果避免偏差 |
| 符号信息 | 编译时保留 `-g` 符号 |
| 火焰图 | 可视化快速定位热点 |
| 系统分析 | `-a` 全系统采样发现整体瓶颈 |

## 6. 问题排查

### 6.1 常见问题

**问题 1: 权限不足**

```bash
# 错误信息
# Error: Permission to access performance events

# 解决方案
sudo sysctl -w kernel.perf_event_paranoid=1
```

**问题 2: 符号不可见**

```bash
# 错误信息
# Failed to open /proc/kallsyms

# 解决方案
echo 0 | sudo tee /proc/sys/kernel/kptr_restrict
```

**问题 3: 内核版本不匹配**

```bash
# 错误信息
# perf: command not found

# 解决方案：安装匹配版本
sudo apt install linux-tools-$(uname -r)
```

**问题 4: 调用栈不完整**

```bash
# 原因：编译时优化或帧指针优化

# 解决方案：编译时保留帧指针
gcc -fno-omit-frame-pointer -g program.c

# 或使用 DWARF 调用链
perf record --call-graph=dwarf -g ./program
```

### 6.2 调试技巧

```bash
# 调试模式
perf record -vv ./program

# 查看采样信息
perf report -D

# 查看事件属性
perf evlist -v
```

## 7. 集成实践

### 7.1 CMake 集成

```cmake
# 添加 perf 性能分析目标
add_custom_target(perf-profile
    COMMAND perf record -F 99 -g -o perf.data $<TARGET_FILE:myapp>
    COMMAND perf report -i perf.data
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running perf profiling..."
)

# 添加 perf 统计目标
add_custom_target(perf-stat
    COMMAND perf stat -d $<TARGET_FILE:myapp>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running perf stat..."
)

# 添加火焰图生成目标
find_program(FLAMEGRAPH_PL flamegraph.pl)
if(FLAMEGRAPH_PL)
    add_custom_target(flamegraph
        COMMAND perf script -i perf.data | ${FLAMEGRAPH_PL} > flamegraph.svg
        DEPENDS perf-profile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating flamegraph..."
    )
endif()
```

### 7.2 实战案例

**案例 1: 分析多线程程序 CPU 热点**

```bash
# 1. 采样
perf record -F 99 -g -p <pid> --call-graph dwarf

# 2. 查看报告
perf report --sort comm,dso,symbol

# 3. 按线程分析
perf report --sort comm,tid,symbol

# 4. 生成火焰图
perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg
```

**案例 2: 分析缓存性能问题**

```bash
# 1. 统计缓存事件
perf stat -e L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./program

# 2. 采样缓存未命中
perf record -e cache-misses -g ./program

# 3. 分析热点
perf report
```

**案例 3: 系统性能瓶颈分析**

```bash
# 1. 全系统采样
perf record -a -g -F 99 sleep 30

# 2. 查看报告
perf report --sort comm,dso,symbol

# 3. 生成火焰图
perf script | stackcollapse-perf.pl | flamegraph.pl > system.svg
```

### 7.3 工作流程推荐

**开发阶段:**

```bash
# 快速验证
perf stat ./program

# 定位热点
perf record -g ./program && perf report
```

**测试阶段:**

```bash
# 性能回归测试
perf stat -r 10 -o baseline.txt ./program_v1
perf stat -r 10 -o current.txt ./program_v2

# 生成火焰图
perf record -F 99 -g ./program
perf script | flamegraph.pl > flame.svg
```

**生产环境:**

```bash
# 低开销采样
perf record -F 9 -g -p <pid> sleep 60

# 实时监控
perf top -p <pid>
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| Linux Perf Wiki | https://perf.wiki.kernel.org/ |
| perf man 手册 | `man perf` |
| perf 教程 | https://perf.wiki.kernel.org/index.php/Tutorial |
| 内核文档 | https://www.kernel.org/doc/Documentation/perf.txt |

### 8.2 学习路径

| 阶段 | 内容 | 工具 |
|------|------|------|
| 初级 | 基本统计、采样 | `perf stat`, `perf record` |
| 中级 | 报告分析、火焰图 | `perf report`, FlameGraph |
| 高级 | 动态追踪、内核分析 | `perf probe`, eBPF |
| 专家 | 自定义事件、硬件分析 | PMU, Intel PT |

### 8.3 相关工具

| 工具 | 用途 |
|------|------|
| FlameGraph | 火焰图可视化 |
| hotspot | perf 数据 GUI 分析 |
| perf-tools | perf 工具集 |
| eBPF/bpftrace | 高级内核追踪 |