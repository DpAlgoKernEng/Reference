# 示例：C/C++ Linux 内核开发学习路线图

## 基本信息

- **总时长**: 4年（零基础到专家）
- **核心语言**: C → 汇编（辅助）
- **目标定位**: Linux 内核开发工程师
- **适合人群**: 有编程基础，零内核开发经验
- **日投入**: 4小时
- **重点领域**: Linux 内核子系统开发

---

## 学习路线总览

```
Year 1: C 语言精通 + 操作系统原理（初级系统工程师）
Year 2: Linux 内核源码分析 + 驱动开发（中级内核工程师）
Year 3: 内核子系统深入 + 性能优化（高级内核工程师）
Year 4: 内核贡献 + 专家能力（Linux 内核专家）
```

---

## Year 1：C 语言精通 + 操作系统原理（第1-12个月）

### 阶段 1.1：C 语言精通（第1-6个月）

#### Week 1-2: C 语言基础

**Topic**: C 语言语法基础与内存模型

**Learning Actions**:
- Read: 《C Primer Plus（第6版）》第1-10章
- Practice: 编写基础数据结构实现
- Analyze: 标准库 stdio.h/string.h 源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-3 | 开发环境配置 + GCC 基础 | 4h | 开发环境配置文档 |
| 4-7 | 指针与内存管理 | 8h | 指针机制总结博客 |
| 8-10 | 结构体/联合体/枚举 | 6h | 数据结构示例项目 |
| 11-14 | 文件操作与预处理器 | 6h | 文件处理工具库 |

**Output Requirement**:
- Code: 实现动态数组/链表/栈/队列
- Documentation: 指针与内存管理学习笔记（1篇博客）

**Validation**:
- Self-test: 解释指针与数组的区别
- Practical: 编写无内存泄漏的数据结构库

---

#### Week 3-4: C 语言高级特性

**Topic**: C 语言高级编程技巧

**Learning Actions**:
- Read: 《C Primer Plus（第6版）》第11-17章
- Practice: 复杂指针使用场景
- Analyze: Linux 内核 C 代码风格

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-3 | 多维指针/函数指针 | 6h | 函数指针应用示例 |
| 4-7 | 位操作/位域 | 8h | 位操作工具库 |
| 8-10 | 动态内存管理深入 | 6h | 内存池实现 |
| 11-14 | 复杂声明解析 | 6h | typedef 使用指南 |

**Output Requirement**:
- Code: 实现内存池分配器
- Documentation: C 高级特性分析博客

**Validation**:
- Self-test: 解析复杂 C 声明
- Practical: 实现自定义内存分配器

---

#### Week 5-8: 数据结构与算法实现

**Topic**: C 语言实现核心数据结构

**Learning Actions**:
- Read: 《数据结构与算法分析——C语言描述》第1-7章
- Practice: 手写所有核心数据结构
- Analyze: Linux 内核数据结构实现（list/rbtree）

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 链表/栈/队列 | 8h | 基础数据结构库 |
| 5-8 | 哈希表实现 | 8h | 哈希表库 |
| 9-12 | 二叉树/红黑树 | 8h | 红黑树实现 |
| 13-16 | B+树实现 | 8h | B+树原型 |
| 17-20 | 图算法实现 | 8h | 图算法库 |
| 21-28 | 算法综合实践 | 12h | 算法测试套件 |

**Output Requirement**:
- Code: 完整数据结构库（10+种）
- Documentation: 数据结构性能对比博客

**Validation**:
- Self-test: 分析红黑树插入/删除流程
- Practical: 实现与内核 list_head 兼容的链表

---

#### Week 9-12: C 语言工程实践

**Topic**: C 语言项目工程化

**Learning Actions**:
- Read: 《C 专家编程》全书
- Practice: 大型 C 项目组织
- Analyze: Linux 内核编码风格文档

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Makefile/构建系统 | 8h | 构建系统模板 |
| 5-8 | 调试工具(gdb/valgrind) | 8h | 调试指南文档 |
| 9-12 | 单元测试框架 | 8h | 测试框架实现 |
| 13-16 | 性能分析工具 | 8h | 性能分析报告 |
| 17-20 | 代码规范检查 | 8h | 代码规范文档 |
| 21-28 | 综合项目实践 | 12h | CLI 工具项目 |

