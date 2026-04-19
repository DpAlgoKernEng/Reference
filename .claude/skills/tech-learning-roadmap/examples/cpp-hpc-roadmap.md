# 示例：C++ HPC 高性能计算开发学习路线图

## 基本信息

- **总时长**: 4年（零基础到专家）
- **核心语言**: C++（主）→ Python/Julia（辅助）
- **目标定位**: HPC高性能计算专家
- **适合人群**: 有编程基础，零并行计算经验
- **日投入**: 4小时
- **重点领域**: 高性能科学计算/并行系统

---

## 学习路线总览

```
Year 1: C++ 语言精通 + 并行计算基础（初级HPC工程师）
Year 2: CUDA/MPI/OpenMP 深入（中级HPC工程师）
Year 3: 高性能算法 + 性能优化专家（高级HPC工程师）
Year 4: 大规模并行系统 + 专家能力（HPC系统专家）
```

---

## Year 1：C++ 语言精通 + 并行计算基础（第1-12个月）

### 阶段 1.1：C++ 现代特性精通（第1-6个月）

#### Week 1-4: C++ 基础与内存模型

**Topic**: C++ 核心语法与内存管理

**Learning Actions**:
- Read: 《C++ Primer 第5版》第1-12章
- Practice: C++ Core Guidelines 练习集
- Analyze: STL 容器源码（vector/map）

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-5 | 环境配置 + 编译工具链 | 8h | CMake/Compiler 配置文档 |
| 6-10 | 现代C++特性（auto/lambda） | 8h | 新特性示例代码库 |
| 11-15 | 智能指针与RAII | 8h | 内存管理工具库 |
| 16-20 | 右值引用与移动语义 | 8h | 移动语义分析报告 |

**Output Requirement**:
- Code: STL风格容器实现（2000+行）
- Documentation: 内存模型学习笔记（1篇博客）

**Validation**:
- Self-test: 解释RAII与智能指针原理
- Practical: 内存泄漏检测工具通过（valgrind零泄漏）

---

#### Week 5-8: 模板与泛型编程

**Topic**: C++ 模板元编程与泛型编程

**Learning Actions**:
- Read: 《C++ 模板元编程》全书
- Practice: 实现类型计算与编译期优化
- Analyze: Boost 库模板源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 函数模板/类模板 | 8h | 泛型算法库 |
| 5-8 | 模板特化与偏特化 | 8h | 特化模式示例集 |
| 9-12 | 可变参数模板 | 8h | 可变参数工具 |
| 13-16 | SFINAE与类型萃取 | 8h | 类型萃取库 |
| 17-20 | constexpr与编译期计算 | 8h | 编译期数学库 |
| 21-28 | 模板实战项目 | 12h | 泛型数值计算库 |

**Output Requirement**:
- Code: 编译期计算库（支持constexpr）
- Documentation: 模板元编程分析博客（2篇）

**Validation**:
- Self-test: 解释SFINAE原理与应用场景
- Practical: 模板库编译时间 < 5秒（1000行）

---

#### Week 9-12: C++ 并发基础

**Topic**: C++ 多线程与原子操作

**Learning Actions**:
- Read: 《C++ 并发编程实战 第2版》第1-7章
- Practice: 实现线程安全数据结构
- Analyze: std::thread/atomic 源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 线程基础与同步原语 | 8h | 多线程任务系统 |
| 5-8 | 原子操作与内存序 | 8h | 原子操作工具库 |
| 9-12 | 线程安全容器 | 8h | 并发队列/哈希表 |
| 13-16 | 无锁编程入门 | 8h | 无锁队列实现 |
| 17-20 | 线程池实现 | 8h | 高性能线程池 |
| 21-28 | 并发项目实践 | 12h | 并行数据处理框架 |

**Output Requirement**:
- Code: 无锁队列 + 线程池（1000+行）
- Documentation: 并发模型分析博客（2篇）

**Validation**:
- Self-test: 解释内存序（memory_order）选择
- Practical: ThreadSanitizer 零错误检测

---

#### Week 13-16: C++ 性能优化基础

**Topic**: C++ 性能分析与基础优化

