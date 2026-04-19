# C++ 语言参考文档

C++ 语言完整参考文档，共 **243** 篇文档（语言参考 200 篇 + 工具链 43 篇），按学习路径组织。

## Language - 语言参考

C++ 语言核心特性文档，涵盖基础概念、语法规则、标准库等内容，共 **200** 篇文档，按主题组织。

## 目录结构

```
cpp/language/
├── 00_basic_concepts/       # 基础概念（22篇）
├── 01_keyword/              # 关键字索引（1篇）
├── 02_declarations/         # 声明（37篇）
├── 03_initialization/       # 初始化（11篇）
├── 04_expressions/          # 表达式（36篇）
├── 05_statements/           # 语句（13篇）
├── 06_functions/            # 函数（9篇）
├── 07_classes/              # 类（28篇）
├── 08_templates/            # 模板（19篇）
├── 09_exceptions/           # 异常（7篇）
├── 10_preprocessor/         # 预处理器（7篇）
├── 11_idioms/               # 惯用法（6篇）
└── 12_miscellaneous/        # 杂项（4篇）
```

---

## 00_basic_concepts - 基础概念

基础入门知识，涵盖词法元素、类型系统、内存模型、作用域等核心概念。

### 00_overview - 总览

- [basic_concepts](language/00_basic_concepts/00_overview/basic_concepts.md) - 基础概念总览

### 01_lexical - 词法元素

- [ascii](language/00_basic_concepts/01_lexical/ascii.md) - ASCII 字符集
- [charset](language/00_basic_concepts/01_lexical/charset.md) - 字符集与编码
- [comments](language/00_basic_concepts/01_lexical/comments.md) - 注释
- [identifier](language/00_basic_concepts/01_lexical/identifier.md) - 标识符
- [punctuators](language/00_basic_concepts/01_lexical/punctuators.md) - 标点符号

### 02_type_system - 类型系统

- [arithmetic_types](language/00_basic_concepts/02_type_system/arithmetic_types.md) - 算术类型
- [type](language/00_basic_concepts/02_type_system/type.md) - 类型系统
- [type_compatibility](language/00_basic_concepts/02_type_system/type_compatibility.md) - 类型兼容性

### 03_memory_object - 内存与对象

- [lifetime](language/00_basic_concepts/03_memory_object/lifetime.md) - 对象生命周期
- [memory_model](language/00_basic_concepts/03_memory_object/memory_model.md) - 内存模型
- [object](language/00_basic_concepts/03_memory_object/object.md) - 对象模型
- [object_representation](language/00_basic_concepts/03_memory_object/object_representation.md) - 对象表示
- [trivial_type](language/00_basic_concepts/03_memory_object/trivial_type.md) - 平凡类型

### 04_name_scope - 名称与作用域

- [lookup](language/00_basic_concepts/04_name_scope/lookup.md) - 名称查找
- [qualified_lookup](language/00_basic_concepts/04_name_scope/qualified_lookup.md) - 限定名称查找
- [scope](language/00_basic_concepts/04_name_scope/scope.md) - 作用域

### 05_program_structure - 程序结构

- [main_function](language/00_basic_concepts/05_program_structure/main_function.md) - main 函数
- [modules](language/00_basic_concepts/05_program_structure/modules.md) - 模块 (C++20)
- [translation_phases](language/00_basic_concepts/05_program_structure/translation_phases.md) - 翻译阶段
- [translation_unit](language/00_basic_concepts/05_program_structure/translation_unit.md) - 翻译单元

### 06_semantics - 语义

- [as_if](language/00_basic_concepts/06_semantics/as_if.md) - as-if 规则
- [behavior_types](language/00_basic_concepts/06_semantics/behavior_types.md) - 行为类型
- [undefined_behavior](language/00_basic_concepts/06_semantics/undefined_behavior.md) - 未定义行为

### 07_multithread - 多线程

- [multithread](language/00_basic_concepts/07_multithread/multithread.md) - 多线程

---

