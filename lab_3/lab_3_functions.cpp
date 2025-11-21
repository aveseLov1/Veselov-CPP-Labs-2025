#include "lab_3_functions.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>

namespace EquationSolver {

namespace {
    constexpr int MAX_ITERATIONS = 100000;

    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    [[nodiscard]] double getValidatedInput(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                return value;
            }
            std::cout << "Ошибка ввода. Пожалуйста, введите число.\n";
            clearInputBuffer();
        }
    }

    void printResult(double root, int iterations, const std::string& methodName) {
        std::cout << "\n=== Результаты метода " << methodName << " ===\n";
        std::cout << "Найденный корень: " << std::setprecision(15) << root << "\n";
        std::cout << "Количество итераций: " << iterations << "\n";
        std::cout << "Значение функции в корне: " << function(root, 1.0) << "\n\n";
    }
}

void runApplication() {
    std::cout << "=== Решение уравнения x - k*cos(x) = 0 ===\n\n";

    do {
        MenuOption option = displayMenu();
        if (option == MenuOption::EXIT) {
            break;
        }
        executeTask(option);
    } while (shouldContinue());

    std::cout << "Программа завершена.\n";
}

MenuOption displayMenu() {
    std::cout << "Выберите метод решения:\n";
    std::cout << "1. Метод простой итерации\n";
    std::cout << "2. Метод половинного деления\n";
    std::cout << "3. Метод Ньютона\n";
    std::cout << "4. Выход\n";
    std::cout << "Ваш выбор: ";

    int choice;
    while (true) {
        if (std::cin >> choice) {
            auto option = static_cast<MenuOption>(choice);
            if (option >= MenuOption::ITERATION_METHOD && option <= MenuOption::EXIT) {
                return option;
            }
        }
        std::cout << "Неверный выбор. Пожалуйста, введите число от 1 до 4: ";
        clearInputBuffer();
    }
}

bool shouldContinue() {
    char response;
    std::cout << "Продолжить работу? (y/n): ";
    std::cin >> response;
    clearInputBuffer();
    return response == 'y' || response == 'Y';
}

void executeTask(MenuOption option) {
    double coefficient = getValidatedInput("Введите коэффициент k при cos(x): ");
    double epsilon = getValidatedInput("Введите точность epsilon (например, 1e-6): ");

    int iterations = 0;
    double root = 0.0;

    switch (option) {
        case MenuOption::ITERATION_METHOD: {
            root = iterationMethod(coefficient, epsilon, iterations);
            printResult(root, iterations, "простой итерации");
            break;
        }
        case MenuOption::BISECTION_METHOD: {
            double left = getValidatedInput("Введите левую границу интервала: ");
            double right = getValidatedInput("Введите правую границу интервала: ");
            root = bisectionMethod(coefficient, left, right, epsilon, iterations);
            printResult(root, iterations, "половинного деления");
            break;
        }
        case MenuOption::NEWTON_METHOD: {
            root = newtonMethod(coefficient, epsilon, iterations);
            printResult(root, iterations, "Ньютона");
            break;
        }
        default:
            break;
    }
}

[[nodiscard]] double function(double x, double coefficient) {
    return x - coefficient * std::cos(x);
}

[[nodiscard]] double derivative(double x, double coefficient) {
    return 1.0 + coefficient * std::sin(x);
}

[[nodiscard]] double iterationMethod(double coefficient, double epsilon, int& iterations) {
    double x_prev = 0.5; // начальное приближение
    double x_next = coefficient * std::cos(x_prev);
    iterations = 0;

    while (std::abs(x_next - x_prev) > epsilon && iterations < MAX_ITERATIONS) {
        x_prev = x_next;
        x_next = coefficient * std::cos(x_prev);
        ++iterations;
    }

    if (iterations >= MAX_ITERATIONS) {
        std::cout << "Предупреждение: достигнуто максимальное количество итераций.\n";
    }

    return x_next;
}

[[nodiscard]] double bisectionMethod(double coefficient, double left, double right, double epsilon, int& iterations) {
    if (function(left, coefficient) * function(right, coefficient) > 0) {
        std::cout << "Ошибка: функция на концах интервала имеет одинаковые знаки.\n";
        return 0.0;
    }

    iterations = 0;
    double mid = 0.0;

    while ((right - left) > epsilon && iterations < MAX_ITERATIONS) {
        mid = (left + right) / 2.0;

        if (std::abs(function(mid, coefficient)) < epsilon) {
            break;
        }

        if (function(left, coefficient) * function(mid, coefficient) < 0) {
            right = mid;
        } else {
            left = mid;
        }

        ++iterations;
    }

    if (iterations >= MAX_ITERATIONS) {
        std::cout << "Предупреждение: достигнуто максимальное количество итераций.\n";
    }

    return (left + right) / 2.0;
}

[[nodiscard]] double newtonMethod(double coefficient, double epsilon, int& iterations) {
    double x_prev = 0.5; // начальное приближение
    double x_next = x_prev - function(x_prev, coefficient) / derivative(x_prev, coefficient);
    iterations = 0;

    while (std::abs(x_next - x_prev) > epsilon && iterations < MAX_ITERATIONS) {
        x_prev = x_next;
        x_next = x_prev - function(x_prev, coefficient) / derivative(x_prev, coefficient);
        ++iterations;
    }

    if (iterations >= MAX_ITERATIONS) {
        std::cout << "Предупреждение: достигнуто максимальное количество итераций.\n";
    }

    return x_next;
}

} // namespace EquationSolver



// #include "lab_3_functions.h"
// #include <cmath>
// #include <iomanip>
// #include <iostream>

// using namespace std;

// double calculateSeries(double x, double epsilon) {
//   double sum = 0.0;
//   double term;
//   int n = 1;

//   cout << fixed << setprecision(6);
//   cout << "n\tТекущий член\tТекущая сумма" << endl;
//   cout << "------------------------------------" << endl;

//   do {
//     term = pow(-1, n + 1) * pow(x, 2 * n - 1) / (2 * n - 1);
//     sum += term;

//     cout << n << "\t" << term << "\t" << sum << endl;

//     n++;
//   } while (fabs(term) >= epsilon);

//   cout << "------------------------------------" << endl;

//   return sum;
// }

// double calculateExact(double x) { return atan(x); }

// void printResults(double sum, double exact_value) {
//   cout << "Результаты:" << endl;
//   cout << "Сумма ряда: " << sum << endl;
//   cout << "Точное значение (arctg(x)): " << exact_value << endl;
//   cout << "Разность: " << fabs(sum - exact_value) << endl;
// }
