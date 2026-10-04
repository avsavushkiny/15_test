#include <iostream>
#include <thread>
#include <mutex>
#include <math.h>
#include <chrono>
#include <cstdlib>
#include <ctime>

int arr[10000000];

void randArr()
{
    for(int i = 0; i < 10000000; i++)
    {
        arr[i] = rand() % 1000;
    }
}

void func(int a, int b)
{
    
}

int main()
{
    randArr();

    std::thread t1(func, 0, 100000);

    for(int i = 0; i < 10; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

    return 0;
}