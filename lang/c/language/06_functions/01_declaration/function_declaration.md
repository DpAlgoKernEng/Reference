# 函数声明 (Function Declaration)

## 1. 概述 (Overview)

函数声明 (Function Declaration) 是 C 语言中用于引入函数标识符的语法结构。它声明一个标识符来指定一个函数，并可选地指定函数参数的类型（称为**原型 prototype**）。

与函数定义不同，函数声明可以出现在块作用域和文件作用域中。函数声明的主要作用包括：
- 引入函数设计ator（function designator）
- 作为函数原型，用于后续函数调用时的类型检查
- 强制参数类型转换
- 编译时参数数量检查

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

C 语言函数声明的语法经历了多个版本的演进：

| 版本 | 特性 | 说明 |
|------|------|------|
| K&R C | 旧式函数声明 | 参数列表只包含标识符，参数类型在函数体前声明 |
| C89/C90 | 新式函数声明 | 引入函数原型（prototype），支持参数类型声明 |
| C99 | 移除隐式 int | 函数返回类型不再默认为 int |
| C23 | 统一语法 | `f()` 等价于 `f(void)`，旧式 K&R 定义被移除 |

### 版本变更详情

**C89 引入函数原型**：C89 标准引入了新式函数声明语法，支持在参数列表中直接声明参数类型，这为编译器提供了完整的类型信息，增强了类型安全性。

**C99 移除隐式 int**：在 C89 中，如果省略类型说明符，函数返回类型默认为 int。C99 标准移除了这一隐式规则。

**C23 统一语法**：C23 标准将 `f()` 和 `f(void)` 统一为相同含义，都表示无参数函数，同时移除了旧式 K&R 函数定义语法。

### 缺陷报告

| DR | 应用版本 | 原发布行为 | 修正行为 |
|------|------|------|------|
| DR 423 | C89 | 返回类型可能被限定 | 返回类型隐式去除限定符 |

## 3. 语法与参数 (Syntax and Parameters)

### 语法形式

函数声明语法的声明符有三种形式：

```
noptr-declarator ( parameter-list ) attr-spec-seq(可选)          // (1) 新式声明
noptr-declarator ( identifier-list ) attr-spec-seq(可选)          // (2) 旧式声明 (C23 前)
noptr-declarator ( ) attr-spec-seq(可选)                          // (3) 非原型声明
```

### 参数说明

| 参数 | 说明 |
|------|------|
| `noptr-declarator` | 除未加括号的指针声明符外的任何声明符。其中包含的标识符成为函数设计ator |
| `parameter-list` | 单独的关键字 `void` 或以逗号分隔的参数列表，可省略结束 |
| `identifier-list` | 以逗号分隔的标识符列表，仅用于旧式函数定义（C23 前） |
| `attr-spec-seq` | (C23) 可选的属性列表，应用于函数类型 |

### 三种语法形式详解

**形式 (1) - 新式函数声明（C89 起）**：
- 引入函数设计ator
- 作为函数原型，强制参数类型转换
- 编译时检查参数数量

**形式 (2) - 旧式 K&R 函数定义（C23 前）**：
- 不引入原型
- 函数调用时执行默认参数提升
- 参数数量不匹配时导致未定义行为

**形式 (3) - 非原型函数声明**：
- C23 前：不引入原型
- C23 起：等价于 `parameter-list` 为 `void`

### 返回类型规则

- 必须是非数组对象类型或 `void` 类型
- 如果声明不是定义，返回类型可以是不完整类型
- 返回类型不能被 cvr 限定（const/volatile/restrict）：任何限定的返回类型都会被调整为非限定版本

### 参数列表规则

1. **参数命名**：非函数定义的声明中，参数可以不命名（C23 前）；C23 起定义中也允许不命名

2. **存储类说明符**：参数只能使用 `register` 存储类说明符，且在非定义声明中被忽略

