# Meson - 现代构建系统

## 1. 概述与背景

### 1.1 工具定位

Meson 是一个现代、快速的构建系统，使用 Python 风格的配置语言，专注于开发体验和构建速度。它旨在提供一种简洁、易读的方式来定义构建过程，同时保持极高的构建效率。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2013 | 0.1 | Jussi Pakkanen 创建项目 |
| 2015 | 0.20 | 添加 Wrap 依赖管理系统 |
| 2017 | 0.40 | 支持更多语言和平台 |
| 2019 | 0.50 | 改进跨平台支持，增强 IDE 集成 |
| 2021 | 0.60 | 改进 CMake 集成和性能优化 |
| 2023 | 1.0 | 稳定版本发布，完善的文档体系 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 配置语言 | Python 风格，简洁易读，无自定义 DSL |
| 构建后端 | Ninja（默认）、Visual Studio、Xcode |
| 构建速度 | 极快，增量构建优化出色 |
| 依赖管理 | 内置 Wrap 系统，支持子项目 |
| 跨平台 | Linux、macOS、Windows、BSD |
| 语言支持 | C、C++、D、Fortran、Java、Rust 等 |
| 测试集成 | 内置测试框架支持 |

### 1.4 适用场景

| 场景 | 推荐 | 理由 |
|------|------|------|
| 新项目 | Meson | 简洁高效，学习成本低 |
| GNOME 项目 | Meson | 官方推荐构建系统 |
| 追求构建速度 | Meson | Ninja 后端，增量构建极快 |
| 大型跨平台项目 | CMake | 生态成熟，IDE 支持更完善 |
| 嵌入式开发 | CMake | 工具链支持更广泛 |

### 1.5 对比分析

| 特性 | Meson | CMake | Ninja |
|------|-------|-------|-------|
| 配置语言 | Python 风格 | 自定义 DSL | 无（构建文件） |
| 学习曲线 | 简单 | 中等 | 不适用 |
| 构建速度 | 快 | 中等 | 极快 |
| 生态成熟度 | 发展中 | 成熟 | 专注构建 |
| IDE 支持 | 良好 | 优秀 | 通过生成器 |
| 依赖管理 | 内置 Wrap | ExternalProject | 无 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS:**
```bash
# 使用 Homebrew
brew install meson

# 使用 pip
pip3 install meson
```

**Linux (Ubuntu/Debian):**
```bash
# 使用 pip
pip3 install meson

# 使用包管理器
sudo apt install meson python3 python3-pip

# 从源码安装
pip3 install --user meson ninja
```

**Windows:**
```bash
# 使用 pip
pip install meson

# 使用 Chocolatey
choco install meson

# 使用 MSYS2
pacman -S mingw-w64-x86_64-meson
```

### 2.2 版本管理

```bash
# 检查版本
meson --version

# 安装特定版本
pip3 install meson==1.2.3

# 升级到最新版本
pip3 install --upgrade meson
```

### 2.3 环境配置

Meson 依赖 Ninja 构建工具，需要确保两者都正确安装：

```bash
# 安装 Ninja
# macOS
brew install ninja

# Linux
sudo apt install ninja-build

# Windows
choco install ninja

# 验证安装
ninja --version
meson --version
```

### 2.4 验证安装

```bash
# 验证 Meson 和 Ninja
meson --version
ninja --version

# 创建测试项目
mkdir meson-test && cd meson-test
cat > meson.build << 'EOF'
project('test', 'cpp')
executable('hello', 'main.cpp')
EOF

cat > main.cpp << 'EOF'
#include <iostream>
int main() { std::cout << "Hello Meson!\n"; return 0; }
EOF

# 构建测试
meson setup build
meson compile -C build
./build/hello
```

## 3. 基础使用

### 3.1 快速入门

**项目结构：**
```
project/
├── meson.build          # 主构建文件
├── meson_options.txt    # 可选配置选项
├── src/
│   ├── meson.build      # 子目录构建文件
│   └── main.cpp
├── include/
│   └── myapp.hpp
├── tests/
│   ├── meson.build
│   └── test_main.cpp
└── build/               # 构建输出目录
```

### 3.2 基本 meson.build 示例

