# clang-format - LLVM 代码格式化工具

## 1. 概述与背景

### 1.1 工具定位

clang-format 是 LLVM 项目提供的代码格式化工具，支持 C、C++、Objective-C、Java、JavaScript、JSON 等多种编程语言。作为 C/C++ 社区最广泛使用的格式化工具，它能够自动调整代码风格，确保团队代码风格一致性，减少代码审查中的风格争议。

### 1.2 发展历史

| 年份 | 版本 | 重要特性 |
|------|------|----------|
| 2013 | 3.3 | 首次发布，支持基本格式化 |
| 2015 | 3.7 | 引入 .clang-format 配置文件 |
| 2017 | 5.0 | 增加 ClangFormat VSCode 扩展支持 |
| 2019 | 9.0 | 完善配置选项，支持更多样式 |
| 2021 | 13.0 | 改进格式化算法，支持 C++20 |
| 2023 | 16.0+ | 持续优化，增强配置灵活性 |

### 1.3 核心特性

- **多语言支持**: C、C++、Objective-C、Java、JavaScript、JSON、Protobuf 等
- **预设样式**: 提供 LLVM、Google、Chromium、Mozilla、WebKit、Microsoft、GNU 等成熟样式
- **高度可配置**: 支持 100+ 配置选项，可精细控制格式化行为
- **增量格式化**: 支持格式化指定行范围，便于增量集成
- **Git 集成**: 可与 Git hooks 无缝集成，实现提交前自动格式化
- **IDE 集成**: 支持主流 IDE（VSCode、CLion、Visual Studio、Vim、Emacs）
- **无副作用**: 不改变代码语义，仅调整格式

### 1.4 适用场景

| 场景 | 描述 |
|------|------|
| 团队协作 | 统一团队代码风格，减少代码审查中的风格争议 |
| 开源贡献 | 快速适配开源项目的代码风格规范 |
| 遗留代码 | 格式化历史代码库，提升代码可读性 |
| CI/CD 流程 | 在持续集成中强制执行代码风格规范 |
| 代码重构 | 在重构过程中保持代码风格一致性 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| **clang-format** | 配置丰富、多语言支持、LLVM 生态 | C/C++ 重点，其他语言支持有限 |
| **astyle** | 轻量级、速度快 | 配置选项较少 |
| **uncrustify** | 高度可配置 | 配置复杂、性能较慢 |
| **EditorConfig** | 跨编辑器支持 | 仅定义基本样式，不执行格式化 |
| **Prettier** | 多语言、生态丰富 | C/C++ 支持需要插件 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS:**

```bash
# 使用 Homebrew
brew install clang-format

# 使用 Homebrew 安装完整 LLVM 工具链
brew install llvm
# clang-format 位于 /usr/local/opt/llvm/bin/clang-format
```

**Ubuntu/Debian:**

```bash
# 安装默认版本
sudo apt update
sudo apt install clang-format

# 安装特定版本（如 clang-format-14）
sudo apt install clang-format-14

# 验证安装
clang-format --version
```

**Windows:**

```powershell
# 方法1: 使用 winget
winget install LLVM.LLVM

# 方法2: 使用 Chocolatey
choco install llvm

# 方法3: 下载官方安装包
# 访问 https://releases.llvm.org/ 下载 Windows 安装包
# 安装后 clang-format 位于 C:\Program Files\LLVM\bin\
```

**Fedora/RHEL:**

```bash
sudo dnf install clang-tools-extra
```

**Arch Linux:**

```bash
sudo pacman -S clang
```

### 2.2 版本管理

```bash
# 查看当前版本
clang-format --version
# 输出: clang-format version 16.0.0

# 列出系统中的所有版本（Ubuntu）
ls /usr/bin/clang-format*

# 使用特定版本
clang-format-14 --version

# 创建符号链接指向特定版本
sudo ln -sf /usr/bin/clang-format-14 /usr/local/bin/clang-format
```

### 2.3 环境配置

