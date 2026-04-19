# C++ 文本处理库（Text processing library）

来源：cppreference.com

## 文本处理库概述

文本处理库包含用于处理文本的组件，提供了从基本字符操作到高级文本格式化的全面功能。

## 核心组件

### 一、本地化库

头文件 `<locale>` 和 `<clocale>` 提供国际化支持，包括：
- 字符分类和字符串排序
- 数字、货币和日期/时间格式化与解析
- 消息检索

### 二、正则表达式库（C++11起）

头文件 `<regex>` 提供表示正则表达式的类，这是一种用于在字符串内执行模式匹配的小型语言。

### 三、格式化库（C++20起）

头文件 `<format>` 提供类型安全且可扩展的字符串格式化功能，是`printf`函数族的替代方案，旨在补充现有的C++ I/O流库。

### 四、以空字符结尾的序列工具

**以空字符结尾的字符序列**（NTCTS）是由空字符终止的字符序列（初始化后的值）。

字符串库提供以下函数来创建、检查和修改此类序列：
- 以空字符结尾的字节字符串（NTBS）辅助函数（包括宽字符类型支持）
- 以空字符结尾的多字节字符串（NTMBS）辅助函数

### 五、基本数值转换（C++17起）

除了C++ I/O库、C I/O库、C++字符串转换器和C字符串转换器提供的复杂本地化相关解析器和格式化器外，头文件 `<charconv>` 还提供轻量级、本地化无关、非分配、非抛出的算术类型解析器和格式化器。

| 组件 | 说明 |
|------|------|
| `<charconv>`头文件 | |
| `to_chars`(C++17) | 将整数或浮点数值转换为字符序列（函数） |
| `to_chars_result`(C++17) | `std::to_chars`的返回类型（类） |
| `from_chars`(C++17) | 将字符序列转换为整数或浮点数值（函数） |
| `from_chars_result`(C++17) | `std::from_chars`的返回类型（类） |
| `chars_format`(C++17) | 为`std::to_chars`和`std::from_chars`指定格式（枚举） |

### 六、文本编码标识（C++26起）

| 组件 | 说明 |
|------|------|
| `<text_encoding>`头文件 | |
| `text_encoding`(C++26) | 描述访问IANA字符集注册表的接口（类） |

---

## 扩展知识详解

### 一、字符序列操作

#### 1. 以空字符结尾的字符串操作
```cpp
#include <cstring>
#include <iostream>
#include <string>

void null_terminated_string_examples() {
    // C风格字符串操作
    const char* cstr1 = "Hello";
    const char* cstr2 = " World";
    
    // 字符串长度
    size_t len1 = std::strlen(cstr1);
    size_t len2 = std::strlen(cstr2);
    std::cout << "Length of cstr1: " << len1 << std::endl;
    std::cout << "Length of cstr2: " << len2 << std::endl;
    
    // 字符串比较
    int cmp_result = std::strcmp(cstr1, "Hello");
    std::cout << "strcmp result: " << cmp_result << std::endl;
    
    // 字符串复制
    char buffer[20];
    std::strcpy(buffer, cstr1);
    std::strcat(buffer, cstr2);
    std::cout << "Concatenated: " << buffer << std::endl;
    
    // 字符串查找
    const char* found = std::strstr(buffer, "World");
    if (found) {
        std::cout << "Found 'World' at position: " << (found - buffer) << std::endl;
    }
    
    // 字符查找
    const char* char_found = std::strchr(buffer, 'W');
    if (char_found) {
        std::cout << "Found 'W' at position: " << (char_found - buffer) << std::endl;
    }
}

void wide_string_examples() {
    // 宽字符串操作
    const wchar_t* wstr = L"Hello, 世界!";
    size_t wlen = std::wcslen(wstr);
    std::wcout << L"Wide string length: " << wlen << std::endl;
    std::wcout << L"Wide string: " << wstr << std::endl;
}
```

