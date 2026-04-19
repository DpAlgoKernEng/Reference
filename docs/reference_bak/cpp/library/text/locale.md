# C++ 本地化库（Localization library）

来源：cppreference.com

## 本地化库概述

本地化设施包括对字符分类和字符串排序、数字、货币、日期/时间格式化和解析以及消息检索的国际化支持。本地化设置控制流I/O、正则表达式库和其他C++标准库组件的行为。

## 核心组件

### 一、本地化和语言环境面（Locales and facets）

| 组件 | 说明 |
|------|------|
| `<locale>`头文件 | |
| **语言环境** |
| `locale` | 封装文化差异的多态面集合（类） |
| `use_facet` | 从语言环境中获取面（函数模板） |
| `has_facet` | 检查语言环境是否实现特定面（函数模板） |
| **面类别基类** |
| `ctype_base` | 定义字符分类类别（类） |
| `codecvt_base` | 定义字符转换错误（类） |
| `messages_base` | 定义消息目录类型（类） |
| `time_base` | 定义日期格式常量（类） |
| `money_base` | 定义货币格式模式（类） |
| **ctype面** |
| `ctype` | 定义字符分类表（类模板） |
| `ctype_byname` | 表示命名语言环境的系统提供的`std::ctype`（类模板） |
| `ctype<char>` | `std::ctype`对char类型的特化（类模板特化） |
| `codecvt` | 在字符编码间转换，包括UTF-8、UTF-16、UTF-32（类模板） |
| `codecvt_byname` | 表示命名语言环境的系统提供的`std::codecvt`（类模板） |
| **数字面** |
| `num_get` | 从输入字符序列解析数值（类模板） |
| `num_put` | 将数值格式化为输出字符序列（类模板） |
| `numpunct` | 定义数字标点规则（类模板） |
| `numpunct_byname` | 表示命名语言环境的系统提供的`std::numpunct`（类模板） |
| **排序面** |
| `collate` | 定义字符串的词典比较和哈希（类模板） |
| `collate_byname` | 表示命名语言环境的系统提供的`std::collate`（类模板） |
| **时间面** |
| `time_get` | 将时间/日期值从输入字符序列解析到`std::tm`（类模板） |
| `time_get_byname` | 表示命名语言环境的系统提供的`std::time_get`（类模板） |
| `time_put` | 将`std::tm`的内容格式化为输出字符序列（类模板） |
| `time_put_byname` | 表示命名语言环境的系统提供的`std::time_put`（类模板） |
| **货币面** |
| `money_get` | 从输入字符序列解析并构造货币值（类模板） |
| `money_put` | 将货币值格式化为输出字符序列（类模板） |
| `moneypunct` | 定义`std::money_get`和`std::money_put`使用的货币格式参数（类模板） |
| `moneypunct_byname` | 表示命名语言环境的系统提供的`std::moneypunct`（类模板） |
| **消息面** |
| `messages` | 实现从消息目录检索字符串（类模板） |
| `messages_byname` | 表示命名语言环境的系统提供的`std::messages`（类模板） |

### 二、字符分类和转换

| 组件 | 说明 |
|------|------|
| `<locale>`头文件 | |
| **字符分类** |
| `isspace(std::locale)` | 检查字符是否被语言环境分类为空白字符（函数模板） |
| `isblank(std::locale)`(C++11) | 检查字符是否被语言环境分类为空格字符（函数模板） |
| `iscntrl(std::locale)` | 检查字符是否被语言环境分类为控制字符（函数模板） |
| `isupper(std::locale)` | 检查字符是否被语言环境分类为大写字符（函数模板） |
| `islower(std::locale)` | 检查字符是否被语言环境分类为小写字符（函数模板） |
| `isalpha(std::locale)` | 检查字符是否被语言环境分类为字母字符（函数模板） |
| `isdigit(std::locale)` | 检查字符是否被语言环境分类为数字字符（函数模板） |
| `ispunct(std::locale)` | 检查字符是否被语言环境分类为标点字符（函数模板） |
| `isxdigit(std::locale)` | 检查字符是否被语言环境分类为十六进制数字字符（函数模板） |
| `isalnum(std::locale)` | 检查字符是否被语言环境分类为字母数字字符（函数模板） |
| `isprint(std::locale)` | 检查字符是否被语言环境分类为可打印字符（函数模板） |
| `isgraph(std::locale)` | 检查字符是否被语言环境分类为图形字符（函数模板） |
| **字符转换** |
| `toupper(std::locale)` | 使用语言环境的ctype面将字符转换为大写（函数模板） |
| `tolower(std::locale)` | 使用语言环境的`ctype`面将字符转换为小写（函数模板） |
| **字符串和流转换** |
| `wstring_convert`(C++11) | 在宽字符串和字节字符串间执行转换（类模板）（C++17起已弃用，C++26起已移除） |
| `wbuffer_convert`(C++11) | 在字节流缓冲区和宽流缓冲区间执行转换（类模板）（C++17起已弃用，C++26起已移除） |

