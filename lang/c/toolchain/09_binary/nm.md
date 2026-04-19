# nm - 符号表查看工具

## 1. 概述与背景

### 1.1 工具定位

nm 是 GNU 二进制工具集（binutils）提供的符号表查看工具，用于分析目标文件、可执行文件和库文件中的符号信息。作为连接器与调试器之间的重要桥梁，nm 在二进制分析、链接错误排查和逆向工程中扮演关键角色。

符号表是目标文件的元数据核心，记录了：
- 全局变量和函数的名称与地址
- 外部依赖的未定义符号
- 符号类型和绑定属性
- 符号大小和所在段信息

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1986 | 1.0 | 随 binutils 发布，支持基本符号查看 |
| 1991 | 2.x | 支持 ELF 格式，增加动态符号选项 |
| 2000 | 2.10 | 添加 C++ 符号解码（-C 选项） |
| 2010 | 2.20 | 增强 POSIX 格式输出（-P） |
| 2018 | 2.31 | 改进符号大小显示，支持 LTO 对象 |
| 2023 | 2.41 | 支持新的目标文件格式，性能优化 |

### 1.3 核心特性

**符号信息提取**：
- 列出所有符号（全局、本地、外部）
- 显示符号地址和大小
- 标识符号类型和绑定属性
- 支持静态符号和动态符号

**格式与输出**：
- 标准 BSD 格式输出
- POSIX 兼容格式（脚本友好）
- C++ 符号名称解码
- 符号排序和过滤

**兼容性**：
- 支持 ELF、COFF、Mach-O 等多种格式
- 跨平台支持（Linux、macOS、Windows）
- 与其他 binutils 工具无缝协作

### 1.4 适用场景

| 场景 | 用途 | 命令示例 |
|------|------|----------|
| 链接错误排查 | 查找未定义符号 | `nm -u program.o` |
| 库接口分析 | 查看导出符号 | `nm -D libmylib.so` |
| 代码膨胀分析 | 查看符号大小 | `nm -S --size-sort program` |
| 符号冲突定位 | 查找重复定义 | `nm *.o \| grep ' T '` |
| 逆向工程 | 函数地址定位 | `nm program \| grep func` |
| 性能优化 | 大符号识别 | `nm -S program \| sort -k2` |

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| nm | 简洁输出、快速查看、脚本友好 | 信息相对简略 |
| readelf -s | 信息详细、显示完整符号表属性 | 输出复杂、可读性较低 |
| objdump -t | 与反汇编配合、信息全面 | 格式较冗长 |
| objdump -T | 专门用于动态符号 | 功能单一 |

**选择建议**：
- 快速符号查找 → nm
- 详细符号分析 → readelf -s
- 动态库符号 → nm -D 或 objdump -T
- 与反汇编配合 → objdump -t

## 2. 安装与配置

### 2.1 多平台安装

**Linux（Debian/Ubuntu）**：
```bash
sudo apt update
sudo apt install binutils
```

**Linux（CentOS/RHEL）**：
```bash
sudo yum install binutils
# 或
sudo dnf install binutils
```

**macOS**：
```bash
# 系统自带 nm（Xcode 工具链）
xcode-select --install

# GNU nm（通过 Homebrew）
brew install binutils
# 使用：glnm 或将 /opt/homebrew/opt/binutils/bin 加入 PATH
```

**Windows（MSYS2）**：
```bash
pacman -S mingw-w64-x86_64-binutils
```

### 2.2 版本管理

**检查版本**：
```bash
nm --version
# 输出示例：
# GNU nm (GNU Binutils) 2.41
# Copyright (C) 2023 Free Software Foundation, Inc.
```

**多版本管理**：
```bash
# 查看已安装的 binutils 版本
dpkg -l | grep binutils

# 安装特定版本（Ubuntu）
sudo apt install binutils=2.38-3ubuntu1
```

### 2.3 环境配置

**PATH 配置**：
```bash
# 添加自定义 binutils 路径
export PATH="/usr/local/binutils/bin:$PATH"

# macOS Homebrew binutils
export PATH="/opt/homebrew/opt/binutils/bin:$PATH"
```

