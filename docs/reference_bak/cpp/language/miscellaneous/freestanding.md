# 自立式与宿主式实现（Freestanding and Hosted Implementations）

> 来源：cppreference.com

C++ 标准定义了两种实现类型：**自立式（freestanding）实现**和**宿主式（hosted）实现**。这两种实现类型的划分主要影响标准库的支持范围和程序的启动方式。

---

## 基本概念

| 实现类型     | 特点说明                                                                 |
|--------------|--------------------------------------------------------------------------|
| **宿主式实现** | 需要运行在操作系统之上，支持完整标准库。大多数常见的 C++ 程序均运行在宿主式实现环境中。 |
| **自立式实现** | 通常运行在无操作系统支持的环境中（如嵌入式系统、内核、固件），标准库支持有限。        |

### 宏定义判断

C++ 标准提供了宏 `__STDC_HOSTED__` 用于判断当前实现类型（自 C++11 起）：

| 宏值         | 含义                  |
|--------------|-----------------------|
| `1`          | 当前为宿主式实现      |
| `0`          | 当前为自立式实现      |

该宏由编译器**预定义**，用户不可更改。

---

## 多线程支持差异

| 项目 | 自立式（freestanding） | 宿主式（hosted） |
|------|------------------------|------------------|
| 多线程支持 | 是否支持多线程由实现定义（implementation-defined） | 明确支持并发执行的多线程（C++11 起） |

---

## `main` 函数要求差异

| 类型 | `main` 函数要求                                                                 |
|------|---------------------------------------------------------------------------------|
| **自立式** | 是否必须定义 `main` 函数由实现定义；程序启动和终止由实现定义（包括静态对象的构造和析构）。 |
| **宿主式** | 必须定义一个全局函数 `main`，作为程序入口；`main` 被主线程调用，同时可能涉及静态对象的初始化和销毁。 |

---

## 标准库头文件要求

### 自立式实现的头文件要求

自立式实现只需支持部分标准库头文件。**必须包含**的头文件如下表所示。

> ✅ **标记说明**：
> - 标注为 `// freestanding`：表示该实体必须提供。
> - 标注为 `// freestanding-deleted`（C++26 起）：表示该实体必须提供或被显式删除。

#### 必需的头文件列表（部分支持）

| 库名 | 组件 | 头文件 | 是否完整支持 |
|------|------|--------|---------------|
| 语言支持 | 公共定义 | `<cstddef>` | ✅ 全部支持 |
| C 标准库 | 标准库函数 | `<cstdlib>` | ⚠️ 部分支持 |
| 实现属性 | 限制常量 | `<cfloat>` `<climits>` `<limits>` `<version>` | ✅ 全部支持 |
| 整数类型 | 固定大小整型 | `<cstdint>` | ✅ 全部支持 |
| 动态内存 | 内存分配 | `<new>` | ✅ 全部支持 |
| 类型识别 | 运行时类型信息 | `<typeinfo>` | ✅ 全部支持 |
| 源码位置 | 源码信息 | `<source_location>` | ✅ 全部支持 |
| 异常处理 | 异常支持 | `<exception>` | ✅ 全部支持 |
| 初始化列表 | 初始化支持 | `<initializer_list>` | ✅ 全部支持 |
| 比较支持 | 比较运算 | `<compare>` | ✅ 全部支持 |
| 协程支持 | 协程机制 | `<coroutine>` | ✅ 全部支持 |
| 其他运行支持 | 可变参数 | `<cstdarg>` | ✅ 全部支持 |
| 概念支持 | 概念定义 | `<concepts>` | ✅ 全部支持 |
| 诊断支持 | 错误信息 | `<cerrno>` `<system_error>` | ⚠️ 部分支持 |
| 内存管理 | 智能指针等 | `<memory>` | ⚠️ 部分支持 |
| 元编程 | 类型特征 | `<type_traits>` | ✅ 全部支持 |
| 编译期数值 | 编译期比例 | `<ratio>` | ✅ 全部支持 |
| 工具组件 | 工具类 | `<utility>` | ✅ 全部支持 |
| 元组支持 | 元组类型 | `<tuple>` | ✅ 全部支持 |
| 函数对象 | 函数包装 | `<functional>` | ⚠️ 部分支持 |
| 数值转换 | 字符转换 | `<charconv>` | ⚠️ 部分支持 |
| 位操作 | 位处理 | `<bit>` | ✅ 全部支持 |
| 字符串支持 | 字符串类 | `<string>` | ⚠️ 部分支持 |
| 空终止序列 | 字符串处理 | `<cstring>` `<cwchar>` | ⚠️ 部分支持 |
| 迭代器支持 | 迭代器机制 | `<iterator>` | ⚠️ 部分支持 |
| 范围支持 | 范围算法 | `<ranges>` | ⚠️ 部分支持 |
| 数值支持 | 数学函数 | `<cmath>` | ⚠️ 部分支持 |
| 并发支持 | 原子操作 | `<atomic>` | ✅ 支持（部分原子类型可能不支持） |

> 🔒 **并发说明**：自立式实现对于 `std::atomic` 中的无锁原子类型支持由实现定义（C++20 起）。

