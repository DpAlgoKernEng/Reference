# main 函数 (Main Function)

## 1. 概述 (Overview)

每个 C++ 程序都必须包含一个名为 `main` 的全局函数，该函数是宿主环境中程序的指定入口点。

### main 函数的角色

| 特性 | 说明 |
|-----|------|
| 入口点 | 程序执行的起始位置 |
| 环境类型 | 宿主环境（有操作系统） |
| 定义要求 | 全局函数，必须返回 `int` |

## 2. 来源与演变 (Origin and Evolution)

### 历史背景

`main` 函数作为程序入口的概念源于 C++ 的 C 语言根源。C++ 继承了这一设计，并添加了更严格的限制和更丰富的语义。

### 版本演进

| C++ 标准版本 | 发布年份 | main 函数相关改进 |
|-------------|---------|------------------|
| C++98 | 1998 | 标准化 main 函数形式，隐式返回 0 |
| C++11 | 2011 | 禁止 `constexpr`，禁止 C 语言链接 |
| C++14 | 2014 | 禁止返回类型推导 (`auto`) |
| C++20 | 2020 | 禁止 `consteval`、协程、模块附加 |

### 缺陷报告

| 缺陷报告 | 问题 | 解决方案 |
|---------|------|---------|
| CWG 1003 | 参数名限制过严 | 支持所有有效参数名 |
| CWG 1886 | main 可声明语言链接 | 禁止 |
| CWG 2479 | main 可声明 `consteval` | 禁止 |
| CWG 2811 | main 是否被使用不明确 | 命名时视为使用 |

## 3. 语法与参数 (Syntax and Parameters)

### 3.1 函数签名

```cpp
// 形式 1: 无参数
int main() { body }

// 形式 2: 带命令行参数
int main(int argc, char* argv[]) { body }

// 形式 3: 实现定义的签名
int main(/* implementation-defined */) { body }
```

| 形式 | 说明 |
|-----|------|
| 形式 1 | 独立于环境提供参数的 main 函数 |
| 形式 2 | 接受环境提供参数的 main 函数 |
| 形式 3 | 实现定义的类型，返回 `int` |

### 3.2 参数说明

| 参数 | 类型 | 说明 |
|-----|------|------|
| `argc` | `int` | 非负值，表示传递给程序的参数数量 |
| `argv` | `char*[]` | 指向参数字符串数组的指针 |
| `body` | 复合语句 | main 函数的函数体 |

#### 参数详解

**argc (Argument Count)**：
- 非负整数值
- 表示从执行环境传递给程序的参数数量

**argv (Argument Vector)**：
- 指向 `argc + 1` 个指针的数组
- `argv[argc]` 保证是空指针
- `argv[0]` 到 `argv[argc-1]` 指向以空字符结尾的多字节字符串

#### argv 数组结构

```
argv[0]      --> "program_name"   (程序名，可能为空字符串)
argv[1]      --> "arg1"           (第一个命令行参数)
argv[2]      --> "arg2"           (第二个命令行参数)
...
argv[argc-1] --> "argN"           (最后一个命令行参数)
argv[argc]   = nullptr            (空指针终止符)
```

### 3.3 参数命名灵活性

参数名可以自由选择，类型声明也有等效形式：

```cpp
// 以下都是等效的
int main(int argc, char* argv[])
int main(int argc, char** argv)
int main(int ac, char** av)
int main(int count, char* args[])
```

### 3.4 实现定义的扩展形式

常见扩展形式包含环境变量参数：

```cpp
// 常见扩展：第三参数为环境变量
int main(int argc, char* argv[], char* envp[])
```

**标准建议**：实现定义的额外参数应放在 `argv` 之后。

## 4. 底层原理 (Underlying Principles)

### 4.1 程序启动流程

```
程序加载
    ↓
非局部静态对象初始化
    ↓
调用 main 函数
    ↓
程序执行
    ↓
main 返回
    ↓
销毁自动存储期对象
    ↓
调用 std::exit(return_value)
    ↓
销毁静态存储期对象
    ↓
程序终止
```

### 4.2 main 函数的特殊属性

| 属性 | 说明 |
|-----|------|
| 隐式返回 0 | 如果控制流到达函数末尾而未执行 `return`，等同于 `return 0;` |
| 返回等价于 std::exit | 返回时先销毁自动对象，再调用 `std::exit()` |