**添加到 PATH（Windows）:**

```powershell
# PowerShell 中添加到用户 PATH
$env:Path += ";C:\Program Files\LLVM\bin"

# 永久添加（系统属性 → 高级 → 环境变量）
```

**验证安装:**

```bash
# 检查版本
clang-format --version

# 检查可执行文件路径
which clang-format    # macOS/Linux
where clang-format    # Windows

# 查看帮助信息
clang-format --help

# 测试格式化
echo 'int main(){return 0;}' | clang-format
# 输出: int main() { return 0; }
```

### 2.4 验证安装

```bash
# 创建测试文件
cat > test.c << 'EOF'
int main(){int x=1;if(x>0){return x;}return 0;}
EOF

# 执行格式化
clang-format test.c

# 预期输出（格式化后的代码）:
# int main() {
#   int x = 1;
#   if (x > 0) {
#     return x;
#   }
#   return 0;
# }
```

## 3. 基础使用

### 3.1 快速入门

**基本命令格式:**

```bash
clang-format [选项] <文件>
```

**常用选项:**

| 选项 | 说明 |
|------|------|
| `-i` | 原地修改文件 |
| `--dry-run` | 仅检查，不修改 |
| `--Werror` | 将格式问题视为错误 |
| `-style=<样式>` | 指定格式样式 |
| `-lines=<n:m>` | 格式化指定行范围 |
| `-dump-config` | 输出当前配置 |
| `-assume-filename=<name>` | 指定文件名（用于管道输入） |

### 3.2 项目结构

**推荐的 .clang-format 放置位置:**

```
project/
├── .clang-format          # 根目录配置
├── src/
│   ├── .clang-format      # 可选：子目录覆盖配置
│   ├── main.cpp
│   └── utils.cpp
├── include/
│   └── project/
│       └── header.h
└── test/
    └── test_main.cpp
```

**配置查找顺序:**
1. 命令行 `-style` 参数
2. 文件所在目录的 `.clang-format`
3. 父目录递归查找 `.clang-format`
4. 用户目录 `~/.clang-format`
5. 默认 LLVM 样式

### 3.3 基本命令

**格式化单个文件:**

```bash
# 格式化并输出到 stdout
clang-format main.c

# 原地修改文件
clang-format -i main.c

# 检查格式是否正确（不修改文件）
clang-format --dry-run --Werror main.c

# 从 stdin 读取，输出到 stdout
cat main.c | clang-format

# 指定文件名（用于识别语言）
cat code.cpp | clang-format -assume-filename=code.cpp
```

**格式化指定范围:**

```bash
# 格式化第 10 到 20 行
clang-format -i -lines=10:20 main.c

# 格式化第 5 行到文件末尾
clang-format -i -lines=5: main.c

# 格式化文件开头到第 30 行
clang-format -i -lines=:30 main.c
```

**使用预设样式:**

```bash
# 使用 LLVM 样式（默认）
clang-format -style=LLVM main.c

# 使用 Google 样式
clang-format -style=Google main.c

# 使用文件中的配置
clang-format -style=file main.c

# 使用内联配置
clang-format -style="{IndentWidth: 4, UseTab: Never}" main.c
```

### 3.4 常用操作

**批量格式化项目:**

```bash
# 方法1: 使用 find
find . -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i

# 方法2: 使用 git ls-files（仅格式化版本控制中的文件）
git ls-files | grep -E '\.(c|h|cpp|hpp|cc|cxx)$' | xargs clang-format -i

# 方法3: 使用 find + 并行处理
find . -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \) -print0 | xargs -0 -P4 clang-format -i

# 方法4: 格式化最近修改的文件
git diff --name-only HEAD~1 | grep -E '\.(c|cpp|h)$' | xargs clang-format -i
```

**检查格式规范:**

