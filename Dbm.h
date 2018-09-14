#ifndef DBM_H
#define DBM_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include <ndbm.h>
/* On some systems you need to replace the above with
#include <gdbm-ndbm.h>
*/

#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

void DbmTest(void);

#ifdef __cplusplus
}
#endif

#endif
