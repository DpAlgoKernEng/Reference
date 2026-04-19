# nm - 符号表查看工具

## 1. 概述与背景

### 1.1 工具定位

nm 是 GNU binutils 工具链提供的符号表查看工具，用于列出目标文件（.o）、可执行文件、静态库（.a）和动态库（.so）中的符号信息。它是 C/C++ 开发中分析符号依赖、排查链接错误、理解程序结构的重要工具。

符号表记录了程序中所有函数、全局变量、常量等标识符的名称、类型、大小和地址信息，nm 通过解析二进制文件的符号表，为开发者提供清晰的符号视图。

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1986 | 1.0 | 随 GNU binutils 发布，支持基本符号查看 |
| 1991 | 2.x | 支持 ELF 格式，增加 C++ 符号解码 |
| 2000 | 2.10 | 支持动态符号表，增加 POSIX 输出格式 |
| 2010 | 2.20 | 增强 64 位支持，改进符号大小显示 |
| 2020 | 2.35 | 支持 LTO 符号，增强 Mach-O 支持 |

### 1.3 核心特性

- **多格式支持**：支持 ELF、COFF、Mach-O 等多种二进制格式
- **符号分类**：自动识别代码段、数据段、未定义符号等类型
- **C++ 解码**：将 mangled 符号还原为可读形式
- **动态符号**：查看动态库的导出/导入符号
- **大小统计**：显示符号占用空间，便于优化分析
- **灵活输出**：支持按地址、名称排序，POSIX 格式输出

### 1.4 适用场景

| 场景 | 用途 |
|------|------|
| 链接错误排查 | 定位未定义符号、重复定义 |
| 库依赖分析 | 查看动态库导出符号 |
| 符号冲突调试 | 识别符号类型和来源 |
| 代码优化 | 分析函数大小、识别大符号 |
| ABI 分析 | 检查库的导出接口 |
| 逆向工程 | 了解程序结构和符号信息 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 | 适用场景 |
|------|------|------|---------|
| nm | 简洁快速、POSIX 标准输出 | 信息相对简略 | 日常符号查找 |
| readelf -s | 信息详细、显示完整符号表 | 输出较长 | 深度分析 ELF |
| objdump -t | 支持多种格式、与反汇编配合 | 输出格式复杂 | 综合分析 |
| c++filt | 专注 C++ 解码、可管道使用 | 仅解码不查看 | C++ 符号解码 |

## 2. 安装与配置

### 2.1 多平台安装

**Linux (Debian/Ubuntu):**

```bash
sudo apt update
sudo apt install binutils

# 验证安装
nm --version
```

**Linux (CentOS/RHEL):**

```bash
sudo yum install binutils
# 或
sudo dnf install binutils
```

**macOS:**

```bash
# macOS 自带 nm（LLVM 版本）
nm --version

# 安装 GNU nm
brew install binutils

# GNU nm 需要使用完整路径
/opt/homebrew/opt/binutils/bin/nm --version
```

**Windows (MinGW/MSYS2):**

```bash
pacman -S mingw-w64-x86_64-binutils
```

### 2.2 版本管理

```bash
# 查看版本
nm --version

# 检查 binutils 完整版本信息
ld --version

# 确认 nm 属于哪个包
dpkg -S $(which nm)  # Debian/Ubuntu
rpm -qf $(which nm)  # CentOS/RHEL
```

### 2.3 环境配置

nm 通常无需特殊配置，直接使用即可。但可配置 PATH 以使用特定版本：

```bash
# 使用 GNU nm（macOS）
export PATH="/opt/homebrew/opt/binutils/bin:$PATH"

# 添加别名简化常用命令
alias nms='nm -S'      # 显示大小
alias nmg='nm -g'      # 仅外部符号
alias nmc='nm -C'      # C++ 解码
```

### 2.4 验证安装

```bash
# 检查 nm 命令是否可用
which nm

# 验证基本功能
echo 'int main() { return 0; }' > test.c
gcc -c test.c
nm test.o
# 预期输出: 0000000000000000 T main

# 清理测试文件
rm -f test.c test.o
```

## 3. 基础使用

### 3.1 快速入门

nm 的基本使用流程：

```bash
# 1. 查看可执行文件符号
nm /bin/ls

# 2. 查看目标文件符号
nm myprogram.o

# 3. 查看静态库符号
nm libmylib.a

# 4. 查看动态库符号
nm -D libmylib.so
```

### 3.2 输出格式解析

nm 输出格式为三列：

```
地址           类型  符号名
0000000000001132 T    add
0000000000001149 T    main
                 U    printf
```

