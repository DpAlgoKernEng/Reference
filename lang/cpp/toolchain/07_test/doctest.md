# doctest - 最快的 C++ 测试框架

## 1. 概述与背景

### 1.1 工具定位

doctest 是一个轻量级、高性能的 C++ 测试框架，核心设计理念是"编译速度最快"。它采用单头文件设计，无任何外部依赖，非常适合需要快速迭代开发的 C++ 项目。

作为现代 C++ 测试框架之一，doctest 在保持功能完整性的同时，将编译速度作为首要优化目标，相比 GoogleTest 和 Catch2 有显著的编译时间优势。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2016 | 1.0 | 首次发布，单头文件设计 |
| 2018 | 2.0 | 引入 SUBCASE 机制，BDD 风格支持 |
| 2019 | 2.2 | 添加自定义报告器，性能优化 |
| 2020 | 2.3 | 参数化测试生成器，并行测试支持 |
| 2021 | 2.4 | C++20 支持，编译时间进一步优化 |
| 2022 | 2.4.11 | 稳定版本，全面支持 C++11-C++20 |

### 1.3 核心特性

| 特性 | 描述 |
|------|------|
| **最快编译** | 编译速度是 Catch2 的 2-3 倍，GoogleTest 的 5-10 倍 |
| **单头文件** | 仅需包含 `doctest.h`，集成简单 |
| **零依赖** | 无外部库依赖，减少构建复杂性 |
| **SUBCASE** | 独特的子测试机制，代码复用性强 |
| **BDD 风格** | 支持 GIVEN/WHEN/THEN 模式 |
| **参数化测试** | GENERATE 宏支持灵活的数据驱动测试 |
| **异常测试** | 完整的异常断言支持 |
| **自定义报告器** | 可扩展测试输出格式 |

### 1.4 适用场景

| 场景 | 推荐 | 理由 |
|------|------|------|
| 编译速度优先项目 | ✅ | 编译时间显著低于其他框架 |
| 单头文件需求 | ✅ | 仅一个头文件，集成极简 |
| 快速原型开发 | ✅ | 无需复杂配置，立即开始测试 |
| CI/CD 快速构建 | ✅ | 减少持续集成构建时间 |
| 嵌入式开发 | ✅ | 轻量级，适合资源受限环境 |
| 大型项目测试 | ⚠️ | 适合，但 Mock 功能需外部支持 |
| 需要 Mock 功能 | ❌ | 不内置 Mock，需配合其他库 |

### 1.5 对比分析

#### 编译速度对比

| 框架 | 编译时间（相对） | 头文件数 | 依赖 |
|------|-----------------|----------|------|
| doctest | 1x（基准） | 1 | 无 |
| Catch2 | 2-3x | 1 | 无 |
| GoogleTest | 5-10x | 多 | 无 |

#### 功能特性对比

| 特性 | doctest | Catch2 | GoogleTest |
|------|---------|--------|------------|
| 编译速度 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ |
| 单头文件 | ✅ | ✅ | ❌ |
| Mock 支持 | ❌ | ❌ | ✅ |
| BDD 风格 | ✅ | ✅ | ❌ |
| 参数化测试 | GENERATE | GENERATE | TEST_P |
| 子测试 | SUBCASE | SECTION | ❌ |
| 自定义报告器 | ✅ | ✅ | ✅ |
| 并行测试 | ✅ | ✅ | ✅ |
| 学习曲线 | 低 | 中 | 中 |

## 2. 安装与配置

### 2.1 多平台安装

#### 方式一：单头文件（推荐）

```bash
# 下载到项目目录
wget https://github.com/doctest/doctest/releases/download/v2.4.11/doctest.h

# 或使用 curl
curl -L -o doctest.h https://github.com/doctest/doctest/releases/download/v2.4.11/doctest.h
```

#### 方式二：vcpkg 包管理器

```bash
# 安装 vcpkg（如果未安装）
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh

# 安装 doctest
./vcpkg install doctest

# CMake 中使用
cmake -B build -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
```

