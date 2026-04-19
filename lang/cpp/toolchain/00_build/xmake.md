# xmake - 国产构建工具

## 1. 概述与背景

### 1.1 工具定位

xmake 是一个基于 Lua 的轻量级跨平台构建工具，致力于简化 C/C++ 项目的构建配置过程。它采用简洁的 Lua 语法描述构建规则，内置包管理器 xrepo，支持多平台、多工具链、多语言项目构建。

### 1.2 发展历史

| 年份 | 版本 | 里程碑事件 |
|------|------|-----------|
| 2015 | 1.0 | 项目启动，核心构建框架搭建 |
| 2017 | 2.0 | 引入 xrepo 包管理器 |
| 2019 | 2.2 | 支持多语言（Rust、Go、Dlang） |
| 2021 | 2.5 | 性能优化，支持远程编译 |
| 2023 | 2.8 | 增强插件系统，改进 IDE 集成 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 配置语言 | Lua，简洁易读 |
| 构建速度 | 极快（Ninja 后端，增量编译） |
| 包管理 | 内置 xrepo |
| 跨平台 | Linux/macOS/Windows/Android/iOS |
| 多语言 | C/C++、Rust、Go、Dlang、Swift |
| 中文文档 | 官方完善支持 |

### 1.4 适用场景

| 场景 | 推荐度 |
|------|--------|
| 初学者项目 | ★★★★★ |
| 中文团队 | ★★★★★ |
| 跨平台项目 | ★★★★☆ |
| 中小型项目 | ★★★★☆ |
| 快速原型 | ★★★★★ |

### 1.5 对比分析

| 特性 | xmake | CMake | Meson |
|------|-------|-------|-------|
| 配置语言 | Lua | 自定义 DSL | Python DSL |
| 学习曲线 | 简单 | 中等 | 中等 |
| 包管理 | 内置 | 需外部 | 需外部 |
| 中文支持 | 完善官方 | 社区翻译 | 有限 |
| 构建速度 | 极快 | 中等 | 快 |

## 2. 安装与配置

### 2.1 多平台安装

```bash
# macOS
brew install xmake

# Ubuntu/Debian
sudo add-apt-repository ppa:xmake-io/xmake
sudo apt update && sudo apt install xmake

# Windows
winget install xmake
```

### 2.2 版本管理

```bash
xmake --version        # 查看版本
xmake update           # 更新
xmake update v2.8.3    # 更新到指定版本
```

### 2.3 环境配置

```bash
# ~/.bashrc 或 ~/.zshrc
export XMAKE_GLOBALDIR=~/.xmake/global
export XMAKE_PKG_INSTALLDIR=~/.xmake/packages
```

### 2.4 验证安装

```bash
xmake --version
xmake create -P test_project && cd test_project
xmake && xmake run
```

## 3. 基础使用

### 3.1 快速入门

**创建项目：**

```bash
xmake create -l c++ -P myproject
```

**最简配置 xmake.lua：**

```lua
set_project("myproject")
set_version("1.0.0")

target("myproject")
    set_kind("binary")
    add_files("src/*.cpp")
```

**构建运行：**

```bash
xmake        # 构建
xmake run    # 运行
```

### 3.2 项目结构

```
project/
├── xmake.lua
├── src/
│   └── main.cpp
├── include/
├── tests/
└── .xmake/
```

### 3.3 基本命令

```bash
# 配置
xmake f -p macosx -a x86_64 -m release

# 构建
xmake -j4           # 4 并行构建
xmake --rebuild     # 完全重新构建

# 运行
xmake run myapp
xmake run myapp --help

# 清理
xmake clean
xmake f -c

# 安装打包
xmake install -o /usr/local
xmake package
```

### 3.4 常用操作

**添加依赖：**

```lua
add_requires("boost", "nlohmann_json", "fmt")

target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("boost", "nlohmann_json", "fmt")
```

**多目标配置：**

```lua
target("mylib")
    set_kind("static")
    add_files("lib/*.cpp")

target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_deps("mylib")
```

**条件配置：**

```lua
if is_plat("windows") then
    add_links("ws2_32")
elseif is_plat("linux") then
    add_links("pthread", "dl")
end

if is_mode("release") then
    add_cxxflags("-O3")
end
```

## 4. 进阶特性

### 4.1 高级配置

**编译模式：**