**地址**：符号在内存中的地址（未定义符号显示为空或 0）

**类型**：单字符表示符号类型和绑定属性

**符号名**：符号的名称（C++ 为 mangled 名称）

### 3.3 符号类型详解

#### 3.3.1 代码段符号

| 类型 | 说明 | 示例 |
|------|------|------|
| `T` | 全局代码段符号（函数） | `T main` |
| `t` | 本地代码段符号（static 函数） | `t helper_func` |
| `W` | 弱符号（可被覆盖） | `W weak_func` |

#### 3.3.2 数据段符号

| 类型 | 说明 | 示例 |
|------|------|------|
| `D` | 全局已初始化数据 | `D global_var` |
| `d` | 本地已初始化数据 | `d static_var` |
| `B` | 全局未初始化数据 | `B uninit_var` |
| `b` | 本地未初始化数据 | `b local_bss` |
| `R` | 只读数据 | `R const_data` |
| `r` | 本地只读数据 | `r local_const` |

#### 3.3.3 特殊符号

| 类型 | 说明 | 示例 |
|------|------|------|
| `U` | 未定义符号（外部引用） | `U printf` |
| `V` | 弱对象符号 | `V weak_obj` |
| `C` | 普通符号（未初始化） | `C common_var` |
| `S` | 小数据段符号 | `S small_data` |
| `A` | 绝对符号（地址固定） | `A absolute_sym` |
| `N` | 调试符号 | `N debug_info` |
| `?` | 未知类型 | `? unknown` |

### 3.4 常用操作

**基本查看：**

```bash
# 列出所有符号
nm program

# 显示符号大小
nm -S program

# 按地址排序
nm -n program

# 按名称排序（默认）
nm program

# 反向排序
nm -r program
```

**过滤符号：**

```bash
# 仅显示外部符号
nm -g program

# 仅显示未定义符号
nm -u program

# 仅显示已定义符号
nm --defined-only program

# 显示动态符号
nm -D libmylib.so
```

**符号查找：**

```bash
# 查找特定符号
nm program | grep main

# 查找所有函数符号
nm program | grep ' T '

# 查找所有全局变量
nm program | grep ' [DBR] '

# 查找未定义符号
nm program | grep ' U '
```

## 4. 进阶特性

### 4.1 C++ 符号解码

C++ 编译器会对符号名进行 mangle（名称修饰），nm 提供 -C 选项进行解码：

```bash
# 显示原始 mangled 名称
nm mycpp_program
# 输出: 0000000000001132 T _ZN3Logger3logEiPKc

# 解码 C++ 符号
nm -C mycpp_program
# 输出: 0000000000001132 T Logger::log(int, const char*)
```

**常见 C++ 符号模式：**

```
_ZN7MyClassC1Ev     → MyClass::MyClass()  (构造函数)
_ZN7MyClassD1Ev     → MyClass::~MyClass() (析构函数)
_ZN7MyClass4funcEi  → MyClass::func(int)
_Z3addii            → add(int, int)
```

### 4.2 动态符号分析

动态库的符号信息存储在动态符号表中，使用 -D 选项查看：

```bash
# 查看动态库导出符号
nm -D libmylib.so

# 仅显示已定义的导出符号
nm -D --defined-only libmylib.so

# 仅显示未定义的导入符号
nm -D --undefined-only libmylib.so
```

**输出示例：**

```
0000000000001132 T add              # 导出函数
0000000000001149 T multiply         # 导出函数
                 U __stack_chk_fail # 导入函数
                 U printf           # 导入函数
```

### 4.3 符号大小分析

使用 -S 选项显示符号大小，用于识别大函数或变量：

```bash
nm -S program

# 按大小排序（从小到大）
nm -S --size-sort program

# 找出最大的 10 个符号
nm -S --size-sort program | tail -n 10

# 找出最大的函数
nm -S program | grep ' T ' | sort -k2 -r | head -n 10
```

**输出格式：**

```
地址            大小        类型  符号名
0000000000001132 00000012 T    add          # 18 字节
0000000000001149 0000002a T    main         # 42 字节
```

### 4.4 POSIX 格式输出

使用 -P 选项生成易于脚本解析的 POSIX 格式：

```bash
nm -P program
```

**输出格式：**

```
符号名 类型 值 大小
add T 0000000000001132 00000012
main T 0000000000001149 0000002a
printf U 0000000000000000
```

**脚本解析示例：**

```bash
# 提取所有函数名和地址
nm -P program | awk '$2=="T" {print $1, $3}'

# 统计各类型符号数量
nm -P program | awk '{count[$2]++} END {for(t in count) print t, count[t]}'
```

