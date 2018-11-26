#ifndef SHARELIB_H
#define SHARELIB_H
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int (*CalculatorFuncPointer)(int, int);

int ShareLib(void);

#ifdef __cplusplus
}
#endif

#endif