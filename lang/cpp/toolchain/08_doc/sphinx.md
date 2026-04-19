# Sphinx - Python 文档生成工具

## 1. 概述与背景

### 1.1 工具定位

Sphinx 是基于 Python 开发的强大文档生成工具，最初为 Python 官方文档创建，现已发展为支持多语言项目的通用文档系统。其核心优势在于灵活的 reStructuredText 标记语言、丰富的扩展生态，以及与 Read the Docs 平台的深度集成。

Sphinx 采用"文档即代码"理念，支持版本控制、协作编辑、自动化构建，适用于 API 文档、用户手册、技术教程等多种场景。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2008 | 0.1 | Python 文档系统诞生 |
| 2010 | 1.0 | 扩展系统、国际化支持 |
| 2014 | 1.3 | 增强的搜索功能、改进的主题 |
| 2018 | 1.8 | HTML5 输出、性能优化 |
| 2020 | 3.0 | Python 3 优先、类型注解支持 |
| 2022 | 5.0 | 改进的增量构建、性能提升 |
| 2023 | 7.0 | 现代化 HTML 主题、增强的扩展 API |

### 1.3 核心特性

Sphinx 的核心能力包括：

| 特性 | 说明 |
|------|------|
| 多格式输出 | HTML、PDF、ePub、Man pages、纯文本 |
| 自动文档提取 | Python 模块、C/C++ API（通过 Breathe） |
| 交叉引用 | 函数、类、模块、章节间的智能链接 |
| 扩展生态 | 200+ 扩展插件，覆盖各种需求 |
| 国际化 | gettext 支持，100+ 语言翻译 |
| 主题系统 | 内置主题 + 第三方主题（sphinx-rtd-theme 等） |
| 版本管理 | sphinx-multiversion 支持多版本文档 |

### 1.4 适用场景

| 场景 | 推荐 | 说明 |
|------|------|------|
| 纯 C/C++ API 文档 | Doxygen | 内置源码解析，更适合 API 文档 |
| 用户手册 + API 文档 | Sphinx + Breathe | 灵活文档结构 + API 提取 |
| 多语言项目 | Sphinx | 统一文档系统，多语言支持 |
| 现代文档网站 | Sphinx | 丰富主题，扩展性强 |
| Read the Docs 部署 | Sphinx | 原生支持，自动构建 |
| 开源项目文档 | Sphinx | 社区生态成熟，易于贡献 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 | 适用场景 |
|------|------|------|----------|
| Sphinx | 扩展性强、主题丰富、社区活跃 | 学习曲线陡峭 | 综合文档项目 |
| Doxygen | C/C++ 原生支持、自动化程度高 | 用户文档能力弱 | 纯 API 文档 |
| MkDocs | Markdown 原生、配置简单 | 扩展较少 | 轻量级文档 |
| GitBook | 界面美观、协作友好 | 定制性差 | 团队知识库 |
| Docusaurus | React 技术栈、现代化 | 生态较小 | React 项目文档 |

## 2. 安装与配置

### 2.1 多平台安装

#### Linux 安装

```bash
# Debian/Ubuntu
sudo apt-get install python3-sphinx python3-sphinx-rtd-theme

# Fedora/RHEL
sudo dnf install python3-sphinx

# pip 方式（推荐）
pip install --user sphinx sphinx-rtd-theme
```

#### macOS 安装

```bash
# Homebrew
brew install sphinx-doc

# pip 方式
pip3 install sphinx sphinx-rtd-theme
```

#### Windows 安装

```powershell
# pip 方式
pip install sphinx sphinx-rtd-theme

# Chocolatey
choco install sphinx
```

### 2.2 扩展安装

Sphinx 的强大来源于丰富的扩展生态系统：

