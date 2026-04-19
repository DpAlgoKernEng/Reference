# Catch2 - 现代 C++ 测试框架

## 1. 概述与背景

### 1.1 工具定位

Catch2 是一个现代化的、轻量级的 C++ 测试框架，专注于提供简洁直观的测试体验。其设计哲学是"测试代码应该像文档一样易读"，让开发者能够用最少的代码编写清晰的测试用例。

核心特点：
- **单头文件集成**：只需包含一个头文件即可使用所有功能
- **BDD 风格支持**：原生支持 GIVEN/WHEN/THEN 模式的行为驱动测试
- **编译速度快**：相比 GoogleTest，编译时间显著降低
- **现代 C++ 特性**：充分利用 C++14/17 特性，代码简洁优雅
- **零依赖**：不需要任何外部库，开箱即用

### 1.2 发展历史

| 年份 | 版本 | 里程碑特性 |
|------|------|------------|
| 2010 | 1.0 | Phil Nash 发布初始版本，单头文件设计 |
| 2015 | 1.5 | 增加 BDD 风格支持，完善 SECTION 机制 |
| 2017 | 2.0 | 重构代码库，改进模板测试支持 |
| 2018 | 2.5 | 增强报告器系统，支持自定义输出格式 |
| 2020 | 3.0 | 模块化架构，支持 vcpkg 和 Conan |
| 2022 | 3.4 | 改进 C++20 支持，优化编译性能 |
| 2024 | 3.5 | 增强错误诊断，完善 CMake 集成 |

### 1.3 核心特性

**测试组织方式：**

| 特性 | 说明 | 优势 |
|------|------|------|
| TEST_CASE | 传统测试用例 | 简单直接 |
| SCENARIO | BDD 风格测试 | 可读性强 |
| SECTION | 子测试嵌套 | 避免代码重复 |
| GENERATE | 参数化测试 | 数据驱动测试 |
| TEST_CASE_METHOD | 夹具支持 | 资源管理 |

**断言系统：**

| 断言类型 | 关键字 | 失败行为 |
|---------|--------|---------|
| 强断言 | REQUIRE | 立即终止测试 |
| 弱断言 | CHECK | 继续执行后续代码 |
| 异常断言 | REQUIRE_THROWS | 验证异常抛出 |
| 浮点断言 | Approx | 处理浮点精度问题 |

### 1.4 适用场景

| 场景 | 推荐度 | 说明 |
|------|--------|------|
| 现代 C++ 项目 | ★★★★★ | 完美契合现代 C++ 开发 |
| BDD 风格测试 | ★★★★★ | 原生支持，体验最佳 |
| 快速原型开发 | ★★★★★ | 单头文件，快速集成 |
| 单元测试 | ★★★★★ | 核心使用场景 |
| 集成测试 | ★★★★☆ | 需配合 Mock 库 |
| 性能测试 | ★★★☆☆ | 有基础支持 |
| 需要 Mock | ★★☆☆☆ | 需要第三方库配合 |

### 1.5 对比分析

| 特性 | Catch2 | GoogleTest | doctest | Boost.Test |
|------|--------|------------|---------|------------|
| 头文件 | 单头 | 多头 | 单头 | 多头 |
| 编译速度 | 快 | 中 | 最快 | 慢 |
| BDD 风格 | ✅ | ❌ | ✅ | ❌ |
| Mock 支持 | ❌ | ✅ GMock | ❌ | ❌ |
| 参数化 | GENERATE | TEST_P | GENERATE | BOOST_DATA_TEST_CASE |
| SECTION 嵌套 | ✅ | ❌ | ✅ | ❌ |
| 现代 C++ | C++17 | C++11 | C++17 | C++11 |
| 文档质量 | 优秀 | 优秀 | 良好 | 优秀 |
| 社区活跃度 | 高 | 极高 | 中 | 中 |

**选择建议：**
- 选择 Catch2：现代项目、BDD 风格、快速编译、单头文件需求
- 选择 GoogleTest：企业级项目、需要 Mock、成熟稳定
- 选择 doctest：极致编译速度、嵌入式系统

## 2. 安装与配置

### 2.1 多平台安装

**方式一：包管理器（推荐）**

```bash
# vcpkg
vcpkg install catch2

# Conan
conan install catch2/3.5.0

# Homebrew (macOS)
brew install catch2

# apt (Ubuntu/Debian)
sudo apt-get install catch2

# pacman (Arch Linux)
sudo pacman -S catch2
```

