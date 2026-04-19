# C++元编程库（Metaprogramming Library）（C++11起）

来源：cppreference.com

## 元编程库概述

C++提供了元编程工具，包括类型特征（type traits）、编译期有理数运算和编译期整数序列。

## 核心组件

### 一、类型特征（Type Traits）

类型特征定义了基于模板的编译期接口，用于查询类型的属性。

#### 基础类
| 类 | 说明 |
|----|------|
| `integral_constant` | 指定类型的编译期常量（类模板） |
| `bool_constant`(C++17) | 布尔类型的编译期常量（类模板） |
| `true_type` | `std::integral_constant<bool, true>` |
| `false_type` | `std::integral_constant<bool, false>` |

#### 一元类型特征
##### 主要类型分类
| 特征 | 说明 |
|------|------|
| `is_void`(C++11) | 检查类型是否为void |
| `is_null_pointer`(C++11) | 检查类型是否为std::nullptr_t |
| `is_integral`(C++11) | 检查类型是否为整数类型 |
| `is_floating_point`(C++11) | 检查类型是否为浮点类型 |
| `is_array`(C++11) | 检查类型是否为数组类型 |
| `is_enum`(C++11) | 检查类型是否为枚举类型 |
| `is_union`(C++11) | 检查类型是否为联合体类型 |
| `is_class`(C++11) | 检查类型是否为非联合体类类型 |
| `is_function`(C++11) | 检查类型是否为函数类型 |
| `is_pointer`(C++11) | 检查类型是否为指针类型 |
| `is_lvalue_reference`(C++11) | 检查类型是否为左值引用 |
| `is_rvalue_reference`(C++11) | 检查类型是否为右值引用 |
| `is_member_object_pointer`(C++11) | 检查类型是否为非静态成员对象指针 |
| `is_member_function_pointer`(C++11) | 检查类型是否为非静态成员函数指针 |

##### 复合类型分类
| 特征 | 说明 |
|------|------|
| `is_fundamental`(C++11) | 检查类型是否为基本类型 |
| `is_arithmetic`(C++11) | 检查类型是否为算术类型 |
| `is_scalar`(C++11) | 检查类型是否为标量类型 |
| `is_object`(C++11) | 检查类型是否为对象类型 |
| `is_compound`(C++11) | 检查类型是否为复合类型 |
| `is_reference`(C++11) | 检查类型是否为引用类型 |
| `is_member_pointer`(C++11) | 检查类型是否为非静态成员指针 |

##### 类型属性
| 特征 | 说明 |
|------|------|
| `is_const`(C++11) | 检查类型是否为const限定 |
| `is_volatile`(C++11) | 检查类型是否为volatile限定 |
| `is_trivial`(C++11) | 检查类型是否为平凡类型（C++26起已弃用） |
| `is_trivially_copyable`(C++11) | 检查类型是否为平凡可复制 |
| `is_standard_layout`(C++11) | 检查类型是否为标准布局类型 |
| `is_pod`(C++11) | 检查类型是否为POD类型（C++20起已弃用） |
| `is_empty`(C++11) | 检查类型是否为空类 |
| `is_polymorphic`(C++11) | 检查类型是否为多态类 |
| `is_abstract`(C++11) | 检查类型是否为抽象类 |
| `is_final`(C++14) | 检查类型是否为final类 |
| `is_aggregate`(C++17) | 检查类型是否为聚合类型 |
| `is_signed`(C++11) | 检查类型是否为有符号算术类型 |
| `is_unsigned`(C++11) | 检查类型是否为无符号算术类型 |

#### 属性查询
| 特征 | 说明 |
|------|------|
| `alignment_of`(C++11) | 获取类型的对齐要求 |
| `rank`(C++11) | 获取数组类型的维度数 |
| `extent`(C++11) | 获取数组类型指定维度的大小 |

