#include <iostream>
#include <string>
#include <Windows.h>

int main() {
    // Открываем общий файл для чтения
    HANDLE file = OpenFileMapping(FILE_MAP_READ, FALSE, L"shared_file");
    if (file == NULL) {
        std::cerr << "Failed to open shared file." << std::endl;
        return 1;
    }

    // Открываем семафор
    HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"message_sem");
    if (semaphore == NULL) {
        std::cerr << "Failed to open semaphore." << std::endl;
        return 1;
    }

    while (true) {
        // Ждем, пока семафор не станет равным 1
        WaitForSingleObject(semaphore, INFINITE);

        // Читаем сообщение из файла
        LPCTSTR message = (LPCTSTR)MapViewOfFile(file, FILE_MAP_READ, 0, 0, 0);
        std::wstring wideMessage(message);
        int size = WideCharToMultiByte(CP_UTF8, 0, wideMessage.c_str(), -1, NULL, 0, NULL, NULL);
        std::string messageStr(size, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wideMessage.c_str(), -1, &messageStr[0], size, NULL, NULL);
        UnmapViewOfFile(message);

        // Выводим сообщение на экран
        std::cout << "Received message: " << messageStr << std::endl;

        // Устанавливаем значение семафора обратно в 0
        ReleaseSemaphore(semaphore, 1, NULL);
    }

    // Закрываем файл и семафор
    CloseHandle(file);
    CloseHandle(semaphore);

    return 0;
}