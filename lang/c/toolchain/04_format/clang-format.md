# clang-format - LLVM 代码格式化工具

## 1. 概述与背景

### 1.1 工具定位

clang-format 是 LLVM 项目提供的代码格式化工具，支持 C、C++、Objective-C、Java、JavaScript、JSON 等多种编程语言。作为 Clang 编译器前端的一部分，它能够精确解析代码语法并按照可配置的规则进行格式化，是 C/C++ 社区最广泛使用的格式化工具。

clang-format 的核心优势在于其基于 Clang 的语法分析能力，能够准确识别代码结构，避免基于正则表达式的格式化工具可能产生的误格式化问题。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2013 | 3.3 | 首次作为独立工具发布 |
| 2014 | 3.5 | 增加 .clang-format 配置文件支持 |
| 2015 | 3.7 | 引入预设样式（Google, Chromium 等） |
| 2017 | 5.0 | 改进格式化性能，支持更多配置选项 |
| 2019 | 9.0 | 支持 C++20 语法特性 |
| 2021 | 13.0 | 增强大括号换行风格支持 |
| 2023 | 17.0 | 支持 C++23 特性，改进格式化算法 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 语法感知 | 基于 Clang AST，精确理解代码结构 |
| 可配置性 | 200+ 配置项，精细控制格式化行为 |
| 预设样式 | 内置 LLVM、Google、Chromium 等主流风格 |
| 增量格式化 | 支持格式化指定行范围 |
| 批量处理 | 支持批量格式化整个项目 |
| CI 集成 | 提供 dry-run 模式用于持续集成检查 |
| IDE 支持 | 主流 IDE 均有插件支持 |

### 1.4 适用场景

| 场景 | 说明 |
|------|------|
| 团队协作 | 统一代码风格，减少代码审查争议 |
| 代码重构 | 批量格式化遗留代码 |
| CI/CD 流水线 | 自动检查代码风格合规性 |
| 开发流程 | 保存时自动格式化 |
| 代码提交 | pre-commit 钩子自动格式化 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| clang-format | 语法精确、配置丰富、生态成熟 | 仅支持部分语言 |
| astyle | 支持更多语言、轻量级 | 语法分析能力较弱 |
| uncrustify | 配置项极多 | 配置复杂、维护不活跃 |
| format(IDE) | 集成度高 | 风格定制受限 |
| prettier | 语言支持广泛 | 对 C/C++ 支持有限 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS**

```bash
# 使用 Homebrew
brew install clang-format

# 或安装完整的 LLVM
brew install llvm
```

**Ubuntu/Debian**

```bash
# 默认版本
sudo apt install clang-format

# 指定版本
sudo apt install clang-format-17
```

**Windows**

```bash
# 方式1: 随 LLVM 安装
# 下载 https://github.com/llvm/llvm-project/releases

# 方式2: 使用 scoop
scoop install llvm

# 方式3: 使用 chocolatey
choco install llvm
```

**Fedora/RHEL**

```bash
sudo dnf install clang-tools-extra
```

### 2.2 版本管理

```bash
# 查看版本
clang-format --version

# 查看支持的版本
apt search clang-format

# 安装特定版本（Ubuntu）
sudo apt install clang-format-15 clang-format-16 clang-format-17

# 使用特定版本
clang-format-17 main.cpp
```

### 2.3 环境配置

```bash
# 添加到 PATH（如果未自动添加）
export PATH="/usr/lib/llvm-17/bin:$PATH"

# 或创建符号链接
sudo ln -sf /usr/lib/llvm-17/bin/clang-format /usr/local/bin/clang-format
```

### 2.4 验证安装

```bash
# 检查版本
clang-format --version
# 输出示例: clang-format version 17.0.0

# 测试格式化
echo 'int main(){return 0;}' | clang-format
# 输出: int main() { return 0; }

# 查看配置选项
clang-format --help
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 最基本用法：格式化文件（原地修改）
clang-format -i main.c

# 格式化并输出到 stdout（不修改原文件）
clang-format main.c

# 检查格式是否符合规范（CI 常用）
clang-format --dry-run --Werror main.c
```

### 3.2 项目结构

