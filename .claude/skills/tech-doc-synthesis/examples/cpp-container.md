# std::unordered_map - 无序映射容器

## 1. 概述

`std::unordered_map` 是 C++ 标准库中的关联容器（associative container），基于哈希表实现。它存储键值对（key-value pairs），支持平均 O(1) 的查找、插入和删除操作。

定义在 `<unordered_map>` 头文件中，位于 `std` 命名空间。

与 `std::map` 的关键区别：
- `map` 基于红黑树，元素有序，操作 O(log n)
- `unordered_map` 基于哈希表，元素无序，操作平均 O(1)

## 2. 来源与演变

### C++11 引入

`std::unordered_map` 在 **C++11** 标准中首次引入，取代了之前的非标准 `hash_map`。

### 设计动机

- `std::map` 的红黑树实现虽然保证了有序性，但 O(log n) 的操作复杂度在某些场景下不够高效
- 哈希表提供平均 O(1) 的操作复杂度，更适合需要快速查找而不关心顺序的场景

### 主要版本变更

| 版本 | 变更 |
|------|------|
| C++11 | 首次引入 |
| C++17 | 新增 `insert_or_assign()`、`try_emplace()` |
| C++20 | `contains()` 方法，`erase()` 返回改进 |

## 3. 语法与参数

### 模板定义

```cpp
namespace std {
    template<
        class Key,
        class T,
        class Hash = std::hash<Key>,
        class KeyEqual = std::equal_to<Key>,
        class Allocator = std::allocator<std::pair<const Key, T>>
    > class unordered_map;
}
```

### 主要方法

| 方法 | 说明 | 复杂度 |
|------|------|--------|
| `operator[]` | 访问/插入元素 | 平均 O(1) |
| `at(key)` | 访问元素（带边界检查） | 平均 O(1) |
| `insert({k, v})` | 插入元素 | 平均 O(1) |
| `emplace(k, v)` | 原地构造插入 | 平均 O(1) |
| `erase(key)` | 删除元素 | 平均 O(1) |
| `find(key)` | 查找元素 | 平均 O(1) |
| `count(key)` | 计数（0 或 1） | 平均 O(1) |
| `contains(key)` | 检查是否存在（C++20） | 平均 O(1) |

## 4. 底层原理

### 哈希表结构

```
桶数组：
[0] -> (key1, val1) -> (key5, val5) -> nullptr
[1] -> nullptr
[2] -> (key2, val2) -> nullptr
[3] -> (key3, val3) -> (key7, val7) -> nullptr
...
```

### 冲突处理

使用**链地址法**（separate chaining）处理哈希冲突：
- 每个桶存储一个链表
- 哈希到同一桶的元素链接在链表中

### 负载因子

```
负载因子 = 元素数量 / 桶数量
```

当负载因子超过阈值时，自动进行再哈希（rehash），增加桶数量。

### 性能特征

| 操作 | 平均 | 最坏 |
|------|------|------|
| 查找 | O(1) | O(n) |
| 插入 | O(1) | O(n) |
| 删除 | O(1) | O(n) |

最坏情况发生在所有元素哈希到同一桶时。

## 5. 使用场景

### 适合使用

- 需要快速查找，不关心顺序
- 键类型支持哈希
- 需要键值对存储

### 不适合使用

- 需要有序遍历 → 使用 `std::map`
- 键类型无法哈希 → 使用 `std::map`
- 需要稳定迭代器 → 使用 `std::map`

### 最佳实践

```cpp
// ✅ 预留足够桶数量
std::unordered_map<int, std::string> m;
m.reserve(1000);  // 减少再哈希

// ✅ 使用 emplace 避免临时对象
m.emplace(1, "hello");

// ✅ 使用 contains 检查存在（C++20）
if (m.contains(key)) { /* ... */ }
```

## 6. 代码示例

### 基础用法

```cpp
#include <unordered_map>
#include <string>
#include <iostream>

int main() {
    std::unordered_map<std::string, int> ages;

    // 插入
    ages["Alice"] = 25;
    ages["Bob"] = 30;
    ages.insert({"Charlie", 28});

    // 查找
    std::cout << "Alice: " << ages["Alice"] << std::endl;  // 25

    // 检查存在
    if (ages.count("Bob") > 0) {
        std::cout << "Bob exists" << std::endl;
    }

    // 遍历（无序）
    for (const auto& [name, age] : ages) {
        std::cout << name << ": " << age << std::endl;
    }

    // 删除
    ages.erase("Charlie");

    return 0;
}
```

### 高级用法

```cpp
#include <unordered_map>
#include <string>

int main() {
    std::unordered_map<std::string, std::vector<int>> data;

    // 原地构造
    data.emplace("nums", std::vector<int>{1, 2, 3});

    // C++17: insert_or_assign
    data.insert_or_assign("nums", std::vector<int>{4, 5, 6});

    // 自定义哈希函数
    struct StringHash {
        size_t operator()(const std::string& s) const {
            return std::hash<std::string>{}(s);
        }
    };

    std::unordered_map<std::string, int, StringHash> customMap;

    return 0;
}
```

### 常见错误

```cpp
// ❌ 错误：访问不存在的键会创建空值
std::unordered_map<std::string, int> m;
int x = m["nonexistent"];  // 创建 m["nonexistent"] = 0

// ✅ 正确：使用 at 或先检查
try {
    int x = m.at("nonexistent");  // 抛出异常
} catch (const std::out_of_range& e) {
    // 处理
}

// ✅ 或使用 count/contains
if (m.count("key") > 0) {
    int x = m["key"];
}
```

## 7. 总结

`std::unordered_map` 提供高效的键值存储：

| 特性 | 说明 |
|------|------|
| 查找效率 | 平均 O(1) |
| 元素顺序 | 无序 |
| 内存开销 | 较大（桶数组） |
| 迭代器稳定性 | 再哈希时失效 |

选择建议：
- 默认需要快速查找 → `unordered_map`
- 需要有序遍历 → `map`
- 需要稳定迭代器 → `map`