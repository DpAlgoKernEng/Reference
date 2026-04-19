---
name: tech-learning-roadmap
description: This skill should be used when the user asks to create learning roadmaps, career development plans, or systematic learning paths. Triggers: "创建学习路线图", "生成技术学习计划", "规划职业发展", "制定学习计划", "我想学", "怎么系统学习", "从零开始学", or mentions "learning path", "career roadmap", "技术路线图". Generates comprehensive technical learning roadmaps from beginner to expert level with specific tasks, milestones, and project deliverables tailored to user's level, time commitment, and target role.
---

# 技术学习路线图生成器

生成完整的技术学习路线图，从初学者到专家级别，包含具体可执行的任务、里程碑和项目产出。

## 目的

将抽象的职业目标转化为结构化、可实现的学习计划。每个路线图包含：

- 年度进度与月度/周度分解
- 具体主题与明确的学习资源
- 实践项目与可衡量的产出
- 技能验证检查点与里程碑
- 开源贡献路径
- 软技能与行业知识整合

## 核心路线图结构

每个生成的路线图遵循标准结构：

```
# [方向名称] 学习路线图

## 概览
- 总时长（年）
- 核心语言/技术
- 职业定位
- 关键差异点

## 年度分解
### Year N: [阶段名称]
#### 阶段 N.1: [子阶段]（第X-Y月）
| 周次 | 主题 | 内容 | 资源 | 产出 |

## 关键项目
- 项目规格
- 技术要求
- 性能目标

## 里程碑检查点
- 技能验证标准
- 项目完成标准

## 开源贡献路径
- 目标项目
- 贡献里程碑

## 通用能力
- 算法/数据结构
- 软技能
- 行业知识
```

## 路线图生成流程

### 步骤 1：检测技术方向

通过关键词自动判断用户的技术方向偏好。

**详见 `references/direction-detection.md`** - 包含完整关键词表和检测规则。

简要规则：
- 核心关键词权重2，辅助关键词权重1
- 多方向得分接近（差距<3分）时，使用 `AskUserQuestion` 让用户确认
- 支持口语化触发检测（如"我想学 Rust"）

### 步骤 2：分析用户需求

收集关键信息。**详见 `references/demand-analysis-guide.md`**。

**必获信息（必须确认）：**
- 目标角色：开发工程师、高级开发、架构师、专家
- 当前水平：零基础、初级、中级、高级
- 每日可投入时间：全职学习 vs 兼职（具体小时数）

**可选信息（按需收集）：**
- 语言偏好、特殊兴趣、现有技术栈

### 步骤 3：选择学习资源

根据用户语言偏好选择资源：

| 用户偏好 | 书籍推荐 | 课程平台 |
|---------|---------|---------|
| 中文优先 | 机械工业/电子工业出版社技术书籍 | B站、极客时间 |
| 英文优先 | O'Reilly/Addison-Wesley 原版书籍 | Coursera, Udemy |
| 无偏好 | 混合推荐，标注语言 | 混合推荐 |

**资源选择原则：** 经典书籍优先、官方文档优先、活跃开源项目优先。

### 步骤 4：使用通用模板

参考 **`references/roadmap-template.md`** 中的通用路线图结构。

### 步骤 5：定制路线图

根据用户情况调整：
- 调整时间线（经验水平）
- 修改深度（有经验者跳过基础）
- 聚焦领域（特殊兴趣）
- 资源本地化（语言偏好）
- 时间分配（每日学习时长）

### 步骤 6：生成具体任务

将每个学习主题转化为可执行任务。**详见 `references/task-generation-methodology.md`**。

### 步骤 7：定义里程碑

设置清晰的检查点。详见 `references/task-generation-methodology.md` 中的里程碑结构模板。

### 步骤 8：添加进度追踪

在路线图末尾添加进度追踪模板，详见 `references/roadmap-template.md` 中的"进度追踪模板"章节。

### 步骤 9：质量验证

执行路线图质量检查清单。**详见 `references/quality-checklist.md`**。

## 错误处理

信息不足、约束冲突、常见问题的处理方式。**详见 `references/error-handling.md`**。

## 输出交付

以格式化 Markdown 呈现路线图。

### 输出结构（按顺序）

1. **基本信息头部** - 总时长、核心语言、目标定位
2. **学习路线总览** - 年度阶段概览
3. **年度详细分解** - 年→阶段→周→天层次
4. **关键项目清单** - 规格、要求、验收标准
5. **里程碑检验标准** - 知识检查、完成标准
6. **开源贡献路径** - 目标项目、PR里程碑
7. **学习资源汇总** - 书籍、课程、文档清单
8. **Quick Start Guide** - 前3个月详细任务
9. **进度追踪模板** - 周度/月度检查清单

## 附加资源

### 参考文件

| 文件 | 内容 |
|------|------|
| `references/direction-detection.md` | 技术方向关键词表和检测规则 |
| `references/demand-analysis-guide.md` | 需求分析指南 |
| `references/roadmap-template.md` | 通用路线图结构模板 |
| `references/task-generation-methodology.md` | 任务生成方法论 |
| `references/quality-checklist.md` | 路线图质量检查清单 |
| `references/error-handling.md` | 错误处理指南 |

### 示例

| 文件 | 技术方向 |
|------|----------|
| `examples/java-enterprise-web-roadmap.md` | Java 企业级Web应用 |
| `examples/java-bigdata-roadmap.md` | Java 大数据架构 |
| `examples/go-cloudnative-roadmap.md` | Go 云原生微服务 |
| `examples/python-llm-app-roadmap.md` | Python AI大模型应用 |
| `examples/rust-database-kernel-roadmap.md` | Rust 数据库内核 |
| `examples/cpp-linux-kernel-roadmap.md` | C++ Linux内核 |
| `examples/cpp-hpc-roadmap.md` | C++ HPC高性能 |
| `examples/cpp-ai-systems-roadmap.md` | C++ AI系统引擎 |

完整示例见 `examples/` 目录。