#include "lab_3_functions.h"

int main() {
    EquationSolver::runApplication();
    return 0;
}


// #include "lab_3_functions.h"
// #include <iostream>

// using namespace std;

// int main() {
//   double x;
//   cout << "Введите x (0.1 <= x <= 1): ";
//   cin >> x;

//   if (x < 0.1 || x > 1) {
//     cout << "Ошибка: x должен быть в диапазоне [0.1, 1]" << endl;
//     return 1;
//   }

//   double epsilon;
//   cout << "Введите точность ε: ";
//   cin >> epsilon;

//   // Вычисление суммы ряда
//   double sum = calculateSeries(x, epsilon);

//   // Вычисление точного значения
//   double exact_value = calculateExact(x);

//   // Вывод результатов
//   printResults(sum, exact_value);

//   return 0;
// }
