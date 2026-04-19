# 示例：Rust 数据库系统内核开发学习路线图

## 基本信息

- **总时长**: 4年（零基础到专家）
- **核心语言**: Rust → C（辅助）
- **目标定位**: 数据库内核开发工程师
- **适合人群**: 有编程基础，零数据库内核开发经验
- **日投入**: 4小时
- **重点领域**: 数据库存储引擎/SQL引擎开发

---

## 学习路线总览

```
Year 1: Rust 精通 + 数据库理论基础（初级数据库工程师）
Year 2: 存储引擎开发（中级数据库内核工程师）
Year 3: SQL 引擎开发（高级数据库内核工程师）
Year 4: 高级特性 + 开源贡献（数据库内核专家）
```

---

## Year 1：Rust 精通 + 数据库理论基础（第1-12个月）

### 阶段 1.1：Rust 语言精通（第1-6个月）

#### Week 1-2: Rust 基础语法

**Topic**: Rust 语言基础与内存安全模型

**Learning Actions**:
- Read: 《Rust 程序设计语言（第2版）》第1-10章
- Watch: Rust 官方教程 + Bilibili Rust 入门课程
- Practice: 编写基础数据结构实现
- Analyze: Rust 标准库源码结构

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-3 | 开发环境配置 + Cargo 基础 | 4h | 开发环境配置文档 |
| 4-7 | 所有权系统深入理解 | 8h | 所有权机制总结博客 |
| 8-10 | 引用与借用规则 | 6h | 借用规则示例项目 |
| 11-14 | 生命周期标注 | 6h | 生命周期使用指南 |

**Output Requirement**:
- Code: 实现动态数组(Vec)/字符串(String)/哈希表(HashMap)
- Documentation: Rust 所有权系统学习笔记（1篇博客）

**Validation**:
- Self-test: 解释所有权转移与借用的区别
- Practical: 编写无编译警告的数据结构库

---

#### Week 3-4: Rust 高级特性

**Topic**: Rust 类型系统与错误处理

**Learning Actions**:
- Read: 《Rust 程序设计语言（第2版）》第11-17章
- Practice: 泛型编程/trait系统实践
- Analyze: std::collections 源码实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-3 | 泛型与 trait 系统 | 6h | 泛型容器实现 |
| 4-7 | 错误处理(Result/Option) | 8h | 错误处理工具库 |
| 8-10 | 智能指针(Box/Rc/Arc) | 6h | 智能指针应用示例 |
| 11-14 | 闭包与迭代器 | 6h | 高性能迭代器实现 |

**Output Requirement**:
- Code: 实现自定义智能指针/错误处理框架
- Documentation: Rust 高级特性分析博客

**Validation**:
- Self-test: 解释 Box/Rc/Arc 的使用场景差异
- Practical: 实现泛型数据结构库

---

#### Week 5-8: 数据结构与算法实现

**Topic**: Rust 实现核心数据结构

**Learning Actions**:
- Read: 《Rust 数据结构与算法》全书
- Practice: 手写所有核心数据结构
- Analyze: Rust 标准库 collections 实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 链表/栈/队列实现 | 8h | 基础数据结构库 |
| 5-8 | 哈希表实现 | 8h | 高性能哈希表 |
| 9-12 | B+树实现 | 8h | B+树原型 |
| 13-16 | 红黑树实现 | 8h | 红黑树实现 |
| 17-20 | 跳表实现 | 8h | 跳表实现 |
| 21-28 | 算法综合实践 | 12h | 算法测试套件 |

**Output Requirement**:
- Code: 完整数据结构库（10+种）
- Documentation: 数据结构性能对比博客（Rust vs C）

**Validation**:
- Self-test: 分析 B+树插入/删除流程
- Practical: 数据结构通过性能基准测试

---

#### Week 9-12: Rust 异步编程与并发

**Topic**: Rust 异步编程与并发安全

**Learning Actions**:
- Read: 《Rust 异步编程》全书 + 《Rust 程序设计语言》第16-20章
- Practice: async/await编程实践
- Analyze: Tokio/async-std 源码实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | async/await 基础 | 8h | 异步编程示例 |
| 5-8 | Future trait 深入 | 8h | 自定义 Future |
| 9-12 | Tokio 运行时 | 8h | Tokio 应用实践 |
| 13-16 | 并发原语(mutex/rwlock) | 8h | 并发安全实现 |
| 17-20 | 无锁数据结构 | 8h | 无锁队列实现 |
| 21-28 | 异步综合实践 | 12h | 异步网络服务 |

**Output Requirement**:
- Code: 异步并发库（包含无锁数据结构）
- Documentation: Rust 异步编程深度分析博客（2篇）

**Validation**:
- Self-test: 解释 Send/Sync trait 的作用
- Practical: 实现高性能无锁队列（benchmark性能）

---

#### Week 13-16: Rust 工程实践

**Topic**: Rust 项目工程化与性能优化

