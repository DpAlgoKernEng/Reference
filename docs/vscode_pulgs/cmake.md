## CMake 和 CMake Tools 插件配置
CMake 是一个开源的跨平台工具，它使用编译器和平台独立的配置文件来生成特定于您的编译器和平台的本机构建工具文件`This extension provides support for CMake in Visual Studio Code.`        
CMake Tools 集成了 VSCode 和 CMake 可以轻松配置、构建和调试 C/C++ 项目`CMake Tools provides the native developer a full-featured, convenient, and powerful workflow for CMake-based projects in Visual Studio Code.`

### 1、CMake 项目先决条件
- 安装配置 C/C++ 编译调试 Clang/GCC、Make/Ninja、GDB/LLDB 工具。
- 安装配置 CMake 构建工具。   
- 安装 VSCode 官网: https://code.visualstudio.com/docs/setup/mac    
- 安装 VSCode 的 C/C++ Extension Pack 或 Clangd 编译扩展插件。  
- 安装 VSCode 的 CMake Tools 和 CMake 插件。  
- 安装 Clang 编译器，并检查编译器的版本。   
  ```bash 
  # 使用 macos 上的 xcode 开发工具包安装 clang 编译器
  xcode-select --install
  
  # 运行下列命令检查 编译器 版本
  clang --version
  clang++ --version
  gcc -v
  g++ -v
  
  # 运行下列命令检查 编译工具 版本
  make -v
  ninja --version
  
  # 运行下列命令检查 调试工具 版本
  lldb -v
  gdb -v  
  ```
- 安装 CMake 构建工具。  
  ```bash
  # 运行下列命令查看 cmake 版本
  cmake --version
  ```

### 2、创建一个 CMake 项目
- 如果现有的 CMake 项目在根目录中已有文件 `CMakeLists.txt`则可以跳到 3、配置 CMake 项目 来配置现有项目，否则为新项目创建一个文件夹在终端窗口中创建一个名为 codeMastery 的空文件夹。    
  ```bash
  mkdir codeMastery
  cd codeMastery
  code .
  ```
- 使用 CMake Tools 创建基本 CMake 项目文件，打开命令面板 (CTRL(CMD)+SHIFT+P) 选择 CMake: Quick Start 命令创建新项目。     
- 输入项目名称为 codeMastery 这个名称将被写入 `CMakeLists.txt`初始文件。    
- 选择项目类型为 Executable 或 Library。选择 Executable 会创建一个 codeMastery 文件。 CMake 项目包含`main.cpp`源文件和  `CMakeLists.txt` (告诉 CMake 工具如何构建项目) 和一个 `build` 以构建文件命名的文件夹。  
  ```bash
  # CMakeLists.txt
  cmake_minimum_required(VERSION 3.29.0)
  project(ccplusplus VERSION 0.1.0 LANGUAGES C CXX)
  
  add_executable(ccplusplus foo.cpp main.cpp)
  
  include(CTest)
  enable_testing()
  
  set(CPACK_PROJECT_NAME ${PROJECT_NAME})
  set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
  include(CPack)
  ```
- 选择 Add a New Preset 和 Create from Compilers 添加新预设并从编译器中创建，程序会自动扫描您计算机上的工具包并查找系统上的编译器列表，选择需要使用的编译器，输入此新预设的名称。预设的名称将被写入`CMakePresets.json`文件。
  ```bash
  # CMakePresets.json
  {
    "version": 8,
    "cmakeMinimumRequired": {
        "major": 3,
        "minor": 19,
        "patch": 0
    },
    "configurePresets": [
        {
            "name": "Clang 15.0.0 arm64-apple-darwin23.2.0",
            "displayName": "Clang 15.0.0 arm64-apple-darwin23.2.0",
            "description": "Using compilers: C = /usr/bin/clang, CXX = /usr/bin/clang++",
            "binaryDir": "${sourceDir}/build",
            "generator": "Unix Makefiles",
            "cacheVariables": {
                "CMAKE_INSTALL_PREFIX": "${sourceDir}/install",
                "CMAKE_C_COMPILER": "/usr/bin/clang",
                "CMAKE_CXX_COMPILER": "/usr/bin/clang++",
                "CMAKE_EXPORT_COMPILE_COMMANDS": "YES",
                "CMAKE_BUILD_TYPE": "Debug"
            }
        }
    ],
    // gcc
    // "configurePresets": [
    //     {
    //         "name": "gcc 11.4.0 x86_64-linux-gnu",
    //         "displayName": "gcc 11.4.0 x86_64-linux-gnu",
    //         "description": "Using compilers: C = /usr/bin/gcc, CXX = /usr/bin/g++",
    //         "binaryDir": "${sourceDir}/build",
    //         "cacheVariables": {
    //             "CMAKE_INSTALL_PREFIX": "${sourceDir}/install",
    //             "CMAKE_C_COMPILER": "/usr/bin/gcc",
    //             "CMAKE_CXX_COMPILER": "/usr/bin/g++",
    //             "CMAKE_EXPORT_COMPILE_COMMANDS": "YES",
    //             "CMAKE_BUILD_TYPE": "Debug"
    //         }
    //     }
    // ],
    "buildPresets":[
  
    ],
    "testPresets": [
  
    ],
    "packagePresets": [
  
    ],
    "workflowPresets": [
        
    ]
  }
  ```
  > `version`: 是文件的版本号，此例中为 8。     
  > `cmakeMinimumRequired`: 指明所需的 CMake 的最低版本号。在此例中，它是 3.19。     
  > `configurePresets`: 这是一个关于项目配置方面的预设列表。每个预设包含以下属性     
  > `name`：预设的唯一名称。   
  > `displayName`: 预设的显示名称。   
  > `description`: 预设的描述。   
  > `generator`: 指定生成 Makefile 或 Ninja 编译构建任务。  
  > `binaryDir`: 二进制文件将被存放的目录。   
  > `cacheVariables`: 此项目中使用的缓存变量，包括安装前缀，C 和 C++ 编译器路径，以及构建类型等。   
  > `buildPresets`: 用于构建项目的预设列表。在此文件中，该列表为空。  
  > `testPresets`: 用于测试项目的预设列表。在此文件中，该列表为空。   
  > `packagePresets`: 用于打包项目的预设列表。在此文件中，该列表为空。   
  > `workflowPresets`: 用于定义构建流程的预设列表。在此文件中，该列表为空。     

