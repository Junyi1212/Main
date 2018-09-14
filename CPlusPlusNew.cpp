#include "CPlusPlusNew.h"

//================================================
//Case1: C++11/14新特性

//1.编译时强化
//1.1当需要使用 NULL 时候，改成nullptr
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
    printf("%s\n", __FUNCTION__);
    if(NULL == (void *)0) 
        printf("NULL == 0\n");    
    else 
        printf("NULL != 0\n");    
    foo1_1(0);    
    //foo(NULL); // 编译无法通过    
    foo1_1(nullptr);    
    return 0;
}
//1.2 constexpr
//编译时把表达式直接优化并植入到程序运行，能增加程序的性能
constexpr int len_foo()
{
    return 5;
}

constexpr int fibonacci(const int n)
{
    return n == 1 || n == 2? 1: fibonacci(n-1)+fibonacci(n-2);
}

#define LEN 10
int CPP_NewTest1_2(void)
{
    printf("%s\n", __FUNCTION__);
    char arr_1[10];//pass
    char arr_2[LEN];//pass

    const int len = 10;//在C++11之前len必须是const；在C 代码中非法
    char arr_3[len];//pass,

    char arr_4[len_foo()];//pass

    std::cout << fibonacci(10) << std::endl;
    // 1, 1, 2, 3, 5, 8, 13, 21, 34, 55
    return 0;
}
//1.3auto,decltype
//类型推导

//传统C++
template <typename R, typename T, typename U>
R add1(T x, U y)
{
    return x+y;
}

//尾返回类型C++11
template <typename T, typename U>
auto add2(T x, U y) -> decltype(x+y)
{
    return x+y;
}

//返回类型推导C++14
template <typename T, typename U>
auto add3(T x, U y)
{
    return x+y;
}

int CPP_NewTest1_3(void)
{
    printf("%s\n", __FUNCTION__);
    auto i = 5;
    int arr[10]= {0};
    auto auto_arr1 = arr;// 正确，对整个类型进行推导
    //auto auto_arr2[10] = arr;// 错误,  无法推导数组元素类型

    auto x = 1;
    auto y = 2;
    decltype(x+y) z1;
    //auto z2;// 错误, 无法推导

    std::cout << add1<int, int, int>(1,2) << std::endl;
    std::cout << add2<int, int>(1,2) << std::endl;
    std::cout << add3<int, int>(1,2) << std::endl;
    return 0;
}

//1.4 for
//区间迭代
int CPP_NewTest1_4(void)
{
    printf("%s\n", __FUNCTION__);
    int array[] = {1,2,3,4,5};

    // & 启用了引用, 如果没有则对 arr 中的元素只能读取不能修改
    for(auto &x : array)
    {
        x = 8;
        std::cout << x << std::endl;
    }
    std::cout <<  std::endl;
    for(auto &x : array)
    {
        std::cout << x << std::endl;
    }

    // 传统 C++ 写法
    std::vector<int> vec(5, 10);
    for(std::vector<int>::iterator i = vec.begin(); i != vec.end(); i++)
    {
        std::cout << *i << std::endl;
    }
    std::cout <<  std::endl;
    // C++11 写法
    for(auto &i : vec)
    {
         std::cout << i << std::endl;
    }
    return 0;
}


//--------------------------
//2.运行时强化


//--------------------------
//3.新增容器
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
    printf("%s\n", __FUNCTION__);
    std::array<int, 5> arr = {2,4,3,1,5};

    //C风格接口转换
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
    printf("%s\n", __FUNCTION__);
    // 两组结构按同样的顺序初始化
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

    // 分别对两组结构进行遍历
    std::cout << "std::unordered_map" << std::endl;
    for( const auto & n : u)
        std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
    
    std::cout << std::endl;
    std::cout << "std::map" << std::endl;
    for( const auto & n : v)
        std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
}

//3.3  std:tuple
// 返回类型被推断为 std::tuple<double, char, std::string>
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
    printf("%s\n", __FUNCTION__);
    auto student = get_student(0);
    std::cout << "ID: 0, "
    << "GPA: " << std::get<0>(student) << ", "
    << "Score: " << std::get<1>(student) << ", "
    << "Name: " << std::get<2>(student) << '\n';
    
    double gpa;
    char grade;
    std::string name;
    
    // 元组进行拆包
    std::tie(gpa, grade, name) = get_student(1);
    std::cout << "ID: 1, "
    << "GPA: " << gpa << ", "
    << "Score: " << grade << ", "
    << "Name: " << name << '\n';
    
    //std:get除了使用常量获取元祖对象外，C++14增加了使用类型来获取元祖中的对象
    std::tuple<std::string, double, double, int> t("abc", 4.5, 6.7, 8);
    std::cout << std::get<std::string>(t) << std::endl;
    // std::cout << std::get<double>(t) << std::endl;   // 非法，编译期错误
    std::cout << std::get<3>(t) << std::endl;
}

