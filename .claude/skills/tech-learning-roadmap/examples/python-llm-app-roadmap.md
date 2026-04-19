# Python AI/ML大模型应用开发学习路线图

## 基本信息

- **总时长**: 3年（中级到专家）
- **核心语言**: Python
- **目标定位**: LLM应用开发工程师
- **适合人群**: 有Python基础，零AI/ML应用开发经验
- **日投入**: 4小时
- **重点领域**: 大模型应用开发、RAG系统、AI Agent、模型微调与部署

---

## 学习路线总览

```
Year 1: Python精通 + 机器学习基础（初级LLM应用工程师）
Year 2: 深度学习 + 大模型API应用（中级LLM应用工程师）
Year 3: 大模型应用架构 + 生产部署（高级LLM应用工程师）
```

---

## Year 1：Python精通 + 机器学习基础（第1-12个月）

### 阶段 1.1：Python高级特性与工程实践（第1-4个月）

#### Week 1-2: Python类型系统与现代特性

**Topic**: Python类型系统与代码质量

**Learning Actions**:
- Read: 《Python编程：从入门到实践（第3版）》第9-12章
- Watch: Python类型系统最佳实践视频教程
- Practice: 类型注解与静态检查实践
- Analyze: 标准库typing模块源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-2 | 类型注解基础/Type Hints | 4h | 类型注解示例库 |
| 3-4 | typing模块深入/泛型类型 | 6h | 泛型工具库 |
| 5-6 | 静态类型检查/mypy/pyright | 4h | 类型检查配置 |
| 7-8 | dataclasses/Pydantic | 6h | 数据模型库 |

**Output Requirement**:
- Code: 类型系统工具库（30+类型示例）
- Documentation: Python类型系统学习笔记（1篇博客）

**Validation**:
- Self-test: 解释Type Hints运行时行为
- Code review: mypy严格模式零警告

---

#### Week 3-4: 异步编程与并发模型

**Topic**: Python异步编程与并发处理

**Learning Actions**:
- Read: 《Python并发编程实战》第1-8章
- Practice: asyncio/协程/异步IO
- Analyze: aiohttp/httpx源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-3 | asyncio基础/协程原理 | 6h | 异步基础示例 |
| 4-6 | async/await/事件循环 | 6h | 异步任务调度器 |
| 7-9 | 异步IO/aiofiles/aiohttp | 8h | 异步HTTP客户端 |
| 10-12 | 并发控制/信号量/锁 | 6h | 并发控制工具 |

**Output Requirement**:
- Code: 异步任务调度系统
- Documentation: 异步编程分析博客（1篇）

**Validation**:
- Self-test: 解释asyncio事件循环工作原理
- Practical: 异步HTTP客户端支持100+并发请求

---

#### Week 5-8: Python工程化实践

**Topic**: Python项目工程化与最佳实践

**Learning Actions**:
- Read: 《Python工程化实践》核心章节
- Practice: 项目结构/测试/CI/CD
- Analyze: 开源项目工程结构

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 项目结构/pyproject.toml | 6h | 项目模板 |
| 5-8 | 单元测试/pytest/mock | 8h | 测试框架集成 |
| 9-12 | 代码质量/linting/formatting | 6h | 代码质量工具链 |
| 13-16 | CI/CD/GitHub Actions | 8h | 自动化流水线 |
| 17-20 | 文档生成/Sphinx/MkDocs | 6h | 文档系统 |
| 21-28 | 性能分析/cProfile/line_profiler | 8h | 性能分析工具 |

**Output Requirement**:
- Code: Python项目工程模板
- Documentation: 工程化实践博客（2篇）

**Validation**:
- Self-test: 解释pytest fixture工作机制
- Practical: 项目CI/CD流水线自动测试

---

### 阶段 1.2：机器学习基础（第5-8个月）

#### Week 1-4: 数据科学与数值计算基础

**Topic**: NumPy/Pandas数据处理

