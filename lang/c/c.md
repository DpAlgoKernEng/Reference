# C 语言参考文档

C 语言完整参考文档，共 **147** 篇文档（语言参考 105 篇 + 工具链 42 篇），按学习路径组织。

## Language - 语言参考

语言参考 C 语言核心语法和语义文档，共 **105** 篇文档，涵盖基础概念、声明、初始化、表达式、语句、函数、预处理器等方面。

## 目录结构

```
c/language/
├── 00_basic_concepts/       # 基础概念（17篇）
├── 01_keyword/              # 关键字索引（1篇）
├── 02_declarations/         # 声明（26篇）
├── 03_initialization/       # 初始化（4篇）
├── 04_expressions/          # 表达式（27篇）
├── 05_statements/           # 语句（10篇）
├── 06_functions/            # 函数（6篇）
├── 07_preprocessor/         # 预处理器（8篇）
└── 08_miscellaneous/        # 杂项（6篇）
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

### 03_memory_object - 内存与对象

- [lifetime](language/00_basic_concepts/03_memory_object/lifetime.md) - 对象生命周期
- [memory_model](language/00_basic_concepts/03_memory_object/memory_model.md) - 内存模型
- [object](language/00_basic_concepts/03_memory_object/object.md) - 对象模型

### 04_name_scope - 名称与作用域

- [lookup](language/00_basic_concepts/04_name_scope/lookup.md) - 名称查找
- [scope](language/00_basic_concepts/04_name_scope/scope.md) - 作用域

### 05_program_structure - 程序结构

- [main_function](language/00_basic_concepts/05_program_structure/main_function.md) - main 函数
- [translation_phases](language/00_basic_concepts/05_program_structure/translation_phases.md) - 翻译阶段

### 06_semantics - 语义

- [as_if](language/00_basic_concepts/06_semantics/as_if.md) - as-if 规则
- [undefined_behavior](language/00_basic_concepts/06_semantics/undefined_behavior.md) - 未定义行为

---

## 01_keyword - 关键字

- [keyword](language/01_keyword/00_overview/keyword.md) - C 语言关键字索引

---

## 02_declarations - 声明

变量、类型、存储类等声明语法。

### 00_overview - 总览

- [declarations](language/02_declarations/00_overview/declarations.md) - 声明总览

### 01_type_declaration - 类型声明

- [enum](language/02_declarations/01_type_declaration/enum.md) - 枚举类型
- [struct](language/02_declarations/01_type_declaration/struct.md) - 结构体
- [typedef](language/02_declarations/01_type_declaration/typedef.md) - 类型别名
- [union](language/02_declarations/01_type_declaration/union.md) - 联合体

### 02_variable_declaration - 变量声明

- [array](language/02_declarations/02_variable_declaration/array.md) - 数组
- [bit_field](language/02_declarations/02_variable_declaration/bit_field.md) - 位域
- [pointer](language/02_declarations/02_variable_declaration/pointer.md) - 指针

### 03_storage_class - 存储类

- [extern](language/02_declarations/03_storage_class/extern.md) - 外部链接
- [static_storage_duration](language/02_declarations/03_storage_class/static_storage_duration.md) - 静态存储期
- [storage_duration](language/02_declarations/03_storage_class/storage_duration.md) - 存储期
- [thread_storage_duration](language/02_declarations/03_storage_class/thread_storage_duration.md) - 线程存储期

### 04_type_qualifier - 类型限定符

- [atomic](language/02_declarations/04_type_qualifier/atomic.md) - 原子类型
- [const](language/02_declarations/04_type_qualifier/const.md) - const 限定符
- [restrict](language/02_declarations/04_type_qualifier/restrict.md) - restrict 限定符
- [volatile](language/02_declarations/04_type_qualifier/volatile.md) - volatile 限定符

### 05_type_specifier - 类型说明符

- [constexpr](language/02_declarations/05_type_specifier/constexpr.md) - constexpr (C23)

### 06_attributes - 属性

- [alignas](language/02_declarations/06_attributes/alignas.md) - 对齐属性
- [attributes](language/02_declarations/06_attributes/attributes.md) - 属性总览
- [deprecated](language/02_declarations/06_attributes/deprecated.md) - 废弃属性
- [fallthrough](language/02_declarations/06_attributes/fallthrough.md) - 直落属性
- [maybe_unused](language/02_declarations/06_attributes/maybe_unused.md) - 未使用属性
- [nodiscard](language/02_declarations/06_attributes/nodiscard.md) - 忽略返回值警告
- [noreturn](language/02_declarations/06_attributes/noreturn.md) - 无返回属性
- [reproducible](language/02_declarations/06_attributes/reproducible.md) - 可复现属性
- [static_assert](language/02_declarations/06_attributes/static_assert.md) - 静态断言

---

## 03_initialization - 初始化

### 00_overview - 总览

- [initialization](language/03_initialization/initialization.md) - 初始化总览

### 初始化方式

- [array_initialization](language/03_initialization/array_initialization.md) - 数组初始化
- [scalar_initialization](language/03_initialization/scalar_initialization.md) - 标量初始化
- [struct_initialization](language/03_initialization/struct_initialization.md) - 结构体初始化

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
- [nullptr](language/04_expressions/01_literal/nullptr.md) - 空指针 (C23)
- [string_literal](language/04_expressions/01_literal/string_literal.md) - 字符串字面量

### 02_value_category - 值类别

- [value_category](language/04_expressions/02_value_category/value_category.md) - 值类别

### 03_type_conversion - 类型转换

- [cast](language/04_expressions/03_type_conversion/cast.md) - 强制转换
- [conversion](language/04_expressions/03_type_conversion/conversion.md) - 隐式转换

### 04_operator - 运算符

- [operator_alternative](language/04_expressions/04_operator/operator_alternative.md) - 运算符替代
- [operator_arithmetic](language/04_expressions/04_operator/operator_arithmetic.md) - 算术运算符
- [operator_assignment](language/04_expressions/04_operator/operator_assignment.md) - 赋值运算符
- [operator_comparison](language/04_expressions/04_operator/operator_comparison.md) - 比较运算符
- [operator_incdec](language/04_expressions/04_operator/operator_incdec.md) - 自增自减
- [operator_logical](language/04_expressions/04_operator/operator_logical.md) - 逻辑运算符
- [operator_member_access](language/04_expressions/04_operator/operator_member_access.md) - 成员访问
- [operator_other](language/04_expressions/04_operator/operator_other.md) - 其他运算符
- [operator_precedence](language/04_expressions/04_operator/operator_precedence.md) - 运算符优先级

### 05_special_expr - 特殊表达式

- [alignof](language/04_expressions/05_special_expr/alignof.md) - alignof 运算符
- [constant_expression](language/04_expressions/05_special_expr/constant_expression.md) - 常量表达式
- [generic](language/04_expressions/05_special_expr/generic.md) - 泛型选择
- [sizeof](language/04_expressions/05_special_expr/sizeof.md) - sizeof 运算符
- [typeof](language/04_expressions/05_special_expr/typeof.md) - typeof 运算符

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
- [while](language/05_statements/02_iteration/while.md) - while 循环

### 03_jump - 跳转语句

- [break](language/05_statements/03_jump/break.md) - break 语句
- [continue](language/05_statements/03_jump/continue.md) - continue 语句
- [goto](language/05_statements/03_jump/goto.md) - goto 语句
- [return](language/05_statements/03_jump/return.md) - return 语句

---

## 06_functions - 函数

函数定义、声明与调用。

### 00_overview - 总览

- [functions](language/06_functions/00_overview/functions.md) - 函数总览

### 01_declaration - 函数声明

- [function_declaration](language/06_functions/01_declaration/function_declaration.md) - 函数声明
- [function_definition](language/06_functions/01_declaration/function_definition.md) - 函数定义

### 02_attributes - 函数属性

- [inline](language/06_functions/02_attributes/inline.md) - 内联函数
- [_noreturn](language/06_functions/02_attributes/_noreturn.md) - 无返回函数

### 03_special - 特殊函数

- [variadic](language/06_functions/03_special/variadic.md) - 可变参数函数

---

## 07_preprocessor - 预处理器

编译前处理指令。

### 00_overview - 总览

- [preprocessor](language/07_preprocessor/00_overview/preprocessor.md) - 预处理器总览

### 01_file_handling - 文件处理

- [embed](language/07_preprocessor/01_file_handling/embed.md) - #embed 指令 (C23)
- [include](language/07_preprocessor/01_file_handling/include.md) - #include 指令

### 02_macro - 宏

- [replace](language/07_preprocessor/02_macro/replace.md) - 宏替换

### 03_conditional - 条件编译

- [conditional](language/07_preprocessor/03_conditional/conditional.md) - 条件编译

### 04_auxiliary - 辅助指令

- [error](language/07_preprocessor/04_auxiliary/error.md) - #error 指令
- [line](language/07_preprocessor/04_auxiliary/line.md) - #line 指令

### 05_implementation - 实现细节

- [impl](language/07_preprocessor/05_implementation/impl.md) - 预定义宏

---

## 08_miscellaneous - 杂项

### 00_overview - 总览

- [history](language/08_miscellaneous/00_overview/history.md) - C 语言历史

### 01_specification - 规范与扩展

- [analyzability](language/08_miscellaneous/01_specification/analyzability.md) - 可分析性
- [asm](language/08_miscellaneous/01_specification/asm.md) - 内联汇编
- [conformance](language/08_miscellaneous/01_specification/conformance.md) - 标准一致性
- [file_scope](language/08_miscellaneous/01_specification/file_scope.md) - 文件作用域
- [ndr](language/08_miscellaneous/01_specification/ndr.md) - 无需诊断

---

## Toolchain - 工具链

C 语言开发工具链文档，共 **42** 篇文档，按工具类型组织。

### 目录结构

```
c/toolchain/
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
- [glibc](toolchain/10_llvm/glibc.md) - GLibc 标准库

### 11_coverage - 代码覆盖率

- [gcov](toolchain/11_coverage/gcov.md) - Gcov 覆盖率工具
- [lcov](toolchain/11_coverage/lcov.md) - Lcov 覆盖率报告

---

## 学习路径

```
基础入门 → 声明与初始化 → 表达式运算 → 流程控制 → 函数组织 → 预处理 → 扩展知识
    ↓           ↓            ↓           ↓          ↓          ↓
basic_   declarations  expressions  statements  functions  preprocessor
concepts
```

## 版本说明

本文档基于 C11/C17/C23 标准编写，部分特性标注了版本要求。

---

*最后更新：2026-04-06*