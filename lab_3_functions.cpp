#include "lab_3_functions.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace EquationSolver {

namespace {
// Константы
constexpr int MAX_ITERATIONS = 100000;
constexpr double MIN_DERIVATIVE = 1e-15;
constexpr double BASE_10 = 10.0;
constexpr double HALF = 0.5;
constexpr double FOUR = 4.0;
constexpr double TWO = 2.0;
constexpr double ONE = 1.0;

void clear_input_buffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double get_validated_input(const std::string& prompt) {
    double value = 0.0;  // Инициализация
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        std::cout << "Ошибка ввода. Пожалуйста, введите число.\n";
        clear_input_buffer();
    }
}

double calculate_tolerance(double epsilon) {
    return std::pow(BASE_10, -epsilon);
}

void print_result(const SolutionResult& result, const std::string& method_name, double epsilon) {
    std::cout << "\n=== Результаты метода " << method_name << " ===\n";

    if (!result.success) {
        std::cout << "Ошибка: " << result.error_message << "\n\n";
        return;
    }

    std::cout << "Найденный корень: " << std::fixed << std::setprecision(static_cast<int>(epsilon)) << result.root << "\n";
    std::cout << "Количество итераций: " << result.iterations << "\n";
    std::cout << "Точность вычислений: 10^(-" << epsilon << ")\n\n";
}

}  // namespace

void run_application() {
    std::cout << "=== Решение уравнений численными методами ===\n\n";

    // Заменяем do-while на while
    bool continue_work = true;
    while (continue_work) {
        EquationType equation_type = select_equation_type();
        solve_equation(equation_type);
        continue_work = should_continue();
    }

    std::cout << "Программа завершена.\n";
}

EquationType select_equation_type() {
    std::cout << "==== Выбор уравнения ====\n";
    std::cout << "1. x - k*cos(x) = 0\n";
    std::cout << "2. cos(4x) - 0.5*x = 0 (вариант 8)\n";
    std::cout << "Выберите уравнение: ";

    int choice = 0;  // Инициализация
    while (true) {
        if (std::cin >> choice) {
            auto eq_type = static_cast<EquationType>(choice);
            if (eq_type == EquationType::X_MINUS_K_COS_X || eq_type == EquationType::COS_4X_MINUS_HALF_X) {
                return eq_type;
            }
        }
        std::cout << "Неверный выбор. Пожалуйста, введите 1 или 2: ";
        clear_input_buffer();
    }
}

