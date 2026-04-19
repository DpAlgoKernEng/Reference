# 本地化支持（Localization support）

来源：[cppreference.com](https://en.cppreference.com)

---

## 简介

本地化（Localization）是一种根据用户的语言、地区或文化习惯调整程序行为的过程。C 语言通过 `<locale.h>` 头文件提供本地化功能，允许程序针对不同区域设置其格式和排序方式。

---

## 功能一览

| 函数/结构体 | 功能描述 |
|-------------|----------|
| `setlocale()` | 获取或设置程序的当前区域设置 |
| `localeconv()` | 查询数字与货币格式细节 |
| `lconv` | 包含格式化信息的结构体 |
| **区域分类宏** | 控制 `setlocale()` 操作范围 |
| `LC_ALL` | 设置所有本地化项 |
| `LC_COLLATE` | 影响字符串比较 |
| `LC_CTYPE` | 影响字符分类和转换 |
| `LC_MONETARY` | 影响货币格式 |
| `LC_NUMERIC` | 影响数字输出格式 |
| `LC_TIME` | 影响时间日期格式 |

---

## 本地化函数详解

### 1. `setlocale()`

```c
char *setlocale(int category, const char *locale);
```

#### 参数说明

| 参数 | 说明 |
|------|------|
| `category` | 指定要设置的本地化类别（如 `LC_ALL`） |
| `locale` | 指定区域名称，`""` 表示系统默认，`NULL` 表示查询当前设置 |

#### 示例代码

```c
#include <stdio.h>
#include <locale.h>

int main() {
    // 查询当前本地化设置
    printf("当前 LC_ALL 设置：%s\n", setlocale(LC_ALL, NULL));

    // 设置为系统默认区域
    setlocale(LC_ALL, "");

    // 设置为简体中文（Linux）
    if (setlocale(LC_ALL, "zh_CN.UTF-8") == NULL) {
        printf("不支持该区域设置\n");
    } else {
        printf("成功设置为：zh_CN.UTF-8\n");
    }

    return 0;
}
```

---

### 2. `localeconv()`

```c
struct lconv *localeconv(void);
```

> 返回一个指向 `lconv` 结构体的指针，包含当前区域下的格式规则。

#### 示例代码

```c
#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    struct lconv *lc = localeconv();

    printf("货币符号：%s\n", lc->int_curr_symbol);
    printf("千位分隔符：%s\n", lc->thousands_sep);
    printf("小数点符号：%s\n", lc->decimal_point);

    return 0;
}
```

---

### 3. `lconv` 结构体字段说明

| 字段名 | 类型 | 含义 |
|--------|------|------|
| `char *decimal_point` | 字符串 | 小数点符号 |
| `char *thousands_sep` | 字符串 | 千位分隔符 |
| `char *grouping` | 字符串 | 数字分组规则 |
| `char *int_curr_symbol` | 字符串 | 国际货币符号（如 USD） |
| `char *currency_symbol` | 字符串 | 货币符号（如 $） |
| `char *mon_decimal_point` | 字符串 | 货币小数点 |
| `char *mon_thousands_sep` | 字符串 | 货币千位分隔符 |
| `char *mon_grouping` | 字符串 | 货币分组规则 |
| `char *positive_sign` | 字符串 | 正数符号 |
| `char *negative_sign` | 字符串 | 负数符号 |
| `char frac_digits` | 整数 | 小数位数 |
| `char p_cs_precedes` | 整数 | 正币值前缀/后缀方式 |
| `char n_cs_precedes` | 整数 | 负币值前缀/后缀方式 |
| `char p_sep_by_space` | 整数 | 币种符号与数值间的空格 |
| `char n_sep_by_space` | 整数 | 负币种符号与数值间的空格 |
| `char p_sign_posn` | 整数 | 正数符号位置 |
| `char n_sign_posn` | 整数 | 负数符号位置 |

---

## 区域类别宏详解

| 宏 | 描述 |
|----|------|
| `LC_ALL` | 设置所有本地化相关的属性 |
| `LC_COLLATE` | 控制字符串比较（`strcoll`, `strxfrm`） |
| `LC_CTYPE` | 控制字符分类和转换（`isalnum`, `toupper`, `mbrtowc`） |
| `LC_MONETARY` | 控制货币格式（`localeconv()`） |
| `LC_NUMERIC` | 控制非货币数值格式（小数点、分组） |
| `LC_TIME` | 控制时间日期格式（`strftime`） |

---

## 常见区域设置示例

| 区域代码 | 语言/地区 |
|----------|-----------|
| `"C"` | 默认标准 C 环境 |
| `""` | 系统默认区域 |
| `"en_US.UTF-8"` | 美式英语 |
| `"de_DE.UTF-8"` | 德语 |
| `"fr_FR.UTF-8"` | 法语 |
| `"zh_CN.UTF-8"` | 简体中文 |
| `"ja_JP.UTF-8"` | 日语 |

---

## 标准文档参考

- **C23 标准 (ISO/IEC 9899:2024)**：
  - §7.11 Localization `<locale.h>`
  - §7.31.6 Localization `<locale.h>`

- **C17 标准 (ISO/IEC 9899:2018)**：
  - 同上

- **C11 标准 (ISO/IEC 9899:2011)**：
  - §7.11 Localization `<locale.h>`（第 223–230 页）
  - §7.31.6 Localization `<locale.h>`（第 455 页）

- **C99 标准 (ISO/IEC 9899:1999)**：
  - §7.11 Localization `<locale.h>`（第 204–211 页）
  - §7.26.5 Localization `<locale.h>`（第 401 页）

- **C89/C90 标准 (ISO/IEC 9899:1990)**：
  - §4.4 LOCALIZATION `<locale.h>`

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 本地化库 | [Localization Library (C++)](https://zh.cppreference.com/w/cpp/locale) |

---

> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=c/locale&oldid=180068>  
> 最后修改时间：2025年2月2日 20:35 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

✅ **小结提示**：
- 使用 `setlocale(LC_ALL, "")` 可让程序自动适应系统本地化设置。
- 不同系统和编译器支持的区域名称可能不同。
- 在处理国际应用程序时，本地化能显著提高用户体验。
- 始终在使用本地化前确认当前环境是否支持所请求的语言/地区设置。