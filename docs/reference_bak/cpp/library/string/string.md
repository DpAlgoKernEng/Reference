# C++ 字符串库（Strings library）

来源：cppreference.com

## 字符串库概述

C++字符串库提供了处理和操作字符序列的组件，包括字符串类、字符串视图类和字符特征类。

## 核心组件

### 一、主要类

| 类 | 说明 |
|----|------|
| `basic_string` | 基础字符串类模板 |
| `basic_string_view`(C++17) | 基础字符串视图类模板 |
| `char_traits` | 字符特征类模板 |

## 字符概念

在C++标准库中，**字符**是当按顺序处理时可以表示文本的对象。

这个术语不仅指字符类型的对象，还包括任何可以通过字符串库和以下库中指定的定义来表示的值：

- 本地化库
- 输入/输出库
- 正则表达式库（C++11起）

在字符串库和正则表达式库中，字符只能是**类字符类型**，即满足以下要求的非数组类型：
- C++20前：PODType
- C++20-C++26：TrivialType和StandardLayoutType
- C++26起：TriviallyCopyable和StandardLayoutType

对于任何类字符类型`T`，`std::is_trivially_default_constructible_v<T>`为true（C++26起）。

因此，在字符串库和正则表达式库中，字符也被称为**类字符对象**。

一些标准库组件接受**字符容器类型**，它们也是用于表示单个字符的类型，用作`std::char_traits`和使用`std::char_traits`的类模板的模板参数之一。

## 库组件详解

### 一、字符特征（Character Traits）

许多字符相关的类模板（如`std::basic_string`）需要一组相关的类型和函数来完成其语义定义。这些类型和函数作为成员typedef名称和函数集合在每个此类模板使用的模板参数`Traits`中提供。能够完成这些语义的类称为CharTraits。

字符串库提供了类模板`std::char_traits`，它为`std::basic_string`和`std::basic_string_view`（C++17起）定义类型和函数。

提供以下特化，它们都满足CharTraits要求：

| 定义位置 | 特化 |
|----------|------|
| `<string>`头文件 | |
| `template<> class char_traits<char>;` | |
| `template<> class char_traits<wchar_t>;` | |
| `template<> class char_traits<char8_t>;` | (C++20起) |
| `template<> class char_traits<char16_t>;` | (C++11起) |
| `template<> class char_traits<char32_t>;` | (C++11起) |

当为`std::basic_string`和`std::basic_string_view`（C++17起）使用用户定义的字符容器类型时，还需要提供相应的字符特征类（可以是`std::char_traits`的特化）。

### 二、字符串类（std::string等）

类模板`std::basic_string`概括了字符序列的操作和存储方式。字符串的创建、操作和销毁都由一组便利的类方法和相关函数处理。

为常用类型提供了以下`std::basic_string`特化：

| 定义位置 | 类型 | 定义 |
|----------|------|------|
| `<string>`头文件 | |
| `std::string` | `std::basic_string<char>` |
| `std::wstring` | `std::basic_string<wchar_t>` |
| `std::u8string`(C++20) | `std::basic_string<char8_t>` |
| `std::u16string`(C++11) | `std::basic_string<char16_t>` |
| `std::u32string`(C++11) | `std::basic_string<char32_t>` |

### 三、字符串视图类（std::string_view等）（C++17起）

类模板`std::basic_string_view`提供了一个轻量级对象，使用类似`std::basic_string`的接口提供对字符串或字符串部分的只读访问。

为常用类型提供了以下`std::basic_string_view`特化：

| 定义位置 | 类型 | 定义 |
|----------|------|------|
| `<string_view>`头文件 | |
| `std::string_view` | `std::basic_string_view<char>` |
| `std::wstring_view` | `std::basic_string_view<wchar_t>` |
| `std::u8string_view`(C++20) | `std::basic_string_view<char8_t>` |
| `std::u16string_view` | `std::basic_string_view<char16_t>` |
| `std::u32string_view` | `std::basic_string_view<char32_t>` |

## 相关库

文本处理库提供以下支持：
- 本地化支持
- 字符串转换（如`std::toupper`）
- 字符分类函数（如`std::isspace`）
- 文本编码识别（`std::text_encoding`）

---

## 扩展知识详解

### 一、字符特征详解

