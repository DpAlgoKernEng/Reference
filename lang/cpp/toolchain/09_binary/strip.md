# strip - 符号移除工具

## 1. 概述与背景

### 1.1 工具定位

strip 是 GNU binutils 工具链提供的二进制文件优化工具，专门用于移除可执行文件和库文件中的符号信息、调试信息和重定位信息，从而显著减小文件体积。

**核心功能**：
- 移除符号表（Symbol Table）
- 移除调试信息（Debug Information）
- 移除重定位信息（Relocation Information）
- 移除特定段（Sections）

**应用场景**：
- 生产环境部署优化
- 嵌入式系统资源节省
- 二进制文件分发
- 代码保护（移除符号信息）

### 1.2 发展历史

| 年份 | 版本/事件 | 特性 |
|------|-----------|------|
| 1986 | GNU binutils 创建 | 作为 binutils 工具集的一部分发布 |
| 1991 | ELF 格式支持 | 支持 ELF 可执行文件格式 |
| 1999 | DWARF 调试信息 | 支持 DWARF 调试信息移除 |
| 2000s | 多架构支持 | 支持多种 CPU 架构（x86, ARM, MIPS 等） |
| 现代 | macOS 适配 | macOS 提供同名工具，选项略有差异 |

### 1.3 核心特性

**主要特性**：

| 特性 | 说明 |
|------|------|
| 灵活的符号移除 | 支持全部移除、部分移除、选择性保留 |
| 多种移除策略 | all、debug、unneeded 三种策略 |
| 段级别控制 | 可移除特定段（.debug, .comment 等） |
| 跨平台支持 | Linux、macOS、BSD 等多平台可用 |
| 保留时间戳 | 可保留文件原始修改时间 |
| 安全输出 | 支持输出到新文件，不修改原文件 |

### 1.4 适用场景

| 场景 | 推荐操作 | 效果 |
|------|----------|------|
| 生产环境部署 | `strip program` | 减小 30-50% 体积 |
| 共享库分发 | `strip -g libxxx.so` | 减小 20-40% 体积 |
| 嵌入式系统 | `strip --strip-all` | 最小化文件大小 |
| 代码保护 | `strip program` | 移除符号信息 |
| 调试信息分离 | `strip + objcopy` | 保留独立调试文件 |

### 1.5 对比分析

| 工具 | 优势 | 劣势 | 适用场景 |
|------|------|------|----------|
| strip | 专用、简单、高效 | 功能单一 | 快速减小文件体积 |
| objcopy | 功能全面、支持格式转换 | 命令复杂 | 高级二进制操作 |
| install -s | 集成安装流程 | 功能受限 | Makefile install 目标 |

**objcopy 扩展功能**：
```bash
# strip 只能移除符号
strip program

# objcopy 可以复制并移除
objcopy --strip-debug program stripped_program

# objcopy 可以转换格式
objcopy -O binary program program.bin
```

## 2. 安装与配置

### 2.1 多平台安装

**Linux 安装**：

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

**macOS 安装**：

```bash
# macOS 自带 strip（Xcode Command Line Tools）
xcode-select --install

# 验证安装
strip -V
```

**从源码编译**：

```bash
# 下载 binutils 源码
wget https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.gz
tar xzf binutils-2.41.tar.gz
cd binutils-2.41

# 配置
./configure --prefix=/usr/local

# 编译安装
make -j$(nproc)
sudo make install
```

### 2.2 版本管理

```bash
# 查看版本
strip --version
# 输出: GNU strip (GNU Binutils) 2.41

# 查看支持的架构
strip --info
```

**版本兼容性**：
- 建议使用 2.30+ 版本以支持最新 ELF 格式
- macOS strip 是独立工具，与 GNU strip 选项有差异

### 2.3 环境配置

strip 通常不需要额外配置，但可以设置环境变量：

```bash
# 设置 strip 路径（交叉编译时）
export STRIP=/usr/local/arm-linux-gnueabihf/bin/strip

# CMake 中指定
set(CMAKE_STRIP /usr/local/arm-linux-gnueabihf/bin/strip)
```

### 2.4 验证安装

```bash
# 检查是否安装
which strip
# 输出: /usr/bin/strip

# 查看版本
strip --version

# 测试基本功能
echo 'int main(){}' > test.c
gcc test.c -o test
strip test
ls -l test
```

## 3. 基础使用

### 3.1 快速入门

**最简单的使用**：

```bash
# 编译程序
gcc -g -o program program.c

# 查看原始大小
ls -lh program
# 输出: 100K

# strip 处理
strip program

# 查看处理后大小
ls -lh program
# 输出: 50K
```

