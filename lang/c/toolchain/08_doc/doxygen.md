# Doxygen - C/C++ 文档生成工具

## 1. 概述

Doxygen 是最流行的 C/C++ 文档生成工具，可从源代码提取注释生成 HTML、PDF 等格式的文档。

### 1.1 核心特性

| 特性 | 说明 |
|------|------|
| 多语言支持 | C、C++、Java、Python、PHP 等 |
| 多输出格式 | HTML、LaTeX、PDF、XML、RTF、Man pages |
| 图形化文档 | 类图、调用图、协作图 |
| 自动提取 | 从源代码自动提取结构信息 |
| 高度可配置 | 200+ 配置选项 |

### 1.2 适用场景

| 场景 | 推荐 |
|------|------|
| C/C++ API 文档 | Doxygen |
| 库文档 | Doxygen |
| 项目文档网站 | Doxygen + Sphinx |
| 快速参考 | Doxygen HTML |
| PDF 文档 | Doxygen LaTeX |

## 2. 安装

### 2.1 Linux 安装

```bash
# Debian/Ubuntu
sudo apt install doxygen

# CentOS/RHEL
sudo yum install doxygen

# Fedora
sudo dnf install doxygen

# Arch Linux
sudo pacman -S doxygen
```

### 2.2 macOS 安装

```bash
# Homebrew
brew install doxygen

# MacPorts
sudo port install doxygen
```

### 2.3 Windows 安装

```bash
# 方式一：下载安装器
# 访问 https://www.doxygen.nl/download.html
# 下载 doxygen-x.x.x-setup.exe

# 方式二：Chocolatey
choco install doxygen.install

# 方式三：vcpkg
vcpkg install doxygen
```

### 2.4 安装 Graphviz（图形支持）

```bash
# Linux
sudo apt install graphviz

# macOS
brew install graphviz

# Windows
# 下载安装器：https://graphviz.org/download/
# 或使用 Chocolatey
choco install graphviz
```

## 3. 基本用法

### 3.1 初始化配置

```bash
# 生成默认配置文件
doxygen -g

# 生成指定名称配置文件
doxygen -g Doxyfile

# 生成精简配置文件（只包含非默认值）
doxygen -g -s Doxyfile
```

### 3.2 生成文档

```bash
# 使用默认配置文件 Doxyfile
doxygen

# 使用指定配置文件
doxygen Doxyfile

# 查看帮助
doxygen --help

# 查看版本
doxygen --version
```

### 3.3 典型工作流程

```bash
# 1. 创建项目目录结构
mkdir -p project/{src,docs,include}

# 2. 生成配置文件
cd project
doxygen -g Doxyfile

# 3. 编辑配置文件
vim Doxyfile
# 修改：PROJECT_NAME、INPUT、OUTPUT_DIRECTORY 等

# 4. 生成文档
doxygen Doxyfile

# 5. 查看生成的文档
open docs/html/index.html
```

## 4. 注释风格

### 4.1 Doxygen 标准风格

```cpp
/**
 * @brief 简要描述
 *
 * 详细描述可以放在这里，支持多行。
 * 可以包含详细的说明、使用场景、注意事项等。
 *
 * @param name 参数说明
 * @param value 参数值说明
 * @return 返回值说明
 *
 * @note 注意事项
 * @warning 警告信息
 * @see 参见其他函数
 *
 * @code
 * // 示例代码
 * int result = add(1, 2);
 * std::cout << result << std::endl;
 * @endcode
 */
int add(int name, int value);
```

### 4.2 Qt 风格

```cpp
/*!
 * \brief 简要描述
 *
 * 详细描述内容。
 *
 * \param name 参数说明
 * \param value 参数值说明
 * \return 返回值说明
 */
int add(int name, int value);
```

### 4.3 JavaDoc 风格

```cpp
/**
 * 简要描述
 *
 * 详细描述内容。
 *
 * @param name 参数说明
 * @param value 参数值说明
 * @return 返回值说明
 */
int add(int name, int value);
```

### 4.4 行内风格

```cpp
/// 简要描述
int add(int name, int value);

/// 简要描述
/// 详细描述可以跟在后面
int subtract(int a, int b);

//! 简要描述（使用感叹号）
int multiply(int a, int b);

//!<
int x;  //!< 成员变量说明（后置注释）
```