#### 方式三：CMake FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest
    GIT_TAG v2.4.11
)

FetchContent_MakeAvailable(doctest)
```

### 2.2 版本管理

| 版本 | C++ 标准 | 特性 |
|------|----------|------|
| 2.4.x | C++11+ | 推荐稳定版 |
| 2.3.x | C++11+ | 并行测试支持 |
| 2.2.x | C++11+ | 自定义报告器 |
| 开发版 | C++11+ | 最新功能，可能不稳定 |

```bash
# 查看版本
grep "DOCTEST_VERSION" doctest.h

# 使用特定版本标签
git clone --branch v2.4.11 https://github.com/doctest/doctest
```

### 2.3 环境配置

#### CMake 项目配置

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProject)

# 方式一：单头文件
add_executable(tests tests.cpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/third/doctest)

# 方式二：FetchContent
include(FetchContent)
FetchContent_Declare(doctest ...)
FetchContent_MakeAvailable(doctest)
target_link_libraries(tests PRIVATE doctest::doctest)

# 启用测试
enable_testing()
add_test(NAME MyTests COMMAND tests)
```

#### 项目目录结构

```
project/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── include/
│   └── mylib.h
├── tests/
│   ├── CMakeLists.txt
│   ├── test_main.cpp      # 测试主程序
│   ├── test_math.cpp      # 数学测试
│   └── test_string.cpp    # 字符串测试
└── third/
    └── doctest/
        └── doctest.h
```

### 2.4 验证安装

```cpp
// test_main.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("Installation check") {
    CHECK(true);
    MESSAGE("doctest installed successfully!");
}
```

```bash
# 编译并运行
g++ -std=c++17 test_main.cpp -o tests
./tests

# 期望输出
# [doctest] doctest version is "2.4.11"
# [doctest] run with "--help" for options
# =========================================
# [doctest] test cases: 1 | 1 passed | 0 failed
```

## 3. 基础使用

### 3.1 快速入门

#### 最简单测试

```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("Basic test") {
    CHECK(1 + 1 == 2);
}
```

#### 分离测试主程序

```cpp
// test_main.cpp
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

// test_math.cpp
#include <doctest/doctest.h>

TEST_CASE("Math operations") {
    CHECK(2 * 3 == 6);
}
```

### 3.2 项目结构

#### 多文件测试组织

```
tests/
├── CMakeLists.txt
├── test_main.cpp           # 主入口
├── test_math.cpp            # 数学测试
├── test_string.cpp          # 字符串测试
└── fixtures/
    ├── database_fixture.hpp
    └── network_fixture.hpp
```

```cmake
# tests/CMakeLists.txt
add_executable(tests
    test_main.cpp
    test_math.cpp
    test_string.cpp
)

target_link_libraries(tests PRIVATE mylib)
```

### 3.3 基本命令

#### 运行测试

```bash
# 运行所有测试
./tests

# 运行特定测试用例
./tests -tc="Addition"

# 运行特定测试套件
./tests -ts="Math"

# 运行特定文件
./tests -sf=math.cpp

# 运行标签过滤
./tests -tse="slow"  # 排除 slow 标签
```

#### 输出控制

```bash
# 详细输出
./tests -s

# 紧凑输出
./tests -r=compact

# 成功时也显示
./tests -s

# 显示测试时间
./tests -d=true

# 统计信息
./tests -sc
```

#### 报告生成

```bash
# XML 报告
./tests -r=xml -o=report.xml

# JUnit 报告
./tests -r=junit -o=junit.xml

# JSON 报告
./tests -r=json -o=report.json
```

### 3.4 常用操作

#### 测试过滤

```bash
# 名称包含 "add"
./tests -tc="*add*"

# 正则表达式
./tests -tc="Math.*"

# 组合过滤
./tests -ts="Math" -tc="*Add*"

# 排除测试
./tests -tce="slow"
```

#### 调试模式

```bash
# 仅编译，不运行
./tests -c

# 设置超时（秒）
./tests -t=30

# 失败时进入调试器
./tests -b
```

