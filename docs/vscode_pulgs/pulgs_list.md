## VSCode 配置

### 1、VSCode 配置文件
- `.vscode` 当在 VSCode 中打开或创建一个项目时会生成一个 `.vscode` 文件夹，该文件夹是 VSCode 的一个隐藏文件夹通常保存在项目的根目录下，主要是存储特定项目的 VSCode 设置包括任务定义、编译、调试配置、推荐的扩展等。
- `.vscode/settings.json` VSCode 会将设置值存储在`settings.json`文件中，设置编辑器是一个用户界面可查看和修改存储在文件中的设置值。也可以直接使用命令面板 (CTRL(CMD)+SHIFT+P) 中选择 Preferences: Open User Settings (JSON) 命令打开此文件`settings.json`来查看和编辑配置。VS Code 有两种设置范围：用户设置--全局应用于打开的任何 VSCode 实例的设置、工作区设置--存储在工作区内的设置仅在打开该工作区时应用。   
- `.vscode/tasks.json` 用于配置和自定义 VSCode 构建任务设置，这个文件会第一次尝试在 VSCode 中运行构建任务时自动创建。也可以直接使用命令面板 (CTRL(CMD)+SHIFT+P) 中选择 Tasks: Configure Task 打开`tasks.json`进行配置。
- `.vscode/launch.json` 用于配置和自定义 VSCode 的调试设置，要创建`launch.json`文件，在运行启动和调试视图中选择create a launch.json file 文件即可。

### 2、VSCode 中 C/C++ 相关配置方式
- VSCode 配置 C/C++ 开发环境有两种方式
    > 基于 C/C++ Extension Pack 插件的配置`GCC + CMake + Make/Ninja + GDB + VSCode + C/C++ Extension Pack`插件。  
    > 基于 clangd 插件的配置`Clang + CMake + Make/Ninja + LLDB + VSCode + clangd + CodeLLDB`插件。
- 微软插件 C/C++ Extension Pack 但是在源文件很多时比较卡顿加载很慢、更新频率低，不推荐。
- LLVM 插件 clangd 整体体验不错，而且有clang全家桶带来的特色功能：自动插入头文件、clang-format、clang-tidy 等功能。clangd 这些插件的原理都是通过分析构建系统生成的`complie_command.json`文件，这个文件里定义了项目所有 C/C++ 文件的编译参数。

### 3、VSCode 插件列表
- 微软插件 C/C++ 插件
  > C/C++ Extension Pack                
  > C/C++                               
  > C/C++ Themes                        
  > C/C++ GNU Global
- LLVM 插件                    
  > clangd                              
  > CodeLLDB 
- CMake 插件                         
  > CMake                               
  > CMake Tools  
- Make 插件                        
  > Makefile Tools                                                  
- Doxygen 注释文档
  > Doxygen Documentation Generator 
- C/C++ Snippets 代码生成
  > C/C++ Snippets  
- Todo 清单
  > Todo Tree       
- Markdown 插件
  > Markdown All in One

### 4、VSCode 自定义 Code Snippets
- Snippets即代码段，代码段是模板可让更轻松地输入重复的代码模式，例如循环或条件语句。
- 使用命令面板 (CTRL(CMD)+SHIFT+P) 中选择 Preferences: Configure User Snippets、选择代码片段作用于哪种语言，例如作用于 C 选择`c.json`编辑即可配置
  ```bash
  {
  	# Place your snippets for c here. Each snippet is defined under a snippet name and has a   prefix, body and 
  	# description. The prefix is what is used to trigger the snippet and the body will be expanded   and inserted. Possible variables are:
  	# $1, $2 for tab stops, $0 for the final cursor position, and ${1:label}, ${2:another} for   placeholders. Placeholders with the 
  	# same ids are connected.
  	# Example:
  	# "Print to console": {
  	# 	"prefix": "log",
  	# 	"body": [
  	# 		"console.log('$1');",
  	# 		"$2"
  	# 	],
  	# 	"description": "Log output to console"
  	# }
  	"Main": {
  		"prefix": "main",
  		"body": [
  			"int main(void) {",
  			"  $1",
  			"  return 0;",
  			"}"
  		],
  		"description": "Main Function"
  	}
  }  
  ```
                    