#define MYLIB_EXPORTS
#include "mylib.h"
#include <iostream>

using namespace std;

int Sum(int a, int b)
{
    return a + b;
}

int Division(int a, int b)
{
    if (a == 0 || b == 0)
    {
        cout << "You cannot divide by zero!" << endl; return 1;
    }
    
    return a / b;
}

int Subtraction( int a, int b)
{
    return a - b;
}

int Multiplication(int a, int b) 
{
    return a * b;
}