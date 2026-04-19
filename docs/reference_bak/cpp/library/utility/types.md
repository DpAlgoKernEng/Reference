# C++ 类型支持库（Type support）（基本类型，RTTI）

来源：cppreference.com

## 类型支持库概述

类型支持库提供了基本类型支持、数值限制查询和运行时类型信息（RTTI）功能，是C++类型系统的重要组成部分。

## 核心组件

### 一、基本类型和宏

| 组件 | 说明 |
|------|------|
| `<cstddef>`头文件 | |
| `size_t` | sizeof操作符返回的无符号整数类型（typedef） |
| `ptrdiff_t` | 两个指针相减时返回的有符号整数类型（typedef） |
| `nullptr_t`(C++11) | 空指针字面量nullptr的类型（typedef） |
| `NULL` | 实现定义的空指针常量（宏常量） |
| `max_align_t`(C++11) | 对齐要求与任何其他标量类型一样大的平凡类型（typedef） |
| `offsetof` | 从标准布局类型开头到指定成员的字节偏移量（函数宏） |
| `byte`(C++17) | 字节类型（枚举） |

### 二、固定宽度类型

| 组件 | 说明 |
|------|------|
| 固定宽度整数类型(C++11) | |
| 固定宽度浮点类型(C++23) | |

### 三、数值限制

| 组件 | 说明 |
|------|------|
| `<limits>`头文件 | |
| `numeric_limits` | 提供查询所有基本数值类型属性的接口（类模板） |
| C数值限制接口 | |

### 四、运行时类型信息

| 组件 | 说明 |
|------|------|
| `<typeinfo>`头文件 | |
| `type_info` | 包含某些类型信息的类，由typeid操作符返回（类） |
| `bad_typeid` | 如果typeid表达式中的参数为空，则抛出的异常（类） |
| `bad_cast` | 由无效的dynamic_cast表达式抛出的异常（类） |
| `<typeindex>`头文件(C++11) | |
| `type_index`(C++11) | `type_info`对象的包装器，可用作关联容器和无序关联容器的索引（类） |

---

## 扩展知识详解

### 一、基本类型详解

#### 1. size_t和ptrdiff_t
```cpp
#include <cstddef>
#include <iostream>
#include <vector>

void size_ptrdiff_examples() {
    // size_t用于表示大小和计数
    std::vector<int> vec = {1, 2, 3, 4, 5};
    size_t vec_size = vec.size();
    std::cout << "Vector size: " << vec_size << std::endl;
    
    // 数组大小计算
    int arr[] = {10, 20, 30, 40, 50};
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    std::cout << "Array size: " << arr_size << std::endl;
    
    // ptrdiff_t用于指针差值
    int* ptr1 = &arr[0];
    int* ptr2 = &arr[4];
    ptrdiff_t diff = ptr2 - ptr1;
    std::cout << "Pointer difference: " << diff << std::endl;
    
    // 跨平台兼容性
    static_assert(sizeof(size_t) >= sizeof(unsigned int), 
                  "size_t should be at least as large as unsigned int");
    static_assert(sizeof(ptrdiff_t) >= sizeof(int), 
                  "ptrdiff_t should be at least as large as int");
}
```

#### 2. nullptr_t和NULL
```cpp
#include <cstddef>
#include <iostream>
#include <typeinfo>

void nullptr_examples() {
    // nullptr是nullptr_t类型的字面量
    nullptr_t null_ptr = nullptr;
    
    // 使用nullptr进行指针初始化
    int* int_ptr = nullptr;
    char* char_ptr = nullptr;
    void* void_ptr = nullptr;
    
    // nullptr与NULL的区别
    std::cout << "nullptr type: " << typeid(nullptr).name() << std::endl;
    std::cout << "nullptr value: " << nullptr << std::endl;
    
    // 函数重载中的区别
    void func(int* ptr) {
        std::cout << "Called func(int*)" << std::endl;
    }
    
    void func(int n) {
        std::cout << "Called func(int)" << std::endl;
    }
    
    // 使用nullptr调用正确的重载
    func(nullptr);  // 调用func(int*)
    func(0);        // 调用func(int)
    func(NULL);     // 可能调用func(int)，取决于实现
    
    // 模板中的nullptr
    template<typename T>
    void template_func(T&& arg) {
        if constexpr (std::is_same_v<std::decay_t<T>, std::nullptr_t>) {
            std::cout << "nullptr passed to template" << std::endl;
        } else {
            std::cout << "Other type passed to template" << std::endl;
        }
    }
    
    template_func(nullptr);
    template_func(42);
}
```