**方式二：单头文件（最快集成）**

```bash
# 下载单头文件
wget https://github.com/catchorg/Catch2/releases/download/v3.5.0/catch.hpp

# 或使用 curl
curl -L https://github.com/catchorg/Catch2/releases/download/v3.5.0/catch.hpp -o catch.hpp

# 放置到项目中
mkdir -p third_party/catch2
mv catch.hpp third_party/catch2/
```

**方式三：CMake FetchContent**

```cmake
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)
```

**方式四：Git 子模块**

```bash
# 添加子模块
git submodule add https://github.com/catchorg/Catch2.git third_party/Catch2

# 更新子模块
git submodule update --init --recursive
```

### 2.2 版本管理

```bash
# 检查当前版本
cat catch.hpp | grep "CATCH_VERSION"

# CMake 中指定版本
find_package(Catch2 3.5.0 REQUIRED)

# Conan 中指定版本
conan install catch2/3.5.0@

# vcpkg 中指定版本
vcpkg install catch2@3.5.0
```

### 2.3 环境配置

**CMakeLists.txt 配置：**

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 方式一：find_package（vcpkg/Conan）
find_package(Catch2 REQUIRED)

# 方式二：FetchContent（已在安装部分配置）

# 创建测试可执行文件
add_executable(tests
    test_main.cpp
    test_math.cpp
    test_vector.cpp
)

# 链接 Catch2（自动提供 main）
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# 启用测试
enable_testing()
add_test(NAME MyTests COMMAND tests)
```

**自定义 main 函数：**

```cpp
#define CATCH_CONFIG_MAIN  // 生成 main 函数
#include <catch2/catch_test_macros.hpp>

// 或者自定义 main
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_test_macros.hpp>

int main(int argc, char* argv[]) {
    // 自定义初始化
    Catch::Session session;
    
    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) {
        return returnCode;
    }
    
    // 运行测试
    return session.run();
}
```

### 2.4 验证安装

```cpp
// test_install.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_version.hpp>

TEST_CASE("Catch2 installation verification", "[install]") {
    // 检查版本
    REQUIRE(CATCH_VERSION_MAJOR == 3);
    REQUIRE(CATCH_VERSION_MINOR >= 5);
    
    // 基本功能测试
    REQUIRE(1 + 1 == 2);
}

TEST_CASE("Catch2 features", "[features]") {
    // SECTION 支持
    SECTION("Basic assertion") {
        REQUIRE(true);
    }
    
    SECTION("Exception support") {
        REQUIRE_THROWS(throw std::runtime_error("test"));
    }
}
```

```bash
# 编译运行
g++ -std=c++17 test_install.cpp -o test_install
./test_install

# 应该看到类似输出
# All tests passed (2 assertions in 2 test cases)
```

## 3. 基础使用

### 3.1 快速入门

**最小示例：**

```cpp
#include <catch2/catch_test_macros.hpp>

TEST_CASE("First test", "[quickstart]") {
    REQUIRE(1 + 1 == 2);
}
```

编译运行：
```bash
g++ -std=c++17 test.cpp -o test
./test
```

**完整示例：**

```cpp
#include <catch2/catch_test_macros.hpp>

// 被测试的函数
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

TEST_CASE("Math operations", "[math]") {
    SECTION("Addition") {
        REQUIRE(add(2, 3) == 5);
        REQUIRE(add(-1, 1) == 0);
    }
    
    SECTION("Multiplication") {
        REQUIRE(multiply(2, 3) == 6);
        REQUIRE(multiply(0, 5) == 0);
    }
}
```

### 3.2 项目结构

```
my_project/
├── CMakeLists.txt
├── src/
│   ├── math.cpp
│   └── utils.cpp
├── include/
│   ├── math.h
│   └── utils.h
└── tests/
    ├── CMakeLists.txt
    ├── test_main.cpp         # 可选的自定义 main
    ├── test_math.cpp
    ├── test_utils.cpp
    └── test_integration.cpp
```

**tests/CMakeLists.txt：**

```cmake
add_executable(tests
    test_main.cpp
    test_math.cpp
    test_utils.cpp
)

target_link_libraries(tests
    PRIVATE
        Catch2::Catch2WithMain
        my_project_lib  # 被测试的库
)

