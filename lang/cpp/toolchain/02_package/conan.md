# Conan - 跨平台 C/C++ 包管理器

## 1. 概述与背景

### 1.1 工具定位

Conan 是一个现代、开源、跨平台的 C/C++ 包管理器，由 JFrog 开发并维护。它解决了 C/C++ 生态系统中长期存在的依赖管理问题，提供了一套完整的包管理解决方案。

**核心定位**：
- **包管理器**：管理 C/C++ 库的依赖关系、版本控制和二进制分发
- **跨平台**：支持 Windows、Linux、macOS 等主流操作系统
- **构建系统无关**：可与 CMake、Meson、MSBuild、Makefiles 等多种构建系统集成
- **二进制管理**：支持预编译二进制包的缓存和分发

### 1.2 发展历史

| 年份 | 版本 | 里程碑事件 |
|------|------|-----------|
| 2015 | 0.1 | 项目启动，发布首个版本 |
| 2017 | 1.0 | 发布 1.0 正式版，确立核心架构 |
| 2019 | 1.18 | 引入 Conan Center 中央仓库 |
| 2022 | 2.0 | 架构重构，改进依赖解析和构建系统集成 |
| 2023 | 2.1 | 增强 CMake 工具链支持，优化性能 |
| 2024 | 2.3+ | 持续优化，扩展生态系统 |

### 1.3 核心特性

**依赖管理**：
- 声明式依赖声明（conanfile.txt / conanfile.py）
- 自动依赖解析和传递依赖处理
- 版本范围支持和冲突检测
- 可选依赖和条件依赖

**二进制管理**：
- 预编译二进制包缓存
- 按编译器、架构、构建类型组织二进制
- 本地缓存和远程仓库同步
- 二进制兼容性检查

**构建系统集成**：
- CMake（CMakeDeps、CMakeToolchain）
- Meson、MSBuild、Bazel
- 自定义生成器支持
- 工具链文件生成

**企业级特性**：
- 私有仓库支持
- 权限和访问控制
- 二进制制品管理
- CI/CD 集成优化

### 1.4 适用场景

| 场景 | 说明 |
|------|------|
| 跨平台项目开发 | 需要 Windows/Linux/macOS 多平台支持 |
| 大型 C++ 项目 | 管理复杂的依赖树和版本约束 |
| 企业私有仓库 | 需要内部包管理和分发能力 |
| CI/CD 流水线 | 缓存编译产物，加速构建过程 |
| 嵌入式开发 | 交叉编译和目标平台二进制管理 |
| 开源库开发 | 发布包到 Conan Center 或私有仓库 |

### 1.5 对比分析

| 特性 | Conan | vcpkg | CPM | Hunter |
|------|-------|-------|-----|--------|
| 开发者 | JFrog | Microsoft | 个人 | 个人 |
| 配置语言 | Python DSL | JSON | CMake | CMake |
| 二进制缓存 | 原生支持 | 支持 | 不支持 | 支持 |
| 私有仓库 | 简单配置 | 需额外配置 | 无 | 需配置 |
| 构建系统集成 | 多种 | CMake 为主 | CMake | CMake |
| Windows 支持 | 良好 | 优秀 | 良好 | 良好 |
| 学习曲线 | 中等 | 较低 | 低 | 中等 |
| 社区活跃度 | 高 | 高 | 中 | 低 |

## 2. 安装与配置

### 2.1 多平台安装

**使用 pip 安装（推荐）**：

```bash
# Linux/macOS
pip install conan

# Windows (以管理员身份运行)
pip install conan

# 使用特定版本
pip install conan==2.3.0
```

**使用 Homebrew 安装（macOS）**：

```bash
brew install conan
```

**使用 Docker**：

```bash
# 拉取官方镜像
docker pull conanio/conan2:latest

# 运行容器
docker run -it -v $(pwd):/home/conan/project conanio/conan2:latest
```

### 2.2 版本管理

```bash
# 查看当前版本
conan --version

# 升级到最新版本
pip install --upgrade conan

# 安装特定版本
pip install conan==2.0.0

# 查看安装路径
which conan
```

### 2.3 环境配置

**Conan 2.x 目录结构**：

```
~/.conan2/
├── profiles/          # 配置文件
│   └── default        # 默认配置
├── settings.yml      # 全局设置
├── cache/            # 本地缓存
│   └── p/            # 包缓存
└── data/             # 数据文件
```

