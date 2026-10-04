// Victim.cpp
// MSVC:  cl /EHsc /Fe:Victim.exe Victim.cpp
// MinGW: g++ -O2 -o Victim.exe Victim.cpp

#include <windows.h>
#include <string>
#include <chrono>
#include <thread>

int main()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string secret = "SECRET: password = hunter2";

    while (true)
    {
        DWORD written = 0;
        WriteConsoleA(hOut, secret.c_str(), (DWORD)secret.size(), &written, nullptr);
        WriteConsoleA(hOut, "\r\n", 2, &written, nullptr);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}