# Catch2 - 现代 C++ 测试框架

## 1. 概述与背景

### 1.1 工具定位

Catch2 是一个轻量级、现代化的 C++ 原生测试框架，采用单头文件设计理念，专注于提供简洁、表达力强的测试语法。其核心设计思想是让测试代码更接近自然语言，通过 SECTION 机制实现测试逻辑的模块化和复用。

**核心特点**：
- **单头文件集成**：只需包含 `catch.hpp` 即可使用
- **编译速度优化**：通过编译屏障技术加速编译
- **BDD 风格支持**：原生支持 GIVEN-WHEN-THEN 模式
- **自动测试注册**：无需手动注册测试用例

### 1.2 发展历史

| 年份 | 版本 | 里程碑 |
|------|------|--------|
| 2010 | v1.0 | Phil Nash 发布首个版本，基于单头文件理念 |
| 2017 | v2.0 | 改进编译性能，增加 REPORTER 扩展 |
| 2020 | v3.0 | 模块化重构，C++17 标准，拆分单头文件 |
| 2022 | v3.2 | 增强类型推断，支持 C++20 协程 |
| 2024 | v3.5 | 完善错误诊断，优化 SECTION 性能 |

### 1.3 核心特性

| 特性类别 | 功能描述 | 技术实现 |
|---------|---------|---------|
| **测试组织** | TEST_CASE + SECTION 嵌套 | 编译期注册表 |
| **断言系统** | REQUIRE/CHECK 双模式 | 表达式模板技术 |
| **参数化** | GENERATE 数据驱动 | 生成器模式 |
| **BDD 支持** | SCENARIO/GIVEN/WHEN | 宏映射到 TEST_CASE |
| **报告系统** | 多格式报告器 | 插件架构 |
| **标签过滤** | 运行时标签选择 | 字符串匹配 |

### 1.4 适用场景

| 项目类型 | 推荐指数 | 理由 |
|---------|---------|------|
| **现代 C++ 项目** | ★★★★★ | C++17 特性深度集成 |
| **教学演示** | ★★★★★ | 语法简洁，学习曲线平缓 |
| **快速原型开发** | ★★★★★ | 单头文件，集成简单 |
| **BDD 实践** | ★★★★☆ | 原生 BDD 支持 |
| **嵌入式测试** | ★★★★☆ | 轻量级，依赖少 |
| **企业级项目** | ★★★☆☆ | Mock 支持较弱 |
| **性能测试** | ★★☆☆☆ | 缺少基准测试功能 |

### 1.5 对比分析

| 维度 | Catch2 | GoogleTest | doctest | Boost.Test |
|------|--------|------------|---------|------------|
| **集成复杂度** | 低（单头） | 中（多头） | 低（单头） | 高（依赖 Boost） |
| **编译速度** | 快 | 中 | 最快 | 慢 |
| **BDD 风格** | ✅ 原生支持 | ❌ 需扩展 | ❌ 需扩展 | ⚠️ 有限支持 |
| **Mock 支持** | ❌ 无 | ✅ GMock | ❌ 无 | ⚠️ 第三方 |
| **SECTION 嵌套** | ✅ 支持 | ❌ 无 | ✅ 支持 | ❌ 无 |
| **参数化测试** | GENERATE | TEST_P | 数据生成器 | 数据驱动 |
| **C++ 标准** | C++17 | C++11 | C++11 | C++11 |
| **报告器扩展** | ✅ 插件架构 | ✅ Listener | ⚠️ 有限 | ✅ 输出格式 |
| **文档质量** | ★★★★☆ | ★★★★★ | ★★★☆☆ | ★★★★☆ |
| **社区活跃度** | 高 | 很高 | 中 | 中 |

## 2. 安装与配置

### 2.1 多平台安装

#### 方式一：vcpkg 包管理器（推荐）

```bash
# 安装 Catch2
vcpkg install catch2

# 指定版本
vcpkg install catch2@3.5.0

# 仅头文件版本
vcpkg install catch2:header-only
```

#### 方式二：Conan 包管理器

```bash
# 安装最新版本
conan install catch2/3.5.0

# conanfile.txt 配置
[requires]
catch2/3.5.0

[generators]
CMakeDeps
CMakeToolchain
```

#### 方式三：单头文件（最简单）

```bash
# 下载单头文件
wget https://github.com/catchorg/Catch2/releases/download/v3.5.0/catch.hpp -O include/catch2/catch.hpp

# 或使用 curl
curl -L -o include/catch2/catch.hpp \
  https://github.com/catchorg/Catch2/releases/download/v3.5.0/catch.hpp
```

