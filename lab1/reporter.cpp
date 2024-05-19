#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage: reporter <input_filename> <output_filename> <pay_rate>" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];
    double payRate = std::stod(argv[3]);

    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile) {
        std::cout << "Error opening input binary file." << std::endl;
        return 1;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile) {
        std::cout << "Error creating output text file." << std::endl;
        return 1;
    }

    outputFile << "Отчет по файлу \"" << inputFileName << "\"" << std::endl;
    outputFile << "Номер сотрудника, имя сотрудника, часы, зарплата" << std::endl;

    struct Employee {
        int num;
        char name[10];
        double hours;
    };

    Employee emp;
    while (inputFile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        double salary = emp.hours * payRate;
        outputFile << emp.num << ", " << emp.name << ", " << emp.hours << ", " << salary << std::endl;
    }

    inputFile.close();
    outputFile.close();

    return 0;
}