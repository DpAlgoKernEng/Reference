# GoogleTest - Google C++ 测试框架

## 1. 概述与背景

### 1.1 工具定位

GoogleTest（gtest）是 Google 开源的 C++ 单元测试框架，与 GoogleMock（gmock）配合使用，提供完整的测试解决方案。作为业界广泛采用的测试框架，GoogleTest 支持自动测试发现、丰富的断言机制、死亡测试、参数化测试等高级特性。

### 1.2 发展历史

| 年份 | 版本 | 里程碑 |
|------|------|--------|
| 2008 | 1.0 | Google 开源发布，成为首批现代 C++ 测试框架 |
| 2013 | 1.7 | 引入类型参数化测试增强功能 |
| 2017 | 1.8 | 合并 GoogleMock，提供完整 Mock 解决方案 |
| 2020 | 1.10 | C++11 最低要求，增强诊断信息 |
| 2022 | 1.12 | 改进 Windows 支持，增强异常处理 |
| 2023 | 1.14 | 完善模块化构建，提升编译速度 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 自动测试发现 | 自动注册测试用例，无需手动维护测试列表 |
| 丰富断言 | 布尔、比较、浮点、字符串等多种断言类型 |
| 测试夹具 | 支持测试前后的 SetUp/TearDown 操作 |
| 参数化测试 | 值参数化、类型参数化测试支持 |
| 死亡测试 | 测试程序崩溃、退出、异常等场景 |
| Mock 框架 | 内置 GoogleMock，支持接口模拟 |
| 测试过滤 | 正则表达式过滤运行指定测试 |
| XML/JSON 报告 | 标准化测试报告输出格式 |

### 1.4 适用场景

| 场景 | 推荐指数 | 说明 |
|------|----------|------|
| 企业级项目 | ★★★★★ | 功能全面，文档完善，社区活跃 |
| 需要 Mock 测试 | ★★★★★ | 原生支持 GoogleMock |
| 参数化测试 | ★★★★☆ | 支持值参数化和类型参数化 |
| 多平台项目 | ★★★★★ | 支持 Linux/macOS/Windows |
| 嵌入式开发 | ★★★☆☆ | 可移植但需配置交叉编译 |
| 快速原型验证 | ★★★☆☆ | 编译较慢，适合稳定项目 |

### 1.5 对比分析

| 特性 | GoogleTest | Catch2 | doctest | Boost.Test |
|------|------------|--------|---------|------------|
| 编译速度 | 中 | 快 | 最快 | 慢 |
| 断言丰富度 | 高 | 中 | 中 | 高 |
| Mock 支持 | ✅ 内置 | ❌ 需第三方 | ❌ 需第三方 | ❌ 需第三方 |
| 参数化测试 | ✅ 完善 | ✅ 支持 | ❌ 有限 | ✅ 完善 |
| 头文件依赖 | 多 | 单头 | 单头 | 多 |
| 学习曲线 | 中 | 低 | 低 | 高 |
| 社区活跃度 | 高 | 高 | 中 | 中 |
| 文档质量 | 高 | 高 | 中 | 中 |

## 2. 安装与配置

### 2.1 多平台安装

**vcpkg 安装（推荐）：**

```bash
# 安装 gtest
vcpkg install gtest

# 指定平台
vcpkg install gtest:x64-linux
vcpkg install gtest:x64-osx
vcpkg install gtest:x64-windows
```

**Conan 安装：**

```bash
# 安装特定版本
conan install gtest/1.14.0@

# conanfile.txt 配置
[requires]
gtest/1.14.0

[generators]
cmake_find_package
cmake_paths
```

**源码编译安装：**

```bash
git clone https://github.com/google/googletest
cd googletest
cmake -B build \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DBUILD_GMOCK=ON \
    -Dgtest_build_tests=OFF
cmake --build build
sudo cmake --install build
```

### 2.2 版本管理

**CMake FetchContent（推荐）：**

```cmake
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.14.0
)

# Windows: 防止覆盖父项目的编译器设置
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
```

**Git Submodule 方式：**

```bash
git submodule add https://github.com/google/googletest third_party/googletest
git submodule update --init --recursive
```

### 2.3 环境配置

**CMakeLists.txt 完整配置：**

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProjectTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 方式一：使用 find_package（系统安装或 vcpkg）
find_package(GTest REQUIRED)