```python
project('myapp', 'cpp',
  version: '1.0.0',
  license: 'MIT',
  default_options: [
    'warning_level=2',
    'cpp_std=c++17',
    'buildtype=release'
  ]
)

# 添加编译器警告
add_global_arguments('-Wall', '-Wextra', language: 'cpp')

# 创建可执行文件
executable('myapp',
  'src/main.cpp',
  install: true
)

# 添加测试
subdir('tests')
```

### 3.3 基本命令

```bash
# 配置项目（推荐方式）
meson setup build

# 配置项目（旧方式，仍支持）
meson build

# 构建项目
meson compile -C build

# 或使用 Ninja 直接构建
ninja -C build

# 安装
meson install -C build

# 运行测试
meson test -C build

# 清理构建目录
meson compile --clean -C build

# 重新配置
meson configure -C build
```

### 3.4 常用操作

**添加子目录：**
```python
# 根目录 meson.build
project('myproject', 'cpp')
subdir('src')
subdir('lib')

# src/meson.build
src_files = files('main.cpp', 'utils.cpp')
executable('myapp', src_files)
```

**查找依赖：**
```python
# 必需依赖
dep_threads = dependency('threads')

# 可选依赖
dep_boost = dependency('boost', required: false)

if dep_boost.found()
  message('Boost found: ' + dep_boost.version())
  executable('myapp', 'main.cpp', dependencies: dep_boost)
else
  warning('Boost not found, using fallback')
endif
```

**添加库：**
```python
# 创建静态库
mylib = static_library('mylib', 'lib.cpp')

# 创建共享库
mylib_shared = shared_library('mylib_shared', 'lib.cpp')

# 链接库到可执行文件
executable('myapp', 'main.cpp', link_with: mylib)
```

## 4. 进阶特性

### 4.1 高级配置

**自定义选项（meson_options.txt）：**
```ini
option('enable_ssl', type: 'boolean', value: false, description: 'Enable SSL support')
option('log_level', type: 'combo', choices: ['debug', 'info', 'warning', 'error'], value: 'info')
option('max_connections', type: 'integer', value: 100)
option('prefix', type: 'string', value: '/usr/local')
```

**使用选项：**
```python
project('myapp', 'cpp')

if get_option('enable_ssl')
  dep_ssl = dependency('openssl')
  add_global_arguments('-DENABLE_SSL', language: 'cpp')
endif

log_level = get_option('log_level')
config_data = configuration_data()
config_data.set('LOG_LEVEL', '"' + log_level + '"')
config_file = configure_file(
  input: 'config.h.in',
  output: 'config.h',
  configuration: config_data
)
```

### 4.2 扩展功能

**生成配置头文件：**
```python
# config.h.in 内容
# define VERSION "@VERSION@"
# define LOG_LEVEL "@LOG_LEVEL@"

# meson.build
config_data = configuration_data()
config_data.set('VERSION', meson.project_version())
config_data.set('LOG_LEVEL', get_option('log_level'))

config_h = configure_file(
  input: 'config.h.in',
  output: 'config.h',
  configuration: config_data
)
```

**自定义目标：**
```python
# 自定义命令
custom_target('generate_docs',
  input: 'src/main.cpp',
  output: 'docs.html',
  command: [find_program('doxygen'), 'Doxyfile'],
  build_by_default: true
)
```

### 4.3 Wrap 依赖管理

Meson 内置 Wrap 系统用于管理子项目依赖：

**subprojects/zlib.wrap:**
```ini
[wrap-file]
directory = zlib-1.2.13
source_url = https://zlib.net/zlib-1.2.13.tar.gz
source_filename = zlib-1.2.13.tar.gz
source_hash = abc123...

[provide]
zlib = zlib_dep
```

**使用 Wrap 依赖：**
```python
# 自动从 wrap 文件获取依赖
zlib_dep = dependency('zlib', fallback: ['zlib', 'zlib_dep'])

executable('myapp',
  'main.cpp',
  dependencies: zlib_dep
)
```

## 5. 性能优化

### 5.1 调优策略

**构建类型配置：**
```bash
# 设置构建类型
meson configure -C build -Dbuildtype=release

# 可选值：debug, debugoptimized, release, minsize
```

