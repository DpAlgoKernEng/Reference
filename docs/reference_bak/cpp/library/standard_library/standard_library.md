# C++标准库（C++ Standard Library）

来源：cppreference.com

## 标准库概述

**C++标准库**提供了在标准C++中可使用的广泛设施。

### 自定义点对象（C++20）
- 提供扩展点机制

### 仅用于说明的实体
- **decay-copy**（C++11）
- **synth-three-way**、**synth-three-way-result**（C++20）

---

## 标准库分类

### 一、语言支持库（Language Support Library）
提供C++语言某些部分所需的组件，如内存分配（new/delete）和异常处理。

### 二、概念库（Concepts Library）（C++20起）
描述C++程序可用于模板参数编译时验证和基于类型属性进行函数分发的库组件。

### 三、诊断库（Diagnostics Library）
提供一致的错误报告框架，包括预定义的异常类。

### 四、内存管理库（Memory Management Library）
提供内存管理组件，包括智能指针和作用域分配器（C++11起）。

### 五、元编程库（Metaprogramming Library）（C++11起）
描述在模板中和常量求值期间使用的工具，包括类型特征、整数序列（C++14起）和有理数运算。

### 六、通用工具库（General Utilities Library）
包括其他库元素使用的组件，如预定义的动态存储管理分配器，以及C++程序中用作基础设施的组件，如元组和（C++11起）函数包装器。

### 七、容器、迭代器、范围（C++20起）和算法库
为C++程序提供最广泛使用的算法和数据结构子集的访问。

### 八、字符串库（Strings Library）
提供对以下类型表示的同质字符序列的文本操作支持：
- `char`
- `char8_t`（C++20起）
- `char16_t`、`char32_t`（C++11起）
- `wchar_t`
- 任何其他类字符类型

### 九、文本处理库（Text Processing Library）
提供正则表达式匹配和搜索（C++11起）、文本格式化工具（C++20起）、文本编码识别（C++26起）和本地化工具。

### 十、数值库（Numerics Library）
提供扩展数值处理支持的数值算法和复数组件。`valarray`组件提供n-at-a-time处理支持，在支持此类处理的平台上可能实现为并行操作。随机数组件提供生成伪随机数的工具（C++11起）。

### 十一、时间库（Time Library）
提供通用的时间工具。

### 十二、输入输出库（Input/Output Library）
提供iostream组件，这是C++程序输入输出的主要机制。它们可以与其他库元素一起使用，特别是字符串、本地化和迭代器。

### 十三、线程支持库（Thread Support Library）（C++11起）
提供创建和管理线程的组件，包括原子操作、互斥量和线程间通信。

### 十四、执行支持库（Execution Support Library）（C++26起）
提供在通用执行资源上管理异步执行的框架。

---

## 标准库内容

C++标准库提供标准库头文件摘要中描述的实体和宏的定义，除非另有说明。

除`operator new`和`operator delete`外，所有库实体都在`std`命名空间内或`std`命名空间内的嵌套命名空间中定义（C标准库工具的实体除外，见下文）。是否在特定命名空间中直接声明名称，或在该命名空间内的内联命名空间中声明，是未指定的（C++11起）。

### 头文件

C++标准库的每个元素都在**头文件**中声明或定义（视情况而定）。头文件不一定是源文件，头文件名中由`<`和`>`分隔的序列也不一定是有效的源文件名。

C++标准库提供**C++库头文件**和**C库工具的附加C++头文件**（参见'headers'页面获取描述）：

#### C++库头文件