# 方式二：使用 FetchContent（见 2.2 节）

# 创建测试可执行文件
add_executable(tests
    test_main.cpp
    test_math.cpp
    test_string.cpp
)

# 链接 GTest 库
target_link_libraries(tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main  # 提供 main 函数
    GTest::gmock       # 如果使用 Mock
    GTest::gmock_main
)

# 启用 CTest
enable_testing()

# 注册测试
include(GoogleTest)
gtest_discover_tests(tests)
```

### 2.4 验证安装

```cpp
// test_version.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(VersionTest, GTestVersion) {
    EXPECT_STREQ(::testing::internal::kGTestVersion, "1.14.0");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

```bash
# 编译运行
cmake -B build && cmake --build build
./build/tests --gtest_list_tests

# 预期输出
VersionTest.
  GTestVersion
```

## 3. 基础使用

### 3.1 快速入门

**最简单的测试：**

```cpp
#include <gtest/gtest.h>

// 独立测试用例
TEST(SimpleTest, BasicAssertion) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}

// 自动生成 main 函数
// 编译时链接 GTest::gtest_main 即可省略 main 函数
```

**编译运行：**

```bash
g++ -std=c++17 test.cpp -lgtest -lgtest_main -pthread -o test
./test

# 输出示例
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from SimpleTest
[ RUN      ] SimpleTest.BasicAssertion
[       OK ] SimpleTest.BasicAssertion (0 ms)
[----------] 1 test from SimpleTest (0 ms total)
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```

### 3.2 项目结构

**推荐目录结构：**

```
project/
├── src/
│   ├── calculator.cpp
│   └── calculator.h
├── test/
│   ├── CMakeLists.txt
│   ├── test_main.cpp        # 自定义 main（可选）
│   ├── test_calculator.cpp   # 计算器测试
│   ├── test_math.cpp         # 数学模块测试
│   └── fixtures/
│       ├── database_fixture.h
│       └── network_fixture.h
└── CMakeLists.txt
```

**test_main.cpp 自定义入口：**

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// 自定义初始化
class MyEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        // 全局初始化（如数据库连接池）
    }
    void TearDown() override {
        // 全局清理
    }
};

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    // 注册全局环境
    ::testing::AddGlobalTestEnvironment(new MyEnvironment);

    return RUN_ALL_TESTS();
}
```

### 3.3 基本命令

| 命令选项 | 说明 | 示例 |
|----------|------|------|
| `--gtest_filter` | 过滤测试 | `--gtest_filter=MathTest.*` |
| `--gtest_list_tests` | 列出所有测试 | `--gtest_list_tests` |
| `--gtest_output` | 输出报告 | `--gtest_output=xml:report.xml` |
| `--gtest_repeat` | 重复运行 | `--gtest_repeat=10` |
| `--gtest_shuffle` | 随机顺序 | `--gtest_shuffle` |
| `--gtest_random_seed` | 随机种子 | `--gtest_random_seed=42` |
| `--gtest_break_on_failure` | 失败断点 | `--gtest_break_on_failure` |
| `--gtest_throw_on_failure` | 失败抛异常 | `--gtest_throw_on_failure` |
| `--gtest_catch_exceptions` | 捕获异常 | `--gtest_catch_exceptions=0` |

### 3.4 常用操作

**测试过滤：**

```bash
# 运行 MathTest 套件的所有测试
./tests --gtest_filter=MathTest.*

# 运行特定测试
./tests --gtest_filter=MathTest.Addition

# 排除某些测试
./tests --gtest_filter=-*SlowTest*

# 组合过滤
./tests --gtest_filter=MathTest.*:StringTest.*-*.SlowTest
```

**重复运行与随机顺序：**

```bash
# 重复运行 100 次
./tests --gtest_repeat=100

# 随机顺序运行
./tests --gtest_shuffle --gtest_random_seed=42

# 无限运行直到失败
./tests --gtest_repeat=-1 --gtest_break_on_failure
```

## 4. 进阶特性

### 4.1 高级断言

**基本断言：**

```cpp
TEST(AssertionTest, Basic) {
    // EXPECT_* 继续执行，ASSERT_* 立即停止
    EXPECT_TRUE(condition);
    EXPECT_FALSE(condition);

    // ASSERT_* 失败后终止当前测试
    ASSERT_TRUE(important_condition);
}
```

**比较断言：**

```cpp
TEST(ComparisonTest, AllTypes) {
    EXPECT_EQ(1 + 1, 2);     // 相等
    EXPECT_NE(1 + 1, 3);     // 不等
    EXPECT_LT(1, 2);          // 小于
    EXPECT_LE(2, 2);          // 小于等于
    EXPECT_GT(3, 2);          // 大于
    EXPECT_GE(3, 3);          // 大于等于
}
```

**浮点断言：**

```cpp
#include <cmath>

