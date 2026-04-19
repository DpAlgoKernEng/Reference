# Doxygen - C/C++ 文档生成工具

## 1. 概述

Doxygen 是最流行的 C/C++ 文档生成工具，可从源代码提取注释生成 HTML、PDF 等格式的文档。

### 1.1 核心特性

| 特性 | 说明 |
|------|------|
| 多语言支持 | C/C++、Java、Python、PHP、Objective-C 等 |
| 多输出格式 | HTML、LaTeX、PDF、RTF、XML、Man pages |
| 自动化提取 | 从源代码注释自动生成文档结构 |
| 图形化支持 | 调用图、类图、协作图、继承图 |
| 交叉引用 | 源代码与文档双向链接 |

### 1.2 应用场景

- **库和框架开发**: 生成 API 参考文档
- **大型项目**: 维护代码文档一致性
- **开源项目**: 自动化文档生成和发布
- **企业应用**: 代码文档标准化管理

## 2. 安装配置

### 2.1 安装 Doxygen

```bash
# Linux (Debian/Ubuntu)
sudo apt update
sudo apt install doxygen

# Linux (CentOS/RHEL)
sudo yum install doxygen

# Linux (Arch)
sudo pacman -S doxygen

# macOS
brew install doxygen

# Windows
# 方法1: 下载安装器
# https://www.doxygen.nl/download.html

# 方法2: 使用 Chocolatey
choco install doxygen.install

# 验证安装
doxygen --version
```

### 2.2 安装 Graphviz（用于调用图）

```bash
# Linux (Debian/Ubuntu)
sudo apt install graphviz

# macOS
brew install graphviz

# Windows
choco install graphviz

# 验证安装
dot -V
```

### 2.3 可选依赖

```bash
# LaTeX 支持（生成 PDF）
sudo apt install texlive-full  # Linux
brew install texlive          # macOS

# MathJax 支持（数学公式）
# 在 Doxyfile 中配置
USE_MATHJAX = YES
```

## 3. 基本用法

### 3.1 快速开始

```bash
# 生成配置文件
doxygen -g Doxyfile

# 编辑配置文件（修改项目名、输入目录等）
vim Doxyfile

# 生成文档
doxygen Doxyfile

# 查看生成的文档
# HTML: docs/html/index.html
# LaTeX: docs/latex/refman.tex
```

### 3.2 配置文件模板

```bash
# 生成精简配置（只包含常用选项）
doxygen -g -s Doxyfile

# 从现有配置生成
doxygen -u Doxyfile  # 更新配置文件
```

### 3.3 命令行选项

| 选项 | 说明 |
|------|------|
| `-g <file>` | 生成配置文件 |
| `-s` | 生成精简配置 |
| `-u <file>` | 更新配置文件模板 |
| `-d <level>` | 设置调试级别 |
| `-w <format>` | 生成布局文件 |

## 4. 注释风格详解

### 4.1 Doxygen 标准风格

```cpp
/**
 * @brief 简要描述
 *
 * 详细描述可以放在这里，支持多行。
 * 可以包含复杂的说明、算法描述等。
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
 * \param name 参数说明
 * \return 返回值说明
 */
int add(int name, int value);
```

### 4.3 JavaDoc 风格

```cpp
/**
 * 简要描述
 *
 * @param name 参数说明
 * @return 返回值说明
 */
int add(int name, int value);
```

### 4.4 行内风格

```cpp
/// 简要描述
int add(int name, int value);

//! 简要描述
int subtract(int a, int b);

///@{
/// 一组函数的注释块
void func1();
void func2();
///@}
```

### 4.5 成员后置注释

```cpp
class Logger {
private:
    std::string name;  //!< 日志器名称
    LogLevel level;    //!< 日志级别
    int maxSize;       //!< 最大日志大小 (MB)
};
```

## 5. 文档标签详解

### 5.1 常用标签分类

**描述类标签**

