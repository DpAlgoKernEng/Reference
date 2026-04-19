
https://www.cnblogs.com/kunsoft/p/13776482.html

https://blog.csdn.net/qq_48383456/article/details/135816242


## CMakeLists.txt 简单配置用法

CMake 是一个开源的、跨平台的自动化构建系统，用于管理软件构建过程。通过编写配置文件（CMakeLists.txt），自动生成适合不同编译器和操作系统的构建脚本或项目文件，CMake 拥有强大的跨平台能力，是众多构建项目的首选工具。    
CMake可以构建的项目简单分为:   
- 单文件项目    
- 多文件项目    
- 多文件夹项目    
- 集成第三方库的大型工程    

### 单文件的 CMakeLists.txt 配置  
- 单文件项目即只有`main.cpp`一个文件，文件夹的布局如下:      
    ```bash
    .
    ├── CMakeLists.txt
    └── main.cpp
    ```
- 简单的`CMakeLists.txt`配置如下，创建名为`SingleFileExample`的可执行程序，源代码为`main.cpp`。       
    ```bash
    cmake_minimum_required(VERSION 3.29.0) # 设置cmake的最低版本要求为3.29.0
    project(SingleFileExample)         # 设置项目名称

    add_executable(SingleFileExample main.cpp) # 添加可执行目标
    ```    
 
### 多文件的 CMakeLists.txt 配置    
- 多文件项目即文件有多个，在`main.cpp`，`func.h`，`func.cpp` 文件夹布局如下:       
    ```bash
    .
    ├── CMakeLists.txt
    ├── func.h
    ├── func.cpp
    └── main.cpp
    ```
- 其`CMakeLists.txt`配置如下，创建名为`MultiFileExample`的可执行程序，源代码包含`main.cpp`，`func.cpp`。     
    ```bash 
    cmake_minimum_required(VERSION 3.29.0)  # 设置cmake的最低版本要求为3.29.0
    project(MultiFileExample)  # 设置项目名称

    aux_source_directory(. SOURCES)  # 在当前目录中查找源文件，并将它们添加到SOURCES变量中

    add_executable(MultiFileExample ${SOURCES}) # 添加可执行文件，并将源文件添加到可执行文件中
    ```

### 多文件夹的 CMakeLists.txt 配置  
- 多文件夹的项目，比如更复杂的项目结构`source`文件夹存放库的代码，`test`文件夹生成测试库的代码每个项目下都有自己的`CmakeLists.txt`构建文件，文件夹布局如下:        
    ```bash
    .
    ├── CMakeLists.txt
    ├── source
    │   ├── CMakeLists.txt
    │   ├── include
    │   │   └── func.h
    │   └── src
    │       └── func.cpp
    └── test
        ├── CMakeLists.txt
        └── main.cpp
    ```    
- 其`CMakeLists.txt`配置如下，示例模拟了库文件和测试程序的构建，其中`source`文件夹存放库的代码，`test`文件夹生成测试库的代码，`CMakeLists.txt`配置意图是生成一个名为`funcTest`的可执行文件，它链接了在`source`目录下生成的`funclib`静态库。        
    ```bash
    # source/CMakeLists.txt
    cmake_minimum_required(VERSION 3.29.0)  # 设置cmake的最低版本要求为3.10
    project(funclib)  # 设置项目名称

    file(GLOB_RECURSE SRC "src/*.cpp")  # 递归查找source子目录下的所有.cpp文件，并将它们添加到SRC变量中

    add_library(${PROJECT_NAME} STATIC ${SRC})  # 添加静态库目标，PROJECT_NAME代表当前项目的名称

    # test/CMakeLists.txt
    cmake_minimum_required(VERSION 3.29.0)  # 设置cmake的最低版本要求为3.10
    project(funcTest)  # 设置项目名称

    add_executable(${PROJECT_NAME} main.cpp) # 生产可执行文件，并将源文件添加到可执行文件中

    target_link_libraries(${PROJECT_NAME} funclib)  # 添加静态库目标

    # ./CMakeLists.txt
    cmake_minimum_required(VERSION 3.29.0)  # 设置cmake的最低版本要求为3.10
    project(MultiFolderExample)  # 设置项目名称
    
    include_directories(${CMAKE_SOURCE_DIR}/source/include) # CMAKE_SOURCE_DIR变量代表当前CMakeLists.txt所在的路径

    add_subdirectory(source)  # 添加source子目录
    add_subdirectory(test)  # 添加test子目录
    ```

