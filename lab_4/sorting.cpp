#include "sorting.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <string>
#include <cstddef>
#include <limits>
#include <chrono>


namespace {

struct SortStats {
    size_t comparisons = 0;
    size_t swaps = 0;
    double time_ms = 0.0;
};

// Функция вывода массива
template<typename T>
void printArray(T* arr, size_t size) {
    std::cout << "[";
    size_t printSize = (size < 20) ? size : 20;
    for (size_t i = 0; i < printSize; ++i) {
        std::cout << std::setw(3) << arr[i];
        if (i < printSize - 1) std::cout << " ";
    }
    if (size > 20) std::cout << " ...";
    std::cout << "]";
}

// Сортировка выбором
template<typename T>
SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
    SortStats stats;
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < size - 1; ++i) {
        size_t extremeIndex = i;

        for (size_t j = i + 1; j < size; ++j) {
            stats.comparisons++;
            if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
                extremeIndex = j;
            }
        }

        if (extremeIndex != i) {
            std::swap(arr[i], arr[extremeIndex]);
            stats.swaps++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    return stats;
}

// Сортировка пузырьком
template<typename T>
SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
    SortStats stats;
    auto start = std::chrono::high_resolution_clock::now();
    bool swapped;

    for (size_t i = 0; i < size - 1; ++i) {
        swapped = false;

        for (size_t j = 0; j < size - i - 1; ++j) {
            stats.comparisons++;
            if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
                std::swap(arr[j], arr[j + 1]);
                stats.swaps++;
                swapped = true;
            }
        }

        if (!swapped) break;
    }

    auto end = std::chrono::high_resolution_clock::now();
    stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    return stats;
}

// Быстрая сортировка (дополнительное задание)
template<typename T>
size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
    T pivot = arr[high];
    size_t i = low;

    for (size_t j = low; j < high; ++j) {
        stats.comparisons++;
        if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
            if (i != j) {
                std::swap(arr[i], arr[j]);
                stats.swaps++;
            }
            i++;
        }
    }

    if (i != high) {
        std::swap(arr[i], arr[high]);
        stats.swaps++;
    }

    return i;
}

template<typename T>
void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
    if (low < high) {
        size_t pi = partition(arr, low, high, ascending, stats);

        if (pi > 0) {
            quickSortRecursive(arr, low, pi - 1, ascending, stats);
        }
        quickSortRecursive(arr, pi + 1, high, ascending, stats);
    }
}

template<typename T>
SortStats quickSort(T* arr, size_t size, bool ascending = true) {
    SortStats stats;
    if (size > 1) {
        auto start = std::chrono::high_resolution_clock::now();
        quickSortRecursive(arr, 0, size - 1, ascending, stats);
        auto end = std::chrono::high_resolution_clock::now();
        stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();
    }
    return stats;
}

// Заполнение массива случайными числами
void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> distribution(min, max);

    for (size_t i = 0; i < size; ++i) {
        arr[i] = distribution(engine);
    }
}

// Копирование массива
int* copyArray(const int* source, size_t size) {
    int* copy = new int[size];
    for (size_t i = 0; i < size; ++i) {
        copy[i] = source[i];
    }
    return copy;
}

// Копирование статического массива
template<size_t N>
void copyStaticArray(const int (&source)[N], int (&dest)[N]) {
    for (size_t i = 0; i < N; ++i) {
        dest[i] = source[i];
    }
}

// Явные инстанциации шаблонов для int
template void printArray<int>(int* arr, size_t size);
template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// Функция для проверки ввода числа
size_t getValidInput(const std::string& prompt) {
    size_t value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите целое число: ";
        } else if (value == 0) {
            std::cout << "Ошибка! Число должно быть больше 0. Попробуйте снова: ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

} // анонимное пространство имен