**Learning Actions**:
- Read: 《Rust 编程实战》全书
- Practice: 大型 Rust 项目组织
- Analyze: TiKV/Rust 关系型数据库源码结构

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Cargo 工程管理 | 8h | 工程模板 |
| 5-8 | 单元测试/集成测试 | 8h | 测试框架 |
| 9-12 | 性能优化技巧 | 8h | 性能优化报告 |
| 13-16 | unsafe Rust 使用 | 8h | unsafe 使用指南 |
| 17-20 | FFI 与 C 交互 | 8h | FFI 示例 |
| 21-28 | 综合项目实践 | 12h | CLI 工具项目 |

**Output Requirement**:
- Code: 完整的 Rust 项目模板（包含测试/文档/性能）
- Documentation: Rust 工程化实践博客（2篇）

**Validation**:
- Self-test: 解释 unsafe Rust 的安全边界
- Practical: 项目通过 clippy 检查 + 性能基准测试

---

### 阶段 1.2：C 语言基础（第7-8个月）

#### C 语言辅助学习

| Week | Topic | Tasks | Output |
|------|-------|-------|--------|
| 1-2 | C 语言基础复习 | 《C Primer Plus》核心章节 | C 基础示例 |
| 3-4 | 指针与内存管理 | 指针深入/内存池实现 | 内存池实现 |
| 5-6 | Rust FFI 深入 | Rust 调用 C 库实践 | FFI 综合示例 |
| 7-8 | 数据库 C 代码分析 | 分析 PostgreSQL/MySQL C 代码 | 源码分析文档 |

**Milestone**: 能够使用 Rust 调用 C 编写的数据库底层库

---

### 阶段 1.3：数据库理论基础（第9-12个月）

#### Week 17-20: 数据库系统原理

**Topic**: 数据库系统核心概念

**Learning Actions**:
- Read: 《数据库系统概念（第6版）》第1-12章
- Watch: CMU 15-445 Database Systems 课程
- Practice: 设计简易数据库原型
- Analyze: SQLite 源码结构

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 关系模型与 SQL 基础 | 8h | SQL 基础总结 |
| 5-8 | 存储与索引结构 | 8h | 索引结构分析 |
| 9-12 | 查询处理基础 | 8h | 查询流程分析 |
| 13-16 | 数据库设计 | 8h | 数据库设计实践 |
| 17-20 | 综合理解 | 8h | 数据库架构图 |
| 21-28 | 原型设计 | 12h | 简易数据库设计文档 |

**Output Requirement**:
- Code: 简易数据库原型设计文档
- Documentation: 数据库系统原理学习笔记（2篇博客）

**Validation**:
- Self-test: 解释关系数据库的核心组件
- Practical: 设计一个支持基本 CRUD 的数据库架构

---

#### Week 21-24: 事务与并发控制

**Topic**: ACID 事务与并发控制机制

**Learning Actions**:
- Read: 《数据库系统概念》第14-16章 + 《事务处理：概念与技术》核心章节
- Practice: 实现事务管理原型
- Analyze: PostgreSQL 事务实现源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | ACID 特性深入 | 8h | ACID 分析文档 |
| 5-8 | 事务隔离级别 | 8h | 隔离级别对比 |
| 9-12 | 并发控制协议 | 8h | 并发控制原型 |
| 13-16 | MVCC 原理 | 8h | MVCC 分析博客 |
| 17-20 | 锁协议实现 | 8h | 锁管理器原型 |
| 21-28 | 事务综合实践 | 12h | 事务管理器模块 |

**Output Requirement**:
- Code: 简易事务管理器（支持隔离级别）
- Documentation: 事务与并发控制分析博客（2篇）

**Validation**:
- Self-test: 解释 MVCC 如何实现读不阻塞写
- Practical: 实现支持 4 种隔离级别的事务管理器

---

#### Week 25-28: 数据库恢复与日志系统

**Topic**: WAL 日志与故障恢复机制

**Learning Actions**:
- Read: 《数据库系统概念》第17章 + 《事务处理：概念与技术》日志章节
- Practice: 实现日志系统原型
- Analyze: MySQL InnoDB redo log 实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | WAL 日志原理 | 8h | WAL 分析文档 |
| 5-8 | 日志记录格式 | 8h | 日志格式设计 |
| 9-12 | 检查点机制 | 8h | 检查点实现 |
| 13-16 | ARIES 恢复算法 | 8h | ARIES 分析 |
| 17-20 | 恢复流程实现 | 8h | 恢复原型 |
| 21-28 | 日志综合实践 | 12h | 日志系统模块 |

**Output Requirement**:
- Code: WAL 日志系统原型（支持崩溃恢复）
- Documentation: 数据库恢复机制分析博客（2篇）

**Validation**:
- Self-test: 解释 WAL 如何保证持久性
- Practical: 实现支持崩溃恢复的日志系统

---

#### Week 29-32: 存储引擎架构

**Topic**: 存储引擎整体架构设计

