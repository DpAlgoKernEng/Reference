# C 语言属性：unsequenced 与 reproducible（C23 起）

## 1. 概述 (Overview)

`[[unsequenced]]` 和 `[[reproducible]]` 是 C23 标准引入的两个函数属性（attribute），用于向编译器提供关于函数访问对象行为的额外信息。通过这些属性，编译器可以推导出函数调用的特定性质，从而进行更激进的优化。

- **`[[unsequenced]]`**：表示函数是无副作用（effectless）、幂等（idempotent）、无状态（stateless）且独立（independent）的。编译器可以将多次调用合并为单次调用，并且可以任意并行化和重排序调用。

- **`[[reproducible]]`**：表示函数是无副作用且幂等的。编译器可以将多次调用合并为单次调用。

这两个属性的核心价值在于为编译器优化提供语义保证，使编译器能够生成更高效的代码。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

在 C23 之前，C 语言缺乏标准化的机制来声明函数的纯度（purity）或副作用特性。编译器在优化时只能依赖保守的假设，这限制了许多优化机会。

### 设计动机

函数属性的设计动机源于以下几个方面：

1. **优化需求**：编译器需要明确知道函数是否具有副作用，以决定是否可以消除冗余调用、重排序或并行化执行。

2. **与其他语言接轨**：类似的概念在其他语言中早已存在，如 GCC 的 `pure` 和 `const` 属性、C++ 的 `[[nodiscard]]` 等。

3. **现代硬件利用**：无副作用的函数可以安全地并行执行，充分利用多核处理器能力。

### 版本变更

| 版本 | 变更内容 |
|------|----------|
| C23 | 首次引入 `[[unsequenced]]` 和 `[[reproducible]]` 属性 |

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

```c
[[unsequenced]]           // 标准形式
[[__unsequenced__]]       // 替代形式（带双下划线）

[[reproducible]]          // 标准形式
[[__reproducible__]]      // 替代形式（带双下划线）
```

### 应用位置

这些属性应用于函数声明器（function declarator）或具有函数类型的类型说明符。属性将成为函数类型本身的属性。

```c
// 应用于函数声明
[[unsequenced]] int compute(int x);

// 应用于函数指针类型
[[reproducible]] typedef int (*func_ptr)(int);

// 应用于函数定义
[[unsequenced]] int square(int x) {
    return x * x;
}
```

### 属性语义对比

| 属性 | 无副作用 | 幂等 | 无状态 | 独立 | 允许的优化 |
|------|----------|------|--------|------|------------|
| `[[unsequenced]]` | 是 | 是 | 是 | 是 | 合并调用、并行化、重排序 |
| `[[reproducible]]` | 是 | 是 | 否 | 否 | 合并调用 |

## 4. 底层原理 (Underlying Principles)

### 核心概念详解

#### 4.1 无副作用 (Effectless)

如果函数调用求值期间执行的所有存储操作都是修改与该调用同步的对象，则称该调用是**无副作用**的。此外，如果该操作是可观察的，则对对象的所有访问必须基于函数的唯一指针参数。

**理解要点**：
- 函数不会修改全局状态
- 函数不会通过非参数途径影响外部对象
- 任何修改都通过唯一的指针参数进行

```c
// 无副作用示例
[[reproducible]] int add(int a, int b) {
    return a + b;  // 只读取参数，不修改任何外部状态
}

// 有副作用示例（不应标记为 reproducible）
int global_counter = 0;
int increment_global(void) {  // 错误：不应使用 [[reproducible]]
    return ++global_counter;  // 修改全局状态
}
```

#### 4.2 幂等 (Idempotent)

如果对表达式 E 进行第二次求值可以紧跟在原始求值之后进行，且不改变结果值（如果有）或执行的可观察状态，则称 E 是**幂等的**。

**理解要点**：
- 多次调用产生相同结果
- 调用不会改变程序的观察状态
- 可以安全地用一次调用替换多次调用

```c
// 幂等函数示例
[[reproducible]] int abs_value(int x) {
    return x < 0 ? -x : x;  // 相同输入永远产生相同输出
}

// 非幂等示例（不应标记为 reproducible）
int get_timestamp(void) {  // 错误：不应使用 [[reproducible]]
    return time(NULL);    // 每次调用返回不同值
}
```

#### 4.3 无状态 (Stateless)

如果函数 F 及其调用的所有函数中，任何静态存储期或线程存储期对象的定义都是 const 限定但非 volatile 限定的，则称函数 F 是**无状态**的。

**理解要点**：
- 函数不依赖或修改静态变量
- 函数不依赖线程局部存储
- 可以安全地跨线程调用