## 01_keyword - 关键字

- [keyword](language/01_keyword/00_overview/keyword.md) - C++ 语言关键字索引

---

## 02_declarations - 声明

变量、类型、命名空间、存储类等声明语法。

### 00_overview - 总览

- [declarations](language/02_declarations/00_overview/declarations.md) - 声明总览

### 01_type_declaration - 类型声明

- [class](language/02_declarations/01_type_declaration/class.md) - 类声明
- [enum](language/02_declarations/01_type_declaration/enum.md) - 枚举
- [scoped_enum](language/02_declarations/01_type_declaration/scoped_enum.md) - 限定作用域枚举
- [struct](language/02_declarations/01_type_declaration/struct.md) - 结构体
- [typedef](language/02_declarations/01_type_declaration/typedef.md) - 类型别名
- [union](language/02_declarations/01_type_declaration/union.md) - 联合体

### 02_variable_declaration - 变量声明

- [array](language/02_declarations/02_variable_declaration/array.md) - 数组
- [bit_field](language/02_declarations/02_variable_declaration/bit_field.md) - 位域
- [pointer](language/02_declarations/02_variable_declaration/pointer.md) - 指针
- [reference](language/02_declarations/02_variable_declaration/reference.md) - 引用
- [structured_binding](language/02_declarations/02_variable_declaration/structured_binding.md) - 结构化绑定 (C++17)

### 03_storage_class - 存储类

- [extern](language/02_declarations/03_storage_class/extern.md) - 外部链接
- [static_storage_duration](language/02_declarations/03_storage_class/static_storage_duration.md) - 静态存储期
- [storage_duration](language/02_declarations/03_storage_class/storage_duration.md) - 存储期
- [thread_storage_duration](language/02_declarations/03_storage_class/thread_storage_duration.md) - 线程存储期

### 04_type_qualifier - 类型限定符

- [const](language/02_declarations/04_type_qualifier/const.md) - const 限定符
- [volatile](language/02_declarations/04_type_qualifier/volatile.md) - volatile 限定符
- [mutable](language/02_declarations/04_type_qualifier/mutable.md) - mutable 限定符

### 05_type_specifier - 类型说明符

- [auto](language/02_declarations/05_type_specifier/auto.md) - auto 类型推导
- [constexpr](language/02_declarations/05_type_specifier/constexpr.md) - constexpr
- [decltype](language/02_declarations/05_type_specifier/decltype.md) - decltype
- [inline](language/02_declarations/05_type_specifier/inline.md) - inline 说明符

### 06_attributes - 属性

- [alignas](language/02_declarations/06_attributes/alignas.md) - 对齐属性
- [attributes](language/02_declarations/06_attributes/attributes.md) - 属性总览
- [deprecated](language/02_declarations/06_attributes/deprecated.md) - 废弃属性
- [fallthrough](language/02_declarations/06_attributes/fallthrough.md) - 直落属性
- [likely](language/02_declarations/06_attributes/likely.md) - 可能分支
- [maybe_unused](language/02_declarations/06_attributes/maybe_unused.md) - 未使用属性
- [nodiscard](language/02_declarations/06_attributes/nodiscard.md) - 忽略返回值警告
- [noreturn](language/02_declarations/06_attributes/noreturn.md) - 无返回属性
- [static_assert](language/02_declarations/06_attributes/static_assert.md) - 静态断言

### 07_namespace - 命名空间

- [namespace](language/02_declarations/07_namespace/namespace.md) - 命名空间
- [namespace_alias](language/02_declarations/07_namespace/namespace_alias.md) - 命名空间别名
- [using_directive](language/02_declarations/07_namespace/using_directive.md) - using 指令

### 08_other - 其他声明

- [elaborated_type_specifier](language/02_declarations/08_other/elaborated_specifier.md) - 详细类型说明符
- [friend](language/02_declarations/08_other/friend.md) - 友元声明
- [using_declaration](language/02_declarations/08_other/using_declaration.md) - using 声明

---