### 三、Unicode转换面（C++11-C++26）

| 组件 | 说明 |
|------|------|
| `<codecvt>`头文件 | |
| `codecvt_utf8`(C++11) | 在UTF-8和UCS-2/UCS-4间转换（类模板）（C++17起已弃用，C++26起已移除） |
| `codecvt_utf16`(C++11) | 在UTF-16和UCS-2/UCS-4间转换（类模板）（C++17起已弃用，C++26起已移除） |
| `codecvt_utf8_utf16`(C++11) | 在UTF-8和UTF-16间转换（类模板）（C++17起已弃用，C++26起已移除） |
| `codecvt_mode`(C++11) | 标记以改变标准codecvt面行为的标签（枚举）（C++17起已弃用，C++26起已移除） |

### 四、C库本地化

| 组件 | 说明 |
|------|------|
| `<clocale>`头文件 | |
| `setlocale` | 获取和设置当前C语言环境（函数） |
| `LC_ALL`等 | `std::setlocale`的语言环境类别（宏常量） |
| `localeconv` | 查询当前语言环境的数字和货币格式详细信息（函数） |
| `lconv` | 格式详细信息，由`std::localeconv`返回（类） |

---

## 扩展知识详解

### 一、语言环境基础

#### 1. 基本语言环境使用
```cpp
#include <locale>
#include <iostream>
#include <string>

void basic_locale_examples() {
    // 创建默认语言环境
    std::locale default_locale;  // "C"语言环境
    std::cout << "Default locale name: " << default_locale.name() << std::endl;
    
    // 创建系统默认语言环境
    std::locale system_locale("");
    std::cout << "System locale name: " << system_locale.name() << std::endl;
    
    // 创建特定语言环境
    try {
        std::locale german_locale("de_DE.UTF-8");
        std::cout << "German locale name: " << german_locale.name() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "German locale not available: " << e.what() << std::endl;
    }
    
    // 复合语言环境 - 组合不同面
    std::locale custom_locale(default_locale, 
                             new std::numpunct_byname<char>("en_US.UTF-8"));
    std::cout << "Custom locale created" << std::endl;
    
    // 检查语言环境面
    std::cout << "Has ctype facet: " << std::has_facet<std::ctype<char>>(default_locale) << std::endl;
    std::cout << "Has numpunct facet: " << std::has_facet<std::numpunct<char>>(default_locale) << std::endl;
    std::cout << "Has collate facet: " << std::has_facet<std::collate<char>>(default_locale) << std::endl;
}
```