**别名设置**（可选）：
```bash
# ~/.bashrc 或 ~/.zshrc
alias nm='nm -C'      # 默认解码 C++ 符号
alias nmd='nm -D'     # 动态符号
alias nms='nm -S'     # 显示大小
```

### 2.4 验证安装

```bash
# 检查命令可用性
which nm
# 输出：/usr/bin/nm

# 查看支持的目标格式
nm --help | grep -A 20 "Target selection"

# 测试基本功能
echo 'int main() { return 0; }' | gcc -x c -c - -o /tmp/test.o
nm /tmp/test.o
# 预期输出包含 main 符号
```

## 3. 基础使用

### 3.1 快速入门

**最简示例**：
```bash
# 创建测试程序
cat > /tmp/test.c << 'EOF'
#include <stdio.h>

int global_var = 42;
static int static_var;

int add(int a, int b) {
    return a + b;
}

int main() {
    printf("%d\n", add(1, 2));
    return 0;
}
EOF

# 编译
gcc /tmp/test.c -o /tmp/test

# 查看符号
nm /tmp/test
```

输出示例：
```
0000000000001149 T add
0000000000001170 T main
                 U printf
0000000000004010 B global_var
0000000000004014 b static_var
```

### 3.2 输出格式解读

nm 输出由三列组成：

```
[地址]        [类型] [名称]
0000000000001149  T    add
```

**地址**：
- 十六进制虚拟地址
- 未定义符号地址为空或 0

**类型字符**（大小写区分）：
- 大写字母：全局符号
- 小写字母：本地符号

**名称**：
- 符号名称（C 语言直接显示）
- C++ 符号可能需要解码

### 3.3 基本命令

**查看可执行文件符号**：
```bash
nm program

# 仅显示外部符号
nm -g program
# 或
nm --extern-only program

# 仅显示未定义符号
nm -u program
```

**查看目标文件符号**：
```bash
nm object.o

# 显示符号大小
nm -S object.o

# 按地址排序
nm -n object.o
```

**查看库文件符号**：
```bash
# 静态库
nm libmylib.a

# 动态库
nm -D libmylib.so

# 仅显示已定义符号
nm -D --defined-only libmylib.so
```

### 3.4 常用操作

**符号搜索**：
```bash
# 查找特定符号
nm program | grep main

# 查找所有函数符号
nm program | grep ' T '

# 查找特定前缀的符号
nm program | grep '^_.*mylib'
```

**排序输出**：
```bash
# 按地址排序
nm -n program

# 按名称排序
nm program | sort -k3

# 按大小排序
nm -S program | sort -k2 -n

# 反向排序
nm -r program
```

**格式化输出**：
```bash
# POSIX 格式（适合脚本解析）
nm -P program

# 显示文件名前缀
nm -A file1.o file2.o
```

## 4. 进阶特性

### 4.1 符号类型详解

**代码段符号（T/t）**：
```bash
# 全局函数
nm program | grep ' T '
# 0000000000001149 T add
# 0000000000001170 T main

# 本地函数（static）
nm program | grep ' t '
# 00000000000011a0 t helper_func
```

**数据段符号（D/d/B/b/R/r）**：
```bash
# 已初始化全局变量
nm program | grep ' D '
# 0000000000004010 D global_var

# 未初始化全局变量（BSS）
nm program | grep ' B '
# 0000000000004020 B uninit_var

# 只读数据
nm program | grep ' R '
# 0000000000002000 R const_str
```

**特殊符号**：
| 类型 | 说明 | 示例 |
|------|------|------|
| U | 未定义（外部引用） | printf, malloc |
| W | 弱符号 | 可被覆盖的实现 |
| C | 普通符号 | 未初始化的全局变量 |
| A | 绝对符号 | 固定地址常量 |
| V | 弱对象符号 | 弱引用对象 |

### 4.2 C++ 符号解码

