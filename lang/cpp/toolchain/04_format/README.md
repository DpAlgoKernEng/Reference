# C++ 代码格式化工具

本目录收录 C++ 代码格式化工具文档。

## 格式化工具概览

| 工具 | 说明 | 配置方式 |
|------|------|----------|
| [clang-format](clang-format.md) | LLVM 代码格式化工具，最流行 | .clang-format |
| [astyle](astyle.md) | Artistic Style，支持多种语言 | .astylerc |

## 工具对比

| 特性 | clang-format | astyle |
|------|--------------|--------|
| 开发者 | LLVM | 独立项目 |
| 配置灵活性 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ |
| IDE 集成 | 广泛 | 良好 |
| 语言支持 | C/C++/Java/JS/... | C/C++/Java/C# |
| 自定义样式 | 基于 LLVM 风格 | 预设风格 |

## 常用代码风格

| 风格 | 说明 |
|------|------|
| LLVM | LLVM 项目默认风格 |
| Google | Google C++ 风格指南 |
| Chromium | Chromium 项目风格 |
| Mozilla | Mozilla 项目风格 |
| WebKit | WebKit 项目风格 |
| Microsoft | 微软风格 |
| GNU | GNU 项目风格 |

## clang-format 配置示例

```yaml
# .clang-format
BasedOnStyle: LLVM
IndentWidth: 4
UseTab: Never
BreakBeforeBraces: Attach
AllowShortIfStatementsOnASingleLine: false
IndentCaseLabels: false
ColumnLimit: 100
```

## 使用方式

### 命令行
```bash
# 格式化单个文件
clang-format -i main.c

# 检查格式（不修改）
clang-format --dry-run --Werror main.c

# 格式化整个项目
find . -name "*.c" -o -name "*.h" | xargs clang-format -i
```

### Git 钩子
```bash
# pre-commit 钩子自动格式化
#!/bin/sh
git diff --name-only --cached | grep -E '\.(c|h)$' | xargs clang-format -i
git add -u
```

## 相关文档

- [静态分析](../05_static_analysis/) - clang-tidy、cppcheck 等