TEST(FloatTest, Comparison) {
    // 默认允许 4 个 ULP（最小单位）误差
    EXPECT_FLOAT_EQ(0.1f + 0.2f, 0.3f);
    EXPECT_DOUBLE_EQ(0.1 + 0.2, 0.3);

    // 指定绝对误差
    EXPECT_NEAR(std::sqrt(2.0), 1.414, 0.001);
}
```

**字符串断言：**

```cpp
TEST(StringTest, Comparison) {
    const char* s1 = "hello";
    const char* s2 = "hello";
    std::string s3 = "HELLO";

    EXPECT_STREQ(s1, s2);         // 相等
    EXPECT_STRNE(s1, "world");    // 不等
    EXPECT_STRCASEEQ(s1, s3.c_str()); // 忽略大小写
    EXPECT_STRCASENE("abc", "def");
}
```

### 4.2 测试夹具详解

```cpp
// 测试夹具基类
class DatabaseTest : public ::testing::Test {
protected:
    Database* db_;
    static Database* shared_db_;  // 测试套件共享

    // 整个测试套件执行一次
    static void SetUpTestSuite() {
        shared_db_ = new Database("test.db");
        shared_db_->connect();
    }

    static void TearDownTestSuite() {
        shared_db_->disconnect();
        delete shared_db_;
        shared_db_ = nullptr;
    }

    // 每个测试执行一次
    void SetUp() override {
        db_ = new Database(":memory:");
        db_->createTable("users");
    }

    void TearDown() override {
        db_->dropTable("users");
        delete db_;
    }
};

// 静态成员初始化
Database* DatabaseTest::shared_db_ = nullptr;

// 使用测试夹具
TEST_F(DatabaseTest, InsertUser) {
    db_->insert("users", {{"name", "Alice"}, {"age", 30}});
    EXPECT_EQ(db_->count("users"), 1);
}

TEST_F(DatabaseTest, QueryUser) {
    db_->insert("users", {{"name", "Bob"}});
    auto user = db_->query("users", "name = 'Bob'");
    EXPECT_FALSE(user.empty());
}
```

### 4.3 参数化测试

**值参数化测试：**

```cpp
// 测试夹具继承 TestWithParam<T>
class PrimeTest : public ::testing::TestWithParam<int> {
protected:
    bool isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) return false;
        }
        return true;
    }
};

// 使用 TEST_P 宏
TEST_P(PrimeTest, IsPrime) {
    int n = GetParam();
    EXPECT_TRUE(isPrime(n));
}

// 实例化测试
INSTANTIATE_TEST_SUITE_P(
    PrimeNumbers,           // 前缀名称
    PrimeTest,              // 测试夹具
    ::testing::Values(2, 3, 5, 7, 11, 13, 17, 19)  // 参数值
);

INSTANTIATE_TEST_SUITE_P(
    NonPrimeNumbers,
    PrimeTest,
    ::testing::Values(1, 4, 6, 8, 9, 10)
);

// 使用 Range 和 ValuesIn
INSTANTIATE_TEST_SUITE_P(
    RangeTest,
    PrimeTest,
    ::testing::Range(1, 100)  // 1 到 99
);

// 从容器获取参数
std::vector<int> GetTestValues() { return {2, 3, 5, 7}; }
INSTANTIATE_TEST_SUITE_P(
    ContainerTest,
    PrimeTest,
    ::testing::ValuesIn(GetTestValues())
);
```

**类型参数化测试：**

```cpp
template<typename T>
class ContainerTest : public ::testing::Test {
protected:
    using Container = T;
    Container container_;
};

// 定义类型列表
using ContainerTypes = ::testing::Types<
    std::vector<int>,
    std::list<int>,
    std::deque<int>,
    std::set<int>
>;

// 注册类型参数化测试套件
TYPED_TEST_SUITE(ContainerTest, ContainerTypes);

