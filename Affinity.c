#include "Affinity.h"

#define THREAD_MAX_NUM 100

int coreNum=0;  //cpu�к���
void* threadFun(void* arg)
{
	cpu_set_t mask;  //CPU�˵ļ���
	cpu_set_t get;   //��ȡ�ڼ����е�CPU
	int *a = (int *)arg; 
	printf("this is:%d\n",*a);  //��ʾ�ǵڼ����߳�
	CPU_ZERO(&mask);    //�ÿ�
	CPU_SET(*a,&mask);   //�����׺���ֵ
	if (sched_setaffinity(0, sizeof(mask), &mask) == -1)//�����߳�CPU�׺���
	{
		printf("warning: could not set CPU affinity, continuing...\n");
	}
	CPU_ZERO(&get);
	if (sched_getaffinity(0, sizeof(get), &get) == -1)//��ȡ�߳�CPU�׺���
	{
		printf("warning: cound not get thread affinity, continuing...\n");
	}
	int i;
	for (i = 0; i < coreNum; i++)
	{
		if (CPU_ISSET(i, &get))//�ж��߳����ĸ�CPU���׺���
		{
			printf("this thread %d is running processor : %d\n", i,i);
		}
	}

	return NULL;
}
 
int AffinityTest(void)
{
     coreNum = sysconf(_SC_NPROCESSORS_CONF);  //��ȡ����
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