### 4.5 风格对比

| 风格 | 语法 | 适用场景 |
|------|------|----------|
| Doxygen 标准 | `/** ... */` + `@tag` | 推荐，功能完整 |
| Qt 风格 | `/*! ... */` + `\tag` | Qt 项目 |
| JavaDoc 风格 | `/** ... */` + `@tag` | 跨语言项目 |
| 行内风格 | `///` 或 `//!` | 简短注释 |

## 5. 文档标签

### 5.1 常用标签

| 标签 | 说明 | 示例 |
|------|------|------|
| `@brief` | 简要描述 | `@brief 计算两数之和` |
| `@param` | 参数说明 | `@param a 第一个数` |
| `@return` | 返回值说明 | `@return 两数之和` |
| `@throws` | 异常说明 | `@throws std::runtime_error` |
| `@see` | 参见链接 | `@see subtract()` |
| `@note` | 注意事项 | `@note 线程安全` |
| `@warning` | 警告 | `@warning 不可重入` |
| `@deprecated` | 已弃用 | `@deprecated 使用 newFunc()` |
| `@since` | 版本信息 | `@since 2.0` |
| `@author` | 作者 | `@author John Doe` |
| `@date` | 日期 | `@date 2024-01-01` |
| `@version` | 版本 | `@version 1.0` |
| `@code/@endcode` | 代码块 | 见示例 |
| `@example` | 示例文件 | `@example test.cpp` |
| `@todo` | 待办事项 | `@todo 优化算法` |
| `@bug` | 已知问题 | `@bug 内存泄漏` |

### 5.2 类和结构体注释

```cpp
/**
 * @brief 日志管理器
 *
 * 提供日志记录和管理功能。支持多级别日志输出、
 * 文件轮转、格式化输出等特性。
 *
 * @note 线程安全
 * @see LoggerConfig
 *
 * @code
 * LoggerConfig config;
 * config.level = LogLevel::DEBUG;
 * Logger logger(config);
 * logger.log(LogLevel::INFO, "Hello");
 * @endcode
 */
class Logger {
public:
    /**
     * @brief 构造函数
     * @param config 配置对象
     * @throws std::invalid_argument 配置无效时抛出
     */
    explicit Logger(const LoggerConfig& config);

    /**
     * @brief 析构函数
     */
    ~Logger();

    /**
     * @brief 记录日志
     * @param level 日志级别
     * @param message 日志消息
     * @param file 源文件名（可选）
     * @param line 行号（可选）
     */
    void log(LogLevel level, const std::string& message,
             const char* file = nullptr, int line = 0);

    /**
     * @brief 设置日志级别
     * @param level 新的日志级别
     */
    void setLevel(LogLevel level);

private:
    LoggerConfig m_config;  //!< 配置对象
    std::ofstream m_file;   //!< 输出文件流
};
```

### 5.3 文件头注释

```cpp
/**
 * @file logger.cpp
 * @brief 日志管理器实现
 * @author John Doe
 * @date 2024-01-01
 * @version 1.0
 *
 * 提供日志记录功能的实现，包括：
 * - 日志格式化输出
 * - 文件轮转支持
 * - 多级别日志管理
 *
 * @see logger.h
 * @see LoggerConfig
 */
```

### 5.4 枚举和宏注释

```cpp
/**
 * @brief 日志级别枚举
 *
 * 定义日志输出的级别，按严重程度递增。
 */
enum class LogLevel {
    DEBUG,  //!< 调试级别，详细诊断信息
    INFO,   //!< 信息级别，常规运行信息
    WARN,   //!< 警告级别，潜在问题提示
    ERROR,  //!< 错误级别，运行时错误
    FATAL   //!< 致命级别，程序无法继续运行
};

/**
 * @def MAX_BUFFER_SIZE
 * @brief 最大缓冲区大小（字节）
 */
#define MAX_BUFFER_SIZE 1024

/**
 * @def LOG_DEBUG(msg)
 * @brief 调试日志宏
 * @param msg 日志消息
 */
#define LOG_DEBUG(msg) \
    Logger::instance().log(LogLevel::DEBUG, msg)
```

### 5.5 命名空间和模块注释

