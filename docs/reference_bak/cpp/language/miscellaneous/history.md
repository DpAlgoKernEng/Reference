# C++语言历史（History of C++）

来源：cppreference.com

## 早期C++发展

### 1979年：带类的C（C with Classes）首次实现
#### 新增特性：
1. **核心语言特性**：
   - 类（classes）
   - 成员函数（member functions）
   - 派生类（derived classes）
   - 分离编译（separate compilation）
   - 公共和私有访问控制（public and private access control）
   - 友元（friends）
   - 函数参数类型检查（type checking of function arguments）
   - 默认参数（default arguments）
   - 内联函数（inline functions）
   - 重载赋值运算符（overloaded assignment operator）
   - 构造函数和析构函数（constructors, destructors）
   - `f()`与`f(void)`等价
   - 调用函数和返回函数（call-function and return-function）（同步特性，未进入C++）

2. **库支持**：
   - 并发任务库（concurrent task library）（未进入C++）

### 1982年：带类的C参考手册发布

### 1984年：C84实现，参考手册发布

### 1985年：Cfront 1.0
#### 新增特性：
- 虚函数（virtual functions）
- 函数和运算符重载（function and operator overloading）
- 引用（references）
- new和delete运算符（new and delete operators）
- `const`关键字
- 作用域解析运算符（scope resolution operator）

#### 库增加：
- 复数（complex number）
- 字符串（`string`，AT&T版本）
- I/O流（I/O stream）

### 1985年：《C++程序设计语言》第一版

### 1986年："whatis?"论文发布
记录剩余设计目标，包括：多重继承、异常处理、模板

### 1987年：GCC 1.15.3支持C++

### 1989年：Cfront 2.0
#### 新增特性：
- 多重继承（multiple inheritance）
- 成员指针（pointers to members）
- 受保护访问（protected access）
- 类型安全链接（type-safe linkage）
- 抽象类（abstract classes）
- 静态和const限定的成员函数（static and const-qualified member functions）
- 类特定的new和delete（class-specific new and delete）

#### 库增加：
- I/O操纵器（I/O manipulators）

### 1990年：《带注释的C++参考手册》
描述了设计中的语言，包括一些尚未实现的特性，作为ISO标准前的事实标准。

#### 新增特性：
- 命名空间（namespaces）
- 异常处理（exception handling）
- 嵌套类（nested classes）
- 模板（templates）

### 1991年：Cfront 3.0

### 1991年：《C++程序设计语言》第二版

---

## 标准化C++

### 1990年：ANSI C++委员会成立

### 1991年：ISO C++委员会成立

### 1992年：STL用C++实现

---

## C++98/03时期

### 1998年：**C++98**（ISO/IEC 14882:1998）
#### 新增特性：
1. **运行时类型识别**（RTTI）：
   - `dynamic_cast`
   - `typeid`
2. **其他特性**：
   - 协变返回类型（covariant return types）
   - 转换运算符（cast operators）
   - `mutable`关键字
   - `bool`类型
   - 条件中的声明（declarations in conditions）
   - 模板实例化（template instantiations）
   - 成员模板（member templates）
   - `export`关键字

#### 库增加：
- 本地化（locales）
- 位集（bitset）
- 数值数组（valarray）
- 自动指针（auto_ptr）
- 模板化字符串（templatized string）
- I/O流（I/O streams）
- 复数（complex numbers）

#### 基于STL：
- 容器（containers）
- 算法（algorithms）
- 迭代器（iterators）
- 函数对象（function objects）

### 1998年：《C++程序设计语言》第三版

### 1999年：Boost由委员会成员创立
目标是为标准生成新的高质量候选库

### 2003年：**C++03**（ISO/IEC 14882:2003）
这是一次小修订，主要是技术勘误，引入了值初始化定义。

#### 修复缺陷报告（92个核心，125个库）：
- CWG#1-#304（核心工作组）
- LWG#1-#227（库工作组）

