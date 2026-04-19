# C 运算符优先级 (Operator Precedence)

## 1. 概述 (Overview)

运算符优先级（Operator Precedence）定义了 C 语言中表达式中各运算符的绑定顺序。当表达式中出现多个运算符时，优先级高的运算符会优先与其操作数结合，就像被括号包围一样。这一机制决定了表达式的解析方式，是理解和编写正确表达式的关键。

运算符优先级与结合性（Associativity）共同作用，确保复杂表达式具有明确且可预测的求值结构。需要注意的是，优先级和结合性**独立于**求值顺序（Order of Evaluation），前者决定表达式的语法结构，后者决定运行时的计算顺序。

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C 语言运算符优先级的设计源于 B 语言和 BCPL 语言，由 Dennis Ritchie 在 20 世纪 70 年代初设计 C 语言时确定。优先级规则的设计考虑了以下因素：

1. **数学惯例**：乘除优先于加减，符合数学表达式习惯
2. **实用性**：常用操作（如成员访问、函数调用）具有最高优先级
3. **可读性**：减少括号使用，使代码更简洁

### 标准演变

| 标准 | 文档位置 | 说明 |
|------|----------|------|
| C89/C90 | A.1.2.1 | 首次标准化，确立基础优先级规则 |
| C99 | A.2.1 | 新增复合字面量（Compound Literal）运算符 |
| C11 | A.2.1 | 新增 `_Alignof` 运算符 |
| C17 | A.2.1 | 保持不变 |
| C23 | A.2.1 | `alignof` 替代 `_Alignof`（关键字形式） |

### C 与 C++ 的差异

C 语言与 C++ 在运算符优先级上存在细微差异：

| 方面 | C 语言 | C++ |
|------|--------|-----|
| 条件运算符优先级 | 高于赋值运算符（第 13 级） | 与赋值运算符同级（第 15 级） |
| 前缀 `++`/`--` 操作数限制 | 不能是类型转换表达式 | 无此限制 |
| 赋值运算符左操作数限制 | 必须是一元（第 2 级非转换）表达式 | 无此限制 |

## 3. 语法与参数 (Syntax and Parameters)

### 完整优先级表

下表按优先级从高到低排列，同一单元格内的运算符具有相同优先级：

| 优先级 | 运算符 | 描述 | 结合性 |
|--------|--------|------|--------|
| **1** | `++` `--` | 后缀自增/自减（Postfix Increment/Decrement） | 从左到右 |
| | `()` | 函数调用（Function Call） | |
| | `[]` | 数组下标（Array Subscripting） | |
| | `.` | 结构体/联合成员访问（Member Access） | |
| | `->` | 通过指针的成员访问（Member Access through Pointer） | |
| | `(type){list}` | 复合字面量（Compound Literal，C99 起） | |
| **2** | `++` `--` | 前缀自增/自减（Prefix Increment/Decrement） | 从右到左 |
| | `+` `-` | 一元正/负（Unary Plus/Minus） | |
| | `!` `~` | 逻辑非/按位非（Logical NOT/Bitwise NOT） | |
| | `(type)` | 类型转换（Cast） | |
| | `*` | 解引用（Indirection/Dereference） | |
| | `&` | 取地址（Address-of） | |
| | `sizeof` | 大小计算（Size-of） | |
| | `_Alignof` | 对齐要求（Alignment Requirement，C11 起） | |
| **3** | `*` `/` `%` | 乘法/除法/取余（Multiplication/Division/Remainder） | 从左到右 |
| **4** | `+` `-` | 加法/减法（Addition/Subtraction） | 从左到右 |
| **5** | `<<` `>>` | 左移/右移（Bitwise Left/Right Shift） | 从左到右 |
| **6** | `<` `<=` | 小于/小于等于（Relational < and ≤） | 从左到右 |
| | `>` `>=` | 大于/大于等于（Relational > and ≥） | |
| **7** | `==` `!=` | 等于/不等于（Relational = and ≠） | 从左到右 |
| **8** | `&` | 按位与（Bitwise AND） | 从左到右 |
| **9** | `^` | 按位异或（Bitwise XOR） | 从左到右 |
| **10** | `\|` | 按位或（Bitwise OR） | 从左到右 |
| **11** | `&&` | 逻辑与（Logical AND） | 从左到右 |
| **12** | `\|\|` | 逻辑或（Logical OR） | 从左到右 |
| **13** | `?:` | 三元条件（Ternary Conditional） | 从右到左 |
| **14** | `=` | 简单赋值（Simple Assignment） | 从右到左 |
| | `+=` `-=` | 加/减赋值（Assignment by Sum/Difference） | |
| | `*=` `/=` `%=` | 乘/除/余赋值（Assignment by Product/Quotient/Remainder） | |
| | `<<=` `>>=` | 左移/右移赋值（Assignment by Bitwise Shift） | |
| | `&=` `^=` `\|=` | 按位与/异/或赋值（Assignment by Bitwise Operation） | |
| **15** | `,` | 逗号运算符（Comma Operator） | 从左到右 |