```cpp
/**
 * @brief 工具函数命名空间
 *
 * 包含字符串处理、文件操作等通用工具函数。
 */
namespace utils {

/**
 * @brief 字符串分割
 * @param str 待分割字符串
 * @param delimiter 分隔符
 * @return 分割后的字符串列表
 */
std::vector<std::string> split(const std::string& str, char delimiter);

/**
 * @defgroup StringUtils 字符串工具
 * @brief 字符串处理工具函数集合
 * @{
 */

std::string trim(const std::string& str);
std::string toLower(const std::string& str);

/** @} */ // end of StringUtils

} // namespace utils
```

## 6. 配置文件

### 6.1 基本配置

```
# 项目信息
PROJECT_NAME           = "MyProject"
PROJECT_NUMBER         = "1.0.0"
PROJECT_BRIEF          = "A brief description"
PROJECT_LOGO           = docs/logo.png

# 输入配置
INPUT                  = src/ include/
RECURSIVE              = YES
FILE_PATTERNS          = *.cpp *.h *.hpp *.c
EXCLUDE                = src/test/ src/third_party/
EXCLUDE_PATTERNS       = */test/* */build/*

# 输出配置
OUTPUT_DIRECTORY       = docs/
GENERATE_HTML          = YES
HTML_OUTPUT            = html
GENERATE_LATEX         = NO
GENERATE_XML           = YES

# 提取选项
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = YES
EXTRACT_STATIC         = YES
EXTRACT_LOCAL_CLASSES  = YES

# 图形选项
HAVE_DOT               = YES
CALL_GRAPH             = YES
CALLER_GRAPH           = YES
CLASS_DIAGRAMS         = YES
UML_LOOK               = YES
```

### 6.2 常用配置选项详解

| 选项 | 说明 | 默认值 |
|------|------|--------|
| `PROJECT_NAME` | 项目名称 | "My Project" |
| `PROJECT_NUMBER` | 项目版本 | 空 |
| `INPUT` | 输入目录 | 项目根目录 |
| `RECURSIVE` | 递归搜索 | NO |
| `FILE_PATTERNS` | 文件匹配模式 | *.c *.cpp 等 |
| `EXCLUDE` | 排除目录 | 空 |
| `EXTRACT_ALL` | 提取所有文档 | NO |
| `EXTRACT_PRIVATE` | 提取私有成员 | NO |
| `EXTRACT_STATIC` | 提取静态成员 | NO |
| `GENERATE_HTML` | 生成 HTML | YES |
| `GENERATE_LATEX` | 生成 LaTeX | YES |
| `HAVE_DOT` | 使用 Graphviz | NO |
| `CALL_GRAPH` | 生成调用图 | NO |
| `CLASS_GRAPH` | 生成类图 | YES |
| `UML_LOOK` | UML 样式 | NO |

### 6.3 高级配置

```
# 源码浏览
SOURCE_BROWSER         = YES
INLINE_SOURCES         = YES

# 搜索引擎
SEARCHENGINE           = YES
SERVER_BASED_SEARCH    = NO

# 关系图
COLLABORATION_GRAPH    = YES
GROUP_GRAPHS           = YES
INCLUDE_GRAPH          = YES
INCLUDED_BY_GRAPH      = YES

# HTML 定制
HTML_HEADER            = docs/header.html
HTML_FOOTER            = docs/footer.html
HTML_STYLESHEET        = docs/style.css
HTML_COLORSTYLE_HUE    = 220
HTML_COLORSTYLE_SAT    = 100

# 预处理
ENABLE_PREPROCESSING   = YES
MACRO_EXPANSION        = YES
EXPAND_ONLY_PREDEF     = YES
PREDEFINED             = DEBUG=1

# 排序
SORT_MEMBER_DOCS       = YES
SORT_BRIEF_DOCS        = YES
SORT_MEMBERS_CTORS_1ST = YES
```

## 7. 调用图和类图

### 7.1 启用图形支持

```
# Graphviz 配置
HAVE_DOT               = YES
DOT_PATH               = /usr/bin/dot

# 调用图
CALL_GRAPH             = YES
CALLER_GRAPH           = YES

# 类图
CLASS_DIAGRAMS         = YES
CLASS_GRAPH            = YES
COLLABORATION_GRAPH    = YES

# UML 样式
UML_LOOK               = YES
UML_LIMIT_NUM_FIELDS   = 10

# 其他图
INCLUDE_GRAPH          = YES
INCLUDED_BY_GRAPH      = YES
GROUP_GRAPHS           = YES
```

