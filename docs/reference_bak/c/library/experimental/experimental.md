# 实验性 C++ 特性（Experimental C++ Features）

来源：[cppreference.com](https://en.cppreference.com)

---

## 实验性特性总览

| 类别 | 功能名称 | 描述 |
|------|----------|------|
| **技术规范阶段** | 文件系统库（Filesystem TS） | 提供标准文件和目录操作接口 |
|                    | 标准库基础扩展（Library Fundamentals TS） | 扩展现有标准库组件功能 |
|                    | 并行计算扩展（Parallelism TS） | 支持数据并行和 SIMD 操作 |
|                    | 并发编程扩展（Concurrency TS） | 新增线程间通信机制 |
|                    | 约束与概念（Concepts TS） | 引入模板类型约束语法 |
|                    | 范围（Ranges TS） | 更灵活、安全的迭代器抽象 |
|                    | 反射（Reflection TS） | 支持元编程时获取类型信息 |
|                    | 数学特殊函数（Special Functions TR） | 增强数学运算能力 |
| **非技术规范扩展** | 模式匹配（Pattern Matching） | 提供类似 Python/Rust 的匹配语法 |
|                    | 线性代数（Linear Algebra） | 支持向量和矩阵计算 |
|                    | 执行器（std::execution） | 新一代异步执行模型 |
|                    | 类契约（Contracts） | 编译期/运行期契约验证机制 |
|                    | 图形库（2D Graphics） | 标准图形渲染接口 |
|                    | 统计分析（Statistics） | 基础统计函数支持 |

---

## 技术报告（TR）与技术规范（TS）发展历史

> ⚠️ 注：2012年前使用 **TR**（Technical Report）形式，之后改用 **TS**（Technical Specification）。

| ISO 编号 | 名称 | 状态 | 链接及头文件 |
|----------|------|------|---------------|
| ISO/IEC TR 18015:2006 | C++ 性能技术报告 | 发布于 2006 年 | [ISO Store](https://www.iso.org/standard/43599.html) |
| ISO/IEC TR 19768:2007 | C++ 库扩展技术报告 | 发布于 2007 年，ISO 29124 分出，其余合并至 C++11 | [ISO Store](https://www.iso.org/standard/43599.html) |
| ISO/IEC 29124:2010 | 数学特殊函数扩展 | 发布于 2010 年，已并入 C++17 | `<cmath>` |
| ISO/IEC TS 18822:2015 | 文件系统技术规范 | 发布于 2015 年，已并入 C++17 | `<filesystem>` |
| ISO/IEC TS 19570:2015 | 并行计算扩展 | 发布于 2015 年，部分并入 C++17 | `<execution>` |
| ISO/IEC TS 19568:2015 | 标准库基本扩展 | 发布于 2015 年，部分并入 C++17 | `<experimental/fundamentals_v1>` |
| ISO/IEC TS 19217:2015 | 概念扩展 | 发布于 2015 年，大幅修改后并入 C++20 | `<concepts>` |
| ISO/IEC TS 19571:2016 | 并发扩展 | 发布于 2016 年，部分并入 C++20 | `<future>` |
| ISO/IEC TS 21425:2017 | 范围技术规范 | 发布于 2017 年，已并入 C++20 | `<ranges>` |
| ISO/IEC TS 22277:2017 | 协程技术规范 | 发布于 2017 年，已并入 C++20 | `<coroutine>` |
| ISO/IEC TS 19216:2018 | 网络编程技术规范 | 发布于 2018 年 | 仍在草案阶段 |
| ISO/IEC TS 21544:2018 | 模块化技术规范 | 发布于 2018 年，已并入 C++20 | `<module>` |
| ISO/IEC TS 19570:2018 | 并行计算 V2 | 发布于 2018 年，仍在草案阶段 | `<execution>` |
| ISO/IEC TS 23619:2021 | 反射技术规范 | 发布于 2021 年 | `<experimental/reflection>` |
| ISO/IEC TS 19568:2024 | 标准库基本扩展 V3 | 发布于 2024 年 | `<experimental/fundamentals_v3>` |
| ISO/IEC TS 9922:2024 | 并发扩展 V2 | 发布于 2024 年 | `<future>` |

---

## 非技术规范提案（Non-TS Proposals）

### 1. 线性代数库（Linear Algebra）

| 提案编号 | 状态 | 更多链接 |
|----------|------|----------|
| [P1673R13](https://wg21.link/P1673R13) | 已于 C++26 合并 | 更多提案参考下方 |
| 主要贡献者 | Mark Hoemmen 等 | [GitHub 项目主页](https://github.com/ORNL/cpp-proposals-pub) |

#### 核心组件
- `<linalg>` 头文件
- `std::linalg::vector`, `std::linalg::matrix`
- 支持 BLAS 接口兼容性
- 内置 SIMD 加速优化选项

#### 示例代码

```cpp
#include <linalg>

int main() {
    std::linalg::vector<float> a{1.0f, 2.0f};
    std::linalg::vector<float> b{3.0f, 4.0f};
    auto dot = std::linalg::dot(a, b); // 返回 11.0
}
```

---

### 2. 执行器（std::execution）

| 提案编号 | 状态 | 更多链接 |
|----------|------|----------|
| [P2300R10](https://wg21.link/P2300R10) | 已于 C++26 合并 | 更多提案参考下方 |

#### 核心组件
- `<execution>` 头文件
- `std::execution::start_detached`
- `std::execution::then`, `std::execution::on`

#### 示例代码

```cpp
#include <execution>
#include <iostream>

int main() {
    auto task = std::execution::just(42)
        | std::execution::then([](int x) {
            return x * 2;
        })
        | std::execution::then([](int y) {
            std::cout << "结果: " << y << '\n';
        });

    std::execution::start_detached(std::move(task));
}
```

---

### 3. SIMD 类型（std::simd）

| 提案编号 | 状态 | 更多链接 |
|----------|------|----------|
| [P1928R15](https://wg21.link/P1928R15) | 已于 C++26 合并 | 更多提案参考下方 |

#### 核心组件
- `<simd>` 头文件
- `std::simd<float>`
- 支持多种架构（SSE、AVX）

#### 示例代码

```cpp
#include <simd>

void add_vectors(const float* a, const float* b, float* result, int n) {
    for (int i = 0; i < n; i += std::simd<float>::size()) {
        std::simd<float> va(a + i);
        std::simd<float> vb(b + i);
        (va + vb).copy_to(result + i);
    }
}
```

---

### 4. 契约机制（Contracts）

| 提案编号 | 状态 | 更多链接 |
|----------|------|----------|
| [P2900R13](https://wg21.link/P2900R13) | C++26 待定 | 更多提案参考下方 |

#### 特性预览
- 使用 `contract_assert` 声明前置/后置条件
- 支持运行期和编译期检查
- 可配置违规行为（终止、中断、忽略）

#### 示例代码

```cpp
#include <contract>

int divide(int a, int b) 
    contract_assert(b != 0, "除数不能为零"); 
{
    return a / b;
}
```

---

## 当前仍在开发中的提案

| 提案类型 | 最新提案编号 | 描述 |
|----------|--------------|------|
| 模式匹配 | [P2688R5](https://wg21.link/P2688R5) | 提供 `match` 表达式语法 |
| 类契约 | [P2900R13](https://wg21.link/P2900R13) | 完善契约模型 |
| 反射 | [P2996R9](https://wg21.link/P2996R9) | 增强类型元信息获取 |
| 图形库 | [P0267R10](https://wg21.link/P0267R10) | 标准图形接口 |
| 统计库 | [P1708R9](https://wg21.link/P1708R9) | 基础统计函数 |

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| 实验性 C 特性 | [Experimental C Features](https://en.cppreference.com/w/c/experimental) |

## 外部资源链接

| 资源名称 | 链接 |
|----------|------|
| C++委员会文档列表 | [PL22.16/WG21 Documents List](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/) |
| 最新提案邮件列表 | [WG21 Mailing List](https://lists.isocpp.org/mailman/listinfo.cgi/std-proposals) |

---

> 页面来源：[cppreference.com/c/experimental](https://en.cppreference.com/mwiki/index.php?title=cpp/experimental)  
> 版本信息：最后更新于 2025年1月20日 17:14 UTC  
> 本地快照获取时间：2025年2月9日 16:39

--- 

✅ **小结提示**：
- 实验性特性是未来标准化的内容候选。
- 为了确保稳定性，正式项目应优先使用标准化特性。
- 开发人员可关注 GitHub 上的 C++ 提案仓库跟踪最新进展。