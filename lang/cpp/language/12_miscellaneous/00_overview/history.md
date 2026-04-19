# C++ 语言发展历史

## 1. 概述 (Overview)

本文档记录了 C++ 编程语言从诞生至今的完整发展历程。C++ 是由 Bjarne Stroustrup 在贝尔实验室开发的通用编程语言，最初被称为"C with Classes"（带类的 C），后于 1983 年正式命名为 C++。本文档详细描述了 C++ 从 1979 年至今的各个重要版本发布、语言特性演进以及标准化进程。

C++ 的发展历程可分为两个主要阶段：
- **早期 C++（1979-1990）**：从"C with Classes"到 Cfront 编译器的演进
- **标准 C++（1990 至今）**：从 ANSI/ISO 标准化到现代 C++ 的持续演进

## 2. 来源与演变 (Origin and Evolution)

### 2.1 早期 C++ (1979-1990)

#### 1979 年：C with Classes 首次实现

C with Classes 是 C++ 的前身，由 Bjarne Stroustrup 在贝尔实验室开发。

**新增特性**：
- 类（classes）和成员函数（member functions）
- 派生类（derived classes）和分离编译（separate compilation）
- 公有和私有访问控制（public and private access control）
- 友元（friends）
- 函数参数类型检查（type checking of function arguments）
- 默认参数（default arguments）
- 内联函数（inline functions）
- 重载赋值运算符（overloaded assignment operator）
- 构造函数（constructors）和析构函数（destructors）
- `f()` 与 `f(void)` 等价
- 调用函数和返回函数（同步特性，未进入 C++）

**库支持**：
- 并发任务库（concurrent task library，未进入 C++）

#### 1982 年：C with Classes 参考手册发布

正式发布了 C with Classes 的参考手册，为后续发展奠定基础。

#### 1984 年：C84 实现

C84 实现完成并发布参考手册。

#### 1985 年：Cfront 1.0 发布

Cfront 是最早的 C++ 编译器前端，将 C++ 代码转换为 C 代码。

**新增特性**：
- 虚函数（virtual functions）
- 函数和运算符重载（function and operator overloading）
- 引用（references）
- `new` 和 `delete` 运算符
- `const` 关键字
- 作用域解析运算符（scope resolution operator `::`）

**库扩展**：
- 复数（complex number）
- `string`（AT&T 版本）
- I/O 流（I/O stream）

#### 1985 年：《The C++ Programming Language》第一版出版

Bjarne Stroustrup 的经典著作首次出版，成为学习 C++ 的权威参考。

#### 1986 年："whatis?" 论文

发表了"whatis?"论文，记录了剩余的设计目标，包括：
- 多重继承（multiple inheritance）
- 异常处理（exception handling）
- 模板（templates）

#### 1987 年：GCC 1.15.3 支持 C++

GNU 编译器集合（GCC）开始支持 C++ 语言。

#### 1989 年：Cfront 2.0 发布

**新增特性**：
- 多重继承（multiple inheritance）
- 成员指针（pointers to members）
- protected 访问控制
- 类型安全链接（type-safe linkage）
- 抽象类（abstract classes）
- static 和 const 限定的成员函数
- 类特定的 new 和 delete

**库扩展**：
- I/O 操纵器（I/O manipulators）

#### 1990 年：《The Annotated C++ Reference Manual》出版

该书描述了设计中的语言，包括一些尚未实现的特性，在 ISO 标准化之前作为事实标准。

**新增特性**：
- 命名空间（namespaces）
- 异常处理（exception handling）
- 嵌套类（nested classes）
- 模板（templates）

#### 1991 年：Cfront 3.0 发布、《The C++ Programming Language》第二版出版

Cfront 3.0 进一步完善了模板支持。

### 2.2 标准 C++ 时代 (1990 至今)

#### 标准化进程启动

- **1990 年**：ANSI C++ 委员会成立
- **1991 年**：ISO C++ 委员会成立
- **1992 年**：STL 在 C++ 中实现

### 2.3 C++98/03 时期

#### 1998 年：C++98 (ISO/IEC 14882:1998)

第一个正式的 C++ 国际标准发布。

**新增特性**：
- RTTI（运行时类型识别）：`dynamic_cast`、`typeid`
- 协变返回类型（covariant return types）
- 类型转换运算符（cast operators）
- `mutable` 关键字
- `bool` 类型
- 条件中的声明（declarations in conditions）
- 模板实例化（template instantiations）
- 成员模板（member templates）
- `export` 关键字（后被移除）

**库扩展**：
- 本地化（locales）
- `bitset`
- `valarray`
- `auto_ptr`
- 模板化的 `string`、I/O 流和复数

**基于 STL**：
- 容器（containers）
- 算法（algorithms）
- 迭代器（iterators）
- 函数对象（function objects）

#### 1998 年：《The C++ Programming Language》第三版出版

#### 1999 年：Boost 库项目成立

委员会成员创立 Boost，旨在为标准库生产高质量的新候选库。

#### 2003 年：C++03 (ISO/IEC 14882:2003)

