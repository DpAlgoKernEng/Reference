# clang-tidy - LLVM 静态分析工具

## 1. 概述与背景

### 1.1 工具定位

clang-tidy 是基于 Clang 的 C/C++ 静态分析工具，提供代码检查、风格验证和自动修复功能。作为 LLVM 工具链的重要组成部分，它能够深入理解代码语义，提供比传统正则表达式工具更精确的分析能力。

### 1.2 核心特性

- **语义分析**: 基于 Clang 编译器前端，深度理解代码结构
- **检查扩展**: 600+ 内置检查规则，支持自定义检查器
- **自动修复**: 对部分问题提供自动修复功能
- **配置灵活**: 支持项目级配置文件 `.clang-tidy`
- **工具链集成**: 与 CMake、VSCode、CLion 等无缝集成

### 1.3 适用场景

| 场景 | 说明 |
|------|------|
| 代码审查 | 在提交前自动检测潜在问题 |
| 遗留代码现代化 | 将旧代码升级到现代标准 |
| 安全审计 | 检测缓冲区溢出、内存泄漏等问题 |
| 性能优化 | 发现不必要的拷贝、低效算法 |

### 1.4 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| clang-tidy | 语义理解强，修复能力强 | 速度较慢，依赖编译环境 |
| cppcheck | 独立运行，速度快 | 语义理解较浅 |
| Coverity | 企业级，深度分析 | 商业付费，配置复杂 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS:**
```bash
# Homebrew 安装
brew install llvm
echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >> ~/.zshrc
clang-tidy --version
```

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install clang-tidy-14
clang-tidy-14 --version
```

**Windows:**
```bash
# 通过 winget 安装
winget install LLVM.LLVM

# 或下载官方安装包
# https://releases.llvm.org/download.html
```

### 2.2 验证安装

```bash
# 测试安装
echo 'int main() { return 0; }' > test.cpp
clang-tidy test.cpp -- -std=c++17

# 查看支持的检查数量
clang-tidy -checks='*' --list-checks | wc -l
```

## 3. 基础使用

### 3.1 快速入门

**单文件检查:**
```bash
# 基本检查（指定标准）
clang-tidy main.cpp -- -std=c++17

# 使用编译数据库
clang-tidy -p build main.cpp