## 03_initialization - 初始化

### 00_overview - 总览

- [initialization](language/03_initialization/00_overview/initialization.md) - 初始化总览

### 01_basic_forms - 基础初始化形式

- [default_initialization](language/03_initialization/01_basic_forms/default_initialization.md) - 默认初始化
- [value_initialization](language/03_initialization/01_basic_forms/value_initialization.md) - 值初始化
- [zero_initialization](language/03_initialization/01_basic_forms/zero_initialization.md) - 零初始化
- [direct_initialization](language/03_initialization/01_basic_forms/direct_initialization.md) - 直接初始化
- [copy_initialization](language/03_initialization/01_basic_forms/copy_initialization.md) - 复制初始化

### 02_special_forms - 特殊初始化场景

- [list_initialization](language/03_initialization/02_special_forms/list_initialization.md) - 列表初始化
- [aggregate_initialization](language/03_initialization/02_special_forms/aggregate_initialization.md) - 聚合初始化
- [reference_initialization](language/03_initialization/02_special_forms/reference_initialization.md) - 引用初始化
- [constant_initialization](language/03_initialization/02_special_forms/constant_initialization.md) - 常量初始化

### 03_optimization - 优化

- [copy_elision](language/03_initialization/03_optimization/copy_elision.md) - 复制消除

---

## 04_expressions - 表达式

值的计算、操作符、类型转换等。

### 00_overview - 总览

- [expressions](language/04_expressions/00_overview/expressions.md) - 表达式总览

### 01_literal - 字面量

- [bool_constant](language/04_expressions/01_literal/bool_constant.md) - 布尔常量
- [character_constant](language/04_expressions/01_literal/character_constant.md) - 字符常量
- [compound_literal](language/04_expressions/01_literal/compound_literal.md) - 复合字面量
- [escape](language/04_expressions/01_literal/escape.md) - 转义序列
- [floating_constant](language/04_expressions/01_literal/floating_constant.md) - 浮点常量
- [integer_constant](language/04_expressions/01_literal/integer_constant.md) - 整型常量
- [nullptr](language/04_expressions/01_literal/nullptr.md) - 空指针
- [string_literal](language/04_expressions/01_literal/string_literal.md) - 字符串字面量

### 02_value_category - 值类别

- [value_category](language/04_expressions/02_value_category/value_category.md) - 值类别

### 03_type_conversion - 类型转换

- [conversion](language/04_expressions/03_type_conversion/conversion.md) - 隐式转换
- [cast](language/04_expressions/03_type_conversion/cast.md) - C 风格转换
- [const_cast](language/04_expressions/03_type_conversion/const_cast.md) - const_cast
- [dynamic_cast](language/04_expressions/03_type_conversion/dynamic_cast.md) - dynamic_cast
- [reinterpret_cast](language/04_expressions/03_type_conversion/reinterpret_cast.md) - reinterpret_cast
- [static_cast](language/04_expressions/03_type_conversion/static_cast.md) - static_cast

### 04_operator - 运算符

- [operator_arithmetic](language/04_expressions/04_operator/operator_arithmetic.md) - 算术运算符
- [operator_assignment](language/04_expressions/04_operator/operator_assignment.md) - 赋值运算符
- [operator_bitwise](language/04_expressions/04_operator/operator_bitwise.md) - 位运算符
- [operator_comparison](language/04_expressions/04_operator/operator_comparison.md) - 比较运算符
- [operator_incdec](language/04_expressions/04_operator/operator_incdec.md) - 自增自减
- [operator_logical](language/04_expressions/04_operator/operator_logical.md) - 逻辑运算符
- [operator_member_access](language/04_expressions/04_operator/operator_member_access.md) - 成员访问
- [operator_other](language/04_expressions/04_operator/operator_other.md) - 其他运算符
- [operator_precedence](language/04_expressions/04_operator/operator_precedence.md) - 运算符优先级
- [operator_splice](language/04_expressions/04_operator/operator_splice.md) - 拼接运算符

