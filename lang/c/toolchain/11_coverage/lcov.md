# lcov - 代码覆盖率可视化工具

## 1. 概述与背景

### 1.1 工具定位

lcov 是 Linux 下基于 GCC/gcov 的代码覆盖率可视化工具，它能够收集 gcov 生成的覆盖率数据，并生成美观的 HTML 格式报告。作为 gcov 的前端工具，lcov 提供了更友好的用户界面和更强大的数据处理能力。

**核心价值**：
- 将 gcov 的文本数据转换为可视化 HTML 报告
- 提供覆盖率数据的收集、过滤、合并功能
- 支持多层次报告（目录、文件、函数、行）
- 方便 CI/CD 集成和团队协作

### 1.2 发展历史

| 年份 | 版本 | 重要特性 |
|------|------|----------|
| 2002 | 1.0 | 初始发布，基于 GCC gcov |
| 2005 | 1.4 | 增加分支覆盖率支持 |
| 2010 | 1.9 | 改进 HTML 输出样式 |
| 2016 | 1.12 | 支持 GCC 5+ 新格式 |
| 2018 | 1.13 | 增加深色主题 |
| 2020 | 1.14 | 改进 C++ 符号解码 |
| 2023 | 1.16 | 性能优化，支持大项目 |

### 1.3 核心特性

| 特性 | 描述 |
|------|------|
| HTML 报告 | 生成美观、可导航的 HTML 覆盖率报告 |
| 多维度统计 | 行覆盖率、函数覆盖率、分支覆盖率 |
| 数据过滤 | 支持移除特定路径或文件的数据 |
| 数据合并 | 合并多次测试的覆盖率数据 |
| 树状视图 | 目录层级结构的覆盖率展示 |
| 代码高亮 | 未覆盖代码醒目标记 |
| 主题支持 | 支持浅色/深色主题 |
| CI 友好 | 易于集成到持续集成流程 |

### 1.4 适用场景

| 场景 | 推荐度 | 说明 |
|------|--------|------|
| GCC 项目覆盖率可视化 | ★★★★★ | lcov 的主要应用场景 |
| CI/CD 自动报告生成 | ★★★★★ | 完美适配持续集成流程 |
| 团队代码质量展示 | ★★★★☆ | 生成可共享的 HTML 报告 |
| 多测试套件覆盖率合并 | ★★★★☆ | 支持数据合并功能 |
| Clang 项目 | ★★☆☆☆ | 部分支持，推荐使用 llvm-cov |
| 快速命令行查看 | ★☆☆☆☆ | 推荐直接使用 gcov |

### 1.5 对比分析

| 工具 | 优势 | 劣势 | 适用场景 |
|------|------|------|----------|
| lcov | HTML 报告美观、功能丰富、易用 | 仅支持 GCC/gcov | GCC 项目可视化 |
| gcov | 原生支持、无需额外安装 | 输出文本格式、无可视化 | 快速命令行查看 |
| llvm-cov | 支持 Clang/LLVM、功能强大 | 不支持 GCC | Clang 项目 |
| gcovr | 支持多种输出格式 | HTML 不如 lcov 美观 | 多格式需求 |
| codecov | 云端服务、PR 集成 | 需要网络、商业服务 | 开源项目 |

## 2. 安装与配置

### 2.1 多平台安装

**Linux (Debian/Ubuntu)**：
```bash
sudo apt update
sudo apt install lcov

# 验证安装
lcov --version
genhtml --version
```

**Linux (CentOS/RHEL/Fedora)**：
```bash
# CentOS/RHEL
sudo yum install lcov

# Fedora
sudo dnf install lcov

# 验证
lcov --version
```

**macOS**：
```bash
# 使用 Homebrew
brew install lcov

# 验证
lcov --version
```

**Windows**：
```bash
# 方案 1：WSL (推荐)
wsl --install -d Ubuntu
sudo apt install lcov

# 方案 2：MSYS2
pacman -S lcov

# 方案 3：Cygwin
# 通过 setup.exe 安装 lcov 包
```

