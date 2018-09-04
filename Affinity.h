#ifndef AFFINITY_H
#define AFFINITY_H

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sysinfo.h>
#include<unistd.h>
 
//#define __USE_GNU
#include<sched.h>
#include<ctype.h>
#include<string.h>
#include<pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

int AffinityTest(void);

#ifdef __cplusplus
}
#endif

#endif