target_include_directories(tests
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

enable_testing()
add_test(NAME AllTests COMMAND tests)
```

### 3.3 基本命令

```bash
# 运行所有测试
./tests

# 运行特定测试用例
./tests "Math operations"

# 运行特定标签
./tests "[math]"
./tests "[quickstart]"

# 排除标签
./tests "~[slow]"

# 组合标签
./tests "[math][fast]"

# 详细输出
./tests -s

# 显示成功测试
./tests -s -a

# 列出所有测试
./tests --list-tests

# 列出所有标签
./tests --list-tags

# 指定输出报告格式
./tests -r console
./tests -r compact
./tests -r junit
./tests -r xml

# 输出到文件
./tests -r junit -o report.xml

# 设置测试顺序
./tests --order rand      # 随机
./tests --order declared  # 声明顺序

# 显示耗时
./tests -d yes

# 失败时中断
./tests --abort
./tests --abortx 5  # 失败 5 次后中断
```

### 3.4 常用操作

**测试用例定义：**

```cpp
#include <catch2/catch_test_macros.hpp>

// 基本测试
TEST_CASE("Test name", "[tag1][tag2]") {
    REQUIRE(condition);
}

// 带描述的测试
TEST_CASE("Test name", "[tag]")
{
    INFO("This is additional information");
    REQUIRE(true);
}

// 嵌套测试
TEST_CASE("Nested test", "[parent]") {
    int x = 0;
    
    SECTION("First section") {
        x = 1;
        REQUIRE(x == 1);
    }
    
    SECTION("Second section") {
        x = 2;
        REQUIRE(x == 2);
    }
    
    // 每个 SECTION 都从 TEST_CASE 开头开始执行
    REQUIRE(x == 0);  // 每个 SECTION 执行前 x 都是 0
}
```

**断言使用：**

```cpp
TEST_CASE("Assertions", "[assertions]") {
    // 基本断言
    REQUIRE(true);           // 必须为真，失败终止
    CHECK(true);             // 检查为真，失败继续
    
    // 假断言
    REQUIRE_FALSE(false);
    CHECK_FALSE(false);
    
    // 相等断言
    REQUIRE(1 + 1 == 2);
    CHECK(2 * 2 == 4);
    
    // 比较断言
    int a = 1, b = 2;
    REQUIRE_LT(a, b);  // a < b
    REQUIRE_LE(a, b);  // a <= b
    REQUIRE_GT(b, a);  // b > a
    REQUIRE_GE(b, a);  // b >= a
    REQUIRE_EQ(a, 1);  // a == 1
    REQUIRE_NE(a, b);  // a != b
    
    // 字符串断言
    std::string s = "hello";
    REQUIRE(s == "hello");
    REQUIRE(s.size() == 5);
}
```

## 4. 进阶特性

### 4.1 高级配置

**测试配置：**

```cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>

// 自定义配置
struct TestConfig {
    static std::string test_data_dir;
    static int verbose_level;
};

std::string TestConfig::test_data_dir = "./data";
int TestConfig::verbose_level = 1;

// 自定义 main
int main(int argc, char* argv[]) {
    Catch::Session session;
    
    // 添加命令行选项
    using namespace Catch::Clara;
    auto cli = session.cli() 
        | Opt(TestConfig::test_data_dir, "dir")
            ["--test-data-dir"]
            ("Test data directory")
        | Opt(TestConfig::verbose_level, "level")
            ["--verbose"]
            ("Verbose level");
    
    session.cli(cli);
    
    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) {
        return returnCode;
    }
    
    return session.run();
}
```

**测试生命周期钩子：**

```cpp
#include <catch2/catch_test_macros.hpp>

struct Listener : Catch::EventListenerBase {
    using EventListenerBase::EventListenerBase;
    
    void testRunStarting(Catch::TestRunInfo const& testRunInfo) override {
        std::cout << "Starting test run: " << testRunInfo.name << std::endl;
    }
    
    void testRunEnded(Catch::TestRunStats const& testRunStats) override {
        std::cout << "Test run completed" << std::endl;
    }
    
    void testCaseStarting(Catch::TestCaseInfo const& testInfo) override {
        std::cout << "Starting test: " << testInfo.name << std::endl;
    }
    
    void testCaseEnded(Catch::TestCaseStats const& testCaseStats) override {
        std::cout << "Test completed: " << testCaseStats.testInfo.name << std::endl;
    }
};

CATCH_REGISTER_LISTENER(Listener)
```

### 4.2 扩展功能

**浮点数比较：**

```cpp
#include <catch2/catch_approx.hpp>

