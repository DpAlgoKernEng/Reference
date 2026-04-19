# C++ 编译器厂商兼容性清单

## 1. 概述

C++ 编译器厂商兼容性清单是各主流编译器对 C++ 语言标准（C++11、C++14、C++17、C++20、C++23、C++26）核心语言特性与标准库支持状态的汇总参考。该清单帮助开发者了解不同编译器版本对新特性的支持程度，以便在项目中选择合适的编译器和语言标准。

**核心内容**：
- 核心语言（core language）支持状态
- 标准库（standard library）支持状态
- 技术规范（Technical Specifications）支持状态
- 核心语言缺陷报告（defect report）处理状态

## 2. 来源与演变

C++ 标准化进程由 ISO C++ 委员会推动，每三年发布一个新标准。各编译器厂商按照自身节奏实现对标准的支持：

| 标准 | 发布年份 | 主要特性 |
|------|----------|----------|
| C++11 | 2011 | auto、lambda、移动语义、智能指针 |
| C++14 | 2014 | 泛型 lambda、二进制字面量、数字分隔符 |
| C++17 | 2017 | 结构化绑定、optional、string_view、if constexpr |
| C++20 | 2020 | 概念（concept）、范围（range）、模块（module）、协程（coroutine） |
| C++23 | 2023 | 标准库模块、std::expected、std::print |
| C++26 | 预计 2026 | 静态反射、契约、并发改进 |

**文档更新频率**：主流编译器（GCC、Clang、MSVC）保持每年更新，文档最后更新时间为 2025 年 1 月。

## 3. 主流编译器支持状态

### 3.1 GCC（GNU Compiler Collection）

**更新日期**：2025-01

| 标准版本 | 核心语言支持 | 标准库支持 |
|----------|-------------|-----------|
| C++11 | 完整支持（4.8.1 起），除 N2670（已在 C++23 移除） | 完整支持（5.1 起） |
| C++14 | 完整支持（5.1 起） | 完整支持（5.1 起） |
| C++17 | 完整支持（7.1 起） | 完整支持（12.0 起） |
| C++20 | 完整支持（11.0 起），模块部分未完成 | 完整支持（14.0 起） |
| C++23 | 开发中 | 开发中 |
| C++26 | 开发中 | 开发中 |

### 3.2 Clang（LLVM）

**更新日期**：2025-01

| 标准版本 | 核心语言支持 | 标准库支持 |
|----------|-------------|-----------|
| C++11 | 完整支持（3.3 起） | 完整支持（2012-07-29 起） |
| C++14 | 完整支持（3.4 起） | 完整支持（3.5 起） |
| C++17 | 完整支持（5.0 起） | 持续更新 |
| C++20 | 开发中 | 开发中 |
| C++23 | 开发中 | 开发中 |
| C++26 | 开发中 | 开发中 |

**特性**：提供实时 libc++ 符合性状态跟踪。

### 3.3 Apple Clang

**更新日期**：2025-01

**参考资源**：
- Xcode 工具链版本（维基百科）
- Xcode C++ 语言与标准库支持文档
- Xcode 发布说明
- Xcode 16 中的 C++20/23/26 支持状态

### 3.4 Microsoft Visual Studio（MSVC）

**更新日期**：2025-01

| 标准版本 | 核心语言支持 | 标准库支持 |
|----------|-------------|-----------|
| C++11 | 完整支持（VS 2015 起） | 完整支持（VS 2015 起） |
| C++14 | 完整支持（VS 2015 Update 2 起） | 完整支持（VS 2015 Update 2 起） |
| C++17 | 完整支持（VS 2017 15.7 起） | 完整支持（VS 2017 15.7 起） |
| C++20 | 开发中 | 开发中 |

**参考资源**：
- Microsoft C/C++ 语言符合性（Visual Studio 2015 及更高版本）
- Visual Studio 2022 变更日志
- Visual Studio 2019 中的 C++17/20 特性与修复

## 4. 其他编译器支持状态

### 4.1 Intel C++

**更新日期**：2023-01

