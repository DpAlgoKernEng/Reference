# do-while 循环语句

## 1. 概述 (Overview)

do-while 循环是 C++ 中的一种迭代语句（iteration statement），用于条件性地重复执行语句。与其他循环结构（如 while 和 for 循环）不同，do-while 循环保证循环体至少执行一次，因为条件检查发生在循环体执行之后。

**核心特性：**
- 循环体至少执行一次
- 条件在每次迭代结束后检查
- 属于后测试循环（post-test loop）结构
- 适用于需要"先执行后判断"的场景

**技术定位：**
- 迭代控制结构
- 入口控制与出口控制的结合体
- 适用于至少需要一次迭代的场景

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

do-while 循环起源于早期的编程语言设计，其概念可以追溯到 ALGOL 和早期的 C 语言设计。这种循环结构的设计动机是为了解决特定场景下的迭代需求。

### 设计动机

1. **简化代码逻辑**：避免在循环外重复代码
2. **语义清晰**：明确表达"至少执行一次"的意图
3. **用户交互场景**：处理需要先获取输入再判断的情况

### 版本变更

| 版本 | 变更内容 |
|------|----------|
| C++98 | 基础 do-while 语法支持 |
| C++11 | 增加属性（attribute）支持，允许在 do 关键字前添加属性说明符 |
| C++26 | 明确了关于非平凡无限循环的前向进度保证（forward progress guarantee）规则 |

### 前向进度保证（C++26 起）

作为 C++ 前向进度保证的一部分，如果一个没有可观察行为的循环不是平凡无限循环（trivial infinite loop），且不终止，则行为是未定义的。编译器被允许移除这样的循环。

## 3. 语法与参数 (Syntax and Parameters)

### 语法格式

```cpp
attr(可选) do statement while ( expression ) ;
```

### 参数说明

| 参数 | 说明 | 起始版本 |
|------|------|----------|
| `attr` | 任意数量的属性（attribute） | C++11 起 |
| `expression` | 表达式，将被上下文转换为 bool 类型 | 所有版本 |
| `statement` | 语句，通常是复合语句（用花括号括起的语句块） | 所有版本 |

### 语法细节

**属性（attr）：**
- 可选的属性说明符
- 用于给循环语句添加额外信息或优化提示
- C++11 起支持

**表达式（expression）：**
- 必须能被上下文转换为 `bool` 类型
- 在每次循环体执行完毕后求值
- 结果为 `true` 时继续执行循环体，`false` 时退出循环

**语句（statement）：**
- 作为循环体的语句
- 可以是单条语句或复合语句（语句块）
- 推荐使用复合语句以提高可读性

## 4. 底层原理 (Underlying Principles)

### 执行流程

```
控制流到达 do 语句
       ↓
执行 statement（循环体）  ←──┐
       ↓                    │
求值 expression             │
       ↓                    │
转换为 bool                 │
       ↓                    │
结果为 true?  ──是──────────┘
       ↓ 否
退出循环，继续执行后续代码
```

### 实现机制

1. **无条件首次执行**：当控制流到达 do 语句时，statement 将无条件执行
2. **条件检查时机**：每次 statement 执行完毕后，expression 被求值并上下文转换为 `bool`
3. **迭代控制**：如果结果为 `true`，statement 再次执行；否则退出循环

### 控制流转移

**终止循环：**
- 使用 `break` 语句立即终止循环
- 条件表达式求值为 `false`

**终止当前迭代：**
- 使用 `continue` 语句跳过当前迭代的剩余部分
- 直接进入下一次条件检查

### 性能特征

| 特性 | 说明 |
|------|------|
| 时间复杂度 | 取决于循环次数和循环体复杂度 |
| 空间开销 | 仅循环控制变量和条件表达式 |
| 编译优化 | 编译器可进行循环展开、不变量外提等优化 |

## 5. 使用场景 (Use Cases)

### 适用场景

1. **用户输入验证**
   - 需要至少读取一次输入
   - 根据输入决定是否继续

2. **菜单驱动程序**
   - 显示菜单并等待用户选择
   - 根据选择决定是否退出

3. **遍历所有排列组合**
   - 使用 `std::next_permutation` 等函数
   - 确保第一个排列被处理

4. **重试机制**
   - 操作失败后重试
   - 设置最大重试次数或条件

### 最佳实践

1. **明确语义**：当循环体必须至少执行一次时使用 do-while
2. **使用复合语句**：即使循环体只有一条语句，也推荐使用花括号
3. **条件清晰**：确保条件表达式简洁明了
4. **避免无限循环**：确保循环最终能够终止

