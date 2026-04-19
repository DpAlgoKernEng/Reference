# Bazel - Google 多语言构建系统

## 1. 概述与背景

### 1.1 工具定位

Bazel 是 Google 开源的现代化构建和测试工具，专为大规模、多语言、多平台代码库设计。它采用内容可寻址的存储模型，支持增量构建、远程缓存和分布式执行，能够显著提升大型项目的构建效率。

核心设计理念：
- **正确性优先**：确保构建结果可重现、可验证
- **增量构建**：只重新构建实际变更的部分
- **可扩展性**：支持分布式执行和远程缓存
- **多语言支持**：原生支持 Java、C/C++、Python、Go、Rust 等语言

### 1.2 发展历史

| 年份 | 版本 | 里程碑事件 |
|------|------|------------|
| 2006 | 内部版 | Google 内部 Blaze 系统开始使用 |
| 2015 | 0.1.0 | 开源发布，更名为 Bazel |
| 2017 | 0.5.0 | 引入 Android 构建规则 |
| 2018 | 0.15.0 | 支持 C++ 工具链配置 |
| 2019 | 1.0.0 | 正式版本发布，API 稳定 |
| 2020 | 3.0.0 | 引入 Bzlmod 依赖管理 |
| 2022 | 5.0.0 | 原生支持 Apple 平台 |
| 2023 | 6.0.0 | Bzlmod 成为默认依赖管理方案 |
| 2024 | 7.0.0 | 性能优化与远程执行增强 |

### 1.3 核心特性

| 特性 | 说明 | 技术原理 |
|------|------|----------|
| 增量构建 | 只重新构建变更部分 | 内容可寻址存储 + 依赖图分析 |
| 远程缓存 | 团队共享构建产物 | 内容寻址存储服务器 (CAS) |
| 分布式执行 | 多机器并行构建 | 远程执行协议 (REAPI) |
| 多语言支持 | 原生支持多种语言 | 规则系统 + 工具链抽象 |
| 可复现构建 | 确定性输出结果 | 沙箱隔离 + 固定时间戳 |
| 沙箱执行 | 隔离构建环境 | Linux namespaces / macOS sandbox |
| 查询语言 | 分析依赖关系 | Sky查询语言 + cquery |

### 1.4 适用场景

| 场景 | 推荐程度 | 说明 |
|------|----------|------|
| 大型 monorepo | 强烈推荐 | Google 内部最佳实践场景 |
| 分布式团队 | 强烈推荐 | 远程缓存加速跨地域协作 |
| 多语言项目 | 推荐 | 原生多语言支持 |
| CI/CD 加速 | 推荐 | 缓存机制显著减少构建时间 |
| 微服务架构 | 推荐 | 统一构建体验 |
| 小型项目 | 不推荐 | 学习成本高于收益 |
| 快速原型开发 | 不推荐 | 配置相对繁琐 |

### 1.5 与其他构建工具对比

| 特性 | Bazel | CMake | Ninja | Make |
|------|-------|-------|-------|------|
| 增量构建 | 优秀 | 良好 | 优秀 | 基础 |
| 远程缓存 | 内置 | 需配置 | 无 | 无 |
| 分布式执行 | 内置 | 无 | 无 | 无 |
| 学习曲线 | 陡峭 | 中等 | 简单 | 简单 |
| 多语言支持 | 原生 | 需配置 | 无 | 无 |
| 生态成熟度 | 发展中 | 成熟 | 成熟 | 成熟 |
| 配置复杂度 | 中等 | 中等 | 简单 | 简单 |
| 可重现构建 | 强 | 弱 | 中 | 弱 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS 安装：**

```bash
# 使用 Homebrew（推荐）
brew install bazel

# 安装特定版本
brew install bazelisk  # 版本管理工具

# 使用二进制安装
curl -LO https://github.com/bazelbuild/bazel/releases/download/7.0.0/bazel-7.0.0-darwin-x86_64
chmod +x bazel-7.0.0-darwin-x86_64
sudo mv bazel-7.0.0-darwin-x86_64 /usr/local/bin/bazel
```

**Ubuntu/Debian 安装：**

