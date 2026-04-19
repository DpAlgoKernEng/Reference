# Meson - 现代构建系统

## 1. 概述与背景

### 1.1 工具定位

Meson 是一个现代、快速的构建系统，设计理念是提供简洁易读的配置语言和卓越的构建性能。它使用类似 Python 的领域特定语言（DSL）编写构建配置，默认使用 Ninja 作为构建后端，同时支持生成 Visual Studio 和 Xcode 项目文件。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2013 | 0.1 | Jussi Pakkanen 发起项目 |
| 2015 | 0.20 | GNOME 项目开始采用 |
| 2017 | 0.40 | Windows 支持完善 |
| 2018 | 0.45 | Qt 支持增强 |
| 2019 | 0.50 | Rust 语言支持 |
| 2020 | 0.55 | 改进的依赖查找 |
| 2021 | 0.60 | 性能优化和 Windows 改进 |
| 2022 | 0.63 | C++20 模块支持 |
| 2023 | 0.64+ | 持续优化和 bug 修复 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 配置语言 | Python 风格，简洁易读，无复杂语法 |
| 构建后端 | Ninja（默认）、Visual Studio、Xcode |
| 构建速度 | 极快，增量构建效率高 |
| 依赖管理 | 内置 Wrap 系统和 pkg-config 集成 |
| 跨平台 | Linux、macOS、Windows 完整支持 |
| 语言支持 | C、C++、D、Fortran、Java、Rust 等 |
| 测试集成 | 内置测试框架和基准测试 |
| 安装支持 | 自动生成安装目标和 pkg-config 文件 |

### 1.4 适用场景

| 场景 | 推荐度 | 说明 |
|------|--------|------|
| 新建 C/C++ 项目 | 强烈推荐 | 配置简洁，上手快 |
| GNOME 项目 | 强烈推荐 | 官方推荐构建系统 |
| 追求构建速度 | 强烈推荐 | Ninja 后端性能优秀 |
| 跨平台项目 | 推荐 | 良好的平台抽象 |
| 小型到中型项目 | 推荐 | 配置简单直观 |
| 大型遗留项目 | 谨慎 | 需要迁移成本 |
| 企业级生态 | 谨慎 | CMake 生态更成熟 |

### 1.5 对比分析

| 特性 | Meson | CMake | Autotools |
|------|-------|-------|-----------|
| 配置语言 | Python 风格 DSL | 自定义 DSL | Shell + M4 |
| 学习曲线 | 简单 | 中等 | 复杂 |
| 配置可读性 | 优秀 | 良好 | 较差 |
| 构建速度 | 极快 | 快 | 慢 |
| 跨平台 | 优秀 | 优秀 | 一般 |
| IDE 支持 | 良好 | 优秀 | 一般 |
| 社区生态 | 发展中 | 成熟 | 传统项目 |
| 依赖管理 | Wrap + pkg-config | ExternalProject | pkg-config |
| 文档质量 | 优秀 | 优秀 | 一般 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# macOS - 使用 Homebrew
brew install meson ninja

# Ubuntu/Debian - 使用 pip
pip3 install meson ninja

# 或使用系统包管理器
sudo apt install meson ninja-build

# Fedora
sudo dnf install meson ninja-build

# Arch Linux
sudo pacman -S meson ninja

# Windows - 使用 pip
pip install meson ninja

# 或下载 MSI 安装包
# https://github.com/mesonbuild/meson/releases
```

### 2.2 版本管理

```bash
# 查看版本
meson --version

# 使用 Python 虚拟环境管理
python3 -m venv build-env
source build-env/bin/activate  # Linux/macOS
build-env\Scripts\activate      # Windows

pip install meson ninja

# 指定特定版本
pip install meson==0.63.0
```

### 2.3 环境配置

```bash
# 确保 Python 3.6+ 可用
python3 --version

# 设置环境变量（可选）
export MESON_BUILD_DIR=build
export NINJA_STATUS="[%f/%t] "

# 验证依赖
meson --version
ninja --version
pkg-config --version  # 依赖查找需要
```

### 2.4 验证安装

```bash
# 创建测试项目
mkdir meson-test && cd meson-test
cat > meson.build << 'EOF'
project('test', 'c')
executable('hello', 'main.c')
EOF

cat > main.c << 'EOF'
#include <stdio.h>
int main() {
    printf("Meson works!\n");
    return 0;
}
EOF

# 测试构建
meson setup build
meson compile -C build
./build/hello
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 完整工作流程
meson setup builddir        # 配置项目
meson compile -C builddir   # 构建
meson test -C builddir      # 测试
meson install -C builddir   # 安装（可能需要 sudo）
```

### 3.2 项目结构

```
myproject/
├── meson.build           # 根配置文件
├── meson_options.txt     # 项目选项（可选）
├── subprojects/          # 子项目依赖（可选）
│   └── wrap files/
├── src/
│   ├── meson.build       # 子目录配置
│   ├── main.c
│   └── util.c
├── include/
│   └── myproject.h
├── tests/
│   ├── meson.build
│   └── test_main.c
└── build/               # 构建目录（自动生成）
```

### 3.3 基本命令

```bash
# setup - 配置项目
meson setup builddir                    # 基本配置
meson setup builddir --buildtype=release  # 指定构建类型
meson setup builddir -Dprefix=/usr/local   # 指定安装前缀
meson setup builddir --cross-file=cross.txt # 交叉编译

