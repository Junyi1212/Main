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

    //���㵱ǰ����ķѵ�CPUʱ��(�û�ʱ���ϵͳʱ��)
    getrusage(RUSAGE_SELF, &r_usage);
    //�û�ʱ��: ����ִ�������ָ�����ķѵ�ʱ��
    //ϵͳʱ��: ����ϵͳΪ����ִ�����ķѵ�ʱ�䣬��ִ���������������ϵͳ���û�������ϵͳ�������ķѵ�ʱ��
    printf("CPU usage: User = %ld.%06ld, System = %ld.%06ld\n",
        r_usage.ru_utime.tv_sec, r_usage.ru_utime.tv_usec,
        r_usage.ru_stime.tv_sec, r_usage.ru_stime.tv_usec);

    //��ȡ��ǰ���̵����ȼ�,Ĭ����0
    priority = getpriority(PRIO_PROCESS, getpid());
    printf("Current priority = %d\n", priority);

    //��ȡ��Դ����
    //RLIMIT_CORE:      core dump�ļ��Ĵ�С���ƣ����ֽ�Ϊ��λ
    //RLIMIT_CPU:        CPUʱ�����ƣ�����Ϊ��λ
    //RLIMIT_DATA:       ���ݶ����ƣ����ֽ�Ϊ��λ
    //RLIMIT_FSIZE:      �ļ���С���ƣ����ֽ�Ϊ��λ
    //RLIMIT_NOFILE:    ���Դ򿪵��ļ�������
    //RLIMIT_STACK:    ջ��С���ƣ����ֽ�Ϊ��λ
    //RLIMIT_AS:          ��ַ�ռ�(ջ������)���ƣ����ֽ�Ϊ��λ
    getrlimit(RLIMIT_FSIZE, &r_limit);
    printf("Current FSIZE limit: soft = %ld, hard = %ld\n",
        r_limit.rlim_cur, r_limit.rlim_max);
    //rlim_cur��ǰ�������ƣ������Եģ������ᵼ�¿⺯�����ش���
    //rlim_maxӲ���ƣ������ᵼ��ϵͳ�����ź���ֹ����

    //we set a file size limit using setrlimit and call work again, which fails because 
    //it attempts to create too large a file.
    r_limit.rlim_cur = 2048;
    r_limit.rlim_max = 4096;
    printf("Setting a 2K file size limit\n");
    setrlimit(RLIMIT_FSIZE, &r_limit);

    work();
}