```bash
# 添加 Bazel 仓库
sudo apt install apt-transport-https curl gnupg
curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg
sudo mv bazel.gpg /etc/apt/trusted.gpg.d/
echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list

# 安装
sudo apt update
sudo apt install bazel

# 安装特定版本
sudo apt install bazel-6.4.0
```

**Windows 安装：**

```powershell
# 使用 winget
winget install bazel

# 使用 Chocolatey
choco install bazel

# 手动安装
# 1. 下载 https://github.com/bazelbuild/bazel/releases
# 2. 添加到 PATH 环境变量
```

### 2.2 版本管理

推荐使用 Bazelisk 进行版本管理：

```bash
# 安装 Bazelisk
npm install -g @bazel/bazelisk
# 或
brew install bazelisk

# 使用 .bazelversion 文件指定版本
echo "7.0.0" > .bazelversion

# 环境变量指定版本
export USE_BAZEL_VERSION=7.0.0
bazel build //...
```

### 2.3 环境配置

**.bazelrc 配置文件：**

```bash
# 构建选项
build --jobs=4
build --compilation_mode=opt

# 平台特定配置
build:macos --apple_platform_type=macos
build:linux --platforms=@platforms//os:linux

# 远程缓存配置
build --remote_cache=grpc://cache.example.com:9092
build --remote_upload_local_results=true

# 输出配置
build --verbose_failures
test --test_output=all

# C++ 工具链配置
build --cxxopt=-std=c++17
build --linkopt=-lm

# 内存限制
build --local_ram_resources=HOST_RAM*.67
```

### 2.4 验证安装

```bash
# 检查版本
bazel --version

# 运行自检
bazel info workspace
bazel info release

# 测试构建
mkdir -p /tmp/bazel-test && cd /tmp/bazel-test
cat > WORKSPACE << 'EOF'
workspace(name = "test")
EOF

cat > BUILD << 'EOF'
cc_binary(
    name = "hello",
    srcs = ["hello.cc"],
)
EOF

cat > hello.cc << 'EOF'
#include <iostream>
int main() {
    std::cout << "Hello Bazel!" << std::endl;
    return 0;
}
EOF

bazel build //:hello
bazel run //:hello
```

## 3. 基础使用

### 3.1 快速入门

完整的 Bazel 项目从创建工作区开始：

```bash
# 创建项目目录
mkdir my_bazel_project && cd my_bazel_project

# 创建工作区文件
cat > WORKSPACE << 'EOF'
workspace(name = "my_bazel_project")

# 声明外部依赖（可选）
http_archive(
    name = "rules_cc",
    urls = ["https://github.com/bazelbuild/rules_cc/releases/download/0.0.1/rules_cc-0.0.1.tar.gz"],
    sha256 = "..."
)
EOF

# 创建构建文件
cat > BUILD << 'EOF'
cc_binary(
    name = "main",
    srcs = ["main.cpp"],
)
EOF

# 创建源文件
cat > main.cpp << 'EOF'
#include <iostream>
int main() {
    std::cout << "Hello, Bazel!" << std::endl;
    return 0;
}
EOF

# 构建并运行
bazel build //:main
bazel run //:main
```

### 3.2 项目结构

```
project/
├── WORKSPACE           # 工作区定义（项目根目录）
├── WORKSPACE.bzlmod    # Bzlmod 模块定义（可选）
├── BUILD               # 根目录构建规则
├── .bazelrc            # Bazel 配置文件
├── .bazelignore        # 忽略文件列表
├── .bazelversion       # 指定 Bazel 版本
├── src/
│   ├── main/
│   │   ├── cpp/
│   │   │   ├── main.cpp
│   │   │   └── BUILD
│   │   └── java/
│   │       ├── Main.java
│   │       └── BUILD
│   └── lib/
│       ├── utils.cpp
│       ├── utils.h
│       └── BUILD
├── third_party/        # 第三方依赖
│   └── BUILD
└── tools/              # 自定义工具和规则
    └── BUILD
```

### 3.3 基本命令

| 命令 | 说明 | 示例 |
|------|------|------|
| `bazel build` | 构建目标 | `bazel build //:myapp` |
| `bazel test` | 运行测试 | `bazel test //...` |
| `bazel run` | 运行程序 | `bazel run //:myapp` |
| `bazel clean` | 清理构建 | `bazel clean` |
| `bazel query` | 查询目标 | `bazel query //...` |
| `bazel cquery` | 配置查询 | `bazel cquery //...` |
| `bazel info` | 获取信息 | `bazel info workspace` |
| `bazel coverage` | 代码覆盖率 | `bazel coverage //...` |

