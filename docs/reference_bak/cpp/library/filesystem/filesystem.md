# C++文件系统库（Filesystem Library）（C++17起）

来源：cppreference.com

## 文件系统库概述

文件系统库提供了对文件系统及其组件（如路径、普通文件和目录）执行操作的工具。

文件系统库最初作为boost.filesystem开发，发布为技术规范ISO/IEC TS 18822:2015，最终在C++17中合并到ISO C++。Boost实现目前在比C++17库更多的编译器和平台上可用。

如果实现无法访问分层文件系统，或者不提供必要功能，文件系统库工具可能不可用。如果底层文件系统不支持某些功能（例如FAT文件系统缺少符号链接且禁止多个硬链接），某些功能可能不可用。在这些情况下，必须报告错误。

如果调用此库中的函数引入**文件系统竞争**，即多个线程、进程或计算机交错访问和修改文件系统中的同一对象，则行为未定义。

## 库级定义

### 一、文件类型定义

- **文件**：持有数据的文件系统对象，可以写入、读取或两者兼有。文件有名称，属性之一是文件类型：

  - **目录**：作为目录条目容器的文件，这些条目标识其他文件（其中一些可能是其他嵌套目录）。讨论特定文件时，它作为条目出现的目录是其**父目录**。父目录可以用相对路径名".."表示。
  - **普通文件**：将名称与现有文件关联的目录条目（即**硬链接**）。如果支持多个硬链接，则在最后一个硬链接被删除后删除文件。
  - **符号链接**：将名称与路径关联的目录条目，该路径可能存在也可能不存在。
  - 其他特殊文件类型：**块设备**、**字符设备**、**命名管道**、**套接字**。

- **文件名**：命名文件的字符串。允许的字符、大小写敏感性、最大长度和禁止的名称由实现定义。名称"."（点）和".."（点点）在库级别有特殊含义。

- **路径**：标识文件的元素序列。它以可选的根名称（例如Windows上的"C:"或"//server"）开头，后跟可选的根目录（例如Unix上的"/"），然后是零个或多个文件名序列（除最后一个外都必须是目录或目录链接）。路径字符串表示的本机格式（例如用作分隔符的字符）和字符编码由实现定义，此库提供路径的可移植表示。

  - **绝对路径**：明确标识文件位置的路径。
  - **规范路径**：不包含符号链接、"."或".."元素的绝对路径。
  - **相对路径**：相对于文件系统上某个位置标识文件位置的路径。特殊路径名"."（点，"当前目录"）和".."（点点，"父目录"）是相对路径。

## 主要组件

### 一、类（Classes）

| 类 | 说明 |
|-----|------|
| `path`(C++17) | 表示路径（类） |
| `filesystem_error`(C++17) | 文件系统错误时抛出的异常（类） |
| `directory_entry`(C++17) | 目录条目（类） |
| `directory_iterator`(C++17) | 目录内容的迭代器（类） |
| `recursive_directory_iterator`(C++17) | 目录及其子目录内容的迭代器（类） |
| `file_status`(C++17) | 表示文件类型和权限（类） |
| `space_info`(C++17) | 文件系统上可用和空闲空间的信息（类） |
| `file_type`(C++17) | 文件类型（枚举） |
| `perms`(C++17) | 标识文件系统权限（枚举） |
| `perm_options`(C++17) | 指定权限操作的语义（枚举） |
| `copy_options`(C++17) | 指定复制操作的语义（枚举） |
| `directory_options`(C++17) | 迭代目录内容的选项（枚举） |
| `file_time_type`(C++17) | 表示文件时间值（typedef） |

### 二、非成员函数（Non-member functions）

| 函数 | 说明 |
|------|------|
| `absolute`(C++17) | 组成绝对路径（函数） |
| `canonical`/`weakly_canonical`(C++17) | 组成规范路径（函数） |
| `relative`/`proximate`(C++17) | 组成相对路径（函数） |
| `copy`(C++17) | 复制文件或目录（函数） |
| `copy_file`(C++17) | 复制文件内容（函数） |
| `copy_symlink`(C++17) | 复制符号链接（函数） |
| `create_directory`/`create_directories`(C++17) | 创建新目录（函数） |
| `create_hard_link`(C++17) | 创建硬链接（函数） |
| `create_symlink`/`create_directory_symlink`(C++17) | 创建符号链接（函数） |
| `current_path`(C++17) | 返回或设置当前工作目录（函数） |
| `exists`(C++17) | 检查路径是否引用存在的文件系统对象（函数） |
| `equivalent`(C++17) | 检查两个路径是否引用相同的文件系统对象（函数） |
| `file_size`(C++17) | 返回文件大小（函数） |
| `hard_link_count`(C++17) | 返回引用特定文件的硬链接数（函数） |
| `last_write_time`(C++17) | 获取或设置最后数据修改时间（函数） |
| `permissions`(C++17) | 修改文件访问权限（函数） |
| `read_symlink`(C++17) | 获取符号链接的目标（函数） |
| `remove`/`remove_all`(C++17) | 删除文件或空目录 / 递归删除文件或目录及其所有内容（函数） |
| `rename`(C++17) | 移动或重命名文件或目录（函数） |
| `resize_file`(C++17) | 通过截断或零填充更改普通文件大小（函数） |
| `space`(C++17) | 确定文件系统上的可用空闲空间（函数） |
| `status`/`symlink_status`(C++17) | 确定文件属性 / 确定文件属性，检查符号链接目标（函数） |
| `temp_directory_path`(C++17) | 返回适合临时文件的目录（函数） |

