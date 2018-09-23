#include "Thread.h"
//===========================================================
//Case 1
//基本的用法
char message1[] = "Hello World";

void *thread_function1(void *arg) 
{
    printf("thread_function is running. Argument was: %s\n", (char *)arg);
    sleep(1);
    strcpy(message1, "Bye!");
    pthread_exit((void*)("Thank you for the CPU time"));
}

void ThreadCase1(void)  
{
    printf("%s\n", __FUNCTION__);
    int res;
    pthread_t a_thread;
    void *thread_result;
    res = pthread_create(&a_thread, NULL, thread_function1, (void *)message1);
    if (res != 0) 
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) 
    {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined, it returned: %s\n", (char *)thread_result);
    printf("Message is: %s\n", message1);
    return;
}  
//===========================================================
//Case 2
//使用信号量同步
//该信号量指的是线程层面
sem_t bin_sem;

#define WORK_SIZE 1024
char work_area[WORK_SIZE];

void *thread_function2(void *arg) 
{
    sem_wait(&bin_sem);
    while(strncmp("end", work_area, 3) != 0) 
    {
        printf("You input %d characters\n", strlen(work_area) -1);
        sem_wait(&bin_sem);
    }
    pthread_exit(NULL);
}

void ThreadCase2(void)  
{
    printf("%s\n", __FUNCTION__);
    int res;
    pthread_t a_thread;
    void *thread_result;

    res = sem_init(&bin_sem, 0, 0);
    if (res != 0) 
    {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_create(&a_thread, NULL, thread_function2, NULL);
    if (res != 0) 
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Input some text. Enter 'end' to finish\n");
    memset(work_area, 0, WORK_SIZE);
    while(strncmp("end", work_area, 3) != 0) 
    {
        fgets(work_area, WORK_SIZE, stdin);
        sem_post(&bin_sem);
    }
    printf("\nWaiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) 
    {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    sem_destroy(&bin_sem);
    return;
}
//===========================================================
//Case 3
//使用互斥锁同步
pthread_mutex_t work_mutex;

void *thread_function3(void *arg) 
{
    while(1)
    {
	    pthread_mutex_lock(&work_mutex);
	    if(strncmp("end", work_area, 3) != 0) 
	    {
	        printf("You input %d characters\n", strlen(work_area) -1);
	        pthread_mutex_unlock(&work_mutex);
		 sleep(1);
	    }
	    else
	    {
	    	break;
           }
    }
    pthread_mutex_unlock(&work_mutex);
    pthread_exit(NULL);
}

void ThreadCase3(void)  
{
    printf("%s\n", __FUNCTION__);
    int res;
    pthread_t a_thread;
    void *thread_result;
    res = pthread_mutex_init(&work_mutex, NULL);
    if (res != 0) 
    {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_create(&a_thread, NULL, thread_function3, NULL);
    if (res != 0) 
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Input some text. Enter 'end' to finish\n");
    memset(work_area, 0, WORK_SIZE);

    while(1)
    {
    	    pthread_mutex_lock(&work_mutex);
	    if(strncmp("end", work_area, 3) != 0) 
	    {
	        fgets(work_area, WORK_SIZE, stdin);
	        pthread_mutex_unlock(&work_mutex);
		 sleep(1);
	    }
           else
           {
           	break;
           }
    }
    pthread_mutex_unlock(&work_mutex);
    printf("\nWaiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) 
    {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    pthread_mutex_destroy(&work_mutex);
    return;
}

//===========================================================
//Case 4
//使用线程属性: detach设置线程为脱离状态
//pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
//1. PTHREAD_CREATE_JOINABLE(默认标志);
//2. PTHREAD_CREATE_DETACHED, 主线程不能调用pthread_join来获得另一个线程的退出状态

char message4[] = "Hello World";
int thread_finished = 0;

void *thread_function4(void *arg) 
{
    printf("thread_function is running. Argument was %s\n", (char *)arg);
    sleep(2);
    printf("Second thread setting finished flag, and exiting now\n");
    thread_finished = 1;
    pthread_exit(NULL);
}

void ThreadCase4(void)   
{
    printf("%s\n", __FUNCTION__);
    int res;
    pthread_t a_thread;
    void *thread_result;
    pthread_attr_t thread_attr;

    res = pthread_attr_init(&thread_attr);
    if (res != 0) 
    {
        perror("Attribute creation failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (res != 0) 
    {
        perror("Setting detached attribute failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_create(&a_thread, &thread_attr, thread_function4, (void *)message4);
    if (res != 0) 
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    (void)pthread_attr_destroy(&thread_attr);
    while(!thread_finished) 
    {
        printf("Waiting for thread to say it's finished...\n");
        sleep(1);
    }
    printf("Other thread finished, bye!\n");
    return;
}
//===========================================================
//Case 5
//取消线程
void *thread_function5(void *arg) 
{
    int i, res, j;

    //允许取消
    res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (res != 0) 
    {
        perror("Thread pthread_setcancelstate failed");
        exit(EXIT_FAILURE);
    }

    //1.PTHREAD_CANCEL_ASYNCHRONOUS立即取消
    //2.PTHREAD_CANCEL_DEFERRED延时取消
    //延时取消一直等待到线程执行了下述函数后才采取行动
    //sleep, pthread_join, pthread_cond_wait, pthread_cond_timewait, pthread_testcancel, sem_wait, sigwait
    res = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    if (res != 0) 
    {
        perror("Thread pthread_setcanceltype failed");
        exit(EXIT_FAILURE);
    }
    printf("thread_function5 is running\n");
    for(i = 0; i < 10; i++) 
    {
        printf("Thread is still running (%d)...\n", i);
        sleep(1);
    }
    printf("pthread_exit\n");
    pthread_exit(NULL);
}

void ThreadCase5(void)  
{
    printf("%s\n", __FUNCTION__);
    int res;
    pthread_t a_thread;
    void *thread_result;

    res = pthread_create(&a_thread, NULL, thread_function5, NULL);
    if (res != 0) 
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    sleep(3);
    printf("Canceling thread...\n");
    res = pthread_cancel(a_thread);
    if (res != 0) 
    {
        perror("Thread cancelation failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) 
    {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    return;
}