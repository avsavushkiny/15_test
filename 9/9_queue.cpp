#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

std::queue<int> buffer;
std::mutex mtx;


void producer()
{
    for (int i = 1; i <= 5; ++i)
    {
        mtx.lock();
        buffer.push(i);
        cout << "PUSH " << i << endl;
        mtx.unlock();
    }
}

void consumer()
{
    for (int i = 0; i < 5;)
    {
        mtx.lock();

        if (!buffer.empty())
        {
            int value = buffer.front(); // Берем данные
            buffer.pop();
            cout << "FRONT " << value << endl;
            i++;
            mtx.unlock();
        }
        else
        {
            mtx.unlock();
        }
    }
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();
    return 0;
}