**Learning Actions**:
- Read: 《数据库系统概念》存储章节 + InnoDB 架构文档
- Practice: 存储引擎原型设计
- Analyze: RocksDB/TiKV 存储引擎源码结构

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 存储引擎架构 | 8h | 架构设计文档 |
| 5-8 | Buffer Pool 管理 | 8h | Buffer Pool 原型 |
| 9-12 | 页面管理机制 | 8h | 页面管理器 |
| 13-16 | 索引组织方式 | 8h | 索引组织对比 |
| 17-20 | 存储格式设计 | 8h | 存储格式规范 |
| 21-28 | 存储引擎原型 | 12h | 存储引擎原型 |

**Output Requirement**:
- Code: 存储引擎架构原型（Buffer Pool + 页面管理）
- Documentation: 存储引擎架构分析博客（2篇）

**Validation**:
- Self-test: 解释 Buffer Pool 的页面替换策略
- Practical: 实现支持 LRU 替换的 Buffer Pool

---

## Year 2：存储引擎开发（第13-24个月）

### 阶段 2.1：KV 存储引擎开发（第13-18个月）

#### Week 33-36: KV 存储引擎基础

**Topic**: 键值存储引擎设计与实现

**Learning Actions**:
- Read: 《数据库系统概念》存储章节 + LevelDB 设计文档
- Practice: 实现基础 KV 存储引擎
- Analyze: LevelDB/RocksDB 源码实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | KV 存储架构设计 | 8h | 架构设计文档 |
| 5-8 | 内存表(MemTable)实现 | 8h | MemTable 模块 |
| 9-12 | SSTable 文件格式 | 8h | SSTable 格式设计 |
| 13-16 | 持久化存储实现 | 8h | 文件存储模块 |
| 17-20 | 基础读写接口 | 8h | KV 接口实现 |
| 21-28 | KV 存储原型 | 12h | 基础 KV 存储引擎 |

**Output Requirement**:
- Code: 基础 KV 存储引擎（支持 Put/Get/Delete）
- Documentation: KV 存储引擎设计博客

**Validation**:
- Self-test: 解释 MemTable 与 SSTable 的关系
- Practical: KV 存储引擎通过基础功能测试

---

#### Week 37-40: KV 存储引擎事务支持

**Topic**: KV 存储引擎事务功能实现

**Learning Actions**:
- Read: RocksDB 事务文档 + TiKV 事务模型文档
- Practice: 实现事务型 KV 存储引擎
- Analyze: TiKV 事务实现源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 事务 Begin/Commit/Rollback | 8h | 事务接口实现 |
| 5-8 | MVCC 实现 | 8h | MVCC 存储层 |
| 9-12 | 事务锁管理 | 8h | 锁管理器 |
| 13-16 | 事务隔离级别 | 8h | 隔离级别实现 |
| 17-20 | WAL 日志集成 | 8h | 日志系统集成 |
| 21-28 | 事务 KV 引擎 | 12h | 事务型 KV 引擎 |

**Output Requirement**:
- Code: 事务型 KV 存储引擎（支持事务/隔离级别）
- Documentation: KV 事务实现分析博客

**Validation**:
- Self-test: 解释 KV 存储如何实现 MVCC
- Practical: 事务引擎通过并发测试（TPCC 基础测试）

---

#### Week 41-44: KV 存储引擎性能优化

**Topic**: KV 存储引擎性能调优

**Learning Actions**:
- Read: RocksDB 性能调优文档 + TiKV 性能优化文档
- Practice: 性能分析与优化
- Analyze: RocksDB 性能优化实践

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 内存表优化 | 8h | 高性能 MemTable |
| 5-8 | SSTable 压缩 | 8h | 压缩算法集成 |
| 9-12 | Block Cache 实现 | 8h | Block Cache |
| 13-16 | Bloom Filter 优化 | 8h | Bloom Filter |
| 17-20 | Compaction 优化 | 8h | Compaction 算法 |
| 21-28 | 性能基准测试 | 12h | 性能测试报告 |

**Output Requirement**:
- Code: 高性能 KV 存储引擎
- Documentation: KV 性能优化分析博客

**Validation**:
- Self-test: 解释 Bloom Filter 如何加速查询
- Practical: KV 引擎随机读写性能 > 50,000 ops/sec

---

### 阶段 2.2：LSM-Tree 存储引擎开发（第19-24个月）

#### Week 45-48: LSM-Tree 原理与设计

**Topic**: LSM-Tree 存储引擎核心原理

**Learning Actions**:
- Read: LSM-Tree 原始论文 + RocksDB 设计文档
- Practice: LSM-Tree 结构设计
- Analyze: RocksDB LSM-Tree 实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | LSM-Tree 原理深入 | 8h | LSM-Tree 分析博客 |
| 5-8 | Level 结构设计 | 8h | Level 结构实现 |
| 9-12 | MemTable 设计 | 8h | MemTable 实现 |
| 13-16 | Immutable MemTable | 8h | Immutable MemTable |
| 17-20 | SSTable 文件组织 | 8h | SSTable 组织 |
| 21-28 | LSM-Tree 原型 | 12h | LSM-Tree 基础原型 |