#### 废弃头文件（C++20 起）

| 头文件 | 说明 |
|--------|------|
| `<ciso646>` | C++20 起被弃用 |
| `<cstdalign>` | C++20 起被弃用 |
| `<cstdbool>` | C++20 起被弃用 |

---

## 实践注意事项（Notes）

- 一些编译器厂商尚未完全支持自立式实现：
  - **GCC libstdc++**：在 13 版本前存在构建问题。
  - **LLVM libcxx** 与 **MSVC STL**：不支持完全的自立式环境。

- **C++23** 开始，许多库组件支持自立式版本（通过“部分头文件”机制）。
- 然而，WG21（C++ 标准委员会）仍在讨论是否将更多库组件加入自立式支持。
- **容器类如 `<vector>`、`<list>`、`<deque>`、`<map>` 等不会被支持为自立式**，因为它们依赖异常与堆内存管理。

- **GCC 13** 引入了对 `<optional>`、`<span>`、`<array>`、`<bitset>` 等头文件的自立式支持，但这些功能可能不具有可移植性或完全兼容宿主环境。因此：
  > ❗ **建议避免在自立式环境中使用这些组件**，即使你的工具链支持。

---

## 特性测试宏（Feature-test Macros）

C++26 提供了用于检测自立式特性支持的宏，如下所示：

| 宏名 | 值 | 标准 | 描述 |
|------|----|------|------|
| `__cpp_lib_freestanding_feature_test_macros` | `202306L` | C++26 | 自立式特性宏测试 |
| `__cpp_lib_freestanding_algorithm` | `202311L` | C++26 | `<algorithm>` 支持 |
| `__cpp_lib_freestanding_array` | `202311L` | C++26 | `<array>` 支持 |
| `__cpp_lib_freestanding_char_traits` | `202306L` | C++26 | `std::char_traits` 支持 |
| `__cpp_lib_freestanding_charconv` | `202306L` | C++26 | `<charconv>` 支持 |
| `__cpp_lib_freestanding_cstdlib` | `202306L` | C++26 | `<cstdlib>` 支持 |
| `__cpp_lib_freestanding_cstring` | `202311L` | C++26 | `<cstring>` 支持 |
| `__cpp_lib_freestanding_cwchar` | `202306L` | C++26 | `<cwchar>` 支持 |
| `__cpp_lib_freestanding_errc` | `202306L` | C++26 | `std::errc` 支持 |
| `__cpp_lib_freestanding_expected` | `202311L` | C++26 | `<expected>` 支持 |
| `__cpp_lib_freestanding_functional` | `202306L` | C++26 | `<functional>` 支持 |
| `__cpp_lib_freestanding_iterator` | `202306L` | C++26 | `<iterator>` 支持 |
| `__cpp_lib_freestanding_mdspan` | `202311L` | C++26 | `<mdspan>` 支持 |
| `__cpp_lib_freestanding_memory` | `202306L` | C++26 | `<memory>` 支持 |
| `__cpp_lib_freestanding_numeric` | `202311L` | C++26 | `<numeric>` 支持 |
| `__cpp_lib_freestanding_optional` | `202311L` | C++26 | `<optional>` 支持 |
| `__cpp_lib_freestanding_ranges` | `202306L` | C++26 | `<ranges>` 支持 |
| `__cpp_lib_freestanding_ratio` | `202306L` | C++26 | `<ratio>` 支持 |
| `__cpp_lib_freestanding_string_view` | `202311L` | C++26 | `<string_view>` 支持 |
| `__cpp_lib_freestanding_tuple` | `202306L` | C++26 | `<tuple>` 支持 |
| `__cpp_lib_freestanding_utility` | `202306L` | C++26 | `<utility>` 支持 |
| `__cpp_lib_freestanding_variant` | `202311L` | C++26 | `<variant>` 支持 |

---

## 标准参考（References）

- **C++23 (ISO/IEC 14882:2024)**
  - 4.1 实现合规性
  - 6.9.2 多线程执行与数据竞争
  - 6.9.3.1 main 函数
  - 16.4.2.5 自立式实现支持

- **C++20 (ISO/IEC 14882:2020)**
  - 4.1 实现合规性
  - 6.9.2 多线程执行与数据竞争
  - 6.9.3.1 main 函数
  - 16.5.1.3 自立式实现支持

- **其他版本**（C++17、C++14、C++11、C++03）均有类似内容，可参考相应标准章节。

---

## 缺陷报告（Defect Reports）

| 编号 | 应用于 | 行为描述 | 修复方式 |
|------|--------|-----------|-----------|
| CWG 1938 | C++98 | 实现无需文档说明是否为宿主式实现 | 明确实现类型必须文档说明 |
| LWG 3653 | C++20 | `<coroutine>` 依赖未支持的 `std::hash` | 将 `<functional>` 变为部分支持 |

---

## 参见（See Also）

| 主题 | 链接 |
|------|------|
| C 语言标准中的合规性 | [C documentation for Conformance] |

---

来源：<https://en.cppreference.com/mwiki/index.php?title=cpp/freestanding&oldid=167777>  
页面最后修改时间：2023年12月29日 23:40  
离线版本获取时间：2025年2月9日 16:39

---