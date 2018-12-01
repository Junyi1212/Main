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
    String(const char *str=NULL);//普通构造函数
    String(const String &other);//拷贝构造函数
    void StringPrint(void);
    String& operator=(const String &other);//赋值函数
    ~String(void);//析构函数
private:
    char *m_data;//保存字符串
};

void CPPTest3(void);

#endif