**Output Requirement**:
- Code: 完整的 C 项目模板（包含构建/测试/调试）
- Documentation: C 工程化实践博客（2篇）

**Validation**:
- Self-test: 使用 gdb 分析程序崩溃
- Practical: 项目通过 valgrind 内存检查

---

### 阶段 1.2：汇编语言基础（第7-8个月）

#### 汇编语言入门

| Week | Topic | Tasks | Output |
|------|-------|-------|--------|
| 1-2 | x86-64 汇编基础 | 《汇编语言（第3版）》+ 指令集学习 | 汇编基础示例 |
| 3-4 | C 与汇编混合编程 | inline asm/函数调用约定 | 混合编程示例 |
| 5-6 | 汇编优化技巧 | 内联汇编优化关键代码 | 性能对比报告 |
| 7-8 | 反汇编分析 | objdump/gdb 反汇编实践 | 反汇编分析文档 |

**Milestone**: 能读懂 C 编译后的汇编代码

---

### 阶段 1.3：操作系统原理（第9-12个月）

#### 操作系统核心概念

| Week | Topic | Tasks | Output |
|------|-------|-------|--------|
| 1-2 | 进程与线程 | 《操作系统导论》进程章节 + 实验 | 进程调度模拟器 |
| 3-4 | 内存管理 | 虚拟内存/分页/TLB | 内存分配器原型 |
| 5-6 | 文件系统 | 文件系统原理 + 简单实现 | 简易文件系统 |
| 7-8 | IO 系统 | 块设备/IO 调度 | IO 模拟器 |
| 9-10 | 中断与异常 | 中断处理/异常机制 | 中断模拟器 |
| 11-12 | 系统调用 | 系统调用机制实现 | 系统调用框架 |

**Milestone**: 实现简易操作系统内核模块

---

## Year 2：Linux 内核源码分析 + 驱动开发（第13-24个月）

### 阶段 2.1：Linux 内核基础（第13-16个月）

#### Week 13-14: 内核开发环境搭建

**Topic**: Linux 内核开发环境配置

**Learning Actions**:
- Read: 《Linux 内核设计与实现》第1-2章
- Practice: 内核编译与安装
- Analyze: 内核源码目录结构

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-3 | 内核源码获取与配置 | 4h | 环境配置文档 |
| 4-7 | 内核编译与安装 | 8h | 自定义内核 |
| 8-10 | 内核调试环境 | 6h | QEMU+GDB 调试环境 |
| 11-14 | 内核模块基础 | 6h | Hello World 模块 |

**Output Requirement**:
- Code: 基础内核模块（Hello World）
- Documentation: 内核开发环境搭建博客

**Validation**:
- Self-test: 成功编译并运行自定义内核
- Practical: 内核模块成功加载/卸载

---

#### Week 15-16: 内核数据结构

**Topic**: Linux 内核核心数据结构

**Learning Actions**:
- Read: 《Linux 内核设计与实现》第3-6章
- Practice: 使用内核数据结构
- Analyze: 内核 list_head/rbtree/kfifo 实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-3 | 链表 list_head | 6h | 链表使用示例 |
| 4-7 | 红黑树 rbtree | 8h | 红黑树模块 |
| 8-10 | 哈希表 hlist | 6h | 哈希表示例 |
| 11-14 | kfifo 环形缓冲区 | 6h | kfifo 应用 |

**Output Requirement**:
- Code: 内核数据结构使用示例
- Documentation: 内核数据结构分析博客

**Validation**:
- Self-test: 解释 list_head 的双向链表实现
- Practical: 在内核模块中使用 rbtree

---

#### Week 17-20: 进程管理

**Topic**: Linux 内核进程调度