#### 方式四：CMake FetchContent（推荐）

```cmake
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.5.0
)

FetchContent_MakeAvailable(Catch2)

# 链接目标
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
```

### 2.2 版本管理

| 版本 | C++ 标准 | 主要特性 | 推荐场景 |
|------|---------|---------|---------|
| **v2.x** | C++14 | 单头文件，兼容性好 | 旧项目维护 |
| **v3.0+** | C++17 | 模块化，改进诊断 | 新项目首选 |
| **v3.5+** | C++17 | 性能优化，错误提示增强 | 生产环境 |

**版本选择建议**：
```cmake
# 新项目：使用 v3.5+
find_package(Catch2 3.5 REQUIRED)

# 旧项目：兼容 v2.x
find_package(Catch2 2.13 REQUIRED)
```

### 2.3 环境配置

#### CMakeLists.txt 完整配置

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyTests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 方式一：FetchContent
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

# 方式二：vcpkg
# find_package(Catch2 REQUIRED)

# 测试可执行文件
add_executable(tests
    test_main.cpp
    test_math.cpp
    test_string.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# 启用测试
enable_testing()
include(Catch)
catch_discover_tests(tests)

# CTest 集成
add_test(NAME MyTests COMMAND tests)
```

#### 项目目录结构

```
project/
├── CMakeLists.txt
├── include/
│   └── mylib/
│       └── math.hpp
├── src/
│   └── math.cpp
└── tests/
    ├── CMakeLists.txt
    ├── test_main.cpp      # 测试入口
    ├── test_math.cpp      # 数学模块测试
    ├── test_string.cpp    # 字符串模块测试
    └── test_data/
        └── fixtures/      # 测试数据
```

### 2.4 验证安装

创建测试文件 `test_install.cpp`：

```cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_version.hpp>

TEST_CASE("Catch2 installation check", "[install]") {
    // 检查版本
    REQUIRE(CATCH_VERSION_MAJOR >= 3);
    
    // 检查基本功能
    SECTION("Basic assertions work") {
        REQUIRE(1 + 1 == 2);
        CHECK(true);
    }
    
    SECTION("SECTION works") {
        int x = 42;
        REQUIRE(x == 42);
    }
}
```

编译运行：

```bash
# 编译
cmake -B build && cmake --build build

# 运行测试
./build/tests

# 预期输出
# All tests passed (3 assertions in 1 test case)
```

## 3. 基础使用

### 3.1 快速入门

#### 最小测试示例

```cpp
#define CATCH_CONFIG_MAIN  // 定义 main 函数
#include <catch2/catch_test_macros.hpp>

TEST_CASE("First test", "[quick]") {
    REQUIRE(1 + 1 == 2);
}
```

#### 标准项目结构

```cpp
// test_main.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

// 自定义 main（可选）
int main(int argc, char* argv[]) {
    Catch::Session session;
    
    // 配置选项
    auto ret = session.applyCommandLine(argc, argv);
    if (ret != 0) return ret;
    
    // 运行测试
    return session.run();
}
```

```cpp
// test_math.cpp
#include <catch2/catch_test_macros.hpp>
#include "mylib/math.hpp"

TEST_CASE("Math functions", "[math]") {
    SECTION("Addition") {
        REQUIRE(add(2, 3) == 5);
    }
    
    SECTION("Multiplication") {
        REQUIRE(multiply(2, 3) == 6);
    }
}
```

### 3.2 测试用例组织

#### TEST_CASE 基本结构

```cpp
TEST_CASE("测试名称", "[标签1][标签2]") {
    // 设置代码
    int value = 42;
    
    // 断言
    REQUIRE(value == 42);
    
    // 清理代码（RAII 自动管理）
}
```

#### SECTION 嵌套机制

```cpp
TEST_CASE("Vector operations", "[vector]") {
    std::vector<int> v;
    
    // SECTION 独立执行，每个 SECTION 都从 TEST_CASE 开头开始
    SECTION("Empty vector") {
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
    }
    
    SECTION("Push elements") {
        v.push_back(1);
        v.push_back(2);
        
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        
        SECTION("Clear vector") {
            v.clear();
            REQUIRE(v.empty());
        }
    }
}
```

**SECTION 执行流程**：

```
执行 1: TEST_CASE 开始 → SECTION "Empty" → 结束
执行 2: TEST_CASE 开始 → SECTION "Push" → 结束
执行 3: TEST_CASE 开始 → SECTION "Push" → SECTION "Clear" → 结束
```

### 3.3 断言系统

#### 基本断言

| 断言 | 失败行为 | 使用场景 |
|------|---------|---------|
| `REQUIRE(expr)` | 终止当前测试 | 前置条件、关键断言 |
| `CHECK(expr)` | 继续执行 | 非关键检查、收集多个错误 |
| `REQUIRE_FALSE(expr)` | 终止测试 | 验证表达式为假 |
| `CHECK_FALSE(expr)` | 继续执行 | 非关键假值检查 |

```cpp
TEST_CASE("Assertion behaviors", "[assert]") {
    int* ptr = nullptr;
    
    // REQUIRE 失败后终止
    REQUIRE(ptr != nullptr);  // 失败时退出
    
    // CHECK 失败后继续
    CHECK(ptr != nullptr);    // 失败时记录并继续
    CHECK(ptr != nullptr);    // 仍然会执行
}
```

#### 比较断言

```cpp
TEST_CASE("Comparison macros", "[compare]") {
    // 相等性比较
    REQUIRE_EQ(a, b);      // REQUIRE(a == b)
    REQUIRE_NE(a, b);      // REQUIRE(a != b)
    
    // 大小比较
    REQUIRE_LT(a, b);      // REQUIRE(a < b)
    REQUIRE_LE(a, b);      // REQUIRE(a <= b)
    REQUIRE_GT(a, b);      // REQUIRE(a > b)
    REQUIRE_GE(a, b);      // REQUIRE(a >= b)
    
    // CHECK 版本
    CHECK_EQ(a, b);
    CHECK_LT(a, b);
}
```

#### 浮点数比较

```cpp
#include <catch2/catch_approx.hpp>

TEST_CASE("Floating point comparison", "[float]") {
    double result = 0.1 + 0.2;
    
    // 使用 Approx 处理浮点精度
    REQUIRE(result == Approx(0.3));
    
    // 设置误差范围
    REQUIRE(result == Approx(0.3).epsilon(0.01));  // 1% 相对误差
    REQUIRE(result == Approx(0.3).margin(0.001));    // 绝对误差
    
    // 自定义比较
    REQUIRE_THAT(result, 
        Catch::Matchers::WithinAbs(0.3, 0.001) ||
        Catch::Matchers::WithinRel(0.3, 0.01));
}
```

#### 异常断言

```cpp
TEST_CASE("Exception assertions", "[exception]") {
    // 要求抛出异常
    REQUIRE_THROWS(throw_exception());
    
    // 要求特定异常类型
    REQUIRE_THROWS_AS(
        throw_runtime_error(), 
        std::runtime_error
    );
    
    // 要求不抛出异常
    REQUIRE_NOTHROW(safe_function());
    
    // 检查异常消息
    REQUIRE_THROWS_MATCHES(
        throw_with_message("error"),
        std::runtime_error,
        Catch::Matchers::Message("error")
    );
}
```

### 3.4 运行测试

#### 命令行选项

```bash
# 运行所有测试
./tests

# 运行特定标签
./tests "[math]"
./tests "[quick]"
./tests "[math][quick]"

# 运行特定测试名称
./tests "Addition works"
./tests "Math*"

# 排除标签
./tests "~[slow]"

# 组合过滤
./tests "[math]~[slow]"

# 详细输出
./tests -s          # 显示成功测试
./tests -s -r compact

# 报告格式
./tests -r console  # 默认格式
./tests -r compact  # 紧凑格式
./tests -r junit    # JUnit XML
./tests -r xml      # Catch XML

# 输出到文件
./tests -r junit -o report.xml

# 列出测试
./tests --list-tests
./tests --list-test-names-only

# 列出标签
./tests --list-tags

# 重复测试
./tests "[math]" --repeat 10

# 随机顺序
./tests --order rand

# 设置超时
./tests "[slow]" --timeout 30
```

## 4. 进阶特性

### 4.1 BDD 风格测试

#### SCENARIO 结构

```cpp
SCENARIO("Vector can be resized", "[vector][bdd]") {
    GIVEN("A vector with some items") {
        std::vector<int> v = {1, 2, 3};
        auto original_size = v.size();
        
        WHEN("Resize to larger size") {
            v.resize(10);
            
            THEN("Size increases") {
                REQUIRE(v.size() == 10);
            }
            
            AND_THEN("New elements are zero-initialized") {
                for (size_t i = original_size; i < v.size(); ++i) {
                    REQUIRE(v[i] == 0);
                }
            }
            
            AND_THEN("Original elements preserved") {
                REQUIRE(v[0] == 1);
                REQUIRE(v[1] == 2);
                REQUIRE(v[2] == 3);
            }
        }
        
        WHEN("Resize to smaller size") {
            v.resize(1);
            
            THEN("Size decreases") {
                REQUIRE(v.size() == 1);
            }
            
            AND_THEN("Remaining element preserved") {
                REQUIRE(v[0] == 1);
            }
        }
        
        WHEN("Clear the vector") {
            v.clear();
            
            THEN("Vector is empty") {
                REQUIRE(v.empty());
                REQUIRE(v.size() == 0);
            }
        }
    }
}
```

**BDD 输出格式**：

```
Scenario: Vector can be resized
  Given: A vector with some items
   When: Resize to larger size
   Then: Size increases
And then: New elements are zero-initialized
And then: Original elements preserved
```

### 4.2 测试夹具

#### 结构体夹具

```cpp
// 共享测试数据
struct DatabaseFixture {
    Database db;
    std::string test_db_path = "test.db";
    
    DatabaseFixture() {
        db.connect(test_db_path);
        db.execute("CREATE TABLE IF NOT EXISTS users (id INT, name TEXT)");
    }
    
    ~DatabaseFixture() {
        db.disconnect();
        std::remove(test_db_path.c_str());
    }
    
    void insert_user(int id, const std::string& name) {
        db.execute("INSERT INTO users VALUES (?, ?)", id, name);
    }
};

TEST_CASE_METHOD(DatabaseFixture, "Database operations", "[db]") {
    SECTION("Insert and query") {
        insert_user(1, "Alice");
        REQUIRE(db.query("SELECT name FROM users WHERE id=1") == "Alice");
    }
    
    SECTION("Multiple inserts") {
        insert_user(1, "Alice");
        insert_user(2, "Bob");
        REQUIRE(db.count("users") == 2);
    }
}
```

#### 类夹具

```cpp
class VectorFixture : public Catch::TestCaseFixture {
protected:
    std::vector<int> vec;
    std::vector<int> expected;
    
public:
    void setUp() override {
        vec = {1, 2, 3, 4, 5};
        expected = {1, 2, 3, 4, 5};
    }
};

TEST_CASE_METHOD(VectorFixture, "Vector algorithms", "[vector]") {
    SECTION("Sort is stable") {
        std::sort(vec.begin(), vec.end());
        REQUIRE(vec == expected);
    }
    
    SECTION("Reverse") {
        std::reverse(vec.begin(), vec.end());
        REQUIRE(vec.back() == 1);
    }
}
```

### 4.3 参数化测试

#### GENERATE 基本用法

```cpp
TEST_CASE("Square function", "[math]") {
    auto value = GENERATE(1, 2, 3, 4, 5);
    
    INFO("Testing value: " << value);
    REQUIRE(square(value) == value * value);
}
```

#### 范围生成器

```cpp
TEST_CASE("Range generation", "[generate]") {
    // 生成 [1, 10) 的值
    auto i = GENERATE(range(1, 10));
    REQUIRE(i > 0);
    REQUIRE(i < 10);
    
    // 带步长
    auto j = GENERATE(range(0, 100, 10));  // 0, 10, 20, ..., 90
    REQUIRE(j % 10 == 0);
}
```

#### 表驱动测试

```cpp
TEST_CASE("Table-driven test", "[table]") {
    auto [input, expected] = GENERATE(
        table<int, int>({
            {0, 0},
            {1, 1},
            {2, 4},
            {3, 9},
            {-1, 1},
            {10, 100}
        })
    );
    
    INFO("Input: " << input << ", Expected: " << expected);
    REQUIRE(square(input) == expected);
}
```

#### 多参数组合

```cpp
TEST_CASE("Multiple parameters", "[multi]") {
    auto a = GENERATE(1, 2, 3);
    auto b = GENERATE(10, 20);
    
    // 组合：(1,10), (1,20), (2,10), (2,20), (3,10), (3,20)
    int result = add(a, b);
    REQUIRE(result == a + b);
}
```

### 4.4 标签系统

#### 定义标签

```cpp
// 基本标签
TEST_CASE("Test", "[math]") {}

// 多标签
TEST_CASE("Complex test", "[math][slow][integration]") {}

// 特殊标签
TEST_CASE("Hidden test", "[.]") {}              // 隐藏测试
TEST_CASE("Important", "[!mayfail]") {}          // 允许失败
TEST_CASE("Expected fail", "[!shouldfail]") {}   // 应该失败
TEST_CASE("Non-portable", "[!nonportable]") {}   // 不可移植测试
```

#### 标签分类

| 标签类型 | 示例 | 用途 |
|---------|------|------|
| **功能标签** | `[math]` `[string]` `[io]` | 按功能模块分类 |
| **速度标签** | `[quick]` `[slow]` | 按执行速度分类 |
| **层级标签** | `[unit]` `[integration]` | 测试层级分类 |
| **状态标签** | `[wip]` `[broken]` | 开发状态标记 |
| **特殊标签** | `[.]` `[!mayfail]` | Catch2 内置标签 |

#### 运行时过滤

```bash
# 运行单个标签
./tests "[math]"

# 组合标签（AND）
./tests "[math][quick]"

# 排除标签
./tests "~[slow]"

# 复杂过滤
./tests "([math] or [string]) and ~[slow]"

# 通配符
./tests "Math*"
./tests "*Quick*"
```

## 5. 报告与诊断

### 5.1 内置报告器

| 报告器 | 命令 | 输出格式 |
|--------|------|---------|
| console | `-r console` | 默认，彩色控制台 |
| compact | `-r compact` | 紧凑单行 |
| junit | `-r junit` | JUnit XML 格式 |
| xml | `-r xml` | Catch2 XML 格式 |
| tap | `-r tap` | TAP 协议格式 |
| sonarqube | `-r sonarqube` | SonarQube 格式 |

```bash
# 控制台输出
./tests -r console

# JUnit XML（CI 集成）
./tests -r junit -o junit-report.xml

# 紧凑输出
./tests -r compact

# 组合使用
./tests "[quick]" -r junit -o quick-tests.xml
```

### 5.2 自定义报告器

```cpp
#include <catch2/reporters/catch_reporter_streaming_base.hpp>

class CustomReporter : public Catch::StreamingReporterBase {
public:
    using StreamingReporterBase::StreamingReporterBase;
    
    void testRunStarting(Catch::TestRunInfo const& runInfo) override {
        stream << "=== Test Run: " << runInfo.name << " ===\n";
    }
    
    void testCaseStarting(Catch::TestCaseInfo const& testInfo) override {
        stream << "Starting: " << testInfo.name << "\n";
        stream << "  Tags: ";
        for (auto const& tag : testInfo.tags) {
            stream << tag << " ";
        }
        stream << "\n";
    }
    
    void assertionEnded(Catch::AssertionStats const& assertionStats) override {
        if (assertionStats.assertionResult.succeeded()) {
            stream << "  ✓ PASSED\n";
        } else {
            stream << "  ✗ FAILED: " 
                   << assertionStats.assertionResult.getExpression() << "\n";
        }
    }
    
    void testCaseEnded(Catch::TestCaseStats const& testCaseStats) override {
        stream << "Ended: " << testCaseStats.testInfo.name << "\n";
        if (testCaseStats.totals.assertions.allPassed()) {
            stream << "  Status: ✓ ALL PASSED\n\n";
        } else {
            stream << "  Status: ✗ FAILED\n\n";
        }
    }
    
    void testRunEnded(Catch::TestRunStats const& testRunStats) override {
        stream << "=== Summary ===\n";
        stream << "Total: " << testRunStats.totals.testCases.total() << "\n";
        stream << "Passed: " << testRunStats.totals.testCases.passed() << "\n";
        stream << "Failed: " << testRunStats.totals.testCases.failed() << "\n";
    }
};

CATCH_REGISTER_REPORTER("custom", CustomReporter)
```

**使用自定义报告器**：

```bash
./tests -r custom -o custom-report.txt
```

### 5.3 日志与诊断

#### INFO/WARN/FAIL

```cpp
TEST_CASE("Logging and diagnostics", "[log]") {
    int value = compute_value();
    
    // 信息日志（失败时显示）
    INFO("Current value: " << value);
    INFO("This will be shown if assertion fails");
    
    // 警告（总是显示）
    WARN("This is a warning message");
    
    // 失败（继续执行）
    FAIL("This test failed but continues");
    
    // 快速失败（终止测试）
    // FAIL_CHECK("This terminates the test");
    
    REQUIRE(value > 0);
}
```

#### CAPTURE 宏

```cpp
TEST_CASE("Capture variables", "[capture]") {
    int a = 10;
    int b = 20;
    std::string s = "hello";
    
    // 自动捕获变量名和值
    CAPTURE(a, b, s);
    // 输出：a := 10, b := 20, s := "hello"
    
    REQUIRE(add(a, b) == 30);
}
```

## 6. 性能优化与最佳实践

### 6.1 编译优化

#### 分离编译模式

```cpp
// catch_main.cpp - 单独编译
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// 其他测试文件
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test 1", "[tag]") {
    REQUIRE(true);
}
```

```cmake
# CMakeLists.txt
add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

