# gcov - GCC 代码覆盖率工具

## 1. 概述与背景

### 1.1 工具定位

gcov 是 GCC（GNU Compiler Collection）工具链提供的代码覆盖率分析工具，用于分析程序的代码执行覆盖率。它能够测量程序在测试过程中哪些代码被执行、执行了多少次，帮助开发者评估测试的完整性，发现未测试的代码路径。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1990 | GCC 1.0 | 早期覆盖率支持 |
| 1999 | GCC 2.95 | 改进的覆盖率输出格式 |
| 2001 | GCC 3.0 | 引入 `.gcno/.gcda` 文件格式 |
| 2005 | GCC 4.0 | 增强分支覆盖率支持 |
| 2008 | GCC 4.3 | 支持函数级别覆盖率 |
| 2015 | GCC 5.0 | 改进多线程支持 |
| 2018 | GCC 8.0 | 优化性能和准确性 |
| 2021 | GCC 11.0 | 改进 C++ 模板覆盖率 |

### 1.3 核心特性

- **行覆盖率分析**：统计每行代码的执行次数
- **分支覆盖率分析**：追踪条件分支的执行情况
- **函数覆盖率统计**：记录函数调用次数和返回率
- **低性能开销**：覆盖率检测开销约 10-50%
- **无缝 GCC 集成**：作为 GCC 标准工具链一部分
- **多种输出格式**：支持文本、XML 等格式
- **可与其他工具配合**：与 lcov、genhtml 等工具协同

### 1.4 适用场景

| 场景 | 描述 | 适用度 |
|------|------|--------|
| 单元测试覆盖率 | 分析单元测试覆盖的代码比例 | ★★★★★ |
| 集成测试验证 | 验证集成测试是否覆盖关键路径 | ★★★★☆ |
| CI/CD 质量门禁 | 自动化覆盖率检查和阈值设置 | ★★★★★ |
| 代码审查辅助 | 识别未测试的危险代码段 | ★★★★☆ |
| 性能热点分析 | 识别频繁执行的代码路径 | ★★★☆☆ |
| 死代码检测 | 发现从未执行的代码段 | ★★★★☆ |

### 1.5 对比分析

| 工具 | 编译器 | 优势 | 劣势 |
|------|--------|------|------|
| gcov | GCC | GCC 原生支持、功能完整、稳定 | 仅支持 GCC |
| lcov | GCC | 生成 HTML 报告、可视化好 | 依赖 gcov |
| llvm-cov | Clang/LLVM | 支持 Clang、现代化 | 不支持 GCC |
| Gcovr | 通用 | 多格式输出、CI 友好 | 额外安装 |
| JaCoCo | Java | Java 生态标准 | 仅限 Java |
| Istanbul | Node.js | JS 生态集成 | 仅限 Node.js |

## 2. 安装与配置

### 2.1 多平台安装

gcov 随 GCC 工具链安装，无需单独安装。

```bash
# Debian/Ubuntu
sudo apt update
sudo apt install gcc g++

# CentOS/RHEL
sudo yum install gcc gcc-c++

# Fedora
sudo dnf install gcc gcc-c++

# macOS (通过 Homebrew)
brew install gcc

# Arch Linux
sudo pacman -S gcc
```

### 2.2 版本管理

```bash
# 验证安装
gcov --version

# 输出示例
gcov (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.

# 查看支持的覆盖率选项
gcc --help=optimizers | grep coverage
```

### 2.3 环境配置

```bash
# 设置覆盖率数据输出目录（可选）
export GCOV_PREFIX=/path/to/coverage/data

# 设置路径前缀（可选）
export GCOV_PREFIX_STRIP=0

# 在 .bashrc 或 .zshrc 中添加
echo 'export GCOV_PREFIX=$HOME/coverage_data' >> ~/.bashrc
```

### 2.4 验证安装

