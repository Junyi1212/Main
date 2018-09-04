#ifndef LISTEXAMPLE_H
#define LISTEXAMPLE_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h> 
#include <string.h>
#include "SimpleList.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FILENAME_MAX_LENGTH 64
typedef enum EFilePriority
{
	EFilePriority_Error = 0,
	EFilePriority_High = 1,  
	EFilePriority_Normal = 2,  
	EFilePriority_Low = 3, 
	EFilePriority_NbrOf = 4
} EFilePriority;

typedef struct SFileInfo
{
	char name[FILENAME_MAX_LENGTH];
	char value;
	EFilePriority priority;
} SFileInfo;

typedef struct SFileListElement
{
	SListElement list;
	SFileInfo    *fileInfo;
} SFileListElement;

void ListTest1(void);

#ifdef __cplusplus
}
#endif

#endif