### 引用第三方库（静态库）的 CMakeLists.txt 配置   
- 在实际开发中，往往需要依赖外部库。以静态库为例，考虑引入其它项目提供的库`func.lib`，其配套的头文件为`func.h`，同时生成一个动态库名称为`BirdEngine`。项目的文件夹布局如下:     
    ```bash
    .
    ├── CMakeLists.txt
    ├── third_party
    │   ├── include
    │   │   └── func.h
    │   └── lib
    │       └── funclib.lib
    └── src
        └── BirdEngine.cpp
        └── BirdEngine.h
    ```
- 其中`CMakeLists.txt`配置如下，首先通过`include_directories`添加第三方库的头文件路径，然后通过`link_directories`添加第三方库的库文件路径，最后通过`target_link_libraries`链接第三方库。      
    ```bash
    cmake_minimum_required(VERSION 3.29.0)  # 设置cmake的最低版本要求为3.29.0
    project(BirdEngine)  # 设置项目名称

    include_directories(${CMAKE_SOURCE_DIR}/third_party/include) # 添加第三方库的头文件路径

    link_directories(${CMAKE_SOURCE_DIR}/third_party/lib) # 添加第三方库的库文件路径

    file(GLOB_RECURSE SOURCES "src/*.cpp") # 递归查找source子目录下的所有.cpp文件，并将它们添加到SOURCES变量中

    add_library(${PROJECT_NAME} SHARED ${SOURCES})  # 生产动态库目标，并将源文件添加到库文件中

    target_link_libraries(${PROJECT_NAME} funclib) # 链接第三方库
    ```

### 引用第三方库（动态库+静态库）的 CMakeLists.txt 配置 
- 针对于同时依赖静态库和动态库的场景，需要在链接阶段找到`lib`库，同时需要在运行阶段找到`dll`库。假设某项目需要库`BirdEngine`，其静态库为`BirdEngine.lib`，动态库为`BirdEngine.dll`，其文件夹布局如下:   
    ```bash
    .
    ├── CMakeLists.txt
    ├── third_party
    │   ├── include
    │   │   └── BirdEngine.h
    │   ├── lib
    │   │   ├── BirdEngine.lib
    │   └── bin
    │       └── BirdEngine.dll
    └── src
        └── main.cpp
    ```    
- 其`CMakeLists.txt`配置如下，首先通过`include_directories`添加第三方库的头文件路径，然后通过`link_directories`添加第三方库的库文件路径，最后通过`target_link_libraries`链接第三方库。同时，通过`file`命令复制动态库到输出目录。     
    ```bash
    cmake_minimum_required(VERSION 3.29.0) # 设置cmake的最低版本要求为3.29.0
    project(BirdEngineTest) # 设置项目名称

    include_directories(${CMAKE_SOURCE_DIR}/third_party/include) # 添加第三方库的头文件路径

    link_directories(${CMAKE_SOURCE_DIR}/third_party/lib) # 添加第三方库的库文件路径

    file(GLOB_RECURSE SOURCES "src/*.cpp") # 递归查找source子目录下的所有.cpp文件，并将它们添加到SOURCES变量中

    add_executable(${PROJECT_NAME} ${SOURCES})

    target_link_libraries(${PROJECT_NAME} BirdEngine)

    file(COPY ${CMAKE_SOURCE_DIR}/third_party/bin/BirdEngine.dll 
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # 复制动态库到输出构建目录，（通常是执行cmake命令的目录）。这样做的目的是在运行生成的BirdEngineTest程序时，程序的运行目录中就会包含需要的动态链接库。
    ```