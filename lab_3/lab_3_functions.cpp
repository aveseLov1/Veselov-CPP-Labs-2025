#include "lab_3_functions.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace EquationSolver {

namespace {
    // Вспомогательные функции в анонимном пространстве имен
    constexpr int MAX_ITERATIONS = 100000;
    constexpr double MIN_DERIVATIVE = 1e-15;
    constexpr double ITERATION_COEFFICIENT_COS_4X = 2.0;  // Для преобразования x = 2*cos(4x)

    void clear_input_buffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    [[nodiscard]] double get_validated_input(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                return value;
            }
            std::cout << "Ошибка ввода. Пожалуйста, введите число.\n";
            clear_input_buffer();
        }
    }

    [[nodiscard]] double calculate_tolerance(double epsilon) {
        return std::pow(10.0, -epsilon);
    }

    void print_result(const SolutionResult& result,
                     const std::string& method_name,
                     double epsilon) {
        std::cout << "\n=== Результаты метода " << method_name << " ===\n";

        if (!result.success) {
            std::cout << "Ошибка: " << result.error_message << "\n\n";
            return;
        }

        std::cout << "Найденный корень: "
                  << std::fixed << std::setprecision(static_cast<int>(epsilon))
                  << result.root << "\n";
        std::cout << "Количество итераций: " << result.iterations << "\n";
        std::cout << "Точность вычислений: 10^(-" << epsilon << ")\n\n";
    }

} // anonymous namespace

// Реализация интерфейсных функций
void run_application() {
    std::cout << "=== Решение уравнений численными методами ===\n\n";

    do {
        const EquationType equation_type = select_equation_type();
        solve_equation(equation_type);
    } while (should_continue());

    std::cout << "Программа завершена.\n";
}

EquationType select_equation_type() {
    std::cout << "==== Выбор уравнения ====\n";
    std::cout << "1. x - k*cos(x) = 0\n";
    std::cout << "2. cos(4x) - 0.5*x = 0 (вариант 8)\n";
    std::cout << "Выберите уравнение: ";

    int choice;
    while (true) {
        if (std::cin >> choice) {
            auto eq_type = static_cast<EquationType>(choice);
            if (eq_type == EquationType::X_MINUS_K_COS_X ||
                eq_type == EquationType::COS_4X_MINUS_HALF_X) {
                return eq_type;
            }
        }
        std::cout << "Неверный выбор. Пожалуйста, введите 1 или 2: ";
        clear_input_buffer();
    }
}

SolutionMethod display_menu(EquationType equation_type) {
    while (true) {
        std::cout << "\n=== Выбор метода решения ===\n";
        std::cout << "1. Метод простой итерации\n";
        std::cout << "2. Метод половинного деления\n";

        // Для второго уравнения метод Ньютона не показываем вообще
        if (equation_type == EquationType::X_MINUS_K_COS_X) {
            std::cout << "3. Метод Ньютона\n";
            std::cout << "4. Выход в меню выбора уравнения\n";
        } else {
            std::cout << "3. Выход в меню выбора уравнения\n";
        }

        std::cout << "Ваш выбор: ";

        int choice;
        if (std::cin >> choice) {
            // Для второго уравнения переназначаем выбор 3 на выход
            if (equation_type == EquationType::COS_4X_MINUS_HALF_X && choice == 3) {
                return SolutionMethod::EXIT;
            }

            auto method = static_cast<SolutionMethod>(choice);

            // Проверка доступности метода Ньютона для второго уравнения
            if (method == SolutionMethod::NEWTON &&
                equation_type == EquationType::COS_4X_MINUS_HALF_X) {
                std::cout << "\nОШИБКА: Метод Ньютона ЗАПРЕЩЕН для уравнения cos(4x) - 0.5*x = 0\n";
                std::cout << "Рекомендуемые методы для этого уравнения: итераций и половинного деления.\n";
                std::cout << "Пожалуйста, выберите другой метод.\n";
                clear_input_buffer();
                continue;
            }

            if ((method >= SolutionMethod::ITERATION && method <= SolutionMethod::NEWTON) ||
                method == SolutionMethod::EXIT) {
                return method;
            }
        }
        std::cout << "Неверный выбор. ";
        if (equation_type == EquationType::X_MINUS_K_COS_X) {
            std::cout << "Пожалуйста, введите число от 1 до 4: ";
        } else {
            std::cout << "Пожалуйста, введите число от 1 до 3: ";
        }
        clear_input_buffer();
    }
}

bool should_continue() {
    char response;
    std::cout << "\nПродолжить работу с другим уравнением? (y/n): ";
    std::cin >> response;
    clear_input_buffer();
    return response == 'y' || response == 'Y';
}

[[nodiscard]] bool is_method_available(
    EquationType equation_type,
    SolutionMethod method
) {
    // Метод Ньютона запрещен для уравнения cos(4x) - 0.5*x = 0
    if (method == SolutionMethod::NEWTON &&
        equation_type == EquationType::COS_4X_MINUS_HALF_X) {
        return false;
    }
    return true;
}

void solve_equation(EquationType equation_type) {
    bool running = true;

    while (running) {
        const SolutionMethod method = display_menu(equation_type);

        if (method == SolutionMethod::EXIT) {
            running = false;
            continue;
        }

        // Дополнительная проверка доступности метода
        if (!is_method_available(equation_type, method)) {
            std::cout << "\nОШИБКА: Выбранный метод недоступен для этого уравнения.\n";
            std::cout << "Метод Ньютона запрещен для уравнения cos(4x) - 0.5*x = 0\n";
            continue;
        }

        double coefficient = DEFAULT_COEFFICIENT;

        // Для первого уравнения запрашиваем коэффициент
        if (equation_type == EquationType::X_MINUS_K_COS_X) {
            coefficient = get_validated_input("Введите коэффициент k при cos(x): ");
        } else {
            coefficient = COS_4X_COEFFICIENT;  // Для второго уравнения коэффициент фиксирован
        }

        const double epsilon = get_validated_input("Введите точность (число знаков после запятой): ");

        SolutionResult result;

        switch (method) {
            case SolutionMethod::ITERATION: {
                std::cout << "\nДля метода итераций:\n";

                double initial_guess;
                if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
                    std::cout << "Рекомендуемые начальные приближения: 1.4 или 1.7\n";
                    initial_guess = get_validated_input("Введите начальное приближение: ");
                } else {
                    initial_guess = get_validated_input("Введите начальное приближение: ");
                }

                result = solve_by_iteration(
                    equation_type,
                    coefficient,
                    epsilon,
                    initial_guess
                );
                print_result(result, "простой итерации", epsilon);
                break;
            }

            case SolutionMethod::BISECTION: {
                std::cout << "\nДля метода половинного деления необходимо задать интервал:\n";

                if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
                    std::cout << "Рекомендуемый интервал: [1.4, 1.7]\n";
                }

                const double left = get_validated_input("Введите левую границу интервала: ");
                const double right = get_validated_input("Введите правую границу интервала: ");

                result = solve_by_bisection(
                    equation_type,
                    coefficient,
                    epsilon,
                    left,
                    right
                );
                print_result(result, "половинного деления", epsilon);
                break;
            }

            case SolutionMethod::NEWTON: {
                // Этот код выполнится только для первого уравнения
                // (для второго уравнения метод Ньютона запрещен)
                std::cout << "\nДля метода Ньютона:\n";
                const double initial_guess = get_validated_input(
                    "Введите начальное приближение: "
                );

                result = solve_by_newton(
                    equation_type,
                    coefficient,
                    epsilon,
                    initial_guess
                );
                print_result(result, "Ньютона", epsilon);
                break;
            }

            default:
                break;
        }

        if (result.success) {
            std::cout << "=== Объяснение результатов ===\n";

            if (equation_type == EquationType::X_MINUS_K_COS_X) {
                std::cout << "Уравнение: x - " << coefficient << "*cos(x) = 0\n";
                std::cout << "1. Метод Ньютона обычно сходится быстрее всего (квадратичная сходимость)\n";
                std::cout << "2. Метод половинного деления гарантирует сходимость при разных знаках на концах интервала\n";
                std::cout << "3. Метод итераций: x = " << coefficient << "*cos(x)\n";
                std::cout << "4. Для k > 1 уравнение может иметь несколько корней\n";
            } else {
                std::cout << "Уравнение: cos(4x) - " << coefficient << "*x = 0 (вариант 8)\n";
                std::cout << "1. Доступные методы: итераций и половинного деления\n";
                std::cout << "2. Метод Ньютона ЗАПРЕЩЕН для этого уравнения\n";
                std::cout << "3. Начальные приближения: 1.4, 1.7 (корень около 1.5)\n";
                std::cout << "4. Рекомендуемый интервал: [1.4, 1.7]\n";
            }
            std::cout << "Точность достигнута: 10^(-" << epsilon << ")\n";
        }

        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.get();
    }
}

