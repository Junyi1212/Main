#ifndef SHM_H
#define SHM_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEXT_SZ 2048

struct shared_use_st {
    int written_by_you;
    char some_text[TEXT_SZ];
};

void ShmCase(int index);

#ifdef __cplusplus
}
#endif

#endif