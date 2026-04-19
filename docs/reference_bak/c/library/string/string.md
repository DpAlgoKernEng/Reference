# 字符串处理库（Strings library）

来源：[cppreference.com](https://en.cppreference.com)

---

## 简介

C 语言中的字符串本质上是以 `\0`（空字符）结尾的字符数组。字符串处理库根据字符宽度和编码方式划分为三类：

1. **字节字符串（Byte Strings）**：单字节字符，基于 ASCII 或 ISO-8859 等编码。
2. **多字节字符串（Multibyte Strings）**：可变长度表示宽字符，常用于 UTF-8。
3. **宽字符串（Wide Strings）**：固定宽度的宽字符（通常为 `wchar_t`），适合表示 Unicode。

这些字符串类型分别由不同的函数集合操作，以适应国际化和高效处理的需求。

---

## 字符串类型分类

| 类型 | 编码示例 | 字符类型 | 头文件 | 特点 |
|------|----------|----------|--------|------|
| [**字节字符串**](#字节字符串-byte-strings) | ASCII / Latin-1 | `char` | `<string.h>` | 固定1字节，处理速度快 |
| [**多字节字符串**](#多字节字符串-multibyte-strings) | UTF-8 | `char[]` | `<wchar.h>` | 变长编码，兼容性好 |
| [**宽字符串**](#宽字符串-wide-strings) | UCS-4 / UTF-32 | `wchar_t` | `<wchar.h>` | 固定宽字符，支持复杂语言 |

---

## 字节字符串（Byte Strings）

### 头文件：`<string.h>`

处理以 `\0` 结尾的普通字符（ASCII）数组。

#### 函数概览

| 函数 | 功能 |
|------|------|
| `strcpy()` / `strncpy()` | 字符串复制 |
| `strcat()` / `strncat()` | 字符串拼接 |
| `strcmp()` / `strncmp()` | 字符串比较 |
| `strlen()` | 获取字符串长度 |
| `strchr()` / `strrchr()` | 查找字符 |
| `strstr()` | 查找子串 |
| `strtok()` | 分割字符串 |
| `memcpy()` / `memset()` | 内存操作 |
| `strerror()` | 错误码转字符串 |

#### 示例代码

```c
#include <stdio.h>
#include <string.h>

int main() {
    char src[] = "Hello";
    char dest[20];

    strcpy(dest, src);         // 复制字符串
    strcat(dest, " World");    // 拼接字符串
    printf("Length: %zu\n", strlen(dest));  // 输出长度
    printf("Result: %s\n", dest);           // 输出结果

    return 0;
}
```

---

## 多字节字符串（Multibyte Strings）

### 头文件：`<wchar.h>`（C95 起）

处理变长多字节字符序列，例如 UTF-8 编码。

#### 函数概览

| 函数 | 功能 |
|------|------|
| `mblen()` | 测量下一个多字节字符的长度 |
| `mbtowc()` | 多字节转宽字符 |
| `wctomb()` | 宽字符转多字节 |
| `mbstowcs()` | 多字节串转宽串 |
| `wcstombs()` | 宽串转多字节串 |

#### 示例代码

```c
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "");  // 设置为系统默认区域
    const char *mb_str = "你好";
    wchar_t wc_str[10];

    mbstowcs(wc_str, mb_str, sizeof(wc_str)/sizeof(wchar_t));
    wprintf(L"Wide string: %ls\n", wc_str);

    return 0;
}
```

---

## 宽字符串（Wide Strings）

### 头文件：`<wchar.h>`（C95 起）

使用固定宽度字符类型 `wchar_t` 表示字符，便于处理国际化文本。

#### 函数概览

| 函数 | 功能 |
|------|------|
| `wcscpy()` / `wcsncpy()` | 宽字符串复制 |
| `wcscat()` / `wcsncat()` | 宽字符串拼接 |
| `wcscmp()` / `wcsncmp()` | 宽字符串比较 |
| `wcslen()` | 宽字符串长度 |
| `wcschr()` / `wcsrchr()` | 查找宽字符 |
| `wcsstr()` | 查找宽子串 |
| `wcstok()` | 分割宽字符串 |
| `wmemcpy()` / `wmemset()` | 宽内存操作 |
| `wprintf()` / `wscanf()` | 宽格式化输入输出 |

#### 示例代码

```c
#include <stdio.h>
#include <wchar.h>

int main() {
    wchar_t src[] = L"Hello";
    wchar_t dest[20];

    wcscpy(dest, src);              // 宽字符串复制
    wcscat(dest, L" World");        // 宽字符串拼接
    wprintf(L"Wide string: %ls\n", dest);  // 宽输出

    return 0;
}
```

---

## 常见安全增强函数（C11）

C11 引入一系列 `_s` 后缀的安全字符串处理函数，防止缓冲区溢出等问题。

#### 示例列表

| 原函数 | 安全版本（C11） |
|--------|------------------|
| `strcpy()` | `strcpy_s()` |
| `strcat()` | `strcat_s()` |
| `sprintf()` | `sprintf_s()` |
| `gets()` | `gets_s()`（现已废弃） |

#### 示例代码

```c
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>

int main() {
    char dest[10];
    const char *src = "Too long string";

    if (strcpy_s(dest, sizeof(dest), src) != 0) {
        printf("Copy failed due to buffer overflow.\n");
    }

    return 0;
}
```

---

## 字符串处理最佳实践

| 场景 | 推荐使用 |
|------|----------|
| 英文系统/简单文本 | 字节字符串 `<string.h>` |
| 多语言 / Unicode 支持 | 宽字符串 `<wchar.h>` |
| UTF-8 兼容处理 | 多字节转换函数 `<wchar.h>` |
| 安全开发 | 使用 `_s` 系列函数（C11） |
| 性能敏感环境 | 避免频繁字符串分配/拼接，使用内存池或静态缓冲 |

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 字符串库 | [C++ Strings Library](https://zh.cppreference.com/w/cpp/string) |
| C 宽字符与多字节处理 | [C Wide and Multibyte Character Handling](https://zh.cppreference.com/w/c/string/wide) |
| C 类型泛型宏 | [Type-generic macros (tgmath.h)](https://zh.cppreference.com/w/c/numeric/tgmath) |

---

> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=c/string&oldid=178895>  
> 最后修改时间：2024年12月31日 09:16 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

✅ **小结提示**：
- 字符串处理应选择合适的类型：字节/多字节/宽字符；
- 安全函数 `_s` 系列为 C11 扩展，需启用编译器支持；
- 多语言项目推荐使用宽字符或 UTF-8 编码结合多字节转换；
- 操作字符串时务必检查边界，防止缓冲区溢出漏洞；
- 推荐使用 `strn*` 系列替代 `str*` 以增加安全性。

如有更详细的需求（如正则表达式、JSON 解析），可借助第三方库如 PCRE、cJSON 等。