| 基础头文件 | C++11新增 | C++14新增 | C++17新增 | C++20新增 |
|------------|-----------|-----------|-----------|-----------|
| `<algorithm>` | `<array>` | `<shared_mutex>` | `<any>` | `<barrier>` |
| `<bitset>` | `<atomic>` | | `<charconv>` | `<bit>` |
| `<complex>` | `<chrono>` | | `<execution>` | `<compare>` |
| `<deque>` | `<codecvt>`* | | `<filesystem>` | `<concepts>` |
| `<exception>` | `<condition_variable>` | | `<memory_resource>` | `<coroutine>` |
| `<fstream>` | `<forward_list>` | | `<optional>` | `<format>` |
| `<functional>` | `<future>` | | `<string_view>` | `<latch>` |
| `<iomanip>` | `<initializer_list>` | | `<variant>` | `<numbers>` |
| `<ios>` | `<mutex>` | | | `<ranges>` |
| `<iosfwd>` | `<random>` | | | `<semaphore>` |
| `<iostream>` | `<ratio>` | | | `<source_location>` |
| `<istream>` | `<regex>` | | | `<span>` |
| `<iterator>` | `<scoped_allocator>` | | | `<stop_token>` |
| `<limits>` | `<system_error>` | | | `<syncstream>` |
| `<list>` | `<thread>` | | | `<version>` |
| `<locale>` | `<tuple>` | | | |
| `<map>` | `<type_traits>` | | | |
| `<memory>` | `<typeindex>` | | | |
| `<new>` | `<unordered_map>` | | | |
| `<numeric>` | `<unordered_set>` | | | |
| `<ostream>` | | | | |
| `<queue>` | | | | |
| `<set>` | | | | |
| `<sstream>` | | | | |
| `<stack>` | | | | |
| `<stdexcept>` | | | | |
| `<streambuf>` | | | | |
| `<string>` | | | | |
| `<typeinfo>` | | | | |
| `<utility>` | | | | |
| `<valarray>` | | | | |
| `<vector>` | | | | |

#### C++23新增头文件
- `<expected>`
- `<flat_set>`
- `<mdspan>`
- `<spanstream>`
- `<stdfloat>`
- `<flat_map>`
- `<generator>`
- `<print>`
- `<stacktrace>`

#### C++26新增头文件
- `<debugging>`
- `<inplace_vector>`
- `<rcu>`
- `<simd>`
- `<text_encoding>`
- `<hazard_pointer>`
- `<linalg>`

#### 已移除头文件
- `<codecvt>`（C++11起，C++17弃用，C++26移除）
- `<strstream>`（C++98弃用，C++26移除）
- `<ccomplex>`（C++11起，C++17弃用，C++20移除）
- `<ciso646>`（C++20移除）
- `<cstdalign>`（C++11起，C++17弃用，C++20移除）
- `<cstdbool>`（C++11起，C++17弃用，C++20移除）
- `<ctgmath>`（C++11起，C++17弃用，C++20移除）

#### C库工具的C++头文件

| 基础头文件 | C++11新增 |
|------------|-----------|
| `<cassert>` | `<cfenv>` |
| `<cctype>` | `<cinttypes>` |
| `<cerrno>` | `<cstdint>` |
| `<cfloat>` | `<cuchar>` |
| `<climits>` | |
| `<clocale>` | |
| `<cmath>` | |
| `<csetjmp>` | |
| `<csignal>` | |
| `<cstdarg>` | |
| `<cstddef>` | |
| `<cstdio>` | |
| `<cstdlib>` | |
| `<cstring>` | |
| `<ctime>` | |
| `<cwchar>` | |
| `<cwctype>` | |

---

## C标准库集成

C++标准库还提供C标准库的工具，经过适当调整以确保静态类型安全。许多库函数的描述依赖C标准库来提供这些函数的语义。

在某些情况下，C++标准中指定的签名可能与C标准库中的签名不同，并且可能声明额外的重载，但行为和前提条件（包括C的restrict所隐含的前提条件）（C++17起）是相同的，除非另有说明。

为了与C标准库兼容，C++标准库提供以下列出的C头文件。这些头文件的预期用途仅为互操作性。C++源文件可能需要包含其中一个头文件才能成为有效的ISO C。不打算同时作为有效ISO C的源文件不应使用任何C头文件。

### C头文件

| 基础头文件 | C++11新增 | C++23新增 | C++26新增 |
|------------|-----------|-----------|-----------|
| `<assert.h>` | `<complex.h>` | `<stdatomic.h>` | `<stdbit.h>`* |
| `<ctype.h>` | `<fenv.h>` | | `<stdchkint.h>`* |
| `<errno.h>` | `<inttypes.h>` | | |
| `<float.h>` | `<iso646.h>` | | |
| `<limits.h>` | `<setjmp.h>` | | |
| `<locale.h>` | `<signal.h>` | | |
| `<math.h>` | `<stdalign.h>` | | |
| `<setjmp.h>` | `<stdbool.h>` | | |
| `<signal.h>` | `<stdint.h>` | | |
| `<stdarg.h>` | `<tgmath.h>` | | |
| `<stddef.h>` | `<uchar.h>` | | |
| `<stdio.h>` | | | |
| `<stdlib.h>` | | | |
| `<string.h>` | | | |
| `<time.h>` | | | |
| `<wchar.h>` | | | |
| `<wctype.h>` | | | |

