# C 语言参考文档

C 语言完整参考文档，共 **171** 篇文档，按模块组织。

## 模块结构

| 模块 | 目录 | 文档数 | 说明 |
|------|------|--------|------|
| 语言参考 | `language/` | 105 | 核心语法和语义 |
| 标准版本 | `language_standard/` | 12 | C89/C99/C11/C17/C23 版本特性 |
| 工具链 | `toolchain/` | 54 | 编译、调试、测试工具 |
| 标准库 | `library/` | - | 规划中 |

## 目录结构

```
lang/c/
├── language/              # 语言参考
│   ├── 00_basic_concepts/ # 基础概念
│   ├── 01_keyword/        # 关键字
│   ├── 02_declarations/   # 声明
│   ├── 03_initialization/ # 初始化
│   ├── 04_expressions/    # 表达式
│   ├── 05_statements/     # 语句
│   ├── 06_functions/      # 函数
│   ├── 07_preprocessor/   # 预处理器
│   └── 08_miscellaneous/  # 杂项
├── language_standard/     # 标准版本
│   ├── 00_versions/       # 版本概述
│   └── 01_compiler_support/
└── toolchain/             # 工具链
    ├── 00_build/          # 构建系统
    ├── 01_compiler/       # 编译器
    ├── 02_package/        # 包管理
    ├── 03_debug/          # 调试工具
    ├── 04_format/         # 代码格式化
    ├── 05_static_analysis/
    ├── 06_dynamic_analysis/
    ├── 07_test/           # 测试框架
    ├── 08_doc/            # 文档生成
    ├── 09_binary/         # 二进制工具
    ├── 10_llvm/           # LLVM 工具链
    └── 11_coverage/       # 覆盖率分析
```

## 标准版本

| 版本 | 年份 | 主要特性 |
|------|------|----------|
| C89 | 1989 | ANSI C 第一版 |
| C95 | 1995 | 宽字符支持 |
| C99 | 1999 | 变长数组、inline |
| C11 | 2011 | 多线程、原子操作 |
| C17 | 2017 | C11 修正版 |
| C23 | 2023 | 类型推断、新属性 |