**Output Requirement**:
- Code: LSM-Tree 基础结构实现
- Documentation: LSM-Tree 原理深度分析博客（2篇）

**Validation**:
- Self-test: 解释 LSM-Tree 的写入优化原理
- Practical: LSM-Tree 基础原型通过写入测试

---

#### Week 49-52: Compaction 机制实现

**Topic**: LSM-Tree Compaction 算法实现

**Learning Actions**:
- Read: RocksDB Compaction 文档 + LevelDB Compaction 源码
- Practice: Compaction 算法实现
- Analyze: RocksDB Compaction 策略

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Compaction 触发策略 | 8h | 触发策略实现 |
| 5-8 | Leveled Compaction | 8h | Leveled Compaction |
| 9-12 | Tiered Compaction | 8h | Tiered Compaction |
| 13-16 | Compaction 调度 | 8h | Compaction 调度器 |
| 17-20 | Compaction 优化 | 8h | Compaction 优化 |
| 21-28 | Compaction 综合实践 | 12h | Compaction 模块 |

**Output Requirement**:
- Code: Compaction 算法实现（支持多种策略）
- Documentation: Compaction 算法分析博客

**Validation**:
- Self-test: 解释 Leveled 与 Tiered Compaction 的差异
- Practical: Compaction 通过性能测试（写入吞吐量）

---

#### Week 53-56: LSM-Tree 高级特性

**Topic**: LSM-Tree 高级功能实现

**Learning Actions**:
- Read: RocksDB 高级特性文档 + TiKV 存储文档
- Practice: LSM-Tree 高级特性实现
- Analyze: RocksDB 高级功能源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Range Scan 优化 | 8h | Range Scan 实现 |
| 5-8 | Snapshot 功能 | 8h | Snapshot 实现 |
| 9-12 |迭代器实现 | 8h | Iterator 实现 |
| 13-16 | TTL 支持 | 8h | TTL 实现 |
| 17-20 | Column Family | 8h | Column Family 实现 |
| 21-28 | LSM-Tree 综合实践 | 12h | 功能完整 LSM-Tree |

**Output Requirement**:
- Code: 功能完整的 LSM-Tree 存储引擎
- Documentation: LSM-Tree 高级特性博客（2篇）

**Validation**:
- Self-test: 解释 Snapshot 如何实现 MVCC 读
- Practical: LSM-Tree 引擎写入吞吐量 > 100,000 ops/sec

---

#### Week 57-60: LSM-Tree 性能基准测试

**Topic**: LSM-Tree 存储引擎性能验证

**Learning Actions**:
- Read: YCSB 基准测试文档 + RocksDB 性能测试文档
- Practice: LSM-Tree 性能测试
- Analyze: RocksDB 性能基准

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | YCSB 基准测试集成 | 8h | YCSB 测试框架 |
| 5-8 | 随机读写测试 | 8h | 随机读写性能 |
| 9-12 | 范围扫描测试 | 8h | Range Scan 性能 |
| 13-16 | 压缩率测试 | 8h | 压缩性能 |
| 17-20 | 并发性能测试 | 8h | 并发性能测试 |
| 21-28 | 性能优化迭代 | 12h | 性能优化报告 |

**Output Requirement**:
- Code: LSM-Tree 存储引擎 + 性能测试框架
- Documentation: LSM-Tree 性能测试报告博客

**Validation**:
- Self-test: 分析 LSM-Tree 的性能瓶颈
- Practical: LSM-Tree 性能指标达到预设目标

---

## Year 3：SQL 引擎开发（第25-36个月）

### 阶段 3.1：SQL 解析器开发（第25-30个月）

#### Week 61-64: 词法分析器实现

**Topic**: SQL 词法分析器设计与实现

**Learning Actions**:
- Read: 《编译原理》词法分析章节 + SQLite 词法分析源码
- Practice: 实现 SQL 词法分析器
- Analyze: MySQL/SQLite 词法分析器源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | SQL 关键字定义 | 8h | 关键字表 |
| 5-8 | Token 类型设计 | 8h | Token 类型系统 |
| 9-12 | 词法分析器实现 | 8h | Lexer 实现 |
| 13-16 | 错误处理机制 | 8h | 词法错误处理 |
| 17-20 | Lexer 优化 | 8h | Lexer 性能优化 |
| 21-28 | Lexer 综合实践 | 12h | 功能完整 Lexer |

**Output Requirement**:
- Code: SQL 词法分析器（支持标准 SQL 关键字）
- Documentation: 词法分析器实现博客

**Validation**:
- Self-test: 解释词法分析的 Token 流
- Practical: Lexer 通过 SQL 语句解析测试

---

#### Week 65-68: 语法分析器实现

**Topic**: SQL 语法分析器设计与实现

