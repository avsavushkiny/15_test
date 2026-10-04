#pragma once

#ifdef MYLIB_EXPORTS
#define MYLIB_API __declspec(dllexport)
#else
#define MYLIB_API __declspec(dllimport)
#endif



extern "C"
{
    MYLIB_API int Sum(int a, int b);
    MYLIB_API int Subtraction( int a, int b);
    MYLIB_API int Division(int a, int b);
    MYLIB_API int Multiplication(int a, int b); 
}
