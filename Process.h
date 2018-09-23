#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#ifdef __cplusplus
extern "C" {
#endif

void ProcessCase1(void);
void ProcessCase2(void);
void ProcessCase3(void);

#ifdef __cplusplus
}
#endif

#endif