```bash
# 检查 gcov 可执行文件
which gcov
# 输出: /usr/bin/gcov

# 检查 GCC 版本
gcc --version

# 测试覆盖率功能
cat > test.c << 'EOF'
#include <stdio.h>
int main() {
    printf("Hello, gcov!\n");
    return 0;
}
EOF

gcc --coverage test.c -o test
./test
gcov test.c
# 预期输出: File 'test.c' Lines executed:100.00%
```

## 3. 基础使用

### 3.1 快速入门

**完整覆盖率测试流程：**

```bash
# 步骤 1: 编译启用覆盖率
gcc -fprofile-arcs -ftest-coverage program.c -o program

# 步骤 2: 运行程序
./program

# 步骤 3: 生成覆盖率报告
gcov program.c

# 步骤 4: 查看报告
cat program.c.gcov
```

**输出示例：**

```
File 'program.c'
Lines executed:85.71% of 14
Branches executed:100.00% of 6
Taken at least once:66.67% of 6
Calls executed:100.00% of 4
Creating 'program.c.gcov'
```

### 3.2 项目结构

**推荐的项目目录结构：**

```
myproject/
├── src/
│   ├── main.c
│   ├── utils.c
│   └── utils.h
├── tests/
│   ├── test_main.c
│   └── test_utils.c
├── build/
│   └── coverage/          # 覆盖率数据目录
├── Makefile
└── CMakeLists.txt
```

### 3.3 基本命令

**编译选项详解：**

```bash
# 方式 1: 分别指定选项
gcc -fprofile-arcs -ftest-coverage program.c -o program

# 方式 2: 使用 --coverage 宏（推荐）
gcc --coverage program.c -o program

# 方式 3: C++ 程序
g++ --coverage program.cpp -o program

# 方式 4: 指定覆盖率数据目录
gcc -fprofile-arcs -ftest-coverage \
    -fprofile-dir=./coverage \
    program.c -o program

# 方式 5: 指定路径前缀（用于容器/虚拟机环境）
gcc --coverage \
    -fprofile-prefix-path=/host/path \
    program.c -o program
```

### 3.4 常用操作

**生成覆盖率报告：**

```bash
# 基本报告
gcov program.c

# 显示分支覆盖率
gcov -b program.c

# 显示函数覆盖率
gcov -f program.c

# 显示所有信息
gcov -a -b -f program.c

# 多文件项目
gcov main.c utils.c helper.c
```

**解读覆盖率文件：**

```bash
# 生成的文件
ls -la *.gc*
# program.c.gcov  # 文本报告
# program.gcno    # 编译时生成的注释文件
# program.gcda    # 运行时生成的数据文件
```

**查看详细报告：**

```bash
# 查看带源码的覆盖率
cat program.c.gcov

# 示例输出
        -:    0:Source:program.c
        -:    0:Graph:program.gcno
        -:    0:Data:program.gcda
        -:    0:Runs:1
        -:    0:Programs:1
        -:    1:#include <stdio.h>
        -:    2:
function main called 1 returned 100% blocks executed 85%
        1:    3:int main(int argc, char *argv[]) {
        1:    4:    if (argc > 1) {
branch  0 taken 0% (fallthrough)
branch  1 taken 100%
    #####:    5:        printf("Args: %s\n", argv[1]);
        -:    6:    } else {
        1:    7:        printf("No args\n");
        -:    8:    }
        1:    9:    return 0;
        -:   10:}
```

## 4. 进阶特性

### 4.1 高级配置

**编译选项详表：**

| 选项 | 说明 | 示例 |
|------|------|------|
| `-fprofile-arcs` | 记录程序执行路径（分支信息） | `gcc -fprofile-arcs test.c` |
| `-ftest-coverage` | 生成 `.gcno` 文件（源码注释） | `gcc -ftest-coverage test.c` |
| `--coverage` | 等同于上面两个选项 + 链接选项 | `gcc --coverage test.c` |
| `-fprofile-dir=dir` | 指定覆盖率数据输出目录 | `gcc --coverage -fprofile-dir=./cov` |
| `-fprofile-prefix-path=dir` | 指定路径前缀（用于路径重映射） | `gcc --coverage -fprofile-prefix-path=/src` |
| `-fprofile-update=atomic` | 多线程安全更新覆盖率数据 | `gcc --coverage -fprofile-update=atomic` |

