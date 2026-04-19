# C++输入/输出库（Input/Output Library）

来源：cppreference.com

## I/O库概述

C++包含以下输入/输出库：
1. 面向对象的基于流的I/O库
2. 基于打印的函数族（C++23起）
3. 标准C风格I/O函数集

## 基于流的I/O（Stream-based I/O）

基于流的输入/输出库围绕抽象输入/输出设备组织。这些抽象设备允许相同的代码处理文件、内存流或执行任意操作（如即时压缩）的自定义适配器设备的输入/输出。

大多数类都是模板化的，因此可以适应任何基本字符类型。为最常见的基本字符类型（char和wchar_t）提供了单独的typedef。类按以下层次结构组织：

### 继承层次结构

```
ios_base (格式化标志和异常管理)
    ↓
basic_ios (管理任意流缓冲区)
    ↓
basic_istream (输入接口)  basic_ostream (输出接口)
    ↓                         ↓
basic_iostream (输入/输出接口)
```

### 抽象类

| 组件 | 说明 |
|------|------|
| `ios_base` | 管理格式化标志和输入/输出异常（类） |
| `basic_ios` | 管理任意流缓冲区（类模板） |
| `basic_streambuf` | 抽象化原始设备（类模板） |
| `basic_istream` | 包装给定抽象设备并提供高级输入接口（类模板） |
| `basic_ostream` | 包装给定抽象设备并提供高级输出接口（类模板） |
| `basic_iostream` | 包装给定抽象设备并提供高级输入/输出接口（类模板） |

### 文件I/O实现

| 组件 | 说明 |
|------|------|
| `basic_filebuf` | 实现原始文件设备（类模板） |
| `basic_ifstream` | 实现高级文件流输入操作（类模板） |
| `basic_ofstream` | 实现高级文件流输出操作（类模板） |
| `basic_fstream` | 实现高级文件流输入/输出操作（类模板） |

### 字符串I/O实现

| 组件 | 说明 |
|------|------|
| `basic_stringbuf` | 实现原始字符串设备（类模板） |
| `basic_istringstream` | 实现高级字符串流输入操作（类模板） |
| `basic_ostringstream` | 实现高级字符串流输出操作（类模板） |
| `basic_stringstream` | 实现高级字符串流输入/输出操作（类模板） |

### 数组I/O实现

| 组件 | 说明 |
|------|------|
| `basic_spanbuf`(C++23) | 实现原始固定字符缓冲区设备（类模板） |
| `basic_ispanstream`(C++23) | 实现固定字符缓冲区输入操作（类模板） |
| `basic_ospanstream`(C++23) | 实现固定字符缓冲区输出操作（类模板） |
| `basic_spanstream`(C++23) | 实现固定字符缓冲区输入/输出操作（类模板） |

### 同步输出（C++20起）

| 组件 | 说明 |
|------|------|
| `basic_syncbuf`(C++20) | 同步输出设备包装器（类模板） |
| `basic_osyncstream`(C++20) | 同步输出流包装器（类模板） |

## 常用类型定义（Typedefs）

### 标准字符类型的typedef

| 类型 | 定义 |
|------|------|
| `std::ios` | `std::basic_ios<char>` |
| `std::wios` | `std::basic_ios<wchar_t>` |
| `std::istream` | `std::basic_istream<char>` |
| `std::wistream` | `std::basic_istream<wchar_t>` |
| `std::ostream` | `std::basic_ostream<char>` |
| `std::wostream` | `std::basic_ostream<wchar_t>` |
| `std::iostream` | `std::basic_iostream<char>` |
| `std::wiostream` | `std::basic_iostream<wchar_t>` |

### 文件流typedef

| 类型 | 定义 |
|------|------|
| `std::ifstream` | `std::basic_ifstream<char>` |
| `std::wifstream` | `std::basic_ifstream<wchar_t>` |
| `std::ofstream` | `std::basic_ofstream<char>` |
| `std::wofstream` | `std::basic_ofstream<wchar_t>` |
| `std::fstream` | `std::basic_fstream<char>` |
| `std::wfstream` | `std::basic_fstream<wchar_t>` |

### 字符串流typedef

| 类型 | 定义 |
|------|------|
| `std::istringstream` | `std::basic_istringstream<char>` |
| `std::wistringstream` | `std::basic_istringstream<wchar_t>` |
| `std::ostringstream` | `std::basic_ostringstream<char>` |
| `std::wostringstream` | `std::basic_ostringstream<wchar_t>` |
| `std::stringstream` | `std::basic_stringstream<char>` |
| `std::wstringstream` | `std::basic_stringstream<wchar_t>` |

