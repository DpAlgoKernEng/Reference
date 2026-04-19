# LLVM - 模块化编译器基础设施

## 1. 概述与背景

### 1.1 工具定位

LLVM（Low Level Virtual Machine）是一个模块化、可复用的编译器基础设施项目，提供从编译器前端到后端的完整工具链。与传统的整体式编译器不同，LLVM 采用库化设计，各组件可独立使用和组合。

核心设计理念：
- **模块化架构**：编译器各阶段作为独立库实现
- **统一中间表示**：所有前端共享相同的 LLVM IR
- **SSA 形式**：静态单赋值形式便于优化
- **多目标支持**：一套 IR 可生成多种目标架构代码

### 1.2 发展历史

| 年份 | 版本 | 里程碑事件 |
|------|------|-----------|
| 2000 | - | Vikram Adve 和 Chris Lattner 开始 LLVM 研究 |
| 2003 | 1.0 | LLVM 项目开源发布 |
| 2005 | - | Apple 赞助开发，Clang 项目启动 |
| 2007 | 2.0 | 引入新 IR 结构和异常处理 |
| 2008 | - | Clang 成为 Apple 官方编译器 |
| 2010 | 2.8 | 引入新的循环优化框架 |
| 2011 | 3.0 | 支持 C++11，引入 LLD 链接器 |
| 2013 | 3.3 | 支持 C++11 完整特性 |
| 2015 | 3.7 | 支持 OpenMP 3.1 |
| 2017 | 4.0 | 引入 ORC JIT，支持 C++17 |
| 2019 | 8.0 | 默认启用新 Pass 管理器 |
| 2020 | 11.0 | 支持 C++20 |
| 2022 | 15.0 | 支持 C++20 完整特性 |
| 2024 | 18.0 | 最新稳定版，持续改进 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| **Clang 前端** | 快速、诊断友好的 C/C++/Objective-C 编译器 |
| **LLVM IR** | 类型化、SSA 形式的中间表示 |
| **优化框架** | 丰富的优化 Pass 库 |
| **多目标后端** | 支持 x86、ARM、RISC-V、WebAssembly 等 |
| **JIT 编译** | 运行时代码生成和执行 |
| **工具链完整** | 编译器、链接器、调试器、分析工具 |

### 1.4 适用场景

| 场景 | 推荐 | 说明 |
|------|------|------|
| 编译器开发 | 强烈推荐 | 模块化设计便于构建新语言 |
| 代码分析工具 | 强烈推荐 | 丰富的 IR 分析 API |
| JIT 编译器 | 强烈推荐 | ORC JIT 成熟稳定 |
| 静态分析 | 强烈推荐 | Clang Static Analyzer |
| 跨平台开发 | 推荐 | 统一的前端和后端 |
| 性能敏感项目 | 推荐 | 高质量代码生成 |
| 快速编译 | 推荐 | Clang 编译速度优于 GCC |

### 1.5 对比分析

| 特性 | LLVM/Clang | GCC |
|------|------------|-----|
| 架构 | 模块化库 | 整体式 |
| 编译速度 | 快 | 中等 |
| 错误诊断 | 详细友好 | 标准输出 |
| IR 设计 | SSA 形式 | RTL/GIMPLE |
| 工具链 | 统一 | 分散 |
| 插件支持 | 完善 | 有限 |
| 跨平台 | 优秀 | 良好 |
| C++ 标准支持 | 快速跟进 | 快速跟进 |
| Sanitizers | 完善 | 完善 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS：**

```bash
# 使用 Homebrew 安装
brew install llvm

# 安装完整工具链
brew install llvm clang-format clang-tidy
```

**Ubuntu/Debian：**

```bash
# 基础安装
sudo apt update
sudo apt install llvm llvm-dev clang lld

# 完整安装（包含文档和工具）
sudo apt install llvm llvm-dev clang lld clang-format clang-tidy \
    libclang-dev libllvm-ocaml-dev libclang-cpp-dev

# 安装特定版本
sudo apt install llvm-18 clang-18 lld-18
```

**CentOS/RHEL/Fedora：**

