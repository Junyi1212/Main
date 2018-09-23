#include "Signal.h"

//===========================================================
//Case 1
//使用信号实现timer
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

//===========================================================
//Case 2
//使用signal 自定义信号处理函数
//SIGINT对应Ctrl+c
void ouch1(int sig)
{
    printf("OUCH! - I got signal %d\n", sig);
    //在信号处理函数中调用printf被认为是不安全，有可能在执行期间被中断
    //安全调用的函数见<Linux程序设计(第4版)>  P414 表11-8
    (void) signal(SIGINT, SIG_DFL);
}

void SignalTest1(void)
{
    (void) signal(SIGINT, ouch1);

    while(1) 
    {
        printf("Hello World!\n");
        sleep(1);
    }
}

//===========================================================
//Case 3
//使用sigaction 自定义信号处理函数
//sigaction更新更健壮
void ouch2(int sig)
{
    printf("OUCH! - I got signal %d\n", sig);
}

void SignalTest2(void)
{
    struct sigaction act;

    act.sa_handler = ouch2;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, 0);
    //sigaction连续处理SIGINT信号，要想终止,输入Crtl+\
    while(1) 
    {
        printf("Hello World!\n");
        sleep(1);
    }
}

