# xmake - 国产跨平台构建工具

## 1. 概述与背景

### 1.1 工具定位

xmake 是一个基于 Lua 的轻量级跨平台构建工具，专注于简化 C/C++ 项目的构建配置过程。它采用"约定优于配置"的设计理念，同时提供灵活的定制能力，旨在成为 CMake 的现代化替代方案。

xmake 的核心设计目标：
- **简洁配置**：使用 Lua 语言，语法直观易懂
- **快速构建**：内置 Ninja 后端，构建速度优秀
- **包管理集成**：内置 xrepo 包管理器，无需额外工具
- **跨平台支持**：原生支持 Linux、macOS、Windows、iOS、Android 等平台

### 1.2 发展历史

| 年份 | 版本 | 里程碑 |
|------|------|--------|
| 2015 | 1.0 | 项目启动，基础构建功能 |
| 2017 | 2.0 | 重构架构，增加包管理 |
| 2019 | 2.2 | 内置 xrepo，支持远程依赖 |
| 2021 | 2.5 | Lua 5.4 升级，性能优化 |
| 2022 | 2.6 | 增加分布式构建支持 |
| 2023 | 2.7 | 改进 C++20 模块支持 |
| 2024 | 2.8 | 增强 Rust/Cuda 混合编译 |

### 1.3 核心特性

| 特性 | 说明 | 优势 |
|------|------|------|
| 配置语言 | Lua 5.4 | 简洁易读，学习成本低 |
| 构建后端 | Ninja/Lua | 编译速度快 |
| 包管理 | xrepo 内置 | 无需额外安装包管理器 |
| 跨平台 | 5+ 平台 | 一份配置多平台构建 |
| 依赖分析 | 增量构建 | 只编译变更文件 |
| 多语言支持 | C/C++/Rust/Cuda | 混合语言项目支持 |
| 中文文档 | 完善 | 国产工具，中文优先 |

### 1.4 适用场景

| 场景 | 推荐程度 | 说明 |
|------|----------|------|
| 新项目起步 | ★★★★★ | 配置简单，快速上手 |
| 中文团队 | ★★★★★ | 原生中文文档和社区 |
| 跨平台项目 | ★★★★☆ | 多平台支持完善 |
| 中小型项目 | ★★★★☆ | 配置维护成本低 |
| 嵌入式开发 | ★★★★☆ | 交叉编译支持良好 |
| 大型企业项目 | ★★★☆☆ | 生态相对年轻 |
| IDE 集成需求 | ★★★☆☆ | IDE 支持持续改进中 |

### 1.5 对比分析

| 特性 | xmake | CMake | Meson |
|------|-------|-------|-------|
| 配置语言 | Lua | 自定义 DSL | Python DSL |
| 学习曲线 | 简单 | 中等 | 中等 |
| 包管理 | 内置 xrepo | 需 vcpkg/Conan | 内置 wrap |
| 中文支持 | 完善 | 社区翻译 | 较少 |
| 构建速度 | 快 | 中等 | 快 |
| IDE 支持 | 良好 | 优秀 | 良好 |
| 社区规模 | 成长中 | 庞大 | 中等 |
| 企业采用 | 新兴 | 主流 | 增长中 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS 安装：**
```bash
# 使用 Homebrew（推荐）
brew install xmake

# 使用官方脚本安装
bash <(curl -fsSL https://xmake.io/shget.text)
```

**Linux 安装：**
```bash
# Ubuntu/Debian
sudo add-apt-repository ppa:xmake-io/xmake
sudo apt update
sudo apt install xmake

# Arch Linux
yay -S xmake

# CentOS/RHEL
sudo yum install xmake

# 通用安装脚本
bash <(curl -fsSL https://xmake.io/shget.text)
```

**Windows 安装：**
```powershell
# 使用 winget
winget install xmake

# 使用 scoop
scoop install xmake

# 使用安装包
# 下载 https://github.com/xmake-io/xmake/releases
```

### 2.2 版本管理

```bash
# 查看当前版本
xmake --version
xmake -v

# 查看详细信息
xmake show -h

# 升级 xmake（官方脚本安装）
xmake update
xmake update dev  # 升级到开发版

# Homebrew 安装的升级
brew upgrade xmake
```

### 2.3 环境配置

xmake 安装后通常无需额外配置，但可以设置环境变量优化体验：