```bash
# CentOS/RHEL
sudo yum install llvm llvm-devel clang lld

# Fedora
sudo dnf install llvm llvm-devel clang lld
```

**Windows：**

```powershell
# 使用 Chocolatey
choco install llvm

# 或下载预编译包
# https://github.com/llvm/llvm-project/releases
```

**源码编译：**

```bash
# 克隆源码
git clone https://github.com/llvm/llvm-project.git
cd llvm-project

# 配置构建
cmake -B build -S llvm \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_PROJECTS="clang;lld" \
    -DLLVM_TARGETS_TO_BUILD="X86;ARM;RISCV" \
    -DLLVM_BUILD_EXAMPLES=ON \
    -DLLVM_BUILD_TESTS=ON

# 编译（多核并行）
cmake --build build -j$(nproc)

# 安装
sudo cmake --install build
```

### 2.2 版本管理

```bash
# 查看当前版本
llvm-config --version
clang --version

# 多版本共存（Ubuntu）
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100

# 切换版本
sudo update-alternatives --config clang
```

### 2.3 环境配置

```bash
# 设置 LLVM 路径（macOS Homebrew）
export LLVM_HOME=/usr/local/opt/llvm
export PATH=$LLVM_HOME/bin:$PATH

# 设置库路径
export LD_LIBRARY_PATH=$LLVM_HOME/lib:$LD_LIBRARY_PATH

# CMake 配置
export CMAKE_PREFIX_PATH=$LLVM_HOME:$CMAKE_PREFIX_PATH
```

### 2.4 验证安装

```bash
# 验证 LLVM 工具
llvm-config --version
llvm-config --prefix
llvm-config --libs all

# 验证 Clang
clang --version
echo 'int main() { return 0; }' | clang -x c - -o /tmp/test && echo "OK"

# 验证 LLD 链接器
clang -fuse-ld=lld --version

# 查看支持的架构
llc --version | grep "Registered Target"
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 编译 C 程序
clang hello.c -o hello

# 编译 C++ 程序
clang++ hello.cpp -o hello

# 编译并运行
clang++ main.cpp -o main && ./main
```

### 3.2 主要组件

| 组件 | 说明 | 用途 |
|------|------|------|
| **llvm-core** | 核心库和 IR | 编译器基础设施 |
| **Clang** | C/C++/ObjC 前端 | 源码编译 |
| **LLD** | LLVM 链接器 | 目标文件链接 |
| **libc++** | C++ 标准库 | 标准库实现 |
| **compiler-rt** | 运行时库 | Sanitizers 支持 |
| **LLVM tools** | 工具集合 | opt、llc、llvm-as 等 |

### 3.3 LLVM IR（中间表示）

#### 3.3.1 IR 格式

LLVM IR 有三种表示形式：

| 格式 | 扩展名 | 说明 |
|------|--------|------|
| 文本格式 | `.ll` | 人类可读的汇编形式 |
| 位码格式 | `.bc` | 高效的二进制格式 |
| 内存格式 | - | 编译时内存中表示 |

#### 3.3.2 生成 IR

```bash
# 生成文本 IR
clang -emit-llvm -S program.c -o program.ll

# 生成位码 IR
clang -emit-llvm program.c -o program.bc

# 从 IR 编译为可执行文件
clang program.ll -o program

# IR 到汇编
llc program.ll -o program.s
```

#### 3.3.3 IR 示例

```llvm
; 函数定义示例
define i32 @add(i32 %a, i32 %b) {
entry:
    %result = add i32 %a, %b
    ret i32 %result
}

; 主函数示例
define i32 @main() {
entry:
    %call = call i32 @add(i32 1, i32 2)
    ret i32 %call
}

; 全局变量
@global_var = global i32 42
@const_str = constant [13 x i8] c"Hello World\00"
```

#### 3.3.4 IR 类型系统

| 类型 | 说明 | 示例 |
|------|------|------|
| `i1` | 1 位整数（布尔） | `i1 true` |
| `i8` | 8 位整数 | `i8 127` |
| `i16` | 16 位整数 | `i16 32767` |
| `i32` | 32 位整数 | `i32 2147483647` |
| `i64` | 64 位整数 | `i64 9223372036854775807` |
| `float` | 32 位浮点 | `float 3.14` |
| `double` | 64 位浮点 | `double 3.14159265359` |
| `void` | 无类型 | `void` |
| `ptr` | 指针（不透明） | `ptr @function` |

