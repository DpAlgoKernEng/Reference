# astyle - Artistic Style 代码格式化工具

## 1. 概述与背景

### 1.1 工具定位

Artistic Style（简称 astyle）是一个免费、快速、轻量级的源代码格式化工具，支持多种编程语言：

- **C/C++**：标准 C、C++、C++/CLI
- **C#**：C# 源代码
- **Java**：Java 源代码
- **Objective-C**：Objective-C 代码

astyle 的设计理念是"艺术化"代码格式化，通过预设样式和灵活配置，让代码具有一致的视觉风格，提高可读性和维护性。

### 1.2 发展历史

| 年份 | 版本 | 里程碑特性 |
|------|------|-----------|
| 1998 | 1.0 | 项目启动，支持基本 C/C++ 格式化 |
| 2002 | 1.15 | 增加 Java 支持 |
| 2006 | 1.21 | 引入预设样式（Allman、K&R、GNU） |
| 2010 | 1.23 | 增加 C# 支持，改进配置文件 |
| 2014 | 2.04 | 支持 Objective-C，新增 Google 样式 |
| 2018 | 3.1 | 改进性能，增加更多缩进选项 |
| 2020 | 3.4 | 支持 C++11/14/17 新特性 |

### 1.3 核心特性

**1. 预设样式丰富**

astyle 内置多种业界标准样式，开箱即用：

- **Allman/BSD**：大括号独占一行，适合可读性优先场景
- **K&R**：经典风格，大括号跟随控制语句
- **Stroustrup**：C++ 之父推荐，K&R 变体
- **Google**：Google C++ 风格指南
- **Mozilla**：Mozilla 项目风格
- **WebKit**：WebKit 项目风格

**2. 高度可配置**

支持 60+ 配置选项，覆盖：

- 缩进策略（空格/Tab）
- 大括号风格
- 空格填充
- 换行规则
- 注释格式化

**3. 批量处理能力**

支持递归目录处理，适合大型项目：

```bash
# 递归格式化整个项目
astyle --recursive --style=google "src/*.cpp" "include/*.h"
```

**4. 安全可靠**

- 原地修改，自动备份
- 语法感知，避免破坏代码结构
- 跨平台支持

### 1.4 适用场景

| 场景 | 推荐指数 | 说明 |
|------|----------|------|
| 新项目快速格式化 | ★★★★★ | 使用预设样式，快速统一代码风格 |
| 遗留代码整理 | ★★★★☆ | 批量格式化老项目，提高可读性 |
| 代码评审前置 | ★★★★☆ | 格式化后提交，减少格式相关评论 |
| 小型团队规范 | ★★★★☆ | 轻量级，易于上手 |
| 大型企业项目 | ★★★☆☆ | 可考虑 clang-format，功能更强大 |
| CI/CD 集成 | ★★★★☆ | 支持命令行，易于集成 |

### 1.5 对比分析

| 维度 | astyle | clang-format | uncrustify |
|------|--------|--------------|------------|
| **支持语言** | C/C++/C#/Java/ObjC | C/C++/Java/JS/JSON/Protobuf 等 | C/C++/C#/Java/ObjC/Pawn |
| **配置灵活性** | 中等（60+ 选项） | 高（500+ 选项） | 极高（700+ 选项） |
| **速度** | 快 | 快 | 中等 |
| **学习曲线** | 低 | 中 | 高 |
| **社区活跃度** | 低 | 高 | 中 |
| **Clang 集成** | ❌ | ✅ | ❌ |
| **配置文件格式** | .astylerc | .clang-format | .cfg |
| **预置样式** | 10+ | 12+ | 需手动配置 |
| **二进制大小** | ~1MB | ~30MB | ~3MB |

**选择建议：**

- **astyle**：轻量级需求、快速上手、简单项目
- **clang-format**：现代 C++ 项目、深度自定义、Clang 工具链
- **uncrustify**：极端自定义需求、多语言混合项目

## 2. 安装与配置

### 2.1 多平台安装

**macOS**

```bash
# Homebrew（推荐）
brew install astyle

# MacPorts
sudo port install astyle

# 手动编译
wget https://sourceforge.net/projects/astyle/files/astyle/astyle%203.4/astyle_3.4_macos.tar.gz
tar -xzf astyle_3.4_macos.tar.gz
cd astyle/build/mac
cmake ..
make
sudo make install
```

