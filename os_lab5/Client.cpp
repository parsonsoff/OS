#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

struct employee {
    int num;
    char name[10];
    double hours;
};

void modifyRecord(const std::string& filename, int recordNum) {
    std::wstring wFilename(filename.begin(), filename.end());
    LPCWSTR lpFilename = wFilename.c_str();

    HANDLE hFile = CreateFile(lpFilename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    OVERLAPPED overlapped;
    memset(&overlapped, 0, sizeof(overlapped));

    employee emp;
    DWORD bytesRead;
    DWORD bytesWritten;

    if (!ReadFile(hFile, &emp, sizeof(emp), &bytesRead, &overlapped)) {
        std::cerr << "Failed to read record." << std::endl;
        CloseHandle(hFile);
        return;
    }

    std::cout << "Current record: Num: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << std::endl;

    std::cout << "Enter new values:" << std::endl;
    std::cout << "Enter employee num: ";
    std::cin >> emp.num;
    std::cout << "Enter employee name: ";
    std::cin >> emp.name;
    std::cout << "Enter employee hours: ";
    std::cin >> emp.hours;

    if (!WriteFile(hFile, &emp, sizeof(emp), &bytesWritten, &overlapped)) {
        std::cerr << "Failed to write record." << std::endl;
        CloseHandle(hFile);
        return;
    }

    CloseHandle(hFile);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: Client.exe <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    int recordNum;
    std::cout << "Enter record number to modify: ";
    std::cin >> recordNum;

    modifyRecord(filename, recordNum);

    return 0;
}