除了其他说明外，每个`cxxx`头文件的内容与C标准库中相应`xxx.h`头文件的内容相同。然而，在C++标准库中，声明（除了在C中定义为宏的名称）位于`std`命名空间的作用域内。这些名称（包括添加的任何重载）是否首先在全局命名空间作用域中声明，然后通过显式using声明注入到`std`命名空间中是未指定的。

在C中定义为宏的名称（assert、offsetof、setjmp、va_arg、va_end和va_start）必须在C++标准库中定义为宏，即使C允许作为函数实现。

在C中定义为函数的名称必须在C++标准库中定义为函数。这禁止了C中允许的提供掩码宏的做法。在C++中实现等效的内联行为的唯一方法是提供extern inline函数定义。

在C++中是关键字或运算符的标识符不能在C++标准库头文件中定义为宏。特别是，包含标准头文件`<iso646.h>`没有效果。

---

## 标准C安全函数名称（C++17起）

如果包含了任何C++头文件，则以下C标准附录K名称是否在全局命名空间中声明是实现定义的（它们都不在`std`命名空间中声明）：

### C标准附录K名称示例
```
abort_handler_s, asctime_s, bsearch_s, constraint_handler_t, ctime_s,
errno_t, fopen_s, fprintf_s, freopen_s, fscanf_s, fwprintf_s, fwscanf_s,
gets_s, gmtime_s, ignore_handler_s, localtime_s, L_tmpnam_s, mbsrtowcs_s,
mbstowcs_s, memcpy_s, memmove_s, memset_s, printf_s, qsort_s, RSIZE_MAX,
scanf_s, set_constraint_handler_s, snprintf_s, snwprintf_s, sprintf_s,
sscanf_s, strcat_s, strcpy_s, strerrorlen_s, strerror_s, strlen_s,
strncat_s, strncpy_s, strnlen_s, strtok_s, tmpfile_s, TMP_MAX_S,
tmpnam_s, vfprintf_s, vfscanf_s, vfwprintf_s, vfwscanf_s, vprintf_s,
vscanf_s, vsnprintf_s, vsnwprintf_s, vsprintf_s, vsscanf_s, vswprintf_s,
```

---

## 使用标准库

### 包含头文件

C++标准库中的实体在头文件中定义，当翻译单元包含适当的`#include`预处理指令时，其内容对翻译单元可用。

翻译单元可以以任何顺序包含库头文件。每个头文件可以包含多次，除了包含`<cassert>`或`<assert.h>`的效果每次依赖于`NDEBUG`的词法当前定义外，其效果与恰好包含一次相同。

翻译单元只能在任何声明或定义之外包含头文件，并且在该翻译单元中对这些头文件中声明的任何实体的第一次引用之前。不需要诊断。

#### 模块单元中的头文件包含（C++20起）
在模块单元中，头文件只能在全局模块片段中包含。

#### 导入头文件（C++20起）
C++库头文件，或对于独立实现，实现提供的此类头文件的子集，统称为**可导入的C++库头文件**。当翻译单元包含适当的导入声明时，可导入的C++库头文件的内容对该翻译单元可用。

#### 导入模块（C++20起）
C++标准库提供以下**C++库模块**：

1. **命名模块std**：
   - 导出可导入C++库头文件提供的`std`命名空间中的声明（如来自`<bit>`的`std::rotr`）
   - 导出C++库工具头文件提供的`std`命名空间中的声明（如来自`<cstdio>`的`std::puts`）
   - 额外导出来自`<new>`的存储分配和释放函数的全局命名空间声明（如`::operator new`）

2. **命名模块std.compat**：
   - 导出与命名模块std相同的声明
   - 额外导出C++库工具头文件提供的`std`命名空间中声明对应的全局命名空间声明（如`::fclose`）

对于标准库中的每个声明：
- 它附加到的模块是未指定的
- 无论通过包含头文件、导入头文件单元还是导入C++库模块使其可达，它都表示相同的实体

