# GoogleTest - Google C++ 测试框架

## 1. 概述

GoogleTest（简称 gtest）是 Google 开发的 C++ 测试框架，提供了丰富的断言机制、灵活的测试组织和强大的 mock 功能，是目前最流行的 C++ 单元测试框架之一。

### 1.1 核心特性

| 特性 | 说明 |
|------|------|
| 丰富断言 | 提供布尔、比较、浮点、字符串等多种断言 |
| 测试夹具 | 支持 SetUp/TearDown 生命周期管理 |
| 参数化测试 | 值参数化、类型参数化测试支持 |
| 死亡测试 | 测试程序崩溃、退出等异常情况 |
| Mock 支持 | 内置 GoogleMock 框架 |
| 测试发现 | 自动注册测试用例，无需手动管理 |
| 跨平台 | 支持 Linux、macOS、Windows |

### 1.2 适用场景

- 企业级 C++ 项目单元测试
- 需要复杂 mock 的测试场景
- 参数化测试需求
- CI/CD 持续集成测试

## 2. 安装配置

### 2.1 包管理器安装

```bash
# vcpkg
vcpkg install gtest

# Conan
conan install gtest/1.14.0

# Homebrew (macOS)
brew install googletest

# apt (Ubuntu/Debian)
sudo apt install libgtest-dev
```

### 2.2 源码编译安装

```bash
git clone https://github.com/google/googletest
cd googletest
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
sudo cmake --install build
```

### 2.3 CMake FetchContent 集成

```cmake
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.14.0
)
# 对于 Windows: 阻止 overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 链接目标
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main)
```

### 2.4 项目 CMake 配置完整示例

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 方式一：FetchContent
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.14.0
)
FetchContent_MakeAvailable(googletest)

# 方式二：find_package（需先安装）
# find_package(GTest REQUIRED)

# 启用测试
enable_testing()

# 创建测试可执行文件
add_executable(my_tests
    test_calculator.cpp
    test_parser.cpp
)

target_link_libraries(my_tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main  # 提供 main 函数
        my_library         # 被测试的库
)

# 注册测试
add_test(NAME MyTests COMMAND my_tests)

# 添加 CTest 支持
include(GoogleTest)
gtest_discover_tests(my_tests)
```

## 3. 基本用法

### 3.1 简单测试用例

```cpp
#include <gtest/gtest.h>

// 最简单的测试
TEST(MathTest, Addition) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_NE(1 + 1, 3);
}

TEST(MathTest, Multiplication) {
    EXPECT_EQ(2 * 3, 6);
    EXPECT_DOUBLE_EQ(2.0 * 3.5, 7.0);
}

// main 函数（如果链接 gtest_main 则不需要）
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

### 3.2 测试夹具（Test Fixture）

测试夹具用于多个测试共享公共设置代码：

```cpp
class VectorTest : public ::testing::Test {
protected:
    std::vector<int> vec;

    // 每个测试前执行
    void SetUp() override {
        vec = {1, 2, 3};
    }

    // 每个测试后执行
    void TearDown() override {
        vec.clear();
    }
};

// 使用 TEST_F 而非 TEST
TEST_F(VectorTest, InitialSize) {
    EXPECT_EQ(vec.size(), 3);
    EXPECT_FALSE(vec.empty());
}

TEST_F(VectorTest, PushBack) {
    vec.push_back(4);
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec.back(), 4);
}

TEST_F(VectorTest, Clear) {
    vec.clear();
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
}
```

### 3.3 测试套件生命周期

```cpp
class DatabaseTest : public ::testing::Test {
protected:
    static Database* shared_db;  // 共享资源
    Database* db;                 // 每个测试独立的资源

    // 整个测试套件开始前执行一次
    static void SetUpTestSuite() {
        shared_db = new Database("shared_connection");
        shared_db->connect();
    }

    // 整个测试套件结束后执行一次
    static void TearDownTestSuite() {
        shared_db->disconnect();
        delete shared_db;
        shared_db = nullptr;
    }

    // 每个测试开始前执行
    void SetUp() override {
        db = new Database("test_db");
        db->create_tables();
    }

    // 每个测试结束后执行
    void TearDown() override {
        db->drop_tables();
        delete db;
    }
};

Database* DatabaseTest::shared_db = nullptr;

TEST_F(DatabaseTest, Insert) {
    db->insert("key", "value");
    EXPECT_EQ(db->get("key"), "value");
}

TEST_F(DatabaseTest, Delete) {
    db->insert("key", "value");
    db->remove("key");
    EXPECT_FALSE(db->exists("key"));
}
```