推荐的 .clang-format 配置文件位置：

```
project/
├── .clang-format          # 根目录配置
├── src/
│   ├── .clang-format      # 可覆盖父目录配置
│   └── main.c
├── include/
│   └── .clang-format
└── tests/
    └── .clang-format
```

配置文件查找顺序：
1. 指定的 `-style=file` 和 `-assume-filename`
2. 源文件所在目录的 `.clang-format`
3. 逐级向上查找父目录
4. 使用默认 LLVM 样式

### 3.3 基本命令

| 选项 | 说明 |
|------|------|
| `-i` | 原地修改文件 |
| `--dry-run` | 模拟运行，不修改文件 |
| `--Werror` | 格式不符时返回非零退出码 |
| `-style=<style>` | 指定格式化样式 |
| `-lines=<start>:<end>` | 格式化指定行范围 |
| `-offset=<int>` | 从指定偏移开始格式化 |
| `-length=<int>` | 格式化指定长度 |
| `-assume-filename` | 指定文件名用于查找配置 |
| `-dump-config` | 输出当前配置 |

```bash
# 格式化指定行
clang-format -i -lines=10:20 main.c

# 从标准输入读取
cat main.c | clang-format -assume-filename=main.c

# 使用特定样式
clang-format -style=Google main.c

# 输出配置
clang-format -style=Google -dump-config
```

### 3.4 常用操作

**格式化单个文件**

```bash
clang-format -i src/main.c
```

**格式化多个文件**

```bash
clang-format -i file1.c file2.c file3.c
```

**格式化项目代码**

```bash
# 使用 find
find . -name "*.c" -o -name "*.h" | xargs clang-format -i

# 使用 git ls-files
git ls-files | grep -E '\.(c|h|cpp|hpp)$' | xargs clang-format -i

# 只格式化修改的文件
git diff --name-only | grep -E '\.(c|h)$' | xargs clang-format -i
```

## 4. 进阶特性

### 4.1 高级配置

**缩进与空白**

```yaml
IndentWidth: 4           # 缩进宽度
TabWidth: 4              # Tab 宽度
UseTab: Never            # 使用 Tab：Never/Always/ForIndentation
IndentCaseLabels: true   # case 标签缩进
IndentPPDirectives: None # 预处理指令缩进：None/AfterHash/BeforeHash
IndentExternBlock: NoIndent
IndentGotoLabels: true
IndentWrappedFunctionNames: false
NamespaceIndentation: None
```

**大括号风格**

```yaml
BreakBeforeBraces: Attach  # 大括号换行方式
# 可选值：Attach, Linux, Stroustrup, Allman, GNU, WebKit, Custom

BraceWrapping:
  AfterCaseLabel: true
  AfterClass: true
  AfterControlStatement: true
  AfterEnum: true
  AfterFunction: true
  AfterNamespace: true
  AfterStruct: true
  AfterUnion: true
  AfterExternBlock: true
  BeforeCatch: true
  BeforeElse: true
  IndentBraces: false
  SplitEmptyFunction: true
```

**指针对齐**

```yaml
PointerAlignment: Left    # Left/Right/Middle
# int* ptr;  (Left)
# int *ptr;  (Right)
# int * ptr; (Middle)

DerivePointerAlignment: false
ReferenceAlignment: Pointer
SpaceAroundPointerQualifiers: Default
```

**行宽与换行**

```yaml
ColumnLimit: 100          # 行宽限制，0 表示不限制
AlignAfterOpenBracket: Align
AlignOperands: true
AlignTrailingComments: true
AllowAllArgumentsOnNextLine: true
AllowAllParametersOfDeclarationOnNextLine: true
AllowShortBlocksOnASingleLine: Empty
AllowShortCaseLabelsOnASingleLine: false
AllowShortEnumsOnASingleLine: true
AllowShortFunctionsOnASingleLine: Inline
AllowShortIfStatementsOnASingleLine: Never
AllowShortLoopsOnASingleLine: false
AlwaysBreakAfterReturnType: None
AlwaysBreakBeforeMultilineStrings: false
BreakBeforeBinaryOperators: None
BreakBeforeTernaryOperators: true
BreakConstructorInitializers: BeforeColon
BreakInheritanceList: BeforeColon
BreakStringLiterals: true
```