**gcov 输出选项：**

| 选项 | 说明 | 输出示例 |
|------|------|----------|
| `-a` | 显示基本块信息 | `block 1` |
| `-b` | 显示分支覆盖率 | `branch 0 taken 5` |
| `-c` | 显示调用图覆盖率 | `call 0 returned 100%` |
| `-f` | 显示函数覆盖率 | `function main called 1` |
| `-l` | 添加源码行号前缀 | `1:    5:printf(...)` |
| `-m` | 显示无条件分支 | `unconditional 0 taken 1` |
| `-n` | 不生成 `.gcov` 文件 | 仅显示摘要 |
| `-o dir` | 指定 `.gcno/.gcda` 文件目录 | `gcov -o build test.c` |
| `-u` | 显示无条件分支（同 -m） | - |
| `-x` | 输出 XML 格式 | `<gcov ...>` |

### 4.2 扩展功能

**多文件项目覆盖率：**

```bash
# 编译所有源文件
gcc --coverage -c main.c -o main.o
gcc --coverage -c utils.c -o utils.o
gcc --coverage -c helper.c -o helper.o
gcc --coverage main.o utils.o helper.o -o myapp

# 运行程序
./myapp

# 生成所有文件的覆盖率报告
gcov -b main.c utils.c helper.c
```

**合并多次测试的覆盖率数据：**

```bash
# 方法 1: 运行多次测试（自动合并）
./myapp test1
./myapp test2
./myapp test3
gcov -b *.c

# 方法 2: 使用 -a 选项合并不同的 .gcda 文件
gcov -a test1.gcda test2.gcda program.c
```

**重置覆盖率数据：**

```bash
# 删除 .gcda 文件重置数据
rm -f *.gcda

# 重新运行测试
./myapp
gcov program.c
```

### 4.3 与 lcov 配合生成可视化报告

```bash
# 安装 lcov
sudo apt install lcov

# 步骤 1: 初始化覆盖率数据
lcov --capture --initial --directory . --output-file coverage_base.info

# 步骤 2: 运行测试
./myapp

# 步骤 3: 捕获覆盖率数据
lcov --capture --directory . --output-file coverage.info

# 步骤 4: 合并基础数据
lcov --add-tracefile coverage_base.info \
     --add-tracefile coverage.info \
     --output-file coverage_total.info

# 步骤 5: 过滤系统头文件
lcov --remove coverage_total.info '/usr/*' \
     --output-file coverage_filtered.info

# 步骤 6: 生成 HTML 报告
genhtml coverage_filtered.info --output-directory coverage_html

# 步骤 7: 查看报告
firefox coverage_html/index.html
```

## 5. 性能优化

### 5.1 调优策略

**性能开销分析：**

| 覆盖率级别 | 性能开销 | 内存开销 | 适用场景 |
|-----------|---------|---------|---------|
| 仅行覆盖率 | 10-20% | 低 | 开发阶段快速反馈 |
| 行+分支覆盖率 | 20-30% | 中 | CI/CD 持续集成 |
| 行+分支+调用覆盖率 | 30-40% | 中高 | 发布前全面验证 |
| 完整覆盖率 + 多线程安全 | 40-50% | 高 | 生产环境覆盖率 |

**性能优化技巧：**

```bash
# 1. 使用单独的覆盖率构建目录
mkdir -p build/coverage
cd build/coverage
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON ../..
make

# 2. 仅对需要测试的文件启用覆盖率
gcc -c main.c -o main.o          # 不启用覆盖率
gcc --coverage -c utils.c -o utils.o  # 启用覆盖率

# 3. 多线程程序使用原子更新
gcc --coverage -fprofile-update=atomic -pthread app.c -o app

# 4. 分离覆盖率数据收集和报告生成
# 编译时指定目录
gcc --coverage -fprofile-dir=/tmp/cov_data app.c -o app
./app
# 报告生成时指定目录
gcov -o /tmp/cov_data app.c
```

