# LLVM - 编译器基础设施项目

## 1. 概述与背景

### 1.1 工具定位

LLVM（Low Level Virtual Machine）是一个模块化、可复用的编译器基础设施项目，提供了一整套编译器和工具链技术。与传统的编译器不同，LLVM 采用了模块化设计，允许开发者重用其各个组件来构建自定义的编译器、代码分析工具和优化器。

LLVM 的核心设计理念：
- **模块化架构**：前端、优化器、后端独立分离
- **统一中间表示**：所有语言共享相同的 IR
- **SSA 形式**：静态单赋值形式简化优化
- **库化设计**：组件可独立使用

### 1.2 发展历史

| 年份 | 版本 | 里程碑事件 |
|------|------|-----------|
| 2000 | - | Vikram Adve 和 Chris Lattner 开始 LLVM 研究 |
| 2003 | 1.0 | LLVM 项目正式开源发布 |
| 2006 | - | Apple 开始采用 LLVM |
| 2007 | 2.0 | 引入新的 IR 结构和优化框架 |
| 2008 | - | Clang 成为 LLVM 官方 C/C++ 前端 |
| 2011 | 3.0 | 支持 C++11 标准 |
| 2013 | 3.3 | Apple 在 Xcode 5 中默认使用 Clang |
| 2017 | 4.0 | 支持 C++17 标准 |
| 2019 | 9.0 | 支持 C++20 标准 |
| 2023 | 17.0 | 支持最新 C++23 标准 |

### 1.3 核心特性

**主要组件**

| 组件 | 说明 |
|------|------|
| **llvm-core** | 核心库和 IR 表示 |
| **Clang** | C/C++/Objective-C 前端编译器 |
| **LLD** | 高性能链接器 |
| **libc++** | C++ 标准库实现 |
| **compiler-rt** | 运行时库（Sanitizers、builtins） |
| **LLVM tools** | opt、llc、llvm-as 等工具集 |
| **MLIR** | 多层中间表示框架 |
| **LLDB** | 调试器 |

### 1.4 适用场景

| 场景 | 说明 | 推荐度 |
|------|------|--------|
| 编译器开发 | 构建新语言的编译器 | ⭐⭐⭐⭐⭐ |
| 代码分析工具 | 静态分析、代码检查 | ⭐⭐⭐⭐⭐ |
| 跨平台编译 | 多目标平台代码生成 | ⭐⭐⭐⭐⭐ |
| JIT 编译 | 即时编译和执行 | ⭐⭐⭐⭐ |
| 代码优化 | 高级优化和转换 | ⭐⭐⭐⭐ |
| 嵌入式开发 | 多架构后端支持 | ⭐⭐⭐⭐ |

### 1.5 对比分析

| 特性 | LLVM/Clang | GCC | MSVC |
|------|------------|-----|------|
| 编译速度 | 快 | 中 | 中 |
| 错误信息 | 详细友好 | 标准 | 标准 |
| 模块化设计 | 高 | 低 | 低 |
| 跨平台支持 | 优秀 | 良好 | Windows |
| IR 设计 | SSA | RTL | 内部 IR |
| 工具链统一度 | 统一 | 分散 | 集成 |
| Sanitizers | 完善 | 完善 | 部分 |
| C++ 标准支持 | 快速 | 快速 | 中等 |

## 2. 安装与配置

### 2.1 多平台安装

**macOS 安装**

```bash
# 使用 Homebrew
brew install llvm

# 安装特定版本
brew install llvm@15
brew install llvm@16
```

**Linux 安装**

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install llvm llvm-dev clang lld

# 安装完整工具链
sudo apt install llvm clang lld lldb libc++-dev libc++abi-dev

# CentOS/RHEL
sudo yum install llvm llvm-devel clang lld

# Fedora
sudo dnf install llvm llvm-devel clang lld
```

**Windows 安装**

```powershell
# 使用 Chocolatey
choco install llvm

# 或下载预编译包
# https://releases.llvm.org/download.html
```

### 2.2 版本管理

```bash
# 查看当前版本
llvm-config --version
clang --version

# 查看安装路径
llvm-config --prefix

# 查看包含路径
llvm-config --includedir

# 查看库路径
llvm-config --libdir
```

### 2.3 环境配置

**环境变量设置**

```bash
# macOS (bash)
export PATH="/usr/local/opt/llvm/bin:$PATH"
export LDFLAGS="-L/usr/local/opt/llvm/lib"
export CPPFLAGS="-I/usr/local/opt/llvm/include"

# Linux (bash)
export PATH="/usr/lib/llvm-15/bin:$PATH"
export LD_LIBRARY_PATH="/usr/lib/llvm-15/lib:$LD_LIBRARY_PATH"
```

### 2.4 验证安装

```bash
# 验证 clang
clang --version

