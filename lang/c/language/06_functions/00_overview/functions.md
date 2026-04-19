# C 语言函数 (Functions)

## 1. 概述

函数 (Function) 是 C 语言的核心构造，它将一个复合语句（函数体）与一个标识符（函数名）关联起来。每个 C 程序都从 `main` 函数开始执行，`main` 函数要么终止，要么调用其他用户定义函数或库函数。

函数是 C 语言模块化编程的基础单元，通过函数可以实现：
- **代码复用**：将常用功能封装为函数，避免重复代码
- **抽象封装**：隐藏实现细节，提供清晰的接口
- **模块化设计**：将复杂程序分解为可管理的小模块

## 2. 来源与演变

### C89/C90 标准

C 语言的函数概念从 C 语言诞生之初就存在。C89/C90 标准首次将函数相关规范正式化：

- 3.5.4.3 Function declarators (including prototypes)
- 3.7.1 Function definitions

### C99 标准

C99 标准对函数进行了重要更新：

| 变化 | 说明 |
|------|------|
| 内联函数 | 引入 `inline` 关键字，允许定义内联函数 |
| 变长数组参数 | 支持变长数组作为函数参数 |
| 灵活数组成员 | 结构体末尾可包含灵活数组成员 |

相关章节：
- 6.7.5.3 Function declarators (including prototypes)
- 6.9.1 Function definitions

### C11 标准

C11 标准延续 C99 的函数规范，并增加了线程支持相关函数：

| 变化 | 说明 |
|------|------|
| _Noreturn 函数说明符 | 标记不返回的函数 |
| 泛型选择 | `_Generic` 支持泛型编程 |

相关章节：
- 6.7.6.3 Function declarators (including prototypes)
- 6.9.1 Function definitions

### C17 标准

C17 标准主要进行缺陷修复，函数规范无重大变化：

- 6.7.6.3 Function declarators (including prototypes)
- 6.9.1 Function definitions

### C23 标准

C23 标准对函数进行了进一步优化：

| 变化 | 说明 |
|------|------|
| 函数原型增强 | 更严格的类型检查 |
| 新的属性语法 | 支持函数属性说明 |

相关章节：
- 6.7.7.4 Function declarators (including prototypes)
- 6.9.2 Function definitions

## 3. 语法与声明

### 函数定义

```c
返回类型 函数名(参数列表)
{
    // 函数体
    return 返回值;
}
```

**完整示例**：

```c
// 定义一个名为 "sum" 的函数
// 函数体为 "{ return x+y; }"
int sum(int x, int y)
{
    return x + y;
}
```

### 函数声明（原型）

函数声明告诉编译器函数的存在，允许在定义之前使用函数：

```c
返回类型 函数名(参数类型列表);  // 函数原型
```

### 参数与实参

| 术语 | 英文 | 说明 |
|------|------|------|
| 参数 | Parameter | 函数定义中的变量，用于接收值 |
| 实参 | Argument | 函数调用时传递的具体值 |

```c
int n = sum(1, 2);  // 参数 x 和 y 分别用实参 1 和 2 初始化
```

### main 函数

每个 C 程序必须有且仅有一个 `main` 函数作为程序入口：

```c
int main(void)        // 无参数形式
{
    return 0;
}

int main(int argc, char *argv[])  // 带命令行参数形式
{
    return 0;
}
```

## 4. 底层原理

### 函数调用机制

当函数被调用时，发生以下过程：

1. **参数传递**：实参值被复制到函数参数（值传递）
2. **栈帧创建**：为函数创建新的栈帧（stack frame）
3. **控制转移**：程序计数器跳转到函数入口
4. **局部变量分配**：在栈上分配局部变量
5. **函数执行**：执行函数体
6. **返回值传递**：将返回值放回指定位置
7. **栈帧销毁**：释放函数栈帧
8. **控制返回**：返回到调用点继续执行

### 函数调用栈示意

```
+------------------+
|   main() 栈帧    |
|   - 局部变量     |
+------------------+
|   sum() 栈帧     |  <- 当前执行
|   - 参数 x, y    |
|   - 返回地址     |
+------------------+
```

### 内存布局

| 区域 | 内容 |
|------|------|
| 代码段 | 函数的可执行代码 |
| 栈区 | 函数调用栈、局部变量 |
| 堆区 | 动态分配的内存 |

### 作用域规则

C 语言函数具有以下作用域特性：

- **文件作用域**：函数定义必须出现在文件作用域
- **无嵌套函数**：C 标准不允许在函数内定义另一个函数
- **独立访问**：函数无法访问调用者的局部变量

## 5. 使用场景

### 适合使用函数的场景

| 场景 | 说明 |
|------|------|
| 重复代码 | 将重复逻辑封装为函数 |
| 复杂逻辑 | 分解复杂算法为多个小函数 |
| 可测试性 | 独立函数便于单元测试 |
| 接口抽象 | 隐藏实现细节，提供清晰接口 |

### 函数声明的位置

函数声明可以出现在任何作用域，包括函数内部：

