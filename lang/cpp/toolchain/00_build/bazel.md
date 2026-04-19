# Bazel - Google 多语言构建系统

## 1. 概述与背景

### 1.1 工具定位

Bazel 是 Google 开源的多语言、多平台构建工具，专为大规模代码库设计。作为 Google 内部构建系统 Blaze 的开源版本，Bazel 继承了其在大型项目中的构建经验和最佳实践，支持增量构建、远程缓存和分布式执行，是现代微服务架构和 monorepo 项目的重要构建工具选择。

### 1.2 发展历史

| 年份 | 里程碑 | 说明 |
|------|--------|------|
| 2015 | 开源发布 | Google 将内部构建系统 Blaze 开源为 Bazel |
| 2016 | 1.0 发布 | 首个稳定版本，支持主流语言 |
| 2018 | 远程执行 | 引入远程缓存和执行功能 |
| 2020 | Bzlmod | 新一代依赖管理系统 |
| 2022 | 性能优化 | 大幅提升构建性能 |
| 2024 | 持续演进 | 增强 Windows 支持，改进用户体验 |

### 1.3 核心特性

| 特性 | 说明 | 优势 |
|------|------|------|
| 增量构建 | 只重新构建变更的部分 | 大幅减少构建时间 |
| 远程缓存 | 团队共享构建缓存 | 避免重复构建 |
| 分布式执行 | 多机器并行构建 | 加速大规模构建 |
| 多语言支持 | C/C++/Java/Python/Go/... | 统一构建系统 |
| 可复现构建 | 确定性输出 | 构建结果一致 |
| 沙箱执行 | 隔离构建环境 | 提高构建可靠性 |
| 依赖分析 | 自动依赖管理 | 精确的增量构建 |

### 1.4 适用场景

| 场景 | 推荐 | 说明 |
|------|------|------|
| 大型 monorepo | ✅ Bazel | 天然支持大规模代码库 |
| 分布式团队 | ✅ Bazel | 远程缓存显著提升效率 |
| 多语言项目 | ✅ Bazel | 统一构建系统 |
| CI/CD 加速 | ✅ Bazel | 增量构建加速流水线 |
| 小型项目 | ❌ 不推荐 | 配置复杂度过高 |
| 快速原型 | ❌ 不推荐 | 启动成本较高 |

### 1.5 与其他构建工具对比

| 特性 | Bazel | CMake | Make | Ninja |
|------|-------|-------|------|-------|
| 增量构建 | 优秀 | 良好 | 基础 | 优秀 |
| 远程缓存 | ✅ 内置 | 需配置 | ❌ | ❌ |
| 学习曲线 | 陡峭 | 中等 | 简单 | 简单 |
| 生态成熟度 | 发展中 | 成熟 | 成熟 | 成熟 |
| 多语言支持 | ✅ 原生 | 需配置 | ❌ | ❌ |
| 依赖管理 | ✅ 强大 | 手动 | 手动 | 手动 |
| 构建速度 | 极快 | 快 | 快 | 极快 |
| 配置复杂度 | 高 | 中 | 低 | 低 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS：**

```bash
# 使用 Homebrew
brew install bazel

# 验证安装
bazel --version

# 安装特定版本
brew install bazelisk  # 推荐使用 Bazelisk 版本管理器
```

**Ubuntu/Debian：**

```bash
# 添加 Bazel 仓库
sudo apt install apt-transport-https curl gnupg
curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg
sudo mv bazel.gpg /etc/apt/trusted.gpg.d/
echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list

# 安装
sudo apt update && sudo apt install bazel
```

**Windows：**

```bash
# 使用 winget
winget install bazel

# 或使用 Chocolatey
choco install bazel

# 配置环境变量
# Bazel 需要安装 MSYS2 和 Visual Studio
```

### 2.2 版本管理

**使用 Bazelisk（推荐）：**

```bash
# 安装 Bazelisk
brew install bazelisk  # macOS
npm install -g @bazel/bazelisk  # npm

# 使用 .bazelversion 文件控制版本
echo "6.4.0" > .bazelversion

# Bazelisk 会自动下载并使用指定版本
bazel build //:target
```

**版本选择策略：**

