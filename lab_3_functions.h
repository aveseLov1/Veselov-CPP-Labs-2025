#ifndef EQUATION_SOLVER_HPP
#define EQUATION_SOLVER_HPP

#include <string>

namespace EquationSolver {

// Константы
constexpr double DEFAULT_INITIAL_GUESS = 0.5;
constexpr double DEFAULT_COEFFICIENT = 1.0;
constexpr double COS_4X_COEFFICIENT = 0.5;  // Для уравнения cos(4x) - 0.5*x = 0

// Типы уравнений
enum class EquationType {
    X_MINUS_K_COS_X = 1,      // Старое: x - k*cos(x) = 0
    COS_4X_MINUS_HALF_X = 2   // Новое: cos(4x) - 0.5*x = 0
};

// Методы решения
enum class SolutionMethod {
    ITERATION = 1,
    BISECTION = 2,
    NEWTON = 3,
    EXIT = 4
};

// Результат вычисления
struct SolutionResult {
    double root;
    int iterations;
    bool success;
    std::string error_message;
};

// Главная функция приложения
void run_application();

// Функции для работы с меню
SolutionMethod display_menu(EquationType equation_type);
bool should_continue();

// Выбор уравнения
EquationType select_equation_type();
void solve_equation(EquationType equation_type);

// Интерфейсные функции вычислений
[[nodiscard]] SolutionResult solve_by_iteration(
    EquationType equation_type,
    double coefficient,
    double epsilon,
    double initial_guess = DEFAULT_INITIAL_GUESS
);

[[nodiscard]] SolutionResult solve_by_bisection(
    EquationType equation_type,
    double coefficient,
    double epsilon,
    double left_bound,
    double right_bound
);

[[nodiscard]] SolutionResult solve_by_newton(
    EquationType equation_type,
    double coefficient,
    double epsilon,
    double initial_guess = DEFAULT_INITIAL_GUESS
);

// Функции уравнений и их производных
[[nodiscard]] double evaluate_equation(
    EquationType equation_type,
    double x,
    double coefficient
);

[[nodiscard]] double evaluate_derivative(
    EquationType equation_type,
    double x,
    double coefficient
);

// Проверка доступности метода
[[nodiscard]] bool is_method_available(
    EquationType equation_type,
    SolutionMethod method
);

} // namespace EquationSolver

#endif // EQUATION_SOLVER_HPP




// #ifndef EQUATION_SOLVER_HPP
// #define EQUATION_SOLVER_HPP

// #include <string>

// namespace EquationSolver {

// // Константы
// constexpr double DEFAULT_INITIAL_GUESS = 0.5;
// constexpr double DEFAULT_COEFFICIENT = 1.0;
// constexpr double COS_4X_COEFFICIENT = 0.5;  // Для уравнения cos(4x) - 0.5*x = 0

// // Типы уравнений
// enum class EquationType {
//     X_MINUS_K_COS_X = 1,      // Старое: x - k*cos(x) = 0
//     COS_4X_MINUS_HALF_X = 2   // Новое: cos(4x) - 0.5*x = 0
// };

// // Методы решения
// enum class SolutionMethod {
//     ITERATION = 1,
//     BISECTION = 2,
//     NEWTON = 3,
//     EXIT = 4
// };

// // Результат вычисления
// struct SolutionResult {
//     double root;
//     int iterations;
//     bool success;
//     std::string error_message;
// };

// // Главная функция приложения
// void run_application();

// // Функции для работы с меню
// SolutionMethod display_menu(EquationType equation_type);
// bool should_continue();

// // Выбор уравнения
// EquationType select_equation_type();
// void solve_equation(EquationType equation_type);

// // Интерфейсные функции вычислений
// [[nodiscard]] SolutionResult solve_by_iteration(
//     EquationType equation_type,
//     double coefficient,
//     double epsilon,
//     double initial_guess = DEFAULT_INITIAL_GUESS
// );

// [[nodiscard]] SolutionResult solve_by_bisection(
//     EquationType equation_type,
//     double coefficient,
//     double epsilon,
//     double left_bound,
//     double right_bound
// );

// [[nodiscard]] SolutionResult solve_by_newton(
//     EquationType equation_type,
//     double coefficient,
//     double epsilon,
//     double initial_guess = DEFAULT_INITIAL_GUESS
// );

// // Функции уравнений и их производных
// [[nodiscard]] double evaluate_equation(
//     EquationType equation_type,
//     double x,
//     double coefficient
// );

