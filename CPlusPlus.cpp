#include "CPlusPlus.h"

//================================================
//Case1:类的基本函数 
String::String(const char *str)
{
    printf("%s_1\n", __FUNCTION__);
    if(NULL == str)
    {
        m_data = new char[1];
        if(NULL != m_data)
        {
            *m_data = '\0';
        }
    }
    else
    {
        int length = strlen(str);
        m_data = new char[length+1];
        if(NULL != m_data)
        {
            strcpy(m_data, str);
        }
    }
}

String::String(const String &other)
{
    printf("%s_2\n", __FUNCTION__);
    int length = strlen(other.m_data);
    m_data = new char[length+1];
    if(NULL != m_data)
    {
        strcpy(m_data, other.m_data);
    }
}

void String::StringPrint(void)
{
    printf("%s:%s\n", __FUNCTION__, m_data);
}

String& String::operator=(const String &other)
{
    printf("%s\n", __FUNCTION__);
    if(this == &other)
    {
        printf("itself\n");
        return *this;
    }
    delete [] m_data;
    int length =strlen(other.m_data);
    m_data = new char[length+1];
    if(NULL != m_data)
    {
        strcpy(m_data, other.m_data);
        return *this;
    }
}

String::~String(void)
{
    printf("%s\n", __FUNCTION__);
    delete [] m_data;
}