### 结合性规则

结合性决定了相同优先级的运算符如何分组：

- **从左到右（Left-to-right）**：运算符从左侧开始结合
  - 例：`a - b - c` 解析为 `(a - b) - c`
  - 例：`a[1][2]++` 解析为 `((a[1])[2])++`

- **从右到左（Right-to-left）**：运算符从右侧开始结合
  - 例：`a = b = c` 解析为 `a = (b = c)`
  - 例：`*p++` 解析为 `*(p++)`（后缀 `++` 优先级高于前缀 `*`）

## 4. 底层原理 (Underlying Principles)

### 语法规则推导

C 标准本身并不直接规定优先级级别，而是通过语法定义间接确定。优先级规则是从语法规则推导出来的：

```
expression:
    assignment-expression
    expression , assignment-expression

assignment-expression:
    conditional-expression
    unary-expression assignment-operator assignment-expression

conditional-expression:
    logical-OR-expression
    logical-OR-expression ? expression : conditional-expression

// 以此类推...
```

语法定义的嵌套层次决定了运算符的"紧密程度"——出现在更深层次语法规则中的运算符，其优先级更高。

### 解析机制

当编译器解析表达式时：

1. **优先级匹配**：表中上方的运算符比下方的运算符更紧密地绑定其操作数
2. **结合性处理**：相同优先级的运算符按结合性方向分组
3. **特殊情况**：某些运算符有特殊的解析规则

### 特殊解析规则

#### 规则一：前缀自增/自减的限制

前缀 `++` 和 `--` 的操作数不能是类型转换表达式。

```c
// 编译器可能拒绝：++(int)x
// 虽然语义上也不合法，但语法规则直接禁止
```

#### 规则二：sizeof 的解析

`sizeof` 的操作数不能是类型转换表达式，这确保了无歧义解析：

```c
sizeof (int) * p
// 解析为：(sizeof(int)) * p
// 而非：sizeof((int)*p)
```

#### 规则三：条件运算符中间表达式

条件运算符中间的表达式（在 `?` 和 `:` 之间）被视为如同被括号包围：

```c
a ? b, c : d
// 解析为：a ? (b, c) : d
// 逗号运算符优先级低于 ?:
```

#### 规则四：赋值运算符左操作数限制

赋值运算符的左操作数必须是一元表达式（第 2 级非转换表达式）。这一规则在语法层面禁止了某些语义上不合法的表达式。

### 优先级与求值顺序的关系

**重要概念区分**：

| 概念 | 决定内容 | 相关规则 |
|------|----------|----------|
| 优先级 + 结合性 | 表达式的**语法结构**（运算符如何分组） | 编译时确定 |
| 求值顺序 | 操作数的**计算顺序** | 运行时行为 |

例如：
```c
int a = 1;
int b = a + a++;  // 未定义行为
```
- 优先级决定这是 `(a) + (a++)`
- 但 `a` 和 `a++` 的求值顺序未定义

## 5. 使用场景 (Use Cases)

### 常见运算符优先级记忆口诀

为了便于记忆，可以按照以下逻辑分组：

1. **成员访问类**：`.` `->` `[]` `()` `后缀++--`（最紧密绑定）
2. **一元运算类**：前缀运算符、`sizeof`、类型转换
3. **算术运算类**：先乘除、后加减
4. **移位运算类**：`<<` `>>`
5. **关系运算类**：比较运算符
6. **位运算类**：`&` > `^` > `|`
7. **逻辑运算类**：`&&` > `||`
8. **条件运算类**：`?:`
9. **赋值运算类**：各种赋值
10. **逗号运算类**：`,`（最松散绑定）

### 最佳实践

#### 1. 存疑时使用括号

```c
// 不推荐：依赖优先级记忆
int result = a & b | c << d;

// 推荐：明确意图
int result = (a & b) | (c << d);  // 或
int result = a & (b | (c << d));  // 根据实际需求
```

#### 2. 理解指针运算优先级

```c
int *p;
int arr[10];
p = arr;

// 常见模式
*p++;        // 解引用 p，然后 p 自增（解析为 *(p++)）
(*p)++;      // p 指向的值自增
*p++ = 10;   // 将 10 赋值给 *p，然后 p 自增

// 结构体成员访问
struct Point { int x, y; };
struct Point *pt;
pt->x++;     // pt->x 自增（解析为 (pt->x)++）
```