#### 3. byte类型（C++17）
```cpp
#include <cstddef>
#include <iostream>
#include <bitset>

void byte_examples() {
    // 创建byte对象
    std::byte b1{42};  // 直接初始化
    std::byte b2 = std::byte{0xFF};  // 复制初始化
    
    // byte的位操作
    std::byte b3 = b1 | b2;  // 按位或
    std::byte b4 = b1 & b2;  // 按位与
    std::byte b5 = b1 ^ b2;  // 按位异或
    std::byte b6 = ~b1;      // 按位取反
    
    // 转换为整数
    unsigned char uc = std::to_integer<unsigned char>(b1);
    std::cout << "Byte value as unsigned char: " << static_cast<int>(uc) << std::endl;
    
    // 位移操作
    std::byte shifted = b1 << 2;
    std::cout << "Shifted byte: " << std::to_integer<int>(shifted) << std::endl;
    
    // byte数组操作
    std::byte buffer[10];
    for (int i = 0; i < 10; ++i) {
        buffer[i] = std::byte{i};
    }
    
    std::cout << "Buffer contents: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << std::to_integer<int>(buffer[i]) << " ";
    }
    std::cout << std::endl;
    
    // 安全的类型转换
    auto int_value = std::to_integer<int>(b1);
    std::cout << "Safe conversion to int: " << int_value << std::endl;
}
```

### 二、固定宽度整数类型（C++11）

#### 1. 标准固定宽度整数类型
```cpp
#include <cstdint>
#include <iostream>
#include <type_traits>

void fixed_width_integer_examples() {
    // 8位整数
    std::int8_t i8 = 127;
    std::uint8_t u8 = 255;
    
    // 16位整数
    std::int16_t i16 = 32767;
    std::uint16_t u16 = 65535;
    
    // 32位整数
    std::int32_t i32 = 2147483647;
    std::uint32_t u32 = 4294967295U;
    
    // 64位整数
    std::int64_t i64 = 9223372036854775807LL;
    std::uint64_t u64 = 18446744073709551615ULL;
    
    // 最快的最小宽度类型
    std::int_fast8_t fast_i8 = 100;
    std::uint_fast16_t fast_u16 = 50000;
    
    // 最小的最快宽度类型
    std::int_least32_t least_i32 = 1000000000;
    
    // 最大值和最小值
    std::cout << "int8_t range: [" << INT8_MIN << ", " << INT8_MAX << "]" << std::endl;
    std::cout << "uint8_t range: [0, " << UINT8_MAX << "]" << std::endl;
    std::cout << "int16_t range: [" << INT16_MIN << ", " << INT16_MAX << "]" << std::endl;
    std::cout << "uint16_t range: [0, " << UINT16_MAX << "]" << std::endl;
    std::cout << "int32_t range: [" << INT32_MIN << ", " << INT32_MAX << "]" << std::endl;
    std::cout << "uint32_t range: [0, " << UINT32_MAX << "]" << std::endl;
    
    // 类型特征检查
    static_assert(std::is_signed_v<std::int8_t>, "int8_t should be signed");
    static_assert(std::is_unsigned_v<std::uint8_t>, "uint8_t should be unsigned");
    
    // 大小检查
    std::cout << "sizeof(int8_t): " << sizeof(std::int8_t) << std::endl;
    std::cout << "sizeof(int16_t): " << sizeof(std::int16_t) << std::endl;
    std::cout << "sizeof(int32_t): " << sizeof(std::int32_t) << std::endl;
    std::cout << "sizeof(int64_t): " << sizeof(std::int64_t) << std::endl;
    
    // 实际应用示例
    struct NetworkPacket {
        std::uint32_t header;
        std::uint16_t type;
        std::uint16_t length;
        std::uint64_t timestamp;
    };
    
    NetworkPacket packet{0x12345678, 1, 100, 123456789012345ULL};
    std::cout << "Packet header: 0x" << std::hex << packet.header << std::dec << std::endl;
}
```

