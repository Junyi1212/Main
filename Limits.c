#include "Limits.h"

void work()
{
    FILE *f;
    int i;
    double x = 4.5;

    f = tmpfile();
    for(i = 0; i < 10000; i++) 
    {
        fprintf(f,"Do some output\n");
        if(ferror(f)) 
	 {
            fprintf(stderr,"Error writing to temporary file\n");
            exit(1);
        }
    }

    //performs some arithmetic to generate load on the CPU.
    for(i = 0; i < 1000000; i++)
        x = log(x*x + 3.21);
}

void LimitsTest(void)
{
    struct rusage r_usage;
    struct rlimit r_limit;
    int priority;

    work();

    //计算当前程序耗费的CPU时间(用户时间和系统时间)
    getrusage(RUSAGE_SELF, &r_usage);
    //用户时间: 程序执行自身的指令所耗费的时间
    //系统时间: 操作系统为程序执行所耗费的时间，即执行输入输出操作的系统调用或者其他系统函数所耗费的时间
    printf("CPU usage: User = %ld.%06ld, System = %ld.%06ld\n",
        r_usage.ru_utime.tv_sec, r_usage.ru_utime.tv_usec,
        r_usage.ru_stime.tv_sec, r_usage.ru_stime.tv_usec);

    //获取当前进程的优先级,默认是0
    priority = getpriority(PRIO_PROCESS, getpid());
    printf("Current priority = %d\n", priority);

    //获取资源限制
    //RLIMIT_CORE:      core dump文件的大小限制，以字节为单位
    //RLIMIT_CPU:        CPU时间限制，以秒为单位
    //RLIMIT_DATA:       数据段限制，以字节为单位
    //RLIMIT_FSIZE:      文件大小限制，以字节为单位
    //RLIMIT_NOFILE:    可以打开的文件数限制
    //RLIMIT_STACK:    栈大小限制，以字节为单位
    //RLIMIT_AS:          地址空间(栈和数据)限制，以字节为单位
    getrlimit(RLIMIT_FSIZE, &r_limit);
    printf("Current FSIZE limit: soft = %ld, hard = %ld\n",
        r_limit.rlim_cur, r_limit.rlim_max);
    //rlim_cur当前的软限制，建议性的，超过会导致库函数返回错误
    //rlim_max硬限制，超过会导致系统发送信号终止程序

    //we set a file size limit using setrlimit and call work again, which fails because 
    //it attempts to create too large a file.
    r_limit.rlim_cur = 2048;
    r_limit.rlim_max = 4096;
    printf("Setting a 2K file size limit\n");
    setrlimit(RLIMIT_FSIZE, &r_limit);

    work();
}
