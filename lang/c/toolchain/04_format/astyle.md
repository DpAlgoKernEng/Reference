# Artistic Style (astyle) - C/C++ 代码格式化工具详解

## 1. 概述与背景

### 1.1 工具定位

Artistic Style (astyle) 是一个免费的、快速的、轻量级的源代码格式化工具，由程序员 Tal Davidson 开发维护。作为一个命令行工具，astyle 专注于代码格式化和缩进，支持多种编程语言和编码风格。

**核心特点：**
- **轻量级**：二进制文件小巧，安装简便，无复杂依赖
- **跨平台**：支持 Windows、Linux、macOS 等主流操作系统
- **可配置**：提供多种预设样式和灵活的自定义选项
- **开源免费**：采用 MIT 许可证，可自由使用和修改

### 1.2 发展历史

| 年份 | 版本 | 主要特性 |
|------|------|----------|
| 2002 | 1.15 | 首次公开发布，支持基本的 C/C++ 格式化 |
| 2005 | 1.20 | 添加 Java 支持，增加多种预设样式 |
| 2008 | 1.23 | 支持更多缩进选项，改进大括号处理 |
| 2010 | 2.01 | 支持 C# 和 Objective-C，添加命名空间缩进 |
| 2013 | 2.04 | 增加 Google 风格，改进模板格式化 |
| 2016 | 3.0 | 完全重写解析器，增强 C++11/14 支持 |
| 2019 | 3.1 | 支持 C++17 特性，改进 Lambda 表达式格式化 |
| 2023 | 3.4 | 支持 C++20 特性，改进性能和稳定性 |

### 1.3 核心特性

**支持语言：**
- C 语言（包括 C11、C18）
- C++ 语言（包括 C++11、C++14、C++17、C++20）
- C++/CLI（微软扩展）
- Objective-C（Apple 扩展）
- C#（部分支持）
- Java

**格式化能力：**
- 多种缩进风格（Allman、K&R、Stroustrup、Google 等）
- 灵活的缩进控制（空格/Tab、缩进宽度）
- 大括号风格处理
- 操作符和括号空格处理
- 命名空间、类、switch 等结构缩进
- 注释格式化
- 代码对齐

### 1.4 适用场景

| 场景 | 适用性 | 说明 |
|------|--------|------|
| 小型项目格式化 | ★★★★★ | 快速、轻量、配置简单 |
| 团队代码风格统一 | ★★★★☆ | 通过配置文件共享样式 |
| CI/CD 集成 | ★★★★☆ | 命令行工具，易于脚本化 |
| 遗留代码格式化 | ★★★★★ | 批量处理能力强 |
| 嵌入式项目 | ★★★★★ | 不依赖外部库，便于交叉编译 |
| 大型 C++ 项目 | ★★★☆☆ | 对复杂模板支持不如 clang-format |

### 1.5 对比分析

#### 与 clang-format 对比

| 特性 | astyle | clang-format |
|------|--------|--------------|
| **安装复杂度** | 低 | 中等（依赖 LLVM） |
| **速度** | 快 | 快 |
| **配置灵活性** | 中等 | 高（YAML 配置） |
| **支持语言** | 6 种 | 20+ 种 |
| **自定义规则** | 有限 | 丰富（80+ 选项） |
| **社区活跃度** | 低 | 高 |
| **Clang 工具链集成** | ❌ | ✅ |
| **二进制大小** | ~1 MB | ~50 MB |
| **对复杂模板支持** | 一般 | 优秀 |

**选择建议：**
- 选择 **astyle**：小型项目、嵌入式环境、简单格式化需求、无 LLVM 依赖环境
- 选择 **clang-format**：大型项目、需要深度自定义、Clang 工具链项目、复杂 C++ 特性

## 2. 安装与配置

### 2.1 多平台安装

#### macOS

```bash
# 使用 Homebrew
brew install astyle

# 验证安装
astyle --version
# 输出示例：Artistic Style Version 3.4
```

#### Ubuntu/Debian

```bash
# 使用 apt
sudo apt update
sudo apt install astyle

# 验证安装
astyle --version
```

#### CentOS/RHEL/Fedora

```bash
# Fedora
sudo dnf install astyle

# CentOS/RHEL (需要 EPEL)
sudo yum install epel-release
sudo yum install astyle
```

#### Windows

