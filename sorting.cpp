#include "sorting.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <new>  // Добавлено для std::bad_alloc
#include <random>
#include <string>
#include <vector>  // Добавлено для std::vector

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
        if (i < printSize - 1)
            std::cout << " ";
    }
    if (size > 20)
        std::cout << " ...";
    std::cout << "]";
}

// Сортировка выбором
template<typename T>
SortStats selectionSort(T* arr, size_t size, bool ascending = true) {
    SortStats stats;
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i + 1 < size; ++i) {  // Исправлено: i + 1 < size
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

    for (size_t i = 0; i + 1 < size; ++i) {  // Исправлено: i + 1 < size
        swapped = false;

        for (size_t j = 0; j + 1 + i < size; ++j) {  // Исправлено: j + 1 + i < size
            stats.comparisons++;
            if (ascending ? (arr[j] > arr[j + 1]) : (arr[j] < arr[j + 1])) {
                std::swap(arr[j], arr[j + 1]);
                stats.swaps++;
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

    auto end = std::chrono::high_resolution_clock::now();
    stats.time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    return stats;
}

// Быстрая сортировка (дополнительное задание) - ИСПРАВЛЕНА
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
            ++i;
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

        // КРИТИЧЕСКОЕ ИСПРАВЛЕНИЕ: предотвращение переполнения беззнакового типа
        if (pi > low) {  // Было: if (pi > 0)
            quickSortRecursive(arr, low, pi - 1, ascending, stats);
        }
        if (pi + 1 < high) {  // Добавлена проверка для правой части
            quickSortRecursive(arr, pi + 1, high, ascending, stats);
        }
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

// Копирование массива (ИСПРАВЛЕНО: добавлена проверка выделения памяти)
int* copyArray(const int* source, size_t size) {
    if (size == 0) {
        return nullptr;
    }

    int* copy = nullptr;
    try {
        copy = new int[size];
        for (size_t i = 0; i < size; ++i) {
            copy[i] = source[i];
        }
    } catch (const std::bad_alloc& e) {
        std::cerr << "Ошибка выделения памяти для массива размером " << size << ": " << e.what() << std::endl;
        return nullptr;
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

}  // namespace

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
    const size_t STATIC_SIZE = 10;  // Фиксированный размер статического массива

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
    std::cout << "\n   Сравнения: " << stats1.comparisons << ", Перестановки: " << stats1.swaps << "\n\n";

    // 2. Уже отсортированный -> по возрастанию (заново)
    std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
    SortStats stats2 = selectionSort(arrSelection2, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrSelection2, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats2.comparisons << ", Перестановки: " << stats2.swaps << "\n\n";

    // 3. Отсортированный -> по убыванию
    std::cout << "3. Отсортированный массив -> по убыванию:\n";
    SortStats stats3 = selectionSort(arrSelection3, STATIC_SIZE, false);
    std::cout << "   Массив: ";
    printArray(arrSelection3, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats3.comparisons << ", Перестановки: " << stats3.swaps << "\n\n";

    // Тестируем сортировку пузырьком (ОСНОВНОЕ ЗАДАНИЕ)
    std::cout << "АЛГОРИТМ: СОРТИРОВКА ПУЗЫРЬКОМ (основное задание)\n";
    std::cout << std::string(70, '-') << "\n";

    // 1. Исходный -> по возрастанию
    std::cout << "1. Исходный массив -> по возрастанию:\n";
    SortStats stats4 = bubbleSort(arrBubble1, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrBubble1, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats4.comparisons << ", Перестановки: " << stats4.swaps << "\n\n";

    // 2. Уже отсортированный -> по возрастанию (заново)
    std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
    SortStats stats5 = bubbleSort(arrBubble2, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrBubble2, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats5.comparisons << ", Перестановки: " << stats5.swaps << "\n\n";

    // 3. Отсортированный -> по убыванию
    std::cout << "3. Отсортированный массив -> по убыванию:\n";
    SortStats stats6 = bubbleSort(arrBubble3, STATIC_SIZE, false);
    std::cout << "   Массив: ";
    printArray(arrBubble3, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats6.comparisons << ", Перестановки: " << stats6.swaps << "\n\n";

    // Тестируем быструю сортировку (ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ)
    std::cout << "АЛГОРИТМ: БЫСТРАЯ СОРТИРОВКА (дополнительное задание)\n";
    std::cout << std::string(70, '-') << "\n";

    // 1. Исходный -> по возрастанию
    std::cout << "1. Исходный массив -> по возрастанию:\n";
    SortStats stats7 = quickSort(arrQuick1, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrQuick1, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats7.comparisons << ", Перестановки: " << stats7.swaps << "\n\n";

    // 2. Уже отсортированный -> по возрастанию (заново)
    std::cout << "2. Отсортированный массив -> по возрастанию (заново):\n";
    SortStats stats8 = quickSort(arrQuick2, STATIC_SIZE, true);
    std::cout << "   Массив: ";
    printArray(arrQuick2, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats8.comparisons << ", Перестановки: " << stats8.swaps << "\n\n";

    // 3. Отсортированный -> по убыванию
    std::cout << "3. Отсортированный массив -> по убыванию:\n";
    SortStats stats9 = quickSort(arrQuick3, STATIC_SIZE, false);
    std::cout << "   Массив: ";
    printArray(arrQuick3, STATIC_SIZE);
    std::cout << "\n   Сравнения: " << stats9.comparisons << ", Перестановки: " << stats9.swaps << "\n\n";

    std::cout << std::string(70, '=') << "\n";
    std::cout << "ТЕСТИРОВАНИЕ СТАТИЧЕСКИХ МАССИВОВ ЗАВЕРШЕНО\n";
    std::cout << std::string(70, '=') << "\n";
}

// Функция для тестирования на кратных размерах (ИСПРАВЛЕНА)
void testMultipleSizes(size_t baseSize) {
    std::vector<size_t> sizes;

    // Создаем последовательность с проверкой переполнения
    size_t current = baseSize;
    for (int i = 0; i < 4; ++i) {
        sizes.push_back(current);

        // Проверка перед умножением (исправлено переполнение)
        if (i < 3) {
            if (current > SIZE_MAX / 10) {
                std::cout << "Предупреждение: достигнут максимальный размер, пропуск дальнейшего умножения\n";
                break;
            }
            current *= 10;
        }
    }

    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "ТЕСТИРОВАНИЕ НА РАЗМЕРАХ: ";
    for (size_t i = 0; i < sizes.size(); ++i) {
        std::cout << sizes[i];
        if (i + 1 < sizes.size())
            std::cout << ", ";
    }
    std::cout << "\n" << std::string(70, '=') << "\n";

    // Заголовок таблицы
    std::cout << "\nРАЗМЕР | МЕТОД         | СРАВНЕНИЯ   | ПЕРЕСТАНОВКИ | ВРЕМЯ (мс)\n";
    std::cout << std::string(70, '-') << "\n";

    // Тестируем для каждого размера
    for (size_t size : sizes) {
        std::cout << "\n" << std::setw(6) << size << " ";
        std::cout << std::string(64, '-') << "\n";

        // Создаем и заполняем массив с проверкой выделения памяти
        int* arr = nullptr;
        try {
            arr = new int[size];
            fillRandom(arr, size);
        } catch (const std::bad_alloc& e) {
            std::cerr << "Ошибка выделения памяти для размера " << size << ": " << e.what() << std::endl;
            continue;
        }

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
            if (arrCopy != nullptr) {
                SortStats stats = selectionSort(arrCopy, size, true);

                std::cout << "       | " << std::setw(13) << std::left << "Выбором"
                          << "| " << std::setw(11) << stats.comparisons << "| " << std::setw(13) << stats.swaps << "| " << std::fixed
                          << std::setprecision(3) << stats.time_ms << "\n";

                delete[] arrCopy;
            }
        }

        // 2. Сортировка пузырьком (ОСНОВНОЕ ЗАДАНИЕ)
        {
            int* arrCopy = copyArray(arr, size);
            if (arrCopy != nullptr) {
                SortStats stats = bubbleSort(arrCopy, size, true);

                std::cout << "       | " << std::setw(13) << std::left << "Пузырьком"
                          << "| " << std::setw(11) << stats.comparisons << "| " << std::setw(13) << stats.swaps << "| " << std::fixed
                          << std::setprecision(3) << stats.time_ms << "\n";

                delete[] arrCopy;
            }
        }

        // 3. Быстрая сортировка (ДОПОЛНИТЕЛЬНОЕ ЗАДАНИЕ)
        {
            int* arrCopy = copyArray(arr, size);
            if (arrCopy != nullptr) {
                SortStats stats = quickSort(arrCopy, size, true);

                std::cout << "       | " << std::setw(13) << std::left << "Быстрая"
                          << "| " << std::setw(11) << stats.comparisons << "| " << std::setw(13) << stats.swaps << "| " << std::fixed
                          << std::setprecision(3) << stats.time_ms << "\n";

                delete[] arrCopy;
            }
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

        if (baseSize > SIZE_MAX / 1000) {
            std::cout << "\n⚠️  ВНИМАНИЕ: Запрашиваемый размер слишком большой.\n"
                      << "   Максимально допустимый базовый размер: " << SIZE_MAX / 1000 << "\n";
            continueProgram = askToContinue();
            continue;
        }

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

}  // namespace AssemblingSorting