### 05_special_expr - 特殊表达式

- [alignof](language/04_expressions/05_special_expr/alignof.md) - alignof 运算符
- [constant_expression](language/04_expressions/05_special_expr/constant_expression.md) - 常量表达式
- [fold](language/04_expressions/05_special_expr/fold.md) - 折叠表达式
- [sizeof](language/04_expressions/05_special_expr/sizeof.md) - sizeof 运算符
- [typeid](language/04_expressions/05_special_expr/typeid.md) - typeid 运算符

### 06_evaluation - 求值

- [eval_order](language/04_expressions/06_evaluation/eval_order.md) - 求值顺序

---

## 05_statements - 语句

程序流程控制语句。

### 00_overview - 总览

- [statements](language/05_statements/00_overview/statements.md) - 语句总览

### 01_conditional - 条件语句

- [if](language/05_statements/01_conditional/if.md) - if 语句
- [switch](language/05_statements/01_conditional/switch.md) - switch 语句

### 02_iteration - 循环语句

- [do](language/05_statements/02_iteration/do.md) - do-while 循环
- [for](language/05_statements/02_iteration/for.md) - for 循环
- [range_for](language/05_statements/02_iteration/range_for.md) - 范围 for (C++11)
- [while](language/05_statements/02_iteration/while.md) - while 循环

### 03_jump - 跳转语句

- [break](language/05_statements/03_jump/break.md) - break 语句
- [continue](language/05_statements/03_jump/continue.md) - continue 语句
- [goto](language/05_statements/03_jump/goto.md) - goto 语句
- [return](language/05_statements/03_jump/return.md) - return 语句

### 04_special - 特殊语句

- [contract_assert](language/05_statements/04_special/contract_assert.md) - 契约断言 (C++26)
- [transactional_memory](language/05_statements/04_special/transactional_memory.md) - 事务内存

---

## 06_functions - 函数

函数定义、重载、Lambda 等。

### 00_overview - 总览

- [functions](language/06_functions/00_overview/functions.md) - 函数总览

### 01_declaration - 函数声明

- [function](language/06_functions/01_declaration/function.md) - 函数声明与定义
- [default_arguments](language/06_functions/01_declaration/default_arguments.md) - 默认参数

### 02_overloading - 重载

- [overload_resolution](language/06_functions/02_overloading/overload_resolution.md) - 重载决议
- [overloaded_address](language/06_functions/02_overloading/overloaded_address.md) - 重载函数地址
- [adl](language/06_functions/02_overloading/adl.md) - 参数依赖查找 (ADL)

### 03_attributes - 函数属性

- [variadic_arguments](language/06_functions/03_attributes/variadic_arguments.md) - 可变参数

### 04_advanced - 高级特性

- [lambda](language/06_functions/04_advanced/lambda.md) - Lambda 表达式
- [coroutines](language/06_functions/04_advanced/coroutines.md) - 协程 (C++20)

---

## 07_classes - 类

面向对象编程核心。

### 00_overview - 总览

- [classes](language/07_classes/00_overview/classes.md) - 类总览

### 01_structure - 类基础结构

- [class](language/07_classes/01_structure/class.md) - 类定义
- [access](language/07_classes/01_structure/access.md) - 访问控制
- [data_members](language/07_classes/01_structure/data_members.md) - 数据成员
- [member_functions](language/07_classes/01_structure/member_functions.md) - 成员函数
- [static](language/07_classes/01_structure/static.md) - 静态成员
- [this](language/07_classes/01_structure/this.md) - this 指针
- [bit_field](language/07_classes/01_structure/bit_field.md) - 位域
- [union](language/07_classes/01_structure/union.md) - 联合体

### 02_constructors - 构造与析构