### 3.4 LLVM 工具链

#### 3.4.1 clang - 前端编译器

```bash
# 基本编译
clang program.c -o program

# C++ 编译
clang++ program.cpp -o program

# 指定语言标准
clang -std=c11 program.c
clang++ -std=c++20 program.cpp

# 优化级别
clang -O0 program.c    # 无优化（调试）
clang -O1 program.c    # 基本优化
clang -O2 program.c    # 标准优化
clang -O3 program.c    # 高级优化
clang -Os program.c    # 体积优化
clang -Oz program.c    # 极致体积优化

# 生成调试信息
clang -g program.c -o program

# 使用 LLD 链接器
clang -fuse-ld=lld program.c -o program

# 查看编译阶段
clang -ccc-print-phases program.c
```

#### 3.4.2 llc - IR 到汇编

```bash
# IR 到汇编
llc program.ll -o program.s

# 指定目标架构
llc -march=x86 program.ll -o program.s
llc -march=arm program.ll -o program.s
llc -march=riscv64 program.ll -o program.s

# 指定优化级别
llc -O3 program.ll -o program.s

# 输出目标文件
llc -filetype=obj program.ll -o program.o
```

#### 3.4.3 opt - IR 优化器

```bash
# 运行所有优化
opt -O3 program.ll -o optimized.bc

# 运行特定优化 Pass
opt -mem2reg program.ll -o optimized.bc
opt -gvn program.ll -o optimized.bc

# 查看 IR 优化效果
opt -O2 -S program.ll -o optimized.ll

# 查看可用的 Pass
opt --help-hidden | grep "pass"
opt --print-passes
```

#### 3.4.4 其他核心工具

| 工具 | 命令 | 说明 |
|------|------|------|
| llvm-as | `llvm-as file.ll -o file.bc` | 文本 IR 到位码 |
| llvm-dis | `llvm-dis file.bc -o file.ll` | 位码到文本 IR |
| lli | `lli program.bc` | IR 解释执行/JIT |
| llvm-link | `llvm-link a.bc b.bc -o linked.bc` | IR 文件链接 |
| llvm-nm | `llvm-nm program.o` | 符号表查看 |
| llvm-objdump | `llvm-objdump -d program.o` | 反汇编 |
| llvm-strip | `llvm-strip program.o` | 移除符号信息 |

### 3.5 编译流程

```
源代码 (C/C++)
    |
    v
+------------------+
| Clang 前端       |
| - 词法分析       |
| - 语法分析       |
| - 语义分析       |
+------------------+
    |
    v
LLVM IR（优化前）
    |
    v
+------------------+
| opt 优化器       |
| - 优化 Pass      |
| - IR 变换        |
+------------------+
    |
    v
LLVM IR（优化后）
    |
    v
+------------------+
| llc 后端         |
| - 指令选择       |
| - 寄存器分配     |
| - 代码生成       |
+------------------+
    |
    v
目标代码（汇编/机器码）
    |
    v
+------------------+
| LLD 链接器       |
| - 符号解析       |
| - 重定位         |
+------------------+
    |
    v
可执行文件
```

## 4. 进阶特性

### 4.1 优化 Pass

#### 4.1.1 常用优化 Pass

| Pass | 说明 | 作用 |
|------|------|------|
| `-mem2reg` | 内存到寄存器 | 提升局部变量到寄存器 |
| `-deadargelim` | 死参数消除 | 删除未使用的函数参数 |
| `-dce` | 死代码消除 | 删除不可达代码 |
| `-adce` | 激进死代码消除 | 更激进的死代码删除 |
| `-gvn` | 全局值编号 | 消除冗余计算 |
| `-licm` | 循环不变代码外提 | 将循环不变代码移出循环 |
| `-inline` | 函数内联 | 内联函数调用 |
| `-constprop` | 常量传播 | 编译期常量计算 |
| `-instcombine` | 指令合并 | 合并冗余指令 |
| `-sroa` | 聚合量标量替换 | 分解结构体/数组 |