```lua
add_rules("mode.debug", "mode.release")

if is_mode("release") then
    add_cxxflags("-O3", "-flto")
    set_symbols("hidden")
    set_strip("all")
end

if is_mode("debug") then
    add_cxxflags("-g", "-ggdb")
    set_symbols("debug")
end
```

**自定义工具链：**

```lua
toolchain("myclang")
    set_kind("standalone")
    set_toolset("cxx", "clang++")
    on_load(function (toolchain)
        toolchain:add("cxflags", "-std=c++20")
    end)
toolchain_end()
```

### 4.2 扩展功能

**预编译头与 Unity Build：**

```lua
target("myapp")
    set_pcxxheader("src/pch.h")
    add_rules("c++.unity_build")
```

### 4.3 插件生态

```bash
xmake project -k vsxmake          # VS 工程
xmake project -k compile_commands # compile_commands.json
xmake format                      # clang-format
xmake check                       # clang-tidy
```

## 5. 性能优化

### 5.1 调优策略

```bash
xmake -j$(nproc)    # 并行编译
```

```lua
set_policy("build.ccache", true)  # 编译缓存
```

### 5.2 最佳实践

```lua
set_project("myproject")
set_version("1.0.0")
set_languages("c++20")

add_rules("mode.debug", "mode.release")
add_requires("boost", "fmt")

includes("lib")
includes("app")
includes("tests")
```

## 6. 问题排查

### 6.1 常见问题

**找不到包：**

```bash
xrepo update-repo
xrepo search <package>
xrepo install <package>
```

**编译错误：**

```bash
xmake f -c                    # 重置配置
xmake f -p linux -m release   # 重新配置
xmake -v                      # 详细输出
```

**链接错误：**

```lua
target("myapp")
    add_links("m", "pthread", "dl")
```

### 6.2 调试技巧

```bash
xmake f --show           # 显示配置
xmake show -l targets    # 显示目标
xmake show -l commands   # 显示编译命令
xmake -n                 # 干运行
```

## 7. 集成实践

### 7.1 工具链集成

**vcpkg 集成：**

```lua
add_requires("vcpkg::fmt", "vcpkg::spdlog")

target("myapp")
    add_packages("vcpkg::fmt", "vcpkg::spdlog")
```

### 7.2 CI/CD 配置

**GitHub Actions：**

```yaml
steps:
  - uses: actions/checkout@v3
  - name: Install xmake
    run: curl -fsSL https://xmake.io/shget.text | bash
  - name: Build
    run: |
      xmake f -y -m release
      xmake -j$(nproc)
      xmake run test
```

### 7.3 实战案例

**跨平台库项目：**

```lua
target("mylib_headers")
    set_kind("headeronly")
    add_includedirs("include", {public = true})

target("mylib")
    set_kind("static")
    add_deps("mylib_headers")
    add_files("src/*.cpp")

target("myapp")
    set_kind("binary")
    add_deps("mylib")
    add_files("app/*.cpp")
```

**GUI 应用：**

```lua
add_requires("qt")

target("mygui")
    set_kind("binary")
    if is_plat("windows") then
        add_rules("qt.application")
        add_files("src/*.cpp", "src/*.ui", "src/*.qrc")
    else
        add_files("src/*.cpp")
        add_packages("qt")
    end
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| 官方网站 | https://xmake.io |
| 中文文档 | https://xmake.io/#/zh-cn |
| GitHub | https://github.com/xmake-io/xmake |
| 包仓库 | https://xrepo.xmake.io |

### 8.2 学习路径

**入门：** 安装验证 → 创建项目 → 基本配置 → 基本命令 → 包管理

**进阶：** 多目标配置 → 条件配置 → 依赖管理 → 自定义规则 → 插件使用

**高级：** 构建生命周期 → 性能优化 → CI/CD 集成 → 插件开发

### 8.3 社区资源

- GitHub Discussions: https://github.com/xmake-io/xmake/discussions
- 问题反馈: https://github.com/xmake-io/xmake/issues
- 示例项目: https://github.com/xmake-io/xmake/tree/master/tests/projects

---

**总结：** xmake 以简洁的 Lua 配置、内置包管理器和出色的构建性能，为 C/C++ 开发者提供现代化的构建方案。完善的中文文档和活跃社区使其成为值得尝试的构建工具。