#### 2. 固定宽度浮点类型（C++23）
```cpp
#include <stdfloat>
#include <iostream>
#include <iomanip>

void fixed_width_float_examples() {
    // 固定宽度浮点类型（C++23）
    // std::float16_t, std::float32_t, std::float64_t, std::float128_t
    // std::bfloat16_t
    
    // 注意：这些类型需要编译器支持
#ifdef __STDCPP_FLOAT16_T__
    std::float16_t f16 = 1.5f16;
    std::cout << "float16_t: " << std::setprecision(10) << static_cast<double>(f16) << std::endl;
#endif

#ifdef __STDCPP_FLOAT32_T__
    std::float32_t f32 = 3.14159f32;
    std::cout << "float32_t: " << std::setprecision(10) << static_cast<double>(f32) << std::endl;
#endif

#ifdef __STDCPP_FLOAT64_T__
    std::float64_t f64 = 2.718281828459045;
    std::cout << "float64_t: " << std::setprecision(20) << static_cast<double>(f64) << std::endl;
#endif

    // 性能和精度考虑
    std::cout << "Size of float: " << sizeof(float) << " bytes" << std::endl;
    std::cout << "Size of double: " << sizeof(double) << " bytes" << std::endl;
    
    // 跨平台兼容性
    static_assert(sizeof(std::float32_t) == 4, "float32_t should be 32 bits");
    static_assert(sizeof(std::float64_t) == 8, "float64_t should be 64 bits");
}
```

### 三、数值限制

#### 1. numeric_limits使用
```cpp
#include <limits>
#include <iostream>
#include <typeinfo>

template<typename T>
void print_numeric_limits(const std::string& type_name) {
    std::cout << "=== " << type_name << " limits ===" << std::endl;
    std::cout << "is_specialized: " << std::numeric_limits<T>::is_specialized << std::endl;
    std::cout << "is_signed: " << std::numeric_limits<T>::is_signed << std::endl;
    std::cout << "is_integer: " << std::numeric_limits<T>::is_integer << std::endl;
    std::cout << "is_exact: " << std::numeric_limits<T>::is_exact << std::endl;
    std::cout << "digits: " << std::numeric_limits<T>::digits << std::endl;
    std::cout << "digits10: " << std::numeric_limits<T>::digits10 << std::endl;
    
    if (std::numeric_limits<T>::is_integer) {
        std::cout << "min(): " << std::numeric_limits<T>::min() << std::endl;
        std::cout << "max(): " << std::numeric_limits<T>::max() << std::endl;
    } else {
        std::cout << "min(): " << std::numeric_limits<T>::min() << std::endl;
        std::cout << "max(): " << std::numeric_limits<T>::max() << std::endl;
        std::cout << "lowest(): " << std::numeric_limits<T>::lowest() << std::endl;
        std::cout << "epsilon(): " << std::numeric_limits<T>::epsilon() << std::endl;
        std::cout << "round_error(): " << std::numeric_limits<T>::round_error() << std::endl;
    }
    
    std::cout << "has_infinity: " << std::numeric_limits<T>::has_infinity << std::endl;
    std::cout << "has_quiet_NaN: " << std::numeric_limits<T>::has_quiet_NaN << std::endl;
    std::cout << "has_signaling_NaN: " << std::numeric_limits<T>::has_signaling_NaN << std::endl;
    std::cout << "has_denorm: " << std::numeric_limits<T>::has_denorm << std::endl;
    std::cout << "is_iec559: " << std::numeric_limits<T>::is_iec559 << std::endl;
    
    std::cout << std::endl;
}

void numeric_limits_examples() {
    // 整数类型限制
    print_numeric_limits<char>("char");
    print_numeric_limits<short>("short");
    print_numeric_limits<int>("int");
    print_numeric_limits<long>("long");
    print_numeric_limits<long long>("long long");
    
    // 无符号整数类型限制
    print_numeric_limits<unsigned char>("unsigned char");
    print_numeric_limits<unsigned int>("unsigned int");
    print_numeric_limits<unsigned long long>("unsigned long long");
    
    // 浮点数类型限制
    print_numeric_limits<float>("float");
    print_numeric_limits<double>("double");
    print_numeric_limits<long double>("long double");
    
    // 特殊值检查
    if (std::numeric_limits<double>::has_infinity) {
        double inf = std::numeric_limits<double>::infinity();
        std::cout << "Infinity value: " << inf << std::endl;
    }
    
    if (std::numeric_limits<double>::has_quiet_NaN) {
        double nan_val = std::numeric_limits<double>::quiet_NaN();
        std::cout << "NaN value: " << nan_val << std::endl;
        std::cout << "Is NaN: " << std::isnan(nan_val) << std::endl;
    }
    
    // 实际应用示例
    template<typename T>
    T safe_divide(T a, T b) {
        if (b == T{0}) {
            if (a > T{0}) {
                return std::numeric_limits<T>::max();
            } else if (a < T{0}) {
                return std::numeric_limits<T>::lowest();
            } else {
                return T{0};
            }
        }
        return a / b;
    }
    
    std::cout << "Safe divide examples:" << std::endl;
    std::cout << "10 / 2 = " << safe_divide(10.0, 2.0) << std::endl;
    std::cout << "10 / 0 = " << safe_divide(10.0, 0.0) << std::endl;
    std::cout << "-10 / 0 = " << safe_divide(-10.0, 0.0) << std::endl;
}
```