// Реализация функций вычислений (остаются без изменений)
[[nodiscard]] SolutionResult solve_by_iteration(
    EquationType equation_type,
    double coefficient,
    double epsilon,
    double initial_guess
) {
    const double tolerance = calculate_tolerance(epsilon);
    double x_prev = initial_guess;
    double x_next;

    // Преобразование уравнения к виду x = φ(x)
    if (equation_type == EquationType::X_MINUS_K_COS_X) {
        x_next = coefficient * std::cos(x_prev);
    } else { // COS_4X_MINUS_HALF_X
        // x = 2*cos(4x)
        x_next = ITERATION_COEFFICIENT_COS_4X * std::cos(4.0 * x_prev);
    }

    int iterations = 0;

    while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
        x_prev = x_next;

        if (equation_type == EquationType::X_MINUS_K_COS_X) {
            x_next = coefficient * std::cos(x_prev);
        } else {
            x_next = ITERATION_COEFFICIENT_COS_4X * std::cos(4.0 * x_prev);
        }

        iterations++;
    }

    SolutionResult result;
    result.root = x_next;
    result.iterations = iterations;
    result.success = (iterations < MAX_ITERATIONS);

    if (!result.success) {
        result.error_message = "Превышено максимальное количество итераций";
    } else if (iterations == 0) {
        result.error_message = "Начальное приближение уже является решением";
    }

    return result;
}

[[nodiscard]] SolutionResult solve_by_bisection(
    EquationType equation_type,
    double coefficient,
    double epsilon,
    double left_bound,
    double right_bound
) {
    const double tolerance = calculate_tolerance(epsilon);

    SolutionResult result;

    // Проверка корректности интервала
    const double f_left = evaluate_equation(equation_type, left_bound, coefficient);
    const double f_right = evaluate_equation(equation_type, right_bound, coefficient);

    if (std::signbit(f_left) == std::signbit(f_right)) {
        result.success = false;
        result.error_message = "Функция на концах интервала имеет одинаковые знаки";
        return result;
    }

    if (left_bound >= right_bound) {
        result.success = false;
        result.error_message = "Левая граница должна быть меньше правой";
        return result;
    }

    double left = left_bound;
    double right = right_bound;
    double mid = 0.0;
    int iterations = 0;

    while ((right - left) > tolerance && iterations < MAX_ITERATIONS) {
        mid = (left + right) / 2.0;
        const double f_mid = evaluate_equation(equation_type, mid, coefficient);
        const double f_left_val = evaluate_equation(equation_type, left, coefficient);

        if (std::signbit(f_left_val) != std::signbit(f_mid)) {
            right = mid;
        } else {
            left = mid;
        }

        iterations++;
    }

    result.root = (left + right) / 2.0;
    result.iterations = iterations;
    result.success = (iterations < MAX_ITERATIONS);

    if (!result.success) {
        result.error_message = "Превышено максимальное количество итераций";
    }

    return result;
}

[[nodiscard]] SolutionResult solve_by_newton(
    EquationType equation_type,
    double coefficient,
    double epsilon,
    double initial_guess
) {
    // Дополнительная защита: если кто-то все-таки вызовет метод Ньютона для второго уравнения
    if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
        SolutionResult result;
        result.success = false;
        result.error_message = "Метод Ньютона ЗАПРЕЩЕН для уравнения cos(4x) - 0.5*x = 0";
        return result;
    }

    const double tolerance = calculate_tolerance(epsilon);
    double x_prev = initial_guess;

    double derivative = evaluate_derivative(equation_type, x_prev, coefficient);

    // Проверка на нулевую производную
    if (std::abs(derivative) < MIN_DERIVATIVE) {
        SolutionResult result;
        result.success = false;
        result.error_message = "Производная слишком близка к нулю в начальной точке";
        return result;
    }

    double x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;

    int iterations = 0;

    while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
        x_prev = x_prev;

        derivative = evaluate_derivative(equation_type, x_prev, coefficient);

        // Проверка на нулевую производную
        if (std::abs(derivative) < MIN_DERIVATIVE) {
            SolutionResult result;
            result.success = false;
            result.error_message = "Производная стала слишком близкой к нулю";
            return result;
        }

        x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;
        iterations++;
    }

    SolutionResult result;
    result.root = x_next;
    result.iterations = iterations;
    result.success = (iterations < MAX_ITERATIONS);

    if (!result.success) {
        result.error_message = "Превышено максимальное количество итераций";
    }

    return result;
}

[[nodiscard]] double evaluate_equation(
    EquationType equation_type,
    double x,
    double coefficient
) {
    switch (equation_type) {
        case EquationType::X_MINUS_K_COS_X:
            return x - coefficient * std::cos(x);

        case EquationType::COS_4X_MINUS_HALF_X:
            return std::cos(4.0 * x) - coefficient * x;

        default:
            throw std::invalid_argument("Неизвестный тип уравнения");
    }
}

[[nodiscard]] double evaluate_derivative(
    EquationType equation_type,
    double x,
    double coefficient
) {
    switch (equation_type) {
        case EquationType::X_MINUS_K_COS_X:
            return 1.0 + coefficient * std::sin(x);

        case EquationType::COS_4X_MINUS_HALF_X:
            return -4.0 * std::sin(4.0 * x) - coefficient;

        default:
            throw std::invalid_argument("Неизвестный тип уравнения");
    }
}

} // namespace EquationSolver




// #include "lab_3_functions.h"
// #include <iostream>
// #include <iomanip>
// #include <cmath>
// #include <limits>
// #include <stdexcept>

// namespace EquationSolver {

// namespace {
//     // Вспомогательные функции в анонимном пространстве имен
//     constexpr int MAX_ITERATIONS = 100000;
//     constexpr double MIN_DERIVATIVE = 1e-15;
//     constexpr double ITERATION_COEFFICIENT_COS_4X = 2.0;  // Для преобразования x = 2*cos(4x)