**Learning Actions**:
- Read: 《C++ 性能优化指南》核心章节
- Practice: 性能分析与优化实践
- Analyze: 性能热点识别方法

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 性能分析工具 | 8h | perf/gprof 使用指南 |
| 5-8 | 缓存友好代码 | 8h | 缓存优化示例集 |
| 9-12 | SIMD 基础（SSE/AVX） | 8h | SIMD 向量化示例 |
| 13-16 | 内存布局优化 | 8h | 内存对齐工具 |
| 17-20 | 编译器优化选项 | 8h | 编译优化矩阵 |
| 21-28 | 性能优化项目 | 12h | 高性能矩阵运算库 |

**Output Requirement**:
- Code: SIMD优化矩阵库（500+行）
- Documentation: 性能优化实战博客（2篇）

**Validation**:
- Self-test: 解释缓存命中率优化策略
- Practical: 矩阵乘法性能提升 > 10倍（对比朴素实现）

---

#### Week 17-24: 综合项目实践

**Topic**: C++ 高性能计算项目

**Specific Tasks**:
| Week | Task | Output |
|------|------|--------|
| 1-2 | 高性能数值库设计 | 数学函数库（SIMD优化） |
| 3-4 | 并行数据处理框架 | 数据流处理引擎 |
| 5-6 | 性能基准测试系统 | Benchmark框架 |
| 7-8 | 综合性能优化项目 | 优化版数值计算库 |

**Milestone**: 单线程性能优化专家（性能提升 > 20倍）

---

### 阶段 1.2：并行计算基础（第7-12个月）

#### Week 25-28: OpenMP 基础

**Topic**: 共享内存并行编程

**Learning Actions**:
- Read: 《OpenMP 编程指南》核心章节
- Practice: OpenMP 并行化实践
- Analyze: OpenMP 性能影响因素

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | OpenMP 基本指令 | 8h | 并行循环示例集 |
| 5-8 | 数据共享与同步 | 8h | 共享变量管理工具 |
| 9-12 | 任务并行 | 8h | 任务调度系统 |
| 13-16 | SIMD 与 OpenMP 结合 | 8h | SIMD+OpenMP 示例 |
| 17-20 | OpenMP 性能调优 | 8h | 性能优化指南 |
| 21-28 | OpenMP 项目 | 12h | 并行科学计算库 |

**Output Requirement**:
- Code: OpenMP 并行库（1000+行）
- Documentation: OpenMP 优化博客（1篇）

**Validation**:
- Self-test: 解释 OpenMP 线程调度策略
- Practical: 8核并行效率 > 75%（相对单线程）

---

#### Week 29-32: 并行算法基础

**Topic**: 并行算法设计

**Learning Actions**:
- Read: 《并行算法导论》核心章节
- Practice: 并行算法实现
- Analyze: 算法并行化策略

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 并行排序算法 | 8h | 并行排序库 |
| 5-8 | 并行搜索算法 | 8h | 并行查找工具 |
| 9-12 | 并行图算法 | 8h | 图算法并行库 |
| 13-16 | 并行数值算法 | 8h | 数值计算库 |
| 17-20 | MapReduce 模式 | 8h | MapReduce 框架 |
| 21-28 | 并行算法项目 | 12h | 综合算法库 |

**Output Requirement**:
- Code: 并行算法库（1500+行）
- Documentation: 并行算法分析博客（2篇）

**Validation**:
- Self-test: 解释并行算法复杂度分析
- Practical: 并行排序性能 > 100M元素/秒

---

#### Week 33-36: 计算机体系结构基础

**Topic**: HPC 系统架构理解

**Learning Actions**:
- Read: 《计算机体系结构：量化研究方法》核心章节
- Practice: 架构性能建模
- Analyze: 性能瓶颈分析方法

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | CPU 架构深入 | 8h | 架构分析报告 |
| 5-8 | 内存层次结构 | 8h | 内存性能模型 |
| 9-12 | 缓存架构 | 8h | 缓存优化工具 |
| 13-16 | 向量架构 | 8h | SIMD 性能分析 |
| 17-20 | 多核架构 | 8h | 多核性能模型 |
| 21-28 | 架构性能建模 | 12h | Roofline 模型实现 |

**Output Requirement**:
- Code: Roofline 性能分析工具
- Documentation: 体系结构分析博客（2篇）

**Validation**:
- Self-test: 解释 Roofline 模型原理
- Practical: 性能预测误差 < 10%

---

#### Week 37-48: Year 1 综合项目

**Project**: 高性能矩阵计算库

