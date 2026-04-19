## clangd、CodeLLDB 插件配置
VSCode 上使用 clangd、CodeLLDB 插件搭建 C/C++ 开发环境`Clang + CMake + Make/Ninja + LLDB + VSCode + clangd + CodeLLDB`的方式。   
- 微软插件 C/C++ Extension Pack 但是在源文件很多时比较卡顿加载很慢、更新频率低，不推荐。   
- LLVM 插件 clangd 整体体验不错，而且有 clang 全家桶带来的特色功能：自动插入头文件、clang-format、clang-tidy 等功能。clangd 这些插件的原理都是通过分析构建系统生成的`complie_command.json`文件，这个文件里定义了项目所有 C/C++ 文件的编译参数。    

### 1、先决条件配置
- 安装 VSCode 官网: https://code.visualstudio.com/docs/setup/mac.       
- 安装 clangd 和 CodeLLDB 插件等。    
- 安装 clang 编译器，并检查编译器的版本。  
- 安装 clangd 语言服务器、 lldb调试器。  
- 安装 CMake, Make 编译构建工具。    
  ```bash 
  # 使用 macos 上的 xcode 开发工具包安装 clang 编译器
  xcode-select --install
  
  # 运行下列命令检查 clang 版本
  clang --version

  # 运行下列命令检查 语言服务器 版本
  clangd --version
  
  # 运行下列命令检查 调试工具 版本
  lldb -v

  # 运行下列命令查看 cmake 版本
  cmake --version

  # 运行下列命令检查 编译构建工具 版本
  make -v
  ninja --version
  ```
  > `clang`: clang 是一种用于 C/C++ 和 Objective-C 编程语言的编译器前端。它被设计为一个快速、高效和高度可定制的编译器，提供出色的诊断和错误信息 clang 是`LLVM`项目的一部分`LLVM`是一个模块化和可重用的编译器和工具链技术集合。   
  > `clangd`: clangd 是一个基于 clang 编译器的语言服务器，用于提供 C/C++ 语言的代码补全、语义分析和代码导航等功能。clangd 通过解析源代码并构建语法树和语义图来理解代码，并根据用户的输入提供相关的代码建议和信息。它还支持跳转到定义、查找引用、重构等功能，帮助开发人员更高效地编写和维护 C/C++ 代码。   
  > `lldb`: lldb 是一个开源的调试器，用于调试 C/C++、Objective-C 和 Swift 等编程语言的应用程序。它是在`LLVM`项目的基础上开发的，和 GDB 功能类似。   
  > `CMake`: CMake 是一个跨平台的开源构建工具，通过 CMakeLists.txt 生成 Make 或 Ninja 构建程序所需要的文件如 Makefile 文件。    
  > `Make`: Make、Ninja 是一个跨平台的最早的构建工具，通过 Makefile 的文本文件来描述源代码文件之间的依赖关系和构建规则。   