```c
int main(void)
{
    int sum(int, int);  // 函数声明（可在任何作用域）
    int x = 1;          // main 的局部变量
    sum(1, 2);          // 函数调用
    return 0;
}
```

### 最佳实践

1. **先声明后使用**：在头文件中声明函数原型
2. **参数命名**：声明中可省略参数名，定义中应有意义的命名
3. **单一职责**：每个函数只做一件事
4. **合理长度**：函数不宜过长，一般不超过 50 行
5. **返回值检查**：调用函数后检查返回值

### 常见陷阱

| 陷阱 | 说明 |
|------|------|
| 尝试定义嵌套函数 | C 标准不支持，会编译错误 |
| 访问调用者局部变量 | 函数无法访问调用者的局部变量 |
| 未声明的函数调用 | 隐式声明可能导致类型不匹配 |
| 忘记 return 语句 | 非 void 函数可能返回不确定值 |

## 6. 代码示例

### 基础用法

```c
#include <stdio.h>

// 函数声明（原型）
int sum(int x, int y);

int main(void)
{
    // 函数调用
    int result = sum(1, 2);
    printf("sum(1, 2) = %d\n", result);  // 输出: sum(1, 2) = 3
    return 0;
}

// 函数定义
int sum(int x, int y)
{
    return x + y;
}
```

### main 函数示例

```c
#include <stdio.h>

int main(void)  // main 函数定义
{
    int sum(int, int);  // 函数声明（可在任何作用域）
    int x = 1;          // main 中的局部变量
    int result = sum(1, 2);  // 函数调用
    printf("Result: %d\n", result);
    return 0;
}

// 函数定义（必须在文件作用域）
int sum(int a, int b)
{
    return a + b;
}
```

### 常见错误及修正

#### 错误 1：尝试定义嵌套函数

```c
int main(void)
{
    int sum(int a, int b)  // 错误：C 不支持嵌套函数
    {
        return a + b;
    }
    return 0;
}
```

**修正**：将函数定义移到文件作用域

```c
// 正确：函数定义在文件作用域
int sum(int a, int b)
{
    return a + b;
}

int main(void)
{
    int result = sum(1, 2);
    return 0;
}
```

#### 错误 2：尝试访问调用者的局部变量

```c
int main(void)
{
    int x = 1;  // main 的局部变量
    // ...
}

int sum(int a, int b)
{
    // return x + a + b;  // 错误：无法访问 main 的 x
    return a + b;          // 正确：只能访问自己的参数
}
```

#### 错误 3：忘记函数声明

```c
int main(void)
{
    int result = sum(1, 2);  // 警告：隐式声明
    return 0;
}

int sum(int a, int b)
{
    return a + b;
}
```

**修正**：在使用前添加函数声明

```c
// 正确：先声明后使用
int sum(int a, int b);  // 函数声明

int main(void)
{
    int result = sum(1, 2);  // 正确
    return 0;
}

int sum(int a, int b)
{
    return a + b;
}
```

### 带命令行参数的 main 函数

```c
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Program name: %s\n", argv[0]);
    printf("Argument count: %d\n", argc);

    for (int i = 1; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    return 0;
}
```

## 7. 总结

C 语言函数是程序模块化的核心机制，具有以下特点：

### 核心要点

| 特性 | 说明 |
|------|------|
| 单一定义 | 非内联函数在程序中只能定义一次 |
| 无嵌套 | 函数定义必须位于文件作用域 |
| 独立作用域 | 函数无法访问调用者的局部变量 |
| 值传递 | 参数通过值传递，不修改实参 |

### 函数 vs 其他语言的对比

| 语言 | 嵌套函数 | 闭包 | 函数重载 |
|------|---------|------|---------|
| C | 不支持（标准） | 不支持 | 不支持 |
| C++ | 不支持 | 支持（lambda） | 支持 |
| Python | 支持 | 支持 | 不支持 |

### 学习建议

1. **理解调用栈**：掌握函数调用的底层机制
2. **善用原型**：在头文件中声明函数原型
3. **模块化思维**：学会将复杂问题分解为函数
4. **注意作用域**：理解变量作用域和生命周期

## 参考资料

- C23 standard (ISO/IEC 9899:2024):
  - 6.7.7.4 Function declarators (including prototypes)
  - 6.9.2 Function definitions
- C17 standard (ISO/IEC 9899:2018):
  - 6.7.6.3 Function declarators (including prototypes)
  - 6.9.1 Function definitions
- C11 standard (ISO/IEC 9899:2011):
  - 6.7.6.3 Function declarators (including prototypes)
  - 6.9.1 Function definitions
- C99 standard (ISO/IEC 9899:1999):
  - 6.7.5.3 Function declarators (including prototypes)
  - 6.9.1 Function definitions
- C89/C90 standard (ISO/IEC 9899:1990):
  - 3.5.4.3 Function declarators (including prototypes)
  - 3.7.1 Function definitions
- cppreference: https://en.cppreference.com/w/c/language/functions