| 项目类型 | 推荐版本 | 说明 |
|---------|---------|------|
| 生产项目 | LTS 版本 | 稳定性优先 |
| 实验项目 | 最新版本 | 功能优先 |
| 开源项目 | .bazelversion | 锁定版本 |

### 2.3 环境配置

**.bazelrc 配置文件：**

```bash
# 构建选项
build --jobs=8                    # 并行任务数
build --compilation_mode=opt      # 优化模式
build --cxxopt=-std=c++17         # C++ 标准

# 缓存配置
build --disk_cache=~/.cache/bazel
build --repository_cache=~/.cache/bazel-repo

# 平台配置
build:macos --apple_platform_type=macos
build:linux --config=linux

# 输出配置
build --show_progress
build --verbose_failures
```

### 2.4 验证安装

```bash
# 检查版本
bazel --version
# 输出：bazel 6.4.0-homebrew

# 运行测试项目
git clone https://github.com/bazelbuild/examples
cd examples/cpp-tutorial
bazel build //main:hello-world
./bazel-bin/main/hello-world
# 输出：Hello world

# 验证远程缓存（可选）
bazel build //main:hello-world --remote_cache=grpc://localhost:9092
```

## 3. 基础使用

### 3.1 快速入门

**最小化项目示例：**

```
minimal_project/
├── WORKSPACE
├── BUILD
└── main.cpp
```

**WORKSPACE 文件：**

```python
workspace(name = "minimal_project")
```

**BUILD 文件：**

```python
cc_binary(
    name = "hello",
    srcs = ["main.cpp"],
)
```

**main.cpp 文件：**

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, Bazel!" << std::endl;
    return 0;
}
```

**构建和运行：**

```bash
# 构建
bazel build //:hello

# 运行
bazel run //:hello

# 输出：Hello, Bazel!
```

### 3.2 项目结构

**标准项目布局：**

```
project/
├── WORKSPACE           # 工作区根定义
├── BUILD              # 根级别构建规则
├── .bazelrc           # Bazel 配置文件
├── .bazelversion      # Bazel 版本锁定
├── src/
│   ├── main/
│   │   ├── cpp/
│   │   │   ├── main.cpp
│   │   │   └── BUILD
│   │   └── java/
│   │       ├── Main.java
│   │       └── BUILD
│   └── test/
│       ├── cpp/
│       │   ├── utils_test.cpp
│       │   └── BUILD
│       └── java/
├── lib/
│   ├── utils/
│   │   ├── utils.cpp
│   │   ├── utils.h
│   │   └── BUILD
│   └── third_party/
│       └── BUILD
└── tools/
    └── build_rules/
        └── custom_rules.bzl
```

### 3.3 基本命令

| 命令 | 说明 | 示例 |
|------|------|------|
| `bazel build` | 构建目标 | `bazel build //:myapp` |
| `bazel test` | 运行测试 | `bazel test //:mylib_test` |
| `bazel run` | 运行程序 | `bazel run //:myapp` |
| `bazel clean` | 清理构建 | `bazel clean` |
| `bazel query` | 查询目标 | `bazel query //...` |
| `bazel cquery` | 配置查询 | `bazel cquery //:myapp` |
| `bazel info` | 显示信息 | `bazel info workspace` |
| `bazel version` | 显示版本 | `bazel version` |

**命令详解：**

```bash
# 构建特定目标
bazel build //src/main/cpp:hello

# 构建所有目标
bazel build //...

# 运行带参数的程序
bazel run //:myapp -- --arg1 value1

# 运行特定测试
bazel test //src/test/cpp:utils_test

# 运行所有测试
bazel test //...

# 清理输出
bazel clean

# 深度清理（包括缓存）
bazel clean --expunge

# 查询目标依赖
bazel query "deps(//:myapp)"

# 查询反向依赖
bazel query "rdeps(//..., //:mylib)"
```

### 3.4 常用操作

**依赖管理：**

```python
# WORKSPACE 文件中声明外部依赖
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "googletest",
    url = "https://github.com/google/googletest/archive/v1.12.1.zip",
    sha256 = "...",
    strip_prefix = "googletest-1.12.1",
)
```

**构建配置：**

```bash
# Debug 模式
bazel build //:myapp --compilation_mode=dbg

# Release 模式
bazel build //:myapp --compilation_mode=opt

# 指定平台
bazel build //:myapp --platforms=//platforms:linux_x86_64
```