#### 类型关系
| 特征 | 说明 |
|------|------|
| `is_same`(C++11) | 检查两个类型是否相同 |
| `is_base_of`(C++11) | 检查类型是否为另一个类型的基类 |
| `is_convertible`(C++11) | 检查类型是否可转换为另一个类型 |

#### 类型变换
##### 常量性修饰符
| 特征 | 说明 |
|------|------|
| `remove_cv`等 | 移除const和/或volatile限定符 |
| `add_cv`等 | 添加const和/或volatile限定符 |

##### 引用
| 特征 | 说明 |
|------|------|
| `remove_reference`(C++11) | 移除引用 |
| `add_lvalue_reference`等 | 添加左值或右值引用 |

##### 符号修饰符
| 特征 | 说明 |
|------|------|
| `make_signed`(C++11) | 获取对应有符号类型 |
| `make_unsigned`(C++11) | 获取对应无符号类型 |

##### 数组
| 特征 | 说明 |
|------|------|
| `remove_extent`(C++11) | 移除一个数组维度 |
| `remove_all_extents`(C++11) | 移除所有数组维度 |

##### 指针
| 特征 | 说明 |
|------|------|
| `remove_pointer`(C++11) | 移除指针 |
| `add_pointer`(C++11) | 添加指针 |

##### 其他变换
| 特征 | 说明 |
|------|------|
| `decay`(C++11) | 应用函数参数值传递的类型变换 |
| `enable_if`(C++11) | 条件性地从重载解析中移除函数重载或模板特化 |
| `conditional`(C++11) | 基于编译期布尔值选择类型 |
| `common_type`(C++11) | 确定一组类型的公共类型 |
| `underlying_type`(C++11) | 获取枚举类型的底层整数类型 |

### 二、逻辑运算（C++17起）

| 特征 | 说明 |
|------|------|
| `conjunction`(C++17) | 变参逻辑AND元函数 |
| `disjunction`(C++17) | 变参逻辑OR元函数 |
| `negation`(C++17) | 逻辑NOT元函数 |

### 三、编译期整数序列（C++14起）

| 组件 | 说明 |
|------|------|
| `integer_sequence`(C++14) | 实现编译期整数序列（类模板） |

### 四、编译期有理数运算

`<ratio>`头文件提供用于操作和存储编译期比率的类型和函数。

---

## 扩展知识详解

### 一、类型特征深度解析

#### 1. 基础类型特征使用
```cpp
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>

// 类型信息打印模板
template<typename T>
void print_type_info() {
    std::cout << "Type: " << typeid(T).name() << std::endl;
    std::cout << "  is_void: " << std::is_void_v<T> << std::endl;
    std::cout << "  is_integral: " << std::is_integral_v<T> << std::endl;
    std::cout << "  is_floating_point: " << std::is_floating_point_v<T> << std::endl;
    std::cout << "  is_array: " << std::is_array_v<T> << std::endl;
    std::cout << "  is_enum: " << std::is_enum_v<T> << std::endl;
    std::cout << "  is_class: " << std::is_class_v<T> << std::endl;
    std::cout << "  is_function: " << std::is_function_v<T> << std::endl;
    std::cout << "  is_pointer: " << std::is_pointer_v<T> << std::endl;
    std::cout << "  is_reference: " << std::is_reference_v<T> << std::endl;
    std::cout << "  is_const: " << std::is_const_v<T> << std::endl;
    std::cout << "  is_volatile: " << std::is_volatile_v<T> << std::endl;
    std::cout << std::endl;
}

void basic_type_traits_example() {
    print_type_info<void>();
    print_type_info<int>();
    print_type_info<double>();
    print_type_info<int[]>();
    print_type_info<std::string>();
    print_type_info<int*>();
    print_type_info<int&>();
    print_type_info<const int>();
    print_type_info<volatile int>();
}
```