| Week | Feature | Performance Target |
|------|---------|-------------------|
| 1-4 | 基础矩阵运算 | 单线程性能 > BLAS Level-1 |
| 5-8 | SIMD 优化 | 性能提升 > 10倍 |
| 9-12 | OpenMP 并行化 | 8核效率 > 70% |
| 13-16 | 缓存优化 | 缓存命中率 > 90% |

**Milestone**: 矩阵乘法性能 > 50 GFLOPS（单节点）

---

## Year 2：CUDA/MPI/OpenMP 深入（第13-24个月）

### 阶段 2.1：CUDA GPU 编程（第13-18个月）

#### Week 1-4: CUDA 基础

**Topic**: GPU 编程模型

**Learning Actions**:
- Read: 《CUDA C 编程权威指南》第1-6章
- Practice: CUDA 核函数实现
- Analyze: GPU 架构特性

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | CUDA 环境配置 | 8h | CUDA 开发环境文档 |
| 5-8 | 核函数基础 | 8h | 核函数示例集 |
| 9-12 | 内存层次结构 | 8h | GPU 内存管理工具 |
| 13-16 | 线程组织与同步 | 8h | 线程调度示例 |
| 17-20 | CUDA 性能分析 | 8h | Profiler 使用指南 |
| 21-28 | CUDA 基础项目 | 12h | GPU 向量运算库 |

**Output Requirement**:
- Code: CUDA 基础库（1000+行）
- Documentation: GPU 编程模型博客（1篇）

**Validation**:
- Self-test: 解释 GPU 执行模型
- Practical: GPU 利用率 > 80%（简单核函数）

---

#### Week 5-8: CUDA 性能优化

**Topic**: GPU 性能调优

**Learning Actions**:
- Read: 《CUDA C 编程权威指南》第7-12章
- Practice: 性能优化实践
- Analyze: 性能瓶颈识别

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 内存访问优化 | 8h | 共享内存优化示例 |
| 5-8 | 并行执行优化 | 8h | 占用率优化工具 |
| 9-12 | 指令优化 | 8h | 指令级并行示例 |
| 13-16 | Warp 级优化 | 8h | Warp 优化示例集 |
| 17-20 | 异步执行 | 8h | Stream/Event 管理 |
| 21-28 | 性能优化项目 | 12h | 优化版矩阵运算 |

**Output Requirement**:
- Code: 优化版 CUDA 库（1500+行）
- Documentation: GPU 优化博客（2篇）

**Validation**:
- Self-test: 解释占用率计算方法
- Practical: 矩阵乘法 > 500 GFLOPS（单GPU）

---

#### Week 9-12: CUDA 高级特性

**Topic**: 高级 GPU 编程

**Learning Actions**:
- Read: 《Professional CUDA C Programming》核心章节
- Practice: 高级特性应用
- Analyze: 复杂核函数优化

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 动态并行 | 8h | 动态并行示例 |
| 5-8 | Cooperative Groups | 8h | 协作组示例集 |
| 9-12 | CUDA Streams 高级 | 8h | 多流管理框架 |
| 13-16 | Unified Memory | 8h | 统一内存示例 |
| 17-20 | Multi-GPU 编程 | 8h | 多 GPU 通信库 |
| 21-28 | 高级 CUDA 项目 | 12h | 多 GPU 计算框架 |

**Output Requirement**:
- Code: 多 GPU 计算框架（2000+行）
- Documentation: 高级特性博客（2篇）

**Validation**:
- Self-test: 解释 Unified Memory 优势与限制
- Practical: 多 GPU 扩展效率 > 85%（2 GPU vs 1 GPU）

---

#### Week 13-24: CUDA 项目实战

**Project**: GPU 加速科学计算库

| Week | Feature | Performance Target |
|------|---------|-------------------|
| 1-4 | 矩阵运算库 | > 1 TFLOPS（FP32） |
| 5-8 | FFT 实现 | > 100 GFLOPS |
| 9-12 | 稀疏矩阵运算 | > 50 GFLOPS |
| 13-16 | 多 GPU 并行 | 扩展效率 > 90% |
| 17-20 | 综合性能优化 | 接近 cuBLAS 性能 |
| 21-24 | 文档与测试 | 完整文档 + 测试套件 |

**Milestone**: GPU 矩阵乘法 > 2 TFLOPS（单卡）

---

### 阶段 2.2：MPI 分布式并行（第19-24个月）

#### Week 25-28: MPI 基础

**Topic**: 分布式内存并行