//     void clear_input_buffer() {
//         std::cin.clear();
//         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//     }

//     [[nodiscard]] double get_validated_input(const std::string& prompt) {
//         double value;
//         while (true) {
//             std::cout << prompt;
//             if (std::cin >> value) {
//                 return value;
//             }
//             std::cout << "Ошибка ввода. Пожалуйста, введите число.\n";
//             clear_input_buffer();
//         }
//     }

//     [[nodiscard]] double calculate_tolerance(double epsilon) {
//         return std::pow(10.0, -epsilon);
//     }

//     void print_result(const SolutionResult& result,
//                      const std::string& method_name,
//                      double epsilon) {
//         std::cout << "\n=== Результаты метода " << method_name << " ===\n";

//         if (!result.success) {
//             std::cout << "Ошибка: " << result.error_message << "\n\n";
//             return;
//         }

//         std::cout << "Найденный корень: "
//                   << std::fixed << std::setprecision(static_cast<int>(epsilon))
//                   << result.root << "\n";
//         std::cout << "Количество итераций: " << result.iterations << "\n";
//         std::cout << "Точность вычислений: 10^(-" << epsilon << ")\n\n";
//     }

// } // anonymous namespace

// // Реализация интерфейсных функций
// void run_application() {
//     std::cout << "=== Решение уравнений численными методами ===\n\n";

//     do {
//         const EquationType equation_type = select_equation_type();
//         solve_equation(equation_type);
//     } while (should_continue());

//     std::cout << "Программа завершена.\n";
// }

// EquationType select_equation_type() {
//     std::cout << "==== Выбор уравнения ====\n";
//     std::cout << "1. x - k*cos(x) = 0\n";
//     std::cout << "2. cos(4x) - 0.5*x = 0 (вариант 8)\n";
//     std::cout << "Выберите уравнение: ";

//     int choice;
//     while (true) {
//         if (std::cin >> choice) {
//             auto eq_type = static_cast<EquationType>(choice);
//             if (eq_type == EquationType::X_MINUS_K_COS_X ||
//                 eq_type == EquationType::COS_4X_MINUS_HALF_X) {
//                 return eq_type;
//             }
//         }
//         std::cout << "Неверный выбор. Пожалуйста, введите 1 или 2: ";
//         clear_input_buffer();
//     }
// }

// SolutionMethod display_menu(EquationType equation_type) {
//     std::cout << "\n=== Выбор метода решения ===\n";
//     std::cout << "1. Метод простой итерации\n";
//     std::cout << "2. Метод половинного деления\n";

//     // Показываем метод Ньютона для всех уравнений, но с пояснением
//     if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
//         std::cout << "3. Метод Ньютона (НЕ РЕКОМЕНДУЕТСЯ для cos(4x) - 0.5*x = 0)\n";
//     } else {
//         std::cout << "3. Метод Ньютона\n";
//     }

//     std::cout << "4. Выход в меню выбора уравнения\n";
//     std::cout << "Ваш выбор: ";

//     int choice;
//     while (true) {
//         if (std::cin >> choice) {
//             auto method = static_cast<SolutionMethod>(choice);

//             // Предупреждение для метода Ньютона для уравнения 2
//             if (method == SolutionMethod::NEWTON &&
//                 equation_type == EquationType::COS_4X_MINUS_HALF_X) {
//                 std::cout << "\nВнимание: Метод Ньютона может не работать корректно для уравнения cos(4x) - 0.5*x = 0\n";
//                 std::cout << "Рекомендуемые методы для этого уравнения: итераций и половинного деления.\n";
//                 std::cout << "Вы уверены, что хотите продолжить? (y/n): ";

//                 char confirm;
//                 std::cin >> confirm;
//                 clear_input_buffer();

//                 if (confirm != 'y' && confirm != 'Y') {
//                     std::cout << "Пожалуйста, выберите другой метод: ";
//                     continue;
//                 }
//             }

//             if (method >= SolutionMethod::ITERATION &&
//                 method <= SolutionMethod::EXIT) {
//                 return method;
//             }
//         }
//         std::cout << "Неверный выбор. Пожалуйста, введите число от 1 до 4: ";
//         clear_input_buffer();
//     }
// }

// bool should_continue() {
//     char response;
//     std::cout << "\nПродолжить работу с другим уравнением? (y/n): ";
//     std::cin >> response;
//     clear_input_buffer();
//     return response == 'y' || response == 'Y';
// }

// void solve_equation(EquationType equation_type) {
//     bool running = true;

//     while (running) {
//         const SolutionMethod method = display_menu(equation_type);

//         if (method == SolutionMethod::EXIT) {
//             running = false;
//             continue;
//         }

//         double coefficient = DEFAULT_COEFFICIENT;

//         // Для первого уравнения запрашиваем коэффициент
//         if (equation_type == EquationType::X_MINUS_K_COS_X) {
//             coefficient = get_validated_input("Введите коэффициент k при cos(x): ");
//         } else {
//             coefficient = COS_4X_COEFFICIENT;  // Для второго уравнения коэффициент фиксирован
//         }

//         const double epsilon = get_validated_input("Введите точность (число знаков после запятой): ");

//         SolutionResult result;

//         switch (method) {
//             case SolutionMethod::ITERATION: {
//                 std::cout << "\nДля метода итераций:\n";

//                 double initial_guess;
//                 if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
//                     std::cout << "Рекомендуемые начальные приближения: 1.4 или 1.7\n";
//                     initial_guess = get_validated_input("Введите начальное приближение: ");
//                 } else {
//                     initial_guess = get_validated_input("Введите начальное приближение: ");
//                 }

//                 result = solve_by_iteration(
//                     equation_type,
//                     coefficient,
//                     epsilon,
//                     initial_guess
//                 );
//                 print_result(result, "простой итерации", epsilon);
//                 break;
//             }

//             case SolutionMethod::BISECTION: {
//                 std::cout << "\nДля метода половинного деления необходимо задать интервал:\n";

//                 if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
//                     std::cout << "Рекомендуемый интервал: [1.4, 1.7]\n";
//                 }

//                 const double left = get_validated_input("Введите левую границу интервала: ");
//                 const double right = get_validated_input("Введите правую границу интервала: ");

//                 result = solve_by_bisection(
//                     equation_type,
//                     coefficient,
//                     epsilon,
//                     left,
//                     right
//                 );
//                 print_result(result, "половинного деления", epsilon);
//                 break;
//             }

//             case SolutionMethod::NEWTON: {
//                 // Метод Ньютона доступен для обоих уравнений, но с предупреждением для второго
//                 std::cout << "\nДля метода Ньютона:\n";

//                 if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
//                     std::cout << "Рекомендуемые начальные приближения: 1.4 или 1.7\n";
//                 }

//                 const double initial_guess = get_validated_input(
//                     "Введите начальное приближение: "
//                 );

//                 result = solve_by_newton(
//                     equation_type,
//                     coefficient,
//                     epsilon,
//                     initial_guess
//                 );
//                 print_result(result, "Ньютона", epsilon);
//                 break;
//             }

//             default:
//                 break;
//         }

//         if (result.success) {
//             std::cout << "=== Объяснение результатов ===\n";

