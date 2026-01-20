#include <clocale>  // Для setlocale
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>  // Для std::string

// Константы для оформления вывода
namespace FormatConstants {
constexpr int PRECISION_VALUE = 6;
constexpr int COLUMN_WIDTH_X = 10;
constexpr int COLUMN_WIDTH_Y = 15;
constexpr int COLUMN_WIDTH_S = 15;
constexpr int COLUMN_WIDTH_N = 8;
constexpr int LINE_LENGTH = 48;
}  // namespace FormatConstants

// Математические константы
namespace MathConstants {
constexpr double EPSILON_TASK3 = 1e-6;
constexpr double STEP_H = 0.2;
constexpr double END_X_VALUE = 1.01;
constexpr double COMPARISON_TOLERANCE = 1e-10;
constexpr int MAX_ITERATIONS_TASK3 = 1000;
constexpr int MAX_LIMIT_TASK3 = 10000;
constexpr double TWO = 2.0;
constexpr double ONE = 1.0;
constexpr double MINUS_ONE = -1.0;
}  // namespace MathConstants

// Задача 1: Сумма чисел, делящихся на 5 и не делящихся на m
void task1() {
    std::cout << "=== ЗАДАЧА 1 ===" << std::endl;
    int n = 0;
    int m = 0;

    std::cout << "Введите n: ";
    if (!(std::cin >> n)) {
        std::cout << "Ошибка ввода числа n!" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cout << "Введите m (m < n): ";
    if (!(std::cin >> m)) {
        std::cout << "Ошибка ввода числа m!" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (m >= n) {
        std::cout << "Ошибка: m должно быть меньше n!" << std::endl;
        return;
    }

    int sum = 0;
    const int divisor = 5;
    for (int i = divisor; i <= n; i = i + divisor) {
        if (i % m != 0) {
            sum += i;
        }
    }
    std::cout << "Сумма чисел: " << sum << std::endl;
}

// Задача 2: Вычисление произведения в зависимости от a
void task2() {
    double a = 0.0;
    std::cout << "Введите число a:" << '\n';
    std::cin >> a;

    double product = 1.0;
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
    using namespace FormatConstants;
    using namespace MathConstants;

    std::cout << "=== ЗАДАЧА 3 ===" << std::endl;

    std::cout << std::fixed << std::setprecision(PRECISION_VALUE);
    std::cout << std::setw(COLUMN_WIDTH_X) << "x" << std::setw(COLUMN_WIDTH_Y) << "Y(x)" << std::setw(COLUMN_WIDTH_S) << "S(x)"
              << std::setw(COLUMN_WIDTH_N) << "N" << std::endl;
    std::cout << std::string(LINE_LENGTH, '-') << std::endl;

    // Заменяем do-while на while
    double x = 0.0;

    while (x <= END_X_VALUE + COMPARISON_TOLERANCE) {
        double Y = std::exp(TWO * x);  // Точное значение функции
        double S = 1.0;                // Сумма ряда (первый член = 1)
        double term = 1.0;             // Текущий член ряда
        int N = 0;                     // Номер последнего слагаемого

        // Вычисление суммы ряда с использованием рекуррентного соотношения
        for (int n = 1; n <= MAX_LIMIT_TASK3; ++n) {
            // Рекуррентное соотношение: term_n = term_{n-1} * (2x / n)
            term *= (TWO * x) / static_cast<double>(n);
            S += term;

            if (std::fabs(term) < EPSILON_TASK3) {
                N = n;
                break;
            }

            // Защита от бесконечного цикла
            if (n == MAX_ITERATIONS_TASK3) {
                N = MAX_ITERATIONS_TASK3;
                break;
            }
        }

        std::cout << std::setw(COLUMN_WIDTH_X) << x << std::setw(COLUMN_WIDTH_Y) << Y << std::setw(COLUMN_WIDTH_S) << S << std::setw(COLUMN_WIDTH_N)
                  << N << std::endl;

        x += STEP_H;
    }
}

// Задача 4: Вычисление знакопеременного ряда с промежуточными суммами
void task4() {
    using namespace FormatConstants;
    using namespace MathConstants;

    std::cout << "=== ЗАДАЧА 4 ===" << std::endl;
    int n = 0;
    double x = 0.0;

    std::cout << "Введите n: ";
    std::cin >> n;
    std::cout << "Введите x: ";
    std::cin >> x;

    double y = 1.0;             // Первый член ряда (n=0)
    double current_term = 1.0;  // Текущий член ряда

    std::cout << std::fixed << std::setprecision(PRECISION_VALUE);
    std::cout << "Промежуточные суммы:" << std::endl;

    for (int i = 1; i <= n; ++i) {
        // Вычисление текущего члена ряда с использованием предыдущего
        // term_i = term_{i-1} * (-1) * x * (2i+1) / (2i)
        current_term *= MINUS_ONE * x * (TWO * static_cast<double>(i) + ONE) / (TWO * static_cast<double>(i));
        y += current_term;

        // Вывод промежуточных сумм
        constexpr int TERM_COUNT_3 = 2;   // 3 слагаемых при i=2 (0,1,2)
        constexpr int TERM_COUNT_10 = 9;  // 10 слагаемых при i=9

        if (i == TERM_COUNT_3) {
            std::cout << "Сумма при 3 слагаемых: " << y << std::endl;
        } else if (i == TERM_COUNT_10) {
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

    int choice = 0;
    char continueChoice = 0;

    // Заменяем do-while на while
    bool shouldContinue = true;

    while (shouldContinue) {
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

        shouldContinue = (continueChoice == 'y' || continueChoice == 'Y');
    }

    std::cout << "Программа завершена." << std::endl;
    return 0;
}
