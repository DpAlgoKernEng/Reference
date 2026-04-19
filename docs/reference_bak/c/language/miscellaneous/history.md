# C语言历史（History of C）

来源：cppreference.com

## 早期C语言发展

### 1969年：B语言诞生
- 基于BCPL语言创建
- 目的：替代PDP-7汇编语言作为Unix系统编程语言
- 新增特性：
  - `++`、`--`运算符
  - 复合赋值运算符
  - 仍保持BCPL的无类型特性

### 1971年：NB语言（"new B"）
- 将B语言移植到PDP-11时创建
- 重要改进：
  - 引入类型系统（int、char、数组、指针）
  - 数组到指针的转换
  - 编译为机器码

### 1972年：更名为C语言
- 重要特性：
  - `struct`结构体
  - `&&`和`||`逻辑运算符
  - 预处理器
  - 可移植的I/O操作

### 1973年：Unix用C重写
- 新增特性：
  - `unsigned`无符号类型
  - `long`长整型
  - `union`联合体
  - `enum`枚举
  - 类型安全增强

### 1978年：《C程序设计语言》第一版
- Brian Kernighan和Dennis Ritchie著
- 成为C语言的事实标准

---

## 标准化C语言

### 1983年：ANSI成立X3J11委员会
- 开始C语言标准化工作

### 1988年：《C程序设计语言》第二版
- 更新内容以反映标准化进展

### 1989年：**C89**（ANSI C标准发布）
#### 标准化成果：
1. **规范化现有实践**：整理已有的C语言使用习惯
2. **新增特性**：
   - `volatile`易失性限定符
   - `enum`枚举
   - `signed`有符号限定符
   - `void`空类型
   - 本地化支持
3. **从C++借鉴**：
   - `const`常量限定符
   - 函数原型

### 1990年：**C90**（ISO/IEC 9899:1990）
- ANSI C标准被ISO接受为国际标准

### 1994年：技术勘误1（ISO/IEC 9899:1990/Cor.1:1994）
- 44个小修改

### 1995年：**C95**（ISO/IEC 9899:1990/Amd.1:1995）
#### 重大扩展：
1. **宽字符和多字节字符支持大幅增强**：
   - `<wctype.h>`宽字符类型头文件
   - `<wchar.h>`宽字符处理头文件
   - 流I/O等功能增强
2. **新特性**：
   - 三字符组（digraphs）
   - `<iso646.h>`头文件

### 1996年：技术勘误2（ISO/IEC 9899:1990/Cor.2:1996）
- 24个小修改

---

## C99标准（1999年）

### ISO/IEC 9899:1999

#### 新增特性：
1. **核心语言特性**：
   - `_Bool`布尔类型
   - `long long`超长整型
   - `<stdint.h>`标准整型头文件
   - `<inttypes.h>`整型格式化头文件
   - `restrict`限定符
   - 复合字面量
   - 可变长度数组
   - 灵活数组成员
   - 指定初始化器
   - `<fenv.h>`浮点环境头文件
   - 可变参数宏
   - 复数类型
   - `__func__`预定义标识符
   - 十六进制浮点格式（%a）
   - 货币格式化（lconv）
   - `isblank`函数
   - 窄字符串和宽字符串字面量连接
   - 枚举中的尾随逗号
   - 函数类宏中的空参数
   - STDC_*编译指示
   - `va_copy`宏
   - `tmpnam`的空返回
   - `setvbuf`中的空指针
   - `printf`中的`hh`和`ll`长度修饰符
   - `snprintf`函数
   - `_Exit`函数
   - `<tgmath.h>`类型泛型数学头文件
   - POSIX风格的`strftime`格式符

2. **从C++借鉴**：
   - `inline`内联函数
   - 声明与代码混合
   - for循环初始化语句中的声明
   - `//`单行注释
   - 源代码中的通用字符名

3. **移除特性**：
   - 隐式函数声明
   - 隐式int类型

### 后续勘误：
- **2001年**：技术勘误1（11个缺陷修复）
- **2004年**：技术勘误2
- **2004年**：Unicode TR（ISO/IEC TR 19769:2004）
- **2007年**：技术勘误3（废弃`gets`函数）