### 2、编译调试设置
- clangd 和 CodeLLDB 插件配置环境有分两种情况: 配置基础的 C/C++ 项目、配置基于 CMake 构建的项目。   
- 配置基础的 C/C++ 项目需要在`.vscode`文件下手动配置以下四个文件`settings.json`、`tasks.json`、`launch.json`、`compile_commands.json`即就是自己手动配置 clangd 所依赖的服务数据库文件、clangd 参数配置、clang 编译构建配置、lldb 调试配置文件等。   
  > `settings.json`(配置 clangd 服务设置) 
    ```bash
    {
      "cmake.generator": "Unix Makefiles",
      // 是否检查插件冲突
      "clangd.detectExtensionConflicts": true,
      // 查找的头文件路径，每一项前缀 -I
      "clangd.fallbackFlags": [],
      // 开启粘贴保存自动格式化
      "editor.formatOnPaste": true,
      "editor.formatOnSave": true,
      "editor.formatOnType": true,
      "C_Cpp.errorSquiggles": "Disabled",
      "C_Cpp.intelliSenseEngineFallback": "Disabled",
      "C_Cpp.intelliSenseEngine": "Disabled",
      "C_Cpp.autocomplete": "Disabled", // So you don't get autocomplete from both extensions.
      "clangd.path": "/usr/bin/clangd",
      // Clangd 运行参数(在终端/命令行输入 clangd --help-list-hidden 可查看更多)
      "clangd.arguments": [
          // 让 Clangd 生成更详细的日志
          "--log=verbose",
          // 输出的 JSON 文件更美观
          "--pretty",
          // 全局补全(输入时弹出的建议将会提供 CMakeLists.txt 里配置的所有文件中可能的符号，会自动补充头文件)
          "--all-scopes-completion",
          // 建议风格：打包(重载函数只会给出一个建议）
          // 相反可以设置为detailed
          "--completion-style=bundled",
          // 跨文件重命名变量
          "--cross-file-rename",
          // 允许补充头文件
          "--header-insertion=iwyu",
          // 输入建议中，已包含头文件的项与还未包含头文件的项会以圆点加以区分
          "--header-insertion-decorators",
          // 在后台自动分析文件(基于 complie_commands，我们用CMake生成)
          "--background-index",
          // 启用 Clang-Tidy 以提供「静态检查」
          "--clang-tidy",
          // Clang-Tidy 静态检查的参数，指出按照哪些规则进行静态检查，详情见「与按照官方文档配置好的 VSCode 相比  拥有的优势」
          // 参数后部分的*表示通配符
          // 在参数前加入-，如-modernize-use-trailing-return-type，将会禁用某一规则
          "--clang-tidy-checks=cppcoreguidelines-*,performance-*,bugprone-*,portability-*,  modernize-*,google-*",
          // 默认格式化风格: 谷歌开源项目代码指南
          "--fallback-style=file",
          // 同时开启的任务数量
          "-j=2",
          // pch优化的位置(memory 或 disk，选择memory会增加内存开销，但会提升性能) 推荐在板子上使用disk
          "--pch-storage=memory",
          // 启用这项时，补全函数时，将会给参数提供占位符，键入后按 Tab 可以切换到下一占位符，乃至函数末
          // 我选择禁用
          "--function-arg-placeholders=false",
          // compelie_commands.json 文件的目录位置(相对于工作区，由于 CMake 生成的该文件默认在 build 文件夹 中，故设置为 build)
          "--compile-commands-dir=build"
      ]
    }
    ```
    > `cmake.generator`: "Unix Makefiles": 这是指定 CMake 使用的生成器 generator 类型，这里设置的是"Unix Makefiles" 意味着 CMake 将会生成 Makefile 文件，这些文件可以被 Unix 系统上的 Make 命令用来编译和链接项目。        
    > `clangd.detectExtensionConflicts`: 这个选项让 clangd（一个用于 C++ 的语言服务器）检测可能的扩展冲突。      
    > `clangd.fallbackFlags`: []: 当 clangd 不能从项目配置中获取编译标志时，将使用这些后备标志。   
    > `editor.formatOnPaste`、`editor.formatOnSave`、`editor.formatOnType`: 这些设置定义了编辑器在粘贴、保存和输入时是否自动格式化代码。     
    > `C_Cpp.errorSquiggles`、`C_Cpp.intelliSenseEngineFallback`、`C_Cpp.intelliSenseEngine`、`C_Cpp.autocomplete`: 这些选项禁用了一些 VSCode 的 C/C++ 插件的功能，如错误标记、智能感知( IntelliSense)、自动补全等。     
    > `clangd.path`: "/usr/bin/clangd": 这是 clangd 的路径。       
    > `clangd.arguments`: 这个设置了 clangd 启动时的一些参数，包括日志级别、代码补全风格、跨文件重命名支持、自动包含头文件的方式、后台索引等。它还配置了 clang-tidy（一个C++的静态分析工具）来执行一系列的检查，如性能、 bug-prone、可移植性、现代化和 Google 的 C++ 风格指南等。最后，它还指定了预编译头 (pch) 的存储方式以及编译命令目录等。             
  
  > `tasks.json`(手动增加编译器构建设置 compiler build settings)  
    ```json
    {
        "tasks": [
            {
                "type": "shell",
                "label": "C/C++: clangd build active file",
                "command": "/usr/bin/clang++",
                "args": [
                    "-std=c++17",
                    "-Wall",
                    "-g",
                    "${file}",
                    "-o",
                    "${workspaceFolder}/build/${fileBasenameNoExtension}"
                ],
                "group": {
                    "kind": "build",
                    "isDefault": true
                },
                "detail": "Task generated by Debugger."
            }
        ],
        "version": "2.0.0"
    }
    ```
    > `type`: "shell"，表示这是一个 shell 命令任务。     
    > `label`: "C/C++: clangd build active file"，这是任务的标签或名称，用于在 VSCode 中识别任务。     
    > `command`: "/usr/bin/clang++"，这是要执行的 shell 命令。在这种情况下，它是 clang++ 编译器的路径，用于编译C++代码。       
    > `args`: 这是传递给命令的参数列表。具体来说，这些参数是       
    > `-std=c++17`: 指定 C++ 的标准为 C++17 标准。       
    > `-Wall`: 启用所有的警告。        
    > `-g`: 生成用于调试的信息。       
    > `${file}`: 这是一个变量，代表当前活动文件的路径。       
    > `-o`: 指定输出文件的名称。       
    > `${workspaceFolder}/build/${fileBasenameNoExtension}`: 这是输出文件的路径和名称，其中`${workspaceFolder}`代表工作空间的根目录，`${fileBasenameNoExtension}`代表当前文件的名称（没有扩展名）。     
    > `group`: 这个字段定义了任务如何组织。在这里，这个任务被标记为默认的构建任务。     
    > `kind`: "build"，表示这是一个构建任务。     
    > `isDefault`: true，表示这是默认的构建任务。     
    > `detail`: "Task generated by Debugger."，这是一条描述任务的详细信息。     
    > `version`: "2.0.0"，这是`tasks.json`文件的版本，2.0.0是当前的版本。         
  
  > `launch.json`(手动增加调试器设置 codeLLDB debugger settings)    
    ```json
    {
        "version": "0.2.0",
        "configurations": [
            {
                "name": "(lldb) Launch",
                "type": "lldb",
                "request": "launch",
                "program": "${workspaceFolder}/build/${fileBasenameNoEXtension}",
                "args": [],
                "cwd": "${workspaceFolder}",
                "preLaunchTask": "C/C++: clangd build active file"
            }
        ]
    }    
    ```
    > `version`: "0.2.0": 这是`launch.json`文件的版本号。     
    > `configurations`: 这个字段包含了一个数组，数组中的每一个对象都代表了一个特定的调试设置。对于数组中的第一个对象:    
    > `name`: "(lldb) Launch": 这是这个调试配置的名称。  
    > `type`: "lldb": 这个字段指定了被用于这个调试会话的调试器类型。   
    > `request`: "launch": 这个字段指定启动模式。"launch" 表示启动一个新的程序进行调试。  
    > `program`: "${workspaceFolder}/build/${fileBasenameNoEXtension}": 这个字段指定了运行要启动的程序路径。   
    > `args`: []: 这个字段用于指定传递给程序的命令行参数。这里没有提供任何参数，所以是一个空数组。   
    > `cwd`: "${workspaceFolder}": 这个字段指定了程序运行时的工作目录。${workspaceFolder} 是 VSCode 内置的变量，表示当前打开的文件夹的路径。     
    > `preLaunchTask`: 这个指定了运行该`(lldb) Launch`任务时，指定`task.json`里的前置 task 即为 C/C++: clangd build active file 名称。          

  > `compile_commands.json`(手动增加配置 clangd 服务器依赖的数据库文件)   
    ```json
    [
        {
            "directory": "/path/to/your/project",
            "command": "c++ -std=c++11 -c -o file.o file.cpp",  
            "file": "/path/to/your/project/file.cpp"
        }
    ]
    ```
    > `command`: 这个为编译命令和参数。   
    > `file`: 这个是指向的源文件路径。   
    > `directory`: 这个是项目的路径。    

