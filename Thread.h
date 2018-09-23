#ifndef THREAD_H
#define THREAD_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C" {
#endif

void ThreadCase1(void);
void ThreadCase2(void);
void ThreadCase3(void);
void ThreadCase4(void);
void ThreadCase5(void);

#ifdef __cplusplus
}
#endif

#endif