3. **数组类型调整**：数组类型参数调整为对应的指针类型

4. **函数类型调整**：函数类型参数调整为对应的函数指针类型

5. **变长参数**：参数列表可以以 `, ...` 或 `...`（C23 起）结尾

6. **void 参数**：参数不能是 `void` 类型，但可以是指向 `void` 的指针；单独的 `void` 表示无参数

7. **不完整类型**：参数可以是不完整类型，可以使用 VLA 表示法 `*`

## 4. 底层原理 (Underlying Principles)

### 类型调整机制

当函数声明中出现数组类型或函数类型参数时，编译器会自动进行类型调整：

**数组到指针调整**：
```c
int f(int[]);              // 调整为 int f(int*)
int g(const int[10]);      // 调整为 int g(const int*)
int h(int[const volatile]); // 调整为 int h(int * const volatile)
```

**函数到指针调整**：
```c
int f(char g(double));     // 调整为 int f(char (*g)(double))
int h(int(void));          // 调整为 int h(int (*)(void))
```

### 作用域与链接

函数声明引入的标识符的作用域和链接属性：

| 声明位置 | 作用域 | 链接属性 |
|------|------|------|
| 任何函数外部 | 文件作用域 | 外部链接（除非使用 `static` 或之前有静态声明可见） |
| 其他函数内部 | 块作用域 | 内部或外部链接 |

### 返回类型限定符处理

根据 DR 423，返回类型的限定符会被隐式去除：

```c
double const bar(void);             // 声明类型为 double(void)
double (*barp)(void) = bar;         // OK: barp 是指向 double(void) 的指针
double const (*barpc)(void) = barp; // OK: barpc 也是指向 double(void) 的指针
```

### 原型与非原型的区别

在 C23 之前，`f()` 和 `f(void)` 具有不同含义：

```c
int f(void);  // 声明：无参数
int g();      // 声明：参数数量未知

int main(void) {
    f(1);     // 编译时错误
    g(2);     // 未定义行为
}
```

C23 起，两者等价，都表示无参数函数。

## 5. 使用场景 (Use Cases)

### 适用场景

1. **头文件声明**：在头文件中声明库函数接口
2. **前向声明**：在使用函数前声明，解决相互依赖问题
3. **函数指针类型定义**：通过 typedef 定义函数类型
4. **API 设计**：声明公共接口

### 最佳实践

1. **始终使用原型**：使用完整的参数类型声明，而非空参数列表
2. **命名参数**：即使声明中不强制要求，也建议为参数命名以提高可读性
3. **使用 `void` 表示无参数**：明确表示函数不接受参数
4. **避免旧式声明**：避免使用 K&R 风格的函数声明

### 常见陷阱

1. **`f()` 与 `f(void)` 混淆**（C23 前）：
   - `f(void)` 表示无参数
   - `f()` 表示参数未知，调用时不进行参数检查

2. **返回类型限定符无效**：
   - `const int f(void);` 中的 `const` 被忽略

3. **数组参数退化为指针**：
   - `int f(int a[10])` 实际接收 `int*`

4. **旧式声明的未定义行为**：
   - 参数数量不匹配时行为未定义

## 6. 代码示例 (Examples)

### 基础用法

**标准函数声明**：
```c
// 函数声明
int max(int a, int b);

// 函数调用 - 参数会被转换
int n = max(12.01, 3.14);  // OK: double 到 int 的转换
```

**无参数函数声明**：
```c
int f(void);    // 推荐：明确表示无参数
int g();        // C23 前：参数未知；C23 起：等价于 g(void)
```

**返回指针的函数声明**：
```c
void f(char *s);                     // 返回类型为 void
int sum(int a, int b);               // 返回类型为 int
int (*foo(const void *p))[3];         // 返回指向 3 元素数组的指针
```

### 高级用法

**函数类型 typedef**：
```c
typedef int p(int q, int r);  // p 是函数类型 int(int, int)
p f;                          // 声明 int f(int, int)
```

