## VSCode 使用 Doxygen 进行注释文档管理配置
Doxygen 是一个开源的文档生成工具，能够根据代码注释生成 HTML 或 LaTeX 格式的文档，Doxygen 在不用额外操作的情况下创建代码结构概览。尽管 Doxygen 主要是用来给 C++ 生成文档的，但对其它语言同样适用，比如 C、Objective-C、 C#、 PHP、Java 和 Python 等。要使用 Doxygen 只需要在源代码中使用 Doxygen 能够识别的语法来写注释，Doxygen 会扫描源码文件，然后根据这些特殊注释生成 HTML 或 LaTeX 文档。  
Graphviz 是 Doxygen 用于自动生成类图的工具。graphviz 是一个由 AT&T 实验室启动的开源工具包，用于绘制 DOT 语言脚本描述的图形。Doxygen 使用 graphviz 自动生成类之间和文件之间的调用关系图，如不需要此功能可不安装该工具包。

### 安装 Doxygen
- 安装 Doxygen 和 Graphviz
  ```bash
  sudo apt install graphviz
  sudo apt install doxygen
  
  # 在当前路径生成一个名为 Doxyfile 配置文件进行配置。
  doxygen -g
  
  # 根据 Doxyfile 配置文件生成文档。
  doxygen <config-file>
  ```

### 配置 Doxygen 产生文档  
- Doxygen 产生文档可以分为三个步骤:    
  > 在程序代码中加上符合 Doxygen 所定义批注格式。  
  > 进行 Doxygen 配置。   
  > 使用 Doxygen 来产生批注文档。       

### VSCode 使用插件 Doxygen Documentation Generator 配置 Doxygen。 
- 安装 Doxygen Documentation Generator 插件。   
- 配置`settings.json`即可。  
    ```bash
    // doxdocgen基础设置
    "doxdocgen.c.triggerSequence": "/**",
    "doxdocgen.c.firstLine": "/**",
    "doxdocgen.c.commentPrefix": "*",
    "doxdocgen.c.lastLine": "*/",
    "doxdocgen.generic.linesToGet": 100, //如果有很多超级大函数要把这个值设置很大
    //author
    "doxdocgen.generic.authorEmail": "miguelau3016@outlook.com",
    "doxdocgen.generic.authorName": "miguel",
    "doxdocgen.generic.authorTag": /*"@author {author} ({email})"*/ " @author{indent:20}{author} {email}",
    //date
    "doxdocgen.generic.dateFormat": "YYYY.MM.DD",
    "doxdocgen.generic.dateTemplate": " @date{indent:20}{date}",
    //brief 居然和函数注释用的同一个
    //"doxdocgen.generic.briefTemplate": "@brief{indent:10}{text}【描述】",
    //details
    // 文件头
    "doxdocgen.file.fileOrder": [
        "file", // 文件名
        "brief", // 文件概述
        "details", // 文件详述
        "version", // 版本
        "author", // 作者
        "date", // 日期
        "copyright", // 版权
        "empty",
        "custom", // 自定义
    ],
    //file
    "doxdocgen.file.fileTemplate": " @file{indent:20}{name}",
    //version
    "doxdocgen.file.versionTag": " @version{indent:20}0.1",
    //copyright
    "doxdocgen.file.copyrightTag": [
        " @copyright{indent:20}Copyright (c) {year}."
    ],
    //custom
    "doxdocgen.file.customTag": [
        " @note{indent:20}Revision History:",
        " <table>",
        " <tr><th>Date{indent:20}<th>Version{indent:40}<th>Author{indent:60}<th>Description",
        " <tr><td>{date}{indent:20}<td>0.1{indent:40}<td>{author}{indent:60}<td>description",
        " </table>",
    ],
    //函数注释
    "doxdocgen.generic.order": [
        "brief",
        "param",
        "return",
        "empty",
        "custom"
    ],
    //brief
    "doxdocgen.generic.briefTemplate": " @brief{indent:20}brief",
    "doxdocgen.generic.splitCasingSmartText": true, //（没用）
    "doxdocgen.c.setterText": "Set {name}", //Get方法（没用）
    "doxdocgen.c.getterText": "Get {name}", //Set方法（没用）
    "doxdocgen.c.factoryMethodText": "Create {name}", //Create方法（没用）
    //param
    "doxdocgen.generic.paramTemplate": " @param{indent:10}{param}{indent:20}Param Description",
    "doxdocgen.generic.filteredKeywords": [
        "me"
    ],
    //return
    "doxdocgen.generic.returnTemplate": " @return{indent:10}{type}{indent:20}Return Description",
    "doxdocgen.generic.includeTypeAtReturn": true,
    "doxdocgen.generic.boolReturnsTrueFalse": false,
    //custom
    "doxdocgen.generic.customTags": [
        // " @warning{indent:20}[不可重入,阻塞等警告]",
        " @note{indent:20}Revision History"
    ], //用户自定义
    //命令建议（没用）
    "doxdocgen.generic.commandSuggestion": true,
    "doxdocgen.generic.commandSuggestionAddPrefix": true,
    ```
