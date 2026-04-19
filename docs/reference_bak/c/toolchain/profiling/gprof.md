## gprof 性能分析

- gprof能够让你知道你的代码哪些地方是比较耗时的，哪些函数是被调用次数很多的，并且能够让你一目了然的看到函数与函数之间的调用关系。gprof是gcc/g++编译器支持的一种性能诊断工具。只要在编译时加上-pg选项，编译器就会在编译程序时在每个函数的开头加一个mcount函数调用，在每一个函数调用之前都会先调用这个mcount函数，在mcount中会保存函数的调用关系图和函数的调用时间和被调次数等信息。最终在程序退出时保存在gmon.out文件中，需要注意的是程序必须是正常退出或者通过exit调用退出，因为只要在exit（）被调用时才会触发程序写gmon.out文件。    

- gprof的使用方法主要以下三步：用-pg参数编译程序、运行程序，并正常退出、查看gmon.out文件。  

gprof使用实例
```c++
#include <iostream> 
using namespace std; 

int add(int a, int b) 
{ 
    return a+b; 
} 
  
int sub(int a, int b) 
{ 
    return a-b; 
} 
 
int call () 
{ 
    std::cout << add(1,2) << std::endl; 
    std::cout << sub(2,4) << std::endl; 
} 

int main() 
{ 
    int a=1, b=2; 
    cout << add(a,b) << endl; 
    for (int i=0; i<10000; i++) 
            call(); 
    return 0; 
}
```

使用g++编译并加上-pg参数  
```bash
g++ -o hello hello_grof.cpp -pg -g
```

得到可执行文件 hello，我们可以使用 readelf 查看一下它的符号表里和没有 -pg 时编译的有啥不同，可以看出多了三个函数符号_mcount, __monstartup, _mcleanup 都是和 gprof 相关的调用，readelf 是一个用于显示 ELF 文件信息的工具。它可以提供关于目标文件的各种详细信息，包括文件头、节表、符号表等   
```bash
readelf -r ./hello和readelf -r ./hello_normal
```

使用 gdb 调试 hello 程序，在 mcount 函数中打断点也可以看到其调用关系，在 add 函数执行前先调用   

接下来运行程序./hello，会在当前目录下生成gmon.out文件。使用gprof查看文件信息，可将gprof -b ./hello中的-b参数去掉，可以显示字段的详细含义描述  
```bash
gprof -b ./hello gmon.out
``` 

使用运行命令生成dot格式的调用关系图文件，可以使用windows版的GVEdit for Graphviz软件查看调用关系图  
```bash
gprof -b ./hello  gmon.out | gprof2doc.py > ~WWW/hello.dot 
```

gprof是常见的性能分析工具，在此罗列一下它的一些不足：对多线程支持不好，不准确，必须退出exit()才行，它只能分析应用程序在运行过程中所消耗掉的用户时间，无法得到程序内核空间的运行时间。对内核态的调用分析无能为力。如果程序系统调用比率比较大，就不适合