### 3.2 符号类型

理解符号类型有助于正确使用 strip：

| 符号类型 | 说明 | strip 行为 |
|---------|------|-----------|
| 全局符号（Global） | 可被外部引用的函数/变量 | --strip-all 移除 |
| 局部符号（Local） | 仅本文件可见的函数/变量 | 默认移除 |
| 调试符号（Debug） | DWARF 调试信息 | -g/--strip-debug 移除 |
| 动态符号（Dynamic） | 动态链接所需符号 | --strip-unneeded 保留 |

**查看符号**：

```bash
# 查看所有符号
nm program

# 查看动态符号
nm -D program

# 查看调试信息
readelf -S program | grep debug
```

### 3.3 基本命令

**常用选项表**：

| 选项 | 简写 | 说明 | 典型用途 |
|------|------|------|----------|
| `--strip-all` | `-s` | 移除所有符号 | 生产环境部署 |
| `--strip-debug` | `-g` | 移除调试符号 | 保留基本调试能力 |
| `--strip-unneeded` | - | 移除不需要的符号 | 目标文件处理 |
| `--keep-symbol` | `-K` | 保留指定符号 | 保留关键符号 |
| `--strip-symbol` | `-N` | 移除指定符号 | 选择性移除 |
| `--remove-section` | `-R` | 移除指定段 | 移除特定段 |
| `--output` | `-o` | 输出到文件 | 保留原文件 |
| `--preserve-dates` | `-p` | 保留时间戳 | 保持文件时间信息 |

### 3.4 常用操作

**操作 1：移除所有符号**

```bash
strip program
# 或
strip --strip-all program
```

效果：
- 移除所有符号表
- 移除调试信息
- 移除重定位信息
- 文件最小化

**操作 2：移除调试符号**

```bash
strip -g program
# 或
strip --strip-debug program
```

效果：
- 保留全局符号表
- 移除调试信息
- 可用于基本调试
- 适合共享库

**操作 3：移除未使用符号**

```bash
strip --strip-unneeded program.o
```

效果：
- 保留需要的符号
- 移除本地符号
- 适合目标文件

**操作 4：输出到新文件**

```bash
# 不修改原文件
strip -o stripped_program -g program

# 保留原文件时间戳
strip -p -o stripped_program program
```

## 4. 进阶特性

### 4.1 高级选项

**选择性符号处理**：

```bash
# 保留特定符号
strip -K main -K init program

# 移除特定符号
strip -N internal_func -N debug_log program

# 保留所有全局符号
strip -x program

# 组合使用
strip -g -K main -K cleanup program
```

**正则表达式匹配**（需要 objcopy）：

```bash
# 移除匹配的符号
objcopy --strip-symbol='*_debug' program stripped_program
```

### 4.2 段操作

**常见段及用途**：

| 段名 | 说明 | 移除影响 |
|------|------|----------|
| `.symtab` | 符号表 | 无法通过符号名调试 |
| `.strtab` | 字符串表 | 符号名丢失 |
| `.debug_*` | 调试信息 | 无法源码级调试 |
| `.comment` | 编译器信息 | 版本信息丢失 |
| `.note.*` | 注释信息 | 无显著影响 |

**移除特定段**：

```bash
# 移除调试段
strip -R .debug_info program
strip -R .debug_abbrev program
strip -R .debug_line program

# 移除注释段
strip -R .comment program

# 移除多个段
strip -R .debug_info -R .debug_abbrev -R .comment program

# 移除所有调试段
for section in $(readelf -S program | grep .debug | awk '{print $2}'); do
    strip -R $section program
done
```

### 4.3 符号保留策略

**策略 1：保留动态链接所需符号**

```bash
# 共享库：保留动态符号
strip --strip-unneeded libmylib.so

# 可执行文件：可安全移除所有符号
strip myapp
```

**策略 2：保留关键调试符号**

```bash
# 保留 main 和关键函数符号
strip -g -K main -K init -K cleanup program
```

**策略 3：分离调试信息**

```bash
# 步骤 1：提取调试信息
objcopy --only-keep-debug program program.debug

# 步骤 2：strip 主文件
strip --strip-debug program

# 步骤 3：添加调试链接
objcopy --add-gnu-debuglink=program.debug program
```

**验证分离结果**：

```bash
# 查看调试链接
readelf -S program | grep gnu_debuglink

# 使用分离的调试信息调试
gdb program
# GDB 会自动加载 program.debug
```