### 三、文件类型检查函数

| 函数 | 说明 |
|------|------|
| `is_block_file`(C++17) | 检查给定路径是否引用块设备（函数） |
| `is_character_file`(C++17) | 检查给定路径是否引用字符设备（函数） |
| `is_directory`(C++17) | 检查给定路径是否引用目录（函数） |
| `is_empty`(C++17) | 检查给定路径是否引用空文件或目录（函数） |
| `is_fifo`(C++17) | 检查给定路径是否引用命名管道（函数） |
| `is_other`(C++17) | 检查参数是否引用**其他**文件（函数） |
| `is_regular_file`(C++17) | 检查参数是否引用普通文件（函数） |
| `is_socket`(C++17) | 检查参数是否引用命名IPC套接字（函数） |
| `is_symlink`(C++17) | 检查参数是否引用符号链接（函数） |
| `status_known`(C++17) | 检查文件状态是否已知（函数） |

---

## 扩展知识详解

### 一、path类深度解析

#### 1. 基础路径操作
```cpp
#include <filesystem>
#include <iostream>

void path_examples() {
    namespace fs = std::filesystem;
    
    // 创建路径
    fs::path p1("/home/user/documents/file.txt");
    fs::path p2("C:\\Users\\User\\Documents\\file.txt");
    
    // 路径属性
    std::cout << "Path: " << p1 << std::endl;
    std::cout << "Root name: " << p1.root_name() << std::endl;
    std::cout << "Root directory: " << p1.root_directory() << std::endl;
    std::cout << "Parent path: " << p1.parent_path() << std::endl;
    std::cout << "Filename: " << p1.filename() << std::endl;
    std::cout << "Stem: " << p1.stem() << std::endl;
    std::cout << "Extension: " << p1.extension() << std::endl;
    
    // 路径操作
    fs::path p3 = p1.parent_path() / "new_file.txt";
    std::cout << "Combined path: " << p3 << std::endl;
    
    // 路径比较
    fs::path p4("/home/user/documents");
    fs::path p5("/home/user/documents/");
    std::cout << "Paths equal: " << (p4 == p5) << std::endl;
}
```

#### 2. 路径转换和规范化
```cpp
#include <filesystem>
#include <iostream>

void path_transformation_examples() {
    namespace fs = std::filesystem;
    
    fs::path relative_path("../documents/file.txt");
    fs::path current = fs::current_path();
    
    // 绝对路径
    fs::path absolute = fs::absolute(relative_path);
    std::cout << "Absolute path: " << absolute << std::endl;
    
    // 规范路径
    try {
        fs::path canonical = fs::canonical(relative_path);
        std::cout << "Canonical path: " << canonical << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cout << "Canonical error: " << e.what() << std::endl;
    }
    
    // 相对路径
    fs::path relative = fs::relative(absolute, current);
    std::cout << "Relative path: " << relative << std::endl;
}
```

### 二、文件和目录操作

#### 1. 目录创建和管理
```cpp
#include <filesystem>
#include <iostream>

void directory_operations() {
    namespace fs = std::filesystem;
    
    try {
        // 创建单个目录
        fs::create_directory("test_dir");
        
        // 创建多级目录
        fs::create_directories("parent/child/grandchild");
        
        // 检查目录是否存在
        if (fs::exists("test_dir")) {
            std::cout << "Directory exists" << std::endl;
        }
        
        // 删除目录
        fs::remove("test_dir");
        
        // 递归删除目录
        fs::remove_all("parent");
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
```