**命令详解：**

```bash
# 构建特定目标
bazel build //src/main:myapp

# 构建所有目标
bazel build //...

# 使用特定配置构建
bazel build -c opt //:myapp  # 优化构建
bazel build -c dbg //:myapp  # 调试构建

# 并行构建
bazel build --jobs=8 //:myapp

# 运行测试并显示详细输出
bazel test --test_output=all //:mylib_test

# 运行特定测试
bazel test //src/lib:utils_test --test_filter=TestUtils.*

# 查看依赖图
bazel query "deps(//:myapp)" --output=graph

# 清理构建产物
bazel clean                    # 清理输出目录
bazel clean --expunge          # 完全清理工作区
```

### 3.4 常用操作

**依赖管理：**

```python
# WORKSPACE 文件中声明外部依赖
http_archive(
    name = "abseil_cpp",
    urls = ["https://github.com/abseil/abseil-cpp/archive/20230125.0.tar.gz"],
    strip_prefix = "abseil-cpp-20230125.0",
)

# BUILD 文件中使用
cc_binary(
    name = "myapp",
    srcs = ["main.cpp"],
    deps = ["@abseil_cpp//absl/strings"],
)
```

**多平台构建：**

```bash
# 为不同平台构建
bazel build //:myapp --platforms=@platforms//os:linux
bazel build //:myapp --platforms=@platforms//os:windows
bazel build //:myapp --platforms=@platforms//os:macos
```

## 4. 进阶特性

### 4.1 高级配置

**自定义工具链：**

```python
# tools/cpp/BUILD
load("@rules_cc//cc:defs.bzl", "cc_toolchain_suite", "cc_toolchain")

cc_toolchain_suite(
    name = "toolchain",
    toolchains = {
        "k8": ":linux_toolchain",
        "darwin": ":macos_toolchain",
    },
)

cc_toolchain(
    name = "linux_toolchain",
    toolchain_identifier = "linux_toolchain",
    toolchain_config = ":linux_config",
)

# 使用自定义工具链
# .bazelrc
build --extra_toolchains=@my_project//tools/cpp:toolchain
```

**传递变量和定义：**

```python
cc_library(
    name = "config",
    defines = [
        "VERSION=\"1.0.0\"",
        "DEBUG_MODE",
    ],
    local_defines = [
        "INTERNAL_FEATURE",
    ],
)

cc_binary(
    name = "myapp",
    srcs = ["main.cpp"],
    defines = ["ENABLE_LOGGING=1"],
    copts = ["-Wall", "-Wextra"],
    linkopts = ["-lpthread"],
)
```

### 4.2 扩展功能

**自定义规则：**

```python
# rules/my_rules.bzl
def _my_custom_rule_impl(ctx):
    output = ctx.actions.declare_file(ctx.label.name + ".out")
    
    ctx.actions.run(
        outputs = [output],
        inputs = ctx.files.srcs,
        executable = ctx.executable._tool,
        arguments = [ctx.attr.message, output.path],
    )
    
    return DefaultInfo(files = depset([output]))

my_custom_rule = rule(
    implementation = _my_custom_rule_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = True),
        "message": attr.string(default = "Hello"),
        "_tool": attr.label(
            default = "//tools:my_tool",
            executable = True,
            cfg = "exec",
        ),
    },
)
```

**宏定义：**

```python
# macros/cc.bzl
def cc_library_with_tests(name, srcs, hdrs, tests = None, **kwargs):
    """创建库并自动生成测试目标"""
    cc_library(
        name = name,
        srcs = srcs,
        hdrs = hdrs,
        **kwargs
    )
    
    if tests:
        cc_test(
            name = name + "_test",
            srcs = tests,
            deps = [":" + name],
        )

# BUILD 文件中使用
load("//macros:cc.bzl", "cc_library_with_tests")

cc_library_with_tests(
    name = "utils",
    srcs = ["utils.cpp"],
    hdrs = ["utils.h"],
    tests = ["utils_test.cpp"],
)
```

### 4.3 插件生态