**Learning Actions**:
- Read: 《Linux 内核设计与实现》第7章 + 《深入理解 Linux 内核》进程章节
- Practice: 分析调度器源码
- Analyze: CFS 调度器实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 进程描述符 task_struct | 8h | task_struct 分析文档 |
| 5-8 | 进程创建与终止 | 8h | 进程生命周期分析 |
| 9-12 | CFS 调度器原理 | 8h | CFS 分析博客 |
| 13-16 | 调度类与策略 | 8h | 调度策略对比报告 |
| 17-20 | 实时调度 | 8h | RT 调度分析 |
| 21-28 | 调度器实践 | 12h | 自定义调度类原型 |

**Output Requirement**:
- Code: 调度器分析工具
- Documentation: 进程调度深度分析博客（2篇）

**Validation**:
- Self-test: 解释 CFS 调度器的公平性实现
- Practical: 分析进程调度延迟

---

### 阶段 2.2：内存管理（第17-20个月）

#### Week 17-20: 内存管理子系统

**Topic**: Linux 内核内存管理

**Learning Actions**:
- Read: 《Linux 内核设计与实现》第8章 + 《深入理解 Linux 内核》内存章节
- Practice: 分析内存管理源码
- Analyze: 页分配器/Slab 分配器实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 内存Zone与页管理 | 8h | Zone 分析文档 |
| 5-8 | Buddy 分配器 | 8h | Buddy 系统分析 |
| 9-12 | Slab/Slob/Slub | 8h | Slab 分配器对比 |
| 13-16 | vmalloc/vmap | 8h | 虚拟内存分配 |
| 17-20 | 内存映射 | 8h | mmap 实现 |
| 21-28 | 内存管理实践 | 12h | 自定义 Slab 分配器 |

**Output Requirement**:
- Code: 内存分配器模块
- Documentation: 内存管理分析博客（2篇）

**Validation**:
- Self-test: 解释 Buddy 系统的伙伴合并算法
- Practical: 实现简化版 Slab 分配器

---

### 阶段 2.3：设备驱动开发（第21-24个月）

#### Week 21-24: 字符设备驱动

**Topic**: Linux 内核字符设备驱动

**Learning Actions**:
- Read: 《Linux 设备驱动程序（第3版）》第1-6章
- Practice: 编写字符设备驱动
- Analyze: 内核驱动示例代码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 字符设备框架 | 8h | 基础字符驱动 |
| 5-8 | file_operations 实现 | 8h | 完整字符驱动 |
| 9-12 | ioctl 实现 | 8h | ioctl 接口 |
| 13-16 | 阻塞与非阻塞 IO | 8h | IO 模式实现 |
| 17-20 | poll/select 实现 | 8h | 事件通知机制 |
| 21-28 | 驱动综合实践 | 12h | 功能完整驱动 |

**Output Requirement**:
- Code: 完整字符设备驱动（支持读写/ioctl/poll）
- Documentation: 驱动开发博客（2篇）

**Validation**:
- Self-test: 解释 file_operations 结构体
- Practical: 驱动通过内核测试套件

---

#### Week 25-28: 块设备驱动

**Topic**: Linux 内核块设备驱动

**Learning Actions**:
- Read: 《Linux 设备驱动程序（第3版）》第12-16章
- Practice: 编写块设备驱动
- Analyze: 块设备层实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 块设备框架 | 8h | 基础块驱动 |
| 5-8 | bio/request 处理 | 8h | IO 请求处理 |
| 9-12 | 块设备注册 | 8h | 设备注册流程 |
| 13-16 | 简易块设备实现 | 8h | 虚拟块设备 |
| 17-20 | 块设备优化 | 8h | 性能优化实践 |
| 21-28 | 块驱动综合实践 | 12h | 功能完整块驱动 |

**Output Requirement**:
- Code: 虚拟块设备驱动
- Documentation: 块设备驱动博客

**Validation**:
- Self-test: 解释 bio 与 request 的区别
- Practical: 块驱动支持文件系统挂载

---

## Year 3：内核子系统深入 + 性能优化（第25-36个月）

### 阶段 3.1：内核同步机制（第25-28个月）

#### Week 25-28: 内核同步原语

**Topic**: Linux 内核同步与并发