**Learning Actions**:
- Read: 《Python数据科学手册》第1-5章
- Practice: NumPy/Pandas核心操作
- Analyze: pandas核心数据结构

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | NumPy数组/矩阵运算 | 8h | 数值计算库 |
| 5-8 | Pandas数据结构/操作 | 8h | 数据处理工具 |
| 9-12 | 数据清洗/转换/聚合 | 8h | 数据清洗脚本 |
| 13-16 | 数据可视化/matplotlib/seaborn | 8h | 可视化工具 |
| 17-28 | 数据分析项目 | 12h | 数据分析报告 |

**Output Requirement**:
- Code: 数据处理工具库
- Documentation: 数据分析博客（1篇）

**Validation**:
- Self-test: 解释NumPy广播机制
- Practical: 完成数据清洗与分析项目

---

#### Week 5-8: 机器学习算法基础

**Topic**: Scikit-learn与经典ML算法

**Learning Actions**:
- Read: 《机器学习实战》第1-8章
- Watch: 机器学习算法视频课程
- Practice: Scikit-learn算法实践
- Analyze: sklearn源码关键模块

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 监督学习/回归/分类 | 8h | 回归分类模型 |
| 5-8 | 无监督学习/聚类/降维 | 8h | 聚类降维工具 |
| 9-12 | 特征工程/预处理 | 8h | 特征工程库 |
| 13-16 | 模型评估/交叉验证 | 6h | 模型评估框架 |
| 17-20 | 模型选择/调优 | 8h | 调优工具 |
| 21-28 | ML项目实战 | 12h | 完整ML项目 |

**Output Requirement**:
- Code: ML工具库（10+算法实现）
- Documentation: ML算法分析博客（2篇）

**Validation**:
- Self-test: 解释交叉验证避免过拟合原理
- Practical: Kaggle入门竞赛Top 50%

---

### 阶段 1.3：深度学习入门（第9-12个月）

#### Week 1-4: PyTorch基础

**Topic**: PyTorch核心概念与API

**Learning Actions**:
- Read: 《动手学深度学习》PyTorch版第1-5章
- Watch: PyTorch官方教程
- Practice: 张量操作/自动求导
- Analyze: PyTorch核心源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 张量操作/设备管理 | 6h | 张量工具库 |
| 5-8 | 自动求导/梯度计算 | 8h | 自动求导示例 |
| 9-12 | 神经网络模块/nn.Module | 8h | 网络模块库 |
| 13-16 | 优化器/损失函数 | 6h | 优化工具 |
| 17-28 | 模型训练流程 | 12h | 训练框架 |

**Output Requirement**:
- Code: PyTorch训练框架
- Documentation: PyTorch学习笔记（1篇）

**Validation**:
- Self-test: 解释反向传播计算图构建
- Practical: MNIST分类模型达到99%精度

---

#### Week 5-8: 经典神经网络模型

**Topic**: CNN/RNN基础模型

**Learning Actions**:
- Read: 《动手学深度学习》第6-9章
- Practice: CNN/RNN模型实现
- Analyze: ResNet/LSTM经典架构

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 卷积神经网络/CNN | 8h | CNN模型库 |
| 5-8 | 循环神经网络/RNN/LSTM | 8h | RNN模型库 |
| 9-12 | 图像分类实战/CIFAR10 | 8h | 图像分类项目 |
| 13-16 | 序列建模实战 | 8h | 序列模型项目 |
| 17-28 | 模型优化技巧 | 12h | 优化版模型 |

**Output Requirement**:
- Code: 经典模型库（CNN/RNN）
- Documentation: 模型架构分析博客（2篇）

**Validation**:
- Self-test: 解释CNN卷积核工作机制
- Practical: CIFAR10分类达到90%精度

---

## Year 2：深度学习 + 大模型API应用（第13-24个月）

### 阶段 2.1：Transformer与注意力机制（第13-16个月）