#### 2. 多字节字符串处理
```cpp
#include <clocale>
#include <cwchar>
#include <iostream>
#include <string>

void multibyte_string_examples() {
    // 设置本地化以支持多字节字符
    std::setlocale(LC_ALL, "en_US.UTF-8");
    
    // 多字节字符串
    const char* mbstr = "Hello, 世界!";
    std::cout << "Multibyte string: " << mbstr << std::endl;
    
    // 转换为宽字符串
    wchar_t wbuffer[50];
    size_t converted = std::mbstowcs(wbuffer, mbstr, 50);
    if (converted != static_cast<size_t>(-1)) {
        std::wcout << L"Converted to wide: " << wbuffer << std::endl;
        std::wcout << L"Number of wide characters: " << converted << std::endl;
    }
    
    // 从宽字符串转回多字节
    char mb_buffer[50];
    size_t back_converted = std::wcstombs(mb_buffer, wbuffer, 50);
    if (back_converted != static_cast<size_t>(-1)) {
        std::cout << "Converted back: " << mb_buffer << std::endl;
    }
    
    // 计算多字节字符串长度
    size_t mb_len = std::mblen(mbstr, 20);
    std::cout << "Multibyte length of first character: " << mb_len << std::endl;
}
```

### 二、数值转换（C++17起）

#### 1. to_chars函数使用
```cpp
#include <charconv>
#include <iostream>
#include <string>
#include <array>

void to_chars_examples() {
    // 整数转换
    std::array<char, 20> buffer;
    
    // 基本整数转换
    int value = 12345;
    auto result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);
    if (result.ec == std::errc{}) {
        std::string str(buffer.data(), result.ptr - buffer.data());
        std::cout << "Integer " << value << " as string: " << str << std::endl;
    }
    
    // 指定进制的转换
    int hex_value = 255;
    auto hex_result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), 
                                   hex_value, 16);
    if (hex_result.ec == std::errc{}) {
        std::string hex_str(buffer.data(), hex_result.ptr - buffer.data());
        std::cout << "Hexadecimal " << hex_value << " as string: " << hex_str << std::endl;
    }
    
    // 浮点数转换
    double pi = 3.141592653589793;
    auto float_result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), pi);
    if (float_result.ec == std::errc{}) {
        std::string float_str(buffer.data(), float_result.ptr - buffer.data());
        std::cout << "Pi as string: " << float_str << std::endl;
    }
    
    // 指定格式的浮点数转换
    auto scientific_result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), 
                                          pi, std::chars_format::scientific, 3);
    if (scientific_result.ec == std::errc{}) {
        std::string sci_str(buffer.data(), scientific_result.ptr - buffer.data());
        std::cout << "Pi in scientific format: " << sci_str << std::endl;
    }
}

void from_chars_examples() {
    // 字符串到整数转换
    const char* int_str = "12345";
    int parsed_int;
    auto result = std::from_chars(int_str, int_str + std::strlen(int_str), parsed_int);
    if (result.ec == std::errc{}) {
        std::cout << "Parsed integer: " << parsed_int << std::endl;
    }
    
    // 字符串到浮点数转换
    const char* float_str = "3.14159";
    double parsed_double;
    auto float_result = std::from_chars(float_str, float_str + std::strlen(float_str), 
                                       parsed_double);
    if (float_result.ec == std::errc{}) {
        std::cout << "Parsed double: " << parsed_double << std::endl;
    }
    
    // 错误处理
    const char* invalid_str = "abc123";
    int invalid_int;
    auto invalid_result = std::from_chars(invalid_str, invalid_str + std::strlen(invalid_str), 
                                         invalid_int);
    if (invalid_result.ec == std::errc::invalid_argument) {
        std::cout << "Invalid argument error occurred" << std::endl;
    }
}
```