这是一个小版本修订，主要是技术勘误。此版本引入了值初始化（value initialization）的定义。

**缺陷报告修复**：92 个核心缺陷 + 125 个库缺陷

#### 2006 年：性能技术报告 (ISO/IEC TR 18015:2006)

讨论了各种 C++ 抽象的成本，提供实现指导，讨论 C++ 在嵌入式系统中的使用，并引入了 `<hardware>` 接口。

#### 2007 年：库扩展 TR1 (ISO/IEC TR 19768:2007)

C++ 库扩展技术报告，添加以下内容：

**来自 Boost**：
- `reference_wrapper`
- 智能指针（Smart pointers）
- 成员函数（Member function）
- `result_of`
- `bind`、`function`
- 类型特征（Type Traits）
- 随机数（Random）
- 数学特殊函数（Mathematical Special Functions）
- `tuple`、`array`
- 无序容器（Unordered Containers，包括 hash）
- 正则表达式（Regular Expressions）

**来自 C99**：
- `<math.h>` 中的新数学函数
- 空白字符类
- 浮点环境
- hexfloat I/O 操纵器
- 固定大小整数类型
- `long long` 类型
- `va_copy`
- `snprintf()` 和 `vfscanf()` 函数族
- C99 的 printf/scanf 转换说明符

TR1 中除特殊函数外的所有内容都包含在 C++11 中（有少量修改）。

#### 2010 年：数学特殊函数 (ISO/IEC 29124:2010)

添加 TR1 中包含但未进入 C++11 的特殊函数：
- 椭圆积分（elliptic integrals）
- 指数积分（exponential integral）
- 拉盖尔多项式（Laguerre polynomials）
- 勒让德多项式（Legendre polynomials）
- 埃尔米特多项式（Hermite polynomials）
- 贝塞尔函数（Bessel functions）
- 诺伊曼函数（Neumann functions）
- beta 函数
- 黎曼 zeta 函数

此标准后来合并到 C++17 中。

### 2.4 C++11 时期

#### 2011 年：C++11 (ISO/IEC 14882:2011)

也称为 C++0x，是一次重大修订，引入了大量变化以标准化现有实践并改进 C++ 程序员可用的抽象。

**主要特性**：
- 自动类型推导（`auto`）
- 范围 for 循环
- 右值引用和移动语义
- 智能指针（`unique_ptr`、`shared_ptr`）
- Lambda 表达式
- `constexpr`
- 初始化列表
- 统一初始化语法
- `nullptr`
- 强类型枚举
- `decltype`
- 可变参数模板
- 模板别名
- 静态断言
- 线程支持库

#### 2011 年：十进制浮点 TR (ISO/IEC TR 24733:2011)

实现 IEEE 754-2008 浮点算术标准中的十进制浮点类型：
- `std::decimal::decimal32`
- `std::decimal::decimal64`
- `std::decimal::decimal128`

#### 2012 年：Standard C++ Foundation 成立

C++ 基金会成立，推动 C++ 语言和社区的发展。

#### 2013 年：《The C++ Programming Language》第四版出版

### 2.5 C++14 时期

#### 2014 年：C++14

C++ 标准的小版本修订。

**主要特性**：
- 泛型 lambda
- 返回类型推导
- `constexpr` 扩展
- 变量模板
- 二进制字面量
- 数字分隔符
- 标准库用户定义字面量

#### 2015 年：文件系统库 TS (ISO/IEC TS 18822:2015)

基于 boost.filesystem V3 的实验性文件系统库扩展，后合并到 C++17。

#### 2015 年：并行扩展 TS (ISO/IEC TS 19570:2015)

标准化所有标准库算法的并行和向量并行 API，添加 `reduce`、`transform_reduce`、`exclusive_scan` 等新算法。后合并到 C++17。

#### 2015 年：事务内存 TS (ISO/IEC TS 19841:2015)

用同步块和原子块扩展 C++ 核心语言，实现事务内存语义。

#### 2015 年：库基础 TS (ISO/IEC TS 19568:2015)

向 C++ 标准库添加多个新组件：
- `optional`、`any`、`string_view`
- `sample`、`search`、`apply`
- 多态分配器（polymorphic allocators）
- 类型特征的变量模板

后合并到 C++17。

#### 2015 年：概念 TS (ISO/IEC TS 19217:2015)

用概念（命名的类型要求）和约束（模板、函数和变量声明中允许的类型限制）扩展 C++ 核心语言，辅助元编程并简化模板实例化诊断。后合并到 C++20（有部分省略）。

#### 2016 年：并发 TS (ISO/IEC TS 19571:2016)

扩展 C++ 库，包括 `std::future` 的扩展、latches 和 barriers、原子智能指针。

### 2.6 C++17 时期

#### 2017 年：C++17

C++11 之后的重大标准修订。

**主要特性**：
- 结构化绑定
- `if` 和 `switch` 中的初始化语句
- 内联变量
- `constexpr if`
- 折叠表达式
- 类模板参数推导
- `auto` 非类型模板参数
- `std::optional`、`std::variant`、`std::any`
- `std::string_view`
- 并行算法
- 文件系统库