TEST_CASE("Floating point", "[float]") {
    // 使用 Approx
    REQUIRE(0.1 + 0.2 == Catch::Approx(0.3));
    
    // 设置误差范围
    double result = 1.23456789;
    REQUIRE(result == Catch::Approx(1.234).epsilon(0.01));  // 1% 误差
    REQUIRE(result == Catch::Approx(1.234).margin(0.01));  // 绝对误差
    
    // 链式设置
    REQUIRE(result == Catch::Approx(1.234)
        .epsilon(0.01)
        .margin(0.001));
}
```

**异常测试：**

```cpp
#include <stdexcept>

void throw_runtime() { throw std::runtime_error("error"); }
void throw_logic() { throw std::logic_error("logic error"); }
void no_throw() {}

TEST_CASE("Exception tests", "[exception]") {
    // 要求抛出任何异常
    REQUIRE_THROWS(throw_runtime());
    
    // 要求抛出特定异常
    REQUIRE_THROWS_AS(throw_runtime(), std::runtime_error);
    REQUIRE_THROWS_AS(throw_logic(), std::logic_error);
    
    // 要求不抛出异常
    REQUIRE_NOTHROW(no_throw());
    
    // 检查异常消息
    REQUIRE_THROWS_WITH(throw_runtime(), "error");
    REQUIRE_THROWS_WITH(throw_runtime(), Catch::ContainsSubstring("err"));
    
    // CHECK 版本（失败继续）
    CHECK_THROWS(throw_runtime());
    CHECK_THROWS_AS(throw_runtime(), std::runtime_error);
}
```

**参数化测试：**

```cpp
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

TEST_CASE("Parameterized tests", "[param]") {
    // 使用 GENERATE
    auto value = GENERATE(1, 2, 3, 4, 5);
    REQUIRE(value > 0);
    
    // 范围生成
    auto i = GENERATE(range(1, 10));
    REQUIRE(i > 0);
    REQUIRE(i < 10);
    
    // 组合生成
    auto x = GENERATE(1, 2, 3);
    auto y = GENERATE(10, 20);
    REQUIRE(x < y);
}

// 表驱动测试
TEST_CASE("Table driven tests", "[table]") {
    auto [input, expected] = GENERATE(
        table<int, int>({
            {1, 1},
            {2, 4},
            {3, 9},
            {4, 16},
            {5, 25}
        })
    );
    
    REQUIRE(input * input == expected);
}

// 自定义生成器
std::vector<int> fibonacci_numbers() {
    return {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};
}

TEST_CASE("Custom generator", "[custom]") {
    auto fib = GENERATE_COPY(from_range(fibonacci_numbers()));
    REQUIRE(fib > 0);
}
```

### 4.3 插件生态

**自定义报告器：**

```cpp
#include <catch2/reporters/catch_reporter_streaming_base.hpp>
#include <fstream>

class JsonReporter : public Catch::StreamingReporterBase {
public:
    using StreamingReporterBase::StreamingReporterBase;
    
    void testRunStarting(Catch::TestRunInfo const& runInfo) override {
        stream << "{\"testRun\":\"" << runInfo.name << "\",";
        stream << "\"tests\":[\n";
        first_test = true;
    }
    
    void testCaseEnded(Catch::TestCaseStats const& stats) override {
        if (!first_test) stream << ",";
        first_test = false;
        
        stream << "{\"name\":\"" << stats.testInfo.name << "\",";
        stream << "\"passed\":" << (stats.totals.assertions.allPassed() ? "true" : "false");
        stream << "}\n";
    }
    
    void testRunEnded(Catch::TestRunStats const& stats) override {
        stream << "]}\n";
    }
    
private:
    bool first_test = true;
};

CATCH_REGISTER_REPORTER("json", JsonReporter)
```

**自定义匹配器：**

```cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

class StartsWithMatcher : public Catch::Matchers::MatcherBase<std::string> {
public:
    StartsWithMatcher(std::string prefix) : m_prefix(std::move(prefix)) {}
    
    bool match(std::string const& s) const override {
        return s.size() >= m_prefix.size() && 
               s.substr(0, m_prefix.size()) == m_prefix;
    }
    
    std::string describe() const override {
        return "starts with: " + m_prefix;
    }
    
private:
    std::string m_prefix;
};

auto StartsWith(std::string prefix) {
    return StartsWithMatcher(std::move(prefix));
}

