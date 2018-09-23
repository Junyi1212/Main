#ifndef PIPE_H
#define PIPE_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

void PipeCase1(void);
void PipeCase2(void);  
void PipeCase3(void);
void PipeCase4(void);
void PipeCase5(void);
void PipeCase6(void);
void PipeCase7(void);
#ifdef __cplusplus
}
#endif

#endif