| 标准版本 | 核心语言支持 | 说明 |
|----------|-------------|------|
| C++11 | 完整支持（15.0 起） | - |
| C++14 | 功能完整（17.0 起） | N3664 为优化特性 |
| C++17 | 不完整 | 持续开发中 |
| C++20 | 不完整 | 持续开发中 |

**标准库**：Intel 不提供独立的 C++ 标准库实现，但提供 Parallel STL（支持执行策略的 C++17 标准库算法实现）。

**兼容性**：与 GCC 的 libstdc++ 版本兼容。

### 4.2 EDG（Edison Design Group）

**更新日期**：2025-01

支持状态：
- C++11、C++14、C++17、C++20、C++23、C++26 核心语言支持状态跟踪

**说明**：EDG 不提供 C++ 标准库实现，其前端被多家编译器厂商采用。

### 4.3 Oracle C++

**更新日期**：2017-07

| 标准版本 | 支持状态 |
|----------|----------|
| C++11 | 5.14 版本完整支持（文档标注有误，仍显示 5.13） |
| C++14 | 5.15 版本完整支持 |

**标准库选项**：
| 库名称 | 说明 |
|--------|------|
| libCstd | RogueWave 标准库 v2，早于 C++98 |
| stlport4 | STLport v4.5.3，早于 C++03 |
| stdcxx4 | Apache 标准库 v4，早于 C++11 |
| libstdc++ | GCC 运行时库，支持 C++11/C++14（取决于版本） |

### 4.4 IBM XL C++

**更新日期**：2018-05

**Linux 版本**：
| 标准版本 | 支持状态 |
|----------|----------|
| C++11 | 完整支持（13.1.6 起） |
| C++14 | 部分支持（16.1.0） |

**AIX 版本**：
| 前端 | C++11 支持状态 |
|------|----------------|
| xlC | 部分支持（13.1.3、16.1.0） |
| xlclang | 完整支持（16.1.0 起） |

**标准库**：IBM 为 AIX 提供 Dinkumware 库版本，支持 C++ TR1 和 `<regex>`，但不支持 C++11。

### 4.5 HP aCC

**标准库**：提供 RogueWave STL 2.0 版本（C++98 标准库实现）。

**参考资源**：HP aC++ A.06.28 发布说明（包含 C++11 核心语言特性）。

### 4.6 Digital Mars C++

支持 C++11 核心语言特性。

### 4.7 Embarcadero C++

**RAD Studio 10.1 Berlin**：
- 传统编译器和 Clang 增强编译器（基于 Clang 3.3）支持的 C++11 特性

**RAD Studio 10.3 Rio**：
- 传统编译器支持的 C++11 特性
- Clang 增强编译器（基于 Clang 5.0）支持的 C++11、C++14、C++17 特性

### 4.8 Cray

**更新日期**：2023-02

| 版本 | C++14 支持状态 |
|------|----------------|
| 8.4 | 完整支持（除 alignas） |
| 8.6 | 完整支持 |
| 9.1 | 声明支持不超过 C++14 |

**HPE Cray Clang**（14.0 版本）：
- 从版本 11 起基于 Clang，具有相应的语言支持
- 协程或模块等涉及代码生成/链接的特性可能滞后（因支持 GPU 等受限设备）

### 4.9 Portland Group（PGI）

**更新日期**：2019-01

| 版本 | C++14 支持状态 |
|------|----------------|
| 2016 | 声明支持 C++14，但不支持：泛化 constexpr、constexpr 成员函数、变量模板、内存分配澄清 |

**标准库**：PGI 不提供 C++ 标准库实现。

### 4.10 NVIDIA CUDA nvcc

**更新日期**：2023-01

| 标准 | 支持版本 | 说明 |
|------|----------|------|
| C++17 | 11.0 起 | 受限于文档描述的限制 |
| C++20 | 12.0 起 | 受限于文档描述的限制 |

**标准库**：NVCC 不提供 C++ 标准库实现。

### 4.11 Texas Instruments

**更新日期**：2018-05

cl430 v18.1.0 声明支持 C++14。

### 4.12 Analog Devices

**更新日期**：2018-05

CrossCore Embedded Studio 2.8.0 for SHARC 声明支持 C++11。

## 5. 选择编译器的考量因素

