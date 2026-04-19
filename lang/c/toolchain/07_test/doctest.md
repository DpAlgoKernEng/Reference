# doctest - 最快的 C++ 测试框架

## 1. 概述与背景

### 1.1 工具定位

doctest 是一个轻量级 C++ 测试框架，核心特点是最快的编译速度、单头文件设计、零外部依赖。它专为 C++11 及以上版本设计，以编译时性能著称，相比其他主流测试框架（Catch2、GoogleTest）可节省 2-10 倍编译时间。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2016 | 1.0 | 初始发布，核心测试功能 |
| 2018 | 2.0 | 性能优化，子测试支持 |
| 2019 | 2.2 | BDD 风格支持，报告器改进 |
| 2021 | 2.4 | C++20 支持，并行测试 |
| 2023 | 2.4.11 | 当前稳定版本，持续优化 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 最快编译 | 比 Catch2 快 2-3 倍，比 GoogleTest 快 5-10 倍 |
| 单头文件 | 仅需 `doctest.h`，集成极简 |
| 零依赖 | 无外部库依赖，标准库即可 |
| 表达式解析 | 断言失败时显示完整表达式 |
| 子测试 | SUBCASE 提供嵌套测试隔离 |
| BDD 风格 | GIVEN/WHEN/THEN 自然语言测试 |
| 参数化 | GENERATE 灵活数据驱动测试 |

### 1.4 适用场景

| 场景 | 推荐 |
|------|------|
| 编译速度优先 | ✅ doctest |
| 单头文件需求 | ✅ doctest |
| 大型项目测试 | doctest/Catch2 |
| 需要 Mock | GoogleTest |
| CI/CD 快速构建 | ✅ doctest |
| 嵌入式开发 | ✅ doctest（轻量） |
| 头文件库测试 | ✅ doctest |

### 1.5 对比分析

| 特性 | doctest | Catch2 | GoogleTest |
|------|---------|--------|------------|
| 编译速度 | 最快 | 快 | 中 |
| 头文件 | 单头 | 单头 | 多头 |
| Mock 支持 | ❌ | ❌ | ✅ |
| BDD 风格 | ✅ | ✅ | ❌ |
| 参数化 | GENERATE | GENERATE | TEST_P |
| 子测试 | SUBCASE | SECTION | ❌ |
| 自定义报告器 | ✅ | ✅ | ✅ |
| 并行测试 | ✅ | ✅ | ✅ |
| 学习曲线 | 低 | 低 | 中 |

## 2. 安装与配置

### 2.1 多平台安装

**方式一：单头文件（推荐）**

```bash
# 下载最新版本
wget https://github.com/doctest/doctest/releases/download/v2.4.11/doctest.h

# 或使用 curl
curl -L -o doctest.h https://github.com/doctest/doctest/releases/download/v2.4.11/doctest.h
```

**方式二：vcpkg 包管理器**

```bash
# 安装 vcpkg（如未安装）
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh

# 安装 doctest
vcpkg install doctest
```

**方式三：CMake FetchContent**

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

```cmake
# 检查最低版本
find_package(doctest 2.4 REQUIRED)

# 或在代码中检查
#include <doctest/doctest.h>
static_assert(doctest::version::major >= 2);
```

### 2.3 环境配置

**项目目录结构：**

```
project/
├── CMakeLists.txt
├── include/
├── src/
├── tests/
│   ├── CMakeLists.txt
│   ├── test_main.cpp
│   └── test_math.cpp
└── third/
    └── doctest/
        └── doctest.h
```

### 2.4 验证安装

```cpp
// test_main.cpp - 最小测试程序
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("Installation check") {
    CHECK(true);
}
```

```bash
# 编译运行
g++ -std=c++17 test_main.cpp -o test_program
./test_program

# 预期输出
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases: 1 | 1 passed | 0 failed | 0 skipped
[doctest] assertions: 1 | 1 passed | 0 failed |
[doctest] Status: SUCCESS!
```

## 3. 基础使用

### 3.1 快速入门

**最简测试：**

```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("Basic math") {
    CHECK(1 + 1 == 2);
    CHECK(2 * 3 == 6);
}
```

**自定义主函数：**

