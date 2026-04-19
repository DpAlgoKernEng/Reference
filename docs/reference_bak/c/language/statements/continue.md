# C 语言 `continue` 语句详解

  [1. continue 语句基本概念](#1-continue-语句基本概念)  
  
  [2. continue 语句语法详解](#2-continue-语句语法详解)  
  
  [3. continue 语句执行机制](#3-continue-语句执行机制)  
  
  [4. continue 语句与循环结构](#4-continue-语句与循环结构)  
  
  [5. continue 语句与 break 语句的区别](#5-continue-语句与-break-语句的区别)  
  
  [6. continue 语句在嵌套循环中的应用](#6-continue-语句在嵌套循环中的应用)  
  
  [7. C23 属性说明符支持](#7-c23-属性说明符支持)  
  
  [8. continue 语句的性能考虑](#8-continue-语句的性能考虑)  
  
  [9. 常见陷阱与最佳实践](#9-常见陷阱与最佳实践)  
  
  [10. 实际应用示例](#10-实际应用示例)  
  
  [11. 标准参考](#11-标准参考)  
  
  [12. 总结](#12-总结)  

## 1. continue 语句基本概念

### 1.1 continue 语句的本质

在 C 语言中，`continue` 语句是一种控制流语句，用于跳过当前循环迭代的剩余部分，并立即进入下一次循环迭代。它只能出现在 `for`、`while` 或 `do-while` 循环体内部。

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        printf("i = %d\n", i);
    }
    return 0;
}
```

### 1.2 continue 语句的核心功能

`continue` 语句的主要功能包括：

- 跳过当前迭代：当满足某个特定条件时，跳过当前循环体中剩余的代码。
- 优化循环逻辑：避免使用复杂的条件表达式来控制循环体的执行。
- 提高代码可读性：通过提前跳过不必要的处理，使代码逻辑更清晰。

### 1.3 continue 语句与其他控制结构的关系

`continue` 语句是结构化程序设计中控制流的一部分，通常与以下语句配合使用：

- `for`、`while`、`do-while` 循环
- `if` 条件语句（用于判断是否跳过当前迭代）

## 2. continue 语句语法详解

### 2.1 基本语法形式

C 语言中 `continue` 语句的基本语法如下：

```c
continue;
```

### 2.2 语法元素详解

**continue 语句的使用位置**：

`continue` 语句只能出现在以下结构中：

- `for`、`while`、`do-while` 循环体内部

**C23 属性说明符支持**：

从 C23 标准开始，`continue` 语句支持属性说明符，允许为语句附加元信息：

```c
[[likely]] continue;  // 概念性示例，表示该 continue 语句很可能被执行
```

### 2.3 continue 语句的限制

- `continue` 语句不能用于跳出循环，只能跳过当前迭代。
- `continue` 语句不能用于跳出函数或文件作用域。

## 3. continue 语句执行机制

### 3.1 执行流程

`continue` 语句的执行流程如下：

1. 当程序执行到 `continue` 语句时，立即跳过当前循环迭代的剩余部分。
2. 控制权转移到循环的条件判断部分，进入下一次迭代。

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        printf("i = %d\n", i);
    }
    return 0;
}
```

### 3.2 continue 语句与 break 语句的区别

- `continue`：跳过当前迭代，继续下一次迭代。
- `break`：完全终止循环。

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        if (i == 7) {
            break;  // 当 i 等于 7 时，跳出循环
        }
        printf("i = %d\n", i);
    }
    printf("Loop exited.\n");
    return 0;
}
```

## 4. continue 语句与循环结构

### 4.1 在 for 循环中的应用

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        printf("i = %d\n", i);
    }
    return 0;
}
```

### 4.2 在 while 循环中的应用

```c
#include <stdio.h>

int main() {
    int i = 0;
    while (i < 10) {
        if (i % 2 == 0) {
            i++;
            continue;  // 跳过偶数
        }
        printf("i = %d\n", i);
        i++;
    }
    return 0;
}
```

### 4.3 在 do-while 循环中的应用

```c
#include <stdio.h>

int main() {
    int i = 0;
    do {
        if (i % 2 == 0) {
            i++;
            continue;  // 跳过偶数
        }
        printf("i = %d\n", i);
        i++;
    } while (i < 10);
    return 0;
}
```

## 5. continue 语句与 break 语句的区别

### 5.1 功能对比

| 语句      | 功能描述                     | 作用范围       |
|-----------|------------------------------|----------------|
| `continue` | 跳过当前迭代，继续下一次迭代 | 当前循环       |
| `break`    | 完全终止循环                 | 当前循环或 switch |

### 5.2 示例对比

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        if (i == 7) {
            break;  // 当 i 等于 7 时，跳出循环
        }
        printf("i = %d\n", i);
    }
    printf("Loop exited.\n");
    return 0;
}
```

## 6. continue 语句在嵌套循环中的应用

### 6.1 嵌套循环中的 continue

`continue` 语句只能跳过当前循环的迭代，不能跳过外层循环：

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j == 1) {
                continue;  // 只跳过内层循环的当前迭代
            }
            printf("i = %d, j = %d\n", i, j);
        }
    }
    return 0;
}
```

### 6.2 嵌套循环中的 break 与 continue 对比

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j == 1) {
                continue;  // 跳过内层循环的当前迭代
            }
            if (i == 1 && j == 2) {
                break;  // 跳出内层循环
            }
            printf("i = %d, j = %d\n", i, j);
        }
    }
    return 0;
}
```

## 7. C23 属性说明符支持

### 7.1 属性说明符简介

C23 标准为 `continue` 语句引入了属性说明符支持，允许为语句附加元信息：

```c
[[likely]] continue;  // 概念性示例，表示该 continue 语句很可能被执行
```

### 7.2 属性说明符的应用场景

- 性能优化：编译器可以根据属性进行更好的优化。
- 代码文档：属性可以作为代码文档的一部分，提高代码可读性。

## 8. continue 语句的性能考虑

### 8.1 continue 语句的性能影响

`continue` 语句本身对性能的影响非常小，因为它只是简单地改变了程序的控制流。然而，在某些情况下，过度使用 `continue` 可能会导致代码难以理解和维护。

### 8.2 性能测试示例

```c
#include <stdio.h>
#include <time.h>

int main() {
    const int iterations = 100000000;
    clock_t start, end;

    start = clock();
    for (int i = 0; i < iterations; i++) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
    }
    end = clock();
    printf("Continue time: %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    return 0;
}
```

## 9. 常见陷阱与最佳实践

### 9.1 常见陷阱

**在 while 循环中忘记更新循环变量**：

```c
#include <stdio.h>

int main() {
    int i = 0;
    while (i < 10) {
        if (i % 2 == 0) {
            continue;  // 无限循环，因为 i 没有更新
        }
        printf("i = %d\n", i);
        i++;
    }
    return 0;
}
```

**在嵌套循环中误用 continue**：

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j == 1) {
                continue;  // 只跳过内层循环的当前迭代
            }
            printf("i = %d, j = %d\n", i, j);
        }
    }
    return 0;
}
```

### 9.2 最佳实践

**使用有意义的条件判断**：

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        printf("i = %d\n", i);
    }
    return 0;
}
```

**避免过度使用 continue**：

```c
#include <stdio.h>

