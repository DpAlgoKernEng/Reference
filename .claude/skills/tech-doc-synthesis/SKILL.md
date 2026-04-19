---
name: tech-doc-synthesis
description: 当用户提供技术文档内容/文件/URL并要求整理总结生成结构化中文文档时使用。触发关键词："整理"、"总结"、"生成文档"、"文档合成"、"转中文文档"、"document synthesis"、"分析这个文档"。单文件处理专用，批量处理请使用 /batch-synthesis 命令。
---

# 技术文档合成器

分析技术文档并生成结构化中文文档，支持多种编程语言和文档类型。

## 支持格式

| 格式 | 扩展名 | 说明 |
|------|--------|------|
| HTML | .html, .htm | 网页文档，自动过滤导航/广告 |
| Markdown | .md, .markdown | 结构化文档 |
| 代码 | .cpp, .py, .js, .java, .go, .rs 等 | 源代码分析 |
| 配置 | .json, .yaml, .toml | 配置文件说明 |

## 文档模式

根据文档类型选择合适的输出结构：

**详见 `references/doc-structure.md`** - 包含各模式：标准模式（默认）、API 文档模式、简化模式、教程模式详细结构说明。

## 使用流程

### 步骤 1：检测输入类型

根据输入判断处理方式：

| 输入类型 | 检测方式 | 处理方法 |
|---------|---------|---------|
| 文件路径 | 存在对应文件 | Read 工具读取 |
| URL | 以 http/https 开头 | WebFetch 获取内容 |
| 文本内容 | 直接提供 | 直接处理 |

**注意：** 如输入是目录路径，提示用户使用 `/batch-synthesis` 命令进行批量处理。

### 步骤 2：识别技术栈

**通过文件扩展名检测：**

| 扩展名 | 技术栈 | 风格指南 |
|--------|--------|---------|
| `.cpp`, `.h`, `.hpp`, `.cxx` | C/C++ | `references/cpp-style.md` |
| `.py`, `.pyw` | Python | `references/python-style.md` |
| `.java` | Java | `references/java-style.md` |
| `.go` | Go | `references/go-style.md` |
| `.rs` | Rust | `references/rust-style.md` |
| `.ts`, `.tsx` | TypeScript | `references/javascript-typescript-style.md` |
| `.js`, `.jsx` | JavaScript | `references/javascript-typescript-style.md` |
| `.json`, `.yaml`（OpenAPI）| REST API | `references/web-api-style.md` |

**通过关键词检测（无扩展名时）：**

| 关键词 | 技术栈 |
|--------|--------|
| `template`, `namespace`, `std::`, `#include` | C++ |
| `def `, `import `, `class `, `__init__` | Python |
| `public class`, `interface `, `extends `, `implements ` | Java |
| `package `, `func `, `goroutine`, `go ` | Go |
| `fn `, `let mut`, `impl `, `pub fn` | Rust |
| `function `, `const `, `=>`, `interface ` | JavaScript/TypeScript |
| `GET`, `POST`, `endpoint`, `API`, `/v1/` | REST API |

### 步骤 3：选择文档模式

**选择优先级（从高到低）：**

| 优先级 | 检测条件 | 模式 | 说明 |
|--------|----------|------|------|
| 1 | 包含 REST 端点（GET/POST/PUT/DELETE）、API 路径（/v1/、/api/） | API 模式 | API 文档特征最明确 |
| 2 | 标题含"教程"、"指南"、"如何"、步骤编号（1. 2. 3.） | 教程模式 | 教学性质文档 |
| 3 | 纯命令列表、配置选项表格、无段落说明 | 简化模式 | 快速参考类 |
| 4 | 其他情况 | 标准模式 | 默认选择 |

**混合内容判断规则：**

```
IF 存在 REST API 端点定义 → API 模式（优先级最高）
ELSE IF 步骤指导 > 50% 内容 → 教程模式
ELSE IF 命令/配置列表 > 70% 内容 → 简化模式
ELSE → 标准模式
```

