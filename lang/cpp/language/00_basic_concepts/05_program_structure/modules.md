# 模块 (Modules)

> C++20 起

## 1. 概述 (Overview)

大多数 C++ 项目使用多个翻译单元 (Translation Unit)，因此需要在这些单元之间共享声明和定义。头文件的使用是为此目的的主要手段，例如标准库的声明可以通过包含相应的头文件来提供。

**模块 (Modules)** 是一种语言特性，用于在翻译单元之间共享声明和定义。它是头文件某些用例的替代方案。

### 核心特性

- **与命名空间正交**：模块与命名空间是独立的概念，互不影响
- **更好的封装性**：模块可以控制哪些声明对外可见
- **更快的编译**：模块不需要重复解析相同的头文件内容
- **避免宏污染**：导入头文件单元时，预处理器宏不会影响其处理

### 基本示例

```cpp
// helloworld.cpp
export module helloworld;  // 模块声明

import <iostream>;         // 导入声明

export void hello()        // 导出声明
{
    std::cout << "Hello world!\n";
}

// main.cpp
import helloworld;  // 导入声明

int main()
{
    hello();
}
```

## 2. 来源与演变 (Origin and Evolution)

### 设计动机

模块的设计旨在解决传统头文件机制的以下问题：

1. **编译效率** - 头文件在每个翻译单元中重复解析，导致编译时间过长
2. **宏隔离** - 头文件中的宏定义可能污染包含它的翻译单元
3. **封装性** - 头文件无法有效隐藏实现细节
4. **依赖管理** - 头文件的依赖关系难以自动追踪

### 标准演进

| C++ 版本 | 变更内容 |
|---------|---------|
| C++20 | 首次引入模块核心语言支持 |
| C++23 | 引入标准库模块 `std` 和 `std.compat` |

### 特性测试宏

| 宏名称 | 值 | 标准 | 特性 |
|-------|-----|------|------|
| `__cpp_modules` | 201907L | C++20 | 模块 — 核心语言支持 |
| `__cpp_lib_modules` | 202207L | C++23 | 标准库模块 std 和 std.compat |

### 缺陷报告详解

**CWG 2732**：C++20 标准中不清楚可导入头文件是否可以对导入点的预处理器状态做出反应。修正后规定不会有反应。

**P3034R1**：C++20 标准中模块名和模块分区可以包含定义为对象类宏的标识符。修正后禁止这种情况。

## 3. 语法与参数 (Syntax and Parameters)

### 核心语法形式

| 语法 | 说明 |
|-----|------|
| `export(opt) module 模块名 模块分区(opt) 属性(opt) ;` | (1) 模块声明 |
| `export 声明` | (2) 导出声明 |
| `export { 声明序列(opt) }` | (3) 导出块 |
| `export(opt) import 模块名 属性(opt) ;` | (4) 导入模块 |
| `export(opt) import 模块分区 属性(opt) ;` | (5) 导入模块分区 |
| `export(opt) import 头文件名 属性(opt) ;` | (6) 导入头文件单元 |
| `module;` | (7) 全局模块片段开始 |
| `module : private;` | (8) 私有模块片段开始 |

### 语法详解

#### 3.1 模块声明

```cpp
export(opt) module 模块名 模块分区(opt) 属性(opt) ;
```

- **模块名**：由一个或多个用点分隔的标识符组成（如 `mymodule`、`mymodule.mysubmodule`）
- **点号含义**：点号没有内在含义，但非正式地用于表示层次结构
- **宏限制**：如果模块名或模块分区中的任何标识符被定义为对象类宏，程序是非良构的

#### 3.2 导出声明

```cpp
export 声明
export { 声明序列(opt) }
```

导出命名空间作用域的所有声明（包括定义）。

#### 3.3 导入声明

```cpp
export(opt) import 模块名 ;
export(opt) import 模块分区 ;
export(opt) import 头文件名 ;
```

导入模块单元、模块分区或头文件单元。

## 4. 底层原理 (Underlying Principles)

### 4.1 模块单元类型

| 单元类型 | 声明形式 | 说明 |
|---------|---------|------|
| 主模块接口单元 | `export module 模块名;` | 每个命名模块必须有且仅有一个，其导出内容对导入者可见 |
| 模块接口单元 | `export module 模块名:分区;` | 模块分区接口，需被主模块接口单元导出导入 |
| 模块实现单元 | `module 模块名;` | 模块的实现部分，不导出任何内容 |
| 分区实现单元 | `module 模块名:分区;` | 分区的实现部分 |

### 4.2 模块所有权

**基本规则**：
- 如果声明出现在模块单元的模块声明之后，它就附加到该模块
- 如果实体的声明附加到命名模块，该实体只能在该模块中定义
- 该实体的所有声明必须附加到同一模块

**模块链接**：
- 如果声明附加到命名模块且未被导出，声明的名称具有模块链接
- 不同模块中相同名称的函数是不同的实体