```bash
# 设置全局配置目录（可选）
export XMAKE_GLOBALDIR=~/.xmake

# 设置缓存目录
export XMAKE_PKG_CACHEDIR=~/.xmake/cache

# 设置代理（网络环境需要时）
export XMAKE_PKG_PROXY=http://proxy.example.com:8080

# Bash 补全支持
source <(xmake core bash-completion)
```

### 2.4 验证安装

```bash
# 检查版本
xmake --version

# 输出示例
# xmake v2.8.5+HEAD.0a3d87479, A cross-platform build utility based on Lua
# Copyright (C) 2015-present Ruki Wang, tboox.org, xmake.io

# 测试构建能力
xmake create -t static test_project
cd test_project
xmake
xmake run test
```

## 3. 基础使用

### 3.1 快速入门

创建并构建第一个项目：

```bash
# 创建新项目
xmake create myproject
cd myproject

# 项目自动生成结构
# myproject/
# ├── xmake.lua
# └── src/
#     └── main.c

# 构建项目
xmake

# 运行程序
xmake run myproject
```

### 3.2 项目结构

标准 xmake 项目结构：

```
project/
├── xmake.lua           # 主配置文件
├── src/                # 源代码目录
│   ├── main.c
│   └── utils.c
├── include/            # 头文件目录
│   └── utils.h
├── tests/              # 测试代码
│   └── test_main.c
├── .xmake/             # xmake 工作目录（自动生成）
└── build/              # 构建输出目录（自动生成）
```

多模块项目结构：

```
project/
├── xmake.lua
├── core/
│   └── xmake.lua       # 子模块配置
├── utils/
│   └── xmake.lua
└── app/
    └── xmake.lua
```

### 3.3 基本命令

**项目生命周期命令：**

```bash
# 配置项目
xmake f -p macosx -a arm64 -m release
xmake config --plat=windows --arch=x64 --mode=debug

# 构建项目
xmake                    # 构建所有目标
xmake -j4                # 使用 4 个并行任务
xmake -r                 # 重新构建
xmake -j$(nproc)         # Linux 多核编译

# 运行程序
xmake run myapp          # 运行目标程序
xmake run myapp arg1 arg2  # 带参数运行

# 安装
xmake install             # 安装到默认目录
xmake install -o /usr/local  # 指定安装目录

# 打包
xmake package             # 打包
xmake package -o output.tar.gz
```

**项目管理命令：**

```bash
# 创建项目
xmake create myproject
xmake create -l cpp myproject      # C++ 项目
xmake create -t static mylib       # 静态库项目
xmake create -t shared mylib       # 动态库项目

# 清理
xmake clean              # 清理构建
xmake f -c               # 清理配置

# 查看信息
xmake show -l targets    # 列出所有目标
xmake show -l options    # 列出所有选项
xmake show target:myapp  # 显示目标详情
```

### 3.4 常用操作

**xmake.lua 基础配置：**

```lua
-- 设置项目信息
set_project("myapp")
set_version("1.0.0")
set_description("A sample application")

-- 设置语言标准
set_languages("c11", "c++17")

-- 定义编译选项
add_rules("mode.debug", "mode.release")

-- 定义目标
target("myapp")
    set_kind("binary")
    add_files("src/*.c")
    add_includedirs("include")
    
    -- 链接系统库
    if is_plat("linux") then
        add_links("pthread", "m")
    end
```

**调试与发布配置：**

```lua
-- 模式特定配置
if is_mode("debug") then
    add_cflags("-g", "-O0")
    add_cxxflags("-g", "-O0")
    add_defines("DEBUG")
end

if is_mode("release") then
    add_cflags("-O3")
    add_cxxflags("-O3")
    add_defines("NDEBUG")
end
```

## 4. 进阶特性

### 4.1 高级配置

**依赖管理：**

```lua
-- 添加远程依赖
add_requires("zlib", "openssl", "spdlog")

-- 指定版本
add_requires("zlib ~1.2.11")
add_requires("openssl >=1.1.0")

-- 使用依赖
target("myapp")
    set_kind("binary")
    add_packages("zlib", "openssl")
```

**多目标项目：**

```lua
-- 静态库
target("mylib")
    set_kind("static")
    add_files("lib/*.c")
    add_includedirs("include", {public = true})

-- 动态库
target("mydll")
    set_kind("shared")
    add_files("dll/*.c")
    add_deps("mylib")

-- 可执行文件
target("myapp")
    set_kind("binary")
    add_files("src/*.c")
    add_deps("mylib")
    add_links("pthread")
```