## 4. 断言详解

### 4.1 EXPECT 与 ASSERT 区别

| 类型 | 失败后行为 | 使用场景 |
|------|------------|----------|
| `EXPECT_*` | 记录失败，继续执行 | 非关键断言，收集所有失败 |
| `ASSERT_*` | 立即停止当前测试 | 关键断言，后续代码依赖其成功 |

```cpp
TEST(AssertExample, Difference) {
    int* ptr = new int(42);

    // 使用 ASSERT：如果为空，后续访问会崩溃
    ASSERT_NE(ptr, nullptr);

    // 使用 EXPECT：即使失败也继续检查
    EXPECT_EQ(*ptr, 42);
    EXPECT_GT(*ptr, 0);

    delete ptr;
}
```

### 4.2 基本断言

| 断言 | 说明 | 示例 |
|------|------|------|
| `EXPECT_TRUE(cond)` | 期望条件为真 | `EXPECT_TRUE(is_valid())` |
| `EXPECT_FALSE(cond)` | 期望条件为假 | `EXPECT_FALSE(is_empty())` |
| `ASSERT_TRUE(cond)` | 断言条件为真 | `ASSERT_TRUE(ptr != nullptr)` |
| `ASSERT_FALSE(cond)` | 断言条件为假 | `ASSERT_FALSE(has_error())` |

### 4.3 比较断言

| 断言 | 说明 | 等价表达式 |
|------|------|-----------|
| `EXPECT_EQ(a, b)` | 期望相等 | `a == b` |
| `EXPECT_NE(a, b)` | 期望不等 | `a != b` |
| `EXPECT_LT(a, b)` | 期望小于 | `a < b` |
| `EXPECT_LE(a, b)` | 期望小于等于 | `a <= b` |
| `EXPECT_GT(a, b)` | 期望大于 | `a > b` |
| `EXPECT_GE(a, b)` | 期望大于等于 | `a >= b` |

```cpp
TEST(ComparisonTest, BasicComparisons) {
    int a = 10, b = 20;

    EXPECT_LT(a, b);
    EXPECT_LE(a, b);
    EXPECT_GT(b, a);
    EXPECT_GE(b, a);
    EXPECT_EQ(a + 10, b);
    EXPECT_NE(a, b);
}
```

### 4.4 浮点比较

```cpp
#include <cmath>

TEST(FloatTest, FloatComparisons) {
    float a = 0.1f + 0.2f;
    float b = 0.3f;

    // 普通比较可能失败（浮点精度问题）
    // EXPECT_EQ(a, b);  // 不推荐

    // 推荐使用浮点专用断言
    EXPECT_FLOAT_EQ(a, b);      // 4 ULPs 误差容忍
    EXPECT_DOUBLE_EQ(0.1 + 0.2, 0.3);

    // 自定义误差范围
    EXPECT_NEAR(a, b, 1e-5);
    EXPECT_NEAR(std::sqrt(2.0) * std::sqrt(2.0), 2.0, 1e-10);
}
```

| 断言 | 说明 | 精度 |
|------|------|------|
| `EXPECT_FLOAT_EQ(a, b)` | 浮点近似相等 | 4 ULPs |
| `EXPECT_DOUBLE_EQ(a, b)` | 双精度近似相等 | 4 ULPs |
| `EXPECT_NEAR(a, b, abs_err)` | 差值在误差范围内 | 自定义 |

### 4.5 字符串比较

```cpp
TEST(StringTest, StringComparisons) {
    const char* s1 = "Hello";
    const char* s2 = "Hello";
    const char* s3 = "HELLO";

    EXPECT_STREQ(s1, s2);        // C 字符串相等
    EXPECT_STRNE(s1, "World");  // C 字符串不等
    EXPECT_STRCASEEQ(s1, s3);   // 忽略大小写相等
    EXPECT_STRCASENE(s1, "World");  // 忽略大小写不等

    // C++ std::string 使用 EXPECT_EQ
    std::string str1 = "Hello";
    std::string str2 = "Hello";
    EXPECT_EQ(str1, str2);
    EXPECT_NE(str1, std::string("World"));
}
```