### 2.2 版本管理

```bash
# 查看当前版本
lcov --version

# 从源码安装特定版本
wget https://github.com/linux-test-project/lcov/releases/download/v1.16/lcov-1.16.tar.gz
tar -xzf lcov-1.16.tar.gz
cd lcov-1.16
sudo make install

# 安装位置
which lcov    # /usr/bin/lcov
which genhtml # /usr/bin/genhtml
```

### 2.3 环境配置

**配置环境变量**：
```bash
# 添加到 ~/.bashrc 或 ~/.zshrc
export LCOV_HOME=/usr/local/lcov
export PATH=$LCOV_HOME/bin:$PATH

# 配置默认参数
alias lcov='lcov --rc lcov_branch_coverage=1'
alias genhtml='genhtml --rc lcov_branch_coverage=1'
```

**配置文件**：
```bash
# ~/.lcovrc 或项目根目录 .lcovrc
# 启用分支覆盖率
lcov_branch_coverage = 1

# 设置排除模式
lcov_excl_line = LCOV_EXCL_LINE
lcov_excl_br_line = LCOV_EXCL_BR_LINE

# 性能优化
genhtml_num_threads = 4
```

### 2.4 验证安装

```bash
# 检查版本
lcov --version
# 输出：lcov 1.16

# 检查 genhtml
genhtml --version
# 输出：genhtml 1.16

# 功能测试
echo 'int main() { return 0; }' > test.c
gcc --coverage test.c -o test
./test
lcov --capture --directory . --output-file test.info
genhtml test.info --output-directory test_html
ls test_html/index.html
# 清理
rm -rf test test.c test.info test_html *.gcno *.gcda
```

## 3. 基础使用

### 3.1 快速入门

**三步快速体验**：
```bash
# 1. 编译（启用覆盖率）
gcc --coverage hello.c -o hello

# 2. 运行并收集
./hello
lcov --capture --directory . --output-file coverage.info

# 3. 生成报告
genhtml coverage.info --output-directory report
open report/index.html
```

### 3.2 完整工作流程

```bash
# 步骤 1：编译程序（启用覆盖率选项）
gcc -fprofile-arcs -ftest-coverage program.c -o program
# 或使用简写
gcc --coverage program.c -o program

# 步骤 2：运行测试
./program

# 步骤 3：收集覆盖率数据
lcov --capture --directory . --output-file coverage.info

# 步骤 4：过滤不需要的数据（可选）
lcov --remove coverage.info '/usr/*' --output-file coverage.info

# 步骤 5：生成 HTML 报告
genhtml coverage.info --output-directory out

# 步骤 6：查看报告
open out/index.html        # macOS
xdg-open out/index.html    # Linux
start out/index.html       # Windows
```

### 3.3 基本命令详解

#### lcov 命令

**收集覆盖率（--capture）**：
```bash
# 基本收集
lcov --capture --directory . --output-file coverage.info

# 指定源码目录
lcov --capture --directory build \
     --source-directory src \
     --output-file coverage.info

# 追加到已有文件
lcov --capture --directory . \
     --output-file coverage.info \
     --add-tracefile existing.info

# 指定编译器类型
lcov --capture --directory . \
     --gcov-tool gcov \
     --output-file coverage.info
```

**过滤数据（--remove/--extract）**：
```bash
# 移除系统头文件（推荐）
lcov --remove coverage.info '/usr/*' --output-file coverage.info

# 移除测试文件
lcov --remove coverage.info '*/test/*' --output-file coverage.info

# 移除第三方库
lcov --remove coverage.info '*/third_party/*' --output-file coverage.info

# 只保留特定目录
lcov --extract coverage.info '*/src/*' --output-file coverage.info

# 多模式过滤
lcov --remove coverage.info \
     '/usr/*' \
     '*/test/*' \
     '*/third_party/*' \
     --output-file coverage.info
```