// 类型参数化测试
TYPED_TEST(ContainerTest, Size) {
    this->container_.insert(this->container_.end(), 1);
    EXPECT_EQ(this->container_.size(), 1);
}

TYPED_TEST(ContainerTest, Empty) {
    EXPECT_TRUE(this->container_.empty());
}
```

### 4.4 死亡测试

测试程序异常终止、断言失败、异常抛出等场景：

```cpp
// 测试断言失败导致的程序终止
TEST(DeathTest, AssertDeath) {
    ASSERT_DEATH({
        assert(false && "assertion failed");
    }, "assertion.*failed");
}

// 测试 exit() 调用
TEST(DeathTest, ExitTest) {
    ASSERT_EXIT({
        std::cout << "Exiting..." << std::endl;
        exit(1);
    }, ::testing::ExitedWithCode(1), "Exiting");
}

// 测试 _exit() 调用
TEST(DeathTest, QuickExit) {
    ASSERT_EXIT({
        _exit(0);
    }, ::testing::ExitedWithCode(0), "");
}

// 测试异常（需要编译选项 -fno-exceptions）
TEST(ExceptionTest, ThrowException) {
    ASSERT_THROW({
        throw std::runtime_error("error");
    }, std::runtime_error);

    EXPECT_NO_THROW({
        int x = 1 + 1;
    });

    EXPECT_ANY_THROW({
        throw 42;
    });
}
```

## 5. GoogleMock

### 5.1 Mock 基础

```cpp
#include <gmock/gmock.h>

// 原始接口
class Database {
public:
    virtual ~Database() = default;
    virtual bool connect(const std::string& url) = 0;
    virtual std::string query(const std::string& sql) = 0;
    virtual int insert(const std::string& table,
                       const std::map<std::string, std::string>& data) = 0;
    virtual void close() = 0;
};

// Mock 类
class MockDatabase : public Database {
public:
    MOCK_METHOD(bool, connect, (const std::string& url), (override));
    MOCK_METHOD(std::string, query, (const std::string& sql), (override));
    MOCK_METHOD(int, insert,
                (const std::string& table,
                 const std::map<std::string, std::string>& data),
                (override));
    MOCK_METHOD(void, close, (), (override));
};

// 使用 Mock
TEST(MockTest, Query) {
    MockDatabase mock;

    // 设置期望
    EXPECT_CALL(mock, connect("localhost"))
        .WillOnce(::testing::Return(true));

    EXPECT_CALL(mock, query("SELECT * FROM users"))
        .WillOnce(::testing::Return("[{\"id\": 1}]"));

    // 调用
    EXPECT_TRUE(mock.connect("localhost"));
    EXPECT_EQ(mock.query("SELECT * FROM users"), "[{\"id\": 1}]");
}
```

### 5.2 Mock 匹配器

| 匹配器 | 说明 | 示例 |
|--------|------|------|
| `_` | 任意值 | `EXPECT_CALL(mock, func(_))` |
| `Eq(value)` | 等于值 | `EXPECT_CALL(mock, func(Eq(42)))` |
| `Ne(value)` | 不等于 | `EXPECT_CALL(mock, func(Ne(0)))` |
| `Lt(n)` | 小于 | `EXPECT_CALL(mock, func(Lt(100)))` |
| `Gt(n)` | 大于 | `EXPECT_CALL(mock, func(Gt(0)))` |
| `Le(n)` | 小于等于 | `EXPECT_CALL(mock, func(Le(10)))` |
| `Ge(n)` | 大于等于 | `EXPECT_CALL(mock, func(Ge(1)))` |
| `StrEq(s)` | 字符串相等 | `EXPECT_CALL(mock, func(StrEq("hello")))` |
| `ContainsRegex(re)` | 正则匹配 | `EXPECT_CALL(mock, func(ContainsRegex("err.*")))` |
| `IsNull()` | 空指针 | `EXPECT_CALL(mock, func(IsNull()))` |
| `NotNull()` | 非空指针 | `EXPECT_CALL(mock, func(NotNull()))` |
| `Pointee(value)` | 解引用匹配 | `EXPECT_CALL(mock, func(Pointee(Eq(5))))` |

```cpp
using ::testing::_;
using ::testing::Eq;
using ::testing::Lt;
using ::testing::Gt;
using ::testing::StrEq;
using ::testing::ContainsRegex;