## 4. 核心构建规则

### 4.1 C/C++ 构建规则

**cc_binary - 可执行文件：**

```python
cc_binary(
    name = "myapp",
    srcs = [
        "main.cpp",
        "app.cpp",
    ],
    hdrs = ["app.h"],
    deps = [
        ":mylib",
        "//lib/utils:utils_lib",
    ],
    copts = [
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-O2",
    ],
    linkopts = [
        "-lpthread",
        "-lm",
    ],
    defines = [
        "DEBUG=1",
        "VERSION=\\\"1.0.0\\\"",
    ],
    visibility = ["//visibility:public"],
)
```

**cc_library - 库文件：**

```python
cc_library(
    name = "mylib",
    srcs = [
        "utils.cpp",
        "parser.cpp",
    ],
    hdrs = [
        "utils.h",
        "parser.h",
        "public_api.h",  # 公共头文件
    ],
    visibility = ["//visibility:public"],
    linkstatic = True,  # 静态库
    # linkstatic = False,  # 动态库
)
```

**cc_test - 测试：**

```python
cc_test(
    name = "utils_test",
    srcs = ["utils_test.cpp"],
    deps = [
        ":mylib",
        "@googletest//:gtest_main",
    ],
    size = "small",  # small/medium/large/enormous
    timeout = "short",  # short/moderate/long/eternal
)
```

**cc_import - 导入预编译库：**

```python
cc_import(
    name = "precompiled_lib",
    static_library = "lib/libmylib.a",
    # 或动态库
    # shared_library = "lib/libmylib.so",
    hdrs = ["include/mylib.h"],
    visibility = ["//visibility:public"],
)
```

### 4.2 WORKSPACE 文件详解

**完整 WORKSPACE 示例：**

```python
workspace(name = "my_project")

# 加载外部依赖
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# Google Test
http_archive(
    name = "googletest",
    url = "https://github.com/google/googletest/archive/refs/tags/v1.12.1.zip",
    sha256 = "some_sha256_hash",
    strip_prefix = "googletest-1.12.1",
)

# spdlog 日志库
http_archive(
    name = "spdlog",
    url = "https://github.com/gabime/spdlog/archive/v1.11.0.zip",
    sha256 = "another_sha256_hash",
    strip_prefix = "spdlog-1.11.0",
    build_file = "//third_party:spdlog.BUILD",  # 自定义 BUILD 文件
)

# 本地工具链配置
local_repository(
    name = "local_config_cc",
    path = "tools/cpp",
)

# 新依赖管理系统（Bzlmod）
# MODULE.bazel 文件替代部分 WORKSPACE 功能
```

### 4.3 BUILD 文件详解

**多目标 BUILD 文件：**

```python
# 文件：lib/utils/BUILD

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

# 库目标
cc_library(
    name = "utils",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob([
        "include/**/*.h",
        "public/**/*.h",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "@spdlog//:spdlog",
    ],
)

# 测试目标
cc_test(
    name = "utils_test",
    srcs = glob(["test/*_test.cpp"]),
    deps = [
        ":utils",
        "@googletest//:gtest_main",
    ],
    size = "small",
)

# 可执行文件
cc_binary(
    name = "utils_demo",
    srcs = ["demo/main.cpp"],
    deps = [":utils"],
)
```

## 5. 进阶特性

### 5.1 远程缓存与执行

**远程缓存配置：**

```bash
# .bazelrc 配置
build --remote_cache=grpc://cache.example.com:9092
build --remote_upload_local_results=true
build --remote_download_outputs=all
```

**远程执行配置：**

```bash
# .bazelrc 配置
build --remote_executor=grpc://exec.example.com:9092
build --remote_timeout=3600
build --jobs=100  # 远程执行可大幅增加并发
```

**认证配置：**

```bash
build --tls_enabled=true
build --tls_certificate=/path/to/cert.pem
build --remote_instance_name=main
```

### 5.2 自定义构建规则

**创建自定义规则：**