### 四、运行时类型信息（RTTI）

#### 1. type_info和typeid
```cpp
#include <typeinfo>
#include <iostream>
#include <string>
#include <vector>

class Base {
public:
    virtual ~Base() = default;
    virtual void print() const { std::cout << "Base" << std::endl; }
};

class Derived : public Base {
public:
    void print() const override { std::cout << "Derived" << std::endl; }
};

void type_info_examples() {
    // 基本typeid使用
    int x = 42;
    const std::type_info& int_type = typeid(x);
    std::cout << "Type of x: " << int_type.name() << std::endl;
    
    // 类型比较
    double y = 3.14;
    if (typeid(x) == typeid(y)) {
        std::cout << "x and y have the same type" << std::endl;
    } else {
        std::cout << "x and y have different types" << std::endl;
    }
    
    // 类类型信息
    Base base;
    Derived derived;
    
    const std::type_info& base_type = typeid(base);
    const std::type_info& derived_type = typeid(derived);
    
    std::cout << "Base type: " << base_type.name() << std::endl;
    std::cout << "Derived type: " << derived_type.name() << std::endl;
    
    // 多态类型检查
    Base* ptr = &derived;
    const std::type_info& ptr_type = typeid(*ptr);
    std::cout << "Polymorphic type through pointer: " << ptr_type.name() << std::endl;
    
    // 类型信息比较
    if (typeid(base) != typeid(derived)) {
        std::cout << "Base and Derived are different types" << std::endl;
    }
    
    if (typeid(*ptr) == typeid(derived)) {
        std::cout << "ptr points to Derived object" << std::endl;
    }
    
    // before函数比较类型信息
    if (base_type.before(derived_type)) {
        std::cout << "Base type comes before Derived type in implementation's ordering" << std::endl;
    }
    
    // hash_code获取
    std::cout << "Base type hash: " << base_type.hash_code() << std::endl;
    std::cout << "Derived type hash: " << derived_type.hash_code() << std::endl;
    
    // 实际应用：类型安全的容器
    std::vector<std::pair<const std::type_info*, std::string>> type_registry;
    type_registry.emplace_back(&typeid(int), "integer");
    type_registry.emplace_back(&typeid(double), "floating point");
    type_registry.emplace_back(&typeid(std::string), "string");
    
    auto find_type_name = [&type_registry](const std::type_info& ti) -> std::string {
        for (const auto& [type_info_ptr, name] : type_registry) {
            if (*type_info_ptr == ti) {
                return name;
            }
        }
        return "unknown";
    };
    
    std::cout << "Type name of int: " << find_type_name(typeid(int)) << std::endl;
    std::cout << "Type name of char: " << find_type_name(typeid(char)) << std::endl;
}
```