```bash
# 检查单个文件
clang-format --dry-run --Werror main.c

# 批量检查（返回非零退出码表示有格式问题）
git ls-files | grep -E '\.(c|cpp|h)$' | xargs clang-format --dry-run --Werror

# 在 CI 中使用
if ! git ls-files | grep -E '\.(c|cpp|h)$' | xargs clang-format --dry-run --Werror; then
    echo "代码格式不符合规范，请运行 clang-format -i"
    exit 1
fi
```

**生成配置差异:**

```bash
# 比较不同样式的配置差异
clang-format -style=LLVM -dump-config > llvm-style.yaml
clang-format -style=Google -dump-config > google-style.yaml
diff llvm-style.yaml google-style.yaml
```

## 4. 进阶特性

### 4.1 高级配置

**完整配置示例:**

```yaml
# .clang-format 完整配置
---
BasedOnStyle: LLVM
Language: Cpp

# 缩进设置
IndentWidth: 4
TabWidth: 4
UseTab: Never
IndentCaseLabels: true
IndentCaseBlocks: false
IndentGotoLabels: true
IndentPPDirectives: None
IndentExternBlock: AfterExternBlock
IndentWrappedFunctionNames: false
NamespaceIndentation: None

# 行宽设置
ColumnLimit: 100

# 大括号风格
BreakBeforeBraces: Attach
BraceWrapping:
  AfterCaseLabel: false
  AfterClass: false
  AfterControlStatement: Never
  AfterEnum: false
  AfterFunction: false
  AfterNamespace: false
  AfterStruct: false
  AfterUnion: false
  AfterExternBlock: false
  BeforeCatch: false
  BeforeElse: false
  BeforeLambdaBody: false
  BeforeWhile: false
  IndentBraces: false
  SplitEmptyFunction: true
  SplitEmptyRecord: true
  SplitEmptyNamespace: true

# 指针对齐
PointerAlignment: Left
DerivePointerAlignment: false
ReferenceAlignment: Pointer

# 空格设置
SpaceAfterCStyleCast: false
SpaceAfterLogicalNot: false
SpaceAfterTemplateKeyword: true
SpaceBeforeAssignmentOperators: true
SpaceBeforeCaseColon: false
SpaceBeforeCpp11BracedList: false
SpaceBeforeCtorInitializerColon: true
SpaceBeforeInheritanceColon: true
SpaceBeforeParens: ControlStatements
SpaceBeforeRangeBasedForLoopColon: true
SpaceInEmptyBlock: false
SpaceInEmptyParentheses: false
SpacesBeforeTrailingComments: 1
SpacesInAngles: Never
SpacesInCStyleCastParentheses: false
SpacesInConditionalStatement: false
SpacesInContainerLiterals: true
SpacesInParentheses: false
SpacesInSquareBrackets: false

# 对齐设置
AlignAfterOpenBracket: Align
AlignArrayOfStructures: None
AlignConsecutiveAssignments: None
AlignConsecutiveBitFields: None
AlignConsecutiveDeclarations: None
AlignConsecutiveMacros: None
AlignEscapedNewlines: Left
AlignOperands: Align
AlignTrailingComments: true

# Include 排序
IncludeBlocks: Preserve
SortIncludes: CaseSensitive
IncludeCategories:
  # 系统头文件
  - Regex: '^<[a-z_]+\.h>'
    Priority: 1
  # C++ 标准库
  - Regex: '^<.*>'
    Priority: 2
  # 项目头文件
  - Regex: '^".*"'
    Priority: 3

# 函数设置
AllowShortBlocksOnASingleLine: Never
AllowShortCaseLabelsOnASingleLine: false
AllowShortEnumsOnASingleLine: true
AllowShortFunctionsOnASingleLine: Empty
AllowShortIfStatementsOnASingleLine: Never
AllowShortLambdasOnASingleLine: All
AllowShortLoopsOnASingleLine: false

# 其他设置
BinPackArguments: true
BinPackParameters: true
BreakBeforeBinaryOperators: None
BreakBeforeConceptDeclarations: true
BreakBeforeTernaryOperators: true
BreakConstructorInitializers: BeforeColon
BreakInheritanceList: BeforeColon
BreakStringLiterals: true
...
```