**初始化配置**：

```bash
# 初始化默认配置文件
conan profile detect

# 查看当前配置
conan profile show

# 列出所有配置
conan profile list
```

### 2.4 验证安装

```bash
# 版本检查
conan --version

# 配置检查
conan profile detect --force

# 测试远程仓库连接
conan search "*" -r conancenter

# 创建测试项目验证
mkdir test_conan && cd test_conan
conan new cmake_lib -d name=test -d version=1.0
conan create .
```

## 3. 基础使用

### 3.1 快速入门

**创建第一个项目**：

```bash
# 创建项目模板
conan new cmake_lib -d name=mylib -d version=1.0

# 生成的文件结构
# .
# ├── conanfile.py    # 包配方
# ├── src/
# │   └── mylib.cpp   # 源文件
# ├── include/
# │   └── mylib.h     # 头文件
# └── CMakeLists.txt  # 构建配置
```

### 3.2 项目结构

**典型 Conan 项目结构**：

```
myproject/
├── conanfile.txt       # 依赖声明（简单项目）
├── conanfile.py        # 包配方（复杂项目）
├── CMakeLists.txt      # 构建配置
├── src/                # 源代码
│   ├── main.cpp
│   └── lib.cpp
├── include/            # 头文件
│   └── lib.h
├── test/               # 测试代码
│   └── test_main.cpp
└── build/              # 构建目录
    └── generared/      # Conan 生成的文件
```

### 3.3 基本命令

**包搜索与安装**：

```bash
# 搜索包
conan search boost -r conancenter
conan search "boost/*" -r conancenter

# 安装依赖
conan install . --output-folder=build --build=missing

# 查看本地缓存
conan list "boost/*"
conan cache path boost/1.81.0

# 清理缓存
conan remove "boost/*" -c
conan cache clean
```

**包创建与发布**：

```bash
# 创建包（构建 + 导出）
conan create . -pr:b=default -pr:h=default

# 导出包到本地缓存
conan export . mylib/1.0@

# 上传到远程仓库
conan upload mylib/1.0@ -r=myrepo --all
```

### 3.4 常用操作

**conanfile.txt 配置**：

```ini
[requires]
boost/1.81.0
openssl/3.0.0
fmt/10.0.0

[generators]
CMakeDeps        # 生成 FindXXX.cmake
CMakeToolchain   # 生成工具链文件

[options]
boost:shared=True
openssl:shared=True

[layout]
cmake_layout      # 使用 CMake 目录布局
```

**conanfile.py 完整示例**：

```python
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy

class MyProjectConan(ConanFile):
    name = "myproject"
    version = "1.0"
    
    # 基本设置
    settings = "os", "compiler", "build_type", "arch"
    
    # 依赖声明
    requirements = [
        "boost/1.81.0",
        "fmt/10.0.0"
    ]
    
    # 构建选项
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True
    }
    
    # 布局配置
    def layout(self):
        cmake_layout(self)
    
    # 生成构建文件
    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.generate()
    
    # 构建过程
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    # 打包
    def package(self):
        copy(self, "include/*.h", dst=self.package_folder, src=self.source_folder)
        copy(self, "lib/*.a", dst=self.package_folder, src=self.build_folder)
        copy(self, "lib/*.so", dst=self.package_folder, src=self.build_folder)
    
    # 包信息
    def package_info(self):
        self.cpp_info.libs = ["myproject"]
```

## 4. 进阶特性

### 4.1 高级配置

**Profile 配置文件**：

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
tools.build:jobs=8

[options]
*:shared=False
boost:shared=True

[buildenv]
CC=/usr/bin/gcc-11
CXX=/usr/bin/g++-11
```

**使用 Profile**：

```bash
# 指定构建和主机 profile
conan install . -pr:b=default -pr:h=release

# 交叉编译示例（Linux -> ARM）
conan install . -pr:b=default -pr:h=arm_profile
```

### 4.2 扩展功能

**自定义生成器**：

```python
from conan import ConanFile

class MyConan(ConanFile):
    def generate(self):
        # 生成自定义配置文件
        content = f"""
        # Auto-generated by Conan
        BOOST_ROOT = "{self.dependencies['boost'].package_folder}"
        OPENSSL_ROOT = "{self.dependencies['openssl'].package_folder}"
        """
        
        self.save("config.txt", content)
```

**条件依赖**：

```python
from conan import ConanFile
from conan.tools.scm import Version

class MyConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    
    def requirements(self):
        # 条件依赖
        if self.settings.os == "Windows":
            self.requires("winsock/1.0")
        
        # 版本条件依赖
        if Version(self.settings.compiler.version) >= "11":
            self.requires("modernlib/2.0")
        else:
            self.requires("modernlib/1.0")
        
        # 可选依赖
        self.requires("optionallib/1.0", options={"enable": True})
```

### 4.3 插件生态

**常用插件与工具**：

| 工具 | 用途 |
|------|------|
| conan-vs-extension | Visual Studio 集成 |
| conan-clion-plugin | CLion IDE 集成 |
| conan-center-index | Conan Center 包索引 |
| conan-recipes | 社区配方仓库 |
| art_hdr | Conan Server 服务端 |

**VS Code 集成**：

```json
// .vscode/settings.json
{
    "conan.path": "/usr/local/bin/conan",
    "conan.outputLevel": "verbose"
}
```

## 5. 性能优化

### 5.1 调优策略

**缓存优化**：

```bash
# 配置本地缓存大小
conan config set cache:storage.max_size=10G

# 预热缓存（提前下载依赖）
conan install . --lockfile=conan.lock --lockfile-out=conan.lock

# 并行下载
conan install . -j 8
```

**构建优化**：

```bash
# 只构建缺失的包
conan install . --build=missing

# 强制从源码构建特定包
conan install . --build=boost

# 跳过构建（仅使用二进制）
conan install . --build=never
```

### 5.2 最佳实践

**项目配置最佳实践**：

```ini
# conanfile.txt 最佳实践
[requires]
# 使用精确版本，避免语义版本范围
boost/1.81.0

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout

[conf]
# 配置工具链
tools.cmake.cmaketoolchain:system_name=Linux
tools.build:compiler_executables={"c": "/usr/bin/gcc", "cpp": "/usr/bin/g++"}
```

**版本锁定**：

```bash
# 生成锁文件
conan lock create conanfile.py --version=1.0

# 使用锁文件确保可重复构建
conan install . --lockfile=conan.lock
```

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|---------|
| 包找不到 | 远程仓库未配置 | `conan remote add` |
| 二进制不兼容 | 编译器/设置不匹配 | `--build=missing` |
| 依赖冲突 | 版本约束冲突 | 检查 requirements |
| 权限错误 | 缓存目录权限 | 检查目录权限 |
| 网络超时 | 仓库连接问题 | 配置代理或镜像 |

**常见错误处理**：

```bash
# 诊断信息
conan install . -vv  # 详细输出

# 清理缓存重新安装
conan remove "*" -c
conan install . --build=missing

# 检查 profile 配置
conan profile show
conan profile detect --force
```

### 6.2 调试技巧

**日志与诊断**：

```bash
# 详细日志
conan install . -vvv

# 保存调试日志
conan install . -vvv 2>&1 | tee conan_debug.log

# 检查包信息
conan inspect boost/1.81.0

# 查看 package ID
conan list boost/1.81.0 -r conancenter
```

**缓存检查**：

```bash
# 查看缓存路径
conan cache path boost/1.81.0

# 列出缓存内容
conan list "*"

# 清理部分缓存
conan remove "boost/*" --src --builds
```

## 7. 集成实践

### 7.1 工具链集成

**CMake 完整集成示例**：

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(myproject CXX)

# Conan 生成的文件会自动设置依赖路径
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)

add_executable(myapp src/main.cpp)

target_link_libraries(myapp
    PRIVATE
    Boost::headers
    OpenSSL::SSL
    fmt::fmt
)
```

**构建脚本**：

```bash
#!/bin/bash
# build.sh - 完整构建脚本

set -e

# 安装依赖
conan install . \
    --output-folder=build \
    --build=missing \
    -s build_type=Release

# 配置 CMake
cmake -S . -B build \
    -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release

# 构建
cmake --build build -j$(nproc)

# 测试
ctest --test-dir build --output-on-failure
```

### 7.2 CI/CD 配置

**GitHub Actions 配置**：