# 验证 llvm-config
llvm-config --version

# 测试编译
echo 'int main() { return 0; }' > test.c
clang test.c -o test
./test && echo "LLVM 安装成功"
```

## 3. 基础使用

### 3.1 快速入门

**编译简单程序**

```bash
# 编译 C 程序
clang program.c -o program

# 编译 C++ 程序
clang++ program.cpp -o program

# 使用特定标准
clang -std=c11 program.c -o program
clang++ -std=c++20 program.cpp -o program

# 优化级别
clang -O0 program.c    # 无优化，用于调试
clang -O1 program.c    # 基本优化
clang -O2 program.c    # 标准优化
clang -O3 program.c    # 高级优化
clang -Os program.c    # 体积优化
clang -Oz program.c    # 极致体积优化
```

### 3.2 编译流程详解

LLVM 编译流程从源代码到可执行文件的完整路径：

```
源代码 (C/C++)
    ↓
预处理 (cpp)
    ↓
Clang 前端（词法/语法分析）
    ↓
LLVM IR（优化前）
    ↓
opt（优化 Pass）
    ↓
LLVM IR（优化后）
    ↓
llc（后端代码生成）
    ↓
目标代码（汇编/机器码）
    ↓
LLD 链接器
    ↓
可执行文件
```

### 3.3 LLVM IR 中间表示

**IR 格式**

LLVM IR 有三种表示形式：

| 格式 | 文件扩展名 | 说明 |
|------|-----------|------|
| 文本格式 | .ll | 人类可读的汇编形式 |
| 位码格式 | .bc | 二进制格式，紧凑高效 |
| 内存格式 | - | 编译器内部使用的内存表示 |

**生成 IR**

```bash
# 生成文本格式 IR
clang -emit-llvm -S program.c -o program.ll

# 生成位码格式 IR
clang -emit-llvm program.c -o program.bc

# 从 IR 编译到可执行文件
clang program.ll -o program

# IR 到汇编
llc program.ll -o program.s
```

**IR 示例**

```llvm
; 简单的加法函数
define i32 @add(i32 %a, i32 %b) {
entry:
    %result = add i32 %a, %b
    ret i32 %result
}

; 主函数调用示例
define i32 @main() {
entry:
    %call = call i32 @add(i32 1, i32 2)
    ret i32 %call
}

; 使用数组的示例
@.str = private constant [13 x i8] c"Hello World\00"

define i32 @main() {
entry:
    %ptr = getelementptr [13 x i8], [13 x i8]* @.str, i64 0, i64 0
    call i32 @puts(i8* %ptr)
    ret i32 0
}

declare i32 @puts(i8*)
```

**IR 数据类型**

| 类型 | 说明 | 示例 |
|------|------|------|
| `i1` | 1 位整数（布尔） | `i1 true` |
| `i8` | 8 位整数 | `i8 255` |
| `i32` | 32 位整数 | `i32 42` |
| `i64` | 64 位整数 | `i64 1000000` |
| `float` | 32 位浮点 | `float 3.14` |
| `double` | 64 位浮点 | `double 3.14159` |
| `void` | 无类型 | 返回类型 |

### 3.4 LLVM 工具链

**clang - 前端编译器**

```bash
# 基础编译
clang program.c -o program

# 生成预处理文件
clang -E program.c -o program.i

# 生成汇编文件
clang -S program.c -o program.s

# 只编译不链接
clang -c program.c -o program.o

# 显示所有警告
clang -Wall program.c

# 将警告视为错误
clang -Werror program.c
```

**llc - IR 到汇编**

```bash
# IR 到汇编
llc program.ll -o program.s

# 指定目标架构
llc -march=x86-64 program.ll -o program.s
llc -march=arm program.ll -o program.s
llc -march=aarch64 program.ll -o program.s

# 指定优化级别
llc -O3 program.ll -o program.s
```

**opt - IR 优化器**

```bash
# 运行所有优化
opt -O3 program.ll -o optimized.bc

# 运行特定优化 Pass
opt -mem2reg program.ll -o optimized.bc
opt -gvn program.ll -o optimized.bc
opt -inline program.ll -o optimized.bc

# 查看所有可用 Pass
opt --help-hidden | grep "pass"