# 检查所有文件
clang-tidy -p build src/*.c
```

**生成编译数据库:**
```bash
# CMake 生成编译数据库
cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

### 3.2 检查组

| 检查组 | 说明 |
|--------|------|
| `bugprone-*` | 潜在 bug 检测 |
| `cert-*` | CERT 安全指南 |
| `cppcoreguidelines-*` | C++ 核心指南 |
| `modernize-*` | 现代化建议 |
| `performance-*` | 性能优化 |
| `readability-*` | 可读性检查 |

### 3.3 基本命令

```bash
# 启用特定检查
clang-tidy -checks='bugprone-*' main.c

# 启用多个检查组
clang-tidy -checks='bugprone-*,performance-*' main.c

# 启用所有检查
clang-tidy -checks='*' main.c

# 禁用特定检查
clang-tidy -checks='*,-readability-magic-numbers' main.c
```

### 3.4 检查项目文件

```bash
# 使用 run-clang-tidy（推荐）
run-clang-tidy -p build -checks='bugprone-*'

# 并行检查
run-clang-tidy -p build -j $(nproc)

# 使用 find
find src -name '*.cpp' | xargs clang-tidy -p build
```

## 4. 进阶特性

### 4.1 配置文件

**.clang-tidy 配置示例:**
```yaml
---
Checks: 'bugprone-*,performance-*,modernize-*'
WarningsAsErrors: ''
HeaderFilterRegex: '.*'
CheckOptions:
  - key: readability-identifier-naming.VariableCase
    value: lower_case
  - key: modernize-use-nullptr.NullMacros
    value: 'NULL'
  - key: readability-identifier-naming.ClassCase
    value: CamelCase
```

### 4.2 自动修复

```bash
# 自动修复（创建备份）
clang-tidy -fix main.c

# 自动修复（无备份）
clang-tidy -fix --fix-errors main.c

# 导出修复建议
clang-tidy --export-fixes=fixes.yaml main.c
```

**修复示例:**
```cpp
// 修复前
int* ptr = NULL;  // warning: use nullptr

// 修复后
int* ptr = nullptr;  // auto-fixed
```

### 4.3 常见检查示例

**bugprone-* 检查:**
```c
// 警告：变量未使用
int unused_var;

// 警告：比较无符号与负数
unsigned int x = 10;
if (x > -1) {}  // 总是 true

// 警告：数组越界
int arr[10];
arr[10] = 0;  // 越界
```

**performance-* 检查:**
```cpp
// 警告：不必要的值拷贝
void func(std::string s);  // 应改为 const std::string&

// 警告：容器拷贝
std::vector<int> v = other;  // 可能不需要拷贝
```

## 5. 性能优化

### 5.1 调优策略

**并行优化:**
```bash
# 使用所有 CPU 核心
run-clang-tidy -p build -j $(nproc)

# 限制内存使用
run-clang-tidy -p build -j 4
```

**选择性检查:**
```yaml
# .clang-tidy 只检查关键问题
Checks: 'bugprone-*,cert-*'
```

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| 增量检查 | 只检查修改的文件（git diff） |
| 分阶段检查 | 先快速检查，后深度检查 |
| CI 集成 | 在 CI 中并行执行 |
| 配置精简 | 禁用不必要的检查 |

**增量检查脚本:**
```bash
#!/bin/bash
CHANGED_FILES=$(git diff --name-only --diff-filter=ACMRTUXB main | grep -E '\.(cpp|h)$')
if [ -n "$CHANGED_FILES" ]; then
    echo "$CHANGED_FILES" | xargs clang-tidy -p build
fi
```

## 6. 问题排查

### 6.1 常见问题

**问题 1: 找不到头文件**
```bash
# 错误信息
fatal error: 'myheader.h' file not found

# 解决方案：生成正确的 compile_commands.json
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 或手动添加包含路径
clang-tidy main.cpp -- -I./include -I./third_party
```

**问题 2: 编译数据库缺失**
```bash
# 错误信息
No compilation database found

# 解决方案：生成 compile_commands.json
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 或使用 Bear 工具
bear -- make
```

**问题 3: 内存不足**
```bash
# 错误信息
LLVM ERROR: out of memory

# 解决方案：减少并行度
run-clang-tidy -p build -j 2
```

### 6.2 调试技巧

```bash
# 查看详细的检查过程
clang-tidy -p build main.cpp -- -v

# 验证 compile_commands.json
jq '.[] | .file' compile_commands.json

# 测试单个检查
clang-tidy -checks='bugprone-argument-comment' main.cpp
```

## 7. 集成实践

### 7.1 CMake 集成

```cmake
find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
    # 全局检查
    set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY_EXE};
        -checks=bugprone-*,cert-*;
        -warnings-as-errors=*;
    )
    
    # 自定义目标
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE}
            -p ${CMAKE_BINARY_DIR}
            ${SOURCES}
        COMMENT "Running clang-tidy"
    )
endif()
```

### 7.2 Git 钩子

```bash
#!/bin/bash
# .git/hooks/pre-commit

FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|h|c|cc)$')

if [ -n "$FILES" ]; then
    echo "Running clang-tidy on staged files..."
    echo "$FILES" | xargs clang-tidy -p build
    
    if [ $? -ne 0 ]; then
        echo "clang-tidy found issues. Please fix them before committing."
        exit 1
    fi
fi
```

### 7.3 CI/CD 配置

**GitHub Actions:**
```yaml
name: clang-tidy

on: [push, pull_request]

jobs:
  check:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install clang-tidy
        run: sudo apt install clang-tidy-14
        
      - name: Configure CMake
        run: cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
        
      - name: Run clang-tidy
        run: find src -name '*.cpp' | xargs clang-tidy-14 -p build
```

### 7.4 实战案例

**案例 1: 遗留代码现代化**
```bash
# 生成现代化建议报告
clang-tidy -checks='modernize-*' -p build --export-fixes=fixes.yaml src/*.cpp

# 应用修复
clang-tidy -fix -checks='modernize-*' -p build src/*.cpp
```

**案例 2: 安全漏洞扫描**
```bash
# CERT 安全检查
clang-tidy -checks='cert-*,bugprone-*,security-*' \
           -p build \
           --export-fixes=security-report.json \
           src/*.cpp
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| clang-tidy 官方文档 | https://clang.llvm.org/extra/clang-tidy/ |
| 检查列表 | https://clang.llvm.org/extra/clang-tidy/checks/list.html |
| LLVM 文档 | https://llvm.org/docs/ |

### 8.2 学习路径

**初级阶段:**
1. 了解基本概念和安装方法
2. 学习常用检查组
3. 配置 `.clang-tidy` 文件
4. 集成到编辑器

**中级阶段:**
1. 掌握编译数据库使用
2. 学习自动修复功能
3. 配置 Git 钩子
4. 集成到 CI/CD

**高级阶段:**
1. 性能优化和调优
2. 自定义检查器开发
3. 团队规范配置

### 8.3 常用检查清单

| 检查 | 说明 |
|------|------|
| `bugprone-argument-comment` | 参数注释检查 |
| `bugprone-assert-side-effect` | assert 副作用 |
| `modernize-deprecated-headers` | 已弃用头文件 |
| `modernize-loop-convert` | 循环转换 |
| `performance-faster-string-find` | 字符串查找优化 |
| `readability-identifier-naming` | 标识符命名 |