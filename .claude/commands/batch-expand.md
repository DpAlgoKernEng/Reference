---
name: batch-expand
description: |
  批量扩展技术文档，支持多种扩展深度。
  支持 4 种模式：brief (100-200)、standard (200-300)、detailed (300-400)、deep (400+)。
arguments:
  - name: source_dir
    description: 源文档目录路径
    required: true
  - name: output_dir
    description: 输出目录路径（可原地更新）
    required: true
  - name: mode
    description: 扩展模式 - brief/standard/detailed/deep
    required: false
    default: "detailed"
  - name: file_pattern
    description: 文件过滤模式（如 *.md）
    required: false
    default: "*.md"
  - name: max_parallel
    description: 最大并行数（0=不限），默认不限
    required: false
    default: "0"
---

# 批量文档扩展命令

处理 `${args.source_dir}` 目录下的文档，使用 `doc-expand` skill 按指定模式扩展，输出到 `${args.output_dir}`。

## 执行流程

### 1. 扫描源目录

使用 Glob 工具扫描源目录：
- Pattern: `${args.source_dir}/${args.file_pattern}`
- 排除 README.md（目录索引不处理）

支持的文件格式：.md, .markdown, .txt, .html, .htm, .rst

**扫描输出示例：**
```
扫描: 15 个文件
待处理: 15 个
模式: detailed
```

### 2. 创建输出目录

确保输出目录存在，如不存在则创建。

### 3. 启动 Worker Agents

为每个待处理文件启动 `doc-expander` agent：

- 使用 `Agent` 工具
- 设置 `subagent_type: "doc-expander"`
- 设置 `run_in_background: true`

**并行控制：**
- `max_parallel > 0`: 分批启动，每批 `max_parallel` 个
- `max_parallel = 0`: 同时启动所有 agents

每个 agent 接收 prompt：
```
输入文件：${file_path}
输出文件：${output_dir}/${filename}
文档模式：${args.mode}

使用 doc-expand skill 扩展此文档。
```

### 4. 进度跟踪

显示实时进度表：

| # | 文件 | 状态 | 输出 |
|---|------|------|------|
| 1 | gcc.md | done | 352 lines |
| 2 | cmake.md | running | — |

状态流转：pending → running → done/failed

### 5. 结果汇总

所有 agents 完成后，输出汇总报告：
- 成功处理的文件数
- 失败的文件列表（如有）
- 输出目录位置

**汇总输出示例：**
```
处理完成 (detailed mode):
  成功: 14 个
  失败: 1 个

详情:
  ✓ gcc.md -> 352 lines (detailed)
  ✓ cmake.md -> 380 lines (detailed)
  ✗ clang.md -> FAILED
```

## 扩展模式

| 模式 | 目标行数 | 章节数 | 适用场景 |
|------|---------|--------|---------|
| `brief` | 100-200 | 3-4 | 备忘清单、快速参考 |
| `standard` | 200-300 | 5-6 | 入门指南、学习笔记 |
| `detailed` | 300-400 | 7-8 | 完整教程、参考文档 |
| `deep` | 400+ | 8+ | 专业指南、深度教程 |

## 使用示例

```bash
# brief 模式 - 快速参考
/batch-expand docs/c/toolchain docs/c/toolchain brief "*.md"

# standard 模式 - 入门指南
/batch-expand docs/cpp/toolchain docs/cpp/toolchain standard "*.md"

# detailed 模式 - 完整文档（默认）
/batch-expand docs/c/toolchain docs/c/toolchain detailed "*.md"
/batch-expand docs/c/toolchain docs/c/toolchain "*.md"  # 默认 detailed

# deep 模式 - 深度教程
/batch-expand docs/cpp/toolchain docs/cpp/toolchain deep "*.md"

# 分批处理（限制并行数）
/batch-expand docs/c docs/c detailed "*.md" 5
```

## 质量标准

| 模式 | 行数 | 章节 | 代码块 |
|------|------|------|--------|
| brief | 100-200 | 3-4 | 3-5 |
| standard | 200-300 | 5-6 | 5-8 |
| detailed | 300-400 | 7-8 | 8-12 |
| deep | 400+ | 8+ | 12+ |

## 注意事项

- 原地更新覆盖原文件，建议 git 管理
- README.md 不处理（保留为目录索引）
- 大量文件（>20）建议设置 `max_parallel` 限制资源
- 技术准确性优先于行数目标