- [default_constructor](language/07_classes/02_constructors/default_constructor.md) - 默认构造函数
- [copy_constructor](language/07_classes/02_constructors/copy_constructor.md) - 复制构造函数
- [move_constructor](language/07_classes/02_constructors/move_constructor.md) - 移动构造函数
- [converting_constructor](language/07_classes/02_constructors/converting_constructor.md) - 转换构造函数
- [initializer_list](language/07_classes/02_constructors/initializer_list.md) - 初始化列表
- [destructor](language/07_classes/02_constructors/destructor.md) - 析构函数
- [explicit](language/07_classes/02_constructors/explicit.md) - explicit 关键字

### 03_operators - 赋值运算符

- [copy_operator](language/07_classes/03_operators/copy_operator.md) - 复制赋值
- [move_operator](language/07_classes/03_operators/move_operator.md) - 移动赋值

### 04_inheritance - 继承机制

- [derived_class](language/07_classes/04_inheritance/derived_class.md) - 派生类
- [virtual](language/07_classes/04_inheritance/virtual.md) - 虚函数
- [abstract_class](language/07_classes/04_inheritance/abstract_class.md) - 抽象类
- [override](language/07_classes/04_inheritance/override.md) - override 关键字
- [final](language/07_classes/04_inheritance/final.md) - final 关键字
- [using_declaration](language/07_classes/04_inheritance/using_declaration.md) - using 声明
- [ebo](language/07_classes/04_inheritance/ebo.md) - 空基类优化
- [injected_class_name](language/07_classes/04_inheritance/injected_class_name.md) - 注入类名

### 05_relationships - 类关系

- [friend](language/07_classes/05_relationships/friend.md) - 友元
- [nested_classes](language/07_classes/05_relationships/nested_classes.md) - 嵌套类

---

## 08_templates - 模板

泛型编程核心。

### 00_overview - 总览

- [templates](language/08_templates/00_overview/templates.md) - 模板总览

### 01_basics - 基础概念

- [template_parameters](language/08_templates/01_basics/template_parameters.md) - 模板参数
- [template_argument_deduction](language/08_templates/01_basics/template_argument_deduction.md) - 模板参数推导
- [ctad](language/08_templates/01_basics/ctad.md) - 类模板参数推导

### 02_types - 模板类型

- [class_template](language/08_templates/02_types/class_template.md) - 类模板
- [function_template](language/08_templates/02_types/function_template.md) - 函数模板
- [member_template](language/08_templates/02_types/member_template.md) - 成员模板
- [variable_template](language/08_templates/02_types/variable_template.md) - 变量模板

### 03_specialization - 特化机制

- [template_specialization](language/08_templates/03_specialization/template_specialization.md) - 模板特化
- [partial_specialization](language/08_templates/03_specialization/partial_specialization.md) - 部分特化

### 04_variadic - 可变参数模板

- [parameter_pack](language/08_templates/04_variadic/parameter_pack.md) - 参数包
- [sizeof...](language/08_templates/04_variadic/sizeof....md) - sizeof... 运算符
- [fold](language/08_templates/04_variadic/fold.md) - 折叠表达式
- [pack_indexing](language/08_templates/04_variadic/pack_indexing.md) - 包索引 (C++26)

### 05_advanced - 高级机制

- [dependent_name](language/08_templates/05_advanced/dependent_name.md) - 依赖名称
- [sfinae](language/08_templates/05_advanced/sfinae.md) - SFINAE
- [template_metaprogramming](language/08_templates/05_advanced/template_metaprogramming.md) - 模板元编程

### 06_constraints - C++20 约束

- [constraints](language/08_templates/06_constraints/constraints.md) - 约束
- [requires](language/08_templates/06_constraints/requires.md) - requires 表达式

---

## 09_exceptions - 异常

异常处理机制。

### 00_overview - 总览

- [exceptions](language/09_exceptions/00_overview/exceptions.md) - 异常处理总览

### 01_handling - 异常捕获

- [try](language/09_exceptions/01_handling/try.md) - try 语句块
- [catch](language/09_exceptions/01_handling/catch.md) - catch 语句块

### 02_throwing - 异常抛出

- [throw](language/09_exceptions/02_throwing/throw.md) - throw 表达式