- 配置 CMake 项目设置，基于 CMake 就无需手动配置编写 clangd 的`compelie_commands.json` 和 编译构建 `tasks.json`文件。   
  > `settings.json`(配置 clangd 服务设置)和上面配置基础 C/C++ 项目一致即可。   
  > `compelie_commands.json`文件需要 CMake 生成 `set(CMAKE_EXPORT_COMPILE_COMMANDS, 1)` 因为 clangd 依赖编译数据库文件`compelie_commands.json`进行静态检查等。   
  > `.vscode/launch.json`文件需要创建设置调试器(调试器设置 codeLLDB debugger settings)   
    ```json
    {
      "version": "0.2.0",
      "configurations": [
          {
              "name": "(lldb) Launch",
              "type": "lldb",
              "request": "launch",
              "program": "${command:cmake.launchTargetPath}",
              "args": [],
              "cwd": "${workspaceFolder}"
          }
      ]
    }
    ```
    > `version`: "0.2.0": 这是`launch.json`文件的版本号。     
    > `configurations`: 这个字段包含了一个数组，数组中的每一个对象都代表了一个特定的调试设置。对于数组中的第一个对象        
    > `name`: "(lldb) Launch": 这是这个调试配置的名称。      
    > `type`: "lldb": 这个字段指定了被用于这个调试会话的调试器类型。     
    > `request`: "launch": 这个字段指定启动模式。"launch" 表示启动一个新的程序进行调试。     
    > `program`: "${command:cmake.launchTargetPath}": 这个字段指定了要启动的程序。此处使用了 VSCode 的内置变量: `${command:cmake.launchTargetPath}`，这个命令会被 CMake Tools 解析，并且返回要调试的目标程序的路径。   
    > `args`: []: 这个字段用于指定传递给程序的命令行参数。这里没有提供任何参数，所以是一个空数组。     
    > `cwd`: "${workspaceFolder}": 这个字段指定了程序运行时的工作目录。${workspaceFolder} 是 VSCode 内置的变量，表示当前打开的文件夹的路径。         
  
