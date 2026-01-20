#include <clocale>  // Добавляем заголовок для setlocale
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

// Убираем using namespace std; используем явные квалификаторы

// Задача 1: Сумма чисел, делящихся на 5 и не делящихся на m
void task1() {
    std::cout << "=== ЗАДАЧА 1 ===" << std::endl;
    int n = 0;  // Инициализация
    int m = 0;  // Инициализация

    std::cout << "Введите n: ";
    std::cin >> n;
    std::cout << "Введите m (m < n): ";
    std::cin >> m;

    if (m >= n) {
        std::cout << "Ошибка: m должно быть меньше n!" << std::endl;
        return;
    }

    int sum = 0;
    const int divisor = 5;  // Константа вместо магического числа
    for (int i = divisor; i <= n; i = i + divisor) {
        if (i % m != 0) {
            sum += i;
        }
    }
    std::cout << "Сумма чисел: " << sum << std::endl;
}

// Задача 2: Вычисление произведения в зависимости от a
void task2() {
    double a = 0.0;  // Используем 0.0 вместо 0.
    std::cout << "Введите число a:" << '\n';
    std::cin >> a;

    double product = 1.0;  // Переименовано из sum в product (по смыслу)
    if (a >= 0.0) {
        const int kMinI = 2;
        const int kMaxI = 8;
        const int kStepI = 2;
        int i = kMinI;
        while (i <= kMaxI) {
            product *= i * i;
            i += kStepI;
        }
        product -= a;
    } else {
        const int kMinI = 3;
        const int kMaxI = 9;
        const int kStepI = 3;
        const int kTwo = 2;
        int i = kMinI;
        while (i <= kMaxI) {
            product *= (i - kTwo);
            i += kStepI;
        }
    }
    std::cout << "Ответ на задачу: " << std::fixed << product << '\n';
}

// Задача 3: Сумма ряда Тейлора для e^(2x) с использованием рекуррентного соотношения
void task3() {
    std::cout << "=== ЗАДАЧА 3 ===" << std::endl;
    const double epsilon = 1e-6;
    const double h = 0.2;
    const int maxIterations = 1000;  // Константа вместо магического числа
    const int maxLimit = 10000;      // Реалистичный предел итераций

    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setw(10) << "x" << std::setw(15) << "Y(x)" << std::setw(15) << "S(x)" << std::setw(8) << "N" << std::endl;
    std::cout << std::string(48, '-') << std::endl;

    // Используем while вместо for с неточным сравнением double
    double x = 0.0;
    const double endX = 1.01;
    const double tolerance = 1e-10;  // Допуск для сравнения double

    while (x <= endX + tolerance) {    // Добавляем допуск
        double Y = std::exp(2.0 * x);  // Точное значение функции
        double S = 1.0;                // Сумма ряда (первый член = 1)
        double term = 1.0;             // Текущий член ряда
        int N = 0;                     // Номер последнего слагаемого

        // Вычисление суммы ряда с использованием рекуррентного соотношения
        for (int n = 1; n <= maxLimit; ++n) {  // Используем реалистичный предел
            // Рекуррентное соотношение: term_n = term_{n-1} * (2x / n)
            term *= (2.0 * x) / static_cast<double>(n);
            S += term;

            if (std::fabs(term) < epsilon) {
                N = n;
                break;
            }

            // Защита от бесконечного цикла
            if (n == maxIterations) {
                N = maxIterations;
                break;
            }
        }

        std::cout << std::setw(10) << x << std::setw(15) << Y << std::setw(15) << S << std::setw(8) << N << std::endl;

        x += h;
    }
}

// Задача 4: Вычисление знакопеременного ряда с промежуточными суммами
void task4() {
    std::cout << "=== ЗАДАЧА 4 ===" << std::endl;
    int n = 0;       // Инициализация
    double x = 0.0;  // Инициализация

    std::cout << "Введите n: ";
    std::cin >> n;
    std::cout << "Введите x: ";
    std::cin >> x;

    double y = 1.0;             // Первый член ряда (n=0)
    double current_term = 1.0;  // Текущий член ряда
    // numerator и denominator не используются, убираем их
    // double numerator = 1.0;   // Убрать - не используется
    // double denominator = 1.0; // Убрать - не используется

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Промежуточные суммы:" << std::endl;

    for (int i = 1; i <= n; ++i) {  // Используем префиксный инкремент
        // Вычисление текущего члена ряда с использованием предыдущего
        // term_i = term_{i-1} * (-1) * x * (2i+1) / (2i)
        current_term *= (-1.0) * x * (2.0 * i + 1.0) / (2.0 * i);
        y += current_term;

        // Вывод промежуточных сумм
        if (i == 2) {
            std::cout << "Сумма при 3 слагаемых: " << y << std::endl;
        } else if (i == 9) {
            std::cout << "Сумма при 10 слагаемых: " << y << std::endl;
        }
    }

    std::cout << "Итоговая сумма при " << n << " слагаемых: " << y << std::endl;
}

// Функция для очистки буфера ввода
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Главное меню
void showMenu() {
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << std::endl;
    std::cout << "1. Задача 1 - Сумма чисел, делящихся на 5 и не делящихся на m" << std::endl;
    std::cout << "2. Задача 2 - Вычисление произведения" << std::endl;
    std::cout << "3. Задача 3 - Ряд Тейлора для e^(2x)" << std::endl;
    std::cout << "4. Задача 4 - Знакопеременный ряд" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите задание: ";
}

int main() {
    // Установка русской локализации для вывода
    std::setlocale(LC_ALL, "Russian");

    int choice = 0;           // Инициализация
    char continueChoice = 0;  // Инициализация

    do {
        showMenu();
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1:
                task1();
                break;
            case 2:
                task2();
                break;
            case 3:
                task3();
                break;
            case 4:
                task4();
                break;
            case 0:
                std::cout << "Выход из программы." << std::endl;
                return 0;
            default:
                std::cout << "Неверный выбор! Попробуйте снова." << std::endl;
                break;
        }

        std::cout << "\nПродолжить работу? (y/n): ";
        std::cin >> continueChoice;
        clearInputBuffer();

    } while (continueChoice == 'y' || continueChoice == 'Y');

    std::cout << "Программа завершена." << std::endl;
    return 0;
}
