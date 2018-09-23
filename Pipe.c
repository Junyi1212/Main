#include "Pipe.h"

//===========================================================
//Case 1
//popen的使用
//读取外部程序的输出，即从管道中读取数据
void PipeCase1(void)  
{
    printf("%s\n", __FUNCTION__);
    FILE *read_fp;
    char buffer[BUFSIZ + 1];
    //BUFSIZ是stdio.h中的宏定义

    printf("%d, %d\n", BUFSIZ, sizeof(char));
    int chars_read;
    memset(buffer, '\0', sizeof(buffer));

    read_fp = popen("uname -a", "r");
    if (read_fp != NULL) 
    {
        chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        if (chars_read > 0) 
	 {
            printf("Output was:\n%s\n", buffer);
        }
        pclose(read_fp);
        return;
    }
    return;
}  

//===========================================================
//Case 2
//popen的使用
//向外部程序输出，即将数据写入管道
//在命令行中，同样可以得到相当的效果:
//echo "Once upon a time, there was..." | od -c
void PipeCase2(void)  
{
    printf("%s\n", __FUNCTION__);
    FILE *write_fp;
    char buffer[BUFSIZ + 1];

    sprintf(buffer, "Once upon a time, there was...\n");
    write_fp = popen("od -c", "w");
    if (write_fp != NULL) 
    {
        fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
        pclose(write_fp);
        return;
    }
    return;
}

//===========================================================
//Case 3
//pipe的使用
//同一进程
void PipeCase3(void)  
{
    printf("%s\n", __FUNCTION__);
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    char buffer[BUFSIZ + 1];

    memset(buffer, '\0', sizeof(buffer));

    if (pipe(file_pipes) == 0) 
    {
        //file_pipes[0]是读文件描述符
        //file_pipes[1]是写文件描述符
        //注意不是文件流FILE*
        //数据先进先出
        data_processed = write(file_pipes[1], some_data, strlen(some_data));
        printf("Wrote %d bytes\n", data_processed);
        data_processed = read(file_pipes[0], buffer, BUFSIZ);
        printf("Read %d bytes: %s\n", data_processed, buffer);
        return;
    }
    return;
}

//===========================================================
//Case 4
//pipe的使用
//不同进程fork
void PipeCase4(void)  
{
    printf("%s\n", __FUNCTION__);
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    char buffer[BUFSIZ + 1];
    pid_t fork_result;

    memset(buffer, '\0', sizeof(buffer));

    if (pipe(file_pipes) == 0) 
    {
        fork_result = fork();
        if (fork_result == -1) 
	 {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }

	 // We've made sure the fork worked, so if fork_result equals zero, we're in the child process.
        if (fork_result == 0) 
	 {
            data_processed = read(file_pipes[0], buffer, BUFSIZ);
            printf("Read %d bytes: %s\n", data_processed, buffer);
            exit(EXIT_SUCCESS);
        }

	 // Otherwise, we must be the parent process.
        else 
	 {
            data_processed = write(file_pipes[1], some_data, strlen(some_data));
            printf("Wrote %d bytes\n", data_processed);
        }
    }
    return;
}

//===========================================================
//Case 5
//pipe的使用
//不同进程fork, execl
//调用子程序PipeChild
void PipeCase5(void)  
{
    printf("%s\n", __FUNCTION__);
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    char buffer[BUFSIZ + 1];
    pid_t fork_result;

    memset(buffer, '\0', sizeof(buffer));

    if (pipe(file_pipes) == 0) 
    {
        fork_result = fork();
        if (fork_result == (pid_t)-1) 
	 {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }

        if (fork_result == 0) 
	 {
	     //调用程序pipeChild
            sprintf(buffer, "%d", file_pipes[0]);
            (void)execl("PipeChild", "PipeChild", buffer, (char *)0);
            exit(EXIT_FAILURE);
        }
        else 
	 {
            data_processed = write(file_pipes[1], some_data, strlen(some_data));
            printf("%d - wrote %d bytes\n", getpid(), data_processed);
        }
    }
    return;
}

//===========================================================
//Case 6
//pipe的使用
//把管道用作标准输入
//dup的使用

//int dup(int file_descriptor);
//创建新的文件描述符，同file_descriptor指向同一个文件(管道)，新的文件描述符总是取最小的可用值

//int dup2(int file_descriptor_one, int file_descriptor_two)
//新创建的文件描述符，或者与file_descriptor_two相同，或者是第一个大于该参数的有用值
void PipeCase6(void)  
{
    printf("%s\n", __FUNCTION__);
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    pid_t fork_result;

    if (pipe(file_pipes) == 0) 
    {
        fork_result = fork();
	 //此时父子进程各有2个文件描述符，共4个
        if (fork_result == (pid_t)-1) 
	 {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }

        if (fork_result == (pid_t)0) 
	 {
            close(0);
            dup(file_pipes[0]);
            close(file_pipes[0]);
            close(file_pipes[1]);

            execlp("od", "od", "-c", (char *)0);
            exit(EXIT_FAILURE);
        }
        else 
	 {
            close(file_pipes[0]);
            data_processed = write(file_pipes[1], some_data, strlen(some_data));
            close(file_pipes[1]);
            printf("%d - wrote %d bytes\n", (int)getpid(), data_processed);
        }
    }
    return;
}

//===========================================================
//Case 7
//fifo有名管道的使用
//不同进程之间的通信
//使用方法:
// 1 ./MyTest -f &
// 2 ./FifoChild

#define FIFO_NAME "my_fifo"
#define BUFFER_SIZE PIPE_BUF
#define TEN_MEG (1024 * 1024 * 10)
void PipeCase7(void)  
{
    printf("%s\n", __FUNCTION__);
    int pipe_fd;
    int res;

    int bytes_sent = 0;
    char buffer[BUFFER_SIZE + 1];

    //判断文件是否存在
    if (access(FIFO_NAME, F_OK) == -1) 
    {
        res = mkfifo(FIFO_NAME, 0777);
        if (res != 0) 
	 {
            fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
            exit(EXIT_FAILURE);
        }
    }

    //PIPE_BUF是limit.h中的宏定义，表示FIFO中可以存放数据的最大值
    printf("PIPE_BUF =  %d \n", PIPE_BUF);
    printf("Process %d opening FIFO O_WRONLY\n", getpid());
    pipe_fd = open(FIFO_NAME, O_WRONLY);
    //open默认是阻塞模式，一直等待到读进程打开这个FIFO
    printf("Process %d result %d\n", getpid(), pipe_fd);

    if (pipe_fd != -1) 
    {
        while(bytes_sent < TEN_MEG) 
	 {
            res = write(pipe_fd, buffer, BUFFER_SIZE);
            if (res == -1) 
	     {
                fprintf(stderr, "Write error on pipe\n");
                exit(EXIT_FAILURE);
            }
            bytes_sent += res;
        }
        (void)close(pipe_fd); 
    }
    else 
    {
        exit(EXIT_FAILURE);        
    }

    printf("Process %d finished\n", getpid());
    return;
}

