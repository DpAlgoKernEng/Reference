# strip - 符号移除工具

## 1. 概述与背景

### 1.1 工具定位

strip 是 GNU binutils 工具链提供的符号表和调试信息移除工具，主要用于减小可执行文件和库文件的体积。作为构建后处理工具，strip 在软件发布流程中扮演着重要角色。

核心功能：
- 移除符号表（Symbol Table）
- 移除调试信息（Debug Information）
- 移除重定位信息（Relocation Information）
- 移除注释段（Comment Sections）

### 1.2 发展历史

| 年份 | 版本 | 特性 |
|------|------|------|
| 1986 | 1.0 | GNU binutils 初始版本，strip 基础功能 |
| 1991 | 2.x | ELF 格式支持，多种符号移除选项 |
| 2000 | 2.10 | 增强共享库处理能力 |
| 2010 | 2.20 | 改进 DWARF 调试信息处理 |
| 2020 | 2.35 | 支持新架构（RISC-V 等） |
| 2024 | 2.42 | 改进 LTO 构建支持 |

### 1.3 核心特性

| 特性 | 说明 |
|------|------|
| 符号移除 | 移除全部或部分符号表 |
| 调试信息移除 | 移除 DWARF/STABS 调试信息 |
| 段移除 | 移除指定的 ELF 段 |
| 选择性保留 | 保留指定符号或符号类型 |
| 多格式支持 | ELF、Mach-O、PE 等格式 |
| 跨平台 | Linux、macOS、BSD 等系统 |

### 1.4 适用场景

| 场景 | 推荐操作 | 效果 |
|------|----------|------|
| 生产环境部署 | strip | 体积减小 30-50% |
| 嵌入式设备 | strip | 节省存储空间 |
| 移动应用库 | strip -g | 减小包体积 |
| 代码保护 | strip | 移除符号增加逆向难度 |
| 分发二进制 | strip | 减少下载带宽 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 |
|------|------|------|
| strip | 专注符号移除、速度快、易用 | 功能单一 |
| objcopy | 功能全面、支持格式转换 | 选项复杂 |
| install -s | 集成安装流程 | 选项有限 |
| elfdump | 详细分析能力 | 非移除工具 |

## 2. 安装与配置

### 2.1 多平台安装

**Linux 发行版**：

```bash
# Debian/Ubuntu
sudo apt update
sudo apt install binutils

# CentOS/RHEL
sudo yum install binutils

# Fedora
sudo dnf install binutils

# Arch Linux
sudo pacman -S binutils
```

**macOS**：

```bash
# macOS 自带 strip（Xcode 工具链）
xcode-select --install

# 或通过 Homebrew 安装 binutils
brew install binutils

# 注意：Homebrew 版本名为 gstrip
gstrip --version
```

**验证安装**：

```bash
strip --version
# GNU strip (GNU Binutils) 2.42
```

### 2.2 版本管理

```bash
# 查看版本
strip --version

# 查看 ELF 支持
readelf -h /usr/bin/strip | grep -i machine

# 不同版本行为可能略有差异
# 建议使用与编译器配套的 binutils 版本
```

### 2.3 环境配置

strip 通常无需特殊配置，但可在构建系统中集成：

```bash
# 设置 strip 路径（如果使用非默认版本）
export STRIP=/usr/local/bin/strip

# 或在 Makefile 中定义
# STRIP = strip
```

### 2.4 验证安装

```bash
# 检查 strip 功能
echo 'int main(){return 0;}' > test.c
gcc test.c -o test

# 验证 strip 前后大小变化
ls -l test
strip test
ls -l test

# 清理
rm test test.c
```

## 3. 基础使用

### 3.1 快速入门

```bash
# 最简单的用法：移除所有符号
strip program

# 查看效果
ls -l program           # 文件变小
file program            # 显示 "stripped"
nm program              # 无符号输出
```

### 3.2 基本命令

**移除所有符号**：

```bash
strip program
# 或显式指定
strip --strip-all program
```

效果：
- 移除所有符号表
- 移除调试信息
- 移除重定位信息
- 文件最小化

**移除调试符号**：

```bash
strip -g program
# 或
strip --strip-debug program
```

效果：
- 保留全局符号表
- 移除调试信息
- 可用于基本调试
- 推荐用于共享库

**移除未使用符号**：

```bash
strip --strip-unneeded program.o
```

效果：
- 保留需要的符号
- 移除本地符号
- 适合目标文件

### 3.3 常用操作

**保留特定符号**：

```bash
# 保留 main 和 init 符号
strip -K main -K init program

# 保留所有全局符号
strip -x program
```

