# 技术文档中文化项目

将技术文档（C/C++ Reference 等）转换为结构化的中文技术文档。

## 项目简介

本项目旨在将高质量的技术文档（如 cppreference、Linux 文档等）转换为结构化的中文文档，便于中文开发者学习和参考。通过 Claude Code 的自定义技能（Skill）实现自动化文档分析和翻译。

## 核心功能

**/batch-synthesis 技能**：核心技能 `/batch-synthesis` 能够分析技术文档并生成结构化的中文文档，输出 7 个标准章节

## 使用方式

**示例1**：/batch-synthesis 使用这个技能，对 original_docs/c/language/statements/*.md 分析生成中文文档，输出到对应的 docs/c/language/statements/*.md 中，if、switch、statements 已经被处理，剩下其他的都处理一下。

**示例2**：/batch-synthesis 使用这个技能，对 original_docs/cpp/language/statements/*.md 分析生成中文文档，输出到对应的 docs/cpp/language/statements/*.md 中，if、switch、statements 已经被处理，剩下其他的都处理一下。

**示例3**： /batch-synthesis original_docs/c/language_standard/compiler_support docs/c/language_standard/compiler_support "*.md" standard

**示例4**： /batch-synthesis original_docs/cpp/language_standard/compiler_support docs/cpp/language_standard/compiler_support "*.md" standard

**示例5**： /batch-expand original_docs/c/toolchain docs/c/toolchain detailed "*.md"

**示例6**： /batch-expand original_docs/cpp/toolchain docs/cpp/toolchain detailed "*.md"

## 项目结构

```
.
├── docs/                      # 生成的中文文档输出目录
│   ├── c/                     # C 语言文档
│   ├── cpp/                   # C++ 文档
│   ├── go/                    # Go 语言文档（待填充）
│   ├── java/                  # Java 文档（待填充）
│   └── rust/                  # Rust 文档（待填充）
├── original_docs/             # 原始英文文档目录
│   ├── c/
│   ├── cpp/
│   ├── go/
│   ├── java/
│   └── rust/
├── .claude/                   # Claude Code 配置
│   ├── settings.json          # 全局设置
│   └── skills/                # 自定义技能
│       └── cppref-synthesis/  # 技术文档分析技能
└── README.md                  # 本文件
```

```
❯   toolchain/
  ├── 00_build/              # 构建系统
  │   ├── cmake.md
  │   ├── make.md
  │   ├── ninja.md           # ✚ 新增
  │   ├── meson.md           # ✚ 新增
  │   ├── bazel.md           # ✚ 新增
  │   ├── xmake.md           # ✚ 新增
  │   └── README.md
  ├── 01_compiler/           # 编译器
  │   ├── gcc.md
  │   ├── clang.md
  │   ├── msvc.md            # ✚ 新增
  │   ├── intel.md           # ✚ 新增
  │   └── README.md
  ├── 02_package/            # 包管理器 (重命名)
  │   ├── vcpkg.md
  │   ├── conan.md           # ✚ 新增
  │   ├── cpm.md             # ✚ 新增
  │   └── README.md
  ├── 03_debug/              # 调试器
  │   ├── gdb.md
  │   ├── lldb.md
  │   ├── windbg.md          # ✚ 新增
  │   ├── rr.md              # ✚ 新增
  │   └── README.md
  ├── 04_format/             # 代码格式化
  │   ├── clang-format.md
  │   ├── astyle.md          # ✚ 新增
  │   └── README.md
  ├── 05_static_analysis/    # 静态分析 (新目录)
  │   ├── clang-tidy.md
  │   ├── cppcheck.md
  │   ├── pvs-studio.md      # ✚ 新增
  │   ├── coverity.md        # ✚ 新增
  │   └── README.md
  ├── 06_dynamic_analysis/   # 动态分析 (新目录)
  │   ├── valgrind.md
  │   ├── sanitizers.md
  │   ├── gprof.md
  │   ├── perf.md
  │   ├── gperftools.md
  │   └── README.md
  ├── 07_test/               # 测试框架 (✚ 新目录)
  │   ├── gtest.md
  │   ├── catch2.md
  │   ├── doctest.md
  │   └── README.md
  ├── 08_doc/                # 文档生成 (✚ 新目录)
  │   ├── doxygen.md
  │   ├── sphinx.md
  │   └── README.md
  ├── 09_binary/             # 二进制工具 (✚ 新目录)
  │   ├── objdump.md
  │   ├── readelf.md
  │   ├── nm.md
  │   ├── strip.md
  │   └── README.md
  └── 10_llvm/               # LLVM 基础设施
      ├── llvm.md
      └── README.md
```

## 贡献指南

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/new-feature`)
3. 提交更改 (`git commit -m 'Add new feature'`)
4. 推送到分支 (`git push origin feature/new-feature`)
5. 创建 Pull Request

## 许可证

本项目采用 MIT 许可证，详见 [LICENSE](LICENSE) 文件。

## 参考资源

- [cppreference.com](https://en.cppreference.com/) - C/C++ 在线参考
- [Claude Code 文档](https://docs.anthropic.com/claude-code) - Claude Code 官方文档
- [MCP 协议](https://modelcontextprotocol.io/) - Model Context Protocol