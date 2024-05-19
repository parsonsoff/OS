#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

struct employee {
    int num;
    char name[10];
    double hours;
};

void createFile(const std::string& filename, const std::vector<employee>& employees) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file." << std::endl;
        return;
    }

    for (const auto& emp : employees) {
        file.write(reinterpret_cast<const char*>(&emp), sizeof(emp));
    }

    file.close();
}

void printFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    employee emp;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        std::cout << "Num: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << std::endl;
    }

    file.close();
}

int main() {
    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;

    std::vector<employee> employees;
    int numEmployees;
    std::cout << "Enter number of employees: ";
    std::cin >> numEmployees;

    for (int i = 0; i < numEmployees; ++i) {
        employee emp;
        std::cout << "Enter employee num: ";
        std::cin >> emp.num;
        std::cout << "Enter employee name: ";
        std::cin >> emp.name;
        std::cout << "Enter employee hours: ";
        std::cin >> emp.hours;

        employees.push_back(emp);
    }

    createFile(filename, employees);

    std::cout << "Created file:" << std::endl;
    printFile(filename);

    int numClients;
    std::cout << "Enter number of clients: ";
    std::cin >> numClients;

    std::vector<HANDLE> clientHandles;

    for (int i = 0; i < numClients; ++i) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::wstring wcommand = L"Client.exe " + std::wstring(filename.begin(), filename.end());
        wchar_t* command = &wcommand[0];

        if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            std::cerr << "Failed to start client process." << std::endl;
            return 1;
        }
        else {
            clientHandles.push_back(pi.hProcess);
        }
    }

    WaitForMultipleObjects(clientHandles.size(), clientHandles.data(), TRUE, INFINITE);

    for (const auto& handle : clientHandles) {
        CloseHandle(handle);
    }

    std::cout << "Modified file:" << std::endl;
    printFile(filename);

    return 0;
}