### 4.5 多文件处理

```bash
# 处理多个文件
nm file1.o file2.o file3.o

# 显示文件名前缀
nm -A file1.o file2.o

# 输出示例
file1.o: 0000000000000000 T func1
file2.o: 0000000000000000 T func2
```

### 4.6 静态库分析

静态库（.a 文件）是多个目标文件的集合：

```bash
# 查看静态库成员
nm libmylib.a

# 显示成员文件名
nm -A libmylib.a

# 查看静态库索引
nm -s libmylib.a
```

**输出示例：**

```
Archive index:
func1 in file1.o
func2 in file2.o

file1.o:
0000000000000000 T func1

file2.o:
0000000000000000 T func2
```

## 5. 实用技巧

### 5.1 链接错误排查

**未定义符号检查：**

```bash
# 找出所有未定义符号
nm -u myprogram.o

# 检查是否在库中定义
nm libmylib.a | grep 'function_name'

# 检查动态库依赖
ldd myprogram
nm -D libmylib.so | grep 'function_name'
```

**重复定义检查：**

```bash
# 查找重复的全局符号
nm *.o | grep ' T ' | awk '{print $3}' | sort | uniq -d

# 查找重复的全局变量
nm *.o | grep ' [DBR] ' | awk '{print $3}' | sort | uniq -d
```

### 5.2 符号统计

```bash
# 按类型统计符号数量
nm program | cut -c10 | sort | uniq -c | sort -rn

# 统计各类符号
echo "Functions: $(nm program | grep ' T ' | wc -l)"
echo "Variables: $(nm program | grep ' [DBR] ' | wc -l)"
echo "Undefined: $(nm program | grep ' U ' | wc -l)"

# 统计代码段大小
nm -S program | grep ' T ' | awk '{sum += strtonum("0x"$2)} END {print sum}'
```

### 5.3 符号过滤技巧

```bash
# 仅显示用户定义符号（排除库符号）
nm program | grep -v '_Z' | grep -v '__'

# 显示所有本地符号（static 函数/变量）
nm program | grep ' [tdbr] '

# 显示所有全局符号
nm program | grep ' [TDBR] '

# 显示弱符号（可能被覆盖的符号）
nm program | grep ' [WwVv] '
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：符号显示为未定义但库已链接**

```bash
# 检查库中是否有该符号
nm -D libmylib.so | grep symbol_name

# 检查符号类型和可见性
readelf -s libmylib.so | grep symbol_name

# 可能原因：
# 1. 符号被声明为 static
# 2. 符号被 visibility hidden
# 3. 符号名被 C++ mangle
```

**问题 2：C++ 符号名难以阅读**

```bash
# 使用 -C 选项解码
nm -C program

# 或使用 c++filt 工具
nm program | c++filt

# 单独解码符号名
echo '_ZN3Logger3logEiPKc' | c++filt
# 输出: Logger::log(int, char const*)
```

**问题 3：动态库符号不显示**

```bash
# 必须使用 -D 选项查看动态符号表
nm -D libmylib.so

# 使用 readelf 查看更详细信息
readelf -s libmylib.so
```

### 6.2 调试技巧

**技巧 1：对比两个版本的符号差异**

```bash
# 导出符号列表
nm -g program_v1.o | sort > symbols_v1.txt
nm -g program_v2.o | sort > symbols_v2.txt

# 对比差异
diff symbols_v1.txt symbols_v2.txt
```

**技巧 2：追踪符号来源**

```bash
# 在静态库中查找符号定义
for lib in *.a; do
  if nm $lib 2>/dev/null | grep -q 'T function_name'; then
    echo "Found in $lib"
  fi
done

# 在动态库中查找
for lib in *.so; do
  if nm -D $lib 2>/dev/null | grep -q 'T function_name'; then
    echo "Found in $lib"
  fi
done
```

**技巧 3：分析符号导出控制**

```bash
# 检查符号可见性
nm -C -D libmylib.so | grep -v ' U '

# 检查是否有 hidden 符号
readelf -sW libmylib.so | grep HIDDEN
```

## 7. 集成实践

### 7.1 工具链集成

**与 GCC/G++ 配合：**

```bash
# 编译后立即检查符号
gcc -c myprogram.c && nm myprogram.o

# 链接前检查未定义符号
nm *.o | grep ' U '

# 检查生成的库
ar rcs libmylib.a *.o && nm -s libmylib.a
```

**与 Makefile 集成：**

```makefile
# Makefile 中添加符号检查目标
.PHONY: symbols
symbols:
	nm -S $(TARGET) | sort -k2 -r | head -n 20