### C++23新特性typedef

| 类型 | 定义 |
|------|------|
| `std::spanstream`(C++23) | `std::basic_spanstream<char>` |
| `std::wspanstream`(C++23) | `std::basic_spanstream<wchar_t>` |
| `std::ispanstream`(C++23) | `std::basic_ispanstream<char>` |
| `std::wispanstream`(C++23) | `std::basic_ispanstream<wchar_t>` |

### 同步流typedef（C++20）

| 类型 | 定义 |
|------|------|
| `std::osyncstream`(C++20) | `std::basic_osyncstream<char>` |
| `std::wosyncstream`(C++20) | `std::basic_osyncstream<wchar_t>` |

## 预定义标准流对象

| 对象 | 说明 |
|------|------|
| `cin`/`wcin` | 从标准C输入流stdin读取（全局对象） |
| `cout`/`wcout` | 写入标准C输出流stdout（全局对象） |
| `cerr`/`wcerr` | 写入标准C错误流stderr，无缓冲（全局对象） |
| `clog`/`wclog` | 写入标准C错误流stderr（全局对象） |

## I/O操纵符（I/O Manipulators）

基于流的I/O库使用I/O操纵符（如std::boolalpha、std::hex等）来控制流的行为。

## 辅助类型

### 位置和大小类型

| 类型 | 说明 |
|------|------|
| `streamoff` | 表示相对文件/流位置（从fpos偏移），足以表示任何文件大小（typedef） |
| `streamsize` | 表示I/O操作中传输的字符数或I/O缓冲区大小（typedef） |
| `fpos` | 表示流或文件中的绝对位置（类模板） |

### 位置类型定义

| 类型 | 定义 |
|------|------|
| `std::streampos` | `std::fpos<std::char_traits<char>::state_type>` |
| `std::wstreampos` | `std::fpos<std::char_traits<wchar_t>::state_type>` |
| `std::u8streampos`(C++20) | `std::fpos<std::char_traits<char8_t>::state_type>` |
| `std::u16streampos`(C++11) | `std::fpos<std::char_traits<char16_t>::state_type>` |
| `std::u32streampos`(C++11) | `std::fpos<std::char_traits<char32_t>::state_type>` |

## 错误分类接口（C++11起）

| 组件 | 说明 |
|------|------|
| `io_errc`(C++11) | IO流错误代码（枚举） |
| `iostream_category`(C++11) | 标识iostream错误分类（函数） |

## 打印函数（C++23起）

Unicode感知的打印族函数，对已格式化的文本执行格式化I/O。它们带来了std::format的所有性能优势，默认情况下与区域设置无关，减少全局状态，避免分配临时std::string对象和调用operator<<，总体上使格式化比iostreams和stdio更高效。

### 提供的打印函数

| 函数 | 说明 |
|------|------|
| `print`(C++23) | 使用参数的格式化表示打印到stdout或文件流（函数模板） |
| `println`(C++23) | 与std::print相同，但每个打印以额外换行符结束（函数模板） |
| `vprint_unicode`/`vprint_unicode_buffered`(C++23) | 使用类型擦除的参数表示打印到Unicode兼容的stdout或文件流（函数） |
| `print(std::ostream)`(C++23) | 输出参数的格式化表示（函数模板） |
| `println(std::ostream)`(C++23) | 输出参数的格式化表示并附加'\n'（函数模板） |

---

## 扩展知识详解

### 一、流类层次结构深入

#### 1. 基础流操作
```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

void basic_stream_operations() {
    // 标准输入输出
    std::cout << "Hello, World!" << std::endl;
    
    // 格式化输出
    int value = 42;
    double pi = 3.14159;
    std::cout << std::hex << value << std::endl;  // 十六进制
    std::cout << std::fixed << std::setprecision(2) << pi << std::endl;
    
    // 字符串流
    std::ostringstream oss;
    oss << "Value: " << value << ", Pi: " << pi;
    std::string result = oss.str();
    std::cout << "String stream result: " << result << std::endl;
    
    // 文件流
    std::ofstream file("test.txt");
    if (file.is_open()) {
        file << "This is a test file.\n";
        file << "Value: " << value << std::endl;
        file.close();
    }
    
    // 读取文件
    std::ifstream input("test.txt");
    std::string line;
    while (std::getline(input, line)) {
        std::cout << "File line: " << line << std::endl;
    }
}
```