add_executable(tests test1.cpp test2.cpp)
target_link_libraries(tests PRIVATE catch_main)
```

#### 预编译头

```cmake
# CMake 预编译头
target_precompile_headers(tests PRIVATE
    <catch2/catch.hpp>
    <vector>
    <string>
)
```

### 6.2 测试组织最佳实践

#### 文件结构

```
tests/
├── unit/              # 单元测试
│   ├── test_math.cpp
│   └── test_string.cpp
├── integration/       # 集成测试
│   └── test_api.cpp
├── performance/       # 性能测试
│   └── test_benchmark.cpp
├── fixtures/          # 共享夹具
│   ├── database_fixture.hpp
│   └── network_fixture.hpp
└── test_main.cpp      # 主入口
```

#### 标签约定

```cpp
// 功能标签
TEST_CASE("Math add", "[math][unit]") {}
TEST_CASE("Math multiply", "[math][unit]") {}

// 速度标签
TEST_CASE("Quick test", "[quick]") {}
TEST_CASE("Slow integration", "[slow][integration]") {}

// 状态标签
TEST_CASE("Work in progress", "[wip]") {}
TEST_CASE("Known bug", "[known-issue][.]") {}
```

### 6.3 测试数据管理

#### 外部测试数据

```cpp
#include <fstream>
#include <nlohmann/json.hpp>