```cpp
export module lib_A;

int f() { return 0; }  // f 具有模块链接
export int x = f();    // x 等于 0

export module lib_B;

int f() { return 1; }  // OK：lib_A 中的 f 和 lib_B 中的 f 是不同实体
export int y = f();    // y 等于 1
```

### 4.3 不附加到模块的声明

以下声明不附加到任何命名模块（因此声明的实体可以在模块外定义）：

1. 具有外部链接的命名空间定义
2. 语言链接规范内的声明

```cpp
export module lib_A;

namespace ns  // ns 不附加到 lib_A
{
    export extern "C++" int f();  // f 不附加到 lib_A
           extern "C++" int g();  // g 不附加到 lib_A
    export              int h();  // h 附加到 lib_A
}
// ns::h 必须在 lib_A 中定义，但 ns::f 和 ns::g 可在其他地方定义
```

### 4.4 头文件单元

头文件单元是从头文件合成的独立翻译单元。导入头文件单元：

- 使其所有定义和声明可访问
- 预处理器宏也可访问（因为导入声明被预处理器识别）
- 与 `#include` 不同，导入点已定义的预处理宏不会影响头文件的处理

## 5. 使用场景 (Use Cases)

### 5.1 基本模块定义与导入

```cpp
/////// A.cpp（模块 'A' 的主模块接口单元）
export module A;

export char const* hello() { return "hello"; }

/////// main.cpp（非模块单元）
import A;

int main()
{
    hello();  // OK：hello 可见
}
```

### 5.2 导出导入（传递性导入）

如果模块 B 导出导入 A，则导入 B 也会使 A 的所有导出可见：

```cpp
/////// A.cpp
export module A;
export char const* hello() { return "hello"; }

/////// B.cpp
export module B;
export import A;  // 导出导入 A
export char const* world() { return "world"; }

/////// main.cpp
import B;  // 同时获得 A 和 B 的导出

int main()
{
    hello();  // OK：来自 A
    world();  // OK：来自 B
}
```

### 5.3 全局模块片段

用于在模块中使用需要预处理器配置的头文件：

```cpp
module;

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

export module A;

import <ctime>;

export double weak_random()
{
    std::timespec ts;
    std::timespec_get(&ts, TIME_UTC);
    srand48(ts.tv_nsec);
    return drand48();
}
```

### 5.4 私有模块片段

允许模块表示为单个翻译单元，同时隐藏实现细节：

```cpp
export module foo;

export int f();

module : private;  // 私有模块片段开始

int f()  // 定义对 foo 的导入者不可达
{
    return 42;
}
```

### 5.5 模块分区

模块分区用于组织大型模块：

```cpp
/////// A-B.cpp（分区接口单元）
export module A:B;
export char const* Hello() { return "Hello"; }

/////// A-C.cpp（分区实现单元）
module A:C;
char const* WorldImpl() { return "World"; }

/////// A.cpp（主模块接口单元）
export module A;

import :C;           // WorldImpl() 仅对 A.cpp 可见
export import :B;    // Hello() 对导入 A 的翻译单元可见

export char const* World()
{
    return WorldImpl();
}
```

### 5.6 导入标准库头文件单元

```cpp
export module A;

import <iostream>;
export import <string_view>;

export void print(std::string_view message)
{
    std::cout << message << std::endl;
}
```

## 6. 代码示例 (Examples)

### 6.1 基础用法：简单模块

```cpp
/////// math.cppm（模块接口文件）
export module math;

export int add(int a, int b) {
    return a + b;
}

export int multiply(int a, int b) {
    return a * b;
}

// 内部函数，不导出
int internal_helper() {
    return 42;
}

/////// main.cpp
import math;
import <iostream>;

int main() {
    std::cout << "1 + 2 = " << add(1, 2) << std::endl;
    std::cout << "3 * 4 = " << multiply(3, 4) << std::endl;
    // internal_helper();  // 错误：不可见
    return 0;
}
```

### 6.2 基础用法：导出命名空间

```cpp
/////// strings.cppm
export module strings;

export namespace str_utils {
    const char* trim(const char* s) { return s; }
    int length(const char* s) {
        int len = 0;
        while (s[len]) ++len;
        return len;
    }
}

/////// main.cpp
import strings;
import <iostream>;

int main() {
    const char* text = "Hello";
    std::cout << "Length: " << str_utils::length(text) << std::endl;
    return 0;
}
```

### 6.3 高级用法：分层模块设计

```cpp
/////// core.cppm
export module mylib.core;

export namespace core {
    void init() { /* 初始化 */ }
    void cleanup() { /* 清理 */ }
}

/////// utils.cppm
export module mylib.utils;

export import mylib.core;  // 传递性导入

export namespace utils {
    void helper() {
        core::init();
        // ...
        core::cleanup();
    }
}

/////// main.cpp
import mylib.utils;  // 同时获得 core 和 utils

int main() {
    core::init();     // OK：可见
    utils::helper();  // OK：可见
    return 0;
}
```