### 03_specification - 异常规范

- [noexcept](language/09_exceptions/03_specification/noexcept.md) - noexcept 运算符
- [noexcept_spec](language/09_exceptions/03_specification/noexcept_spec.md) - noexcept 函数声明
- [except_spec](language/09_exceptions/03_specification/except_spec.md) - 动态异常规范（已废弃）

---

## 10_preprocessor - 预处理器

编译前处理指令。

### 00_overview - 总览

- [preprocessor](language/10_preprocessor/00_overview/preprocessor.md) - 预处理器总览

### 01_file_handling - 文件处理

- [include](language/10_preprocessor/01_file_handling/include.md) - #include 指令

### 02_macro - 宏

- [replace](language/10_preprocessor/02_macro/replace.md) - 宏替换

### 03_conditional - 条件编译

- [conditional](language/10_preprocessor/03_conditional/conditional.md) - 条件编译

### 04_auxiliary - 辅助指令

- [error](language/10_preprocessor/04_auxiliary/error.md) - #error 指令
- [line](language/10_preprocessor/04_auxiliary/line.md) - #line 指令

### 05_implementation - 实现细节

- [impl](language/10_preprocessor/05_implementation/impl.md) - 预定义宏

---

## 11_idioms - 惯用法

C++ 编程惯用法和最佳实践。

### 00_overview - 总览

- [idioms](language/11_idioms/00_overview/idioms.md) - 惯用法总览

### 01_patterns - 设计模式

- [crtp](language/11_idioms/01_patterns/crtp.md) - CRTP 奇异递归模板模式
- [pimpl](language/11_idioms/01_patterns/pimpl.md) - Pimpl 指向实现的指针

### 02_resource_management - 资源管理

- [raii](language/11_idioms/02_resource_management/raii.md) - RAII 资源获取即初始化
- [rule_of_three](language/11_idioms/02_resource_management/rule_of_three.md) - 三法则/五法则

### 03_principles - 设计原则

- [zero_overhead_principle](language/11_idioms/03_principles/zero_overhead_principle.md) - 零开销原则

---

## 12_miscellaneous - 杂项

### 00_overview - 总览

- [acronyms](language/12_miscellaneous/00_overview/acronyms.md) - 缩写术语表
- [history](language/12_miscellaneous/00_overview/history.md) - C++ 语言历史

### 01_specification - 规范与扩展

- [extending_std](language/12_miscellaneous/01_specification/extending_std.md) - 扩展标准库
- [ndr](language/12_miscellaneous/01_specification/ndr.md) - 无需诊断

---

## Toolchain - 工具链

C++ 语言开发工具链文档，共 **43** 篇文档，按工具类型组织。

### 目录结构

```
cpp/toolchain/
├── 00_build/              # 构建系统
├── 01_compiler/           # 编译器
├── 02_package/            # 包管理
├── 03_debug/              # 调试工具
├── 04_format/             # 代码格式化
├── 05_static_analysis/    # 静态分析
├── 06_dynamic_analysis/   # 动态分析
├── 07_test/               # 测试框架
├── 08_doc/                # 文档工具
├── 09_binary/             # 二进制工具
├── 10_llvm/               # LLVM 工具链
└── 11_coverage/           # 代码覆盖率
```

### 00_build - 构建系统

- [cmake](toolchain/00_build/cmake.md) - CMake 构建系统
- [make](toolchain/00_build/make.md) - Make 构建工具
- [ninja](toolchain/00_build/ninja.md) - Ninja 构建系统
- [meson](toolchain/00_build/meson.md) - Meson 构建系统
- [bazel](toolchain/00_build/bazel.md) - Bazel 构建系统
- [xmake](toolchain/00_build/xmake.md) - XMake 构建系统

### 01_compiler - 编译器

- [gcc](toolchain/01_compiler/gcc.md) - GCC 编译器
- [clang](toolchain/01_compiler/clang.md) - Clang 编译器
- [msvc](toolchain/01_compiler/msvc.md) - MSVC 编译器
- [intel](toolchain/01_compiler/intel.md) - Intel 编译器