#### 1. char_traits基础使用
```cpp
#include <string>
#include <iostream>

void char_traits_examples() {
    // char_traits提供了字符操作的基本函数
    using traits = std::char_traits<char>;
    
    char c1 = 'A';
    char c2 = 'B';
    
    // 字符比较
    std::cout << "Compare 'A' and 'B': " << traits::compare(&c1, &c2, 1) << std::endl;
    
    // 字符赋值
    char c3;
    traits::assign(c3, c1);
    std::cout << "Assigned character: " << c3 << std::endl;
    
    // 字符移动
    char buffer[10] = "Hello";
    traits::move(buffer + 1, buffer, 5);
    std::cout << "After move: " << buffer << std::endl;
    
    // 字符复制
    char dest[6];
    traits::copy(dest, "World", 5);
    dest[5] = '\0';
    std::cout << "Copied string: " << dest << std::endl;
    
    // 字符查找
    const char* str = "Hello World";
    const char* found = traits::find(str, 11, 'o');
    if (found) {
        std::cout << "Found 'o' at position: " << (found - str) << std::endl;
    }
}
```

#### 2. 自定义字符特征
```cpp
#include <string>
#include <iostream>
#include <cctype>

// 自定义字符特征类：忽略大小写的字符比较
struct case_insensitive_traits : public std::char_traits<char> {
    // 重写比较函数实现忽略大小写
    static bool eq(char c1, char c2) {
        return std::tolower(c1) == std::tolower(c2);
    }
    
    static bool lt(char c1, char c2) {
        return std::tolower(c1) < std::tolower(c2);
    }
    
    static int compare(const char* s1, const char* s2, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            char lc1 = std::tolower(s1[i]);
            char lc2 = std::tolower(s2[i]);
            if (lc1 < lc2) return -1;
            if (lc1 > lc2) return 1;
        }
        return 0;
    }
    
    static const char* find(const char* s, size_t n, char a) {
        for (size_t i = 0; i < n; ++i) {
            if (std::tolower(s[i]) == std::tolower(a)) {
                return &s[i];
            }
        }
        return nullptr;
    }
};

void custom_char_traits_example() {
    // 使用自定义字符特征的字符串
    using ci_string = std::basic_string<char, case_insensitive_traits>;
    
    ci_string str1 = "Hello";
    ci_string str2 = "HELLO";
    
    std::cout << "Case insensitive comparison:" << std::endl;
    std::cout << "str1 == str2: " << (str1 == str2) << std::endl;
    std::cout << "str1 < str2: " << (str1 < str2) << std::endl;
}
```

### 二、字符串类详解

#### 1. basic_string基础操作
```cpp
#include <string>
#include <iostream>
#include <algorithm>

void basic_string_operations() {
    // 字符串构造
    std::string str1 = "Hello";
    std::string str2("World");
    std::string str3(5, 'A');  // "AAAAA"
    std::string str4(str1, 1, 3);  // "ell"
    
    std::cout << "str1: " << str1 << std::endl;
    std::cout << "str2: " << str2 << std::endl;
    std::cout << "str3: " << str3 << std::endl;
    std::cout << "str4: " << str4 << std::endl;
    
    // 字符串连接
    std::string combined = str1 + " " + str2 + "!";
    std::cout << "Combined: " << combined << std::endl;
    
    // 字符串修改
    str1 += " World";
    std::cout << "Modified str1: " << str1 << std::endl;
    
    // 字符串查找
    size_t pos = str1.find("World");
    if (pos != std::string::npos) {
        std::cout << "Found 'World' at position: " << pos << std::endl;
    }
    
    // 字符串替换
    str1.replace(pos, 5, "C++");
    std::cout << "After replacement: " << str1 << std::endl;
    
    // 字符串子串
    std::string substr = str1.substr(0, 5);
    std::cout << "Substring: " << substr << std::endl;
    
    // 字符串迭代
    std::cout << "Characters: ";
    for (char c : str1) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

void string_capacity_operations() {
    std::string str = "Hello";
    
    std::cout << "Initial string: " << str << std::endl;
    std::cout << "Size: " << str.size() << std::endl;
    std::cout << "Length: " << str.length() << std::endl;
    std::cout << "Capacity: " << str.capacity() << std::endl;
    std::cout << "Empty: " << str.empty() << std::endl;
    
    // 预分配容量
    str.reserve(100);
    std::cout << "After reserve(100): " << std::endl;
    std::cout << "Capacity: " << str.capacity() << std::endl;
    
    // 收缩到实际大小
    str.shrink_to_fit();
    std::cout << "After shrink_to_fit(): " << std::endl;
    std::cout << "Capacity: " << str.capacity() << std::endl;
    
    // 清空字符串
    str.clear();
    std::cout << "After clear(): " << std::endl;
    std::cout << "Size: " << str.size() << std::endl;
    std::cout << "Empty: " << str.empty() << std::endl;
}
```

