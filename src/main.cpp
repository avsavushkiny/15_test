#include "calculator.h"
#include <iostream>

int main()
{
    std::cout << "=== C++ Simple Calculator ===" << std::endl;

    // Демонстрация сложения
    int num1 = 10;
    int num2 = 20;
    int sum = add(num1, num2);

    std::cout << "Sum of " << num1 << " + " << num2 << " = " << sum << std::endl;

    // Демонстрация вывода текста
    printMessage("Hello from C++ program!");

    std::cout << "=== Program finished ===" << std::endl;
    return 0;
}

// g++ -std=c++17 -o main.exe main.cpp calculator.cpp