struct TestData {
    std::string input;
    std::string expected;
    
    static std::vector<TestData> load(const std::string& path) {
        std::ifstream file(path);
        nlohmann::json j;
        file >> j;
        return j.get<std::vector<TestData>>();
    }
};

TEST_CASE("Data-driven test", "[data]") {
    auto test_data = TestData::load("tests/data/test_cases.json");
    
    for (const auto& data : test_data) {
        INFO("Input: " << data.input);
        REQUIRE(process(data.input) == data.expected);
    }
}
```

## 7. 问题排查

### 7.1 常见问题

#### 问题 1：SECTION 执行次数过多

**症状**：测试运行时间过长，SECTION 被重复执行。

**原因**：多个 SECTION 组合导致执行路径爆炸。

**解决方案**：

```cpp
// 问题代码
TEST_CASE("Bad SECTION usage") {
    std::vector<int> v;
    
    SECTION("A") { v.push_back(1); }
    SECTION("B") { v.push_back(2); }
    SECTION("C") { v.push_back(3); }  // 3 个 SECTION 独立执行
}

// 改进：拆分为多个 TEST_CASE
TEST_CASE("Push A", "[vector]") {
    std::vector<int> v;
    v.push_back(1);
    REQUIRE(v.size() == 1);
}

TEST_CASE("Push B", "[vector]") {
    std::vector<int> v;
    v.push_back(2);
    REQUIRE(v.size() == 1);
}
```

#### 问题 2：浮点数比较失败

**症状**：`REQUIRE(0.1 + 0.2 == 0.3)` 失败。

**解决方案**：

```cpp
// 错误方式
REQUIRE(0.1 + 0.2 == 0.3);  // 失败