#### Week 1-4: Transformer架构深入

**Topic**: Transformer核心原理与实现

**Learning Actions**:
- Read: 《注意力机制与Transformer》论文精读
- Watch: Transformer架构解析视频
- Practice: 手写实现Transformer
- Analyze: Hugging Face Transformers源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 注意力机制原理 | 8h | 注意力机制实现 |
| 5-8 | Self-Attention实现 | 8h | Self-Attention模块 |
| 9-12 | Multi-Head Attention | 8h | 多头注意力实现 |
| 13-16 | Transformer Encoder | 8h | Encoder实现 |
| 17-28 | Transformer Decoder | 12h | 完整Transformer |

**Output Requirement**:
- Code: Transformer实现（PyTorch）
- Documentation: Transformer架构博客（1篇）

**Validation**:
- Self-test: 解释注意力机制数学原理
- Practical: 手写Transformer训练成功

---

#### Week 5-8: 预训练模型与Hugging Face

**Topic**: Hugging Face Transformers使用

**Learning Actions**:
- Read: Hugging Face官方文档
- Practice: 预训练模型加载与微调
- Analyze: transformers库核心API

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Transformers库基础 | 6h | 模型加载工具 |
| 5-8 | 预训练模型使用 | 8h | 模型应用示例 |
| 9-12 | Tokenizer原理 | 6h | Tokenizer工具 |
| 13-16 | 模型微调基础 | 8h | 微调脚本 |
| 17-28 | 下游任务实践 | 12h | NLU/NLG项目 |

**Output Requirement**:
- Code: HF模型应用工具
- Documentation: 预训练模型使用博客（2篇）

**Validation**:
- Self-test: 解释BERT/GPT架构差异
- Practical: 完成文本分类微调任务

---

### 阶段 2.2：大模型API应用开发（第17-20个月）

#### Week 1-4: OpenAI/Claude API集成

**Topic**: 商业大模型API应用开发

**Learning Actions**:
- Read: OpenAI/Claude API官方文档
- Practice: API调用与最佳实践
- Analyze: API SDK源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-2 | OpenAI API基础 | 4h | API调用工具 |
| 3-4 | Claude API集成 | 4h | Claude客户端 |
| 5-6 | API参数优化 | 6h | 参数调优工具 |
| 7-8 | 错误处理/重试机制 | 4h | 健壮客户端 |
| 9-12 | 成本控制/Token计算 | 6h | 成本管理工具 |
| 13-28 | 多模型切换系统 | 12h | 多模型API服务 |

**Output Requirement**:
- Code: LLM API客户端库
- Documentation: API集成博客（1篇）

**Validation**:
- Self-test: 解释Token计费机制
- Practical: API客户端支持错误重试

---

#### Week 5-8: Prompt Engineering实践

**Topic**: Prompt设计最佳实践

**Learning Actions**:
- Read: Prompt Engineering指南
- Watch: Prompt优化视频教程
- Practice: Prompt模板设计
- Analyze: 高质量Prompt案例

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-2 | Prompt设计原则 | 4h | Prompt模板库 |
| 3-4 | System Prompt优化 | 6h | 系统提示优化 |
| 5-6 | Few-shot/Zero-shot | 6h | 示例学习模板 |
| 7-8 | Chain-of-Thought | 6h | 思维链模板 |
| 9-12 | 多轮对话Prompt | 8h | 对话Prompt库 |
| 13-28 | Prompt自动化工具 | 12h | Prompt管理系统 |

**Output Requirement**:
- Code: Prompt模板库（50+模板）
- Documentation: Prompt Engineering博客（2篇）

**Validation**:
- Self-test: 解释Few-shot学习原理
- Practical: Prompt模板提升模型准确率

---

### 阶段 2.3：RAG架构开发（第21-24个月）

#### Week 1-4: 向量数据库与检索基础

**Topic**: 向量数据库使用与检索优化