### 5.2 最佳实践

**实践 1：分离构建目录**

```bash
# 推荐的项目结构
project/
├── src/
├── build/
│   ├── debug/        # 普通调试构建
│   └── coverage/     # 覆盖率构建
└── tests/
```

**实践 2：运行所有测试后生成报告**

```bash
# 错误做法：每次测试后都生成报告
./test_unit
gcov *.c
rm *.gcda

./test_integration
gcov *.c
rm *.gcda

# 正确做法：运行所有测试后统一生成
./test_unit
./test_integration
./test_e2e
gcov *.c  # 包含所有测试的覆盖率
```

**实践 3：使用 lcov 生成可视化报告**

```bash
# 一键生成 HTML 报告的脚本
#!/bin/bash
# coverage_report.sh

# 清理旧数据
rm -f *.gcda *.gcov coverage.info coverage_html -rf

# 捕获初始数据
lcov --capture --initial --directory . --output-file coverage_base.info

# 运行所有测试
./run_all_tests.sh

# 捕获测试数据
lcov --capture --directory . --output-file coverage_test.info

# 合并数据
lcov --add-tracefile coverage_base.info \
     --add-tracefile coverage_test.info \
     --output-file coverage.info

# 过滤不需要的文件
lcov --remove coverage.info '/usr/*' '*/test/*' \
     --output-file coverage_filtered.info

# 生成 HTML
genhtml coverage_filtered.info \
        --output-directory coverage_html \
        --title "Project Coverage Report" \
        --legend \
        --show-details

echo "Coverage report generated: coverage_html/index.html"
```

**实践 4：CI/CD 集成**

```yaml
# .github/workflows/coverage.yml
name: Coverage

on: [push, pull_request]

jobs:
  coverage:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

      - name: Install dependencies
        run: |
          sudo apt update
          sudo apt install -y gcc lcov

      - name: Build with coverage
        run: |
          mkdir build && cd build
          cmake -DENABLE_COVERAGE=ON ..
          make

      - name: Run tests
        run: |
          cd build
          ctest --output-on-failure

      - name: Generate coverage report
        run: |
          cd build
          lcov --capture --directory . --output-file coverage.info
          lcov --remove coverage.info '/usr/*' --output-file coverage_filtered.info
          genhtml coverage_filtered.info --output-directory coverage_html

      - name: Upload coverage
        uses: actions/upload-artifact@v3
        with:
          name: coverage-report
          path: build/coverage_html/
```

**实践 5：设置覆盖率阈值**

```bash
# 在 Makefile 中添加覆盖率检查
COVERAGE_THRESHOLD=80

coverage-check: coverage
    @coverage=$$(grep "Lines executed" program.c.gcov | grep -oP '\d+\.\d+'); \
    if (( $$(echo "$$coverage < $(COVERAGE_THRESHOLD)" | bc -l) )); then \
        echo "Coverage $$coverage% is below threshold $(COVERAGE_THRESHOLD)%"; \
        exit 1; \
    else \
        echo "Coverage $$coverage% meets threshold $(COVERAGE_THRESHOLD)%"; \
    fi
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：找不到 .gcda 文件**

```bash
# 现象
gcov: cannot open graph file program.gcno
gcov: cannot open data file program.gcda

# 原因：未运行程序或程序异常退出
# 解决：确保程序正常执行
./program
ls -la *.gcda  # 确认文件存在
```

**问题 2：路径不匹配**

```bash
# 现象
gcov: source file path mismatch

# 原因：编译路径和运行路径不一致
# 解决：使用 -o 选项指定目录
gcov -o build/coverage program.c
```

**问题 3：多线程程序覆盖率不准确**

```bash
# 现象：覆盖率数据丢失或重复
# 原因：多线程同时写入 .gcda 文件