```cpp
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

TEST_CASE("Custom main") {
    CHECK(true);
}
```

### 3.2 项目结构

**多文件测试组织：**

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

int add(int a, int b) { return a + b; }

TEST_CASE("Math operations") {
    CHECK(add(1, 2) == 3);
}
```

### 3.3 基本命令

| 命令选项 | 说明 |
|---------|------|
| `-tc="Name"` | 运行指定测试用例 |
| `-ts="Suite"` | 运行指定测试套件 |
| `-sf=file.cpp` | 运行指定源文件测试 |
| `-s` | 显示成功测试详情 |
| `-r=xml` | XML 格式输出 |
| `-np=4` | 4 线程并行运行 |

```bash
# 运行所有测试
./test_program

# 运行特定测试
./test_program -tc="Addition"

# 运行特定文件
./test_program -sf=math.cpp

# 详细输出
./test_program -s

# 生成 XML 报告
./test_program -r=xml -o=report.xml

# 并行运行
./test_program -np=4
```

### 3.4 常用操作

**测试过滤：**

```bash
# 通配符匹配
./test_program -tc="Math*"

# 排除测试
./test_program -tc="!SlowTest"

# 组合条件
./test_program -ts="Core" -tc="!Skip*"
```

## 4. 进阶特性

### 4.1 高级配置

**编译期配置：**

```cpp
// 禁用测试（生产环境）
#define DOCTEST_CONFIG_DISABLE

// 减少编译时间
#define DOCTEST_CONFIG_NO_UNARY_MACRO

// 自定义命名空间
#define DOCTEST_CONFIG_NAMESPACE mytest

// 多线程支持
#define DOCTEST_CONFIG_POSIX_SIGNALS
```

### 4.2 断言详解

**基本断言：**

| 断言 | 说明 | 失败后 |
|------|------|--------|
| `CHECK(expr)` | 检查表达式 | 继续测试 |
| `REQUIRE(expr)` | 要求表达式 | 终止测试 |
| `CHECK_FALSE(expr)` | 检查为假 | 继续 |
| `REQUIRE_FALSE(expr)` | 要求为假 | 终止 |

**比较断言：**

| 断言 | 说明 |
|------|------|
| `CHECK_EQ(a, b)` | 相等 |
| `CHECK_NE(a, b)` | 不等 |
| `CHECK_LT(a, b)` | 小于 |
| `CHECK_LE(a, b)` | 小于等于 |
| `CHECK_GT(a, b)` | 大于 |
| `CHECK_GE(a, b)` | 大于等于 |

**异常断言：**

```cpp
// 要求抛出异常
CHECK_THROWS(throw_exception());

// 要求特定异常
CHECK_THROWS_AS(throw_runtime(), std::runtime_error);

// 要求异常消息
CHECK_THROWS_WITH(throw_error(), "error message");

// 要求不抛出异常
CHECK_NOTHROW(safe_function());
```

**快速断言（无表达式解析，编译更快）：**

```cpp
FAST_CHECK_EQ(a, b);       // 快速相等检查
FAST_CHECK_UNARY(expr);    // 快速真值检查
FAST_REQUIRE_EQ(a, b);     // 快速要求相等
```

### 4.3 测试组织

**测试夹具（Fixture）：**

```cpp
struct VectorFixture {
    std::vector<int> vec = {1, 2, 3};
    
    VectorFixture() {
        // 每个子测试前执行
    }
    
    ~VectorFixture() {
        // 每个子测试后执行
    }
};

TEST_CASE_FIXTURE(VectorFixture, "Vector operations") {
    CHECK(vec.size() == 3);
}
```

**子测试（SUBCASE）：**

```cpp
TEST_CASE("Nested subcases") {
    int x = 0;
    
    SUBCASE("Initialize") {
        x = 10;
        CHECK(x == 10);
    }
    
    SUBCASE("Modify") {
        x = 20;
        CHECK(x == 20);
    }
    
    // SUBCASE 之间独立执行，x 每次重置为 0
}
```

**测试套件：**

```cpp
TEST_SUITE("Math operations") {
    TEST_CASE("Addition") {
        CHECK(add(1, 1) == 2);
    }
    
    TEST_CASE("Multiplication") {
        CHECK(mul(2, 3) == 6);
    }
}

