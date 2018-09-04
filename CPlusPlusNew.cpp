#include "CPlusPlusNew.h"

//================================================
//Case1: C++11/14������

//1.����ʱǿ��
//1.1����Ҫʹ�� NULL ʱ�򣬸ĳ�nullptr
void foo1_1(char *ch) 
{    
    printf("call foo(char*)\n");
}

void foo1_1(int i) 
{    
    printf("call foo(int)\n");
}

int CPP_NewTest1_1(void) 
{    
    if(NULL == (void *)0) 
        printf("NULL == 0\n");    
    else 
        printf("NULL != 0\n");    
    foo1_1(0);    
    //foo(NULL); // �����޷�ͨ��    
    foo1_1(nullptr);    
    return 0;
}

//--------------------------
//2.����ʱǿ��


//--------------------------
//3.��������
//3.1 std::array
void foo3_1(int *p, int len)
{
    for(int i=0; i<len; i++)
    {
        printf("%d\t", *(p+i));
    }
    printf("\n");
}

int CPP_NewTest3_1(void) 
{
    std::array<int, 5> arr = {2,4,3,1,5};

    //C���ӿ�ת��
    foo3_1(arr.data(), arr.size());
    foo3_1(&arr[0], arr.size());

    std::sort(arr.begin(), arr.end());
    printf("call std::sort\n");
    foo3_1(arr.data(), arr.size());

    for(auto &i : arr)
        printf("%d\t", i);
    printf("\n");
}

//3.2 std::unordered_map
int CPP_NewTest3_2(void) 
{
    // ����ṹ��ͬ����˳���ʼ��
    std::unordered_map<int, std::string> u = {
        {1, "1"},
        {3, "3"},
        {2, "2"}
    };
    std::map<int, std::string> v = {
        {1, "1"},
        {3, "3"},
        {2, "2"}
    };

    // �ֱ������ṹ���б���
    std::cout << "std::unordered_map" << std::endl;
    for( const auto & n : u)
        std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
    
    std::cout << std::endl;
    std::cout << "std::map" << std::endl;
    for( const auto & n : v)
        std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
}

//3.3  std:tuple
// �������ͱ��ƶ�Ϊ std::tuple<double, char, std::string>
auto get_student(int id)
{
    if (id == 0)
        return std::make_tuple(3.8, 'A', "ZhangSan");
    if (id == 1)
        return std::make_tuple(2.9, 'C', "LiSi");
    if (id == 2)
        return std::make_tuple(1.7, 'D', "WangWu");

    return std::make_tuple(0.0, 'D', "null");
}

int CPP_NewTest3_3(void)
{
    auto student = get_student(0);
    std::cout << "ID: 0, "
    << "GPA: " << std::get<0>(student) << ", "
    << "Score: " << std::get<1>(student) << ", "
    << "Name: " << std::get<2>(student) << '\n';
    
    double gpa;
    char grade;
    std::string name;
    
    // Ԫ����в��
    std::tie(gpa, grade, name) = get_student(1);
    std::cout << "ID: 1, "
    << "GPA: " << gpa << ", "
    << "Score: " << grade << ", "
    << "Name: " << name << '\n';
    
    //std:get����ʹ�ó�����ȡԪ������⣬C++14������ʹ����������ȡԪ���еĶ���
    std::tuple<std::string, double, double, int> t("abc", 4.5, 6.7, 8);
    std::cout << std::get<std::string>(t) << std::endl;
    // std::cout << std::get<double>(t) << std::endl;   // �Ƿ��������ڴ���
    std::cout << std::get<3>(t) << std::endl;
}

//--------------------------
//4.����ָ������ü���
//4.1 std:shared_ptr
//std::shared_ptr��һ��ǿָ��
void foo4_1(std::shared_ptr<int> i)
{    
    (*i)++;
}

int CPP_NewTest4_1(void)
{
    // auto pointer = new int(10); // �Ƿ�, ������ֱ�Ӹ�ֵ
    // ������һ�� std::shared_ptr
    auto pointer = std::make_shared<int>(10);
    auto pointer2 = pointer;    // ���ü���+1
    auto pointer3 = pointer;    // ���ü���+1

    foo4_1(pointer);
    std::cout << *pointer << std::endl; // 11
    int *p = pointer.get();             // ���������������ü���
    std::cout << *p << std::endl; // 11

    std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;
    std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;
    std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;

    pointer2.reset();
    std::cout << "reset pointer2:" << std::endl;
    std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;
    std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;
    std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;
    
    pointer3.reset();
    std::cout << "reset pointer3:" << std::endl;
    std::cout << "pointer.use_count() = " << pointer.use_count() << std::endl;
    std::cout << "pointer2.use_count() = " << pointer2.use_count() << std::endl;
    std::cout << "pointer3.use_count() = " << pointer3.use_count() << std::endl;

    // std::cout << *pointer << std::endl; // ���ü���Ϊ0ʱ, �Ƿ�����
    // �뿪������ǰ��pointer �ᱻ���������ü�����Ϊ0, �Ӷ��ͷ��ڴ�
    return 0;
}