## 4. 进阶特性

### 4.1 高级配置

#### 配置宏

```cpp
// 禁用测试（生产构建）
#define DOCTEST_CONFIG_DISABLE

// 使用线程安全模式
#define DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS

// 禁用一元宏
#define DOCTEST_CONFIG_NO_UNARY_MACRO

// 自定义命名空间
#define DOCTEST_CONFIG_NAMESPACE mytest

// 输出颜色控制
#define DOCTEST_CONFIG_COLORS_WINDOWS
```

#### 运行时配置

```cpp
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char** argv) {
    doctest::Context context;
    
    // 设置默认选项
    context.setOption("abort-after", 5);    // 失败5次后停止
    context.setOption("sort", "name");      // 按名称排序
    context.setOption("duration", true);    // 显示时间
    
    // 解析命令行
    context.applyCommandLine(argc, argv);
    
    // 运行测试
    int res = context.run();
    
    // 清理资源...
    return res;
}
```

### 4.2 扩展功能

#### 自定义报告器

```cpp
#include <doctest/doctest.h>

class MyReporter : public doctest::IReporter {
public:
    void test_run_start(const doctest::TestRunStats&) override {
        std::cout << "=== 测试开始 ===" << std::endl;
    }
    
    void test_case_start(const doctest::TestCaseData& tc) override {
        std::cout << "运行: " << tc.m_name << std::endl;
    }
    
    void test_case_end(const doctest::CurrentTestCaseStats&) override {
        std::cout << "完成" << std::endl;
    }
    
    void test_run_end(const doctest::TestRunStats&) override {
        std::cout << "=== 测试结束 ===" << std::endl;
    }
};

// 注册报告器
REGISTER_REPORTER("myreporter", 1, MyReporter);
```

#### 测试事件监听

```cpp
// 测试开始前
TEST_CASE("With setup") {
    // 设置代码
    SUBCASE("Test 1") { /* ... */ }
    SUBCASE("Test 2") { /* ... */ }
    // 清理代码（自动执行）
}
```

### 4.3 插件生态

| 扩展 | 功能 |
|------|------|
| doctest-extension | 额外断言宏 |
| doctest-tap | TAP 格式输出 |
| doctest-teamcity | TeamCity 集成 |
| doctest-json | JSON 报告器 |

## 5. 性能优化

### 5.1 调优策略

#### 编译优化

```cpp
// 1. 生产构建时禁用测试
#ifdef NDEBUG
#define DOCTEST_CONFIG_DISABLE
#endif

// 2. 减少模板实例化
#define DOCTEST_CONFIG_NO_UNARY_MACRO

// 3. 预编译头文件
// pch.h
#include <doctest/doctest.h>
```

#### 链接优化

```cmake
# 使用预编译头
target_precompile_headers(tests PRIVATE
    <doctest/doctest.h>
)

# 分离编译
add_library(test_impl test_impl.cpp)
target_link_libraries(tests PRIVATE test_impl)
```

### 5.2 最佳实践

#### 测试隔离

```cpp
// 使用 SUBCASE 隔离测试状态
TEST_CASE("Stack operations") {
    std::stack<int> s;
    
    SUBCASE("Empty stack") {
        CHECK(s.empty());
        CHECK(s.size() == 0);
    }
    
    SUBCASE("Push and pop") {
        s.push(1);
        s.push(2);
        CHECK(s.size() == 2);
        
        s.pop();
        CHECK(s.size() == 1);
    }
}
```

#### 避免重复编译

```cpp
// test_main.cpp（单独编译一次）
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char** argv) {
    return doctest::Context().run();
}

// test_xxx.cpp（每个测试文件）
#include <doctest/doctest.h>
// 不需要定义 DOCTEST_CONFIG_IMPLEMENT
```

#### 并行测试

```bash
# 多线程运行
./tests -np=4

# 指定线程数
./tests --no-throw=false -np=auto
```

## 6. 问题排查

### 6.1 常见问题