### 4.2 预设样式详解

| 样式 | 说明 | 适用项目 |
|------|------|----------|
| LLVM | LLVM 项目默认风格，宽松行宽 | LLVM 项目 |
| Google | Google C++ 风格指南，严格规范 | Google 开源项目 |
| Chromium | Chromium 项目风格 | Chromium/Chrome |
| Mozilla | Mozilla 项目风格 | Firefox/Thunderbird |
| WebKit | WebKit 项目风格 | Safari/WebKit |
| Microsoft | 微软风格 | Windows 开发 |
| GNU | GNU 项目风格 | GNU 项目 |

```bash
# 生成各预设样式的配置
clang-format -style=llvm -dump-config > .clang-format-llvm
clang-format -style=google -dump-config > .clang-format-google
clang-format -style=mozilla -dump-config > .clang-format-mozilla
```

### 4.3 Include 排序

```yaml
SortIncludes: CaseSensitive  # Never/CaseSensitive/CaseInsensitive
IncludeBlocks: Preserve       # Preserve/Merge/Regroup

IncludeCategories:
  # 系统头文件
  - Regex: '<[a-z_]+>'
    Priority: 1
  # STL 头文件
  - Regex: '<[a-z_/]+>'
    Priority: 2
  # 项目头文件
  - Regex: '".*"'
    Priority: 3
```

### 4.4 自定义样式

```yaml
BasedOnStyle: Google
Language: Cpp

# 覆盖特定配置
IndentWidth: 4
ColumnLimit: 120
BreakBeforeBraces: Stroustrup
PointerAlignment: Left
AllowShortFunctionsOnASingleLine: Empty
SortIncludes: CaseSensitive

# 函数调用参数换行
BinPackArguments: true
BinPackParameters: true
AlignAfterOpenBracket: Align
```

## 5. 性能优化

### 5.1 调优策略

**增量格式化**

```bash
# 只格式化修改的行
git diff -U0 --no-color HEAD | clang-format-diff.py -i

# 只格式化指定行
clang-format -i -lines=50:100 main.c
```

**并行格式化**

```bash
# 使用 GNU Parallel
find . -name "*.cpp" | parallel -j$(nproc) clang-format -i {}

# 使用 xargs 并行
find . -name "*.cpp" | xargs -P$(nproc) -I{} clang-format -i {}
```

**配置优化**

```yaml
# 禁用耗时特性
SortIncludes: Never
AlignConsecutiveMacros: false
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false
```

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| 锁定版本 | 团队统一 clang-format 版本 |
| 配置文件入库 | 将 .clang-format 加入版本控制 |
| CI 检查 | 在 CI 中强制格式检查 |
| Git 钩子 | 使用 pre-commit 自动格式化 |
| 排除文件 | 使用 .gitignore 排除不需要格式化的文件 |

## 6. 问题排查

### 6.1 常见问题

**配置文件未生效**

```bash
# 检查配置文件位置
clang-format -style=file -dump-config main.c

# 显式指定配置文件
clang-format -style=file:.clang-format main.c
```

**格式化结果不符合预期**

```bash
# 查看当前使用的配置
clang-format -style=file -dump-config main.c

# 检查是否有父目录配置覆盖
clang-format -style=file -dump-config --verbose main.c
```

**版本兼容性问题**

```bash
# 检查配置版本
# .clang-format 中添加
# ---
# Language: Cpp
# BasedOnStyle: LLVM
# Standard: c++17
```

### 6.2 调试技巧

```bash
# 详细输出模式
clang-format --verbose main.c

# 显示游标位置
clang-format --cursor=10 main.c

# 输出替换范围
clang-format --output-replacements-xml main.c
```

**常见错误代码**

| 错误 | 原因 | 解决方案 |
|------|------|----------|
| "No style file found" | 未找到配置文件 | 创建 .clang-format 或使用 -style |
| "Invalid configuration" | 配置语法错误 | 检查 YAML 语法 |
| "Unknown key" | 未知配置项 | 检查配置项名称拼写 |
| "Error parsing" | 代码语法错误 | 修复代码语法问题 |