# 生成优化后的文本 IR
opt -O3 -S program.ll -o optimized.ll
```

**其他核心工具**

| 工具 | 说明 | 示例 |
|------|------|------|
| llvm-as | 文本 IR 转位码 | `llvm-as program.ll -o program.bc` |
| llvm-dis | 位码转文本 IR | `llvm-dis program.bc -o program.ll` |
| lli | IR 解释执行 | `lli program.bc` |
| llvm-link | IR 文件链接 | `llvm-link a.bc b.bc -o linked.bc` |
| llvm-nm | 符号表查看 | `llvm-nm program.o` |
| llvm-objdump | 反汇编 | `llvm-objdump -d program.o` |
| llvm-strip | 移除符号 | `llvm-strip program.o` |
| llvm-config | 配置信息 | `llvm-config --cxxflags` |

## 4. 进阶特性

### 4.1 优化 Pass 详解

**常用优化 Pass**

| Pass | 说明 | 使用场景 |
|------|------|---------|
| `-mem2reg` | 内存到寄存器 | 提升 SSA 形式 |
| `-deadargelim` | 删除死参数 | 函数签名优化 |
| `-dce` | 死代码消除 | 删除无用代码 |
| `-adce` | 激进死代码消除 | 更彻底的清理 |
| `-gvn` | 全局值编号 | 消除冗余计算 |
| `-licm` | 循环不变代码外提 | 循环优化 |
| `-inline` | 函数内联 | 减少调用开销 |
| `-constprop` | 常量传播 | 编译期计算 |
| `-instcombine` | 指令合并 | 简化指令序列 |
| `-loop-unroll` | 循环展开 | 减少循环开销 |

**优化级别说明**

| 级别 | 说明 | 适用场景 |
|------|------|---------|
| `-O0` | 无优化 | 调试阶段 |
| `-O1` | 基本优化 | 快速编译 |
| `-O2` | 标准优化 | 发布版本 |
| `-O3` | 高级优化 | 性能优先 |
| `-Os` | 体积优化 | 存储受限 |
| `-Oz` | 极致体积优化 | 嵌入式系统 |

### 4.2 Sanitizers 运行时检测

**AddressSanitizer (ASan)**

```bash
# 检测内存错误
clang -fsanitize=address -g program.c -o program

# ASan 可检测的问题
# - 堆缓冲区溢出
# - 栈缓冲区溢出
# - 全局缓冲区溢出
# - Use-after-free
# - Double-free
# - Memory leaks
```

**ThreadSanitizer (TSan)**

```bash
# 检测数据竞争
clang -fsanitize=thread -g program.c -o program

# TSan 可检测的问题
# - 数据竞争
# - 死锁
```

**MemorySanitizer (MSan)**

```bash
# 检测未初始化内存读取
clang -fsanitize=memory -g program.c -o program
```

**UndefinedBehaviorSanitizer (UBSan)**

```bash
# 检测未定义行为
clang -fsanitize=undefined -g program.c -o program

# UBSan 可检测的问题
# - 整数溢出
# - 空指针解引用
# - 除以零
# - 数组越界
```

### 4.3 编写自定义 Pass

**Pass 基本结构**

```cpp
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct HelloPass : public FunctionPass {
    static char ID;
    HelloPass() : FunctionPass(ID) {}
    
    bool runOnFunction(Function &F) override {
        errs() << "Hello: " << F.getName() << "\n";
        return false;  // 未修改函数
    }
};
}

char HelloPass::ID = 0;
static RegisterPass<HelloPass> X("hello", "Hello World Pass");
```

**CMake 配置**

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyPass)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_library(MyPass MODULE
    MyPass.cpp
)

target_link_libraries(MyPass
    LLVMCore
    LLVMSupport
)

# 设置输出后缀
set_target_properties(MyPass PROPERTIES PREFIX "")
```

## 5. 性能优化

### 5.1 编译性能优化

**并行编译**

```bash
# 使用多核编译
make -j$(nproc)

# 使用 Ninja 构建系统
cmake -G Ninja ..
ninja
```

**编译缓存**

```bash
# 使用 ccache 加速编译
export CC="ccache clang"
export CXX="ccache clang++"

# 或在 CMake 中配置
cmake -DCMAKE_C_COMPILER_LAUNCHER=ccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache ..
```

**预编译头文件**

```cmake
# CMake 配置预编译头
target_precompile_headers(myapp PRIVATE
    <vector>
    <string>
    <map>
)
```

### 5.2 运行时性能优化

**Profile-Guided Optimization (PGO)**

```bash
# 第一步：编译带插桩的程序
clang -fprofile-instr-generate program.c -o program

# 第二步：运行程序收集数据
./program
llvm-profdata merge -o default.profdata *.profraw

# 第三步：使用 Profile 数据重新编译
clang -fprofile-instr-use=default.profdata program.c -o program_optimized
```

**Link-Time Optimization (LTO)**

```bash
# 启用 LTO
clang -flto program.c -o program

# 完整 LTO
clang -flto=full program.c -o program

# 精简 LTO（更快，内存更少）
clang -flto=thin program.c -o program
```

## 6. 问题排查

### 6.1 常见问题

**版本冲突**

```bash
# 检查版本一致性
clang --version
llvm-config --version

# 确保使用同一版本
which clang
which llvm-config
```

**库路径问题**