**Learning Actions**:
- Read: 《编译原理》语法分析章节 + 《SQL 语言艺术》
- Practice: 实现 SQL 语法分析器
- Analyze: MySQL/SQLite 语法分析器源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | SQL 语法规则设计 | 8h | 语法规则文档 |
| 5-8 | AST 结构设计 | 8h | AST 类型系统 |
| 9-12 | Parser 实现（SELECT） | 8h | SELECT Parser |
| 13-16 | Parser 实现（INSERT/UPDATE/DELETE） | 8h | DML Parser |
| 17-20 | Parser 实现（DDL） | 8h | DDL Parser |
| 21-28 | Parser 综合实践 | 12h | 功能完整 Parser |

**Output Requirement**:
- Code: SQL 语法分析器（生成 AST）
- Documentation: 语法分析器实现博客（2篇）

**Validation**:
- Self-test: 解释 AST 的结构设计
- Practical: Parser 通过复杂 SQL 语句解析测试

---

#### Week 69-72: SQL 解析器优化

**Topic**: SQL 解析器性能与扩展性优化

**Learning Actions**:
- Read: 《编译原理》优化章节 + PostgreSQL Parser 源码
- Practice: SQL 解析器性能优化
- Analyze: PostgreSQL SQL 解析器实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Parser 性能分析 | 8h | 性能分析报告 |
| 5-8 | AST 优化 | 8h | AST 优化 |
| 9-12 | 错误恢复机制 | 8h | 错误恢复实现 |
| 13-16 | SQL 扩展支持 | 8h | SQL 扩展实现 |
| 17-20 | Parser 优化 | 8h | Parser 性能优化 |
| 21-28 | Parser 综合实践 | 12h | 高性能 Parser |

**Output Requirement**:
- Code: 高性能 SQL 解析器
- Documentation: SQL 解析器优化博客

**Validation**:
- Self-test: 分析 Parser 的性能瓶颈
- Practical: Parser 解析性能 > 10,000 SQL/sec

---

### 阶段 3.2：查询执行器开发（第31-36个月）

#### Week 73-76: 查询执行器基础

**Topic**: 查询执行引擎设计与实现

**Learning Actions**:
- Read: 《数据库系统概念》查询处理章节 + CMU 15-445 课程
- Practice: 实现基础查询执行器
- Analyze: MySQL/SQLite 查询执行器源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 执行器架构设计 | 8h | 执行器架构文档 |
| 5-8 | 执行计划结构 | 8h | ExecutionPlan 实现 |
| 9-12 | 算子接口设计 | 8h | Operator 接口 |
| 13-16 | 基础算子实现 | 8h | 基础算子（Scan/Filter） |
| 17-20 | 表达式计算 | 8h | Expression Evaluator |
| 21-28 | 执行器原型 | 12h | 基础执行器原型 |

**Output Requirement**:
- Code: 基础查询执行器（支持简单 SELECT）
- Documentation: 查询执行器设计博客

**Validation**:
- Self-test: 解释火山模型的执行流程
- Practical: 执行器通过简单查询测试

---

#### Week 77-80: 火山模型执行器

**Topic**: 火山模型（Iterator Model）查询执行器

**Learning Actions**:
- Read: 《数据库系统概念》查询执行章节 + PostgreSQL 执行器源码
- Practice: 实现火山模型执行器
- Analyze: PostgreSQL 执行器实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Scan Operator | 8h | Scan 算子实现 |
| 5-8 | Filter Operator | 8h | Filter 算子实现 |
| 9-12 | Project Operator | 8h | Project 算子实现 |
| 13-16 | Join Operator | 8h | Join 算子实现 |
| 17-20 | Aggregation Operator | 8h | Agg 算子实现 |
| 21-28 | 火山模型综合实践 | 12h | 火山模型执行器 |

**Output Requirement**:
- Code: 火山模型查询执行器（支持基本 SQL 操作）
- Documentation: 火山模型执行器分析博客（2篇）

**Validation**:
- Self-test: 解释火山模型的 next() 调用链
- Practical: 火山模型执行器通过 TPCH Q1 测试

---

#### Week 81-84: 向量化执行器

**Topic**: 向量化查询执行器实现

**Learning Actions**:
- Read: MonetDB 向量化论文 + DuckDB 向量化执行器源码
- Practice: 实现向量化查询执行器
- Analyze: DuckDB/Velox 向量化执行器实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 向量化原理深入 | 8h | 向量化分析博客 |
| 5-8 | Columnar 数据结构 | 8h | 列式数据结构 |
| 9-12 | Vectorized Operator | 8h | 向量化算子实现 |
| 13-16 | Batch Processing | 8h | Batch 处理实现 |
| 17-20 | SIMD 优化 | 8h | SIMD 指令优化 |
| 21-28 | 向量化综合实践 | 12h | 向量化执行器 |

**Output Requirement**:
- Code: 向量化查询执行器
- Documentation: 向量化执行器深度分析博客（2篇）

**Validation**:
- Self-test: 解释向量化执行的性能优势
- Practical: 向量化执行器性能比火山模型提升 3x+

---

#### Week 85-88: 查询优化器基础

**Topic**: 查询优化器基础实现

