#include <iostream>
#include <windows.h>

using namespace std;

typedef int (*SubFunc)(int, int);

int main()
{
    HMODULE hDLL = LoadLibrary (TEXT("mylib.dll"));

    if (hDLL == NULL)
    {
        cout << "DLL not found" << endl;
        return 1;
    }

    SubFunc Sub = (SubFunc)GetProcAddress(hDLL, "Subtraction");

    if (Sub == NULL)
    {
        cout << "Function Subtraction not found" << endl;
        FreeLibrary(hDLL);
        return 1;
    }

    int result = Sub(15, 27);
    cout << "Result SUB: " << result << endl;

    FreeLibrary(hDLL);

    return 0;
}