# 解决：使用原子更新选项
gcc --coverage -fprofile-update=atomic -pthread app.c -o app
```

**问题 4：覆盖率数据过大**

```bash
# 现象：.gcda 文件过大影响性能
# 解决：定期清理或分离测试

# 方法 1: 定期清理
find . -name "*.gcda" -mtime +7 -delete

# 方法 2: 分离测试套件
./test_unit    # 生成单元测试覆盖率
mv *.gcda unit/
./test_e2e     # 生成集成测试覆盖率
mv *.gcda e2e/
```

**问题 5：符号链接问题**

```bash
# 现象：通过符号链接编译时覆盖率不工作
# 解决：使用绝对路径或禁用符号链接解析

# 方法 1: 使用绝对路径
gcc --coverage $(realpath src/main.c) -o app

# 方法 2: 设置 GCOV_PREFIX_STRIP
export GCOV_PREFIX_STRIP=0
```

### 6.2 调试技巧

**与 GDB 配合调试：**

```bash
# 启用覆盖率并调试编译
gcc --coverage -g program.c -o program

# 使用 GDB 调试
gdb ./program
(gdb) run
(gdb) quit

# 生成覆盖率报告
gcov program.c
```

**查看详细覆盖率信息：**

```bash
# 查看所有覆盖率信息
gcov -a -b -f -l program.c

# 查看特定函数的覆盖率
gcov -f program.c | grep -A 5 "function_name"

# 查看未覆盖的代码行
grep "#####" program.c.gcov
```

**分析覆盖率文件：**

```bash
# 查看 .gcno 文件（编译时生成）
gcov-dump program.gcno

# 查看 .gcda 文件（运行时生成）
gcov-dump program.gcda

# 对比两个 .gcda 文件
gcov-dump test1.gcda > dump1.txt
gcov-dump test2.gcda > dump2.txt
diff dump1.txt dump2.txt
```

## 7. 集成实践

### 7.1 CMake 集成

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(MyProject C)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Coverage enabled for GCC")
        add_compile_options(-fprofile-arcs -ftest-coverage)
        add_link_options(-lgcov --coverage)

        # 设置覆盖率数据输出目录
        set(COVERAGE_DIR ${CMAKE_BINARY_DIR}/coverage)
        file(MAKE_DIRECTORY ${COVERAGE_DIR})
    else()
        message(WARNING "Coverage only supported with GCC, current: ${CMAKE_C_COMPILER_ID}")
    endif()
endif()

# 添加源文件
add_executable(myapp src/main.c src/utils.c)

# 添加覆盖率目标
if(ENABLE_COVERAGE)
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}
        COMMAND gcov -b -o ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src/*.c
        WORKING_DIRECTORY ${COVERAGE_DIR}
        COMMENT "Generating coverage report"
    )

    # 添加 lcov 目标
    find_program(LCOV_PATH lcov)
    find_program(GENHTML_PATH genhtml)

    if(LCOV_PATH AND GENHTML_PATH)
        add_custom_target(coverage-html
            COMMAND ${LCOV_PATH} --capture --initial --directory ${CMAKE_BINARY_DIR} --output-file coverage_base.info
            COMMAND ${LCOV_PATH} --capture --directory ${CMAKE_BINARY_DIR} --output-file coverage.info
            COMMAND ${LCOV_PATH} --add-tracefile coverage_base.info --add-tracefile coverage.info --output-file coverage_total.info
            COMMAND ${LCOV_PATH} --remove coverage_total.info '/usr/*' '*/test/*' --output-file coverage_filtered.info
            COMMAND ${GENHTML_PATH} coverage_filtered.info --output-directory ${COVERAGE_DIR}/html
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating HTML coverage report"
        )
    endif()
endif()
```

**使用方法：**

```bash
# 配置项目
cmake -B build -DENABLE_COVERAGE=ON

# 构建
cmake --build build

# 运行测试
cd build && ctest

# 生成覆盖率报告
cmake --build build --target coverage
# 或生成 HTML 报告
cmake --build build --target coverage-html
```