## 5. 性能优化

### 5.1 大小优化策略

**大小对比实测**：

```bash
# 编译带调试信息的程序
gcc -g -O2 -o program program.c

# 查看原始大小
ls -lh program
# 输出: 1.2M

# strip -g 后大小
strip -g program
ls -lh program
# 输出: 800K（减小 33%）

# strip --strip-all 后大小
gcc -g -O2 -o program program.c
strip program
ls -lh program
# 输出: 600K（减小 50%）
```

**不同文件类型的优化建议**：

| 文件类型 | 推荐操作 | 减小幅度 | 注意事项 |
|---------|---------|---------|---------|
| 可执行文件 | `strip program` | 30-50% | 生产环境推荐 |
| 共享库 | `strip -g libxxx.so` | 20-40% | 保留动态符号 |
| 静态库 | 不推荐 strip | - | 可能导致链接失败 |
| 目标文件 | `strip --strip-unneeded` | 10-30% | 保留需要的符号 |
| 调试文件 | 不 strip | - | 保留完整调试信息 |

**批量优化脚本**：

```bash
#!/bin/bash
# optimize-binaries.sh

for file in "$@"; do
    if [ -f "$file" ]; then
        original_size=$(stat -f%z "$file")
        
        case "$file" in
            *.so|*.so.*)
                strip -g "$file"
                ;;
            *)
                strip "$file"
                ;;
        esac
        
        new_size=$(stat -f%z "$file")
        reduction=$(( (original_size - new_size) * 100 / original_size ))
        echo "$file: $(( original_size / 1024 ))K -> $(( new_size / 1024 ))K (-$reduction%)"
    fi
done
```

### 5.2 最佳实践

**实践 1：开发与生产分离**

```bash
# 开发环境：保留调试信息
gcc -g -O0 -o myapp_debug main.c

# 生产环境：分离调试信息
gcc -g -O2 -o myapp main.c
objcopy --only-keep-debug myapp myapp.debug
strip myapp
objcopy --add-gnu-debuglink=myapp.debug myapp

# 分发时只分发 myapp
# 调试时使用 myapp.debug
```

**实践 2：CI/CD 自动化**

```bash
# Makefile 示例
install: all
	# 提取调试信息
	objcopy --only-keep-debug $(TARGET) $(TARGET).debug
	# strip 主文件
	strip $(TARGET)
	# 添加调试链接
	objcopy --add-gnu-debuglink=$(TARGET).debug $(TARGET)
	# 安装
	install -m 755 $(TARGET) $(DESTDIR)/usr/bin/
	# 安装调试文件
	install -m 644 $(TARGET).debug $(DESTDIR)/usr/lib/debug/
```

**实践 3：符号白名单**

```bash
# 创建符号保留列表
cat > keep_symbols.txt << EOF
main
init
cleanup
handle_error
EOF

# 从列表构建 -K 参数
KEEP_OPTS=$(awk '{print "-K " $1}' keep_symbols.txt | tr '\n' ' ')

# 应用 strip
strip -g $KEEP_OPTS program
```

## 6. 问题排查

### 6.1 常见问题

**问题 1：动态链接失败**

```bash
# 错误现象
./myapp: symbol lookup error: ./myapp: undefined symbol: foo

# 原因：strip 移除了动态链接需要的符号
strip libmylib.so  # 错误：移除了所有符号

# 解决方案：使用 --strip-unneeded 或 -g
strip --strip-unneeded libmylib.so
# 或
strip -g libmylib.so
```

**问题 2：静态库链接失败**

```bash
# 错误现象
ar: libmylib.a: file format not recognized

# 原因：strip 破坏了静态库索引
strip libmylib.a

# 解决方案：重新打包
ar -x libmylib.a
strip --strip-unneeded *.o
ar rcs libmylib.a *.o
ranlib libmylib.a
```

**问题 3：GDB 调试失败**

```bash
# 错误现象
(gdb) break main
No symbol table is loaded. Use the "file" command.

# 原因：strip 移除了所有符号
strip program

# 解决方案 1：使用分离的调试文件
gdb -s program.debug program

# 解决方案 2：只移除调试信息
strip -g program  # 保留符号表
```

**问题 4：崩溃分析困难**

```bash
# 错误现象
# 崩溃时只有地址，没有函数名
Segmentation fault (core dumped)

# 解决方案：使用 addr2line 和调试文件
addr2line -e program.debug 0x4005b0
# 或
gdb program.debug core
```

### 6.2 调试信息分离

**完整的调试信息分离流程**：