# compile - 编译
meson compile -C builddir               # 构建所有目标
meson compile -C builddir -j4          # 指定并行任务数
meson compile -C builddir --clean      # 清理

# test - 测试
meson test -C builddir                 # 运行所有测试
meson test -C builddir --suite=unit    # 运行指定测试套件
meson test -C builddir -v              # 详细输出

# install - 安装
meson install -C builddir              # 安装到默认位置
meson install -C builddir --destdir=/tmp/install # 指定安装目录

# 其他命令
meson dist -C builddir                 # 创建发布包
meson rewrite -C builddir              # 修改构建配置
```

### 3.4 常用操作

```python
# meson.build 基本结构
project('myproject', 'c', 'cpp',
  version: '1.0.0',
  default_options: [
    'warning_level=2',
    'c_std=c11',
    'cpp_std=c++17',
    'buildtype=release'
  ]
)

# 定义可执行文件
executable('myapp',
  'src/main.c',
  'src/util.c',
  include_directories: include_directories('include'),
  install: true
)

# 定义静态库
lib = static_library('mylib',
  'src/lib.c',
  include_directories: include_directories('include')
)

# 定义共享库
shlib = shared_library('mylib',
  'src/lib.c',
  version: '1.0.0',
  soversion: '1',
  install: true
)

# 添加测试
test_exe = executable('test_main', 'tests/test_main.c')
test('basic_test', test_exe)
```

## 4. 进阶特性

### 4.1 高级配置

```python
# 条件编译
if host_machine.system() == 'linux'
  # Linux 特定配置
  executable('myapp', 'src/main_linux.c')
elif host_machine.system() == 'windows'
  # Windows 特定配置
  executable('myapp', 'src/main_win.c')
endif

# 编译器检测
if cc.has_header('unistd.h')
  config_h.set('HAVE_UNISTD_H', 1)
endif

# 自定义配置头文件
config_h = configuration_data()
config_h.set('VERSION', meson.project_version())
config_h.set_quoted('PACKAGE_NAME', meson.project_name())
configure_file(
  output: 'config.h',
  configuration: config_h
)

# 查找程序
prog = find_program('bash', required: true)

# 运行脚本
result = run_command('scripts/gen_version.sh', check: true)
version = result.stdout().strip()
```

### 4.2 扩展功能

```python
# 子目录管理
subdir('src')
subdir('tests')

# 依赖管理
dep_zlib = dependency('zlib', required: false)
dep_pthread = dependency('threads')

if dep_zlib.found()
  executable('myapp', 'main.c', dependencies: dep_zlib)
endif

# 内置依赖
inc = include_directories('include')

# 自定义目标
custom_target('generate',
  output: 'generated.c',
  command: [prog_python, 'scripts/gen.py', '@OUTPUT@'],
  capture: true
)

# 子项目（Wrap 系统）
libfoo = subproject('libfoo').get_variable('lib')

# pkg-config 生成
pkg = import('pkgconfig')
pkg.generate(libraries: shlib,
  version: '1.0.0',
  name: 'mylib',
  description: 'My Library'
)
```

### 4.3 插件生态

```python
# 使用内置模块

# pkgconfig 模块
pkg = import('pkgconfig')
pkg.generate(lib)

# gnome 模块（GNOME 项目）
gnome = import('gnome')
gnome.gdbus_codegen('dbus-proxy',
  sources: 'org.example.Service.xml',
  interface_prefix: 'org.example.',
  namespace: 'Example'
)

# i18n 模块（国际化）
i18n = import('i18n')
i18n.gettext('myapp',
  args: '--keyword=_'
)

# Python 模块
py = import('python').find_installation('python3')

# Rust 模块
rust = import('rust')
```

## 5. 性能优化

### 5.1 调优策略

```python
# 构建优化配置
project('myproject', 'c',
  default_options: [
    'buildtype=release',
    'optimization=3',
    'lto=true',           # 链接时优化
    'b_lto=true',
    'b_ndebug=true',
    'strip=true'
  ]
)

# 并行构建
# Ninja 自动并行，也可手动指定
# meson compile -C build -j8

# 预编译头
pch_h = files('include/pch.h')
executable('myapp', 'main.c',
  c_pch: pch_h
)
```

### 5.2 最佳实践

| 实践 | 说明 |
|------|------|
| 构建目录分离 | 使用单独构建目录，保持源码目录清洁 |
| 增量构建 | Meson 自动处理依赖关系，增量构建高效 |
| 选项默认值 | 在 project() 中设置合理的 default_options |
| 错误处理 | 使用 required: false 处理可选依赖 |
| 版本控制 | 使用 meson_options.txt 管理用户选项 |
| 测试集成 | 将测试集成到构建系统 |

```python
# meson_options.txt 示例
option('enable_ssl', type: 'boolean', value: true,
  description: 'Enable SSL support')