SolutionMethod display_menu(EquationType equation_type) {
    int choice = 0;  // Инициализация

    while (true) {
        std::cout << "\n=== Выбор метода решения ===\n";
        std::cout << "1. Метод простой итерации\n";
        std::cout << "2. Метод половинного деления\n";

        if (equation_type == EquationType::X_MINUS_K_COS_X) {
            std::cout << "3. Метод Ньютона\n";
            std::cout << "4. Выход в меню выбора уравнения\n";
        } else {
            std::cout << "3. Выход в меню выбора уравнения\n";
        }

        std::cout << "Ваш выбор: ";

        if (std::cin >> choice) {
            if (equation_type == EquationType::COS_4X_MINUS_HALF_X && choice == 3) {
                return SolutionMethod::EXIT;
            }

            auto method = static_cast<SolutionMethod>(choice);

            if (method == SolutionMethod::NEWTON && equation_type == EquationType::COS_4X_MINUS_HALF_X) {
                std::cout << "\nОШИБКА: Метод Ньютона ЗАПРЕЩЕН для уравнения cos(4x) - 0.5*x = 0\n";
                std::cout << "Рекомендуемые методы для этого уравнения: итераций и половинного деления.\n";
                std::cout << "Пожалуйста, выберите другой метод.\n";
                clear_input_buffer();
                continue;
            }

            if ((method >= SolutionMethod::ITERATION && method <= SolutionMethod::NEWTON) || method == SolutionMethod::EXIT) {
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
    char response = '\0';  // Инициализация
    std::cout << "\nПродолжить работу с другим уравнением? (y/n): ";
    std::cin >> response;
    clear_input_buffer();
    return response == 'y' || response == 'Y';
}

bool is_method_available(EquationType equation_type, SolutionMethod method) {
    if (method == SolutionMethod::NEWTON && equation_type == EquationType::COS_4X_MINUS_HALF_X) {
        return false;
    }
    return true;
}

void solve_equation(EquationType equation_type) {
    bool running = true;

    while (running) {
        SolutionMethod method = display_menu(equation_type);

        if (method == SolutionMethod::EXIT) {
            running = false;
            continue;
        }

        if (!is_method_available(equation_type, method)) {
            std::cout << "\nОШИБКА: Выбранный метод недоступен для этого уравнения.\n";
            std::cout << "Метод Ньютона запрещен для уравнения cos(4x) - 0.5*x = 0\n";
            continue;
        }

        double coefficient = DEFAULT_COEFFICIENT;

        if (equation_type == EquationType::X_MINUS_K_COS_X) {
            coefficient = get_validated_input("Введите коэффициент k при cos(x): ");
        } else {
            coefficient = COS_4X_COEFFICIENT;
        }

        double epsilon = get_validated_input("Введите точность (число знаков после запятой): ");

        SolutionResult result;

        switch (method) {
            case SolutionMethod::ITERATION: {
                std::cout << "\nДля метода итераций:\n";

                double initial_guess = 0.0;  // Инициализация
                if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
                    std::cout << "Внимание! |x| должен быть ≤ 2 для сходимости\n";
                    std::cout << "Рекомендуемое начальное приближение: 1.5\n";
                    initial_guess = get_validated_input("Введите начальное приближение: ");
                } else {
                    initial_guess = get_validated_input("Введите начальное приближение: ");
                }

                result = solve_by_iteration(equation_type, coefficient, epsilon, initial_guess);
                print_result(result, "простой итерации", epsilon);
                break;
            }

            case SolutionMethod::BISECTION: {
                std::cout << "\nДля метода половинного деления необходимо задать интервал:\n";

                double left = get_validated_input("Введите левую границу интервала: ");
                double right = get_validated_input("Введите правую границу интервала: ");

                result = solve_by_bisection(equation_type, coefficient, epsilon, left, right);
                print_result(result, "половинного деления", epsilon);
                break;
            }

            case SolutionMethod::NEWTON: {
                std::cout << "\nДля метода Ньютона:\n";
                double initial_guess = get_validated_input("Введите начальное приближение: ");

                result = solve_by_newton(equation_type, coefficient, epsilon, initial_guess);
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
                std::cout << "1. Метод Ньютона обычно сходится быстрее всего\n";
                std::cout << "2. Метод половинного деления гарантирует сходимость\n";
                std::cout << "3. Метод итераций: x = " << coefficient << "*cos(x)\n";
                std::cout << "4. Для k > 1 уравнение может иметь несколько корней\n";
            } else {
                std::cout << "Уравнение: cos(4x) - " << coefficient << "*x = 0 (вариант 8)\n";
                std::cout << "1. Доступные методы: итераций и половинного деления\n";
                std::cout << "2. Метод Ньютона ЗАПРЕЩЕН для этого уравнения\n";
                std::cout << "3. Метод итераций требует: |x| ≤ 2\n";
                std::cout << "4. Рекомендуемый интервал для половинного деления: [1.5, 1.65]\n";
            }
            std::cout << "Точность достигнута: 10^(-" << epsilon << ")\n";
        }

        std::cout << "\nНажмите Enter для продолжения...";
        clear_input_buffer();
    }
}

SolutionResult solve_by_iteration(EquationType equation_type, double coefficient, double epsilon, double initial_guess) {
    double tolerance = calculate_tolerance(epsilon);
    double x_prev = initial_guess;
    double x_next = 0.0;  // Инициализация

    if (equation_type == EquationType::X_MINUS_K_COS_X) {
        x_next = coefficient * std::cos(x_prev);
    } else {
        if (std::abs(HALF * x_prev) > ONE) {
            SolutionResult result;
            result.success = false;
            result.error_message = "Для метода итераций |x| должен быть ≤ 2";
            return result;
        }
        x_next = std::acos(HALF * x_prev) / FOUR;
    }

    int iterations = 0;

    while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
        x_prev = x_next;

        if (equation_type == EquationType::X_MINUS_K_COS_X) {
            x_next = coefficient * std::cos(x_prev);
        } else {
            if (std::abs(HALF * x_prev) > ONE) {
                SolutionResult result;
                result.success = false;
                result.error_message = "Итерация вышла за область определения arccos";
                return result;
            }
            x_next = std::acos(HALF * x_prev) / FOUR;
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

SolutionResult solve_by_bisection(EquationType equation_type, double coefficient, double epsilon, double left_bound, double right_bound) {
    double tolerance = calculate_tolerance(epsilon);
    SolutionResult result;

    // 1. Проверка корректности интервала
    if (left_bound >= right_bound) {
        result.success = false;
        result.error_message = "Левая граница должна быть меньше правой";
        return result;
    }

    // 2. Вычисляем значения функции
    double f_left = evaluate_equation(equation_type, left_bound, coefficient);
    double f_right = evaluate_equation(equation_type, right_bound, coefficient);

    // 3. Классическая проверка метода половинного деления: f(a)*f(b) < 0
    if (f_left * f_right > 0.0) {
        result.success = false;
        result.error_message = "Функция на концах интервала имеет одинаковые знаки";
        return result;
    }

    // 4. Основной алгоритм
    double left = left_bound;
    double right = right_bound;
    double mid = 0.0;  // Инициализация
    int iterations = 0;

    while ((right - left) > tolerance && iterations < MAX_ITERATIONS) {
        mid = (left + right) / TWO;
        double f_mid = evaluate_equation(equation_type, mid, coefficient);

        double f_left_val = evaluate_equation(equation_type, left, coefficient);

        if (f_left_val * f_mid <= 0.0) {
            right = mid;
        } else {
            left = mid;
        }

        iterations++;
    }

    // 5. Результат
    result.root = (left + right) / TWO;
    result.iterations = iterations;
    result.success = (iterations < MAX_ITERATIONS);

    if (!result.success) {
        result.error_message = "Превышено максимальное количество итераций";
    }

    return result;
}

SolutionResult solve_by_newton(EquationType equation_type, double coefficient, double epsilon, double initial_guess) {
    if (equation_type == EquationType::COS_4X_MINUS_HALF_X) {
        SolutionResult result;
        result.success = false;
        result.error_message = "Метод Ньютона ЗАПРЕЩЕН для уравнения cos(4x) - 0.5*x = 0";
        return result;
    }

    double tolerance = calculate_tolerance(epsilon);
    double x_prev = initial_guess;

    double derivative = evaluate_derivative(equation_type, x_prev, coefficient);

    if (std::abs(derivative) < MIN_DERIVATIVE) {
        SolutionResult result;
        result.success = false;
        result.error_message = "Производная слишком близка к нулю в начальной точке";
        return result;
    }

    double x_next = x_prev - evaluate_equation(equation_type, x_prev, coefficient) / derivative;

    int iterations = 0;

    while (std::abs(x_next - x_prev) > tolerance && iterations < MAX_ITERATIONS) {
        x_prev = x_next;

        derivative = evaluate_derivative(equation_type, x_prev, coefficient);

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

double evaluate_equation(EquationType equation_type, double x, double coefficient) {
    switch (equation_type) {
        case EquationType::X_MINUS_K_COS_X:
            return x - coefficient * std::cos(x);

        case EquationType::COS_4X_MINUS_HALF_X:
            // ВАЖНО: coefficient = 0.5 для этого уравнения!
            return std::cos(FOUR * x) - coefficient * x;

        default:
            throw std::invalid_argument("Неизвестный тип уравнения");
    }
}

double evaluate_derivative(EquationType equation_type, double x, double coefficient) {
    switch (equation_type) {
        case EquationType::X_MINUS_K_COS_X:
            return ONE + coefficient * std::sin(x);

        case EquationType::COS_4X_MINUS_HALF_X:
            return -FOUR * std::sin(FOUR * x) - coefficient;

        default:
            throw std::invalid_argument("Неизвестный тип уравнения");
    }
}

}  // namespace EquationSolver