**合并数据（--add-tracefile）**：
```bash
# 合并两个文件
lcov --add-tracefile test1.info \
     --add-tracefile test2.info \
     --output-file total.info

# 合并多个文件
lcov --add-tracefile unit_test.info \
     --add-tracefile integration_test.info \
     --add-tracefile system_test.info \
     --output-file total.info

# 使用通配符
lcov --add-tracefile "*.info" --output-file total.info
```

**重置数据（--zerocounters）**：
```bash
# 清零计数器
lcov --zerocounters --directory .

# 清零指定目录
lcov --zerocounters --directory build
```

**查看摘要（--summary/--list）**：
```bash
# 查看摘要
lcov --summary coverage.info

# 列出详细内容
lcov --list coverage.info

# 列出特定文件
lcov --list coverage.info | grep "main.c"
```

#### genhtml 命令

**基本生成**：
```bash
# 生成 HTML 报告
genhtml coverage.info --output-directory out

# 设置标题
genhtml coverage.info --output-directory out \
       --title "My Project Coverage Report"

# 显示详细过程
genhtml coverage.info --output-directory out --verbose
```

**样式定制**：
```bash
# 使用深色主题
genhtml coverage.info --output-directory out --dark

# 启用所有可视化选项
genhtml coverage.info --output-directory out \
       --title "Coverage Report" \
       --legend \
       --highlight \
       --frame

# 显示函数和分支覆盖率
genhtml coverage.info --output-directory out \
       --show-details \
       --function-coverage \
       --branch-coverage

# C++ 符号解码
genhtml coverage.info --output-directory out --demangle-cpp
```

**输出选项表**：

| 选项 | 说明 | 示例 |
|------|------|------|
| `--title` | 设置报告标题 | `--title "Project Coverage"` |
| `--legend` | 显示图例说明 | `--legend` |
| `--highlight` | 高亮显示代码 | `--highlight` |
| `--frame` | 使用框架布局 | `--frame` |
| `--dark` | 深色主题 | `--dark` |
| `--show-details` | 显示详细覆盖率 | `--show-details` |
| `--function-coverage` | 显示函数覆盖率 | `--function-coverage` |
| `--branch-coverage` | 显示分支覆盖率 | `--branch-coverage` |
| `--demangle-cpp` | 解码 C++ 符号 | `--demangle-cpp` |
| `--ignore-errors` | 忽略错误继续生成 | `--ignore-errors source` |

### 3.4 常用操作流程

**清理并重新生成**：
```bash
# 完整清理流程
lcov --zerocounters --directory .
rm -f coverage.info
rm -rf coverage_html

# 重新收集
./run_tests.sh
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
genhtml coverage.info --output-directory coverage_html --legend --highlight
```

**增量覆盖率报告**：
```bash
# 基准覆盖率
lcov --capture --directory . --output-file baseline.info

# 运行更多测试
./more_tests.sh

# 新覆盖率
lcov --capture --directory . --output-file current.info

# 计算增量
lcov --subtract current.info baseline.info --output-file delta.info
genhtml delta.info --output-directory delta_html
```

## 4. 进阶特性

### 4.1 高级配置

#### CMake 项目集成

