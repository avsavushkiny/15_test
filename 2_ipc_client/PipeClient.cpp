#include <windows.h>
#include <iostream>

int main()
{
    // 1. Открываем существующий канал
    HANDLE hPipe = CreateFile(
        "\\\\.\\pipe\\HelloPipe", // То же имя, что у сервера
        GENERIC_WRITE,            // Только запись
        0, NULL,
        OPEN_EXISTING, // Канал должен уже существовать
        0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        // std::cout << "Ошибка открытия канала: " << GetLastError() << std::endl;
        // std::cout << "Убедитесь, что сервер запущен!" << std::endl;
        return 1;
    }

    // std::cout << "Подключено к серверу!" << std::endl;

    // 2. Отправляем сообщение
    const char *message = "Hello World";
    DWORD bytesWritten;

    if (WriteFile(hPipe, message, strlen(message) + 1, &bytesWritten, NULL))
    {
        // std::cout << "Отправлено: " << message << std::endl;
        // std::cout << "Байт отправлено: " << bytesWritten << std::endl;
    }
    else
    {
        // std::cout << "Ошибка записи: " << GetLastError() << std::endl;
    }

    // 3. Закрываем канал
    CloseHandle(hPipe);

    return 0;
}