#### 2. type_index使用（C++11）
```cpp
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>

class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const = 0;
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!" << std::endl; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "Meow!" << std::endl; }
};

void type_index_examples() {
    // 创建type_index对象
    std::type_index int_type{typeid(int)};
    std::type_index double_type{typeid(double)};
    std::type_index string_type{typeid(std::string)};
    
    // 比较type_index
    if (int_type != double_type) {
        std::cout << "int and double have different type indices" << std::endl;
    }
    
    // 用作关联容器的键
    std::map<std::type_index, std::string> type_names;
    type_names[typeid(int)] = "integer";
    type_names[typeid(double)] = "double precision floating point";
    type_names[typeid(std::string)] = "string";
    type_names[typeid(Animal)] = "animal base class";
    
    std::cout << "Type names:" << std::endl;
    for (const auto& [type_idx, name] : type_names) {
        std::cout << "  " << name << std::endl;
    }
    
    // 用作无序关联容器的键
    std::unordered_map<std::type_index, int> type_counts;
    type_counts[typeid(int)] = 100;
    type_counts[typeid(double)] = 200;
    type_counts[typeid(std::string)] = 300;
    
    std::cout << "Type counts:" << std::endl;
    for (const auto& [type_idx, count] : type_counts) {
        std::cout << "  Count for " << type_idx.name() << ": " << count << std::endl;
    }
    
    // 实际应用：工厂模式
    std::unordered_map<std::type_index, std::function<std::unique_ptr<Animal>()>> animal_factory;
    
    animal_factory[typeid(Dog)] = []() { return std::make_unique<Dog>(); };
    animal_factory[typeid(Cat)] = []() { return std::make_unique<Cat>(); };
    
    // 创建动物对象
    auto dog = animal_factory[typeid(Dog)]();
    auto cat = animal_factory[typeid(Cat)]();
    
    dog->speak();
    cat->speak();
    
    // 类型统计
    std::unordered_map<std::type_index, size_t> object_counts;
    
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(animal_factory[typeid(Dog)]());
    animals.push_back(animal_factory[typeid(Cat)]());
    animals.push_back(animal_factory[typeid(Dog)]());
    
    for (const auto& animal : animals) {
        object_counts[std::type_index(typeid(*animal))]++;
    }
    
    std::cout << "Object counts by type:" << std::endl;
    for (const auto& [type_idx, count] : object_counts) {
        std::cout << "  " << type_idx.name() << ": " << count << std::endl;
    }
}
```

