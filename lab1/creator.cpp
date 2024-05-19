#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: creator <filename> <num_records>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    int numRecords = std::stoi(argv[2]);

    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
        std::cout << "Error creating binary file." << std::endl;
        return 1;
    }

    struct Employee {
        int num;
        char name[10];
        double hours;
    };

    for (int i = 0; i < numRecords; i++) {
        Employee emp;

        std::cout << "Enter employee number: ";
        std::cin >> emp.num;
        std::cout << "Enter employee name: ";
        std::cin >> emp.name;
        std::cout << "Enter number of hours worked: ";
        std::cin >> emp.hours;

        file.write(reinterpret_cast<const char*>(&emp), sizeof(emp));
    }

    file.close();

    return 0;
}