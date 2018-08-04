#include "Affinity.h"

#define THREAD_MAX_NUM 100

int coreNum=0;  //cpu中核数
void* threadFun(void* arg)
{
	cpu_set_t mask;  //CPU核的集合
	cpu_set_t get;   //获取在集合中的CPU
	int *a = (int *)arg; 
	printf("this is:%d\n",*a);  //显示是第几个线程
	CPU_ZERO(&mask);    //置空
	CPU_SET(*a,&mask);   //设置亲和力值
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1)//设置线程CPU亲和力
	{
		printf("warning: could not set CPU affinity, continuing...\n");
	}
	CPU_ZERO(&get);
	if (sched_getaffinity(0, sizeof(get), &get) == -1)//获取线程CPU亲和力
	{
		printf("warning: cound not get thread affinity, continuing...\n");
	}
	int i;
	for (i = 0; i < coreNum; i++)
	{
		if (CPU_ISSET(i, &get))//判断线程与哪个CPU有亲和力
		{
			printf("this thread %d is running processor : %d\n", i,i);
		}
	}

	return NULL;
}
 
int AffinityTest(void)
{
     coreNum = sysconf(_SC_NPROCESSORS_CONF);  //获取核数
     pthread_t thread[THREAD_MAX_NUM];
     printf("system has %i processor(s). \n", coreNum);
     int tid[THREAD_MAX_NUM];
     int i;
     for(i=0;i<coreNum;i++)
     {
           tid[i] = i;
           pthread_create(&thread[i],NULL,threadFun,(void*)&tid[i]);
     }
     return 0;
}
