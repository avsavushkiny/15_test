#include "calculator.h"
#include <iostream>

int add(int a, int b) {
    return a + b;
}

void printMessage(const std::string& message) {
    std::cout << "Message: " << message << std::endl;
}

int addWithCheck(int a, int b) {
    // Проверка на переполнение (простая версия)
    if (a > 0 && b > 0 && a > INT_MAX - b) {
        throw std::overflow_error("Positive overflow detected");
    }
    if (a < 0 && b < 0 && a < INT_MIN - b) {
        throw std::overflow_error("Negative overflow detected");
    }
    return a + b;
}