//             if (equation_type == EquationType::X_MINUS_K_COS_X) {
//                 std::cout << "Уравнение: x - " << coefficient << "*cos(x) = 0\n";
//                 std::cout << "1. Метод Ньютона обычно сходится быстрее всего (квадратичная сходимость)\n";
//                 std::cout << "2. Метод половинного деления гарантирует сходимость при разных знаках на концах интервала\n";
//                 std::cout << "3. Метод итераций: x = " << coefficient << "*cos(x)\n";
//                 std::cout << "4. Для k > 1 уравнение может иметь несколько корней\n";
//             } else {
//                 std::cout << "Уравнение: cos(4x) - " << coefficient << "*x = 0 (вариант 8)\n";
//                 std::cout << "1. Рекомендуемые методы: итераций и половинного деления\n";
//                 std::cout << "2. Начальные приближения: 1.4, 1.7 (корень около 1.5)\n";
//                 std::cout << "3. Метод Ньютона может не сходиться из-за производной: f'(x) = -4*sin(4x) - " << coefficient << "\n";
//                 std::cout << "4. Производная может обращаться в ноль или менять знак\n";
//             }
//             std::cout << "Точность достигнута: 10^(-" << epsilon << ")\n";
//         }

//         std::cout << "\nНажмите Enter для продолжения...";
//         std::cin.get();
//     }
// }

// // Реализация функций вычислений
// [[nodiscard]] SolutionResult solve_by_iteration(
//     EquationType equation_type,
//     double coefficient,
//     double epsilon,
//     double initial_guess
// ) {
//     const double tolerance = calculate_tolerance(epsilon);
//     double x_prev = initial_guess;
//     double x_next;

//     // Преобразование уравнения к виду x = φ(x)
//     if (equation_type == EquationType::X_MINUS_K_COS_X) {
//         x_next = coefficient * std::cos(x_prev);
//     } else { // COS_4X_MINUS_HALF_X
//         // x = 2*cos(4x)
//         x_next = ITERATION_COEFFICIENT_COS_4X * std::cos(4.0 * x_prev);
//     }

//     int iterations = 0;

//     while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
//         x_prev = x_next;

//         if (equation_type == EquationType::X_MINUS_K_COS_X) {
//             x_next = coefficient * std::cos(x_prev);
//         } else {
//             x_next = ITERATION_COEFFICIENT_COS_4X * std::cos(4.0 * x_prev);
//         }

//         iterations++;
//     }

//     SolutionResult result;
//     result.root = x_next;
//     result.iterations = iterations;
//     result.success = (iterations < MAX_ITERATIONS);

//     if (!result.success) {
//         result.error_message = "Превышено максимальное количество итераций";
//     } else if (iterations == 0) {
//         result.error_message = "Начальное приближение уже является решением";
//     }

//     return result;
// }

// [[nodiscard]] SolutionResult solve_by_bisection(
//     EquationType equation_type,
//     double coefficient,
//     double epsilon,
//     double left_bound,
//     double right_bound
// ) {
//     const double tolerance = calculate_tolerance(epsilon);

//     SolutionResult result;

//     // Проверка корректности интервала
//     const double f_left = evaluate_equation(equation_type, left_bound, coefficient);
//     const double f_right = evaluate_equation(equation_type, right_bound, coefficient);

//     if (std::signbit(f_left) == std::signbit(f_right)) {
//         result.success = false;
//         result.error_message = "Функция на концах интервала имеет одинаковые знаки";
//         return result;
//     }

//     if (left_bound >= right_bound) {
//         result.success = false;
//         result.error_message = "Левая граница должна быть меньше правой";
//         return result;
//     }

//     double left = left_bound;
//     double right = right_bound;
//     double mid = 0.0;
//     int iterations = 0;

//     while ((right - left) > tolerance && iterations < MAX_ITERATIONS) {
//         mid = (left + right) / 2.0;
//         const double f_mid = evaluate_equation(equation_type, mid, coefficient);
//         const double f_left_val = evaluate_equation(equation_type, left, coefficient);

//         if (std::signbit(f_left_val) != std::signbit(f_mid)) {
//             right = mid;
//         } else {
//             left = mid;
//         }

//         iterations++;
//     }

//     result.root = (left + right) / 2.0;
//     result.iterations = iterations;
//     result.success = (iterations < MAX_ITERATIONS);

//     if (!result.success) {
//         result.error_message = "Превышено максимальное количество итераций";
//     }

//     return result;
// }

// [[nodiscard]] SolutionResult solve_by_newton(
//     EquationType equation_type,
//     double coefficient,
//     double epsilon,
//     double initial_guess
// ) {
//     const double tolerance = calculate_tolerance(epsilon);
//     double x_prev = initial_guess;

//     double derivative = evaluate_derivative(equation_type, x_prev, coefficient);

//     // Проверка на нулевую производную
//     if (std::abs(derivative) < MIN_DERIVATIVE) {
//         SolutionResult result;
//         result.success = false;
//         result.error_message = "Производная слишком близка к нулю в начальной точке";
//         return result;
//     }

//     double x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;

//     int iterations = 0;

//     while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
//         x_prev = x_next;

//         derivative = evaluate_derivative(equation_type, x_prev, coefficient);

//         // Проверка на нулевую производную
//         if (std::abs(derivative) < MIN_DERIVATIVE) {
//             SolutionResult result;
//             result.success = false;
//             result.error_message = "Производная стала слишком близкой к нулю";
//             return result;
//         }

//         x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;
//         iterations++;
//     }

//     SolutionResult result;
//     result.root = x_next;
//     result.iterations = iterations;
//     result.success = (iterations < MAX_ITERATIONS);

//     if (!result.success) {
//         result.error_message = "Превышено максимальное количество итераций";
//     }

//     return result;
// }

// [[nodiscard]] double evaluate_equation(
//     EquationType equation_type,
//     double x,
//     double coefficient
// ) {
//     switch (equation_type) {
//         case EquationType::X_MINUS_K_COS_X:
//             return x - coefficient * std::cos(x);

//         case EquationType::COS_4X_MINUS_HALF_X:
//             return std::cos(4.0 * x) - coefficient * x;

//         default:
//             throw std::invalid_argument("Неизвестный тип уравнения");
//     }
// }

// [[nodiscard]] double evaluate_derivative(
//     EquationType equation_type,
//     double x,
//     double coefficient
// ) {
//     switch (equation_type) {
//         case EquationType::X_MINUS_K_COS_X:
//             return 1.0 + coefficient * std::sin(x);

//         case EquationType::COS_4X_MINUS_HALF_X:
//             return -4.0 * std::sin(4.0 * x) - coefficient;

//         default:
//             throw std::invalid_argument("Неизвестный тип уравнения");
//     }
// }

// } // namespace EquationSolver






// // #include "lab_3_functions.h"
// // #include <iostream>
// // #include <iomanip>
// // #include <cmath>
// // #include <limits>
// // #include <stdexcept>

// // namespace EquationSolver {

// // namespace {
// //     // Вспомогательные функции в анонимном пространстве имен
// //     constexpr int MAX_ITERATIONS = 100000;

// //     void clear_input_buffer() {
// //         std::cin.clear();
// //         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// //     }

// //     [[nodiscard]] double get_validated_input(const std::string& prompt) {
// //         double value;
// //         while (true) {
// //             std::cout << prompt;
// //             if (std::cin >> value) {
// //                 return value;
// //             }
// //             std::cout << "Ошибка ввода. Пожалуйста, введите число.\n";
// //             clear_input_buffer();
// //         }
// //     }