// [[nodiscard]] double evaluate_derivative(
//     EquationType equation_type,
//     double x,
//     double coefficient
// );

// } // namespace EquationSolver

// #endif // EQUATION_SOLVER_HPP




// // #ifndef EQUATION_SOLVER_HPP
// // #define EQUATION_SOLVER_HPP

// // #include <string>
// // #include <vector>

// // namespace EquationSolver {

// // // Типы уравнений
// // enum class EquationType {
// //     X_MINUS_K_COS_X = 1,      // Старое: x - k*cos(x) = 0
// //     COS_4X_MINUS_HALF_X = 2   // Новое: cos(4x) - 0.5*x = 0
// // };

// // // Методы решения
// // enum class SolutionMethod {
// //     ITERATION = 1,
// //     BISECTION = 2,
// //     NEWTON = 3,
// //     EXIT = 4
// // };

// // // Результат вычисления
// // struct SolutionResult {
// //     double root;
// //     int iterations;
// //     bool success;
// //     std::string error_message;
// // };

// // // Главная функция приложения
// // void run_application();

// // // Функции для работы с меню
// // SolutionMethod display_menu(EquationType equation_type);
// // bool should_continue();

// // // Выбор уравнения
// // EquationType select_equation_type();
// // void solve_equation(EquationType equation_type);

// // // Интерфейсные функции вычислений
// // [[nodiscard]] SolutionResult solve_by_iteration(
// //     EquationType equation_type,
// //     double coefficient,
// //     double epsilon,
// //     double initial_guess = 0.5
// // );

// // [[nodiscard]] SolutionResult solve_by_bisection(
// //     EquationType equation_type,
// //     double coefficient,
// //     double epsilon,
// //     double left_bound,
// //     double right_bound
// // );

// // [[nodiscard]] SolutionResult solve_by_newton(
// //     EquationType equation_type,
// //     double coefficient,
// //     double epsilon,
// //     double initial_guess = 0.5
// // );

// // // Функции уравнений и их производных
// // [[nodiscard]] double evaluate_equation(
// //     EquationType equation_type,
// //     double x,
// //     double coefficient
// // );

// // [[nodiscard]] double evaluate_derivative(
// //     EquationType equation_type,
// //     double x,
// //     double coefficient
// // );

// // } // namespace EquationSolver

// // #endif // EQUATION_SOLVER_HPP



// // // #ifndef EQUATION_SOLVER_HPP
// // // #define EQUATION_SOLVER_HPP

// // // #include <string>
// // // #include <vector>

// // // namespace EquationSolver {

// // // // Типы уравнений
// // // enum class EquationType {
// // //     X_MINUS_K_COS_X = 1,      // Старое: x - k*cos(x) = 0
// // //     COS_4X_MINUS_HALF_X = 2   // Новое: cos(4x) - 0.5*x = 0
// // // };

// // // // Методы решения
// // // enum class SolutionMethod {
// // //     ITERATION = 1,
// // //     BISECTION = 2,
// // //     NEWTON = 3,
// // //     EXIT = 4
// // // };

// // // // Результат вычисления
// // // struct SolutionResult {
// // //     double root;
// // //     int iterations;
// // //     bool success;
// // //     std::string error_message;
// // // };

// // // // Главная функция приложения
// // // void run_application();

// // // // Функции для работы с меню
// // // SolutionMethod display_menu();
// // // bool should_continue();

// // // // Выбор уравнения
// // // EquationType select_equation_type();
// // // void solve_equation(EquationType equation_type);

// // // // Интерфейсные функции вычислений
// // // [[nodiscard]] SolutionResult solve_by_iteration(
// // //     EquationType equation_type,
// // //     double coefficient,
// // //     double epsilon,
// // //     double initial_guess = 0.5
// // // );

// // // [[nodiscard]] SolutionResult solve_by_bisection(
// // //     EquationType equation_type,
// // //     double coefficient,
// // //     double epsilon,
// // //     double left_bound,
// // //     double right_bound
// // // );

// // // [[nodiscard]] SolutionResult solve_by_newton(
// // //     EquationType equation_type,
// // //     double coefficient,
// // //     double epsilon,
// // //     double initial_guess = 0.5
// // // );

// // // // Функции уравнений и их производных
// // // [[nodiscard]] double evaluate_equation(
// // //     EquationType equation_type,
// // //     double x,
// // //     double coefficient
// // // );

// // // [[nodiscard]] double evaluate_derivative(
// // //     EquationType equation_type,
// // //     double x,
// // //     double coefficient
// // // );