## 7. 集成实践

### 7.1 Git 集成

**pre-commit 钩子**

```bash
#!/bin/sh
# .git/hooks/pre-commit

# 获取暂存的 C/C++ 文件
FILES=$(git diff --cached --name-only --diff-filter=ACMR | \
        grep -E '\.(c|h|cpp|hpp|cc|cxx)$')

if [ -n "$FILES" ]; then
    echo "Running clang-format on staged files..."
    echo "$FILES" | xargs clang-format -i
    git add $FILES
    echo "Formatted files: $FILES"
fi
```

**使用 .pre-commit-config.yaml**

```yaml
repos:
  - repo: https://github.com/pre-commit/mirrors-clang-format
    rev: v17.0.0
    hooks:
      - id: clang-format
        files: '\.(c|h|cpp|hpp|cc|cxx)$'
```

### 7.2 CI/CD 配置

**GitHub Actions**

```yaml
name: Format Check

on: [push, pull_request]

jobs:
  format-check:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install clang-format
        run: sudo apt install clang-format-17
      - name: Check format
        run: |
          find . -name "*.cpp" -o -name "*.h" | \
          xargs clang-format-17 --dry-run --Werror
```

**GitLab CI**

```yaml
format:
  stage: test
  image: ubuntu:22.04
  before_script:
    - apt update && apt install -y clang-format
  script:
    - find . -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run --Werror
  allow_failure: false
```

### 7.3 IDE 集成

| IDE | 配置方式 |
|-----|----------|
| VSCode | 安装 clang-format 扩展，配置 `editor.formatOnSave` |
| CLion | 内置支持，Settings > Code Style > C/C++ |
| Visual Studio | 安装 LLVM 工具扩展，Tools > Options > LLVM/Clang |
| Vim | 安装 vim-clang-format 插件 |
| Emacs | 加载 clang-format.el |
| Xcode | 安装 ClangFormat-Xcode 插件 |

**VSCode 配置**

```json
{
  "editor.formatOnSave": true,
  "C_Cpp.formatting": "clang-format",
  "clang-format.style": "file",
  "clang-format.executable": "/usr/bin/clang-format-17"
}
```

### 7.4 实战案例

**项目初始化脚本**

```bash
#!/bin/bash
# setup-format.sh - 初始化项目格式化配置

# 创建 .clang-format 配置
cat > .clang-format << 'EOF'
BasedOnStyle: Google
Language: Cpp
IndentWidth: 4
TabWidth: 4
UseTab: Never
ColumnLimit: 100
BreakBeforeBraces: Stroustrup
PointerAlignment: Left
EOF

# 创建 .pre-commit-config.yaml
cat > .pre-commit-config.yaml << 'EOF'
repos:
  - repo: https://github.com/pre-commit/mirrors-clang-format
    rev: v17.0.0
    hooks:
      - id: clang-format
EOF

# 安装 pre-commit
pre-commit install

echo "Format configuration complete!"
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 地址 |
|------|------|
| 官方文档 | https://clang.llvm.org/docs/ClangFormat.html |
| 配置选项参考 | https://clang.llvm.org/docs/ClangFormatStyleOptions.html |
| LLVM 项目 | https://github.com/llvm/llvm-project |
| Release 下载 | https://github.com/llvm/llvm-project/releases |

### 8.2 学习路径

| 阶段 | 内容 | 资源 |
|------|------|------|
| 入门 | 基本命令、预设样式 | 官方文档快速入门 |
| 进阶 | 自定义配置、Git 集成 | 本文档第 4-5 章 |
| 高级 | CI/CD 集成、大型项目配置 | 开源项目配置分析 |
| 精通 | 自定义样式、插件开发 | ClangFormat 源码 |

### 8.3 社区资源

| 资源 | 说明 |
|------|------|
| clang-format 配置生成器 | https://zed0.co.uk/clang-format-configurator/ |
| LLVM Discourse | https://discourse.llvm.org/ |
| Stack Overflow | `clang-format` 标签 |
| GitHub Actions | `clang-format-action` |