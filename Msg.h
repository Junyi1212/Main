#ifndef MSG_H
#define MSG_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TEXT 512

void MsgCase(int index);

#ifdef __cplusplus
}
#endif

#endif