#### 3. 赋值与条件运算符

```c
// C 语言中条件运算符优先级高于赋值
int a = b = c ? d : e;    // 解析为：a = (b = (c ? d : e))
int a = c ? d : e = f;    // 解析为：a = ((c ? d : e) = f) 语义错误！

// 在 C++ 中，后者解析为：a = (c ? d : (e = f))，合法
```

### 常见陷阱

#### 陷阱一：位运算优先级低于比较运算符

```c
// 错误：想判断 x 的第 3 位是否为 1
if (x & 0x04 == 0x04) { ... }  // 解析为：x & (0x04 == 0x04)，即 x & 1

// 正确写法
if ((x & 0x04) == 0x04) { ... }
```

#### 陷阱二：移位运算符优先级

```c
int x = 1 << 2 + 3;  // 解析为：1 << (2 + 3) = 1 << 5 = 32，而非 (1 << 2) + 3 = 7

// 正确写法（如果想要后者）
int x = (1 << 2) + 3;  // = 7
```

#### 陷阱三：函数调用与类型转换

```c
// 函数调用优先级高于类型转换
int result = (int)func() + 1;  // 对 func() 返回值转型后加 1
// 而非
int result = (int)(func() + 1);  // 加 1 后再转型
```

#### 陷阱四：逗号运算符在函数调用中

```c
// 函数调用中的逗号是分隔符，不是逗号运算符
func(a, b, c);  // 三个参数

// 要使用逗号运算符，需要括号
func((a, b), c);  // 两个参数，第一个是逗号表达式的值
```

## 6. 代码示例 (Examples)

### 基础用法示例

```c
#include <stdio.h>

int main(void) {
    int a = 5, b = 3, c = 2;

    // 算术运算符优先级
    printf("a + b * c = %d\n", a + b * c);        // 5 + (3 * 2) = 11
    printf("(a + b) * c = %d\n", (a + b) * c);    // (5 + 3) * 2 = 16

    // 关系与逻辑运算符
    int x = 1, y = 0;
    printf("x && y || !y = %d\n", x && y || !y);  // (1 && 0) || 1 = 1

    // 位运算优先级演示
    int flags = 0x05;  // 二进制 0101
    int mask = 0x04;   // 二进制 0100
    printf("flags & mask == mask: %d\n",
           (flags & mask) == mask);  // 正确：使用括号

    return 0;
}
```

### 指针与数组示例

```c
#include <stdio.h>

int main(void) {
    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;

    // 后缀 ++ 优先级高于解引用 *
    printf("*p++ = %d\n", *p++);  // 打印 10，然后 p 指向 arr[1]
    printf("*p = %d\n", *p);     // 打印 20

    // 解引用与自增的组合
    int x = 100;
    int *px = &x;
    printf("(*px)++ = %d\n", (*px)++);  // 打印 100，x 变为 101
    printf("x = %d\n", x);               // 打印 101

    // 数组下标与指针算术
    printf("arr[2] = %d\n", arr[2]);      // 30
    printf("*(arr + 2) = %d\n", *(arr + 2));  // 30，等价写法
    printf("2[arr] = %d\n", 2[arr]);     // 30，合法但不推荐

    return 0;
}
```

### 条件运算符与赋值示例

```c
#include <stdio.h>

int main(void) {
    int a = 10, b = 20, max;

    // 条件运算符与赋值
    max = (a > b) ? a : b;  // max = 20
    printf("max = %d\n", max);

    // 嵌套条件运算符（从右到左结合）
    int x = 1, y = 2, z = 3, result;
    result = x < y ? x++ : y++ ? z++ : 0;
    // 等价于：x < y ? (x++) : ((y++) ? (z++) : 0)
    printf("result = %d, x=%d, y=%d, z=%d\n", result, x, y, z);

    // 赋值运算符链（从右到左结合）
    int p, q, r;
    p = q = r = 5;  // 等价于：p = (q = (r = 5))
    printf("p=%d, q=%d, r=%d\n", p, q, r);

    return 0;
}
```

### 常见错误及修正