| 插件/规则 | 说明 | 适用语言 |
|-----------|------|----------|
| rules_cc | C/C++ 构建规则 | C/C++ |
| rules_java | Java 构建规则 | Java |
| rules_python | Python 构建规则 | Python |
| rules_go | Go 构建规则 | Go |
| rules_rust | Rust 构建规则 | Rust |
| rules_proto | Protocol Buffers | 多语言 |
| rules_docker | Docker 镜像构建 | 容器 |
| rules_k8s | Kubernetes 配置 | K8s |

## 5. 性能优化

### 5.1 调优策略

**并行构建优化：**

```bash
# .bazelrc 配置
build --jobs=auto              # 自动检测 CPU 核心数
build --local_ram_resources=HOST_RAM*.8  # 使用 80% 内存

# 分析构建性能
bazel build //:myapp --profile=/tmp/profile
bazel analyze-profile /tmp/profile

# 远程执行加速
build --remote_executor=grpc://executor.example.com:9090
build --remote_timeout=3600
```

**缓存优化：**

```bash
# 磁盘缓存
build --disk_cache=/path/to/cache

# 远程缓存
build --remote_cache=grpc://cache.example.com:9092
build --remote_upload_local_results=true
build --remote_accept_cached=true

# 增量构建分析
bazel aquery "outputs('*.o', //:myapp)"
```

### 5.2 最佳实践

| 优化项 | 说明 | 配置示例 |
|--------|------|----------|
| 细粒度目标 | 小而专注的库单元 | 每个 `cc_library` 职责单一 |
| 依赖声明 | 明确声明所有依赖 | `deps` 完整列出依赖 |
| 沙箱隔离 | 使用沙箱保证可重现 | `--spawn_strategy=sandboxed` |
| 输出目录分离 | 不同配置独立输出 | `--distinct_host_configuration` |
| 避免通配符 | 明确列出文件 | 不使用 `glob(["**/*.cpp"])` |

```python
# 好的实践：细粒度库
cc_library(
    name = "string_utils",
    srcs = ["string_utils.cpp"],
    hdrs = ["string_utils.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "file_utils",
    srcs = ["file_utils.cpp"],
    hdrs = ["file_utils.h"],
    deps = [":string_utils"],
)

# 避免：大而全的库
cc_library(
    name = "utils",  # 包含太多内容
    srcs = glob(["**/*.cpp"]),  # 不推荐
    hdrs = glob(["**/*.h"]),
)
```

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|----------|
| 构建缓存失效 | 时间戳/环境变化 | 使用 `--incompatible_strict_action_env` |
| 依赖解析失败 | WORKSPACE 配置错误 | 检查 `http_archive` URL 和 sha256 |
| 工具链找不到 | 平台配置缺失 | 配置 `--extra_toolchains` |
| 内存不足 | 构建目标过大 | 减少 `--jobs` 或使用远程执行 |
| 沙箱权限错误 | 文件系统限制 | 使用 `--spawn_strategy=standalone` |

**调试命令：**

```bash
# 查看详细构建信息
bazel build //:myapp --verbose_failures --sandbox_debug

# 分析依赖问题
bazel query "somepath(//:myapp, @external_lib//:target)"
bazel query "rdeps(//..., //:my_target)"

# 检查工具链配置
bazel cquery //:myapp --output=starlark --starlark:expr="target.toolchains"

# 清理并重新构建
bazel clean --expunge
bazel build //:myapp
```

### 6.2 调试技巧

```bash
# 查看构建动作
bazel aquery "mnemonic('CppCompile', //:myapp)"

# 检查目标属性
bazel cquery //:myapp --output=starlark --starlark:expr="provider(target, 'CcInfo')"

# 查看构建日志
bazel info output_base  # 获取输出目录
ls $(bazel info output_base)/execroot/my_project/bazel-out/

# 运行时调试
bazel run //:myapp --gdb  # 使用 GDB 调试

# 测试调试
bazel test //:mylib_test --test_output=streamed --test_arg=--gtest_filter=TestFoo.*
```

## 7. 集成实践

### 7.1 工具链集成

**与 Clang/LLVM 集成：**

