#include "Process.h"

void ProcessCase1(void)
{
    printf("Running ls with system1\n");
    system("ls -l");
    //system��ͬ������,��Ҫls -l�˳��ŷ���
    printf("Done1.\n");
    sleep(1);
	
    printf("Running ls with system2\n");
    system("ls -l&");
    printf("Done2.\n");
    sleep(1);
}

void ProcessCase2(void)
{
    //����ĵ�һ��ֵ�ǳ������֣����һ��ֵ��0
    //char *const ls_argv[] = {"ls", "-l", 0};

    //��������
    //char *const ls_envp[] = {"PATH=/bin:/usr/bin", "TERM=console", 0};
    printf("Running ls with exec\n");

    //exec�����飬6ѡ1
    //execl("/bin/ls", "ls", "-l", 0);
    execlp("ls", "ls", "-l", 0);
    //execle("/bin/ls", "ls", "-l", 0, ls_envp);

    //execv("/bin/ls", ls_argv);
    //execvp("ls", ls_argv);
    //execve("/bin/ls", ls_argv, ls_envp);

	
    //exec�е�ls �����滻��ǰ����,Done3������ʾ
    printf("Done3.\n");
    sleep(100);
}

void ProcessCase3(void)
{
    pid_t pid;
    std::string message;
    int n;
    int exit_code;

    printf("fork program starting\n");
    pid = fork();
    switch(pid) 
    {
    case -1:
        exit(1);
    case 0:
        message = "This is the child";
        n = 5;
        exit_code = 37;
        break;
    default:
        message = "This is the parent";
        n = 3;
        exit_code = 0;
        break;
    }

    for(; n > 0; n--) 
    {
        puts(message.c_str());
        sleep(1);
    }

/*  This section of the program waits for the child process to finish.  */

    if(pid) 
    {
        int stat_val;
        pid_t child_pid;

        child_pid = wait(&stat_val);

        printf("Child has finished: PID = %d\n", child_pid);
        if(WIFEXITED(stat_val))
            printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
        else
            printf("Child terminated abnormally\n");
    }
    exit (exit_code);
}