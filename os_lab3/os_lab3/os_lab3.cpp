#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <csignal>

std::mutex mtx; // Мьютекс для синхронизации доступа к массиву

bool terminateFlag = false; // Флаг для обозначения получения сигнала на завершение работы

// Обработчик сигнала на завершение работы
void signalHandler(int signum) {
    terminateFlag = true;
}

// Функция потока marker
void markerThread(int threadNum, std::vector<int>& markerArray, int arraySize, std::atomic<bool>& markerDone, std::atomic<int>& markedCount)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, arraySize - 1);

    while (!markerDone && !terminateFlag)
    {
        int randomNum = dis(gen);

        std::lock_guard<std::mutex> lock(mtx);
        if (markerArray[randomNum] == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            markerArray[randomNum] = threadNum;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            markedCount++;
        }
        else
        {
            std::cout << "Thread " << threadNum << ": marked_count=" << markedCount << ", index=" << randomNum << std::endl;
            markerDone = true;
        }
    }

    std::cout << "Thread " << threadNum << " finished" << std::endl;
}

int main()
{
    int arraySize;
    std::cout << "Enter the size of the array: ";
    std::cin >> arraySize;

    std::vector<int> markerArray(arraySize, 0);

    int numThreads;
    std::cout << "Enter the number of marker threads: ";
    std::cin >> numThreads;

    std::vector<std::thread> markerThreads;
    std::atomic<bool> markerDone(false);
    std::atomic<int> markedCount(0);

    for (int i = 0; i < numThreads; i++)
    {
        markerThreads.emplace_back(markerThread, i + 1, std::ref(markerArray), arraySize, std::ref(markerDone), std::ref(markedCount));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Даем время потокам marker инициализироваться

    std::signal(SIGINT, signalHandler); // Установка обработчика сигнала на завершение работы

    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        markerDone = false;
        markedCount = 0;
        lock.unlock();

        std::cout << "Array: ";
        for (int num : markerArray)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        int threadNum;
        std::cout << "Enter the thread number to terminate (or enter 0 to exit): ";
        std::cin >> threadNum;

        if (threadNum == 0) {
            break;
        }

        lock.lock();
        markerDone = true;
        lock.unlock();

        markerThreads[threadNum - 1].join();

        std::cout << "Array: ";
        for (int num : markerArray)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        lock.lock();
        if (std::all_of(markerThreads.begin(), markerThreads.end(), [](const std::thread& t) { return !t.joinable(); }))
        {
            break;
        }
        lock.unlock();
    }

    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < arraySize; i++) {
        if (markerArray[i] != 0) {
            markerArray[i] = 0;
        }
    }

    std::cout << "Main thread finished" << std::endl;

    return 0;
}