### 链接

标准库中的实体具有外部链接。除非另有说明，对象和函数具有默认的`extern "C++"`链接。

C标准库中声明为外部链接的名称具有`extern "C"`还是`extern "C++"`链接是实现定义的。C++标准建议在此情况下使用`extern "C++"`。

程序所需的库中定义的对象和函数在程序启动前包含在程序中。

---

## 标准库实现要求

### 保证

C++头文件必须提供以下内容的声明和定义：
- 该头文件摘要中的内容，或
- 该头文件摘要中显示包含的另一个头文件的摘要中的内容

对于在多个头文件中定义的类型和宏（如NULL），以任何顺序包含任意数量的这些头文件从不违反单一定义规则。

除非另有说明，所有由C标准库定义的展开为整型常量表达式的对象类宏都可以在`#if`预处理指令中使用。

调用标准库非成员函数签名总是导致实际调用该函数。因此，符合标准的库实现不能定义可由有效C++程序调用的额外非成员函数。

非成员函数签名从不声明带有额外默认参数。

除非另有说明，标准库函数对非运算符、非成员函数的调用不使用通过参数依赖名称查找找到的来自其他命名空间的函数。

对于类（模板）定义中的函数（模板）友元声明，不为该函数（模板）提供其他声明。

#### C++11起的额外要求：

标准库函数签名只能在要求为constexpr时才声明为constexpr（libstdc++的cmath在此方面明显不符合标准）。如果头文件提供了constexpr函数或构造函数的任何非定义声明，则相应的定义也应在该头文件中提供。

除非另有说明，每个标准库函数应满足以下防止数据竞争的要求：
- C++标准库函数不能（直接或间接）访问当前线程以外的线程可访问的对象，除非通过函数参数（包括this）直接或间接访问这些对象
- C++标准库函数不能（直接或间接）修改当前线程以外的线程可访问的对象，除非通过函数的非const参数（包括this）直接或间接访问这些对象
- 具有静态存储持续时间的对象不能在没有同步的情况下用于内部目的，因为这样做即使在不显式在线程间共享对象的程序中也可能导致数据竞争
- C++标准库函数只能通过在其规范中要求的那些容器元素上调用函数来访问通过其参数或其容器参数元素间接访问的对象
- 调用标准库容器或字符串成员函数获得的迭代器上的操作可以访问但不能修改底层容器
- 特别是，使迭代器无效的容器操作与该容器相关联的迭代器操作冲突
- C++标准库函数只有在那些操作对用户可见时才能仅在当前线程内执行所有操作
- 没有可见副作用的操作可以并行化

对于C++标准库中要求派生自另一个标准库类的每个类：
- 如果指定为虚基类，则基类必须是虚的
- 如果未指定为虚基类，则基类不能是虚的
- 除非另有说明，具有不同名称的类型应该是不同的类型

除非另有说明，C++标准库中指定的所有类型都是非final类型（C++11起）。

如果标准库中定义的函数指定在给定类型的情况下抛出异常，则抛出的异常只能是该类型或派生自该类型的类型，以便基类型异常处理程序可以捕获它。

C标准库函数只有在调用程序提供的抛出异常的函数时才能抛出异常（qsort()和bsearch()满足此条件）。

标准库中定义的析构函数操作从不抛出异常。标准库中的每个析构函数的行为都好像具有无异常抛出规范。

如果标准库中的函数通过std::error_code对象报告错误，则对于源自操作系统的错误，该对象的category()成员必须返回std::system_category()，或对于源自其他地方的错误，返回对实现定义的std::error_category对象的引用。应定义这些错误类别的value()的可能值。

标准库中定义的类型的对象可以被移动。移动操作可以是显式指定的或隐式生成的。除非另有说明，此类移动后的对象将处于有效但未指定的状态。

可以将标准库中定义的类型的对象移动赋值给自己。除非另有说明，此类赋值将对象置于有效但未指定的状态。

### 实现自由度

是否将C++标准库中的任何成员或非成员函数定义为内联是未指定的。

对于非虚的C++标准库成员函数，可以声明不同的成员函数签名集，前提是调用该成员函数时，从给定声明集中选择重载的行为就好像选择了该重载一样。这允许：

