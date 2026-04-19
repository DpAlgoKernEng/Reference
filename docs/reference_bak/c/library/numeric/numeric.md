# 数值处理库（Numerics）

来源：[cppreference.com](https://en.cppreference.com)

---

## 简介

C 数值处理库为程序提供了广泛的数学运算支持，涵盖基本数学函数、浮点环境控制、伪随机数生成、复数计算等，是科学计算、工程模拟和加密算法等领域的基础组件。

---

## 数值处理库组成概览

| 子模块 | 功能描述 |
|--------|----------|
| [**常用数学函数**](#常用数学函数) | 提供 `fabs`, `sqrt`, `sin` 等基础数学运算 |
| [**浮点环境控制**](#浮点环境控制)（C99） | 控制浮点异常（溢出、除零等） |
| [**伪随机数生成**](#伪随机数生成) | 通过 `rand()` 和 `srand()` 实现简单随机数 |
| [**复数运算**](#复数运算)（C99） | 支持复数类型的加减乘除等操作 |
| [**泛型数学宏**](#泛型数学宏)（C99） | 根据参数类型自动调用合适函数 |
| [**位操作支持**](#位操作支持)（C23） | 提供位级操作函数（如字节序、位计数） |
| [**安全整数运算**](#安全整数运算)（C23） | 检查整数加减乘过程中的溢出 |

---

## 常用数学函数

### 头文件：`<math.h>`

提供包括三角函数、指数函数、取整函数、绝对值函数等在内的数学运算。

#### 示例代码

```c
#include <stdio.h>
#include <math.h>

int main() {
    double x = 2.5;
    printf("sqrt(%.2f) = %.2f\n", x, sqrt(x));
    printf("sin(%.2f) = %.2f\n", x, sin(x));
    printf("fabs(-%.2f) = %.2f\n", x, fabs(-x));
    return 0;
}
```

#### 常见函数速查表

| 类型 | 函数（部分） | 说明 |
|------|---------------|------|
| 三角函数 | `sin`, `cos`, `tan`, `asin`, `acos`, `atan` | |
| 双曲函数 | `sinh`, `cosh`, `tanh` | |
| 指数对数 | `exp`, `log`, `log10`, `pow`, `sqrt` | |
| 浮点操作 | `ceil`, `floor`, `round`, `trunc` | |
| 绝对值 | `fabs`, `abs`（整数） | |
| 特殊函数 | `erf`, `gamma`, `lgamma`（C99） | |

> 📌 注：需要链接 `-lm`（Linux）或使用编译器自动链接选项。

---

## 浮点环境控制

### 头文件：`<fenv.h>`（C99）

用于控制浮点运算中的异常和舍入选项。

#### 示例代码

```c
#include <stdio.h>
#include <fenv.h>
#include <math.h>

int main() {
    // 清除所有浮点异常状态
    feclearexcept(FE_ALL_EXCEPT);

    // 触发一个浮点异常
    double result = 1.0 / 0.0;

    // 测试是否触发了除零异常
    if (fetestexcept(FE_DIVBYZERO)) {
        printf("检测到除零异常\n");
    }

    return 0;
}
```

#### 常见宏定义

| 宏 | 功能 |
|----|------|
| `FE_DIVBYZERO` | 除零异常 |
| `FE_INEXACT` | 结果不精确 |
| `FE_INVALID` | 无效操作 |
| `FE_OVERFLOW` | 溢出 |
| `FE_UNDERFLOW` | 下溢 |
| `FE_ALL_EXCEPT` | 所有异常 |

---

## 伪随机数生成

### 头文件：`<stdlib.h>`

C 语言通过种子函数 `srand()` 和随机数生成器 `rand()` 实现基础伪随机数支持。

#### 示例代码

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // 使用当前时间作为随机数种子
    srand((unsigned int)time(NULL));

    for (int i = 0; i < 5; ++i)
        printf("%d ", rand() % 100);  // 生成 0~99 的随机数

    printf("\n");
    return 0;
}
```

> 📌 更高质量的随机数可使用 `<random>`（C++）或第三方库如 PCG、Mersenne Twister。

---

## 复数运算

### 头文件：`<complex.h>`（C99）

支持复数类型和运算，包括加减乘除、三角函数、指数函数等。

#### 示例代码

```c
#include <stdio.h>
#include <complex.h>

int main() {
    double complex z1 = 1.0 + 2.0 * I;
    double complex z2 = 3.0 + 4.0 * I;

    double complex sum = z1 + z2;
    double complex product = z1 * z2;

    printf("z1 = %.2f + %.2fi\n", creal(z1), cimag(z1));
    printf("z2 = %.2f + %.2fi\n", creal(z2), cimag(z2));
    printf("z1 + z2 = %.2f + %.2fi\n", creal(sum), cimag(sum));
    printf("z1 * z2 = %.2f + %.2fi\n", creal(product), cimag(product));

    return 0;
}
```

#### 常见复数函数表

| 函数 | 说明 |
|------|------|
| `creal(z)` | 提取实部 |
| `cimag(z)` | 提取虚部 |
| `cabs(z)` | 复数绝对值 |
| `carg(z)` | 复数幅角 |
| `cexp(z)` | 复数指数 |
| `csqrt(z)` | 复数平方根 |

---

## 泛型数学宏

### 头文件：`<tgmath.h>`（C99）

根据实参类型自动选择对应的数学函数（real/complex + float/double/long double）。

#### 示例代码

```c
#include <stdio.h>
#include <tgmath.h>  // 自动选择 sqrtf / sqrt / sqrtl / csqrt 等

int main() {
    float f = 4.0f;
    double d = 9.0;
    float complex fc = 1.0f + 2.0f * I;

    printf("sqrt(f) = %.2f\n", sqrt(f));   // 调用 sqrtf
    printf("sqrt(d) = %.2f\n", sqrt(d));   // 调用 sqrt
    printf("sqrt(fc) = %.2f + %.2fi\n", creal(sqrt(fc)), cimag(sqrt(fc)));  // 调用 csqrtf

    return 0;
}
```

---

## 位操作支持

### 头文件：`<stdbit.h>`（C23）

新增对整数类型的位级操作支持。

#### 示例代码

```c
#include <stdio.h>
#include <stdbit.h>

int main() {
    unsigned int x = 12;  // 二进制: 1100

    printf("前导零个数: %u\n", stdc_leading_zeros_ui(x));  // C23
    printf("尾随零个数: %u\n", stdc_trailing_zeros_ui(x));  // C23
    printf("1 的个数: %u\n", stdc_count_ones_ui(x));        // C23

    return 0;
}
```

#### 常见函数

| 函数 | 用途 |
|------|------|
| `stdc_leading_zeros_ui` | 前导零计数 |
| `stdc_trailing_zeros_ui` | 尾随零计数 |
| `stdc_count_ones_ui` | 1 的个数统计 |
| `stdc_bit_width_ui` | 所需最小位宽 |

---

## 安全整数运算

### 头文件：`<stdckdint.h>`（C23）

防止整数运算过程中发生溢出。

#### 示例代码

```c
#include <stdio.h>
#include <stdbool.h>
#include <stdckdint.h>

int main() {
    int a = 2000000000;
    int b = 2000000000;
    int result;

    if (ckd_add(&result, a, b)) {
        printf("溢出发生\n");
    } else {
        printf("结果: %d\n", result);
    }

    return 0;
}
```

#### 可用宏

| 宏 | 功能 |
|----|------|
| `ckd_add` | 安全加法 |
| `ckd_sub` | 安全减法 |
| `ckd_mul` | 安全乘法 |

---

## 参见（See also）

| 名称 | 链接 |
|------|------|
| C++ 数值库 | [C++ Numerics Library](https://zh.cppreference.com/w/cpp/numeric) |

---

> 页面来源：<https://en.cppreference.com/mwiki/index.php?title=c/numeric&oldid=180191>  
> 最后修改时间：2025年2月5日 22:26 UTC  
> 离线版本获取时间：2025年2月9日 16:39

--- 

✅ **小结提示**：
- 使用数学函数前请确保正确链接数学库（如 `-lm`）；
- 浮点异常检测有助于调试精度问题；
- C23 提供的新特性（如 `stdbit.h`）提升底层编程能力；
- 泛型数学宏简化跨类型调用负担；
- 安全整数运算避免潜在运行时错误。