//--------------------------
//4.智能指针和引用计数
//4.1 std:shared_ptr
//std::shared_ptr是一种强指针
void foo4_1(std::shared_ptr<int> i)
{    
    (*i)++;
}

int CPP_NewTest4_1(void)
{
    printf("%s\n", __FUNCTION__);
    // auto pointer = new int(10); // 非法, 不允许直接赋值
    // 构造了一个 std::shared_ptr
    auto pointer = std::make_shared<int>(10);
    auto pointer2 = pointer;    // 引用计数+1
    auto pointer3 = pointer;    // 引用计数+1

    foo4_1(pointer);
    std::cout << *pointer << std::endl; // 11
    int *p = pointer.get();             // 这样不会增加引用计数
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

    // std::cout << *pointer << std::endl; // 引用计数为0时, 非法访问
    // 离开作用域前，pointer 会被析构，引用计数减为0, 从而释放内存
    return 0;
}

//4.2 std:unique_ptr
//std::unique_ptr是一种独占的智能指针，它禁止其他智能指针与其共享同一个对象，保证代码安全
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
    printf("%s\n", __FUNCTION__);
    std::unique_ptr<Foo> p1(std::make_unique<Foo>());

    // p1 不空, 输出
    if(p1)
        p1->foo();

    //虽然是独占，但是可以利用std::move将其转移给其他的unique_ptr
    std::unique_ptr<Foo> p2(std::move(p1));

    // p2不空，输出
    f(*p2);

    // p2不空，输出
    if(p2)
        p2->foo();

    // p1 为空, 无输出
    if(p1)
        p1->foo();

    p1 = std::move(p2);

    // p2 为空, 无输出
    if(p2)
        p2->foo();

    // p1 不空, 输出
    if(p1)
        p1->foo();

    return 0;
}

//4.3 std:weak_ptr
//std::weak_ptr是一种弱指针
//std::weak_ptr 没有 * 运算符和 -> 运算符，所以不能够对资源进行操作，它的唯一作用就是用于检查 std::shared_ptr 是否存在
class A;
class B;

class A{
public:
    std::weak_ptr<B> pointer;
    //std::shared_ptr<B> pointer;
    //强引用无法释放，导致内存泄露，需要使用weak_ptr
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
    printf("%s\n", __FUNCTION__);
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();

    a->pointer = b;
    b->pointer = a;
    return 0;
}

//--------------------------
//5.语言级线程支持
//5.1 std::thread
std::mutex mtx;
void block_area(void)
{
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "blcok area" << std::endl;

    //临界区
}

int CPP_NewTest5_1(void)
{
    printf("%s\n", __FUNCTION__);
    std::thread thd1(block_area);

    thd1.join();
    return 0;
}

//5.2 
// 生产者消费者模型
int CPP_NewTest5_2(void)
{
    printf("%s\n", __FUNCTION__);
    // 生产者数量
    std::queue<int> produced_nums;
    // 互斥锁
    std::mutex m;
    // 条件变量
    std::condition_variable cond_var;
    // 结束标志
    bool done = false;
    // 通知标志
    bool notified = false;
    
    // 生产者线程
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // 创建互斥锁
            std::unique_lock<std::mutex> lock(m);
            std::cout << "producing " << i << '\n';
            produced_nums.push(i);
            notified = true;
            // 通知一个线程
            cond_var.notify_one();
        }
        done = true;
        cond_var.notify_one();
    });
    // 消费者线程
    std::thread consumer([&]() {
        std::unique_lock<std::mutex> lock(m);
        while (!done) {
            while (!notified) {  // 循环避免虚假唤醒
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
    // 将一个返回值为7的 lambda 表达式封装到 task 中
    // std::packaged_task 的模板参数为要封装函数的类型
    std::packaged_task<int()> task([](){return 7;});
    // 获得 task 的 future
    std::future<int> result = task.get_future();    // 在一个线程中执行 task
    std::thread(std::move(task)).detach();    
    std::cout << "Waiting...";
    result.wait();
    // 输出执行结果
    std::cout << "Done!" << std:: endl << "Result is " << result.get() << '\n';
}

int CPP_NewTest5_3(void) 
{
    printf("%s\n", __FUNCTION__);
    std::thread t(foo5_3);
    
    task_future();
    t.join();
    return 0;
}

//--------------------------
//6. Misc
//6.1原始字符串字面量
int CPP_NewTest6_1(void)
{
    printf("%s\n", __FUNCTION__);
    std::string str = R"(C:\\What\\The\\Fxxk)";
    std::cout << str << std::endl;
    return 0;
}