**移除特定段**：

```bash
# 移除调试段
strip -R .debug_info program

# 移除注释段
strip -R .comment program

# 移除多个段
strip -R .debug_info -R .comment program
```

**输出到新文件**：

```bash
# 不修改原文件
strip -o stripped_program program

# 组合选项
strip -g -o program_stripped program
```

### 3.4 处理多个文件

```bash
# 批量处理目标文件
strip *.o

# 处理目录下所有共享库
find . -name "*.so" -exec strip -g {} \;

# 使用 xargs 提高效率
find . -name "*.so" | xargs strip -g

# Makefile 风格
for f in *.so; do strip -g "$f"; done
```

## 4. 进阶特性

### 4.1 高级配置

**选择性符号操作**：

```bash
# 移除特定符号
strip -N internal_function program

# 保留符号列表（从文件读取）
strip -K @symbol_list.txt program

# 组合使用保留和移除
strip -K main -N internal_debug program
```

**保留元数据**：

```bash
# 保留时间戳
strip -p program

# 保留文件权限
strip --preserve-dates program
```

### 4.2 库文件处理

**共享库处理**：

```bash
# 推荐：仅移除调试符号
strip -g libmylib.so

# 谨慎：移除所有符号可能影响动态链接
strip libmylib.so  # 可能导致问题

# 验证动态符号
nm -D libmylib.so
```

**静态库处理**：

```bash
# 不推荐直接 strip 静态库
strip libmylib.a  # 可能导致链接失败

# 正确方法：提取、strip、重新打包
ar -x libmylib.a
strip --strip-unneeded *.o
ar rcs libmylib.a *.o
```

### 4.3 跨平台处理

**macOS strip 额外选项**：

```bash
# 移除调试符号
strip -S program

# 移除所有符号
strip -x program

# 移除代码签名
strip -l program

# 移除特定架构（通用二进制）
strip -arch i386 -S universal_binary
```

**与 objcopy 配合**：

```bash
# objcopy 功能更全面
objcopy --strip-debug program stripped_program
objcopy --strip-all program stripped_program

# 分离调试信息
objcopy --only-keep-debug program program.debug
strip program
objcopy --add-gnu-debuglink=program.debug program
```

## 5. 性能优化

### 5.1 大小优化策略

| 文件类型 | 推荐操作 | 减小幅度 |
|----------|----------|----------|
| 可执行文件 | strip | 30-50% |
| 共享库 | strip -g | 20-40% |
| 静态库 | 不 strip 或提取后处理 | 15-30% |
| 目标文件 | strip --strip-unneeded | 10-20% |

**实际效果示例**：

```bash
# 查看优化效果
echo "原始大小: $(ls -l program | awk '{print $5}') 字节"
strip program
echo "strip 后: $(ls -l program | awk '{print $5}') 字节"

# 详细信息
size program
```

### 5.2 最佳实践

**调试信息分离流程**：

```bash
# 1. 构建时包含调试信息
gcc -g -O2 program.c -o program

# 2. 分离调试信息
objcopy --only-keep-debug program program.debug

# 3. strip 主文件
strip program

# 4. 添加调试链接
objcopy --add-gnu-debuglink=program.debug program

# 5. 压缩调试信息（可选）
gzip program.debug
```

**GDB 使用分离调试信息**：

```bash
# 自动加载调试信息
gdb program

# 或指定调试文件路径
set debug-file-directory /path/to/debug/files
```

**构建系统集成**：

```cmake
# CMake Release 模式自动 strip
set(CMAKE_BUILD_TYPE Release)

# 手动添加 strip 步骤
add_custom_command(TARGET myapp POST_BUILD
    COMMAND strip -g $<TARGET_FILE:myapp>
    COMMENT "Stripping debug symbols from myapp"
)

# 安装时 strip
install(TARGETS myapp 
    RUNTIME DESTINATION bin
    COMPONENT Runtime
)
set_target_properties(myapp PROPERTIES 
    INSTALL_RPATH_USE_LINK_PATH TRUE
)
```

**Makefile 集成**：

```makefile
# 安装时 strip
INSTALL = install
STRIP = strip

install: all
	$(STRIP) -g $(TARGET)
	$(INSTALL) -m 755 $(TARGET) $(DESTDIR)/usr/local/bin/

# 或使用 install -s
install: all
	install -s $(TARGET) $(DESTDIR)/usr/local/bin/
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：动态链接失败**

```bash
# 症状
error: symbol lookup error: undefined symbol