#### 返回语义详解

```cpp
int main() {
    // ...
    return 0;
    // 等价于:
    // 1. 正常离开函数（销毁自动存储期对象）
    // 2. 调用 std::exit(0)（销毁静态对象并终止程序）
}
```

### 4.3 main 函数的限制

违反以下限制将导致程序**非良构 (ill-formed)**：

#### 禁止的操作

| 限制 | 说明 | 引入版本 |
|-----|------|---------|
| 不能被命名 | 不能在程序任何地方引用 main | C++98 |
| 不能递归调用 | main 不能调用自己 | C++98 |
| 不能取地址 | 不能获取 main 的函数指针 | C++98 |
| 不能用于 typeid/decltype | 不能在这些表达式中使用 | C++11 |
| 不能预定义或重载 | 全局命名空间的 main 名保留给函数 | C++98 |
| 不能定义为 deleted | 不能 `= delete` | C++11 |
| 不能有语言链接说明 | 不能 `extern "C"` 等 | C++98 |
| 不能是 constexpr | 不能 `constexpr main()` | C++11 |
| 不能是 consteval | 不能 `consteval main()` | C++20 |
| 不能是 inline | 不能 `inline main()` | C++98 |
| 不能是 static | 不能 `static main()` | C++98 |
| 不能推导返回类型 | 不能 `auto main()` | C++14 |
| 不能是协程 | 不能使用 co_await 等 | C++20 |
| 不能附加到命名模块 | 不能在模块中使用 | C++20 |

#### 名字保留规则

```cpp
// 全局命名空间：main 保留给函数
int main() {}              // OK

void main() {}             // 错误：返回类型必须是 int
int main(int x) {}         // 错误：非标准签名（除非实现定义）

// 非全局命名空间：main 可用于其他实体
namespace MyApp {
    class main {};         // OK
    void main() {}         // OK（但注意不能有 C 语言链接）
}

// 错误：不能有 C 语言链接
extern "C" void main() {}  // 错误！
```

### 4.4 宿主环境 vs 独立环境

| 特性 | 宿主环境 | 独立环境 |
|-----|---------|---------|
| 入口点名称 | `main` | 实现定义 |
| 入口点类型 | 标准定义 | 实现定义 |
| 典型场景 | 操作系统应用程序 | 引导加载器、操作系统内核、嵌入式系统 |

### 4.5 函数 try 块的注意事项

```cpp
#include <iostream>

int main() try {
    // 主逻辑
    throw std::runtime_error("error");
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    // 注意：静态对象析构函数抛出的异常不会被捕获！
    // 因为隐式 std::exit 发生在 main 返回之后
}
```

## 5. 使用场景 (Use Cases)

### 5.1 典型应用场景

| 场景 | 推荐形式 |
|-----|---------|
| 简单程序 | `int main()` |
| 命令行工具 | `int main(int argc, char* argv[])` |
| 需要环境变量 | `int main(int argc, char* argv[], char* envp[])`（扩展） |
| 库/模块测试 | 带参数的 main 用于测试配置 |

### 5.2 命令行参数处理最佳实践

```cpp
#include <iostream>
#include <cstdlib>
#include <string_view>

void print_usage(std::string_view program_name) {
    std::cerr << "用法: " << program_name << " <输入文件> <输出文件>\n";
}

int main(int argc, char* argv[]) {
    // 检查参数数量
    if (argc != 3) {
        print_usage(argv[0] ? argv[0] : "program");
        return EXIT_FAILURE;
    }

    std::string_view input_file = argv[1];
    std::string_view output_file = argv[2];

    // 处理文件...
    std::cout << "输入: " << input_file << ", 输出: " << output_file << '\n';

    return EXIT_SUCCESS;
}
```

### 5.3 注意事项

1. **argv 可修改性**：
   - 参数字符串可以被修改
   - 修改会持续到程序终止
   - 修改不会传回执行环境

2. **argv[0] 的不确定性**：
   - 可能是程序名、空字符串或 `nullptr`
   - 不应依赖其值进行关键逻辑

3. **环境变量访问**：
   - 可移植方式：使用 `std::getenv()`
   - 扩展方式：`envp` 参数（非标准）