### 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 条件永远为真 | 导致无限循环 | 确保条件最终变为 false |
| 分号遗漏 | 在 while 后忘记分号 | 记住语法：`while (cond);` |
| 变量作用域 | 在条件中使用循环体内声明的变量 | 在循环外声明变量 |

### do-while 与 while 循环对比

| 特性 | do-while | while |
|------|----------|-------|
| 执行次数 | 至少一次 | 可能为零次 |
| 条件检查时机 | 循环体执行后 | 循环体执行前 |
| 适用场景 | 必须先执行后判断 | 先判断后执行 |

## 6. 代码示例 (Examples)

### 基础用法

```cpp
#include <iostream>

int main() {
    int j = 2;
    do {
        j += 2;
        std::cout << j << ' ';
    } while (j < 9);
    std::cout << '\n';
    return 0;
}
// 输出: 4 6 8 10
```

**说明：** 即使初始条件 `j < 9` 在某次迭代后变为假，循环体已经执行，输出结果包含了 10。

### 遍历排列组合

```cpp
#include <algorithm>
#include <iostream>
#include <string>

int main() {
    std::string s = "aba";
    std::sort(s.begin(), s.end());

    // 使用 do-while 确保第一个排列被输出
    do {
        std::cout << s << '\n';
    } while (std::next_permutation(s.begin(), s.end()));

    return 0;
}
// 输出:
// aab
// aba
// baa
```

**说明：** 这是 do-while 循环的典型应用场景——遍历所有排列组合时，第一个排列必须先处理。

### 用户输入验证

```cpp
#include <iostream>

int main() {
    int choice;
    do {
        std::cout << "请输入选项 (1-5, 0退出): ";
        std::cin >> choice;

        if (choice >= 1 && choice <= 5) {
            std::cout << "选择了选项 " << choice << std::endl;
        }
    } while (choice != 0);

    std::cout << "程序退出" << std::endl;
    return 0;
}
```

### 带属性的循环（C++11 起）

```cpp
#include <iostream>

[[nodiscard]] bool should_continue();

int main() {
    // 使用属性修饰的 do-while 循环
    do {
        // 循环体
    } while (should_continue());

    return 0;
}
```

### 使用 break 和 continue

```cpp
#include <iostream>

int main() {
    int i = 0;
    do {
        i++;

        // 跳过偶数
        if (i % 2 == 0) {
            continue;
        }

        std::cout << i << ' ';

        // 满足条件时终止循环
        if (i > 10) {
            break;
        }
    } while (true);  // 无限循环，依靠 break 退出

    return 0;
}
```

### 常见错误：缺少分号

```cpp
// 错误示例：while 后缺少分号
int x = 0;
do {
    x++;
} while (x < 5)  // 编译错误：缺少分号

// 正确写法
int x = 0;
do {
    x++;
} while (x < 5);  // 正确：while 后有分号
```

### 常见错误：变量作用域问题

```cpp
// 错误示例：在条件中使用循环体内声明的变量
do {
    int value = get_value();
} while (value > 0);  // 错误：value 不在此作用域内

// 正确写法：在循环外声明变量
int value = 0;
do {
    value = get_value();
} while (value > 0);  // 正确
```

## 7. 总结 (Summary)

### 核心要点

1. **执行保证**：do-while 循环保证循环体至少执行一次
2. **后测试特性**：条件检查发生在循环体执行之后
3. **语法关键**：`while (条件);` 后的分号不可省略
4. **控制语句**：支持 `break` 和 `continue` 控制循环流程

### 技术对比

| 循环类型 | 最少执行次数 | 条件检查时机 | 典型用途 |
|----------|-------------|-------------|----------|
| `for` | 0 次 | 执行前 | 已知迭代次数 |
| `while` | 0 次 | 执行前 | 条件驱动迭代 |
| `do-while` | 1 次 | 执行后 | 至少执行一次的场景 |

### 学习建议

1. **选择原则**：只有当循环体必须至少执行一次时，才选择 do-while
2. **代码风格**：始终使用复合语句（花括号）作为循环体
3. **调试技巧**：在条件表达式处设置断点，观察循环退出条件
4. **避免陷阱**：注意分号和变量作用域问题

### 扩展阅读

- while 循环语句
- for 循环语句
- 范围 for 循环（range-based for loop）
- break 和 continue 语句
- C++ 前向进度保证（forward progress guarantee）