TEST_CASE("Custom matcher", "[matcher]") {
    std::string s = "Hello World";
    REQUIRE_THAT(s, StartsWith("Hello"));
    REQUIRE_THAT(s, !StartsWith("World"));
}
```

## 5. 性能优化

### 5.1 调优策略

**编译速度优化：**

```cmake
# 使用预编译头
target_precompile_headers(tests
    PRIVATE
        <catch2/catch_test_macros.hpp>
)

# 分离编译单元
add_library(test_utils test_utils.cpp)
target_link_libraries(tests PRIVATE test_utils)

# 减少编译警告
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tests PRIVATE -w)  # 仅测试代码
endif()
```

**运行速度优化：**

```cpp
#include <catch2/catch_test_macros.hpp>

// 使用 TEST_CASE_PERSISTENT_FIXTURE 避免重复初始化
struct ExpensiveSetup {
    ExpensiveSetup() {
        // 只执行一次
        heavy_initialization();
    }
    
    void heavy_initialization() {
        // 耗时操作
    }
};

TEST_CASE_PERSISTENT_FIXTURE(ExpensiveSetup, "Fast test", "[fast]") {
    // 每个测试共享同一个 setup 实例
}

// 使用 GENERATE 优化参数化测试
TEST_CASE("Optimized param test", "[opt]") {
    auto value = GENERATE(1, 2, 3, 4, 5);
    // 所有值在同一个测试用例中运行
}
```

**并行测试：**

```bash
# 使用 CTest 并行
ctest -j 4

# 使用 pytest-parallel 风格的并行（需要外部工具）
./tests --shard-count 4 --shard-index 0
```

### 5.2 最佳实践

**测试组织：**

```cpp
// ✅ 好的做法：使用标签分类
TEST_CASE("Math add", "[math][unit][fast]") {}
TEST_CASE("Database query", "[database][integration][slow]") {}

// ✅ 好的做法：使用 SECTION 避免重复
TEST_CASE("String operations", "[string]") {
    std::string s = "hello";
    
    SECTION("Append") {
        s += " world";
        REQUIRE(s == "hello world");
    }
    
    SECTION("Clear") {
        s.clear();
        REQUIRE(s.empty());
    }
    
    // s 在每个 SECTION 开始时重置为 "hello"
}

// ❌ 避免：过度嵌套
TEST_CASE("Bad nesting") {
    SECTION("L1") {
        SECTION("L2") {
            SECTION("L3") {
                SECTION("L4") {  // 太深了
                    // ...
                }
            }
        }
    }
}
```

**断言选择：**

```cpp
// ✅ 使用 REQUIRE：关键断言，失败应停止
TEST_CASE("Critical test") {
    auto ptr = get_pointer();
    REQUIRE(ptr != nullptr);  // 失败后无意义继续
    REQUIRE(ptr->value == 42);
}

// ✅ 使用 CHECK：非关键断言，失败可继续
TEST_CASE("Non-critical test") {
    CHECK(value1 == 1);
    CHECK(value2 == 2);  // 即使 value1 失败也继续
    CHECK(value3 == 3);
}

// ✅ 使用 INFO 提供上下文
TEST_CASE("With context") {
    int value = compute();
    INFO("Computed value: " << value);
    CHECK(value > 0);
}

// ✅ 使用 UNSCOPED_INFO（跨 SECTION）
TEST_CASE("Across sections") {
    UNSCOPED_INFO("This persists across sections");
    
    SECTION("First") {
        CHECK(true);
    }
    
    SECTION("Second") {
        CHECK(false);  // 也会显示 UNSCOPED_INFO
    }
}
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：找不到头文件**

```
fatal error: catch2/catch_test_macros.hpp: No such file or directory
```

解决方案：
```cmake
# 方式一：find_package
find_package(Catch2 REQUIRED)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# 方式二：手动包含
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/third_party)
```

**问题 2：链接错误**

```
undefined reference to `Catch::Session::run()'
```

解决方案：
```cmake
# 使用 Catch2WithMain（推荐）
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# 或自定义 main
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_test_macros.hpp>
int main(int argc, char* argv[]) {
    return Catch::Session().run(argc, argv);
}
```

**问题 3：SECTION 执行多次**

```cpp
TEST_CASE("Section behavior") {
    int count = 0;
    SECTION("A") { count++; }
    SECTION("B") { count++; }
    // count 在每个 SECTION 开始时都是 0
}
```

这是预期行为。SECTION 机制会从 TEST_CASE 开头重新执行。

**问题 4：浮点数比较失败**

```cpp
TEST_CASE("Float problem") {
    REQUIRE(0.1 + 0.2 == 0.3);  // 失败！
}