### 4.2 扩展功能

**条件编译指令格式化:**

```yaml
# 预处理指令缩进
IndentPPDirectives: AfterHash   # None, AfterHash, BeforeHash

# 格式化前:
#if DEBUG
#define LOG(x) printf(x)
#endif

# 格式化后（AfterHash）:
#if DEBUG
#  define LOG(x) printf(x)
#endif
```

**Include 文件排序:**

```yaml
SortIncludes: CaseSensitive
IncludeBlocks: Regroup  # Preserve, Merge, Regroup

IncludeCategories:
  - Regex: '^<(sys|linux|arpa|net)/'
    Priority: 1
    SortPriority: 1
  - Regex: '^<[a-z_]+\.h>'
    Priority: 2
  - Regex: '^<.*>'
    Priority: 3
  - Regex: '^".*"'
    Priority: 4
```

**C++11/14/17/20 特性支持:**

```yaml
# C++11 特性
Cpp11BracedListStyle: true

# C++20 模块支持
InsertBraces: false  # 不自动插入大括号

# Lambda 格式化
LambdaBodyIndentation: Signature  # Signature, OuterScope, Default
```

### 4.3 插件生态

**VSCode 集成:**

```json
// settings.json
{
  "clang-format.style": "file",
  "clang-format.fallbackStyle": "LLVM",
  "editor.formatOnSave": true,
  "editor.defaultFormatter": "xaver.clang-format"
}
```

**CLion 集成:**

```
Settings → Editor → Code Style → C/C++ → ClangFormat
勾选 "Enable ClangFormat"
```

**Vim 集成:**

```vim
" 使用 vim-clang-format 插件
Plug 'rhysd/vim-clang-format'

" 配置
let g:clang_format#style = 'file'
let g:clang_format#auto_format = 1
autocmd FileType c,cpp ClangFormatAutoEnable
```

**Emacs 集成:**

```elisp
;; 使用 clang-format.el
(require 'clang-format)
(setq clang-format-style "file")
(global-set-key (kbd "C-c f") 'clang-format-region)
```

## 5. 性能优化

### 5.1 调优策略

**批量处理优化:**

```bash
# 使用并行处理加速
find . -name "*.cpp" | xargs -P$(nproc) clang-format -i

# 使用 GNU parallel
find . -name "*.cpp" | parallel -j$(nproc) clang-format -i

# 减少文件系统操作（使用 git ls-files）
git ls-files | grep -E '\.(cpp|h)$' | xargs clang-format -i
```

**性能对比:**

| 方法 | 处理 1000 文件时间 |
|------|-------------------|
| 串行处理 | ~30 秒 |
| xargs -P4 并行 | ~8 秒 |
| GNU parallel -j8 | ~4 秒 |

### 5.2 最佳实践

**配置文件管理:**

```yaml
# 项目根目录 .clang-format
---
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 120
---
# 语言特定配置
Language: Java
IndentWidth: 2
---
Language: JavaScript
IndentWidth: 2
```

**渐进式格式化策略:**

```bash
# 方法1: 使用 git blame 信息，只格式化自己的代码
git blame --line-porcelain file.cpp | grep "author " | head -1

# 方法2: 格式化后保留 git 历史
# 使用 git-clang-format 工具
git clang-format

# 方法3: 分支级别格式化
git checkout -b format-code
find . -name "*.cpp" -o -name "*.h" | xargs clang-format -i
git commit -am "style: 格式化所有代码"
```

**性能优化配置:**

```yaml
# 禁用耗时功能以提升速度
SortIncludes: false          # 不排序 include
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false
AlignOperands: false
AlignTrailingComments: false
```

## 6. 问题排查

### 6.1 常见问题

**问题 1: 配置文件未生效**

```bash
# 检查配置文件查找路径
clang-format -style=file -dump-config main.cpp

# 验证配置文件语法
# 使用 YAML 在线验证器检查 .clang-format 文件

# 指定配置文件路径
clang-format -style=file:/path/to/.clang-format main.cpp
```