```bash
# 常用扩展安装
pip install sphinx-rtd-theme      # Read the Docs 主题
pip install breathe               # Doxygen 集成（C/C++ 文档）
pip install sphinx-c-autodoc      # C 语言自动文档
pip install sphinxcontrib-mermaid # Mermaid 图表
pip install sphinx-copybutton     # 代码复制按钮
pip install sphinx-tabs           # 标签页支持
pip install recommonmark          # Markdown 支持
pip install myst-parser           # MyST Markdown 解析器
```

### 2.3 环境配置

#### 虚拟环境配置

```bash
# 创建虚拟环境
python -m venv .venv
source .venv/bin/activate  # Linux/macOS
# .venv\Scripts\activate  # Windows

# 安装依赖
pip install -r docs/requirements.txt
```

requirements.txt 示例：

```text
sphinx>=7.0.0
sphinx-rtd-theme>=2.0.0
breathe>=4.35.0
myst-parser>=2.0.0
sphinx-copybutton>=0.5.0
sphinx-tabs>=3.4.0
```

### 2.4 验证安装

```bash
# 检查 Sphinx 版本
sphinx-build --version

# 检查已安装扩展
python -c "import sphinx; print(sphinx.__version__)"

# 验证主题
python -c "import sphinx_rtd_theme; print('OK')"
```

## 3. 基础使用

### 3.1 快速入门

使用 `sphinx-quickstart` 命令快速创建项目骨架：

```bash
# 创建文档目录并初始化
mkdir docs
cd docs
sphinx-quickstart

# 交互式配置提示
# > Separate source and build directories (y/n) [n]: y
# > Name prefix for templates and static dir [_]: _
# > Project name: MyProject
# > Author name(s): John Doe
# > Project release []: 1.0.0
```

### 3.2 项目结构

标准 Sphinx 项目结构：

```
docs/
├── source/              # 源文件目录
│   ├── conf.py         # 配置文件
│   ├── index.rst       # 主页面
│   ├── api.rst         # API 文档
│   ├── tutorial.rst    # 教程
│   ├── _static/        # 静态资源
│   └── _templates/      # 自定义模板
├── build/               # 输出目录
│   └── html/           # HTML 输出
└── Makefile            # 构建脚本（Unix）
```

简化结构（单目录）：

```
docs/
├── conf.py              # 配置文件
├── index.rst            # 主页面
├── Makefile             # 构建脚本
└── _build/              # 输出目录
```

### 3.3 基本命令

#### 构建命令

```bash
# 构建 HTML 文档
sphinx-build -b html source/ build/html/

# 使用 Makefile（Unix）
make html
make latexpdf
make epub

# 清理重建
make clean && make html

# 增量构建（仅修改部分）
sphinx-build -b html source/ build/html/

# 完全重建
sphinx-build -a -b html source/ build/html/
```

#### 自动化构建

```bash
# 使用 sphinx-autobuild 实时预览
pip install sphinx-autobuild
sphinx-autobuild source/ build/html/

# 浏览器访问 http://127.0.0.1:8000
```

### 3.4 常用操作

#### 添加新页面

1. 创建 `newpage.rst` 文件
2. 在 `index.rst` 的 `toctree` 中添加：

```rst
.. toctree::
   :maxdepth: 2
   :caption: Contents:

   introduction
   tutorial
   api
   newpage
```

#### 更新配置

编辑 `conf.py` 文件：

```python
# 项目信息
project = 'MyProject'
author = 'John Doe'
copyright = '2024, John Doe'
version = '1.0'
release = '1.0.0'

# 扩展配置
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.viewcode',
    'sphinx.ext.napoleon',
    'sphinx.ext.intersphinx',
]

# 主题设置
html_theme = 'sphinx_rtd_theme'
```

## 4. 进阶特性

### 4.1 高级配置

#### 配置文件详解