#### 问题一：链接错误

**症状**：`undefined reference to 'doctest::Context::run()'`

**原因**：未实现主函数

**解决**：

```cpp
// 方案一：使用宏
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 方案二：手动实现
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char** argv) {
    return doctest::Context().run();
}
```

#### 问题二：编译时间过长

**症状**：包含 doctest 后编译变慢

**解决**：

```cpp
// 1. 预编译头
// CMakeLists.txt
target_precompile_headers(tests PRIVATE <doctest/doctest.h>)

// 2. 生产构建禁用
#ifdef NDEBUG
#define DOCTEST_CONFIG_DISABLE
#endif

// 3. 减少宏展开
#define DOCTEST_CONFIG_NO_UNARY_MACRO
```

#### 问题三：测试顺序问题

**症状**：测试顺序不同导致失败

**解决**：

```cpp
// 确保测试独立性
TEST_CASE("Independent test") {
    // 使用 SUBCASE 而非共享状态
    SUBCASE("Case 1") {
        int x = 0;  // 局部变量
        // ...
    }
}

// 或使用夹具
struct Fixture {
    int x = 0;
    void setup() { x = 10; }
    void teardown() { x = 0; }
};

TEST_CASE_FIXTURE(Fixture, "With fixture") {
    CHECK(x == 10);
}
```

### 6.2 调试技巧

#### 详细输出

```bash
# 显示所有断言
./tests -s

# 显示失败详情
./tests -sc -s

# 只运行失败的
./tests -tc="*failed*"
```

#### 错误定位

```cpp
// 使用 MESSAGE 记录调试信息
TEST_CASE("Debug test") {
    int result = complex_function();
    MESSAGE("Result: ", result);
    CHECK(result == expected);
}

// 使用 INFO 添加上下文
TEST_CASE("With context") {
    INFO("Testing with input: ", input);
    CHECK(process(input) == expected);
}
```

## 7. 集成实践

### 7.1 工具链集成

#### CMake 集成

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(MyProject)

# 添加 doctest
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest
    GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

# 创建测试目标
enable_testing()
add_executable(tests tests/test_main.cpp)
target_link_libraries(tests PRIVATE doctest::doctest)

# 添加 CTest 集成
add_test(NAME MyTests COMMAND tests)

# 设置输出目录
set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
```

#### Conan 集成

```yaml
# conanfile.txt
[requires]
doctest/2.4.11

[generators]
cmake

[options]
doctest:header_only=True
```

#### Meson 集成

```python
# meson.build
doctest = dependency('doctest', fallback: ['doctest', 'doctest_dep'])

tests = executable('tests', 'test_main.cpp',
    dependencies: doctest,
)

test('MyTests', tests)
```

### 7.2 CI/CD 配置

#### GitHub Actions

```yaml
# .github/workflows/test.yml
name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake g++
    
    - name: Configure
      run: cmake -B build -DCMAKE_BUILD_TYPE=Debug
    
    - name: Build
      run: cmake --build build
    
    - name: Run tests
      run: |
        cd build
        ctest --output-on-failure
        # 或直接运行
        # ./bin/tests
    
    - name: Upload report
      uses: actions/upload-artifact@v3
      with:
        name: test-report
        path: build/report.xml
```

#### GitLab CI

```yaml
# .gitlab-ci.yml
stages:
  - test

test:
  stage: test
  image: gcc:latest
  script:
    - cmake -B build
    - cmake --build build
    - cd build && ctest --output-on-failure
  artifacts:
    reports:
      junit: build/report.xml
```

#### Jenkins Pipeline

```groovy
pipeline {
    agent any
    
    stages {
        stage('Build') {
            steps {
                sh 'cmake -B build'
                sh 'cmake --build build'
            }
        }
        
        stage('Test') {
            steps {
                sh './build/bin/tests -r=junit -o=report.xml'
                junit 'report.xml'
            }
        }
    }
}
```

### 7.3 实战案例

#### 案例：数学库测试

```cpp
// math.hpp
#pragma once
#include <cmath>