**问题 2: 格式化结果不符合预期**

```bash
# 查看当前生效的配置
clang-format -style=file -dump-config

# 测试特定配置项
clang-format -style="{IndentWidth: 8}" test.cpp

# 调试输出
clang-format -dump-config -style=file
```

**问题 3: 代码格式化后编译失败**

```c
// 某些格式化可能改变宏的行为
#define MACRO(x) x

// 格式化前
MACRO(int a = 1 + 2);

// 格式化后（可能破坏宏语义）
MACRO(int a = 1 +
           2);
```

**解决方案:**

```yaml
# 禁用宏内部格式化
MacroBlockBegin: "^BEGIN_MACRO"
MacroBlockEnd: "^END_MACRO"

# 或使用注释保护
// clang-format off
#define COMPLEX_MACRO(x) \
    do { \
        x; \
    } while(0)
// clang-format on
```

**问题 4: 大文件格式化超时**

```bash
# 分段格式化
for i in {1..100}; do
    clang-format -i -lines=$((i*100)):$((i*100+99)) large_file.cpp
done

# 或使用更快的配置
clang-format -style="{SortIncludes: false, ColumnLimit: 0}" large_file.cpp
```

### 6.2 调试技巧

**启用调试输出:**

```bash
# 使用 verbose 模式
clang-format -v main.cpp

# 查看配置解析过程
clang-format -dump-config -style="{BasedOnStyle: LLVM, IndentWidth: 4}"
```

**问题隔离:**

```bash
# 测试最小示例
echo "int main(){return 0;}" | clang-format -style=LLVM

# 对比不同样式的输出
echo "int*x,y;" | clang-format -style=LLVM
echo "int*x,y;" | clang-format -style=Google

# 输出行号信息
clang-format --cursor=0 main.cpp
```

## 7. 集成实践

### 7.1 工具链集成

**CMake 集成:**

```cmake
# CMakeLists.txt
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/include/*.h
    )
    
    add_custom_target(
        format-check
        COMMAND ${CLANG_FORMAT}
        --dry-run
        --Werror
        -style=file
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/include/*.h
    )
endif()
```

**使用方式:**

```bash
# 格式化代码
cmake --build build --target format

# 检查格式
cmake --build build --target format-check
```

**Makefile 集成:**

```makefile
# Makefile
CLANG_FORMAT := clang-format
SOURCES := $(shell find src -name "*.cpp" -o -name "*.h")

.PHONY: format format-check
format:
	$(CLANG_FORMAT) -i $(SOURCES)

format-check:
	@$(CLANG_FORMAT) --dry-run --Werror $(SOURCES)
```

### 7.2 CI/CD 配置

**GitHub Actions:**

```yaml
# .github/workflows/format-check.yml
name: Format Check

on: [push, pull_request]

jobs:
  format-check:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Install clang-format
        run: sudo apt-get install -y clang-format
      
      - name: Check format
        run: |
          find src include -name "*.cpp" -o -name "*.h" | \
            xargs clang-format --dry-run --Werror
```

**GitLab CI:**

```yaml
# .gitlab-ci.yml
format-check:
  stage: test
  image: ubuntu:latest
  before_script:
    - apt-get update && apt-get install -y clang-format
  script:
    - find src include -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run --Werror
  rules:
    - if: $CI_PIPELINE_SOURCE == "merge_request_event"
```

**Jenkins Pipeline:**

```groovy
// Jenkinsfile
pipeline {
    agent any
    stages {
        stage('Format Check') {
            steps {
                sh '''
                    sudo apt-get install -y clang-format
                    find src include -name "*.cpp" -o -name "*.h" | \
                        xargs clang-format --dry-run --Werror
                '''
            }
        }
    }
}
```

**Git Hooks 集成:**

