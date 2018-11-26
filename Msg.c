#include "Msg.h"

//===========================================================
//msgget,msgrcv,msgsnd, msgctl
//index==1: receiver
//index==2: sender

struct my_msg_st {
    long int my_msg_type;//������һ�������ͳ�Ա������ʼ
    char some_text[MAX_TEXT];//�û��Զ�������
};

void MsgCase(int index)
{
    printf("%s\n", __FUNCTION__);
    if((index != 1) && (index != 2))
    {
    	printf("Enter error param:%d\n", index);
	return;
    }

    int running = 1;
    int msgid;
    struct my_msg_st some_data;
    long int msg_to_receive = 0;
    char buffer[MAX_TEXT];

/* First, we set up the message queue. */
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (msgid == -1) 
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if(index == 1)//receiver
    {
	/* Then the messages are retrieved from the queue, until an end message is encountered. */
	    while(running) 
	    {
	        //msg_to_receive = 0, ������Ϣ���͵�˳��������
	        //msg_to_receive = n(n > 0), ֻ��ȡĳһ�ض����͵���Ϣ
	        //msg_to_receive = -n(n > 0), ֻ��ȡ���ڻ���С��n ����Ϣ 
	        if (msgrcv(msgid, (void *)&some_data, MAX_TEXT, msg_to_receive, 0) == -1) 
		 {
	            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
	            exit(EXIT_FAILURE);
	        }
	        printf("You wrote: %s", some_data.some_text);
	        if (strncmp(some_data.some_text, "end", 3) == 0) 
		 {
	            running = 0;
	        }
	    }

	/* Lastly, the message queue is deleted.*/
	    if (msgctl(msgid, IPC_RMID, 0) == -1) 
	    {
	        fprintf(stderr, "msgctl(IPC_RMID) failed:%s\n",strerror(errno));
	        exit(EXIT_FAILURE);
	    }
    }
    else if(index == 2)//sender
    {
	    while(running) 
	    {
	        printf("Enter some text: ");
	        fgets(buffer, BUFSIZ, stdin);
	        some_data.my_msg_type = 1;
	        strcpy(some_data.some_text, buffer);

		//�������ݳ��Ȳ�����������my_msg_type �ĳ���
	        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1) 
		 {
	            fprintf(stderr, "msgsnd failed\n");
	            exit(EXIT_FAILURE);
	        }
	        if (strncmp(buffer, "end", 3) == 0) 
		 {
	            running = 0;
	        }
	    }
    }

    return;
}