```bash
# 方式 1: 使用 Chocolatey
choco install astyle

# 方式 2: 手动安装
# 1. 从 SourceForge 下载 ZIP 包
# 2. 解压到目标目录
# 3. 添加到 PATH 环境变量
```

#### 从源码编译

```bash
# 克隆源码
git clone https://gitlab.com/saalen/astyle.git
cd astyle

# 创建构建目录
mkdir build && cd build

# 配置和编译
cmake ..
make -j$(nproc)

# 安装（可选）
sudo make install
```

### 2.2 版本管理

```bash
# 查看版本
astyle --version

# 查看帮助
astyle --help

# 查看详细选项
astyle --help=html > astyle_options.html
```

**版本兼容性：**
- 2.x 版本：稳定版本，广泛使用
- 3.x 版本：新增 C++17/20 支持，推荐使用

### 2.3 环境配置

#### 配置文件位置

astyle 按以下顺序查找配置文件：

1. `--options` 参数指定的文件
2. 当前目录的 `.astylerc` 文件
3. 用户主目录的 `.astylerc` 文件
4. 环境变量 `ARTISTIC_STYLE_OPTIONS` 指定的文件

#### 创建配置文件

```bash
# 在项目根目录创建
vim .astylerc

# 示例配置内容
cat > .astylerc << 'EOF'
--style=google
--indent=spaces=2
--pad-oper
--pad-header
--lineend=linux
--attach-extern-c
--indent-preproc-define
EOF
```

### 2.4 验证安装

```bash
# 检查可执行文件
which astyle
# 输出示例：/usr/local/bin/astyle

# 测试格式化
echo 'int main(){printf("hello");return 0;}' > test.c
astyle test.c
cat test.c
# 输出格式化后的代码
```

## 3. 基础使用

### 3.1 快速入门

#### 最简单的使用

```bash
# 格式化单个文件（原地修改）
astyle main.c

# 格式化并保留原文件
astyle -n main.c
# 生成 main.c.orig 作为备份

# 指定备份后缀
astyle --suffix=.bak main.c
# 生成 main.c.bak
```

#### 使用预设样式

```bash
# 使用 Allman 风格
astyle --style=allman main.c

# 使用 Google 风格
astyle --style=google main.cpp

# 使用 K&R 风格
astyle --style=kr main.c
```

### 3.2 项目结构

推荐的项目格式化结构：

```
project/
├── .astylerc          # astyle 配置文件
├── src/
│   ├── main.c
│   ├── utils.c
│   └── utils.h
├── include/
│   └── config.h
└── scripts/
    └── format.sh      # 格式化脚本
```

**格式化脚本示例（format.sh）：**

```bash
#!/bin/bash

# 格式化所有 C/C++ 文件
astyle --options=.astylerc \
       --recursive \
       --exclude=third_party \
       "src/*.c" "src/*.h" "include/*.h"

# 或使用 find
find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" \) \
    ! -path "./third_party/*" \
    -exec astyle --options=.astylerc {} \;
```

### 3.3 基本命令

#### 文件处理选项

| 选项 | 说明 | 示例 |
|------|------|------|
| 默认 | 原地修改，创建 `.orig` 备份 | `astyle main.c` |
| `-n` | 不保留备份 | `astyle -n main.c` |
| `--suffix=XXX` | 指定备份后缀 | `astyle --suffix=.bak main.c` |
| `--suffix=none` | 不创建备份 | `astyle --suffix=none main.c` |
| `--recursive` | 递归处理目录 | `astyle --recursive "*.c"` |
| `--exclude=XXX` | 排除目录/文件 | `astyle --exclude=build --recursive "*.c"` |
| `--dry-run` | 仅预览，不修改 | `astyle --dry-run main.c` |

#### 缩进选项

```bash
# 使用 4 空格缩进
astyle -s4 main.c
astyle --indent=spaces=4 main.c

# 使用 Tab 缩进
astyle -t main.c
astyle --indent=tab main.c

# 使用 Tab，转换为空格
astyle --indent=force-tab=4 main.c
```

#### 大括号选项

```bash
# 缩进大括号
astyle -B main.c
astyle --indent-braces main.c

# 不缩进空大括号
astyle --indent-braces --indent-braces-no-empties main.c
```

### 3.4 常用操作

#### 操作 1：格式化整个项目