**Learning Actions**:
- Read: 《MPI 高性能编程》第1-8章
- Practice: MPI 通信实现
- Analyze: MPI 性能特性

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | MPI 环境配置 | 8h | MPI 开发环境文档 |
| 5-8 | 点对点通信 | 8h | 点对点通信库 |
| 9-12 | 集合通信 | 8h | 集合通信示例集 |
| 13-16 | 拓扑与虚拟拓扑 | 8h | 拓扑管理工具 |
| 17-20 | MPI 性能分析 | 8h | 性能分析指南 |
| 21-28 | MPI 基础项目 | 12h | 分布式向量运算 |

**Output Requirement**:
- Code: MPI 通信库（1000+行）
- Documentation: MPI 编程博客（1篇）

**Validation**:
- Self-test: 解释 MPI 通信模式选择
- Practical: 16进程并行效率 > 85%

---

#### Week 29-32: MPI 性能优化

**Topic**: MPI 通信优化

**Learning Actions**:
- Read: 《Parallel Programming with MPI》性能章节
- Practice: 通信优化实践
- Analyze: 通信瓶颈分析

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 非阻塞通信 | 8h | 异步通信库 |
| 5-8 | 通信与计算重叠 | 8h | 重叠优化示例 |
| 9-12 | 消息聚合 | 8h | 消息优化工具 |
| 13-16 | 拓扑优化 | 8h | 拓扑优化框架 |
| 17-20 | MPI + OpenMP 混合 | 8h | 混合并行示例 |
| 21-28 | MPI 优化项目 | 12h | 优化版分布式计算 |

**Output Requirement**:
- Code: MPI 优化库（1500+行）
- Documentation: MPI 优化博客（2篇）

**Validation**:
- Self-test: 解释通信隐藏策略
- Practical: 32进程扩展效率 > 80%

---

#### Week 33-48: MPI + CUDA 混合并行

**Project**: MPI+CUDA 分布式模拟系统

| Week | Feature | Performance Target |
|------|---------|-------------------|
| 1-8 | MPI+CUDA 框架设计 | 多节点 GPU 通信库 |
| 9-16 | 分布式矩阵计算 | 4节点 > 8 TFLOPS |
| 17-24 | 分布式模拟系统 | 扩展效率 > 75%（8节点） |
| 25-32 | 性能优化与文档 | 接近理论峰值性能 |

**Milestone**: 8节点并行效率 > 70%（相对单节点）

---

## Year 3：高性能算法 + 性能优化专家（第25-36个月）

### 阶段 3.1：高性能数值算法（第25-30个月）

#### Week 1-8: 矩阵算法优化

**Topic**: 高性能线性代数

**Learning Actions**:
- Read: 《数值线性代数》核心章节
- Practice: 算法优化实现
- Analyze: LAPACK/BLAS 源码

**Specific Tasks**:
| Week | Topic | Output |
|------|-------|--------|
| 1-2 | 矩阵分解（LU/QR/SVD） | 分解算法库 |
| 3-4 | 稀疏矩阵算法 | 稀疏求解器 |
| 5-6 | 迭代求解器 | Krylov 子空间方法 |
| 7-8 | 算法并行化 | 并行求解器 |

**Output Requirement**:
- Code: 线性代数库（3000+行）
- Documentation: 算法优化博客（3篇）

**Validation**:
- Self-test: 解释算法稳定性分析
- Practical: 性能接近 MKL（相对误差 < 20%）

---

#### Week 9-16: FFT 与谱算法

**Topic**: 高性能谱算法

**Learning Actions**:
- Read: 《FFT 算法与应用》核心内容
- Practice: FFT 优化实现
- Analyze: FFTW 源码

**Specific Tasks**:
| Week | Topic | Output |
|------|-------|--------|
| 1-2 | FFT 算法实现 | FFT 库原型 |
| 3-4 | FFT 性能优化 | 优化版 FFT |
| 5-6 | 多维 FFT | 多维 FFT 库 |
| 7-8 | GPU FFT 实现 | CUDA FFT |

**Output Requirement**:
- Code: FFT 库（2000+行）
- Documentation: FFT 优化博客（2篇）

**Validation**:
- Self-test: 解释 FFT 性能影响因素
- Practical: FFT 性能 > 100 GFLOPS

---

#### Week 17-24: 科学计算算法

**Topic**: 高性能科学计算