```bash
#!/bin/bash
# separate-debug-info.sh

PROGRAM=$1

# 1. 提取调试信息
objcopy --only-keep-debug $PROGRAM $PROGRAM.debug

# 2. 生成调试信息 ID
BUILD_ID=$(readelf -n $PROGRAM | grep "Build ID" | awk '{print $3}')

# 3. 创建调试信息目录
mkdir -p /usr/lib/debug/.build-id/${BUILD_ID:0:2}

# 4. 放置调试信息
ln -s $PROGRAM.debug /usr/lib/debug/.build-id/${BUILD_ID:0:2}/${BUILD_ID:2}.debug

# 5. strip 主程序
strip $PROGRAM

# 6. 添加调试链接
objcopy --add-gnu-debuglink=$PROGRAM.debug $PROGRAM

# 7. 验证
file $PROGRAM
file $PROGRAM.debug
```

**验证调试信息可用性**：

```bash
# 检查调试链接
readelf -S program | grep gnu_debuglink

# 检查符号表
nm program

# 测试调试
gdb program
(gdb) info sources  # 查看源文件列表
(gdb) break main    # 测试断点设置
```

## 7. 集成实践

### 7.1 工具链集成

**CMake 集成**：

```cmake
# CMakeLists.txt

# 方式 1：Release 模式自动 strip
set(CMAKE_BUILD_TYPE Release)
# CMake 在 install 时自动 strip

# 方式 2：手动添加 strip 步骤
add_custom_command(TARGET myapp POST_BUILD
    COMMAND strip -g $<TARGET_FILE:myapp>
    COMMENT "Stripping debug symbols from myapp"
)

# 方式 3：分离调试信息
add_custom_command(TARGET myapp POST_BUILD
    COMMAND objcopy --only-keep-debug 
        $<TARGET_FILE:myapp> $<TARGET_FILE:myapp>.debug
    COMMAND strip $<TARGET_FILE:myapp>
    COMMAND objcopy --add-gnu-debuglink=$<TARGET_FILE:myapp>.debug
        $<TARGET_FILE:myapp>
    COMMENT "Separating debug info from myapp"
)

# 方式 4：安装时 strip
install(TARGETS myapp
    RUNTIME DESTINATION bin
    COMPONENT Runtime
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/myapp.debug
    DESTINATION lib/debug
    COMPONENT Debug
)
```

**Makefile 集成**：

```makefile
# Makefile

TARGET = myapp
SOURCES = main.c utils.c
OBJECTS = $(SOURCES:.c=.o)

# 编译选项
CFLAGS = -Wall -O2 -g
LDFLAGS = 

# 目标
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(MAKE) strip-debug

# strip 目标
strip-all: $(TARGET)
	strip $(TARGET)

strip-debug: $(TARGET)
	strip -g $(TARGET)

# 分离调试信息
separate-debug: $(TARGET)
	objcopy --only-keep-debug $(TARGET) $(TARGET).debug
	strip $(TARGET)
	objcopy --add-gnu-debuglink=$(TARGET).debug $(TARGET)

# 安装
install: $(TARGET)
	install -m 755 $(TARGET) $(DESTDIR)/usr/bin/
	
# 清理
clean:
	rm -f $(TARGET) $(TARGET).debug $(OBJECTS)

.PHONY: all strip-all strip-debug separate-debug install clean
```

### 7.2 CI/CD 配置

**GitHub Actions 示例**：

```yaml
# .github/workflows/release.yml
name: Release

on:
  push:
    tags:
      - 'v*'

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Build
        run: |
          gcc -O2 -g -o myapp src/*.c
          
      - name: Separate debug info
        run: |
          objcopy --only-keep-debug myapp myapp.debug
          strip myapp
          objcopy --add-gnu-debuglink=myapp.debug myapp
          
      - name: Create release
        uses: actions/upload-release-asset@v1
        with:
          files: |
            myapp
            myapp.debug
```

**GitLab CI 示例**：

```yaml
# .gitlab-ci.yml
stages:
  - build
  - package

build:
  stage: build
  script:
    - gcc -O2 -g -o myapp src/*.c
    - objcopy --only-keep-debug myapp myapp.debug
    - strip myapp
    - objcopy --add-gnu-debuglink=myapp.debug myapp
  artifacts:
    paths:
      - myapp
      - myapp.debug

package:
  stage: package
  script:
    - tar czf myapp-${CI_COMMIT_TAG}.tar.gz myapp
    - tar czf myapp-debug-${CI_COMMIT_TAG}.tar.gz myapp.debug
  only:
    - tags
```