| 标签 | 说明 | 示例 |
|------|------|------|
| `@brief` | 简要描述 | `@brief 初始化日志系统` |
| `@details` | 详细描述 | `@details 实现细节说明` |
| `@file` | 文件说明 | `@file logger.cpp` |
| `@author` | 作者信息 | `@author John Doe` |
| `@date` | 日期 | `@date 2024-01-01` |
| `@version` | 版本 | `@version 1.0` |

**参数和返回标签**

| 根签 | 说明 | 示例 |
|------|------|------|
| `@param` | 参数说明 | `@param name 日志名称` |
| `@param[in]` | 输入参数 | `@param[in] data 输入数据` |
| `@param[out]` | 输出参数 | `@param[out] result 输出结果` |
| `@param[in,out]` | 输入输出 | `@param[in,out] buffer 缓冲区` |
| `@return` | 返回值说明 | `@return 成功返回 true` |
| `@throws` | 异常说明 | `@throws std::runtime_error` |

**警告和提示标签**

| 标签 | 说明 | 示例 |
|------|------|------|
| `@note` | 注意事项 | `@note 线程安全` |
| `@warning` | 警告信息 | `@warning 不可重入` |
| `@see` | 参见链接 | `@see Logger::init()` |
| `@deprecated` | 已弃用 | `@deprecated 使用 newFunc()` |
| `@since` | 版本信息 | `@since version 2.0` |
| `@todo` | 待办事项 | `@todo 添加错误处理` |
| `@bug` | 已知问题 | `@bug 内存泄漏问题` |

### 5.2 类和结构体注释

```cpp
/**
 * @brief 日志管理器
 *
 * 提供日志记录和管理功能，支持多级别日志输出。
 * 
 * @section logger_usage 使用方法
 * @code
 * Logger logger(config);
 * logger.log(LogLevel::INFO, "Application started");
 * @endcode
 *
 * @see LoggerConfig
 * @see LogLevel
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
     * @brief 记录日志
     * @param[in] level 日志级别
     * @param[in] message 日志消息
     * @return 成功返回 true，失败返回 false
     *
     * @note 线程安全函数
     * @warning 调用前必须调用 init()
     */
    bool log(LogLevel level, const std::string& message);
    
    /**
     * @brief 设置日志级别
     * @param level 最小日志级别
     */
    void setLevel(LogLevel level);
    
private:
    LoggerConfig m_config;  //!< 配置对象
    std::mutex m_mutex;     //!< 线程安全锁
};
```

### 5.3 文件头注释模板

```cpp
/**
 * @file logger.cpp
 * @brief 日志管理器实现
 * @author John Doe
 * @date 2024-01-01
 * @version 1.0
 *
 * @details
 * 实现日志管理器的核心功能，包括：
 * - 多级别日志输出
 * - 文件轮转
 * - 线程安全
 *
 * @copyright Copyright (c) 2024
 */
```

### 5.4 枚举和宏定义

```cpp
/**
 * @brief 日志级别枚举
 */
enum class LogLevel {
    DEBUG = 0,  //!< 调试级别
    INFO = 1,   //!< 信息级别
    WARN = 2,   //!< 警告级别
    ERROR = 3,  //!< 错误级别
    FATAL = 4   //!< 致命错误级别
};

/**
 * @def MAX_BUFFER_SIZE
 * @brief 最大缓冲区大小
 */
#define MAX_BUFFER_SIZE 1024

/**
 * @def LOG_INFO(msg)
 * @brief 信息日志宏
 * @param msg 日志消息
 */
#define LOG_INFO(msg) \
    Logger::instance().log(LogLevel::INFO, msg)
```

## 6. 配置文件详解

### 6.1 项目基本配置

```
# 项目信息
PROJECT_NAME = "MyProject"
PROJECT_NUMBER = "1.0.0"
PROJECT_BRIEF = "A brief description"
PROJECT_LOGO = docs/logo.png

# 输入配置
INPUT = src/ include/
INPUT_ENCODING = UTF-8
RECURSIVE = YES
FILE_PATTERNS = *.cpp *.h *.hpp *.c

# 排除配置
EXCLUDE = build/ test/
EXCLUDE_PATTERNS = */test/* */build/*
```