namespace math {
    double add(double a, double b) { return a + b; }
    double divide(double a, double b) {
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    }
}

// test_math.cpp
#include <doctest/doctest.h>
#include "math.hpp"

TEST_SUITE("Math library") {
    
    TEST_CASE("Addition") {
        CHECK(math::add(1, 2) == 3);
        CHECK(math::add(-1, 1) == 0);
        CHECK(math::add(0.1, 0.2) == doctest::Approx(0.3));
    }
    
    TEST_CASE("Division") {
        SUBCASE("Normal division") {
            CHECK(math::divide(6, 2) == 3);
        }
        
        SUBCASE("Division by zero") {
            CHECK_THROWS_AS(math::divide(1, 0), std::runtime_error);
        }
    }
}
```

#### 案例：类测试与夹具

```cpp
// database.hpp
class Database {
public:
    void insert(const std::string& key, int value) {
        data_[key] = value;
    }
    
    int get(const std::string& key) const {
        auto it = data_.find(key);
        if (it == data_.end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }
    
    size_t size() const { return data_.size(); }
    
private:
    std::map<std::string, int> data_;
};

// test_database.cpp
#include <doctest/doctest.h>
#include "database.hpp"

struct DatabaseFixture {
    Database db;
    
    DatabaseFixture() {
        db.insert("one", 1);
        db.insert("two", 2);
    }
};

TEST_CASE_FIXTURE(DatabaseFixture, "Database operations") {
    SUBCASE("Size check") {
        CHECK(db.size() == 2);
    }
    
    SUBCASE("Get existing key") {
        CHECK(db.get("one") == 1);
        CHECK(db.get("two") == 2);
    }
    
    SUBCASE("Get non-existing key") {
        CHECK_THROWS_AS(db.get("three"), std::out_of_range);
    }
    
    SUBCASE("Insert new key") {
        db.insert("three", 3);
        CHECK(db.size() == 3);
        CHECK(db.get("three") == 3);
    }
}
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| 官方仓库 | https://github.com/doctest/doctest |
| 官方文档 | https://github.com/doctest/doctest/blob/master/doc/markdown/readme.md |
| API 参考 | https://github.com/doctest/doctest/blob/master/doc/markdown/reference.md |
| 示例代码 | https://github.com/doctest/doctest/tree/master/examples |
| 发布版本 | https://github.com/doctest/doctest/releases |

### 8.2 学习路径

#### 初级路径

1. **基础概念**：理解测试驱动开发（TDD）
2. **安装配置**：单头文件集成
3. **基本断言**：CHECK、REQUIRE 使用
4. **测试组织**：TEST_CASE、TEST_SUITE

#### 中级路径

1. **高级断言**：异常、比较断言
2. **SUBCASE**：子测试机制
3. **夹具使用**：测试数据准备与清理
4. **参数化测试**：GENERATE 宏

#### 高级路径

1. **自定义报告器**：扩展输出格式
2. **CI/CD 集成**：持续测试流程
3. **性能优化**：编译时间优化
4. **并行测试**：多线程执行

### 8.3 社区资源

| 资源 | 描述 |
|------|------|
| GitHub Issues | 问题反馈与讨论 |
| Stack Overflow | 标签 `[doctest]` |
| CppCon 视频 | doctest 作者演讲 |
| 测试框架对比 | Catch2、GoogleTest 对比文章 |

### 8.4 最佳实践总结

| 实践 | 描述 |
|------|------|
| 预编译头 | 使用 CMake 预编译头加速编译 |
| 生产禁用 | 使用 `DOCTEST_CONFIG_DISABLE` 移除测试代码 |
| 分离主程序 | 单独编译测试主函数 |
| SUBCASE 优先 | 使用 SUBCASE 代替共享状态 |
| BDD 风格 | 复杂场景使用 GIVEN/WHEN/THEN |
| CI 集成 | 自动化测试，生成报告 |
| 测试隔离 | 确保每个测试独立运行 |