### 技术报告（TR）：
- **2007年**：边界检查接口TR（ISO/IEC TR 24731-1:2007）
- **2008年**：嵌入式TR（ISO/IEC TR 18037:2008）
- **2009年**：十进制浮点TR（ISO/IEC TR 24732:2009）
- **2009年**：数学特殊函数TR（ISO/IEC TR 24747:2009）
- **2010年**：动态分配函数TR（ISO/IEC TR 24731-2:2010）

---

## C11标准（2011年）

### ISO/IEC 9899:2011

#### 主要特性：
1. **多线程支持**：
   - 线程感知的内存模型
   - `<stdatomic.h>`原子操作头文件
   - `<threads.h>`线程支持头文件
   - 类型泛型函数

2. **语言增强**：
   - `alignas`/`alignof`对齐控制
   - `_Noreturn`不返回函数
   - `static_assert`静态断言
   - 可分析性扩展
   - 复数和虚数类型的扩展
   - 匿名结构体和联合体
   - 独占文件打开模式
   - `quick_exit`快速退出

3. **移除特性**：
   - 移除`gets`函数（安全风险）

4. **从技术报告集成**：
   - 边界检查接口
   - 从Unicode TR：`char16_t`、`char32_t`、`<uchar.h>`

### 2012年：技术勘误1
- 修复缺陷报告411

### 2013年：安全编码规则TS（ISO/IEC TS 17961:2013）

---

## 浮点运算技术规范（2014-2016）

### ISO/IEC TS 18661系列

#### 第1部分：二进制浮点运算（2014）
- 基于IEC 60559:2011标准更新C11（主要是附录F）

#### 第2部分：十进制浮点运算（2015）
- 支持IEC 60559:2011的十进制浮点运算要求
- 取代ISO/IEC TR 24732:2009

#### 第3部分：交换和扩展类型（2015）
- 支持IEC 60559:2011的扩展浮点格式和交换格式

#### 第4部分：补充函数（2015）
- 支持IEC 60559:2011推荐的所有数学运算

#### 第5部分：补充属性（2016）
- 支持IEC 60559:2011推荐的所有补充属性

---

## 现代C语言标准

### 2018年：**C17**（ISO/IEC 9899:2018）
#### 主要内容：
- 修复54个缺陷报告（DR 400-491）

### 2023年：**C23**（ISO/IEC 9899:2024）
#### 当前C语言标准版本：
- 修复多个缺陷报告（DR 440, 432, 467等）

---

## 未来发展方向

### 技术规范草案：
- **并行性TS**（草案N2017 2016-03-10）
- **事务内存TS**（草案N1961 2015-09-23）

### **C29**（最新草案n3435 2025-01-03）
- 下一个主要C语言标准修订版本
- 包含未获得DR状态的问题列表（N2556 2020-08-02）

---

## C语言发展里程碑

| 年份 | 标准 | 重要特性 |
|------|------|----------|
| 1972 | 早期C | 结构体、逻辑运算符 |
| 1978 | K&R C | 第一本权威教材 |
| 1989 | C89 | 第一个ANSI标准 |
| 1999 | C99 | 布尔类型、可变数组 |
| 2011 | C11 | 多线程、原子操作 |
| 2018 | C17 | 缺陷修复 |
| 2023 | C23 | 当前标准 |

---

## 扩展知识详解

### 一、C语言设计哲学

#### 1. 简洁性
```c
// C语言设计简洁，语法紧凑
int main() {
    printf("Hello, World!\n");
    return 0;
}
```

#### 2. 高效性
- 接近汇编语言的控制能力
- 编译器优化潜力大
- 系统级编程首选

#### 3. 可移植性
- "一次编写，到处编译"
- 标准化促进跨平台兼容

### 二、C语言对其他语言的影响

#### 1. C++语言
```cpp
// C++继承C语言语法
class MyClass {
public:
    int value;
    void func();  // 从C语言函数演变
};
```

#### 2. Java、C#等
```java
// Java语法深受C影响
public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello, World!");
    }
}
```