### 2006年：性能技术报告（ISO/IEC TR 18015:2006）
讨论了各种C++抽象的代价，提供实现指导，讨论嵌入式系统中的C++使用，并引入与C的ISO/IEC TR 18037:2008 `<iohw.h>`对应的`<hardware>`接口。

---

## 技术报告和扩展

### 2007年：库扩展TR1（ISO/IEC TR 19768:2007）

#### 从Boost引入：
- `reference_wrapper`
- 智能指针（Smart pointers）
- 成员函数（Member function）
- `result_of`
- `bind`
- `function`
- 类型特征（Type Traits）
- 随机数（Random）
- 数学特殊函数（Mathematical Special Functions）
- 元组（tuple）
- 数组（array）
- 无序容器（Unordered Containers，包括哈希）
- 正则表达式（Regular Expressions）

#### 从C99引入：
- C99新增的`<math.h>`数学函数
- 空白字符类
- 浮点环境
- 十六进制浮点I/O操纵器
- 固定大小整型类型
- `long long`类型
- `va_copy`
- `snprintf()`和`vfscanf()`函数族
- `printf()`和`scanf()`函数族的C99转换说明符

> ⚠️ TR1中除特殊函数外的所有内容都被纳入C++11，略有修改。

### 2010年：数学特殊函数（ISO/IEC 29124:2010）
添加TR1中的特殊函数但未被纳入C++11的部分：椭圆积分、指数积分、拉盖尔多项式、勒让德多项式、埃尔米特多项式、贝塞尔函数、诺依曼函数、贝塔函数、黎曼ζ函数。该标准被合并到C++17。

---

## C++11时期

### 2011年：**C++11**（ISO/IEC 14882:2011）
大量变化被引入，既标准化了现有实践，也改善了C++程序员可用的抽象。

### 2011年：十进制浮点TR（ISO/IEC TR 24733:2011）
实现IEEE 754-2008标准的十进制浮点类型：
- `std::decimal::decimal32`
- `std::decimal::decimal64`
- `std::decimal::decimal128`

### 2012年：标准C++基金会成立

### 2013年：《C++程序设计语言》第四版

---

## C++14时期

### 2014年：**C++14**
C++标准的小修订

### 2015年：文件系统库TS（ISO/IEC TS 18822:2015）
基于boost.filesystem V3的文件系统库实验性扩展，被合并到C++17。

### 2015年：并行扩展TS（ISO/IEC TS 19570:2015）
为所有标准库算法标准化并行和向量并行API，添加新算法如`reduce`、`transform_reduce`、`exclusive_scan`，被合并到C++17。

### 2015年：事务内存扩展TS（ISO/IEC TS 19841:2015）
扩展C++核心语言，添加同步和原子块以及事务安全函数，实现事务内存语义。

### 2015年：库基础扩展TS（ISO/IEC TS 19568:2015）
为C++标准库添加多个新组件：`optional`、`any`、`string_view`、`sample`、`search`、`apply`、多态分配器、类型特征的变量模板，被合并到C++17。

### 2015年：概念扩展TS（ISO/IEC TS 19217:2015）
扩展C++核心语言，添加概念（命名类型要求）和约束（对模板、函数和变量声明中允许的类型的限制），辅助元编程并简化模板实例化诊断，被合并到C++20（有部分省略）。

### 2016年：并发扩展TS（ISO/IEC TS 19571:2016）
扩展C++库，包括对`std::future`的多个扩展、门闩和屏障、原子智能指针。

---

## C++17时期

### 2017年：**C++17**（n4659 2017-03-21最终草案）
C++11后的重大修订

### 2017年：范围扩展TS（ISO/IEC TS 21425:2017）
扩展C++库，包括范围（ranges）、范围视图、哨兵范围、投影变换、新的迭代器适配器和算法，使`sort(v)`成为可能。

### 2017年：协程扩展TS（ISO/IEC TS 22277:2017）
扩展C++核心语言和标准库，包括无栈协程（可恢复函数），添加关键字`co_await`、`co_yield`、`co_return`。