### 7.2 函数调用图示例

```cpp
/**
 * @brief 主处理函数
 *
 * 处理输入数据并返回结果。
 *
 * @callergraph
 */
void process() {
    loadData();
    validateInput();
    compute();
    saveResult();
}

/**
 * @brief 加载数据
 *
 * @callgraph
 */
void loadData() {
    readConfig();
    openDatabase();
    fetchData();
}
```

### 7.3 类关系图示例

```cpp
/**
 * @brief 形状基类
 */
class Shape {
public:
    virtual double area() const = 0;
    virtual void draw() const = 0;
};

/**
 * @brief 矩形类
 *
 * @inherit Shape
 */
class Rectangle : public Shape {
public:
    double area() const override;
    void draw() const override;
};

/**
 * @brief 圆形类
 *
 * @inherit Shape
 */
class Circle : public Shape {
public:
    double area() const override;
    void draw() const override;
};
```

## 8. 构建系统集成

### 8.1 CMake 集成

```cmake
# 基础集成
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # 设置输出目录
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)

    # 创建自定义目标
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation"
        VERBATIM
    )

    # 添加安装目标
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html
            DESTINATION share/doc/${PROJECT_NAME})
endif()
```

### 8.2 CMake 使用 Doxygen.cmake 模块

```cmake
# 查找 Doxygen 包
find_package(Doxygen
             OPTIONAL_COMPONENTS dot mscgen dia)

if(DOXYGEN_FOUND)
    # 使用 doxygen_add_docs 简化配置
    doxygen_add_docs(doc
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        COMMENT "Generate API documentation"
    )

    # 输出 Graphviz 支持
    if(DOXYGEN_DOT_FOUND)
        message(STATUS "Doxygen: Graphviz support enabled")
    endif()
endif()
```

### 8.3 Makefile 集成

```makefile
# Makefile
.PHONY: doc clean-doc

DOC_DIR = docs
DOXYFILE = Doxyfile

doc:
	doxygen $(DOXYFILE)
	@echo "Documentation generated in $(DOC_DIR)/html/"

clean-doc:
	rm -rf $(DOC_DIR)/html $(DOC_DIR)/latex
```

### 8.4 CI/CD 集成示例

```yaml
# GitHub Actions
name: Documentation

on:
  push:
    branches: [ main ]

jobs:
  build-docs:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install doxygen graphviz

      - name: Generate documentation
        run: doxygen Doxyfile

      - name: Deploy to GitHub Pages
        uses: peaceiris/actions-gh-pages@v3
        with:
          github_token: ${{ secrets.GITHUB_TOKEN }}
          publish_dir: ./docs/html
```

## 9. 与 Sphinx 结合

### 9.1 Doxygen 生成 XML

```
# Doxyfile 配置
GENERATE_XML           = YES
XML_OUTPUT             = xml
XML_PROGRAMLISTING     = YES
```

### 9.2 Sphinx 配置

```python
# conf.py
extensions = ['breathe', 'exhale']

# Breathe 配置
breathe_projects = {
    'myproject': '_build/xml'
}
breathe_default_project = 'myproject'

# Exhale 配置（自动生成 API 文档）
exhale_args = {
    "containmentFolder": "./api",
    "rootFileName": "library_root.rst",
    "doxygenStripFromPath": "..",
    "rootFileTitle": "API Reference",
    "createTreeView": True
}
```

### 9.3 Sphinx 文档结构

```rst
# index.rst
Welcome to My Project
=====================

.. toctree::
   :maxdepth: 2

   intro
   api/library_root
   examples

API Reference
-------------

.. doxygenfunction:: add
   :project: myproject

.. doxygenclass:: Logger
   :project: myproject
   :members:
```

## 10. 输出格式

### 10.1 支持的输出格式

| 格式 | 配置选项 | 用途 |
|------|----------|------|
| HTML | `GENERATE_HTML = YES` | 网页浏览 |
| LaTeX | `GENERATE_LATEX = YES` | PDF 生成 |
| XML | `GENERATE_XML = YES` | Sphinx 集成 |
| RTF | `GENERATE_RTF = YES` | Word 文档 |
| Man pages | `GENERATE_MAN = YES` | Unix 手册 |
| DocBook | `GENERATE_DOCBOOK = YES` | 结构化文档 |