#### 2. 语言环境面的使用
```cpp
#include <locale>
#include <iostream>
#include <sstream>

void locale_facets_examples() {
    // 使用ctype面进行字符分类
    std::locale loc("");
    const std::ctype<char>& ct = std::use_facet<std::ctype<char>>(loc);
    
    char test_chars[] = {'A', 'z', '5', ' ', '\t', '@'};
    for (char c : test_chars) {
        std::cout << "Character '" << c << "': ";
        if (ct.is(std::ctype_base::upper, c)) std::cout << "uppercase ";
        if (ct.is(std::ctype_base::lower, c)) std::cout << "lowercase ";
        if (ct.is(std::ctype_base::digit, c)) std::cout << "digit ";
        if (ct.is(std::ctype_base::space, c)) std::cout << "space ";
        if (ct.is(std::ctype_base::punct, c)) std::cout << "punctuation ";
        std::cout << std::endl;
    }
    
    // 使用numpunct面获取数字格式信息
    const std::numpunct<char>& np = std::use_facet<std::numpunct<char>>(loc);
    std::cout << "Decimal point: '" << np.decimal_point() << "'" << std::endl;
    std::cout << "Thousands separator: '" << np.thousands_sep() << "'" << std::endl;
    std::cout << "Grouping: ";
    for (char g : np.grouping()) {
        std::cout << static_cast<int>(g) << " ";
    }
    std::cout << std::endl;
    
    // 使用collate面进行字符串比较
    const std::collate<char>& coll = std::use_facet<std::collate<char>>(loc);
    std::string str1 = "apple";
    std::string str2 = "Apple";
    
    int result = coll.compare(str1.data(), str1.data() + str1.length(),
                             str2.data(), str2.data() + str2.length());
    std::cout << "Comparison result: " << result << std::endl;
}
```

### 二、字符分类和转换

#### 1. 字符分类函数
```cpp
#include <locale>
#include <iostream>
#include <string>

void character_classification_examples() {
    std::locale loc("");  // 系统默认语言环境
    
    // 测试不同类型的字符
    std::string test_string = "Hello, World! 123 \t\n";
    
    std::cout << "Character analysis:" << std::endl;
    for (char c : test_string) {
        std::cout << "Character '" << c << "' (" << static_cast<int>(c) << "): ";
        
        if (std::isspace(c, loc)) std::cout << "space ";
        if (std::isblank(c, loc)) std::cout << "blank ";
        if (std::iscntrl(c, loc)) std::cout << "control ";
        if (std::isupper(c, loc)) std::cout << "upper ";
        if (std::islower(c, loc)) std::cout << "lower ";
        if (std::isalpha(c, loc)) std::cout << "alpha ";
        if (std::isdigit(c, loc)) std::cout << "digit ";
        if (std::ispunct(c, loc)) std::cout << "punct ";
        if (std::isxdigit(c, loc)) std::cout << "xdigit ";
        if (std::isalnum(c, loc)) std::cout << "alnum ";
        if (std::isprint(c, loc)) std::cout << "print ";
        if (std::isgraph(c, loc)) std::cout << "graph ";
        
        std::cout << std::endl;
    }
}

void character_conversion_examples() {
    std::locale loc("");  // 系统默认语言环境
    
    std::string mixed_case = "Hello, World! 123";
    std::cout << "Original: " << mixed_case << std::endl;
    
    // 转换为大写
    std::string upper_case = mixed_case;
    for (char& c : upper_case) {
        c = std::toupper(c, loc);
    }
    std::cout << "Uppercase: " << upper_case << std::endl;
    
    // 转换为小写
    std::string lower_case = mixed_case;
    for (char& c : lower_case) {
        c = std::tolower(c, loc);
    }
    std::cout << "Lowercase: " << lower_case << std::endl;
}
```

### 三、数字格式化和解析

#### 1. 数字面的使用
```cpp
#include <locale>
#include <iostream>
#include <sstream>
#include <iomanip>

void numeric_formatting_examples() {
    std::locale loc("en_US.UTF-8");  // 美国英语语言环境
    
    // 使用numpunct面获取格式信息
    const std::numpunct<char>& np = std::use_facet<std::numpunct<char>>(loc);
    std::cout << "Decimal point: " << np.decimal_point() << std::endl;
    std::cout << "Thousands separator: " << np.thousands_sep() << std::endl;
    
    // 数字格式化
    std::ostringstream oss;
    oss.imbue(loc);
    oss << std::fixed << std::setprecision(2) << 1234567.89;
    std::cout << "Formatted number: " << oss.str() << std::endl;
    
    // 数字解析
    std::string number_str = "1,234,567.89";
    std::istringstream iss(number_str);
    iss.imbue(loc);
    double parsed_number;
    iss >> parsed_number;
    std::cout << "Parsed number: " << parsed_number << std::endl;
}

void locale_aware_streaming() {
    // 创建不同语言环境的流
    std::cout.imbue(std::locale("en_US.UTF-8"));
    std::cout << "US format: " << 1234567.89 << std::endl;
    
    // 注意：某些系统可能不支持所有语言环境
    try {
        std::cout.imbue(std::locale("de_DE.UTF-8"));
        std::cout << "German format: " << 1234567.89 << std::endl;
    } catch (const std::runtime_error&) {
        std::cout << "German locale not available" << std::endl;
    }
    
    // 恢复默认语言环境
    std::cout.imbue(std::locale::classic());
    std::cout << "Default format: " << 1234567.89 << std::endl;
}
```