// 正确方式 1：使用 Approx
REQUIRE(0.1 + 0.2 == Approx(0.3));

// 正确方式 2：自定义误差
REQUIRE(result == Approx(expected).epsilon(0.001));

// 正确方式 3：绝对误差
REQUIRE(result == Approx(expected).margin(1e-10));
```

#### 问题 3：REQUIRE 终止测试

**症状**：第一个 REQUIRE 失败后测试终止，无法看到后续错误。

**解决方案**：

```cpp
// 错误：关键和非关键断言都用 REQUIRE
TEST_CASE("All REQUIRE") {
    REQUIRE(check1());  // 失败后终止
    REQUIRE(check2());  // 不会执行
    REQUIRE(check3());  // 不会执行
}

// 正确：非关键断言使用 CHECK
TEST_CASE("REQUIRE and CHECK") {
    REQUIRE(precondition());  // 前置条件，失败终止
    CHECK(check1());          // 失败继续
    CHECK(check2());         // 失败继续
    CHECK(check3());         // 失败继续
}
```

### 7.2 调试技巧

#### 启用详细输出

```bash
# 显示所有断言
./tests -s

# 显示所有测试
./tests -a

# 显示标签
./tests --list-tags

# 详细日志
./tests "[test]" -s 2>&1 | tee debug.log
```

#### 使用断点调试

```cpp
TEST_CASE("Debug test", "[debug]") {
    int value = compute();
    
    // 设置断点在这行
    CAPTURE(value);
    
    // 检查中间值
    INFO("Intermediate: " << value);
    
    REQUIRE(value > 0);
}
```

#### 打印测试信息

```bash
# 列出所有测试名称
./tests --list-test-names-only > test_names.txt

