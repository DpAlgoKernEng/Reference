# 技术文档中文化项目

将技术文档（C/C++ Reference 等）转换为结构化的中文技术文档。

## 项目简介

本项目旨在将高质量的技术文档（如 cppreference、Linux 文档等）转换为结构化的中文文档，便于中文开发者学习和参考。通过 Claude Code 的自定义技能（Skill）实现自动化文档分析和翻译。

## 核心功能

**cppref-synthesis 技能**：核心技能 `cppref-synthesis` 能够分析技术文档并生成结构化的中文文档，输出 7 个标准章节

## 使用方式

**示例1**：/cppref-synthesis 使用这个技能，对original_docs/c/language/statements/statements.md 分析生成中文文档，输出到对应的docs/c/language/statements/statements.md 中

**示例2**：/cppref-synthesis 使用这个技能，对original_docs/cpp/language/statements/statements.md 分析生成中文文档，输出到对应的docs/cpp/language/statements/statements.md 中

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