// //     [[nodiscard]] double calculate_tolerance(double epsilon) {
// //         return std::pow(10.0, -epsilon);
// //     }

// //     void print_result(const SolutionResult& result,
// //                      const std::string& method_name,
// //                      double epsilon) {
// //         std::cout << "\n=== Результаты метода " << method_name << " ===\n";

// //         if (!result.success) {
// //             std::cout << "Ошибка: " << result.error_message << "\n\n";
// //             return;
// //         }

// //         std::cout << "Найденный корень: "
// //                   << std::fixed << std::setprecision(static_cast<int>(epsilon))
// //                   << result.root << "\n";
// //         std::cout << "Количество итераций: " << result.iterations << "\n";
// //         std::cout << "Точность вычислений: 10^(-" << epsilon << ")\n\n";
// //     }

// // } // anonymous namespace

// // // Реализация интерфейсных функций
// // void run_application() {
// //     std::cout << "=== Решение уравнений численными методами ===\n\n";

// //     do {
// //         const EquationType equation_type = select_equation_type();
// //         solve_equation(equation_type);
// //     } while (should_continue());

// //     std::cout << "Программа завершена.\n";
// // }

// // EquationType select_equation_type() {
// //     std::cout << "==== Выбор уравнения ====\n";
// //     std::cout << "1. x - k*cos(x) = 0\n";
// //     std::cout << "2. cos(4x) - 0.5*x = 0 (вариант 8)\n";
// //     std::cout << "Выберите уравнение: ";

// //     int choice;
// //     while (true) {
// //         if (std::cin >> choice) {
// //             auto eq_type = static_cast<EquationType>(choice);
// //             if (eq_type == EquationType::X_MINUS_K_COS_X ||
// //                 eq_type == EquationType::COS_4X_MINUS_HALF_X) {
// //                 return eq_type;
// //             }
// //         }
// //         std::cout << "Неверный выбор. Пожалуйста, введите 1 или 2: ";
// //         clear_input_buffer();
// //     }
// // }

// // SolutionMethod display_menu(EquationType equation_type) {
// //     std::cout << "\n=== Выбор метода решения ===\n";
// //     std::cout << "1. Метод простой итерации\n";
// //     std::cout << "2. Метод половинного деления\n";

// //     // Показываем метод Ньютона для всех уравнений, но с пояснением
// //     if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// //         std::cout << "3. Метод Ньютона (НЕ РЕКОМЕНДУЕТСЯ для cos(4x) - 0.5*x = 0)\n";
// //     } else {
// //         std::cout << "3. Метод Ньютона\n";
// //     }

// //     std::cout << "4. Выход в меню выбора уравнения\n";
// //     std::cout << "Ваш выбор: ";

// //     int choice;
// //     while (true) {
// //         if (std::cin >> choice) {
// //             auto method = static_cast<SolutionMethod>(choice);

// //             // Предупреждение для метода Ньютона для уравнения 2
// //             if (method == SolutionMethod::NEWTON &&
// //                 equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// //                 std::cout << "\nВнимание: Метод Ньютона может не работать корректно для уравнения cos(4x) - 0.5*x = 0\n";
// //                 std::cout << "Рекомендуемые методы для этого уравнения: итераций и половинного деления.\n";
// //                 std::cout << "Вы уверены, что хотите продолжить? (y/n): ";

// //                 char confirm;
// //                 std::cin >> confirm;
// //                 clear_input_buffer();

// //                 if (confirm != 'y' && confirm != 'Y') {
// //                     std::cout << "Пожалуйста, выберите другой метод: ";
// //                     continue;
// //                 }
// //             }

// //             if (method >= SolutionMethod::ITERATION &&
// //                 method <= SolutionMethod::EXIT) {
// //                 return method;
// //             }
// //         }
// //         std::cout << "Неверный выбор. Пожалуйста, введите число от 1 до 4: ";
// //         clear_input_buffer();
// //     }
// // }

// // bool should_continue() {
// //     char response;
// //     std::cout << "\nПродолжить работу с другим уравнением? (y/n): ";
// //     std::cin >> response;
// //     clear_input_buffer();
// //     return response == 'y' || response == 'Y';
// // }

// // void solve_equation(EquationType equation_type) {
// //     bool running = true;

// //     while (running) {
// //         const SolutionMethod method = display_menu(equation_type);

// //         if (method == SolutionMethod::EXIT) {
// //             running = false;
// //             continue;
// //         }

// //         double coefficient = 0.0;

// //         // Для первого уравнения запрашиваем коэффициент
// //         if (equation_type == EquationType::X_MINUS_K_COS_X) {
// //             coefficient = get_validated_input("Введите коэффициент k при cos(x): ");
// //         }

// //         const double epsilon = get_validated_input("Введите точность (число знаков после запятой): ");

// //         SolutionResult result;

// //         switch (method) {
// //             case SolutionMethod::ITERATION: {
// //                 std::cout << "\nДля метода итераций:\n";

// //                 double initial_guess;
// //                 if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// //                     std::cout << "Рекомендуемые начальные приближения: 1.4 или 1.7\n";
// //                     initial_guess = get_validated_input("Введите начальное приближение: ");
// //                 } else {
// //                     initial_guess = get_validated_input("Введите начальное приближение: ");
// //                 }

// //                 result = solve_by_iteration(
// //                     equation_type,
// //                     coefficient,
// //                     epsilon,
// //                     initial_guess
// //                 );
// //                 print_result(result, "простой итерации", epsilon);
// //                 break;
// //             }

// //             case SolutionMethod::BISECTION: {
// //                 std::cout << "\nДля метода половинного деления необходимо задать интервал:\n";

// //                 if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// //                     std::cout << "Рекомендуемый интервал: [1.4, 1.7]\n";
// //                 }

// //                 const double left = get_validated_input("Введите левую границу интервала: ");
// //                 const double right = get_validated_input("Введите правую границу интервала: ");

// //                 result = solve_by_bisection(
// //                     equation_type,
// //                     coefficient,
// //                     epsilon,
// //                     left,
// //                     right
// //                 );
// //                 print_result(result, "половинного деления", epsilon);
// //                 break;
// //             }

// //             case SolutionMethod::NEWTON: {
// //                 // Метод Ньютона доступен для обоих уравнений, но с предупреждением для второго
// //                 std::cout << "\nДля метода Ньютона:\n";

// //                 if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// //                     std::cout << "Рекомендуемые начальные приближения: 1.4 или 1.7\n";
// //                 }

// //                 const double initial_guess = get_validated_input(
// //                     "Введите начальное приближение: "
// //                 );

// //                 result = solve_by_newton(
// //                     equation_type,
// //                     coefficient,
// //                     epsilon,
// //                     initial_guess
// //                 );
// //                 print_result(result, "Ньютона", epsilon);
// //                 break;
// //             }

// //             default:
// //                 break;
// //         }

// //         if (result.success) {
// //             std::cout << "=== Объяснение результатов ===\n";