int main() {
    int i = 0;
    while (i < 10) {
        if (i % 2 == 0) {
            i++;
            continue;  // 跳过偶数
        }
        printf("i = %d\n", i);
        i++;
    }
    return 0;
}
```

## 10. 实际应用示例

### 10.1 跳过无效输入

```c
#include <stdio.h>

int main() {
    int input;
    printf("Enter numbers (0 to exit): ");
    while (1) {
        scanf("%d", &input);
        if (input == 0) {
            break;  // 退出循环
        }
        if (input < 0) {
            continue;  // 跳过负数
        }
        printf("Valid input: %d\n", input);
    }
    return 0;
}
```

### 10.2 跳过数组中的特定元素

```c
#include <stdio.h>

int main() {
    int array[] = {1, 2, 3, 4, 5};
    int size = sizeof(array) / sizeof(array[0]);

    for (int i = 0; i < size; i++) {
        if (array[i] % 2 == 0) {
            continue;  // 跳过偶数
        }
        printf("Odd number: %d\n", array[i]);
    }

    return 0;
}
```

## 11. 标准参考

### 11.1 C 标准版本演进

`continue` 语句相关标准条款的演进：

- C23 标准（ISO/IEC 9899:2024）：6.8.6.2 continue 语句，引入属性说明符支持
- C17 标准（ISO/IEC 9899:2018）：6.8.6.2 continue 语句（第111页）
- C11 标准（ISO/IEC 9899:2011）：6.8.6.2 continue 语句（第153页）
- C99 标准（ISO/IEC 9899:1999）：6.8.6.2 continue 语句（第138页）
- C89/C90 标准（ISO/IEC 9899:1990）：3.6.6.2 continue 语句

### 11.2 相关标准条款详解

**6.8.6.2 continue 语句（C17 标准）**:

该条款详细定义了 `continue` 语句的语法和语义：

```c
continue;
```

**关键语义规则**：

- `continue` 语句只能出现在循环体内部。
- 执行 `continue` 语句时，控制权立即转移到循环的条件判断部分，进入下一次迭代。

## 12. 总结

### 12.1 核心要点回顾

C 语言的 `continue` 语句是程序控制流的重要组成部分，包含以下核心要点：

- 基本语法：`continue;`
- 使用场景：循环结构中跳过当前迭代
- 执行机制：跳过当前迭代的剩余部分，进入下一次迭代
- 限制：不能跳出循环，只能跳过当前迭代
- 现代特性：C23 引入属性说明符支持

### 12.2 设计原则与最佳实践

**结构化编程原则**：

- 单一入口单一出口：每个 `continue` 语句应该有明确的跳过条件。
- 避免过度使用：`continue` 语句应谨慎使用，避免破坏代码的可读性。
- 清晰的控制流：使用适当的缩进和注释。

**代码可读性**：

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // 跳过偶数
        }
        printf("i = %d\n", i);
    }
    return 0;
}
```

通过深入理解和熟练掌握 C 语言的 `continue` 语句，开发者可以编写出更加高效、安全和可维护的代码。`continue` 语句不仅是程序执行的基本控制结构，更是实现复杂逻辑和算法的基础构建块。随着 C 语言标准的不断演进，新的特性和最佳实践将继续推动现代 C 编程的发展。