//4.2 std:unique_ptr
//std::unique_ptr��һ�ֶ�ռ������ָ�룬����ֹ��������ָ�����乲��ͬһ�����󣬱�֤���밲ȫ
struct Foo{
    Foo()      { std::cout << "Foo::Foo" << std::endl;  }
    ~Foo()     { std::cout << "Foo::~Foo" << std::endl; }
    void foo() { std::cout << "Foo::foo" << std::endl;  }
};

void f(const Foo &)
{
    std::cout << "f(const Foo&)" << std::endl;
}

int CPP_NewTest4_2(void)
{
    std::unique_ptr<Foo> p1(std::make_unique<Foo>());

    // p1 ����, ���
    if(p1)
        p1->foo();

    //��Ȼ�Ƕ�ռ�����ǿ�������std::move����ת�Ƹ�������unique_ptr
    std::unique_ptr<Foo> p2(std::move(p1));

    // p2���գ����
    f(*p2);

    // p2���գ����
    if(p2)
        p2->foo();

    // p1 Ϊ��, �����
    if(p1)
        p1->foo();

    p1 = std::move(p2);

    // p2 Ϊ��, �����
    if(p2)
        p2->foo();

    // p1 ����, ���
    if(p1)
        p1->foo();

    return 0;
}

//4.3 std:weak_ptr
//std::weak_ptr��һ����ָ��
//std::weak_ptr û�� * ������� -> ����������Բ��ܹ�����Դ���в���������Ψһ���þ������ڼ�� std::shared_ptr �Ƿ����
class A;
class B;

class A{
public:
    std::weak_ptr<B> pointer;
    //std::shared_ptr<B> pointer;
    //ǿ�����޷��ͷţ������ڴ�й¶����Ҫʹ��weak_ptr
    ~A()
    {
        std::cout << "A destroy" << std::endl;
    }
};

class B{
public:
    std::shared_ptr<A> pointer;
    ~B()
    {
        std::cout << "B destroy" << std::endl;
    }
};

int CPP_NewTest4_3(void)
{
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();

    a->pointer = b;
    b->pointer = a;
    return 0;
}

//--------------------------
//5.���Լ��߳�֧��
//5.1 std::thread
std::mutex mtx;
void block_area(void)
{
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "blcok area" << std::endl;

    //�ٽ���
}

int CPP_NewTest5_1(void)
{
    std::thread thd1(block_area);

    thd1.join();
    return 0;
}

//5.2 
// ������������ģ��
int CPP_NewTest5_2(void)
{
    // ����������
    std::queue<int> produced_nums;
    // ������
    std::mutex m;
    // ��������
    std::condition_variable cond_var;
    // ������־
    bool done = false;
    // ֪ͨ��־
    bool notified = false;
    
    // �������߳�
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // ����������
            std::unique_lock<std::mutex> lock(m);
            std::cout << "producing " << i << '\n';
            produced_nums.push(i);
            notified = true;
            // ֪ͨһ���߳�
            cond_var.notify_one();
        }
        done = true;
        cond_var.notify_one();
    });
    // �������߳�
    std::thread consumer([&]() {
        std::unique_lock<std::mutex> lock(m);
        while (!done) {
            while (!notified) {  // ѭ��������ٻ���
                cond_var.wait(lock);
            }
            while (!produced_nums.empty()) {
                std::cout << "consuming " << produced_nums.front() << '\n';
                produced_nums.pop();
            }
            notified = false;
        }
    });
    
    producer.join();
    consumer.join();
}

//5.3 std::future
void foo5_3() {
    std::cout << "hello world" << std::endl;
}

void task_future() {
    // ��һ������ֵΪ7�� lambda ���ʽ��װ�� task ��
    // std::packaged_task ��ģ�����ΪҪ��װ����������
    std::packaged_task<int()> task([](){return 7;});
    // ��� task �� future
    std::future<int> result = task.get_future();    // ��һ���߳���ִ�� task
    std::thread(std::move(task)).detach();    
    std::cout << "Waiting...";
    result.wait();
    // ���ִ�н��
    std::cout << "Done!" << std:: endl << "Result is " << result.get() << '\n';
}

int CPP_NewTest5_3(void) 
{
    std::thread t(foo5_3);
    
    task_future();
    t.join();
    return 0;
}

//--------------------------
//6. Misc
//6.1ԭʼ�ַ���������
int CPP_NewTest6_1(void)
{
    std::string str = R"(C:\\What\\The\\Fxxk)";
    std::cout << str << std::endl;
    return 0;
}