#### 2. 不同字符类型的字符串
```cpp
#include <string>
#include <iostream>

void different_string_types() {
    // 窄字符字符串
    std::string narrow_str = "Hello World";
    std::cout << "Narrow string: " << narrow_str << std::endl;
    
    // 宽字符字符串
    std::wstring wide_str = L"Hello Wide World";
    std::wcout << L"Wide string: " << wide_str << std::endl;
    
    // UTF-8字符串（C++20）
    std::u8string utf8_str = u8"Hello UTF-8 World";
    std::cout << "UTF-8 string length: " << utf8_str.length() << std::endl;
    
    // UTF-16字符串
    std::u16string utf16_str = u"Hello UTF-16 World";
    std::cout << "UTF-16 string length: " << utf16_str.length() << std::endl;
    
    // UTF-32字符串
    std::u32string utf32_str = U"Hello UTF-32 World";
    std::cout << "UTF-32 string length: " << utf32_str.length() << std::endl;
}
```

### 三、字符串视图详解（C++17起）

#### 1. basic_string_view基础使用
```cpp
#include <string_view>
#include <string>
#include <iostream>

void string_view_examples() {
    // 从字符串创建string_view
    std::string str = "Hello, World!";
    std::string_view sv1(str);
    std::cout << "String view 1: " << sv1 << std::endl;
    
    // 从字符串字面量创建
    std::string_view sv2 = "Hello, C++!";
    std::cout << "String view 2: " << sv2 << std::endl;
    
    // 从字符数组创建
    const char* cstr = "Hello, String View!";
    std::string_view sv3(cstr, 5);  // 只取前5个字符
    std::cout << "String view 3: " << sv3 << std::endl;
    
    // string_view操作
    std::cout << "Length: " << sv1.length() << std::endl;
    std::cout << "Empty: " << sv1.empty() << std::endl;
    std::cout << "First char: " << sv1.front() << std::endl;
    std::cout << "Last char: " << sv1.back() << std::endl;
    std::cout << "Char at index 7: " << sv1[7] << std::endl;
    
    // 查找操作
    size_t pos = sv1.find("World");
    if (pos != std::string_view::npos) {
        std::cout << "Found 'World' at position: " << pos << std::endl;
    }
    
    // 子串操作
    std::string_view substr = sv1.substr(7, 5);
    std::cout << "Substring: " << substr << std::endl;
    
    // 比较操作
    std::string_view sv4 = "Hello";
    std::string_view sv5 = "World";
    std::cout << "sv4 < sv5: " << (sv4 < sv5) << std::endl;
    std::cout << "sv4 compare sv5: " << sv4.compare(sv5) << std::endl;
}

void string_view_performance() {
    // string_view的性能优势：避免不必要的字符串复制
    
    // 低效方式：创建子字符串
    std::string original = "This is a very long string for demonstration purposes";
    std::string substr = original.substr(10, 15);  // 创建新字符串
    std::cout << "Substring (copy): " << substr << std::endl;
    
    // 高效方式：使用string_view
    std::string_view sv(original.data() + 10, 15);  // 只是视图，无复制
    std::cout << "String view (no copy): " << sv << std::endl;
    
    // 函数参数使用string_view
    auto process_text = [](std::string_view text) {
        std::cout << "Processing: " << text << " (length: " << text.length() << ")" << std::endl;
    };
    
    process_text("Direct literal");           // 直接使用字面量
    process_text(original);                   // 使用string
    process_text(substr);                     // 使用substring
}
```