#### 2017 年：Ranges TS (ISO/IEC TS 21425:2017)

扩展 C++ 库以包含 ranges，一种更强大的抽象来替代迭代器对，以及 range views、sentinel ranges、投影、新的迭代器适配器和算法。使得可以用 `sort(v);` 对 vector 排序。

#### 2017 年：协程 TS (ISO/IEC TS 22277:2017)

扩展 C++ 核心语言和标准库以包含无栈协程（可恢复函数）。添加关键字 `co_await`、`co_yield`、`co_return`。

#### 2018 年：网络 TS (ISO/IEC TS 19216:2018)

扩展 C++ 库以包含基于 boost.asio 的 TCP/IP 网络支持。

#### 2018 年：模块 TS (ISO/IEC TS 21544:2018)

扩展 C++ 核心语言以包含模块。添加特殊标识符 `module`、`import`，并重新引入 `export` 关键字（新含义）。

#### 2018 年：并行 V2 TS (ISO/IEC TS 19570:2018)

扩展 C++ 库：
- 两个新的执行策略（`unseq` 和 `vec`）
- 额外的并行算法（如 `reduction_plus`、`for_loop_strided`）
- 并行任务的 fork/join 任务块
- SIMD 类型及相关操作

### 2.7 C++20 时期

#### 2020 年：C++20

C++17 之后的重大标准修订。

**主要特性**：
- 概念（Concepts）
- 模块（Modules）
- 协程（Coroutines）
- Ranges 库
- `std::span`
- 指定初始化器
- 三向比较运算符 `<=>`
- `constinit`
- `consteval`
- `std::format`
- 日历和时区库
- 并发改进（`std::jthread`、信号量、latches、barriers）

#### 2021 年：反射 TS (ISO/IEC TS 23619:2021)

扩展 C++ 以检查程序实体（变量、枚举、类及其成员、lambda 及其捕获等）。

### 2.8 未来发展

#### 实验性技术规范

多项技术规范正在开发中，为未来标准做准备。

#### 2026 年：C++26

C++ 的下一个主要修订版本，最新草案为 n5001（2024-12-17）。

## 3. 语法与参数 (Syntax and Parameters)

N/A - 本文档为历史记录文档，不涉及特定语法或参数说明。

## 4. 底层原理 (Underlying Principles)

N/A - 本文档为历史记录文档，不涉及底层实现原理。

## 5. 使用场景 (Use Cases)

N/A - 本文档为历史记录文档，不涉及具体使用场景。

## 6. 代码示例 (Examples)

N/A - 本文档为历史记录文档，不涉及代码示例。

## 7. 总结 (Summary)

### 7.1 核心要点

C++ 语言发展历程的核心里程碑如下表所示：

| 年份 | 版本/事件 | 重要性 |
|------|-----------|--------|
| 1979 | C with Classes | C++ 前身诞生 |
| 1985 | Cfront 1.0 | 首个 C++ 编译器 |
| 1998 | C++98 | 首个国际标准 |
| 2011 | C++11 | 现代 C++ 开端 |
| 2014 | C++14 | 小版本完善 |
| 2017 | C++17 | 重大修订 |
| 2020 | C++20 | 概念、模块、协程 |
| 2026 | C++26 | 下一个主要版本 |

### 7.2 发展阶段对比

| 发展阶段 | 时间范围 | 特点 |
|----------|----------|------|
| 早期 C++ | 1979-1990 | 从 C with Classes 演进，核心特性确立 |
| 标准化初期 | 1990-1998 | 标准化进程启动，STL 集成 |
| C++98/03 时期 | 1998-2010 | 第一个标准，成熟稳定 |
| 现代 C++ | 2011 至今 | 大幅改进，每 3 年一个版本 |

### 7.3 技术规范 (TS) 与标准关系

多个技术规范为正式标准提供实验性特性：

| 技术规范 | 合并版本 |
|----------|----------|
| TR1 | C++11 |
| 文件系统 TS | C++17 |
| 并行 TS | C++17 |
| 库基础 TS | C++17 |
| 概念 TS | C++20 |
| 协程 TS | C++20 |
| Ranges TS | C++20 |

### 7.4 学习建议

1. **了解历史有助于理解语言设计**：C++ 的许多特性都有其历史背景和演进原因
2. **关注现代 C++**：C++11 及以后版本被称为"现代 C++"，建议优先学习
3. **追踪新标准**：C++ 现在每 3 年发布一个新版本，保持学习新特性
4. **利用 Boost 库**：许多标准库特性源自 Boost，是学习新技术规范的好资源

### 7.5 参考资料

| 资源 | 说明 |
|------|------|
| A History of C++: 1979-1991 | Bjarne Stroustrup 的历史回顾论文 |
| Evolving a language in and for the real world: C++ 1991-2006 | 语言演进论文 |
| Thriving in a crowded and changing world: C++ 2006-2020 | 近期发展论文 |
| Standard C++ Foundation | C++ 基金会官方网站 |
| C++ Standards Committee | ISO C++ 标准委员会 |