**Learning Actions**:
- Read: 向量数据库官方文档
- Practice: 向量检索与索引构建
- Analyze: FAISS/Pinecone源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 向量嵌入基础 | 6h | 嵌入生成工具 |
| 5-8 | FAISS索引构建 | 8h | FAISS检索系统 |
| 9-12 | Pinecone/Milvus集成 | 8h | 云向量数据库 |
| 13-16 | 检索策略优化 | 6h | 检索优化工具 |
| 17-28 | 向量检索服务 | 12h | 检索API服务 |

**Output Requirement**:
- Code: 向量检索系统
- Documentation: 向量数据库博客（1篇）

**Validation**:
- Self-test: 解释向量检索相似度计算
- Practical: 检索系统支持100万向量

---

#### Week 5-8: RAG系统实现

**Topic**: RAG架构设计与实现

**Learning Actions**:
- Read: RAG架构设计论文/博客
- Watch: RAG系统实现教程
- Practice: RAG系统构建
- Analyze: LangChain/LlamaIndex RAG源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | RAG架构设计 | 6h | RAG架构文档 |
| 5-8 | 文档处理管道 | 8h | 文档处理系统 |
| 9-12 | 检索增强生成 | 8h | RAG核心模块 |
| 13-16 | 上下文优化 | 6h | 上下文管理 |
| 17-28 | RAG系统集成 | 12h | 完整RAG系统 |

**Output Requirement**:
- Code: RAG系统框架
- Documentation: RAG架构博客（2篇）

**Validation**:
- Self-test: 解释RAG检索增强原理
- Practical: RAG问答准确率达到85%

---

### 阶段 2.4：AI Agent开发（第25-28个月）

#### Week 1-4: Function Calling与工具调用

**Topic**: LLM工具调用机制

**Learning Actions**:
- Read: Function Calling官方文档
- Practice: 工具定义与调用
- Analyze: Agent框架源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | Function Calling原理 | 6h | 工具调用示例 |
| 5-8 | 工具定义规范 | 6h | 工具定义库 |
| 9-12 | 多工具调用编排 | 8h | 工具编排系统 |
| 13-28 | 工具调用Agent原型 | 12h | Agent原型系统 |

**Output Requirement**:
- Code: Function Calling工具库
- Documentation: 工具调用博客（1篇）

**Validation**:
- Self-test: 解释Function Calling工作机制
- Practical: Agent支持5+工具调用

---

#### Week 5-8: Agent框架应用

**Topic**: LangChain/LlamaIndex Agent开发

**Learning Actions**:
- Read: LangChain/LlamaIndex官方文档
- Practice: Agent框架使用
- Analyze: Agent框架核心模块

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | LangChain Agent | 6h | LangChain Agent |
| 5-8 | LlamaIndex Agent | 6h | LlamaIndex Agent |
| 9-12 | Agent记忆系统 | 8h | 记忆管理模块 |
| 13-16 | 多Agent协作 | 8h | 协作Agent系统 |
| 17-28 | Agent应用实战 | 12h | 完整Agent应用 |

**Output Requirement**:
- Code: Agent应用系统
- Documentation: Agent开发博客（2篇）

**Validation**:
- Self-test: 解释Agent决策循环机制
- Practical: Agent完成复杂多步骤任务

---

## Year 3：大模型应用架构 + 生产部署（第29-36个月）

### 阶段 3.1：模型微调技术（第29-32个月）

#### Week 1-4: 微调方法与LoRA

**Topic**: 模型微调技术与LoRA应用

**Learning Actions**:
- Read: LoRA/QLoRA论文与文档
- Watch: 模型微调教程视频
- Practice: LoRA微调实战
- Analyze: PEFT库源码

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 微调方法对比 | 6h | 微调方法分析 |
| 5-8 | LoRA原理实现 | 8h | LoRA实现示例 |
| 9-12 | QLoRA量化微调 | 8h | QLoRA微调脚本 |
| 13-16 | PEFT库使用 | 6h | PEFT工具 |
| 17-28 | 微调项目实战 | 12h | 定制化模型 |