```python
# conf.py 完整配置示例

# -- 项目信息 -------------------------------------------------
project = 'CArsenal'
author = 'Development Team'
copyright = '2024, CArsenal Team'
version = '1.0'        # 短版本号
release = '1.0.0'      # 完整版本号

# -- 路径配置 -------------------------------------------------
import os
import sys
sys.path.insert(0, os.path.abspath('../../library'))

# -- 扩展配置 -------------------------------------------------
extensions = [
    'sphinx.ext.autodoc',          # 自动文档提取
    'sphinx.ext.viewcode',         # 源码链接
    'sphinx.ext.napoleon',         # Google/Numpy 风格注释
    'sphinx.ext.intersphinx',      # 外部文档链接
    'sphinx.ext.todo',             # TODO 指令
    'sphinx.ext.coverage',         # 文档覆盖率
    'breathe',                     # Doxygen 集成
    'myst_parser',                 # Markdown 支持
]

# -- 主题配置 -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'logo_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': False,
    'collapse_navigation': True,
    'navigation_depth': 4,
    'titles_only': False,
}

# -- Intersphinx 配置 -----------------------------------------
intersphinx_mapping = {
    'python': ('https://docs.python.org/3', None),
    'cpp': ('https://en.cppreference.com/w', None),
}

# -- Breathe 配置（C/C++ 集成）-------------------------------
breathe_projects = {
    'carsenal': '_build/doxygen/xml'
}
breathe_default_project = 'carsenal'
breathe_default_members = ('members', 'protected-members')

# -- HTML 输出配置 --------------------------------------------
html_static_path = ['_static']
html_css_files = ['custom.css']
html_js_files = ['custom.js']

# -- 全局 substitutions ----------------------------------------
rst_epilog = """
.. |version| replace:: 1.0.0
.. |project| replace:: CArsenal
"""

# -- 需要处理的文件模式 ----------------------------------------
source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}
```

### 4.2 RST 格式详解

#### 标题层级

```rst
一级标题
========

二级标题
--------

三级标题
~~~~~~~~

四级标题
^^^^^^^^
```

#### 文本格式

```rst
**粗体文本**
*斜体文本*
`行内代码`
``代码中有空格` `

删除线文本（需要扩展）
```

#### 列表

```rst
无序列表
--------

- 项目 1
- 项目 2
  - 嵌套项目 2.1
  - 嵌套项目 2.2
- 项目 3

有序列表
--------

1. 第一项
2. 第二项
   a. 子项 a
   b. 子项 b
3. 第三项

定义列表
--------

术语
    术语定义

另一个术语
    另一个定义
```

#### 代码块

```rst
.. code-block:: python
   :linenos:
   :emphasize-lines: 3,5

   def hello():
       print("Hello")  # 高亮行
       print("World")
       return True     # 高亮行
```

#### 表格

```rst
网格表格
========

+------------+------------+-----------+
| Header 1   | Header 2   | Header 3  |
+============+============+===========+
| Cell 1     | Cell 2     | Cell 3    |
+------------+------------+-----------+
| Cell 4     | Cell 5     | Cell 6    |
+------------+------------+-----------+

简单表格
========

=====  =====  ======
H1     H2     H3
=====  =====  ======
C1     C2     C3
C4     C5     C6
=====  =====  ======

列表表格
========

.. list-table:: 表格标题
   :header-rows: 1
   :widths: 20 30 50

   * - Header 1
     - Header 2
     - Header 3
   * - Cell 1
     - Cell 2
     - Cell 3
```

### 4.3 C/C++ 集成（Breathe）

#### Doxygen 配置

创建 `Doxyfile`：

```doxygen
# 项目配置
PROJECT_NAME = "CArsenal"
PROJECT_NUMBER = 1.0.0
OUTPUT_DIRECTORY = docs/_build/doxygen

# 输入配置
INPUT = library/ program/
RECURSIVE = YES
FILE_PATTERNS = *.h *.cpp *.c

# 输出配置
GENERATE_XML = YES
XML_OUTPUT = xml
GENERATE_HTML = NO
GENERATE_LATEX = NO

# 提取配置
EXTRACT_ALL = YES
EXTRACT_PRIVATE = YES
EXTRACT_STATIC = YES
```