#### 2. 复合类型特征使用
```cpp
#include <type_traits>
#include <iostream>
#include <vector>
#include <string>

// 复合类型特征示例
template<typename T>
void print_composite_traits() {
    std::cout << "Type: " << typeid(T).name() << std::endl;
    std::cout << "  is_arithmetic: " << std::is_arithmetic_v<T> << std::endl;
    std::cout << "  is_fundamental: " << std::is_fundamental_v<T> << std::endl;
    std::cout << "  is_scalar: " << std::is_scalar_v<T> << std::endl;
    std::cout << "  is_object: " << std::is_object_v<T> << std::endl;
    std::cout << "  is_compound: " << std::is_compound_v<T> << std::endl;
    std::cout << "  is_trivially_copyable: " << std::is_trivially_copyable_v<T> << std::endl;
    std::cout << "  is_standard_layout: " << std::is_standard_layout_v<T> << std::endl;
    std::cout << std::endl;
}

void composite_traits_example() {
    print_composite_traits<int>();
    print_composite_traits<double>();
    print_composite_traits<std::string>();
    print_composite_traits<std::vector<int>>();
    print_composite_traits<int*>();
    print_composite_traits<int&>();
}
```

#### 3. 类型支持操作检查
```cpp
#include <type_traits>
#include <iostream>
#include <string>
#include <memory>

class SimpleClass {
public:
    SimpleClass() = default;
    SimpleClass(const SimpleClass&) = default;
    SimpleClass& operator=(const SimpleClass&) = default;
    ~SimpleClass() = default;
};

class ComplexClass {
private:
    std::unique_ptr<int> ptr_;
    
public:
    ComplexClass() : ptr_(std::make_unique<int>(0)) {}
    
    // 显式声明移动构造函数和移动赋值运算符
    ComplexClass(ComplexClass&&) = default;
    ComplexClass& operator=(ComplexClass&&) = default;
    
    // 显式声明拷贝构造函数和拷贝赋值运算符
    ComplexClass(const ComplexClass&) = delete;
    ComplexClass& operator=(const ComplexClass&) = delete;
    
    ~ComplexClass() = default;
};

template<typename T>
void print_construction_traits() {
    std::cout << "Type: " << typeid(T).name() << std::endl;
    std::cout << "  is_default_constructible: " << std::is_default_constructible_v<T> << std::endl;
    std::cout << "  is_copy_constructible: " << std::is_copy_constructible_v<T> << std::endl;
    std::cout << "  is_move_constructible: " << std::is_move_constructible_v<T> << std::endl;
    std::cout << "  is_copy_assignable: " << std::is_copy_assignable_v<T> << std::endl;
    std::cout << "  is_move_assignable: " << std::is_move_assignable_v<T> << std::endl;
    std::cout << "  is_destructible: " << std::is_destructible_v<T> << std::endl;
    std::cout << std::endl;
}

void construction_traits_example() {
    print_construction_traits<int>();
    print_construction_traits<std::string>();
    print_construction_traits<SimpleClass>();
    print_construction_traits<ComplexClass>();
}
```

### 二、类型关系和变换

#### 1. 类型关系检查
```cpp
#include <type_traits>
#include <iostream>

class Base {
public:
    virtual ~Base() = default;
    virtual void virtual_func() {}
};

class Derived : public Base {
public:
    void virtual_func() override {}
};

class Unrelated {};

template<typename T, typename U>
void print_type_relationships() {
    std::cout << "Type1: " << typeid(T).name() << std::endl;
    std::cout << "Type2: " << typeid(U).name() << std::endl;
    std::cout << "  is_same: " << std::is_same_v<T, U> << std::endl;
    std::cout << "  is_base_of: " << std::is_base_of_v<T, U> << std::endl;
    std::cout << "  is_convertible: " << std::is_convertible_v<T, U> << std::endl;
    std::cout << std::endl;
}

void type_relationships_example() {
    print_type_relationships<int, int>();
    print_type_relationships<Base, Derived>();
    print_type_relationships<Derived, Base>();
    print_type_relationships<Base, Unrelated>();
    print_type_relationships<int, double>();
}
```