# 列出所有标签
./tests --list-tags

# 显示测试统计
./tests --list-tests
```

## 8. 集成实践

### 8.1 CMake 深度集成

#### CTest 集成

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject CXX)

enable_testing()

# 查找 Catch2
find_package(Catch2 3.5 REQUIRED)

# 测试可执行文件
add_executable(tests
    test_main.cpp
    test_math.cpp
    test_string.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# 自动发现测试
include(Catch)
catch_discover_tests(tests
    TEST_PREFIX "unit."
    TEST_SUFFIX ".test"
    PROPERTIES
        LABELS "unit"
        TIMEOUT 300
)

# 添加测试标签
set_tests_properties(
    test_math PROPERTIES LABELS "math;quick"
)
set_tests_properties(
    test_slow PROPERTIES LABELS "slow;integration"
)
```

#### 运行 CTest

```bash
# 运行所有测试
ctest

# 运行特定标签
ctest -L quick

# 详细输出
ctest -V

# 并行运行
ctest -j 4

# 输出到文件
ctest -T Test --no-compress-output
```

### 8.2 CI/CD 集成

#### GitHub Actions

```yaml
name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v4
    
    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake g++
    
    - name: Configure
      run: cmake -B build -DCMAKE_BUILD_TYPE=Debug
    
    - name: Build
      run: cmake --build build -j$(nproc)
    
    - name: Run tests
      working-directory: build
      run: |
        ctest --output-on-failure -j$(nproc)
        # 生成 JUnit 报告
        ./tests -r junit -o test-report.xml
    
    - name: Publish test results
      uses: EnricoMi/publish-unit-test-result-action@v2
      if: always()
      with:
        files: build/test-report.xml
```