#### Breathe 指令

```rst
API Reference
=============

Classes
-------

.. doxygenclass:: Logger
   :members:
   :protected-members:
   :undoc-members:
   :membergroups: public-functions,protected-functions

Functions
---------

.. doxygenfunction:: log_message
   :param: const char* message, LogLevel level

.. doxygenfunction:: init_logger
   :param: const char* filename

Enums
-----

.. doxygenenum:: LogLevel

.. doxygenenum:: ErrorCode

Files
-----

.. doxygenfile:: logger.h
   :sections: defines, enums, functions

Typedefs
--------

.. doxygentypedef:: LoggerPtr

.. doxygentypedef:: CallbackFunc
```

## 5. 性能优化

### 5.1 构建优化

#### 增量构建

```bash
# 增量构建（默认行为）
sphinx-build -b html source/ build/html/

# 强制完全重建
sphinx-build -a -b html source/ build/html/

# 并行构建
sphinx-build -j auto -b html source/ build/html/
```

#### 配置优化

```python
# conf.py 性能配置

# 并行构建（多核）
nitpicky = True

# 忽略警告
suppress_warnings = [
    'ref.python',
    'toc.not_readable',
]

# 缓存 intersphinx
intersphinx_cache_limit = 5  # 天

# 排除不需要的文件
exclude_patterns = [
    '_build',
    'Thumbs.db',
    '.DS_Store',
    'drafts/*',
]
```

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| 分离源文件和构建目录 | 提高增量构建效率 |
| 使用 `exclude_patterns` | 排除草稿、临时文件 |
| 启用并行构建 | `sphinx-build -j auto` |
| 缓存 intersphinx | 减少网络请求 |
| 最小化扩展 | 仅启用必要扩展 |
| 使用 `.md` 或 `.rst` | 统一格式，减少解析开销 |

## 6. 问题排查

### 6.1 常见问题

#### 构建错误

**问题 1：找不到模块**

```text
WARNING: autodoc: failed to import module 'mymodule'
```

解决方案：

```python
# conf.py 添加模块路径
import sys
import os
sys.path.insert(0, os.path.abspath('../../src'))
```

**问题 2：交叉引用失败**

```text
WARNING: unknown document: 'api'
```

解决方案：

```rst
# 检查文件名和路径
# 确保文件在 toctree 中
# 使用相对路径 :doc:`../api`
```

**问题 3：主题未找到**

```text
Theme 'sphinx_rtd_theme' not found
```

解决方案：

```bash
pip install sphinx-rtd-theme
```

#### 中文支持问题

```python
# conf.py 中文配置
language = 'zh_CN'
locale_dirs = ['locale/']
gettext_compact = False

# 字体配置
html_theme_options = {
    'analytics_id': '',
    'logo_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': False,
    'collapse_navigation': True,
    'navigation_depth': 4,
    'titles_only': False,
}

# 添加中文字体
html_css_files = [
    'https://fonts.googleapis.com/css2?family=Noto+Sans+SC:wght@400;700&display=swap',
]
```

### 6.2 调试技巧

#### 详细日志

```bash
# 启用详细输出
sphinx-build -v -b html source/ build/html/

# 更详细的调试信息
sphinx-build -vv -b html source/ build/html/
```

#### 检查配置

```bash
# 验证配置文件
python -c "import conf; print(conf.extensions)"

# 检查 intersphinx 链接
sphinx-build -b linkcheck source/ build/html/
```

## 7. 集成实践

### 7.1 CMake 集成

#### 基本 CMake 配置

