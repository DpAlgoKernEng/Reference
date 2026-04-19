# C++ 静态分析工具

本目录收录 C++ 静态代码分析工具文档。

## 静态分析工具概览

| 工具 | 类型 | 说明 |
|------|------|------|
| [clang-tidy](clang-tidy.md) | 开源 | LLVM 静态分析器，检查全面 |
| [cppcheck](cppcheck.md) | 开源 | C/C++ 专用静态分析，误报率低 |
| [PVS-Studio](pvs-studio.md) | 商业 | 企业级静态分析，检测能力强 |
| [Coverity](coverity.md) | 商业 | 业界标杆，大型项目首选 |

## 工具对比

| 特性 | clang-tidy | cppcheck | PVS-Studio | Coverity |
|------|------------|----------|------------|----------|
| 开源 | ✅ | ✅ | ❌ | ❌ |
| 免费使用 | ✅ | ✅ | 有限免费 | 有限免费 |
| 检查规则数 | 500+ | 200+ | 1000+ | 2000+ |
| 自定义规则 | ✅ | ❌ | ✅ | ✅ |
| CI/CD 集成 | ✅ | ✅ | ✅ | ✅ |
| IDE 插件 | ✅ | ✅ | ✅ | 部分 |

## 可检测问题类型

| 问题类型 | clang-tidy | cppcheck | PVS-Studio |
|----------|------------|----------|------------|
| 内存泄漏 | ✅ | ✅ | ✅ |
| 空指针解引用 | ✅ | ✅ | ✅ |
| 缓冲区溢出 | ✅ | ✅ | ✅ |
| 未初始化变量 | ✅ | ✅ | ✅ |
| 死代码 | ✅ | ✅ | ✅ |
| 代码风格 | ✅ | ❌ | 部分 |
| 安全漏洞 | ✅ | ✅ | ✅ |

## 使用建议

| 场景 | 推荐工具 | 理由 |
|------|----------|------|
| 日常开发 | clang-tidy | 集成 Clang，与编译器一致 |
| CI/CD 流水线 | cppcheck | 快速，轻量级 |
| 企业项目 | PVS-Studio | 检测全面，支持丰富 |
| 开源项目 | clang-tidy + cppcheck | 免费且有效 |

## 基本使用

### clang-tidy
```bash
# 检查单个文件
clang-tidy main.c -- -std=c11

# 使用编译数据库
clang-tidy -p build main.c

# 自动修复
clang-tidy -fix main.c
```

### cppcheck
```bash
# 检查文件
cppcheck main.c

# 启用所有检查
cppcheck --enable=all main.c

# 检查整个项目
cppcheck --enable=all src/
```

## 相关文档

- [代码格式化](../04_format/) - clang-format 等
- [动态分析](../06_dynamic_analysis/) - 运行时检测