| 断言 | 说明 |
|------|------|
| `EXPECT_STREQ(a, b)` | C 字符串相等 |
| `EXPECT_STRNE(a, b)` | C 字符串不等 |
| `EXPECT_STRCASEEQ(a, b)` | 忽略大小写相等 |
| `EXPECT_STRCASENE(a, b)` | 忽略大小写不等 |

### 4.6 异常断言

```cpp
// 自定义异常类
class MyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "My exception";
    }
};

// 可能抛出异常的函数
void throw_error() {
    throw MyException();
}

void throw_value() {
    throw 42;
}

TEST(ExceptionTest, ExceptionAssertions) {
    // 期望抛出特定类型异常
    EXPECT_THROW(throw_error(), MyException);
    EXPECT_THROW(throw_value(), int);

    // 断言版本（失败停止）
    ASSERT_THROW(throw_error(), MyException);

    // 期望抛出任意异常
    EXPECT_ANY_THROW(throw_error());

    // 期望不抛出异常
    EXPECT_NO_THROW(std::cout << "safe");
}
```

| 断言 | 说明 |
|------|------|
| `EXPECT_THROW(expr, type)` | 期望抛出指定类型异常 |
| `EXPECT_NO_THROW(expr)` | 期望不抛出异常 |
| `EXPECT_ANY_THROW(expr)` | 期望抛出任意异常 |

## 5. 测试组织

### 5.1 测试命名规范

```cpp
// 推荐命名规范：功能_场景_预期结果
// 格式：<测试套件名>.<测试名>

// 好的命名示例
TEST(Calculator, Add_TwoPositiveNumbers_ReturnsSum) {}
TEST(Calculator, Divide_ByZero_ThrowsException) {}
TEST(UserService, Login_ValidCredentials_ReturnsTrue) {}
TEST(UserService, Login_InvalidCredentials_ReturnsFalse) {}

// 避免
TEST(Test1, test1) {}  // 不清晰
TEST(Test, Add) {}     // 不够具体
```

### 5.2 值参数化测试

```cpp
// 定义参数化测试夹具
class PrimeTest : public ::testing::TestWithParam<int> {};

// 使用参数
TEST_P(PrimeTest, IsPrime) {
    int n = GetParam();
    EXPECT_TRUE(is_prime(n));
}

// 实例化测试套件
INSTANTIATE_TEST_SUITE_P(
    PrimeValues,           // 实例化名称前缀
    PrimeTest,             // 测试套件名
    ::testing::Values(2, 3, 5, 7, 11, 13, 17, 19)  // 参数值
);

// 测试命名：PrimeValues/PrimeTest.IsPrime/0, /1, ...

// 使用范围
INSTANTIATE_TEST_SUITE_P(
    RangePrimes,
    PrimeTest,
    ::testing::Range(1, 100)  // 1 到 99
);

// 使用 ValuesIn 从容器获取
std::vector<int> test_data = {10, 20, 30, 40, 50};
INSTANTIATE_TEST_SUITE_P(
    VectorPrimes,
    PrimeTest,
    ::testing::ValuesIn(test_data)
);

// 结合测试命名
INSTANTIATE_TEST_SUITE_P(
    NamedPrimes,
    PrimeTest,
    ::testing::Values(2, 3, 5, 7),
    ::testing::PrintToStringParamName()  // 使用值作为测试名
);
```

### 5.3 类型参数化测试

```cpp
// 定义类型参数化测试夹具
template<typename T>
class ContainerTest : public ::testing::Test {
protected:
    std::unique_ptr<T> container;

    void SetUp() override {
        container = std::make_unique<T>();
    }
};

// 指定测试类型
using ContainerTypes = ::testing::Types<
    std::vector<int>,
    std::list<int>,
    std::deque<int>
>;

TYPED_TEST_SUITE(ContainerTest, ContainerTypes);

// 类型参数化测试
TYPED_TEST(ContainerTest, InitialSize) {
    EXPECT_EQ(this->container->size(), 0);
}

TYPED_TEST(ContainerTest, PushBackIncreasesSize) {
    this->container->push_back(1);
    EXPECT_EQ(this->container->size(), 1);
}
```