### 2018年：网络扩展TS（ISO/IEC TS 19216:2018）
扩展C++库，包括基于boost.asio的TCP/IP网络。

### 2018年：模块扩展TS（ISO/IEC TS 21544:2018）
扩展C++核心语言，包括模块，添加特殊标识符`module`、`import`，并以新含义重新引入关键字`export`。

### 2018年：并行扩展第2版TS（ISO/IEC TS 19570:2018）
扩展C++库，包括两个新执行策略（`unseq`和`vec`）、额外的并行算法、任务块、SIMD类型和操作。

---

## C++20时期

### 2020年：**C++20**（最终草案n4860 2020-03-31）
C++17后的重大修订

### 2021年：反射TS（ISO/IEC TS 23619:2021）
扩展C++，提供检查程序实体（变量、枚举、类及其成员、lambda及其捕获等）的工具。

---

## 未来发展方向

### 实验性技术规范

### 2026年：**C++23**最新草案n5001（2024-12-17）
下一个主要的C++标准修订版

---

## C++发展里程碑

| 年份 | 标准 | 关键特性 |
|------|------|----------|
| 1979 | 带类的C | 类、继承、封装 |
| 1985 | Cfront 1.0 | 虚函数、重载、引用 |
| 1989 | Cfront 2.0 | 多重继承、抽象类 |
| 1998 | C++98 | STL、RTTI、异常处理 |
| 2003 | C++03 | 技术修正 |
| 2011 | C++11 | 自动类型推导、lambda、智能指针 |
| 2014 | C++14 | 小幅改进 |
| 2017 | C++17 | 结构化绑定、并行算法 |
| 2020 | C++20 | 概念、范围、协程 |

---

## 扩展知识详解

### 一、C++语言设计理念

#### 1. 向后兼容性
```cpp
// C++保持与C的兼容性
extern "C" {
    #include <stdio.h>  // C风格头文件
}

int main() {
    printf("Hello from C++\n");  // C函数调用
    return 0;
}
```

#### 2. 零开销抽象
```cpp
// 模板提供零开销抽象
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;  // 编译时展开，无运行时开销
}
```

### 二、重要特性演进

#### 1. C++98到C++11关键变化
```cpp
// C++98风格
std::vector<int> v;
for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    // 复杂的迭代器声明
}

// C++11风格
auto it = v.begin();  // 自动类型推导
for (auto& element : v) {  // 范围for循环
    // 简洁的语法
}
```

#### 2. 智能指针演进
```cpp
// C++98 - 手动内存管理风险
int* ptr = new int(42);
delete ptr;  // 忘记delete会导致内存泄漏

// C++11 - 智能指针自动管理
std::unique_ptr<int> smart_ptr = std::make_unique<int>(42);
// 自动释放内存
```

### 三、标准库发展

#### 1. 容器演进
```cpp
// C++98 - 基础容器
std::vector<int> vec;
std::map<std::string, int> m;

// C++11 - 新容器
std::unordered_map<std::string, int> um;  // 哈希表
std::array<int, 5> arr = {1, 2, 3, 4, 5};  // 固定大小数组
```

#### 2. 并发支持演进
```cpp
// C++11前 - 无标准并发支持
// 需要平台特定API

// C++11 - 标准线程库
#include <thread>
#include <mutex>

std::mutex mtx;
void worker() {
    std::lock_guard<std::mutex> lock(mtx);
    // 线程安全操作
}
```

### 四、现代C++特性

#### 1. Lambda表达式（C++11）
```cpp
// 简洁的匿名函数
auto lambda = [](int x, int y) { return x + y; };
int result = lambda(3, 4);  // 结果：7
```

#### 2. 概念约束（C++20）
```cpp
// C++20概念 - 编译时类型约束
template<std::integral T>
T add(T a, T b) {
    return a + b;
}
```

