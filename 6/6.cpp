#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <semaphore>
#include <mutex>

std::mutex io_mtx;                 // защита std::cout
std::counting_semaphore<4> sem(4); // максимум 4 одновременно

void task(int id)
{
    sem.acquire(); // ждём разрешение
    {
        std::lock_guard<std::mutex> lock(io_mtx);
        std::cout << "Задача " << id << " стартовала\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    {
        std::lock_guard<std::mutex> lock(io_mtx);
        std::cout << "Задача " << id << " завершена\n";
    }
    sem.release(); // освобождаем разрешение
}

int main()
{
    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < 20; ++i)
        threads.emplace_back(task, i);
   
        for (auto &t : threads)
        t.join();

    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << "Общее время: " << ms << " мс\n"; // ~1000 мс
}
