#include <stdio.h>
#include <stdlib.h>

//=====================Case19:Msg消息队列===================//
#include "Msg.h"
//=====================Case18:Shm共享内存===================//
#include "Shm.h"
//=====================Case17:Sem信号量===================//
#include "Sem.h"
//=====================Case16:Pipe管道===================//
#include "Pipe.h"
//=====================Case15:Thread===================//
#include "Thread.h"
//=====================Case14:Process===================//
#include "Process.h"
//=====================Case13:Dbm===================//
#include "Dbm.h"
//=====================Case11:Limit===================//
#include "Limits.h"
//=====================Case10:Dir===================//
#include "PrintDir.h"
//=====================Case9:List===================//
#include "SimpleList.h"
#include "ListExample.h"
//=====================Case8:Affinity===================//
#include "Affinity.h"
//=====================Case7:Signal Timer================//
#include "Signal.h"
//=====================Case6:Exception===================//
#include "Exception.h"
//=====================Case5:ShareLib====================//
#include "ShareLib.h"
//=====================Case4:SyncAdd====================//
#include "SyncAdd.h"
//=====================Case3:C++=======================//
#include "CPlusPlus.h"
#include "CPlusPlusNew.h"
//=====================Case2:strtok/strtok_r=================//
//Reference: https://blog.csdn.net/buaa_shang/article/details/8189984 
void strtok_test()
{
        int in = 0, j;
        char buffer[20] = "Fred,John,Ann";
        const char *sep  = " ,.;";//满足其中一个分隔符即可
        char *p[3];
        char *buf = buffer;
	 printf("buffer before= %s\n", buffer);
        while((p[in] = strtok(buf, sep)) != NULL)
        {   
                in++;
                buf = NULL;
        }   
        printf("buffer after= %s\n", buffer);
        printf("Here we have %d strings\n", in);
        for(j = 0; j < in; j++)
                printf(">%s<\n", p[j]);
}

//多级分割
void strtok_r_test()
{
        int in = 0, j;
        char buffer[100] = "Fred male 25,John male 62,Anna female 16";
        char *p[20];
        char *buf = buffer;
        char *outer_ptr = NULL;
        char *inner_ptr = NULL;
        while((p[in] = strtok_r(buf, ",", &outer_ptr)) != NULL) 
        {   
                buf = p[in];
                while((p[in] = strtok_r(buf, " ", &inner_ptr)) != NULL)
                {    
                        in++;
                        buf = NULL;
                }   
                buf = NULL;
        }   
        printf("Here we have %d strings\n", in);
        for(j = 0; j < in; j++)
                printf(">%s<\n", p[j]);
}

//=====================Case1:Linux Env====================//

//Case1.1: Little/Big: Check the CPU is Little_endian Or Big_endian
//union的存放顺序是所有成员都从低地址开始存放
//option 1
bool checkCPU(void)
{
    union w
    {
        int a;
        char b;
    }c;
    c.a = 1;
    return (c.b == 1);
}
//option 2
//Linux Kernel Implement
//MY_ENDIANNESS = l(little), b(big)
static union
{
    char c[4];
    unsigned int l;
} endian_test = {{'l', '?', '?', 'b'}};
#define MY_ENDIANNESS ((char)(endian_test.l))
//--------------------------------------------------
//Case1.2:读取和设置环境变量
extern char **environ;
void checkEnv(void)
{
    const char* keyVar = "testEnv";
    const char* setVar = "testEnv=123";
    char* valueVar = NULL;

    valueVar = getenv(keyVar);
    if(valueVar)
    	printf("Key %s = %s\n", keyVar, valueVar);
    else
	printf("Key %s has no value!\n", keyVar);

    //该环境变量仅对程序本身有效，不会反映到外部环境中；
    //因为变量的值不会从子进程(该程序)传播到父进程(Shell)
    if(putenv((char* )setVar) != 0)
    {
    	printf("putenv failed\n");
	return;
    }

    valueVar = getenv(keyVar);
    if(valueVar)
    	printf("Key %s = %s\n", keyVar, valueVar);
    else
	printf("Key %s has no value!\n", keyVar);

    //打印出所有环境变量
    char **env = environ;
    while(*env)
    {
    	printf("%s\n", *env);
	env++;
    }
}
//--------------------------------------------------
//Case1.3:创建临时文件
void create_tmpfile(void)
{
    char tmpname[L_tmpnam];
    char *filename; 
    FILE *tmpfp;

    //创建临时文件名，应尽快打开，以减小另外一个程序用同样的名字
    //filename = tmpnam(tmpname);
    //printf("Temporary file name is: %s\n", filename);

    //创建并打开文件流，可以避免tmpname的问题
    tmpfp = tmpfile();
    if(tmpfp)
        printf("Opened a temporary file OK\n");
    else
	perror("tmpfile");
}