**章节数量对照：**

| 模式 | 章节数 | 适用场景 |
|------|--------|----------|
| 标准模式 | 7 | 类/函数定义、库介绍、技术概念 |
| API 模式 | 7 | REST API、SDK 文档 |
| 简化模式 | 3 | 速查表、命令参考 |
| 教程模式 | 6 | 教程、指南、操作步骤 |

### 步骤 4：应用风格指南

**执行方式：** 风格指南作为参考规范，在生成文档时直接应用，无需显式 Read。

**应用规则：**

| 检测结果 | 执行动作 |
|----------|----------|
| 识别到技术栈 | 应用对应 `references/*-style.md` 的术语表和代码规范 |
| 未识别技术栈 | 使用通用规范，术语首次出现时标注英文 |
| 风格指南不存在 | 使用默认规范 |

**术语标注规范：**
- 首次出现：`中文（English）` 或 `中文 \`English\``
- 后续出现：仅使用中文
- 代码中的术语保持英文原样

**代码示例规范：**
- 遵循风格指南中的命名规范
- 使用风格指南推荐的头文件/导入方式
- 标注版本要求（如 C++17、Python 3.10+）

### 步骤 5：生成文档

按照选定模式的章节结构生成中文文档：
1. 提取完整详细的核心信息，过滤无关内容，但不要遗漏重要细节
2. 按章节组织内容
3. 添加代码示例和表格
4. 标注技术术语英文原文

### 步骤 6：质量检查与输出

**执行时机：** 文档生成完成后、写入文件前，执行质量检查。

**检查流程：**

```
生成文档内容
    ↓
执行质量检查（遍历下方清单）
    ↓
全部通过 → 使用 Write 工具写入文件
    ↓
输出完成信息："已生成文档：{输出路径}"
```

**质量检查清单（逐项验证）：**

1. 术语标注

- [ ] 技术术语首次出现标注英文原文
- [ ] 格式：`中文（English）` 或 `中文 \`English\``
- [ ] 同一术语全文保持一致

2. 代码示例

- [ ] 所有代码块标注语言标识
- [ ] 代码语法正确，可运行
- [ ] 包含必要注释说明
- [ ] 使用风格指南推荐的代码风格

3. 结构完整

- [ ] 所有章节完整呈现
- [ ] 章节标题层次正确
- [ ] 无重要信息遗漏
- [ ] 表格/列表格式正确

4. 格式规范

- [ ] 使用标准 Markdown 语法
- [ ] 标题层级合理（最多 4 级）
- [ ] 链接有效可访问
- [ ] 图片有替代文本

**检查失败处理：**

| 问题类型 | 处理方式 |
|----------|----------|
| 缺少术语标注 | 补充标注后继续 |
| 代码块无语言 | 补充语言标识后继续 |
| 章节缺失 | 补充章节内容后继续 |
| 格式错误 | 修正格式后继续 |

**输出确认：**

文档写入后，输出确认信息：
```
✅ 文档生成完成
   输入：{输入源}
   输出：{输出路径}
   模式：{文档模式}
   章节：{实际章节数}
```

## 风格指南

不同技术栈有特定的文档风格，详见 `references/` 目录：

| 文件 | 说明 |
|------|------|
| `cpp-style.md` | C/C++ 文档风格 |
| `python-style.md` | Python 文档风格 |
| `java-style.md` | Java 文档风格 |
| `go-style.md` | Go 文档风格 |
| `rust-style.md` | Rust 文档风格 |
| `javascript-typescript-style.md` | JavaScript/TypeScript 文档风格 |
| `web-api-style.md` | Web API 文档风格 |

## 示例参考

`examples/` 目录提供不同类型的完整示例：