#### 2. 高性能数值转换
```cpp
#include <charconv>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>

void performance_comparison() {
    const int iterations = 1000000;
    int test_value = 123456789;
    
    // std::to_chars性能测试
    auto start = std::chrono::high_resolution_clock::now();
    std::array<char, 20> buffer;
    for (int i = 0; i < iterations; ++i) {
        auto result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), test_value);
        if (result.ec != std::errc{}) {
            std::cout << "Error in to_chars" << std::endl;
            break;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto to_chars_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // std::to_string性能测试
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::string str = std::to_string(test_value);
    }
    end = std::chrono::high_resolution_clock::now();
    auto to_string_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // stringstream性能测试
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::ostringstream oss;
        oss << test_value;
        std::string str = oss.str();
    }
    end = std::chrono::high_resolution_clock::now();
    auto stringstream_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "to_chars time: " << to_chars_duration.count() << " microseconds" << std::endl;
    std::cout << "to_string time: " << to_string_duration.count() << " microseconds" << std::endl;
    std::cout << "stringstream time: " << stringstream_duration.count() << " microseconds" << std::endl;
    std::cout << "to_chars is " << static_cast<double>(to_string_duration.count()) / to_chars_duration.count() 
              << "x faster than to_string" << std::endl;
}
```

### 三、现代格式化（C++20起）

#### 1. std::format基础使用
```cpp
#include <format>
#include <iostream>
#include <string>

void format_examples() {
    // 基本格式化
    std::string formatted = std::format("Hello, {}!", "World");
    std::cout << formatted << std::endl;
    
    // 数值格式化
    int num = 42;
    double pi = 3.14159;
    std::string numbers = std::format("Number: {}, Pi: {:.2f}", num, pi);
    std::cout << numbers << std::endl;
    
    // 对齐和填充
    std::string aligned = std::format("{:>10} | {:<10} | {:^10}", "Right", "Left", "Center");
    std::cout << aligned << std::endl;
    
    // 进制转换
    std::string bases = std::format("Dec: {:d}, Hex: {:x}, Oct: {:o}", 255, 255, 255);
    std::cout << bases << std::endl;
    
    // 条件格式化
    bool condition = true;
    std::string conditional = std::format("Value is {}", condition ? "true" : "false");
    std::cout << conditional << std::endl;
    
    // 复杂对象格式化
    struct Point {
        int x, y;
    };
    
    Point p{10, 20};
    // 注意：需要自定义格式化器或使用结构化绑定
    std::string point_str = std::format("Point({},{})", p.x, p.y);
    std::cout << point_str << std::endl;
}
```

#### 2. 格式化性能和安全性
```cpp
#include <format>
#include <iostream>
#include <chrono>
#include <sstream>

void format_performance() {
    const int iterations = 100000;
    int value = 12345;
    double pi = 3.14159;
    
    // std::format性能测试
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::string formatted = std::format("Value: {}, Pi: {:.2f}", value, pi);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto format_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // sprintf性能测试（不安全，仅用于比较）
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        char buffer[100];
        sprintf(buffer, "Value: %d, Pi: %.2f", value, pi);
        std::string str(buffer);
    }
    end = std::chrono::high_resolution_clock::now();
    auto sprintf_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // stringstream性能测试
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::ostringstream oss;
        oss << "Value: " << value << ", Pi: " << std::fixed << std::setprecision(2) << pi;
        std::string str = oss.str();
    }
    end = std::chrono::high_resolution_clock::now();
    auto stringstream_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "std::format time: " << format_duration.count() << " microseconds" << std::endl;
    std::cout << "sprintf time: " << sprintf_duration.count() << " microseconds" << std::endl;
    std::cout << "stringstream time: " << stringstream_duration.count() << " microseconds" << std::endl;
    
    // 安全性演示 - std::format不会缓冲区溢出
    try {
        std::string safe_format = std::format("{:*<10}", "Hello");
        std::cout << "Safe format: " << safe_format << std::endl;
    } catch (const std::format_error& e) {
        std::cout << "Format error: " << e.what() << std::endl;
    }
}
```

### 四、文本编码处理（C++26起）

