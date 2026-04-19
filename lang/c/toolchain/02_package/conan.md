# Conan - 跨平台 C/C++ 包管理器

## 1. 概述与背景

### 1.1 工具定位

Conan 是一个现代、跨平台的 C/C++ 包管理器，由 JFrog 开发并开源。它解决了 C/C++ 生态系统中长期存在的依赖管理难题，为开发者提供了类似 npm（Node.js）、pip（Python）的包管理体验。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 2015 | 0.1 | 首次发布，基础包管理功能 |
| 2017 | 1.0 | 稳定版本，完善生态系统 |
| 2018 | 1.5 | 引入 conan_server，私有仓库支持 |
| 2019 | 1.18 | 增强 CMake 集成 |
| 2022 | 2.0 | 架构重构，新工具链集成，性能优化 |

### 1.3 核心特性

- **跨平台支持**：支持 Windows、Linux、macOS，以及嵌入式和移动平台
- **多构建系统集成**：支持 CMake、Visual Studio、Makefiles、Autotools 等
- **二进制缓存**：预编译二进制包，避免重复构建，加速开发周期
- **灵活的依赖管理**：支持版本范围、传递依赖、冲突检测
- **私有仓库**：轻松搭建私有包仓库，支持企业内部管理
- **Python 编写**：易于扩展和定制，可编写自定义 recipe

### 1.4 适用场景

| 场景 | 描述 |
|------|------|
| 企业开发 | 统一管理内部库，版本控制和分发 |
| 跨平台项目 | 一份配置，多平台构建 |
| 开源项目 | 便捷地发布和分发 C/C++ 库 |
| CI/CD 集成 | 自动化依赖管理和构建流程 |
| 嵌入式开发 | 支持交叉编译和目标平台配置 |

### 1.5 对比分析

| 特性 | Conan | vcpkg | Build2 |
|------|-------|-------|--------|
| 开发者 | JFrog | Microsoft | Build2 |
| 配置语言 | Python | JSON | 自定义语言 |
| 二进制缓存 | ✅ 内置支持 | ✅ 二进制缓存 | ✅ |
| 私有仓库 | ✅ 简单配置 | 需额外配置 | ✅ |
| 构建系统集成 | 多种（CMake/VS/Make等） | 主要 CMake | 自带构建系统 |
| Windows 支持 | ✅ | ✅✅ 优秀 | ✅ |
| 学习曲线 | 中等 | 低 | 高 |
| 社区规模 | 大 | 大 | 中等 |

## 2. 安装与配置

### 2.1 多平台安装

**要求：** Python 3.6+ 或 Python 2.7

```bash
# 使用 pip 安装（推荐）
pip install conan

# 使用 pipx 安装（隔离环境）
pipx install conan

# 使用 Homebrew 安装（macOS）
brew install conan

# 使用 apt 安装（Ubuntu/Debian）
sudo apt-get update
sudo apt-get install -y pip
pip install conan
```

### 2.2 版本管理

```bash
# 查看当前版本
conan --version

# 升级到最新版本
pip install --upgrade conan

# 安装指定版本
pip install conan==2.0.17

# 查看已安装版本
pip show conan
```

### 2.3 环境配置

Conan 2.0 使用新的配置结构，默认配置位于 `~/.conan2/`：

```
~/.conan2/
├── profiles/           # 配置文件
│   ├── default         # 默认配置
│   └── release         # 自定义配置
├── settings.yml        # 全局设置
├── remotes.json        # 远程仓库配置
└── data/               # 本地缓存
    ├── packages/       # 包数据
    └── locks/          # 锁文件
```

**初始化配置：**

```bash
# 自动检测并生成默认 profile
conan profile detect

# 手动创建 profile
conan profile new default --detect
```

### 2.4 验证安装

```bash
# 检查版本
conan --version

# 查看配置
conan profile list
conan profile show default

# 测试远程仓库连接
conan search zlib -r conancenter

# 查看 Conan 信息
conan info
```