**完整 CMake 配置**：
```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(MyProject C CXX)

# 覆盖率选项
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(COVERAGE_HTML "Generate HTML coverage report" ON)

if(ENABLE_COVERAGE)
    # 编译选项
    add_compile_options(
        -fprofile-arcs
        -ftest-coverage
        -fPIC
    )
    
    # 链接选项
    add_link_options(--coverage)
    
    # 查找 lcov
    find_program(LCOV_PATH lcov)
    find_program(GENHTML_PATH genhtml)
    
    if(LCOV_PATH AND GENHTML_PATH)
        # 初始化覆盖率数据
        add_custom_target(coverage-init
            COMMAND ${LCOV_PATH} --zerocounters --directory ${CMAKE_BINARY_DIR}
            COMMENT "Zeroing coverage counters"
        )
        
        # 收集覆盖率数据
        add_custom_target(coverage-capture
            COMMAND ${LCOV_PATH} --capture 
                    --directory ${CMAKE_BINARY_DIR}
                    --output-file ${CMAKE_BINARY_DIR}/coverage.info
            DEPENDS ${CMAKE_PROJECT_NAME}
            COMMENT "Capturing coverage data"
        )
        
        # 过滤覆盖率数据
        add_custom_target(coverage-filter
            COMMAND ${LCOV_PATH} --remove 
                    ${CMAKE_BINARY_DIR}/coverage.info
                    '/usr/*'
                    '*/test/*'
                    '*/third_party/*'
                    --output-file ${CMAKE_BINARY_DIR}/coverage.info
            DEPENDS coverage-capture
            COMMENT "Filtering coverage data"
        )
        
        # 生成 HTML 报告
        if(COVERAGE_HTML)
            add_custom_target(coverage
                COMMAND ${GENHTML_PATH} 
                        ${CMAKE_BINARY_DIR}/coverage.info
                        --output-directory ${CMAKE_BINARY_DIR}/coverage_html
                        --title "${PROJECT_NAME} Coverage Report"
                        --legend
                        --highlight
                        --show-details
                        --function-coverage
                        --branch-coverage
                        --demangle-cpp
                DEPENDS coverage-filter
                COMMENT "Generating HTML coverage report"
            )
        else()
            add_custom_target(coverage
                DEPENDS coverage-filter
            )
        endif()
        
        # 打开报告（macOS/Linux）
        add_custom_target(coverage-open
            COMMAND open ${CMAKE_BINARY_DIR}/coverage_html/index.html
            DEPENDS coverage
            COMMENT "Opening coverage report"
        )
    else()
        message(WARNING "lcov or genhtml not found, coverage target disabled")
    endif()
endif()
```

**使用流程**：
```bash
# 配置并构建
cmake -B build -DENABLE_COVERAGE=ON
cmake --build build

# 运行测试
cd build
ctest --output-on-failure

# 生成覆盖率报告
make coverage

# 打开报告
make coverage-open  # 或 open coverage_html/index.html
```

#### Makefile 项目集成

```makefile
# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O0 -g --coverage
LDFLAGS = --coverage

# 源文件
SRCS = src/main.c src/utils.c src/parser.c
OBJS = $(SRCS:.c=.o)
TEST_SRCS = test/test_main.c test/test_utils.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

# 覆盖率文件
COVERAGE_DIR = coverage_html
COVERAGE_INFO = coverage.info

# 默认目标
all: myapp

# 构建应用
myapp: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# 构建测试
test_app: $(TEST_OBJS) $(filter-out src/main.o,$(OBJS))
	$(CC) $(LDFLAGS) $^ -o $@

# 运行测试
test: test_app
	./test_app

# 初始化覆盖率
coverage-init:
	lcov --zerocounters --directory .
	rm -f $(COVERAGE_INFO)
	rm -rf $(COVERAGE_DIR)

# 收集覆盖率
coverage-capture: test
	lcov --capture --directory . --output-file $(COVERAGE_INFO)

# 过滤覆盖率
coverage-filter: coverage-capture
	lcov --remove $(COVERAGE_INFO) '/usr/*' '*/test/*' --output-file $(COVERAGE_INFO)

# 生成报告
coverage: coverage-filter
	genhtml $(COVERAGE_INFO) \
		--output-directory $(COVERAGE_DIR) \
		--title "My Project Coverage" \
		--legend \
		--highlight \
		--show-details \
		--function-coverage \
		--branch-coverage

# 查看报告
coverage-view: coverage
	open $(COVERAGE_DIR)/index.html

# 清理
clean:
	rm -f $(OBJS) $(TEST_OBJS) myapp test_app
	rm -f *.gcno *.gcda *.gcov $(COVERAGE_INFO)
	rm -rf $(COVERAGE_DIR)

.PHONY: all test coverage-init coverage-capture coverage-filter coverage coverage-view clean
```

