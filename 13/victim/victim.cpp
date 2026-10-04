// Victim.cpp
// MSVC:  cl /EHsc /Fe:Victim.exe Victim.cpp
// MinGW: g++ -O2 -o Victim.exe Victim.cpp

#include <windows.h>
#include <string>
#include <chrono>
#include <thread>

int main()
{
    const std::string secret = "SECRET: password = hunter2";

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return 1;

    while (true)
    {
        DWORD written = 0;
        WriteConsoleA(hOut, secret.c_str(), (DWORD)secret.size(), &written, nullptr);
        WriteConsoleA(hOut, "\r\n", 2, &written, nullptr);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}