#### 2. 自定义流缓冲区
```cpp
#include <iostream>
#include <streambuf>
#include <vector>

class VectorBuffer : public std::streambuf {
private:
    std::vector<char> buffer_;
    size_t pos_;
    
public:
    VectorBuffer() : pos_(0) {
        buffer_.reserve(1024);
    }
    
protected:
    // 输出字符
    virtual int overflow(int c) override {
        if (c != EOF) {
            buffer_.push_back(static_cast<char>(c));
        }
        return c;
    }
    
    // 获取字符
    virtual int underflow() override {
        if (pos_ >= buffer_.size()) {
            return EOF;
        }
        return buffer_[pos_];
    }
    
    // 移动到下一个字符
    virtual int uflow() override {
        if (pos_ >= buffer_.size()) {
            return EOF;
        }
        return buffer_[pos_++];
    }
    
    // 获取当前缓冲区内容
    const std::vector<char>& get_buffer() const {
        return buffer_;
    }
};

void custom_streambuf_example() {
    VectorBuffer buf;
    std::ostream os(&buf);
    
    os << "Hello, Custom Buffer!" << std::endl;
    os << "This is a test." << std::endl;
    
    const auto& data = buf.get_buffer();
    std::cout << "Buffer contents: ";
    for (char c : data) {
        std::cout << c;
    }
    std::cout << std::endl;
}
```

### 二、现代C++23打印功能

#### 1. 格式化打印
```cpp
#include <print>
#include <string>
#include <vector>

void print_examples() {
    // 基本打印
    std::print("Hello, {}!\n", "World");
    
    // 数值格式化
    std::print("Value: {:#x}, Pi: {:.2f}\n", 42, 3.14159);
    
    // 容器打印
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::print("Vector: {}\n", vec);
    
    // 带换行的打印
    std::println("This line ends with newline");
    std::println("Value: {}, Name: {}", 100, "Test");
}

void print_to_stream_examples() {
    // 打印到字符串流
    std::ostringstream oss;
    std::print(oss, "Formatted: {} and {}", 42, "hello");
    std::cout << "Stream output: " << oss.str() << std::endl;
    
    // 打印到文件
    std::ofstream file("output.txt");
    if (file.is_open()) {
        std::println(file, "File output: {} {}", "test", 123);
        file.close();
    }
}
```

### 三、字符串流操作

#### 1. 数据解析和序列化
```cpp
#include <sstream>
#include <iostream>
#include <iomanip>

class Person {
public:
    std::string name;
    int age;
    double salary;
    
    Person() = default;
    Person(const std::string& n, int a, double s) : name(n), age(a), salary(s) {}
    
    // 序列化到流
    void serialize(std::ostream& os) const {
        os << name << "|" << age << "|" << std::fixed << std::setprecision(2) << salary;
    }
    
    // 从流反序列化
    bool deserialize(std::istream& is) {
        std::string line;
        if (std::getline(is, line)) {
            std::istringstream iss(line);
            std::string token;
            
            // 解析姓名
            if (std::getline(iss, token, '|')) {
                name = token;
            }
            
            // 解析年龄
            if (std::getline(iss, token, '|')) {
                age = std::stoi(token);
            }
            
            // 解析薪资
            if (std::getline(iss, token)) {
                salary = std::stod(token);
            }
            
            return true;
        }
        return false;
    }
};

void serialization_example() {
    // 序列化
    Person person("John Doe", 30, 50000.50);
    std::ostringstream oss;
    person.serialize(oss);
    std::string serialized = oss.str();
    std::cout << "Serialized: " << serialized << std::endl;
    
    // 反序列化
    std::istringstream iss(serialized);
    Person deserialized;
    if (deserialized.deserialize(iss)) {
        std::cout << "Deserialized - Name: " << deserialized.name 
                  << ", Age: " << deserialized.age 
                  << ", Salary: " << deserialized.salary << std::endl;
    }
}
```

### 四、文件I/O高级操作

#### 1. 二进制文件操作
```cpp
#include <fstream>
#include <iostream>
#include <vector>

void binary_file_operations() {
    // 写入二进制数据
    std::vector<int> data = {1, 2, 3, 4, 5};
    std::ofstream file("binary.dat", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), 
                   data.size() * sizeof(int));
        file.close();
    }
    
    // 读取二进制数据
    std::ifstream input("binary.dat", std::ios::binary);
    if (input.is_open()) {
        input.seekg(0, std::ios::end);
        size_t size = input.tellg() / sizeof(int);
        input.seekg(0, std::ios::beg);
        
        std::vector<int> read_data(size);
        input.read(reinterpret_cast<char*>(read_data.data()), 
                   size * sizeof(int));
        
        std::cout << "Read binary data: ";
        for (int val : read_data) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        
        input.close();
    }
}
```