#### 2. 文件操作
```cpp
#include <filesystem>
#include <fstream>
#include <iostream>

void file_operations() {
    namespace fs = std::filesystem;
    
    try {
        // 创建测试文件
        std::ofstream file("test.txt");
        file << "Hello, Filesystem!";
        file.close();
        
        // 文件信息
        if (fs::exists("test.txt")) {
            auto status = fs::status("test.txt");
            std::cout << "File size: " << fs::file_size("test.txt") << " bytes" << std::endl;
            std::cout << "Is regular file: " << fs::is_regular_file(status) << std::endl;
            std::cout << "Last write time: " << 
                std::chrono::duration_cast<std::chrono::seconds>(
                    fs::last_write_time("test.txt").time_since_epoch()
                ).count() << " seconds" << std::endl;
        }
        
        // 复制文件
        fs::copy_file("test.txt", "test_copy.txt");
        
        // 重命名文件
        fs::rename("test_copy.txt", "renamed.txt");
        
        // 调整文件大小
        fs::resize_file("test.txt", 100);  // 扩展到100字节
        
        // 删除文件
        fs::remove("test.txt");
        fs::remove("renamed.txt");
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "File operation error: " << e.what() << std::endl;
    }
}
```

#### 3. 符号链接操作
```cpp
#include <filesystem>
#include <iostream>

void symlink_operations() {
    namespace fs = std::filesystem;
    
    try {
        // 创建目标文件
        std::ofstream file("target.txt");
        file << "Target file content";
        file.close();
        
        // 创建符号链接
        fs::create_symlink("target.txt", "link_to_target");
        
        // 检查符号链接
        if (fs::is_symlink("link_to_target")) {
            std::cout << "Is symlink: true" << std::endl;
            std::cout << "Symlink target: " << fs::read_symlink("link_to_target") << std::endl;
        }
        
        // 复制符号链接
        fs::copy_symlink("link_to_target", "link_copy");
        
        // 删除符号链接
        fs::remove("link_to_target");
        fs::remove("link_copy");
        fs::remove("target.txt");
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "Symlink error: " << e.what() << std::endl;
    }
}
```

### 三、目录遍历

#### 1. 简单目录遍历
```cpp
#include <filesystem>
#include <iostream>

void simple_directory_traversal() {
    namespace fs = std::filesystem;
    
    try {
        // 创建测试目录结构
        fs::create_directories("test_root/subdir1/subdir2");
        std::ofstream("test_root/file1.txt") << "File 1";
        std::ofstream("test_root/subdir1/file2.txt") << "File 2";
        
        // 遍历目录
        std::cout << "Directory contents:" << std::endl;
        for (const auto& entry : fs::directory_iterator("test_root")) {
            std::cout << entry.path() << std::endl;
            if (fs::is_regular_file(entry)) {
                std::cout << "  (regular file)" << std::endl;
            } else if (fs::is_directory(entry)) {
                std::cout << "  (directory)" << std::endl;
            }
        }
        
        // 清理
        fs::remove_all("test_root");
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "Traversal error: " << e.what() << std::endl;
    }
}
```

#### 2. 递归目录遍历
```cpp
#include <filesystem>
#include <iostream>

void recursive_directory_traversal() {
    namespace fs = std::filesystem;
    
    try {
        // 创建测试目录结构
        fs::create_directories("recursive_test/level1/level2/level3");
        std::ofstream("recursive_test/file1.txt") << "Root file";
        std::ofstream("recursive_test/level1/file2.txt") << "Level 1 file";
        std::ofstream("recursive_test/level1/level2/file3.txt") << "Level 2 file";
        
        // 递归遍历
        std::cout << "Recursive directory traversal:" << std::endl;
        for (const auto& entry : fs::recursive_directory_iterator("recursive_test")) {
            // 根据递归深度缩进
            int depth = entry.depth();
            for (int i = 0; i < depth; ++i) {
                std::cout << "  ";
            }
            std::cout << entry.path().filename() << std::endl;
        }
        
        // 清理
        fs::remove_all("recursive_test");
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "Recursive traversal error: " << e.what() << std::endl;
    }
}
```

### 四、文件系统信息和权限

#### 1. 文件系统空间信息
```cpp
#include <filesystem>
#include <iostream>

void filesystem_space_info() {
    namespace fs = std::filesystem;
    
    try {
        // 获取文件系统空间信息
        fs::space_info space = fs::space(".");
        
        std::cout << "Filesystem space information:" << std::endl;
        std::cout << "Capacity: " << space.capacity << " bytes" << std::endl;
        std::cout << "Free: " << space.free << " bytes" << std::endl;
        std::cout << "Available: " << space.available << " bytes" << std::endl;
        
        // 转换为更易读的格式
        auto format_size = [](std::uintmax_t bytes) {
            const char* units[] = {"B", "KB", "MB", "GB", "TB"};
            int unit_index = 0;
            double size = static_cast<double>(bytes);
            
            while (size >= 1024.0 && unit_index < 4) {
                size /= 1024.0;
                unit_index++;
            }
            
            return std::to_string(size) + " " + units[unit_index];
        };
        
        std::cout << "Formatted capacity: " << format_size(space.capacity) << std::endl;
        std::cout << "Formatted free: " << format_size(space.free) << std::endl;
        std::cout << "Formatted available: " << format_size(space.available) << std::endl;
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "Space info error: " << e.what() << std::endl;
    }
}
```