**编译优化选项：**
```python
project('myapp', 'cpp',
  default_options: [
    'buildtype=release',
    'optimization=3',
    'debug=false',
    'lto=true'  # 链接时优化
  ]
)

# 或在构建后修改
meson configure -C build -Doptimization=3 -Dlto=true
```

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| 使用 build 目录分离 | 永远在单独目录构建，保持源码清洁 |
| 增量构建 | Meson 自动处理增量构建，避免 clean |
| Unity 构建 | 小项目可启用 unity 构建加速 |
| 缓存优化 | 使用 ccache 加速编译 |

```python
# 启用 Unity 构建（合并编译单元）
project('myapp', 'cpp',
  default_options: ['unity=on']
)
```

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| 依赖未找到 | 未安装或路径错误 | 检查安装，设置 PKG_CONFIG_PATH |
| 权限错误 | 安装目录权限不足 | 使用 sudo 或指定 --prefix |
| 编译错误 | 编译器版本不支持 | 检查编译器版本，调整 std 标准 |
| 链接错误 | 库路径未设置 | 使用 -Dlibdir 或 link_with |

### 6.2 调试技巧

```bash
# 查看配置信息
meson configure -C build

# 查看所有依赖
meson introspect -C build --dependencies

# 查看构建目标
meson introspect -C build --targets

# 查看安装路径
meson introspect -C build --install-plan

# 详细输出
meson setup build --verbose
ninja -C build -v
```

## 7. 集成实践

### 7.1 工具链集成

**与 CMake 项目集成：**
```python
# 使用 CMake 子项目
cmake = import('cmake')
sub_proj = cmake.subproject('cmake_project')
cmake_lib = sub_proj.get_variable('cmake_lib')
executable('myapp', 'main.cpp', link_with: cmake_lib)
```

**与 pkg-config 集成：**
```python
# 自动使用 pkg-config 查找依赖
dep = dependency('gtk+-3.0', version: '>=3.20')
executable('myapp', 'main.cpp', dependencies: dep)
```

### 7.2 CI/CD 配置

**GitHub Actions 示例：**
```yaml
name: Build
on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies
        run: |
          pip3 install meson ninja
          sudo apt install -y libboost-dev
      
      - name: Setup
        run: meson setup build
      
      - name: Build
        run: meson compile -C build
      
      - name: Test
        run: meson test -C build --print-errorlogs
```

### 7.3 实战案例

**完整 C++ 项目示例：**
```python
# meson.build - 完整项目配置
project('network_lib', 'cpp',
  version: '2.0.0',
  license: 'Apache-2.0',
  default_options: [
    'cpp_std=c++17',
    'warning_level=3',
    'werror=true'
  ]
)

# 检查编译器
if not meson.get_compiler('cpp').has_header('thread')
  error('Threading support required')
endif

# 依赖
deps = []
deps += dependency('threads')
deps += dependency('openssl', required: get_option('ssl'))

# 源文件
sources = files(
  'src/main.cpp',
  'src/network.cpp',
  'src/protocol.cpp'
)

headers = files(
  'include/network.hpp',
  'include/protocol.hpp'
)

# 库目标
mylib = library('network',
  sources,
  install: true,
  include_directories: include_directories('include'),
  dependencies: deps
)

# 可执行文件
executable('server',
  'src/server_main.cpp',
  link_with: mylib,
  dependencies: deps
)

# 测试
if get_option('tests')
  subdir('tests')
endif

# 安装配置
install_headers(headers, subdir: 'network')
pkgconfig = import('pkgconfig')
pkgconfig.generate(mylib, description: 'Network library')
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| 官方网站 | https://mesonbuild.com |
| 官方文档 | https://mesonbuild.com/Reference-manual.html |
| 教程 | https://mesonbuild.com/Tutorial.html |
| GitHub | https://github.com/mesonbuild/meson |

### 8.2 学习路径

| 阶段 | 内容 |
|------|------|
| 入门 | 基本命令、项目结构、简单配置 |
| 进阶 | 依赖管理、自定义选项、测试配置 |
| 高级 | Wrap 系统、跨平台配置、CI/CD 集成 |
| 精通 | 自定义模块、构建优化、大规模项目管理 |