### 4.2 分支覆盖率

**启用分支覆盖率**：
```bash
# 编译时启用分支覆盖率
gcc -fprofile-arcs -ftest-coverage -fbranch-probabilities program.c

# 收集时启用
lcov --rc lcov_branch_coverage=1 \
     --capture --directory . \
     --output-file coverage.info

# 生成报告时显示
genhtml --rc lcov_branch_coverage=1 \
        --branch-coverage \
        coverage.info \
        --output-directory out
```

**配置文件设置**：
```bash
# ~/.lcovrc 或 .lcovrc
lcov_branch_coverage = 1
```

### 4.3 数据合并与差异分析

**合并多个测试结果**：
```bash
# 单元测试
./run_unit_tests.sh
lcov --capture --directory . --output-file unit.info

# 集成测试
./run_integration_tests.sh
lcov --capture --directory . --output-file integration.info

# 系统测试
./run_system_tests.sh
lcov --capture --directory . --output-file system.info

# 合并所有测试
lcov --add-tracefile unit.info \
     --add-tracefile integration.info \
     --add-tracefile system.info \
     --output-file total.info

# 过滤并生成报告
lcov --remove total.info '/usr/*' '*/test/*' --output-file total.info
genhtml total.info --output-directory coverage_html
```

**覆盖率差异分析**：
```bash
# 基准覆盖率
lcov --capture --directory . --output-file baseline.info

# 开发后新覆盖率
lcov --capture --directory . --output-file current.info

# 计算新增覆盖率
lcov --subtract current.info baseline.info --output-file added.info

# 计算减少的覆盖率
lcov --subtract baseline.info current.info --output-file removed.info

# 生成差异报告
genhtml added.info --output-directory added_html --title "Added Coverage"
genhtml removed.info --output-directory removed_html --title "Removed Coverage"
```

### 4.4 代码标记排除

**在代码中标记排除**：
```c
// 排除单行
int debug_code() {
    printf("Debug info\n"); // LCOV_EXCL_LINE
    
    // 排除代码块
    // LCOV_EXCL_START
    if (debug_mode) {
        log_debug("Debug information");
        dump_stack_trace();
    }
    // LCOV_EXCL_STOP
    
    return 0;
}

// 排除分支
if (rare_condition) { // LCOV_EXCL_BR_LINE
    handle_rare_case();
}
```

**排除标记表**：

| 标记 | 说明 |
|------|------|
| `LCOV_EXCL_LINE` | 排除当前行 |
| `LCOV_EXCL_START` | 开始排除块 |
| `LCOV_EXCL_STOP` | 结束排除块 |
| `LCOV_EXCL_BR_LINE` | 排除当前行的分支 |

## 5. 性能优化

### 5.1 大型项目优化

**多线程处理**：
```bash
# genhtml 多线程
genhtml coverage.info \
    --output-directory out \
    --num-threads 4

# 配置文件设置
echo "genhtml_num_threads = 4" >> ~/.lcovrc
```

**减少报告大小**：
```bash
# 不显示源代码
genhtml coverage.info --output-directory out --no-source

# 压缩报告
genhtml coverage.info --output-directory out
tar -czf coverage_report.tar.gz out/

# 只生成摘要
genhtml coverage.info --output-directory out --no-function-coverage
```

**选择性生成**：
```bash
# 只生成特定目录的报告
lcov --extract coverage.info '*/src/core/*' --output-file core.info
genhtml core.info --output-directory core_html
```

### 5.2 最佳实践