#### 2. 文件位置操作
```cpp
#include <fstream>
#include <iostream>

void file_position_operations() {
    std::fstream file("position_test.txt", std::ios::in | std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        // 写入数据
        file << "Line 1\nLine 2\nLine 3\n";
        
        // 获取当前位置
        std::streampos pos = file.tellp();
        std::cout << "Current position: " << pos << std::endl;
        
        // 移动到文件开头
        file.seekp(0, std::ios::beg);
        file << "Modified ";
        
        // 移动到特定位置
        file.seekp(7, std::ios::cur);
        file << " First";
        
        file.close();
    }
    
    // 读取修改后的文件
    std::ifstream input("position_test.txt");
    std::string line;
    while (std::getline(input, line)) {
        std::cout << "File line: " << line << std::endl;
    }
}
```

### 五、同步输出流（C++20）

#### 1. 线程安全输出
```cpp
#include <syncstream>
#include <iostream>
#include <thread>
#include <vector>

void synchronized_output_example() {
    std::vector<std::thread> threads;
    
    // 使用传统cout（可能输出混乱）
    std::cout << "=== Unsynced output ===" << std::endl;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([i]() {
            for (int j = 0; j < 5; ++j) {
                std::cout << "Thread " << i << " - " << j << std::endl;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // 使用同步输出流
    std::cout << "\n=== Synced output ===" << std::endl;
    threads.clear();
    
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([i]() {
            std::osyncstream synced_cout(std::cout);
            for (int j = 0; j < 5; ++j) {
                synced_cout << "Thread " << i << " - " << j << std::endl;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
}
```

### 六、C++23 span流操作

#### 1. 固定缓冲区操作
```cpp
#include <spanstream>
#include <iostream>
#include <array>

void spanstream_example() {
    // 使用固定大小缓冲区
    std::array<char, 100> buffer{};
    
    // 输出到span
    std::ospanstream oss(buffer);
    oss << "Hello, spanstream! Value: " << 42;
    
    // 读取span内容
    std::cout << "Buffer content: " << buffer.data() << std::endl;
    
    // 从span读取
    std::ispanstream iss(buffer);
    std::string word;
    int value;
    iss >> word >> word >> word >> value;  // Skip "Hello," and "spanstream!"
    std::cout << "Read value: " << value << std::endl;
}
```

### 七、错误处理和异常

#### 1. I/O异常处理
```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <system_error>

void io_error_handling() {
    try {
        // 设置异常模式
        std::ifstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);
        
        // 尝试打开不存在的文件
        file.open("nonexistent.txt");
        
    } catch (const std::ios_base::failure& e) {
        std::cout << "I/O exception: " << e.what() << std::endl;
        std::cout << "Error code: " << e.code().message() << std::endl;
    }
    
    // 使用错误码而非异常
    std::error_code ec;
    std::ifstream safe_file("nonexistent.txt", std::ios::in, ec);
    if (ec) {
        std::cout << "Error opening file: " << ec.message() << std::endl;
    }
}
```

### 八、性能优化技巧

#### 1. 缓冲区优化
```cpp
#include <iostream>
#include <fstream>
#include <chrono>

void performance_optimization() {
    const int iterations = 10000;
    
    // 未优化的输出
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::cout << "Value: " << i << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto unoptimized_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 优化的输出 - 使用缓冲
    std::ostringstream oss;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        oss << "Value: " << i << '\n';  // 使用'\n'而非std::endl
    }
    std::cout << oss.str();  // 一次性输出
    end = std::chrono::high_resolution_clock::now();
    auto optimized_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Unoptimized time: " << unoptimized_time.count() << "ms" << std::endl;
    std::cout << "Optimized time: " << optimized_time.count() << "ms" << std::endl;
}
```

### 九、实际应用示例