C++ 编译器会对函数名进行名称修饰（name mangling），nm 提供 `-C` 选项解码：

```bash
# 编译 C++ 程序
cat > /tmp/test.cpp << 'EOF'
namespace MyLib {
    class Logger {
    public:
        void log(int level, const char* msg);
        static Logger* getInstance();
    };
}

void MyLib::Logger::log(int level, const char* msg) {}
MyLib::Logger* MyLib::Logger::getInstance() { return nullptr; }

int main() { return 0; }
EOF

g++ /tmp/test.cpp -o /tmp/test_cpp

# 未解码
nm /tmp/test_cpp | grep Logger
# 0000000000001149 T _ZN5MyLib6Logger3logEiPKc
# 0000000000001170 T _ZN5MyLib6Logger11getInstanceEv

# 解码后
nm -C /tmp/test_cpp | grep Logger
# 0000000000001149 T MyLib::Logger::log(int, char const*)
# 0000000000001170 T MyLib::Logger::getInstance()
```

**解码格式说明**：
- `_Z` 前缀：表示 C++ 修饰名称
- `N...E`：表示命名空间或类
- 数字前缀：表示名称长度

### 4.3 动态符号分析

动态符号位于 `.dynsym` 和 `.dynstr` 段，用于动态链接：

```bash
# 查看动态库导出符号
nm -D libmylib.so

# 仅显示已定义的动态符号
nm -D --defined-only libmylib.so

# 显示动态符号与静态符号对比
nm libmylib.so > static_symbols.txt
nm -D libmylib.so > dynamic_symbols.txt
diff static_symbols.txt dynamic_symbols.txt
```

**动态符号与静态符号的区别**：
| 特性 | 静态符号表 | 动态符号表 |
|------|-----------|-----------|
| 段名称 | .symtab, .strtab | .dynsym, .dynstr |
| 内容 | 所有符号 | 仅动态链接相关符号 |
| strip 后 | 可能被移除 | 保留 |
| 用途 | 调试、静态链接 | 动态链接、运行时加载 |

### 4.4 多文件处理

**批量分析**：
```bash
# 分析多个目标文件
nm file1.o file2.o file3.o

# 显示文件名前缀
nm -A *.o

# 输出示例：
# file1.o:0000000000000000 T func1
# file2.o:0000000000000000 T func2
```

**归档库处理**：
```bash
# 创建静态库
ar rcs libmylib.a file1.o file2.o

# 查看静态库符号
nm libmylib.a

# 显示库成员索引
nm -s libmylib.a
```

## 5. 性能优化

### 5.1 大文件处理

对于大型可执行文件或库，优化 nm 性能：

```bash
# 仅查看外部符号（减少输出量）
nm -g large_program

# 仅查看动态符号（跳过静态符号表解析）
nm -D large_library.so

# 使用 POSIX 格式（减少格式化开销）
nm -P large_program
```

### 5.2 脚本优化

**并行处理多个文件**：
```bash
# 使用 xargs 并行
find . -name "*.o" | xargs -P4 -I {} nm {} > all_symbols.txt

# 使用 GNU parallel
find . -name "*.o" | parallel nm {} > all_symbols.txt
```

**高效过滤**：
```bash
# 避免 grep 二次处理
nm -u program.o    # 比 nm program.o | grep ' U ' 更快

nm -g program      # 比 nm program | grep ' [A-Z] ' 更准确
```

### 5.3 最佳实践

**常用组合命令**：
```bash
# 查找最大函数
nm -S --size-sort program | grep ' T ' | tail -n 20

# 统计各类型符号数量
nm program | awk '{print $2}' | sort | uniq -c | sort -rn

# 导出符号列表（用于 API 审计）
nm -D --defined-only libmylib.so | awk '{print $3}' | sort > exported_symbols.txt

# 查找潜在重复定义
nm *.o | grep ' T ' | awk '{print $3}' | sort | uniq -d
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：找不到符号**
```bash
# 症状
nm program.o
# 输出为空或符号很少