#### 2. 类型变换示例
```cpp
#include <type_traits>
#include <iostream>
#include <vector>
#include <string>

// 类型变换演示
template<typename T>
void demonstrate_type_transformations() {
    std::cout << "Original type: " << typeid(T).name() << std::endl;
    
    // 移除引用和const限定符
    using decayed = std::decay_t<T>;
    std::cout << "Decayed type: " << typeid(decayed).name() << std::endl;
    
    // 移除const
    using remove_const = std::remove_const_t<T>;
    std::cout << "Remove const: " << typeid(remove_const).name() << std::endl;
    
    // 添加指针
    using add_pointer = std::add_pointer_t<T>;
    std::cout << "Add pointer: " << typeid(add_pointer).name() << std::endl;
    
    // 转换为有符号类型
    if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
        using make_signed = std::make_signed_t<T>;
        std::cout << "Make signed: " << typeid(make_signed).name() << std::endl;
    }
    
    std::cout << std::endl;
}

void type_transformation_example() {
    demonstrate_type_transformations<int>();
    demonstrate_type_transformations<const int&>();
    demonstrate_type_transformations<int[5]>();
    demonstrate_type_transformations<void()>();
}
```

### 三、类型特征在模板编程中的应用

#### 1. SFINAE和enable_if使用
```cpp
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>

// SFINAE示例：为不同类型提供不同的函数实现
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
process_value(const T& value) {
    std::cout << "Processing integral value: " << value << std::endl;
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
process_value(const T& value) {
    std::cout << "Processing floating point value: " << value << std::endl;
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value, void>::type
process_value(const T& value) {
    std::cout << "Processing string value: " << value << std::endl;
}

// C++17简化写法
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, void>
process_arithmetic(const T& value) {
    std::cout << "Processing arithmetic value: " << value << std::endl;
}

void sfinae_example() {
    process_value(42);
    process_value(3.14);
    process_value(std::string("Hello"));
    process_arithmetic(100);
    process_arithmetic(2.71);
}
```

#### 2. 类型约束和概念检查
```cpp
#include <type_traits>
#include <iostream>
#include <vector>
#include <string>

// 自定义类型特征组合
template<typename T>
struct is_string_like : std::disjunction<
    std::is_same<std::decay_t<T>, std::string>,
    std::is_same<std::decay_t<T>, const char*>
> {};

template<typename T>
constexpr bool is_string_like_v = is_string_like<T>::value;

// 检查是否为容器类型
template<typename T, typename = void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<T, std::void_t<
    typename T::value_type,
    typename T::size_type,
    typename T::iterator,
    decltype(std::declval<T>().begin()),
    decltype(std::declval<T>().end())
>> : std::true_type {};

template<typename T>
constexpr bool is_container_v = is_container<T>::value;

template<typename T>
void print_if_container(const T& container) {
    if constexpr (is_container_v<T>) {
        std::cout << "Container with " << container.size() << " elements" << std::endl;
    } else {
        std::cout << "Not a container" << std::endl;
    }
}

template<typename T>
void print_if_string_like(const T& str) {
    if constexpr (is_string_like_v<T>) {
        std::cout << "String-like: " << str << std::endl;
    } else {
        std::cout << "Not string-like" << std::endl;
    }
}

void custom_traits_example() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    print_if_container(vec);
    print_if_container(42);
    
    print_if_string_like(std::string("Hello"));
    print_if_string_like("World");
    print_if_string_like(42);
}
```

### 四、编译期整数序列