#### 2. 不同类型的string_view
```cpp
#include <string_view>
#include <iostream>

void different_string_view_types() {
    // 窄字符字符串视图
    std::string_view narrow_sv = "Hello World";
    std::cout << "Narrow string view: " << narrow_sv << std::endl;
    
    // 宽字符字符串视图
    std::wstring_view wide_sv = L"Hello Wide World";
    std::wcout << L"Wide string view: " << wide_sv << std::endl;
    
    // UTF-8字符串视图（C++20）
    std::u8string_view utf8_sv = u8"Hello UTF-8 World";
    std::cout << "UTF-8 string view length: " << utf8_sv.length() << std::endl;
    
    // UTF-16字符串视图
    std::u16string_view utf16_sv = u"Hello UTF-16 World";
    std::cout << "UTF-16 string view length: " << utf16_sv.length() << std::endl;
    
    // UTF-32字符串视图
    std::u32string_view utf32_sv = U"Hello UTF-32 World";
    std::cout << "UTF-32 string view length: " << utf32_sv.length() << std::endl;
}
```

### 四、实际应用场景

#### 1. 函数参数优化
```cpp
#include <string>
#include <string_view>
#include <iostream>

// 使用string_view作为参数可以接受string、const char*等
void print_message(std::string_view message) {
    std::cout << "Message: " << message << " (length: " << message.length() << ")" << std::endl;
}

// 返回string_view避免不必要的字符串复制
std::string_view get_greeting() {
    static const char* greeting = "Hello, World!";
    return greeting;  // 返回视图，无复制
}

void function_parameter_examples() {
    // 可以传递不同类型的参数
    print_message("Direct literal");
    print_message(std::string("String object"));
    
    std::string str = "Variable string";
    print_message(str);
    
    std::string_view sv = "String view";
    print_message(sv);
    
    // 获取问候语
    std::string_view greeting = get_greeting();
    print_message(greeting);
}
```

#### 2. 字符串处理工具
```cpp
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <iostream>

class StringProcessor {
public:
    // 分割字符串
    static std::vector<std::string_view> split(std::string_view text, char delimiter) {
        std::vector<std::string_view> result;
        size_t start = 0;
        
        while (start < text.length()) {
            size_t end = text.find(delimiter, start);
            if (end == std::string_view::npos) {
                end = text.length();
            }
            
            result.emplace_back(text.substr(start, end - start));
            start = end + 1;
        }
        
        return result;
    }
    
    // 去除前后空白字符
    static std::string_view trim(std::string_view text) {
        size_t start = text.find_first_not_of(" \t\n\r");
        if (start == std::string_view::npos) {
            return "";  // 全是空白字符
        }
        
        size_t end = text.find_last_not_of(" \t\n\r");
        return text.substr(start, end - start + 1);
    }
    
    // 检查是否以指定前缀开始
    static bool starts_with(std::string_view text, std::string_view prefix) {
        return text.length() >= prefix.length() && 
               text.substr(0, prefix.length()) == prefix;
    }
    
    // 检查是否以指定后缀结束
    static bool ends_with(std::string_view text, std::string_view suffix) {
        return text.length() >= suffix.length() && 
               text.substr(text.length() - suffix.length()) == suffix;
    }
};

void string_processing_examples() {
    // 分割示例
    std::string text = "apple,banana,cherry,date";
    auto parts = StringProcessor::split(text, ',');
    std::cout << "Split parts: ";
    for (const auto& part : parts) {
        std::cout << "[" << part << "] ";
    }
    std::cout << std::endl;
    
    // 去除空白字符
    std::string_view with_spaces = "  Hello, World!  ";
    std::string_view trimmed = StringProcessor::trim(with_spaces);
    std::cout << "Original: '" << with_spaces << "'" << std::endl;
    std::cout << "Trimmed: '" << trimmed << "'" << std::endl;
    
    // 前缀和后缀检查
    std::string_view url = "https://www.example.com";
    std::cout << "Starts with 'https': " 
              << StringProcessor::starts_with(url, "https://") << std::endl;
    std::cout << "Ends with '.com': " 
              << StringProcessor::ends_with(url, ".com") << std::endl;
}
```

### 五、性能优化技巧