#### GitLab CI

```yaml
test:
  stage: test
  image: gcc:latest
  script:
    - cmake -B build
    - cmake --build build -j$(nproc)
    - cd build
    - ctest --output-on-failure
    - ./tests -r junit -o test-report.xml
  artifacts:
    when: always
    reports:
      junit: build/test-report.xml
  coverage: '/lines: \d+\.\d+%/'
```

#### Jenkins Pipeline

```groovy
pipeline {
    agent any
    
    stages {
        stage('Build') {
            steps {
                sh 'cmake -B build -DCMAKE_BUILD_TYPE=Debug'
                sh 'cmake --build build -j$(nproc)'
            }
        }
        
        stage('Test') {
            steps {
                dir('build') {
                    sh 'ctest --output-on-failure'
                    sh './tests -r junit -o test-report.xml'
                }
            }
            post {
                always {
                    junit 'build/test-report.xml'
                }
            }
        }
    }
}
```

### 8.3 实战案例

#### 案例 1：数学库测试

```cpp
// test_math.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "mylib/math.hpp"

TEST_CASE("Math library", "[math]") {
    SECTION("Addition") {
        REQUIRE(add(1, 2) == 3);
        REQUIRE(add(-1, 1) == 0);
        REQUIRE(add(0, 0) == 0);
    }
    
    SECTION("Division") {
        REQUIRE(divide(10, 2) == 5);
        REQUIRE(divide(1, 3) == Approx(0.333).epsilon(0.01));
        
        REQUIRE_THROWS_AS(divide(1, 0), std::runtime_error);
    }
    
    SECTION("Square root") {
        REQUIRE(sqrt(4) == 2);
        REQUIRE(sqrt(2) == Approx(1.414).epsilon(0.001));
        REQUIRE_THROWS_AS(sqrt(-1), std::invalid_argument);
    }
}

// 表驱动测试
TEST_CASE("Square function - table driven", "[math][table]") {
    auto [input, expected] = GENERATE(
        table<int, int>({
            {0, 0},
            {1, 1},
            {2, 4},
            {10, 100},
            {-5, 25}
        })
    );
    
    REQUIRE(square(input) == expected);
}

// 参数化测试
TEST_CASE("Factorial", "[math]") {
    auto n = GENERATE(0, 1, 2, 5, 10);
    
    SECTION("Factorial values") {
        int result = factorial(n);
        CAPTURE(n, result);
        REQUIRE(result >= 1);
    }
}
```