### 02_package - 包管理

- [vcpkg](toolchain/02_package/vcpkg.md) - vcpkg 包管理器
- [conan](toolchain/02_package/conan.md) - Conan 包管理器
- [cpm](toolchain/02_package/cpm.md) - CPM.cmake 包管理

### 03_debug - 调试工具

- [gdb](toolchain/03_debug/gdb.md) - GDB 调试器
- [lldb](toolchain/03_debug/lldb.md) - LLDB 调试器
- [rr](toolchain/03_debug/rr.md) - RR 录制重放调试器
- [windbg](toolchain/03_debug/windbg.md) - WinDbg 调试器

### 04_format - 代码格式化

- [clang-format](toolchain/04_format/clang-format.md) - ClangFormat 格式化工具
- [astyle](toolchain/04_format/astyle.md) - Artistic Style 格式化工具

### 05_static_analysis - 静态分析

- [clang-tidy](toolchain/05_static_analysis/clang-tidy.md) - Clang-Tidy 静态分析
- [cppcheck](toolchain/05_static_analysis/cppcheck.md) - Cppcheck 静态分析
- [pvs-studio](toolchain/05_static_analysis/pvs-studio.md) - PVS-Studio 静态分析
- [coverity](toolchain/05_static_analysis/coverity.md) - Coverity 静态分析

### 06_dynamic_analysis - 动态分析

- [valgrind](toolchain/06_dynamic_analysis/valgrind.md) - Valgrind 内存检测
- [sanitizers](toolchain/06_dynamic_analysis/sanitizers.md) - Sanitizers 运行时检测
- [perf](toolchain/06_dynamic_analysis/perf.md) - Perf 性能分析
- [gperftools](toolchain/06_dynamic_analysis/gperftools.md) - GPerftools 性能分析
- [gprof](toolchain/06_dynamic_analysis/gprof.md) - GProf 性能分析

### 07_test - 测试框架

- [gtest](toolchain/07_test/gtest.md) - Google Test 测试框架
- [doctest](toolchain/07_test/doctest.md) - Doctest 测试框架
- [catch2](toolchain/07_test/catch2.md) - Catch2 测试框架

### 08_doc - 文档工具

- [doxygen](toolchain/08_doc/doxygen.md) - Doxygen 文档生成
- [sphinx](toolchain/08_doc/sphinx.md) - Sphinx 文档系统

### 09_binary - 二进制工具

- [nm](toolchain/09_binary/nm.md) - nm 符号表查看
- [strip](toolchain/09_binary/strip.md) - strip 符号移除
- [ldd](toolchain/09_binary/ldd.md) - ldd 动态库依赖
- [readelf](toolchain/09_binary/readelf.md) - readelf ELF 分析
- [objdump](toolchain/09_binary/objdump.md) - objdump 反汇编

### 10_llvm - LLVM 工具链

- [llvm](toolchain/10_llvm/llvm.md) - LLVM 工具链
- [libcxx](toolchain/10_llvm/libcxx.md) - libc++ 标准库
- [libstdcxx](toolchain/10_llvm/libstdcxx.md) - libstdc++ 标准库

### 11_coverage - 代码覆盖率

- [gcov](toolchain/11_coverage/gcov.md) - Gcov 覆盖率工具
- [lcov](toolchain/11_coverage/lcov.md) - Lcov 覆盖率报告

---

## 学习路径

```
基础入门 → 基础语法 → 流程控制 → 代码组织 → 面向对象 → 泛型编程 → 错误处理 → 编译机制 → 最佳实践
    ↓           ↓           ↓          ↓          ↓          ↓           ↓          ↓          ↓
basic_   declarations  statements  functions  classes   templates  exceptions preprocessor idioms
concepts
```

## 版本说明

本文档基于 C++17/C++20/C++23 标准编写，部分特性标注了版本要求。

---

*最后更新：2026-04-06*