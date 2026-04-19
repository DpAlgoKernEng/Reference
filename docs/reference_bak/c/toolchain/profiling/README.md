## profiling 性能分析
- [profiling 性能分析](#profiling-性能分析)
  - [性能分析](#性能分析)
  - [静态代码检测](#静态代码检测)

### 性能分析
- gnu gprof 这是一个 gnu 的性能分析工具，主要用于分析程序的函数调用关系，以及每个函数的运行时间等。
  > Flat Profile 功能列表，每个功能的调用次数，每次调用的平均执行时间，以及总执行时间。
  
  > Call Graph 记录哪个函数调用了哪个函数，以及调用发生的次数。这有助于跟踪函数的调用路径。
  
  > 注解源代码 通过注解源代码，可以看到每行代码的执行次数。
  
  > 使用 gprof，你需要在编译代码时使用 -pg 选项来启用分析 gcc -pg -o output source.c 运行程序。这将生成一个名为 gmon.out 的文件，你可以使用 gprof 来分析这个文件。gprof output gmon.out > analysis.txt
  
  > gprof 是一个非常有用的工具，但它也有一些限制。例如，它不能正确地处理多线程应用，也不能测量内核函数或硬件中断的时间。此外，它可能无法准确地测量优化后的代码，因为编译器可能会更改函数的调用结构。

- linux perf 可以收集 CPU 使用情况、缓存命中率、分支预测错误等多种性能数据。是 linux 内核提供的一个强大的性能分析工具，基于 linux 内核的性能计数器子系统 (PCL)。主要功能如下
  > CPU 性能计数器
  
  > 硬件性能计数器
  
  > 软件性能计数器 (比如 Linux 内核的 tracepoints)
  
  > 动态追踪 (如 uprobes)
  
  > CPU cache 命中/未命中率
  
  > 指令执行周期数
  
  > CPU 迁移和调度延迟
  
  > 简单的 perf 使用示例，用于测量一个程序的 CPU cache 命中率  perf record -e cache-misses ./your_program 、perf report
  
  > 虽然 perf 是一个强大的工具，但是它仍然有一些限制，例如它并不能直接测量诸如内存使用量，I/O 延迟等方面的性能。

- valgrind 工具套件包括 7 个生产级工具：一个内存错误检测器、两个线程错误检测器、一个缓存和分支预测分析器、一个调用图生成缓存和分支预测分析器以及两个不同的堆分析器。
  > Memcheck 可检测内存管理问题，当程序在 Memcheck 的监督下运行时，将检查所有内存读写操作，并拦截对 malloc/new/free/delete 的调用。

  > Cachegrind 是一个缓存分析器。它对 CPU 中的 I1、D1 和 L2 缓存进行详细模拟，因此可以准确找出代码中缓存未命中的来源。

  > Callgrind 是 Cachegrind 的一个扩展。它提供了 Cachegrind 提供的所有信息，以及有关调用图的额外信息。另外还有一个很棒的可视化工具 KCachegrind，它能更好地概览 Callgrind 收集的数据；它还可用于可视化 Cachegrind 的输出。
  
  > Massif 是一个堆分析器。它通过定期拍摄程序堆的快照来执行详细的堆分析。它会生成一个显示堆使用情况随时间变化的图表，包括有关程序哪些部分负责最多内存分配的信息。
  
  > Helgrind 是一个线程调试器，用于查找多线程程序中的数据争用。它查找由多个 (POSIX p-) 线程访问但找不到一致使用的 (pthread_mutex_) 锁的内存位置。
  
  > DRD 是一种用于检测多线程 C 和 C++ 程序中的错误的工具。该工具适用于任何使用 POSIX 线程原语或使用基于 POSIX 线程原语构建的线程概念的程序。
  
  > DHAT 是一款用于检查程序如何使用堆分配的工具。它跟踪已分配的块，并检查每次内存访问以查找要访问的块（如果有）。它附带一个 GUI，方便您探索分析结果。

  > 使用 Valgrind 来检查内存泄漏，可以执行 valgrind --leak-check=full --show-leak-kinds=all your_program

- gperftools 是一个高性能多线程 malloc() 实现，以及一些非常漂亮的性能分析工具包含：一个 tcmalloc 内存申请、一个堆分析器、一个堆检查器、一个 CPU 分析器。
  > TCMALLOC 一个快速、多线程友好的内存分配器。TCMalloc比默认的系统内存分配器性能更好，特别是在多线程环境下。只需链接 -ltcmalloc 或 -ltcmalloc_minimal 即可。
  
  > HEAP PROFILER 一个用于研究如何程序使用内存的工具，参阅 docs/heapprofile.html 堆分析器并分析其输出。

  > HEAP CHECKER 参阅 docs/heap_checker.html 堆检查器分析堆泄漏等(弃用)。考虑使用 ASan 全称 AddressSanitizer，是一种内存错误检测工具，目的是帮助开发者检测和调试内存相关的问题，如使用未分配的内存、使用已释放的内存、堆内存溢出等。ASan 是由 Google 开发的，广泛用于 C、C++ 等语言的代码中。

  > CPU PROFILER 一个用于确定程序最消耗CPU时间的部分并生成调用图的工具，参阅 docs/cpuprofile.html 分析器并分析其输出。

  > Perftools Unittests 一套对工具进行测试的单元测试工具。

- sanitizer 集合 Google 的 Sanitizer 集合包括其他有用的检查工具，如 AddressSanitizer、LeakSanitizer、ThreadSanitizer 和 MemorySanitizer、HWASAN、UBSan。是一组基于 Clang 和 LLVM 的工具，用于动态检测各种类型的运行时错误，可以检测出那些只有在程序运行时才会出现的问题。
  > AddressSanitizer（ASan）是一个快速的内存错误检测工具。它包括一个编译 instrumentation 模块和一个提供 malloc()/free() 替代项的运行时库，对内存的读写操作进行检测和记录。当程序运行时，ASan会监测内存访问，一旦发现内存访问错误，比如越界访问、释放后再次访问等，会立即输出错误信息并中断程序执行，同时提供详细的报告帮助开发者定位问题的源头。要使用 ASan，只需要使用支持 ASan 的编译器，如 Clang 或 GCC，并开启ASan 相关的编译选项即可。使用 Clang 编译器，在终端执行以下命令 clang -fsanitize=address -g your_program.c -o your_program、使用 GCC 编译器，在终端执行以下命令 gcc -fsanitize=address -g your_program.c -o your_program
  
  > LeakSanitizer 这是一个内存泄漏检测器，可以无需更改代码或重编译就能检测出程序中的内存泄漏问题。它也被集成到了 LLVM/Clang 编译器中。 LeakSanitizer 已经被集成到 GCC 和 Clang 编译器中，可以通过在编译时添加特定的标志来启用，如 -fsanitize=leak。编译后，你的程序会在运行时进行检查，并在检测到内存泄漏时报告。

  > ThreadSanitizer 检测线程竞态和死锁问题

  > MemorySanitizer 检测未初始化内存的使用

  > HWASAN 即硬件辅助 AddressSanitizer，是 AddressSanitizer 的一个较新版本，消耗的内存更少
  
  > UBSan UndefinedBehaviorSanitizer

- visual studio profiler 是一种性能分析工具，它可以帮助开发人员识别和解决应用程序中的性能问题。Visual Studio Profiler提供了多种分析选项，包括CPU使用情况、内存使用情况、线程活动等。
  > Visual Studio Profiler 的优势在于它与 Visual Studio 集成紧密，可以方便地进行代码级别的性能分析和调试。它提供了丰富的分析选项和报告功能，帮助开发人员快速定位和解决性能问题。

### 静态代码检测
- cppcheck 是一款静态 C/C++ 代码分析工具。用于在不实际运行程序的情况下检测C和C++代码中的错误。cppcheck可以检测的错误类型包括内存泄漏、未初始化的变量、未使用的函数和超出数组边界等。cppcheck的特点是它尽可能减少误报，但这也意味着它可能会错过一些不那么常见的错误类型。
  > 检查 C/C++ 代码中的 bug，例如内存泄漏、缓冲区溢出等。
  
  > 检查代码中的未使用的函数、变量等。
  
  > 检查语法错误和逻辑错误。
  
  > 提供命令行界面和图形用户界面两种使用方式。
  
  > 支持插件扩展，可以通过插件提供额外的检查功能。
  
  > 命令行方式在终端中输入 cppcheck 命令，加上相应的参数和要检查的文件名即可。例如 cppcheck --enable=all myfile.cpp 或 图形用户界面方式通过 Cppcheck 的 GUI 程序进行交互式的检查和修改。

- clang-tidy 是基于 Clang 和 LLVM 的 C++"linter" 工具，它既可以进行静态代码分析，也可以进行一些自动代码重构。包含大量的检查器，每个检查器都关注代码中的某种特定问题，例如代码样式问题、可维护性问题、潜在的编程错误、编程实践问题，性能问题等。clang-tidy 的一个优点是它可以很容易地进行定制，你可以选择启用或禁用哪些检查器，甚至可以编写自己的检查器。
  > 易于扩展 clang-tidy 有许多内置的检查器，这些检查器可以用来检测不同类型的问题。此外，你还可以编写自己的检查器，以便对特定的编程实践进行检查。
  
  > 现代化工具 clang-tidy 可以帮助你自动现代化代码，将旧的 C++ 代码自动更新以使用新的 C++ 语言特性。
  
  > 重构和诊断工具 clang-tidy 不仅可以找到问题，还可以自动修复某些类型的问题。它还可以生成易于理解的诊断信息。

  > clang-tidy 可以在命令行中使用，也可以集成到各种集成开发环境（IDE）中 clang-tidy source.cpp -- -Iinclude_directory

  > clang-tidy 可以使用一个 .clang-tidy 文件进行配置，此文件应该放在你的项目目录下。在这个文件中，你可以启用或禁用特定的检查器，也可以设置检查器的选项

perf
https://zhuanlan.zhihu.com/p/471379451

Valgrind
https://mp.weixin.qq.com/s/1jHD6oUH76d4waDWHqSdrQ

gperftools
https://weakyon.com/2018/08/16/summarize-of-gperftools.html

sanitizers
https://www.cnblogs.com/thammer/p/17117286.html

cppcheck
https://wenbo1188.github.io/2017/07/23/cppcheck-manual-chinese/

clang-tidy
https://blog.csdn.net/sexyluna/article/details/132001613
