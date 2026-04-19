# Reference

技术文档中文化项目，将高质量英文技术文档转换为结构化中文文档，便于中文开发者学习和参考。

## 项目简介

本项目旨在构建一个完整的中文技术文档知识库，涵盖 C、C++、Go、Java、Python、Rust 等主流编程语言。通过 Claude Code 自定义技能实现自动化文档处理，确保文档质量和一致性。

**当前状态**：共 447 篇文档（C 语言 172 篇 + C++ 语言 271 篇）

## 目录结构

```
Reference/
├── lang/                      # 语言参考文档
│   ├── c/                     # C 语言（172篇）
│   ├── cpp/                   # C++ 语言（271篇）
│   ├── go/                    # Go 语言（规划）
│   ├── java/                  # Java 语言（规划）
│   ├── python/                # Python 语言（规划）
│   └── rust/                  # Rust 语言（规划）
├── docs/                      # 辅助文档
│   ├── book.md                # 174本技术书籍推荐
│   ├── program.md             # 60个 C/C++ 练手项目
│   ├── third.md               # 第三方库管理指南
│   ├── linux_env/             # Linux 环境配置
│   └── vscode_pulgs/          # VSCode 插件配置
└── CMakePresets.json          # CMake 预设配置
```

## 语言文档结构

每个语言目录包含四个子模块：

| 目录 | 说明 |
|------|------|
| `language/` | 语言参考 - 核心语法和语义 |
| `language_standard/` | 标准版本 - 各版本特性与编译器支持 |
| `toolchain/` | 工具链 - 编译、调试、测试等工具 |
| `library/` | 标准库 - 核心库文档 |

### C 语言 (172篇)

```
lang/c/
├── language/                  # 语言参考（105篇）
│   ├── 00_basic_concepts/     # 基础概念（17篇）
│   ├── 01_keyword/            # 关键字（1篇）
│   ├── 02_declarations/       # 声明（26篇）
│   ├── 03_initialization/     # 初始化（4篇）
│   ├── 04_expressions/        # 表达式（27篇）
│   ├── 05_statements/         # 语句（10篇）
│   ├── 06_functions/          # 函数（6篇）
│   ├── 07_preprocessor/       # 预处理器（8篇）
│   └── 08_miscellaneous/      # 杂项（6篇）
├── language_standard/         # 标准版本（13篇）
│   ├── 00_versions/           # 版本概述（7篇）
│   └── 01_compiler_support/   # 编译器支持（6篇）
└── toolchain/                 # 工具链（54篇）
    ├── 00_build/              # 构建系统
    ├── 01_compiler/           # 编译器（GCC/Clang）
    ├── 02_package/            # 包管理
    ├── 03_debug/              # 调试工具
    ├── 04_format/             # 代码格式化
    ├── 05_static_analysis/    # 静态分析
    ├── 06_dynamic_analysis/   # 动态分析
    ├── 07_test/               # 测试框架
    ├── 08_doc/                # 文档生成
    ├── 09_binary/             # 二进制工具
    ├── 10_llvm/               # LLVM 工具链
    └── 11_coverage/           # 覆盖率分析
```

### C++ 语言 (271篇)

```
lang/cpp/
├── language/                  # 语言参考（200篇）
│   ├── 00_basic_concepts/     # 基础概念（22篇）
│   ├── 01_keyword/            # 关键字（1篇）
│   ├── 02_declarations/       # 声明（37篇）
│   ├── 03_initialization/     # 初始化（11篇）
│   ├── 04_expressions/        # 表达式（36篇）
│   ├── 05_statements/         # 语句（13篇）
│   ├── 06_functions/          # 函数（9篇）
│   ├── 07_classes/            # 类（28篇）
│   ├── 08_templates/          # 模板（19篇）
│   ├── 09_exceptions/         # 异常（7篇）
│   ├── 10_preprocessor/       # 预处理器（7篇）
│   ├── 11_idioms/             # 惯用法（6篇）
│   └── 12_miscellaneous/      # 杂项（4篇）
├── language_standard/         # 标准版本（13篇）
│   ├── 00_versions/           # 版本概述（7篇）
│   └── 01_compiler_support/   # 编译器支持（6篇）
└── toolchain/                 # 工具链（55篇）
    ├── 00_build/              # 构建系统
    ├── 01_compiler/           # 编译器（GCC/Clang/MSVC）
    ├── 02_package/            # 包管理（vcpkg/Conan）
    ├── 03_debug/              # 调试工具
    ├── 04_format/             # 代码格式化
    ├── 05_static_analysis/    # 静态分析
    ├── 06_dynamic_analysis/   # 动态分析
    ├── 07_test/               # 测试框架
    ├── 08_doc/                # 文档生成
    ├── 09_binary/             # 二进制工具
    ├── 10_llvm/               # LLVM 工具链
    └── 11_coverage/           # 覆盖率分析
```

## 辅助文档

### docs/book.md - 技术书单

174 本技术书籍推荐，涵盖：
- C/C++ 核心书籍（《C Primer Plus》《Effective C++》等）
- 数据结构与算法
- 操作系统与网络
- 软件工程与设计模式

### docs/program.md - 练手项目

60 个 C/C++ 实战项目，从初级到高级：
- **初级**：命令行工具、文件操作、基础算法
- **中级**：网络编程、数据结构、系统工具
- **高级**：编译器、数据库、操作系统组件

### docs/third.md - 第三方库管理

- vcpkg：跨平台 C++ 包管理器
- Git 子模块：源码集成方案
- 依赖管理最佳实践

### docs/linux_env/ - Linux 环境

SSH 配置、Git 环境设置等开发环境指南。

### docs/vscode_pulgs/ - VSCode 配置

| 配置文件 | 说明 |
|----------|------|
| `clangd.md` | clangd LSP 配置 |
| `cmake.md` | CMake 插件配置 |
| `doxygen.md` | 文档生成配置 |
| `pulgs_list.md` | 推荐插件列表 |
| `remote.md` | 远程开发配置 |

## 文档规范

每篇技术文档遵循标准结构：

1. **概述** - 功能简介和用途
2. **来源演变** - 标准版本历史
3. **语法参数** - 语法形式和参数说明
4. **底层原理** - 编译器实现和内存模型
5. **使用场景** - 最佳实践和典型用法
6. **代码示例** - 完整可运行的示例代码
7. **总结** - 关键要点和注意事项

## 自动化技能

本项目使用 Claude Code 自定义技能实现文档自动化：

| 技能 | 用途 | 命令 |
|------|------|------|
| `tech-doc-synthesis` | 单文档合成 | `/tech-doc-synthesis` |
| `doc-expand` | 文档扩展（4种深度） | `/doc-expand` |
| `batch-synthesis` | 批量并行处理 | `/batch-synthesis` |
| `batch-expand` | 批量文档扩展 | `/batch-expand` |
| `tech-learning-roadmap` | 学习路线图生成 | `/tech-learning-roadmap` |

### 使用示例

```bash
# 批量处理 C 语言基础概念文档
/batch-synthesis lang/c/language/00_basic_concepts/

# 批量扩展 C++ 类文档（深度模式）
/batch-expand lang/cpp/language/07_classes/ --mode detailed
```

## 构建配置

支持跨平台构建（CMake + Ninja + vcpkg）：

```bash
# 查看预设
cmake --list-presets

# Windows MSVC
cmake --preset windows-release

# Linux GCC
cmake --preset linux-release

# macOS Clang
cmake --preset mac-release
```

## 许可证

MIT License