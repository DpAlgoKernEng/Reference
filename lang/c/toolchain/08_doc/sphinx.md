# Sphinx - Python 文档生成工具

## 1. 概述与背景

### 1.1 工具定位

Sphinx 是基于 Python 开发的文档生成工具，最初为 Python 官方文档创建，现已发展为支持多语言项目的通用文档系统。通过 Breathe 扩展可与 Doxygen 集成，为 C/C++ 项目生成专业文档。

### 1.2 发展历史

| 年份 | 版本 | 里程碑特性 |
|------|------|-----------|
| 2008 | 0.1 | 项目启动，用于 Python 文档 |
| 2014 | 1.3 | 内置 Markdown 支持 |
| 2020 | 3.0 | Python 3 only，现代化架构 |
| 2024 | 7.0 | 增强的扩展系统 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 多格式输出 | HTML、PDF、ePub、Man pages 等 |
| reStructuredText | 强大的 RST 标记语言 |
| 自动文档 | Python 代码自动提取文档 |
| 扩展生态 | 丰富的第三方扩展 |
| C/C++ 集成 | Breathe 扩展集成 Doxygen |

### 1.4 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| **Sphinx** | 多语言、灵活、主题丰富 | C/C++ 需配置 Breathe |
| Doxygen | C/C++ 内置支持 | 文档编写灵活性低 |
| MkDocs | Markdown 原生 | 自动文档能力弱 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# pip 安装（推荐）
pip install sphinx

# 完整安装（含常用扩展）
pip install sphinx sphinx-rtd-theme breathe sphinx-c-autodoc

# macOS
brew install sphinx-doc

# Ubuntu/Debian
sudo apt-get install python3-sphinx
```

### 2.2 版本管理

```bash
# 查看版本
sphinx-build --version

# 安装特定版本
pip install sphinx==7.2.6

# 创建虚拟环境
python -m venv .venv
source .venv/bin/activate
pip install -r docs/requirements.txt
```

### 2.3 验证安装

```bash
# 验证 Sphinx 安装
sphinx-build --version

# 验证扩展
python -c "import breathe; print(breathe.__version__)"
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 交互式创建项目
sphinx-quickstart docs

# 快速启动选项
sphinx-quickstart docs \
  --project "MyProject" \
  --author "John Doe" \
  --release "1.0.0" \
  --language "zh_CN"
```

### 3.2 项目结构

```
docs/
├── conf.py          # 配置文件
├── index.rst        # 主页面
├── api.rst          # API 文档
├── Makefile         # 构建脚本
├── _static/         # 静态文件
└── _build/          # 输出目录
```

### 3.3 基本命令

```bash
# 构建 HTML 文档
sphinx-build -b html docs/ docs/_build/html

# 使用 Makefile
make html          # 构建 HTML
make latexpdf      # 构建 PDF
make clean         # 清理构建

# 并行构建
sphinx-build -b html -j 4 docs/ docs/_build/html
```

### 3.4 RST 格式基础

```rst
标题层级
========

一级标题用 =，二级用 -，三级用 ~。

段落文本，支持 **粗体**、*斜体*、`代码`。

列表
----

- 无序列表项 1
- 无序列表项 2

1. 有序列表项
2. 有序列表项

代码块
------

.. code-block:: c

   int main() {
       return 0;
   }

表格
----

+------------+------------+
| Header 1   | Header 2   |
+============+============+
| Cell 1     | Cell 2     |
+------------+------------+
```

## 4. 进阶特性

### 4.1 高级配置

```python
# conf.py 配置示例
project = 'MyProject'
author = 'John Doe'
version = '1.0'
language = 'zh_CN'

# 扩展
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.viewcode',
    'sphinx.ext.napoleon',
    'breathe',
]

# 主题
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'navigation_depth': 4,
}

# Breathe 配置（C/C++ 集成）
breathe_projects = {
    'myproject': '_build/doxygen/xml'
}
breathe_default_project = 'myproject'
```

### 4.2 扩展功能

| 扩展 | 功能 | 使用场景 |
|------|------|---------|
| sphinx.ext.autodoc | 自动提取 Python 文档 | Python 项目 |
| sphinx.ext.napoleon | 支持 Google/Numpy 风格注释 | 代码注释 |
| breathe | Doxygen 集成 | C/C++ 文档 |
| recommonmark | Markdown 支持 | 混合文档 |

## 5. C/C++ 集成（Breathe）

### 5.1 Doxygen 配置

```bash
# Doxyfile 配置
GENERATE_XML = YES
XML_OUTPUT = xml
INPUT = src include
RECURSIVE = YES
EXTRACT_ALL = YES
```

### 5.2 Breathe 使用

```rst
API Reference
=============

Classes
-------

.. doxygenclass:: Logger
   :members:
   :undoc-members:

Functions
---------

.. doxygenfunction:: log_init

Files
-----

.. doxygenfile:: logger.h
```

## 6. CMake 集成

```cmake
# 查找 Sphinx
find_package(Sphinx REQUIRED)

# Sphinx 目标
add_custom_target(sphinx-doc
    COMMAND ${SPHINX_EXECUTABLE}
        -b html
        -c ${CMAKE_SOURCE_DIR}/docs
        ${CMAKE_SOURCE_DIR}/docs
        ${CMAKE_SOURCE_DIR}/docs/_build/html
    COMMENT "Building Sphinx documentation"
)

# Doxygen + Sphinx 组合
add_custom_target(doc
    COMMAND doxygen Doxyfile
    COMMAND make html
    COMMENT "Generating full documentation"
)
```

## 7. CI/CD 集成

### 7.1 GitHub Actions

```yaml
name: Documentation
on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v4
    - name: Install dependencies
      run: pip install -r docs/requirements.txt
    - name: Build docs
      run: cd docs && make html
    - name: Deploy
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: docs/_build/html
```

### 7.2 Read the Docs

```yaml
# .readthedocs.yaml
version: 2
build:
  os: ubuntu-22.04
  tools:
    python: "3.11"
sphinx:
  configuration: docs/conf.py
```

## 8. 最佳实践与参考资源

### 8.1 最佳实践

| 实践 | 说明 |
|------|------|
| 合理组织目录 | 按模块分离文档，避免单文件过大 |
| 使用 toctree | 控制文档层级深度（maxdepth: 2-4） |
| 并行构建 | 使用 -j auto 加速构建 |
| CI 缓存 | 缓存 _build/doxygen 目录 |

### 8.2 常见问题

**问题 1：RST 格式错误**

确保列表、代码块的缩进一致（使用空格，不要用 Tab）。

**问题 2：Breathe 找不到 Doxygen XML**

检查 conf.py 中的路径配置，确保 Doxygen 已生成 XML。

### 8.3 参考资源

| 资源 | URL |
|------|-----|
| Sphinx 官方文档 | https://www.sphinx-doc.org |
| Breathe 文档 | https://breathe.readthedocs.io |
| Read the Docs | https://readthedocs.org |
| sphinx-rtd-theme | https://sphinx-rtd-theme.readthedocs.io |