// 解决方案
TEST_CASE("Float solution") {
    REQUIRE(0.1 + 0.2 == Catch::Approx(0.3));
}
```

**问题 5：测试顺序随机**

```bash
# 默认随机顺序
./tests

# 固定顺序
./tests --order declared

# 固定随机种子
./tests --rng-seed 12345
```

### 6.2 调试技巧

**详细输出：**

```bash
# 显示所有测试（包括成功）
./tests -s

# 显示测试持续时间
./tests -d yes

# 显示测试消息
./tests -s -m

# 显示所有断言
./tests -s -a
```

**调试特定测试：**

```bash
# 只运行失败的测试
./tests -f

# 运行特定测试
./tests "Specific test name"

# 使用标签过滤
./tests "[debug]"

# 详细输出特定测试
./tests "Test name" -s
```

**使用调试器：**

```bash
# 使用 lldb
lldb ./tests
(lldb) run "Test name"

# 使用 gdb
gdb ./tests
(gdb) run "Test name"

# VSCode launch.json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Catch2 Test",
            "type": "cppdbg",
            "program": "${workspaceFolder}/build/tests",
            "args": ["\"Test name\""],
            "cwd": "${workspaceFolder}"
        }
    ]
}
```

**失败诊断：**

```cpp
TEST_CASE("Diagnosis", "[debug]") {
    int result = complex_calculation();
    
    // 使用 INFO 提供上下文
    INFO("Input parameters: a=1, b=2");
    INFO("Intermediate result: " << intermediate);
    CHECK(result == expected);
    
    // 使用 REQUIRE 检查前置条件
    auto ptr = get_data();
    REQUIRE(ptr != nullptr);
    
    // 使用 CAPTURE 自动显示变量值
    int x = 10, y = 20;
    CAPTURE(x, y);
    CHECK(x + y == 30);
}
```

## 7. 集成实践

### 7.1 工具链集成

**CMake 集成：**

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject)

# 启用测试
enable_testing()

# 查找 Catch2
find_package(Catch2 REQUIRED)

# 测试目标
add_executable(tests
    test_main.cpp
    test_module1.cpp
    test_module2.cpp
)

target_link_libraries(tests
    PRIVATE
        Catch2::Catch2WithMain
        my_project_lib
)

# 注册测试
add_test(NAME UnitTests COMMAND tests)
add_test(NAME SpecificTest COMMAND tests "[specific]")

# 自定义测试目标
add_custom_target(run_tests
    COMMAND tests -s
    DEPENDS tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
```

**CTest 集成：**

```cmake
# CMakeLists.txt
enable_testing()

add_test(NAME FastTests COMMAND tests "[fast]")
add_test(NAME SlowTests COMMAND tests "[slow]")
add_test(NAME AllTests COMMAND tests)

# 设置测试属性
set_tests_properties(FastTests PROPERTIES
    TIMEOUT 60
    LABELS "unit"
)

set_tests_properties(SlowTests PROPERTIES
    TIMEOUT 300
    LABELS "integration"
)
```

```bash
# 运行测试
ctest

# 详细输出
ctest --output-on-failure

# 并行运行
ctest -j 4

# 按标签过滤
ctest -L unit

# 生成报告
ctest -T Test
```

**Makefile 集成：**

```makefile
# Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./third_party/catch2

TEST_SRC = $(wildcard tests/*.cpp)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

test: $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

run_test: test
	./test -s

clean:
	rm -f test $(TEST_OBJ)

.PHONY: test run_test clean
```

### 7.2 CI/CD 配置

**GitHub Actions：**

```yaml
# .github/workflows/test.yml
name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        build_type: [Debug, Release]
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install Catch2 (Ubuntu/macOS)
      if: runner.os != 'Windows'
      run: |
        git clone https://github.com/catchorg/Catch2.git
        cd Catch2
        cmake -Bbuild -H. -DBUILD_TESTING=OFF
        sudo cmake --build build --target install
    
    - name: Configure
      run: cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=${{ matrix.build_type }}
    
    - name: Build
      run: cmake --build build --config ${{ matrix.build_type }}
    
    - name: Test
      run: |
        cd build
        ctest --build-config ${{ matrix.build_type }} --output-on-failure
    
    - name: Upload Test Results
      if: always()
      uses: actions/upload-artifact@v3
      with:
        name: test-results-${{ matrix.os }}
        path: build/report.xml
```

