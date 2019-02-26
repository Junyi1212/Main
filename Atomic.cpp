#include "Atomic.h"

//Case1: atomic_flag test_and_set()
std::atomic<bool> ready (false);// can be checked without being set
std::atomic_flag winner1 = ATOMIC_FLAG_INIT;// always set when checked  //初始化为false
std::atomic<bool> winner2 (false);

void count1m (int id)
{
    // wait for the ready signal
    while (!ready)
    {
        std::this_thread::yield();
        //当前线程放弃执行，操作系统调度另一线程继续执行
    }
    for (volatile int i=0; i<1000000; ++i)
    {
    }

    // 2个都行
#if 0
    //设置winner1为true，并且返回是否在调用函数之前winner1已经设置为true
    if (!winner1.test_and_set())
    {
        std::cout << "thread #" << id << " won!\n";
    }
#else
    //用true替换winner2的值，并返回它之前的值
    if (!winner2.exchange(true))
    {
        std::cout << "thread #" << id << " won!\n";
    }
#endif
};

void AtomicCase1(void)
{
    std::cout << __FUNCTION__ << std::endl;

    std::vector<std::thread> threads;
    std::cout << "spawning 10 threads that count to 1 million...\n";
    for (int i=1; i<=10; ++i)
		threads.push_back(std::thread(count1m,i));
    ready = true;
    for (auto& th : threads) 
		th.join();

    return;
}

//Case2: atomic_flag clear()
std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;//初始化为false
std::stringstream stream;

void append_number(int x)
{
    //设置lock_stream为true，并且返回是否在调用函数之前lock_stream已经设置为true
    while (lock_stream.test_and_set()) 
    {}
    stream << "thread #" << x << '\n';

    //设置lock_stream为false，使得下次调用test_and_set返回false
    lock_stream.clear();
}

void AtomicCase2(void)
{
    std::cout << __FUNCTION__ << std::endl;

    std::vector<std::thread> threads;
    for (int i=1; i<=10; ++i)
    {
        threads.push_back(std::thread(append_number,i));
    }
    for (auto& th : threads)
    {
        th.join();
    }

    std::cout << stream.str();

    return;
}

//Case3:
std::atomic<int> foo (0);

void set_foo(int x)
{
    foo.store(x,std::memory_order_relaxed);     // set value atomically
}

void print_foo()
{
    int x;
    do
    {
      x = foo.load(std::memory_order_relaxed);  // get value atomically
    }while (x==0);
    std::cout << "foo: " << x << '\n';
}
  
void AtomicCase3(void)
{
    std::cout << __FUNCTION__ << std::endl;
    std::thread first (print_foo);
    std::thread second (set_foo,10);
    first.join();
    second.join();
    return;
}

//Case4:
std::atomic<long long> data;
void do_work()
{
    data.fetch_add(1, std::memory_order_relaxed);
}

void AtomicCase4(void)
{
    std::cout << __FUNCTION__ << std::endl;

    std::thread th1(do_work);
    std::thread th2(do_work);
    std::thread th3(do_work);
    std::thread th4(do_work);
    std::thread th5(do_work);
 
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
 
    std::cout << "Result:" << data << '\n';
    return;
}

//=========================================================================//
//六种原子操作内存顺序参考
//https://blog.csdn.net/what951006/article/details/78273903
//https://www.cnblogs.com/zifeiye/p/8194949.html

//Case5: relaxed的内存序(松弛的内存序)
std::atomic<bool> x5,y5;
std::atomic<int> z5;
void write_x_then_y_5()
{
    x5.store(true,std::memory_order_relaxed);  // 1
    y5.store(true,std::memory_order_relaxed);  // 2
}
void read_y_then_x_5()
{
    while(!y5.load(std::memory_order_relaxed));  // 3
    if(x5.load(std::memory_order_relaxed))  // 4
    ++z5;
}

void AtomicCase5_1(void)
{
    int z = 0;
    std::cout << __FUNCTION__ << std::endl;
    x5=false;
    y5=false;
    z5=0;
    std::thread a(write_x_then_y_5);
    std::thread b(read_y_then_x_5);
    a.join();
    b.join();

    z = z5.load();
    std::cout << "The Result z: " << z<< std::endl;  // 5
    assert(z!=0);                        //断言发生，z是可能等于0的
//1先于2（同一个线程保证原子执行顺序）
//但是在不同线程间的执行顺序是没有约束的，所以#4也有可能是false , z为0

    return;
}

std::atomic<int> num_a{0};
std::atomic<int> num_b{0};
void SetValue()
{// atomic类模板中的函数都是原子操作.  int temp = a.load();相当于 int temp = a的原子操作
     int t = 1;
     num_a.store(t, std::memory_order_relaxed); // 相当于 a = t的原子操作
     num_b.store(2, std::memory_order_relaxed); // 相当于 b = 2的原子操作
}
void Observer()
{
     std::cout << num_a << num_b << std::endl;
}

void AtomicCase5_2(void)
{
    int z = 0;
    std::cout << __FUNCTION__ << std::endl;

    std::thread T1(SetValue);
    std::thread T2(Observer);
 
    T1.join(); // 主线程(调用方)等待子线程 T1 执行完成,才能继续执行,阻塞
    T2.join(); // 同上,执行这一行前: T1已经结束,T2很可能也结束了
}

void AtomicCase5(void)
{
    int z = 0;
    std::cout << __FUNCTION__ << std::endl;
    AtomicCase5_1();
    AtomicCase5_2();
}

//Case6: memory_order_release，memory_order_acquire的内存序
std::atomic<bool> x6,y6;
std::atomic<int> z6;
void write_x_6()
{
     x6.store(true,std::memory_order_release);
}
void write_y_6()
{
     y6.store(true,std::memory_order_release);
}
void read_x_then_y_6()
{
     while(!x6.load(std::memory_order_acquire));
     if(y6.load(std::memory_order_acquire))//y为false;
        ++z6;
}
void read_y_then_x_6()
{
     while(!y6.load(std::memory_order_acquire));
     if(x6.load(std::memory_order_acquire))//x为false;
        ++z6;
}
void AtomicCase6(void)
{
    std::cout << __FUNCTION__ << std::endl;

    int z  = 0;
     x6=false;
     y6=false;
     z6=0;

     std::thread a(write_x_6);
     std::thread b(write_y_6);
     std::thread c(read_x_then_y_6);
     std::thread d(read_y_then_x_6);

     a.join();
     b.join();
     c.join();
     d.join();

    z = z6.load();
    std::cout << "The Result z: " << z << std::endl;  // 5
     assert(z!=0);
}

//Case7: memory_order_release，memory_order_consume的内存序
struct X7
{
     int i;
     std::string s;
};
std::atomic<X7*> p;
std::atomic<int> a;
void create_x()
{
     X7* x=new X7;
     x->i=42;
     x->s="hello";
     a.store(99,std::memory_order_relaxed);  //   1
     p.store(x,std::memory_order_release);
}
void use_x()
{
     X7* x;
     while(!(x=p.load(std::memory_order_consume))) // 2

     assert(x->i==42);
     assert(x->s=="hello");
     assert(a.load(std::memory_order_relaxed)==99); //3
}

void AtomicCase7(void)
{
    std::cout << __FUNCTION__ << std::endl;
     std::thread t1(create_x);
     std::thread t2(use_x);
     t1.join();
     t2.join();
}
