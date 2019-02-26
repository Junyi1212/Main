#ifndef SYNCADD_H
#define SYNCADD_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h> 
#include <atomic>
#include <time.h>
#include <unistd.h>
#include <iostream> 

#ifdef __cplusplus
extern "C" {
#endif

void SyncCase1(void);
void SyncCase2(void);

#ifdef __cplusplus
}
#endif

#endif