TEST(MatcherTest, Examples) {
    MockDatabase mock;

    // 任意参数
    EXPECT_CALL(mock, connect(_)).WillOnce(::testing::Return(true));

    // 条件匹配
    EXPECT_CALL(mock, query(ContainsRegex("SELECT"))).Times(1);

    // 组合匹配
    EXPECT_CALL(mock, insert(StrEq("users"), _)).WillOnce(::testing::Return(1));
}
```

### 5.3 动作与返回值

```cpp
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::ReturnPointee;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::Throw;

TEST(ActionTest, ReturnValue) {
    MockDatabase mock;

    // 返回固定值
    EXPECT_CALL(mock, connect(_))
        .WillOnce(Return(true))
        .WillOnce(Return(false));

    // 返回引用
    std::string result = "data";
    EXPECT_CALL(mock, query(_))
        .WillRepeatedly(ReturnRef(result));

    // 调用函数
    EXPECT_CALL(mock, insert(_, _))
        .WillOnce(Invoke([](auto table, auto data) {
            return data.size();
        }));
}

TEST(ActionTest, ComplexAction) {
    MockDatabase mock;

    // 组合动作
    EXPECT_CALL(mock, query(_))
        .WillOnce(DoAll(
            SetArgPointee<0>("modified"),  // 修改输出参数
            Return("result")
        ));

    // 抛出异常
    EXPECT_CALL(mock, connect("invalid"))
        .WillOnce(Throw(std::runtime_error("connection failed")));
}
```

### 5.4 调用次数控制

```cpp
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::AtMost;
using ::testing::Between;

TEST(CardinalityTest, Examples) {
    MockDatabase mock;

    // 任意次数
    EXPECT_CALL(mock, close()).Times(AnyNumber());

    // 精确次数
    EXPECT_CALL(mock, connect(_)).Times(2);

    // 范围次数
    EXPECT_CALL(mock, query(_)).Times(Between(1, 3));

    // 最少/最多
    EXPECT_CALL(mock, insert(_, _)).Times(AtLeast(1));
    EXPECT_CALL(mock, insert(_, _)).Times(AtMost(5));
}
```

## 6. CMake 与 CI/CD 集成

### 6.1 CMake 集成

**完整 CMakeLists.txt 示例：**

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProjectTests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用测试
enable_testing()

# 方式一：FetchContent
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.14.0
)
FetchContent_MakeAvailable(googletest)

# 方式二：find_package（vcpkg 或系统安装）
# find_package(GTest REQUIRED)

# 测试源文件
add_executable(tests
    test_main.cpp
    test_math.cpp
    test_database.cpp
)

target_link_libraries(tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
)

# 方式一：使用 CTest
add_test(NAME MyTests COMMAND tests)

# 方式二：使用 gtest_discover_tests（推荐）
include(GoogleTest)
gtest_discover_tests(tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    PROPERTIES
        LABELS "unit"
        TIMEOUT 300
)

# 测试属性
set_tests_properties(MyTests PROPERTIES
    ENVIRONMENT "TEST_DATA_DIR=${CMAKE_SOURCE_DIR}/data"
)
```

### 6.2 CTest 运行

```bash
# 配置项目
cmake -B build -S .

# 构建测试
cmake --build build

# 运行所有测试
cd build && ctest --output-on-failure

# 运行特定标签的测试
ctest -L unit

# 并行运行
ctest -j 4

# 详细输出
ctest -V

# 生成 XML 报告
ctest -T Test --no-compress-output
```

### 6.3 GitHub Actions 配置

```yaml
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
    - uses: actions/checkout@v4

    - name: Install vcpkg
      uses: lukka/run-vcpkg@v11
      with:
        vcpkgGitCommitId: latest

    - name: Configure
      run: |
        cmake -B build \
          -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
          -DCMAKE_BUILD_TYPE=${{ matrix.build_type }}

    - name: Build
      run: cmake --build build --config ${{ matrix.build_type }}

    - name: Test
      working-directory: build
      run: ctest -C ${{ matrix.build_type }} --output-on-failure

    - name: Upload Test Results
      if: always()
      uses: actions/upload-artifact@v4
      with:
        name: test-results-${{ matrix.os }}-${{ matrix.build_type }}
        path: build/Testing/Temporary/LastTests.log
```

### 6.4 测试覆盖率

