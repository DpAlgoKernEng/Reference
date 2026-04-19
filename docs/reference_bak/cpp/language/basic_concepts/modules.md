# C++ 模块详解

  [1. 模块基础概念](#1-模块基础概念)
  
  [2. 模块声明和语法](#2-模块声明和语法)
  
  [3. 导出声明和定义](#3-导出声明和定义)
  
  [4. 导入模块和头单元](#4-导入模块和头单元)
  
  [5. 全局模块片段](#5-全局模块片段)
  
  [6. 私有模块片段](#6-私有模块片段)
  
  [7. 模块分区](#7-模块分区)
  
  [8. 模块拥有权](#8-模块拥有权)
  
  [9. 模块与传统头文件对比](#9-模块与传统头文件对比)
  
  [10. 实践示例](#10-实践示例)
  
  [11. 标准参考](#11-标准参考)
  
  [12. 总结](#12-总结)

## 1. 模块基础概念

### 1.1 模块的定义和目的

模块是C++20引入的重要特性，用于在翻译单元之间共享声明和定义，是传统头文件用例的现代化替代方案。

```cpp
#include <iostream>

void modules_basics() {
    std::cout << "Modules Basics:" << std::endl;
    
    // 传统头文件方式
    std::cout << "  === Traditional Header Files ===" << std::endl;
    std::cout << "    - 使用 #include 指令包含头文件" << std::endl;
    std::cout << "    - 预处理器文本替换" << std::endl;
    std::cout << "    - 编译时间较长" << std::endl;
    std::cout << "    - 容易出现重复包含问题" << std::endl;
    
    // 模块方式
    std::cout << "  === C++ Modules ===" << std::endl;
    std::cout << "    - 使用 import 声明导入模块" << std::endl;
    std::cout << "    - 编译器直接处理模块接口" << std::endl;
    std::cout << "    - 编译时间更短" << std::endl;
    std::cout << "    - 更好的封装性" << std::endl;
    std::cout << "    - 避免命名空间污染" << std::endl;
    
    // 模块与命名空间的关系
    std::cout << "  === Modules vs Namespaces ===" << std::endl;
    std::cout << "    - 模块是编译时概念，用于组织代码" << std::endl;
    std::cout << "    - 命名空间是语言概念，用于避免名称冲突" << std::endl;
    std::cout << "    - 两者是正交的概念，可以同时使用" << std::endl;
}
```

### 1.2 模块的优势

模块相比传统头文件具有显著优势：

```cpp
#include <iostream>
#include <chrono>
#include <vector>

void modules_advantages() {
    std::cout << "Modules Advantages:" << std::endl;
    
    // 1. 编译性能
    std::cout << "  === Compilation Performance ===" << std::endl;
    std::cout << "    - 模块接口预先编译，减少重复解析" << std::endl;
    std::cout << "    - 避免宏展开和文本替换开销" << std::endl;
    std::cout << "    - 更快的增量编译" << std::endl;
    
    // 2. 封装性
    std::cout << "  === Encapsulation ===" << std::endl;
    std::cout << "    - 明确控制导出内容" << std::endl;
    std::cout << "    - 隐藏实现细节" << std::endl;
    std::cout << "    - 防止内部实现污染使用者命名空间" << std::endl;
    
    // 3. 安全性
    std::cout << "  === Safety ===" << std::endl;
    std::cout << "    - 避免宏定义冲突" << std::endl;
    std::cout << "    - 防止意外的文本替换" << std::endl;
    std::cout << "    - 更严格的接口控制" << std::endl;
    
    // 4. 维护性
    std::cout << "  === Maintainability ===" << std::endl;
    std::cout << "    - 清晰的依赖关系" << std::endl;
    std::cout << "    - 更好的代码组织" << std::endl;
    std::cout << "    - 简化的构建系统" << std::endl;
}
```

## 2. 模块声明和语法

### 2.1 模块声明语法详解

模块声明是模块单元的第一个声明，定义了模块的名称和类型：

```cpp
#include <iostream>

void module_declaration_syntax() {
    std::cout << "Module Declaration Syntax:" << std::endl;
    
    // 基本模块声明
    std::cout << "  === Basic Module Declaration ===" << std::endl;
    std::cout << "    export module module_name;" << std::endl;
    std::cout << "    - export: 可选，表示接口单元" << std::endl;
    std::cout << "    - module: 关键字" << std::endl;
    std::cout << "    - module_name: 模块名称" << std::endl;
    
    // 模块名称层次结构
    std::cout << "  === Module Name Hierarchy ===" << std::endl;
    std::cout << "    export module mymodule;" << std::endl;
    std::cout << "    export module mymodule.submodule;" << std::endl;
    std::cout << "    export module mymodule.submodule.component;" << std::endl;
    std::cout << "    - 点号表示逻辑层次结构" << std::endl;
    std::cout << "    - 不同于命名空间，仅用于组织" << std::endl;
    
    // 模块单元类型
    std::cout << "  === Module Unit Types ===" << std::endl;
    
    // 主模块接口单元
    std::cout << "    Primary Module Interface Unit:" << std::endl;
    std::cout << "    export module A;  // 每个命名模块只能有一个" << std::endl;
    
    // 模块实现单元
    std::cout << "    Module Implementation Unit:" << std::endl;
    std::cout << "    module A;         // 实现单元，不导出接口" << std::endl;
    
    // 模块分区接口单元
    std::cout << "    Module Partition Interface Unit:" << std::endl;
    std::cout << "    export module A:B; // 分区接口单元" << std::endl;
    
    // 模块分区实现单元
    std::cout << "    Module Partition Implementation Unit:" << std::endl;
    std::cout << "    module A:B;        // 分区实现单元" << std::endl;
}
```

### 2.2 模块声明示例

```cpp
// math_module.cpp - 主模块接口单元
export module math;

// 导入标准库模块
import <iostream>;
import <cmath>;

// 导出函数声明
export namespace math {
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
    
    // 内联函数可以直接定义在接口中
    inline double square(double x) {
        return x * x;
    }
}

// math_impl.cpp - 模块实现单元
module math;

// 实现函数（不导出）
namespace math {
    double add(double a, double b) {
        return a + b;
    }
    
    double subtract(double a, double b) {
        return a - b;
    }
    
    double multiply(double a, double b) {
        return a * b;
    }
    
    double divide(double a, double b) {
        if (b == 0.0) {
            throw std::invalid_argument("Division by zero");
        }
        return a / b;
    }
}

// main.cpp - 使用模块
import math;
import <iostream>;

int main() {
    using namespace math;
    
    double result = add(5.0, 3.0);
    std::cout << "5 + 3 = " << result << std::endl;
    
    result = square(4.0);
    std::cout << "4^2 = " << result << std::endl;
    
    return 0;
}
```

## 3. 导出声明和定义

### 3.1 导出声明语法

导出声明控制模块中哪些内容对外可见：

```cpp
#include <iostream>

void export_declaration_syntax() {
    std::cout << "Export Declaration Syntax:" << std::endl;
    
    // 单个导出声明
    std::cout << "  === Single Export Declaration ===" << std::endl;
    std::cout << "    export void function();" << std::endl;
    std::cout << "    export int variable = 42;" << std::endl;
    std::cout << "    export class MyClass { ... };" << std::endl;
    
    // 导出块
    std::cout << "  === Export Block ===" << std::endl;
    std::cout << "    export {" << std::endl;
    std::cout << "        void func1();" << std::endl;
    std::cout << "        void func2();" << std::endl;
    std::cout << "        int var1, var2;" << std::endl;
    std::cout << "    }" << std::endl;
    
    // 导出命名空间
    std::cout << "  === Export Namespace ===" << std::endl;
    std::cout << "    export namespace ns {" << std::endl;
    std::cout << "        void function();" << std::endl;
    std::cout << "        class Class { ... };" << std::endl;
    std::cout << "    }" << std::endl;
}
```

### 3.2 导出示例

```cpp
// geometry.cpp - 几何模块接口
export module geometry;

import <iostream>;
import <vector>;

// 导出单个声明
export const double PI = 3.14159265359;

// 导出函数
export double circle_area(double radius);
export double rectangle_area(double width, double height);

// 导出类
export class Point {
public:
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    
    // 内联函数可以直接定义
    double distance_to(const Point& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
};

// 导出块 - 批量导出相关声明
export {
    class Shape {
    public:
        virtual ~Shape() = default;
        virtual double area() const = 0;
        virtual void print() const = 0;
    };
    
    class Circle : public Shape {
        Point center;
        double radius;
    public:
        Circle(const Point& c, double r) : center(c), radius(r) {}
        double area() const override;
        void print() const override;
    };
    
    class Rectangle : public Shape {
        Point topLeft, bottomRight;
    public:
        Rectangle(const Point& tl, const Point& br) : topLeft(tl), bottomRight(br) {}
        double area() const override;
        void print() const override;
    };
}

// 导出命名空间
export namespace utils {
    template<typename T>
    void print_vector(const std::vector<T>& vec) {
        for (const auto& item : vec) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}

// 模块实现部分（不导出）
module geometry;

double circle_area(double radius) {
    return PI * radius * radius;
}

double rectangle_area(double width, double height) {
    return width * height;
}

double Circle::area() const {
    return circle_area(radius);
}

void Circle::print() const {
    std::cout << "Circle: center(" << center.x << ", " << center.y 
              << "), radius=" << radius << std::endl;
}

double Rectangle::area() const {
    double width = abs(bottomRight.x - topLeft.x);
    double height = abs(bottomRight.y - topLeft.y);
    return rectangle_area(width, height);
}

void Rectangle::print() const {
    std::cout << "Rectangle: topLeft(" << topLeft.x << ", " << topLeft.y
              << "), bottomRight(" << bottomRight.x << ", " << bottomRight.y << ")" << std::endl;
}
```

## 4. 导入模块和头单元

### 4.1 导入声明语法

导入声明用于在翻译单元中使用模块提供的内容：

```cpp
#include <iostream>

void import_declaration_syntax() {
    std::cout << "Import Declaration Syntax:" << std::endl;
    
    // 导入命名模块
    std::cout << "  === Import Named Module ===" << std::endl;
    std::cout << "    import module_name;" << std::endl;
    std::cout << "    import module_name.submodule;" << std::endl;
    
    // 导出导入（传递导出）
    std::cout << "  === Export Import (Re-export) ===" << std::endl;
    std::cout << "    export import module_name;" << std::endl;
    std::cout << "    - 导入的同时重新导出" << std::endl;
    std::cout << "    - 使导入者也能使用被导入模块的内容" << std::endl;
    
    // 导入模块分区
    std::cout << "  === Import Module Partition ===" << std::endl;
    std::cout << "    import :partition_name;" << std::endl;
    std::cout << "    - 只能在同一模块内部使用" << std::endl;
    
    // 导入头单元
    std::cout << "  === Import Header Unit ===" << std::endl;
    std::cout << "    import <header_name>;" << std::endl;
    std::cout << "    import \"header_name\";" << std::endl;
}
```

### 4.2 导入示例

```cpp
// graphics.cpp - 图形模块
export module graphics;

// 导入标准库头单元
import <iostream>;
import <string>;
import <vector>;

// 导入自定义模块
import geometry;

// 导出图形相关功能
export class Canvas {
private:
    std::vector<std::unique_ptr<Shape>> shapes;
    
public:
    void add_shape(std::unique_ptr<Shape> shape) {
        shapes.push_back(std::move(shape));
    }
    
    void draw_all() const {
        for (const auto& shape : shapes) {
            shape->print();
            std::cout << "Area: " << shape->area() << std::endl;
        }
    }
    
    double total_area() const {
        double total = 0;
        for (const auto& shape : shapes) {
            total += shape->area();
        }
        return total;
    }
};

// main.cpp - 使用多个模块
import graphics;
import geometry;
import <iostream>;
import <memory>;

int main() {
    Canvas canvas;
    
    // 使用从geometry模块导入的类
    canvas.add_shape(std::make_unique<Circle>(Point(0, 0), 5.0));
    canvas.add_shape(std::make_unique<Rectangle>(Point(0, 0), Point(4, 3)));
    
    canvas.draw_all();
    
    std::cout << "Total area: " << canvas.total_area() << std::endl;
    
    // 使用从geometry模块导入的工具函数
    std::vector<Point> points = {Point(0, 0), Point(1, 1), Point(2, 2)};
    utils::print_vector(points);
    
    return 0;
}
```

## 5. 全局模块片段

### 5.1 全局模块片段语法

全局模块片段用于在模块单元中包含传统头文件：

```cpp
#include <iostream>

void global_module_fragment_syntax() {
    std::cout << "Global Module Fragment Syntax:" << std::endl;
    
    std::cout << "  === Global Module Fragment ===" << std::endl;
    std::cout << "    module;" << std::endl;
    std::cout << "    // 预处理指令" << std::endl;
    std::cout << "    #include <header.h>" << std::endl;
    std::cout << "    #define MACRO value" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    export module module_name;" << std::endl;
    std::cout << "    // 模块内容" << std::endl;
    
    std::cout << "  === Purpose ===" << std::endl;
    std::cout << "    - 兼容需要预处理器宏配置的旧头文件" << std::endl;
    std::cout << "    - 在模块环境中使用传统C库" << std::endl;
    std::cout << "    - 处理平台特定的头文件" << std::endl;
}
```

### 5.2 全局模块片段示例

```cpp
// system_utils.cpp - 系统工具模块
module;

// 在全局模块片段中定义宏
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <sys/time.h>
#include <ctime>

export module system_utils;

import <iostream>;
import <string>;

// 导出系统相关功能
export namespace system_utils {
    // 获取当前时间戳
    inline std::time_t get_current_timestamp() {
        return std::time(nullptr);
    }
    
    // 获取进程ID
    inline int get_process_id() {
        return getpid();  // 来自 <unistd.h>
    }
    
    // 获取系统时间（微秒）
    long long get_system_time_microseconds() {
        struct timeval tv;
        gettimeofday(&tv, nullptr);  // 来自 <sys/time.h>
        return tv.tv_sec * 1000000LL + tv.tv_usec;
    }
    
    // 格式化时间
    std::string format_time(std::time_t time) {
        char buffer[100];
        std::tm* tm_info = std::localtime(&time);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
        return std::string(buffer);
    }
}

// main.cpp - 使用系统工具模块
import system_utils;
import <iostream>;

int main() {
    using namespace system_utils;
    
    std::cout << "Process ID: " << get_process_id() << std::endl;
    std::cout << "Current time: " << format_time(get_current_timestamp()) << std::endl;
    std::cout << "System time (microseconds): " << get_system_time_microseconds() << std::endl;
    
    return 0;
}
```

## 6. 私有模块片段

### 6.1 私有模块片段语法

私有模块片段用于将模块实现放在主接口单元中：

```cpp
#include <iostream>

void private_module_fragment_syntax() {
    std::cout << "Private Module Fragment Syntax:" << std::endl;
    
    std::cout << "  === Private Module Fragment ===" << std::endl;
    std::cout << "    export module module_name;" << std::endl;
    std::cout << "    // 导出声明" << std::endl;
    std::cout << "    export void public_function();" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    module : private;" << std::endl;
    std::cout << "    // 私有实现" << std::endl;
    std::cout << "    void public_function() {" << std::endl;
    std::cout << "        // 实现细节" << std::endl;
    std::cout << "    }" << std::endl;
    std::cout << "    " << std::endl;
    std::cout << "    static void helper_function() {" << std::endl;
    std::cout << "        // 私有辅助函数" << std::endl;
    std::cout << "    }" << std::endl;
    
    std::cout << "  === Purpose ===" << std::endl;
    std::cout << "    - 将模块实现放在单个文件中" << std::endl;
    std::cout << "    - 避免创建额外的实现单元文件" << std::endl;
    std::cout << "    - 保持接口和实现的紧密关联" << std::endl;
}
```

### 6.2 私有模块片段示例

```cpp
// string_utils.cpp - 字符串工具模块
export module string_utils;

import <string>;
import <vector>;
import <algorithm>;

// 导出声明
export std::string to_upper(const std::string& str);
export std::string to_lower(const std::string& str);
export std::vector<std::string> split(const std::string& str, char delimiter);
export bool starts_with(const std::string& str, const std::string& prefix);
export std::string trim(const std::string& str);

////////////////////////////////////////////////////////////
// 私有模块片段开始
module : private;

// 私有辅助函数
static bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static std::string& string_to_upper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

static std::string& string_to_lower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

////////////////////////////////////////////////////////////
// 公共函数实现
std::string to_upper(const std::string& str) {
    std::string result = str;
    return string_to_upper(result);
}

std::string to_lower(const std::string& str) {
    std::string result = str;
    return string_to_lower(result);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    
    for (char c : str) {
        if (c == delimiter) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    
    tokens.push_back(token);  // 添加最后一个token
    return tokens;
}

bool starts_with(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) {
        return false;
    }
    return str.substr(0, prefix.length()) == prefix;
}

std::string trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), is_whitespace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), is_whitespace).base();
    return (start < end) ? std::string(start, end) : std::string();
}

// main.cpp - 使用字符串工具模块
import string_utils;
import <iostream>;
import <vector>;

int main() {
    using namespace string_utils;
    
    std::string text = "  Hello, World!  ";
    std::cout << "Original: '" << text << "'" << std::endl;
    std::cout << "Upper: '" << to_upper(text) << "'" << std::endl;
    std::cout << "Lower: '" << to_lower(text) << "'" << std::endl;
    std::cout << "Trimmed: '" << trim(text) << "'" << std::endl;
    
    std::string csv = "apple,banana,cherry,date";
    auto fruits = split(csv, ',');
    std::cout << "Split result: ";
    for (const auto& fruit : fruits) {
        std::cout << "'" << fruit << "' ";
    }
    std::cout << std::endl;
    
    std::cout << "Starts with 'Hello': " << std::boolalpha 
              << starts_with("Hello, World!", "Hello") << std::endl;
    
    return 0;
}
```

## 7. 模块分区

### 7.1 模块分区语法

模块分区用于将大型模块分解为更小的逻辑单元：

```cpp
#include <iostream>

void module_partition_syntax() {
    std::cout << "Module Partition Syntax:" << std::endl;
    
    // 模块分区声明
    std::cout << "  === Module Partition Declaration ===" << std::endl;
    std::cout << "    export module module_name:partition_name;" << std::endl;
    std::cout << "    module module_name:partition_name;" << std::endl;
    
    // 导入模块分区
    std::cout << "  === Import Module Partition ===" << std::endl;
    std::cout << "    import :partition_name;" << std::endl;
    std::cout << "    export import :partition_name;" << std::endl;
    
    // 分区类型
    std::cout << "  === Partition Types ===" << std::endl;
    std::cout << "    - 接口分区: export module A:B;" << std::endl;
    std::cout << "    - 实现分区: module A:B;" << std::endl;
    
    std::cout << "  === Benefits ===" << std::endl;
    std::cout << "    - 模块化大型项目" << std::endl;
    std::cout << "    - 更好的代码组织" << std::endl;
    std::cout << "    - 独立编译分区" << std::endl;
    std::cout << "    - 控制接口可见性" << std::endl;
}
```

### 7.2 模块分区示例

```cpp
// network_core.cpp - 网络模块核心分区（接口）
export module network:core;

import <string>;
import <memory>;

// 核心网络类型和接口
export namespace network {
    class Socket {
    protected:
        int socket_fd;
        
    public:
        Socket();
        virtual ~Socket();
        virtual bool connect(const std::string& host, int port) = 0;
        virtual void close() = 0;
    };
    
    class Endpoint {
        std::string host;
        int port;
        
    public:
        Endpoint(const std::string& h, int p) : host(h), port(p) {}
        const std::string& get_host() const { return host; }
        int get_port() const { return port; }
    };
}

// network_tcp.cpp - TCP网络分区（实现）
module network:tcp;

import :core;
import <iostream>;
import <sys/socket.h>
import <netinet/in.h>
import <arpa/inet.h>
import <unistd.h>

namespace network {
    class TCPSocket : public Socket {
    public:
        bool connect(const std::string& host, int port) override;
        void close() override;
    };
    
    bool TCPSocket::connect(const std::string& host, int port) {
        // TCP连接实现
        std::cout << "Connecting to " << host << ":" << port << " via TCP" << std::endl;
        return true;
    }
    
    void TCPSocket::close() {
        if (socket_fd >= 0) {
            ::close(socket_fd);
            socket_fd = -1;
        }
    }
}

// network_udp.cpp - UDP网络分区（实现）
module network:udp;

import :core;
import <iostream>;

namespace network {
    class UDPSocket : public Socket {
    public:
        bool connect(const std::string& host, int port) override;
        void close() override;
    };
    
    bool UDPSocket::connect(const std::string& host, int port) {
        // UDP连接实现
        std::cout << "Connecting to " << host << ":" << port << " via UDP" << std::endl;
        return true;
    }
    
    void UDPSocket::close() {
        if (socket_fd >= 0) {
            ::close(socket_fd);
            socket_fd = -1;
        }
    }
}

// network.cpp - 主网络模块接口
export module network;

// 导入并重新导出核心接口
export import :core;

// 导入实现分区（不重新导出）
import :tcp;
import :udp;

import <memory>;

// 导出工厂函数
export namespace network {
    std::unique_ptr<Socket> create_tcp_socket();
    std::unique_ptr<Socket> create_udp_socket();
}

// 私有模块片段 - 实现工厂函数
module : private;

std::unique_ptr<Socket> network::create_tcp_socket() {
    return std::make_unique<TCPSocket>();
}

std::unique_ptr<Socket> network::create_udp_socket() {
    return std::make_unique<UDPSocket>();
}

// main.cpp - 使用网络模块
import network;
import <iostream>;
import <memory>;

int main() {
    using namespace network;
    
    // 使用工厂函数创建不同类型的套接字
    auto tcp_socket = create_tcp_socket();
    auto udp_socket = create_udp_socket();
    
    Endpoint server("localhost", 8080);
    
    tcp_socket->connect(server.get_host(), server.get_port());
    udp_socket->connect(server.get_host(), server.get_port());
    
    return 0;
}
```

## 8. 模块拥有权

### 8.1 模块拥有权概念

模块拥有权定义了声明和定义的归属关系：

```cpp
#include <iostream>

void module_ownership_concepts() {
    std::cout << "Module Ownership Concepts:" << std::endl;
    
    // 实体归属
    std::cout << "  === Entity Ownership ===" << std::endl;
    std::cout << "    - 声明附加于模块后，实体归属于该模块" << std::endl;
    std::cout << "    - 归属实体只能在所属模块中定义" << std::endl;
    std::cout << "    - 不同模块中的同名实体是不同实体" << std::endl;
    
    // 模块作用域
    std::cout << "  === Module Scope ===" << std::endl;
    std::cout << "    - 未导出的声明具有模块作用域" << std::endl;
    std::cout << "    - 模块作用域名称在模块外部不可见" << std::endl;
    
    // 全局模块实体
    std::cout << "  === Global Module Entities ===" << std::endl;
    std::cout << "    - 某些声明不附加于任何命名模块" << std::endl;
    std::cout << "    - 可以在模块外部定义" << std::endl;
}
```

### 8.2 模块拥有权示例

```cpp
// math_lib.cpp - 数学库模块
export module math_lib;

// 导出的函数声明（附加于math_lib模块）
export int calculate(int a, int b);

// 未导出的辅助函数（具有模块作用域）
int helper_function(int x);

// 私有模块片段 - 实现
module : private;

// 模块内定义
int calculate(int a, int b) {
    return helper_function(a) + helper_function(b);
}

int helper_function(int x) {
    return x * 2;  // 简单实现
}

// graphics_lib.cpp - 图形库模块
export module graphics_lib;

// 导入数学库
import math_lib;

// 导出函数
export void render(int value);

// 私有模块片段
module : private;

void render(int value) {
    // 可以使用从math_lib导入的函数
    int processed = calculate(value, 10);
    // 渲染实现...
}

// main.cpp - 主程序
// 注意：这里不能定义math_lib或graphics_lib中的函数
// 因为它们已经归属于相应的模块

import math_lib;
import graphics_lib;
import <iostream>;

int main() {
    int result = calculate(5, 3);
    std::cout << "Calculation result: " << result << std::endl;
    
    render(result);
    
    return 0;
}
```

## 9. 模块与传统头文件对比

### 9.1 性能对比

```cpp
#include <iostream>
#include <chrono>

void performance_comparison() {
    std::cout << "Performance Comparison:" << std::endl;
    
    // 传统头文件方式
    std::cout << "  === Traditional Header Files ===" << std::endl;
    std::cout << "    编译过程:" << std::endl;
    std::cout << "    1. 预处理器展开所有#include" << std::endl;
    std::cout << "    2. 文本替换所有宏定义" << std::endl;
    std::cout << "    3. 解析重复的声明" << std::endl;
    std::cout << "    4. 生成目标文件" << std::endl;
    std::cout << "    特点: 编译时间长，重复工作多" << std::endl;
    
    // 模块方式
    std::cout << "  === C++ Modules ===" << std::endl;
    std::cout << "    编译过程:" << std::endl;
    std::cout << "    1. 预编译模块接口" << std::endl;
    std::cout << "    2. 直接导入预编译接口" << std::endl;
    std::cout << "    3. 避免重复解析" << std::endl;
    std::cout << "    4. 生成目标文件" << std::endl;
    std::cout << "    特点: 编译时间短，无重复工作" << std::endl;
    
    // 实际性能测试示例
    std::cout << "  === Performance Test Example ===" << std::endl;
    std::cout << "    // 传统方式编译时间: ~2.5秒" << std::endl;
    std::cout << "    // 模块方式编译时间: ~0.8秒" << std::endl;
    std::cout << "    // 性能提升: ~68%" << std::endl;
}
```

### 9.2 使用对比示例

```cpp
// 传统头文件方式
// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#ifdef USE_DOUBLE_PRECISION
typedef double real_t;
#else
typedef float real_t;
#endif

namespace math_utils {
    real_t add(real_t a, real_t b);
    real_t multiply(real_t a, real_t b);
    
    inline real_t square(real_t x) {
        return x * x;
    }
}

#endif

// math_utils.cpp
#include "math_utils.h"

namespace math_utils {
    real_t add(real_t a, real_t b) {
        return a + b;
    }
    
    real_t multiply(real_t a, real_t b) {
        return a * b;
    }
}

// main_traditional.cpp
#include "math_utils.h"
#include <iostream>

int main() {
    using namespace math_utils;
    real_t result = add(5.0, 3.0);
    std::cout << "Result: " << result << std::endl;
    return 0;
}

// 模块方式
// math_utils.cpp (模块)
export module math_utils;

module;

// 全局模块片段中定义宏
#define USE_DOUBLE_PRECISION
#ifdef USE_DOUBLE_PRECISION
using real_t = double;
#else
using real_t = float;
#endif

export module math_utils;

import <iostream>;

export namespace math_utils {
    real_t add(real_t a, real_t b);
    real_t multiply(real_t a, real_t b);
    
    inline real_t square(real_t x) {
        return x * x;
    }
}

module : private;

real_t math_utils::add(real_t a, real_t b) {
    return a + b;
}

real_t math_utils::multiply(real_t a, real_t b) {
    return a * b;
}

// main_modules.cpp
import math_utils;
import <iostream>;

int main() {
    using namespace math_utils;
    real_t result = add(5.0, 3.0);
    std::cout << "Result: " << result << std::endl;
    return 0;
}
```

## 10. 实践示例

### 10.1 完整的模块项目示例

```cpp
// logger_interface.cpp - 日志模块接口分区
export module logger:interface;

import <string>;
import <iostream>;

export namespace logger {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    
    class Logger {
    private:
        LogLevel level;
        std::string name;
        
    public:
        Logger(const std::string& logger_name, LogLevel lvl = LogLevel::INFO);
        
        void set_level(LogLevel lvl);
        LogLevel get_level() const;
        
        void debug(const std::string& message);
        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);
        
    private:
        void log(LogLevel msg_level, const std::string& message);
        std::string level_to_string(LogLevel level);
    };
}

// logger_file.cpp - 文件日志实现分区
module logger:file;

import :interface;
import <fstream>;
import <iomanip>
import <chrono>

namespace logger {
    class FileLogger : public Logger {
    private:
        std::string filename;
        std::ofstream file;
        
    public:
        FileLogger(const std::string& name, const std::string& file_name);
        ~FileLogger();
        
        void log_to_file(LogLevel level, const std::string& message);
    };
}

// logger_console.cpp - 控制台日志实现分区
module logger:console;

import :interface;
import <iostream>
import <iomanip>
import <chrono>

namespace logger {
    class ConsoleLogger : public Logger {
    public:
        ConsoleLogger(const std::string& name);
        
        void log_to_console(LogLevel level, const std::string& message);
    };
}

// logger.cpp - 主日志模块
export module logger;

// 导出接口
export import :interface;

// 导入实现（不导出）
import :file;
import :console;

import <memory>;

// 导出工厂函数
export namespace logger {
    std::unique_ptr<Logger> create_console_logger(const std::string& name);
    std::unique_ptr<Logger> create_file_logger(const std::string& name, const std::string& filename);
}

// 私有模块片段 - 实现
module : private;

using namespace std::chrono;

// Logger实现
logger::Logger::Logger(const std::string& logger_name, LogLevel lvl)
    : level(lvl), name(logger_name) {}

void logger::Logger::set_level(LogLevel lvl) {
    level = lvl;
}

logger::LogLevel logger::Logger::get_level() const {
    return level;
}

void logger::Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void logger::Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void logger::Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void logger::Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void logger::Logger::log(LogLevel msg_level, const std::string& message) {
    if (msg_level >= level) {
        auto now = system_clock::now();
        auto time_t = system_clock::to_time_t(now);
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        
        std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        std::cout << "." << std::setfill('0') << std::setw(3) << ms.count() << "]";
        std::cout << " [" << level_to_string(msg_level) << "]";
        std::cout << " [" << name << "] " << message << std::endl;
    }
}

std::string logger::Logger::level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

// 工厂函数实现
std::unique_ptr<logger::Logger> logger::create_console_logger(const std::string& name) {
    return std::make_unique<Logger>(name);
}

std::unique_ptr<logger::Logger> logger::create_file_logger(const std::string& name, const std::string& filename) {
    // 简化实现，实际应该返回FileLogger实例
    return std::make_unique<Logger>(name);
}

// main.cpp - 使用日志模块
import logger;
import <iostream>;
import <memory>;

int main() {
    // 创建控制台日志记录器
    auto console_logger = logger::create_console_logger("MainApp");
    
    console_logger->set_level(logger::LogLevel::DEBUG);
    
    console_logger->debug("This is a debug message");
    console_logger->info("Application started");
    console_logger->warning("This is a warning");
    console_logger->error("An error occurred");
    
    // 创建文件日志记录器
    auto file_logger = logger::create_file_logger("FileApp", "app.log");
    file_logger->info("Logging to file");
    
    return 0;
}
```

## 11. 标准参考

C++ 模块的相关标准条款：

- C++20 标准（ISO/IEC 14882:2020）：
  - 10.1 模块 [module]
  - 10.2 模块单元 [module.unit]
  - 10.3 模块声明 [module decl]
  - 10.4 导入声明 [module.import]
  - 10.5 导出声明 [module.export]
  - 10.6 全局模块片段 [module.global]
  - 10.7 私有模块片段 [module.private]
  - 10.8 模块分区 [module.partition]
  - 10.9 模块拥有权 [module.owner]

特性测试宏：

- `__cpp_modules`: 201907L (C++20) - 模块核心语言支持
- `__cpp_lib_modules`: 202207L (C++23) - 标准库模块 std 和 std.compat

相关的缺陷报告：

- CWG 2732: 澄清可导入标头在导入时是否能响应预处理器状态
- P3034R1: 禁止模块名称和模块分区包含定义为对象样式的宏的标识符

## 12. 总结

C++ 模块是现代C++的重要特性，为代码组织和编译性能带来了显著改进。

### 12.1 核心要点

- 模块基础：模块是头文件的现代化替代，提供更好的封装和性能
- 模块声明：使用 `export module` 声明模块接口，`module` 声明模块实现
- 导出控制：通过 `export` 关键字精确控制模块接口的可见性
- 导入机制：使用 `import` 声明使用模块提供的功能
- 模块分区：将大型模块分解为更小的逻辑单元
- 模块拥有权：定义声明和定义的归属关系

### 12.2 最佳实践

- 渐进采用：从新项目或模块开始使用模块
- 合理分区：根据功能逻辑合理划分模块分区
- 明确接口：只导出必要的接口，隐藏实现细节
- 兼容处理：使用全局模块片段处理传统头文件
- 性能优化：利用模块提升编译性能

### 12.3 未来发展趋势

- 工具支持：编译器和IDE对模块的支持将持续完善
- 标准库模块：标准库将逐步提供模块化版本
- 性能优化：模块系统的性能将进一步提升
- 生态系统：第三方库将广泛采用模块
- 语言集成：模块将与C++其他特性更深度集成

通过深入理解和正确应用C++模块，开发者可以构建更高效、更易维护的现代化C++应用程序。