```bash
# 使用通配符
astyle --style=google --recursive "src/*.c" "src/*.h"

# 使用 find 命令
find . -type f \( -name "*.c" -o -name "*.h" \) \
    -exec astyle --style=google {} \;

# 排除特定目录
find . -type f \( -name "*.c" -o -name "*.h" \) \
    ! -path "./build/*" \
    ! -path "./third_party/*" \
    -exec astyle --style=google {} \;
```

#### 操作 2：检查代码风格

```bash
# 检查文件是否符合风格（不修改）
astyle --dry-run --style=google main.c

# 输出格式化后的内容到 stdout
astyle --style=google < main.c > main_formatted.c
```

#### 操作 3：集成到 Makefile

```makefile
# Makefile 中添加格式化目标
SOURCES = $(wildcard src/*.c) $(wildcard src/*.h)

format:
	astyle --options=.astylerc $(SOURCES)

format-check:
	@for file in $(SOURCES); do \
		astyle --dry-run --options=.astylerc $$file; \
	done
```

## 4. 进阶特性

### 4.1 高级配置

#### 完整配置文件示例

```ini
# .astylerc 完整示例

# 基本样式
--style=google

# 缩进设置
--indent=spaces=2              # 2 空格缩进
--indent-classes               # 缩进类定义
--indent-switches              # 缩进 switch case
--indent-cases                 # 缩进 case 标签
--indent-namespaces            # 缩进命名空间
--indent-labels                # 缩进标签
--indent-preproc-define        # 缩进预处理器定义
--indent-preproc-cond          # 缩进预处理器条件

# 大括号设置
--indent-braces                # 缩进大括号
--indent-braces-no-empties     # 不缩进空大括号

# 空格设置
--pad-oper                     # 操作符周围添加空格
--pad-comma                    # 逗号后添加空格
--pad-header                   # 关键字后添加空格
--unpad-paren                  # 移除括号内部多余空格

# 对齐设置
--align-pointer=name           # 指针符号靠近变量名
--align-reference=name         # 引用符号靠近变量名

# 换行设置
--lineend=linux                # Unix 换行符
--add-brackets                 # 单行语句添加大括号
--convert-tabs                 # 转换 Tab 为空格

# 其他选项
--attach-return-type           # 返回类型与函数名连接
--attach-return-type-decl      # 声明中的返回类型连接
--break-closing-braces         # 闭合大括号换行
--break-elseifs                # else if 换行
```

### 4.2 预设样式详解

#### Allman/BSD 风格

```c
// 特点：大括号独占一行
int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        printf("Arguments: %d\n", argc);
    }
    return 0;
}
```

```bash
astyle --style=allman main.c
# 或
astyle --style=bsd main.c
```

#### K&R 风格

```c
// 特点：开括号在同一行
int main(int argc, char* argv[]) {
    if (argc > 1) {
        printf("Arguments: %d\n", argc);
    }
    return 0;
}
```

```bash
astyle --style=kr main.c
```

#### Stroustrup 风格

```c
// 特点：K&R 变体，函数独占括号行
class Example {
public:
    int getValue()
    {
        return value_;
    }
    void setValue(int v) {
        value_ = v;
    }
private:
    int value_;
};
```

```bash
astyle --style=stroustrup main.cpp
```

#### Google 风格

```c
// 特点：2 空格缩进，开括号同行
namespace example {

int main(int argc, char* argv[]) {
  if (argc > 1) {
    printf("Arguments: %d\n", argc);
  }
  return 0;
}

}  // namespace example
```

```bash
astyle --style=google main.cpp
```

#### Mozilla 风格

```c
// 特点：用于 Mozilla 项目，4 空格缩进
int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        printf("Arguments: %d\n", argc);
    }
    return 0;
}
```

```bash
astyle --style=mozilla main.c
```

### 4.3 扩展功能

#### 代码对齐

```bash
# 指针和引用符号对齐到变量名
astyle --align-pointer=name file.c
# int* ptr;    ->  int *ptr;
# char & ref;  ->  char &ref;

# 指针和引用符号对齐到类型
astyle --align-pointer=type file.c
# int *ptr;    ->  int* ptr;
# char &ref;   ->  char& ref;
```

#### 多行处理

```bash
# 设置最大行宽
astyle --max-code-length=80 main.c

# 强制换行
astyle --break-after-logical main.c
```

#### 注释格式化

```bash
# 格式化注释
astyle --pad-header main.c
// 将 if(condition) 转换为 if (condition)
```

## 5. 性能优化

### 5.1 调优策略

#### 批量处理优化

