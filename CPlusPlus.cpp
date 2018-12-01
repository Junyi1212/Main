#include "CPlusPlus.h"

//================================================
//Case1:��Ļ������� 
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
//================================================
//Case2:��ļ̳�
//--------------------------------------------------------------------------------------------------------
//�����Ա|    private   |  protected  |   pubilc    |   private   |  protected  |     public   |   private    | protected | public |
//--------------------------------------------------------------------------------------------------------
//�̳з�ʽ|                      private                     |                       protected                 |                    public                   |
//--------------------------------------------------------------------------------------------------------
//����           |
//�ڲ�����|���ɼ�|   private    |   private   |���ɼ�|  protected | protected  | ���ɼ�| protected | public |
//--------------------------------------------------------------------------------------------------------
//�������|
//�ⲿ����|���ɼ�|���ɼ�|���ɼ�|���ɼ�|���ɼ�|���ɼ�|���ɼ�|���ɼ�|�ɼ�
//--------------------------------------------------------------------------------------------------------


//================================================
//Case3:��Ķ�̬��
//C++�еĺ�������Ĭ�ϲ�ʹ�ö�̬�󶨣�����������������:
// 1.ֻ��ָ��Ϊ�麯���ĳ�Ա�������ܽ��ж�̬�󶨣�
//        ��Ա����Ĭ��Ϊ���麯���������ж�̬��
// 2.���뾭���������͵����û�ָ����к�������

class MyBase
{
public:
    virtual void disp1()
    {
        printf("hello, Mybase1\n");
    }
    void disp2()
    {
        printf("hello, Mybase2\n");
    }
};

class MyChild : public MyBase
{
public:
    void disp1()
    {
        printf("hello, Mychild1\n");
    }
    void disp2()
    {
        printf("hello, Mychild2\n");
    }
};

void CPPTest3(void)
{
    printf("%s\n", __FUNCTION__);
    MyBase* base = NULL;
    MyChild child;
    base = &child;

    //��virtualʱ����������ĸ��汾�ĺ���ȡ����ָ����ָ���������ͣ�
    // ������ָ�뱾�������
    base->disp1();

    //����virtualʱ����������ĸ��汾�ĺ���ȡ����ָ�뱾������ͣ�
    //��ָ����ָ����������޹�
    base->disp2();
}