**GitLab CI：**

```yaml
# .gitlab-ci.yml
test:
  image: gcc:latest
  stage: test
  script:
    - apt-get update && apt-get install -y cmake
    - mkdir build && cd build
    - cmake -DCMAKE_BUILD_TYPE=Debug ..
    - make -j$(nproc)
    - ctest --output-on-failure -T Test
  artifacts:
    reports:
      junit: build/report.xml
    paths:
      - build/report.xml
    expire_in: 1 week
  coverage: '/lines: \d+\.\d+%/'
```

**Jenkins Pipeline：**

```groovy
// Jenkinsfile
pipeline {
    agent any
    
    stages {
        stage('Build') {
            steps {
                sh 'mkdir -p build && cd build'
                sh 'cmake -DCMAKE_BUILD_TYPE=Debug ..'
                sh 'make -j$(nproc)'
            }
        }
        
        stage('Test') {
            steps {
                sh 'cd build && ctest --output-on-failure -T Test'
            }
            post {
                always {
                    junit 'build/report.xml'
                    archiveArtifacts artifacts: 'build/report.xml'
                }
            }
        }
    }
}
```

### 7.3 实战案例

**案例一：计算器测试**

```cpp
// test_calculator.cpp
#include <catch2/catch_test_macros.hpp>
#include "calculator.h"

class CalculatorFixture {
protected:
    Calculator calc;
    
    CalculatorFixture() {
        calc.clear();
    }
};

TEST_CASE_METHOD(CalculatorFixture, "Basic operations", "[calculator]") {
    SECTION("Addition") {
        REQUIRE(calc.add(2, 3) == 5);
        REQUIRE(calc.add(-1, 1) == 0);
        REQUIRE(calc.add(0, 0) == 0);
    }
    
    SECTION("Subtraction") {
        REQUIRE(calc.subtract(5, 3) == 2);
        REQUIRE(calc.subtract(3, 5) == -2);
    }
    
    SECTION("Multiplication") {
        REQUIRE(calc.multiply(2, 3) == 6);
        REQUIRE(calc.multiply(0, 5) == 0);
        REQUIRE(calc.multiply(-2, 3) == -6);
    }
    
    SECTION("Division") {
        REQUIRE(calc.divide(6, 3) == 2);
        
        REQUIRE_THROWS_AS(calc.divide(1, 0), std::runtime_error);
    }
}

TEST_CASE_METHOD(CalculatorFixture, "History tracking", "[calculator]") {
    calc.add(2, 3);
    calc.multiply(4, 5);
    
    auto history = calc.get_history();
    REQUIRE(history.size() == 2);
    REQUIRE(history[0].operation == "add");
    REQUIRE(history[1].result == 20);
}
```

**案例二：数据库测试**

```cpp
// test_database.cpp
#include <catch2/catch_test_macros.hpp>
#include "database.h"

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

TEST_CASE_METHOD(DatabaseFixture, "Database operations", "[database]") {
    SECTION("Insert and query") {
        db.execute("INSERT INTO users VALUES (1, 'Alice')");
        db.execute("INSERT INTO users VALUES (2, 'Bob')");
        
        auto results = db.query("SELECT * FROM users");
        REQUIRE(results.size() == 2);
        REQUIRE(results[0]["name"] == "Alice");
    }
    
    SECTION("Transaction") {
        db.begin_transaction();
        db.execute("INSERT INTO users VALUES (1, 'Alice')");
        db.rollback();
        
        auto results = db.query("SELECT * FROM users");
        REQUIRE(results.empty());
    }
    
    SECTION("Error handling") {
        REQUIRE_THROWS_AS(
            db.execute("INVALID SQL"),
            DatabaseError
        );
    }
}

// 参数化测试
TEST_CASE_METHOD(DatabaseFixture, "Multiple inserts", "[database]") {
    auto [id, name] = GENERATE(
        table<int, std::string>({
            {1, "Alice"},
            {2, "Bob"},
            {3, "Charlie"}
        })
    );
    
    db.execute("INSERT INTO users VALUES (?, ?)", id, name);
    
    auto results = db.query("SELECT * FROM users WHERE id = ?", id);
    REQUIRE(results.size() == 1);
    REQUIRE(results[0]["name"] == name);
}
```

**案例三：BDD 风格测试**