- 完成这些步骤后，将生成一个完整的 CMake 项目，其中包含以下文件：`main.cpp`、`CMakeLists.txt`、`CMakePresets.json`和`build`构建文件夹。
   
### 3、配置 CMake 项目
- 在构建项目之前需要对其进行配置 CMake Tools 要使用的系统的编译器。有两种方法可以在 VSCode 中 CMake 配置。 
  
- 使用 CMake 预设（CMake Kits/Variants）
  > 使用 CMake 预设，建议使用 CMake 预设来管理 CMake 配置。预设可指定一个通用文件`CMakePresets.json`存储项目的所有配置，可以跨不同的 IDE 和操作系统和其他人共享此文件配置。
  >    
  > 如果项目没有`CMakePresets.json`文件，则可通过 CMake: Quick Start 来生成或者也可以直接新建一个`CMakePresets.json`文件，重新加载 CMake 项目生效。 
  >   
  > 如果项目有`CMakePresets.json`文件，则可使用配置和构建预设来指定如何进行构建项目。可以通过 CMake Tools 左侧视图配置 Configure 和 Build 也可以打开命令面板 (CTRL(CMD)+SHIFT+P) 选择 CMake: Select Configure Preset 和 CMake: Select Build Preset 进行配置。 
  >   
  > CMakePresets 支持配置 `workflow` 决定你的工程有多少个配置阶段。CMakePresets 支持 `configure、build、test、package` 几个阶段，如果不需要使用 CTest 和 CPack 那后面两个阶段也可以不做配置。    

- 使用 CMake 套件/变体（CMake Kits/Variants）
  > 使用 CMake 套件/变体，套件代表一个工具链它是编译器、链接器和用于构建项目的工具，变体代表有关如何构建项目的说明。         
  >
  > 选择一个套件 (Select a Kit) 可以在 CMake 工具侧栏中的项目状态视图中可以查看该套件或从命令面板 (CTRL(CMD)+SHIFT+P) 中运行 CMake：Select a Kit 命令。如果没有看到所需的编译器可以编辑 `cmake-tools-kits.json`项目中的文件。要编辑该文件需要打开命令面板 (CTRL(CMD)+SHIFT+P) 并运行 CMake: Edit User-Local CMake Kits 命令。      
  >
  > 选择一个变体（Select Variant）包含有关如何构建项目的说明。默认情况下 CMake 工具扩展提供四个变体，每个变体对应一个默认构建类型：Debug、Release、MinRelSize、RelWithDebInfo等。      
    > `Debug`：禁用优化并包含调试信息。     
    > `Release`：包括优化但没有调试信息。     
    > `MinRelSize`：优化尺寸但没有调试信息。     
    > `RelWithDebInfo`：优化速度并包含调试信息。  

- 现在已经通过预设或套件/变体选择了配置设置，可以打开命令面板 (CTRL(CMD)+SHIFT+P)并运行 CMake: Configure 命令来配置项目，这将使用选择的配置在项目的构建文件夹中生成`build`构建文件。           

### 4、构建 CMake 项目
- 配置项目后就可以构建了，打开命令面板 (CTRL(CMD)+SHIFT+P) 并运行 CMake: Build 命令或从状态栏中选择 Build 按钮。
- 通过从命令面板 (CTRL(CMD)+SHIFT+P) 中选择 CMake: Set Build Target 来选择要构建的目标。默认情况下，CMake Tools 构建所有目标。选定的目标可以在 "CMake Tools" 侧栏中的 "Project Status" 视图中 "Build" 节点下方的查看也可以从那里进行设置。

### 5、调试 CMake 项目
- 需要运行和调试 CMake 项目，打开`main.cpp`并设置一个断点，然后打开命令面板 (CTRL(CMD)+SHIFT+P) 并运行 CMake: Debug 调试器或从状态栏中选择 Debug 按钮进行调试。





