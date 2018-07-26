#include <stdio.h>
#include <stdlib.h>
//=====================Case5:ShareLib====================//
#include "ShareLib.h"
//=====================Case4:SyncAdd====================//
#include "SyncAdd.h"
//=====================Case3:C++=======================//
#include "CPlusPlus.h"

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

//=====================Case1:Little/Big====================//
//Check the CPU is Little_endian Or Big_endian
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
//==================================================//

int main()
{
    //Case1
    bool isLittle = checkCPU();
    printf("The CPU is %s\n", isLittle == true? "Little":"Big");
    printf("The Linux kernel %c\n", MY_ENDIANNESS);

    //Case2
    strtok_test();
    strtok_r_test();

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

    //Case4
    SyncCase1();
    SyncCase2();

    //Case5
    ShareLib();
    return 0;
}