```c
// 无状态函数示例
[[unsequenced]] int multiply(int a, int b) {
    return a * b;  // 不依赖任何静态状态
}

// 有状态函数示例（不应标记为 unsequenced）
static int last_result = 0;  // 静态状态
int compute_and_store(int x) {  // 错误：不应使用 [[unsequenced]]
    last_result = x * x;
    return last_result;
}
```

#### 4.4 独立 (Independent)

如果对于函数 F 通过非基于调用参数的左值观察到的任何对象 X，在同一个程序执行期间对 F 的所有调用中，对 X 的所有访问都观察到相同的值；或者如果访问基于指针参数，则必须存在唯一的指针参数 P，使得对 X 的所有访问都基于 P，则称函数 F 是**独立**的。

**对象被函数调用观察的定义**：
- 对象与调用同步
- 对象不是调用的局部对象
- 对象的生命周期在函数调用之前开始
- 对对象的访问在调用期间被排序

**理解要点**：
- 函数对同一输入产生一致的输出
- 通过指针参数访问的对象是唯一的
- 不存在"隐藏"的依赖关系

```c
// 独立函数示例
[[unsequenced]] int process(const int *data) {
    return *data * 2;  // 仅通过参数访问数据
}

// 非独立函数示例（不应标记为 unsequenced）
int global_value = 10;
int read_global(int x) {  // 错误：不应使用 [[unsequenced]]
    return x + global_value;  // 依赖全局变量
}
```

### 编译器优化原理

```
原始代码                      优化后代码
─────────────────────────────────────────────────────
[[reproducible]]             [[reproducible]]
int f(int x);                int f(int x);

int result = f(5);           int temp = f(5);
int result2 = f(5);    ──>   int result = temp;
int result3 = f(5);          int result2 = temp;
                             int result3 = temp;
```

对于 `[[unsequenced]]`，编译器还可以：
- 重排序调用
- 并行化执行
- 向量化循环中的调用

## 5. 使用场景 (Use Cases)

### 适用场景

#### 5.1 数学计算函数

```c
[[unsequenced]] double square_root(double x) {
    return sqrt(x);
}

[[reproducible]] int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}
```

#### 5.2 纯数据转换函数

```c
[[unsequenced]] uint32_t hash_combine(uint32_t a, uint32_t b) {
    return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

[[reproducible]] void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
}
```

#### 5.3 查找与比较函数

```c
[[unsequenced]] int compare_ints(const void *a, const void *b) {
    return (*(const int *)a - *(const int *)b);
}

[[reproducible]] size_t find_first(const char *str, char c) {
    for (size_t i = 0; str[i]; i++) {
        if (str[i] == c) return i;
    }
    return SIZE_MAX;
}
```

### 最佳实践

1. **保守使用**：只有在完全确定函数满足所有要求时才使用这些属性。错误使用会导致未定义行为。

2. **文档化假设**：在注释中说明函数为何满足属性要求。

3. **测试验证**：编写测试用例验证函数在预期场景下的行为。

4. **代码审查**：对这些属性的添加进行严格的代码审查。

### 常见陷阱

| 陷阱 | 说明 | 正确做法 |
|------|------|----------|
| 访问全局变量 | 通过全局变量读写状态 | 仅通过参数传递数据 |
| 调用非纯函数 | 调用 I/O 函数或随机数生成器 | 确保所有被调用函数也满足属性要求 |
| 隐式依赖 | 通过指针间接访问非参数对象 | 明确所有数据依赖通过参数传递 |
| 假设线程安全 | `reproducible` 不保证线程安全 | 需要并行化时使用 `unsequenced` |

## 6. 代码示例 (Examples)

### 6.1 正确用法示例

#### 示例 1：基本数学函数

```c
#include <math.h>

// 完全满足 unsequenced 要求的函数
[[unsequenced]] double circle_area(double radius) {
    return 3.14159265358979323846 * radius * radius;
}

// 满足 reproducible 但不满足 unsequenced 的函数
[[reproducible]] double safe_sqrt(double x) {
    if (x < 0) return 0.0;  // 处理负数输入
    return sqrt(x);
}
```

#### 示例 2：字符串处理函数

```c
#include <string.h>
#include <stddef.h>

// 无状态、幂等、独立的字符串长度计算
[[unsequenced]] size_t safe_strlen(const char *str) {
    if (str == NULL) return 0;
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

// 字符比较函数（常用于排序）
[[unsequenced]] int str_compare(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(const unsigned char *)a - *(const unsigned char *)b;
}
```

#### 示例 3：位操作函数

