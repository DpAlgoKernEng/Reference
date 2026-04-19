# C++ 编译器

本目录收录主流 C++ 编译器文档。

## 编译器概览

| 编译器 | 平台 | 说明 |
|--------|------|------|
| [GCC](gcc.md) | 跨平台 | GNU 编译器集合，开源标准 |
| [Clang](clang.md) | 跨平台 | LLVM 前端，诊断优秀 |
| [MSVC](msvc.md) | Windows | 微软 Visual C++ 编译器 |
| [Intel C++](intel.md) | 跨平台 | Intel 芯片优化编译器 |

## 编译器对比

| 特性 | GCC | Clang | MSVC |
|------|-----|-------|------|
| 开源 | ✅ GPL | ✅ Apache 2.0 | ❌ 闭源 |
| 跨平台 | ✅ | ✅ | ❌ Windows only |
| 编译速度 | 中 | 快 | 快 |
| 错误诊断 | 一般 | 优秀 | 良好 |
| C++23 支持 | 部分 | 部分 | 部分 |

## 编译流程

```
预处理 (.i/.ii) → 编译 (.s) → 汇编 (.o/.obj) → 链接 (可执行文件)
```

## 常用编译选项

### GCC/Clang 通用选项

```bash
# 指定 C++ 标准
-std=c++17 -std=c++20 -std=c++23

# 警告级别
-Wall -Wextra -Wpedantic -Werror

# 优化级别
-O0 -O1 -O2 -O3 -Os -Ofast

# 调试信息
-g -g3 -ggdb

# 定义宏
-DDEBUG -DNDEBUG
```

### MSVC 选项

```bash
# C++ 标准
/std:c++17 /std:c++20 /std:c++latest

# 警告级别
/W1 /W2 /W3 /W4 /Wall

# 优化
/Od /O1 /O2 /Ox

# 调试
/Zi /Z7
```

## 相关文档

- [构建系统](../00_build/) - CMake、Make、Ninja 等
- [调试器](../03_debug/) - GDB、LLDB 等