### 10.2 HTML 定制

```
# HTML 定制选项
HTML_HEADER            = docs/header.html
HTML_FOOTER            = docs/footer.html
HTML_STYLESHEET        = docs/doxygen.css
HTML_EXTRA_STYLESHEET  = docs/custom.css
HTML_TIMESTAMP         = YES
HTML_DYNAMIC_SECTIONS  = YES
HTML_INDEX_NUM_ENTRIES = 100

# 主题配置
HTML_COLORSTYLE_HUE    = 220
HTML_COLORSTYLE_SAT    = 100
HTML_COLORSTYLE_GAMMA  = 80
```

### 10.3 生成 PDF

```bash
# 1. 配置 Doxyfile
GENERATE_LATEX         = YES
LATEX_OUTPUT           = latex
PDF_HYPERLINKS         = YES
USE_PDFLATEX           = YES

# 2. 生成 LaTeX
doxygen Doxyfile

# 3. 编译 PDF
cd docs/latex
make
# 或
pdflatex refman.tex
```

## 11. 分组与模块

### 11.1 使用 @defgroup 创建模块

```cpp
/**
 * @defgroup MathUtils 数学工具
 * @brief 数学运算工具函数
 *
 * 提供常用数学运算功能。
 * @{
 */

/**
 * @brief 计算两数之和
 * @param a 第一个数
 * @param b 第二个数
 * @return 两数之和
 */
int add(int a, int b);

/**
 * @brief 计算两数之差
 */
int subtract(int a, int b);

/** @} */ // end of MathUtils

/**
 * @defgroup StringUtils 字符串工具
 * @brief 字符串处理函数
 * @{
 */

std::string trim(const std::string& str);
std::string toUpper(const std::string& str);

/** @} */ // end of StringUtils
```

### 11.2 使用 @ingroup 归类

```cpp
/**
 * @brief 计算乘积
 * @ingroup MathUtils
 */
int multiply(int a, int b);

/**
 * @brief 计算除法
 * @ingroup MathUtils
 */
int divide(int a, int b);
```

### 11.3 模块配置

```
# Doxyfile
GROUP_NESTED_COMPOUNDS = YES
GROUP_GRAPHS           = YES
```

## 12. 最佳实践

### 12.1 注释规范

1. **注释完整性**: 为所有公开 API 添加注释
2. **简要描述**: 使用 `@brief` 提供简洁说明
3. **参数说明**: 所有参数添加 `@param`，包括方向（`[in]`、`[out]`、`[in,out]`）
4. **代码示例**: 使用 `@code/@endcode` 提供使用示例
5. **版本管理**: 使用 `@since` 标记新增功能，`@deprecated` 标记弃用接口

### 12.2 配置建议

| 场景 | 推荐配置 |
|------|----------|
| 开发阶段 | `EXTRACT_ALL = YES` |
| 发布文档 | `EXTRACT_ALL = NO`，依赖注释 |
| 内部文档 | `EXTRACT_PRIVATE = YES` |
| 公开文档 | `EXTRACT_PRIVATE = NO` |
| 调试依赖 | `CALL_GRAPH = YES` |

### 12.3 CI/CD 集成建议

```yaml
# 完整的 CI 流程
- 检查文档覆盖率
- 自动生成文档
- 部署到文档服务器
- 检查链接有效性
```

### 12.4 常见问题排查

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| 文档不完整 | 未启用 `EXTRACT_ALL` | 设置 `EXTRACT_ALL = YES` |
| 图形未生成 | 未安装 Graphviz | 安装并设置 `HAVE_DOT = YES` |
| 中文乱码 | 编码问题 | 设置 `INPUT_ENCODING = UTF-8` |
| 私有成员缺失 | 未提取私有成员 | 设置 `EXTRACT_PRIVATE = YES` |
| 搜索功能失效 | 未启用搜索引擎 | 设置 `SEARCHENGINE = YES` |

### 12.5 文档维护策略

1. **与代码同步更新**: 修改代码时同步更新注释
2. **定期审核**: 检查文档准确性和完整性
3. **版本标记**: 使用 `@since` 和 `@version` 跟踪变更
4. **自动化检查**: CI 中集成文档覆盖率检查
5. **代码审查**: 文档变更纳入 Code Review 流程