**Ubuntu/Debian**

```bash
# APT（推荐）
sudo apt update
sudo apt install astyle

# 源码编译
sudo apt install cmake g++
wget https://sourceforge.net/projects/astyle/files/astyle/astyle%203.4/astyle_3.4_linux.tar.gz
tar -xzf astyle_3.4_linux.tar.gz
cd astyle/build/gcc
cmake ..
make
sudo make install
```

**Windows**

```bash
# Chocolatey（推荐）
choco install astyle

# Scoop
scoop install astyle

# 手动安装
# 1. 下载 ZIP 包：https://sourceforge.net/projects/astyle/
# 2. 解压到目标目录（如 C:\Program Files\AStyle）
# 3. 添加到 PATH 环境变量
```

**验证安装**

```bash
astyle --version
# 输出：Artistic Style Version 3.4
```

### 2.2 版本管理

**查看版本信息**

```bash
astyle --version
```

**版本兼容性**

| astyle 版本 | C++ 标准 | 特性支持 |
|------------|----------|---------|
| 2.x | C++11 | 基础支持 |
| 3.0+ | C++14 | lambda、auto 改进 |
| 3.1+ | C++17 | 结构化绑定 |
| 3.4+ | C++20 | Concepts 等 |

**建议**：项目应明确指定 astyle 版本，避免不同版本格式化差异。

### 2.3 环境配置

**配置文件位置**

astyle 按以下顺序查找配置文件：

1. 命令行 `--options` 指定
2. 当前目录 `.astylerc`
3. 项目根目录 `.astylerc`
4. 用户主目录 `~/.astylerc`
5. 系统目录 `/etc/astylerc`（Linux）

**环境变量**

```bash
# 设置默认配置文件路径
export ARTISTIC_STYLE_OPTIONS="$HOME/.astylerc"

# 添加到 shell 配置（~/.zshrc 或 ~/.bashrc）
echo 'export ARTISTIC_STYLE_OPTIONS="$HOME/.astylerc"' >> ~/.zshrc
source ~/.zshrc
```

**编辑器集成**

| 编辑器 | 插件/配置 |
|--------|----------|
| VS Code | "AStyle Formatter" 扩展 |
| Vim | vim-astyle 插件 |
| Emacs | astyle.el 脚本 |
| CLion | External Tool 配置 |

### 2.4 验证安装

**快速测试**

```bash
# 创建测试文件
cat > test.c << 'EOF'
int main(){if(1){printf("hello");}return 0;}
EOF

# 运行格式化
astyle test.c

# 查看结果
cat test.c
```

**期望输出**

```c
int main()
{
    if (1)
    {
        printf("hello");
    }

    return 0;
}
```

## 3. 基础使用

### 3.1 快速入门

**一分钟上手**

```bash
# 1. 格式化单个文件（使用默认风格）
astyle main.c

# 2. 使用 Google 风格
astyle --style=google main.c

# 3. 格式化并保留原文件
astyle --suffix=.orig --style=google main.c

# 4. 批量格式化
astyle --style=google --recursive "src/*.c" "src/*.h"
```

### 3.2 项目结构

**典型项目配置**

```
project/
├── .astylerc          # astyle 配置文件
├── src/
│   ├── main.c
│   └── utils.c
├── include/
│   └── utils.h
└── scripts/
    └── format.sh      # 格式化脚本
```

**.astylerc 配置示例**

```bash
# 代码风格
--style=google

# 缩进
--indent=spaces=2

# 大括号
--keep-one-line-blocks

# 空格填充
--pad-oper
--pad-header
--unpad-paren

# 换行
--max-code-length=100

# 文件
--lineend=linux
--suffix=.orig
```

### 3.3 基本命令

**文件操作**

```bash
# 格式化单个文件（输出到标准输出）
astyle main.c

# 原地修改（覆盖原文件）
astyle main.c
# 注意：默认行为就是原地修改，会创建 .orig 备份

# 不创建备份
astyle -n main.c

# 指定备份后缀
astyle --suffix=.bak main.c

# 输出到标准输出（不修改文件）
astyle --stdout main.c > main_formatted.c

# 递归处理目录
astyle --recursive "src/*.cpp"
```

**样式选择**