### 7.3 实战案例

**案例 1：嵌入式系统优化**

```bash
# 场景：嵌入式 Linux 系统存储空间有限

# 1. 编译时优化
arm-linux-gnueabihf-gcc -Os -g -o myapp main.c

# 2. strip 所有二进制文件
find /target -type f \( -perm -u=x -o -name "*.so*" \) -exec \
    arm-linux-gnueabihf-strip --strip-unneeded {} \;

# 3. 检查大小
du -sh /target
```

**案例 2：软件分发优化**

```bash
# 场景：分发软件包，减小下载体积

# 创建发布脚本
cat > release.sh << 'EOF'
#!/bin/bash
VERSION=$1
PKG_NAME="myapp-${VERSION}"

# 创建目录
mkdir -p ${PKG_NAME}/bin
mkdir -p ${PKG_NAME}/lib/debug

# 编译
gcc -O2 -g -o ${PKG_NAME}/bin/myapp src/*.c

# 分离调试信息
objcopy --only-keep-debug ${PKG_NAME}/bin/myapp ${PKG_NAME}/lib/debug/myapp.debug
strip ${PKG_NAME}/bin/myapp
objcopy --add-gnu-debuglink=../lib/debug/myapp.debug ${PKG_NAME}/bin/myapp

# 打包
tar czf ${PKG_NAME}.tar.gz ${PKG_NAME}

# 打包调试信息（单独分发）
tar czf ${PKG_NAME}-debug.tar.gz ${PKG_NAME}/lib/debug

echo "Created ${PKG_NAME}.tar.gz (${PKG_NAME}-debug.tar.gz)"
EOF

chmod +x release.sh
./release.sh 1.0.0
```

**案例 3：多架构支持**

```bash
# 场景：支持多个架构的二进制文件

for ARCH in x86_64 arm64; do
    # 编译
    ${ARCH}-linux-gnu-gcc -O2 -g -o myapp-${ARCH} src/*.c
    
    # 分离调试信息
    objcopy --only-keep-debug myapp-${ARCH} myapp-${ARCH}.debug
    ${ARCH}-linux-gnu-strip myapp-${ARCH}
    objcopy --add-gnu-debuglink=myapp-${ARCH}.debug myapp-${ARCH}
    
    # 创建包
    tar czf myapp-${ARCH}.tar.gz myapp-${ARCH}
    tar czf myapp-${ARCH}-debug.tar.gz myapp-${ARCH}.debug
done
```

## 8. 参考资源

### 8.1 官方文档

| 资源 | 链接 |
|------|------|
| GNU Binutils 手册 | https://sourceware.org/binutils/docs/ |
| strip 手册页 | `man strip` |
| ELF 格式规范 | https://refspecs.linuxfoundation.org/elf/elf.pdf |
| DWARF 调试格式 | https://dwarfstd.org/ |

### 8.2 相关工具

| 工具 | 用途 | 关系 |
|------|------|------|
| nm | 查看符号表 | strip 前/后验证 |
| readelf | 查看 ELF 信息 | 分析段信息 |
| objdump | 反汇编工具 | 分析二进制 |
| objcopy | 复制/转换二进制 | 高级二进制操作 |
| addr2line | 地址转行号 | 调试 strip 后的程序 |
| gdb | 调试器 | 使用分离的调试信息 |

**推荐学习路径**：

```
1. 掌握基本用法
   └── strip program / strip -g program

2. 理解符号类型
   └── nm / readelf -s

3. 学习调试信息分离
   └── objcopy --only-keep-debug / --add-gnu-debuglink

4. 集成到构建系统
   └── CMake / Makefile

5. CI/CD 自动化
   └── GitHub Actions / GitLab CI
```

### 8.3 常用命令速查

```bash
# 基本操作
strip program                          # 移除所有符号
strip -g program                       # 移除调试符号
strip --strip-unneeded program.o       # 移除不需要的符号

# 高级操作
strip -K main -K init program          # 保留特定符号
strip -R .comment program              # 移除特定段
strip -o stripped program              # 输出到新文件

# 调试信息分离
objcopy --only-keep-debug program program.debug
strip program
objcopy --add-gnu-debuglink=program.debug program

# 查看/验证
nm program                             # 查看符号表
readelf -S program                     # 查看段信息
file program                           # 查看文件类型
ls -lh program                         # 查看文件大小
```

---

*strip 是二进制优化的重要工具，合理使用可以显著减小部署包体积，同时通过调试信息分离保留调试能力。*