```c
#include <stdint.h>

// 位反转函数
[[unsequenced]] uint32_t reverse_bits(uint32_t x) {
    x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
    x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
    x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
    x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
    x = ((x & 0x0000FFFF) << 16) | ((x & 0xFFFF0000) >> 16);
    return x;
}

// 人口计数（计算 1 的个数）
[[unsequenced]] int popcount(uint32_t x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}
```

### 6.2 错误用法示例

#### 示例 1：错误使用 reproducible（有副作用）

```c
// 错误：修改全局状态
static int call_count = 0;

[[reproducible]] int get_call_count(int x) {  // 错误！
    call_count++;  // 副作用：修改全局状态
    return x * 2;
}

// 修正：移除属性或移除副作用
int get_doubled(int x) {  // 移除属性
    call_count++;
    return x * 2;
}

// 或更好的修正：纯函数
[[reproducible]] int double_value(int x) {
    return x * 2;
}
```

#### 示例 2：错误使用 unsequenced（非幂等）

```c
#include <stdlib.h>
#include <time.h>

// 错误：每次调用返回不同值
[[unsequenced]] int get_random(void) {  // 错误！
    return rand();  // 非幂等
}

// 正确：接受种子作为参数
[[unsequenced]] int seeded_random(unsigned int *seed) {
    *seed = *seed * 1103515245 + 12345;
    return (*seed / 65536) % 32768;
}
```

#### 示例 3：错误使用 unsequenced（有状态）

```c
// 错误：使用静态变量
[[unsequenced]] int next_id(void) {  // 错误！
    static int id = 0;
    return id++;  // 修改静态状态
}

// 修正：通过参数传递状态
[[reproducible]] int allocate_id(int *current_id) {
    int id = *current_id;
    (*current_id)++;
    return id;
}
```

#### 示例 4：错误使用 unsequenced（非独立）

```c
// 错误：依赖全局变量
int threshold = 100;

[[unsequenced]] int check_threshold(int x) {  // 错误！
    return x > threshold;  // 依赖全局变量
}

// 修正：将依赖作为参数传递
[[unsequenced]] int check_against(int x, int limit) {
    return x > limit;
}
```

### 6.3 完整示例：优化效果演示

```c
#include <stdio.h>

[[unsequenced]] int compute(int x, int y) {
    return x * x + y * y;
}

int main(void) {
    int a = 5, b = 3;

    // 编译器可以将这三次调用优化为一次
    int r1 = compute(a, b);
    int r2 = compute(a, b);
    int r3 = compute(a, b);

    // 优化后可能等价于：
    // int temp = compute(a, b);
    // int r1 = temp, r2 = temp, r3 = temp;

    printf("Results: %d, %d, %d\n", r1, r2, r3);
    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **属性用途**：`[[unsequenced]]` 和 `[[reproducible]]` 是 C23 引入的函数属性，用于向编译器提供函数行为的语义保证，以支持优化。

2. **属性区别**：
   - `[[reproducible]]`：保证函数无副作用且幂等
   - `[[unsequenced]]`：保证函数无副作用、幂等、无状态且独立

3. **优化效果**：
   - `[[reproducible]]`：允许编译器合并多次相同调用
   - `[[unsequenced]]`：允许编译器合并、重排序和并行化调用

4. **使用前提**：只有完全满足属性要求时才能使用，错误使用会导致未定义行为。

### 技术对比

| 特性 | `[[reproducible]]` | `[[unsequenced]]` | GCC `pure` | GCC `const` |
|------|-------------------|-------------------|------------|-------------|
| 无副作用 | 是 | 是 | 是 | 是 |
| 幂等 | 是 | 是 | 是 | 是 |
| 无状态 | 否 | 是 | 否 | 是 |
| 独立 | 否 | 是 | 否 | 是 |
| 可并行化 | 否 | 是 | 否 | 是 |

### 学习建议

1. **从简单开始**：先使用 `[[reproducible]]`，熟练后再考虑 `[[unsequenced]]`。

2. **理解概念**：深入理解无副作用、幂等、无状态、独立四个概念的定义。

3. **验证假设**：在使用属性前，仔细检查函数及其调用的所有函数是否满足要求。

4. **阅读编译器文档**：不同编译器可能对这些属性有不同的优化策略和警告支持。

5. **性能测试**：在实际项目中测量这些属性带来的性能提升，确保优化效果符合预期。

### 参考资料

- ISO/IEC 9899:2024 (C23 标准)
- cppreference: https://en.cppreference.com/w/c/language/attributes/unsequenced
- GCC 函数属性文档