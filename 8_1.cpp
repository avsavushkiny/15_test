#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> global_counter(0); // Атомарный счетчик

void increment()
{
    for (int i = 0; i < 1000; ++i)
    {
        global_counter++; // Потокобезопасно!
    }
}

int main()
{
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();

    std::cout << "Результат: " << global_counter.load() << std::endl; // Выведет 2000
    return 0;
}
