# C 语言二进制分析工具

本目录收录 C 语言二进制分析工具文档。

## 二进制工具概览

| 工具 | 说明 | 用途 |
|------|------|------|
| [objdump](objdump.md) | 反汇编工具 | 查看汇编代码 |
| [readelf](readelf.md) | ELF 分析工具 | 分析 ELF 文件结构 |
| [nm](nm.md) | 符号表工具 | 查看符号信息 |
| [strip](strip.md) | 符号剥离工具 | 减小可执行文件体积 |
| [ldd](ldd.md) | 动态库依赖工具 | 查看程序依赖的共享库 |

## 工具对比

| 功能 | objdump | readelf | nm | strip | ldd |
|------|---------|---------|-----|-------|-----|
| 反汇编 | ✅ | ❌ | ❌ | ❌ | ❌ |
| ELF 头信息 | ✅ | ✅ | ❌ | ❌ | ❌ |
| 段信息 | ✅ | ✅ | ❌ | ❌ | ❌ |
| 符号表 | ✅ | ✅ | ✅ | ✅ (删除) | ❌ |
| 调试信息 | ✅ | ✅ | ✅ | ✅ (删除) | ❌ |
| 动态库依赖 | ✅ | ✅ | ❌ | ❌ | ✅ |

## 常用命令

### objdump - 反汇编
```bash
# 反汇编所有段
objdump -d program

# 反汇编并显示源码
objdump -d -S program

# 显示所有头信息
objdump -x program

# 显示段内容
objdump -s -j .rodata program
```

### readelf - ELF 分析
```bash
# 显示 ELF 头
readelf -h program

# 显示段头表
readelf -S program

# 显示符号表
readelf -s program

# 显示动态段
readelf -d program

# 显示程序头
readelf -l program
```

### nm - 符号表
```bash
# 列出所有符号
nm program

# 显示符号定义位置
nm -S program

# 仅显示外部符号
nm -g program

# 显示动态符号
nm -D program
```

### strip - 符号剥离
```bash
# 剥离所有符号
strip program

# 仅剥离调试符号
strip --strip-debug program

# 保留文件备份
strip program -o program_stripped
```

### ldd - 动态库依赖
```bash
# 查看程序依赖
ldd program

# 详细输出
ldd -v program

# 检查依赖是否缺失
ldd program | grep "not found"
```

## ELF 文件结构

```
+------------------+
|    ELF Header    |
+------------------+
| Program Headers  |
+------------------+
|    .text 段      |
+------------------+
|    .rodata 段    |
+------------------+
|    .data 段      |
+------------------+
|    .bss 段       |
+------------------+
| Section Headers  |
+------------------+
```

## 实用场景

| 场景 | 工具组合 |
|------|----------|
| 查看汇编代码 | `objdump -d` |
| 分析动态库依赖 | `ldd` 或 `readelf -d` |
| 定位符号定义 | `nm` + `grep` |
| 优化可执行文件大小 | `strip` |
| 调试段布局 | `readelf -S` |
| 排查库缺失问题 | `ldd` |

## 相关文档

- [编译器](../01_compiler/) - GCC、Clang 等
- [调试器](../03_debug/) - GDB、LLDB 等