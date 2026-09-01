//https://clck.ru/3VZAaw

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx1;
std::mutex mtx2;

void thread1()
{
    mtx1.lock();
    std::cout << "Thread 1 captured mtx1\n"; // захватили

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Thread 1 is waiting mtx2...\n"; // ждем
    mtx2.lock();

    std::cout << "Thread 1 is running\n"; // работает

    mtx2.unlock();
    mtx1.unlock();
}

void thread2()
{
    mtx2.lock();
    std::cout << "Thread 2 captured mtx2\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Thread 2 is waiting mtx1...\n";
    mtx1.lock();

    std::cout << "Thread 2 is running\n";

    mtx1.unlock();
    mtx2.unlock();
}

int main()
{
    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();

    return 0;
}