| 文件 | 类型 | 风格指南 | 说明 |
|------|------|---------|------|
| `cpp-container.md` | 标准模式 | cpp-style.md | C++ STL 容器文档 |
| `python-requests.md` | 标准模式 | python-style.md | Python requests 库文档 |
| `go-concurrency.md` | 标准模式 | go-style.md | Go 并发原语文档 |
| `rust-ownership.md` | 标准模式 | rust-style.md | Rust 所有权与并发文档 |
| `java-async.md` | 标准模式 | java-style.md | Java 异步编程文档 |
| `typescript-promise.md` | 标准模式 | javascript-typescript-style.md | TypeScript 异步编程文档 |
| `rest-api.md` | API 模式 | web-api-style.md | REST API 文档 |
| `quick-reference.md` | 简化模式 | - | Git 速查表 |
| `python-requests-tutorial.md` | 教程模式 | python-style.md | Python requests 库教程 |

## 输入处理

### 单文件处理（本 Skill）

本 Skill 专注于**单文件处理**，支持以下输入类型：

| 输入类型 | 检测方式 | 处理方法 |
|---------|---------|---------|
| 文件路径 | 存在对应文件 | Read 工具读取 |
| URL | 以 http/https 开头 | WebFetch 获取内容 |
| 文本内容 | 直接提供 | 直接处理 |

### 批量处理（使用 /batch-synthesis 命令）

**如需处理多个文件，请使用 `/batch-synthesis` 命令：**

```bash
# 批量处理示例
/batch-synthesis /path/to/source /path/to/output "*.html" standard
```

**Skill 与 Command 边界：**

| 维度 | tech-doc-synthesis (Skill) | batch-synthesis (Command) |
|------|---------------------------|---------------------------|
| 处理范围 | 单文件 | 多文件/目录 |
| 执行方式 | 直接执行 | 并行启动多个 Agent |
| 输出控制 | 单文件输出 | 批量输出到指定目录 |
| 进度跟踪 | 无 | 实时进度表格 |

### 超大文件处理

大于 1MB 的文件自动启用分块处理：

| 文件类型 | 分块边界 |
|---------|---------|
| HTML | `<h1>`, `<h2>` 标题 |
| Markdown | `##` 标题 |
| 代码 | 类/函数定义 |

分块后按章节合并，去除重复内容。

## 输出规范

### 输出路径决策

```
用户指定输出路径？
├── 是 → 使用指定路径
│       └── 路径已存在？覆盖文件
└── 否 → 默认行为
        ├── 输入是文件 → 当前目录，原名.md
        └── 输入是URL/文本 → 输出到当前目录，命名为 document.md
```

### 文件命名规则

| 场景 | 输入 | 输出文件名 | 示例 |
|------|------|-----------|------|
| 单文件 | `vector.html` | `vector.md` | 同名换扩展名 |
| 单文件（无扩展名） | `README` | `README.md` | 追加扩展名 |
| URL | `https://example.com/api` | `api.md` 或 `document.md` | 提取路径名 |
| 文本内容 | 直接文本 | `document.md` | 默认名称 |
| 用户指定 | 任意 | 用户指定名称 | 优先使用用户指定 |

### 输出位置逻辑

| 条件 | 行为 |
|------|------|
| 用户指定路径 | 使用指定路径（绝对或相对） |
| 未指定路径 | 输出到当前工作目录（`pwd`） |
| 输出文件已存在 | 覆盖写入（使用 Write 工具） |
| 输出目录不存在 | 自动创建（使用 Write 工具） |

### 使用 Write 工具

生成文档后，使用 Write 工具写入文件：

```
Write 工具参数：
- file_path: 确定的输出路径
- content: 生成的 Markdown 内容
```

### 格式要求

- 使用 UTF-8 编码
- 使用标准 Markdown 格式
- 代码块必须标注语言
- 表格用于对比和参数说明
- 列表用于步骤和要点

## 附加资源

- `references/*-style.md` - 语言风格指南（见上方表格）
- `examples/*.md` - 完整示例（见上方表格）
