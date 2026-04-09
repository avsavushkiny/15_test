#include "../src/calculator.h"
#include <iostream>
#include <cassert>

void testAdd() {
    std::cout << "Running testAdd... ";
    assert(add(2, 3) == 5);
    assert(add(-1, 1) == 0);
    assert(add(0, 0) == 0);
    assert(add(-5, -3) == -8);
    std::cout << "PASSED" << std::endl;
}

void testAddWithCheck() {
    std::cout << "Running testAddWithCheck... ";
    assert(addWithCheck(10, 20) == 30);
    assert(addWithCheck(-10, -20) == -30);
    
    // Тест на переполнение
    try {
        addWithCheck(INT_MAX, 1);
        assert(false); // Должно быть исключение
    } catch (const std::overflow_error&) {
        // Ожидаемое исключение
    }
    
    try {
        addWithCheck(INT_MIN, -1);
        assert(false); // Должно быть исключение
    } catch (const std::overflow_error&) {
        // Ожидаемое исключение
    }
    std::cout << "PASSED" << std::endl;
}

void testPrintMessage() {
    std::cout << "Running testPrintMessage... ";
    // Просто проверяем, что функция не выбрасывает исключений
    try {
        printMessage("Test message");
        std::cout << "PASSED" << std::endl;
    } catch (...) {
        std::cout << "FAILED" << std::endl;
        assert(false);
    }
}

int main() {
    std::cout << "Running tests..." << std::endl;
    testAdd();
    testAddWithCheck();
    testPrintMessage();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}