**Output Requirement**:
- Code: 模型微调工具链
- Documentation: 微调技术博客（2篇）

**Validation**:
- Self-test: 解释LoRA低秩适应原理
- Practical: LoRA微调模型性能达标

---

#### Week 5-8: 数据集构建与微调流水线

**Topic**: 微调数据集与训练流程

**Learning Actions**:
- Read: 数据集构建最佳实践
- Practice: 微调数据准备
- Analyze: 微调训练流程

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 微调数据集构建 | 8h | 数据集构建工具 |
| 5-8 | 数据清洗与格式化 | 8h | 数据处理脚本 |
| 9-12 | 微调训练流水线 | 8h | 训练流水线 |
| 13-16 | 模型评估与验证 | 6h | 评估框架 |
| 17-28 | 微调项目完整实现 | 12h | 微调模型应用 |

**Output Requirement**:
- Code: 微调流水线系统
- Documentation: 微调实践博客（1篇）

**Validation**:
- Self-test: 解释微调数据质量要求
- Practical: 微调模型达到任务要求

---

### 阶段 3.2：大模型生产部署（第33-36个月）

#### Week 1-4: 推理服务架构

**Topic**: LLM推理服务架构设计

**Learning Actions**:
- Read: vLLM/TGI官方文档
- Watch: 推理服务架构视频
- Practice: 推理服务搭建
- Analyze: vLLM源码核心模块

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | vLLM推理引擎 | 8h | vLLM部署实践 |
| 5-8 | TGI推理服务 | 8h | TGI部署实践 |
| 9-12 | 推理服务API设计 | 6h | API服务框架 |
| 13-28 | 推理服务集成 | 12h | 完整推理服务 |

**Output Requirement**:
- Code: LLM推理服务
- Documentation: 推理部署博客（2篇）

**Validation**:
- Self-test: 解释PagedAttention优化原理
- Practical: 推理服务达到100+ tokens/s

---

#### Week 5-8: 生产环境部署与优化

**Topic**: 生产级LLM应用部署

**Learning Actions**:
- Read: 生产部署最佳实践
- Practice: 生产环境配置
- Analyze: 生产级架构案例

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 容器化部署/Docker | 6h | Docker配置 |
| 5-8 | Kubernetes部署 | 8h | K8s部署配置 |
| 9-12 | 监控与日志系统 | 6h | 监控系统 |
| 13-16 | 性能优化实践 | 8h | 优化版服务 |
| 17-28 | 生产部署实战 | 12h | 生产级应用 |

**Output Requirement**:
- Code: 生产部署配置
- Documentation: 生产部署博客（1篇）

**Validation**:
- Self-test: 解释推理服务性能瓶颈
- Practical: 生产服务稳定性达标

---

### 阶段 3.3：企业级应用架构（第37-40个月）

#### Week 1-4: 企业知识库系统

**Topic**: 企业级知识库架构设计

**Learning Actions**:
- Read: 企业知识库设计文档
- Practice: 知识库系统构建
- Analyze: 企业知识库案例

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | 知识库架构设计 | 6h | 架构设计文档 |
| 5-8 | 文档管理系统 | 8h | 文档管理系统 |
| 9-12 | 智能检索服务 | 8h | 检索服务 |
| 13-28 | 知识库完整实现 | 12h | 企业知识库 |

**Output Requirement**:
- Code: 企业知识库系统
- Documentation: 知识库架构博客（2篇）

**Validation**:
- Self-test: 解释知识库检索优化策略
- Practical: 知识库支持10000+文档

---

#### Week 5-8: AI应用API服务

**Topic**: AI应用API服务架构

**Learning Actions**:
- Read: API服务设计最佳实践
- Practice: API服务开发
- Analyze: FastAPI/Flask框架