#### 1. text_encoding使用
```cpp
#include <text_encoding>
#include <iostream>

void text_encoding_examples() {
    // 获取系统默认编码
    auto default_encoding = std::text_encoding::system();
    std::cout << "System encoding name: " << default_encoding.name() << std::endl;
    
    // 创建特定编码
    try {
        auto utf8_encoding = std::text_encoding("UTF-8");
        std::cout << "UTF-8 encoding name: " << utf8_encoding.name() << std::endl;
        
        // 检查编码属性
        std::cout << "Is ASCII compatible: " << utf8_encoding.is_ascii_compatible() << std::endl;
    } catch (const std::system_error& e) {
        std::cout << "Encoding error: " << e.what() << std::endl;
    }
    
    // 列出所有可用编码
    auto encodings = std::text_encoding::all_encodings();
    std::cout << "Available encodings:" << std::endl;
    for (const auto& encoding : encodings) {
        std::cout << "  " << encoding << std::endl;
    }
}
```

### 五、实际应用示例

#### 1. 综合文本处理器
```cpp
#include <charconv>
#include <format>
#include <regex>
#include <string>
#include <vector>
#include <iostream>

class TextProcessor {
public:
    // 高性能整数转换
    static std::string fast_int_to_string(int value) {
        std::array<char, 20> buffer;
        auto result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);
        if (result.ec == std::errc{}) {
            return std::string(buffer.data(), result.ptr - buffer.data());
        }
        return {};
    }
    
    // 高性能字符串到整数转换
    static std::optional<int> fast_string_to_int(const std::string& str) {
        int value;
        auto result = std::from_chars(str.data(), str.data() + str.size(), value);
        if (result.ec == std::errc{}) {
            return value;
        }
        return std::nullopt;
    }
    
    // 格式化日志消息
    static std::string format_log(const std::string& level, const std::string& message) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        
        return std::format("[{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}] [{}] {}",
                          tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                          tm.tm_hour, tm.tm_min, tm.tm_sec,
                          level, message);
    }
    
    // 提取数字
    static std::vector<double> extract_numbers(const std::string& text) {
        std::regex number_regex(R"([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)");
        std::sregex_iterator iter(text.begin(), text.end(), number_regex);
        std::sregex_iterator end;
        
        std::vector<double> numbers;
        for (; iter != end; ++iter) {
            try {
                double num = std::stod(iter->str());
                numbers.push_back(num);
            } catch (const std::exception&) {
                // 忽略无效数字
            }
        }
        return numbers;
    }
};

void text_processor_examples() {
    // 高性能数字转换
    std::cout << "Fast int to string: " << TextProcessor::fast_int_to_string(12345) << std::endl;
    
    auto parsed = TextProcessor::fast_string_to_int("67890");
    if (parsed) {
        std::cout << "Fast string to int: " << *parsed << std::endl;
    }
    
    // 格式化日志
    std::string log_message = TextProcessor::format_log("INFO", "Application started");
    std::cout << "Log message: " << log_message << std::endl;
    
    // 提取数字
    std::string text_with_numbers = "The temperature is 23.5°C and humidity is 65.2%";
    auto numbers = TextProcessor::extract_numbers(text_with_numbers);
    std::cout << "Extracted numbers: ";
    for (double num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
```