```bash
#!/bin/bash
# .git/hooks/pre-commit
# 自动格式化暂存文件

set -e

# 获取暂存的 C/C++ 文件
FILES=$(git diff --cached --name-only --diff-filter=ACMR | \
        grep -E '\.(c|h|cpp|hpp|cc|cxx)$')

if [ -n "$FILES" ]; then
    echo "格式化以下文件:"
    echo "$FILES"
    
    # 格式化文件
    echo "$FILES" | xargs clang-format -i
    
    # 重新添加到暂存区
    echo "$FILES" | xargs git add
    
    echo "代码格式化完成"
fi

exit 0
```

**pre-commit 框架集成:**

```yaml
# .pre-commit-config.yaml
repos:
  - repo: https://github.com/pre-commit/mirrors-clang-format
    rev: v16.0.0
    hooks:
      - id: clang-format
        types_or: [c, c++, java]
```

```bash
# 安装 pre-commit
pip install pre-commit
pre-commit install

# 手动运行检查
pre-commit run --all-files
```

### 7.3 实战案例

**案例 1: 大型项目迁移**

```bash
#!/bin/bash
# 迁移脚本：保留 git 历史的格式化

# 1. 创建格式化分支
git checkout -b code-format-migration

# 2. 配置 git 忽略格式化的 blame
git config blame.ignoreRevsFile .git-blame-ignore-revs

# 3. 执行格式化
find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# 4. 记录格式化的 commit hash
git commit -am "style: 统一代码格式"

# 5. 更新 ignoreRevsFile
git log -1 --format="%H" >> .git-blame-ignore-revs
git add .git-blame-ignore-revs
git commit -m "chore: 添加格式化 commit 到 ignoreRevsFile"
```

**案例 2: 增量格式化检查**

```bash
#!/bin/bash
# 仅检查修改的文件

# 获取目标分支（默认为 main）
TARGET=${1:-main}

# 获取修改的文件
CHANGED_FILES=$(git diff --name-only $TARGET...HEAD | \
                grep -E '\.(cpp|h|c|hpp)$')

if [ -z "$CHANGED_FILES" ]; then
    echo "没有需要检查的文件"
    exit 0
fi

echo "检查以下文件:"
echo "$CHANGED_FILES"

# 执行格式检查
echo "$CHANGED_FILES" | xargs clang-format --dry-run --Werror
```

**案例 3: 混合项目配置**

```yaml
# .clang-format - 支持多语言项目
---
# C/C++ 默认配置
BasedOnStyle: Google
Language: Cpp
IndentWidth: 4
---
# Java 配置
Language: Java
IndentWidth: 2
BasedOnStyle: Google
---
# JavaScript 配置
Language: JavaScript
IndentWidth: 2
---
# Protobuf 配置
Language: Proto
...
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| LLVM 官方文档 | https://clang.llvm.org/docs/ClangFormat.html |
| ClangFormat 选项手册 | https://clang.llvm.org/docs/ClangFormatStyleOptions.html |
| LLVM Releases | https://releases.llvm.org/ |
| ClangFormat 源码 | https://github.com/llvm/llvm-project/tree/main/clang/tools/clang-format |

### 8.2 学习路径

**初级阶段:**
1. 安装 clang-format 并验证
2. 使用预设样式格式化代码
3. 生成并修改 .clang-format 配置
4. 集成到 IDE 中

**中级阶段:**
1. 理解常用配置选项
2. 配置 Git hooks 自动格式化
3. 在 CI/CD 中添加格式检查
4. 管理多语言项目配置

**高级阶段:**
1. 自定义配置选项详解
2. Include 排序规则定制
3. 大型项目迁移策略
4. 性能优化与调试技巧

**推荐阅读:**

- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)
- [Effective Modern C++](https://www.aristeia.com/EMC++.html) - 代码风格最佳实践

**社区资源:**

- [clang-format-configurator](https://zed0.co.uk/clang-format-configurator/) - 在线配置生成器
- [clang-format-lite](https://github.com/fabzbenj/clang-format-lite) - 简化配置工具