#### 2. 文件权限管理
```cpp
#include <filesystem>
#include <iostream>

void permission_examples() {
    namespace fs = std::filesystem;
    
    try {
        // 创建测试文件
        std::ofstream file("perm_test.txt");
        file << "Permission test file";
        file.close();
        
        // 获取当前权限
        auto perms = fs::status("perm_test.txt").permissions();
        std::cout << "Current permissions: " << std::oct << static_cast<int>(perms) << std::endl;
        
        // 修改权限
        fs::permissions("perm_test.txt", fs::perms::owner_read | fs::perms::owner_write);
        
        // 添加权限
        fs::permissions("perm_test.txt", fs::perms::group_read, fs::perm_options::add);
        
        // 移除权限
        fs::permissions("perm_test.txt", fs::perms::others_all, fs::perm_options::remove);
        
        // 删除测试文件
        fs::remove("perm_test.txt");
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "Permission error: " << e.what() << std::endl;
    }
}
```

### 五、错误处理和异常

#### 1. 文件系统异常处理
```cpp
#include <filesystem>
#include <iostream>
#include <system_error>

void error_handling_examples() {
    namespace fs = std::filesystem;
    
    try {
        // 可能抛出异常的操作
        fs::create_directory("/root/forbidden");  // 需要管理员权限
        
    } catch (const fs::filesystem_error& e) {
        std::cout << "Filesystem error: " << e.what() << std::endl;
        std::cout << "Path1: " << e.path1() << std::endl;
        std::cout << "Path2: " << e.path2() << std::endl;
        std::cout << "Error code: " << e.code().message() << std::endl;
    }
    
    // 无异常的检查方式
    std::error_code ec;
    bool exists = fs::exists("/nonexistent/file", ec);
    if (ec) {
        std::cout << "Error checking existence: " << ec.message() << std::endl;
    } else {
        std::cout << "File exists: " << exists << std::endl;
    }
}
```

### 六、实际应用示例

#### 1. 文件系统监控工具
```cpp
#include <filesystem>
#include <chrono>
#include <iostream>
#include <map>

class FileSystemMonitor {
private:
    std::map<std::filesystem::path, std::filesystem::file_time_type> file_timestamps_;
    
public:
    void scan_directory(const std::filesystem::path& dir) {
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
                if (std::filesystem::is_regular_file(entry)) {
                    auto last_write = std::filesystem::last_write_time(entry);
                    file_timestamps_[entry.path()] = last_write;
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Scan error: " << e.what() << std::endl;
        }
    }
    
    void check_for_changes(const std::filesystem::path& dir) {
        try {
            std::map<std::filesystem::path, std::filesystem::file_time_type> current_files;
            
            for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
                if (std::filesystem::is_regular_file(entry)) {
                    auto last_write = std::filesystem::last_write_time(entry);
                    current_files[entry.path()] = last_write;
                    
                    auto it = file_timestamps_.find(entry.path());
                    if (it != file_timestamps_.end() && it->second != last_write) {
                        std::cout << "File changed: " << entry.path() << std::endl;
                    } else if (it == file_timestamps_.end()) {
                        std::cout << "New file: " << entry.path() << std::endl;
                    }
                }
            }
            
            // 检查删除的文件
            for (const auto& [path, timestamp] : file_timestamps_) {
                if (current_files.find(path) == current_files.end()) {
                    std::cout << "File deleted: " << path << std::endl;
                }
            }
            
            file_timestamps_ = std::move(current_files);
            
        } catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Change check error: " << e.what() << std::endl;
        }
    }
};
```