```bash
# Allman 风格（大括号独占一行）
astyle --style=allman main.c

# Java 风格（大括号同行，类定义独占）
astyle --style=java main.java

# K&R 风格（经典 C 风格）
astyle --style=kr main.c

# Stroustrup 风格（C++ 推荐）
astyle --style=stroustrup main.cpp

# Google 风格
astyle --style=google main.cpp

# Mozilla 风格
astyle --style=mozilla main.cpp

# WebKit 风格
astyle --style=webkit main.cpp
```

### 3.4 常用操作

**缩进控制**

```bash
# 4 空格缩进（默认）
astyle -s4 main.c

# 2 空格缩进
astyle -s2 main.c

# Tab 缩进
astyle --indent=tab main.c

# 强制 Tab（将空格转为 Tab）
astyle --force-indent=tab main.c
```

**大括号处理**

```bash
# 仅函数定义大括号独占一行
astyle --break-blocks main.c

# 缩进大括号
astyle --indent-braces main.c

# 不缩进命名空间大括号
astyle --indent-namespaces main.c
```

**空格填充**

```bash
# 操作符两侧加空格
astyle --pad-oper main.c
# 效果：a=b+c → a = b + c

# 关键字后加空格
astyle --pad-header main.c
# 效果：if(x) → if (x)

# 括号内外加空格
astyle --pad-paren main.c
# 效果：func(x) → func ( x )

# 移除括号多余空格
astyle --unpad-paren main.c
```

## 4. 进阶特性

### 4.1 高级配置

**自定义大括号风格**

```bash
# 仅 if/else 大括号独占一行
astyle --break-blocks main.c

# 类定义大括号独占一行
astyle --break-blocks=all main.c

# 单行语句保持一行
astyle --keep-one-line-statements main.c

# 单行代码块保持一行
astyle --keep-one-line-blocks main.c
```

**指针和引用对齐**

```bash
# 指针符号靠近类型
astyle --align-pointer=type main.c
# 效果：int* ptr;

# 指针符号靠近变量名
astyle --align-pointer=name main.c
# 效果：int *ptr;

# 中间对齐
astyle --align-pointer=middle main.c
# 效果：int * ptr;
```

**代码长度控制**

```bash
# 最大行长度 100
astyle --max-code-length=100 main.c

# 最大行长度 120
astyle --max-code-length=120 main.c

# 断行模式
astyle --break-after-logical main.c
```

### 4.2 扩展功能

**命名空间处理**

```bash
# 缩进命名空间内容
astyle --indent-namespaces main.cpp

# 缩进类内容
astyle --indent-classes main.cpp

# 缩进 switch case
astyle --indent-switches main.cpp
# 效果：
# switch (x) {
#   case 1:
#     break;
# }
```

**标签处理**

```bash
# 缩进标签
astyle --indent-labels main.c

# 缩进预处理器
astyle --indent-preproc-cond main.c
# 效果：
# #ifdef DEBUG
#     printf("debug");
# #endif
```

**注释格式化**

```bash
# 格式化注释
astyle --format-comments main.c

# 保持注释缩进
astyle --indent-col1-comments main.c
```

### 4.3 插件生态

**VS Code 集成**

```json
// settings.json
{
  "astyle.executable": "/usr/local/bin/astyle",
  "astyle.configPath": "${workspaceFolder}/.astylerc",
  "astyle.autoFormatOnSave": true,
  "[c]": {
    "editor.defaultFormatter": "auiworks.astyle-formatter"
  },
  "[cpp]": {
    "editor.defaultFormatter": "auiworks.astyle-formatter"
  }
}
```

**Vim 集成**

```vim
" ~/.vimrc
let g:astyle_config = expand('~/.astylerc')
let g:astyle_executable = '/usr/local/bin/astyle'

" 自动格式化
autocmd BufWritePost *.c,*.cpp,*.h silent !astyle --options=<args> <afile>
```

**Git Pre-commit Hook**

```bash
#!/bin/bash
# .git/hooks/pre-commit

FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(c|cpp|h|hpp)$')

if [ -n "$FILES" ]; then
    echo "Running astyle on staged files..."
    echo "$FILES" | xargs astyle --options=.astylerc
    echo "$FILES" | xargs git add
fi
```

## 5. 性能优化

### 5.1 调优策略

**批量处理优化**