option('log_level', type: 'combo', choices: ['debug', 'info', 'warn'],
  value: 'info', description: 'Logging level')
option('max_connections', type: 'integer', value: 100,
  description: 'Maximum concurrent connections')
```

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| meson: command not found | 未安装或未添加到 PATH | 重新安装或检查 PATH |
| ninja: error: loading 'build.ninja' | 未正确配置 | 先执行 meson setup |
| dependency not found | 缺少系统依赖 | 安装对应开发包 |
| permission denied | 安装权限不足 | 使用 sudo 或修改前缀 |
| Unknown option | 选项不存在 | 检查 meson_options.txt |

```bash
# 调试配置
meson setup builddir --backend=ninja -Dlog_level=debug

# 查看配置选项
meson configure builddir

# 修改配置选项
meson configure builddir -Dbuildtype=debug

# 重新配置
meson setup --reconfigure builddir
```

### 6.2 调试技巧

```bash
# 查看所有选项
meson configure builddir

# 查看特定选项
meson configure builddir | grep buildtype

# 打印构建命令
meson compile -C builddir -v

# 查看依赖信息
meson introspect builddir --dependencies

# 查看目标信息
meson introspect builddir --targets

# 查看测试信息
meson introspect builddir --tests

# 详细测试输出
meson test -C builddir -v --timeout-multiplier 0
```

## 7. 集成实践

### 7.1 工具链集成

```python
# 集成 CMake 子项目
cmake = import('cmake')
sub_proj = cmake.subproject('external_lib')
lib = sub_proj.get_variable('lib_target')

# 集成 pkg-config
dep = dependency('libcurl', required: false)

# 交叉编译配置
# cross_file.txt
[binaries]
c = 'arm-linux-gnueabihf-gcc'
ar = 'arm-linux-gnueabihf-ar'
strip = 'arm-linux-gnueabihf-strip'

[host_machine]
system = 'linux'
cpu_family = 'arm'
cpu = 'armv7'
endian = 'little'

# 使用交叉编译文件
# meson setup builddir --cross-file=cross_file.txt
```

### 7.2 CI/CD 配置

```yaml
# GitHub Actions 示例
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        buildtype: [release, debug]
    steps:
      - uses: actions/checkout@v3

      - name: Setup Python
        uses: actions/setup-python@v4
        with:
          python-version: '3.10'

      - name: Install Meson
        run: pip install meson ninja

      - name: Configure
        run: meson setup builddir --buildtype=${{ matrix.buildtype }}

      - name: Build
        run: meson compile -C builddir

      - name: Test
        run: meson test -C builddir -v
```

### 7.3 实战案例

```python
# 完整项目示例：多目标、测试、安装

project('network_lib', 'c', 'cpp',
  version: '1.2.0',
  license: 'MIT',
  default_options: [
    'warning_level=2',
    'c_std=c11',
    'cpp_std=c++17'
  ]
)

# 配置头文件
conf = configuration_data()
conf.set('VERSION', meson.project_version())
conf.set('HAVE_EPOLL', host_machine.system() == 'linux')
conf.set_quoted('PACKAGE_STRING', meson.project_name() + ' ' + meson.project_version())

configure_file(
  output: 'config.h',
  configuration: conf
)

# 构建库
lib_sources = files(
  'src/socket.c',
  'src/connection.c',
  'src/buffer.c'
)

lib = library('network', lib_sources,
  version: '1.2.0',
  soversion: '1',
  install: true,
  include_directories: include_directories('include')
)

# 构建可执行文件
exe = executable('network_test', 'examples/main.c',
  link_with: lib,
  install: true
)

# 测试
test_exe = executable('test_socket', 'tests/test_socket.c', link_with: lib)
test('socket_test', test_exe)

test_exe2 = executable('test_buffer', 'tests/test_buffer.c', link_with: lib)
test('buffer_test', test_exe2)

# pkg-config
pkg = import('pkgconfig')
pkg.generate(lib,
  description: 'A simple network library',
  url: 'https://example.com/network_lib'
)

# 安装头文件
install_headers('include/network.h', subdir: 'network')
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| 官方网站 | https://mesonbuild.com |
| 官方文档 | https://mesonbuild.com/Reference-manual.html |
| 快速教程 | https://mesonbuild.com/Tutorial.html |
| GitHub 仓库 | https://github.com/mesonbuild/meson |

### 8.2 学习路径

```
入门 → 基本项目配置 → 依赖管理 → 测试集成 → CI/CD → 跨平台发布
```

| 阶段 | 内容 | 时间 |
|------|------|------|
| 入门 | 基本配置、构建、安装 | 1-2 天 |
| 进阶 | 依赖管理、子项目、测试 | 3-5 天 |
| 精通 | 跨平台、交叉编译、插件开发 | 1-2 周 |

**推荐项目练手**：
1. 小型 C 工具项目
2. 包含测试的共享库
3. 跨平台 GUI 应用
4. GNOME/GTK 应用