```cpp
// test_user_management.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/bdd/catch_bdd.hpp>
#include "user_manager.h"

SCENARIO("User management", "[user][bdd]") {
    GIVEN("An empty user database") {
        UserManager manager;
        
        WHEN("A new user is registered") {
            User user{"alice", "alice@example.com", "password123"};
            bool success = manager.register_user(user);
            
            THEN("Registration succeeds") {
                REQUIRE(success);
            }
            
            AND_THEN("User count is 1") {
                REQUIRE(manager.user_count() == 1);
            }
            
            AND_THEN("User can be found") {
                auto found = manager.find_user("alice");
                REQUIRE(found.has_value());
                REQUIRE(found->email == "alice@example.com");
            }
        }
        
        WHEN("Attempting to register duplicate user") {
            User user1{"alice", "alice@example.com", "pass1"};
            User user2{"alice", "alice2@example.com", "pass2"};
            
            manager.register_user(user1);
            bool success = manager.register_user(user2);
            
            THEN("Registration fails") {
                REQUIRE_FALSE(success);
            }
            
            AND_THEN("User count remains 1") {
                REQUIRE(manager.user_count() == 1);
            }
        }
    }
    
    GIVEN("A user with valid credentials") {
        UserManager manager;
        User user{"bob", "bob@example.com", "secret123"};
        manager.register_user(user);
        
        WHEN("User logs in with correct password") {
            auto session = manager.login("bob", "secret123");
            
            THEN("Login succeeds") {
                REQUIRE(session.has_value());
            }
            
            AND_THEN("Session is valid") {
                REQUIRE(session->is_valid());
            }
        }
        
        WHEN("User logs in with wrong password") {
            auto session = manager.login("bob", "wrongpass");
            
            THEN("Login fails") {
                REQUIRE_FALSE(session.has_value());
            }
        }
    }
}
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 | 说明 |
|------|------|------|
| 官方仓库 | https://github.com/catchorg/Catch2 | 源码和问题追踪 |
| 官方文档 | https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md | 完整文档 |
| 发布版本 | https://github.com/catchorg/Catch2/releases | 下载地址 |
| Tutorial | https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md | 入门教程 |
| Reference | https://github.com/catchorg/Catch2/blob/devel/docs/reference-index.md | API 参考 |

**推荐阅读顺序：**
1. Tutorial - 快速入门
2. Test cases and sections - 测试组织
3. Assertions - 断言详解
4. Generators - 参数化测试
5. Logging - 日志和消息
6. Reporters - 报告输出

### 8.2 学习路径

**初级路径（1-2 周）：**

```
第 1 天：安装配置
├── 单头文件集成
├── CMake 配置
└── 第一个测试用例

第 2-3 天：基础语法
├── TEST_CASE 和 SECTION
├── REQUIRE 和 CHECK
└── 基本断言

第 4-5 天：测试组织
├── 标签系统
├── 测试过滤
└── 命令行选项

第 6-7 天：实践项目
├── 为现有代码编写测试
└── 集成到构建系统
```

**中级路径（2-3 周）：**

```
第 1 周：进阶特性
├── 参数化测试
├── 测试夹具
├── BDD 风格
└── 异常测试

第 2 周：集成实践
├── CMake 深度集成
├── CTest 使用
├── CI/CD 配置
└── 报告生成

第 3 周：最佳实践
├── 测试设计模式
├── 性能优化
├── 大型项目组织
└── 调试技巧
```

**高级路径（持续）：**

```
持续学习：
├── 自定义报告器
├── 自定义匹配器
├── 性能基准测试
├── 并行测试
└── 贡献代码
```

**推荐项目实践：**

1. **入门项目**：为简单数学库编写测试
2. **进阶项目**：为字符串处理库编写 BDD 风格测试
3. **实战项目**：为小型数据库编写完整测试套件
4. **高级项目**：为网络库编写集成测试和性能测试

**社区资源：**

| 资源 | 说明 |
|------|------|
| GitHub Issues | 问题追踪和讨论 |
| Stack Overflow | `catch2` 标签 |
| Reddit | r/cpp 社区 |
| CppCon 演讲 | "Modern C++ Testing with Catch2" |

**对比学习资源：**

- GoogleTest 对比文档
- doctest 性能对比
- Boost.Test 功能对比

通过本指南，您应该能够全面掌握 Catch2 的使用，从基础测试到高级特性，从个人项目到企业级应用。