#### 2. 磁盘使用统计工具
```cpp
#include <filesystem>
#include <iostream>
#include <map>

class DiskUsageAnalyzer {
public:
    struct FileStats {
        std::uintmax_t total_size = 0;
        std::size_t file_count = 0;
        std::size_t dir_count = 0;
    };
    
    static FileStats analyze_directory(const std::filesystem::path& path) {
        FileStats stats;
        
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
                if (std::filesystem::is_regular_file(entry)) {
                    stats.total_size += std::filesystem::file_size(entry);
                    stats.file_count++;
                } else if (std::filesystem::is_directory(entry)) {
                    stats.dir_count++;
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Analysis error: " << e.what() << std::endl;
        }
        
        return stats;
    }
    
    static void print_file_type_statistics(const std::filesystem::path& path) {
        std::map<std::string, FileStats> type_stats;
        
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
                if (std::filesystem::is_regular_file(entry)) {
                    std::string extension = entry.path().extension().string();
                    if (extension.empty()) {
                        extension = "(no extension)";
                    }
                    
                    auto& stats = type_stats[extension];
                    stats.total_size += std::filesystem::file_size(entry);
                    stats.file_count++;
                }
            }
            
            std::cout << "File type statistics:" << std::endl;
            for (const auto& [ext, stats] : type_stats) {
                std::cout << ext << ": " << stats.file_count 
                         << " files, " << stats.total_size << " bytes" << std::endl;
            }
            
        } catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Type analysis error: " << e.what() << std::endl;
        }
    }
};
```

#### 3. 跨平台路径处理工具
```cpp
#include <filesystem>
#include <iostream>

class CrossPlatformPathHandler {
public:
    static std::filesystem::path normalize_path(const std::string& path) {
        // 处理不同平台的路径分隔符
        std::string normalized = path;
        
#ifdef _WIN32
        // Windows: 将Unix风格分隔符转换为Windows风格
        for (auto& ch : normalized) {
            if (ch == '/') {
                ch = '\\';
            }
        }
#else
        // Unix-like: 将Windows风格分隔符转换为Unix风格
        for (auto& ch : normalized) {
            if (ch == '\\') {
                ch = '/';
            }
        }
#endif
        
        return std::filesystem::path(normalized);
    }
    
    static std::string get_config_directory() {
#ifdef _WIN32
        const char* appdata = std::getenv("APPDATA");
        if (appdata) {
            return std::string(appdata) + "\\MyApp";
        }
        return "C:\\MyApp";
#else
        const char* home = std::getenv("HOME");
        if (home) {
            return std::string(home) + "/.myapp";
        }
        return "/tmp/myapp";
#endif
    }
    
    static void demonstrate_platform_paths() {
        std::cout << "Config directory: " << get_config_directory() << std::endl;
        std::cout << "Temp directory: " 
                  << std::filesystem::temp_directory_path() << std::endl;
        std::cout << "Current directory: " 
                  << std::filesystem::current_path() << std::endl;
    }
};
```

## 编译注意事项

### 一、编译器支持
```bash
# GNU编译器（9.1之前版本）
g++ -std=c++17 -lstdc++fs program.cpp

# LLVM编译器（9.0之前版本）
clang++ -std=c++17 -lc++fs program.cpp

# 新版本编译器通常不需要额外链接
g++ -std=c++17 program.cpp
```

### 二、特性测试宏
```cpp
#include <filesystem>

#if defined(__cpp_lib_filesystem) && __cpp_lib_filesystem >= 201703L
    // 文件系统库可用
    #include <filesystem>
    namespace fs = std::filesystem;
#else
    // 回退到实验性实现或其他实现
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif
```

## 性能和最佳实践

### 一、性能优化建议

1. **批量操作**：尽可能批量处理文件系统操作
2. **缓存信息**：避免重复查询相同的文件系统信息
3. **异常处理**：合理使用异常处理vs错误码
4. **路径操作**：优先使用`path`类而不是字符串操作

### 二、安全考虑

1. **路径遍历防护**：验证和清理用户输入的路径
2. **权限检查**：在执行敏感操作前检查权限
3. **符号链接**：谨慎处理符号链接以防止安全问题
4. **临时文件**：使用`temp_directory_path()`创建临时文件

## 相关页面

| 页面 | 说明 |
|------|------|
| C++文档 - 文件系统TS | 文件系统技术规范文档 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/filesystem&oldid=172316>
- 最后修改时间：2024年6月9日 07:54
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++文件系统库是现代C++开发的重要工具，提供了跨平台的文件系统操作能力。理解path类、目录遍历、文件操作和错误处理机制，是有效使用该库的基础。从简单的文件检查到复杂的目录监控，文件系统库支持各种应用场景。正确处理异常和错误码，可以构建更加健壮的文件系统应用。注意编译器兼容性和平台差异，确保代码的可移植性。持续学习和实践文件系统库的新特性，是掌握现代C++开发技能的重要途径。文件系统库的成功应用将显著提高C++程序处理文件和目录的能力，使开发者能够构建更加强大和灵活的应用程序。