### 四、时间和货币处理

#### 1. 时间面的使用
```cpp
#include <locale>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

void time_formatting_examples() {
    std::locale loc("en_US.UTF-8");
    
    // 获取当前时间
    std::time_t t = std::time(nullptr);
    std::tm* tm_ptr = std::localtime(&t);
    
    // 使用time_put面格式化时间
    const std::time_put<char>& tp = std::use_facet<std::time_put<char>>(loc);
    
    std::ostringstream oss;
    oss.imbue(loc);
    
    // 格式化为完整日期时间
    std::string format = "%A, %B %d, %Y %I:%M:%S %p";
    tp.put(oss, oss, ' ', tm_ptr, format.data(), format.data() + format.length());
    std::cout << "Formatted time: " << oss.str() << std::endl;
    
    // 简单时间格式
    oss.str("");
    std::string simple_format = "%x %X";  // 本地日期和时间
    tp.put(oss, oss, ' ', tm_ptr, simple_format.data(), simple_format.data() + simple_format.length());
    std::cout << "Simple format: " << oss.str() << std::endl;
}

void time_parsing_examples() {
    std::locale loc("en_US.UTF-8");
    const std::time_get<char>& tg = std::use_facet<std::time_get<char>>(loc);
    
    std::string time_str = "Monday, January 15, 2024 03:30:45 PM";
    std::istringstream iss(time_str);
    iss.imbue(loc);
    
    std::tm parsed_tm = {};
    std::ios_base::iostate state;
    
    tg.get(iss, std::string(), iss, state, &parsed_tm, 
           "%A, %B %d, %Y %I:%M:%S %p");
    
    if (state == std::ios_base::goodbit) {
        std::cout << "Parsed successfully:" << std::endl;
        std::cout << "Year: " << parsed_tm.tm_year + 1900 << std::endl;
        std::cout << "Month: " << parsed_tm.tm_mon + 1 << std::endl;
        std::cout << "Day: " << parsed_tm.tm_mday << std::endl;
        std::cout << "Hour: " << parsed_tm.tm_hour << std::endl;
        std::cout << "Minute: " << parsed_tm.tm_min << std::endl;
        std::cout << "Second: " << parsed_tm.tm_sec << std::endl;
    } else {
        std::cout << "Parsing failed" << std::endl;
    }
}
```

#### 2. 货币面的使用
```cpp
#include <locale>
#include <iostream>
#include <sstream>
#include <iomanip>

void monetary_formatting_examples() {
    std::locale loc("en_US.UTF-8");
    const std::moneypunct<char>& mp = std::use_facet<std::moneypunct<char>>(loc);
    
    std::cout << "Currency symbol: " << mp.curr_symbol() << std::endl;
    std::cout << "Decimal point: " << mp.decimal_point() << std::endl;
    std::cout << "Thousands separator: " << mp.thousands_sep() << std::endl;
    std::cout << "Positive sign: '" << mp.positive_sign() << "'" << std::endl;
    std::cout << "Negative sign: '" << mp.negative_sign() << "'" << std::endl;
    
    // 货币格式化
    std::ostringstream oss;
    oss.imbue(loc);
    oss << std::showbase << std::put_money(123456);  // 1234.56
    std::cout << "Formatted currency: " << oss.str() << std::endl;
}

void monetary_parsing_examples() {
    std::locale loc("en_US.UTF-8");
    
    std::string currency_str = "$1,234.56";
    std::istringstream iss(currency_str);
    iss.imbue(loc);
    
    long double parsed_amount;
    iss >> std::get_money(parsed_amount);
    
    if (iss) {
        std::cout << "Parsed currency amount: " << parsed_amount << std::endl;
        std::cout << "In dollars: " << parsed_amount / 100.0 << std::endl;
    } else {
        std::cout << "Currency parsing failed" << std::endl;
    }
}
```

