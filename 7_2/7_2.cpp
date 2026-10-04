#include <algorithm>
#include <execution>
#include <vector>
#include <iostream>
#include <numeric>

int main() 
{
    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 0); // 0, 1, 2, ..., 99

    std::for_each(std::execution::par, data.begin(), data.end(), [](int &n) {
        n *= 2; // Параллельная операция
    });

    for (const auto &val : data) 
    {
        std::cout << val << " ";
    }
    return 0;
}