**推荐的覆盖率工作流**：
```bash
#!/bin/bash
# coverage.sh - 标准覆盖率脚本

set -e

COVERAGE_DIR="coverage_html"
COVERAGE_FILE="coverage.info"

echo "=== 开始覆盖率分析 ==="

# 1. 清理旧数据
echo "[1/6] 清理旧数据..."
lcov --zerocounters --directory . 2>/dev/null || true
rm -f "$COVERAGE_FILE"
rm -rf "$COVERAGE_DIR"

# 2. 编译
echo "[2/6] 编译项目..."
cmake -B build -DENABLE_COVERAGE=ON
cmake --build build

# 3. 运行测试
echo "[3/6] 运行测试..."
cd build
ctest --output-on-failure
cd ..

# 4. 收集覆盖率
echo "[4/6] 收集覆盖率数据..."
lcov --capture --directory build \
     --output-file "$COVERAGE_FILE" \
     --rc lcov_branch_coverage=1

# 5. 过滤数据
echo "[5/6] 过滤不需要的数据..."
lcov --remove "$COVERAGE_FILE" \
     '/usr/*' \
     '*/test/*' \
     '*/third_party/*' \
     '*/build/*' \
     --output-file "$COVERAGE_FILE" \
     --rc lcov_branch_coverage=1

# 6. 生成报告
echo "[6/6] 生成 HTML 报告..."
genhtml "$COVERAGE_FILE" \
        --output-directory "$COVERAGE_DIR" \
        --title "Coverage Report" \
        --legend \
        --highlight \
        --show-details \
        --function-coverage \
        --branch-coverage \
        --demangle-cpp \
        --num-threads 4

# 显示摘要
echo ""
echo "=== 覆盖率摘要 ==="
lcov --summary "$COVERAGE_FILE"

echo ""
echo "报告已生成: $COVERAGE_DIR/index.html"
```

**覆盖率目标设置**：
```bash
# 计算覆盖率百分比
COVERAGE=$(lcov --summary coverage.info 2>&1 | grep "lines" | awk '{print $2}')
echo "当前行覆盖率: $COVERAGE"

# 检查是否达到目标（例如 80%）
THRESHOLD=80.0
ACTUAL=$(echo "$COVERAGE" | tr -d '%')

if (( $(echo "$ACTUAL >= $THRESHOLD" | bc -l) )); then
    echo "✓ 覆盖率达标 ($COVERAGE >= $THRESHOLD%)"
    exit 0
else
    echo "✗ 覆盖率未达标 ($COVERAGE < $THRESHOLD%)"
    exit 1
fi
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：找不到 .gcda 文件**

```bash
# 错误信息
# geninfo: ERROR: cannot read .gcda file

# 原因：程序未运行或未生成覆盖率数据

# 解决方案
./my_program    # 先运行程序
lcov --capture --directory . --output-file coverage.info
```

**问题 2：覆盖率数据不完整**

```bash
# 错误信息
# lcov: WARNING: cannot find source file

# 原因：源文件路径不正确

# 解决方案：指定源码目录
lcov --capture --directory build \
     --source-directory src \
     --output-file coverage.info
```

**问题 3：分支覆盖率显示为 0%**

```bash
# 原因：未启用分支覆盖率

# 解决方案
# 编译时启用
gcc -fprofile-arcs -ftest-coverage program.c

# 收集时启用
lcov --rc lcov_branch_coverage=1 \
     --capture --directory . \
     --output-file coverage.info

# 生成报告时启用
genhtml --branch-coverage coverage.info --output-directory out
```

**问题 4：C++ 符号未解码**

```bash
# 原因：缺少 demangle 支持

# 解决方案
genhtml coverage.info --output-directory out --demangle-cpp

# 或确保安装了 c++filt
which c++filt
```

**问题 5：路径问题导致无法生成报告**

```bash
# 错误信息
# genhtml: ERROR: cannot read source file

# 解决方案：调整路径
lcov --capture --directory build \
     --source-directory $(pwd)/src \
     --output-file coverage.info