### 7.2 Makefile 集成

```makefile
# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

# 源文件
SRCS = src/main.c src/utils.c
OBJS = $(SRCS:.c=.o)
TARGET = myapp

# 覆盖率选项
ifdef COVERAGE
    CFLAGS += --coverage
    LDFLAGS += --coverage
    COVERAGE_DIR = coverage
endif

# 默认目标
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 运行测试
test: $(TARGET)
	./$(TARGET)

# 生成覆盖率报告
coverage: clean
	$(MAKE) COVERAGE=1 $(TARGET)
	./$(TARGET)
	mkdir -p $(COVERAGE_DIR)
	gcov -b -o . $(SRCS)
	mv *.gcov $(COVERAGE_DIR)/

# 生成 HTML 覆盖率报告
coverage-html: coverage
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' --output-file coverage_filtered.info
	genhtml coverage_filtered.info --output-directory $(COVERAGE_DIR)/html

# 清理
clean:
	rm -f $(OBJS) $(TARGET) *.gcno *.gcda *.gcov coverage.info

# 完全清理
distclean: clean
	rm -rf $(COVERAGE_DIR)

.PHONY: test coverage coverage-html clean distclean
```

**使用方法：**

```bash
# 普通构建
make

# 覆盖率构建
make coverage

# HTML 报告
make coverage-html

# 清理
make clean
```

### 7.3 实战案例

**案例 1：测试驱动开发（TDD）工作流**

```bash
# 1. 编写测试用例
cat > test_calculator.c << 'EOF'
#include <assert.h>
#include "calculator.h"

int main() {
    assert(add(2, 3) == 5);
    assert(subtract(5, 2) == 3);
    assert(multiply(3, 4) == 12);
    assert(divide(10, 2) == 5);
    return 0;
}
EOF

# 2. 实现功能（初始失败）
cat > calculator.c << 'EOF'
int add(int a, int b) { return 0; }
int subtract(int a, int b) { return 0; }
int multiply(int a, int b) { return 0; }
int divide(int a, int b) { return 0; }
EOF

# 3. 运行测试查看覆盖率
gcc --coverage test_calculator.c calculator.c -o test
./test || true
gcov -b calculator.c
# 覆盖率: 100% 但断言失败

# 4. 实现正确功能
cat > calculator.c << 'EOF'
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }
EOF

# 5. 重新测试
rm *.gcda *.gcov
gcc --coverage test_calculator.c calculator.c -o test
./test
gcov -b calculator.c
# 覆盖率: 100%，所有测试通过
```

**案例 2：持续集成覆盖率检查**

```bash
#!/bin/bash
# ci_coverage_check.sh

set -e

# 配置
COVERAGE_THRESHOLD=80
PROJECT_DIR=$(pwd)
BUILD_DIR=$PROJECT_DIR/build
COVERAGE_DIR=$BUILD_DIR/coverage

echo "=== 开始覆盖率检查 ==="
echo "阈值: ${COVERAGE_THRESHOLD}%"

# 1. 清理旧数据
echo "清理旧构建..."
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

# 2. 配置和构建
echo "配置项目（启用覆盖率）..."
cd $BUILD_DIR
cmake -DENABLE_COVERAGE=ON ..
make -j$(nproc)

# 3. 运行测试
echo "运行测试..."
ctest --output-on-failure

# 4. 生成覆盖率报告
echo "生成覆盖率报告..."
mkdir -p $COVERAGE_DIR
lcov --capture --initial --directory . --output-file coverage_base.info
lcov --capture --directory . --output-file coverage_test.info
lcov --add-tracefile coverage_base.info \
     --add-tracefile coverage_test.info \
     --output-file coverage_total.info
lcov --remove coverage_total.info '/usr/*' '*/test/*' \
     --output-file coverage_filtered.info
genhtml coverage_filtered.info --output-directory $COVERAGE_DIR/html

# 5. 检查覆盖率阈值
echo "检查覆盖率阈值..."
TOTAL_COVERAGE=$(lcov --summary coverage_filtered.info 2>&1 | \
                 grep "lines" | grep -oP '\d+\.\d+' | head -1)

echo "总覆盖率: ${TOTAL_COVERAGE}%"

if (( $(echo "$TOTAL_COVERAGE < $COVERAGE_THRESHOLD" | bc -l) )); then
    echo "❌ 覆盖率 ${TOTAL_COVERAGE}% 低于阈值 ${COVERAGE_THRESHOLD}%"
    exit 1
else
    echo "✅ 覆盖率 ${TOTAL_COVERAGE}% 达到阈值 ${COVERAGE_THRESHOLD}%"
fi

echo "=== 覆盖率检查完成 ==="
echo "HTML 报告: $COVERAGE_DIR/html/index.html"
```