```bash
# 使用 find + xargs 并行处理
find . -name "*.cpp" -print0 | xargs -0 -P 4 astyle --style=google

# 使用 GNU parallel（更快）
find . -name "*.cpp" | parallel -j 4 astyle --style=google {}
```

**内存优化**

对于超大文件（>10MB），可考虑：

```bash
# 分段处理（手动分割文件）
head -n 10000 large.cpp > temp.cpp
astyle temp.cpp
```

**性能对比**

| 文件数量 | 总大小 | astyle 耗时 | clang-format 耗时 |
|---------|--------|------------|-------------------|
| 10 files | 50KB | 0.1s | 0.2s |
| 100 files | 500KB | 0.8s | 1.2s |
| 1000 files | 5MB | 7s | 10s |
| 10000 files | 50MB | 65s | 90s |

### 5.2 最佳实践

**项目配置建议**

1. **统一配置文件**

   将 `.astylerc` 置于项目根目录，纳入版本控制。

2. **CI/CD 集成**

   ```yaml
   # .github/workflows/format-check.yml
   name: Format Check

   on: [push, pull_request]

   jobs:
     format:
       runs-on: ubuntu-latest
       steps:
         - uses: actions/checkout@v2
         - name: Install astyle
           run: sudo apt install astyle
         - name: Check formatting
           run: |
             astyle --options=.astylerc --dry-run --recursive "src/*.cpp" "include/*.h"
             git diff --exit-code
   ```

3. **编辑器自动格式化**

   配置保存时自动格式化，确保提交前代码已格式化。

4. **忽略文件**

   使用 `.gitignore` 忽略备份文件：

   ```
   # .gitignore
   *.orig
   *.bak
   ```

## 6. 问题排查

### 6.1 常见问题

**问题 1：中文注释乱码**

```bash
# 原因：文件编码问题
# 解决：确保文件为 UTF-8 编码

# 检查编码
file -i main.c

# 转换编码
iconv -f GBK -t UTF-8 main.c > main_utf8.c
astyle main_utf8.c
```

**问题 2：格式化后编译错误**

```bash
# 原因：astyle 可能破坏宏定义或条件编译
# 解决：使用 --preserve-date 保留文件时间戳，便于定位

astyle --preserve-date main.c

# 对于复杂宏，手动格式化或使用 --indent-preproc-block
```

**问题 3：备份文件冲突**

```bash
# 问题：.orig 文件已存在
# 解决：使用 --suffix 指定唯一后缀

astyle --suffix=.$(date +%Y%m%d%H%M%S) main.c
```

**问题 4：递归模式不工作**

```bash
# 错误写法
astyle --recursive *.c  # shell 展开通配符，非递归

# 正确写法
astyle --recursive "src/*.c"  # 用引号包围，让 astyle 处理递归
```

### 6.2 调试技巧

**预览模式**

```bash
# 使用 --dry-run 预览格式化结果（不修改文件）
astyle --dry-run --verbose main.c
```

**详细输出**

```bash
# 使用 --verbose 显示详细信息
astyle --verbose main.c

# 输出示例：
# main.c
#  formatted    main.c
```

**差异对比**

```bash
# 格式化后对比
cp main.c main_backup.c
astyle main.c
diff -u main_backup.c main.c
```

## 7. 集成实践

### 7.1 工具链集成

**与 CMake 集成**

```cmake
# CMakeLists.txt
find_program(ASTYLE_EXE astyle)

if(ASTYLE_EXE)
    add_custom_target(
        format
        COMMAND ${ASTYLE_EXE}
            --options=${CMAKE_SOURCE_DIR}/.astylerc
            --recursive
            "${CMAKE_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_SOURCE_DIR}/include/*.h"
        COMMENT "Running astyle format..."
    )
endif()
```

**使用命令：**

```bash
cmake --build build --target format
```

**与 Make 集成**

```makefile
# Makefile
.PHONY: format

format:
	astyle --options=.astylerc --recursive "src/*.cpp" "include/*.h"
```

### 7.2 CI/CD 配置

**GitHub Actions 完整配置**

