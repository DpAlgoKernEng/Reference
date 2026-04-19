#pragma once
#ifndef BASIC_CONCEPTS_H
#define BASIC_CONCEPTS_H // 只使用 #pragma once 或 include guard 中的一种即可

#ifdef __cplusplus       // 如果希望支持 C++，在 basic_concepts.h 中加： #ifdef __cplusplus extern "C" { #endif
extern "C" {
#endif

extern int global_flag; // 声明

void print_basic_concepts();

void print_global_flag();

#ifdef __cplusplus
}
#endif

#endif // BASIC_CONCEPTS_H