.PHONY: check-undefined
check-undefined:
	nm -u $(OBJECTS) && echo "OK: No undefined symbols" || echo "ERROR: Undefined symbols found"
```

**与 CMake 集成：**

```cmake
# 添加自定义目标检查符号
add_custom_target(symbols
    COMMAND nm -S $<TARGET_FILE:myapp> | sort -k2 -r | head -n 20
    DEPENDS myapp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Show top 20 largest symbols"
)
```

### 7.2 CI/CD 配置

**GitLab CI 示例：**

```yaml
symbol-check:
  stage: test
  script:
    - mkdir build && cd build
    - cmake .. && make
    # 检查未定义符号
    - |
      if nm -u myapp | grep -q .; then
        echo "ERROR: Found undefined symbols"
        nm -u myapp
        exit 1
      fi
    # 检查符号大小
    - nm -S --size-sort myapp | tail -n 10
  artifacts:
    paths:
      - build/myapp
```

**GitHub Actions 示例：**

```yaml
- name: Check Symbols
  run: |
    cmake -B build
    cmake --build build
    # 检查大符号
    echo "Top 10 largest symbols:"
    nm -S build/myapp | grep ' T ' | sort -k2 -r | head -n 10
    # 检查未定义符号
    if nm -u build/myapp | grep -q .; then
      echo "Warning: Undefined symbols found"
      nm -u build/myapp
    fi
```

### 7.3 实战案例

**案例 1：排查 "undefined reference" 错误**

```bash
# 1. 找出未定义符号
nm myprogram.o | grep ' U '
# 输出: U calculate_crc32

# 2. 在可用库中搜索
nm libcrc.a | grep calculate_crc32
# 输出: 0000000000000000 T calculate_crc32

# 3. 添加库到链接命令
gcc myprogram.o -L. -lcrc -o myprogram
```

**案例 2：分析动态库导出接口**

```bash
# 查看导出接口
nm -D --defined-only libmylib.so | grep ' T '

# 生成接口文档
nm -D --defined-only libmylib.so | awk '/ T / {print $3}' > exported_functions.txt

# 检查 ABI 兼容性
nm -D libmylib.so.v1 | sort > symbols_v1.txt
nm -D libmylib.so.v2 | sort > symbols_v2.txt
diff symbols_v1.txt symbols_v2.txt
```

**案例 3：识别代码膨胀**

```bash
# 找出最大的函数
nm -S --size-sort myprogram | grep ' T ' | tail -n 20

# 分析模板膨胀
nm -C myprogram | grep '<' | sort -k2 -r | head -n 10

# 生成大小报告
nm -S myprogram | awk '/ T / {print $4, $3}' | sort -rn | head -n 20 > large_symbols.txt
```

## 8. 参考资源

### 8.1 官方文档

- GNU Binutils 手册: https://sourceware.org/binutils/docs/binutils/nm.html
- ELF 格式规范: https://refspecs.linuxfoundation.org/elf/elf.pdf
- GCC 符号可见性: https://gcc.gnu.org/wiki/Visibility

### 8.2 相关工具

| 工具 | 用途 | 说明 |
|------|------|------|
| readelf | 查看 ELF 详细信息 | readelf -s 显示完整符号表 |
| objdump | 反汇编和符号查看 | objdump -t 显示符号表 |
| c++filt | C++ 符号解码 | 解码 mangled 符号名 |
| ar | 静态库管理 | ar -t 列出库成员 |
| strip | 移除符号 | strip 移除符号表 |
| ldd | 动态库依赖 | 查看动态库依赖 |

### 8.3 学习路径

1. **基础阶段**：掌握 nm 基本用法，理解符号类型
2. **进阶阶段**：学习符号解码、动态符号分析
3. **高级阶段**：结合 readelf/objdump 进行深度分析
4. **实践阶段**：在项目中应用 nm 排查链接问题、优化代码

### 8.4 常用命令速查

```bash
# 基本查看
nm program                  # 查看所有符号
nm -S program               # 显示符号大小
nm -C program               # C++ 符号解码

# 过滤选项
nm -g program               # 仅外部符号
nm -u program               # 仅未定义符号
nm --defined-only program   # 仅已定义符号
nm -D lib.so                # 动态符号

# 排序选项
nm -n program               # 按地址排序
nm -S --size-sort program   # 按大小排序

# 格式选项
nm -P program               # POSIX 格式
nm -A file1.o file2.o       # 显示文件名
```