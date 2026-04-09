#include "calculator.h"
#include <iostream>

int main() {
    std::cout << "=== C++ Simple Calculator ===" << std::endl;
    
    // Демонстрация сложения
    int num1 = 10;
    int num2 = 20;
    int sum = add(num1, num2);
    
    std::cout << "Sum of " << num1 << " + " << num2 << " = " << sum << std::endl;
    
    // Демонстрация вывода текста
    printMessage("Hello from C++ program!");
    
    // Демонстрация с проверкой
    try {
        int safeSum = addWithCheck(100, 200);
        std::cout << "Safe sum: " << safeSum << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    std::cout << "=== Program finished ===" << std::endl;
    return 0;
}