```bash
# 使用 xargs 并行处理（多核加速）
find . -name "*.c" -o -name "*.h" | \
    xargs -P $(nproc) -I {} astyle {}

# 使用 GNU parallel（更快）
find . -name "*.c" -o -name "*.h" | \
    parallel astyle {}
```

#### 排除不必要的文件

```bash
# 排除生成的代码、第三方库
astyle --recursive \
    --exclude=build \
    --exclude=third_party \
    --exclude=generated \
    "*.c" "*.h"
```

#### 使用配置文件减少参数解析

```bash
# 不推荐：每次都解析长参数列表
astyle --style=google --indent=spaces=2 --pad-oper --pad-header \
    --lineend=linux --suffix=none file.c

# 推荐：使用配置文件
astyle --options=.astylerc file.c
```

### 5.2 最佳实践

#### 项目配置建议

1. **配置文件版本控制**
   ```bash
   # 将 .astylerc 加入版本控制
   git add .astylerc
   
   # 在 README 中说明格式化配置
   ```

2. **EditorConfig 集成**
   ```ini
   # .editorconfig
   root = true
   
   [*.c]
   indent_style = space
   indent_size = 4
   
   [*.cpp]
   indent_style = space
   indent_size = 2
   ```

3. **Git 钩子集成**
   ```bash
   # .git/hooks/pre-commit
   #!/bin/bash
   FILES=$(git diff --cached --name-only --diff-filter=ACM | \
           grep -E '\.(c|cpp|h|hpp)$')
   if [ -n "$FILES" ]; then
       astyle --options=.astylerc $FILES
       git add $FILES
   fi
   ```

#### 格式化检查建议

```bash
# CI 中的格式检查脚本
#!/bin/bash
set -e

# 查找需要格式化的文件
FILES=$(find . -type f \( -name "*.c" -o -name "*.h" \) \
        ! -path "./build/*" ! -path "./third_party/*")

# 检查格式
for file in $FILES; do
    if ! astyle --dry-run --options=.astylerc "$file" | grep -q "Formatted"; then
        echo "File not properly formatted: $file"
        exit 1
    fi
done

echo "All files are properly formatted!"
```

## 6. 问题排查

### 6.1 常见问题

#### 问题 1：格式化后代码不符合预期

**症状：** astyle 格式化的代码风格与预期不一致

**解决方案：**
```bash
# 1. 检查配置文件是否正确
cat .astylerc

# 2. 使用 dry-run 模式预览
astyle --dry-run --verbose main.c

# 3. 检查配置文件路径
astyle --options=.astylerc --verbose main.c
```

#### 问题 2：编码问题导致乱码

**症状：** 中文注释格式化后乱码

**解决方案：**
```bash
# astyle 默认处理 UTF-8
# 确保源文件编码正确
file -i main.c

# 使用 iconv 转换编码
iconv -f GBK -t UTF-8 main.c > main_utf8.c
astyle main_utf8.c
```

#### 问题 3：大文件处理慢

**症状：** 格式化大文件耗时过长

**解决方案：**
```bash
# 1. 检查文件大小
ls -lh large_file.cpp

# 2. 避免不必要的格式化
astyle --dry-run large_file.cpp | grep -q "Unchanged" || astyle large_file.cpp

# 3. 分块处理超大文件
head -n 1000 large_file.cpp | astyle > temp.cpp
```

#### 问题 4：预处理器指令格式错误

**症状：** 宏定义被错误缩进

**解决方案：**
```bash
# 启用预处理器缩进选项
astyle --indent-preproc-define --indent-preproc-cond main.c
```

### 6.2 调试技巧

#### 查看格式化差异

```bash
# 使用 diff 查看变更
cp main.c main.c.bak
astyle main.c
diff main.c.bak main.c

# 使用 git diff
git diff --no-index main.c.bak main.c
```

#### 详细输出模式

```bash
# 启用详细输出
astyle --verbose main.c

# 输出格式化统计
astyle --statistics main.c
```

#### 分步调试

```bash
# 1. 测试基本格式化
echo 'int main(){return 0;}' | astyle

# 2. 添加选项逐步测试
echo 'int main(){return 0;}' | astyle --style=google

# 3. 测试配置文件
echo 'int main(){return 0;}' | astyle --options=.astylerc
```

## 7. 集成实践

### 7.1 工具链集成

#### 与 CMake 集成

