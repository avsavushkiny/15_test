#include <windows.h> //https://clck.ru/3Vadr6
#include <stdio.h>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Создаём процесс
    if (!CreateProcess(
            NULL,                                 // Имя модуля (NULL = берем из командной строки)
            "C:\\Windows\\System32\\notepad.exe", // Командная строка
            NULL,                                 // Атрибуты безопасности процесса
            NULL,                                 // Атрибуты безопасности потока
            FALSE,                                // Наследование дескрипторов
            0,                                    // Флаги создания
            NULL,                                 // Переменные окружения
            NULL,                                 // Рабочая директория
            &si,                                  // STARTUPINFO
            &pi                                   // PROCESS_INFORMATION
            ))
    {
        printf("CreateProcess failed (%d)\n", GetLastError());
        return 1;
    }

    // Ждём завершения процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Закрываем дескрипторы
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
