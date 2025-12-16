#include "lab_3_functions.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace EquationSolver {

    constexpr int MAX_ITERATIONS = 100000;

    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    double getValidatedInput(const std::string& prompt) {
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

    void printResult(double root, int iterations, const std::string& methodName, double precision) {
        std::cout << "\n=== Результаты метода " << methodName << " ===\n";
        std::cout << "Найденный корень: " << std::fixed << std::setprecision(precision) << root << "\n";
        std::cout << "Количество итераций: " << iterations << "\n\n";
    }

void runApplication() {
    std::cout << "====  Решение уравнения:  ====\n";
    std::cout << "====  1) x - k*cos(x) = 0 ====\n";
    std::cout << "====  2) cos(4x) - x * 0.5    ====\n";
    int z = 0;
    std::cin >> z;
    if (z == 1) {
        do {
            MenuOption option = displayMenu();
            if (option == MenuOption::EXIT) {
                break;
            }
            executeTask(option);
            } while (shouldContinue());
    }
    if (z == 2) {
        do {
            MenuOption option = displayMenu();
            if (option == MenuOption::EXIT) {
                break;
            }
            executeTask2(option);
            } while (shouldContinue());
    }


    std::cout << "Программа завершена.\n";
}

    double iterationMethod(double coefficient, double epsilon, int choice) {
        double x_prev = 0.5;
        double x_next = coefficient * std::cos(x_prev);
        int iterations = 0;
        double tolerance = std::pow(10.0, -epsilon);

        while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
            x_prev = x_next;
            x_next = coefficient * std::cos(x_prev);
            iterations++;
        }

        return (choice == 0) ? x_next : static_cast<double>(iterations);
    }

    double bisectionMethod(double coefficient, double left, double right, double epsilon, int choice) {
        double f_left = function(left, coefficient);
        double f_right = function(right, coefficient);

        if (f_left * f_right > 0) {
            std::cout << "Ошибка: функция на концах интервала имеет одинаковые знаки.\n";
            return 0.0;
        }

        int iterations = 0;
        double mid = 0.0;
        double tolerance = std::pow(10.0, -epsilon);

        while ((right - left) > tolerance && iterations < MAX_ITERATIONS) {
            mid = (left + right) / 2.0;
            double f_mid = function(mid, coefficient);

            if (f_left * f_mid < 0) {
                right = mid;
                f_right = f_mid;
            } else {
                left = mid;
                f_left = f_mid;
            }
            iterations++;
        }

        return (choice == 0) ? mid : static_cast<double>(iterations);
    }

    double newtonMethod(double coefficient, double epsilon, int choice) {
        double x_prev = 0.5;
        double x_next = x_prev - function(x_prev, coefficient) / derivative(x_prev, coefficient);
        int iterations = 0;
        double tolerance = std::pow(10.0, -epsilon);

        while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
            x_prev = x_next;
            x_next = x_prev - function(x_prev, coefficient) / derivative(x_prev, coefficient);
            iterations++;
        }

        return (choice == 0) ? x_next : static_cast<double>(iterations);
    }


[[nodiscard]] double iterationMethod2(double coefficient, double epsilon, int choice) {
    double x_prev = 0.5; // начальное приближение
    double x_next = pow(x_prev, 5) - x_prev + 0.2;
    int iterations = 0;

    while (std::abs(x_next - x_prev) > pow(10.0, -epsilon) && iterations < 100000) {
        x_prev = x_next;
        x_next = pow(x_prev, 5) - x_prev + 0.2;
        iterations += 1;
    }

    if (choice == 0) {
        return x_next;
    }
    else {
        return iterations;
    }
}

[[nodiscard]] double bisectionMethod2(double coefficient, double left, double right, double epsilon, int  choice) {
    if (function2(left, coefficient) * function2(right, coefficient) > 0) {
        std::cout << "Ошибка: функция на концах интервала имеет одинаковые знаки.\n";
        return 0.0;
    }

    int iterations = 0;
    double mid = 0.0;

    while ((right - left) > pow(10.0, -epsilon)) {
        mid = (left + right) / 2.0;

        if (function2(left, coefficient) * function2(mid, coefficient) < 0) {
            right = mid;
        } else {
            left = mid;
        }

        iterations += 1;
    }

    if (choice == 0) {
        return mid;
    }
    else {
        return iterations;
    }
}

    MenuOption displayMenu() {
        std::cout << "Выберите метод решения:\n";
        std::cout << "1. Метод простой итерации\n";
        std::cout << "2. Метод половинного деления\n";
        // std::cout << "3. Метод Ньютона\n";
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
        double coefficient = getValidatedInput("Введите коэффициент при cos(x): ");
        double epsilon = getValidatedInput("Введите точность (число знаков после запятой): ");

        switch (option) {
            case MenuOption::ITERATION_METHOD: {
                double root = iterationMethod(coefficient, epsilon, 0);
                int iterations = static_cast<int>(iterationMethod(coefficient, epsilon, 1));
                printResult(root, iterations, "простой итерации", epsilon);
                break;
            }
            case MenuOption::BISECTION_METHOD: {
                double left = getValidatedInput("Введите левую границу интервала: ");
                double right = getValidatedInput("Введите правую границу интервала: ");
                double root = bisectionMethod(coefficient, left, right, epsilon, 0);
                int iterations = static_cast<int>(bisectionMethod(coefficient, left, right, epsilon, 1));
                printResult(root, iterations, "половинного деления", epsilon);
                break;
            }
            case MenuOption::NEWTON_METHOD: {
                double root = newtonMethod(coefficient, epsilon, 0);
                int iterations = static_cast<int>(newtonMethod(coefficient, epsilon, 1));
                printResult(root, iterations, "Ньютона", epsilon);
                break;
            }
            default:
                break;
        }
    }



void executeTask2(MenuOption option) {
    double coefficient = 0;
    double epsilon = getValidatedInput("точность(число знаков после запятой): ");

    int iterations = 0;

    switch (option) {
        case MenuOption::ITERATION_METHOD: {
            printResult(iterationMethod2(coefficient, epsilon, 0), iterationMethod2(coefficient, epsilon, 1), "простой итерации", epsilon);
            break;
        }
        case MenuOption::BISECTION_METHOD: {
            double left = getValidatedInput("Левая граница интервала: ");
            double right = getValidatedInput("Правая граница интервала: ");
            printResult(bisectionMethod2(coefficient, left, right, epsilon, 0), bisectionMethod2(coefficient, left, right, epsilon, 1), "половинного деления", epsilon);
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

[[nodiscard]] double function2(double x, double coefficient) {
return std::cos(x) - x * 0.5;
}

} // namespace EquationSolver