# 原因：编译时被 strip 或优化
# 解决：检查编译选项
gcc -O0 -g program.c -o program
```

**问题 2：符号类型显示为 `?`**
```bash
# 症状
nm object.o | grep '?'
# 0000000000000000 ? unknown_symbol

# 原因：特殊段或目标文件格式不标准
# 解决：使用 readelf 查看详细信息
readelf -s object.o | grep unknown_symbol
```

**问题 3：C++ 符号解码失败**
```bash
# 症状
nm -C program
# 仍然显示修饰后的名称

# 原因：nm 版本不支持或符号表损坏
# 解决：使用 c++filt 手动解码
nm program | c++filt
```

### 6.2 调试技巧

**链接错误排查**：
```bash
# undefined reference to 'xxx' 错误
# 1. 查找未定义符号
nm -u program.o | grep xxx

# 2. 在库中查找符号
nm -D libmylib.so | grep xxx

# 3. 检查符号可见性
nm -g libmylib.a | grep xxx
```

**符号冲突定位**：
```bash
# multiple definition of 'xxx' 错误
# 查找所有定义
nm *.o | grep ' T xxx'

# 定位具体文件
nm -A *.o | grep ' T xxx'
```

**ABI 兼容性检查**：
```bash
# 对比新旧版本库的导出符号
nm -D libmylib.so.1 > symbols_v1.txt
nm -D libmylib.so.2 > symbols_v2.txt
diff symbols_v1.txt symbols_v2.txt
```

### 6.3 与其他工具配合

**结合 grep 和 awk**：
```bash
# 提取所有函数名
nm program | grep ' T ' | awk '{print $3}' > functions.txt

# 统计各段符号大小
nm -S program | awk '{size[$2]+=$2; count[$2]++} END {for(t in size) printf "%s: %d symbols, %d bytes\n", t, count[t], size[t]}'
```

**结合 objdump**：
```bash
# 查找符号地址后反汇编
addr=$(nm program | grep ' T main' | awk '{print $1}')
objdump -d --start-address=0x$addr program
```

## 7. 集成实践

### 7.1 工具链集成

**Makefile 集成**：
```makefile
# 符号检查目标
check-symbols:
	@echo "=== Undefined symbols ==="
	nm -u $(TARGET)
	@echo "=== Large functions (>1KB) ==="
	nm -S $(TARGET) | awk '$$2 > 1000 {print $$0}'
	@echo "=== Exported symbols ==="
	nm -g $(TARGET) | grep ' T '

# 导出符号列表
export-symbols:
	nm -D --defined-only libmylib.so | awk '{print $$3}' > exports.txt
```

**CMake 集成**：
```cmake
# 添加符号检查目标
add_custom_target(check_symbols
    COMMAND nm -u ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    COMMAND nm -S ${CMAKE_BINARY_DIR}/${PROJECT_NAME} | grep " T "
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Checking symbol table"
)
```

### 7.2 CI/CD 配置

**GitHub Actions 示例**：
```yaml
name: Symbol Check

on: [push, pull_request]

jobs:
  check:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies
        run: sudo apt install binutils
      
      - name: Build
        run: gcc -o program main.c
      
      - name: Check undefined symbols
        run: |
          if nm -u program | grep -q .; then
            echo "Found undefined symbols:"
            nm -u program
            exit 1
          fi
      
      - name: Check symbol sizes
        run: |
          echo "Top 10 largest functions:"
          nm -S --size-sort program | grep ' T ' | tail -n 10
```

**GitLab CI 示例**：
```yaml
symbol-analysis:
  stage: test
  script:
    - gcc -o program main.c
    - nm -u program > undefined_symbols.txt
    - nm -S program > symbol_sizes.txt
    - |
      if [ -s undefined_symbols.txt ]; then
        echo "Warning: Undefined symbols found"
        cat undefined_symbols.txt
      fi
  artifacts:
    paths:
      - undefined_symbols.txt
      - symbol_sizes.txt