#### 案例 2：数据库测试

```cpp
// test_database.cpp
#include <catch2/catch_test_macros.hpp>
#include "mylib/database.hpp"

struct DatabaseFixture {
    Database db;
    
    DatabaseFixture() {
        db.connect(":memory:");
        db.execute("CREATE TABLE users (id INTEGER, name TEXT)");
    }
    
    ~DatabaseFixture() {
        db.disconnect();
    }
};

TEST_CASE_METHOD(DatabaseFixture, "Database operations", "[db]") {
    SECTION("Insert and query") {
        db.insert("users", {{"id", 1}, {"name", "Alice"}});
        db.insert("users", {{"id", 2}, {"name", "Bob"}});
        
        REQUIRE(db.count("users") == 2);
        
        auto result = db.query("SELECT * FROM users WHERE id=1");
        REQUIRE(result["name"] == "Alice");
    }
    
    SECTION("Transaction") {
        db.begin_transaction();
        db.insert("users", {{"id", 1}, {"name", "Alice"}});
        db.rollback();
        
        REQUIRE(db.count("users") == 0);
    }
    
    SECTION("Error handling") {
        REQUIRE_THROWS_AS(
            db.query("SELECT * FROM nonexistent"),
            std::runtime_error
        );
    }
}
```

#### 案例 3：HTTP 客户端测试

```cpp
// test_http_client.cpp
#include <catch2/catch_test_macros.hpp>
#include "mylib/http_client.hpp"

TEST_CASE("HTTP Client", "[http][integration]") {
    HttpClient client("http://httpbin.org");
    
    SECTION("GET request") {
        auto response = client.get("/get");
        
        REQUIRE(response.status_code == 200);
        REQUIRE(response.body.contains("args"));
    }
    
    SECTION("POST request") {
        auto response = client.post("/post", R"({"key": "value"})");
        
        REQUIRE(response.status_code == 200);
        REQUIRE(response.body.contains("key"));
    }
    
    SECTION("Timeout handling") {
        HttpClient slow_client("http://httpbin.org");
        slow_client.set_timeout(1);
        
        REQUIRE_THROWS_AS(
            slow_client.get("/delay/5"),
            TimeoutException
        );
    }
}
```

## 9. 参考资源

### 9.1 官方文档

| 资源 | 链接 | 说明 |
|------|------|------|
| **GitHub 仓库** | https://github.com/catchorg/Catch2 | 源码和 Issue |
| **官方文档** | https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md | 完整文档 |
| **发布版本** | https://github.com/catchorg/Catch2/releases | 版本下载 |
| **示例代码** | https://github.com/catchorg/Catch2/tree/devel/examples | 官方示例 |

### 9.2 学习路径

| 阶段 | 内容 | 时间 |
|------|------|------|
| **入门** | 基本断言、TEST_CASE、SECTION | 2-3 小时 |
| **进阶** | BDD 风格、参数化测试、夹具 | 4-6 小时 |
| **高级** | 自定义报告器、标签系统、性能优化 | 6-8 小时 |
| **精通** | CI/CD 集成、插件开发、大型项目实践 | 持续实践 |

### 9.3 社区资源

- **Stack Overflow**: `[catch2]` 标签
- **Reddit**: r/cpp
- **CppCon 视频**: "Modern C++ Testing with Catch2"

### 9.4 相关工具

| 工具 | 用途 | 集成方式 |
|------|------|---------|
| **CTest** | CMake 测试驱动 | `catch_discover_tests()` |
| **Jenkins** | CI/CD | JUnit 报告器 |
| **GitHub Actions** | CI/CD | 直接运行 |
| **SonarQube** | 代码质量 | SonarQube 报告器 |
| **Coverage** | 代码覆盖率 | gcov/lcov |

---

*本文档基于 Catch2 v3.5 编写，适用于 C++17 及以上标准。*