// //             if (equation_type == EquationType::X_MINUS_K_COS_X) {
// //                 std::cout << "Уравнение: x - " << coefficient << "*cos(x) = 0\n";
// //                 std::cout << "1. Метод Ньютона обычно сходится быстрее всего (квадратичная сходимость)\n";
// //                 std::cout << "2. Метод половинного деления гарантирует сходимость при разных знаках на концах интервала\n";
// //                 std::cout << "3. Метод итераций: x = " << coefficient << "*cos(x)\n";
// //                 std::cout << "4. Для k > 1 уравнение может иметь несколько корней\n";
// //             } else {
// //                 std::cout << "Уравнение: cos(4x) - 0.5*x = 0 (вариант 8)\n";
// //                 std::cout << "1. Рекомендуемые методы: итераций и половинного деления\n";
// //                 std::cout << "2. Начальные приближения: 1.4, 1.7 (корень около 1.5)\n";
// //                 std::cout << "3. Метод Ньютона может не сходиться из-за производной: f'(x) = -4*sin(4x) - 0.5\n";
// //                 std::cout << "4. Производная может обращаться в ноль или менять знак\n";
// //             }
// //             std::cout << "Точность достигнута: 10^(-" << epsilon << ")\n";
// //         }

// //         std::cout << "\nНажмите Enter для продолжения...";
// //         std::cin.get();
// //     }
// // }

// // // Реализация функций вычислений
// // [[nodiscard]] SolutionResult solve_by_iteration(
// //     EquationType equation_type,
// //     double coefficient,
// //     double epsilon,
// //     double initial_guess
// // ) {
// //     const double tolerance = calculate_tolerance(epsilon);
// //     double x_prev = initial_guess;
// //     double x_next;

// //     // Преобразование уравнения к виду x = φ(x)
// //     if (equation_type == EquationType::X_MINUS_K_COS_X) {
// //         x_next = coefficient * std::cos(x_prev);
// //     } else { // COS_4X_MINUS_HALF_X
// //         // x = 2*cos(4x)
// //         x_next = 2.0 * std::cos(4.0 * x_prev);
// //     }

// //     int iterations = 0;

// //     while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
// //         x_prev = x_next;

// //         if (equation_type == EquationType::X_MINUS_K_COS_X) {
// //             x_next = coefficient * std::cos(x_prev);
// //         } else {
// //             x_next = 2.0 * std::cos(4.0 * x_prev);
// //         }

// //         iterations++;
// //     }

// //     SolutionResult result;
// //     result.root = x_next;
// //     result.iterations = iterations;
// //     result.success = (iterations < MAX_ITERATIONS);

// //     if (!result.success) {
// //         result.error_message = "Превышено максимальное количество итераций";
// //     } else if (iterations == 0) {
// //         result.error_message = "Начальное приближение уже является решением";
// //     }

// //     return result;
// // }

// // [[nodiscard]] SolutionResult solve_by_bisection(
// //     EquationType equation_type,
// //     double coefficient,
// //     double epsilon,
// //     double left_bound,
// //     double right_bound
// // ) {
// //     const double tolerance = calculate_tolerance(epsilon);

// //     SolutionResult result;

// //     // Проверка корректности интервала
// //     const double f_left = evaluate_equation(equation_type, left_bound, coefficient);
// //     const double f_right = evaluate_equation(equation_type, right_bound, coefficient);

// //     if (std::signbit(f_left) == std::signbit(f_right)) {
// //         result.success = false;
// //         result.error_message = "Функция на концах интервала имеет одинаковые знаки";
// //         return result;
// //     }

// //     if (left_bound >= right_bound) {
// //         result.success = false;
// //         result.error_message = "Левая граница должна быть меньше правой";
// //         return result;
// //     }

// //     double left = left_bound;
// //     double right = right_bound;
// //     double mid = 0.0;
// //     int iterations = 0;

// //     while ((right - left) > tolerance && iterations < MAX_ITERATIONS) {
// //         mid = (left + right) / 2.0;
// //         const double f_mid = evaluate_equation(equation_type, mid, coefficient);
// //         const double f_left_val = evaluate_equation(equation_type, left, coefficient);

// //         if (std::signbit(f_left_val) != std::signbit(f_mid)) {
// //             right = mid;
// //         } else {
// //             left = mid;
// //         }

// //         iterations++;
// //     }

// //     result.root = (left + right) / 2.0;
// //     result.iterations = iterations;
// //     result.success = (iterations < MAX_ITERATIONS);

// //     if (!result.success) {
// //         result.error_message = "Превышено максимальное количество итераций";
// //     }

// //     return result;
// // }

// // [[nodiscard]] SolutionResult solve_by_newton(
// //     EquationType equation_type,
// //     double coefficient,
// //     double epsilon,
// //     double initial_guess
// // ) {
// //     const double tolerance = calculate_tolerance(epsilon);
// //     double x_prev = initial_guess;

// //     double derivative = evaluate_derivative(equation_type, x_prev, coefficient);

// //     // Проверка на нулевую производную
// //     if (std::abs(derivative) < 1e-15) {
// //         SolutionResult result;
// //         result.success = false;
// //         result.error_message = "Производная слишком близка к нулю в начальной точке";
// //         return result;
// //     }

// //     double x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;

// //     int iterations = 0;

// //     while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
// //         x_prev = x_next;

// //         derivative = evaluate_derivative(equation_type, x_prev, coefficient);

// //         // Проверка на нулевую производную
// //         if (std::abs(derivative) < 1e-15) {
// //             SolutionResult result;
// //             result.success = false;
// //             result.error_message = "Производная стала слишком близкой к нулю";
// //             return result;
// //         }

// //         x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;
// //         iterations++;
// //     }

// //     SolutionResult result;
// //     result.root = x_next;
// //     result.iterations = iterations;
// //     result.success = (iterations < MAX_ITERATIONS);

// //     if (!result.success) {
// //         result.error_message = "Превышено максимальное количество итераций";
// //     }

// //     return result;
// // }

// // [[nodiscard]] double evaluate_equation(
// //     EquationType equation_type,
// //     double x,
// //     double coefficient
// // ) {
// //     switch (equation_type) {
// //         case EquationType::X_MINUS_K_COS_X:
// //             return x - coefficient * std::cos(x);

// //         case EquationType::COS_4X_MINUS_HALF_X:
// //             return std::cos(4.0 * x) - 0.5 * x;

// //         default:
// //             throw std::invalid_argument("Неизвестный тип уравнения");
// //     }
// // }

// // [[nodiscard]] double evaluate_derivative(
// //     EquationType equation_type,
// //     double x,
// //     double coefficient
// // ) {
// //     switch (equation_type) {
// //         case EquationType::X_MINUS_K_COS_X:
// //             return 1.0 + coefficient * std::sin(x);

// //         case EquationType::COS_4X_MINUS_HALF_X:
// //             return -4.0 * std::sin(4.0 * x) - 0.5;

// //         default:
// //             throw std::invalid_argument("Неизвестный тип уравнения");
// //     }
// // }

// // } // namespace EquationSolver



// // // #include "lab_3_functions.h"
// // // #include <iostream>
// // // #include <iomanip>
// // // #include <cmath>
// // // #include <limits>
// // // #include <stdexcept>

// // // namespace EquationSolver {

// // // namespace {
// // //     // Вспомогательные функции в анонимном пространстве имен
// // //     constexpr int MAX_ITERATIONS = 100000;

// // //     void clear_input_buffer() {
// // //         std::cin.clear();
// // //         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// // //     }

// // //     [[nodiscard]] double get_validated_input(const std::string& prompt) {
// // //         double value;
// // //         while (true) {
// // //             std::cout << prompt;
// // //             if (std::cin >> value) {
// // //                 return value;
// // //             }
// // //             std::cout << "Ошибка ввода. Пожалуйста, введите число.\n";
// // //             clear_input_buffer();
// // //         }
// // //     }

