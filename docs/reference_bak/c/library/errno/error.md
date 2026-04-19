# 错误处理（Error handling）

来源：[cppreference.com](https://en.cppreference.com)

---

## 错误处理组件一览表

| 功能分类 | 成员 | 简要描述 |
|----------|------|----------|
| **错误码机制** | `errno` 系列宏 | 获取函数调用错误的状态变量 |
|                | 错误码常量（如 `EACCES`、`EINVAL`） | 标准 POSIX 错误码枚举 |
| **断言机制** | `assert` | 程序开发期检查条件并终止程序 |
|              | `static_assert`(C11, C23移除) | 编译期静态断言检查 |
| **边界检查功能** | `set_constraint_handler_s`(C11) | 设置边界检查失败时的回调函数 |
|                 | `abort_handler_s`(C11) | 默认中止程序的回调实现 |
|                 | `ignore_handler_s`(C11) | 忽略错误但记录日志的回调实现 |

---

## 错误码（Error Numbers）

定义于 `<errno.h>` 头文件中：

| 名称 | 描述 |
|------|------|
| `extern int errno;`（宏扩展形式） | 当函数发生错误时设置的线程局部变量，值为标准 POSIX 错误码之一。 |
| `E2BIG`, `EACCES`, ..., `EXDEV` | 一组表示标准错误条件的宏定义，便于跨平台错误诊断。 |

> ⚠️ **注意**：
> - `errno` 是一个**线程安全**的变量（TLS），每个线程拥有独立实例。
> - 调用函数前需先清零 `errno = 0`，调用后再检测其值判断是否出错。
> - 常见错误码说明（部分）：
>   - `EACCES`：访问被拒绝；
>   - `ENOENT`：文件不存在；
>   - `EINVAL`：无效参数；
>   - `ENOMEM`：内存不足。

---

## 断言（Assertions）

定义于 `<assert.h>` 头文件中：

| 名称 | 描述 |
|------|------|
| `void assert(int expression);`（宏） | 若表达式为假，则在标准错误输出打印诊断信息并终止程序；可使用 `-DNDEBUG` 编译选项关闭断言。 |
| `_Static_assert`(C11), `static_assert`(C23起为关键字) | 在编译期间验证常量表达式是否为真，否则报错。 |

### 示例代码：

```c
#include <stdio.h>
#include <assert.h>

int main() {
    int x = 5;
    assert(x > 0); // 正确，不会中断
    assert(x < 0); // 错误，触发断言失败并终止程序

    return 0;
}
```

**编译运行示例：**
```bash
$ gcc -g test.c -o test
$ ./test
test: test.c:6: main: Assertion `x < 0' failed.
Aborted (core dumped)
```

---

## 边界检查功能（Bounds Checking）

从 C11 开始提供边界检查增强接口，作为可选项存在。

### 触发条件

边界检查功能只在定义 `__STDC_LIB_EXT1__` 宏时启用，表示当前实现支持 ISO/IEC TR 24731-1:2005 标准。

### 关键类型与常量定义

| 名称 | 描述 |
|------|------|
| `typedef int errno_t;` | 用于返回错误码的整型别名，提升代码可读性 |
| `typedef size_t rsize_t;` | 一种受限制的 `size_t` 类型，用于运行期参数范围检查 |
| `#define RSIZE_MAX (~(rsize_t)0 >> 1)` | 可接受的最大 `rsize_t` 值，通常为 `(SIZE_MAX / 2)` |

### 约束处理函数

| 函数 | 描述 |
|------|------|
| `void set_constraint_handler_s(void (*handler)(const char * restrict msg, void * restrict ptr, errno_t error));` | 注册用户自定义的错误处理函数，在边界检查失败时调用 |
| `void abort_handler_s(const char * restrict msg, void * restrict ptr, errno_t error);` | 默认处理方式：将错误信息输出到 `stderr` 并调用 `abort()` 终止程序 |
| `void ignore_handler_s(const char * restrict msg, void * restrict ptr, errno_t error);` | 忽略错误但将错误消息写入日志 |

### 示例代码：

```c
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

int main() {
#ifdef __STDC_LIB_EXT1__
    char src[20] = "Hello";
    char dest[5];

    errno_t err = strcpy_s(dest, sizeof(dest), src);
    if (err != 0) {
        fprintf(stderr, "Error occurred: %d\n", err);
        return 1;
    }
#else
    puts("Bounds-checking interface not supported.");
#endif

    return 0;
}
```

> ⚠️ 目前主流编译器（GCC/Clang）尚未完整支持此功能，替代方案可以使用开源库如 **Safe C Library (Safe C)**、**Microsoft's Safer CRT** 或 **Watcom C** 提供的版本。

---

## 标准文档参考

- **C23 标准 (ISO/IEC 9899:2024)**：
  - §7.2 Diagnosis facilities `<assert.h>`
  - §7.5 Error numbers `<errno.h>`
  - §7.19 Common definitions `<stddef.h>`
  - §K.3 Bounds-checking interfaces

- **C17 标准 (ISO/IEC 9899:2018)**：
  - 同上，但部分章节页码暂未公开

- **C11 标准 (ISO/IEC 9899:2011)**：
  - §7.2 Diagnostics `<assert.h>`（页码：186–187）
  - §7.5 Errors `<errno.h>`（页码：205）
  - §K.3 Bounds-checking interfaces（页码：584–585）

- **C99 标准 (ISO/IEC 9899:1999)**：
  - §7.2 Diagnostics
  - §7.5 Errors

- **C89/C90 标准 (ISO/IEC 9899:1990)**：
  - §4.2 Diagnostics `<assert.h>`
  - §4.1.3 Errors `<errno.h>`

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| `math_errhandling`, `MATH_ERRNO`, `MATH_ERREXCEPT`(C99) | 数学函数错误处理模式控制宏 |
| C++ 错误处理机制文档 | [C++ Error handling](https://zh.cppreference.com/w/cpp/error) |

---

> 页面获取时间：2025年2月9日 16:39  
> 最后修订时间：2025年2月1日 21:06 UTC  
> 源地址：<https://en.cppreference.com/mwiki/index.php?title=c/error&oldid=180038>

--- 

✅ **小结提示**：
- 使用 `errno` 时务必先置零再检测。
- `assert` 适合开发调试阶段，生产环境可通过 `-DNDEBUG` 屏蔽。
- C11 的边界检查功能是面向安全编程的重要特性，尽管目前支持有限。
- 静态断言 `static_assert`（或 `_Static_assert`）有助于编译期验证配置和结构体大小一致性。