```

### 7.3 实战案例

**案例 1：动态库 API 审计**

需求：确保动态库只导出必要的符号，避免内部实现暴露。

```bash
# 步骤 1：查看所有导出符号
nm -D --defined-only libmylib.so > exported.txt

# 步骤 2：对比预期导出列表
diff exported.txt expected_exports.txt

# 步骤 3：使用符号可见性控制
# 源码中添加：
# __attribute__((visibility("hidden"))) void internal_func();
```

**案例 2：定位代码膨胀**

需求：找出占用空间最大的函数，优化代码体积。

```bash
# 分析可执行文件
nm -S --size-sort program | grep ' T ' | tail -n 20

# 输出示例：
# 00000000000031a0 00000500 T large_function
# 00000000000036a0 00000300 T medium_function

# 计算总代码段大小
nm -S program | grep ' T ' | awk '{sum += strtonum("0x"$2)} END {printf "Total: %d bytes\n", sum}'
```

**案例 3：验证静态库完整性**

需求：确保静态库包含所有必需的目标文件和符号。

```bash
# 创建静态库
ar rcs libmylib.a file1.o file2.o file3.o

# 验证成员
ar -t libmylib.a

# 验证符号
nm -s libmylib.a

# 检查缺失符号
nm file1.o file2.o file3.o > all_symbols.txt
nm libmylib.a > lib_symbols.txt
diff all_symbols.txt lib_symbols.txt
```

## 8. 参考资源

### 8.1 官方文档

- **GNU Binutils 手册**: https://sourceware.org/binutils/docs/binutils/nm.html
- **nm man page**: `man nm`
- **ELF 格式规范**: https://refspecs.linuxfoundation.org/elf/elf.pdf

### 8.2 相关工具

| 工具 | 说明 | 关系 |
|------|------|------|
| readelf | 显示 ELF 文件信息 | 详细符号表：`readelf -s` |
| objdump | 反汇编和目标文件信息 | 符号表：`objdump -t` |
| c++filt | C++ 符号解码 | nm -C 的独立工具 |
| ar | 静态库管理 | nm 可查看 ar 创建的库 |
| strip | 移除符号 | strip 后 nm 输出减少 |
| ld | 链接器 | 使用 nm 分析的符号信息 |

### 8.3 学习路径

**入门阶段**：
1. 理解符号表概念和作用
2. 掌握 nm 基本命令和输出格式
3. 学会符号类型识别

**进阶阶段**：
1. 掌握 C++ 符号解码
2. 理解动态符号与静态符号
3. 学习符号大小分析和代码优化

**高级阶段**：
1. 结合 readelf 深入分析 ELF 格式
2. 理解符号绑定和可见性
3. 掌握 ABI 兼容性分析

### 8.4 扩展阅读

- 《程序员的自我修养：链接、装载与库》- 符号解析章节
- 《链接器和加载器》- 符号表结构
- 《深入理解计算机系统》- 链接章节
- Linux 源码：include/linux/module.h（内核符号表）

### 8.5 常见问题解答

**Q: nm 和 readelf -s 有什么区别？**

A: nm 输出简洁，适合快速查看；readelf -s 显示完整符号表信息，包括段索引、绑定属性等详细字段。

**Q: 为什么 strip 后 nm 输出变少了？**

A: strip 命令移除了 .symtab 和 .strtab 段（静态符号表），但保留 .dynsym（动态符号表）。使用 `nm -D` 仍可查看动态符号。

**Q: 如何只查看函数符号？**

A: 使用 `nm program | grep ' T '` 查看全局函数，或 `nm -g program | grep ' T '` 只看全局符号中的函数。

**Q: 符号地址为 0 或空是什么意思？**

A: 地址为空或 0 表示未定义符号（类型为 U），需要从其他目标文件或库中解析。

**Q: nm 支持哪些目标文件格式？**

A: nm 支持多种格式，包括 ELF、COFF（Windows）、Mach-O（macOS）、a.out 等，自动识别格式。

---

*本文档详细介绍了 nm 工具的使用方法，从基础符号查看到高级分析技巧，涵盖日常开发和问题排查场景。*