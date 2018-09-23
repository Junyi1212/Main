#include "Pipe.h"

//===========================================================
//Case 1
//popen��ʹ��
//��ȡ�ⲿ�������������ӹܵ��ж�ȡ����
void PipeCase1(void)  
{
    printf("%s\n", __FUNCTION__);
    FILE *read_fp;
    char buffer[BUFSIZ + 1];
    //BUFSIZ��stdio.h�еĺ궨��

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
//popen��ʹ��
//���ⲿ�����������������д��ܵ�
//���������У�ͬ�����Եõ��൱��Ч��:
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
//pipe��ʹ��
//ͬһ����
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
        //file_pipes[0]�Ƕ��ļ�������
        //file_pipes[1]��д�ļ�������
        //ע�ⲻ���ļ���FILE*
        //�����Ƚ��ȳ�
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
//pipe��ʹ��
//��ͬ����fork
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
//pipe��ʹ��
//��ͬ����fork, execl
//�����ӳ���PipeChild
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
	     //���ó���pipeChild
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
//pipe��ʹ��
//�ѹܵ�������׼����
//dup��ʹ��

//int dup(int file_descriptor);
//�����µ��ļ���������ͬfile_descriptorָ��ͬһ���ļ�(�ܵ�)���µ��ļ�����������ȡ��С�Ŀ���ֵ

//int dup2(int file_descriptor_one, int file_descriptor_two)
//�´������ļ���������������file_descriptor_two��ͬ�������ǵ�һ�����ڸò���������ֵ
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
	 //��ʱ���ӽ��̸���2���ļ�����������4��
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
//fifo�����ܵ���ʹ��
//��ͬ����֮���ͨ��
//ʹ�÷���:
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

    //�ж��ļ��Ƿ����
    if (access(FIFO_NAME, F_OK) == -1) 
    {
        res = mkfifo(FIFO_NAME, 0777);
        if (res != 0) 
	 {
            fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
            exit(EXIT_FAILURE);
        }
    }

    //PIPE_BUF��limit.h�еĺ궨�壬��ʾFIFO�п��Դ�����ݵ����ֵ
    printf("PIPE_BUF =  %d \n", PIPE_BUF);
    printf("Process %d opening FIFO O_WRONLY\n", getpid());
    pipe_fd = open(FIFO_NAME, O_WRONLY);
    //openĬ��������ģʽ��һֱ�ȴ��������̴����FIFO
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