**Learning Actions**:
- Read: 《科学计算导论》核心章节
- Practice: 科学计算实现
- Analyze: 科学计算库源码

**Specific Tasks**:
| Week | Topic | Output |
|------|-------|--------|
| 1-2 | 数值积分 | 积分算法库 |
| 3-4 | ODE/PDE 求解器 | 方程求解器 |
| 5-6 | 优化算法 | 优化求解器 |
| 7-8 | 并行科学计算 | 并行求解框架 |

**Output Requirement**:
- Code: 科学计算库（2500+行）
- Documentation: 科学计算博客（2篇）

**Validation**:
- Self-test: 解释算法收敛性分析
- Practical: 并行求解加速 > 10倍

---

### 阶段 3.2：性能优化专家能力（第31-36个月）

#### Week 25-28: 性能分析方法

**Topic**: 性能瓶颈诊断

**Learning Actions**:
- Read: 《性能优化方法论》核心内容
- Practice: 性能分析实践
- Analyze: 性能建模方法

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 性能分析工具链 | 8h | 工具链集成框架 |
| 5-8 | Roofline 模型应用 | 8h | Roofline 分析工具 |
| 9-12 | 性能瓶颈识别 | 8h | 瓶颈诊断工具 |
| 13-16 | 性能建模 | 8h | 性能预测模型 |
| 17-20 | 自动性能调优 | 8h | 自动调优框架 |
| 21-28 | 性能分析项目 | 12h | 综合分析系统 |

**Output Requirement**:
- Code: 性能分析工具（1500+行）
- Documentation: 性能分析方法博客（2篇）

**Validation**:
- Self-test: 解释性能优化决策流程
- Practical: 性能预测准确度 > 90%

---

#### Week 29-32: 缓存与内存优化

**Topic**: 内存系统优化

**Learning Actions**:
- Read: 《内存优化技术》核心内容
- Practice: 缓存优化实践
- Analyze: 内存性能瓶颈

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 缓存友好算法设计 | 8h | 缓存优化算法库 |
| 5-8 | 数据布局优化 | 8h | 数据布局工具 |
| 9-12 | 内存预取 | 8h | 预取优化示例 |
| 13-16 | NUMA 优化 | 8h | NUMA 优化工具 |
| 17-20 | 内存带宽优化 | 8h | 带宽优化框架 |
| 21-28 | 内存优化项目 | 12h | 优化版数值库 |

**Output Requirement**:
- Code: 内存优化库（2000+行）
- Documentation: 内存优化博客（2篇）

**Validation**:
- Self-test: 解释缓存命中率优化策略
- Practical: 内存带宽利用率 > 80%

---

#### Week 33-48: Year 3 综合项目

**Project**: 高性能科学计算库

| Week | Feature | Performance Target |
|------|---------|-------------------|
| 1-8 | 核心算法库 | 性能接近 MKL/FFTW |
| 9-16 | 并行求解器 | 16进程效率 > 85% |
| 17-24 | GPU 加速版本 | GPU 性能 > 80%理论峰值 |
| 25-32 | 综合优化 | 接近最优性能 |

**Milestone**: 矩阵求解性能 > 500 GFLOPS（单节点）

---

## Year 4：大规模并行系统 + 专家能力（第37-48个月）

### 阶段 4.1：大规模并行系统（第37-42个月）

#### Week 1-8: 大规模并行架构

**Topic**: 集群与超算系统

**Learning Actions**:
- Read: 《高性能计算系统架构》核心章节
- Practice: 大规模并行设计
- Analyze: 超算系统案例

**Specific Tasks**:
| Week | Topic | Output |
|------|-------|--------|
| 1-2 | 集群架构设计 | 集群设计方案 |
| 3-4 | 互连网络优化 | 通信优化框架 |
| 5-6 | 存储系统设计 | 存储系统方案 |
| 7-8 | 系统平衡性分析 | 平衡性分析工具 |

**Output Requirement**:
- Code: 集群管理工具（2000+行）
- Documentation: 大规模系统博客（2篇）

**Validation**:
- Self-test: 解释系统平衡性原理
- Practical: 64节点扩展效率 > 60%

---

#### Week 9-16: 性能可扩展性

**Topic**: 强/弱可扩展性优化

**Learning Actions**:
- Read: 《并行计算可扩展性》核心内容
- Practice: 可扩展性优化
- Analyze: 可扩展性瓶颈

