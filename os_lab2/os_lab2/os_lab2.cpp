#include <iostream>
#include <windows.h>
#include <process.h>

#define THREAD_COUNT 3

int* arr;
int size;

HANDLE hThreads[THREAD_COUNT];
DWORD threadIDs[THREAD_COUNT];

CRITICAL_SECTION cs;

void min_max(void* param) {
    int min = arr[0];
    int max = arr[0];

    for (int i = 1; i < size; i++) {
        Sleep(7);
        if (arr[i] < min) {
            min = arr[i];
        }

        if (arr[i] > max) {
            max = arr[i];
        }
    }

    EnterCriticalSection(&cs);
    std::cout << "Минимальный элемент: " << min << std::endl;
    std::cout << "Максимальный элемент: " << max << std::endl;
    LeaveCriticalSection(&cs);
}

void average(void* param) {
    int sum = 0;

    for (int i = 0; i < size; i++) {
        Sleep(12);
        sum += arr[i];
    }

    float avg = (float)sum / size;

    EnterCriticalSection(&cs);
    std::cout << "Среднее значение: " << avg << std::endl;
    LeaveCriticalSection(&cs);
}

int main() {
    setlocale(LC_ALL, "Russian");

    InitializeCriticalSection(&cs);

    std::cout << "Введите размер массива: ";
    std::cin >> size;

    arr = new int[size];

    std::cout << "Введите элементы массива: ";
    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }

    hThreads[0] = (HANDLE)_beginthread(min_max, 0, NULL);
    hThreads[1] = (HANDLE)_beginthread(average, 0, NULL);

    WaitForMultipleObjects(THREAD_COUNT - 1, hThreads, TRUE, INFINITE);

    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }

    float avg = sum / size;

    int min = arr[0];
    int max = arr[0];

    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }

        if (arr[i] > max) {
            max = arr[i];
        }
    }

    for (int i = 0; i < size; i++) {
        if (arr[i] == min || arr[i] == max) {
            arr[i] = avg;
        }
    }

    std::cout << "Массив после замены минимального и максимального элементов: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    DeleteCriticalSection(&cs);
    delete[] arr;

    return 0;
}