#### 4.1.2 优化级别

| 级别 | 说明 | 典型用途 |
|------|------|---------|
| `-O0` | 无优化 | 调试 |
| `-O1` | 基本优化 | 快速编译 |
| `-O2` | 标准优化 | 发布版本 |
| `-O3` | 高级优化 | 性能优先 |
| `-Os` | 体积优化 | 嵌入式/移动端 |
| `-Oz` | 极致体积优化 | 代码大小关键 |
| `-Ofast` | 最快执行 | 允许非标准优化 |

### 4.2 Sanitizers 动态分析

```bash
# AddressSanitizer - 内存错误检测
clang -fsanitize=address -g program.c -o program
./program  # 自动检测内存错误

# MemorySanitizer - 未初始化内存读取
clang -fsanitize=memory -g program.c -o program

# ThreadSanitizer - 数据竞争检测
clang -fsanitize=thread -g program.c -o program

# UndefinedBehaviorSanitizer - 未定义行为
clang -fsanitize=undefined -g program.c -o program

# 组合使用
clang -fsanitize=address,undefined -g program.c -o program

# 泄漏检测
clang -fsanitize=leak program.c -o program
```

### 4.3 编写 LLVM Pass

#### 4.3.1 Pass 基本结构

```cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// 新 Pass 管理器
struct MyPass : public PassInfoMixin<MyPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
        errs() << "Processing function: " << F.getName() << "\n";
        
        // 遍历基本块
        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {
                // 处理指令
                if (auto *Call = dyn_cast<CallInst>(&I)) {
                    // 处理函数调用
                }
            }
        }
        
        return PreservedAnalyses::all();
    }
};
```

#### 4.3.2 注册 Pass

```cpp
// 新 Pass 管理器注册
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "MyPass", LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "my-pass") {
                        FPM.addPass(MyPass());
                        return true;
                    }
                    return false;
                });
        }};
}
```

#### 4.3.3 CMake 配置

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyPass)

find_package(LLVM REQUIRED CONFIG)

add_library(MyPass MODULE
    MyPass.cpp
)

target_include_directories(MyPass PRIVATE
    ${LLVM_INCLUDE_DIRS}
)

target_compile_features(MyPass PRIVATE cxx_std_17)

# 设置输出扩展名
set_target_properties(MyPass PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)
```

### 4.4 libc++ 标准库

```bash
# 使用 libc++ 替代 libstdc++
clang++ -stdlib=libc++ program.cpp -o program

# 指定 libc++ 头文件路径
clang++ -stdlib=libc++ -I/usr/include/c++/v1 program.cpp

# 链接 libc++abi
clang++ -stdlib=libc++ -lc++abi program.cpp
```

## 5. 性能优化

### 5.1 编译优化策略

| 策略 | 选项 | 说明 |
|------|------|------|
| 链接时优化 | `-flto` | 全局优化机会 |
| 配置文件优化 | `-fprofile-instr-use` | 基于运行数据的优化 |
| 循环向量化 | `-fvectorize` | 自动向量化 |
| 自动向量化 | `-Rpass=loop-vectorize` | 查看向量化报告 |
| 函数内联 | `-finline-functions` | 更激进的内联 |

### 5.2 Profile-Guided Optimization (PGO)

```bash
# 步骤 1：生成插桩版本
clang -fprofile-instr-generate program.c -o program_pgo

# 步骤 2：运行典型工作负载
./program_pgo < typical_input.txt
# 生成 default.profraw

# 步骤 3：合并 Profile 数据
llvm-profdata merge -output=default.profdata default.profraw