namespace AssemblingSorting {

// Функция для запроса продолжения
bool askToContinue() {
    char choice;
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:\n";
    std::cout << "1 - Выполнить тест с другим размером массива\n";
    std::cout << "0 - Завершить программу\n";
    std::cout << "Выбор: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return (choice == '1');
}

// Тестирование на статических массивах (5-10 элементов)
void testStaticArrays() {
    const size_t STATIC_SIZE = 10; // Фиксированный размер статического массива

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "ЧАСТЬ 1: ТЕСТИРОВАНИЕ НА СТАТИЧЕСКОМ МАССИВЕ (" << STATIC_SIZE << " элементов)\n";
    std::cout << std::string(70, '=') << "\n";

    // Создаем и заполняем исходный статический массив
    int originalArr[STATIC_SIZE];
    fillRandom(originalArr, STATIC_SIZE);

    std::cout << "\nИсходный массив: ";
    printArray(originalArr, STATIC_SIZE);
    std::cout << "\n\n";

    // Массивы для копий (для каждого алгоритма и каждой итерации)
    int arrSelection1[STATIC_SIZE], arrSelection2[STATIC_SIZE], arrSelection3[STATIC_SIZE];
    int arrBubble1[STATIC_SIZE], arrBubble2[STATIC_SIZE], arrBubble3[STATIC_SIZE];
    int arrQuick1[STATIC_SIZE], arrQuick2[STATIC_SIZE], arrQuick3[STATIC_SIZE];

    // Копируем исходный массив
    copyStaticArray(originalArr, arrSelection1);
    copyStaticArray(originalArr, arrSelection2);
    copyStaticArray(originalArr, arrSelection3);
    copyStaticArray(originalArr, arrBubble1);
    copyStaticArray(originalArr, arrBubble2);
    copyStaticArray(originalArr, arrBubble3);
    copyStaticArray(originalArr, arrQuick1);
    copyStaticArray(originalArr, arrQuick2);
    copyStaticArray(originalArr, arrQuick3);

    // Тестируем сортировку выбором (ОСНОВНОЕ ЗАДАНИЕ)
    std::cout << "АЛГОРИТМ: СОРТИРОВКА ВЫБОРОМ (основное задание)\n";
    std::cout << std::string(70, '-') << "\n";

    // 1. Исходный -> по возрастанию
    std::cout << "1. Исходный массив -> по возрастанию:\n";
    SortStats stats1 = selectionSort(arrSelection1, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrSelection1, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats1.comparisons
              << ", Перестановки: " << stats1.swaps << "\n\n";

    // 2. Уже отсортированный -> по возрастанию (заново)
    std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
    SortStats stats2 = selectionSort(arrSelection2, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrSelection2, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats2.comparisons
              << ", Перестановки: " << stats2.swaps << "\n\n";

    // 3. Отсортированный -> по убыванию
    std::cout << "3. Отсортированный массив -> по убыванию:\n";
    SortStats stats3 = selectionSort(arrSelection3, STATIC_SIZE, false);
    std::cout << "   Массив: ";
    printArray(arrSelection3, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats3.comparisons
              << ", Перестановки: " << stats3.swaps << "\n\n";

    // Тестируем сортировку пузырьком (ОСНОВНОЕ ЗАДАНИЕ)
    std::cout << "АЛГОРИТМ: СОРТИРОВКА ПУЗЫРЬКОМ (основное задание)\n";
    std::cout << std::string(70, '-') << "\n";

    // 1. Исходный -> по возрастанию
    std::cout << "1. Исходный массив -> по возрастанию:\n";
    SortStats stats4 = bubbleSort(arrBubble1, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrBubble1, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats4.comparisons
              << ", Перестановки: " << stats4.swaps << "\n\n";

    // 2. Уже отсортированный -> по возрастанию (заново)
    std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
    SortStats stats5 = bubbleSort(arrBubble2, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrBubble2, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats5.comparisons
              << ", Перестановки: " << stats5.swaps << "\n\n";

    // 3. Отсортированный -> по убыванию
    std::cout << "3. Отсортированный массив -> по убыванию:\n";
    SortStats stats6 = bubbleSort(arrBubble3, STATIC_SIZE, false);
    std::cout << "   Массив: ";
    printArray(arrBubble3, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats6.comparisons
              << ", Перестановки: " << stats6.swaps << "\n\n";

    // Тестируем быструю сортировку (ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ)
    std::cout << "АЛГОРИТМ: БЫСТРАЯ СОРТИРОВКА (дополнительное задание)\n";
    std::cout << std::string(70, '-') << "\n";

    // 1. Исходный -> по возрастанию
    std::cout << "1. Исходный массив -> по возрастанию:\n";
    SortStats stats7 = quickSort(arrQuick1, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrQuick1, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats7.comparisons
              << ", Перестановки: " << stats7.swaps << "\n\n";

    // 2. Уже отсортированный -> по возрастанию (заново)
    std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
    SortStats stats8 = quickSort(arrQuick2, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrQuick2, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats8.comparisons
              << ", Перестановки: " << stats8.swaps << "\n\n";

    // 3. Отсортированный -> по убыванию
    std::cout << "3. Отсортированный массив -> по убыванию:\n";
    SortStats stats9 = quickSort(arrQuick3, STATIC_SIZE, false);
    std::cout << "   Массив: ";
    printArray(arrQuick3, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats9.comparisons
              << ", Перестановки: " << stats9.swaps << "\n\n";

    std::cout << std::string(70, '=') << "\n";
    std::cout << "ТЕСТИРОВАНИЕ СТАТИЧЕСКИХ МАССИВОВ ЗАВЕРШЕНО\n";
    std::cout << std::string(70, '=') << "\n";
}

// Функция для тестирования на кратных размерах
void testMultipleSizes(size_t baseSize) {
    std::vector<size_t> sizes;

    // Создаем последовательность: baseSize, baseSize*10, baseSize*100, baseSize*1000
    size_t current = baseSize;
    for (int i = 0; i < 4; i++) {
        sizes.push_back(current);
        current *= 10;
    }

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "ТЕСТИРОВАНИЕ НА РАЗМЕРАХ: ";
    for (size_t i = 0; i < sizes.size(); i++) {
        std::cout << sizes[i];
        if (i < sizes.size() - 1) std::cout << ", ";
    }
    std::cout << "\n" << std::string(70, '=') << "\n";

    // Заголовок таблицы
    std::cout << "\nРАЗМЕР | МЕТОД         | СРАВНЕНИЯ   | ПЕРЕСТАНОВКИ | ВРЕМЯ (мс)\n";
    std::cout << std::string(70, '-') << "\n";

    // Тестируем для каждого размера
    for (size_t size : sizes) {
        std::cout << "\n" << std::setw(6) << size << " ";
        std::cout << std::string(64, '-') << "\n";

        // Создаем и заполняем массив
        int* arr = new int[size];
        fillRandom(arr, size);

        // Для маленьких массивов показываем исходные данные
        if (size <= 20) {
            std::cout << "Исходный массив: ";
            printArray(arr, size);
            std::cout << "\n";
        }

        // Тестируем три алгоритма

        // 1. Сортировка выбором (ОСНОВНОЕ ЗАДАНИЕ)
        {
            int* arrCopy = copyArray(arr, size);
            SortStats stats = selectionSort(arrCopy, size, true);

            std::cout << "       | " << std::setw(13) << std::left << "Выбором"
                      << "| " << std::setw(11) << stats.comparisons
                      << "| " << std::setw(13) << stats.swaps
                      << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

            delete[] arrCopy;
        }

        // 2. Сортировка пузырьком (ОСНОВНОЕ ЗАДАНИЕ)
        {
            int* arrCopy = copyArray(arr, size);
            SortStats stats = bubbleSort(arrCopy, size, true);

            std::cout << "       | " << std::setw(13) << std::left << "Пузырьком"
                      << "| " << std::setw(11) << stats.comparisons
                      << "| " << std::setw(13) << stats.swaps
                      << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

            delete[] arrCopy;
        }

        // 3. Быстрая сортировка (ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ)
        {
            int* arrCopy = copyArray(arr, size);
            SortStats stats = quickSort(arrCopy, size, true);

            std::cout << "       | " << std::setw(13) << std::left << "Быстрая"
                      << "| " << std::setw(11) << stats.comparisons
                      << "| " << std::setw(13) << stats.swaps
                      << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

            delete[] arrCopy;
        }

        delete[] arr;
    }

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "ТЕСТИРОВАНИЕ ЗАВЕРШЕНО\n";
}

void Assembling() {
    bool continueProgram = true;

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "           ТЕСТИРОВАНИЕ АЛГОРИТМОВ СОРТИРОВКИ\n";
    std::cout << "           (автоматическое тестирование кратных размеров)\n";
    std::cout << std::string(70, '=') << "\n";

    // Часть 1: Тестирование на статических массивах
    testStaticArrays();

    do {
        // Часть 2: Тестирование на динамических массивах (кратные размеры)
        std::cout << "\n\nЧАСТЬ 2: ТЕСТИРОВАНИЕ НА ДИНАМИЧЕСКИХ МАССИВАХ\n";
        std::cout << std::string(70, '-') << "\n";

        std::cout << "\nВведите БАЗОВЫЙ размер массива (например, 2, 5, 10):\n";
        std::cout << "Программа автоматически протестирует размеры: N, N*10, N*100, N*1000\n";
        std::cout << "Базовый размер: ";

        size_t baseSize = getValidInput("");

        if (baseSize > 10000) {
            char confirm;
            std::cout << "\n⚠️  ВНИМАНИЕ: Максимальный тестовый размер будет " << baseSize * 1000
                      << " элементов.\n   Это может занять много времени и памяти.\n"
                      << "   Продолжить? (y/n): ";
            std::cin >> confirm;
            if (confirm != 'y' && confirm != 'Y') {
                continueProgram = askToContinue();
                continue;
            }
        }

        // Выполняем тестирование на кратных размерах
        testMultipleSizes(baseSize);

        // Запрос о продолжении
        continueProgram = askToContinue();

    } while (continueProgram);

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "           ПРОГРАММА ЗАВЕРШЕНА. ДО СВИДАНИЯ!\n";
    std::cout << std::string(70, '=') << "\n";
}

}


// #include "sorting.hpp"
// #include <iostream>
// #include <iomanip>
// #include <random>
// #include <algorithm>
// #include <string>
// #include <cstddef>
// #include <limits>
// #include <chrono>
// #include <vector>

// namespace {

// struct SortStats {
//     size_t comparisons = 0;
//     size_t swaps = 0;
//     double time_ms = 0.0;
// };

// // Функция вывода массива
// template<typename T>
// void printArray(T* arr, size_t size) {
//     std::cout << "[";
//     for (size_t i = 0; i < size && i < 20; ++i) {  // показываем максимум 20 элементов
//         std::cout << std::setw(3) << arr[i];
//         if (i < size - 1 && i < 19) std::cout << " ";
//     }
//     if (size > 20) std::cout << " ...";
//     std::cout << "]";
// }

// // Сортировка выбором
// template<typename T>
// SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
//     SortStats stats;
//     auto start = std::chrono::high_resolution_clock::now();

//     for (size_t i = 0; i < size - 1; ++i) {
//         size_t extremeIndex = i;

//         for (size_t j = i + 1; j < size; ++j) {
//             stats.comparisons++;
//             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
//                 extremeIndex = j;
//             }
//         }

//         if (extremeIndex != i) {
//             std::swap(arr[i], arr[extremeIndex]);
//             stats.swaps++;
//         }
//     }

//     auto end = std::chrono::high_resolution_clock::now();
//     stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

//     return stats;
// }

// // Сортировка пузырьком
// template<typename T>
// SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
//     SortStats stats;
//     auto start = std::chrono::high_resolution_clock::now();
//     bool swapped;

//     for (size_t i = 0; i < size - 1; ++i) {
//         swapped = false;

//         for (size_t j = 0; j < size - i - 1; ++j) {
//             stats.comparisons++;
//             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
//                 std::swap(arr[j], arr[j + 1]);
//                 stats.swaps++;
//                 swapped = true;
//             }
//         }

//         if (!swapped) break;
//     }

//     auto end = std::chrono::high_resolution_clock::now();
//     stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

//     return stats;
// }

// // Быстрая сортировка (дополнительное задание)
// template<typename T>
// size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
//     T pivot = arr[high];
//     size_t i = low;

//     for (size_t j = low; j < high; ++j) {
//         stats.comparisons++;
//         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
//             if (i != j) {
//                 std::swap(arr[i], arr[j]);
//                 stats.swaps++;
//             }
//             i++;
//         }
//     }

//     if (i != high) {
//         std::swap(arr[i], arr[high]);
//         stats.swaps++;
//     }

//     return i;
// }

// template<typename T>
// void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
//     if (low < high) {
//         size_t pi = partition(arr, low, high, ascending, stats);

//         if (pi > 0) {
//             quickSortRecursive(arr, low, pi - 1, ascending, stats);
//         }
//         quickSortRecursive(arr, pi + 1, high, ascending, stats);
//     }
// }

// template<typename T>
// SortStats quickSort(T* arr, size_t size, bool ascending = true) {
//     SortStats stats;
//     if (size > 1) {
//         auto start = std::chrono::high_resolution_clock::now();
//         quickSortRecursive(arr, 0, size - 1, ascending, stats);
//         auto end = std::chrono::high_resolution_clock::now();
//         stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();
//     }
//     return stats;
// }

// // Заполнение массива случайными числами
// void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
//     std::random_device rd;
//     std::default_random_engine engine(rd());
//     std::uniform_int_distribution<int> distribution(min, max);

//     for (size_t i = 0; i < size; ++i) {
//         arr[i] = distribution(engine);
//     }
// }

// // Копирование массива
// int* copyArray(const int* source, size_t size) {
//     int* copy = new int[size];
//     for (size_t i = 0; i < size; ++i) {
//         copy[i] = source[i];
//     }
//     return copy;
// }

// // Копирование статического массива
// template<size_t N>
// void copyStaticArray(const int (&source)[N], int (&dest)[N]) {
//     for (size_t i = 0; i < N; ++i) {
//         dest[i] = source[i];
//     }
// }

// // Явные инстанциации шаблонов для int
// template void printArray<int>(int* arr, size_t size);
// template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // Функция для проверки ввода числа
// size_t getValidInput(const std::string& prompt) {
//     size_t value;
//     while (true) {
//         std::cout << prompt;
//         std::cin >> value;

//         if (std::cin.fail()) {
//             std::cin.clear();
//             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//             std::cout << "Ошибка! Введите целое число: ";
//         } else if (value == 0) {
//             std::cout << "Ошибка! Число должно быть больше 0. Попробуйте снова: ";
//         } else {
//             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//             return value;
//         }
//     }
// }

// } // анонимное пространство имен

// namespace AssemblingSorting {

// // Функция для запроса продолжения
// bool askToContinue() {
//     char choice;
//     std::cout << "\n" << std::string(60, '=') << "\n";
//     std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:\n";
//     std::cout << "1 - Выполнить тест с другим размером массива\n";
//     std::cout << "0 - Завершить программу\n";
//     std::cout << "Выбор: ";
//     std::cin >> choice;
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

//     return (choice == '1');
// }

// // Тестирование на статических массивах (5-10 элементов)
// void testStaticArrays() {
//     const size_t SIZE = 10;

//     std::cout << "\n" << std::string(70, '=') << "\n";
//     std::cout << "ТЕСТИРОВАНИЕ НА СТАТИЧЕСКОМ МАССИВЕ (" << SIZE << " элементов)\n";
//     std::cout << std::string(70, '=') << "\n";

//     // Создаем и заполняем исходный статический массив
//     int originalArr[SIZE];
//     fillRandom(originalArr, SIZE);

//     std::cout << "\nИсходный массив: ";
//     printArray(originalArr, SIZE);
//     std::cout << "\n\n";

//     // Массивы для копий
//     int arrSelection1[SIZE], arrSelection2[SIZE], arrSelection3[SIZE];
//     int arrBubble1[SIZE], arrBubble2[SIZE], arrBubble3[SIZE];

//     // Копируем исходный массив
//     copyStaticArray(originalArr, arrSelection1);
//     copyStaticArray(originalArr, arrSelection2);
//     copyStaticArray(originalArr, arrSelection3);
//     copyStaticArray(originalArr, arrBubble1);
//     copyStaticArray(originalArr, arrBubble2);
//     copyStaticArray(originalArr, arrBubble3);

//     // Тестируем сортировку выбором
//     std::cout << "АЛГОРИТМ: СОРТИРОВКА ВЫБОРОМ\n";
//     std::cout << std::string(70, '-') << "\n";

//     // 1. Исходный -> по возрастанию
//     std::cout << "1. Исходный массив -> по возрастанию:\n";
//     SortStats stats1 = selectionSort(arrSelection1, SIZE, true);
//     std::cout << "   Массив: ";
//     printArray(arrSelection1, SIZE);
//     std::cout << "\n   Сравнения: " << stats1.comparisons
//               << ", Перестановки: " << stats1.swaps << "\n\n";

//     // 2. Уже отсортированный -> по возрастанию (заново)
//     std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
//     SortStats stats2 = selectionSort(arrSelection2, SIZE, true);
//     std::cout << "   Массив: ";
//     printArray(arrSelection2, SIZE);
//     std::cout << "\n   Сравнения: " << stats2.comparisons
//               << ", Перестановки: " << stats2.swaps << "\n\n";

//     // 3. Отсортированный -> по убыванию
//     std::cout << "3. Отсортированный массив -> по убыванию:\n";
//     SortStats stats3 = selectionSort(arrSelection3, SIZE, false);
//     std::cout << "   Массив: ";
//     printArray(arrSelection3, SIZE);
//     std::cout << "\n   Сравнения: " << stats3.comparisons
//               << ", Перестановки: " << stats3.swaps << "\n\n";

//     // Тестируем сортировку пузырьком
//     std::cout << "АЛГОРИТМ: СОРТИРОВКА ПУЗЫРЬКОМ\n";
//     std::cout << std::string(70, '-') << "\n";

//     // 1. Исходный -> по возрастанию
//     std::cout << "1. Исходный массив -> по возрастанию:\n";
//     SortStats stats4 = bubbleSort(arrBubble1, SIZE, true);
//     std::cout << "   Массив: ";
//     printArray(arrBubble1, SIZE);
//     std::cout << "\n   Сравнения: " << stats4.comparisons
//               << ", Перестановки: " << stats4.swaps << "\n\n";

//     // 2. Уже отсортированный -> по возрастанию (заново)
//     std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
//     SortStats stats5 = bubbleSort(arrBubble2, SIZE, true);
//     std::cout << "   Массив: ";
//     printArray(arrBubble2, SIZE);
//     std::cout << "\n   Сравнения: " << stats5.comparisons
//               << ", Перестановки: " << stats5.swaps << "\n\n";

//     // 3. Отсортированный -> по убыванию
//     std::cout << "3. Отсортированный массив -> по убыванию:\n";
//     SortStats stats6 = bubbleSort(arrBubble3, SIZE, false);
//     std::cout << "   Массив: ";
//     printArray(arrBubble3, SIZE);
//     std::cout << "\n   Сравнения: " << stats6.comparisons
//               << ", Перестановки: " << stats6.swaps << "\n\n";

//     std::cout << std::string(70, '=') << "\n";
//     std::cout << "ТЕСТИРОВАНИЕ СТАТИЧЕСКИХ МАССИВОВ ЗАВЕРШЕНО\n";
//     std::cout << std::string(70, '=') << "\n";
// }

// // Функция для тестирования на кратных размерах
// void testMultipleSizes(size_t baseSize) {
//     std::vector<size_t> sizes;

//     // Создаем последовательность: baseSize, baseSize*10, baseSize*100, baseSize*1000
//     size_t current = baseSize;
//     for (int i = 0; i < 4; i++) {
//         sizes.push_back(current);
//         current *= 10;
//     }

//     std::cout << "\n" << std::string(70, '=') << "\n";
//     std::cout << "ТЕСТИРОВАНИЕ НА РАЗМЕРАХ: ";
//     for (size_t i = 0; i < sizes.size(); i++) {
//         std::cout << sizes[i];
//         if (i < sizes.size() - 1) std::cout << ", ";
//     }
//     std::cout << "\n" << std::string(70, '=') << "\n";

//     // Заголовок таблицы
//     std::cout << "\nРАЗМЕР | МЕТОД         | СРАВНЕНИЯ   | ПЕРЕСТАНОВКИ | ВРЕМЯ (мс)\n";
//     std::cout << std::string(70, '-') << "\n";

//     // Тестируем для каждого размера
//     for (size_t size : sizes) {
//         std::cout << "\n" << std::setw(6) << size << " ";
//         std::cout << std::string(64, '-') << "\n";

//         // Создаем и заполняем массив
//         int* arr = new int[size];
//         fillRandom(arr, size);

//         // Для маленьких массивов показываем исходные данные
//         if (size <= 20) {
//             std::cout << "Исходный массив: ";
//             printArray(arr, size);
//             std::cout << "\n";
//         }

//         // Тестируем три алгоритма

//         // 1. Сортировка выбором
//         {
//             int* arrCopy = copyArray(arr, size);
//             SortStats stats = selectionSort(arrCopy, size, true);

//             std::cout << "       | " << std::setw(13) << std::left << "Выбором"
//                       << "| " << std::setw(11) << stats.comparisons
//                       << "| " << std::setw(13) << stats.swaps
//                       << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

//             delete[] arrCopy;
//         }

//         // 2. Сортировка пузырьком
//         {
//             int* arrCopy = copyArray(arr, size);
//             SortStats stats = bubbleSort(arrCopy, size, true);

//             std::cout << "       | " << std::setw(13) << std::left << "Пузырьком"
//                       << "| " << std::setw(11) << stats.comparisons
//                       << "| " << std::setw(13) << stats.swaps
//                       << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

//             delete[] arrCopy;
//         }

//         // 3. Быстрая сортировка (дополнительное задание)
//         {
//             int* arrCopy = copyArray(arr, size);
//             SortStats stats = quickSort(arrCopy, size, true);

//             std::cout << "       | " << std::setw(13) << std::left << "Быстрая"
//                       << "| " << std::setw(11) << stats.comparisons
//                       << "| " << std::setw(13) << stats.swaps
//                       << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

//             delete[] arrCopy;
//         }

//         delete[] arr;
//     }

//     std::cout << "\n" << std::string(70, '=') << "\n";
//     std::cout << "ТЕСТИРОВАНИЕ ЗАВЕРШЕНО\n";
// }

// void Assembling() {
//     bool continueProgram = true;

//     std::cout << "\n" << std::string(70, '=') << "\n";
//     std::cout << "           ТЕСТИРОВАНИЕ АЛГОРИТМОВ СОРТИРОВКИ\n";
//     std::cout << "           (автоматическое тестирование кратных размеров)\n";
//     std::cout << std::string(70, '=') << "\n";

//     // Часть 1: Тестирование на статических массивах
//     testStaticArrays();

//     do {
//         // Часть 2: Тестирование на динамических массивах (кратные размеры)
//         std::cout << "\n\nЧАСТЬ 2: ТЕСТИРОВАНИЕ НА ДИНАМИЧЕСКИХ МАССИВАХ\n";
//         std::cout << std::string(70, '-') << "\n";

//         std::cout << "\nВведите БАЗОВЫЙ размер массива (например, 2, 5, 10):\n";
//         std::cout << "Программа автоматически протестирует размеры: N, N*10, N*100, N*1000\n";
//         std::cout << "Базовый размер: ";

//         size_t baseSize = getValidInput("");

//         if (baseSize > 10000) {
//             char confirm;
//             std::cout << "\n⚠️  ВНИМАНИЕ: Максимальный тестовый размер будет " << baseSize * 1000
//                       << " элементов.\n   Это может занять много времени и памяти.\n"
//                       << "   Продолжить? (y/n): ";
//             std::cin >> confirm;
//             if (confirm != 'y' && confirm != 'Y') {
//                 continueProgram = askToContinue();
//                 continue;
//             }
//         }

//         // Выполняем тестирование на кратных размерах
//         testMultipleSizes(baseSize);

//         // Запрос о продолжении
//         continueProgram = askToContinue();

//     } while (continueProgram);

//     std::cout << "\n" << std::string(70, '=') << "\n";
//     std::cout << "           ПРОГРАММА ЗАВЕРШЕНА. ДО СВИДАНИЯ!\n";
//     std::cout << std::string(70, '=') << "\n";
// }

// }



// // #include "sorting.hpp"
// // #include <iostream>
// // #include <iomanip>
// // #include <random>
// // #include <algorithm>
// // #include <string>
// // #include <cstddef>
// // #include <limits>
// // #include <chrono>

// // namespace {

// // struct SortStats {
// //     size_t comparisons = 0;
// //     size_t swaps = 0;
// //     double time_ms = 0.0;
// // };

// // // Функция вывода массива
// // template<typename T>
// // void printArray(T* arr, size_t size) {
// //     std::cout << "[";
// //     for (size_t i = 0; i < size; ++i) {
// //         std::cout << std::setw(3) << arr[i];
// //         if (i < size - 1) std::cout << " ";
// //     }
// //     std::cout << "]";
// // }

// // // Сортировка выбором
// // template<typename T>
// // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// //     SortStats stats;
// //     auto start = std::chrono::high_resolution_clock::now();

// //     for (size_t i = 0; i < size - 1; ++i) {
// //         size_t extremeIndex = i;

// //         for (size_t j = i + 1; j < size; ++j) {
// //             stats.comparisons++;
// //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// //                 extremeIndex = j;
// //             }
// //         }

// //         if (extremeIndex != i) {
// //             std::swap(arr[i], arr[extremeIndex]);
// //             stats.swaps++;
// //         }
// //     }

// //     auto end = std::chrono::high_resolution_clock::now();
// //     stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

// //     return stats;
// // }

// // // Сортировка пузырьком
// // template<typename T>
// // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// //     SortStats stats;
// //     auto start = std::chrono::high_resolution_clock::now();
// //     bool swapped;

// //     for (size_t i = 0; i < size - 1; ++i) {
// //         swapped = false;

// //         for (size_t j = 0; j < size - i - 1; ++j) {
// //             stats.comparisons++;
// //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// //                 std::swap(arr[j], arr[j + 1]);
// //                 stats.swaps++;
// //                 swapped = true;
// //             }
// //         }

// //         if (!swapped) break;
// //     }

// //     auto end = std::chrono::high_resolution_clock::now();
// //     stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

// //     return stats;
// // }

// // // Быстрая сортировка (дополнительное задание)
// // template<typename T>
// // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// //     T pivot = arr[high];
// //     size_t i = low;

// //     for (size_t j = low; j < high; ++j) {
// //         stats.comparisons++;
// //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// //             if (i != j) {
// //                 std::swap(arr[i], arr[j]);
// //                 stats.swaps++;
// //             }
// //             i++;
// //         }
// //     }

// //     if (i != high) {
// //         std::swap(arr[i], arr[high]);
// //         stats.swaps++;
// //     }

// //     return i;
// // }

// // template<typename T>
// // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// //     if (low < high) {
// //         size_t pi = partition(arr, low, high, ascending, stats);

// //         if (pi > 0) {
// //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// //         }
// //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// //     }
// // }

// // template<typename T>
// // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// //     SortStats stats;
// //     if (size > 1) {
// //         auto start = std::chrono::high_resolution_clock::now();
// //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// //         auto end = std::chrono::high_resolution_clock::now();
// //         stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();
// //     }
// //     return stats;
// // }

// // // Заполнение массива случайными числами
// // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// //     std::random_device rd;
// //     std::default_random_engine engine(rd());
// //     std::uniform_int_distribution<int> distribution(min, max);

// //     for (size_t i = 0; i < size; ++i) {
// //         arr[i] = distribution(engine);
// //     }
// // }

// // // Копирование массива (для динамических)
// // int* copyArray(const int* source, size_t size) {
// //     int* copy = new int[size];
// //     for (size_t i = 0; i < size; ++i) {
// //         copy[i] = source[i];
// //     }
// //     return copy;
// // }

// // // Копирование статического массива в другой статический
// // template<size_t N>
// // void copyStaticArray(const int (&source)[N], int (&dest)[N]) {
// //     for (size_t i = 0; i < N; ++i) {
// //         dest[i] = source[i];
// //     }
// // }

// // // Явные инстанциации шаблонов для int
// // template void printArray<int>(int* arr, size_t size);
// // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // Функция для проверки ввода числа
// // size_t getValidInput(const std::string& prompt) {
// //     size_t value;
// //     while (true) {
// //         std::cout << prompt;
// //         std::cin >> value;

// //         if (std::cin.fail()) {
// //             std::cin.clear();
// //             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// //             std::cout << "Ошибка! Введите целое число: ";
// //         } else if (value == 0) {
// //             std::cout << "Ошибка! Число должно быть больше 0. Попробуйте снова: ";
// //         } else {
// //             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// //             return value;
// //         }
// //     }
// // }

// // } // анонимное пространство имен

// // namespace AssemblingSorting {

// // // Функция для запроса продолжения
// // bool askToContinue() {
// //     char choice;
// //     std::cout << "\n" << std::string(60, '=') << "\n";
// //     std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:\n";
// //     std::cout << "1 - Продолжить работу\n";
// //     std::cout << "0 - Завершить программу\n";
// //     std::cout << "Выбор: ";
// //     std::cin >> choice;
// //     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// //     return (choice == '1');
// // }

// // // Тестирование на статических массивах (5-10 элементов)
// // void testStaticArrays() {
// //     const size_t SIZE = 10; // Статический массив из 10 элементов

// //     std::cout << "\n" << std::string(70, '=') << "\n";
// //     std::cout << "ТЕСТИРОВАНИЕ НА СТАТИЧЕСКОМ МАССИВЕ (" << SIZE << " элементов)\n";
// //     std::cout << std::string(70, '=') << "\n";

// //     // Создаем и заполняем исходный статический массив
// //     int originalArr[SIZE];
// //     fillRandom(originalArr, SIZE);

// //     std::cout << "\nИсходный массив: ";
// //     printArray(originalArr, SIZE);
// //     std::cout << "\n\n";

// //     // Массивы для копий (для каждого алгоритма и каждой итерации)
// //     int arrSelection1[SIZE], arrSelection2[SIZE], arrSelection3[SIZE];
// //     int arrBubble1[SIZE], arrBubble2[SIZE], arrBubble3[SIZE];

// //     // Копируем исходный массив
// //     copyStaticArray(originalArr, arrSelection1);
// //     copyStaticArray(originalArr, arrSelection2);
// //     copyStaticArray(originalArr, arrSelection3);
// //     copyStaticArray(originalArr, arrBubble1);
// //     copyStaticArray(originalArr, arrBubble2);
// //     copyStaticArray(originalArr, arrBubble3);

// //     // Тестируем сортировку выбором
// //     std::cout << "АЛГОРИТМ: СОРТИРОВКА ВЫБОРОМ\n";
// //     std::cout << std::string(70, '-') << "\n";

// //     // 1. Исходный -> по возрастанию
// //     std::cout << "1. Исходный массив -> по возрастанию:\n";
// //     SortStats stats1 = selectionSort(arrSelection1, SIZE, true);
// //     std::cout << "   Массив: ";
// //     printArray(arrSelection1, SIZE);
// //     std::cout << "\n   Сравнения: " << stats1.comparisons
// //               << ", Перестановки: " << stats1.swaps << "\n\n";

// //     // 2. Уже отсортированный -> по возрастанию (заново)
// //     std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
// //     SortStats stats2 = selectionSort(arrSelection2, SIZE, true);
// //     std::cout << "   Массив: ";
// //     printArray(arrSelection2, SIZE);
// //     std::cout << "\n   Сравнения: " << stats2.comparisons
// //               << ", Перестановки: " << stats2.swaps << "\n\n";

// //     // 3. Отсортированный -> по убыванию
// //     std::cout << "3. Отсортированный массив -> по убыванию:\n";
// //     SortStats stats3 = selectionSort(arrSelection3, SIZE, false);
// //     std::cout << "   Массив: ";
// //     printArray(arrSelection3, SIZE);
// //     std::cout << "\n   Сравнения: " << stats3.comparisons
// //               << ", Перестановки: " << stats3.swaps << "\n\n";

// //     // Тестируем сортировку пузырьком
// //     std::cout << "АЛГОРИТМ: СОРТИРОВКА ПУЗЫРЬКОМ\n";
// //     std::cout << std::string(70, '-') << "\n";

// //     // 1. Исходный -> по возрастанию
// //     std::cout << "1. Исходный массив -> по возрастанию:\n";
// //     SortStats stats4 = bubbleSort(arrBubble1, SIZE, true);
// //     std::cout << "   Массив: ";
// //     printArray(arrBubble1, SIZE);
// //     std::cout << "\n   Сравнения: " << stats4.comparisons
// //               << ", Перестановки: " << stats4.swaps << "\n\n";

// //     // 2. Уже отсортированный -> по возрастанию (заново)
// //     std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
// //     SortStats stats5 = bubbleSort(arrBubble2, SIZE, true);
// //     std::cout << "   Массив: ";
// //     printArray(arrBubble2, SIZE);
// //     std::cout << "\n   Сравнения: " << stats5.comparisons
// //               << ", Перестановки: " << stats5.swaps << "\n\n";

// //     // 3. Отсортированный -> по убыванию
// //     std::cout << "3. Отсортированный массив -> по убыванию:\n";
// //     SortStats stats6 = bubbleSort(arrBubble3, SIZE, false);
// //     std::cout << "   Массив: ";
// //     printArray(arrBubble3, SIZE);
// //     std::cout << "\n   Сравнения: " << stats6.comparisons
// //               << ", Перестановки: " << stats6.swaps << "\n\n";

// //     std::cout << std::string(70, '=') << "\n";
// //     std::cout << "ТЕСТИРОВАНИЕ СТАТИЧЕСКИХ МАССИВОВ ЗАВЕРШЕНО\n";
// //     std::cout << std::string(70, '=') << "\n";
// // }

// // // Тестирование на динамическом массиве заданного размера
// // void testDynamicArray(size_t size) {
// //     std::cout << "\n" << std::string(70, '=') << "\n";
// //     std::cout << "ТЕСТИРОВАНИЕ НА ДИНАМИЧЕСКОМ МАССИВЕ (" << size << " элементов)\n";
// //     std::cout << std::string(70, '=') << "\n";

// //     // Создаем и заполняем исходный динамический массив
// //     int* originalArr = new int[size];
// //     fillRandom(originalArr, size);

// //     // Для маленьких массивов показываем исходные данные
// //     if (size <= 20) {
// //         std::cout << "\nИсходный массив: ";
// //         printArray(originalArr, size);
// //         std::cout << "\n\n";
// //     }

// //     // Заголовок таблицы
// //     std::cout << "\nМЕТОД         | СРАВНЕНИЯ   | ПЕРЕСТАНОВКИ | ВРЕМЯ (мс)\n";
// //     std::cout << std::string(60, '-') << "\n";

// //     // Тестируем сортировку выбором
// //     {
// //         int* arrCopy = copyArray(originalArr, size);
// //         SortStats stats = selectionSort(arrCopy, size, true);

// //         std::cout << std::setw(13) << std::left << "Выбором"
// //                   << "| " << std::setw(11) << stats.comparisons
// //                   << "| " << std::setw(13) << stats.swaps
// //                   << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

// //         delete[] arrCopy;
// //     }

// //     // Тестируем сортировку пузырьком
// //     {
// //         int* arrCopy = copyArray(originalArr, size);
// //         SortStats stats = bubbleSort(arrCopy, size, true);

// //         std::cout << std::setw(13) << std::left << "Пузырьком"
// //                   << "| " << std::setw(11) << stats.comparisons
// //                   << "| " << std::setw(13) << stats.swaps
// //                   << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

// //         delete[] arrCopy;
// //     }

// //     // Тестируем быструю сортировку (дополнительное задание)
// //     {
// //         int* arrCopy = copyArray(originalArr, size);
// //         SortStats stats = quickSort(arrCopy, size, true);

// //         std::cout << std::setw(13) << std::left << "Быстрая"
// //                   << "| " << std::setw(11) << stats.comparisons
// //                   << "| " << std::setw(13) << stats.swaps
// //                   << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

// //         delete[] arrCopy;
// //     }

// //     delete[] originalArr;

// //     std::cout << std::string(60, '-') << "\n";
// //     std::cout << "\n" << std::string(70, '=') << "\n";
// //     std::cout << "ТЕСТИРОВАНИЕ ДИНАМИЧЕСКИХ МАССИВОВ ЗАВЕРШЕНО\n";
// //     std::cout << std::string(70, '=') << "\n";
// // }

// // void Assembling() {
// //     bool continueProgram = true;

// //     std::cout << "\n" << std::string(70, '=') << "\n";
// //     std::cout << "           ТЕСТИРОВАНИЕ АЛГОРИТМОВ СОРТИРОВКИ\n";
// //     std::cout << "           Лабораторная работа №4\n";
// //     std::cout << std::string(70, '=') << "\n";

// //     // Часть 1: Тестирование на статических массивах
// //     testStaticArrays();

// //     do {
// //         // Часть 2: Тестирование на динамических массивах
// //         std::cout << "\n\nЧАСТЬ 2: ТЕСТИРОВАНИЕ НА ДИНАМИЧЕСКИХ МАССИВАХ\n";
// //         std::cout << std::string(70, '-') << "\n";

// //         std::cout << "\nВведите размер динамического массива (целое число > 0): ";
// //         size_t dynamicSize = getValidInput("");

// //         if (dynamicSize > 1000000) {
// //             char confirm;
// //             std::cout << "\n⚠️  ВНИМАНИЕ: Размер массива очень большой (" << dynamicSize
// //                       << " элементов).\n   Это может занять много времени и памяти.\n"
// //                       << "   Продолжить? (y/n): ";
// //             std::cin >> confirm;
// //             if (confirm != 'y' && confirm != 'Y') {
// //                 continueProgram = askToContinue();
// //                 continue;
// //             }
// //         }

// //         testDynamicArray(dynamicSize);

// //         // Запрос о продолжении
// //         continueProgram = askToContinue();

// //     } while (continueProgram);

// //     std::cout << "\n" << std::string(70, '=') << "\n";
// //     std::cout << "           ПРОГРАММА ЗАВЕРШЕНА. ДО СВИДАНИЯ!\n";
// //     std::cout << std::string(70, '=') << "\n";
// // }

// // }





// // // #include "sorting.hpp"
// // // #include <iostream>
// // // #include <iomanip>
// // // #include <random>
// // // #include <algorithm>
// // // #include <string>
// // // #include <cstddef>
// // // #include <limits>
// // // #include <chrono>

// // // namespace {

// // // struct SortStats {
// // //     size_t comparisons = 0;
// // //     size_t swaps = 0;
// // //     double time_ms = 0.0;  // добавим время выполнения
// // // };

// // // // Функция вывода массива (только для маленьких массивов)
// // // template<typename T>
// // // void printArray(T* arr, size_t size) {
// // //     if (size <= 20) {  // выводим только если массив маленький
// // //         std::cout << "[";
// // //         for (size_t i = 0; i < std::min(size, size_t(20)); ++i) {
// // //             std::cout << std::setw(3) << arr[i];
// // //         }
// // //         std::cout << "]";
// // //         if (size > 20) std::cout << "...";
// // //         std::cout << std::endl;
// // //     }
// // // }

// // // // Сортировка выбором
// // // template<typename T>
// // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // //     SortStats stats;
// // //     auto start = std::chrono::high_resolution_clock::now();

// // //     for (size_t i = 0; i < size - 1; ++i) {
// // //         size_t extremeIndex = i;

// // //         for (size_t j = i + 1; j < size; ++j) {
// // //             stats.comparisons++;
// // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // //                 extremeIndex = j;
// // //             }
// // //         }

// // //         if (extremeIndex != i) {
// // //             std::swap(arr[i], arr[extremeIndex]);
// // //             stats.swaps++;
// // //         }
// // //     }

// // //     auto end = std::chrono::high_resolution_clock::now();
// // //     stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

// // //     return stats;
// // // }

// // // // Сортировка пузырьком
// // // template<typename T>
// // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // //     SortStats stats;
// // //     auto start = std::chrono::high_resolution_clock::now();
// // //     bool swapped;

// // //     for (size_t i = 0; i < size - 1; ++i) {
// // //         swapped = false;

// // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // //             stats.comparisons++;
// // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // //                 std::swap(arr[j], arr[j + 1]);
// // //                 stats.swaps++;
// // //                 swapped = true;
// // //             }
// // //         }

// // //         if (!swapped) break;
// // //     }

// // //     auto end = std::chrono::high_resolution_clock::now();
// // //     stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

// // //     return stats;
// // // }

// // // // Быстрая сортировка (вспомогательные функции)
// // // template<typename T>
// // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // //     T pivot = arr[high];
// // //     size_t i = low;

// // //     for (size_t j = low; j < high; ++j) {
// // //         stats.comparisons++;
// // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // //             if (i != j) {
// // //                 std::swap(arr[i], arr[j]);
// // //                 stats.swaps++;
// // //             }
// // //             i++;
// // //         }
// // //     }

// // //     if (i != high) {
// // //         std::swap(arr[i], arr[high]);
// // //         stats.swaps++;
// // //     }

// // //     return i;
// // // }

// // // template<typename T>
// // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // //     if (low < high) {
// // //         size_t pi = partition(arr, low, high, ascending, stats);

// // //         if (pi > 0) {
// // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // //         }
// // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // //     }
// // // }

// // // // Быстрая сортировка
// // // template<typename T>
// // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // //     SortStats stats;
// // //     if (size > 1) {
// // //         auto start = std::chrono::high_resolution_clock::now();
// // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // //         auto end = std::chrono::high_resolution_clock::now();
// // //         stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();
// // //     }
// // //     return stats;
// // // }

// // // // Заполнение массива случайными числами
// // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // //     std::random_device rd;
// // //     std::default_random_engine engine(rd());
// // //     std::uniform_int_distribution<int> distribution(min, max);

// // //     for (size_t i = 0; i < size; ++i) {
// // //         arr[i] = distribution(engine);
// // //     }
// // // }

// // // // Копирование массива
// // // int* copyArray(const int* source, size_t size) {
// // //     int* copy = new int[size];
// // //     for (size_t i = 0; i < size; ++i) {
// // //         copy[i] = source[i];
// // //     }
// // //     return copy;
// // // }

// // // // Явные инстанциации шаблонов для int
// // // template void printArray<int>(int* arr, size_t size);
// // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // Функция для проверки ввода числа
// // // size_t getValidInput(const std::string& prompt) {
// // //     size_t value;
// // //     while (true) {
// // //         std::cout << prompt;
// // //         std::cin >> value;

// // //         if (std::cin.fail()) {
// // //             std::cin.clear();
// // //             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// // //             std::cout << "Ошибка! Введите целое число: ";
// // //         } else if (value == 0) {
// // //             std::cout << "Ошибка! Число должно быть больше 0. Попробуйте снова: ";
// // //         } else {
// // //             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// // //             return value;
// // //         }
// // //     }
// // // }

// // // } // анонимное пространство имен

// // // namespace AssemblingSorting {

// // // // Функция для запроса продолжения
// // // bool askToContinue() {
// // //     char choice;
// // //     std::cout << "\n" << std::string(60, '=') << "\n";
// // //     std::cout << "ВЫБЕРИТЕ ДЕЙСТВИЕ:\n";
// // //     std::cout << "1 - Выполнить тест с другим размером массива\n";
// // //     std::cout << "0 - Завершить программу\n";
// // //     std::cout << "Выбор: ";
// // //     std::cin >> choice;
// // //     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// // //     return (choice == '1');
// // // }

// // // // Функция для тестирования на кратных размерах
// // // void testMultipleSizes(size_t baseSize) {
// // //     std::vector<size_t> sizes;

// // //     // Создаем последовательность: baseSize, baseSize*10, baseSize*100, baseSize*1000
// // //     size_t current = baseSize;
// // //     for (int i = 0; i < 4; i++) {
// // //         sizes.push_back(current);
// // //         current *= 10;
// // //     }

// // //     std::cout << "\n" << std::string(70, '=') << "\n";
// // //     std::cout << "ТЕСТИРОВАНИЕ НА РАЗМЕРАХ: ";
// // //     for (size_t i = 0; i < sizes.size(); i++) {
// // //         std::cout << sizes[i];
// // //         if (i < sizes.size() - 1) std::cout << ", ";
// // //     }
// // //     std::cout << "\n" << std::string(70, '=') << "\n";

// // //     // Заголовок таблицы
// // //     std::cout << "\nРАЗМЕР | МЕТОД         | СРАВНЕНИЯ   | ПЕРЕСТАНОВКИ | ВРЕМЯ (мс)\n";
// // //     std::cout << std::string(70, '-') << "\n";

// // //     // Тестируем для каждого размера
// // //     for (size_t size : sizes) {
// // //         std::cout << "\n" << std::setw(6) << size << " ";
// // //         std::cout << std::string(64, '-') << "\n";

// // //         // Создаем и заполняем массив
// // //         int* arr = new int[size];
// // //         fillRandom(arr, size);

// // //         // Для маленьких массивов показываем исходные данные
// // //         if (size <= 20) {
// // //             std::cout << "Исходный массив: ";
// // //             printArray(arr, size);
// // //         }

// // //         // Тестируем три алгоритма

// // //         // 1. Сортировка выбором
// // //         {
// // //             int* arrCopy = copyArray(arr, size);
// // //             SortStats stats = selectionSort(arrCopy, size, true);

// // //             std::cout << "       | " << std::setw(13) << std::left << "Выбором"
// // //                       << "| " << std::setw(11) << stats.comparisons
// // //                       << "| " << std::setw(13) << stats.swaps
// // //                       << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

// // //             delete[] arrCopy;
// // //         }

// // //         // 2. Сортировка пузырьком
// // //         {
// // //             int* arrCopy = copyArray(arr, size);
// // //             SortStats stats = bubbleSort(arrCopy, size, true);

// // //             std::cout << "       | " << std::setw(13) << std::left << "Пузырьком"
// // //                       << "| " << std::setw(11) << stats.comparisons
// // //                       << "| " << std::setw(13) << stats.swaps
// // //                       << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

// // //             delete[] arrCopy;
// // //         }

// // //         // 3. Быстрая сортировка
// // //         {
// // //             int* arrCopy = copyArray(arr, size);
// // //             SortStats stats = quickSort(arrCopy, size, true);

// // //             std::cout << "       | " << std::setw(13) << std::left << "Быстрая"
// // //                       << "| " << std::setw(11) << stats.comparisons
// // //                       << "| " << std::setw(13) << stats.swaps
// // //                       << "| " << std::fixed << std::setprecision(3) << stats.time_ms << "\n";

// // //             delete[] arrCopy;
// // //         }

// // //         delete[] arr;
// // //     }

// // //     std::cout << "\n" << std::string(70, '=') << "\n";
// // //     std::cout << "ТЕСТИРОВАНИЕ ЗАВЕРШЕНО\n";
// // // }

// // // void Assembling() {
// // //     bool continueProgram = true;

// // //     std::cout << "\n" << std::string(70, '=') << "\n";
// // //     std::cout << "           ТЕСТИРОВАНИЕ АЛГОРИТМОВ СОРТИРОВКИ\n";
// // //     std::cout << "           (автоматическое тестирование кратных размеров)\n";
// // //     std::cout << std::string(70, '=') << "\n";

// // //     do {
// // //         // Запрос базового размера
// // //         std::cout << "\nВведите БАЗОВЫЙ размер массива (например, 2, 5, 10):\n";
// // //         std::cout << "Программа автоматически протестирует размеры: N, N*10, N*100, N*1000\n";
// // //         std::cout << "Базовый размер: ";

// // //         size_t baseSize = getValidInput("");

// // //         if (baseSize > 10000) {
// // //             char confirm;
// // //             std::cout << "\n⚠️  ВНИМАНИЕ: Максимальный тестовый размер будет " << baseSize * 1000
// // //                       << " элементов.\n   Это может занять много времени и памяти.\n"
// // //                       << "   Продолжить? (y/n): ";
// // //             std::cin >> confirm;
// // //             if (confirm != 'y' && confirm != 'Y') {
// // //                 continueProgram = askToContinue();
// // //                 continue;
// // //             }
// // //         }

// // //         // Выполняем тестирование на кратных размерах
// // //         testMultipleSizes(baseSize);

// // //         // Запрос о продолжении
// // //         continueProgram = askToContinue();

// // //     } while (continueProgram);

// // //     std::cout << "\n" << std::string(70, '=') << "\n";
// // //     std::cout << "           ПРОГРАММА ЗАВЕРШЕНА. ДО СВИДАНИЯ!\n";
// // //     std::cout << std::string(70, '=') << "\n";
// // // }

// // // } // namespace AssemblingSorting





// // // #include "sorting.hpp"
// // // #include <iostream>
// // // #include <iomanip>
// // // #include <random>
// // // #include <cstddef>
// // // #include <limits>

// // // namespace {

// // // struct SortStats {
// // //     size_t comparisons = 0;
// // //     size_t swaps = 0;
// // // };

// // // // Функция вывода массива
// // // template<typename T>
// // // void printArray(T* arr, size_t size) {
// // //     std::cout << "[";
// // //     for (size_t i = 0; i < size; ++i) {
// // //         std::cout << std::setw(3) << arr[i];
// // //     }
// // //     std::cout << "]" << std::endl;
// // // }

// // // // Сортировка выбором
// // // template<typename T>
// // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // //     SortStats stats;

// // //     for (size_t i = 0; i < size - 1; ++i) {
// // //         size_t extremeIndex = i;

// // //         for (size_t j = i + 1; j < size; ++j) {
// // //             stats.comparisons++;
// // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // //                 extremeIndex = j;
// // //             }
// // //         }

// // //         if (extremeIndex != i) {
// // //             std::swap(arr[i], arr[extremeIndex]);
// // //             stats.swaps++;
// // //         }
// // //     }

// // //     return stats;
// // // }

// // // // Сортировка пузырьком
// // // template<typename T>
// // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // //     SortStats stats;
// // //     bool swapped;

// // //     for (size_t i = 0; i < size - 1; ++i) {
// // //         swapped = false;

// // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // //             stats.comparisons++;
// // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // //                 std::swap(arr[j], arr[j + 1]);
// // //                 stats.swaps++;
// // //                 swapped = true;
// // //             }
// // //         }

// // //         if (!swapped) break;
// // //     }

// // //     return stats;
// // // }

// // // // Быстрая сортировка (вспомогательные функции)
// // // template<typename T>
// // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // //     T pivot = arr[high];
// // //     size_t i = low;

// // //     for (size_t j = low; j < high; ++j) {
// // //         stats.comparisons++;
// // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // //             if (i != j) {
// // //                 std::swap(arr[i], arr[j]);
// // //                 stats.swaps++;
// // //             }
// // //             i++;
// // //         }
// // //     }

// // //     if (i != high) {
// // //         std::swap(arr[i], arr[high]);
// // //         stats.swaps++;
// // //     }

// // //     return i;
// // // }

// // // template<typename T>
// // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // //     if (low < high) {
// // //         size_t pi = partition(arr, low, high, ascending, stats);

// // //         if (pi > 0) {
// // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // //         }
// // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // //     }
// // // }

// // // // Быстрая сортировка
// // // template<typename T>
// // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // //     SortStats stats;
// // //     if (size > 1) {
// // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // //     }
// // //     return stats;
// // // }

// // // // Заполнение массива случайными числами
// // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // //     std::random_device rd;
// // //     std::default_random_engine engine(rd());
// // //     std::uniform_int_distribution<int> distribution(min, max);

// // //     for (size_t i = 0; i < size; ++i) {
// // //         arr[i] = distribution(engine);
// // //     }
// // // }

// // // // Копирование массива
// // // int* copyArray(const int* source, size_t size) {
// // //     int* copy = new int[size];
// // //     for (size_t i = 0; i < size; ++i) {
// // //         copy[i] = source[i];
// // //     }
// // //     return copy;
// // // }

// // // // Явные инстанциации шаблонов для int
// // // template void printArray<int>(int* arr, size_t size);
// // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // Функция для проверки ввода числа


// // // size_t getValidInput(const std::string& prompt) {
// // //     size_t value;
// // //     while (true) {
// // //         std::cout << prompt;
// // //         std::cin >> value;

// // //         if (std::cin.fail()) {
// // //             std::cin.clear();
// // //             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// // //             std::cout << "Ошибка! Введите целое число: ";
// // //         } else {
// // //             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// // //             return value;
// // //         }
// // //     }
// // // }

// // // } // анонимное пространство имен

// // // namespace AssemblingSorting {

// // // // Функция для запроса продолжения
// // // bool askToContinue() {
// // //     char choice;
// // //     std::cout << "\n============================================================\n";
// // //     std::cout << "Хотите выполнить еще один тест?\n";
// // //     std::cout << "1 - Выполнить новый тест с другими значениями\n";
// // //     std::cout << "0 - Выйти из программы\n";
// // //     std::cout << "Выбор: ";
// // //     std::cin >> choice;
// // //     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// // //     return (choice == '1');
// // // }

// // // void Assembling() {
// // //     bool continueProgram = true;

// // //     std::cout << "\n============================================================\n";
// // //     std::cout << "             АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ\n";
// // //     std::cout << "============================================================\n";

// // //     do {
// // //         // Часть 1: Статический массив
// // //         std::cout << "\nЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ\n";
// // //         std::cout << "============================================================\n";

// // //         // Запрос размера статического массива
// // //         size_t staticSize = getValidInput("Введите размер статического массива: ");

// // //         if (staticSize == 0) {
// // //             std::cout << "Размер массива должен быть больше 0!\n";
// // //             continue;
// // //         }

// // //         // Создаем статический массив через динамическое выделение
// // //         int* staticArr = new int[staticSize];
// // //         fillRandom(staticArr, staticSize);

// // //         std::cout << "\nИсходный массив (" << staticSize << " элементов): ";
// // //         printArray(staticArr, staticSize);

// // //         // Сортировка выбором
// // //         {
// // //             std::cout << "\n--------------- СОРТИРОВКА ВЫБОРОМ ---------------\n";
// // //             std::cout << "Метод          Направление           Сравнения  Перестановки\n";
// // //             std::cout << "------------------------------------------------------------\n";

// // //             int* arrCopy = copyArray(staticArr, staticSize);

// // //             SortStats stats = selectionSort(arrCopy, staticSize, true);
// // //             std::cout << "Выбором        возр. (не отсорт.)    "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             stats = selectionSort(arrCopy, staticSize, true);
// // //             std::cout << "Выбором        возр. (отсорт.)       "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             stats = selectionSort(arrCopy, staticSize, false);
// // //             std::cout << "Выбором        убыв.                 "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             std::cout << "------------------------------------------------------------\n";
// // //             std::cout << "Массив после: ";
// // //             printArray(arrCopy, staticSize);

// // //             delete[] arrCopy;
// // //         }

// // //         // Сортировка пузырьком
// // //         {
// // //             std::cout << "\n--------------- СОРТИРОВКА ПУЗЫРЬКОМ ---------------\n";
// // //             std::cout << "Метод          Направление           Сравнения  Перестановки\n";
// // //             std::cout << "------------------------------------------------------------\n";

// // //             int* arrCopy = copyArray(staticArr, staticSize);

// // //             SortStats stats = bubbleSort(arrCopy, staticSize, true);
// // //             std::cout << "Пузырьком      возр. (не отсорт.)    "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             stats = bubbleSort(arrCopy, staticSize, true);
// // //             std::cout << "Пузырьком      возр. (отсорт.)       "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             stats = bubbleSort(arrCopy, staticSize, false);
// // //             std::cout << "Пузырьком      убыв.                 "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             std::cout << "------------------------------------------------------------\n";
// // //             std::cout << "Массив после: ";
// // //             printArray(arrCopy, staticSize);

// // //             delete[] arrCopy;
// // //         }

// // //         // Быстрая сортировка
// // //         {
// // //             std::cout << "\n--------------- БЫСТРАЯ СОРТИРОВКА ---------------\n";
// // //             std::cout << "Метод          Направление           Сравнения  Перестановки\n";
// // //             std::cout << "------------------------------------------------------------\n";

// // //             int* arrCopy = copyArray(staticArr, staticSize);

// // //             SortStats stats = quickSort(arrCopy, staticSize, true);
// // //             std::cout << "Быстрая        возр. (не отсорт.)    "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             stats = quickSort(arrCopy, staticSize, true);
// // //             std::cout << "Быстрая        возр. (отсорт.)       "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             stats = quickSort(arrCopy, staticSize, false);
// // //             std::cout << "Быстрая        убыв.                 "
// // //                       << std::setw(10) << stats.comparisons << "     "
// // //                       << std::setw(10) << stats.swaps << "\n";

// // //             std::cout << "------------------------------------------------------------\n";
// // //             std::cout << "Массив после: ";
// // //             printArray(arrCopy, staticSize);

// // //             delete[] arrCopy;
// // //         }

// // //         delete[] staticArr;

// // //         // Часть 2: Динамический массив
// // //         std::cout << "\n============================================================\n";
// // //         std::cout << "             ДИНАМИЧЕСКИЙ МАССИВ\n";
// // //         std::cout << "============================================================\n";

// // //         // Запрос размера динамического массива
// // //         size_t dynamicSize = getValidInput("\nВведите размер динамического массива: ");

// // //         if (dynamicSize == 0) {
// // //             std::cout << "Размер массива должен быть больше 0!\n";
// // //             continueProgram = askToContinue();
// // //             continue;
// // //         }

// // //         int* dynamicArr = new int[dynamicSize];
// // //         fillRandom(dynamicArr, dynamicSize);

// // //         std::cout << "\nПервые 10 элементов: [";
// // //         for (size_t i = 0; i < std::min(dynamicSize, size_t(10)); ++i) {
// // //             std::cout << std::setw(3) << dynamicArr[i];
// // //         }
// // //         std::cout << "]";
// // //         if (dynamicSize > 10) std::cout << " ... (всего " << dynamicSize << ")";
// // //         std::cout << "\n";

// // //         std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ (" << dynamicSize << " элементов)\n";


// // //         std::cout << "============================================================\n";
// // //         std::cout << "Метод          Направление    Сравнения      Перестановки\n";
// // //         std::cout << "------------------------------------------------------------\n";

// // //         // Сортировка выбором
// // //         int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // //         SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // //         std::cout << "Выбором        возр.          "
// // //                   << std::setw(12) << stats1.comparisons << "     "
// // //                   << std::setw(12) << stats1.swaps << "\n";
// // //         delete[] arrCopy1;

// // //         // Сортировка пузырьком
// // //         int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // //         SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // //         std::cout << "Пузырьком      возр.          "
// // //                   << std::setw(12) << stats2.comparisons << "     "
// // //                   << std::setw(12) << stats2.swaps << "\n";
// // //         delete[] arrCopy2;

// // //         // Быстрая сортировка
// // //         int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // //         SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // //         std::cout << "Быстрая        возр.          "
// // //                   << std::setw(12) << stats3.comparisons << "     "
// // //                   << std::setw(12) << stats3.swaps << "\n";
// // //         delete[] arrCopy3;

// // //         std::cout << "============================================================\n";

// // //         delete[] dynamicArr;

// // //         // Запрос о продолжении
// // //         continueProgram = askToContinue();

// // //     } while (continueProgram);

// // //     std::cout << "\n============================================================\n";
// // //     std::cout << "             ПРОГРАММА ЗАВЕРШЕНА. ДО СВИДАНИЯ!\n";
// // //     std::cout << "============================================================\n";
// // // }

// // // } // namespace AssemblingSorting





// // // // #include "sorting.hpp"
// // // // #include <iostream>
// // // // #include <iomanip>
// // // // #include <random>


// // // // #include <cstddef>

// // // // namespace {

// // // // struct SortStats {
// // // //     size_t comparisons = 0;
// // // //     size_t swaps = 0;
// // // // };

// // // // // Функция вывода массива
// // // // template<typename T>
// // // // void printArray(T* arr, size_t size) {
// // // //     std::cout << "[";
// // // //     for (size_t i = 0; i < size; ++i) {
// // // //         std::cout << std::setw(3) << arr[i];
// // // //     }
// // // //     std::cout << "]" << std::endl;
// // // // }

// // // // // Сортировка выбором
// // // // template<typename T>
// // // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // // //     SortStats stats;

// // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // //         size_t extremeIndex = i;

// // // //         for (size_t j = i + 1; j < size; ++j) {
// // // //             stats.comparisons++;
// // // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // // //                 extremeIndex = j;
// // // //             }
// // // //         }

// // // //         if (extremeIndex != i) {
// // // //             std::swap(arr[i], arr[extremeIndex]);
// // // //             stats.swaps++;
// // // //         }
// // // //     }

// // // //     return stats;
// // // // }

// // // // // Сортировка пузырьком
// // // // template<typename T>
// // // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // // //     SortStats stats;
// // // //     bool swapped;

// // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // //         swapped = false;

// // // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // // //             stats.comparisons++;
// // // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // // //                 std::swap(arr[j], arr[j + 1]);
// // // //                 stats.swaps++;
// // // //                 swapped = true;
// // // //             }
// // // //         }

// // // //         if (!swapped) break;
// // // //     }

// // // //     return stats;
// // // // }

// // // // // Быстрая сортировка (вспомогательные функции)
// // // // template<typename T>
// // // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // //     T pivot = arr[high];
// // // //     size_t i = low;

// // // //     for (size_t j = low; j < high; ++j) {
// // // //         stats.comparisons++;
// // // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // // //             if (i != j) {
// // // //                 std::swap(arr[i], arr[j]);
// // // //                 stats.swaps++;
// // // //             }
// // // //             i++;
// // // //         }
// // // //     }

// // // //     if (i != high) {
// // // //         std::swap(arr[i], arr[high]);
// // // //         stats.swaps++;
// // // //     }

// // // //     return i;
// // // // }

// // // // template<typename T>
// // // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // //     if (low < high) {
// // // //         size_t pi = partition(arr, low, high, ascending, stats);

// // // //         if (pi > 0) {
// // // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // // //         }
// // // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // // //     }
// // // // }

// // // // // Быстрая сортировка
// // // // template<typename T>
// // // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // // //     SortStats stats;
// // // //     if (size > 1) {
// // // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // // //     }
// // // //     return stats;
// // // // }

// // // // // Заполнение массива случайными числами
// // // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // // //     std::random_device rd;
// // // //     std::default_random_engine engine(rd());
// // // //     std::uniform_int_distribution<int> distribution(min, max);

// // // //     for (size_t i = 0; i < size; ++i) {
// // // //         arr[i] = distribution(engine);
// // // //     }
// // // // }

// // // // // Копирование массива
// // // // int* copyArray(const int* source, size_t size) {
// // // //     int* copy = new int[size];
// // // //     for (size_t i = 0; i < size; ++i) {
// // // //         copy[i] = source[i];
// // // //     }
// // // //     return copy;
// // // // }

// // // // } // анонимное пространство имен

// // // // namespace AssemblingSorting {

// // // // void Assembling() {
// // // //     const size_t STATIC_SIZE = 5;
// // // //     int staticArr[STATIC_SIZE];

// // // //     std::cout << "\n============================================================\n";
// // // //     std::cout << "             АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ\n";
// // // //     std::cout << "============================================================\n";

// // // //     // Часть 1: Статический массив
// // // //     std::cout << "\nЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ (" << STATIC_SIZE << " элементов)\n";
// // // //     std::cout << "============================================================\n";

// // // //     fillRandom(staticArr, STATIC_SIZE);
// // // //     std::cout << "\nИсходный массив: ";
// // // //     printArray(staticArr, STATIC_SIZE);

// // // //     // Сортировка выбором
// // // //     {
// // // //         std::cout << "\n--------------- СОРТИРОВКА ВЫБОРОМ ---------------\n";
// // // //         std::cout << "Метод          Направление           Сравнения  Перестановки\n";
// // // //         std::cout << "------------------------------------------------------------\n";

// // // //         int arrCopy[STATIC_SIZE];
// // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // //         SortStats stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // //         std::cout << "Выбором        возр. (не отсорт.)    "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // //         std::cout << "Выбором        возр. (отсорт.)       "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         stats = selectionSort(arrCopy, STATIC_SIZE, false);
// // // //         std::cout << "Выбором        убыв.                 "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         std::cout << "------------------------------------------------------------\n";
// // // //         std::cout << "Массив после: ";
// // // //         printArray(arrCopy, STATIC_SIZE);
// // // //     }

// // // //     // Сортировка пузырьком
// // // //     {
// // // //         std::cout << "\n--------------- СОРТИРОВКА ПУЗЫРЬКОМ ---------------\n";
// // // //         std::cout << "Метод          Направление           Сравнения  Перестановки\n";
// // // //         std::cout << "------------------------------------------------------------\n";

// // // //         int arrCopy[STATIC_SIZE];
// // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // //         SortStats stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // //         std::cout << "Пузырьком      возр. (не отсорт.)    "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // //         std::cout << "Пузырьком      возр. (отсорт.)       "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, false);
// // // //         std::cout << "Пузырьком      убыв.                 "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         std::cout << "------------------------------------------------------------\n";
// // // //         std::cout << "Массив после: ";
// // // //         printArray(arrCopy, STATIC_SIZE);
// // // //     }

// // // //     // Быстрая сортировка
// // // //     {
// // // //         std::cout << "\n--------------- БЫСТРАЯ СОРТИРОВКА ---------------\n";
// // // //         std::cout << "Метод          Направление           Сравнения  Перестановки\n";
// // // //         std::cout << "------------------------------------------------------------\n";

// // // //         int arrCopy[STATIC_SIZE];
// // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // //         SortStats stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // //         std::cout << "Быстрая        возр. (не отсорт.)    "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // //         std::cout << "Быстрая        возр. (отсорт.)       "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         stats = quickSort(arrCopy, STATIC_SIZE, false);
// // // //         std::cout << "Быстрая        убыв.                 "
// // // //                   << std::setw(10) << stats.comparisons << "     "
// // // //                   << std::setw(10) << stats.swaps << "\n";

// // // //         std::cout << "------------------------------------------------------------\n";
// // // //         std::cout << "Массив после: ";
// // // //         printArray(arrCopy, STATIC_SIZE);
// // // //     }

// // // //     // Часть 2: Динамический массив
// // // //     std::cout << "\n============================================================\n";
// // // //     std::cout << "             ДИНАМИЧЕСКИЙ МАССИВ\n";
// // // //     std::cout << "============================================================\n";

// // // //     size_t dynamicSize;
// // // //     std::cout << "\nВведите размер динамического массива: ";
// // // //     std::cin >> dynamicSize;

// // // //     if (dynamicSize == 0) {
// // // //         std::cout << "Ошибка: размер должен быть > 0!\n";
// // // //         return;
// // // //     }

// // // //     int* dynamicArr = new int[dynamicSize];
// // // //     fillRandom(dynamicArr, dynamicSize);

// // // //     std::cout << "\nПервые 10 элементов: [";
// // // //     for (size_t i = 0; i < std::min(dynamicSize, size_t(10)); ++i) {
// // // //         std::cout << std::setw(3) << dynamicArr[i];
// // // //     }
// // // //     std::cout << "]";
// // // //     if (dynamicSize > 10) std::cout << " ... (всего " << dynamicSize << ")";
// // // //     std::cout << "\n";

// // // //     std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ (" << dynamicSize << " элементов)\n";
// // // //     std::cout << "============================================================\n";
// // // //     std::cout << "Метод          Направление    Сравнения      Перестановки\n";
// // // //     std::cout << "------------------------------------------------------------\n";

// // // //     // Сортировка выбором
// // // //     int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // // //     SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // // //     std::cout << "Выбором        возр.          "
// // // //               << std::setw(12) << stats1.comparisons << "     "
// // // //               << std::setw(12) << stats1.swaps << "\n";
// // // //     delete[] arrCopy1;

// // // //     // Сортировка пузырьком
// // // //     int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // // //     SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // // //     std::cout << "Пузырьком      возр.          "
// // // //               << std::setw(12) << stats2.comparisons << "     "
// // // //               << std::setw(12) << stats2.swaps << "\n";
// // // //     delete[] arrCopy2;

// // // //     // Быстрая сортировка
// // // //     int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // // //     SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // // //     std::cout << "Быстрая        возр.          "
// // // //               << std::setw(12) << stats3.comparisons << "     "
// // // //               << std::setw(12) << stats3.swaps << "\n";
// // // //     delete[] arrCopy3;

// // // //     std::cout << "============================================================\n";

// // // //     delete[] dynamicArr;

// // // //     std::cout << "\n============================================================\n";
// // // //     std::cout << "             РАБОТА ЗАВЕРШЕНА\n";
// // // //     std::cout << "============================================================\n";
// // // // }

// // // // } // namespace AssemblingSorting



// // // // // #include "sorting.hpp"
// // // // // #include <iostream>
// // // // // #include <iomanip>
// // // // // #include <random>
// // // // // #include <algorithm>
// // // // // #include <string>
// // // // // #include <cstddef>

// // // // // namespace {

// // // // // struct SortStats {
// // // // //     size_t comparisons = 0;
// // // // //     size_t swaps = 0;
// // // // // };

// // // // // // Функция вывода массива
// // // // // template<typename T>
// // // // // void printArray(T* arr, size_t size) {
// // // // //     std::cout << "[ ";
// // // // //     for (size_t i = 0; i < size; ++i) {
// // // // //         std::cout << std::setw(2) << arr[i] << " ";
// // // // //     }
// // // // //     std::cout << "]" << std::endl;
// // // // // }

// // // // // // Сортировка выбором
// // // // // template<typename T>
// // // // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // // // //     SortStats stats;

// // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // //         size_t extremeIndex = i;

// // // // //         for (size_t j = i + 1; j < size; ++j) {
// // // // //             stats.comparisons++;
// // // // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // // // //                 extremeIndex = j;
// // // // //             }
// // // // //         }

// // // // //         if (extremeIndex != i) {
// // // // //             std::swap(arr[i], arr[extremeIndex]);
// // // // //             stats.swaps++;
// // // // //         }
// // // // //     }

// // // // //     return stats;
// // // // // }

// // // // // // Сортировка пузырьком
// // // // // template<typename T>
// // // // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // // // //     SortStats stats;
// // // // //     bool swapped;

// // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // //         swapped = false;

// // // // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // // // //             stats.comparisons++;
// // // // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // // // //                 std::swap(arr[j], arr[j + 1]);
// // // // //                 stats.swaps++;
// // // // //                 swapped = true;
// // // // //             }
// // // // //         }

// // // // //         if (!swapped) break;
// // // // //     }

// // // // //     return stats;
// // // // // }

// // // // // // Быстрая сортировка (вспомогательные функции)
// // // // // template<typename T>
// // // // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // //     T pivot = arr[high];
// // // // //     size_t i = low;

// // // // //     for (size_t j = low; j < high; ++j) {
// // // // //         stats.comparisons++;
// // // // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // // // //             if (i != j) {
// // // // //                 std::swap(arr[i], arr[j]);
// // // // //                 stats.swaps++;
// // // // //             }
// // // // //             i++;
// // // // //         }
// // // // //     }

// // // // //     if (i != high) {
// // // // //         std::swap(arr[i], arr[high]);
// // // // //         stats.swaps++;
// // // // //     }

// // // // //     return i;
// // // // // }

// // // // // template<typename T>
// // // // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // //     if (low < high) {
// // // // //         size_t pi = partition(arr, low, high, ascending, stats);

// // // // //         if (pi > 0) {
// // // // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // // // //         }
// // // // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // // // //     }
// // // // // }

// // // // // // Быстрая сортировка
// // // // // template<typename T>
// // // // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // // // //     SortStats stats;
// // // // //     if (size > 1) {
// // // // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // // // //     }
// // // // //     return stats;
// // // // // }

// // // // // // Заполнение массива случайными числами
// // // // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // // // //     std::random_device rd;
// // // // //     std::default_random_engine engine(rd());
// // // // //     std::uniform_int_distribution<int> distribution(min, max);

// // // // //     for (size_t i = 0; i < size; ++i) {
// // // // //         arr[i] = distribution(engine);
// // // // //     }
// // // // // }

// // // // // // Копирование массива
// // // // // int* copyArray(const int* source, size_t size) {
// // // // //     int* copy = new int[size];
// // // // //     for (size_t i = 0; i < size; ++i) {
// // // // //         copy[i] = source[i];
// // // // //     }
// // // // //     return copy;
// // // // // }

// // // // // // Вывод таблицы
// // // // // void printTable(const std::string& title) {
// // // // //     std::cout << "\n" << title << std::endl;
// // // // //     std::cout << "------------------------------------------------------------" << std::endl;
// // // // //     std::cout << "| Метод        | Направление       | Сравнения | Перестановки |" << std::endl;
// // // // //     std::cout << "------------------------------------------------------------" << std::endl;
// // // // // }

// // // // // void printRow(const std::string& method, const std::string& dir, size_t comp, size_t swaps) {
// // // // //     std::cout << "| " << std::setw(12) << std::left << method
// // // // //               << "| " << std::setw(17) << dir
// // // // //               << "| " << std::setw(10) << comp
// // // // //               << "| " << std::setw(12) << swaps << " |" << std::endl;
// // // // // }

// // // // // // Явные инстанциации шаблонов для int
// // // // // template void printArray<int>(int* arr, size_t size);
// // // // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // // } // анонимное пространство имен

// // // // // namespace AssemblingSorting {

// // // // // void Assembling() {
// // // // //     const size_t STATIC_SIZE = 8;
// // // // //     int staticArr[STATIC_SIZE];

// // // // //     std::cout << "\n============================================================" << std::endl;
// // // // //     std::cout << "           АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ" << std::endl;
// // // // //     std::cout << "============================================================" << std::endl;

// // // // //     // Часть 1: Статический массив
// // // // //     std::cout << "\nЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ (" << STATIC_SIZE << " элементов)" << std::endl;
// // // // //     std::cout << "============================================================" << std::endl;

// // // // //     // Заполняем массив случайными числами
// // // // //     fillRandom(staticArr, STATIC_SIZE);

// // // // //     std::cout << "\nИсходный массив: ";
// // // // //     printArray(staticArr, STATIC_SIZE);

// // // // //     // Тестируем сортировку выбором
// // // // //     {
// // // // //         printTable("1. СОРТИРОВКА ВЫБОРОМ");

// // // // //         int arrCopy[STATIC_SIZE];
// // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // //         SortStats stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // //         printRow("Выбором", "возр. (не отсорт.)", stats.comparisons, stats.swaps);

// // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // //         printRow("Выбором", "возр. (отсорт.)", stats.comparisons, stats.swaps);

// // // // //         // 3. Сортировка по убыванию
// // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, false);
// // // // //         printRow("Выбором", "убыв.", stats.comparisons, stats.swaps);

// // // // //         std::cout << "------------------------------------------------------------" << std::endl;
// // // // //         std::cout << "Массив после всех операций: ";
// // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // //     }

// // // // //     // Тестируем сортировку пузырьком
// // // // //     {
// // // // //         printTable("2. СОРТИРОВКА ПУЗЫРЬКОМ");

// // // // //         int arrCopy[STATIC_SIZE];
// // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // //         SortStats stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // //         printRow("Пузырьком", "возр. (не отсорт.)", stats.comparisons, stats.swaps);

// // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // //         printRow("Пузырьком", "возр. (отсорт.)", stats.comparisons, stats.swaps);

// // // // //         // 3. Сортировка по убыванию
// // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, false);
// // // // //         printRow("Пузырьком", "убыв.", stats.comparisons, stats.swaps);

// // // // //         std::cout << "------------------------------------------------------------" << std::endl;
// // // // //         std::cout << "Массив после всех операций: ";
// // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // //     }

// // // // //     // Тестируем быструю сортировку
// // // // //     {
// // // // //         printTable("3. БЫСТРАЯ СОРТИРОВКА");

// // // // //         int arrCopy[STATIC_SIZE];
// // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // //         SortStats stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // //         printRow("Быстрая", "возр. (не отсорт.)", stats.comparisons, stats.swaps);

// // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // //         stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // //         printRow("Быстрая", "возр. (отсорт.)", stats.comparisons, stats.swaps);

// // // // //         // 3. Сортировка по убыванию
// // // // //         stats = quickSort(arrCopy, STATIC_SIZE, false);
// // // // //         printRow("Быстрая", "убыв.", stats.comparisons, stats.swaps);

// // // // //         std::cout << "------------------------------------------------------------" << std::endl;
// // // // //         std::cout << "Массив после всех операций: ";
// // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // //     }

// // // // //     // Часть 2: Динамический массив
// // // // //     std::cout << "\n============================================================" << std::endl;
// // // // //     std::cout << "           ДИНАМИЧЕСКИЙ МАССИВ" << std::endl;
// // // // //     std::cout << "============================================================" << std::endl;

// // // // //     size_t dynamicSize;
// // // // //     std::cout << "\nВведите размер динамического массива: ";
// // // // //     std::cin >> dynamicSize;

// // // // //     if (dynamicSize == 0) {
// // // // //         std::cout << "Ошибка: размер массива должен быть больше 0!" << std::endl;
// // // // //         return;
// // // // //     }

// // // // //     // Создаем и заполняем динамический массив
// // // // //     int* dynamicArr = new int[dynamicSize];
// // // // //     fillRandom(dynamicArr, dynamicSize);

// // // // //     std::cout << "\nПервые 10 элементов исходного массива:\n[ ";
// // // // //     for (size_t i = 0; i < std::min(dynamicSize, size_t(10)); ++i) {
// // // // //         std::cout << std::setw(2) << dynamicArr[i] << " ";
// // // // //     }
// // // // //     std::cout << "]";
// // // // //     if (dynamicSize > 10) {
// // // // //         std::cout << " ... (всего " << dynamicSize << " элементов)";
// // // // //     }
// // // // //     std::cout << std::endl;

// // // // //     std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ (" << dynamicSize << " элементов):" << std::endl;
// // // // //     std::cout << "------------------------------------------------------------" << std::endl;
// // // // //     std::cout << "| Метод        | Направление | Сравнения  | Перестановки   |" << std::endl;
// // // // //     std::cout << "------------------------------------------------------------" << std::endl;

// // // // //     // Сортировка выбором
// // // // //     int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // // // //     SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // // // //     printRow("Выбором", "возр.", stats1.comparisons, stats1.swaps);
// // // // //     delete[] arrCopy1;

// // // // //     // Сортировка пузырьком
// // // // //     int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // // // //     SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // // // //     printRow("Пузырьком", "возр.", stats2.comparisons, stats2.swaps);
// // // // //     delete[] arrCopy2;

// // // // //     // Быстрая сортировка
// // // // //     int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // // // //     SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // // // //     printRow("Быстрая", "возр.", stats3.comparisons, stats3.swaps);
// // // // //     delete[] arrCopy3;

// // // // //     std::cout << "------------------------------------------------------------" << std::endl;

// // // // //     // Освобождаем память
// // // // //     delete[] dynamicArr;

// // // // //     std::cout << "\n============================================================" << std::endl;
// // // // //     std::cout << "           РАБОТА ЗАВЕРШЕНА" << std::endl;
// // // // //     std::cout << "============================================================" << std::endl;
// // // // // }

// // // // // } // namespace AssemblingSorting




// // // // // // #include "sorting.hpp"
// // // // // // #include <iostream>
// // // // // // #include <iomanip>
// // // // // // #include <random>
// // // // // // #include <algorithm>
// // // // // // #include <string>
// // // // // // #include <cstddef>

// // // // // // namespace {

// // // // // // struct SortStats {
// // // // // //     size_t comparisons = 0;
// // // // // //     size_t swaps = 0;
// // // // // // };

// // // // // // // Функция вывода массива
// // // // // // template<typename T>
// // // // // // void printArray(T* arr, size_t size) {
// // // // // //     std::cout << "[";
// // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // //         std::cout << std::setw(3) << arr[i];
// // // // // //     }
// // // // // //     std::cout << " ]" << std::endl;
// // // // // // }

// // // // // // // Сортировка выбором
// // // // // // template<typename T>
// // // // // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // // // // //     SortStats stats;

// // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // //         size_t extremeIndex = i;

// // // // // //         for (size_t j = i + 1; j < size; ++j) {
// // // // // //             stats.comparisons++;
// // // // // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // // // // //                 extremeIndex = j;
// // // // // //             }
// // // // // //         }

// // // // // //         if (extremeIndex != i) {
// // // // // //             std::swap(arr[i], arr[extremeIndex]);
// // // // // //             stats.swaps++;
// // // // // //         }
// // // // // //     }

// // // // // //     return stats;
// // // // // // }

// // // // // // // Сортировка пузырьком
// // // // // // template<typename T>
// // // // // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // // // // //     SortStats stats;
// // // // // //     bool swapped;

// // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // //         swapped = false;

// // // // // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // // // // //             stats.comparisons++;
// // // // // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // // // // //                 std::swap(arr[j], arr[j + 1]);
// // // // // //                 stats.swaps++;
// // // // // //                 swapped = true;
// // // // // //             }
// // // // // //         }

// // // // // //         if (!swapped) break;
// // // // // //     }

// // // // // //     return stats;
// // // // // // }

// // // // // // // Быстрая сортировка (вспомогательные функции)
// // // // // // template<typename T>
// // // // // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // //     T pivot = arr[high];
// // // // // //     size_t i = low;

// // // // // //     for (size_t j = low; j < high; ++j) {
// // // // // //         stats.comparisons++;
// // // // // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // // // // //             if (i != j) {
// // // // // //                 std::swap(arr[i], arr[j]);
// // // // // //                 stats.swaps++;
// // // // // //             }
// // // // // //             i++;
// // // // // //         }
// // // // // //     }

// // // // // //     if (i != high) {
// // // // // //         std::swap(arr[i], arr[high]);
// // // // // //         stats.swaps++;
// // // // // //     }

// // // // // //     return i;
// // // // // // }

// // // // // // template<typename T>
// // // // // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // //     if (low < high) {
// // // // // //         size_t pi = partition(arr, low, high, ascending, stats);

// // // // // //         if (pi > 0) {
// // // // // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // // // // //         }
// // // // // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // // // // //     }
// // // // // // }

// // // // // // // Быстрая сортировка
// // // // // // template<typename T>
// // // // // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // // // // //     SortStats stats;
// // // // // //     if (size > 1) {
// // // // // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // // // // //     }
// // // // // //     return stats;
// // // // // // }

// // // // // // // Заполнение массива случайными числами
// // // // // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // // // // //     std::random_device rd;
// // // // // //     std::default_random_engine engine(rd());
// // // // // //     std::uniform_int_distribution<int> distribution(min, max);

// // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // //         arr[i] = distribution(engine);
// // // // // //     }
// // // // // // }

// // // // // // // Копирование массива
// // // // // // int* copyArray(const int* source, size_t size) {
// // // // // //     int* copy = new int[size];
// // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // //         copy[i] = source[i];
// // // // // //     }
// // // // // //     return copy;
// // // // // // }

// // // // // // // Простая таблица без рамок
// // // // // // void printSimpleTableHeader() {
// // // // // //     std::cout << std::endl;
// // // // // //     std::cout << std::setw(25) << std::left << "Метод сортировки"
// // // // // //               << std::setw(20) << "Направление"
// // // // // //               << std::setw(15) << "Сравнения"
// // // // // //               << std::setw(15) << "Перестановки"
// // // // // //               << std::endl;
// // // // // //     std::cout << std::string(75, '-') << std::endl;
// // // // // // }

// // // // // // // Вывод строки таблицы
// // // // // // void printSimpleTableRow(const std::string& method, const std::string& direction,
// // // // // //                          const SortStats& stats) {
// // // // // //     std::cout << std::setw(25) << std::left << method
// // // // // //               << std::setw(20) << direction
// // // // // //               << std::setw(15) << stats.comparisons
// // // // // //               << std::setw(15) << stats.swaps
// // // // // //               << std::endl;
// // // // // // }

// // // // // // // Явные инстанциации шаблонов для int
// // // // // // template void printArray<int>(int* arr, size_t size);
// // // // // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // // // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // // // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // // // } // анонимное пространство имен

// // // // // // namespace AssemblingSorting {

// // // // // // void Assembling() {
// // // // // //     const size_t STATIC_SIZE = 8;
// // // // // //     int staticArr[STATIC_SIZE];

// // // // // //     std::cout << "\n" << std::string(60, '=') << std::endl;
// // // // // //     std::cout << "     ЛАБОРАТОРНАЯ РАБОТА: АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ" << std::endl;
// // // // // //     std::cout << std::string(60, '=') << std::endl;

// // // // // //     // Часть 1: Статический массив
// // // // // //     std::cout << "\nЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ (" << STATIC_SIZE << " элементов)\n";
// // // // // //     std::cout << std::string(40, '-') << std::endl;

// // // // // //     // Заполняем массив случайными числами
// // // // // //     fillRandom(staticArr, STATIC_SIZE);

// // // // // //     std::cout << "\nИсходный массив: ";
// // // // // //     printArray(staticArr, STATIC_SIZE);

// // // // // //     // Тестируем сортировку выбором
// // // // // //     {
// // // // // //         std::cout << "\n1. СОРТИРОВКА ВЫБОРОМ:" << std::endl;
// // // // // //         printSimpleTableHeader();

// // // // // //         int arrCopy[STATIC_SIZE];
// // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // //         SortStats stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // //         printSimpleTableRow("Выбором", "ВВЕРХ (не отсорт.)", stats);

// // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // //         printSimpleTableRow("Выбором", "ВВЕРХ (отсорт.)", stats);

// // // // // //         // 3. Сортировка по убыванию
// // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, false);
// // // // // //         printSimpleTableRow("Выбором", "ВНИЗ", stats);

// // // // // //         std::cout << "\nМассив после всех операций: ";
// // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // //     }

// // // // // //     // Тестируем сортировку пузырьком
// // // // // //     {
// // // // // //         std::cout << "\n2. СОРТИРОВКА ПУЗЫРЬКОМ:" << std::endl;
// // // // // //         printSimpleTableHeader();

// // // // // //         int arrCopy[STATIC_SIZE];
// // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // //         SortStats stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // //         printSimpleTableRow("Пузырьком", "ВВЕРХ (не отсорт.)", stats);

// // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // //         printSimpleTableRow("Пузырьком", "ВВЕРХ (отсорт.)", stats);

// // // // // //         // 3. Сортировка по убыванию
// // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, false);
// // // // // //         printSimpleTableRow("Пузырьком", "ВНИЗ", stats);

// // // // // //         std::cout << "\nМассив после всех операций: ";
// // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // //     }

// // // // // //     // Тестируем быструю сортировку
// // // // // //     {
// // // // // //         std::cout << "\n3. БЫСТРАЯ СОРТИРОВКА:" << std::endl;
// // // // // //         printSimpleTableHeader();

// // // // // //         int arrCopy[STATIC_SIZE];
// // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // //         SortStats stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // //         printSimpleTableRow("Быстрая", "ВВЕРХ (не отсорт.)", stats);

// // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // //         printSimpleTableRow("Быстрая", "ВВЕРХ (отсорт.)", stats);

// // // // // //         // 3. Сортировка по убыванию
// // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, false);
// // // // // //         printSimpleTableRow("Быстрая", "ВНИЗ", stats);

// // // // // //         std::cout << "\nМассив после всех операций: ";
// // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // //     }

// // // // // //     // Часть 2: Динамический массив
// // // // // //     std::cout << "\n" << std::string(60, '=') << std::endl;
// // // // // //     std::cout << "          ЧАСТЬ 2: ДИНАМИЧЕСКИЙ МАССИВ" << std::endl;
// // // // // //     std::cout << std::string(60, '=') << std::endl;

// // // // // //     size_t dynamicSize;
// // // // // //     std::cout << "\nВведите размер динамического массива: ";
// // // // // //     std::cin >> dynamicSize;

// // // // // //     if (dynamicSize == 0) {
// // // // // //         std::cout << "Ошибка: размер массива должен быть больше 0!" << std::endl;
// // // // // //         return;
// // // // // //     }

// // // // // //     // Создаем и заполняем динамический массив
// // // // // //     int* dynamicArr = new int[dynamicSize];
// // // // // //     fillRandom(dynamicArr, dynamicSize);

// // // // // //     std::cout << "\nПервые 10 элементов исходного массива:\n[";
// // // // // //     for (size_t i = 0; i < std::min(dynamicSize, size_t(10)); ++i) {
// // // // // //         std::cout << std::setw(3) << dynamicArr[i];
// // // // // //     }
// // // // // //     std::cout << " ]";
// // // // // //     if (dynamicSize > 10) {
// // // // // //         std::cout << " ... (всего " << dynamicSize << " элементов)";
// // // // // //     }
// // // // // //     std::cout << std::endl;

// // // // // //     std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ ДИНАМИЧЕСКОГО МАССИВА ("
// // // // // //               << dynamicSize << " элементов):" << std::endl;
// // // // // //     printSimpleTableHeader();

// // // // // //     // Сортировка выбором
// // // // // //     int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // // // // //     SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // // // // //     printSimpleTableRow("Выбором", "ВВЕРХ", stats1);
// // // // // //     delete[] arrCopy1;

// // // // // //     // Сортировка пузырьком
// // // // // //     int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // // // // //     SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // // // // //     printSimpleTableRow("Пузырьком", "ВВЕРХ", stats2);
// // // // // //     delete[] arrCopy2;

// // // // // //     // Быстрая сортировка
// // // // // //     int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // // // // //     SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // // // // //     printSimpleTableRow("Быстрая", "ВВЕРХ", stats3);
// // // // // //     delete[] arrCopy3;

// // // // // //     // Освобождаем память
// // // // // //     delete[] dynamicArr;

// // // // // //     std::cout << "\n" << std::string(60, '=') << std::endl;
// // // // // //     std::cout << "          РАБОТА ПРОГРАММЫ ЗАВЕРШЕНА" << std::endl;
// // // // // //     std::cout << std::string(60, '=') << std::endl;
// // // // // // }

// // // // // // } // namespace AssemblingSorting




// // // // // // // #include "sorting.hpp"
// // // // // // // #include <iostream>
// // // // // // // #include <iomanip>
// // // // // // // #include <random>
// // // // // // // #include <algorithm>
// // // // // // // #include <string>
// // // // // // // #include <cstddef>

// // // // // // // namespace {

// // // // // // // struct SortStats {
// // // // // // //     size_t comparisons = 0;
// // // // // // //     size_t swaps = 0;
// // // // // // // };

// // // // // // // // Функция вывода массива
// // // // // // // template<typename T>
// // // // // // // void printArray(T* arr, size_t size) {
// // // // // // //     std::cout << "[";
// // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // //         std::cout << std::setw(3) << arr[i];
// // // // // // //     }
// // // // // // //     std::cout << " ]" << std::endl;
// // // // // // // }

// // // // // // // // Сортировка выбором
// // // // // // // template<typename T>
// // // // // // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // // // // // //     SortStats stats;

// // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // //         size_t extremeIndex = i;

// // // // // // //         for (size_t j = i + 1; j < size; ++j) {
// // // // // // //             stats.comparisons++;
// // // // // // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // // // // // //                 extremeIndex = j;
// // // // // // //             }
// // // // // // //         }

// // // // // // //         if (extremeIndex != i) {
// // // // // // //             std::swap(arr[i], arr[extremeIndex]);
// // // // // // //             stats.swaps++;
// // // // // // //         }
// // // // // // //     }

// // // // // // //     return stats;
// // // // // // // }

// // // // // // // // Сортировка пузырьком
// // // // // // // template<typename T>
// // // // // // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // // // // // //     SortStats stats;
// // // // // // //     bool swapped;

// // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // //         swapped = false;

// // // // // // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // // // // // //             stats.comparisons++;
// // // // // // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // // // // // //                 std::swap(arr[j], arr[j + 1]);
// // // // // // //                 stats.swaps++;
// // // // // // //                 swapped = true;
// // // // // // //             }
// // // // // // //         }

// // // // // // //         if (!swapped) break;
// // // // // // //     }

// // // // // // //     return stats;
// // // // // // // }

// // // // // // // // Быстрая сортировка (вспомогательные функции)
// // // // // // // template<typename T>
// // // // // // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // //     T pivot = arr[high];
// // // // // // //     size_t i = low;

// // // // // // //     for (size_t j = low; j < high; ++j) {
// // // // // // //         stats.comparisons++;
// // // // // // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // // // // // //             if (i != j) {
// // // // // // //                 std::swap(arr[i], arr[j]);
// // // // // // //                 stats.swaps++;
// // // // // // //             }
// // // // // // //             i++;
// // // // // // //         }
// // // // // // //     }

// // // // // // //     if (i != high) {
// // // // // // //         std::swap(arr[i], arr[high]);
// // // // // // //         stats.swaps++;
// // // // // // //     }

// // // // // // //     return i;
// // // // // // // }

// // // // // // // template<typename T>
// // // // // // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // //     if (low < high) {
// // // // // // //         size_t pi = partition(arr, low, high, ascending, stats);

// // // // // // //         if (pi > 0) {
// // // // // // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // // // // // //         }
// // // // // // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // // // // // //     }
// // // // // // // }

// // // // // // // // Быстрая сортировка
// // // // // // // template<typename T>
// // // // // // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // // // // // //     SortStats stats;
// // // // // // //     if (size > 1) {
// // // // // // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // // // // // //     }
// // // // // // //     return stats;
// // // // // // // }

// // // // // // // // Заполнение массива случайными числами
// // // // // // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // // // // // //     std::random_device rd;
// // // // // // //     std::default_random_engine engine(rd());
// // // // // // //     std::uniform_int_distribution<int> distribution(min, max);

// // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // //         arr[i] = distribution(engine);
// // // // // // //     }
// // // // // // // }

// // // // // // // // Копирование массива
// // // // // // // int* copyArray(const int* source, size_t size) {
// // // // // // //     int* copy = new int[size];
// // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // //         copy[i] = source[i];
// // // // // // //     }
// // // // // // //     return copy;
// // // // // // // }

// // // // // // // // Вывод горизонтальной линии
// // // // // // // void printHorizontalLine(int width, char symbol = '-') {
// // // // // // //     std::cout << std::string(width, symbol) << std::endl;
// // // // // // // }

// // // // // // // // Вывод заголовка таблицы
// // // // // // // void printTableHeader() {
// // // // // // //     std::cout << "+----------------------+------------------+------------------+------------------+" << std::endl;
// // // // // // //     std::cout << "| Метод сортировки    | Направление      | Сравнения        | Перестановки     |" << std::endl;
// // // // // // //     std::cout << "+----------------------+------------------+------------------+------------------+" << std::endl;
// // // // // // // }

// // // // // // // // Вывод строки таблицы
// // // // // // // void printTableRow(const std::string& method, const std::string& direction,
// // // // // // //                    const SortStats& stats) {
// // // // // // //     std::cout << "| " << std::setw(20) << std::left << method
// // // // // // //               << "| " << std::setw(16) << direction
// // // // // // //               << "| " << std::setw(16) << stats.comparisons
// // // // // // //               << "| " << std::setw(16) << stats.swaps
// // // // // // //               << "|" << std::endl;
// // // // // // //     std::cout << "+----------------------+------------------+------------------+------------------+" << std::endl;
// // // // // // // }

// // // // // // // // Вывод строки таблицы без нижней линии
// // // // // // // void printTableRowNoLine(const std::string& method, const std::string& direction,
// // // // // // //                          const SortStats& stats) {
// // // // // // //     std::cout << "| " << std::setw(20) << std::left << method
// // // // // // //               << "| " << std::setw(16) << direction
// // // // // // //               << "| " << std::setw(16) << stats.comparisons
// // // // // // //               << "| " << std::setw(16) << stats.swaps
// // // // // // //               << "|" << std::endl;
// // // // // // // }

// // // // // // // // Явные инстанциации шаблонов для int
// // // // // // // template void printArray<int>(int* arr, size_t size);
// // // // // // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // // // // } // анонимное пространство имен

// // // // // // // namespace AssemblingSorting {

// // // // // // // void Assembling() {
// // // // // // //     const size_t STATIC_SIZE = 8;
// // // // // // //     int staticArr[STATIC_SIZE];

// // // // // // //     std::cout << "\n";
// // // // // // //     printHorizontalLine(70, '=');
// // // // // // //     std::cout << "               ЛАБОРАТОРНАЯ РАБОТА: АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ" << std::endl;
// // // // // // //     printHorizontalLine(70, '=');

// // // // // // //     // Часть 1: Статический массив
// // // // // // //     std::cout << "\nЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ (" << STATIC_SIZE << " элементов)\n";
// // // // // // //     printHorizontalLine(50);

// // // // // // //     // Заполняем массив случайными числами
// // // // // // //     fillRandom(staticArr, STATIC_SIZE);

// // // // // // //     std::cout << "\nИсходный массив: ";
// // // // // // //     printArray(staticArr, STATIC_SIZE);
// // // // // // //     std::cout << std::endl;

// // // // // // //     // Тестируем сортировку выбором
// // // // // // //     {
// // // // // // //         std::cout << "1. СОРТИРОВКА ВЫБОРОМ:\n" << std::endl;
// // // // // // //         printTableHeader();

// // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // //         SortStats stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // //         printTableRowNoLine("Выбором", "ВВЕРХ (не отсорт.)", stats);

// // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // //         printTableRowNoLine("Выбором", "ВВЕРХ (отсорт.)", stats);

// // // // // // //         // 3. Сортировка по убыванию
// // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, false);
// // // // // // //         printTableRow("Выбором", "ВНИЗ", stats);

// // // // // // //         std::cout << "\nМассив после всех операций: ";
// // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // //         std::cout << std::endl;
// // // // // // //     }

// // // // // // //     // Тестируем сортировку пузырьком
// // // // // // //     {
// // // // // // //         std::cout << "2. СОРТИРОВКА ПУЗЫРЬКОМ:\n" << std::endl;
// // // // // // //         printTableHeader();

// // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // //         SortStats stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // //         printTableRowNoLine("Пузырьком", "ВВЕРХ (не отсорт.)", stats);

// // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // //         printTableRowNoLine("Пузырьком", "ВВЕРХ (отсорт.)", stats);

// // // // // // //         // 3. Сортировка по убыванию
// // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, false);
// // // // // // //         printTableRow("Пузырьком", "ВНИЗ", stats);

// // // // // // //         std::cout << "\nМассив после всех операций: ";
// // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // //         std::cout << std::endl;
// // // // // // //     }

// // // // // // //     // Тестируем быструю сортировку
// // // // // // //     {
// // // // // // //         std::cout << "3. БЫСТРАЯ СОРТИРОВКА:\n" << std::endl;
// // // // // // //         printTableHeader();

// // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // //         SortStats stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // //         printTableRowNoLine("Быстрая", "ВВЕРХ (не отсорт.)", stats);

// // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // //         printTableRowNoLine("Быстрая", "ВВЕРХ (отсорт.)", stats);

// // // // // // //         // 3. Сортировка по убыванию
// // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, false);
// // // // // // //         printTableRow("Быстрая", "ВНИЗ", stats);

// // // // // // //         std::cout << "\nМассив после всех операций: ";
// // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // //         std::cout << std::endl;
// // // // // // //     }

// // // // // // //     // Часть 2: Динамический массив
// // // // // // //     printHorizontalLine(70, '=');
// // // // // // //     std::cout << "                      ЧАСТЬ 2: ДИНАМИЧЕСКИЙ МАССИВ" << std::endl;
// // // // // // //     printHorizontalLine(70, '=');

// // // // // // //     size_t dynamicSize;
// // // // // // //     std::cout << "\nВведите размер динамического массива: ";
// // // // // // //     std::cin >> dynamicSize;

// // // // // // //     if (dynamicSize == 0) {
// // // // // // //         std::cout << "Ошибка: размер массива должен быть больше 0!" << std::endl;
// // // // // // //         return;
// // // // // // //     }

// // // // // // //     // Создаем и заполняем динамический массив
// // // // // // //     int* dynamicArr = new int[dynamicSize];
// // // // // // //     fillRandom(dynamicArr, dynamicSize);

// // // // // // //     std::cout << "\nПервые 10 элементов исходного массива:\n[";
// // // // // // //     for (size_t i = 0; i < std::min(dynamicSize, size_t(10)); ++i) {
// // // // // // //         std::cout << std::setw(3) << dynamicArr[i];
// // // // // // //     }
// // // // // // //     std::cout << " ]";
// // // // // // //     if (dynamicSize > 10) {
// // // // // // //         std::cout << " ... (всего " << dynamicSize << " элементов)";
// // // // // // //     }
// // // // // // //     std::cout << std::endl;

// // // // // // //     std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ ДИНАМИЧЕСКОГО МАССИВА ("
// // // // // // //               << dynamicSize << " элементов):\n" << std::endl;
// // // // // // //     printTableHeader();

// // // // // // //     // Сортировка выбором
// // // // // // //     int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // // // // // //     SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // // // // // //     printTableRowNoLine("Выбором", "ВВЕРХ", stats1);
// // // // // // //     delete[] arrCopy1;

// // // // // // //     // Сортировка пузырьком
// // // // // // //     int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // // // // // //     SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // // // // // //     printTableRowNoLine("Пузырьком", "ВВЕРХ", stats2);
// // // // // // //     delete[] arrCopy2;

// // // // // // //     // Быстрая сортировка
// // // // // // //     int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // // // // // //     SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // // // // // //     printTableRow("Быстрая", "ВВЕРХ", stats3);
// // // // // // //     delete[] arrCopy3;

// // // // // // //     // Освобождаем память
// // // // // // //     delete[] dynamicArr;

// // // // // // //     std::cout << "\n";
// // // // // // //     printHorizontalLine(70, '=');
// // // // // // //     std::cout << "                     РАБОТА ПРОГРАММЫ ЗАВЕРШЕНА" << std::endl;
// // // // // // //     printHorizontalLine(70, '=');
// // // // // // // }

// // // // // // // } // namespace AssemblingSorting





// // // // // // // // #include "sorting.hpp"
// // // // // // // // #include <iostream>
// // // // // // // // #include <iomanip>
// // // // // // // // #include <random>
// // // // // // // // #include <algorithm>
// // // // // // // // #include <string>
// // // // // // // // #include <cstddef>
// // // // // // // // #include <locale>

// // // // // // // // namespace {

// // // // // // // // struct SortStats {
// // // // // // // //     size_t comparisons = 0;
// // // // // // // //     size_t swaps = 0;
// // // // // // // // };

// // // // // // // // // Функция вывода массива
// // // // // // // // template<typename T>
// // // // // // // // void printArray(T* arr, size_t size) {
// // // // // // // //     std::cout << "[ ";
// // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // //         std::cout << std::setw(3) << arr[i] << " ";
// // // // // // // //     }
// // // // // // // //     std::cout << "]" << std::endl;
// // // // // // // // }

// // // // // // // // // Сортировка выбором
// // // // // // // // template<typename T>
// // // // // // // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // //     SortStats stats;

// // // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // // //         size_t extremeIndex = i;

// // // // // // // //         for (size_t j = i + 1; j < size; ++j) {
// // // // // // // //             stats.comparisons++;
// // // // // // // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // // // // // // //                 extremeIndex = j;
// // // // // // // //             }
// // // // // // // //         }

// // // // // // // //         if (extremeIndex != i) {
// // // // // // // //             std::swap(arr[i], arr[extremeIndex]);
// // // // // // // //             stats.swaps++;
// // // // // // // //         }
// // // // // // // //     }

// // // // // // // //     return stats;
// // // // // // // // }

// // // // // // // // // Сортировка пузырьком
// // // // // // // // template<typename T>
// // // // // // // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // //     SortStats stats;
// // // // // // // //     bool swapped;

// // // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // // //         swapped = false;

// // // // // // // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // // // // // // //             stats.comparisons++;
// // // // // // // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // // // // // // //                 std::swap(arr[j], arr[j + 1]);
// // // // // // // //                 stats.swaps++;
// // // // // // // //                 swapped = true;
// // // // // // // //             }
// // // // // // // //         }

// // // // // // // //         if (!swapped) break;
// // // // // // // //     }

// // // // // // // //     return stats;
// // // // // // // // }

// // // // // // // // // Быстрая сортировка (вспомогательные функции)
// // // // // // // // template<typename T>
// // // // // // // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // // //     T pivot = arr[high];
// // // // // // // //     size_t i = low;

// // // // // // // //     for (size_t j = low; j < high; ++j) {
// // // // // // // //         stats.comparisons++;
// // // // // // // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // // // // // // //             if (i != j) {
// // // // // // // //                 std::swap(arr[i], arr[j]);
// // // // // // // //                 stats.swaps++;
// // // // // // // //             }
// // // // // // // //             i++;
// // // // // // // //         }
// // // // // // // //     }

// // // // // // // //     if (i != high) {
// // // // // // // //         std::swap(arr[i], arr[high]);
// // // // // // // //         stats.swaps++;
// // // // // // // //     }

// // // // // // // //     return i;
// // // // // // // // }

// // // // // // // // template<typename T>
// // // // // // // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // // //     if (low < high) {
// // // // // // // //         size_t pi = partition(arr, low, high, ascending, stats);

// // // // // // // //         if (pi > 0) {
// // // // // // // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // // // // // // //         }
// // // // // // // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // // // // // // //     }
// // // // // // // // }

// // // // // // // // // Быстрая сортировка
// // // // // // // // template<typename T>
// // // // // // // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // //     SortStats stats;
// // // // // // // //     if (size > 1) {
// // // // // // // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // // // // // // //     }
// // // // // // // //     return stats;
// // // // // // // // }

// // // // // // // // // Заполнение массива случайными числами
// // // // // // // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // // // // // // //     std::random_device rd;
// // // // // // // //     std::default_random_engine engine(rd());
// // // // // // // //     std::uniform_int_distribution<int> distribution(min, max);

// // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // //         arr[i] = distribution(engine);
// // // // // // // //     }
// // // // // // // // }

// // // // // // // // // Копирование массива
// // // // // // // // int* copyArray(const int* source, size_t size) {
// // // // // // // //     int* copy = new int[size];
// // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // //         copy[i] = source[i];
// // // // // // // //     }
// // // // // // // //     return copy;
// // // // // // // // }

// // // // // // // // // Вывод заголовка таблицы
// // // // // // // // void printTableHeader() {
// // // // // // // //     std::cout << std::string(86, '=') << std::endl;
// // // // // // // //     std::cout << "| " << std::setw(22) << std::left << "Метод сортировки"
// // // // // // // //               << " | " << std::setw(18) << "Направление"
// // // // // // // //               << " | " << std::setw(18) << "Сравнения"
// // // // // // // //               << " | " << std::setw(18) << "Перестановки"
// // // // // // // //               << " |" << std::endl;
// // // // // // // //     std::cout << std::string(86, '=') << std::endl;
// // // // // // // // }

// // // // // // // // // Вывод разделителя строк
// // // // // // // // void printTableSeparator() {
// // // // // // // //     std::cout << "|" << std::string(84, '-') << "|" << std::endl;
// // // // // // // // }

// // // // // // // // // Вывод строки таблицы
// // // // // // // // void printTableRow(const std::string& method, const std::string& direction,
// // // // // // // //                    const SortStats& stats) {
// // // // // // // //     std::cout << "| " << std::setw(22) << std::left << method
// // // // // // // //               << " | " << std::setw(18) << direction
// // // // // // // //               << " | " << std::setw(18) << stats.comparisons
// // // // // // // //               << " | " << std::setw(18) << stats.swaps
// // // // // // // //               << " |" << std::endl;
// // // // // // // // }

// // // // // // // // // Вывод итогов таблицы
// // // // // // // // void printTableFooter() {
// // // // // // // //     std::cout << std::string(86, '=') << std::endl << std::endl;
// // // // // // // // }

// // // // // // // // // Явные инстанциации шаблонов для int
// // // // // // // // template void printArray<int>(int* arr, size_t size);
// // // // // // // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // // // // // } // анонимное пространство имен

// // // // // // // // namespace AssemblingSorting {

// // // // // // // // void Assembling() {
// // // // // // // //     // Устанавливаем локаль для поддержки кириллицы
// // // // // // // //     std::locale::global(std::locale(""));
// // // // // // // //     std::wcout.imbue(std::locale());

// // // // // // // //     const size_t STATIC_SIZE = 8;
// // // // // // // //     int staticArr[STATIC_SIZE];

// // // // // // // //     std::cout << "\n" << std::string(70, '=') << std::endl;
// // // // // // // //     std::cout << "ЛАБОРАТОРНАЯ РАБОТА: АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ" << std::endl;
// // // // // // // //     std::cout << std::string(70, '=') << std::endl << std::endl;

// // // // // // // //     // Часть 1: Статический массив
// // // // // // // //     std::cout << "ЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ (" << STATIC_SIZE << " элементов)" << std::endl;
// // // // // // // //     std::cout << std::string(60, '-') << std::endl;

// // // // // // // //     // Заполняем массив случайными числами
// // // // // // // //     fillRandom(staticArr, STATIC_SIZE);

// // // // // // // //     std::cout << "\nИсходный массив: ";
// // // // // // // //     printArray(staticArr, STATIC_SIZE);
// // // // // // // //     std::cout << std::endl;

// // // // // // // //     // Тестируем сортировку выбором
// // // // // // // //     {
// // // // // // // //         std::cout << "\n1. СОРТИРОВКА ВЫБОРОМ:" << std::endl;
// // // // // // // //         printTableHeader();

// // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // //         SortStats stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // // //         printTableRow("Выбором", "↑ (не отсорт.)", stats);
// // // // // // // //         printTableSeparator();

// // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // // //         printTableRow("Выбором", "↑ (отсорт.)", stats);
// // // // // // // //         printTableSeparator();

// // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, false);
// // // // // // // //         printTableRow("Выбором", "↓", stats);
// // // // // // // //         printTableFooter();

// // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // //         std::cout << std::endl;
// // // // // // // //     }

// // // // // // // //     // Тестируем сортировку пузырьком
// // // // // // // //     {
// // // // // // // //         std::cout << "\n2. СОРТИРОВКА ПУЗЫРЬКОМ:" << std::endl;
// // // // // // // //         printTableHeader();

// // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // //         SortStats stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // // //         printTableRow("Пузырьком", "↑ (не отсорт.)", stats);
// // // // // // // //         printTableSeparator();

// // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // // //         printTableRow("Пузырьком", "↑ (отсорт.)", stats);
// // // // // // // //         printTableSeparator();

// // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, false);
// // // // // // // //         printTableRow("Пузырьком", "↓", stats);
// // // // // // // //         printTableFooter();

// // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // //         std::cout << std::endl;
// // // // // // // //     }

// // // // // // // //     // Тестируем быструю сортировку
// // // // // // // //     {
// // // // // // // //         std::cout << "\n3. БЫСТРАЯ СОРТИРОВКА:" << std::endl;
// // // // // // // //         printTableHeader();

// // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // //         SortStats stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // // //         printTableRow("Быстрая", "↑ (не отсорт.)", stats);
// // // // // // // //         printTableSeparator();

// // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // // //         printTableRow("Быстрая", "↑ (отсорт.)", stats);
// // // // // // // //         printTableSeparator();

// // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, false);
// // // // // // // //         printTableRow("Быстрая", "↓", stats);
// // // // // // // //         printTableFooter();

// // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // //         std::cout << std::endl;
// // // // // // // //     }

// // // // // // // //     // Часть 2: Динамический массив
// // // // // // // //     std::cout << std::string(70, '=') << std::endl;
// // // // // // // //     std::cout << "ЧАСТЬ 2: ДИНАМИЧЕСКИЙ МАССИВ" << std::endl;
// // // // // // // //     std::cout << std::string(70, '=') << std::endl;

// // // // // // // //     size_t dynamicSize;
// // // // // // // //     std::cout << "\nВведите размер динамического массива: ";
// // // // // // // //     std::cin >> dynamicSize;

// // // // // // // //     if (dynamicSize == 0) {
// // // // // // // //         std::cout << "Ошибка: размер массива должен быть больше 0!" << std::endl;
// // // // // // // //         return;
// // // // // // // //     }

// // // // // // // //     // Создаем и заполняем динамический массив
// // // // // // // //     int* dynamicArr = new int[dynamicSize];
// // // // // // // //     fillRandom(dynamicArr, dynamicSize);

// // // // // // // //     std::cout << "\nПервые 10 элементов исходного массива:\n";
// // // // // // // //     std::cout << "[ ";
// // // // // // // //     for (size_t i = 0; i < std::min(dynamicSize, size_t(10)); ++i) {
// // // // // // // //         std::cout << std::setw(3) << dynamicArr[i] << " ";
// // // // // // // //     }
// // // // // // // //     std::cout << "]";
// // // // // // // //     if (dynamicSize > 10) {
// // // // // // // //         std::cout << " ... (всего " << dynamicSize << " элементов)";
// // // // // // // //     }
// // // // // // // //     std::cout << std::endl;

// // // // // // // //     std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ ДИНАМИЧЕСКОГО МАССИВА ("
// // // // // // // //               << dynamicSize << " элементов):" << std::endl;
// // // // // // // //     printTableHeader();

// // // // // // // //     // Сортировка выбором
// // // // // // // //     int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // // // // // // //     SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // // // // // // //     printTableRow("Выбором", "↑", stats1);
// // // // // // // //     printTableSeparator();
// // // // // // // //     delete[] arrCopy1;

// // // // // // // //     // Сортировка пузырьком
// // // // // // // //     int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // // // // // // //     SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // // // // // // //     printTableRow("Пузырьком", "↑", stats2);
// // // // // // // //     printTableSeparator();
// // // // // // // //     delete[] arrCopy2;

// // // // // // // //     // Быстрая сортировка
// // // // // // // //     int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // // // // // // //     SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // // // // // // //     printTableRow("Быстрая", "↑", stats3);
// // // // // // // //     delete[] arrCopy3;

// // // // // // // //     printTableFooter();

// // // // // // // //     // Освобождаем память
// // // // // // // //     delete[] dynamicArr;

// // // // // // // //     std::cout << std::string(70, '=') << std::endl;
// // // // // // // //     std::cout << "РАБОТА ПРОГРАММЫ ЗАВЕРШЕНА" << std::endl;
// // // // // // // //     std::cout << std::string(70, '=') << std::endl;
// // // // // // // // }

// // // // // // // // } // namespace AssemblingSorting





// // // // // // // // // #include "sorting.hpp"
// // // // // // // // // #include <iostream>
// // // // // // // // // #include <iomanip>
// // // // // // // // // #include <random>
// // // // // // // // // #include <algorithm>
// // // // // // // // // #include <string>
// // // // // // // // // #include <cstddef>
// // // // // // // // // #include <locale>

// // // // // // // // // namespace {

// // // // // // // // // struct SortStats {
// // // // // // // // //     size_t comparisons = 0;
// // // // // // // // //     size_t swaps = 0;
// // // // // // // // // };

// // // // // // // // // // Функция вывода массива
// // // // // // // // // template<typename T>
// // // // // // // // // void printArray(T* arr, size_t size) {
// // // // // // // // //     std::cout << "[ ";
// // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // //         std::cout << std::setw(3) << arr[i] << " ";
// // // // // // // // //     }
// // // // // // // // //     std::cout << "]" << std::endl;
// // // // // // // // // }

// // // // // // // // // // Сортировка выбором
// // // // // // // // // template<typename T>
// // // // // // // // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // //     SortStats stats;

// // // // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // // // //         size_t extremeIndex = i;

// // // // // // // // //         for (size_t j = i + 1; j < size; ++j) {
// // // // // // // // //             stats.comparisons++;
// // // // // // // // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // // // // // // // //                 extremeIndex = j;
// // // // // // // // //             }
// // // // // // // // //         }

// // // // // // // // //         if (extremeIndex != i) {
// // // // // // // // //             std::swap(arr[i], arr[extremeIndex]);
// // // // // // // // //             stats.swaps++;
// // // // // // // // //         }
// // // // // // // // //     }

// // // // // // // // //     return stats;
// // // // // // // // // }

// // // // // // // // // // Сортировка пузырьком
// // // // // // // // // template<typename T>
// // // // // // // // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // //     SortStats stats;
// // // // // // // // //     bool swapped;

// // // // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // // // //         swapped = false;

// // // // // // // // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // // // // // // // //             stats.comparisons++;
// // // // // // // // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // // // // // // // //                 std::swap(arr[j], arr[j + 1]);
// // // // // // // // //                 stats.swaps++;
// // // // // // // // //                 swapped = true;
// // // // // // // // //             }
// // // // // // // // //         }

// // // // // // // // //         if (!swapped) break;
// // // // // // // // //     }

// // // // // // // // //     return stats;
// // // // // // // // // }

// // // // // // // // // // Быстрая сортировка (вспомогательные функции)
// // // // // // // // // template<typename T>
// // // // // // // // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // // // //     T pivot = arr[high];
// // // // // // // // //     size_t i = low;

// // // // // // // // //     for (size_t j = low; j < high; ++j) {
// // // // // // // // //         stats.comparisons++;
// // // // // // // // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // // // // // // // //             if (i != j) {
// // // // // // // // //                 std::swap(arr[i], arr[j]);
// // // // // // // // //                 stats.swaps++;
// // // // // // // // //             }
// // // // // // // // //             i++;
// // // // // // // // //         }
// // // // // // // // //     }

// // // // // // // // //     if (i != high) {
// // // // // // // // //         std::swap(arr[i], arr[high]);
// // // // // // // // //         stats.swaps++;
// // // // // // // // //     }

// // // // // // // // //     return i;
// // // // // // // // // }

// // // // // // // // // template<typename T>
// // // // // // // // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // // // //     if (low < high) {
// // // // // // // // //         size_t pi = partition(arr, low, high, ascending, stats);

// // // // // // // // //         if (pi > 0) {
// // // // // // // // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // // // // // // // //         }
// // // // // // // // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // // // // // // // //     }
// // // // // // // // // }

// // // // // // // // // // Быстрая сортировка
// // // // // // // // // template<typename T>
// // // // // // // // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // //     SortStats stats;
// // // // // // // // //     if (size > 1) {
// // // // // // // // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // // // // // // // //     }
// // // // // // // // //     return stats;
// // // // // // // // // }

// // // // // // // // // // Заполнение массива случайными числами
// // // // // // // // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // // // // // // // //     std::random_device rd;
// // // // // // // // //     std::default_random_engine engine(rd());
// // // // // // // // //     std::uniform_int_distribution<int> distribution(min, max);

// // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // //         arr[i] = distribution(engine);
// // // // // // // // //     }
// // // // // // // // // }

// // // // // // // // // // Копирование массива
// // // // // // // // // int* copyArray(const int* source, size_t size) {
// // // // // // // // //     int* copy = new int[size];
// // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // //         copy[i] = source[i];
// // // // // // // // //     }
// // // // // // // // //     return copy;
// // // // // // // // // }

// // // // // // // // // // Вывод заголовка таблицы
// // // // // // // // // void printTableHeader() {
// // // // // // // // //     std::cout << std::string(86, '=') << std::endl;
// // // // // // // // //     std::cout << "| " << std::setw(22) << std::left << "Метод сортировки"
// // // // // // // // //               << " | " << std::setw(18) << "Направление"
// // // // // // // // //               << " | " << std::setw(18) << "Сравнения"
// // // // // // // // //               << " | " << std::setw(18) << "Перестановки"
// // // // // // // // //               << " |" << std::endl;
// // // // // // // // //     std::cout << std::string(86, '=') << std::endl;
// // // // // // // // // }

// // // // // // // // // // Вывод разделителя строк
// // // // // // // // // void printTableSeparator() {
// // // // // // // // //     std::cout << "|" << std::string(84, '-') << "|" << std::endl;
// // // // // // // // // }

// // // // // // // // // // Вывод строки таблицы
// // // // // // // // // void printTableRow(const std::string& method, const std::string& direction,
// // // // // // // // //                    const SortStats& stats) {
// // // // // // // // //     std::cout << "| " << std::setw(22) << std::left << method
// // // // // // // // //               << " | " << std::setw(18) << direction
// // // // // // // // //               << " | " << std::setw(18) << stats.comparisons
// // // // // // // // //               << " | " << std::setw(18) << stats.swaps
// // // // // // // // //               << " |" << std::endl;
// // // // // // // // // }

// // // // // // // // // // Вывод итогов таблицы
// // // // // // // // // void printTableFooter() {
// // // // // // // // //     std::cout << std::string(86, '=') << std::endl << std::endl;
// // // // // // // // // }

// // // // // // // // // // Явные инстанциации шаблонов для int
// // // // // // // // // template void printArray<int>(int* arr, size_t size);
// // // // // // // // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // // // // // // } // анонимное пространство имен

// // // // // // // // // namespace AssemblingSorting {

// // // // // // // // // void Assembling() {
// // // // // // // // //     // Устанавливаем локаль для поддержки кириллицы
// // // // // // // // //     std::locale::global(std::locale(""));
// // // // // // // // //     std::wcout.imbue(std::locale());

// // // // // // // // //     const size_t STATIC_SIZE = 8;
// // // // // // // // //     int staticArr[STATIC_SIZE];

// // // // // // // // //     std::cout << "\n" << std::string(70, '=') << std::endl;
// // // // // // // // //     std::cout << "ЛАБОРАТОРНАЯ РАБОТА: АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ" << std::endl;
// // // // // // // // //     std::cout << std::string(70, '=') << std::endl << std::endl;

// // // // // // // // //     // Часть 1: Статический массив
// // // // // // // // //     std::cout << "ЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ (" << STATIC_SIZE << " элементов)" << std::endl;
// // // // // // // // //     std::cout << std::string(60, '-') << std::endl;

// // // // // // // // //     // Заполняем массив случайными числами
// // // // // // // // //     fillRandom(staticArr, STATIC_SIZE);

// // // // // // // // //     std::cout << "\nИсходный массив: ";
// // // // // // // // //     printArray(staticArr, STATIC_SIZE);
// // // // // // // // //     std::cout << std::endl;

// // // // // // // // //     // Тестируем сортировку выбором
// // // // // // // // //     {
// // // // // // // // //         std::cout << "\n1. СОРТИРОВКА ВЫБОРОМ:" << std::endl;
// // // // // // // // //         printTableHeader();

// // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // //         SortStats stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // //         printTableRow("Выбором", "↑ (не отсорт.)", stats);
// // // // // // // // //         printTableSeparator();

// // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // //         printTableRow("Выбором", "↑ (отсорт.)", stats);
// // // // // // // // //         printTableSeparator();

// // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // //         printTableRow("Выбором", "↓", stats);
// // // // // // // // //         printTableFooter();

// // // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // //         std::cout << std::endl;
// // // // // // // // //     }

// // // // // // // // //     // Тестируем сортировку пузырьком
// // // // // // // // //     {
// // // // // // // // //         std::cout << "\n2. СОРТИРОВКА ПУЗЫРЬКОМ:" << std::endl;
// // // // // // // // //         printTableHeader();

// // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // //         SortStats stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // //         printTableRow("Пузырьком", "↑ (не отсорт.)", stats);
// // // // // // // // //         printTableSeparator();

// // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // //         printTableRow("Пузырьком", "↑ (отсорт.)", stats);
// // // // // // // // //         printTableSeparator();

// // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // //         printTableRow("Пузырьком", "↓", stats);
// // // // // // // // //         printTableFooter();

// // // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // //         std::cout << std::endl;
// // // // // // // // //     }

// // // // // // // // //     // Тестируем быструю сортировку
// // // // // // // // //     {
// // // // // // // // //         std::cout << "\n3. БЫСТРАЯ СОРТИРОВКА:" << std::endl;
// // // // // // // // //         printTableHeader();

// // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // //         SortStats stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // //         printTableRow("Быстрая", "↑ (не отсорт.)", stats);
// // // // // // // // //         printTableSeparator();

// // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // //         printTableRow("Быстрая", "↑ (отсорт.)", stats);
// // // // // // // // //         printTableSeparator();

// // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // //         printTableRow("Быстрая", "↓", stats);
// // // // // // // // //         printTableFooter();

// // // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // //         std::cout << std::endl;
// // // // // // // // //     }

// // // // // // // // //     // Часть 2: Динамический массив
// // // // // // // // //     std::cout << std::string(70, '=') << std::endl;
// // // // // // // // //     std::cout << "ЧАСТЬ 2: ДИНАМИЧЕСКИЙ МАССИВ" << std::endl;
// // // // // // // // //     std::cout << std::string(70, '=') << std::endl;

// // // // // // // // //     size_t dynamicSize;
// // // // // // // // //     std::cout << "\nВведите размер динамического массива: ";
// // // // // // // // //     std::cin >> dynamicSize;

// // // // // // // // //     if (dynamicSize == 0) {
// // // // // // // // //         std::cout << "Ошибка: размер массива должен быть больше 0!" << std::endl;
// // // // // // // // //         return;
// // // // // // // // //     }

// // // // // // // // //     // Создаем и заполняем динамический массив
// // // // // // // // //     int* dynamicArr = new int[dynamicSize];
// // // // // // // // //     fillRandom(dynamicArr, dynamicSize);

// // // // // // // // //     std::cout << "\nПервые 10 элементов исходного массива:\n";
// // // // // // // // //     std::cout << "[ ";
// // // // // // // // //     for (size_t i = 0; i < std::min(dynamicSize, size_t(10)); ++i) {
// // // // // // // // //         std::cout << std::setw(3) << dynamicArr[i] << " ";
// // // // // // // // //     }
// // // // // // // // //     std::cout << "]";
// // // // // // // // //     if (dynamicSize > 10) {
// // // // // // // // //         std::cout << " ... (всего " << dynamicSize << " элементов)";
// // // // // // // // //     }
// // // // // // // // //     std::cout << std::endl;

// // // // // // // // //     std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ ДИНАМИЧЕСКОГО МАССИВА ("
// // // // // // // // //               << dynamicSize << " элементов):" << std::endl;
// // // // // // // // //     printTableHeader();

// // // // // // // // //     // Сортировка выбором
// // // // // // // // //     int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // //     SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // // // // // // // //     printTableRow("Выбором", "↑", stats1);
// // // // // // // // //     printTableSeparator();
// // // // // // // // //     delete[] arrCopy1;

// // // // // // // // //     // Сортировка пузырьком
// // // // // // // // //     int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // //     SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // // // // // // // //     printTableRow("Пузырьком", "↑", stats2);
// // // // // // // // //     printTableSeparator();
// // // // // // // // //     delete[] arrCopy2;

// // // // // // // // //     // Быстрая сортировка
// // // // // // // // //     int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // //     SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // // // // // // // //     printTableRow("Быстрая", "↑", stats3);
// // // // // // // // //     delete[] arrCopy3;

// // // // // // // // //     printTableFooter();

// // // // // // // // //     // Освобождаем память
// // // // // // // // //     delete[] dynamicArr;

// // // // // // // // //     std::cout << std::string(70, '=') << std::endl;
// // // // // // // // //     std::cout << "РАБОТА ПРОГРАММЫ ЗАВЕРШЕНА" << std::endl;
// // // // // // // // //     std::cout << std::string(70, '=') << std::endl;
// // // // // // // // // }

// // // // // // // // // } // namespace AssemblingSorting



// // // // // // // // // // #include "sorting.hpp"
// // // // // // // // // // #include <iostream>
// // // // // // // // // // #include <iomanip>
// // // // // // // // // // #include <random>
// // // // // // // // // // #include <algorithm>
// // // // // // // // // // #include <string>
// // // // // // // // // // #include <cstddef>

// // // // // // // // // // namespace {

// // // // // // // // // // struct SortStats {
// // // // // // // // // //     size_t comparisons = 0;
// // // // // // // // // //     size_t swaps = 0;
// // // // // // // // // // };

// // // // // // // // // // // Функция вывода массива
// // // // // // // // // // template<typename T>
// // // // // // // // // // void printArray(T* arr, size_t size) {
// // // // // // // // // //     std::cout << "[ ";
// // // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // // //         std::cout << std::setw(3) << arr[i] << " ";
// // // // // // // // // //     }
// // // // // // // // // //     std::cout << "]" << std::endl;
// // // // // // // // // // }

// // // // // // // // // // // Сортировка выбором
// // // // // // // // // // template<typename T>
// // // // // // // // // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // // //     SortStats stats;

// // // // // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // // // // //         size_t extremeIndex = i;

// // // // // // // // // //         for (size_t j = i + 1; j < size; ++j) {
// // // // // // // // // //             stats.comparisons++;
// // // // // // // // // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // // // // // // // // //                 extremeIndex = j;
// // // // // // // // // //             }
// // // // // // // // // //         }

// // // // // // // // // //         if (extremeIndex != i) {
// // // // // // // // // //             std::swap(arr[i], arr[extremeIndex]);
// // // // // // // // // //             stats.swaps++;
// // // // // // // // // //         }
// // // // // // // // // //     }

// // // // // // // // // //     return stats;
// // // // // // // // // // }

// // // // // // // // // // // Сортировка пузырьком
// // // // // // // // // // template<typename T>
// // // // // // // // // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // // //     SortStats stats;
// // // // // // // // // //     bool swapped;

// // // // // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // // // // //         swapped = false;

// // // // // // // // // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // // // // // // // // //             stats.comparisons++;
// // // // // // // // // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // // // // // // // // //                 std::swap(arr[j], arr[j + 1]);
// // // // // // // // // //                 stats.swaps++;
// // // // // // // // // //                 swapped = true;
// // // // // // // // // //             }
// // // // // // // // // //         }

// // // // // // // // // //         if (!swapped) break;
// // // // // // // // // //     }

// // // // // // // // // //     return stats;
// // // // // // // // // // }

// // // // // // // // // // // Быстрая сортировка (вспомогательные функции)
// // // // // // // // // // template<typename T>
// // // // // // // // // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // // // // //     T pivot = arr[high];
// // // // // // // // // //     size_t i = low;

// // // // // // // // // //     for (size_t j = low; j < high; ++j) {
// // // // // // // // // //         stats.comparisons++;
// // // // // // // // // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // // // // // // // // //             if (i != j) {
// // // // // // // // // //                 std::swap(arr[i], arr[j]);
// // // // // // // // // //                 stats.swaps++;
// // // // // // // // // //             }
// // // // // // // // // //             i++;
// // // // // // // // // //         }
// // // // // // // // // //     }

// // // // // // // // // //     if (i != high) {
// // // // // // // // // //         std::swap(arr[i], arr[high]);
// // // // // // // // // //         stats.swaps++;
// // // // // // // // // //     }

// // // // // // // // // //     return i;
// // // // // // // // // // }

// // // // // // // // // // template<typename T>
// // // // // // // // // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // // // // //     if (low < high) {
// // // // // // // // // //         size_t pi = partition(arr, low, high, ascending, stats);

// // // // // // // // // //         if (pi > 0) {
// // // // // // // // // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // // // // // // // // //         }
// // // // // // // // // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // // // // // // // // //     }
// // // // // // // // // // }

// // // // // // // // // // // Быстрая сортировка
// // // // // // // // // // template<typename T>
// // // // // // // // // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // // //     SortStats stats;
// // // // // // // // // //     if (size > 1) {
// // // // // // // // // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // // // // // // // // //     }
// // // // // // // // // //     return stats;
// // // // // // // // // // }

// // // // // // // // // // // Заполнение массива случайными числами
// // // // // // // // // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // // // // // // // // //     std::random_device rd;
// // // // // // // // // //     std::default_random_engine engine(rd());
// // // // // // // // // //     std::uniform_int_distribution<int> distribution(min, max);

// // // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // // //         arr[i] = distribution(engine);
// // // // // // // // // //     }
// // // // // // // // // // }

// // // // // // // // // // // Копирование массива
// // // // // // // // // // int* copyArray(const int* source, size_t size) {
// // // // // // // // // //     int* copy = new int[size];
// // // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // // //         copy[i] = source[i];
// // // // // // // // // //     }
// // // // // // // // // //     return copy;
// // // // // // // // // // }

// // // // // // // // // // // Вывод заголовка таблицы
// // // // // // // // // // void printTableHeader() {
// // // // // // // // // //     std::cout << std::string(80, '=') << std::endl;
// // // // // // // // // //     std::cout << "| " << std::setw(20) << std::left << "Метод сортировки"
// // // // // // // // // //               << " | " << std::setw(15) << "Направление"
// // // // // // // // // //               << " | " << std::setw(15) << "Сравнения"
// // // // // // // // // //               << " | " << std::setw(15) << "Перестановки"
// // // // // // // // // //               << " |" << std::endl;
// // // // // // // // // //     std::cout << std::string(80, '=') << std::endl;
// // // // // // // // // // }

// // // // // // // // // // // Вывод разделителя строк
// // // // // // // // // // void printTableSeparator() {
// // // // // // // // // //     std::cout << "|" << std::string(78, '-') << "|" << std::endl;
// // // // // // // // // // }

// // // // // // // // // // // Вывод строки таблицы
// // // // // // // // // // void printTableRow(const std::string& method, const std::string& direction,
// // // // // // // // // //                    const SortStats& stats) {
// // // // // // // // // //     std::cout << "| " << std::setw(20) << std::left << method
// // // // // // // // // //               << " | " << std::setw(15) << direction
// // // // // // // // // //               << " | " << std::setw(15) << stats.comparisons
// // // // // // // // // //               << " | " << std::setw(15) << stats.swaps
// // // // // // // // // //               << " |" << std::endl;
// // // // // // // // // // }

// // // // // // // // // // // Вывод итогов таблицы
// // // // // // // // // // void printTableFooter() {
// // // // // // // // // //     std::cout << std::string(80, '=') << std::endl << std::endl;
// // // // // // // // // // }

// // // // // // // // // // // Явные инстанциации шаблонов для int
// // // // // // // // // // template void printArray<int>(int* arr, size_t size);
// // // // // // // // // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // // // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // // // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // // // // // // // } // анонимное пространство имен

// // // // // // // // // // namespace AssemblingSorting {

// // // // // // // // // // void Assembling() {
// // // // // // // // // //     const size_t STATIC_SIZE = 8;
// // // // // // // // // //     int staticArr[STATIC_SIZE];

// // // // // // // // // //     std::cout << "\n" << std::string(60, '=') << std::endl;
// // // // // // // // // //     std::cout << "ЛАБОРАТОРНАЯ РАБОТА: АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ" << std::endl;
// // // // // // // // // //     std::cout << std::string(60, '=') << std::endl << std::endl;

// // // // // // // // // //     // Часть 1: Статический массив
// // // // // // // // // //     std::cout << "ЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ (" << STATIC_SIZE << " элементов)" << std::endl;
// // // // // // // // // //     std::cout << std::string(50, '-') << std::endl;

// // // // // // // // // //     // Заполняем массив случайными числами
// // // // // // // // // //     fillRandom(staticArr, STATIC_SIZE);

// // // // // // // // // //     std::cout << "\nИсходный массив: ";
// // // // // // // // // //     printArray(staticArr, STATIC_SIZE);
// // // // // // // // // //     std::cout << std::endl;

// // // // // // // // // //     // Тестируем сортировку выбором
// // // // // // // // // //     {
// // // // // // // // // //         std::cout << "\n1. СОРТИРОВКА ВЫБОРОМ:" << std::endl;
// // // // // // // // // //         printTableHeader();

// // // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // // //         SortStats stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // //         printTableRow("Выбором", "↑ (не отсорт.)", stats);
// // // // // // // // // //         printTableSeparator();

// // // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // //         printTableRow("Выбором", "↑ (отсорт.)", stats);
// // // // // // // // // //         printTableSeparator();

// // // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // // //         printTableRow("Выбором", "↓", stats);
// // // // // // // // // //         printTableFooter();

// // // // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // // //     }

// // // // // // // // // //     // Тестируем сортировку пузырьком
// // // // // // // // // //     {
// // // // // // // // // //         std::cout << "\n2. СОРТИРОВКА ПУЗЫРЬКОМ:" << std::endl;
// // // // // // // // // //         printTableHeader();

// // // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // // //         SortStats stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // //         printTableRow("Пузырьком", "↑ (не отсорт.)", stats);
// // // // // // // // // //         printTableSeparator();

// // // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // //         printTableRow("Пузырьком", "↑ (отсорт.)", stats);
// // // // // // // // // //         printTableSeparator();

// // // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // // //         printTableRow("Пузырьком", "↓", stats);
// // // // // // // // // //         printTableFooter();

// // // // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // // //     }

// // // // // // // // // //     // Тестируем быструю сортировку
// // // // // // // // // //     {
// // // // // // // // // //         std::cout << "\n3. БЫСТРАЯ СОРТИРОВКА:" << std::endl;
// // // // // // // // // //         printTableHeader();

// // // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // // //         SortStats stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // //         printTableRow("Быстрая", "↑ (не отсорт.)", stats);
// // // // // // // // // //         printTableSeparator();

// // // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // //         printTableRow("Быстрая", "↑ (отсорт.)", stats);
// // // // // // // // // //         printTableSeparator();

// // // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // // //         printTableRow("Быстрая", "↓", stats);
// // // // // // // // // //         printTableFooter();

// // // // // // // // // //         std::cout << "Массив после всех операций: ";
// // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // // //     }

// // // // // // // // // //     // Часть 2: Динамический массив
// // // // // // // // // //     std::cout << "\n" << std::string(50, '=') << std::endl;
// // // // // // // // // //     std::cout << "ЧАСТЬ 2: ДИНАМИЧЕСКИЙ МАССИВ" << std::endl;
// // // // // // // // // //     std::cout << std::string(50, '=') << std::endl;

// // // // // // // // // //     size_t dynamicSize;
// // // // // // // // // //     std::cout << "\nВведите размер динамического массива: ";
// // // // // // // // // //     std::cin >> dynamicSize;

// // // // // // // // // //     if (dynamicSize == 0) {
// // // // // // // // // //         std::cout << "Ошибка: размер массива должен быть больше 0!" << std::endl;
// // // // // // // // // //         return;
// // // // // // // // // //     }

// // // // // // // // // //     // Создаем и заполняем динамический массив
// // // // // // // // // //     int* dynamicArr = new int[dynamicSize];
// // // // // // // // // //     fillRandom(dynamicArr, dynamicSize);

// // // // // // // // // //     std::cout << "\nПервые 10 элементов исходного массива: ";
// // // // // // // // // //     printArray(dynamicArr, std::min(dynamicSize, size_t(10)));
// // // // // // // // // //     if (dynamicSize > 10) {
// // // // // // // // // //         std::cout << "... (всего " << dynamicSize << " элементов)" << std::endl;
// // // // // // // // // //     }

// // // // // // // // // //     std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ ДИНАМИЧЕСКОГО МАССИВА ("
// // // // // // // // // //               << dynamicSize << " элементов):" << std::endl;
// // // // // // // // // //     printTableHeader();

// // // // // // // // // //     // Сортировка выбором
// // // // // // // // // //     int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // // //     SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // // // // // // // // //     printTableRow("Выбором", "↑", stats1);
// // // // // // // // // //     printTableSeparator();
// // // // // // // // // //     delete[] arrCopy1;

// // // // // // // // // //     // Сортировка пузырьком
// // // // // // // // // //     int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // // //     SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // // // // // // // // //     printTableRow("Пузырьком", "↑", stats2);
// // // // // // // // // //     printTableSeparator();
// // // // // // // // // //     delete[] arrCopy2;

// // // // // // // // // //     // Быстрая сортировка
// // // // // // // // // //     int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // // //     SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // // // // // // // // //     printTableRow("Быстрая", "↑", stats3);
// // // // // // // // // //     delete[] arrCopy3;

// // // // // // // // // //     printTableFooter();

// // // // // // // // // //     // Освобождаем память
// // // // // // // // // //     delete[] dynamicArr;

// // // // // // // // // //     std::cout << std::string(60, '=') << std::endl;
// // // // // // // // // //     std::cout << "РАБОТА ПРОГРАММЫ ЗАВЕРШЕНА" << std::endl;
// // // // // // // // // //     std::cout << std::string(60, '=') << std::endl;
// // // // // // // // // // }

// // // // // // // // // // } // namespace AssemblingSorting



// // // // // // // // // // // #include "sorting.hpp"
// // // // // // // // // // // #include <iostream>
// // // // // // // // // // // #include <iomanip>
// // // // // // // // // // // #include <random>
// // // // // // // // // // // #include <algorithm>
// // // // // // // // // // // #include <string>
// // // // // // // // // // // #include <cstddef>

// // // // // // // // // // // namespace {

// // // // // // // // // // // struct SortStats {
// // // // // // // // // // //     size_t comparisons = 0;
// // // // // // // // // // //     size_t swaps = 0;
// // // // // // // // // // // };

// // // // // // // // // // // // Функция вывода массива
// // // // // // // // // // // template<typename T>
// // // // // // // // // // // void printArray(T* arr, size_t size) {
// // // // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // // // //         std::cout << arr[i] << " ";
// // // // // // // // // // //     }
// // // // // // // // // // //     std::cout << std::endl;
// // // // // // // // // // // }

// // // // // // // // // // // // Сортировка выбором
// // // // // // // // // // // template<typename T>
// // // // // // // // // // // SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // // // //     SortStats stats;

// // // // // // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // // // // // //         size_t extremeIndex = i;

// // // // // // // // // // //         for (size_t j = i + 1; j < size; ++j) {
// // // // // // // // // // //             stats.comparisons++;
// // // // // // // // // // //             if (ascending ? (arr[j] < arr[extremeIndex]) : (arr[j] > arr[extremeIndex])) {
// // // // // // // // // // //                 extremeIndex = j;
// // // // // // // // // // //             }
// // // // // // // // // // //         }

// // // // // // // // // // //         if (extremeIndex != i) {
// // // // // // // // // // //             std::swap(arr[i], arr[extremeIndex]);
// // // // // // // // // // //             stats.swaps++;
// // // // // // // // // // //         }
// // // // // // // // // // //     }

// // // // // // // // // // //     return stats;
// // // // // // // // // // // }

// // // // // // // // // // // // Сортировка пузырьком
// // // // // // // // // // // template<typename T>
// // // // // // // // // // // SortStats bubbleSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // // // //     SortStats stats;
// // // // // // // // // // //     bool swapped;

// // // // // // // // // // //     for (size_t i = 0; i < size - 1; ++i) {
// // // // // // // // // // //         swapped = false;

// // // // // // // // // // //         for (size_t j = 0; j < size - i - 1; ++j) {
// // // // // // // // // // //             stats.comparisons++;
// // // // // // // // // // //             if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
// // // // // // // // // // //                 std::swap(arr[j], arr[j + 1]);
// // // // // // // // // // //                 stats.swaps++;
// // // // // // // // // // //                 swapped = true;
// // // // // // // // // // //             }
// // // // // // // // // // //         }

// // // // // // // // // // //         if (!swapped) break;
// // // // // // // // // // //     }

// // // // // // // // // // //     return stats;
// // // // // // // // // // // }

// // // // // // // // // // // // Быстрая сортировка (вспомогательные функции)
// // // // // // // // // // // template<typename T>
// // // // // // // // // // // size_t partition(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // // // // // //     T pivot = arr[high];
// // // // // // // // // // //     size_t i = low;

// // // // // // // // // // //     for (size_t j = low; j < high; ++j) {
// // // // // // // // // // //         stats.comparisons++;
// // // // // // // // // // //         if (ascending ? (arr[j] <= pivot) : (arr[j] >= pivot)) {
// // // // // // // // // // //             if (i != j) {
// // // // // // // // // // //                 std::swap(arr[i], arr[j]);
// // // // // // // // // // //                 stats.swaps++;
// // // // // // // // // // //             }
// // // // // // // // // // //             i++;
// // // // // // // // // // //         }
// // // // // // // // // // //     }

// // // // // // // // // // //     if (i != high) {
// // // // // // // // // // //         std::swap(arr[i], arr[high]);
// // // // // // // // // // //         stats.swaps++;
// // // // // // // // // // //     }

// // // // // // // // // // //     return i;
// // // // // // // // // // // }

// // // // // // // // // // // template<typename T>
// // // // // // // // // // // void quickSortRecursive(T* arr, size_t low, size_t high, bool ascending, SortStats& stats) {
// // // // // // // // // // //     if (low < high) {
// // // // // // // // // // //         size_t pi = partition(arr, low, high, ascending, stats);

// // // // // // // // // // //         if (pi > 0) {
// // // // // // // // // // //             quickSortRecursive(arr, low, pi - 1, ascending, stats);
// // // // // // // // // // //         }
// // // // // // // // // // //         quickSortRecursive(arr, pi + 1, high, ascending, stats);
// // // // // // // // // // //     }
// // // // // // // // // // // }

// // // // // // // // // // // // Быстрая сортировка
// // // // // // // // // // // template<typename T>
// // // // // // // // // // // SortStats quickSort(T* arr, size_t size, bool ascending = true) {
// // // // // // // // // // //     SortStats stats;
// // // // // // // // // // //     if (size > 1) {
// // // // // // // // // // //         quickSortRecursive(arr, 0, size - 1, ascending, stats);
// // // // // // // // // // //     }
// // // // // // // // // // //     return stats;
// // // // // // // // // // // }

// // // // // // // // // // // // Заполнение массива случайными числами
// // // // // // // // // // // void fillRandom(int* arr, size_t size, int min = 0, int max = 99) {
// // // // // // // // // // //     std::random_device rd;
// // // // // // // // // // //     std::default_random_engine engine(rd());
// // // // // // // // // // //     std::uniform_int_distribution<int> distribution(min, max);

// // // // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // // // //         arr[i] = distribution(engine);
// // // // // // // // // // //     }
// // // // // // // // // // // }

// // // // // // // // // // // // Копирование массива
// // // // // // // // // // // int* copyArray(const int* source, size_t size) {
// // // // // // // // // // //     int* copy = new int[size];
// // // // // // // // // // //     for (size_t i = 0; i < size; ++i) {
// // // // // // // // // // //         copy[i] = source[i];
// // // // // // // // // // //     }
// // // // // // // // // // //     return copy;
// // // // // // // // // // // }

// // // // // // // // // // // // Вывод заголовка таблицы
// // // // // // // // // // // void printTableHeader() {
// // // // // // // // // // //     std::cout << std::setw(20) << "Метод сортировки "
// // // // // // // // // // //               << std::setw(15) << "Направление "
// // // // // // // // // // //               << std::setw(15) << "Сравнения "
// // // // // // // // // // //               << std::setw(15) << "Перестановки"
// // // // // // // // // // //               << std::endl;
// // // // // // // // // // //     std::cout << std::string(65, '-') << std::endl;
// // // // // // // // // // // }

// // // // // // // // // // // // Вывод строки таблицы
// // // // // // // // // // // void printTableRow(const std::string& method, const std::string& direction,
// // // // // // // // // // //                    const SortStats& stats) {
// // // // // // // // // // //     std::cout << std::setw(20) << method
// // // // // // // // // // //               << std::setw(15) << direction
// // // // // // // // // // //               << std::setw(15) << stats.comparisons
// // // // // // // // // // //               << std::setw(15) << stats.swaps
// // // // // // // // // // //               << std::endl;
// // // // // // // // // // // }

// // // // // // // // // // // // Явные инстанциации шаблонов для int
// // // // // // // // // // // template void printArray<int>(int* arr, size_t size);
// // // // // // // // // // // template SortStats selectionSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // // // // // template SortStats bubbleSort<int>(int* arr, size_t size, bool ascending);
// // // // // // // // // // // template SortStats quickSort<int>(int* arr, size_t size, bool ascending);

// // // // // // // // // // // } // анонимное пространство имен

// // // // // // // // // // // namespace AssemblingSorting {

// // // // // // // // // // // void Assembling() {
// // // // // // // // // // //     const size_t STATIC_SIZE = 8;
// // // // // // // // // // //     int staticArr[STATIC_SIZE];

// // // // // // // // // // //     std::cout << "ЛАБОРАТОРНАЯ РАБОТА: АНАЛИЗ АЛГОРИТМОВ СОРТИРОВКИ" << std::endl;
// // // // // // // // // // //     std::cout << "=================================================" << std::endl << std::endl;

// // // // // // // // // // //     // Часть 1: Статический массив
// // // // // // // // // // //     std::cout << "ЧАСТЬ 1: СТАТИЧЕСКИЙ МАССИВ (" << STATIC_SIZE << " элементов)" << std::endl;
// // // // // // // // // // //     std::cout << "=================================================" << std::endl;

// // // // // // // // // // //     // Заполняем массив случайными числами
// // // // // // // // // // //     fillRandom(staticArr, STATIC_SIZE);

// // // // // // // // // // //     std::cout << "Исходный массив: ";
// // // // // // // // // // //     printArray(staticArr, STATIC_SIZE);
// // // // // // // // // // //     std::cout << std::endl;

// // // // // // // // // // //     // Тестируем сортировку выбором
// // // // // // // // // // //     {
// // // // // // // // // // //         std::cout << "\nСОРТИРОВКА ВЫБОРОМ:" << std::endl;
// // // // // // // // // // //         printTableHeader();

// // // // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // // // //         SortStats stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // // //         printTableRow("Выбором", "↑ (не отсорт.)", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);

// // // // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // // //         printTableRow("Выбором", "↑ (отсорт.)", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);

// // // // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // // // //         stats = selectionSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // // // //         printTableRow("Выбором", "↓", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // // // //     }

// // // // // // // // // // //     // Тестируем сортировку пузырьком
// // // // // // // // // // //     {
// // // // // // // // // // //         std::cout << "\nСОРТИРОВКА ПУЗЫРЬКОМ:" << std::endl;
// // // // // // // // // // //         printTableHeader();

// // // // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // // // //         SortStats stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // // //         printTableRow("Пузырьком", "↑ (не отсорт.)", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);

// // // // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // // //         printTableRow("Пузырьком", "↑ (отсорт.)", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);

// // // // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // // // //         stats = bubbleSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // // // //         printTableRow("Пузырьком", "↓", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // // // //     }

// // // // // // // // // // //     // Тестируем быструю сортировку
// // // // // // // // // // //     {
// // // // // // // // // // //         std::cout << "\nБЫСТРАЯ СОРТИРОВКА:" << std::endl;
// // // // // // // // // // //         printTableHeader();

// // // // // // // // // // //         int arrCopy[STATIC_SIZE];
// // // // // // // // // // //         std::copy(staticArr, staticArr + STATIC_SIZE, arrCopy);

// // // // // // // // // // //         // 1. Сортировка неотсортированного массива по возрастанию
// // // // // // // // // // //         SortStats stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // // //         printTableRow("Быстрая", "↑ (не отсорт.)", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);

// // // // // // // // // // //         // 2. Сортировка отсортированного массива по возрастанию
// // // // // // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, true);
// // // // // // // // // // //         printTableRow("Быстрая", "↑ (отсорт.)", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);

// // // // // // // // // // //         // 3. Сортировка по убыванию
// // // // // // // // // // //         stats = quickSort(arrCopy, STATIC_SIZE, false);
// // // // // // // // // // //         printTableRow("Быстрая", "↓", stats);
// // // // // // // // // // //         std::cout << "Массив после сортировки: ";
// // // // // // // // // // //         printArray(arrCopy, STATIC_SIZE);
// // // // // // // // // // //     }

// // // // // // // // // // //     // Часть 2: Динамический массив
// // // // // // // // // // //     std::cout << "\n\nЧАСТЬ 2: ДИНАМИЧЕСКИЙ МАССИВ" << std::endl;
// // // // // // // // // // //     std::cout << "==============================" << std::endl;

// // // // // // // // // // //     size_t dynamicSize;
// // // // // // // // // // //     std::cout << "Введите размер динамического массива: ";
// // // // // // // // // // //     std::cin >> dynamicSize;

// // // // // // // // // // //     if (dynamicSize == 0) {
// // // // // // // // // // //         std::cout << "Размер массива должен быть больше 0!" << std::endl;
// // // // // // // // // // //         return;
// // // // // // // // // // //     }

// // // // // // // // // // //     // Создаем и заполняем динамический массив
// // // // // // // // // // //     int* dynamicArr = new int[dynamicSize];
// // // // // // // // // // //     fillRandom(dynamicArr, dynamicSize);

// // // // // // // // // // //     std::cout << "\nПервые 10 элементов исходного массива: ";
// // // // // // // // // // //     printArray(dynamicArr, std::min(dynamicSize, size_t(10)));
// // // // // // // // // // //     if (dynamicSize > 10) {
// // // // // // // // // // //         std::cout << "... (всего " << dynamicSize << " элементов)" << std::endl;
// // // // // // // // // // //     }

// // // // // // // // // // //     std::cout << "\nРЕЗУЛЬТАТЫ СОРТИРОВКИ ДИНАМИЧЕСКОГО МАССИВА:" << std::endl;
// // // // // // // // // // //     printTableHeader();

// // // // // // // // // // //     // Сортировка выбором
// // // // // // // // // // //     int* arrCopy1 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // // // //     SortStats stats1 = selectionSort(arrCopy1, dynamicSize, true);
// // // // // // // // // // //     printTableRow("Выбором", "↑", stats1);
// // // // // // // // // // //     delete[] arrCopy1;

// // // // // // // // // // //     // Сортировка пузырьком
// // // // // // // // // // //     int* arrCopy2 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // // // //     SortStats stats2 = bubbleSort(arrCopy2, dynamicSize, true);
// // // // // // // // // // //     printTableRow("Пузырьком", "↑", stats2);
// // // // // // // // // // //     delete[] arrCopy2;

// // // // // // // // // // //     // Быстрая сортировка
// // // // // // // // // // //     int* arrCopy3 = copyArray(dynamicArr, dynamicSize);
// // // // // // // // // // //     SortStats stats3 = quickSort(arrCopy3, dynamicSize, true);
// // // // // // // // // // //     printTableRow("Быстрая", "↑", stats3);
// // // // // // // // // // //     delete[] arrCopy3;

// // // // // // // // // // //     // Освобождаем память
// // // // // // // // // // //     delete[] dynamicArr;
// // // // // // // // // // // }

// // // // // // // // // // // } // namespace AssemblingSorting