### 五、编码转换

#### 1. 字符编码转换（C++11-C++26，已弃用但仍可演示）
```cpp
#include <locale>
#include <codecvt>
#include <string>
#include <iostream>

void encoding_conversion_examples() {
    // UTF-8到UTF-16转换
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    
    std::string utf8_string = u8"Hello, 世界!";  // 包含中文字符
    std::u16string utf16_string = converter.from_bytes(utf8_string);
    std::string back_to_utf8 = converter.to_bytes(utf16_string);
    
    std::cout << "Original UTF-8: " << utf8_string << std::endl;
    std::cout << "Converted back: " << back_to_utf8 << std::endl;
    std::cout << "UTF-16 length: " << utf16_string.length() << std::endl;
    
    // UTF-8到宽字符转换
    std::wstring_convert<std::codecvt_utf8<wchar_t>> wconverter;
    std::wstring wide_string = wconverter.from_bytes(utf8_string);
    std::string wide_back = wconverter.to_bytes(wide_string);
    
    std::wcout << L"Wide string: " << wide_string << std::endl;
    std::cout << "Wide converted back: " << wide_back << std::endl;
}
```

### 六、C库本地化函数

#### 1. C风格本地化
```cpp
#include <clocale>
#include <cstdio>
#include <ctime>
#include <iostream>

void c_locale_examples() {
    // 保存当前语言环境
    char* old_locale = std::setlocale(LC_ALL, nullptr);
    std::cout << "Current locale: " << (old_locale ? old_locale : "C") << std::endl;
    
    // 设置为系统默认语言环境
    char* new_locale = std::setlocale(LC_ALL, "");
    std::cout << "New locale: " << (new_locale ? new_locale : "C") << std::endl;
    
    // 设置为特定语言环境
    char* german_locale = std::setlocale(LC_TIME, "de_DE.UTF-8");
    if (german_locale) {
        std::cout << "German time locale: " << german_locale << std::endl;
    } else {
        std::cout << "German locale not available" << std::endl;
    }
    
    // 时间格式化
    std::time_t now = std::time(nullptr);
    std::cout << "Time in current locale: " << std::ctime(&now);
    
    // 数字和货币格式信息
    struct std::lconv* lc = std::localeconv();
    std::cout << "Decimal point: " << lc->decimal_point << std::endl;
    std::cout << "Thousands separator: " << lc->thousands_sep << std::endl;
    std::cout << "Currency symbol: " << lc->int_curr_symbol << std::endl;
    
    // 恢复原语言环境
    std::setlocale(LC_ALL, old_locale ? old_locale : "C");
}
```

### 七、实际应用场景

#### 1. 国际化应用程序
```cpp
#include <locale>
#include <iostream>
#include <string>
#include <map>

class InternationalizedApp {
private:
    std::map<std::string, std::locale> supported_locales_;
    std::locale current_locale_;
    
public:
    InternationalizedApp() {
        // 初始化支持的语言环境
        try {
            supported_locales_["en"] = std::locale("en_US.UTF-8");
            supported_locales_["de"] = std::locale("de_DE.UTF-8");
            supported_locales_["fr"] = std::locale("fr_FR.UTF-8");
        } catch (const std::runtime_error& e) {
            // 如果某些语言环境不可用，使用默认语言环境
            std::cerr << "Warning: Some locales not available: " << e.what() << std::endl;
            supported_locales_["en"] = std::locale::classic();
        }
        current_locale_ = supported_locales_["en"];
    }
    
    bool setLocale(const std::string& lang_code) {
        auto it = supported_locales_.find(lang_code);
        if (it != supported_locales_.end()) {
            current_locale_ = it->second;
            std::cout.imbue(current_locale_);
            return true;
        }
        return false;
    }
    
    void printNumber(double value) {
        std::cout << "Number: " << value << std::endl;
    }
    
    void printCurrency(long double amount) {
        std::cout << "Amount: ";
        std::cout << std::showbase << std::put_money(static_cast<long long>(amount * 100)) << std::endl;
    }
    
    void printDate() {
        std::time_t now = std::time(nullptr);
        std::tm* local_time = std::localtime(&now);
        
        const std::time_put<char>& tp = std::use_facet<std::time_put<char>>(current_locale_);
        std::ostringstream oss;
        oss.imbue(current_locale_);
        
        std::string format = "%x";  // 本地日期格式
        tp.put(oss, oss, ' ', local_time, format.data(), format.data() + format.length());
        std::cout << "Date: " << oss.str() << std::endl;
    }
};

void internationalization_example() {
    InternationalizedApp app;
    
    // 英语环境
    std::cout << "=== English ===" << std::endl;
    app.printNumber(1234567.89);
    app.printCurrency(1234.56);
    app.printDate();
    
    // 尝试德语环境
    if (app.setLocale("de")) {
        std::cout << "\n=== German ===" << std::endl;
        app.printNumber(1234567.89);
        app.printCurrency(1234.56);
        app.printDate();
    } else {
        std::cout << "\nGerman locale not available" << std::endl;
    }
}
```

