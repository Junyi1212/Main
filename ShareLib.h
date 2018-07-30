#ifndef SHARELIB_H
#define SHARELIB_H
#include <stdio.h>
#include <dlfcn.h>

typedef int (*CalculatorFuncPointer)(int, int);

int ShareLib(void);
#endif