**条件配置：**

```lua
-- 平台特定配置
if is_plat("windows") then
    add_links("ws2_32", "wininet")
    add_defines("WIN32_LEAN_AND_MEAN")
elseif is_plat("linux") then
    add_links("pthread", "dl", "m")
elseif is_plat("macosx") then
    add_links("pthread", "m")
    add_frameworks("Foundation", "CoreFoundation")
end

-- 架构特定配置
if is_arch("x86_64", "arm64") then
    add_defines("64BIT")
end
```

### 4.2 扩展功能

**自定义任务：**

```lua
-- 定义自定义任务
task("generate")
    set_menu {
        usage = "xmake generate [options]",
        options = {
            {nil, "output", "kv", "output.h", "Output file."}
        }
    }
    
    on_run(function()
        import("core.project.config")
        local output = get_config("output")
        -- 执行代码生成逻辑
        print("Generating to: " .. output)
    end)
```

**自定义规则：**

```lua
-- 定义自定义规则
rule("lex")
    set_extensions(".l", ".lex")
    on_build_file(function(target, sourcefile, opt)
        -- 处理 lex 文件
        local outfile = path.join(target:autogendir(), path.basename(sourcefile) .. ".c")
        os.iorunv("flex", {"-o", outfile, sourcefile})
        table.insert(target:objectfiles(), outfile)
    end)
```

### 4.3 插件生态

xmake 支持插件扩展：

```bash
# 安装插件
xmake plugin install xmake-plugin-cmake

# 常用插件
xmake project -k cmakelists   # 生成 CMakeLists.txt
xmake project -k makefile      # 生成 Makefile
xmake project -k compile_commands  # 生成 compile_commands.json
xmake project -k vscode        # 生成 VSCode 配置
xmake project -k clang-tidy    # 生成 clang-tidy 配置
```

IDE 集成生成：

```lua
-- 在 xmake.lua 中配置
add_rules("plugin.compile_commands.autoupdate")
```

## 5. 性能优化

### 5.1 调优策略

**并行构建：**

```bash
# 自动使用所有 CPU 核心
xmake -j$(nproc)       # Linux
xmake -j$(sysctl -n hw.ncpu)  # macOS

# 设置默认并行数
# 在 xmake.lua 中
set_policy("build.across_targets_in_parallel", true)
```

**缓存构建：**

```bash
# 启用 ccache 加速
xmake f --ccache=n

# 配置远程缓存
xmake f --ccache=redis://localhost:6379
```

**增量构建：**

```lua
-- 启用增量编译检查
set_policy("check.auto_ignore_files", true)

-- 禁用不必要的检查加速构建
set_policy("check.auto_detect_files", false)
```

### 5.2 最佳实践

| 实践 | 说明 | 配置示例 |
|------|------|----------|
| 头文件依赖 | 启用自动依赖生成 | `add_rules("c.unity_build")` |
| Unity 构建 | 合并编译单元 | `add_rules("c.unity_build")` |
| PCH 预编译 | 使用预编译头 | `set_pcxxheader("pch.h")` |
| 分布式构建 | 多机编译 | `xmake service` |
| 缓存利用 | 启用编译缓存 | `xmake f --ccache=y` |

**预编译头配置：**

```lua
target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    set_pcxxheader("src/pch.h")  -- C++ 预编译头
```

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| 找不到头文件 | include 路径未配置 | `add_includedirs("include")` |
| 链接错误 | 库路径或名称错误 | 检查 `add_links` 和 `add_linkdirs` |
| 包下载失败 | 网络问题 | 设置代理或使用镜像 |
| 交叉编译失败 | 工具链未配置 | 使用 `xmake f --sdk=...` 指定 |
| 权限错误 | 安装目录权限不足 | 使用 `sudo` 或指定用户目录 |

**常见错误处理：**

```bash
# 清理并重新配置
xmake clean
xmake f -c
xmake f -p macosx -a arm64 -m release
xmake -r

# 详细输出诊断
xmake -vD  # 详细 + 调试信息

# 检查配置
xmake show
xmake show -l options
```

### 6.2 调试技巧

```bash
# 启用调试输出
xmake -v          # 详细模式
xmake -vD         # 详细 + 调试模式
xmake --verbose   # 完整命令输出

# 查看构建命令
xmake show build:myapp

# 检查依赖树
xmake show dependencies

# 导出构建信息
xmake project -k compile_commands
```