```bash
# 检查库路径
llvm-config --libdir
ldconfig -p | grep llvm

# 设置库路径
export LD_LIBRARY_PATH=/usr/lib/llvm-15/lib:$LD_LIBRARY_PATH
```

**头文件路径**

```bash
# 查看包含路径
clang -E -x c - -v < /dev/null

# 显式指定路径
clang -I/usr/include/llvm-15 program.c
```

### 6.2 调试技巧

**查看编译过程**

```bash
# 详细输出编译过程
clang -v program.c

# 查看预处理结果
clang -E program.c

# 查看 AST
clang -Xclang -ast-dump program.c

# 查看 IR（每个 Pass 后）
clang -mllvm -print-after-all program.c
```

**调试符号**

```bash
# 生成调试符号
clang -g program.c -o program

# 使用 LLDB 调试
lldb program
(lldb) run
(lldb) bt
```

## 7. 集成实践

### 7.1 CMake 集成

**基础配置**

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject)

# 查找 LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# 设置包含路径
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# 链接 LLVM 库
llvm_map_components_to_libnames(llvm_libs
    core
    support
    irreader
    passes
)

add_executable(myapp main.cpp)
target_link_libraries(myapp ${llvm_libs})
```

**高级配置**

```cmake
# 使用 LLVM 工具链
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# 启用 LTO
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto=thin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=thin")

# 启用 Sanitizer
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
if(ENABLE_ASAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()
```

### 7.2 CI/CD 配置

**GitHub Actions 示例**

```yaml
name: Build with LLVM

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install LLVM
      run: |
        sudo apt update
        sudo apt install llvm clang lld
    
    - name: Configure
      run: |
        mkdir build && cd build
        cmake -DCMAKE_C_COMPILER=clang \
              -DCMAKE_CXX_COMPILER=clang++ \
              ..
    
    - name: Build
      run: cmake --build build -- -j$(nproc)
    
    - name: Test
      run: |
        cd build
        ctest --output-on-failure
```

### 7.3 实战案例

**代码静态分析工具**

```cpp
// 使用 Clang AST 进行静态分析
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class FindFuncVisitor
    : public RecursiveASTVisitor<FindFuncVisitor> {
public:
    explicit FindFuncVisitor(ASTContext *Context) : Context(Context) {}
    
    bool VisitFunctionDecl(FunctionDecl *Declaration) {
        if (Context->getSourceManager().isInMainFile(
            Declaration->getLocation())) {
            llvm::outs() << "Found function: "
                        << Declaration->getNameInfo().getName()
                        << "\n";
        }
        return true;
    }

private:
    ASTContext *Context;
};
```

**自定义优化 Pass**

```cpp
// 实现常量折叠优化
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"

using namespace llvm;

struct ConstantFoldingPass : public FunctionPass {
    static char ID;
    ConstantFoldingPass() : FunctionPass(ID) {}
    
    bool runOnFunction(Function &F) override {
        bool Changed = false;
        
        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {
                if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
                    // 检查操作数是否为常量
                    if (ConstantInt *C1 = dyn_cast<ConstantInt>(BinOp->getOperand(0))) {
                        if (ConstantInt *C2 = dyn_cast<ConstantInt>(BinOp->getOperand(1))) {
                            // 执行常量折叠
                            Constant *Result = ConstantExpr::get(
                                BinOp->getOpcode(), C1, C2);
                            BinOp->replaceAllUsesWith(Result);
                            Changed = true;
                        }
                    }
                }
            }
        }
        return Changed;
    }
};

char ConstantFoldingPass::ID = 0;
static RegisterPass<ConstantFoldingPass> X(
    "constfold", "Constant Folding Pass");
```

## 8. 参考资源

### 8.1 官方文档

- LLVM 官网：https://llvm.org
- LLVM 文档：https://llvm.org/docs/
- Clang 文档：https://clang.llvm.org/docs/
- LLVM IR 参考：https://llvm.org/docs/LangRef.html
- libc++ 文档：https://libcxx.llvm.org
- compiler-rt 文档：https://compiler-rt.llvm.org

### 8.2 学习路径

**初级阶段**
1. 学习 Clang 基本编译命令
2. 理解 LLVM IR 基础
3. 使用 llvm-* 工具链
4. 掌握基本优化选项

**中级阶段**
1. 学习编写 LLVM Pass
2. 理解 IR 结构和语义
3. 使用 Sanitizers 进行调试
4. 掌握 PGO 和 LTO 优化

**高级阶段**
1. 开发自定义前端
2. 实现新的优化 Pass
3. 后端代码生成
4. JIT 编译器开发

### 8.3 社区资源

- LLVM GitHub：https://github.com/llvm/llvm-project
- LLVM 邮件列表：https://lists.llvm.org
- LLVM Discord：开发者社区
- LLVM 年会：LLVM Developers Meeting