```python
# 文件：tools/build_rules/custom_rules.bzl

def _my_custom_rule_impl(ctx):
    output = ctx.actions.declare_file(ctx.label.name + ".out")
    
    ctx.actions.run(
        outputs = [output],
        inputs = ctx.files.srcs,
        executable = ctx.executable._tool,
        arguments = [ctx.expand_location("$(location %s)" % ctx.attr.config)],
        progress_message = "Running custom rule for %s" % ctx.label.name,
    )
    
    return DefaultInfo(files = depset([output]))

my_custom_rule = rule(
    implementation = _my_custom_rule_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = True),
        "config": attr.label(allow_single_file = True),
        "_tool": attr.label(
            default = "//tools:my_tool",
            executable = True,
            cfg = "exec",
        ),
    },
)
```

**使用自定义规则：**

```python
# BUILD 文件
load("//tools/build_rules:custom_rules.bzl", "my_custom_rule")

my_custom_rule(
    name = "generated_output",
    srcs = ["input.txt"],
    config = "config.json",
)
```

### 5.3 平台与工具链

**平台定义：**

```python
# platforms/BUILD
platform(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

platform(
    name = "macos_arm64",
    constraint_values = [
        "@platforms//os:macos",
        "@platforms//cpu:arm64",
    ],
)
```

**工具链配置：**

```python
# toolchains/BUILD
toolchain(
    name = "linux_gcc_toolchain",
    exec_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
    target_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
    toolchain = "//toolchains:cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)
```

**使用平台构建：**

```bash
# 交叉编译
bazel build //:myapp --platforms=//platforms:linux_x86_64
```

## 6. 性能优化

### 6.1 构建性能分析

**性能分析工具：**

```bash
# 生成构建配置文件
bazel build //:myapp --profile=/tmp/bazel_profile.gz

# 查看分析结果
bazel analyze-profile /tmp/bazel_profile.gz
```

**性能优化选项：**

```bash
# .bazelrc 性能配置
build --jobs=auto              # 自动并行
build --experimental_repository_cache_hardlinks  # 使用硬链接
build --disk_cache=~/.cache/bazel  # 磁盘缓存
build --memory_oom_score=0      # OOM 分数

# 内存优化
build --experimental_convenience_symlinks=ignore
build --experimental_convenience_symlink_library_resolution_strategy=stat
```

### 6.2 最佳实践

**1. 细粒度目标划分：**

```python
# 好：细粒度目标
cc_library(
    name = "string_utils",
    srcs = ["string_utils.cpp"],
    hdrs = ["string_utils.h"],
)

cc_library(
    name = "file_utils",
    srcs = ["file_utils.cpp"],
    hdrs = ["file_utils.h"],
)

# 差：巨型目标
cc_library(
    name = "all_utils",
    srcs = glob(["src/*.cpp"]),  # 过度聚合
    hdrs = glob(["include/*.h"]),
)
```

**2. 精确的依赖声明：**

```python
# 好：精确依赖
cc_library(
    name = "parser",
    srcs = ["parser.cpp"],
    hdrs = ["parser.h"],
    deps = [
        ":lexer",        # 本地依赖
        "//lib/utils:utils",  # 包内依赖
    ],
)

# 差：过度依赖
cc_library(
    name = "parser",
    deps = ["//..."],  # 不要使用 //...
)
```

**3. 可见性控制：**

```python
# 好：最小可见性
cc_library(
    name = "internal_lib",
    visibility = ["//my_package:__subpackages__"],  # 包内可见
)

cc_library(
    name = "public_lib",
    visibility = ["//visibility:public"],  # 公开可见
)
```

## 7. 问题排查

### 7.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|---------|
| 构建失败：找不到头文件 | 包含路径未配置 | 在 `cc_library` 中添加 `includes` |
| 链接错误：undefined reference | 依赖未声明 | 在 `deps` 中添加依赖 |
| 构建速度慢 | 目标过大、缓存未利用 | 细粒度目标划分、启用缓存 |
| 内存不足 | 构建并发过高 | 降低 `--jobs` 参数 |
| 沙箱权限问题 | macOS 安全限制 | 添加 `--spawn_strategy=standalone` |

### 7.2 调试技巧

**详细输出：**

```bash
# 显示详细错误
bazel build //:myapp --verbose_failures

# 显示子命令
bazel build //:myapp --subcommands

# 显示动作信息
bazel build //:myapp --experimental_ui_debug_all_events
```

**查询调试：**