**多声明组合**：
```c
int f(void), *fip(), (*pfi)(), *ap[3];  // 声明两个函数和两个对象
```

**块作用域声明**：
```c
int main(void)
{
    int f(int);  // 外部链接，块作用域
    f(1);        // 定义需要在程序某处可用
}
```

**参数类型调整示例**：
```c
int f(int[]);               // 声明 int f(int*)
int g(const int[10]);        // 声明 int g(const int*)
int h(int[const volatile]);  // 声明 int h(int * const volatile)
int x(int[*]);              // 声明 int x(int*)

int f(char g(double));      // 声明 int f(char (*g)(double))
int h(int(void));           // 声明 int h(int (*)(void))
```

### 常见错误及修正

**错误 1：旧式 K&R 声明（C23 前）**：
```c
// 错误：旧式声明不提供原型
int max(a, b)
    int a, b;
{
    return a > b ? a : b;
}

int n = max(12.01f, 3.14);  // 未定义行为：传入 double 而非 int
```

修正：
```c
// 正确：使用原型声明
int max(int a, int b);

int max(int a, int b) {
    return a > b ? a : b;
}

int n = max(12.01, 3.14);  // OK: 参数会转换为 int
```

**错误 2：inline 限定符用于非函数**：
```c
inline int g(int), n;  // 错误：inline 限定符仅用于函数
```

修正：
```c
inline int g(int);
int n;  // 分开声明
```

**错误 3：数组返回类型**：
```c
typedef int array_t[3];
array_t a, h();  // 错误：数组类型不能作为函数返回类型
```

修正：
```c
typedef int array_t[3];
array_t a;
int (*h(void))[3];  // 返回指向数组的指针
```

**错误 4：void 参数类型**：
```c
int f(void);   // OK
int g(void x); // 错误：参数不能是 void 类型
```

**错误 5：参数存储类说明符**：
```c
int f(static int x);     // 错误：参数不能使用 static
int f(int [static 10]);  // OK：数组索引 static 不是存储类说明符
```

## 7. 总结 (Summary)

### 核心要点

| 要点 | 说明 |
|------|------|
| 函数声明 vs 定义 | 声明可以多次出现，定义只能一次 |
| 原型重要性 | 提供类型检查和参数转换，避免未定义行为 |
| 类型调整 | 数组和函数类型参数自动转换为指针 |
| 作用域规则 | 文件作用域或块作用域，链接属性取决于声明位置和存储类 |
| 版本差异 | C23 统一了 `f()` 和 `f(void)` 的语义 |

### 技术对比

| 特性 | C89/C90 | C99 | C23 |
|------|---------|-----|-----|
| 隐式 int 返回类型 | 支持 | 移除 | 移除 |
| 旧式 K&R 声明 | 支持 | 支持 | 移除 |
| `f()` 语义 | 参数未知 | 参数未知 | 无参数 |
| `f(void)` 语义 | 无参数 | 无参数 | 无参数 |
| 参数命名要求（定义） | 必须 | 必须 | 可选 |

### 学习建议

1. **始终使用函数原型**：避免使用空参数列表的旧式声明
2. **理解类型调整机制**：掌握数组到指针、函数到指针的自动转换
3. **关注 C23 变化**：了解新标准对函数声明语法的简化
4. **参考标准文档**：查阅 ISO/IEC 9899 标准了解完整规范

### 参考标准

- **C17** (ISO/IEC 9899:2018): 6.7.6.3 Function declarators (including prototypes)
- **C11** (ISO/IEC 9899:2011): 6.7.6.3 Function declarators (including prototypes)
- **C99** (ISO/IEC 9899:1999): 6.7.5.3 Function declarators (including prototypes)
- **C89/C90** (ISO/IEC 9899:1990): 3.5.4.3 Function declarators (including prototypes)