# 或使用绝对路径
lcov --capture --directory /abs/path/to/build \
     --source-directory /abs/path/to/src \
     --output-file coverage.info
```

### 6.2 调试技巧

**详细输出模式**：
```bash
# 启用详细日志
lcov --capture --directory . --output-file coverage.info --verbose

# 查看详细列表
lcov --list coverage.info

# 检查具体文件
lcov --list coverage.info | grep "main.c"
```

**验证数据完整性**：
```bash
# 检查 .gcno 和 .gcda 文件
find . -name "*.gcno" -o -name "*.gcda"

# 检查文件内容
gcov main.c

# 测试单个文件
lcov --capture --directory . --output-file test.info --test-name "single"
```

**忽略错误继续生成**：
```bash
# 忽略源文件缺失错误
genhtml coverage.info --output-directory out --ignore-errors source

# 忽略所有错误
genhtml coverage.info --output-directory out --ignore-errors all
```

## 7. 集成实践

### 7.1 CI/CD 集成

#### GitHub Actions 完整配置

```yaml
name: Coverage Report

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  coverage:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v4

    - name: Install dependencies
      run: |
        sudo apt update
        sudo apt install -y lcov

    - name: Configure
      run: cmake -B build -DENABLE_COVERAGE=ON

    - name: Build
      run: cmake --build build

    - name: Test
      run: |
        cd build
        ctest --output-on-failure

    - name: Capture coverage
      run: |
        lcov --capture --directory build \
             --output-file coverage.info \
             --rc lcov_branch_coverage=1

    - name: Filter coverage
      run: |
        lcov --remove coverage.info \
             '/usr/*' \
             '*/test/*' \
             '*/third_party/*' \
             --output-file coverage.info

    - name: Generate report
      run: |
        genhtml coverage.info \
                --output-directory coverage_html \
                --title "Coverage Report" \
                --legend \
                --highlight \
                --branch-coverage \
                --show-details

    - name: Upload coverage report
      uses: actions/upload-artifact@v4
      with:
        name: coverage-report
        path: coverage_html

    - name: Upload to Codecov (optional)
      uses: codecov/codecov-action@v4
      with:
        files: coverage.info
        flags: unittests
        name: codecov-umbrella

    - name: Coverage badge
      run: |
        COVERAGE=$(lcov --summary coverage.info 2>&1 | grep "lines" | awk '{print $2}')
        echo "COVERAGE=$COVERAGE" >> $GITHUB_ENV
        echo "Coverage: $COVERAGE"
```

#### GitLab CI 配置

```yaml
# .gitlab-ci.yml
stages:
  - test
  - coverage

test:
  stage: test
  image: gcc:latest
  script:
    - apt update && apt install -y lcov cmake
    - cmake -B build -DENABLE_COVERAGE=ON
    - cmake --build build
    - cd build && ctest --output-on-failure
  artifacts:
    paths:
      - build/
    expire_in: 1 hour

coverage:
  stage: coverage
  image: gcc:latest
  dependencies:
    - test
  script:
    - lcov --capture --directory build --output-file coverage.info
    - lcov --remove coverage.info '/usr/*' '*/test/*' --output-file coverage.info
    - genhtml coverage.info --output-directory coverage_html
  artifacts:
    paths:
      - coverage_html/
    expire_in: 30 days
  coverage: '/lines\.\.\.(\d+\.\d+%)/'
```

### 7.2 与其他工具集成

**与 CTest 集成**：
```cmake
# CMakeLists.txt
enable_testing()

# 添加测试
add_test(NAME UnitTest COMMAND test_runner)