### 5.1 适用场景

| 场景 | 推荐编译器 | 理由 |
|------|-----------|------|
| 跨平台开发 | GCC、Clang | 广泛的平台支持，标准符合性高 |
| Windows 开发 | MSVC | 与 Visual Studio 深度集成 |
| 嵌入式开发 | 特定厂商编译器 | 目标平台支持 |
| 高性能计算 | Intel C++ | 特定优化 |
| CUDA 开发 | nvcc | GPU 计算支持 |

### 5.2 注意事项

1. **标准符合性**：GCC 和 Clang 通常最快支持新标准特性
2. **标准库实现**：不同编译器可能使用不同的标准库（libstdc++、libc++、MSVC STL）
3. **平台限制**：某些编译器仅支持特定平台
4. **模块支持**：C++20 模块在各编译器中实现程度不同
5. **缺陷报告**：核心语言缺陷修复可能在不同版本中实现

### 5.3 常见陷阱

- 在不同编译器间迁移代码时，需注意标准库实现的差异
- 某些特性可能通过编译但行为不一致（实现定义行为）
- 旧版本编译器可能部分支持新标准特性，但需显式启用

## 6. 代码示例

### 6.1 检测编译器版本

```cpp
// 检测编译器类型和版本
#if defined(__GNUC__)
    #define COMPILER_NAME "GCC"
    #define COMPILER_VERSION __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__
#elif defined(__clang__)
    #define COMPILER_NAME "Clang"
    #define COMPILER_VERSION __clang_major__, __clang_minor__, __clang_patchlevel__
#elif defined(_MSC_VER)
    #define COMPILER_NAME "MSVC"
    #define COMPILER_VERSION _MSC_VER / 100, _MSC_VER % 100, 0
#endif

#include <iostream>

int main() {
    std::cout << "Compiler: " << COMPILER_NAME << std::endl;
    return 0;
}
```

### 6.2 条件编译启用特性

```cpp
// 根据编译器版本条件编译
#if __cplusplus >= 202002L
    #include <concepts>
    template<std::integral T>
    T add(T a, T b) { return a + b; }
#elif __cplusplus >= 201703L
    #include <type_traits>
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>, T>
    add(T a, T b) { return a + b; }
#else
    template<typename T>
    T add(T a, T b) { return a + b; }
#endif
```

### 6.3 编译器特定扩展

```cpp
// GCC/Clang 特性
#if defined(__GNUC__) || defined(__clang__)
    #define LIKELY(x)   __builtin_expect(!!(x), 1)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define LIKELY(x)   (x)
    #define UNLIKELY(x) (x)
#endif

// MSVC 特性
#ifdef _MSC_VER
    #include <intrin.h>
    #define POPCOUNT(x) __popcnt(x)
#else
    #define POPCOUNT(x) __builtin_popcount(x)
#endif
```

## 7. 总结

### 7.1 核心要点

- **GCC 和 Clang**：对新标准的支持最及时，通常在标准正式发布前已实现大部分特性
- **MSVC**：自 Visual Studio 2017 15.7 起完整支持 C++17，C++20 支持持续更新中
- **其他厂商编译器**：支持进度因平台和商业策略而异，嵌入式平台通常滞后
- **标准库**：主流编译器使用 libstdc++（GCC）、libc++（Clang）、MSVC STL（Microsoft）

### 7.2 选择建议

| 需求 | 建议 |
|------|------|
| 最新 C++ 特性 | 优先选择 GCC 14+ 或 Clang 18+ |
| Windows 平台 | MSVC 2022 或 Clang-cl |
| 嵌入式开发 | 参考目标平台的厂商编译器支持状态 |
| CUDA 开发 | nvcc 12.0+ 以支持 C++20 |

### 7.3 参考资源

- GCC C++ 标准支持状态：https://gcc.gnu.org/projects/cxx-status.html
- Clang C++ 标准支持状态：https://clang.llvm.org/cxx_status.html
- MSVC 语言符合性：https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance
- cppreference 编译器支持：https://en.cppreference.com/w/cpp/compiler_support

---

*文档来源：cppreference.com，最后更新于 2025 年 1 月 25 日*