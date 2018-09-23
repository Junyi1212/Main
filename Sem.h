#ifndef SEM_H
#define SEM_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/sem.h>

#ifdef __cplusplus
extern "C" {
#endif

/* according to X/OPEN we have to define it ourselves */
union semun {
	int val;                    /* value for SETVAL */
	struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
	unsigned short int *array;  /* array for GETALL, SETALL */
	struct seminfo *__buf;      /* buffer for IPC_INFO */
};

void SemCase(int index);

#ifdef __cplusplus
}
#endif

#endif