```yaml
name: Code Style Check

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  format-check:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout code
      uses: actions/checkout@v3

    - name: Install astyle
      run: sudo apt-get update && sudo apt-get install -y astyle

    - name: Check formatting
      run: |
        astyle --version
        astyle --options=.astylerc \
               --dry-run \
               --recursive \
               "src/*.cpp" "src/*.c" "include/*.h"

    - name: Verify no changes needed
      run: git diff --exit-code
```

**GitLab CI 配置**

```yaml
# .gitlab-ci.yml
stages:
  - style

format-check:
  stage: style
  image: ubuntu:latest
  before_script:
    - apt-get update && apt-get install -y astyle
  script:
    - astyle --options=.astylerc --dry-run --recursive "src/*.cpp"
    - git diff --exit-code
  only:
    - merge_requests
```

### 7.3 实战案例

**案例 1：大型遗留项目重构**

```bash
#!/bin/bash
# scripts/format_legacy.sh

# 分阶段格式化遗留项目

PROJECT_ROOT="/path/to/project"
ASTYLE_OPTS="--style=google --indent=spaces=2 --pad-oper --suffix=.orig"

echo "阶段 1: 备份原始代码"
find "$PROJECT_ROOT" -name "*.c" -o -name "*.h" | tar -czf backup_$(date +%Y%m%d).tar.gz -T -

echo "阶段 2: 格式化 C 文件"
find "$PROJECT_ROOT/src" -name "*.c" | xargs astyle $ASTYLE_OPTS

echo "阶段 3: 格式化头文件"
find "$PROJECT_ROOT/include" -name "*.h" | xargs astyle $ASTYLE_OPTS

echo "阶段 4: 检查编译"
cd "$PROJECT_ROOT/build" && cmake .. && make

echo "格式化完成"
```

**案例 2：多风格项目配置**

```bash
# 项目结构
# project/
# ├── .astylerc           # C 代码风格
# ├── src_c/
# ├── src_cpp/
# └── .astylerc_cpp       # C++ 代码风格

# format_all.sh
#!/bin/bash

# 格式化 C 代码
astyle --options=.astylerc --recursive "src_c/*.c"

# 格式化 C++ 代码
astyle --options=.astylerc_cpp --recursive "src_cpp/*.cpp"
```

**案例 3：增量格式化**

```bash
#!/bin/bash
# scripts/incremental_format.sh

# 只格式化最近修改的文件

CHANGED_FILES=$(git diff --name-only HEAD~1 | grep -E '\.(c|cpp|h)$')

if [ -n "$CHANGED_FILES" ]; then
    echo "Formatting changed files:"
    echo "$CHANGED_FILES"
    echo "$CHANGED_FILES" | xargs astyle --options=.astylerc
else
    echo "No C/C++ files changed"
fi
```

## 8. 参考资源

### 8.1 官方文档

- **官方网站**：http://astyle.sourceforge.net/
- **用户文档**：http://astyle.sourceforge.net/astyle.html
- **命令行选项**：http://astyle.sourceforge.net/astyle.html#_Options
- **配置文件格式**：http://astyle.sourceforge.net/astyle.html#_Options_File

### 8.2 学习路径

**入门阶段（1-2 天）**

1. 安装 astyle
2. 理解预设样式（Allman、K&R、Google）
3. 格式化单个文件
4. 配置 .astylerc 文件

**进阶阶段（1 周）**

1. 掌握 60+ 配置选项
2. 批量格式化项目
3. 编辑器集成
4. Git pre-commit 钩子

**高级阶段（2 周）**

1. CI/CD 集成
2. 多风格项目管理
3. 性能优化
4. 问题排查与调试

**推荐学习资源**

- 《代码整洁之道》- Robert C. Martin（理解代码风格重要性）
- 《Effective C++》- Scott Meyers（C++ 风格最佳实践）
- Google C++ Style Guide（业界标准参考）
- LLVM Coding Standards（现代 C++ 风格参考）

**社区资源**

- SourceForge 项目页：https://sourceforge.net/projects/astyle/
- GitHub 镜像：https://github.com/AStyle/AStyle
- Stack Overflow 标签：`astyle`

---

**总结**

Artistic Style 是一个轻量级、易用的代码格式化工具，特别适合：

- 快速统一项目代码风格
- 小型团队规范化
- 遗留代码整理
- CI/CD 集成

对于大型项目或需要深度自定义的场景，可考虑 clang-format。但对于大多数 C/C++ 项目，astyle 已足够满足需求。