### 6.4 高级用法：模块分区组织

```cpp
/////// container_vector.cppm
export module containers:vector;

export namespace containers {
    template<typename T>
    class vector {
        // 实现...
    };
}

/////// container_list.cppm
export module containers:list;

export namespace containers {
    template<typename T>
    class list {
        // 实现...
    };
}

/////// containers.cppm（主模块接口）
export module containers;

export import :vector;
export import :list;

// 额外的通用功能
export namespace containers {
    template<typename Container>
    auto size(const Container& c) {
        return c.size();
    }
}

/////// main.cpp
import containers;

int main() {
    containers::vector<int> v;
    containers::list<int> l;
    return 0;
}
```

### 6.5 高级用法：与传统头文件混合

```cpp
/////// legacy_wrapper.cppm
module;

// 传统头文件，可能需要宏配置
#define LEGACY_API_VERSION 2
#include "legacy_api.h"

export module legacy_wrapper;

export namespace modern {
    // 封装传统 API
    void do_something() {
        legacy_function();  // 来自 legacy_api.h
    }
}

/////// main.cpp
import legacy_wrapper;

int main() {
    modern::do_something();
    return 0;
}
```

### 6.6 常见错误及修正

**错误示例 1：在模块单元中使用 #include**

```cpp
// 错误代码
export module mymodule;

#include <vector>  // 错误：应该使用 import

export void process(std::vector<int>& v);
```

```cpp
// 修正方法：使用 import
export module mymodule;

import <vector>;  // 正确

export void process(std::vector<int>& v);
```

**错误示例 2：导入位置错误**

```cpp
// 错误代码
export module mymodule;

export void f() { }

import <iostream>;  // 错误：import 必须在模块声明后、其他声明前
```

```cpp
// 修正方法：正确放置 import
export module mymodule;

import <iostream>;  // 正确：import 紧跟模块声明

export void f() { }
```

**错误示例 3：跨模块定义实体**

```cpp
// 错误代码
/////// module_a.cppm
export module A;
export int g();  // 声明附加到模块 A

/////// module_b.cppm
export module B;
int g() { return 1; }  // 错误：定义附加到模块 B，与声明不匹配
```

```cpp
// 修正方法：在同一模块中定义
/////// module_a.cppm
export module A;
export int g();

/////// module_a_impl.cpp
module A;
int g() { return 1; }  // 正确：在同一模块中定义
```

**错误示例 4：导出模块实现分区**

```cpp
// 错误代码
/////// A-impl.cpp
module A:impl;  // 模块实现分区

/////// A.cppm
export module A;
export import :impl;  // 错误：不能导出模块实现单元
```

```cpp
// 修正方法：实现分区仅内部使用
/////// A-impl.cpp
module A:impl;
int internal_func() { return 42; }

/////// A.cppm
export module A;
import :impl;  // 正确：非导出导入，仅内部使用

export int get_value() {
    return internal_func();  // 可在模块内部使用
}
```

## 7. 总结 (Summary)

### 核心要点

1. **模块与头文件**：模块是头文件的替代方案，提供更好的封装性和编译效率

2. **模块单元类型**：
   - 主模块接口单元（必须且仅一个）
   - 模块实现单元
   - 模块分区（接口/实现）

3. **导入规则**：
   - 所有 import 声明必须分组在模块声明之后、其他声明之前
   - 导出导入提供传递性可见性
   - 头文件可以导入为头文件单元

4. **模块所有权**：
   - 附加到模块的实体只能在该模块中定义
   - 未导出的声明具有模块链接
   - 命名空间定义和语言链接规范内的声明不附加到模块

### 模块与传统头文件对比

| 特性 | 传统头文件 | 模块 |
|-----|-----------|------|
| 编译效率 | 每次重新解析 | 编译一次，重复使用 |
| 宏隔离 | 宏可污染包含者 | 导入点宏不影响导入内容 |
| 封装性 | 所有内容可见 | 仅导出内容可见 |
| 依赖管理 | 依赖关系隐含 | 依赖关系明确 |
| 循环依赖 | 容易产生 | 更难产生（需分区处理） |

### 相关概念

- **翻译单元 (Translation Unit)** - 编译器处理的基本单元
- **模块接口单元** - 导出声明的模块单元
- **模块实现单元** - 不导出的模块单元
- **模块分区** - 模块内部的逻辑划分
- **头文件单元** - 从头文件合成的模块单元

### 学习建议

1. 新项目优先考虑使用模块，提高编译效率和代码组织
2. 理解模块所有权规则，避免跨模块定义错误
3. 使用模块分区组织大型模块，保持清晰的接口边界
4. 与传统代码集成时，使用全局模块片段处理需要宏配置的头文件
5. 注意 import 声明的位置要求，必须在模块声明后、其他声明前