**Specific Tasks**:
| Week | Topic | Output |
|------|-------|--------|
| 1-2 | 强可扩展性优化 | 强扩展优化方案 |
| 3-4 | 弱可扩展性优化 | 弱扩展优化方案 |
| 5-6 | 通信开销优化 | 通信优化库 |
| 7-8 |负载平衡优化 | 负载平衡框架 |

**Output Requirement**:
- Code: 可扩展性优化库（1500+行）
- Documentation: 可扩展性博客（2篇）

**Validation**:
- Self-test: 解释强/弱可扩展性区别
- Practical: 128节点弱扩展效率 > 90%

---

#### Week 17-24: 分布式存储与IO

**Topic**: 高性能IO系统

**Learning Actions**:
- Read: 《并行IO系统》核心内容
- Practice: IO优化实践
- Analyze: IO瓶颈分析

**Specific Tasks**:
| Week | Topic | Output |
|------|-------|--------|
| 1-2 | 并行文件系统 | 并行IO库 |
| 3-4 | IO 缓存优化 | IO 缓存框架 |
| 5-6 | 数据压缩 | 压缩工具 |
| 7-8 | 分布式存储 | 存储管理工具 |

**Output Requirement**:
- Code: IO 优化库（1500+行）
- Documentation: IO 优化博客（2篇）

**Validation**:
- Self-test: 解释并行IO性能影响因素
- Practical: IO 吞吐 > 10 GB/s

---

### 阶段 4.2：专家能力培养（第43-48个月）

#### Week 25-32: 性能自动调优

**Topic**: 自动性能优化

**Learning Actions**:
- Read: 《自动调优技术》核心内容
- Practice: 自动调优实现
- Analyze: ATLAS/FFTW 自调优机制

**Specific Tasks**:
| Week | Topic | Output |
|------|-------|--------|
| 1-2 | 参数自动调优 | 自动调优框架 |
| 3-4 | 代码生成优化 | 代码生成器 |
| 5-6 | 搜索算法优化 | 搜索优化工具 |
| 7-8 | 综合调优系统 | 调优系统原型 |

**Output Requirement**:
- Code: 自动调优系统（2500+行）
- Documentation: 自动调优博客（2篇）

**Validation**:
- Self-test: 解释自动调优原理
- Practical: 自动调优性能接近手工优化

---

#### Week 33-48: 专家项目与开源贡献

**Project**: 大规模并行模拟系统

| Week | Feature | Performance Target |
|------|---------|-------------------|
| 1-8 | 系统架构设计 | 256节点设计方案 |
| 9-16 | 核心实现 | 128节点部署运行 |
| 17-24 | 性能优化 | 扩展效率 > 60% |
| 25-32 | 文档与开源 | 开源发布 |

**开源贡献路径**:

| Year | Target Project | Contribution Type | Goal |
|------|----------------|-------------------|------|
| 2 | BLAS/LAPACK | Bug修复/文档 | 1 PR merged |
| 3 | PETSc/Trilinos | 功能开发 | 2 PR merged |
| 4 | Kokkos/RAJA | 性能优化 | Contributor |

**Milestone**: 成为 HPC 开源项目 Contributor + 性能接近顶级超算应用

---

## 关键项目清单

| Year | Project | Key Tech | Performance Target | Duration |
|------|---------|----------|-------------------|----------|
| 1 | 高性能矩阵计算库 | SIMD/OpenMP | > 50 GFLOPS（单节点） | 12周 |
| 2 | GPU 加速科学计算库 | CUDA | > 2 TFLOPS（单GPU） | 12周 |
| 2 | MPI 分布式模拟系统 | MPI+CUDA | 4节点 > 8 TFLOPS | 16周 |
| 3 | 高性能科学计算库 | 全栈技术 | > 500 GFLOPS | 24周 |
| 4 | 大规模并行模拟系统 | 大规模并行 | 128节点效率 > 60% | 16周 |

---

## 里程碑检验标准

