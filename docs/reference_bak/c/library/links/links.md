# 实用资源（Useful resources）

来源：[cppreference.com](https://en.cppreference.com)

---

## C 常见问题解答（FAQs）

| 资源名称 | 描述 |
|----------|------|
| [C FAQ](http://c-faq.com/) | 来自 Usenet 新闻组 `comp.lang.c` 的常见问题官方解答集合 |

---

## C 语言与标准库参考资料

| 资源名称 | 描述 |
|----------|------|
| [JTC1/SC22/WG14（C 标准委员会）](http://www.open-std.org/jtc1/sc22/wg14/) | 官方 C 语言标准委员会网站，提供标准草案、提案和缺陷报告 |
| [C89 草案](http://port70.net/~nsz/c/c89/c89-draft.html) | ANSI C / ISO C 1989 标准草案（HTML 版） |
| [C99 草案（含 TC1、TC2）](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf) | n1124.pdf |
| [C99 草案（含 TC1、TC2、TC3）](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf) | n1256.pdf |
| [C11 最终工作草案](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) | n1570.pdf |
| [C17/C18 最终草案](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2176.pdf) | n2176（FDIS） |
| [C23 首次发布后草案](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf) | n3220.pdf（仅包含编辑性修正） |
| [C 最新公开工作草案（2025-01-03）](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n3435.pdf) | n3435.pdf |
| [comp.std.c](https://groups.google.com/g/comp.std.c) | Google Groups 上关于 C 语言标准讨论的邮件列表 |
| [GNU C 手册](https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html) | GNU 编译器实现的 C 核心语言参考手册 |

---

## C 应用二进制接口（ABIs）

| ABI 名称 | 支持架构 |
|----------|----------|
| [System V ABI](https://refspecs.linuxfoundation.org/elf/abi.html) | x86-64、Intel386、IA MCU 等 Unix 系统常用架构 |
| [Windows x64 ABI](https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention) | Microsoft Windows 平台 64 位系统 |
| [PowerPC EABI](https://www.nxp.com/docs/en/application-note/AN2891.pdf) | PowerPC 32 位嵌入式处理器 |
| [PowerOpen ABI](https://www.ibm.com/support/pages/poweropen-environment-binary-compatibility-specification) | 较旧的 PowerPC 平台 |
| [ARM ABI](https://developer.arm.com/documentation/ihi0036/latest/) | ARM 架构（包括 AAPCS） |

---

## 相关标准规范

| 标准名称 | 描述 |
|----------|------|
| [POSIX.1-2024 / IEEE Std 1003.1-2024](https://pubs.opengroup.org/onlinepubs/97999999999/) | Unix 兼容系统的操作系统 API 接口标准 |
| [Linux 手册页（man pages）](https://man7.org/linux/man-pages/) | Linux 操作系统 API 官方文档 |

---

## 开源实现项目

### 标准库实现

| 实现名称 | 简介 |
|----------|------|
| [GNU libc (glibc)](https://www.gnu.org/software/libc/) | GNU/Linux 系统常用 C 标准库 |
| [MUSL](https://musl.libc.org/) | 轻量级、符合标准、安全的 C 库，适用于嵌入式系统 |
| [Newlib](https://sourceware.org/newlib/) | 为嵌入式系统设计的 C 标准库 |
| [PDClib](https://github.com/DevSolar/pdclib) | 公共领域 C 库实现，注重教学用途 |

### 编译器实现

| 编译器名称 | 简介 |
|------------|------|
| [GCC（GNU Compiler Collection）](https://gcc.gnu.org/) | GNU 项目下的老牌编译器套件，支持 C/C++ 多种语言 |
| [LLVM Clang](https://clang.llvm.org/) | 基于 LLVM 的现代 C/C++ 编译器，性能优秀 |
| [SDCC（Small Device C Compiler）](http://sdcc.sourceforge.net/) | 用于小型微控制器（如 8051、Z80）的 C 编译器 |

---

## 其他推荐资源

| 类别 | 资源描述 |
|------|----------|
| C 库清单 | [C Libraries List](https://en.wikipedia.org/wiki/List_of_C_libraries)（维基百科） |
| 实用工具 | [Compiler Explorer (Godbolt)](https://godbolt.org/)：在线查看汇编代码 |
| 学习平台 | [GeeksforGeeks C Programming](https://www.geeksforgeeks.org/c-programming-language/) |
| 安全实践 | [CERT C 编码标准](https://wiki.sei.cmu.edu/confluence/display/c) |

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 实用链接资源 | [C++ External Links](https://zh.cppreference.com/w/cpp/links) |

---

> 页面来源：[cppreference.com/c/links](https://en.cppreference.com/mwiki/index.php?title=c/links&oldid=180167)  
> 页面最后修改时间：2025年2月4日 23:19 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

✅ **小结建议**：
- 查阅官方标准文档时注意区分不同年份和修订版本。
- 嵌入式开发时优先选择 Newlib 或 MUSL。
- 现代工程可结合使用 Compiler Explorer 快速验证代码行为。
- 关注 CERT 安全编码规则，提升程序健壮性。