// // //     [[nodiscard]] double calculate_tolerance(double epsilon) {
// // //         return std::pow(10.0, -epsilon);
// // //     }

// // //     void print_result(const SolutionResult& result,
// // //                      const std::string& method_name,
// // //                      double epsilon) {
// // //         std::cout << "\n=== Результаты метода " << method_name << " ===\n";

// // //         if (!result.success) {
// // //             std::cout << "Ошибка: " << result.error_message << "\n\n";
// // //             return;
// // //         }

// // //         std::cout << "Найденный корень: "
// // //                   << std::fixed << std::setprecision(static_cast<int>(epsilon))
// // //                   << result.root << "\n";
// // //         std::cout << "Количество итераций: " << result.iterations << "\n";
// // //         std::cout << "Точность вычислений: 10^(-" << epsilon << ")\n\n";
// // //     }

// // // } // anonymous namespace

// // // // Реализация интерфейсных функций
// // // void run_application() {
// // //     std::cout << "=== Решение уравнений численными методами ===\n\n";

// // //     do {
// // //         const EquationType equation_type = select_equation_type();
// // //         solve_equation(equation_type);
// // //     } while (should_continue());

// // //     std::cout << "Программа завершена.\n";
// // // }

// // // EquationType select_equation_type() {
// // //     std::cout << "==== Выбор уравнения ====\n";
// // //     std::cout << "1. x - k*cos(x) = 0\n";
// // //     std::cout << "2. cos(4x) - 0.5*x = 0 (вариант 8)\n";
// // //     std::cout << "Выберите уравнение: ";

// // //     int choice;
// // //     while (true) {
// // //         if (std::cin >> choice) {
// // //             auto eq_type = static_cast<EquationType>(choice);
// // //             if (eq_type == EquationType::X_MINUS_K_COS_X ||
// // //                 eq_type == EquationType::COS_4X_MINUS_HALF_X) {
// // //                 return eq_type;
// // //             }
// // //         }
// // //         std::cout << "Неверный выбор. Пожалуйста, введите 1 или 2: ";
// // //         clear_input_buffer();
// // //     }
// // // }

// // // SolutionMethod display_menu(EquationType equation_type) {
// // //     std::cout << "\n=== Выбор метода решения ===\n";
// // //     std::cout << "1. Метод простой итерации\n";
// // //     std::cout << "2. Метод половинного деления\n";

// // //     if (is_method_available(equation_type, SolutionMethod::NEWTON)) {
// // //         std::cout << "3. Метод Ньютона\n";
// // //     } else {
// // //         std::cout << "3. Метод Ньютона (НЕДОСТУПЕН для этого уравнения)\n";
// // //     }

// // //     std::cout << "4. Выход в меню выбора уравнения\n";
// // //     std::cout << "Ваш выбор: ";

// // //     int choice;
// // //     while (true) {
// // //         if (std::cin >> choice) {
// // //             auto method = static_cast<SolutionMethod>(choice);

// // //             // Проверка доступности метода Ньютона для уравнения 2
// // //             if (method == SolutionMethod::NEWTON &&
// // //                 equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// // //                 std::cout << "Ошибка: Метод Ньютона не работает для уравнения cos(4x) - 0.5*x = 0\n";
// // //                 std::cout << "Пожалуйста, выберите другой метод (1, 2 или 4): ";
// // //                 continue;
// // //             }

// // //             if (method >= SolutionMethod::ITERATION &&
// // //                 method <= SolutionMethod::EXIT) {
// // //                 return method;
// // //             }
// // //         }
// // //         std::cout << "Неверный выбор. Пожалуйста, введите число от 1 до 4: ";
// // //         clear_input_buffer();
// // //     }
// // // }

// // // bool should_continue() {
// // //     char response;
// // //     std::cout << "\nПродолжить работу с другим уравнением? (y/n): ";
// // //     std::cin >> response;
// // //     clear_input_buffer();
// // //     return response == 'y' || response == 'Y';
// // // }

// // // bool is_method_available(EquationType equation_type, SolutionMethod method) {
// // //     // Метод Ньютона не работает для уравнения cos(4x) - 0.5*x = 0
// // //     if (method == SolutionMethod::NEWTON &&
// // //         equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// // //         return false;
// // //     }
// // //     return true;
// // // }

// // // void solve_equation(EquationType equation_type) {
// // //     bool running = true;

// // //     while (running) {
// // //         const SolutionMethod method = display_menu(equation_type);

// // //         if (method == SolutionMethod::EXIT) {
// // //             running = false;
// // //             continue;
// // //         }

// // //         double coefficient = 0.0;

// // //         // Для первого уравнения запрашиваем коэффициент
// // //         if (equation_type == EquationType::X_MINUS_K_COS_X) {
// // //             coefficient = get_validated_input("Введите коэффициент k при cos(x): ");
// // //         }

// // //         const double epsilon = get_validated_input("Введите точность (число знаков после запятой): ");

// // //         SolutionResult result;

// // //         switch (method) {
// // //             case SolutionMethod::ITERATION: {
// // //                 std::cout << "\nДля метода итераций:\n";

// // //                 double initial_guess;
// // //                 if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// // //                     std::cout << "Рекомендуемые начальные приближения: 1.4 или 1.7\n";
// // //                     initial_guess = get_validated_input("Введите начальное приближение: ");
// // //                 } else {
// // //                     initial_guess = get_validated_input("Введите начальное приближение: ");
// // //                 }

// // //                 result = solve_by_iteration(
// // //                     equation_type,
// // //                     coefficient,
// // //                     epsilon,
// // //                     initial_guess
// // //                 );
// // //                 print_result(result, "простой итерации", epsilon);
// // //                 break;
// // //             }

// // //             case SolutionMethod::BISECTION: {
// // //                 std::cout << "\nДля метода половинного деления необходимо задать интервал:\n";

// // //                 if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// // //                     std::cout << "Рекомендуемый интервал: [1.4, 1.7]\n";
// // //                 }

// // //                 const double left = get_validated_input("Введите левую границу интервала: ");
// // //                 const double right = get_validated_input("Введите правую границу интервала: ");

// // //                 result = solve_by_bisection(
// // //                     equation_type,
// // //                     coefficient,
// // //                     epsilon,
// // //                     left,
// // //                     right
// // //                 );
// // //                 print_result(result, "половинного деления", epsilon);
// // //                 break;
// // //             }

// // //             case SolutionMethod::NEWTON: {
// // //                 // Этот код выполнится только для первого уравнения
// // //                 std::cout << "\nДля метода Ньютона:\n";
// // //                 const double initial_guess = get_validated_input(
// // //                     "Введите начальное приближение: "
// // //                 );

// // //                 result = solve_by_newton(
// // //                     equation_type,
// // //                     coefficient,
// // //                     epsilon,
// // //                     initial_guess
// // //                 );
// // //                 print_result(result, "Ньютона", epsilon);
// // //                 break;
// // //             }

// // //             default:
// // //                 break;
// // //         }

// // //         if (result.success) {
// // //             std::cout << "=== Объяснение результатов ===\n";

