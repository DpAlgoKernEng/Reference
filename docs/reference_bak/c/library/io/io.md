# 文件输入/输出（File input/output）

来源：[cppreference.com](https://en.cppreference.com)

---

## 文件 I/O 函数总览表

### 核心类型与对象

| 类型/对象 | 功能描述 |
|---------|---------|
| `FILE` | 控制 C 标准 I/O 流的对象类型 |
| `fpos_t` | 表示文件位置和多字节解析状态的完整类型 |
| `stdin`, `stdout`, `stderr` | 预定义的标准输入、输出与错误流指针 |

### 文件操作函数分类列表

| 分类 | 函数 | 简要说明 |
|------|------|---------|
| **文件访问控制** | `fopen`, `fopen_s`(C11) | 打开文件 |
|                 | `freopen`, `freopen_s`(C11) | 重定向已打开的流 |
|                 | `fclose` | 关闭文件 |
|                 | `fflush` | 刷新输出缓冲区到文件 |
|                 | `setbuf`, `setvbuf` | 设置缓冲方式 |
|                 | `fwide`(C95) | 指定流是宽字符还是窄字符模式 |
| **直接读写操作** | `fread`, `fwrite` | 二进制方式读写数据块 |
| **格式化输入输出** | `scanf` 系列, `printf` 系列 | 格式化输入输出 |
| **非格式化读写** | `fgetc`, `fgets`, `fputc`, `fputs` | 单字符或字符串读写 |
|                     | `getchar`, `gets`(C11移除), `putchar`, `puts` | 标准输入输出操作 |
|                     | `ungetc` | 回退一个字符 |
|                     | `fgetwc`, `fgetws`, `fputwc`, `fputws`(C95) | 宽字符版本 |
|                     | `getwchar`, `putwchar`(C95) | 宽字符标准流操作 |
|                     | `ungetwc`(C95) | 回退一个宽字符 |
| **文件定位** | `ftell`, `fgetpos` | 获取当前位置 |
|               | `fseek`, `fsetpos`, `rewind` | 移动文件指针 |
| **错误处理** | `clearerr`, `feof`, `ferror` | 清除错误标志或检测错误 |
|               | `perror` | 显示错误消息 |
| **文件操作** | `remove`, `rename` | 删除或重命名文件 |
|               | `tmpfile`, `tmpnam`(C11移除), `tmpnam_s`(C11) | 临时文件操作 |

---

## 类型定义详解

### 标准流对象

定义于 `<stdio.h>` 中：

| 名称 | 描述 |
|------|------|
| `stdin` | 标准输入流指针（键盘等输入设备） |
| `stdout` | 标准输出流指针（屏幕等输出设备） |
| `stderr` | 标准错误输出流指针（用于错误信息） |

### 关键结构体与类型

| 类型 | 描述 |
|------|------|
| `typedef struct _iobuf FILE;` | 表示 I/O 流的数据结构，只能通过指针访问 |
| `typedef long fpos_t;` | 保存文件当前位置信息的类型 |

---

## 文件操作函数详解

### 1. 文件访问类函数

```c
FILE *fopen(const char *filename, const char *mode);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
int fclose(FILE *stream);
int fflush(FILE *stream);
void setbuf(FILE *stream, char *buffer);
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);
```

#### 示例代码：

```c
#include <stdio.h>

int main() {
    // 打开一个文件进行写入
    FILE *fp = fopen("example.txt", "w");
    if (!fp) {
        perror("打开文件失败");
        return 1;
    }

    // 写入内容
    fprintf(fp, "Hello, 文件操作!\n");
    fclose(fp);

    return 0;
}
```

### 2. 非格式化读写函数

#### 窄字符函数：

```c
int fgetc(FILE *stream);
char *fgets(char *str, int n, FILE *stream);
int fputc(int ch, FILE *stream);
int fputs(const char *str, FILE *stream);
int getchar(void);
char *gets(char *str); // C11 已废弃
int putchar(int ch);
int puts(const char *str);
int ungetc(int ch, FILE *stream);
```

#### 宽字符函数（C95）：

```c
wint_t fgetwc(FILE *stream);
wchar_t *fgetws(wchar_t *str, int n, FILE *stream);
wint_t fputwc(wchar_t wc, FILE *stream);
int fputws(const wchar_t *str, FILE *stream);
wint_t getwchar(void);
wint_t putwchar(wchar_t wc);
wint_t ungetwc(wint_t wc, FILE *stream);
```

#### 示例代码：

```c
#include <stdio.h>

int main() {
    char line[256];
    FILE *fp = fopen("data.txt", "r");

    while (fgets(line, sizeof(line), fp)) {
        printf("读取一行: %s", line);
    }

    fclose(fp);
    return 0;
}
```

### 3. 格式化输入输出函数

#### 输入函数（`scanf` 系列）：

```c
int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int sscanf(const char *s, const char *format, ...);
```

#### 输出函数（`printf` 系列）：

```c
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t n, const char *format, ...); // C99+
```

#### 示例代码：

```c
#include <stdio.h>

int main() {
    char name[50];
    int age;

    // 读取用户输入
    printf("请输入姓名和年龄: ");
    scanf("%s %d", name, &age);
    printf("姓名: %s, 年龄: %d\n", name, age);

    // 输出到文件
    FILE *fp = fopen("output.txt", "w");
    fprintf(fp, "姓名: %s, 年龄: %d\n", name, age);
    fclose(fp);

    return 0;
}
```

---

## 宏常量详解

所有宏常量均定义于 `<stdio.h>` 中。

| 宏定义 | 说明 |
|-------|------|
| `EOF` | 文件结束标志（-1） |
| `FOPEN_MAX` | 平台支持的最大同时打开文件数 |
| `FILENAME_MAX` | 支持的最大文件名长度 |
| `BUFSIZ` | 缓冲区默认大小 |
| `_IOFBF`, `_IOLBF`, `_IONBF` | 缓冲模式：全缓存、行缓存、无缓存 |
| `SEEK_SET`, `SEEK_CUR`, `SEEK_END` | 文件定位方式：文件头、当前位置、文件尾 |
| `TMP_MAX`, `TMP_MAX_S`(C11) | 最大唯一临时文件名数量 |
| `L_tmpnam`, `L_tmpnam_s`(C11) | 临时文件名缓冲区长度 |

---

## 文件位置控制函数

```c
long ftell(FILE *stream);
int fgetpos(FILE *stream, fpos_t *pos);
int fseek(FILE *stream, long offset, int origin);
int fsetpos(FILE *stream, const fpos_t *pos);
void rewind(FILE *stream);
```

#### 示例代码：

```c
#include <stdio.h>

int main() {
    FILE *fp = fopen("data.txt", "rb");

    // 跳转到文件末尾
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    printf("文件大小：%ld 字节\n", file_size);

    // 回到文件开头
    rewind(fp);

    fclose(fp);
    return 0;
}
```

---

## 错误处理函数

```c
void clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
void perror(const char *msg);
```

#### 示例代码：

```c
#include <stdio.h>

int main() {
    FILE *fp = fopen("nonexistent.txt", "r");

    if (!fp) {
        perror("打开文件失败");
        return 1;
    }

    fclose(fp);
    return 0;
}
```

---

## 文件操作函数

```c
int remove(const char *filename);
int rename(const char *oldname, const char *newname);
FILE *tmpfile(void);
char *tmpnam(char *s);
errno_t tmpnam_s(char *s, rsize_t maxsize); // C11
```

---

## 示例：综合使用文件 I/O

```c
#include <stdio.h>
#include <string.h>

int main() {
    FILE *fp = fopen("students.csv", "w");

    if (!fp) {
        perror("无法创建文件");
        return 1;
    }

    // 写入标题
    fprintf(fp, "姓名,年龄,成绩\n");

    // 写入数据
    fprintf(fp, "张三,20,85\n");
    fprintf(fp, "李四,21,90\n");

    fclose(fp);

    // 读取并打印
    fp = fopen("students.csv", "r");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }
    fclose(fp);

    return 0;
}
```

---

## 标准文档参考

- **C23 标准 (ISO/IEC 9899:2024)**：
  - §7.21 Input/output `<stdio.h>`
  - §7.29 Extended multibyte and wide character utilities `<wchar.h>`
  - §K.3.5 Input/output security extensions

- **C17 标准 (ISO/IEC 9899:2018)**：
  - 同上

- **C11 标准 (ISO/IEC 9899:2011)**：
  - §7.21 Input/output `<stdio.h>`（第 296–339 页）
  - §7.29 Wide character I/O `<wchar.h>`（第 402–446 页）
  - §K.3.5 Secure I/O extensions（第 586–603 页）

- **C99 标准 (ISO/IEC 9899:1999)**：
  - §7.19 Input/output `<stdio.h>`（第 262–305 页）
  - §7.24 Wide character I/O `<wchar.h>`（第 348–392 页）

- **C89/C90 标准 (ISO/IEC 9899:1990)**：
  - §4.9 INPUT/OUTPUT `<stdio.h>`

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 文件 I/O（iostream） | [C++ I/O streams](https://zh.cppreference.com/w/cpp/io) |

---

> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=c/io&oldid=180208>  
> 页面最后修改时间：2025年2月6日 16:26 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

✅ **小结提示**：
- `gets()` 函数在 C11 中已被废弃，使用 `fgets()` 替代。
- 所有文件操作完成后务必调用 `fclose()`。
- 多线程程序中应避免多个线程共享同一 FILE 对象。
- 建议对文件操作进行错误检查（如返回值判断）。