// 运行套件
// ./test_program -ts="Math operations"
```

### 4.4 参数化测试

```cpp
// 使用生成器
TEST_CASE("Parameterized values") {
    int value = GENERATE(1, 2, 3, 4, 5);
    CHECK(value > 0);
}

// 范围生成
TEST_CASE("Range values") {
    int i = GENERATE(range(1, 10));
    CHECK(i >= 1 && i < 10);
}

// 组合生成
TEST_CASE("Combined values") {
    int a = GENERATE(1, 2, 3);
    int b = GENERATE(10, 20);
    CHECK(a + b > a);
}
```

### 4.5 BDD 风格

```cpp
SCENARIO("Vector resizing") {
    GIVEN("An empty vector") {
        std::vector<int> v;
        
        WHEN("Push an element") {
            v.push_back(1);
            
            THEN("Size increases") {
                CHECK(v.size() == 1);
            }
            
            THEN("Not empty") {
                CHECK(!v.empty());
            }
        }
    }
}
```

### 4.6 报告器

```bash
# 内置报告器
./test_program -r=console   # 默认控制台输出
./test_program -r=compact   # 紧凑输出
./test_program -r=xml       # XML 格式
./test_program -r=junit     # JUnit 格式
```

## 5. 性能优化

### 5.1 编译优化

**减少编译时间：**

```cpp
// 方式一：完全禁用测试
#define DOCTEST_CONFIG_DISABLE

// 方式二：减少宏展开
#define DOCTEST_CONFIG_NO_UNARY_MACRO

// 方式三：使用快速断言
FAST_CHECK_EQ(a, b);  // 比 CHECK(a == b) 编译更快
```

**分离测试实现：**

```cpp
// test_runner.cpp - 仅一次编译
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char** argv) {
    doctest::Context context;
    return context.run();
}

// test_*.cpp - 仅包含测试，可并行编译
#include <doctest/doctest.h>
TEST_CASE("...") { /* ... */ }
```

### 5.2 运行速度

```bash
# 并行运行测试
./test_program -np=4

# 时间测量
./test_program -d=true

# 优先级排序（慢测试后运行）
./test_program -ob=time
```

### 5.3 最佳实践

| 实践 | 说明 |
|------|------|
| 独立测试 | 测试间无依赖，可并行执行 |
| 快速断言 | 性能关键处用 FAST_CHECK_* |
| 分离实现 | 主函数单独编译，测试并行编译 |
| 合理 SUBCASE | 避免过度嵌套影响可读性 |
| 测试分组 | 使用 TEST_SUITE 组织相关测试 |

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| 找不到 doctest.h | 头文件路径未配置 | 添加 `-I/path/to/doctest` |
| 链接错误 | 多次定义主函数 | 仅一个文件定义 `IMPLEMENT` |
| 测试未运行 | 名称匹配失败 | 检查 `-tc` 过滤条件 |
| 编译慢 | 大量断言解析 | 使用 FAST_CHECK_* |
| SUBCASE 不独立 | 变量在 SUBCASE 外修改 | 在 TEST_CASE 内初始化 |

**问题一：链接错误 - 重复定义**

```cpp
// 错误：多个文件定义 IMPLEMENT_WITH_MAIN
// file1.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN  // ❌

// file2.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN  // ❌

// 正确：仅一个文件定义，其他仅包含头文件
// test_main.cpp
#define DOCTEST_CONFIG_IMPLEMENT  // ✅

// test_*.cpp
#include <doctest/doctest.h>  // ✅ 无需定义宏
```

**问题二：SUBCASE 状态污染**

```cpp
// 错误示例
TEST_CASE("Bad subcase") {
    static int counter = 0;  // ❌ 静态变量跨测试共享
    
    SUBCASE("A") { counter++; }
    SUBCASE("B") { CHECK(counter == 0); }  // 可能失败
}

// 正确示例
TEST_CASE("Good subcase") {
    int counter = 0;  // ✅ 每个子测试重新初始化
    
    SUBCASE("A") { counter++; }
    SUBCASE("B") { CHECK(counter == 0); }  // 总是成功
}
```

### 6.2 调试技巧

```bash
# 显示所有测试名称
./test_program --list-test-cases