```cmake
# CMakeLists.txt
find_program(ASTYLE_EXE astyle)

if(ASTYLE_EXE)
    # 定义格式化目标
    add_custom_target(format
        COMMAND ${ASTYLE_EXE}
            --options=${CMAKE_SOURCE_DIR}/.astylerc
            --recursive
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/src/*.h
        COMMENT "Formatting code with astyle"
    )

    # 定义格式检查目标
    add_custom_target(format-check
        COMMAND ${ASTYLE_EXE}
            --dry-run
            --options=${CMAKE_SOURCE_DIR}/.astylerc
            --recursive
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/src/*.h
        COMMENT "Checking code format"
    )
endif()
```

#### 与 Make 集成

```makefile
# Makefile
ASTYLE = astyle
ASTYLE_OPTIONS = --options=.astylerc

SOURCES = $(wildcard src/*.c) $(wildcard src/*.h)

.PHONY: format format-check

format:
	$(ASTYLE) $(ASTYLE_OPTIONS) $(SOURCES)

format-check:
	@for file in $(SOURCES); do \
		if ! $(ASTYLE) --dry-run $(ASTYLE_OPTIONS) $$file | grep -q "Unchanged"; then \
			echo "Format error in $$file"; \
			exit 1; \
		fi; \
	done
	@echo "All files formatted correctly"
```

#### 与编辑器集成

**Vim 配置：**
```vim
" ~/.vimrc

" 格式化当前文件
nnoremap <F8> :!astyle --options=/path/to/.astylerc %<CR>

" 保存时自动格式化
autocmd BufWritePost *.c,*.h,*.cpp,*.hpp :!astyle --options=/path/to/.astylerc %

" 格式化选中区域
vnoremap <F8> :!astyle --options=/path/to/.astylerc<CR>
```

**Emacs 配置：**
```elisp
;; ~/.emacs.d/init.el

(defun astyle-buffer ()
  "Run astyle on current buffer."
  (interactive)
  (let ((saved-point (point)))
    (shell-command-on-region
     (point-min) (point-max)
     "astyle --options=/path/to/.astylerc"
     (current-buffer) t)
    (goto-char saved-point)))

(add-hook 'c-mode-hook
          (lambda ()
            (local-set-key [f8] 'astyle-buffer)))
```

**VS Code 配置：**
```json
// .vscode/tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Format with astyle",
            "type": "shell",
            "command": "astyle",
            "args": [
                "--options=${workspaceFolder}/.astylerc",
                "${file}"
            ],
            "problemMatcher": [],
            "presentation": {
                "reveal": "silent"
            }
        }
    ]
}
```

### 7.2 CI/CD 配置

#### GitHub Actions

```yaml
# .github/workflows/format-check.yml
name: Code Format Check

on: [push, pull_request]

jobs:
  format-check:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

      - name: Install astyle
        run: sudo apt-get install -y astyle

      - name: Check code format
        run: |
          FILES=$(find . -type f \( -name "*.c" -o -name "*.h" \) \
                  ! -path "./build/*" ! -path "./third_party/*")
          for file in $FILES; do
            astyle --dry-run --options=.astylerc "$file" | grep -q "Formatted" && \
            echo "Format error in $file" && exit 1
          done
          echo "All files properly formatted"
```

#### GitLab CI

```yaml
# .gitlab-ci.yml
format-check:
  stage: test
  image: ubuntu:latest
  before_script:
    - apt-get update && apt-get install -y astyle
  script:
    - |
      for file in $(find . -type f \( -name "*.c" -o -name "*.h" \)); do
        astyle --dry-run --options=.astylerc "$file" | grep -q "Formatted" && \
        echo "Format error in $file" && exit 1
      done
    - echo "All files properly formatted"
  only:
    - merge_requests
```

#### Jenkins Pipeline

```groovy
// Jenkinsfile
pipeline {
    agent any
    stages {
        stage('Format Check') {
            steps {
                sh '''
                    # 安装 astyle
                    if ! command -v astyle &> /dev/null; then
                        apt-get update && apt-get install -y astyle
                    fi
                    
                    # 检查格式
                    find . -type f \\( -name "*.c" -o -name "*.h" \\) | while read file; do
                        astyle --dry-run --options=.astylerc "$file" | grep -q "Formatted" && \
                        echo "Format error in $file" && exit 1
                    done
                    echo "All files properly formatted"
                '''
            }
        }
    }
}
```

### 7.3 实战案例

#### 案例 1：重构遗留代码