## 3. 基础使用

### 3.1 快速入门

**场景：** 使用 zlib 库创建一个简单的压缩程序。

**第一步：** 创建项目结构

```bash
mkdir my_project && cd my_project
```

**第二步：** 创建 `conanfile.txt`

```ini
[requires]
zlib/1.2.13

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

**第三步：** 安装依赖

```bash
conan install . --output-folder=build --build=missing
```

**第四步：** 构建项目

```bash
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build .
```

### 3.2 项目结构

**Conan 1.x 项目结构：**

```
my_project/
├── conanfile.txt       # 简单依赖配置
├── CMakeLists.txt      # 构建配置
└── src/
    └── main.cpp
```

**Conan 2.x 推荐结构：**

```
my_project/
├── conanfile.py        # Python 配置（推荐）
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── test/
│   └── test_main.cpp
└── test_package/       # Conan 测试包
    ├── conanfile.py
    └── example.cpp
```

### 3.3 基本命令

| 命令 | 功能 | 示例 |
|------|------|------|
| `conan search` | 搜索包 | `conan search zlib -r conancenter` |
| `conan install` | 安装依赖 | `conan install . --output-folder=build` |
| `conan create` | 创建包 | `conan create . mylib/1.0@` |
| `conan upload` | 上传包 | `conan upload mylib/1.0@ -r=myrepo` |
| `conan download` | 下载包 | `conan download mylib/1.0@ -r=myrepo` |
| `conan profile` | 管理配置 | `conan profile detect` |
| `conan build` | 构建项目 | `conan build .` |
| `conan export` | 导出包 | `conan export . mylib/1.0@` |

### 3.4 常用操作

**安装指定版本的依赖：**

```bash
conan install zlib/1.2.12@
conan install "zlib/[>=1.2.11 <1.3]"
```

**安装开发依赖：**

```ini
[requires]
zlib/1.2.13

[build_requires]
cmake/3.25.0
ninja/1.11.1

[test_requires]
gtest/1.12.1
```

**安装可选依赖：**

```python
def requirements(self):
    self.requires("zlib/1.2.13")
    self.requires("openssl/3.0.0", options={"shared": True})
```

## 4. 进阶特性

### 4.1 高级配置

**Profile 文件详解：**

```ini
# ~/.conan2/profiles/release
[settings]
os=Linux
arch=x86_64
compiler=gcc
compiler.version=11
compiler.libcxx=libstdc++11
build_type=Release

[conf]
tools.cmake.cmaketoolchain:generator=Ninja
tools.build:jobs=4

[options]
zlib:shared=True
openssl:shared=False

[environment]
CC=gcc-11
CXX=g++-11
```

**使用 Profile：**

```bash
# 使用默认 profile
conan install .

# 指定 build 和 host profile
conan install . -pr:b=default -pr:h=release

# 交叉编译配置
conan install . -pr:h=linux_armv8 -pr:b=default
```

### 4.2 扩展功能

**完整的 conanfile.py 配置：**

```python
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy

class MyProjectConan(ConanFile):
    name = "myproject"
    version = "1.0.0"
    
    # 包的元信息
    description = "A sample C++ project"
    license = "MIT"
    author = "Your Name <your.email@example.com>"
    url = "https://github.com/user/myproject"
    topics = ("cpp", "library", "example")
    
    # 设置
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True
    }
    
    # 依赖
    requirements = [
        "zlib/1.2.13",
        "openssl/3.0.0"
    ]
    
    # 生成器
    generators = "CMakeDeps", "CMakeToolchain"
    
    def layout(self):
        cmake_layout(self)
    
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
    
    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
    
    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["MY_OPTION"] = True
        tc.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    def package(self):
        copy(self, "include/*.h", dst=self.package_folder, src=self.source_folder)
        copy(self, "lib/*.a", dst=self.package_folder, src=self.build_folder)
        copy(self, "lib/*.so", dst=self.package_folder, src=self.build_folder)
    
    def package_info(self):
        self.cpp_info.libs = ["myproject"]
        self.cpp_info.requires = ["zlib::zlib", "openssl::openssl"]