```python
# WORKSPACE
http_archive(
    name = "llvm_toolchain",
    urls = ["https://github.com/llvm/llvm-project/releases/download/llvmorg-17.0.0/clang+llvm-17.0.0-x86_64-linux-gnu-ubuntu-22.04.tar.xz"],
    strip_prefix = "clang+llvm-17.0.0-x86_64-linux-gnu-ubuntu-22.04",
)

# .bazelrc
build --repo_env=CC=clang
build --repo_env=CXX=clang++
```

**与 CMake 项目集成：**

```python
# WORKSPACE
cmake_external(
    name = "cmake_dep",
    cmake_options = [
        "-DCMAKE_BUILD_TYPE=Release",
        "-DBUILD_SHARED_LIBS=OFF",
    ],
    lib_source = "@external_lib//:srcs",
)
```

### 7.2 CI/CD 配置

**GitHub Actions 示例：**

```yaml
# .github/workflows/bazel.yml
name: Bazel CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Setup Bazel
        uses: bazelbuild/setup-bazelisk@v3
      
      - name: Mount bazel cache
        uses: actions/cache@v3
        with:
          path: |
            ~/.cache/bazel
            ~/.cache/bazelisk
          key: bazel-${{ hashFiles('WORKSPACE', '.bazelversion') }}
      
      - name: Build
        run: bazel build //...
      
      - name: Test
        run: bazel test //... --test_output=errors
      
      - name: Coverage
        run: bazel coverage //...
```

**远程缓存配置：**

```bash
# 使用 Google Cloud Storage 作为远程缓存
build --remote_cache=https://storage.googleapis.com/my-bucket/cache
build --google_credentials=/path/to/credentials.json

# 使用本地 Redis 缓存
build --remote_cache=grpc://redis.example.com:9092
```

### 7.3 实战案例

**大型 C++ 项目构建配置：**

```python
# WORKSPACE
workspace(name = "large_cpp_project")

# 依赖管理
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_absl",
    urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20230125.0.tar.gz"],
    strip_prefix = "abseil-cpp-20230125.0",
)

http_archive(
    name = "com_github_gflags_gflags",
    urls = ["https://github.com/gflags/gflags/archive/refs/tags/v2.2.2.tar.gz"],
    strip_prefix = "gflags-2.2.2",
)

http_archive(
    name = "com_google_googletest",
    urls = ["https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz"],
    strip_prefix = "googletest-1.14.0",
)
```

```python
# src/lib/core/BUILD
cc_library(
    name = "core",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/container",
    ],
    copts = ["-Wall", "-Werror"],
    visibility = ["//visibility:public"],
)

# src/server/BUILD
cc_binary(
    name = "server",
    srcs = ["main.cc", "server.cc"],
    hdrs = ["server.h"],
    deps = [
        "//src/lib/core:core",
        "@com_github_gflags_gflags//:gflags",
    ],
)

cc_test(
    name = "server_test",
    srcs = ["server_test.cc"],
    deps = [
        ":server",
        "@com_google_googletest//:gtest_main",
    ],
)
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 | 说明 |
|------|------|------|
| 官方主页 | https://bazel.build | Bazel 官方网站 |
| 官方文档 | https://bazel.build/docs | 完整文档 |
| 构建百科 | https://bazel.build/rules/rules | 构建规则参考 |
| 迁移指南 | https://bazel.build/migrate | 迁移文档 |
| GitHub 仓库 | https://github.com/bazelbuild/bazel | 源码仓库 |

### 8.2 学习路径

| 阶段 | 学习内容 | 资源 |
|------|----------|------|
| 入门 | WORKSPACE、BUILD 文件基础 | 官方入门教程 |
| 进阶 | 自定义规则、工具链配置 | Advanced Bazel 课程 |
| 高级 | 远程执行、分布式缓存 | BazelConf 演讲 |
| 专家 | 规则开发、性能调优 | 源码阅读 |

**推荐学习顺序：**

1. 完成官方入门教程
2. 理解依赖管理和增量构建原理
3. 学习 Starlark 语言编写自定义规则
4. 配置远程缓存和分布式执行
5. 深入阅读 rules_cc 源码

### 8.3 社区资源

- **Bazel Blog**: https://blog.bazel.build
- **Stack Overflow**: [bazel] 标签
- **Slack**: #bazel 频道
- **邮件列表**: bazel-discuss@googlegroups.com