**案例 3：多模块项目覆盖率**

```bash
# 项目结构
project/
├── core/
│   ├── core.c
│   └── core.h
├── utils/
│   ├── utils.c
│   └── utils.h
├── tests/
│   ├── test_core.c
│   └── test_utils.c
└── Makefile

# Makefile
CC = gcc
CFLAGS = --coverage -Wall

.PHONY: all test coverage

all:
	$(CC) $(CFLAGS) -c core/core.c -o core/core.o
	$(CC) $(CFLAGS) -c utils/utils.c -o utils/utils.o
	$(CC) $(CFLAGS) tests/test_core.c core/core.o -o tests/test_core
	$(CC) $(CFLAGS) tests/test_utils.c utils/utils.o -o tests/test_utils

test: all
	./tests/test_core
	./tests/test_utils

coverage: test
	gcov -b core/core.c
	gcov -b utils/utils.c
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_html
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 | 说明 |
|------|------|------|
| GCC gcov 文档 | https://gcc.gnu.org/onlinedocs/gcc/Gcov.html | 官方完整文档 |
| GCC 覆盖率选项 | https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html | 编译选项详解 |
| lcov 手册 | http://ltp.sourceforge.net/coverage/lcov.php | lcov 使用指南 |
| gcov-dump 工具 | https://gcc.gnu.org/onlinedocs/gcc/Gcov-Dump.html | 文件分析工具 |

### 8.2 学习路径

**初级阶段：**
1. 理解代码覆盖率基本概念
2. 掌握 gcov 基本命令和选项
3. 学会解读覆盖率报告
4. 在简单项目中应用覆盖率测试

**中级阶段：**
1. 掌握 lcov 生成可视化报告
2. 学习覆盖率指标分析（行、分支、函数）
3. CI/CD 集成覆盖率检查
4. 设置覆盖率阈值和门禁

**高级阶段：**
1. 多模块、多测试套件覆盖率合并
2. 性能优化和开销控制
3. 自定义覆盖率报告脚本
4. 与其他质量工具集成（SonarQube、Codecov）

### 8.3 相关工具

| 工具 | 用途 | 链接 |
|------|------|------|
| lcov | HTML 可视化报告 | http://ltp.sourceforge.net/coverage/lcov.php |
| gcovr | 多格式输出 | https://gcovr.com/ |
| codecov | 云端覆盖率服务 | https://codecov.io/ |
| SonarQube | 代码质量管理 | https://www.sonarqube.org/ |
| Coveralls | 云端覆盖率 | https://coveralls.io/ |

### 8.4 最佳实践总结

1. **分离构建**：使用独立的覆盖率构建目录
2. **完整测试**：运行所有测试用例后生成报告
3. **可视化**：使用 lcov 生成 HTML 报告
4. **自动化**：CI/CD 集成覆盖率检查
5. **阈值控制**：设置项目覆盖率目标（如 80%）
6. **持续改进**：定期审查未覆盖代码，补充测试
7. **性能考虑**：生产环境不启用覆盖率编译
8. **团队协作**：覆盖率报告纳入代码审查流程