4. **返回值语义**：
   - 使用 `EXIT_SUCCESS` 和 `EXIT_FAILURE` 提高可读性
   - 显式返回比依赖隐式返回更清晰

## 6. 代码示例 (Examples)

### 6.1 基本用法：打印命令行参数

```cpp
#include <cstdlib>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "argc == " << argc << '\n';

    for (int ndx{}; ndx != argc; ++ndx) {
        std::cout << "argv[" << ndx << "] == "
                  << std::quoted(argv[ndx]) << '\n';
    }

    // 验证 argv[argc] == nullptr
    std::cout << "argv[" << argc << "] == "
              << static_cast<void*>(argv[argc]) << '\n';

    return argc == 3 ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

**执行示例**：
```bash
$ ./convert table_in.dat table_out.dat
argc == 3
argv[0] == "./convert"
argv[1] == "table_in.dat"
argv[2] == "table_out.dat"
argv[3] == 0
```

### 6.2 命令行选项解析

```cpp
#include <iostream>
#include <cstdlib>
#include <string>
#include <optional>
#include <string_view>

struct Config {
    bool verbose = false;
    std::optional<std::string> filename;
};

int main(int argc, char* argv[]) {
    Config config;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];

        if (arg == "-v" || arg == "--verbose") {
            config.verbose = true;
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "用法: " << argv[0] << " [-v] <文件>\n";
            return EXIT_SUCCESS;
        } else if (!arg.starts_with('-')) {
            config.filename = std::string(arg);
        } else {
            std::cerr << "未知选项: " << arg << '\n';
            return EXIT_FAILURE;
        }
    }

    if (!config.filename) {
        std::cerr << "错误: 未指定文件\n";
        return EXIT_FAILURE;
    }

    if (config.verbose) {
        std::cout << "处理文件: " << *config.filename << '\n';
    }

    // 处理文件...

    return EXIT_SUCCESS;
}
```

### 6.3 使用 std::strtok 修改参数

```cpp
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {
    // 演示修改 argv 字符串
    for (int i = 1; i < argc; ++i) {
        std::cout << "原始参数: " << argv[i] << '\n';

        // strtok 会修改字符串
        char* token = std::strtok(argv[i], ",;:");

        while (token != nullptr) {
            std::cout << "  token: " << token << '\n';
            token = std::strtok(nullptr, ",;:");
        }
    }

    return EXIT_SUCCESS;
}
```

### 6.4 访问环境变量（可移植方式）

```cpp
#include <iostream>
#include <cstdlib>

int main() {
    // 可移植的环境变量访问方式
    if (const char* path = std::getenv("PATH")) {
        std::cout << "PATH: " << path << '\n';
    }
    if (const char* home = std::getenv("HOME")) {
        std::cout << "HOME: " << home << '\n';
    }
    if (const char* user = std::getenv("USER")) {
        std::cout << "USER: " << user << '\n';
    }

    return EXIT_SUCCESS;
}
```

### 6.5 RAII 与 main 函数返回

```cpp
#include <iostream>
#include <fstream>
#include <string>

class Resource {
public:
    Resource(const std::string& name) : name_(name) {
        std::cout << "获取资源: " << name_ << '\n';
    }
    ~Resource() {
        std::cout << "释放资源: " << name_ << '\n';
    }
private:
    std::string name_;
};

int main() {
    Resource r1("自动存储期对象");

    // 隐式 return 0; 触发：
    // 1. 销毁 r1（自动存储期对象）
    // 2. 调用 std::exit(0)
    // 3. 销毁静态对象（如果有）
    // 4. 程序终止
}
```

### 6.6 函数 try 块

```cpp
#include <iostream>
#include <stdexcept>

int main() try {
    std::cout << "程序开始\n";

    throw std::runtime_error("测试异常");

    return EXIT_SUCCESS;

} catch (const std::exception& e) {
    std::cerr << "捕获异常: " << e.what() << '\n';
    return EXIT_FAILURE;

} catch (...) {
    std::cerr << "捕获未知异常\n";
    return EXIT_FAILURE;
}
```

### 6.7 常见错误及修正

#### 错误示例 1：错误的返回类型

```cpp
// 错误：使用 void 返回类型
#include <iostream>

