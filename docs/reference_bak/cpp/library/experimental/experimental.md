# C++实验性特性（Experimental C++ Features）

来源：cppreference.com

## 概述

C++标准委员会发布实验性的C++语言和库扩展，用于未来标准化。这些特性在成为正式标准之前，以技术报告（TR）或技术规范（TS）的形式发布，供开发者试用和反馈。

**注意**：直到2012年，这些出版物使用****TR****（技术报告）格式。自2012年起，ISO程序改为使用****TS****（技术规范）格式。

## 技术规范（TR/TS）状态

### 已发布的技术规范

| ISO编号 | 名称 | 状态 | 链接、头文件 |
|---------|------|------|-------------|
| ISO/IEC TR 18015:2006 | C++性能技术报告 | 2006年发布 |  |
| ISO/IEC 29124:2010 | C++库扩展支持数学特殊函数 | 2010年发布，****✔****已合并到C++17 | 特殊函数（实验性）, 特殊函数 |
| ISO/IEC TS 18822:2015 | C++文件系统技术规范 | 2015年发布，****✔****已合并到C++17 | 文件系统（实验性）, 文件系统, `<filesystem>` |
| ISO/IEC TS 19570:2015 | C++并行扩展 | 2015年发布，****✔****已合并到C++17 | 并行性 |
| ISO/IEC TS 19217:2015 | C++概念扩展 | 2015年发布，****✔****已合并到C++20（有修改） | 约束和概念（实验性）, 概念, `<concepts>` |
| ISO/IEC TS 21425:2017 | 范围TS | 2017年发布，****✔****已合并到C++20 | 范围（实验性）, 范围, `<ranges>` |
| ISO/IEC TS 22277:2017 | 协程TS | 2017年发布，****✔****已合并到C++20 | `<coroutine>` |
| ISO/IEC TS 19216:2018 | 网络TS | 2018年发布 | 参见非TS提案 |
| ISO/IEC TS 21544:2018 | 模块TS | 2018年发布，****✔****已合并到C++20 |  |
| ISO/IEC TS 19570:2018 | 并行性TS版本2 | 2018年发布 | 并行性2 |
| ISO/IEC TS 23619:2021 | 反射TS | 2021年发布 | 反射 |
| ISO/IEC TS 9922 | 并发TS版本2 | 2024年发布 | 并发2") |
| ISO/IEC TS 19568:2024 | C++库基础扩展，版本3 | 2024年发布 | 库扩展3 |

## 非TS提案状态

### 已合并到C++26的特性

#### 1. 线性代数（Linear Algebra）
- ****✔****已合并到C++26
- 提案：P1673R13 (2023-11-10)
- 头文件：`<linalg>`

#### 2. 执行库（std::execution）
- ****✔****已合并到C++26
- 提案：P2300R10 (2024-06-28)
- 头文件：`<execution>`

#### 3. 就地向量（std::inplace_vector）
- ****✔****已合并到C++26
- 提案：P0843R14 (2024-06-26)
- 头文件：`<inplace_vector>`

#### 4. SIMD支持（std::simd）
- ****✔****已合并到C++26
- 提案：P1928R15 (2024-11-22)
- 头文件：`<simd>`

### 开发中的特性

#### 1. 模式匹配（Pattern Matching）
- 早期开发阶段
- 草案：P1371R3 (2020-09-15)

#### 2. 反射（Reflection）
- 主文档：ISO/IEC TS 23619:2021
- 多个相关提案在开发中

#### 3. 契约（Contracts）
- 早期开发阶段
- 提案：P2659R2 (2022-11-30)

#### 4. 2D图形（2D Graphics）
- 早期开发阶段
- 草案：P0267R10 (2019-10-07)

#### 5. 图形库（Graph Library）
- 早期开发阶段
- 多个相关提案

#### 6. 统计（Statistics）
- 早期开发阶段
- 提案：P1708R9 (2024-10-15)

#### 7. 配置文件（Profiles）
- 早期开发阶段
- 多个相关提案

## 扩展知识详解

### 一、实验性特性演进路径

#### 1. 从实验到标准化的典型流程
```cpp
// 实验性特性通常经历以下阶段：
// 1. 技术报告/技术规范（TR/TS）- 实验阶段
// 2. 提案（Proposal）- 标准化讨论
// 3. 正式标准 - 纳入C++标准

// 示例：文件系统库的演进
// TR阶段（实验性）
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

// C++17标准化后
#include <filesystem>
namespace fs = std::filesystem;
```

#### 2. 使用实验性特性的最佳实践
```cpp
// 条件编译使用实验性特性
#if __has_include(<execution>)
    #include <execution>
    #define HAS_STD_EXECUTION 1
#else
    #define HAS_STD_EXECUTION 0
#endif

void use_execution_if_available() {
#if HAS_STD_EXECUTION
    // 使用标准执行库
    auto result = std::execution::just(42)
        | std::execution::then([](int x) { return x * 2; });
#else
    // 回退实现
    auto result = 42 * 2;
#endif
}
```

### 二、重要实验性特性详解

