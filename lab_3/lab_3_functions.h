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

[[nodiscard]] double iterationMethod(double coefficient, double epsilon, int& iterations);
[[nodiscard]] double bisectionMethod(double coefficient, double left, double right, double epsilon, int& iterations);
[[nodiscard]] double newtonMethod(double coefficient, double epsilon, int& iterations);

[[nodiscard]] double function(double x, double coefficient);
[[nodiscard]] double derivative(double x, double coefficient);

}

#endif


// #ifndef FUNCTIONS_H
// #define FUNCTIONS_H

// double calculateSeries(double x, double epsilon);

// double calculateExact(double x);

// void printResults(double sum, double exact_value);

// #endif
