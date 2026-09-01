#include <windows.h>
#include <iostream>

int main()
{
    // 1. Создаём именованный канал
    HANDLE hPipe = CreateNamedPipe(
        "\\\\.\\pipe\\HelloPipe",      // Имя канала (уникальное)
        PIPE_ACCESS_INBOUND,           // Только чтение (сервер читает)
        PIPE_TYPE_MESSAGE | PIPE_WAIT, // Режим сообщений
        1,                             // Только 1 клиент
        0, 1024,                       // Размеры буферов
        0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        // std::cout << "Ошибка создания канала: " << GetLastError() << std::endl;
        return 1;
    }

    // std::cout << "Сервер ожидает подключения..." << std::endl;

    // 2. Ждём подключения клиента
    if (!ConnectNamedPipe(hPipe, NULL))
    {
        // std::cout << "Ошибка подключения: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return 1;
    }

    // std::cout << "Клиент подключился!" << std::endl;

    // 3. Читаем сообщение
    char buffer[256] = {0};
    DWORD bytesRead;

    if (ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL))
    {
        std::cout << "message received: " << buffer << std::endl;
        std::cout << "number of bytes: " << bytesRead << std::endl;
    }
    else
    {
        // std::cout << "Ошибка чтения: " << GetLastError() << std::endl;
    }

    // 4. Закрываем канал
    CloseHandle(hPipe);
    // std::cout << "Сервер завершил работу" << std::endl;
    std::cin.get();

    return 0;
}