#### 3. 异常处理
```cpp
#include <typeinfo>
#include <iostream>
#include <memory>

class PolymorphicBase {
public:
    virtual ~PolymorphicBase() = default;
};

class DerivedA : public PolymorphicBase {};
class DerivedB : public PolymorphicBase {};

void exception_examples() {
    // bad_typeid异常
    try {
        PolymorphicBase* ptr = nullptr;
        const std::type_info& ti = typeid(*ptr);  // 这会抛出bad_typeid
        std::cout << "Type: " << ti.name() << std::endl;
    } catch (const std::bad_typeid& e) {
        std::cout << "Caught bad_typeid: " << e.what() << std::endl;
    }
    
    // bad_cast异常
    try {
        std::unique_ptr<PolymorphicBase> base_ptr = std::make_unique<DerivedA>();
        DerivedB& derived_ref = dynamic_cast<DerivedB&>(*base_ptr);  // 这会抛出bad_cast
        derived_ref.do_something();
    } catch (const std::bad_cast& e) {
        std::cout << "Caught bad_cast: " << e.what() << std::endl;
    }
    
    // 安全的类型检查
    auto safe_dynamic_cast = [](PolymorphicBase* base, const std::type_info& target_type) {
        try {
            if (base == nullptr) {
                throw std::bad_typeid();
            }
            
            const std::type_info& actual_type = typeid(*base);
            if (actual_type == target_type) {
                return true;
            }
            return false;
        } catch (const std::bad_typeid&) {
            return false;
        }
    };
    
    std::unique_ptr<PolymorphicBase> ptr = std::make_unique<DerivedA>();
    if (safe_dynamic_cast(ptr.get(), typeid(DerivedA))) {
        std::cout << "Safe cast to DerivedA succeeded" << std::endl;
    }
    
    if (!safe_dynamic_cast(ptr.get(), typeid(DerivedB))) {
        std::cout << "Safe cast to DerivedB failed as expected" << std::endl;
    }
}
```

### 五、实际应用场景

#### 1. 序列化框架中的类型系统
```cpp
#include <typeindex>
#include <unordered_map>
#include <iostream>
#include <string>
#include <any>

class TypeRegistry {
private:
    std::unordered_map<std::type_index, std::string> type_names_;
    std::unordered_map<std::string, std::type_index> name_types_;

public:
    template<typename T>
    void register_type(const std::string& name) {
        std::type_index ti{typeid(T)};
        type_names_[ti] = name;
        name_types_[name] = ti;
    }
    
    template<typename T>
    std::string get_type_name() const {
        auto it = type_names_.find(std::type_index{typeid(T)});
        return (it != type_names_.end()) ? it->second : "unknown";
    }
    
    std::string get_type_name(const std::type_info& ti) const {
        auto it = type_names_.find(std::type_index{ti});
        return (it != type_names_.end()) ? it->second : "unknown";
    }
    
    bool is_registered(const std::type_info& ti) const {
        return type_names_.find(std::type_index{ti}) != type_names_.end();
    }
};

void serialization_example() {
    TypeRegistry registry;
    
    // 注册类型
    registry.register_type<int>("integer");
    registry.register_type<double>("double");
    registry.register_type<std::string>("string");
    registry.register_type<bool>("boolean");
    
    // 类型名称查询
    std::cout << "Type names:" << std::endl;
    std::cout << "int: " << registry.get_type_name<int>() << std::endl;
    std::cout << "double: " << registry.get_type_name<double>() << std::endl;
    std::cout << "string: " << registry.get_type_name<std::string>() << std::endl;
    std::cout << "bool: " << registry.get_type_name<bool>() << std::endl;
    
    // 运行时类型检查
    auto print_type_info = [&registry](const std::any& value) {
        const std::type_info& ti = value.type();
        std::cout << "Runtime type: " << registry.get_type_name(ti) << std::endl;
        
        if (!registry.is_registered(ti)) {
            std::cout << "Warning: Type not registered for serialization" << std::endl;
        }
    };
    
    std::any int_value = 42;
    std::any double_value = 3.14;
    std::any string_value = std::string("Hello");
    std::any custom_value = std::vector<int>{1, 2, 3};  // 未注册的类型
    
    print_type_info(int_value);
    print_type_info(double_value);
    print_type_info(string_value);
    print_type_info(custom_value);
}
```