- 添加带有默认参数的参数
- 用两个或多个具有等效行为的成员函数替换具有默认参数的成员函数
- 为成员函数名添加额外的签名

除非另有说明，C++标准库中哪些函数可以递归重入是实现定义的。

C++标准库实现可以在受保护免受数据竞争影响且对用户不可见的情况下在线程间共享其自己的内部对象（C++11起）。

C++标准库中的任何函数签名或类是否是另一个标准库类的友元是未指定的。

此处描述的名称和全局函数签名保留给实现使用。

标准库中的任何类都可以派生自具有保留给实现的名称的类。如果标准库中定义的类要求派生自标准库中的其他类，则该类可以直接派生自所需的基类，或通过具有保留给实现名称的基类层次结构间接派生。

如果标准库中定义的函数未指定抛出异常但没有无异常抛出规范，则抛出的异常是实现定义的，但其类型应为std::exception或派生自std::exception的任何类型。

非虚函数的异常规范可以通过添加无异常抛出规范来加强。

---

## 缺陷报告

以下行为改变的缺陷报告已追溯适用于先前发布的C++标准。

| 缺陷报告 | 适用标准 | 发布时的行为 | 正确行为 |
|----------|----------|--------------|----------|
| LWG 1 | C++98 | C标准库名称的语言链接未指定 | 它们是实现定义的 |
| LWG 119 | C++98 | 虚函数的异常规范可以加强 | 仅允许非虚函数 |
| LWG 147 | C++98 | 非成员函数规范仅考虑全局函数 | 也考虑非全局函数 |
| LWG 225 | C++98 | 标准库函数可能由于参数依赖查找调用来自其他命名空间的非成员函数 | 除非另有说明，否则禁止 |
| LWG 336 | C++98 | `<strstream>`不是C++库头文件 | 它是C++库头文件 |
| LWG 343 | C++98 | 库头文件依赖关系未指定 | 指定（在摘要中列出） |
| LWG 456 | C++98 | C++库工具头文件只能在std命名空间中提供定义 | 允许在全局命名空间中定义然后注入到std命名空间 |
| LWG 465 | C++98 | 在C++中是关键字或运算符的标识符可以在C++标准库头文件中定义为宏（只有`<ciso646>`要求不将它们定义为宏） | 所有C++标准库头文件都不能将它们定义为宏 |
| LWG 1178 | C++98 | C++头文件必须包含包含任何所需定义的C++头文件 | C++头文件必须提供其摘要中直接或间接包含的声明和定义 |
| LWG 2013 | C++11 | 未要求标准为constexpr的函数是否可以由标准库声明为constexpr是未指定的 | 禁止 |
| LWG 2225 | C++98 | 如果在不正确位置包含头文件需要诊断 | 此情况下不需要诊断 |

---

## 扩展知识详解

### 一、标准库演进历程

#### 1. 基础设施建立（C++98/03）
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

void cpp98_example() {
    std::vector<int> vec(10);
    std::generate(vec.begin(), vec.end(), std::rand);
    std::sort(vec.begin(), vec.end());
    
    // 基础的STL使用
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
}
```

#### 2. 现代化改进（C++11/14）
```cpp
#include <memory>
#include <thread>
#include <chrono>

void cpp11_example() {
    // 智能指针
    auto ptr = std::make_unique<std::vector<int>>(10);
    
    // 范围for循环
    for (auto& element : *ptr) {
        element = std::rand();
    }
    
    // 线程支持
    std::thread t([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    t.join();
}
```

#### 3. 重大升级（C++17/20）
```cpp
#include <optional>
#include <filesystem>
#include <ranges>
#include <format>

void cpp17_20_example() {
    // 可选值
    std::optional<int> value = 42;
    
    // 文件系统
    std::filesystem::path p = "/home/user/documents";
    
    // 范围库
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto even = vec | std::views::filter([](int n) { return n % 2 == 0; });
    
    // 格式化
    std::string msg = std::format("Value: {}", value.value_or(0));
}
```

#### 4. 前沿发展（C++23/26）
```cpp
#include <expected>
#include <print>
#include <generator>

void cpp23_example() {
    // 预期值
    std::expected<int, std::string> result = 42;
    
    // 现代打印
    std::print("Modern printing: {}\n", result.value());
    
    // 协程生成器
    auto fibonacci = []() -> std::generator<int> {
        int a = 0, b = 1;
        while (true) {
            co_yield a;
            auto temp = a;
            a = b;
            b = temp + b;
        }
    };
}
```

### 二、库设计原则

#### 1. 零开销抽象
```cpp
// 模板提供编译时优化
template<typename Container, typename Predicate>
auto filter(const Container& container, Predicate pred) {
    // 编译时展开，无运行时开销
    Container result;
    for (const auto& item : container) {
        if (pred(item)) {
            result.push_back(item);
        }
    }
    return result;
}
```

#### 2. 值语义和移动语义
```cpp
#include <utility>

class MyClass {
public:
    // 移动构造函数
    MyClass(MyClass&& other) noexcept 
        : data_(std::move(other.data_)) {
        // 零拷贝转移资源
    }
    
    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
        }
        return *this;
    }
    
private:
    std::vector<int> data_;
};
```

#### 3. RAII（资源获取即初始化）
```cpp
#include <fstream>
#include <memory>