```bash
# 编译时启用覆盖率
cmake -B build -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="--coverage -fprofile-arcs -ftest-coverage"

cmake --build build
cd build && ctest

# 生成覆盖率报告
gcov *.cpp
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report

# 使用 lcov 过滤
lcov --remove coverage.info '/usr/*' --output-file coverage.info
```

## 7. 问题排查

### 7.1 常见问题

**问题 1：链接错误 - 未定义的引用**

```
undefined reference to `testing::Test::Test()'
```

解决方案：

```cmake
# 确保链接正确的库
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)

# 或者使用 pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTEST REQUIRED gtest)
target_link_libraries(tests PRIVATE ${GTEST_LIBRARIES})
```

**问题 2：测试夹具 SetUp 未调用**

```cpp
// 错误：忘记继承 ::testing::Test
class MyTest {  // 错误！
protected:
    void SetUp() { /* ... */ }
};

// 正确
class MyTest : public ::testing::Test {  // 正确
protected:
    void SetUp() override { /* ... */ }
};
```

**问题 3：死亡测试在 Release 模式下失败**

```cpp
// Release 模式下 assert 可能被优化掉
TEST(DeathTest, Assert) {
    // 在 Release 模式下可能失败
    ASSERT_DEATH(assert(false), "");

    // 解决方案：使用 gtest 的断言
    EXPECT_DEATH(::testing::internal::posix::Abort(), "");
}
```

**问题 4：线程安全问题**

```cpp
// 全局共享资源需要加锁
class ThreadSafeTest : public ::testing::Test {
protected:
    static std::mutex mutex_;
    static Database* db_;

    void SetUp() override {
        std::lock_guard<std::mutex> lock(mutex_);
        // 初始化操作
    }
};
```

### 7.2 调试技巧

**输出调试信息：**

```cpp
TEST(DebugTest, Output) {
    // 输出额外信息
    EXPECT_EQ(1, 1) << "Additional debug info";

    // 使用 RecordProperty 记录属性
    RecordProperty("test_case", "debug_example");
    RecordProperty("iterations", 100);

    // 使用 SCOPED_TRACE 追踪调用栈
    {
        SCOPED_TRACE("Inner block");
        EXPECT_EQ(1, 1);  // 失败时会显示 "Inner block"
    }
}
```

**使用 GDB 调试：**

```bash
# 编译调试版本
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# GDB 调试
gdb ./build/tests
(gdb) break MathTest::Addition
(gdb) run --gtest_filter=MathTest.Addition

# 失败时自动断点
./tests --gtest_break_on_failure
```

**测试隔离问题排查：**

```cpp
// 检测测试顺序依赖
TEST(OrderTest, First) {
    EXPECT_EQ(1, 1);
}

TEST(OrderTest, Second) {
    // 随机顺序运行检测依赖
    EXPECT_EQ(1, 1);
}

// 运行命令
// ./tests --gtest_shuffle --gtest_repeat=100
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| GitHub 仓库 | https://github.com/google/googletest |
| 官方文档 | https://google.github.io/googletest/ |
| GoogleMock 文档 | https://google.github.io/googletest/gmock_for_dummies.html |
| 高级指南 | https://google.github.io/googletest/advanced.html |
| Primer | https://google.github.io/googletest/primer.html |

### 8.2 学习路径

| 阶段 | 内容 | 时间 |
|------|------|------|
| 入门 | 基本断言、测试组织、夹具使用 | 1-2 天 |
| 进阶 | 参数化测试、死亡测试、Mock 基础 | 3-5 天 |
| 高级 | 高级 Mock、测试覆盖率、CI/CD 集成 | 1-2 周 |
| 专家 | 性能测试、模糊测试、测试框架扩展 | 持续学习 |

### 8.3 最佳实践

1. **测试命名规范**：`TestSuiteName.TestCaseName`，使用描述性名称
2. **每个测试单一职责**：一个测试只验证一个行为
3. **使用测试夹具**：避免重复的 SetUp/TearDown 代码
4. **EXPECT vs ASSERT**：需要后续断言用 EXPECT，立即停止用 ASSERT
5. **死亡测试隔离**：使用 `//` 分隔死亡测试
6. **Mock 适度使用**：避免过度 Mock 导致测试脆弱
7. **持续集成**：每次提交自动运行测试
8. **覆盖率目标**：核心代码覆盖率 > 80%