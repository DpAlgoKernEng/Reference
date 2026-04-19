---
name: batch-synthesis
description: Batch process technical documents in parallel using tech-doc-synthesis skill to convert source files into structured Chinese documentation
arguments:
  - name: source_dir
    description: Source directory path containing files to process
    required: true
  - name: output_dir
    description: Output directory path for generated documents
    required: true
  - name: file_pattern
    description: File filter pattern (e.g., *.html, *.md), defaults to all supported files
    required: false
    default: "*"
  - name: mode
    description: Document mode (standard/api/simplified/tutorial), defaults to standard
    required: false
    default: "standard"
  - name: max_parallel
    description: Maximum number of parallel agents (0 = unlimited), defaults to 0
    required: false
    default: "0"
---

# Batch Document Processing Command

## Task Overview

Process files from `${args.source_dir}` directory in parallel using `tech-doc-synthesis` skill, output structured Chinese documents to `${args.output_dir}`.

## Execution Steps

### 1. Scan Source Directory

First scan the source directory to get list of files to process:

```bash
# Scan based on file pattern
# Supported formats: .html, .htm, .md, .markdown, .txt, .cpp, .hpp, .c, .h, .py, .js, .java, .go, .rs, .json, .yaml
```

Use Glob tool to scan:
- Pattern: `${args.source_dir}/${args.file_pattern}`

### 2. Create Output Directory

Ensure output directory exists, create if necessary.

### 3. Launch Worker Agents in Parallel

为每个待处理文件启动 `doc-processor` agent：

- 使用 `Agent` 工具
- 设置 `subagent_type: "doc-processor"`
- 设置 `run_in_background: true`

**并行控制**：
- `max_parallel > 0`: 分批启动，每批 `max_parallel` 个
- `max_parallel = 0`: 同时启动所有 agents

每个 agent 接收 prompt：
```
输入文件：${file_path}
输出文件：${output_dir}/${filename}.md
文档模式：${args.mode}

使用 tech-doc-synthesis skill 处理此文件，生成结构化中文文档。
```

### 4. Track Progress

Display real-time progress table:

| # | File | Status | Output |
|---|------|--------|--------|
| 1 | file1.html | done | output1.md |
| 2 | file2.md | running | — |

### 5. Summarize Results

After all agents complete, output summary report:
- Number of successfully processed files
- List of failed files (if any)
- Output directory location

## Document Modes

Select appropriate mode based on content type:

| Mode | Use Case | Sections |
|------|----------|----------|
| `standard` | General technical docs, libraries/frameworks | 7 |
| `api` | REST API, SDK documentation | 7 |
| `simplified` | Quick reference, cheat sheets | 3 |
| `tutorial` | Tutorials, guides | 6 |

## Usage Examples

```bash
# Process all HTML files (standard mode)
/batch-synthesis /path/to/html_files /path/to/output "*.html"

# Process API documentation
/batch-synthesis /api_docs /output "*.md" api

# Process Python files with max 5 parallel agents
/batch-synthesis /source/python /docs "*.py" standard 5

# Process all supported files with unlimited parallelism
/batch-synthesis /source /output
```

## Notes

- For large number of files (>20), consider using `max_parallel` to limit resource usage
- Large files (>1MB) are automatically processed in chunks
- Default parallelism is unlimited, adjust based on system resources
- Different programming languages automatically apply corresponding documentation styles