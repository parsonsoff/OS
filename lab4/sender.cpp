#include <iostream>
#include <string>
#include <Windows.h>

int main() {
    // Создаем или открываем общий файл для записи
    HANDLE file = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, L"shared_file");
    if (file == NULL) {
        std::cerr << "Failed to create or open shared file." << std::endl;
        return 1;
    }

    // Создаем или открываем семафор
    HANDLE semaphore = CreateSemaphore(NULL, 0, 1, L"message_sem");
    if (semaphore == NULL) {
        std::cerr << "Failed to create or open semaphore." << std::endl;
        return 1;
    }

    while (true) {
        // Читаем сообщение с клавиатуры
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        // Записываем сообщение в файл
        std::wstring wideMessage(message.begin(), message.end());
        LPVOID messageBuffer = MapViewOfFile(file, FILE_MAP_WRITE, 0, 0, 0);
        std::copy(wideMessage.c_str(), wideMessage.c_str() + wideMessage.size() + 1, (WCHAR*)messageBuffer);
        UnmapViewOfFile(messageBuffer);

        // Устанавливаем значение семафора в 1
        ReleaseSemaphore(semaphore, 1, NULL);
    }

    // Закрываем файл и семафор
    CloseHandle(file);
    CloseHandle(semaphore);

    return 0;
}