```bash
# 查看目标依赖树
bazel query "deps(//:myapp)" --output=graph

# 查看反向依赖
bazel query "rdeps(//..., //lib/utils:utils)"

# 查看目标属性
bazel query --output=build //:myapp
```

**清理缓存：**

```bash
# 清理输出目录
bazel clean

# 完全清理（包括外部依赖）
bazel clean --expunge

# 清理特定目标
bazel clean //lib/utils:utils
```

## 8. 集成实践

### 8.1 CI/CD 配置

**GitHub Actions 集成：**

```yaml
# .github/workflows/bazel.yml
name: Bazel CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Mount Bazel Cache
        uses: actions/cache@v3
        with:
          path: |
            ~/.cache/bazel
            ~/.cache/bazel-repo
          key: bazel-${{ runner.os }}-${{ hashFiles('WORKSPACE', 'WORKSPACE.bzlmod', '.bazelversion') }}
          restore-keys: |
            bazel-${{ runner.os }}-
      
      - name: Build
        run: bazel build //...
      
      - name: Test
        run: bazel test //...
```

**GitLab CI 集成：**

```yaml
# .gitlab-ci.yml
bazel_build:
  image: gcr.io/bazel-public/bazel:latest
  cache:
    paths:
      - .cache/bazel
  script:
    - bazel build //...
    - bazel test //...
  artifacts:
    paths:
      - bazel-bin/
```

### 8.2 IDE 集成

**VS Code 集成：**

```json
// .vscode/settings.json
{
  "bazel.buildifier.executable": "/usr/local/bin/buildifier",
  "bazel.buildifier.fixOnFormat": true,
  "bazel.queries.shareContainer": true,
  "java.configuration.updateBuildConfiguration": "automatic"
}
```

**CLion 集成：**

1. 安装 Bazel 插件
2. 导入 Bazel 项目：File → Import Bazel Project
3. 配置同步：Tools → Bazel → Sync Project with BUILD Files

### 8.3 实战案例

**项目案例：多语言微服务：**

```
microservices_project/
├── WORKSPACE
├── MODULE.bazel
├── services/
│   ├── auth_service/           # C++ 服务
│   │   ├── BUILD
│   │   └── src/
│   ├── api_gateway/            # Java 服务
│   │   ├── BUILD
│   │   └── src/
│   └── data_service/           # Go 服务
│       ├── BUILD
│       └── src/
├── libs/
│   ├── common_cpp/
│   ├── common_java/
│   └── common_go/
└── proto/                      # 共享 Proto 定义
    └── BUILD
```

**构建命令：**

```bash
# 构建所有服务
bazel build //services:all

# 构建特定服务
bazel build //services/auth_service:auth_server

# 构建并运行容器镜像（需要 rules_docker）
bazel run //services/auth_service:auth_image
```

## 9. 参考资源

### 9.1 官方文档

- [Bazel 官方文档](https://bazel.build/)
- [Bazel C++ 构建指南](https://bazel.build/tutorials/cpp)
- [Bazel 规则参考](https://bazel.build/reference/be/c-cpp)
- [Bazel 最佳实践](https://bazel.build/best-practices)

### 9.2 学习路径

| 阶段 | 内容 | 时间 |
|------|------|------|
| 入门 | 基本概念、安装配置、简单构建 | 1-2 天 |
| 进阶 | 自定义规则、远程缓存、平台配置 | 1-2 周 |
| 高级 | 性能优化、工具链定制、大型项目实践 | 1-2 月 |
| 专家 | 内部原理、规则开发、社区贡献 | 持续学习 |

### 9.3 社区资源

- [Bazel GitHub](https://github.com/bazelbuild/bazel)
- [Bazel 规则仓库](https://github.com/bazelbuild/rules_* )
- [Bazel 用户群组](https://groups.google.com/forum/#!forum/bazel-discuss)
- [Stack Overflow Bazel 标签](https://stackoverflow.com/questions/tagged/bazel)

### 9.4 推荐工具

| 工具 | 用途 | 链接 |
|------|------|------|
| Bazelisk | 版本管理 | https://github.com/bazelbuild/bazelisk |
| Buildifier | 代码格式化 | https://github.com/bazelbuild/buildtools |
| Bazel Query | 依赖分析 | Bazel 内置 |
| Bazel Profiler | 性能分析 | Bazel 内置 |