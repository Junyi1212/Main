#include "Signal.h"

//还有其他几种方式
//Reference: https://blog.csdn.net/centnetHY/article/details/79695464
static int count = 0;
static void SelfTest1Handler(int number)
{
	alarm(1);
	count++;
	printf("%s: count=%d\n", __FUNCTION__, count);
}

static void SelfTest2Handler(int number)
{  
	time_t t;  
	char p[32];  

	time(&t);  
	strftime(p, sizeof(p), "%T", localtime(&t));  
	printf("%s: time is %s\n", __FUNCTION__, p);  
}  

void TimerTest1(void)
{
	alarm(1);
	signal(SIGALRM, SelfTest1Handler);
}

void TimerTest2(void)
{  
	struct sigevent evp;  
	struct itimerspec ts;  
	timer_t timer;  
	int ret;  

	memset(&evp, 0, sizeof(struct sigevent));

	evp.sigev_value.sival_ptr = &timer;  
	evp.sigev_notify = SIGEV_SIGNAL;  
	evp.sigev_signo = SIGUSR1;  
	signal(SIGUSR1, SelfTest2Handler);  

	ret = timer_create(CLOCK_REALTIME, &evp, &timer);  
	if(ret)
	{
		printf("timer_create");  
	}

	ts.it_interval.tv_sec = 1;  
	ts.it_interval.tv_nsec = 0;  
	ts.it_value.tv_sec = 3;  
	ts.it_value.tv_nsec = 0;  

	ret = timer_settime(timer, 0, &ts, NULL);  
	if(ret)   
	{
		printf("timer_settime");  
	}
} 