**Learning Actions**:
- Read: 《数据库系统概念》查询优化章节 + 《数据库查询优化器的艺术》
- Practice: 实现基础查询优化器
- Analyze: MySQL/PostgreSQL 查询优化器源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 优化器架构设计 | 8h | 优化器架构文档 |
| 5-8 | 统计信息收集 | 8h | Statistics Collector |
| 9-12 | 代价模型设计 | 8h | Cost Model 实现 |
| 13-16 | 规则优化实现 | 8h | Rule-based Optimizer |
| 17-20 | 代价优化实现 | 8h | Cost-based Optimizer |
| 21-28 | 优化器综合实践 | 12h | 基础查询优化器 |

**Output Requirement**:
- Code: 基础查询优化器（支持规则+代价优化）
- Documentation: 查询优化器设计博客（2篇）

**Validation**:
- Self-test: 解释规则优化与代价优化的差异
- Practical: 优化器生成高效执行计划

---

## Year 4：高级特性 + 开源贡献（第37-48个月）

### 阶段 4.1：分布式数据库基础（第37-40个月）

#### Week 89-92: 分布式存储原理

**Topic**: 分布式数据库存储架构

**Learning Actions**:
- Read: 《分布式数据库系统原理》核心章节 + TiDB 架构文档
- Practice: 分布式存储原型设计
- Analyze: TiKV/CockroachDB 分布式存储实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 分布式存储架构 | 8h | 分布式架构文档 |
| 5-8 | 数据分片策略 | 8h | Sharding 实现 |
| 9-12 | 分布式一致性 | 8h | Consistency 实现 |
| 13-16 | 分布式事务 | 8h | 分布式事务原型 |
| 17-20 | 副本管理 | 8h | Replica 管理 |
| 21-28 | 分布式存储原型 | 12h | 分布式存储原型 |

**Output Requirement**:
- Code: 分布式存储原型（支持分片+副本）
- Documentation: 分布式存储架构分析博客（2篇）

**Validation**:
- Self-test: 解释分布式事务的 2PC 流程
- Practical: 分布式存储原型通过基础测试

---

#### Week 93-96: 分布式共识算法

**Topic**: Raft/Paxos 分布式共识算法

**Learning Actions**:
- Read: Raft 论文 + Paxos 论文 + TiKV Raft 实现文档
- Practice: 实现 Raft 协议
- Analyze: TiKV Raft 源码实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Raft 协议原理深入 | 8h | Raft 分析博客 |
| 5-8 | Leader Election | 8h | Leader 选举实现 |
| 9-12 | Log Replication | 8h | 日志复制实现 |
| 13-16 | Safety 保证 | 8h | Safety 实现 |
| 17-20 | Raft 优化 | 8h | Raft 优化实践 |
| 21-28 | Raft 综合实践 | 12h | Raft 协议实现 |

**Output Requirement**:
- Code: Raft 协议实现
- Documentation: Raft 协议深度分析博客（2篇）

**Validation**:
- Self-test: 解释 Raft 的 Leader 选举流程
- Practical: Raft 实现通过 Jepsen 测试

---

### 阶段 4.2：高级数据库特性（第41-44个月）

#### Week 97-100: 高级索引实现

**Topic**: 高级索引结构与算法

**Learning Actions**:
- Read: 《数据库系统概念》索引章节 + 高级索引论文
- Practice: 实现高级索引结构
- Analyze: PostgreSQL 高级索引实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Adaptive Hash Index | 8h | AHI 实现 |
| 5-8 | 布隆过滤器索引 | 8h | Bloom Filter Index |
| 9-12 | 倒排索引 | 8h | Inverted Index |
| 13-16 | 空间索引(R-Tree) | 8h | R-Tree 实现 |
| 17-20 | 全文索引 | 8h | Full-Text Index |
| 21-28 | 高级索引综合实践 | 12h | 高级索引模块 |

**Output Requirement**:
- Code: 高级索引实现（支持多种索引类型）
- Documentation: 高级索引分析博客（2篇）

**Validation**:
- Self-test: 解释 Adaptive Hash Index 的自适应策略
- Practical: 高级索引通过性能测试

---

#### Week 101-104: 列式存储引擎

**Topic**: 列式存储引擎设计与实现

**Learning Actions**:
- Read: 列式存储论文 + ClickHouse 架构文档
- Practice: 实现列式存储引擎
- Analyze: ClickHouse/Parquet 列式存储实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 列式存储原理 | 8h | 列式存储分析 |
| 5-8 | Column Chunk 实现 | 8h | Column Chunk |
| 9-12 | 列式压缩 | 8h | Column Compression |
| 13-16 | 列式执行集成 | 8h | Columnar Execution |
| 17-20 | 列式优化 | 8h | 列式优化实践 |
| 21-28 | 列式存储引擎 | 12h | 列式存储引擎原型 |

**Output Requirement**:
- Code: 列式存储引擎原型
- Documentation: 列式存储引擎分析博客（2篇）

**Validation**:
- Self-test: 解释列式存储的压缩优势
- Practical: 列式存储引擎通过分析查询测试

---

### 阶段 4.3：开源数据库贡献（第45-48个月）

#### Week 105-108: 开源数据库源码分析