**Learning Actions**:
- Read: 《Linux 内核设计与实现》第9章 + 《深入理解 Linux 内核》同步章节
- Practice: 实现各种同步机制
- Analyze: 内核锁实现源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 原子操作 | 8h | 原子操作示例 |
| 5-8 | 自旋锁 spinlock | 8h | spinlock 实现 |
| 9-12 | 互斥锁 mutex | 8h | mutex 实现 |
| 13-16 | 读写锁 rwlock | 8h | rwlock 实现 |
| 17-20 | RCU 机制 | 8h | RCU 分析 |
| 21-28 | 同步机制实践 | 12h | 并发驱动模块 |

**Output Requirement**:
- Code: 同步机制示例库
- Documentation: 内核同步深度分析博客（2篇）

**Validation**:
- Self-test: 解释 RCU 的读写并发优化原理
- Practical: 实现无锁环形缓冲区

---

### 阶段 3.2：内核子系统深入（第29-34个月）

#### Week 29-32: 文件系统实现

**Topic**: Linux 内核文件系统

**Learning Actions**:
- Read: 《Linux 内核设计与实现》第12章 + 《深入理解 Linux 内核》文件系统章节
- Practice: 实现简化文件系统
- Analyze: ext4/VFS 实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | VFS 框架 | 8h | VFS 分析文档 |
| 5-8 | superblock/inode/dentry | 8h | VFS 对象实现 |
| 9-12 | 文件系统注册 | 8h | 文件系统模块 |
| 13-16 | 简易文件系统实现 | 8h | myfs 原型 |
| 17-20 | 文件读写实现 | 8h | 完整文件操作 |
| 21-28 | 文件系统综合实践 | 12h | 功能完整文件系统 |

**Output Requirement**:
- Code: 简化版文件系统 myfs（支持基本操作）
- Documentation: 文件系统实现博客（2篇）

**Validation**:
- Self-test: 解释 VFS 的抽象层设计
- Practical: 文件系统成功挂载并读写文件

---

#### Week 33-36: 网络子系统

**Topic**: Linux 内核网络协议栈

**Learning Actions**:
- Read: 《深入理解 Linux 内核》网络章节 + 《Linux 内核网络协议栈》
- Practice: 分析网络子系统
- Analyze: TCP/IP 协议栈实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Socket 实现 | 8h | Socket 分析 |
| 5-8 | TCP 协议实现 | 8h | TCP 分析文档 |
| 9-12 | 网络设备驱动 | 8h | 网络驱动分析 |
| 13-16 | Netfilter 框架 | 8h | Netfilter 分析 |
| 17-20 | 网络优化 | 8h | 性能优化分析 |
| 21-28 | 网络子系统实践 | 12h | 网络功能模块 |

**Output Requirement**:
- Code: Netfilter 模块/网络统计工具
- Documentation: 网络子系统分析博客（2篇）

**Validation**:
- Self-test: 解释 TCP 状态机转换
- Practical: 实现 Netfilter hook 模块

---

### 阶段 3.3：内核性能优化（第37-42个月）

#### Week 37-42: 内核性能工程

**Topic**: Linux 内核性能优化

**Learning Actions**:
- Read: 《性能之巅》内核章节 + perf/ftrace 文档
- Practice: 性能分析与优化
- Analyze: 内核性能瓶颈

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | perf 性能分析 | 8h | perf 使用指南 |
| 5-8 | ftrace 追踪 | 8h | ftrace 分析 |
| 9-12 | eBPF 技术 | 8h | eBPF 工具开发 |
| 13-16 | 内核锁优化 | 8h | 锁优化实践 |
| 17-20 | 内存优化 | 8h | 内存性能优化 |
| 21-28 | 性能优化实践 | 12h | 性能补丁集 |

**Output Requirement**:
- Code: eBPF 性能分析工具
- Documentation: 性能优化博客（2篇）

**Validation**:
- Self-test: 使用 perf 分析内核热点
- Practical: 实现性能优化补丁（提升10%+）

---

### 阶段 3.4：内核调试技术（第43-48个月）

#### Week 43-48: 内核调试与故障排查

**Topic**: Linux 内核调试技术