**场景：** 将老旧 C 项目从混乱的代码风格统一为 K&R 风格

```bash
#!/bin/bash
# format_legacy.sh

# 配置文件
cat > .astylerc << 'EOF'
--style=kr
--indent=spaces=4
--pad-oper
--pad-header
--lineend=linux
--suffix=.orig
--preserve-date
EOF

# 查找所有 C 文件
echo "Finding C source files..."
FILES=$(find . -type f \( -name "*.c" -o -name "*.h" \) \
        ! -path "./vendor/*" \
        ! -path "./build/*")

# 统计
TOTAL=$(echo "$FILES" | wc -l)
CURRENT=0

# 格式化
for file in $FILES; do
    CURRENT=$((CURRENT + 1))
    echo "[$CURRENT/$TOTAL] Formatting: $file"
    astyle --options=.astylerc "$file"
done

echo "Done! Formatted $TOTAL files."
echo "Original files backed up with .orig extension"
```

#### 案例 2：团队协作规范

**场景：** 为团队项目建立代码格式化规范

```bash
# 项目目录结构
project/
├── .astylerc              # 格式化配置
├── .editorconfig          # 编辑器配置
├── scripts/
│   └── format-code.sh     # 格式化脚本
├── .git/
│   └── hooks/
│       └── pre-commit     # Git 钩子
└── .github/
    └── workflows/
        └── format-check.yml  # CI 检查
```

**团队配置文件：**
```ini
# .astylerc - 团队统一配置
--style=google
--indent=spaces=2
--pad-oper
--pad-header
--align-pointer=name
--lineend=linux
--suffix=none

# 命名空间和类缩进
--indent-namespaces
--indent-classes
--indent-switches
```

#### 案例 3：嵌入式项目格式化

**场景：** 嵌入式 C 项目，无外部依赖环境

```bash
# 交叉编译环境中使用 astyle
# 1. 静态编译 astyle
wget https://downloads.sourceforge.net/project/astyle/astyle/astyle%203.4/astyle_3.4_linux.tar.gz
tar xzf astyle_3.4_linux.tar.gz
cd astyle/build/gcc
make static

# 2. 复制到嵌入式工具链目录
cp bin/astyle /opt/arm-toolchain/bin/

# 3. 配置嵌入式项目
cat > .astylerc << 'EOF'
--style=allman
--indent=spaces=4
--pad-oper
--lineend=linux
--suffix=none
--preserve-date
EOF

# 4. 格式化所有源码
find src -name "*.c" -o -name "*.h" | xargs astyle
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| 官方网站 | https://astyle.sourceforge.net/ |
| 文档主页 | https://astyle.sourceforge.net/astyle.html |
| 下载页面 | https://sourceforge.net/projects/astyle/ |
| Git 仓库 | https://gitlab.com/saalen/astyle |
| 发布说明 | https://astyle.sourceforge.net/notes.html |

### 8.2 学习路径

#### 初学者路径

1. **第 1 天：基础使用**
   - 安装 astyle
   - 学习基本命令
   - 格式化单个文件
   - 理解预设样式

2. **第 2-3 天：配置定制**
   - 学习配置文件语法
   - 创建项目 .astylerc
   - 理解缩进和空格选项
   - 实践批量格式化

3. **第 4-5 天：工具集成**
   - 集成到编辑器
   - 配置 Git 钩子
   - 编写格式化脚本
   - 设置 CI 检查

#### 进阶者路径

1. **深入理解**
   - 研究各种预设样式的差异
   - 学习高级配置选项
   - 理解 astyle 解析原理

2. **团队应用**
   - 制定团队编码规范
   - 建立 CI/CD 集成
   - 处理遗留代码格式化

3. **扩展应用**
   - 与其他代码质量工具集成
   - 开发自定义格式化脚本
   - 对比 astyle 与 clang-format

### 8.3 相关工具

| 工具 | 类型 | 说明 |
|------|------|------|
| clang-format | 格式化工具 | LLVM 项目，功能更强大 |
| indent | 格式化工具 | GNU indent，传统工具 |
| uncrustify | 格式化工具 | 支持更多语言和选项 |
| EditorConfig | 编辑器配置 | 跨编辑器统一配置 |
| pre-commit | Git 钩子框架 | 自动化代码检查 |

---

**文档版本：** 1.0  
**最后更新：** 2026-04-06  
**适用版本：** astyle 3.4+