### 6.2 提取选项

```
# 提取所有文档（即使没有注释）
EXTRACT_ALL = YES

# 提取私有成员
EXTRACT_PRIVATE = YES

# 提取静态成员
EXTRACT_STATIC = YES

# 提取局部类
EXTRACT_LOCAL_CLASSES = YES

# 显示命名空间
SHOW_NAMESPACES = YES

# 显示文件
SHOW_FILES = YES

# 显示包含文件
SHOW_INCLUDE_FILES = YES
```

### 6.3 输出格式配置

```
# HTML 输出
GENERATE_HTML = YES
HTML_OUTPUT = html
HTML_FILE_EXTENSION = .html
HTML_HEADER = docs/header.html
HTML_FOOTER = docs/footer.html
HTML_STYLESHEET = docs/style.css

# LaTeX 输出
GENERATE_LATEX = NO
LATEX_OUTPUT = latex

# XML 输出（用于 Sphinx 集成）
GENERATE_XML = YES
XML_OUTPUT = xml

# Man pages
GENERATE_MAN = NO
```

### 6.4 图形配置

```
# Graphviz 设置
HAVE_DOT = YES
DOT_PATH = /usr/bin/dot

# 调用图
CALL_GRAPH = YES
CALLER_GRAPH = YES

# 类图
CLASS_DIAGRAMS = YES
CLASS_GRAPH = YES

# 协作图
COLLABORATION_GRAPH = YES

# UML 样式
UML_LOOK = YES
UML_LIMIT_NUM_FIELDS = 10

# 包含图
INCLUDE_GRAPH = YES
INCLUDED_BY_GRAPH = YES
```

### 6.5 源码浏览配置

```
# 源码浏览
SOURCE_BROWSER = YES
INLINE_SOURCES = YES
STRIP_CODE_COMMENTS = NO
REFERENCED_BY_RELATION = YES
REFERENCES_RELATION = YES
```

## 7. 高级功能

### 7.1 调用图和类图

在代码中启用图形：

```cpp
/**
 * @brief 主处理函数
 *
 * 处理数据并调用子函数完成工作流。
 *
 * @callergraph
 */
void process();

/**
 * @brief 辅助函数
 *
 * 被主处理函数调用，执行具体任务。
 *
 * @callgraph
 */
void helper();
```

### 7.2 分组和模块

```cpp
/**
 * @defgroup logging 日志模块
 * @brief 日志相关功能
 * @{
 */

/**
 * @brief 日志级别
 */
enum class LogLevel { ... };

/**
 * @brief 日志管理器
 */
class Logger { ... };

/** @} */ // end of logging group

/**
 * @defgroup config 配置模块
 * @{
 */
class Config { ... };
/** @} */
```

### 7.3 页面和子页面

```cpp
/**
 * @page mainpage 项目主页
 *
 * @section intro 介绍
 * 项目介绍内容...
 *
 * @section features 特性
 * - 特性 1
 * - 特性 2
 *
 * @subsection feature1 特性 1 详情
 * 详细说明...
 */

/**
 * @page example_page 示例页面
 * 示例内容...
 */
```

## 8. 项目集成

### 8.1 CMake 集成

```cmake
# 查找 Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # 设置输出目录
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
    
    # 创建输出目录
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
    
    # 添加文档目标
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    
    # 添加到构建目标
    add_custom_command(TARGET doc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Documentation generated at ${DOXYGEN_OUTPUT_DIR}/html"
    )
    
    message(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE}")
else()
    message(WARNING "Doxygen not found - documentation target not available")
endif()
```

### 8.2 Makefile 集成

```makefile
# Makefile
DOC_DIR = docs
DOXYFILE = Doxyfile

.PHONY: doc doc-clean

doc:
	doxygen $(DOXYFILE)
	@echo "Documentation generated in $(DOC_DIR)/html/"

doc-clean:
	rm -rf $(DOC_DIR)/html $(DOC_DIR)/latex $(DOC_DIR)/xml
```