**配置调试：**

```lua
-- 在 xmake.lua 中添加调试信息
on_load(function(target)
    print("target: " .. target:name())
    print("kind: " .. target:kind())
    print("files: " .. table.concat(target:sourcefiles(), ", "))
end)
```

## 7. 集成实践

### 7.1 工具链集成

**vcpkg 集成：**

```lua
-- 使用 vcpkg 包
add_requires("vcpkg::zlib", "vcpkg::openssl")

target("myapp")
    add_packages("vcpkg::zlib", "vcpkg::openssl")
```

**Conan 集成：**

```lua
add_requires("conan::zlib/1.2.11", "conan::openssl/1.1.1k")

target("myapp")
    add_packages("conan::zlib", "conan::openssl")
```

**交叉编译配置：**

```bash
# 交叉编译到 ARM Linux
xmake f -p linux -a arm64 --sdk=/path/to/arm-toolchain

# 交叉编译到 Android
xmake f -p android --ndk=/path/to/ndk --api=21

# 交叉编译到 iOS
xmake f -p iphoneos -a arm64
```

### 7.2 CI/CD 配置

**GitHub Actions 集成：**

```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
    
    runs-on: ${{ matrix.os }}
    
    steps:
      - uses: actions/checkout@v4
      
      - name: Install xmake
        uses: xmake-io/github-action-setup-xmake@v1
      
      - name: Configure
        run: xmake f -y
      
      - name: Build
        run: xmake -j$(nproc)
      
      - name: Test
        run: xmake run tests
```

**GitLab CI 集成：**

```yaml
stages:
  - build
  - test

build:
  stage: build
  image: alpine:latest
  before_script:
    - apk add --no-cache xmake
  script:
    - xmake f -y
    - xmake -j$(nproc)
  artifacts:
    paths:
      - build/

test:
  stage: test
  script:
    - xmake run tests
```

### 7.3 实战案例

**多平台库项目：**

```lua
-- xmake.lua - 跨平台库项目
set_project("mylib")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")

-- 库目标
target("mylib")
    set_kind("$(kind)")  -- 支持静态/动态切换
    add_files("src/*.c")
    add_includedirs("include", {public = true})
    
    -- 平台特定配置
    if is_plat("windows") then
        add_defines("MYLIB_EXPORTS")
    end
    
    if is_plat("linux") then
        add_links("m", "pthread")
    end

-- 测试目标
target("test_mylib")
    set_kind("binary")
    add_files("tests/*.c")
    add_deps("mylib")
```

**混合语言项目：**

```lua
-- C/C++/Rust 混合项目
target("myapp")
    set_kind("binary")
    add_files("src/*.cpp")
    
    -- C 部分
    add_files("lib/*.c")
    
    -- Rust 部分（需要 xmake-rust 插件）
    add_files("rust/src/*.rs")
    add_packages("rust::my_crate")
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 地址 | 说明 |
|------|------|------|
| 官方网站 | https://xmake.io | 项目主页 |
| 官方文档 | https://xmake.io/#/zh-cn | 中文文档 |
| GitHub 仓库 | https://github.com/xmake-io/xmake | 源码仓库 |
| 包仓库 | https://github.com/xmake-io/xmake-repo | 官方包仓库 |
| 示例项目 | https://github.com/xmake-io/xmake/tree/master/tests | 测试用例 |

### 8.2 学习路径

| 阶段 | 学习内容 | 建议时间 |
|------|----------|----------|
| 入门 | 基本命令、简单配置 | 1-2 天 |
| 进阶 | 多目标、依赖管理 | 1 周 |
| 高级 | 自定义规则、插件开发 | 2-4 周 |
| 精通 | CI/CD 集成、大型项目架构 | 持续实践 |

**推荐学习顺序：**

1. 官方快速入门教程
2. 创建简单项目实践
3. 学习包管理 xrepo
4. 多目标项目配置
5. 自定义任务和规则
6. CI/CD 集成实践
7. 参与社区贡献

### 8.3 社区资源

| 资源 | 链接 | 说明 |
|------|------|------|
| 官方论坛 | https://github.com/xmake-io/xmake/discussions | 问题讨论 |
| Gitter | https://gitter.im/xmake-io/xmake | 实时聊天 |
| Bilibili | 搜索"xmake" | 视频教程 |
| 知乎专栏 | 搜索"xmake" | 文章教程 |