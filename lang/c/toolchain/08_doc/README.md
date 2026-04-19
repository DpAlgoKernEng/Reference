# C 语言文档生成工具

本目录收录 C 语言文档生成工具文档。

## 文档生成工具概览

| 工具 | 说明 | 输出格式 |
|------|------|----------|
| [Doxygen](doxygen.md) | 标准文档生成器 | HTML/LaTeX/PDF |
| [Sphinx](sphinx.md) | Python 文档工具 | HTML/PDF |

## 工具对比

| 特性 | Doxygen | Sphinx |
|------|---------|--------|
| 语言支持 | C/C++/Java/... | 多语言 (通过扩展) |
| 注释风格 | 专有格式 | reStructuredText |
| 输出格式 | HTML/LaTeX/PDF/... | HTML/PDF/ePub |
| 图表支持 | ✅ (Graphviz) | ✅ |
| 搜索功能 | ✅ | ✅ |
| 主题定制 | ✅ | ✅ (丰富) |

## Doxygen 注释风格

### 文件注释
```c
/**
 * @file example.c
 * @brief 文件简要描述
 * @author 作者名
 * @date 2024-01-01
 */
```

### 函数注释
```c
/**
 * @brief 计算两数之和
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return 两数之和
 */
int add(int a, int b);
```

### 结构体注释
```c
/**
 * @brief 点结构体
 */
struct Point {
    int x;  /**< X 坐标 */
    int y;  /**< Y 坐标 */
};
```

## 使用流程

```
1. 编写代码注释（Doxygen 格式）
2. 生成配置文件：doxygen -g
3. 编辑 Doxyfile 配置
4. 生成文档：doxygen Doxyfile
5. 查看 HTML 文档
```

## Doxyfile 常用配置

```
PROJECT_NAME = "My Project"
INPUT = src include
RECURSIVE = YES
EXTRACT_ALL = YES
GENERATE_HTML = YES
HAVE_DOT = YES
UML_LOOK = YES
```

## 相关文档

- [构建系统](../00_build/) - CMake 文档集成