```cmake
# CMakeLists.txt
find_package(Sphinx REQUIRED)

# Sphinx 目标
add_custom_target(sphinx-doc
    COMMAND ${SPHINX_EXECUTABLE}
        -b html
        -c ${CMAKE_SOURCE_DIR}/docs
        ${CMAKE_SOURCE_DIR}/docs
        ${CMAKE_BINARY_DIR}/docs/html
    COMMENT "Building Sphinx documentation"
)

# 安装文档
install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/html/
    DESTINATION share/doc/${PROJECT_NAME}/html
)
```

#### Doxygen + Sphinx 集成

```cmake
# Find packages
find_package(Doxygen)
find_package(Sphinx)

# Doxygen 目标
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/doxygen)

    add_custom_target(doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
        COMMENT "Generating Doxygen XML"
    )
endif()

# 组合目标
if(Sphinx_FOUND AND DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/docs/doxygen/xml
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        COMMAND ${SPHINX_EXECUTABLE}
            -b html
            -c ${CMAKE_SOURCE_DIR}/docs
            ${CMAKE_SOURCE_DIR}/docs
            ${CMAKE_BINARY_DIR}/docs/html
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
        COMMENT "Generating full documentation"
        DEPENDS doxygen
    )
endif()
```

### 7.2 CI/CD 配置

#### GitHub Actions

```yaml
name: Documentation

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v4

    - name: Set up Python
      uses: actions/setup-python@v5
      with:
        python-version: '3.11'

    - name: Install dependencies
      run: |
        pip install -r docs/requirements.txt

    - name: Build documentation
      run: |
        cd docs
        make html

    - name: Deploy to GitHub Pages
      if: github.event_name == 'push'
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: docs/_build/html
```

### 7.3 实战案例

#### CArsenal 项目文档结构

```
docs/
├── conf.py
├── index.rst
├── architecture/
│   ├── overview.rst
│   ├── modules.rst
│   └── design-decisions.rst
├── library/
│   ├── colorfmt.rst
│   ├── cmdline-parser.rst
│   └── index.rst
├── program/
│   ├── calculator/
│   │   ├── design.rst
│   │   ├── api.rst
│   │   └── usage.rst
│   └── editor/
├── reference/
│   ├── c-language.rst
│   ├── cpp-language.rst
│   └── tools.rst
└── _static/
    └── custom.css
```

#### 文档构建脚本

```bash
#!/bin/bash
# build-docs.sh

set -e

echo "Generating Doxygen XML..."
cd docs
doxygen Doxyfile

echo "Building Sphinx documentation..."
sphinx-build -b html -W source/ _build/html/

echo "Documentation generated at docs/_build/html/"
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| Sphinx 官方文档 | https://www.sphinx-doc.org |
| reStructuredText 入门 | https://docutils.sourceforge.io/rst.html |
| Breathe 文档 | https://breathe.readthedocs.io |
| sphinx-rtd-theme | https://sphinx-rtd-theme.readthedocs.io |
| MyST-Parser | https://myst-parser.readthedocs.io |

### 8.2 学习路径

| 阶段 | 学习内容 | 时间 |
|------|----------|------|
| 入门 | RST 语法、sphinx-quickstart、基本构建 | 2-3 小时 |
| 进阶 | 配置文件、主题、扩展、交叉引用 | 1-2 天 |
| 实战 | C/C++ 集成、CI/CD、多版本文档 | 2-3 天 |
| 高级 | 自定义扩展、性能优化、自定义主题 | 1 周 |

### 8.3 社区资源

| 资源 | 链接 |
|------|------|
| Sphinx Users Group | https://groups.google.com/g/sphinx-users |
| GitHub Issues | https://github.com/sphinx-doc/sphinx/issues |
| Awesome Sphinx | https://github.com/yoloseem/awesome-sphinxdoc |
| Read the Docs | https://readthedocs.org |

---

**相关文档**：
- [Doxygen 文档生成](doxygen.md)
- [CMake 构建](../03_build/cmake.md)
- [项目架构](../../architecture.md)