#### 2. 性能监控系统
```cpp
#include <chrono>
#include <typeindex>
#include <unordered_map>
#include <iostream>
#include <iomanip>

class PerformanceMonitor {
private:
    struct TypeInfo {
        std::string name;
        size_t instance_count = 0;
        size_t total_size = 0;
    };
    
    std::unordered_map<std::type_index, TypeInfo> type_stats_;

public:
    template<typename T>
    void register_type(const std::string& name) {
        type_stats_[std::type_index{typeid(T)}] = {name, 0, 0};
    }
    
    template<typename T>
    void object_created(const T& obj) {
        auto it = type_stats_.find(std::type_index{typeid(T)});
        if (it != type_stats_.end()) {
            it->second.instance_count++;
            it->second.total_size += sizeof(obj);
        }
    }
    
    template<typename T>
    void object_destroyed(const T& obj) {
        auto it = type_stats_.find(std::type_index{typeid(T)});
        if (it != type_stats_.end() && it->second.instance_count > 0) {
            it->second.instance_count--;
            if (it->second.total_size >= sizeof(obj)) {
                it->second.total_size -= sizeof(obj);
            }
        }
    }
    
    void print_statistics() const {
        std::cout << std::left << std::setw(20) << "Type" 
                  << std::setw(15) << "Instances" 
                  << std::setw(15) << "Total Size" 
                  << std::setw(15) << "Avg Size" << std::endl;
        std::cout << std::string(65, '-') << std::endl;
        
        for (const auto& [type_idx, info] : type_stats_) {
            double avg_size = info.instance_count > 0 ? 
                             static_cast<double>(info.total_size) / info.instance_count : 0;
            std::cout << std::left << std::setw(20) << info.name
                      << std::setw(15) << info.instance_count
                      << std::setw(15) << info.total_size
                      << std::setw(15) << std::fixed << std::setprecision(2) << avg_size
                      << std::endl;
        }
    }
};

void performance_monitoring_example() {
    PerformanceMonitor monitor;
    
    // 注册类型
    monitor.register_type<int>("integer");
    monitor.register_type<double>("double");
    monitor.register_type<std::string>("string");
    monitor.register_type<std::vector<int>>("vector<int>");
    
    // 模拟对象创建和销毁
    {
        int a = 42;
        double b = 3.14;
        std::string c = "Hello World";
        std::vector<int> d = {1, 2, 3, 4, 5};
        
        monitor.object_created(a);
        monitor.object_created(b);
        monitor.object_created(c);
        monitor.object_created(d);
        
        // 创建更多对象
        for (int i = 0; i < 10; ++i) {
            int temp = i;
            monitor.object_created(temp);
        }
    }
    
    // 打印统计信息
    std::cout << "Performance Statistics:" << std::endl;
    monitor.print_statistics();
}
```

## 最佳实践建议

### 一、类型使用准则

1. **优先使用size_t**：对于数组索引和大小计算使用size_t
2. **正确使用nullptr**：现代C++中优先使用nullptr而非NULL
3. **类型安全**：利用类型系统和RTTI实现类型安全的操作
4. **固定宽度类型**：对于需要特定大小的整数使用固定宽度类型

### 二、性能考虑

1. **避免频繁RTTI查询**：RTTI操作有一定开销，避免在性能关键路径中频繁使用
2. **合理使用type_index**：在需要类型作为键的容器中使用type_index
3. **类型注册模式**：对于复杂的类型系统实现类型注册模式
4. **编译时优化**：尽可能将类型检查移到编译时

### 三、异常安全

1. **处理bad_typeid**：在使用typeid操作符时处理可能的bad_typeid异常
2. **处理bad_cast**：在使用dynamic_cast时处理可能的bad_cast异常
3. **安全的类型转换**：实现安全的类型转换检查机制
4. **资源管理**：确保异常情况下资源的正确释放

## 相关页面

| 页面 | 说明 |
|------|------|
| 类型特征和元编程库 | 编译时类型操作 |
| C文档类型支持库 | C风格类型支持 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/types&oldid=170737>
- 最后修改时间：2024年4月9日 07:49
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++类型支持库为现代C++开发提供了强大的类型系统支持。从基础的size_t、nullptr_t到复杂的RTTI机制，这些组件构成了C++类型系统的基础。正确理解和使用这些类型支持特性对于编写高效、安全的C++代码至关重要。通过合理的类型选择和类型安全设计，可以构建出更加健壮和高效的程序。持续学习和实践类型系统相关技术，结合现代C++的最佳实践，能够充分发挥C++类型系统的强大能力。记住在使用RTTI时要考虑性能影响，合理选择固定宽度类型以确保跨平台兼容性。