#### 1. integer_sequence使用
```cpp
#include <utility>
#include <iostream>
#include <tuple>

// 使用integer_sequence展开tuple
template<typename Tuple, std::size_t... Indices>
void print_tuple_impl(const Tuple& t, std::index_sequence<Indices...>) {
    ((std::cout << std::get<Indices>(t) << " "), ...);
    std::cout << std::endl;
}

template<typename... Args>
void print_tuple(const std::tuple<Args...>& t) {
    print_tuple_impl(t, std::index_sequence_for<Args...>{});
}

// 生成序列的数学计算
template<typename T, T... Ints>
constexpr T sum_sequence(std::integer_sequence<T, Ints...>) {
    return (Ints + ... + 0);
}

template<typename T, T... Ints>
constexpr T product_sequence(std::integer_sequence<T, Ints...>) {
    if constexpr (sizeof...(Ints) == 0) {
        return 1;
    } else {
        return (Ints * ... * 1);
    }
}

void integer_sequence_example() {
    // tuple打印示例
    auto t = std::make_tuple(1, "hello", 3.14);
    print_tuple(t);
    
    // 序列计算示例
    using seq = std::make_index_sequence<5>;
    constexpr auto sum = sum_sequence(std::make_index_sequence<5>{});
    constexpr auto product = product_sequence(std::make_index_sequence<5>{});
    
    std::cout << "Sum of 0..4: " << sum << std::endl;
    std::cout << "Product of 0..4: " << product << std::endl;
    
    // 自定义序列
    using custom_seq = std::integer_sequence<int, 1, 2, 3, 4, 5>;
    constexpr auto custom_sum = sum_sequence(custom_seq{});
    std::cout << "Sum of custom sequence: " << custom_sum << std::endl;
}
```

### 五、现代C++20特性

#### 1. concepts与类型特征结合
```cpp
#include <type_traits>
#include <concepts>
#include <iostream>

// C++20 concepts示例
template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept Container = requires(T t) {
    typename T::value_type;
    typename T::size_type;
    { t.size() } -> std::convertible_to<typename T::size_type>;
    { t.begin() } -> std::input_iterator;
    { t.end() } -> std::input_iterator;
};

// 使用concepts的函数
template<Integral T>
void process_integral(T value) {
    std::cout << "Integral value: " << value << std::endl;
}

template<FloatingPoint T>
void process_floating_point(T value) {
    std::cout << "Floating point value: " << value << std::endl;
}

template<Container T>
void process_container(const T& container) {
    std::cout << "Container size: " << container.size() << std::endl;
}

void concepts_example() {
    process_integral(42);
    process_floating_point(3.14);
    
    std::vector<int> vec = {1, 2, 3};
    process_container(vec);
}
```

#### 2. type_identity使用
```cpp
#include <type_traits>
#include <iostream>

// type_identity用于模板参数推导
template<typename T>
void print_type(const T&) {
    std::cout << "Type: " << typeid(T).name() << std::endl;
}

// 使用type_identity避免模板参数推导问题
template<typename T>
void print_type_explicit(std::type_identity_t<T> value) {
    std::cout << "Explicit type: " << typeid(T).name() 
              << ", Value: " << value << std::endl;
}

void type_identity_example() {
    print_type(42);  // T被推导为int
    print_type_explicit<int>(42);  // 显式指定类型
    
    // 在需要显式类型指定的场景中特别有用
    auto lambda = [](auto value) {
        print_type_explicit<decltype(value)>(value);
    };
    lambda(3.14);
}
```

### 六、实际应用场景

#### 1. 序列化库中的类型检查
```cpp
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>

// 简单的可序列化检查
template<typename T>
struct is_serializable {
    static constexpr bool value = std::disjunction_v<
        std::is_arithmetic<T>,
        std::is_same<std::decay_t<T>, std::string>,
        std::is_same<std::decay_t<T>, const char*>
    >;
};

template<typename T>
constexpr bool is_serializable_v = is_serializable<T>::value;

// 容器序列化检查
template<typename T>
struct is_container_serializable {
    static constexpr bool value = 
        is_serializable_v<typename T::value_type> && 
        is_serializable_v<std::size_t>;  // 容器大小
};

template<typename T, typename = void>
struct is_serializable_container : std::false_type {};

template<typename T>
struct is_serializable_container<T, std::void_t<typename T::value_type>> 
    : std::bool_constant<is_container_serializable<T>::value> {};

template<typename T>
constexpr bool is_serializable_container_v = is_serializable_container<T>::value;

template<typename T>
void serialize(const T& value) {
    if constexpr (is_serializable_v<T>) {
        std::cout << "Serializing simple value: " << value << std::endl;
    } else if constexpr (is_serializable_container_v<T>) {
        std::cout << "Serializing container with " << value.size() << " elements" << std::endl;
    } else {
        std::cout << "Type not serializable" << std::endl;
    }
}

void serialization_example() {
    serialize(42);
    serialize(3.14);
    serialize(std::string("Hello"));
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    serialize(vec);
}
```

