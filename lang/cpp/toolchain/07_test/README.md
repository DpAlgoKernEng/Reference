# C++ 测试框架

本目录收录 C++ 测试框架文档。

## 测试框架概览

| 框架 | 说明 | 特点 |
|------|------|------|
| [GoogleTest](gtest.md) | Google 测试框架 | 功能全面，业界标准 |
| [Catch2](catch2.md) | 现代测试框架 | 单头文件，易集成 |
| [doctest](doctest.md) | 最快测试框架 | 编译速度快，使用简单 |

## 框架对比

| 特性 | GoogleTest | Catch2 | doctest |
|------|------------|--------|---------|
| C++支持 | ❌ (C++) | ✅ | ✅ |
| 头文件 | 多文件 | 单头文件 | 单头文件 |
| 编译速度 | 慢 | 中等 | 最快 |
| 断言宏 | 丰富 | 丰富 | 丰富 |
| Mock 支持 | ✅ (gMock) | 第三方 | ❌ |
| 测试发现 | 自动 | 自动 | 自动 |
| 学习曲线 | 中等 | 简单 | 简单 |

## C++测试框架推荐

由于 GoogleTest 是 C++ 框架，纯 C 项目推荐：

| 框架 | 说明 |
|------|------|
| **Unity** | 嵌入式项目首选，轻量级 |
| **Cmocka** | 支持 mock，功能全面 |
| **Check** | 自动 fork 测试进程 |
| **Criterion** | 现代化，自动测试发现 |

## 基本测试结构

```c
// test_example.c
#include <assert.h>

void test_addition(void) {
    assert(1 + 1 == 2);
    assert(2 + 2 == 4);
}

int main(void) {
    test_addition();
    return 0;
}
```

### Catch2 示例
```cpp
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("addition works", "[math]") {
    REQUIRE(1 + 1 == 2);
    REQUIRE(2 + 2 == 4);
}
```

### doctest 示例
```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("addition works") {
    CHECK(1 + 1 == 2);
    CHECK(2 + 2 == 4);
}
```

## 测试类型

| 测试类型 | 说明 |
|----------|------|
| 单元测试 | 测试单个函数/模块 |
| 集成测试 | 测试模块间交互 |
| 回归测试 | 确保修改未破坏功能 |
| 性能测试 | 验证性能指标 |
| 边界测试 | 测试边界条件 |

## 相关文档

- [构建系统](../00_build/) - CMake 测试集成
- [动态分析](../06_dynamic_analysis/) - 运行时检测