// // //             if (equation_type == EquationType::X_MINUS_K_COS_X) {
// // //                 std::cout << "Уравнение: x - " << coefficient << "*cos(x) = 0\n";
// // //                 std::cout << "1. Метод Ньютона обычно сходится быстрее всего\n";
// // //                 std::cout << "2. Метод половинного деления гарантирует сходимость\n";
// // //                 std::cout << "3. Метод итераций: x = " << coefficient << "*cos(x)\n";
// // //             } else {
// // //                 std::cout << "Уравнение: cos(4x) - 0.5*x = 0 (вариант 8)\n";
// // //                 std::cout << "1. Рекомендуемые методы: итераций и половинного деления\n";
// // //                 std::cout << "2. Начальные приближения: 1.4, 1.7\n";
// // //                 std::cout << "3. Метод Ньютона не работает для этого уравнения\n";
// // //                 std::cout << "4. Корень находится вблизи 1.5\n";
// // //             }
// // //             std::cout << "Точность достигнута: 10^(-" << epsilon << ")\n";
// // //         }

// // //         std::cout << "\nНажмите Enter для продолжения...";
// // //         std::cin.get();
// // //     }
// // // }

// // // // Реализация функций вычислений
// // // [[nodiscard]] SolutionResult solve_by_iteration(
// // //     EquationType equation_type,
// // //     double coefficient,
// // //     double epsilon,
// // //     double initial_guess
// // // ) {
// // //     const double tolerance = calculate_tolerance(epsilon);
// // //     double x_prev = initial_guess;
// // //     double x_next;

// // //     // Преобразование уравнения к виду x = φ(x)
// // //     if (equation_type == EquationType::X_MINUS_K_COS_X) {
// // //         x_next = coefficient * std::cos(x_prev);
// // //     } else { // COS_4X_MINUS_HALF_X
// // //         // x = 2*cos(4x)
// // //         x_next = 2.0 * std::cos(4.0 * x_prev);
// // //     }

// // //     int iterations = 0;

// // //     while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
// // //         x_prev = x_next;

// // //         if (equation_type == EquationType::X_MINUS_K_COS_X) {
// // //             x_next = coefficient * std::cos(x_prev);
// // //         } else {
// // //             x_next = 2.0 * std::cos(4.0 * x_prev);
// // //         }

// // //         iterations++;
// // //     }

// // //     SolutionResult result;
// // //     result.root = x_next;
// // //     result.iterations = iterations;
// // //     result.success = (iterations < MAX_ITERATIONS);

// // //     if (!result.success) {
// // //         result.error_message = "Превышено максимальное количество итераций";
// // //     } else if (iterations == 0) {
// // //         result.error_message = "Начальное приближение уже является решением";
// // //     }

// // //     return result;
// // // }

// // // [[nodiscard]] SolutionResult solve_by_bisection(
// // //     EquationType equation_type,
// // //     double coefficient,
// // //     double epsilon,
// // //     double left_bound,
// // //     double right_bound
// // // ) {
// // //     const double tolerance = calculate_tolerance(epsilon);

// // //     SolutionResult result;

// // //     // Проверка корректности интервала
// // //     const double f_left = evaluate_equation(equation_type, left_bound, coefficient);
// // //     const double f_right = evaluate_equation(equation_type, right_bound, coefficient);

// // //     if (std::signbit(f_left) == std::signbit(f_right)) {
// // //         result.success = false;
// // //         result.error_message = "Функция на концах интервала имеет одинаковые знаки";
// // //         return result;
// // //     }

// // //     if (left_bound >= right_bound) {
// // //         result.success = false;
// // //         result.error_message = "Левая граница должна быть меньше правой";
// // //         return result;
// // //     }

// // //     double left = left_bound;
// // //     double right = right_bound;
// // //     double mid = 0.0;
// // //     int iterations = 0;

// // //     while ((right - left) > tolerance && iterations < MAX_ITERATIONS) {
// // //         mid = (left + right) / 2.0;
// // //         const double f_mid = evaluate_equation(equation_type, mid, coefficient);
// // //         const double f_left_val = evaluate_equation(equation_type, left, coefficient);

// // //         if (std::signbit(f_left_val) != std::signbit(f_mid)) {
// // //             right = mid;
// // //         } else {
// // //             left = mid;
// // //         }

// // //         iterations++;
// // //     }

// // //     result.root = (left + right) / 2.0;
// // //     result.iterations = iterations;
// // //     result.success = (iterations < MAX_ITERATIONS);

// // //     if (!result.success) {
// // //         result.error_message = "Превышено максимальное количество итераций";
// // //     }

// // //     return result;
// // // }

// // // [[nodiscard]] SolutionResult solve_by_newton(
// // //     EquationType equation_type,
// // //     double coefficient,
// // //     double epsilon,
// // //     double initial_guess
// // // ) {
// // //     // Для уравнения cos(4x) - 0.5*x = 0 метод Ньютона не должен вызываться
// // //     if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
// // //         SolutionResult result;
// // //         result.success = false;
// // //         result.error_message = "Метод Ньютона не работает для этого уравнения";
// // //         return result;
// // //     }

// // //     const double tolerance = calculate_tolerance(epsilon);
// // //     double x_prev = initial_guess;

// // //     double derivative = evaluate_derivative(equation_type, x_prev, coefficient);

// // //     // Проверка на нулевую производную
// // //     if (std::abs(derivative) < 1e-15) {
// // //         SolutionResult result;
// // //         result.success = false;
// // //         result.error_message = "Производная слишком близка к нулю в начальной точке";
// // //         return result;
// // //     }

// // //     double x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;

// // //     int iterations = 0;

// // //     while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
// // //         x_prev = x_next;

// // //         derivative = evaluate_derivative(equation_type, x_prev, coefficient);

// // //         // Проверка на нулевую производную
// // //         if (std::abs(derivative) < 1e-15) {
// // //             SolutionResult result;
// // //             result.success = false;
// // //             result.error_message = "Производная стала слишком близкой к нулю";
// // //             return result;
// // //         }

// // //         x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;
// // //         iterations++;
// // //     }

// // //     SolutionResult result;
// // //     result.root = x_next;
// // //     result.iterations = iterations;
// // //     result.success = (iterations < MAX_ITERATIONS);

// // //     if (!result.success) {
// // //         result.error_message = "Превышено максимальное количество итераций";
// // //     }

// // //     return result;
// // // }

// // // [[nodiscard]] double evaluate_equation(
// // //     EquationType equation_type,
// // //     double x,
// // //     double coefficient
// // // ) {
// // //     switch (equation_type) {
// // //         case EquationType::X_MINUS_K_COS_X:
// // //             return x - coefficient * std::cos(x);

// // //         case EquationType::COS_4X_MINUS_HALF_X:
// // //             return std::cos(4.0 * x) - 0.5 * x;

// // //         default:
// // //             throw std::invalid_argument("Неизвестный тип уравнения");
// // //     }
// // // }

// // // [[nodiscard]] double evaluate_derivative(
// // //     EquationType equation_type,
// // //     double x,
// // //     double coefficient
// // // ) {
// // //     switch (equation_type) {
// // //         case EquationType::X_MINUS_K_COS_X:
// // //             return 1.0 + coefficient * std::sin(x);

// // //         case EquationType::COS_4X_MINUS_HALF_X:
// // //             // Для информации, но метод Ньютона не используется для этого уравнения
// // //             return -4.0 * std::sin(4.0 * x) - 0.5;

// // //         default:
// // //             throw std::invalid_argument("Неизвестный тип уравнения");
// // //     }
// // // }

// // // } // namespace EquationSolver