class FileHandler {
public:
    explicit FileHandler(const std::string& filename) 
        : file_(std::make_unique<std::ifstream>(filename)) {
        if (!file_->is_open()) {
            throw std::runtime_error("Failed to open file");
        }
    }
    
    // 自动资源管理
    ~FileHandler() = default;
    
    std::istream& get_stream() { return *file_; }
    
private:
    std::unique_ptr<std::ifstream> file_;
};
```

### 三、并发支持演进

#### 1. 基础线程支持（C++11）
```cpp
#include <thread>
#include <mutex>
#include <atomic>

class ThreadSafeCounter {
private:
    std::atomic<int> count_{0};
    mutable std::mutex mtx_;
    
public:
    void increment() {
        count_.fetch_add(1);
    }
    
    int get_value() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return count_.load();
    }
};
```

#### 2. 异步编程（C++11/14）
```cpp
#include <future>
#include <chrono>

auto async_computation() {
    // 异步任务
    auto future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 42;
    });
    
    // 非阻塞处理
    while (future.wait_for(std::chrono::milliseconds(100)) 
           != std::future_status::ready) {
        // 执行其他工作
        std::cout << "Working...\n";
    }
    
    return future.get();
}
```

#### 3. 现代同步原语（C++20）
```cpp
#include <barrier>
#include <latch>
#include <semaphore>

void modern_synchronization() {
    // 屏障 - 等待所有线程到达
    std::barrier barrier(3);
    
    // 门闩 - 等待计数归零
    std::latch latch(2);
    
    // 信号量 - 控制资源访问
    std::counting_semaphore<3> semaphore(3);
}
```

### 四、容器和算法现代化

#### 1. 容器改进（C++11/17/20）
```cpp
#include <vector>
#include <map>
#include <unordered_map>

void container_modernization() {
    // 统一初始化
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 结构化绑定（C++17）
    std::map<std::string, int> m = {{"key1", 1}, {"key2", 2}};
    for (const auto& [key, value] : m) {
        std::cout << key << ": " << value << std::endl;
    }
    
    // 跨越视图（C++20）
    std::span<const int> sp(vec);
    for (int value : sp) {
        std::cout << value << " ";
    }
}
```

#### 2. 范围算法（C++20）
```cpp
#include <ranges>
#include <vector>
#include <algorithm>

void range_algorithms() {
    std::vector<int> vec = {5, 2, 8, 1, 9, 3};
    
    // 管道风格操作
    auto result = vec 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; })
        | std::ranges::to<std::vector>();
    
    // 范围算法
    std::ranges::sort(vec);
    auto found = std::ranges::find(vec, 5);
}
```

#### 3. 新容器类型（C++23/26）
```cpp
#include <flat_map>
#include <expected>

void new_containers() {
    // 扁平映射 - 提高缓存局部性
    std::flat_map<std::string, int> fmap;
    fmap["key"] = 42;
    
    // 预期值容器
    std::expected<std::string, int> exp = "Success";
    if (exp) {
        std::cout << "Value: " << *exp << std::endl;
    } else {
        std::cout << "Error code: " << exp.error() << std::endl;
    }
}
```

### 五、错误处理现代化

#### 1. 异常安全保证
```cpp
#include <memory>
#include <stdexcept>