```c
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    int status = 0x03;
    int ready = 0x01;
    int error = 0x02;

    // 错误一：位运算与比较运算符混淆
    // 错误写法
    // if (status & ready == ready) { ... }  // 解析错误！

    // 正确写法
    if ((status & ready) == ready) {
        printf("Ready flag is set\n");
    }

    // 错误二：移位运算优先级
    int shift = 1, add = 2;

    // 错误写法（如果意图是先移位再加）
    // int result = shift << add + 1;  // 解析为 shift << (add + 1)

    // 正确写法
    int result = (shift << add) + 1;  // (1 << 2) + 1 = 5
    printf("result = %d\n", result);

    // 错误三：逗号运算符误用
    int x, y;

    // 函数调用中的逗号是分隔符
    printf("x=%d, y=%d\n",
           (x = 1, y = 2, x + y),  // 逗号表达式，值为 3
           y);                      // y 的值为 2

    // 错误四：赋值与条件运算符混淆
    int a = 5, b = 0;

    // C 语言中的陷阱
    // int val = a > b ? a : b = 100;  // 错误！C 中条件运算符优先级高于赋值

    // 正确写法
    int val = (a > b ? a : b);
    val = 100;  // 或者使用 if-else 结构
    printf("val = %d\n", val);

    return 0;
}
```

### 结构体成员访问示例

```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node *next;
};

int main(void) {
    // 静态结构体
    struct Node node1 = {10, NULL};
    struct Node node2 = {20, &node1};

    // 成员访问运算符优先级最高
    printf("node2.value = %d\n", node2.value);      // 20
    printf("node2.next->value = %d\n", node2.next->value);  // 10

    // 后缀自增与成员访问
    node1.value++;
    printf("After increment: node1.value = %d\n", node1.value);  // 11

    // 动态结构体
    struct Node *p = malloc(sizeof(struct Node));
    p->value = 100;
    p->next = NULL;

    // 等价写法
    printf("p->value = %d\n", p->value);
    printf("(*p).value = %d\n", (*p).value);

    // 常见模式：遍历链表
    struct Node *current = &node2;
    while (current != NULL) {
        printf("Value: %d\n", current->value);
        current = current->next;
    }

    free(p);
    return 0;
}
```

## 7. 总结 (Summary)

### 核心要点

1. **优先级决定绑定顺序**：高优先级的运算符先与其操作数结合，形成更紧密的语法结构

2. **结合性解决同级冲突**：相同优先级的运算符按照结合性方向（从左到右或从右到左）进行分组

3. **优先级 ≠ 求值顺序**：优先级决定语法结构，但操作数的求值顺序是另一回事，许多情况下是未定义的

4. **从语法推导**：C 标准通过语法定义间接确定优先级，而非直接规定优先级级别

### 记忆要点

| 优先级组 | 运算符 | 结合性 | 记忆提示 |
|----------|--------|--------|----------|
| 最高 | `()` `[]` `.` `->` 后缀`++` `--` | 左→右 | 成员访问最紧密 |
| 高 | 前缀运算符、`sizeof`、类型转换 | 右→左 | 一元运算统一处理 |
| 中高 | `*` `/` `%` | 左→右 | 数学优先级 |
| 中 | `+` `-` | 左→右 | 加减次之 |
| 中低 | `<<` `>>` | 左→右 | 移位 |
| 低 | 关系、相等 | 左→右 | 比较 |
| 较低 | 位运算（`&` > `^` > `\|`） | 左→右 | 按位操作 |
| 更低 | 逻辑运算（`&&` > `\|\|`） | 左→右 | 逻辑连接 |
| 特殊 | `?:` | 右→左 | 条件选择 |
| 次低 | 赋值运算符 | 右→左 | 赋值链 |
| 最低 | `,` | 左→右 | 逗号分隔 |

### 学习建议

1. **掌握核心规则**：成员访问 > 一元 > 算术 > 比较 > 位运算 > 逻辑 > 条件 > 赋值 > 逗号

2. **不确定时加括号**：括号是消除歧义的最安全方式，不会影响性能

3. **理解常见陷阱**：位运算与比较运算符的优先级关系是最常见的错误来源

4. **注意 C/C++ 差异**：条件运算符和赋值运算符的限制在两语言中不同

5. **区分概念**：明确区分"优先级/结合性"（语法结构）与"求值顺序"（运行时行为）

### 相关概念

- **序列点（Sequence Point）**：定义求值顺序的关键点
- **求值顺序（Order of Evaluation）**：运行时操作数的计算顺序
- **未定义行为（Undefined Behavior）**：违反优先级规则的复杂情况可能导致 UB

---

**参考资料**：
- ISO/IEC 9899:2018 (C17) A.2.1 Expressions
- ISO/IEC 9899:2011 (C11) A.2.1 Expressions
- ISO/IEC 9899:1999 (C99) A.2.1 Expressions
- ISO/IEC 9899:1990 (C89/C90) A.1.2.1 Expressions