### 3、配置代码格式化 
- 配置`clang-format`格式化命令 clang-format -style=格式名 -dump-config > .clang-format 其中格式名的取值可以为`LLVM`, `Google`, `Chromium`, `Mozilla`, `Webkit` 中的任一种，这会将对应格式预设生成写入`.clang-format`文件。 
- clangd 根据`.clang-format`配置文件进行格式化代码，若改配置文件未找到，则使用`fallback-style`参数指定的风格进行格式化代码。若要自定义格式化，则需要添加`.clang-format`文件进行配置   
  ```yaml
  # We'll use defaults from the LLVM style, but with 4 columns indentation.
  BasedOnStyle: LLVM
  IndentWidth: 4

  Language: Cpp
  # Force pointers to the type for C++.
  DerivePointerAlignment: false
  PointerAlignment: Left
  ```
  > `BasedOnStyle: LLVM`: 这个配置是基于`LLVM`编码风格，`LLVM`是一种流行的编程风格，被广大程序员所接受。     
  > `IndentWidth: 4`: 这表示代码的缩进宽度为4个空格字符。这意味着每一级缩进都会有4个空格。      
  > `Language: Cpp`: 表明这个配置是用于 C++ 语言的。     
  > `DerivePointerAlignment: false`: 这表示不会根据上下文自动推断指针对齐的方式，而是会遵循`PointerAlignment`的设置。     
  > `PointerAlignment: Left`: 这表示指针的星号(*)会跟随类型放在左边。例如，对于一个 int 类型的指针，它应该被写作`int* ptr;` 而不是 `int *ptr;` 或 `int * ptr;`。    
  
### 4、配置静态分析
- clang-tidy 是一个基于 clang 的 C++ "linter" 工具。其作用主要是用来检查和修正代码中的典型编程问题，像代码风格/接口误用/通过静态分析可定位到的 bug, clang-tidy 模块化功能做的很好，有预留接口，非常便于实现新的代码检查功能。
- clang-tidy 提供了一个配置文件，可以指定要使用的检测器、检测器的参数等。配置文件使用 YAML 语法编写，一般放在当前工程目录下，文件名为`.clang-tidy`。每一项检测都有其自己的名字，使用 -checks 参数可以指定要进行哪些检测，它指定了逗号分隔的正向和反向规则，正向规则指定了要添加的检测子集，反向规则指定了要移除的规则 -checks 指定要进行哪些检测 -* 表示移除所有检测 modernize-* 表示保留 modernize 开头的所有检测。
  ```yaml
  # .clang-tidy 文件配置
  Checks: 'cppcoreguidelines-*,performance-*,bugprone-*,portability-*,modernize-*,google-*'
  ```
  > `cppcoreguidelines-*`: 启用所有的 C++ Core Guidelines 检查。C++ Core Guidelines 是一组试图帮助程序员在现代 C++ 中写出更高质量、更健壮的代码的规则。  
  > `performance-*`: 启用所有的性能检查，这些检查主要用于发现可能影响程序运行性能的代码。     
  > `bugprone-*`: 启用所有的可能导致 bug 的检查，这些检查主要用于发现可能导致程序行为错误或不可预测的代码。   
  > `portability-*`: 启用所有的可移植性检查，这些检查主要用于发现可能在不同平台或编译器上表现不一致的代码。   
  > `modernize-*`: 启用所有的现代化检查，这些检查主要用于发现可以使用新的 C++ 特性改写的代码。   
  > `google-*`: 启用所有的 Google 风格检查，这些检查主要用于确保代码符合 Google C++ 风格指南。这些规则集可以帮助程序员检。   
  