class ExceptionSafeClass {
public:
    // 强异常安全保证
    void strong_guarantee_operation(const std::vector<int>& data) {
        auto temp = data_;  // 备份当前状态
        try {
            // 执行可能抛出异常的操作
            for (const auto& item : data) {
                temp.push_back(process(item));
            }
            data_ = std::move(temp);  // 只有成功才更新
        } catch (...) {
            // 异常时状态不变
            throw;
        }
    }
    
private:
    std::vector<int> data_;
    
    int process(int value) {
        if (value < 0) {
            throw std::invalid_argument("Negative value");
        }
        return value * 2;
    }
};
```

#### 2. 现代错误处理（C++17/23）
```cpp
#include <expected>
#include <system_error>

// 使用expected替代异常
std::expected<int, std::error_code> safe_divide(int a, int b) {
    if (b == 0) {
        return std::unexpected(std::make_error_code(std::errc::invalid_argument));
    }
    return a / b;
}

void error_handling_modern() {
    auto result = safe_divide(10, 2);
    if (result) {
        std::cout << "Result: " << *result << std::endl;
    } else {
        std::cout << "Error: " << result.error().message() << std::endl;
    }
}
```

---

## 实际应用示例

### 一、现代文件处理系统
```cpp
#include <filesystem>
#include <fstream>
#include <format>
#include <expected>

class ModernFileProcessor {
public:
    std::expected<std::string, std::error_code> 
    read_file(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            return std::unexpected(
                std::make_error_code(std::errc::no_such_file_or_directory));
        }
        
        std::ifstream file(path);
        if (!file.is_open()) {
            return std::unexpected(
                std::make_error_code(std::errc::io_error));
        }
        
        return std::string(std::istreambuf_iterator<char>(file),
                          std::istreambuf_iterator<char>());
    }
    
    std::expected<void, std::error_code>
    write_file(const std::filesystem::path& path, 
               const std::string& content) {
        std::ofstream file(path);
        if (!file.is_open()) {
            return std::unexpected(
                std::make_error_code(std::errc::io_error));
        }
        
        file << content;
        return {};
    }
};
```

### 二、并发数据处理管道
```cpp
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <ranges>

template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex mtx_;
    std::queue<T> queue_;
    std::condition_variable cond_;
    
public:
    void push(T item) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(std::move(item));
        cond_.notify_one();
    }
    
    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) {
            return std::nullopt;
        }
        T item = std::move(queue_.front());
        queue_.pop();
        return item;
    }
    
    T wait_and_pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        cond_.wait(lock, [this] { return !queue_.empty(); });
        T item = std::move(queue_.front());
        queue_.pop();
        return item;
    }
};
```

### 三、现代网络客户端
```cpp
#include <expected>
#include <string>
#include <format>
#include <chrono>
#include <future>

class ModernHttpClient {
public:
    struct HttpResponse {
        int status_code;
        std::string body;
        std::chrono::steady_clock::time_point timestamp;
    };
    
    std::expected<HttpResponse, std::string>
    async_get(const std::string& url) {
        // 模拟异步HTTP请求
        auto future = std::async(std::launch::async, [url]() -> HttpResponse {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return HttpResponse{
                200,
                std::format("Response from {}", url),
                std::chrono::steady_clock::now()
            };
        });
        
        return future.get();
    }
};
```

---

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/standard_library&oldid=179112>
- 最后修改时间：2025年1月2日 22:37
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++标准库是现代C++编程的核心，提供了从基础语言支持到高级并发和范围处理的全面功能。理解标准库的结构、演进历史和设计原则，对于编写高效、安全和现代化的C++程序至关重要。从C++98的基础STL到C++23的前沿特性，标准库不断演进以满足现代软件开发的需求。掌握库组件的最佳实践和使用模式，能够显著提高开发效率和代码质量。库的设计遵循零开销抽象、RAII等核心原则，确保了性能和安全性。随着模块化、协程、范围库等新特性的引入，C++标准库正在变得更加现代化和易用。正确使用异常安全保证和现代错误处理机制，能够构建更加健壮的应用程序。标准库的并发支持为多线程编程提供了强大而安全的工具。持续关注标准库的发展和新特性，是成为一名优秀C++开发者的重要途径。