# 步骤 4：使用 Profile 重新编译
clang -fprofile-instr-use=default.profdata program.c -o program_optimized
```

### 5.3 最佳实践

| 场景 | 推荐配置 |
|------|---------|
| 开发调试 | `-O0 -g -fsanitize=address,undefined` |
| 单元测试 | `-O1 -g --coverage` |
| 发布版本 | `-O3 -DNDEBUG -flto` |
| 嵌入式 | `-Os -ffunction-sections -fdata-sections` |
| 性能基准 | `-O3 -march=native -flto` |

## 6. 问题排查

### 6.1 常见问题

| 问题 | 原因 | 解决方案 |
|------|------|---------|
| 找不到头文件 | 路径未配置 | `-I` 指定路径 |
| 链接错误 | 库路径缺失 | `-L` 指定库路径 |
| 链接器错误 | LLD 未安装 | 安装 lld 包 |
| IR 解析失败 | 版本不兼容 | 更新 LLVM 版本 |
| Pass 加载失败 | 接口不匹配 | 检查 Pass 接口 |

### 6.2 调试技巧

```bash
# 查看详细编译过程
clang -v program.c

# 输出预处理结果
clang -E program.c

# 输出 AST
clang -Xclang -ast-dump program.c

# 输出所有优化 Pass
opt -debug-pass-manager -O3 program.ll -o /dev/null

# 查看 IR 各阶段
clang -mllvm -print-after-all program.c

# 查看指令选择
llc -debug-only=isel program.ll
```

## 7. 集成实践

### 7.1 CMake 集成

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject)

# 查找 LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# 设置包含路径
include_directories(${LLVM_INCLUDE_DIRS})

# 定义 LLVM 组件
llvm_map_components_to_libnames(llvm_libs
    core
    support
    irreader
    bitwriter
    passes
)

# 添加可执行文件
add_executable(my_tool main.cpp)

# 链接 LLVM 库
target_link_libraries(my_tool ${llvm_libs})

# 设置 C++ 标准
set_target_properties(my_tool PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
```

### 7.2 CI/CD 配置

**GitHub Actions 示例：**

```yaml
name: Build with LLVM

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        llvm-version: [16, 17, 18]
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install LLVM
      run: |
        wget https://apt.llvm.org/llvm.sh
        chmod +x llvm.sh
        sudo ./llvm.sh ${{ matrix.llvm-version }}
    
    - name: Build
      run: |
        mkdir build && cd build
        cmake .. \
          -DCMAKE_C_COMPILER=clang-${{ matrix.llvm-version }} \
          -DCMAKE_CXX_COMPILER=clang++-${{ matrix.llvm-version }}
        make -j$(nproc)
    
    - name: Test
      run: |
        cd build
        ctest --output-on-failure
```

### 7.3 实战案例：简单代码分析工具

```cpp
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

int main(int argc, char **argv) {
    if (argc < 2) {
        errs() << "Usage: " << argv[0] << " <file.ll>\n";
        return 1;
    }

    LLVMContext Context;
    SMDiagnostic Err;
    
    // 解析 IR 文件
    std::unique_ptr<Module> M = parseIRFile(argv[1], Err, Context);
    if (!M) {
        Err.print(argv[0], errs());
        return 1;
    }

    // 统计函数信息
    outs() << "Module: " << M->getName() << "\n";
    outs() << "Functions: " << M->size() << "\n";
    
    for (Function &F : *M) {
        if (!F.isDeclaration()) {
            outs() << "  " << F.getName() 
                   << ": " << F.size() << " basic blocks\n";
        }
    }

    return 0;
}
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| LLVM 官网 | https://llvm.org |
| LLVM 文档 | https://llvm.org/docs |
| LLVM IR 参考 | https://llvm.org/docs/LangRef.html |
| Clang 文档 | https://clang.llvm.org/docs |
| libc++ 文档 | https://libcxx.llvm.org |
| compiler-rt | https://compiler-rt.llvm.org |
| LLD 文档 | https://lld.llvm.org |

### 8.2 学习路径

| 阶段 | 内容 | 推荐资源 |
|------|------|---------|
| 入门 | Clang 编译器使用 | 官方 Getting Started |
| 进阶 | LLVM IR 编程 | LLVM IR Language Reference |
| 高级 | 编写优化 Pass | Writing an LLVM Pass 文档 |
| 专家 | JIT 开发 | ORC JIT Tutorial |
| 专家 | 后端开发 | Writing an LLVM Backend |

### 8.3 社区资源

- **GitHub**: https://github.com/llvm/llvm-project
- **邮件列表**: llvm-dev@lists.llvm.org
- **Discord**: LLVM Community Server
- **Stack Overflow**: [llvm] 标签