//==================================================//
void printHelp(void)
{
	printf("\r\n");
	printf("USAGE\r\n");
	printf("    -1    Case1:Check the Linux Env.\r\n");
	printf("    -2    Case2:The example of strtok() and strtok_r().\r\n");
	printf("    -3    Case3:The example of C++ usage.\r\n");
	printf("    -4    Case4:The correction and efficiency of atomic operation.\r\n");
	printf("    -5    Case5:The example of shared lib usage.\r\n");
	printf("    -6    Case6:The example of exception process.\r\n");
	printf("    -7    Case7:The example of signal timer.\r\n");
	printf("    -8    Case8:The example of cpu affinity.\r\n");
	printf("    -9    Case9:The example of sorted doubly linked list .\r\n");
	printf("    -a    Case10:The example of print all directory and file.\r\n");
	printf("    -b    Case11:The example of getting and setting system resources.\r\n");
	printf("    -c    Case12:The example of dbm.\r\n");
	printf("    -d    Case13:The example of Process.\r\n");
	printf("    -e    Case14:The example of Thread.\r\n");
	printf("    -f     Case15:The example of Pipe.\r\n");
	printf("    -g    Case16:The example of Sem.\r\n");
	printf("    	1    The 1st program.\r\n");
	printf("    	2    The 2st program.\r\n");
	printf("    -h    Case17:The example of Shm.\r\n");
	printf("    	1    The consumer program.\r\n");
	printf("    	2    The producer program.\r\n");
	printf("    -i    Case18:The example of Msg.\r\n");
	printf("    	1    The receiver program.\r\n");
	printf("    	2    The sender program.\r\n");
	printf("\r\n");
}

int main(int argc, char **argv)
{
	int c = 0;
	if (argc == 1)
	{
		printHelp();
		return -1;
	}

	while ((c = getopt(argc, argv, "123456789abcdefg:h:i:")) != -1)
	{
		switch (c)
		{
			case '1':
			{
				//Case1
				bool isLittle = checkCPU();
				printf("The CPU is %s\n", isLittle == true? "Little":"Big");
				printf("The Linux kernel %c\n", MY_ENDIANNESS);

				checkEnv();

				create_tmpfile();
				break;
			}
			case '2':
			{
				//Case2
				strtok_test();
				strtok_r_test();
				break;
			}
			case '3':
			{
				//Case3
				String str1 = String("111");
				String str2 = String(str1);
				String str3 = String("222");
				String str4;
				String str5 = String("333");
				str4 = str3;
				str5 = str5;
				str1.StringPrint();
				str2.StringPrint();
				str3.StringPrint();
				str4.StringPrint();
				str5.StringPrint();

				CPP_NewTest1_1();
				CPP_NewTest1_2();
				CPP_NewTest1_3();
				CPP_NewTest1_4();
				CPP_NewTest3_1();
				CPP_NewTest3_2();
				CPP_NewTest3_3();
				CPP_NewTest4_1();
				CPP_NewTest4_2();
				CPP_NewTest4_3();
				CPP_NewTest5_1();
				CPP_NewTest5_2();
				CPP_NewTest5_3();
				CPP_NewTest6_1();
				break;
			}
			case '4':
			{
				//Case4
				SyncCase1();
				SyncCase2();
				break;
			}
			case '5':
			{
				//Case5
				ShareLib();
				break;
			}
			case '6':
			{
				//Case6
				ExceptionInit();
				ExceptionStart();
				break;
			}
			case '7':
			{
				//Case7
				TimerTest1();
				TimerTest2();
				//SignalTest1();
				SignalTest2();
				while(1);
				break;
			}
			case '8':
			{
				//Case8
				AffinityTest();
				break;
			}
			case '9':
			{
				//Case9
				ListTest1();
				break;
			}
			case 'a':
			{
				//Case10
				PrintDirTest();
				break;	
			}
			case 'b':
			{
				//Case11
				LimitsTest();
			}
			case 'c':
			{
				//Case12
				DbmTest();
			}
			case 'd':
			{
				//Case13
				ProcessCase1();
				//ProcessCase2();
				ProcessCase3();
			}
			case 'e':
			{
				//Case14
				ThreadCase1();
				ThreadCase2();
				ThreadCase3();
				ThreadCase4();
				ThreadCase5();
			}
			case 'f':
			{
				//Case15
				PipeCase1();
				PipeCase2();
				PipeCase3();
				PipeCase4();
				PipeCase5();
				PipeCase6();
				PipeCase7();
			}
			case 'g':
			{
				//Case16
				SemCase(atoi(optarg));
			}
			case 'h':
			{
				//Case17
				ShmCase(atoi(optarg));
			}
			case 'i':
			{
				//Case18
				MsgCase(atoi(optarg));
			}
			default:
			break;
		}
	}

	return 0;
}
