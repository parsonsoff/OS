#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

struct employee
{
    int num;
    char name[10];
    double hours;
};

void createFile(const string& fileName, int numRecords)
{
    ofstream file(fileName, ios::binary);
    if (!file)
    {
        cout << "Error creating file" << endl;
        return;
    }

    employee emp;
    for (int i = 0; i < numRecords; i++)
    {
        cout << "Enter details for employee " << i + 1 << ":" << endl;
        cout << "Number: ";
        cin >> emp.num;
        cout << "Name: ";
        cin >> emp.name;
        cout << "Hours: ";
        cin >> emp.hours;

        file.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    file.close();
}

void generateReport(const string& inputFileName, const string& outputFileName, double payRate)
{
    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile)
    {
        cout << "Error opening input file" << endl;
        return;
    }

    ofstream outputFile(outputFileName);
    if (!outputFile)
    {
        cout << "Error creating output file" << endl;
        return;
    }

    outputFile << "Отчет по файлу \"" << inputFileName << "\"" << endl;
    outputFile << "Номер сотрудника, имя сотрудника, часы, зарплата" << endl;

    employee emp;
    while (inputFile.read(reinterpret_cast<char*>(&emp), sizeof(employee)))
    {
        double salary = emp.hours * payRate;
        outputFile << emp.num << ", " << emp.name << ", " << emp.hours << ", " << salary << endl;
    }

    inputFile.close();
    outputFile.close();
}

int main()
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    string fileName;
    int numRecords;

    cout << "Enter the name of the binary file: ";
    cin >> fileName;
    cout << "Enter the number of records: ";
    cin >> numRecords;

    STARTUPINFOA startupInfo;
    ZeroMemory(&startupInfo, sizeof(STARTUPINFOA));
    startupInfo.cb = sizeof(STARTUPINFOA);

    PROCESS_INFORMATION processInfoCreator;
    string commandCreator = "Creator.exe " + fileName + " " + to_string(numRecords).c_str();
    if (!CreateProcessA(NULL, const_cast<char*>(commandCreator.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfoCreator))
    {
        cout << "Error creating Creator process" << endl;
        return 1;
    }

    WaitForSingleObject(processInfoCreator.hProcess, INFINITE);

    cout << "Contents of the binary file:" << endl;
    ifstream file(fileName, ios::binary);
    if (!file)
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    employee emp;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee)))
    {
        cout << "Number: " << emp.num << endl;
        cout << "Name: " << emp.name << endl;
        cout << "Hours: " << emp.hours << endl;
    }

    file.close();

    string outputFileName;
    double payRate;

    cout << "Enter the name of the report file: ";
    cin >> outputFileName;
    cout << "Enter the pay rate per hour: ";
    cin >> payRate;

    STARTUPINFOA startupInfoReporter;
    ZeroMemory(&startupInfoReporter, sizeof(STARTUPINFOA));
    startupInfoReporter.cb = sizeof(STARTUPINFOA);

    PROCESS_INFORMATION processInfoReporter;
    string commandReporter = "Reporter.exe " + fileName + " " + outputFileName + " " + to_string(payRate).c_str();
    if (!CreateProcessA(NULL, const_cast<char*>(commandReporter.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfoReporter, &processInfoReporter))
    {
        cout << "Error creating Reporter process" << endl;
        return 1;
    }

    WaitForSingleObject(processInfoReporter.hProcess, INFINITE);

    cout << "Report:" << endl;
    ifstream report(outputFileName);
    if (!report)
    {
        cout << "Error opening report file" << endl;
        return 1;
    }

    string line;
    while (getline(report, line))
    {
        cout << line << endl;
    }

    report.close();

    return 0;
}