| Milestone | Knowledge Check | Project Completion | External Validation |
|-----------|-----------------|-------------------|-------------------|
| Year 1 Q2 | C++ 现代特性/并发基础 | SIMD 优化矩阵库 | 性能 > 10倍提升 |
| Year 1 Q4 | 并行计算基础/OpenMP | OpenMP 并行库 | 8核效率 > 75% |
| Year 2 Q2 | CUDA 编程模型 | GPU 计算框架 | GPU利用率 > 80% |
| Year 2 Q4 | MPI 分布式并行 | MPI+CUDA 系统 | 8节点效率 > 70% |
| Year 3 Q2 | 高性能算法 | 科学计算库 | 性能接近 MKL |
| Year 3 Q4 | 性能优化专家能力 | 优化版求解器 | 带宽利用率 > 80% |
| Year 4 Q2 | 大规模并行系统 | 集群设计方案 | 64节点部署 |
| Year 4 Q4 | 专家能力/开源贡献 | 大规模系统 + 开源 | Contributor + 论文 |

---

## 开源贡献路径

| Year | Target Project | Contribution Type | Goal |
|------|----------------|-------------------|------|
| 1 | Eigen/Armadillo | Bug修复/文档 | 1 PR merged |
| 2 | BLAS/LAPACK | 性能优化 | 1 PR merged |
| 3 | PETSc/Trilinos | 功能开发 | 2 PR merged |
| 4 | Kokkos/RAJA | 核心贡献 | Contributor |

---

## 学习资源汇总

### 书籍清单

| Category | Book Name | Priority | Chapters |
|----------|-----------|----------|----------|
| C++ 基础 | 《C++ Primer 第5版》 | 高 | 全书 |
| C++ 进阶 | 《C++ 并发编程实战 第2版》 | 高 | 全书 |
| C++ 性能 | 《C++ 性能优化指南》 | 高 | 核心6章 |
| CUDA | 《CUDA C 编程权威指南》 | 高 | 全书 |
| MPI | 《MPI 高性能编程》 | 高 | 第1-12章 |
| OpenMP | 《OpenMP 编程指南》 | 高 | 核心8章 |
| 数值算法 | 《数值线性代数》 | 高 | 第1-8章 |
| 科学计算 | 《科学计算导论》 | 中 | 核心章节 |
| 体系结构 | 《计算机体系结构：量化研究方法》 | 高 | 第1-5章 |
| 并行算法 | 《并行算法导论》 | 中 | 核心章节 |

### 课程清单

| Platform | Course Name | Duration | Priority |
|----------|-------------|----------|----------|
| NVIDIA DLI | "Fundamentals of Accelerated Computing with CUDA C" | 8小时 | 高 |
| Coursera | "Parallel Programming with MPI" | 40小时 | 高 |
| MIT OpenCourse | "Performance Engineering of Software Systems" | 24小时 | 高 |
| Stanford Online | "Parallel Computing" | 40小时 | 中 |

### 论文清单

| Paper | Purpose |
|-------|---------|
| "The Roofline Model" | 性能分析方法 |
| "Optimization of Dense Matrix Multiplication" | 矩阵优化原理 |
| "FFTW: An Adaptive Software FFT Library" | 自调优技术 |
| "The CUDA Programming Model" | GPU 编程原理 |
| "MPI Performance Optimization" | MPI 优化方法 |

### 工具与库

| Type | Name | Purpose |
|------|------|---------|
| 性能分析 | perf/VTune/Nsight | 性能瓶颈诊断 |
| 数学库 | MKL/OpenBLAS/FFTW | 基准参考 |
| MPI 库 | OpenMPI/MPICH | 分布式并行 |
| CUDA 工具 | CUDA Toolkit/Nsight | GPU 开发 |
| 科学计算 | PETSc/Trilinos | 科学计算框架 |

---

## 进度追踪模板

详见 `references/roadmap-template.md` 中的"进度追踪模板"章节。

---

## Quick Start Guide（前3个月详细任务）

### Month 1: C++ 基础

**Week 1**:
- Day 1-2: 环境配置 + 编译工具链
- Day 3-4: 现代 C++ 特性基础
- Day 5-6: 智能指针与 RAII
- Day 7: 学习笔记整理

**Week 2**:
- Day 1-2: 右值引用与移动语义
- Day 3-4: STL 容器深入
- Day 5-6: 函数对象与 lambda
- Day 7: 练习巩固

**Week 3-4**: 模板与泛型编程

---

### Month 2: C++ 并发基础

**Week 1-2**: 多线程与同步原语
**Week 3-4**: 原子操作与无锁编程

---

### Month 3: 性能优化基础

**Week 1-2**: 性能分析工具链
**Week 3-4**: SIMD 与缓存优化

---

*此路线图为零基础开发者设计，4年达到 HPC 高性能计算专家水平。*