#### 1. 日志系统实现
```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

class Logger {
private:
    std::ofstream log_file_;
    bool console_output_;
    
public:
    Logger(const std::string& filename, bool console = true) 
        : log_file_(filename, std::ios::app), console_output_(console) {}
    
    template<typename... Args>
    void log(const std::string& level, const Args&... args) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        oss << " [" << level << "] ";
        
        log_impl(oss, args...);
        oss << '\n';
        
        std::string message = oss.str();
        if (console_output_) {
            std::cout << message;
        }
        if (log_file_.is_open()) {
            log_file_ << message;
            log_file_.flush();
        }
    }
    
private:
    template<typename T, typename... Args>
    void log_impl(std::ostringstream& oss, const T& first, const Args&... rest) {
        oss << first;
        if constexpr (sizeof...(rest) > 0) {
            log_impl(oss, rest...);
        }
    }
    
    void log_impl(std::ostringstream& oss) {}
};

void logger_example() {
    Logger logger("app.log");
    
    logger.log("INFO", "Application started");
    logger.log("DEBUG", "Processing item ", 42, " with value ", 3.14);
    logger.log("ERROR", "Failed to process item");
    logger.log("INFO", "Application finished");
}
```

#### 2. 配置文件解析器
```cpp
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>

class ConfigParser {
private:
    std::map<std::string, std::string> config_;
    
public:
    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            // 跳过注释和空行
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }
            
            // 解析键值对
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                // 去除空白字符
                key = trim(key);
                value = trim(value);
                config_[key] = value;
            }
        }
        
        return true;
    }
    
    template<typename T>
    T get(const std::string& key, const T& default_value = T{}) const {
        auto it = config_.find(key);
        if (it != config_.end()) {
            std::istringstream iss(it->second);
            T value;
            if (iss >> value) {
                return value;
            }
        }
        return default_value;
    }
    
    void set(const std::string& key, const std::string& value) {
        config_[key] = value;
    }
    
    bool save(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        for (const auto& [key, value] : config_) {
            file << key << "=" << value << std::endl;
        }
        
        return true;
    }
    
private:
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) {
            return "";
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
};

void config_parser_example() {
    ConfigParser config;
    
    // 设置配置值
    config.set("server.host", "localhost");
    config.set("server.port", "8080");
    config.set("database.timeout", "30");
    config.set("debug.enabled", "true");
    
    // 保存到文件
    config.save("config.ini");
    
    // 重新加载并使用
    ConfigParser loaded_config;
    if (loaded_config.load("config.ini")) {
        std::string host = loaded_config.get<std::string>("server.host", "127.0.0.1");
        int port = loaded_config.get<int>("server.port", 80);
        int timeout = loaded_config.get<int>("database.timeout", 10);
        bool debug = loaded_config.get<bool>("debug.enabled", false);
        
        std::cout << "Host: " << host << std::endl;
        std::cout << "Port: " << port << std::endl;
        std::cout << "Timeout: " << timeout << std::endl;
        std::cout << "Debug: " << (debug ? "enabled" : "disabled") << std::endl;
    }
}
```

## 编译和使用注意事项

### 一、C++版本兼容性
```cpp
#include <iostream>

// 条件编译检查特性可用性
#if __cplusplus >= 202302L
    #include <print>
    #define HAS_PRINT_FEATURES 1
#else
    #define HAS_PRINT_FEATURES 0
#endif

#if __cplusplus >= 202002L
    #include <syncstream>
    #define HAS_SYNCSTREAM 1
#else
    #define HAS_SYNCSTREAM 0
#endif

void version_compatibility_example() {
#if HAS_PRINT_FEATURES
    std::println("Using C++23 print features");
#else
    std::cout << "Using traditional I/O" << std::endl;
#endif

#if HAS_SYNCSTREAM
    std::osyncstream synced(std::cout);
    synced << "Synchronized output\n";
#else
    std::cout << "Regular output\n";
#endif
}
```

### 二、性能考虑

1. **避免频繁的std::endl**：使用'\n'代替std::endl以避免不必要的刷新
2. **批量输出**：使用ostringstream缓冲输出内容
3. **异常处理**：合理使用异常模式避免性能损失
4. **缓冲区管理**：根据需要调整流缓冲区大小

## 相关页面

| 页面 | 说明 |
|------|------|
| 文件系统库（C++17起） | 文件系统操作相关功能 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/io&oldid=179905>
- 最后修改时间：2025年1月28日 11:21
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++输入/输出库是现代C++编程的核心组件，提供了强大的数据输入输出能力。从基础的iostream到现代的print函数，从文件操作到同步输出，I/O库不断发展以满足现代应用需求。理解流类层次结构、掌握各种流操作技巧、合理使用新特性，是编写高效C++程序的关键。正确处理I/O异常、优化性能、实现线程安全操作，能够构建更加健壮的应用系统。持续学习和实践I/O库的新特性，特别是C++20和C++23引入的同步流和打印函数，是保持技术前沿的必要途径。I/O库的成功应用将显著提高C++程序的数据处理能力和用户体验。