### 5.4 测试分组与过滤

```bash
# 运行特定测试套件
./test_program --gtest_filter=MathTest.*

# 运行特定测试
./test_program --gtest_filter=MathTest.Addition

# 使用通配符
./test_program --gtest_filter=*Prime*

# 排除测试
./test_program --gtest_filter=-SlowTest.*

# 组合过滤
./test_program --gtest_filter=MathTest.*:StringTest.*-SlowTest.*
```

## 6. 高级功能

### 6.1 死亡测试（Death Test）

死亡测试用于验证程序在特定条件下是否按预期终止：

```cpp
// 测试程序崩溃
void cause_crash() {
    assert(false && "intentional crash");
}

TEST(DeathTest, Crash) {
    ASSERT_DEATH(cause_crash(), "intentional crash");
}

// 测试退出代码
TEST(DeathTest, Exit) {
    ASSERT_EXIT(exit(1), ::testing::ExitedWithCode(1), "");
}

// 测试信号（POSIX）
TEST(DeathTest, Signal) {
    ASSERT_DEATH(raise(SIGSEGV), ".*");
}

// 使用正则表达式匹配输出
TEST(DeathTest, MessageMatch) {
    ASSERT_DEATH(
        []() { std::cerr << "Error: fatal\n"; abort(); }(),
        "Error: fatal"
    );
}
```

| 宏 | 说明 |
|-----|------|
| `ASSERT_DEATH(stmt, pattern)` | 进程异常终止，输出匹配模式 |
| `EXPECT_DEATH(stmt, pattern)` | 同上，但失败继续 |
| `ASSERT_EXIT(stmt, predicate, pattern)` | 进程退出，predicate 检查退出状态 |
| `ASSERT_THROW(stmt, exception_type)` | 抛出指定异常 |

### 6.2 测试事件监听器

```cpp
// 自定义测试事件监听器
class CustomListener : public ::testing::TestEventListener {
public:
    void OnTestProgramStart(const ::testing::UnitTest& unit_test) override {
        std::cout << "=== Test Program Start ===" << std::endl;
    }

    void OnTestStart(const ::testing::TestInfo& test_info) override {
        std::cout << "Running: " << test_info.test_suite_name()
                  << "." << test_info.name() << std::endl;
    }

    void OnTestEnd(const ::testing::TestInfo& test_info) override {
        if (test_info.result()->Passed()) {
            std::cout << "[ PASSED ] ";
        } else {
            std::cout << "[ FAILED ] ";
        }
        std::cout << test_info.test_suite_name() << "."
                  << test_info.name() << std::endl;
    }

    void OnTestProgramEnd(const ::testing::UnitTest& unit_test) override {
        std::cout << "=== Test Program End ===" << std::endl;
        std::cout << "Total tests: " << unit_test.total_test_count() << std::endl;
        std::cout << "Passed: " << unit_test.successful_test_count() << std::endl;
        std::cout << "Failed: " << unit_test.failed_test_count() << std::endl;
    }
};

// 注册监听器
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // 移除默认监听器（可选）
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());

    // 添加自定义监听器
    listeners.Append(new CustomListener);

    return RUN_ALL_TESTS();
}
```

### 6.3 测试跳过与禁用

```cpp
// 跳过测试
TEST(SkipTest, SkipExample) {
    GTEST_SKIP() << "Skipping this test for now";
    // 后续代码不会执行
}

// 条件跳过
TEST(SkipTest, ConditionalSkip) {
    if (!feature_enabled()) {
        GTEST_SKIP() << "Feature not enabled";
    }
    // 正常测试逻辑
}

// 禁用测试（测试名添加 DISABLED_ 前缀）
TEST(DISABLED_DisabledTest, Example) {
    // 此测试默认不运行
    EXPECT_EQ(1, 1);
}

// 禁用整个测试套件
class DISABLED_OldFeatureTest : public ::testing::Test {};

TEST_F(DISABLED_OldFeatureTest, Example) {
    // 整个套件被禁用
}

// 运行被禁用的测试
// ./test_program --gtest_also_run_disabled_tests
```