**Topic**: 开源数据库深度源码分析

**Learning Actions**:
- Read: TiDB/Rust 数据库源码文档
- Practice: 深度分析开源数据库源码
- Analyze: TiDB/TiKV/RisingWave 源码实现

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | TiDB 源码结构分析 | 8h | TiDB 源码分析文档 |
| 5-8 | TiKV 源码深度分析 | 8h | TiKV 源码分析博客 |
| 9-12 | RisingWave 源码分析 | 8h | RisingWave 源码分析 |
| 13-16 | DuckDB 源码分析 | 8h | DuckDB 源码分析 |
| 17-20 | 数据库对比分析 | 8h | 数据库对比报告 |
| 21-28 | 源码分析综合实践 | 12h | 深度源码分析文档 |

**Output Requirement**:
- Code: 源码分析工具/文档
- Documentation: 开源数据库源码深度分析博客（3篇）

**Validation**:
- Self-test: 解释 TiDB 的整体架构设计
- Practical: 源码分析文档通过社区审核

---

#### Week 109-112: 开源数据库贡献实践

**Topic**: 成为开源数据库贡献者

**Learning Actions**:
- Read: TiDB/Rust 数据库贡献指南
- Practice: 提交开源数据库补丁
- Analyze: 开源数据库社区流程

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 开源社区参与学习 | 8h | 社区参与文档 |
| 5-8 | Issue 分析与修复 | 8h | Bug 修复补丁 |
| 9-12 | 功能开发实践 | 8h | 功能补丁实现 |
| 13-16 | 性能优化贡献 | 8h | 性能优化补丁 |
| 17-20 | 文档贡献 | 8h | 文档补丁 |
| 21-28 | 开源贡献综合实践 | 12h | 多个补丁提交 |

**Output Requirement**:
- Code: 提交到开源数据库的补丁
- Documentation: 开源贡献指南博客

**Validation**:
- Self-test: 理解开源数据库贡献流程
- Practical: 补丁被开源数据库社区接受

---

#### Week 113-116: 数据库内核专家能力验证

**Topic**: 数据库内核专家能力验证

**Learning Actions**:
- Read: 数据库领域前沿论文
- Practice: 专家级数据库内核开发
- Analyze: 数据库领域最新发展

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 前沿技术跟踪 | 8h | 前沿技术报告 |
| 5-8 | 专家级项目实现 | 8h | 专家级项目 |
| 9-12 | 性能极限优化 | 8h | 性能极限优化 |
| 13-16 | 技术分享 | 8h | 技术演讲 |
| 17-20 | 专家能力验证 | 8h | 专家能力验证 |
| 21-28 | 专家级综合实践 | 12h | 专家级贡献 |

**Output Requirement**:
- Code: 专家级数据库内核模块
- Documentation: 专家级技术分析博客/演讲

**Validation**:
- Self-test: 成为数据库内核领域专家
- Practical: 专家级贡献被社区认可

---

## 关键项目清单

| Year | Project | Key Tech | Performance Target | Duration |
|------|---------|----------|-------------------|----------|
| 1 | Rust 数据结构库 | 所有权/泛型 | 无编译警告 | 8周 |
| 1 | 异步并发库 | async/await/Send/Sync | 性能基准测试 | 8周 |
| 1 | 简易数据库原型设计 | 数据库理论基础 | 架构设计文档 | 12周 |
| 2 | KV 存储引擎 | MemTable/SSTable/事务 | 50,000 ops/sec | 16周 |
| 2 | LSM-Tree 存储引擎 | LSM-Tree/Compaction | 100,000 ops/sec | 16周 |
| 3 | SQL 解析器 | 词法分析/语法分析/AST | 10,000 SQL/sec | 12周 |
| 3 | 火山模型执行器 | 火山模型/算子 | TPCH Q1测试 | 12周 |
| 3 | 向量化执行器 | 向量化/SIMD | 性能提升3x+ | 12周 |
| 4 | Raft 协议实现 | Raft/分布式一致性 | Jepsen测试 | 8周 |
| 4 | 列式存储引擎 | 列式存储/压缩 | 分析查询优化 | 8周 |
| 4 | 开源数据库贡献 | TiDB/TiKV/RisingWave | 补丁被接受 | 12周 |

---

## 里程碑检验标准

| Milestone | Knowledge Check | Project Completion | External Validation |
|-----------|-----------------|-------------------|-------------------|
| Year 1 Q2 | Rust 所有权/借用 | 数据结构库 | 无编译警告测试 |
| Year 1 Q4 | 数据库理论基础 | 简易数据库设计文档 | 架构设计验证 |
| Year 2 Q2 | KV 存储原理 | KV 存储引擎原型 | 基础功能测试 |
| Year 2 Q4 | LSM-Tree/Compaction | LSM-Tree 存储引擎 | 100,000 ops/sec |
| Year 3 Q2 | SQL 解析原理 | SQL 解析器 | 10,000 SQL/sec |
| Year 3 Q4 | 查询执行/优化 | 向量化执行器 | 性能提升3x+ |
| Year 4 Q2 | 分布式数据库 | Raft 协议实现 | Jepsen测试通过 |
| Year 4 Q4 | 数据库内核专家能力 | 开源数据库贡献 | 补丁被接受 |

