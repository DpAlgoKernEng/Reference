# 日期与时间工具（Date and time utilities）

来源：[cppreference.com](https://en.cppreference.com)

---

## 日期与时间工具函数一览表

| 分类 | 函数名 | 简要说明 |
|------|--------|----------|
| **时间操作** | `difftime` | 计算两个时间之间的差值（单位：秒） |
|               | `time`     | 获取从纪元开始的系统日历时间 |
|               | `clock`    | 获取程序启动以来的 CPU 时钟周期 |
|               | `timespec_get`(C11) | 获取以秒和纳秒表示的日历时间 |
|               | `timespec_getres`(C23) | 获取基于指定时间基的时间分辨率 |
| **格式转换** | `asctime` / `asctime_s`(C11, C23废弃) | 将 `tm` 结构体转换为字符串形式 |
|               | `ctime` / `ctime_s`(C11, C23废弃) | 将 `time_t` 转换为字符串形式 |
|               | `strftime` | 根据用户格式化规则生成文本表示 |
|               | `wcsftime`(C95) | 宽字符版本的 `strftime` |
|               | `gmtime` / `gmtime_r` / `gmtime_s`(C23废弃) | 将时间戳转换为 UTC 时间 |
|               | `localtime` / `localtime_r` / `localtime_s`(C23废弃) | 将时间戳转换为本地时间 |
|               | `mktime`   | 将本地时间结构转换回时间戳 |
| **常量定义** | `CLOCKS_PER_SEC` | 每秒处理器时钟滴答数（宏常量） |
| **类型定义** | `tm`         | 表示日历时间的结构体类型 |
|               | `time_t`     | 从纪元开始的时间类型 |
|               | `clock_t`    | 处理器时间类型 |
|               | `timespec`(C11) | 以秒和纳秒表示的时间类型 |

---

## 函数详解与使用说明

### 时间操作类函数

以下函数定义在 `<time.h>` 中：

| 函数 | 功能描述 |
|------|----------|
| `double difftime(time_t time1, time_t time0);` | 返回 `time1 - time0` 的差值，单位为秒。 |
| `time_t time(time_t *arg);` | 返回自 Unix 纪元（1970-01-01 00:00:00 UTC）以来的秒数。若 `arg` 不为 NULL，则也写入该地址。 |
| `clock_t clock(void);` | 返回程序启动以来的 CPU 时间（以处理器时钟滴答计）。常用于性能测试。 |
| `int timespec_get(struct timespec *ts, int base);`(C11) | 将当前日历时间以秒和纳秒的形式存入 `ts`。参数 `base` 决定时间起点（如 `TIME_UTC`）。返回非零表示成功。 |
| `int timespec_getres(struct timespec *res, int base);`(C23) | 获取指定时间基的最小可分辨时间间隔，写入 `res`。 |

### 格式转换函数

#### 文本表示转换函数

| 函数 | 功能描述 |
|------|----------|
| `char *asctime(const struct tm *timeptr);`<br>`errno_t asctime_s(char *s, rsize_t maxsize, const struct tm *timeptr);`(C11, C23废弃) | 将 `tm` 结构体转换为固定格式的字符串。例如："Wed Jun 30 21:49:08 1993\n"。`asctime_s` 为安全版本。 |
| `char *ctime(const time_t *timer);`<br>`errno_t ctime_s(char *buffer, rsize_t bufsz, const time_t *timer);`(C11, C23废弃) | 将 `time_t` 时间戳转换为与 `asctime` 相同格式的字符串。实际会调用 `localtime` 和 `asctime`。 |
| `size_t strftime(char *str, size_t count, const char *format, const struct tm *timeptr);` | 使用 `format` 格式化 `tm` 结构体为自定义字符串。支持类似 printf 的格式化选项，如 `%Y`, `%m`, `%d`, `%H`, `%M`, `%S` 等。 |
| `size_t wcsftime(wchar_t *wcs, size_t maxsize, const wchar_t *format, const struct tm *timeptr);`(C95) | 宽字符版本的 `strftime`，适用于多语言环境。 |

#### 时间转换函数

| 函数 | 功能描述 |
|------|----------|
| `struct tm *gmtime(const time_t *timer);`<br>`struct tm *gmtime_r(const time_t *restrict timer, struct tm *restrict result);`<br>`errno_t gmtime_s(struct tm *result, const time_t *timer);`(C11, C23废弃) | 将时间戳 `timer` 转换为协调世界时（UTC）表示的日历结构。线程安全版本为 `gmtime_r` 或 `gmtime_s`。 |
| `struct tm *localtime(const time_t *timer);`<br>`struct tm *localtime_r(const time_t *restrict timer, struct tm *restrict result);`<br>`errno_t localtime_s(struct tm *result, const time_t *timer);`(C11, C23废弃) | 将时间戳转换为本地时间表示的 `tm` 结构。线程安全版本分别为 `localtime_r` 和 `localtime_s`。 |
| `time_t mktime(struct tm *timeptr);` | 反向转换：将 `tm` 结构体（本地时间）转换为从纪元开始的时间戳。若转换失败返回 `(time_t)-1`。 |

---

## 常量与类型定义

### 常量

| 常量名 | 含义 |
|--------|------|
| `CLOCKS_PER_SEC` | `clock()` 函数返回值每秒所包含的处理器时钟滴答次数。通常为 1,000,000（即 1 MHz）。 |

### 数据类型

| 类型名 | 描述 |
|--------|------|
| `struct tm` | 包含年、月、日、时、分、秒等字段的日历时间结构体。 |
| `time_t`    | 表示自纪元以来的秒数，通常是一个算术类型（如 `long` 或 `long long`）。 |
| `clock_t`   | 表示处理器时间，常用于测量执行时间。 |
| `struct timespec`(C11) | 表示以秒和纳秒为单位的精确时间值，包含 `tv_sec` 和 `tv_nsec` 字段。 |

---

## 技术解析与示例代码

### 示例 1：获取并打印当前时间

```c
#include <stdio.h>
#include <time.h>

int main() {
    time_t now;
    time(&now);
    printf("当前时间戳：%ld\n", now);

    struct tm *local = localtime(&now);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
    printf("本地时间：%s\n", buffer);

    return 0;
}
```

**输出示例：**
```
当前时间戳：1710978543
本地时间：2024-03-20 20:49:03
```

### 示例 2：计算两个时间点的差值

```c
#include <stdio.h>
#include <time.h>
#include <unistd.h> // for sleep()

int main() {
    time_t start = time(NULL);
    sleep(2); // 模拟耗时操作
    time_t end = time(NULL);

    double elapsed = difftime(end, start);
    printf("经过时间：%.2f 秒\n", elapsed);

    return 0;
}
```

**输出示例：**
```
经过时间：2.00 秒
```

---

## 标准文档参考

- **C17 标准 (ISO/IEC 9899:2018)**：
  - §7.27：日期与时间 `<time.h>`（页码：284–291）
  - §7.29.5.1：`wcsftime` 函数（页码：320–321）
  - §7.31.14：附加日期时间相关函数（页码：333）

- **C11 标准 (ISO/IEC 9899:2011)**：
  - §7.27：日期与时间 `<time.h>`（页码：388–397）
  - §7.29.5.1：`wcsftime` 函数（页码：439–440）
  - §7.31.14：日期与时间附加函数（页码：456）

- **C99 标准 (ISO/IEC 9899:1999)**：
  - §7.23：日期与时间 `<time.h>`（页码：338–347）
  - §7.24.5.1：`wcsftime` 函数（页码：385–386）

- **C89/C90 标准 (ISO/IEC 9899:1990)**：
  - §4.12：日期与时间 `<time.h>`

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 日期与时间库文档 | [C++ Chrono library](https://zh.cppreference.com/w/cpp/chrono) |

---

> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=c/chrono&oldid=143967>  
> 页面最后更新时间：2022年10月2日 05:04 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

**注意事项**：
- 所有时间函数依赖系统的时区设置。
- 使用 `asctime`, `ctime` 等函数时应避免线程不安全性，推荐使用 `_r` 或 `_s` 版本。
- 自 C23 起，`asctime` 和 `ctime` 已被标记为弃用。