```yaml
# .github/workflows/build.yml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-22.04, windows-2022, macos-13]
        build_type: [Release, Debug]

    steps:
    - uses: actions/checkout@v4
    
    - name: Set up Python
      uses: actions/setup-python@v5
      with:
        python-version: '3.11'
    
    - name: Install Conan
      run: pip install conan
    
    - name: Configure Conan
      run: conan profile detect --force
    
    - name: Cache Conan packages
      uses: actions/cache@v4
      with:
        path: ~/.conan2/cache
        key: conan-${{ runner.os }}-${{ hashFiles('conanfile.txt') }}
    
    - name: Install dependencies
      run: |
        conan install . \
          --build=missing \
          -s build_type=${{ matrix.build_type }}
    
    - name: Build
      run: |
        cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/generators/conan_toolchain.cmake
        cmake --build build --config ${{ matrix.build_type }}
```

**GitLab CI 配置**：

```yaml
# .gitlab-ci.yml
stages:
  - build
  - test

variables:
  CONAN_USER_HOME: "${CI_PROJECT_DIR}/.conan"

build:linux:
  stage: build
  image: conanio/conan2:latest
  script:
    - conan profile detect --force
    - conan install . --build=missing
    - cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/generators/conan_toolchain.cmake
    - cmake --build build
  artifacts:
    paths:
      - build/
    expire_in: 1 week

test:linux:
  stage: test
  image: conanio/conan2:latest
  needs: [build:linux]
  script:
    - cd build
    - ctest --output-on-failure
```

### 7.3 实战案例

**案例：多平台库开发**

项目结构：

```
mylib/
├── conanfile.py
├── CMakeLists.txt
├── src/
│   └── mylib.cpp
├── include/
│   └── mylib.h
├── test/
│   └── test_mylib.cpp
└── conan/
    ├── profiles/
    │   ├── linux_gcc11
    │   ├── windows_msvc
    │   └── macos_clang
    └── recipes/
        └── conanfile.py
```

完整的 conanfile.py：

```python
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, get
from conan.tools.scm import Version
import os

class MyLibConan(ConanFile):
    name = "mylib"
    version = "1.0.0"
    description = "A cross-platform C++ library"
    license = "MIT"
    author = "Your Name <your@email.com>"
    url = "https://github.com/yourorg/mylib"
    topics = ("library", "cross-platform", "cpp")
    
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_ssl": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "with_ssl": False
    }
    
    def requirements(self):
        self.requires("fmt/10.0.0")
        if self.options.with_ssl:
            self.requires("openssl/3.0.0")
    
    def layout(self):
        cmake_layout(self, src_folder="src")
    
    def validate(self):
        if self.settings.compiler.cppstd:
            if Version(self.settings.compiler.cppstd) < "17":
                raise ConanInvalidConfiguration("mylib requires C++17 or higher")
    
    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["WITH_SSL"] = self.options.with_ssl
        tc.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        # 运行测试
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake.test()
    
    def package(self):
        copy(self, "LICENSE", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)
        copy(self, "*.h", dst=os.path.join(self.package_folder, "include"), src=os.path.join(self.source_folder, "include"))
        copy(self, "*.a", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
        copy(self, "*.so", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
        copy(self, "*.lib", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
        copy(self, "*.dll", dst=os.path.join(self.package_folder, "bin"), src=self.build_folder, keep_path=False)
    
    def package_info(self):
        self.cpp_info.libs = ["mylib"]
        if self.options.with_ssl:
            self.cpp_info.requires = ["openssl::openssl"]
        self.cpp_info.set_property("cmake_file_name", "mylib")
        self.cpp_info.set_property("cmake_target_name", "mylib::mylib")
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| 官方文档 | https://docs.conan.io/ |
| Conan Center | https://conan.io/center/ |
| GitHub 仓库 | https://github.com/conan-io/conan |
| 示例仓库 | https://github.com/conan-io/examples |

### 8.2 学习路径

**入门阶段**：
1. 安装 Conan 并配置环境
2. 学习 conanfile.txt 基本语法
3. 使用现有包管理简单项目
4. 理解 Profile 和配置系统

**进阶阶段**：
1. 编写 conanfile.py 配方
2. 创建和发布自定义包
3. 配置私有仓库
4. CI/CD 集成实践

**高级阶段**：
1. 开发自定义生成器
2. 复杂依赖场景处理
3. 交叉编译配置
4. 企业级部署和管理

**推荐学习资源**：
- Conan 官方教程：https://docs.conan.io/2/tutorial.html
- Conan Center 文档：https://conan.io/center/docs
- JFrog 博客：https://jfrog.com/blog/
- C++ Weekly 频道 Conan 相关视频