**Specific Tasks**:
| Day | Task | Duration | Output |
|-----|------|----------|--------|
| 1-4 | FastAPI服务设计 | 6h | API服务框架 |
| 5-8 | 认证与权限控制 | 8h | 认证系统 |
| 9-12 | API限流与缓存 | 6h | 限流缓存模块 |
| 13-16 | API文档与测试 | 8h | API文档系统 |
| 17-28 | 完整API服务 | 12h | AI应用API |

**Output Requirement**:
- Code: AI应用API服务
- Documentation: API服务博客（1篇）

**Validation**:
- Self-test: 解释API服务高并发处理
- Practical: API服务支持1000+ QPS

---

## 关键项目清单

| Year | Project | Key Tech | Performance Target | Duration |
|------|---------|----------|-------------------|----------|
| 1 | 数据分析工具库 | NumPy/Pandas/Matplotlib | 完整数据处理流程 | 8周 |
| 1 | ML分类预测系统 | Scikit-learn/特征工程 | Kaggle Top 50% | 8周 |
| 1 | CNN图像分类器 | PyTorch/CNN | CIFAR10 90%精度 | 8周 |
| 2 | Transformer实现 | PyTorch/Attention | 模型训练成功 | 8周 |
| 2 | 智能问答系统(RAG) | 向量数据库/LangChain | 85%准确率 | 8周 |
| 2 | AI Agent助手 | Function Calling/多工具 | 完成复杂任务 | 8周 |
| 3 | 模型微调应用 | LoRA/QLoRA/PEFT | 定制化任务达标 | 8周 |
| 3 | LLM推理服务 | vLLM/TGI/Docker | 100+ tokens/s | 8周 |
| 3 | 企业知识库系统 | RAG/API服务 | 10000+文档支持 | 12周 |
| 3 | AI应用API服务 | FastAPI/K8s | 1000+ QPS | 12周 |

---

## 里程碑检验标准

| Milestone | Knowledge Check | Project Completion | External Validation |
|-----------|-----------------|-------------------|-------------------|
| Year 1 Q2 | Python异步/类型系统 | 类型注解工具库 | mypy零警告 |
| Year 1 Q4 | ML基础算法 | ML预测系统 | Kaggle Top 50% |
| Year 2 Q2 | Transformer原理 | Transformer实现 | 模型训练成功 |
| Year 2 Q4 | RAG架构/Agent开发 | 智能问答系统 | 85%准确率 |
| Year 3 Q2 | 模型微调技术 | 微调模型应用 | 任务性能达标 |
| Year 3 Q4 | 生产部署/架构设计 | AI应用服务 | 性能指标达标 |
| Year 3 Q4 | 企业级应用 | 企业知识库 | 文档规模达标 |

---

## 开源贡献路径

| Year | Target Project | Contribution Type | Goal |
|------|----------------|-------------------|------|
| 2 | LangChain | 文档改进/示例 | 1 PR |
| 2 | LlamaIndex | Bug修复/工具开发 | 1 PR |
| 3 | Hugging Face PEFT | 微调示例/文档 | 1 PR |
| 3 | vLLM | 文档/性能优化 | 1 PR |
| 3 | LangChain | 功能开发/优化 | Contributor |

---

## 学习资源汇总

### 书籍清单

| Category | Book Name | Priority | Chapters |
|----------|-----------|----------|----------|
| Python基础 | 《Python编程：从入门到实践（第3版）》 | 高 | 第9-18章 |
| Python进阶 | 《Python并发编程实战》 | 高 | 第1-10章 |
| 数据科学 | 《Python数据科学手册》 | 高 | 第1-5章 |
| 机器学习 | 《机器学习实战》 | 高 | 第1-10章 |
| 深度学习 | 《动手学深度学习（PyTorch版）》 | 高 | 第1-10章 |
| 深度学习 | 《深度学习》（中文版） | 中 | 第1-10章 |
| NLP | 《自然语言处理实战》 | 高 | 第1-8章 |
| LLM应用 | 《大语言模型应用开发实战》 | 高 | 全书 |
| Prompt | 《Prompt Engineering指南》 | 高 | 全书 |