# 覆盖率目标
add_custom_target(coverage
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --output-file coverage.info
    COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage_html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating coverage report"
)
```

**与 GDB 调试器集成**：
```bash
# 在 GDB 中运行覆盖率测试
gdb ./my_program
(gdb) run
# 程序结束后
(gdb) quit
lcov --capture --directory . --output-file coverage.info
```

**与 Valgrind 集成**：
```bash
# 同时运行内存检查和覆盖率
gcc -g --coverage program.c -o program
valgrind --leak-check=full ./program
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
```

### 7.3 实战案例

**案例：多模块项目覆盖率**

项目结构：
```
project/
├── src/
│   ├── core/
│   ├── utils/
│   └── network/
├── test/
│   ├── unit/
│   └── integration/
└── third_party/
```

覆盖率脚本：
```bash
#!/bin/bash
# coverage_all.sh - 多模块覆盖率分析

set -e

echo "=== 多模块覆盖率分析 ==="

# 清理
lcov --zerocounters --directory .
rm -f *.info
rm -rf coverage_html

# 运行单元测试
echo "运行单元测试..."
cd test/unit
./run_tests.sh
cd ../..
lcov --capture --directory . --output-file unit.info

# 运行集成测试
echo "运行集成测试..."
cd test/integration
./run_tests.sh
cd ../..
lcov --capture --directory . --output-file integration.info

# 合并覆盖率
echo "合并覆盖率数据..."
lcov --add-tracefile unit.info \
     --add-tracefile integration.info \
     --output-file total.info

# 分模块过滤
echo "生成模块报告..."
for module in core utils network; do
    echo "  - $module"
    lcov --extract total.info "*/src/$module/*" --output-file "$module.info"
    genhtml "$module.info" \
            --output-directory "coverage_$module" \
            --title "$module Coverage"
done

# 生成总体报告
echo "生成总体报告..."
lcov --remove total.info '/usr/*' '*/test/*' '*/third_party/*' --output-file final.info
genhtml final.info \
        --output-directory coverage_html \
        --title "Project Coverage" \
        --legend --highlight

echo "完成！查看报告：coverage_html/index.html"
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| lcov 官网 | https://lcov.readthedocs.io/ |
| GitHub 仓库 | https://github.com/linux-test-project/lcov |
| 手册页 | `man lcov` / `man genhtml` |
| GCC gcov 文档 | https://gcc.gnu.org/onlinedocs/gcc/Gcov.html |

### 8.2 相关工具

| 工具 | 用途 | 链接 |
|------|------|------|
| gcov | GCC 原生覆盖率工具 | GCC 内置 |
| llvm-cov | LLVM/Clang 覆盖率工具 | https://llvm.org/docs/CommandGuide/llvm-cov.html |
| gcovr | 多格式覆盖率报告 | https://gcovr.com/ |
| codecov | 云端覆盖率服务 | https://codecov.io/ |
| coveralls | 覆盖率托管平台 | https://coveralls.io/ |

### 8.3 学习路径

**入门路径**：
1. 理解代码覆盖率基本概念（行覆盖、分支覆盖、函数覆盖）
2. 学习 gcov 基础用法
3. 掌握 lcov 的收集、过滤、生成流程
4. 集成到项目构建系统（CMake/Makefile）
5. 配置 CI/CD 自动生成报告

**进阶路径**：
1. 多测试套件覆盖率合并
2. 覆盖率差异分析
3. 性能优化（大项目处理）
4. 自定义报告样式
5. 与代码质量平台集成

**最佳实践**：
1. 设置覆盖率阈值（推荐 70-90%）
2. 定期审查未覆盖代码
3. CI 中强制覆盖率检查
4. 排除不必要代码（测试、第三方库）
5. 结合静态分析工具使用

### 8.4 常用命令速查

```bash
# 快速流程
gcc --coverage program.c -o program
./program
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory out

# 清理
lcov --zerocounters --directory .
rm -f coverage.info *.gcno *.gcda

# 过滤
lcov --remove coverage.info '/usr/*' '*/test/*' --output-file coverage.info

# 合并
lcov --add-tracefile test1.info --add-tracefile test2.info --output-file total.info

# 查看摘要
lcov --summary coverage.info

# 详细列表
lcov --list coverage.info
```