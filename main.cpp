#include <iostream>

#include "lab_3_functions.h"

int main() {
    try {
        EquationSolver::run_application();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Неизвестная критическая ошибка" << std::endl;
        return 1;
    }
}