#### 3. 范围库（C++20）
```cpp
// 现代范围操作
std::vector<int> vec = {1, 2, 3, 4, 5};
auto even = vec | std::views::filter([](int n) { return n % 2 == 0; });
// 管道风格操作
```

### 五、语言演进哲学

#### 1. 渐进式改进
```cpp
// 每个标准版本都是对前一版本的渐进改进
// C++98 → C++03 → C++11 → C++14 → C++17 → C++20
```

#### 2. 库驱动发展
```cpp
// Boost库推动标准发展
// TR1大部分来源于Boost
#include <boost/optional.hpp>  // Boost版本
#include <optional>            // C++17标准版本
```

---

## 实际应用示例

### 一、跨版本兼容代码

```cpp
#include <iostream>
#include <vector>

// 条件编译支持不同版本特性
#if __cplusplus >= 201103L
    // C++11特性
    #include <memory>
    using SmartPtr = std::unique_ptr<int>;
#else
    // C++98/03兼容
    #include <memory>
    using SmartPtr = std::auto_ptr<int>;
#endif

#if __cplusplus >= 201703L
    // C++17特性
    #include <optional>
    std::optional<int> getValue(bool valid) {
        if (valid) return 42;
        else return std::nullopt;
    }
#endif

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    #if __cplusplus >= 201103L
        // C++11范围for循环
        for (const auto& value : vec) {
            std::cout << value << " ";
        }
    #else
        // C++98/03传统循环
        for (std::vector<int>::const_iterator it = vec.begin(); 
             it != vec.end(); ++it) {
            std::cout << *it << " ";
        }
    #endif
    
    std::cout << std::endl;
    return 0;
}
```

### 二、现代C++实践

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <optional>

// C++17结构化绑定
auto createPerson() {
    return std::make_pair("Alice", 30);
}

// C++20概念约束
template<std::integral T>
T safeAdd(T a, T b) {
    if (a > 0 && b > 0 && a > std::numeric_limits<T>::max() - b) {
        throw std::overflow_error("Integer overflow");
    }
    return a + b;
}

int main() {
    // C++11自动类型推导
    auto vec = std::vector{1, 2, 3, 4, 5};
    
    // C++20范围算法
    #if __cplusplus >= 202002L
        std::ranges::sort(vec);
        auto even = std::ranges::find_if(vec, [](int n) { return n % 2 == 0; });
    #else
        std::sort(vec.begin(), vec.end());
        auto even = std::find_if(vec.begin(), vec.end(), 
                                [](int n) { return n % 2 == 0; });
    #endif
    
    // C++17结构化绑定
    auto [name, age] = createPerson();
    std::cout << name << " is " << age << " years old\n";
    
    // 智能指针
    auto ptr = std::make_unique<int>(42);
    std::cout << "Value: " << *ptr << std::endl;
    
    return 0;
}
```

---

## 参考文献与外部链接

1. **A History of C++: 1979-1991**
2. **Evolving a language in and for the real world: C++ 1991-2006**
3. **Thriving in a crowded and changing world: C++ 2006-2020**
4. **Standard C++ foundation**
5. **C++ on Wikipedia**
6. **C++ Standards Committee**

---

## 相关页面

| 页面名称 | 说明 |
|----------|------|
| [C语言历史文档] | C语言发展历史对照 |

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/language/history&oldid=176073>
- 最后修改时间：2024年9月13日 11:08
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++语言的发展历程体现了编程语言设计的演进过程，从最初的带类C到现代的C++20，每个版本都在保持向后兼容的同时引入了重要的新特性。理解C++的历史发展有助于更好地掌握现代C++的特性和最佳实践。标准化过程确保了C++的可移植性和稳定性，使其成为系统编程、高性能计算和现代应用程序开发的重要工具。从C++98的基础特性到C++20的现代化改进，C++持续演进以满足现代软件开发的复杂需求。学习各版本标准的差异和演进，对于编写高效、安全和可维护的C++代码具有重要意义。C++的渐进式发展哲学和库驱动发展策略，为其他编程语言的设计提供了宝贵经验。