#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

int c = 0;

class SpinLock
{
    atomic_flag flag = ATOMIC_FLAG_INIT;

    public:
    void lock()
    {
        while(flag.test_and_set());
    }

    void unLock()
    {
        flag.clear();
    }
};

SpinLock sl;

void workOnResurse()
{
    for (int a = 0; a < 1000000; a++)
    {
        sl.lock();
        c++;
        sl.unLock();
    }
}

int main()
{
    thread t1(workOnResurse);
    thread t2(workOnResurse);

    t1.join();
    t2.join();

    cout << c;

    return 0;
}