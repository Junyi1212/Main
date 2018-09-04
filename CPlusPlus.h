#ifndef CPLUSPLUS_H
#define CPLUSPLUS_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

class String
{
public:
    String(const char *str=NULL);//��ͨ���캯��
    String(const String &other);//�������캯��
    void StringPrint(void);
    String& operator=(const String &other);//��ֵ����
    ~String(void);//��������
private:
    char *m_data;//�����ַ���
};
#endif

