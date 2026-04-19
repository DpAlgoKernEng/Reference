#include <stdio.h>
#include "basic_concepts.h"

int global_flag = 0; // 定义

void print_basic_concepts() {
    printf("Hello, Basic Concepts!\n");
}

void print_global_flag() {
    printf("Global flag value: %d\n", global_flag);
}

int main() {
    print_basic_concepts();
    print_global_flag();
    return 0;
}