---

## 开源贡献路径

| Year | Target Project | Contribution Type | Goal |
|------|----------------|-------------------|------|
| 2 | TiKV 文档 | 文档翻译/修正 | 1 PR merged |
| 3 | TiKV/RisingWave | Bug修复/小功能 | 2 PR merged |
| 4 | TiDB/TiKV/RisingWave | 性能优化/功能开发 | Contributor |

---

## 学习资源汇总

### 书籍清单

| Category | Book Name | Priority | Chapters |
|----------|-----------|----------|----------|
| Rust 基础 | 《Rust 程序设计语言（第2版）》 | 高 | 全书 |
| Rust 高级 | 《Rust 异步编程》 | 高 | 全书 |
| Rust 数据结构 | 《Rust 数据结构与算法》 | 中 | 全书 |
| Rust 实战 | 《Rust 编程实战》 | 中 | 全书 |
| C 语言 | 《C Primer Plus（第6版）》 | 中 | 核心章节 |
| 数据库原理 | 《数据库系统概念（第6版）》 | 高 | 第1-17章 |
| 事务处理 | 《事务处理：概念与技术》 | 高 | 核心章节 |
| SQL 语言 | 《SQL 语言艺术》 | 中 | 全书 |
| 编译原理 | 《编译原理》 | 中 | 词法/语法分析章节 |
| 分布式系统 | 《分布式数据库系统原理》 | 高 | 核心章节 |
| 查询优化 | 《数据库查询优化器的艺术》 | 高 | 全书 |

### 课程清单

| Platform | Course Name | Duration | Priority |
|----------|-------------|----------|----------|
| CMU | 15-445 Database Systems | 16周 | 高 |
| CMU | 15-721 Advanced Database Systems | 16周 | 高 |
| Stanford | CS346 Data Systems Seminar | 12周 | 中 |
| MIT | 6.824 Distributed Systems | 16周 | 高 |
| Coursera | Rust Programming | 8周 | 中 |

### 文档/源码

| Type | Name | Link | Purpose |
|------|------|------|---------|
| 官方文档 | Rust 官方文档 | https://doc.rust-lang.org/ | Rust 学习 |
| 源码分析 | TiKV 源码 | https://github.com/tikv/tikv | 分布式 KV 存储学习 |
| 源码分析 | TiDB 源码 | https://github.com/pingcap/tidb | 分布式数据库学习 |
| 源码分析 | RisingWave 源码 | https://github.com/risingwavelabs/risingwave | 流式数据库学习 |
| 源码分析 | DuckDB 源码 | https://github.com/duckdb/duckdb | 向量化执行器学习 |
| 源码分析 | RocksDB 源码 | https://github.com/facebook/rocksdb | LSM-Tree 存储学习 |
| 论文 | LSM-Tree 论文 | ACM Digital Library | LSM-Tree 原理学习 |
| 论文 | Raft 论文 | https://raft.github.io/ | 分布式共识学习 |

---

## 性能基准目标

| Component | Metric | Target | Benchmark Tool |
|-----------|--------|--------|----------------|
| KV 存储引擎 | 随机读写吞吐量 | > 50,000 ops/sec | YCSB |
| LSM-Tree 引擎 | 写入吞吐量 | > 100,000 ops/sec | YCSB |
| SQL 解析器 | 解析吞吐量 | > 10,000 SQL/sec | 自定义测试 |
| 火山模型执行器 | TPCH Q1 性能 | 基准测试通过 | TPCH |
| 向量化执行器 | 性能提升倍数 | > 3x 火山模型 | TPCH |
| Raft 协议 | 故障恢复时间 | < 5秒 | Jepsen |
| 列式存储引擎 | 压缩率 | > 5x 行式存储 | 自定义测试 |

---

## 进度追踪模板

详见 `references/roadmap-template.md` 中的"进度追踪模板"章节。

---

## Quick Start Guide（前3个月详细任务）

### Month 1: Rust 基础语法

**Week 1**:
- Day 1-2: 开发环境配置 + Cargo 基础
- Day 3-4: 所有权系统基础
- Day 5-6: 引用与借用规则
- Day 7: 学习笔记整理

**Week 2**:
- Day 1-2: 生命周期标注深入
- Day 3-4: 结构体/枚举/匹配
- Day 5-6: 错误处理基础
- Day 7: 练习巩固

**Week 3-4**: 泛型/trait/智能指针基础

---

### Month 2: Rust 高级特性

**Week 1-2**: 闭包/迭代器/模块系统
**Week 3-4**: 异步编程基础/并发安全

---

### Month 3: 数据结构实现

**Week 1-2**: 链表/栈/队列/哈希表
**Week 3-4**: B+树/红黑树/跳表

---

*此路线图为零数据库内核开发经验开发者设计，4年达到数据库内核开发专家水平。*