## 7. GoogleMock 集成

### 7.1 Mock 类基础

```cpp
#include <gmock/gmock.h>

// 原始接口
class Database {
public:
    virtual ~Database() = default;
    virtual std::string get(const std::string& key) = 0;
    virtual void insert(const std::string& key, const std::string& value) = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual int count() const = 0;
};

// Mock 类
class MockDatabase : public Database {
public:
    MOCK_METHOD(std::string, get, (const std::string& key), (override));
    MOCK_METHOD(void, insert, (const std::string& key, const std::string& value), (override));
    MOCK_METHOD(bool, remove, (const std::string& key), (override));
    MOCK_METHOD(int, count, (const), (override));
};

// 使用 Mock
TEST(MockTest, BasicUsage) {
    MockDatabase mock;

    // 设置期望
    EXPECT_CALL(mock, get("test_key"))
        .WillOnce(::testing::Return("test_value"));

    // 调用
    std::string result = mock.get("test_key");
    EXPECT_EQ(result, "test_value");
}
```

### 7.2 Mock 方法定义语法

```cpp
class MockExample : public Example {
public:
    // 基本语法：MOCK_METHOD(返回类型, 方法名, (参数列表), (修饰符))

    // 无参数方法
    MOCK_METHOD(int, getSize, (), (const, override));

    // 多参数方法
    MOCK_METHOD(void, setData, (int id, const std::string& name), (override));

    // 返回引用
    MOCK_METHOD(const std::string&, getName, (), (const, override));

    // 移动语义
    MOCK_METHOD(void, setValue, (std::string&& value), (override));

    // 模板方法（需要额外处理）
    // 使用 MOCK_METHOD_CALL 或特化
};
```

### 7.3 期望设置

```cpp
using ::testing::_;
using ::testing::Return;
using ::testing::Throw;

TEST(MockTest, Expectations) {
    MockDatabase mock;

    // 基本期望
    EXPECT_CALL(mock, get("key"))
        .WillOnce(Return("value"));

    // 多次调用
    EXPECT_CALL(mock, count())
        .Times(3)
        .WillOnce(Return(1))
        .WillOnce(Return(2))
        .WillRepeatedly(Return(3));

    // 任意参数
    EXPECT_CALL(mock, get(_))
        .WillRepeatedly(Return("default"));

    // 抛出异常
    EXPECT_CALL(mock, remove("invalid"))
        .WillOnce(Throw(std::runtime_error("not found")));

    // 链式调用
    EXPECT_CALL(mock, insert(_, _))
        .Times(::testing::AtLeast(1));
}
```

### 7.4 匹配器

```cpp
#include <gmock/gmock.h>
using ::testing::_;           // 任意值
using ::testing::Eq;          // 等于
using ::testing::Lt;          // 小于
using ::testing::Gt;          // 大于
using ::testing::Le;          // 小于等于
using ::testing::Ge;          // 大于等于
using ::testing::Ne;          // 不等于
using ::testing::Contains;    // 包含
using ::testing::StartsWith;  // 以...开头
using ::testing::EndsWith;    // 以...结尾
using ::testing::IsEmpty;     // 为空
using ::testing::NotNull;     // 非空指针
using ::testing::Pointee;     // 解引用比较

TEST(MatcherTest, BasicMatchers) {
    MockDatabase mock;

    // 值匹配器
    EXPECT_CALL(mock, get(Eq("key")));      // 等于 "key"
    EXPECT_CALL(mock, get(Ne("invalid")));  // 不等于 "invalid"

    // 范围匹配器
    EXPECT_CALL(mock, count())
        .WillOnce(Return(5));
    EXPECT_CALL(mock, remove(Gt("m")));     // 大于 "m"

    // 字符串匹配器
    EXPECT_CALL(mock, get(StartsWith("pre")));    // 以 "pre" 开头
    EXPECT_CALL(mock, get(EndsWith("suffix")));   // 以 "suffix" 结尾
    EXPECT_CALL(mock, get(Contains("middle")));  // 包含 "middle"

    // 组合匹配器
    using ::testing::AllOf;
    using ::testing::AnyOf;
    using ::testing::Not;

    EXPECT_CALL(mock, get(AllOf(StartsWith("a"), EndsWith("z"))));
    EXPECT_CALL(mock, get(AnyOf(Eq("first"), Eq("second"))));
    EXPECT_CALL(mock, get(Contains("sub")));
}
```

