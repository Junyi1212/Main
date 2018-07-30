#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <execinfo.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ucontext.h>
#include <pthread.h>
#include <unwind.h>

void ExceptionInit(void);
int ExceptionStart(void);
#endif