# LLVM 基础设施

本目录收录 LLVM 编译器基础设施相关文档。

## LLVM 概述

LLVM（Low Level Virtual Machine）是一个模块化、可重用的编译器和工具链技术集合。它已成为现代编译器开发的基础设施。

## 核心组件

| 组件 | 说明 |
|------|------|
| **LLVM Core** | 编译器中间表示（IR）和优化器 |
| **Clang** | C/C++/Objective-C 前端编译器 |
| **LLDB** | 调试器 |
| [libc++](libcxx.md) | LLVM C++ 标准库实现 |
| [libstdc++](libstdcxx.md) | GNU C++ 标准库实现 |
| **compiler-rt** | 运行时库（Sanitizers 等） |
| **MLIR** | 多层中间表示 |

## LLVM 工具链

| 工具 | 说明 |
|------|------|
| `llc` | LLVM 静态编译器（IR → 汇编） |
| `opt` | LLVM 优化器 |
| `llvm-as` | LLVM 汇编器（.ll → .bc） |
| `llvm-dis` | LLVM 反汇编器（.bc → .ll） |
| `llvm-link` | LLVM 链接器 |
| `lli` | LLVM 解释器/JIT |

## 编译流程

```
源代码 (.c/.cpp)
      ↓
   Clang 前端
      ↓
LLVM IR (.ll/.bc)
      ↓
   opt 优化器
      ↓
  LLVM IR (优化后)
      ↓
   llc 后端
      ↓
  汇编代码 (.s)
      ↓
   系统汇编器
      ↓
目标文件 (.o)
      ↓
   系统链接器
      ↓
可执行文件
```

## LLVM IR 示例

```llvm
; 定义函数 add
define i32 @add(i32 %a, i32 %b) {
entry:
  %result = add i32 %a, %b
  ret i32 %result
}

; 定义 main 函数
define i32 @main() {
entry:
  %result = call i32 @add(i32 1, i32 2)
  ret i32 %result
}
```

## 相关工具文档

| 类别 | 文档 |
|------|------|
| 编译器 | [Clang](../01_compiler/clang.md) |
| 调试器 | [LLDB](../03_debug/lldb.md) |
| 格式化 | [clang-format](../04_format/clang-format.md) |
| 静态分析 | [clang-tidy](../05_static_analysis/clang-tidy.md) |
| 动态分析 | [Sanitizers](../06_dynamic_analysis/sanitizers.md) |

## 参考资源

- [LLVM 官网](https://llvm.org/)
- [LLVM 文档](https://llvm.org/docs/)
- [LLVM IR 语言参考](https://llvm.org/docs/LangRef.html)