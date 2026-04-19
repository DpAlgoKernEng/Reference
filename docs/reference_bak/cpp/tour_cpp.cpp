#include <iostream>
#include <string>

#include "menu_manager/menu_cmdline_cpp/MenuBuilder.h"
#include "menu_manager/menu_cmdline_cpp/MenuManager.h"
#include "menu_manager/menu_cmdline_cpp/MenuLayout.h"

// 示例功能函数
/**
 * 示例功能1
 * 演示功能项的实现
 */
void sampleFunction1() {
    std::cout << "这是功能1的执行结果\n";
    // 实际功能代码...
}

/**
 * 示例功能2
 * 演示功能项的实现
 */
void sampleFunction2() {
    std::cout << "功能2已成功执行\n";
    // 实际功能代码...
}

/**
 * 示例功能3
 * 演示功能项的实现
 */
void sampleFunction3() {
    std::cout << "正在处理功能3...\n";
    // 实际功能代码...
}

/**
 * 程序入口点
 * 构建菜单结构并运行菜单系统
 */
int main() {
    // 创建菜单管理器实例
    MenuManager manager;

    // 设置网格布局显示
    MenuDisplayConfig config;
    config.layout = MenuLayout::GRID;
    config.columns = 3;
    manager.setDisplayConfig(config);

    // 使用构建器模式构建菜单结构
    auto builder = manager.getBuilder();

    // 使用链式调用构建复杂的菜单结构
    builder.addFunction("语言标准", sampleFunction1)
        .addFunction("编译器支持", sampleFunction2)
        .addSubMenu("语言概念")
        .addFunction("网络设置", []() {
            std::cout << "网络配置选项...\n";
        })
        .addSubMenu("安全设置")
        .addFunction("密码管理", []() {
            std::cout << "修改登录密码...\n";
        })
        .addFunction("权限设置", []() {
            std::cout << "用户权限管理...\n";
        })
        .endSubMenu()
        .endSubMenu()
        .toMainMenu()
        .addFunction("标准库头文件", sampleFunction3)
        .addSubMenu("命名要求")
        .addFunction("数据备份", []() {
            std::cout << "正在备份数据...\n";
        })
        .addFunction("系统诊断", []() {
            std::cout << "执行系统诊断...\n";
        })
        .addFunction("功能测试", sampleFunction3)
        .addFunction("技术规格", sampleFunction3)
        .addFunction("有用资源", sampleFunction3)
        .endSubMenu();

    // 运行菜单系统
    manager.run();

    // 程序正常退出
    return 0;
}