**Learning Actions**:
- Read: 《Linux 内核调试技术》 + 内核文档 Documentation/
- Practice: 内核调试与故障排查
- Analyze: 内核崩溃案例

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | KGDB 内核调试 | 8h | KGDB 使用指南 |
| 5-8 | Kdump/crash 分析 | 8h | 崩溃分析实践 |
| 9-12 |内核日志与追踪 | 8h | 日志分析工具 |
| 13-16 | 死锁检测 | 8h | 死锁分析工具 |
| 17-20 | 内存泄漏检测 | 8h | 泄漏检测工具 |
| 21-28 | 调试综合实践 | 12h | 故障排查案例 |

**Output Requirement**:
- Code: 内核调试工具集
- Documentation: 调试技术博客（2篇）

**Validation**:
- Self-test: 分析内核崩溃 dump
- Practical: 定位并修复内核内存泄漏

---

## Year 4：内核贡献 + 专家能力（第37-48个月）

### 阶段 4.1：内核子系统专家化（第37-42个月）

#### Week 37-42: 深入特定子系统

**Topic**: 专业化某个内核子系统

**Learning Actions**:
- Read: 内核子系统文档 + 相关论文
- Practice: 深入研究特定子系统
- Analyze: 子系统最新发展

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 子系统源码全面分析 | 8h | 子系统分析文档 |
| 5-8 | 子系统性能瓶颈分析 | 8h | 性能分析报告 |
| 9-12 | 子系统优化方案设计 | 8h | 优化方案文档 |
| 13-16 | 优化补丁实现 | 8h | 功能补丁 |
| 17-20 | 补丁测试与验证 | 8h | 测试报告 |
| 21-28 | 子系统专家实践 | 12h | 专家级贡献 |

**Output Requirement**:
- Code: 子系统优化补丁集
- Documentation: 子系统深度分析博客（3篇）

**Validation**:
- Self-test: 成为子系统领域专家
- Practical: 补丁通过内核测试

---

### 阶段 4.2：内核社区贡献（第43-48个月）

#### Week 43-48: Linux 内核社区参与

**Topic**: 成为 Linux 内核贡献者

**Learning Actions**:
- Read: 内核文档 Documentation/process/
- Practice: 提交内核补丁
- Analyze: 内核邮件列表流程

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 内核补丁规范学习 | 8h | 补丁规范文档 |
| 5-8 | checkpatch.pl 使用 | 8h | 补丁检查工具 |
| 9-12 | git format-patch | 8h | 补丁格式化 |
| 13-16 | 邮件列表订阅与参与 | 8h | 社区参与 |
| 17-20 | 补丁提交流程 | 8h | 首个补丁提交 |
| 21-28 | 内核贡献实践 | 12h | 多个补丁提交 |

**Output Requirement**:
- Code: 提交到内核主线的补丁
- Documentation: 内核贡献指南博客

**Validation**:
- Self-test: 理解内核补丁提交流程
- Practical: 补丁被内核主线接受

---

## 关键项目清单

| Year | Project | Key Tech | Performance Target | Duration |
|------|---------|----------|-------------------|----------|
| 1 | C 数据结构库 | 指针/内存管理 | 无内存泄漏 | 8周 |
| 1 | 内存池分配器 | 动态内存管理 | 性能优于 malloc | 4周 |
| 1 | 简易操作系统内核 | 进程/内存/文件系统 | 基本功能运行 | 12周 |
| 2 | 内核调试环境 | QEMU/GDB/kgdb | 完整调试能力 | 4周 |
| 2 | 字符设备驱动 | file_operations/ioctl | 功能完整 | 8周 |
| 2 | 块设备驱动 | bio/request | 支持文件系统 | 8周 |
| 3 | 简化文件系统 | VFS/inode/dentry | 成功挂载 | 12周 |
| 3 | eBPF 性能工具 | perf/ftrace/eBPF | 性能分析能力 | 8周 |
| 3 | 内核调试工具集 | kdump/crash/kgdb | 故障排查能力 | 8周 |
| 4 | 子系统优化补丁 | 性能优化 | 性能提升10%+ | 12周 |
| 4 | 内核主线补丁 | 补丁规范 | 补丁被接受 | 12周 |

