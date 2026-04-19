# 标准库符号索引（Symbol Index）

来源：[cppreference.com](https://en.cppreference.com)

---

本页面列出 **C 标准库** 中所有公开的符号，供快速查阅。

> 📌 **说明**：
> - 函数名结尾带有 `()`；
> - 通用函数名（如原子操作）标记为 (generic)；
> - 宏名称列在单独页面中；
> - 所有符号来自 `<stdio.h>`、`<stdlib.h>`、`<string.h>`、`<math.h>`、`<time.h>`、`<wchar.h>`、`<threads.h>` 等标准头文件。

---

## 符号列表（按字母顺序）

### _ (下划线)

| 符号 | 描述 |
|------|------|
| `_Exit()` | 立即终止程序（C99） |

### A

| 符号 | 描述 |
|------|------|
| `abort()` | 强制异常终止程序 |
| `abort_handler_s()` | 安全版本终止处理器（C11） |
| `abs()` | 整数绝对值 |
| `acos()` / `acosf()` / `acosl()` | 反余弦函数 |
| `acosh()` 等 | 反双曲余弦函数（C99） |
| `aligned_alloc()` | 对齐内存分配（C11） |
| `asctime()` / `asctime_s()` | 时间结构体转字符串（C23废弃） |
| `asin()` 等 | 反正弦函数 |
| `asinh()` 等 | 反双曲正弦函数 |
| `atan()` 等 | 反正切函数 |
| `atan2()` 等 | 四象限反正切函数 |
| `at_quick_exit()` | 快速退出处理注册（C11） |
| `atexit()` | 终止函数注册 |
| `atof()` / `atoi()` / `atol()` / `atoll()` | 字符串转数值类型 |

#### 原子操作类型（C11）

```plaintext
atomic_bool, atomic_char, atomic_char16_t, atomic_char32_t,
atomic_int, atomic_int_fast16_t, atomic_int_fast32_t,
atomic_int_fast64_t, atomic_int_fast8_t, atomic_int_least16_t,
atomic_int_least32_t, atomic_int_least64_t, atomic_int_least8_t,
atomic_intmax_t, atomic_intptr_t, atomic_llong, atomic_long,
atomic_ptrdiff_t, atomic_schar, atomic_short, atomic_size_t,
atomic_uchar, atomic_uint, atomic_uint_fast16_t, atomic_uint_fast32_t,
atomic_uint_fast64_t, atomic_uint_fast8_t, atomic_uint_least16_t,
atomic_uint_least32_t, atomic_uint_least64_t, atomic_uint_least8_t,
atomic_uintmax_t, atomic_uintptr_t, atomic_ullong, atomic_ulong,
atomic_ushort, atomic_wchar_t
```

#### 原子操作函数（C11）

```plaintext
atomic_compare_exchange_strong (generic)
atomic_compare_exchange_strong_explicit (generic)
atomic_compare_exchange_weak (generic)
atomic_compare_exchange_weak_explicit (generic)
atomic_exchange (generic)
atomic_exchange_explicit (generic)
atomic_fetch_add (generic)
atomic_fetch_add_explicit (generic)
atomic_fetch_and (generic)
atomic_fetch_and_explicit (generic)
atomic_fetch_or (generic)
atomic_fetch_or_explicit (generic)
atomic_fetch_sub (generic)
atomic_fetch_sub_explicit (generic)
atomic_fetch_xor (generic)
atomic_fetch_xor_explicit (generic)
atomic_flag
atomic_flag_clear
atomic_flag_clear_explicit
atomic_flag_test_and_set
atomic_flag_test_and_set_explicit
atomic_init (generic)
atomic_is_lock_free (generic)
atomic_load (generic)
atomic_load_explicit (generic)
atomic_signal_fence
atomic_store (generic)
atomic_store_explicit (generic)
atomic_thread_fence
```

---

### B

| 符号 | 描述 |
|------|------|
| `bsearch()` / `bsearch_s()` | 二分查找 |
| `btowc()` | 单字节转宽字符（C95） |

---

### C

| 符号 | 描述 |
|------|------|
| `c16rtomb()` / `c32rtomb()` | 多字节转换（C11） |
| `cabs()` 等 | 复数绝对值（C99） |
| `cacos()` 等 | 复数反余弦（C99） |
| `call_once()` | 一次性调用机制（C11） |
| `calloc()` | 分配并清零内存 |
| `carg()` 等 | 复数幅角（C99） |
| `casin()` 等 | 复数反正弦（C99） |
| `catan()` 等 | 复数反正切（C99） |
| `cbrt()` 等 | 立方根（C99） |
| `ccos()` 等 | 复数余弦（C99） |
| `ceil()` 等 | 向上取整 |
| `cexp()` 等 | 复数指数（C99） |
| `char16_t` / `char32_t` | Unicode 字符类型（C11） |
| `cimag()` 等 | 复数虚部（C99） |
| `clearerr()` | 清除流错误标志 |
| `clock()` | 获取处理器时钟时间 |
| `clock_t` | 处理器时间类型 |
| `clog()` 等 | 复数对数（C99） |
| `conj()` 等 | 复数共轭（C99） |
| `copysign()` 等 | 拷贝符号（C99） |
| `cos()` 等 | 三角函数 |
| `cpow()` 等 | 复数幂（C99） |
| `cproj()` 等 | 黎曼球面投影（C99） |
| `creal()` 等 | 复数实部（C99） |
| `csin()` 等 | 复数正弦（C99） |
| `csqrt()` 等 | 复数平方根（C99） |
| `ctan()` 等 | 复数正切（C99） |
| `ctime()` / `ctime_s()` | 时间戳转字符串（C23废弃） |

---

### D

| 符号 | 描述 |
|------|------|
| `difftime()` | 计算时间差 |
| `div()` | 整数除法结构体 |
| `div_t` | div 函数返回类型 |
| `double_t` | 双精度浮点数类型（C99） |

---

### E

| 符号 | 描述 |
|------|------|
| `erf()` 等 | 误差函数（C99） |
| `erfc()` 等 | 余补误差函数（C99） |
| `errno_t` | 错误码类型（C11） |
| `exit()` | 正常终止程序 |
| `exp()` 等 | 自然指数函数 |
| `exp2()` 等 | 2 的幂次（C99） |
| `expm1()` 等 | e^x - 1（C99） |

---

### F

| 符号 | 描述 |
|------|------|
| `fabs()` 等 | 浮点绝对值 |
| `fclose()` | 关闭流 |
| `fdim()` 等 | 正差最大值（C99） |
| `feclearexcept()` 等 | 浮点异常控制（C99） |
| `fegetenv()` 等 | 浮点环境管理（C99） |
| `feof()` | 检查文件结束 |
| `ferror()` | 检查错误标志 |
| `fflush()` | 刷新输出缓冲区 |
| `fgetc()` 等 | 字符读取 |
| `fgets()` 等 | 行读取 |
| `FILE` | 流对象类型 |
| `float_t` | 浮点类型（C99） |
| `floor()` 等 | 向下取整 |
| `fma()` 等 | 融合乘加运算（C99） |
| `fmax()` 等 | 浮点最大值（C99） |
| `fmin()` 等 | 浮点最小值（C99） |
| `fmod()` 等 | 浮点模运算 |
| `fopen()` / `fopen_s()` | 打开文件 |
| `fpos_t` | 流位置类型 |
| `fprintf()` / `fprintf_s()` | 格式化输出到流 |
| `fputc()` / `fputs()` | 字符/字符串写入 |
| `fread()` | 二进制读取 |
| `free()` | 释放内存 |
| `freopen()` / `freopen_s()` | 重定向流 |
| `frexp()` 等 | 将浮点数分解成尾数和指数 |
| `fscanf()` / `fscanf_s()` | 格式化输入 |
| `fseek()` / `fsetpos()` | 定位文件指针 |
| `ftell()` | 获取当前位置 |
| `fwide()` | 设置字符宽度模式（C95） |
| `fwprintf()` 等 | 宽字符输出（C95） |
| `fwrite()` | 二进制写入 |
| `fwscanf()` 等 | 宽字符输入（C95） |

---

### G

| 符号 | 描述 |
|------|------|
| `getc()` / `getchar()` | 标准输入读取 |
| `getenv()` / `getenv_s()` | 获取环境变量 |
| `gets()` / `gets_s()` | 不安全输入（C11废弃） |
| `getwc()` / `getwchar()` | 宽字符输入（C95） |
| `gmtime()` / `gmtime_r()` / `gmtime_s()` | UTC 时间转换 |

---

### H

| 符号 | 描述 |
|------|------|
| `hypot()` 等 | 平方和开方（C99） |

---

### I

| 符号 | 描述 |
|------|------|
| `ignore_handler_s()` | 忽略安全错误处理（C11） |
| `ilogb()` 等 | 提取指数部分（C99） |
| `imaxabs()` / `imaxdiv()` | 最大整数绝对值/除法（C99） |
| `imaxdiv_t` / `intmax_t` | 最大整数类型（C99） |
| `int_fast16_t` 等 | 快速整数类型（C99） |
| `isalnum()` 等 | 字符分类 |
| `iswalnum()` 等 | 宽字符分类（C95） |

---

### J

| 符号 | 描述 |
|------|------|
| `jmp_buf` | 非局部跳转缓冲区 |

---

### L

| 符号 | 描述 |
|------|------|
| `labs()` | 长整数绝对值 |
| `lconv` | 本地数字格式结构体 |
| `ldexp()` 等 | x * 2^n |
| `ldiv()` / `ldiv_t` | 长整数除法 |
| `lgamma()` 等 | 对数伽马函数（C99） |
| `llabs()` / `lldiv()` / `lldiv_t` | 更长整数操作（C99） |
| `llrint()` 等 | 四舍五入为整数（C99） |
| `localeconv()` | 本地数字格式信息 |
| `localtime()` / `localtime_r()` / `localtime_s()` | 本地时间转换 |
| `log()` 等 | 对数函数 |
| `log10()` 等 | 以 10 为底 |
| `log1p()` 等 | ln(1+x)（C99） |
| `log2()` 等 | 以 2 为底（C99） |
| `logb()` 等 | 浮点指数部分（C99） |
| `longjmp()` | 非局部跳转 |
| `lrint()` / `lround()` 等 | 转换为整数（C99） |

---

### M

| 符号 | 描述 |
|------|------|
| `malloc()` | 动态内存分配 |
| `max_align_t` | 最大对齐类型（C11） |
| `mblen()` 等 | 多字节字符处理（C95） |
| `memchr()` 等 | 内存操作 |
| `memory_order` 等 | 内存序常量（C11） |
| `memset()` 等 | 填充内存 |
| `memset_explicit()` | 显式内存填充（C23） |
| `mktime()` | 日历时间转时间戳 |
| `modf()` 等 | 分解浮点数 |
| `mtx_destroy()` 等 | 互斥量操作（C11） |

---

### N

| 符号 | 描述 |
|------|------|
| `nan()` 等 | NaN 值生成（C99） |
| `nearbyint()` 等 | 四舍五入（C99） |
| `nextafter()` 等 | 获取下一个浮点数（C99） |

---

### O

| 符号 | 描述 |
|------|------|
| `once_flag` | 一次性调用标志（C11） |

---

### P

| 符号 | 描述 |
|------|------|
| `perror()` | 打印错误信息 |
| `pow()` 等 | 幂函数 |
| `printf()` / `printf_s()` | 标准输出格式化 |
| `ptrdiff_t` | 指针差类型 |
| `putc()` / `putchar()` / `puts()` | 输出字符/字符串 |
| `putwc()` 等 | 宽字符输出（C95） |

---

### Q

| 符号 | 描述 |
|------|------|
| `qsort()` / `qsort_s()` | 快速排序 |
| `quick_exit()` | 快速程序退出（C11） |

---

### R

| 符号 | 描述 |
|------|------|
| `raise()` | 发送信号 |
| `rand()` / `srand()` | 伪随机数 |
| `realloc()` | 重新分配内存 |
| `remainder()` 等 | 余数函数（C99） |
| `remove()` / `rename()` | 文件操作 |
| `rewind()` | 回到文件头部 |
| `rint()` 等 | 四舍五入（C99） |
| `round()` 等 | 四舍五入（C99） |
| `rsize_t` | 安全范围大小（C11） |

---

### S

| 符号 | 描述 |
|------|------|
| `scalbln()` 等 | x * 2^n（C99） |
| `scanf()` / `scanf_s()` | 格式化输入 |
| `set_constraint_handler_s()` | 设置安全错误处理（C11） |
| `setbuf()` / `setvbuf()` | 设置流缓冲 |
| `sig_atomic_t` | 原子信号类型 |
| `signal()` | 注册信号处理函数 |
| `sin()` 等 | 三角函数 |
| `snprintf()` 等 | 格式化安全输出（C99） |
| `sqrt()` 等 | 平方根 |
| `sscanf()` 等 | 字符串解析 |
| `strcat()` 等 | 字符串操作 |
| `strchr()` 等 | 字符串搜索 |
| `strcmp()` 等 | 字符串比较 |
| `strcpy()` 等 | 字符串复制 |
| `strdup()` / `strndup()` | 字符串复制（C23） |
| `strerror()` / `strerror_s()` | 错误信息字符串 |
| `strftime()` | 时间格式化 |
| `strlen()` 等 | 字符串长度相关 |
| `strtod()` 等 | 字符串转浮点 |
| `strtoimax()` 等 | 字符串转大整数（C99） |
| `strtok()` 等 | 字符串分割 |
| `strtol()` 等 | 字符串转整数 |
| `strtoumax()` 等 | 字符串转无符号大整数（C99） |
| `swprintf()` 等 | 宽字符格式化（C95） |

---

### T

| 符号 | 描述 |
|------|------|
| `tan()` 等 | 三角函数 |
| `tgamma()` 等 | 伽马函数（C99） |
| `thrd_create()` 等 | 线程操作（C11） |
| `time()` | 获取当前时间 |
| `time_t` | 时间戳类型 |
| `timespec` / `timespec_get()`（C11） |
| `tm` | 时间结构体 |
| `tmpfile()` / `tmpnam()` 等 | 临时文件操作 |
| `tolower()` / `toupper()` | 字符大小写转换 |
| `towlower()` 等 | 宽字符大小写转换（C95） |
| `trunc()` 等 | 截断小数部分（C99） |
| `tss_create()` 等 | 线程局部存储（C11） |

---

### U

| 符号 | 描述 |
|------|------|
| `uint_fast16_t` 等 | 无符号快速整数类型（C99） |
| `ungetc()` / `ungetwc()` | 回退字符 |

---

### V

| 符号 | 描述 |
|------|------|
| `vfprintf()` 等 | 可变参数版本 |
| `vfwprintf()` 等 | 宽字符可变参数版本（C95） |
| `vprintf()` 等 | printf 的可变参数版本 |
| `vsnprintf()` 等 | 安全版本（C99） |
| `vswprintf()` 等 | 宽字符版本（C95） |
| `vwprintf()` 等 | 宽字符输出（C95） |

---

### W

| 符号 | 描述 |
|------|------|
| `wchar_t` | 宽字符类型 |
| `wcrtomb()` 等 | 字符转换（C95） |
| `wcscat()` 等 | 宽字符串操作（C95） |
| `wcscmp()` 等 | 宽字符串比较 |
| `wcsftime()` | 宽字符串时间格式化（C95） |
| `wcslen()` 等 | 宽字符串长度 |
| `wcstod()` 等 | 宽字符串转数值（C95） |
| `wcstok()` 等 | 宽字符串分割 |
| `wcsxfrm()` 等 | 宽字符串变换 |
| `wctob()` 等 | 宽字符转单字节（C95） |
| `wctype()` 等 | 宽字符类别（C95） |
| `wmemcpy()` 等 | 宽内存操作（C95） |

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 符号索引 | [C++ Symbol Index](https://zh.cppreference.com/w/cpp/symbol_index) |

---

> 页面获取时间：2025年2月9日 16:39  
> 最后更新时间：2024年5月29日 03:13 UTC  
> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=c/symbol_index&oldid=172123>

--- 

✅ **小结提示**：
- 非安全函数如 `gets()` 已被移除，请优先使用 `_s` 版本。
- C11 引入原子操作与线程支持，提升了并发能力。
- 宽字符系列函数适用于国际化应用开发。
- 浮点运算函数（如 `log`, `exp`, `sin`）需包含 `<math.h>`。