void main() {  // 非良构！
    std::cout << "Hello\n";
}
```

**正确做法**：

```cpp
#include <iostream>

int main() {  // 标准形式
    std::cout << "Hello\n";
    return 0;
}
```

#### 错误示例 2：尝试递归调用 main

```cpp
// 错误：尝试递归调用 main
#include <iostream>

int main() {
    static int count = 0;
    if (count++ < 5) {
        std::cout << count << '\n';
        main();  // 错误：main 不能被命名/调用！
    }
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>

void recursive_func(int& count) {
    if (count++ < 5) {
        std::cout << count << '\n';
        recursive_func(count);
    }
}

int main() {
    int count = 0;
    recursive_func(count);
    return 0;
}
```

#### 错误示例 3：尝试取 main 地址

```cpp
// 错误：尝试获取 main 的函数指针
#include <iostream>

int main() {
    auto f = &main;  // 错误：不能取 main 的地址！
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>

void other_func() {
    std::cout << "其他函数\n";
}

int main() {
    auto f = &other_func;  // OK
    f();
    return 0;
}
```

#### 错误示例 4：使用 auto 推导返回类型

```cpp
// 错误：使用 auto 推导返回类型
#include <iostream>

auto main() {  // 错误：main 返回类型不能推导！
    std::cout << "Hello\n";
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>

int main() {  // 显式指定返回类型
    std::cout << "Hello\n";
    return 0;
}
```

#### 错误示例 5：声明为 constexpr

```cpp
// 错误：声明 main 为 constexpr
#include <iostream>

constexpr int main() {  // 错误：main 不能是 constexpr！
    std::cout << "Hello\n";
    return 0;
}
```

**正确做法**：

```cpp
#include <iostream>

int main() {  // 普通 main 函数
    std::cout << "Hello\n";
    return 0;
}
```

## 7. 总结 (Summary)

### 7.1 核心要点

1. **标准签名**：
   - `int main()`
   - `int main(int argc, char* argv[])`
   - 实现定义的其他签名（返回 `int`）

2. **参数含义**：
   - `argc`：参数计数（非负）
   - `argv`：参数向量（以 `nullptr` 结尾）

3. **特殊属性**：
   - 隐式返回 0
   - 返回等价于 `std::exit()`

4. **严格限制**：
   - 不能命名、调用、取地址
   - 不能有语言链接、constexpr、consteval、inline、static
   - 不能推导返回类型
   - 不能是协程或附加到模块

### 7.2 与 C 语言的对比

| 特性 | C++ | C |
|-----|-----|---|
| 无 return 语句 | 始终隐式返回 0 | C99+ 隐式返回 0 |
| 递归调用 | 禁止 | 允许 |
| 取地址 | 禁止 | 允许 |
| 标准签名 | 两种 + 实现定义 | 两种 + 实现定义 |
| 函数重载 | 禁止 | N/A |

### 7.3 返回值最佳实践

| 返回方式 | 推荐 | 原因 |
|---------|-----|------|
| `return 0;` | 推荐 | 明确表示成功 |
| `return EXIT_SUCCESS;` | 推荐 | 可读性好 |
| `return EXIT_FAILURE;` | 推荐 | 可读性好 |
| 无 return 语句 | 不推荐 | 虽然合法，但不明确 |

### 7.4 学习建议

1. **始终使用标准签名**：
   - 无参数时用 `int main()`
   - 需要命令行参数时用 `int main(int argc, char* argv[])`

2. **始终显式返回值**：
   - 即使 C++ 允许省略，显式返回更清晰
   - 使用 `EXIT_SUCCESS` 和 `EXIT_FAILURE` 提高可读性

3. **正确处理参数**：
   - 检查 `argc` 再访问 `argv` 元素
   - 不要依赖 `argv[0]` 的值
   - 考虑使用现代 C++ 的 `std::string_view`

4. **避免非标准用法**：
   - 不使用 `void main`
   - 不尝试递归调用 main
   - 使用普通函数代替 main 递归

---

**参考资源**：
- ISO/IEC 14882:2024 (C++23 标准) 6.9.3.1
- `<cstdlib>` - `EXIT_SUCCESS`, `EXIT_FAILURE`, `std::exit()`, `std::getenv()`
- `<cstring>` - `std::strtok()`
- `<iomanip>` - `std::quoted()`