```

### 4.3 私有仓库管理

**配置远程仓库：**

```bash
# 添加远程仓库
conan remote add myrepo https://conan.mycompany.com/artifactory/api/conan/local

# 列出所有远程仓库
conan remote list

# 设置仓库优先级
conan remote update_priority myrepo 1

# 移除仓库
conan remote remove myrepo

# 认证配置
conan user -p mypassword -r myrepo myuser
```

**上传和下载包：**

```bash
# 创建包
conan create . mylib/1.0@

# 上传包到私有仓库
conan upload mylib/1.0@ -r=myrepo --all

# 上传所有匹配的包
conan upload "mylib/*" -r=myrepo

# 从私有仓库下载
conan download mylib/1.0@ -r=myrepo

# 同步包
conan sync mylib/1.0@ -r=myrepo
```

## 5. 性能优化

### 5.1 二进制缓存

Conan 的二进制缓存是其核心性能优化特性。

**缓存配置：**

```bash
# 查看缓存位置
conan cache path

# 清理缓存
conan cache clean

# 清理所有缓存
conan cache clean --all

# 设置缓存策略
conan config set cache.db=db
```

**构建策略：**

```bash
# 从源码构建所有依赖
conan install . --build=*

# 构建缺失的包
conan install . --build=missing

# 构建特定包
conan install . --build=zlib

# 从不构建（只使用二进制）
conan install . --build=never
```

### 5.2 最佳实践

**1. 使用 Build Profile 和 Host Profile 分离：**

```bash
# Conan 2.x 推荐方式
conan install . -pr:b=default -pr:h=release
```

**2. 利用并行构建：**

```python
# conanfile.py
def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build(clients=4)  # 使用 4 个并行任务
```

**3. 缓存二进制包：**

```bash
# 为所有构建配置预生成二进制
conan install . -pr:h=release -pr:b=default
conan install . -pr:h=debug -pr:b=default
conan create . mylib/1.0@
```

**4. 使用本地缓存服务器：**

```bash
# 配置本地缓存
conan remote add local-cache http://localhost:9300

# 设置默认仓库
conan remote update_priority local-cache 1
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：依赖冲突**

```
ERROR: Conflict between openssl/1.1.1 and openssl/3.0.0
```

解决方法：

```bash
# 查看依赖树
conan graph info . --format=html > graph.html

# 强制使用特定版本
conan install . --requires=openssl/3.0.0 --build=missing
```

**问题 2：找不到包**

```
ERROR: Unable to find 'zlib/1.2.13' in remotes
```

解决方法：

```bash
# 检查远程仓库
conan remote list

# 搜索包
conan search zlib -r conancenter --all

# 从源码构建
conan install . --build=missing
```

**问题 3：编译器配置不匹配**

```
ERROR: Invalid setting 'compiler.libcxx=libstdc++'
```

解决方法：

```bash
# 重新检测配置
conan profile detect --force

# 手动编辑 profile
conan profile update settings.compiler.libcxx=libstdc++11 default
```

### 6.2 调试技巧

**启用详细日志：**

```bash
# 调试模式
conan install . -v

# 详细调试
conan install . -vvv

# 保存日志到文件
conan install . -vvv 2>&1 | tee conan.log
```

**检查依赖图：**

```bash
# 生成依赖图
conan graph info .

# 输出为 JSON
conan graph info . --format=json

# 输出为 HTML 可视化
conan graph info . --format=html > deps.html
```

**测试包配置：**