#### 2. 配置文件解析器
```cpp
#include <charconv>
#include <format>
#include <string>
#include <map>
#include <iostream>
#include <sstream>

class ConfigParser {
private:
    std::map<std::string, std::string> config_;
    
public:
    bool parse(const std::string& config_text) {
        std::istringstream iss(config_text);
        std::string line;
        
        while (std::getline(iss, line)) {
            // 跳过注释和空行
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }
            
            // 查找等号
            size_t eq_pos = line.find('=');
            if (eq_pos != std::string::npos) {
                std::string key = line.substr(0, eq_pos);
                std::string value = line.substr(eq_pos + 1);
                
                // 去除空白字符
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                config_[key] = value;
            }
        }
        return true;
    }
    
    template<typename T>
    std::optional<T> get(const std::string& key) const {
        auto it = config_.find(key);
        if (it != config_.end()) {
            return string_to_type<T>(it->second);
        }
        return std::nullopt;
    }
    
    void set(const std::string& key, const std::string& value) {
        config_[key] = value;
    }
    
    std::string to_string() const {
        std::string result;
        for (const auto& [key, value] : config_) {
            result += std::format("{}={}\n", key, value);
        }
        return result;
    }

private:
    template<typename T>
    std::optional<T> string_to_type(const std::string& str) const {
        if constexpr (std::is_same_v<T, int>) {
            int value;
            auto result = std::from_chars(str.data(), str.data() + str.size(), value);
            if (result.ec == std::errc{}) {
                return value;
            }
        } else if constexpr (std::is_same_v<T, double>) {
            try {
                return std::stod(str);
            } catch (const std::exception&) {
                return std::nullopt;
            }
        } else if constexpr (std::is_same_v<T, std::string>) {
            return str;
        } else if constexpr (std::is_same_v<T, bool>) {
            if (str == "true" || str == "1" || str == "yes") {
                return true;
            } else if (str == "false" || str == "0" || str == "no") {
                return false;
            }
        }
        return std::nullopt;
    }
};

void config_parser_example() {
    std::string config_text = R"(
        # Application configuration
        server_port=8080
        max_connections=100
        timeout=30.5
        debug=true
        log_level=info
    )";
    
    ConfigParser parser;
    if (parser.parse(config_text)) {
        std::cout << "Configuration parsed successfully" << std::endl;
        
        // 获取配置值
        auto port = parser.get<int>("server_port");
        if (port) {
            std::cout << "Server port: " << *port << std::endl;
        }
        
        auto max_conn = parser.get<int>("max_connections");
        if (max_conn) {
            std::cout << "Max connections: " << *max_conn << std::endl;
        }
        
        auto timeout = parser.get<double>("timeout");
        if (timeout) {
            std::cout << "Timeout: " << *timeout << std::endl;
        }
        
        auto debug = parser.get<bool>("debug");
        if (debug) {
            std::cout << "Debug mode: " << std::boolalpha << *debug << std::endl;
        }
        
        auto log_level = parser.get<std::string>("log_level");
        if (log_level) {
            std::cout << "Log level: " << *log_level << std::endl;
        }
        
        // 输出完整配置
        std::cout << "\nFull configuration:\n" << parser.to_string() << std::endl;
    }
}
```

## 最佳实践建议

### 一、字符序列处理准则

1. **选择合适的字符串类型**：根据需求选择`std::string`、C风格字符串或宽字符串
2. **正确处理多字节字符**：使用适当的本地化设置和多字节函数
3. **避免缓冲区溢出**：使用安全的字符串操作函数
4. **内存管理**：注意动态分配的字符串内存释放

### 二、数值转换最佳实践

1. **使用C++17 charconv**：对于高性能、无异常、无分配的数值转换
2. **错误处理**：始终检查转换结果的错误码
3. **缓冲区大小**：确保字符转换缓冲区足够大
4. **进制选择**：根据需要选择合适的数值进制

### 三、格式化建议

1. **优先使用std::format**：C++20起使用类型安全的格式化
2. **性能考虑**：std::format通常比传统方法更快更安全
3. **格式字符串验证**：在编译时检查格式字符串正确性
4. **本地化支持**：复杂格式化需求时考虑使用本地化库

### 四、编码处理建议

1. **统一编码标准**：项目中统一使用UTF-8编码
2. **编码转换**：需要时进行正确的编码转换
3. **错误处理**：妥善处理编码相关异常
4. **国际化支持**：考虑多语言文本处理需求

## 相关页面

| 页面 | 说明 |
|------|------|
| 字符串库 | 基础字符串操作 |
| 本地化库 | 本地化文本处理 |
| 正则表达式库 | 文本模式匹配 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/text&oldid=179207>
- 最后修改时间：2025年1月3日 09:49
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++文本处理库为开发者提供了全面的文本处理能力，从基础的字符操作到高级的格式化和编码处理。正确理解和使用这些特性对于构建高质量的应用程序至关重要。现代C++特性如`std::format`和`std::charconv`提供了更安全、更高效的文本处理方案。通过合理使用这些工具，结合良好的编程实践，能够构建出功能强大且性能优异的文本处理应用程序。持续学习和实践这些技术，关注C++标准的最新发展，能够帮助开发者保持技术的先进性。