#### 2. 字符串排序和比较
```cpp
#include <locale>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

void locale_aware_sorting() {
    std::vector<std::string> words = {
        "äpple", "Österreich", "Zürich", "apple", "zebra", "Ägypten"
    };
    
    std::cout << "Original order:" << std::endl;
    for (const auto& word : words) {
        std::cout << "  " << word << std::endl;
    }
    
    // 使用默认C语言环境排序
    std::sort(words.begin(), words.end());
    std::cout << "\nSorted with C locale:" << std::endl;
    for (const auto& word : words) {
        std::cout << "  " << word << std::endl;
    }
    
    // 使用系统默认语言环境排序
    try {
        std::locale system_locale("");
        const std::collate<char>& coll = std::use_facet<std::collate<char>>(system_locale);
        
        std::sort(words.begin(), words.end(), 
                 [&coll](const std::string& a, const std::string& b) {
                     return coll.compare(a.data(), a.data() + a.length(),
                                       b.data(), b.data() + b.length()) < 0;
                 });
        
        std::cout << "\nSorted with system locale:" << std::endl;
        for (const auto& word : words) {
            std::cout << "  " << word << std::endl;
        }
    } catch (const std::runtime_error& e) {
        std::cout << "System locale sorting failed: " << e.what() << std::endl;
    }
}
```

## 最佳实践建议

### 一、语言环境使用准则

1. **合理选择语言环境**：根据应用需求选择合适的语言环境
2. **错误处理**：处理语言环境不可用的情况
3. **性能考虑**：避免频繁创建和切换语言环境
4. **一致性**：在整个应用中保持语言环境使用的一致性

### 二、字符处理建议

1. **使用语言环境感知函数**：优先使用`std::locale`版本的字符分类函数
2. **编码一致性**：确保字符编码在整个应用中保持一致
3. **国际化支持**：设计时考虑多语言支持
4. **安全处理**：正确处理特殊字符和多字节字符

### 三、数据格式化建议

1. **用户友好格式**：使用用户熟悉的数据格式
2. **本地化显示**：根据用户语言环境显示数据
3. **输入验证**：验证用户输入的数据格式
4. **错误恢复**：提供清晰的错误信息和恢复机制

## 相关页面

| 页面 | 说明 |
|------|------|
| 字符串库 | 字符串本地化支持 |
| 流I/O库 | 本地化流操作 |
| 正则表达式库 | 本地化正则表达式 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/locale&oldid=179015>
- 最后修改时间：2025年1月2日 19:48
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++本地化库为开发国际化应用程序提供了强大的支持。从基础的语言环境管理到复杂的字符编码转换，从简单的字符分类到高级的货币和时间格式化，本地化库帮助开发者创建能够适应不同文化和语言环境的应用程序。正确理解和使用本地化库特性对于开发现代国际化应用程序至关重要。通过合理使用语言环境面，开发者可以实现灵活的本地化支持，同时保持代码的可维护性和性能。持续学习和实践本地化技术，结合实际应用场景，能够构建出真正国际化的高质量应用程序。