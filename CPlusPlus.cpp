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
//================================================
//Case2:类的继承
//--------------------------------------------------------------------------------------------------------
//基类成员|    private   |  protected  |   pubilc    |   private   |  protected  |     public   |   private    | protected | public |
//--------------------------------------------------------------------------------------------------------
//继承方式|                      private                     |                       protected                 |                    public                   |
//--------------------------------------------------------------------------------------------------------
//子类           |
//内部访问|不可见|   private    |   private   |不可见|  protected | protected  | 不可见| protected | public |
//--------------------------------------------------------------------------------------------------------
//子类对象|
//外部访问|不可见|不可见|不可见|不可见|不可见|不可见|不可见|不可见|可见
//--------------------------------------------------------------------------------------------------------


//================================================
//Case3:类的动态绑定
//C++中的函数调用默认不使用动态绑定，除非满足两个条件:
// 1.只有指定为虚函数的成员函数才能进行动态绑定，
//        成员函数默认为非虚函数，不进行动态绑定
// 2.必须经过基类类型的引用或指针进行函数调用

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

    //加virtual时，具体调用哪个版本的函数取决于指针所指向对象的类型，
    // 而不是指针本身的类型
    base->disp1();

    //不加virtual时，具体调用哪个版本的函数取决于指针本身的类型，
    //和指针所指对象的类型无关
    base->disp2();
}