#### 2. 性能优化中的类型特征
```cpp
#include <type_traits>
#include <iostream>
#include <chrono>
#include <vector>

// 根据类型特征优化算法
template<typename T>
void optimized_operation(std::vector<T>& vec) {
    if constexpr (std::is_trivially_copyable_v<T>) {
        // 对于平凡可复制类型，可以使用memcpy等优化
        std::cout << "Using optimized copy for trivially copyable type" << std::endl;
        // 实际实现中会使用更高效的复制方法
    } else if constexpr (std::is_move_constructible_v<T>) {
        // 对于可移动类型，优先使用移动语义
        std::cout << "Using move semantics for move constructible type" << std::endl;
    } else {
        // 对于其他类型，使用拷贝构造
        std::cout << "Using copy semantics for other types" << std::endl;
    }
    
    // 对于空类，可以特殊处理
    if constexpr (std::is_empty_v<T>) {
        std::cout << "Handling empty class optimization" << std::endl;
    }
}

void performance_optimization_example() {
    std::vector<int> int_vec(1000);
    optimized_operation(int_vec);
    
    std::vector<std::string> string_vec(1000);
    optimized_operation(string_vec);
    
    // 空类示例
    struct Empty {};
    std::vector<Empty> empty_vec(1000);
    optimized_operation(empty_vec);
}
```

## 最佳实践建议

### 一、类型特征使用准则

1. **优先使用_v变量模板**：C++17后使用`std::is_integral_v<T>`而非`std::is_integral<T>::value`
2. **合理组合类型特征**：使用`std::conjunction`、`std::disjunction`等组合逻辑
3. **避免运行时检查**：类型特征应在编译期确定
4. **注意类型退化**：使用`std::decay`处理函数参数类型

### 二、SFINAE和约束使用

1. **清晰的约束条件**：使用具体的类型特征表达约束
2. **良好的错误信息**：提供有意义的编译错误信息
3. **逐步约束**：从简单到复杂的约束组合
4. **C++20概念**：优先使用concepts替代复杂的SFINAE

### 三、性能考虑

1. **编译期计算**：尽可能在编译期完成类型检查和计算
2. **避免过度约束**：只检查必需的类型属性
3. **缓存编译结果**：使用constexpr和const变量缓存结果
4. **模板特化**：针对特定类型提供优化实现

## 相关页面

| 页面 | 说明 |
|------|------|
| 概念库 | C++20概念系统 |
| 类型特征变量模板 | C++17类型特征变量 |

## 页面信息

- 页面地址：<https://en.cppreference.com/mwiki/index.php?title=cpp/meta&oldid=179895>
- 最后修改时间：2025年1月28日 10:20
- 离线版本获取时间：2025年2月9日 16:39

---

✅ C++元编程库为现代C++提供了强大的编译期编程能力。类型特征使我们能够在编译期检查和变换类型，编译期整数序列支持模板元编程的序列操作，而有理数运算提供了编译期数学计算能力。正确理解和使用这些特性是掌握现代C++模板编程的关键。类型特征帮助我们编写更安全、更高效的模板代码，SFINAE机制提供了灵活的函数重载控制，而编译期序列操作为复杂的模板元编程提供了基础。持续学习和实践这些元编程技术，结合C++20的概念系统，能够构建出更加灵活和强大的C++库和应用程序。