```python
# test_package/conanfile.py
from conan import ConanFile
from conan.tools.build import can_run

class TestPackageConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    test_type = "explicit"
    
    def requirements(self):
        self.requires(self.tested_reference_str)
    
    def test(self):
        if can_run(self):
            self.run("mylib --version", env="conanrun")
```

## 7. 集成实践

### 7.1 工具链集成

**CMake 集成（推荐方式）：**

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(myproject CXX)

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(myapp src/main.cpp)

target_link_libraries(myapp PRIVATE
    ZLIB::ZLIB
    OpenSSL::SSL
    OpenSSL::Crypto
)
```

```bash
# 安装依赖
conan install . --output-folder=build --build=missing

# 构建项目
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build .
```

**Visual Studio 集成：**

```bash
# 生成 Visual Studio 解决方案
conan install . -g VisualStudio

# 或使用 CMake
conan install . -g CMakeToolchain -g CMakeDeps
cmake . -G "Visual Studio 17 2022"
```

### 7.2 CI/CD 配置

**GitHub Actions 集成：**

```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
        build_type: [Release, Debug]
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Conan
        run: pip install conan
      
      - name: Configure Conan
        run: conan profile detect --force
      
      - name: Install dependencies
        run: conan install . --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
      
      - name: Build
        run: |
          cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
          cmake --build build --config ${{ matrix.build_type }}
      
      - name: Test
        run: cd build && ctest --build-config ${{ matrix.build_type }}
```

**GitLab CI 集成：**

```yaml
image: conanio/gcc11

stages:
  - build
  - test

before_script:
  - conan profile detect --force

build:
  stage: build
  script:
    - conan install . --build=missing
    - cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
    - cmake --build build
  artifacts:
    paths:
      - build/

test:
  stage: test
  script:
    - cd build
    - ctest --output-on-failure
  dependencies:
    - build
```

### 7.3 实战案例

**案例 1：多配置项目**

```python
# conanfile.py
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout

class MultiConfigConan(ConanFile):
    name = "multiconfig"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    
    def layout(self):
        cmake_layout(self)
    
    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
```

```bash
# 构建多个配置
conan install . -s build_type=Release
conan install . -s build_type=Debug
conan build . -c tools.cmake.cmaketoolchain:is_multi_config=True
```

**案例 2：企业私有库管理**

```bash
# 设置企业仓库
conan remote add company-repo https://conan.company.com
conan user -p $CONAN_PASSWORD -r company-repo $CONAN_USER

# 创建并上传内部库
cd internal_lib
conan create . companylib/1.0@
conan upload companylib/1.0@ -r=company-repo

# 在项目中使用
echo "[requires]\ncompanylib/1.0" > conanfile.txt
conan install . -r=company-repo
```

## 8. 参考资源

### 8.1 官方文档

- **官方文档：** https://docs.conan.io/
- **Conan Center（公共仓库）：** https://conan.io/center/
- **GitHub 仓库：** https://github.com/conan-io/conan
- **Conan 2.0 迁移指南：** https://docs.conan.io/en/latest/migrating_to_2.0/

### 8.2 学习路径

| 阶段 | 内容 | 参考资源 |
|------|------|----------|
| 入门 | 安装、基本命令、简单项目 | 官方教程 Getting Started |
| 进阶 | Profile 配置、多构建系统 | 官方文档 Build Systems |
| 高级 | 自定义 recipe、私有仓库 | 官方文档 Developing Packages |
| 专家 | 插件开发、企业级配置 | Conan 源码、社区博客 |

**推荐学习顺序：**

1. 安装 Conan 并配置环境
2. 创建简单项目，使用 `conanfile.txt`
3. 学习 Profile 配置和多平台构建
4. 掌握 `conanfile.py` 编写
5. 实践 CI/CD 集成
6. 搭建私有仓库，管理内部库

**社区资源：**

- **Conan Blog：** https://blog.conan.io/
- **Stack Overflow：** 标签 `[conan]`
- **Gitter 社区：** https://gitter.im/conan-io/conan