---

## 里程碑检验标准

| Milestone | Knowledge Check | Project Completion | External Validation |
|-----------|-----------------|-------------------|-------------------|
| Year 1 Q2 | C 指针/内存管理 | 数据结构库 | 无内存泄漏测试 |
| Year 1 Q4 | 操作系统原理 | 简易内核模块 | 功能运行验证 |
| Year 2 Q2 | 内核数据结构/进程调度 | 内核调试环境 | 调试功能验证 |
| Year 2 Q4 | 内存管理/驱动开发 | 字符/块设备驱动 | 驱动功能测试 |
| Year 3 Q2 | 内核同步机制 | 同步机制模块 | 并发测试通过 |
| Year 3 Q4 | 文件系统/网络子系统 | 简化文件系统 | 挂载读写测试 |
| Year 4 Q2 | 性能优化/调试技术 | eBPF 工具/调试工具集 | 性能提升验证 |
| Year 4 Q4 | 子系统专家能力 | 内核主线补丁 | 补丁被接受 |

---

## 开源贡献路径

| Year | Target Project | Contribution Type | Goal |
|------|----------------|-------------------|------|
| 2 | Linux 内核文档 | 文档翻译/修正 | 1 PR merged |
| 3 | Linux 内核驱动 | Bug修复/小功能 | 2 PR merged |
| 4 | Linux 内核子系统 | 性能优化/功能开发 | Contributor |

---

## 学习资源汇总

### 书籍清单

| Category | Book Name | Priority | Chapters |
|----------|-----------|----------|----------|
| C 基础 | 《C Primer Plus（第6版）》 | 高 | 全书 |
| C 高级 | 《C 专家编程》 | 高 | 全书 |
| C 高级 | 《C 和指针》 | 高 | 全书 |
| 汇编 | 《汇编语言（第3版）》王爽 | 中 | 全书 |
| 操作系统 | 《操作系统导论》 | 高 | 核心3章 |
| 内核基础 | 《Linux 内核设计与实现》 | 高 | 全书 |
| 内核深入 | 《深入理解 Linux 内核》 | 高 | 核心8章 |
| 驱动开发 | 《Linux 设备驱动程序（第3版）》 | 高 | 第1-16章 |
| 性能优化 | 《性能之巅》 | 中 | 内核章节 |
| 调试技术 | 《Linux 内核调试技术》 | 中 | 全书 |
| 数据结构 | 《数据结构与算法分析——C语言描述》 | 中 | 第1-7章 |

### 文档/源码

| Type | Name | Link | Purpose |
|------|------|------|---------|
| 官方文档 | Linux Kernel Documentation | https://www.kernel.org/doc/ | 内核开发指南 |
| 源码分析 | Linux Kernel Source | https://github.com/torvalds/linux | 内核实现学习 |
| 邮件列表 | LKML | https://lkml.org/ | 内核社区参与 |
| 中文文档 | Linux 内核中文文档 | https://www.kernel.org/doc/Documentation/translations/zh_CN/ | 中文学习 |

---

## 进度追踪模板

详见 `references/roadmap-template.md` 中的"进度追踪模板"章节。

---

## Quick Start Guide（前3个月详细任务）

### Month 1: C 语言基础

**Week 1**:
- Day 1-2: 开发环境配置 + GCC 基础
- Day 3-4: 变量/类型/运算符
- Day 5-6: 指针基础概念
- Day 7: 学习笔记整理

**Week 2**:
- Day 1-2: 指针深入 + 数组
- Day 3-4: 指针运算/多级指针
- Day 5-6: 动态内存管理
- Day 7: 练习巩固

**Week 3-4**: 结构体/联合体/文件操作

---

### Month 2: C 语言进阶

**Week 1-2**: 函数指针/位操作/预处理器
**Week 3-4**: 内存池/数据结构实现

---

### Month 3: 数据结构与算法

**Week 1-2**: 链表/栈/队列/哈希表
**Week 3-4**: 树结构/红黑树/B+树

---

*此路线图为零内核开发经验开发者设计，4年达到 Linux 内核工程师水平。*