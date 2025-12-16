#ifndef EQUATION_SOLVER_H
#define EQUATION_SOLVER_H

namespace EquationSolver {

enum class MenuOption {
    ITERATION_METHOD = 1,
    BISECTION_METHOD,
    NEWTON_METHOD,
    EXIT
};

void runApplication();
MenuOption displayMenu();
bool shouldContinue();
void executeTask(MenuOption option);

void executeTask2(MenuOption option);

[[nodiscard]] double iterationMethod(double coefficient, double epsilon, int& iterations);
[[nodiscard]] double bisectionMethod(double coefficient, double left, double right, double epsilon, int& iterations);
[[nodiscard]] double newtonMethod(double coefficient, double epsilon, int& iterations);

[[nodiscard]] double function(double x, double coefficient);
[[nodiscard]] double derivative(double x, double coefficient);

[[nodiscard]] double iterationMethod2(double coefficient, double epsilon, int& iterations);
[[nodiscard]] double bisectionMethod2(double coefficient, double left, double right, double epsilon, int& iterations);
[[nodiscard]] double newtonMethod2(double coefficient, double epsilon, int& iterations);

[[nodiscard]] double function2(double x, double coefficient);
[[nodiscard]] double derivative2(double x, double coefficient);

}

#endif



// #ifndef EQUATION_SOLVER_H
// #define EQUATION_SOLVER_H

// namespace EquationSolver {

// enum class MenuOption {
//     ITERATION_METHOD = 1,
//     BISECTION_METHOD,
//     NEWTON_METHOD,
//     EXIT
// };

// void runApplication();
// MenuOption displayMenu();
// bool shouldContinue();
// void executeTask(MenuOption option);

// void executeTask2(MenuOption option);

// [[nodiscard]] double iterationMethod(double coefficient, double epsilon, int& iterations);
// [[nodiscard]] double bisectionMethod(double coefficient, double left, double right, double epsilon, int& iterations);
// [[nodiscard]] double newtonMethod(double coefficient, double epsilon, int& iterations);

// [[nodiscard]] double function(double x, double coefficient);
// [[nodiscard]] double derivative(double x, double coefficient);

// [[nodiscard]] double iterationMethod2(double coefficient, double epsilon, int& iterations, int choice);
// [[nodiscard]] double bisectionMethod2(double coefficient, double left, double right, double epsilon, int& iterations, int choice);
// [[nodiscard]] double newtonMethod2(double coefficient, double epsilon, int& iterations);

// [[nodiscard]] double function2(double x, double coefficient);
// [[nodiscard]] double derivative2(double x, double coefficient);

// }

// #endif


// // #ifndef EQUATION_SOLVER_H
// // #define EQUATION_SOLVER_H

// // namespace EquationSolver {

// // enum class MenuOption {
// //     ITERATION_METHOD = 1,
// //     BISECTION_METHOD,
// //     NEWTON_METHOD,
// //     EXIT
// // };

// // void runApplication();
// // MenuOption displayMenu();
// // bool shouldContinue();
// // void executeTask(MenuOption option);

// // [[nodiscard]] double iterationMethod(double coefficient, double epsilon, int& iterations);
// // [[nodiscard]] double bisectionMethod(double coefficient, double left, double right, double epsilon, int& iterations);
// // [[nodiscard]] double newtonMethod(double coefficient, double epsilon, int& iterations);

// // [[nodiscard]] double function(double x, double coefficient);
// // [[nodiscard]] double derivative(double x, double coefficient);

// // }

// // #endif