#### 1. 线性代数库（C++26）
```cpp
#include <linalg>
#include <iostream>

void linear_algebra_example() {
    // 创建矩阵和向量
    std::linalg::vector<double> vec1{1.0, 2.0, 3.0};
    std::linalg::vector<double> vec2{4.0, 5.0, 6.0};
    
    // 向量运算
    auto dot_product = std::linalg::dot(vec1, vec2);
    auto cross_product = std::linalg::cross(vec1, vec2);
    
    std::cout << "Dot product: " << dot_product << std::endl;
    
    // 矩阵运算
    std::linalg::matrix<double> mat(3, 3);
    // ... 初始化矩阵
    
    auto result = std::linalg::matrix_vector_multiply(mat, vec1);
}
```

#### 2. SIMD支持（C++26）
```cpp
#include <simd>
#include <vector>
#include <iostream>

void simd_example() {
    // 创建SIMD向量
    std::simd<double> a{1.0, 2.0, 3.0, 4.0};
    std::simd<double> b{5.0, 6.0, 7.0, 8.0};
    
    // SIMD运算
    auto result = a + b;
    
    // 转换为标量数组
    std::vector<double> output(result.size());
    result.copy_to(output.data());
    
    for (const auto& val : output) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}
```

#### 3. 就地向量（C++26）
```cpp
#include <inplace_vector>
#include <iostream>

void inplace_vector_example() {
    // 固定容量的就地向量
    std::inplace_vector<int, 10> vec;
    
    // 添加元素
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    
    // 访问元素
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // 容量信息
    std::cout << "Size: " << vec.size() << std::endl;
    std::cout << "Capacity: " << vec.capacity() << std::endl;
}
```

### 三、开发中特性的预览

#### 1. 模式匹配（Pattern Matching）
```cpp
// 模式匹配的预期语法（非标准）
/*
match (value) {
    of(0) => std::cout << "zero\n",
    of(1) => std::cout << "one\n",
    of(let x) when (x > 10) => std::cout << "greater than 10: " << x << "\n",
    of(let x) => std::cout << "other: " << x << "\n"
};
*/
```

#### 2. 契约（Contracts）
```cpp
// 契约的预期语法（非标准）
/*
int divide(int a, int b) 
    [[expects: b != 0]]  // 前置条件
    [[ensures r: r == a/b]]  // 后置条件
{
    return a / b;
}
*/
```

### 四、使用实验性特性的注意事项

#### 1. 兼容性考虑
```cpp
// 版本检查
#if __cplusplus >= 202602L
    // C++26特性可用
#elif __cplusplus >= 202302L
    // C++23特性可用
#else
    // 使用传统实现
#endif

// 编译器支持检查
#ifdef __cpp_lib_execution
    // 执行库可用
#endif
```

#### 2. 实验特性迁移示例
```cpp
// 从实验性到标准的迁移
// 早期（实验性）
/*
#include <experimental/execution>
namespace exec = std::experimental::execution;
*/

// 现在（标准化后）
#include <execution>
namespace exec = std::execution;
```

### 五、跟踪实验性特性发展

#### 1. 关注提案状态
```cpp
// 建议关注的资源：
// 1. ISO C++官方网站
// 2. WG21提案文档
// 3. 编译器实现状态
// 4. C++标准委员会会议记录
```

#### 2. 实验性特性测试框架
```cpp
#include <iostream>
#include <string>

class ExperimentalFeatureTester {
public:
    static void test_feature_availability() {
        std::cout << "Testing experimental feature availability:\n";
        
#ifdef __cpp_lib_filesystem
        std::cout << "- Filesystem: Available\n";
#else
        std::cout << "- Filesystem: Not available\n";
#endif
        
#ifdef __cpp_lib_execution
        std::cout << "- Execution: Available\n";
#else
        std::cout << "- Execution: Not available\n";
#endif
        
#ifdef __cpp_lib_ranges
        std::cout << "- Ranges: Available\n";
#else
        std::cout << "- Ranges: Not available\n";
#endif
    }
};
```

## 相关资源

### 标准文档
1. **PL22.16/WG21文档列表** - 所有C++委员会文档（TS/提案）2016-2025年
2. **JTC1/SC22/WG21提案** - 2025年邮寄提案

### 相关页面
| 页面 | 说明 |
|------|------|
| C文档 - 实验性C特性 | C语言的实验性特性 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/experimental&oldid=179779>
- 最后修改时间：2025年1月20日 17:14
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++实验性特性代表了语言和标准库的未来发展方向。理解这些特性的演进路径和当前状态，有助于开发者提前了解和准备新技术。从文件系统库到并行算法，从协程到概念，实验性特性不断推动C++向前发展。正确使用条件编译和版本检查，可以在保持兼容性的同时利用新特性。跟踪WG21提案和编译器实现状态，可以帮助开发者及时了解特性的标准化进度。实验性特性的使用应该谨慎，考虑生产环境的稳定性和兼容性要求。持续关注和学习实验性特性，是保持C++技能前沿的重要途径。随着C++26等新标准的推出，这些实验性特性将逐步成为现代C++开发的标准工具。开发者应该在合适的时机采用这些新特性，以提高代码质量和开发效率。