// // // // Проверка доступности метода для уравнения
// // // [[nodiscard]] bool is_method_available(
// // //     EquationType equation_type,
// // //     SolutionMethod method
// // // );

// // // } // namespace EquationSolver

// // // #endif // EQUATION_SOLVER_HPP


// // // // #ifndef EQUATION_SOLVER_H
// // // // #define EQUATION_SOLVER_H

// // // // namespace EquationSolver {

// // // // enum class MenuOption {
// // // //     ITERATION_METHOD = 1,
// // // //     BISECTION_METHOD,
// // // //     NEWTON_METHOD,
// // // //     EXIT
// // // // };

// // // // void runApplication();
// // // // MenuOption displayMenu();
// // // // bool shouldContinue();
// // // // void executeTask(MenuOption option);

// // // // void executeTask2(MenuOption option);

// // // // [[nodiscard]] double iterationMethod(double coefficient, double epsilon, int& iterations);
// // // // [[nodiscard]] double bisectionMethod(double coefficient, double left, double right, double epsilon, int& iterations);
// // // // [[nodiscard]] double newtonMethod(double coefficient, double epsilon, int& iterations);

// // // // [[nodiscard]] double function(double x, double coefficient);
// // // // [[nodiscard]] double derivative(double x, double coefficient);

// // // // [[nodiscard]] double iterationMethod2(double coefficient, double epsilon, int& iterations);
// // // // [[nodiscard]] double bisectionMethod2(double coefficient, double left, double right, double epsilon, int& iterations);
// // // // [[nodiscard]] double newtonMethod2(double coefficient, double epsilon, int& iterations);

// // // // [[nodiscard]] double function2(double x, double coefficient);
// // // // [[nodiscard]] double derivative2(double x, double coefficient);

// // // // }

// // // // #endif



// // // // // #ifndef EQUATION_SOLVER_H
// // // // // #define EQUATION_SOLVER_H

// // // // // namespace EquationSolver {

// // // // // enum class MenuOption {
// // // // //     ITERATION_METHOD = 1,
// // // // //     BISECTION_METHOD,
// // // // //     NEWTON_METHOD,
// // // // //     EXIT
// // // // // };

// // // // // void runApplication();
// // // // // MenuOption displayMenu();
// // // // // bool shouldContinue();
// // // // // void executeTask(MenuOption option);

// // // // // void executeTask2(MenuOption option);

// // // // // [[nodiscard]] double iterationMethod(double coefficient, double epsilon, int& iterations);
// // // // // [[nodiscard]] double bisectionMethod(double coefficient, double left, double right, double epsilon, int& iterations);
// // // // // [[nodiscard]] double newtonMethod(double coefficient, double epsilon, int& iterations);

// // // // // [[nodiscard]] double function(double x, double coefficient);
// // // // // [[nodiscard]] double derivative(double x, double coefficient);

// // // // // [[nodiscard]] double iterationMethod2(double coefficient, double epsilon, int& iterations, int choice);
// // // // // [[nodiscard]] double bisectionMethod2(double coefficient, double left, double right, double epsilon, int& iterations, int choice);
// // // // // [[nodiscard]] double newtonMethod2(double coefficient, double epsilon, int& iterations);

// // // // // [[nodiscard]] double function2(double x, double coefficient);
// // // // // [[nodiscard]] double derivative2(double x, double coefficient);

// // // // // }

// // // // // #endif


// // // // // // #ifndef EQUATION_SOLVER_H
// // // // // // #define EQUATION_SOLVER_H

// // // // // // namespace EquationSolver {

// // // // // // enum class MenuOption {
// // // // // //     ITERATION_METHOD = 1,
// // // // // //     BISECTION_METHOD,
// // // // // //     NEWTON_METHOD,
// // // // // //     EXIT
// // // // // // };

// // // // // // void runApplication();
// // // // // // MenuOption displayMenu();
// // // // // // bool shouldContinue();
// // // // // // void executeTask(MenuOption option);

// // // // // // [[nodiscard]] double iterationMethod(double coefficient, double epsilon, int& iterations);
// // // // // // [[nodiscard]] double bisectionMethod(double coefficient, double left, double right, double epsilon, int& iterations);
// // // // // // [[nodiscard]] double newtonMethod(double coefficient, double epsilon, int& iterations);

// // // // // // [[nodiscard]] double function(double x, double coefficient);
// // // // // // [[nodiscard]] double derivative(double x, double coefficient);

// // // // // // }

// // // // // // #endif