| 匹配器 | 说明 | 示例 |
|--------|------|------|
| `_` | 任意值 | `EXPECT_CALL(obj, foo(_))` |
| `Eq(v)` | 等于 | `EXPECT_CALL(obj, foo(Eq(5)))` |
| `Lt(v)` | 小于 | `EXPECT_CALL(obj, foo(Lt(10)))` |
| `Gt(v)` | 大于 | `EXPECT_CALL(obj, foo(Gt(0)))` |
| `StartsWith(s)` | 以字符串开头 | `EXPECT_CALL(obj, foo(StartsWith("pre")))` |
| `Contains(s)` | 包含字符串 | `EXPECT_CALL(obj, foo(Contains("mid")))` |

### 7.5 动作

```cpp
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::ReturnPointee;
using ::testing::Throw;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;

TEST(ActionTest, BasicActions) {
    MockDatabase mock;

    // 返回值
    EXPECT_CALL(mock, count())
        .WillOnce(Return(10));

    // 返回引用
    std::string value = "cached";
    EXPECT_CALL(mock, get(_))
        .WillOnce(ReturnRef(value));

    // 抛出异常
    EXPECT_CALL(mock, remove("invalid"))
        .WillOnce(Throw(std::runtime_error("not found")));

    // 调用函数
    EXPECT_CALL(mock, get(_))
        .WillOnce(Invoke([](const std::string& key) {
            return "computed_" + key;
        }));

    // 设置输出参数
    EXPECT_CALL(mock, get_with_result(_, _))
        .WillOnce(DoAll(
            SetArgPointee<1>("output_value"),
            Return(true)
        ));
}
```

## 8. 运行与报告

### 8.1 命令行选项

```bash
# 运行所有测试
./test_program

# 运行特定测试
./test_program --gtest_filter=TestSuiteName.TestName

# 重复运行
./test_program --gtest_repeat=10

# 随机顺序运行
./test_program --gtest_shuffle
./test_program --gtest_shuffle --gtest_random_seed=12345

# 并行运行（需要支持）
./test_program --gtest_jobs=4

# 列出所有测试
./test_program --gtest_list_tests

# 输出控制
./test_program --gtest_print_time=0      # 不显示时间
./test_program --gtest_output=xml:report.xml   # XML 报告
./test_program --gtest_output=json:report.json # JSON 报告

# 运行被禁用的测试
./test_program --gtest_also_run_disabled_tests

# 设置死亡测试风格
./test_program --gtest_death_test_style=threadsafe
./test_program --gtest_death_test_style=fast

# 暂停等待调试器
./test_program --gtest_break_on_failure
```

### 8.2 测试报告格式

**XML 报告格式：**

```xml
<?xml version="1.0" encoding="UTF-8"?>
<testsuites tests="10" failures="1" disabled="2" errors="0" time="0.5">
  <testsuite name="MathTest" tests="5" failures="0" time="0.2">
    <testcase name="Addition" status="run" time="0.01"/>
    <testcase name="Subtraction" status="run" time="0.01"/>
  </testsuite>
  <testsuite name="StringTest" tests="5" failures="1" time="0.3">
    <testcase name="Concatenation" status="run" time="0.01">
      <failure message="Expected: 'hello world'&#10;Actual: 'hello'" type=""/>
    </testcase>
  </testsuite>
</testsuites>
```

### 8.3 CTest 集成

```cmake
# CMakeLists.txt
enable_testing()
include(GoogleTest)

# 自动发现并注册所有测试
gtest_discover_tests(my_tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test
    PROPERTIES
        LABELS "unit"
        TIMEOUT 30
)

# 运行测试
# ctest --output-on-failure
# ctest -L unit            # 运行指定标签
# ctest -R MathTest        # 运行匹配名称的测试
# ctest --parallel 4       # 并行运行
# ctest -T Test            # 详细输出
```

### 8.4 CI/CD 集成示例