# 原因：strip 移除了动态链接需要的符号
# 解决：仅移除调试符号
strip -g libmylib.so
```

**问题 2：静态库链接失败**

```bash
# 症状
undefined reference to `xxx'

# 原因：直接 strip 静态库
# 解决：提取目标文件后单独处理
ar -x libmylib.a
strip --strip-unneeded *.o
ar rcs libmylib.a *.o
```

**问题 3：调试困难**

```bash
# 症状：gdb 无法显示源码
(gdb) list
No symbol table is loaded.

# 解决：使用分离的调试文件
objcopy --add-gnu-debuglink=program.debug program
```

**问题 4：段错误无法分析**

```bash
# 症状：coredump 无符号信息
# 解决：保留调试文件或使用分离调试信息

# 构建 debug 包
gcc -g -O2 program.c -o program.debug
cp program.debug /var/lib/debug/

# 发布时 strip
strip program -o program.release
```

### 6.2 调试技巧

**检查 strip 效果**：

```bash
# 查看符号表
nm program

# 查看动态符号
nm -D program

# 查看 ELF 头信息
readelf -h program

# 查看段信息
readelf -S program

# 查看是否已 strip
file program
# 输出: ELF 64-bit LSB executable, x86-64, stripped
```

**对比 strip 前后**：

```bash
# 保存原始文件
cp program program.orig

# strip
strip program

# 对比
ls -l program.orig program
nm program.orig > symbols.orig
nm program > symbols.stripped
diff symbols.orig symbols.stripped
```

## 7. 集成实践

### 7.1 工具链集成

**与编译器配合**：

```bash
# GCC 内置 strip 选项
gcc -s program.c -o program          # 链接时 strip
gcc -Wl,-s program.c -o program      # 传递给链接器

# 仅 Release 构建 strip
if [ "$BUILD_TYPE" = "release" ]; then
    gcc -s program.c -o program
else
    gcc program.c -o program
fi
```

**与打包工具配合**：

```bash
# RPM 打包
%install
make install DESTDIR=%{buildroot}
strip -g %{buildroot}/usr/bin/myapp

# Debian 打包
override_dh_auto_install:
	dh_auto_install
	strip -g debian/myapp/usr/bin/myapp
```

### 7.2 CI/CD 配置

**GitHub Actions**：

```yaml
- name: Build Release
  run: |
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    strip build/myapp

- name: Create Debug Package
  run: |
    objcopy --only-keep-debug build/myapp myapp.debug
    gzip myapp.debug
```

**GitLab CI**：

```yaml
build:
  stage: build
  script:
    - cmake -B build -DCMAKE_BUILD_TYPE=Release
    - cmake --build build
    - strip -g build/myapp
  artifacts:
    paths:
      - build/myapp
```

### 7.3 实战案例

**案例 1：嵌入式设备部署**

```bash
#!/bin/bash
# 构建并优化嵌入式程序

# 编译
arm-linux-gnueabi-gcc -Os -s program.c -o program

# 进一步优化
strip -R .comment -R .note program

# 检查大小
ls -l program

# 生成映射文件
arm-linux-gnueabi-nm -S --size-sort program > symbols.map
```

**案例 2：库文件分发**

```bash
#!/bin/bash
# 分发优化后的共享库

LIBNAME=libmylib.so.1.0

# 编译带调试信息
gcc -shared -fPIC -g -O2 -o $LIBNAME mylib.c

# 分离调试信息
objcopy --only-keep-debug $LIBNAME $LIBNAME.debug
strip -g $LIBNAME
objcopy --add-gnu-debuglink=$LIBNAME.debug $LIBNAME

# 创建符号链接
ln -sf $LIBNAME libmylib.so.1
ln -sf $LIBNAME libmylib.so

# 打包
tar czf mylib-release.tar.gz libmylib.so* $LIBNAME.debug
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| GNU Binutils 手册 | https://sourceware.org/binutils/docs/ |
| strip 手册页 | man strip |
| ELF 格式规范 | https://refspecs.linuxfoundation.org/elf/elf.pdf |

### 8.2 学习路径

```
入门阶段
├── 理解符号表概念
├── 掌握基本 strip 命令
└── 了解文件大小优化

进阶阶段
├── 掌握各种 strip 选项
├── 学习调试信息分离
└── 集成到构建系统

高级阶段
├── 深入 ELF 格式理解
├── 优化策略制定
└── 跨平台处理
```

### 8.3 相关工具

| 工具 | 用途 |
|------|------|
| nm | 查看符号表 |
| readelf | 查看 ELF 信息 |
| objdump | 反汇编工具 |
| objcopy | 对象文件复制和转换 |
| size | 查看段大小 |
| file | 查看文件类型 |