### 课程清单

| Platform | Course Name | Duration | Priority |
|----------|-------------|----------|----------|
| Coursera | Machine Learning Specialization | 3个月 | 高 |
| Fast.ai | Practical Deep Learning for Coders | 7周 | 高 |
| DeepLearning.AI | Natural Language Processing Specialization | 4个月 | 高 |
| DeepLearning.AI | Generative AI with LLMs | 4周 | 高 |
| Hugging Face | NLP Course | 免费 | 高 |
| LangChain | LangChain Academy | 4周 | 高 |

### 技术文档/源码

| Type | Name | Link | Purpose |
|------|------|------|---------|
| 官方文档 | OpenAI API | https://platform.openai.com/docs | API集成 |
| 官方文档 | Anthropic Claude | https://docs.anthropic.com | Claude API |
| 官方文档 | Hugging Face | https://huggingface.co/docs | 预训练模型 |
| 官方文档 | LangChain | https://python.langchain.com/docs | Agent框架 |
| 官方文档 | LlamaIndex | https://docs.llamaindex.ai | RAG框架 |
| 官方文档 | vLLM | https://docs.vllm.ai | 推理引擎 |
| 源码分析 | LangChain | https://github.com/langchain-ai/langchain | Agent架构 |
| 源码分析 | PEFT | https://github.com/huggingface/peft | 微调技术 |
| 源码分析 | vLLM | https://github.com/vllm-project/vllm | 推理优化 |

---

## 性能检验标准量化指标

| Component | Metric | Target | Tool |
|-----------|--------|--------|------|
| ML模型 | 分类准确率 | Kaggle Top 50% | Kaggle评分 |
| CNN模型 | CIFAR10精度 | 90% | 模型测试 |
| Transformer | 训练损失 | 收敛 | 训练监控 |
| RAG系统 | 问答准确率 | 85% | 测试集评估 |
| Agent系统 | 任务完成率 | 90% | 功能测试 |
| 微调模型 | 任务性能 | 基线提升10% | 性能对比 |
| 推理服务 | 推理速度 | 100+ tokens/s | 性能测试 |
| API服务 | QPS | 1000+ | 压测工具 |
| 知识库 | 文档规模 | 10000+ | 系统容量 |

---

## 软技能发展

### 通用能力

| Theme | Content | Importance |
|-------|---------|------------|
| 英语能力 | 技术文档/论文阅读 | ⭐⭐⭐⭐⭐ |
| 开源参与 | Issue/PR/Review | ⭐⭐⭐⭐⭐ |
| 技术写作 | 博客/技术文档 | ⭐⭐⭐⭐⭐ |
| 系统设计 | 应用架构设计 | ⭐⭐⭐⭐ |
| 业务理解 | 企业应用场景 | ⭐⭐⭐⭐ |

---

## Quick Start Guide（前3个月详细任务）

### Month 1: Python高级特性

**Week 1**:
- Day 1-2: 类型注解基础
- Day 3-4: typing模块深入
- Day 5-6: mypy静态检查
- Day 7: 学习笔记整理

**Week 2**:
- Day 1-2: asyncio基础
- Day 3-4: async/await实践
- Day 5-6: 异步HTTP客户端
- Day 7: 练习巩固

**Week 3-4**: 项目工程化/测试/CI

---

### Month 2: 数据科学基础

**Week 1-2**: NumPy/Pandas数据处理
**Week 3-4**: 数据清洗/可视化/分析项目

---

### Month 3: 机器学习入门

**Week 1-2**: Scikit-learn算法实践
**Week 3-4**: 特征工程/模型评估/Kaggle入门

---

*此路线图为有Python基础开发者设计，3年达到LLM应用开发工程师水平。*