#### 1. 避免不必要的字符串复制
```cpp
#include <string>
#include <string_view>
#include <chrono>
#include <iostream>
#include <vector>

void performance_comparison() {
    // 大字符串
    std::string large_string(10000, 'A');
    
    // 测试字符串复制的性能
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> copies;
    for (int i = 0; i < 1000; ++i) {
        copies.push_back(large_string.substr(100, 50));  // 创建子字符串（复制）
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto copy_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 测试string_view的性能
    start = std::chrono::high_resolution_clock::now();
    std::vector<std::string_view> views;
    for (int i = 0; i < 1000; ++i) {
        views.emplace_back(large_string.data() + 100, 50);  // 创建视图（无复制）
    }
    end = std::chrono::high_resolution_clock::now();
    auto view_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "String copy time: " << copy_duration.count() << " microseconds" << std::endl;
    std::cout << "String view time: " << view_duration.count() << " microseconds" << std::endl;
    std::cout << "Performance improvement: " 
              << static_cast<double>(copy_duration.count()) / view_duration.count() 
              << "x faster" << std::endl;
}
```

#### 2. 字符串构建优化
```cpp
#include <string>
#include <sstream>
#include <iostream>

void string_building_optimization() {
    // 低效方式：多次字符串连接
    auto inefficient_way = []() {
        std::string result;
        for (int i = 0; i < 1000; ++i) {
            result += "Item " + std::to_string(i) + "\n";
        }
        return result;
    };
    
    // 高效方式：使用stringstream
    auto efficient_way = []() {
        std::ostringstream oss;
        for (int i = 0; i < 1000; ++i) {
            oss << "Item " << i << "\n";
        }
        return oss.str();
    };
    
    // 现代C++高效方式：预分配
    auto modern_way = []() {
        std::string result;
        result.reserve(1000 * 20);  // 预估大小
        for (int i = 0; i < 1000; ++i) {
            result += "Item " + std::to_string(i) + "\n";
        }
        return result;
    };
    
    // 测试性能
    auto start = std::chrono::high_resolution_clock::now();
    auto result1 = inefficient_way();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    start = std::chrono::high_resolution_clock::now();
    auto result2 = efficient_way();
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    start = std::chrono::high_resolution_clock::now();
    auto result3 = modern_way();
    end = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Inefficient way: " << duration1.count() << " microseconds" << std::endl;
    std::cout << "Efficient way (stringstream): " << duration2.count() << " microseconds" << std::endl;
    std::cout << "Modern way (reserve): " << duration3.count() << " microseconds" << std::endl;
}
```

## 最佳实践建议

### 一、字符串使用准则

1. **优先使用string_view**：函数参数使用`string_view`可以接受多种字符串类型
2. **避免不必要的复制**：使用`string_view`避免创建不必要的字符串副本
3. **合理预分配**：大量字符串操作前使用`reserve`预分配内存
4. **选择合适类型**：根据字符编码需求选择合适的字符串类型

### 二、性能优化建议

1. **使用移动语义**：适当使用`std::move`避免不必要的复制
2. **批量操作**：使用`append`、`assign`等批量操作而非逐字符操作
3. **避免频繁重分配**：预估字符串大小并使用`reserve`
4. **利用string_view**：只读场景优先使用`string_view`

### 三、编码和国际化

1. **统一编码**：项目中统一使用UTF-8编码
2. **国际化支持**：使用本地化库处理多语言文本
3. **宽字符处理**：正确处理宽字符和多字节字符
4. **编码转换**：需要时进行正确的编码转换

## 相关页面

| 页面 | 说明 |
|------|------|
| 本地化库 | 字符串本地化支持 |
| 正则表达式库 | 字符串模式匹配 |
| 输入/输出库 | 字符串I/O操作 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/string&oldid=179902>
- 最后修改时间：2025年1月28日 11:14
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++字符串库为现代C++开发提供了全面的字符串处理能力。从基础的`std::string`到高效的`std::string_view`，从通用的字符特征到特定编码的字符串类型，这些组件帮助开发者处理各种字符串操作需求。正确理解和使用这些特性是掌握现代C++字符串处理的关键。`string_view`的引入显著提升了性能，通过避免不必要的字符串复制实现了高效的字符串操作。字符特征类提供了灵活的字符比较和操作机制，支持自定义字符串行为。持续学习和实践这些字符串处理技术，结合性能分析和最佳实践，能够构建出高效且正确的字符串处理应用程序。