# 显示测试统计
./test_program --count=true

# 详细失败信息
./test_program -s

# 交互式调试
gdb ./test_program
(gdb) run -tc="FailingTest"
```

## 7. 集成实践

### 7.1 CMake 集成

**单头文件方式：**

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(MyProject)

add_executable(tests
    tests/test_main.cpp
    tests/test_math.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/third/doctest
)

enable_testing()
add_test(NAME MyTests COMMAND tests)
```

**FetchContent 方式：**

```cmake
include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest
    GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE doctest::doctest)
```

**CMake 测试发现：**

```cmake
enable_testing()

# 添加测试
add_test(NAME UnitTests COMMAND tests)

# 添加测试属性
set_tests_properties(UnitTests PROPERTIES
    TIMEOUT 60
    LABELS "unit"
)
```

### 7.2 CI/CD 配置

**GitHub Actions：**

```yaml
name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Configure CMake
        run: cmake -B build -S .
        
      - name: Build
        run: cmake --build build
        
      - name: Run tests
        run: cd build && ctest --output-on-failure
        
      - name: Generate coverage
        run: |
          cd build
          ./tests -r=xml -o=test-results.xml
```

**GitLab CI：**

```yaml
test:
  stage: test
  script:
    - cmake -B build -S .
    - cmake --build build
    - cd build && ctest --output-on-failure
  artifacts:
    reports:
      junit: build/test-results.xml
```

### 7.3 实战案例

**案例一：数学库测试**

```cpp
// math_lib.h
#pragma once
namespace math {
    int add(int a, int b);
    int multiply(int a, int b);
    double divide(double a, double b);
}

// test_math.cpp
#include <doctest/doctest.h>
#include "math_lib.h"

TEST_SUITE("Math library") {
    TEST_CASE("Addition") {
        CHECK(math::add(1, 2) == 3);
        CHECK(math::add(-1, 1) == 0);
        CHECK(math::add(0, 0) == 0);
    }
    
    TEST_CASE("Multiplication") {
        CHECK(math::multiply(2, 3) == 6);
        CHECK(math::multiply(-1, 1) == -1);
    }
    
    TEST_CASE("Division") {
        CHECK(math::divide(6.0, 2.0) == doctest::Approx(3.0));
        CHECK_THROWS_AS(math::divide(1.0, 0.0), std::runtime_error);
    }
}
```

**案例二：字符串处理测试**

```cpp
// string_utils.h
#include <string>
std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, char delim);

// test_string.cpp
#include <doctest/doctest.h>
#include "string_utils.h"

TEST_CASE("String trimming") {
    SUBCASE("Leading spaces") {
        CHECK(trim("  hello") == "hello");
    }
    
    SUBCASE("Trailing spaces") {
        CHECK(trim("world  ") == "world");
    }
    
    SUBCASE("Both sides") {
        CHECK(trim("  both  ") == "both");
    }
}

TEST_CASE("String splitting") {
    auto parts = split("a,b,c", ',');
    CHECK(parts.size() == 3);
    CHECK(parts[0] == "a");
    CHECK(parts[1] == "b");
    CHECK(parts[2] == "c");
}
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| GitHub 仓库 | https://github.com/doctest/doctest |
| 官方文档 | https://github.com/doctest/doctest/blob/master/doc/markdown/readme.md |
| API 参考 | https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md |
| 发行版本 | https://github.com/doctest/doctest/releases |

### 8.2 学习路径

| 阶段 | 内容 |
|------|------|
| 入门 | 单头文件集成、基本断言、TEST_CASE |
| 进阶 | SUBCASE、测试套件、参数化测试 |
| 高级 | BDD 风格、自定义报告器、CI/CD 集成 |
| 实战 | 性能优化、大型项目组织、Mock 替代方案 |

### 8.3 社区资源

| 资源 | 说明 |
|------|------|
| GitHub Issues | 问题反馈与讨论 |
| CppCon 演讲 | 性能对比与最佳实践 |
| Awesome Cpp | 测试框架对比列表 |