```yaml
# GitHub Actions
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
        run: cmake --build build --parallel

      - name: Run tests
        run: |
          cd build
          ctest --output-on-failure --timeout 300
          # 或直接运行测试可执行文件
          # ./my_tests --gtest_output=xml:test_report.xml

      - name: Upload test results
        uses: actions/upload-artifact@v3
        if: always()
        with:
          name: test-results
          path: build/test_report.xml
```

## 9. 最佳实践

### 9.1 测试组织建议

```cpp
// 按功能模块组织测试套件
// 文件：test/calculator_test.cpp

class CalculatorTest : public ::testing::Test {
protected:
    Calculator calc;
};

// 按场景分组测试
class CalculatorAddTest : public CalculatorTest {};
class CalculatorDivTest : public CalculatorTest {};

TEST_F(CalculatorAddTest, TwoPositiveNumbers) {
    EXPECT_EQ(calc.add(2, 3), 5);
}

TEST_F(CalculatorDivTest, NonZeroDivisor) {
    EXPECT_EQ(calc.divide(10, 2), 5);
}

TEST_F(CalculatorDivTest, ZeroDivisor) {
    EXPECT_THROW(calc.divide(10, 0), std::runtime_error);
}
```

### 9.2 断言选择指南

| 场景 | 推荐 |
|------|------|
| 后续测试依赖此断言 | `ASSERT_*` |
| 收集所有失败信息 | `EXPECT_*` |
| 指针非空检查 | `ASSERT_NE(ptr, nullptr)` |
| 浮点数比较 | `EXPECT_FLOAT_EQ` / `EXPECT_NEAR` |
| 字符串比较 | `EXPECT_STREQ` (C) / `EXPECT_EQ` (std::string) |
| 异常测试 | `EXPECT_THROW` |
| 程序终止测试 | `ASSERT_DEATH` |

### 9.3 常见陷阱

```cpp
// 错误：在 ASSERT 后继续使用资源
TEST(WrongTest, UseAfterAssert) {
    int* ptr = new int(42);
    ASSERT_NE(ptr, nullptr);  // 如果失败，后续不会执行
    // 但如果成功，delete 会执行
    delete ptr;
}

// 正确：使用智能指针或夹具
class PtrTest : public ::testing::Test {
protected:
    std::unique_ptr<int> ptr;

    void SetUp() override {
        ptr = std::make_unique<int>(42);
    }
};

TEST_F(PtrTest, UseSmartPtr) {
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 42);
}

// 错误：EXPECT 在 ASSERT 之前依赖的数据
TEST(WrongTest, DependencyOrder) {
    std::vector<int> vec;
    vec.push_back(1);
    EXPECT_EQ(vec.size(), 1);  // 如果失败，继续
    EXPECT_EQ(vec[0], 1);      // 这仍然有意义
}

TEST(WrongTest, AssertDependency) {
    int* ptr = nullptr;
    ASSERT_NE(ptr, nullptr);  // 如果失败，停止
    EXPECT_EQ(*ptr, 42);       // 这不会执行，安全
}
```

## 10. 框架对比

| 特性 | GoogleTest | Catch2 | doctest |
|------|------------|--------|---------|
| 编译速度 | 中 | 快 | 最快 |
| 断言丰富度 | 高 | 中 | 中 |
| Mock 支持 | GMock 内置 | 需第三方 | 需第三方 |
| 参数化测试 | 支持 | 支持 | 不支持 |
| 类型参数化 | 支持 | 支持 | 不支持 |
| 头文件依赖 | 多头文件 | 单头 | 单头 |
| 学习曲线 | 中等 | 简单 | 简单 |
| 社区活跃度 | 高 | 高 | 中 |
| 适用场景 | 企业级项目 | 中小项目 | 快速原型 |

**选择建议：**

- 大型项目、需要 Mock → GoogleTest
- 追求编译速度 → doctest
- 现代风格、BDD 测试 → Catch2
- 已有 Google 技术栈 → GoogleTest

## 11. 参考资源

- [GoogleTest 官方文档](https://google.github.io/googletest/)
- [GoogleMock 官方文档](https://google.github.io/googletest/gmock_for_dummies.html)
- [GitHub 仓库](https://github.com/google/googletest)
- [CMake 集成指南](https://google.github.io/googletest/quickstart-cmake.html)