### 三、各版本C标准的关键差异

#### 1. C89 vs C99
```c
// C89不支持的特性（C99新增）
for (int i = 0; i < 10; i++) {  // C99支持for循环内声明
    // C99支持混合声明和代码
    int value = i * i;
}

// C99新增_Bool类型
_Bool flag = 1;  // C89无布尔类型

// C99支持//注释
// C89只支持/* */注释
```

#### 2. C99 vs C11
```c
// C11新增多线程支持
#include <threads.h>

int thread_func(void* arg) {
    // 线程函数
    return 0;
}

// C11新增静态断言
static_assert(sizeof(int) >= 4, "int too small");
```

### 四、现代C语言特性

#### 1. 泛型选择（C11）
```c
#define max(a, b) _Generic((a), \
    int: max_int, \
    double: max_double \
)(a, b)
```

#### 2. 原子操作（C11）
```c
#include <stdatomic.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void increment() {
    atomic_fetch_add(&counter, 1);
}
```

#### 3. 对齐控制（C11）
```c
// 指定对齐
_Alignas(16) char buffer[256];

// 查询对齐要求
size_t alignment = _Alignof(struct MyStruct);
```

### 五、安全增强特性

#### 1. 边界检查函数（来自TR 24731）
```c
#include <string.h>

// 安全的字符串复制
char dest[10];
errno_t result = strcpy_s(dest, sizeof(dest), "hello");
```

#### 2. 弃用危险函数
- `gets()`函数被弃用（安全风险）
- 推荐使用`fgets()`等安全替代

---

## 实际应用示例

### 一、跨版本兼容性处理

```c
#include <stdio.h>

// 条件编译处理不同版本特性
#if __STDC_VERSION__ >= 199901L
    // C99特性
    #include <stdbool.h>
    bool is_valid = true;
#else
    // C89兼容
    int is_valid = 1;
#endif

#if __STDC_VERSION__ >= 201112L
    // C11特性
    static_assert(sizeof(int) >= 2, "int too small");
#endif

int main(void) {
    #if __STDC_VERSION__ >= 199901L
        // C99支持混合声明
        for (int i = 0; i < 5; i++) {
            printf("C99 loop: %d\n", i);
        }
    #else
        // C89兼容写法
        int i;
        for (i = 0; i < 5; i++) {
            printf("C89 loop: %d\n", i);
        }
    #endif
    
    return 0;
}
```

### 二、现代C语言应用

```c
#include <stdio.h>
#include <stdatomic.h>
#include <threads.h>

// 原子计数器
atomic_int global_counter = ATOMIC_VAR_INIT(0);

// 线程函数
int worker_thread(void* arg) {
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&global_counter, 1);
    }
    return 0;
}

int main(void) {
    // 创建多个线程
    thrd_t threads[4];
    for (int i = 0; i < 4; i++) {
        thrd_create(&threads[i], worker_thread, NULL);
    }
    
    // 等待线程完成
    for (int i = 0; i < 4; i++) {
        thrd_join(threads[i], NULL);
    }
    
    printf("Final counter: %d\n", atomic_load(&global_counter));
    return 0;
}
```

---

## 参考文献与外部链接

1. **Dennis M. Ritchie**："The Development of the C Language"
2. **C99标准理由书**：Rationale for the C99 standard

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C++历史文档] | C++语言发展历史对照 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=c/language/history&oldid=178579>
- 最后修改时间：2024年12月20日 13:51
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C语言作为现代编程语言的基石，其发展历程体现了计算机科学和技术的进步。从最初的B语言到现代的C23标准，每个版本都在保持向后兼容的同时引入了重要的新特性。理解C语言的历史发展，有助于更好地掌握现代C语言的特性和最佳实践。标准化过程确保了C语言的可移植性和稳定性，使其成为系统编程、嵌入式开发和高性能计算的首选语言。从C89的基础特性到C11的多线程支持，再到C23的现代化改进，C语言持续演进以满足现代编程需求。学习各版本标准的差异，对于编写兼容性和安全性更好的C代码具有重要意义。