### 8.3 CI/CD 集成（GitHub Actions）

```yaml
# .github/workflows/docs.yml
name: Documentation

on:
  push:
    branches: [ main ]

jobs:
  docs:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    
    - name: Install Doxygen
      run: sudo apt-get install doxygen graphviz
    
    - name: Generate Documentation
      run: doxygen Doxyfile
    
    - name: Deploy to GitHub Pages
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: ./docs/html
```

### 8.4 与 Sphinx 结合

**步骤 1: 配置 Doxygen 生成 XML**

```
# Doxyfile
GENERATE_XML = YES
XML_OUTPUT = xml
```

**步骤 2: 安装 Breathe**

```bash
pip install breathe sphinx
```

**步骤 3: 配置 Sphinx**

```python
# conf.py
extensions = ['breathe']

breathe_projects = {
    'myproject': 'docs/doxygen/xml'
}
breathe_default_project = 'myproject'
```

**步骤 4: 在 RST 中使用**

```rst
.. doxygenfunction:: Logger::log
   :project: myproject

.. doxygenclass:: Logger
   :project: myproject
   :members:
```

## 9. 最佳实践

### 9.1 注释规范

1. **为所有公开 API 添加注释**: 确保用户可见的接口都有完整文档
2. **简要描述必须**: 使用 `@brief` 提供简洁说明（一行内）
3. **参数说明完整**: 所有参数添加 `@param`，注明输入输出方向
4. **返回值明确**: 使用 `@return` 说明返回值含义
5. **异常文档化**: 使用 `@throws` 说明可能抛出的异常

### 9.2 代码示例

```cpp
/**
 * @brief 打开日志文件
 *
 * 打开指定路径的日志文件用于写入。
 *
 * @param[in] filePath 日志文件路径
 * @param[in] append 是否追加模式打开
 * @return 成功返回 true，失败返回 false
 *
 * @throws std::invalid_argument 文件路径为空
 * @throws std::runtime_error 无法打开文件
 *
 * @note 文件不存在时会自动创建
 * @warning 不支持网络路径
 *
 * @code
 * Logger logger;
 * if (logger.open("/var/log/app.log", true)) {
 *     logger.log(LogLevel::INFO, "Log opened");
 * }
 * @endcode
 *
 * @see close()
 * @see log()
 */
bool open(const std::string& filePath, bool append = false);
```

### 9.3 版本管理

```cpp
/**
 * @brief 初始化日志系统
 * @since version 1.0.0
 */
void init();

/**
 * @brief 旧版日志函数
 * @deprecated 使用 log(LogLevel, const std::string&) 代替
 * @param msg 日志消息
 */
void log(const std::string& msg);
```

### 9.4 文档维护

- **与代码同步**: 文档注释与代码实现同步更新
- **CI 集成**: 在 CI 中自动生成文档
- **定期审查**: 检查文档完整性和准确性
- **示例代码**: 保持示例代码可运行

## 10. 常见问题

### 10.1 中文乱码

```
# Doxyfile 配置
INPUT_ENCODING = UTF-8
```

### 10.2 图形不显示

```bash
# 安装 Graphviz
sudo apt install graphviz

# Doxyfile 配置
HAVE_DOT = YES
```

### 10.3 私有成员不显示

```
# Doxyfile 配置
EXTRACT_PRIVATE = YES
```

### 10.4 命名空间显示

```
# Doxyfile 配置
SHOW_NAMESPACES = YES
```

### 10.5 源码浏览

```
# Doxyfile 配置
SOURCE_BROWSER = YES
INLINE_SOURCES = YES
```

## 11. 参考资源

- [Doxygen 官方文档](https://www.doxygen.nl/manual/)
- [Doxygen Markdown 支持](https://www.doxygen.nl/manual/markdown.html)
- [Graphviz 官网](https://graphviz.org/)
- [Breathe 文档](https://breathe.readthedocs.io/)