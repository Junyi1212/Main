#include "SyncAdd.h"

//Reference: https://blog.csdn.net/houzengjiang/article/details/40918673
//===========================================================
//Case 1
#define WORK_SIZE 5000000
#define WORKER_COUNT 10
pthread_t g_tWorkerID[WORKER_COUNT];

int g_iSum = 0;
std::atomic_long g_iSum_atomic;

int g_iFlagCase = 1;
void * thread_1(void *arg) 
{
   //printf ("Thread %08X Startup\n", (unsigned int)pthread_self());
   int i=0;
   for (i=0; i<WORK_SIZE; i++) 
   {
	switch(g_iFlagCase)
	{
	    case 1:
		__sync_fetch_and_add(&g_iSum, 1);
		break;
	    case 2:
		g_iSum++;
		break;
	    case 3:
		g_iSum_atomic++;
		break;
	    default:
		break;
	}
   }
   return NULL;
}

void SyncAdd(void)
{
   g_iSum = 0;

   g_iFlagCase = 1;
   
   int i=0; 
   for (i=0;i<WORKER_COUNT;++i) 
   {
       pthread_create(&g_tWorkerID[i], NULL, thread_1, NULL);
   }
   for (i=0;i<WORKER_COUNT;++i) 
   {
       pthread_join(g_tWorkerID[i], NULL);
   }

   printf ("The Sum: %d\n", g_iSum);
}

void OriginAdd(void)
{
   g_iSum = 0;

   g_iFlagCase = 2;
   
   int i=0; 
   for (i=0;i<WORKER_COUNT;++i) 
   {
       pthread_create(&g_tWorkerID[i], NULL, thread_1, NULL);
   }
   for (i=0;i<WORKER_COUNT;++i) 
   {
       pthread_join(g_tWorkerID[i], NULL);
   }

   printf ("The Sum: %d\n", g_iSum);
}

void SyncCase1(void)
{
    printf("%s\n", __FUNCTION__);
    SyncAdd();
    OriginAdd();
}
//===========================================================
//Case 2
pthread_mutex_t mutex_lock;  
void *thread_2(void *arg)  
{  
    int i = 0;  
    for(i = 0; i < WORK_SIZE; i++)  
    {  
        pthread_mutex_lock(&mutex_lock);  
        g_iSum++;
        pthread_mutex_unlock(&mutex_lock);  
    }  
    return NULL;  
}

void LockAdd(void)  
{  
    g_iSum = 0;
    pthread_mutex_init(&mutex_lock, NULL);  

    int i = 0;  
    for(i = 0; i < WORKER_COUNT; i++)  
    {  
        pthread_create(&g_tWorkerID[i], NULL, thread_2, NULL);  
    }  
  
    for(i = 0; i < WORKER_COUNT; i++)  
    {  
        pthread_join(g_tWorkerID[i], NULL);  
    }  

    pthread_mutex_destroy(&mutex_lock);
    printf ("The Sum: %d\n", g_iSum); 
}

void AtomicAdd(void)
{
   g_iSum_atomic = 0;

   g_iFlagCase = 3;
   
   int i=0; 
   for (i=0;i<WORKER_COUNT;++i) 
   {
       pthread_create(&g_tWorkerID[i], NULL, thread_1, NULL);
   }
   for (i=0;i<WORKER_COUNT;++i) 
   {
       pthread_join(g_tWorkerID[i], NULL);
   }

   std::cout<<"The Sum: "<<g_iSum_atomic<< std::endl;
}

void SyncCase2(void)  
{  
    printf("%s\n", __FUNCTION__);
    struct timeval t1;  
    struct timeval t2;
    struct timeval t3;
    struct timeval t4;
    gettimeofday(&t1,NULL);
    SyncAdd();
    gettimeofday(&t2,NULL);
    LockAdd();
    gettimeofday(&t3,NULL);
    AtomicAdd();
    gettimeofday(&t4,NULL);

    long millSec1 = (t2.tv_sec * 1000 + t2.tv_usec/1000) - (t1.tv_sec * 1000 + t1.tv_usec/1000);
    long millSec2 = (t3.tv_sec * 1000 + t3.tv_usec/1000) - (t2.tv_sec * 1000 + t2.tv_usec/1000);
    long millSec3 = (t4.tv_sec * 1000 + t4.tv_usec/1000) - (t3.tv_sec * 1000 + t3.tv_usec/1000);
    printf("SyncAdd costs %llu ms\n", millSec1);
    printf("LockAdd costs %llu ms\n", millSec2);
    printf("AtomicAdd costs %llu ms\n", millSec3);
}  
